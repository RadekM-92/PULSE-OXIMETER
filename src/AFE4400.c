
#include <stdio.h>
#include <string.h>

#include "AFE4400_TxStage.h"
#include "AFE4400_RxStage.h"
#include "AFE4400_Timers.h"
#include "AFE4400_Maintenance.h"

#ifndef STM32F103
#define STM32F103
#endif

#ifdef STM32F103
#include "stm32f1xx_hal.h"
SPI_HandleTypeDef hspi2;
#endif


#define AFE4400_PD_ALM_Pin GPIO_PIN_4
#define AFE4400_PD_ALM_GPIO_Port GPIOC
#define AFE4400_CS_Pin GPIO_PIN_1
#define AFE4400_CS_GPIO_Port GPIOB
#define AFE4400_ADC_RDY_Pin GPIO_PIN_2
#define AFE4400_ADC_RDY_GPIO_Port GPIOB
#define AFE4400_ADC_RDY_EXTI_IRQn EXTI2_IRQn
#define AFE4400_RST_Pin GPIO_PIN_12
#define AFE4400_RST_GPIO_Port GPIOB
#define AFE4400_PDN_Pin GPIO_PIN_10
#define AFE4400_PDN_GPIO_Port GPIOA
#define AFE4400_LED_ALM_Pin GPIO_PIN_4
#define AFE4400_LED_ALM_GPIO_Port GPIOB
#define AFE4400_DIAG_END_Pin GPIO_PIN_5
#define AFE4400_DIAG_END_GPIO_Port GPIOB


#define AFE4400_Delay(x)  HAL_Delay(x)
#define AFE4400_HwPowerUp()  \
  HAL_GPIO_WritePin(AFE4400_PDN_GPIO_Port, AFE4400_PDN_Pin, GPIO_PIN_SET);  \
  AFE4400_Delay(1000);
#define AFE4400_PowerDown()  HAL_GPIO_WritePin(AFE4400_PDN_GPIO_Port, AFE4400_PDN_Pin, GPIO_PIN_RESET);
#define AFE4400_RST() \
  HAL_GPIO_WritePin(AFE4400_RST_GPIO_Port, AFE4400_RST_Pin, GPIO_PIN_RESET);  \
  AFE4400_Delay(20);  \
  HAL_GPIO_WritePin(AFE4400_RST_GPIO_Port, AFE4400_RST_Pin, GPIO_PIN_SET);  \
  AFE4400_Delay(20);
#define AFE4400_CS_Enable() \
  HAL_GPIO_WritePin(AFE4400_CS_GPIO_Port, AFE4400_CS_Pin, GPIO_PIN_RESET);
#define AFE4400_CS_Disable() \
  HAL_GPIO_WritePin(AFE4400_CS_GPIO_Port, AFE4400_CS_Pin, GPIO_PIN_SET);




AFE4400_Data_t AFE4400_Data = {0};                /** AFE4400 All registers data */
AFE4400_Parameters_t AFE4400_Parameters = {0};    /** AFE4400 - Parameters */
AFE4400_LEDs_RealDataADC_t AFE4400_LEDs = {0};    /** AFE4400 - LEDs real ADC Data */




/** AFE4400 Init */
extern void AFE4400_Init(void)
{
    ParametersInit(&AFE4400_Parameters);
    AFE4400_CS_Disable();
    AFE4400_HwPowerUp();
    AFE4400_RST();
    TxInit();
    RxInit();
    TimerModuleInit();
}

uint8_t AFE4400_Write(AFE4400_REGS_ADDRESS_t Address, const uint32_t *Data, uint8_t Size)
{
    uint8_t WriteEnBuf[] = {CONTROL0, 0x00, 0x00, 0x00};
    uint8_t TxBuf[4] = {0};
    uint8_t i, n;
    int8_t WriteStatus = 0U;

    AFE4400_CS_Enable();

    if (HAL_OK != HAL_SPI_Transmit(&hspi2, (uint8_t*) WriteEnBuf, sizeof(WriteEnBuf), 100))
    {
        WriteStatus = 1U;
    }

    for(n=0; n<Size; n++)
    {
        for(i=0; i<sizeof(TxBuf); i++)
        {
            if (0 == i)
            {
                TxBuf[i] = Address + n;
                continue;
            }     
            
            TxBuf[i] = (uint8_t)(Data[n] >> (24 - 8*i));  
        }

        if (HAL_OK != HAL_SPI_Transmit(&hspi2, (uint8_t*) TxBuf, sizeof(TxBuf), 100))
    {
        WriteStatus = 1U;
    }
    }
    
    AFE4400_CS_Disable();

    return WriteStatus;
}

uint8_t AFE4400_Read(AFE4400_REGS_ADDRESS_t Address, uint32_t *Data, uint8_t Size)
{
    uint8_t RegAdd;
    uint8_t ReadEnBuf[] = {CONTROL0, 0x00, 0x00, SPI_READ};
    uint8_t RxBuf[3] = {0};
    uint8_t i, n, k;
    uint8_t ReadStatus = 0U;
    uint8_t WriteStatus = 0U;

    AFE4400_CS_Enable();

    WriteStatus = HAL_SPI_Transmit(&hspi2, (uint8_t*) ReadEnBuf, sizeof(ReadEnBuf), 100);
    
    if (HAL_OK == WriteStatus)
    {
        for(n=0; n<Size; n++)
        {
            RegAdd = (uint8_t) (Address + n);
            WriteStatus = HAL_SPI_Transmit(&hspi2, (uint8_t*) &RegAdd, 1U, 100);
            if (HAL_OK == WriteStatus)
            {
                memset(RxBuf, 0x00, sizeof(RxBuf));
                ReadStatus = HAL_SPI_Receive(&hspi2, (uint8_t*) RxBuf, sizeof(RxBuf), 100);
                if (HAL_OK == ReadStatus)
                {
                    Data[n] = 0;
                    for(i=sizeof(RxBuf), k=0; i>0; i--, k++)
                    {
                        Data[n] |= RxBuf[i-1] << 8*k;
                    }
                }
            }
        }
    }

    AFE4400_CS_Disable();

    return (0 == ReadStatus && 0 == WriteStatus)? 0U : 1U;
}

extern uint8_t AFE4400_ConvertMeasurement(void)
{
    LEDs_RealDataADC_Update(&AFE4400_Data, &AFE4400_LEDs);
}

extern uint8_t AFE4400_ReadMeasurement(void)
{
    return AFE4400_Read(LED2VAL, &AFE4400_Data.LED2VAL, 6U);
}
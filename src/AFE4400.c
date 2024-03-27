
#include <stdio.h>
#include <string.h>

#include "AFE4400.h"
#include "AFE4400_Types.h"

#ifndef STM32F103
#define STM32F103
#endif

#ifdef STM32F103
#include "stm32f1xx_hal.h"
SPI_HandleTypeDef hspi2;
#endif

AFE4400_Data_t AFE4400_Data = {0};                /** AFE4400 All registers data */
AFE4400_Parameters_t AFE4400_Parameters = {0};    /** AFE4400 - Parameters */
AFE4400_LEDs_RealDataADC_t AFE4400_LEDs = {0};    /** AFE4400 - LEDs real ADC Data */




/** AFE4400 Init */
void AFE4400_Init(void)
{
    ParametersInit(&AFE4400_Parameters);

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

 

    return (0 == ReadStatus && 0 == WriteStatus)? 0U : 1U;
}
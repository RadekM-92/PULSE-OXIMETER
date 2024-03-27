
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
    
}
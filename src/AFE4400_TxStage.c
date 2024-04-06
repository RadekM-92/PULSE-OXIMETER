#include "AFE4400_TxStage.h"


/** Tx Stage Init*/
extern void TxInit(void)
{
    TxLedsCurrentInit(&AFE4400_Parameters, &AFE4400_Data);

    AFE4400_Write(LEDCNTRL, &AFE4400_Data.LEDCNTRL, 1U);

    AFE4400_Data.CONTROL2 |= (1<<8);
    AFE4400_Write(CONTROL2, &AFE4400_Data.CONTROL2, 1U);

    AFE4400_Data.ALARM |= (1<<7);
    AFE4400_Write(ALARM, &AFE4400_Data.ALARM, 1U);
}

/** Tx LEDs current init
 * Parameters - configuration parameters
 * Data - AFE4400 registers
*/
static void TxLedsCurrentInit(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data)
{
    const uint32_t LED1_Mask = 0xffff00ff;
    const uint32_t LED2_Mask = 0xffffff00;
    uint8_t LED1_CurrentRaw;
    uint8_t LED2_CurrentRaw;

    LED1_CurrentRaw = LED_Current_mA_to_Raw(Parameters->LED1_Current_mA);
    LED2_CurrentRaw = LED_Current_mA_to_Raw(Parameters->LED2_Current_mA);
    
    if (0 != LED1_CurrentRaw && 0 != LED2_CurrentRaw)
    {
        Data->LEDCNTRL &= (LED1_Mask & LED2_Mask);
        Data->LEDCNTRL |= ((LED1_CurrentRaw << 8) | LED2_CurrentRaw);
        Data->LEDCNTRL |= LEDCUROFF;
        Data->LEDCNTRL |= (1<<16);
    }
    else
    {
        Data->LEDCNTRL &= ~LEDCUROFF;
    }

}

/** LED current calculate
 * LedCurrent - 0..50 [mA]
 * Return value from 0 to 256
*/
static uint8_t LED_Current_mA_to_Raw(uint8_t LedCurrent)
{
    uint16_t LedCurrentTMP;

    if((0 <= LedCurrent) && (LED_CurrentMax_mA >= LedCurrent))
    {
        LedCurrentTMP = (LedCurrent * LED_CurrentMax_Raw) / LED_CurrentMax_mA;

        return LedCurrentTMP;
    }
    else
    {
        return 0;
    }
}





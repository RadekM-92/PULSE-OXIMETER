

#include "AFE4400.h"



AFE4400_Data_t AFE4400_Data;    /** AFE4400 All registers data */


/** Software reset - resets all internal registers to the default values */
static void SoftwareReset(AFE4400_Data_t *Data)
{
    Data->CONTROL0 |= SW_RST;
}

/** Diagnostic enable - At the end of the sequence, all fault status are stored in the DIAG register */
static void DiagnosticEnable(AFE4400_Data_t *Data)
{
    Data->CONTROL0 |= DIAG_EN;
}

/**
 * Timer module initialization function
 * PRF - Pulse Repetition Period [Hz]
 * DutyCycle - 0..100[%]
*/
static void TimerModuleInit( uint16_t PRF , uint8_t DutyCycle, AFE4400_Data_t *Data)
{
   if (0 != PRF)
   {
        Data->PRPCOUNT = (uint16_t *)(AFE4400_CLOCK_FRQ / PRF - 1);
        Data->ADCRSTENDCT0 = Data->PRPCOUNT;
   }

   ADC_TimersInit(PRF, DutyCycle, &Data);

    
}

/** ADC TimersInit 
 * PRF - Pulse Repetition Period [Hz]
 * DutyCycle - 0..100[%]
 * 
*/
static void ADC_TimersInit(uint8_t PRF, uint8_t DutyCycle, AFE4400_Data_t *Data)
{
    if ((0 != PRF) && (0 < DutyCycle) && (100 > DutyCycle))
    {
       Data->ADCRSTSTCT0 = 0;
       Data->ADCRSTENDCT0 = (uint16_t *)(Data->ADCRSTSTCT0 + ADC_Reset_ClockCycle);
       Data->ADCRSTSTCT1 = (uint16_t *)( Data->ADCRSTSTCT0 + ((AFE4400_CLOCK_FRQ / PRF) * (DutyCycle / 100)));
       Data->ADCRSTENDCT1 = (uint16_t *)( Data->ADCRSTSTCT1 + ADC_Reset_ClockCycle);
       Data->ADCRSTSTCT2 = (uint16_t *)( Data->ADCRSTSTCT1 + ((AFE4400_CLOCK_FRQ / PRF) * (DutyCycle / 100)));
       Data->ADCRSTENDCT2 = (uint16_t *)( Data->ADCRSTSTCT2 + ADC_Reset_ClockCycle);
       Data->ADCRSTSTCT3 = (uint16_t *)( Data->ADCRSTSTCT2 + ((AFE4400_CLOCK_FRQ / PRF) * (DutyCycle / 100)));
       Data->ADCRSTENDCT3 = (uint16_t *)( Data->ADCRSTSTCT3 + ADC_Reset_ClockCycle);
    }
}


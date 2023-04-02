

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
 * PRF - Pulse Repetition Frequency [Hz]
 * DutyCycle - 0..100[%]
*/
static void TimerModuleInit( uint16_t PRF , uint8_t DutyCycle, AFE4400_Data_t *Data)
{
   if ((0 != PRF) && (0 < DutyCycle) && (100 > DutyCycle))
   {
        PRP_TimerInit(PRF, &Data);
        ADC_TimersInit(PRF, DutyCycle, &Data);
        Convert_TimersInit(&Data);
        Sample_TimersInit(&Data);
        LedPulse_TimersInit(&Data);
   }
}

/** TimerModuleInit - check parameters
 *  1-OK, 0-NOK
 *  PRF - Pulse Repetition Frequency [Hz]
 *  DutyCycle - 0..100[%]
*/
static uint8_t IsTimerModuleParamOk( uint16_t PRF , uint8_t DutyCycle)
{
     if ((0 != PRF) && (0 < DutyCycle) && (100 > DutyCycle))
     {
        return 1;
     }
     else
     {
        return 0;
     }
}

/** Pulse Repetition Frequency Timer Init
 * PRF - Pulse Repetition Frequency [Hz]
*/
static void PRP_TimerInit(uint16_t PRF, AFE4400_Data_t *Data)
{
     Data->PRPCOUNT = (uint16_t *)(AFE4400_CLOCK_FRQ / PRF - 1);
}

/** ADC TimersInit 
 * PRF - Pulse Repetition Frequency [Hz]
 * DutyCycle - 0..100[%]
 * 
*/
static void ADC_TimersInit(uint8_t PRF, uint8_t DutyCycle, AFE4400_Data_t *Data)
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

/** Convert TimersInit
* ADC_TimersInit() shall be call before call Convert_TimersInit()
*/
static void Convert_TimersInit(AFE4400_Data_t *Data)
{
    Data->LED2CONVST = (uint16_t *)(Data->ADCRSTENDCT0 + 1);
    Data->LED2CONVEND = (uint16_t *)(Data->ADCRSTSTCT1 - 1);
    Data->ALED2CONVST = (uint16_t *)(Data->ADCRSTENDCT1 + 1);
    Data->ALED2CONVEND = (uint16_t *)(Data->ADCRSTSTCT2 - 1);
    Data->LED1CONVST = (uint16_t *)(Data->ADCRSTENDCT2 + 1);
    Data->LED1CONVEND = (uint16_t *)(Data->ADCRSTSTCT3 - 1);
    Data->ALED1CONVST = (uint16_t *)(Data->ADCRSTENDCT3 + 1);
    Data->ALED1CONVEND = Data->PRPCOUNT;
}

/** Sample TimersInit
 *  ADC_TimersInit() shall be call before call Sample_TimersInit()
*/
static void Sample_TimersInit(AFE4400_Data_t *Data)
{
    Data->ALED2STC = (uint16_t *)(Data->ADCRSTSTCT0 + 50);
    Data->ALED2ENDC = (uint16_t *)(Data->ADCRSTSTCT1 - 2);

    Data->LED1STC = (uint16_t *)(Data->ADCRSTSTCT1 + 50);
    Data->LED1ENDC = (uint16_t *)(Data->ADCRSTSTCT2 - 2);

    Data->ALED1STC = (uint16_t *)(Data->ADCRSTSTCT2 + 50);
    Data->ALED1ENDC = (uint16_t *)(Data->ADCRSTSTCT3 - 2);
    
    Data->LED2STC = (uint16_t *)(Data->ADCRSTSTCT3 + 50);
    Data->LED2ENDC = (uint16_t *)(Data->PRPCOUNT - 1);
}

/** LED pulse Timers Init 
 *  ADC_TimersInit() shall be call before call Sample_TimersInit()
*/
static void LedPulse_TimersInit(AFE4400_Data_t *Data)
{
    Data->LED2LEDSTC = (uint16_t *)(Data->ADCRSTSTCT3);
    Data->LED2LEDENDC = (uint16_t *)(Data->ALED1CONVEND);
    Data->LED1LEDSTC = (uint16_t *)(Data->ADCRSTSTCT1);
    Data->LED1LEDENDC = (uint16_t *)(Data->ALED2CONVEND);
}



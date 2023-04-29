


#include "AFE4400.h"
#include "AFE4400_Types.h"



/** TimerModuleInit - check parameters */
static uint8_t IsTimerModuleParamOk(const AFE4400_Parameters_t *Parameters);

/** Pulse Repetition Frequency Timer Init */
static void PRP_TimerInit(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data);

/** ADC TimersInit */
static void ADC_TimersInit(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data);

/** Convert TimersInit */
static void Convert_TimersInit(AFE4400_Data_t *Data);

/** Sample TimersInit
 *  ADC_TimersInit() shall be call before call Sample_TimersInit()
*/
static void Sample_TimersInit(AFE4400_Data_t *Data);

/** LED pulse Timers Init 
 *  ADC_TimersInit() shall be call before call Sample_TimersInit()
*/
static void LedPulse_TimersInit(AFE4400_Data_t *Data);



/** Timer module initialization function
* Parameters - configuration parameters
* Data - AFE4400 registers
*/
extern void TimerModuleInit(void)
{
   if (IsTimerModuleParamOk(&AFE4400_Parameters))
   {
        PRP_TimerInit(&AFE4400_Parameters, &AFE4400_Data);
        ADC_TimersInit(&AFE4400_Parameters, &AFE4400_Data);
        Convert_TimersInit(&AFE4400_Data);
        Sample_TimersInit(&AFE4400_Data);
        LedPulse_TimersInit(&AFE4400_Data);
   }
   else
   {
    ;
   }
}

/** Pulse Repetition Frequency Timer Init
* Parameters - configuration parameters
* Data - AFE4400 registers
*/
static void PRP_TimerInit(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data)
{
     Data->PRPCOUNT = (AFE4400_CLOCK_FRQ / Parameters->PRF - 1);
}


/** TimerModuleInit - check parameters
* Parameters - configuration parameters
*/
static uint8_t IsTimerModuleParamOk(const AFE4400_Parameters_t *Parameters)
{
     if ((0 != Parameters->PRF) && (0 < Parameters->DutyCycle) && (25 >= Parameters->DutyCycle))
     {
        return 1;
     }
     else
     {
        return 0;
     }
}

/** ADC TimersInit 
 * Parameters - configuration parameters
 * Data - AFE4400 registers
 * 
*/
static void ADC_TimersInit(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data)
{
    Data->ADCRSTSTCT0 = 0;
    Data->ADCRSTENDCT0  = ( Data->ADCRSTSTCT0 + ADC_Reset_ClockCycle);
    Data->ADCRSTSTCT1   = ( Data->ADCRSTSTCT0 + ((AFE4400_CLOCK_FRQ / Parameters->PRF) * Parameters->DutyCycle) / 100 );
    Data->ADCRSTENDCT1  = ( Data->ADCRSTSTCT1 + ADC_Reset_ClockCycle);
    Data->ADCRSTSTCT2   = ( Data->ADCRSTSTCT1 + ((AFE4400_CLOCK_FRQ / Parameters->PRF) * Parameters->DutyCycle) / 100 );
    Data->ADCRSTENDCT2  = ( Data->ADCRSTSTCT2 + ADC_Reset_ClockCycle);
    Data->ADCRSTSTCT3   = ( Data->ADCRSTSTCT2 + ((AFE4400_CLOCK_FRQ / Parameters->PRF) * Parameters->DutyCycle) / 100 );
    Data->ADCRSTENDCT3  = ( Data->ADCRSTSTCT3 + ADC_Reset_ClockCycle);
}

/** Convert TimersInit
* ADC_TimersInit() shall be call before call Convert_TimersInit()
*/
static void Convert_TimersInit(AFE4400_Data_t *Data)
{
    Data->LED2CONVST    = (Data->ADCRSTENDCT0 + 1);
    Data->LED2CONVEND   = (Data->ADCRSTSTCT1 - 1);
    Data->ALED2CONVST   = (Data->ADCRSTENDCT1 + 1);
    Data->ALED2CONVEND  = (Data->ADCRSTSTCT2 - 1);
    Data->LED1CONVST    = (Data->ADCRSTENDCT2 + 1);
    Data->LED1CONVEND   = (Data->ADCRSTSTCT3 - 1);
    Data->ALED1CONVST   = (Data->ADCRSTENDCT3 + 1);
    Data->ALED1CONVEND  = Data->PRPCOUNT;
}

/** Sample TimersInit
 *  ADC_TimersInit() shall be call before call Sample_TimersInit()
*/
static void Sample_TimersInit(AFE4400_Data_t *Data)
{
    Data->ALED2STC  = (Data->ADCRSTSTCT0 + 50);
    Data->ALED2ENDC = (Data->ADCRSTSTCT1 - 2);

    Data->LED1STC   = (Data->ADCRSTSTCT1 + 50);
    Data->LED1ENDC  = (Data->ADCRSTSTCT2 - 2);

    Data->ALED1STC  = (Data->ADCRSTSTCT2 + 50);
    Data->ALED1ENDC = (Data->ADCRSTSTCT3 - 2);
    
    Data->LED2STC   = (Data->ADCRSTSTCT3 + 50);
    Data->LED2ENDC  = (Data->PRPCOUNT - 1);
}

/** LED pulse Timers Init 
 *  ADC_TimersInit() shall be call before call Sample_TimersInit()
*/
static void LedPulse_TimersInit(AFE4400_Data_t *Data)
{
    Data->LED2LEDSTC    = (Data->ADCRSTSTCT3);
    Data->LED2LEDENDC   = (Data->ALED1CONVEND);
    Data->LED1LEDSTC    = (Data->ADCRSTSTCT1);
    Data->LED1LEDENDC   = (Data->ALED2CONVEND);
}



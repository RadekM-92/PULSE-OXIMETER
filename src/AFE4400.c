
#include "AFE4400.h"
#include "AFE4400_Types.h"

AFE4400_Data_t AFE4400_Data = {0};                /** AFE4400 All registers data */
AFE4400_Parameters_t AFE4400_Parameters = {0};    /** AFE4400 - Parameters */
AFE4400_LEDs_RealDataADC_t AFE4400_LEDs = {0};    /** AFE4400 - LEDs real ADC Data */


/** Software reset - resets all internal registers to the default values */
static void SoftwareReset(AFE4400_Data_t *Data);

/** Diagnostic enable - At the end of the sequence, all fault status are stored in the DIAG register */
static void DiagnosticEnable(AFE4400_Data_t *Data);

/** Timer module initialization function */
static void TimerModuleInit(void);

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

/** AFE4400 Power down */
static void AFE4400_PowerDown(AFE4400_Data_t *Data);

/** AFE4400 Power Up */
static void AFE4400_PowerUp(AFE4400_Data_t *Data);



/** Rx low pass filter init */
static void RxLowPassFilterInit(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data);

/** Rx Stage 2 init */
static void RxStage2Init(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data);

/** Rx Stage Init */
static void RxInit(void);

void ParametersInit(AFE4400_Parameters_t *Parameters);

/** Two's complement to decimal conversion */
int32_t TwosCompToDec(uint32_t TwosVal, uint32_t n_bits);

/** ADC scaling 
 * -2097152 -> -1.2[V]
 * 2097151  ->  1.2[V]
*/
float ADC_RawToReal(int32_t ADC_RawVal);

/** LEDs real ADC measurement data update */
void LEDs_RealDataADC_Update(const AFE4400_Data_t *Data, AFE4400_LEDs_RealDataADC_t *LEDs);





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

/** Timer module initialization function
* Parameters - configuration parameters
* Data - AFE4400 registers
*/
static void TimerModuleInit(void)
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

/** Pulse Repetition Frequency Timer Init
* Parameters - configuration parameters
* Data - AFE4400 registers
*/
static void PRP_TimerInit(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data)
{
     Data->PRPCOUNT = (AFE4400_CLOCK_FRQ / Parameters->PRF - 1);
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

/** AFE4400 Power down */
static void AFE4400_PowerDown(AFE4400_Data_t *Data)
{
    AFE4400_Data.CONTROL2 |= PDN_AFE;
}

/** AFE4400 Power Up */
static void AFE4400_PowerUp(AFE4400_Data_t *Data)
{
    AFE4400_Data.CONTROL2 &= ~PDN_AFE;
}





/** Rx low pass filter init */
static void RxLowPassFilterInit(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data)
{
    const uint32_t RF_Mask = 0xFFFFFFF8;
    const uint32_t CF_Mask = 0xFFFFFF07;

    Data->TIA_AMB_GAIN &= (RF_Mask & CF_Mask);
    Data->TIA_AMB_GAIN |= ((Parameters->Cf<<3) | (Parameters->Rf));
}

/** Rx Stage 2 init */
static void RxStage2Init(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data)
{
   const uint32_t SG2GAIN_Mask   = 0xFFFFF8FF;
   const uint32_t AMBDAC_Mask    = 0xFFF8FFFF;

    if (0 <= Parameters->CancellationCurrent && RxStage2_CurrentMax >= Parameters->CancellationCurrent)
    {
         Data->TIA_AMB_GAIN &= AMBDAC_Mask;
         Data->TIA_AMB_GAIN |= (Parameters->CancellationCurrent<<16);
    }

    if (0 <= Parameters->STG2_GAIN && RxStage2_MaxGain >= Parameters->STG2_GAIN)
    {
        Data->TIA_AMB_GAIN &= SG2GAIN_Mask;
        Data->TIA_AMB_GAIN |= (Parameters->STG2_GAIN<<8);
        Data->TIA_AMB_GAIN |= STAGE2EN;
    }
}

/** Rx Stage Init */
static void RxInit(void)
{
    RxLowPassFilterInit(&AFE4400_Parameters, &AFE4400_Data);
    RxStage2Init(&AFE4400_Parameters, &AFE4400_Data);
}

/** AFE4400 Init */
void AFE4400_Init(void)
{
    ParametersInit(&AFE4400_Parameters);

    TxInit();
    RxInit();
    TimerModuleInit();
}

void ParametersInit(AFE4400_Parameters_t *Parameters)
{
    Parameters->PRF = 500;
    Parameters->DutyCycle = 25;

    Parameters->LED1_Current_mA = 20;
    Parameters->LED2_Current_mA = 20;

    Parameters->Cf = Cf_25pF;
    Parameters->Rf = Rf_10k;

    Parameters->CancellationCurrent = 5;
    Parameters->STG2_GAIN = 3;
}

/** Two's complement to decimal conversion */
int32_t TwosCompToDec(uint32_t TwosVal, uint32_t n_bits)
{
    uint32_t sign_mask = 1 << (n_bits - 1);

    if (TwosVal & sign_mask)
    {
        uint32_t mask_neg = ~((1 << n_bits) - 1);
        return (int32_t)(TwosVal | mask_neg);
    }
    else
    {
        return TwosVal;
    }
}

/** ADC scaling
 * -2097152 -> -1.2[V]
 * 2097151  ->  1.2[V]
*/
float ADC_RawToReal(int32_t ADC_RawVal)
{
    const int16_t Raw_10mV = 17476;

    return (float)(ADC_RawVal / Raw_10mV) / 100.0f;
}

/** LEDs real ADC measurement data update */
void LEDs_RealDataADC_Update(const AFE4400_Data_t *Data, AFE4400_LEDs_RealDataADC_t *LEDs)
{
    const uint8_t ADC_22bit = 22;

    LEDs->LED2_On       = ADC_RawToReal(TwosCompToDec(Data->LED2VAL, ADC_22bit));
    LEDs->LED2_Ambient  = ADC_RawToReal(TwosCompToDec(Data->LED2_ALED2VAL, ADC_22bit));
    LEDs->LED1_On       = ADC_RawToReal(TwosCompToDec(Data->LED1VAL, ADC_22bit));
    LEDs->LED1_Ambient  = ADC_RawToReal(TwosCompToDec(Data->LED1_ALED1VAL, ADC_22bit));
}

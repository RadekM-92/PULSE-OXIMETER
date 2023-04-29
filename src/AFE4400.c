
#include "AFE4400.h"
#include "AFE4400_Types.h"

AFE4400_Data_t AFE4400_Data = {0};                /** AFE4400 All registers data */
AFE4400_Parameters_t AFE4400_Parameters = {0};    /** AFE4400 - Parameters */
AFE4400_LEDs_RealDataADC_t AFE4400_LEDs = {0};    /** AFE4400 - LEDs real ADC Data */

/** Software reset - resets all internal registers to the default values */
static void SoftwareReset(AFE4400_Data_t *Data);

/** Diagnostic enable - At the end of the sequence, all fault status are stored in the DIAG register */
static void DiagnosticEnable(AFE4400_Data_t *Data);

/** AFE4400 Power down */
static void AFE4400_PowerDown(AFE4400_Data_t *Data);

/** AFE4400 Power Up */
static void AFE4400_PowerUp(AFE4400_Data_t *Data);

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

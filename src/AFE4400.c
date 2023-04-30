
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

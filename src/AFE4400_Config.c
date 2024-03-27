

#include "AFE4400.h"
#include "AFE4400_Types.h"


/** Software reset - resets all internal registers to the default values */
static void SoftwareReset(AFE4400_Data_t *Data);

/** Diagnostic enable - At the end of the sequence, all fault status are stored in the DIAG register */
static void DiagnosticEnable(AFE4400_Data_t *Data);

/** AFE4400 Power down */
static void AFE4400_PowerDown(AFE4400_Data_t *Data);

/** AFE4400 Power Up */
static void AFE4400_PowerUp(AFE4400_Data_t *Data);






/** Configuration of parameters */
extern void ParametersInit(AFE4400_Parameters_t *Parameters)
{
    Parameters->PRF = 500;
    Parameters->DutyCycle = 25;

    Parameters->LED1_Current_mA = 10;
    Parameters->LED2_Current_mA = 10;

    Parameters->Cf = Cf_05pF;
    Parameters->Rf = Rf_500k;

    Parameters->CancellationCurrent = 0;
    Parameters->STG2_GAIN = 1;
}

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

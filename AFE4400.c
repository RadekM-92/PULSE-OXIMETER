

#include "AFE4400.h"



AFE4400_Data_t AFE4400_Data;    /** AFE4400 All registers data */


static void SoftwareReset(AFE4400_Data_t *Data)
{
    Data->CONTROL0 |= SW_RST;
}

static void DiagnosticEnable(AFE4400_Data_t *Data)
{
    Data->CONTROL0 |= DIAG_EN;
}


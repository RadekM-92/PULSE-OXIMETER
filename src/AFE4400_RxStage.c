

#include "AFE4400.h"
#include "AFE4400_Types.h"




/** Rx low pass filter init */
static void RxLowPassFilterInit(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data);

/** Rx Stage 2 init */
static void RxStage2Init(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data);



/** Rx Stage Init */
extern void RxInit(void)
{
    RxLowPassFilterInit(&AFE4400_Parameters, &AFE4400_Data);
    RxStage2Init(&AFE4400_Parameters, &AFE4400_Data);
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
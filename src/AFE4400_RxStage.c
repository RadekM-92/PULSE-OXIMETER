

#include "AFE4400.h"
#include "AFE4400_Types.h"




/** Rx low pass filter init */
static void RxLowPassFilterInit(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data);

/** Rx Stage 2 init */
static void RxStage2Init(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data);

/** ADC scaling 
 * -2097152 -> -1.2[V]
 * 2097151  ->  1.2[V]
*/
float ADC_RawToReal(int32_t ADC_RawVal);

/** Two's complement to decimal conversion */
int32_t TwosCompToDec(uint32_t TwosVal, uint32_t n_bits);



/** Rx Stage Init */
extern void RxInit(void)
{
    RxLowPassFilterInit(&AFE4400_Parameters, &AFE4400_Data);
    RxStage2Init(&AFE4400_Parameters, &AFE4400_Data);

    AFE4400_Write(TIA_AMB_GAIN, &AFE4400_Data.TIA_AMB_GAIN, 1U);
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
    else
    {
        Data->TIA_AMB_GAIN &= ~STAGE2EN;
    }
}

/** LEDs real ADC measurement data update */
extern void LEDs_RealDataADC_Update(const AFE4400_Data_t *Data, AFE4400_LEDs_RealDataADC_t *LEDs)
{
    const uint32_t RegisterMask = 0x3FFFFU;

    LEDs->LED2_On       = ADC_RawToReal(TwosCompToDec(Data->LED2VAL, 22U));
    LEDs->LED2_Ambient  = ADC_RawToReal(TwosCompToDec(Data->ALED2VAL, 22U));
    LEDs->LED2_Diff     = ADC_RawToReal(TwosCompToDec(Data->LED2_ALED2VAL, 22U));
    LEDs->LED1_On       = ADC_RawToReal(TwosCompToDec(Data->LED1VAL, 22U));
    LEDs->LED1_Ambient  = ADC_RawToReal(TwosCompToDec(Data->ALED1VAL, 22U));
    LEDs->LED1_Diff     = ADC_RawToReal(TwosCompToDec(Data->LED1_ALED1VAL, 22U));
}

/** ADC scaling
 * -2097152 -> -1.2[V]
 * 2097151  ->  1.2[V]
*/
float ADC_RawToReal(int32_t ADC_RawVal)
{
    const float Vref = 1.2f;
    const float Denominator = 2097152.0f;
    float ADC_Real_Volt;
    float Numerator = (float)ADC_RawVal * Vref;

    ADC_Real_Volt = Numerator / Denominator;

    return ADC_Real_Volt * 1000.0f;
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


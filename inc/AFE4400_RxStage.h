#ifndef AFE4400_RxStage_H
#define AFE4400_RxStage_H

#include "AFE4400.h"
#include "AFE4400_Types.h"

/** Rx Stage Init */
extern void RxInit(void);

/** Rx low pass filter init */
static void RxLowPassFilterInit(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data);

/** Rx Stage 2 init */
static void RxStage2Init(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data);

/** ADC scaling 
 * -2097152 -> -1.2[V]
 * 2097151  ->  1.2[V]
*/
static float ADC_RawToReal(int32_t ADC_RawVal);

/** Two's complement to decimal conversion */
static int32_t TwosCompToDec(uint32_t TwosVal, uint32_t n_bits);

#endif
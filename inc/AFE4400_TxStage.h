#ifndef AFE4400_TxStage_H
#define AFE4400_TxStage_H

#include "AFE4400.h"
#include "AFE4400_Types.h"

/** Tx Stage Init*/
extern void TxInit(void);

/** Tx LEDs current init
 * Parameters - configuration parameters
 * Data - AFE4400 registers
*/
static void TxLedsCurrentInit(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data);

/** LED current calculate
 * LedCurrent - 0..50 [mA]
 * Return value from 0 to 256
*/
static uint8_t LED_Current_mA_to_Raw(uint8_t LedCurrent);

#endif
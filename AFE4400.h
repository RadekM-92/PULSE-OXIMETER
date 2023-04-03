#ifndef AFE4400_H
#define AFE4400_H

#include <stdint.h>

#define AFE4400_CLOCK_FRQ       4000000U
#define ADC_Reset_ClockCycle    3U
#define LED_CurrentMax_mA       50U

/** AFE4400 Power down */
void AFE4400_PowerDown(AFE4400_Data_t *Data);

/** AFE4400 Power Up */
void AFE4400_PowerUp(AFE4400_Data_t *Data);


#endif



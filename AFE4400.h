#ifndef AFE4400_H
#define AFE4400_H

#include <stdint.h>

#define AFE4400_CLOCK_FRQ       4000000U
#define ADC_Reset_ClockCycle    3U

/** AFE4400 Power down */
void AFE4400_PowerDown(void);

/** AFE4400 Power Up */
void AFE4400_PowerUp(void);


#endif



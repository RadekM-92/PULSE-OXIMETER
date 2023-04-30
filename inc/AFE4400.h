#ifndef AFE4400_H
#define AFE4400_H



#define AFE4400_CLOCK_FRQ       4000000U
#define ADC_Reset_ClockCycle    3U
#define LED_CurrentMax_mA       50U
#define LED_CurrentMax_Raw      256
#define RxStage2_MaxGain        4U
#define RxStage2_CurrentMax     10U

/** AFE4400 Init */
void AFE4400_Init(void);

#endif



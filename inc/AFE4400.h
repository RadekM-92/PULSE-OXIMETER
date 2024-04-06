#ifndef AFE4400_H
#define AFE4400_H

#include "AFE4400_Types.h"

#define AFE4400_CLOCK_FRQ       4000000U
#define ADC_Reset_ClockCycle    3U
#define LED_CurrentMax_mA       50U
#define LED_CurrentMax_Raw      256
#define RxStage2_MaxGain        4U
#define RxStage2_CurrentMax     10U

/** AFE4400 Init */
extern void AFE4400_Init(void);
extern uint8_t AFE4400_ConvertMeasurement(void);

/** Configuration of parameters */
extern void ParametersInit(AFE4400_Parameters_t *Parameters);



extern uint8_t AFE4400_Write(AFE4400_REGS_ADDRESS_t Address, const uint32_t *Data, uint8_t Size);
extern uint8_t AFE4400_Read(AFE4400_REGS_ADDRESS_t Address, uint32_t *Data, uint8_t Size);

#endif



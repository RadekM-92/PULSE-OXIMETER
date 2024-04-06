#ifndef AFE4400_Timers_H
#define AFE4400_Timers_H

#include "AFE4400.h"
#include "AFE4400_Types.h"

/** Timer module initialization function */
extern void TimerModuleInit(void);

/** TimerModuleInit - check parameters */
static uint8_t IsTimerModuleParamOk(const AFE4400_Parameters_t *Parameters);

/** Pulse Repetition Frequency Timer Init */
static void PRP_TimerInit(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data);

/** ADC TimersInit */
static void ADC_TimersInit(const AFE4400_Parameters_t *Parameters, AFE4400_Data_t *Data);

/** Convert TimersInit */
static void Convert_TimersInit(AFE4400_Data_t *Data);

/** Sample TimersInit
 *  ADC_TimersInit() shall be call before call Sample_TimersInit()
*/
static void Sample_TimersInit(AFE4400_Data_t *Data);

/** LED pulse Timers Init 
 *  ADC_TimersInit() shall be call before call Sample_TimersInit()
*/
static void LedPulse_TimersInit(AFE4400_Data_t *Data);

/** Timer Enable */
static void TimerEnable(AFE4400_Data_t *Data);

/** Timer Disable */
static void TimerDisable(AFE4400_Data_t *Data);

#endif
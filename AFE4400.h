#ifndef AFE4400_H
#define AFE4400_H


/**
 * AFE4400 register map
*/
typedef enum
{
    CONTROL0		 =	0x00,   /** Control Register 0 */

    LED2STC			 =	0x01,   /** Sample LED2 Start Count Register */
    LED2ENDC		 =	0x02,   /** Sample LED2 End Count Register */
    LED2LEDSTC		 =	0x03,   /** LED2 start count */
    LED2LEDENDC		 =	0x04,   /** LED2 end count */
    ALED2STC		 =	0x05,   /** Sample ambient LED2 start count */
    ALED2ENDC		 =	0x06,   /** Sample ambient LED2 end count */

    LED1STC			 =	0x07,   /** Sample LED1 Start Count Register */
    LED1ENDC		 =	0x08,   /** Sample LED1 End Count Register */
    LED1LEDSTC		 =	0x09,   /** LED1 start count */
    LED1LEDENDC		 =	0x0A,   /** LED1 end count */
    ALED1STC		 =	0x0B,   /** Sample ambient LED1 start count */
    ALED1ENDC		 =	0x0C,   /** Sample ambient LED1 end count */
    
    LED2CONVST		 =	0x0D,   /** LED2 convert start count */
    LED2CONVEND		 =	0x0E,   /** LED2 convert end count */
    ALED2CONVST		 =	0x0F,   /** LED2 ambient convert start count */
    ALED2CONVEND	 =	0x10,   /** LED2 ambient convert end count */

    LED1CONVST		 =	0x11,   /** LED1 convert start count */
    LED1CONVEND		 =	0x12,   /** LED1 convert end count */
    ALED1CONVST		 =	0x13,   /** LED1 ambient convert start count */
    ALED1CONVEND	 =	0x14,   /** LED1 ambient convert end count */

    ADCRSTSTCT0		 =	0x15,   /** ADC Reset 0 Start Count Register */
    ADCRSTENDCT0	 =	0x16,   /** ADC RESET 0 start count */
    ADCRSTSTCT1		 =	0x17,   /** ADC Reset 1 Start Count Register */
    ADCRSTENDCT1	 =	0x18,   /** ADC RESET 1 start count */
    ADCRSTSTCT2		 =	0x19,   /** ADC Reset 2 Start Count Register */
    ADCRSTENDCT2	 =	0x1A,   /** ADC RESET 2 start count */
    ADCRSTSTCT3		 =	0x1B,   /** ADC Reset 3 Start Count Register */
    ADCRSTENDCT3	 =	0x1C,   /** ADC RESET 3 start count */

    PRPCOUNT		 =	0x1D,   /** Pulse Repetition Period Count Register */
    CONTROL1		 =	0x1E,   /** Control Register 1 */
    SPARE1			 =	0x1F,   /** SPARE1 Register For Future Use */
    TIAGAIN			 =	0x20,   /** Transimpedance Amplifier Gain Setting Register */
    TIA_AMB_GAIN	 =	0x21,   /** Transimpedance Amplifier and Ambient Cancellation Stage Gain Register */
    LEDCNTRL		 =	0x22,   /** LED Control Register */
    CONTROL2		 =	0x23,   /** Control Register 2 */
    SPARE2		 	 =	0x24,   /** SPARE2 Register For Future Use */
    SPARE3		 	 =	0x25,   /** SPARE3 Register For Future Use */
    SPARE4		 	 =	0x26,   /** SPARE4 Register For Future Use */
    RESERVED1		 =	0x27,   /** RESERVED1 Register For Factory Use Only */
    RESERVED2		 =	0x28,   /** RESERVED2 Register For Factory Use Only */
    ALARM		 	 =	0x29,   /** Alarm Register */
    LED2VAL		 	 =	0x2A,   /** LED2 digital value */
    ALED2VAL		 =	0x2B,   /** LED2 ambient digital value */
    LED1VAL		 	 =	0x2C,   /** LED1 digital value */
    ALED1VAL		 =	0x2D,   /** LED1 ambient digital value */
    LED2_ALED2VAL	 =	0x2E,   /** (LED2 – LED2 ambient) digital value */
    LED1_ALED1VAL	 =	0x2F 	/** (LED1 – LED1 ambient) digital value */
} AFE4400_REGS_ADDRESS_t;

/**
 * CONTROL0 Bits
*/
typedef enum
{
    SPI_READ        =   (1<<0), /** SPI read */
    TIM_COUNT_RST   =   (1<<1), /** Timer counter reset */
    DIAG_EN         =   (1<<2), /** Diagnostic enable */
    SW_RST          =   (1<<3)  /** Software reset */
} CONTROL0_BITS_t;

/**
 * CONTROL1 Bits
*/
typedef enum
{
    TIMEREN         =   (1<<8), /** Timer enable */
    CLKALMPIN_0     =   (1<<9), /** Clocks on ALM pin 0 */
    CLKALMPIN_1     =   (1<<10), /** Clocks on ALM pin 0 */
    CLKALMPIN_2     =   (1<<11) /** Clocks on ALM pin 0 */
} CONTROL1_Bits_t;

/**
 * TIA_AMP_GAIN Bits
*/
typedef enum
{
    RF_LED_0    =   (1<<0), /** Program RF for LEDs - bit 0 */
    RF_LED_1    =   (1<<1), /** Program RF for LEDs - bit 1 */
    RF_LED_2    =   (1<<2), /** Program RF for LEDs - bit 2 */

    CF_LED_0    =   (1<<3), /** Program CF for LEDs - bit 0 */
    CF_LED_1    =   (1<<4), /** Program CF for LEDs - bit 1 */
    CF_LED_2    =   (1<<5), /** Program CF for LEDs - bit 2 */
    CF_LED_3    =   (1<<6), /** Program CF for LEDs - bit 3 */
    CF_LED_4    =   (1<<7), /** Program CF for LEDs - bit 4 */

    ST2GAIN_0   =   (1<<8), /** Stage 2 gain setting - bit 0 */
    ST2GAIN_1   =   (1<<9), /** Stage 2 gain setting - bit 1 */
    ST2GAIN_2   =   (1<<10),    /** Stage 2 gain setting - bit 2 */

    STAGE2EN    =   (1<<14),    /** Stage 2 enable for LED 2 */

    AMBDAC_0    =   (1<<16),    /** Ambient DAC value - bit 0 */
    AMBDAC_1    =   (1<<17),    /** Ambient DAC value - bit 1 */
    AMBDAC_2    =   (1<<18),    /** Ambient DAC value - bit 2 */
    AMBDAC_3    =   (1<<19),    /** Ambient DAC value - bit 3 */

} TIA_AMP_GAIN_t;



#endif



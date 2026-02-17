////////////////////////////////////////////////////////////////////////////////
// © 2019 Microchip Technology Inc. and its subsidiaries.
//
// MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: Subject to your compliance with 
// these terms, you may use Microchip software and any derivatives exclusively 
// with Microchip products. It is your responsibility to comply with third party
// license terms applicable to your use of third party software (including open 
// source software) that may accompany Microchip software.
//
// THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
// EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
// IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
// FOR A PARTICULAR PURPOSE.
//
// IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
// INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
// WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
// HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
// THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
// CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
// OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////


#include <stdint.h>
#include "p33Exxxx.h" 
#include "define.h"
#include "init.h"
#include "VMC_coeffs.h"
#include "delay.h"

int16_t  abCoefficients2P2Z[5] __attribute__ ((space(xmemory)));
int16_t  errorcontrolHistory2P2Z[4] __attribute__ ((space(ymemory), far));  

int16_t  compOutput;
int16_t  LLCOptions;
int16_t  ACCAHstore;
int16_t  ACCAUstore;
uint8_t SFRupdateFlag=0;
uint16_t newPTPER;  

void initClock(void)
{
   /* Configure Oscillator to operate the device at 68 MHz
    * Fosc = Fin*M/(N1*N2), Fcy = Fosc/2
    * Fosc = 7.37*(76)/(2*2)= ~140Mhz for Fosc, Fcy = 70 MHz */

   /* Configure PLL prescaler, PLL postscaler, PLL divisor */
   PLLFBD             = PLL_DIV;                /* M = PLLFBD + 2 */
   CLKDIVbits.PLLPOST =  PLLPOSTX;              /* N1 = 2 */
   CLKDIVbits.PLLPRE  =  PLLPRIX;               /* N2 = 2 */

   __builtin_write_OSCCONH(NEWOSC_PRIMARY_WITHPLL); /* New Oscillator External 
                                                     * Crystal w/ PLL */      
   __builtin_write_OSCCONL(NEWOSC_SWITCH_ENABLE);   /* New Oscillator switch 
                                                     * enable */

   while(OSCCONbits.COSC != CURRENTOSC_PRIMARY_WITHPLL);/* Wait for Osc. to switch 
                                                         * to External Osc w/ PLL */
   while(OSCCONbits.LOCK != 1);                         /* Wait for PLL to Lock */

   /* Now setup the ADC and PWM clock for 120MHz
	* ((EXT OSC * 16) / APSTSCLR ) = (7.37 * 16) / 1 = ~ 120MHz */

	ACLKCONbits.ASRCSEL = ACLOCK_ASRCSEL_PRIMARY;   /* External Oscillator provides 
                                                     * the clock for APLL */
	ACLKCONbits.FRCSEL = ACLOCK_FRCSEL_PRIMARY;		/* ASRCSEL provides input for 
                                                     * Auxiliary PLL (x16) */ 
	ACLKCONbits.SELACLK = ACLOCK_DIVSOURCE_APLL;	/* Auxiliary Oscillator provides 
                                                     * clock source for PWM & ADC */ 
	ACLKCONbits.APSTSCLR = ACLOCK_DIVIDER_BY1;		/* Divide Auxiliary clock by 1 */ 
	ACLKCONbits.ENAPLL = APLL_ENABLE;               /* Enable Auxiliary PLL */ 
	
	while(ACLKCONbits.APLLCK != 1);                 /* Wait for Auxiliary PLL to 
                                                     * Lock */
    Delay_Us(Delay50us);                            /* Suggested in Errata */    
}

void initIOPorts(void)
{
	TRISBbits.TRISB5 = IO_PIN_OUTPUT;   /* LED1-For normal running condition */ 
	TRISBbits.TRISB11 = IO_PIN_OUTPUT;  /* LED2-For Debug/testing */
	TRISBbits.TRISB12 = IO_PIN_OUTPUT;  /* FAULT-For Fault Indication */
	TRISBbits.TRISB15 = IO_PIN_OUTPUT;  /* AUX-For auxiliary restart as output */ 

	LED_FAULT = OFF;					/* Fault LED disabled at start-up */ 
	LED_1 = ON;                         /* Turn on LED1 to indicate device 
                                         * is running */ 
	LED_2 = OFF;						/* This LED can be used for debugging */ 

	AUXILIARY_START = ON;				/* Keep Aux on until 12V starts up */ 	
}

void initPWM(void)
{
	/* PWM setup for half-bridge */
 	PTPER = SOFTSTARTPERIOD;                    /* PTPER is at lowest value at 
                                                 * starting; it will start LLC 
                                                 * converter at highest switching 
                                                 * frequency */
    newPTPER = PTPER;                           /* PTPER value is copied in dummy 
                                                 * variable of PTPER */  

    IOCON1bits.PENH = PWM_PIN_IO;               /* PWM1H is controlled by I/O 
                                                 * module */ 
    IOCON1bits.PENL = PWM_PIN_IO;               /* PWM1L is controlled by I/O 
                                                 * module */ 
    IOCON1bits.PMOD = MODE_PUSHPULL;            /* PWM1 is in Push-Pull Mode */ 
	IOCON1bits.OVRDAT = PWM_PIN_OVERDAT_LOW;    /*PWM assign low is OVERDAT 
                                                 * enable */
    PWMCON1bits.DTC = DEADTIME_DISABLE;         /*PWM1 dead-time is disabled; it 
                                                 * is added into duty cycle*/ 
   	PWMCON1bits.IUE = IUE_ENABLE;               /*PWM1 Immediate duty cycle 
                                                 * update enable; it is used in 
                                                 * SEVTCMP interrupt*/  
    PWMCON1bits.ITB = PRIMARY_TIMEBASE;         /*PWM1 is in Primary Time base 
                                                 * mode*/              
	FCLCON1bits.FLTMOD = PWM_FAULTMODE_LATCH;   /* Latched Fault Mode */ 
	FCLCON1bits.FLTSRC = PWM_FAULTSOURCE_ACMP1; /* Fault Source ACMP1 - Current */ 
	FCLCON1bits.FLTPOL = PWM_FAULTSOURCE_HIGH;  /* Fault Source Active High */ 

	PHASE1 = INITIAL_PHASE1;                    /* PHASE1 is set to 16 counts */
	SPHASE1 = INITIAL_SPHASE1 ;                 /* SPHASE1 is set to 16 counts */

	#ifdef OPEN_LOOP
	PDC1 = PTPER  - DEADTIME;
	#else           
	PDC1 = INITIAL_PDC1;                        /* Initial PDC1 is set to 9 
                                                 * counts */ 
	#endif

	TRIG1 = TRIG1_COUNT;                        /* Hard coded Trigger instant needs 
                                                 * to work for entire operating 
                                                 * range; set at 1740 */
	TRGCON1bits.TRGSTRT = TRIGSTART_DELAY_1PWM;	/* Wait for 1 PWM cycle for Trig 
                                                 * start*/  
	TRGCON1bits.TRGDIV = TRIGOUTPUT_EVENT_2;    /* Trig=2 generated at alternate 
                                                 * cycle; Push-Pull mode trigger 
                                                 * occurs on every cycle */ 

	/* PWM 2 Configuration */
	IOCON2bits.PENH = PWM_PIN_IO;               /* PWM2H is controlled by I/O 
                                                 * module */ 
	IOCON2bits.PENL = PWM_PIN_IO;               /* PWM2L is controlled by I/O 
                                                 * module */ 
   	IOCON2bits.PMOD = MODE_PUSHPULL;            /* PWM2 is in Push-Pull Mode */ 
	IOCON2bits.SWAP = SWAP_ENABLE;              /* PWM2 swap high and low pins 
                                                 * for transformer polarity */
	IOCON2bits.OVRDAT = PWM_PIN_OVERDAT_LOW;    /*PWM assign low is OVERDAT 
                                                 * enable */
        
    PWMCON2bits.DTC = DEADTIME_DISABLE;         /* Dead-time disabled; added into 
                                                 * duty cycle */ 
   	PWMCON2bits.IUE = IUE_ENABLE;               /* Enable Immediate duty cycle 
                                                 * update as SEVTCMP is used */
    PWMCON2bits.ITB = PRIMARY_TIMEBASE;         /* Primary Time-base Mode is 
                                                 * set */
   		                                         
	FCLCON2bits.FLTMOD = PWM_FAULTMODE_LATCH;   /* Latched Fault Mode */ 
	FCLCON2bits.FLTSRC = PWM_FAULTSOURCE_ACMP1; /* Fault Source ACMP1 - Current */
	FCLCON2bits.FLTPOL = PWM_FAULTSOURCE_HIGH;  /* Fault Source Active High */ 

	#ifdef OPEN_LOOP
	if(PTPER == 2940)
		PDC2 = (1271<<1)- SRDEADTIME;
	else if (PTPER == 2850)
		PDC2 = (1261<<1)- SRDEADTIME;
	else if (PTPER == 2760)
		PDC2 = (1248<<1)- SRDEADTIME;
	else if (PTPER == 2690)
		PDC2 = (1237<<1)- SRDEADTIME;
	else if (PTPER == 2620)
		PDC2 = (1227<<1)- SRDEADTIME;
	else if (PTPER == 2550)
		PDC2 = (1217<<1)- SRDEADTIME;
	else if (PTPER == 2480)
		PDC2 = (1207<<1)- SRDEADTIME;
	else if (PTPER == 2400)
		PDC2 = (1195<<1)- SRDEADTIME;		 
	else
	PDC2 = PTPER  - SRDEADTIME;
	#else                         
	PDC2 = INITIAL_PDC2;
	#endif
	
	TRGCON2bits.TRGSTRT = TRIGSTART_DELAY_1PWM; /* Wait for 1 PWM cycle for Trig 
                                                 * start*/
	TRGCON2bits.TRGDIV = TRIGOUTPUT_EVENT_2;    /* Trig=2 generated at alternate 
                                                 * cycle; Push-Pull mode trigger 
                                                 * occurs on every cycle */    
	TRIG2 = PTPER>>2;                           /* Trig2 at PTPER/2 */ 
    
    /* PWM3 Configuration */
    IOCON3bits.PENH = PWM_PIN_IO;               /* PWM3H is controlled by I/O 
                                                 * module */ 
    IOCON3bits.PENL = PWM_PIN_IO;               /* PWM3L is controlled by I/O 
                                                 * module */
    
    /* Special Event Trigger Interrupt Configuration */
    SEVTCMP = PTPER - 20;                       /* Special Event Trigger set 20 
                                                 * count before PTPER roll-over 
                                                 * to ensure SFRs update not at 
                                                 * PTPER roll-over */
    PTCONbits.SEVTPS = SEVT_EVERY_COMPARE_MATCH;    /* Special Event Trigger output 
                                                     * postscaler set to 1:1; 
                                                     * selection (trigger generated 
                                                     * Every PWM cycle) */
    PTCONbits.EIPU = EIPU_ENABLE;               /* Immediate Period update 
                                                 * enable */    
    IFS3bits.PSEMIF = INTERRUPT_FLAG_CLEAR;     /* SEVTCMP interrupt flag 
                                                 * disable */
    IPC14bits.PSEMIP = INTERRUPT_PRIORITY_6;    /* SEVTCMP interrupt priority 
                                                 * highest */
    PTCONbits.SEIEN = INTERRUPT_DISABLE;        /* Special Event Trigger interrupt 
                                                 * is disable */
}

void initADC(void)
{
    /* ADC Initialization */
    
 	/* Setup ADC Clock Input Max speed of 70 MHz --> Fosc = 140 MHz */
    ADCON3Hbits.CLKSEL  = ADC_CLOCKSOURCE_OSCILLATOR;   /* 0-Fsys, 1-Fosc, 
                                                         * 2-FRC, 3-APLL */
    ADCON3Hbits.CLKDIV  = ADC_CLOCKDIV_1BY1;    /* Global Clock divider (1:1) */
    ADCORE0Hbits.ADCS   = ADC_CORECLOCK_1BY2;   /* Core 0 clock divider (1:2) */
    ADCORE1Hbits.ADCS   = ADC_CORECLOCK_1BY2;   /* Core 1 clock divider (1:2) */
    ADCORE2Hbits.ADCS   = ADC_CORECLOCK_1BY2;   /* Core 2 clock divider (1:2) */
    ADCORE3Hbits.ADCS   = ADC_CORECLOCK_1BY2;   /* Core 3 clock divider (1:2) */
    
    ADCON1Hbits.FORM = ADC_INTGER_FORM;         /* Integer format */
    ADCON2Lbits.EIEN = ADC_EARLYINT_DISABLE;    /* Early Interrupt disabled */
    ADCON3Lbits.REFSEL = ADC_REFVOLT_AVDD;      /* AVdd as voltage reference */

    /* ADC Cores in 12-bit resolution mode */
    ADCORE0Hbits.RES = ADC_12BIT_RESOLUTION;    /* ADC Core 0 in 12-bit 
                                                 * resolution mode */
    ADCORE1Hbits.RES = ADC_12BIT_RESOLUTION;    /* ADC Core 1 in 12-bit 
                                                 * resolution mode */
    ADCORE2Hbits.RES = ADC_12BIT_RESOLUTION;    /* ADC Core 2 in 12-bit 
                                                 * resolution mode */
    ADCORE3Hbits.RES = ADC_12BIT_RESOLUTION;    /* ADC Core 3 in 12-bit 
                                                 * resolution mode */
    
    /* Configure ANx for unsigned format and single ended (0,0) */
    ADMOD0L = ADC_ALLCHANNEL_SINGLE_UNSIGNED;

    InitCalibrateADC();

    /* ADC Channel Trigger Source: For AN0 and AN1 Trigger Source is PWM2, 
     * For AN2 and AN3 Trigger Source is PWM1 */
    ADTRIG0Lbits.TRGSRC0 = ADC_TRIGGERSOURCE_PWM2;  /* ADC AN0 triggered by 
                                                     * PWM2-Trig2 */
    ADTRIG0Lbits.TRGSRC1 = ADC_TRIGGERSOURCE_PWM2;  /* ADC AN1 triggered by 
                                                     * PWM2-Trig2 */
    ADTRIG0Hbits.TRGSRC2 = ADC_TRIGGERSOURCE_PWM1;  /* ADC AN2 triggered by 
                                                     * PWM1-Trig1 */
    ADTRIG0Hbits.TRGSRC3 = ADC_TRIGGERSOURCE_PWM1;  /* ADC AN3 triggered by 
                                                     * PWM1-Trig1 */

    IFS6bits.ADCAN0IF  = INTERRUPT_FLAG_CLEAR;  /* Clear ADC AN0 interrupt flag */
    IPC27bits.ADCAN0IP = INTERRUPT_PRIORITY_4;  /* Set ADC AN0 interrupt priority 
                                                 * to 4 */
    
    IFS7bits.ADCAN3IF  = INTERRUPT_FLAG_CLEAR;  /* Clear ADC AN3 interrupt flag */
    IPC28bits.ADCAN3IP = INTERRUPT_PRIORITY_3;  /* Set ADC AN3 interrupt priority 
                                                 * to 3 */
    
    IEC6bits.ADCAN0IE  = INTERRUPT_DISABLE;     /* Disable ADC AN0 interrupt till 
                                                 * soft-start */
    ADIELbits.IE0 = INTERRUPT_DISABLE;          /* Enable ADC common and individual 
                                                 * interrupt */
    
    IEC7bits.ADCAN3IE  = INTERRUPT_DISABLE;     /* Disable ADC AN3 interrupt till 
                                                 * soft-start */
    ADIELbits.IE3 = INTERRUPT_DISABLE;          /* Enable ADC common and individual 
                                                 * interrupt */
}

void InitCalibrateADC(void)
{
    /* Power Up delay: 2048 Core Clock Source Periods (TCORESRC) for all ADC 
     * cores (~14.6 us) */
    _WARMTIME = ADC_CALI_WARMUP;

    /* Turn on ADC module */
    ADCON1Lbits.ADON  = ADC_MODULE_ENABLE;

    /* Turn on analog power for dedicated core 0 */
    ADCON5Lbits.C0PWR = ADC_CORE_POWER_ENABLE;
    while(ADCON5Lbits.C0RDY == 0);
    ADCON3Hbits.C0EN  = ADC_CORE_ENABLE;        /* Enable ADC core 0 */

    /* Turn on analog power for dedicated core 1 */
    ADCON5Lbits.C1PWR = ADC_CORE_POWER_ENABLE;
    while(ADCON5Lbits.C1RDY == 0);
    ADCON3Hbits.C1EN  = ADC_CORE_ENABLE;        /* Enable ADC core 1 */

    /* Turn on analog power for dedicated core 2 */
    ADCON5Lbits.C2PWR = ADC_CORE_POWER_ENABLE;
    while(ADCON5Lbits.C2RDY == 0);
    ADCON3Hbits.C2EN  = ADC_CORE_ENABLE;        /* Enable ADC core 2 */

    /* Turn on analog power for dedicated core 3 */
    ADCON5Lbits.C3PWR = ADC_CORE_POWER_ENABLE;
    while(ADCON5Lbits.C3RDY == 0);
    ADCON3Hbits.C3EN  = ADC_CORE_ENABLE;        /* Enable ADC core 3 */

    /* Calibration for the dedicated core 0 */
    ADCAL0Lbits.CAL0EN   = ADC_CORE_CALI_ENABLE;    /*Core 0 calibration enable */
    ADCAL0Lbits.CAL0DIFF = ADC_CORE_CALI_SINGLEMODE;/* Single-ended calibration */
    ADCAL0Lbits.CAL0RUN  = ADC_CORE_CALI_START;     /* Start core 0 calibration */
    while(ADCAL0Lbits.CAL0RDY == 0);
    ADCAL0Lbits.CAL0EN   = ADC_CORE_CALI_DISABLE;   /*Core 0 calibration 
                                                     * complete */

    /* Calibration for the dedicated core 1 */
    ADCAL0Lbits.CAL1EN   = ADC_CORE_CALI_ENABLE;    /*Core 1 calibration enable */
    ADCAL0Lbits.CAL1DIFF = ADC_CORE_CALI_SINGLEMODE;/* Single-ended calibration */                                                     
    ADCAL0Lbits.CAL1RUN  = ADC_CORE_CALI_START;     /* Start core 1 calibration */
    while(ADCAL0Lbits.CAL1RDY == 0);
    ADCAL0Lbits.CAL1EN   = ADC_CORE_CALI_DISABLE;   /*Core 1 calibration 
                                                     * complete */

    /* Calibration for the dedicated core 2 */
    ADCAL0Hbits.CAL2EN   = ADC_CORE_CALI_ENABLE;    /*Core 2 calibration enable */
    ADCAL0Hbits.CAL2DIFF = ADC_CORE_CALI_SINGLEMODE;/* Single-ended calibration */                                                     
    ADCAL0Hbits.CAL2RUN  = ADC_CORE_CALI_START;     /* Start core 2 calibration */
    while(ADCAL0Hbits.CAL2RDY == 0);
    ADCAL0Hbits.CAL2EN   = ADC_CORE_CALI_DISABLE;   /*Core 2 calibration 
                                                     * complete */

    /* Calibration for the dedicated core 3 */
    ADCAL0Hbits.CAL3EN   = ADC_CORE_CALI_ENABLE;    /*Core 3 calibration enable */
    ADCAL0Hbits.CAL3DIFF = ADC_CORE_CALI_SINGLEMODE;/* Single-ended calibration */
    ADCAL0Hbits.CAL3RUN  = ADC_CORE_CALI_START;     /* Start core 3 calibration */
    while(ADCAL0Hbits.CAL3RDY == 0);
    ADCAL0Hbits.CAL3EN   = ADC_CORE_CALI_DISABLE;   /*Core 3 calibration 
                                                     * complete */
}

void intControlConst(void)
{
    /* Control coefficient assignment */
    abCoefficients2P2Z[0] = CON_2P2Z_COEFF_B0; 
    abCoefficients2P2Z[1] = CON_2P2Z_COEFF_B1; 
    abCoefficients2P2Z[2] = CON_2P2Z_COEFF_B2; 
    abCoefficients2P2Z[3] = CON_2P2Z_COEFF_A1; 
    abCoefficients2P2Z[4] = CON_2P2Z_COEFF_A2; 
    
    LLCOptions = 0;               /* Trigger options of 2P2Z library disabled */
    
    InitAltRegContext1Setup();    /* SMPS_library context set-up */     
}

void init_TIMER2(void)
{
    /*Initialize TIMER2*/
    PR2 = T2PERIOD;                         /* Timer period:(1/200kHz/14.2857ns) 
                                             * = 350 = 0x015E */
    T2CONbits.TCKPS = TIMER_PRESCALER_1BY1; /* 1:1 pre-scaler;Ftimer = Fcy */ 
    T2CONbits.TCS = TIMER_INTERNAL_CLOCK;   /* Internal clock */
    T2CONbits.TGATE = TIMER_GATEDTIME_DISABLE; /* Gated time accumulation 
                                                * disable */
    T2CONbits.TSIDL = TIMER_IDLEMODE_DISABLE;  /* Continuous time operation in 
                                                * idle mode */
    TMR2 = TIMER_RESET;                     /* Reset Timer-2 */
	IFS0bits.T2IF = INTERRUPT_FLAG_CLEAR;   /* Clear Timer-2 interrupt flag */ 
    IEC0bits.T2IE = INTERRUPT_DISABLE;      /* Disable Timer-2 interrupt */ 
    IPC1bits.T2IP = INTERRUPT_PRIORITY_5;   /* Higher priority than ADC 
                                             * interrupts */
    T2CONbits.TON = TIMER_MODULE_ENABLE;    /* Turn-on Timer2 */       
}

void init_PWMstart(void)
{
    PTCONbits.PTEN = PWM_MODULE_ENABLE;     /* Enable PWM outputs */

	/* To eliminate PWM glitch at start-up, enable PWM module and then give PWM 
     * ownership of the pin */

	IOCON1bits.PENH = PWM_PIN_PWM;          /* PWM1H is controlled by PWM module */
	IOCON1bits.PENL = PWM_PIN_PWM;          /* PWM1L is controlled by PWM module */

	IOCON2bits.PENH = PWM_PIN_PWM;          /* PWM2H is controlled by PWM module */
	IOCON2bits.PENL = PWM_PIN_PWM;          /* PWM2L is controlled by PWM module */

    /* Special Event Interrupt Setting */
    PTCONbits.SEVTPS = SEVT_EVERY_COMPARE_MATCH; /* Special event trigger at every 
                                                  * compare match */
    PTCONbits.SEIEN = INTERRUPT_ENABLE;     /* Enable special event interrupt */
    
    IEC3bits.PSEMIE = INTERRUPT_ENABLE;     /* Enable special event interrupt */
    IFS3bits.PSEMIF = INTERRUPT_FLAG_CLEAR; /* Disable special event interrupt 
                                             * flag */
            
}
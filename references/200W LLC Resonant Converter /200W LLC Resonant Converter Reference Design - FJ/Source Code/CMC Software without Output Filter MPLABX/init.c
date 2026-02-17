/******************************************************************************* ******************************************************************************
MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any derivatives created by 
any person or entity by or on your behalf, exclusively with Microchip’s products.  Microchip and its licensors
 retain all ownership and intellectual property rights in the accompanying software and in all derivatives hereto.  
This software and any accompanying information is for suggestion only.  It does not modify Microchip’s standard warranty for its products.  
You agree that you are solely responsible for testing the software and determining its suitability.  Microchip has no obligation to modify, test, certify, 
or support the software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, 
BUT NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE 
APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP’S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN 
ANY APPLICATION. 

IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY),
STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS,
DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED 
OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON 
ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP 
FOR THIS SOFTWARE.
MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS.

*************************************************************************************************************************************************************/

#include "p33Fxxxx.h" 
#include "define.h"

extern void delay_100us(unsigned int delay);
extern void faultLoop();
extern unsigned int inputVoltage, outputVoltage,tankCurrent;
extern unsigned char index_SR, faultState;
extern unsigned int synchRectDutyCycle[126][2];
unsigned char softStartFlag = 0;

void initClock(void)
{
	/* Because LLC Half-Bridge converter is frequency controlled an external oscillator 
	is used to provide the optimum tolerance over the specified operating temperature range */	

	/* Configure Oscillator to operate the device near 40Mhz
	   Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
 	   Fosc= 7.37*(43)/(2*2) = ~80Mhz for Fosc, Fcy = ~40Mhz */

	/* Configure PLL prescaler, PLL postscaler, PLL divisor */
	PLLFBD = 41; 			  /* M = PLLFBD + 2 */
	CLKDIVbits.PLLPOST = 0;   /* N1 = 2 */
	CLKDIVbits.PLLPRE = 0;    /* N2 = 2 */

   	 __builtin_write_OSCCONH(0x03);			/* New Oscillator External Crystal w/ PLL */
   	 __builtin_write_OSCCONL(0x01);  		/* Enable Switch */
    
	while(OSCCONbits.COSC != 0b011);		/* Wait for new Oscillator to become External Crystal w/ PLL */  
    	while(OSCCONbits.LOCK != 1);			/* Wait for Pll to Lock */

	/* Now setup the ADC and PWM clock for 120MHz
	   ((EXT OSC * 16) / APSTSCLR ) = (7.37 * 16) / 1 = ~ 120MHz*/

	ACLKCONbits.ASRCSEL = 1;				/* External Oscillator provides the clock for APLL */
	ACLKCONbits.FRCSEL = 0;					/* ASRCSEL provides input for Auxiliary PLL (x16) */
	ACLKCONbits.SELACLK = 1;				/* Auxiliary Oscillator provides clock source for PWM & ADC */
	ACLKCONbits.APSTSCLR = 7;				/* Divide Auxiliary clock by 1 */
	ACLKCONbits.ENAPLL = 1;					/* Enable Auxiliary PLL */
	
	while(ACLKCONbits.APLLCK != 1);                         /* Wait for Auxiliary PLL to Lock */
    
}

void initIOPorts(void)
{
	TRISBbits.TRISB5 = 0;                                           /* All LEDs set for output */
	TRISBbits.TRISB11 = 0;
	TRISBbits.TRISB12 = 0;
	TRISBbits.TRISB15 = 0;                                          /* Auxiliary restart as output */

	LED_FAULT = OFF;						/* Fault LED disabled at start-up */
	LED_1 = ON;						    	/* Turn on one LED to indicate device running */
	LED_2 = OFF;							/* This LED can be used for debugging purposes */

	AUXILIARY_START = ON;                                           /* Keep Aux on until 12V starts up */

	RPINR29bits.FLT1R = 0b100000;                                   /* PWM FLT1 Remapped to RP32 */
	RPOR16bits.RP32R = 0b100111;                                    /* ACMP1 (Current) Remapped to RP32 */
}

void initPWM(void)
{
	/* PWM setup for half-bridge */

 	PTPER = SOFTSTARTPERIOD;                        

    	IOCON1bits.PENH = 0;                             /* PWM1H is controlled by I/O module */
    	IOCON1bits.PENL = 0;                             /* PWM1L is controlled by I/O module */
    	IOCON1bits.PMOD = 2;                             /* Push-Pull Mode */

	IOCON1bits.OVRDAT = 0;

    	PWMCON1bits.DTC = 2;                            /* Deadtime is disabled added into duty cycle */
   	 PWMCON1bits.IUE = 0;                           /* Disable Immediate duty cycle updates */
    	PWMCON1bits.ITB = 0;                            /* Select Primary Timebase mode */

	FCLCON1bits.FLTMOD = 0;				/* Latched Fault Mode */
	FCLCON1bits.FLTSRC = 0;				/* Fault Source FLT1 - Current */
	FCLCON1bits.FLTPOL = 0;				/* Active High */

	PHASE1 = 16;
	SPHASE1 = 16;

	#ifdef OPEN_LOOP
	PDC1 = PTPER  - DEADTIME;
	#else                         
	PDC1 = 9;
	#endif

	TRIG1 = PTPER>>2;
	TRGCON1bits.TRGSTRT = 1;			  
	TRGCON1bits.TRGDIV = 0;				  /* Trigger generated every cycle of
                                                              Push-Pull mode so trigger occurs on every 5th PWMH on time */
	/* PWM 2 Configuration */

	IOCON2bits.PENH = 0;                             /* PWM2H is controlled by I/O module */
	IOCON2bits.PENL = 0;                             /* PWM2L is controlled by I/O module */
   	 IOCON2bits.PMOD = 2;                            /* Push-Pull Mode */
	IOCON2bits.SWAP = 1;                             /* SWAP high and low for transformer polarity */
	
	IOCON2bits.OVRDAT = 0;

    	PWMCON2bits.DTC = 2;                            /* Deadtime disabled added into duty cycle */
   	 PWMCON2bits.IUE = 0;                           /* Disable Immediate duty cycle updates */
    	PWMCON2bits.ITB = 0;                             /* Select Idependent Timebase mode */
   		                                         
	FCLCON2bits.FLTMOD = 0;				/* Latched Fault Mode */
	FCLCON2bits.FLTSRC = 0;				/* Fault Source FLT1 - Current */
	FCLCON2bits.FLTPOL = 0;				/* Active High */

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
	PDC2 = 9;
	#endif
	
	TRGCON2bits.TRGSTRT = 1;
	TRGCON2bits.TRGDIV = 0;
	TRIG2 = PTPER>>2;

}

void initADC(void)
{

 	ADCONbits.FORM = 0;                               /* Integer data format */
    	ADCONbits.EIE = 0;                                /* Early Interrupt disabled */
    	ADCONbits.ORDER = 1;                              /* Convert odd channel first */
    	ADCONbits.SEQSAMP = 0;                            /* Select simultaneous sampling */
	ADCONbits.SLOWCLK = 1;				  /* Slow clock must be set */
	ADCONbits.ASYNCSAMP = 0;			  /* Dedicated S&H starts sampling when trigger detected */
    	ADCONbits.ADCS = 5;                               /* ADC clock = FADC/6 = ~ 120MHz / 6 = 20MHz, 14*Tad = 1.4 MSPS, two SARs = 2.8 MSPS */


	ADCPC0bits.TRGSRC0 = 5;				  /* PWM2H triggers ADCP0  */
	ADCPC0bits.TRGSRC1 = 4;				  /* PWM1H triggers ADCP1 */

	IFS6bits.ADCP0IF = 0;				  /* Clear ADC Interrupt Flags */
	IFS6bits.ADCP1IF = 0;

	IEC6bits.ADCP0IE = 0;				  /* Disabled until Softstart */
	IEC6bits.ADCP1IE = 0;				  /* Disabled until Softstart */

	IPC27bits.ADCP0IP = 5;				  /* Critical ADC Pairs have higher priority */

	ADCONbits.ADON = 1;                                /* Enable ADC now to allow time to stabilize */

}

void softStartRoutine(void)
{
	/* Manually reduce the frequency until the output voltage is close to the Reference Voltage (12V) */

	softStartFlag = 1;
	 
	IFS6bits.ADCP0IF = 0;
	IFS6bits.ADCP1IF = 0;
	IEC6bits.ADCP0IE = 1;	/* Enable ADC ISRs */
	IEC6bits.ADCP1IE = 1;

	while(outputVoltage <= (OUTPUTVOLTAGEREFERENCE - 200))
	{
		PTPER = PTPER + 50;
	
		if(PTPER >= MAXPERIOD)
		{
			PTPER = MAXPERIOD;
			faultState = FAULT_SOFTSTART;
			faultLoop();
		}

		PDC1 = PTPER - DEADTIME;

		if(PTPER >= RESONANTPERIOD)
		{
			index_SR = (PTPER & 0xFFF8) - RESONANTPERIOD;
			if(index_SR <= 0)
			{
				index_SR = 0;
			}

			PDC2 = (synchRectDutyCycle[index_SR>>3][1])-SRDEADTIME;
		}

		else
		{
			PDC2 = PTPER - SRDEADTIME;
		}
		
		delay_100us(2);

	}
	
	softStartFlag = 0;		/* End of Soft-Start, PI compensator now controlling */
}


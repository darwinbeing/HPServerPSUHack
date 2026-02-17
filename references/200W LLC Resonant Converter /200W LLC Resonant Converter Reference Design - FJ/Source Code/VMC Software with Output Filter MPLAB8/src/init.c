/*******************************************************************************
* © 2009 Microchip Technology Inc.
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE,  
* WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF 
* STATUTORY DUTY), STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR 
* ANY INDIRECT, SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, 
* DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE, 
* HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE
* DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S 
* TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS CODE, SHALL NOT 
* EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO HAVE THIS 
* CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
*
*******************************************************************************/

#include "p33Fxxxx.h" 
#include "define.h"

extern void delay_100us(unsigned int delay);
extern void faultLoop();
extern unsigned int inputVoltage, outputVoltage;
extern long integralOutput;
extern int modifier;
extern unsigned char index_SR, faultState;
extern unsigned int synchRectDutyCycle[126][2];

unsigned char softStartFlag = 0;
signed long prevIntegralOutput = 0;


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
	
	while(ACLKCONbits.APLLCK != 1);			/* Wait for Auxiliary PLL to Lock */
    
}

void initIOPorts(void)
{
	TRISBbits.TRISB5 = 0;					/* All LEDs set for output */
	TRISBbits.TRISB11 = 0;
	TRISBbits.TRISB12 = 0;
	TRISBbits.TRISB15 = 0;					/* Auxiliary restart as output */

	LED_FAULT = OFF;						/* Fault LED disabled at start-up */
	LED_1 = ON;						    	/* Turn on one LED to indicate device running */
	LED_2 = OFF;							/* This LED can be used for debugging purposes */

	AUXILIARY_START = ON;					/* Keep Aux on until 12V starts up */

	RPINR29bits.FLT1R = 0b100000;			/* PWM FLT1 Remapped to RP32 */
	RPOR16bits.RP32R = 0b100111;			/* ACMP1 (Current) Remapped to RP32 */
}

void initPWM(void)
{
	/* PWM setup for half-bridge */

 	PTPER = SOFTSTARTPERIOD;                        

    IOCON1bits.PENH = 0;                  /* PWM1H is controlled by I/O module */
    IOCON1bits.PENL = 0;                  /* PWM1L is controlled by I/O module */
    IOCON1bits.PMOD = 2;                  /* Push-Pull Mode */

	IOCON1bits.OVRDAT = 0;

    PWMCON1bits.DTC = 2;                  /* Deadtime is disabled added into duty cycle */
    PWMCON1bits.IUE = 0;                  /* Disable Immediate duty cycle updates */
    PWMCON1bits.ITB = 0;                  /* Select Primary Timebase mode */                                        

	FCLCON1bits.FLTMOD = 0;				  /* Latched Fault Mode */
	FCLCON1bits.FLTSRC = 0;				  /* Fault Source FLT1 - Current */
	FCLCON1bits.FLTPOL = 0;				  /* Active High */

	PHASE1 = 16;
	SPHASE1 = 16;

	#ifdef OPEN_LOOP
	PDC1 = PTPER  - DEADTIME;
	#else                         
	PDC1 = 9;
	#endif

	TRIG1 = 1740;							/* Hard coded Trigger instant needs to work for entire operating range*/
	TRGCON1bits.TRGSTRT = 7;			  
	TRGCON1bits.TRGDIV = 9;				  /* Trigger generated every ten cycles
											 Push-Pull mode so trigger occurs on every 5th PWMH on time */


	/* PWM 2 Configuration */

	IOCON2bits.PENH = 0;                  /* PWM2H is controlled by I/O module */
	IOCON2bits.PENL = 0;                  /* PWM2L is controlled by I/O module */
    IOCON2bits.PMOD = 2;                  /* Push-Pull Mode */
	IOCON2bits.SWAP = 1;                  /* SWAP high and low for transformer polarity */
	
	IOCON2bits.OVRDAT = 0;

    PWMCON2bits.DTC = 2;                  /* Deadtime disabled added into duty cycle */
    PWMCON2bits.IUE = 0;                  /* Disable Immediate duty cycle updates */
    PWMCON2bits.ITB = 0;                  /* Select Idependent Timebase mode */
   		                                         
	FCLCON2bits.FLTMOD = 0;				  /* Latched Fault Mode */
	FCLCON2bits.FLTSRC = 0;				  /* Fault Source FLT1 - Current */
	FCLCON2bits.FLTPOL = 0;				  /* Active High */

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
	TRGCON2bits.TRGDIV = 7;
	TRIG2 = PTPER>>1;

}

void initADC(void)
{

 	ADCONbits.FORM = 0;                   /* Integer data format */
    ADCONbits.EIE = 0;                    /* Early Interrupt disabled */
    ADCONbits.ORDER = 1;                  /* Convert odd channel first */
    ADCONbits.SEQSAMP = 0;                /* Select simultaneous sampling */
	ADCONbits.SLOWCLK = 1;				  /* Slow clock must be set */
	ADCONbits.ASYNCSAMP = 0;			  /* Dedicated S&H starts sampling when trigger detected */
    ADCONbits.ADCS = 5;                   /* ADC clock = FADC/6 = ~ 120MHz / 6 = 20MHz, 14*Tad = 1.4 MSPS, two SARs = 2.8 MSPS */  


	ADCPC0bits.TRGSRC0 = 5;				  /* PWM2H triggers ADCP0  */
	ADCPC0bits.TRGSRC1 = 4;				  /* PWM1H triggers ADCP1 */

	IFS6bits.ADCP0IF = 0;				  /* Clear ADC Interrupt Flags */
	IFS6bits.ADCP1IF = 0;

	IEC6bits.ADCP0IE = 0;				  /* Disabled until Softstart */
	IEC6bits.ADCP1IE = 0;				  /* Disabled until Softstart */

	IPC27bits.ADCP0IP = 5;				  /* Critical ADC Pairs have higher priority */

	ADCONbits.ADON = 1;					  /* Enable ADC now to allow time to stabilize */

}

void initCMP(void)
{

	/* Comparator used for over current protection on large load transients*/

	CMPCON1bits.INSEL = 0;				  /* Select comparator 1A */
	CMPCON1bits.RANGE = 1;				  /* Select High Range */
		
	CMPDAC1 = 1000;						  /* 1.6v / (1.65/1023), 1.6 is max input voltage to
											the analog comaparator */ 

	IFS1bits.AC1IF = 0;
	IPC4bits.AC1IP = 6;
	IEC1bits.AC1IE = 1;					  /* Enable CMP interrupt */

	CMPCON1bits.CMPON = 1;				  /* Enable Comparator */

}



void softStartRoutine(void)
{

/* Start with a high switching frequency (~300kHz) and manually increment the duty cycle until max duty cycle is obtained. 
   This will bring the voltage up to ~ 8-11V depending on input voltage. Afterwards, change to frequency control.
   
   Ideally we would push the frequency ~5x the resonant frequency and just sweep the frequecny but instead we are limited to 300kHz 
   as this is the max switching frequency for the gate drive transformers */

	softStartFlag = 1;
	 
	IFS6bits.ADCP0IF = 0;
	IFS6bits.ADCP1IF = 0;
	IEC6bits.ADCP0IE = 1;	/* Enable ADC ISRs */
	IEC6bits.ADCP1IE = 1;

	while(PDC1 < MAXDUTYCYCLE)
	{
		if(PDC1 <= 500)
		{
			PDC1 += 30; 
			PDC2 = PDC1;
		}
	
		else if ((PDC1 > 500) && (PDC1 <= 800))
		{
			PDC1 += 10; 
			PDC2 = PDC1;
		}
		else
		{	
			PDC1 += 100; 
			PDC2 = PDC1;
		}

		if(PDC1 >= MAXDUTYCYCLE)
		{
			PDC1 = MAXDUTYCYCLE;
			PDC2 = PDC1;
		}

		delay_100us(3);

	}

		/* The output voltage should never be less then ~7.5V at the end of Duty Cycle softstart and should never be greater then
		    11.5V @ 300kHz switching frequency. If it is then the input voltage may be out of spec*/

		if((outputVoltage <= SOFTSTARTMINVOLTAGE) | (outputVoltage >= SOFTSTARTMAXVOLTAGE))
		{
			faultState = FAULT_INPUTVOLTAGE;

			faultLoop();
		}


	/* Now manually reduce the frequency until the output voltage is close to the Reference Voltage (12V) */

	while(outputVoltage <= (OUTPUTVOLTAGEREFERENCE - 300))
	{
		PTPER = PTPER + 20;
	
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

	/* Need to calculate integral output for compensator history to have a smooth transition */

 	prevIntegralOutput = (int)PTPER - (NOMINALPERIOD>>1);
	prevIntegralOutput = (prevIntegralOutput << (16 - PRESCALER));
	prevIntegralOutput = prevIntegralOutput / BASEPERIOD;
	prevIntegralOutput = prevIntegralOutput << 15;
	prevIntegralOutput = prevIntegralOutput / (int) modifier;

	integralOutput = prevIntegralOutput;	/* New Integral Term */

	softStartFlag = 0;		/* End of Soft-Start, PI compensator now controlling */



}

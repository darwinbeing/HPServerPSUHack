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
                                             
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ *
** ADDITIONAL NOTES:       LLC SeriesResonant converter without output Inductor                                                                                                                                                                    
*************************************************************************************************************************************************************/
#include <xc.h>
#include "p33Fxxxx.h"
#include "define.h"

/* Configuration Bits */
_FOSCSEL(FNOSC_FRC)
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT)
_FWDT(FWDTEN_OFF)
_FPOR(FPWRT_PWR128)
_FICD(ICS_PGD2 & JTAGEN_OFF)


extern void initClock(void);
extern void initIOPorts(void);
extern void initPWM(void);
extern void initADC(void);
extern void softStartRoutine(void);
void delay_100us(unsigned int delay);
void faultCheck(void);
void faultLoop(void);

volatile extern unsigned int timerInterruptCount;

/* Local variables for fault management */
unsigned char outputVoltageCnt = 0, tempCnt = 0, faultState = 0, tankCurrentCnt = 0, faultCheckFlag = 0;
unsigned int outputVoltage = 0, tempPCB = 0, tankCurrent = 0, maxTankCurrent = 0;


int main(void)
{

    	initClock();                            				/* Initialize Primary and Auxiliary oscillators */

	initIOPorts();								/* Setup LEDs and other I/O Ports */	

	initPWM();								/* Initialize Half-bridge and synchronous rectification PWMs */

	initADC();								/* Setup ADC module and ADC triggering */

	delay_100us(30000);							/* Since Converter is stand alone we need to allow sufficient time for the 
                                                                                    DC Source to ramp up (SLOW) or for the PFC to complete its softstart.
                                                                                    This value is adjustable depending on input voltage source */

	PTCONbits.PTEN = 1;                                                     /* Enable PWM outputs */


	/* To eliminate PWM glitch at start-up, enable PWM module and then give PWM ownership of the pin */

	IOCON1bits.PENH = 1;                    /* PWM1H is controlled by PWM module */
	IOCON1bits.PENL = 1;                    /* PWM1L is controlled by PWM module */

	IOCON2bits.PENH = 1;                    /* PWM2H is controlled by PWM module */
	IOCON2bits.PENL = 1;                    /* PWM2L is controlled by PWM module */


	#ifndef OPEN_LOOP
	softStartRoutine();

	#endif

	AUXILIARY_START = OFF;			/* Disable Auxiliary as output is now regulating */
	
	
    while(1)
	{
		#ifndef OPEN_LOOP
		if(faultCheckFlag == 4)
		{
			faultCheck();
			faultCheckFlag = 0;
		}
		else
		{
			Nop();
			Nop();
			Nop();
			Nop();
			Nop();
			faultCheckFlag++;
		}
		#endif
	}
		
}


void delay_100us ( unsigned int delay)
{
   	timerInterruptCount = 0;    			/* Clear Interrupt counter flag */
    
 	PR1 = 0xFA0;					/* (100us / 25ns) = 4000 = 0xFA0 */

	IEC0bits.T1IE = 1;          			/* Enable Timer1 interrupts */
    	IPC0bits.T1IP = 5;			
    	T1CONbits.TON = 1;          			/* Enable Timer1 */
    
    while (timerInterruptCount < delay);
                                                        /* Wait for Interrupt counts to equal delay */
    
    T1CONbits.TON = 0;          			/* Disable the Timer */
}

void faultCheck()
{     
	if(tempPCB >= PCBTEMP_MAX)
	{	
		tempCnt++;

		/* To ensure over-temp, the measured PCB temp has to  exceed the max set point 250 consecutive times */

		if (tempCnt >= 250)  		
		{
			AUXILIARY_START = ON;
           		faultState = FAULT_OVERTEMP;
        		}
	}

	else
	{
		tempCnt = 0;					/* Clear count */
	}


	if ((outputVoltage >= OUTPUTVOLTAGE_MAX) | (outputVoltage <= OUTPUTVOLTAGE_MIN))
	{
		outputVoltageCnt++;
		
		/* The output voltage must be higher/lower then allowed limits for a few consecutive times
		   this eliminates false faults due to load transients */
	
		if (outputVoltageCnt >= 250)
		{
			AUXILIARY_START = ON;
			
			if(outputVoltage >= OUTPUTVOLTAGE_MAX)
			{
				faultState = FAULT_OUTPUTOVERVOLTAGE;
			}
			
			else
			{
				faultState = FAULT_OUTPUTUNDERVOLTAGE;
			}
		}
	}

	else
	{	
		outputVoltageCnt = 0;		
	}


	/* Determine maxTankCurrent based on input voltage, will need to put in same scale as measured current 
	Below is a lookup table to determine the maxTankCurrent based on the Input Voltage Range (350V-420V) */

	
          if ((PDC1 > 2700)&& (tankCurrent>15000))
	{
		maxTankCurrent = 15600;
	}
        else if((PDC1 > 2590)&& (tankCurrent>15750))
        {
            maxTankCurrent =15800;
        }
        else if((PDC1 > 2500)&& (tankCurrent>16100))
        {
            maxTankCurrent =16100;
        }
        else if((PDC1 > 2400)&& (tankCurrent>16200))
        {
            maxTankCurrent =16200;
        }
         else if((PDC1 > 2300)&& (tankCurrent>16300))
        {
            maxTankCurrent =16300;
        }
         else if((PDC1 > 2200)&& (tankCurrent>16400))
        {
            maxTankCurrent =16400;
        }
        else if((PDC1 > 2100)&& (tankCurrent>16500))
        {
            maxTankCurrent =16500;
        }
        else
        {
            maxTankCurrent=16600;
        }
        
        if(tankCurrent >= maxTankCurrent)
	{
		tankCurrentCnt++;
		if (tankCurrentCnt >= 250)
		{
			AUXILIARY_START = ON;
			faultState = FAULT_OVERCURRENT;
		}
	}
	else
	{
		tankCurrentCnt = 0;
	}
	/* If a fault is detected call the fault loop */

	if (faultState != 0) faultLoop();

}

void faultLoop()
{
	unsigned char faultCount = 0;

    	PTCONbits.PTEN = 0;					/* Disable the PWM module */
	ADCONbits.ADON = 0;					/* Disable the ADC module */

	faultCount = faultState;
	
	while(1)
	{	

		while (faultCount != 0)
		{
			LED_FAULT = ON;
			delay_100us(2500);
			LED_FAULT = OFF;
			delay_100us(2500);
			faultCount--;
		}
			faultCount = faultState;
			delay_100us(5000);
	}
}

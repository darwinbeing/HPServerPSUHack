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
#include "dsp.h"

extern char faultState, softStartFlag;
extern unsigned int outputVoltage, tempPCB, tankCurrent;
extern unsigned int synchRectDutyCycle[126][2];

volatile unsigned int timerInterruptCount = 0;

unsigned int LLCUpdatedPeriod = 0; 
unsigned char index_SR = 0, interruptCount = 4;

int VoltageError = 0, prevVoltageError = 0, prevVoltageError1 = 0,  prevVoltageError2 = 0 ,prevVoltageError3 = 0;
int PIDOutput1,PIDOutput2,PIDOutput3,PIDOutput4 ;
long SPPIDOutput,PIDOutput;
int modifier = Q15(0.999);
#ifdef dmciVariables
int dmciArray[150];
int dmciArray1[150];
int dmciArray2[150];
unsigned int dmciArrayInd = 0;
unsigned int dmciCounter = 0;
#endif
unsigned char overVoltageCounter = 0;
unsigned char underVoltageCounter = 0;


/* Voltage Mode COntrolled Compensator */
void __attribute__((interrupt, no_auto_psv)) _ADCP0Interrupt()

{
	
	tankCurrent = ((ADCBUF0) << 5);
	outputVoltage = ((ADCBUF1) << 5);

	VoltageError = (OUTPUTVOLTAGEREFERENCE -  outputVoltage);	    /* Find error */


	if (softStartFlag == 0)
	{
		if (interruptCount ==4)
		{
			/* 3P3Z compensator  in digital form
			Transfer function: from the MATLAB
			0.2711 z^3 - 0.178 z^2 - 0.1828 z + 0.2663
			---------------------------------------------------
  			      z^3 - 0.6791 z^2 - 0.7342 z + 0.4133 
			Sampling time: 2e-005
			
			y[n] =n1*e[n]-n2*e[n-1]-n3*e[n-3]+n4*e[n-4]+d2*y[n-1]+d3*y[n-2]-d4*y[n-3]
			e  => e[n]= present voltage error =VoltageError			
			y => y[n]  = present output  =PIDOutput 
			e[n-1]  = previous voltage error = prevVoltageError	 
			e[n-2] = Error previous to e[n-1] = prevVoltageError1
			e[n-3] = Error previous to e[n-2] = prevVoltageError2				
			y[n-1]  = previous output  =PIDOutput1		
			y[n-2] = Output previous to y[n-1] =PIDOutput2
			y[n-3] = Output previous to y[n-2]=PIDOutput3
			where n1,n2,n3,n4,d2,d3,d4 are the coefficients of 3P3Z compensator defined in Define.h */
			
			PIDOutput= ((__builtin_mulss((int)n1,VoltageError))>>15) -(( __builtin_mulss((int)n2,prevVoltageError))>>15) - ((__builtin_mulss((int)n3,prevVoltageError1))>>15) + (( __builtin_mulss((int)n4,prevVoltageError2))>>15); 
			PIDOutput = PIDOutput +((__builtin_mulss((int)d2,PIDOutput1))>>15)+((__builtin_mulss((int)d3,PIDOutput2))>>15) - ((__builtin_mulss((int)d4,PIDOutput3))>>15) ; 
			interruptCount = 0;
		}
		interruptCount++;

		
		SPPIDOutput = PIDOutput;  
	
		if(SPPIDOutput > SPPIDOutputCLAMP)		//saturating to  +1 and -1 				
		{
			SPPIDOutput = SPPIDOutputCLAMP;
		
		}
		else if(SPPIDOutput < - SPPIDOutputCLAMP)
		{
			SPPIDOutput = - SPPIDOutputCLAMP;
		}

	    	LLCUpdatedPeriod = (((__builtin_mulss((int)SPPIDOutput,(int)BASEPERIOD)) >> (15 - PRESCALER)) + NOMINALPERIOD) >> 1;
	    	
	    	
		/* Clamp frequency to minimum allowable frequency*/ 
		
		if(LLCUpdatedPeriod  >= MAXPERIOD)					
		{	
			PTPER = MAXPERIOD;
			PDC1 = PTPER - DEADTIME;
			PDC2 = 2546 - SRDEADTIME;		/* From synchRectDutyCycle */
		}

		/* Clamp frequency to maximum allowable frequency*/ 
		
		else if(LLCUpdatedPeriod <= MINPERIOD)
		{
			PTPER = MINPERIOD;
			PDC1 = PTPER - DEADTIME;
			PDC2 = PTPER - SRDEADTIME;
		}

		/* Normal operating condition */

		else
		{
			PTPER = LLCUpdatedPeriod;
			PDC1 = PTPER - DEADTIME;

			/* Check if New frequency <= resonant frequency, if so need SR Duty Cycle from lookup table */

			if(LLCUpdatedPeriod >= RESONANTPERIOD)
			{
				index_SR = (PTPER & 0xFFF8) - RESONANTPERIOD;
				if(index_SR <= 0)
				{
					index_SR = 0;
				}
				PDC2 = (synchRectDutyCycle[index_SR >> 3][1]) - SRDEADTIME;
			
			}
		
			/* If frequency is above resonance directly update secondary duty cycle */

			else
			{
				PDC2 = PTPER - SRDEADTIME;
			}
		}
			
		
		TRIG2 = (PTPER >> 1)+100;            		/* Trigger to start ADC conversion */

		
		prevVoltageError = VoltageError;		/* Update previous voltage error */
		prevVoltageError1 = prevVoltageError;
		prevVoltageError2 = prevVoltageError1;
		prevVoltageError3 = prevVoltageError2;
		
		/* Upadation of previous Compensator outputs */
		
		PIDOutput1= PIDOutput;
		PIDOutput2= PIDOutput1;
		PIDOutput3= PIDOutput2;


		if (tankCurrent < 6000)
		{
			if (PTPER < MAXPERIOD_NOLOAD)
			{
				/* Fault OverVoltage */
				overVoltageCounter++;
			}
			else if (PTPER > MINPERIOD_NOLOAD)
			{
				/* Fault UnderVoltage */
				underVoltageCounter++;
			}
           			 else      
			{
				overVoltageCounter = 0;
				underVoltageCounter = 0;
			}			
		}
		else if (tankCurrent < 11000)
		{
			if (PTPER < MAXPERIOD_HALFLOAD)	
		    	{
                               /*Fault OverVoltage */
                               overVoltageCounter++;
		    	}
       			else if (PTPER > MINPERIOD_HALFLOAD)
		        {
                              /* Fault UnderVoltage */
                                underVoltageCounter++;
		        }
                        else
                        {
                                overVoltageCounter = 0;
                                underVoltageCounter = 0;
                        }
		}
		else 
                {
			if (PTPER < MAXPERIOD_LOAD)
			{
				/* Fault OverVoltage */
				overVoltageCounter++;
			}
			else if (PTPER > MINPERIOD_LOAD)
			{
				/* Fault UnderVoltage */
				underVoltageCounter++;
			}   
			else
		 	{
				overVoltageCounter = 0;
				underVoltageCounter = 0;
		  	}	
		}
		
		if((overVoltageCounter >= 200) || (underVoltageCounter >= 200))
		{
			AUXILIARY_START = ON;
			faultState = FAULT_INPUTVOLTAGE;
		}

	 }


      #ifdef dmciVariables
	if(softStartFlag == 0)
	{
		if(dmciCounter == 1)
		{		
                        dmciArray[dmciArrayInd] = ADCBUF0;
			dmciArray1[dmciArrayInd] = tankCurrent;
			dmciArrayInd++;
			dmciCounter = 0;
		}
		
		dmciCounter++;
	
		if(dmciArrayInd == 150)
		{
			dmciArrayInd = 0;
		}
	}
     #endif

	IFS6bits.ADCP0IF = 0;
	ADSTATbits.P0RDY = 0;
}

void __attribute__((interrupt, no_auto_psv)) _ADCP1Interrupt()
{
	tempPCB = ADCBUF3;              

	IFS6bits.ADCP1IF = 0;
	ADSTATbits.P1RDY = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt()
{
  timerInterruptCount ++; 	/* Increment interrupt counter */
  IFS0bits.T1IF = 0; 		/* Clear Interrupt Flag */
}


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

int VoltageError = 0, prevVoltageError = 0;
int CurrentError = 0, prevCurrentError = 0, prevCurrentError1 = 0;
int CPIDOutput1,CPIDOutput2 ;
long proportionalOutput = 0,integralOutput = 0, PIOutput = 0, PIFinalOutput = 0,CPIDOutput;
int modifier = Q15(0.995);
#ifdef dmciVariables
int dmciArray[150];
int dmciArray1[150];
int dmciArray2[150];
unsigned int dmciArrayInd = 0;
unsigned int dmciCounter = 0;
#endif
unsigned char overVoltageCounter = 0;
unsigned char underVoltageCounter = 0;



void __attribute__((interrupt, no_auto_psv)) _ADCP0Interrupt()

{
	tankCurrent = ((ADCBUF0) << 5);
	outputVoltage = ((ADCBUF1) << 5);


	VoltageError = (OUTPUTVOLTAGEREFERENCE -  outputVoltage);	    /* Voltage error */


	if (softStartFlag == 0)
	{
		if (interruptCount ==4)
		{
			integralOutput = integralOutput+((__builtin_mulss((int)IGAIN,VoltageError)) >> 14);    /* Calculate I term */
		
		interruptCount = 0;
		}	
		interruptCount++;
		if(integralOutput > INTEGRALCLAMP)
		{
			integralOutput = INTEGRALCLAMP;
		}

		if(integralOutput <-INTEGRALCLAMP)
		{
			integralOutput = -INTEGRALCLAMP;
		}	

		proportionalOutput = ((__builtin_mulss((int)PGAIN,prevVoltageError)) >> 12); 	/* Calculate P term */
	
		PIOutput = (long)proportionalOutput + (long)integralOutput ;                   /*PI output */

		if(PIOutput >  SPPIDOutputCLAMP )                                             /* Satuaration from +1 t0 -1*/
		{ 
			PIOutput =  SPPIDOutputCLAMP;
		}		
	
		else if(PIOutput < -SPPIDOutputCLAMP) 
		{
			PIOutput = -SPPIDOutputCLAMP;	
		}
	
		PIFinalOutput = __builtin_mulss((int)PIOutput,(int)modifier) >> 15;
	
	    	CurrentError = (int) (PIFinalOutput)- tankCurrent ;                         /*Current error*/
	    	
                        /* 2-pole 2-zero Digital compensator
                        Transfer function: from the MATLAB
			0.03558 z^2 - 0.05895 z + 0.03495
			-------------------------------
                            z^2 - 1.977 z + 0.9767
                         Sampling time: 2e-005
			y[n] =n1*e[n]-n2*e[n-1]+n3*e[n-3]+d2*y[n-1]-d3*y[n-2]
			e  => e[n]= present current error =CurrentError
			y => y[n]  = present output  =CPIDOutput
			e[n-1]  = previous current error = prevCurrentError
			e[n-2] = Error previous to e[n-1] = prevCurrentError1
			y[n-1]  = previous output  =CPIDOutput1
			y[n-2] = Output previous to y[n-1] =CPIDOutput2
			where n1,n2,n3,d2,d3 are the coefficients of 3P3Z compensator defined in Define.h */
	    	
	    	CPIDOutput= ((__builtin_mulss((int)ni1,CurrentError))>>15) -(( __builtin_mulss((int)ni2,prevCurrentError))>>15) +((__builtin_mulss((int)ni3,prevCurrentError1))>>15); 
		CPIDOutput = CPIDOutput +((__builtin_mulss((int)di2,CPIDOutput1))>>14)-((__builtin_mulss((int)di3,CPIDOutput2))>>15) ; 
		
	    	CPIDOutput =  (int)CPIDOutput;  
	
		if(CPIDOutput > SPPIDOutputCLAMP)		//saturating to  +1 and -1 				
		{
			CPIDOutput = SPPIDOutputCLAMP;
		
		}
		else if(CPIDOutput < - SPPIDOutputCLAMP)
		{
			CPIDOutput = - SPPIDOutputCLAMP;
		}
	    
	       LLCUpdatedPeriod = (((__builtin_mulss((int) CPIDOutput,(int)BASEPERIOD)) >> (15 - PRESCALER)) + NOMINALPERIOD) >> 1;
	
		
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
		
		prevVoltageError = VoltageError ;		/* Update previous Voltage error */
		
		prevCurrentError = CurrentError;		/* Update previous Current error */
		prevCurrentError1 = prevCurrentError;
	
		/* Upadation of previous Compensator outputs */
		
		CPIDOutput1=  (int) CPIDOutput;
		CPIDOutput2=CPIDOutput1;
		
		if (tankCurrent < 7000)
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
		else if (tankCurrent < 11500)
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
                        dmciArray[dmciArrayInd] = tankCurrent;
			dmciArray1[dmciArrayInd] = PTPER;
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


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
#include "dsp.h"

extern char faultState, softStartFlag;
extern unsigned int averageInputVoltage, outputVoltage, tempPCB, tankCurrent;
extern unsigned int synchRectDutyCycle[126][2];

volatile unsigned int timerInterruptCount = 0;

unsigned int LLCUpdatedPeriod = 0; 
unsigned char index_SR = 0, interruptCount = 4;

int voltageError = 0, prevVoltageError = 0;
int proportionalOutput = 0, voltageDecoupleTerm = 0, nominalVoltage = 0;
long integralOutput = 0, PIOutput = 0, PIFinalOutput = 0;

unsigned char inputVoltageArrayCnt = 0; 
unsigned int inputVoltageArray[8];
long unsigned int inputVoltageSum = 0;
unsigned int inputVoltage = 0; 


#ifdef dmciVariables
int dmciArray[150];
int dmciArray2[150];
unsigned int dmciArrayInd = 0;
unsigned int dmciCounter = 0;
#endif


//int modifier = Q15(0.833);//420
//int modifier = Q15(0.875);//400
//int modifier = Q15(0.92);//380
int modifier = Q15(0.999);//350


void __attribute__((interrupt, no_auto_psv)) _ADCP0Interrupt()
{
	tankCurrent = (ADCBUF0 << 5);
	outputVoltage = (ADCBUF1 << 5);

	voltageError = (OUTPUTVOLTAGEREFERENCE -  outputVoltage);	    /* Find error */

	if (softStartFlag == 0)
	{
		if (interruptCount == 4)
		{
			integralOutput = integralOutput + ((__builtin_mulss((int)IGAIN,voltageError))>>15);    /* Calculate I term */
			interruptCount = 0;
		}
	
		interruptCount++;

		if(integralOutput > INTEGRALCLAMP)
		{
			integralOutput = INTEGRALCLAMP;
		}

		if(integralOutput < -INTEGRALCLAMP)
		{
			integralOutput = -INTEGRALCLAMP;
		}	

		proportionalOutput = ((__builtin_mulss((int)PGAIN,voltageError))>>15); 	/* Calculate P term */

		voltageDecoupleTerm = ((__builtin_mulss((int)DECOUPLING,(int)outputVoltage ))>>15);	/* Including scale factor */

		PIOutput = (long)proportionalOutput + (long)integralOutput + voltageDecoupleTerm - NOMINALVOLTAGE;

		if(PIOutput > PICLAMP )
		{ 
			PIOutput = PICLAMP;
		}		
	
		else if(PIOutput < -PICLAMP) 
		{
			PIOutput = -PICLAMP;	
		}

		PIFinalOutput = __builtin_mulss((int)PIOutput,(int)modifier)>>15;


		LLCUpdatedPeriod = (((__builtin_mulss((int)PIFinalOutput,(int)BASEPERIOD))>> (15-PRESCALER)) + NOMINALPERIOD) >> 1;


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
				PDC2 = (synchRectDutyCycle[index_SR>>3][1])-SRDEADTIME;
			}
		
			/* If frequency is above resonance directly update secondary duty cycle */

			else
			{
				PDC2 = PTPER - SRDEADTIME;
			}
		}
			TRIG2 = PTPER >> 1;
			prevVoltageError = voltageError;		/* Update previous error with voltage error */
	}

#ifdef dmciVariables
	if(softStartFlag == 0)
	{
		if(dmciCounter == 1)
		{		
			dmciArray[dmciArrayInd] = PDC1;
			dmciArray2[dmciArrayInd] = PDC2;
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
	inputVoltage = (ADCBUF2 << 5) + 2000;
	tempPCB = ADCBUF3;

	inputVoltageSum = inputVoltageSum + inputVoltage - inputVoltageArray[inputVoltageArrayCnt];
	averageInputVoltage = inputVoltageSum >> 3;

	inputVoltageArray[inputVoltageArrayCnt++] = inputVoltage;

	if(inputVoltageArrayCnt == 8)
	{
		inputVoltageArrayCnt = 0;
	}

	IFS6bits.ADCP1IF = 0;
	ADSTATbits.P1RDY = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt()
{
  timerInterruptCount ++; 	/* Increment interrupt counter */
  IFS0bits.T1IF = 0; 		/* Clear Interrupt Flag */
}

void __attribute__((__interrupt__, no_auto_psv)) _CMP1Interrupt()
{
	AUXILIARY_START = ON;
	PTCONbits.PTEN = 0;					/* Disable the PWM module */
	IFS1bits.AC1IF = 0;					/* Clear Interrupt Flag */
	
	faultState = FAULT_OVERCURRENT;
}


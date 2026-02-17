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
#include "dsp.h"
#include "delay.h"

uint8_t outputVoltageCnt = 0, tempCnt = 0, tankCurrentCnt = 0;
uint8_t overVoltageCounter = 0;
uint8_t underVoltageCounter = 0;
uint8_t index_SR = 0;

uint16_t maxTankCurrent = 0;
uint16_t outputVoltage = 0, tempPCB = 0, tankCurrent = 0, tankCurrent_pro = 0;

#ifdef dmciVariables
int16_t dmciArray[150];
int16_t dmciArray1[150];
int16_t dmciArray2[150];
uint16_t dmciArrayInd = 0;
uint16_t dmciCounter = 0;
#endif

extern uint8_t faultState;
extern uint8_t softStartFlag;

void faultLoop(void);

/* Input under voltage and over voltage protection code */
void __attribute__((interrupt, no_auto_psv)) _ADCAN1Interrupt()
{
    // ADC reading of tank current and output voltage during soft-start 
    if (softStartFlag == 1)
    {
        tankCurrent = (ADCBUF0);
        outputVoltage = (ADCBUF1);
    }
    
    /* Current protection */  
    if (softStartFlag == 0)
    {
        if (tankCurrent < 750)                     
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
        else if (tankCurrent < 1375)               
        {
            if (PTPER < MAXPERIOD_HALFLOAD)	
            {
                /* Fault OverVoltage */ 
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
        
    IFS6bits.ADCAN1IF  = 0;             /* Clear ADC AN0 interrupt flag */       
}

/* PCB temp measurement */
void __attribute__((interrupt, no_auto_psv)) _ADCAN3Interrupt()
{
	tempPCB = ADCBUF3;                  /* ADC reading of temperature */

	IFS7bits.ADCAN3IF  = 0;             /* Clear ADC AN3 interrupt flag */   
}

/* PCB temp, Output under voltage, over voltage and tank over current 
 * protection */
void faultCheck(void)
{     
	if(tempPCB >= PCBTEMP_MAX)
	{	
		tempCnt++;

		/* To ensure over-temp, the measured PCB temp has to  exceed the max 
         * set point 250 consecutive times */

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
		
		/* The output voltage must be higher/lower then allowed limits for a 
         * few consecutive times this eliminates false faults due to load 
         * transients */
	
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

	/* Determine maxTankCurrent based on input voltage, will need to put in 
     * same scale as measured current Below is a lookup table to determine the 
     * maxTankCurrent based on the Input Voltage Range (350V-420V) */
	tankCurrent_pro = (tankCurrent << 3);
    
        if ((PDC1 > 2700)&& (tankCurrent_pro>15000))            
        {
            maxTankCurrent = 15600;                         
        }
        else if((PDC1 > 2590)&& (tankCurrent_pro>15750))        
        {
            maxTankCurrent =15800;                          
        }
        else if((PDC1 > 2500)&& (tankCurrent_pro>16100))        
        {
            maxTankCurrent =16100;                          
        }
        else if((PDC1 > 2400)&& (tankCurrent_pro>16200))        
        {
            maxTankCurrent =16200;                          
        }
         else if((PDC1 > 2300)&& (tankCurrent_pro>16300))       
        {
            maxTankCurrent =16300;                          
        }
         else if((PDC1 > 2200)&& (tankCurrent_pro>16400))       
        {
            maxTankCurrent =16400;                          
        }
        else if((PDC1 > 2100)&& (tankCurrent_pro>16500))        
        {
            maxTankCurrent =16500;                          
        }
        else
        {
            maxTankCurrent=16600;                           
        }
         
            if(tankCurrent_pro >= maxTankCurrent)
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

	if (faultState != 0) 
        faultLoop();

}

void faultLoop()
{
	uint8_t faultCount = 0;

    PTCONbits.PTEN = 0;                     /* Disable the PWM module in Fault 
                                            * Condition*/ 
	ADCON1Lbits.ADON  = 0;                  /* Disable the ADC module in Fault 
                                            * Condition */ 
         
    faultCount = faultState;                /* LED blink count is faultCount; 
                                             * this reflects fault state also */
	
	while(1)
	{	
		while (faultCount != 0)
		{
			LED_FAULT = ON;
            Delay(Delay_250mS_Cnt);
			LED_FAULT = OFF;
			Delay(Delay_250mS_Cnt);
			faultCount--;
		}
			faultCount = faultState;
			Delay(Delay_500mS_Cnt);
	}
}


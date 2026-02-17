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
#include "delay.h"

uint8_t softStartFlag = 0;
uint8_t faultState = 0;
uint16_t newPDC1, newPDC2;

extern uint16_t newPTPER;
extern uint16_t synchRectDutyCycle[126][2];
extern uint16_t outputVoltage;
extern uint8_t index_SR;

void faultLoop(void);

void softStartRoutine(void)
{
/* Start with a high switching frequency (~300kHz) and manually increment the 
 * period (decrement frequency) until max period is obtained. This will bring 
 * the voltage up to ~ 8-11V depending on input voltage. Afterwards, change to 
 * frequency control.
   
 * Ideally we would push the frequency ~5x the resonant frequency and just sweep 
 * the frequency but instead we are limited to 300kHz as this is the max 
 * switching frequency for the gate drive transformers */

    softStartFlag = 1;
	 
    IFS6bits.ADCAN0IF  = 0;             /* Clear ADC interrupt flag */
    IFS7bits.ADCAN3IF  = 0;             /* Clear ADC interrupt flag */
    IFS0bits.T2IF = 0;                  /* Clear Timer-2 interrupt flag */
	    
    IEC6bits.ADCAN0IE  = 1;             /* Enable ADC AN0 interrupt */
    ADIELbits.IE0 = 1;                  /* Enable ADC common and individual 
                                         * interrupt */
    
    IEC7bits.ADCAN3IE  = 1;             /* Enable ADC AN3 interrupt */
    ADIELbits.IE3 = 1;                  /* Enable ADC common and individual 
                                         * interrupt */
        
    /* Manually reduce the frequency until the output voltage is close to the 
     * Reference Voltage (12V) */
	while(outputVoltage <= (OUTPUTVOLTAGEREFERENCE - 12))
	{
        newPTPER = newPTPER + 50;       /* Soft-start Period is incremented by 
                                         * fix count */
	
		if(newPTPER >= MAXPERIOD)       /* Check for max period */
		{
			newPTPER = MAXPERIOD;
			faultState = FAULT_SOFTSTART;
			faultLoop();
		}

		newPDC1 = newPTPER - DEADTIME;  /* Added dead-time */

		if(newPTPER >= RESONANTPERIOD)  /* Period greater than resonance period 
                                         * set PDC2 */
		{
			index_SR = (newPTPER & 0xFFF8) - RESONANTPERIOD;
			if(index_SR <= 0)
			{
				index_SR = 0;
			}
			newPDC2 = (synchRectDutyCycle[index_SR>>3][1])-SRDEADTIME;
		}
		else
		{
			newPDC2 = newPTPER - SRDEADTIME;
		}
		
		Delay_Us(Delay200us);       
	}
    softStartFlag = 0;                  /* End of Soft-Start, Compensator now 
                                         * controlling */    
    IEC0bits.T2IE = 1;                  /* Enable Timer-2 interrupts */    
}



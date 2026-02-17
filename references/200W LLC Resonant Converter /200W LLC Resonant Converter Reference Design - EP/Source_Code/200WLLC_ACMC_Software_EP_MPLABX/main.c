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
                                             
/*******************************************************************************
** ADDITIONAL NOTES:  LLC resonant converter Average Current Mode Control Firmware                                                                                                                                                                     
*******************************************************************************/


#include <stdint.h>
#include "p33Exxxx.h"
#include "define.h"
#include "main.h"
#include "delay.h"

/* Configuration Bits */
#pragma config FNOSC = FRC
#pragma config FCKSM = CSECMD
#pragma config OSCIOFNC = OFF
#pragma config POSCMD = XT
#pragma config WDTEN = OFF
#pragma config ICS = PGD2
#pragma config JTAGEN = OFF
#pragma config PWMLOCK = OFF

/* Interrupt Priority Level For Alternate Working Registers */
#pragma config CTXT1 = IPL6
#pragma config CTXT2 = IPL5

/* Declaration of Variables */
uint8_t faultCheckFlag = 0;

int main(void)
{
    initClock();                        /* Initialize Primary and Auxiliary 
                                         * oscillators */

	initIOPorts();						/* Setup LEDs and other I/O Ports */	

	initPWM();                          /* Initialize Half-bridge and 
                                         * synchronous rectification PWMs */

	initADC();                          /* Setup ADC module and ADC triggering */

    intControlConst();                  /* Initialize control constants */
    
    init_TIMER2();                      /* Initialize timer2 interrupt */
    
    Delay(Delay_3S_Cnt);                /* Initial delay for input voltage 
                                         * ramp-up */
        				
    /* Since Converter is stand alone we need to allow sufficient time for the 
      DC Source to ramp up (SLOW) or for the PFC to complete its softstart.
      This value is adjustable depending on input voltage source */

    init_PWMstart();                    /* Initialization for PWM module 
                                         * starting */
    
    #ifndef OPEN_LOOP
        softStartRoutine();             /* Open-loop soft-start */
    #endif

	AUXILIARY_START = OFF;            /* Disable Auxiliary as output is now 
                                       * regulating */
		
    while(1)
	{
        #ifndef OPEN_LOOP
        /* Fault checking */
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
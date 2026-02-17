/**********************************************************************
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
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
*
*******************************************************************************/

#include "p33Fxxxx.h"


#define TRUE              1
#define FALSE             0
#define ON                1
#define OFF               0

/* If OPEN_LOOP is defined then the frequency and duty cycle will be fixed. The period is defined according to
   SOFTSTARTPERIOD below. Faults will be disabled. When in open loop there is not a soft-start routine so do not start
   the converter with much load */

//#define OPEN_LOOP 		  1


/* This #define allows the use of two arrays and various variables for dmci debugging/viewing in the ADCP0 
	interrupt service routine.  */ 

//#define dmciVariables		1


#define LED_FAULT           		LATBbits.LATB12
#define LED_1         		 		LATBbits.LATB5
#define LED_2         		 		LATBbits.LATB11

#define AUXILIARY_START				LATBbits.LATB15


#define FAULT_INPUTVOLTAGE			1			/* Input under or over voltage */
#define FAULT_OVERCURRENT			2			/* Output over current */
#define FAULT_OUTPUTOVERVOLTAGE		3			/* 12V output over voltage */
#define FAULT_OUTPUTUNDERVOLTAGE	4			/* 12V output under voltage */
#define FAULT_OVERTEMP    			5			/* Over temperature fault */
#define FAULT_SOFTSTART				6			/* Failed Soft-start */


#define PCBTEMP_MAX 	  			375			/* PCB Temperature Check 
										  	       Declare Fault if PCB temperature is greater than 55C 
											       Vout = (Ta * Tc) + V0C, where Ta is the ambient temperature
										 	       Tc is the temperature coefficent and V0C is the voltage at 0C.
											       Vout = (70C * 10mV/C) + .5V = 1.2V, Convert to ADC reading
											       (1.2 * 1023) / 3.3 = 372. Add some factor*/ 

#define INPUTVOLTAGE_MIN			24800			/* Input voltage check
												   [4.7k / ((3*110k) + 4.7k)] * 350/2
												   (Half-Bridge mid point voltage). Convert to ADC reading and then Q15*/

#define INPUTVOLTAGE_MAX			29200			/* Inut voltage check
												   [4.7k / ((3*110k) + 4.7k)] * 420/2
												   (Half-Bridge mid point voltage). Convert to ADC reading and then Q15 */
		

#define OUTPUTVOLTAGE_MIN			24074			/* Output voltage check
												   [1.5k / (1.5k + 4.99k)] * 10.5
												   Convert to ADC reading and Q15 format */

#define OUTPUTVOLTAGE_MAX			30952			/* Output voltage check
												   [1.5k / (1.5k + 4.99k)] * 13.5
												   Convert to ADC reading and Q15 format*/

#ifdef OPEN_LOOP

#define SOFTSTARTPERIOD			2230
												//2940		/*~160kHz*/
												//2850		/*~165kHz*/
												//2760		/*~170kHz*/
												//2690		/*~175kHz*/
												//2620		/*~180kHz*/
												//2550		/*~185kHz*/
												//2480		/*~190kHz*/
												//2400		/*~195kHz*/
												//2350		/*~200kHz*/
												//2290		/*~205kHz*/
												//2230		/*~210kHz*/
												//2180		/*~215kHz*/
												//2130		/*~220kHz*/
												//2080		/*~225kHz*/
												//2040		/*~230kHz*/
												//2000		/*~235kHz*/
												//1960		/*~240kHz*/
												//1920		/*~245kHz*/
												//1880		/*~250kHz*/
												//1840		/*~255kHz*/	

#else

#define SOFTSTARTPERIOD				1560		/* PTPER = ((1 / 300kHz) / 1.07ns) = 3115 / 2 (push-pull) = 1558 where 300kHz 
											       is the desired initial switching frequency for the softstart. 
												   The Gate Drive Transformers maximum rated switching frequency (300kHz) */


#endif

#define MAXDUTYCYCLE				1300		/* Soft start period minus dead time */

#define RESONANTPERIOD				2232		/* Resonant Period */
		
#define MINPERIOD					1870		/* MINPERIOD = PTPER = ((1 / 250kHz) / 1.07ns) / 2 = 1870, where 250kHz
													is the maximum operating range */

#define MAXPERIOD					3222		/* MAXPERIOD = PTPER = ((1 / 145kHz) / 1.07ns) / 2 = 3222, where 145kHz
													is the minimum allowed as defined by the gain of the converter and input 
													voltage operating range (ensure inductive region) */					

#define DEADTIME					250			/* Dead-time of Halh-Bridge Converter */
#define SRDEADTIME					350			/* Dead-time for Synchronous Rectifier */ 

#define SOFTSTARTMINVOLTAGE			14000		/* The minimum output voltage at startup before changing the frequency
													is greater than 8V (@350V input full load) 300kHz switching frequency) */

#define SOFTSTARTMAXVOLTAGE			26366		/* Correspondes to 11.5V output */

#define OUTPUTVOLTAGEREFERENCE 		27500 		/* Reference voltage is from resistor divider circuit 
												   	(1.5kOhm / (1.5kOhm + 4.99kOhm)) * 12V = 2.774V
												   	Now calculate expected ADC value (2.774V * 1023)/3.3V = 860 
								    			   	Then left shift by 5 for Q15 format (860 * 32) = 27520 = 0x6B80 */


#define BASEPERIOD   (MAXPERIOD << 1)  			/* Model uses complementary mode so MAXPERIOD * 2 = 6.6667 us */
//#define NOMINALPERIOD 4717					/* 200kHz */
#define NOMINALPERIOD 5147						/* 180kHz */


/* Coefficients for Tn - 1/140k operation */
#define PGAIN Q15(0.3624)
#define IGAIN Q15(0.0996)
#define DECOUPLING Q15(0.7599)


#define PRESCALER 1
#define INTEGRALCLAMP (65534>>PRESCALER)
#define PICLAMP (32767>>PRESCALER)

//nominalVoltage = ((__builtin_mulss((int)DECOUPLING,(int)OUTPUTVOLTAGEREFERENCE ))>>15);
#define NOMINALVOLTAGE 20896  					/* coeff for Tn 1/140k */

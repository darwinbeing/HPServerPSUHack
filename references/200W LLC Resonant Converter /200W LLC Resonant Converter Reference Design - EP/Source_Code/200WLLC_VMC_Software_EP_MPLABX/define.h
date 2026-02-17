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

#define TRUE              1
#define FALSE             0
#define ON                1
#define OFF               0

// If OPEN_LOOP is defined then the frequency and duty cycle will be fixed. The period is defined according to
// SOFTSTARTPERIOD below. Faults will be disabled. When in open loop there is not a soft-start routine so do not start
// the converter with much load

//#define OPEN_LOOP                             1


// This #define allows the use of two arrays and various variables for dmci debugging/viewing in the ADCP0 interrupt service routine. 

#define dmciVariables                           1

#define LED_FAULT           		LATBbits.LATB12
#define LED_1         		 		LATBbits.LATB5
#define LED_2         		 		LATBbits.LATB11
#define AUXILIARY_START             LATBbits.LATB15

#define FAULT_INPUTVOLTAGE			1               // Input under or over voltage
#define FAULT_OVERCURRENT			2               // Output over current
#define FAULT_OUTPUTOVERVOLTAGE     3               // 12V output over voltage
#define FAULT_OUTPUTUNDERVOLTAGE	4               // 12V output under voltage
#define FAULT_OVERTEMP    			5               // Over temperature fault
#define FAULT_SOFTSTART				6               // Failed Soft-start

#define FCY                         70000000UL                              /*System Clock Frequency 70MHz*/
#define FSAMPLING                   200000UL                                /*Timer-2 Interrupt Frequency - Sampling 
                                                            Frequency of VMC Controller, 200 kHz*/  
#define T2PERIOD                (uint16_t)((float)FCY/(float)FSAMPLING) /*Timer-2 Period corresponds 
                                                            to Sampling Frequency */

#define ADC_REF                     (float)3.3                  /*ADC Reference voltage*/
#define KVO                         (float)0.231125             /*Output voltage sensor resisotor divider: [1.5k / (1.5k + 4.99k)]*/  
#define VOMIN                       (float)10.5                 /*Output under-voltage limit voltage*/
#define VOMAX                       (float)13.5                 /*Output over-voltage limit voltage*/
#define VOUT                        (float)12                   /*Desired Output voltage*/ 
#define ADCBITS                     (uint16_t)12                /*ADC bits*/
#define ADCMAXCOUNT                 (uint16_t)(4096)            /*Max ADC Count*/ 

#define OUTPUTVOLTAGE_MIN       (uint16_t)((float)VOMIN*(float)KVO*(uint16_t)ADCMAXCOUNT/(float)ADC_REF)        /*OUTPUTVOLTAGE_MIN = 3012*/
#define OUTPUTVOLTAGE_MAX       (uint16_t)((float)VOMAX*(float)KVO*(uint16_t)ADCMAXCOUNT/(float)ADC_REF)        /*OUTPUTVOLTAGE_MAX = 3872*/ 

#define OUTPUTVOLTAGEREFERENCE      860            // Reference voltage is from resistor divider circuit
                                                    // (1.5kOhm / (1.5kOhm + 4.99kOhm)) * 12V = 2.774V
                                                    // Now calculate expected ADC value (2.774V * 4095)/3.3V = 3440

#define KTEMP                       (float)0.01                 // Temp sensor sensitivity 10mV/C
#define TEMPMAX                     (float)70.0                 // Maximum temprature value for protection
#define OFFSET_TEMP                 (float)0.5
#define PCBTEMP_MAX             (uint16_t)( ( ( (float)TEMPMAX*(float)KTEMP) + (float)OFFSET_TEMP )*(uint16_t)ADCMAXCOUNT/(float)ADC_REF)
                                                                /* PCBTEMP_MAX = 1489*/
                                                                // PCB Temperature Check
                                                                // Declare Fault if PCB temperature is greater than 55C 
                                                                // Vout = (Ta * Tc) + V0C, where Ta is the ambient temperature
                                                                // Tc is the temperature coefficent and V0C is the voltage at 0C.
                                                                // Vout = (70C * 10mV/C) + .5V = 1.2V, Convert to ADC reading
                                                                // (1.2 * 4095) / 3.3 = 1489 Add some factor

#define MINPERIOD_NOLOAD			2850                        // Use switching frequency to determine min/max input voltage
#define MINPERIOD_HALFLOAD          2920
#define MINPERIOD_LOAD				3100
#define MAXPERIOD_NOLOAD			2000
#define MAXPERIOD_HALFLOAD          2150
#define MAXPERIOD_LOAD				2240	

#define F_IN                        7370000UL  
#define F_PLLO                      140000000UL
#define PLL_PRI                     (float)0
#define PLL_POST                    (float)0

#define PLL_DIV                 (uint16_t) ( ( (F_PLLO*((float)PLL_PRI + 2)*((float)PLL_POST + 2))/F_IN )-2 )
#define PLLPRIX                 (uint16_t) (PLL_PRI/1)
#define PLLPOSTX                (uint16_t) (PLL_POST/1)


#define F_AUX                       117000000UL                   /*Auxiliary clock frequency*/
#define T_AUX                   (float) (1/((float)F_AUX))    /*Auxiliary clock period*/

#define F_HRES                  (float) (F_AUX *8)           /* High resolution clock freq*/
#define T_HRES                  (float) (1/((float)F_HRES))  /* High Resolution clock period*/

#ifdef OPEN_LOOP
#define F_SOFTSTART                 300000UL     
//#define SOFTSTARTPERIOD                         1560 //2350
												//2940		~160kHz
												//2850		~165kHz
												//2760		~170kHz
												//2690		~175kHz
												//2620		~180kHz
												//2550		~185kHz
												//2480		~190kHz
												//2400		~195kHz
												//2350		~200kHz
												//2290		~205kHz
												//2230		~210kHz
												//2180		~215kHz
												//2130		~220kHz
												//2080		~225kHz
												//2040		~230kHz
												//2000		~235kHz
												//1960		~240kHz
												//1920		~245kHz
												//1880		~250kHz
												//1840		~255kHz	

#else
#define F_SOFTSTART                 300000UL
#endif

#define FACTOR_PUSHPULL             (float)0.5

#define T_SOFTSTART             (float)(1/(float)F_SOFTSTART)
#define SOFTSTARTPERIOD         (uint16_t)((float)FACTOR_PUSHPULL*(float)T_SOFTSTART/(float)T_HRES)     /*SOFTSTARTPERIOD = 1560*/

#define DEADTIME                  250             // Dead-time of Halh-Bridge Converter
#define SRDEADTIME                350             // Dead-time for Synchronous Rectifier 

#define MAXDUTYCYCLE				1300            // Soft start period minus dead time

#define F_RESONANCE                 208000UL
#define T_RESONANCE             (float)(1/(float)F_RESONANCE)
#define RESONANTPERIOD            2232            // Resonant Period

#define F_LLCMIN                    250000UL
#define T_LLCMIN                (float)(1/(float)F_LLCMIN)
#define MINPERIOD                 1870            // MINPERIOD = PTPER = ((1 / 250kHz) / 1.07ns) / 2 = 1870, where 250kHz
                                                    //	is the maximum operating range

#define F_LLCMAX                    145000UL
#define T_LLCMAX                (float)(1/(float)F_LLCMAX)
#define MAXPERIOD                 3222            // MAXPERIOD = PTPER = ((1 / 145kHz) / 1.07ns) / 2 = 3222, where 145kHz
                                                    // is the minimum allowed as defined by the gain of the converter and input 
                                                    // voltage operating range (ensure inductive region)					

#define F_LLCBASE                   212000UL
#define T_LLCBASE               (float)(1/(float)F_LLCMAX)
#define BASEPERIOD                  4400            // Model uses Push-Pull mode so BASE PERIOD is chosen close to resonant frequency of the LLC

#define F_LLCNOMINAL                182000UL
#define T_LLCNOMINAL            (float)(1/(float)F_LLCMAX)
#define NOMINALPERIOD               5140            // 180kHz

#define INITIAL_PHASE1          16
#define INITIAL_SPHASE1         16
#define INITIAL_PDC1            16
#define INITIAL_PDC2            9
#define TRIG1_COUNT             1740


////////////////////////////////////////////////////////////////////////////////
// (C) 2015 Microchip Technology Inc.
//
// MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
// derivatives created by any person or entity by or on your behalf, exclusively
// with Microchip's products.  Microchip and its licensors retain all ownership
// and intellectual property rights in the accompanying software and in all
// derivatives here to.
//
// This software and any accompanying information is for suggestion only.  It
// does not modify Microchip's standard warranty for its products.  You agree
// that you are solely responsible for testing the software and determining its
// suitability.  Microchip has no obligation to modify, test, certify, or
// support the software.
//
// THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
// EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
// PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
// COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
//
// IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
// (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
// INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
// EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
// ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
// MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
// TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
// CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
// FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
//
// MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
// TERMS.
////////////////////////////////////////////////////////////////////////////////

#define ENABLED  1
#define DISABLED 0

#define FCY         70000000UL

#define LCD_REFRESHCOUNT 45000      // Refresh LCD rate = value * ~17us = 750ms
#define DEBOUNCE_DELAY 	    32	    // Push button debounce delay (ms)

#define IO_DRV         _LATB12

//PWM/ADC Configuration
#define CONVSWITCHFREQ 350000       // PWMx Switching frequency in Hz

// PWM Resolution (~1.04 ns max.)
#define PWMRESOLUTION_REG 1         // 1 ~= 1ns, 2 ~= 2ns, 3~=4ns
                                    // 4 ~= 8ns, Used in PTCON2 register

#define PWMDIVIDERSETTING 1         // 1, 2, 4, 8, 16 (aligns w/ above #define)
#define F_ACLK  (unsigned long)(7372800 * 128)
#define T_ACLK	((float)(1/(float)(F_ACLK/PWMDIVIDERSETTING)))
// PWM Period Calculation
#define PERIODVALUE  ((unsigned int)(((float)((1/(float)(CONVSWITCHFREQ))/T_ACLK)-1)))

#define ADCRESOLUTION  1241      // 4096/3.3V
#define DACRESOLUTION  1241

//Feedback Gains
#define VINFBGAIN     0.181000   // 2.21k/(2.21k+10k)
#define VBUCKFBGAIN   0.755981   // 3.16k/(1.02k+3.16k)
#define VBOOSTFBGAIN  0.163399   // 1k/(1k + 5.12k)

#define IBUCKFBGAIN       0.96   // 1/125 * 120R
#define IBOOSTFBGAIN      1.95   // two 3.9R in parallel

//Input Voltage #defines
#define INPUTUNDERVOLTAGE 7      // Under Voltage threshold in volts
#define INPUTUNDERVOLTAGEADC (int)(INPUTUNDERVOLTAGE*VINFBGAIN*ADCRESOLUTION)

#define INPUTOVERVOLTAGE 11      // Over Voltage threshold in volts
#define INPUTOVERVOLTAGEADC  (int)(INPUTOVERVOLTAGE*VINFBGAIN*ADCRESOLUTION)

#define INPUTVOLTAGEHYST 0.5     // Hysteresis in volts
#define INPUTVOLTAGEHYSTADC  (int)(INPUTVOLTAGEHYST*VINFBGAIN*ADCRESOLUTION)

#define INPUTVOLTMINHYST (INPUTUNDERVOLTAGEADC + INPUTVOLTAGEHYSTADC)
#define INPUTVOLTMAXHYST (INPUTOVERVOLTAGEADC  - INPUTVOLTAGEHYSTADC)


// Calculate temperature of MCP9700
// Vout = TCoefficient * TempAmbient + V0C, V_0C = 500mV , TCoeff = 10m
#define V_0C (int)(0.5*ADCRESOLUTION)
#define TEMPCOEFFICIENT (int)(32767/(.01*ADCRESOLUTION))

#define WARNINGTEMP     60              // In degrees Celsius
#define WARNINGTEMPADC  (int)  (((.01*WARNINGTEMP)+ 0.5)*(ADCRESOLUTION))

#define TEMPMAX         80              // In degrees Celsius
#define TEMPMAXADC	(int)  (((.01*TEMPMAX)+ 0.5)*(ADCRESOLUTION))

#define TEMPHYST        10              // In degrees Celsius

#define TEMPREINIT     (WARNINGTEMP - TEMPHYST)   // 50 C temperature of re-initialization
#define TEMPREINITADC  (int)  (((.01*TEMPREINIT)+ 0.5)*(ADCRESOLUTION))


// ----------------- BUCK/BOOST CONFIGURATIONS --------------- //

// Enable/Disable Converters
#define BUCK  ENABLED
#define BOOST ENABLED

// Output Voltage Setpoints
#define BUCKVOLTAGEREFERENCE    3.3   // Buck output voltage in volts (max 3.8V with on-board load)
#define BOOSTVOLTAGEREFERENCE  15.0   // Boost output voltage in volts (max 17.3V with on-board load)

// Buck/Boost current limits
#define BUCKMAXCURRENT         2.8    // In Amps - taking into account the slope compensation ramp
#define BUCKMAXCURRENTDAC (unsigned int) (((BUCKMAXCURRENT*IBUCKFBGAIN)-0.3) * (DACRESOLUTION))

#define BOOSTMAXCURRENT        1.2    // In Amps - account for slope compensation and 7V input voltage
#define BOOSTMAXCURRENTDAC (unsigned int)(BOOSTMAXCURRENT*DACRESOLUTION*IBOOSTFBGAIN)

#define BUCKVOLTAGEREFERENCEADC (unsigned int)(VBUCKFBGAIN*ADCRESOLUTION*BUCKVOLTAGEREFERENCE)
#define BOOSTVOLTAGEREFERENCEADC (unsigned int)(VBOOSTFBGAIN*ADCRESOLUTION*BOOSTVOLTAGEREFERENCE*0.98)

// New constants for isr.c file
#define BUCK_SS_ERROR  (BUCKVOLTAGEREFERENCEADC>>2)
#define BOOST_SS_ERROR (BOOSTVOLTAGEREFERENCEADC/3.725)

#define BUCKINCREMENT   8
#define BOOSTINCREMENT  2

#define SOFTSTARTRATE  15     // Time in uS

#define TMR2PERIOD      ((unsigned int)SOFTSTARTRATE*70)  // us/14n = 70 (based on instruction clock)
#define TMR2SCALER      0


#define MACRO_CLR_BUCKHISTORY()         \
        buckControlHistory3P3Z[0] = 0;  \
        buckControlHistory3P3Z[1] = 0;  \
        buckControlHistory3P3Z[2] = 0;  \
        buckErrorHistory3P3Z[0]   = 0;  \
        buckErrorHistory3P3Z[1]   = 0;  \
        buckErrorHistory3P3Z[2]   = 0;  \
        buckErrorHistory3P3Z[3]   = 0;

#define MACRO_CLR_BOOSTHISTORY()     \
        boostControlHistory[0] = 0;  \
        boostControlHistory[1] = 0;  \
        boostControlHistory[2] = 0;  \
        boostErrorHistory[0]   = 0;  \
        boostErrorHistory[1]   = 0;  \
        boostErrorHistory[2]   = 0;  \
        boostErrorHistory[3]   = 0;





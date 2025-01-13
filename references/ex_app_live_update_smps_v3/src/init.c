////////////////////////////////////////////////////////////////////////////////
// © 2015 Microchip Technology Inc.
//
// MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
// derivatives created by any person or entity by or on your behalf, exclusively
// with Microchips products.  Microchip and its licensors retain all ownership
// and intellectual property rights in the accompanying software and in all
// derivatives here to.
//
// This software and any accompanying information is for suggestion only.  It
// does not modify Microchips standard warranty for its products.  You agree
// that you are solely responsible for testing the software and determining its
// suitability.  Microchip has no obligation to modify, test, certify, or
// support the software.
//
// THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
// EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
// PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIPS PRODUCTS,
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
#include <p33Fxxxx.h>
#include <xc.h>
#include "init.h"
#include "../i2c.h"
#include <libpic30.h>


#if(BUCK == ENABLED)
int16_t buckACoefficients3P3Z[3]    __attribute__ ((space(xmemory), preserved));  
int16_t buckBCoefficients3P3Z[4]    __attribute__ ((space(xmemory), preserved));  

int16_t buckErrorHistory3P3Z[4]     __attribute__ ((space(ymemory), far, preserved));   
int16_t buckControlHistory3P3Z[3]   __attribute__ ((space(ymemory), far, preserved)); 
#endif

#if(BOOST == ENABLED)
int16_t boostACoefficients[3]       __attribute__ ((space(xmemory), preserved));
int16_t boostBCoefficients[4]       __attribute__ ((space(xmemory), preserved));

int16_t boostErrorHistory[4]        __attribute__ ((space(ymemory), far, preserved));
int16_t boostControlHistory[3]      __attribute__ ((space(ymemory), far, preserved));
#endif

// I2C communications variables for communicating with the _MI2C1Interrupt() ISR
void InitI2C1(void);    // Prototype for function in i2c.c


void InitClock(void)
{
    // Switch to FRC clock (no PLL), in case if the PLL is currently in use.
    // We are not allowed to change the PLL prescalar, postscalar or feedback
    // divider while it is running.
    __builtin_write_OSCCONH(0x00);
    __builtin_write_OSCCONL(0x01);

    // Wait for clock switch to complete
    while(_COSC != _NOSC);

    // Configure PLL for Fosc = 140MHz/Fcy = 70MIPS using 7.37 MHz internal FRC oscillator
    CLKDIV = 0xB000; // ROI = 1, DOZE = 8:1, FRCDIV = 1:1, PLLPOST = 2:1, PLLPRE = 2:1
    PLLFBD = (FCY * 2u * 2u * 2u + 7370000u / 2u) / 7370000u - 2u; // 74 @ 70 MIPS (7.37 MHz input clock from FRC)
    __builtin_write_OSCCONH(0x01);      // Initiate Clock Switch to use the FRC Oscillator + PLL (NOSC = 0b001)
    __builtin_write_OSCCONL(0x01);

   while(OSCCONbits.COSC != 0b001);     // Wait for Osc. to switch to FRC w/ PLL
   while(OSCCONbits.LOCK != 1);         // Wait for Pll to Lock

   // Setup the ADC and PWM clock for 120MHz
   // ((FRC * 16) / APSTSCLR ) = (7.37MHz * 16) / 1 = 117.9MHz

   ACLKCONbits.FRCSEL   = 1;            // FRC provides input for Auxiliary PLL (x16)
   ACLKCONbits.SELACLK  = 1;            // Aux Osc. provides clock source for PWM & ADC
   ACLKCONbits.APSTSCLR = 7;            // Divide Auxiliary clock by 1
   ACLKCONbits.ENAPLL   = 1;            // Enable Auxiliary PLL

    while(!ACLKCONbits.APLLCK);         // Wait for Auxiliary PLL to Lock
   __delay_us(50);                      // Errata#1 (reference:DS80000656A)
}


void InitI2C(void)
{
    // Enable Master I2C interrupt @ IPL 5
    _MI2C1IP = 5;
    // _ANSB6 = 0;                         // Disable Analog/enable Digital I/O mode for I2C pins
    // _ANSB7 = 0;
    InitI2C1();                         // Initialize the TX FIFO and first resistor load state
}


void InitTimer(void)
{
    // Configure Timer 3 for Dynamic Load switching after softstart
    #if((BUCKDYNAMICLOAD == ENABLED) || (BOOSTDYNAMICLOAD == ENABLED))
    PR3 = TMRPERIOD;
    T3CONbits.TCKPS = TMRSCALER;
    T3CONbits.TCS = 0;                  // Clock source is internal Fcy

    _T3IF = 0;
    _T3IP = 4;
    _T3IE = 1;
    T3CONbits.TON = 1;
    #endif

    // Configure Timer 2 for soft start
    PR2 = TMR2PERIOD;
    T2CONbits.TCKPS = TMR2SCALER;
    T2CONbits.TCS = 0;                  // Clock source is internal Fcy

    _T2IF = 0;
    _T2IP = 4;
    _T2IE = 1;
}

void InitBuckPWM(void)
{
    IOCON1bits.PENH = 0;                // GPIO controls I/O port
    IOCON1bits.PENL = 0;

    IOCON1bits.PMOD = 0;                // Complementary Mode

    IOCON1bits.POLH = 0;                // Drive signals are active-high
    IOCON1bits.POLL = 0;                // Drive signals are active-high

    IOCON1bits.OVRENH = 0;              // Override disabled
    IOCON1bits.OVRENL = 0;
    IOCON1bits.OVRDAT = 0b00;           // Override data PWMH and PWML
    IOCON1bits.FLTDAT = 0b01;           // If fault occurs:
                                        // PWMH = 0 & PWML = 1

    PWMCON1bits.DTC   = 0;              // Positive Deadtime enabled
    DTR1    = 80;
    ALTDTR1 = 110;

    PWMCON1bits.IUE = 0;                // Disable Immediate duty cycle updates
    PWMCON1bits.ITB = 0;                // Select Primary Timebase mode

    FCLCON1bits.FLTSRC = 0b01101;       // Fault Control Signal assigned to CMP1
    FCLCON1bits.FLTPOL = 0;             // Fault Signal is active-high
    FCLCON1bits.FLTMOD = 1;             // Cycle-by-Cycle current limiting

    // In order to block the sensed current spike at the
    // turn on edge of the HS FET, use Leading Edge blanking.
    LEBCON1bits.PHR      = 1;           // Enable LEB bit for HS MOSFET rising edge
    LEBCON1bits.FLTLEBEN = 1;           // Fault Input LEB Enabled
    LEBDLY1bits.LEB      = 20;          // 8.32n Steps x 20 = 160ns

    TRGCON1bits.TRGDIV  = 1;             // Trigger interrupt generated every 2 PWM cycles
    TRGCON1bits.TRGSTRT = 0;            // Trigger generated after waiting 0 PWM cycles

    PDC1 = 0;                           // Initial pulse-width
    TRIG1 = 250;                        // Set Initial Trigger location

}


void InitBoostPWM(void)
{
    IOCON2bits.PENH = 0;                // PWM2H is controlled by I/O module
    IOCON2bits.PENL = 0;                // PWM2L is controlled by I/O module

    IOCON2bits.PMOD = 1;                // Redundant Mode w/ PWMH not used
    IOCON2bits.POLL = 0;                // Drive signal- active-high

    IOCON2bits.OVRENL = 0;		// Override disabled
    IOCON2bits.OVRDAT = 0b00;           // Override data:
                                        // PWMH = 0 & PWML = 0
    PWMCON2bits.DTC = 2;                // Deadtime disabled

    PWMCON2bits.IUE = 0;                // Disable Immediate duty cycle updates
    PWMCON2bits.ITB = 0;                // Select Primary Timebase mode

    FCLCON2bits.FLTSRC = 0b01110;       // Fault Control Signal assigned to CMP2
    FCLCON2bits.FLTPOL = 0;             // Fault Signal is active-high
    FCLCON2bits.FLTMOD = 1;             // Cycle-by-Cycle Fault Mode

    //Enable LEB to blank (mask) any turn-on spike
    LEBCON2bits.PLR      =  1;          // Enable LEB bit for PWML
    LEBCON2bits.FLTLEBEN =  1;          // Fault Input LEB Enabled
    LEBDLY2bits.LEB      = 20;          // 8.32n Steps x 20 = 160ns


    TRGCON2bits.TRGDIV   = 1;           // Trigger interrupt generated every 2 PWM cycles
    TRGCON2bits.TRGSTRT = 2;            // Trigger generated after waiting 1 PWM cycles

    PHASE2 = PTPER>>1;                  // Introduce phase shift from Buck converter to reduce Vin distortion
                                        // 180Deg phase-shift from Buck converter
    PDC2  = 0;                          // Initial Duty Cycle
    TRIG2 = 250;                        // Trigger generated at beginning of PWM period
}


void InitPWM3(void)
{
    // PWM3 only used as clock source for triggering AN4/AN7
    #if(BOOST == ENABLED)
    PHASE3 = PTPER >> 1;
    TRGCON3bits.TRGSTRT = 2;   // Trigger generated after waiting 3 PWM cycles

    #else
    TRGCON3bits.TRGSTRT = 0;   // Trigger generated after waiting 0 PWM cycles

    #endif
    TRIG3 = 500;
    TRGCON3bits.TRGDIV = 5;    // Trigger interrupt generated once every 6 PWM cycle
    FCLCON3bits.FLTMOD = 3;
    IOCON3bits.PMOD    = 1;

    IOCON3bits.PENL    = 0;    // Give Ownership of PWM3 to device pin
    IOCON3bits.PENH    = 0;
}


void InitCMP(void)   // Buck & Boost overcurrent protection
{
    #if(BUCK == ENABLED)
    CMP1CONbits.INSEL  = 0;          // Comparator 1A
    CMP1CONbits.HYSSEL = 1;          // 15mV of Hysteresis
    CMP1CONbits.RANGE = 1;           // AVdd is the max DACx output voltage
    CMP1DAC = BUCKMAXCURRENTDAC;     // DAC voltage for overcurrent

    CMP1CONbits.CMPON = 1;
    #endif

    #if(BOOST == ENABLED)
    CMP2CONbits.INSEL  = 0;          // Comparator 2A input
    CMP2CONbits.HYSSEL = 1;          // 15mV of Hysteresis
    CMP2CONbits.RANGE = 1;           // AVdd is the max DACx output voltage
    CMP2DAC = BOOSTMAXCURRENTDAC;

    CMP2CONbits.CMPON = 1;

    #endif
}

void InitADC(void)
{
    // Setup ADC Clock Input Max speed of 70 MHz --> Fosc = 140 MHz
    ADCON3Hbits.CLKSEL  = 1;    // 0-Fsys, 1-Fosc, 2-FRC, 3-APLL
    ADCON3Hbits.CLKDIV  = 0;    // Global Clock divider (1:1)
    ADCORE0Hbits.ADCS   = 0;    // Core 0 clock divider (1:2)
    ADCORE1Hbits.ADCS   = 0;    // Core 1 clock divider (1:2)
    ADCORE2Hbits.ADCS   = 0;    // Core 2 clock divider (1:2)
    ADCORE3Hbits.ADCS   = 0;    // Core 3 clock divider (1:2)
    ADCON2Lbits.SHRADCS = 0;    // 1/2 clock divider

    ADCON1Hbits.FORM    = 0;    // Integer format
    ADCON3Lbits.REFSEL  = 0;    // AVdd as voltage reference

     // ADC Cores in 12-bit resolution mode
    ADCON1Hbits.SHRRES  = 3;    // Shared ADC Core in 12-bit resolution mode
    ADCORE0Hbits.RES    = 3;    // Core 0 ADC Core in 12-bit resolution mode
    ADCORE1Hbits.RES    = 3;    // Core 1 ADC Core in 12-bit resolution mode
    ADCORE2Hbits.RES    = 3;    // Core 2 ADC Core in 12-bit resolution mode
    ADCORE3Hbits.RES    = 3;    // Core 3 ADC Core in 12-bit resolution mode
    ADCON2Hbits.SHRSAMC = 2;    // Shared ADC Core sample time 4Tad

    // Configure ANx for unsigned format and single ended (0,0)
    ADMOD0L = 0x0000;

    InitCalibrateADC();


#if(BUCK == ENABLED)
    ADTRIG0Lbits.TRGSRC0 = 5;    // ADC AN0 triggered by PWM1
    ADTRIG0Lbits.TRGSRC1 = 5;    // ADC AN1 triggered by PWM1

    //ADCORE1Hbits.EISEL   = 7;    // Early Interrupt (8 Tads earlier)
    //ADEIELbits.EIEN1     = 1;

    _ADCAN1IF            = 0;    // Clear Buck ADC interrupt flag
    _ADCAN1IP            = 7;	 // Set Buck ADC interrupt priority - NOTE: This MUST match a dedicated W-array register shadow set (set in Config words) because the ISR does not save any registers. This project assigns a set to IPL7. No other ISRs can use this IPL.
#endif

#if (BOOST == ENABLED)
    ADTRIG0Hbits.TRGSRC2 = 6;    // ADC AN2/AN3 triggered by PWM2
    ADTRIG0Hbits.TRGSRC3 = 1;    // Initial trigger by SW for softstart


    _ADCAN3IF            = 0;    // Clear Boost ADC interrupt flag
    _ADCAN3IP            = 6;    // Set Boost ADC interrupt priority - NOTE: This - NOTE: This MUST match a dedicated W-array register shadow set (set in Config words) because the ISR does not save any registers. This project assigns a set to IPL6. No other ISRs can use this IPL.
#endif

    // Vin FB & Temp Sense analog inputs
    ADTRIG1Lbits.TRGSRC4 = 7;    // ADC AN4/AN7 triggered by PWM3
    ADTRIG1Hbits.TRGSRC7 = 7;    // for synchronizing trigger event

    // Input voltage and temperature sampling for LCD display
    _ADCAN7IF            = 0;    // Clear ADC interrupt flag
    _ADCAN7IP            = 5;	 // Set ADC interrupt priority
    _ADCAN7IE            = 1;	 // Enable the ADC AN7 interrupt
    ADIELbits.IE7        = 1;    // Enable ADC Common Interrupt
}


void InitCalibrateADC(void)
{
    // Power Up delay: 2048 Core Clock Source Periods (TCORESRC) for all ADC cores
    // (~14.6 us)
    _WARMTIME = 11;

    // Turn on ADC module
    ADCON1Lbits.ADON  = 1;

    // Turn on analog power for dedicated core 0
    ADCON5Lbits.C0PWR = 1;
    while(ADCON5Lbits.C0RDY == 0);
    ADCON3Hbits.C0EN  = 1;   // Enable ADC core 0

    // Turn on analog power for dedicated core 1
    ADCON5Lbits.C1PWR = 1;
    while(ADCON5Lbits.C1RDY == 0);
    ADCON3Hbits.C1EN  = 1;   // Enable ADC core 1

    // Turn on analog power for dedicated core 2
    ADCON5Lbits.C2PWR = 1;
    while(ADCON5Lbits.C2RDY == 0);
    ADCON3Hbits.C2EN  = 1;   // Enable ADC core 2

    // Turn on analog power for dedicated core 3
    ADCON5Lbits.C3PWR = 1;
    while(ADCON5Lbits.C3RDY == 0);
    ADCON3Hbits.C3EN  = 1;   // Enable ADC core 3

    // Turn on analog power for shared core
    ADCON5Lbits.SHRPWR = 1;
    while(ADCON5Lbits.SHRRDY == 0);
    ADCON3Hbits.SHREN  = 1;  // Enable shared ADC core

    // Enable calibration for the dedicated core 0
    ADCAL0Lbits.CAL0EN   = 1;
    ADCAL0Lbits.CAL0DIFF = 0;         // Single-ended input calibration
    ADCAL0Lbits.CAL0RUN  = 1;         // Start Cal
    while(ADCAL0Lbits.CAL0RDY == 0);
    ADCAL0Lbits.CAL0EN   = 0;         // Cal complete

    // Enable calibration for the dedicated core 1
    ADCAL0Lbits.CAL1EN   = 1;
    ADCAL0Lbits.CAL1DIFF = 0;         // Single-ended input calibration
    ADCAL0Lbits.CAL1RUN  = 1;         // Start Cal
    while(ADCAL0Lbits.CAL1RDY == 0);
    ADCAL0Lbits.CAL1EN   = 0;         // Cal complete

    // Enable calibration for the dedicated core 2
    ADCAL0Hbits.CAL2EN   = 1;
    ADCAL0Hbits.CAL2DIFF = 0;         // Single-ended input calibration
    ADCAL0Hbits.CAL2RUN  = 1;         // Start Cal
    while(ADCAL0Hbits.CAL2RDY == 0);
    ADCAL0Hbits.CAL2EN   = 0;         // Cal complete

    // Enable calibration for the dedicated core 3
    ADCAL0Hbits.CAL3EN   = 1;
    ADCAL0Hbits.CAL3DIFF = 0;         // Single-ended input calibration
    ADCAL0Hbits.CAL3RUN  = 1;         // Start Cal
    while(ADCAL0Hbits.CAL3RDY == 0);
    ADCAL0Hbits.CAL3EN   = 0;         // Cal complete


    // Enable calibration for the shared core
    ADCAL1Hbits.CSHREN   = 1;
    ADCAL1Hbits.CSHRDIFF = 0;        // Single-ended input calibration
    ADCAL1Hbits.CSHRRUN  = 1;        // Start calibration cycle
    while(ADCAL1Hbits.CSHRRDY == 0); // while calibration is still in progress

    ADCAL1Hbits.CSHREN   = 0;        // Calibration is complete
}


void InitADCMP(void)
{
    // Digital Comparator 0 for temperature
    ADCMP0CONbits.HIHI   = 1;   // Interrupt when ADCBUF is greater than ADCMP0HI
    ADCMP0ENLbits.CMPEN7 = 1;   // ADCBUF7 processed by digital comparator

    ADCMP0HI = WARNINGTEMPADC;  // Monitor for temperature exceeding the high temperature warning threshold

    ADCMP0CONbits.CMPEN = 1;    // Enable digital comparator

    ADCMP0CONbits.IE    = 1;    // Enable interrupt for DCMP

    _ADCMP0IP = 4;              // Set ADC Digital Comparator 0 Interrupt Priority Level to 4
    _ADCMP0IF = 0;              // Clear ADC Digital Comparator 0 Flag
    _ADCMP0IE = 1;              // Enable ADC Digital Comparator 0 Interrupt

    // Digital Comparator 1 for input voltage
    ADCMP1CONbits.HIHI   = 1;   // Interrupt when ADCBUF is greater than DCMP HI
    ADCMP1CONbits.LOLO   = 1;   // Interrupt when ADCBUF is less than DCMP LO
    ADCMP1ENLbits.CMPEN4 = 1;   // ADCBUF4 processed by digital comparator

    ADCMP1HI = INPUTOVERVOLTAGEADC;
    ADCMP1LO = INPUTUNDERVOLTAGEADC;

    ADCMP1CONbits.CMPEN = 1;    // Enable digital comparator

    ADCMP1CONbits.IE    = 1;    // Enable interrupt for DCMP

    _ADCMP1IP = 5;              // Interrupt priority - NOTE: Cannot be 6 or 7 because these have dedicated W-reg shadow sets assigned for the ADC sampling ISR
    _ADCMP1IF = 0;              // Clear ADC Digital Comparator 1 Flag
    _ADCMP1IE = 1;              // Enable ADC Digital Comparator 1 Interrupt

}


#if(BUCK == ENABLED)
void __attribute__((optimize(1))) InitBuckComp(void)
{
    MACRO_CLR_BUCKHISTORY();

    buckACoefficients3P3Z[0] = BUCK_COMP_3P3Z_COEFF_A1;
    buckACoefficients3P3Z[1] = BUCK_COMP_3P3Z_COEFF_A2;
    buckACoefficients3P3Z[2] = BUCK_COMP_3P3Z_COEFF_A3;

    buckBCoefficients3P3Z[0] = BUCK_COMP_3P3Z_COEFF_B0;
    buckBCoefficients3P3Z[1] = BUCK_COMP_3P3Z_COEFF_B1;
    buckBCoefficients3P3Z[2] = BUCK_COMP_3P3Z_COEFF_B2;
    buckBCoefficients3P3Z[3] = BUCK_COMP_3P3Z_COEFF_B3;

    // Swap to Alternate W-Reg #1
    InitAltRegContext1Setup();        // Setup working registers to be used with compensator
}
#endif

#if(BOOST == ENABLED)
void InitBoostComp(void)
{
    MACRO_CLR_BOOSTHISTORY();

    boostACoefficients[0] = BOOST_COMP_3P3Z_COEFF_A1;
    boostACoefficients[1] = BOOST_COMP_3P3Z_COEFF_A2;
    boostACoefficients[2] = BOOST_COMP_3P3Z_COEFF_A3;

    boostBCoefficients[0] = BOOST_COMP_3P3Z_COEFF_B0;
    boostBCoefficients[1] = BOOST_COMP_3P3Z_COEFF_B1;
    boostBCoefficients[2] = BOOST_COMP_3P3Z_COEFF_B2;
    boostBCoefficients[3] = BOOST_COMP_3P3Z_COEFF_B3;

    // Swap to Alternate W-Reg #2
    InitAltRegContext2Setup();        // Setup working registers to be used with compensator
}
#endif

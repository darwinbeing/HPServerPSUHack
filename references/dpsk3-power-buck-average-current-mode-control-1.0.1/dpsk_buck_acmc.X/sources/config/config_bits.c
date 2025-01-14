/*
 * File:   config_bits.c
 * Author: M91406
 *
 * Created on July 8, 2019, 2:39 PM
 */


#include <xc.h>
#include "hal.h"

// Configuration bits: selected in the GUI

/*******************************************************************************************************/
// FICD

#pragma config ICS = PGD1    //ICD Communication Channel Select bits->Communicate on PGC2 and PGD2
#pragma config JTAGEN = OFF    //JTAG Enable bit->JTAG is disabled
#pragma config NOBTSWP = DISABLED    //BOOTSWP instruction disable bit->BOOTSWP instruction is disabled

/*******************************************************************************************************/
// FALTREG
// 
// Alternate Working Register Interrupt Priority Level COnfiguration
// OFF -> Not Assigned
// IPL1 ... IPL6 -> Interrupt Priority Level 1 ... 6

#if (_OSTIMER_PRIORITY == 0)
    #pragma config CTXT1 = OFF    //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
#elif (_OSTIMER_PRIORITY == 1)
    #pragma config CTXT1 = IPL1   //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
#elif (_OSTIMER_PRIORITY == 2)
    #pragma config CTXT1 = IPL2   //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
#elif (_OSTIMER_PRIORITY == 3)
    #pragma config CTXT1 = IPL3   //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
#elif (_OSTIMER_PRIORITY == 4)
    #pragma config CTXT1 = IPL4   //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
#elif (_OSTIMER_PRIORITY == 5)
    #pragma config CTXT1 = IPL5   //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
#elif (_OSTIMER_PRIORITY == 6)
    #pragma config CTXT1 = IPL6   //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
#else
    #pragma config CTXT1 = OFF    //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
    #pragma message "WARNING: operating system timer priority invalid."
#endif

#if (BUCK_VOUT_ISR_PRIORITY == 0)
    #pragma config CTXT2 = OFF    //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
#elif (BUCK_VOUT_ISR_PRIORITY == 1)
    #pragma config CTXT2 = IPL1   //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
#elif (BUCK_VOUT_ISR_PRIORITY == 2)
    #pragma config CTXT1 = IPL2   //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
#elif (BUCK_VOUT_ISR_PRIORITY == 3)
    #pragma config CTXT2 = IPL3   //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
#elif (BUCK_VOUT_ISR_PRIORITY == 4)
    #pragma config CTXT2 = IPL4   //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
#elif (BUCK_VOUT_ISR_PRIORITY == 5)
    #pragma config CTXT2 = IPL5   //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
#elif (BUCK_VOUT_ISR_PRIORITY == 6)
    #pragma config CTXT2 = IPL6   //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
#else
    #pragma config CTXT2 = OFF    //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits
    #pragma message "WARNING: buck converter output voltage loop control interrupt priority invalid."
#endif

#pragma config CTXT3 = OFF    //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 3 bits->Not Assigned
#pragma config CTXT4 = OFF    //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 4 bits->Not Assigned

/*******************************************************************************************************/
// FSEC
#pragma config BWRP = OFF    //Boot Segment Write-Protect bit->Boot Segment may be written
#pragma config BSS = DISABLED    //Boot Segment Code-Protect Level bits->No Protection (other than BWRP)
#pragma config BSEN = OFF    //Boot Segment Control bit->No Boot Segment
#pragma config GWRP = OFF    //General Segment Write-Protect bit->General Segment may be written
#pragma config GSS = DISABLED    //General Segment Code-Protect Level bits->No Protection (other than GWRP)
#pragma config CWRP = OFF    //Configuration Segment Write-Protect bit->Configuration Segment may be written
#pragma config CSS = DISABLED    //Configuration Segment Code-Protect Level bits->No Protection (other than CWRP)
#pragma config AIVTDIS = OFF    //Alternate Interrupt Vector Table bit->Disabled AIVT

// FBSLIM
#pragma config BSLIM = 8191    //Boot Segment Flash Page Address Limit bits->8191

// FOSCSEL
#pragma config FNOSC = FRC    //Oscillator Source Selection->Fast RC Oscillator with divide-by-N with PLL module (FRCPLL) 
#pragma config IESO = OFF    //Two-speed Oscillator Start-up Enable bit->Start up with user-selected oscillator source

// FOSC
#pragma config POSCMD = NONE    //Primary Oscillator Mode Select bits->Primary Oscillator disabled
#pragma config OSCIOFNC = ON    //OSC2 Pin Function bit->OSC2 is general purpose digital I/O pin
#pragma config FCKSM = CSECMD    //Clock Switching Mode bits->Clock switching is enabled,Fail-safe Clock Monitor is disabled
#pragma config PLLKEN = ON    //PLL Lock Status Control->PLL lock signal will be used to disable PLL clock output if lock is lost
#pragma config XTCFG = G3    //XT Config->24-32 MHz crystals
#pragma config XTBST = ENABLE    //XT Boost->Boost the kick-start

// FWDT
//#pragma config RWDTPS = PS2147483648    //Run Mode Watchdog Timer Post Scaler select bits->1:2147483648
#pragma config RCLKSEL = LPRC    //Watchdog Timer Clock Select bits->Always use LPRC
#pragma config WINDIS = OFF    //Watchdog Timer Window Enable bit->Watchdog Timer in Window mode
#pragma config WDTWIN = WIN25    //Watchdog Timer Window Select bits->WDT Window is 25% of WDT period
//#pragma config SWDTPS = PS2147483648    //Sleep Mode Watchdog Timer Post Scaler select bits->1:2147483648
#pragma config FWDTEN = ON_SW    //Watchdog Timer Enable bit->WDT controlled via SW, use WDTCON.ON bit

// FPOR
#pragma config BISTDIS = DISABLED    //Memory BIST Feature Disable->mBIST on reset feature disabled

// FDMTIVTL
#pragma config DMTIVTL = 0    //Dead Man Timer Interval low word->0

// FDMTIVTH
#pragma config DMTIVTH = 0    //Dead Man Timer Interval high word->0

// FDMTCNTL
#pragma config DMTCNTL = 0    //Lower 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF)->0

// FDMTCNTH
#pragma config DMTCNTH = 0    //Upper 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF)->0

// FDMT
#pragma config DMTDIS = OFF    //Dead Man Timer Disable bit->Dead Man Timer is Disabled and can be enabled by software

// FDEVOPT
#pragma config ALTI2C1 = OFF    //Alternate I2C1 Pin bit->I2C1 mapped to SDA1/SCL1 pins
#pragma config ALTI2C2 = OFF    //Alternate I2C2 Pin bit->I2C2 mapped to SDA2/SCL2 pins
#pragma config ALTI2C3 = OFF    //Alternate I2C3 Pin bit->I2C3 mapped to SDA3/SCL3 pins
#pragma config SMBEN = SMBUS    //SM Bus Enable->SMBus input threshold is enabled
#pragma config SPI2PIN = PPS    //SPI2 Pin Select bit->SPI2 uses I/O remap (PPS) pins

// FBTSEQ
#pragma config BSEQ = 4095    //Relative value defining which partition will be active after device Reset; the partition containing a lower boot number will be active->4095
#pragma config IBSEQ = 4095    //The one's complement of BSEQ; must be calculated by the user and written during device programming.->4095

// FBOOT
#pragma config BTMODE = SINGLE    //Device Boot Mode Configuration->Device is in Single Boot (legacy) mode

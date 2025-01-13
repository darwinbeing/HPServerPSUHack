/*
 * File:   pic24fj1024gb610_explorer_16.c
 *
 * Initializes the device configuration words, clock frequency, UART2 pins,
 * LED I/O, Button I/O, and 25LC256 SPI2 pins for the PIC24FJ1024GB610 PIM on
 * the Explorer 16 or Explorer 16/32 development board. For I2C projects, I2C 1
 * is configured for use with SCL1/SDA1.
 *
 * PIC24FJ1024GB610 product page:
 *   http://www.microchip.com/pic24fj1024gb610
 *
 * PIC24FJ1024GB610 PIM Info Sheet and schematic:
 *   http://ww1.microchip.com/downloads/en/DeviceDoc/50002423a.pdf
 *
 * Explorer 16/32 schematic:
 *   http://ww1.microchip.com/downloads/en/DeviceDoc/Explorer_16_32_Schematics_R6_3.pdf
 *
 * For the Explorer 16 (original version) schematic:
 *   http://ww1.microchip.com/downloads/en/DeviceDoc/DM240001%20BOM%20and%20Schematics.pdf
 */

/*******************************************************************************
  Copyright (C) 2018 Microchip Technology Inc.

  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
  derivatives created by any person or entity by or on your behalf, exclusively
  with Microchip's products.  Microchip and its licensors retain all ownership
  and intellectual property rights in the accompanying software and in all
  derivatives here to.

  This software and any accompanying information is for suggestion only.  It
  does not modify Microchip's standard warranty for its products.  You agree
  that you are solely responsible for testing the software and determining its
  suitability.  Microchip has no obligation to modify, test, certify, or
  support the software.

  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
  TERMS.
*******************************************************************************/

// NOTE: This initialization code is specific to the GB610/GA610 PIM, so
// GB606/GA606 devices are listed here only for convenience.
// Such devices may not compile without I/O or Config word changes.
#if defined(__PIC24FJ1024GB610__) || defined(__PIC24FJ512GB610__) || defined(__PIC24FJ256GB610__) || defined(__PIC24FJ128GB610__) || \
    defined(__PIC24FJ1024GA610__) || defined(__PIC24FJ512GA610__) || defined(__PIC24FJ256GA610__) || defined(__PIC24FJ128GA610__) || \
    defined(__PIC24FJ1024GB606__) || defined(__PIC24FJ512GB606__) || defined(__PIC24FJ256GB606__) || defined(__PIC24FJ128GB606__) || \
    defined(__PIC24FJ1024GA606__) || defined(__PIC24FJ512GA606__) || defined(__PIC24FJ256GA606__) || defined(__PIC24FJ128GA606__)


#define FCY         16000000ul      // Changing this automatically changes the PLL settings to run at this target frequency

#include <xc.h>
#include "../ezbl_integration/ezbl.h"


// Device Configuration Words
// Config words can be defined in a Bootloader project, Application project,
// or mixed between each other so long as any given flash/config write-word-
// size location has exactly one definition. On devices with flash Config
// words and a flash double word minimum programming size (0x4 program
// addresses), this means two adjacent Config words may not be mixed between
// projects and instead both Config words must be defined in the same project.
//
// These defaults place all (or almost all) in the Bootloader project as this
// is the safest from a bootloader-bricking standpoint.
#if defined(EZBL_BOOT_PROJECT)  // Compiling for a Bootloader Project
    EZBL_SET_CONF(_FSEC, BWRP_OFF & BSS_OFF & BSEN_OFF & GWRP_OFF & GSS_OFF & CWRP_OFF & CSS_DIS & AIVTDIS_DISABLE)
    EZBL_SET_CONF(_FOSCSEL, FNOSC_FRC & PLLMODE_PLL96DIV2 & IESO_OFF)
    EZBL_SET_CONF(_FOSC, POSCMD_XT & OSCIOFCN_ON & SOSCSEL_ON & PLLSS_PLL_PRI & IOL1WAY_OFF & FCKSM_CSECME)
    EZBL_SET_CONF(_FWDT, WDTPS_PS1024 & FWPSA_PR32 & SWON & WINDIS_OFF & WDTCMX_LPRC & WDTCLK_LPRC)
    EZBL_SET_CONF(_FPOR, BOREN_ON & LPCFG_ON & DNVPEN_ENABLE)
    EZBL_SET_CONF(_FICD, PGX2 & JTAGEN_OFF & BTSWP_ON)
    EZBL_SET_CONF(_FDEVOPT1, ALTCMPI_DISABLE & TMPRPIN_OFF & SOSCHP_ON & ALTVREF_ALTVREFDIS)
    #if defined(__DUAL_PARTITION)
    EZBL_SET_CONF(_FBOOT, BTMODE_DUAL)    // FBOOT = 0xFFFF = Single Partition mode; 0xFFFE = Dual Partition mode; NOTE: Do not define this as BTMODE_SINGLE, it is a 0xFFFF default, so is unnecessary, it can't be written in RTSP mode and can cause verification problems.
    #endif
#else   // Compiling for an Application Project (EZBL_BOOT_PROJECT is not defined)
    //EZBL_SET_CONF_FBTSEQ(4094)          // FBTSEQ is set at run time - should not be defined explicitly unless you need to reset back to a max value. If doing so, define it in the App project, not the Bootloader.
#endif  // Goes to: #if defined(EZBL_BOOT_PROJECT)



// Set flash page erase size = 0x800 program addresses = 3072 Flash bytes/1024 instruction words/512 Flash double words.
// PIC24FJ1024GB610/GA610 family devices implement 0x800; dsPIC33EP64GS506, dsPIC33EP128GS808, PIC24FJ256GB412/GA412 family devices implement 0x400.
// Single Partition bootloaders don't have to define this as it is automatically obtained by ezbl_tools.jar and inserted in the .gld linker script.
EZBL_SetSYM(EZBL_ADDRESSES_PER_SECTOR, 0x800);



//const unsigned int EZBL_i2cSlaveAddr = 0x60;          // Define I2C Slave Address that this Bootloader will listen/respond to, applicable only if I2C_Reset() is called
EZBL_FIFO *EZBL_COMBootIF __attribute__((persistent));  // Pointer to RX FIFO to check activity on for bootloading
const long EZBL_COMBaud = 115200;                      // Communications baud rate: <= 0 means auto-baud with default value (for TX) set by the 2's complemented value; otherwise baud in bits/sec (ex: 460800)



/**
 * Initializes system level hardware, such as the system clock source (PLL),
 * I/O pins for LED status indication, a timer for the NOW_*() timekeeping and
 * scheduling APIs and one or more communications peripherals for EZBL
 * bootloading.
 *
 * Although not required or used for bootloading, this function also initializes
 * I/O pins for push buttons, LCD interfacing and some extra items commonly on
 * Microchip development boards. When porting to other hardware, the extra
 * initialization code can be deleted.
 *
 * @return unsigned long system execution clock, in instructions/second (FCY).
 *
 *         One 16-bit timer peripheral will be enabled, along with it's ISR at a
 *         a period of 65536 system clocks. The timer/CCP used for this is
 *         selected within this code using the NOW_Reset() macro.
 *
 *         At least one communications is also selected and initialized, along
 *         with 1 or 2 ISRs for it (UART RX + TX ISRs or Slave I2C (no Master
 *         I2C).
 */
// @return: FCY clock speed we just configured the processor for
unsigned long InitializeBoard(void)
{
    // Switch to FRC clock (no PLL), in case if the PLL is currently in use.
    // We should not be changing the PLL prescalar, postscalar or feedback
    // divider (if present) while the PLL is clocking anything.
    __builtin_write_OSCCONH(0x00);
    __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);
    while(OSCCONbits.OSWEN);            // Wait for clock switch to complete

    // Configure PLL for Fosc = 32MHz/Fcy = 16MIPS using 8 MHz internal FRC oscillator + SOSC self-tuning
    CLKDIV = 0xB120;                    // ROI = 1, DOZE = 8:1, RCDIV<2:0> = Fast RC Oscillator (FRC) with PLL module (FRCPLL), CPDIV<1:0> = 32MHz (96MHz PLL post divider div by 1 for 32MHz CPU clock); PLLEN = 1 (PLL stays active for USB when CPU not being clocked from PLL)
    OSCDIV = 0x0000;                    // 1:1 divide on FRC, XT or EC oscillator (2:1 is the default)
    OSCTUN = 0x8000;                    // FRC self-tuning enabled, STSRC = 0 (FRC is tuned to approximately match the 32.768 kHz SOSC tolerance)
    
    __builtin_write_OSCCONH(0x01);      // Initiate Clock Switch to use the FRC Oscillator + PLL (NOSC = 0b001)
    __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);

    // Wait for clock switch to complete and PLL to be locked (if enabled)
    while(OSCCONbits.OSWEN);            // Wait for clock switch to complete
    if((OSCCONbits.COSC & 0x5) == 0x1)  // 0x0 = Fast RC Oscillator (FRC); 0x1 = Fast RC Oscillator (FRC) with Divide-by-N and PLL; 0x2 = Primary Oscillator (XT, HS, EC); 0x3 = Primary Oscillator (XT, HS, EC) with PLL; 0x4 = Secondary Oscillator (SOSC); 0x5 = Low-Power RC Oscillator (LPRC); 0x6 = Fast RC Oscillator (FRC) with Divide-by-16; 0x7 = Fast RC Oscillator (FRC) with Divide-by-N
    {
        while(!_LOCK);
    }


    // Initialize/select 16-bit hardware timer for NOW time keeping/scheduling
    // APIs. This call selects the hardware timer resource (can be TMR1-TMR6 or
    // CCP1-CCP8, if available) and the _Tx/_CCTxInterrupt gets automatically
    // implemented by code in ezbl_lib.a.
    NOW_Reset(TMR1, FCY);


    // Set push buttons as GPIO inputs
    //
    // Function     Explorer 16 PIM Header      PIC24FJ1024GB610 Device Pins
    // Button       PIM#, PICtail#, name        PIC#, name
    // S4 (LSb)       80, 106, RD13               80, OCM3F/PMD13/RD13
    // S5             92,  74, RA7                92, AN22/OCM1F/PMA17/RA7              <- Pin function is muxed with LED D10; S5 button can't be used because LED clamps weak 10k pull up voltage too low
    // S6             84, 100, RD7                84, C3INA/U5RTS/U5BCLK/OC5/PMD15/RD7
    // S3 (MSb)       83,  99, RD6                83, C3INB/U5RX/OC4/PMD14/RD6
    EZBL_DefineButtonMap(RD6, RD7, RD13);
    _TRISD13 = 1;
    //_TRISA7  = 1;
    _TRISD7  = 1;
    _TRISD6  = 1;
    //_ANSA7   = 0;
    _ANSD7   = 0;
    _ANSD6   = 0;


    // Set LED pins as GPIO outputs
    //
    // Function     Explorer 16 PIM Header      PIC24FJ1024GB610 Device Pins
    // LED          PIM#, PICtail#, name        PIC#, name
    // D3 (LSb)       17, 69, RA0/TMS             17, TMS/OCM3D/RA0
    // D4             38, 70, RA1/TCK             38, TCK/RA1
    // D5             58, 38, RA2/SCL2            58, PMPCS1/SCL2/RA2
    // D6             59, 40, RA3/SDA2            59, SDA2/PMA20/RA3
    // D7             60, 71, RA4/TDI             60, TDI/PMA21/RA4
    // D8             61, 72, RA5/TDO             61, TDO/RA5
    // D9             91, 73, RA6                 91, AN23/OCM1E/RA6
    // D10 (MSb)      92, 74, RA7                 92, AN22/OCM1F/PMA17/RA7        <- Pin function is muxed with button S5; we will use it as an LED output only
    EZBL_DefineLEDMap(RA7, RA6, RA5, RA4, RA3, RA2, RA1, RA0);
    *((volatile unsigned char *)&LATA)  = 0x00; // Write bits LATA<7:0> simultaneously, don't change LATA<15:8>
    *((volatile unsigned char *)&TRISA) = 0x00;
    *((volatile unsigned char *)&ANSA)  = 0x00;


//// Code commented: not normally needed for a Bootloader, but pin data should be valid
//    // Set 16x2 character LCD pins as GPIO outputs
//    //
//    // Function     Explorer 16 PIM Header      PIC24FJ1024GB610 Device Pins
//    // 16x2 LCD     PIM#, PICtail#, name        PIC#, name
//    // Data 0         93, 109, RE0/PMPD0          93, PMD0/RE0
//    // Data 1         94, 110, RE1/PMPD1          94, PMD1/RE1
//    // Data 2         98, 111, RE2/PMPD2          98, PMD2/RE2
//    // Data 3         99, 112, RE3/PMPD3          99, CTED9/PMD3/RE3
//    // Data 4        100, 113, RE4/PMPD4         100, HLVDIN/CTED8/PMD4/RE4
//    // Data 5          3, 114, RE5/PMPD5           3, IC4/CTED4/PMD5/RE5
//    // Data 6          4, 115, RE6/PMPD6           4, SCL3/IC5/PMD6/RE6
//    // Data 7          5, 116, RE7/PMPD7           5, SDA3/IC6/PMD7/RE7
//    // E (Enable)     81,  97, RD4/PMPWR          81, RP25/PMWR/PMENB/RD4
//    // R/!W           82,  98, RD5/PMPRD          82, RP20/PMRD/PMWR/RD5
//    // RS (Reg Sel)   44,  84, RB15/PMPA0         44, AN15/RP29/CTED6/PMA0/PMALL/RB15
//    *((volatile unsigned char *)&LATE)  = 0x00; // Write bits LATE<7:0> simultaneously, don't change LATE<15:8>
//    *((volatile unsigned char *)&TRISE) = 0x00;
//    *((volatile unsigned char *)&ANSE)  = 0x00;
//    _LATD4   = 0;
//    _LATD5   = 0;
//    _TRISD4  = 0;
//    _TRISD5  = 0;
//    _LATB15  = 0;
//    _TRISB15 = 0;
//    _ANSB15  = 0;


//// Code commented: not normally needed for a Bootloader, but pin data should be valid
//    // Configure SPI2 pins for 25LC256 (32Kbyte SPI EEPROM)
//    // - Pin names are with respect to the PIC, which is the SPI Master.
//    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
//    //   SPI hardware overrides it.
//    //
//    // Function     Explorer 16 PIM Header      PIC24FJ1024GB610 Device Pins
//    // SPI2         PIM#, PICtail#, name        PIC#, name
//    // !CS   (out)    79, 105, RD12               79, RPI42/OCM3E/PMD12/RD12
//    // SCK2  (out)    10,  35, RG6/PMPA5/SCK2     10, AN17/C1IND/RP21/ICM1/OCM1A/PMA5/RG6
//    // SDI2  (in)     11,  37, RG7/PMPA4/SDI2     11, AN18/C1INC/RP26/OCM1B/PMA4/RG7
//    // SDO2  (out)    12,  39, RG8/PMPA3/SDO2     12, AN19/C2IND/RP19/ICM2/OCM2A/PMA3/RG8
//    _LATD12  = 1;               // 1 is inactive
//    _TRISD12 = 0;               // !CS on RD12
//    _IOCPDG7 = 1;               // Turn on pull down on SDI2 so it doesn't float when SPI module tri-states it
//    _TRISG6  = 1;
//    _TRISG7  = 1;
//    _TRISG8  = 1;
//    _ANSG6   = 0;
//    _ANSG7   = 0;
//    _ANSG8   = 0;
//    _SDI2R   = 26;              // SDI2 (MISO2) on RP26
//    _RP21R   = _RPOUT_SCK2OUT;  // SCK2 on RP21
//    _RP19R   = _RPOUT_SDO2;     // SDO2 (MOSI2) on RP19


#if defined(XPRJ_i2c) || defined(XPRJ_default) || defined(EZBL_INIT_I2C)    // XPRJ_* definitions defined by MPLAB X on command line based on Build Configuration. If you need this interface always, you can alternatively define a project level EZBL_INIT_I2C macro.
    // Configure I2C1 pins for MCP2221A
    // Function     Explorer 16 PIM Header      PIC24FJ1024GB610 Device Pins    PIC24FJ1024GA610 Device Pins
    // I2C 1        PIM#, PICtail#, name        PIC#, name                      PIC#, name
    // SDA1 (in/out)  56, 8, RG3/SDA1             66, RPI36/SCL1/PMA22/RA14       56, SDA1/RG3
    // SCL1 (in/out)  57, 6, RG2/SCL1             67, RPI35/SDA1/PMBE1/RA15       57, SCL1/RG2
    IOCPUAbits.IOCPA14 = 1;                          // Enable Weak pull up on SDA1 line so pins float to idle state if not connected or externally pulled up
    IOCPUAbits.IOCPA15 = 1;                          // Enable Weak pull up on SCL1 line so pins float to idle state if not connected or externally pulled up
    //IOCPUGbits.IOCPG3 = 1;  // PIC24FJ1024GA610 pins // Enable Weak pull up on SDA1 line so pins float to idle state if not connected or externally pulled up
    //IOCPUGbits.IOCPG2 = 1;                           // Enable Weak pull up on SCL1 line so pins float to idle state if not connected or externally pulled up
    EZBL_COMBootIF = I2C_Reset(1, FCY, 0, EZBL_i2cSlaveAddr, 0);
#endif


#if defined(XPRJ_uart) || defined(XPRJ_default) || defined(EZBL_INIT_UART)    // XPRJ_* definitions defined by MPLAB X on command line based on Build Configuration. If you need this interface always, you can alternatively define a project level EZBL_INIT_UART macro.    // Configure UART2 pins as UART.
    // - Pin names are with respect to the PIC.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   UART2 hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      PIC24FJ1024GB610 Device Pins
    // UART2        PIM#, PICtail#, name        PIC#, name
    // U2RX  (in)     49, 34, RF4/PMPA9/U2RX      49, RP10/PMA9/RF4
    // U2TX  (out)    50, 36, RF5/PMPA8/U2TX      50, RP17/PMA8/RF5
    IOCPUFbits.IOCPF4 = 1;      // Turn on weak pull up on U2RX so no spurious data arrives if nobody connected
    _U2RXR  = 10;               // U2RX on RP10
    _RP17R  = _RPOUT_U2TX;      // U2TX on RP17
    if(EZBL_COMBaud <= 0)       // If auto-baud enabled, delay our UART initialization so MCP2221A POR timer and init
    {                           // is complete before we start listening. POR timer max spec is 140ms, so MCP2221A TX
        NOW_Wait(140u*NOW_ms);  // pin glitching could occur long after we have enabled our UART without this forced delay.
    }
    EZBL_COMBootIF = UART_Reset(2, FCY, EZBL_COMBaud, 0);
#endif


// Code commented: intended for debugging; requires extra hardware
    // Configure UART 1 for debug messages when using bootloader on UART 2
    // - Requires MCP2221A or MCP2200 Breakout Module to be plugged in vertically
    // Explorer 16/32 - mikroBUS B top left column pins (AN/P24, RST/P19, CS/P23, SCK/P55, MISO/P54)
    // U1RX     AN/P24      PIC Pin 24  PGEC1/ALTCVREF-/ALTVREF-/AN1/RP1/CTED12/RB1
    // VDD      RST/P19     PIC pin 19  AN21/RPI34/PMA19/RE9
    // GND      CS/P23      PIC pin 23  AN2/CTCMP/C2INB/RP13/CTED13/RB2
    // NC
    // NC
    // U1TX     MOSI/P53    PIC pin 53  RP15/RF8
    IOCPUBbits.IOCPB1 = 1;  //       Turn on pull-up on U1RX input in case if no one is attached to drive it (no spurious RX)
    _ANSB1  = 0;
    _U1RXR  = 1;            // mikroBUS B AN/P24,   PIC pin 24, PGEC1/ALTCVREF-/ALTVREF-/AN1/RP1/CTED12/RB1
    _ANSE9  = 0;            // mikroBUS B RST/P19,  PIC pin 19, AN21/RPI34/PMA19/RE9
    _LATE9  = 1;            // Emulate VDD by driving GPIO = '1' for MCP2200/MCP2221 breakout board
    _TRISE9 = 0;
    _ANSB2  = 0;            // mikroBUS B CS/P23,   PIC pin 23, AN2/CTCMP/C2INB/RP13/CTED13/RB2
    _LATB2  = 0;            // Emulate VSS by driving GPIO = '0' for MCP2200/MCP2221 breakout board
    _TRISB2 = 0;
    _RP15R  = _RPOUT_U1TX;  // mikroBUS B MOSI/P53, PIC pin 53, RP15/RF8
    UART_Reset(1, FCY, 115200, 1);  // Initialize UART1 @ 500000 baud for STDIO and printing debug messages
    EZBL_ConsoleReset();
    EZBL_printf("\n\nHello World!"
                 "\n  RCON  = 0x%04X"
                 "\n  U1BRG = 0x%04X"
                 "\n  U2BRG = 0x%04X", RCON, U1BRG, U2BRG);



//// Code commented: not normally needed for a Bootloader, but pin data should be valid
//    // Configure Analog Inputs for U4 TC1047A Temperature Sensor and R6 10K Potentiometer
//    //
//    // Function     Explorer 16 PIM Header      PIC24FJ1024GB610 Device Pins
//    // Analog Input PIM#, PICtail#, name        PIC#, name
//    // TC1047A Temp   21, 14, RB4/AN4             21, PGED3/AN4/C1INB/RP28/USBOEN/OCM3B/RB4
//    // 10K Pot        20, 77, RB5/AN5             20, PGEC3/AN5/C1INA/RP18/ICM3/OCM3A/RB5
//    _TRISB4 = 1;
//    _TRISB5 = 1;
//    _ANSB4  = 1;
//    _ANSB5  = 1;


    // Report 16 MIPS on PIC24F
    return FCY;
}


#endif //#if defined(__PIC24FJ1024GB610__) || defined(__PIC24FJ512GB610__) || defined(__PIC24FJ256GB610__) || defined(__PIC24FJ128GB610__) ||
       //    defined(__PIC24FJ1024GA610__) || defined(__PIC24FJ512GA610__) || defined(__PIC24FJ256GA610__) || defined(__PIC24FJ128GA610__) ||
       //    defined(__PIC24FJ1024GB606__) || defined(__PIC24FJ512GB606__) || defined(__PIC24FJ256GB606__) || defined(__PIC24FJ128GB606__) ||
       //    defined(__PIC24FJ1024GA606__) || defined(__PIC24FJ512GA606__) || defined(__PIC24FJ256GA606__) || defined(__PIC24FJ128GA606__)

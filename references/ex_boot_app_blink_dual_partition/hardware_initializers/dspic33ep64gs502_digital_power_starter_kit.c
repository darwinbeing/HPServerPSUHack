/*
 * File:   dspic33ep64gs502_digital_power_starter_kit.c
 *
 * Initializes the device configuration fuses, clock frequency, UART2 pins,
 * for the dsPIC33EP64GS502 soldered on the Microchip Digital Power Starter Kit.
 *
 * Digital Power Starter Kit (DM330017-2) User's Guide, including schematic:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/50002330b.pdf
 *
 * dsPIC33EP64GS502 product page:
 * http://www.microchip.com/wwwproducts/en/dsPIC33EP64GS502
 *
 * To get UART access from a PC onto this board, connect an MCP2221A Breakout 
 * Module (ADM00559) to the J1 header on the left side of the PCB.  MCP2221A 
 * breakout module J3 pin 1 (TX) should connect to the top-most pin of J1 (pin 
 * closest to the LCD). Ensure the module power jumper is set for 3.3V.
 * 
 * An MCP2200 Breakout Module (ADM00393) can be used instead of the MCP2221A, 
 * but J1 pins 4 and 5 need to be bent away or otherwise left disconnected from 
 * the breakout module as they are I2C SDA and SCL lines on the Digital Power 
 * Starter Kit, but RTS/CTS signals on the breakout module.
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

#if defined(__dsPIC33EP64GS506__) || defined(__dsPIC33EP64GS504__)  || defined(__dsPIC33EP64GS502__) ||     \
    defined(__dsPIC33EP32GS506__) || defined(__dsPIC33EP32GS504__)  || defined(__dsPIC33EP32GS502__) ||     \
    defined(__dsPIC33EP16GS506__) || defined(__dsPIC33EP16GS504__)  || defined(__dsPIC33EP16GS502__)

#define FCY                 70000000ul

#include <xc.h>
#include <libpic30.h>   // For __delay32()
#include <stdarg.h>     // For va_args used by LCD_printf()
#include "../ezbl_integration/ezbl.h"


// Device Configuration Words
// Config words can be defined in a Bootloader project, Application project,
// or mixed between each other so long as any given flash/config write-word-
// size location has exectly one definition. On devices with flash Config
// words and a flash double word minimum programming size (0x4 program
// addresses), this means two adjacent Config words may not be mixed between
// projects and instead both Config words must be defined in the same project.
//
// These defaults place all (or almost all) in the Bootloader project as this
// is the safest from a bootloader-bricking standpoint.
#if defined(EZBL_BOOT_PROJECT)  // Compiling for a Bootloader Project
    EZBL_SET_CONF(_FSEC, BWRP_OFF & BSS_DISABLED & BSEN_OFF & GWRP_OFF & GSS_DISABLED & CWRP_OFF & CSS_DISABLED & AIVTDIS_OFF)
    EZBL_SET_CONF(_FOSCSEL, FNOSC_FRC & IESO_OFF)
    EZBL_SET_CONF(_FOSC, POSCMD_NONE & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSECME & PLLKEN_ON)        // No primary oscillator, recover OSC2 pin as GPIO, allow multiple PPS remappings, enable clock switching and fail safe clock monitor, wait for PLL to lock
    EZBL_SET_CONF(_FWDT, WDTPOST_PS256 & WDTPRE_PR32 & WDTEN_ON_SWDTEN & WINDIS_OFF & WDTWIN_WIN75) // ~256ms timeout when watchdog is turned on in software (not forced on in hardware), use ordinary (non-windowed) Watchdog Timer mode
    EZBL_SET_CONF(_FICD, ICS_PGD2 & JTAGEN_OFF & BTSWP_ON)
    EZBL_SET_CONF(_FDEVOPT, PWMLOCK_OFF & ALTI2C1_OFF & ALTI2C2_OFF & DBCC_OFF)
    #if defined(__DUAL_PARTITION)
    EZBL_SET_CONF(_FBOOT, BTMODE_DUAL)  // Default is BTMODE_SINGLE, BTMODE_DUAL is for ping-ponging, BTMODE_DUALPROT is for statically write-protecting Partition 1, BTMODE_DUALPRIV is reserved/do not use
    #endif
#else   // Compiling for an Application Project (EZBL_BOOT_PROJECT is not defined)
    //EZBL_SET_CONF_FBTSEQ(4096)        // Use this is if you need to explicitly set the FBTSEQ value in code. Generally, you should NOT have this defined as it will be computed at run-time based on the programming order and predefined values will force an erase-modify-write of the whole Flash page contents.
    EZBL_SET_CONF(_FALTREG, CTXT1_IPL7 & CTXT2_IPL6)                                                // DO NOT assign a register set to IPL4 used by the NOW timer/NOW_TaskDispatcher() without changing the NOW IPL to something different.
#endif  // Goes to: #if defined(EZBL_BOOT_PROJECT)



// Set flash page erase size = 0x400 program addresses = 1536 Flash bytes/512 instruction words/256 Flash double words.
// PIC24FJ1024GB610/GA610 family devices implement 0x800; dsPIC33EP64GS506, dsPIC33EP128GS808, PIC24FJ256GB412/GA412 family devices implement 0x400.
// Single Partition bootloaders don't have to define this as it is automatically obtained by ezbl_tools.jar and inserted in the .gld linker script.
EZBL_SetSYM(EZBL_ADDRESSES_PER_SECTOR, 0x400);



//const unsigned int EZBL_i2cSlaveAddr = 0x60;          // Define I2C Slave Address that this Bootloader will listen/respond to, applicable only if I2C_Reset() is called
EZBL_FIFO *EZBL_COMBootIF __attribute__((persistent));  // Pointer to RX FIFO to check activity on for bootloading
const long EZBL_COMBaud = -230400;                      // Communications baud rate: <= 0 means auto-baud with default value (for TX) set by the 2's complemented value; otherwise baud in bits/sec (ex: 460800)



#define LCD_RST         _LATB15     // 15, PGEC3/SCL2/RP47/RB15
#define LCD_RST_TRIS    _TRISB15
#define LCD_CS          _LATB11     // 21, TMS/PWM3H/RP43/RB11
#define LCD_CS_TRIS     _TRISB11
#define LCD_RS          _LATB5      // 16, TDO/AN19/PGA2N2/RP37/RB5
#define LCD_RS_TRIS     _TRISB5
#define LCD_SCK         _LATB10     // 7, AN5/CMP2D/CMP3B/ISRC3/RP42/RB10
#define LCD_SCK_TRIS    _TRISB10
#define LCD_MOSI        _LATB1      // 9, OSC1/CLKI/AN6/CMP3C/CMP4A/ISRC2/RP33/RB1
#define LCD_MOSI_TRIS   _TRISB1


static const unsigned int line_address[] = {0x00, 0x40, 0x20, 0x60};  // Line 1 addresses are 0x00-0x1F, Line 2 addresses are 0x40-0x6F, Line 3 (if present) addresses are 0x20-0x3F, Line 4(if present) addresses are 0x60-0x7F
static const unsigned int xLimit = 16;                                // 16 characters per line on this LCD
static const unsigned int yLimit = 2;                                 // 2 line LCD

static unsigned long lastLCDUpdate  __attribute__((persistent));
static unsigned long busyInterval   __attribute__((persistent));    // NOW counter value indicating when the last command is finished
static int lastX, lastY             __attribute__((persistent));
static unsigned int emulatedLEDs    __attribute__((persistent));    // State holder for LEDs emulated on character LCD screen

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Goto(int x, int y);
void LCD_Put8(int data_instr, char c);
void LCD_Put(int x, int y, const char *data, unsigned int dataLen);
void LCD_PutStr(int x, int y, const char *s);
void LCD_printf(int x, int y, const char *format, ...);



// @return: FCY clock speed we just configured the processor for
unsigned long InitializeBoard(void)
{
    // Switch to FRC clock (no PLL), in case if the PLL is currently in use.
    // We are not allowed to change the PLL prescalar, postscalar or feedback
    // divider while it is running.
    __builtin_write_OSCCONH(0x00);
    __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);
    while(OSCCONbits.OSWEN);            // Wait for clock switch to complete

    // Configure PLL for Fosc = 140MHz/Fcy = 70MIPS using 7.37 MHz internal FRC oscillator
    CLKDIV = 0xB000; // ROI = 1, DOZE = 8:1, FRCDIV = 1:1, PLLPOST = 2:1, PLLPRE = 2:1
    PLLFBD = (FCY * 2u * 2u * 2u + 7370000u / 2u) / 7370000u - 2u; // 74 @ 70 MIPS (7.37 MHz input clock from FRC)
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

    
    // Set push button as GPIO input
    //
    // Function     dsPIC33EP64GS502 Device Pins
    // Button       PIC#, name
    // SW1          14, PGED3/RP8/CN8/RB8
    EZBL_DefineButtonMap(RB8);
    _TRISB8 = 1;


#if defined(XPRJ_i2c) || defined(XPRJ_default) || defined(EZBL_INIT_I2C)    // XPRJ_* definitions defined by MPLAB X on command line based on Build Configuration. If you need this interface always, you can alternatively define a project level EZBL_INIT_I2C macro.
    // Configure I2C 1 pins
    //
    // SDA1 = PGEC1/AN21/SDA1/RP39/RB7
    // SCL1 = PGED1/TDI/AN20/SCL1/RP38/RB6
    _CNPUB7 = 1;    // Enable weak pull up on SDA1 line
    _CNPUB6 = 1;    // Enable weak pull up on SCL1 line
    _ANSB6  = 0;    // 0 = Digital I/O; 1 = Analog I/O
    _ANSB7  = 0;    // 0 = Digital I/O; 1 = Analog I/O
    EZBL_COMBootIF = I2C_Reset(1, FCY, 0, EZBL_i2cSlaveAddr, 0);
#endif


#if defined(XPRJ_uart) || defined(XPRJ_default) || defined(EZBL_INIT_UART)    // XPRJ_* definitions defined by MPLAB X on command line based on Build Configuration. If you need this interface always, you can alternatively define a project level EZBL_INIT_UART macro.
    // Configure GPIO1 and GPIO2 as UART2 pins.
    // - Pin names are with respect to the PIC.
    //
    // Function     dsPIC33EP64GS502 Device Pins
    // UART2        PIC#, name
    // U2TX  (out)  23, GPIO2 - PWM2H/RP45/RB13
    // U2RX  (in)   22, GPIO1 - TCK/PWM3L/RP44/RB12
    IOCON2bits.PENH = 0;        // Disable PWM2H which is on by default and higher priority than PPS
    IOCON3bits.PENL = 0;        // Disable PWM3L which is on by default and higher priority than PPS
    _CNPUB12 = 1;               // Enable Weak pull up on U2RX line (GPIO1) so we don't receive spurious data while nobody is connected
    _U2RXR   = 44;              // U2RX on RP44
    _RP45R   = _RPOUT_U2TX;     // U2TX on RP45
    EZBL_SetSYM(UART2_TX_ISR_PRIORITY, 1);  // Optionally change the U2TX Interrupt priority. The default is IPL 1 (lowest possible priority).
    EZBL_SetSYM(UART2_RX_ISR_PRIORITY, 2);  // Optionally change the U2RX Interrupt priority. The default is IPL 2 (low priority).
    if(EZBL_COMBaud <= 0)       // If auto-baud enabled, delay our UART initialization so MCP2221A POR timer and init
    {                           // is complete before we start listening. POR timer max spec is 140ms, so MCP2221A TX
        NOW_Wait(140u*NOW_ms);  // pin glitching could occur long after we have enabled our UART without this forced delay.
    }
    EZBL_COMBootIF = UART_Reset(2, FCY, EZBL_COMBaud, 1);
#endif


//    // Configure UART1 on PGED2/RP35/RB3 and PGEC2/RP36/RB4 for extra debug
//    // purposes (like printing console messages while bootloading). These pins
//    // are used as PGED2/PGEC2, so you must unplug any air-wired UART hardware
//    // while doing ICSP programming or debug.
//    //
//    // Function     dsPIC33EP64GS502 Device Pins
//    // UART1        PIC#, name
//    // U1RX  (in)     11, "PGD2" - PGED2/AN18/DACOUT1/INT0/RP35/RB3
//    // U1TX  (out)    12, "PGC2" - PGEC2/ADTRG31/EXTREF1/RP36/RB4
//    _CNPUB3 = 1;            // Enable Weak pull up on U1RX line (PGD2) so we don't receive spurious data while nobody is connected
//    _ANSB3  = 0;
//    _ANSB4  = 0;
//    _U1RXR  = 35;           // U1RX on RP35
//    _RP36R  = _RPOUT_U1TX;  // U1TX on RP36
//    EZBL_SetSYM(UART1_TX_ISR_PRIORITY, 1);          // Optionally change the U1TX Interrupt priority. The default is IPL 1 (lowest possible priority).
//    EZBL_SetSYM(UART1_RX_ISR_PRIORITY, 2);          // Optionally change the U1RX Interrupt priority. The default is IPL 2 (low priority).
//    UART_Reset(1, FCY, 230400, 1);
//    EZBL_COMBootIF = &UART1_RxFifo;       // Code needed if for setting this RX interface as the active one for bootloader activity checking


    // Configure GPIO for LCD
    // Function             dsPIC33EP64GS502 Device Pins
    // LCD1                 PIC#, name
    // LCD_RS      (out)      16, TDO/AN19/PGA2N2/RP37/RB5
    // LCD_CSB     (out)      21, TMS/PWM3H/RP43/RB11
    // LCD_MOSI    (out)       9, OSC1/CLKI/AN6/CMP3C/CMP4A/ISRC2/RP33/RB1
    // LCD_SCK     (out)       7, AN5/CMP2D/CMP3B/ISRC3/RP42/RB10
    // LCD_RST     (out)      15, PGEC3/SCL2/RP47/RB15
    _LATB15  = 0;           // LCD_RESET = 0
    _TRISB15 = 0;
    IOCON3bits.PENH = 0;    // Disable PWM3H which is on by default and higher priority than PPS
    _ANSB5   = 0;
    _ANSB1   = 0;
    _ANSB10  = 0;
    _LATB11  = 1;           // LCD_CSB = 1
    _TRISB11 = 0;
    _LATB5   = 0;           // LCD_RS = 0   // Register Select = Instruction (0)
    _TRISB5  = 0;
    _LATB15  = 1;           // LCD_RESET = 1 now
    emulatedLEDs = 0x0000;
    LCD_Init();


    // I2C1 pins used for I2C (MCP23008 attached at I2C address 0x20)
    // MCP23008 data sheet:
    // http://ww1.microchip.com/downloads/en/DeviceDoc/21919e.pdf
    //
    // Function             dsPIC33EP64GS502 Device Pins
    // I2C1                 PIC#, name
    // SCL1                   14, PGED1/TDI/AN20/SCL1/RP38/RB6
    // SDA1                   15, PGEC1/AN21/SDA1/RP39/RB7
    _CNPUB6 = 1;    // Enable weak internal pull up (board already has 2.2K resistors, but may be convenient for testing on other PCBs without pull ups)
    _CNPUB7 = 1;    // Enable weak internal pull up (board already has 2.2K resistors, but may be convenient for testing on other PCBs without pull ups)
    _ANSB6 = 0;
    _ANSB7 = 0;


    // Report 70 MIPS on dsPIC33E
    return FCY;
}


/**
 *  @function LEDToggle Toggles the state of an array of LEDs using a logical
 *                  XOR of the ledBitField provided. I.e. only set bits toggle
 *                  an LED state. Clear bits have no effect.
 *  @param ledBitField A bitmask representing an array of LEDs that should be
 *                     toggled state. Each '1' bit in the bit field toggles the
 *                     corresponding LED on or off.
 *  @return Bitmask representing the LEDs that were previously on before
 *          XORing in the ledBitField value. Bits for LEDs that do not exist are
 *          returned as '0' values in the bitmask.
 */
unsigned int LEDToggle(unsigned int ledBitField)
{
    unsigned int i;
    unsigned int ret;
    char emuLEDLine[] = "LEDs: 00000000  ";

    // Early return if we were called just to obtain the current state (for LEDSet()/LEDOn()/LEDOff())
    if(!ledBitField)
        return emulatedLEDs;

    // No LEDs on Digital Power Starter Kit that can be readily controlled, so
    // remap them to emulated LEDs on the LCD.
    ret = emulatedLEDs;

    emulatedLEDs ^= ledBitField;
    for(i = 0; i < 8u; i++)
    {
        if(emulatedLEDs & (1u<<(7-i)))
            emuLEDLine[6+i] = '1';
    }

    LCD_PutStr(0, 1, emuLEDLine);
    return ret;
}


/**
 * Moves the LCD cursor to the specified x and y coordinates.
 *
 * @param x 0-based column number to seek to. 0 means the left-most character,
 *          15 or 19 is typically the right-most character, depending  on if the
 *          LCD is a 16 character or 20 character wide LCD.
 *
 * @param y 0-based row or line number to seek to. 0 means the top-most
 *          character/line, 1 or 3 is typically the bottom-most visible line.
 */
void LCD_Goto(int x, int y)
{
    if(x < 0)
        x = lastX;
    if(y < 0)
        y = lastY;

    LCD_Put8(0, 0x80 |  (line_address[y] | x)); // 0x80 = Set DDRAM Address: 1 AC6 AC5 AC4 AC3 AC2 AC1 AC0 Set DDRAM address in address counter. 26.3 �s @ 2.7V
    lastX = x;
    lastY = y;
}

/**
 * Erases all characters being displayed by the LCD and resets the cursor to
 * (0,0) - top left corner.
 */
void LCD_Clear(void)
{
    LCD_Put8(0, 0x01);  // Clear Display 0000000001 Write 0x20 to DDRAM and set DDRAM address to 0x00 1.08ms
}


/**
 * Writes a null terminated string to the character LCD
 *
 * @param s Null-terminated string to print
 */

void LCD_PutStr(int x, int y, const char *s)
{
    LCD_Goto(x, y);
    while(*s)
    {
        LCD_Put8(1, *s++);
    }
}

/**
 * Writes a set of bytes to the LCD for display.
 *
 * @param x Column or horizontal coordinate to start printing at, where 0 is the
 *          left-most visible character on a line.
 *
 *          Specify -1 for the current x position.
 *
 * @param y Row or line coordinate to start printing at, where 0 is the top-most
 *          visible line.
 *
 *          Specify -1 for the current y position.
 *
 * @param s Pointer to character array to write to the LCD
 *
 * @param dataLen Number of characters of data to write to the LCD
 */

void LCD_Put(int x, int y, const char *data, unsigned int dataLen)
{
    LCD_Goto(x, y);
    while(dataLen--)
    {
        LCD_Put8(1, *data++);
    }
}


/**
 * Initializes the (NHD-C0216CZ-FSW-FBW-3V3) 16x2 character LCD with a clear
 * display and visible cursor at 0,0 position.
 *
 * I/O pins and SPI peripheral are initialized.
 */
void LCD_Init(void)
{
    LCD_RST        = 0;
    LCD_CS         = 1;
    LCD_RST_TRIS   = 0; // LCD_RST = LCD Reset "RST", active low
    LCD_CS_TRIS    = 0; // LCD_CS = LCD Chip Select "CSB", active low
    LCD_RS_TRIS    = 0; // LCD_RS = LCD Register Select; 0 = Instruction, 1 = Data

    LCD_MOSI_TRIS  = 0; // LCD_MOSI = LCD Serial Data Input "SI"
    LCD_SCK_TRIS   = 0; // LCD_SCK  = LCD Serial Data Input Clock "SCL" - SPI-like, but idle state == 1

    LCD_RST        = 0; // Assert LCD active low RST reset signal
    NOW_Wait(25u*NOW_ms);
    LCD_RST        = 1; // end reset
    busyInterval   = 0; // No initial delay time

    LCD_Put8(0, 0x30);  // Wake Up - Function Set = 8-bits | Instruction Table 0
    busyInterval = 2u*NOW_ms;
    LCD_Put8(0, 0x30);  // Wake Up - Function Set = 8-bits | Instruction Table 0
    LCD_Put8(0, 0x30);  // Wake Up - Function Set = 8-bits | Instruction Table 0

    LCD_Put8(0, 0x39);  // Instruction Table 0: Function Set = 8-bits | 2-line | Instruction Table 1 select
    LCD_Put8(0, 0x14);  // Instruction Table 1: Internal OSC Frequency = 1/4 bias | F0 OSC adjust
    LCD_Put8(0, 0x56);  // Instruction Table 1: Power/ICON Control = ICON display off | Set boost circuit on | Internal contrast follower mode = 0x2
    LCD_Put8(0, 0x6D);  // Instruction Table 1: Follower Control = Follower circuit on | Follower amplifier ratio = 0x5
    LCD_Put8(0, 0x70);  // Instruction Table 1: Contrast Set = Contrast = 0x0
    LCD_Put8(0, 0x0C);  // Instruction Table 0: Display ON/OFF = Entire display on, Cursor off, Cursor Blink off
    LCD_Put8(0, 0x06);  // Instruction Table 0: Entry Mode Set = Post-write move cursor right, do not shift display
    LCD_Put8(0, 0x01);  // Instruction Table 0: Clear Display (and set cursor home at 0,0 position)
}


/**
 * Writes one byte of data to the LCD Display RAM or the LCD Instruction
 * handler.
 *
 * If a prior command or data was executed recently, this function blocks until
 * the prior command/data is finished executing/writing. Upon return, a software
 * timer is captured to correctly discern in the future how long a delay may be
 * needed to complete this command, if not finished by the subsequent call.
 *
 * This function internally handles Chip Select assertion/deassertion with chip
 * select always being deasserted upon return.
 *
 * @param data_instr Write target: 0 = LCD Instruction handler; 1 = Display RAM
 *
 * @param c Byte of data to write to the LCD.
 */
void LCD_Put8(int data_instr, char c)
{
    unsigned int i;

    LCD_SCK       = 1;               // Idle state is high
    LCD_SCK_TRIS  = 0;
    while((unsigned long)(NOW_32() - lastLCDUpdate) < busyInterval); // Wait for last transaction to complete with necessary hold time, if any
    LCD_CS        = 0;               // Assert chip select - 20ns set up time to clock rising edge, 350ns hold time @ 2.7V
    LCD_CS_TRIS   = 0;
    LCD_RS        = data_instr;      // Select RS = Instruction target (0) or Data RAM (0) - 10ns set up time to clock rising edge, 250ns hold time @ 2.7V
    LCD_RS_TRIS   = 0;
    LCD_MOSI_TRIS = 0;


    for(i = 0; i < 8u; i++)
    {
        LCD_SCK  = 0;       // Clock low
        LCD_MOSI = 0;       // Generate output data - MSbit first
        if(c & 0x80)
            LCD_MOSI = 1;
        c <<= 1;
        __delay32(32);      // Clock low for at least 160ns @ 2.7V. 32-CPU clocks will be good for at least up to 200MHz
        LCD_SCK  = 1;       // Clock high - 20ns min with 200ns min clock period
        __delay32(8);       // Clock low for at least 160ns @ 2.7V. 8-CPU clocks will be good for at least up to 200MHz
    }

    lastLCDUpdate = NOW_32();
    busyInterval = 32u*NOW_us;  // 26.3us execution time by default assumed
    if(!data_instr)         // Do special things for certain instructions
    {
        if((c & 0x03) == c) // Clear Display (0x01) or Return Home (0x02 or 0x03) take 1.08ms @ slowest clock; everything else 26.3us
        {
            busyInterval = 2u*NOW_ms;
            lastX = 0;
            lastY = 0;
        }
    }
    else
    {   // Special things for data
        lastX++;
    }

    __delay32(32);
    LCD_CS = 1;                                     // Deselect LCD; 350ns hold time, which above code surely meets
}



/**
 * Writes the specified data to the LCD using a printf()-style variable argument
 * list. All existing characters on the LCD not specifically set with new
 * data are erased to whitespace.
 *
 * Internally calls vsprintf(), so see printf() compiler documentation for exact
 * formatting parameters.
 *
 * Buffer space is allocated for a 2 line x 16 character LCD, so ensure the
 * specified parameters to print do not exceed 32 characters. Excess characters
 * will cause stack overflow and undefined behavior.
 *
 * This is a blocking function that returns upon write completion.
 *
 * An example calling sequence to get a known number of characters per line is:
 *      LCD_printf("Buck %01.2fV %01.2fW"
 *                 "Bst % 2.2fV %01.2fW", buckVoltage, buckLoadPower, boostVoltage, boostLoadPower);
 * where buckVoltage, buckLoadPower, boostVoltage, and boostLoadPower are all of
 * type double. "01.2" " 2.2" specify digits to display to the left and
 * right of the radix point, with '0' or ' ' (space) characters to display as
 * padding if the value is < 1.00. Two back-to-back string literals in C are
 * automatically concatenated into one string (note that there is no comma
 * separating them).
 */
void LCD_printf(int x, int y, const char *format, ...)
{
    extern int EZBL_vsprintf(char *outputBuf, const char *format, void *args);  // Prototype so we don't have to include ezbl.h. NOTE: to use this function and not get a link error for the EZBL_vsprintf symbol, you must have ezbl_lib.a included in your project tree under Libraries.

    va_list args;
    unsigned int bufSize;
    char *c;
    unsigned int escapeLevel;

    bufSize = xLimit*yLimit + 32u;          // Extra 32 bytes for null termination, non-printing formatting chars like '\r' or '\n' and in case if sprintf() overflows. sprintf() does not support a max output length parameter.
    c = (char*)__builtin_alloca(bufSize);   // Allocate temporary buffer on the stack

    va_start(args, format); // Initialize variable arguments list
    EZBL_vsprintf(c, format, args);
    va_end(args);           // Clean up variable arguments list (does nothing in GNU C)

    escapeLevel = 0;
    LCD_Goto(x, y);         // Start at the specified position
    while(1)
    {
        if(*c == 0x00)      // Null terminator
            return;

        if(escapeLevel == 0u)
        {
            switch(*c)
            {
                case '\r':                  // Erase all other characters on this line until we reach the end, then reposition cursor to beginning of this same line
                    while(x++ < xLimit)
                        LCD_Put8(1, ' ');
                    x = 0;
                    LCD_Goto(x, y);
                    break;

                case '\n':                  // Don't erase any characters, just move down one line and seek to x = 0
                    if(++y >= yLimit)
                        return;
                    x = 0;
                    LCD_Goto(x, y);
                    break;

                case '\t':
                    if(x >= xLimit)
                    {
                        x = 0;
                        if(++y >= yLimit)
                            return;
                        LCD_Goto(x, y);
                        break;
                    }
                    while(x++ < xLimit)
                    {
                        LCD_Put8(1, ' ');       // Write spaces until we reach the next multiple of 4 boundary
                        if((x & 0x3u) == 0x0u)
                            break;
                    }
                    break;

                case '\f':  // Form Feed (0x0C). This will explicitly clear the screen and put the cursor at position (0,0).
                    LCD_Put8(0, 0x01);   // Clear Display 0000000001 Write 0x20 to DDRAM and set DDRAM address to 0x00 1.08ms                    x = 0;
                    x = 0;
                    y = 0;
                    break;

                case 0x1B:  // ^[ (CTRL+[) escape character key. Use '\x1B' in a string to invoke this. Ex: "\x1BC\x1BC\x1BC\x1BC0x%04X" will keep 4 characters unchanged on the LCD, followed by the printing of an uppercase hexadecimal integer padded to 4 digits.
                    escapeLevel = 1;
                    break;

                default:
                    if(x++ >= xLimit)
                    {
                        x = 0;
                        if(++y >= yLimit)
                            return;
                        LCD_Goto(x, y);
                    }
                    LCD_Put8(1, *c);
                    break;
            }
        }
        else if(escapeLevel == 1u)
        {
            switch(*c)
            {
                case 'A':   // Up - move up a line, maintaining x position and saturating at y = 0
                    if(y > 0u)
                        y--;
                    break;

                case 'B':   // Down - same as \n, but saturates just after the last line without returning
                    if(y < yLimit)
                        y++;
                    break;

                case 'C':   // Right - move right a character without affecting old contents, saturating at the end of the line
                    if(x < xLimit)
                        ++x, y;
                    break;

                case 'D':   // Left - move left a character without affecting old contents, saturating at x = 0
                    if(x > 0u)
                        x--;
                    break;

                case 'F':   // End (of the current line - actually the last character in the printable line)
                    x = xLimit;
                    break;

                case 'H':   // Home (of the current line)
                    x = 0;
                    break;

                default:    // Unsupported - just eat this character
                    break;
            }
            LCD_Goto(x, y);
            escapeLevel = 0;
        }

        if(--bufSize == 0u)
            return;
        c++;
    }
}

//----------------------------------------------------------------------------


#endif //#if defined(__dsPIC33EP64GS506__) || defined(__dsPIC33EP64GS504__)  || defined(__dsPIC33EP64GS502__) || defined(__dsPIC33EP32GS506__) || defined(__dsPIC33EP32GS504__)  || defined(__dsPIC33EP32GS502__) || defined(__dsPIC33EP16GS506__) || defined(__dsPIC33EP16GS504__)  || defined(__dsPIC33EP16GS502__)


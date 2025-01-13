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
#ifndef __LCD_H
#define __LCD_H

#include <xc.h>
#include <stdint.h>
#include "define.h"
#include <libpic30.h>



//******************************************************/
//		LCD DEFINES
//******************************************************/
// Registers and Register Bits
// LCD port pins
#define LCD_CS              _LATB11
#define LCD_SCL		        _LATB10
#define LCD_SDI		        _LATB1
#define LCD_RS		        _LATB5
#define LCD_RST		        _LATB15


#define TRIS_LCD_CS         _TRISB11
#define TRIS_LCD_SCL        _TRISB10
#define TRIS_LCD_SDI        _TRISB1
#define TRIS_LCD_RS         _TRISB5
#define TRIS_LCD_RST        _TRISB15


#if defined(__dsPIC33EP128GS702__)
#undef LCD_CS
#undef LCD_SCL
#undef TRIS_LCD_CS
#undef TRIS_LCD_SCL
#define LCD_CS              PCL         // Changed from _LATB11 to PCL on TLAY DPSK - a read only register being used as a write bit-bucket
#define LCD_SCL		        _LATB11     // Changed from _LATB10 to _LATB11 in TLAY DPSK
#define TRIS_LCD_CS         PCL         // Changed from _LATB11 to PCL on TLAY DPSK - a read only register being used as a write bit-bucket
#define TRIS_LCD_SCL        _TRISB11    // Changed from _TRISB10 to _TRISB11 in TLAY DPSK
#endif


// Number Assignment, Misc.
#define LCD_RS_COMMAND      0
#define LCD_RS_DATA         1
#define LCD_LOW             0
#define LCD_HIGH            1
#define LCD_SELECTED        0
#define LCD_nSELECTED       1


//******************************************************/
//		LCD Prototypes
//******************************************************/
extern void LCD_Init(void);
extern void lcd_clear(void);
extern void lcd_goto(uint8_t x, uint8_t y);
extern void lcd_putchar(const char c);
extern void lcd_putstr(const char *s);
extern void lcd_putcmd(uint8_t Command);
extern void lcd_putline(uint8_t l, const char *s);
extern void lcd_backlight_on(void);
extern void lcd_backlight_off(void);


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
 *      lcd_printf("Buck %01.2fV %01.2fW"
 *                 "Bst % 2.2fV %01.2fW", buckVoltage, buckLoadPower, boostVoltage, boostLoadPower);
 * where buckVoltage, buckLoadPower, boostVoltage, and boostLoadPower are all of 
 * type double. "01.2" " 2.2" specify digits to display to the left and 
 * right of the radix point, with '0' or ' ' (space) characters to display as 
 * padding if the value is < 1.00. Two back-to-back string literals in C are 
 * automatically concatenated into one string (note that there is no comma 
 * separating them).
 */
void lcd_printf(const char *format, ...);


//----------------------------------------------------------------------------

#endif // __LCD_H

//******************************************************/
//		LCD_cmds Defines
//******************************************************/
// Command set for the LCD LCD driver used by the Newhaven NHD-C0216CZ-FSW-FBW

#ifndef __LCD_CMDS_H
#define __LCD_CMDS_H

#define LCD_CLEAR       0x01
#define LCD_HOME        0x02

#define CURSOR_nSHIFT   0x00
#define CURSOR_SHIFT    0x01
#define DATA_DECREMENT  0x00
#define DATA_INCREMENT  0x02
#define LCD_ENTRY_MODE  0x04

#define CURSOR_OFF      0x00
#define CURSOR_ON       0x02
#define BLINK_OFF       0x00
#define BLINK_ON        0x01
#define LCD_DISPLAY_OFF 0x08
#define LCD_DISPLAY_ON  0x0C


#define FUNCTION_nIS    0x00
#define FUNCTION_IS     0x01
#define FUNCTION_1_HIGH 0x00
#define FUNCTION_2_HIGH 0x04
#define FUNCTION_1_LINE 0x00
#define FUNCTION_2_LINE 0x08
#define FUNCTION_4BITS  0x00
#define FUNCTION_8BITS  0x10

#define LCD_FUNCTION    0x20

#define LCD_CGRAM_ADDRESS(adr) (0x40 | (adr & 0x3F))
#define LCD_DDRAM_ADDRESS(adr) (0x80 | (adr & 0x7F))

//** Second Instruction Page (IS)
#define BIAS_1_5        0x00
#define BIAS_1_4        0x08
#define FREQ_CNTRL(f)   (f&0x07)
#define LCD_OSC_FREQ    0x10


#define LCD_ICON_ADDRESS(adr) (0x40 | (adr & 0x0F))

#define nICON               0x00
#define ICON                0x08
#define nBOOST              0x00
#define BOOSTLCD            0x04
#define CONTRAST(c)         (c&0x03)
#define LCD_PWR_CONTROL     0x50

#define FOLLOWER_GAIN(g)    (g&0x07)
#define LCD_FOLLOWER_OFF    0x60
#define LCD_FOLLOWER_ON     0x68

#define LCD_CONTRAST(c)     (0x70 | (c & 0x0F))

#define LCD_BUSY_FLAG_MASK  0x80
#define LCD_ADDRESS_MASK    0x7F

#define LCD_ADDRESS_LINE_1  0x00
#define LCD_ADDRESS_LINE_2  0x40

#endif // ifndef __LCD_H
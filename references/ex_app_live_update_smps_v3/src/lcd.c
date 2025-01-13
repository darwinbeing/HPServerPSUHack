////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2015 Microchip Technology Inc.
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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "lcd.h"


const int8_t line_address[] = {LCD_ADDRESS_LINE_1,LCD_ADDRESS_LINE_2};
const uint8_t xLimit = 16;    // 16 characters per line
const uint8_t yLimit = 2;     // 2 line LCD

///////////////////////////////////////////////////////////////////////////////
// Function:        SPI_TX
//
// PreCondition:    LCD must be setup.
//
// Side Effects:    Writes to screen
//
///////////////////////////////////////////////////////////////////////////////
// @ingroup         LCD
//
// @brief           This function writes to the LCD screen.
//
// @note            Send byte via spi to the lcd
//
///////////////////////////////////////////////////////////////////////////////
void SPI_TX(uint8_t Command)
{
  uint8_t mask;

   for(mask = 0x80; mask != 0; mask >>= 1)
   {
        LCD_SCL = LCD_LOW;
        __delay_us(1);
        LCD_SDI = LCD_LOW;
        if(Command & mask)
        {
            LCD_SDI = LCD_HIGH;
        }
        LCD_SCL = LCD_HIGH;
        __delay_us(1);
    }
    LCD_CS  = LCD_nSELECTED;

    __delay_us(50);
}



//////////////////////////////////////////////////////////////////////////////
// Function:             lcd_goto
//
// PreCondition: 	None
//
// Side Effects: 	Moves cursor.
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// @ingroup  		LCD
//
// @brief    		This function points to an LCD position.
//
// @note    		None
//
//////////////////////////////////////////////////////////////////////////////
void lcd_goto(uint8_t x,uint8_t y)
{
    lcd_putcmd(LCD_DDRAM_ADDRESS((line_address[y] + x)));
}

//////////////////////////////////////////////////////////////////////////////
// Function:             lcd_clear
//
// PreCondition: 	None
//
// Side Effects: 	Clears LCD
//
//
// @ingroup  		LCD
//
// @brief    		This function clears the LCD screen.
//
// @note    		None
//
//////////////////////////////////////////////////////////////////////////////
void lcd_clear(void)
{
    lcd_putcmd(LCD_CLEAR);   //clear
    __delay_us(20);
}

//////////////////////////////////////////////////////////////////////////////
// Function:             lcd_putline
//
// PreCondition: 	None
//
// Side Effects: 	None
///
///
// @ingroup  		LCD
//
// @brief    		This function sends an LCD message.
//
// @note    		Sends LCD message and line location
//
//////////////////////////////////////////////////////////////////////////////
void lcd_putline(uint8_t l, const char *buffer)
{
    lcd_putcmd(line_address[l]);
    lcd_putstr(buffer);
}

//////////////////////////////////////////////////////////////////////////////
// Function:             lcd_putstr
//
// PreCondition: 	None
//
// Side Effects: 	Writes message to LCD
//
//
// @ingroup  		LCD
//
// @brief    		This function writes a string to the LCD.
//
// @note    		Write data to LCD up to null
//
//////////////////////////////////////////////////////////////////////////////
void lcd_putstr(const char *s)
{
    while(*s)
    {
        lcd_putchar(*s++);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Function:         LCD_Init
//
// PreCondition: 	None
//
// Side Effects: 	Sets up LCD
//
//
// @ingroup  		LCD
//
// @brief    		This function initialises the LCD.
//
// @note    		None
//
//////////////////////////////////////////////////////////////////////////////
void LCD_Init(void)
{
   TRIS_LCD_RST   = 0;
   TRIS_LCD_CS    = 0;
   TRIS_LCD_RS    = 0;

   TRIS_LCD_SDI   = 0;
   TRIS_LCD_SCL   = 0;

   LCD_RST        = 0;  // RESET

   __delay_ms(25);

   LCD_RST        = 1;  // end reset

   lcd_putcmd(LCD_FUNCTION | FUNCTION_8BITS | FUNCTION_1_HIGH | FUNCTION_1_LINE | FUNCTION_nIS);   //function set

   __delay_ms(25);

   lcd_putcmd(LCD_FUNCTION | FUNCTION_8BITS | FUNCTION_1_HIGH | FUNCTION_1_LINE | FUNCTION_nIS);   //function set
   lcd_putcmd(LCD_FUNCTION | FUNCTION_8BITS | FUNCTION_1_HIGH | FUNCTION_1_LINE | FUNCTION_nIS);   //function set

   // Enter the second page of instructions
   lcd_putcmd(LCD_FUNCTION | FUNCTION_8BITS | FUNCTION_1_HIGH | FUNCTION_2_LINE | FUNCTION_IS);   //function set
   lcd_putcmd(LCD_OSC_FREQ | BIAS_1_5 | FREQ_CNTRL(4));   //internal osc frequency
   lcd_putcmd(LCD_PWR_CONTROL | nICON | BOOSTLCD | CONTRAST(2));   //power control
   lcd_putcmd(LCD_FOLLOWER_ON | FOLLOWER_GAIN(5));   //follower control
   lcd_putcmd(LCD_CONTRAST(0));   //contrast
   // leave second instruction page

   //lcd_putcmd(LCD_FUNCTION | FUNCTION_8BITS | FUNCTION_1_HIGH | FUNCTION_2_LINE | FUNCTION_nIS);   //function set
   lcd_putcmd(LCD_DISPLAY_ON | CURSOR_OFF | BLINK_OFF);           //display on
   lcd_putcmd(LCD_ENTRY_MODE | CURSOR_nSHIFT | DATA_INCREMENT);   //entry mode
   lcd_putcmd(LCD_CLEAR);   //clear

   __delay_ms(150);

   // Load LCD Startup Display
   lcd_printf(" Digital Power\r\n"
              "  Starter Kit\r");
   __delay_ms(750);
   lcd_printf("   MICROCHIP\r\n"
              "dsPIC33EP64GS502\r");
   __delay_ms(750);
   lcd_printf("  Live  Update\r\n"
              "  Demonstration\r");
   __delay_ms(750);
}

/////////////////////////////////////////////////////////////////////////////
// Function:             lcd_putcmd
//
// PreCondition: 	None
//
// Side Effects: 	Sends command to the LCD.
//
//
// @ingroup  		LCD
//
// @brief    		This function sends a command to the LCD.
//
// @note    		Send comand byte to the lcd
//
//////////////////////////////////////////////////////////////////////////////
void lcd_putcmd(uint8_t Command)
{
   LCD_CS = LCD_SELECTED;
   LCD_RS = LCD_RS_COMMAND;
   SPI_TX(Command);
}

//////////////////////////////////////////////////////////////////////////////
// Function:             lcd_putchar
//
// PreCondition: 	None
//
// Side Effects: 	Puts character on LCD
//
//
// @ingroup  		LCD
//
// @brief    		This function puts a character onto the screen.
//
// @note    		Write data to the lcd
//
//////////////////////////////////////////////////////////////////////////////
void lcd_putchar(const char c)
{
   LCD_CS = LCD_SELECTED;
   LCD_RS = LCD_RS_DATA;
   SPI_TX(c);
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
 *      lcd_printf("Buck %01.2fV %01.2fW"
 *                 "Bst % 2.2fV %01.2fW", buckVoltage, buckLoadPower, boostVoltage, boostLoadPower);
 * where buckVoltage, buckLoadPower, boostVoltage, and boostLoadPower are all of 
 * type double. "01.2" " 2.2" specify digits to display to the left and 
 * right of the radix point, with '0' or ' ' (space) characters to display as 
 * padding if the value is < 1.00. Two back-to-back string literals in C are 
 * automatically concatenated into one string (note that there is no comma 
 * separating them).
 */
void lcd_printf(const char *format, ...)
{
    extern int EZBL_vsprintf(char *outputBuf, const char *format, void *args);  // Prototype so we don't have to include ezbl.h. NOTE: to use this function and not get a link error for the EZBL_vsprintf symbol, you must have ezbl_lib.a included in your project tree under Libraries.
    
    va_list args;
    unsigned char x;
    unsigned char y;
    unsigned int bufSize;
    char *c;
    unsigned int escapeLevel;
    
    bufSize = xLimit*yLimit + 32u;          // Extra 32 bytes for null termination, non-printing formatting chars like '\r' or '\n' and in case if sprintf() overflows. sprintf() does not support a max output length parameter.
    c = (char*)__builtin_alloca(bufSize);   // Allocate temporary buffer on the stack
    
    va_start(args, format); // Initialize variable arguments list
    EZBL_vsprintf(c, format, args);
    va_end(args);           // Clean up variable arguments list (does nothing in GNU C)

    x = 0;
    y = 0;
    escapeLevel = 0;
    lcd_goto(x, y);     // Start at the home position
    while(1)
    {
        if(*c == 0x00)  // Null terminator
            return;
        
        if(escapeLevel == 0u)
        {
            switch(*c)
            {
                case '\r':                  // Erase all other characters on this line until we reach the end, then reposition cursor to beginning of this same line
                    while(x++ < xLimit)
                        lcd_putchar(' ');   
                    x = 0;
                    lcd_goto(x, y);
                    break;

                case '\n':                  // Don't erase any characters, just move down one line and seek to x = 0
                    if(++y >= yLimit)
                        return;
                    x = 0;
                    lcd_goto(x, y);
                    break;

                case '\t':
                    if(x >= xLimit)
                    {
                        x = 0;
                        if(++y >= yLimit)
                            return;
                        lcd_goto(x, y);
                        break;
                    }
                    while(x++ < xLimit)
                    {
                        lcd_putchar(' ');       // Write spaces until we reach the next multiple of 4 boundary
                        if((x & 0x3u) == 0x0u)
                            break;
                    }
                    break;
                    
                case '\f':  // Form Feed (0x0C). This will explicitly clear the screen and put the cursor at position (0,0).
                    x = 0;
                    for(y = 0; y < yLimit; y++)
                    {
                        lcd_goto(x, y);
                        for(x = 0; x < xLimit; x++)
                            lcd_putchar(' ');
                    }
                    x = 0;
                    y = 0;
                    lcd_goto(x, y);
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
                        lcd_goto(x, y);
                    }
                    lcd_putchar(*c);
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
            lcd_goto(x, y);
            escapeLevel = 0;
        }
        
        if(--bufSize == 0u)
            return;
        c++;
    }
}

//----------------------------------------------------------------------------

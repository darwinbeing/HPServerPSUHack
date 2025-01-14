//======================================================================================================================
// Copyright(c) 2018 Microchip Technology Inc. and its subsidiaries.
// Subject to your compliance with these terms, you may use Microchip software and any derivatives exclusively with
// Microchip products. It is your responsibility to comply with third party license terms applicable to your use of
// third-party software (including open source software) that may accompany Microchip software.
// THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO
// THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
// PURPOSE.
// IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE,
// COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED
// OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY
// ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE
// PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
//======================================================================================================================

//======================================================================================================================
// @file dev_lcd.c
// @author M91406
// @brief LC display device driver source file
//
//======================================================================================================================

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer types header file
#include <stdbool.h> // include standard boolean types header file

#include "common/delay.h"
#include "lcd/drivers/drv_lcd_interface.h" 

/*********************************************************************************
 * @ingroup lib-layer-lcd-properties-private
 * @{
 * @brief  Newhaven NHD-C0216CZ-FSW-FBW LCD controller command set
 * @details
 *  Defines for the command set of a Newhaven NHD-C0216CZ-FSW-FBW LCD controller
 **********************************************************************************/

//==================================================================================
// Private defines
//==================================================================================

// there is a difference in real display size and display memory
#define LCD_ADDRESS_LINE_1 0x00
#define LCD_ADDRESS_LINE_2 0x40

#define LCD_DISPLAYSIZE_X   16
#define LCD_DISPLAYSIZE_Y   2

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

// Second Instruction Page (IS)
#define BIAS_1_5      0x00
#define BIAS_1_4      0x08
#define FREQ_CNTRL(f) (f&0x07)
#define LCD_OSC_FREQ  0x10

#define LCD_ICON_ADDRESS(adr) (0x40 | (adr & 0x0F))

#define nICON           0x00
#define ICON            0x08
#define nBOOST          0x00
#define BOOSTLCD        0x04
#define CONTRAST(c)    (c&0x03)
#define LCD_PWR_CONTROL 0x50

#define FOLLOWER_GAIN(g) (g&0x07)
#define LCD_FOLLOWER_OFF   0x60
#define LCD_FOLLOWER_ON    0x68

#define LCD_CONTRAST(c) (0x70 | (c & 0x0F))

#define LCD_BUSY_FLAG_MASK 0x80
#define LCD_ADDRESS_MASK   0x7F

//==================================================================================
// Private Variables
//==================================================================================

const uint8_t line_address[] = {LCD_ADDRESS_LINE_1, LCD_ADDRESS_LINE_2};
uint8_t pos_x = 0;
uint8_t pos_y = 0;
uint8_t change_position = false;

/** @} */ // lib-layer-lcd-properties-private


/*********************************************************************************
 * @ingroup lib-layer-lcd-functions-public
 * @fn void dev_Lcd_Initialize(void)
 * @brief  Initializes the LCD Device
 * @param  void
 * @return void
 * @details
 *  This function initializes the LCD Device Driver. It needs to be called 
 *  once at boot-up before that device can be used.
 **********************************************************************************/

void dev_Lcd_Initialize(void)
{
    drv_LcdInterface_Initialize();
    drv_LcdInterface_Reset();
    
    drv_LcdInterface_SendCmd(LCD_FUNCTION | FUNCTION_8BITS | FUNCTION_1_HIGH | FUNCTION_1_LINE | FUNCTION_nIS);   //function set
 
    __delay_ms(25);

    drv_LcdInterface_SendCmd(LCD_FUNCTION | FUNCTION_8BITS | FUNCTION_1_HIGH | FUNCTION_1_LINE | FUNCTION_nIS);   //function set
    drv_LcdInterface_SendCmd(LCD_FUNCTION | FUNCTION_8BITS | FUNCTION_1_HIGH | FUNCTION_1_LINE | FUNCTION_nIS);   //function set

    // Enter the second page of instructions
    drv_LcdInterface_SendCmd(LCD_FUNCTION | FUNCTION_8BITS | FUNCTION_1_HIGH | FUNCTION_2_LINE | FUNCTION_IS);   //function set
    drv_LcdInterface_SendCmd(LCD_OSC_FREQ | BIAS_1_5 | FREQ_CNTRL(4));   //internal osc frequency
    drv_LcdInterface_SendCmd(LCD_PWR_CONTROL | nICON | BOOSTLCD | CONTRAST(2));   //power control
    drv_LcdInterface_SendCmd(LCD_FOLLOWER_ON | FOLLOWER_GAIN(5));   //follower control
    drv_LcdInterface_SendCmd(LCD_CONTRAST(0));   //contrast
    // leave second instruction page

    //drv_Lcd_WriteCommand(LCD_FUNCTION | FUNCTION_8BITS | FUNCTION_1_HIGH | FUNCTION_2_LINE | FUNCTION_nIS);   //function set
    drv_LcdInterface_SendCmd(LCD_DISPLAY_ON | CURSOR_OFF | BLINK_OFF);           //display on
    drv_LcdInterface_SendCmd(LCD_ENTRY_MODE | CURSOR_nSHIFT | DATA_INCREMENT);   //entry mode
    drv_LcdInterface_SendCmd(LCD_CLEAR);   //clear

    __delay_ms(150);
}

/*********************************************************************************
 * @ingroup lib-layer-lcd-functions-public
 * @fn void dev_Lcd_Clear(void)
 * @brief  Clears the LC Display Screen
 * @param  void
 * @return void
 * @details
 *  This function clears the LCD screen and sets the cursor position at 0,0 
 *  (left upper corner). Instead of calling this function, clearing the screen
 *  can also be achieved by sending the character '\f' within a string
 **********************************************************************************/

void dev_Lcd_Clear(void)
{
    drv_LcdInterface_SendCmd(LCD_CLEAR);
    __delay_ms(1);
}

/*********************************************************************************
 * @ingroup lib-layer-lcd-functions-public
 * @fn void dev_Lcd_GotoXY(volatile uint8_t x, volatile uint8_t y)
 * @brief  Sets the cursor position to the given x- and y-coordinates
 * @param  x  x-coordinate for the new cursor position starting with zero
 * @param  y  y-coordinate for the new cursor position starting with zero
 * @return void
 * @details
 *  This function sets the cursor position to the given x- and y-coordinates
 *  starting at (0, 0) in the upper left corner.
 **********************************************************************************/

void dev_Lcd_GotoXY(volatile uint8_t x, volatile uint8_t y)
{
    drv_LcdInterface_SendCmd(LCD_DDRAM_ADDRESS((line_address[y] + x)));
    pos_x = x;
    pos_y = y;
    change_position = false;
}

/*********************************************************************************
 * @ingroup lib-layer-lcd-functions-public
 * @fn void dev_Lcd_WriteChar(const char ch)
 * @brief  Writes a character on the LCD screen
 * @param  ch character be written to the LCD screen
 * @return void
 * @details
 *  This function writes the single character 'ch' to the currently 
 *  selected position on LCD screen. The following special characters 
 *  will result in a specific response of the LCD:
 * 
 * - Character '\f' clears the screen and positions the cursor on the upper left corner
 * - Character '\r' sets the x position of the cursor to 0
 * - Character '\n' puts the cursor on the next line (without changing the x-position)
 * 
 **********************************************************************************/

void dev_Lcd_WriteChar(const char ch)
{
    if (change_position)
    {
        dev_Lcd_GotoXY(pos_x, pos_y);
        change_position = false;
    }
                
    switch (ch)
    {
        case '\f':          //sets position to 0,0 after clearing the screen. this is slow (1ms)!
            dev_Lcd_Clear();
            pos_x = 0;
            pos_y = 0;
            change_position = false;
            break;
        case '\v':          //sets position to 0,0 without clearing the screen
            pos_x = 0;
            pos_y = 0;
            change_position = true;
            break;
        case '\r':  //carriage return ==> x=0;
            pos_x = 0;
            change_position = true;
            break;
        case '\n':  //new line return ==> y++;
            pos_y++;
            change_position = true;
            break;
        default:
            if (pos_x < LCD_DISPLAYSIZE_X && pos_y < LCD_DISPLAYSIZE_Y)
                drv_LcdInterface_SendChar(ch);
            if (++pos_x >= LCD_DISPLAYSIZE_X)
            {
                pos_x = 0;
                pos_y++;
                change_position = true;
            }
            break;
    }
}

/*********************************************************************************
 * @ingroup lib-layer-lcd-functions-public
 * @fn void dev_Lcd_WriteString(const char *str)
 * @brief  Writes a complete string on the LCD screen
 * @param  str Pointer to a string variable to be written on the LCD screen
 * @return void
 * @details
 *  This function writes a complete string 'str' to the currently 
 *  selected position on LCD screen. If the length of the string
 *  exceeds the available number display digits, the position will
 *  overrun and start again from position (0, 0).
 *  
 *  The following special characters within the string will result in a 
 *  specific response of the LCD:
 * 
 * - Character '\f' clears the screen and positions the cursor on the upper left corner
 * - Character '\r' sets the x position of the cursor to 0
 * - Character '\n' puts the cursor on the next line (without changing the x-position)
 * 
 **********************************************************************************/

void dev_Lcd_WriteString(const char *str)
{
    while(*str)
    {
        dev_Lcd_WriteChar(*str);
        str++;
    }
}

/*********************************************************************************
 * @ingroup lib-layer-lcd-functions-public
 * @fn void dev_Lcd_WriteStringXY(uint8_t column_index, uint8_t line_index, const char *str)
 * @brief  Sets the cursor position to the given x- and y-coordinates and writes the given string on the lcd screen
 * @param  column_index x-coordinates for the new cursor position starting with zero
 * @param  line_index   y-coordinates for the new cursor position starting with zero
 * @param  str pointer to a string variable to be written on the LCD screen
 * @return void
 * @details
 *  This function positions the cursor at the location defined by parameters
 *  x and y and writes a complete string 'str' to the newly selected 
 *  position on LCD screen. If the length of the string exceeds the available 
 *  number display digits, the position will overrun and start again from position 
 *  (0, 0) in the upper left corner.
 *  
 *  The following special characters within the string will result in a 
 *  specific response of the LCD:
 * 
 * - Character '\f' clears the screen and positions the cursor on the upper left corner
 * - Character '\r' sets the x position of the cursor to 0
 * - Character '\n' puts the cursor on the next line (without changing the x-position)
 * 
 **********************************************************************************/

void dev_Lcd_WriteStringXY(volatile uint8_t column_index, volatile uint8_t line_index, const char *str)
{
    dev_Lcd_GotoXY(column_index, line_index);
    dev_Lcd_WriteString(str);    
}

// end of file

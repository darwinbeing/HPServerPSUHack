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

#ifndef _Dev_LCD_H_
#define	_Dev_LCD_H_

#include <stdint.h>
#include <stdio.h>

#include "lcd/drivers/drv_lcd_interface.h"

/*********************************************************************************
 * @ingroup lib-layer-lcd-properties-public
 * @def TEMPSTR_LCD_SIZE
 * @brief  Defines the total string length of the LC display
 * @details
 *  This defines the total number of characters which can be displayed 
 *  on the LC display. This number may be subject to change if another
 *  LC display type is used.
 **********************************************************************************/
#define TEMPSTR_LCD_SIZE    40
    
/*********************************************************************************
 * @ingroup lib-layer-lcd-properties-public
 * @def PrintLcd
 * @brief  Writes a complete line to the LC display
 * @details
 *  This macro can be used to write a complete line to the LC display.
 *  If the length of the string exceeds the available number of characters
 *  in one line, the position will overflow into the next line.
 **********************************************************************************/

#define PrintLcd(LINE, ...) \
    do{ \
        char __print_utils_string[TEMPSTR_LCD_SIZE]; \
        sprintf(__print_utils_string, __VA_ARGS__); \
        dev_Lcd_WriteStringXY(0, LINE, __print_utils_string); \
    } while(0)



extern void dev_Lcd_Initialize(void);
extern void dev_Lcd_GotoXY(volatile uint8_t x, volatile uint8_t y);
extern void dev_Lcd_WriteChar(const char ch);
extern void dev_Lcd_WriteString(const char *str);
extern void dev_Lcd_WriteStringXY(volatile uint8_t column_index, volatile uint8_t line_index, const char *str);


#endif	// _Dev_LCD_H_


/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   app_lcd.h
 * Author: M91406
 * Comments: LCD application layer
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef APPLICATION_LAYER_LC_DISPLAY_H
#define	APPLICATION_LAYER_LC_DISPLAY_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

#include "lcd/devices/dev_lcd.h"


// PUBLIC TYPE DECLARATIONS

/***********************************************************************************
 * @ingroup app-layer-lcd-properties-public
 * @struct LCD_s
 * @brief Declaration of public LC display data object
 * @details
 *  The struct LCD_s data structure defines data fields for all publicly 
 *  accessible user settings required to configure and drive an on-board LC 
 *  display.
 **********************************************************************************/

typedef struct LCD_s{
    volatile bool enabled;
    volatile uint16_t refresh;
    volatile uint16_t screen;
    volatile uint16_t screens;
}LCD_t;
    


// PUBLIC VARIABLE DECLARATION
extern volatile LCD_t lcd;
    
// PUBLIC FUNCTION PROTOTYPE DECLARATION
extern volatile uint16_t appLCD_Initialize(void);
extern volatile uint16_t appLCD_Execute(void);
extern volatile uint16_t appLCD_Dispose(void);

#endif	/* APPLICATION_LAYER_LC_DISPLAY_H */


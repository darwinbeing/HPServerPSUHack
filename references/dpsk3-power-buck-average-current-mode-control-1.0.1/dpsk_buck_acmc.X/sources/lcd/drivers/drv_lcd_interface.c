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
// @file drv_lcd_interface.c
// @brief driver for the spi-interface to access the LCD controller
//======================================================================================================================

#include <stdint.h>
#include "common/delay.h"
#include "config/hal.h"


// PRIVATE FUNCTION CALL PROTOTYPES
void drv_LcdInterface_SpiSend(uint8_t data);

/*********************************************************************************
 * @ingroup lib-driver-layer-lcd-interface-properties-private
 * @{
 * @brief  Initial interface pin state conditions
 **********************************************************************************/

#define LCD_CS_nSELECTED    LCD_CS_SetHigh
#define LCD_CS_SELECTED     LCD_CS_SetLow

#define LCD_RS_COMMAND      LCD_RS_SetLow
#define LCD_RS_DATA         LCD_RS_SetHigh

/** @} */ // end of group lib-driver-layer-lcd-interface-properties-private

/*********************************************************************************
 * @ingroup lib-driver-layer-lcd-interface-functions-public
 * @fn void drv_LcdInterface_Initialize(void)
 * @brief  Initializes the LCD interface driver
 * @details 
 *  This function needs to be called once at startup before calling 
 *  any other function of the LCD device driver
 **********************************************************************************/

void drv_LcdInterface_Initialize(void)
{
    LCD_RST_SetHigh();
    LCD_CS_nSELECTED();
    LCD_SCL_SetLow();
    LCD_SDI_SetLow();
    LCD_RS_SetLow();
    
    LCD_RST_SetDigitalOutput();
    LCD_CS_SetDigitalOutput();
    LCD_SCL_SetDigitalOutput();
    LCD_SDI_SetDigitalOutput();
    LCD_RS_SetDigitalOutput();
}

/*********************************************************************************
 * @ingroup lib-driver-layer-lcd-interface-functions-public
 * @fn void drv_LcdInterface_Reset(void)
 * @brief  Resets the LCD controller
 * @details 
 *  This function needs to be called once after system startup to reset 
 *  the LCD controller and get into a defined state.
 **********************************************************************************/

void drv_LcdInterface_Reset(void)
{
    LCD_RST_SetLow();
    __delay_ms(25);
    LCD_RST_SetHigh();
}

/*********************************************************************************
 * @ingroup lib-driver-layer-lcd-interface-functions-public
 * @fn void drv_LcdInterface_SendCmd(uint8_t data)
 * @brief  Sends a command to the LCD controller
 * @details 
 *  This function sends a one byte long command via the initialized SPI interface 
 *  to the LCD controller.
 **********************************************************************************/

void drv_LcdInterface_SendCmd(uint8_t cmd)
{		
   LCD_CS_SELECTED();
   LCD_RS_COMMAND();
   drv_LcdInterface_SpiSend(cmd);
}

/*********************************************************************************
 * @ingroup lib-driver-layer-lcd-interface-functions-public
 * @fn void drv_LcdInterface_SendCmd(uint8_t data)
 * @brief  Sends one character to the display controller to be displayed on the LCD screen
 * @details 
 *  This function sends a single character via the initialized SPI interface 
 *  to the LCD controller to be displayed on the LCD screen
 **********************************************************************************/

extern void drv_LcdInterface_SendChar(const char c)
{
   LCD_CS_SELECTED();
   LCD_RS_DATA();
   drv_LcdInterface_SpiSend(c);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE FUNCTIONS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*********************************************************************************
 * @ingroup lib-driver-layer-lcd-interface-functions-private
 * @fn void drv_LcdInterface_SpiSend(uint8_t data)
 * @brief  Sends data through the SPI interface to the LCD controller
 * @details 
 *  This function sends one byte via the initialized SPI interface to the
 *  LCD controller.
 **********************************************************************************/

void drv_LcdInterface_SpiSend(uint8_t data)
{
    uint8_t mask;

    for(mask = 0x80; mask != 0; mask >>= 1)
    {
        LCD_SCL_SetLow();
        Nop();
        __delay_us(1);
        if(data & mask)
            LCD_SDI_SetHigh();
        else
            LCD_SDI_SetLow();
        LCD_SCL_SetHigh();
        Nop();
        __delay_us(1);
    }
    LCD_CS_nSELECTED();
    __delay_us(50);
}



// end of file

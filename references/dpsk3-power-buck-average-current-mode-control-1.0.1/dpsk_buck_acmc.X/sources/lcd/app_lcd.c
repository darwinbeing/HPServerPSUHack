/*
 * File:   app_lcd.c
 * Author: M91406
 *
 * Created on March 12, 2020, 12:10 PM
 */

#include "lcd/app_lcd.h"
#include <math.h>

// Additional header files required by this task
#include "config/apps.h"
#include "config/hal.h"

// PRIVATE VARIABLE DELARATIONS

volatile struct LCD_s lcd;      // declare one LCD data object

/***********************************************************************************
 * @ingroup app-layer-lcd-properties-private
 * @{
 * @var lcd_cnt
 * @brief  LCD driver time-base counter 
 * @details
 *  The LCD driver screen update is performed with a fixed frequency generated 
 *  by counting task scheduler ticks. The time-base counter 'lcd_cnt' is 
 *  incremented with every task scheduler call of function appLCD_Execute().
 *  If the time-base counter value matches or exceeds the constant user value
 *  defined by LCD_STARTUP, the startup screen will be switched to the first
 *  default LCD screen and regular screen updates will be performed.
 *  If the time-base counter value matches or exceeds the constant user value
 *  defined by LCD_STARTUP, the screen update will be performed.
 * 
 **********************************************************************************/
volatile uint16_t lcd_cnt = 0;  ///< Local counter used to trigger LCD refresh event

/** @} */ // end of group app-layer-lcd-properties-private

/***********************************************************************************
 * @ingroup app-layer-lcd-properties-private
 * @{
 * @def LCD_STARTUP
 * @brief Startup screen delay compare value
 * @details
 *  Period counter compare value determining how long the startup screen will be shown
 ***********************************************************************************/
#define LCD_STARTUP   30000     ///< Value of 30000 equals a period of 3 seconds

/** @} */ // end of group app-layer-lcd-properties-private

/***********************************************************************************
 * @ingroup app-layer-lcd-properties-private
 * @{
 * @def LCD_REFRESH
 * @brief Screen refresh delay compare value
 * @details
 *  Period counter compare value determining the LCD refresh rate
 ***********************************************************************************/
#define LCD_REFRESH   2000      ///< Value of 30000 equals a period of 200 milliseconds

/** @} */ // end of group app-layer-lcd-properties-private

/***********************************************************************************
 * @ingroup app-layer-lcd-properties-private
 * @{
 * @def LCD_NO_OF_SCREENS
 * @brief Number of screens which can be selected
 * @details
 *  This application supports multiple different screens which can be selected by
 *  the user to display different runtime data fields.
 ***********************************************************************************/
#define LCD_NO_OF_SCREENS   3   ///< Number of screens which can be selected

/** @} */ // end of group app-layer-lcd-properties-private

/*********************************************************************************
 * @ingroup app-layer-lcd-functions-public
 * @fn volatile uint16_t appLCD_Initialize(void)
 * @brief  Initializes the LC display
 * @param  void
 * @return unsigned int (0=failure, 1=success)
 * @details
 *  This function initializes the LC display driver data object and 
 *  loads the startup screen.
 *
 **********************************************************************************/

volatile uint16_t appLCD_Initialize(void) 
{
    volatile uint16_t retval = 1;
    
    if (lcd.refresh == 0)
        lcd.refresh = LCD_STARTUP;
    
    lcd.screens = LCD_NO_OF_SCREENS;

    dev_Lcd_Initialize();
    dev_Lcd_WriteStringXY(0,0,"==== DPSK-3 ====");
    dev_Lcd_WriteStringXY(0,1,"   BUCK  ACMC   ");

    lcd_cnt = 0;
    lcd.enabled = true;
    
    return(retval);
}

/*********************************************************************************
 * @ingroup app-layer-lcd-functions-public
 * @fn volatile uint16_t appLCD_Execute(void)
 * @brief  Refreshes the LC display
 * @param  void
 * @return unsigned int (0=failure, 1=success)
 * @details
 *  This function is frequently called by the task scheduler automatically
 *  updating currently displayed data and/or loading the most recent screen
 *  if a screen switch has been triggered by externally.
 *
 **********************************************************************************/

volatile uint16_t appLCD_Execute(void) 
{
    volatile uint16_t retval = 1;
    volatile float vi=0.0, vo=0.0, isns=0.0, temp=0.0;
    
    // IF LCD output is disabled, exit here
    if(!lcd.enabled)
        return(retval);
    
    // Refresh LCD and reset refresh counter
    lcd_cnt++;
    
    // If REFRESH period has expired, update LCD contents
    if(lcd_cnt == lcd.refresh) {
        
        // Calculate output values
        vi = ((buck.data.v_in << 3) * ADC_GRANULARITY); // Scale ADC value to physical unit
        vi = (float)(int)(100.0 * vi);      // Rounding operation required to prevent display
        vi /= 100.0;                        // rounding issues around 9.99 and 10.0 ° C

        // Input voltage display
        if((double)vi < 10.000)
            PrintLcd(0, "VIN     = %2.2f V", (double)vi);
        else
            PrintLcd(0, "VIN     = %2.1f V", (double)vi);

        switch (lcd.screen)
        {
            case 1:     // Show Temperature Output

				temp = ((float)(buck.data.temp - TEMP_FB_ZERO) / TEMP_FB_SLOPE); // Scale ADC value to physical unit
				temp = (float)(int)(100.0 * temp);  // Rounding operation required to prevent display 
				temp /= 100.0;                      // rounding issues around 9.99 and 10.0 V

                if((double)temp < 10.000)
                    PrintLcd(1, "TEMP    = %2.2f C", (double)temp);
                else
                    PrintLcd(1, "TEMP    = %2.1f C", (double)temp);
                break;
            
            case 2:     // Show Current Output

				isns = ((buck.data.i_out * ADC_GRANULARITY) /  BUCK_ISNS_FEEDBACK_GAIN); // Scale ADC value to physical unit

                if((double)isns < 1.000)
                {
                    isns *= 1000.0;
                    PrintLcd(1, "ISNS    = %3d mA", (int)isns);
                }
                else
                {
                    PrintLcd(1, "ISNS    = %1.2f A", (double)isns);
                }
                break;

            case 3:     // Firmware Version Number
                PrintLcd(1, "Firmware: %s", FIRMWARE_VERSION_STRING);
                break;

            default:    // Output voltage display
                
				vo = ((buck.data.v_out << 1) * ADC_GRANULARITY); // Scale ADC value to physical unit

                if((double)vo < 10.000)
                    PrintLcd(1, "VOUT    = %2.2f V", (double)vo);
                else
                    PrintLcd(1, "VOUT    = %2.1f V", (double)vo);

                break;
        }
        
        // Add Error Indicators
        if ((lcd.screen<3) && (buck.status.bits.fault_active))
        {
            if (fltobj_BuckUVLO.Status.bits.FaultStatus)
                dev_Lcd_WriteStringXY(4, 1, "(UV)");
            else if (fltobj_BuckOVLO.Status.bits.FaultStatus)
                dev_Lcd_WriteStringXY(4, 1, "(OV)");
            else if (fltobj_BuckRegErr.Status.bits.FaultStatus)
                dev_Lcd_WriteStringXY(4, 1, "(RE)");
            else if (fltobj_BuckOCP.Status.bits.FaultStatus)
                dev_Lcd_WriteStringXY(4, 1, "(OC)");
            else 
                dev_Lcd_WriteStringXY(4, 1, "(LA)");
        }

        // Trigger LCD Refresh
        lcd.refresh = LCD_REFRESH;
        lcd_cnt = 0; // Reset internal interval counter
    }
    
    return(retval);
}

/*********************************************************************************
 * @ingroup app-layer-lcd-functions-public
 * @fn volatile uint16_t appLCD_Dispose(void)
 * @brief  Unloads the LC display data object and resources
 * @param  void
 * @return unsigned int (0=failure, 1=success)
 * @details
 *  This function unloads the LC display data object and frees its resources.
 *  The LCD_s data object of this display needs to be reinitialized before 
 *  the LC display can be used again.
 *
 **********************************************************************************/

volatile uint16_t appLCD_Dispose(void) 
{
    volatile uint16_t retval = 1;
    
    /* PLACE DISPOSE CODE HERE */

    return(retval);
}

// end of file

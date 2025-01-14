/*
 * File:   app_push_button.c
 * Author: M91406
 *
 * Created on March 12, 2020, 12:10 PM
 */


#include "config/hal.h"
#include "app_push_button.h" 
#include "lcd/app_lcd.h"

// PRIVATE VARIABLE DELARATIONS

#define PUSH_BUTTON_DEBOUNCE_DELAY_DEFAULT    199    // Push Button needs to be pressed >20ms to trip a PUSH BUTTON switch event
#define PUSH_BUTTON_LONG_PRESS_DELAY_DEFAULT  4999   // Push Button needs to be pressed >500ms to trip a PUSH BUTTON switch event

volatile PUSH_BUTTON_OBJECT_t push_button;

// Push Button Events
volatile uint16_t appPushButton_EventButtonDown(void);
volatile uint16_t appPushButton_EventButtonPressed(void);
volatile uint16_t appPushButton_EventButtonLongPress(void);
volatile uint16_t appPushButton_EventButtonUp(void);

/*********************************************************************************
 * @ingroup app-layer-push-button-functions-public
 * @fn volatile uint16_t appPushButton_Initialize(void)
 * @brief  Initializes the USER push button on DPSK3
 * @param  void
 * @return unsigned integer (0=failure, 1=success)
 * 
 * @details
 *  DPSK3 has one on-board push button labeled 'USER' which is used in this 
 *  application example to switch between different LCD screens allowing the 
 *  user to view runtime data.
 *  
 *  This function initializes the push button function driver used by the 
 *  push button monitoring function appPushButton_Execute() by setting 
 *  user defined delays to debounce the switching edge and set up filters
 *  for short and long press events.
 *
 **********************************************************************************/

volatile uint16_t appPushButton_Initialize(void) 
{
    volatile uint16_t retval = 1;
    
    retval &= drv_PushButton_Initialize(&push_button);   
    
    push_button.debounce_delay = PUSH_BUTTON_DEBOUNCE_DELAY_DEFAULT;
    push_button.long_press_delay = PUSH_BUTTON_LONG_PRESS_DELAY_DEFAULT;
    push_button.status.bits.enabled = true;
    
    push_button.event_btn_down = &appPushButton_EventButtonDown;
    push_button.event_btn_up = &appPushButton_EventButtonUp;
    push_button.event_pressed = NULL; // Event not used
    push_button.event_long_press = &appPushButton_EventButtonLongPress;
    
    return(retval);
}

/*********************************************************************************
 * @ingroup app-layer-push-button-functions-public
 * @fn volatile uint16_t appPushButton_Execute(void)
 * @brief  Executes the USER push button monitor
 * @param  void
 * @return unsigned integer (0=failure, 1=success)
 * 
 * @details
 *  DPSK3 has one on-board push button labeled 'USER' which is used in this 
 *  application example to switch between different LCD screens allowing the 
 *  user to view runtime data.
 *  
 *  This function monitors the defined general purpose I/O to detect the status 
 *  of the on-board push button 'USER', scanning for Long Press events, which 
 *  will trigger the switch-over between different LCD screens.
 *
 **********************************************************************************/

volatile uint16_t appPushButton_Execute(void) 
{
    volatile uint16_t retval = 1;

    retval &= drv_PushButton_Execute(&push_button);

    return(retval);
}

/*********************************************************************************
 * @ingroup app-layer-push-button-functions-public
 * @fn volatile uint16_t appPushButton_Dispose(void)
 * @brief  Unloads the push button data object and frees its resources
 * @param  void
 * @return unsigned integer (0=failure, 1=success)
 * 
 * @details
 *  This function is used to unload all push button function driver data 
 *  objects and free their resources. 
 * 
 *  All user settings will get reset. The PUSH_BUTTON_OBJECT_t data 
 *  object holding all user-defined settings of the push-button object need
 *  to be re-initialized before this function driver can be used again. * 
 **********************************************************************************/

volatile uint16_t appPushButton_Dispose(void) 
{
    volatile uint16_t retval = 1;
    
    retval &= drv_PushButton_Dispose(&push_button);

    return(retval);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE FUNCTIONS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*********************************************************************************
 * @ingroup app-layer-push-button-functions-private
 * @fn volatile uint16_t appPushButton_EventButtonDown(void)
 * @brief  Push button event raised at the "pressed" transition
 * @param  void
 * @return unsigned integer (0=failure, 1=success)
 * 
 * @details
 *  This function is called when the push button detects a transition event from 
 *  'not being pressed' to 'being pressed'. Enabling this interrupt requires to 
 *  declare the function pointer to this function in the push button data object
 *  event_btn_down data field. If this data field remains uninitialized or is set
 *  to NULL, this even is disabled.
 * 
 **********************************************************************************/

volatile uint16_t appPushButton_EventButtonDown(void) 
{
    Nop();
    return(1);
}

/*********************************************************************************
 * @ingroup app-layer-push-button-functions-private
 * @fn volatile uint16_t appPushButton_EventButtonUp(void)
 * @brief  Push button event raised at the "not pressed" transition
 * @param  void
 * @return unsigned int (0=failure, 1=success)
 * 
 * @details
 *  This function is called when the push button detects a transition event from 
 *  'being pressed' to 'not being pressed'. Enabling this interrupt requires to 
 *  declare the function pointer to this function in the push button data object
 *  event_btn_up data field. If this data field remains uninitialized or is set
 *  to NULL, this even is disabled.
 * 
 **********************************************************************************/

volatile uint16_t appPushButton_EventButtonUp(void) 
{
    Nop();
    return(1);
}

/*********************************************************************************
 * @ingroup app-layer-push-button-functions-private
 * @fn volatile uint16_t appPushButton_EventButtonPressed(void)
 * @brief  Push button event raised at the "is pressed" detection
 * @param  void
 * @return unsigned int (0=failure, 1=success)
 * 
 * @details
 *  This function is called after the push button detects a transition event from 
 *  'not being pressed' to 'being pressed'. Enabling this interrupt requires to 
 *  declare the function pointer to this function in the push button data object
 *  event_btn_pressed data field. If this data field remains uninitialized or is set
 *  to NULL, this even is disabled.
 * 
 **********************************************************************************/

volatile uint16_t appPushButton_EventButtonPressed(void)
{
    Nop();
    return(1);
}

/*********************************************************************************
 * @ingroup app-layer-push-button-functions-private
 * @fn volatile uint16_t appPushButton_EventButtonLongPress(void)
 * @brief  Push button event raised at the "is long pressed" detection
 * @param  void
 * @return unsigned int (0=failure, 1=success)
 * 
 * @details
 *  This function is called after the push button event PRESSED has been detected 
 *  and the Long Press Delay has expired without detecting a Button Up event.
 *  Enabling this interrupt requires to declare the function pointer to this 
 *  function in the push button data object event_long_press data field. If this 
 *  data field remains uninitialized or is set to NULL, this even is disabled.
 *
 **********************************************************************************/

volatile uint16_t appPushButton_EventButtonLongPress(void) {
    
    lcd.screen += 1;    // increment screen index
    if (lcd.screen > lcd.screens) // Roll-over after screen #3
        lcd.screen = 0; // Reset to default view
    return(1);
}


// end of file

/*
 * File:   switch.c
 * Author: M91406
 *
 * Created on March 12, 2020, 12:10 PM
 */


#include "push_button/app_push_button.h"
#include "config/hal.h"

// PRIVATE VARIABLE DELARATIONS
// (none)

/*********************************************************************************
 * @ingroup lib-layer-push-button-functions-public
 * @fn volatile uint16_t drv_PushButton_Initialize(volatile struct PUSH_BUTTON_OBJECT_s* pushbtn)
 * @brief  Initializes the push button device driver
 * @param  pushbtn  Push button object of type struct PUSH_BUTTON_OBJECT_s*
 * @return unsigned integer (0=failure, 1=success)
 * 
 * @details
 *  The push button driver offers typical functions of a human-machine interface
 *  push button such as 
 * 
 *   - hardware de-bouncing
 *   - detection of short press
 *   - detection of long press
 *   - push-button status (pressed/unpressed)
 *   - switch event indication
 *
 *  Users of this function driver can define and tune delay to adopt 
 *  detection delays and related responses to the task execution period
 *  and hardware circuit related dependencies.
 * 
 **********************************************************************************/

volatile uint16_t drv_PushButton_Initialize(volatile struct PUSH_BUTTON_OBJECT_s* pushbtn) 
{
    volatile uint16_t retval = 1;
    
    SW_USER_Init();   // Initialize GPIO used to read switch

    // Initializing switch object 
    pushbtn->debounce_delay = 0;              // Clear de-bounce delay
    pushbtn->long_press_delay = 0;            // Clear 'long press" delay
    pushbtn->status.bits.pressed = false;     // Reset PRESSED status
    pushbtn->status.bits.long_press = false;  // Reset LONG_PRESS status
    pushbtn->status.bits.enabled = false;     // Turn off Switch Button
    pushbtn->status.bits.sw_event = false;    // Clear SWITCH EVENT flag 
    
    return(retval);
}

/*********************************************************************************
 * @ingroup lib-layer-push-button-functions-public
 * @fn     volatile uint16_t drv_PushButton_Execute(volatile struct PUSH_BUTTON_OBJECT_s *pushbtn)
 * @brief  Initializes the push button device driver
 * @param  pushbtn  Pointer to push button data object of type struct PUSH_BUTTON_OBJECT_s 
 * @return unsigned int (0=failure, 1=success)
 * 
 * @details
 *  This function has to be called by a task scheduler at a constant frequency.
 *  With ever function call the state of the digital I/O is scanned and monitoring 
 *  timers are updated to detect the most recent status of the push button and 
 *  detect events like short press, long press and raise the respective events.
 * 
 **********************************************************************************/

volatile uint16_t drv_PushButton_Execute(volatile struct PUSH_BUTTON_OBJECT_s* pushbtn) 
{
    volatile uint16_t retval = 1; // Return value
    static uint16_t press_cnt, release_cnt;     // local counters of SWITCH_USER being pressed/released
    static bool pre_pressed = false, pre_long_press = false;
    
    // If switch is disabled, exit here
    if (!pushbtn->status.bits.enabled)
    {
        pushbtn->status.bits.pressed = false; // Mark Switch as RELEASED
        return(1);  // Exit function
    }
    
	// Trigger on a PRESSED event with applied de-bouncing
	if ((!SW_USER_Get()) && (!pushbtn->status.bits.pressed)) { 
		
        // switch button PRESSED event
        if (++press_cnt > pushbtn->debounce_delay) {
            pushbtn->status.bits.pressed = true; // Set PRESSED flag
            if (pushbtn->event_btn_down != NULL)   // Raise Button Event
                pushbtn->event_btn_down();
        }
	}
	// Trigger on a LONG PRESS event with applied de-bouncing
    else if ((!SW_USER_Get()) && (pushbtn->status.bits.pressed)) {

        // switch button LONG PRESS event
        if (++press_cnt > pushbtn->long_press_delay) {
            
            if (!pushbtn->status.bits.long_press)    // Long Press Event is triggered for the first time
            if (pushbtn->event_long_press != NULL)   // Raise Button Long Press Event
                pushbtn->event_long_press();
                
            pushbtn->status.bits.long_press = true;  // Set LONG_PRESS flag
            press_cnt = pushbtn->long_press_delay; // Clamp counter to threshold
            
            if (pushbtn->event_pressed != NULL)   // Raise Button Pressed Event
                pushbtn->event_pressed();
            
        }
        
    }
    // Trigger on a RELEASE event with applied de-bouncing
	else if ((SW_USER_Get()) && (pushbtn->status.bits.pressed)) {  
        
        // switch button RELEASE event
        if (++release_cnt > pushbtn->debounce_delay) {
            if (pushbtn->event_btn_up != NULL)       // Raise Button Event
                pushbtn->event_btn_up();
            pushbtn->status.bits.pressed = false;      // Clear PRESSED flag
            pushbtn->status.bits.long_press = false;   // Clear LONG_PRESS flag
            release_cnt = pushbtn->debounce_delay; // Clamp counter to threshold
        }
    }
    else {
        press_cnt = 0;   // Clear switch de-bounce counter PRESSED
        release_cnt = 0; // Clear switch de-bounce counter RELEASE
    }
    
    // Trigger on switch events
    pushbtn->status.bits.sw_event = (bool) //(pre_pressed != pushbtn->status.pressed) ;
        ((pre_pressed != pushbtn->status.bits.pressed) || (pre_long_press != pushbtn->status.bits.long_press));
    
    pre_pressed = pushbtn->status.bits.pressed;
    pre_long_press = pushbtn->status.bits.long_press;
        
    return(retval);
}

/*********************************************************************************
 * @ingroup lib-layer-push-button-functions-public
 * @fn volatile uint16_t drv_PushButton_Dispose(volatile struct PUSH_BUTTON_OBJECT_s* pushbtn)
 * @brief  Initializes the push button device driver
 * @param  pushbtn  Push button data object of type struct PUSH_BUTTON_OBJECT_s*
 * @return unsigned int (0=failure, 1=success)
 * @details
 *  This function will unload the push-button function driver and free its 
 *  resources. All user settings will get reset. The PUSH_BUTTON_t data 
 *  object holding all user-defined settings of the push-button object need
 *  to be re-initialized before this function driver can be used again.
 * 
 **********************************************************************************/

volatile uint16_t drv_PushButton_Dispose(volatile struct PUSH_BUTTON_OBJECT_s* pushbtn) 
{
    volatile uint16_t retval = 1;
    
    pushbtn->debounce_delay = 0;
    pushbtn->status.bits.pressed = false;
    pushbtn->status.bits.long_press = false;
    pushbtn->status.bits.enabled = false;
    SW_USER_Init();

    return(retval);
}

// end of file

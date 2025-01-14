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
 * File:   led.h
 * Author: M91406
 * Comments: LCD application layer
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SWITCH_BUTTON_DRIVER_H
#define	SWITCH_BUTTON_DRIVER_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

/***********************************************************************************
 * @ingroup lib-layer-push-button-properties-public
 * @enum  SWITCH_STATUS_FLAGS_e
 * @brief Enumeration of push button states
 * @details
 *  This enumeration lists the different conditions of the push button connected 
 *  to the specified GPIO of the device, which can be detecting by this push button 
 *  function driver.
 ***********************************************************************************/

typedef enum SWITCH_STATUS_FLAGS_e{
    SWITCH_STAT_PRESSED    = 0b1100000000000001,
    SWITCH_STAT_LONG_PRESS = 0b1110000000000001,
    SWITCH_STAT_RELEASED   = 0b1000000000000001
} SWITCH_STATUS_FLAGS_t;

/***********************************************************************************
 * @ingroup lib-layer-push-button-properties-public
 * @struct PUSH_BUTTON_STATUS_s
 * @memberof PUSH_BUTTON_OBJECT_s
 * @extends PUSH_BUTTON_OBJECT_s
 * @brief Status word of the push button driver
 * @details
 *  The status word of the push button function driver allows asynchronous 
 *  monitoring of the push button status, encoding the detection of short and 
 *  long press as well as the immediate switch event.
 * 
 *  In addition, an Enable control bit is provided to allow external code modules
 *  to turn the monitoring of the push-button input on or off.
 **********************************************************************************/

typedef struct PUSH_BUTTON_STATUS_s{
    
    union {
    struct{
        volatile bool sw_event :1;  ///< Bit 0:  Event bit indicating a state has changed (cleared automatically)
        volatile unsigned     :1;   ///< Bit 1:  (reserved)
        volatile unsigned     :1;   ///< Bit 2:  (reserved)
        volatile unsigned     :1;   ///< Bit 3:  (reserved)
        volatile unsigned     :1;   ///< Bit 4:  (reserved)
        volatile unsigned     :1;   ///< Bit 5:  (reserved)
        volatile unsigned     :1;   ///< Bit 6:  (reserved)
        volatile unsigned     :1;   ///< Bit 7:  (reserved)
        volatile unsigned     :1;   ///< Bit 8:  (reserved)
        volatile unsigned     :1;   ///< Bit 9:  (reserved)
        volatile unsigned     :1;   ///< Bit 10: (reserved)
        volatile unsigned     :1;   ///< Bit 11: (reserved)
        volatile unsigned     :1;   ///< Bit 12: (reserved)
        volatile bool long_press :1; ///< Bit 13: Indicates if switch has been pressed for a longer time
        volatile bool pressed :1;   ///< Bit 14: Indicates if the button is pressed or not
        volatile bool enabled :1;   ///< Bit 15: Enables/disables the Switch button object
    }__attribute__((packed)) bits;  ///< Status bit field
    volatile uint16_t value;        ///< Status word
    };
    
} PUSH_BUTTON_STATUS_t;             ///< Push Button status

/***********************************************************************************
 * @ingroup lib-layer-push-button-properties-public
 * @struct PUSH_BUTTON_OBJECT_s
 * @brief Push button function driver data object
 * @details
 *  The Push Button function driver data object is used to store the user
 *  configuration of a push-button. Thus, multiple push button objects can be 
 *  defined, which will be individually monitored by the drv_PushButton_Execute
 *  function.
 **********************************************************************************/

// PUBLIC DATA TYPE DECLARATION
typedef struct PUSH_BUTTON_OBJECT_s{
    
    volatile struct PUSH_BUTTON_STATUS_s status; ///< Status word of the switch object
    volatile uint16_t debounce_delay;   ///< Number of call cycles until a switch event is triggered
    volatile uint16_t long_press_delay; ///< Number of call cycles until a "long press" switch event is triggered
    volatile uint16_t (*event_btn_down)(void);   ///< Function pointer to user function triggering a PRESSED event
    volatile uint16_t (*event_long_press)(void); ///< Function pointer to user function triggering a LONG_PRESS event
    volatile uint16_t (*event_pressed)(void);    ///< Function pointer to user function triggering a LONG_PRESS event
    volatile uint16_t (*event_btn_up)(void);    ///< Function pointer to user function triggering a RELEASE event

}PUSH_BUTTON_OBJECT_t;  ///< Push Button data object
    

// PUBLIC FUNCTION PROTOTYPE DECLARATION
extern volatile uint16_t drv_PushButton_Initialize(volatile struct PUSH_BUTTON_OBJECT_s* pushbtn);
extern volatile uint16_t drv_PushButton_Execute(volatile struct PUSH_BUTTON_OBJECT_s* pushbtn);
extern volatile uint16_t drv_PushButton_Dispose(volatile struct PUSH_BUTTON_OBJECT_s* pushbtn);

#endif	/* SWITCH_BUTTON_DRIVER_H */


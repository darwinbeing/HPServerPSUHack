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
 * File:   app_power_control.h
 * Author: M91406
 * Comments: Power control application layer
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef APPLICATION_LAYER_POWER_CONTROL_H
#define	APPLICATION_LAYER_POWER_CONTROL_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

#include "power_control/devices/dev_buck_typedef.h"

// PUBLIC DATA OBJECT DECLARATIONS
extern volatile struct BUCK_CONVERTER_s buck;

// PUBLIC FUNCTION PROTOTYPE DECLARATIONS
extern volatile uint16_t appPowerSupply_Initialize(void);
extern volatile uint16_t appPowerSupply_Execute(void);
extern volatile uint16_t appPowerSupply_Start(void);
extern volatile uint16_t appPowerSupply_Stop(void);
extern volatile uint16_t appPowerSupply_Suspend(void);
extern volatile uint16_t appPowerSupply_Resume(void);


// CONTROL LOOP USER EXTENSION FUNCTIONS

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * PowerSmart DCLD allows users to create and call user extension 
 * functions from specific locations of the main control loop to 
 * cover design-specific requirements and features which are not
 * supported by the main controller by default.
 * 
 * Control Loop User Extension Declaration Example:
 * 
 *    buck.v_loop.controller->ExtensionHooks.ptrExtHookStartFunction = (uint16_t)&my_function; 
 * 
 * Please refer to the PowerSmart DCLD User Guide for more details about
 * how to use this feature, its requirements and limitations.
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
// extern void __attribute__((near)) my_function(void); // Control Loop User Extension Example


//#else
//  #pragma message "app_power_control.h inclusion bypassed"
#endif	/* APPLICATION_LAYER_POWER_CONTROL_H */


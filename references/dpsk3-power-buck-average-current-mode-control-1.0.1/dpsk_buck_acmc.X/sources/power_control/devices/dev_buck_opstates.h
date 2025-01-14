/**
 *  (c) 2020 Microchip Technology Inc. and its subsidiaries.
 *
 *  Subject to your compliance with these terms, you may use Microchip software
 *  and any derivatives exclusively with Microchip products. You're responsible
 *  for complying with 3rd party license terms applicable to your use of 3rd
 *  party software (including open source software) that may accompany Microchip
 *  software.
 *
 *  SOFTWARE IS "AS IS." NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 *  APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 *  MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 *  INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 *  WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
 *  HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
 *  THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
 *  CLAIMS RELATED TO THE SOFTWARE WILL NOT EXCEED AMOUNT OF FEES, IF ANY,
 *  YOU PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 *  @file    dev_buck_opstates.h
 *  @brief   This file contains APIs to for the power control
 *
 *  @note
 *  Microchip Technology Inc. has followed development methods required by
 *  IEC-60730 and performed extensive validation and static testing to ensure
 *  that the code operates as intended. Any modification to the code can
 *  invalidate the results of Microchip's validation and testing.
 *
 */
/* 
 * File:   dev_buck_opstates.h
 * Author: M91406
 * Comments: Buck converter operation states header file
 * Revision history: 
 *   10/09/20   1.0  initial version
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BUCK_CONVERTER_OPERATION_STATES_H
#define	BUCK_CONVERTER_OPERATION_STATES_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types

/*******************************************************************************
 * @var	volatile uint16_t(*BuckConverterStateMachine[])(volatile struct BUCK_CONVERTER_s *buckInstance) 
 * @ingroup lib-layer-buck-state-machine-properties-variables
 * @brief   Function pointer array defining the state machine execution sequence	
 * @details
 * The function pointer list BuckConverterStateMachine[] is providing 
 * access to a list of state functions. Each function mapped into this array 
 * as function pointer represents a self-contained, independent sub-state. 
 * The main state machine will call functions from this list in order of their
 * index within the array, starting from '0'. While this list is executed, the 
 * calling main state is transparent by passing through the return value of the 
 * most recent sub-state. Once all sub-states have been executed, the calling main 
 * state will return the COMPLETE flag, allowing the main state machine to move on.
 * 
 * Each function needs to be called by handing over a parameter of type
 * 
 * - struct BUCK_CONVERTER_s 
 * 
 * Each function returns of type unsigned integer:
 * 
 * - 0 = BUCK_OPSRET_ERROR
 * - 1 = BUCK_OPSRET_COMPLETE
 * - 2 = BUCK_OPSRET_REPEAT
 * 
 **********************************************************************************/

extern volatile uint16_t (*BuckConverterStateMachine[])
            (volatile struct BUCK_CONVERTER_s *buckInstance); ///< Buck converter state machine function pointer array

/*******************************************************************************
 * @var	    volatile uint16_t BuckStateList_size
 * @ingroup lib-layer-buck-state-machine-properties-variables 
 * @brief   Buck converter state machine function pointer array size	
 *********************************************************************************/

extern volatile uint16_t BuckStateList_size; ///< Buck converter state machine function pointer array size


#endif	/* BUCK_CONVERTER_OPERATION_STATES_H */

// END OF FILE

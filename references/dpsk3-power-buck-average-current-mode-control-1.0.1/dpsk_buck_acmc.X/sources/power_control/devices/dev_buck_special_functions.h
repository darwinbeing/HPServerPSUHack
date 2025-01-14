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
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BUCK_CONVERTER_SPECIAL_FUNCTIONS_H
#define	BUCK_CONVERTER_SPECIAL_FUNCTIONS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

/****************************************************************************************************
 * @ingroup lib-layer-buck-specialfn-properties-data-types
 * @enum    BUCK_SPECIAL_FUNCTIONS_e
 * @brief   Enumeration of special function sub-states
 * @details 
 *  This enumeration provides labels for indices of the private array BuckConverterSpecialFunctions[].
 *  External functions call special functions by calling the high level function 
 *  drv_BuckConverter_SpecialFunctionExecute() by handing in a pointer to a power converter object
 *  and the Special Function state to execute. This allows the extension of state machine to include 
 *  Special Functions available.
 **************************************************************************************************** */
typedef enum BUCK_SPECIAL_FUNCTIONS_e 
{
    CS_OFSET_CALIBRATION    ///< Special Function Index of Current Sense Feedback Offset Calibration
        
} BUCK_SPECIAL_FUNCTIONS_t; // Enumeration of special function sub-states


/* PUBLIC FUNCTION CALL PROTOTYPES */

extern volatile uint16_t drv_BuckConverter_SpecialFunctionExecute (
                volatile struct BUCK_CONVERTER_s * buckInstance, 
                volatile enum BUCK_SPECIAL_FUNCTIONS_e specialFunction);

#endif	/* BUCK_CONVERTER_SPECIAL_FUNCTIONS_H */

// end of file

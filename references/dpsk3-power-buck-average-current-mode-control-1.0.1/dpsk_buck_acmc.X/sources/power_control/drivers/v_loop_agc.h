/* *********************************************************************************
 * PowerSmart™ Digital Control Library Designer, Version 0.9.12.672
 * *********************************************************************************
 * 4p4z controller function declarations and compensation filter coefficients
 * derived for following operating conditions:
 * *********************************************************************************
 *
 *  Controller Type:    4P4Z - Advanced High-Q Compensator
 *  Sampling Frequency: 500000 Hz
 *  Fixed Point Format: Q15
 *  Scaling Mode:       3 - Dual Bit-Shift Scaling
 *  Input Gain:         0.5
 *
 * *********************************************************************************
 * CGS Version:         3.0.6
 * CGS Date:            02/03/2021
 * *********************************************************************************
 * User:                M91406
 * Date/Time:           02/24/2021 00:14:41
 * ********************************************************************************/

/**
@file v_loop_agc.h
@brief AGC observer function call declarations
*/

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef __SPECIAL_FUNCTION_LAYER_V_LOOP_AGC_H__
#define __SPECIAL_FUNCTION_LAYER_V_LOOP_AGC_H__

#include <xc.h>                                   // include processor files - each processor file is guarded
#include <dsp.h>                                  // include DSP data types (e.g. fractional)
#include <stdint.h>                               // include standard integer number data types
#include <stdbool.h>                              // include standard boolean data types (true/false)

#include "npnz16b.h"                              // include NPNZ library header file

// ADVANCED CONTROL FUNCTION DECLARATIONS

/*********************************************************************************
 * @fn void v_loop_AGCFactorUpdate(volatile struct NPNZ16b_s* controller)
 * @ingroup special-function-layer-npnz16-functions
 * @{
 * @brief Prototype of the Assembly Adaptive Gain Control Observer routine helping to call the v_loop AGC observer from C-code
 * @param controller: Pointer to NPNZ16b data object of type struct NPNZ16b_s*
 *
 * @details
 * This function is the Adaptive Gain Control Observer extension function which can be included
 * in the main feedback control loop by referencing a pointer to this function in the NPNZ16b
 * data structure of the main control loop using the following configuration example:
 * 
 * @code {.c}
 *      v_loop.GainControl.ptrAgcObserverFunction = (uint16_t)&v_loop_AGCFactorUpdate;
 * @endcode
 * 
 * This function calculates the most recent B-term modulation factor k_agc based on the ratio of 
 * the voltage applied across the main inductor under nominal operating conditions and the instantaneous 
 * voltage across the inductor. This ratio will decrease the loop gain when the voltage across the inductor
 * increases and will increase the loop gain when the voltage across the inductor drops below nominal 
 * operating conditions. This approach effectively compensates gain variations of the plant stabilizing
 * the frequency response of the converter at the settings made at the nominal operating point.
 * 
 * @note
 * Available control options depend on the controller feature configuration.
 * The Adaptive Gain Control feature is enabled using the Advanced Control Options 
 * in PowerSmart Digital Control LIbrary Designer (PS-DCLD.
 * 
 * Please refer to the PS-DCLD user guide for more detailed information.
 ********************************************************************************/
extern void __attribute__((near))v_loop_AGCFactorUpdate( // Calls the AGC Factor Modulation Observer Function (Assembly)
		volatile NPNZ16b_t* controller 			  		 // Pointer to nPnZ data type object
    );                                            

 /** @} */ // End of function declaration

#endif	/* __SPECIAL_FUNCTION_LAYER_V_LOOP_AGC_H__ */


/* *********************************************************************************
 * PowerSmart™ Digital Control Library Designer, Version 0.9.14.676
 * *********************************************************************************
 * 3p3z controller function declarations and compensation filter coefficients
 * derived for following operating conditions:
 * *********************************************************************************
 *
 *  Controller Type:    3P3Z - Basic Voltage Mode Compensator
 *  Sampling Frequency: 500000 Hz
 *  Fixed Point Format: Q15
 *  Scaling Mode:       3 - Dual Bit-Shift Scaling
 *  Input Gain:         0.5
 *
 * *********************************************************************************
 * CGS Version:         3.0.8
 * CGS Date:            03/12/2021
 * *********************************************************************************
 * User:                M91406
 * Date/Time:           04/02/2021 00:59:29
 * ********************************************************************************/

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef __SPECIAL_FUNCTION_LAYER_V_LOOP_H__
#define __SPECIAL_FUNCTION_LAYER_V_LOOP_H__

#include <xc.h>                                   // include processor files - each processor file is guarded
#include <dsp.h>                                  // include DSP data types (e.g. fractional)
#include <stdint.h>                               // include standard integer number data types
#include <stdbool.h>                              // include standard boolean data types (true/false)

#include "npnz16b.h"                              // include NPNZ library header file

/* *******************************************************************************
 * Data Arrays:
 * The NPNZ16b_s data structure contains pointers to coefficient, control and error
 * history arrays. The pointer target objects (variables and arrays) are defined
 * in controller source file v_loop.c
 *
 * Type definitions for A- and B- coefficient arrays as well as error- and control
 * history arrays are aligned in memory using the 'packed' attribute for optimized
 * addressing during DSP computations. These aligned data structures need to be
 * placed in specific memory locations to allow direct X/Y-access from the DSP.
 * This X/Y-memory placement is covered by the declarations used in controller
 * source file v_loop.c
 * ******************************************************************************/

/*********************************************************************************
 * @ingroup special-function-layer-npnz16-data-objects
 * @struct  V_LOOP_CONTROL_LOOP_COEFFICIENTS_s
 * @brief   Data structure packing A- and B- coefficient arrays in a linear memory space for optimized DSP code execution
 ********************************************************************************/
struct V_LOOP_CONTROL_LOOP_COEFFICIENTS_s
{
    volatile int32_t ACoefficients[3];            // A-Coefficients
    volatile int32_t BCoefficients[4];            // B-Coefficients
} __attribute__((packed));                        // Data structure packing A- and B- coefficient arrays in a linear memory space for optimized DSP code execution
typedef struct V_LOOP_CONTROL_LOOP_COEFFICIENTS_s V_LOOP_CONTROL_LOOP_COEFFICIENTS_t;

/*********************************************************************************
 * @ingroup special-function-layer-npnz16-data-objects
 * @struct  V_LOOP_CONTROL_LOOP_HISTORIES_s
 * @brief   Data structure packing A- and B- coefficient arrays in a linear memory space for optimized DSP code execution
 ********************************************************************************/
struct V_LOOP_CONTROL_LOOP_HISTORIES_s
{
    volatile fractional ControlHistory[3];        // Control History Array
    volatile fractional ErrorHistory[4];          // Error History Array
} __attribute__((packed));                        // Data structure packing control and error histories arrays in a linear memory space for optimized DSP code execution
typedef struct V_LOOP_CONTROL_LOOP_HISTORIES_s V_LOOP_CONTROL_LOOP_HISTORIES_t; // Data type of data structure packing control and error histories arrays

/*********************************************************************************
 * @ingroup special-function-layer-npnz16-variables
 * @var     v_loop_pterm_factor
 * @brief   Q15 fractional of the P-Term Coefficient for Plant Measurements
 ********************************************************************************/
extern volatile int16_t v_loop_pterm_factor;      // Q15 fractional of the P-Term factor

/*********************************************************************************
 * @ingroup special-function-layer-npnz16-variables
 * @var     v_loop_pterm_scaler
 * @brief   Bit-shift scaler of the P-Term Coefficient for Plant Measurements
 ********************************************************************************/
extern volatile int16_t v_loop_pterm_scaler;      // Bit-shift scaler of the P-Term factor


/*********************************************************************************
 * @ingroup special-function-layer-npnz16-objects
 * @var     v_loop
 * @brief   External reference to user-defined NPNZ16b controller data object 'v_loop'
 ********************************************************************************/
extern volatile struct NPNZ16b_s v_loop;          // user-controller data object


/* *******************************************************************************
 * Function call prototypes for initialization routines and control loop handling
 * ******************************************************************************/

/*********************************************************************************
 * @fn volatile uint16_t v_loop_Initialize(volatile struct NPNZ16b_s* controller)
 * @ingroup special-function-layer-npnz16-functions
 * @brief Initializes controller coefficient arrays and normalization factors
 * @param controller: Pointer to NPNZ Controller Data Object of type struct NPNZ16b_s
 *
 * @details
 * This function needs to be called from user code at startup once to initialize
 * coefficient arrays and number normalization settings of the v_loop controller
 * object.
 *
 * @attention
 * This routine DOES NOT initialize the complete controller object.
 * User-defined settings such as pointers to the control reference, source and
 * target registers, output minima and maxima and further, design-dependent
 * settings, need to be specified in user code.
 ********************************************************************************/
extern volatile uint16_t v_loop_Initialize(       // v_loop initialization function call
        volatile struct NPNZ16b_s* controller     // Pointer to NPNZ16b data object
    );

/*********************************************************************************
 * @fn void v_loop_Reset(volatile struct NPNZ16b_s* controller)
 * @ingroup special-function-layer-npnz16-functions
 * @brief Prototype of the Assembly routine '_v_loop_Reset' clearing the NPNZ16b controller output and error histories
 * @param controller: Pointer to NPNZ16b data object of type struct NPNZ16b_s*
 *
 * @details
 * This Assembly function clears the NPNZ16b controller output and
 * error histories by settings all elements of the delay lines to zero. This
 * resets the controller to its default state. This function should be called
 * every time before the control loop is started from a disabled, unbiased output.
 * @note
 * Use function 'v_loop_Precharge' to start the feedback loop controller when
 * the output is pre-biased.
 ********************************************************************************/
extern void v_loop_Reset(                         // v_loop reset function call (Assembly)
        volatile struct NPNZ16b_s* controller     // Pointer to NPNZ16b data object
    );

/*********************************************************************************
 * @fn void v_loop_Precharge(volatile struct NPNZ16b_s* controller, volatile fractional ctrl_input, volatile fractional ctrl_output)
 * @ingroup special-function-layer-npnz16-functions
 * @brief Prototype of the Assembly routine '_v_loop_Precharge' loading user-defined values into the NPNZ16b  controller output and error histories
 * @param controller: Pointer to NPNZ16b data object of type struct NPNZ16b_s*
 * @param ctrl_input: user-defined, constant error history value of type fractional
 * @param ctrl_output: user-defined, constant control output history value of type fractional
 *
 * @details
 * This function loads user-defined values into NPNZ16b controller
 * output and error histories where the parameters ctrl_input and ctrl_output
 * will written to the entire delay line of the filter emulating a pre-existing
 * steady state operation under the user defined conditions.
 ********************************************************************************/
extern void v_loop_Precharge(                     // v_loop history pre-charge function call (Assembly)
        volatile struct NPNZ16b_s* controller,    // Pointer to NPNZ16b data object
        volatile fractional ctrl_input,           // user-defined, constant error history value
        volatile fractional ctrl_output           // user-defined, constant control output history value
    );

/*********************************************************************************
 * @fn void v_loop_Update(volatile struct NPNZ16b_s* controller)
 * @ingroup special-function-layer-npnz16-functions
 * @brief Prototype of the Assembly feedback control loop routine helping to call the v_loop controller from C-code
 * @param controller: Pointer to NPNZ16b data object of type struct NPNZ16b_s*
 *
 * @details
 * This function is the main controller function which must be called at
 * the control frequency from the control interrupt service routine. It
 * calculates the most recent control error and processes it in the compensation
 * filter computation after which the new result is written to the control
 * output target.
 * Runtime control is provided through the NPNZ16b data object status & control word.
 * @note
 * Available control options depend on the controller feature configuration.
 * Please refer to the user guide for more detailed information.
 ********************************************************************************/
extern void v_loop_Update(                        // Calls the 3P3Z controller (Assembly)
        volatile struct NPNZ16b_s* controller     // Pointer to NPNZ16b data object
    );

/*********************************************************************************
 * @fn void v_loop_PTermUpdate(volatile struct NPNZ16b_s* controller)
 * @ingroup special-function-layer-npnz16-functions
 * @brief Prototype of the alternate Assembly P-Term control loop helping to call the v_loop P-Term controller from C-code
 * @param controller: Pointer to NPNZ16b data object of type struct NPNZ16b_s*
 *
 * @details
 * This function has been added as extension function supporting the development
 * process, introducing an alternative control loop able to replace the conventional
 * compensation feedback loop with a simple P-Term controller, allowing users to
 * perform measurements of the plant transfer function.
 * To use this function, users may replace the default compensation feedback loop
 * function call v_loop_Update by v_loop_PTermUpdate to perform the measurement.
 *
 * This additional controller seamlessly fits into the controller data
 * interface by using the same NPNZ16b_s data structure and does not need
 * additional configuration or initialization.
 *
 * @attention
 * THIS CONTROLLER IS USED FOR MEASUREMENTS OF THE PLANT TRANSFER FUNCTION ONLY.
 * THIS LOOP IS BY DEFAULT UNSTABLE AND ONLY WORKS UNDER STABLE TEST CONDITIONS.
 * DO NOT USE THIS CONTROLLER TYPE FOR NORMAL OPERATION
 ********************************************************************************/
extern void v_loop_PTermUpdate(                   // Calls the P-Term controller (Assembly)
        volatile struct NPNZ16b_s* controller     // Pointer to NPNZ16b data object
    );

#endif                                            // end of __SPECIAL_FUNCTION_LAYER_V_LOOP_H__


//**********************************************************************************
// Download latest version of this tool here: https://areiter128.github.io/DCLD
//**********************************************************************************


/*
 * File:   dev_buck_opstates.c
 * Author: M91406
 * Created on October 9, 2020, 9:16 AM
 */


#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types

#include "dev_buck_pconfig.h" // include buck converter
#include "dev_buck_typedef.h" // include buck converter data object declarations
#include "dev_buck_substates.h" // include state machine sub-state declarations
#include "dev_buck_special_functions.h" // include buck converter special function declarations


// Private function prototypes of state functions

volatile uint16_t __attribute__((always_inline)) State_Error(volatile struct BUCK_CONVERTER_s *buckInstance);
volatile uint16_t __attribute__((always_inline)) State_Initialize(volatile struct BUCK_CONVERTER_s *buckInstance);
volatile uint16_t __attribute__((always_inline)) State_Reset(volatile struct BUCK_CONVERTER_s *buckInstance);
volatile uint16_t __attribute__((always_inline)) State_Standby(volatile struct BUCK_CONVERTER_s *buckInstance);
volatile uint16_t __attribute__((always_inline)) State_RampUp(volatile struct BUCK_CONVERTER_s *buckInstance);
volatile uint16_t __attribute__((always_inline)) State_Online(volatile struct BUCK_CONVERTER_s *buckInstance);

// Declaration of function pointer array listing op-state functions in order of execution
volatile uint16_t (*BuckConverterStateMachine[])(volatile struct BUCK_CONVERTER_s *buckInstance) = 
{
    State_Error,        ///< State #0: That's the blank "undefined default state", causing the state machine to reset
    State_Initialize,   ///< State #1: Initialize state machine by resetting all runtime flags to default
    State_Reset,        ///< State #2: Reset key runtime flags when power converter was already turned on
    State_Standby,      ///< State #3: After successful initialization, power converter waits to be launched
    State_RampUp,       ///< State #4: Topology-specific startup sub-states are handled in Ramp-Up function
    State_Online        ///< State #5: During normal operation the converter responds to changes in reference
};

// Declaration variable capturing the size of the sub-state function pointer array 
volatile uint16_t BuckStateList_size = (sizeof(BuckConverterStateMachine)/sizeof(BuckConverterStateMachine[0])); 

/*******************************************************************************
 * @fn uint16_t State_Initialize(volatile struct BUCK_CONVERTER_s *buckInstance)
 * @ingroup lib-layer-buck-state-machine-functions
 * @brief   This function resets the counters and conditional flag bits.
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 *
 * @details
 * If the controller has not been run yet, the POWER ON and POWER GOOD delay
 * counters are reset and all conditional flag bits are cleared. Status of 
 * power source, ADC and current sensor calibration have to be set during
 * runtime by system check routines. 
 *********************************************************************************/
volatile uint16_t State_Initialize(volatile struct BUCK_CONVERTER_s *buckInstance)
{
    volatile uint16_t _i=0;
    
    buckInstance->startup.power_on_delay.counter = 0;   // Reset power on counter
    buckInstance->startup.power_good_delay.counter = 0; // Reset power good counter

    // Reset all status bits
    buckInstance->status.bits.adc_active = false;

    // Initiate current sensor calibration flag bit
    if (buckInstance->set_values.control_mode == BUCK_CONTROL_MODE_VMC)
        buckInstance->status.bits.cs_calib_complete = true; 
    else if (buckInstance->status.bits.cs_calib_enable)
        buckInstance->status.bits.cs_calib_complete = false; 
    else 
        buckInstance->status.bits.cs_calib_complete = true; 
    
    // Disable control loops
    buckInstance->v_loop.controller->status.bits.enabled = false; // Disable voltage loop
    
    if (buckInstance->set_values.control_mode == BUCK_CONTROL_MODE_ACMC) // In current mode...
    {
        for (_i=0; _i<buckInstance->set_values.no_of_phases; _i++) {
            buckInstance->i_loop[_i].controller->status.bits.enabled = false; // Disable current loop
        }        
    }
    
    // Clear busy bit
    buckInstance->status.bits.busy = false; // Clear BUSY bit
    buckInstance->status.bits.ready = true; // Set READY bit indicating state machine has passed INITIALIZED state
    
    // Transition to STATE_RESET
    return(BUCK_OPSRET_COMPLETE); 
    
}

/*******************************************************************************
 * @fn uint16_t State_Reset(volatile struct BUCK_CONVERTER_s *buckInstance)
 * @ingroup lib-layer-buck-state-machine-functions
 * @brief   This function resets the buck control operation by re-initiating the control mode,
 * references and status bits. 
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 * 
 * @details
 * After successful initialization or after an externally triggered state machine reset,
 * the state machine returns to this RESET mode, re-initiating control mode, references 
 * and status bits before switching further into STANDBY mode. 
 *********************************************************************************/
volatile uint16_t State_Reset(volatile struct BUCK_CONVERTER_s *buckInstance)
{
    volatile uint16_t retval=1;
    volatile uint16_t _i=0;

    // Disable all PWM outputs & control loops (immediate power cut-off)
    retval &= buckPWM_Suspend(buckInstance); // Disable PWM outputs
    
    // Disable voltage loop controller and reset control loop histories
    buckInstance->v_loop.controller->status.bits.enabled = false; // disable voltage control loop
    buckInstance->v_loop.ctrl_Reset(buckInstance->v_loop.controller); // Reset control histories of outer voltage controller
    *buckInstance->v_loop.controller->Ports.Target.ptrAddress = 
        buckInstance->v_loop.controller->Limits.MinOutput;

    // Disable current loop controller and reset control loop histories
    if (buckInstance->set_values.control_mode == BUCK_CONTROL_MODE_ACMC) 
    {   // Disable all current control loops and reset control loop histories
        for (_i=0; _i<buckInstance->set_values.no_of_phases; _i++) {
            buckInstance->i_loop[_i].controller->status.bits.enabled = false; 
            buckInstance->i_loop[_i].ctrl_Reset(buckInstance->i_loop[_i].controller); 
            *buckInstance->i_loop[_i].controller->Ports.Target.ptrAddress = 
                buckInstance->i_loop[_i].controller->Limits.MinOutput;
        }
    }

    // Reset the bulk voltage settling counters
    buckInstance->startup.power_on_delay.counter = 0; // Clear Power On Delay counter
    buckInstance->startup.power_good_delay.counter = 0; // Clear Power Good Delay counter

    // Reset all status bits
    buckInstance->status.bits.adc_active = false;
    buckInstance->status.bits.busy = false; // Clear BUSY bit
    
    // If any sub-function call went unsuccessful, reset state machine
    // else, move on to next state
    
    if (retval)
        return(BUCK_OPSRET_COMPLETE);
    else
        return(BUCK_OPSRET_ERROR);
    
}
                
/*******************************************************************************
 * @fn uint16_t State_Standby(volatile struct BUCK_CONVERTER_s *buckInstance)
 * @ingroup lib-layer-buck-state-machine-functions
 * @brief   This function waits until all start-up conditions are met.
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 * 
 * @details
 * After a successful state machine reset, the state machine waits in  
 * STANDBY mode until all conditional flag bits are set/cleared allowing  
 * the converter to run.
 *********************************************************************************/
volatile uint16_t State_Standby(volatile struct BUCK_CONVERTER_s *buckInstance)
{
    volatile uint16_t retval=0;

    // if the 'autorun' option is set, automatically set the GO bit when the 
    // converter is enabled
    if ((buckInstance->status.bits.enabled) && (buckInstance->status.bits.autorun))
    { buckInstance->status.bits.GO = true; }

    // If current sense feedback offset calibration is enabled, 
    // call calibration sub-state routine
    retval = drv_BuckConverter_SpecialFunctionExecute(buckInstance, CS_OFSET_CALIBRATION);
        
    switch (retval)
    {
        case BUCK_OPSRET_COMPLETE:
            buckInstance->status.bits.cs_calib_complete = true; // Set CS Calibration Flag to COMPLETE
            buckInstance->status.bits.busy = false; // Clear BUSY bit indicating on-going activity
            break;
        case BUCK_OPSRET_REPEAT:
            buckInstance->status.bits.cs_calib_complete = false; // Set CS Calibration Flag to COMPLETE
            buckInstance->status.bits.busy = true; // Set BUSY bit indicating on-going activity
            return(BUCK_OPSRET_REPEAT);
            break;
        default:
            buckInstance->status.bits.cs_calib_complete = false; // Set CS Calibration Flag to COMPLETE
            buckInstance->status.bits.busy = false; // Clear BUSY bit indicating on-going activity
            return(BUCK_OPSRET_ERROR);
            break;
    }

    // Wait for all startup conditions to be met
    if ((buckInstance->status.bits.enabled) &&          // state machine needs to be enabled
        (buckInstance->status.bits.GO) &&               // GO-bit needs to be set
        (buckInstance->status.bits.adc_active) &&       // ADC needs to be running
        (buckInstance->status.bits.pwm_active) &&       // PWM needs to be running 
        (!buckInstance->status.bits.fault_active) &&    // No active fault is present
        (!buckInstance->status.bits.suspend) &&         // Power supply is not held in suspend mode
        (buckInstance->status.bits.cs_calib_complete)   // Current Sensor Calibration complete
        )
    {
        buckInstance->status.bits.GO = false;
        return(BUCK_OPSRET_COMPLETE);
    }
    else
    // Remain in current state until bit-test becomes true
    {
        return(BUCK_OPSRET_REPEAT);
    }
    
}

/*******************************************************************************
 * @fn uint16_t State_RampUp(volatile struct BUCK_CONVERTER_s *buckInstance)
 * @ingroup lib-layer-buck-state-machine-functions
 * @brief   This function ramps up the voltage/input to its nominal value
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  0 = BUCK_OPSRET_REPEAT
 * @return  1 = BUCK_OPSRET_COMPLETE
 * @return  2 = BUCK_OPSRET_REPEAT
 *
 * @details
 * After a successful state machine Standby, the state machine executes the RAMPUP 
 * mode. In this mode, the voltage/current ramps up to the nominal value.  
 *********************************************************************************/

volatile uint16_t State_RampUp(volatile struct BUCK_CONVERTER_s *buckInstance)
{
    volatile uint16_t retval=0;
    
    // If sub-state pointer index is out of range, reset to ZERO
    if ((uint16_t)(buckInstance->state_id.bits.substate_id) >= (uint16_t)(BuckRampUpSubStateList_size))
        buckInstance->state_id.bits.substate_id = 0;
    
    // If selected sub-state index contains a NULL-pointer, exit here
    if (BuckConverterRampUpSubStateMachine[buckInstance->state_id.bits.substate_id] == NULL)
        return(BUCK_OPSRET_ERROR);
    
    // Execute ramp-up sub-state
    retval = BuckConverterRampUpSubStateMachine[buckInstance->state_id.bits.substate_id](buckInstance);
    
    // Validate sub-state function return
    switch (retval) 
    {   
        // If the sub-state returns a REPEAT, leave the sub-state pointer at the recent 
        // index and return REPEAT to the outer state machine to call State_RampUp again
        case BUCK_OPSRET_REPEAT:
            
            retval = BUCK_OPSRET_REPEAT;
            
            break;

        // If the sub-state returns a COMPLETE, increment the sub-state pointer to call the 
        // next sub-state next time and return REPEAT to the outer state machine to call
        // State_RampUp again
        case BUCK_OPSRET_COMPLETE:

            // Increment sub-state pointer by one tick
            buckInstance->state_id.bits.substate_id++;
            
            // CHeck if pointer is out of range
            if ((uint16_t)(buckInstance->state_id.bits.substate_id) < (uint16_t)(BuckRampUpSubStateList_size))
            { // if execution list is not complete yet, return op-state as REPEAT
                retval = BUCK_OPSRET_REPEAT;

                if (buckInstance->state_id.bits.substate_id == BUCK_OPSTATE_I_RAMP_UP)
                { // if the next state is set to "I Ramp-Up", check if converter supports
                  // this sub-state in its current configuration and step over it if not

                    if (buckInstance->set_values.control_mode == BUCK_CONTROL_MODE_VMC)
                        buckInstance->state_id.bits.substate_id++; // Increment sub-state pointer
                    
                }

            }
            else
            {
            // if last item of execution list is complete. return op-state as COMPLETE
                retval = BUCK_OPSRET_COMPLETE;
            }
            
            break;

        // If the sub-state returns any other value, something went wrong. In this case
        // return ERROR to outer state machine triggering a main state machine reset.
        default:
            // if any other return value is received, switch immediately to ERROR
            retval = BUCK_OPSRET_ERROR;        
            
            break;
    }
    
    return(retval);
}

/*******************************************************************************
 * @fn uint16_t State_Online(volatile struct BUCK_CONVERTER_s *buckInstance)
 * @ingroup lib-layer-buck-state-machine-functions
 * @brief   This function tunes the controller reference to the new user control reference level.
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 * 
 * @details
 * After soft-start and when state POWER_GOOD_DELAY has expired, the converter 
 * enters normal operation.
 * 
 * During normal operation the state machine scans the user reference setting. 
 * Once a change between the user reference setting and the most recent controller 
 * reference has been detected, the state machine will tune the controller 
 * reference to the new user control reference level in incremental steps,
 * applying the same ramp slope as during soft-start.
 * 
 * While ramping the output voltage up or down, the BUSY bit will be set and any 
 * new changes to the reference will be ignored until the ramp up/down is complete.
 *********************************************************************************/

volatile uint16_t State_Online(volatile struct BUCK_CONVERTER_s *buckInstance)
{
    if(buckInstance->set_values.v_ref != buckInstance->v_loop.reference) 
    {
        // Set the BUSY bit indicating a delay/ramp period being executed
        buckInstance->status.bits.busy = true;

        // New reference value is less than controller reference value => ramp down
        if(buckInstance->set_values.v_ref < buckInstance->v_loop.reference){
            // decrement reference until new reference level is reached
            buckInstance->v_loop.reference -= buckInstance->startup.v_ramp.ref_inc_step; // decrement reference
            if(buckInstance->v_loop.reference < buckInstance->set_values.v_ref) { // check if ramp is complete
                buckInstance->v_loop.reference = buckInstance->set_values.v_ref; // clamp reference level to setting
            }

        }
        // New reference value is greater than controller reference value => ramp up
        else if(buckInstance->set_values.v_ref > buckInstance->v_loop.reference){
            // increment reference until new reference level is reached
            buckInstance->v_loop.reference += buckInstance->startup.v_ramp.ref_inc_step; // increment reference
            if(buckInstance->v_loop.reference > buckInstance->set_values.v_ref) { // check if ramp is complete
                buckInstance->v_loop.reference = buckInstance->set_values.v_ref; // clamp reference level to setting
            }

        }

    }
    else{
        // Clear the BUSY bit indicating "no state machine activity"
        buckInstance->status.bits.busy = false;
    }

    // remain in STATE_ONLINE
    return(BUCK_OPSRET_REPEAT);
    
}

/*******************************************************************************
 * @fn uint16_t State_Error(volatile struct BUCK_CONVERTER_s *buckInstance)
 * @ingroup lib-layer-buck-state-machine-functions
 * @brief   If this function is called, the state machine is reset to INITIALIZE (URL=@ref State_Initialize).
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 * 
 * @details
 * This function is a default anchor in case task list index #0 is ever called.
 * This is the equivalent of a switch case "default".
 *********************************************************************************/
volatile uint16_t State_Error(volatile struct BUCK_CONVERTER_s *buckInstance)
{
    volatile uint16_t retval=0;
    
    // If this function is ever called, bring state machine back on track 
    // by resetting it to INITIALIZE
    
    retval = buckPWM_Suspend(buckInstance);             // Hold PWM output (turning off power)
    buckInstance->status.bits.busy = false;             // Reset busy bit

    if(retval)
        retval = BUCK_OPSRET_COMPLETE;
    else
        retval = BUCK_OPSRET_ERROR;

    
    return(retval);
}


// ______________________________________
// end of file

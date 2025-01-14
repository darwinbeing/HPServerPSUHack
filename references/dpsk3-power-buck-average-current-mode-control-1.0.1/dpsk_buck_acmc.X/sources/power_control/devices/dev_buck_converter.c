/*
 * File:   dev_buck_converter.c
 * Author: M91406
 *
 * Created on July 9, 2019, 1:10 PM
 */


#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer types header file
#include <stdbool.h> // include standard boolean types header file

#include "dev_buck_typedef.h"
#include "dev_buck_opstates.h"
#include "dev_buck_pconfig.h"

/*******************************************************************************
 * @fn	volatile uint16_t drv_BuckConverter_Initialize(volatile struct BUCK_CONVERTER_s *buckInstance)
 * @ingroup lib-layer-buck-converter-functions-public
 * @brief   This function initializes all peripheral modules and their instances used by the power controller
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 * This function initializes the PWM module, PWM channels, ADC channels for temperature,
 *  input voltage, output voltage and phase current. The buck IO pins are also initialize
 * while keeping the buck converter operation disabled. The state machine is set to Initialize. 
 *********************************************************************************/

volatile uint16_t drv_BuckConverter_Initialize(volatile struct BUCK_CONVERTER_s *buckInstance) 
{
    volatile uint16_t retval = 1;
    volatile uint16_t _i=0;
    
    retval &= buckPWM_ModuleInitialize(buckInstance); // Initialize PWM Module
    retval &= buckPWM_ChannelInitialize(buckInstance);  // Initialize PWM Channel of Buck Converter
    
    retval &= buckADC_ModuleInitialize();     // Initialize ADC Module
    
    retval &= buckADC_ChannelInitialize(&buckInstance->feedback.ad_temp); // Initialize Temperature Channel
    retval &= buckADC_ChannelInitialize(&buckInstance->feedback.ad_vin);  // Initialize Input Voltage Channel
    retval &= buckADC_ChannelInitialize(&buckInstance->feedback.ad_vout); // Initialize Output Voltage Channel
    
    for (_i=0; _i<buckInstance->set_values.no_of_phases; _i++) // Reset phase current values
        retval &= buckADC_ChannelInitialize(&buckInstance->feedback.ad_isns[_i]); // Initialize Phase Current Channel
    
    retval &= buckGPIO_Initialize(buckInstance); // Initialize additional control IOs

    
    buckInstance->v_loop.controller->status.bits.enabled = false; // Disable voltage loop
    
    if (buckInstance->set_values.control_mode == BUCK_CONTROL_MODE_ACMC) { // In current mode...
     
        for (_i=0; _i<buckInstance->set_values.no_of_phases; _i++) // Reset phase current values
        { buckInstance->i_loop[_i].controller->status.bits.enabled = false; } // Disable current loop
    
    }
        
    buckInstance->status.bits.enabled = false;  // Disable Buck Converter
    buckInstance->state_id.value = (uint32_t)BUCK_OPSTATE_INITIALIZE; // Reset state machine
    
    return(retval);
}

/*******************************************************************************
 * @fn	volatile uint16_t drv_BuckConverter_Execute(volatile struct BUCK_CONVERTER_s * buckInstance)
 * @ingroup lib-layer-buck-converter-functions-public
 * @brief   This function is the main buck converter state machine executing the most recent state 
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  0 = BUCK_OPSRET_REPEAT
 * @return  1 = BUCK_OPSRET_COMPLETE
 * @return  2 = BUCK_OPSRET_REPEAT
 *
 * @details
 * This function performs tasks in the state machine.  
 *  - If state machine state returns ERROR, switch to ERROR state in next execution cycle
 *  - If state machine state signals state completion, move on to next state in line
 *  - When state machine state returns REPEAT, the recent state function will be called again
 *  - When state machine state returns an unknown result, the state machine will be reset to INITIALIZE again 
 *********************************************************************************/

volatile uint16_t drv_BuckConverter_Execute(volatile struct BUCK_CONVERTER_s *buckInstance) 
{
    volatile uint16_t retval=1;
    
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /* NULL POINTER PROTECTION                                                            */
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    // If no buck instance has been declared, leave here
    if(buckInstance == NULL)
        return(0);
    
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /* CAPTURE ENABLE PIN STATE IF ENABLED BY USER CODE                                   */
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    if(buckInstance->gpio.EnableInput.enabled)
    {
        // Capture Enable Input pin status (1=high, 0=low)
        uint16_t pin_state = buckGPIO_GetPinState(&buckInstance->gpio.EnableInput);
        
        if(!buckInstance->gpio.EnableInput.polarity)
        // If POLARITY setting 0 = Active High (default)
            buckInstance->status.bits.enabled = (bool)(pin_state == 1);
        else
        // If POLARITY setting 1 = Active Low
            buckInstance->status.bits.enabled = (bool)(pin_state == 0);
    }
    
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /* DISABLE/SUSPEND/FAULT-RESET                                                                      */
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    // When enable status has changed from ENABLED to DISABLED or a fault condition 
    // is active, reset the state machine and hold it in RESET state
    if ((!buckInstance->status.bits.enabled) || (buckInstance->status.bits.suspend) ||
        (buckInstance->status.bits.fault_active))
    {
        if (!buckInstance->status.bits.ready)
            buckInstance->state_id.value = (uint32_t)BUCK_OPSTATE_INITIALIZE;
        else
            buckInstance->state_id.value = (uint32_t)BUCK_OPSTATE_RESET;
        
        // Call most recent state
        retval = BuckConverterStateMachine[buckInstance->state_id.bits.opstate_id](buckInstance);
        
        return((bool)(retval>0)); // Return
    }    
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /* FUNCTION CALL PROTECTION                                                           */
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    // If the state array pointer is out of range, roll over and start from first 
    // valid state
    if(buckInstance->state_id.bits.opstate_id >= BuckStateList_size)
        buckInstance->state_id.value = (uint32_t)BUCK_OPSTATE_INITIALIZE;
    
    if (BuckConverterStateMachine[buckInstance->state_id.bits.opstate_id] == NULL)
        return(0);
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    // Execute state machine step
    retval = BuckConverterStateMachine[buckInstance->state_id.bits.opstate_id](buckInstance);
    
    switch (retval) 
    {
        /* If state machine state returns ERROR, switch to ERROR state in next execution cycle */
        case BUCK_OPSRET_ERROR:
            
            buckInstance->state_id.value = (uint32_t)BUCK_OPSTATE_INITIALIZE;
            retval = 0;
            break;
            
        /* IF state machine state signals state completion, move on to next state in line */
        case BUCK_OPSRET_COMPLETE:
            
            // Increment main operating state pointer by one tick
            buckInstance->state_id.value = (uint32_t)(buckInstance->state_id.bits.opstate_id++);
            
            // Check if new index is out of range, reset to RESET if so
            if (buckInstance->state_id.bits.opstate_id >= BuckStateList_size)
                buckInstance->state_id.value = (uint32_t)BUCK_OPSTATE_RESET;

            retval = 1;
            break;
            
        /* When state machine state returns REPEAT, the recent state function will be called again */
        case BUCK_OPSRET_REPEAT:
            // Do nothing, same state will be called next time
            retval = 1;
            break;
            
        /* When state machine state returns an unknown result, the 
         * state machine will be reset to INITIALIZE again */
        default:
            // In case an undefined return value has been received,
            // REset state machine and start from scratch
            buckInstance->state_id.value = (uint32_t)BUCK_OPSTATE_INITIALIZE;
            retval = 0;
            break;
    }

    return(retval);
}

/*******************************************************************************
 * @fn	volatile uint16_t drv_BuckConverter_Start(volatile struct BUCK_CONVERTER_s * buckInstance)
 * @ingroup lib-layer-buck-converter-functions-public
 * @brief   This function starts the buck converter
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 * This function starts the buck converter operation by enabling the the buck PWM and ADC peripherals, 
 * enabling the buck converter and reseting the state machine to Initialize. 
 *********************************************************************************/

volatile uint16_t drv_BuckConverter_Start(volatile struct BUCK_CONVERTER_s * buckInstance) {

    volatile uint16_t retval=1;
    volatile uint16_t _i=0;
    
    // Disable control loops
    buckInstance->v_loop.controller->status.bits.enabled = false; // Disable voltage loop
    buckInstance->v_loop.ctrl_Reset(buckInstance->v_loop.controller); // Reset voltage loop histories

    if (buckInstance->set_values.control_mode == BUCK_CONTROL_MODE_ACMC) { // In current mode...
        
        for (_i=0; _i<buckInstance->set_values.no_of_phases; _i++)
        { 
            buckInstance->i_loop[_i].controller->status.bits.enabled = false; // Disable current loop
            buckInstance->i_loop[_i].ctrl_Reset(buckInstance->i_loop[_i].controller); // Reset current loop histories
        }
    }
    
    // Sequence PWM and ADC peripheral startup
    retval &= buckPWM_Start(buckInstance);   // Start PWM (All Outputs Disabled)
    if (retval) buckInstance->status.bits.pwm_active = 1; // IF PWM startup was successful, set PWM_ACTIVE flag
    retval &= buckADC_Start();              // Start ADC

    // Enable buck converter and reset state machine to INITIALIZE
    buckInstance->status.bits.enabled = true;   // Enable Buck Converter
    buckInstance->state_id.value = (uint32_t)BUCK_OPSTATE_INITIALIZE; // Reset state machine

    return(retval);
}

/*******************************************************************************
 * @fn	volatile uint16_t drv_BuckConverter_Stop(volatile struct BUCK_CONVERTER_s * buckInstance)
 * @ingroup lib-layer-buck-converter-functions-public
 * @brief  This function stop the buck converter opration
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 * 
 * @details
 * This function stops the buck converter operation by shutting down the PWM generator, 
 * disabling the voltage/current loop and reset the state machine to Initialize.
 * 
 *********************************************************************************/

volatile uint16_t drv_BuckConverter_Stop(volatile struct BUCK_CONVERTER_s *buckInstance) {

    volatile uint16_t retval=1;
    volatile uint16_t _i=0;
    
    // Stop PWM completely (shuts down PWM generator)
    retval &= buckPWM_Stop(buckInstance); // Stop PWM
    
    buckInstance->v_loop.controller->status.bits.enabled = false; // Disable voltage loop
    
    if (buckInstance->set_values.control_mode == BUCK_CONTROL_MODE_ACMC) {// In current mode...
        for (_i=0; _i<buckInstance->set_values.no_of_phases; _i++)
        { buckInstance->i_loop[_i].controller->status.bits.enabled = false; } // Disable current loop
    }
    
    buckInstance->status.bits.enabled = false;  // Disable Buck Converter
    buckInstance->state_id.value = (uint32_t)BUCK_OPSTATE_INITIALIZE; // Reset state machine

    return(retval);
}

/*******************************************************************************
 * @ingroup lib-layer-buck-converter-functions-public
 * @fn	volatile uint16_t drv_BuckConverter_Suspend(volatile struct BUCK_CONVERTER_s * buckInstance)
 * @brief  This function suspends the operation of the buck converter
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 *
 * @details
 * This function sets the suspend bit terminating operation. This bit will be evaluated in the
 * State machine tasks which eventually shuts down the operation of the buck converter.
 *********************************************************************************/

volatile uint16_t drv_BuckConverter_Suspend(volatile struct BUCK_CONVERTER_s *buckInstance) {
    
    volatile uint16_t retval=1;
    
    buckInstance->status.bits.suspend = true;  // Set SUSPEND bit terminating operation
    retval &= drv_BuckConverter_Execute(buckInstance);  // Enforce state switch immediately
    
    return(retval);
}

/*******************************************************************************
 * @fn	volatile uint16_t drv_BuckConverter_Resume(volatile struct BUCK_CONVERTER_s * buckInstance)
 * @ingroup lib-layer-buck-converter-functions-public
 * @brief   This function resume the operation of the buck converter
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 * 
 * @details
 * This function executes the latest task in the state machine enforcing state switch immediately.
 *********************************************************************************/

volatile uint16_t drv_BuckConverter_Resume(volatile struct BUCK_CONVERTER_s *buckInstance) {
    
    volatile uint16_t retval=1;
    
    buckInstance->status.bits.suspend = false;  // Reset running state machine
    retval &= drv_BuckConverter_Execute(buckInstance);  // Enforce state switch immediately
    
    return(retval);
}

// end of file

/*
 * File:   init_opa.c
 * Author: M91406
 *
 * Created on May 19, 2020, 1:19 PM
 */


#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer types header file
#include <stdbool.h> // include standard boolean types header file

#include "config/hal.h"
#include "init_dac.h"
#include "power_control/devices/dev_buck_typedef.h"

/**
 * @ingroup dac-initialization
 * @{
 */
/***********************************************************************************
 * @fn uint16_t sysDacModule_Initialize
 * @brief  DAC Module Base Register Initialization
 * @return unsigned integer
 * @return 0=failure
 * @return 1=success
 * 
 * @details
 * This function initializes the DAC module base registers for being operated 
 * by the auxiliary oscillator at an input clock frequency of 500 MHz. 
 * 
 * In this application the DAC is used to provide a constant reference voltage
 * of VDD/2 = 1.65 V to the external current sense shunt amplifier devices.
 * Neither, built-in comparator nor Pulse-Density Modulator (PDM) ramp generator
 * are used.
 *
 **********************************************************************************/

volatile uint16_t sysDacModule_Initialize(void) 
{
    volatile uint16_t retval=1;
    volatile struct P33C_DAC_MODULE_s* dac;

    dac = p33c_DacModule_GetHandle();
    retval &= p33c_DacModule_Dispose();
    retval &= p33c_DacModule_ConfigWrite(dacModuleDefault);

    return (retval);
    
}

/***********************************************************************************
 * @fn uint16_t sysDacOutput_Initialize(volatile uint16_t dacInstance, volatile uint16_t initValue)
 * @brief  Initializes a user specified DAC instance with DAC output enabled
 * @param  dacInstance Index of the selected DAC Instance (1=DAC1, 2=DAC2, etc.)
 * @param  initValue Integer value at startup of the DAC peripheral instance
 * @return unsigned integer (0=failure, 1=success)
 * 
 * @details
 * This function clears all registers of the declared DAC instance and 
 * initializes the registers required by a static Digital-to-Analog Converter
 * output with the given default values.
 * 
 * @note
 * The initialization function does not enable the DAC instance. Please use
 * function sysDacOutput_Enable() to turn on the DAC instance at the appropriate
 * time.
 *
 **********************************************************************************/

volatile uint16_t sysDacOutput_Initialize(volatile uint16_t dacInstance, volatile uint16_t initValue)
{
    volatile uint16_t retval=1;
    volatile struct P33C_DAC_INSTANCE_s* dac;
    
    dac = p33c_DacInstance_GetHandle(dacInstance);
    retval &= p33c_DacInstance_Dispose(dacInstance);
    
    dac->DACxCONL.bits.DACOEN = 1; // Enable DAC output pin
    dac->DACxCONH.bits.TMCB  = BUCK_LEB_PERIOD; // Set Leading Edge blanking interval period
    dac->DACxDATH.value = initValue; // Set DAC output to initial value
    
    return (retval);
}

/***********************************************************************************
 * @fn uint16_t sysDacOutput_Enable
 * @brief  Enables a user specified DAC output
 * @param  dacInstance Index of the selected DAC Instance (1=DAC1, 2=DAC2, etc.)
 * @return unsigned integer (0=failure, 1=success)
 * 
 * @details
 * This function enables the DAC output and DAC module specified in parameter
 * dacInstance. This function should be called after the DAC instance has been 
 * initialized by calling function sysDacOutput_Initialize().
 *
 **********************************************************************************/

volatile uint16_t sysDacOutput_Enable(volatile uint16_t dacInstance) {
    
    volatile uint16_t retval=1;
    volatile struct P33C_DAC_MODULE_s* dacmod;
    volatile struct P33C_DAC_INSTANCE_s* dacinst;
    
    // Capture DAC module SFR set
    dacmod = p33c_DacModule_GetHandle();
    dacmod->DacModuleCtrl1L.bits.DACON = true; // Ensure DAC module is enabled

    // Capture specified DAC instance SFR set
    dacinst = p33c_DacInstance_GetHandle(dacInstance);
    dacinst->DACxCONL.bits.DACEN = true; // Enable DAC instance
    dacinst->DACxCONL.bits.DACOEN = true; // Enable DAC output pin
    
    return (retval);
}

/***********************************************************************************
 * @fn uint16_t sysDacOutput_Disable
 * @brief  Disables a user specified DAC output
 * @param  dacInstance Index of the selected DAC Instance (1=DAC1, 2=DAC2, etc.)
 * @return unsigned integer (0=failure, 1=success)
 * 
 * @details
 * This function disables the DAC output and DAC module specified in parameter
 * dacInstance. 
 *
 **********************************************************************************/

volatile uint16_t sysDacOutput_Disable(volatile uint16_t dacInstance) {
    
    volatile uint16_t retval=1;
    volatile struct P33C_DAC_INSTANCE_s* dacinst;
    
    // Capture specified DAC instance SFR set
    dacinst = p33c_DacInstance_GetHandle(dacInstance);
    dacinst->DACxCONL.bits.DACOEN = false; // Disable DAC output pin
    dacinst->DACxCONL.bits.DACEN = false; // Disable DAC instance

    return (retval);
}
/** @}*/ // end of group dac-initialization

// end of file

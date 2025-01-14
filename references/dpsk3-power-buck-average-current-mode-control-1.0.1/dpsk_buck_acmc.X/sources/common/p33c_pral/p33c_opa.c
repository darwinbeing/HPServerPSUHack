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

// Include standard header files
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

#include "p33c_opa.h"

/*********************************************************************************
 * @fn struct P33C_OPA_MODULE_s* p33c_OpaModule_GetHandle(void)
 * @ingroup lib-layer-pral-functions-public-opa
 * @brief Gets pointer to op-amp Module SFR set
 * @return struct P33C_OPA_MODULE_s*: Pointer to op-amp module special function register set object 
 *  
 * @details
 * This function returns the pointer to a op-amp module register set
 * Special Function Register memory space. This pointer can be used to 
 * directly write to/read from the Special Function Registers of the 
 * op-amp peripheral module configuration.
 *********************************************************************************/

volatile struct P33C_OPA_MODULE_s* p33c_OpaModule_GetHandle(void)
{
    volatile struct P33C_OPA_MODULE_s* opa;
    
    // Capture Handle: set pointer to memory address of desired op-amp instance
    opa = (volatile struct P33C_OPA_MODULE_s*)((volatile uint8_t*) &AMPCON1L);
    
    return(opa);
}

/*********************************************************************************
 * @fn uint16_t p33c_OpaModule_Dispose(void)
 * @ingroup lib-layer-pral-functions-public-opa
 * @brief Resets all Op-Amp Module registers to their RESET default values
 * @return unsigned integer
 * @return 0 = failure, disposing OP-Amp module was not successful
 * @return 1 = success, disposing OP-Amp module was successful
 * 
 * @details
 * This function clears all op-amp module registers to their
 * default values set when the device comes out of RESET. 
 * 
 * Default configuration:
 * - all operational amplifiers are turned off
 * - the operational amplifier module is turned off
 * - all operational amplifier negative inputs are enabled
 * 
 *********************************************************************************/

volatile uint16_t p33c_OpaModule_Dispose(void)
{
    volatile uint16_t retval=1;
    
    retval = p33c_OpaModule_ConfigWrite(opaModuleConfigClear);
    
    return(retval);
}

/*********************************************************************************
 * @fn struct P33C_OPA_MODULE_s p33c_OpaModule_ConfigRead(void)
 * @ingroup lib-layer-pral-functions-public-opa
 * @brief Read the current configuration from the op-amp module base registers
 * @return struct P33C_OPA_MODULE_s
 * 
 * @details
 * This function reads all registers with their current configuration into
 * a data structure of type P33C_OPA_MODULE_s. Users can read and
 * verify or modify the configuration to write it back to the op-amp
 * module registers.
 * 
 *********************************************************************************/

volatile struct P33C_OPA_MODULE_s p33c_OpaModule_ConfigRead(void)
{
    volatile struct P33C_OPA_MODULE_s* opa;

    // Set pointer to memory address of desired Op-Amp instance
    opa = (volatile struct P33C_OPA_MODULE_s*)((volatile uint8_t*) &AMPCON1L);

    return(*opa);
    
}

/*********************************************************************************
 * @fn uint16_t p33c_OpaModule_ConfigWrite(volatile struct P33C_OPA_MODULE_s opaModuleConfig)
 * @ingroup lib-layer-pral-functions-public-opa
 * @brief Writes a user-defined configuration to the op-amp module base registers
 * @param  opaModuleConfig Operation Amplifier Peripheral SFR data object of type struct P33C_OPA_MODULE_s
 * @return unsigned integer
 * @return 0 = failure, writing op-amp module was not successful
 * @return 1 = success, writing op-amp module was successful
 * 
 * @details
 * This function writes a user-defined op-amp module configuration of
 * type P33C_OPA_MODULE_s to the op-amp module base registers. The 
 * individual register configurations have to be set in user-code 
 * before calling this function. To simplify the configuration process
 * of standard functions, this driver provides templates, which can be 
 * loaded and written directly.
 * 
 *********************************************************************************/

volatile uint16_t p33c_OpaModule_ConfigWrite(
        volatile struct P33C_OPA_MODULE_s opaModuleConfig
)
{
    volatile uint16_t retval=1;
    volatile struct P33C_OPA_MODULE_s* opa;    

    // Set pointer to memory address of the op-amp module base registers
    opa = (volatile struct P33C_OPA_MODULE_s*)((volatile uint8_t*) &AMPCON1L);
    *opa = opaModuleConfig;
    
    return(retval);
    
}

/*********************************************************************************
 * @var opaModuleConfigClear
 * @ingroup lib-layer-pral-functions-public-opa
 * @brief Default RESET configuration of the op-amp module base SFRs
 *
 * @details
 *   Default configuration of the op-amp module SFRs with all its registers 
 *   being reset to their default state when the device comes out of RESET.
 *   Programmers can use this template to reset (dispose) a previously used
 *   op-amp module when it's not used anymore or to ensure a known startup
 *   condition before writing individual configurations to its SFRs.
 * 
 ********************************************************************************/

volatile struct P33C_OPA_MODULE_s opaModuleConfigClear = {

    .AmpCon1L.value = 0x0000,
    .AmpCon1H.value = 0x0000

};

/*********************************************************************************
 * @var opaModuleDefault
 * @ingroup lib-layer-pral-functions-public-opa
 * @brief Default configuration of op-amp module running from 500 MHz input clock
 *
 * @details
 * Default configuration of the op-amp module SFRs with all its registers 
 * being reset to their default state when the device comes out of RESET.
 * 
 * Programmers can use this template to reset a previously used
 * op-amp module when it's not used anymore or to ensure a known startup
 * condition before writing individual configurations to its SFRs.
 * 
 ********************************************************************************/
/*  */
volatile struct P33C_OPA_MODULE_s opaModuleDefault = {

    .AmpCon1L.value = 0x0000,
    .AmpCon1H.value = 0x0007,

};



// end of file

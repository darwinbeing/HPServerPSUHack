#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

#include "p33c_ccp.h"

/*********************************************************************************
 * @ingroup lib-layer-pral-properties-private-ccp
 * @var ccpConfigDefault
 * @brief CCP Register Set reset state template
 * 
 * @details
 *     This P33C_CCP_MODULE_SFRSET_s data object provides a template of register 
 *     configuration values for a CCP peripheral instance. 
 * 
 *     Default configuration:
 *         - all options are reset to their default state
 * 
 *********************************************************************************/
/**@ingroup lib-layer-pral-properties-private-ccp */

volatile struct P33C_CCP_INSTANCE_SFRSET_s ccpConfigDefault = { 
    
    .CCPxTMRL.value = 0,
    .CCPxTMRH.value = 0,
    .CCPxBUFH.value = 0,
    .CCPxBUFL.value = 0,
    .CCPxPRH.value  = 0,
    .CCPxPRL.value = 0,
    .CCPxRAL.value = 0,
    .CCPxRBL.value = 0,
    .CCPxCON1H.value= 0,
    .CCPxCON1L.value = 0x0000,
    .CCPxCON2H.value = 0,
    .CCPxCON2L.value = 0,
    .CCPxCON3H.value= 0,
    .CCPxSTATL.value= 0
    
   };

/* PRIVATE FUNCTION CALL PROTOTYPES*/
//    (none)

/*********************************************************************************
 * @fn     volatile struct P33C_CCP_INSTANCE_SFRSET_s* p33c_CcpInstance_GetHandle(volatile uint16_t ccpInstance)
 * @ingroup lib-layer-pral-functions-public-ccp
 * @brief  Gets pointer to CCP instance SFR set
 * @param  ccpInstance Index of the Capture/Compare peripheral instance of type unsinged integer
 * @return Pointer to SCCP/MCCP instance special function register data object of type struct P33C_CCP_INSTANCE_SFRSET_s 
 *  
 * @details
 *      This function returns the pointer to a CCP module register set
 *    Special Function Register memory space. This pointer can be used to 
 *    directly write to/read from the Special Function Registers of the 
 *    CCP peripheral module configuration.
 * 
 *********************************************************************************/

volatile struct P33C_CCP_INSTANCE_SFRSET_s* p33c_CcpInstance_GetHandle(volatile uint16_t ccpInstance){

 volatile struct P33C_CCP_INSTANCE_SFRSET_s* ccp;
    
    // Capture Handle: set pointer to memory address of desired PWM instance
    ccp = (volatile struct P33C_CCP_INSTANCE_SFRSET_s*) 
         ((volatile uint8_t*)&CCP1CON1L + ((ccpInstance - 1) * P33C_CCPGEN_SFR_OFFSET));
    
    return(ccp);
}

/*********************************************************************************
 * @fn     volatile struct P33C_CCP_INSTANCE_SFRSET_s p33c_CcpInstance_ConfigRead(volatile uint16_t ccpInstance)
 * @ingroup lib-layer-pral-functions-public-ccp
 * @brief  Read the current configuration from the CCP instance registers
 * @param  ccpInstance Index of the Capture/Compare peripheral instance of type unsinged integer
 * @return SCCP/MCCP instance special function register data object of type struct P33C_CCP_INSTANCE_SFRSET_s
 * 
 * @details
 *     This function reads all registers with their current configuration into
 *     a data structure of type P33C_CCP_MODULE_SFRSET_s. Users can read and 
 *     verify of modify the configuration to write it back to any other CCP 
 *     peripheral instance.
 * 
 *********************************************************************************/

volatile struct P33C_CCP_INSTANCE_SFRSET_s p33c_CcpInstance_ConfigRead(volatile uint16_t ccpInstance){

 volatile struct P33C_CCP_INSTANCE_SFRSET_s* ccp;
    
    // Capture Handle: set pointer to memory address of desired PWM instance
    ccp = (volatile struct P33C_CCP_INSTANCE_SFRSET_s*) 
         ((volatile uint8_t*)&CCP1CON1L + ((ccpInstance - 1) * P33C_CCPGEN_SFR_OFFSET));
    
    return(*ccp);
}

/********************************************************************************
 * @fn     volatile uint16_t p33c_CcpInstance_ConfigWrite(volatile uint16_t ccpInstance, volatile struct P33C_CCP_INSTANCE_SFRSET_s ccpConfig)
 * @ingroup lib-layer-pral-functions-public-ccp
 * @brief  Writes a user-defined configuration to the CCP instance registers
 * @param  ccpInstance Index of the Capture/Compare peripheral instance of type unsinged integer
 * @param  ccpConfig SCCP/MCCP instance special function register data object of type struct P33C_CCP_INSTANCE_SFRSET_s
 * @return 0 = failure, writing to CCP instance was not successful
 * @return 1 = success, writing to CCP instance was successful
 * 
 * @details
 *     This function writes a user-defined CCP instance configuration of type 
 *     P33C_CCP_MODULE_SFRSET_s to the CCP instance registers. The 
 *     individual register configurations have to be set in user-code 
 *     before calling this function. To simplify the configuration process
 *     of standard functions, this driver provides templates, which can be 
 *     loaded and written directly 
 * 
 *********************************************************************************/

volatile uint16_t p33c_CcpInstance_ConfigWrite(volatile uint16_t ccpInstance, 
                                volatile struct P33C_CCP_INSTANCE_SFRSET_s ccpConfig)
{
    volatile uint16_t retval=1;
    volatile struct P33C_CCP_INSTANCE_SFRSET_s* ccp;    

    // Set pointer to memory address of desired PWM instance
    ccp = (volatile struct P33C_CCP_INSTANCE_SFRSET_s*) 
        ((volatile uint8_t*)&CCP1CON1L + ((ccpInstance - 1) * P33C_CCPGEN_SFR_OFFSET));
    *ccp = ccpConfig;
    
    return(retval);
    
}

// end of file

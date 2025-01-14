/*
 * File:   init_pmd.c
 * Author: M91406
 *
 * Created on July 8, 2019, 6:26 PM
 */


#include <xc.h>
#include "init_pmd.h"

/***********************************************************************************
 * @fn uint16_t sysPmd_Initialize(void)
 * @ingroup pmd-initialization
 * @brief  Powers down all device peripherals
 * @return unsigned integer (0=failure, 1=success)
 * 
 * @details
 *  When the device is coming out of RESET, all peripheral modules are powered
 *  and active, ready to be initialized and used. However, each active peripheral
 *  consumes power and accidental writes to any of its specifal function registers
 *  may be a potential source for undesired behavior. Hence, in this application 
 *  all power supply to all peripheral modules are disabled, minimizing the device 
 *  power consumption and preventing potential misbehavior of unused peripheral 
 *  modules.
 *
 * @note
 *  Unpowered peripherals cannot be initialized and reads from/write to the 
 *  respective special function registers will fail until the power supply to 
 *  the peripheral has been enabled. Hence, power to peripheral modules has to
 *  be enabled before any intialization of its register can take place.
 * 
 *  Depending on the peripheral a short delay period may be required for the 
 *  peripheral functions to become available after power up. 
 *
 *  Please refer to the device data sheet for details.
 *
 **********************************************************************************/

volatile uint16_t sysPmd_Initialize(void) {
    
    volatile uint16_t retval=1;
    
    // Disable all peripheral modules
	PMDCONbits.PMDLOCK = 0; // Unlock writes to PMD registers

	PMD1 = 0xffff;	// 
	PMD2 = 0xffff;	// 
	PMD3 = 0xffff;	// 

	PMDCONbits.PMDLOCK = 1; // Lock writes to PMD registers 
    
    return(retval);
}

// end of file

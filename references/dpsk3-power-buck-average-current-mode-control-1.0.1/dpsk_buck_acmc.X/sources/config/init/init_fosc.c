/*
 * File:   init_fosc.c
 * Author: M91406
 *
 * Created on July 8, 2019, 1:55 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#include "config/init/init_fosc.h"
#include "p33c_osc.h"   

#define TIMEOUT_LIMIT   5000    // timeout counter maximum

/***********************************************************************************
 * @fn uint16_t sysFosc_Initialize(void)
 * @ingroup clock-initialization
 * @brief  System oscillator initialization
 * @return unsigned integer 
 * 0=failure
 * 1=success
 * 
 * @details
 * This function initializes the internal 8 MHz RC oscillator as the main
 * oscillator and initializes the PLL to operate the CPU at maximum performance 
 * of 100 MHz instruction cycle.
 * 
 **********************************************************************************/
volatile uint16_t sysFosc_Initialize(void) 
{
    volatile uint16_t retval=1;
    
    retval = p33c_OscFrc_DefaultInitialize(CPU_SPEED_100_MIPS);
    
    return(retval);

}

/***********************************************************************************
 * @fn uint16_t sysAclk_Initialize(void)
 * @ingroup clock-initialization
 * @brief  Initializes the auxiliary clock of the device
 * @return unsigned integer (0=failure, 1=success)
 * 
 * @details
 * This function initializes the auxiliary PLL for operating ADC and PWM module
 * at maximum performance at an input clock of 500 MHz. Each peripheral module
 * may use additional frequency scalers and multipliers to further convert this
 * frequency.
 *
 **********************************************************************************/
volatile uint16_t sysAclk_Initialize(void) {

    volatile uint16_t retval=1;

    retval &= p33c_OscAuxClk_DefaultInitialize(AFPLLO_500_MHZ);

    return(retval);
}

// end of file


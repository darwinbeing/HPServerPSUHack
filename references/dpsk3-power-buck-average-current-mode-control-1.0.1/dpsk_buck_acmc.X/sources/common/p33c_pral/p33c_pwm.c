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

#include "p33c_pwm.h"

/*********************************************************************************
 * @fn uint16_t p33c_PwmModule_Initialize(void)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief Initializes the PWM base module by resetting all its registers to default
 * @return 0 = failure, disposing PWM generator was not successful
 * @return 1 = success, disposing PWM generator was successful
 * 
 * @details
 *     This function initializes the PWM module base registers with default
 *     values for maximum performance. 
 * 
 *     Default configuration:
 *         - auxiliary clock is selected as main input clock source
 *         - all output logic functions are disabled
 *         - all output logic events are disabled
 *         - all master timing registers are cleared
 *         - all PWM register locks are removed, allowing unrestricted SFR writes
 * 
 *********************************************************************************/

volatile uint16_t p33c_PwmModule_Initialize(void)
{
    volatile uint16_t retval=1;
    
    retval = p33c_PwmModule_ConfigWrite(pwmConfigDefault);
    
    return(retval);
}

/*********************************************************************************
 * @fn struct P33C_PWM_MODULE_s* p33c_PwmModule_GetHandle(void)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief Gets pointer to PWM module SFR set
 * @return struct P33C_PWM_GENERATOR_s: PWM generator object of the selected PWM generator instance
 *  
 * @details
 *      This function returns the PWM generator index, the PWM generator group
 *      (1 = [PG1-PG4], 2 = [PG5-PG8]) and the address pointer (handle) of
 *      the PWM generator Special Function Register set. handle can be used
 *      to directly read from/write to PWM registers of the selected PWM 
 *      generator with zero API overhead.
 * 
 *********************************************************************************/

volatile struct P33C_PWM_MODULE_s* p33c_PwmModule_GetHandle(void)
{
    volatile struct P33C_PWM_MODULE_s* pwm;
    
    // Capture Handle: set pointer to memory address of desired PWM instance
    pwm = (volatile struct P33C_PWM_MODULE_s*) ((volatile uint8_t*)&PCLKCON);
    
    return(pwm);
}

/*********************************************************************************
 * @fn uint16_t p33c_PwmModule_Dispose(void)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief Resets all PWM base module registers to their RESET default values
 * @return  0 = failure, disposing PWM generator was not successful
 * @return  1 = success, disposing PWM generator was successful
 * 
 * @details
 *     This function clears all PWM module base registers to their
 *     default values set when the device comes out of RESET. 
 * 
 *     Default configuration:
 *         - no main input clock source and scalers are selected
 *         - all output logic functions are disabled
 *         - all output logic events are disabled
 *         - all master timing registers are cleared
 *         - all PWM register locks are removed, allowing unrestricted SFR writes
 * 
 *********************************************************************************/

volatile uint16_t p33c_PwmModule_Dispose(void)
{
    volatile uint16_t retval=1;
    
    retval = p33c_PwmModule_ConfigWrite(pwmConfigClear);
    
    return(retval);
}

/*********************************************************************************
 * @fn struct P33C_PWM_MODULE_s p33c_PwmModule_ConfigRead(void)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief Read the current configuration from the PWM base module registers
 * @return struct P33C_PWM_MODULE_s
 * 
 * @details
 *     This function reads all registers with their current configuration into
 *     a data structure of type P33C_PWM_MODULE_s. Users can read and 
 *     verify of modify the configuration to write it back to the PWM module  
 *     base registers.
 * 
 *********************************************************************************/

volatile struct P33C_PWM_MODULE_s p33c_PwmModule_ConfigRead(void)
{
    volatile struct P33C_PWM_MODULE_s* pwm;

    // Set pointer to memory address of desired PWM instance
    pwm = (volatile struct P33C_PWM_MODULE_s*) 
        ((volatile uint8_t*) &PCLKCON);

    return(*pwm);    
    
}

/*********************************************************************************
 * @fn uint16_t p33c_PwmModule_ConfigWrite(volatile struct P33C_PWM_MODULE_s pwmConfig)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief Writes a user-defined configuration to the PWM base module registers
 * @param  pwmConfig PWM module register set of type struct P33C_PWM_MODULE_s
 * @return 0 = failure, disposing PWM generator was not successful
 * @return 1 = success, disposing PWM generator was successful
 * 
 * @details
 *     This function writes a user-defined PWM module configuration of type 
 *     P33C_PWM_MODULE_s to the PWM module base registers. The 
 *     individual register configurations have to be set in user-code 
 *     before calling this function. To simplify the configuration process
 *     of standard functions, this driver provides templates, which can be 
 *     loaded and written directly (see function p33c_PwmInitialize(void) 
 *     for details)
 * 
 *********************************************************************************/

volatile uint16_t p33c_PwmModule_ConfigWrite(volatile struct P33C_PWM_MODULE_s pwmConfig)
{
    volatile uint16_t retval=1;
    volatile struct P33C_PWM_MODULE_s* pwm;    

    // Set pointer to memory address of desired PWM instance
    pwm = (volatile struct P33C_PWM_MODULE_s*) 
        ((volatile uint8_t*) &PCLKCON);
    *pwm = pwmConfig;
    
    return(retval);
    
}

/*********************************************************************************
 * @fn struct P33C_PWM_GENERATOR_s p33c_PwmGenerator_ConfigRead(volatile uint16_t pgInstance)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief  Disposes a given PWM generator by resetting all its registers to default
 * @param  pgInstance Instance of the PWM generator (e.g. 1 = PG1, 2=PG2, etc.) of type unsigned integer
 * @return Generic PWM generator Special Function Register set of type struct P33C_PWM_GENERATOR_s
 * 
 * @details
 *     This function copies the contents of all PWM generator registers of the 
 *     specified generator instance (e.g. PG2) to a user variable of type 
 *     P33C_PWM_GENERATOR_t. This 'virtual' PWM configuration can then , for  
 *     example, be analyzed and/or modified in user code and applied to another 
 *     PWM generator using the function p33c_PwmGenerator_ConfigWrite(...). 
 * 
 *********************************************************************************/

volatile struct P33C_PWM_GENERATOR_s p33c_PwmGenerator_ConfigRead(volatile uint16_t pgInstance)
{
    volatile struct P33C_PWM_GENERATOR_s* pg;    

    // Set pointer to memory address of desired PWM instance
    pg = (volatile struct P33C_PWM_GENERATOR_s*) 
        ((volatile uint8_t*) &PG1CONL + ((pgInstance - 1) * P33C_PWMGEN_SFR_OFFSET));

    return(*pg);
    
}

/*********************************************************************************
 * @fn uint16_t p33c_PwmGenerator_ConfigWrite(
        volatile uint16_t pgInstance, 
        volatile struct P33C_PWM_GENERATOR_s pgConfig)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief Disposes a given PWM generator by resetting all its registers to default
 * @param pgInstance Instance of the PWM generator of type unsigned integer (e.g. 1=PG1, 2=PG2, etc.)
 * @param pgConfig   Generic PWM generator Special Function Register set of type struct P33C_PWM_GENERATOR_s
 * @return 0 = failure, disposing PWM generator was not successful
 * @return 1 = success, disposing PWM generator was successful
 * 
 * @details
 *     This function writes a user-defined PWM generator configuration of type 
 *     P33C_PWM_GENERATOR_t to the given PWM generator peripheral instance (e.g. PG2). 
 * 
 *********************************************************************************/

volatile uint16_t p33c_PwmGenerator_ConfigWrite(
        volatile uint16_t pgInstance, 
        volatile struct P33C_PWM_GENERATOR_s pgConfig
)
{
    volatile uint16_t retval=1;
    volatile struct P33C_PWM_GENERATOR_s* pg;    

    // Set pointer to memory address of desired PWM instance
    pg = (volatile struct P33C_PWM_GENERATOR_s*) 
        ((volatile uint8_t*) &PG1CONL + ((pgInstance - 1) * P33C_PWMGEN_SFR_OFFSET));
    *pg = pgConfig;
    
    return(retval);
    
}

/*********************************************************************************
 * @fn uint16_t p33c_PwmGenerator_Initialize(volatile uint16_t pgInstance)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief  Initializes a given PWM generator by resetting all its registers to default
 * @param  pgInstance Instance of the PWM generator of type unsigned integer (e.g. 1=PG1, 2=PG2, etc.)
 * @return 0 = failure, disposing PWM generator was not successful
 * @return 1 = success, disposing PWM generator was successful
 * 
 * @details
 *     This function initializes the specified PWM Generator with default
 *     input clock settings and high resolution mode enabled. All other
 *     Special Function Registers (SFR) are reset to their RESET default 
 *     values.
 * 
 *********************************************************************************/

volatile uint16_t p33c_PwmGenerator_Initialize(volatile uint16_t pgInstance)
{
    volatile uint16_t retval=1;
    volatile struct P33C_PWM_GENERATOR_s* pg;    
    
    
    // Set pointer to memory address of desired PWM instance
    pg = p33c_PwmGenerator_GetHandle(pgInstance);

    // Disable the PWM generator
    retval &= p33c_PwmGenerator_Disable(pg);

    // Reset all SFRs to default
    retval &= p33c_PwmGenerator_ConfigWrite(pgInstance, pgConfigClear);
    
    /* PWM GENERATOR CONTROL REGISTER LOW */
    pg->PGxCONL.bits.ON = 0;         // Disable PWM generator
    pg->PGxCONL.bits.CLKSEL = 0b01;  // Clock Selection: Selected by PWM module register PCLKCON.MCLKSEL bits  
    pg->PGxCONL.bits.MODSEL = 0b000; // Mode Selection: Independent Edge PWM mode 
    pg->PGxCONL.bits.TRGCNT = 0b000; // Trigger Count Selection: PWM Generator produces 1 PWM cycle after triggered 
    pg->PGxCONL.bits.HREN = 1;       // PWM Generator 1 High-Resolution Enable bit: PWM Generator 1 operates in High-Resolution mode
       
    return(retval);
}


/*********************************************************************************
 * @fn uint16_t p33c_PwmGenerator_Dispose(volatile uint16_t pgInstance)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief  Disposes a given PWM generator by resetting all its registers to default
 * @param  pgInstance Instance of the PWM generator of type unsigned integer (e.g. 1=PG1, 2=PG2, etc.)
 * @return 0 = failure, disposing PWM generator was not successful
 * @return 1 = success, disposing PWM generator was successful
 * 
 * @details
 *     This function disables the specified PWM Generator and clears all
 *     its Special Function Registers (SFR) to their RESET default values.
 * 
 * ********************************************************************************/

volatile uint16_t p33c_PwmGenerator_Dispose(volatile uint16_t pgInstance)
{
    volatile uint16_t retval=1;
    
    // Clear all registers of pgInstance
    p33c_PwmGenerator_ConfigWrite(pgInstance, pgConfigClear);
    
    return(retval);
}

/*********************************************************************************
 * @fn uint16_t p33c_PwmGenerator_Enable(volatile struct P33C_PWM_GENERATOR_s* pg)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief  Enables a given PWM generator with output pins disabled
 * @param  pg  Pointer to PWM generator peripheral instance register set of type struct P33C_PWM_GENERATOR_s
 * @return 0 = failure, enabling PWM generator was not successful
 * @return 1 = success, enabling PWM generator was successful
 * 
 * @details
 *     This function enables the PWM Generator and adds delay before enabling  
 *     PWM output pins. After having successfully enabled the generator,
 *     users need to call function PWM_Generator_Resume to allow the PWM 
 *     generator 
 * 
 *********************************************************************************/

volatile uint16_t p33c_PwmGenerator_Enable(volatile struct P33C_PWM_GENERATOR_s* pg)
{
    volatile uint16_t retval=1;
    volatile uint16_t timeout=0;
    
    // Set PWM generator override bits to prevent signals being generated outside the device
    pg->PGxIOCONL.bits.OVRENH = 1;
    pg->PGxIOCONL.bits.OVRENL = 1;

    // Assign GPIO ownership to I/O module control 
    pg->PGxIOCONH.bits.PENH = 0;
    pg->PGxIOCONH.bits.PENL = 0;
    
    // Turn on the PWM generator
    pg->PGxCONL.bits.ON = 1;
    
    // enforce update of timing registers
    pg->PGxSTAT.bits.UPDREQ = 1;
   
    // If high resolution mode is enabled, check if clock has locked in without errors
    if(pg->PGxCONL.bits.HREN)
    {
        while((!PCLKCONbits.HRRDY) && (timeout++<5000));
         if ((timeout >= 5000) || (PCLKCONbits.HRERR)) // if there is an error
            return(0);  // return ERROR     
        
    }
    
    // Assign GPIO ownership to given PWM generator 
    pg->PGxIOCONH.bits.PENH = 1;
    pg->PGxIOCONH.bits.PENL = 1;
    
    return(retval);       
    
}
 
/*********************************************************************************
 * @fn uint16_t p33c_PwmGenerator_Disable(volatile struct P33C_PWM_GENERATOR_s* pg)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief  Disables a given PWM generator 
 * @param  pg  Pointer to PWM generator peripheral instance register set of type struct P33C_PWM_GENERATOR_s
 * @return 0 = failure, disabling PWM generator was not successful
 * @return 1 = success, disabling PWM generator was successful
 * 
 * @details
 *     This function disables the PWM Generator and locks its
 *     PWM output pins in a safe state.
 * 
 *********************************************************************************/

volatile uint16_t p33c_PwmGenerator_Disable(volatile struct P33C_PWM_GENERATOR_s* pg)
{
    volatile uint16_t retval=1;
    
    // Set PWM generator override bits to prevent signals being generated outside the device
    pg->PGxIOCONL.bits.OVRENH = 1;
    pg->PGxIOCONL.bits.OVRENL = 1;

    // Assign GPIO ownership to I/O module control 
    pg->PGxIOCONH.bits.PENH = 0;
    pg->PGxIOCONH.bits.PENL = 0;
    
    // Turn on the PWM generator
    pg->PGxCONL.bits.ON = 0;
    
    return(retval);       
    
}

/*********************************************************************************
 * @fn uint16_t p33c_PwmGenerator_Resume(volatile struct P33C_PWM_GENERATOR_s* pg)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief  Disables a PWM generator IOs
 * @param  pg  Pointer to PWM generator peripheral instance register set of type struct P33C_PWM_GENERATOR_s
 * @return 0 = failure, enabling PWM generator IOs was not successful
 * @return 1 = success, enabling PWM generator IOs was successful
 * 
 * @details
 *     This function enables the PWM Generator IO instance allowing 
 *     signals to be routed to the output pins. This on/off transition
 *     occurs synchronously to the PWM period. 
 * 
 *********************************************************************************/

volatile uint16_t p33c_PwmGenerator_Resume(volatile struct P33C_PWM_GENERATOR_s* pg)
{
    volatile uint16_t retval=1;
    
    // Set PWM generator override bits to prevent signals being generated outside the device
    pg->PGxIOCONL.bits.OVRENH = 0;
    pg->PGxIOCONL.bits.OVRENL = 0;

    
    return(retval);       
    
}

/*********************************************************************************
 * @fn uint16_t p33c_PwmGenerator_Suspend(volatile struct P33C_PWM_GENERATOR_s* pg)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief  Disables the PWM generator outputs but keeps the module running
 * @param  pg  Pointer to PWM generator peripheral instance register set of type struct P33C_PWM_GENERATOR_s
 * @return 0 = failure, enabling PWM generator IOs was not successful
 * @return 1 = success, enabling PWM generator IOs was successful
 * 
 * @details
 *     This function disables the PWM Generator IO instance preventing 
 *     signals to be routed to the output pins. This on/off transition
 *     occurs synchronously to the PWM period. 
 * 
 *********************************************************************************/

volatile uint16_t p33c_PwmGenerator_Suspend(volatile struct P33C_PWM_GENERATOR_s* pg)
{
    volatile uint16_t retval=1;
    
    // Set PWM generator override bits to prevent signals being generated outside the device
    pg->PGxIOCONL.bits.OVRENH = 1;
    pg->PGxIOCONL.bits.OVRENL = 1;

    
    return(retval);       
    
}


/*********************************************************************************
 * @fn uint16_t p33c_PwmGenerator_SetPeriod(volatile struct P33C_PWM_GENERATOR_s* pg, volatile uint16_t period )
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief  Sets the period of a given PWM generator 
 * @param  pg  Pointer to PWM generator peripheral instance register set of type struct P33C_PWM_GENERATOR_s
 * @param  period  Counter compare value of type unsigned integer of the PWM generator time base
 * @return 0 = failure, disabling PWM generator was not successful
 * @return 1 = success, disabling PWM generator was successful
 * 
 * @details
 *     This function writes the value specified by 'period' to the PERIOD register
 *     of the specified PWM generator.
 * 
 *********************************************************************************/

volatile uint16_t p33c_PwmGenerator_SetPeriod(
            volatile struct P33C_PWM_GENERATOR_s* pg, 
            volatile uint16_t period
    )
{
    volatile uint16_t retval=1;
    
    // Set PWM generator period
    pg->PGxPER.value = period;
    
    return(retval);       
    
}


/*********************************************************************************
 * @fn uint16_t p33c_PwmGenerator_SetDutyCycle(
            volatile struct P33C_PWM_GENERATOR_s* pg, 
            volatile uint16_t duty
    )
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief Sets the on-time of a given PWM generator 
 * @param pg  Pointer to PWM generator peripheral instance register set of type struct P33C_PWM_GENERATOR_s
 * @param duty Duty cycle counter value of type unsigned integer
 * @return 0 = failure, disabling PWM generator was not successful
 * @return 1 = success, disabling PWM generator was successful
 * 
 * @details
 *     This function writes the value specified by 'duty' to the DUTY CYCLE register
 *     of the specified PWM generator.
 * 
 *********************************************************************************/

volatile uint16_t p33c_PwmGenerator_SetDutyCycle(
            volatile struct P33C_PWM_GENERATOR_s* pg, 
            volatile uint16_t duty
    )
{
    volatile uint16_t retval=1;
    
    // Set PWM generator duty cycle
    pg->PGxDC.value = duty;
    
    return(retval);       
    
}


/*********************************************************************************
 * @fn uint16_t p33c_PwmGenerator_SetDeadTimes(
                volatile struct P33C_PWM_GENERATOR_s* pg, 
                volatile uint16_t dead_time_rising, 
                volatile uint16_t dead_time_falling)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief  Sets the dead-times of a given PWM generator 
 * @param  pg  Pointer to PWM generator peripheral instance register set of type struct P33C_PWM_GENERATOR_s
 * @param  dead_time_rising  Rising edge dead time counter value of type unsigned integer
 / @param  dead_time_falling Falling edge dead time counter value of type unsigned integer
 * @return 0 = failure, disabling PWM generator was not successful
 * @return 1 = success, disabling PWM generator was successful
 * 
 * @details
 *     This function writes the value specified by 'duty' to the DUTY CYCLE register
 *     of the specified PWM generator.
 * 
 *********************************************************************************/

volatile uint16_t p33c_PwmGenerator_SetDeadTimes(
                volatile struct P33C_PWM_GENERATOR_s* pg, 
                volatile uint16_t dead_time_rising, 
                volatile uint16_t dead_time_falling
    )
{
    volatile uint16_t retval=1;
    
    // Set PWM generator period
    pg->PGxDTH.value = dead_time_rising;
    pg->PGxDTL.value = dead_time_falling;
    
    return(retval);       
    
}
/*********************************************************************************
 * @fn struct P33C_PWM_GENERATOR_s* p33c_PwmGenerator_GetHandle(volatile uint16_t pgInstance)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief  Returns the PWM generator index
 * @param  pgInstance Instance of the PWM generator of type unsigned integer (e.g. 1=PG1, 2=PG2, etc.)
 * @return Pointer address to PWM generator instance of type struct P33C_PWM_GENERATOR_s specified by parameter pgInstance
 *  
 * @details
 * This function returns the address pointer (pgHandle) of the PWM generator
 * Special Function Register set specified by parameter pgInstance. pgHandle 
 * can be used to assign a global variable in user code, which allows to directly 
 * read from/write to PWM registers of the selected PWM generator with zero API 
 * overhead.
 * 
 *********************************************************************************/

volatile struct P33C_PWM_GENERATOR_s* p33c_PwmGenerator_GetHandle(volatile uint16_t pgInstance)
{
    volatile struct P33C_PWM_GENERATOR_s* pg;
    
    // Capture Handle: set pointer to memory address of desired PWM instance
    pg = (volatile struct P33C_PWM_GENERATOR_s*) 
         ((volatile uint8_t*)&PG1CONL + ((pgInstance - 1) * P33C_PWMGEN_SFR_OFFSET));
    
    return(pg);
}

/*********************************************************************************
 * volatile uint16_t p33c_PwmGenerator_GetInstance(volatile struct P33C_PWM_GENERATOR_s* pg)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief   Get the PWM generator instance index of a known PWM object pointer address
 * @param   pg  Pointer to PWM generator peripheral instance register set of type struct P33C_PWM_GENERATOR_s
 * @return  index of PWM generator instance of type unsigned integer
 *  
 * @details
 * This function returns the non-zero PWM generator instance index of the PWM generator of
 * the PWM generator Special Function Register set referenced by the pointer address of 
 * function parameter pg. If the memory address does not match any available PWM generator 
 * instance start address, this function returns '0'.
 * 
 *********************************************************************************/
volatile uint16_t p33c_PwmGenerator_GetInstance(volatile struct P33C_PWM_GENERATOR_s* pg)
{
    volatile uint16_t retval=1;

    // Null-pointer protection
    if (pg == NULL)
        return(0);

    // Capture Instance: set pointer to memory address of desired PWM instance
    retval = (volatile uint16_t)
        (((volatile uint16_t)&pg->PGxCONL - (volatile uint16_t)&PG1CONL) / P33C_PWMGEN_SFR_OFFSET) + 1;
            
    if (retval > P33C_PG_COUNT)
        return(0); // PWM generator not member of a valid group 

    return(retval);
}

/*********************************************************************************
 * volatile uint16_t p33c_PwmGenerator_GetGroup(volatile struct P33C_PWM_GENERATOR_s* pg)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief  Get the PWM generator group
 * @param  pg  Pointer to PWM generator peripheral instance register set of type struct P33C_PWM_GENERATOR_s
 * @return 0 = error, PWM generator group not found
 * @return 1 = PWM generator is in group #1
 * @return 2 = PWM generator is in group #2
 *  
 * @details
 * This function identify the group to which the PWM generator instance belongs
 * (1 = [PG1-PG4], 2 = [PG5-PG8]) and verifies if the PWMgenerator group 
 * is valid and available. If no group is found, the function returns '0'
 *********************************************************************************/
volatile uint16_t p33c_PwmGenerator_GetGroup(volatile struct P33C_PWM_GENERATOR_s* pg)
{
    volatile uint16_t retval=1;
    volatile uint16_t pgInstance;

    // Null-pointer protection
    if (pg == NULL)
        return(0);

    // Get group of PWM generator
    pgInstance = (volatile uint16_t)
        (((volatile uint16_t)&pg->PGxCONL - (volatile uint16_t)&PG1CONL) / P33C_PWMGEN_SFR_OFFSET + 1);
    
    // Verify PWM generator group is valid and available
    if (pgInstance > P33C_PG_COUNT)
        return(0); // PWM generator not member of a valid group 
    else if (pgInstance > 4)
        retval = 2; // PWM generator is member of group #2 [PG5-PG8]
    else
        retval = 1; // PWM generator is member of group #1 [PG1-PG4]

    return(retval);
}

/*********************************************************************************
 * @fn uint16_t p33c_PwmGenerator_SyncGenerators(volatile struct P33C_PWM_GENERATOR_s* pgHandleMother, volatile uint16_t pgMotherTriggerOutput, volatile struct P33C_PWM_GENERATOR_s* pgHandleChild, volatile bool ChildImmediateUpdate)
 * @ingroup lib-layer-pral-functions-public-pwm
 * @brief Sets the synchronization triggers of two PWM generators, of which the Mother PWM generator is the trigger provider and the Child PWM generator is the synchronization trigger receiver.
 * @param pgHandleMother Pointer to PWM generator object of type struct P33C_PWM_GENERATOR_s of triggering PWM generator (sync trigger provider)
 * @param pgMotherTriggerOutput Trigger output selection of type unsigned integer of Sync Trigger Provider of the selected PWM generator instance
 *          0 = EOC/SOC
 *          1 = PGxTRIGA
 *          2 = PGxTRIGB
 *          3 = PGxTRIGC
 * @param pgHandleChild  Pointer to PWM generator object of type struct P33C_PWM_GENERATOR_s of triggered PWM generator (sync trigger receiver)
 * @param ChildImmediateUpdate 
 *          true = synchronization trigger synchronizes Child PWM generator at trigger edge
 *          false = synchronization trigger synchronizes Child PWM generator at EOC/SOC
 * @return 0 = failure, PWM generator synchronization was not successful
 * @return 1 = success, PWM generator synchronization was successful
 * 
 * @details
 *      This function sets the synchronization triggers of two PWM generators, 
 *      of which the Mother PWM generator is the trigger provider and the Child
 *      PWM generator is the synchronization trigger receiver.
 * 
 *      The Mother PWM generator is configured to broadcast its UPDATE bit and
 *      provides one of four, selectable synchronization trigger outputs (EOC/SOC, 
 *      PGxTRIGA, PGxTRIGB or PGxTRIGC). The Mother PWM generator can be 
 *      independent or being triggered by another PWM generator.
 * 
 *      The Child PWM generator is configured to trigger on the event generated
 *      by the Mother PWM generator as configured above. The optional selection
 *      of immediate synchronization or synchronization of the SOC event is set 
 *      by parameter ChildImmediateUpdate.
 * 
 *********************************************************************************/

volatile uint16_t p33c_PwmGenerator_SyncGenerators(
                            volatile struct P33C_PWM_GENERATOR_s* pgHandleMother, 
                            volatile uint16_t pgMotherTriggerOutput,
                            volatile struct P33C_PWM_GENERATOR_s* pgHandleChild,
                            volatile bool ChildImmediateUpdate
    )
{
    volatile uint16_t retval=1;
    volatile uint16_t pgMotherInstance=0;
    volatile uint16_t pgChildInstance=0;
    volatile uint16_t pgMotherGroup=0;
    volatile uint16_t pgChildGroup=0;
    volatile uint16_t pgInstance;
    
    // Null-pointer protection
    if ((pgHandleMother == NULL) || (pgHandleChild == NULL))
        return(0);
    
    // Capture PWM generator instances and groups for given handles
    pgMotherInstance = p33c_PwmGenerator_GetInstance(pgHandleMother);
    pgMotherGroup = p33c_PwmGenerator_GetGroup(pgHandleMother);
    
    pgChildInstance = p33c_PwmGenerator_GetInstance(pgHandleChild);
    pgChildGroup = p33c_PwmGenerator_GetGroup(pgHandleChild);
    
    // Enable update trigger broadcast in Mother PWM
    // PWM generator broadcasts software set/clear of the UPDREQ status bit and EOC signal
    pgHandleMother->PGxCONH.bits.MSTEN = 1;  

    // PWM Generator Trigger Output Selection
    // 0b011 = PGxTRIGC compare event is the PWM Generator trigger
    // 0b010 = PGxTRIGB compare event is the PWM Generator trigger
    // 0b001 = PGxTRIGA compare event is the PWM Generator trigger
    // 0b000 = EOC event is the PWM Generator trigger
    pgHandleMother->PGxEVTL.bits.PGTRGSEL = (pgMotherTriggerOutput & 0x0003);  

    // Configure child PWM in slaved mode, incorporating immediate of EOC selection
    pgHandleChild->PGxCONH.bits.UPDMOD = (0b010 | ChildImmediateUpdate); // Slaved SOC update 
    pgHandleChild->PGxCONH.bits.TRGMOD = 1;     // PWM Generator operates in Retriggerable mode
    pgHandleChild->PGxEVTL.bits.UPDTRG = 0b00;  // Timing register updates triggered through UPDREQ bit (PGxSTAT[3])
    pgHandleChild->PGxCONL.bits.MODSEL = 0b000; // Independent Edge PWM mode;
    
    /* Set sync trigger input to Child generator
    
    SOCS[3:0]: Start-of-Cycle Selection bits(1,2,3)
    1111 = TRIG bit or PCI Sync function only (no hardware trigger source is selected)
    1110-0101 = Reserved
    0100 = Trigger output selected by PG4 or PG8 PGTRGSEL[2:0] bits (PGxEVTL[2:0])
    0011 = Trigger output selected by PG3 or PG7 PGTRGSEL[2:0] bits (PGxEVTL[2:0])
    0010 = Trigger output selected by PG2 or PG6 PGTRGSEL[2:0] bits (PGxEVTL[2:0])
    0001 = Trigger output selected by PG1 or PG5 PGTRGSEL[2:0] bits (PGxEVTL[2:0])
    0000 = Local EOC ? PWM Generator is self-triggered
    
    */
    
   if (pgMotherGroup == pgChildGroup)
   { 
       // If both PWM generators are member of the same group, 
       // direct synchronization can be used
       
       if (pgMotherGroup == 1)
           pgHandleChild->PGxCONH.bits.SOCS = pgMotherInstance;
       else if (pgMotherGroup == 2)
           pgHandleChild->PGxCONH.bits.SOCS = (pgMotherInstance - 4);
       else
           return(0); // Exit if PWM generator group is out of range
       
   }
   else
   {
        // Synchronization across PWM generator groups need to be routed 
        // through the PCI Sync function 
        
        pgInstance = p33c_PwmGenerator_GetInstance(pgHandleChild);
        
        pgHandleChild->PGxCONH.bits.SOCS = 0b1111; 
        pgHandleChild->PGxSPCIL.bits.PSS = 0b00001; // Internally connected to the output of PWMPCI[2:0] MUX
        pgHandleChild->PGxLEBH.bits.PWMPCI = pgInstance; 
    }
    
    
    return(retval);       

}

/* ********************************************************************************************* * 
 * PWM MODULE BASE CONFIGURATION TEMPLATES
 * ********************************************************************************************* */
 
/*********************************************************************************
 * @var struct P33C_PWM_MODULE_s pwmConfigClear
 * @ingroup lib-layer-pral-properties-private-pwm
 * @brief Default RESET configuration of the PWM module SFRs
 *
 * @details
 *   Default configuration of the PWM module SFRs with all its registers being 
 *   reset to their default state when the device comes out of RESET.
 *   Programmers can use this template to reset (dispose) a previously used
 *   PWM module when it's not used anymore or to secure a known startup
 *   condition before writing individual configurations to its SFRs.
 * 
 ********************************************************************************/

volatile struct P33C_PWM_MODULE_s pwmConfigClear = { 
    
        .vPCLKCON.value = 0x0000, // HRRDY=0, HRERR=0, LOCK=0, DIVSEL=0b00, MCLKSEL=0b00
        .vFSCL.value = 0x0000, // FSCL=0
        .vFSMINPER.value = 0x0000, // FSMINPER=0
        .vMPHASE.value = 0x0000, // MPHASE=0
        .vMDC.value = 0x0000, // MDC=0
        .vMPER.value = 0x0000, // MPER=0
        .vLFSR.value = 0x0000, // LFSR=0
        .vCMBTRIGL.value = 0x0000, // CTA1EN=0, CTA2EN=0, CTA3EN=0, CTA4EN=0, CTA5EN=0, CTA6EN=0, CTA7EN=0, CTA8EN=0
        .vCMBTRIGH.value = 0x0000, // CTB1EN=0, CTB2EN=0, CTB3EN=0, CTB4EN=0, CTB5EN=0, CTB6EN=0, CTB7EN=0, CTB8EN=0
        .LOGCON_A.value = 0x0000, // PWMS1A=0b0000, PWMS2A=0b0000, S1APOL=0, S2APOL=0, PWMLFA=0b00, PWMLFAD=0b000
        .LOGCON_B.value = 0x0000, // PWMS1B=0b0000, PWMS2B=0b0000, S1BPOL=0, S2BPOL=0, PWMLFB=0b00, PWMLFBD=0b000
        .LOGCON_C.value = 0x0000, // PWMS1C=0b0000, PWMS2C=0b0000, S1CPOL=0, S2CPOL=0, PWMLFC=0b00, PWMLFCD=0b000
        .LOGCON_D.value = 0x0000, // PWMS1D=0b0000, PWMS2D=0b0000, S1DPOL=0, S2DPOL=0, PWMLFD=0b00, PWMLFDD=0b000
        .LOGCON_E.value = 0x0000, // PWMS1E=0b0000, PWMS2E=0b0000, S1EPOL=0, S2EPOL=0, PWMLFE=0b00, PWMLFED=0b000
        .LOGCON_F.value = 0x0000, // PWMS1F=0b0000, PWMS2F=0b0000, S1FPOL=0, S2FPOL=0, PWMLFF=0b00, PWMLFFD=0b000
        .PWMEVT_A.value = 0x0000, // EVTAOEN=0, EVTAPOL=0, EVTASTRD=0, EVTASYNC=0, EVTASEL=0b000, EVTAPGS=0b000
        .PWMEVT_B.value = 0x0000, // EVTBOEN=0, EVTBPOL=0, EVTBSTRD=0, EVTBSYNC=0, EVTBSEL=0b000, EVTBPGS=0b000
        .PWMEVT_C.value = 0x0000, // EVTCOEN=0, EVTCPOL=0, EVTCSTRD=0, EVTCSYNC=0, EVTCSEL=0b000, EVTCPGS=0b000
        .PWMEVT_D.value = 0x0000, // EVTDOEN=0, EVTDPOL=0, EVTDSTRD=0, EVTDSYNC=0, EVTDSEL=0b000, EVTDPGS=0b000
        .PWMEVT_E.value = 0x0000, // EVTEOEN=0, EVTEPOL=0, EVTESTRD=0, EVTESYNC=0, EVTESEL=0b000, EVTEPGS=0b000
        .PWMEVT_F.value = 0x0000  // EVTFOEN=0, EVTFPOL=0, EVTFSTRD=0, EVTFSYNC=0, EVTFSEL=0b000, EVTFPGS=0b000
   };

/*******************************************************************************
 * @var struct P33C_PWM_MODULE_s pwmConfigDefault
 * @ingroup lib-layer-pral-properties-private-pwm
 * @brief Default initialization configuration of the PWM module SFRs
 *
 * @details
 *   Default configuration of the PWM module SFRs for normal operation at 
 *   maximum performance using the auxiliary clock input, expecting a 
 *   500 MHz input clock. All other functions of the PWM module base registers,
 *   such as combinatorial logic blocks, are turned off.
 *   Programmers can use this template to initialize the PWM module 
 *   in default mode before writing individual configurations to its SFRs.
 * 
 ********************************************************************************/

volatile struct P33C_PWM_MODULE_s pwmConfigDefault = { 
    
        .vPCLKCON.value = 0x0003,   // HRRDY=0, HRERR=0, LOCK=0, DIVSEL=0b00, MCLKSEL=0b11
        .vFSCL.value = 0x0000,      // FSCL=0
        .vFSMINPER.value = 0x0000,  // FSMINPER=0
        .vMPHASE.value = 0x0000,    // MPHASE=0
        .vMDC.value = 0x0000,       // MDC=0
        .vMPER.value = 0x0000,      // MPER=0
        .vLFSR.value = 0x0000,      // LFSR=0
        .vCMBTRIGL.value = 0x0000,  // CTA1EN=0, CTA2EN=0, CTA3EN=0, CTA4EN=0, CTA5EN=0, CTA6EN=0, CTA7EN=0, CTA8EN=0
        .vCMBTRIGH.value = 0x0000,  // CTB1EN=0, CTB2EN=0, CTB3EN=0, CTB4EN=0, CTB5EN=0, CTB6EN=0, CTB7EN=0, CTB8EN=0
        .LOGCON_A.value = 0x0000,   // PWMS1A=0b0000, PWMS2A=0b0000, S1APOL=0, S2APOL=0, PWMLFA=0b00, PWMLFAD=0b000
        .LOGCON_B.value = 0x0000,   // PWMS1B=0b0000, PWMS2B=0b0000, S1BPOL=0, S2BPOL=0, PWMLFB=0b00, PWMLFBD=0b000
        .LOGCON_C.value = 0x0000,   // PWMS1C=0b0000, PWMS2C=0b0000, S1CPOL=0, S2CPOL=0, PWMLFC=0b00, PWMLFCD=0b000
        .LOGCON_D.value = 0x0000,   // PWMS1D=0b0000, PWMS2D=0b0000, S1DPOL=0, S2DPOL=0, PWMLFD=0b00, PWMLFDD=0b000
        .LOGCON_E.value = 0x0000,   // PWMS1E=0b0000, PWMS2E=0b0000, S1EPOL=0, S2EPOL=0, PWMLFE=0b00, PWMLFED=0b000
        .LOGCON_F.value = 0x0000,   // PWMS1F=0b0000, PWMS2F=0b0000, S1FPOL=0, S2FPOL=0, PWMLFF=0b00, PWMLFFD=0b000
        .PWMEVT_A.value = 0x0000,   // EVTAOEN=0, EVTAPOL=0, EVTASTRD=0, EVTASYNC=0, EVTASEL=0b000, EVTAPGS=0b000
        .PWMEVT_B.value = 0x0000,   // EVTBOEN=0, EVTBPOL=0, EVTBSTRD=0, EVTBSYNC=0, EVTBSEL=0b000, EVTBPGS=0b000
        .PWMEVT_C.value = 0x0000,   // EVTCOEN=0, EVTCPOL=0, EVTCSTRD=0, EVTCSYNC=0, EVTCSEL=0b000, EVTCPGS=0b000
        .PWMEVT_D.value = 0x0000,   // EVTDOEN=0, EVTDPOL=0, EVTDSTRD=0, EVTDSYNC=0, EVTDSEL=0b000, EVTDPGS=0b000
        .PWMEVT_E.value = 0x0000,   // EVTEOEN=0, EVTEPOL=0, EVTESTRD=0, EVTESYNC=0, EVTESEL=0b000, EVTEPGS=0b000
        .PWMEVT_F.value = 0x0000    // EVTFOEN=0, EVTFPOL=0, EVTFSTRD=0, EVTFSYNC=0, EVTFSEL=0b000, EVTFPGS=0b000
   };

/* ********************************************************************************************* * 
 * PWM GENERATOR CONFIGURATION TEMPLATES
 * ********************************************************************************************* */

/*********************************************************************************
 * @var struct P33C_PWM_GENERATOR_s pgConfigClear
 * @ingroup lib-layer-pral-properties-private-pwm
 * @brief Default RESET configuration of one PWM generator instance SFRs
 *
 * @details
 *   Default configuration of the PWM generator SFRs with all its registers 
 *   being reset to their default state when the device comes out of RESET.
 *   Programmers can use this template to reset (dispose) a previously used
 *   PWM generator when it's not used anymore or to secure a known startup
 *   condition before writing individual configurations to its SFRs.
 * 
 ********************************************************************************/

volatile struct P33C_PWM_GENERATOR_s pgConfigClear = {
    
        .PGxCONL.value = 0x0000,    // ON=0, TRGCNT=0b000, HREN=0, CLKSEL=b00, MODSEL=0b000
        .PGxCONH.value = 0x0000,    // MDCSEL=0, MPERSEL=0, MPHSEL=0, MSTEN=0, UPDMOD=0b000, TRGMOD=0, SOCS=0b0000
        .PGxSTAT.value = 0x0000,    // SEVT=0, FLTEVT=0, CLEVT=0, FFEVT=0, SACT=0, FLTACT=0, CLACT=0, FFACT=0, TRSET=0, TRCLR=0, CAP=0, UPDATE=0, UPDREQ=0, STEER=0, CAHALF=0, TRIG=0
        .PGxIOCONL.value = 0x0000,  // CLMOD=0, SWAP=0, OVRENH=0, OVRENL=0, OVRDAT=0, OSYNC=0b00, FLTDAT=0, CLDAT=0, FFDAT=0, DBDAT=0
        .PGxIOCONH.value = 0x0000,  // CAPSRC=0b000, DTCMPSEL=0, PMOD=0b00, PENH=0, PENL=0, POLH=0, POLL=0
        .PGxEVTL.value = 0x0000,    // ADTR1PS=0b00000, ADTR1EN3=0, ADTR1EN2=0, ADTR1EN1=0, UPDTRG=0b00, PGTRGSEL=0b000
        .PGxEVTH.value = 0x0000,    // FLTIEN=0, CLIEN=0, FFIEN=0, SIEN=0, IEVTSEL=0b00, ADTR2EN3=0, ADTR2EN2=0, ADTR2EN1=0, ADTR1OFS=0b00000
        .PGxFPCIL.value = 0x0000,   // TSYNCDIS=0, TERM=0b000, AQPS=0, AQSS=0b000, SWTERM=0, PSYNC=0, PPS=0, PSS=0b00000
        .PGxFPCIH.value = 0x0000,   // BPEN=0, BPSEL=0b000, ACP0b000, SWPCI=0, SWPCIM=0b00, LATMOD=0, TQPS=0, TQSS=0b000
        .PGxCLPCIL.value = 0x0000,  // TSYNCDIS=0, TERM=0b000, AQPS=0, AQSS=0b000, SWTERM=0, PSYNC=0, PPS=0, PSS=0b00000
        .PGxCLPCIH.value = 0x0000,  // BPEN=0, BPSEL=0b000, ACP0b000, SWPCI=0, SWPCIM=0b00, LATMOD=0, TQPS=0, TQSS=0b000
        .PGxFFPCIL.value = 0x0000,  // TSYNCDIS=0, TERM=0b000, AQPS=0, AQSS=0b000, SWTERM=0, PSYNC=0, PPS=0, PSS=0b00000
        .PGxFFPCIH.value = 0x0000,  // BPEN=0, BPSEL=0b000, ACP0b000, SWPCI=0, SWPCIM=0b00, LATMOD=0, TQPS=0, TQSS=0b000
        .PGxSPCIL.value = 0x0000,   // TSYNCDIS=0, TERM=0b000, AQPS=0, AQSS=0b000, SWTERM=0, PSYNC=0, PPS=0, PSS=0b00000
        .PGxSPCIH.value = 0x0000,   // BPEN=0, BPSEL=0b000, ACP0b000, SWPCI=0, SWPCIM=0b00, LATMOD=0, TQPS=0, TQSS=0b000
        .PGxLEBL.value = 0x0000,    // LEB=0
        .PGxLEBH.value = 0x0000,    // PWMPCI=0b000, PHR=0, PHF=0, PLR=0, PLF=0, 
        .PGxPHASE.value = 0x0000,   // PGxPHASE=0
        .PGxDC.value = 0x0000,      // PGxDC=0
        .PGxDCA.value = 0x0000,     // PGxDCA=0
        .PGxPER.value = 0x0000,     // PGxPER=0
        .PGxTRIGA.value = 0x0000,   // PGxTRIGA=0
        .PGxTRIGB.value = 0x0000,   // PGxTRIGB=0
        .PGxTRIGC.value = 0x0000,   // PGxTRIGC=0
        .PGxDTL.value = 0x0000,     // DTL=0
        .PGxDTH.value = 0x0000,     // DTH=0
        .PGxCAP.value = 0x0000      // CAP=0
    };



// end of file

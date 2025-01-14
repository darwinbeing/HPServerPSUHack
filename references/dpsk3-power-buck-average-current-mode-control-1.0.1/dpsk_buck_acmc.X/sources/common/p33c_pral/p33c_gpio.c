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

#include "p33c_gpio.h"

/*********************************************************************************
 * @fn struct P33C_GPIO_INSTANCE_s* p33c_GpioInstance_GetHandle(volatile uint16_t gpioInstance)
 * @ingroup lib-layer-pral-functions-public-gpio
 * @brief Gets pointer to GPIO Instance SFR set
 * @param gpioInstance Index of the selected GPIO Instance (1=Port A, 2=Port B, etc.)
 * @return Pointer to GPIO instance object of type struct P33C_GPIO_INSTANCE_s of the selected GPIO instance
 *  
 * @details
 *    This function returns the pointer to a GPIO instance register set in 
 *    Special Function Register memory space. This pointer can be used to directly
 *    write to/read from the Special Function Registers of a given peripheral
 *    instance.
 * 
 *********************************************************************************/

volatile struct P33C_GPIO_INSTANCE_s* p33c_GpioInstance_GetHandle(volatile uint16_t gpioInstance)
{
    volatile struct P33C_GPIO_INSTANCE_s* gpio;
    
    // Capture Handle: set pointer to memory address of desired GPIO instance
    gpio = (volatile struct P33C_GPIO_INSTANCE_s*) 
        ((volatile uint8_t*) &ANSELA + (gpioInstance * P33C_GPIO_SFR_OFFSET));
    
    return(gpio);
}

/*********************************************************************************
 * @fn uint16_t p33c_GpioInstance_Dispose(volatile uint16_t gpioInstance)
 * @ingroup lib-layer-pral-functions-public-gpio
 * @brief Resets all GPIO Instance registers to their RESET default values
 * @param  gpioInstance Index of the selected GPIO Instance (1=Port A, 2=Port B, etc.)
 * @return 0 = failure, disposing GPIO instance was not successful
 * @return 1 = success, disposing GPIO instance was successful
 * 
 * @details
 *     This function clears all GPIO Instance registers to their
 *     default values set when the device comes out of RESET. 
 * 
 *     Default configuration:
 *         - all outputs are set to logic functions
 *         - all analog functions are disabled
 *         - all pull-up and pull-down resistors are disabled
 *         - all GPIOs are operating in push-pull mode (open drain disabled)
 *         - all GPIOs are configured as input with their signal level HIGH
 * 
 *********************************************************************************/

volatile uint16_t p33c_GpioInstance_Dispose(volatile uint16_t gpioInstance)
{
    volatile uint16_t retval=1;
    
    retval = p33c_GpioInstance_ConfigWrite(gpioInstance, gpioConfigClear);
    
    return(retval);
}

/*********************************************************************************
 * @fn struct P33C_GPIO_INSTANCE_s p33c_GpioInstance_ConfigRead(volatile uint16_t gpioInstance)
 * @ingroup lib-layer-pral-functions-public-gpio
 * @brief Read the current configuration from the GPIO instance registers  
 * @param  gpioInstance Index of the selected GPIO Instance (1=Port A, 2=Port B, etc.)
 * @return 0 = failure, reading GPIO instance was not successful (returns NULL)
 * @return n = success, reading GPIO instance was successful (returns 16-bit wide pointer)
 * 
 * @details
 *     This function reads all registers with their current configuration into
 *     a data structure of type P33C_GPIO_INSTANCE_s. Users can read and 
 *     verify or modify the configuration to write it back to the GPIO instance  
 *     registers or copy configurations to other instances of the same type.
 * 
 * ********************************************************************************/

volatile struct P33C_GPIO_INSTANCE_s p33c_GpioInstance_ConfigRead(volatile uint16_t gpioInstance)
{
    volatile struct P33C_GPIO_INSTANCE_s* gpio;

    // Set pointer to memory address of desired GPIO instance
    gpio = (volatile struct P33C_GPIO_INSTANCE_s*) 
        ((volatile uint8_t*) &ANSELA + (gpioInstance * P33C_GPIO_SFR_OFFSET));

    return(*gpio);
    
}

/*********************************************************************************
 * @fn uint16_t p33c_GpioInstance_ConfigWrite(volatile uint16_t gpioInstance,volatile struct P33C_GPIO_INSTANCE_s gpioConfig)
 * @ingroup lib-layer-pral-functions-public-gpio
 * @brief  Writes a user-defined configuration to the GPIO instance registers
 * @param  gpioInstance Index of the selected GPIO Instance (1=Port A, 2=Port B, etc.)
 * @param  gpioConfig GPIO peripheral instance SFR object of type struct P33C_GPIO_INSTANCE_s
 * @return 0 = failure, writing GPIO instance was not successful
 * @return 1 = success, writing GPIO instance was successful
 * 
 * @details
 *     This function writes a user-defined GPIO instance configuration of type 
 *     P33C_GPIO_INSTANCE_s to the GPIO instance registers. The 
 *     individual register configurations have to be set in user-code 
 *     before calling this function. To simplify the configuration process
 *     of standard functions, this driver provides templates, which can be 
 *     loaded and written directly.
 * 
 *********************************************************************************/

volatile uint16_t p33c_GpioInstance_ConfigWrite(
        volatile uint16_t gpioInstance, 
        volatile struct P33C_GPIO_INSTANCE_s gpioConfig
)
{
    volatile uint16_t retval=1;
    volatile struct P33C_GPIO_INSTANCE_s* gpio;    

    // Set pointer to memory address of desired GPIO instance
    gpio = (volatile struct P33C_GPIO_INSTANCE_s*) 
        ((volatile uint8_t*) &ANSELA + (gpioInstance * P33C_GPIO_SFR_OFFSET));
    *gpio = gpioConfig;
    
    return(retval);
    
}


/* ============================================================================== */
/* ============================================================================== */
/* ============================================================================== */

/*********************************************************************************
 * @var gpioConfigClear 
 * @ingroup lib-layer-pral-properties-private-gpio
 * @brief Default RESET configuration of one GPIO instance SFRs
 *
 * @details
 *   Default configuration of the GPIO instance SFRs with all its registers 
 *   being reset to their default state when the device comes out of RESET.
 *   Programmers can use this template to reset (dispose) a previously used
 *   GPIO instance when it's not used anymore or to secure a known startup
 *   condition before writing individual configurations to its SFRs.
 * 
 ********************************************************************************/

volatile struct P33C_GPIO_INSTANCE_s gpioConfigClear = {
    
        .ANSELx.value = 0x0000, // Disable all analog functions
        .CNCONx.value = 0x0000, // Reset all change notification configurations
        .CNEN0x.value = 0x0000, // Disable all change notification functions
        .CNEN1x.value = 0x0000, // Disable all change notification functions
        .CNFx.value = 0x0000, // Clear all change notification interrupt flags
        .CNPDx.value = 0x0000, // Disable all dull-down resistors
        .CNPUx.value = 0x0000, // Disable all dull-up resistors
        .CNSTATx.value = 0x0000, // Clear all change notification status notifications
        .LATx.value = 0x0000, // Set all IOs of selected instance LOW
        .ODCx.value = 0x0000, // Clear all open-drain configurations of instance
        .PORTx.value = 0x0000, // CLear port registers of all IOs of instance 
        .TRISx.value = 0x1111, // Set all IOs of instance to INPUT
        
    };


// end of file

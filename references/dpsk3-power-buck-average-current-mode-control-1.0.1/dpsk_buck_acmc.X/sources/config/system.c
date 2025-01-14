/*
 * File:   system_initialize.c
 * Author: M91406
 *
 * Created on November 12, 2020, 10:33 AM
 */

#include "apps.h"
#include "hal.h"
#include "system.h"

/***********************************************************************************
 * @fn uint16_t SYSTEM_Initialize(void) 
 * @ingroup mcu-initialization
 * @brief  Initializes essential chip resources
 * @return unsigned integer 
 * 0=failure
 * 1=success
 * 
 * @details
 * The SYSTEM_Initialize function covers the initialization of essential chip 
 * resources such as main oscillator, auxiliary oscillator, watchdog timer and
 * general purpose I/Os (GPIO). All other, design specific peripherals are 
 * initialized in the User Peripheral Initialization or by the respective 
 * User Task Device Drivers included in the firmware project
 * 
 **********************************************************************************/
volatile uint16_t SYSTEM_Initialize(void) 
{
    volatile uint16_t retval=1;
	
    retval &= sysFosc_Initialize(); ///< Set up system oscillator for 100 MIPS operation
    retval &= sysAclk_Initialize(); ///< Set up Auxiliary PLL for 500 MHz (source clock to PWM module)
    retval &= sysGpio_Initialize(); ///< Initialize common device GPIOs
    retval &= sysDsp_Initialize(); ///< Initialize the DSP engine for fractional multiplication with saturation
    
	return(retval);

}

/***********************************************************************************
 * @fn uint16_t sysUserPeriperhals_Initialize(void)
 * @ingroup user-peripherals-initialization
 * @brief  Initializes the user-defined chip resources
 * @return unsigned integer (0=failure, 1=success)
 * 
 * @details
 * The Digital Power Starter Kit 3 supports a Test Point allowing to observe
 * the DAC output of the dsPIC33C device. Using this feature requires the 
 * configuration of further on-chip resources. This configuration is static
 * and not related to any other task or function of the application and therefore
 * needs to be added and placed manually. 
 * For this kind of Special Features, The startup procedure offers the following 
 * default function call allowing to place proprietary user code for individual
 * device configurations beyond the default setup. 
 **********************************************************************************/
volatile uint16_t sysUserPeriperhals_Initialize(void) {

    volatile uint16_t retval=1;
    
    // Reset the operation amplifier module to a disabled default state.
    retval &= sysOpAmp_ModuleReset();
    
    // Initialize op-amp
    retval &= sysOpAmp_Initialize(DAC_BUFFER_OPA_INSTANCE, true); // Initialize op-amp #2 used to drive the reference voltage for current sense amplifiers
    
    // Initialize DAC
    retval &= sysDacModule_Initialize();  // Initialize DAC module
    retval &= sysDacOutput_Initialize(DAC_OUTPUT_INSTANCE, DAC_OUTPUT_INIT_VALUE); // Initialize DAC #1 used to generate the reference voltage for current sense amplifiers
    retval &= sysDacOutput_Enable(DAC_OUTPUT_INSTANCE); // Enable DAC providing reference to current sense amplifiers

    // Enable op-amp
    retval &= sysOpAmp_ModuleEnable(); // Enable the operational amplifier module
    
    // Initialize debugging Pins
    #ifdef DBGPIN1_PIN
    DBGPIN1_Init(); ///< Device pin #1  (not routed)
    DBGPIN1_Clear(); // Clear debug pin #1
    #endif
    #ifdef DBGPIN2_PIN
    DBGPIN2_Init(); ///< Device pin #2  (not routed)
    DBGPIN2_Clear(); // Clear debug pin #2
    #endif
    
	return(retval);

}

/***********************************************************************************
 * @fn uint16_t sysUserTasks_Initialize
 * @ingroup user-task-initialization
 * @brief  Initializes the user-defined tasks
 * @return unsigned integer (0=failure, 1=success)
 * 
 * @details
 * The EPC9143 16th brick power module reference design has a very simple GPIO 
 * user interface, signaling the state of the output regulation on a POWER GOOD
 * output pin. Hence, this firmware4 mainly consists of the power control state
 * machine and fault handler, protecting the hardware.
 *
 **********************************************************************************/
volatile uint16_t sysUserTasks_Initialize(void) {

    volatile uint16_t retval=1;
	
    // Initialize task scheduler time base
    retval &= sysOsTimer_Initialize();     // Set up Timer1 as scheduler time base (see MAIN_EXECUTION_PERIOD for details)

    // Initialize software modules
    retval &= appLCD_Initialize(); // Initialize LC Display task
    retval &= appLED_Initialize(); // Initialize Debugging LED task
    retval &= appPushButton_Initialize(); // Initialize user switch button
    retval &= appPowerSupply_Initialize(); // Initialize BUCK converter object and state machine
    retval &= appFaultMonitor_Initialize(); // Initialize fault objects and fault handler task
    
	return(retval);

}

// end of file

/*
 * File:   app_power_config.c
 * Author: M91406
 *
 * Created on January 13, 2021, 12:18 PM
 */


#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

#include "config/hal.h" // include hardware abstraction layer declarations
#include "app_power_control.h" // include applicaiton layer of power control header

#include "devices/dev_buck_converter.h" // include the buck covnerter device driver
#include "drivers/v_loop.h" // include voltage mode control feedback loop object header
#include "drivers/i_loop.h" // include current mode control feedback loop object header

/*******************************************************************************
 * @ingroup app-layer-power-control-functions-private
 * @fn	volatile uint16_t appPowerSupply_ConverterObjectInitialize(void)
 * @brief  This function initializes the buck converter device driver instance
 * @return unsigned integer (0=failure, 1=success)
 *  
 * @details
 *  This function initialize the buck converter object status, reset the buck state 
 *  machine, set reference values, clear the runtime data, initialize the switch node,
 *  and setup the feedback channels and start-up settings.
 *********************************************************************************/

volatile uint16_t appPowerSupply_ConverterObjectInitialize(void)
{
    volatile uint16_t retval = 1;
    
    // Initialize Buck Converter Object Status
    buck.status.bits.ready = false; // Clear READY flag
    buck.status.bits.adc_active = false; // Clear ADC STARTED flag
    buck.status.bits.pwm_active = false; // clear PWM STARTED flag
    buck.status.bits.fault_active = true; // Set global FAULT flag
    
    buck.status.bits.cs_calib_enable = BUCK_ISNS_OFFSET_CALIBRATION_ENABLE; // Disable current sense feedback calibration
    buck.status.bits.async_mode = false; // Start up converter in synchronous mode
    buck.status.bits.autorun = true;    // Allow the buck converter to start automatically when cleared of faults
    buck.status.bits.enabled = false; // Disable buck converter
 
    // Set Initial State Machine State
    buck.state_id.value = BUCK_OPSTATE_INITIALIZE; // Reset Buck State Machine
    
    // Set Reference values
    buck.set_values.control_mode = BUCK_CONTROL_MODE_ACMC; // Set Control Mode
    buck.set_values.no_of_phases = BUCK_NO_OF_PHASES; // Set number of power train phases
    buck.set_values.i_ref = BUCK_ISNS_REF; // Set current loop reference
    buck.set_values.v_ref = BUCK_VOUT_REF; // Set voltage loop reference
    
    // Clear Runtime Data
    buck.data.v_out = 0; // Reset output voltage value
    buck.data.i_sns[0] = 0; // Reset output current value
    buck.data.v_in = 0;  // Reset input voltage value
    buck.data.temp = 0;  // Reset output temperature value
    
    // Initialize Switch Node
    buck.sw_node[0].pwm_instance = BUCK_PWM_CHANNEL;
    buck.sw_node[0].gpio_instance = BUCK_PWM_GPIO_INSTANCE;
    buck.sw_node[0].gpio_high = BUCK_PWM_GPIO_PORT_PINH;
    buck.sw_node[0].gpio_low = BUCK_PWM_GPIO_PORT_PINL;
    buck.sw_node[0].swap_outputs = BUCK_PWM_OUTPUT_SWAP;
    buck.sw_node[0].master_period_enable = false;
    buck.sw_node[0].sync_drive = true; 
    buck.sw_node[0].period = BUCK_PWM_PERIOD;
    buck.sw_node[0].phase = BUCK_PWM_PHASE_SHIFT;
    buck.sw_node[0].duty_ratio_min = BUCK_PWM_DC_MIN;
    buck.sw_node[0].duty_ratio_init = BUCK_PWM_DC_MIN;
    buck.sw_node[0].duty_ratio_max = BUCK_PWM_DC_MAX;
    buck.sw_node[0].dead_time_rising = BUCK_PWM_DEAD_TIME_LE;
    buck.sw_node[0].dead_time_falling = BUCK_PWM_DEAD_TIME_FE;
    buck.sw_node[0].leb_period = BUCK_LEB_PERIOD;
    buck.sw_node[0].trigger_offset = BUCK_PWM_ADTR1OFS;
    buck.sw_node[0].trigger_scaler = BUCK_PWM_ADTR1PS;
    buck.sw_node[0].high_resolution_enable = PWM_CLOCK_HIGH_RESOLUTION;

    // Initialize additional GPIOs 
    
    // ~~~ EXTERNAL ENABLE INPUT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    buck.gpio.EnableInput.enabled = false; // this converter doesn't support external enable control
    // ~~~ EXTERNAL ENABLE INPUT END ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // ~~~ POWER GOOD OUTPUT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    buck.gpio.PowerGood.enabled = true; // This converter supports an additional POWER GOOD output
    buck.gpio.PowerGood.port = PWRGOOD_PORT; // Number of the GPIO port (0=A, 1=B, 2=C, etc.)
    buck.gpio.PowerGood.pin = PWRGOOD_PIN; // Number of the GPIO port pin
    buck.gpio.PowerGood.polarity = 0;   // This pin is ACTIVE HIGH (only required if io_type = OUTPUT)
    buck.gpio.PowerGood.io_type = 0;    // This pin is configured as Push-Pull OUTPUT
    
    // ~~~ POWER GOOD OUTPUT END ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    // Initialize Feedback Channels
    
    // ~~~ OUTPUT VOLTAGE FEEDBACK ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    buck.feedback.ad_vout.enabled = true;   // Use this channel

    buck.feedback.ad_vout.adc_input = BUCK_VOUT_ADCIN;
    buck.feedback.ad_vout.adc_core = BUCK_VOUT_ADCCORE;
    buck.feedback.ad_vout.adc_buffer = &BUCK_VOUT_ADCBUF;
    buck.feedback.ad_vout.trigger_source = BUCK_VOUT_TRGSRC;

    buck.feedback.ad_vout.differential_input = false;
    buck.feedback.ad_vout.interrupt_enable = true;
    buck.feedback.ad_vout.early_interrupt_enable = true;
    buck.feedback.ad_vout.level_trigger = true;
    buck.feedback.ad_vout.signed_result = false;
    
    buck.feedback.ad_vout.scaling.factor = BUCK_VOUT_NORM_FACTOR;
    buck.feedback.ad_vout.scaling.scaler = BUCK_VOUT_NORM_SCALER;
    buck.feedback.ad_vout.scaling.offset = BUCK_VOUT_OFFSET;

    BUCK_VOUT_ANSEL = buck.feedback.ad_vout.enabled;
    
    // ~~~ OUTPUT VOLTAGE FEEDBACK END ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // ~~~ INPUT VOLTAGE FEEDBACK ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    buck.feedback.ad_vin.enabled = true;   // Use this channel

    buck.feedback.ad_vin.adc_input = BUCK_VIN_ADCIN;
    buck.feedback.ad_vin.adc_core = BUCK_VIN_ADCCORE;
    buck.feedback.ad_vin.adc_buffer = &BUCK_VIN_ADCBUF;
    buck.feedback.ad_vin.trigger_source = BUCK_VIN_TRGSRC;

    buck.feedback.ad_vin.differential_input = false;
    buck.feedback.ad_vin.interrupt_enable = false;
    buck.feedback.ad_vin.early_interrupt_enable = false;
    buck.feedback.ad_vin.level_trigger = false;
    buck.feedback.ad_vin.signed_result = false;

    buck.feedback.ad_vin.scaling.factor = BUCK_VIN_NORM_FACTOR;
    buck.feedback.ad_vin.scaling.scaler = BUCK_VIN_NORM_SCALER;
    buck.feedback.ad_vin.scaling.offset = BUCK_VIN_OFFSET;

    BUCK_VIN_ANSEL = buck.feedback.ad_vin.enabled;
    
    // ~~~ INPUT VOLTAGE FEEDBACK END ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // ~~~ OUTPUT CURRENT FEEDBACK ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    buck.feedback.ad_isns[0].enabled = true;   // Use this channel

    buck.feedback.ad_isns[0].adc_input = BUCK_ISNS_ADCIN;
    buck.feedback.ad_isns[0].adc_core = BUCK_ISNS_ADCCORE;
    buck.feedback.ad_isns[0].adc_buffer = &BUCK_ISNS_ADCBUF;
    buck.feedback.ad_isns[0].trigger_source = BUCK_ISNS_TRGSRC;

    buck.feedback.ad_isns[0].differential_input = false;
    buck.feedback.ad_isns[0].interrupt_enable = false;
    buck.feedback.ad_isns[0].early_interrupt_enable = false;
    buck.feedback.ad_isns[0].level_trigger = true;
    buck.feedback.ad_isns[0].signed_result = false;

    buck.feedback.ad_isns[0].scaling.factor = BUCK_ISNS_NORM_FACTOR;
    buck.feedback.ad_isns[0].scaling.scaler = BUCK_ISNS_NORM_SCALER;
    buck.feedback.ad_isns[0].scaling.offset = BUCK_ISNS_FB_OFFSET;

    BUCK_ISNS_ANSEL = buck.feedback.ad_isns[0].enabled;
    
    // ~~~ OUTPUT CURRENT FEEDBACK END ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // ~~~ TEMPERATURE FEEDBACK ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    buck.feedback.ad_temp.enabled = true;   // Use this channel

    buck.feedback.ad_temp.adc_input = TEMP_ADCIN;
    buck.feedback.ad_temp.adc_core = TEMP_ADCCORE;
    buck.feedback.ad_temp.adc_buffer = &TEMP_ADCBUF;
    buck.feedback.ad_temp.trigger_source = TEMP_TRGSRC;

    buck.feedback.ad_temp.differential_input = false;
    buck.feedback.ad_temp.interrupt_enable = false;
    buck.feedback.ad_temp.early_interrupt_enable = false;
    buck.feedback.ad_temp.level_trigger = false;
    buck.feedback.ad_temp.signed_result = false;

    TEMP_ANSEL = buck.feedback.ad_temp.enabled;
    
    // ~~~ TEMPERATURE FEEDBACK END ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Initialize Startup Settings
    
    buck.startup.power_on_delay.counter = 0;
    buck.startup.power_on_delay.period = BUCK_POD;
    buck.startup.power_on_delay.ref_inc_step = 0;
    buck.startup.power_on_delay.reference = 0;
    
    buck.startup.v_ramp.counter = 0;
    buck.startup.v_ramp.period = BUCK_VRAMP_PER;
    buck.startup.v_ramp.ref_inc_step = BUCK_VREF_STEP;
    if (buck.startup.v_ramp.ref_inc_step == 0)
        buck.startup.v_ramp.ref_inc_step = 1;
    buck.startup.v_ramp.reference = 0;
    
    if (buck.set_values.control_mode == BUCK_CONTROL_MODE_ACMC) 
    {
        buck.startup.i_ramp.counter = 0;
        buck.startup.i_ramp.period = BUCK_IRAMP_PER;
        buck.startup.i_ramp.ref_inc_step = BUCK_IREF_STEP;
        buck.startup.i_ramp.reference = 0;
    }
    
    buck.startup.power_good_delay.counter = 0;
    buck.startup.power_good_delay.period = BUCK_PGD;
    buck.startup.power_good_delay.ref_inc_step = 0;
    buck.startup.power_good_delay.reference = BUCK_VOUT_REF;
    
    
    return(retval);
}


/*******************************************************************************
 * @ingroup app-layer-power-control-functions-private
 * @fn	volatile uint16_t appPowerSupply_ControllerInitialize(void)
 * @brief  This function initializes the control system feedback loop objects
 * @return 0=failure
 * @return 1=success
 * 
 * @details
 * This function allows the user to set up and initialize the loop configuration. This 
 * includes the following setup.
 *     - Initialize Default Loop Configuration
 *     - Set Controller Object of Voltage Loop
 *     - Configure Voltage Loop Controller Object
 *     - Configure controller input/output ports
 *     - Data Input/Output Limit Configuration
 *     - ADC Trigger Control Configuration
 *     - Data Provider Configuration
 *     - Cascaded Function Configuration
 *     - Initialize Advanced Control Settings 
 *     - Custom Advanced Control Settings
 *********************************************************************************/

volatile uint16_t appPowerSupply_ControllerInitialize(void)
{
    volatile uint16_t retval = 1;
    
    // ~~~ VOLTAGE LOOP CONFIGURATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // Initialize Default Loop Configuration
    buck.v_loop.feedback_offset = BUCK_VOUT_OFFSET;
    buck.v_loop.reference = BUCK_VOUT_REF;
    buck.v_loop.minimum = BUCK_ISNS_MIN;
    buck.v_loop.maximum = BUCK_ISNS_OCL;
    
    // Move trigger point 400 ns further in to leave enough room for the AGC computation
    buck.v_loop.trigger_offset = ((BUCK_PWM_PERIOD >> 1) + BUCK_VOUT_ADC_TRGDLY) - BUCK_AGC_EXEC_DLY; 
    
     // Set Controller Object of Voltage Loop
    buck.v_loop.controller = &v_loop;
    buck.v_loop.ctrl_Initialize = &v_loop_Initialize;
    buck.v_loop.ctrl_Update = &v_loop_Update;
    buck.v_loop.ctrl_Reset = &v_loop_Reset;
    buck.v_loop.ctrl_Precharge = &v_loop_Precharge;
    
    // Configure Voltage Loop Controller Object
    buck.v_loop.ctrl_Initialize(&v_loop);   // Call Initialization Routine setting histories and scaling
    
    // Configure controller input ports
    buck.v_loop.controller->Ports.Source.ptrAddress = &BUCK_VOUT_ADCBUF; // Output Voltage is Common Source
    buck.v_loop.controller->Ports.Source.Offset = buck.v_loop.feedback_offset; // Output Voltage feedback signal offset 
    buck.v_loop.controller->Ports.Source.NormScaler = BUCK_VOUT_NORM_SCALER; // Output voltage normalization factor bit-shift scaler 
    buck.v_loop.controller->Ports.Source.NormFactor = BUCK_VOUT_NORM_FACTOR; // Output voltage normalization factor fractional
    
    buck.v_loop.controller->Ports.AltSource.ptrAddress = &BUCK_VIN_ADCBUF; // Input Voltage Is Alternate Source
    buck.v_loop.controller->Ports.AltSource.Offset = BUCK_VIN_OFFSET; // Input Voltage feedback signal offset 
    buck.v_loop.controller->Ports.AltSource.NormScaler = BUCK_VIN_NORM_SCALER; // Input voltage normalization factor bit-shift scaler 
    buck.v_loop.controller->Ports.AltSource.NormFactor = BUCK_VIN_NORM_FACTOR; // Input voltage normalization factor fractional

    // Configure controller output ports
    buck.v_loop.controller->Ports.Target.ptrAddress = &buck.i_loop[0].reference; // Current loop reference is Control Target
    buck.v_loop.controller->Ports.Target.Offset = 0; // Static primary output value offset
    buck.v_loop.controller->Ports.Target.NormScaler = 0; // Primary control output normalization factor bit-shift scaler 
    buck.v_loop.controller->Ports.Target.NormFactor = 0x7FFF; // Primary control output normalization factor fractional 

    buck.v_loop.controller->Ports.AltTarget.ptrAddress = NULL; // No alternate target used
    buck.v_loop.controller->Ports.AltTarget.Offset = 0; // Static secondary output value offset
    buck.v_loop.controller->Ports.AltTarget.NormScaler = 0; // Secondary control output normalization factor bit-shift scaler
    buck.v_loop.controller->Ports.AltTarget.NormFactor = 0x7FFF; // Secondary control output normalization factor fractional 
    
    // Configure controller control ports
    buck.v_loop.controller->Ports.ptrControlReference = &buck.v_loop.reference; // Set pointer to Reference
    
    // Data Input/Output Limit Configuration
    buck.v_loop.controller->Limits.MinOutput = buck.v_loop.minimum;
    buck.v_loop.controller->Limits.MaxOutput = buck.v_loop.maximum;
    buck.v_loop.controller->Limits.AltMinOutput = 0; // not used
    buck.v_loop.controller->Limits.AltMaxOutput = 0; // not used

    // ADC Trigger Control Configuration (ADC trigger controlled by current loop)
    buck.v_loop.controller->ADCTriggerControl.ptrADCTriggerARegister = NULL;
    buck.v_loop.controller->ADCTriggerControl.ADCTriggerAOffset = 0;
    buck.v_loop.controller->ADCTriggerControl.ptrADCTriggerBRegister = NULL;
    buck.v_loop.controller->ADCTriggerControl.ADCTriggerBOffset = 0; 
    
    // Data Provider Configuration
    buck.v_loop.controller->DataProviders.ptrDProvControlInput = &buck.data.control_input; 
    buck.v_loop.controller->DataProviders.ptrDProvControlInputCompensated = &buck.data.v_out; 
    buck.v_loop.controller->DataProviders.ptrDProvControlError = &buck.data.control_error; 
    buck.v_loop.controller->DataProviders.ptrDProvControlOutput = &buck.data.control_output;
    
    // User Extension Function Configuration
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * 
     * PowerSmart DCLD allows users to create and call user extension 
     * functions from specific locations of the main control loop to 
     * cover design-specific requirements and features which are not
     * supported by the main controller by default.
     * 
     * Control Loop User Extension Declaration Example:
     * 
     *  buck.v_loop.controller->ExtensionHooks.ptrExtHookStartFunction = (uint16_t)&my_function; 
     * 
     * Control Loop User Extension Parameter Declaration Example (optional):
     * 
     *  buck.v_loop.controller->ExtensionHooks.ExtHookStartFunctionParam = 512;
     * 
     * Please refer to the PowerSmart DCLD User Guide for more details about
     * how to use this feature, its requirements and limitations.
     * 
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    */
    
    buck.v_loop.controller->ExtensionHooks.ptrExtHookStartFunction = NULL;
    buck.v_loop.controller->ExtensionHooks.ExtHookStartFunctionParam = 0;
    buck.v_loop.controller->ExtensionHooks.ptrExtHookSourceFunction = NULL;
    buck.v_loop.controller->ExtensionHooks.ExtHookSourceFunctionParam = 0;
    buck.v_loop.controller->ExtensionHooks.ptrExtHookPreAntiWindupFunction = NULL;
    buck.v_loop.controller->ExtensionHooks.ExtHookPreAntiWindupFunctionParam = 0;
    buck.v_loop.controller->ExtensionHooks.ptrExtHookPreTargetWriteFunction = NULL;
    buck.v_loop.controller->ExtensionHooks.ExtHookPreTargetWriteFunctionParam = 0;
    buck.v_loop.controller->ExtensionHooks.ptrExtHookEndOfLoopFunction = NULL;
    buck.v_loop.controller->ExtensionHooks.ExtHookEndOfLoopFunctionParam = 0;
    buck.v_loop.controller->ExtensionHooks.ptrExtHookExitFunction = (uint16_t)&i_loop_Update;
    buck.v_loop.controller->ExtensionHooks.ExtHookExitFunctionParam = (uint16_t)&i_loop;
    
    // Adaptive Gain Control configuration
    buck.v_loop.controller->GainControl.AgcFactor = 0;
    buck.v_loop.controller->GainControl.AgcScaler = 0;
    buck.v_loop.controller->GainControl.AgcMedian = 0;

    buck.v_loop.controller->GainControl.ptrAgcObserverFunction = NULL;

    // Custom Advanced Control Settings
    buck.v_loop.controller->Advanced.usrParam0 = 0; // No additional advanced control options used
    buck.v_loop.controller->Advanced.usrParam1 = 0; // No additional advanced control options used
    buck.v_loop.controller->Advanced.usrParam2 = 0; // No additional advanced control options used
    buck.v_loop.controller->Advanced.usrParam3 = 0; // No additional advanced control options used
    buck.v_loop.controller->Advanced.usrParam4 = 0; // No additional advanced control options used
    buck.v_loop.controller->Advanced.usrParam5 = 0; // No additional advanced control options used
    buck.v_loop.controller->Advanced.usrParam6 = 0; // No additional advanced control options used
    buck.v_loop.controller->Advanced.usrParam7 = 0; // No additional advanced control options used
    
    // Reset Controller Status
    buck.v_loop.controller->status.bits.enabled = false; // Keep controller disabled
    buck.v_loop.controller->status.bits.swap_source = false; // use SOURCE as major control input
    buck.v_loop.controller->status.bits.swap_target = false; // use TARGET as major control output
    buck.v_loop.controller->status.bits.invert_input = false; // Do not invert input value
    buck.v_loop.controller->status.bits.lower_saturation_event = false; // Reset Anti-Windup Minimum Status bit
    buck.v_loop.controller->status.bits.upper_saturation_event = false; // Reset Anti-Windup Minimum Status bits
    buck.v_loop.controller->status.bits.agc_enabled = false;   // Enable Adaptive Gain Modulation by default

    // ~~~ VOLTAGE LOOP CONFIGURATION END ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    

    // ~~~ CURRENT LOOP CONFIGURATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // Initialize Default Loop Configuration
    buck.i_loop[0].feedback_offset = BUCK_ISNS_FB_OFFSET;
    buck.i_loop[0].reference = BUCK_ISNS_REF;
    buck.i_loop[0].minimum = BUCK_PWM_DC_MIN;
    buck.i_loop[0].maximum = BUCK_PWM_DC_MAX;
    
    // Move trigger point to compensate for propagation delays
    buck.i_loop[0].trigger_offset = BUCK_ISNS_ADC_TRGDLY; 
    
     // Set Controller Object of Current Loop
    buck.i_loop[0].controller = &i_loop;
    buck.i_loop[0].ctrl_Initialize = &i_loop_Initialize;
    buck.i_loop[0].ctrl_Update = &i_loop_Update;
    buck.i_loop[0].ctrl_Reset = &i_loop_Reset;
    buck.i_loop[0].ctrl_Precharge = &i_loop_Precharge;
    
    // Configure Voltage Loop Controller Object
    buck.i_loop[0].ctrl_Initialize(&i_loop);   // Call Initialization Routine setting histories and scaling
    
    // Configure controller input ports
    buck.i_loop[0].controller->Ports.Source.ptrAddress = &BUCK_ISNS_ADCBUF; // Inductor Current is Common Source
    buck.i_loop[0].controller->Ports.Source.Offset = buck.i_loop[0].feedback_offset; // Inductor Current feedback signal offset 
    buck.i_loop[0].controller->Ports.Source.NormScaler = BUCK_VOUT_NORM_SCALER; // Inductor Current normalization factor bit-shift scaler 
    buck.i_loop[0].controller->Ports.Source.NormFactor = BUCK_VOUT_NORM_FACTOR; // Inductor Current normalization factor fractional
    
    buck.i_loop[0].controller->Ports.AltSource.ptrAddress = NULL; // No alternate source used 
    buck.i_loop[0].controller->Ports.AltSource.Offset = 0; // Static secondary input value offset
    buck.i_loop[0].controller->Ports.AltSource.NormScaler = 0; // Secondary control input normalization factor bit-shift scaler
    buck.i_loop[0].controller->Ports.AltSource.NormFactor = 0; // Secondary control input normalization factor fractional 

    // Configure controller output ports
    buck.i_loop[0].controller->Ports.Target.ptrAddress = &BUCK_PWM_PDC; // PWM Duty Cycle is Control Target
    buck.i_loop[0].controller->Ports.Target.Offset = 0; // Static primary output value offset
    buck.i_loop[0].controller->Ports.Target.NormScaler = 0; // Primary control output normalization factor bit-shift scaler 
    buck.i_loop[0].controller->Ports.Target.NormFactor = 0x7FFF; // Primary control output normalization factor fractional 

    buck.i_loop[0].controller->Ports.AltTarget.ptrAddress = NULL; // No alternate target used
    buck.i_loop[0].controller->Ports.AltTarget.Offset = 0; // Static secondary output value offset
    buck.i_loop[0].controller->Ports.AltTarget.NormScaler = 0; // Secondary control output normalization factor bit-shift scaler
    buck.i_loop[0].controller->Ports.AltTarget.NormFactor = 0x7FFF; // Secondary control output normalization factor fractional 
    
    // Configure controller control ports
    buck.i_loop[0].controller->Ports.ptrControlReference = &buck.i_loop[0].reference; // Set pointer to Reference
    
    // Data Input/Output Limit Configuration
    buck.i_loop[0].controller->Limits.MinOutput = buck.i_loop[0].minimum;
    buck.i_loop[0].controller->Limits.MaxOutput = buck.i_loop[0].maximum;
    buck.i_loop[0].controller->Limits.AltMinOutput = 0; // not used
    buck.i_loop[0].controller->Limits.AltMaxOutput = 0; // not used

    // ADC Trigger Control Configuration
    buck.i_loop[0].controller->ADCTriggerControl.ptrADCTriggerARegister = &BUCK_VOUT_ADCTRIG;
    buck.i_loop[0].controller->ADCTriggerControl.ADCTriggerAOffset = buck.v_loop.trigger_offset;
    buck.i_loop[0].controller->ADCTriggerControl.ptrADCTriggerBRegister = &BUCK_ISNS_ADCTRIG;
    buck.i_loop[0].controller->ADCTriggerControl.ADCTriggerBOffset = BUCK_ISNS_ADC_TRGDLY; 
    
    // Data Provider Configuration
    buck.i_loop[0].controller->DataProviders.ptrDProvControlInput = NULL; 
    buck.i_loop[0].controller->DataProviders.ptrDProvControlInputCompensated = &buck.data.i_sns[0]; 
    buck.i_loop[0].controller->DataProviders.ptrDProvControlError = NULL; 
    buck.i_loop[0].controller->DataProviders.ptrDProvControlOutput = NULL;
    
    // User Extension Function Configuration
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * 
     * PowerSmart DCLD allows users to create and call user extension 
     * functions from specific locations of the main control loop to 
     * cover design-specific requirements and features which are not
     * supported by the main controller by default.
     * 
     * Control Loop User Extension Declaration Example:
     * 
     *  buck.i_loop[0].controller->ExtensionHooks.ptrExtHookStartFunction = (uint16_t)&my_function; 
     * 
     * Control Loop User Extension Parameter Declaration Example (optional):
     * 
     *  buck.i_loop[0].controller->ExtensionHooks.ExtHookStartFunctionParam = 512;
     * 
     * Please refer to the PowerSmart DCLD User Guide for more details about
     * how to use this feature, its requirements and limitations.
     * 
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    */
    
    buck.i_loop[0].controller->ExtensionHooks.ptrExtHookStartFunction = NULL;
    buck.i_loop[0].controller->ExtensionHooks.ExtHookStartFunctionParam = 0;
    buck.i_loop[0].controller->ExtensionHooks.ptrExtHookSourceFunction = NULL;
    buck.i_loop[0].controller->ExtensionHooks.ExtHookSourceFunctionParam = 0;
    buck.i_loop[0].controller->ExtensionHooks.ptrExtHookPreAntiWindupFunction = NULL;
    buck.i_loop[0].controller->ExtensionHooks.ExtHookPreAntiWindupFunctionParam = 0;
    buck.i_loop[0].controller->ExtensionHooks.ptrExtHookPreTargetWriteFunction = NULL;
    buck.i_loop[0].controller->ExtensionHooks.ExtHookPreTargetWriteFunctionParam = 0;
    buck.i_loop[0].controller->ExtensionHooks.ptrExtHookEndOfLoopFunction = NULL;
    buck.i_loop[0].controller->ExtensionHooks.ExtHookEndOfLoopFunctionParam = 0;
    buck.i_loop[0].controller->ExtensionHooks.ptrExtHookExitFunction = NULL;
    buck.i_loop[0].controller->ExtensionHooks.ExtHookExitFunctionParam = 0;
    
    // Adaptive Gain Control configuration
    buck.i_loop[0].controller->GainControl.AgcFactor = 0;
    buck.i_loop[0].controller->GainControl.AgcScaler = 0;
    buck.i_loop[0].controller->GainControl.AgcMedian = 0;

    buck.i_loop[0].controller->GainControl.ptrAgcObserverFunction = NULL;

    // Custom Advanced Control Settings
    buck.i_loop[0].controller->Advanced.usrParam0 = 0; // No additional advanced control options used
    buck.i_loop[0].controller->Advanced.usrParam1 = 0; // No additional advanced control options used
    buck.i_loop[0].controller->Advanced.usrParam2 = 0; // No additional advanced control options used
    buck.i_loop[0].controller->Advanced.usrParam3 = 0; // No additional advanced control options used
    buck.i_loop[0].controller->Advanced.usrParam4 = 0; // No additional advanced control options used
    buck.i_loop[0].controller->Advanced.usrParam5 = 0; // No additional advanced control options used
    buck.i_loop[0].controller->Advanced.usrParam6 = 0; // No additional advanced control options used
    buck.i_loop[0].controller->Advanced.usrParam7 = 0; // No additional advanced control options used
    
    // Reset Controller Status
    buck.i_loop[0].controller->status.bits.enabled = false; // Keep controller disabled
    buck.i_loop[0].controller->status.bits.swap_source = false; // use SOURCE as major control input
    buck.i_loop[0].controller->status.bits.swap_target = false; // use TARGET as major control output
    buck.i_loop[0].controller->status.bits.invert_input = false; // Do not invert input value
    buck.i_loop[0].controller->status.bits.lower_saturation_event = false; // Reset Anti-Windup Minimum Status bit
    buck.i_loop[0].controller->status.bits.upper_saturation_event = false; // Reset Anti-Windup Minimum Status bits
    buck.i_loop[0].controller->status.bits.agc_enabled = false;   // Enable Adaptive Gain Modulation by default

    // ~~~ CURRENT LOOP CONFIGURATION END ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    
    return(retval);
}

/*******************************************************************************
 * @ingroup app-layer-power-control-functions-private
 * @fn	volatile uint16_t appPowerSupply_PeripheralsInitialize(void)
 * @brief  This function is used to load peripheral configuration templates from the power controller device driver
 * @return 0=failure
 * @return 1=success
 * 
 * @details
 * This function hand over the peripheral configuration to the buck converter driver
 *********************************************************************************/

volatile uint16_t appPowerSupply_PeripheralsInitialize(void)
{
    volatile uint16_t retval=1;
    
    retval &= drv_BuckConverter_Initialize(&buck);
    
    return(retval);
}


// end of file

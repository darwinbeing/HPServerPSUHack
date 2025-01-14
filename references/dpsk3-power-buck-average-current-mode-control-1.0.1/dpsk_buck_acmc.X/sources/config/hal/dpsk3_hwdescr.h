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

/* 
 * @file   dpsk3_hwdescr.h
 * Author: M91406
 * @brief  DPSK3 Hardware Descriptor header file
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DPSK3_HARDWARE_DESCRIPTOR_H
#define	DPSK3_HARDWARE_DESCRIPTOR_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types
#include <math.h> // include standard math functions library


/* DPSK3 BOARD CIRCUIT SEGMENTS */

#define INCLUDE_BUCK    true    // Include buck converter declarations
#define INCLUDE_BOOST   false   // Include boost converter declarations
#define INCLUDE_LCD     true    // Include LCD declarations

/**************************************************************************************************
 * @ingroup hardware-id-macro
 * @{
 * @brief Global macro identifying the hardware version supported by 
 * 
 * @details
 * This global macro is used to identify the hardware version supported by this hardware 
 * abstraction layer header file. 
 * 
 **************************************************************************************************/

#ifndef __DPSK3_R30__
  #define __DPSK3_R30__
#endif


/** @} */ // end of group hardware-id-macro

/**************************************************************************************************
 * @ingroup special-options
 * @{
 * @brief Global defines used to enable/disable special firmware options
 * 
 * @details
 * This section is used to enable/disable special options useful during development, debugging, 
 * troubleshooting the recent firmware or to tailor special functions of this application. 
 * 
 **************************************************************************************************/

/* CUSTOM RUNTIME OPTIONS */
#define PLANT_MEASUREMENT   false ///< If enabled, replaces the common voltage control loop by a simple P-control loop to perform measurements of the plant transfer function.

#define DBGPIN1_ENABLE      true ///< Enables debug pin indicating control loop execution timing
#define DBGPIN2_ENABLE      true ///< Enables debug pin indicating control loop execution timing

#define DBGDAC_ENABLE       true ///< Enables debug DAC output pin reproduing control loop output as analog signal

/** @} */ // end of group special-options

/**************************************************************************************************
 * @ingroup device-abstraction-settings
 * @{
 * @brief Fundamental microcontroller device settings
 * 
 * @details
 * This section is used to define device specific parameters like ADC reference and
 * resolution, main execution clock frequency and peripheral time base settings. 
 * All parameters are defined using physical quantities. 
 * 
 **************************************************************************************************/
#define CPU_FREQUENCY       (float)100000000.0  ///< CPU frequency in [Hz]

// ADC/DAC Reference and Resolution Settings    
#define ADC_REFERENCE       (float)3.300 ///< ADC reference voltage in [V]
#define ADC_RESOLUTION      (float)12.0  ///< ADC resolution in [bit]

// PWM/ADC Clock Settings   
#define PWM_CLOCK_HIGH_RESOLUTION   true ///< Enables/disables the PWM generator high resolution mode of 250 ps versus 2 ns

#if (PWM_CLOCK_HIGH_RESOLUTION)
#define PWM_CLOCK_FREQUENCY (float)4.0e+9 ///< PWM Clock Frequency in [Hz]
#else
#define PWM_CLOCK_FREQUENCY (float)500.0e+6 ///< PWM Clock Frequency in [Hz]
#endif

/** @} */ // end of group device-abstraction-settings

/**
 * @ingroup device-abstraction-macros
 * @{
 * @brief Conversion macros of fundamental microcontroller device settings
 * 
 * @details
 * This section is used to convert device specific parameters like ADC reference and
 * resolution, main execution clock frequency and peripheral time base settings, declared 
 * in physical quantities, into binary (integer) numbers to be written to variables and SFRs.
 */

#define CPU_TCY             (float)(1.0/CPU_FREQUENCY) ///< Instruction period
#define ADC_GRANULARITY     (float)(ADC_REFERENCE / pow(2.0, ADC_RESOLUTION)) ///< ADC granularity in [V/tick]
#define ADC_VALUE_MAX       (uint16_t) (pow(2.0, ADC_RESOLUTION) - 1.0) // DO NOT CHANGE
#define PWM_CLOCK_PERIOD    (float)(1.0/PWM_CLOCK_FREQUENCY) ///< PWM Clock Period in [sec]

/** @} */ // end of group device-abstraction-macros

/**************************************************************************************************
 * @ingroup state-machine-settings
 * @{
 * @brief Global state-machine user-settings
 * 
 * @details
 * This section is used to set, modify, enable or disable common state machine parameters
 * and features. 
 * 
 * (Please see individual settings description for detail) 
 * 
 **************************************************************************************************/

#define MAIN_EXECUTION_PERIOD   (float)100.0e-6 ///< main state machine function call period in [sec]

/** @} */ // end of group state-machine-settings ~~~~~~~~~~

/**
 * @ingroup state-machine-macros
 * @{
 * @brief Global state-machine user-settings conversion macros
 * 
 * @details
 * Conversion macros are used to convert user settings defined in physical quantities into 
 * binary (integer) numbers, which will be written to registers and variables and/or used in 
 * calculations throughout the firmware.
 */

#define MAIN_EXEC_PER           (uint16_t)((CPU_FREQUENCY * MAIN_EXECUTION_PERIOD)-1) // DO NOT CHANGE

/** @} */ // end of group state-machine-macros ~~~~~~~~~~

/**
 * @ingroup state-machine-mcal
 * @{
 * @brief Global state-machine peripheral assignments
 * 
 * @details
 * The main task scheduler time base required a timer interrupt to separate high-priority
 * from low-priority tasks. The high priority task timer interrupt is configured here.
 */

#define _OsTimerInterrupt      _T1Interrupt ///< Interrupt serivce routine label 
#define _OSTIMER_IP            _T1IP ///< interrupt priority register
#define _OSTIMER_IE            _T1IE ///< interrupt enable bit
#define _OSTIMER_IF            _T1IF ///< interrupt flag bit
#define _OSTIMER_PRIORITY       2    ///< interrupt priority (1 ... 7, default = 2)

/** @} */ // end of group state-machine-mcal ~~~~~~~~~~

/***************************************************************************************************
 * @ingroup circuit-gpio-mcal
 * @{
 * @brief Global abstraction labels of special circuit signal device pin assignments
 * 
 * @details
 * This section is used to define labels of hardware specific signals, which are directly 
 * assigned to specific device pins. These labels will be used throughout the code and remain
 * unchanged even if this firmware is migrated to another device or pin-out changes between
 * hardware revisions.
 * 
 **************************************************************************************************/

#if (DBGPIN1_ENABLE == true)    ///< Enable debugging pin TP50 on DPSK3
#define DBGPIN1_PORT        1   ///< GPIO port declaration where 0=Port RA, 0=Port RB, 0=Port RC, etc.
#define DBGPIN1_PIN         5   ///< GPIO port pin declaration where 0=Rx0, 1=Rx1, 2=Rx3, etc.
#define DBGPIN1_Get()       _RB5 ///< Macro instruction to set a pin state to logic HIGH
#define DBGPIN1_Set()       { _LATB5 = 1; } ///< Macro instruction to set a pin state to logic HIGH
#define DBGPIN1_Clear()     { _LATB5 = 0; } ///< Macro instruction to set a pin state to logic LOW
#define DBGPIN1_Toggle()    { _LATB5 ^= 1; } ///< Macro instruction to toggle most recent pin state
#define DBGPIN1_Init()      { _LATB5 = 0; _TRISB5 = 0; } ///< Macro instruction initializing the specified GPIO as output
#define DBGPIN1_Dispose()   { _LATB5 = 1; _TRISB5 = 1; } ///< Macro instruction initializing the specified GPIO as input
#endif

#if (DBGPIN2_ENABLE == true)    ///< Enable debugging pin TP52 on DPSK3
#define DBGPIN2_PORT        1   ///< GPIO port declaration where 0=Port RA, 0=Port RB, 0=Port RC, etc.
#define DBGPIN2_PIN         11  ///< GPIO port pin declaration where 0=Rx0, 1=Rx1, 2=Rx3, etc.
#define DBGPIN2_Get()       _RB11 ///< Macro instruction to set a pin state to logic HIGH
#define DBGPIN2_Set()       { _LATB11 = 1; } ///< Macro instruction to set a pin state to logic HIGH
#define DBGPIN2_Clear()     { _LATB11 = 0; } ///< Macro instruction to set a pin state to logic LOW
#define DBGPIN2_Toggle()    { _LATB11 ^= 1; } ///< Macro instruction to toggle most recent pin state
#define DBGPIN2_Init()      { _LATB11 = 0; _TRISB11 = 0; } ///< Macro instruction initializing the specified GPIO as output
#define DBGPIN2_Dispose()   { _LATB11 = 1; _TRISB11 = 1; } ///< Macro instruction initializing the specified GPIO as input
#endif

// Ref LED on DPSK3 (TP51)
#define DBGLED_PORT         1   ///< GPIO port declaration where 0=Port RA, 0=Port RB, 0=Port RC, etc.
#define DBGLED_PIN          6   ///< GPIO port pin declaration where 0=Rx0, 1=Rx1, 2=Rx3, etc.
#define DBGLED_Get()        _RB6 ///< Macro instruction to set a pin state to logic HIGH
#define DBGLED_Set()        { _LATB6 = 1; } ///< Macro instruction to set a pin state to logic HIGH
#define DBGLED_Clear()      { _LATB6 = 0; } ///< Macro instruction to set a pin state to logic LOW
#define DBGLED_Toggle()     { _LATB6 ^= 1; } ///< Macro instruction to toggle most recent pin state
#define DBGLED_Init()       { _LATB6 = 0; _TRISB6 = 0; } ///< Macro instruction initializing the specified GPIO as output
#define DBGLED_Dispose()    { _LATB6 = 1; _TRISB6 = 1; } ///< Macro instruction initializing the specified GPIO as input 

// Power Good Output
#define PWRGOOD_PORT        1   ///< GPIO port declaration where 0=Port RA, 0=Port RB, 0=Port RC, etc.
#define PWRGOOD_PIN         12  ///< GPIO port pin declaration where 0=Rx0, 1=Rx1, 2=Rx3, etc.
#define PWRGOOD_Get()       { _RB12; } ///< Macro instruction to set a pin state to logic HIGH
#define PWRGOOD_Set()       { _LATB12 = 1; } ///< Macro instruction to set a pin state to logic HIGH
#define PWRGOOD_Clear()	    { _LATB12 = 0; } ///< Macro instruction to set a pin state to logic LOW
#define PWRGOOD_Toggle()    { _LATB12 ^= 1; } ///< Macro instruction to toggle most recent pin state
#define PWRGOOD_Init()      { _LATB12 = 0; _TRISB12 = 0; } ///< Macro instruction initializing the specified GPIO as output
#define PWRGOOD_Dispose()   { _LATB12 = 1; _TRISB12 = 1; } ///< Macro instruction initializing the specified GPIO as input
    
// User Switch Input
#define SW_USER_PORT        3   ///< GPIO port declaration where 0=Port RA, 0=Port RB, 0=Port RC, etc.
#define SW_USER_PIN         1   ///< GPIO port pin declaration where 0=Rx0, 1=Rx1, 2=Rx3, etc.
#define SW_USER_Get()       _RD1 ///< Macro instruction to set a pin state to logic HIGH
#define SW_USER_Set()       { _LATD1 = 1; } ///< Macro instruction to set a pin state to logic HIGH
#define SW_USER_Clear()	    { _LATD1 = 0; } ///< Macro instruction to set a pin state to logic LOW
#define SW_USER_Toggle()    { _LATD1 ^= 1; } ///< Macro instruction to toggle most recent pin state
#define SW_USER_Init()      { _LATD1 = 1; _TRISD1 = 1; } ///< Macro instruction initializing the specified GPIO as input
#define SW_USER_Dispose()   { _LATD1 = 1; _TRISD1 = 1; } ///< Macro instruction initializing the specified GPIO as input


/** @} */ // end of group circuit-gpio-mcal

/**************************************************************************************************
 * @ingroup circuit-peripheral-mcal
 * @{
 * @brief Special Function chip resources declarations
 * 
 * @details
 * Some applications may require additional chip resources, which are not supported by
 * default library modules used in this project. For static configurations, these chip 
 * resources are initialized in function \ref sysUserPeriperhals_Initialize during system 
 * startup. Any specific declarations required for abstracting hardware or microcontroller
 * features is placed in this section
 * 
 **************************************************************************************************/

#define DAC_OUTPUT_INSTANCE     1U ///< Digital-To-Analog Converter instance used for enhanced debugging
#define DAC_BUFFER_OPA_INSTANCE 3U ///< Operational amplifier instance used to buffer DAC output
#define DAC_OUTPUT_INIT_VALUE   0U ///< initial value of the Digital-To-Analog Converter debugging output


#if (DAC_OUTPUT_INSTANCE == 1U)
  #define DACOUT_Set(x) { DAC1DATH=x; } ///< Direct SFR Write Access Macro for Digital-To-Analog Converter instance #1
  #define DACOUT_Get()    DAC1DATH      ///< Direct SFR Read Access Macro for Digital-To-Analog Converter instance #1
#elif (DAC_OUTPUT_INSTANCE == 2U)
  #define DACOUT_Set(x) { DAC2DATH=x; } ///< Direct SFR Write Access Macro for Digital-To-Analog Converter instance #2
  #define DACOUT_Get()    DAC2DATH      ///< Direct SFR Read Access Macro for Digital-To-Analog Converter instance #2
#elif (DAC_OUTPUT_INSTANCE == 3U)
  #define DACOUT_Set(x) { DAC3DATH=x; } ///< Direct SFR Write Access Macro for Digital-To-Analog Converter instance #3
  #define DACOUT_Get()    DAC3DATH      ///< Direct SFR Read Access Macro for Digital-To-Analog Converter instance #3
#endif

/** @} */ // end of group circuit-peripheral-mcal


#if (INCLUDE_BUCK == true)
/** @cond INCLUDE_BUCK */

/**************************************************************************************************
 * @ingroup pwm-settings-buck
 * @{
 * @brief User-declaration of global defines for PWM signal generator settings
 * 
 * @details
 * This section defines fundamental PWM settings required for the on-board buck converter of DPSK3.
 * These settings are determined by hardware and defined using physical quantities. Pre-compiler 
 * macros are used to convert physical values into binary (integer) numbers to be written to 
 * Special Function Registers (SFR).
 * 
 **************************************************************************************************/

#define BUCK_NO_OF_PHASES              1U ///< Number of power converter phases of this design

#define BUCK_SWITCHING_FREQUENCY       (float)500.0e+3 ///< Fixed Switching frequency in [Hz]
#define BUCK_PWM_DUTY_CYCLE_MIN        (float)  0.010  ///< Minimum on/off-time ratio (duty ratio) in [%]
#define BUCK_PWM_DUTY_CYCLE_MAX        (float)  0.900  ///< Maximum on/off-time ratio (duty ratio) in [%]
#define BUCK_LEADING_EDGE_BLANKING     (float)120.0e-9 ///< Leading Edge Blanking period in [sec]
#define BUCK_DEAD_TIME_LEADING_EDGE    (float) 20.0e-9 ///< Leading Edge Dead Time delay in [sec]
#define BUCK_DEAD_TIME_FALLING_EDGE    (float) 60.0e-9 ///< Falling Edge Dead Time delay in [sec]

/** @} */ // end of group pwm-settings-buck ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** 
 * @ingroup pwm-mcal-buck
 * @{
 * @brief PWM peripheral output pins, control signals and register assignments of converter phase #1
 * 
 * @details
 * Converter phase #1 uses a simple half-bridge to commutate the switch node. The signal source
 * therefore only requires a single PWM generator instance to be configured in fixed frequency 
 * complementary mode with dead times. Additional PWM peripheral features are used by the firmware
 * to respond to interrupts, trigger ADC conversions, control device output pins during startup 
 * and fault responses and to change timing settings on the fly. 
 * 
 * Please review the device data sheet for details about register names and settings.
 */
#define BUCK_PWM_CHANNEL            1U ///< PWM Instance Index (e.g. 1=PWM1, 2=PWM2, etc.)
#define BUCK_PWM_GPIO_INSTANCE      1U ///< Number indicating device port, where 0=Port RA, 0=Port RB, 0=Port RC, etc.
#define BUCK_PWM_GPIO_PORT_PINH     14U ///< Port Pin Number
#define BUCK_PWM_GPIO_PORT_PINL     15U ///< Port Pin Number
#define BUCK_PWM_OUTPUT_SWAP        false ///< true = PWMxH is the leading PWM output, false = PWMxL is the leading PWM output
    
#define BUCK_PWM_PDC                PG1DC    ///< PWM Instance Duty Cycle Register
#define BUCK_PWMH_TRIS              _TRISB14 ///< Device Port TRIS register
#define BUCK_PWMH_WR                _LATB14  ///< Device Pin WRITE
#define BUCK_PWMH_RD                _RB14    ///< Device Pin READ
#define BUCK_PWMH_RPx               (uint8_t)46 ///< Device Pin output remappable pin number (RPx)
#define BUCK_PWML_TRIS              _TRISB15 ///< Device Port TRIS register
#define BUCK_PWML_WR                _LATB15  ///< Device Pin WRITE
#define BUCK_PWML_RD                _RB15    ///< Device Pin READ
#define BUCK_PWML_RPx               (uint8_t)47 ///< Device Pin output remappable pin number (RPx)

#define _BUCK_PWM_Interrupt         _PWM1Interrupt ///< PWM Interrupt Service Routine label
#define BUCK_PWM_IF                 _PWM1IF  ///< PWM Interrupt Flag Bit
#define BUCK_PWM_IE                 _PWM1IE  ///< PWM Interrupt Enable Bit
#define BUCK_PWM_IP                 _PWM1IP  ///< PWM Interrupt Priority
#define BUCK_PWM_TRGSRC_TRG1        0b00100  ///< PWM Trigger #1 Trigger Source of this channel
#define BUCK_PWM_TRGSRC_TRG2        0b00101  ///< PWM Trigger #2 Trigger Source of this channel
#define BUCK_PWM_PGxTRIGA           PG1TRIGA ///< PWM trigger register A
#define BUCK_PWM_PGxTRIGB           PG1TRIGB ///< PWM trigger register B
#define BUCK_PWM_PGxTRIGC           PG1TRIGC ///< PWM trigger register C
    
#define BUCK_PWM_ADTR1OFS           0U ///< ADC Trigger 1 Offset:  0...31
#define BUCK_PWM_ADTR1PS            0U ///< ADC Trigger 1 Postscaler: 0...31

#define BUCK_PWM_UPDREQ             PG1STATbits.UPDREQ

/** @} */ // end of group pwm-mcal-buck ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup pwm-macros-buck
 * @{
 * @brief Conversion macros for user-declarations of PWM parameters
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */

// Conversion Macros
#define BUCK_SWITCHING_PERIOD   (float)(1.0/BUCK_SWITCHING_FREQUENCY)   ///< Switching period in [sec]
#define BUCK_PWM_PERIOD         (uint16_t)(float)(BUCK_SWITCHING_PERIOD / PWM_CLOCK_PERIOD) ///< This sets the switching period of the converter
#define BUCK_PWM_PHASE_SHIFT    (uint16_t)((float)BUCK_PWM_PERIOD / (float)BUCK_NO_OF_PHASES) ///< This sets the phase shift between phase #1 and #2
#define BUCK_PWM_DC_MIN         (uint16_t)(BUCK_PWM_DUTY_CYCLE_MIN * (float)BUCK_PWM_PERIOD) ///< This sets the minimum duty cycle
#define BUCK_PWM_DC_MAX         (uint16_t)(BUCK_PWM_DUTY_CYCLE_MAX * (float)BUCK_PWM_PERIOD) ///< This sets the maximum duty cycle
#define BUCK_LEB_PERIOD         (uint16_t)(BUCK_LEADING_EDGE_BLANKING / (float)PWM_CLOCK_PERIOD) ///< Leading Edge Blanking = n x PWM resolution (here: 50 x 2ns = 100ns)
#define BUCK_PWM_DEAD_TIME_LE   (uint16_t)(BUCK_DEAD_TIME_LEADING_EDGE / (float)PWM_CLOCK_PERIOD) ///< Rising edge dead time [tick = 250ps]
#define BUCK_PWM_DEAD_TIME_FE   (uint16_t)(BUCK_DEAD_TIME_FALLING_EDGE / (float)PWM_CLOCK_PERIOD) ///< Falling edge dead time [tick = 250ps]

/** @} */ // end of group pwm-macros-buck ~~~~~~~~~~~~~~~~~

    
/**************************************************************************************************
 * @ingroup input-voltage-feedback-settings-buck
 * @{
 * @brief Declaration of input voltage feedback properties
 * 
 * @details
 * In this section the output voltage feedback signal scaling, gain, valid signal limits and nominal
 * operating point is specified. Physical quantities are used to define values. Macros are used to 
 * convert given physical values into binary (integer) number to be written into SFRs and variables.
 * *************************************************************************************************/

// Feedback Declarations
#define BUCK_VIN_MINIMUM            (float) 6.500   ///< Minimum input voltage in [V]
#define BUCK_VIN_NOMINAL            (float) 9.000   ///< Nominal input voltage in [V]
#define BUCK_VIN_MAXIMUM            (float)13.800   ///< Maximum input voltage in [V]

#define BUCK_VIN_UNDER_VOLTAGE      (float) 6.000   ///< Under Voltage Lock Out Cut Off in [V]
#define BUCK_VIN_OVER_VOLTAGE       (float)14.300   ///< Over Voltage Lock Out Cut Off in [V]
#define BUCK_VIN_HYSTERESIS         (float) 1.000   ///< UVLO/OVLO Hysteresis in [V]
    
#define BUCK_VIN_R1                 (float) 6.980   ///< Upper voltage divider resistor in [kOhm]
#define BUCK_VIN_R2                 (float) 1.000   ///< Lower voltage divider resistor in [kOhm]
    
#define BUCK_VIN_FEEDBACK_OFFSET    (float) 0.0000  ///< Physical, static signal offset in [V]
#define BUCK_VIN_ADC_TRG_DELAY      (float) 20.0e-9 ///< ADC trigger delay in [sec]

/** @} */ // end of group input-voltage-feedback-settings-buck ~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup input-voltage-feedback-mcal-buck
 * @{
 * @brief ADC input assignments of input voltage feedback signals
 * 
 * @details
 * In this section the ADC input channels, related ADC result buffers, trigger
 * sources and interrupt vectors are defined. These settings allow the fast 
 * re-assignments of feedback signals in case of hardware changes.
 */

// Peripheral Assignments
#define _BUCK_VIN_ADCInterrupt  _ADCAN12Interrupt ///< ADC interrupt service routine function call of the input voltage feedback channel
#define _BUCK_VIN_ADCISR_IF     _ADCAN12IF   ///< ADC interrupt flag bit of the input voltage feedback channel

#define BUCK_VIN_ANSEL          _ANSELC0    ///< GPIO analog function mode enable bit
#define BUCK_VIN_ADCCORE        8           ///< 0=Dedicated Core #0, 1=Dedicated Core #1, 8=Shared ADC Core
#define BUCK_VIN_ADCIN          12          ///< Analog input number (e.g. '5' for 'AN5')
#define BUCK_VIN_ADCBUF         ADCBUF12     ///< ADC input buffer of this ADC channel
#define BUCK_VIN_ADCTRIG        PG1TRIGB    ///< Register used for trigger placement
#define BUCK_VIN_TRGSRC         BUCK_PWM_TRGSRC_TRG2 ///< PWM1 (=PG1) Trigger 2 via PGxTRIGB

/** @} */ // end of group input-voltage-feedback-mcal-buck ~~~~~~~~~~~~~~~~~~~~


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup input-voltage-feedback-macros-buck
 * @{
 * @brief Conversion macros of input voltage feedback parameters
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */
    
#define BUCK_VIN_FEEDBACK_GAIN  (float)((BUCK_VIN_R2) / (BUCK_VIN_R1 + BUCK_VIN_R2)) // DO NOT CHANGE
#define BUCK_VIN_MIN            (uint16_t)(BUCK_VIN_MINIMUM * BUCK_VIN_FEEDBACK_GAIN / ADC_GRANULARITY)   ///< Minimum input voltage
#define BUCK_VIN_NOM            (uint16_t)(BUCK_VIN_NOMINAL * BUCK_VIN_FEEDBACK_GAIN / ADC_GRANULARITY)   ///< Nominal input voltage
#define BUCK_VIN_MAX            (uint16_t)(BUCK_VIN_MAXIMUM * BUCK_VIN_FEEDBACK_GAIN / ADC_GRANULARITY)   ///< Maximum input voltage
#define BUCK_VIN_HYST           (uint16_t)(BUCK_VIN_HYSTERESIS * BUCK_VIN_FEEDBACK_GAIN / ADC_GRANULARITY)  ///< Over Voltage LOck Out voltage    
#define BUCK_VIN_UVLO_TRIP      (uint16_t)(BUCK_VIN_UNDER_VOLTAGE * BUCK_VIN_FEEDBACK_GAIN / ADC_GRANULARITY) ///< Under Voltage LOck Out voltage
#define BUCK_VIN_UVLO_RELEASE   (uint16_t)((BUCK_VIN_UNDER_VOLTAGE + BUCK_VIN_HYSTERESIS) * BUCK_VIN_FEEDBACK_GAIN / ADC_GRANULARITY) ///< Under Voltage LOck Out voltage
#define BUCK_VIN_OVLO_TRIP      (uint16_t)(BUCK_VIN_OVER_VOLTAGE * BUCK_VIN_FEEDBACK_GAIN / ADC_GRANULARITY)  ///< Over Voltage LOck Out voltage
#define BUCK_VIN_OVLO_RELEASE   (uint16_t)((BUCK_VIN_OVER_VOLTAGE - BUCK_VIN_HYSTERESIS) * BUCK_VIN_FEEDBACK_GAIN / ADC_GRANULARITY)  ///< Over Voltage LOck Out voltage
#define BUCK_VIN_ADC_TRGDLY     (uint16_t)(BUCK_VIN_ADC_TRG_DELAY / PWM_CLOCK_PERIOD) ///< Input voltage ADC trigger delay
#define BUCK_VIN_OFFSET         (uint16_t)(BUCK_VIN_FEEDBACK_OFFSET / ADC_GRANULARITY) ///< Input voltage feedback offset

#define BUCK_VIN_NORM_INV_G     (float)(1.0/BUCK_VIN_FEEDBACK_GAIN) ///< Inverted feedback gain required for value normalization
#define BUCK_VIN_NORM_SCALER    (int16_t)(ceil(log(BUCK_VIN_NORM_INV_G)/log(2))) ///< VIN normalization  
#define BUCK_VIN_NORM_FACTOR    (int16_t)((BUCK_VIN_NORM_INV_G / pow(2.0, BUCK_VIN_NORM_SCALER)) * (pow(2.0, 15)-1)) ///< VIN normalization factor scaled in Q15

#define BUCK_VIN_RANGE_MAX      (float)(ADC_REFERENCE * BUCK_VIN_NORM_INV_G)

/** @} */ // end of group input-voltage-feedback-macros-buck ~~~~~~~~~~~~~~~~~~

/**************************************************************************************************
 * @ingroup output-voltage-feedback-settings-buck
 * @{
 * @brief Declaration of output voltage feedback properties
 * 
 * @details
 * In this section the output voltage feedback signal scaling, gain, valid signal limits and nominal
 * operating point is specified. Physical quantities are used to define values. Macros are used to 
 * convert given physical values into binary (integer) number to be written into SFRs and variables.
 * *************************************************************************************************/

// Feedback Declarations
#define BUCK_VOUT_MINIMUM           (float) 0.000   ///< Absolute minimum output voltage
#define BUCK_VOUT_NOMINAL           (float) 3.300   ///< Nominal output voltage
#define BUCK_VOUT_MAXIMUM           (float) 4.000   ///< Absolute maximum output voltage (Over Voltage Protection)
#define BUCK_VOUT_HYSTERESIS        (float) 0.500   ///< Over Voltage Protection Hysteresis

#define BUCK_VOUT_TOLERANCE_MAX     (float) 0.200   ///< Output voltage tolerance at nominal operating point [+/-]
#define BUCK_VOUT_TOLERANCE_MIN     (float) 0.100   ///< Output voltage tolerance at nominal operating point [+/-]
    
#define BUCK_VOUT_DIV_R1            (float) 1.000  ///< Upper voltage divider resistor in kOhm
#define BUCK_VOUT_DIV_R2            (float) 1.000  ///< Lower voltage divider resistor in kOhm
#define BUCK_VOUT_FEEDBACK_OFFSET   (float) 0.000  ///< Physical, static signal offset in [V]
#define BUCK_VOUT_ADC_TRG_DELAY     (float)20.0e-9 ///< Trigger delay in [sec]

/** @} */ // end of group output-voltage-feedback-settings-buck ~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup output-voltage-feedback-mcal-buck
 * @{
 * @brief ADC input assignments of output voltage feedback signals
 * 
 * @details
 * In this section the ADC input channels, related ADC result buffers, trigger
 * sources and interrupt vectors are defined. These settings allow the fast 
 * re-assignments of feedback signals in case of hardware changes.
 */

// Peripheral Assignments
#define BUCK_VOUT_ANSEL             _ANSELC1    ///< GPIO analog function mode enable bit
#define BUCK_VOUT_ADCCORE           8           ///< 0=Dedicated Core #0, 1=Dedicated Core #1, 8=Shared ADC Core
#define BUCK_VOUT_ADCIN             13          ///< Analog input number (e.g. '5' for 'AN5')
#define BUCK_VOUT_ADCBUF            ADCBUF13     ///< ADC input buffer of this ADC channel
#define BUCK_VOUT_ADCTRIG           PG1TRIGA    ///< Register used for trigger placement
#define BUCK_VOUT_TRGSRC            BUCK_PWM_TRGSRC_TRG1 ///< PWM1 (=PG1) Trigger 1 via PGxTRIGA

/** @} */ // end of group output-voltage-feedback-mcal-buck ~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup output-voltage-feedback-macros-buck
 * @{
 * @brief Conversion macros of output voltage feedback parameters
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */

#define BUCK_VOUT_FEEDBACK_GAIN (float)((BUCK_VOUT_DIV_R2) / (BUCK_VOUT_DIV_R1 + BUCK_VOUT_DIV_R2)) ///< Feedback gain required for value normalization

#define BUCK_VOUT_REF           (uint16_t)(BUCK_VOUT_NOMINAL * BUCK_VOUT_FEEDBACK_GAIN / ADC_GRANULARITY) ///< Macro calculating the integer number equivalent of the output voltage reference given above in [V]
#define BUCK_VOUT_NOM           (uint16_t) BUCK_VOUT_REF ///< Alias macro of the integer number equivalent of the nominal output voltage given above in [V]
#define BUCK_VOUT_DEV_TRIP      (uint16_t)((BUCK_VOUT_TOLERANCE_MAX * BUCK_VOUT_FEEDBACK_GAIN) / ADC_GRANULARITY) ///< Macro calculating the integer number equivalent of the maximum allowed output voltage deviation given above in [V], which will lead to a converter shut down when exceeded.
#define BUCK_VOUT_DEV_RELEASE   (uint16_t)((BUCK_VOUT_TOLERANCE_MIN * BUCK_VOUT_FEEDBACK_GAIN) / ADC_GRANULARITY) ///< Macro calculating the integer number equivalent of the maximum allowed output voltage deviation given above in [V], which needs to be underrun before a shut-down converter can recover
#define BUCK_VOUT_OVP_TRIP      (uint16_t)((BUCK_VOUT_MAXIMUM * BUCK_VOUT_FEEDBACK_GAIN) / ADC_GRANULARITY)  ///< Over Voltage Protection voltage trip signal level
#define BUCK_VOUT_OVP_RELEASE   (uint16_t)((BUCK_VOUT_MAXIMUM - BUCK_VOUT_HYSTERESIS) * BUCK_VOUT_FEEDBACK_GAIN / ADC_GRANULARITY)  ///< Over Voltage Protection voltage recovery signal level
#define BUCK_VOUT_OFFSET        (uint16_t)(BUCK_VOUT_FEEDBACK_OFFSET / ADC_GRANULARITY) ///< Macro calculating the integer number equivalent of the physical, static signal offset of this feedback channel
#define BUCK_VOUT_ADC_TRGDLY    (uint16_t)(BUCK_VOUT_ADC_TRG_DELAY / PWM_CLOCK_PERIOD) ///< Macro calculating the integer number equivalent of the signal chain time delay between internal PWM timebase and effective switching edge of the leading FET

#define BUCK_VOUT_NORM_INV_G    (float)(1.0/BUCK_VOUT_FEEDBACK_GAIN) ///< Inverted feedback gain required for value normalization
#define BUCK_VOUT_NORM_SCALER   (int16_t)(ceil(log(BUCK_VOUT_NORM_INV_G)/log(2))) ///< VOUT normalization bit-shift scaler  
#define BUCK_VOUT_NORM_FACTOR   (int16_t)((BUCK_VOUT_NORM_INV_G / pow(2.0, BUCK_VOUT_NORM_SCALER)) * (pow(2.0, 15)-1)) ///< VOUT normalization factor scaled in Q15

#define BUCK_VOUT_RANGE_MAX     (float)(ADC_REFERENCE * BUCK_VOUT_NORM_INV_G) ///< Macro calculating the integer number equivalent of the total output voltage range defined by the settings given above in [V]]
    
/** @} */ // end of group output-voltage-feedback-macros-buck ~~~~~~~~~~~~~~~~~

/**************************************************************************************************
 * @ingroup phase-current-feedback-settings-buck
 * @{
 * @brief Declaration of phase-current feedback properties
 * 
 * @details
 * In this section the phase-current feedback signal scaling, gain, valid signal limits and nominal
 * operating point is specified. Physical quantities are used to define parameter values to ease
 * the system configuration. 
 * 
 * As DPSK3 supports two different current sense devices (current sense transformers and shunt
 * amplifier devices) users can select which one should be used for regulating the output current.
 * Based on the selection made, the appropriate feedback channels and signal scaling values will be
 * used by the firmware.
 * 
 * Macros are used to convert given physical values into binary (integer) number to be written
 * into SFRs and variables and being used in runtime calculations.  
 * (see \ref phase-current-feedback-macros-buck for details)
 * *************************************************************************************************/

// Feedback Declarations
#define BUCK_ISNS_CT    0                   ///< Use current sense transformer as main current feedback source
#define BUCK_ISNS_AMP   1                   ///< Use shunt amplifier as main current feedback source
    
#define BUCK_ISNS_OPTION    BUCK_ISNS_CT    ///< Select one of the available current sense feedback options

// Settings based on the selection made above
#if (BUCK_ISNS_OPTION == BUCK_ISNS_CT)

    #define BUCK_ISNS_FEEDBACK_GAIN     (float) 1.000   ///< Current Gain in V/A
    #define BUCK_ISNS_MINIMUM           (float)-1.000   ///< absolute total minimum output current (average)
    #define BUCK_ISNS_MAXIMUM           (float) 2.500   ///< absolute total maximum output current (average)
    #define BUCK_ISNS_RELEASE           (float) 2.000   ///< current reset level after over current event
    #define BUCK_ISNS_REFERENCE         (float) 2.000   ///< output current reference (average)
    #define BUCK_ISNS_ADC_TRG_DELAY     (float) 80.0e-9 ///< ADC trigger delay for current sense in [sec]
    #define BUCK_ISNS_FEEDBACK_OFFSET   (float) 0.000   ///< Current sense feedback offset (average)

    #define BUCK_ISNS_OFFSET_CALIBRATION_ENABLE  false      ///< Current Sense Offset Calibration is disabled 

#elif (BUCK_ISNS_OPTION == BUCK_ISNS_AMP)

    #define BUCK_ISNS_FEEDBACK_GAIN     (float) 0.600       ///< Current Gain in V/A
    #define BUCK_ISNS_MINIMUM           (float) 0.000       ///< absolute total minimum output current (average)
    #define BUCK_ISNS_MAXIMUM           (float) 2.500       ///< absolute total maximum output current (average)
    #define BUCK_ISNS_RELEASE           (float) 2.500       ///< current reset level after over current event
    #define BUCK_ISNS_REFERENCE         (float) 2.500       ///< output current reference (average)
    #define BUCK_ISNS_ADC_TRG_DELAY     (float) 240.0e-9    ///< ADC trigger delay for current sense in [sec]
    #define BUCK_ISNS_FEEDBACK_OFFSET   (float) 1.650       ///< current sense #1 feedback offset (average)

    #define BUCK_ISNS_OFFSET_CALIBRATION_ENABLE true        ///< Current Sense Offset Calibration is disabled 

#endif

/** @} */ // end of group phase-current-feedback-settings-buck ~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup phase-current-feedback-macros-buck
 * @{
 * @brief Conversion macros of phase current feedback parameters
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */

// Phase Current Feedback Settings Conversion Macros
#define BUCK_ISNS_MIN           (uint16_t)(int16_t)(((BUCK_ISNS_MINIMUM-BUCK_ISNS_FEEDBACK_OFFSET) * BUCK_ISNS_FEEDBACK_GAIN) / ADC_GRANULARITY)  ///< Over Current Limit
#define BUCK_ISNS_OCL           (uint16_t)(((BUCK_ISNS_MAXIMUM-BUCK_ISNS_FEEDBACK_OFFSET) * BUCK_ISNS_FEEDBACK_GAIN) / ADC_GRANULARITY)  ///< Over Current Limit
#define BUCK_ISNS_OCL_RELEASE   (uint16_t)(((BUCK_ISNS_RELEASE-BUCK_ISNS_FEEDBACK_OFFSET) * BUCK_ISNS_FEEDBACK_GAIN) / ADC_GRANULARITY)  ///< Over Current Release Level
#define BUCK_ISNS_REF           (uint16_t)((BUCK_ISNS_REFERENCE * BUCK_ISNS_FEEDBACK_GAIN) / ADC_GRANULARITY)  ///< Output Current Reference
#define BUCK_ISNS_FB_OFFSET     (uint16_t)(BUCK_ISNS_FEEDBACK_OFFSET / ADC_GRANULARITY)
#define BUCK_ISNS_ADC_TRGDLY    (uint16_t)(BUCK_ISNS_ADC_TRG_DELAY / PWM_CLOCK_PERIOD)

#define BUCK_ISNS_NORM_INV_G    (float)(1.0/BUCK_ISNS_FEEDBACK_GAIN) ///< Inverted feedback gain required for value normalization
#define BUCK_ISNS_NORM_SCALER   (int16_t)(ceil(log(BUCK_ISNS_NORM_INV_G)/log(2))) ///< ISNS normalization  
#define BUCK_ISNS_NORM_FACTOR   (int16_t)((BUCK_ISNS_NORM_INV_G / pow(2.0, BUCK_ISNS_NORM_SCALER)) * (pow(2.0, 15)-1)) ///< ISNS normalization factor scaled in Q15

/** @} */ // end of group phase-current-feedback-macros ~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup phase-current-feedback-mcal-buck
 * @{
 * @brief ADC input assignments of phase current feedback signals
 * 
 * @details
 * In this section the ADC input channels, related ADC result buffers, trigger
 * sources and interrupt vectors are defined. These settings allow the fast 
 * re-assignments of feedback signals in case of hardware changes.
 */
 
// Peripheral Assignments
#if (BUCK_ISNS_OPTION == BUCK_ISNS_CT)

    #define _BUCK_ISNS_ADCInterrupt     _ADCAN0Interrupt ///< Interrupt Service Routine function name
    #define _BUCK_ISNS_ADCISR_IF        _ADCAN0IF   ///< Interrupt Service Routine Flag Bit

    #define BUCK_ISNS_ANSEL             _ANSELA0    ///< GPIO analog function mode enable bit
    #define BUCK_ISNS_ADCCORE           0           ///< 0=Dedicated Core #0, 1=Dedicated Core #1, 2=Shared ADC Core
    #define BUCK_ISNS_ADCIN             0           ///< Analog input number (e.g. '5' for 'AN5')
    #define BUCK_ISNS_ALT_IN_SELECT     0b00        ///< Alternative Analog Input Selection (dedicated ADC cores only))																										   
    #define BUCK_ISNS_ADCBUF            ADCBUF0     ///< ADC input buffer of this ADC channel
    #define BUCK_ISNS_ADCTRIG           PG1TRIGB    ///< Register used for trigger placement
    #define BUCK_ISNS_TRGSRC            BUCK_PWM_TRGSRC_TRG2 ///< PWM1 (=PG1) Trigger 2 via PGxTRIGB

#elif (BUCK_ISNS_OPTION == BUCK_ISNS_AMP)

    #define _BUCK_ISNS_ADCInterrupt     _ADCAN1Interrupt ///< Interrupt Service Routine function name
    #define _BUCK_ISNS_ADCISR_IF        _ADCAN1IF   ///< Interrupt Service Routine Flag Bit

    #define BUCK_ISNS_ANSEL             _ANSELA1    ///< GPIO analog function mode enable bit
    #define BUCK_ISNS_ADCCORE           1           ///< 0=Dedicated Core #0, 1=Dedicated Core #1, 7=Shared ADC Core
    #define BUCK_ISNS_ADCIN             1           ///< Analog input number (e.g. '5' for 'AN5')
	#define BUCK_ISNS_ALT_IN_SELECT     0b01        ///< Alternative Analog Input Selection (dedicated ADC cores only))
    #define BUCK_ISNS_ADCBUF            ADCBUF1     ///< ADC input buffer of this ADC channel
    #define BUCK_ISNS_ADCTRIG           PG1TRIGB    ///< Register used for trigger placement
    #define BUCK_ISNS_TRGSRC            BUCK_PWM_TRGSRC_TRG2 ///< PWM1 (=PG1) Trigger 2 via PGxTRIGB

#else
    #pragma message "hardware abstraction layer warning: no current sense feedback selected."
#endif

/** @} */ // end of group phase-current-feedback-mcal-buck ~~~~~~~~~~~~~~~~~~~~

/**************************************************************************************************
 * @ingroup adaptive-gain-control-settings-buck
 * @{
 * @brief Declaration of additional hardware-specific defines required for adaptive gain control
 * 
 * @details
 * In this section additional macros are defined to calculate constant parameters for the
 * adaptive gain modulation algorithm using user defined settings declared in their respective
 * sections. Any change of these parameters will also result in a change of the values of the 
 * gain modulation parameters of this section.
 * *************************************************************************************************/

#define BUCK_AGC_EXECUTION_DELAY    (float)(370.0e-9) ///< AGC Observer Algorithm Execution Time in [sec]

/** @} */ // end of group adaptive-gain-control-settings-buck ~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup adaptive-gain-control-macros-buck
 * @{
 * @brief Conversion macros of phase current feedback parameters
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */

#define BUCK_VL_MINIMUM         (float)(BUCK_VIN_UNDER_VOLTAGE - BUCK_VOUT_MAXIMUM) ///< Minimum input voltage - maximum output voltage
#define BUCK_VL_NOMINAL         (float)(BUCK_VIN_NOMINAL       - BUCK_VOUT_NOMINAL) ///< Nominal input voltage - nominal output voltage
#define BUCK_VL_MAXIMUM         (float)(BUCK_VIN_RANGE_MAX     - 0) ///< Maximum input voltage - output voltage = 0V

// The AGC compare value is defined at nominal input voltage and output voltage 
// The maximum modulation factor is normalized to fractional '1' to prevent number overruns
#define BUCK_AGC_FACTOR_MAX     (float)(BUCK_VL_NOMINAL / BUCK_VL_MINIMUM) ///< Floating point number of the maximumm limit of the adaptive gain modulation factor (float)
#define BUCK_AGC_NOM_SCALER     (uint16_t)(ceil(log(BUCK_AGC_FACTOR_MAX)/log(2))) ///< Bit-shift scaler of the floating point number of the maimum limit of the adaptive gain modulation factor
#define BUCK_AGC_NOM_FACTOR     (uint16_t)(0x7FFF >> BUCK_AGC_NOM_SCALER) ///< Fractional of the floating point number of the maimum limit of the adaptive gain modulation factor
#define BUCK_AGC_MEDIAN         (int16_t)(((int16_t)(((float)BUCK_VIN_NOM * BUCK_VIN_NORM_FCT) - BUCK_VOUT_NOM))>>BUCK_AGC_NOM_SCALER) ///< Adaptive gain modulation factor at nominal operating point

// To calculate the voltage across the inductor, input and output voltage ADC results need to be normalized. The normalization factor is determined here
// Each input voltage sample has to be multiplied with this scaling factor to allow the calculation of the instantaneous voltage across the inductor
#define BUCK_VIN_NORM_FCT       (float)(BUCK_VOUT_FEEDBACK_GAIN / BUCK_VIN_FEEDBACK_GAIN)   ///< VIN-2-VOUT Normalization Factor
#define BUCK_AGC_IO_NORM_SCALER (int16_t)(ceil(log(BUCK_VIN_NORM_FCT)/log(2))) ///< Nominal VL Q15 scaler  
#define BUCK_AGC_IO_NORM_FACTOR (int16_t)((BUCK_VIN_NORM_FCT / pow(2.0, BUCK_AGC_IO_NORM_SCALER)) * (pow(2.0, 15)-1)) ///< Nominal VL Q15 factor 

// Additional execution time calculation to be considered in trigger delay and overall control timing
#define BUCK_AGC_EXEC_DLY       (uint16_t)(BUCK_AGC_EXECUTION_DELAY / PWM_CLOCK_PERIOD) ///< Macro calculating the integer number equivalent of the AGC algorithm computation time

/** @} */ // end of group adaptive-gain-control-macros-buck ~~~~~~~~~~~~~~~~~~~

/**************************************************************************************************
 * @ingroup isr-settings-buck
 * @{
 * @brief Control loop Interrupt Vector Settings
 * 
 * @details
 * Control loops are called in dedicated interrupt service routines of PWM events, ADC events
 * or triggered by timers. This section allows users to set and modify the interrupt service 
 * routine triggers and their priority to set up and optimize the control system interrupt 
 * structure.
 * 
 * *************************************************************************************************/
    
// Hardware-dependent defines
#define BUCK_VOUT_TRIG_PWM  0   ///< Buck VOUT control loop is called in PWM interrupt
#define BUCK_VOUT_TRIG_ADC  1   ///< Buck VOUT control loop is called in ADC interrupt

#define BUCK_VOUT_TRIGGER_MODE  BUCK_VOUT_TRIG_PWM ///< Currently selected voltage loop interrupt vector
#define BUCK_VOUT_ISR_PRIORITY  5 ///< Voltage loop interrupt vector priority (valid settings between 0...6 with 6 being the highest priority)

#if (BUCK_VOUT_TRIGGER_MODE == BUCK_VOUT_TRIG_ADC)    

  #define _BUCK_VLOOP_Interrupt     _ADCAN13Interrupt ///< Interrupt vector function call label
  #define _BUCK_VLOOP_ISR_IP        _ADCAN13IP ///< Interupt vector priority register bits
  #define _BUCK_VLOOP_ISR_IF        _ADCAN13IF ///< Interupt vector flag bit register bit
  #define _BUCK_VLOOP_ISR_IE        _ADCAN13IE ///< Interupt vector enable bit register bit

#elif (BUCK_VOUT_TRIGGER_MODE == BUCK_VOUT_TRIG_PWM)

  #define _BUCK_VLOOP_Interrupt     _PWM1Interrupt ///< Interrupt vector function call label
  #define _BUCK_VLOOP_ISR_IP        _PWM1IP ///< Interupt vector priority register
  #define _BUCK_VLOOP_ISR_IF        _PWM1IF ///< Interupt vector flag bit register bit
  #define _BUCK_VLOOP_ISR_IE        _PWM1IE ///< Interupt vector enable bit register bit

#endif

/** @} */ // end of group isr-settings-buck ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**************************************************************************************************
 * @ingroup startup-timing-settings-buck
 * @{
 * @brief Global defines for soft-start specific parameters
 * 
 * @details
 * This section is used to define power supply startup timing settings. The soft-start sequence 
 * is part of the power controller. It allows to program specific timings for 
 *   - Power On Delay
 *   - Ramp Period 
 *   - Power Good Delay
 * 
 * After the startup has passed these three timing periods, the power supply is ending up in 
 * "normal" operation, continuously regulating the output until a fault is detected or the 
 * operating state is changed for any other reason. When the output voltage reference is changed, 
 * the power control state machine will use the voltage ramp slope defined here to tune from the 
 * recent voltage reference to the new reference value. During this period the BUSY-bit of the 
 * power controller (status word, bit #7) will be set. This status bit will be cleared automatically
 * by the power controller state machine once the new reference value has been applied and the 
 * converter is back in constant regulation mode.
 * 
 * Pre-compiler macros are used to translate physical values into binary (integer) numbers to 
 * be written to SFRs and variables.  
 * (see \ref startup-timing-macros-buck for details)
 * 
 * @note
 * On DPSK3 it takes roughly 500 ms until the auxiliary power has been started, the 
 * PIC24 housekeeping controller on the bottom side of the board resets the protection
 * logic and allows the dsPIC to run. After this period the dsPIC controller starts to 
 * execute its firmware.
 *  
 * This additional startup delay of ~500 ms is not considered in the settings below and 
 * needs to be taken into account when adjusting startup timing. Use an independent debugging
 * pin toggle at the beginning of the firmware to verify the specified startup timing is 
 * applied as desired.
 **************************************************************************************************/

#define BUCK_POWER_ON_DELAY          (float) 200e-3 ///< power on delay in [sec]
#define BUCK_VRAMP_PERIOD            (float) 100e-3 ///< voltage ramp-up period in [sec]
#define BUCK_IRAMP_PERIOD            (float) 100e-3 ///< output current ramp-up period in [sec]
#define BUCK_POWER_GOOD_DELAY        (float) 200e-3 ///< power good delay in [sec]

/** @} */ // end of group startup-timing-settings ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup startup-timing-macros-buck
 * @{
 * @brief Conversion Macros of Startup Timing Settings
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */

#define BUCK_POD       (uint16_t)(((float)BUCK_POWER_ON_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0)
#define BUCK_VRAMP_PER (uint16_t)(((float)BUCK_VRAMP_PERIOD / (float)MAIN_EXECUTION_PERIOD)-1.0)
#define BUCK_VREF_STEP (uint16_t)((float)BUCK_VOUT_REF / (float)(BUCK_VRAMP_PER + 1.0))
#define BUCK_IRAMP_PER (uint16_t)(((float)BUCK_IRAMP_PERIOD / (float)MAIN_EXECUTION_PERIOD)-1.0)
#define BUCK_IREF_STEP (uint16_t)((float)BUCK_ISNS_REF / (float)(BUCK_VRAMP_PER + 1.0))
#define BUCK_PGD       (uint16_t)(((float)BUCK_POWER_GOOD_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0)

/** @} */ // end of group startup-timing-macros-buck ~~~~~~~~~~~~~~~~~~~~~~~~~~

/**************************************************************************************************
 * @ingroup fault-response-settings-buck
 * @{
 * @brief Global defines for fault-monitor related parameters
 * 
 * @details
 * This section is used to define power supply fault object timing settings. The fault monitor 
 * is continuously monitoring declared data objects at the high-priority task frequency defined by 
 * \ref MAIN_EXECUTION_PERIOD. Based on this known interval, filtering delays for fault trip and 
 * fault recovery events to allow users to adjust the fault detection sensitivity.
 * 
 * - Fault Trip Event Delay   
 * This setting defines for how long a fault condition has to be continuously active before the 
 * effective fault trip status/event will be triggered.
 * 
 * - Fault Recovery Event Delay   
 * This setting defines for how long a fault condition has to be continuously cleared before the 
 * effective fault recovery status/event will be triggered.
 * 
 * - Restart Cycle Limit
 * In case of a persistent fault condition (e.g continuous short circuit or unstable feedback loop)
 * the fault monitor will limit the number of restart attempts specified by FAULT_RESTART_CYCLES.
 * When this threshold is exceeded, the power supply will latch in save state (off) and a CPU or 
 * power cycle is required to start-up the power supply again.
 * 
 *************************************************************************************************/

#define BUCK_UVLO_TRIP_DELAY        (float) 5e-3   ///< under voltage lock out trip delay in [sec]
#define BUCK_UVLO_RECOVERY_DELAY    (float) 500e-3 ///< under voltage lock out recovery delay in [sec]
#define BUCK_OVLO_TRIP_DELAY        (float) 5e-3   ///< over voltage lock out trip delay in [sec]
#define BUCK_OVLO_RECOVERY_DELAY    (float) 500e-3 ///< over voltage lock out recovery delay in [sec]
#define BUCK_REGERR_TRIP_DELAY      (float) 25e-3  ///< regulation error trip delay in [sec]
#define BUCK_REGERR_RECOVERY_DELAY  (float) 2000e-3 ///< regulation error recovery delay in [sec]
#define BUCK_OCP_TRIP_DELAY         (float) 2e-3   ///< over current proection trip delay in [sec]
#define BUCK_OCP_RECOVERY_DELAY     (float) 2000e-3 ///< over current proection recovery delay in [sec]

#define BUCK_FAULT_RESTART_CYCLES   10  ///< Maximum number of restart cycles: Fault state will be latched until CPU reset or power cycle

/** @} */ // end of group fault-response-settings-buck ~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup fault-response-macros-buck
 * @{
 * @brief Conversion Macros of Fault Response Timing Settings
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */

#define BUCK_UVLO_TDLY   (uint16_t)(((float)      BUCK_UVLO_TRIP_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< under voltage lock out trip delay conversion nacro
#define BUCK_UVLO_RDLY   (uint16_t)(((float)  BUCK_UVLO_RECOVERY_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< under voltage lock out recovery delay conversion nacro
#define BUCK_OVLO_TDLY   (uint16_t)(((float)      BUCK_OVLO_TRIP_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< over voltage lock out trip delay conversion nacro
#define BUCK_OVLO_RDLY   (uint16_t)(((float)  BUCK_OVLO_RECOVERY_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< over voltage lock out recovery delay conversion nacro
#define BUCK_REGERR_TDLY (uint16_t)(((float)    BUCK_REGERR_TRIP_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< regulation error trip delay conversion macro
#define BUCK_REGERR_RDLY (uint16_t)(((float)BUCK_REGERR_RECOVERY_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< regulation error recovery delay conversion macro
#define BUCK_OCP_TDLY    (uint16_t)(((float)       BUCK_OCP_TRIP_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< over current protection trip Delay conversion macro
#define BUCK_OCP_RDLY    (uint16_t)(((float)   BUCK_OCP_RECOVERY_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< over current protection recovery delay conversion nacro

/** @} */ // end of group fault-response-macros-buck ~~~~~~~~~~~~~~~~~~~~~~~~~~

/** @endcond */ // Doxygen section
#endif   /* INCLUDE_BUCK */

#if (INCLUDE_BOOST == true)
/** @cond INCLUDE_BOOST */

/**************************************************************************************************
 * @ingroup pwm-settings-boost
 * @{
 * @brief User-declaration of global defines for PWM signal generator settings
 * 
 * @details
 * This section defines fundamental PWM settings required for the on-board boost converter of DPSK3.
 * These settings are determined by hardware and defined using physical quantities. Pre-compiler 
 * macros are used to convert physical values into binary (integer) numbers to be written to 
 * Special Function Registers (SFR).
 * 
 **************************************************************************************************/

#define BOOST_NO_OF_PHASES              1U ///< Number of power converter phases of this design

#define BOOST_SWITCHING_FREQUENCY       (float)500.0e+3 ///< Fixed Switching frequency in [Hz]
#define BOOST_PWM_DUTY_CYCLE_MIN        (float)  0.030  ///< Minimum on/off-time ratio (duty ratio) in [%]
#define BOOST_PWM_DUTY_CYCLE_MAX        (float)  0.900  ///< Maximum on/off-time ratio (duty ratio) in [%]
#define BOOST_LEADING_EDGE_BLANKING     (float)120.0e-9 ///< Leading Edge Blanking period in [sec]
#define BOOST_DEAD_TIME_LEADING_EDGE    (float)  0.0e-9 ///< Leading Edge Dead Time delay in [sec]
#define BOOST_DEAD_TIME_FALLING_EDGE    (float)  0.0e-9 ///< Falling Edge Dead Time delay in [sec]

/** @} */ // end of group pwm-settings-boost ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** 
 * @ingroup pwm-mcal-boost
 * @{
 * @brief PWM peripheral output pins, control signals and register assignments of converter phase #1
 * 
 * @details
 * Converter phase #1 uses a simple half-bridge to commutate the switch node. The signal source
 * therefore only requires a single PWM generator instance to be configured in fixed frequency 
 * complementary mode with dead times. Additional PWM peripheral features are used by the firmware
 * to respond to interrupts, trigger ADC conversions, control device output pins during startup 
 * and fault responses and to change timing settings on the fly. 
 * 
 * Please review the device data sheet for details about register names and settings.
 */
#define BOOST_PWM_CHANNEL            2U ///< PWM Instance Index (e.g. 1=PWM1, 2=PWM2, etc.)
#define BOOST_PWM_GPIO_INSTANCE      1U ///< Number indicating device port, where 0=Port RA, 0=Port RB, 0=Port RC, etc.
#define BOOST_PWM_GPIO_PORT_PINH     13U ///< Port Pin Number
#define BOOST_PWM_GPIO_PORT_PINL     12U ///< Port Pin Number
#define BOOST_PWM_OUTPUT_SWAP        true ///< true = PWMxH is the leading PWM output, false = PWMxL is the leading PWM output

#define BOOST_PWM_PDC                PG2DC    ///< PWM Instance Duty Cycle Register
#define BOOST_PWMH_TRIS              _TRISB12 ///< Device Port TRIS register
#define BOOST_PWMH_WR                _LATB12  ///< Device Pin WRITE
#define BOOST_PWMH_RD                _RB12    ///< Device Pin READ
#define BOOST_PWMH_RPx               (uint8_t)44 ///< Device Pin output remappable pin number (RPx)
#define BOOST_PWML_TRIS              _TRISB13 ///< Device Port TRIS register
#define BOOST_PWML_WR                _LATB13  ///< Device Pin WRITE
#define BOOST_PWML_RD                _RB13    ///< Device Pin READ
#define BOOST_PWML_RPx               (uint8_t)45 ///< Device Pin output remappable pin number (RPx)

#define _BOOST_PWM_Interrupt         _PWM2Interrupt ///< PWM Interrupt Service Routine label
#define BOOST_PWM_IF                 _PWM2IF  ///< PWM Interrupt Flag Bit
#define BOOST_PWM_IE                 _PWM2IE  ///< PWM Interrupt Enable Bit
#define BOOST_PWM_IP                 _PWM2IP  ///< PWM Interrupt Priority
#define BOOST_PWM_TRGSRC_TRG1        0b00110  ///< PWM Trigger #1 Trigger Source of this channel
#define BOOST_PWM_TRGSRC_TRG2        0b00111  ///< PWM Trigger #2 Trigger Source of this channel
#define BOOST_PWM_PGxTRIGA           PG2TRIGA ///< PWM trigger register A
#define BOOST_PWM_PGxTRIGB           PG2TRIGB ///< PWM trigger register B
#define BOOST_PWM_PGxTRIGC           PG2TRIGC ///< PWM trigger register C
    
#define BOOST_PWM_ADTR1OFS           0U ///< ADC Trigger 1 Offset:  0...31
#define BOOST_PWM_ADTR1PS            0U ///< ADC Trigger 1 Postscaler: 0...31

#define BOOST_PWM_UPDREQ             PG2STATbits.UPDREQ

/** @} */ // end of group pwm-mcal-boost ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup pwm-macros-boost
 * @{
 * @brief Conversion macros for user-declarations of PWM parameters
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */

// Conversion Macros
#define BOOST_SWITCHING_PERIOD   (float)(1.0/BOOST_SWITCHING_FREQUENCY)   ///< Switching period in [sec]
#define BOOST_PWM_PERIOD         (uint16_t)(float)(BOOST_SWITCHING_PERIOD / PWM_CLOCK_PERIOD) ///< This sets the switching period of the converter
#define BOOST_PWM_PHASE_SHIFT    (uint16_t)((float)BOOST_PWM_PERIOD / (float)BOOST_NO_OF_PHASES) ///< This sets the phase shift between phase #1 and #2
#define BOOST_PWM_DC_MIN         (uint16_t)(BOOST_PWM_DUTY_CYCLE_MIN * (float)BOOST_PWM_PERIOD) ///< This sets the minimum duty cycle
#define BOOST_PWM_DC_MAX         (uint16_t)(BOOST_PWM_DUTY_CYCLE_MAX * (float)BOOST_PWM_PERIOD) ///< This sets the maximum duty cycle
#define BOOST_LEB_PERIOD         (uint16_t)(BOOST_LEADING_EDGE_BLANKING / (float)PWM_CLOCK_PERIOD) ///< Leading Edge Blanking = n x PWM resolution (here: 50 x 2ns = 100ns)
#define BOOST_PWM_DEAD_TIME_LE   (uint16_t)(BOOST_DEAD_TIME_LEADING_EDGE / (float)PWM_CLOCK_PERIOD) ///< Rising edge dead time [tick = 250ps]
#define BOOST_PWM_DEAD_TIME_FE   (uint16_t)(BOOST_DEAD_TIME_FALLING_EDGE / (float)PWM_CLOCK_PERIOD) ///< Falling edge dead time [tick = 250ps]

/** @} */ // end of group pwm-macros-boost ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    
/**************************************************************************************************
 * @ingroup input-voltage-feedback-settings-boost
 * @{
 * @brief Declaration of input voltage feedback properties
 * 
 * @details
 * In this section the output voltage feedback signal scaling, gain, valid signal limits and nominal
 * operating point is specified. Physical quantities are used to define values. Macros are used to 
 * convert given physical values into binary (integer) number to be written into SFRs and variables.
 * *************************************************************************************************/

// Feedback Declarations
#define BOOST_VIN_MINIMUM            (float) 6.500   ///< Minimum input voltage in [V]
#define BOOST_VIN_NOMINAL            (float) 9.000   ///< Nominal input voltage in [V]
#define BOOST_VIN_MAXIMUM            (float)13.800   ///< Maximum input voltage in [V]

#define BOOST_VIN_UNDER_VOLTAGE      (float) 6.000   ///< Under Voltage Lock Out Cut Off in [V]
#define BOOST_VIN_OVER_VOLTAGE       (float)14.300   ///< Over Voltage Lock Out Cut Off in [V]
#define BOOST_VIN_HYSTERESIS         (float) 1.000   ///< UVLO/OVLO Hysteresis in [V]
    
#define BOOST_VIN_R1                 (float) 6.980   ///< Upper voltage divider resistor in [kOhm]
#define BOOST_VIN_R2                 (float) 1.000   ///< Lower voltage divider resistor in [kOhm]
    
#define BOOST_VIN_FEEDBACK_OFFSET    (float) 0.0000  ///< Physical, static signal offset in [V]
#define BOOST_VIN_ADC_TRG_DELAY      (float)20.0e-9  ///< ADC trigger delay in [sec]

/** @} */ // end of group input-voltage-feedback-settings-boost ~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup input-voltage-feedback-mcal-boost
 * @{
 * @brief ADC input assignments of input voltage feedback signals
 * 
 * @details
 * In this section the ADC input channels, related ADC result buffers, trigger
 * sources and interrupt vectors are defined. These settings allow the fast 
 * re-assignments of feedback signals in case of hardware changes.
 */

// Peripheral Assignments
#define _BOOST_VIN_ADCInterrupt  _ADCAN12Interrupt ///< ADC interrupt service routine function call of the input voltage feedback channel
#define _BOOST_VIN_ADCISR_IF     _ADCAN12IF   ///< ADC interrupt flag bit of the input voltage feedback channel

#define BOOST_VIN_ANSEL          _ANSELC0    ///< GPIO analog function mode enable bit
#define BOOST_VIN_ADCCORE        8           ///< 0=Dedicated Core #0, 1=Dedicated Core #1, 8=Shared ADC Core
#define BOOST_VIN_ADCIN          12          ///< Analog input number (e.g. '5' for 'AN5')
#define BOOST_VIN_ADCBUF         ADCBUF12     ///< ADC input buffer of this ADC channel
#define BOOST_VIN_ADCTRIG        PG2TRIGB    ///< Register used for trigger placement
#define BOOST_VIN_TRGSRC         BOOST_PWM_TRGSRC_TRG2 ///< PWM2 (=PG2) Trigger 2 via PGxTRIGB

/** @} */ // end of group input-voltage-feedback-mcal-boost ~~~~~~~~~~~~~~~~~~~


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup input-voltage-feedback-macros-boost
 * @{
 * @brief Conversion macros of input voltage feedback parameters
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */
    
#define BOOST_VIN_FEEDBACK_GAIN  (float)((BOOST_VIN_R2) / (BOOST_VIN_R1 + BOOST_VIN_R2)) // DO NOT CHANGE
#define BOOST_VIN_MIN            (uint16_t)(BOOST_VIN_MINIMUM * BOOST_VIN_FEEDBACK_GAIN / ADC_GRANULARITY)   ///< Minimum input voltage
#define BOOST_VIN_NOM            (uint16_t)(BOOST_VIN_NOMINAL * BOOST_VIN_FEEDBACK_GAIN / ADC_GRANULARITY)   ///< Nominal input voltage
#define BOOST_VIN_MAX            (uint16_t)(BOOST_VIN_MAXIMUM * BOOST_VIN_FEEDBACK_GAIN / ADC_GRANULARITY)   ///< Maximum input voltage
#define BOOST_VIN_HYST           (uint16_t)(BOOST_VIN_HYSTERESIS * BOOST_VIN_FEEDBACK_GAIN / ADC_GRANULARITY)  ///< Over Voltage LOck Out voltage    
#define BOOST_VIN_UVLO_TRIP      (uint16_t)(BOOST_VIN_UNDER_VOLTAGE * BOOST_VIN_FEEDBACK_GAIN / ADC_GRANULARITY) ///< Under Voltage LOck Out voltage
#define BOOST_VIN_UVLO_RELEASE   (uint16_t)((BOOST_VIN_UNDER_VOLTAGE + BOOST_VIN_HYSTERESIS) * BOOST_VIN_FEEDBACK_GAIN / ADC_GRANULARITY) ///< Under Voltage LOck Out voltage
#define BOOST_VIN_OVLO_TRIP      (uint16_t)(BOOST_VIN_OVER_VOLTAGE * BOOST_VIN_FEEDBACK_GAIN / ADC_GRANULARITY)  ///< Over Voltage LOck Out voltage
#define BOOST_VIN_OVLO_RELEASE   (uint16_t)((BOOST_VIN_OVER_VOLTAGE - BOOST_VIN_HYSTERESIS) * BOOST_VIN_FEEDBACK_GAIN / ADC_GRANULARITY)  ///< Over Voltage LOck Out voltage
#define BOOST_VIN_ADC_TRGDLY     (uint16_t)(BOOST_VIN_ADC_TRG_DELAY / PWM_CLOCK_PERIOD) ///< Input voltage ADC trigger delay
#define BOOST_VIN_OFFSET         (uint16_t)(BOOST_VIN_FEEDBACK_OFFSET / ADC_GRANULARITY) ///< Input voltage feedback offset

#define BOOST_VIN_NORM_INV_G     (float)(1.0/BOOST_VIN_FEEDBACK_GAIN) ///< Inverted feedback gain required for value normalization
#define BOOST_VIN_NORM_SCALER    (int16_t)(ceil(log(BOOST_VIN_NORM_INV_G)/log(2))) ///< VIN normalization  
#define BOOST_VIN_NORM_FACTOR    (int16_t)((BOOST_VIN_NORM_INV_G / pow(2.0, BOOST_VIN_NORM_SCALER)) * (pow(2.0, 15)-1)) ///< VIN normalization factor scaled in Q15

#define BOOST_VIN_RANGE_MAX      (float)(ADC_REFERENCE * BOOST_VIN_NORM_INV_G)

/** @} */ // end of group input-voltage-feedback-macros-boost ~~~~~~~~~~~~~~~~~

/**************************************************************************************************
 * @ingroup output-voltage-feedback-settings-boost
 * @{
 * @brief Declaration of output voltage feedback properties
 * 
 * @details
 * In this section the output voltage feedback signal scaling, gain, valid signal limits and nominal
 * operating point is specified. Physical quantities are used to define values. Macros are used to 
 * convert given physical values into binary (integer) number to be written into SFRs and variables.
 * *************************************************************************************************/

// Feedback Declarations
#define BOOST_VOUT_MINIMUM           (float)14.00   ///< Absolute minimum output voltage
#define BOOST_VOUT_NOMINAL           (float)15.00   ///< Nominal output voltage
#define BOOST_VOUT_MAXIMUM           (float)16.00   ///< Absolute maximum output voltage (Over Voltage Protection)
#define BOOST_VOUT_HYSTERESIS        (float)0.500   ///< Over Voltage Protection Hysteresis

#define BOOST_VOUT_TOLERANCE_MAX     (float)0.500   ///< Output voltage tolerance at nominal operating point [+/-]
#define BOOST_VOUT_TOLERANCE_MIN     (float)0.100   ///< Output voltage tolerance at nominal operating point [+/-]
#define BOOST_VOUT_VFWD_DROP_MAX     (float)0.400   ///< Recitifer diode forward voltage drop maximum
    
#define BOOST_VOUT_DIV_R1            (float) 6.980  ///< Upper voltage divider resistor in kOhm
#define BOOST_VOUT_DIV_R2            (float) 1.000  ///< Lower voltage divider resistor in kOhm
#define BOOST_VOUT_FEEDBACK_OFFSET   (float) 0.000  ///< Physical, static signal offset in [V]
#define BOOST_VOUT_ADC_TRG_DELAY     (float)20.0e-9 ///< Trigger delay in [sec]

/** @} */ // end of group output-voltage-feedback-settings-boost ~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup output-voltage-feedback-mcal-boost
 * @{
 * @brief ADC input assignments of output voltage feedback signals
 * 
 * @details
 * In this section the ADC input channels, related ADC result buffers, trigger
 * sources and interrupt vectors are defined. These settings allow the fast 
 * re-assignments of feedback signals in case of hardware changes.
 */

// Peripheral Assignments
#define BOOST_VOUT_ANSEL             _ANSELD10   ///< GPIO analog function mode enable bit
#define BOOST_VOUT_ADCCORE           8           ///< 0=Dedicated Core #0, 1=Dedicated Core #1, 8=Shared ADC Core
#define BOOST_VOUT_ADCIN             18          ///< Analog input number (e.g. '5' for 'AN5')
#define BOOST_VOUT_ADCBUF            ADCBUF18    ///< ADC input buffer of this ADC channel
#define BOOST_VOUT_ADCTRIG           PG2TRIGA    ///< Register used for trigger placement
#define BOOST_VOUT_TRGSRC            BOOST_PWM_TRGSRC_TRG1 ///< PWM2 (=PG2) Trigger 1 via PGxTRIGA

/** @} */ // end of group output-voltage-feedback-mcal-boost ~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup output-voltage-feedback-macros-boost
 * @{
 * @brief Conversion macros of output voltage feedback parameters
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */

#define BOOST_VOUT_FEEDBACK_GAIN (float)((BOOST_VOUT_DIV_R2) / (BOOST_VOUT_DIV_R1 + BOOST_VOUT_DIV_R2))

#define BOOST_VOUT_REF          (uint16_t)(BOOST_VOUT_NOMINAL * BOOST_VOUT_FEEDBACK_GAIN / ADC_GRANULARITY) ///< Macro calculating the integer number equivalent of the output voltage reference given above in [V]
#define BOOST_VOUT_NOM          BOOST_VOUT_REF ///< Alias macro of the integer number equivalent of the nominal output voltage given above in [V]
#define BOOST_VOUT_DEV_TRIP     (uint16_t)(BOOST_VOUT_TOLERANCE_MAX * BOOST_VOUT_FEEDBACK_GAIN / ADC_GRANULARITY) ///< Macro calculating the integer number equivalent of the maximum allowed output voltage deviation given above in [V], which will lead to a converter shut down when exceeded.
#define BOOST_VOUT_DEV_RELEASE  (uint16_t)(BOOST_VOUT_TOLERANCE_MIN * BOOST_VOUT_FEEDBACK_GAIN / ADC_GRANULARITY) ///< Macro calculating the integer number equivalent of the maximum allowed output voltage deviation given above in [V], which needs to be underrun before a shut-down converter can recover
#define BOOST_VOUT_OVP_TRIP     (uint16_t)((BOOST_VOUT_MAXIMUM * BOOST_VOUT_FEEDBACK_GAIN) / ADC_GRANULARITY)  ///< Over Voltage Protection voltage trip signal level
#define BOOST_VOUT_OVP_RELEASE  (uint16_t)((BOOST_VOUT_MAXIMUM - BOOST_VOUT_HYSTERESIS) * BOOST_VOUT_FEEDBACK_GAIN / ADC_GRANULARITY)  ///< Over Voltage Protection voltage recovery signal level
#define BOOST_VOUT_OFFSET       (uint16_t)(BOOST_VOUT_FEEDBACK_OFFSET / ADC_GRANULARITY) ///< Macro calculating the integer number equivalent of the physical, static signal offset of this feedback channel
#define BOOST_VOUT_ADC_TRGDLY   (uint16_t)(BOOST_VOUT_ADC_TRG_DELAY / PWM_CLOCK_PERIOD) ///< Macro calculating the integer number equivalent of the signal chain time delay between internal PWM timebase and effective switching edge of the leading FET
#define BOOST_VOUT_VFWD_DROP    (uint16_t)(BOOST_VOUT_VFWD_DROP_MAX / ADC_GRANULARITY) ///< Macro calculating the integer number equivalent of the maximum forward voltage drop across teh rectifier diode

#define BOOST_VOUT_NORM_INV_G   (float)(1.0/BOOST_VOUT_FEEDBACK_GAIN) ///< Inverted feedback gain required for value normalization
#define BOOST_VOUT_NORM_SCALER  (int16_t)(ceil(log(BOOST_VOUT_NORM_INV_G)/log(2))) ///< VOUT normalization  
#define BOOST_VOUT_NORM_FACTOR  (int16_t)((BOOST_VOUT_NORM_INV_G / pow(2.0, BOOST_VOUT_NORM_SCALER)) * (pow(2.0, 15)-1)) ///< VOUT normalization factor scaled in Q15

#define BOOST_VOUT_RANGE_MAX    (float)(ADC_REFERENCE * BOOST_VOUT_NORM_INV_G) ///< Macro calculating the integer number equivalent of the total output voltage range defined by the settings given above in [V]]
    
/** @} */ // end of group output-voltage-feedback-macros-boost ~~~~~~~~~~~~~~~~

/**************************************************************************************************
 * @ingroup phase-current-feedback-settings-boost
 * @{
 * @brief Declaration of phase-current feedback properties
 * 
 * @details
 * In this section the phase-current feedback signal scaling, gain, valid signal limits and nominal
 * operating point is specified. Physical quantities are used to define parameter values to ease
 * the system configuration. 
 * 
 * As DPSK3 supports two different current sense devices (current sense transformers and shunt
 * amplifier devices) users can select which one should be used for regulating the output current.
 * Based on the selection made, the appropriate feedback channels and signal scaling values will be
 * used by the firmware.
 * 
 * Macros are used to convert given physical values into binary (integer) number to be written
 * into SFRs and variables and being used in runtime calculations.  
 * (see \ref phase-current-feedback-macros-boost for details)
 * *************************************************************************************************/

// Feedback Declarations
#define BOOST_ISNS_LSCS     0                   ///< Use low-side shunt resistor as main current feedback source
#define BOOST_ISNS_AMP      1                   ///< Use shunt amplifier as main current feedback source
    
#define BOOST_ISNS_OPTION    BOOST_ISNS_LSCS    ///< Select one of the available current sense feedback options

// Settings based on the selection made above
#if (BOOST_ISNS_OPTION == BOOST_ISNS_LSCS)

    #define BOOST_ISNS_FEEDBACK_GAIN     (float) 1.000      ///< Current Gain in V/A
    #define BOOST_ISNS_MINIMUM           (float)-1.000      ///< absolute total minimum output current (average) in [A]
    #define BOOST_ISNS_MAXIMUM           (float) 2.500      ///< absolute total maximum output current (average) in [A]
    #define BOOST_ISNS_RELEASE           (float) 1.500      ///< current reset level after over current event in [A]
    #define BOOST_ISNS_REFERENCE         (float) 2.500      ///< output current reference (average) in [A]
    #define BOOST_ISNS_REFERENCE_STARTUP (float) 2.500      ///< maximum output current (average) at startup in [A]
    #define BOOST_ISNS_ADC_TRG_DELAY     (float)80.0e-9     ///< ADC trigger delay for current sense in [sec]
    #define BOOST_ISNS_FEEDBACK_OFFSET   (float) 1.125      ///< Current sense feedback offset (average) in [V]

    #define BOOST_ISNS_OFFSET_CALIBRATION_ENABLE  true       ///< Current Sense Offset Calibration is disabled 

#elif (BOOST_ISNS_OPTION == BOOST_ISNS_AMP)

    #define BOOST_ISNS_FEEDBACK_GAIN     (float) 0.600      ///< Current Gain in V/A
    #define BOOST_ISNS_MINIMUM           (float) 0.000      ///< absolute total minimum output current (average) in [A]
    #define BOOST_ISNS_MAXIMUM           (float) 2.500      ///< absolute total maximum output current (average) in [A]
    #define BOOST_ISNS_RELEASE           (float) 2.500      ///< current reset level after over current event in [A]
    #define BOOST_ISNS_REFERENCE         (float) 2.500      ///< output current reference (average) in [A]
    #define BOOST_ISNS_REFERENCE_STARTUP (float) 2.500      ///< maximum output current (average) at startup in [A]
    #define BOOST_ISNS_ADC_TRG_DELAY     (float) 240.0e-9   ///< ADC trigger delay for current sense in [sec]
    #define BOOST_ISNS_FEEDBACK_OFFSET   (float) 1.650      ///< current sense #1 feedback offset (average) in [V]

    #define BOOST_ISNS_OFFSET_CALIBRATION_ENABLE true       ///< Current Sense Offset Calibration is disabled 

#endif

/** @} */ // end of group phase-current-feedback-settings-boost ~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup phase-current-feedback-macros-boost
 * @{
 * @brief Conversion macros of phase current feedback parameters
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */

// Phase Current Feedback Settings Conversion Macros
#define BOOST_ISNS_MIN           (uint16_t)(int16_t)(((BOOST_ISNS_MINIMUM-BOOST_ISNS_FEEDBACK_OFFSET) * BOOST_ISNS_FEEDBACK_GAIN) / ADC_GRANULARITY)  ///< Over Current Limit
#define BOOST_ISNS_OCL           (uint16_t)(((BOOST_ISNS_MAXIMUM-BOOST_ISNS_FEEDBACK_OFFSET) * BOOST_ISNS_FEEDBACK_GAIN) / ADC_GRANULARITY)  ///< Over Current Limit
#define BOOST_ISNS_OCL_RELEASE   (uint16_t)(((BOOST_ISNS_RELEASE-BOOST_ISNS_FEEDBACK_OFFSET) * BOOST_ISNS_FEEDBACK_GAIN) / ADC_GRANULARITY)  ///< Over Current Release Level
#define BOOST_ISNS_REF           (uint16_t)((BOOST_ISNS_REFERENCE * BOOST_ISNS_FEEDBACK_GAIN) / ADC_GRANULARITY)  ///< Output Current Reference
#define BOOST_ISNS_REF_STARTUP   (uint16_t)((BOOST_ISNS_REFERENCE_STARTUP * BOOST_ISNS_FEEDBACK_GAIN) / ADC_GRANULARITY)  ///< Output Current Startup Reference
#define BOOST_ISNS_FB_OFFSET     (uint16_t)(BOOST_ISNS_FEEDBACK_OFFSET / ADC_GRANULARITY)
#define BOOST_ISNS_ADC_TRGDLY    (uint16_t)(BOOST_ISNS_ADC_TRG_DELAY / PWM_CLOCK_PERIOD)

#define BOOST_ISNS_NORM_INV_G    (float)(1.0/BOOST_ISNS_FEEDBACK_GAIN) ///< Inverted feedback gain required for value normalization
#define BOOST_ISNS_NORM_SCALER   (int16_t)(ceil(log(BOOST_ISNS_NORM_INV_G)/log(2))) ///< ISNS normalization  
#define BOOST_ISNS_NORM_FACTOR   (int16_t)((BOOST_ISNS_NORM_INV_G / pow(2.0, BOOST_ISNS_NORM_SCALER)) * (pow(2.0, 15)-1)) ///< ISNS normalization factor scaled in Q15

/** @} */ // end of group phase-current-feedback-macros-boost ~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup phase-current-feedback-mcal-boost
 * @{
 * @brief ADC input assignments of phase current feedback signals
 * 
 * @details
 * In this section the ADC input channels, related ADC result buffers, trigger
 * sources and interrupt vectors are defined. These settings allow the fast 
 * re-assignments of feedback signals in case of hardware changes.
 */
 
// Peripheral Assignments
#if (BOOST_ISNS_OPTION == BOOST_ISNS_LSCS)

    #define _BOOST_ISNS_ADCInterrupt    _ADCAN1Interrupt ///< Interrupt Service Routine function name
    #define _BOOST_ISNS_ADCISR_IF       _ADCAN1IF   ///< Interrupt Service Routine Flag Bit

    #define BOOST_ISNS_ANSEL            _ANSELA1    ///< GPIO analog function mode enable bit
    #define BOOST_ISNS_ADCCORE          1           ///< 0=Dedicated Core #0, 1=Dedicated Core #1, 2=Shared ADC Core
    #define BOOST_ISNS_ADCIN            1           ///< Analog input number (e.g. '5' for 'AN5')
    #define BOOST_ISNS_ALT_IN_SELECT    0b00        ///< Alternative Analog Input Selection (dedicated ADC cores only))
    #define BOOST_ISNS_ADCBUF           ADCBUF1     ///< ADC input buffer of this ADC channel
    #define BOOST_ISNS_ADCTRIG          PG2TRIGB    ///< Register used for trigger placement
    #define BOOST_ISNS_TRGSRC           BOOST_PWM_TRGSRC_TRG2 ///< PWM2 (=PG2) Trigger 2 via PGxTRIGB

    #define BOOST_ISNS_OPA_INSTANCE     2U  ///< Operational amplifier instance used as ISNS aplifier (0 = disables op-amp option))

#elif (BOOST_ISNS_OPTION == BOOST_ISNS_AMP)

    #define _BOOST_ISNS_ADCInterrupt     _ADCAN1Interrupt ///< Interrupt Service Routine function name
    #define _BOOST_ISNS_ADCISR_IF        _ADCAN1IF   ///< Interrupt Service Routine Flag Bit

    #define BOOST_ISNS_ANSEL             _ANSELA1    ///< GPIO analog function mode enable bit
    #define BOOST_ISNS_ADCCORE           1           ///< 0=Dedicated Core #0, 1=Dedicated Core #1, 7=Shared ADC Core
    #define BOOST_ISNS_ADCIN             1           ///< Analog input number (e.g. '5' for 'AN5')
    #define BOOST_ISNS_ALT_IN_SELECT     0b00        ///< Alternative Analog Input Selection (dedicated ADC cores only))
    #define BOOST_ISNS_ADCBUF            ADCBUF1     ///< ADC input buffer of this ADC channel
    #define BOOST_ISNS_ADCTRIG           PG2TRIGB    ///< Register used for trigger placement
    #define BOOST_ISNS_TRGSRC            BOOST_PWM_TRGSRC_TRG2 ///< PWM2 (=PG2) Trigger 2 via PGxTRIGB

    #define BOOST_ISNS_OPA_INSTANCE     0U  ///< Operational amplifier instance used as ISNS aplifier (0 = disables op-amp))

#else
    #pragma message "hardware abstraction layer warning: no current sense feedback selected."
#endif

/** @} */ // end of group phase-current-feedback-mcal-boost ~~~~~~~~~~~~~~~~~~~

/**************************************************************************************************
 * @ingroup adaptive-gain-control-settings-boost
 * @{
 * @brief Declaration of additional hardware-specific defines required for adaptive gain control
 * 
 * @details
 * In this section additional macros are defined to calculate constant parameters for the
 * adaptive gain modulation algorithm using user defined settings declared in their respective
 * sections. Any change of these parameters will also result in a change of the values of the 
 * gain modulation parameters of this section.
 * *************************************************************************************************/

#define BOOST_AGC_EXECUTION_DELAY    (float)(400.0e-9) ///< AGC Observer Algorithm Execution Time in [sec]

/** @} */ // end of group adaptive-gain-control-settings-boost ~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup adaptive-gain-control-macros-boost
 * @{
 * @brief Conversion macros of phase current feedback parameters
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */
/* ToDo: AGC is temporarily disabled for the boost converter and requires further verification 

#define BOOST_VL_MINIMUM         (float)(BOOST_VIN_UNDER_VOLTAGE) 	///< Minimum input voltage - maximum output voltage
#define BOOST_VL_NOMINAL         (float)(BOOST_VIN_NOMINAL) 		///< Nominal input voltage - nominal output voltage
#define BOOST_VL_MAXIMUM         (float)(BOOST_VIN_RANGE_MAX) 		///< Maximum input voltage - output voltage = 0V

// The AGC compare value is defined at nominal input voltage and output voltage 
// The maximum modulation factor is normalized to fractional '1' to prevent number overruns
#define BOOST_AGC_FACTOR_MAX     (float)(BOOST_VL_NOMINAL / BOOST_VL_MINIMUM) ///< Floating point number of the maximumm limit of the adaptive gain modulation factor (float)
#define BOOST_AGC_NOM_SCALER     (uint16_t)(ceil(log(BOOST_AGC_FACTOR_MAX)/log(2))) ///< Bit-shift scaler of the floating point number of the maimum limit of the adaptive gain modulation factor
#define BOOST_AGC_NOM_FACTOR     (uint16_t)(0x7FFF >> BOOST_AGC_NOM_SCALER) ///< Fractional of the floating point number of the maimum limit of the adaptive gain modulation factor
#define BOOST_AGC_MEDIAN         (int16_t)(((int16_t)(((float)BOOST_VIN_NOM * BOOST_VIN_NORM_FCT) - BOOST_VOUT_NOM))>>BOOST_AGC_NOM_SCALER) ///< Adaptive gain modulation factor at nominal operating point

// To calculate the voltage across the inductor, input and output voltage ADC results need to be normalized. The normalization factor is determined here
// Each input voltage sample has to be multiplied with this scaling factor to allow the calculation of the instantaneous voltage across the inductor
#define BOOST_VIN_NORM_FCT       (float)(BOOST_VOUT_FEEDBACK_GAIN / BOOST_VIN_FEEDBACK_GAIN)   ///< VIN-2-VOUT Normalization Factor
#define BOOST_AGC_IO_NORM_SCALER (int16_t)(ceil(log(BOOST_VIN_NORM_FCT)/log(2))) ///< Nominal VL Q15 scaler  
#define BOOST_AGC_IO_NORM_FACTOR (int16_t)((BOOST_VIN_NORM_FCT / pow(2.0, BOOST_AGC_IO_NORM_SCALER)) * (pow(2.0, 15)-1)) ///< Nominal VL Q15 factor 

// Additional execution time calculation to be considered in trigger delay and overall control timing
#define BOOST_AGC_EXEC_DLY       (uint16_t)(BOOST_AGC_EXECUTION_DELAY / PWM_CLOCK_PERIOD) ///< Macro calculating the integer number equivalent of the AGC algorithm computation time

*/
/** @} */ // end of group adaptive-gain-control-macros-boost ~~~~~~~~~~~~~~~~~~

/**************************************************************************************************
 * @ingroup isr-settings-boost
 * @{
 * @brief Control loop Interrupt Vector Settings
 * 
 * @details
 * Control loops are called in dedicated interrupt service routines of PWM events, ADC events
 * or triggered by timers. This section allows users to set and modify the interrupt service 
 * routine triggers and their priority to set up and optimize the control system interrupt 
 * structure.
 * 
 * *************************************************************************************************/
    
// Hardware-dependent defines
#define BOOST_VOUT_TRIG_PWM  0   ///< Boost VOUT control loop is called in PWM interrupt
#define BOOST_VOUT_TRIG_ADC  1   ///< Boost VOUT control loop is called in ADC interrupt

#define BOOST_VOUT_TRIGGER_MODE  BOOST_VOUT_TRIG_PWM ///< Currently selected voltage loop interrupt vector
#define BOOST_VOUT_ISR_PRIORITY  5 ///< Voltage loop interrupt vector priority (valid settings between 0...6 with 6 being the highest priority)

#if (BOOST_VOUT_TRIGGER_MODE == BOOST_VOUT_TRIG_ADC)    

  #define _BOOST_VLOOP_Interrupt     _ADCAN18Interrupt ///< Interrupt vector function call label
  #define _BOOST_VLOOP_ISR_IP        _ADCAN18IP ///< Interrupt vector priority register bits
  #define _BOOST_VLOOP_ISR_IF        _ADCAN18IF ///< Interrupt vector flag bit register bit
  #define _BOOST_VLOOP_ISR_IE        _ADCAN18IE ///< Interrupt vector enable bit register bit

#elif (BOOST_VOUT_TRIGGER_MODE == BOOST_VOUT_TRIG_PWM)

  #define _BOOST_VLOOP_Interrupt     _PWM2Interrupt ///< Interrupt vector function call label
  #define _BOOST_VLOOP_ISR_IP        _PWM2IP ///< Interrupt vector priority register
  #define _BOOST_VLOOP_ISR_IF        _PWM2IF ///< Interrupt vector flag bit register bit
  #define _BOOST_VLOOP_ISR_IE        _PWM2IE ///< Interrupt vector enable bit register bit

#endif

/** @} */ // end of group isr-settings-boost ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**************************************************************************************************
 * @ingroup startup-timing-settings-boost
 * @{
 * @brief Global defines for soft-start specific parameters
 * 
 * @details
 * This section is used to define power supply startup timing settings. The soft-start sequence 
 * is part of the power controller. It allows to program specific timings for 
 *   - Power On Delay
 *   - Ramp Period 
 *   - Power Good Delay
 * 
 * After the startup has passed these three timing periods, the power supply is ending up in 
 * "normal" operation, continuously regulating the output until a fault is detected or the 
 * operating state is changed for any other reason. When the output voltage reference is changed, 
 * the power control state machine will use the voltage ramp slope defined here to tune from the 
 * recent voltage reference to the new reference value. During this period the BUSY-bit of the 
 * power controller (status word, bit #7) will be set. This status bit will be cleared automatically
 * by the power controller state machine once the new reference value has been applied and the 
 * converter is back in constant regulation mode.
 * 
 * Pre-compiler macros are used to translate physical values into binary (integer) numbers to 
 * be written to SFRs and variables.  
 * (see \ref startup-timing-macros-boost for details)
 * 
 * @note
 * On DPSK3 it takes roughly 500 ms until the auxiliary power has been started, the 
 * PIC24 housekeeping controller on the bottom side of the board resets the protection
 * logic and allows the dsPIC to run. After this period the dsPIC controller starts to 
 * execute its firmware.
 *  
 * This additional startup delay of ~500 ms is not considered in the settings below and 
 * needs to be taken into account when adjusting startup timing. Use an independent debugging
 * pin toggle at the beginning of the firmware to verify the specified startup timing is 
 * applied as desired.
 **************************************************************************************************/

#define BOOST_POWER_ON_DELAY        (float) 200e-3 ///< power on delay in [sec]
#define BOOST_VRAMP_PERIOD          (float) 100e-3 ///< voltage ramp-up period in [sec]
#define BOOST_IRAMP_PERIOD          (float) 100e-3 ///< output current ramp-up period in [sec]
#define BOOST_POWER_GOOD_DELAY      (float) 200e-3 ///< power good delay in [sec]

#define BOOST_CHARGEUP_PERIOD       (float)  50e-3 ///< output capacitor charge up monitor period in [sec]
#define BOOST_CHARGEUP_TIMEOUT      (float) 200e-3 ///< output capacitor charge up monitor timeout in [sec]
    
/** @} */ // end of group startup-timing-settings-boost ~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup startup-timing-macros-boost
 * @{
 * @brief Conversion Macros of Startup Timing Settings
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */

#define BOOST_POD       (uint16_t)(((float)BOOST_POWER_ON_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0)
#define BOOST_VRAMP_PER (uint16_t)(((float)BOOST_VRAMP_PERIOD / (float)MAIN_EXECUTION_PERIOD)-1.0)
#define BOOST_VREF_STEP (uint16_t)((float)BOOST_VOUT_REF / (float)(BOOST_VRAMP_PER + 1.0))
#define BOOST_IRAMP_PER (uint16_t)(((float)BOOST_IRAMP_PERIOD / (float)MAIN_EXECUTION_PERIOD)-1.0)
#define BOOST_IREF_STEP (uint16_t)((float)BOOST_ISNS_REF / (float)(BOOST_VRAMP_PER + 1.0))
#define BOOST_PGD       (uint16_t)(((float)BOOST_POWER_GOOD_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0)
#define BOOST_CHRG_PER  (uint16_t)(((float)BOOST_CHARGEUP_PERIOD / (float)MAIN_EXECUTION_PERIOD)-1.0)
#define BOOST_CHRG_TOUT (uint16_t)(((float)BOOST_CHARGEUP_TIMEOUT / (float)MAIN_EXECUTION_PERIOD)-1.0)

/** @} */ // end of group startup-timing-macros-boost ~~~~~~~~~~~~~~~~~~~~~~~~~

/**************************************************************************************************
 * @ingroup fault-response-settings-boost
 * @{
 * @brief Global defines for fault-monitor related parameters
 * 
 * @details
 * This section is used to define power supply fault object timing settings. The fault monitor 
 * is continuously monitoring declared data objects at the high-priority task frequency defined by 
 * \ref MAIN_EXECUTION_PERIOD. Based on this known interval, filtering delays for fault trip and 
 * fault recovery events to allow users to adjust the fault detection sensitivity.
 * 
 * - Fault Trip Event Delay   
 * This setting defines for how long a fault condition has to be continuously active before the 
 * effective fault trip status/event will be triggered.
 * 
 * - Fault Recovery Event Delay   
 * This setting defines for how long a fault condition has to be continuously cleared before the 
 * effective fault recovery status/event will be triggered.
 * 
 * - Restart Cycle Limit
 * In case of a persistent fault condition (e.g continuous short circuit or unstable feedback loop)
 * the fault monitor will limit the number of restart attempts specified by FAULT_RESTART_CYCLES.
 * When this threshold is exceeded, the power supply will latch in save state (off) and a CPU or 
 * power cycle is required to start-up the power supply again.
 * 
 *************************************************************************************************/

#define BOOST_UVLO_TRIP_DELAY       (float) 5e-3   ///< under voltage lock out trip delay in [sec]
#define BOOST_UVLO_RECOVERY_DELAY   (float) 500e-3 ///< under voltage lock out recovery delay in [sec]
#define BOOST_OVLO_TRIP_DELAY       (float) 5e-3   ///< over voltage lock out trip delay in [sec]
#define BOOST_OVLO_RECOVERY_DELAY   (float) 500e-3 ///< over voltage lock out recovery delay in [sec]
#define BOOST_REGERR_TRIP_DELAY     (float) 25e-3  ///< regulation error trip delay in [sec]
#define BOOST_REGERR_RECOVERY_DELAY (float)2000e-3 ///< regulation error recovery delay in [sec]
#define BOOST_OCP_TRIP_DELAY        (float) 2e-3   ///< over current proection trip delay in [sec]
#define BOOST_OCP_RECOVERY_DELAY    (float)2000e-3 ///< over current proection recovery delay in [sec]

#define BOOST_FAULT_RESTART_CYCLES  10  ///< Maximum number of restart cycles: Fault state will be latched until CPU reset or power cycle

/** @} */ // end of group fault-response-settings-boost ~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup fault-response-macros-boost
 * @{
 * @brief Conversion Macros of Fault Response Timing Settings
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */

#define BOOST_UVLO_TDLY   (uint16_t)(((float)      BOOST_UVLO_TRIP_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< under voltage lock out trip delay conversion nacro
#define BOOST_UVLO_RDLY   (uint16_t)(((float)  BOOST_UVLO_RECOVERY_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< under voltage lock out recovery delay conversion nacro
#define BOOST_OVLO_TDLY   (uint16_t)(((float)      BOOST_OVLO_TRIP_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< over voltage lock out trip delay conversion nacro
#define BOOST_OVLO_RDLY   (uint16_t)(((float)  BOOST_OVLO_RECOVERY_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< over voltage lock out recovery delay conversion nacro
#define BOOST_REGERR_TDLY (uint16_t)(((float)    BOOST_REGERR_TRIP_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< regulation error trip delay conversion macro
#define BOOST_REGERR_RDLY (uint16_t)(((float)BOOST_REGERR_RECOVERY_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< regulation error recovery delay conversion macro
#define BOOST_OCP_TDLY    (uint16_t)(((float)       BOOST_OCP_TRIP_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< over current protection trip Delay conversion macro
#define BOOST_OCP_RDLY    (uint16_t)(((float)   BOOST_OCP_RECOVERY_DELAY / (float)MAIN_EXECUTION_PERIOD)-1.0) ///< over current protection recovery delay conversion nacro

/** @} */ // end of group fault-response-macros-boost ~~~~~~~~~~~~~~~~~~~~~~~~~
    

/** @endcond */
#endif  /* INCLUDE_BOOST */


/**************************************************************************************************
 * @ingroup temperature-feedback-settings
 * @{
 * @brief Declaration of temperature feedback properties
 * 
 * @details
 * In this section the temperature feedback signal scaling, gain and valid signal limits are 
 * specified. Physical quantities are used to define parameter values to ease the system 
 * configuration. 
 * 
 * As DPSK3 has one, central on-board temperature sensor between the load resistor banks 
 * indicating the worst case system temperature.
 * 
 * Macros are used to convert given physical values into binary (integer) number to be written
 * into SFRs and variables and being used in runtime calculations.  
 * (see \ref temperature-feedback-macros for details)
 * *************************************************************************************************/

#define TEMP_CAL_ZERO       (float) 0.500   // Temperature sense signal zero point voltage in [V]
#define TEMP_CAL_SLOPE      (float) 0.010   // Temperature sense signal slope in [V/K]

/** @} */ // end of group temperature-feedback-settings

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup temperature-feedback-macros
 * @{
 * @brief Conversion macros of temperature feedback parameters
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 */

#define TEMP_FB_ZERO            (uint16_t)(TEMP_CAL_ZERO / ADC_GRANULARITY)
#define TEMP_FB_SLOPE           (float)(TEMP_CAL_SLOPE / ADC_GRANULARITY)

/** @} */ // end of group temperature-feedback-macros

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
 * @ingroup temperature-feedback-mcal
 * @{
 * @brief ADC input assignments of temperature feedback signals
 * 
 * @details
 * In this section the ADC input channels, related ADC result buffers, trigger
 * sources and interrupt vectors are defined. These settings allow the fast 
 * re-assignments of feedback signals in case of hardware changes.
 */

#define _TEMP_ADCInterrupt          _ADCAN2Interrupt ///< Interrupt Service Routine function name
#define _TEMP_ADCISR_IF             _ADCAN2IF   ///< Interrupt Service Routine Flag Bit

#define TEMP_ANSEL                  _ANSELB7    ///< GPIO analog function mode enable bit
#define TEMP_ADCCORE                8           // 0=Dedicated Core #0, 1=Dedicated Core #1, 8=Shared ADC Core
#define TEMP_ADCIN                  2           // Analog input number (e.g. '5' for 'AN5')
#define TEMP_ADCBUF                 ADCBUF2     ///< GPIO analog function mode enable bit
#define TEMP_ADCTRIG                BUCK_PWM_PGxTRIGB    ///< Register used for trigger placement
#define TEMP_TRGSRC                 BUCK_PWM_TRGSRC_TRG2    // PWM2 Trigger 2 via PG1TRIGB
    
/** @} */ // end of group temperature-feedback-mcal


#if (INCLUDE_LCD == true)
/** @cond INCLUDE_LCD */

/**************************************************************************************************
 * @ingroup lcd-interface-declarations
 * @{
 * @brief LC Display Interface Declarations
 * @details
 * The LCD device driver is controlling the on-board LC display via a SPI interface. The required
 * SPI interface device pin assignments are declared here.
 * 
 * *************************************************************************************************/

#define LCD_SCL_SetHigh()           _LATC5 = 1
#define LCD_SCL_SetLow()            _LATC5 = 0
#define LCD_SCL_Toggle()            _LATC5 ^= 1
#define LCD_SCL_GetValue()          _RC5
#define LCD_SCL_SetDigitalInput()   _TRISC5 = 1
#define LCD_SCL_SetDigitalOutput()  _TRISC5 = 0

#define LCD_SDI_SetHigh()           _LATC4 = 1
#define LCD_SDI_SetLow()            _LATC4 = 0
#define LCD_SDI_Toggle()            _LATC4 ^= 1
#define LCD_SDI_GetValue()          _RC4
#define LCD_SDI_SetDigitalInput()   _TRISC4 = 1
#define LCD_SDI_SetDigitalOutput()  _TRISC4 = 0

#define LCD_CS_SetHigh()            _LATD8 = 1
#define LCD_CS_SetLow()             _LATD8 = 0
#define LCD_CS_Toggle()             _LATD8 ^= 1
#define LCD_CS_GetValue()           _RD8
#define LCD_CS_SetDigitalInput()    _TRISD8 = 1
#define LCD_CS_SetDigitalOutput()   _TRISD8 = 0

#define LCD_RST_SetHigh()           _LATC8 = 1
#define LCD_RST_SetLow()            _LATC8 = 0
#define LCD_RST_Toggle()            _LATC8 ^= 1
#define LCD_RST_GetValue()          _RC8
#define LCD_RST_SetDigitalInput()   _TRISC8 = 1
#define LCD_RST_SetDigitalOutput()  _TRISC8 = 0

#define LCD_RS_SetHigh()            _LATC9 = 1
#define LCD_RS_SetLow()             _LATC9 = 0
#define LCD_RS_Toggle()             _LATC9 ^= 1
#define LCD_RS_GetValue()           _RC9
#define LCD_RS_SetDigitalInput()    _TRISC9 = 1
#define LCD_RS_SetDigitalOutput()   _TRISC9 = 0

/** @} */ // end of group lcd-interface-declarations

/** @endcond */ // Doxygen section
#endif /* INCLUDE_LCD */

#endif	/* DPSK3_HARDWARE_DESCRIPTOR_H */

// end of file

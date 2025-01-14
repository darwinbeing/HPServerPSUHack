/* *********************************************************************************
 * PowerSmart™ Digital Control Library Designer, Version 0.9.14.676
 * *********************************************************************************
 * Generic library header for z-domain compensation filter assembly functions
 * CGS Version: 3.0.8
 * CGS Date:    03/12/2021
 * ********************************************************************************/
// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef __SPECIAL_FUNCTION_LAYER_LIB_NPNZ16B_H__
#define __SPECIAL_FUNCTION_LAYER_LIB_NPNZ16B_H__

#include <xc.h>                                   // include processor files - each processor file is guarded
#include <dsp.h>                                  // include DSP data types (e.g. fractional)
#include <stdint.h>                               // include standard integer number data types
#include <stddef.h>                               // include standard definition data types
#include <stdbool.h>                              // include standard boolean data types (true/false)

/***************************************************************************************************
 * @ingroup special-function-layer-npnz16-macros
 * @def   __PSDCLD_VERSION
 * @brief Generic macro allowing to identify the file version of 'npnz16b.h'
 *
 * @details
 * This version key represents the product version of PS-DCLD as integer number
 * of the form [MAJOR][MINOR][REVISION] => version 0.9.3.xxx would be shown as 903.
 * User code can check if the file version is compliant with the proprietary user
 * code by using pre-compiler directives such as
 *
 * @code{.c}
 * #if (__PSDCLD_VERSION > 908)
 *  #pragma message "This code has not been tested with the recently included version of npnz16b.h"
 * #endif
 * @endcode
 *
 ***************************************************************************************************/

#ifndef __PSDCLD_VERSION
    #define __PSDCLD_VERSION    914
#endif

/***************************************************************************************************
 * @enum    NPNZ_STATUS_FLAGS_e
 * @brief   Common Controller Status and Control Flag Bits
 * @details
 * The 16-bit wide NPNZ_STATUS_s data object holds status and control bits for
 * monitoring and control of the NPNZ16b_s controller during runtime. The lower 8 bit
 * of the status word are used for status indication while the upper 8 bit are used
 * by control bits.
 * For enhanced programming convenience, definitions of status and control flags are
 * provided in single bit and full word format. All available flags are consolidated
 * in the npnzFlagList object.
 ***************************************************************************************************/

enum NPNZ_STATUS_FLAGS_e
{
    NPNZ_STATUS_CLEAR            = 0b0000000000000000,
    NPNZ_STATUS_SATUATION_MSK    = 0b0000000000000011,
    NPNZ_STATUS_LSAT_ACTIVE      = 0b0000000000000001,
    NPNZ_STATUS_LSAT_CLEAR       = 0b0000000000000000,
    NPNZ_STATUS_USAT_ACTIVE      = 0b0000000000000010,
    NPNZ_STATUS_USAT_CLEAR       = 0b0000000000000000,
    NPNZ_STATUS_AGC_DISABLE      = 0b0000000000000000,
    NPNZ_STATUS_AGC_ENABLED      = 0b0000100000000000,
    NPNZ_STATUS_TARGET_DEFAULT   = 0b0000000000000000,
    NPNZ_STATUS_TARGET_SWAPED    = 0b0001000000000000,
    NPNZ_STATUS_SOURCE_DEFAULT   = 0b0000000000000000,
    NPNZ_STATUS_SOURCE_SWAPED    = 0b0010000000000000,
    NPNZ_STATUS_INV_INPUT_OFF    = 0b0000000000000000,
    NPNZ_STATUS_INV_INPUT_ON     = 0b0100000000000000,
    NPNZ_STATUS_ENABLE_OFF       = 0b0000000000000000,
    NPNZ_STATUS_ENABLE_ON        = 0b1000000000000000
};                                                ///< NPNZ controller status flags
typedef enum NPNZ_STATUS_FLAGS_e NPNZ_STATUS_FLAGS_t; ///< NPNZ controller status flags data type

/***************************************************************************************************
 * @enum    NPNZ_STATUS_SATURATION_e
 * @brief   Enumeration of control loop saturation status bits
 **************************************************************************************************/
enum NPNZ_STATUS_SATURATION_e{
    NPNZ_SAT_CLEAR  = 0b0,                        ///< No saturation condition detected
    NPNZ_SAT_ACTIVE = 0b1                         ///< Saturation limit violation detected
};                                                ///< NPNZ output saturation status bits
typedef enum NPNZ_STATUS_SATURATION_e NPNZ_STATUS_SATURATION_t; ///< NPNZ output saturation status bits data type
extern volatile enum NPNZ_STATUS_SATURATION_e npnzEnumControlStatusSaturation; ///< List Object Control Status Saturation

/* Control flags (bit-field) */
/***************************************************************************************************
 * @enum    NPNZ_STATUS_AGC_ENABLE_e
 * @brief   Enumeration of Adaptive Gain Modulation enable/disable control bits
 **************************************************************************************************/
enum NPNZ_STATUS_AGC_ENABLE_e{
    NPNZ_AGC_DISABLED = 0b0,                      ///< Adaptive Gain Modulation is disabled
    NPNZ_AGC_ENABLED  = 0b1                       ///< Adaptive Gain Modulation is enabled
};                                                ///< Adaptive Gain Modulation control bits
typedef enum NPNZ_STATUS_AGC_ENABLE_e NPNZ_STATUS_AGC_ENABLE_t; ///< Adaptive Gain Modulation control bits data type
extern volatile enum NPNZ_STATUS_AGC_ENABLE_e npnzEnumControlAgcEnable; ///< List Object Control AGC Enable

/***************************************************************************************************
 * @enum    NPNZ_STATUS_SOURCE_SWAP_e
 * @brief   Enumeration of control input port swap control bits
 **************************************************************************************************/
enum NPNZ_STATUS_SOURCE_SWAP_e{
    NPNZ_SOURCE_DEFAULT = 0b0,                    ///< Controller source ports are not swapped, primary source is active input
    NPNZ_SOURCE_SWAPED  = 0b1                     ///< Controller source ports are swapped, alternate source is active input
};                                                ///< NPNZ Source Port Swap Control bits
typedef enum NPNZ_STATUS_SOURCE_SWAP_e NPNZ_STATUS_SOURCE_SWAP_t; ///< NPNZ Source Port Swap Control bits data type
extern volatile enum NPNZ_STATUS_SOURCE_SWAP_e npnzEnumControlSourceSwap; ///< List Object Control Source Swap

/***************************************************************************************************
 * @enum    NPNZ_STATUS_TARGET_SWAP_e
 * @brief   Enumeration of control output port swap control bits
 **************************************************************************************************/
enum NPNZ_STATUS_TARGET_SWAP_e{
    NPNZ_TARGET_DEFAULT = 0b0,                    ///< Controller target ports are not swapped, primary source is active output
    NPNZ_TARGET_SWAPED  = 0b1                     ///< Controller target ports are swapped, alternate target is active output
};                                                ///< NPNZ Target Port Swap Control bits
typedef enum NPNZ_STATUS_TARGET_SWAP_e NPNZ_STATUS_TARGET_SWAP_t; ///< NPNZ Target Port Swap Control bits
extern volatile enum NPNZ_STATUS_TARGET_SWAP_e npnzEnumControlTargetSwap; ///< List Object Control Target Swap

/***************************************************************************************************
 * @enum    NPNZ_STATUS_INPUT_INV_e
 * @brief   Enumeration of input value inversion control bits
 **************************************************************************************************/
enum NPNZ_STATUS_INPUT_INV_e{
    NPNZ_INPUT_DEFAULT  = 0b0,                    ///< Controller error value is not inverted
    NPNZ_INPUT_INVERTED = 0b1                     ///< Controller error value is inverted
};                                                ///< NPNZ Error Value Inversion Control bit
typedef enum NPNZ_STATUS_INPUT_INV_e NPNZ_STATUS_INPUT_INV_t; ///< NPNZ Error Value Inversion Control bit
extern volatile enum NPNZ_STATUS_INPUT_INV_e npnzEnumControlInputInversion; ///< List Object Control Input Inversion

/***************************************************************************************************
 * @enum    NPNZ_STATUS_ENABLE_e
 * @brief   Enumeration of control loop enable/disable control bits
 **************************************************************************************************/
enum NPNZ_STATUS_ENABLE_e{
    NPNZ_DISABLED = 0b0,                          ///< Controller error value is not inverted
    NPNZ_ENABLED  = 0b1                           ///< Controller error value is inverted
};                                                ///< NPNZ Controller Enable Control bit
typedef enum NPNZ_STATUS_ENABLE_e NPNZ_STATUS_ENABLE_t; ///< NPNZ Controller Enable Control bit
extern volatile enum NPNZ_STATUS_ENABLE_e npnzEnumControlEnable; ///< List Object Control Enable

/****************************************************************************************************
 * @struct  NPNZ_FLAGS_s
 * @brief   Structure providing all public enumerated lists of constants
 **************************************************************************************************** */
struct NPNZ_FLAGS_s
{
    volatile enum NPNZ_STATUS_FLAGS_e       StatusWordFlags; ///< List of all status and control flags of the NPNZ16b status word
    volatile enum NPNZ_STATUS_SATURATION_e  flagSaturation; ///< List of all status and control flags of the NPNZ16b status word
    volatile enum NPNZ_STATUS_AGC_ENABLE_e  flagAgcControl; ///< List of all status and control flags of the NPNZ16b status word
    volatile enum NPNZ_STATUS_SOURCE_SWAP_e flagSourceSwap; ///< List of State Machine Operating State IDs
    volatile enum NPNZ_STATUS_TARGET_SWAP_e flagTargetSwap; ///< List of State Machine Sub-State IDs
    volatile enum NPNZ_STATUS_INPUT_INV_e   flagCtrlInputInversion; ///< List of State Machine Operating State Return Values
    volatile enum NPNZ_STATUS_ENABLE_e      flagControlEnable; ///< List of Supported Control Modes
};                                                ///< Consolidated list of status bit value enumerations
typedef struct NPNZ_FLAGS_s NPNZ_FLAGS_t;         ///< Consolidated list of status bit value enumerations data type
extern volatile struct NPNZ_FLAGS_s npnzFlagList; ///< List object of consolidated status bit value enumerations

/***************************************************************************************************
 * @ingroup special-function-layer-npnz16-object-members
 * @struct  NPNZ_STATUS_s
 * @brief   NPNZ16b controller object status and control word
 * @extends NPNZ16b_s
 *
 * @details
 * The NPNZ16b_s status word is providing status flag bits for monitoring and controlling the
 * NPNZ16b control library code execution from outside the library module.
 *
 * 1) Status Byte
 * The low byte of the NPNZ16b_s status word is used for READ ONLY status flags, set and cleared
 * automatically by the control loop library routine.
 *
 * 2) Control Byte
 * The high byte of the status word is used for control flags, through which users can control
 * the control loop execution. This includes enabling/disabling the control loop execution,
 * switch between different input and output sources, invert input values or enable/disable
 * advanced functions.
 *
 ***************************************************************************************************/

/* Controller status data structure */
struct NPNZ_STATUS_s {
    union {
    struct {
        volatile bool lower_saturation_event : 1; ///< Bit 0: control loop is clamped at minimum output level
        volatile bool upper_saturation_event : 1; ///< Bit 1: control loop is clamped at maximum output level
        volatile unsigned : 1;                    ///< Bit 2: reserved
        volatile unsigned : 1;                    ///< Bit 3: reserved
        volatile unsigned : 1;                    ///< Bit 4: reserved
        volatile unsigned : 1;                    ///< Bit 5: reserved
        volatile unsigned : 1;                    ///< Bit 6: reserved
        volatile unsigned : 1;                    ///< Bit 7: reserved

        volatile unsigned : 1;                    ///< Bit 8: reserved
        volatile unsigned : 1;                    ///< Bit 9: reserved
        volatile unsigned : 1;                    ///< Bit 11: reserved
        volatile bool agc_enabled: 1;             ///< Bit 11: when set, Adaptive Gain Control Modulation is enabled
        volatile bool swap_target: 1;             ///< Bit 12: when set, AltTarget is used as data output of controller
        volatile bool swap_source: 1;             ///< Bit 13: when set, AltSource is used as data input to controller
        volatile bool invert_input: 1;            ///< Bit 14: when set, most recent error input value to controller is inverted
        volatile bool enabled : 1;                ///< Bit 15: enables/disables control loop execution
    } __attribute__((packed))bits;                ///< Controller status bit-field for direct bit access
    volatile uint16_t value;                      ///< Controller status full register access
    };
} __attribute__((packed));                        ///< Controller status word data structure allowing bit-wise access to status and control bits
typedef struct NPNZ_STATUS_s NPNZ_STATUS_t;       ///< Data type of controller status word data structure allowing bit-wise access to status and control bits

/***************************************************************************************************
 * @ingroup special-function-layer-npnz16-object-members
 * @struct  NPNZ_PORT_s
 * @brief   Data Input/Output Port declaration of memory addresses, signal offsets and normalization settings
 * @extends NPNZ_PORTS_s
 *
 * @details
 * The NPNZ_PORT_s data object defines the basic parameters required to read data from or write
 * data to user-defined memory addresses as well as offers data fields for additional settings
 * such as normalization scaling factors or signal offsets, which can be used to compensate analog
 * offsets and normalize raw ADC data to other ADC input values and their respective physical
 * struct NPNZ_PORT_t declares its individual source/target memory address, normalization
 * quantity.
 *
 ***************************************************************************************************/

/* Controller Input/Output Port */
struct NPNZ_PORT_s{

    volatile uint16_t* ptrAddress;                ///< Pointer to register or variable where the value is read from (e.g. ADCBUFx) or written to (e.g. PGxDC)
    volatile int16_t   NormScaler;                ///< Bit-shift scaler of the Q15 normalization factor
    volatile fractional NormFactor;               ///< Q15 normalization factor
    volatile int16_t   Offset;                    ///< Value/signal offset of this port

} __attribute__((packed));                        ///< Data structure defining parameters of a controller input or output port
typedef struct NPNZ_PORT_s NPNZ_PORT_t;           ///< Data structure data type defining parameters of a controller input or output port

/***************************************************************************************************
 * @ingroup special-function-layer-npnz16-object-members
 * @struct  NPNZ_PORTS_s
 * @brief   Filter Coefficient Arrays, Number Format Handling and Input/Output History Parameters
 * @extends NPNZ16b_s
 *
 * @details
 * The NPNZ_PORTS_t data object holds a list of nested NPNZ_PORT_t data objects, each
 * defining an individual controller input or output port. The NPNZ16b_s data objects defines
 * up to two input and two output ports of type struct NPNZ_PORT_t and one additional
 * pointer to an external, user-defined 16-bit reference source variable. Each port of type
 * struct NPNZ_PORT_t declares its individual source/target memory address, normalization
 * scaler and offset:
 *
 *  - Primary Source:   common feedback input object
 *  - Alternate Source: additional, alternate feedback input object (optional)
 *  - Primary Target:   common control output target object
 *  - Alternate Target: additional, alternate control output target object (optional)
 *  - Control Reference: pointer to external 16-bit reference source variable
 *
 ***************************************************************************************************/

/* List of Controller Input/Output Ports */
struct NPNZ_PORTS_s{

    volatile struct NPNZ_PORT_s Source;           ///< Primary data input port declaration
    volatile struct NPNZ_PORT_s AltSource;        ///< Secondary data input port declaration
    volatile struct NPNZ_PORT_s Target;           ///< Primary data output port declaration
    volatile struct NPNZ_PORT_s AltTarget;        ///< Secondary data output port declaration
    volatile uint16_t* ptrControlReference;       ///< Pointer to global variable of input register holding the controller reference value (e.g. uint16_t my_ref)

} __attribute__((packed));                        ///< Data structure merging all defined controller input and output ports
typedef struct NPNZ_PORTS_s NPNZ_PORTS_t;         ///< Data structure merging all defined controller input and output ports

/***************************************************************************************************
 * @ingroup special-function-layer-npnz16-object-members
 * @struct  NPNZ_FILTER_PARAMS_s
 * @brief   Filter Coefficient Arrays, Number Format Handling and Input/Output History Parameters
 * @extends NPNZ16b_s
 *
 * @details
 * The NPNZ_FILTER_PARAMS_t data object holds all configuration parameters of the compensation
 * filter. These parameters include pointers to external arrays of filter coefficients, error
 * and control history as well as number format normalization parameters like pre- and post-
 * scalers.
 *
 ***************************************************************************************************/

struct NPNZ_FILTER_PARAMS_s{

    volatile int32_t* ptrACoefficients;           ///< Pointer to A coefficients located in X-space
    volatile int32_t* ptrBCoefficients;           ///< Pointer to B coefficients located in X-space
    volatile fractional* ptrControlHistory;       ///< Pointer to n delay-line samples located in Y-space with first sample being the most recent
    volatile fractional* ptrErrorHistory;         ///< Pointer to n+1 delay-line samples located in Y-space with first sample being the most recent

    // Array size information
    volatile uint16_t ACoefficientsArraySize;     ///< Size of the A coefficients array in X-space
    volatile uint16_t BCoefficientsArraySize;     ///< Size of the B coefficients array in X-space
    volatile uint16_t ControlHistoryArraySize;    ///< Size of the control history array in Y-space
    volatile uint16_t ErrorHistoryArraySize;      ///< Size of the error history array in Y-space

    // Feedback scaling Input/Output Normalization
    volatile int16_t normPreShift;                ///< Normalization of ADC-resolution to Q15 (R/W)
    volatile int16_t normPostShiftA;              ///< Normalization of A-term control output to Q15 (R/W)
    volatile int16_t normPostShiftB;              ///< Normalization of B-term control output to Q15 (R/W)
    volatile int16_t normPostScaler;              ///< Control output normalization factor (Q15) (R/W)

    // P-Term Coefficients (for plant measurements only)
    volatile int16_t PTermScaler;                 ///< Q15 P-Term Coefficient Bit-Shift Scaler (R/W)
    volatile int16_t PTermFactor;                 ///< Q15 P-Term Coefficient Factor (R/W)

} __attribute__((packed));                        ///< Data structure for filter parameters such as pointer to history and coefficient arrays and number scaling factors
typedef struct NPNZ_FILTER_PARAMS_s NPNZ_FILTER_PARAMS_t; ///< Data structure for filter parameters such as pointer to history and coefficient arrays and number scaling factors

/***************************************************************************************************
 * @ingroup special-function-layer-npnz16-object-members
 * @struct  NPNZ_LIMITS_s
 * @brief   System Anti-Windup (Output Clamping) Thresholds
 * @extends NPNZ16b_s
 *
 * @details
 * The NPNZ_LIMITS_t data object holds all parameters required to automatically clamp the
 * most recent control output to user-defined thresholds. This data type allows the
 * definition of individual minimum and maximum output values for the NPNZ controller primary
 * and alternate output port.
 *
 * This feature is optional and needs to be enabled, configured and managed manually in
 * user code.
 *
 ***************************************************************************************************/

struct NPNZ_LIMITS_s{

    volatile int16_t MinOutput;                   ///< Minimum output value used for clamping (R/W)
    volatile int16_t MaxOutput;                   ///< Maximum output value used for clamping (R/W)
    volatile int16_t AltMinOutput;                ///< Alternate minimum output value used for clamping (R/W)
    volatile int16_t AltMaxOutput;                ///< Alternate maximum output value used for clamping (R/W)

} __attribute__((packed));                        ///< Data strucure holding control output clamping threshold values
typedef struct NPNZ_LIMITS_s  NPNZ_LIMITS_t;      ///< Data strucure holding control output clamping threshold values

/***************************************************************************************************
 * @ingroup special-function-layer-npnz16-object-members
 * @struct  NPNZ_ADC_TRGCTRL_s
 * @brief   Automated ADC Trigger handling
 * @extends NPNZ16b_s
 *
 * @details
 * The NPNZ_ADC_TRGCTRL_t data object holds all parameters required to automatically position
 * ADC triggers based on the most recent control output. This feature is used in voltage or
 * average current mode control to automatically track average values in triangular feedback
 * signal waveforms.
 *
 * This feature is optional and needs to be enabled, configured and managed manually in
 * user code.
 *
 ***************************************************************************************************/

struct NPNZ_ADC_TRGCTRL_s{

    volatile uint16_t* ptrADCTriggerARegister;    ///< Pointer to ADC trigger #1 register (e.g. TRIG1)
    volatile uint16_t ADCTriggerAOffset;          ///< ADC trigger #1 offset to compensate propagation delays
    volatile uint16_t* ptrADCTriggerBRegister;    ///< Pointer to ADC trigger #2 register (e.g. TRIG2)
    volatile uint16_t ADCTriggerBOffset;          ///< ADC trigger #2 offset to compensate propagation delays

} __attribute__((packed));                        ///< Automatic ADC trigger placement parameters for primary ADC trigger A and secondary trigger B
typedef struct NPNZ_ADC_TRGCTRL_s NPNZ_ADC_TRGCTRL_t; ///< Automatic ADC trigger placement parameters for primary ADC trigger A and secondary trigger B

/***************************************************************************************************
 * @ingroup special-function-layer-npnz16-object-members
 * @struct  NPNZ_DATA_PROVIDERS_s
 * @brief   Data Provider Target Memory Addresses
 * @extends NPNZ16b_s
 *
 * @details
 * The NPNZ_DATA_PROVIDERS_t data object holds pointers to external, user-defined, global
 * variables allowing the NPNZ controller to push internal data to external, user-defined,
 * global variables during the execution of the NPNZ controller, resulting in an automated
 * updated of user-code variable values during runtime.
 *
 * This feature is optional and needs to be enabled, configured and managed manually in
 * user code.
 *
 ***************************************************************************************************/

struct NPNZ_DATA_PROVIDERS_s{

    volatile uint16_t* ptrDProvControlInput;      ///< Pointer to external data buffer of most recent, raw control input
    volatile uint16_t* ptrDProvControlInputCompensated; ///< Pointer to external data buffer of most recent, compensated control input
    volatile uint16_t* ptrDProvControlError;      ///< Pointer to external data buffer of most recent control error
    volatile uint16_t* ptrDProvControlOutput;     ///< Pointer to external data buffer of most recent control output

} __attribute__((packed));                        ///< Automated data provider pointers used to push most recent data points to user-defined variables
typedef struct NPNZ_DATA_PROVIDERS_s NPNZ_DATA_PROVIDERS_t; ///< Automated data provider pointers used to push most recent data points to user-defined variables

/**************************************************************************************************
 * @ingroup special-function-layer-npnz16-object-members
 * @struct  NPNZ_EXTENSION_HOOKS_s
 * @brief   User Extension Function Call Parameters
 * @extends NPNZ16b_s
 *
 * @details
 * The NPNZ_EXTENSION_HOOKS_s data object holds all parameters required to call user-defined extension
 * functions supporting advanced use cases, which are not covered by the standard functions provided.
 * When enabled, the NPNZ controller can automatically call user-defined functions at specific points
 * within the control loop execution flow. Each function pointer is supporting function calls with one
 * additional, 16-bit wide function parameter for each extension call. These parameters can either be
 * variables or pointers to variables including start addresses of user defined data structures.
 *
 * Each extension function call is optional and needs to be enabled, configured and managed manually
 * in user code.
 *
 ***************************************************************************************************/

struct NPNZ_EXTENSION_HOOKS_s{

    volatile uint16_t ptrExtHookStartFunction;    ///< Pointer to Function which will be called at the beginning of the control loop. This function pointer is stored in the data field as common unsigned integer value and needs to be casted as such. Example: my_loop.ExtensionHooks.ptrExtHookStartFunction = (uint16_t)(&my_StartHookFunction).
    volatile uint16_t ExtHookStartFunctionParam;  ///< Parameter of function called (can be a variable or pointer to a data structure). This parameter is optional and needs to be supported by the assembly routine to be pushed to the user function. Parameter support can be enabled/disabled for each hook function by selecting the respective option in PS-DCLD.

    volatile uint16_t ptrExtHookSourceFunction;   ///< Pointer to Function which will be called after the source has been read and compensated. This function pointer is stored in the data field as common unsigned integer value and needs to be casted as such. Example: my_loop.ExtensionHooks.ptrExtHookSourceFunction = (uint16_t)(&my_SourceHookFunction).
    volatile uint16_t ExtHookSourceFunctionParam; ///< Parameter of function called (can be a variable or a pointer to a data structure). This parameter is optional and needs to be supported by the assembly routine to be pushed to the user function. Parameter support can be enabled/disabled for each hook function by selecting the respective option in PS-DCLD.

    volatile uint16_t ptrExtHookPreAntiWindupFunction; ///< Pointer to Function which will be called after the compensation filter computation is complete and before anti-windup clamping is applied. This function pointer is stored in the data field as common unsigned integer value and needs to be casted as such. Example: my_loop.ExtensionHooks.ptrExtHookPreAntiWindupFunction = (uint16_t)(&my_PreAntiWindupHookFunction).
    volatile uint16_t ExtHookPreAntiWindupFunctionParam; ///< Parameter of function called (can be a variable or a pointer to a data structure). This parameter is optional and needs to be supported by the assembly routine to be pushed to the user function. Parameter support can be enabled/disabled for each hook function by selecting the respective option in PS-DCLD.

    volatile uint16_t ptrExtHookPreTargetWriteFunction; ///< Pointer to Function which will be called before the most recent control output is written to target. This function pointer is stored in the data field as common unsigned integer value and needs to be casted as such. Example: my_loop.ExtensionHooks.ptrExtHookPreTargetWriteFunction = (uint16_t)(&my_PreTargetWriteHookFunction).
    volatile uint16_t ExtHookPreTargetWriteFunctionParam; ///< Parameter of function called (can be a variable or a pointer to a data structure). This parameter is optional and needs to be supported by the assembly routine to be pushed to the user function. Parameter support can be enabled/disabled for each hook function by selecting the respective option in PS-DCLD.

    volatile uint16_t ptrExtHookEndOfLoopFunction; ///< Pointer to Function which is called at the end of the control loop but will be bypassed when the control loop is disabled. This function pointer is stored in the data field as common unsigned integer value and needs to be casted as such. Example: my_loop.ExtensionHooks.ptrExtHookEndOfLoopFunction = (uint16_t)(&my_EndOfLoopHookFunction).
    volatile uint16_t ExtHookEndOfLoopFunctionParam; ///< Parameter of function called (can be a variable or a pointer to a data structure). This parameter is optional and needs to be supported by the assembly routine to be pushed to the user function. Parameter support can be enabled/disabled for each hook function by selecting the respective option in PS-DCLD.

    volatile uint16_t ptrExtHookExitFunction;     ///< Pointer to Function which is called at the end of the control loop and will also be called when the control loop is disabled. This function pointer is stored in the data field as common unsigned integer value and needs to be casted as such. Example: my_loop.ExtensionHooks.ptrExtHookExitFunction = (uint16_t)(&my_ExitHookFunction).
    volatile uint16_t ExtHookExitFunctionParam;   ///< Parameter of function called (can be a variable or a pointer to a data structure). This parameter is optional and needs to be supported by the assembly routine to be pushed to the user function. Parameter support can be enabled/disabled for each hook function by selecting the respective option in PS-DCLD.

} __attribute__((packed));                        ///< Set of function pointers and parameters used to tie in user-defined, external extension functions at specific points of the control loop execution
typedef struct NPNZ_EXTENSION_HOOKS_s NPNZ_EXTENSION_HOOKS_t; ///< Function pointers and parameters used to tie in user-defined, external extension functions at specific points of the control loop execution

/***************************************************************************************************
 * @ingroup special-function-layer-npnz16-object-members
 * @struct  NPNZ_GAIN_CONTROL_s
 * @brief   Adaptive Gain Control Modulation Parameters
 * @extends NPNZ16b_s
 *
 * @details
 * The NPNZ_GAIN_CONTROL_t data object holds all parameters required to perform real-time
 * gain modulation of the z-domain feedback loop. The loop gain is modulated by multiplying
 * the result of the NPNZ controller B-term with an additional scaling factor. This scaling
 * factor is represented by a fast floating point number, consisting of a factional factor
 * <AgcFactor> between -1 and 1 and an integer bit-shift scaler <AgcScaler>.
 *
 * This feature is optional and needs to be enabled, configured and managed manually in
 * user code.
 *
 ***************************************************************************************************/

struct NPNZ_GAIN_CONTROL_s{

    volatile uint16_t AgcScaler;                  ///< Bit-shift scaler of Adaptive Gain Modulation factor
    volatile fractional AgcFactor;                ///< Q15 value of Adaptive Gain Modulation factor
    volatile fractional AgcMedian;                ///< Q15 value of Adaptive Gain Modulation nominal operating point
    volatile uint16_t ptrAgcObserverFunction;     ///< Function Pointer to Observer function updating the AGC modulation factor. This function pointer is stored in the data field as common unsigned integer value and needs to be casted as such. Example: my_loop.GainControl.ptrAgcObserverFunction = (uint16_t)(&my_AGCFactorUpdate);

} __attribute__((packed));                        ///< Data structure holding parameters required for adaptive or manual loop gain manipulation during runtime
typedef struct NPNZ_GAIN_CONTROL_s NPNZ_GAIN_CONTROL_t; ///< Data structure data type holding parameters required for adaptive or manual loop gain manipulation during runtime

/***************************************************************************************************
 * @ingroup special-function-layer-npnz16-object-members
 * @struct  NPNZ_USER_DATA_BUFFER_s
 * @brief   User Data Space for Advanced Control Functions
 * @extends NPNZ16b_s

 * @details
 * The NPNZ_USER_DATA_BUFFER_t data object reserves four word of additional data space for
 * user parameters. These parameters may be handled by user code and are not assigned to any
 * specific, pre-defined functions.
 *
 ***************************************************************************************************/

struct NPNZ_USER_DATA_BUFFER_s{

    volatile uint16_t usrParam0;                  ///< generic 16-bit wide, user-defined parameter #1 for advanced control options
    volatile uint16_t usrParam1;                  ///< generic 16-bit wide, user-defined parameter #2 for advanced control options
    volatile uint16_t usrParam2;                  ///< generic 16-bit wide, user-defined parameter #3 for advanced control options
    volatile uint16_t usrParam3;                  ///< generic 16-bit wide, user-defined parameter #4 for advanced control options
    volatile uint16_t usrParam4;                  ///< generic 16-bit wide, user-defined parameter #5 for advanced control options
    volatile uint16_t usrParam5;                  ///< generic 16-bit wide, user-defined parameter #6 for advanced control options
    volatile uint16_t usrParam6;                  ///< generic 16-bit wide, user-defined parameter #7 for advanced control options
    volatile uint16_t usrParam7;                  ///< generic 16-bit wide, user-defined parameter #8 for advanced control options

} __attribute__((packed));                        ///< Generic data buffer for undetermined use. These data buffers may be used by advanced control algorithms or be used by proprietary user code modules
typedef struct NPNZ_USER_DATA_BUFFER_s NPNZ_USER_DATA_BUFFER_t; ///< Data type of generic data buffer for undetermined use. These data buffers may be used by advanced control algorithms or be used by proprietary user code modules

/***************************************************************************************************
 * @ingroup special-function-layer-npnz16-data-objects
 * @struct  NPNZ16b_s
 * @brief   Global NPNZ controller data object
 *
 * @details
 * The NPNZ16b_s data object holds all configuration, status, control and monitoring values
 * of a z-domain lead-lag compensator based controller. All data types of this data object,
 * including floating, are scaled to a 16 bit number space, optimized for code execution on
 * Microchip dsPIC33 family of devices. Please refer to the description of nested data
 * structures above for more information about nested data objects.
 *
 ***************************************************************************************************/

struct NPNZ16b_s {

    volatile struct NPNZ_STATUS_s status;         ///< Control Loop Status and Control flags
    volatile struct NPNZ_PORTS_s Ports;           ///< Controller input and output ports
    volatile struct NPNZ_FILTER_PARAMS_s Filter;  ///< Filter parameters such as pointer to history and coefficient arrays and number scaling
    volatile struct NPNZ_GAIN_CONTROL_s GainControl; ///< Parameter section for advanced control options
    volatile struct NPNZ_LIMITS_s Limits;         ///< Input and output clamping values
    volatile struct NPNZ_ADC_TRGCTRL_s ADCTriggerControl; ///< Automatic ADC trigger placement options for ADC Trigger A and B
    volatile struct NPNZ_DATA_PROVIDERS_s DataProviders; ///< Automated data sources pushing recent data points to user-defined variables
    volatile struct NPNZ_EXTENSION_HOOKS_s ExtensionHooks; ///< User extension function triggers using function pointers with parameters
    volatile struct NPNZ_USER_DATA_BUFFER_s Advanced; ///< Parameter section for advanced user control options

} __attribute__((packed)) ;                       ///< Generic NPNZ16b Controller Object. This data structure is the main API data object providing single-point access to all controller settings and parameters
typedef struct NPNZ16b_s NPNZ16b_t;               ///< Generic NPNZ16b Controller Object Data Type. This data structure is the main API data object providing single-point access to all controller settings and parameters

/* ********************************************************************************/

#endif                                            // end of __SPECIAL_FUNCTION_LAYER_LIB_NPNZ16B_H__ header file section


//**********************************************************************************
// Download latest version of this tool here: https://areiter128.github.io/DCLD
//**********************************************************************************


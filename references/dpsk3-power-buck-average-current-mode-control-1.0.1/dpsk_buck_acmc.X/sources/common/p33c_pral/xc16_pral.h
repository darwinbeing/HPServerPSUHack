/*************************************************************************************************
 * <b>Software License Agreement</b>
 *
 * Copyright © 2020 Microchip Technology Inc.  All rights reserved. Microchip licenses to you the
 * right to use, modify, copy and distribute Software only when embedded on a Microchip 
 * microcontroller or digital signal controller, which is integrated into your product or third 
 * party product (pursuant to the sublicense terms in the accompanying license agreement).
 *
 * You should refer to the license agreement accompanying this Software for additional information 
 * regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR 
 * IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR 
 * OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT  
 * LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS  
 * OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY  
 * THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *
 ************************************************************************************************/
/*************************************************************************************************
 * @ingroup xc16-pral
 * @{
 *************************************************************************************************
 * @file
 * @brief Includes generic peripheral special function register abstraction layer header files
 *
 * @details
 * This additional header file contains include paths to header files providing additional 
 * peripheral special function register abstraction drivers. These PRAL drivers provide 
 * abstracted data structures of peripheral module and instance register sets used as virtual
 * register templates to create silicon-independent firmware modules replacing direct peripheral 
 * register manipulation operations by abstracted versions, which are mapped to a specific 
 * module or instance at runtime.
 * 
 ************************************************************************************************/

#ifndef MCAL_P33C_SFR_ABSTRACTION_DRIVER_H
#define MCAL_P33C_SFR_ABSTRACTION_DRIVER_H

// Device header file
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer types header file
#include <stdbool.h> // include standard boolean types header file
#include <stddef.h> // include standard definition types header file

#include "p33smps_devices.h" // Include extended device identifier macros

#if defined (__P33SMPS_CK__) || defined (__P33SMPS_CH__)

#include "p33c_ccp.h"  ///< include Capture/Compare instance SFR abstraction driver file
#include "p33c_dac.h"  ///< include Digital-to-Analog Converter module & instance SFR abstraction driver file
#include "p33c_dsp.h"  ///< include Digital Signal Processor configuration SFR abstraction driver file
#include "p33c_gpio.h" ///< include GPIO instance SFR abstraction driver file
#include "p33c_opa.h"  ///< include Operational Amplifier SFR abstraction driver file
#include "p33c_osc.h"  ///< include Oscillator module SFR abstraction driver file
#include "p33c_pps.h"  ///< include Peripheral Pin Select SFR abstraction driver file
#include "p33c_pwm.h"  ///< include high-speed PWM module & instance SFR abstraction driver file

#include "p33c_macros.h"  ///< include generic dsPIC33C CPU instruction macros

#else
    #pragma message "warning: currently selected device is not supported by XC16 Periperhal Register Abstraction Layer (PRAL) drivers"
#endif

#endif // End of MCAL_P33C_SFR_ABSTRACTION_DRIVER_H

/** @}*/ // end of group xc16-pral
// end of file

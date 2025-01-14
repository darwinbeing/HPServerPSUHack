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
 * File:   dev_buck_ptemp_adc.h
 * Author: M91406
 * Comments: ADC Peripheral Special Function Register Configuration Template
 * Revision history: 
 *   10/29/2020     1.0     initial release
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BUCK_CONVERTER_PERIPHERAL_CONFIGURATION_ADC_H
#define	BUCK_CONVERTER_PERIPHERAL_CONFIGURATION_ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

/**
 * @ingroup lib-layer-buck-ptemplate-properties-defines
 * @{
 */
/* ********************************************************************************
 * ADC BASE MODULE BASE REGISTERS CONFIGURATION
 * *******************************************************************************/
/**
 * @def ADC_POWERUP_TIMEOUT
 * @brief Digital-To-Analog Converter and High Speed Comparator Special Function Register Set
 */  
#define ADC_POWERUP_TIMEOUT  5000
/* SLPxDAT: DACx SLOPE DATA REGISTER => Timing Register: User value will be set in code */

// ADC specific declarations
/**
 * @def ADC_CORE_MASK
 * @brief This define masks all cores of ADC
 */
#define ADC_CORE_MASK           0b0000000010001111

/**
 * @def ADC_CORE0_MASK_INDEX
 * @brief This define masks core 0 of ADC
 */
#define ADC_CORE0_MASK_INDEX    0b0000000000000001

/**
 * @def ADC_CORE1_MASK_INDEX
 * @brief This define masks core 1 of ADC
 */
#define ADC_CORE1_MASK_INDEX    0b0000000000000010

/**
 * @def ADC_CORE2_MASK_INDEX
 * @brief This define masks core 2 of ADC
 */
#define ADC_CORE2_MASK_INDEX    0b0000000000000100

/**
 * @def ADC_CORE3_MASK_INDEX
 * @brief This define masks core 3 of ADC
 */
#define ADC_CORE3_MASK_INDEX    0b0000000000001000

/**
 * @def ADC_SHRCORE_MASK_INDEX
 * @brief This define masks shared core of ADC
 */
#define ADC_SHRCORE_MASK_INDEX  0b0000000010000000

/* ********************************************************************************
 * ADC INPUT INSTANCE CONFIGURATION
 * *******************************************************************************/


/* REGISTER VALUE DECLARATION TEMPLATE */
/* PCLKCON: PWM CLOCK CONTROL REGISTER

                           ________________ BIT 15: HRRDY: High-Resolution Ready bit
                          | _______________ BIT 14: HRERR: High-Resolution Error bit
                          || ______________ BIT 13: (unimplemented) 
                          ||| _____________ BIT 12: (unimplemented) 
                          |||| ____________ BIT 11: (unimplemented) 
                          ||||| ___________ BIT 10: (unimplemented)
                          |||||| __________ BIT  9: (unimplemented)
                          ||||||| _________ BIT  8: LOCK: Lock bit
                          |||||||| ________ BIT  7: (unimplemented)
                          ||||||||| _______ BIT  6: (unimplemented)
                          |||||||||| ______ BIT  5: DIVSEL[1:0]: PWM Clock Divider Selection bits
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: (unimplemented)
                          ||||||||||||| ___ BIT  2: (unimplemented)
                          |||||||||||||| __ BIT  1: MCLKSEL[1:0]: PWM Master Clock Selection bits
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  *//*
#define REG_PCLKCON     0b0000000000000011   */



#endif	/* BUCK_CONVERTER_PERIPHERAL_CONFIGURATION_ADC_H */

/**@}*/
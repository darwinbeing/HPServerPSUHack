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
 * File:   dev_buck_pconfig.h
 * Author: M91406
 * Comments: Peripheral configuration template header file for generic buck converter driver
 * Revision history: 
 * 1.0  initial release
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BUCK_CONVERTER_PERIPHERAL_CONFIGURATION_H
#define	BUCK_CONVERTER_PERIPHERAL_CONFIGURATION_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

#include "dev_buck_typedef.h"

/* ********************************************************************************
 * DEVICE GPIO INSTANCE CONFIGURATION
 * *******************************************************************************/

extern volatile uint16_t buckGPIO_Initialize(volatile struct BUCK_CONVERTER_s* buckInstance);
extern volatile uint16_t buckGPIO_Set(volatile struct BUCK_GPIO_INSTANCE_s* buckGPIOInstance);
extern volatile uint16_t buckGPIO_Clear(volatile struct BUCK_GPIO_INSTANCE_s* buckGPIOInstance);
extern volatile bool     buckGPIO_GetPinState(volatile struct BUCK_GPIO_INSTANCE_s* buckGPIOInstance);

/* ********************************************************************************
 * HIGH SPEED PWM MODULE CONFIGURATION
 * *******************************************************************************/
    
extern volatile uint16_t buckPWM_ModuleInitialize(volatile struct BUCK_CONVERTER_s* buckInstance);
extern volatile uint16_t buckPWM_ChannelInitialize(volatile struct BUCK_CONVERTER_s* buckInstance);
extern volatile uint16_t buckPWM_Start(volatile struct BUCK_CONVERTER_s* buckInstance);
extern volatile uint16_t buckPWM_Stop(volatile struct BUCK_CONVERTER_s* buckInstance);
extern volatile uint16_t buckPWM_Suspend(volatile struct BUCK_CONVERTER_s* buckInstance);
extern volatile uint16_t buckPWM_Resume(volatile struct BUCK_CONVERTER_s* buckInstance);

extern volatile struct P33C_PWM_MODULE_s buckPwmModuleConfig;
extern volatile struct P33C_PWM_GENERATOR_s buckPwmGeneratorConfig;

/* ********************************************************************************
 * HIGH SPEED ADC MODULE CONFIGURATION
 * *******************************************************************************/

extern volatile uint16_t buckADC_ModuleInitialize(void);
extern volatile uint16_t buckADC_ChannelInitialize(volatile struct BUCK_ADC_INPUT_SETTINGS_s* adcInstance);
extern volatile uint16_t buckADC_Start(void);

/* ********************************************************************************
 * HIGH ANALOG COMPARATOR INSTANCE CONFIGURATION
 * *******************************************************************************/

// (not supported yet)

#endif	/* BUCK_CONVERTER_PERIPHERAL_CONFIGURATION_H */


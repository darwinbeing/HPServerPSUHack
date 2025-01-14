/**
 *  (c) 2020 Microchip Technology Inc. and its subsidiaries.
 *
 *  Subject to your compliance with these terms, you may use Microchip software
 *  and any derivatives exclusively with Microchip products. You're responsible
 *  for complying with 3rd party license terms applicable to your use of 3rd
 *  party software (including open source software) that may accompany Microchip
 *  software.
 *
 *  SOFTWARE IS "AS IS." NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 *  APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 *  MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 *  INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 *  WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
 *  HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
 *  THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
 *  CLAIMS RELATED TO THE SOFTWARE WILL NOT EXCEED AMOUNT OF FEES, IF ANY,
 *  YOU PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 *  @file    dev_buck_converter.h
 *  @brief   This file contains APIs to for the power control
 *
 *  @note
 *  Microchip Technology Inc. has followed development methods required by
 *  IEC-60730 and performed extensive validation and static testing to ensure
 *  that the code operates as intended. Any modification to the code can
 *  invalidate the results of Microchip's validation and testing.
 *
 */

/* 
 * File:   dev_buck_converter.h
 * Author: M91406
 * Comments: power controller functions for buck converter
 * Revision history: 
 * 1.0  initial version
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BUCK_CONVERTER_STATE_MACHINE_H
#define	BUCK_CONVERTER_STATE_MACHINE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer types header file
#include <stdbool.h> // include standard boolean types header file

#include "dev_buck_typedef.h"

 // ==============================================================================================
 // BUCK CONVERTER PUBLIC API FUNCTION CALL PROTOTYPES
 // ==============================================================================================

extern volatile uint16_t drv_BuckConverter_Initialize(volatile struct BUCK_CONVERTER_s* buckInstance);
extern volatile uint16_t drv_BuckConverter_Execute(volatile struct BUCK_CONVERTER_s* buckInstance);
extern volatile uint16_t drv_BuckConverter_Start(volatile struct BUCK_CONVERTER_s* buckInstance);
extern volatile uint16_t drv_BuckConverter_Stop(volatile struct BUCK_CONVERTER_s* buckInstance);
extern volatile uint16_t drv_BuckConverter_Suspend(volatile struct BUCK_CONVERTER_s* buckInstance);
extern volatile uint16_t drv_BuckConverter_Resume(volatile struct BUCK_CONVERTER_s* buckInstance);

#endif	/* BUCK_CONVERTER_STATE_MACHINE_H */

// END OF FILE

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
 * File:   system_initialize.h
 * Author: M91406
 * Comments: Combines the fundamental MCU and peripheral initialization routines
 * Revision history: 
 *    1.0   initial release
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef APPLICAITON_SYSTEM_INITIALIZE_H
#define	APPLICAITON_SYSTEM_INITIALIZE_H

#include <xc.h> // include processor files - each processor file is guarded.  

/* INCLUDE DEFAULT SYSTEM COMPONENT INITIALIZATION MODULES */

#include "config/init/init_dsp.h"
#include "config/init/init_fosc.h"
#include "config/init/init_gpio.h"
#include "config/init/init_timer1.h"

/* INCLUDE USER SPECIAL FUNCTION COMPONENT INITIALIZATION MODULES */

#include "config/init/init_dac.h"
#include "config/init/init_opa.h"

/* PUCLIC FUNCTION CALL PROTOTYPES */

extern volatile uint16_t SYSTEM_Initialize(void);
extern volatile uint16_t sysUserTasks_Initialize(void);
extern volatile uint16_t sysUserPeriperhals_Initialize(void);

#endif	/* APPLICAITON_SYSTEM_INITIALIZE_H */


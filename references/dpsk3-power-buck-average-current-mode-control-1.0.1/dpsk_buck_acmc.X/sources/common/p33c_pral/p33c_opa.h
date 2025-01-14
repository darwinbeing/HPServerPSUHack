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

/**************************************************************************************************
 * @file p33c_opa.h
 * @see  p33c_opa.c
 * @brief Generic Operational Amplifier Driver Module (header file)
 * @details 
 * This additional header file contains defines for all required bit-settings of all related 
 * special function registers of a peripheral module and/or instance. 
 * This file is an additional header file on top of the generic device header file.
 *	
 *************************************************************************************************/

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef P33C_OPA_SFR_ABSTRACTION_H
#define	P33C_OPA_SFR_ABSTRACTION_H

// Include standard header files
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types


// GENERIC PDM OP-AMP MODULE SPECIAL FUNCTION REGISTER SET
#ifndef P33C_OPA_MODULE_s

    struct P33C_OPA_MODULE_s{
        union {
            volatile struct tagAMPCON1LBITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        } AmpCon1L;    // (AMPCON1L) OP-AMP CONTROL 1 REGISTER LOW
        union {
            volatile struct tagAMPCON1HBITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        } AmpCon1H;    // (AMPCON1H) OP-AMP CONTROL 2 REGISTER LOW
    } __attribute__((packed));
	typedef struct P33C_OPA_MODULE_s P33C_OPA_MODULE_t; // PDM OP-AMP MODULE REGISTER SET

#endif


/* ********************************************************************************************* * 
 * API FUNCTION PROTOTYPES
 * ********************************************************************************************* */

extern volatile struct P33C_OPA_MODULE_s* p33c_OpaModule_GetHandle(void);

extern volatile uint16_t p33c_OpaModule_Dispose(void);

extern volatile struct P33C_OPA_MODULE_s p33c_OpaModule_ConfigRead(void);
extern volatile uint16_t p33c_OpaModule_ConfigWrite(
                    volatile struct P33C_OPA_MODULE_s opaConfig
                );


/* ********************************************************************************************* * 
 * OP-AMP INSTANCE CONFIGURATION TEMPLATES
 * ********************************************************************************************* */
extern volatile struct P33C_OPA_MODULE_s opaModuleConfigClear;
extern volatile struct P33C_OPA_MODULE_s opaModuleDefault;

#endif	/* P33C_OPA_SFR_ABSTRACTION_H */

// end of file

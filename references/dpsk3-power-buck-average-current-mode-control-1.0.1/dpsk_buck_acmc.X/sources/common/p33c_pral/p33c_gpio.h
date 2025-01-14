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
 * @file p33c_gpio.h
 * @see  p33c_gpio.c
 * @brief Generic Digital Signal Processor Driver Module (header file)
 * @details
 * This additional header file contains defines for all required bit-settings of all related 
 * special function registers of a peripheral module and/or instance. 
 * This file is an additional header file on top of the generic device header file.
 * 
 *************************************************************************************************/
/* Revision history: 
 * 12/10/20     1.0     initial release
 *************************************************************************************************/

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef P33C_GPIO_SFR_ABSTRACTION_H
#define	P33C_GPIO_SFR_ABSTRACTION_H

// Include standard header files
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

// GENERIC GPIO INSTANCE SPECIAL FUNCTION REGISTER SET
// DO NOT REORDER ELEMENTS AS THEY MAP ONTO DEVICE REGISTER MEMORY !!!
#ifndef P33C_GPIO_INSTANCE_s

    struct P33C_GPIO_INSTANCE_s {
        union {   
            volatile struct tagLATABITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        }ANSELx; // ANSELx: ANALOG SELECT FOR PORTx REGISTER
        union {   
            volatile struct tagLATABITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        }TRISx; // TRISx: OUTPUT ENABLE FOR PORTx REGISTER
        union {   
            volatile struct tagLATABITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        }PORTx; // PORTx: INPUT DATA FOR PORTx REGISTER
        union {   
            volatile struct tagLATABITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        }LATx; // LATx: OUTPUT DATA FOR PORTx REGISTER
        union {   
            volatile struct tagLATABITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        }ODCx; // ODCx: OPEN-DRAIN ENABLE FOR PORTx REGISTER
        union {   
            volatile struct tagLATABITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        }CNPUx; // CNPUx: CHANGE NOTIFICATION PULL-UP ENABLE FOR PORTx REGISTER
        union {   
            volatile struct tagLATABITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        }CNPDx; // CNPDx: CHANGE NOTIFICATION PULL-DOWN ENABLE FOR PORTx REGISTER
        union {   
            volatile struct tagLATABITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        }CNCONx; // CNCONx: CHANGE NOTIFICATION CONTROL FOR PORTx REGISTER
        union {   
            volatile struct tagLATABITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        }CNEN0x; // CNEN0x: CHANGE NOTIFICATION INTERRUPT ENABLE FOR PORTx REGISTER
        union {   
            volatile struct tagLATABITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        }CNSTATx; // CNSTATx: CHANGE NOTIFICATION STATUS FOR PORTx REGISTER
        union {   
            volatile struct tagLATABITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        }CNEN1x; // CNEN1x: CHANGE NOTIFICATION EDGE SELECT FOR PORTx REGISTER
        union {   
            volatile struct tagLATABITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        }CNFx; // CNFx: CHANGE NOTIFICATION INTERRUPT FLAG FOR PORTx REGISTER
    } __attribute__((packed)); // GPIO REGISTER SET
	typedef struct P33C_GPIO_INSTANCE_s  P33C_GPIO_INSTANCE_t;
	
    #define P33C_GPIO_SFR_OFFSET  ((volatile uint16_t)&ANSELB - (volatile uint16_t)&ANSELA)

#endif

/* ********************************************************************************************* * 
 * API FUNCTION PROTOTYPES
 * ********************************************************************************************* */
    
extern volatile struct P33C_GPIO_INSTANCE_s* p33c_GpioInstance_GetHandle(
            volatile uint16_t gpio_Instance
        );
    
extern volatile uint16_t p33c_GpioInstance_Dispose(
            volatile uint16_t gpioInstance
        );    
    
extern volatile struct P33C_GPIO_INSTANCE_s p33c_GpioInstance_ConfigRead(
            volatile uint16_t gpioInstance
        );

extern volatile uint16_t p33c_GpioInstance_ConfigWrite(
            volatile uint16_t gpioInstance, 
            volatile struct P33C_GPIO_INSTANCE_s gpioConfig
        );
    
/* ********************************************************************************************* * 
 * GPIO INSTANCE CONFIGURATION TEMPLATES
 * ********************************************************************************************* */
extern volatile struct P33C_GPIO_INSTANCE_s gpioConfigClear;
    
#endif	/* P33C_GPIO_SFR_ABSTRACTION_H */


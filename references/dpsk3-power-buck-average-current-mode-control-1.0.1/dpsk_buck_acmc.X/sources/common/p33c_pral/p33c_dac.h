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
 * @file p33c_dac.h
 * @see  p33c_dac.c
 * @brief Generic Digital-To-Analog Converter Driver Module (header file)
 * @details
 * This additional header file contains defines for all required bit-settings of all related 
 * special function registers of a peripheral module and/or instance. 
 * This file is an additional header file on top of the generic device header file.
 * 
 *************************************************************************************************/

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef P33C_DAC_SFR_ABSTRACTION_H
#define	P33C_DAC_SFR_ABSTRACTION_H

// Include standard header files
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types


// GENERIC PDM DAC MODULE SPECIAL FUNCTION REGISTER SET
#ifndef P33C_DAC_MODULE_s

/***********************************************************************************
 * @ingroup lib-layer-pral-properties-public-dac
 * @struct P33C_DAC_MODULE_s
 * @brief Abstracted set of Special Function Registers of a Digital-to-Analog Converter peripheral
 * @details
 * This structure defines an abstracted set of Special Function Registers of 
 * Digital-to-Analog Converter peripheral module base registers. Users can use this 
 * abstracted set of registers to capture register settings from or write generic  
 * register settings to these peripheral base registers.
 * 
 **********************************************************************************/

    struct P33C_DAC_MODULE_s{
        union {
            volatile struct tagDACCTRL1LBITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        } DacModuleCtrl1L;    // (DACCTRL1L) DAC CONTROL 1 REGISTER LOW
        volatile unsigned : 16;         // (reserved)
        union {
            volatile struct tagDACCTRL2LBITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        } DacModuleCtrl2L;    // (DACCTRL2L) DAC CONTROL 2 REGISTER LOW
        union {
            volatile struct tagDACCTRL2HBITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        } DacModuleCtrl2H;    // (DACCTRL2H) DAC CONTROL 2 REGISTER HIGH
    } __attribute__((packed));
    typedef struct P33C_DAC_MODULE_s P33C_DAC_MODULE_t; // PDM DAC MODULE REGISTER SET
    
#endif

// GENERIC PDM DAC INSTANCE SPECIAL FUNCTION REGISTER SET
#ifndef P33C_DAC_INSTANCE_s    

/***********************************************************************************
 * @ingroup lib-layer-pral-properties-public-dac
 * @struct P33C_DAC_MODULE_s
 * @brief Abstracted set of Special Function Registers of a Digital-to-Analog Converter peripheral instance
 * @details
 * This structure defines an abstracted set of Special Function Registers of
 * Digital-to-Analog Converter peripheral instance registers. It provides an 
 * abstracted set of registers to capture register settings from or write generic  
 * register settings to this peripheral instance.
 * 
 **********************************************************************************/
    
    struct P33C_DAC_INSTANCE_s{
        union {
            volatile struct tagDAC1CONLBITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        } DACxCONL; // DACxCONL: DACx CONTROL LOW REGISTER
        union {
            volatile struct tagDAC1CONHBITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        } DACxCONH; // DACxCONH: DACx CONTROL HIGH REGISTER
        union {
            volatile struct tagDAC1DATLBITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        } DACxDATL; // DACxDATL: DACx DATA LOW REGISTER
        union {
            volatile struct tagDAC1DATHBITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        } DACxDATH; // DACxDATH: DACx DATA HIGH REGISTER
        union {
            volatile struct tagSLP1CONLBITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        } SLPxCONL; // SLPxCONL: DACx SLOPE CONTROL LOW REGISTER
        union {
            volatile struct tagSLP1CONHBITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        } SLPxCONH; // SLPxCONH: DACx SLOPE CONTROL HIGH REGISTER
        union {
            volatile struct tagSLP1DATBITS bits; // Register bit-field
            volatile uint16_t value; // 16-bit wide register value
        } SLPxDAT;  // SLPxDAT: DACx SLOPE DATA REGISTER
    } __attribute__((packed));
    typedef struct P33C_DAC_INSTANCE_s P33C_DAC_INSTANCE_t; // PDM DAC INSTANCE REGISTER SET
    
/*********************************************************************************
 * @ingroup lib-layer-pral-properties-public-dac
 * @def     P33C_CCPGEN_SFR_OFFSET
 * @brief   Derives the address offset between two peripheral instances
 * @details
 * This macro derives the address offset between two peripheral instances. 
 * Users can use this address offset to derive the start address to/from which
 * the register set should be written or read.
 **********************************************************************************/
    #define P33C_DAC_SFR_OFFSET  ((volatile uint16_t)&DAC2CONL - (volatile uint16_t)&DAC1CONL)

#endif


/* ********************************************************************************************* * 
 * API FUNCTION PROTOTYPES
 * ********************************************************************************************* */

extern volatile struct P33C_DAC_MODULE_s* p33c_DacModule_GetHandle(void);

extern volatile uint16_t p33c_DacModule_Dispose(void);

extern volatile struct P33C_DAC_MODULE_s p33c_DacModule_ConfigRead(void);
extern volatile uint16_t p33c_DacModule_ConfigWrite(
                    volatile struct P33C_DAC_MODULE_s dacConfig
                );


extern volatile struct P33C_DAC_INSTANCE_s* p33c_DacInstance_GetHandle(
                    volatile uint16_t dacInstance
                );

extern volatile uint16_t p33c_DacInstance_Dispose(
                    volatile uint16_t dacInstance
                );

extern volatile struct P33C_DAC_INSTANCE_s p33c_DacInstance_ConfigRead(
                    volatile uint16_t dacInstance
                );

extern volatile uint16_t p33c_DacInstance_ConfigWrite(
                    volatile uint16_t dacInstance,
                    volatile struct P33C_DAC_INSTANCE_s dacConfig
                );

/* ********************************************************************************************* * 
 * DAC INSTANCE CONFIGURATION TEMPLATES
 * ********************************************************************************************* */
extern volatile struct P33C_DAC_MODULE_s dacModuleConfigClear;
extern volatile struct P33C_DAC_INSTANCE_s dacConfigClear;
extern volatile struct P33C_DAC_MODULE_s dacModuleDefault;

#endif	/* P33C_DAC_SFR_ABSTRACTION_H */
// END OF FILE

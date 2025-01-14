/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/*@@p33c_pwm.h
 * ************************************************************************************************
 * Summary:
 * Generic High-Speed SMPS PWM Driver Module & Generator Instances (header file)
 *
 * Description:
 * This additional header file contains defines for all required bit-settings of all related 
 * special function registers of a peripheral module and/or instance. 
 * This file is an additional header file on top of the generic device header file.
 * 
 * See Also:
 *	p33c_pwm.c
 * 
 * ***********************************************************************************************#
 * Revision history: 
 * 06/22/20     1.0     initial release
 * 09/16/20     1.1     Simplified module and generator addressing
 * ***********************************************************************************************/

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef P33C_PWM_SFR_ABSTRACTION_H
#define	P33C_PWM_SFR_ABSTRACTION_H

// Include standard header files
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

//#ifndef __dsPIC33C__
//   #error "peripheral driver p33c_pwm.h does not support the selected device"
//#endif

    
/* GENERIC SPECIAL FUNCTION REGISTER (SFR) SETS
 * ============================================
 * 
 * Direct addressing of Special Function Registers (SFR) in user code is good 
 * enough for a specific application but makes it harder to maintain the code 
 * or migrate the code across devices and applications. 
 * 
 * Especially with complex configurations with multiple dependencies across various 
 * peripherals, migrating configurations can become very challenging to review, 
 * debug and migrate. 
 * 
 * Therefore this PWM configuration example uses a 'virtual PWM generator' to set up 
 * the required PWM configuration but provides a function allowing users to write this 
 * configuration to any PWM generator of choice. Hence, pre-defined configurations 
 * can be mapped on specific peripheral instance by using their index defined by the 
 * data sheet (e.g. pgInstance = 1 maps pre-defined configuration to PG1)
 * 
 * The data types used for each SFR in the P33C_PWM_GENERATOR_t data structure below,
 * are data types declared in the common C-header file of the device (e.g. <d33CK256MP506.h>). 
 * These header files are provided with the XC16 C-Compiler installation and don't have to be 
 * written nor changed manually. 
 * 
 */

// GENERIC PWM MODULE SPECIAL FUNCTION REGISTER SET

#ifndef P33C_PWM_MODULE_s     

    typedef struct P33C_PWM_MODULE_s {
        
        union {
            struct tagPCLKCONBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } vPCLKCON; // PCLKCON: PWM CLOCK CONTROL REGISTER
        union {   
        struct {
            uint16_t FSCL : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } vFSCL; // FSCL: FREQUENCY SCALE REGISTER
        union {   
        struct {
            uint16_t FSMINPER : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }vFSMINPER; // FSMINPER: FREQUENCY SCALING MINIMUM PERIOD REGISTER
        union {   
        struct {
            uint16_t MPHASE : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }vMPHASE; // MPHASE: MASTER PHASE REGISTER
        union {   
        struct {
            uint16_t MDC : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }vMDC; // MDC: MASTER DUTY CYCLE REGISTER
        union {   
        struct {
            uint16_t MPER : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }vMPER; // MPER: MASTER PERIOD REGISTER
        union {   
        struct {
            uint16_t LFSR : 15; // Register bit-field
            uint16_t      : 1;  // (reserved)
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }vLFSR; // LFSR: LINEAR FEEDBACK SHIFT REGISTER
        union {   
            struct tagCMBTRIGLBITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }vCMBTRIGL; // CMBTRIGL: COMBINATIONAL TRIGGER REGISTER LOW
        union {   
            struct tagCMBTRIGHBITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }vCMBTRIGH; // CMBTRIGL: COMBINATIONAL TRIGGER REGISTER HIGH
        union {   
            struct tagLOGCONABITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }LOGCON_A; // LOGCONx: COMBINATORIAL PWM LOGIC CONTROL REGISTER x
        union {   
            struct tagLOGCONABITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }LOGCON_B; // LOGCONx: COMBINATORIAL PWM LOGIC CONTROL REGISTER x
        union {   
            struct tagLOGCONABITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }LOGCON_C; // LOGCONx: COMBINATORIAL PWM LOGIC CONTROL REGISTER x
        union {   
            struct tagLOGCONABITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }LOGCON_D; // LOGCONx: COMBINATORIAL PWM LOGIC CONTROL REGISTER x
        union {   
            struct tagLOGCONABITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }LOGCON_E; // LOGCONx: COMBINATORIAL PWM LOGIC CONTROL REGISTER x
        union {   
            struct tagLOGCONABITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }LOGCON_F; // LOGCONx: COMBINATORIAL PWM LOGIC CONTROL REGISTER x
        union {   
            struct tagPWMEVTABITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }PWMEVT_A; // PWMEVTx: PWM EVENT OUTPUT CONTROL REGISTER x
        union {   
            struct tagPWMEVTABITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }PWMEVT_B; // PWMEVTx: PWM EVENT OUTPUT CONTROL REGISTER x
        union {   
            struct tagPWMEVTABITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }PWMEVT_C; // PWMEVTx: PWM EVENT OUTPUT CONTROL REGISTER x
        union {   
            struct tagPWMEVTABITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }PWMEVT_D; // PWMEVTx: PWM EVENT OUTPUT CONTROL REGISTER x
        union {   
            struct tagPWMEVTABITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }PWMEVT_E; // PWMEVTx: PWM EVENT OUTPUT CONTROL REGISTER x
        union {   
            struct tagPWMEVTABITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }PWMEVT_F; // PWMEVTx: PWM EVENT OUTPUT CONTROL REGISTER x
       
    } __attribute__((packed)) P33C_PWM_MODULE_t; // PWM MODULE SPECIAL FUNCTION REGISTER SET

#endif
        
        
// GENERIC PWM GENERATOR SPECIAL FUNCTION REGISTER SET
    
#ifndef P33C_PWM_GENERATOR_s     

    typedef struct P33C_PWM_GENERATOR_s {
        union {   
            struct tagPG1CONLBITS bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }PGxCONL; // PGxCONH: PWM GENERATOR x CONTROL REGISTER HIGH
        union {
            struct tagPG1CONHBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxCONH; // PGxCONH: PWM GENERATOR x CONTROL REGISTER HIGH
        union {
            struct tagPG1STATBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxSTAT; // PGxSTAT: PWM GENERATOR x STATUS REGISTER
        union {
            struct tagPG1IOCONLBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxIOCONL; // PGxIOCONL: PWM GENERATOR x I/O CONTROL REGISTER LOW
        union {
            struct tagPG1IOCONHBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } PGxIOCONH; // PGxIOCONH: PWM GENERATOR x I/O CONTROL REGISTER HIGH
        union {
            struct tagPG1EVTLBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxEVTL;  // PGxEVTL: PWM GENERATOR x EVENT REGISTER LOW
        union {
            struct tagPG1EVTHBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxEVTH;  // PGxEVTH: PWM GENERATOR x EVENT REGISTER HIGH
        union {
            struct tagPG1FPCILBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxFPCIL; // PGxFPCIL: PWM GENERATOR x FAULT PCI REGISTER LOW
        union {
            struct tagPG1FPCIHBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxFPCIH; // PGxFPCIH: PWM GENERATOR x FAULT PCI REGISTER HIGH
        union {
            struct tagPG1CLPCILBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxCLPCIL; // PGxCLPCIL: PWM GENERATOR x CURRENT LIMIT PCI REGISTER LOW
        union {
            struct tagPG1CLPCIHBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxCLPCIH; // PGxCLPCIH: PWM GENERATOR x CURRENT LIMIT PCI REGISTER HIGH
        union {
            struct tagPG1FFPCILBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxFFPCIL; // PGxFFPCIL: PWM GENERATOR x FEED FORWARD PCI REGISTER LOW
        union {
            struct tagPG1FFPCIHBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxFFPCIH; // PGxFFPCIH: PWM GENERATOR x FEED FORWARD PCI REGISTER HIGH
        union {
            struct tagPG1SPCILBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxSPCIL; // PGxSPCIL: PWM GENERATOR x SOFTWARE PCI REGISTER LOW
        union {
            struct tagPG1SPCIHBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxSPCIH; // PGxSPCIH: PWM GENERATOR x SOFTWARE PCI REGISTER HIGH
        union {
            struct {
                uint16_t LEBL : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxLEBL;  // PGxLEBL: PWM GENERATOR x LEADING-EDGE BLANKING REGISTER LOW
        union {
            struct tagPG1LEBHBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxLEBH;  // PGxLEBH: PWM GENERATOR x LEADING-EDGE BLANKING REGISTER HIGH
        union {
            struct {
                uint16_t PHASE : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxPHASE; // PGxPHASE: PWM GENERATOR x PHASE REGISTER
        union {
            struct {
                uint16_t DC : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxDC;    // PGxDC: PWM GENERATOR x DUTY CYCLE REGISTER
        union {
            struct tagPG1DCABITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxDCA;   // PGxDCA: PWM GENERATOR x DUTY CYCLE ADJUSTMENT REGISTER
        union {
            struct {
                uint16_t PER : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxPER;   // PGxPER: PWM GENERATOR x PERIOD REGISTER
        union {
            struct {
                uint16_t TRIG : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxTRIGA; // PGxTRIGA: PWM GENERATOR x TRIGGER A REGISTER
        union {
            struct {
                uint16_t TRIG : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxTRIGB; // PGxTRIGB: PWM GENERATOR x TRIGGER B REGISTER
        union {
            struct {
                uint16_t TRIG : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxTRIGC; // PGxTRIGC: PWM GENERATOR x TRIGGER C REGISTER
        union {
            struct tagPG1DTLBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxDTL;   // PGxDTL: PWM GENERATOR x DEAD-TIME REGISTER LOW
        union {
            struct tagPG1DTHBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        }PGxDTH;   // PGxDTH: PWM GENERATOR x DEAD-TIME REGISTER HIGH
        union  { 
            struct {
                uint16_t CAP : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value : 16; // 16-bit wide register value
        }PGxCAP; // PGxCAP: PWM GENERATOR x CAPTURE REGISTER
    } __attribute__((packed)) P33C_PWM_GENERATOR_t; // PWM GENERATOR INSTANCE SPECIAL FUNCTION REGISTER SET
        
    // PWM generator instance Special Function Register set address offset
    #define P33C_PWMGEN_SFR_OFFSET  ((volatile uint16_t)&PG2CONL - (volatile uint16_t)&PG1CONL)

#endif

// Determine number of available PWM generators on the selected device
#if defined (PG8CONL)
  #define P33C_PG_COUNT   8
#elif defined (PG4CONL)
  #define P33C_PG_COUNT   4    
#endif
    
/* ********************************************************************************************* * 
 * API FUNCTION PROTOTYPES
 * ********************************************************************************************* */

/* ********************************************************************************************* * 
 * Basic PWM Module Configuration Function Call Prototypes
 * ********************************************************************************************* */
    
// PWM Module essential functions
extern volatile struct P33C_PWM_MODULE_s* p33c_PwmModule_GetHandle(void);

extern volatile struct P33C_PWM_MODULE_s p33c_PwmModule_ConfigRead(void);
extern volatile uint16_t p33c_PwmModule_ConfigWrite(volatile struct P33C_PWM_MODULE_s pwmConfig);

// PWM Module higher functions
extern volatile uint16_t p33c_PwmModule_Initialize(void); 
extern volatile uint16_t p33c_PwmModule_Dispose(void);

/* ********************************************************************************************* * 
 * Individual PWM Generator Configuration Function Call Prototypes
 * ********************************************************************************************* */

// PWM Generator essential functions
extern volatile struct P33C_PWM_GENERATOR_s p33c_PwmGenerator_ConfigRead(volatile uint16_t pgInstance);
extern volatile uint16_t p33c_PwmGenerator_ConfigWrite(volatile uint16_t pgInstance, 
                            volatile struct P33C_PWM_GENERATOR_s pgConfig);

extern volatile struct P33C_PWM_GENERATOR_s* p33c_PwmGenerator_GetHandle(volatile uint16_t pgInstance);
extern volatile uint16_t p33c_PwmGenerator_GetInstance(volatile struct P33C_PWM_GENERATOR_s* pg);
extern volatile uint16_t p33c_PwmGenerator_GetGroup(volatile struct P33C_PWM_GENERATOR_s* pg);


// PWM Generator higher functions
extern volatile uint16_t p33c_PwmGenerator_Initialize(volatile uint16_t pgInstance);
extern volatile uint16_t p33c_PwmGenerator_Dispose(volatile uint16_t pgInstance);

extern volatile uint16_t p33c_PwmGenerator_Enable(volatile struct P33C_PWM_GENERATOR_s* pg);
extern volatile uint16_t p33c_PwmGenerator_Disable(volatile struct P33C_PWM_GENERATOR_s* pg);
extern volatile uint16_t p33c_PwmGenerator_Resume(volatile struct P33C_PWM_GENERATOR_s* pg);
extern volatile uint16_t p33c_PwmGenerator_Suspend(volatile struct P33C_PWM_GENERATOR_s* pg);

// Standard Generator Functions API
extern volatile uint16_t p33c_PwmGenerator_SetPeriod(volatile struct P33C_PWM_GENERATOR_s* pg, 
                            volatile uint16_t period);
extern volatile uint16_t p33c_PwmGenerator_SetDutyCycle(volatile struct P33C_PWM_GENERATOR_s* pg, 
                            volatile uint16_t duty);
extern volatile uint16_t p33c_PwmGenerator_SetDeadTimes(volatile struct P33C_PWM_GENERATOR_s* pg, 
                            volatile uint16_t dead_time_rising, volatile uint16_t dead_time_falling);

volatile uint16_t p33c_PwmGenerator_SyncGenerators(
        volatile struct P33C_PWM_GENERATOR_s* pgHandleMother, 
        volatile uint16_t pgMotherTriggerOutput,
        volatile struct P33C_PWM_GENERATOR_s* pgHandleChild,
        volatile bool ChildImmediateUpdate
    );

/* ********************************************************************************************* * 
 * PWM GENERATOR CONFIGURATION TEMPLATES
 * ********************************************************************************************* */
extern volatile struct P33C_PWM_MODULE_s pwmConfigClear;
extern volatile struct P33C_PWM_MODULE_s pwmConfigDefault;

/* ********************************************************************************************* * 
 * PWM GENERATOR CONFIGURATION TEMPLATES
 * ********************************************************************************************* */
extern volatile struct P33C_PWM_GENERATOR_s pgConfigClear;


#endif	/* P33C_PWM_SFR_ABSTRACTION_H */
// END OF FILE

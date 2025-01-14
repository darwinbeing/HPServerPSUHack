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
 *  @file    dev_buck_pconfig.h
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
 * File:   dev_buck_ptemp_pwm.h
 * Author: M91406
 * Comments: PWM Peripheral Special Function Register Configuration Template
 * Revision history: 
 *   10/29/2020     1.0     initial release
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BUCK_CONVERTER_PERIPHERAL_CONFIGURATION_PWM_H
#define	BUCK_CONVERTER_PERIPHERAL_CONFIGURATION_PWM_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

/**
 * @ingroup lib-layer-buck-ptemplate-properties-defines
 * @{
 */
/* ********************************************************************************
 * PWM MODULE BASE REGISTER CONFIGURATION
 * *******************************************************************************/
/**
 * @def REG_PCLKCON
 * @brief PCLKCON: PWM CLOCK CONTROL REGISTER
 */
/* 
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
                          ||||||||||||||||  */
#define REG_PCLKCON     0b0000000000000011  

/**
 * @def REG_CMBTRIGL
 * @brief CMBTRIGL: COMBINATIONAL TRIGGER REGISTER LOW
 */
/* 

                           ________________ BIT 15: (unimplemented)
                          | _______________ BIT 14: (unimplemented)
                          || ______________ BIT 13: (unimplemented) 
                          ||| _____________ BIT 12: (unimplemented) 
                          |||| ____________ BIT 11: (unimplemented) 
                          ||||| ___________ BIT 10: (unimplemented)
                          |||||| __________ BIT  9: (unimplemented)
                          ||||||| _________ BIT  8: (unimplemented)
                          |||||||| ________ BIT  7: CTA8EN: Enable Trigger Output from PWM Generator #8 as Source for Combinational Trigger A bit
                          ||||||||| _______ BIT  6: CTA7EN: Enable Trigger Output from PWM Generator #7 as Source for Combinational Trigger A bit
                          |||||||||| ______ BIT  5: CTA6EN: Enable Trigger Output from PWM Generator #6 as Source for Combinational Trigger A bit
                          ||||||||||| _____ BIT  4: CTA5EN: Enable Trigger Output from PWM Generator #5 as Source for Combinational Trigger A bit
                          |||||||||||| ____ BIT  3: CTA4EN: Enable Trigger Output from PWM Generator #4 as Source for Combinational Trigger A bit
                          ||||||||||||| ___ BIT  2: CTA3EN: Enable Trigger Output from PWM Generator #3 as Source for Combinational Trigger A bit
                          |||||||||||||| __ BIT  1: CTA2EN: Enable Trigger Output from PWM Generator #2 as Source for Combinational Trigger A bit
                          ||||||||||||||| _ BIT  0: CTA1EN: Enable Trigger Output from PWM Generator #1 as Source for Combinational Trigger A bit
                          ||||||||||||||||  */
#define REG_CMBTRIGL    0b0000000000000000      

/**
 * @def REG_CMBTRIGH
 * @brief CMBTRIGH: COMBINATIONAL TRIGGER REGISTER HIGH
 */
/* CMBTRIGH: COMBINATIONAL TRIGGER REGISTER HIGH

                           ________________ BIT 15: (unimplemented)
                          | _______________ BIT 14: (unimplemented)
                          || ______________ BIT 13: (unimplemented) 
                          ||| _____________ BIT 12: (unimplemented) 
                          |||| ____________ BIT 11: (unimplemented) 
                          ||||| ___________ BIT 10: (unimplemented)
                          |||||| __________ BIT  9: (unimplemented)
                          ||||||| _________ BIT  8: (unimplemented)
                          |||||||| ________ BIT  7: CTB8EN: Enable Trigger Output from PWM Generator #8 as Source for Combinational Trigger B bit
                          ||||||||| _______ BIT  6: CTB7EN: Enable Trigger Output from PWM Generator #7 as Source for Combinational Trigger B bit
                          |||||||||| ______ BIT  5: CTB6EN: Enable Trigger Output from PWM Generator #6 as Source for Combinational Trigger B bit
                          ||||||||||| _____ BIT  4: CTB5EN: Enable Trigger Output from PWM Generator #5 as Source for Combinational Trigger B bit
                          |||||||||||| ____ BIT  3: CTB4EN: Enable Trigger Output from PWM Generator #4 as Source for Combinational Trigger B bit
                          ||||||||||||| ___ BIT  2: CTB3EN: Enable Trigger Output from PWM Generator #3 as Source for Combinational Trigger B bit
                          |||||||||||||| __ BIT  1: CTB2EN: Enable Trigger Output from PWM Generator #2 as Source for Combinational Trigger B bit
                          ||||||||||||||| _ BIT  0: CTB1EN: Enable Trigger Output from PWM Generator #1 as Source for Combinational Trigger B bit
                          ||||||||||||||||  */
#define REG_CMBTRIGH    0b0000000000000000  

/**
 * @def REG_LOGCONA
 * @brief LOGCONA: COMBINATORIAL PWM LOGIC CONTROL REGISTER A
 */
/* 

                           ________________ BIT 15: PWMS1y[3:0]: Combinatorial PWM Logic Source #1 Selection bits
                          | _______________ BIT 14: 
                          || ______________ BIT 13:  
                          ||| _____________ BIT 12:  
                          |||| ____________ BIT 11: PWMS2y[3:0]: Combinatorial PWM Logic Source #1 Selection bits
                          ||||| ___________ BIT 10: 
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: S1yPOL: Combinatorial PWM Logic Source #1 Polarity bit
                          ||||||||| _______ BIT  6: S2yPOL: Combinatorial PWM Logic Source #2 Polarity bit
                          |||||||||| ______ BIT  5: PWMLFy[1:0]: Combinatorial PWM Logic Function Selection bits
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: (unimplemented)
                          ||||||||||||| ___ BIT  2: PWMLFyD[2:0]: Combinatorial PWM Logic Destination Selection bits
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_LOGCONA     0b0000000000000000  

/**
 * @def REG_LOGCONB
 * @brief LOGCONB: COMBINATORIAL PWM LOGIC CONTROL REGISTER B
 */
/* LOGCONB: COMBINATORIAL PWM LOGIC CONTROL REGISTER B

                           ________________ BIT 15: PWMS1y[3:0]: Combinatorial PWM Logic Source #1 Selection bits
                          | _______________ BIT 14: 
                          || ______________ BIT 13:  
                          ||| _____________ BIT 12:  
                          |||| ____________ BIT 11: PWMS2y[3:0]: Combinatorial PWM Logic Source #1 Selection bits
                          ||||| ___________ BIT 10: 
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: S1yPOL: Combinatorial PWM Logic Source #1 Polarity bit
                          ||||||||| _______ BIT  6: S2yPOL: Combinatorial PWM Logic Source #2 Polarity bit
                          |||||||||| ______ BIT  5: PWMLFy[1:0]: Combinatorial PWM Logic Function Selection bits
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: (unimplemented)
                          ||||||||||||| ___ BIT  2: PWMLFyD[2:0]: Combinatorial PWM Logic Destination Selection bits
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_LOGCONB     0b0000000000000000

/**
 * @def REG_LOGCONC
 * @brief LOGCONC: COMBINATORIAL PWM LOGIC CONTROL REGISTER C
 */
/* 

                           ________________ BIT 15: PWMS1y[3:0]: Combinatorial PWM Logic Source #1 Selection bits
                          | _______________ BIT 14: 
                          || ______________ BIT 13:  
                          ||| _____________ BIT 12:  
                          |||| ____________ BIT 11: PWMS2y[3:0]: Combinatorial PWM Logic Source #1 Selection bits
                          ||||| ___________ BIT 10: 
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: S1yPOL: Combinatorial PWM Logic Source #1 Polarity bit
                          ||||||||| _______ BIT  6: S2yPOL: Combinatorial PWM Logic Source #2 Polarity bit
                          |||||||||| ______ BIT  5: PWMLFy[1:0]: Combinatorial PWM Logic Function Selection bits
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: (unimplemented)
                          ||||||||||||| ___ BIT  2: PWMLFyD[2:0]: Combinatorial PWM Logic Destination Selection bits
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_LOGCONC     0b0000000000000000

/**
 * @def REG_LOGCOND
 * @brief LOGCOND: COMBINATORIAL PWM LOGIC CONTROL REGISTER D
 */
/* 

                           ________________ BIT 15: PWMS1y[3:0]: Combinatorial PWM Logic Source #1 Selection bits
                          | _______________ BIT 14: 
                          || ______________ BIT 13:  
                          ||| _____________ BIT 12:  
                          |||| ____________ BIT 11: PWMS2y[3:0]: Combinatorial PWM Logic Source #1 Selection bits
                          ||||| ___________ BIT 10: 
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: S1yPOL: Combinatorial PWM Logic Source #1 Polarity bit
                          ||||||||| _______ BIT  6: S2yPOL: Combinatorial PWM Logic Source #2 Polarity bit
                          |||||||||| ______ BIT  5: PWMLFy[1:0]: Combinatorial PWM Logic Function Selection bits
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: (unimplemented)
                          ||||||||||||| ___ BIT  2: PWMLFyD[2:0]: Combinatorial PWM Logic Destination Selection bits
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_LOGCOND     0b0000000000000000

/**
 * @def REG_LOGCONE
 * @brief LOGCONE: COMBINATORIAL PWM LOGIC CONTROL REGISTER E
 */
/* 

                           ________________ BIT 15: PWMS1y[3:0]: Combinatorial PWM Logic Source #1 Selection bits
                          | _______________ BIT 14: 
                          || ______________ BIT 13:  
                          ||| _____________ BIT 12:  
                          |||| ____________ BIT 11: PWMS2y[3:0]: Combinatorial PWM Logic Source #1 Selection bits
                          ||||| ___________ BIT 10: 
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: S1yPOL: Combinatorial PWM Logic Source #1 Polarity bit
                          ||||||||| _______ BIT  6: S2yPOL: Combinatorial PWM Logic Source #2 Polarity bit
                          |||||||||| ______ BIT  5: PWMLFy[1:0]: Combinatorial PWM Logic Function Selection bits
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: (unimplemented)
                          ||||||||||||| ___ BIT  2: PWMLFyD[2:0]: Combinatorial PWM Logic Destination Selection bits
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_LOGCONE     0b0000000000000000

/**
 * @def REG_LOGCONF
 * @brief LOGCONF: COMBINATORIAL PWM LOGIC CONTROL REGISTER F
 */
/* 

                           ________________ BIT 15: PWMS1y[3:0]: Combinatorial PWM Logic Source #1 Selection bits
                          | _______________ BIT 14: 
                          || ______________ BIT 13:  
                          ||| _____________ BIT 12:  
                          |||| ____________ BIT 11: PWMS2y[3:0]: Combinatorial PWM Logic Source #1 Selection bits
                          ||||| ___________ BIT 10: 
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: S1yPOL: Combinatorial PWM Logic Source #1 Polarity bit
                          ||||||||| _______ BIT  6: S2yPOL: Combinatorial PWM Logic Source #2 Polarity bit
                          |||||||||| ______ BIT  5: PWMLFy[1:0]: Combinatorial PWM Logic Function Selection bits
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: (unimplemented)
                          ||||||||||||| ___ BIT  2: PWMLFyD[2:0]: Combinatorial PWM Logic Destination Selection bits
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_LOGCONF     0b0000000000000000

/**
 * @def REG_PWMEVTA
 * @brief PWMEVTA: PWM EVENT OUTPUT CONTROL REGISTER A
 */
/* 

                           ________________ BIT 15: EVTyOEN: PWM Event Output Enable bit
                          | _______________ BIT 14: EVTyPOL: PWM Event Output Polarity bit
                          || ______________ BIT 13: EVTySTRD: PWM Event Output Stretch Disable bit
                          ||| _____________ BIT 12: EVTySYNC: PWM Event Output Sync bit
                          |||| ____________ BIT 11: (unimplemented)
                          ||||| ___________ BIT 10: (unimplemented)
                          |||||| __________ BIT  9: (unimplemented)
                          ||||||| _________ BIT  8: (unimplemented)
                          |||||||| ________ BIT  7: EVTySEL[3:0]: PWM Event Selection bits
                          ||||||||| _______ BIT  6: 
                          |||||||||| ______ BIT  5: 
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: (unimplemented)
                          ||||||||||||| ___ BIT  2: EVTyPGS[2:0]: PWM Event Source Selection bits
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PWMEVTA     0b0000000000000000

/**
 * @def REG_PWMEVTB
 * @brief PWMEVTB: PWM EVENT OUTPUT CONTROL REGISTER B
 */
/* 

                           ________________ BIT 15: EVTyOEN: PWM Event Output Enable bit
                          | _______________ BIT 14: EVTyPOL: PWM Event Output Polarity bit
                          || ______________ BIT 13: EVTySTRD: PWM Event Output Stretch Disable bit
                          ||| _____________ BIT 12: EVTySYNC: PWM Event Output Sync bit
                          |||| ____________ BIT 11: (unimplemented)
                          ||||| ___________ BIT 10: (unimplemented)
                          |||||| __________ BIT  9: (unimplemented)
                          ||||||| _________ BIT  8: (unimplemented)
                          |||||||| ________ BIT  7: EVTySEL[3:0]: PWM Event Selection bits
                          ||||||||| _______ BIT  6: 
                          |||||||||| ______ BIT  5: 
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: (unimplemented)
                          ||||||||||||| ___ BIT  2: EVTyPGS[2:0]: PWM Event Source Selection bits
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PWMEVTB     0b0000000000000000

/**
 * @def REG_PWMEVTC
 * @brief PWMEVTC: PWM EVENT OUTPUT CONTROL REGISTER C
 */
/* 

                           ________________ BIT 15: EVTyOEN: PWM Event Output Enable bit
                          | _______________ BIT 14: EVTyPOL: PWM Event Output Polarity bit
                          || ______________ BIT 13: EVTySTRD: PWM Event Output Stretch Disable bit
                          ||| _____________ BIT 12: EVTySYNC: PWM Event Output Sync bit
                          |||| ____________ BIT 11: (unimplemented)
                          ||||| ___________ BIT 10: (unimplemented)
                          |||||| __________ BIT  9: (unimplemented)
                          ||||||| _________ BIT  8: (unimplemented)
                          |||||||| ________ BIT  7: EVTySEL[3:0]: PWM Event Selection bits
                          ||||||||| _______ BIT  6: 
                          |||||||||| ______ BIT  5: 
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: (unimplemented)
                          ||||||||||||| ___ BIT  2: EVTyPGS[2:0]: PWM Event Source Selection bits
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PWMEVTC     0b0000000000000000

/**
 * @def REG_PWMEVTD
 * @brief PWMEVTD: PWM EVENT OUTPUT CONTROL REGISTER D
 */
/*

                           ________________ BIT 15: EVTyOEN: PWM Event Output Enable bit
                          | _______________ BIT 14: EVTyPOL: PWM Event Output Polarity bit
                          || ______________ BIT 13: EVTySTRD: PWM Event Output Stretch Disable bit
                          ||| _____________ BIT 12: EVTySYNC: PWM Event Output Sync bit
                          |||| ____________ BIT 11: (unimplemented)
                          ||||| ___________ BIT 10: (unimplemented)
                          |||||| __________ BIT  9: (unimplemented)
                          ||||||| _________ BIT  8: (unimplemented)
                          |||||||| ________ BIT  7: EVTySEL[3:0]: PWM Event Selection bits
                          ||||||||| _______ BIT  6: 
                          |||||||||| ______ BIT  5: 
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: (unimplemented)
                          ||||||||||||| ___ BIT  2: EVTyPGS[2:0]: PWM Event Source Selection bits
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PWMEVTD     0b0000000000000000

/**
 * @def REG_PWMEVTE
 * @brief PWMEVTE: PWM EVENT OUTPUT CONTROL REGISTER E
 */
/*

                           ________________ BIT 15: EVTyOEN: PWM Event Output Enable bit
                          | _______________ BIT 14: EVTyPOL: PWM Event Output Polarity bit
                          || ______________ BIT 13: EVTySTRD: PWM Event Output Stretch Disable bit
                          ||| _____________ BIT 12: EVTySYNC: PWM Event Output Sync bit
                          |||| ____________ BIT 11: (unimplemented)
                          ||||| ___________ BIT 10: (unimplemented)
                          |||||| __________ BIT  9: (unimplemented)
                          ||||||| _________ BIT  8: (unimplemented)
                          |||||||| ________ BIT  7: EVTySEL[3:0]: PWM Event Selection bits
                          ||||||||| _______ BIT  6: 
                          |||||||||| ______ BIT  5: 
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: (unimplemented)
                          ||||||||||||| ___ BIT  2: EVTyPGS[2:0]: PWM Event Source Selection bits
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PWMEVTE     0b0000000000000000

/**
 * @def REG_PWMEVTF
 * @brief PWMEVTF: PWM EVENT OUTPUT CONTROL REGISTER F
 */
/*

                           ________________ BIT 15: EVTyOEN: PWM Event Output Enable bit
                          | _______________ BIT 14: EVTyPOL: PWM Event Output Polarity bit
                          || ______________ BIT 13: EVTySTRD: PWM Event Output Stretch Disable bit
                          ||| _____________ BIT 12: EVTySYNC: PWM Event Output Sync bit
                          |||| ____________ BIT 11: (unimplemented)
                          ||||| ___________ BIT 10: (unimplemented)
                          |||||| __________ BIT  9: (unimplemented)
                          ||||||| _________ BIT  8: (unimplemented)
                          |||||||| ________ BIT  7: EVTySEL[3:0]: PWM Event Selection bits
                          ||||||||| _______ BIT  6: 
                          |||||||||| ______ BIT  5: 
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: (unimplemented)
                          ||||||||||||| ___ BIT  2: EVTyPGS[2:0]: PWM Event Source Selection bits
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PWMEVTF     0b0000000000000000

/* ********************************************************************************
 * PWM GENERATOR CONFIGURATION
 * *******************************************************************************/
/**
 * @def P33C_PGxCONL_PWM_ON
 * @brief control bit in PGxCONL enabling/disabling the PWM generator
 */    
#define P33C_PGxCONL_PWM_ON     0x8000  
/**
 * @def P33C_PGxCONL_HRES_EN
 * @brief control bit in PGxCONL enabling/disabling High Resolution Mode
 */  
#define P33C_PGxCONL_HRES_EN    0x0080  
/**
 * @def P33C_PGxIOCONL_OVREN_SYNC
 * @brief control bits in PGxIOCONL enabling/disabling the PWM output override in synchronous mode
 */  
#define P33C_PGxIOCONL_OVREN_SYNC   0x3000  
/**
 * @def P33C_PGxIOCONL_OVREN_ASYNC
 * @brief control bits in PGxIOCONL enabling/disabling the PWM output override in asynchronous mode
 */  
#define P33C_PGxIOCONL_OVREN_ASYNC      0x2000  
#define P33C_PGxIOCONL_OVREN_ASYNC_SWAP 0x1000  
/**
 * @def P33C_PGxIOCONH_PEN_SYNC
 * @brief control bits in PGxIOCONH enabling/disabling the PWM outputs in synchronous mode
 */  
#define P33C_PGxIOCONH_PEN_SYNC     0x000C  
/**
 * @def P33C_PGxIOCONH_PEN_SYNC
 * @brief control bits in PGxIOCONH enabling/disabling the PWM outputs in asynchronous mode
 */  
#define P33C_PGxIOCONH_PEN_ASYNC        0x0008  
#define P33C_PGxIOCONH_PEN_ASYNC_SWAP   0x0004
/**
 * @def P33C_PGxSTAT_UPDREQ
 * @brief Control bit in PGxSTAT setting the Update Request bit
 */  
#define P33C_PGxSTAT_UPDREQ     0x0008  
/**
 * @def P33C_PGxCONH_MPERSEL
 * @brief Control bit in PGxCONH seting the PERIOD register source
 */  
#define P33C_PGxCONH_MPERSEL    0x4000  
/**
 * @def P33C_PGxCONH_UPDMOD_MSTR
 * @brief Master Immediate Update
 */      
#define P33C_PGxCONH_UPDMOD_MSTR 0b001  
/**
 * @def P33C_PGxCONH_UPDMOD_SLV
 * @brief Slaved immediate Update
 */  
#define P33C_PGxCONH_UPDMOD_SLV  0b011  

// ==============================================================================================
// BUCK converter Peripheral Configuration for Voltage and Average Current Mode Control
// ==============================================================================================
/**
 * @def REG_PGxCONL
 * @brief PGxCONL: PWM GENERATOR x CONTROL REGISTER LOW
 */  
/* 

                           ________________ BIT 15: ON: Enable: PWM Generator Enable
                          | _______________ BIT 14: (reserved) 
                          || ______________ BIT 13: (unimplemented) 
                          ||| _____________ BIT 12: (unimplemented) 
                          |||| ____________ BIT 11: (unimplemented) 
                          ||||| ___________ BIT 10: TRGCNT[2:0]: Trigger Count Selection
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: HREN: PWM Generator x High-Resolution Enable
                          ||||||||| _______ BIT  6: (unimplemented)
                          |||||||||| ______ BIT  5: (unimplemented)
                          ||||||||||| _____ BIT  4: CLKSEL[1:0]: Clock Selection
                          |||||||||||| ____ BIT  3: 
                          ||||||||||||| ___ BIT  2: MODSEL[2:0]: Mode Selection
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PGxCONL     0b0000000010001000

/**
 * @def REG_PGxCONH
 * @brief PGxCONH: PWM GENERATOR x CONTROL REGISTER LOW
 */ 
/*

                           ________________ BIT 15: MDCSEL: Master Duty Cycle Register Selection: 0 = PWM Generator uses PGxDC register
                          | _______________ BIT 14: MPERSEL: Master Period Register Selection: 1 = PWM Generator uses MPER register
                          || ______________ BIT 13: MPHSEL: Master Phase Register Selection: 0 = PWM Generator uses PGxPHASE register
                          ||| _____________ BIT 12: (unimplemented) 
                          |||| ____________ BIT 11: MSTEN: Master Update Enable: 0 = PWM Generator does not broadcast the UPDREQ status bit state or EOC signal
                          ||||| ___________ BIT 10: UPDMOD[2:0]: PWM Buffer Update Mode Selection: 001 = Immediate update
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: (reserved)
                          ||||||||| _______ BIT  6: TRGMOD: PWM Generator Trigger Mode Selection: PWM Generator operates in Retriggerable mode
                          |||||||||| ______ BIT  5: (unimplemented)
                          ||||||||||| _____ BIT  4: (unimplemented)
                          |||||||||||| ____ BIT  3: SOCS[3:0]: Start-of-Cycle Selection: Local EOC ? PWM Generator is self-triggered
                          ||||||||||||| ___ BIT  2: 
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PGxCONH     0b0000000100000000

/**
 * @def REG_PGxIOCONL
 * @brief PGxIOCONL: PWM GENERATOR x I/O CONTROL REGISTER LOW
 */
/* PGxIOCONL: PWM GENERATOR x I/O CONTROL REGISTER LOW

                           ________________ BIT 15: CLMOD: Current-Limit Mode Selection
                          | _______________ BIT 14: SWAP: Swap PWM Signals to PWMxH and PWMxL Device Pins
                          || ______________ BIT 13: OVRENH: User Override Enable for PWMxH Pin
                          ||| _____________ BIT 12: OVRENL: User Override Enable for PWMxL Pin
                          |||| ____________ BIT 11: OVRDAT[1:0]: Data for PWMxH/PWMxL Pins if Override is Enabled
                          ||||| ___________ BIT 10: 
                          |||||| __________ BIT  9: OSYNC[1:0]: User Output Override Synchronization Control
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: FLTDAT[1:0]: Data for PWMxH/PWMxL Pins if Fault Event is Active
                          ||||||||| _______ BIT  6: 
                          |||||||||| ______ BIT  5: CLDAT[1:0]: Data for PWMxH/PWMxL Pins if Current-Limit Event is Active
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: FFDAT[1:0]: Data for PWMxH/PWMxL Pins if Feed-Forward Event is Active
                          ||||||||||||| ___ BIT  2: 
                          |||||||||||||| __ BIT  1: DBDAT[1:0]: Data for PWMxH/PWMxL Pins if Debug Mode is Active
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PGxIOCONL   0b0011000000000000

/**
 * @def REG_PGxIOCONH
 * @brief PGxIOCONH: PWM GENERATOR x I/O CONTROL REGISTER LOW
 */   
/* 

                           ________________ BIT 15: (unimplemented)
                          | _______________ BIT 14: CAPSRC[2:0]: Time Base Capture Source Selection
                          || ______________ BIT 13: 
                          ||| _____________ BIT 12: 
                          |||| ____________ BIT 11: (unimplemented)
                          ||||| ___________ BIT 10: (unimplemented)
                          |||||| __________ BIT  9: (unimplemented)
                          ||||||| _________ BIT  8: DTCMPSEL: Dead-Time Compensation Select
                          |||||||| ________ BIT  7: (unimplemented)
                          ||||||||| _______ BIT  6: (unimplemented)
                          |||||||||| ______ BIT  5: PMOD[1:0]: PWM Generator Output Mode Selection
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: PENH: PWMxH Output Port Enable
                          ||||||||||||| ___ BIT  2: PENL: PWMxL Output Port Enable
                          |||||||||||||| __ BIT  1: POLH: PWMxH Output Polarity
                          ||||||||||||||| _ BIT  0: POLL: PWMxL Output Polarity
                          ||||||||||||||||  */
#define REG_PGxIOCONH   0b0000000000000000

/**
 * @def REG_PGxEVTL
 * @brief PGxEVTL: PWM GENERATOR x EVENT REGISTER LOW
 */
/* 

                           ________________ BIT 15: ADTR1PS[4:0]: ADC Trigger 1 Postscaler Selection
                          | _______________ BIT 14: 
                          || ______________ BIT 13: 
                          ||| _____________ BIT 12: 
                          |||| ____________ BIT 11: 
                          ||||| ___________ BIT 10: ADTR1EN3: ADC Trigger 1 Source is PGxTRIGC Compare Event Enable
                          |||||| __________ BIT  9: ADTR1EN2: ADC Trigger 1 Source is PGxTRIGB Compare Event Enable
                          ||||||| _________ BIT  8: ADTR1EN1: ADC Trigger 1 Source is PGxTRIGA Compare Event Enable
                          |||||||| ________ BIT  7: (unimplemented)
                          ||||||||| _______ BIT  6: (unimplemented)
                          |||||||||| ______ BIT  5: (unimplemented)
                          ||||||||||| _____ BIT  4: UPDTRG[1:0]: Update Trigger Selection
                          |||||||||||| ____ BIT  3: 
                          ||||||||||||| ___ BIT  2: PGTRGSEL[2:0]: PWM Generator Trigger Output Selection
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PGxEVTL     0b0000000100011001

/**
 * @def REG_PGxEVTH
 * @brief PGxEVTH: PWM GENERATOR x EVENT REGISTER LOW
 */
/*

                           ________________ BIT 15: FLTIEN: PCI Fault Interrupt Enable
                          | _______________ BIT 14: CLIEN: PCI Current-Limit Interrupt Enable
                          || ______________ BIT 13: FFIEN: PCI Feed-Forward Interrupt Enable
                          ||| _____________ BIT 12: SIEN: PCI Sync Interrupt Enable
                          |||| ____________ BIT 11: (unimplemented)
                          ||||| ___________ BIT 10: (unimplemented)
                          |||||| __________ BIT  9: IEVTSEL[1:0]: Interrupt Event Selection = Interrupts CPU at TRIGA compare event
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: ADTR2EN3: ADC Trigger 2 Source is PGxTRIGC Compare Event Enable
                          ||||||||| _______ BIT  6: ADTR2EN2: ADC Trigger 2 Source is PGxTRIGB Compare Event Enable
                          |||||||||| ______ BIT  5: ADTR2EN1: ADC Trigger 2 Source is PGxTRIGA Compare Event Enable
                          ||||||||||| _____ BIT  4: ADTR1OFS[4:0]: ADC Trigger 1 Offset Selection
                          |||||||||||| ____ BIT  3: 
                          ||||||||||||| ___ BIT  2: 
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PGxEVTH     0b0000000101000000

/**
 * @def REG_PGxCLPCIL
 * @brief PGxCLPCIL: PWM GENERATOR CURRENT LIMIT PCI REGISTER LOW
 */
/* 

                           ________________ BIT 15: TSYNCDIS: Termination Synchronization Disable
                          | _______________ BIT 14: TERM[2:0]: Termination Event Selection
                          || ______________ BIT 13: 
                          ||| _____________ BIT 12: 
                          |||| ____________ BIT 11: AQPS: Acceptance Qualifier Polarity Selection
                          ||||| ___________ BIT 10: AQSS[2:0]: Acceptance Qualifier Source Selection
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: SWTERM: PCI Software Termination
                          ||||||||| _______ BIT  6: PSYNC: PCI Synchronization Control
                          |||||||||| ______ BIT  5: PPS: PCI Polarity Selection
                          ||||||||||| _____ BIT  4: PSS[4:0]: PCI Source Selection
                          |||||||||||| ____ BIT  3: 
                          ||||||||||||| ___ BIT  2: 
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0:                
                          ||||||||||||||||  */
//#define REG_PGxCLPCIL   0b0001101000011011    // Peak Current Mode Configuration
#define REG_PGxCLPCIL   0b0000000000000000    

/**
 * @def REG_PGxCLPCIH
 * @brief PGxCLPCIH: PWM GENERATOR CURRENT LIMIT PCI REGISTER HIGH
 */
/* 

                           ________________ BIT 15: BPEN: PCI Bypass Enable
                          | _______________ BIT 14: BPSEL[2:0]: PCI Bypass Source Selection
                          || ______________ BIT 13: 
                          ||| _____________ BIT 12: 
                          |||| ____________ BIT 11: (unimplemented)
                          ||||| ___________ BIT 10: ACP[2:0]: PCI Acceptance Criteria Selection
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: SWPCI: Software PCI Control
                          ||||||||| _______ BIT  6: SWPCIM[1:0]: Software PCI Control Mode
                          |||||||||| ______ BIT  5: 
                          ||||||||||| _____ BIT  4: LATMOD: PCI SR Latch Mode
                          |||||||||||| ____ BIT  3: TQPS: Termination Qualifier Polarity Selection
                          ||||||||||||| ___ BIT  2: TQSS[2:0]: Termination Qualifier Source Selection
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
//#define REG_PGxCLPCIH   0b0000011000000000  // Peak Current Mode Configuration
#define REG_PGxCLPCIH   0b0000000000000000  

/**
 * @def REG_PGxFPCIL
 * @brief PGxFPCIL: PWM GENERATOR FAULT PCI REGISTER LOW
 */
/* 

                           ________________ BIT 15: TSYNCDIS: Termination Synchronization Disable
                          | _______________ BIT 14: TERM[2:0]: Termination Event Selection
                          || ______________ BIT 13: 
                          ||| _____________ BIT 12: 
                          |||| ____________ BIT 11: AQPS: Acceptance Qualifier Polarity Selection
                          ||||| ___________ BIT 10: AQSS[2:0]: Acceptance Qualifier Source Selection
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: SWTERM: PCI Software Termination
                          ||||||||| _______ BIT  6: PSYNC: PCI Synchronization Control
                          |||||||||| ______ BIT  5: PPS: PCI Polarity Selection
                          ||||||||||| _____ BIT  4: PSS[4:0]: PCI Source Selection
                          |||||||||||| ____ BIT  3: 
                          ||||||||||||| ___ BIT  2: 
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PGxFPCIL    0b0000000000000000

/**
 * @def REG_PGxFPCIH
 * @brief PGxFPCIH: PWM GENERATOR FAULT PCI REGISTER HIGH
 */
/* 

                           ________________ BIT 15: BPEN: PCI Bypass Enable
                          | _______________ BIT 14: BPSEL[2:0]: PCI Bypass Source Selection
                          || ______________ BIT 13: 
                          ||| _____________ BIT 12: 
                          |||| ____________ BIT 11: (unimplemented)
                          ||||| ___________ BIT 10: ACP[2:0]: PCI Acceptance Criteria Selection
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: SWPCI: Software PCI Control
                          ||||||||| _______ BIT  6: SWPCIM[1:0]: Software PCI Control Mode
                          |||||||||| ______ BIT  5: 
                          ||||||||||| _____ BIT  4: LATMOD: PCI SR Latch Mode
                          |||||||||||| ____ BIT  3: TQPS: Termination Qualifier Polarity Selection
                          ||||||||||||| ___ BIT  2: TQSS[2:0]: Termination Qualifier Source Selection
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0:               
                          ||||||||||||||||  */
#define REG_PGxFPCIH    0b0000000000000000

/**
 * @def REG_PGxFFPCIL
 * @brief PGxFFPCIL: PWM GENERATOR FEED FORWARD PCI REGISTER LOW
 */
/* PGxFFPCIL: PWM GENERATOR FEED FORWARD PCI REGISTER LOW

                           ________________ BIT 15: TSYNCDIS: Termination Synchronization Disable
                          | _______________ BIT 14: TERM[2:0]: Termination Event Selection
                          || ______________ BIT 13: 
                          ||| _____________ BIT 12: 
                          |||| ____________ BIT 11: AQPS: Acceptance Qualifier Polarity Selection
                          ||||| ___________ BIT 10: AQSS[2:0]: Acceptance Qualifier Source Selection
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: SWTERM: PCI Software Termination
                          ||||||||| _______ BIT  6: PSYNC: PCI Synchronization Control
                          |||||||||| ______ BIT  5: PPS: PCI Polarity Selection
                          ||||||||||| _____ BIT  4: PSS[4:0]: PCI Source Selection
                          |||||||||||| ____ BIT  3: 
                          ||||||||||||| ___ BIT  2: 
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PGxFFPCIL   0b0000000000000000
 
/**
 * @def REG_PGxFFPCIH
 * @brief PGxFFPCIH: PWM GENERATOR FEED FORWARD PCI REGISTER LOW
 */
/* 

                           ________________ BIT 15: BPEN: PCI Bypass Enable
                          | _______________ BIT 14: BPSEL[2:0]: PCI Bypass Source Selection
                          || ______________ BIT 13: 
                          ||| _____________ BIT 12: 
                          |||| ____________ BIT 11: (unimplemented)
                          ||||| ___________ BIT 10: ACP[2:0]: PCI Acceptance Criteria Selection
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: SWPCI: Software PCI Control
                          ||||||||| _______ BIT  6: SWPCIM[1:0]: Software PCI Control Mode
                          |||||||||| ______ BIT  5: 
                          ||||||||||| _____ BIT  4: LATMOD: PCI SR Latch Mode
                          |||||||||||| ____ BIT  3: TQPS: Termination Qualifier Polarity Selection
                          ||||||||||||| ___ BIT  2: TQSS[2:0]: Termination Qualifier Source Selection
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0:               
                          ||||||||||||||||  */
#define REG_PGxFFPCIH   0b0000000000000000

/**
 * @def REG_PGxSPCIL
 * @brief PGxSPCIL: PWM GENERATOR SOFTWARE PCI REGISTER LOW
 */
/* PGxSPCIL: PWM GENERATOR SOFTWARE PCI REGISTER LOW

                           ________________ BIT 15: TSYNCDIS: Termination Synchronization Disable
                          | _______________ BIT 14: TERM[2:0]: Termination Event Selection
                          || ______________ BIT 13: 
                          ||| _____________ BIT 12: 
                          |||| ____________ BIT 11: AQPS: Acceptance Qualifier Polarity Selection
                          ||||| ___________ BIT 10: AQSS[2:0]: Acceptance Qualifier Source Selection
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: SWTERM: PCI Software Termination
                          ||||||||| _______ BIT  6: PSYNC: PCI Synchronization Control
                          |||||||||| ______ BIT  5: PPS: PCI Polarity Selection
                          ||||||||||| _____ BIT  4: PSS[4:0]: PCI Source Selection
                          |||||||||||| ____ BIT  3: 
                          ||||||||||||| ___ BIT  2: 
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PGxSPCIL    0b0000000000000000
  
/**
 * @def REG_PGxSPCIH
 * @brief PGxSPCIH: PWM GENERATOR SOFTWARE PCI REGISTER LOW
 */
/* 

                           ________________ BIT 15: BPEN: PCI Bypass Enable
                          | _______________ BIT 14: BPSEL[2:0]: PCI Bypass Source Selection
                          || ______________ BIT 13: 
                          ||| _____________ BIT 12: 
                          |||| ____________ BIT 11: (unimplemented)
                          ||||| ___________ BIT 10: ACP[2:0]: PCI Acceptance Criteria Selection
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: SWPCI: Software PCI Control
                          ||||||||| _______ BIT  6: SWPCIM[1:0]: Software PCI Control Mode
                          |||||||||| ______ BIT  5: 
                          ||||||||||| _____ BIT  4: LATMOD: PCI SR Latch Mode
                          |||||||||||| ____ BIT  3: TQPS: Termination Qualifier Polarity Selection
                          ||||||||||||| ___ BIT  2: TQSS[2:0]: Termination Qualifier Source Selection
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0:               
                          ||||||||||||||||  */
#define REG_PGxSPCIH    0b0000000000000000

/**
 * @def REG_PGxLEBH
 * @brief PGxLEBH: PWM GENERATOR x LEADING-EDGE BLANKING REGISTER HIGH
 */
/* 

                           ________________ BIT 15: (unimplemented)
                          | _______________ BIT 14: (unimplemented)
                          || ______________ BIT 13: (unimplemented)
                          ||| _____________ BIT 12: (unimplemented)
                          |||| ____________ BIT 11: (unimplemented)
                          ||||| ___________ BIT 10: PWMPCI[2:0]: PWM Source for PCI Selection
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: (unimplemented)
                          ||||||||| _______ BIT  6: (unimplemented)
                          |||||||||| ______ BIT  5: (unimplemented)
                          ||||||||||| _____ BIT  4: (unimplemented)
                          |||||||||||| ____ BIT  3: PHR: PWMxH Rising Edge Trigger Enable
                          ||||||||||||| ___ BIT  2: PHF: PWMxH Falling Edge Trigger Enable
                          |||||||||||||| __ BIT  1: PLR: PWMxL Rising Edge Trigger Enable
                          ||||||||||||||| _ BIT  0: PLF: PWMxL Falling Edge Trigger Enable
                          ||||||||||||||||  */
#define REG_PGxLEBH     0b0000000000001000

/**
 * @def REG_PGxLEBL 
 * @brief PGxLEBL: PWM GENERATOR x LEADING-EDGE BLANKING REGISTER LOW
 */
/* PGxLEBL: PWM GENERATOR x LEADING-EDGE BLANKING REGISTER LOW

                           ________________ BIT 15: LEB[15:0]: Leading-Edge Blanking Period bits
                          | _______________ BIT 14: 
                          || ______________ BIT 13: 
                          ||| _____________ BIT 12: 
                          |||| ____________ BIT 11: 
                          ||||| ___________ BIT 10: 
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: 
                          ||||||||| _______ BIT  6: 
                          |||||||||| ______ BIT  5: 
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: 
                          ||||||||||||| ___ BIT  2: 
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PGxLEBL     0b0000000000000000
    
/**
 * @def REG_PGxDCA
 * @brief PGxDCA: PWM GENERATOR x DUTY CYCLE ADJUSTMENT REGISTER
 */
/* 

                           ________________ BIT 15: PGxDCA[15:0]: PWM Generator x Duty Cycle Adjustment Register
                          | _______________ BIT 14: 
                          || ______________ BIT 13: 
                          ||| _____________ BIT 12: 
                          |||| ____________ BIT 11: 
                          ||||| ___________ BIT 10: 
                          |||||| __________ BIT  9: 
                          ||||||| _________ BIT  8: 
                          |||||||| ________ BIT  7: 
                          ||||||||| _______ BIT  6: 
                          |||||||||| ______ BIT  5: 
                          ||||||||||| _____ BIT  4: 
                          |||||||||||| ____ BIT  3: 
                          ||||||||||||| ___ BIT  2: 
                          |||||||||||||| __ BIT  1: 
                          ||||||||||||||| _ BIT  0: 
                          ||||||||||||||||  */
#define REG_PGxDCA      0b0000000000000000


#endif	/* BUCK_CONVERTER_PERIPHERAL_CONFIGURATION_PWM_H */

/**@}*/
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copyright (C) 2016 Microchip Technology Inc.
;
; MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
; derivatives created by any person or entity by or on your behalf, exclusively
; with Microchip's products.  Microchip and its licensors retain all ownership
; and intellectual property rights in the accompanying software and in all
; derivatives here to.
;
; This software and any accompanying information is for suggestion only.  It
; does not modify Microchip's standard warranty for its products.  You agree
; that you are solely responsible for testing the software and determining its
; suitability.  Microchip has no obligation to modify, test, certify, or
; support the software.
;
; THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
; EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
; WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
; PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP�S PRODUCTS,
; COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
;
; IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
; (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
; INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
; EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
; ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
; MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
; TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
; CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
; FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
;
; MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
; TERMS.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


.include "xc.inc"
#include "../h/define.h"
#include "../h/boost_defines.h"
#include "../h/buck_defines.h"


/**
 * void InitAltRegContext1Setup(void);
 * Initializes the first Alternate W-register set for extremely rapid access to 
 * Buck Converter parameters in the buck handler ISR.
 *
 * @return Context 1 Alternate W-register set is loaded with needed parameters 
 *         and the default Context 0 is reselected before return.
 */
    .pushsection .text.InitAltRegContext1Setup, code
    .global _InitAltRegContext1Setup
    .type   _InitAltRegContext1Setup, @function
_InitAltRegContext1Setup:
    ctxtswp #0x1   ;Swap to Alternate W-Reg context #1

    ; Note: w0 register will be used for compensator control reference parameter

    ; Initialize Alternate Working Registers context #1
    mov     #ADCBUF1,                   w1
    mov     #PDC1,                      w2
    mov     #BUCK_COMP_3P3Z_PRESHIFT,   w3
    ; W4, W5 used for MAC/MPY instructions

    mov     #BUCK_COMP_3P3Z_POSTSHIFT,  w6
    mov     #BUCK_COMP_3P3Z_POSTSCALER, w7
    mov     #_buckACoefficients3P3Z,    w8
    mov     #_buckBCoefficients3P3Z,    w9
    mov     #_buckErrorHistory3P3Z,     w10
    mov     #_buckControlHistory3P3Z,   w11
    mov     #BUCK_COMP_3P3Z_MIN_CLAMP,  w12
    mov     #BUCK_COMP_3P3Z_MAX_CLAMP,  w13
    mov     #0x0080,                    w14

    ctxtswp #0x0  ; Swap back to main register set
    return        ; Exit AltWREG1Setup funtion
    
    .size   _InitAltRegContext1Setup, . - _InitAltRegContext1Setup
    .popsection

    
/**
 * void InitAltRegContext1Setup(void);
 * Initializes the second Alternate W-register set for extremely rapid access to 
 * Boost Converter parameters in the boost handler ISR.
 *
 * @return Context 2 Alternate W-register set is loaded with needed parameters 
 *         and the default Context 0 is reselected before return.
 */
    .pushsection .text.InitAltRegContext2Setup, code
    .global _InitAltRegContext2Setup
    .type   _InitAltRegContext2Setup, @function
_InitAltRegContext2Setup:
    ctxtswp #0x2   ; Swap to Alternate W-Reg #2
    
    ; Note: w0 register will be used for compensator control reference parameter

    ; Initialize Alternate Working Registers context #2
    mov     #ADCBUF3,                    w1
    mov     #PDC2,                       w2
    mov     #BOOST_COMP_3P3Z_PRESHIFT,   w3
    ; w4, w5 used for MAC/MPY instructions

    mov     #BOOST_COMP_3P3Z_POSTSHIFT,  w6
    mov     #BOOST_COMP_3P3Z_POSTSCALER, w7
    mov     #_boostACoefficients,        w8
    mov     #_boostBCoefficients,        w9
    mov     #_boostErrorHistory,         w10
    mov     #_boostControlHistory,       w11
    mov     #BOOST_COMP_3P3Z_MIN_CLAMP,  w12
    mov     #BOOST_COMP_3P3Z_MAX_CLAMP,  w13
    mov     #0x0080,                     w14

    ctxtswp #0x0  ; Swap back to main register set
    return        ; Exit AltWREG2Setup function

    .size   _InitAltRegContext2Setup, . - _InitAltRegContext2Setup
    .popsection
    
.end


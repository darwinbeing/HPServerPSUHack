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
; PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
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


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ISR:          ADCAN1Interrupt
; Description:	Interrupt for Buck converter voltage and current control.
;               Reads the buckCurrent and buckVoltage values via the ADC pins
;               and makes functin call to SMPS_Controller3P3Z structure.
; Working Reg:  Only uses Alt W-Reg Context #1 (CTXT1_IPL7 Config must be set)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    .pushsection    .text._ADCAN1Interrupt, code, keep
    .global __ADCAN1Interrupt
    .type   __ADCAN1Interrupt, @function
    .extern _buckControlReference
    .extern _SMPS_Controller3P3Z
    .extern _buckCurrent
    .extern _buckVoltage
    .extern _criticalISRDone
    .extern _buckACoefficients3P3Z
    .extern _buckBCoefficients3P3Z
    .extern _buckErrorHistory3P3Z
    .extern _buckControlHistory3P3Z
__ADCAN1Interrupt:
    bset LATB, #3
    mov  _buckControlReference, w0
    call _SMPS_Controller3P3Z

    mov  ADCBUF0, w0
    mov  w0, _buckCurrent
    mov  ADCBUF1, w0
    mov  w0, _buckVoltage

    .if(BOOST == DISABLED)
    setm _criticalISRDone
    .endif

    bclr IFS6, #ADCAN1IF	    ; Clear ADCAN1 interrupt flag.
    bclr LATB, #3
    retfie            ; Return from interrupt service routine

    .popsection
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   End of ADCAN1Interrupt ISR
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ISR:          ADCAN3Interrupt
; Description:	Interrupt for Boost converter voltage and current control
;               Reads the boostCurrent and boostVoltage values via the ADC pins
;               and makes functin call to SMPS_Controller3P3Z structure.
; Working Reg:  Only uses Alt W-Reg Context #2 (CTXT2_IPL6 Config must be set)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    .pushsection .text._ADCAN3Interrupt, code, keep
    .global __ADCAN3Interrupt
    .type   __ADCAN3Interrupt, @function
    .extern _boostControlReference
    .extern _SMPS_Controller3P3Z
    .extern _boostCurrent
    .extern _boostVoltage
    .extern _criticalISRDone
__ADCAN3Interrupt:
    bset LATB, #3
    mov  _boostControlReference, w0
    call _SMPS_Controller3P3Z

    mov  ADCBUF2, w0
    mov  w0, _boostCurrent
    mov  ADCBUF3, w0
    mov  w0, _boostVoltage

    setm _criticalISRDone
    bclr IFS7, #ADCAN3IF	    ; Clear ADCAN3 interrupt flag.
    bclr LATB, #3
    retfie               ; Return from interrupt service routine

    .popsection
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   End of ADCAN3Interrupt ISR
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.end


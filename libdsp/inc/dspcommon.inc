;*********************************************************************
;                                                                    *
;                       Software License Agreement                   *
;                                                                    *
;   The software supplied herewith by Microchip Technology           *
;   Incorporated (the "Company") for its dsPIC controller            *
;   is intended and supplied to you, the Company's customer,         *
;   for use solely and exclusively on Microchip dsPIC                *
;   products. The software is owned by the Company and/or its        *
;   supplier, and is protected under applicable copyright laws. All  *
;   rights are reserved. Any use in violation of the foregoing       *
;   restrictions may subject the user to criminal sanctions under    *
;   applicable laws, as well as to civil liability for the breach of *
;   the terms and conditions of this license.                        *
;                                                                    *
;   THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION.  NO           *
;   WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING,    *
;   BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND    *
;   FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE     *
;   COMPANY SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,  *
;   INCIDENTAL OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.  *
;                                                                    *
;   (c) Copyright 2003 Microchip Technology, All rights reserved.    *
;*********************************************************************

        ; Global inclusions.
        .nolist
        ; none...
        .list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Some constants:
        .equ    kSof,2                          ; sizeof (fractional), or
                                                ; sizeof (fractional*) if
                                                ; addressing data memory
        .equ    kSinPiQ,0x5A82                  ; sin (pi/4)
        .equ    kInvSqrt2,kSinPiQ               ; 1/sqrt(2) = sin (pi/4)

;............................................................................

        ; Declare special function registers as of DS70030C page 4-3,4
        ; since IAR does not recognize the symbols CORCON, MODCON, ...
        .equ    PSVPAG,0x0034
        .equ    CORCON,0x0044
        .equ    MODCON,0x0046
        .equ    XBREV,0x0050
        .equ    XMODSRT,0x0048
        .equ    XMODEND,0x004A
        .equ    YMODSRT,0x004C
        .equ    YMODEND,0x004E

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        ; Operational modes for fractional computation:
        .equ    SATA_ON,1<<7                    ; AccA sat. enabled
        .equ    SATB_ON,1<<6                    ; AccB sat. enabled
        .equ    SATDW_ON,1<<5                   ; data write sat. enabled
        .equ    ACCSAT_ON,1<<4                  ; Accs sat. 9.31 enabled
        .equ    FRACT_SET,SATA_ON|SATB_ON|SATDW_ON|ACCSAT_ON    ; set mask

        .equ    RND_OFF,~(1<<1)                 ; convergent rnd. enabled
        .equ    IF_OFF,~(1<<0)                  ; multiply fractional enabled
        .equ    FRACT_RESET,RND_OFF&IF_OFF      ; reset mask

        .equ    FRACT_MODE,FRACT_SET&FRACT_RESET        ; fractional mask

        .ifdef  TOOLVERS                        ; [
        .equ    FRACT_MODE,FRACT_MODE&0x3ff     ; get rid of leading (sign)
                                                ; bits so that the mask can
                                                ; be used as a Slit10
        .endif  ; ]

        ; Operational modes for Program Space Visibility.
	.ifdef __dsPIC33F
        .equ    PSV_ON,1<<2                     ; program space visible in
                                                ; data space
	.endif

;............................................................................

        ; Prepare CORCON for fractional computation.
        .macro  fractsetup      wx              ; [
        mov     #FRACT_MODE,\wx
        mov     \wx,CORCON
        .endm   ; ]

        ; Prepare CORCON for Program Space Visibility access.
        .macro  psvaccess       wx              ; [
        mov     CORCON,\wx
        ior     #PSV_ON,\wx
        mov     \wx,CORCON
        .endm   ; ]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; FIR filter structure access.
        .equ    COEFFS_IN_DATA,0xFF00   ; indicates that coefficients are in
                                        ; (X) data memory.

        .equ    oNumCoeffs,0            ; FIRFilter->numCoeffs : (int)
        .equ    oCoeffsBase,(oNumCoeffs+kSof)
                                        ; FIRFilter->coeffsBase : (fractional*)
        .equ    oCoeffsEnd,(oCoeffsBase+kSof)
                                        ; FIRFilter->coeffsEnd : (fractional*)
        .equ    oCoeffsPage,(oCoeffsEnd+kSof)
                                        ; FIRFilter->coeffsPage : (int)
        .equ    oDelayBase,(oCoeffsPage+kSof)
                                        ; FIRFilter->delayBase : (fractional*)
        .equ    oDelayEnd,(oDelayBase+kSof)
                                        ; FIRFilter->delayEnd : (fractional*)
        .equ    oDelay,(oDelayEnd+kSof)
                                        ; FIRFilter->delay : (fractional*)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; IIR Lattice filter structure access.
        .equ    oOrder,0                ; filter->order : (int)
        .equ    oKappaVals,(oOrder+kSof); filter->kappaVals : (fractional*)
        .equ    oGammaVals,(oKappaVals+kSof); filter->gammaVals : (fractional*)
        .equ    oLattCoeffsPage,(oGammaVals+kSof); filter->coeffsPage : (int)
        .equ    oLattDelay,(oLattCoeffsPage+kSof); filter->delay : (fractional*)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; OEF

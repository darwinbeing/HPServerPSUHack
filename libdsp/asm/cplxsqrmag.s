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


        ; Local inclusions.
        .nolist
        .include        "dspcommon.inc"         ; fractsetup
        .list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        .section .libdsp, code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _SquareMagnitudeCplx: Computation of the Squared Magnitude of an array of
;                       complex data elements,
;                       srcV[n] = Real(srcV[n])+j*Imaginary(srcV[n])
;
; Operation:
;       dstV[n] = Real(srcV[n])^2 + Imag(srcV[n])^2, with
;
; n in {0, 1,... , numElems-1}
;
; Input:
;       w0 = number elements in vector(s) (numElems)
;       w1 = ptr to complex source vector (srcV)
;            Elements in srcV are assumed to be stored such that
;            the real and imaginary portions of the complex element
;            are located in consecutive 16-bit words, with the
;            Real part in the lower address and the Imaginary part
;            in the higher address
;       w2 = ptr to Real destination vector (dstV)
;            Elements in dstV are 16-bit real elements that
;            represent the squared magnitude of the complex
;            elements in the srcV vector
;       Note: It is allowed for the source and destination vectors
;             to be the same.
; Return:
;       w0 = ptr to Real destination vector (dstV)
;
; System resources usage:
;       {w0..w5}        used, not restored
;        AccuA          used, not restored
;        CORCON         saved, used, restored
;
; DO and REPEAT instruction usage.
;       1 level DO instruction
;       no REPEAT intructions
;
; Program words (24-bit instructions):
;       19
;
; Cycles (including C-function call and return overheads):
;       20 + 3*numElems
;............................................................................

        .global _SquareMagnitudeCplx    ; provide global scope to routine
_SquareMagnitudeCplx:

;............................................................................
.ifndef PSV_ON
	    push	DSRPAG
	    movpag #0x0001, DSRPAG
.endif

        ; Save working registers.
        push    w2                      ; save return value (dstV)
        push.d  w4
        push    w10

        push    CORCON                  ; Prepare CORCON for fractional computation.
        fractsetup      w4
;............................................................................

        ; Prepare operation.
        mov     w1, w10                 ; w10 points to beginning of complex input array
        clr     a, [w10]+=2, w4         ; Fetch Real(srcV[0]) and increment pointer to array

        dec2    w0, w0                  ; w0 = numElems-1
        do      w0, L0
.ifdef YMEM_ERRATA
    nop
.endif
        mpy     w4*w4, a, [w10]+=2, w5  ; accA = Real(srcV[n])^2, fetch Imag(srcV[n])
        mac     w5*w5, a, [w10]+=2, w4  ; accA = Real(srcV[n])^2 + Imag(srcV[n])^2
L0:     sac.r   a, #0, [w2++]           ; round, saturate and store result in dstV[n] array

;Last iteration outside the DO loop
        mpy     w4*w4, a, [w10]+=2, w5  ; accA = Real(srcV[n])^2, fetch Imag(srcV[n])
        mac     w5*w5, a                ; accA = Real(srcV[n])^2 + Imag(srcV[n])^2
        sac.r   a, #0, [w2++]           ; round, saturate and store result in dstV[n] array

; }

;............................................................................

        ; restore CORCON.
        pop     CORCON

        ; Restore working registers.
        pop     w10
        pop.d   w4
        pop     w0                              ; restore return value.

.ifndef PSV_ON
	    pop	DSRPAG
.endif
;............................................................................

        return

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        .end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF


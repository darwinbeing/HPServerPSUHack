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
	.include	"dspcommon.inc"		; fractsetup
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _VectorNegate: Vector Negation.
;
; Operation:
;	dstV[n] = (-1)*srcV1[n] + 0, 0 <= n < numElems
;
; NOTE: negate of 0x8000 is 0x7FFF.
;
; Input:
;	w0 = number elements in vector(s) (numElems)
;	w1 = ptr to destination vector (dstV)
;	w2 = ptr to source vector (srcV)
; Return:
;	w0 = ptr to destination vector (dstV)
;
; System resources usage:
;	{w0..w5}	used, not restored
;	 AccuA		used, not restored
;	 CORCON		saved, used, restored
;
; DO and REPEAT instruction usage.
;	1 level DO instruction
;	no REPEAT intructions

;............................................................................

	.global	_VectorNegate	; export
_VectorNegate:

;............................................................................
.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif

;............................................................................

	; Prepare CORCON for fractional computation.
	push	CORCON
	fractsetup	w3

;............................................................................

	mov	w1,w3				; save return value (dstV)

;............................................................................

	; Prepare operation.
	dec	w0,w0				; w0 = numElems-1
	mov	#0x8000,w4			; w4 = -1[Q.15] (to negate)
	clr	a				; a  = 0

	; Perform operation.
	do	w0,_endNeg		; {	; do (numElems-1)+1 times
	mov	[w2++],w5			; w5 = srcV[n]
						; w2-> srcV[n+1]
	mac	w4*w5,a				; a = (-1)*srcV[n]
	sac	a,[w1++]			; *(dstV++) = -(*(srcV))
_endNeg:
	clr	a				; reset
; }

;............................................................................

	mov	w3,w0				; restore return value

;............................................................................

	; restore CORCON.
	pop	CORCON

;............................................................................

.ifndef PSV_ON
	pop	DSRPAG
.endif
;............................................................................

	return	


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

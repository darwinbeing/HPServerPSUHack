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
; _VectorDotProduct: Vector Dot Product.
;
; Operation:
;	dotVal = sum (srcV1[n] * srcV2[n]), with
;
; n in {0, 1,... , numElems-1}
;
; Input:
;	w0 = number elements in vector(s) (numElems)
;	w1 = ptr to source one vector (srcV1)
;	w2 = ptr to source two vector (srcV2)
; Return:
;	w0 = dot product value (dotVal)
;
; System resources usage:
;	{w0..w2}	used, not restored
;	{w4..w5}		used, not restored
;	 AccuA		used, not restored
;	 CORCON		saved, used, restored
;
; DO and REPEAT instruction usage.
;	1 level DO instruction
;	no REPEAT intructions

;............................................................................

	.global	_VectorDotProduct	; export
_VectorDotProduct:

;............................................................................
.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif

;............................................................................

	; Prepare CORCON for fractional computation.
	push	CORCON
	fractsetup	w4

;............................................................................

	; Prepare operation.
	dec	w0,w0				; w0 = numElems-1
	clr	a				; a = 0

	; Perform operation.
	do	w0,_endDot		; {	; do (numElems-1)+1 times
	mov	[w1++],w4			; w4 = srcV1[n]
						; w1-> srcV1[n+1]
	mov	[w2++],w5			; w5 = srcV2[n]
						; w2-> srcV2[n+1]
_endDot:
	mac	w4*w5,a				; a += srcV1[n]*srcV2[n]
; }

;............................................................................

	sac.r	a,w0				; dotVal return value

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

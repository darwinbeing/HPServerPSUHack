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
	.include	"dspcommon.inc"		; FIR filter structure
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _FIRDelayInit: initialization to zero of FIR filter delay.
;
; Operation:
;	FIRFilter->delayBase[m] = 0, 0 <= m < FIRFilter->numCoeffs (= M)
;
; Input:
;	w0 = h, ptr FIR filter structure (see included file)
; Return:
;	(void)
;
; System resources usage:
;	{w0..w2}	used, not restored
;
; DO and REPEAT instruction usage.
;	no DO instructions
;	1 level REPEAT intruction


	.global	_FIRDelayInit	; export
_FIRDelayInit:
.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif
;............................................................................

	; Prepare operation.
	mov	[w0+oNumCoeffs],w1		; w1 = M
	dec	w1,w2				; w2 = M-1
	mov	[w0+oDelayBase],w1		; w1-> delayBase[0]
	mov	#0,w0				; w0 = 0

;............................................................................

	; Perform operation.
	repeat	w2				; do (M-1)+1 times
	mov	w0,[w1++]			; delayBase[m] = 0
						; w1-> delayBase[m+1]

;............................................................................
.ifndef PSV_ON
	pop	DSRPAG
.endif
	return	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

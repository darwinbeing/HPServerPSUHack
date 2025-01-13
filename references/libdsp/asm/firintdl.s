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
; _FIRInterpDelayInit: initialization to zero of FIR interpolator filter delay.
;
; Operation:
;	FIRFilter->delayBase[m] = 0, 0 <= m < FIRFilter->numCoeffs/rate (= M/R)
;
; Input:
;	w0 = h, ptr FIR filter structure (see included file)
;	w1 = R, rate of interpolation R (from 1)
; Return:
;	(void)
;
; System resources usage:
;	{w0..w4}	used, not restored
;
; DO and REPEAT instruction usage.
;	no DO instructions
;	1 level REPEAT intruction

	.global	_FIRInterpDelayInit	; export
_FIRInterpDelayInit:
.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif
;............................................................................
	; Prepare operation.
	mov	[w0+oNumCoeffs],w2		; w2 = M
	; Compute (M/R)-1 = q-1:
	mov	w1,w3				; w3 = R (increment till M)
	mov	#0x0,w4				; w4 = 0 (increment till q-1)
_divide:
	cp	w2,w3				; M-R*(p+1) ?
	bra	z,_done				; M-R*(p+1)=0 => p=q-1 (done)
	inc	w4,w4				; w4 = p+1
	add	w1,w3,w3			; w3 = R*(p+2)
	bra	_divide
_done:
						; now:
						; w4 = (M/R)-1
	mov	[w0+oDelayBase],w2		; w2-> delayBase[0]
	mov	#0,w0				; w0 = 0
;............................................................................
	; Perform operation.
	repeat	w4				; do (M/R-1)+1 times
	mov	w0,[w2++]			; delayBase[m] = 0
						; w2-> delayBase[m+1]
;............................................................................
.ifndef PSV_ON
	pop	DSRPAG
.endif
	return	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.end
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

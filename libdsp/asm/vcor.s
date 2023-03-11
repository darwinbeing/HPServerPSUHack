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
; _VectorCorrelate: Vector correlation (using convolution).
;
; Operation:
;	r[n] = sum_(k=0:N-1){x[k]*y[k+n]},
; where:
;	x[n] defined for 0 <= n < N,
;	y[n] defined for 0 <= n < M, (M <= N),
;	r[n] defined for 0 <= n < N+M-1,
;
; Input:
;	w0 = N, number elements in vector one
;	w1 = M, number elements in vector two
;	w2 = r, ptr to destination vector, with R elements
;	w3 = x, ptr to source vector one
;	w4 = y, ptr to source vector two
; Return:
;	w0 = r, ptr to destination vector
;
; System resources usage:
;	{w0..w7}	used, not restored
; plus resuorces from VectorConvolve.
;
; DO and REPEAT instruction usage.
;	1 level DO instruction
;	no REPEAT intructions
; plus Do/REPEAT instructions from VectorConvolve.

;............................................................................

	; External symbols.
	.extern	_VectorConvolve

	.global	_VectorCorrelate	; export
_VectorCorrelate:

;............................................................................
.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif

;............................................................................

	; Compute correlation (using convolution).

	; First, revert y (source vector two).
	mov	w4,w7				; w7-> y[0]
	dec	w1,w5				; w5 = M-1
	sl	w5,w5				; w5 = (M-1)*sizeof(fractional)
	add	w4,w5,w6			; w6-> y[M-1]
	lsr	w1,w5				; w5 = floor (M/2)
	dec	w5,w5				; w5 = M/2-1
	do	w5,_endRevert		; {	; do (M/2-1)+1 times
						; w5 up for grabs...
	mov	[w6],w5				; w5 = y[M-1-n]
	mov	[w7],[w6--]			; y[n] into y[M-1-n]
						; w6-> y[M-1-(n+1)]
_endRevert:
	mov	w5,[w7++]			; y[M-1-n] into y[n]
						; w7-> y[n+1]
; }

	; Then, invoke convolution...
	call	_VectorConvolve

;............................................................................

.ifndef PSV_ON
	pop	DSRPAG
.endif
;............................................................................

	return			; NOTE that w0 is set up by _VectorConvolve

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

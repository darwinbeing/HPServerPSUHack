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
	.include	"dspcommon.inc"		; MODCON, XBREV
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _BitReverseComplex: Complex (in-place) Bit Reverse re-organization.
;
; Operation:
;
; Input:
;	w0 = number stages in FFT (log2NVal)
;	w1 = ptr to complex source vector (srcCV)
; Return:
;	w0 = ptr to source vector (srcCV)
;
; System resources usage:
;	{w0..w7}	used, not restored
;	 MODCON		saved, used, restored
;	 XBREV		saved, used, restored
;
; DO and REPEAT instruction usage.
;	1 level DO intruction
;	no REPEAT intructions


	.global	_BitReverseComplex	; export
_BitReverseComplex:

;............................................................................

	; Save working registers.
	; none to save...

;............................................................................
.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif

	; Prepare for bit reverse addressing via w3.
	push	MODCON
	push	XBREV
	mov	#0x0200,w2			; uses w2 for bit reverse
	mov	w2,MODCON
	mov	#0x8000,w2			; enable bit reverse addresing
	mov	#0x1,w3				; shifting base
	sl	w3,w0,w3			; w3 = N = 2^(log2NVal)
	ior	w3,w2,w2
	mov	w2,XBREV			; pivot N (equivalent to a
						; pivot N/2 for complex data)


;............................................................................

	mov	w1,w0				; save return value (srcCV)

;............................................................................

	; Bit reversal.
	mov	w1,w2				; w2: bit reverse addressing
						; w1: sequential addressing
	mov	#2,w6				; w6 = sizeof(fractional)
	mov	#4,w7				; w7 = sizeof(fractcomplex)

	dec	w3,w3				; w3 = N-1
	do	w3,_endBitRev		; {	; do (N-1)+1 times
	cp	w2,w1				; w2-w1 ?
	bra	le,_skip			; w2 <= w1 => skip to next
						; otherwise,
	; Swap complex value pairs.
	mov.d	[w1],w4				; w4 = real[n], w5 = imag[n]
	mov	[w2],[w1]			; replace real part sequential
	mov	[w2+w6],[w1+w6]			; replace imag part sequential
	mov.d	w4,[w2]				; replace real/imag bit reverse

_skip:	; Pointers update:
	add	w1,w7,w1			; w1 += sizeof(fractcomplex)
						; sequential update

_endBitRev:
	mov	[w2],[w2++]			; bit reverse update
						; (only on data writes!!!)
; }

;............................................................................

	; Restore configuration.
	pop	XBREV
	pop	MODCON
.ifndef PSV_ON
	pop	DSRPAG
.endif
;............................................................................

	; Restore working registers.
	; none to restore...

;............................................................................

	return

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

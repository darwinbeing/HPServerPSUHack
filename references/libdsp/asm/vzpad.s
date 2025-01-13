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
; _VectorZeroPad: pad with zeros the numZeros tail elements of a vector.
;
; Operation:
;	dstV[n] = srcV[n], 0 <= n < numElems
;	dstV[n] = 0, numElems <= n < numElems+numZeros
; NOTE:
;	srcV defined AT LEAST WITHIN 0 <= n < numElems
;	dstV defined EXACTLY WITHIN 0 <= n < numElems+numZeros
;
; Input:
;	w0 = number elements in source vector to be preserved (numElems)
;	w1 = number elements to zero pad (numZeros)
;	w2 = ptr to destination vector (dstV)
;	w3 = ptr to source vector (srcV)
; Return:
;	w0 = ptr to destination vector (dstV)
;
; System resources usage:
;	{w0..w6}	used, not restored
;
; DO and REPEAT instruction usage.
;	no DO instructions
;	1 level REPEAT intruction

;............................................................................

	; External symbols.
	.extern	_VectorCopy

	.global	_VectorZeroPad	; export
_VectorZeroPad:
.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif
;............................................................................

	mov	w2,w6				; save return value (dstV)

	; Copy numElems from srcV to dstV.
	sl	w0,w4				; w4 = sizeof(numElems)
	dec	w1,w5				; w5 = numZeros-1
	mov	w2,w1				; w1-> dstV[0]
	mov	w3,w2				; w2-> srcV[0]
	call _VectorCopy
						; now w0-> dstV[0]
	; Advance to zero padding area.
	add	w0,w4,w0			; w0-> dstV[numElems]
	mov	#0,w1
	repeat	w5				; do (numZeros-1)+1 times
	mov	w1,[w0++]			; dstV[n] = 0
						; w0-> dstV[n+1]

	mov	w6,w0				; restore return value

;............................................................................
.ifndef PSV_ON
	pop	DSRPAG
.endif
	return	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

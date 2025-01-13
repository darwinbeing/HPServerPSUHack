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
; _VectorMax: Vector Maximum Value (and last index with maximum value).
;
; Operation:
;	maxVal = max {srcV[n], n in {0, 1,...numElems-1} }
;   residually,
;	if srcV[i] = srcV[j] = maxVal, and i < j, then *(maxIndex) = j
;
; Input:
;	w0 = number elements in vector(s) (numElems)
;	w1 = ptr to source vector (srcV)
;	w2 = ptr to index of maximum value (&maxIndex)
; Return:
;	w0 = maximum value (maxVal)
;
; System resources usage:
;	{w0..w5}	used, not restored
;
; DO and REPEAT instruction usage.
;	no DO intructions
;	no REPEAT intructions

;............................................................................

	.global	_VectorMax	; export
_VectorMax:
.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif
;............................................................................

	mov	[w1++],w5			; w5 = srcV[0] (current maxVal)
	clr	[w2]				; *w2 = current max value index
	dec	w0,w4				; w4 = numElems-1
	bra	le,_endMax			; no more elements...
_compare:
	mov	[w1++],w3			; w3 = srcV[n+1]
	cp	w5,w3				; srcV[n] < srcV[n+1] ?
	bra	gt,_noUpdate			; yes => no update
_doUpdate:					; no  => do update
	mov	w3,w5				; w5 = (current maxVal)
	sub	w0,w4,[w2]			; *w2 = current max value index
_noUpdate:
	dec	w4,w4				; w4 = numElems-n
	bra	gt,_compare			; no more elements...
_endMax:
	mov	w5,w0				; restore return value

;............................................................................
.ifndef PSV_ON
	pop	DSRPAG
.endif
	return	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

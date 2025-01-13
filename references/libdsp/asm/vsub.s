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
; _VectorSubtract: Vector Subtraction.
;
; Operation:
;	dstV[n] = srcV1[n] - srcV2[n], with
;
; n in {0, 1,... , numElems-1}
;
; Input:
;	w0 = number elements in vector(s) (numElems)
;	w1 = ptr to destination vector (dstV)
;	w2 = ptr to source one vector (srcV1)
;	w3 = ptr to source two vector (srcV2)
; Return:
;	w0 = ptr to destination vector (dstV)
;
; System resources usage:
;	{w0..w4}	used, not restored
;	 AccuA		used, not restored
;	 AccuB		used, not restored
;	 CORCON		saved, used, restored
;
; DO and REPEAT instruction usage.
;	1 level DO instruction
;	no REPEAT intructions

;............................................................................

	.global	_VectorSubtract	; export
_VectorSubtract:

.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif

	; Prepare CORCON for fractional computation.
	push	CORCON
	fractsetup	w4

;............................................................................

	mov	w1,w4				; save return value (dstV)

;............................................................................

	; Prepare operation.
	dec	w0,w0				; w0 = numElems-1

	; Perform operation.
	do	w0,_endSub		; {	; do (numElems-1)+1 times
.ifdef PSV_ON_ERRATA
	mov	[w2++],w5
	lac	w5,a			; a  = srcV1[n]
						; w2-> srcV1[n+1]
	mov	[w3++],w5
	lac	w5,b			; b  = srcV2[n]
						; w3-> srcV2[n+1]
.else
	lac	[w2++],a			; a  = srcV1[n]
						; w2-> srcV1[n+1]
	lac	[w3++],b			; b  = srcV2[n]
						; w3-> srcV2[n+1]
.endif
	sub	a				; a -= b
_endSub:
	sac	a,[w1++]			; dstV[n] = srcV1[n] - srcV2[n]
; }

;............................................................................

	mov	w4,w0				; restore return value

;............................................................................

	; restore CORCON.
	pop	CORCON

.ifndef PSV_ON
	pop	DSRPAG
.endif
;............................................................................

	return	


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

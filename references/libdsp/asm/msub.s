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
; _MatrixSubtract: Matrix subtraction.
;
; Operation:
;	dstM[i][j] = srcM1[i][j] - srcM2[i][j]
;
; Input:
;	w0 = number rows in source matrix (numRows)
;	w1 = number cols in source matrix (numCols)
;	w2 = ptr to destination matrix (dstM)
;	w3 = ptr to source one matrix (srcM1)
;	w4 = ptr to source two matrix (srcM2)
; Return:
;	w0 = ptr to destination matrix (dstM)
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

	.global	_MatrixSubtract	; export
_MatrixSubtract:

.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif
;............................................................................

	; Prepare operation.
	mul.uu	w0,w1,w0			; w0 = numRows*numCols
						; w1 available for reuse
	dec	w0,w0				; w0 = num elements-1

;............................................................................

	; Prepare CORCON for fractional computation.
	push	CORCON
	fractsetup	w1

;............................................................................

	mov	w2,w1				; save return value (dstV)

;............................................................................

	; Perform operation.
	do	w0,_endSub		; {	; do (num elems-1)+1 times
.ifdef PSV_ON_ERRATA
	mov	[w3++],w5
	lac	w5,a			; a  = srcM1[r][c]
						; w3-> srcM1[r][c+1]
	mov	[w4++],w5
	lac	w5,b			; b  = srcM2[r][c]
						; w4-> srcM2[r][c+1]
.else
	lac	[w3++],a			; a  = srcM1[r][c]
						; w3-> srcM1[r][c+1]
	lac	[w4++],b			; b  = srcM2[r][c]
						; w4-> srcM2[r][c+1]
.endif
	sub	a				; a += b
_endSub:
	sac	a,[w2++]			; dstM[n] =
						;    srcM1[r][c] - srcM2[r][c]
						; w2-> dstM[r][c+1]
; }

;............................................................................

	mov	w1,w0				; restore return value

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

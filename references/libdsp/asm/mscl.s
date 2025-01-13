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
; _MatrixScale: Matrix scale.
;
; Operation:
;	dstM[i][j] = sclVal * srcM[i][j]
;
; Input:
;	w0 = number rows in source matrix (numRows)
;	w1 = number cols in source matrix (numCols)
;		(NOTE: numRows*numCols < 2^14)
;	w2 = ptr to destination matrix (dstM)
;	w3 = ptr to source matrix (srcM)
;	w4 = scale value (sclVal)
; Return:
;	w0 = ptr to destination matrix (dstM)
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

	.global	_MatrixScale	; export
_MatrixScale:

;............................................................................

	; Save working registers.
	; None to save...

;............................................................................
.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif
	; Prepare CORCON for fractional computation.
	push	CORCON
	fractsetup	w5

;............................................................................

	; Prepare operation.
	mul.uu	w0,w1,w0			; w0 = numRows*numCols
	dec	w0,w0				; w0 = num elements-1
						; w4 = sclVal from input

	mov	w2,w1				; save return value (dstV)

	; Perform operation.
	do	w0,_endScl		; {	; do (num elems-1)+1 times
	mov	[w3++],w5			; w5 = srcM(n)
						; w3-> srcM(n+1)
	mpy	w4*w5,a				; a=sclVal*srcM(n)
_endScl:
	sac.r	a,[w2++]			; dstM[r][c] = sclVal*srcM[r][c]
						; w2-> dstM(n+1)
; }

	mov	w1,w0				; restore return value

;............................................................................

	; restore CORCON.
	pop	CORCON
.ifndef PSV_ON
	pop	DSRPAG
.endif
;............................................................................

	; Restore working registers.
	; None to restore...

;............................................................................

	return	


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

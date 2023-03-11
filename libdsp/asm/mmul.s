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
; _MatrixMultiply: Matrix Multiplication.
;
; Operation:
;	dstM[i][j] = sum_k(srcM1[i][k]*srcM2[k][j]), with
; i in {0, 1, ..., numRows1-1}
; j in {0, 1, ..., numCols2-1}
; k in {0, 1, ..., numCols1Rows2-1}
;
; Input:
;	w0 = number rows in source one matrix (numRows1)
;	w1 = number cols in source one matrix,
;	     and rows in source tow matrix (numCols1Rows2)
;	w2 = number cols in source two matrix (numCols2)
;	w3 = ptr to destination matrix (dstM)
;	w4 = ptr to source one matrix (srcM1)
;	w5 = ptr to source two matrix (srcM2)
; Return:
;	w0 = ptr to destination matrix (dstM)
;
; System resources usage:
;	{w0..w7}	used, not restored
;	{w8..w13}	used, and restored
;	 AccuA		used, not restored
;	 CORCON		saved, used, restored
;
; DO and REPEAT instruction usage.
;	2 level DO instruction
;	no REPEAT intructions

;............................................................................

	.global	_MatrixMultiply	; export
_MatrixMultiply:

;............................................................................
.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif

	; Save working registers.
	push.d	w8				; {w8:w9} to TOS
	push.d	w10				; {w10:w11} to TOS
	push.d	w12				; {w12:w13} to TOS

;............................................................................

	; Prepare CORCON for fractional computation.
	push	CORCON
	fractsetup	w13

;............................................................................

	mov	w3,w13				; save return value (dstM)

;............................................................................

	dec	w1,w12				; w12 = numCols1Rows2-1
	dec	w2,w11				; w11 = numCols2-1
	add	w1,w1,w1			; w1  = sizeof (Cols1Rows2)
	add	w2,w2,w2			; w2  = sizeof (Cols2)

_doRows1:
	dec	w0,w0				; w0  = w0-1
	bra	lt,_doneRows1
						; do (numRows1) times
	mov	w5,w10				; w10-> srcM2[0][0]

	do	w11,_endCols2		; {	; do (numCols2-1)+1 times
.ifndef PSV_ON
    nop
.endif
	mov	w4,w8				; w8 -> srcM1[i][0]
	mov	w10,w9				; w9 -> srcM2[0][j]
	clr	a				; a   = 0

	do	w12,_endCols1Rows2	; {	; do (numCols1Rows2-1)+1 times
	mov	[w8++],w6			; w6  = srcM1[i][k]
						; w8 -> srcM1[i][k+1]
	mov	[w9],w7				; w7  = srcM2[k][j]
	mac	w6*w7,a				; a  += srcM1[i][k]*srcM2[k][j]
_endCols1Rows2:
	add	w9,w2,w9			; w9 -> srcM2[k+1][0]
; }

	sac.r	a,[w3++]			; dst[i][j] =
						; sum_k(srcM1[i][k]*srcM2[k][j])
	; Update for next column.
_endCols2:
	inc2	w10,w10				; w10-> srcM2[0][j+1]
; }

	; Update for next row.
	add	w4,w1,w4			; w4 -> srcM1[i][0]
	bra	_doRows1
_doneRows1:

;............................................................................

	mov	w13,w0				; restore return value

;............................................................................

	; restore CORCON.
	pop	CORCON

;............................................................................

	; Restore working registers.
	pop.d	w12				; {w12:w13} from TOS
	pop.d	w10				; {w10:w11} from TOS
	pop.d	w8				; {w8:w9} from TOS

.ifndef PSV_ON
	pop	DSRPAG
.endif
;............................................................................

	return	


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

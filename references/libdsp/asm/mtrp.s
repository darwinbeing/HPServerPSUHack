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
; _MatrixTranspose: Matrix transposition.
;
; Operation:
;	dstM[i][j] = srcM[j][i]
;
; Input:
;	w0 = number rows in source matrix (numRows)
;	w1 = number cols in source matrix (numCols)
;	w2 = ptr to destination matrix (dstM)
;	w3 = ptr to source matrix (srcM)
; Return:
;	w0 = ptr to destination matrix (dstM)
;
; System resources usage:
;	{w0..w5}	used, not restored
;
; DO and REPEAT instruction usage.
;	2 level DO instruction
;	no REPEAT intructions

;............................................................................

	.global	_MatrixTranspose	; export
_MatrixTranspose:

.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif
;............................................................................

	mov	w2,w5				; save return value (dstV)

;............................................................................

	; Prepare operation.
	dec	w0,w0				; w0 = numRows-1
	dec	w1,w4				; w4 = numCols-1
	add	w1,w1,w1			; w1 = sizeof (Rows)

	; Perform operation.
	do	w4,_endCols		; {	; do (numCols-1)+1 times
	mov	w3,w4				; w4-> srcM[0][c]
	do	w0,_endRows		; {	; do (numRows-1)+1 times
	mov	[w4],[w2++]			; dstM[c][r] = srcM[r][c]
						; w2-> dstM[c][r+1]
_endRows:
	add	w4,w1,w4			; w4-> srcM[r+1][c]
; }
_endCols:
	inc2	w3,w3				; w3-> srcM[0][c+1]
; }

;............................................................................

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

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
; _VectorConvolve: Vector convolution.
;
; Operation:
;	y[n] = sum_(k=0:n){x[k]*h[n-k]},	0 <= n < M
;	y[n] = sum_(k=n-M+1:n){x[k]*h[n-k]},	M <= n < N
;	y[n] = sum_(k=n-M+1:N-1){x[k]*h[n-k]},	N <= n < N+M-1
;
; Input:
;	w0 = N, number elements in vector one
;	w1 = M, number elements in vector two, M <= N
;	w2 = y, ptr to destination vector, with (N + M - 1) elements
;	w3 = x, ptr to source vector one
;	w4 = h, ptr to source vector two
; Return:
;	w0 = y, ptr to destination vector
;
; System resources usage:
;	{w0..w7}	used, not restored
;	{w8..w10}	saved, used, restored
;	 AccuA		used, not restored
;	 CORCON		saved, used, restored
;
; DO and REPEAT instruction usage.
;	2 level DO instruction
;	no REPEAT intructions

;............................................................................

	.global	_VectorConvolve	; export
_VectorConvolve:

;............................................................................
.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif
	; Save working registers.
	push.d	w8				; {w8:w9} to TOS
	push	w10				; {w10} to TOS

;............................................................................

	; Prepare CORCON for fractional computation.
	push	CORCON
	fractsetup	w7

;............................................................................

	; Set up.
	push	w2				; save return value (y)

;............................................................................

	; First stage: y[n] = sum_(k=0:n){x[k]*h[n-k]}, 0 <= n < M.

	; Prepare operation.
	dec	w1,w7				; w7 = M-1
	clr	w10				; w10= 0

	; Perform operation.
	do	w7,_endOutFirst		; {	; do (M-1)+1 times
.ifndef PSV_ON
    nop
.endif
	; Prepare operation.
	mov	w3,w8				; w8-> x[0]
	mov	w4,w9				; w9-> h[n]
	clr	a				; a  = 0

	; Perform operation.
	do	w10,_endInFirst		; {	; do n+1 times
	mov	[w8++],w5			; w5 = x[k]
						; w8-> x[k+1]
	mov	[w9--],w6			; w6 = h[n-k]
						; w9-> h[n-k-1]
_endInFirst:
	mac	w5*w6,a				; a  = x[k]*h[n-k]
; }

	sac.r	a,[w2++]			; y[n] store

	; Update for next operation.
	inc2	w4,w4				; w4-> h[n+1]
_endOutFirst:
	inc	w10,w10				; w10++
; }

	; Update for next stage.
	dec2	w4,w4				; w4-> h[M-1]
	inc2	w3,w3				; w3-> x[1]

	; Second stage: y[n] = sum_(k=n-M+1:n){x[k]*h[n-k]}, M <= n < N. 
	; ONLY if M < N!!!

	; Prepare operation.
	sub	w0,w1,w10			; w10= N-M
	bra	le,_begThird			; M == N (skip second stage)
	dec	w10,w10				; w10= N-M-1

	; Perform operation.
	do	w10,_endOutSecond	; {	; do (N-M-1)+1 times

	; Prepare operation.
	mov	w3,w8				; w8-> x[k]
	mov	w4,w9				; w9-> h[M-1]
	clr	a				; a  = 0

	; Perform operation.
	do	w7,_endInSecond		; {	; do (M-1)+1 times
	mov	[w8++],w5			; w5 = x[k]
						; w8-> x[k+1]
	mov	[w9--],w6			; w6 = h[n-k]
						; w9-> h[n-k-1]
_endInSecond:
	mac	w5*w6,a				; a  = x[k]*h[n-k]
; }

	sac.r	a,[w2++]			; y[n] store

	; Update for next operation.
_endOutSecond:
	inc2	w3,w3				; w3-> x[k]
; }

	; Third stage: y[n] = sum_(k=n-M+1:N-1){x[k]*h[n-k]}, N <= n < N+M-1.
	; ONLY if M > 1!!!

_begThird:
	; Prepare operation.
	dec	w7,w7				; w7 = (M-1)-1
	bra	lt,_noMore			; M == 1 (skip third stage)

	; Perform operation.
	do	w7,_endOutThird		; {	; do (M-2)+1 times

	; Prepare operation.
	mov	w3,w8				; w8-> x[k]
	mov	w4,w9				; w9-> h[M-1]
	clr	a				; a  = 0

	; Perform operation.
	do	w7,_endInThird		; {	; do N+M-1-n times
	mov	[w8++],w5			; w5 = x[k]
						; w8-> x[k+1]
	mov	[w9--],w6			; w6 = h[n-k]
						; w9-> h[n-k-1]
_endInThird:
	mac	w5*w6,a				; a  = x[k]*h[n-k]
; }

	sac.r	a,[w2++]			; y[n] store

	; Update for next operation.
	inc2	w3,w3				; w3-> x[k]
_endOutThird:
	dec	w7,w7				; w7--
; }

_noMore:
;............................................................................

	pop	w0				; restore return value

;............................................................................

	; Restore CORCON.
	pop	CORCON

;............................................................................

	; Restore working registers.
	pop	w10				; {w10} from TOS
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

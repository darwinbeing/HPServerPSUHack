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
	.include	"dspcommon.inc"		; fractsetup,FIRStruct,
						; MODCON, XMODSRT, XMODEND,
						; YMODSRT, YMODEND, CORCON,
						; COEFFS_IN_DATA
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _FIRLMSNorm: FIR filtering with Normalized LMS coefficient adaptation.
;
; Operation:
;	y[n] = sum_{m=0:M-1}(h[m]*x[n-m]), 0 <= n < N.
;
;	h(n+1)[m] = h(n)[m] + nu*(r[n]-y[n])*x[n-m], 0 <= m < M.
; with
;	nu[n] = mu/(mu*E[n]),
; where
;	E[n] = E[n-1] + (x[n])^2 - (x[n-M+1])^2
; an estimate of the input signal energy at each sample.
;
; x[n] defined for 0 <= n < N,
; r[n] defined for 0 <= n < N,
; y[n] defined for 0 <= n < N,
; h[m] defined for 0 <= m < M as an increasing circular buffer,
; mu in [-1, 1).
; NOTE: delay defined for 0 <= m < M as an increasing circular buffer.
; NOTE: filter coefficients should not be allocated in program memory,
;	since in this case they cannot be adapted at run time.
;
; NOTE that the energy estimate may be also expressed as:
;
;	E[n] = x[n]^2 + x[n-1]^2 + ... + x[n-M+2]^2,
;
; then, to avoid saturation while computing the estimated energy,
; the input signal values should be bound so that
;
;	sum_{m=0:-M+2}((x[n+m])^2) < 1, 0 <= n < N.
;
; Input:
;	w0 = N, number of input samples (N)
;	w1 = y, ptr output samples (0 <= n < N)
;	w2 = x, ptr input samples (0 <= n < N)
;	w3 = filter structure (FIRStruct, h)
;	w4 = r, ptr reference samples (0 <= n < N)
;	w5 = mu.
;	w6-> E[-1] on start up, and E[N-1] upon return.
; Return:
;	w0 = y, ptr output samples (0 <= n < N)
;	NULL if it was detected that coefficients had been allocated in
;	program memory.
;
; System resources usage:
;	{w0..w7}	used, not restored
;	{w8..w13}	saved, used, restored
;	 AccuA		used, not restored
;	 AccuB		used, not restored
;	 CORCON		saved, used, restored
;	 MODCON		saved, used, restored
;	 XMODSRT	saved, used, restored
;	 XMODEND	saved, used, restored
;	 YMODSRT	saved, used, restored
;	 YMODEND	saved, used, restored
;
; DO and REPEAT instruction usage.
;	2 level DO intruction
;	1 level REPEAT intruction

;............................................................................

	.global	_FIRLMSNorm	; export
_FIRLMSNorm:

;............................................................................

	; Save working registers.
	push.d	w8				; {w8,w9} to TOS
	push.d	w10				; {w10,w11} to TOS
	push.d	w12				; {w12,w13} to TOS

.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif
;............................................................................

	; Prepare CORCON for fractional computation.
	push	CORCON
	fractsetup	w7

;............................................................................

	; Make sure that coefficients are not located in program memory.
	; (If they were, they could not be adapted at run time...)
	mov	[w3+oCoeffsPage],w10		; w10= coefficients page
	mov	#COEFFS_IN_DATA,w8		; w8 = COEFFS_IN_DATA
	cp	w8,w10				; w8 - w10
	bra	z,_noPSV			; if w10 = COEFFS_IN_DATA
						; no PSV management
						; else
	mov	#0,w0				; w0 = 0 (NULL)
	bra	_restore			; restore saved registers
						; and return NULL
_noPSV:

;............................................................................

	; Prepare core registers for modulo addressing.
	push	MODCON
	push	XMODSRT
	push	XMODEND
	push	YMODSRT
	push	YMODEND

;............................................................................

	; Setup registers for modulo addressing.
	mov	#0xC0A8,w10			; XWM = w8, YWM = w10
						; set XMODEND and YMODEND bits
	mov	w10,MODCON			; enable X,Y modulo addressing

	mov	[w3+oCoeffsEnd],w8		; w8 -> last byte of h[M-1]
	mov	w8,XMODEND			; init'ed to coeffs end address
	mov	[w3+oCoeffsBase],w8		; w8 -> h[0]
	mov	w8,XMODSRT			; init'ed to coeffs base address
						; (increasing buffer,
						;  2^n aligned)
	mov	[w3+oDelayEnd],w10		; w10-> last byte of d[M-1]
	mov	w10,YMODEND			; init'ed to delay end address
	mov	[w3+oDelayBase],w10		; w10 -> d[0]
	mov	w10,YMODSRT			; init'ed to delay base address
						; (increasing buffer,
						;  2^n aligned)

;............................................................................

	push	w1				; save return value (y)

;............................................................................

	; Perpare to filter all samples.
	mov	[w3+oDelay],w10			; w10 points at current delay
						; sample d[m], 0 <= m < M
						; referred to as delay[0]
						; for each iteration...
	mov	w4,w7				; w7-> r[0] (temporarily)
	mov	[w3+oNumCoeffs],w4		; w4 = M
	sub	w4,#2,w4			; W4 = M-2
	dec	w0,w0				; w0 = N-1
	mov	w7,w13				; w13->r[0] (permanently)
	mov	w5,w7				; w7 = mu (used as numerator)
	mov	w6,w11				; w11->E[-1]

;............................................................................

	; Perform filtering of all samples.
	do	w0,_endFilter		; {	; do (N-1)+1 times

	; Prepare to normalize.
	mov	[w2],w5				; w5 = x[n]
	lac	[w11],a				; a = E[n-1]
	mac	w5*w5,a				; a += (x[n])^2
	sac.r	a,[w11]				;*w11= E[n-1] + (x[n])^2

	; Prepare to filter sample.
	mov	[w2++],[w10]			; store new sample into delay

.ifdef YMEM_ERRATA
    nop
.endif

	clr	a,[w8]+=2,w5,[w10]+=2,w6	; a  = 0
						; w5 = h[0]
						; w8-> h[1]
						; w6 = delay[0]
						; w10->delay[1]

	; Filter each sample.
	; (Perform all but last MACs.)
	repeat	w4			; {	; do (M-2)+1 times
	mac	w5*w6,a,[w8]+=2,w5,[w10]+=2,w6	; a += h[m]*delay[m]
						; w5 = h[m+1]
						; w8-> h[m+2]
						; w6 = delay[m+1]
						; w10->delay[m+2]
; }
	; (Perform last MAC.)
	mac	w5*w6,a				; a += h[M-1]*delay[M-1]
						; now:
						; w6 = delay[M-1] = x[n-M+1]
						; w8-> h[0]
						; w10->delay[0]

	; Save filtered result.
	sac.r	a,[w1]				; y[n] =
						;   sum_{m=0:M-1}(h[m]*x[n-m])

	; Continue normalizing.
	lac	[w11],a				; a  = E[n-1] + (x[n])^2
	mpy	w6*w6,b				; b  = (x[n-M+1])^2
	sub	a				; a -= (x[n-M+1])^2
	sac.r	a,[w11]				;*w11= E[n] (estimate)
	add	w7,a				; a += mu
	sac.r	a,w6				; w6 = mu + E[n] (denominator)
	; Divide.
	push.d	w0				; {w0,w1} to TOS
	repeat	#17
	divf	w7,w6				; w0 = mu/(mu+E[n])
	mov	w0,w6				; w6 = nu[n]
	pop.d	w0				; {w0,w1} from TOS

	; With the new output, and the corresponding reference sample,
	; compute normalize factor.
	lac	[w1++],a			; a  = y[n]
						; w1-> y[n+1]
	lac	[w13++],b			; b  = r[n]
						; w13->r[n+1]
	sub	b				; b  = r[n] - y[n]
	sac.r	b,w5				; w5:  current error
	mpy	w5*w6,a				; a  = nu[n]*(r[n]-y[n])
	sac.r	a,w5				; w5:  attenuated error

	; Adaptation: h[m] = h[m] + attError*x[n-m].
	; Here the h[m] cannot be addressed as a circular buffer,
	; because their values are accessed via a 'LAC' instruction...
	; Thus, use w9 instead.

	; Prepare adaptation.
	mov	w8,w9				; w9-> h[0]
	clr	a,[w10]+=2,w6			; w6 = delay[0]
						; w10->delay[1]

	; Perform adaptation (all but last two coefficients).
	dec	w4,w12				; w12= M-3
	do	w12,_endAdapt		; {	; do (M-3)+1 times
	lac	[w9],a				; a  = h[m]
	mac	w5*w6,a,[w10]+=2,w6		; a += attError*delay[m]
						; w6 = delay[m+1]
						; w9-> delay[m+2]
_endAdapt:
	sac.r	a,[w9++]			; store adapted h[m]
						; w9-> h[m+1]
; }
	; Perform adaptation for second to last coefficient.
	lac	[w9],a				; a  = h[M-2]
	mac	w5*w6,a,[w10],w6		; a += attError*h[M-2]
						; w6 = delay[M-1]
						; w10->delay[M-1]
	sac.r	a,[w9++]			; store adapted h[M-2]
	; Perform adaptation for last coefficient.
	lac	[w9],a				; a  = h[M-1]
	mac	w5*w6,a				; a += attError*h[M-1]
_endFilter:
	sac.r	a,[w9]				; store adapted h[M-1]
; }

;............................................................................

	; Update delay pointer.
	mov	w10,[w3+oDelay]			; note that the delay pointer
						; may wrap several times around
						; d[m], 0 <= m < M, depending
						; on the value of N
						; (it is the same as delay[0])

;............................................................................

	pop	w0				; restore return value

;............................................................................

	; Restore core registers for modulo addressing.
	pop	YMODEND
	pop	YMODSRT
	pop	XMODEND
	pop	XMODSRT
	pop	MODCON

;............................................................................

_restore:

	; Restore CORCON.
	pop	CORCON

;............................................................................
.ifndef PSV_ON
	pop DSRPAG
.endif

	; Restore working registers.
	pop.d	w12				; {w12,w13} from TOS
	pop.d	w10				; {w10,w11} from TOS
	pop.d	w8				; {w8,w9} from TOS

;............................................................................

	return	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

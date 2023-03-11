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
						; CORCON,PSVPAG
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _FIRInterpolate: Ratio 1:R interpolation by FIR (low pass) filtering.
;
; Operation:
;	y[R*n] = H(x[n])
;
; x[n] defined for 0 <= n < N,
; y[n] defined for 0 <= n < N*R,
; h[k] defined for 0 <= k < M (M = q*R, 1 < q integer).
; d[k] defined for 0 <= k < M/R.
;
; Input:
;	w0 = N, number of input samples (N = p*R, 1 < p integer)
;	w1 = y, ptr output samples (0 <= n < N*R)
;	w2 = x, ptr input samples (0 <= n < N)
;	w3 = filter structure (FIRStruct, h)
;	w4 = R, rate of interpolation R (from 1)
; Return:
;	w0 = y, ptr output samples (0 <= n < N*R)
;
; System resources usage:
;	{w0..w7}	used, not restored
;	{w8..w14}	saved, used, restored
;	 AccuA		used, not restored
;	 CORCON		saved, used, restored
;	 PSVPAG		saved, used, restored (if coeffs in P memory)
;
; DO and REPEAT instruction usage.
;	2 level DO intruction
;	1 level REPEAT intruction


	.global	_FIRInterpolate	; export
_FIRInterpolate:

;............................................................................

	; Save working registers.
	push.d	w8		; {w8,w9} to TOS
	push.d	w10		; {w10.w11} to TOS
	push.d	w12		; {w12,w13} to TOS	
	push    w14             ; w14 to TOS

;............................................................................

	; Prepare CORCON for fractional computation.
	push	CORCON
	fractsetup	w7

;............................................................................

	; Prepare DSRPAG or PSVPAG for possible access of data
	; located in program memory
.ifndef PSV_ON
	push	DSRPAG
.else
	push    PSVPAG
.endif

	push	w1				; save return value (y)
	
	mov     w15,w14             ; save SP

;............................................................................
	mov	[w3+oCoeffsBase],w8		; w8 -> h[0]

	mov	[w3+oCoeffsPage],w10		; w10= coefficients page
	mov	#COEFFS_IN_DATA,w7		; w7 = COEFFS_IN_DATA
	cp	w7,w10				; w7 - w10
	bra	z,_noPSV			; if w10 = COEFFS_IN_DATA
							; no PSV management

.ifdef PSV_ON
	psvaccess	w7			; enable PSV bit in CORCON
	mov	w10,PSVPAG			; load PSVPAG with program
							; space page offset
.else
	; coefficients in flash
	movpag    w10,DSRPAG				; set DSRPAG to PSV page

	; check if sufficient space is available in stack
	; condition (SP + TABLE SIZE + STACK_GUARD) < SPLIM
	mov    SPLIM,w5             ; load stack pointer limit
	mov    _STACK_GUARD,w13     ; stack guard space for interrupts etc ...
	                            ; NOTE: THIS IS USER-DEFINABLE
    mov    #__YDATA_BASE,w4

	mov	   [w3+oNumCoeffs],w9	; number of coefficients
	sl     w9,w9                ; TABLE_SIZE in bytes
	
	add    w9,w15,w10           ; SP + TABLE_SIZE
	bra    c,_runFromFlash
	sub    w10,w4,w10           ; SP + TABLE_SIZE - __YDATA_BASE
	bra    geu,_runFromFlash
	
	add    w9,w13,w9            ; add _STACK_GUARD
	bra    c,_runFromFlash
	add    w9,w15,w9            ; add SP
	bra    c,_runFromFlash

	sub    w9,w5,w13            ; Check against SPLIM
	bra    ltu,_copyToStack

;...............................................................................	
; RUN FROM FLASH
;...............................................................................	
_runFromFlash:
	; not enough stack space, run from flash
	
;............................................................................

	; Get parameters from filter structure.
	mov	w4,w6				; w6 = R
	mov	[w3+oCoeffsBase],w4		; w4-> h[0]
	mov	[w3+oDelayBase],w5		; w5-> d[0]
	mov	[w3+oNumCoeffs],w3		; w3 = M

;............................................................................

	; Set up filtering.

	; Compute M/R = q:
	mov	#0x1,w8				; w8 = 1 (increment till q)
	mov	w6,w13				; w13= R (increment till M)
_divide2:
	cp	w3,w13				; M-R*p ?
	bra	z,_done2				; M-R*p = 0 => p = q (done)
	inc	w8,w8				; w8 = p
	add	w6,w13,w13			; w13= R*p
	bra	_divide2
_done2:
						; now:
						; w8 = M/R
						; w13 available for reuse
	sl	w6,w12				; w12= R*sizeof(sample)

	dec	w3,w3				; w3 = M-1
	dec	w6,w13				; w13= R-1
						; w6 available for reuse
	dec2	w8,w8				; w8 = M/R-2

	; Generate the N*R output samples from the N input samples.
_doInter2:
	dec	w0,w0				; w0 = N-i
	bra	lt,_doneInter2			; after N iterations: done...

	push DSRPAG
	movpag #0x0001, DSRPAG

	; Make room in delay for next input sample.
	mov	w5,w10				; w10->d[0]
	inc2	w5,w9				; w9-> d[1]
	repeat	w8			; {	; do (M/R-2)+1 times
	mov	[w9++],[w10++]			; d[k] <- d[k+1]
						; w9-> d[k+2]
						; w10->d[k+1]
; }
						; now:
						; w9-> d[M/R]
						; w10->d[M/R-1]

	; Place next input sample in delay.
	mov	[w2++],[w10]			; d[M/R-1] = x[n]
						; w2-> x[n+1]

	pop DSRPAG

	; Set up next R outputs.
	mov	w4,w11				; w11->h[0]
	mov	w10,w5				; w5-> d[M/R-1]

	; Generate next R outputs.
	do	w13,_endOutputs2		; {	; do (R-1)+1 times

	; Set up next output.
	mov	w11,w9				; w9-> h[k]
	mov	w5,w10				; w10->d[M/R-1]
	clr	a,[w9],w6,[w10]-=2,w7		; a  = 0
						; w7 = d[M/R-2] = x[n-1]
						; w10->d[M/R-3] = x[n-2]
						; w6 = h[k]

	; Generate next output.
	; (Perform all but last MAC.)
	do	w8,_endOutput2		; {	; do (M/R-2)+1 times

	nop					; NOP needed for PSV access at start of DO loop
	mac	w6*w7,a,[w9+w12],w6,[w10]-=2,w7	; a += h[R*k]*d[M/R-1-k]
						;    = h[R*k]*x[n-k]
						; w7 = d[M/R-1-k-1] = x[n-k-1]
						; w10->d[M/R-1-k-2] = x[n-k-2]
						; w6 = h[R*(k+1)]
_endOutput2:
	add	w9,w12,w9			; w9-> h[R*(k+1)]
; }
	; (Perform last MAC.)
	mac	w6*w7,a

	sac.r	a,[w1++]			; store y[R*n+k]
						; w1-> y[R*n+k+1]

	; Update for next sample.
_endOutputs2:
	inc2	w11,w11				; w11->h[k+1]
; }

	; Update for next R samples.
	inc	w8,w6				; w6 = M/R-1
	sl	w6,w11				; w11= (M/R-1)*sizeof(sample)
	sub	w5,w11,w5			; w5-> d[0]

	; Porcess next input sample.
	bra	_doInter2

_doneInter2:
						; now:
						; w1-> y[N*R]
						; w2-> x[N]

	bra    _completedFIRInterpolate

;...............................................................................
; STACK SPACE AVAILABLE
;...............................................................................
_copyToStack:	
	mov	   [w3+oNumCoeffs],w9			; number of coefficients

	; Save coefficient table pointer
	mov    w15,w7			; w7-> WN(0) (real part)
	; Copy entire coefficient table to stack
	dec    w9,w13           ; repeat count
	repeat w13              ; set repeat count
	mov    [w8++],[w15++]   ; copy from PSV to stack
	
	mov    w7,w8            ; restore w8

.endif

_noPSV:
.ifndef PSV_ON
      movpag #0x0001,DSRPAG
.endif

;............................................................................

	; Set up filtering.
	mov	w4,w6				; w6 = R
	mov	w8,w4		; w4-> h[0]
	mov	[w3+oDelayBase],w5		; w5-> d[0]
	mov	[w3+oNumCoeffs],w3		; w3 = M

	; Compute M/R = q:
	mov	#0x1,w8				; w8 = 1 (increment till q)
	mov	w6,w13				; w13= R (increment till M)
_divide:
	cp	w3,w13				; M-R*p ?
	bra	z,_done				; M-R*p = 0 => p = q (done)
	inc	w8,w8				; w8 = p
	add	w6,w13,w13			; w13= R*p
	bra	_divide
_done:
						; now:
						; w8 = M/R
						; w13 available for reuse
	sl	w6,w12				; w12= R*sizeof(sample)

	dec	w3,w3				; w3 = M-1
	dec	w6,w13				; w13= R-1
						; w6 available for reuse
	dec2	w8,w8				; w8 = M/R-2

	; Generate the N*R output samples from the N input samples.
_doInter:
	dec	w0,w0				; w0 = N-i
	bra	lt,_doneInter			; after N iterations: done...

	; Make room in delay for next input sample.
	mov	w5,w10				; w10->d[0]
	inc2	w5,w9				; w9-> d[1]
	repeat	w8			; {	; do (M/R-2)+1 times
	mov	[w9++],[w10++]			; d[k] <- d[k+1]
						; w9-> d[k+2]
						; w10->d[k+1]
; }
						; now:
						; w9-> d[M/R]
						; w10->d[M/R-1]

	; Place next input sample in delay.
	mov	[w2++],[w10]			; d[M/R-1] = x[n]
						; w2-> x[n+1]
	; Set up next R outputs.
	mov	w4,w11				; w11->h[0]
	mov	w10,w5				; w5-> d[M/R-1]

	; Generate next R outputs.
	do	w13,_endOutputs		; {	; do (R-1)+1 times

	; Set up next output.
	mov	w11,w9				; w9-> h[k]
	mov	w5,w10				; w10->d[M/R-1]
	clr	a,[w9],w6,[w10]-=2,w7		; a  = 0
						; w7 = d[M/R-2] = x[n-1]
						; w10->d[M/R-3] = x[n-2]
						; w6 = h[k]

	; Generate next output.
	; (Perform all but last MAC.)
	do	w8,_endOutput		; {	; do (M/R-2)+1 times
	mac	w6*w7,a,[w9+w12],w6,[w10]-=2,w7	; a += h[R*k]*d[M/R-1-k]
						;    = h[R*k]*x[n-k]
						; w7 = d[M/R-1-k-1] = x[n-k-1]
						; w10->d[M/R-1-k-2] = x[n-k-2]
						; w6 = h[R*(k+1)]
_endOutput:
	add	w9,w12,w9			; w9-> h[R*(k+1)]
; }
	; (Perform last MAC.)
	mac	w6*w7,a

	sac.r	a,[w1++]			; store y[R*n+k]
						; w1-> y[R*n+k+1]

	; Update for next sample.
_endOutputs:
	inc2	w11,w11				; w11->h[k+1]
; }

	; Update for next R samples.
	inc	w8,w6				; w6 = M/R-1
	sl	w6,w11				; w11= (M/R-1)*sizeof(sample)
	sub	w5,w11,w5			; w5-> d[0]

	; Porcess next input sample.
	bra	_doInter

_doneInter:
						; now:
						; w1-> y[N*R]
						; w2-> x[N]

;............................................................................

_completedFIRInterpolate:
	mov     w14,w15         ; restore SP
	pop	w0				; restore return value

;............................................................................

	; Restore DSRPAG/PSVPAG and CORCON.
.ifndef PSV_ON
	pop	DSRPAG
.else
	pop	PSVPAG
.endif
	pop	CORCON

;............................................................................

	; Restore working registers.
	pop		w14				; w14 from TOS
	pop.d	w12				; {w12,w13} from TOS
	pop.d	w10				; {w10,w11} from TOS
	pop.d	w8				; {w8,w9} from TOS

;............................................................................

	return	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

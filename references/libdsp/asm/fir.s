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
	.include	"dspcommon.inc"		; MODCON, XMODSRT, XMODEND,
						; YMODSRT, YMODEND, CORCON,
						; PSVPAG, COEFFS_IN_DATA,
						; FIRStruct
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _FIR: FIR block filtering.
;
; Operation:
;	y[n] = sum_(m=0:M-1){h[m]*x[n-m]}, 0 <= n < N.
;
; x[n] defined for 0 <= n < N,
; y[n] defined for 0 <= n < N,
; h[m] defined for 0 <= m < M as an increasing circular buffer,
; NOTE: delay defined for 0 <= m < M as an increasing circular buffer.
;
; Input:
;	w0 = number of samples to generate (numSamps, N)
;	w1 = ptr to output samples (dstSamps, y)
;	w2 = ptr to input samples (srcSamps, x)
;	w3 = filter structure (FIRStruct, h)
;
; Return:
;	w0 = ptr to output samples (dstSamps, y)
;
; System resources usage:
;	{w0..w7}	used, not restored
;	{w8..w14}	saved, used, restored
;	 AccuA		used, not restored
;	 CORCON		saved, used, restored
;	 PSVPAG		saved, used, restored (if coeffs in P memory)
;	 MODCON		saved, used, restored
;	 XMODSRT	saved, used, restored
;	 XMODEND	saved, used, restored
;	 YMODSRT	saved, used, restored
;	 YMODEND	saved, used, restored
;
; DO and REPEAT instruction usage.
;	1 DO instructions
;	1 REPEAT intructions


	.global	_FIR	; export
_FIR:

;............................................................................

	; Save working registers.
	push.d	w8		; {w8,w9} to TOS
	push.d	w10		; {w10.w11} to TOS
	push.d	w12		; {w12,w13} to TOS	
	push    w14             ; w14 to TOS
	
;............................................................................

	; Prepare CORCON for fractional computation.
	push	CORCON
	fractsetup	w8

;............................................................................

	; Prepare DSRPAG or PSVPAG for possible access of data
	; located in program memory
.ifndef PSV_ON
	push	DSRPAG
.else
	push    PSVPAG
.endif

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
	
	mov     w15,w14             ; save SP
	
;............................................................................

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
	mov    SPLIM,w4             ; load stack pointer limit
	mov    _STACK_GUARD,w13     ; stack guard space for interrupts etc ...
	                            ; NOTE: THIS IS USER-DEFINABLE
    mov    #__YDATA_BASE,w5

	mov	   [w3+oNumCoeffs],w9	; number of coefficients
	sl     w9,w9                ; TABLE_SIZE in bytes

	add    w9,w15,w10           ; SP + TABLE_SIZE
	bra    c,_runFromFlash
	sub    w10,w5,w10           ; SP + TABLE_SIZE - __YDATA_BASE
	bra    geu,_runFromFlash
	
	add    w9,w13,w9            ; add _STACK_GUARD
	bra    c,_runFromFlash
	add    w9,w15,w9            ; add SP
	bra    c,_runFromFlash

	sub    w9,w4,w13            ; Check against SPLIM
	bra    ltu,_copyToStack

;...............................................................................	
; RUN FROM FLASH
;...............................................................................	
_runFromFlash:
	; not enough stack space, run from flash

	; Prepare to filter.
	mov	[w3+oDelay],w10			; w10 points at current delay
						; sample d[m], 0 <= m < M
	mov	[w3+oNumCoeffs],w4		; w4 = M
	sub	w4,#3,w4			; W4 = M-3
	dec	w0,w0				; w0 = N-1

;............................................................................

	; Perform filtering of all samples.
	do	w0,_endFilter2		; {	; do (N-1)+1 times

	push DSRPAG
	movpag #0x0001,DSRPAG
	mov [W2++],[W10]
	pop DSRPAG

	clr	a,[w8]+=2,w5,[w10]+=2,w6	; a  = 0
						; w5 = h[0]
						; w8-> h[1]
						; w6 = d[current]
						; w10->d[next]

	; Filter each sample.
	; (Perform all but two last MACs.)
	repeat	w4			; {	; do (M-3)+1 times
	mac	w5*w6,a,[w8]+=2,w5,[w10]+=2,w6	; a += h[m]*d[current]
						; w5 = h[m+1]
						; w8-> h[m+2]
						; w6 = d[next]
						; w10->d[next+1]
; }
	nop                        ; NOP for errata workaround

	; (Perform second last MAC.)
	mac	w5*w6,a,[w8]+=2,w5,[w10],w6	; a += h[M-2]*d[current]
						; w5 = h[M-1]
						; w8-> h[0]
						; w6 = d[next]
						; w10->d[next]
	; (Perform last MAC.)
	mac	w5*w6,a				; a += h[M-1]*d[current]

_endFilter2:
	; Save filtered result.
	sac.r	a,[w1++]			; y[n] =
						;   sum_{m=0:M-1}(h[m]*x[n-m])
						; w1-> y[n+1]
; }

;............................................................................

	; Update delay pointer.
	mov	w10,[w3+oDelay]			; note that the delay pointer
						; may wrap several times around
						; d[m], 0 <= m < M, depending
						; on the value of N

	bra    _completedFIR

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
	dec w15,w15
	mov	w15,XMODEND			; re-initialize XMODEND to end address of RAM copy of coefficients
	mov	w8,XMODSRT			; re-initialize XMODSRT to end address of RAM copy of coefficients
.endif

_noPSV:
.ifndef PSV_ON
    movpag #0x0001,DSRPAG
.endif

;............................................................................

	; Prepare to filter.
	mov	[w3+oDelay],w10			; w10 points at current delay
						; sample d[m], 0 <= m < M
	mov	[w3+oNumCoeffs],w4		; w4 = M
	sub	w4,#3,w4			; W4 = M-3
	dec	w0,w0				; w0 = N-1

	; Perform filtering of all samples.
	do	w0,_endFilter		; {	; do (N-1)+1 times

	mov	[w2++],[w10]			; store new sample into delay

.ifdef YMEM_ERRATA
	nop
.endif

	clr	a,[w8]+=2,w5,[w10]+=2,w6	; a  = 0
						; w5 = h[0]
						; w8-> h[1]
						; w6 = d[current]
						; w10->d[next]

	; Filter each sample.
	; (Perform all but two last MACs.)
	repeat	w4			; {	; do (M-3)+1 times
	mac	w5*w6,a,[w8]+=2,w5,[w10]+=2,w6	; a += h[m]*d[current]
						; w5 = h[m+1]
						; w8-> h[m+2]
						; w6 = d[next]
						; w10->d[next+1]
; }
	; (Perform second last MAC.)
	mac	w5*w6,a,[w8]+=2,w5,[w10],w6	; a += h[M-2]*d[current]
						; w5 = h[M-1]
						; w8-> h[0]
						; w6 = d[next]
						; w10->d[next]
	; (Perform last MAC.)
	mac	w5*w6,a				; a += h[M-1]*d[current]

_endFilter:
	; Save filtered result.
	sac.r	a,[w1++]			; y[n] =
						;   sum_{m=0:M-1}(h[m]*x[n-m])
						; w1-> y[n+1]
; }

;............................................................................

	; Update delay pointer.
	mov	w10,[w3+oDelay]			; note that the delay pointer
						; may wrap several times around
						; d[m], 0 <= m < M, depending
						; on the value of N

;............................................................................

_completedFIR:
	mov     w14,w15         ; restore SP
	pop	w0				; restore return value

;............................................................................

	; Restore core registers for modulo addressing.
	pop	YMODEND
	pop	YMODSRT
	pop	XMODEND
	pop	XMODSRT
	pop	MODCON

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
	pop	w14				; w14 from TOS
	pop.d	w12				; {w12,w13} from TOS
	pop.d	w10				; {w10,w11} from TOS
	pop.d	w8				; {w8,w9} from TOS


;............................................................................

	return	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

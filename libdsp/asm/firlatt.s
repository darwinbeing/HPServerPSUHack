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
; _FIRLattice: FIR filtering with lattice implementation.
;
; Operation:
;	e_(0)[n] = e'_(0)[n] = x[n],
;	e_(m)[n] =  e_(m-1)[n] - k_(m-1)*e'_(m-1)[n-1],
;      e'_(m)[n] = -k_(m-1)*e_(m-1)[n] + e'_(m-1)[n-1],
; and	    y[n] = e_(M)[n];
;
; x[n] defined for 0 <= n < N,
; y[n] defined for 0 <= n < N,
; k[m] defined for 0 <= m < M, and
; e'_(m)[n] defined for 0 <= m < M, for -M <= n < N.
;
; Input:
;	w0 = N, number of input samples (N)
;	w1 = y, ptr output samples (0 <= n < N)
;	w2 = x, ptr input samples (0 <= n < N)
;	w3 = h, ptr filter structure (see included file)
; Return:
;	w0 = y, ptr output samples (0 <= n < N)
;
; System resources usage:
;	{w0..w7}	used, not restored
;	{w8..w14}	saved, used, restored
;	 AccuA		used, not restored
;	 AccuB		used, not restored
;	 CORCON		saved, used, restored
;	 PSVPAG		saved, used, restored (if coeffs in program memory)
;
; DO and REPEAT instruction usage.
;	2 level DO intruction
;	no REPEAT intructions


	.global	_FIRLattice	; export
_FIRLattice:

;............................................................................

	; Save working registers.
	push.d	w8		; {w8,w9} to TOS
	push.d	w10		; {w10,w11} to TOS
	push.d	w12		; {w12,w13} to TOS
	push    w14             ; w14 to TOS

;............................................................................

	; Prepare CORCON for fractional computation.
	push	CORCON
	fractsetup	w12

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
	mov	[w3+oCoeffsBase],w8		; w8-> k[0]

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
	sl     w9,w9               	; TABLE_SIZE in bytes
	
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

;............................................................................

	; Set up filter structure.
	mov	[w3+oNumCoeffs],w12		; w12= M
	mov	w8,w10				; w10->k[0] (for rewind)
	mov	[w3+oDelayBase],w9		; w9-> del[0]
	mov	w9,w11				; w11->del[0] (for rewind)

	; Set up filtering.
	dec	w0,w0				; w0 = N-1
	dec2	w12,w12				; w12 = M-2

	; Filter the N input samples.
	; Note that at this point, we have x-> x[0], and del[m] = e'_(m)[n-1].
	do	w0,_endFilter2		; {	; do (N-1)+1 times
						; w0 available for reuse

	; For m = 0 (recursion set up).
	push DSRPAG
	movpag #0x0001,DSRPAG
	lac	[w2++],a			; e_(0)[n] = x[n] (via accuA), w2-> x[n+1]
	
	sac.r	a,w7				; w7 = x[n] (tmp = e_(0)[n])
	mov	[w9],w6				; w6 = e'_(0)[n-1]
	sac.r	a,[w9++]			; e'_(0)[n] = x[n], w9-> del[m+1] (e'_(1)[n-1])
	pop DSRPAG
	
	; For 1 <= m < M (recursion proper).
	do	w12,_endRecurse2	; {	; do (M-2)+1 times
						; w12 available for reuse
	nop					; NOP needed for PSV access at start of DO loop

	; Compute recursive terms.
	mov	[w8++],w5			; w5 = k[m-1]
						; w8-> k[m]

	; Upper branch: e_(m)[n] = e_(m-1)[n] - k_(m-1)*e'_(m-1)[n-1].
	msc	w5*w6,a				; a  = e_(m)[n]

	; Lower branch: e'_(m)[n] = -k_(m-1)*e_(m-1)[n] + e'_(m-1)[n-1].
	lac	w6,b				; b  = e'_(m-1)[n-1]
	msc	w5*w7,b				; b  = e'_(m)[n]

	; Update.
	push DSRPAG
	movpag #0x0001,DSRPAG
	mov	[w9],w6				; w6 = e'_(m)[n-1]
	sac.r	b,[w9++]			; save e'_(m)[n], w9-> del[m+1] (e'_(m+1)[n-1])
	pop DSRPAG
						
_endRecurse2:
	sac.r	a,w7				; w7 = e_(m)[n]
; }

	; For m = M (generate output).
	; y[n] = e_(M)[n] = e_(M-1)[n] - k_(M-1)*e'_(M-1)[n-1].
	mov	[w8],w5				; w5 = k[M-1]
	msc	w5*w6,a				; a  = e_(M)[n]

	sac.r	a,[w1++]			; save y[n] = e_(M)[n]
						; w1-> y[n+1]

	; Rewind pointers.
	mov	w10,w8				; w8-> k[0]
_endFilter2:
	mov	w11,w9				; w9-> del[0]
; }

	bra    _completedFIRLattice

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

	; Set up filter structure.
	mov	[w3+oNumCoeffs],w12		; w12= M
	mov	w8,w10				; w10->k[0] (for rewind)
	mov	[w3+oDelayBase],w9		; w9-> del[0]
	mov	w9,w11				; w11->del[0] (for rewind)

	; Set up filtering.
	dec	w0,w0				; w0 = N-1
	dec2	w12,w12				; w12 = M-2

	; Filter the N input samples.
	; Note that at this point, we have x-> x[0], and del[m] = e'_(m)[n-1].
	do	w0,_endFilter		; {	; do (N-1)+1 times
						; w0 available for reuse

	; For m = 0 (recursion set up).
	lac	[w2++],a			; e_(0)[n] = x[n] (via accuA)
						; w2-> x[n+1]
	sac.r	a,w7				; w7 = x[n] (tmp = e_(0)[n])
	mov	[w9],w6				; w6 = e'_(0)[n-1]
	sac.r	a,[w9++]			; e'_(0)[n] = x[n]
						; w9-> del[m+1] (e'_(1)[n-1])

	; For 1 <= m < M (recursion proper).
	do	w12,_endRecurse		; {	; do (M-2)+1 times
						; w12 available for reuse
	; Compute recursive terms.
	mov	[w8++],w5			; w5 = k[m-1]
						; w8-> k[m]

	; Upper branch: e_(m)[n] = e_(m-1)[n] - k_(m-1)*e'_(m-1)[n-1].
	msc	w5*w6,a				; a  = e_(m)[n]

	; Lower branch: e'_(m)[n] = -k_(m-1)*e_(m-1)[n] + e'_(m-1)[n-1].
	lac	w6,b				; b  = e'_(m-1)[n-1]
	msc	w5*w7,b				; b  = e'_(m)[n]

	; Update.
	mov	[w9],w6				; w6 = e'_(m)[n-1]
	sac.r	b,[w9++]			; save e'_(m)[n]
						; w9-> del[m+1] (e'_(m+1)[n-1])
_endRecurse:
	sac.r	a,w7				; w7 = e_(m)[n]
; }

	; For m = M (generate output).
	; y[n] = e_(M)[n] = e_(M-1)[n] - k_(M-1)*e'_(M-1)[n-1].
	mov	[w8],w5				; w5 = k[M-1]
	msc	w5*w6,a				; a  = e_(M)[n]

	sac.r	a,[w1++]			; save y[n] = e_(M)[n]
						; w1-> y[n+1]

	; Rewind pointers.
	mov	w10,w8				; w8-> k[0]
_endFilter:
	mov	w11,w9				; w9-> del[0]
; }

;............................................................................

_completedFIRLattice:
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

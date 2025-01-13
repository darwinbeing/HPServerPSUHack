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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; This real valued DCT implementation expects that the input data is a
; real valued vector such that the absolute value of each of its elements
; is less than 0.5. If greater or equal to this value the results could
; produce saturation.
;
; The input vector must have been zero padded from its original length N
; to length 2*N, prior to invoking this operation; N must be an integer
; power of 2.
;
; Because the algorithm uses an IFFT operation, the user must provide as
; input a pointer to the N/2 complex twiddle factors to be used by the
; N-point IFFT; i.e., the complex conjugates of a regular set of twiddle
; factors. Besides, a pointer to the N/2 cosine-sine factors for the DCT
; must be input to this function.
;
; Also, the program performs an implicit scaling of 1/sqrt(2*N), with N
; the number of elements in the input vector prior to zero padding.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


	; Local inclusions.
	.nolist
	.include	"dspcommon.inc"		; fractsetup, kSinPiQ
						; CORCON,PSVPAG,COEFFS_IN_DATA
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _DCT: Real valued (in-place) Type II DCT.
;
; Operation:
;	X(k) = sqrt(2/N)*c[k]*sum_n {x[n]*CN[k(2*n+1)]},
; with
;	CN[m] = cos(m*pi/(2*N)),
; and
;	c[0] = 1/sqrt(2), c[k>0] = 1;
;
; n in {0, 1,... , N-1}, and
; k in {0, 1,... , N-1}, with N = 2^m.
;
; Input:
;	w0 = log2(N), N number of elements prior to zero padding (log2N)
;	w1 = ptr to (zero padded to length 2*N) source vector (srcV)
;	w2 = ptr to N/2 complex cosine-sine factors (cosineFactors)
;		 CN(k) = exp(i*k*pi/(2*N)), CN(0)...CN(N/2-1)
;	w3 = ptr to N/2 complex twiddle factors (twidFactors)
;		 WN(k) = exp(i*k*2*pi/N), WN(0)...WN(N/2-1)
;	w4 = COEFFS_IN_DATA, or memory program page with factors.
; Return:
;	w0 = ptr to source vector (srcV)
;
; System resources usage:
;	{w0..w7}	used, not restored
;	{w8..w14}	saved, used, restored
;	 AccuA		used, not restored
;	 CORCON		saved, used, restored
;	 PSVPAG		saved, used, restored (if factors in program memory)
; plus system resources from IFFTComplexIP.
;
; DO and REPEAT instruction usage.
;	1 level DO intruction
;	1 level REPEAT intruction


	; External symbols.
	.extern	_IFFTComplexIP

	.global	_DCTIP	; export
_DCTIP:

;............................................................................

	; Save working registers.
	push.d	w8				; {w8,w9} to TOS
	push.d	w10				; {w10,w11} to TOS
	push.d	w12				; {w12,w13} to TOS
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

	push	w1				    ; save return value (srcCV)
	
	mov     w15,w14             ; save SP

	mov	    #COEFFS_IN_DATA,w7	; w7 = COEFFS_IN_DATA
	cp	    w7,w4				; w7 - w4
	bra	    z,_noPSV            ; if w4 = COEFFS_IN_DATA

.ifdef PSV_ON
	psvaccess	w7              ; enable PSV bit in CORCON
	mov	w4,PSVPAG               ; load PSVPAG with program
	                            ; space page offset
.else
	; twiddles in flash
    movpag w4,DSRPAG            ; set DSRPAG to PSV page

	; check if sufficient space is available in stack
	; condition (SP + TABLE SIZE + STACK_GUARD) < SPLIM
	mov    SPLIM,w5             ; load stack pointer limit
	mov    _STACK_GUARD,w13     ; stack guard space for interrupts etc ...
	                            ; NOTE: THIS IS USER-DEFINABLE
    mov    #__YDATA_BASE,w4

	mov	   #0x1,w9				; to be shifted...
	sl	   w9,w0,w9			    ; w9 = N (1<<log2N)
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

	; Operation set up.
	mov	#0x1,w13			; to be shifted...
	dec	w0,w12				; w12= log2N-1
	sl	w13,w12,w12			; w12= N/2 (1<<(log2N-1))
	sl	w13,w0,w13			; w13= N (1<<log2N)
	sl	w13,w8				; w8 = N*sizeof(element)
	dec	w12,w11				; w11= N/2-1
	sl	w12,w12				; w12= N/2*sizeof(element)


	; Arrange even/odd elements of srcV to form a (complex) vector
	; with zero imaginary part as follows:
	; srcV[n] = srcV[2*n],
	; srcV[2*N-2-2*n] = srcV[2*n+1], for 0 <= n < N/2-1, and
	; srcV[2*n+1] = i*0, for 0 <= n < N-1.
	; (Result in place using zero padded area N <= n <2*N.)
	; In other words, push in the indicated order all odd elements
	; to zero padded area, and zero out imaginary parts:
	inc2	w1,w10				; w10->srcV[1] (low odd)
	add	w1,w8,w9			; w9-> srcV[N]
	add	w9,w8,w9			; w9-> srcV[2*N]
	
	push DSRPAG
	movpag #0x0001, DSRPAG
    nop

	clr	a,[w10],w7			; a  = 0
						; w7 = srcV[1] (get first odd)
	dec2	w9,w9				; w9-> srcV[2*N-1] (last odd)
	do	w11,_endArrangePSV	; {	; do (N/2-1)+1 times
	sac	a,[w10++]			; srcV[2*n+1] = 0 (zero odd)
						; w10->srcV[2*n+2] (low even)
	inc2	w10,w10				; leave even alone...
						; w10-> srcV[2*n+3] (low odd)
	sac	a,[w9--]			; srcV[2*N-1-2*n] = 0 (zero odd)
						; w9-> srcV[2*N-2-2*n]
						; (high even)
	mov	w7,[w9--]			; srcV[2*N-2-2*n] = srcV[2*n+1]
						; w9-> srcV[2*N-3+2*n+3]
						; (high odd)
_endArrangePSV:
	mov	[w10],w7			; w7 = srcV[2*n+3] (get odd)
; }

	pop DSRPAG

	; Apply complex IFFT to reorganized vector.
	push	w8				; {w8} to TOS
	push	w11				; {w11} to TOS
	push.d	w12				; {w12,w13} to TOS
	push	w1				; save pointer to srcV
	push	w2				; save pointer to cosFactors
	mov	w3,w2				; w2-> twidFactors
	mov	w4,w3				; w3 = factPage
	call _IFFTComplexIP
	pop	w2				; restore pointer to cosFactors
	pop	w1				; restore pointer to srcV
	pop.d	w12				; {w12,w13} from TOS
	pop	w11				; {w11} from TOS
	pop	w8				; {w8} from TOS

	; Modulate: Let cVect be the complex output of the IFFT; i.e.,
	;	cVect[n] = srcV[2*n] + i*srcV[2*n+1], 0 <= n < N.
	; Then, modulation consists of:	
	;	cVect[n] = exp(i*n*theta)*cVect[n], 0 <= n < N/2,
	; with theta = PI/(2*N).
	; Since	
	;	srcV[0] = real(cVect[0]) = cVect[0],
	;	srcV[N] = real(cVect[N/2]) = cVect[N/2], and for 1 <= n < N/2
	; 	srcV[2*n] = real(cVect[n]) and srcV[2*n+1] = imag(cVect[n]),
	; then modulation is equivalently applied as follows:
	;	srcV[0] = cos(0*theta)*srcV[0] = srcV[0], (trivial; do nothing)
	;	srcV[N] = cos((N/2)*theta)*srcV[N]+i*sin((N/2)*theta)*srcV[N],
	; (but only imaginary part needed!),
	; and for 1 <= n < N/2
	;	srcV[2*n] = cos(n*theta)*srcV[2*n] - sin(n*theta)*srcV[2*n+1],
	; srcV[2*n+1] = cos(n*theta)*srcV[2*n+1] + sin(n*theta)*srcV[2*n].
	; NOTE that
	;	cosFactors[n] = cos(n*theta), and
	;	cosFactors[n+1] = sin(n*theta).
	; Do modulation: */


	dec	w11,w11				; w11= N/2-2
	add	w1,#0x4,w10			; w10->srcV[2]
	add	w2,#0x4,w9			; w9-> cosFact[1] (real)
	mov	[w9++],w6			; w6 = cos(1*theta)
						; w9-> cosFact[2*n+1] (imag)

	push DSRPAG
	movpag #0x0001, DSRPAG
	nop
	
	mov	[w10++],w4			; w4 = srcV[2] (real)
						; w10->srcV[3] (imag)

	pop DSRPAG
	
	do	w11,_endModulatePSV	; {	; do (N/2-2)+1 times

    nop
	
	; Compute real part.
	mpy	w4*w6,a,[w9]+=2,w7,[w10]-=2,w5	; a  = cos*real
						; w7 = cosFact[2*n+1] (imag)
						; w9-> cosFact[2*n+2] (real)
						; w5 = srcV[2*n+1] (imag)
						; w10->srcV[2*n] (real)
	msc	w5*w7,a				; a  = cos*real-sin*imag
	sac.r	a,[w10++]			; save real
						; w10->srcV[2*n+1] (imag)

						; Compute imaginary part.
	mpy	w5*w6,a,[w9]+=2,w6		; a  = cos*imag
						; w6 = cosFact[2*n+2] (real)
						; w9-> cosFact[2*n+3] (imag)
	mac	w4*w7,a				; a  = cos*imag+sin*real
	sac.r	a,[w10++]			; save imag
						; w10->srcV[2*n+2] (real)

	; Fetch next multiplicand w4...
	push DSRPAG
	movpag #0x0001, DSRPAG
	nop
	
	mov	[w10++],w4			; w4 = srcV[2*n+2] (real)
						; w10->srcV[2*n+3] (imag)
						; (pipeline stall here)
_endModulatePSV:
	pop DSRPAG
	
; }
   						; now w4 = srcV[N] (real)
   						; now w10->srcV[N+1] (imag)
	; Now for N+1 (only imaginary part needed).
	mov	#kSinPiQ,w7			; w7 = sin(pi/4)

	push DSRPAG
	movpag #0x0001, DSRPAG
	nop
	
	mpy	w4*w7,a,[w10]-=2,w5		; a  = sin*real
						; w5 (trash)
						; w10->srcV[N] (real)
	sac.r	a,[w10--]			; save imag (on real...)
						; w10->srcV[N-1] (imag)

	; Reorganize into transform.
	; First, imaginary parts to upper half of vector (zero padd extension).
	add	w10,#0x4,w9			; w9-> srcV[N+1]
	do	w11,_endUpperPSV	; {	; do (N/2-2)+1 times
	mov	[w10--],[w9++]			; srcV[N+1+n] = srcV[N-1-2*n]
						; w10->srcV[N-1-2*n-1]
						; w9-> srcV[N+1+n+1]
_endUpperPSV:
	dec2	w10,w10				; w10->srcV[N-1-2*n-2]
						; (pipeline stall here)
; }

	; Second, compact real parts.
	add	w1,#0x4,w10			; w10->srcV[2]
	dec2	w10,w9				; w9-> srcV[1]
	do	w11,_endCompactPSV		; {	; do (N/2-2)+1 times
	mov	[w10++],[w9++]			; srcV[1+n] = srcV[2+2*n]
						; w10->srcV[2+2*n+1]
						; w9-> srcV[1+n+1]
_endCompactPSV:
	inc2	w10,w10				; w10->srcV[2+2*n+2]
						; (pipeline stall here)
; }


	; Third, bring imaginary parts back to lower half of vector.
	inc	w11,w11				; w11 = N/2-1
	add	w1,w8,w10			; w10->srcV[N]
	add	w1,w12,w9			; w9-> srcV[N/2]
	repeat	w11				; do (N/2-1)+1 times
	mov	[w10++],[w9++]			; srcV[N/2+n] = srcV[N/2+n]
						; w10->srcV[N/2+n+1]
						; w9-> srcV[N/2+n+1]

	; To avoid computation of square root, elements are not scaled.
	; The elements should be scaled by the factor sqrt(2*N).
	; First element must be scaled also by 1/sqrt(2) constant.
	; Since scaling by 1/sqrt(2) constant does not require square
	; root computation, the scaling of the first element is performed.
	mov	[w1],w4				; w4 = srcV[0]
	mov	#kInvSqrt2,w5			; w5 = 1/sqrt(2)
	mpy	w4*w5,a				; a  = srcV[0]/sqrt(2)
	sac.r	a,[w1]				; save into srcV[0]

;............................................................................

	pop DSRPAG

	bra    _completedDCT

;...............................................................................
; STACK SPACE AVAILABLE
;...............................................................................
_copyToStack:	
	mov	   #0x1,w9			; to be shifted...
	sl	   w9,w0,w9			; w9 = N (1<<log2N)

	; Save twiddle table pointer
	mov    w15,w8			; w8-> WN(0) (real part)
	; Copy entire twiddle table to stack
	dec    w9,w13           ; repeat count
	repeat w13              ; set repeat count
	mov    [w2++],[w15++]   ; copy from PSV to stack
	
	mov    w8,w2            ; restore w2
	
.endif

_noPSV:
.ifndef PSV_ON
    movpag #0x0001, DSRPAG
.endif
	
;............................................................................

; RAM version here

	; Operation set up.
	mov	#0x1,w13			; to be shifted...
	dec	w0,w12				; w12= log2N-1
	sl	w13,w12,w12			; w12= N/2 (1<<(log2N-1))
	sl	w13,w0,w13			; w13= N (1<<log2N)
	sl	w13,w8				; w8 = N*sizeof(element)
	dec	w12,w11				; w11= N/2-1
	sl	w12,w12				; w12= N/2*sizeof(element)


	; Arrange even/odd elements of srcV to form a (complex) vector
	; with zero imaginary part as follows:
	; srcV[n] = srcV[2*n],
	; srcV[2*N-2-2*n] = srcV[2*n+1], for 0 <= n < N/2-1, and
	; srcV[2*n+1] = i*0, for 0 <= n < N-1.
	; (Result in place using zero padded area N <= n <2*N.)
	; In other words, push in the indicated order all odd elements
	; to zero padded area, and zero out imaginary parts:
	inc2	w1,w10				; w10->srcV[1] (low odd)
	add	w1,w8,w9			; w9-> srcV[N]
	add	w9,w8,w9			; w9-> srcV[2*N]
	
	clr	a,[w10],w7			; a  = 0
						; w7 = srcV[1] (get first odd)
	dec2	w9,w9				; w9-> srcV[2*N-1] (last odd)
	do	w11,_endArrange		; {	; do (N/2-1)+1 times
	sac	a,[w10++]			; srcV[2*n+1] = 0 (zero odd)
						; w10->srcV[2*n+2] (low even)
	inc2	w10,w10				; leave even alone...
						; w10-> srcV[2*n+3] (low odd)
	sac	a,[w9--]			; srcV[2*N-1-2*n] = 0 (zero odd)
						; w9-> srcV[2*N-2-2*n]
						; (high even)
	mov	w7,[w9--]			; srcV[2*N-2-2*n] = srcV[2*n+1]
						; w9-> srcV[2*N-3+2*n+3]
						; (high odd)
_endArrange:
	mov	[w10],w7			; w7 = srcV[2*n+3] (get odd)
; }

	; Apply complex IFFT to reorganized vector.
	push	w8				; {w8} to TOS
	push	w11				; {w11} to TOS
	push.d	w12				; {w12,w13} to TOS
	push	w1				; save pointer to srcV
	push	w2				; save pointer to cosFactors
	mov	w3,w2				; w2-> twidFactors
	mov	w4,w3				; w3 = factPage
	call _IFFTComplexIP
	pop	w2				; restore pointer to cosFactors
	pop	w1				; restore pointer to srcV
	pop.d	w12				; {w12,w13} from TOS
	pop	w11				; {w11} from TOS
	pop	w8				; {w8} from TOS

	; Modulate: Let cVect be the complex output of the IFFT; i.e.,
	;	cVect[n] = srcV[2*n] + i*srcV[2*n+1], 0 <= n < N.
	; Then, modulation consists of:	
	;	cVect[n] = exp(i*n*theta)*cVect[n], 0 <= n < N/2,
	; with theta = PI/(2*N).
	; Since	
	;	srcV[0] = real(cVect[0]) = cVect[0],
	;	srcV[N] = real(cVect[N/2]) = cVect[N/2], and for 1 <= n < N/2
	; 	srcV[2*n] = real(cVect[n]) and srcV[2*n+1] = imag(cVect[n]),
	; then modulation is equivalently applied as follows:
	;	srcV[0] = cos(0*theta)*srcV[0] = srcV[0], (trivial; do nothing)
	;	srcV[N] = cos((N/2)*theta)*srcV[N]+i*sin((N/2)*theta)*srcV[N],
	; (but only imaginary part needed!),
	; and for 1 <= n < N/2
	;	srcV[2*n] = cos(n*theta)*srcV[2*n] - sin(n*theta)*srcV[2*n+1],
	; srcV[2*n+1] = cos(n*theta)*srcV[2*n+1] + sin(n*theta)*srcV[2*n].
	; NOTE that
	;	cosFactors[n] = cos(n*theta), and
	;	cosFactors[n+1] = sin(n*theta).
	; Do modulation: */


	dec	w11,w11				; w11= N/2-2
	add	w1,#0x4,w10			; w10->srcV[2]
	add	w2,#0x4,w9			; w9-> cosFact[1] (real)
	mov	[w9++],w6			; w6 = cos(1*theta)
						; w9-> cosFact[2*n+1] (imag)

	mov	[w10++],w4			; w4 = srcV[2] (real)
						; w10->srcV[3] (imag)

	do	w11,_endModulate	; {	; do (N/2-2)+1 times

	; Compute real part.
	mpy	w4*w6,a,[w9]+=2,w7,[w10]-=2,w5	; a  = cos*real
						; w7 = cosFact[2*n+1] (imag)
						; w9-> cosFact[2*n+2] (real)
						; w5 = srcV[2*n+1] (imag)
						; w10->srcV[2*n] (real)
	msc	w5*w7,a				; a  = cos*real-sin*imag
	sac.r	a,[w10++]			; save real
						; w10->srcV[2*n+1] (imag)

						; Compute imaginary part.
	mpy	w5*w6,a,[w9]+=2,w6		; a  = cos*imag
						; w6 = cosFact[2*n+2] (real)
						; w9-> cosFact[2*n+3] (imag)
	mac	w4*w7,a				; a  = cos*imag+sin*real
	sac.r	a,[w10++]			; save imag
						; w10->srcV[2*n+2] (real)

_endModulate:
	; Fetch next multiplicand w4...
	mov	[w10++],w4			; w4 = srcV[2*n+2] (real)
						; w10->srcV[2*n+3] (imag)
						; (pipeline stall here)
	
; }
   						; now w4 = srcV[N] (real)
   						; now w10->srcV[N+1] (imag)
	; Now for N+1 (only imaginary part needed).
	mov	#kSinPiQ,w7			; w7 = sin(pi/4)

	mpy	w4*w7,a,[w10]-=2,w5		; a  = sin*real
						; w5 (trash)
						; w10->srcV[N] (real)
	sac.r	a,[w10--]			; save imag (on real...)
						; w10->srcV[N-1] (imag)

	; Reorganize into transform.
	; First, imaginary parts to upper half of vector (zero padd extension).
	add	w10,#0x4,w9			; w9-> srcV[N+1]
	do	w11,_endUpper		; {	; do (N/2-2)+1 times
	mov	[w10--],[w9++]			; srcV[N+1+n] = srcV[N-1-2*n]
						; w10->srcV[N-1-2*n-1]
						; w9-> srcV[N+1+n+1]
_endUpper:
	dec2	w10,w10				; w10->srcV[N-1-2*n-2]
						; (pipeline stall here)
; }

	; Second, compact real parts.
	add	w1,#0x4,w10			; w10->srcV[2]
	dec2	w10,w9				; w9-> srcV[1]
	do	w11,_endCompact		; {	; do (N/2-2)+1 times
	mov	[w10++],[w9++]			; srcV[1+n] = srcV[2+2*n]
						; w10->srcV[2+2*n+1]
						; w9-> srcV[1+n+1]
_endCompact:
	inc2	w10,w10				; w10->srcV[2+2*n+2]
						; (pipeline stall here)
; }


	; Third, bring imaginary parts back to lower half of vector.
	inc	w11,w11				; w11 = N/2-1
	add	w1,w8,w10			; w10->srcV[N]
	add	w1,w12,w9			; w9-> srcV[N/2]
	repeat	w11				; do (N/2-1)+1 times
	mov	[w10++],[w9++]			; srcV[N/2+n] = srcV[N/2+n]
						; w10->srcV[N/2+n+1]
						; w9-> srcV[N/2+n+1]

	; To avoid computation of square root, elements are not scaled.
	; The elements should be scaled by the factor sqrt(2*N).
	; First element must be scaled also by 1/sqrt(2) constant.
	; Since scaling by 1/sqrt(2) constant does not require square
	; root computation, the scaling of the first element is performed.
	mov	[w1],w4				; w4 = srcV[0]
	mov	#kInvSqrt2,w5			; w5 = 1/sqrt(2)
	mpy	w4*w5,a				; a  = srcV[0]/sqrt(2)
	sac.r	a,[w1]				; save into srcV[0]

;............................................................................
_completedDCT:
	mov     w14,w15         ; restore SP
	pop	    w0              ; restore return value

;............................................................................

	; Restore PSVPAG and CORCON.
.ifndef PSV_ON
	pop	DSRPAG
.else
	pop	PSVPAG
.endif
	pop	CORCON

;............................................................................

	; Restore working registers.
	pop     w14             ; w14 from TOS
	pop.d	w12				; {w12,w13} from TOS
	pop.d	w10				; {w10,w11} from TOS
	pop.d	w8				; {w8,w9} from TOS


;............................................................................

	return	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

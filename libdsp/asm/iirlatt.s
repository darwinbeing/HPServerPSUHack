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
	.include	"dspcommon.inc"		; fractsetup, CORCON
						; IIR Lattice structure
						; PSVPAG, COEFFS_IN_DATA,
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _IIRLattice: IIR filtering with lattice implementation.
;
; Operation for an Mth order filter:
;## Filter samples.
;for n = 1:N
;
;   ## Get new sample.
;   current = x(n);
;
;   ## Lattice structure.
;   for m = 1:M
;      after     = current  - k(M+1-m) * d(m+1);
;      d(m)      = d(m+1) + k(M+1-m) * after;
;      current   = after;
;   end
;   d(M+1) = after;
;
;   ## Ladder structure (computes output).
;   if (g == 0)
;      y(n) = d(M+1);
;   else
;      for m = 1:M+1
;         y(n) = y(n) + g(M+2-m)*d(m);
;      endfor
;   endif
;
;endif
;
; x[n] defined for 0 <= n < N,
; y[n] defined for 0 <= n < N,
; k[m] defined for 0 <= m < M,
; g[m] defined for 0 <= m <= M, and
; d[m] defined for 0 <= m <= M,
; 0 <= n < N.
;
; NOTE: to avoid saturation in the computation of the lattice structure, the
;	input signal may need to be appropriately scaled.
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
;	 PSVPAG		saved, used, restored (if coeffs in P memory)
;
; DO and REPEAT instruction usage.
;	2 level DO intruction
;	1 REPEAT intruction

;............................................................................

	.global	_IIRLattice	; export
_IIRLattice:

;............................................................................

	; Save working registers.
	push.d	w8				; {w8,w9} to TOS
	push.d	w10				; {w10,w11} to TOS
	push.d	w12				; {w12,w13} to TOS
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

	push	w1				; save return value (y)
	
	mov     w15,w14             ; save SP
	
;............................................................................

	mov	[w3+oLattCoeffsPage],w10		; w10= coefficients page
	mov	#COEFFS_IN_DATA,w8		; w7 = COEFFS_IN_DATA
	cp	w8,w10				; w8 - w10
	bra	z,_noPSV			; if w10 = COEFFS_IN_DATA
							; no PSV management
.ifdef PSV_ON
	psvaccess	w8			; enable PSV bit in CORCON
	mov	w10,PSVPAG			; load PSVPAG with program
							; space page offset
.else
	; coefficients in flash
	movpag    w10,DSRPAG				; set DSRPAG to PSV page

	; check if sufficient space is available in stack
	; condition (SP + TABLE SIZE + STACK_GUARD) < SPLIM
	mov    SPLIM,w4             ; load stack pointer limit
;	mov    _STACK_GUARD,w13     ; stack guard space for interrupts etc ...
	                            ; NOTE: THIS IS USER-DEFINABLE
;   NOTE: Since there are very few coefficients, we remove the _STACK_GUARD
;   requirement by forcing it to 0.
	mov    #0x0,w13
    mov    #__YDATA_BASE,w5

	mov	   [w3+oOrder],w9	    ; order of lattice
	mul.uu w9,#2,w6             ; number of coefficients in [w7:w6]
	sl     w6,w9               	; TABLE_SIZE in bytes
	add    w9,#10,w9            ; Add IIR filter structure size
	
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

	push DSRPAG
	movpag #0x0001, DSRPAG
	nop

	; Set up filter structure.
	mov	[w3+oOrder],w13			; w13= M
	mov	[w3+oLattDelay],w11		; w11->del[0]
	dec	w13,w13				; w13= M-1
	sl	w13,w12				; w12= (M-1)*sizeof(sample)
	mov	[w3+oKappaVals],w8		; w8-> k[0]
	
	pop DSRPAG

	add	w12,w8,w8			; w8-> k[M-1]
	mov	w8,w9				; w9-> k[M-1] (for rewind)
	dec2	w13,w13				; w13= M-3
	inc	w13,w12				; w12= M-2

	; Set up filtering.
	dec	w0,w0				; w0 = N-1

	; Filter the N input samples.
	do	w0,_endFilterPSV    ; {	; do (N-1)+1 times
						    ; w0 available for later use
;	nop
;	nop
; .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . .

	; Lattice structure.
	inc2	w11,w10				; w10->del[1]
	clr	a,[w8]-=2,w4,[w10]+=2,w5	; a  = 0 (for double fetch)
						; w4 = k[M-1]
						; w8-> k[M-2]
						; w5 = del[1]
						; w10->del[2]

	; Get new input sample.
	push DSRPAG
	movpag #0x0001, DSRPAG
	nop

	lac	[w2++],a			; a  = x[n] (current)
						; w2-> x[n+1]
	pop DSRPAG

	; All but last two iteration...
	do	w13,_endLatticePSV		; {	; do (M-3)+1 times

;	nop
;	nop
	
	; Upper branch: after = current - k[M-1-m]*del[m+1].
	msc	w4*w5,a				; a -= k[M-1-m]*del[m+1]
						; a  = after
	sac.r	a,w6				; w6 = after

	; Lower branch: del[m] = del[m+1] + k[M-1-m]*after.
	lac	w5,a				; a  = del[m+1]

	mac	w4*w6,a,[w8]-=2,w4,[w10]+=2,w5	; a += k[M-1-m]*after
						; a  = del[m]
						; w4 = k[M-1-m-1]
						; w8-> k[M-1-m-2]
						; w5 = del[m+1+1]
						; w10->del[m+1+2]

	sac.r	a,[w11++]			; del[m] (updated)
						; w11->del[m+1]
_endLatticePSV:
	lac	w6,a				; a  = after
						; a  = current (next)
; }

	; One before last iteration...
	; Upper branch: after = current - k[1]*del[M-2+1].
	msc	w4*w5,a				; a -= k[1]*del[M-2+1]
						; a  = after
	sac.r	a,w6				; w6 = after

	; Lower branch: del[M-2] = del[M-2+1] + k[1]*after.
	lac	w5,a				; a  = del[M-2+1]

	mac	w4*w6,a,[w8],w4,[w10],w5	; a += k[1]*after
						; a  = del[M-2]
						; w4 = k[0]
						; w5 = del[M-1]

	sac.r	a,[w11++]			; del[M-2] (updated)
						; w11->del[M-1]
	lac	w6,a				; a  = after
						; a  = current (last)

	; Last iteration...
	; Upper branch: after = current - k[0]*del[M-1+1].
	msc	w4*w5,a				; a -= k[0]*del[M-1+1]
						; a  = after
	sac.r	a,w6				; w6 = after

	; Lower branch: del[M-1] = del[M-1+1] + k[0]*after.
	lac	w5,a				; a  = del[M-1+1]

	mac	w4*w6,a				; a += k[0]*after
						; a  = del[M-1]

	sac.r	a,[w11++]			; del[M-1] (updated)
						; w11->del[M]

	; Update last delay.
	mov	w6,[w11]			; del[M] = after

; .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . .

	; Ladder structure.
	push DSRPAG
	movpag #0x0001, DSRPAG
	nop
	mov	[w3+oGammaVals],w8		; w8-> g[0]
	pop DSRPAG

	; Only for zero-pole implementations,
	; but not for all-pole implementations...
	cp0	w8				; w8 == NULL ?
	bra	z,_allPolePSV	; Yes => all pole
						; No  => zero-pole
	clr	a,[w8]+=2,w4,[w11]-=2,w5	; a  = 0
						; w4 = g[0]
						; w8-> g[1]
						; w5 = del[M]
						; w10->del[M-1]
	
	; All but last two iterations...
	repeat	w12			; {	; do (M-2)+1 times
	mac	w4*w5,a,[w8]+=2,w4,[w11]-=2,w5	; accumulate
						; w4 = g[m+1]
						; w8-> g[m+2]
						; w5 = del[M-m-1]
						; w10->del[M-m-2]
; }

	nop                        ; NOP for errata workaround

	; One before last iteration...
	mac	w4*w5,a,[w8],w4,[w11],w5	; accumulate
						; w4 = g[M]
						; w5 = del[0]

	; Last iteration...
	mac	w4*w5,a				; accumulate
	bra	_storeOutputPSV

_allPolePSV:
	lac	w6,a				; a  = after
	inc2	w12,w8				; w8 = M
	sl	w8,w8				; w8 = (M)*sizeof(sample)
	sub	w11,w8,w11			; w11->del[0]

_storeOutputPSV:
	; Store output.
	sac.r	a,[w1++]			; store y[n]
						; w1-> y[n+1]

_endFilterPSV:
	; Rewind pointer.
	mov	w9,w8				; w8-> k[M-1]
; }


    bra _completedIIR
	
_copyToStack:

	; Save coefficient table pointer
	mov    w15,w12
	; Copy entire coefficient table to stack
	mov	   [w3+oOrder],w6	    ; order of lattice
	inc    w6,w6
	mov	   [w3+oKappaVals],w8	; w8 = base address of coeffs
	dec    w6,w13           ; repeat count
	repeat w13              ; set repeat count
	mov    [w8++],[w15++]   ; copy from PSV to stack

	mov	   [w3+oGammaVals],w8	; w8 = base address of coeffs
	dec    w6,w13           ; repeat count
	repeat w13              ; set repeat count
	mov    [w8++],[w15++]   ; copy from PSV to stack

	; Copy IIR filter structure to stack
    repeat #4
    mov    [w3++],[w15++]	;
	sub    w15,#10,w3
	inc2   w3,w3
	mov    w12,[w3++]	    ; w3 = base address of coeffs in stack
	add    w12,w6,w12
	add    w12,w6,w12
	mov    w12,[w3--]
	dec2   w3,w3
	
	
.endif

_noPSV:
.ifndef PSV_ON
    movpag #0x0001,DSRPAG
.endif

	; Set up filter structure.
	mov	[w3+oOrder],w13			; w13= M
	mov	[w3+oLattDelay],w11		; w11->del[0]
	dec	w13,w13				; w13= M-1
	sl	w13,w12				; w12= (M-1)*sizeof(sample)
	mov	[w3+oKappaVals],w8		; w8-> k[0]

	add	w12,w8,w8			; w8-> k[M-1]
	mov	w8,w9				; w9-> k[M-1] (for rewind)
	dec2	w13,w13				; w13= M-3
	inc	w13,w12				; w12= M-2

	; Set up filtering.
	dec	w0,w0				; w0 = N-1

	; Filter the N input samples.
	do	w0,_endFilter		; {	; do (N-1)+1 times
						; w0 available for later use
; .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . .

	; Lattice structure.
	inc2	w11,w10				; w10->del[1]
	clr	a,[w8]-=2,w4,[w10]+=2,w5	; a  = 0 (for double fetch)
						; w4 = k[M-1]
						; w8-> k[M-2]
						; w5 = del[1]
						; w10->del[2]

	; Get new input sample.
	lac	[w2++],a			; a  = x[n] (current)
						; w2-> x[n+1]

	; All but last two iteration...
	do	w13,_endLattice		; {	; do (M-3)+1 times

	; Upper branch: after = current - k[M-1-m]*del[m+1].
	msc	w4*w5,a				; a -= k[M-1-m]*del[m+1]
						; a  = after
	sac.r	a,w6				; w6 = after

	; Lower branch: del[m] = del[m+1] + k[M-1-m]*after.
	lac	w5,a				; a  = del[m+1]

	mac	w4*w6,a,[w8]-=2,w4,[w10]+=2,w5	; a += k[M-1-m]*after
						; a  = del[m]
						; w4 = k[M-1-m-1]
						; w8-> k[M-1-m-2]
						; w5 = del[m+1+1]
						; w10->del[m+1+2]

	sac.r	a,[w11++]			; del[m] (updated)
						; w11->del[m+1]
_endLattice:
	lac	w6,a				; a  = after
						; a  = current (next)
; }

	; One before last iteration...
	; Upper branch: after = current - k[1]*del[M-2+1].
	msc	w4*w5,a				; a -= k[1]*del[M-2+1]
						; a  = after
	sac.r	a,w6				; w6 = after

	; Lower branch: del[M-2] = del[M-2+1] + k[1]*after.
	lac	w5,a				; a  = del[M-2+1]

	mac	w4*w6,a,[w8],w4,[w10],w5	; a += k[1]*after
						; a  = del[M-2]
						; w4 = k[0]
						; w5 = del[M-1]

	sac.r	a,[w11++]			; del[M-2] (updated)
						; w11->del[M-1]
	lac	w6,a				; a  = after
						; a  = current (last)

	; Last iteration...
	; Upper branch: after = current - k[0]*del[M-1+1].
	msc	w4*w5,a				; a -= k[0]*del[M-1+1]
						; a  = after
	sac.r	a,w6				; w6 = after

	; Lower branch: del[M-1] = del[M-1+1] + k[0]*after.
	lac	w5,a				; a  = del[M-1+1]

	mac	w4*w6,a				; a += k[0]*after
						; a  = del[M-1]

	sac.r	a,[w11++]			; del[M-1] (updated)
						; w11->del[M]

	; Update last delay.
	mov	w6,[w11]			; del[M] = after

; .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . .

	; Ladder structure.
	mov	[w3+oGammaVals],w8		; w8-> g[0]

	; Only for zero-pole implementations,
	; but not for all-pole implementations...
	cp0	w8				; w8 == NULL ?
	bra	z,_allPole			; Yes => all pole
						; No  => zero-pole
	clr	a,[w8]+=2,w4,[w11]-=2,w5	; a  = 0
						; w4 = g[0]
						; w8-> g[1]
						; w5 = del[M]
						; w10->del[M-1]
	
	; All but last two iterations...
	repeat	w12			; {	; do (M-2)+1 times
	mac	w4*w5,a,[w8]+=2,w4,[w11]-=2,w5	; accumulate
						; w4 = g[m+1]
						; w8-> g[m+2]
						; w5 = del[M-m-1]
						; w10->del[M-m-2]
; }

	; One before last iteration...
	mac	w4*w5,a,[w8],w4,[w11],w5	; accumulate
						; w4 = g[M]
						; w5 = del[0]

	; Last iteration...
	mac	w4*w5,a				; accumulate
	bra	_storeOutput

_allPole:
	lac	w6,a				; a  = after
	inc2	w12,w8				; w8 = M
	sl	w8,w8				; w8 = (M)*sizeof(sample)
	sub	w11,w8,w11			; w11->del[0]

_storeOutput:
	; Store output.
	sac.r	a,[w1++]			; store y[n]
						; w1-> y[n+1]

_endFilter:
	; Rewind pointer.
	mov	w9,w8				; w8-> k[M-1]
; }






_completedIIR:
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
	pop     w14             ; w14 from TOS
	pop.d	w12				; {w12,w13} from TOS
	pop.d	w10				; {w10,w11} from TOS
	pop.d	w8				; {w8,w9} from TOS

;............................................................................

	return	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;############################################################################

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Imported from: /cvs/mds/cust/microchip/dspic30f/asm/iirlattd.asm,v
; Revision: 1.4
;
; Copyright 2002, Microchip, Inc.  All rights reserved.
;
; IIR Lattice delay initialization to zero.
;
; Revision 1.4  2003/04/02 17:18:37  joaquin
; Corrected filter delay initialization.
;
; Revision 1.3  2002/05/22 20:57:39  joaquin
; Corrected comment, since the initialization applies to either all pole,
; or zero-pole implementations. NO CHANGES IN CODE.
;
; Revision 1.2  2002/05/02 03:01:48  joaquin
; Comments for cycle count, memory size, and system resources usage.
;
; Revision 1.1  2002/04/12 01:02:24  joaquin
; Initial version.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _IIRLatticeInit: initialization to zero of IIR filter delay.
;
; Operation:
;	filter->delay[n] = 0, 0 <= m <= M
;
;	with M the filter order.
;
; Input:
;	w0 = h, ptr filter structure (see included file)
; Return:
;	(void)
;
; System resources usage:
;	{w0..w2}	used, not restored
;
; DO and REPEAT instruction usage.
;	no DO intructions
;	1 level REPEAT intruction
;
; Program words (24-bit instructions):
;	6
;
; Cycles (including C-function call and return overheads):
;	10 + M
;............................................................................

	.global	_IIRLatticeInit	; export
_IIRLatticeInit:
.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif
;............................................................................

	; Set up filter structure.
	mov	[w0+oOrder],w1			; w1 = M
	mov	[w0+oLattDelay],w0		; w0-> del[0]

;............................................................................

	mov	#0,w2				; w2 = 0
	repeat	w1				; do (M)+1 times
	mov	w2,[w0++]			; del[m] = 0
						; w0-> del[m+1]

;............................................................................
.ifndef PSV_ON
	pop	DSRPAG
.endif

	return	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

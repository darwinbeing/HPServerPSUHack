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
; The output vector must have a length of 2*N, even though only the first
; N output samples represent the actual result of the transformation. Note:
; N the length of the input vector; N must be an integer power of 2.
;
; Because the algorithm uses an IFFT operation, the user must provide as
; input a pointer to the N/2 complex twiddle factors to be used by the
; N-point IFFT; i.e., the complex conjugates of a regular set of twiddle
; factors. Besides, a pointer to the N/2 cosine-sine factors for the DCT
; must be input to this function.
;
; Also, the program performs an implicit scaling of 1/sqrt(2*N), with N
; the number of elements in the input vector.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


	; Local inclusions.
	.nolist
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _DCT: Real valued (out-of-place) Type II DCT.
;
; Operation:
;	X(k) = sqrt(2/N)*c[k)*sum_n {x[n)*CN[k(2*n+1)]},
; with
;	CN[m] = cos(m*pi/(2*N)),
; and
;	c[0] = 1/sqrt(2), c[k>0] = 1;
;
; n in {0, 1,... , N-1}, and
; k in {0, 1,... , N-1}, with N = 2^m.
;
; Input:
;	w0 = log2(N), N number of elements in source vector (log2N)
;	w1 = ptr to length 2*N destination vector (dstV)
;	w2 = ptr to length N source vector (srcV)
;	w3 = ptr to N/2 complex cosine-sine factors (cosineFactors)
;		 CN(k) = exp(i*k*pi/(2*N)), CN(0)...CN(N/2-1)
;	w4 = ptr to N/2 complex twiddle factors (twidFactors)
;		 WN(k) = exp(i*k*2*pi/N), WN(0)...WN(N/2-1)
;	w5 = COEFFS_IN_DATA, or memory program page with factors.
; Return:
;	w0 = ptr to destination vector (dstV)
;		(only first N samples represent DCT)
;
; System resources usage:
;	{w0..w5}	used, not restored
; plus system resources from VectorZeroPad, and DCTIP.
;
; DO and REPEAT instruction usage.
;	no DO intructions
;	no REPEAT intructions
; plus DO/REPEAT usage from VectorZeroPad, and DCTIP.
;............................................................................

	; External symbols.
	.extern	_VectorZeroPad
	.extern	_DCTIP

	.global	_DCT	; export
_DCT:

;............................................................................

	; Save context.
	push.d	w0				; {log2N,dstV} to TOS
	push	w3				; cosFactors to TOS
	push.d	w4				; {twidFactors,factPage} to TOS

;............................................................................

	; Find number of real plus imaginary componets.
	mov	#0x1,w3				; to be shifted...
	sl	w3,w0,w0			; w0 = N (1<<log2NVal)
	mov	w2,w3				; w3-> srcV[0]
	mov	w1,w2				; w2-> dstV[0]
	mov	w0,w1				; w1 = N

	; Zero pad source vector into destination vector.
	call	_VectorZeroPad
						; now w0-> dstV

	; Restore context.
	pop	w4				; factPage from TOS
	pop.d	w2				; {cosFactors,twidFactors}
						; from TOS
	pop.d	w0				; {log2N,dstV} from TOS

;............................................................................

	; Apply in-place operation to destination vector.
	call	_DCTIP
						; now w0-> dstV

;............................................................................

	return	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

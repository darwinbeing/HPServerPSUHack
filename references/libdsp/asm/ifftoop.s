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
; This Complex DIF IFFT implementation expects that the input data is a
; complex vector such that the magnitude of the real and imaginary parts
; of each of its elements is less than 0.5. If greater or equal to this
; value the results could produce saturation.
;
; Also, the program performs an implicit scaling of 1/2 for every stage
; to the intermediate values, so that the output values are scaled by a
; factor of 1/N, with N the length of the IFFT (which is desired for the
; computation of the inverse transform).
;
; NOTE: input is expected in natural order, and output is produced also
; in natural order.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


	; Local inclusions.
	.nolist
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _IFFTComplex: Complex DIF IFFT.
;
; Operation:
;	F(k) = 1/N*sum_n (f(n)*WN(-kn)), WN(-kn) = exp[(j*2*pi*k*n)/N],
;
; n in {0, 1,... , N-1}, and
; k in {0, 1,... , N-1}, with N = 2^m.
;
; Input:
;	w0 = number stages in IFFT (log2NVal)
;	w1 = ptr to complex destination vector (dstCV)
;	w2 = ptr to complex source vector (srcCV)
;	w3 = ptr to complex twiddle factors (twidFactors)
;	w4 = COEFFS_IN_DATA, or memory program page with twiddle factors.
; Return:
;	w0 = ptr to complex destination vector (dstCV)
;
; System resources usage:
;	{w0..w4}	used, not restored
; plus system resources from VectorCopy, and IFFTComplexIP.
;
; DO and REPEAT instruction usage.
;	no DO intructions
;	no REPEAT intructions
; plus DO/REPEAT usage from VectorCopy, and IFFTComplexIP.


;............................................................................

	; External symbols.
	.extern	_VectorCopy
	.extern	_IFFTComplexIP

	.global	_IFFTComplex	; export
_IFFTComplex:

;............................................................................

	; Save context.
	push.d	w0				; {log2N,dstCV} to TOS
	push	w3				; twidFactors to TOS
	push	w4				; twidPage to TOS

;............................................................................

	; Find number of real plus imaginary componets.
	mov	#0x2,w3				; to be shifted...
	sl	w3,w0,w0			; w0 = 2*N (2<<log2NVal)
						; w0 = N*sizeof(complex)

	; Copy (complex) source vector into (complex) destination vector.
	; w1,w2 already set to destination and source, respectively...
	call	_VectorCopy
						; now w0-> dstCV

	; Restore context.
	pop.d	w2				; {twidFactors,twidPage}
						; from TOS
	pop.d	w0				; {log2N,dstCV} from TOS

;............................................................................

	; Apply in-place operation to destination vector.
	call	_IFFTComplexIP
						; now w0-> dstCV

;............................................................................

	return	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

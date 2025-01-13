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
; factor of 1/N, with N the length of the FFT (which is desired for the
; computation of the inverse transform).
;
; NOTE: input is expected in bit reverse ordering, while output is produced
; in natural (sequential) ordering.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


	; Local inclusions.
	.nolist
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _IFFTComplexIP: Complex (in-place) DIF IFFT.
;
; Operation:
;	F(k) = 1/N*sum_n (f(n)*WN(-kn)), WN(-kn) = exp[(j*2*pi*k*n)/N],
;
; n in {0, 1,... , N-1}, and
; k in {0, 1,... , N-1}, with N = 2^m.
;
; Input:
;	w0 = number stages in FFT (log2NVal)
;	w1 = ptr to complex source vector (srcCV)
;	w2 = ptr to complex twiddle factors (twidFactors)
;	w3 = COEFFS_IN_DATA, or memory program page with twiddle factors.
; Return:
;	w0 = ptr to source vector (srcCV)
;
; NOTE: the complex source vector MUST be modulo 2*N word aligned for
;	the implicit bit reverse operation to succeed.
;
; System resources usage:
;	{w0,w3}		used, not restored
; plus resources from FFTComplexIP and BitReverseComplex.
;
; DO and REPEAT instruction usage.
;	no DO intructions
;	no REPEAT intructions


;............................................................................

	; External symbols.
	.extern	_BitReverseComplex
	.extern	_FFTComplexIP

	.global	_IFFTComplexIP	; export
_IFFTComplexIP:

;............................................................................

	; Save working registers.
	; none to save...

;............................................................................

	push	w1				; save return value (srcCV)

;............................................................................

	; Compute IFFT using DIF FFT algorithm.
	push	w0				; save log2NVal
	push	w1				; save pointer to srcCV
	call _FFTComplexIP
	pop	w1				; restore pointer to srcCV
	pop	w0				; restore log2NVal

	; Finally, unscramble results back to natural order.
	call _BitReverseComplex

;............................................................................

	pop	w0				; restore return value

;............................................................................

	; Restore working registers.
	; none to restore...

;............................................................................

	return	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

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
	.include	"dspcommon.inc"		; FIR filter structure
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _FIRStructInit: initialization of FIR filter structure.
;
; Operation:
;	FIRFilter->numCoeffs = numCoeffs;
;	FIRFilter->coeffsBase = coeffsBase;
;	FIRFilter->coeffsEnd = coeffsBase+numCoeffs-(1 byte);
;	FIRFilter->coeffsPage = coeffsPage;
;	FIRFilter->delayBase = delayBase;
;	FIRFilter->delayEnd = delayBase+numCoeffs-(1 byte);
;	FIRFilter->delay = delayBase;
;
; Input:
;	w0 = h, ptr FIR filter structure (see included file)
;	w1 = numCoeffs;
;	w2 = coeffsBase;
;	w3 = coeffsPage;
;	w4 = delayBase;
; Return:
;	(void)
;
; System resources usage:
;	{w0..w5}	used, not restored
;
; DO and REPEAT instruction usage.
;	no DO intructions
;	no REPEAT intruction


	.global	_FIRStructInit	; export
_FIRStructInit:
.ifndef PSV_ON
	push	DSRPAG
	movpag #0x0001, DSRPAG
.endif
;............................................................................

	; Prepare for initialization.
	sl	w1,w5				; w5 = numCoeffs*sizeof(coeffs)
	dec	w5,w5				; w5 =
						;   numCoeffs*sizeof(coeffs)-1

;............................................................................

	; Set up filter structure.
	mov	w1,[w0++]		; FIRFilter->numCoeffs = numCoeffs
					; w0 =&(FIRFilter->coeffsBase)
	mov	w2,[w0++]		; FIRFilter->coeffsBase = coeffsBase
					; w0 =&(FIRFilter->coeffsEnd)
	add	w2,w5,[w0++]		; FIRFilter->coeffsEnd initialized
					; w0 =&(FIRFilter->coeffsPage)
	mov	w3,[w0++]		; FIRFilter->coeffsPage = coeffsPage
					; w0 =&(FIRFilter->delayBase)
	mov	w4,[w0++]		; FIRFilter->delayBase = delayBase
					; w0 =&(FIRFilter->delayEnd)
	add	w4,w5,[w0++]		; FIRFilter->delayEnd initialized
					; w0 =&(FIRFilter->delay)
	mov	w4,[w0]			; FIRFilter->delay initialized
.ifndef PSV_ON
	pop	DSRPAG
.endif
;............................................................................

	return	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF

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
	.include	"dspcommon.inc"		; fractsetup
	.list

	.section .libdsp, code

	.global	_MatrixInvert	; export
_MatrixInvert:
	lnk	#24

.ifndef PSV_ON
	movpag #0x0001, DSRPAG
.endif

	mov.d	w8,[w15++]
	mov.d	w10,[w15++]
	mov.d	w12,[w15++]
	mov	w14,[w15++]

	mov	w0,w14
	mov	w1,w13
	mov	w3,[w15-38]
	mov	w4,[w15-36]
	mov	w5,[w15-34]
	mov	w1,[w15-32]
	mul.uu	w0,#0,w0
		
	mov	w0,[w15-30]
	mov	w1,[w15-28]
	mov	#0,w0
	mov	w0,[w15-22]
	mov	w0,[w15-20]
	mov	w0,[w15-24]
	subr	w14,#0,[w15]
	bra	ge,.L75
	mul.uu	w4,#0,w4
		
.L6:
	mov	[w15-24],w8
	mul.uu	w8,#4,w0
	mov	[w15-38],w1
	add	w1,w0,w12
	mov.d	w4,[w12]
	
	sl w8,w9
	mov	[w15-36],w11
	add	w9,w11,w10
	clr	[w10]
	mov	[w15-34],w7
	add	w9,w7,w3
	clr	[w3]
	inc	w8,w6
	mov	w6,[w15-24]
	sub	w6,w14,[w15]
	bra	lt,.L6
.L75:
	sub	w2,w13,[w15]
	bra	z,.L7
	cp0	w14
	bra	le,.L77
	mov	w14,[w15-24]
.L17:
	cp0	w14
	bra	le,.L79
	mov	w14,w11
.L16:
	mov	[w2++],[w13++]
	mov	[w2++],[w13++]
	dec	w11,w11
	bra	nz,.L16
.L79:
	mov	[w15-24],w0
	dec	w0,w5
	mov	w5,[w15-24]
	bra	nz,.L17
.L77:
	mov	[w15-32],w13
.L7:
	mov	#0,w2
	mov	w2,[w15-26]
	cp0	w14
	bra	le,.L81
.L62:
	mul.uu	w8,#0,w8
		
	mov	#0,w6
	mov	w6,[w15-24]
	cp0	w14
	bra	le,.L83
.L37:
	mov	[w15-24],w3
	mul.uu	w3,#4,w0
	mov	[w15-38],w7
	add	w7,w0,w0
	mov.d	[w0],w0
	mul.uu	w2,#0,w2
		
	call	___eqsf2
	cp0	w0
	bra	nz,.L24
	mov	#0,w11
	cp0	w14
	bra	le,.L24
.L36:
	mul.uu	w11,#4,w0
	mov	[w15-38],w10
	add	w10,w0,w0
	mov.d	[w0],w0
	mul.uu	w2,#0,w2
		
	call	___eqsf2
	cp0	w0
	bra	nz,.L30
	mov	[w15-24],w12
	mul.ss	w12,w14,w0
	add	w11,w0,w0
	mul.uu	w0,#4,w0
	add	w13,w0,w0
	mov.d	[w0],w0
	call	___extendsfdf2
	bclr	w3,#15
	call	___truncdfsf2
	mov	w0,[w15-30]
	mov	w1,[w15-28]
	mov.d	w8,w2
	call	___gesf2
	cp0	w0
	bra	lt,.L30
	mov	[w15-30],w8
	mov	[w15-28],w9
	mov	[w15-24],w1
	mov	w1,[w15-22]
	mov	w11,[w15-20]
.L30:
	inc	w11,w11
	sub	w11,w14,[w15]
	bra	lt,.L36
.L24:
	mov	[w15-24],w5
	inc	w5,w11
	mov	w11,[w15-24]
	sub	w11,w14,[w15]
	bra	lt,.L37
.L83:
	mov	[w15-20],w2
	mul.uu	w2,#4,w8
	mov	[w15-38],w0
	add	w0,w8,w9
	mov.d	[w9],w0
	mov	#0,w2
	mov	#16256,w3
	call	___addsf3
	mov.d	w0,[w9]
	mov	[w15-22],w7
	mov	[w15-20],w8
	sub	w7,w8,[w15]
	bra	z,.L94
	mov	#0,w11
	cp0	w14
	bra	le,.L95
	mul.ss	w7,w14,w0
	mov	[w15-20],w3
	mul.ss	w3,w14,w2
	mov	w2,[w15-18]
	mov	w3,[w15-16]
	mov	w2,w5
	mov	w0,w4
.L43:
	mul.uu	w4,#4,w2
	add	w13,w2,w6
	mov.d	[w6],w8
	mul.uu	w5,#4,w0
	add	w13,w0,w3
	mov	[w3++],[w6++]
	mov	[w3--],[w6--]
	mov.d	w8,[w3]
	inc	w5,w5
	inc	w4,w4
	inc	w11,w11
	sub	w11,w14,[w15]
	bra	lt,.L43
.L38:
	mov	[w15-26],w0
	
	sl w0,w11
	mov	[w15-22],w9
	mov	[w15-36],w5
	mov	w9,[w11+w5]
	mov	[w15-20],w12
	mov	[w15-34],w1
	mov	w12,[w11+w1]
	mov	[w15-18],w7
	add	w12,w7,w0
	mul.uu	w0,#4,w0
	add	w13,w0,w12
	mov.d	[w12],w0
	call	___extendsfdf2
	mov.d	w2,w10
	mov.d	w0,w8
	mul.uu	w4,#0,w4
	mul.uu	w6,#0,w6
	call	___eqdf2
	mov	#0,w1
	cp0	w0
	bra	z,.L1
	mov.d	w8,w4
	mov.d	w10,w6
	mul.uu	w0,#0,w0
	mov	#0,w2
	mov	#16368,w3
	call	___divdf3
	call	___truncdfsf2
	mov	w0,[w15-30]
	mov	w1,[w15-28]
	mov	#0,w2
	mov	#16256,w3
	mov.d	w2,[w12]
	cp0	w14
	bra	le,.L87
	mov	[w15-18],w10
	mov	w14,w11
.L50:
	mul.uu	w10,#4,w8
	add	w13,w8,w12
	mov.d	[w12],w0
	mov	[w15-30],w2
	mov	[w15-28],w3
	call	___mulsf3
	mov.d	w0,[w12]
	inc	w10,w10
	dec	w11,w11
	bra	nz,.L50
.L87:
	mov	#0,w3
	mov	w3,[w15-24]
	cp0	w14
	bra	le,.L89
.L61:
	mov	[w15-24],w0
	mov	[w15-20],w1
	sub	w0,w1,[w15]
	bra	z,.L53
	mul.ss	w0,w14,w4
	add	w1,w4,w0
	mul.uu	w0,#4,w0
	add	w13,w0,w8
	mov.d	[w8],w10
	mov	w10,[w15-30]
	mov	w11,[w15-28]
	mul.uu	w6,#0,w6
		
	mov.d	w6,[w8]
	cp0	w14
	bra	le,.L53
	mov	[w15-18],w12
	mov	w4,w10
	mov	w14,w11
.L60:
	mul.uu	w10,#4,w8
	add	w13,w8,w9
	mul.uu	w12,#4,w0
	add	w13,w0,w0
	mov.d	[w0],w0
	mov	[w15-30],w2
	mov	[w15-28],w3
	call	___mulsf3
	mov.d	w0,w2
	mov.d	[w9],w0
	call	___subsf3
	mov.d	w0,[w9]
	inc	w12,w12
	inc	w10,w10
	dec	w11,w11
	bra	nz,.L60
.L53:
	mov	[w15-24],w7
	inc	w7,w4
	mov	w4,[w15-24]
	sub	w4,w14,[w15]
	bra	lt,.L61
.L89:
	mov	[w15-26],w11
	inc	w11,w1
	mov	w1,[w15-26]
	sub	w1,w14,[w15]
	bra	lt,.L62
.L81:
	dec	w14,w11
	bra	lt,.L92
.L73:
	
	sl w11,w5
	mov	[w15-36],w0
	mov	[w5+w0],w1
	mov	[w15-34],w12
	mov	[w5+w12],w0
	sub	w1,w0,[w15]
	bra	z,.L65
	mov	#0,w2
	mov	w2,[w15-24]
	cp0	w14
	bra	le,.L65
	mov.d	w0,w4
.L72:
	mov	[w15-24],w9
	mul.ss	w9,w14,w0
	add	w5,w0,w2
	mul.uu	w2,#4,w2
	add	w13,w2,w6
	mov.d	[w6],w8
	add	w4,w0,w0
	mul.uu	w0,#4,w0
	add	w13,w0,w10
	mov	[w10++],[w6++]
	mov	[w10--],[w6--]
	mov.d	w8,[w10]
	mov	[w15-24],w8
	inc	w8,w3
	mov	w3,[w15-24]
	sub	w3,w14,[w15]
	bra	lt,.L72
.L65:
	dec	w11,w11
	bra	ge,.L73
.L92:
	mov	[w15-32],w1
	bra	.L1
.L95:
	mov	[w15-20],w3
	mul.ss	w3,w14,w4
	mov	w4,[w15-18]
	mov	w5,[w15-16]
	bra	.L38
.L94:
	mov	[w15-20],w1
	mul.ss	w1,w14,w10
	mov	w10,[w15-18]
	mov	w11,[w15-16]
	bra	.L38
.L1:
	mov	w1,w0

	mov	[--w15],w14
	mov.d	[--w15],w12
	mov.d	[--w15],w10
	mov.d	[--w15],w8

.ifndef PSV_ON
	pop	DSRPAG
.endif

	ulnk
	return

	.end

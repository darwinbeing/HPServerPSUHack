;*********************************************************************
;                                                                    *
;                       Software License Agreement                   *
;                                                                    *
;   The software supplied herewith by Microchip Technology           *
;   Incorporated (the "Company") for its dsPIC controller            *
;   is intended and supplied to you, the Company's customer,         *
;   for use solely and exclusively on Microchip dsPIC                *
;   products. The software is owned by the Company and/or its        *
;   supplier, and is protected under applicable copyright laws. 	 *
;	All rights are reserved. Any use in violation of the foregoing   *
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
;   (c) Copyright 2007 Microchip Technology, All rights reserved.    *
;*********************************************************************


	; Local inclusions.
	.nolist
	.include	"dspcommon.inc"		; MODCON, XBREV
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code
		
	.global _MagnitudeCplx32bIP


	.macro 	CPLXMAG_MACRO
; Pr*Pr 32-bit multiplication								
			MOV.D	[w8],W6					; (w6,w7)=Pr=(y0,y1)								
			MUL.SU	w7,[w8],w0				; y1*x0
			MUL.US 	w6,[++w8],w2			; y0*x1
			MUL.SS	w7,[w8++],w4			; y1*x1

			LAC		w3,A
			ADD		w1,A					
			SFTAC	A,#16					; A=(y1*x0 + y0*x1)>>16

			ADD		w4,[w10],[w10++]
			ADDC	w5,[w10],[w10--]		; A=Pr*Pr=y1*x1 + (y1*x0 + y0*x1)>>16	

	
; Pi*Pi 32-bit multiplication
			MOV.D	[w8],W6					; (w6,w7)=Pi=(y0,y1)								
			MUL.SU	w7,[w8],w0				; y1*x0
			MUL.US 	w6,[++w8],w2			; y0*x1
			MUL.SS	w7,[w8++],w4			; y1*x1

			LAC		w3,B
			ADD		w1,B					
			SFTAC	B,#16					; B=(y1*x0 + y0*x1)>>16

			ADD		w4,[w11],[w11++]
			ADDC	w5,[w11],[w11--]		; B=Pi*Pi=y1*x1 + (y1*x0 + y0*x1)>>16	

			ADD		A						; A=(Pr)^2 + (Pi)^2
	.endm




;............................................................................
; Real FFT Calculation
;............................................................................
_MagnitudeCplx32bIP:
;............................................................................
; Context Save
			PUSH.D	w8						; {w8,w9} to TOS
			PUSH.D	w10						; {w10,w11} to TOS
;............................................................................
			MOV		w1,w8					; w8 ---> Source
			MOV		w1,w9					; w9 ---> Destination

			MOV		#ACCAL,w10
			MOV		#ACCBL,w11
	
			DEC		w0,w0					; N=N-1

			DO 		w0,MAG_END			

			CPLXMAG_MACRO
			MOV		[w10++],[w9++]			; Store Magnitude Result	
MAG_END:	MOV		[w10--],[w9++]
     

 ;............................................................................
; Context Restore
			POP.D	w10					; {w10,w11} from TOS
			POP.D	w8					; {w8,w9} from TOS
;............................................................................
			RETURN	


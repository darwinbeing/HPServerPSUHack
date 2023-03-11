/**********************************************************************
* © 2006 Microchip Technology Inc.
*
* FileName:        toneGen.s
* Dependencies:    Header (.h/.inc) files if applicable, see below
* Processor:       dsPIC33Fxxxx/PIC24Hxxxx
* Compiler:        MPLAB® C30 v2.01.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Inc. (“Microchip”) licenses this software to you
* solely for use with Microchip dsPIC® digital signal controller
* products. The software is owned by Microchip and is protected under
* applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED “AS IS.”  MICROCHIP EXPRESSLY DISCLAIMS ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
* BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
* DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
* PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
* BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
* ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MCHP	          22Sep06  First release of source file
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
**********************************************************************/
	
	; Local inclusions.
	.nolist
	.include	"dspcommon.inc"		; MODCON, XBREV
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code
	
	.global _toneGen32b

	.equ	TF_FFT_SIZE,1024			; 10stage, 1024 point complex FFT

;............................................................................
_toneGen32b:
;............................................................................
; Context Save
			PUSH	CORCON
.ifndef PSV_ON
			PUSH 	DSRPAG
.else
			PUSH	PSVPAG
.endif
			LNK     #20
;............................................................................

;............................................................................
;;	PSVPAG = __builtin_psvpage(&twiddleFactor);
;;	CORCONbits.PSV = 1;	
.ifndef PSV_ON
			MOV		w4,DSRPAG
.else
			MOV		w4,PSVPAG
			BSET	CORCON,#2
.endif

			MOV		#4,w4
			MUL.UU	w4,w1,w4				; 4*tone
			MUL.UU	w4,w0,w4				; 4*tone*sample
			SL		w2,#2,w1				; 2*phase
			ADD		w4,w1,w4

			MOV		#(TF_FFT_SIZE*2),w6
			DEC		w6,w6	
			AND		w6,w4,w7
		

			MOV		#ACCAL,w5
			ADD		w7,w3,w7
			LAC		[++w7],A
			MOV		[--w7],[w5]
	


			INC		w6,w6
			AND 	w6,w4,w6

			BRA		nz,NEG_VAL
			BRA		EXIT			


NEG_VAL:
			NEG		A


EXIT:
			MOV		[w5++],w0
			MOV		[w5],w1
;............................................................................
; Context Restore
			ULNK
.ifndef PSV_ON
			POP 	DSRPAG
.else
			POP		PSVPAG
.endif
			POP		CORCON
;............................................................................
			RETURN	


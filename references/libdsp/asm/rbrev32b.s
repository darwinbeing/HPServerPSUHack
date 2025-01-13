/**********************************************************************
* © 2006 Microchip Technology Inc.
*
* FileName:        bitRev.s
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
* MCHP	           22Sep06  First release of source file
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
		
	.global _BitReverseReal32bIP


;............................................................................
_BitReverseReal32bIP:
;............................................................................
; Context Save
			PUSH	MODCON
			PUSH	XBREV
;............................................................................
; MODCONbits.BWM=0b0111  ; W2 for Bit reverse
; XBREVbits.BREN=1
; XBREVbits.XB= N 
; N-----> Size of the real FFT, for 1024 point real FFT "N" should be 1024

			MOV		#0x0700,w7				; Uses w7 for bit reverse
			MOV		w7,MODCON
			MOV		#0x8000,w7				; Enable bit reverse addresing
			IOR		w0,w7,w7
			MOV		w7,XBREV				; Element N (equivalent to a
											; sample N/2 for complex data)
			LSR		w0,#1,w0
			DEC 	w0,w0					; w6=(N/2)-1
			MOV		w1,w7					


			DO 		w0,BREV_END
            CP		w7,w1					; w1-w7 ?
			BRA		le,BREV_SKIP			; w1 <= w7 => skip to next

											; Swap 32-bit Complex pairs   
            MOV   	[w1+0],w2    			
			MOV		[w1+2],w3
            MOV   	[w1+4],w4    			
			MOV		[w1+6],w5

			MOV		[w7++],[w1++]
			MOV		[w7++],[w1++]			
			MOV		[w7++],[w1++]
			MOV		[w7++],[w1++]

            MOV   	w5,[--w7]         		; Swap Real Data
            MOV   	w4,[--w7]  
            MOV   	w3,[--w7]         		; Swap Real Data
            MOV   	w2,[--w7] 
			SUB		w1,#8,w1

BREV_SKIP:     


            ADD		w1,#8,w1
BREV_END:	MOV		[w7],[w7++]				; bit reverse update
											; (only on data writes!!!)


;............................................................................
; Context Restore
			POP		XBREV
			POP		MODCON
;............................................................................
			RETURN	


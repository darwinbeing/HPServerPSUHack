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
;   (c) Copyright 2007 Microchip Technology, All rights reserved.    *
;*********************************************************************



	; Local inclusions.
	.nolist
	.include	"dspcommon.inc"		; MODCON, XBREV
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.section .libdsp, code
	
	.global _IFFTComplex32bIP



	.equ	TF_FFT_SIZE,1024			; 10stage, 1024 point complex FFT
	.equ	TF_FFT_STG, 10


;******************************************************************************
; Butterfly Computation
; P=Pr+jPi, Q=Qr+jQi, W=Wr-jWi
; P'=P+W*Q and Q'=P-W*Q
;
; Indirect Address Pointers, it will be update to point to next date inputs 
; W8->(Pr,Pi,Pr+1,Pi+1,....)
; W9->(Qr,Qi,Qr+1,Qi+1,....)
; W10-> Wr
; W11-> Wi

; W12 -> ACCA
; W13 -> ACCB
; W14 -> FP, First two location used for TEMP

; Equations for Butterfly Computation
; Pr'=Pr+(Qr*Wr-Qi*Wi)  
; Qr'=Pr-(Qr*Wr-Qi*Wi)
; Pi'=Pi+(Qi*Wr+Qr*Wi)
; Qi'=Pi-(Qi*Wr+Qr*Wi)
;******************************************************************************          
	.macro BFLY_MACRO
	
; Qr*Wi 32-bit multiplication
			MOV.D	[w11],W6				; (w6,w7)=Wi=(y0,y1)
											; (w9,w9+1)=Wi=(x0,x1)

			MUL.SU	w7,[w9],w0				; y1*x0
			MUL.US 	w6,[++w9],w2			; y0*x1
			MUL.SS	w7,[w9++],w4			; y1*x1

			LAC		w3,B
			ADD		w1,B					
			SFTAC	B,#16					; B=(y1*x0 + y0*x1)>>16

			ADD		w4,[w13++],[w14++]
			ADDC	w5,[w13--],[w14--]		; TEMP=Qr*Wi=y1*x1 + (y1*x0 + y0*x1)>>16	


; Qi*Wi 32-bit multiplication
											; (w6,w7)=Wi=(y0,y1)
											; (w9,w9+1)=Qi=(x0,x1)								
			MUL.SU	w7,[w9],w0				; y1*x0
			MUL.US 	w6,[++w9],w2			; y0*x1
			MUL.SS	w7,[w9--],w4			; y1*x1

			LAC		w3,B
			ADD		w1,B					
			SFTAC	B,#16					; B=(y1*x0 + y0*x1)>>16
		
			ADD 	w4,[w13],[w13++]
			ADDC	w5,[w13],[w13--]		; B=Qi*Wi=y1*x1 + (y1*x0 + y0*x1)>>16	


; Qr*Wr 32-bit multiplication
			MOV.D	[w10],W6				; (w6,w7)=Wr=(y0,y1)
											; (w9-2,w9-1)=Qr=(x0,x1)	
							
			MUL.US 	w6,[--w9],w2			; y0*x1
			MUL.SS	w7,[w9--],w4			; y1*x1
			MUL.SU	w7,[w9],w0				; y1*x0

			LAC		w3,A
			ADD		w1,A					
			SFTAC	A,#16					; A=(y1*x0 + y0*x1)>>16
		
			ADD 	w4,[w12],[w12++]
			ADDC	w5,[w12],[w12--]		; A=Qr*Wr=y1*x1 + (y1*x0 + y0*x1)>>16



; Pr'=(Pr+Qr*Wr+Qi*Wi)/2  
; Qr'=(Pr-Qr*Wr-Qi*Wi)/2		
			SUB		A						; A=Qr*Wr-Qi*Wi	
			SFTAC	A,#-1	
			LAC		[++W8],B				; B=Pr	
			MOV		[--w8],[w13]
			
			ADD		A						; A=Pr+(Qr*Wr-Qi*Wi)
			MOV		[w12++],[W8++]
			MOV		[w12--],[w8++]			; Pr'=Pr+(Qr*Wr-Qi*Wi) 
		
			SUB		A						
			SUB		B						; B=Pr-(Qr*Wr-Qi*Wi)				
			MOV		[w13++],[W9++]
			MOV		[w13--],[w9++]			; Qr'=Pr-(Qr*Wr-Qi*Wi) 	
	
	
; Qi*Wr 32-bit multiplication
											; (w6,w7)=Wr=(y0,y1)
											; (w9,w9++)=Qi=(x0,x1)
			MUL.SU	w7,[w9],w0				; y1*x0
			MUL.US 	w6,[++w9],w2			; y0*x1
			MUL.SS	w7,[w9--],w4			; y1*x1

			LAC		w3,A
			ADD		w1,A					
			SFTAC	A,#16					; B=(y1*x0 + y0*x1)>>16
		
			ADD 	w4,[w12],[w12++]
			ADDC	w5,[w12],[w12--]		; A=Qi*Wr=y1*x1 + (y1*x0 + y0*x1)>>16	
											
; Pi'=(Pi+Qi*Wr-Qr*Wi)/2
; Qi'=(Pi-Qi*Wr+Qr*Wi)/2
			LAC		[++W14],B					
			MOV		[--w14],[w13]			; B=Qr*Wi
	
			ADD		A						; A=Qi*Wr+Qr*Wi
			SFTAC	A,#-1		
			LAC		[++W8],B				; B=Pi	
			MOV		[--w8],[w13]

			ADD		A						; A=Pi+(Qi*Wr+Qr*Wi)		
			MOV		[w12++],[W8++]
			MOV		[w12--],[w8++]			; Pi'=Pi+(Qi*Wr+Qr*Wi) 
			
			SUB		A
			SUB		B						; B=Pi-(Qi*Wr+Qr*Wi)		
			MOV		[w13++],[W9++]
			MOV		[w13--],[w9++]			; Qi'=Pi-(Pi*Wr+Qr*Wi)  	

	.endm


	.macro BFLY_MACRO_PSV
	
; Qr*Wi 32-bit multiplication
			POP		DSRPAG
			MOV.D	[w11],W6				; (w6,w7)=Wi=(y0,y1)
											; (w9,w9+1)=Wi=(x0,x1)
			PUSH	DSRPAG
			MOVPAG  #0x01,DSRPAG

			MUL.SU	w7,[w9],w0				; y1*x0
			MUL.US 	w6,[++w9],w2			; y0*x1
			MUL.SS	w7,[w9++],w4			; y1*x1

			LAC		w3,B
			ADD		w1,B					
			SFTAC	B,#16					; B=(y1*x0 + y0*x1)>>16

			ADD		w4,[w13++],[w14++]
			ADDC	w5,[w13--],[w14--]		; TEMP=Qr*Wi=y1*x1 + (y1*x0 + y0*x1)>>16	


; Qi*Wi 32-bit multiplication
											; (w6,w7)=Wi=(y0,y1)
											; (w9,w9+1)=Qi=(x0,x1)								
			MUL.SU	w7,[w9],w0				; y1*x0
			MUL.US 	w6,[++w9],w2			; y0*x1
			MUL.SS	w7,[w9--],w4			; y1*x1

			LAC		w3,B
			ADD		w1,B					
			SFTAC	B,#16					; B=(y1*x0 + y0*x1)>>16
		
			ADD 	w4,[w13],[w13++]
			ADDC	w5,[w13],[w13--]		; B=Qi*Wi=y1*x1 + (y1*x0 + y0*x1)>>16	


; Qr*Wr 32-bit multiplication
			POP		DSRPAG									
			MOV.D	[w10],W6				; (w6,w7)=Wr=(y0,y1)
											; (w9-2,w9-1)=Qr=(x0,x1)	
			PUSH	DSRPAG
			MOVPAG  #0x01,DSRPAG
							
			MUL.US 	w6,[--w9],w2			; y0*x1
			MUL.SS	w7,[w9--],w4			; y1*x1
			MUL.SU	w7,[w9],w0				; y1*x0

			LAC		w3,A
			ADD		w1,A					
			SFTAC	A,#16					; A=(y1*x0 + y0*x1)>>16
		
			ADD 	w4,[w12],[w12++]
			ADDC	w5,[w12],[w12--]		; A=Qr*Wr=y1*x1 + (y1*x0 + y0*x1)>>16



; Pr'=(Pr+Qr*Wr+Qi*Wi)/2  
; Qr'=(Pr-Qr*Wr-Qi*Wi)/2		
			SUB		A						; A=Qr*Wr-Qi*Wi	
			SFTAC	A,#-1	
			LAC		[++W8],B				; B=Pr	
			MOV		[--w8],[w13]
			
			ADD		A						; A=Pr+(Qr*Wr-Qi*Wi)
			MOV		[w12++],[W8++]
			MOV		[w12--],[w8++]			; Pr'=Pr+(Qr*Wr-Qi*Wi) 
		
			SUB		A						
			SUB		B						; B=Pr-(Qr*Wr-Qi*Wi)				
			MOV		[w13++],[W9++]
			MOV		[w13--],[w9++]			; Qr'=Pr-(Qr*Wr-Qi*Wi) 	
	
	
; Qi*Wr 32-bit multiplication
											; (w6,w7)=Wr=(y0,y1)
											; (w9,w9++)=Qi=(x0,x1)
			MUL.SU	w7,[w9],w0				; y1*x0
			MUL.US 	w6,[++w9],w2			; y0*x1
			MUL.SS	w7,[w9--],w4			; y1*x1

			LAC		w3,A
			ADD		w1,A					
			SFTAC	A,#16					; B=(y1*x0 + y0*x1)>>16
		
			ADD 	w4,[w12],[w12++]
			ADDC	w5,[w12],[w12--]		; A=Qi*Wr=y1*x1 + (y1*x0 + y0*x1)>>16	
											
; Pi'=(Pi+Qi*Wr-Qr*Wi)/2
; Qi'=(Pi-Qi*Wr+Qr*Wi)/2
			LAC		[++W14],B					
			MOV		[--w14],[w13]			; B=Qr*Wi
	
			ADD		A						; A=Qi*Wr+Qr*Wi
			SFTAC	A,#-1		
			LAC		[++W8],B				; B=Pi	
			MOV		[--w8],[w13]

			ADD		A						; A=Pi+(Qi*Wr+Qr*Wi)		
			MOV		[w12++],[W8++]
			MOV		[w12--],[w8++]			; Pi'=Pi+(Qi*Wr+Qr*Wi) 
			
			SUB		A
			SUB		B						; B=Pi-(Qi*Wr+Qr*Wi)		
			MOV		[w13++],[W9++]
			MOV		[w13--],[w9++]			; Qi'=Pi-(Pi*Wr+Qr*Wi)  	

	.endm


;............................................................................
; Pointer Usage
; w8->(Pr1,Pi1)
; w9->(Pr2,Pi2)
; w10->(Pr3,Pi3)
; w11->(Pr4,Pi4)
;
; W12 -> ACCA
; W13 -> ACCB
; W14 -> FP, First two location used for TEMP
;
; Calculations
; Pr6   ->      Pr1'=(Pr1+Pr2)+(Pr3+Pr4)
; Pr2   ->      Pr2'=(Pr1-Pr2)+(Pi3-Pi4)
; Pr3   ->      Pr3'=(Pr1+Pr2)-(Pr3+Pr4)
; Pr4   ->      Pr4'=(Pr1-Pr2)-(Pi3-Pi4)
; Pr6+1 ->      Pi1'=(Pi1+Pi2)+(Pi3+Pi4)
; Pr2+1 ->      Pi2'=(Pi1-Pi2)-(Pr3-Pr4)
; Pr3+1 ->      Pi3'=(Pi1+Pi2)-(Pi3+Pi4)
; Pr4+1 ->      Pi4'=(Pi1-Pi2)+(Pr3-Pr4)
;............................................................................ 
   

	.macro STG12_MACRO
;----------
			LAC		[++w8],B				; B=Pr1	
			MOV		[--w8],[w13]	
			LAC		[++w9],A				; A=Pr2	
			MOV		[--w9],[w12]

			ADD		A						; A=(Pr1+Pr2)
			MOV		[w12++],[W8++]			; Pr1=(Pr1+Pr2)
			MOV		[w12--],[w8++]			

			SUB		A
			SUB		B						; B=(Pr1-Pr2)
			MOV		[w13++],[W9++]			; Pr2=(Pr1-Pr2)
			MOV		[w13--],[w9++]			  

;----------
			LAC		[++w10],B				; B=Pr3	
			MOV		[--w10],[w13]
			LAC		[++w11],A				; A=Pr4	
			MOV		[--w11],[w12]

			ADD		A						; A=(Pr3+Pr4)
			MOV		[w12++],[W10++]			; Pr3=(Pr3+Pr4)
			MOV		[w12--],[w10++]			

			SUB		A
			SUB		B						; B=(Pr3-Pr4)
			MOV		[w13++],[W11++]			; Pr4=(Pr3-Pr4)
			MOV		[w13--],[w11++]		

			MOV		[w13++],w0				; (w0,w1)=(Pr3-Pr4)
			MOV		[w13--],w1			  
			
;----------
			LAC		[++w8],B				; B=Pi1	
			MOV		[--w8],[w13]
			LAC		[++w9],A				; A=Pi2	
			MOV		[--w9],[w12]

			ADD		A						; A=(Pi1+Pi2)
			MOV		[w12++],[W8++]			; Pi1=(Pi1+Pi2)
			MOV		[w12--],[w8--]			

			SUB		A
			SUB		B						; B=(Pi1-Pi2)
			MOV		[w13++],[W9++]			; Pi2=(Pi1-Pi2)
			MOV		[w13--],[w9--]			  

;----------
			LAC		[++w10],B				; B=Pi3	
			MOV		[--w10],[w13]
			LAC		[++w11],A				; A=Pi4	
			MOV		[--w11],[w12]

			ADD		A						; A=(Pi3+Pi4)
			MOV		[w12++],[W10++]			; Pr3=(Pi3+Pi4)
			MOV		[w12--],[w10--]			

			SUB		A
			SUB		B						; B=(Pi3-Pi4)
			MOV		[w13++],w2				; (w2,w3)=(Pi3-Pi4)
			MOV		[w13--],w3				
;---------------------------------
;---------------------------------	  
;---------- (Pi1,Pi3)
			LAC		[++w8],B				; B=(Pi1+Pi2)
			MOV		[--w8],[w13]
			LAC		[++w10],A				; A=(Pi3+Pi4)
			MOV		[--w10],[w12]

			ADD		A						; A=(Pi1+Pi2+Pi3+Pi4)
			MOV		[w12++],[W8++]			; Pi1=(Pi1+Pi2+Pi3+Pi4)
			MOV		[w12--],[w8--]			

			SUB		A
			SUB		B						; B=(Pi1+Pi2-Pi3-Pi4)
			MOV		[w13++],[W10++]			; Pi3=(Pi1+Pi2-Pi3-Pi4)
			MOV		[w13--],[w10--]			  

;---------- (Pr1,Pr3)
			LAC		[--w8],B				; B=(Pr1+Pr2)	
			MOV		[--w8],[w13]
			LAC		[--w10],A				; A=(Pr3+Pr4)	
			MOV		[--w10],[w12]

			ADD		A						; A=(Pr1+Pr2+Pr3+Pr4)
			MOV		[w12++],[W8++]			; Pr1=(Pr1+Pr2+Pr3+Pr4)
			MOV		[w12--],[w8--]			

			SUB		A
			SUB		B						; B=(Pr1+Pr2-Pr3-Pr4)
			MOV		[w13++],[W10++]			; Pr3=(Pr1+Pr2-Pr3-Pr4)
			MOV		[w13--],[w10--]			  

;---------- (Pi2,Pi4)
			LAC		[++w9],B				; B=(Pi1-Pi2)	
			MOV		[--w9],[w13]
			LAC		w1,A					; A=(Pr3-Pr4)	
			MOV		w0,[w12]

			ADD		A						; A=(Pi1-Pi2+Pr3-Pr4)
			MOV		[w12++],[W9++]			; Pi2=(Pi1-Pi2)+(Pr3+Pr4)
			MOV		[w12--],[w9--]			


			SUB		A
			SUB		B						; B=(Pi1-Pi2-Pr3+Pr4)
			MOV		[w13++],[W11++]			; Pi4=(Pi1-Pi2)-(Pr3-Pr4)
			MOV		[w13--],[w11--]			  

;----------(Pr2,Pr4)
			LAC		[--w9],B				; B=(Pr1-Pr2)/4	
			MOV		[--w9],[w13]
			LAC		w3,A					; A=(Pi3-Pi4)/4	
			MOV		w2,[w12]

			ADD		A						; A=(Pr1-Pr2+Pi3-Pi4)
			MOV		[++w12],[--W11]			; Pr4=(Pr1-Pr2+Pi3-Pi4)
			MOV		[--w12],[--w11]			

			SUB		A
			SUB		B						; B=(Pr1-Pr2-Pi3+Pi4)
			MOV		[w13++],[W9++]			; Pr2=(Pr1-Pr2-Pi3+Pi4)
			MOV		[w13--],[w9--]			  	

	.endm



; Local Stack Frame
;............................................................................
;   |_______|
;   |_______|<- Stack Pointer                          	SP 
;   |_______|<- Stack Pointer Return                   	SP+20
;   |_______|<- Buffer Pointer			                FP+18
;   |_______|<- Wr Pointer								FP+16
;   |_______|<- Wi Pointer                           	FP+14 
;   |_______|<- Stage Counter				            FP+12
;   |_______|<- Block Counter			                FP+10
;   |_______|<- Butterfly Counter		                FP+8
;   |_______|<- Block INDEX     						FP+6
;   |_______|<- Butterfly INDEX	              			FP+4
;   |_______|<- Temp                               		FP                                              
;............................................................................

    .equ    SP_RET,     20
	.equ	BUF_PTR,	18
	.equ	WR_PTR,		16
	.equ	WI_PTR,		14
	.equ	STG_CNTR,   12
	.equ 	BLK_CNTR,   10
	.equ	BFL_CNTR,   8
	.equ 	BLK_INDX, 	6
	.equ	BFL_INDX,  	4
	.equ	TEMP,		0
 

;............................................................................
; Complex FFT Calculation
;............................................................................
_IFFTComplex32bIP:
;............................................................................
; Context Save
			PUSH.D	w8						; {w8,w9} to TOS
			PUSH.D	w10						; {w10,w11} to TOS
			PUSH.D	w12						; {w12,w13} to TOS
			PUSH	CORCON
	.ifndef PSV_ON
			PUSH 	DSRPAG
	.else
			PUSH	PSVPAG
	.endif
			LNK     #22
;............................................................................
;;	PSVPAG = __builtin_PSVPAGe(&twiddleFactor);
;;	CORCONbits.PSV = 1;	
.ifndef PSV_ON

        	MOV	    #COEFFS_IN_DATA,W7	; W7 = COEFFS_IN_DATA
	        CP	    W7,W3				; W7 - W3
	        BRA	    Z,_NOPSV            ; IF W4 = COEFFS_IN_DATA
            NOP

        	; TWIDDLES IN FLASH
            MOVPAG W3,DSRPAG            ; SET DSRPAG TO PSV PAGE

         	; CHECK IF SUFFICIENT SPACE IS AVAILABLE IN STACK
        	; CONDITION (SP + TABLE SIZE + STACK_GUARD) < SPLIM
	        MOV    SPLIM,W5             ; LOAD STACK POINTER LIMIT
          	MOV    _STACK_GUARD,W13     ; STACK GUARD SPACE FOR INTERRUPTS ETC ...
	                            ; NOTE: THIS IS USER-DEFINABLE
        	MOV	   #TF_FFT_SIZE,W9		; 
          	SL     W9,W12               ; 
          	ADD    W9,W12,W9            ; TABLE_SIZE IN BYTES
          	ADD    W9,W13,W9            ; ADD _STACK_GUARD
           	BRA    C,_RUNFROMFLASH
	        ADD    W9,W15,W9            ; ADD SP
         	BRA    C,_RUNFROMFLASH

        	SUB    W9,W5,W13            ; CHECK AGAINST SPLIM
        	BRA    LTU,_COPYTOSTACK

_RUNFROMFLASH:
			PUSH	DSRPAG
			MOVPAG	#0x01,DSRPAG

; Store Input Parameters
			MOV		w1,[w14+BUF_PTR]	
			MOV		w2,[w14+WI_PTR]			; Wi = SIN(*)
			MOV		#TF_FFT_SIZE,W8
			ADD		w2,w8,w2
			MOV		w2,[W14+WR_PTR]			; Wr = COS(*)
			MOV		w0,[w14+STG_CNTR]
	

			MOV		#ACCAL,w12
			MOV		#ACCBL,w13

; Stage 1 to 2 unrolled for cycle efficiency
;............................................................................ 
			MOV		w1,w8
			ADD		w1,#8,w9
			ADD		w1,#16,w10
			ADD		w1,#24,w11
                                                                                        
     
			MOV		#1,w2		
			SL		w2,w0,w2				; N = 1<<logN2
			LSR		w2,#2,w2
			DEC		w2,w2

            MOV     #32,w0        			; w0=32
			MOV		w0,[w14+BFL_INDX]  

; Loop over butterflies in a block
;............................................................................
			DO      w2,STG12_END_PSV     
STG12_START_PSV:  
			STG12_MACRO
			MOV		[w14+BFL_INDX],w0
			ADD		w8,w0,w8				; Next Data Set
			ADD		w9,w0,w9				; Next Data Set
			ADD		w10,w0,w10				; Next Data Set
STG12_END_PSV:	ADD		w11,w0,w11				; Next Data Set
 ;............................................................................


; Stage 3 to LOG2N 
;............................................................................ 
; Butterfly counter for each block, Block counter for each stage and Stage Counter
; Block Index between P & Q inputs of Butterfly for Stage 3
; Twiddle Index for stage 3  
	
			MOV		#4,w0
			MOV		w0,[w14+BFL_CNTR]		; BFL_CNTR=4
			MOV		[w14+STG_CNTR],w1
			MOV		#1,w2		
			SL		w2,w1,w2				; N = 1<<logN2
			LSR		w2,#3,w2				; N/8
			MOV		w2,[w14+BLK_CNTR]		; BLK_CNTR=N/8	
		
			MOV		#(TF_FFT_SIZE/2),w2		
			MOV		w2,[w14+BFL_INDX]

			MOV		#32,w0
			MOV		w0,[w14+BLK_INDX]

			SUB		w1,#2,w1				; STG_CNTR=STG_CNTR-1
			MOV		w1,[w14+STG_CNTR]	


; Stage 1 to LOG2N 
;............................................................................ 
;			MOV		#1,w0
;			MOV		w0,[w14+BFL_CNTR]		; BFL_CNTR=1
;			MOV		[w14+STG_CNTR],w1
;			MOV		#1,w2		
;			SL		w2,w1,w2				; N = 1<<logN2
;			LSR		w2,#1,w2				; N/2
;			MOV		w2,[w14+BLK_CNTR]		; BLK_CNTR=N/2
;
;			MOV		#(TF_FFT_SIZE*2),w2		
;			MOV		w2,[w14+BFL_INDX]
;
;			MOV		#8,w0
;			MOV		w0,[w14+BLK_INDX]

	
; Loop over stages
;............................................................................
STG_START_PSV:                        
			MOV		[w14+BUF_PTR],w8		; Next Butterfly Block (Pr)
			MOV		[w14+BLK_INDX],w9	
			ADD		w8,w9,w9				; Next Butterfly Block (Qr)


			MOV		[w14+BLK_CNTR],w0		; Block loop count
			DEC 	w0,w0


; Loop over blocks in a stage
;............................................................................
			DO      w0,BLK_END_PSV
BLK_START_PSV:	
			MOV		[w14+WR_PTR],w10		; Initialise Twiddle Factor (Wr) pointer
			MOV		[w14+WI_PTR],w11		; Initialisae Twiddle Factor (Wi) pointer
			MOV		[w14+BFL_CNTR],w0		; Butterfly loop count
			DEC 	w0,w0
; Loop over butterflies in a block
;............................................................................
			DO      w0,BFL_END_PSV      
BFL_START_PSV:  
			BFLY_MACRO_PSV
			MOV		[w14+BFL_INDX],w0
			ADD		w10,w0,w10				; Next Twiddle Factor (Wr)
BFL_END_PSV:	ADD		w11,w0,w11				; Next Twiddle Factor (Wi)
;............................................................................            
			MOV		[w14+BLK_INDX],w0
			ADD		w8,w0,w8				; Next Butterfly Block (Pr)
BLK_END_PSV:	ADD		w9,w0,w9				; Next Butterfly Block(Qr)
 
;............................................................................      
			ADD		W14,#BFL_INDX,W0


			LAC		[w0],#1,A				; BFL_INDX = BFL_INDX/2
			SAC		A,[w0++]

			LAC		[w0],#-1,A				; BLK_INDX = BLK_INDX * 2
			SAC		A,[w0++]	



			LAC		[w0],#-1,A				; BFL_CNTR = BFL_CNTR * 2
			SAC		A,[w0++]	

			LAC		[w0],#1,A				; BLK_CNTR = BLK_CNTR/2
			SAC		A,[w0++]
			
			DEC		[w0],[w0]				; STG_CNTR = STG_CNTR - 1
			BRA     NZ,STG_START_PSV     
			
            BRA     _DONECPLXIFFT

_COPYTOSTACK:
			MOV     W15,W10
			
        	MOV	   #TF_FFT_SIZE,W9		; 
	        SL     W9,W12               ; 
         	ADD    W9,W12,W9            ; TABLE_SIZE IN WORDS
			
        	; COPY ENTIRE TWIDDLE TABLE TO STACK
	        DEC    W9,W13           ; REPEAT COUNT
	        REPEAT W13              ; SET REPEAT COUNT
	        MOV    [W2++],[w15++]   ; copy from PSV to stack
	
; Store Input Parameters
			MOV		w1,[w14+BUF_PTR]	
			MOV		w10,[w14+WI_PTR]			; Wi = SIN(*)
			MOV		#TF_FFT_SIZE,W8
			ADD		w10,w8,w10
			MOV		w10,[W14+WR_PTR]			; Wr = COS(*)
			MOV		w0,[w14+STG_CNTR]

			MOVPAG	#0x01,DSRPAG
			NOP
			BRA     _INPUTPARAMSSTORED

_NOPSV:
			MOVPAG	#0x01,DSRPAG

.else	
			MOV		w3,PSVPAG		
			BSET	CORCON,#2
.endif

; Store Input Parameters
			MOV		w1,[w14+BUF_PTR]	
			MOV		w2,[w14+WI_PTR]			; Wi = SIN(*)
			MOV		#TF_FFT_SIZE,W8
			ADD		w2,w8,w2
			MOV		w2,[W14+WR_PTR]			; Wr = COS(*)
			MOV		w0,[w14+STG_CNTR]
			
_INPUTPARAMSSTORED:
			MOV		#ACCAL,w12
			MOV		#ACCBL,w13

; Stage 1 to 2 unrolled for cycle efficiency
;............................................................................ 
			MOV		w1,w8
			ADD		w1,#8,w9
			ADD		w1,#16,w10
			ADD		w1,#24,w11
                                                                                        
     
			MOV		#1,w2		
			SL		w2,w0,w2				; N = 1<<logN2
			LSR		w2,#2,w2
			DEC		w2,w2

            MOV     #32,w0        			; w0=32
			MOV		w0,[w14+BFL_INDX]  

; Loop over butterflies in a block
;............................................................................
			DO      w2,STG12_END         
STG12_START:  
			STG12_MACRO
			MOV		[w14+BFL_INDX],w0
			ADD		w8,w0,w8				; Next Data Set
			ADD		w9,w0,w9				; Next Data Set
			ADD		w10,w0,w10				; Next Data Set
STG12_END:	ADD		w11,w0,w11				; Next Data Set
 ;............................................................................


; Stage 3 to LOG2N 
;............................................................................ 
; Butterfly counter for each block, Block counter for each stage and Stage Counter
; Block Index between P & Q inputs of Butterfly for Stage 3
; Twiddle Index for stage 3  
	
			MOV		#4,w0
			MOV		w0,[w14+BFL_CNTR]		; BFL_CNTR=4
			MOV		[w14+STG_CNTR],w1
			MOV		#1,w2		
			SL		w2,w1,w2				; N = 1<<logN2
			LSR		w2,#3,w2				; N/8
			MOV		w2,[w14+BLK_CNTR]		; BLK_CNTR=N/8	
		
			MOV		#(TF_FFT_SIZE/2),w2		
			MOV		w2,[w14+BFL_INDX]

			MOV		#32,w0
			MOV		w0,[w14+BLK_INDX]

			SUB		w1,#2,w1				; STG_CNTR=STG_CNTR-1
			MOV		w1,[w14+STG_CNTR]	


; Stage 1 to LOG2N 
;............................................................................ 
;			MOV		#1,w0
;			MOV		w0,[w14+BFL_CNTR]		; BFL_CNTR=1
;			MOV		[w14+STG_CNTR],w1
;			MOV		#1,w2		
;			SL		w2,w1,w2				; N = 1<<logN2
;			LSR		w2,#1,w2				; N/2
;			MOV		w2,[w14+BLK_CNTR]		; BLK_CNTR=N/2
;
;			MOV		#(TF_FFT_SIZE*2),w2		
;			MOV		w2,[w14+BFL_INDX]
;
;			MOV		#8,w0
;			MOV		w0,[w14+BLK_INDX]

	
; Loop over stages
;............................................................................
STG_START:                        
			MOV		[w14+BUF_PTR],w8		; Next Butterfly Block (Pr)
			MOV		[w14+BLK_INDX],w9	
			ADD		w8,w9,w9				; Next Butterfly Block (Qr)


			MOV		[w14+BLK_CNTR],w0		; Block loop count
			DEC 	w0,w0


; Loop over blocks in a stage
;............................................................................
			DO      w0,BLK_END
BLK_START:	
			MOV		[w14+WR_PTR],w10		; Initialise Twiddle Factor (Wr) pointer
			MOV		[w14+WI_PTR],w11		; Initialisae Twiddle Factor (Wi) pointer
			MOV		[w14+BFL_CNTR],w0		; Butterfly loop count
			DEC 	w0,w0
; Loop over butterflies in a block
;............................................................................
			DO      w0,BFL_END       
BFL_START:  
			BFLY_MACRO
			MOV		[w14+BFL_INDX],w0
			ADD		w10,w0,w10				; Next Twiddle Factor (Wr)
BFL_END:	ADD		w11,w0,w11				; Next Twiddle Factor (Wi)
;............................................................................            
			MOV		[w14+BLK_INDX],w0
			ADD		w8,w0,w8				; Next Butterfly Block (Pr)
BLK_END:	ADD		w9,w0,w9				; Next Butterfly Block(Qr)
 
;............................................................................      
			ADD		W14,#BFL_INDX,W0


			LAC		[w0],#1,A				; BFL_INDX = BFL_INDX/2
			SAC		A,[w0++]

			LAC		[w0],#-1,A				; BLK_INDX = BLK_INDX * 2
			SAC		A,[w0++]	



			LAC		[w0],#-1,A				; BFL_CNTR = BFL_CNTR * 2
			SAC		A,[w0++]	

			LAC		[w0],#1,A				; BLK_CNTR = BLK_CNTR/2
			SAC		A,[w0++]
			
			DEC		[w0],[w0]				; STG_CNTR = STG_CNTR - 1
			BRA     NZ,STG_START         

_DONECPLXIFFT:			
	
;............................................................................
; Context Restore
			ULNK
	.ifndef PSV_ON
			POP 	DSRPAG
	.else
			POP		PSVPAG
	.endif
			POP		CORCON
			POP.D	w12						; {w12,w13} from TOS
			POP.D	w10						; {w10,w11} from TOS
			POP.D	w8						; {w8,w9} from TOS
;............................................................................
			RETURN	


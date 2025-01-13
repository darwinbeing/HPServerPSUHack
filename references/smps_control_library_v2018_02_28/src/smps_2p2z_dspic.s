;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; (c) 2013 Microchip Technology Inc.
;
; MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
; derivatives created by any person or entity by or on your behalf, exclusively with
; Microchip?s products.  Microchip and its licensors retain all ownership and intellectual
; property rights in the accompanying software and in all derivatives here to.
;
; This software and any accompanying information is for suggestion only.  It does not
; modify Microchip?s standard warranty for its products.  You agree that you are solely
; responsible for testing the software and determining its suitability.  Microchip has
; no obligation to modify, test, certify, or support the software.
;
; THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT,
; MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION
; WITH MICROCHIP?S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
;
; IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
; (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY,
; CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL
; OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
; SOFTWARE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR
; THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL
; LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES,
; IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
;
; MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Local inclusions.
	.nolist
	.list

; Define data structure address offsets

	.equ    offsetACoefficients, 	0
	.equ    offsetBCoefficients, 	2
	.equ    offsetControlHistory,	4
	.equ	offsetErrorHistory, 	6
	.equ	offsetPreShift,		8
	.equ	offsetPostShift,        10
	.equ	offsetPostScaler, 	12
	.equ	offsetMinOutput,        14
	.equ	offsetMaxOutput,        16


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 ;    .section .libdsp, code			; use this section type when used as hidden library file
      .section .text				; use this section type for debugging

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _SMPS_Controller2P2ZUpdate:
; Prototype:
;    void SMPS_Controller2P2ZUpdate(SMPS_2P2Z_T* controllerData,
;                volatile uint16_t* controllerInputRegister,int16_t reference,
;                volatile uint16_t* controllerOutputRegister);
;
; Operation:
;                                          Wcp0   ( S + Wcz1 )
;  Calculate the transfer function Hc(s) = ---- X ------------------------
;                                           S     ( S + Wcp1 )( S + Wcp2 )
;    Error Normalization
;      ------------
;     |   prsft    |
;      ------------
;Reference   |       e(n)          e(n-1)        e(n-2)
;Input       |             --------      --------
;        -   -   -        |        |    |        |
;  o----|-|-|x|-|x|----o->| z^(-1) |-o->| z^(-1) |-o          Post
;        -   -   -     |  |        | |  |        | |         Scaling
;        |      NORM   |   --------  |   --------  |
;        |             |             |             |          --------
;Measured|            ---           ---           ---       |          |     - - - - - - - - - -
;Input   |       B0=>| X |     B1=>| X |     B2=>| X |      |posft     |    |                   |
;        |            ---           ---           ---       |postScalar|    |     P O W E R     |
;        |             |             |             |         ----------     |     P L A N T     |
;        |            -------------------------------------        |        |                   |
;        |           |                                     | U[n] ---       |                   |
;        |           |        A C C U M U L A T O R        |--o--| X |------O Duty Ratio=>PWM  |
;        |           |                                     |  |   ---       |                   |
;        |            -------------------------------------   |             | Voltage/Current   |
;        |             |              |                       |             |      Feedback     |
;        |            ---            ---                      |             |         O         |
;        |       A2=>| X |      A1=>| X |                     |             |         |         |
;        |            ---            ---                      |              - - - - - - - - - -
;        |             |              |                       |                      |
;        |             |   --------   |  --------             |                      | A/D
;        |             |  |        |  | |        |            |                      | Input
;        |             o--| z^(-1) |<-o-| z^(-1) |<-----------                       |
;        |                |        |    |        |                                   |
;        |                 --------      --------                                    |
;        |                  u[n-2]        u[n-1]                                     |
;        |                                                                           |
;        |                                                                           |
;         ---------------------------------------------------------------------------
;
;   controlHistory[n] = errorHistory[n]   * bCoefficients[0]
;                     + errorHistory[n-1] * bCoefficients[1]
;                     + errorHistory[n-2] * bCoefficients[2]
;                     + controlHistory[n-1] * aCoefficients[0]
;                     + controlHistory[n-2] * aCoefficients[1]
;
;  where:
;
;   aCoefficients[0]  = A1
;   aCoefficients[1]  = A2
;   bCoefficients[0]  = B0
;   bCoefficients[1]  = B1
;   bCoefficients[2]  = B2
;   controlHistory[0] = u(n)
;   controlHistory[1] = u(n-1)
;   errorHistory[0]   = e(n)	=> most recent error {(controlReference) - (sourceRegister)}
;   errorHistory[1]   = e(n-1)
;   errorHistory[2]   = e(n-2)
;   preShift          = prsft	=> pre scaling factor for Q15 normalization purposes
;   postShift         = posft	=> post scaling factor for normalization purposes
;   postScaler        = postScaler => post scaling coefficient for normalization purpose
;
;   Function call inputs:
;   sourceRegister    = ADBUFx  => measured feedback input   
;   controlReference  = V_ref	=> reference voltage
;   targetRegister = PDCx => output duty cycle
;
;  where:
;
;   aCoefficients, bCoefficients, controlHistory, errorHistory, 
;   preShift and postShift are all members of the data structure SMPS_2P2Z_T.
;
; Input:
;       w0 = Address of SMPS_2P2Z_T data structure
;       w1 = Address of the Source Register (Input)
;       w2 = Control Reference
;       w3 = Address of the Target Register (Output)
; Return:
;       w0 = Address of SMPS_2P2Z_T data structure
;       w1 = Address of the Source Register (Input)
;       w2 = Control Reference
;       w3 = Address of the Target Register (Output)
;
; System resources usage:
;       {w4..w5}        saved, used, restored
;       {w8,w10}        saved, used, restored
;        AccA           used, not restored
;        CORCON         saved, used, restored
;
; Note
;  This function requires CORCON register to be setup in a certain state
;  in order to operate correctly. Due to this requirement, this function
;  will save the CORCON register on the stack in the beginning of the
;  function and restore it before the function return.
;  After saving the CORCON register, this function writes to all bits of
;  the CORCON register. Thus, for the brief duration when this function is
;  executing, the state of CORCON register may be different from its state
;  as set by the function caller. This may change the CPU core behaviour with
;  respect to exception processing latency, DO loop termination, CPU interrupt
;  priority level and DSP-engine behaviour.
;............................................................................

        .global _SMPS_Controller2P2ZUpdate                    ; provide global scope to routine
_SMPS_Controller2P2ZUpdate:

        ; Save working registers.

        push	w4						; Save register W4
        push	w5						; Save register W5
        push    w8						; Save register W8
        push    w10						; Save register W10
        push    CORCON						; Save CORCON as it will be modified for fractional computation

        ; Set up DSP core for signed fractional operation, saturation on accumulator A
        ; and for write-back instructions from ACCx to WREGx,
        ; accumulator saturation mode is 1.31

        mov  #0x00A0, w4					; Load literal value 0x00A0 CORCON register: 0b 0000 0000 1010 0000
        mov  w4, _CORCON

		; Initialize source input, reference, error history and normalization variables

        mov [w0 + #offsetErrorHistory], w10			; w10 = Base Address of _ErrorHistory array  (e[n], e[n-1], e[n-2], e[n-3])
        mov [w0 + #offsetPreShift], w8				; Load error amplifier normalization bit-shift step

		; Update the error history

	mov     [w10 + #2], w5					; W5 = ErrorHistory[n-1]
        mov     w5, [w10 + #4]					; ErrorHistory[n-2] = W5
        mov     [w10 + #0], w5					; W5 = ErrorHistory[n]
        mov     w5, [w10 + #2]					; ErrorHistory[n-1] = W5

		; Calculate most recent error with normalization,

		sub w2, [w1], w5                   		; w5 = Reference - inputSource
		sl	w5, w8, w5				; shift error by PRE_SHIFT bit to the left (Q15 scaling)

		; Store most recent error to error history array
		mov 	w5, [w10]				; controller2P2Z.errorHistory[n] = w5

		; Initialize B coefficients array

        mov [w0 + #offsetBCoefficients], w8			; w8  = Base Address of _BCoefficients array (B0, B1, B2, B3)

        ; Calculate first section of 2P2Z controller -> Error History
        clr     a, [w8]+=2, w4, [w10]+=2, w5			; w4   = B0
								; w5   = e[0]
        mac     w4*w5, a, [w8]+=2, w4, [w10]+=2, w5		; ACCA = B0 * e[n]
								; w4   = B1, w5 = e[n-1]
        mac     w4*w5, a, [w8], w4, [w10], w5			; ACCA = B0 * e[n] + B1 * e[n-1]
								; w4   = B2, w5 = e[n-2]
        mac     w4*w5, a					; ACCA = B0 * e[n] + B1 * e[n-1] + B2 * e[n-2]

       ; Initialize A coefficients and control history arrays

		mov 	[w0 + #offsetACoefficients],  w8	; w8  = Base Address of _ACoefficients array  (A1, A2, A3)
		mov 	[w0 + #offsetControlHistory], w10	; w10 = Base Address of _ControlHistory array (y[n-1], y[n-2], y[n-3])

        ; Calculate second section of 2P2Z controller - Controller History

		movsac	a, [w8]+=2, w4, [w10]+=2, w5		; w4  = A1, w5 = y[n-1]
		mac     w4*w5, a, [w8], w4, [w10], w5		; ACCA = A1 * y[n-1] + B0 * e[n] + B1 * e[n-1] + B2 * e[n-2]
								; w4   = A2, w5 = y[n-2]
		mac     w4*w5, a				; ACCA = A1 * y[n-1] + A2 * y[n-2] + B0 * e[n] + B1 * e[n-1] + B2 * e[n-2]

        ; Initialize Scale-factor and multiply
		sac.r   a, w4					; w4 = Sat(Rnd(ACCAH))
		mov 	[w0 + #offsetPostScaler],  w5		; w5 = postScaler
		mpy	w4*w5, a				; Multiply control output and scale-factor
                              	

	; Backwards normalization & write back
		mov	[w0 + #offsetPostShift], w5		; w5 = Normalization Shift to compensate coefficient scaling
		sftac	a, w5					; Backward normalization to compensate coefficient scaling
		sac.r   a, w4                                   ; w4 = Sat(Rnd(ACCAH))	
		
		
		; Update the controller output history on the delay line

	mov     [w10 + #-2], w5					; W5 = ControlHistory[n-1]
        mov     w5, [w10]			                ; ControlHistory[n-2] = W5
        mov     w4, [w10 + #-2]			                ; ControlHistory[n]   = w2

		; Clamp controller output to min/max values if needed
        mov 	[w0 + #offsetMinOutput], w5
	cpsgt	w4, w5
        mov.w   w5, w4
        mov 	[w0 + #offsetMaxOutput], w5
	cpslt	w4, w5
        mov.w   w5, w4

		; Controller write back into target register
	mov w4, [w3]						; write result into target register

		; restore registers

        pop     CORCON						; restore CORCON.
        pop     w10						; restore working register W10
        pop     w8						; restore working register W8
        pop     w5						; restore working register W5
        pop     w4						; restore working register W4
        pop.s							; restore working registers W0...W3


        return

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; _SMPS_Controller2P2ZInitialize:
;
; Prototype:
;      void SMPS_Controller2P2ZInitialize( SMPS_2P2Z_T *controllerData )
;
; Operation: This routine clears the delay line elements in the arrays
;            _ControlHistory and _ErrorHistory.
;
; Input:
;       w0 = Address of data structure SMPS_2P2Z_T (type defined in smps_control.h)
;
; Return:
;       (void)
;
; System resources usage:
;       w0      saved, used, restored
;
;............................................................................

	.global _SMPS_Controller2P2ZInitialize          ; provide global scope to routine
        
_SMPS_Controller2P2ZInitialize:
      							
		; Clear controlHistory variables
        push	w0					; Save w0
        mov     [w0 + #offsetControlHistory], w0	; Set up pointer to the base of 
		                                        ; controlHistory variables within SMPS_2P2Z_T
        clr     [w0++]					; ControlHistory[n-1] = 0
        clr     [w0]					; ControlHistory[n-2] = 0
	pop	w0					; Restore pointer to base of SMPS_2P2Z_T

        ; Clean up errorHistory variables
        push	w0					; Save w0
        mov [w0 + #offsetErrorHistory], w0		; w0 = Address of _ErrorHistory array (e[n], e[n-1], e[n-2], e[n-3])

        clr     [w0++]					; ErrorHistory[n]   = 0
        clr     [w0++]					; ErrorHistory[n-1] = 0
        clr     [w0]					; ErrorHistory[n-2] = 0
        pop     w0					; Restore pointer to base of SMPS_2P2Z_T


        return
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        .end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; EOF


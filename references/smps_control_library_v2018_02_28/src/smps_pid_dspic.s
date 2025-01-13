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
    .equ    offsetabcCoefficients,  0
    .equ    offsetErrorHistory,     2
    .equ    offsetControlHistory,   4 
    .equ    offsetPostScaler,	    6
    .equ    offsetPreShift,	    8
    .equ    offsetPostShift,        10
    .equ    offsetMinOutput,        12
    .equ    offsetMaxOutput,        14


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  ;    .section .libdsp, code		        ; use this section type when used as hidden library file
        .section .text				; use this section type for debugging

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _cPID16b_v1 :
; Prototype:
;      void SMPS_ControllerPIDUpdate(SMPS_PID_T* controllerData,
;                 volatile uint16_t* controllerInputRegister,int16_t reference,
;                 volatile uint16_t* controllerOutputRegister);
; Operation:
;
;                                             ----   Proportional
;                                            |    |  Output
;                             ---------------| Kp |-----------------
;                            |               |    |                 |
;                            |                ----                  |
;Reference                   |                                     ---
;Input         ---           |           --------------  Integral | + | Control   -------
;     --------| + |  Control |          |      Ki      | Output   |   | Output   |       |
;             |   |----------|----------| ------------ |----------| + |----------| Plant |--
;        -----| - |Difference|          |  1 - Z^(-1)  |          |   |          |       |  |
;       |      ---  (error)  |           --------------           | + |           -------   |
;       |                    |                                     ---                      |
;       | Measured           |         -------------------  Deriv   |                       |
;       | Outut              |        |                   | Output  |                       |
;       |                     --------| Kd * (1 - Z^(-1)) |---------                        |
;       |                             |                   |                                 |
;       |                              -------------------                                  |
;       |                                                                                   |
;       |                                                                                   |
;        -----------------------------------------------------------------------------------
;
;   controlOutput[n] = controlHistory[n-1]
;                    + ErrorHistory[n] * abcCoefficients[0]
;                    + ErrorHistory[n-1] * abcCoefficients[1]
;                    + ErrorHistory[n-2] * abcCoefficients[2]
;
;  where:
;   abcCoefficients[0] = Kp + Ki + Kd
;   abcCoefficients[1] = -(Kp + 2*Kd)
;   abcCoefficients[2] = Kd
;   ErrorHistory[n] = referenceInput[n] - measuredInput[n]
;   controlHistory = > control calculated output[n-1]
;   preShift          = prsft	=> pre scaling factor for Q15 normalization purposes
;   postShift         = posft	=> post scaling factor for normalization purposes
;   postScaler        = postScaler => post scaling coefficient for normalization purpose
;
;   Function call inputs:
;   sourceRegister    = ADBUFx  => measured feedback input   
;   controlReference  = V_ref	=> reference voltage
;   targetRegister = PDCx => output duty cycle
;
;   where:
;   abcCoefficients, controlHistory, controlOutput, controlInput and controlReference
;   are all members of the data structure SMPS_PID_T.
;
; Input:
;       w0 = Address of SMPS_PID_T data structure
;       w1 = Address of the Source Register (Input)
;       w2 = Control Reference
;       w3 = Address of the Target Register (Output)
; Return:
;       w0 = Address of SMPS_PID_T data structure
;       w1 = Address of the Source Register (Input)
;       w2 = Control Reference
;       w3 = Address of the Target Register (Output)
;
; System resources usage:
;       {w4..w5}        used, not restored
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


        .global _SMPS_ControllerPIDUpdate			; provide global scope to routine
_SMPS_ControllerPIDUpdate:

   ; Save working registers.
        push	w4						; Save register W4
        push	w5						; Save register W5
        push    w8						; Save register W8
        push    w10						; Save register W10
        push    CORCON						; Save CORCON as it will be modified for fractional computation

   ; Set up DSP core for signed fractional operation, saturation on accumulator A
   ; and for write-back instructions from ACCx to WREGx,
   ; accumulator saturation mode is 1.31

        mov  #0x00A0, w4					; Load literal value 0x00A0 to CORCON register: 0b 0000 0000 1010 0000
        mov  w4, _CORCON

  ; Initialize source input, reference, error history and normalization variables

        mov [w0 + #offsetErrorHistory], w10			; w10 = Address of _ErrorHistory array (state/delay line)
        mov [w0 + #offsetPreShift], w8				; Load error amplifier normalization bit-shift step

   ; Calculate most recent error with normalization,

		sub w2, [w1], w5				; w5 = Reference - inputSource
		sl	w5, w8, w5				; shift error by PRE_SHIFT bit to the left (Q15 scaling)

   ; Store most recent error to error history array
		mov 	w5, [w10]				; controllerPID.errorHistory[n] = w5

   ; Initialize coefficients and history array

        mov [w0 + #offsetabcCoefficients], w8			; w8 = Base Address of _abcCoefficients array [(Kp+Ki+Kd), -(Kp+2Kd), Kd]
        mov [w0 + #offsetControlHistory], w2			; w1=ControlOutput[n-1], load previous control output

   ; Calculate PID Control Output
        clr     a, [w8]+=2, w4, [w10]+=2, w5			; w4 = (Kp+Ki+Kd), w5 = _ErrorHistory[n]
        lac     w2, a						; A = ErrorOutput[n-1]
        mac     w4*w5, a, [w8]+=2, w4, [w10]+=2, w5		; A += (Kp+Ki+Kd) * _ErrorHistory[n]
								; w4 = -(Kp+2Kd), w5 = _ErrorHistory[n-1]
        mac     w4*w5, a, [w8], w4, [w10]-=2, w5		; A += -(Kp+2Kd) * _ErrorHistory[n-1]
								; w4 = Kd, w5 = _ErrorHistory[n-2]
        mac     w4*w5, a, [w10]-=2, w5				; A += Kd * _ErrorHistory[n-2]
								; w5 = _ErrorHistory[n-1]
								; w10 = &_ErrorHistory[n-2]
        sac.r   a, w5						; ControlOutput[n] = Sat(Rnd(A))

    ; Copy control output to structure (non-array)
		mov w5, [w0 + #offsetControlHistory]

	; Initialize Scale-factor and multiply
		sac.r   a, w4					; w4 = Sat(Rnd(ACCAH))
		mov 	[w0 + #offsetPostScaler],  w5		; w5 = postScaler
		mpy	w4*w5, a				; Multiply control output and scale-factor
		

	; Backwards normalization & write back
		mov	[w0 + #offsetPostShift], w8		; w8 = Normalization Shift to compensate coefficient scaling
		sftac	a, w8					; Backward normalization to compensate coefficient scaling
		sac.r   a, w4                                   ; w4 = Sat(Rnd(ACCAH))

        

     ; Clamp controller output to min/max values if needed
        mov 	[w0 + #offsetMinOutput], w5
	cpsgt	w4, w5
        mov.w   w5, w4
        mov 	[w0 + #offsetMaxOutput], w5
	cpslt	w4, w5
        mov.w   w5, w4

     ; Controller write back into target register
	mov w4, [w3]						; write result into target register

    ;Update control output history on the delay line
	mov     [w10 + #2], w5					; W5 = ErrorHistory[n-1]
        mov     w5, [w10 + #4]					; ErrorHistory[n-2] = W5
        mov     [w10], w5					; W5 = ErrorHistory[n]
        mov     w5, [w10 + #2]					; ErrorHistory[n-1] = W5


        pop     CORCON						; restore CORCON.
        pop     w10						; Restore working registers.
        pop     w8
        pop     w5
        pop     w4

        return




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; _SMPS_ControllerPIDInitialize:
;
; Prototype:
; void SMPS_PIDInitialize( SMPS_PID_T *controller_data )
;
; Operation: This routine clears the delay line elements in the arrays
;            _ControlHistory and _ErrorHistory, as well as clears the current
;            control output element, _ControlOutput
;
; Input:
;       w0 = Address of data structure SMPS_PID_T (type defined in smps_control.h)
;
; Return:
;       (void)
;
; System resources usage:
;       w0      saved, used, restored
;
;............................................................................

	.global _SMPS_ControllerPIDInitialize			; provide global scope to routine

_SMPS_ControllerPIDInitialize:

		; Clean up most recent controller output
        push    w0
        add     #offsetControlHistory, w0 ;clear controlOutput
        clr     [w0]
        pop     w0

        ; Clean up errorHistory variables
        push	w0						;Set up pointer to the base of
        mov     [w0 + #offsetErrorHistory], w0			; w0 = Address of _ErrorHistory array (e[n], e[n-1], e[n-2])
        clr     [w0++]						; ErrorHistory[n]   = 0
        clr     [w0++]						; ErrorHistory[n-1] = 0
        clr     [w0]						; ErrorHistory[n-2] = 0
        pop     w0						;Restore pointer to base of SMPS_PID_T
        return

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        .end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; EOF


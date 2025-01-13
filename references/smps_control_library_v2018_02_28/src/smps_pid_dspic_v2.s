
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; © 2015 Microchip Technology Inc.
;
; MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
; derivatives created by any person or entity by or on your behalf, exclusively
; with Microchip?s products.  Microchip and its licensors retain all ownership
; and intellectual property rights in the accompanying software and in all
; derivatives here to.
;
; This software and any accompanying information is for suggestion only.  It
; does not modify Microchip?s standard warranty for its products.  You agree
; that you are solely responsible for testing the software and determining its
; suitability.  Microchip has no obligation to modify, test, certify, or
; support the software.
;
; THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
; EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
; WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
; PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP?S PRODUCTS,
; COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
;
; IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
; (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
; INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
; EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
; ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
; MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
; TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
; CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
; FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
;
; MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
; TERMS.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Local inclusions.
	.nolist
	.list


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;     .section .libdsp, code		; use this section type when used as hidden library file
       .section .text			; use this section type for debugging

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
    ; Structure defined offsets:

   .equ delay,            2
   .equ triggerRegister,  4
   .equ periodRegister,   6


;  _SMPS_ControllerPIDUpdate_HW_Accel:
;
;  Prototype:
;
;            void SMPS_ControllerPIDUpdate_HW_Accel(void);
;
;                                          _               _
;                                         |       Ki        |
;  Calculate the transfer function H(s) = | Kp + --- + s*Kd | * E(s)
;                                         |       s         |
;                                          -               -
;
;
;                                      -
;                                     |A|                                                                                               System Plant
;                                      -                                                  			                      - - - - - - - - - - - - - - - - - - - - - - -
;Reference                             |                                                   			                     |                                             |
;Input                                 |                      -------------        ------------                          |     ------                                  |
;        +     -  e[n]  ----------     -  e[n]*A  _          |  postShift  |      | postScalar |        ---------        |    |      |                 Voltage/Current |
;  o----------|+|------|-preShift |-->|x|------->|+|-------->|  Rnd(ACCA)  |----->| Rnd(ACCA)  |------>|Clamping |------>0--->| PDCx |                   Feedback      0 ----
;              -   |    ----------     -          -     |     -------------        ------------      	---------        |    |      |                                 |     |
;        -     |   |                              |     |                                  			                     |     ------                                  |     |
;              |   |                              |     |          -----------             			                     |	                                           |     |
;              |   |                              |      >--------| Rnd(ACCA) |-------------->     			              - - - - - - - - - - 0 - - - - - - - - - - - -      |
;              |   |                              |                -----------                |                                               |                              |
;              |   |                               -----------------------------------        |                                               |                              |
;              |   |                                                                  |       |                                               |                              |
;              |   |                                                                  |       |                                               |                              |
;  Measured    |   |             ------     e[n-1]      ------    e[n-2]              |       |                                               | (Selectable by user)         |
;  Input       |    ----------->| z^-1 |-------------->| z^-1 |-----------            |       |                                               |                              |
;              |                 ------       |         ------            |           |       |                                 ----------    |     ---------                |
;              |                              |                           |           |       |                                |  Trigger |<------>| Trigger |               |
;              |                              -                           -           |       |                                | Off-Time |        | On-Time |               |
;              |                       B --->|x|                  C ---->|x|          |       |                                 ----------          ---------                |
;              |                              -                           -           |       |                                       |                  |                   |
;              |                              |                           |           |       |                                       |                  |                   |
;              |                              |                           |           |       |                                       |                  |                   |
;              |                   ----------------------------------------------     |       |                                        -------->-<-------                    |
;              |                  |                                              |    |       |                                                 |                            |
;              |                  |            A C C U M U L A T O R             |--->        |                                                 |                            |
;              |                  |                                              |            |                                                 |                            |
;              |                   ----------------------------------------------             |                                              -------                         |
;              |                               |                                              |                                             | TRIGx |  (Update trigger)      |
;              |                               |                                              |                                              -------   (register)            |
;              |                                <--------------------------------------------<                                                                               |
;              |									               u[n-1]	    																				             |
;              |																																				             |
;              |																			Measured Output 								                                 |
;               -------------------------------------------------------------------------------------------------------------------------------------------------------------
;
;    PID Function:
;                 u[n] = u[n-1] + e[n]*A + e[n-1]*B + e[n-2]*C
;
; Outputs:
;              u[n]   = Current output
;              u[n-1] = Previous output
;
; Errors
; calculated:
;              e[n]   = current error
;              e[n-1] = previous error
;              e[n-2] = error from two iterations ago
; PID Values:
;              KA = xPIDCoefficients[0] = Kp + Ki + Kd
;              KB = xPIDCoefficients[1] = -(Kp + 2Kd)
;              KC = xPIDCoefficients[2] = Kd
;
; where 'x' is name of converter, i.e., buck, boost, etc.
;
;
;   preShift   = pre-scaling factor for normalization purposes
;   postShift  = backward normalization to compensate coefficient scaling
;   postScaler = post-scaling coefficient for normalization scaling
;
;   Function call inputs:
;   none
;
;   Note 1:
;   w0 is used to read in the converter voltage reference value just prior to the library function
;   call.  After the input error has been calculated, it may be used freely within the library.  This
;   is done so that at least one working register is available inside the ISR at all times.  If register
;   w14 is not preloaded with any data, then one more register would be available by the user at all times.
;
;   Note 2:
;   ABC Coefficients are preloaded into their respective array. The preShift, postShift, postScalar,
;   min/max clamping values, source register address, and target register address are all preloaded
;   into their respective Alt-W registers at system initialization. See resource usage definition below.
;
;   Alternate Working Register System resource usage:
;   ------------------------------------------------
;
;       w0  = Control Reference value
;       w1  = Address of the Source Register (Input)  - ADCBUFx
;       w2  = Address of the Target Register (Output) - PDCx
;       w3  = ACCAL ... and misc operands
;       w4  = misc operands
;       w5  = ACCAU/MAC ... and misc operands
;       w6  = postScalar
;       w7  = postShift
;       w8  = structure pointer
;       w9  = PID Coefficients array pointer
;       w10 = PID errorHistory array pointer
;       w11 = minClamp
;       w12 = maxClamp
;       w13 = preShift
;       w14 = ACCAH/MAC ... and misc operands
;
;
;   Note 3:
;   The library comes with the option of configuring the TRIGx value in real time.
;   The options that are included are duty cycle on time trigger (On-Time Trigger) and
;   duty cycle off time trigger (Off-Time Trigger).  The following two equations show
;   how these values are computed:
;
;   On-Time Trigger:  TRIGx = PDCx  + Delay
;                             ----
;                              2
;
;   Off-Time Trigger: TRIGx = PDCx  +  (PTPER - PDCx) + Delay
;                                       ------------
;                                            2
;
;   In order to make use of this library option, the library options structure must be initialized.
;   Please refer to the smps_control.h file for further details.
;
;   Note 4:
;   This function requires CORCON register to be setup in a certain state in order to operate correctly.
;   Due to this requirement, this function will save the CORCON register on the stack in the beginning of
;   the function and restore it before the function returns.  After saving the CORCON register, this
;   function writes to all bits of the CORCON register. Thus, for the brief duration when this function is
;   executing, the state of CORCON register may be different from its state as set by the function caller.
;   This may change the CPU core behaviour with respect to exception processing latency, DO loop termination,
;   CPU interrupt priority level and DSP-engine behaviour.
;
;............................................................................

        .global _SMPS_ControllerPIDUpdate_HW_Accel   ; Provide global scope to routine

  _SMPS_ControllerPIDUpdate_HW_Accel:

      push    CORCON      ; Prepare CORCON for fractional computation.

	  ; Load accumulator with results from previous iteration
	  
      mov w3, ACCAL
      mov w14, ACCAH
      mov w5, ACCAU

      mov  #0xA0, w4     ; Load literal value 0x00A0 to CORCON register: 0b 0000 0000 1010 0000
      mov  w4, _CORCON

      sub w0, [w1], w5   ; w5 = Control Reference - measured output ( w5 = w0 - [w1] )
      sl  w5, w13, w5	 ; 'preShift' e[n] by w13 (preShift) value and store result to w5 - (for Q15 scaling)
			 ; w5 = e[n]
      mov w5, [w10]      ; Store most recent error to errorHistory array

      movsac a,     [w9]+=2, w4, [w10]+=2, w5    ; ACCA = previous iteration results prior to shifting or clamping
      	                                         ; w4 = KA, w5 = e[n]

      mac w4*w5, a, [w9]+=2, w4, [w10]+=2, w5    ; ACCA = u[n-1] + e[n]*KA
                                                 ; w4 = KB, w5 = e[n-1]

      mac w4*w5, a, [w9],    w4, [w10],    w5    ; ACCA = u[n-1] + e[n-1]*KB
                                                 ; w4 = KC, w5 = e[n-2]

      mac w4*w5, a                               ; ACCA = u[n-1] + e[n]*KA + e[n-1]*KB + e[n-2]*KC

      ; Save accumulator for next iteration

      mov ACCAL, w3
      mov ACCAH, w14
      mov ACCAU, w5

      ; Initialize Scale-factor and multiply
      sac.r a, w4	   ; w4 = Rnd(ACCAH)
      mpy w4*w6, a	   ; Multiply control output with 'postScalar' value
	  
	  
	  ; Backwards normalization & write back
      sftac a, w7        ; 'postShift' backward normalization to compensate coefficient scaling
      sac.r a, w4        ; w4 = Rnd(ACCAH)

      ; Clamp to minimum if needed
      cpsgt w4, w11       ; Check if u[n] > minClamp.  If true, the next instruction is discarded and a Nop() is executed
                          ; If not true, execute next instruction
      mov   w11, w4       ; Update u[n] with minClamp value

      ; Clamp to maximum if needed
      cpslt w4, w12       ; Check if u[n] < maxClamp. If true, the next instruction is discarded and a Nop() is executed
                          ; If not true, execute next instruction
      mov   w12, w4       ; Update u[n] with maxClamp value

      mov w4, [w2]        ; Update the target register (Output):  [w2] = PDCx/CMPxDAC

      ;   ---------------      End of critical path    		----------------------------------------------

      ; Update errorHistory output on the delay line

	  mov [--w10], w14         ; w14 = e[n-1]
	  mov w14, [w10 + #2]      ; e[n-2] = w14
	  mov [--w10], w14         ; w14 = e[n]
	  mov w14, [w10 + #2]      ; e[n-1] = w14

          sub  w9,  #0x04, w9      ; Reset PID coefficients array pointer

	  ; Trigger options check:

	  btsc [w8], #0               ; Check if trigger On-Time is ENABLED
	  bra triggerOnTime           ; if so, go to TriggerOnTime function, else skip

	  btsc [w8], #1               ; Check if trigger Off-Time is ENABLED
	  bra triggerOffTime	      ; if so, go to TriggerOffTime function

	  bra triggerBypass           ; else go directly to end of function call

      ; Trigger functions:

 triggerOffTime:

      mov [w8 + #periodRegister], w14    ;  Load address of time base to working register w14
      mov [w14], w14                     ;  Move the contents of the address (PTPER) to w14
      sub w14, w0, w14                   ;  Subtract duty cycle from time base and store result to w14
      asr w14, #1, w14                   ;  Right shift result previous results by literal value
      add w14, w0, w14                   ;  Duty cycle + 50% off time (w0 + w14) and store result to w14
      mov [w8 + #delay], w0              ;  Move delay value to w0
      add w14, w0, w14                   ;  Add delay to account for gate drive delay
      mov [w8 + #triggerRegister], w0    ;  Load address of target register
      mov w14, [w0]                      ;  Update the Trigger target register

	 bra triggerBypass

 triggerOnTime:

      asr w0, #1, w14                   ; Right-shift PDCx by literal and store value in w14
      mov [w8 + #delay], w0             ; Load delay value to register w0
      add w14, w0, w14                  ; Add delay to account for gate drive delay
      mov [w8 + #triggerRegister], w0   ; Load address of target register
      mov w14, [w0]                     ; Update the Trigger target register

      ; End of trigger options

 triggerBypass:

      pop CORCON           ; restore CORCON.

      return               ; End of function

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

         .end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; EOF


		
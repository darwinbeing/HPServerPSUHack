;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; © 2019 Microchip Technology Inc. and its subsidiaries.
;;
;; MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: Subject to your compliance with 
;; these terms, you may use Microchip software and any derivatives exclusively 
;; with Microchip products. It is your responsibility to comply with third party
;; license terms applicable to your use of third party software (including open 
;; source software) that may accompany Microchip software.
;;
;; THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
;; EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
;; IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
;; FOR A PARTICULAR PURPOSE.
;;
;; IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
;; INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
;; WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
;; HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
;; THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
;; CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
;; OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
;; SOFTWARE.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


; Local inclusions.
	.nolist
	.list

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 ;      .section .libdsp, code		; use this section type when used as hidden library file
        .section .text			; use this section type for debugging

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;

    ; Structure defined offsets:
	
   .equ delay,            2
   .equ triggerRegister,  4
   .equ periodRegister,   6


; _SMPS_Controller2P2ZUpdate_HW_Accel:

; Prototype:

;            void SMPS_Controller2P2ZUpdate_HW_Accel(void);
;
; Operation:
;                                          Wcp0   ( S + Wcz1 )
;  Calculate the transfer function Hc(s) = ---- X ------------
;                                           S     ( S + Wcp1 )
;
;
;
;                                  --                                                   ----------
;                                 |Bo|                                                 |postScalar|
;                                  --                                                   ----------
;Reference                          |                                                       |                                                    ------------------
;Input                              |                                                       |                                                   |   SYSTEM PLANT   |
;        -   e[n]                   |   e[n]*Bo   -      ---------       -------------      |           ------------       --------    u[n]     |                  |
;  o----|-|----------------------->|x|---------->|+|--->|postShift|---->| Rnd(ACCAH)  |--->|x|-------->| Rnd(ACCAH) |---->|Clamping|---------- >0   PDCx Update    |   
;        -    |                     -             -      ---------       -------------      -           ------------   |   --------     |       |                  |
;        |    |                                   ^                                                                    |                |       |  Voltage/Current |
;        |    |                                   |                                                                    |                |       |    Feedback      |
;        |    |  				  |								       |	        |       |       |          |
;	 |    |					   <-----------------------------<    		u[n] pre-clamping      |	        |       |       |          |
;	 |    |								 	  |	     <------------------------< 	        |        -------0----------
;	 |    |									  |	    |					        |               |
;	 |    |					                                  | 	    |					        |               |
;        |    |                                                                   |         |                       --------------------                |
;        |    |                                                                   |         |                      |                                    |
;Measured|    |         ------     e[n-1]      ------    e[n-2]                   |         |                      | (Selectable by user)               |
;Input   |     ------->| z^-1 |-------------->| z^-1 |-----------                 |         |                      |                                    |
;        |              ------       |         ------            |                |         |        ----------    |     ---------                      |
;        |                           |                           |                |         |       |  Trigger |<------>| Trigger |                     |
;        |                           -                           -                |         |       | Off-Time |        | On-Time |                     |
;        |                   B1 --->|x|                 B2 ---->|x|               |         |        ----------          ---------                      |
;        |                           -                           -                |         |             |                  |                          |
;        |                           |                           |                |         |             |                  |                          |
;        |                           |                           |                |         |             |                  |                          |
;        |                     ----------------------------------------------     |         |              -------->-<-------                           |
;        |                    |                                              |    |         |                       |                                   |
;        |                    |            A C C U M U L A T O R             |--->          |                       |                                   |
;        |                    |                                              |              |                       |                                   |
;        |                     ----------------------------------------------               |                   ---------                               |
;        |                           |                           |                          |                  |  TRIGx  |                              |
;        |                           |                           |                          |                   ---------                               |
;        |                          |x|<---- A2                 |x|<---- A1                 |                                                           |
;        |                           -                           -                          |                                                           |
;        |                           |                           |                          |                                                           |
;        |                           |     ------                |    ------                |                                                           |
;        |                            ----| z^-1 |<------------------| z^-1 |<-------------<                                                            |
;        |                       u[n-2]    ------              u[n-1] ------                                                                            |
;        |                                   													        |
;        |                                    													        |
;         <--------------------------------------------------------------------------------------------------------------------------------------------<
;                                                                                                    Measured Output
;
;   2P2Z Compensator Function:
;
;   u[n] = B0*e[n] + B1*e[n-1] + B2*e[n-2] + A1*u[n-1] + A2*u[n-2] 
;					  
;
;  where:
;
;   xABCoefficients[0]  = B0
;   xABCoefficients[1]  = B1
;   xABCoefficients[2]  = B2

;   xABCoefficients[3]  = A1
;   xABCoefficients[4]  = A2

;   xErrorControlHistory[0] = e[n-1]
;   xErrorControlHistory[1] = e[n-2]

;   xErrorControlHistory[2] = u[n-1]   
;   xErrorControlHistory[3] = u[n-2] 

;   where 'x' is for the converter name
; 
;   Note that e[n] and u[n] are obtained during the critical path, prior to the MAC
;   instructions and thus not shown in the array assignments above.  After the critical
;   path, they become e[n-1] and u[n-1] for the subsequent iteration.
;
;   postShift  = post scaling factor for normalization purposes
;   postScaler = post scaling coefficient for normalization purpose
;
;   Function call inputs:
;   none
;
;   Note 1: 
;   w0 is used to read in the converter voltage reference value just prior to the library function
;   call.  After the input error has been calculated, it may be used freely within the library.  This 
;   is done so that at least one working register is available inside the ISR at all times.  If registers 
;   w13 and w14 are not preloaded with any data, then two more registers would be available by the user
;   at all times.

;   Note 2:
;   (A-Coefficients)/(B-Coefficients) are preloaded into their respective array. The postShift,
;   postScalar, min/max clamping values, source register address, and target register address
;   are all preloaded into their respective Alt-W registers at system initialization. See resource 
;   usage definition below.
;
;   Alternate Working Register System resource usage:
;
;       w0  = Control Reference value
;       w1  = Address of the Source Register (Input)  - ADCBUFx
;       w2  = Address of the Target Register (Output) - PDCx/CMPxDAC
;       w3  = ACCAL ... and misc operands
;       w4  = ACCAH ... and misc operands
;       w5  = ACCAU ... and misc operands
;       w6  = postScalar
;       w7  = postShift
;       w8  = Library options structure pointer
;       w9  = ACoefficients/BCoefficients array base address { B0, B1, B2, A1, A2 }
;       w10 = ErrorHistory/ControlHistory array base address { e[n-1], e[n-2], u[n-1], u[n-2] }
;       w11 = minclamp
;       w12 = maxClamp
;       w13 = user defined, misc use
;       w14 = user defined, misc use
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
;;  Off-Time Trigger: TRIGx = PDCx  +  (PTPER - PDCx) + Delay
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
;............................................................................


    .global _SMPS_Controller2P2ZUpdate_HW_Accel        ; Provide global scope to routine

  _SMPS_Controller2P2ZUpdate_HW_Accel:

    push    CORCON     ; Save CORCON as it will be modified for fractional computation
                       ; 1.31 saturation
					   
    ; Immediately load previous ACCUMULATOR results to free up Alt-W registers w3, w4, & w5 for misc. instructions

    mov w3, ACCAL     ; Load bits 15:0
    mov w4, ACCAH     ; Load bits 31:16
    mov w5, ACCAU     ; Load bits 39:32

    mov  #0xA0, w4    ; Load literal value 0x00A0 to CORCON register: 0b 0000 0000 1010 0000
    mov  w4, _CORCON

    sub w0, [w1], w5  ; w5 = Control Reference - measured output  ( w5 = w0 - [w1] )

    mov w5, [w10]     ; Store most recent error; becomes e[n-1] for next iteration

    mov [w9++], w4    ; w4 = B0
    mac w4*w5,  a     ; ACCA = B0 * e[n] + Previously ACCA stored results


    sac.r a, #0, w4   ; w4 = Rnd(ACCAH) - this is the control output value from previous
                      ; line of code after having been shifted.  Store bits 31:16 of the ACCA
                      ; into w4 temp register

    mpy   w4*w6, a    ; Multiply control output (after rounding) and postScalar factor
    
    sftac a, w7       ; w7 = Normalization shift value to compensate coefficient scaling
                      ; ACC = ACC>>PostShift or ACC = ACC<<PostShift depending on sign of value
    
    sac.r a, w4       ; w4 = Rnd(ACCAH) again after normalization

    ; Save control output to control history array prior to clamping

    mov w4, [w10 + #0x4]    ; w4 = u[n] ( becomes u[n-1] for next calculation )

     ; Clamp to minimum if needed
    cpsgt w4, w11     ; Check if u[n] > minClamp.  If true, the next instruction is discarded and a Nop() is executed
                      ; If not true, execute next instruction
    mov.w w11, w4     ; Update u[n] with minClamp value

    ; Clamp to maximum if needed
    cpslt w4, w12     ; Check if u[n] < maxClamp. If true, the next instruction is discarded and a Nop() is executed
                      ; If not true, execute next instruction
    mov.w w12, w4     ; Update u[n] with maxClamp value


    mov w4, [w2]      ; Update the target register (Output):  [w2] = PDCx/CMPxDAC

    ; ------------------------------- End of Critical Path ------------------------------------------ ;

    ; Trigger options check:

    btsc [w8], #0               ; Check if trigger On-Time is ENABLED
    bra triggerOnTime           ; if so, go to TriggerOnTime function, else skip

    btsc [w8], #1               ; Check if trigger Off-Time is ENABLED
    bra triggerOffTime	        ; if so, go to TriggerOffTime function
	
    bra controlHistoryUpdate    ; else go directly to next iteration calculations
	
    ; Trigger functions:

 triggerOffTime:

      mov [w8 + #periodRegister], w3   ;  Load address of time base to working register w3
      mov [w3], w3                     ;  Move the contents of the address (PTPER) to w3
      sub w3, w4, w3                   ;  Subtract duty cycle from time base and store result to w3
      asr w3, #1, w3                   ;  Right shift result previous results by literal value
      add w3, w4, w3                   ;  Duty cycle + 50% off time (w4 + w3) and store result to w3
      mov [w8 + #delay], w5            ;  Move delay value to w5
      add w3, w5, w3                   ;  Add delay to account for gate drive delay
      mov [w8 + #triggerRegister], w5  ;  Load address of target register
      mov w3, [w5]                     ;  Update the Trigger target register

      bra controlHistoryUpdate
	  
 triggerOnTime:

      asr w4, #1, w3                   ; Right-shift PDCx by literal and store value in w3
      mov [w8 + #delay], w5            ; Load delay value to register w5
      add w3, w5, w3                   ; Add delay to account for gate drive delay
      mov [w8 + #triggerRegister], w5  ; Load address of target register
      mov w3, [w5]                     ; Update the Trigger target register	  
	  
   ; End of trigger options
   
 controlHistoryUpdate:

   ; Update control history:

    clr  a,        [w9]+=2, w4, [w10]+=2, w5     ; ACCA = EMPTY
      	                                         ; w4   = B1, w5 = e[n-1]

    mac  w4*w5, a, [w9]+=2, w4, [w10]+=2, w5     ; ACCA = B1 * e[n-1]
                                                 ; w4   = B2, w5 = e[n-2]

    ; Calculate second section of 2P2Z controller -> (ACoefficients * controlHistory) and add results to accumulator 'A'

    mac  w4*w5, a, [w9]+=2, w4, [w10]+=2, w5     ; ACCA = B1 * e[n-1] + B2 * e[n-2] 
                                                 ; w4 = A1, w5 = u[n-1]

    mac  w4*w5, a, [w9]-=2, w4, [w10],    w5     ; ACCA = A1 * u[n-1] + B1 * e[n-1] + B2 * e[n-2] + B3 * e[n-3]
                                                 ; w4 = A2, w5 = u[n-2]
                                                 ; Decrement w9 (coefficient array)
    mac  w4*w5, a, [w9]-=6, w4, [w10]-=6, w5     ; ACCA = A1 * u[n-1] + A2 * u[n-2] + B1 * e[n-1] + B2 * e[n-2]
                                                 ; Decrement w10 (error/control History) array back to base element
                                                 ; Decrement w9 (coefficient array)

    ; Update compensator errorControlHistory array elements

    mov [w10 + #4], w3	    ; w3 = u[n-1]
    mov  w3, [w10 + #6]     ; u[n-2] = w3
	
    mov [w10], w3	    ; w3 = e[n-1]
    mov  w3, [w10 + #2]     ; e[n-2] = w3

    ; Save 40-bit accumulator results for next iteration

    mov ACCAL, w3           ; Load bits: 15:0
    mov ACCAH, w4           ; Load bits: 31:16
    mov ACCAU, w5           ; Load bits: 39:32

    pop CORCON	            ; Restore CORCON
    
        return
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      .end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; EOF


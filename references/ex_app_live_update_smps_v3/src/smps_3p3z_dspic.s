;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copyright (C) 2016 Microchip Technology Inc.
;
; MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
; derivatives created by any person or entity by or on your behalf, exclusively
; with Microchip's products.  Microchip and its licensors retain all ownership
; and intellectual property rights in the accompanying software and in all
; derivatives here to.
;
; This software and any accompanying information is for suggestion only.  It
; does not modify Microchip's standard warranty for its products.  You agree
; that you are solely responsible for testing the software and determining its
; suitability.  Microchip has no obligation to modify, test, certify, or
; support the software.
;
; THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
; EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
; WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
; PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
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


    .list
    

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Operation:
;                                          Wcp0   ( S + Wcz1 )( S + Wcz2 )
;  Calculate the transfer function Hc(s) = ---- X ------------------------
;                                           S     ( S + Wcp1 )( S + Wcp2 )
;    Error Normalization
;      ------------
;     |   prsft    |
;      ------------
;Reference   |       e(n)          e(n-1)        e(n-2)        e(n-3)
;Input       |             --------      --------      --------
;        -   -   -        |        |    |        |    |        |
;  o----|-|-|x|-|x|----o->| z^(-1) |-o->| z^(-1) |-o->| z^(-1) |--o       Post
;        -   -   -     |  |        | |  |        | |  |        |  |      Scaling
;        |      NORM   |   --------  |   --------  |   --------   |
;        |             |             |             |              |       --------
;Measured|            ---           ---           ---            ---     |        |   - - - - - - - - - -
;Input   |       B0=>| X |     B1=>| X |     B2=>| X |      B3=>| X |    |poshft  |  |                   |
;        |            ---           ---           ---            ---     |postScalar |     P O W E R     |
;        |             |             |             |              |       --------   |     P L A N T     |
;        |            ----------------------------------------------        |        |                   |
;        |           |                                              | U[n] ---       | PDCx              |
;        |           |            A C C U M U L A T O R             |--o--| X |------O Duty Ratio => PWM |
;        |           |                                              |  |   ---       |                   |
;        |            ----------------------------------------------   |             |   Voltage/Current |
;        |             |              |             |                  |             |      Feedback     |
;        |            ---            ---           ---                 |             |         O         |
;        |       A3=>| X |      A2=>| X |     A1=>| X |                |             |         |         |
;        |            ---            ---           ---                 |              - - - - - - - - - -
;        |             |              |             |                  |                       |
;        |             |   --------   |  --------   |  --------        |                       | A/D
;        |             |  |        |  | |        |  | |        |       |                       | Input
;        |             o--| z^(-1) |<-o-| z^(-1) |<-o-| z^(-1) |-------                        |
;        |                |        |    |        |    |        |                               |
;        |                 --------      --------      --------                                |
;        |                  u[n-3]        u[n-2]        u[n-1]                                 |
;        |                                                                                     |
;        |                                                                                     |
;        '-------------------------------------------------------------------------------------'
;
;
; Note
;  This function requires CORCON register to be setup in a certain state
;  in order to operate correctly. Due to this requirement, this function
;  will save the CORCON register on the stack in the beginning of the
;  function and restore it before the function returns.
;  After saving the CORCON register, this function writes to all bits of
;  the CORCON register. Thus, for the brief duration when this function is
;  executing, the state of CORCON register may be different from its state
;  as set by the function caller. This may change the CPU core behavior with
;  respect to exception processing latency, DO loop termination, CPU interrupt
;  priority level and DSP-engine behaviour.
;............................................................................  


;    .pushsection    .libdsp, code                  ; use this section type when used as hidden library file
    .pushsection    .text.SMPS_Controller3P3Z, code ; use this section type for debugging and efficient unused section discarding
    .global _SMPS_Controller3P3Z                    ; Make function entry point publically callable
    .type   _SMPS_Controller3P3Z, @function
_SMPS_Controller3P3Z:
    push    CORCON

    ; Set up DSP core for signed fractional operation, saturation on accumulator A
    ; and for write-back instructions from ACCx to WREGx,
    ; accumulator saturation mode is 1.31
    mov     w14, _CORCON                            ; Initialize CORCON register

    ; Calculate most recent error with normalization,
    sub     w0, [w1], w5                            ; w5 = Reference - inputSource
    sl      w5, w3, w5                              ; shift error by PRE_SHIFT

    ; Store most recent error to error history array
    mov     w5, [w10]                               ; errorHistory[n] = w5

    ; Calculate first section of 3P3Z controller -> Error History
    clr     a, [w9]+=2, w4, [w10]+=2, w5            ; ACCA = EMPTY
                                                    ; w4   = B0, w5   = e[0]
    mac     w4*w5, a, [w9]+=2, w4, [w10]+=2, w5     ; ACCA = B0 * e[n]
                                                    ; w4   = B1, w5 = e[n-1]
    mac     w4*w5, a, [w9]+=2, w4, [w10]+=2, w5     ; ACCA = B0 * e[n] + B1 * e[n-1]
                                                    ; w4   = B2, w5 = e[n-2]
    mac     w4*w5, a, [w9], w4, [w10], w5           ; ACCA = B0 * e[n] + B1 * e[n-1] + B2 * e[n-2]
                                                    ; w4   = B3, w5 = e[n-3]
    mac     w4*w5, a, [w9]-=6, w5                   ; ACCA = B0 * e[n] + B1 * e[n-1] + B2 * e[n-2] + B3 * e[n-3]
                                                    ; Reset BCoefficients

    ; Calculate second section of 3P3Z controller - Controller History
    movsac  a, [w8]+=2, w4, [w11]+=2, w5            ; w4  = A1, w5 = y[n-1]
    mac     w4*w5, a, [w8]+=2, w4, [w11]+=2, w5     ; ACCA = A1 * y[n-1] + B0 * e[n] + B1 * e[n-1] + B2 * e[n-2] + B3 * e[n-3]
                                                    ; w4   = A2, w5 = y[n-2]
    mac     w4*w5, a, [w8], w4, [w11], w5           ; ACCA = A1 * y[n-1] + A2 * y[n-2] + B0 * e[n] + B1 * e[n-1] + B2 * e[n-2] + B3 * e[n-3]
                                                    ; w4   = A3, w5 = y[n-3]
    mac     w4*w5, a, [w8]-=4, w5                   ; ACCA = A1 * y[n-1] + A2 * y[n-2] + A3 * y[n-3] + B0 * e[n] + B1 * e[n-1] + B2 * e[n-2] + B3 * e[n-3]
                                                    ; reset address of A coefficient

    ; Backwards normalization & write back
    sftac   a, w6                       ; Backward normalization to compensate coefficient scaling
    sac.r   a, #0, w4                   ; w4 = Sat(Rnd(ACCAH))

    mpy     w4*w7, a                    ; Multiply control output
    sac.r   a, w4                       ; w4 = Sat(Rnd(ACCAH))

    ; Update the controller output history on the delay line (Reset Address of w11)
    mov     w11, w5
    mov     [--w11], [w5--]             ;ControlHistory[n-3] = ControlHistory[n-2]
    mov     [--w11], [w5--]             ;ControlHistory[n-2] = ControlHistory[n-1]
    mov     w4, [w5--]                  ;ControlHistory[n-1] = CurrentOutput

    ; Clamp controller output to min/max values if needed
    ; W12 Min, W13 Max
    cpsgt   w4, w12
    mov.w   w12, w4

    cpslt   w4, w13
    mov.w   w13, w4

    ; Controller write back into target register
    mov     w4, [w2]                    ; write result into target register

    ; Update the error history (Reset address of w10)
    mov     w10, w5
    mov     [--w10], [w5--]             ; ErrorHistory[n-3] = ErrorHistory[n-2]
    mov     [--w10], [w5--]             ; ErrorHistory[n-2] = ErrorHistory[n-1]
    mov     [--w10], [w5--]             ; ErrorHistory[n-1] = ErrorHistory[n]

    pop     CORCON
    return
    .popsection

.end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; EOF


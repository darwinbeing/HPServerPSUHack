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
        .include        "dspcommon.inc"         ; fractsetup
        .list

        .equ    offsetabcCoefficients, 0
        .equ    offsetcontrolHistory, 2
        .equ    offsetcontrolOutput, 4
        .equ    offsetmeasuredOutput, 6
        .equ    offsetcontrolReference, 8

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        .section .libdsp, code

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _PID:
; Prototype:
;              tPID PID ( tPID *fooPIDStruct )
;
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
;             |   |----------|----------| ------------ |----------|+  |----------| Plant |--
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
;   controlOutput[n] = controlOutput[n-1]
;                    + controlHistory[n] * abcCoefficients[0]
;                    + controlHistory[n-1] * abcCoefficients[1]
;                    + controlHistory[n-2] * abcCoefficients[2]
;
;  where:
;   abcCoefficients[0] = Kp + Ki + Kd
;   abcCoefficients[1] = -(Kp + 2*Kd)
;   abcCoefficients[2] = Kd
;   controlHistory[n] = measuredOutput[n] - referenceInput[n]
;  where:
;   abcCoefficients, controlHistory, controlOutput, measuredOutput and controlReference
;   are all members of the data structure tPID.
;
; Input:
;       w0 = Address of tPID data structure

; Return:
;       w0 = Address of tPID data structure
;
; System resources usage:
;       {w0..w5}        used, not restored
;       {w8,w10}        saved, used, restored
;        AccA, AccB     used, not restored
;        CORCON         saved, used, restored
;
; DO and REPEAT instruction usage.
;       0 level DO instruction
;       0 REPEAT intructions

;............................................................................

        .global _PID                    ; provide global scope to routine
_PID:

.ifndef PSV_ON
		push	DSRPAG
		movpag #0x0001, DSRPAG
.endif

        ; Save working registers.
        push    w8
        push    w10
        push    CORCON                  ; Prepare CORCON for fractional computation.

        fractsetup      w8

        mov [w0 + #offsetabcCoefficients], w8    ; w8 = Base Address of _abcCoefficients array [(Kp+Ki+Kd), -(Kp+2Kd), Kd]
        mov [w0 + #offsetcontrolHistory], w10    ; w10 = Address of _ControlHistory array (state/delay line)

        mov [w0 + #offsetcontrolOutput], w1
        mov [w0 + #offsetmeasuredOutput], w2
        mov [w0 + #offsetcontrolReference], w3

        ; Calculate most recent error with saturation, no limit checking required
        lac     w3, a                   ; A = tPID.controlReference
        lac     w2, b                   ; B = tPID.MeasuredOutput
        sub     a                       ; A = tPID.controlReference - tPID.measuredOutput
        sac.r   a, [w10]                ; tPID.ControlHistory[n] = Sat(Rnd(A))

        ; Calculate PID Control Output
        clr     a, [w8]+=2, w4, [w10]+=2, w5            ; w4 = (Kp+Ki+Kd), w5 = _ControlHistory[n]
        lac     w1, a                                   ; A = ControlOutput[n-1]
        mac     w4*w5, a, [w8]+=2, w4, [w10]+=2, w5     ; A += (Kp+Ki+Kd) * _ControlHistory[n]
                                                        ; w4 = -(Kp+2Kd), w5 = _ControlHistory[n-1]
        mac     w4*w5, a, [w8], w4, [w10]-=2, w5        ; A += -(Kp+2Kd) * _ControlHistory[n-1]
                                                        ; w4 = Kd, w5 = _ControlHistory[n-2]
        mac     w4*w5, a, [w10]+=2, w5                  ; A += Kd * _ControlHistory[n-2]
                                                        ; w5 = _ControlHistory[n-1]
                                                        ; w10 = &_ControlHistory[n-2]
        sac.r   a, w1                                   ; ControlOutput[n] = Sat(Rnd(A))
        mov     w1, [w0 + #offsetcontrolOutput]

        ;Update the error history on the delay line
        mov     w5, [w10]               ; _ControlHistory[n-2] = _ControlHistory[n-1]
        mov     [w10 + #-4], w5         ; _ControlHistory[n-1] = ControlHistory[n]
        mov     w5, [--w10]

        pop     CORCON                  ; restore CORCON.
        pop     w10                     ; Restore working registers.
        pop     w8

.ifndef PSV_ON
		pop	DSRPAG
.endif
        return

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; _PIDInit:
;
; Prototype:
; void PIDInit ( tPID *fooPIDStruct )
;
; Operation: This routine clears the delay line elements in the array
;            _ControlHistory, as well as clears the current PID output
;            element, _ControlOutput
;
; Input:
;       w0 = Address of data structure tPID (type defined in dsp.h)
;
; Return:
;       (void)
;
; System resources usage:
;       w0             used, restored
;
; DO and REPEAT instruction usage.
;       0 level DO instruction
;       0 REPEAT intructions
;
; Program words (24-bit instructions):
;       11
;
; Cycles (including C-function call and return overheads):
;       13
;............................................................................
        .global _PIDInit                ; provide global scope to routine
_PIDInit:

.ifndef PSV_ON
		push	DSRPAG
 	    movpag #0x0001, DSRPAG
.endif

        push    w0
        add  #offsetcontrolOutput, w0   ; Set up pointer for controlOutput
	    clr  [w0]                       ; Clear controlOutput
	    pop  w0
	    push w0
                                        ;Set up pointer to the base of
                                        ;controlHistory variables within tPID
        mov     [w0 + #offsetcontrolHistory], w0
                                        ; Clear controlHistory variables
                                        ; within tPID
        clr     [w0++]                  ; ControlHistory[n]=0
        clr     [w0++]                  ; ControlHistory[n-1] = 0
        clr     [w0]                    ; ControlHistory[n-2] = 0
        pop     w0                      ;Restore pointer to base of tPID

.ifndef PSV_ON
		pop	DSRPAG
.endif
        return


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; _PIDCoeffCalc:
;
; Prototype:
; void PIDCoeffCalc ( fractional *fooPIDGainCoeff, tPID *fooPIDStruct )
;
; Operation: This routine computes the PID coefficients to use based on values
;            of Kp, Ki and Kd provided. The calculated coefficients are:
;             A = Kp + Ki + Kd
;             B = -(Kp + 2*Kd)
;             C = Kd
; Input:
;       w0 = Address of coefficient array containing Kp, Ki and Kd in sequence
;       w1 = Address of PID data structure, tPID (type defined in dsp.h)
;
; Return:
;       (void)
;
; System resources usage:
;       {w0..w2}        used, not restored
;        AccA, AccB     used, not restored
;        CORCON         saved, used, restored
;
; DO and REPEAT instruction usage.
;       0 level DO instruction
;       0 REPEAT intructions
;
; Program words (24-bit instructions):
;       18
;
; Cycles (including C-function call and return overheads):
;       20
;............................................................................
        .global _PIDCoeffCalc
_PIDCoeffCalc:

.ifndef PSV_ON
		push	DSRPAG
	    movpag #0x0001, DSRPAG
.endif

        push    CORCON                  ; Prepare CORCON for fractional computation.
        fractsetup      w2
                                        ;Calculate Coefficients from Kp, Ki and Kd
        mov     [w1], w1
        lac     [w0++], a               ; ACCA = Kp
        lac     [w0++], b               ; ACCB = Ki
        add     a                       ; ACCA = Kp + Ki
        lac     [w0--], b               ; ACCB = Kd
        add     a                       ; ACCA = Kp + Ki + Kd
        sac.r   a, [w1++]               ; _abcCoefficients[0] =  Sat(Rnd(Kp + Ki + Kd))
        lac     [--w0], a               ; ACCA = Kp
        add     a                       ; ACCA = Kp + Kd
        add     a                       ; ACCA = Kp + 2Kd
        neg     a                       ; ACCA = -(Kp + 2Kd)
        sac.r   a, [w1++]               ; _abcCoefficients[1] =  Sat(Rnd(-Kp - 2Kd))
        sac     b, [w1]                 ; _abcCoefficients[2] =  Kd

        pop     CORCON

.ifndef PSV_ON
		pop	DSRPAG
.endif

        return


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        .end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OEF


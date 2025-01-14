; **********************************************************************************
;  SDK Version: PowerSmart™ Digital Control Library Designer v0.9.12.672
;  CGS Version: Code Generator Script v3.0.6 (02/03/2021)
;  Author:      M91406
;  Date/Time:   02/23/2021 15:45:42
; **********************************************************************************
;  Adaptive Gain Control (AGC) Observer Code calculating the AGC modulation factor k_agc
; **********************************************************************************

;------------------------------------------------------------------------------
;file start
	.nolist
	.list
	
;------------------------------------------------------------------------------
;local inclusions.
	.section .data    ; place constant data in the data section
	
    ; include NPNZ16b_t object data structure value offsets and status flag labels
    .include "./sources/power_control/drivers/npnz16b.inc"

;------------------------------------------------------------------------------
;local variables.
    
    .section .bss
    ; no variables declared

;------------------------------------------------------------------------------
;code section.
	.section .text    ; place code in the code section
	
;------------------------------------------------------------------------------
; Global function declaration
; This function calls the z-domain controller processing the latest data point input
;------------------------------------------------------------------------------
	
	.global _v_loop_AGCFactorUpdate
_v_loop_AGCFactorUpdate:
    
    nop ; (debugging break point anchor)

    ; determine most recent VL
    
    ; read and normalize input voltage (normalize to output voltage, not absolute!)
    mov [w0 + #ptrAltSourceRegister], w1    ; load pointer to most recent input voltage register
    mov [w1], w4                            ; load value into w1
    mov [w0 + usrParam1], w5                ; load Q15 factor of input-2-output voltage normalization
    mpy w4*w5, b                            ; multiply ADC reading of input voltage with normalization factor
    mov [w0 + usrParam0], w2                ; load bit-shift scaler of input-2-output voltage normalization factor 
    sftac b, w2                             ; shift result by input voltage normalization scaler
    sac.r b, w1                             ; store most recent accumulator result in working register
    
    ; read output voltage 
    mov [w0 + #ptrSourceRegister], w2       ; load pointer to most recent output voltage register
    mov [w2], w2                            ; load value into w2
    
    ; Calculate instantaneous VL
    sub w1, w2, w5                          ; calculate most recent VL, place result in w5
    
    ; Load modulation median
    mov [w0 + #AgcMedian], w4        ; load pointer to nominal VL
; ToDo: Remove - AGC Median is a constant value and so is factor scaling 
;       (no need to calculate at runtime)
;    mov [w0 + #agcGainModScaler], w2        ; load AGC factor scaler
;    neg w2, w2                              ; invert sign of AGC factor scaler
;    asr w4, w2, w4                          ; shift AGC median by factor scaler
    
    ; Divide median by instatneous VL
    push.s      ; Save pointer to NPNZ16b_t data structure
    repeat #5   ; run divide in 6 steps
    divf w4, w5 ; divide VL_nom/VL_inst
    mov w0, w4  ; move result to w4
    
    ; ----------------------------------------------------------------------------------
    ; ToDo: Possible improvement would be to find first bit from left and pack result in
    ;       factor and scaler. This doesn't work due to number format of division result 
    ;       and remainer. Packing result as floating point won't increase number 
    ;       resolution.
    ; ----------------------------------------------------------------------------------
    
    pop.s       ; restore pointer to NPNZ16b_t data structure
    mov w4, [w0 + #AgcFactor] ; load result into NPNZ16b_t data structure 
                                     ; for further processing
    
;------------------------------------------------------------------------------
; End of routine
	return
;------------------------------------------------------------------------------
    
;------------------------------------------------------------------------------
; End of file
	.end
;------------------------------------------------------------------------------



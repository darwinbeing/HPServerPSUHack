; **********************************************************************************
;  SDK Version: PowerSmart(TM) Digital Control Library Designer v0.9.12.660
;  CGS Version: Code Generator Script v3.0.2 (01/05/2021)
;  Author:      M91406
;  Date/Time:   01/12/2021 21:22:33
; **********************************************************************************
;  Template for user extension functions extending functions of the common
;  control loop code using User Extension Hooks. These extensions allow user 
;  to tie in proprietary user code into the assembly routine of the main control
;  loop.
;
;  Please refer to the PS-DCLD User Guide for details about how to use this
;  feature.
;
; **********************************************************************************
    
;------------------------------------------------------------------------------
; file start
    .nolist                                 ; (no external dependencies)
    .list                                   ; list of all external dependencies
    
;------------------------------------------------------------------------------
; local inclusions.
    .section .data                          ; place constant data in the data section
    
;------------------------------------------------------------------------------
; include NPNZ16B_t data structure and global constants to allow access
; to the active control loop data object
 
    .include "./sources/power_control/drivers/npnz16b.inc" ; include NPNZ16b_t object data structure value offsets and status flag labels
    
;------------------------------------------------------------------------------
; source code section.
    .section .text                          ; place code in the text section
    
;------------------------------------------------------------------------------
; Global function call prototype declarations
; These global function calls are used to publish functions across the 
; application. Add a function call prototype to a C-header file to allow 
; calling this function from C-code.
;
;        extern void __attribute__((near))my_function(void);
;
;------------------------------------------------------------------------------

    .global _my_function                  ; provide global scope to routine
 
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
; User Functions
;------------------------------------------------------------------------------
    
;------------------------------------------------------------------------------
; MY FUNCTION is a simple function template, when used as a user extension 
; function, this function will be directly called by a CALL Wn instruction
; to minimize the function call overhead to 2 instruction cycles only. 
; However, there is no context handling as it is assumed user extension
; functions are commonly using the same data object as the main control loop.
; Hence, all data is kept in its place just like this code would be part of
; the main loop itself.
;    
; Please refer to the PS-DCLD User Guide for details about how to use this
; feature, which working registers are in use by the main loop and which
; can/may be used and/or manipulated by this routine.
;
;------------------------------------------------------------------------------
; Start of routine
_my_function:                               ; local function label (placeholder)
    
    nop                                     ; place your code here
    nop
    nop
    
    mov #0x88B8, w4
    
    nop                                     ; place your code here
    nop
    nop

    
    lsr w4, w2
    
    ; Check for upper limit violation
    mov [w0 + #MaxOutput], w6               ; load upper limit value
    lsr w6, w3               ; load upper limit value, shifted one bit to the right
    cpslt w2, w3                            ; compare values and skip next instruction if control output is within operating range (control output < upper limit)
    mov w6, w4                              ; override controller output

    ; Check for lower limit violation
    mov [w0 + #MinOutput], w6               ; load lower limit value
    lsr w6, w3               ; load upper limit value, shifted one bit to the right
    cpsgt w2, w3                            ; compare values and skip next instruction if control output is within operating range (control output > lower limit)
    mov w6, w4                              ; override controller output

    
    
;------------------------------------------------------------------------------
; End of routine
    return                                  ; end of function; return to caller
    
;------------------------------------------------------------------------------

    
;------------------------------------------------------------------------------
; End of file
    .end                                    ; end of file v_loop_extensions.s
    
;------------------------------------------------------------------------------

     
; **********************************************************************************
;  Download latest version of this tool here: https://areiter128.github.io/DCLD
; **********************************************************************************

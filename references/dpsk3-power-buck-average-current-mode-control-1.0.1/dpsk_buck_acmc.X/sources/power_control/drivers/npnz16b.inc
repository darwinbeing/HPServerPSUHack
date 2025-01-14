;LICENSE / DISCLAIMER
; **********************************************************************************
; Microchip Technology Inc. and its subsidiaries.  You may use this software 
;  and any derivatives exclusively with Microchip products. 
;  
;  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
;  EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
;  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
;  PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
;  WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
;  
;  IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
;  INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
;  WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
;  BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
;  FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
;  IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
;  ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE. 
;  
;  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
;  TERMS.
; **********************************************************************************
; **********************************************************************************
;  SDK Version: PowerSmart™ Digital Control Library Designer v0.9.14.676
;  CGS Version: Code Generator Script v3.0.8 (03/12/2021)
;  Author:      M91406
;  Date/Time:   04/02/2021 00:59:29
; **********************************************************************************
    
;------------------------------------------------------------------------------
; Define status flags bit positions
    .equ NPNZ16_STATUS_ENABLED,      15     ; bit position of the ENABLE control bit
    .equ NPNZ16_STATUS_INVERT_INPUT, 14     ; bit position of the INVERT_INPUT control bit
    .equ NPNZ16_STATUS_SWAP_SOURCE,  13     ; bit position of the SWAP_SOURCE control bit
    .equ NPNZ16_STATUS_SWAP_TARGET,  12     ; bit position of the SWAP_TARGET control bit
    .equ NPNZ16_STATUS_AGC_ENABLED,  11     ; bit position of the AGC_ENABLED control bit
    .equ NPNZ16_STATUS_USAT,         1      ; bit position of the UPPER_SATURATION_FLAG status bit
    .equ NPNZ16_STATUS_LSAT,         0      ; bit position of the LOWER_SATURATION_FLAG status bit
    
;------------------------------------------------------------------------------
; NPNZ16b_s data structure address offset declarations for data structure addressing
    .equ Status,                                0 ; controller object status word at address-offset = 0
    .equ ptrSourceRegister,                     2 ; parameter group Ports.Source: pointer to source memory address
    .equ SourceNormShift,                       4 ; parameter group Ports.Source: bit-shift scaler of normalization factor
    .equ SourceNormFactor,                      6 ; parameter group Ports.Source: Q15 normalization factor
    .equ SourceOffset,                          8 ; parameter group Ports.Source: value of source input signal/value offset
    .equ ptrAltSourceRegister,                 10 ; parameter group Ports.AltSource: pointer to alternate source memory address
    .equ AltSourceNormShift,                   12 ; parameter group Ports.AltSource: bit-shift scaler of normalization factor
    .equ AltSourceNormFactor,                  14 ; parameter group Ports.AltSource: Q15 normalization factor
    .equ AltSourceOffset,                      16 ; parameter group Ports.AltSource: value of alternate source input signal/value offset
    .equ ptrTargetRegister,                    18 ; parameter group Ports.Target: pointer to target memory address
    .equ TargetNormShift,                      20 ; parameter group Ports.Target: bit-shift scaler of normalization factor
    .equ TargetNormFactor,                     22 ; parameter group Ports.Target: Q15 normalization factor
    .equ TargetOffset,                         24 ; parameter group Ports.Target: value of target output signal/value offset
    .equ ptrAltTargetRegister,                 26 ; parameter group Ports.AltTarget: pointer to alternate target memory address
    .equ AltTargetNormShift,                   28 ; parameter group Ports.AltTarget: bit-shift scaler of normalization factor
    .equ AltTargetNormFactor,                  30 ; parameter group Ports.AltTarget: Q15 normalization factor
    .equ AltTargetOffset,                      32 ; parameter group Ports.AltTarget: value of alternate target output signal/value offset
    .equ ptrControlReference,                  34 ; parameter group Ports.ConrolReference: pointer to control reference variable/register memory address
    .equ ptrACoefficients,                     36 ; parameter group Filter: pointer to A-coefficients array start address
    .equ ptrBCoefficients,                     38 ; parameter group Filter: pointer to B-coefficients array start address
    .equ ptrControlHistory,                    40 ; parameter group Filter: pointer to control history array start address
    .equ ptrErrorHistory,                      42 ; parameter group Filter: pointer to error history array start address
    .equ ACoefficientsArraySize,               44 ; parameter group Filter: size of the A-coefficients array
    .equ BCoefficientsArraySize,               46 ; parameter group Filter: size of the B-coefficients array
    .equ ControlHistoryArraySize,              48 ; parameter group Filter: size of the control history array
    .equ ErrorHistoryArraySize,                50 ; parameter group Filter: size of the error history array
    .equ normPreShift,                         52 ; parameter group Filter: value of input value normalization bit-shift scaler
    .equ normPostShiftA,                       54 ; parameter group Filter: value of A-term normalization bit-shift scaler
    .equ normPostShiftB,                       56 ; parameter group Filter: value of B-term normalization bit-shift scaler
    .equ normPostScaler,                       58 ; parameter group Filter: control loop output normalization factor
    .equ PTermScaler,                          60 ; parameter group Filter: P-Term coefficient scaler
    .equ PTermFactor,                          62 ; parameter group Filter: P-Term coefficient fractional factor
    .equ AgcScaler,                            64 ; parameter group GainControl: bit-shift scaler of Adaptive Gain Control Modulation factor
    .equ AgcFactor,                            66 ; parameter group GainControl: Q15 value of Adaptive Gain Control Modulation factor
    .equ AgcMedian,                            68 ; parameter group GainControl: Q15 value of Adaptive Gain Control Modulation nominal operating point
    .equ ptrAgcObserverFunction,               70 ; parameter group GainControl: function pointer to observer function updating the AGC modulation factor
    .equ MinOutput,                            72 ; parameter group Limits: minimum clamping value of primary control output
    .equ MaxOutput,                            74 ; parameter group Limits: maximum clamping value of primary control output
    .equ AltMinOutput,                         76 ; parameter group Limits: minimum clamping value of alternate control output
    .equ AltMaxOutput,                         78 ; parameter group Limits: maximum clamping value of alternate control output
    .equ ptrADCTriggerARegister,               80 ; parameter group ADCTriggerControl: pointer to ADC trigger A register memory address
    .equ ADCTriggerAOffset,                    82 ; parameter group ADCTriggerControl: value of ADC trigger A offset
    .equ ptrADCTriggerBRegister,               84 ; parameter group ADCTriggerControl: pointer to ADC trigger B register memory address
    .equ ADCTriggerBOffset,                    86 ; parameter group ADCTriggerControl: value of ADC trigger B offset
    .equ ptrDProvControlInput,                 88 ; parameter group DataProviders: pointer to external variable/register the most recent, raw control input will be pushed to
    .equ ptrDProvControlInputComp,             90 ; parameter group DataProviders: pointer to external variable/register the most recent, compensated control input will be pushed to
    .equ ptrDProvControlError,                 92 ; parameter group DataProviders: pointer to external variable/register the most recent control error will be pushed to
    .equ ptrDProvControlOutput,                94 ; parameter group DataProviders: pointer to external variable/register the most recent control output will be pushed to
    .equ ptrExtHookStartFunction,              96 ; parameter group ExtensionHooks: pointer to external extension function which will be called by this controller at the beginning after evaluating the enable flag. This function call will be bypassed when the controller is disabled.
    .equ ExtHookStartFunctionParam,            98 ; parameter group ExtensionHooks: 16-bit wide function parameter variable or pointer to a parameter variable or data structure of extension function
    .equ ptrExtHookSourceFunction,            100 ; parameter group ExtensionHooks: pointer to external extension function which will be called by this controller after the most recent controller input has been read and compensated but before the most recent error is calculated.
    .equ ExtHookSourceFunctionParam,          102 ; parameter group ExtensionHooks: 16-bit wide function parameter variable or pointer to a parameter variable or data structure of extension function
    .equ ptrExtHookPreAntiWindupFunction,     104 ; parameter group ExtensionHooks: pointer to external extension function which will be called by this controller after the most recent compensation filter result has been computed and before  anti-windup clamping is applied
    .equ ExtHookPreAntiWindupFunctionParam,   106 ; parameter group ExtensionHooks: 16-bit wide function parameter variable or pointer to a parameter variable or data structure of extension function
    .equ ptrExtHookPreTargetWriteFunction,    108 ; parameter group ExtensionHooks: pointer to external extension function which will be called by this controller before the most recent controller output is written to target, after the value has passed through anti-windup clamping.
    .equ ExtHookPreTargetWriteFunctionParam,  110 ; parameter group ExtensionHooks: 16-bit wide function parameter variable or pointer to a parameter variable or data structure of extension function
    .equ ptrExtHookEndOfLoopFunction,         112 ; parameter group ExtensionHooks: pointer to external extension function which will be called by this controller after the compensation loop computation has been completed. This function call will be bypassed when the controller is disabled.
    .equ ExtHookEndOfLoopFunctionParam,       114 ; parameter group ExtensionHooks: 16-bit wide function parameter variable or pointer to a parameter variable or data structure of extension function
    .equ ptrExtHookExitFunction,              116 ; parameter group ExtensionHooks: pointer to external extension function which will be called by this controller after the compensation loop computation has been completed, regardless of the loop execution being bypassed or not
    .equ ExtHookExitFunctionParam,            118 ; parameter group ExtensionHooks: 16-bit wide function parameter variable or pointer to a parameter variable or data structure of extension function
    .equ usrParam0,                           120 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #1 for user-defined, advanced control options
    .equ usrParam1,                           122 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #2 for user-defined, advanced control options
    .equ usrParam2,                           124 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #3 for user-defined, advanced control options
    .equ usrParam3,                           126 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #4 for user-defined, advanced control options
    .equ usrParam4,                           128 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #5 for user-defined, advanced control options
    .equ usrParam5,                           130 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #6 for user-defined, advanced control options
    .equ usrParam6,                           132 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #7 for user-defined, advanced control options
    .equ usrParam7,                           134 ; parameter group Advanced: generic 16-bit wide, user-defined parameter #8 for user-defined, advanced control options
    
;------------------------------------------------------------------------------
;end of include of NPNZ16b_s data structure and global constants
     
; **********************************************************************************
;  Download latest version of this tool here: https://areiter128.github.io/DCLD
; **********************************************************************************

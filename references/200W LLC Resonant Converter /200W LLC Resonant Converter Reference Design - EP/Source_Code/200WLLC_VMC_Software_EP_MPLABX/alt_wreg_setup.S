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

    
.include "p33Exxxx.inc"
#include "VMC_coeffs.h"
    
    .extern _compOutput
    .extern _LLCOptions
    .extern _abCoefficients2P2Z
    .extern _errorcontrolHistory2P2Z 

.text					; Begin program instructions

.global _InitAltRegContext1Setup
    
_InitAltRegContext1Setup:

  CTXTSWP #0x1				; Swap to Alternate W-Reg context #1

  ; Note: w0 register will be used for compensator control reference parameter
  ; Initialize Alternate Working Registers context #1
    
  mov	#ADCBUF1,   w1			; Address of the ADCBUF1 register (Input)
  mov	#_compOutput,   w2		; Address of the compensator output 
					; target location (Output)

  ; w3, w4, w5 used for ACCAx registers and for MAC/MPY instructions
  ; Initialize registers to '0'

  mov #CON_2P2Z_POSTSCALER, w6
  mov #CON_2P2Z_POSTSHIFT,  w7
  mov #_LLCOptions, w8			; Points to user options structure
  mov #_abCoefficients2P2Z, w9
  mov #_errorcontrolHistory2P2Z,    w10 
  mov #CON_2P2Z_MINOUTPUT,  w11
  mov #CON_2P2Z_MAXOUTPUT,  w12

  CTXTSWP #0x0				; Swap back to main register set

  return				; Exit Alt-WREG1 set-up function

.end




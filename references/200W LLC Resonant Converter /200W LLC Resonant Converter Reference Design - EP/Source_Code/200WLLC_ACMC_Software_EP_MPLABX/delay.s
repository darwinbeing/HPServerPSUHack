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


.set Fcy,        70150000        ;50000000

.set US_K,       Fcy/1000000
.set MS_K,       Fcy/10000

    .global _Delay
    .global _Delay_Us

;===============================================
; ms Delay Function
;===============================================
_Delay:

ms_oloop:
    mov #MS_K,w1
ms_iloop:
    nop
    nop
    nop
    nop
    nop

    nop
    nop


    dec     w1, w1
    bra     nz, ms_iloop                    ; The inner loop will provide a delay of 1ms
    
    dec     w0,w0
    bra     nz,ms_oloop                     ; Outer Loop will provide as many ms as the argument

    return

;===============================================
; us Delay Function
;===============================================

_Delay_Us:

us_oloop:
    
    .rept (US_K-6)
    nop
    .endr
 
    
    dec     w0,w0
    bra     nz,us_oloop

    return







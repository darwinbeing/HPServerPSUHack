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
#include "define.h"
#include "VMC_coeffs.h"
      
      .extern _SFRupdateFlag
      .extern _softStartFlag
      .extern _outputVoltage
      .extern _ACCAHstore
      .extern _ACCAUstore
      .extern _compOutput
      .extern _newPTPER
      .extern _newPDC1
      .extern _newPDC2
      .extern _synchRectDutyCycle      
            
.text

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ISR:          TIMER2 Interrupt
; Description:	VMC controller: 2P2Z compensator control function call
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.global __T2Interrupt

__T2Interrupt:

    push    CORCON			; Save CORCON as it will be 
					; modified for fractional computation
    bset    _SFRupdateFlag, #0		; SFR update flag is set as PWM 
					; SFR new values calculation is 
					; started (to avoid update partial 
					; update of SFR values)
    btsc    _softStartFlag,#0		; Check for softstart flag; if softstart 
					; is completed go for closed loop opeartion
    bra	    endcalc
    mov	    [w1], w0			; Store output voltage ADC value to 
					; outputVoltage variable
    mov	    w0, _outputVoltage
    mov	    #OUTPUTVOLTAGEREFERENCE, w0		; Store output voltage reference 
						; value to w0
    call    _SMPS_Controller2P2ZUpdate_HW_Accel ; call 2P2Z controller
    mov	    w4,_ACCAHstore			; store w4 in temp variable for 
						; post calcultion of period
    mov	    w5,_ACCAUstore			; store w5 in temp variable for 
						; post calcultion of period
        
    ;Calculation of LLC Period = ( (compOutput*BASEPERIOD) + NOMINALPERIOD )/2 
    mov	    _compOutput, w5			; Output of 2p2z controller is 
						; stored in w5 			  
    sl	    w5, #3, w5				; Left shift by 3 bits to convert 
						; in Q15 
    mov	    #BASEPERIOD, w4			; Store Base Period value to w4
    mpy	    w4*w5, a				; Multiply BASEPERIOD and output 
						; of 2p2z controller
    sac.r   a, w5				; Store accumulator to w5
    mov	    #NOMINALPERIOD, w4			; Store NOMINALPERIOD value to w4
    add	    w5, w4, w5				; Add NOMINALPERIOD to w5
    asr	    w5, #1, w0				; Right shift by 1 bit for push 
						; pull mode
    ;--------------------------------------------------------------
    ; Checking new Period within max and min period
    mov	    #MAXPERIOD,w4
    cpslt   w0,w4
    bra	    branch1
    mov	    #MINPERIOD, w4
    cpsgt   w0,w4
    bra	    branch2
    ; If new Period within max and min period, store new Period and primary FET 
    ; duty in temp variable
    mov	    w0, _newPTPER
    mov	    #DEADTIME, w4
    sub	    w0,w4,w5			
    mov	    w5, _newPDC1	    ; newPDC1 = LLCUpdatedperiod - DEADTIME
    ; Check new Period less than Resonace period
    mov	    #RESONANTPERIOD, w4
    cpslt   w0,w4
    bra	    branch3
    ; If new Period less than Resonance period, store new synch FET duty in 
    ; temp variable 
    mov	    #SRDEADTIME, w4
    sub	    w0,w4,w5			
    mov	    w5, _newPDC2	    ; newPDC2 = LLCUpdatedperiod - SRDEADTIME
    bra	    endcalc
    ; If new Period grater than max period, saturate new Period to max period 
    ; and store new period, primary and synch FET duty in temp variables
branch1:
    mov	    w4, _newPTPER
    mov	    #DEADTIME, w5
    sub	    w4,w5,w5			
    mov	    w5, _newPDC1	    ; newPDC1 = LLCUpdatedperiod - DEADTIME
    mov	    #2546,w4 
    mov	    #SRDEADTIME, w5
    sub	    w4,w5,w5			 
    mov	    w5, _newPDC2	    ; newPDC2 = 2546-SRDEADTIME
    bra	    endcalc
    ; If new Period greater than max period, saturate new Period to min period 
    ; and store new period, primary and synch FET duty in temp variables
branch2:
    mov	    w4, _newPTPER
    mov	    #DEADTIME, w5
    sub	    w4,w5,w5
    mov	    w5, _newPDC1	    ; newPDC1 = LLCUpdatedperiod - DEADTIME
    mov	    #SRDEADTIME, w5
    sub	    w4,w5,w5
    mov	    w5, _newPDC2	    ; newPDC2 = LLCUpdatedperiod - SRDEADTIME
    bra	    endcalc
    ; If new Period greater than resonance period, store new synch FET duty in 
    ; temp variable based on SR table 
    ; Intex to selection SR FET duty from table is as follows: 
    ; index_SR = (newPTPER & 0xFFF8) - RESONANTPERIOD. For positive index_SR, 
    ; newPDC2 = (synchRectDutyCycle[index_SR >> 3][1]) - SRDEADTIME;
branch3:
    mov	    #0xFFF8, w4
    and	    w0, w4, w4
    mov	    #RESONANTPERIOD, w5
    sub	    w4, w5, w4
    clr	    w0
    cpsgt   w4,w0
    clr	    w4
    asr	    w4,#1,w4
    add	    w4,#2,w4
    mov	    #_synchRectDutyCycle,w0
    mov	    [w0+w4],w5
    mov	    #SRDEADTIME, w4
    sub	    w5,w4,w5			
    mov	    w5, _newPDC2	; newPDC2 = (synchRectDutyCycle[index_SR >> 3][1]) - SRDEADTIME;    
    ; Restore w4 and w5 registers from temp variables 
endcalc:           
    mov	    _ACCAHstore, w4
    mov	    _ACCAUstore, w5    
    bclr    _SFRupdateFlag, #0		; SFR update flag is clear as PWM 
					; SFR new values calculation is 
					; ended  					
    pop     CORCON			; restore CORCON
    bclr    IFS0, #7			; Clear T2 interrupt flag
    
    retfie				; Return from interrupt service 
					; routine
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   End of T2 Interrupt ISR
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ISR:          SEVTCMPInterrupt
; Description:	PTPER, PDC1, PDC2 updating
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

   .global __PWMSpEventMatchInterrupt

__PWMSpEventMatchInterrupt:

    push    w0
          
    btsc    _SFRupdateFlag,#0		; Check for SFR update flag is clear or 
					; not; if it is not clear, don't upate PWM SFRs 
    bra	    endSFRupdate
    
    mov	    _newPTPER, w0
    
    mov	    w0, PTPER			; PTPER = newPTPER
    mov	    _newPDC1, w0
    mov	    w0, PDC1			; PDC1 = newPDC1
    mov	    _newPDC2, w0
    mov	    w0, PDC2			; PDC2 = newPDC2
    
    mov	    PTPER, w0			     
    asr	    w0, #1, w0
    add	    #100, w0
    mov	    w0, TRIG2			; TRIG2 = (PTPER >> 1)+100			 		
    
    mov	    PTPER, w0			      
    sub	    #20,w0
    mov	    w0, SEVTCMP			; SEVTCMP = PTPER - 20
  
endSFRupdate:    
    pop	    w0 
    bclr    IFS3, #9			; Clear SEVTCMP interrupt flag.
     
    retfie				; Return from interrupt service routine
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   End of SEVTCMP Interrupt ISR
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    .end
    
    



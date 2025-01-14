/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   p33c_macros.h
 * Author: M91406
 * Comments: Header files providing inline assembly macros for basic CPU instructions
 * Revision history: 
 * 1.0	Initial release		06/22/20
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef P33C_CPU_MACROS_H
#define	P33C_CPU_MACROS_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define WDT_RESET()     asm volatile ("CRLWDT\n")
#define PWRSAV_IDLE()   asm volatile ("PWRSAV #1\n")
#define PWRSAV_SLEEP()  asm volatile ("PWRSAV #0\n")
#define CPU_RESET()     asm volatile ("RESET\n")

/**********************************************************************************
 * @def ALTWREG_SWAP
 * @brief Swaps the current working register set
 * @param  x: Index of new Working Register Set (Target) of type unsigned integer
 * @return Index of previous Working Register Set (Origin) of type unsigned integer
 **********************************************************************************/
#define ALTWREG_SWAP(x) __extension__ ({ \
    volatile uint16_t __x = (x), __v; \
    __asm__ ("ctxtswp %1;\n\t" : "=d" (__v) : "d" (__x)); __v; \
})    


#endif	/* P33C_CPU_MACROS_H */

// end of file 

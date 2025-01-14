/*LICENSE *****************************************************************************************
 *
 * Software License Agreement
 *
 * Copyright (R) 2012 Microchip Technology Inc.  All rights reserved. Microchip licenses to you the
 * right to use, modify, copy and distribute Software only when embedded on a Microchip 
 * microcontroller or digital signal controller, which is integrated into your product or third 
 * party product (pursuant to the sublicense terms in the accompanying license agreement).
 *
 * You should refer to the license agreement accompanying this Software for additional information 
 * regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR 
 * IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR 
 * OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT  
 * LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS  
 * OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY  
 * THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *
 * ***********************************************************************************************/

#ifndef P33C_DSP_SFR_ABSTRACTION_H
#define P33C_DSP_SFR_ABSTRACTION_H

/**************************************************************************************************
 * @file p33c_dac.h
 * @see	 p33c_dsp.c
 * @brief Generic Digital Signal Processor Driver Module (header file)
 * @details
 * This additional header file contains defines for all required bit-settings of all related 
 * special function registers of a peripheral module and/or instance. 
 * This file is an additional header file on top of the generic device header file.
 * 
 *************************************************************************************************/

// CORCON: CORE CONTROL REGISTER

#define REG_CORCON_UNUSED_MSK         	0b0100000000000000
#define REG_CORCON_VALID_DATA_WRITE_MSK	0b1011100011110011
#define REG_CORCON_VALID_DATA_READ_MSK	0b1011111111111111

#define REG_CORCON_VAR_VARIABLE         0b1000000000000000
#define REG_CORCON_VAR_FIXED            0b0000000000000000

enum CORCON_VAR_e{
    CORCON_VAR_VARIABLE = 0b1, // Variable exception processing is enabled
    CORCON_VAR_FIXED = 0b0 // Fixed exception processing is enabled
}; 
typedef enum CORCON_VAR_e CORCON_VAR_t; // Variable Exception Processing Latency Control bit

#define REG_CORCON_US_MIXED     0b0010000000000000 // DSP engine multiplies are mixed sign
#define REG_CORCON_US_UNSIGNED  0b0001000000000000 // DSP engine multiplies are unsigned
#define REG_CORCON_US_SIGNED    0b0000000000000000 // DSP engine multiplies are signed

enum CORCON_US_e{
    CORCON_US_MIXED = 0b10, // DSP engine multiplies are mixed sign
    CORCON_US_UNSIGNED = 0b01, // DSP engine multiplies are unsigned
    CORCON_US_SIGNED = 0b00 // DSP engine multiplies are signed
}; 
typedef enum CORCON_US_e CORCON_US_t; // DSP Multiply Unsigned/Signed Control bits

#define REG_CORCON_EDT_TERMINATE 0b0000100000000000
#define REG_CORCON_EDT_RUN 0b0000000000000000

enum CORCON_EDT_e{
    CORCON_EDT_TERMINATE = 0b1, // Terminates executing DO loop at the end of the current loop iteration
    CORCON_EDT_RUN = 0b0 // No effect
}; 
typedef enum CORCON_EDT_e CORCON_EDT_t; // Early DO Loop Termination Control bit

#define REG_CORCON_DL_7 0b0000011100000000 // Seven DO loops are active
#define REG_CORCON_DL_6 0b0000011100000000 // Six DO loops are active
#define REG_CORCON_DL_5 0b0000011100000000 // Five DO loops are active
#define REG_CORCON_DL_4 0b0000011100000000 // Four DO loops are active
#define REG_CORCON_DL_3 0b0000011100000000 // Three DO loops are active
#define REG_CORCON_DL_2 0b0000011100000000 // Two DO loops are active
#define REG_CORCON_DL_1 0b0000011100000000 // One DO loops are active
#define REG_CORCON_DL_0 0b0000011100000000 // Zero DO loops are active

enum CORCON_DL_STAT_e{
    CORCON_DL_7 = 0b111, // Seven DO loops are active
    CORCON_DL_6 = 0b110, // Six DO loops are active
    CORCON_DL_5 = 0b101, // Five DO loops are active
    CORCON_DL_4 = 0b100, // Four DO loops are active
    CORCON_DL_3 = 0b011, // Three DO loops are active
    CORCON_DL_2 = 0b010, // Two DO loops are active
    CORCON_DL_1 = 0b001, // One DO loops are active
    CORCON_DL_0 = 0b000  // Zero DO loops are active
}; 
typedef enum CORCON_DL_STAT_e CORCON_DL_STAT_t; // DO Loop Nesting Level Status bits

#define REG_CORCON_SATA_ON  0b0000000010000000 // Accumulator A saturation is enabled
#define REG_CORCON_SATA_OFF 0b0000000000000000 // Accumulator A saturation is disabled

enum CORCON_SATA_e{
    CORCON_SATA_ON = 0b1, // Accumulator A saturation is enabled
    CORCON_SATA_OFF = 0b0 // Accumulator A saturation is disabled
}; 
typedef enum CORCON_SATA_e CORCON_SATA_t; // ACCA Saturation Enable bit

#define REG_CORCON_SATB_ON  0b0000000001000000 // Accumulator B saturation is enabled
#define REG_CORCON_SATB_OFF 0b0000000000000000 // Accumulator B saturation is disabled

enum CORCON_SATB_e{
    CORCON_SATB_ON = 0b1, // Accumulator B saturation is enabled
    CORCON_SATB_OFF = 0b0 // Accumulator B saturation is disabled
}; 
typedef enum CORCON_SATB_e CORCON_SATB_t; // ACCB Saturation Enable bit

#define REG_CORCON_SATDW_ON  0b0000000000100000 // Data Space write saturation is enabled
#define REG_CORCON_SATDW_OFF 0b0000000000000000 // Data Space write saturation is disabled

enum CORCON_SATDW_e{
    CORCON_SATDW_ON = 0b1, // Data Space write saturation is enabled
    CORCON_SATDW_OFF = 0b0 // Data Space write saturation is disabled
}; 
typedef enum CORCON_SATDW_e CORCON_SATDW_t; // Data Space Write from DSP Engine Saturation Enable bit

#define REG_CORCON_ACCSAT_931 0b0000000000010000 // 9.31 saturation (super saturation)
#define REG_CORCON_ACCSAT_131 0b0000000000000000  // 1.31 saturation (normal saturation)

enum CORCON_ACCSAT_e{
    CORCON_ACCSAT_931 = 0b1, // 9.31 saturation (super saturation)
    CORCON_ACCSAT_131 = 0b0  // 1.31 saturation (normal saturation)
};
typedef enum CORCON_ACCSAT_e CORCON_ACCSAT_t; // Accumulator Saturation Mode Select bit

#define REG_CORCON_IPL3_STAT_GT7 0b0000000000001000
#define REG_CORCON_IPL3_STAT_LT7 0b0000000000000000

enum CORCON_IPL3_STAT_e{
    CORCON_IPL3_STAT_GT7 = 0b1, // CPU Interrupt Priority Level is greater than 7
    CORCON_IPL3_STAT_LT7 = 0b0  // CPU Interrupt Priority Level is 7 or less
}; 
typedef enum CORCON_IPL3_STAT_e CORCON_IPL3_STAT_t; // CPU Interrupt Priority Level Status bit 3

#define REG_CORCON_SFA_ACTIVE   0b0000000000000100 // Stack frame is active; W14 and W15 address 0x0000 to 0xFFFF, regardless of DSRPAG
#define REG_CORCON_SFA_INACTIVE 0b0000000000000000 // Stack frame is not active; W14 and W15 address the base Data Space

enum CORCON_SFA_e{
    CORCON_SFA_ACTIVE = 0b1, // Stack frame is active; W14 and W15 address 0x0000 to 0xFFFF, regardless of DSRPAG
    CORCON_SFA_INACTIVE = 0b0 // Stack frame is not active; W14 and W15 address the base Data Space
}; 
typedef enum CORCON_SFA_e CORCON_SFA_t; // Stack Frame Active Status bit

#define REG_CORCON_RND_BIASED   0b0000000000000010 // Biased (conventional) rounding is enabled
#define REG_CORCON_RND_UNBIASED 0b0000000000000000 // Unbiased (convergent) rounding is enabled

enum CORCON_RND_e{
    CORCON_RND_BIASED = 0b1, // Biased (conventional) rounding is enabled
    CORCON_RND_UNBIASED = 0b0 // Unbiased (convergent) rounding is enabled
}; 
typedef enum CORCON_RND_e CORCON_RND_t; // Rounding Mode Select bit

#define REG_CORCON_IF_INTEGER    0b0000000000000001 // Integer mode is enabled for DSP multiply
#define REG_CORCON_IF_FRACTIONAL 0b0000000000000000 // Fractional mode is enabled for DSP multiply

enum CORCON_IF_e{
    CORCON_IF_INTEGER = 0b1, // Integer mode is enabled for DSP multiply
    CORCON_IF_FRACTIONAL = 0b0 // Fractional mode is enabled for DSP multiply
}; 
typedef enum CORCON_IF_e CORCON_IF_t; // Integer or Fractional Multiplier Mode Select bit

struct DSP_CONFIG_s
{
    union {
    struct {
        volatile CORCON_IF_t IF : 1; // Integer or Fractional Multiplier Mode Select bit
        volatile CORCON_RND_t RND : 1; // Rounding Mode Select bit
        volatile CORCON_SFA_t SFA : 1; // Stack Frame Active Status bit
        volatile CORCON_IPL3_STAT_t IPL3 : 1; // CPU Interrupt Priority Level Status bit 3
        volatile CORCON_ACCSAT_t ACCSAT : 1; // Accumulator Saturation Mode Select bit
        volatile CORCON_SATDW_t SATDW : 1; // Data Space Write from DSP Engine Saturation Enable bit
        volatile CORCON_SATB_t SATB : 1; // ACCB Saturation Enable bit
        volatile CORCON_SATA_t SATA : 1; // ACCA Saturation Enable bit

        volatile CORCON_DL_STAT_t DL : 3; // DO Loop Nesting Level Status bits
        volatile CORCON_EDT_t EDT : 1; // Early DO Loop Termination Control bit
        volatile CORCON_US_t US : 2; // DSP Multiply Unsigned/Signed Control bits
        volatile unsigned : 1; // reserved
        volatile CORCON_VAR_t VAR : 1; // Variable Exception Processing Latency Control bit
    } __attribute__((packed)) bits; // CORCON: CORE CONTROL REGISTER BITFIELD

    volatile uint16_t value;  // CORCON: CORE CONTROL REGISTER VALUE
    };
}; 
typedef struct DSP_CONFIG_s DSP_CONFIG_t; // CORCON: CORE CONTROL REGISTER

/* PROTOTYPES */
extern volatile uint16_t Dsp_SetConfig(volatile struct DSP_CONFIG_s dsp_cfg);
extern volatile struct DSP_CONFIG_s Dsp_GetConfig(void);

#endif /* end of P33C_DSP_SFR_ABSTRACTION_H */ 

// end of file

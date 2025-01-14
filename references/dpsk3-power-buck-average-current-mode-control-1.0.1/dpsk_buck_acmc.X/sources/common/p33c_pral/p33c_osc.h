/*LICENSE ********************************************************************
 * Microchip Technology Inc. and its subsidiaries.  You may use this software 
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
 * ***************************************************************************/

/*****************************************************************************
 *
 * @file    p33c_osc.h
 * @author  M91406
 * @date    October 27, 2017, 11:24 AM
 * 
 * History:
 * 10/27/17 initial release
 * 07/07/18 separation of FJ, EP and CH/CK device families
 * 
 * Know limitations in this release:
 * - REFCLK is not supported
 * ***************************************************************************/

#ifndef MCAL_P33SMPS_OSCILLATOR_H
#define MCAL_P33SMPS_OSCILLATOR_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer types header file
#include <stdbool.h> // include standard boolean types header file

#include "p33smps_devices.h" // DEVICES header to derive device-dependent properties

/**************************************************************************************************
 * System Frequencies
 * Summary:
 * Set of defines, data types and data structures for system frequency adaption
 *
 * Description:
 * This library offers default preset for CPU frequencies to simplify the oscillator 
 * configuration for standard applications using the internal Fast RC oscillator (FRC).
 * These parameters are supposed to be used with the function call init_FRCCLK_Defaults().
 * To change the CPU frequency at any point during runtime, clock switching must be enabled
 * by setting the configuration bit FCKSM:
 * 
 *      #pragma config FCKSM = CSECMD (Clock Switching Enabled/Clock Monitor Disabled) 
 *   or                        CSECME (Clock Switching Enabled/Clock Monitor Enabled)
 * 
 * Example:
 * The following code line configures the internal FRC oscillator for 80 MIPS operation:
 * 
 *      init_FRCCLK_Defaults(CPU_SPEED_80_MIPS);    // Configuring FRC for 80 MIPS operation
 * 
 *************************************************************************************************/

typedef enum CPU_SPEED_DEFAULTS_e
{
    CPU_SPEED_20_MIPS = 20, // CPU Speed setting for 20 MIPS operation
    CPU_SPEED_30_MIPS = 30, // CPU Speed setting for 30 MIPS operation
    CPU_SPEED_40_MIPS = 40, // CPU Speed setting for 40 MIPS operation
    CPU_SPEED_50_MIPS = 50, // CPU Speed setting for 50 MIPS operation
    CPU_SPEED_60_MIPS = 60, // CPU Speed setting for 60 MIPS operation
    CPU_SPEED_70_MIPS = 70, // CPU Speed setting for 70 MIPS operation
    CPU_SPEED_80_MIPS = 80, // CPU Speed setting for 80 MIPS operation
    CPU_SPEED_90_MIPS = 90, // CPU Speed setting for 90 MIPS operation
    CPU_SPEED_100_MIPS = 100 // CPU Speed setting for 100 MIPS operation
} CPU_SPEED_DEFAULTS_t;  // Default CPU speed settings 


typedef enum AUX_PLL_DEFAULTS_e
{
    AFPLLO_100_MHZ  = 100, // Auxiliary PLL output frequency of 500 MHz
    AFPLLO_200_MHZ  = 200, // Auxiliary PLL output frequency of 500 MHz
    AFPLLO_300_MHZ  = 300, // Auxiliary PLL output frequency of 500 MHz
    AFPLLO_400_MHZ  = 400, // Auxiliary PLL output frequency of 500 MHz
    AFPLLO_500_MHZ  = 500, // Auxiliary PLL output frequency of 500 MHz (Default for high resolution PWM)
    AFPLLO_600_MHZ  = 600, // Auxiliary PLL output frequency of 600 MHz
    AFPLLO_700_MHZ  = 700, // Auxiliary PLL output frequency of 700 MHz
    AFPLLO_800_MHZ  = 800  // Auxiliary PLL output frequency of 800 MHz
} AUX_PLL_DEFAULTS_t;  // Default Auxiliary PLL output frequency settings 


/**************************************************************************************************
 * @struct OSCILLATOR_SYSTEM_FREQUENCIES_s
 * @brief  Global data structure holding system frequencies of different clock domains
 *
 * @details
 * The data structure "system_frequencies" of type OSCILLATOR_SYSTEM_FREQUENCIES_t is used
 * to broadcast most recent system frequencies of multiple clock domains. Contents of this data 
 * structure are NOT updated automatically. 
 * 
 * The function p33c_Osc_GetFrequencies() must be called from user code to update/refresh the 
 * contents of this data structure every time a oscillator configuration has been changed.
 *
 * <p><b>Example:</b></p>
 * The following code lines initialize the internal FRC oscillator for 100 MIPS operation and
 * the auxiliary PLL for 500 MHz to support 250ps resolution of the PWM module. After both 
 * configurations have been set, the function 'osc_get_frequencies()' is used to update the 
 * most recent frequencies of multiple clock domains.
 * 
 * @code{.c}
 *      init_FRCCLK_Defaults(CPU_SPEED_100_MIPS);   // Initialize FRC for 100 MIPS operation
 *      init_AUXCLK_500MHz();                       // Initialize AuxPLL for 500 MHz clock output
 *      osc_get_frequencies(0);                     // Update system frequencies data structure
 * @endcode
 * 
 * @note
 * When an external oscillator is used, the function osc_get_frequencies() must be called 
 * to set the external frequency value in [Hz] and update all related frequencies accordingly.
 * If only the internal FRC oscillator is used, this parameter should be set = 0.
 * ***********************************************************************************************/

typedef struct OSCILLATOR_SYSTEM_FREQUENCIES_s {
    volatile uint32_t frc;      // Internal fast RC oscillator frequency incl. tuning
    volatile uint32_t fpri;     // External primary oscillator frequency 
    volatile uint32_t fclk;     // Clock frequency (external or internal oscillator frequency)
    volatile uint32_t fosc;     // Oscillator frequency
    volatile uint32_t fcy;      // CPU click frequency (instruction frequency = MIPS incl. DOZE divider)
    volatile uint32_t fp;       // Peripheral bus clock frequency
    volatile uint32_t fpllo;    // PLL output frequency
    volatile uint32_t fvco;     // PLL VCO frequency output incl. divider
    volatile float tp;          // Peripheral clock period 
    volatile float tcy;         // CPU clock period 
    volatile uint32_t afpllo;   // APLL output frequency
    volatile uint32_t afvco;    // APLL VCO frequency output incl. divider
}OSCILLATOR_SYSTEM_FREQUENCIES_t;


/* FRC oscillator settings and tuning */
#if defined (__P33SMPS_CH__) 

    #define FRCTUN_MIN              -32     // minimum tuning value
    #define FRCTUN_MAX              31      // maximum tuning value
    #define OSC_FRC_FREQ            8000000     // Frequency of the internal oscillator in [Hz]
    #define OSC_FRC_TUN             0           // <OSCTUN> FRC Oscillator Tuning Rregister default value
    #define OSC_TUN_STEP_PERCENTAGE 0.00047     // Oscillator frequency step size of <OSCTUN>

#elif defined (__P33SMPS_CK__) 

    #define FRCTUN_MIN              -32     // minimum tuning value
    #define FRCTUN_MAX              31      // maximum tuning value
    #define OSC_FRC_FREQ            8000000     // Frequency of the internal oscillator in [Hz]
    #define OSC_FRC_TUN             0           // <OSCTUN> FRC Oscillator Tuning Rregister value
    #define OSC_TUN_STEP_PERCENTAGE 0.00047     // Oscillator frequency step size of <OSCTUN>

#else
    #pragma message "error: === selected device family not supported by oscillator mcal driver library ==="
#endif

#define OSC_TUN_STEP_FREQUENCY  (volatile int32_t)(OSC_FRC_FREQ * OSC_TUN_STEP_PERCENTAGE)

/* ***************************************************************************************
 *	BASIC DEFINES
 * **************************************************************************************/

/* ===========================================================================
 * OSCCON: OSCILLATOR CONTROL REGISTER
 * ===========================================================================*/

#define REG_OSCCON_VALID_DATA_WRITE_MASK    0x0789
#define REG_OSCCON_VALID_DATA_READ_MASK     0x77A9

#define REG_OSCCON_OSWEN_REQUEST_SWITCH  0b0000000000001000
#define REG_OSCCON_OSWEN_SWITCH_COMPLETE 0b0000000000000000

typedef enum {
    OSCCON_OSWEN_REQUEST_SWITCH = 0b1, // Requests oscillator switch to the selection specified by the NOSC<2:0> bits
    OSCCON_OSWEN_SWITCH_COMPLETE = 0b0 // Oscillator switch is complete
} OSCCON_OSWEN_e; // Oscillator Switch Enable bit

#define REG_OSCCON_CF_CLKSTAT_FAIL 0b0000000000001000
#define REG_OSCCON_CF_CLKSTAT_OK   0b0000000000000000

typedef enum {
    OSCCON_CF_CLKSTAT_FAIL = 0b1, // FSCM has detected a clock failure
    OSCCON_CF_CLKSTAT_OK = 0b0 // FSCM has not detected a clock failure
} OSCCON_CF_e; // Clock Fail Detect bit

#define REG_OSCCON_LOCK_PLL_LOCKED   0b0000000000100000
#define REG_OSCCON_LOCK_PLL_UNLOCKED 0b0000000000000000

typedef enum {
    OSCCON_LOCK_PLL_LOCKED = 0b1, // Indicates that PLL is in lock or PLL start-up timer is satisfied
    OSCCON_LOCK_PLL_UNLOCKED = 0b0 // Indicates that PLL is out of lock, start-up timer is in progress or PLL is disabled
} OSCCON_LOCK_e; // PLL Lock Status bit (read-only)

#define REG_OSCCON_CLKLOCK_LOCKED   0b0000000010000000
#define REG_OSCCON_CLKLOCK_UNLOCKED 0b0000000000000000

typedef enum {
    OSCCON_CLKLOCK_LOCKED = 0b1, // If (FCKSM0 = 1), then clock and PLL configurations are locked; if (FCKSM0 = 0), then clock and PLL configurations may be modified
    OSCCON_CLKLOCK_UNLOCKED = 0b0 // Clock and PLL selections are not locked, configurations may be modified
} OSCCON_CLKLOCK_e; // Clock Lock Enable bit

#define REG_OSCCON_NOSC_FRCDIVN 0b0000011100000000
#define REG_OSCCON_NOSC_BFRC    0b0000011000000000
#define REG_OSCCON_NOSC_LPRC    0b0000010100000000
#define REG_OSCCON_NOSC_PRIPLL  0b0000001100000000
#define REG_OSCCON_NOSC_PRI     0b0000001000000000
#define REG_OSCCON_NOSC_FRCPLL  0b0000000100000000
#define REG_OSCCON_NOSC_FRC     0b0000000000000000

#define REG_OSCCON_COSC_FRCDIVN 0b0111000000000000
#define REG_OSCCON_COSC_BFRC    0b0110000000000000
#define REG_OSCCON_COSC_LPRC    0b0101000000000000
#define REG_OSCCON_COSC_PRIPLL  0b0011000000000000
#define REG_OSCCON_COSC_PRI     0b0010000000000000
#define REG_OSCCON_COSC_FRCPLL  0b0001000000000000
#define REG_OSCCON_COSC_FRC     0b0000000000000000


typedef enum {
    OSCCON_xOSC_FRC = 0b000, // Fast RC Oscillator, no PLL 
    OSCCON_xOSC_FRCPLL = 0b001, // Fast RC Oscillator with PLL
    OSCCON_xOSC_PRI = 0b010, // Primary Oscillator (EC, XT, HS), no PLL
    OSCCON_xOSC_PRIPLL = 0b011, // Primary Oscillator (EC, XT, HS) with PLL
    OSCCON_xOSC_LPRC = 0b101, // Low Power Oscillator for Idle/Sleep Mode
    OSCCON_xOSC_BFRC = 0b110, // Backup Fast RC Oscillator
    OSCCON_xOSC_FRCDIVN = 0b111 // Fast RC Oscillator with variable Divider
} OSCCON_xOSC_TYPE_e ;// Oscillator Type Selection bits

typedef struct {
    volatile OSCCON_OSWEN_e OSWEN : 1; // Oscillator Switch Enable bit
    volatile unsigned : 2; // reserved
    volatile OSCCON_CF_e CF : 1; // Clock Fail Detect bit
    volatile unsigned : 1; // reserved
    volatile OSCCON_LOCK_e LOCK : 1; // PLL Lock Status bit (read only)
    volatile unsigned : 1; // reserved
    volatile OSCCON_CLKLOCK_e CLKLOCK : 1; // Clock Lock Enable bit
    volatile OSCCON_xOSC_TYPE_e NOSC : 3; // New Oscillator Selection bits
    volatile unsigned : 1; // reserved
    volatile OSCCON_xOSC_TYPE_e COSC : 3; // Current Oscillator Selection bits (read only)

} __attribute__((packed)) OSCCON_t; // Oscillator configuration register

typedef union {
    volatile uint16_t value;
    volatile OSCCON_t OSCCON;
} REGBLK_OSCCON_CONFIG_t;

/* ===========================================================================
 * CLKDIV: CLOCK DIVIDER REGISTER
 * ===========================================================================*/

#if defined (__P33SMPS_CH__) || defined (__P33SMPS_CK__)

#define REG_CLKDIV_VALID_DATA_WRITE_MASK    0xFF0F
#define REG_CLKDIV_VALID_DATA_READ_MASK     0xFF0F

#define REG_CLKDIV_PLLPRE_DIV_MASK          0b0000000000001111
#define REG_CLKDIV_PLLPRE_DIVIDER_N1(x)     {(x & REG_CLKDIV_PLLPRE_DIV_MASK)}

typedef enum {
    CLKDIV_PLLDIV_N1_1 = 0b000001, // PLL Input Clock Divider Setting 1:1
    CLKDIV_PLLDIV_N1_2 = 0b000010, // PLL Input Clock Divider Setting 1:2
    CLKDIV_PLLDIV_N1_3 = 0b000011, // PLL Input Clock Divider Setting 1:3
    CLKDIV_PLLDIV_N1_4 = 0b000100, // PLL Input Clock Divider Setting 1:4
    CLKDIV_PLLDIV_N1_5 = 0b000101, // PLL Input Clock Divider Setting 1:5
    CLKDIV_PLLDIV_N1_6 = 0b000110, // PLL Input Clock Divider Setting 1:6
    CLKDIV_PLLDIV_N1_7 = 0b000111, // PLL Input Clock Divider Setting 1:7
    CLKDIV_PLLDIV_N1_8 = 0b001000 // PLL Input Clock Divider Setting 1:8
} CLKDIV_PLLPRE_e; // PLL Phase Detector Input Divider Select bits (also denoted as ?N1?, PLL prescaler)

#else
    #pragma message "error: === selected device family is not supported by oscillator mcal library ==="
#endif

#define REG_CLKDIV_DOZE_DIV_1    0b0000000000000000
#define REG_CLKDIV_DOZE_DIV_2    0b0001000000000000
#define REG_CLKDIV_DOZE_DIV_4    0b0010000000000000
#define REG_CLKDIV_DOZE_DIV_8    0b0011000000000000
#define REG_CLKDIV_DOZE_DIV_16   0b0100000000000000
#define REG_CLKDIV_DOZE_DIV_32   0b0101000000000000
#define REG_CLKDIV_DOZE_DIV_64   0b0110000000000000
#define REG_CLKDIV_DOZE_DIV_128  0b0111000000000000

typedef enum {
    CLKDIV_DOZE_DIV_1 = 0b000, // FCY Clock Divider Setting 1:1
    CLKDIV_DOZE_DIV_2 = 0b001, // FCY Clock Divider Setting 1:2
    CLKDIV_DOZE_DIV_4 = 0b010, // FCY Clock Divider Setting 1:4
    CLKDIV_DOZE_DIV_8 = 0b011, // FCY Clock Divider Setting 1:8
    CLKDIV_DOZE_DIV_16 = 0b100, // FCY Clock Divider Setting 1:16
    CLKDIV_DOZE_DIV_32 = 0b101, // FCY Clock Divider Setting 1:32
    CLKDIV_DOZE_DIV_64 = 0b110, // FCY Clock Divider Setting 1:64
    CLKDIV_DOZE_DIV_128 = 0b111 // FCY Clock Divider Setting 1:128
} CLKDIV_DOZE_e; // Processor Clock Reduction Select bits

#define REG_CLKDIV_DOZEN_ENABLED  0b0000100000000000
#define REG_CLKDIV_DOZEN_DISABLED 0b0000000000000000

typedef enum {
    CLKDIV_DOZEN_ENABLED = 0b1, // DOZE<2:0> field specifies the ratio between the peripheral clocks and the processor clocks
    CLKDIV_DOZEN_DISABLED = 0b0 // Processor clock and peripheral clock ratio is forced to 1:1
} CLKDIV_DOZEN_e; // Doze Mode Enable bit

#define REG_CLKDIV_FRCDIVN_256  0b0000011100000000
#define REG_CLKDIV_FRCDIVN_64   0b0000011000000000
#define REG_CLKDIV_FRCDIVN_32   0b0000010100000000
#define REG_CLKDIV_FRCDIVN_16   0b0000010000000000
#define REG_CLKDIV_FRCDIVN_8    0b0000001100000000
#define REG_CLKDIV_FRCDIVN_4    0b0000001000000000
#define REG_CLKDIV_FRCDIVN_2    0b0000000100000000
#define REG_CLKDIV_FRCDIVN_1    0b0000000000000000

typedef enum CLKDIV_FRCDIVN_e {
    CLKDIV_FRCDIVN_1 = 0b000, // Fast RC Oscillator Clock Divider Setting 1:1
    CLKDIV_FRCDIVN_2 = 0b001, // Fast RC Oscillator Clock Divider Setting 1:2
    CLKDIV_FRCDIVN_4 = 0b010, // Fast RC Oscillator Clock Divider Setting 1:4
    CLKDIV_FRCDIVN_8 = 0b011, // Fast RC Oscillator Clock Divider Setting 1:8
    CLKDIV_FRCDIVN_16 = 0b100, // Fast RC Oscillator Clock Divider Setting 1:16
    CLKDIV_FRCDIVN_32 = 0b101, // Fast RC Oscillator Clock Divider Setting 1:32
    CLKDIV_FRCDIVN_64 = 0b110, // Fast RC Oscillator Clock Divider Setting 1:64
    CLKDIV_FRCDIVN_256 = 0b111 // Fast RC Oscillator Clock Divider Setting 1:256
} CLKDIV_FRCDIVN_t; // Internal Fast RC Oscillator Postscaler bits

#define REG_CLKDIV_ROI_ENABLED  0b1000000000000000
#define REG_CLKDIV_ROI_DISABLED 0b0000000000000000

typedef enum {
    CLKDIV_ROI_ENABLED = 0b1, // Interrupts will clear the DOZEN bit and the processor clock, and the peripheral clock ratio is set to 1:1
    CLKDIV_ROI_DISABLED = 0b0 // Interrupts have no effect on the DOZEN bit
} CLKDIV_ROI_e; // Recover on Interrupt bit

typedef struct {
    volatile CLKDIV_PLLPRE_e PLLPRE : 6; // PLL Phase Detector Input Divider Select bits (also denoted as ?N1?, PLL prescaler)
    volatile unsigned : 2;
    volatile enum CLKDIV_FRCDIVN_e FRCDIV : 3; // Internal Fast RC Oscillator Postscaler bits
    volatile CLKDIV_DOZEN_e DOZEN : 1; // Doze Mode Enable bit
    volatile CLKDIV_DOZE_e DOZE : 3; // Processor Clock Reduction Select bits
    volatile CLKDIV_ROI_e ROI : 1; // Recover on Interrupt bit
} __attribute__((packed)) CLKDIV_t;

typedef union {
    volatile uint16_t value;
    volatile CLKDIV_t CLKDIV;
} REGBLK_CLKDIV_CONFIG_t;


/* ===========================================================================
 * PLLFBD: PLL FEEDBACK DIVIDER REGISTER
 * ===========================================================================*/

#if defined (__P33SMPS_CH__) ||  defined (__P33SMPS_CK__)

#define REG_PLLFBD_VALID_DATA_WRITE_MASK    0x00FF
#define REG_PLLFBD_VALID_DATA_READ_MASK     0x00FF

#define REG_PLLFBD_PLLFBDIV_M_MASK           0b0000000011111111
#define REG_PLLFBD_MULTIPLIER_M(x)     {(x & REG_PLLFBD_PLLFBDIV_M_MASK)}

typedef enum {
    PLLFBD_PLLFBDIV_M_16 = 0b00010000, // PLL Input Clock Multiplier Setting x16
    PLLFBD_PLLFBDIV_M_17 = 0b00010001, // PLL Input Clock Multiplier Setting x17
    PLLFBD_PLLFBDIV_M_18 = 0b00010010, // PLL Input Clock Multiplier Setting x18
    PLLFBD_PLLFBDIV_M_19 = 0b00010011, // PLL Input Clock Multiplier Setting x19
    PLLFBD_PLLFBDIV_M_20 = 0b00010100, // PLL Input Clock Multiplier Setting x20
    PLLFBD_PLLFBDIV_M_21 = 0b00010101, // PLL Input Clock Multiplier Setting x21
    PLLFBD_PLLFBDIV_M_22 = 0b00010110, // PLL Input Clock Multiplier Setting x22
    PLLFBD_PLLFBDIV_M_23 = 0b00010111, // PLL Input Clock Multiplier Setting x23
    PLLFBD_PLLFBDIV_M_24 = 0b00011000, // PLL Input Clock Multiplier Setting x24
    PLLFBD_PLLFBDIV_M_25 = 0b00011001, // PLL Input Clock Multiplier Setting x25
    PLLFBD_PLLFBDIV_M_26 = 0b00011010, // PLL Input Clock Multiplier Setting x26
    PLLFBD_PLLFBDIV_M_27 = 0b00011011, // PLL Input Clock Multiplier Setting x27
    PLLFBD_PLLFBDIV_M_28 = 0b00011100, // PLL Input Clock Multiplier Setting x28
    PLLFBD_PLLFBDIV_M_29 = 0b00011101, // PLL Input Clock Multiplier Setting x29
    PLLFBD_PLLFBDIV_M_30 = 0b00011110, // PLL Input Clock Multiplier Setting x30
    PLLFBD_PLLFBDIV_M_31 = 0b00011111, // PLL Input Clock Multiplier Setting x31
    PLLFBD_PLLFBDIV_M_32 = 0b00100000, // PLL Input Clock Multiplier Setting x32
    PLLFBD_PLLFBDIV_M_33 = 0b00100001, // PLL Input Clock Multiplier Setting x33
    PLLFBD_PLLFBDIV_M_34 = 0b00100010, // PLL Input Clock Multiplier Setting x34
    PLLFBD_PLLFBDIV_M_35 = 0b00100011, // PLL Input Clock Multiplier Setting x35
    PLLFBD_PLLFBDIV_M_36 = 0b00100100, // PLL Input Clock Multiplier Setting x36
    PLLFBD_PLLFBDIV_M_37 = 0b00100101, // PLL Input Clock Multiplier Setting x37
    PLLFBD_PLLFBDIV_M_38 = 0b00100110, // PLL Input Clock Multiplier Setting x38
    PLLFBD_PLLFBDIV_M_39 = 0b00100111, // PLL Input Clock Multiplier Setting x39
    PLLFBD_PLLFBDIV_M_40 = 0b00101000, // PLL Input Clock Multiplier Setting x40
    PLLFBD_PLLFBDIV_M_41 = 0b00101001, // PLL Input Clock Multiplier Setting x41
    PLLFBD_PLLFBDIV_M_42 = 0b00101010, // PLL Input Clock Multiplier Setting x42
    PLLFBD_PLLFBDIV_M_43 = 0b00101011, // PLL Input Clock Multiplier Setting x43
    PLLFBD_PLLFBDIV_M_44 = 0b00101100, // PLL Input Clock Multiplier Setting x44
    PLLFBD_PLLFBDIV_M_45 = 0b00101101, // PLL Input Clock Multiplier Setting x45
    PLLFBD_PLLFBDIV_M_46 = 0b00101110, // PLL Input Clock Multiplier Setting x46
    PLLFBD_PLLFBDIV_M_47 = 0b00101111, // PLL Input Clock Multiplier Setting x47
    PLLFBD_PLLFBDIV_M_48 = 0b00110000, // PLL Input Clock Multiplier Setting x48
    PLLFBD_PLLFBDIV_M_49 = 0b00110001, // PLL Input Clock Multiplier Setting x49
    PLLFBD_PLLFBDIV_M_50 = 0b00110010, // PLL Input Clock Multiplier Setting x50
    PLLFBD_PLLFBDIV_M_51 = 0b00110011, // PLL Input Clock Multiplier Setting x51
    PLLFBD_PLLFBDIV_M_52 = 0b00110100, // PLL Input Clock Multiplier Setting x52
    PLLFBD_PLLFBDIV_M_53 = 0b00110101, // PLL Input Clock Multiplier Setting x53
    PLLFBD_PLLFBDIV_M_54 = 0b00110110, // PLL Input Clock Multiplier Setting x54
    PLLFBD_PLLFBDIV_M_55 = 0b00110111, // PLL Input Clock Multiplier Setting x55
    PLLFBD_PLLFBDIV_M_56 = 0b00111000, // PLL Input Clock Multiplier Setting x56
    PLLFBD_PLLFBDIV_M_57 = 0b00111001, // PLL Input Clock Multiplier Setting x57
    PLLFBD_PLLFBDIV_M_58 = 0b00111010, // PLL Input Clock Multiplier Setting x58
    PLLFBD_PLLFBDIV_M_59 = 0b00111011, // PLL Input Clock Multiplier Setting x59
    PLLFBD_PLLFBDIV_M_60 = 0b00111100, // PLL Input Clock Multiplier Setting x60
    PLLFBD_PLLFBDIV_M_61 = 0b00111101, // PLL Input Clock Multiplier Setting x61
    PLLFBD_PLLFBDIV_M_62 = 0b00111110, // PLL Input Clock Multiplier Setting x62
    PLLFBD_PLLFBDIV_M_63 = 0b00111111, // PLL Input Clock Multiplier Setting x63
    PLLFBD_PLLFBDIV_M_64 = 0b01000000, // PLL Input Clock Multiplier Setting x64
    PLLFBD_PLLFBDIV_M_65 = 0b01000001, // PLL Input Clock Multiplier Setting x65
    PLLFBD_PLLFBDIV_M_66 = 0b01000010, // PLL Input Clock Multiplier Setting x66
    PLLFBD_PLLFBDIV_M_67 = 0b01000011, // PLL Input Clock Multiplier Setting x67
    PLLFBD_PLLFBDIV_M_68 = 0b01000100, // PLL Input Clock Multiplier Setting x68
    PLLFBD_PLLFBDIV_M_69 = 0b01000101, // PLL Input Clock Multiplier Setting x69
    PLLFBD_PLLFBDIV_M_70 = 0b01000110, // PLL Input Clock Multiplier Setting x70
    PLLFBD_PLLFBDIV_M_71 = 0b01000111, // PLL Input Clock Multiplier Setting x71
    PLLFBD_PLLFBDIV_M_72 = 0b01001000, // PLL Input Clock Multiplier Setting x72
    PLLFBD_PLLFBDIV_M_73 = 0b01001001, // PLL Input Clock Multiplier Setting x73
    PLLFBD_PLLFBDIV_M_74 = 0b01001010, // PLL Input Clock Multiplier Setting x74
    PLLFBD_PLLFBDIV_M_75 = 0b01001011, // PLL Input Clock Multiplier Setting x75
    PLLFBD_PLLFBDIV_M_76 = 0b01001100, // PLL Input Clock Multiplier Setting x76
    PLLFBD_PLLFBDIV_M_77 = 0b01001101, // PLL Input Clock Multiplier Setting x77
    PLLFBD_PLLFBDIV_M_78 = 0b01001110, // PLL Input Clock Multiplier Setting x78
    PLLFBD_PLLFBDIV_M_79 = 0b01001111, // PLL Input Clock Multiplier Setting x79
    PLLFBD_PLLFBDIV_M_80 = 0b01010000, // PLL Input Clock Multiplier Setting x80
    PLLFBD_PLLFBDIV_M_81 = 0b01010001, // PLL Input Clock Multiplier Setting x81
    PLLFBD_PLLFBDIV_M_82 = 0b01010010, // PLL Input Clock Multiplier Setting x82
    PLLFBD_PLLFBDIV_M_83 = 0b01010011, // PLL Input Clock Multiplier Setting x83
    PLLFBD_PLLFBDIV_M_84 = 0b01010100, // PLL Input Clock Multiplier Setting x84
    PLLFBD_PLLFBDIV_M_85 = 0b01010101, // PLL Input Clock Multiplier Setting x85
    PLLFBD_PLLFBDIV_M_86 = 0b01010110, // PLL Input Clock Multiplier Setting x86
    PLLFBD_PLLFBDIV_M_87 = 0b01010111, // PLL Input Clock Multiplier Setting x87
    PLLFBD_PLLFBDIV_M_88 = 0b01011000, // PLL Input Clock Multiplier Setting x88
    PLLFBD_PLLFBDIV_M_89 = 0b01011001, // PLL Input Clock Multiplier Setting x89
    PLLFBD_PLLFBDIV_M_90 = 0b01011010, // PLL Input Clock Multiplier Setting x90
    PLLFBD_PLLFBDIV_M_91 = 0b01011011, // PLL Input Clock Multiplier Setting x91
    PLLFBD_PLLFBDIV_M_92 = 0b01011100, // PLL Input Clock Multiplier Setting x92
    PLLFBD_PLLFBDIV_M_93 = 0b01011101, // PLL Input Clock Multiplier Setting x93
    PLLFBD_PLLFBDIV_M_94 = 0b01011110, // PLL Input Clock Multiplier Setting x94
    PLLFBD_PLLFBDIV_M_95 = 0b01011111, // PLL Input Clock Multiplier Setting x95
    PLLFBD_PLLFBDIV_M_96 = 0b01100000, // PLL Input Clock Multiplier Setting x96
    PLLFBD_PLLFBDIV_M_97 = 0b01100001, // PLL Input Clock Multiplier Setting x97
    PLLFBD_PLLFBDIV_M_98 = 0b01100010, // PLL Input Clock Multiplier Setting x98
    PLLFBD_PLLFBDIV_M_99 = 0b01100011, // PLL Input Clock Multiplier Setting x99
    PLLFBD_PLLFBDIV_M_100 = 0b01100100, // PLL Input Clock Multiplier Setting x100
    PLLFBD_PLLFBDIV_M_101 = 0b01100101, // PLL Input Clock Multiplier Setting x101
    PLLFBD_PLLFBDIV_M_102 = 0b01100110, // PLL Input Clock Multiplier Setting x102
    PLLFBD_PLLFBDIV_M_103 = 0b01100111, // PLL Input Clock Multiplier Setting x103
    PLLFBD_PLLFBDIV_M_104 = 0b01101000, // PLL Input Clock Multiplier Setting x104
    PLLFBD_PLLFBDIV_M_105 = 0b01101001, // PLL Input Clock Multiplier Setting x105
    PLLFBD_PLLFBDIV_M_106 = 0b01101010, // PLL Input Clock Multiplier Setting x106
    PLLFBD_PLLFBDIV_M_107 = 0b01101011, // PLL Input Clock Multiplier Setting x107
    PLLFBD_PLLFBDIV_M_108 = 0b01101100, // PLL Input Clock Multiplier Setting x108
    PLLFBD_PLLFBDIV_M_109 = 0b01101101, // PLL Input Clock Multiplier Setting x109
    PLLFBD_PLLFBDIV_M_110 = 0b01101110, // PLL Input Clock Multiplier Setting x110
    PLLFBD_PLLFBDIV_M_111 = 0b01101111, // PLL Input Clock Multiplier Setting x111
    PLLFBD_PLLFBDIV_M_112 = 0b01110000, // PLL Input Clock Multiplier Setting x112
    PLLFBD_PLLFBDIV_M_113 = 0b01110001, // PLL Input Clock Multiplier Setting x113
    PLLFBD_PLLFBDIV_M_114 = 0b01110010, // PLL Input Clock Multiplier Setting x114
    PLLFBD_PLLFBDIV_M_115 = 0b01110011, // PLL Input Clock Multiplier Setting x115
    PLLFBD_PLLFBDIV_M_116 = 0b01110100, // PLL Input Clock Multiplier Setting x116
    PLLFBD_PLLFBDIV_M_117 = 0b01110101, // PLL Input Clock Multiplier Setting x117
    PLLFBD_PLLFBDIV_M_118 = 0b01110110, // PLL Input Clock Multiplier Setting x118
    PLLFBD_PLLFBDIV_M_119 = 0b01110111, // PLL Input Clock Multiplier Setting x119
    PLLFBD_PLLFBDIV_M_120 = 0b01111000, // PLL Input Clock Multiplier Setting x120
    PLLFBD_PLLFBDIV_M_121 = 0b01111001, // PLL Input Clock Multiplier Setting x121
    PLLFBD_PLLFBDIV_M_122 = 0b01111010, // PLL Input Clock Multiplier Setting x122
    PLLFBD_PLLFBDIV_M_123 = 0b01111011, // PLL Input Clock Multiplier Setting x123
    PLLFBD_PLLFBDIV_M_124 = 0b01111100, // PLL Input Clock Multiplier Setting x124
    PLLFBD_PLLFBDIV_M_125 = 0b01111101, // PLL Input Clock Multiplier Setting x125
    PLLFBD_PLLFBDIV_M_126 = 0b01111110, // PLL Input Clock Multiplier Setting x126
    PLLFBD_PLLFBDIV_M_127 = 0b01111111, // PLL Input Clock Multiplier Setting x127
    PLLFBD_PLLFBDIV_M_128 = 0b10000000, // PLL Input Clock Multiplier Setting x128
    PLLFBD_PLLFBDIV_M_129 = 0b10000001, // PLL Input Clock Multiplier Setting x129
    PLLFBD_PLLFBDIV_M_130 = 0b10000010, // PLL Input Clock Multiplier Setting x130
    PLLFBD_PLLFBDIV_M_131 = 0b10000011, // PLL Input Clock Multiplier Setting x131
    PLLFBD_PLLFBDIV_M_132 = 0b10000100, // PLL Input Clock Multiplier Setting x132
    PLLFBD_PLLFBDIV_M_133 = 0b10000101, // PLL Input Clock Multiplier Setting x133
    PLLFBD_PLLFBDIV_M_134 = 0b10000110, // PLL Input Clock Multiplier Setting x134
    PLLFBD_PLLFBDIV_M_135 = 0b10000111, // PLL Input Clock Multiplier Setting x135
    PLLFBD_PLLFBDIV_M_136 = 0b10001000, // PLL Input Clock Multiplier Setting x136
    PLLFBD_PLLFBDIV_M_137 = 0b10001001, // PLL Input Clock Multiplier Setting x137
    PLLFBD_PLLFBDIV_M_138 = 0b10001010, // PLL Input Clock Multiplier Setting x138
    PLLFBD_PLLFBDIV_M_139 = 0b10001011, // PLL Input Clock Multiplier Setting x139
    PLLFBD_PLLFBDIV_M_140 = 0b10001100, // PLL Input Clock Multiplier Setting x140
    PLLFBD_PLLFBDIV_M_141 = 0b10001101, // PLL Input Clock Multiplier Setting x141
    PLLFBD_PLLFBDIV_M_142 = 0b10001110, // PLL Input Clock Multiplier Setting x142
    PLLFBD_PLLFBDIV_M_143 = 0b10001111, // PLL Input Clock Multiplier Setting x143
    PLLFBD_PLLFBDIV_M_144 = 0b10010000, // PLL Input Clock Multiplier Setting x144
    PLLFBD_PLLFBDIV_M_145 = 0b10010001, // PLL Input Clock Multiplier Setting x145
    PLLFBD_PLLFBDIV_M_146 = 0b10010010, // PLL Input Clock Multiplier Setting x146
    PLLFBD_PLLFBDIV_M_147 = 0b10010011, // PLL Input Clock Multiplier Setting x147
    PLLFBD_PLLFBDIV_M_148 = 0b10010100, // PLL Input Clock Multiplier Setting x148
    PLLFBD_PLLFBDIV_M_149 = 0b10010101, // PLL Input Clock Multiplier Setting x149
    PLLFBD_PLLFBDIV_M_150 = 0b10010110, // PLL Input Clock Multiplier Setting x150
    PLLFBD_PLLFBDIV_M_151 = 0b10010111, // PLL Input Clock Multiplier Setting x151
    PLLFBD_PLLFBDIV_M_152 = 0b10011000, // PLL Input Clock Multiplier Setting x152
    PLLFBD_PLLFBDIV_M_153 = 0b10011001, // PLL Input Clock Multiplier Setting x153
    PLLFBD_PLLFBDIV_M_154 = 0b10011010, // PLL Input Clock Multiplier Setting x154
    PLLFBD_PLLFBDIV_M_155 = 0b10011011, // PLL Input Clock Multiplier Setting x155
    PLLFBD_PLLFBDIV_M_156 = 0b10011100, // PLL Input Clock Multiplier Setting x156
    PLLFBD_PLLFBDIV_M_157 = 0b10011101, // PLL Input Clock Multiplier Setting x157
    PLLFBD_PLLFBDIV_M_158 = 0b10011110, // PLL Input Clock Multiplier Setting x158
    PLLFBD_PLLFBDIV_M_159 = 0b10011111, // PLL Input Clock Multiplier Setting x159
    PLLFBD_PLLFBDIV_M_160 = 0b10100000, // PLL Input Clock Multiplier Setting x160
    PLLFBD_PLLFBDIV_M_161 = 0b10100001, // PLL Input Clock Multiplier Setting x161
    PLLFBD_PLLFBDIV_M_162 = 0b10100010, // PLL Input Clock Multiplier Setting x162
    PLLFBD_PLLFBDIV_M_163 = 0b10100011, // PLL Input Clock Multiplier Setting x163
    PLLFBD_PLLFBDIV_M_164 = 0b10100100, // PLL Input Clock Multiplier Setting x164
    PLLFBD_PLLFBDIV_M_165 = 0b10100101, // PLL Input Clock Multiplier Setting x165
    PLLFBD_PLLFBDIV_M_166 = 0b10100110, // PLL Input Clock Multiplier Setting x166
    PLLFBD_PLLFBDIV_M_167 = 0b10100111, // PLL Input Clock Multiplier Setting x167
    PLLFBD_PLLFBDIV_M_168 = 0b10101000, // PLL Input Clock Multiplier Setting x168
    PLLFBD_PLLFBDIV_M_169 = 0b10101001, // PLL Input Clock Multiplier Setting x169
    PLLFBD_PLLFBDIV_M_170 = 0b10101010, // PLL Input Clock Multiplier Setting x170
    PLLFBD_PLLFBDIV_M_171 = 0b10101011, // PLL Input Clock Multiplier Setting x171
    PLLFBD_PLLFBDIV_M_172 = 0b10101100, // PLL Input Clock Multiplier Setting x172
    PLLFBD_PLLFBDIV_M_173 = 0b10101101, // PLL Input Clock Multiplier Setting x173
    PLLFBD_PLLFBDIV_M_174 = 0b10101110, // PLL Input Clock Multiplier Setting x174
    PLLFBD_PLLFBDIV_M_175 = 0b10101111, // PLL Input Clock Multiplier Setting x175
    PLLFBD_PLLFBDIV_M_176 = 0b10110000, // PLL Input Clock Multiplier Setting x176
    PLLFBD_PLLFBDIV_M_177 = 0b10110001, // PLL Input Clock Multiplier Setting x177
    PLLFBD_PLLFBDIV_M_178 = 0b10110010, // PLL Input Clock Multiplier Setting x178
    PLLFBD_PLLFBDIV_M_179 = 0b10110011, // PLL Input Clock Multiplier Setting x179
    PLLFBD_PLLFBDIV_M_180 = 0b10110100, // PLL Input Clock Multiplier Setting x180
    PLLFBD_PLLFBDIV_M_181 = 0b10110101, // PLL Input Clock Multiplier Setting x181
    PLLFBD_PLLFBDIV_M_182 = 0b10110110, // PLL Input Clock Multiplier Setting x182
    PLLFBD_PLLFBDIV_M_183 = 0b10110111, // PLL Input Clock Multiplier Setting x183
    PLLFBD_PLLFBDIV_M_184 = 0b10111000, // PLL Input Clock Multiplier Setting x184
    PLLFBD_PLLFBDIV_M_185 = 0b10111001, // PLL Input Clock Multiplier Setting x185
    PLLFBD_PLLFBDIV_M_186 = 0b10111010, // PLL Input Clock Multiplier Setting x186
    PLLFBD_PLLFBDIV_M_187 = 0b10111011, // PLL Input Clock Multiplier Setting x187
    PLLFBD_PLLFBDIV_M_188 = 0b10111100, // PLL Input Clock Multiplier Setting x188
    PLLFBD_PLLFBDIV_M_189 = 0b10111101, // PLL Input Clock Multiplier Setting x189
    PLLFBD_PLLFBDIV_M_190 = 0b10111110, // PLL Input Clock Multiplier Setting x190
    PLLFBD_PLLFBDIV_M_191 = 0b10111111, // PLL Input Clock Multiplier Setting x191
    PLLFBD_PLLFBDIV_M_192 = 0b11000000, // PLL Input Clock Multiplier Setting x192
    PLLFBD_PLLFBDIV_M_193 = 0b11000001, // PLL Input Clock Multiplier Setting x193
    PLLFBD_PLLFBDIV_M_194 = 0b11000010, // PLL Input Clock Multiplier Setting x194
    PLLFBD_PLLFBDIV_M_195 = 0b11000011, // PLL Input Clock Multiplier Setting x195
    PLLFBD_PLLFBDIV_M_196 = 0b11000100, // PLL Input Clock Multiplier Setting x196
    PLLFBD_PLLFBDIV_M_197 = 0b11000101, // PLL Input Clock Multiplier Setting x197
    PLLFBD_PLLFBDIV_M_198 = 0b11000110, // PLL Input Clock Multiplier Setting x198
    PLLFBD_PLLFBDIV_M_199 = 0b11000111, // PLL Input Clock Multiplier Setting x199
    PLLFBD_PLLFBDIV_M_200 = 0b11001000 // PLL Input Clock Multiplier Setting x200
} PLLFBD_PLLFBDIV_e; // PLL Feedback Divider bits (also denoted as ?M?, PLL multiplier)

typedef struct {
    volatile PLLFBD_PLLFBDIV_e PLLFBDIV : 8; // PLL Feedback Divider bits (also denoted as ?M?, PLL multiplier)
    volatile unsigned : 8; // reserved
} PLLFBD_t;

#else
    #pragma message "error: === selected device family is not supported by oscillator mcal library ==="
#endif

typedef union {
    volatile uint16_t value;
    volatile PLLFBD_t PLLFBD;
} REGBLK_PLLFBD_CONFIG_t;


/* ===========================================================================
 * OSCTUN: FRC OSCILLATOR TUNING REGISTER
 * ===========================================================================*/

#define REG_OSCTUN_VALID_DATA_WRITE_MASK    0x003F
#define REG_OSCTUN_VALID_DATA_READ_MASK     0x003F

#define REG_OSCTUN_TUNE_VALUE_MASK           0b0000000000111111
#define REG_OSCTUN_TUNE_VALUE(x)     {(x & REG_OSCTUN_TUNE_VALUE_MASK)}

#if defined (__P33SMPS_CH__) ||  defined (__P33SMPS_CK__)

typedef enum OSCTUN_TUN_e {
    OSCTUN_TUN_MINUS_31 = 0b100001, // Center frequency -1.457% (=7.88344 MHz)
    OSCTUN_TUN_MINUS_30 = 0b100010, // Center frequency -1.41% (=7.8872 MHz)
    OSCTUN_TUN_MINUS_29 = 0b100011, // Center frequency -1.363% (=7.89096 MHz)
    OSCTUN_TUN_MINUS_28 = 0b100100, // Center frequency -1.316% (=7.89472 MHz)
    OSCTUN_TUN_MINUS_27 = 0b100101, // Center frequency -1.269% (=7.89848 MHz)
    OSCTUN_TUN_MINUS_26 = 0b100110, // Center frequency -1.222% (=7.90224 MHz)
    OSCTUN_TUN_MINUS_25 = 0b100111, // Center frequency -1.175% (=7.906 MHz)
    OSCTUN_TUN_MINUS_24 = 0b101000, // Center frequency -1.128% (=7.90976 MHz)
    OSCTUN_TUN_MINUS_23 = 0b101001, // Center frequency -1.081% (=7.91352 MHz)
    OSCTUN_TUN_MINUS_22 = 0b101010, // Center frequency -1.034% (=7.91728 MHz)
    OSCTUN_TUN_MINUS_21 = 0b101011, // Center frequency -0.987% (=7.92104 MHz)
    OSCTUN_TUN_MINUS_20 = 0b101100, // Center frequency -0.94% (=7.9248 MHz)
    OSCTUN_TUN_MINUS_19 = 0b101101, // Center frequency -0.893% (=7.92856 MHz)
    OSCTUN_TUN_MINUS_18 = 0b101110, // Center frequency -0.846% (=7.93232 MHz)
    OSCTUN_TUN_MINUS_17 = 0b101111, // Center frequency -0.799% (=7.93608 MHz)
    OSCTUN_TUN_MINUS_16 = 0b110000, // Center frequency -0.752% (=7.93984 MHz)
    OSCTUN_TUN_MINUS_15 = 0b110001, // Center frequency -0.705% (=7.9436 MHz)
    OSCTUN_TUN_MINUS_14 = 0b110010, // Center frequency -0.658% (=7.94736 MHz)
    OSCTUN_TUN_MINUS_13 = 0b110011, // Center frequency -0.611% (=7.95112 MHz)
    OSCTUN_TUN_MINUS_12 = 0b110100, // Center frequency -0.564% (=7.95488 MHz)
    OSCTUN_TUN_MINUS_11 = 0b110101, // Center frequency -0.517% (=7.95864 MHz)
    OSCTUN_TUN_MINUS_10 = 0b110110, // Center frequency -0.47% (=7.9624 MHz)
    OSCTUN_TUN_MINUS_9 = 0b110111, // Center frequency -0.423% (=7.96616 MHz)
    OSCTUN_TUN_MINUS_8 = 0b111000, // Center frequency -0.376% (=7.96992 MHz)
    OSCTUN_TUN_MINUS_7 = 0b111001, // Center frequency -0.329% (=7.97368 MHz)
    OSCTUN_TUN_MINUS_6 = 0b111010, // Center frequency -0.282% (=7.97744 MHz)
    OSCTUN_TUN_MINUS_5 = 0b111011, // Center frequency -0.235% (=7.9812 MHz)
    OSCTUN_TUN_MINUS_4 = 0b111100, // Center frequency -0.188% (=7.98496 MHz)
    OSCTUN_TUN_MINUS_3 = 0b111101, // Center frequency -0.141% (=7.98872 MHz)
    OSCTUN_TUN_MINUS_2 = 0b111110, // Center frequency -0.094% (=7.99248 MHz)
    OSCTUN_TUN_MINUS_1 = 0b111111, // Center frequency -0.047% (=7.99624 MHz)
    OSCTUN_TUN_NOMINAL = 0b000000, // Center frequency +0% (=8 MHz)
    OSCTUN_TUN_PLUS_1 = 0b000001, // Center frequency +0.047% (=8.00376 MHz)
    OSCTUN_TUN_PLUS_2 = 0b000010, // Center frequency +0.094% (=8.00752 MHz)
    OSCTUN_TUN_PLUS_3 = 0b000011, // Center frequency +0.141% (=8.01128 MHz)
    OSCTUN_TUN_PLUS_4 = 0b000100, // Center frequency +0.188% (=8.01504 MHz)
    OSCTUN_TUN_PLUS_5 = 0b000101, // Center frequency +0.235% (=8.0188 MHz)
    OSCTUN_TUN_PLUS_6 = 0b000110, // Center frequency +0.282% (=8.02256 MHz)
    OSCTUN_TUN_PLUS_7 = 0b000111, // Center frequency +0.329% (=8.02632 MHz)
    OSCTUN_TUN_PLUS_8 = 0b001000, // Center frequency +0.376% (=8.03008 MHz)
    OSCTUN_TUN_PLUS_9 = 0b001001, // Center frequency +0.423% (=8.03384 MHz)
    OSCTUN_TUN_PLUS_10 = 0b001010, // Center frequency +0.47% (=8.0376 MHz)
    OSCTUN_TUN_PLUS_11 = 0b001011, // Center frequency +0.517% (=8.04136 MHz)
    OSCTUN_TUN_PLUS_12 = 0b001100, // Center frequency +0.564% (=8.04512 MHz)
    OSCTUN_TUN_PLUS_13 = 0b001101, // Center frequency +0.611% (=8.04888 MHz)
    OSCTUN_TUN_PLUS_14 = 0b001110, // Center frequency +0.658% (=8.05264 MHz)
    OSCTUN_TUN_PLUS_15 = 0b001111, // Center frequency +0.705% (=8.0564 MHz)
    OSCTUN_TUN_PLUS_16 = 0b010000, // Center frequency +0.752% (=8.06016 MHz)
    OSCTUN_TUN_PLUS_17 = 0b010001, // Center frequency +0.799% (=8.06392 MHz)
    OSCTUN_TUN_PLUS_18 = 0b010010, // Center frequency +0.846% (=8.06768 MHz)
    OSCTUN_TUN_PLUS_19 = 0b010011, // Center frequency +0.893% (=8.07144 MHz)
    OSCTUN_TUN_PLUS_20 = 0b010100, // Center frequency +0.94% (=8.0752 MHz)
    OSCTUN_TUN_PLUS_21 = 0b010101, // Center frequency +0.987% (=8.07896 MHz)
    OSCTUN_TUN_PLUS_22 = 0b010110, // Center frequency +1.034% (=8.08272 MHz)
    OSCTUN_TUN_PLUS_23 = 0b010111, // Center frequency +1.081% (=8.08648 MHz)
    OSCTUN_TUN_PLUS_24 = 0b011000, // Center frequency +1.128% (=8.09024 MHz)
    OSCTUN_TUN_PLUS_25 = 0b011001, // Center frequency +1.175% (=8.094 MHz)
    OSCTUN_TUN_PLUS_26 = 0b011010, // Center frequency +1.222% (=8.09776 MHz)
    OSCTUN_TUN_PLUS_27 = 0b011011, // Center frequency +1.269% (=8.10152 MHz)
    OSCTUN_TUN_PLUS_28 = 0b011100, // Center frequency +1.316% (=8.10528 MHz)
    OSCTUN_TUN_PLUS_29 = 0b011101, // Center frequency +1.363% (=8.10904 MHz)
    OSCTUN_TUN_PLUS_30 = 0b011110, // Center frequency +1.41% (=8.1128 MHz)
    OSCTUN_TUN_PLUS_31 = 0b011111 // Center frequency +1.457% (=8.11656 MHz)
} OSCTUN_TUN_t; // FRC Oscillator Tuning bits

#else
    #pragma message "error: === selected device family is not supported by oscillator mcal library ==="
#endif

typedef struct {
    volatile enum OSCTUN_TUN_e TUN : 6; // FRC Oscillator Tuning bits
    volatile unsigned : 10; // reserved
} __attribute__((packed)) OSCTUN_t;

typedef union {
    volatile uint16_t value;
    volatile OSCTUN_t OSCTUN;
} REGBLK_OSCTUN_CONFIG_t;


/* ===========================================================================
 * PLLDIV: PLL OUTPUT DIVIDER REGISTER
 * ===========================================================================*/

#if defined (__P33SMPS_CH__) ||  defined (__P33SMPS_CK__)

#define REG_PLLDIV_VALID_DATA_WRITE_MASK    0x0377
#define REG_PLLDIV_VALID_DATA_READ_MASK     0x0377

#define REG_PLLDIV_POST1DIV_N2_MASK         0b0000000001110000
#define REG_PLLDIV_POST1DIV_N2(x)     {((x << 4) & REG_PLLDIV_POST1DIV_N2_MASK)}

#define REG_PLLDIV_POST2DIV_N3_MASK         0b0000000000000111
#define REG_PLLDIV_POST2DIV_N3(x)     {(x & REG_PLLDIV_POST1DIV_N3_MASK)}

typedef enum {
    PLLDIV_POST2DIV_N2N3_1 = 0b001, // PLL Output Divider Ratio bits 1:1
    PLLDIV_POST2DIV_N2N3_2 = 0b010, // PLL Output Divider Ratio bits 1:2
    PLLDIV_POST2DIV_N2N3_3 = 0b011, // PLL Output Divider Ratio bits 1:3
    PLLDIV_POST2DIV_N2N3_4 = 0b100, // PLL Output Divider Ratio bits 1:4
    PLLDIV_POST2DIV_N2N3_5 = 0b101, // PLL Output Divider Ratio bits 1:5
    PLLDIV_POST2DIV_N2N3_6 = 0b110, // PLL Output Divider Ratio bits 1:6
    PLLDIV_POST2DIV_N2N3_7 = 0b111 // PLL Output Divider Ratio bits 1:7
} PLLDIV_POSTxDIV_e; // PLL Output Divider Ratio bits (also denoted as ?N2? and ?N3?, PLL divider)

#define REG_PLLDIV_VCODIV_FVCO_DIV_BY_1	0b0000001100000000
#define REG_PLLDIV_VCODIV_FVCO_DIV_BY_2	0b0000001000000000
#define REG_PLLDIV_VCODIV_FVCO_DIV_BY_3	0b0000000100000000
#define REG_PLLDIV_VCODIV_FVCO_DIV_BY_4	0b0000000000000000

typedef enum {
    PLLDIV_VCODIV_FVCO_DIV_BY_1 = 0b11, // PLL VCO Output Divider 1:1
    PLLDIV_VCODIV_FVCO_DIV_BY_2 = 0b10, // PLL VCO Output Divider 1:2
    PLLDIV_VCODIV_FVCO_DIV_BY_3 = 0b01, // PLL VCO Output Divider 1:3
    PLLDIV_VCODIV_FVCO_DIV_BY_4 = 0b00 // PLL VCO Output Divider 1:4
} PLLDIV_VCODIV_e; // PLL VCO Output Divider Select bits

typedef struct {
    volatile PLLDIV_POSTxDIV_e POST2DIV : 3;
    volatile unsigned : 1;
    volatile PLLDIV_POSTxDIV_e POST1DIV : 3;
    volatile unsigned : 1;
    volatile PLLDIV_VCODIV_e VCODIV : 2;
    volatile unsigned : 6;
} __attribute__((packed)) PLLDIV_t;

typedef union {
    volatile uint16_t value;
    volatile PLLDIV_t PLLDIV;
} REGBLK_PLLDIV_CONFIG_t;

#endif


/* ===========================================================================
 * ACLKCON1: AUXILIARY CLOCK CONTROL REGISTER
 * ===========================================================================*/

#if defined (__P33SMPS_CH__) ||  defined (__P33SMPS_CK__)

#define REG_ACLKCON_VALID_DATA_WRITE_MASK    0xC10F
#define REG_ACLKCON_VALID_DATA_READ_MASK     0xC10F

#define REG_ACLKCON_APLLPRE_DIV_MASK          0b0000000000001111
#define REG_ACLKCON_APLLPRE_DIVIDER_N1(x)     {(x & REG_ACLKCON_APLLPRE_DIV_MASK)}

typedef enum {
    ACLKCON_APLLDIV_N1_1 = 0b000001, // APLL Input Clock Divider Setting 1:1
    ACLKCON_APLLDIV_N1_2 = 0b000010, // APLL Input Clock Divider Setting 1:2
    ACLKCON_APLLDIV_N1_3 = 0b000011, // APLL Input Clock Divider Setting 1:3
    ACLKCON_APLLDIV_N1_4 = 0b000100, // APLL Input Clock Divider Setting 1:4
    ACLKCON_APLLDIV_N1_5 = 0b000101, // APLL Input Clock Divider Setting 1:5
    ACLKCON_APLLDIV_N1_6 = 0b000110, // APLL Input Clock Divider Setting 1:6
    ACLKCON_APLLDIV_N1_7 = 0b000111, // APLL Input Clock Divider Setting 1:7
    ACLKCON_APLLDIV_N1_8 = 0b001000 // APLL Input Clock Divider Setting 1:8
} ACLKCON_APLLPRE_e; // PLL Phase Detector Input Divider Select bits (also denoted as ?N1?, PLL prescaler)

#define REG_ACLKCON_FRCSEL_FRC  0b0000000100000000
#define REG_ACLKCON_FRCSEL_PRI  0b0000000000000000

typedef enum {
    PLLDIV_ACLKCON_FRCSEL_FRC = 0b1, // FRC is the clock source for APLL
    PLLDIV_ACLKCON_FRCSEL_PRI = 0b0 // Primary Oscillator is the clock source for APLL
} ACLKCON_FRCSEL_e; // APLL Clock Source Select bit

#define REG_ACLKCON_APLLCK_STAT_LOCKED	  0b0100000000000000
#define REG_ACLKCON_APLLCK_STAT_UNLOCKED  0b0000000000000000

typedef enum {
    ACLKCON_APLLCK_STAT_LOCKED = 0b1, // APLL Phase-Locked State Status bit
    ACLKCON_APLLCK_STAT_UNLOCKED = 0b0 // Auxiliary PLL is not in lock
} ACLKCON_APLLCK_e; // APLL Phase-Locked State Status bit

#define REG_ACLKCON_APLLEN_ENABLED	 0b1000000000000000
#define REG_ACLKCON_APLLEN_DISABLED  0b0000000000000000

typedef enum {
    ACLKCON_APLLEN_ENABLED = 0b1, // AFPLLO is connected to the APLL post-divider output (bypass disabled)
    ACLKCON_APLLEN_DISABLED = 0b0 // AFPLLO is connected to the APLL input clock (bypass enabled)
} ACLKCON_APLLEN_e; // Auxiliary PLL Enable/Bypass select bit

typedef struct {
    volatile ACLKCON_APLLPRE_e APLLPRE : 6;
    volatile unsigned : 2;
    volatile ACLKCON_FRCSEL_e FRCSEL : 1;
    volatile unsigned : 5;
    volatile ACLKCON_APLLCK_e APLLCK : 1;
    volatile ACLKCON_APLLEN_e APLLEN : 1;
} ACLKCON_t;

typedef union {
    volatile uint16_t value;
    volatile ACLKCON_t ACLKCON; // APLL Feedback Divider bits
} REGBLK_ACLKCON_CONFIG_t;

#else
    #pragma message "error: === selected device family is not supported by oscillator mcal library ==="
#endif

/* ===========================================================================
 * APLLFBD1: APLL FEEDBACK DIVIDER REGISTER
 * ===========================================================================*/

#if defined (__P33SMPS_CH__) ||  defined (__P33SMPS_CK__)

#define REG_APLLFBD_VALID_DATA_WRITE_MASK    0x00FF
#define REG_APLLFBD_VALID_DATA_READ_MASK     0x00FF

#define REG_APLLFBD_APLLFBDIV_M_MASK           0b0000000011111111
#define REG_APLLFBD_MULTIPLIER_M(x)     {(x & REG_APLLFBD_APLLFBDIV_M_MASK)}

typedef enum {
    APLLFBD_APLLFBDIV_M_16 = 0b00010000, // APLL Input Clock Multiplier Setting x16
    APLLFBD_APLLFBDIV_M_17 = 0b00010001, // APLL Input Clock Multiplier Setting x17
    APLLFBD_APLLFBDIV_M_18 = 0b00010010, // APLL Input Clock Multiplier Setting x18
    APLLFBD_APLLFBDIV_M_19 = 0b00010011, // APLL Input Clock Multiplier Setting x19
    APLLFBD_APLLFBDIV_M_20 = 0b00010100, // APLL Input Clock Multiplier Setting x20
    APLLFBD_APLLFBDIV_M_21 = 0b00010101, // APLL Input Clock Multiplier Setting x21
    APLLFBD_APLLFBDIV_M_22 = 0b00010110, // APLL Input Clock Multiplier Setting x22
    APLLFBD_APLLFBDIV_M_23 = 0b00010111, // APLL Input Clock Multiplier Setting x23
    APLLFBD_APLLFBDIV_M_24 = 0b00011000, // APLL Input Clock Multiplier Setting x24
    APLLFBD_APLLFBDIV_M_25 = 0b00011001, // APLL Input Clock Multiplier Setting x25
    APLLFBD_APLLFBDIV_M_26 = 0b00011010, // APLL Input Clock Multiplier Setting x26
    APLLFBD_APLLFBDIV_M_27 = 0b00011011, // APLL Input Clock Multiplier Setting x27
    APLLFBD_APLLFBDIV_M_28 = 0b00011100, // APLL Input Clock Multiplier Setting x28
    APLLFBD_APLLFBDIV_M_29 = 0b00011101, // APLL Input Clock Multiplier Setting x29
    APLLFBD_APLLFBDIV_M_30 = 0b00011110, // APLL Input Clock Multiplier Setting x30
    APLLFBD_APLLFBDIV_M_31 = 0b00011111, // APLL Input Clock Multiplier Setting x31
    APLLFBD_APLLFBDIV_M_32 = 0b00100000, // APLL Input Clock Multiplier Setting x32
    APLLFBD_APLLFBDIV_M_33 = 0b00100001, // APLL Input Clock Multiplier Setting x33
    APLLFBD_APLLFBDIV_M_34 = 0b00100010, // APLL Input Clock Multiplier Setting x34
    APLLFBD_APLLFBDIV_M_35 = 0b00100011, // APLL Input Clock Multiplier Setting x35
    APLLFBD_APLLFBDIV_M_36 = 0b00100100, // APLL Input Clock Multiplier Setting x36
    APLLFBD_APLLFBDIV_M_37 = 0b00100101, // APLL Input Clock Multiplier Setting x37
    APLLFBD_APLLFBDIV_M_38 = 0b00100110, // APLL Input Clock Multiplier Setting x38
    APLLFBD_APLLFBDIV_M_39 = 0b00100111, // APLL Input Clock Multiplier Setting x39
    APLLFBD_APLLFBDIV_M_40 = 0b00101000, // APLL Input Clock Multiplier Setting x40
    APLLFBD_APLLFBDIV_M_41 = 0b00101001, // APLL Input Clock Multiplier Setting x41
    APLLFBD_APLLFBDIV_M_42 = 0b00101010, // APLL Input Clock Multiplier Setting x42
    APLLFBD_APLLFBDIV_M_43 = 0b00101011, // APLL Input Clock Multiplier Setting x43
    APLLFBD_APLLFBDIV_M_44 = 0b00101100, // APLL Input Clock Multiplier Setting x44
    APLLFBD_APLLFBDIV_M_45 = 0b00101101, // APLL Input Clock Multiplier Setting x45
    APLLFBD_APLLFBDIV_M_46 = 0b00101110, // APLL Input Clock Multiplier Setting x46
    APLLFBD_APLLFBDIV_M_47 = 0b00101111, // APLL Input Clock Multiplier Setting x47
    APLLFBD_APLLFBDIV_M_48 = 0b00110000, // APLL Input Clock Multiplier Setting x48
    APLLFBD_APLLFBDIV_M_49 = 0b00110001, // APLL Input Clock Multiplier Setting x49
    APLLFBD_APLLFBDIV_M_50 = 0b00110010, // APLL Input Clock Multiplier Setting x50
    APLLFBD_APLLFBDIV_M_51 = 0b00110011, // APLL Input Clock Multiplier Setting x51
    APLLFBD_APLLFBDIV_M_52 = 0b00110100, // APLL Input Clock Multiplier Setting x52
    APLLFBD_APLLFBDIV_M_53 = 0b00110101, // APLL Input Clock Multiplier Setting x53
    APLLFBD_APLLFBDIV_M_54 = 0b00110110, // APLL Input Clock Multiplier Setting x54
    APLLFBD_APLLFBDIV_M_55 = 0b00110111, // APLL Input Clock Multiplier Setting x55
    APLLFBD_APLLFBDIV_M_56 = 0b00111000, // APLL Input Clock Multiplier Setting x56
    APLLFBD_APLLFBDIV_M_57 = 0b00111001, // APLL Input Clock Multiplier Setting x57
    APLLFBD_APLLFBDIV_M_58 = 0b00111010, // APLL Input Clock Multiplier Setting x58
    APLLFBD_APLLFBDIV_M_59 = 0b00111011, // APLL Input Clock Multiplier Setting x59
    APLLFBD_APLLFBDIV_M_60 = 0b00111100, // APLL Input Clock Multiplier Setting x60
    APLLFBD_APLLFBDIV_M_61 = 0b00111101, // APLL Input Clock Multiplier Setting x61
    APLLFBD_APLLFBDIV_M_62 = 0b00111110, // APLL Input Clock Multiplier Setting x62
    APLLFBD_APLLFBDIV_M_63 = 0b00111111, // APLL Input Clock Multiplier Setting x63
    APLLFBD_APLLFBDIV_M_64 = 0b01000000, // APLL Input Clock Multiplier Setting x64
    APLLFBD_APLLFBDIV_M_65 = 0b01000001, // APLL Input Clock Multiplier Setting x65
    APLLFBD_APLLFBDIV_M_66 = 0b01000010, // APLL Input Clock Multiplier Setting x66
    APLLFBD_APLLFBDIV_M_67 = 0b01000011, // APLL Input Clock Multiplier Setting x67
    APLLFBD_APLLFBDIV_M_68 = 0b01000100, // APLL Input Clock Multiplier Setting x68
    APLLFBD_APLLFBDIV_M_69 = 0b01000101, // APLL Input Clock Multiplier Setting x69
    APLLFBD_APLLFBDIV_M_70 = 0b01000110, // APLL Input Clock Multiplier Setting x70
    APLLFBD_APLLFBDIV_M_71 = 0b01000111, // APLL Input Clock Multiplier Setting x71
    APLLFBD_APLLFBDIV_M_72 = 0b01001000, // APLL Input Clock Multiplier Setting x72
    APLLFBD_APLLFBDIV_M_73 = 0b01001001, // APLL Input Clock Multiplier Setting x73
    APLLFBD_APLLFBDIV_M_74 = 0b01001010, // APLL Input Clock Multiplier Setting x74
    APLLFBD_APLLFBDIV_M_75 = 0b01001011, // APLL Input Clock Multiplier Setting x75
    APLLFBD_APLLFBDIV_M_76 = 0b01001100, // APLL Input Clock Multiplier Setting x76
    APLLFBD_APLLFBDIV_M_77 = 0b01001101, // APLL Input Clock Multiplier Setting x77
    APLLFBD_APLLFBDIV_M_78 = 0b01001110, // APLL Input Clock Multiplier Setting x78
    APLLFBD_APLLFBDIV_M_79 = 0b01001111, // APLL Input Clock Multiplier Setting x79
    APLLFBD_APLLFBDIV_M_80 = 0b01010000, // APLL Input Clock Multiplier Setting x80
    APLLFBD_APLLFBDIV_M_81 = 0b01010001, // APLL Input Clock Multiplier Setting x81
    APLLFBD_APLLFBDIV_M_82 = 0b01010010, // APLL Input Clock Multiplier Setting x82
    APLLFBD_APLLFBDIV_M_83 = 0b01010011, // APLL Input Clock Multiplier Setting x83
    APLLFBD_APLLFBDIV_M_84 = 0b01010100, // APLL Input Clock Multiplier Setting x84
    APLLFBD_APLLFBDIV_M_85 = 0b01010101, // APLL Input Clock Multiplier Setting x85
    APLLFBD_APLLFBDIV_M_86 = 0b01010110, // APLL Input Clock Multiplier Setting x86
    APLLFBD_APLLFBDIV_M_87 = 0b01010111, // APLL Input Clock Multiplier Setting x87
    APLLFBD_APLLFBDIV_M_88 = 0b01011000, // APLL Input Clock Multiplier Setting x88
    APLLFBD_APLLFBDIV_M_89 = 0b01011001, // APLL Input Clock Multiplier Setting x89
    APLLFBD_APLLFBDIV_M_90 = 0b01011010, // APLL Input Clock Multiplier Setting x90
    APLLFBD_APLLFBDIV_M_91 = 0b01011011, // APLL Input Clock Multiplier Setting x91
    APLLFBD_APLLFBDIV_M_92 = 0b01011100, // APLL Input Clock Multiplier Setting x92
    APLLFBD_APLLFBDIV_M_93 = 0b01011101, // APLL Input Clock Multiplier Setting x93
    APLLFBD_APLLFBDIV_M_94 = 0b01011110, // APLL Input Clock Multiplier Setting x94
    APLLFBD_APLLFBDIV_M_95 = 0b01011111, // APLL Input Clock Multiplier Setting x95
    APLLFBD_APLLFBDIV_M_96 = 0b01100000, // APLL Input Clock Multiplier Setting x96
    APLLFBD_APLLFBDIV_M_97 = 0b01100001, // APLL Input Clock Multiplier Setting x97
    APLLFBD_APLLFBDIV_M_98 = 0b01100010, // APLL Input Clock Multiplier Setting x98
    APLLFBD_APLLFBDIV_M_99 = 0b01100011, // APLL Input Clock Multiplier Setting x99
    APLLFBD_APLLFBDIV_M_100 = 0b01100100, // APLL Input Clock Multiplier Setting x100
    APLLFBD_APLLFBDIV_M_101 = 0b01100101, // APLL Input Clock Multiplier Setting x101
    APLLFBD_APLLFBDIV_M_102 = 0b01100110, // APLL Input Clock Multiplier Setting x102
    APLLFBD_APLLFBDIV_M_103 = 0b01100111, // APLL Input Clock Multiplier Setting x103
    APLLFBD_APLLFBDIV_M_104 = 0b01101000, // APLL Input Clock Multiplier Setting x104
    APLLFBD_APLLFBDIV_M_105 = 0b01101001, // APLL Input Clock Multiplier Setting x105
    APLLFBD_APLLFBDIV_M_106 = 0b01101010, // APLL Input Clock Multiplier Setting x106
    APLLFBD_APLLFBDIV_M_107 = 0b01101011, // APLL Input Clock Multiplier Setting x107
    APLLFBD_APLLFBDIV_M_108 = 0b01101100, // APLL Input Clock Multiplier Setting x108
    APLLFBD_APLLFBDIV_M_109 = 0b01101101, // APLL Input Clock Multiplier Setting x109
    APLLFBD_APLLFBDIV_M_110 = 0b01101110, // APLL Input Clock Multiplier Setting x110
    APLLFBD_APLLFBDIV_M_111 = 0b01101111, // APLL Input Clock Multiplier Setting x111
    APLLFBD_APLLFBDIV_M_112 = 0b01110000, // APLL Input Clock Multiplier Setting x112
    APLLFBD_APLLFBDIV_M_113 = 0b01110001, // APLL Input Clock Multiplier Setting x113
    APLLFBD_APLLFBDIV_M_114 = 0b01110010, // APLL Input Clock Multiplier Setting x114
    APLLFBD_APLLFBDIV_M_115 = 0b01110011, // APLL Input Clock Multiplier Setting x115
    APLLFBD_APLLFBDIV_M_116 = 0b01110100, // APLL Input Clock Multiplier Setting x116
    APLLFBD_APLLFBDIV_M_117 = 0b01110101, // APLL Input Clock Multiplier Setting x117
    APLLFBD_APLLFBDIV_M_118 = 0b01110110, // APLL Input Clock Multiplier Setting x118
    APLLFBD_APLLFBDIV_M_119 = 0b01110111, // APLL Input Clock Multiplier Setting x119
    APLLFBD_APLLFBDIV_M_120 = 0b01111000, // APLL Input Clock Multiplier Setting x120
    APLLFBD_APLLFBDIV_M_121 = 0b01111001, // APLL Input Clock Multiplier Setting x121
    APLLFBD_APLLFBDIV_M_122 = 0b01111010, // APLL Input Clock Multiplier Setting x122
    APLLFBD_APLLFBDIV_M_123 = 0b01111011, // APLL Input Clock Multiplier Setting x123
    APLLFBD_APLLFBDIV_M_124 = 0b01111100, // APLL Input Clock Multiplier Setting x124
    APLLFBD_APLLFBDIV_M_125 = 0b01111101, // APLL Input Clock Multiplier Setting x125
    APLLFBD_APLLFBDIV_M_126 = 0b01111110, // APLL Input Clock Multiplier Setting x126
    APLLFBD_APLLFBDIV_M_127 = 0b01111111, // APLL Input Clock Multiplier Setting x127
    APLLFBD_APLLFBDIV_M_128 = 0b10000000, // APLL Input Clock Multiplier Setting x128
    APLLFBD_APLLFBDIV_M_129 = 0b10000001, // APLL Input Clock Multiplier Setting x129
    APLLFBD_APLLFBDIV_M_130 = 0b10000010, // APLL Input Clock Multiplier Setting x130
    APLLFBD_APLLFBDIV_M_131 = 0b10000011, // APLL Input Clock Multiplier Setting x131
    APLLFBD_APLLFBDIV_M_132 = 0b10000100, // APLL Input Clock Multiplier Setting x132
    APLLFBD_APLLFBDIV_M_133 = 0b10000101, // APLL Input Clock Multiplier Setting x133
    APLLFBD_APLLFBDIV_M_134 = 0b10000110, // APLL Input Clock Multiplier Setting x134
    APLLFBD_APLLFBDIV_M_135 = 0b10000111, // APLL Input Clock Multiplier Setting x135
    APLLFBD_APLLFBDIV_M_136 = 0b10001000, // APLL Input Clock Multiplier Setting x136
    APLLFBD_APLLFBDIV_M_137 = 0b10001001, // APLL Input Clock Multiplier Setting x137
    APLLFBD_APLLFBDIV_M_138 = 0b10001010, // APLL Input Clock Multiplier Setting x138
    APLLFBD_APLLFBDIV_M_139 = 0b10001011, // APLL Input Clock Multiplier Setting x139
    APLLFBD_APLLFBDIV_M_140 = 0b10001100, // APLL Input Clock Multiplier Setting x140
    APLLFBD_APLLFBDIV_M_141 = 0b10001101, // APLL Input Clock Multiplier Setting x141
    APLLFBD_APLLFBDIV_M_142 = 0b10001110, // APLL Input Clock Multiplier Setting x142
    APLLFBD_APLLFBDIV_M_143 = 0b10001111, // APLL Input Clock Multiplier Setting x143
    APLLFBD_APLLFBDIV_M_144 = 0b10010000, // APLL Input Clock Multiplier Setting x144
    APLLFBD_APLLFBDIV_M_145 = 0b10010001, // APLL Input Clock Multiplier Setting x145
    APLLFBD_APLLFBDIV_M_146 = 0b10010010, // APLL Input Clock Multiplier Setting x146
    APLLFBD_APLLFBDIV_M_147 = 0b10010011, // APLL Input Clock Multiplier Setting x147
    APLLFBD_APLLFBDIV_M_148 = 0b10010100, // APLL Input Clock Multiplier Setting x148
    APLLFBD_APLLFBDIV_M_149 = 0b10010101, // APLL Input Clock Multiplier Setting x149
    APLLFBD_APLLFBDIV_M_150 = 0b10010110, // APLL Input Clock Multiplier Setting x150
    APLLFBD_APLLFBDIV_M_151 = 0b10010111, // APLL Input Clock Multiplier Setting x151
    APLLFBD_APLLFBDIV_M_152 = 0b10011000, // APLL Input Clock Multiplier Setting x152
    APLLFBD_APLLFBDIV_M_153 = 0b10011001, // APLL Input Clock Multiplier Setting x153
    APLLFBD_APLLFBDIV_M_154 = 0b10011010, // APLL Input Clock Multiplier Setting x154
    APLLFBD_APLLFBDIV_M_155 = 0b10011011, // APLL Input Clock Multiplier Setting x155
    APLLFBD_APLLFBDIV_M_156 = 0b10011100, // APLL Input Clock Multiplier Setting x156
    APLLFBD_APLLFBDIV_M_157 = 0b10011101, // APLL Input Clock Multiplier Setting x157
    APLLFBD_APLLFBDIV_M_158 = 0b10011110, // APLL Input Clock Multiplier Setting x158
    APLLFBD_APLLFBDIV_M_159 = 0b10011111, // APLL Input Clock Multiplier Setting x159
    APLLFBD_APLLFBDIV_M_160 = 0b10100000, // APLL Input Clock Multiplier Setting x160
    APLLFBD_APLLFBDIV_M_161 = 0b10100001, // APLL Input Clock Multiplier Setting x161
    APLLFBD_APLLFBDIV_M_162 = 0b10100010, // APLL Input Clock Multiplier Setting x162
    APLLFBD_APLLFBDIV_M_163 = 0b10100011, // APLL Input Clock Multiplier Setting x163
    APLLFBD_APLLFBDIV_M_164 = 0b10100100, // APLL Input Clock Multiplier Setting x164
    APLLFBD_APLLFBDIV_M_165 = 0b10100101, // APLL Input Clock Multiplier Setting x165
    APLLFBD_APLLFBDIV_M_166 = 0b10100110, // APLL Input Clock Multiplier Setting x166
    APLLFBD_APLLFBDIV_M_167 = 0b10100111, // APLL Input Clock Multiplier Setting x167
    APLLFBD_APLLFBDIV_M_168 = 0b10101000, // APLL Input Clock Multiplier Setting x168
    APLLFBD_APLLFBDIV_M_169 = 0b10101001, // APLL Input Clock Multiplier Setting x169
    APLLFBD_APLLFBDIV_M_170 = 0b10101010, // APLL Input Clock Multiplier Setting x170
    APLLFBD_APLLFBDIV_M_171 = 0b10101011, // APLL Input Clock Multiplier Setting x171
    APLLFBD_APLLFBDIV_M_172 = 0b10101100, // APLL Input Clock Multiplier Setting x172
    APLLFBD_APLLFBDIV_M_173 = 0b10101101, // APLL Input Clock Multiplier Setting x173
    APLLFBD_APLLFBDIV_M_174 = 0b10101110, // APLL Input Clock Multiplier Setting x174
    APLLFBD_APLLFBDIV_M_175 = 0b10101111, // APLL Input Clock Multiplier Setting x175
    APLLFBD_APLLFBDIV_M_176 = 0b10110000, // APLL Input Clock Multiplier Setting x176
    APLLFBD_APLLFBDIV_M_177 = 0b10110001, // APLL Input Clock Multiplier Setting x177
    APLLFBD_APLLFBDIV_M_178 = 0b10110010, // APLL Input Clock Multiplier Setting x178
    APLLFBD_APLLFBDIV_M_179 = 0b10110011, // APLL Input Clock Multiplier Setting x179
    APLLFBD_APLLFBDIV_M_180 = 0b10110100, // APLL Input Clock Multiplier Setting x180
    APLLFBD_APLLFBDIV_M_181 = 0b10110101, // APLL Input Clock Multiplier Setting x181
    APLLFBD_APLLFBDIV_M_182 = 0b10110110, // APLL Input Clock Multiplier Setting x182
    APLLFBD_APLLFBDIV_M_183 = 0b10110111, // APLL Input Clock Multiplier Setting x183
    APLLFBD_APLLFBDIV_M_184 = 0b10111000, // APLL Input Clock Multiplier Setting x184
    APLLFBD_APLLFBDIV_M_185 = 0b10111001, // APLL Input Clock Multiplier Setting x185
    APLLFBD_APLLFBDIV_M_186 = 0b10111010, // APLL Input Clock Multiplier Setting x186
    APLLFBD_APLLFBDIV_M_187 = 0b10111011, // APLL Input Clock Multiplier Setting x187
    APLLFBD_APLLFBDIV_M_188 = 0b10111100, // APLL Input Clock Multiplier Setting x188
    APLLFBD_APLLFBDIV_M_189 = 0b10111101, // APLL Input Clock Multiplier Setting x189
    APLLFBD_APLLFBDIV_M_190 = 0b10111110, // APLL Input Clock Multiplier Setting x190
    APLLFBD_APLLFBDIV_M_191 = 0b10111111, // APLL Input Clock Multiplier Setting x191
    APLLFBD_APLLFBDIV_M_192 = 0b11000000, // APLL Input Clock Multiplier Setting x192
    APLLFBD_APLLFBDIV_M_193 = 0b11000001, // APLL Input Clock Multiplier Setting x193
    APLLFBD_APLLFBDIV_M_194 = 0b11000010, // APLL Input Clock Multiplier Setting x194
    APLLFBD_APLLFBDIV_M_195 = 0b11000011, // APLL Input Clock Multiplier Setting x195
    APLLFBD_APLLFBDIV_M_196 = 0b11000100, // APLL Input Clock Multiplier Setting x196
    APLLFBD_APLLFBDIV_M_197 = 0b11000101, // APLL Input Clock Multiplier Setting x197
    APLLFBD_APLLFBDIV_M_198 = 0b11000110, // APLL Input Clock Multiplier Setting x198
    APLLFBD_APLLFBDIV_M_199 = 0b11000111, // APLL Input Clock Multiplier Setting x199
    APLLFBD_APLLFBDIV_M_200 = 0b11001000 // APLL Input Clock Multiplier Setting x200
} APLLFBD_APLLFBDIV_e; // APLL Feedback Divider bits (also denoted as ?M?, PLL multiplier)

typedef struct {
    volatile APLLFBD_APLLFBDIV_e APLLFBDIV : 8; // APLL Feedback Divider bits (also denoted as ?M?, PLL multiplier)
    volatile unsigned : 8; // reserved
} APLLFBD_t; // APLL Feedback Divider bits

typedef union {
    volatile uint16_t value;
    volatile APLLFBD_t APLLFBD; // APLL Feedback Divider bits
} REGBLK_APLLFBD_CONFIG_t;

#else
    #pragma message "error: === selected device family is not supported by oscillator mcal library ==="
#endif

/* ===========================================================================
 * APLLDIV1: APLL OUTPUT DIVIDER REGISTER
 * ===========================================================================*/

#if defined (__P33SMPS_CH__) ||  defined (__P33SMPS_CK__)

#define REG_APLLDIV_VALID_DATA_WRITE_MASK    0x0377
#define REG_APLLDIV_VALID_DATA_READ_MASK     0x0377

#define REG_APLLDIV_POST1DIV_N2_MASK         0b0000000001110000
#define REG_APLLDIV_POST1DIV_N2(x)     {((x << 4) & REG_APLLDIV_POST1DIV_N2_MASK)}

#define REG_APLLDIV_POST2DIV_N3_MASK         0b0000000000000111
#define REG_APLLDIV_POST2DIV_N3(x)     {(x & REG_APLLDIV_POST1DIV_N3_MASK)}

typedef enum {
    APLLDIV_POST2DIV_N2N3_1 = 0b001, // PLL Output Divider Ratio bits 1:1
    APLLDIV_POST2DIV_N2N3_2 = 0b010, // PLL Output Divider Ratio bits 1:2
    APLLDIV_POST2DIV_N2N3_3 = 0b011, // PLL Output Divider Ratio bits 1:3
    APLLDIV_POST2DIV_N2N3_4 = 0b100, // PLL Output Divider Ratio bits 1:4
    APLLDIV_POST2DIV_N2N3_5 = 0b101, // PLL Output Divider Ratio bits 1:5
    APLLDIV_POST2DIV_N2N3_6 = 0b110, // PLL Output Divider Ratio bits 1:6
    APLLDIV_POST2DIV_N2N3_7 = 0b111 // PLL Output Divider Ratio bits 1:7
} APLLDIV_POSTxDIV_e; // PLL Output Divider Ratio bits (also denoted as ?N2? and ?N3?, PLL divider)

#define REG_APLLDIV_AVCODIV_FVCO_DIV_BY_1	0b0000001100000000
#define REG_APLLDIV_AVCODIV_FVCO_DIV_BY_2	0b0000001000000000
#define REG_APLLDIV_AVCODIV_FVCO_DIV_BY_3	0b0000000100000000
#define REG_APLLDIV_AVCODIV_FVCO_DIV_BY_4	0b0000000000000000

typedef enum {
    APLLDIV_AVCODIV_FVCO_DIV_BY_1 = 0b11, // APLL AVCO Output Divider 1:1
    APLLDIV_AVCODIV_FVCO_DIV_BY_2 = 0b10, // APLL AVCO Output Divider 1:2
    APLLDIV_AVCODIV_FVCO_DIV_BY_3 = 0b01, // APLL AVCO Output Divider 1:3
    APLLDIV_AVCODIV_FVCO_DIV_BY_4 = 0b00 // APLL AVCO Output Divider 1:4
} APLLDIV_AVCODIV_e; // APLL AVCO Output Divider Select bits

typedef struct {
    volatile APLLDIV_POSTxDIV_e APOST2DIV : 3;
    volatile unsigned : 1;
    volatile APLLDIV_POSTxDIV_e APOST1DIV : 3;
    volatile unsigned : 1;
    volatile APLLDIV_AVCODIV_e AVCODIV : 2;
    volatile unsigned : 6;
} __attribute__((packed)) APLLDIV_t;

typedef union {
    volatile uint16_t value;
    volatile APLLDIV_t APLLDIV; // APLL Output Divider Select bits
} REGBLK_APLLDIV_CONFIG_t;

#else
    #pragma message "error: === selected device family is not supported by oscillator mcal library ==="
#endif

/* ===========================================================================
 * OSCILLATOR CONFIGURATION OBJECT 
 * ===========================================================================*/

#if defined (__P33SMPS_CH__) ||  defined (__P33SMPS_CK__)

typedef struct OSC_CONFIG_s {
    volatile OSCCON_xOSC_TYPE_e osc_type;
    volatile enum CLKDIV_FRCDIVN_e frc_div;
    volatile enum OSCTUN_TUN_e frc_tune;
    volatile CLKDIV_PLLPRE_e N1;
    volatile PLLFBD_PLLFBDIV_e M;
    volatile PLLDIV_POSTxDIV_e N2;
    volatile PLLDIV_POSTxDIV_e N3;
    volatile PLLDIV_VCODIV_e VCODIV;
} OSC_CONFIG_t;

typedef struct AUXOSC_CONFIG_s {
    volatile ACLKCON_APLLPRE_e N1;
    volatile APLLFBD_APLLFBDIV_e M;
    volatile APLLDIV_POSTxDIV_e N2;
    volatile APLLDIV_POSTxDIV_e N3;
    volatile APLLDIV_AVCODIV_e AVCODIV;
    volatile ACLKCON_FRCSEL_e FRCSEL : 1;
    volatile ACLKCON_APLLCK_e APLLCK : 1;
    volatile ACLKCON_APLLEN_e APLLEN : 1;
} AUXOSC_CONFIG_t;

#else
    #pragma message "error: === selected device family is not supported by oscillator mcal library ==="
#endif


/* ***************************************************************************************
 *	ERROR CODES
 * **************************************************************************************/

typedef enum OSC_CFG_ERR_RESULT_e{
    OSCERR_FAILURE = 0x0000, // Global Clock Error
    OSCERR_SUCCESS = 0x0001, // Clock initialization was successfully performed
    OSCERR_CSF = 0x0002, // Clock switch-over failed
    OSCERR_RST = 0x0004, // Clock switch-over failed
    OSCERR_CSD = 0x0008, // Clock switching is disabled but desired clock differs from current clock
    OSCERR_PLL_LCK = 0x0010, // Primary PLL does not lock in
    OSCERR_APLL_LCK = 0x0020, // Auxiliary PLL does not lock in
} OSC_CFG_ERR_RESULT_t;


/* ***************************************************************************************
 *	Prototypes
 * **************************************************************************************/

extern volatile uint16_t p33c_Osc_Initialize(volatile struct OSC_CONFIG_s osc_config);
extern volatile uint16_t p33c_OscFrc_Initialize(volatile enum CLKDIV_FRCDIVN_e frc_div, volatile enum OSCTUN_TUN_e frc_tun);
extern volatile uint16_t p33c_OscAuxClk_Initialize(volatile struct AUXOSC_CONFIG_s aux_clock_config);

extern volatile uint16_t p33c_OscFrc_DefaultInitialize(volatile enum CPU_SPEED_DEFAULTS_e cpu_speed);
extern volatile uint16_t p33c_OscAuxClk_DefaultInitialize(volatile enum AUX_PLL_DEFAULTS_e afpllo_frequency);
extern volatile uint16_t p33c_Osc_GetFrequencies(volatile uint32_t main_osc_frequency);

#endif  /* MCAL_P33SMPS_OSCILLATOR_H */

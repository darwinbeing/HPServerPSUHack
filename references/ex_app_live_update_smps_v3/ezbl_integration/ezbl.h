/*******************************************************************************
  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM API Definitions

  File Name:
    ezbl.h

  Summary:
    EZBL and Bootloader definitions for Bootloader and Application API call use.

  Description:
    Bootloader definitions for the EZBL Application callable API implemented as
    macros, inline functions, and true functions implemented in the companion
    ezbl_lib.a (or ezbl_lib32mm.a) archive library.

    This file is intended to be #included into any Application or Bootloader
    files which wish to communicate, making timing decisions, read/write Flash
    memory, access linker symbols, declare Configuration words, perform 16-bit
    atomic memory accesses, do FIFO buffering, control interrupts in a more
    portable manner, allocate holes in Flash, or do many other BIOS-like tasks
    across any Microchip PIC24/dsPIC 16-bit product or PIC32MM family device).
 ******************************************************************************/

/*******************************************************************************
  Copyright (C) 2018 Microchip Technology Inc.

  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
  derivatives created by any person or entity by or on your behalf, exclusively
  with Microchip's products.  Microchip and its licensors retain all ownership
  and intellectual property rights in the accompanying software and in all
  derivatives here to.

  This software and any accompanying information is for suggestion only.  It
  does not modify Microchip's standard warranty for its products.  You agree
  that you are solely responsible for testing the software and determining its
  suitability.  Microchip has no obligation to modify, test, certify, or
  support the software.

  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
  TERMS.
*******************************************************************************/

#ifndef EZBL_H
#define	EZBL_H

// Easy Bootloader Library for PIC24/dsPIC33/PIC32MM release version
#define EZBL_VERSION            2.11        // The complete EZBL version string, but written as a floating point number, including two decimals.
#define EZBL_VERSION_MAJOR      2           // The single EZBL major revision numeral to the left of the dot
#define EZBL_VERSION_MINOR      11          // The numerical EZBL minor revision to the right of the dot. This is 1 for version x.01, 10 for x.10, 26 for x.26, etc.
#define EZBL_VERSION_MINORH     1           // The single EZBL minor revision numeral one place right of the dot (immediate right of the dot)
#define EZBL_VERSION_MINORL     1           // The single EZBL minor revision numeral two places right of the dot



#if !defined(__VA_COUNT)
/**
 * Macro returns a count of the number of variadic macro arguments passed in.
 * Supports 0 to 32 elements.
 */
#define __VA_COUNT(...) __VA_COUNT_(,##__VA_ARGS__,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#define __VA_COUNT_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32, N, ...) N   // Internal helper for __VA_COUNT(): eats real arguments, returning one of the trailing numbers this macro was called with
#endif


#if !defined(STRINGIFY)
#define STRINGIFY_INNER(x)              #x  // Internal macro to make STRINGIFY() actually work. Call STRINGIFY() instead of this one.

/**
 * Converts tokens or preprocessor macros into a literal compile/assemble-time
 * constant string. For preprocessor macros, the macro is evaluated before
 * conversion.
 *
 * This macro is useful for doing string concatenation of preprocessor macro
 * constants since ordinary asm/C language tools treat adjacent literal strings
 * with whitespace between them as a single concatenated string.

 * @param value The token(s) or macro expression to convert to a string literal
 *              constant.
 *
 * @return The 'value' token(s), after any macro expansion, converted to a
 *         string literal. This is essentially equivalent to writing:
 *              "value"
 *         except that anything passed in as value will have processor
 *         evaluation/substitution applied to it.
 */
#define STRINGIFY(value)                STRINGIFY_INNER(value)
#endif


/**
 * Macros to concatenate tokens into one larger token. Each token can be literal
 * typed characters or a processor macro that is evaluated before
 * concatenation.
 *
 * These work functionality similar to the STRINGIFY() macro, except that the
 * output remains the same type as originally passed in (usually a C or ASM
 * identifier/code fragment).
 */
#if !defined(CAT2)
#define CAT2_IN(a0,a1)                      a0##a1                          // Not recommended to use this macro directly; macro expansion won't occur unless another macro wraps it.
#define CAT3_IN(a0,a1,a2)                   a0##a1##a2                      // Not recommended to use this macro directly; macro expansion won't occur unless another macro wraps it.
#define CAT4_IN(a0,a1,a2,a3)                a0##a1##a2##a3                  // Not recommended to use this macro directly; macro expansion won't occur unless another macro wraps it.
#define CAT5_IN(a0,a1,a2,a3,a4)             a0##a1##a2##a3##a4              // Not recommended to use this macro directly; macro expansion won't occur unless another macro wraps it.
#define CAT6_IN(a0,a1,a2,a3,a4,a5)          a0##a1##a2##a3##a4##a5          // Not recommended to use this macro directly; macro expansion won't occur unless another macro wraps it.
#define CAT2(a0,a1)                         CAT2_IN(a0,a1)                      // Use this; allows macro expansion
#define CAT3(a0,a1,a2)                      CAT3_IN(a0,a1,a2)                   // Use this; allows macro expansion
#define CAT4(a0,a1,a2,a3)                   CAT4_IN(a0,a1,a2,a3)                // Use this; allows macro expansion
#define CAT5(a0,a1,a2,a3,a4)                CAT5_IN(a0,a1,a2,a3,a4)             // Use this; allows macro expansion
#define CAT6(a0,a1,a2,a3,a4,a5)             CAT6_IN(a0,a1,a2,a3,a4,a5)          // Use this; allows macro expansion


#define __CAT0(...)                                         // Nullifies any/all parameters
#define __CAT1(arg1,...)       arg1                         // Discards all but the first parameter
#define __CAT2(arg1,...)       __CAT1(arg1##__VA_ARGS__)
#define __CAT3(arg1,...)       __CAT2(arg1##__VA_ARGS__)
#define __CAT4(arg1,...)       __CAT3(arg1##__VA_ARGS__)
#define __CAT5(arg1,...)       __CAT4(arg1##__VA_ARGS__)
#define __CAT6(arg1,...)       __CAT5(arg1##__VA_ARGS__)
#define __CAT7(arg1,...)       __CAT6(arg1##__VA_ARGS__)
#define __CAT8(arg1,...)       __CAT7(arg1##__VA_ARGS__)
#define __CAT9(arg1,...)       __CAT8(arg1##__VA_ARGS__)
#define __CAT10(arg1,...)      __CAT9(arg1##__VA_ARGS__)
#define __CAT11(arg1,...)      __CAT10(arg1##__VA_ARGS__)
#define __CAT12(arg1,...)      __CAT11(arg1##__VA_ARGS__)
#define __CAT13(arg1,...)      __CAT12(arg1##__VA_ARGS__)
#define __CAT14(arg1,...)      __CAT13(arg1##__VA_ARGS__)
#define __CAT15(arg1,...)      __CAT14(arg1##__VA_ARGS__)
#define __CAT16(arg1,...)      __CAT15(arg1##__VA_ARGS__)
#define CAT(...)               CAT2(__CAT,__VA_COUNT(__VA_ARGS__))(__VA_ARGS__)
#endif


#if defined(__XC16__)
#define XC16_NEAR                       __attribute__((near))
#define EZBL_PAGE_ATTR                  ", page"
#define EZBL_PWORD                      ".pword  "
#define STRINGIFY_SYM(symbolToken)      STRINGIFY(CAT(_,symbolToken))   // Put a leading underscore on the symbol then convert to string (used for XC16 symbols/identifiers in C)
#if defined(ANSA) && !defined(ANSELA)
#define ANSELA  ANSA
#endif
#if defined(ANSB) && !defined(ANSELB)
#define ANSELB  ANSB
#endif
#if defined(ANSC) && !defined(ANSELC)
#define ANSELC  ANSC
#endif
#if defined(ANSD) && !defined(ANSELD)
#define ANSELD  ANSD
#endif
#if defined(ANSE) && !defined(ANSELE)
#define ANSELE  ANSE
#endif
#if defined(ANSF) && !defined(ANSELF)
#define ANSELF  ANSF
#endif
#if defined(ANSG) && !defined(ANSELG)
#define ANSELG  ANSG
#endif
#if defined(ANSH) && !defined(ANSELH)
#define ANSELH  ANSH
#endif
#if defined(ANSJ) && !defined(ANSELJ)
#define ANSELJ  ANSJ
#endif
#elif defined(__XC32__) || defined(__PIC32__)
#define __eds__
#define __psv__                         const
#define __prog__                        const
#define __pack_upper_byte               const
#define XC16_NEAR
#define EZBL_PAGE_ATTR
#define EZBL_PWORD                      ".word   "
#define STRINGIFY_SYM(symbolToken)      STRINGIFY(symbolToken)              // Just convert to string without leading underscore on XC32
#if !defined(Idle)
void Idle(void);
#endif
#if !defined(WDTCON)
extern volatile unsigned int __attribute__((section("sfrs"))) WDTCON;
#endif
static inline void __attribute__((always_inline, optimize(1))) ClrWdt(void) // Clears the watchdog timer by writing 0x5743 to the WDTCLRKEY bitfield (16-bit write to upper half-word of WDTCON)
{
    ((volatile unsigned short *)(&WDTCON))[3] = 0x5743;
}
#endif


#if !defined(__builtin_software_breakpoint)
#if defined(__XC16__) && (__XC16_VERSION__ <= 1110)
#define __builtin_software_breakpoint()                 __asm__ __volatile__(".pword 0xDA4000")         // __builtin_software_breakpoint() XC16/XC32 function: returns (void), but generates a 'break' instruction in flash as executable code. A 'break' instruction, when executed, will halt CPU execution in a debug build or (on 16-bit devices) trigger an illegal opcode reset in a production build.
#else
#define __builtin_software_breakpoint()                 __builtin_software_breakpoint()                 // __builtin_software_breakpoint() XC16/XC32 function: returns (void), but generates a 'break' instruction in flash as executable code. A 'break' instruction, when executed, will halt CPU execution in a debug build or (on 16-bit devices) trigger an illegal opcode reset in a production build.
#endif
#endif


// Items applicable to both XC16 and XC32
#if !defined(__builtin_constant_p)          
#define __builtin_constant_p(test_item)                         __builtin_constant_p(test_item)                 // __builtin_constant_p() GCC function: returns true ((int)1) if the specified object or expression resolves to a compile-time constant. NOTE: expressions being tested are evaluated, even though they will not result in any code generation, so this will trigger warnings or errors if the expression would do so in other usage contexts. For example, a mathematical expression containing integer division with a compile-time constant value of 0 will trigger a divide by zero warning. Returns false ((int)0) if the parameter or expression cannot be proved by the compiler to be a compile-time constant. Evaluation is made within the same compilation unit, so expressions including relocatable linker symbols will will yield a true return value, even if they will be constant after linking.
#endif
#if !defined(__builtin_types_compatible_p)
#define __builtin_types_compatible_p(parameter1, parameter2)    __builtin_types_compatible_p(parameter1, parameter2)    // __builtin_types_compatible_p() GCC function: returns true ((int)1) if the specified objects have the "same" base data type. 
#endif
#if !defined(__builtin_alloca)
#define __builtin_alloca(size)                                  __builtin_alloca(size)                          // __builtin_alloca() GCC function: allocates the specified number of bytes on the stack as a temporary buffer and returns a pointer to the allocated memory. The memory is automatically deallocated and removed from the stack when the calling function returns. The returned buffer is not initialized or zeroed. For a non-null return value, the pointer will be aligned to the stack's native alignment (i.e. 0x2 byte aligned on PIC24/dsPIC, 0x4 on PIC32).
#endif
#if !defined(__builtin_section_begin)
#define __builtin_section_begin(section_name_string)            __builtin_section_begin(section_name_string)    // __builtin_section_begin() XC16/XC32 function: returns (unsigned long) start address for the specified section. The section name must be represented as a string literal and be present to link successfully. The section can be located in flash or RAM (or even virtual debug info space).
#endif
#if !defined(__builtin_section_end)
#define __builtin_section_end(section_name_string)              __builtin_section_end(section_name_string)      // __builtin_section_end() XC16/XC32 function: returns (unsigned long) end address for the specified section. The section name must be represented as a string literal and be present to link successfully. The section can be located in flash or RAM (or even virtual debug info space).
#endif
#if !defined(__builtin_section_size)
#define __builtin_section_size(section_name_string)             __builtin_section_size(section_name_string)     // __builtin_section_size() XC16/XC32 function: returns (unsigned long) size for the specified section (i.e. __builtin_section_end() - __builtin_section_start(), but without requiring run-time arithmetic to compute). The section name must be represented as a string literal and be present to link successfully. The section can be located in flash or RAM (or even virtual debug info space). For flash sections, the size is in program addresses, so for 16-bit devices, divide by 2 and multiply by 3 to get a byte count.
#endif
#if !defined(__builtin_enable_interrupts)
#define __builtin_enable_interrupts()                           __builtin_enable_interrupts()                   // __builtin_enable_interrupts() XC16/XC32 function. Sets a global interrupt enable flag. On XC16, when present, sets the INTCON2bits.GIE = 1. Alternatively sets the CPU IPL to IPL7. On XC32, will generate a 'ei' opcode.
#endif
#if !defined(__builtin_disable_interrupts)
#define __builtin_disable_interrupts()                          __builtin_disable_interrupts()                  // __builtin_disable_interrupts() XC16/XC32 function. Clears a global interrupt enable flag. On XC16, when present, clears the INTCON2bits.GIE bit. Alternatively changes the CPU IPL. On XC32, will generate a 'di' opcode.
#endif
//#if !defined(__builtin_next_arg)
//#define __builtin_next_arg(format)                              __builtin_next_arg(format)                      // __builtin_next_arg() XC16/XC32 function. In variadic functions, returned the argument located after the specified formatting string or other argument.
//#endif
//#if !defined(__builtin_strncmp)
//#define __builtin_strncmp(str1, str2)                          __builtin_strncmp(str1, str2)                    // __builtin_strncmp() XC16/XC32 function.
//#endif
//#if !defined(__builtin_strnlen)
//#define __builtin_strnlen(str)                                 __builtin_strnlen(str)                           // __builtin_strnlen() XC16/XC32 function.
//#endif


#if defined(__XC16__) && !defined(__builtin_write_NVM)
#define __builtin_write_NVM()                           __builtin_write_NVM()                           // __builtin_write_NVM() XC16 function: Generates the back-to-back "mov #0x55, Wn; mov Wn, _NVMKEY; mov #0xAA, Wn; mov Wn, _NVMKEY; bset _NVMCON, #15; nop; nop" ASM instructions for starting an NVM erase/write operation. Interrupts should be disabled prior to calling this function to ensure the instructions execute back-to-back.
#endif
#if defined(__XC16__) && !defined(__builtin_write_NVM)
#define __builtin_write_NVM_secure(key1, key2)          __builtin_write_NVM_secure(key1, key2)          // __builtin_write_NVM_secure() XC16 function: Generates the back-to-back "mov key1, Wn; mov Wn, _NVMKEY; mov key2, Wn; mov Wn, _NVMKEY; bset _NVMCON, #15; nop; nop" ASM instructions for starting an NVM erase/write operation. Interrupts should be disabled prior to calling this function to ensure the instructions execute back-to-back.
#endif
#if defined(__XC16__) && !defined(__builtin_nop)
#define __builtin_nop()                                 __builtin_nop()                                 // __builtin_nop() XC16 function: returns (void), but generates a 'nop' (no-operation) instruction that consumes one instruction word of flash and requires 1 instruction cycle to execute.
#endif
#if defined(__XC16__) && !defined(__builtin_mulss)
#define __builtin_mulss(s16_operand_0, s16_operand_1)   __builtin_mulss(s16_operand_0, s16_operand_1)   // __builtin_mulss() XC16 function: returns (signed long) = (signed int) * (signed int)
#endif
#if defined(__XC16__) && !defined(__builtin_mulus)
#define __builtin_mulus(u16_operand_0, s16_operand_1)   __builtin_mulus(u16_operand_0, s16_operand_1)   // __builtin_mulus() XC16 function: returns (signed long) = (unsigned int) * (signed int)
#endif
#if defined(__XC16__) && !defined(__builtin_mulsu)
#define __builtin_mulsu(s16_operand_0, u16_operand_1)   __builtin_mulsu(s16_operand_0, u16_operand_1)   // __builtin_mulsu() XC16 function: returns (signed long) = (signed int) * (unsigned int)
#endif
#if defined(__XC16__) && !defined(__builtin_muluu)
#define __builtin_muluu(u16_operand_0, u16_operand_1)   __builtin_muluu(u16_operand_0, u16_operand_1)   // __builtin_mulus() XC16 function: returns (unsigned long) = (unsigned int) * (unsigned int)
#endif

#if defined(__XC32__) && !defined(__builtin_nop)
#define __builtin_nop()                                 __asm__ __volatile__ ("nop")                    // Generates a "nop" no-operation opcode (normally nop16 on microMIPS devices)
#endif


#if defined(__XC16__) && !defined(__builtin_divmodud)
/* __builtin_divmodud() XC16 function
 *
 * returns (unsigned int) = (unsigned long)/(unsigned int), with the
 * (unsigned int) remainder being written back to *u16_remainder_ptr.
 *
 * Limits of the div.ud hardware instruction apply (i.e. quotient return value
 * must fit in 16-bits for mathematical correctness and divide by 0 causes a
 * trap exception).
 */
#define __builtin_divmodud(u32_dividend, u16_divisor, u16_remainder_ptr)    __builtin_divmodud(u32_dividend, u16_divisor, u16_remainder_ptr)
#endif


#if defined(__XC16__) && !defined(__builtin_divmodsd)
/* __builtin_divmodsd() XC16 function
 *
 * returns (signed int) = (signed long)/(signed int), with the
 * (signed int) remainder being written back to *s16_remainder_ptr.
 *
 * Limits of the div.sd hardware instruction apply (i.e. quotient must fit in
 * 16-bits for mathematical correctness and divide by 0 causes a trap exception).
 */
#define __builtin_divmodsd(s32_dividend, s16_divisor, s16_remainder_ptr)    __builtin_divmodsd(s32_dividend, s16_divisor, s16_remainder_ptr)
#endif


#if defined(__XC16__) && !defined(__builtin_divsd)
/* __builtin_divsd() XC16 function
 *
 * returns (signed int) = (signed long)/(signed int), discarding the remainder
 *
 * Limits of the div.sd hardware instruction apply (i.e. quotient must fit in
 * 16-bits for mathematical correctness and divide by 0 causes a trap exception).
 */
#define __builtin_divsd(s32_dividend, s16_divisor)    __builtin_divsd(s32_dividend, s16_divisor)
#endif


#if defined(__XC16__) && !defined(__builtin_divud)
/* __builtin_divud() XC16 function
 *
 * returns (unsigned int) = (unsigned long)/(unsigned int), discarding the
 * remainder.
 *
 * Limits of div.ud hardware instruction apply (i.e. quotient must fit in
 * 16-bits for mathematical correctness and divide by 0 causes a trap exception).
 */
#define __builtin_divud(u32_dividend, u16_divisor)    __builtin_divud(u32_dividend, u16_divisor)
#endif


#if defined(__XC16__)
/**
 * Macro defines and assigns the value of a Device Configuration Word. The
 * Configuration Word can be either "volatile" (i.e. stored in Flash program
 * memory) or "non-volatile" type (stored in byte-wide reprogrammable fuses
 * separate from the main or auxiliary Flash arrays.
 *
 * Unlike the #pragma config compiler directive, no validation is done against
 * the Config Word name or values you attempt to program them to. If the target
 * device does not have the named Configuration word, this macro will NOT
 * generate any errors or messages, allowing you to create generic definitions
 * that will work on potentially multiple different devices without changes.
 * Additionally, this macro will set all 24-bits of the Config word as you
 * specify it, rather than zero the upper 8 bits and only define the lower
 * 16-bits.
 *
 * Because Config Words are located in the 0xF800xx memory range on many
 * devices, it is often not feasible to read a Config Word's value by directly
 * referencing it as an extern __prog__ value. Attempting to reference any
 * address >= 0x800000 is impossible via EDS hardware (can only address 0x000000
 * to 0x7FFFFF in Program Space), so an incorrect address will be read instead.
 * To avoid this, it is recommended that any Config Word's value instead be read
 * using:
 *      <code>EZBL_ReadFlash(__FWDT_BASE)</code>
 * EZBL's EZBL_EZBL_ReadFlash() function will issue a Table Read against
 * the full 24-bit address and therefore always work, regardless of where your
 * device has Config Words located.
 *
 * @param config_word_name The all upper-case Configuration Word's name, as
 *                         specified in the target device's data sheet and with
 *                         the compiler's leading underscore. Ex: _FOSC
 *
 *                         If the target device does not have a matching
 *                         Configuration Word, this macro will still generate a
 *                         Flash linker section for it with the chosen value,
 *                         but it will be allocated to an arbitrary/otherwise
 *                         unoccupied Flash memory address chosen by the linker.
 *                         Alternatively, if the "Remove unused sections"
 *                         (--gc-sections) linker option is used, the generated
 *                         section will be silently thrown away as
 *                         non-referenced/dead data.
 *
 * @param config_value     The value to set the Config Word to. Generally,
 *                         this will be a bit-wise ANDed bitmask of preexisting
 *                         config values #defined in the device header file. Ex:
 *                         <code>EZBL_SET_CONF(_FWDT, WDTPS_PS2048 & FWPSA_PR32 & FWDTEN_ON)</code>
 *
 *                         With typical MPLAB X IDE settings, holding CTRL
 *                         and then clicking on an existing Config Word name or
 *                         macro value will automatically open the device header
 *                         and give you a comprehensive list of available config
 *                         bitfield names.
 */
#define EZBL_SET_CONF(config_word_name, config_value)                           \
__asm__("\n\t.pushsection   _" STRINGIFY(config_word_name) ".sec, code"         \
        "\n\t.global    _" STRINGIFY(config_word_name)                          \
        "\n\t.global    __" STRINGIFY(config_word_name)                         \
        "\n_" STRINGIFY(config_word_name) ":"                                   \
        "\n__" STRINGIFY(config_word_name) ":"                                  \
        "\n\t.pword " STRINGIFY(config_value)                                   \
        "\n\t.popsection");


/**
 * Creates a valid FBTSEQ value from 4095 (or preferably 4094) down to 0 and
 * saves its ones complement in the upper 12 bits of the 24-bit FBTSEQ program
 * location.
 *
 * Use this macro where you would normally define your device Configuration
 * Words. I.e. use EZBL_SET_CONF_FBTSEQ(1234) instead of _FBTSEQ(1234) or
 * #pragma config BSEQ = 1234, #pragma config IBSEQ = ~1234. The compiler (circa
 * XC16 v1.26) does not automatically handle the upper complement bit
 * calculations and the _FBTSEQ() macro will truncate off bits 16 to 23.
 *
 * @param seq_num The numerical partition boot up sequence number to use for
 *                this project's default .hex file. Valid values are 4095 down
 *                to 0 with higher numbers meaning "lower boot priority". It is
 *                recommended, however, that seq_num be set to <= 4094 instead
 *                of 4095.
 *
 *                In the event both partitions contain the same FBTSEQ priority
 *                value, the device will automatically select Partition 1 and
 *                make Partition 2 the inactive one.
 *
 *                In the event an FBTSEQ value (on either partition) is invalid
 *                due to a mismatched complement in the upper 12-bits (which is
 *                the erased state condition), the hardware will treat that
 *                partition's effective FBTSEQ value as 0x003FF (4095), which is
 *                the lowest priority. Note, however, that if Partition 1's
 *                FBTSEQ == 0xFFFFFF (i.e. the unprogrammed state) and
 *                Partition 2's FBTSEQ == 0x003FF (a valid 4095 lowest
 *                priority), the hardware will still select Partition 1 on reset
 *                for execution. This is due to 0xFFFFFF being invalid,
 *                internally treated as 0x003FF and then being slightly higher
 *                natural-order priority than Partition 2. To avoid having to
 *                worry about this, set seq_num to 4094 and lower only.
 *
 *
 * @return Creates the 24-bit FBTSEQ program Flash Configuration Word with the
 *         upper 12-bits computed and defined as the ones complement of the
 *         lower 12-bit seq_num value. The full 24-bit FBTSEQ Config word value
 *         will appear in the output hex file when this macro is used.
 */
#define EZBL_SET_CONF_FBTSEQ(seq_num)                                           \
__asm__("\n\t.pushsection   __FBTSEQ.sec, code"                                 \
        "\n\t.global        ___FBTSEQ"                                          \
        "\n___FBTSEQ:\t.pword " STRINGIFY(((seq_num & 0x000FFF) | (((~(seq_num & 0x0FFF))<<12) & 0xFFF000)))    \
        "\n\t.popsection");


/**
 * Creates a FBSLIM Config word value from a desired General Segment start address.
 *
 * @param gs_start_addr Flash program space address indicating the desired 
 *                      General Segment start address (inclusive). All addresses 
 *                      less than this value will belong to the Boot Segment 
 *                      (assuming FSEC<BSEN> == '0' to enable the Boot Segment) 
 *                      while all addresses equal to and greater than gs_start_addr 
 *                      will belong to the General Segment.
 *
 *                      This parameter should be integral divisble by the flash 
 *                      page erase size, in program space addresses (unaligned 
 *                      values will have low order address bits truncated off 
 *                      to start the General Segment at the closest aligned 
 *                      boundary that is less than the provided address).
 *
 *                      When enabling a Boot Segment to use the hardware AIVT 
 *                      function, the AIVT will reside at:
 *                          gs_start_addr - (one page erase block size) + 0x4
 *                      For example, on a device with a 0x800 address (3072 byte) 
 *                      page erase size, a bootloader that needs 0x001800 of reserved 
 *                      code space (inclusive of reset vector and IVT), specify
 *                      gs_start_addr = 0x2000. All addresses between 0x001800 
 *                      and 0x001FFF will be reserved for the AIVT and cannot be 
 *                      used for code in either the bootloader or application 
 *                      projects. The first AIVT trap vector will be located at 
 *                      0x001804.
 *
 * @return Creates the FBSLIM program Flash Configuration Word encoded as
 *         required for hardware to match the provided gs_start_addr.
 */
#define EZBL_SET_CONF_FBSLIM(gs_start_addr)                                                 \
__asm__("\n\t.pushsection   __FBSLIM.sec, code"                                             \
        "\n\t.global        ___FBSLIM"                                                      \
        "\n___FBSLIM:\t.pword " STRINGIFY(-((gs_start_addr)/_EZBL_ADDRESSES_PER_SECTOR)-1)  \
        "\n\t.popsection");
#endif


#if defined(__XC32__) || defined(__PIC32__)
enum EZBL_CP0_REGSEL
{
    // Common Name = ((MIPS CP0 Register Num)<<16) | (Select Num)<<11;
    EZBL_CP0_BadVAddr  =  (8<<16) | (0<<11),    // Address for the most recent address-related exception
    EZBL_CP0_BadInstr  =  (8<<16) | (1<<11),    // Instruction that caused the most recent exception
    EZBL_CP0_BadInstrP =  (8<<16) | (2<<11),    // Reports the branch instruction if a delay slot caused the most recent exception
    EZBL_CP0_Count     =  (9<<16) | (0<<11),    // Core Timer (processor cycle count/2)
    EZBL_CP0_Compare   = (11<<16) | (0<<11),    // Core Timer period/compare interrupt value
    EZBL_CP0_Status    = (12<<16) | (0<<11),    // Processor Status and control
    EZBL_CP0_IntCtrl   = (12<<16) | (1<<11),    // Interrupt system status and control
    EZBL_CP0_SRSCtl    = (12<<16) | (2<<11),    // Shadow Register Sets status and control
    EZBL_CP0_SRSMap1   = (12<<16) | (3<<11),    // Shadow set IPL mapping
    EZBL_CP0_View_IPL  = (12<<16) | (4<<11),    // Contiguous view of IM and IPL fields
    EZBL_CP0_SRSMap2   = (12<<16) | (5<<11),    // Shadow set IPL mapping
    EZBL_CP0_Cause     = (13<<16) | (0<<11),    // Cause of last exception
    EZBL_CP0_View_RIPL = (13<<16) | (4<<11),
    EZBL_CP0_NestedExc = (13<<16) | (5<<11),
    EZBL_CP0_EPC       = (14<<16) | (0<<11),    // Program counter at last exception
    EZBL_CP0_NestedEPC = (14<<16) | (2<<11),    // Program counter at last exception
    EZBL_CP0_PRId      = (15<<16) | (0<<11),    // Processor identification and revision
    EZBL_CP0_EBase     = (15<<16) | (1<<11),    // Exception Base Address Register
    EZBL_CP0_PerfCtl0  = (25<<16) | (0<<11),    // Performance Counter 0 Control
    EZBL_CP0_PerfCnt0  = (25<<16) | (1<<11),    // Performance Counter 0
    EZBL_CP0_PerfCtl1  = (25<<16) | (2<<11),    // Performance Counter 1 Control
    EZBL_CP0_PerfCnt1  = (25<<16) | (3<<11),    // Performance Counter 1
    EZBL_CP0_ErrorEPC  = (30<<16) | (0<<11),    // Program counter at last error
};


/**
 * Macro reads a PIC32 MIPS Coprocessor 0 (CP0) register, supporting MPLAB X
 * IDE context sensitive help for the register name/select definition.
 *
 * @param enum EZBL_CP0_REGSEL Compile time constant specifying which CP0
 *                             Register + Select to read from. This should be
 *                             one of the values defined in the EZBL_CP0_REGSEL
 *                             enumeration and not obtained from cp0defs.h.
 *
 * @return 32-bit unsigned integer read from the CP0 register.
 */
static inline unsigned int __attribute__((always_inline, optimize(1))) EZBL_MFC0(const enum EZBL_CP0_REGSEL cp0RegSel)
{
    unsigned int i;
    __asm__("mfc0   %0, $%1, %2" : "=d"(i) : "O"(cp0RegSel>>16), "O"((cp0RegSel>>11) & 0x7));
    return i;
}


/**
 * Macro writes a PIC32 MIPS Coprocessor 0 (CP0) register, supporting MPLAB X
 * IDE context sensitive help for the register name/select definition.
 *
 * @param enum EZBL_CP0_REGSEL Compile time constant specifying which CP0
 *                             Register + Select to write to. This should be one
 *                             of the values defined in the EZBL_CP0_REGSEL
 *                             enumeration and not obtained from cp0defs.h.
 *
 * @param value 32-bit value to write to the CP0 register. This value can be
 *              specified at run time as a variable.
 */
static inline void __attribute__((always_inline, optimize(1))) EZBL_MTC0(const enum EZBL_CP0_REGSEL cp0RegSel, unsigned int value)
{
    __asm__("mtc0   %0, $%1, %2" : : "c"(value), "O"(cp0RegSel>>16), "O"((cp0RegSel>>11) & 0x7));
}


/**
 * Macro sets bits within a PIC32 MIPS Coprocessor 0 (CP0) register, supporting
 * MPLAB X IDE context sensitive help for the register name/select definition.
 *
 * @param enum EZBL_CP0_REGSEL Compile time constant specifying which CP0
 *                             Register + Select to write to. This should be one
 *                             of the values defined in the EZBL_CP0_REGSEL
 *                             enumeration and not obtained from cp0defs.h.
 *
 * @param mask 32-bit value indicating which bits to set in the CP0 register.
 *             '1' bits will be written as '1's, while '0' bits will be left
 *             unchanged through a read-modify-write operation.
 *
 *             This value can be specified at run time as a variable.
 */
static inline void __attribute__((always_inline, optimize(1))) EZBL_BSC0(const enum EZBL_CP0_REGSEL cp0RegSel, unsigned int mask)
{
    EZBL_MTC0(cp0RegSel, EZBL_MFC0(cp0RegSel) | mask);
}


/**
 * Macro clears bits within a PIC32 MIPS Coprocessor 0 (CP0) register,
 * supporting MPLAB X IDE context sensitive help for the register name/select
 * definition.
 *
 * @param enum EZBL_CP0_REGSEL Compile time constant specifying which CP0
 *                             Register + Select to write to. This should be one
 *                             of the values defined in the EZBL_CP0_REGSEL
 *                             enumeration and not obtained from cp0defs.h.
 *
 * @param mask 32-bit value indicating which bits to clear in the CP0 register.
 *             '1' bits will be written as '0's, while '0' bits will be left
 *             unchanged through a read-modify-write operation.
 *
 *             This value can be specified at run time as a variable.
 */
static inline void __attribute__((always_inline, optimize(1))) EZBL_BCC0(const enum EZBL_CP0_REGSEL cp0RegSel, unsigned int mask)
{
    EZBL_MTC0(cp0RegSel, EZBL_MFC0(cp0RegSel) & ~mask);
}

#endif


/**
 * Macro keeps a function, variable, or other linker symbol from going
 * unreferenced and optimized away as dead code/ram/information when the
 * --gc-sections linker option (MPLAB X IDE "Discard unused sections" project
 * option) is specified at project build time. Also ensures a strong symbol
 * reference exists to trigger searching of archive libraries for weak declared
 * extern functions or variables.
 *
 * NOTE: This macro will not work if the symbol you are trying to keep is
 * declared in the same source file as a weak symbol or as an extern weak symbol
 * which you are referencing in the same source file. To keep these weak symbols
 * place the EZBL_KeepSYM() macro call in a different source file.
 *
 * Unlike the EZBL_KeepSYMIfPresent() macro, if the symbol is not declared at
 * link time, then linking failure will occur, signaling that an expected
 * file/function/variable function is missing from the project. Use
 * EZBL_KeepSYMIfPresent() if you just want to create a symbol reference
 * which can be optionally ignored if missing.
 *
 * This macro makes a dummy reference to the specified symbol within the 'info'
 * linker memory space. The reference requires no RAM or Flash space, but in the
 * event the named symbol would ordinarily have been garbaged collected, both
 * Flash and RAM using can grow. All data or instructions within the linker
 * section that the referenced symbol is defined in is kept. Additionally, any
 * symbols/sections that the kept code references will also become automatically
 * kept if not referenced elsewhere.
 *
 * @param symbolName    The C function or variable name to keep (without a
 *                      leading underscore).
 *
 */
#define __EZBL_KeepSYM1(symbolName)                                             \
__asm__ ("\n    .pushsection .info.EZBL_KeepSYM, info, keep"                    \
         "\n    .long   " STRINGIFY_SYM(symbolName)                             \
         "\n    .popsection"                                                    \
)

#define __EZBL_KeepSYM0(...)
#define __EZBL_KeepSYM2(symbolName, ...)     __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM1(__VA_ARGS__)
#define __EZBL_KeepSYM3(symbolName, ...)     __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM2(__VA_ARGS__)
#define __EZBL_KeepSYM4(symbolName, ...)     __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM3(__VA_ARGS__)
#define __EZBL_KeepSYM5(symbolName, ...)     __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM4(__VA_ARGS__)
#define __EZBL_KeepSYM6(symbolName, ...)     __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM5(__VA_ARGS__)
#define __EZBL_KeepSYM7(symbolName, ...)     __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM6(__VA_ARGS__)
#define __EZBL_KeepSYM8(symbolName, ...)     __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM7(__VA_ARGS__)
#define __EZBL_KeepSYM9(symbolName, ...)     __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM8(__VA_ARGS__)
#define __EZBL_KeepSYM10(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM9(__VA_ARGS__)
#define __EZBL_KeepSYM11(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM10(__VA_ARGS__)
#define __EZBL_KeepSYM12(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM11(__VA_ARGS__)
#define __EZBL_KeepSYM13(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM12(__VA_ARGS__)
#define __EZBL_KeepSYM14(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM13(__VA_ARGS__)
#define __EZBL_KeepSYM15(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM14(__VA_ARGS__)
#define __EZBL_KeepSYM16(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM15(__VA_ARGS__)
#define __EZBL_KeepSYM17(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM16(__VA_ARGS__)
#define __EZBL_KeepSYM18(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM17(__VA_ARGS__)
#define __EZBL_KeepSYM19(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM18(__VA_ARGS__)
#define __EZBL_KeepSYM20(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM19(__VA_ARGS__)
#define __EZBL_KeepSYM21(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM20(__VA_ARGS__)
#define __EZBL_KeepSYM22(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM21(__VA_ARGS__)
#define __EZBL_KeepSYM23(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM22(__VA_ARGS__)
#define __EZBL_KeepSYM24(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM23(__VA_ARGS__)
#define __EZBL_KeepSYM25(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM24(__VA_ARGS__)
#define __EZBL_KeepSYM26(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM25(__VA_ARGS__)
#define __EZBL_KeepSYM27(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM26(__VA_ARGS__)
#define __EZBL_KeepSYM28(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM27(__VA_ARGS__)
#define __EZBL_KeepSYM29(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM28(__VA_ARGS__)
#define __EZBL_KeepSYM30(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM29(__VA_ARGS__)
#define __EZBL_KeepSYM31(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM30(__VA_ARGS__)
#define __EZBL_KeepSYM32(symbolName, ...)    __EZBL_KeepSYM1(symbolName); __EZBL_KeepSYM31(__VA_ARGS__)
#define __EZBL_KeepSYMN(...)     CAT(__EZBL_KeepSYM,__VA_COUNT(__VA_ARGS__))(__VA_ARGS__)


/**
 * Macro keeps one or more function, variable, or other linker symbols from
 * going unreferenced and optimized away as dead code/ram/information when the
 * --gc-sections linker option (MPLAB X IDE "Discard unused sections" project
 * option) is specified at project build time. Also ensures a strong symbol
 * reference exists to trigger searching of archive libraries for weak declared
 * extern functions or variables.
 *
 * NOTE: This macro will not work if the symbol(s) you are trying to keep are
 * declared in the same source file as a weak symbol or as an extern weak symbol
 * which you are referencing in the same source file. To keep these weak symbols
 * place the EZBL_KeepSYM() macro call in a different source file.
 *
 * Unlike the EZBL_KeepSYMIfPresent() macro, if the symbol is not declared at
 * link time, then linking failure will occur, signaling that an expected
 * file/function/variable function is missing from the project. Use
 * EZBL_KeepSYMIfPresent() if you just want to create a symbol reference
 * which can be optionally ignored if missing.
 *
 * This macro makes a dummy reference to the specified symbol(s) within the
 * 'info' linker memory space. The reference requires no RAM or Flash space, but
 * in the event the named symbol would ordinarily have been garbaged collected,
 * both flash and RAM usage can grow. All data or instructions within the linker
 * section that the referenced symbol is defined in is kept. Additionally, any
 * symbols/sections that the kept code references will also become automatically
 * kept if not referenced elsewhere.
 *
 * This macro may be used both at global/file scope or within a function with
 * the same effect in both locations.
 *
 * @param ...    The C function or variable names to keep (without a leading
 *               underscore). Each comma separated parameter represents one
 *               symbol name to keep.
 *
 *               This macro supports 0 to 32 simultaneous arguments. If more
 *               than 32 symbols must be kept, use the EZBL_KeepSYM() macro
 *               multiple times.
 */
#define EZBL_KeepSYM(...)   __EZBL_KeepSYMN(__VA_ARGS__)



#define __EZBL_KeepSYMIfPresent1(symbolName)                                    \
__asm__ ("\n    .pushsection .info.EZBL_KeepSYMIfPresent, info, keep"           \
         "\n    .ifndef __" STRINGIFY_SYM(symbolName) "_def_local__"            \
         "\n    .weak   " STRINGIFY_SYM(symbolName)                             \
         "\n    .endif"                                                         \
         "\n     .long   " STRINGIFY_SYM(symbolName)                            \
         "\n    .popsection"                                                    \
)
#define __EZBL_KeepSYMIfPresent0(...)
#define __EZBL_KeepSYMIfPresent2(symbolName, ...)   __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent1(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent3(symbolName, ...)   __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent2(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent4(symbolName, ...)   __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent3(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent5(symbolName, ...)   __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent4(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent6(symbolName, ...)   __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent5(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent7(symbolName, ...)   __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent6(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent8(symbolName, ...)   __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent7(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent9(symbolName, ...)   __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent8(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent10(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent9(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent11(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent10(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent12(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent11(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent13(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent12(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent14(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent13(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent15(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent14(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent16(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent15(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent17(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent16(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent18(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent17(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent19(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent18(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent20(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent19(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent21(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent20(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent22(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent21(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent23(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent22(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent24(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent23(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent25(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent24(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent26(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent25(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent27(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent26(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent28(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent27(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent29(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent28(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent30(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent29(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent31(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent30(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresent32(symbolName, ...)  __EZBL_KeepSYMIfPresent1(symbolName); __EZBL_KeepSYMIfPresent31(__VA_ARGS__)
#define __EZBL_KeepSYMIfPresentN(...)               CAT(__EZBL_KeepSYMIfPresent,__VA_COUNT(__VA_ARGS__))(__VA_ARGS__)


/**
 * Macro keeps function, variable, or other linker symbols from going
 * unreferenced and optimized away as dead code/ram/information when the
 * --gc-sections linker option (MPLAB X IDE "Discard unused sections" project
 * option) is specified at project build time.
 *
 * Unlike the EZBL_KeepSYM() macro, if the symbols are not declared at link
 * time, then nothing is silently kept, allowing linking to complete. Use
 * EZBL_KeepSYM() instead of EZBL_KeepSYMIfPresent() to force a
 * compile-time linker error if the specified symbol(s) are not declared.
 *
 * This macro makes a dummy reference to the specified symbols within the 'info'
 * linker memory space. The reference requires no RAM or flash space, but in the
 * event the named symbols would ordinarily have been garbaged collected, both
 * flash and RAM usage could grow. All data or instructions within the linker
 * section that the referenced symbols are defined in is kept. Additionally, any
 * symbols/sections that the kept code references will also become automatically
 * kept if not referenced elsewhere.
 *
 * @param ... Up to 32 comma separated C function or variable names to try and
 *            keep (without a leading underscore, which is appended by the macro
 *            internally when using XC16).
 */
#define EZBL_KeepSYMIfPresent(...)   __EZBL_KeepSYMIfPresentN(__VA_ARGS__)


/**
 * void EZBL_SetSYM(symbolName, value);
 *
 * Macro declares a link time absolute symbol of the specified name and sets its
 * value. Symbols defined with this macro can be obtained in other source files
 * using the EZBL_SYM32(), EZBL_SYM(), EZBL_WeakSYM32(), or EZBL_WeakSYM()
 * macros.
 *
 * @param symbolName Token or identifier to name the symbol. This token
 *                   automatically receives one leading underscore for
 *                   consistency with compiler generated symbols and the
 *                   EZBL_SYM32() reading macros.
 *
 * @param value Numerical value or address to assign to the symbol. The number
 *              must by immediately evaluatable by the assembler. Ex: 5, 5+9,
 *              1337*2, 16/2*3, ((0xdead) & 0x42), etc. are all allowed, whereas
 *              sizeof(unsigned long), 5 ? 1 : 0, &myVar, etc. will fail to
 *              assemble.
 * @return _symbolName is created as a global absolute symbol of the specified
 *         value. The symbol is not subject to section discarding when
 *         unreferenced (-gc-sections).
 */
#define EZBL_SetSYM(symbolName, value)                                          \
    __asm__("\n     .pushsection    .info.EZBL_KeepSYM, info, keep"             \
            "\n     .global " STRINGIFY_SYM(symbolName)                         \
            "\n" STRINGIFY_SYM(symbolName) " = " STRINGIFY(value)               \
            "\n     .ifdef " STRINGIFY_SYM(symbolName)                          \
            "\n     .equ __" STRINGIFY_SYM(symbolName) "_def_local__, 1"        \
            "\n     .endif"                                                     \
            "\n     .long " STRINGIFY(value) " /* Store data in case if it is a symbol reference */ " \
            "\n     .popsection"                                                \
    )


/**
 * void EZBL_SetWeakSYM(symbolName, value);
 *
 * Macro declares a link time absolute symbol of the specified name and sets its
 * value. The symbol is exported as a weak symbol, so calls to EZBL_SetSYM() in
 * a separate source file can override the value set by this call.
 *
 * Symbols defined with this macro can be obtained in other source files
 * using the EZBL_SYM32(), EZBL_SYM(), EZBL_WeakSYM32(), or EZBL_WeakSYM()
 * macros.
 *
 * @param symbolName Token or identifier to name the symbol. This token
 *                   automatically receives one leading underscore with XC16 for
 *                   consistency with compiler generated symbols and the
 *                   EZBL_SYM32() reading macros. With XC32, no leading
 *                   underscore is appended.
 *
 * @param value Numerical value or address to assign to the symbol. The number
 *              must by immediately evaluatable by the assembler. Ex: 5, 5+9,
 *              1337*2, 16/2*3, ((0xdead) & 0x42), etc. are all allowed, whereas
 *              sizeof(unsigned long), 5 ? 1 : 0, &myVar, etc. will fail to
 *              assemble.
 * @return _symbolName (XC16) or symbolName (XC32) is created as a global weak
 *         absolute symbol of the specified value. The symbol is not subject to
 *         section discarding when unreferenced (-gc-sections).
 */
#define EZBL_SetWeakSYM(symbolName, value)                                      \
    __asm__("\n     .pushsection    .info.EZBL_KeepSYM, info, keep"             \
            "\n     .weak " STRINGIFY_SYM(symbolName)                           \
            "\n" STRINGIFY_SYM(symbolName) " = " STRINGIFY(value)               \
            "\n     .ifdef " STRINGIFY_SYM(symbolName)                          \
            "\n     .equ __" STRINGIFY_SYM(symbolName) "_def_local__, 1"        \
            "\n     .endif"                                                     \
            "\n     .long " STRINGIFY(value) " /* Store data in case if it is a symbol reference */ " \
            "\n     .popsection"                                                \
    )


/**
 * Macro prevents zero or more function, global variable, constant or other
 * global linker symbol(s) from being exported from an EZBL Bootloader project
 * for use by Application projects.
 *
 * Internally, this macro saves the specified regular expression string(s) to
 * the .info.EZBL_NoExportSYM debugging section, which is then read by
 * ezbl_tools.jar during .merge.S file creation. If any symbol exists in the
 * Bootloader project and matches any of the no-export regular expression
 * strings, the symbol will be explicitly prevented from being defined in the
 * .merge.S file. Referencing this macro consumes no flash/RAM/run-time
 * execution or other device resources and will only work against normal or
 * single partition Bootloader projects.
 *
 * This macro can be referenced multiple times and in any .c source file within
 * the project.
 *
 * @param symbolNameRegEx Literal constant regular expression string containing
 *                        the function, variable, object, symbol name or pattern
 *                        to prevent the export of.
 *
 *                        If the pattern does not match any symbols in the
 *                        project, this macro has no effect.
 *
 *                        As an example, to prevent the function myFunc() from
 *                        being visible in the App project, use:
 *                          EZBL_NoExportSYM("_muFunc");   // XC16
 *                        Or
 *                          EZBL_NoExportSYM("muFunc");    // XC32
 *
 *                        Unlike most other EZBL macros accepting symbol
 *                        parameters, this macro will not generate a leading
 *                        underscore for XC16 symbols. To abstractly suppress
 *                        the export of myFunc()'s address for both toolchains
 *                        with the same macro call, use a regular expression
 *                        that makes the underscore optional, for example:
 *                          EZBL_NoExportSYM("_?myFunc");
 */
#define EZBL_NoExportSYM(symbolNameRegEx)                                       \
    __asm__("\n     .pushsection    .info.EZBL_NoExportSYM, info, keep"         \
            "\n     .string \"" symbolNameRegEx "\""                            \
            "\n     .popsection")


/**
 * Macro prevents all symbols within zero or more named sections from being
 * exported from an EZBL Bootloader project for use by Application projects.
 * 
 * To suppress a symbol from being exported by a specific name, use
 * EZBL_NoExportSYM() instead.
 *
 * Internally, this macro saves the specified regular expression string(s) to
 * the .info.EZBL_NoExportSection debugging section, which is then read by
 * ezbl_tools.jar during .merge.S file creation. Any symbol that exists in the
 * specified section (or section matching the name pattern) in the Bootloader
 * project will be explicitly prevented from being defined in the
 * .merge.S file. Referencing this macro consumes no flash/RAM/run-time
 * execution or other device resources and will only work against normal or
 * single partition Bootloader projects.
 *
 * This macro can be referenced multiple times and in any .c source file within
 * the project.
 *
 * @param sectionNameRegEx Literal constant regular expression string containing
 *                         the section name or section-name pattern to prevent symbol exporting from.
 *
 *                        If the pattern does not match any sections in the
 *                        project, this macro has no effect.
 */
#define EZBL_NoExportSection(sectionNameRegEx)                                  \
    __asm__("\n     .pushsection    .info.EZBL_NoExportSection, info, keep"     \
            "\n     .string \"" sectionNameRegEx "\""                           \
            "\n     .popsection")


/**
 * Macro creates a "hole" in program memory where no code or constants shall
 * reside, but space will be allocated and consumed from other linker uses. The
 * compiled .hex file will contain a void in the allocated address range with no
 * records defining any value for them, and unintentional attempts to use the
 * same memory locations will trigger a build-time linker allocation error.
 *
 * The exact hole is defined as a:
 *   __prog__ unsigned int __attribute__((space(prog), aligned(<i>padAlignAddrSize</i>), section(".text.<i>holeName</i>"))) <i>holeName</i>[];
 * with effective allocated size of the chosen byteLen + start address
 * alignment + end address padding, and optionally absolute address assignment.
 *
 * Unlike allocation in XC16 C code, this array can be any length and is not
 * limited to around 32K addresses in size. Any size can be chosen, and when an
 * absolute address is assigned, it is also possible to allocate the hole
 * outside of implemented Flash memory. However, this also means that the
 * sizeof() operator cannot be used as the array length is not known at compile
 * time.
 *
 * The EZBL_FlashHoleAddr(), EZBL_FlashHoleAddrEnd(), EZBL_FlashHoleAddrLen(),
 * and EZBL_FlashHoleLen() macros can be used to retrieve the allocated object's
 * 32-bit true program address, end address, resultant address width, and
 * resultant byte width. They may additionally be called from different source
 * files as the object is declared at global scope (without a need to declare a
 * prototype for it).
 *
 * @param holeName
 *      C identifier for the hole program object. If you wish to take the
 *      address of the array or make a reference to an index in it, this is the
 *      exact name you would use. Therefore, the name can only contain
 *      [_a-zA-Z0-9] characters in it, and cannot start with a [0-9] character.
 *
 * @param byteLen
 *      Number of Flash bytes to allocate to the array. On 16-bit devices, this
 *      byte count is computed based on storing 3 bytes into each 24-bit program
 *      instruction word.
 *
 *      The true number of array elements will be computed such that at least
 *      the specified byteLen will exist, subject to minimum alignment and
 *      padding requirements. Ex: 1,2 or 3 bytes will allocate no less than 0x2
 *      Program addresses or 1 24-bit instruction word worth of Flash memory on
 *      16-bit devices.
 *
 *      This parameter number must be an explicit number or immediately
 *      computable in assembly language. A computed value like (24*3+0x2)|0x7 is
 *      allowed, including with preprocessor macros, while something like
 *      sizeof(double)*16 will not work. C type casts, builtins, or type
 *      identifier suffixes are not allowed.
 *
 * @param padAlignAddrSize
 *      Number of program addresses to both align and pad to. For typical
 *      Bootloader, EEPROM emulation, and other Flash uses where elements must
 *      be erased without destroying adjacent data, the padding and alignment
 *      must be the minimum Flash Page erase size. On most devices, this is
 *      either 0x400 or 0x800 program addresses (1536 or 3072 Flash bytes) (see
 *      device data sheet as it is part specific).
 *
 *      Alignment refers to the start address boundary for the array. Padding
 *      refers to effective byteLen extension to ensure the end address of the
 *      array perfectly matches a Flash Erase Page end address, thus preventing
 *      dynamically allocated code or other Flash objects from being placed
 *      unintentionally on the same erasable block.
 *
 *      If the hole will never be erased and will only be programmed, the
 *      device's minimum Flash programming block size will normally apply, which
 *      is most commonly 0x2 or 0x4 program addresses, depending on the NVM
 *      controllers support for a "word" programming opcode or a "double word"
 *      programming opcode.
 *
 *      As the XC16 linker does not support objects at odd addresses, the
 *      minimum padAlignAddrSize is 0x2 on 16-bit devices. The minimum is 1 on
 *      32-bit devices.
 *
 *      Like byteLen, this parameter must be immediately known or computable by
 *      the assembler so can contain no C constructs or operators (other than
 *      math and logic operators or equivalent preprocessor macros).
 *
 * @param optAllocAddr
 *      Optional parameter specifying an absolute address to allocate this Flash
 *      hole to. Set to <i>-1</i> to let the linker's best fit allocator
 *      dynamically select the start address (subject to the alignment and
 *      padding specified).
 *
 *      If an absolute allocation address is specified which does not match the
 *      alignment size, the lower bits will be masked off as '0's to match the
 *      closest alignment boundary. This means, the resulting position may be at
 *      a lower address in such cases.
 *
 *      Sometimes when allocating objects at high addresses, the best fit
 *      allocator in the linker will begin using addresses after the allocated
 *      hole for subsequent objects rather than low memory addresses which may
 *      go unused. If this behavior is undesirable, call EZBL_AllocFlashHole()
 *      again, immediately after the first one, and declare a second hole of 1
 *      byteLen, 0x2 paddAlignAddrSize, and with an optAllocAddr someplace low
 *      in memory where things exist (where you'd like them to exist).
 *
 *      Like byteLen, this parameter cannot use C constructs unknown to the
 *      assembler.
 *
 * @return
 *      An extern declaration for the allocated program space hole object of
 *      the specified name.
 */
#define EZBL_AllocFlashHole(holeName, byteLen, padAlignAddrSize, optAllocAddr)                                                              \
    __asm__(                                                                                                                                \
            "\n     .set    XC16Assembler, 0"                                                                                               \
            "\n     .set    objSize, " STRINGIFY(byteLen)                                                                                   \
            "\n     .set    allocAddr, " STRINGIFY(optAllocAddr)                                                                            \
            "\n     .set    alignSize, " STRINGIFY(padAlignAddrSize)                                                                        \
            "\n     .set    paddingSize, 0"                                                                                                 \
            "\n     .ifdef __C30ELF"                                                                                                        \
            "\n         .set XC16Assembler, 1"                                                                                              \
            "\n     .endif"                                                                                                                 \
            "\n     .ifdef __C30COFF"                                                                                                       \
            "\n         .set XC16Assembler, 1"                                                                                              \
            "\n     .endif"                                                                                                                 \
            "\n     .if XC16Assembler == 1"                                                                                                 \
            "\n         .set objSize, " STRINGIFY(((byteLen)+2)/3*0x2)                                                                      \
            "\n         .if alignSize < 2"                                                                                                  \
            "\n             .set alignSize, 2"                                                                                              \
            "\n         .else"                                                                                                              \
            "\n             .set paddingSize, " STRINGIFY(((padAlignAddrSize) - (((((byteLen)+2)/3*0x2)) % (padAlignAddrSize))) % (padAlignAddrSize))   \
            "\n             .if allocAddr >= 0"                                                                                             \
            "\n                 .set allocAddr, " STRINGIFY((optAllocAddr) - ((optAllocAddr)%(padAlignAddrSize)))                           \
            "\n             .endif"                                                                                                         \
            "\n         .endif"                                                                                                             \
            "\n     .else   /* PIC32 case */"                                                                                               \
            "\n         .if alignSize <= 1"                                                                                                 \
            "\n             .set alignSize, 1"                                                                                              \
            "\n         .else"                                                                                                              \
            "\n             .set paddingSize, " STRINGIFY(((padAlignAddrSize) - ((byteLen) % (padAlignAddrSize))) % (padAlignAddrSize))     \
            "\n             .if allocAddr >= 0"                                                                                             \
            "\n                 .set allocAddr, " STRINGIFY((optAllocAddr) - ((optAllocAddr)%(padAlignAddrSize)))                           \
            "\n             .endif"                                                                                                         \
            "\n         .endif"                                                                                                             \
            "\n     .endif"                                                                                                                 \
            "\n     .if (objSize == 0) && (paddingSize == 0)"                                                                               \
            "\n         .set paddingSize, alignSize /* When objSize == paddingSize == 0, generate padding to prevent section discard */"    \
            "\n      .endif"                                                                                                                \
            "\n     .if allocAddr >= 0"                                                                                                     \
            "\n         .pushsection    .text." STRINGIFY(holeName) ", code, keep, noload, address(allocAddr)"                              \
            "\n     .else"                                                                                                                  \
            "\n         .pushsection    .text." STRINGIFY(holeName) ", code, keep, noload, align(alignSize)"                                \
            "\n     .endif"                                                                                                                 \
            "\n     .global         " STRINGIFY_SYM(holeName)                                                                               \
            "\n     .type           " STRINGIFY_SYM(holeName) ", @object"                                                                   \
            "\n" STRINGIFY_SYM(holeName) ":"                                                                                                \
            "\n     .space objSize"                                                                                                         \
            "\n     .space paddingSize"                                                                                                     \
            "\n     .size  " STRINGIFY_SYM(holeName) ", . - " STRINGIFY_SYM(holeName)                                                       \
            "\n     .popsection"                                                                                                            \
            );                                                                                                                              \
            extern __prog__ unsigned int __attribute__((space(prog), aligned((padAlignAddrSize) < 1 ? 1 : (padAlignAddrSize)), section(".text." STRINGIFY(holeName)))) holeName[]


/**
 * Macro returns the unsigned long starting program address of a Flash hole
 * declared earlier (or elsewhere in another file) with the
 * EZBL_AllocFlashHole() macro.
 */
#define EZBL_FlashHoleAddr(holeName)    __builtin_section_begin(".text." STRINGIFY(holeName))

/**
 * Macro returns the unsigned long ending program address of a Flash hole
 * declared earlier (or elsewhere in another file) with the
 * EZBL_AllocFlashHole() macro. The end address is the first address after the
 * hole which the hole does not occupy.
 */
#define EZBL_FlashHoleAddrEnd(holeName) __builtin_section_end(  ".text." STRINGIFY(holeName))

/**
 * Macro returns the length of a Flash hole declared earlier (or elsewhere in
 * another file) with the EZBL_AllocFlashHole() macro. The units are in program
 * addresses, not bytes. Each 3 bytes equates to 0x2 addresses, and an odd
 * address will never be returned.
 */
#define EZBL_FlashHoleAddrLen(holeName) __builtin_section_size(".text." STRINGIFY(holeName))

#if defined(__XC16__)
/**
 * Macro returns the number of bytes occupied by a Flash hole declared earlier
 * (or elsewhere in another file) by the EZBL_AllocFlashHole() macro. The units
 * are in program space bytes, not addresses. Each 0x2 program addresses equates
 * to 3 bytes.
 */
#define EZBL_FlashHoleLen(holeName)     (__builtin_section_size(".text." STRINGIFY(holeName))/2u*3u)
#elif defined(__XC32__) || defined(__PIC32__)
/**
 * Macro returns the number of bytes occupied by a Flash hole declared earlier
 * (or elsewhere in another file) by the EZBL_AllocFlashHole() macro.
 */
#define EZBL_FlashHoleLen(holeName)     __builtin_section_size(".text." STRINGIFY(holeName))
#endif


/**
 * void EZBL_SetAppReservedHole(unsigned long startAddr, unsigned long endAddr);
 *
 * Macro reserves a range of program space addresses as Application space, off
 * limits for Bootloader project code or const objects.
 *
 * This macro is intended for use in EZBL Bootloader projects (particularly
 * normal, Single Partition types).
 *
 * Typically, all implemented flash memory addresses not consumed by Bootloader
 * code, const data or page erase size padding are defined as Application
 * space. However, by explicitly using EZBL_SetAppRange(), it is possible to
 * mark address ranges as keep-out when linking the Bootloader image. This
 * allows automatic, best-fit located Bootloader code to flow around reserved
 * parts of flash so future Application projects are guaranteed to be able to
 * use the range.
 *
 * This macro can also be used to force free space to exist on the same PSV/EDS
 * page that the Bootloader project allocates its .const section to. This
 * ensures space for future Application project .const sections which must
 * reside on the same PSV/EDS page as the Bootloader uses to permit using
 * strings and constants in all functions across both projects.
 *
 * @param startAddr unsigned long Program space starting address to reserve for
 *                  Application code. This must be a literal number or constant
 *                  expression that the assembler can evaluate.
 *
 *                  This address should be aligned to the device's minimum flash
 *                  erase size (typically 0x400 or 0x800 addresses). Unaligned
 *                  addresses can allow Bootloader code to exist elsewhere on
 *                  the same erase page, forcing page padding and disallowing
 *                  the complete range from being used in future Application
 *                  projects.
 *
 * @param endAddr unsigned long Program space ending address for the reserved
 *                region. This is the first address that will not be explicitly
 *                marked as reserved.
 *
 *                This address should be erase page aligned.
 *
 * @return void
 */
#define EZBL_SetAppReservedHole(startAddr, endAddr)                                                                                         \
    __asm__("\n     .pushsection .text.EZBL_AppReservedHole." STRINGIFY(__LINE__)", code, noload, keep, address(" STRINGIFY(startAddr) ")"  \
            "\n_EZBL_AppReservedRange_" STRINGIFY(__LINE__)":"                                                                              \
            "\n     .space (" STRINGIFY(endAddr) ") - (" STRINGIFY(startAddr) ")"                                                           \
            "\n     .popsection"                                                                                                            \
            )


/**
 * void EZBL_SetAppReservedRAMHole(unsigned int startAddr, unsigned int endAddr);
 *
 * Macro reserves a range of data space addresses as Application space, off
 * limits for Bootloader global/static variables or data space constants.
 *
 * This macro is intended for use in EZBL Bootloader projects.
 *
 * Typically, the linker will use RAM starting from the base address of RAM and
 * allocate to higher addresses until all variables have been allocated.
 * However, some RAM locations have special hardware abilities (such as 'near'
 * RAM located in the first 8192 bytes of data addressing space). This macro
 * generates a specially named keep-out/noload block in data space so the linker
 * cannot use the addresses, ensuring it stays available for future Application
 * projects.
 *
 * @param startAddr unsigned int Data space starting address to reserve for
 *                  Applications. This must be a literal number or constant
 *                  expression that the assembler can evaluate.
 *
 *                  To reserve near RAM for Applications, specify a start
 *                  address above SFRs and ICD debugger RAM (ex: 0x1050), but
 *                  below the near-space addressing limit of 8192 (0x2000).
 *
 * @param endAddr unsigned int Data space ending address for the reserved
 *                region. This is the first address that will not be explicitly
 *                marked as reserved.
 *
 * @return Generates a .EZBL_AppReservedRAMHole.(line number) section in RAM
 *         with persistent, noload, keep and absolute address attributes
 *         applied.
 *
 *         ezbl_tools.jar deletes data sections containing
 *         "EZBL_AppReservedRAMHole" in their section name (with the noload
 *         attribute) so the range is unoccupied in the generated
 *          [project_name].merge.S file that Applications links against.
 */
#define EZBL_SetAppReservedRAMHole(startAddr, endAddr)                                                                                      \
    __asm__("\n     .pushsection .EZBL_AppReservedRAMHole." STRINGIFY(__LINE__)", persist, noload, keep, address(" STRINGIFY(startAddr) ")" \
            "\n_EZBL_AppReservedRAMRange_" STRINGIFY(__LINE__)":"                                                                           \
            "\n     .space (" STRINGIFY(endAddr) ") - (" STRINGIFY(startAddr) ")"                                                           \
            "\n     .popsection"                                                                                                            \
            )


/**
 * void EZBL_SetAppReservedNearHole(unsigned int nearAddressesReserved);
 *
 * Macro reserves a block of near data space addresses as Application space, 
 * off limits for Bootloader global/static variables or data space constants.
 *
 * This macro is intended for use in EZBL Bootloader projects.
 *
 * Typically, the linker will use RAM starting from the base address of RAM and
 * allocate to higher addresses until all variables have been allocated.
 * However, near RAM locations with address < 0x2000 (8192) have special
 * hardware abilities. This macro generates a specially named keep-out/noload
 * block in near data space so the linker cannot use the addresses, ensuring it
 * stays available for future Application projects.
 *
 * @param nearAddressesReserved unsigned int number of bytes of near data space 
 *                              RAM to reserve for Application projects. This
 *                              must be a compile time constant that the
 *                              assembler understands.
 *
 * @return Generates a named "EZBL_APP_RAM_RESERVE" section in RAM with near,
 *         persistent, noload and keep attributes applied.
 *
 *         ezbl_tools.jar deletes data sections named "EZBL_APP_RAM_RESERVE"
 *         (with the noload attribute) so the range is unoccupied in the
 *         generated [project_name].merge.S file that Applications link 
 *         against.
 */
#define EZBL_SetAppReservedNearHole(nearAddressesReserved)                          \
    __asm__("\n     .pushsection EZBL_APP_RAM_RESERVE, persist, noload, keep, near" \
            "\n_EZBL_AppReservedNearRAM_" STRINGIFY(__LINE__)":"                    \
            "\n     .space (" STRINGIFY(nearAddressesReserved) ")"                  \
            "\n     .popsection"                                                    \
            )


/**
 * Macro void *EZBL_AllocSRAM(token varName, int bytesToAllocate, ...);
 *
 * Statically allocates an object, accepting various attributes and returns a
 * (void*) pointer to it. Typically, this macro is used to allocate
 * static variables or arrays in RAM from another macro that could be called
 * from code.
 *
 * The object is allocated into a section named ".varName.line_num", where
 * varName is the varName parameter, converted to a string, and line_num is the
 * .c source file line number that this macro was called from (not unique valid
 * when called from a .h file).
 *
 * @param varName A C symbol name to give the space that is allocated. This
 *                symbol is not declared in the C environment, so requires an
 *                explicit extern declaration if using C-syntax code to access
 *                the allocated object.
 *
 *                Do not enclose the varName in quotes.
 *
 * @param bytesToAllocate The size, in bytes, to allocate to the object via the
 *                ASM .space directive. This must be a literal constant or
 *                expression that can be evaluated by the assembler (ex: 5,
 *                5*10, _U2MODE+2, etc.). Symbols in the expression can only be
 *                added or subtracted to using literal constants or literal
 *                constant expressions.
 *
 * @param ... ASM section attributes to allocate the object with. Ex: 'bss',
 *            'persistent', 'info', 'code, noload, keep', etc. At least one
 *            section attribute must be provided.
 *
 * @return (void*) pointer to the statically allocated object. If the object is
 *            in a data space which cannot be represented by a 16-bit pointer,
 *            ex: 'code' flash program space, the upper bits of the pointer are
 *            truncated off.
 */
#define EZBL_AllocSRAM(varName, bytesToAllocate, ...)                                                   \
({                                                                                                      \
    register void *i;                                                                                   \
    __asm__ __volatile__ ("\n"                                                                          \
            "\n    .pushsection ." STRINGIFY(varName) "." STRINGIFY(__LINE__) "," STRINGIFY(__VA_ARGS)  \
            "\n    .global _" STRINGIFY(varName)                                                        \
            "\n    .type _" STRINGIFY(varName) ", @object"                                              \
            "\n_" STRINGIFY(varName) ":"                                                                \
            "\n    .space  " STRINGIFY(bytesToAllocate)                                                 \
            "\n    .popsection"                                                                         \
            "\n    mov   #_" STRINGIFY(varName) " & 0xFFFF, %0"                                         \
    : "=r"(i) : : ); i;                                                                                 \
})


/**
 * void EZBL_SetNoProgramRange(long startAddr, long endAddr);
 *
 * Macro defines a program address range [inclusive, exclusive) which will be
 * be disallowed from being programmed by the EZBL_WriteROM() primitive (and all
 * APIs that internally call EZBL_WriteROM()) unless explicitly overridden by
 * the write API parameters. When a no program range is encountered in the input
 * data, the given address(es) will be skipped over.
 *
 * Writing and the EZBL_WriteROM() return value will otherwise occur unchanged.
 *
 * To define multiple no-program ranges, call EZBL_SetNoProgramRange() multiple
 * times. Each instance can be located in separate source files or within the
 * same one.
 *
 * The address range must start (be aligned to) and end on a minimal Flash write
 * size boundary (normally 0x4 addresses on newer dsPIC33E/PIC24E/PIC24F
 * devices, or 0x2 on earlier PIC24F/PIC24H/dsPIC33F devices).
 *
 * NOTE: This macro is only applicable for Bootloader projects or projects that
 * include the ezbl_lib.a code for direct linkage to EZBL_WriteROM(). I.e. you
 * cannot add no-program ranges to the Bootloader from an Application project
 * that uses the Bootloader's EZBL_WriteROM() copy. To add extra run-time
 * no-program ranges for a specific write call, see the EZBL_WriteROMEx() API.
 *
 * The EZBL_AddrInTable() API can be used to test if a given program address is
 * within a No-program range or not. The necessary input parameters can be
 * obtained from:
 *      __builtin_section_begin(".text.EZBLNoProgramRanges")
 *      __builtin_section_size(".text.EZBLNoProgramRanges")
 */
#define EZBL_SetNoProgramRange(startAddr, endAddr)                                        \
    __asm__("\n     .pushsection    .text.EZBLNoProgramRanges, code, keep" EZBL_PAGE_ATTR \
            "\n     " EZBL_PWORD STRINGIFY(startAddr)                                     \
            "\n     " EZBL_PWORD STRINGIFY(endAddr)                                       \
            "\n     .popsection"                                                          \
            )


/**
 * void EZBL_SetNoEraseRange(long startAddr, long endAddr);
 *
 * Macro defines a program address range [inclusive, exclusive) which will be
 * be disallowed from being page erased by the EZBL_EraseAppSpace() API
 * unless explicitly overridden by the API parameters. When a no erase range is
 * encountered in the input data, the given address(es) will be skipped over.
 *
 * To define multiple no-erase ranges, call EZBL_SetNoEraseRange() multiple
 * times. Each instance can be located in separate source files or within the
 * same one.
 *
 * The address range must start (be aligned to) and end on a minimal flash erase
 * page size boundary (normally 0x800 or 0x400 addresses).
 *
 * NOTE: This macro is only applicable for Bootloader projects or projects that
 * include the ezbl_lib.a code for direct linkage to EZBL_EraseAppSpace(). I.e.
 * you cannot add no-erase ranges to the Bootloader from an Application project.
 *
 * The EZBL_AddrInTable() API can be used to test if a given program address is
 * within a No-erase range or not. The necessary input parameters can be
 * obtained from:
 *      __builtin_section_begin(".text.EZBLNoEraseRanges")
 *      __builtin_section_size(".text.EZBLNoEraseRanges")
 */
#define EZBL_SetNoEraseRange(startAddr, endAddr)                                        \
    __asm__("\n     .pushsection    .text.EZBLNoEraseRanges, code, keep" EZBL_PAGE_ATTR \
            "\n     " EZBL_PWORD STRINGIFY(startAddr)                                   \
            "\n     " EZBL_PWORD STRINGIFY(endAddr)                                     \
            "\n     .popsection"                                                        \
            )

/**
 * void EZBL_SetNoVerifyRange(unsigned long startAddr, unsigned long endAddr);
 *
 * Macro defines a program address range [inclusive, exclusive) which will be
 * be ignored during read-back verification post programming by the
 * EZBL_WriteROM() primitive (and all APIs that internally call
 * EZBL_WriteROM()). When a no verify program range is
 * encountered in the input parameters, the given address(es) will be programmed
 * (assuming not excluded as a NoProgramRange) but then skipped when computing
 * success/failure of the write.
 *
 * To define multiple no-verify ranges, call EZBL_SetNoVerifyRange() multiple
 * times. Each instance can be located in separate source files or within the
 * same one.
 *
 * The address range must start (be aligned to) and end on a minimal Flash write
 * size boundary (normally 0x4 addresses on newer dsPIC33E/PIC24E/PIC24F
 * devices, or 0x2 on earlier PIC24F/PIC24H/dsPIC33F devices).
 *
 * At run-time, this table of address pairs can be read using the
 * EZBL_ReadPackedFlash() API together with the addresses/size returned by:
 *      __builtin_section_begin(".text.EZBLNoVerifyRanges")
 *      __builtin_section_end(".text.EZBLNoVerifyRanges")
 *      (__builtin_section_size(".text.EZBLNoVerifyRanges")/2u*3u)
 * Each address is stored as a 3-byte Instruction Word, so unpacking is likely
 * required before use.
 */
#define EZBL_SetNoVerifyRange(startAddr, endAddr)                                           \
    __asm__("\n     .pushsection    .text.EZBLNoVerifyRanges, code, keep" EZBL_PAGE_ATTR    \
            "\n     " EZBL_PWORD STRINGIFY(startAddr)                                       \
            "\n     " EZBL_PWORD STRINGIFY(endAddr)                                         \
            "\n     .popsection"                                                            \
            )


/**
 * Generates data in the ".text.EZBLEraseRestoreTable" section for use by
 * EZBL_RestoreAppErasable(). The data gets auto-rewritten to flash after being
 * erased due to an EZBL_EraseAppSpace() call.
 *
 * @param pgmAddr Program space address where this data resides (ex: 0x004000)
 * @param pwordArrayString  Array of 24-bit integers to store sequentially into
 *                          program space starting at pgmAddr address. This
 *                          value must be passed as a common separated string,
 *                          ex: "0x221100, 0x654321, 3+1, 69^96"
 *
 *                          Any expressions must be resolvable by the assembler
 *                          so cannot be complex C expressions.
 */
#define EZBL_SetEraseRestoreWords(pgmAddr, pwordArrayString)                                \
    __asm__("\n     .pushsection    .text.EZBLEraseRestoreTable, code, keep" EZBL_PAGE_ATTR \
            "\n     " EZBL_PWORD STRINGIFY(pgmAddr)                                         \
            "\n     .ifdef __C30ELF"                                                        \
            "\n     " EZBL_PWORD "(1337f - (. + 0x2))/2*3"                                  \
            "\n     .else"                                                                  \
            "\n     " EZBL_PWORD "(1337f - .)"                                              \
            "\n     .endif"                                                                 \
            "\n     " EZBL_PWORD pwordArrayString                                           \
            "\n1337:"                                                                       \
            "\n     .popsection"                                                            \
            )


#define __DefineMapInit(mapName, ioRegPrefix, bitfieldPrefix, mapLen)                                                           \
    __asm__(                                                                                                                    \
        "\n    .ifndef make_all_reg_bit_equs_defined"                                                                           \
        "\n        .equ    make_all_reg_bit_equs_defined, 1"                                                                    \
        "\n        .macro  make_all_reg_bit_equs regBaseName, portLetter, bitNum"                                               \
        "\n        .equ    EZBL_MAP_REG_&regBaseName&R&portLetter&&bitNum&, &regBaseName&&portLetter&   /* Generates something like .equ EZBL_MAP_REG_LATRC3, LATC */"  \
        "\n        .equ    EZBL_MAP_BIT_&regBaseName&R&portLetter&&bitNum&, &bitNum&                    /* Generates something like .equ EZBL_MAP_BIT_LATRC3, 3 */"     \
        "\n        .endm"                                                                                                       \
        "\n    .endif"                                                                                                          \
        "\n    .pushsection .debug.EZBL_dummy, info"                                                                            \
        "\n    .irp    reg_prefix," STRINGIFY(ioRegPrefix) "/* ex: _PORT (XC16) or PORT (XC32) */"                              \
        "\n        .irp    port_letter,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P"                                                         \
        "\n            .irp    bit_num,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31"   \
        "\n                make_all_reg_bit_equs &reg_prefix&, &port_letter&, &bit_num&"                                        \
        "\n            .endr"                                                                                                   \
        "\n        .endr"                                                                                                       \
        "\n    .endr"                                                                                                           \
        "\n    .popsection"                                                                                                     \
        "\n"                                                                                                                    \
        "\n    .ifdef __C30ELF"                                                                                                 \
        "\n        .pushsection .const, psv, page"                                                                              \
        "\n        .align   2"                                                                                                  \
        "\n    .else"                                                                                                           \
        "\n        .pushsection .rodata"                                                                                        \
        "\n        .align   4"                                                                                                  \
        "\n    .endif"                                                                                                          \
        "\n    .equ    " STRINGIFY_SYM(mapName) "Len, " STRINGIFY(mapLen)                                                       \
        "\n    .global " STRINGIFY_SYM(mapName) "Len"                                                                           \
        "\n    .global " STRINGIFY_SYM(mapName)                                                                                 \
        "\n" STRINGIFY_SYM(mapName) ":"                                                                                         \
        "\n    .word   " STRINGIFY_SYM(mapName) "Len"                                                                           \



#if defined(__XC16__)
#define __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, commonGPIOTargetName)                           \
        "\n    .word   EZBL_MAP_REG_" STRINGIFY(ioRegPrefix) STRINGIFY(commonGPIOTargetName) " + (EZBL_MAP_BIT_" STRINGIFY(ioRegPrefix) STRINGIFY(commonGPIOTargetName) "<<12)"         // Encodes 16-bit word containing mapped elements's SFR address with a 4-bit bitfield offset saved in bits<15:12>; SFR only exist at addresses < 0x1000
#elif defined(__XC32__) || defined(__PIC32__)
#define __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, commonGPIOTargetName)                           \
        "\n    .word   EZBL_MAP_REG_" STRINGIFY(ioRegPrefix) STRINGIFY(commonGPIOTargetName) " + ((EZBL_MAP_BIT_" STRINGIFY(ioRegPrefix) STRINGIFY(commonGPIOTargetName) " & 0x1F)<<17)"  // Encodes 32-bit word containing mapped element's SFR address with a 5-bit bitfield offset saved in bits<21:17> of the address
#endif

#define __DefineMapEnd(mapName)                                                                             \
        "\n    .popsection"                                                                                 \
        "\n    .pushsection .info.EZBL_KeepSYM, info, keep"                                                 \
        "\n    .long   " STRINGIFY_SYM(mapName)                                                             \
        "\n    .popsection"                                                                                 \
    )   // End block of inline asm macro started with __DefineMapInit()


#define __DefineMapItem0(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)
#define __DefineMapItem1(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)                                                                        __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem2(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)     __DefineMapItem1(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__)  __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem3(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)     __DefineMapItem2(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__)  __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem4(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)     __DefineMapItem3(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__)  __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem5(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)     __DefineMapItem4(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__)  __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem6(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)     __DefineMapItem5(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__)  __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem7(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)     __DefineMapItem6(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__)  __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem8(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)     __DefineMapItem7(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__)  __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem9(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)     __DefineMapItem8(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__)  __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem10(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem9(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__)  __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem11(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem10(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem12(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem11(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem13(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem12(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem14(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem13(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem15(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem14(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem16(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem15(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem17(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem16(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem18(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem17(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem19(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem18(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem20(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem19(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem21(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem20(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem22(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem21(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem23(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem22(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem24(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem23(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem25(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem24(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem26(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem25(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem27(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem26(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem28(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem27(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem29(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem28(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem30(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem29(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem31(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem30(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapItem32(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName, ...)    __DefineMapItem31(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__) __DefineMapItem(mapName, ioRegPrefix, regBitPrefix, GPIOCommonName)
#define __DefineMapN(mapName, ioRegPrefix, regBitPrefix, ...)   __DefineMapInit(mapName, ioRegPrefix, regBitPrefix, __VA_COUNT(__VA_ARGS__))                        \
                                                                CAT(__DefineMapItem,__VA_COUNT(__VA_ARGS__))(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__)       \
                                                                __DefineMapEnd(mapName)  /* Internal macro generates an initialization array. See/use EZBL_DefineMap() macro instead. */
#define __DefineWeakMapN(mapName, ioRegPrefix, regBitPrefix, ...)   __DefineMapInit(mapName, ioRegPrefix, regBitPrefix, __VA_COUNT(__VA_ARGS__))                    \
                                                                    "\n    .weak " STRINGIFY_SYM(mapName)                                                           \
                                                                    "\n    .weak " STRINGIFY_SYM(mapName) "Len"                                                     \
                                                                    CAT(__DefineMapItem,__VA_COUNT(__VA_ARGS__))(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__)   \
                                                                    __DefineMapEnd()  /* Internal macro generates an initialization array. See/use EZBL_DefineMap() macro instead. */


/**
 * void EZBL_DefineLEDMap(GPIO_pin_names, ...);
 *
 * Macro generates a compact mapping array for reading and writing an array of
 * up to 16 I/O pins or LEDs at once using the LEDToggle() ezbl_lib.a function.
 * This macro is used to generate a static (i.e. build time) array of
 * arbitrary/unordered GPIO pin names that correspond to ordered bit positions
 * in a bitmask. LATx SFR addresses and bit positions are encoded in the map
 * such that future accesses can read-combine/write-distribute data from the map
 * using loops to save code space (at the expense of speed).
 *
 * Each LED can be mapped to any physical port + bit independently. If an
 * assigned I/O mapping does not have a physical I/O pin associated with it,
 * reads of the bit will return '0' and writes will be ignored.
 *
 * Example usage:
 *      EZBL_DefineLEDMap(RA7, RA6, RA5, RA, RB1, RB2, RB3, RB4);
 *      LEDToggle(0xF);  // Toggle LATB4, LATB3, LATB2, and LATB1 (in sequential order)
 *      unsigned int LEDsOn = LEDToggle(0); // LEDsOn will return | _LATA7<<7 | _LATA6<<6 | _LATA5<<5 | _LATA4<<4;
 *
 * The first value specified (RA7) is the Most Significant bit, while the last
 * value specified is the Least Significant bit (RB4 = mask bit 0).
 *
 * 0 up to 16 mappings can be defined. A 0 length map will always return 0x0000
 * (null) data on read and throw data away on write. The number of LEDs mapped
 * corresponds to the number of parameters passed to the macro. For less than 16
 * mappings, the map bits will be right-justified (i.e. right-most parameter is
 * always bit 0).
 *
 * @param GPIO_pin_names 0 to 16 comma separated GPIO pin names corresponding to
 *                       a bit order for read/write bitmasks using the map. Bit
 *                       names are specified Most Signfiicant bit first,
 *                       down to the least significant bit (last parameter).
 *
 *                       Pin names are non-C tokens, such as RA0, RB6, RC15,
 *                       etc.
 *
 *                       If a mapping is made against a GPIO pin that does not
 *                       exist on the target device, reads will return 0 and
 *                       writes will throw the data away (accesses LATx<bit> SFR
 *                       locations that are R-0).
 *
 * @return void
 *
 *         Internally, 2 bytes of fixed overhead + 2 bytes per mapping will be
 *         stored in the .const section (usually PSV program memory space
 *         consumed).
 */
#define EZBL_DefineLEDMap(...) __DefineMapN(EZBL_LEDMap, LAT, R, __VA_ARGS__)


/**
 * Generates EZBL_LEDMap - a bit scatter/gather map for GPIO LED outputs with
 * respect to LATx registers.
 *
 * See EZBL_DefineLEDMap(). This macro generates the same .const data, but the
 * EZBL_LEDMap (and EZBL_LEDMapLen) symbols are defined weak so they can be
 * overridden in a separate source file.
 */
#define EZBL_DefineWeakLEDMap(...) __DefineWeakMapN(EZBL_LEDMap, LAT, R, __VA_ARGS__)


/**
 * IO mapping array created by the EZBL_DefineLEDMap() macro, referenced to
 * <b>LATx</b> registers.
 *
 * A pointer to this object may be used with the EZBL_MapWrite[Ex](),
 * EZBL_MapClr[Ex](), EZBL_MapSet[Ex](), EZBL_MapInv[Ex]() or other APIs that
 * require a mapping array input.
 *
 * The LEDToggle()/LEDSet()/LEDOn()/LEDOff() ezbl_lib.a functions also reference
 * this map object.
 */
extern const unsigned int EZBL_LEDMap[];


/**
 * void EZBL_DefineButtonMap(GPIO_pin_names, ...);
 *
 * Macro generates the EZBL_ButtonMap[] const mapping array against <b>PORTx</b>
 * SFRs for reading/accessing an array of up to 16 I/O pins as an (unsigned int)
 * integral bitfield, but with physical I/O pins distributed across one or more 
 * actual PORT registers in arbitrary, circuit defined bit order.
 * 
 * The ButtonRead() and ButtonPeek() ezbl_lib.a functions reference the
 * generated EZBL_ButtonMap object.
 *
 * Each Button can be mapped to any physical port + bit independently. If an
 * assigned I/O mapping does not have a physical I/O pin associated with it,
 * reads of the bit will return '0' and writes will be ignored.
 *
 * Example usage:
 *      EZBL_DefineButtonMap(RC0, RA7, RB3, RB4);
 *      EZBL_MapSetEx(0xF, EZBL_ButtonMap, (unsigned int)&TRISA - (unsigned int)&PORTA);
 *      unsigned int buttonsPushed = ButtonRead();  // Returns {0,0,0,0,0,0,0,0,0,0,0,0,!_RC0,!_RA7,!_RB3,!_RB4}
 *
 * The first value specified (RA7) is the Most Significant bit, while the last
 * value specified (RB4) is the Least Significant bit in the integral bitfield
 * form.
 *
 * 0 up to 16 mappings can be defined. A 0 length map will always return 0x0000
 * (null) data on read and throw data away on write. The number of Buttons 
 * mapped corresponds to the number of parameters passed to the macro. For less
 * than 16 mappings, the map bits will be right-justified (i.e. right-most
 * parameter is always bit 0, high order unimplemented bits ).
 *
 * @param GPIO_pin_names 0 to 16 comma separated GPIO pin names corresponding to
 *                       the desired integral bitmask form of the map. Bit names
 *                       are specified Most Signfiicant bit first,
 *                       down to the least significant bit (last parameter).
 *
 *                       Pin names are data sheet tokens, such as RA0, RB6,
 *                       RC15, not actual PORT register bitfield references.
 *
 *                       If a mapping is made against a GPIO pin that does not
 *                       exist on the target device, reads will return 0 and
 *                       writes will throw the data away (accesses PORTx<bit>
 *                       SFR locations that are R-0).
 *
 * @return Array reference to EZBL_ButtonMap[] generate by the call.
 *
 *         Internally, 2 bytes of fixed overhead + 2 bytes per mapping bit will
 *         be stored in the .const section (usually PSV program memory space
 *         consumed).
 */
#define EZBL_DefineButtonMap(...)       __DefineMapN(EZBL_ButtonMap, PORT, R, __VA_ARGS__)


/**
 * Generates EZBL_ButtonMap - a bit scatter/gather map for GPIO button inputs 
 * with respect to PORTx registers.
 * 
 * See EZBL_DefineButtonMap() for full information. This macro generates the
 * same .const data, but the EZBL_ButtonMap (and EZBL_ButtonMapLen) symbols are
 * defined weak so they can be overridden in a separate source file.
 */
#define EZBL_DefineWeakButtonMap(...)   __DefineWeakMapN(EZBL_ButtonMap, PORT, R, __VA_ARGS__)


/**
 * IO mapping array created by the EZBL_DefineButtonMap() macro, referenced to 
 * PORTx registers.
 * 
 * A pointer to this object may be used with the EZBL_MapWrite[Ex](),
 * EZBL_MapClr[Ex](), EZBL_MapSet[Ex](), EZBL_MapInv[Ex]() or other APIs that
 * require a mapping array input.
 */
extern const unsigned int EZBL_ButtonMap[];


/**
 * const unsigned int[] EZBL_DefineMap(mapName, sfrNamePrefix, bitfieldPrefix, bitfieldNames...);
 *
 * Macro generates a const mapping array against a generic template for SFR 
 * register names, SFR bitfield names within the register, and a series of 
 * tokens to concatenate to form an array of SFR address + bitfield offsets.
 * 
 * The generated map is intended to be used for reading/writing several
 * bitfields spread arbitrarily amongst different register addresses and
 * different bitfield offsets as if they were a single unsigned int composed of
 * the concatenated and reordered bitfields.
 *
 * After being defined, the map is manipulated using the EZBL_MapClr[Ex](),
 * EZBL_MapSet[Ex](), EZBL_MapInv[Ex](), EZBL_MapWrite[Ex](), and EZBL_MapMask()
 * APIs. Each write operation takes in one unsigned integer value and
 * distributes the bits into the mapped address/bit locations and returns the
 * value read (just prior to writing) as a combined unsigned integer. To read
 * the map without modifying it, use EZBL_MapClr[Ex](&map, 0) or
 * EZBL_MapInv[Ex](&map, 0).
 * 
 * Example usage:
 *      EZBL_DefineMap(EZBL_LEDMap, LAT, R, RC0, RA7, RB3, RB4);
 *      ...
 *      unsigned int priorLEDState = EZBL_MapSet(0x000D, EZBL_LEDMap);
 * priorLEDState will be written with {0,0,0,0,0,0,0,0,0,0,0,0,_LATC0,_LATA7,_LATB3,_LATB4}<sub>BASE 2</sub>,
 * while the following targets are written as _LATC0 = 1; _LATA7 = 1; _LATB3 = 0; _LATB4 = 1;
 *
 * The first value specified in the map (RC0) is the Most Significant bit, while the last
 * value specified (RB4) is the Least Significant bit in the combined integral
 * form.
 *
 * 0 up to 16 mappings can be defined. A 0 length map will always return 0x0000
 * (null) data on read and throw data away on write. For less than a full
 * unsigned int sized map, the map bits will be right-justified (i.e. right-most
 * parameter is always bit 0, high order will contain unimplemented bits that
 * are ignored on write/read as '0').
 *
 * @param mapName       C usage and linking token of the const unsigned int
 *                      array to be generated. This pointer may be saved in RAM
 *                      for use in other .c/.s/.o/.a source or object files, or
 *                      referenced by the C name as an extern object.
 *
 *                      When using the map in a source file which did not define
 *                      the map (or the same file before the map was defined)
 *                      requires you to first include an extern prototype
 *                      reference, for example:
 *                          extern const unsigned int * mapName;
 *
 * @param sfrNamePrefix Base SFR name that each of the bitfields belong to, not 
 *                      including any variable postfixes, like 'A', 'B', 'C' or 
 *                      '0', '1', 2', etc.
 * @param bitfieldPrefix
 * @param bitfieldNames...  0 to sizeof(unsigned int)*8 comma separated bitfield
 *                          names corresponding to the desired full SFR
 *                          name + bitfield specifier. Bit names are specified
 *                          Most Signfiicant bit first, down to the least
 *                          significant bit (last parameter).
 *
 *                          Pin names are data sheet tokens, such as RA0, RB6,
 *                          RC15, not actual PORT register bitfield references
 *                          when defining maps against
 *                          LATx/TRISx/ANSELx/ODx/CNPUx/CNPDx I/O SFRs.
 *
 *                          If a mapping is made against a SFR bit that does not
 *                          exist on the target device, reads of the bit will
 *                          return the unimplemented value read from the same
 *                          register bit position (typically '0') and writes
 *                          will behave the same as a write to an unimplemented
 *                          or read-only bit position (usually just thrown
 *                          away).
 *
 * @return const unsigned int[] array reference to the just created map
 *
 *         Internally, sizeof(unsigned int)*(1+COUNT(bitfieldNames...)) bytes of
 *         data will be stored in the .const or .rodata section (PSV or flash
 *         space) for use by the EZBL_Map*() APIs.
 *         sizeof(mapName)/sizeof(mapName[0]) will therefore yield 1 more than
 *         the actual number of bits in the map.
 */
#define EZBL_DefineMap(mapName, ioRegPrefix, regBitPrefix, ...)                 \
    ({                                                                          \
        extern const unsigned int mapName[1 + __VA_COUNT(__VA_ARGS__)];         \
        __DefineMapN(mapName, ioRegPrefix, regBitPrefix, __VA_ARGS__);          \
        mapName;                                                                \
    })



/**
 * Returns the (unsigned int) integral bitmask form of the map structure with
 * '1' LSbits indicating the bit is implemented in the map and '0' high order
 * bits indicating the map contains no mapping for the bit position.
 *
 * None of the SFRs targeted by the map are read or written, only the
 * mappingArray contents are checked.
 *
 * @param mappingArray Pointer to a (unsigned int) array. Index 0 must contain
 *                     a count of how many bit mappings follow in the array.
 *                     This is usually generated using the EZBL_DefineMap(),
 *                     EZBL_DefineLEDMap(), EZBL_DefineButtonMap() macros, or
 *                     weak variations of them.
 *
 * @return Bitmask representing the bits within the map that have corresponding
 *         SFR targets defined in the map structure. Unimplemented bits in the
 *         map return '0'.
 *
 *         Mathematically, the return value is:
 *              (1<<mappingArray[0]) - 1
 *
 *         For example, given a map generaed using:
 *              EZBL_DefineLEDMap(RA14, RA13, RC2, RC1, RC0, RD2);
 *         EZBL_MapMask(EZBL_LEDMap) will return ((unsigned int)(0x003F))
 */
static inline unsigned int __attribute__((always_inline, optimize(1))) EZBL_MapMask(const unsigned int *mappingArray)
{
    return (0x0001u<<mappingArray[0]) - 1u;
}


/**
 * Write's '0' values from an integer bitmask into a mapped array of SFR<bit>
 * destinations, leaving any SFR<bit> targets corresponding to '0' bitmask
 * values unchanged.
 *
 * This function internally calls EZBL_MapInv() twice - first with 0x0000 as
 * the toggle value to read the existing state, then computes the needed toggle
 * values to clr all '1' bits specified by the clrBits bitfield.
 *
 * @param clrBits Bitmasked bits that should be written as '0' to the mapping
 *                array, leaving mapping array bits corresponding to '0'
 *                bitfield bits unchanged.
 *
 * @param mappingArray Pointer to a (unsigned int) array. Index 0 must contain
 *                     a count of how many bit mappings follow in the array.
 *
 *                     Index 1 up to 16 shall encode SFR addresses and bit
 *                     positions to map to. Bits <15:12> encode the bit position
 *                     while bits <11:0> encode the SFR target address that the
 *                     bit applies to.
 *
 *                     Index 1 corresponds to bit 0 (LSbit) of the clrBits,
 *                     index 2, is bit 1, etc.
 *
 * @return Bitmasked values read from the mapped SFR<bit> registers before
 *         writing '0's according to clrBits.
 */
unsigned int EZBL_MapClr(unsigned int clrBits, const unsigned int *mappingArray);


/**
 * Write's '0' values from an integer bitmask into a mapped array of SFR<bit>
 * destinations, leaving any SFR<bit> targets corresponding to '0' bitmask
 * values unchanged.
 *
 * This function internally calls EZBL_MapInv() twice - first with 0x0000 as
 * the toggle value to read the existing state, then computes the needed toggle
 * values to clr all '1' bits specified by the clrBits bitfield.
 *
 * @param clrBits Bitmasked bits that should be written as '0' to the mapping
 *                array, leaving mapping array bits corresponding to '0'
 *                bitfield bits unchanged.
 *
 * @param mappingArray Pointer to a (unsigned int) array. Index 0 must contain
 *                     a count of how many bit mappings follow in the array.
 *
 *                     Index 1 up to 16 shall encode SFR addresses and bit
 *                     positions to map to. Bits <15:12> encode the bit position
 *                     while bits <11:0> encode the SFR target address that the
 *                     bit applies to.
 *
 *                     Index 1 corresponds to bit 0 (LSbit) of the clrBits,
 *                     index 2, is bit 1, etc.
 *
 * @param sfrAddrOffset Signed integer to add to each mappingArray SFR address.
 *                      Use this field to access related SFRs for the bitmap at
 *                      a known address offset while storing only one mapping
 *                      array for the hardware.
 *
 *                      For example, if the mappingArray was created against
 *                      LATx registers, and the TRISx register needs to be
 *                      read/toggled, specify sfrAddrOffset as:
 *                          (unsigned int)&TRISA - (unsigned int)&LATA
 *                      Note that TRISA comes before LATA on most device memory
 *                      maps, so the value computed will actually be a negative
 *                      number.
 *
 *                      Ensure the integral (unsigned int) cast is maintained
 *                      when referencing SFR pointers. Subtracting two pointers
 *                      directly yields a scaled quantity relative to the size
 *                      of the pointed to objects, which is not the address
 *                      offset.
 *
 * @return Bitmasked values read from the mapped SFR<bit> registers before
 *         writing '0's according to clrBits.
 */
unsigned int EZBL_MapClrEx(unsigned int clrBits, const unsigned int *mappingArray, int sfrAddrOffset);


/**
 * Write's '1' values from an integer bitmask into a mapped array of SFR<bit>
 * destinations, leaving any SFR<bit> targets corresponding to '0' bitmask
 * values unchanged.
 *
 * This function internally calls EZBL_MapInv() twice - first with 0x0000 as
 * the toggle value to read the existing state, then computes the needed toggle
 * values to set all '1' bits specified by the setBits bitfield.
 *
 * @param setBits Bitmasked bits that should be written as '1' to the mapping
 *                array, leaving mapping array bits corresponding to '0'
 *                bitfield bits unchanged.
 *
 * @param mappingArray Pointer to a (unsigned int) array. Index 0 must contain
 *                     a count of how many bit mappings follow in the array.
 *
 *                     Index 1 up to 16 shall encode SFR addresses and bit
 *                     positions to map to. Bits <15:12> encode the bit position
 *                     while bits <11:0> encode the SFR target address that the
 *                     bit applies to.
 *
 *                     Index 1 corresponds to bit 0 (LSbit) of the setBits,
 *                     index 2, is bit 1, etc.
 *
 * @return Bitmasked values read from the mapped SFR<bit> registers before
 *         writing '1's according to setBits.
 */
unsigned int EZBL_MapSet(unsigned int setBits, const unsigned int *mappingArray);


/**
 * Write's '1' values from an integer bitmask into a mapped array of SFR<bit>
 * destinations, leaving any SFR<bit> targets corresponding to '0' bitmask
 * values unchanged.
 *
 * This function internally calls EZBL_MapInv() twice - first with 0x0000 as
 * the toggle value to read the existing state, then computes the needed toggle
 * values to set all '1' bits specified by the setBits bitfield.
 *
 * @param setBits Bitmasked bits that should be written as '1' to the mapping
 *                array, leaving mapping array bits corresponding to '0'
 *                bitfield bits unchanged.
 *
 * @param mappingArray Pointer to a (unsigned int) array. Index 0 must contain
 *                     a count of how many bit mappings follow in the array.
 *
 *                     Index 1 up to 16 shall encode SFR addresses and bit
 *                     positions to map to. Bits <15:12> encode the bit position
 *                     while bits <11:0> encode the SFR target address that the
 *                     bit applies to.
 *
 *                     Index 1 corresponds to bit 0 (LSbit) of the setBits,
 *                     index 2, is bit 1, etc.
 *
 * @param sfrAddrOffset Signed integer to add to each mappingArray SFR address.
 *                      Use this field to access related SFRs for the bitmap at
 *                      a known address offset while storing only one mapping
 *                      array for the hardware.
 *
 *                      For example, if the mappingArray was created against
 *                      LATx registers, and the TRISx register needs to be
 *                      read/toggled, specify sfrAddrOffset as:
 *                          (unsigned int)&TRISA - (unsigned int)&LATA
 *                      Note that TRISA comes before LATA on most device memory
 *                      maps, so the value computed will actually be a negative
 *                      number.
 *
 *                      Ensure the integral (unsigned int) cast is maintained
 *                      when referencing SFR pointers. Subtracting two pointers
 *                      directly yields a scaled quantity relative to the size
 *                      of the pointed to objects, which is not the address
 *                      offset.
 *
 * @return Bitmasked values read from the mapped SFR<bit> registers before
 *         writing '1's according to setBits.
 */
unsigned int EZBL_MapSetEx(unsigned int setBits, const unsigned int *mappingArray, int sfrAddrOffset);


/**
 * Inverts (toggles) the state of an array of mapped SFR<bit> destinations using
 * a logical XOR of the invertMask provided. I.e. only '1' bits toggle a mapped
 * SFR state. '0' bits have no effect on the corresponding SFR<bit> value.
 *
 * @param invertMask A bitmask representing an array of ordered bits who's state
 *                   should be toggled. Each '1' bit in the bit field toggles
 *                   the corresponding bit in the arbitrary SFR map. Unmapped
 *                   high order bits are ignored.
 *
 *                   Specify a invertMask of 0x0 to read the bit map without
 *                   writing/altering existing state.
 *
 * @param mappingArray Pointer to a (unsigned int) array. Index 0 must contain
 *                     a count of how many bit mappings follow in the array.
 *
 *                     Index 1 up to 16 shall encode SFR addresses and bit
 *                     positions to map to. Bits <15:12> encode the bit position
 *                     while bits <11:0> encode the SFR target address that the
 *                     bit applies to.
 *
 *                     Index 1 corresponds to bit 0 (LSbit) of the invertMask,
 *                     index 2, is bit 1, etc.
 *
 * @return Bitmask representing the mapped SFR bit values read before issuing
 *         any toggle write backs. Higher order bits that are not specified in
 *         the mapping array are returned as '0'.
 */
unsigned int EZBL_MapInv(unsigned int invertMask, const unsigned int *mappingArray);


/**
 * Inverts (toggles) the state of an array of mapped SFR<bit> destinations using
 * a logical XOR of the invertMask provided. I.e. only '1' bits toggle a mapped
 * SFR state. '0' bits have no effect on the corresponding SFR<bit> value.
 *
 * This function is identical to EZBL_MapInv(), but includes an extra
 * sfrAddrOffset parameter for reading/writing to related SFR<bit> destinations
 * located at a fixed address offset from their original SFR address in the map.
 *
 * @param invertMask A bitmask representing an array of ordered bits who's state
 *                   should be toggled. Each '1' bit in the bit field toggles
 *                   the corresponding bit in the arbitrary SFR map. Unmapped
 *                   high order bits are ignored.
 *
 *                   Specify a invertMask of 0x0 to read the bit map without
 *                   writing/altering existing state.
 *
 * @param mappingArray Pointer to a (unsigned int) array. Index 0 must contain
 *                     a count of how many bit mappings follow in the array.
 *
 *                     Index 1 up to 16 shall encode SFR addresses and bit
 *                     positions to map to. Bits <15:12> encode the bit position
 *                     while bits <11:0> encode the SFR target address that the
 *                     bit applies to.
 *
 *                     Index 1 corresponds to bit 0 (LSbit) of the invertMask,
 *                     index 2, is bit 1, etc.
 *
 * @param sfrAddrOffset Signed integer to add to each mappingArray SFR address.
 *                      Use this field to access related SFRs for the bitmap at
 *                      a known address offset while storing only one mapping
 *                      array for the hardware.
 *
 *                      For example, if the mappingArray was created against
 *                      LATx registers, and the TRISx register needs to be
 *                      read/toggled, specify sfrAddrOffset as:
 *                          (unsigned int)&TRISA - (unsigned int)&LATA
 *                      Note that TRISA comes before LATA on most device memory
 *                      maps, so the value computed will actually be a negative
 *                      number.
 *
 *                      Ensure the integral (unsigned int) cast is maintained
 *                      when referencing SFR pointers. Subtracting two pointers
 *                      directly yields a scaled quantity relative to the size
 *                      of the pointed to objects, which is not the address
 *                      offset.
 *
 * @return Bitmask representing the mapped SFR bit values read before issuing
 *         any toggle write backs. Higher order bits that are not specified in
 *         the mapping array are returned as '0'.
 */
unsigned int EZBL_MapInvEx(unsigned int invertMask, const unsigned int *mappingArray, int sfrAddrOffset);


/**
 * Writes bits from a bitfield to an array of mapped SFR/bit-offset
 * destinations.
 *
 * This function internally calls EZBL_MapInv() twice - first with 0x0000 as
 * the toggle value to read the existing state, then computes the needed toggle
 * values to set and clear all bits to match the supplied bitfield.
 *
 * @param writeBits Bitmask representing an array of IO states that should be
 *                  written to the IO mapping array, setting and clearing each
 *                  signal.
 *
 * @param mappingArray Pointer to a (unsigned int) array. Index 0 must contain
 *                     a count of how many bit mappings follow in the array.
 *
 *                     Index 1 up to 16 shall encode SFR addresses and bit
 *                     positions to map to. Bits <15:12> encode the bit position
 *                     while bits <11:0> encode the SFR target address that the
 *                     bit applies to.
 *
 *                     Index 1 corresponds to bit 0 (LSbit) of the bitValues,
 *                     index 2, is bit 1, etc.
 *
 * @return Bitmasked values for the IO map read before writing the new
 *         writeBits.
 */
unsigned int EZBL_MapWrite(unsigned int writeBits, const unsigned int *mappingArray);


/**
 * Writes bits from a bitfield to an array of mapped SFR/bit-offset
 * destinations.
 *
 * This function internally calls EZBL_MapInv() twice - first with 0x0000 as
 * the toggle value to read the existing state, then computes the needed toggle
 * values to set and clear all bits to match the supplied bitfield.
 *
 * @param writeBits Bitmask representing an array of IO states that should be
 *                  written to the IO mapping array, setting and clearing each
 *                  signal.
 *
 * @param mappingArray Pointer to a (unsigned int) array. Index 0 must contain
 *                     a count of how many bit mappings follow in the array.
 *
 *                     Index 1 up to 16 shall encode SFR addresses and bit
 *                     positions to map to. Bits <15:12> encode the bit position
 *                     while bits <11:0> encode the SFR target address that the
 *                     bit applies to.
 *
 *                     Index 1 corresponds to bit 0 (LSbit) of the bitValues,
 *                     index 2, is bit 1, etc.
 *
 * @param sfrAddrOffset Signed integer to add to each mappingArray SFR address.
 *                      Use this field to access related SFRs for the bitmap at
 *                      a known address offset while storing only one mapping
 *                      array for the hardware.
 *
 *                      For example, if the mappingArray was created against
 *                      LATx registers, and the TRISx register needs to be
 *                      read/toggled, specify sfrAddrOffset as:
 *                          (unsigned int)&TRISA - (unsigned int)&LATA
 *                      Note that TRISA comes before LATA on most device memory
 *                      maps, so the value computed will actually be a negative
 *                      number.
 *
 *                      Ensure the integral (unsigned int) cast is maintained
 *                      when referencing SFR pointers. Subtracting two pointers
 *                      directly yields a scaled quantity relative to the size
 *                      of the pointed to objects, which is not the address
 *                      offset.
 *
 * @return Bitmasked values for the IO map read before writing the new
 *         writeBits.
 */
unsigned int EZBL_MapWriteEx(unsigned int writeBits, const unsigned int *mappingArray, int sfrAddrOffset);


#if defined(__PIC24__) || defined(__dsPIC__) || defined(__XC16__)
// Atomically inclusive OR's the bitsToSet into the target l-value (usually an
// SFR). The target must be a 16-bit aligned data object.
#define EZBL_SetBits(target, bitsToSet)     __asm__ __volatile__("ior\t%1, [%0], [%0]" : : "r"(&(target)), "r"(bitsToSet) : "memory", "cc")

// Atomically AND's the bitsToSet into the target l-value (usually an
// SFR). The target must be a 16-bit aligned data object.
#define EZBL_ClrBits(target, bitsToClear)   __asm__ __volatile__("and\t%1, [%0], [%0]" : : "r"(&(target)), "r"(~bitsToClear) : "memory", "cc")

// Atomically XOR's the bitsToSet into the target l-value (usually an
// SFR). The target must be a 16-bit aligned data object.
#define EZBL_InvBits(target, bitsToInvert)  __asm__ __volatile__("xor\t%1, [%0], [%0]" : : "r"(&(target)), "r"(bitsToInvert) : "memory", "cc")

// Reads the target l-value's memory address, computes an XOR quantity which
// will toggle the existing bits to make it match the inserted bits (without
// changing any other bits outside the chosen bitfield), and then
// pseudo-atomically XOR's the computed value into the target l-value (usually
// an SFR). The target must be a 16-bit aligned data object. NOTE: only the
// write operation is atomic. If the initial read-value no longer matches the
// target contents during the XOR write-back, the resulting output value written
// will also be incorrect. However, because the write value is done using an XOR
// operation with all non-insert bitfield bits being unchanged by XORing '0's,
// only the target bitfield can be written incorrectly. Bits outside the
// bitfield will not be corrupted by these operations.
#define EZBL_InsBits(target, insertBitPos, numBits, valToInsert)                \
    __asm__ __volatile__ ("\n     xor  %1, [%0], %1"                            \
                          "\n     xor  %1, [%0], [%0]"                          \
                          : : "r"(&(target)), "r"((((valToInsert)&((1u<<(numBits))-1u))<<(insertBitPos))) : "memory", "cc")

#define EZBL_ATOMIC_ADD(lval, value)    __asm__ __volatile__("add  %1, [%0], [%0]" : : "r"(&(lval)), "r"(value) : "memory", "cc")
#define EZBL_ATOMIC_SUB(lval, value)    __asm__ __volatile__("subr %1, [%0], [%0]" : : "r"(&(lval)), "r"(value) : "memory", "cc")
#define EZBL_ATOMIC_IOR(lval, value)    __asm__ __volatile__("ior  %1, [%0], [%0]" : : "r"(&(lval)), "r"(value) : "memory", "cc")
#define EZBL_ATOMIC_XOR(lval, value)    __asm__ __volatile__("xor  %1, [%0], [%0]" : : "r"(&(lval)), "r"(value) : "memory", "cc")
#define EZBL_ATOMIC_MOV(dest, source)   __asm__ __volatile__("mov  [%1], [%0]    " : : "r"(&(dest)), "r"(&(source)) : "memory")
#elif defined(__PIC32__) || defined(__XC32__)
#define EZBL_ATOMIC_ADD(lval, value)    do                                  \
                                        {                                   \
                                            __builtin_disable_interrupts(); \
                                            lval += value;                  \
                                            __builtin_enable_interrupts();  \
                                        } while(0)

#define EZBL_ATOMIC_SUB(lval, value)    do                                  \
                                        {                                   \
                                            __builtin_disable_interrupts(); \
                                            lval -= value;                  \
                                            __builtin_enable_interrupts();  \
                                        } while(0)
#define EZBL_ATOMIC_MOV(dest, source)   do                                  \
                                        {                                   \
                                            __builtin_disable_interrupts(); \
                                            dest = source;                  \
                                            __builtin_enable_interrupts();  \
                                        } while(0)
#endif


/**
 * Macro allocates the specified number of bytes in a 'persist' attributed RAM
 * section and writes the generated address to a register. This RAM is
 * statically allocated and will never go out of scope. The memory will have
 * unknown initial contents.
 *
 * NOTE: Each instance of this macro allocates RAM, but being a macro, it is not
 * run-time evaluated. You therefore will only get one effective call if you try
 * to use this macro in a loop (not recommended).
 *
 * @param ptr Destination to write a (void*) pointer for the allocated RAM.
 * @param bytesToAllocate Number of bytes of static RAM to allocate. This must
 *                        be a literal (ex: 4) that the assembler can evaluate,
 *                        so most C constructs are not allowed (ex:
 *                        sizeof(unsigned long) cannot be used).
 */
#define EZBL_AllocPersistentRAM(ptr, bytesToAllocate)                           \
__asm__ __volatile__ ("\n"                                                      \
        "\n    .pushsection .pbss." STRINGIFY(ptr) "." STRINGIFY(__LINE__) ", persist" \
        "\n1337:"                                                               \
        "\n    .space  " STRINGIFY(bytesToAllocate)                             \
        "\n    .popsection"                                                     \
        "\n     .ifdef __C30ELF"                                                \
        "\n     mov   #1337b, %0"                                               \
        "\n     .else"                                                          \
        "\n     la  %0, 1337b,"                                                 \
        "\n     .endif"                                                         \
        : "=r"(ptr) : : );


// Structure for multiplexing/demultiplexing 8 bytes of aligned data between an
// unsigned long data type, or an array of 2 unsigned longs, 4 unsigned shorts
// or 8 unsigned chars.
typedef union
{
    unsigned long long  u64;
    unsigned long       u32[2];
    unsigned short      u16[4];
    unsigned char       u8[8];
} EZBL_UNION_64;


// Structure for reading consecutive program space instruction words with
// post-inc indirect addressing and 0x00 byte padded 32-bit return values.
// Use with EZBL_ReadUnpackedInd() API.
typedef struct
{
    unsigned long readPgmAddr;  // Instruction word aligned read start address; Self increments by 0x2 addresses per instruction word read
    unsigned long *dest;        // Aligned RAM pointer to write data to. Null valid only when destLen is <= 4.
    unsigned int  destLen;      // Number of bytes of space at *dest to fill with program word data. A non-multiple of 4 will be processed only up to the last complete multiple of 4.
} EZBL_UNPACKED_READER;


typedef union tagEZBL_APPID_VER        // 8-byte APPID in little-endian comparable/editable formats
{
    unsigned char       appIDVer8[8];  // Byte 0-3 is APPID_VER_BUILD, 4-5 is APPID_VER_MINOR, 6-7 is APPID_VER_MAJOR
    unsigned short      appIDVer16[4]; // Short 0-1 is APPID_VER_BUILD, 2 is APPID_VER_MINOR, 3 is APPID_VER_MAJOR
    unsigned long       appIDVer32[2]; // Long 0 is APPID_VER_BUILD, 1 is APPID_VER_MINOR and APPID_VER_MAJOR (upper half)
    unsigned long long  appIDVer64;    // Full version information in direct comparable format
    struct
    {
        unsigned long   appIDVerBuild;
        unsigned short  appIDVerMinor;
        unsigned short  appIDVerMajor;
    };
    struct
    {
        unsigned long   :32;            // filler for appIDVerBuild, which was already declared above
        unsigned long   appIDVerMajorMinor;
    };
} EZBL_APPID_VER;


// 16-byte BOOTID_HASH in .bl2 files (SHA-256; right-most 128 bits. Left most printed characters are truncated off.)
typedef union
{
    unsigned char       bootIDHash8[16];
    unsigned short      bootIDHash16[8];
    unsigned long       bootIDHash32[4];
    unsigned long long  bootIDHash64[2];
} EZBL_BOOTID_HASH;


extern const EZBL_APPID_VER EZBL_appIDVer;      // Our running Application ID version fields, generated in ezbl_app.mk/ezbl_dual_partition.mk makefile. All fields return zeros if accessed in a single partition Bootloader project where _APPID_VER_BUILD, _APPID_VER_MINOR, _APPID_VER_MAJOR are not passed to the linker during Bootloader compilation.
extern const EZBL_BOOTID_HASH EZBL_bootIDHash;  // Our running Bootliader ID hash, generated in ezbl_book.mk/ezbl_dual_partition.mk makefile. This value can be accessed in either Bootloader or Application projects.

// Structure stored at EZBL_appBootloadState after successful .bl2 programming
// completion (on single partition targets). Contains flags useful for choosing
// if the Application is missing or invalid, what the last programmed .bl2 file
// looked like (checksum), and what the present App version is for checking if
// offered firmware is newer/older/a major upgrade requiring possibly different
// handling.
typedef struct tagEZBL_INSTALLED_FLAGS
{
    EZBL_APPID_VER appVer;                      // 8 bytes, program address offset 0x0 low byte to 0x4 middle byte, inclusive
    struct
    {
        union                                   // 4 bytes, program address offset 0x4 high byte to 0x6 high byte, inclusive
        {
            unsigned long   hash32;             // Set to last 4 bytes of the last programmed .bl2 file (hash field) to identify if a future offered image file is the same as the one presently installed or not
            unsigned short  hash16[2];
            unsigned char   hash8[4];
        };
        union                                   // 4 bytes, program address offset 0x8 low byte to 0xA low byte, inclusive
        {
            unsigned long   CRC32;              // Set to last 4 bytes of the last programmed .bl2 file (CRC32 field) to identify if a future offered image file is the same as the one presently installed or not
            unsigned short  CRC16[2];           // NOTE: This is the CRC32 field, represented as two 16-bit entities, not a different CRC-16 number.
            unsigned char   CRC8[4];
        };
    } id;
    union                                       // 2 bytes, program address offset 0xA middle byte to 0xA high byte, inclusive
    {
        unsigned short appInstalled;            // Erase first and set to EZBL_APP_INSTALLED (0x12CD) last to signal atomic programming operation is complete. App is valid and available to be executed.
        unsigned char appInstalled8[2];
    };
} EZBL_INSTALLED_FLAGS;


typedef union EZBL_COM_ACTIVITY COM_ACTIVITY_FLAGS; // Defined for backwards compatibility - use EZBL_COM_ACTIVITY instead.

// Bitfield structure for signaling communication FIFO state changes, especially
// for code operating in different asynchronous contexts.
typedef union
{
    struct
    {
        unsigned int tx:1;          // One or more bytes moved from the TX software FIFO to the TX hardware (ex: U2TX ISR wrote to U2TXREG)
        unsigned int rx:1;          // One or more bytes moved from RX hardware into the software RX FIFO (ex: _U2RXInterrupt() copied data out of U2RXREG)
        unsigned int softTx:1;      // Zero or more bytes moved into the TX software FIFO using a call to EZBL_FIFOWrite() or a wrapper function. Ex: UART2_TX_FIFO_OnWrite() callback executed.
        unsigned int softRx:1;      // Zero or more bytes peeked or read from the RX software FIFO as a result of a call to EZBL_FIFORead() or EZBL_FIFOPeek(). Ex: UART2_RX_FIFO_OnRead() callback executed.
        unsigned int rxOverflow:1;  // Hardware or software RX FIFO overflow/data lost
        unsigned int framingError:1;// UART RX Framing Error detected
        unsigned int parityError:1; // RX Parity Error detected
        unsigned int bootkeyRx:1;   // Bootloader synchronization/wakeup key detected (normally "MCUPHCME")
        unsigned int :7;
        unsigned int other:1;       // Status bit indicating other (ill-defined) hardware events or software calls have taken place that affect the state of the software TX/RX FIFOs or underlying hardware. Example captured status updates include: FIFO reset/initialized and software arming of auto-baud.
    };
    unsigned int any;           // Integral union form of the tx, rx, softTx, softRx, and other flags for testing/clearing multiple flags at once. Use with the EZBL_COM_ACTIVITY_*_MASK bit-masking constants.
} EZBL_COM_ACTIVITY;


#define EZBL_COM_ACTIVITY_TX_MASK           (1u<<0)                 // Bit 0: One or more bytes moved from the TX software FIFO to the TX hardware (ex: U2TX ISR wrote to U2TXREG)
#define EZBL_COM_ACTIVITY_RX_MASK           (1u<<1)                 // Bit 1: One or more bytes moved from RX hardware into the software RX FIFO (ex: _U2RXInterrupt() copied data out of U2RXREG)
#define EZBL_COM_ACTIVITY_SOFTTX_MASK       (1u<<2)                 // Bit 2: Zero or more bytes moved into the TX software FIFO using a call to EZBL_FIFOWrite() or a wrapper function. Ex: UART2_TX_FIFO_OnWrite() callback executed.
#define EZBL_COM_ACTIVITY_SOFTRX_MASK       (1u<<3)                 // Bit 3: Zero or more bytes peeked or read from the RX software FIFO as a result of a call to EZBL_FIFORead() or EZBL_FIFOPeek(). Ex: UART2_RX_FIFO_OnRead() callback executed.
#define EZBL_COM_ACTIVITY_RXOVERFLOW_MASK   (1u<<4)                 // Bit 4: Hardware or software RX FIFO overflow/data lost
#define EZBL_COM_ACTIVITY_FRAMINGERROR_MASK (1u<<5)                 // Bit 5: UART RX Framing Error detected
#define EZBL_COM_ACTIVITY_PARITYERROR_MASK  (1u<<6)                 // Bit 6: RX Parity Error detected
#define EZBL_COM_ACTIVITY_BOOTKEYRX_MASK    (1u<<7)                 // Bit 7: Bootloader synchronization/wakeup key detected (normally "MCUPHCME")
#define EZBL_COM_ACTIVITY_OTHER_MASK        (1u<<15)                // Bit 15: Status bit indicating other (ill-defined) hardware events or software calls have taken place that affect the state of the software TX/RX FIFOs or underlying hardware. Example captured status updates include: FIFO reset/initialized and software arming of auto-baud.
#define EZBL_COM_ACTIVITY_ANY_MASK          ((unsigned int)(-1))    // All bits: 0xFFFF


typedef struct EZBL_FIFO EZBL_FIFO;     
struct EZBL_FIFO
{
    volatile unsigned int dataCount;    // Number of bytes that exist in the FIFO. i.e. headPtr - tailPtr, adjusted for wraparound and made atomically readable. You must use EZBL_ATOMIC_ADD() and EZBL_ATOMIC_SUB() if this value is to be modified.
    unsigned char *headPtr;             // Pointer to fifoRAM area for writing
    unsigned char *tailPtr;             // Pointer to fifoRAM area for reading
    unsigned int fifoSize;              // Number of bytes allocated to the fifoRAM
    unsigned char *fifoRAM;             // Main FIFO buffer RAM pointer (or const PSV pointer if only doing reading)
    unsigned int (*onWriteCallback)(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO);   // Pointer to a function to be called anytime the FIFO is written (or attempted to be written)
    unsigned int (*onReadCallback)(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *srcFIFO);      // Pointer to a function to be called anytime the FIFO is read (or attempted to be read)
    unsigned int (*flushFunction)(EZBL_FIFO *fifo, unsigned long *startTime, unsigned long *timeout); // Pointer to a function to be called anytime the EZBL_FIFOFlush() function is called
    unsigned int irqNum;                // IRQ Number to associate with this FIFO. This number is used to decode the correct IFSx/IECx/IPCx register/bit(s) to access when calling EZBL_FIFOIntEnable()/EZBL_FIFOIntDisable()/EZBL_FIFOIntClear()/EZBL_FIFOIntRaise()/EZBL_FIFOIntEnableSet()/EZBL_FIFOIntFlagSet()/EZBL_FIFOGetIntEn()/EZBL_FIFOGetIntPri() functions. IRQ numbers are zero-based where 0 indicates the first ordinary peripheral hardware interrupt (i.e. not a trap)
    volatile EZBL_COM_ACTIVITY activity;// Status bit flags capturing various software and hardware state change events like RX interrupt byte, software RX FIFO read, TX interrupt byte, software TX FIFO write, RX bootloader wake key detection, HW/SW FIFO RX overflow and other activity
};  // Circular software FIFO buffer structure for use with EZBL_FIFOReset()/EZBL_FIFOWrite()/EZBL_FIFORead() and related APIs. FIFOs permit high speed non-blocking transfer of ordered data streams across asynchronous timing domains, such as a UART RX ISR and a bootloader consumer, or indirect printf() output to the UART TX ISR.


typedef struct
{
    enum _EZBL_INSTALL_CTX_STATE
    {
        SM_SYNC_INIT = 0,           // Initial state machine entry; begin looking for 'BL2B' file offering
        SM_GET_HEADER,              // Get 48-byte file header {FILE_ID (4 - 'BL2B'), FILE_LEN (4), BOOTID_HASH (16), APPID_VER (8), HMAC-SHA-256 (16)}
        SM_ERASE,                   // Valid header, erase target flash (sequential page erase unless dual partition)
        SM_SET_UP_GET,              // Set up to read an 8-byte DATA_RECORD header {REC_LEN (4) + REC_ADDR (4)}, 36 byte file footer {FILE_HASH (32) + CRC32 (4)}, or variable length REC_DATA data chunk
        SM_GET_DATA,                // Read a chunk from the RX FIFO, placing flow control data in TX FIFO as necessary, updating CRC32 info
        SM_PROG_REC_DATA,           // Program and verify last REC_DATA chunk
        SM_TEST_CHECKSUMS,          // Validate CRC32 and/or hash, then program Application valid indicator if successful
        SM_TERMINATE,               // Send a 0x0000 + Bootload termination status code to the image provider
        SM_IGNORING,                // Passivation state to eat remainder of BL2B file stream without acting on any of the data or attempting to process bytes from the BL2B file as another bootload offering
    } state, getDataNextState;
    EZBL_APPID_VER appVer;          // 8 bytes, program address offset 0x0 low byte to 0x4 middle byte, inclusive
    unsigned long timeout;
    unsigned long lastActivity;     // Timer used to detect communications and/or work-done idle periods and reset back to synchronization
    unsigned long bytesToEOF;
    unsigned long crc32;
    struct
    {
        union
        {
            unsigned long  len;
            unsigned short len16[2];
            unsigned char  len8[4];
        };
        union
        {
            unsigned long  addr;
            unsigned char  addr8[4];
            unsigned short addr16[2];
        };
    } recHdr;
    unsigned short bytesRequested;
    unsigned short getDataBytes;
    short bootCode;                 // One of EZBL_ERROR_SUCCESS, EZBL_ERROR_SUCCESS_VER_GAP, EZBL_ERROR_COM_READ_TIMEOUT, EZBL_ERROR_IMAGE_MALFORMED, EZBL_ERROR_BOOTID_HASH_MISMATCH, EZBL_ERROR_APPID_VER_MISMATCH, EZBL_ERROR_HARD_VERIFY_ERROR, EZBL_ERROR_SOFT_VERIFY_ERROR, EZBL_ERROR_IMAGE_CRC
} EZBL_INSTALL_CTX;


typedef struct NOW_TASK NOW_TASK;
struct NOW_TASK
{
    NOW_TASK *next;                 // Pointer to next NOW_TASK in a linked list. 0 (NULL) for the end of the list. Do not edit this field directly. Use the NOW_EndTask() function to remove this task from the list and NOW_CreateTask()/NOW_CreateRepeatingTask() to place it back into the list (or modify the repeat interval).
    unsigned short timeout[3];      // NOW time (absolute) specifying when the callbackFunction needs to be called next.
    unsigned short interval[3];     // NOW time 47-bit repeat interval to add to the timeout each time timeout is reached. For one-shot timers, this value is zero and bit 47 is also zero. The interval is in positive time form. For repeat timers, bit 47 is always set for quick testing in the ISR handler.
    int (*callbackFunction)(void);  // Callback function to execute when "NOW_48"()'s return value rolls past timeout.
    struct                          // RESERVED STRUCT MEMBER/NOT TESTED. USE AT OWN RISK! CPU Priority Level to execute the callback function at (range 0 to 7). Also includes PC[22:16] if needed.
    {
        unsigned char callbackHigh; // RESERVED STRUCT MEMBER/NOT TESTED. USE AT OWN RISK! CPU Priority Level to execute the callback function at (range 0 to 7). Also includes PC[22:16] if needed.
        struct
        {
            unsigned char SR_C:1;
            unsigned char SR_Z:1;
            unsigned char SR_OV:1;
            unsigned char SR_N:1;
            unsigned char SR_DC:1;
            unsigned char IPL:3;    // RESERVED STRUCT MEMBER/NOT TESTED. USE AT OWN RISK! CPU Priority Level to execute the callback function at (range 0 to 7). Also includes PC[22:16] if needed.
        };
    };
    volatile int returnSignal;      // Callback function's return value
};


#if defined(__XC16__)
/**
 * Macro to convert an (__eds__*), (__prog__*), or (__psv__*) pointer type to a
 * linear 23-bit physical Flash program address of type unsigned long. The
 * output address will most often be useful when writing to NVMADRL:NVMADRH, or
 * when attempting to do Flash table reads using TBLPAG + tblwtl/tblwth opcodes.
 *
 * EDS pointers pointing to RAM will not generate a warning, but cannot be
 * converted to a program space address.
 *
 * NOTE: EDS/PSV pointers can only address program addresses up to and including
 * 0x7FFFFE.
 *
 * On devices which do not implement EDS addressing, this macro should not be
 * used as PSV const addresses do not carry the upper PSVPAG address bits, which
 * the compiler fixes at link time and assumes is left unchanged during
 * execution.
 *
 * @param edsPgmAddr    (__eds__*), (__prog__*), or (__psv__*) pointer value set
 *                      to point to a Flash address. The address can either be
 *                      in the 0x2xxxxxx range for the lower 16-bits of the
 *                      program words, or the 0x3xxxxxx range for the upper
 *                      8-bits of program words.
 *
 * @return  32-bit unsigned long linear Program Flash address. The possible
 *          output number will always lie within the range of 0x000000 to
 *          0x7FFFFF.
 *
 *          Although program addresses always have bit 0 clear because each
 *          program space address increments by 0x2, this macro will preserve
 *          the bit 0 value from the original edsAddr pointer. These odd
 *          addresses can be used to virtually address the middle 8-bits of each
 *          24-bit program word when using char sized eds pointers.
 */
static inline unsigned long __attribute__((always_inline, optimize(1))) EZBL_EDSToLongPGM(__eds__ void *edsPgmAddr)
{
    volatile union
    {
        __eds__ void *edsPtr;
        unsigned long longAddr;
    } convert;
    convert.edsPtr = edsPgmAddr;

    return ((((unsigned int)(unsigned long)(convert.longAddr)) & 0x7FFFu) | ((((unsigned long)(convert.longAddr))>>1) & 0x07F8000u));
}
#elif defined(__XC32__) || defined(__PIC32__)
#define EZBL_EDSToLongPGM(edsPgmAddr)   ((unsigned long)(edsPgmAddr))
#endif


#if defined(__XC16__)
/**
 * Convert a linear program space address to an (__eds__ void*) pointer type
 * pointing to the same program address. The output pointer is set to address
 * the lower 16-bits of each program word.
 *
 * NOTE: EDS/PSV pointers can only address program addresses up to and including
 * 0x7FFFFF. Program spaces addresses 0x800000 and above do not have a
 * (__prog__*) equivalent address and must be accessed exclusively using table
 * read/write instructions (w/TBLPAG SFR).
 *
 * @param pgmAddr  Linear unsigned long type program space address to generate
 *                 an EDS pointer to. This address must be 0x7FFFFF or less.
 *
 * @return  (__eds__ void*) pointer pointing to the lower 16-bit PSV view of
 *          program space.
 */
static inline __eds__ void * __attribute__((always_inline, optimize(1))) EZBL_LongPGMToEDS(unsigned long pgmAddr)
{
    return (__eds__ void*)(((unsigned int)pgmAddr) | ((pgmAddr & 0x7F8000u)<<1) | 0x2008000ul);
}
#elif defined(__XC32__) || defined(__PIC32__)
#define EZBL_LongPGMToEDS(pgmAddr)  (pgmAddr)
#endif


/**
 * Macro returns the seconds portion of the PC system clock when the source file
 * was last pre-processed/compiled.
 *
 * @return unsigned char between 0 and 59.
 */
static inline unsigned char __attribute__((always_inline, optimize(1))) EZBL_CompileSecond(void)
{
    return (unsigned char)((__TIME__[6]-'0')*10 + (__TIME__[7]-'0'));
}


/**
 * Macro returns the minutes portion of the PC system clock when the source file
 * was last pre-processed/compiled.
 *
 * @return unsigned char between 0 and 59.
 */
static inline unsigned char __attribute__((always_inline, optimize(1))) EZBL_CompileMinute(void)
{
    return (unsigned char)((__TIME__[3]-'0')*10 + (__TIME__[4]-'0'));
}


/**
 * Macro returns the hour portion of the PC system clock when the source file
 * was last pre-processed/compiled.
 *
 * @return unsigned char between 0 and 23 (military time format). 0 represents
 *         12:00 AM, 1 = 1:00 AM, 12 = 12:00 PM, 13 = 1:00 PM, etc.
 */
static inline unsigned char __attribute__((always_inline, optimize(1))) EZBL_CompileHour(void)
{
    return (unsigned char)((__TIME__[0]-'0')*10 + (__TIME__[1]-'0'));
}


/**
 * Macro returns the month portion of the PC system clock when the source file
 * was last pre-processed/compiled.
 *
 * @return unsigned char between 1 and 12. 1 = January, 2 = February, etc.
 */
static inline unsigned char __attribute__((always_inline, optimize(1))) EZBL_CompileMonth(void)
{
    return  (__builtin_strncmp(__DATE__, "Jan", 3) == 0) ? 1 :
            (__builtin_strncmp(__DATE__, "Feb", 3) == 0) ? 2 :
            (__builtin_strncmp(__DATE__, "Mar", 3) == 0) ? 3 :
            (__builtin_strncmp(__DATE__, "Apr", 3) == 0) ? 4 :
            (__builtin_strncmp(__DATE__, "May", 3) == 0) ? 5 :
            (__builtin_strncmp(__DATE__, "Jun", 3) == 0) ? 6 :
            (__builtin_strncmp(__DATE__, "Jul", 3) == 0) ? 7 :
            (__builtin_strncmp(__DATE__, "Aug", 3) == 0) ? 8 :
            (__builtin_strncmp(__DATE__, "Sep", 3) == 0) ? 9 :
            (__builtin_strncmp(__DATE__, "Oct", 3) == 0) ? 10 :
            (__builtin_strncmp(__DATE__, "Nov", 3) == 0) ? 11 :
            (__builtin_strncmp(__DATE__, "Dec", 3) == 0) ? 12 : 0;
}


/**
 * Macro returns the day of month portion of the PC system clock when the source
 * file was last pre-processed/compiled.
 *
 * @return unsigned char between 1 and 31. 1 is the first day of the month.
 */
static inline unsigned char __attribute__((always_inline, optimize(1))) EZBL_CompileDay(void)
{
    return (unsigned char)((__DATE__[4] == ' ') ? (__DATE__[5]-'0') :
                                                  ((__DATE__[4]-'0')*10 + (__DATE__[5]-'0')));
}


/**
 * Macro returns the year portion of the PC system clock when the source file
 * was last pre-processed/compiled.
 *
 * @return unsigned int in 4-digit year format, such as 2016 (not 16).
 */
static inline unsigned int __attribute__((always_inline, optimize(1))) EZBL_CompileYear(void)
{
    return ((unsigned int)(__DATE__[7]-'0'))*1000u + ((unsigned int)(__DATE__[8]-'0'))*100u + ((unsigned int)(__DATE__[9]-'0'))*10u + ((unsigned int)(__DATE__[10]-'0'));
}

#if defined(__XC16__)
#if !defined(_RPOUT_U1TX) && (__XC16_VERSION__ < 1027)
/*
 * PPS Remapping Pin Names
 */
#define _RPOUT_U1TX 3
#define _RPOUT_U1RTS 4
#define _RPOUT_U1BCLK 4
#define _RPOUT_U2TX 5
#define _RPOUT_U2RTS 6
#define _RPOUT_U2BCLK 6
#define _RPOUT_SDO1 7
#define _RPOUT_SCK1OUT 8
#define _RPOUT_SS1OUT 9
#define _RPOUT_SDO2 10
#define _RPOUT_SCK2OUT 11
#define _RPOUT_SS2OUT 12
#define _RPOUT_U3TX 19
#define _RPOUT_U3RTS 20
#define _RPOUT_U3BCLK 20
#define _RPOUT_U4TX 21
#define _RPOUT_U4RTS 22
#define _RPOUT_U4BCLK 22
#define _RPOUT_SDO3 23
#define _RPOUT_SCK3OUT 24
#define _RPOUT_SS3OUT 25
#endif
#endif


#if defined(__XC16__)
/**
 * Macro resolves a PPS RPx pin number to its associated RPORx register and
 * internal RPxR bitfield in a manner that is compatible with XC16 and XC32
 * processor header files.
 *
 * @param RP_PIN Numerical expression of the RPx pin number to access. Ex: <code>5</code> for
 *               pin <code>RP5</code>.
 *
 * @return unsigned char or unsigned bitfield l-value of a RPORx register
 *         location that you can read or assign _RPOUT_* values to.
 *
 * @example
 *         An example calling sequence is:
 *         <code>RPOUT(5) = _RPOUT_U2TX; // Assign U2TX function to RP5</code>
 *
 *         This sequence is functionally equivalent to writing:
 *         <code>RPOR1bits.RPOR5R = 1;  // Assign RP5 to magic output function 1</code>
 *         or for XC16 targets:
 *         <code>_RPOR5R = 1;</code>
 */
#define RPOUT(RP_PIN)       CAT(_RP,RP_PIN,R)


#elif defined(__XC32__) || defined(__PIC32__)
/**
 * Macro resolves a PPS RPx pin number to its associated RPORx register and
 * internal RPxR bitfield in a manner that is compatible with XC16 and XC32
 * processor header files.
 *
 * @param RP_PIN Numerical expression of the RPx pin number to access. Ex: <code>5</code> for
 *               pin <code>RP5</code>.
 *
 * @return unsigned char or unsigned bitfield l-value of a RPORx register
 *         location that you can read or assign _RPOUT_* values to.
 *
 * @example
 *         An example calling sequence is:
 *         <code>RPOUT(5) = _RPOUT_U2TX; // Assign U2TX function to RP5</code>
 *
 *         This sequence is functionally equivalent to writing:
 *         <code>RPOR1bits.RPOR5R = 1;  // Assign RP5 to magic output function 1</code>
 *         or for XC16 targets:
 *         <code>_RPOR5R = 1;</code>
 */
#define RPOUT(RP_PIN)       (((unsigned char *)&((&RPOR0)[(((RP_PIN)-1u)/4u)*4u]))[((RP_PIN)-1u)%4u])

// PPS Output Function Number macros for RPORx registers on PIC32MM0064GPL036 Family devices
#if defined(__PIC32MM) && (__PIC32_FEATURE_SET0 == 'G') && (__PIC32_FEATURE_SET1 == 'P') && (__PIC32_PRODUCT_GROUP == 'L')   // PIC32MM0064GPL036 Family devices
#ifndef _RPOUT_NONE
#define _RPOUT_NONE     0
#endif
#ifndef _RPOUT_U2TX
#define _RPOUT_U2TX     1
#endif
#ifndef _RPOUT_U2RTS
#define _RPOUT_U2RTS    2
#endif
#ifndef _RPOUT_SDO2
#define _RPOUT_SDO2     3
#endif
#ifndef _RPOUT_SCK2OUT
#define _RPOUT_SCK2OUT  4
#endif
#ifndef _RPOUT_SS2OUT
#define _RPOUT_SS2OUT   5
#endif
#ifndef _RPOUT_OCM2
#define _RPOUT_OCM2     6
#endif
#ifndef _RPOUT_OCM3
#define _RPOUT_OCM3     7
#endif
#ifndef _RPOUT_CLC1OUT
#define _RPOUT_CLC1OUT  8
#endif
#ifndef _RPOUT_CLC2OUT
#define _RPOUT_CLC2OUT  9
#endif
#elif defined(__PIC32MM) && (__PIC32_FEATURE_SET0 == 'G') && (__PIC32_FEATURE_SET1 == 'P') && (__PIC32_PRODUCT_GROUP == 'M')   // PIC32MM0256GPM064 Family devices
#ifndef _RPOUT_NONE
#define _RPOUT_NONE     0
#endif
#ifndef _RPOUT_C1OUT
#define _RPOUT_C1OUT    1
#endif
#ifndef _RPOUT_C2OUT
#define _RPOUT_C2OUT    2
#endif
#ifndef _RPOUT_C3OUT
#define _RPOUT_C3OUT    3
#endif
#ifndef _RPOUT_U2TX
#define _RPOUT_U2TX     4
#endif
#ifndef _RPOUT_U2RTS
#define _RPOUT_U2RTS    5
#endif
#ifndef _RPOUT_U3TX
#define _RPOUT_U3TX     6
#endif
#ifndef _RPOUT_U3RTS
#define _RPOUT_U3RTS    7
#endif
#ifndef _RPOUT_SDO2
#define _RPOUT_SDO2     8
#endif
#ifndef _RPOUT_SCK2OUT
#define _RPOUT_SCK2OUT  9
#endif
#ifndef _RPOUT_SS2OUT
#define _RPOUT_SS2OUT   10
#endif
#ifndef _RPOUT_OCM4
#define _RPOUT_OCM4     11
#endif
#ifndef _RPOUT_OCM5
#define _RPOUT_OCM5     12
#endif
#ifndef _RPOUT_OCM6
#define _RPOUT_OCM6     13
#endif
#ifndef _RPOUT_OCM7
#define _RPOUT_OCM7     14
#endif
#ifndef _RPOUT_OCM8
#define _RPOUT_OCM8     15
#endif
#ifndef _RPOUT_OCM9
#define _RPOUT_OCM9     16
#endif
#ifndef _RPOUT_CLC1OUT
#define _RPOUT_CLC1OUT  17
#endif
#ifndef _RPOUT_CLC2OUT
#define _RPOUT_CLC2OUT  18
#endif
#ifndef _RPOUT_CLC3OUT
#define _RPOUT_CLC3OUT  19
#endif
#ifndef _RPOUT_CLC4OUT
#define _RPOUT_CLC4OUT  20
#endif

#endif

#endif


#if defined(__XC16__)
/**
 * void __attribute__((noreturn)) EZBL_ResetCPU(void);
 *
 * Issues the CPU 'reset' opcode or device function. All peripheral and
 * processor state is lost (excluding bits in the RCON register).
 *
 * @return Does not return. The noreturn attribute can help clue the compiler
 *         into optimizing subsequent dead code away (such as the ulnk and
 *         return instruction that is nearly always present for a C function).
 */
static inline void __attribute__((always_inline, noreturn)) EZBL_ResetCPU(void)
{
    __asm__ __volatile__("reset" : : : "memory");   // "memory" clobbers value ensures no-reordering by instruction scheduling compiler optimization
    __builtin_unreachable();
}
#elif defined(__XC32__) || defined(__PIC32__)
static void __attribute__((weakref("__pic32_software_reset"), far, noreturn)) EZBL_ResetCPU(void);
//static void inline __attribute__((always_inline, noreturn, optimize(1))) EZBL_ResetCPU(void)
//{
//    __builtin_disable_interrupts();
//    SYSKEY = 0x00000000;            // Put unlock decoder in known locked starting state
//    SYSKEY = 0xAA996655;
//    SYSKEY = 0x556699AA;
//    RSWRSTSET = 0x00000001;         // Set SWRST = 1 to arm software reset
//    RSWRSTSET = RSWRST;             // Do a read of RSWRST to trigger the reset
//}
#endif


#if defined(__XC16__)
/**
 * Restarts device execution by branching to address 0x000000.
 *
 * This differs from the EZBL_ResetCPU() function by not issuing a physical
 * device reset that would clear SFRs and peripheral state. However, any RAM
 * variables and the stack/heap will be reinitialized by the C Run-Time (CRT) as
 * if a device reset took place -- effectively restarting most code and
 * typically launching a Bootloader.
 *
 * @return This function never returns.
 */
static inline void __attribute__((always_inline, noreturn)) EZBL_PseudoReset(void)
{
    void (*nullFuncPtr)(void) = 0;
    nullFuncPtr();
    __builtin_unreachable();
}
#elif defined(__XC32__) || defined(__PIC32__)
/**
 * Restarts device execution by branching to address 0xBFC00000.
 *
 * This differs from the EZBL_ResetCPU() function by not issuing a physical
 * device reset that would clear SFRs and peripheral state. However, any RAM
 * variables and the stack/heap will be reinitialized by the C Run-Time (CRT) as
 * if a device reset took place -- effectively restarting most code and
 * typically launching a Bootloader.
 *
 * @return This function never returns.
 */
static void inline __attribute__((always_inline, optimize(1), noreturn)) EZBL_PseudoReset(void)
{
    void __attribute__((noreturn, far)) _RESET_ADDR(void);
    _RESET_ADDR();
    __builtin_unreachable();
}
#endif


#if defined(__XC16__)
/**
 * void EZBL_RAMCopy(void *dest, const void *source, unsigned int length);
 *
 * Copies RAM efficiently, much like memcpy() but with run-time auto-detect of
 * even aligned source and destination pointers for switch over to word-wise
 * higher speed copying.
 *
 * Copy performance should be (14 + 1/Byte) instruction cycles for byte mode on
 * dsPIC33E/PIC24E devices, including call and return time. For aligned
 * pointers, performance should be (~22 + 0.5/Byte) instruction cycles. Exact
 * performance may differ if an inline __builtin_memcpy() repeat mov loop is
 * generated instead.
 *
 * @param *dest RAM/SFR pointer to write the copied data to. Can be byte
 *              aligned, which will force a byte-wise copy.
 *
 *              This pointer cannot be NULL (0x0000) except when <i>length</i>
 *              is 0.
 *
 *              This pointer may overlap with the *source pointer memory range,
 *              in which case, the copy operation will proceed starting at the
 *              first dest/source byte (or word!) and proceed to the end with
 *              each byte/word being written before the next is read.
 *
 * @param *source SFR/RAM or PSV/Flash pointer to read data from. Can be byte
 *                aligned, which will force a byte-wise copy.
 *
 *                This pointer cannot be NULL (0x0000) except when <i>length</i>
 *                is 0.
 *
 * @param length Number of bytes to copy.
 *
 *               This value must be <= 16384 on dsPIC30, PIC24F, PIC24H, or
 *               dsPIC33F families (or <= 32768 when dest and source are both
 *               even aligned). 0 is allowed, resulting in no operation.
 *
 *               When the dest and source pointers are both even/word aligned
 *               length still represents a total byte count even though the copy
 *               will proceed using word moves. If length is not a word count
 *               integral, the final element will be moved with a byte
 *               operation.
 *
 *               If this value is a compile-time constant, the EZBL_RAMCopy()
 *               call is substituted with a __builtin_memcpy() function call,
 *               which normally evaluates to a repeat loop with a mov.b
 *               operation.
 *
 * @return *dest pointer, incremented by length bytes. The increment is still in
 *         bytes, even if a faster word move operation was executed.
 */
void EZBL_RAMCopy(void *dest, const void *source, unsigned int length);
#define EZBL_RAMCopy(dest, source, length)  (__builtin_constant_p(length) ? __builtin_memcpy(dest, source, length) : EZBL_RAMCopy(dest, source, length))


#elif defined(__XC32__) || defined(__PIC32MM__)
/**
 * void * EZBL_RAMCopy(void *dest, const void *source, unsigned int length);
 *
 * Copies RAM efficiently, much like memcpy() but with run-time auto-detect of
 * even aligned source and destination pointers for switch over to word-wise
 * higher speed copying.
 *
 * @param *dest RAM/SFR pointer to write the copied data to. Can be byte
 *              aligned, which will force a byte-wise copy.
 *
 *              This pointer cannot be null except when <i>length</i> is 0.
 *
 *              This pointer may overlap with the *source pointer memory range,
 *              in which case, the copy operation will proceed starting at the
 *              first dest/source byte (or word!) and proceed to the end with
 *              each byte/word being written before the next is read.
 *
 * @param *source SFR/RAM or PSV/Flash pointer to read data from. Can be byte
 *                aligned, which will force a byte-wise copy.
 *
 *                This pointer cannot be null except when <i>length</i> is 0.
 *
 * @param length Number of bytes to copy. 0 is allowed, resulting in no
 *               operation.
 *
 *               When the dest and source pointers are both even/word aligned
 *               length still represents a total byte count even though the copy
 *               will proceed using word moves. If length is not a word count
 *               integral, the final element(s) will be moved with a byte
 *               operation.
 *
 * @return *dest pointer, unchanged.
 */
void * EZBL_RAMCopy(void *dest, const void *source, unsigned int length);
#endif


/**
 * Same as EZBL_RAMCopy(), but returns the *dest pointer incremented by length
 * bytes (instead of void) for use when concatenating objects.
 *
 * @param *dest RAM/SFR pointer to write the copied data to. Can be byte
 *              aligned, which will force a byte-wise copy.
 *
 *              This pointer cannot be NULL (0x0000) except when <i>length</i>
 *              is 0.
 *
 *              This pointer may overlap with the *source pointer memory range,
 *              in which case, the copy operation will proceed starting at the
 *              first dest/source byte (or word!) and proceed to the end with
 *              each byte/word being written before the next is read.
 *
 * @param *source SFR/RAM or PSV/Flash pointer to read data from. Can be byte
 *                aligned, which will force a byte-wise copy.
 *
 *                This pointer cannot be NULL (0x0000) except when <i>length</i>
 *                is 0.
 *
 * @param length Number of bytes to copy. 0 is allowed, resulting in no
 *               operation.
 *
 *               When the dest and source pointers are both even/word aligned
 *               length still represents a total byte count even though the copy
 *               will proceed using word moves. If length is not a word count
 *               integral, the final element will be moved with a byte
 *               operation.
 *
 * @return *dest pointer, incremented by length bytes. The increment is still in
 *         bytes, even if a faster word move operation was executed.
 */
void * EZBL_RAMCopyEx(void *dest, const void *source, unsigned int length);


/**
 * void EZBL_RAMSet(void *dest, int value, unsigned int length);
 *
 * Sets RAM efficiently, just like memset() but without dependence on a C
 * library that could go in application space or get optimized to inline code.
 * On the dsPIC30, PIC24F, PIC24H and dsPIC33F device families, 'length' is
 * limited to 14-bits + 1, or in other words, must be in the set of: {0..16,384}.
 *
 * @param *dest RAM pointer to write the repeating byte value to. Address can be
 *              byte aligned. This pointer cannot be NULL (0x0000) except when
 *              <i>length</i> is 0.
 * @param value 8-bit character to write to *dest. Upper bits are ignored.
 * @param length Number of bytes to write. This value must be <= 16384 on
 *               dsPIC30, PIC24F, PIC24H, or dsPIC33F families. 0 is allowed,
 *               resulting in no operation.
 */
void EZBL_RAMSet(void *dest, int value, unsigned int length);
#if defined(__XC16__)
#define EZBL_RAMSet(dest, value, length)  /* macro for inline compiler memset() when length is constant */    \
    (__builtin_constant_p(length) ? __builtin_memset(dest, value, length) : EZBL_RAMSet(dest, value, length))
#endif


/**
 * int EZBL_RAMCompare(void *source1, void *source2, unsigned int length);
 *
 * Compares <i>length</i> bytes from <i>*source1</i> with <i>*source2</i>. On
 * exact match 0 is returned. If different, 0x1 is returned.
 *
 * @param *source1 RAM or PSV pointer to data to compare against. Address can be
 *                 byte aligned. This pointer cannot be NULL (0x0000) except
 *                 when <i>length</i> is 0.
 * @param *source2 RAM or PSV pointer to data to compare. Address can be
 *                 byte aligned. This pointer cannot be NULL (0x0000) except
 *                 when <i>length</i> is 0.
 * @param length Number of bytes to compare. Unlike EZBL_RAMCopy() and
 *               EZBL_RAMSet(), length can be any representable value on any
 *               device. 0 is allowed, resulting in no operation.
 */
int EZBL_RAMCompare(const void *source1, const void *source2, unsigned int length);


/**
 * Byte-wise compares Flash/program memory to a data array, returning the first
 * difference encountered. This function is similar to the memcmp() C stdlib
 * function (and EZBL_RAMCompare()), but takes a 23-bit Program memory
 * address/symbol value as one of the input arguments instead of two RAM
 * pointers.
 *
 * @param pgmAddress
 *      Unsigned long program memory address for the first array to read from.
 *      This must be an even aligned value. The value 0x000000 means program
 *      Flash address 0x000000 and is not a null value. Data is read from Flash
 *      in packed form (3-bytes per 0x2 addresses and not a dummy zero in every
 *      4th byte location).
 *
 *      No address checking is done to confirm validity of the program address.
 *      Attempting to read from an unimplemented Flash address will normally
 *      result in a Address Error Trap.
 *
 * @param *cmpData Pointer to a byte array, located in RAM or data space
 *                 (including PSV/EDS) to compare against.
 *
 * @param length Maximum number of bytes to compare before returning.
 *
 * @return 0x0000 if both arrays have matching contents. A non-zero value is
 *         returned if a mismatch is detected.
 *
 *         If <i>length</i> is zero, a 0x0000 matching return value is always
 *         returned without reading from either memory.
 */
int EZBL_ROMCompare(unsigned long pgmAddress, const void *cmpData, unsigned int length);


typedef struct tagEZBL_MISMATCH_STATUS
{
    unsigned long pgmAddress;   // Program space address starting the mismatch; aligned to instruction word boundaries.
    void *dataAddress;          // Pointer to data space corresponding to the program space address that mismatched. The value returned will always point to the low byte of the 24-bit instruction word that triggered comparison mismatch, even if the true byte that mismatched is in the middle or upper byte locations of the program word.
    unsigned long pgmData;      // 24-bit instruction word that was read from Flash/program space that caused the mismatch. Bits 24-31 of this value are always 0x00.
    unsigned long compareData;  // 24-bits (or less) of RAM data that was read from the *cmpData pointer corresponding to the mismatch. When less than 24-bits of compare data were provided, the middle/upper bytes of compare data is returned as 0xFF padding bytes. Bits 24-31 of this value are always 0x00.
} EZBL_MISMATCH_STATUS;


typedef enum tagEZBL_COMPARE_FLAGS
{
    EZBL_FLAG_EQUALS = 0x0000,          // Evaluate binary equivalence where 1's must equal 1's and 0's must equal 0's.
    EZBL_FLAG_ZEROS = 0x0001,           // Evaluate equivalence only considering 0's in RAM must be 0's in program space. 1's in RAM are don't care positions in Flash.
    EZBL_FLAG_ONES  = 0x0002,           // Evaluate equivalence only considering 1's in RAM must be 1's in program space. 0's in RAM are don't care positions in Flash.
    EZBL_FLAG_16BIT = 0x0004,           // Evaluate equivalence only considering the lower 16-bits of each 24-bit instruction word. Mismatches in the high byte are read, but ignored.
    EZBL_FLAG_8BIT  = 0x0008,           // Evaluate equivalence only considering the lower 8-bits of each 24-bit instruction word. Mismatches in the high bytes are read, but ignored.
    EZBL_FLAG_IPARTITION = 0x1000,
    EZBL_FLAG_FIXED_PARTITION = 0x2000
} EZBL_COMPARE_FLAGS;


/**
 * Byte-wise compares Flash/program memory to a data array, returning the first
 * difference encountered. This function is similar to the memcmp() C stdlib
 * function (and EZBL_RAMCompare()), but takes a 23-bit Program memory
 * address/symbol value as one of the input arguments instead of two RAM
 * pointers.
 *
 * @param pgmAddress
 *      Unsigned long program memory address for the first array to read from.
 *      This must be an even aligned value. The value 0x000000 means program
 *      Flash address 0x000000 and is not a null value. Data is read from Flash
 *      in packed form (3-bytes per 0x2 addresses and not a dummy zero in every
 *      4th byte location).
 *
 *      No address checking is done to confirm validity of the program address.
 *      Attempting to read from an unimplemented Flash address will normally
 *      result in a Address Error Trap.
 *
 * @param *cmpData Pointer to a byte array, located in RAM or data space
 *                 (including PSV/EDS) to compare against.
 *
 * @param length Maximum number of bytes to compare before returning.
 *
 * @param flags A bitwise OR of the following flag constants:
 *    <ul>
 *      <li>EZBL_FLAG_EQUALS (0x0000): Compare for exact binary equality. All
 *                  bits must match.
 *                  <br>Logically, this tests cmpData - pgmData == 0.</li>
 *      <li>EZBL_FLAG_ZEROS (0x0001): Compare cleared bits only. All '0' bits in
                    *cmpData must have a '0' bit in the corresponding program
 *                  space cell, whereas all '1' bits in *cmpData are don't care
 *                  bits in Flash and can read back as either '1' or '0' and
 *                  still be a match.
 *                  <br>Logically, this tests, (~cmpData) & pgmData == 0.</li>
 *      <li>EZBL_FLAG_ONES (0x0002): Compare set bits only. All '1' bits in
                    *cmpData must have a '1' bit in the corresponding program
 *                  space cell, whereas all '0' bits in the cmpData array are
 *                  don't care values in the Flash.
 *                  <br>Logically, this tests, (~cmpData) | pgmData == 0.</li>
 *      <li>EZBL_FLAG_16BIT (0x0004): Check only lower 16-bits of each
 *                  instruction word. Bits 16-23 of each program word are don't
 *                  care values on read-back, but there must still be dummy
 *                  equivalent cmdData locations for them. The lower 16-bits are
 *                  tested according to the binary EQUALS/ZEROS/ONES matching
 *                  flags.</li>
 *      <li>EZBL_FLAG_8BIT (0x0008): Check only lower 8-bits of each instruction
 *                  word. Bits 8-23 of each program word are don't care values
 *                  on read-back, but there must still be dummy equivalent
 *                  cmpData locations for them. The lower 8-bits are tested
 *                  according to the binary EQUALS/ZEROS/ONES matching flags.</li>
 *      <li>EZBL_FLAG_IPARTITION (0x1000): Force 0x400000-0x7FFFFF Inactive
 *                  Partition address range (set bit 22 of pgmAddress). The
 *                  next/mismatch return addresses will also reflect the
 *                  inactive partition range.</li>
 *      <li>EZBL_FLAG_FIXED_PARTITION (0x2000): Use fixed Partition 1/Partition
 *                  2 address range for pgmAddress instead of Active/Inactive
 *                  relative addresses (XOR bit 22 of pgmAddress with
 *                  !NVMCON<10>, which is !P2ACTIV on Dual Partition capable
 *                  devices).
 *
 *                  The mismatch status program address will reflect the decoded
 *                  address range that was actually compared against.</li>
 *    </ul>
 *
 * @param *mismatchStatus
 *              Pointer to a caller allocated EZBL_MISMATCH_STATUS structure to
 *              receive extended comparison mismatch status data. Set to null if
 *              unneeded.
 *
 *              Values returned should be ignored if EZBL_ROMCompareEx() returns
 *              0, indicating a match.
 *
 * @return 0x0000 if both arrays have matching contents. A non-zero value is
 *         returned if a mismatch is detected.
 *
 *         If <i>length</i> is zero, a 0x0000 matching return value is always
 *         returned without reading from either memory. Unless null,
 *         *mismatchStatus may still be written to with dummy data.
 */
int EZBL_ROMCompareEx(unsigned long pgmAddress, const void *cmpData, unsigned int length, EZBL_COMPARE_FLAGS flags, EZBL_MISMATCH_STATUS *mismatchStatus);


/**
 * Compares an array of RAM/data space bytes to Flash/program space memory and
 * adjusts return results for Bootloading specific problems that make the Flash
 * behave as a non-ideal read/write memory, such as reserved zeros in Flash
 * Config bytes, unimplemented bits in non-volatile Config bytes, and don't-care
 * mismatches between compiler generated .hex data and MPLAB ICSP programming
 * tools (ex: Config bit masking).
 *
 * See EZBL_ROMCompare()/EZBL_ROMCompareEx() if you wish to do binary
 * comparisons without any equivalence adjustment. This is a wrapper function
 * that internally calls EZBL_ROMCompareEx().
 *
 * @param pgmAddress Even program space address to start verification at.
 *
 * @param *compareData Pointer to RAM/data space (including PSV/EDS) byte array
 *        containing the data that was attempted to be written to Flash. The
 *        data must be packed in byte form without phantom padding. I.e.
 *        byte 0 = pgmAddress low 8-bits,
 *        byte 1 = pgmAddress middle 8-bits,
 *        byte 2 = pgmAddress upper 8-bits,
 *        byte 3 = next pgmAddress low 8-bits, etc.
 *
 * @param length Maximum number of bytes to compare. A non-multiple of 3 is
 *               allowed.
 * @param status [out] Optional pointer to a EZBL_MISMATCH_STATUS structure to
 *        receive extended failure data in the event of a mismatch.
 *
 * @return 0x0000 on successful match, which may or may not have required
 *         adjustment.
 *
 *         Negative return codes for uncorrectable verification mismatches.
 *         -1: General failure with no exception allowed. Mismatch may be due to:
 *             - Attempting to overwrite a Bootloader reserved address that
 *               EZBL_WriteROM[Ex]() blocked.
 *             - Incorrect value in EZBL_NVMKey. See EZBL_WriteROM() API requirements.
 *             - Hardware write protect/Code protect settings
 *             - RAM corruption from an asynchronous ISR that isn't saving state or
 *               is causing incorrect concurrent execution of EZBL functions that
 *               require the same hardware.
 *             - Improper/unimplemented address
 *             - Bad voltage or other electricals/environmentals?
 *
 *         Positive return codes for verification mismatches of lessor
 *         significance, such as an expected failed attempt to overwrite a
 *         Bootloader reserved location.
 */
int EZBL_VerifyROM(unsigned long pgmAddress, const void *compareData, unsigned int length, EZBL_MISMATCH_STATUS *status);


/**
 * Checks the given program space address lies within a Bootloader defined
 * special address range defined by instances of EZBL_SetNoEraseRange(),
 * EZBL_SetNoProgramRange(), EZBL_SetNoVerifyRange() or EZBL_SetNoReadRange().
 *
 * @param tableBaseAddr Unsigned long start address for an EZBL program table.
 *                      Use the __builtin_section_begin() function to obtain the
 *                      base address for a table.
 * @param tablePgmAddrLen Unsigned int size of the EZBL program table, in
 *                      program space address. Use __builtin_section_size() to
 *                      retrieve this value.
 * @param testAddress Program space address to test. Unaligned addresses are
 *                    testable.
 *
 * @return 0 if the test address was not found in the given program table. 1
 *         otherwise.
 */
int EZBL_AddrInTable(unsigned long tableBaseAddr, unsigned int tablePgmAddrLen, unsigned long testAddress);


/**
 * Checks the given program space address to see if it is marked as non-erasable
 * via prior instances of the EZBL_SetNoEraseRange() macro (in the EZBL
 * Bootloader project) or due to EZBL Bootloader link time size computations.
 *
 * Although this function can be called from an Application project, it is not
 * possible to reserve additional address ranges as non-erasable without
 * recompiling the Bootloader project. This function will always return the
 * values that were applicable when the Bootloader was built.
 *
 * @param testAddress Program space address to test. Unaligned addresses are
 *                    testable.
 * @return 0 if the test address lies on a Flash page that is marked as
 *         non-erasable.
 *
 *         1 if the test address has no software restrictions applied to it.
 *         Note that hardware write protected, and unimplemented addresses will
 *         normally return 1 as well unless explicitly included in a no-erase
 *         range.
 */
static inline int __attribute__((always_inline, optimize(1))) EZBL_IsEraseRestricted(unsigned long testAddress)
{
    // Create a dummy 0 item section in case if there are no calls to EZBL_SetNoEraseRanges(). This takes no space.
    __asm__("\n    .pushsection    .text.EZBLNoEraseRanges, code, keep" EZBL_PAGE_ATTR
            "\n    .popsection");
    
    return EZBL_AddrInTable(__builtin_section_begin(".text.EZBLNoEraseRanges"), (unsigned int)__builtin_section_size(".text.EZBLNoEraseRanges"), testAddress);
}


static inline int __attribute__((always_inline, optimize(1))) EZBL_IsProgramRestricted(unsigned long testAddress)
{
    // Create a dummy 0 item section in case if there are no calls to .text.EZBLNoProgramRanges(). This takes no space.
    __asm__("\n    .pushsection    .text.EZBLNoProgramRanges, code, keep" EZBL_PAGE_ATTR
            "\n    .popsection");

    return EZBL_AddrInTable(__builtin_section_begin(".text.EZBLNoProgramRanges"), (unsigned int)__builtin_section_size(".text.EZBLNoProgramRanges"), testAddress);
}

static inline int __attribute__((always_inline, optimize(1))) EZBL_IsVerifyRestricted(unsigned long testAddress)
{
    // Create a dummy 0 item section in case if there are no calls to .text.EZBLNoVerifyRanges(). This takes no space.
    __asm__("\n    .pushsection    .text.EZBLNoVerifyRanges, code, keep" EZBL_PAGE_ATTR
            "\n    .popsection");

    return EZBL_AddrInTable(__builtin_section_begin(".text.EZBLNoVerifyRanges"), (unsigned int)__builtin_section_size(".text.EZBLNoVerifyRanges"), testAddress);
}


/**
 * unsigned long EZBL_SYM32(symbolName);
 *
 * Macro returns a 32-bit linker symbol address (or value) of absolutely defined
 * symbols or functions/variables and other items in data space, program space,
 * or debug/info sections. The symbol's value is not modified.
 *
 * If the symbol does not exist anywhere in the project and cannot be found in
 * an archive, a linker error is generated. This macro declares the symbol as an
 * extern, so creates an undefined symbol of the same name for the linker to
 * resolve.
 *
 * Use EZBL_SetSYM() if you wish to pass numerical symbol constants between
 * different source files and precompiled object libraries at compile/link time.
 *
 * @param symbolName token or identifier of the symbol without a leading
 *                   underscore. This can be the name of a function, variable,
 *                   constant, or other item without any scope or type checking.
 *
 * @return The post-linking symbol value/address for the specified symbol.
 */
#if defined(__PIC32__) || defined (__XC32__)
#define EZBL_SYM32(symbolName)                                                  \
({                                                                              \
    unsigned int i;                                                             \
    __asm__("la %0, " STRINGIFY(symbolName) : "=d"(i));                         \
    i;                                                                          \
})
#else
#define EZBL_SYM32(symbolName)                                                  \
({                                                                              \
    register volatile unsigned long i;                                          \
    __asm__ __volatile__ (                                                      \
        "\n     .ifdef __" STRINGIFY(symbolName) "_def_local__"                 \
        "\n     mov #_" STRINGIFY(symbolName) "&0xFFFF, %0"                     \
        "\n     mov #_" STRINGIFY(symbolName) ">>16, %d0"                       \
        "\n     .else"                                                          \
        "\n     mov #packed_lo(_" STRINGIFY(symbolName) "), %0"                 \
        "\n     mov #packed_hi(_" STRINGIFY(symbolName) "), %d0"                \
        "\n     .endif"                                                         \
        : "=C"(i)); ((unsigned long)i);                                         \
})
#endif


/**
 * unsigned long EZBL_SYM(symbolName);
 *
 * Macro returns the least significant 16-bits of a linker symbol address (or
 * value) of absolutely defined symbols or functions/variables and other items
 * in data space, program space, or debug/info sections. The symbol's value is
 * not modified.
 *
 * If the symbol does not exist anywhere in the project and cannot be found in
 * an archive, a linker error is generated. This macro declares the symbol as an
 * extern, so creates an undefined symbol of the same name for the linker to
 * resolve.
 *
 * @param symbolName token or identifier of the symbol without a leading
 *                   underscore. This can be the name of a function, variable,
 *                   constant, or other item without any scope or type checking.
 *
 * @return The lower 16-bits of a raw post-linking symbol value/address for the
 *         specified symbol.
 */
#if defined(__PIC32__) || defined(__XC32)
#define EZBL_SYM(symbolName)                                                    \
({                                                                              \
    unsigned int i;                                                             \
    __asm__ ("li %0, %%lo(" STRINGIFY(symbolName) ")" : "=d"(i));               \
    i;                                                                          \
})
#else
#define EZBL_SYM(symbolName)                                                    \
({                                                                              \
    unsigned int i;                                                             \
    __asm__ (                                                                   \
        "\n     .ifdef __" STRINGIFY(symbolName) "_def_local__"                 \
        "\n     mov #_" STRINGIFY(symbolName) " & 0xFFFF, %0"                   \
        "\n     .else"                                                          \
        "\n     mov #packed_lo(_" STRINGIFY(symbolName) "), %0"                 \
        "\n     .endif"                                                         \
        : "=r"(i)); i;                                                          \
})
#endif


/**
 * unsigned long EZBL_WeakSYM32(symbolName);
 *
 * Macro declares the specified extern symbol as weak and attempts to return
 * its 32-bit linker symbol address (or value). If the symbol cannot be found,
 * the project can still be built and this macro will return 0x00000000 at run
 * time.
 *
 * NOTE: When a symbol is declared weak, you can on longer make strong
 * references to the same symbol from within the same .c/.o object file.
 *
 * @param symbolName token or identifier of the symbol without a leading
 *                   underscore. This can be the name of a function, variable,
 *                   constant, or other item without any scope or type checking.
 *
 * @return The raw post-linking symbol value/address for the specified symbol.
 *         When not defined, 0x00000000 is returned.
 */
#if defined(__PIC32__) || defined(__XC32__)
#define EZBL_WeakSYM32(symbolName)                                              \
({                                                                              \
    unsigned int i;                                                             \
    __asm__ (                                                                   \
        "\n     .ifndef __" STRINGIFY(symbolName) "_def_local__"                \
        "\n     .weak " STRINGIFY(symbolName)                                   \
        "\n     .endif"                                                         \
        "\n     lui %0, %%hi(" STRINGIFY(symbolName) ")"                        \
        "\n     ori %0, %%lo(" STRINGIFY(symbolName) ")"                        \
        : "=d"(i)); i;                                                          \
})
#else
#define EZBL_WeakSYM32(symbolName)                                              \
({                                                                              \
    register volatile unsigned long i;                                          \
    __asm__ __volatile__ (                                                      \
        "\n     .ifdef __" STRINGIFY(symbolName) "_def_local__"                 \
        "\n     mov #_" STRINGIFY(symbolName) "&0xFFFF, %0"                     \
        "\n     mov #_" STRINGIFY(symbolName) ">>16, %d0"                       \
        "\n     .else"                                                          \
        "\n     .weak _" STRINGIFY(symbolName)                                  \
        "\n     mov #packed_lo(_" STRINGIFY(symbolName) "), %0"                 \
        "\n     mov #packed_hi(_" STRINGIFY(symbolName) "), %d0"                \
        "\n     .endif"                                                         \
        : "=C"(i)); ((unsigned long)i);                                         \
})
#endif


/**
 * unsigned long EZBL_WeakSYM(symbolName);
 *
 * Macro declares the specified extern symbol as weak and attempts to return
 * the lower 16-bits of the  linker symbol address (or value). If the symbol
 * cannot be found, the project can still be built and this macro will return
 * 0x0000 at run time.
 *
 * NOTE: When a symbol is declared weak, you can on longer make strong
 * references to the same symbol from within the same .c/.o object file.
 *
 * @param symbolName token or identifier of the symbol without a leading
 *                   underscore. This can be the name of a function, variable,
 *                   constant, or other item without any scope or type checking.
 *
 * @return The lower 16-bits of a raw post-linking symbol value/address for the
 *         specified symbol. When not found, returns 0x0000
 */
#if defined(__PIC32__) || defined(__XC32)
#define EZBL_WeakSYM(symbolName)                                                                \
({                                                                                              \
    unsigned int i;                                                                             \
    __asm__ ("\n    .ifndef __weaksymref_" STRINGIFY(symbolName)                                \
             "\n    .weakref __weaksymref_" STRINGIFY(symbolName) ", " STRINGIFY(symbolName)    \
             "\n    .endif"                                                                     \
             "\n    li %0, %%lo(__weaksymref_" STRINGIFY(symbolName) ")" : "=d"(i));            \
    i;                                                                                          \
})
#else
#define EZBL_WeakSYM(symbolName)                                                \
({                                                                              \
    unsigned int i;                                                             \
    __asm__ (                                                                   \
        "\n     .ifdef __" STRINGIFY(symbolName) "_def_local__"                 \
        "\n     mov #_" STRINGIFY(symbolName) " & 0xFFFF, %0"                   \
        "\n     .else"                                                          \
        "\n     .weak _" STRINGIFY(symbolName)                                  \
        "\n     mov #packed_lo(_" STRINGIFY(symbolName) "), %0"                 \
        "\n     .endif"                                                         \
        : "=r"(i)); i;                                                          \
})
#endif


/**
 * unsigned int EZBL_Handle(functionName);
 *
 * Macro returns the linker generated handle for a function. This will typically
 * either be the function address in program space or the address of a goto
 * trampoline in the first 96KB of flash generated by the linker.
 *
 * @param functionName Name of the function or other symbol to obtain a handle
 *                     to. This should be specified as a literal token. Ex:
 *                         myMainPtr = EZBL_WeakHandle(main);
 *
 * @return unsigned int handle value for the specified symbol.
 *
 *         If the symbol isn't defined in the project, a linker error is
 *         generated at build time.
 */
#define EZBL_Handle(symbolName)                                                     \
({                                                                                  \
    volatile register unsigned int i;                                               \
    __asm__ __volatile__ ("mov #handle(_" STRINGIFY(symbolName) "), %0" : "=r"(i)); \
    ((unsigned int)i);                                                              \
})


/**
 * unsigned int EZBL_WeakHandle(functionName);
 *
 * Macro returns the linker generated handle for a function. This will typically
 * either be the function address in program space or the address of a goto
 * trampoline in the first 96KB of flash generated by the linker.
 *
 * @param functionName Name of the function or other symbol to obtain a handle
 *                     to. This should be specified as a literal token. Ex:
 *                         myMainPtr = EZBL_WeakHandle(main);
 *
 * @return unsigned int handle value for the specified symbol.
 *
 *         If the symbol isn't defined in the project, a null handle is
 *         returned.
 */
#define EZBL_WeakHandle(symbolName)                                             \
({                                                                              \
    volatile register unsigned int i;                                           \
    __asm__ __volatile__ (                                                      \
        "\n     .ifndef __" STRINGIFY(symbolName) "_def_local__"                \
        "\n     .weak _" STRINGIFY(symbolName)                                  \
        "\n     .endif"                                                         \
        "\n     mov #handle(_" STRINGIFY(symbolName) "), %0"                    \
        : "=r"(i));                                                             \
        ((unsigned int)i);                                                      \
})


#if defined(__XC16__)
/**
 * void EZBL_ForwardIntToBoot(interruptName);
 *
 * Macro clears a bit in EZBL_ForwardBootloaderISR indicating the specified
 * hardware interrupt should be handled by the corresponding Bootloader defined
 * ISR (if present). If the Bootloader does not implement the specified ISR,
 * this macro does nothing.
 *
 * This macro is only applicable on ordinary Single Partition Bootloaders and
 * Applications (not Dual Partition projects).
 *
 * This macro is equivalent to calling:
 *      EZBL_ForwardBootloaderISR &= ~EZBL_WeakSYM32(EZBL_FORWARD_MASK_interruptName);
 * However, the code generated by this macro is smaller and more efficient (6
 * instructions).
 *
 * @param interruptName A literal specifying the name of the interrupt the
 *                      Bootloader should handle. This parameter must be a
 *                      literal token, such as:
 *                          T1
 *                          U2RX
 *                          SI2C1
 *                      These correspond, respectively, to the Timer 1
 *                      Interrupt, the UART 2 Receive Interrupt and the I2C 1
 *                      Slave Interrupt. The legal interrupt names are the same
 *                      as the bit names in the device's IECx Interrupt Enable
 *                      registers or the IFSx Interrupt Flag registers, but
 *                      without an 'IE' or 'IF' suffix.
 */
#define EZBL_ForwardIntToBoot(interruptName)                                                \
    __asm__ __volatile__ (                                                                  \
        "\n     .ifndef __EZBL_FORWARD_MASK_" STRINGIFY(interruptName) "_def_local__"       \
        "\n     .weak   _EZBL_FORWARD_MASK_" STRINGIFY(interruptName)                       \
        "\n     .endif"                                                                     \
        "\n     mov     #packed_lo(_EZBL_FORWARD_MASK_" STRINGIFY(interruptName) "), w0"    \
        "\n     com     w0, w0"                                                             \
        "\n     and     _EZBL_ForwardBootloaderISR"                                         \
        "\n     mov     #packed_hi(_EZBL_FORWARD_MASK_" STRINGIFY(interruptName) "), w0"    \
        "\n     com     w0, w0"                                                             \
        "\n     and     _EZBL_ForwardBootloaderISR+2"                                       \
        : : : "cc", "w0")


/**
 * void EZBL_ForwardIntToApp(interruptName);
 *
 * Macro sets a bit in EZBL_ForwardBootloaderISR indicating the specified
 * hardware interrupt should be handled by the corresponding Application defined
 * ISR (or Application defined _DefaultInterrupt() when no App ISR exists). If
 * the Bootloader does not implement the specified ISR, this macro does nothing.
 *
 * This macro is only applicable on ordinary Single Partition Bootloaders and
 * Applications (not Dual Partition projects).
 *
 * This macro is equivalent to calling:
 *      EZBL_ForwardBootloaderISR |= EZBL_WeakSYM32(EZBL_FORWARD_MASK_interruptName);
 * However, the code generated by this macro is smaller and more efficient (4
 * instructions).
 *
 * @param interruptName A literal specifying the name of the interrupt the
 *                      Bootloader should handle. This parameter must be a
 *                      literal token, such as:
 *                          T1
 *                          U2RX
 *                          SI2C1
 *                      These correspond, respectively, to the Timer 1
 *                      Interrupt, the UART 2 Receive Interrupt and the I2C 1
 *                      Slave Interrupt. The legal interrupt names are the same
 *                      as the bit names in the device's IECx Interrupt Enable
 *                      registers or the IFSx Interrupt Flag registers, but
 *                      without an 'IE' or 'IF' suffix.
 */
#define EZBL_ForwardIntToApp(interruptName)                                             \
    __asm__ __volatile__ (                                                              \
        "\n     .ifndef __EZBL_FORWARD_MASK_" STRINGIFY(interruptName) "_def_local__"   \
        "\n     .weak   _EZBL_FORWARD_MASK_" STRINGIFY(interruptName)                   \
        "\n     .endif"                                                                 \
        "\n     .ifndef __EZBL_ForwardBootloaderISR_def_local__"                        \
        "\n     .weak   _EZBL_ForwardBootloaderISR"                                     \
        "\n     .endif"                                                                 \
        "\n     mov     #packed_lo(_EZBL_FORWARD_MASK_" STRINGIFY(interruptName) "), w0"\
        "\n     ior     _EZBL_ForwardBootloaderISR"                                     \
        "\n     mov     #packed_hi(_EZBL_FORWARD_MASK_" STRINGIFY(interruptName) "), w0"\
        "\n     ior     _EZBL_ForwardBootloaderISR+2"                                   \
        : : : "cc", "w0")


/**
 * void EZBL_ForwardAllIntToBoot(void);
 *
 * Macro clears all bits in EZBL_ForwardBootloaderISR indicating that all
 * interrupt handlers defined in your Bootloader project should be used,
 * regardless of if the Bootloader or Application project is executing at the
 * moment.
 *
 * All interrupts lacking any handler in the Bootloader project will always
 * vector for handling by the Application project.
 *
 * This macro is only applicable on ordinary 16-bit, Single Partition 
 * Bootloaders and Applications (not Dual Partition projects).
 */
#define EZBL_ForwardAllIntToBoot()                          \
    __asm__ __volatile__ (                                  \
        "\n     clr     _EZBL_ForwardBootloaderISR"         \
        "\n     clr     _EZBL_ForwardBootloaderISR+0x2")


/**
 * void EZBL_ForwardAllIntToApp(void);
 *
 * Macro sets all bits in EZBL_ForwardBootloaderISR indicating that all
 * interrupt handlers defined in your Bootloader project should be ignored.
 * All interrupts will vector for handling by the Application project.
 *
 * This macro is only applicable on ordinary 16-bit, Single Partition 
 * Bootloaders and Applications (not Dual Partition projects).
 */
#define EZBL_ForwardAllIntToApp()                           \
    __asm__ __volatile__ (                                  \
        "\n     setm    _EZBL_ForwardBootloaderISR"         \
        "\n     setm    _EZBL_ForwardBootloaderISR+0x2")


#define EZBL_ForwardAllIRQToApp()   EZBL_ForwardAllIntToApp()   // Macro referencing EZBL_ForwardAllIntToApp(). This macro exists for API compatibility between 16-bit and 32-bit versions of EZBL.
#define EZBL_ForwardAllIRQToBoot()  EZBL_ForwardAllIntToBoot()  // Macro referencing EZBL_ForwardAllIntToBoot(). This macro exists for API compatibility between 16-bit and 32-bit versions of EZBL.


/**
 * unsigned int EZBL_GetForwardInt(interruptName);
 *
 * Macro extracts a bit from EZBL_ForwardBootloaderISR indicating if the
 * specified hardware interrupt is currently being forwarded to the Application
 * for handling by an Application defined ISR, or if the interrupt will be
 * processed by a Bootloader defined ISR.
 *
 * This macro is only applicable on ordinary Single Partition Bootloaders and
 * Applications (not Dual Partition projects).
 *
 * @param interruptName A literal specifying the name of the interrupt to test.
 *                      This parameter must be a literal token, such as:
 *                          T1
 *                          U2RX
 *                          SI2C1
 *                      These correspond, respectively, to the Timer 1
 *                      Interrupt, the UART 2 Receive Interrupt and the I2C 1
 *                      Slave Interrupt. The legal interrupt names are the same
 *                      as the bit names in the device's IECx Interrupt Enable
 *                      registers or the IFSx Interrupt Flag registers, but
 *                      without an 'IE' or 'IF' suffix.
 *
 * @return Bit state of the specified interrupt within the
 *         EZBL_ForwardBootloaderISR global control variable.
 *
 *         1 - The interrupt is being forwarded to the Application via the
 *             Application's Interrupt Goto Table. If the Application does not
 *             implement the ISR, execution will branch to the Application's
 *             _DefaultInterrupt() handler. All Bootloader handling for the
 *             interrupt is disabled.
 *
 *         0 - The interrupt is being handled by the Bootloader via the hardware
 *             IVT. If the Bootloader does not implement an ISR for this
 *             interrupt, then the interrupt is implicitly forwarded to the
 *             Application, despite this return result.
 *
 *         If interruptName is a token which is not a hardware implemented
 *         interrupt on the target processor or which does not have an already
 *         implemented ISR for it in the Bootloader project, this macro will
 *         trigger a link-time build error. Therefore, this macro is most
 *         suitable for use in Application projects. Bootloader projects should
 *         use the EZBL_GetWeakForwardInt() macro instead to avoid new
 *         definition linking errors.
 */
#define EZBL_GetForwardInt(interruptName)                                               \
({                                                                                      \
    volatile register unsigned int i;                                                   \
                                                                                        \
    __asm__ __volatile__ (                                                              \
        "\n     mov     #packed_lo(_EZBL_FORWARD_MASK_" STRINGIFY(interruptName) "), w0"\
        "\n     and     _EZBL_ForwardBootloaderISR, WREG"                               \
        "\n     push    w0"                                                             \
        "\n     mov     #packed_hi(_EZBL_FORWARD_MASK_" STRINGIFY(interruptName) "), w0"\
        "\n     and     _EZBL_ForwardBootloaderISR+2, WREG"                             \
        "\n     ior     w0, [--w15], %0"                                                \
        "\n     btss    _SR, #1" /* SR<1> is Zero status bit */                         \
        "\n     mov     #1, %0"                                                         \
        : "=r"(i) : : "cc", "w0");                                                      \
        i;  /* return value */                                                          \
})



/**
 * unsigned int EZBL_GetWeakForwardInt(interruptName);
 *
 * Macro extracts a bit from EZBL_ForwardBootloaderISR indicating if the
 * specified hardware interrupt is currently being forwarded to the Application
 * for handling by an Application defined ISR, or if the interrupt will be
 * processed by a Bootloader defined ISR (when present).
 *
 * This macro is only applicable on ordinary Single Partition Bootloaders and
 * Applications. Calling this function in a Dual Partition project will succeed
 * but return an undefined value.
 *
 * This macro is equivalent to the EZBL_GetForwardInt() macro, but will not
 * generate a build time error if the specified interrupt does not exist on the
 * device or if the Bootloader has not yet implemented an ISR for the specified
 * interrupt. This macro is preferable only in Bootloader projects.
 *
 * @param interruptName A literal specifying the name of the interrupt to test.
 *                      This parameter must be a literal token, such as:
 *                          T1
 *                          U2RX
 *                          SI2C1
 *                      These correspond, respectively, to the Timer 1
 *                      Interrupt, the UART 2 Receive Interrupt and the I2C 1
 *                      Slave Interrupt. The legal interrupt names are the same
 *                      as the bit names in the device's IECx Interrupt Enable
 *                      registers or the IFSx Interrupt Flag registers, but
 *                      without an 'IE' or 'IF' suffix.
 *
 * @return Bit state of the specified interrupt within the
 *         EZBL_ForwardBootloaderISR global control variable.
 *
 *         1 - The interrupt is being forwarded to the Application via the
 *             Application's Interrupt Goto Table. If the Application does not
 *             implement the ISR, execution will branch to the Application's
 *             _DefaultInterrupt() handler. All Bootloader handling for the
 *             interrupt is disabled.
 *
 *         0 - The interrupt is being handled by the Bootloader via the hardware
 *             IVT. If the Bootloader does not implement an ISR for this
 *             interrupt, then the interrupt is implicitly forwarded to the
 *             Application, despite this return result.
 *
 *         If the interruptName specifies a token which is not a hardware
 *         implemented interrupt on the target processor, this "weak" macro
 *         version will compile and link normally, but return an unspecified
 *         value when called.
 */
#define EZBL_GetWeakForwardInt(interruptName)                                           \
({                                                                                      \
    volatile register unsigned int i;                                                   \
                                                                                        \
    __asm__ __volatile__ (                                                              \
        "\n     .ifndef __EZBL_FORWARD_MASK_" STRINGIFY(interruptName) "_def_local__"   \
        "\n     .weak   _EZBL_FORWARD_MASK_" STRINGIFY(interruptName)                   \
        "\n     .endif"                                                                 \
        "\n     .ifndef __EZBL_ForwardBootloaderISR_def_local__"                        \
        "\n     .weak   _EZBL_ForwardBootloaderISR"                                     \
        "\n     .endif"                                                                 \
        "\n     mov     #packed_lo(_EZBL_FORWARD_MASK_" STRINGIFY(interruptName) "), w0"\
        "\n     and     _EZBL_ForwardBootloaderISR, WREG"                               \
        "\n     push    w0"                                                             \
        "\n     mov     #packed_hi(_EZBL_FORWARD_MASK_" STRINGIFY(interruptName) "), w0"\
        "\n     and     _EZBL_ForwardBootloaderISR+2, WREG"                             \
        "\n     ior     w0, [--w15], %0"                                                \
        "\n     btss    _SR, #1" /* SR<1> is Zero status bit */                         \
        "\n     mov     #1, %0"                                                         \
        : "=r"(i) : : "cc", "w0");                                                      \
        i;  /* return value */                                                          \
})


#elif defined(__XC32__) || defined(__PIC32__)
     
/**
 * Dynamically redirects project ownership of the specified hardware interrupt
 * vector between Bootloader and Application projects.
 *
 * @param toAppProject Sets the destination project which should receive the
 *                     specified interrupt. 0 means the Bootloader project will
 *                     receive the interrupt, non-zero means the Application
 *                     project will receive it instead.
 *
 *                     For convenience, the EZBL_ForwardIRQToApp() and
 *                     EZBL_ForwardIRQToBoot() macros may be used to avoid 
 *                     having to specify this parameter. 
 *
 * @param irqNum IRQ/Interrupt Vector number that you wish to change the
 *               hardware vectoring destination for. Use the macros defined in
 *               the <xc.h> to specify this parameter, ex: _UART2_RX_VECTOR,
 *               _TIMER_1_VECTOR, etc.
 *
 *               If the Bootloader project does not implement an ISR for the
 *               specified IRQ, this function will do nothing (hardware
 *               vectoring will branch to whatever project is executing/defined
 *               by EBase).
 *
 * @return Last ISR function address which was programmed for the vector number.
 *         If no Bootloader ISR was defined for the vector number, 0 is
 *         returned.
 *
 *         If an interrupt is forwarded to the Application and no Application
 *         exists in flash, or the Application does not implement the vector, 
 *         this function has no current or future effect and will return 0. If 
 *         the Bootloader currently was the IRQ's target, the IRQ will continue 
 *         to branch to the Bootloader's ISR.
 */
unsigned int EZBL_ForwardIRQ(int toAppProject, int irqNum);


/**
 * Forwards all hardware interrupts to a single project (either Bootloader or
 * Application). This is a wrapper for EZBL_ForwardIRQ() that self-enumerates
 * all of the valid vector numbers which can be dynamically forwarded. I.e. only 
 * affects interrupts that have a Bootloader-defined ISR handler for it. 
 *
 * If there is no Application code currently programmed in flash or the 
 * Application does not implement an interrupt vector that does exist in the 
 * Bootloader, then the corresponding vector target will be left unchanged 
 * (presumably still pointing to the Bootloader's ISR).
 *
 * Related macros/functions:
 *  <ul>
 *      <li>EZBL_ForwardIRQ()</li>
 *      <li>EZBL_ForwardAllIRQToApp()</li>
 *      <li>EZBL_ForwardAllIRQToBoot()</li>
 *      <li>EZBL_ForwardIRQToApp()</li>
 *      <li>EZBL_ForwardIRQToBoot()</li>
 *  </ul>
 *
 * @param toAppProject Sets the destination project which should receive 
 *                     Bootloader-implemented interrupts. 0 means the Bootloader 
 *                     project will receive all of its interrupts, non-zero 
 *                     means the Application project will receive them instead.
 */
void EZBL_ForwardAllIRQ(int toAppProject);


/**
 * Dynamically redirects project ownership of all available hardware interrupts
 * to the Application project. The Bootloader project will not control any
 * interrupts after calling this macro.
 *
 * Ensure the Application project has interrupt handlers defined for the
 * Bootloader implemented interrupts or that the interrupts are disabled prior
 * to calling this macro as the effect will be immediate.
 *
 * If there is no Application code currently programmed in flash, no change 
 * to interrupt vectoring will occur.
 */
#define EZBL_ForwardAllIRQToApp()               EZBL_ForwardAllIRQ(1)
#define EZBL_ForwardAllIntToApp()               EZBL_ForwardAllIRQ(1)           // Macro referencing EZBL_ForwardAllIRQToApp(). This macro exists for API compatibility between 16-bit and 32-bit versions of EZBL.


/**
 * Dynamically redirects project ownership of all hardware interrupts to the
 * Bootloader project. Only interrupts the Bootloader project actually
 * implements ISRs for will be affected by this macro. The currently executing
 * project will retain control of all other hardware interrupts.
 */
#define EZBL_ForwardAllIRQToBoot()              EZBL_ForwardAllIRQ(0)
#define EZBL_ForwardAllIntToBoot()              EZBL_ForwardAllIRQ(0)           // Macro referencing EZBL_ForwardAllIRQToBoot(). This macro exists for API compatibility between 16-bit and 32-bit versions of EZBL.



/**
 * Dynamically redirects the specified hardware interrupt to the Application
 * project instead of executing a Bootloader defined ISR.
 *
 * Ensure the Application project has an appropriate interrupt handler for the
 * vector or that the interrupt enable bit (IECx bit) is cleared before
 * reassigning an interrupt vector as the effect will be immediate.
 *
 * @param irqNum IRQ/Interrupt Vector number that you wish to change the
 *               hardware vectoring destination for. Use the macros defined in
 *               the <xc.h> to specify this parameter, ex: _UART2_RX_VECTOR,
 *               _TIMER_1_VECTOR, etc.
 *
 *               If the Bootloader project does not implement an ISR for the
 *               specified IRQ, this function will do nothing (hardware
 *               vectoring will branch to whatever project is executing/defined
 *               by EBase).
 *
 * @return Last ISR function address which was programmed for the vector number.
 *         If no Bootloader ISR was defined for the vector number, 0 is
 *         returned.
 */
#define EZBL_ForwardIRQToApp(irqNum)      EZBL_ForwardIRQ(1, irqNum)


/**
 * Dynamically redirects the specified hardware interrupt to the Bootloader
 * project instead of executing a whatever ISR exists in the currently executing
 * project.
 *
 * @param irqNum IRQ/Interrupt Vector number that you wish to change the
 *               hardware vectoring destination for. Use the macros defined in
 *               the <xc.h> to specify this parameter, ex: _UART2_RX_VECTOR,
 *               _TIMER_1_VECTOR, etc.
 *
 *               If the Bootloader project does not implement an ISR for the
 *               specified IRQ, this function will do nothing (hardware
 *               vectoring will branch to whatever project is executing/defined
 *               by EBase).
 *
 * @return Last ISR function address which was programmed for the vector number.
 *         If no Bootloader ISR was defined for the vector number, 0 is
 *         returned.
 */
#define EZBL_ForwardIRQToBoot(irqNum)     EZBL_ForwardIRQ(0, irqNum)
#endif


/**
 * DEPRECATED: Use EZBL_SYM32() or EZBL_SYM() instead.
 *
 * #define EZBL_GetSymbol(dest, symbolName)
 *
 * Macro that retrieves the 24 or 32-bit program memory address of the specified
 * symbol and writes it into a variable.
 *
 * Executing this macro has a similar effect to using the C address-of operator
 * (&) to get a pointer. For example calling:
 *      EZBL_GetSymbol(myFunctionPtr, main);
 * will have a similar effect to:
 *      myFunctionPtr = &main;
 * However, this macro will always capture the true 24-bit program address of
 * the main() function. Using the address-of operator will sometimes return a
 * true 16-bit program address of the main() function, and other times return a
 * 16-bit handle to a "near" program memory address that contains a "goto _main"
 * instruction in it.
 *
 * This macro can be used to retrieve symbol addresses for items that have
 * global scope, but are not defined in the C context. For example, any symbol
 * defined in a .gld linker script. The symbol need not have any C extern
 * declaration associated with it.
 *
 * Occasionally, this macro will generate an impossible operand reload assembly
 * error with certain XC16 compiler versions if certain other surrounding code
 * conditions and compiler optimization settings are used. In such a case, you
 * can either isolate this macro use to a separate function, disable compiler
 * optimizations and/or revert to the more computationally expensive SYM32()
 * macro defined in ezbl_core.h.
 *
 * @param dest 32-bit variable to write the symbol address into. This parameter
 *             must be an L-value. I.e. something you would put on the left hand
 *             side of an assignment statement such as the 'i' in:
 *                i = counter + 1;
 *             dest normally should be of unsigned long type, but other L-value
 *             variables could be used as well, so long as an assignment to it
 *             can be made from an unsigned long without requiring a type-cast.
 *
 * @param symbolName Raw C-style symbol name. For example, to get the address of
 *                   the main() function you would simply type "main" without
 *                   the quotation marks and without a leading underscore which
 *                   gets automatically added to all C symbols.
 *
 * @return 24 or 32-bit symbol address (value) in 'dest' variable.
 */
#define EZBL_GetSymbol(dest, symbolName)                                                    \
    do                                                                                      \
    {                                                                                       \
        volatile register unsigned long destReg;                                            \
        __asm__ __volatile__ ("\n    mov #packed_lo(_" STRINGIFY(symbolName) "), %0"        \
                              "\n    mov #packed_hi(_" STRINGIFY(symbolName) "), %d0"       \
                              : "=r"(destReg));                                             \
        dest = destReg;                                                                     \
    } while(0)


/**
 * DEPRECATED: Use EZBL_WeakSYM32() or EZBL_WeakSYM() instead.
 *
 * #define EZBL_GetWeakSymbol(dest, symbolName)
 *
 * Macro that retrieves the 24 or 32-bit program memory address of the specified
 * symbol and writes it into a variable. If the symbol is undefined at link
 * time, rather than failing to link, linking will succeed and the run-time
 * return dest value will be 0.
 *
 * Executing this macro has a similar effect to using the C address-of operator
 * (&) to get a pointer. For example calling:
 *      EZBL_GetWeakSymbol(myFunctionPtr, main);
 * will have a similar effect to:
 *      myFunctionPtr = &main;
 * However, this macro will always capture the true 24-bit program address of
 * the main() function. Using the address-of operator will sometimes return a
 * true 16-bit program address of the main() function, and other times return a
 * 16-bit handle to a "near" program memory address that contains a "goto _main"
 * instruction in it.
 *
 * This macro can be used to retrieve symbol addresses for items that have
 * global scope, but are not defined in the C context. For example, any symbol
 * defined in a .gld linker script. The symbol need not have any C extern
 * declaration associated with it.
 *
 * Occasionally, this macro will generate an impossible operand reload assembly
 * error with certain XC16 compiler versions if certain other surrounding code
 * conditions and compiler optimization settings are used. In such a case, you
 * can either isolate this macro use to a separate function, disable compiler
 * optimizations and/or revert to the more computationally expensive SYM32()
 * macro defined in ezbl_core.h.
 *
 * @param dest 32-bit variable to write the symbol address into. This parameter
 *             must be an L-value. I.e. something you would put on the left hand
 *             side of an assignment statement such as the 'i' in:
 *                i = counter + 1;
 *             dest normally should be of unsigned long type, but other L-value
 *             variables could be used as well, so long as an assignment to it
 *             can be made from an unsigned long without requiring a type-cast.
 *
 * @param symbolName Raw C-style symbol name. For example, to get the address of
 *                   the main() function you would simply type "main" without
 *                   the quotation marks and without a leading underscore which
 *                   gets automatically added to all C symbols.
 *
 * @return 24 or 32-bit symbol address (value) in 'dest' variable if symbolName
 *         exists. When symbolName does not exist, 0 is returned.
 */
#define EZBL_GetWeakSymbol(dest, symbolName)                                                            \
    do                                                                                                  \
    {                                                                                                   \
        volatile register unsigned long destReg;                                                        \
        __asm__ __volatile__(".weak     _" STRINGIFY(symbolName) "            \n"                       \
                             "    mov #packed_lo(_" STRINGIFY(symbolName) "), %0  \n"                   \
                             "    mov #packed_hi(_" STRINGIFY(symbolName) "), %d0   " : "=r"(destReg)); \
        dest = destReg;                                                                                 \
    } while(0)


extern EZBL_FIFO *EZBL_STDIN;   // Pointer to the RX FIFO generated by a call to UART_Reset() or I2C_Reset() with the useForStdio parameter == 1
extern EZBL_FIFO *EZBL_STDOUT;  // Pointer to the TX FIFO generated by a call to UART_Reset() or I2C_Reset() with the useForStdio parameter == 1
//extern EZBL_FIFO *EZBL_STDERR;  // Not used in this version of EZBL. This value is not currently set by UART_Reset() or I2C_Reset() calls.


/**
 * EZBL's printf() implementation (similar to _printf_cdnopuxX() which
 * has no floating point support). Small (~1.2KB) and fast.
 *
 * Outputs to stdout via write() or *EZBL_STDOUT via EZBL_FIFOWrite().
 *
 * Does not support floating point, decimal output for 64-bit long long integers
 * (long longs with run-time value that fits in a 32-bit long is okay, i.e. has
 * range -2147483648 to +4294967295 will still work), and some formatting
 * options, but is otherwise intended to be compatible/identical.
 *
 * @param *format Formatting string
 *      See XC16 Libraries documentation for printf() to get
 *      full documentation. The basic formatting template is:
 *      <ul>
 *          <li> %[flags][width][.precision][size]type</li>
 *      </ul>
 *
 *      size field values are:
 *      <ul>
 *          <li>[nothing] 16-bit input</li>
 *          <li>'h' explicit 16-bit short input (better to omit this for easier compatibility with XC32's printf())</li>
 *          <li>'l' long or 32-bit input (16-bit wide character for %c)</li>
 *          <li>'ll' long long or 64-bit input</li>
 *      </ul>
 *
 *      type codes are:
 *      <ul>
 *          <li>'i' signed decimal integer</li>
 *          <li>'d' signed decimal integer</li>
 *          <li>'u' unsigned decimal</li>
 *          <li>'x' lowercase hexadecimal</li>
 *          <li>'X' uppercase hexadecimal</li>
 *          <li>'c' character (print a binary value, typically an ASCII character)</li>
 *          <li>'s' null terminated string (not recursively parsed for escapes)</li>
 *          <li>'%' the literal percent character</li>
 *          <li>NOT SUPPORTED: 'f', 'e', 'E', 'g', 'G' floating point types</li>
 *      </ul>
 *
 * @param ... Variable arguments matching the formatting string
 *
 * @return Number of characters written to stdout via the write() function, or
 *         when not defined, the EZBL_STDOUT FIFO.
 */
int EZBL_printf(const char *format, ...);
static int __attribute__((weakref("EZBL_printf"))) EZBL_printfW(const char *format, ...);   // Weak referenced symbol for EZBL_printf()


/**
 * EZBL_printf() wrapper function that takes a va_args pointer instead of
 * variable number of function arguments. Use this function when you wish to
 * create your own printf-style function when you need to pass through a whole
 * list of variadic parameters from your function to a printf() implementation.
 *
 * See EZBL_printf() for more information.
 *
 * @param *format Formatting string. Same as EZBL_printf() parameter.
 * @param *args Pointer to variadic parameters. This is typically a va_list
 *        returned by calling va_start().
 *
 * @return Number of characters written to stdout via write(), or when not
 *         implemented, the EZBL_COM_TX FIFO.
 */
int EZBL_vprintf(const char *format, void *args);
static int __attribute__((weakref("EZBL_vprintf"))) EZBL_vprintfW(const char *format, void *args);  // Weak referenced symbol for EZBL_vprintf


/**
 * EZBL's sprintf() implementation (similar to _printf_cdnopuxX() which
 * has no floating point support). Small (~1.2KB) and fast.
 *
 * EZBL_sprintf() is implemented as a wrapper to the EZBL_printf() function, so
 * calling both in one project will not adversely impact code size.
 *
 * Does not support floating point, decimal output for 64-bit long long integers
 * (long longs with run-time value that fit in a 32-bit long is okay, i.e. has
 * range -2147483648 to +4294967295 will still work), and some formatting
 * options, but is otherwise intended to be compatible/identical.
 *
 * @param *outputBuf Pointer to a caller allocated memory buffer to store the
 *        generated string. The buffer must be large enough to hold the entire
 *        contents of a worst case formatting string with converted parameters,
 *        plus a null terminator byte.
 *
 * @param *format Formatting string
 *      See XC16 Libraries documentation for printf() to get
 *      full documentation. The basic formatting template is:
 *      <ul>
 *          <li> %[flags][width][.precision][size]type</li>
 *      </ul>
 *
 *      size field values are:
 *      <ul>
 *          <li>[nothing] 16-bit input</li>
 *          <li>'h' explicit 16-bit short input (better to omit this for easier compatibility with XC32's printf())</li>
 *          <li>'l' long or 32-bit input (16-bit wide character for %c)</li>
 *          <li>'ll' long long or 64-bit input</li>
 *      </ul>
 *
 *      type codes are:
 *      <ul>
 *          <li>'i' signed decimal integer</li>
 *          <li>'d' signed decimal integer</li>
 *          <li>'u' unsigned decimal</li>
 *          <li>'x' lowercase hexadecimal</li>
 *          <li>'X' uppercase hexadecimal</li>
 *          <li>'c' character (print a binary value, typically an ASCII character)</li>
 *          <li>'s' null terminated string (not recursively parsed for escapes)</li>
 *          <li>'%' the literal percent character</li>
 *          <li>NOT SUPPORTED: 'f', 'e', 'E', 'g', 'G' floating point types</li>
 *      </ul>
 *
 * @param ... Variable arguments matching the formatting string
 *
 * @return Number of characters written to the outputBuf pointer. This sprintf()
 *         version includes the null terminator in the output string and
 *         includes it in the return count.
 */
int EZBL_sprintf(char *outputBuf, const char *format, ...);
static int __attribute__((weakref("EZBL_sprintf"))) EZBL_sprintfW(char *outputBuf, const char *format, ...);    // Weak referenced symbol for EZBL_sprintf()


/**
 * EZBL_sprintf() wrapper function that takes a va_args pointer instead of
 * variable number of function arguments. Use this function when you wish to
 * create your own sprintf-style function when you need to pass through a whole
 * list of variadic parameters from your function to an sprintf()
 * implementation.
 *
 * See EZBL_sprintf() for more information.
 *
 * @param *outputBuf Pointer to a caller allocated memory buffer to store the
 *        generated string. The buffer must be large enough to hold the entire
 *        contents of a worst case formatting string with converted parameters,
 *        plus a null terminator byte.
 *
 * @param *format Formatting string. Same as EZBL_printf() parameter.
 *
 * @param *args Pointer to variadic parameters. This is typically a va_list
 *        returned by calling va_start().
 *
 * @return Number of characters written to the outputBuf pointer. This
 *         vsprintf() version includes the null terminator in the output string
 *         and includes it in the return count.
 */
int EZBL_vsprintf(char *outputBuf, const char *format, void *args);
static int __attribute__((weakref("EZBL_vsprintf"))) EZBL_vsprintfW(char *outputBuf, const char *format, void *args);   // Weak referenced symbol for EZBL_vsprintf()



int EZBL_vfprintf(EZBL_FIFO *dest, const char *format, void *args);
int EZBL_fprintf(EZBL_FIFO *dest, const char *format, ...);



/**
 * Converts an unsigned 16-bit integer into an ASCII decimal (base 10) string
 * representation. The string is NOT null terminated, but leading zeros are
 * omitted.
 *
 * An ASCII decimal string is essentially Binary Coded Decimal, with each 0-9
 * digit encoded into an 8-bit field and with the '0' (0x30) character offset
 * value added.
 *
 * @param i unsigned (up to) 16-bit integer to write in ASCII form.
 *
 * @param *dest Pointer to a memory buffer to receive the generated ASCII
 *              characters. This buffer should be at least 5 bytes long when
 *              converting integers >=10000, 4 bytes for <=9999 and >= 1000, 3
 *              bytes for >=999 and <=100, etc.
 *
 *              Characters are written in print form, i.e. most significant
 *              character first.
 *
 *              Each buffer byte will be internally written up to 11 times, so
 *              do not specify a peripheral SFR pointer that will do something
 *              in hardware when written with arbitrary contents.
 *
 * @return Number of ASCII characters written to *dest. The value will always be
 *         >= 1 and <= 5. Values less than 5 indicate the number had leading
 *         zero digits that were omitted from being written.
 *
 *         An input of zero is outputted as a single '0' digit.
 */
unsigned int EZBL_uitoa(unsigned int i, void *dest);
static unsigned int __attribute__((weakref("EZBL_uitoa"))) EZBL_uitoaW(unsigned int i, void *dest);


/**
 * Converts an unsigned 32-bit integer into an ASCII decimal (base 10) string
 * representation. The string is NOT null terminated, but leading zeros are
 * omitted.
 *
 * An ASCII decimal string is essentially Binary Coded Decimal, with each 0-9
 * digit encoded into an 8-bit field and with the '0' (0x30) character offset
 * value added.
 *
 * @param i unsigned (up to) 32-bit integer to write in ASCII form.
 *
 * @param *dest Pointer to a memory buffer to receive the generated ASCII
 *              characters. This buffer should be at least 10 bytes long when
 *              converting integers >= 1 billion, 9 bytes for <=999 million and
 *              >= 100 million, etc.
 *
 *              Characters are written in print form, i.e. most significant
 *              character first.
 *
 *              Each buffer byte will be internally written up to 11 times, so
 *              do not specify a peripheral SFR pointer that will do something
 *              in hardware when written with arbitrary contents.
 *
 * @return Number of ASCII characters written to *dest. The value will always be
 *         >= 1 and <= 10. Values less than 10 indicate the number had leading
 *         zero digits that were omitted from being written.
 *
 *         An input of zero is outputted as a single '0' digit.
 */
unsigned int EZBL_ultoa(unsigned long i, void *dest);
static unsigned int __attribute__((weakref("EZBL_ultoa"))) EZBL_ultoaW(unsigned long i, void *dest);


/**
 * Converts an unsigned 64-bit integer into an ASCII decimal (base 10) string
 * representation. The string is NOT null terminated, but leading zeros are
 * omitted.
 *
 * An ASCII decimal string is essentially Binary Coded Decimal, with each 0-9
 * digit encoded into an 8-bit field and with the '0' (0x30) character offset
 * value added.
 *
 * @param i unsigned (up to) 64-bit integer to write in ASCII form.
 *
 * @param *dest Pointer to a memory buffer to receive the generated ASCII
 *              characters. This buffer should be at least 20 bytes long when
 *              converting integers >= 1*10^19, 19 bytes for < 1*10^19, 18 bytes
 *              for < 1*10^18, etc.
 *
 *              Characters are written in print form, i.e. most significant
 *              character first.
 *
 *              Each buffer byte will be internally written up to 11 times, so
 *              do not specify a peripheral SFR pointer that will do something
 *              in hardware when written with arbitrary contents.
 *
 * @return Number of ASCII characters written to *dest. The value will always be
 *         >= 1 and <= 20. Values less than 20 indicate the number had leading
 *         zero digits that were omitted from being written.
 *
 *         An input of zero is outputted as a single '0' digit.
 */
unsigned int EZBL_ulltoa(unsigned long long i, void *dest);
static unsigned int __attribute__((weakref("EZBL_ulltoa"))) EZBL_ulltoaW(unsigned long long i, void *dest);


/**
 * Converts a signed 16-bit integer into an ASCII decimal (base 10) string
 * representation. The string is NOT null terminated, but leading zeros are
 * omitted.
 *
 * For negative numbers, a leading '-' negative sign is written. Zero and
 * positive values do not have a leading sign character applied.
 *
 * An ASCII decimal string is essentially Binary Coded Decimal, with each 0-9
 * digit encoded into an 8-bit field and with the '0' (0x30) character offset
 * value added.
 *
 * @param i signed (up to) 16-bit integer to write in ASCII form.
 *
 * @param *dest Pointer to a memory buffer to receive the generated ASCII
 *              characters. This buffer should be at least 6 bytes long when
 *              converting integers <= -10000, 5 bytes for >= -9999 or >= 10000,
 *              4 bytes for >= -999 or >= 1000, etc.
 *
 *              Characters are written in print form, i.e. sign (only if
 *              negative), followed by most significant character first.
 *
 *              Each buffer byte will be internally written up to 11 times, so
 *              do not specify a peripheral SFR pointer that will do something
 *              in hardware when written with arbitrary contents.
 *
 * @return Number of ASCII characters written to *dest, including the negative
 *         sign if applicable. The value will always be >= 1 and <= 6. Values
 *         less than 6 indicate the number had leading positive sign and/or zero
 *         digits that were omitted from being written.
 *
 *         An input of zero is outputted as a single '0' digit.
 */
unsigned int EZBL_itoa(int i, void *dest);
static unsigned int __attribute__((weakref("EZBL_itoa"))) EZBL_itoaW(int i, void *dest);


/**
 * Converts a signed 32-bit long integer into an ASCII decimal (base 10)
 * string representation. The string is NOT null terminated, but leading zeros
 * are omitted.
 *
 * For negative numbers, a leading '-' negative sign is written. Zero and
 * positive values do not have a leading sign character applied.
 *
 * An ASCII decimal string is essentially Binary Coded Decimal, with each 0-9
 * digit encoded into an 8-bit field and with the '0' (0x30) character offset
 * value added.
 *
 * @param i signed (up to) 32-bit integer to write in ASCII form.
 *
 * @param *dest Pointer to a memory buffer to receive the generated ASCII
 *              characters. This buffer should be at least 11 bytes long when
 *              converting integers <= -1 billion, 10 bytes for > -999 million
 *              or >= 1 billion, 9 bytes for <= -10 million or >= 100 million,
 *              etc.
 *
 *              Characters are written in print form, i.e. sign (only if
 *              negative), followed by most significant character first.
 *
 *              Each buffer byte will be internally written up to 11 times, so
 *              do not specify a peripheral SFR pointer that will do something
 *              in hardware when written with arbitrary contents.
 *
 * @return Number of ASCII characters written to *dest, including the negative
 *         sign if applicable. The value will always be >= 1 and <= 11. Values
 *         less than 11 indicate the number had leading positive sign and/or
 *         zero digits that were omitted from being written.
 *
 *         An input of zero is outputted as a single '0' digit.
 */
unsigned int EZBL_ltoa(long i, void *dest);
static unsigned int __attribute__((weakref("EZBL_ltoa"))) EZBL_ltoaW(long i, void *dest);


/**
 * Converts a signed 64-bit long long integer into an ASCII decimal (base 10)
 * string representation. The string is NOT null terminated, but leading zeros
 * are omitted.
 *
 * For negative numbers, a leading '-' negative sign is written. Zero and
 * positive values do not have a leading sign character applied.
 *
 * An ASCII decimal string is essentially Binary Coded Decimal, with each 0-9
 * digit encoded into an 8-bit field and with the '0' (0x30) character offset
 * value added.
 *
 * @param i signed (up to) 64-bit integer to write in ASCII form.
 *
 * @param *dest Pointer to a memory buffer to receive the generated ASCII
 *              characters. This buffer should be at least 21 bytes long when
 *              converting integers <= -1*10^19, 20 bytes for > -1*10^19 or >=
 *              1*10^19, 19 bytes for > -1*10^18 or 1*10^18, etc.
 *
 *              Characters are written in print form, i.e. sign (only if
 *              negative), followed by most significant character first.
 *
 *              Each buffer byte will be internally written up to 11 times, so
 *              do not specify a peripheral SFR pointer that will do something
 *              in hardware when written with arbitrary contents.
 *
 * @return Number of ASCII characters written to *dest, including the negative
 *         sign if applicable. The value will always be >= 1 and <= 21. Values
 *         less than 21 indicate the number had leading positive sign and/or
 *         zero digits that were omitted from being written.
 *
 *         An input of zero is outputted as a single '0' digit.
 */
unsigned int EZBL_lltoa(unsigned long i, void *dest);
static unsigned int __attribute__((weakref("EZBL_lltoa"))) EZBL_lltoaW(unsigned long i, void *dest);


#if defined(__XC16__)
/**
 * Prints a given RAM/SFR data range in human readable hexadecimal words to
 * stdout.
 *
 * Use the UART_Reset() or I2C_Reset() macros to initialize the _write handler
 * for stdout. Also, for UARTs, ensure the TX I/O pin has a PPS assignment for
 * it, assuming it needs one. Ex:
 *         // Output on U2TX pin assigned to RP45 at 460800 baud, 8N1, with a
 *         // system clock of 70MIPS, and set this UART interface as the stdio
 *         // target.
 *         _RP45R = _RPOUT_U1TX;
 *         UART_Reset(2, 70000000, 460800, 1);
 *
 * @param startPtr Starting memory address pointer to begin reading and
 *                 printing. This value can be odd (high byte in a 16-bit
 *                 word).
 *
 *                 Pointers to 0x8000 and above will point to PSVPAG/DSRPAG
 *                 space (typically a 32KB chunk of flash contents instead of
 *                 RAM).
 *
 * @param byteLen   Number of bytes at startPtr to print
 */
void EZBL_DumpRAM(const void *startPtr, unsigned int byteLen);


/**
 * Prints a given flash/program space range in human readable hexadecimal words
 * to stdout.
 *
 * Use the UART_Reset() or I2C_Reset() macros to initialize the _write handler
 * for stdout. Also, for UARTs, ensure the TX I/O pin has a PPS assignment for
 * it, assuming it needs one. Ex:
 *         // Output on U2TX pin assigned to RP45 at 460800 baud, 8N1, with a
 *         // system clock of 70MIPS, and set this UART interface as the stdio
 *         // target.
 *         _RP45R = _RPOUT_U1TX;
 *         UART_Reset(2, 70000000, 460800, 1);
 *
 * @param startAddr Starting memory address to begin reading and printing. This
 *                  value should be even since program memory addresses always
 *                  increment by 0x2 addresses.
 *
 *                  Must be a legal memory address for the device (address
 *                  beyond the end of flash up to 0x7FFFFF will typically cause
 *                  an Address Error Trap).
 *
 * @param endAddr   End memory address to stop printing when reached. This
 *                  address is NOT printed (i.e. set this to one location after
 *                  the memory of interest).
 */
void EZBL_DumpFlash(unsigned long startAddr, unsigned long endAddr);


#elif defined(__XC32__) || defined(__PIC32__)

/**
 * Wrapper macro that calls EZBL_DumpFlashDiff(). On PIC32 devices, RAM and flash
 * share the same addressable memory space and both have a 32-bit word size.
 */
#define EZBL_DumpRAM(startPtr, byteLen)     EZBL_DumpFlashDiff(0, 0, ((unsigned int)(startPtr)), 0, 0, (unsigned int)(byteLen))

/**
 * Prints a given flash/program space range in human readable hexadecimal words
 * to stdout.
 *
 * Use the UART_Reset() or I2C_Reset() macros to initialize the _write handler
 * for stdout. Also, for UARTs, ensure the TX I/O pin has a PPS assignment for
 * it, assuming it needs one. Ex:
 *         // Output on U2TX pin assigned to RP45 at 460800 baud, 8N1, with a
 *         // system clock of 70MIPS, and set this UART interface as the stdio
 *         // target.
 *         _RP45R = _RPOUT_U1TX;
 *         UART_Reset(2, 70000000, 460800, 1);
 *
 * @param startAddr unsigned int starting memory address to begin reading and
 *                  printing. This value should be 32-bit aligned (i.e. 2 LSbits
 *                  == '0') and be a kseg0 or kseg1 address, not a physical
 *                  address. Set bit 31 to make a kseg0 address from a physical
 *                  address.
 *
 *                  Must be a legal memory address for the device (addresses
 *                  outside the implemented address ranges will normally cause a
 *                  Bus Error Exception: load or store (cause = 0x7).
 *
 * @param endAddr   unsigned int end memory address to stop printing when
 *                  reached. This address is NOT printed (i.e. set this to one
 *                  location after the memory of interest).
 */
#define EZBL_DumpFlash(startAddr, endAddr)  EZBL_DumpFlashDiff(0, 0, (unsigned int)(startAddr), 0, 0, ((unsigned int)(endAddr)) - ((unsigned int)(startAddr)))
#endif


/**
 * On stdout, displays one packed RAM byte array of flash instruction words
 * compared against a second RAM byte array of instruction words. Differences
 * are highlighted in red on a per-instruction basis on ANSI compatible terminal
 * emulation software.
 *
 * Data is displayed for both arrays an hex ASCII word groups of 24-bits each
 * (little endian). Ex:
 *
 * EZBL_DumpFlashDiff("Expect: ", "Read-back: ", 0x001004, &expectData[0], &readData[0], sizeof(expectData));
 *
 *   Expect:    001000                FDC000 60036F DE0044 E1306A AF0042 434307
 *              001010  436C03 E11C18 AE2042 EB0100 540402 E90285 320003 628363
 *              001020  3AFFF2 37FFED E9014F 57807C 540200 AE0042 E80204 97B41F
 *              001030  510284 3C0002 A14001 370011 E90105 200207 A34801 3A000D
 *              001040  A33801 320009
 *   Read-back: 001000                FDC000 60036F DE0044 E1306A AF0042 434307
 *              001010  436C03 E11C18 AE2000 EB0100 540402 E90285 320003 628363
 *              001020  3AFFF2 37FFED E9014F 57807C 540200 AE0042 E80204 97B41F
 *              001030  510284 3C0002 A14001 370011 E90105 200207 A34801 3A000D
 *              001040  A338FF 320009
 *
 *
 * @param refMsgPreFix Null terminated string to prefix blocks of the reference
 *                     array during printing. Ex: "Expect: ".
 *
 *                     This parameter can be null if no prefixing is desired.
 *
 *                     Space padding is added such that refMsgPreFix and
 *                     cmpMsgPreFix are both the same length and the printed
 *                     data is easily viewed in columns.
 *
 * @param cmpMsgPreFix Null terminated string to prefix blocks of the compared
 *                     array during printing. Ex: "Read-back: ".
 *
 *                     This parameter can be null if no prefixing is desired.
 *
 *                     Space padding is added such that refMsgPreFix and
 *                     cmpMsgPreFix are both the same length and the printed
 *                     data is easily viewed in columns.
 *
 * @param addr Program memory start address to display in the right-most column
 *             of each line. This address is not used to do a flash read-back
 *             but instead is merely used to make sense of each line's compared
 *             data within the physical device.
 *
 *             Each line increments by 0x10 program addresses (8 instruction
 *             words), and a non-multiple of 0x10 is allowed for addr. In such a
 *             case, the address will become aligned and a gap of spaces will be
 *             inserted to align all subsequent columns.
 *
 *             addr should be an even number (odd numbers will have the LSbit
 *             cleared internally for address printing).
 *
 * @param dataRef RAM located byte array in packed form where each 3 bytes
 *                represents one instruction word. This pointer defines the dump
 *                data which will always be neutrally colored (i.e. white text).
 *
 *                This parameter cannot be null unless byteLen is zero.
 *
 * @param dataCmp Compare data located in RAM as a RAM byte array synonymous
 *                with the dataRef pointer. However, any mismatched instruction
 *                words will be displayed in red text.
 *
 *                If a regular dump of flash is desired without any compare
 *                data, this parameter can be null.
 *
 * @param byteLen Number of bytes in both the dataRef and dataCmp (when
 *                applicable) arrays to display/compare. This length can be a
 *                non-multiple of 3 bytes.
 *
 * @return void.
 *
 *         dataRef and dataCmp (when not null) is printed to stdout, as defined
 *         by the write() XC16 function or _mon_putc() XC32 function.
 */
void EZBL_DumpFlashDiff(const char *refMsgPreFix, const char *cmpMsgPreFix, unsigned long addr, const void *dataRef, const void *dataCmp, unsigned int byteLen);


/**
 * Initializes or re-initializes an EZBL_FIFO structure with zero bytes of data,
 * and a specified backing memory.
 *
 * @param *fifo pointer to an EZBL_FIFO structure to reset.
 * @param *buffer pointer to a RAM array to store the FIFO's data to.
 * @param bufferLen Number of bytes at the *buffer location to allocate to the
 *                  FIFO.
 * @param *onWriteCallback() pointer to a callback function to call anytime 0 or
 *        more bytes of data is written to the FIFO via public APIs
 *        (EZBL_FIFOWrite(), EZBL_FIFOWrite8(), EZBL_FIFOprintf(), etc.). Set to
 *        null if unneeded. This function is called after all data has been
 *        written. Set to null ((void*)0) if you do not need onWrite
 *        notification.
 * @param *onReadCallback() pointer to a callback function to call anytime 0 or
 *        more bytes are read or peeked from the FIFO via public APIs. This
 *        function is called after all data has been read into the callers
 *        buffer (or discarded for a null read buffer output pointer).
 *
 *        Set to null ((void*)0) if you do not need onRead/onPeek notification.
 *
 * @return fifo's dataCount, fifoSize, fifoRAM, headPtr, and tailPtr elements
 *         are all reset. The activity flags are all cleared, except for 'other'
 *         which is set to '1'.
 *
 *         Note, the memory contained in the physical buffer memory is not
 *         zeroed, so any sensitive data that might be contained in them should
 *         be zeroed externally.
 */
void EZBL_FIFOReset(EZBL_FIFO *fifo, void *buffer, unsigned int bufferLen, unsigned int (*onWriteCallback)(unsigned int bytesWritten, void *writeData, unsigned int dataLen, EZBL_FIFO *writeFIFO), unsigned int (*onReadCallback)(unsigned int bytesRead, void *readData, unsigned int dataLen, EZBL_FIFO *readFIFO));


/**
 * Writes data into a circular EZBL_FIFO RAM buffer from a normal data array,
 * updating the headPtr and dataCount in the FIFO structure. dataCount is
 * updated atomically without disabling interrupts.
 *
 * After the write is complete and the headPtr/dataCount FIFO parameters
 * updated, an optional destFIFO->onWriteCallback() function pointer is called.
 * This callback is invoked only when the pointer is non-null.
 *
 * The onWrite callback function must have type:
 *      unsigned int yourOnWriteFuncName(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO);
 * The return value of this callback is returned unchanged by EZBL_FIFOWrite(),
 * giving you an opportunity to manipulate the FIFO contents, return value,
 * and/or trigger other code to start reading the data out of the FIFO (ex: to a
 * UART TX or other communications ISR). The bytesPushed parameter specifies the
 * byte count actually added to the FIFO, *writeSrc is the original value of
 * *source, and reqWriteLen is the original value of dataLen.
 *
 * Note: the onWrite callback is called after the write has already taken place.
 * Therefore, any manipulation done to the writeSrc array will have no effect on
 * the FIFO. However, as writing to the FIFO does not destroy or manipulate the
 * source data, it is possible to re-read the source data from the original
 * pointer if you wish to save the data elsewhere (ex: a second FIFO connected
 * to a debug terminal or other "communications tee").
 *
 * @param *destFIFO EZBL_FIFO pointer to write the copied data to. FIFO overflow
 *                  is supressed. FIFO wraparound and write pointer update is
 *                  internally handled.
 *
 *                  If this pointer is null (0x0000), all processing is skipped
 *                  and no data is read from *source. However, EZBL_FIFOWrite()
 *                  returns dataLen, emulating an infinite bit-bucket.
 * @param *source RAM or PSV pointer to read data from. Address can be byte
 *                aligned. This pointer cannot be null except when <i>length</i>
 *                is 0.
 * @param dataLen Number of bytes to copy. This value must be <= 16384 on
 *                dsPIC30, PIC24F, PIC24H, or dsPIC33F families.
 *
 *                Zero is allowed, resulting in no write operation. However, the
 *                destFIFO->onWriteCallback() function will still be called, if
 *                not null.
 *
 * @return Value returned by destFIFO->onWriteCallback() or the number of bytes
 *         written to the FIFO. When unchanged by the callback, this parameter
 *         will be less than <i>length</i> only when the FIFO becomes completely
 *         full and the write is terminated early to avoid FIFO overflow.
 *
 *         If *destFIFO is null, dataLen is returned, emulating a successful
 *         write.
 */
unsigned int EZBL_FIFOWrite(EZBL_FIFO *destFIFO, const void *source, unsigned int dataLen);

static unsigned int __attribute__((weakref("EZBL_FIFOWrite"))) EZBL_FIFOWriteW(EZBL_FIFO *destFIFO, const void *source, unsigned int dataLen);  // Weak reference to EZBL_FIFOWrite()

// Wrapper for EZBL_FIFOWrite() that accepts 8-bits as inline parameter data instead of an array pointer.
static inline unsigned int __attribute__((always_inline)) EZBL_FIFOWrite8(EZBL_FIFO *dest, unsigned char writeData)
{
    return EZBL_FIFOWrite(dest, &writeData, 1);
}
// Wrapper for EZBL_FIFOWrite() that accepts 16-bits as inline parameter data instead of an array pointer.
static inline unsigned int __attribute__((always_inline)) EZBL_FIFOWrite16(EZBL_FIFO *dest, unsigned int writeData)
{
    return EZBL_FIFOWrite(dest, &writeData, 2);
}
// Wrapper for EZBL_FIFOWrite() that accepts 24-bits as inline parameter data instead of an array pointer.
static inline unsigned int __attribute__((always_inline)) EZBL_FIFOWrite24(EZBL_FIFO *dest, unsigned long writeData)
{
    return EZBL_FIFOWrite(dest, &writeData, 3);
}
// Wrapper for EZBL_FIFOWrite() that accepts 32-bits as inline parameter data instead of an array pointer.
static inline unsigned int __attribute__((always_inline)) EZBL_FIFOWrite32(EZBL_FIFO *dest, unsigned long writeData)
{
    return EZBL_FIFOWrite(dest, &writeData, 4);
}
// Wrapper for EZBL_FIFOWrite() that accepts 48-bits as inline parameter data instead of an array pointer.
static inline unsigned int __attribute__((always_inline)) EZBL_FIFOWrite48(EZBL_FIFO *dest, unsigned long long writeData)
{
    return EZBL_FIFOWrite(dest, &writeData, 6);
}
// Wrapper for EZBL_FIFOWrite() that accepts 64-bits as inline parameter data instead of an array pointer.
static inline unsigned int __attribute__((always_inline)) EZBL_FIFOWrite64(EZBL_FIFO *dest, unsigned long long writeData)
{
    return EZBL_FIFOWrite(dest, &writeData, 8);
}
// Wrapper for EZBL_FIFOWrite() that accepts a null terminated string. The null terminator is not written to the FIFO.
static inline unsigned int __attribute__((always_inline)) EZBL_FIFOWriteStr(EZBL_FIFO *dest, const char *writeData)
{
    return EZBL_FIFOWrite(dest, writeData, __builtin_strlen(writeData));   // Do not write null terminator
}


/**
 * Prints formatted text to an EZBL_FIFO using a variable length argument list.
 *
 * This function internally calls EZBL_fprintf(). See the compiler libraries
 * printf() documentation for full formatting options.
 *
 * @param ezbl_fifo_dest (EZBL_FIFO*) pointer to an EZBL_FIFO to write the
 *                       output to. This parameter will route unchanged though
 *                       the _write() function.
 * @param *format Constant formatting string for printf(). See compiler library
 *                documentation on printf() for full details.
 * @param ...     Zero or more additional variables to be printed when
 *                encountered in the format string.
 *
 * @return Number of bytes successfully generated and written to the FIFO. This
 *         will be less than the number of bytes the printf() call should have
 *         generated if the FIFO becomes full.
 */
#define EZBL_FIFOprintf(ezbl_fifo_dest, format, ...)    EZBL_fprintf(ezbl_fifo_dest, format, __VA_ARGS__)


/**
 * Reads and removes data from a circular EZBL_FIFO, outputting to a normal RAM
 * array. The tailPtr is incremented and dataCount atomically reduced without
 * needing to disable interrupts.
 *
 * After the data has been removed from the FIFO and copied to *dest, an
 * optional call to the source->onReadCallback() function is made. This call is
 * made only when *onReadCallback is not null.
 *
 * The onRead callback function must have type:
 *      unsigned int yourOnReadFuncName(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *srcFIFO);
 * The return value of this callback is returned unchanged by EZBL_FIFORead(),
 * giving you an opportunity to manipulate the read contents, return value,
 * and/or trigger other code to refill any FIFO free space generated by the
 * read. The bytesPulled parameter specifies the byte count removed from the
 * FIFO, *readDest is the original value of *dest, and reqReadLen is the
 * original value of dataLen, and *srcFIFO is the pointer to the FIFO that is
 * calling the callback (so one callback handler function could theoretically
 * handle multiple different FIFOs simultaneously).
 *
 * Note: when implementing the onReadCallback(), *readDest can be null if *dest
 * was null when EZBL_FIFORead() was called. In such cases, the read data was
 * thrown away and cannot be reobtained. Technically the data is still in the
 * FIFO backing RAM and could be read for real through direct access, but since
 * the source->dataCount and tailPtr values have already been updated by the
 * time the callback is called, the data is subject to being overwritten by any
 * asynchronous events that may write to the FIFO. This ordering exists to allow
 * APIs that write to the FIFO to be called directly in the onReadCallback().
 *
 * @param *dest Pointer to write the output data to. If null, no read is
 *              performed but the tailPtr, dataCount, and return value will
 *              still be set as if a normal read took place, effectively
 *              throwing the buffer data away.
 *
 *              If source->onReadCallback() is non-null, it will still be called
 *              for null *dest values. However, in this special case, the
 *              reqReadLen callback parameter is adjusted to match the actual
 *              number of bytes thrown away rather than the original dataLen
 *              value this API was called with. This adjustment allows the
 *              callback to ignore read requests for excess data that is just
 *              being thrown away (ex: EZBL_FIFORead(0, &fifo, 0xFFFF) to flush
 *              the existing FIFO contents without blocking to perform an actual
 *              read of 65535 bytes of data).
 *
 * @param *srcFIFO EZBL_FIFO structure to read data from. Read underflow is
 *                 suppressed.
 *
 *                 If this pointer is null, the function returns immediately
 *                 with 0.
 *
 * @param dataLen Number of bytes to read. This value must be <= 16384 on
 *                dsPIC30, PIC24F, PIC24H, or dsPIC33F families.
 *
 *                Zero length is allowed, resulting in no read operation, but
 *                the source->onReadCallback() callback is still called, if
 *                defined.
 *
 * @return Return value from the source->onReadCallback() function or the number
 *         of bytes read and removed from the FIFO when no callback is defined.
 *         Unless changed by the callback routine, this parameter will be less
 *         than <i>length</i> only if the FIFO has run out of data to read.
 */
unsigned int EZBL_FIFORead(void *dest, EZBL_FIFO *srcFIFO, unsigned int dataLen);

static unsigned int __attribute__((weakref("EZBL_FIFORead"))) EZBL_FIFOReadW(void *dest, EZBL_FIFO *srcFIFO, unsigned int dataLen); // Weak reference to EZBL_FIFORead()

/**
 * Reads one byte from an EZBL_FIFO and returns it inline.
 *
 * If the FIFO does not have a byte available, this function may block until one
 * becomes available, depending on the source FIFO's onRead() callback
 * implementation.
 *
 * @param *source Pointer to the EZBL_FIFO to read from.
 *
 *                If this pointer is null, no data is read and an undefined
 *                value is returned.
 *
 * @return 8-bit unsigned char data read from the FIFO's tail pointer.
 */
static inline unsigned char __attribute__((always_inline, optimize(1))) EZBL_FIFORead8(EZBL_FIFO *source)
{
    unsigned char readData;
    EZBL_FIFORead(&readData, source, 1);
    return readData;
}


/**
 * Reads two bytes from an EZBL_FIFO and returns it inline as an unsigned short.
 *
 * If the FIFO does not have 2 byte available, this function may block until
 * they become available, depending on the source FIFO's onRead() callback
 * implementation.
 *
 * @param *source Pointer to the EZBL_FIFO to read from.
 *
 *                If this pointer is null, no data is read and an undefined
 *                value is returned.
 *
 * @return 16-bit unsigned short data read from the FIFO's tail pointer.
 */
static inline unsigned short __attribute__((always_inline, optimize(1))) EZBL_FIFORead16(EZBL_FIFO *source)
{
    unsigned short readData;
    EZBL_FIFORead(&readData, source, 2);
    return readData;
}


/**
 * Reads three bytes from an EZBL_FIFO and returns it inline as an unsigned
 * long. The upper-most byte is set to zero.
 *
 * If the FIFO does not have 3 byte available, this function may block until
 * they become available, depending on the source FIFO's onRead() callback
 * implementation.
 *
 * @param *source Pointer to the EZBL_FIFO to read from.
 *
 *                If this pointer is null, no data is read and an undefined
 *                value is returned.
 *
 * @return 24-bits of data returned as an unsigned long read from the FIFO's
 *         tail pointer. Bits 24-31 are always zero.
 */
static inline unsigned long __attribute__((always_inline, optimize(1))) EZBL_FIFORead24(EZBL_FIFO *source)
{
    unsigned long readData;
    EZBL_FIFORead(&readData, source, 3);
    ((unsigned char *)&readData)[3] = 0x00;
    return readData;
}


/**
 * Reads 4 bytes from an EZBL_FIFO and returns it inline as an unsigned
 * long.
 *
 * If the FIFO does not have 4 byte available, this function may block until
 * they become available, depending on the source FIFO's onRead() callback
 * implementation.
 *
 * @param *source Pointer to the EZBL_FIFO to read from.
 *
 *                If this pointer is null, no data is read and an undefined
 *                value is returned.
 *
 * @return 32-bits of data returned as an unsigned long read from the FIFO's
 *         tail pointer.
 */
static inline unsigned long __attribute__((always_inline, optimize(1))) EZBL_FIFORead32(EZBL_FIFO *source)
{
    unsigned long readData;
    EZBL_FIFORead(&readData, source, 4);
    return readData;
}


/**
 * Reads 6 bytes from an EZBL_FIFO and returns it inline as an unsigned
 * long long.
 *
 * If the FIFO does not have 6 byte available, this function may block until
 * they become available, depending on the source FIFO's onRead() callback
 * implementation.
 *
 * @param *source Pointer to the EZBL_FIFO to read from.
 *
 *                If this pointer is null, no data is read and an undefined
 *                value is returned.
 *
 * @return 48-bits of data returned as an unsigned long long read from the
 *         FIFO's tail pointer. Bits 48-63 are always zero.
 */
static inline unsigned long long __attribute__((always_inline, optimize(1))) EZBL_FIFORead48(EZBL_FIFO *source)
{
    unsigned long long readData;
    EZBL_FIFORead(&readData, source, 6);
    ((unsigned char *)&readData)[6] = 0x00;
    ((unsigned char *)&readData)[7] = 0x00;
    return readData;
}


/**
 * Reads 8 bytes from an EZBL_FIFO and returns it inline as an unsigned
 * long long.
 *
 * If the FIFO does not have 8 byte available, this function may block until
 * they become available, depending on the source FIFO's onRead() callback
 * implementation.
 *
 * @param *source Pointer to the EZBL_FIFO to read from.
 *
 *                If this pointer is null, no data is read and an undefined
 *                value is returned.
 *
 * @return 64-bits of data returned as an unsigned long long read from the
 *         FIFO's tail pointer.
 */
static inline unsigned long long __attribute__((always_inline, optimize(1))) EZBL_FIFORead64(EZBL_FIFO *source)
{
    unsigned long long readData;
    EZBL_FIFORead(&readData, source, 8);
    return readData;
}


/**
 * Peeks at the data in a circular EZBL_FIFO, outputting to a normal RAM array.
 * FIFO state and contents are left unchanged, however, the onRead() callback
 * function for the FIFO, if set, is still called.
 *
 * @param *dest Pointer to write the output data to.
 *
 *              Null is allowed, but results in no operation occurring.
 *
 * @param *srcFIFO EZBL_FIFO structure to peek data from. Read underflow is
 *                 suppressed.
 *
 *                 If this pointer is null, the function returns immediately
 *                 with 0.
 *
 * @param dataLen Number of bytes to read.
 *
 *                0 is allowed, resulting in no operation.
 *
 * @return Number of bytes peeked from the FIFO. This parameter will be less
 *         than <i>length</i> only if the peeking pointer has reached the end of
 *         all available data.
 */
unsigned int EZBL_FIFOPeek(void *dest, EZBL_FIFO *srcFIFO, unsigned int dataLen);

static unsigned int __attribute__((weakref("EZBL_FIFOPeek"))) EZBL_FIFOPeekW(void *dest, EZBL_FIFO *source, unsigned int length);   // Weak reference to the EZBL_FIFOPeek() function.

// Wrapper for EZBL_FIFOPeek() that returns 8-bits as inline parameter data instead of writing to an array pointer.
static inline unsigned char __attribute__((always_inline, optimize(1))) EZBL_FIFOPeek8(EZBL_FIFO *source)
{
    unsigned char peekData;
    EZBL_FIFOPeek(&peekData, source, 1);
    return peekData;
}
// Wrapper for EZBL_FIFOPeek() that returns 16-bits as inline parameter data instead of writing to an array pointer.
static inline unsigned short __attribute__((always_inline, optimize(1))) EZBL_FIFOPeek16(EZBL_FIFO *source)
{
    unsigned short peekData;
    EZBL_FIFOPeek(&peekData, source, 2);
    return peekData;
}
// Wrapper for EZBL_FIFOPeek() that returns 24-bits as inline parameter data instead of writing to an array pointer. Bits 24-31 are always zero.
static inline unsigned long __attribute__((always_inline, optimize(1))) EZBL_FIFOPeek24(EZBL_FIFO *source)
{
    unsigned long peekData;
    EZBL_FIFOPeek(&peekData, source, 3);
    ((unsigned char *)&peekData)[3] = 0x00;
    return peekData;
}
// Wrapper for EZBL_FIFOPeek() that returns 32-bits as inline parameter data instead of writing to an array pointer.
static inline unsigned long __attribute__((always_inline, optimize(1))) EZBL_FIFOPeek32(EZBL_FIFO *source)
{
    unsigned long peekData;
    EZBL_FIFOPeek(&peekData, source, 4);
    return peekData;
}
// Wrapper for EZBL_FIFOPeek() that returns 48-bits as inline parameter data instead of writing to an array pointer. Bits 48-63 are always zero.
static inline unsigned long long __attribute__((always_inline, optimize(1))) EZBL_FIFOPeek48(EZBL_FIFO *source)
{
    unsigned long long peekData;
    EZBL_FIFOPeek(&peekData, source, 6);
    ((unsigned char *)&peekData)[6] = 0x00;
    ((unsigned char *)&peekData)[7] = 0x00;
    return peekData;
}
// Wrapper for EZBL_FIFOPeek() that returns 64-bits as inline parameter data instead of writing to an array pointer.
static inline unsigned long long __attribute__((always_inline, optimize(1))) EZBL_FIFOPeek64(EZBL_FIFO *source)
{
    unsigned long long peekData;
    EZBL_FIFOPeek(&peekData, source, 8);
    return peekData;
}


/**
 * Sets the IECx<y> Interrupt Enable bit for the given FIFO. The quantities of
 * x and y are computed based on fifo->irqNum.
 *
 * IECx register bit change is done atomically.
 *
 * @param *fifo Pointer to the FIFO to enable the interrupt on.
 *
 * @return Prior IECx Interrupt Enable state, not taking into account global
 *         interrupt enable/disable states, such as a DISI instruction or
 *         GIE == 0 condition.
 *          <ul>
 *              <li>0 = Interrupt Enable bit was previously clear</li>
 *              <li>1 = Interrupt Enable bit was previously set</li>
 *          </ul>
 */
int EZBL_FIFOIntEnable(EZBL_FIFO *fifo);


/**
 * Clears the IECx<y> Interrupt Enable bit for the given FIFO. The quantities of
 * x and y are computed based on fifo->irqNum.
 *
 * IECx register bit change is done atomically.
 *
 * @param *fifo Pointer to the FIFO to disable the interrupt on.
 *
 * @return Prior IECx Interrupt Enable state, not taking into account global
 *         interrupt enable/disable states, such as a DISI instruction or
 *         GIE == 0 condition.
 *          <ul>
 *              <li>0 = Interrupt Enable bit was previously clear</li>
 *              <li>1 = Interrupt Enable bit was previously set</li>
 *          </ul>
 */
int EZBL_FIFOIntDisable(EZBL_FIFO *fifo);


/**
 * Returns the IECx<y> Interrupt Enable bit for the given FIFO. The quantities
 * of x and y are computed based on fifo->irqNum.
 *
 * @param *fifo Pointer to the FIFO to return the Interrupt Enable status on.
 *
 * @return IECx Interrupt Enable state, not taking into account global
 *         interrupt enable/disable states, such as a DISI instruction or
 *         GIE == 0 condition.
 *          <ul>
 *              <li>0 = Interrupt Disabled</li>
 *              <li>1 = Interrupt Enabled</li>
 *          </ul>
 */
int EZBL_FIFOGetIntEn(EZBL_FIFO *fifo);


/**
 * Clears the IFSx<y> Interrupt Flag bit for the given FIFO. The quantities of x
 * and y are computed based on the fifo->irqNum with the operation occuring
 * atomically.
 *
 * @param *fifo Pointer to the FIFO to clear the interrupt flag on.
 *
 * @return Prior IFSx Interrupt Flag state
 */
int EZBL_FIFOIntClear(EZBL_FIFO *fifo);


/**
 * Sets the IFSx<y> Interrupt Flag bit for the given FIFO. The quantities of
 * x and y are computed based on fifo->irqNum.
 *
 * IFSx register bit change is done atomically.
 *
 * @param *fifo Pointer to the FIFO to set the interrupt flag on.
 *
 * @return Prior IFSx Interrupt Flag state
 *          <ul>
 *              <li>0 = No interrupt was pending</li>
 *              <li>1 = Interrupt was already pending</li>
 *          </ul>
 */
int EZBL_FIFOIntRaise(EZBL_FIFO *fifo);


/**
 * Returns the IFSx<y> Interrupt Flag bit for the given FIFO. The quantities of
 * x and y are computed based on fifo->irqNum.
 *
 * @param *fifo Pointer to the FIFO to return the flag status on.
 *
 * @return  <ul>
 *              <li>0 = No interrupt pending</li>
 *              <li>1 = Interrupt pending</li>
 *          </ul>
 */
int EZBL_FIFOGetInt(EZBL_FIFO *fifo);


/**
 * Sets or clears the IECx<y> Interrupt Enable bit for the given FIFO. The
 * quantities of x and y are computed based on fifo->irqNum.
 *
 * IECx register bit change is done atomically.

 * @param *fifo Pointer to the FIFO to enable or disable interrupts on.
 *
 * @param enable 1 - Sets IECx<y> to enables the interrupt
 *               0 - Clears IECx<y> to disable the interrupt
 *
 * @return  Prior IECx<y> bit state.
 */
int EZBL_FIFOIntEnableSet(EZBL_FIFO *fifo, int enable);


/**
 * Sets or clears the IFSx<y> Interrupt Flag bit for the given FIFO. The
 * quantities of x and y are computed based on fifo->irqNum.
 *
 *  IFSx register bit change is done atomically.
 *
 * @param *fifo Pointer to the FIFO to enable or disable interrupts on.
 *
 * @param enable 1 - Sets IECx<y> to enables the interrupt
 *               0 - Clears IECx<y> to disable the interrupt
 *
 * @return  Prior IECx<y> bit state.
 */
int EZBL_FIFOIntFlagSet(EZBL_FIFO *fifo, int flag);


/**
 * Sets the IPCx<y> Interrupt Priority bitfield for the given FIFO. The
 * quantities of x and y are computed based on fifo->irqNum.
 *
 * @param *fifo Pointer to the FIFO to set the interrupt priority on.
 *
 * @param newPriority CPU IPL priority level to assign to the FIFO's interrupt.
 *                    0 is the lowest used to execute the main() context
 *                    (typically means the interrupt is disabled). While 7 is
 *                    the highest priority.
 *
 *                    Set this parameter to 0-7 (i.e. bits LSb-justified)
 *                    regardless of original IPCx bitfield packing position.
 *
 * @return Prior IPCx Interrupt Priority bits for this FIFO with all other bits
 *         sharing the register masked off. The bitfield is always LSb
 *         justified (i.e. starts in bit position 0, regardless of original
 *         position in the IPCx register).
 */
unsigned int EZBL_FIFOSetIntPri(EZBL_FIFO *fifo, unsigned int newPriority);


/**
 * Returns the IPCx<y> Interrupt Priority bitfield for the given FIFO. The
 * quantities of x and y are computed based on fifo->irqNum.
 *
 * @param *fifo Pointer to the FIFO to return the interrupt priority on
 *
 * @return IPCx Interrupt Priority bits for this FIFO with all other bits
 *         sharing the register masked off. The bitfield is always LSb
 *         justified (i.e. starts in bit position 0, regardless of original
 *         position in the IPCx register).
 */
 unsigned int EZBL_FIFOGetIntPri(EZBL_FIFO *fifo);

 
/**
 * Sets the baud rate associated with a UART TX or RX EZBL_FIFO and/or enables
 * hardware auto-baud. Existing FIFO contents are unchanged.
 *
 * @param *TxRxFifo Pointer to an EZBL_FIFO structure associated with a UART TX
 *                  or UART RX FIFO. Usually, this will be a &UARTx_TxFifo or
 *                  &UARTx_RxFifo reference, or a saved pointer like
 *                  EZBL_STDOUT or EZBL_COM_RX. Typically these are instanced by
 *                  a prior call to the UART_Reset() macro.
 *
 *                  The TxRxFifo->irqNum parameter must be set before calling
 *                  this function so that the correct UxBRG register (and
 *                  optionally UxMODEbits.ABAUD bit) are referenced. This
 *                  parameter is automatically set if the FIFO was previously
 *                  initialized using UART_Reset(), UART_ResetBRG(), or
 *                  UART_ResetCalc().
 *
 *                  This pointer can be null, resulting in no operation.
 *                  However, this pointer must not point to a non-UART FIFO
 *                  type (doing so will result in data corruption of an
 *                  undefined UART's SFRs).
 *
 * @param newBaud   Desired baud rate to set the hardware UART peripheral to.
 *                  Set to 0 or a negative value to enable hardware auto-baud.
 *
 *                  A negative newBaud value is defined to mean: enable
 *                  auto-baud, but still compute and assign UxBRG to the
 *                  positive equivalent newBaud value. This typically allows TX
 *                  operations at a fixed, known baud rate until a 0x55 ('U') RX
 *                  character is received to redefine the baud rate.
 *
 *                  The NOW_Fcy global variable is referenced to run-time
 *                  compute the necessary UxBRG value based on the current
 *                  system clock setting. If newBaud == 0, this reference is
 *                  still generated, but the NOW_Fcy value is unused.
 *
 * @return Prior value contained in UxBRG of the UART that was updated. NOTE:
 *         this is returned as a signed integer, so if the hardware implements
 *         UxBRG with 16 or more bits on a 16-bit platform, UxBRG values > 32767
 *         will be returned as a negative value. Any higher order bits would not
 *         be returned.
 *
 *         The following status bits are written by this function:
 *              TxRxFifo->activity.other = 1;
 *              TxRxFifo->activity.framingError = 0;
 *              TxRxFifo->activity.parityError  = 0;
 *
 *         If TxRxFifo is a null pointer or TxRxFifo->irqNum is an illegal IRQ
 *         number (i.e. > 255), -1 is returned.
 */
int EZBL_FIFOSetBaud(EZBL_FIFO *TxRxFifo, long newBaud);


/**
 * Blocks until the FIFO holds 0 bytes, a timeout is reached, or an
 * implementation defined flush callback function returns.
 *
 * When the fifo->flushFunction pointer callback has been configured (set to a
 * non-null value), execution is passed to the callback and nothing is done
 * internally except return the callback value.
 *
 * Only when fifo->flushFunction is zero, this function internally blocks until
 * the FIFO contents reach zero or the specified NOW timeout is reached.
 *
 * @param *fifo Pointer to the FIFO to "flush". If null, no operation is
 *              performed.
 *
 * @param timeout Maximum length of time the flush operation can block for,
 *                specified in NOW counts.
 *
 *                Use a multiple of NOW_sec, NOW_ms, and/or
 *                NOW_us to choose this timeout.
 *
 *                The special value (unsigned long)-1, or 0xFFFFFFFF, indicates
 *                an infinite wait time.
 *
 * @return flushFunction()'s callback return value, or, when null:
 *         <ul>
 *              <li>1 - All bytes in the FIFO were flushed (0 bytes remain) or
 *                      fifo was a null pointer</li>
 *              <li>0 - Timeout occurred</li>
 *         </ul>
 */
int EZBL_FIFOFlush(EZBL_FIFO *fifo, unsigned long timeout);


///**
// * Reads data from one EZBL_FIFO and writes it into a second EZBL_FIFO. Data is
// * copied chunkwise so greatest efficiency is achieved with larger data sizes.
// *
// * @param *dest Destination EZBL_FIFO to write to. This parameter cannot be null
// *              unless dataLen is 0.
// * @param *source Source EZBL_FIFO to read from. This parameter cannot be null
// *              unless dataLen is 0.
// * @param dataLen Number of bytes to copy between the two FIFOs.
// *
// * @return Number of bytes successfully copied. This can be less than dataLen
// *         when the source FIFO runs out of data or the destination FIFO becomes
// *         full.
// */
//unsigned int EZBL_FIFOWriteFIFO(EZBL_FIFO *dest, EZBL_FIFO *source, unsigned int dataLen);


/**
 * Reads and removes data from one EZBL_FIFO and writes it to another EZBL_FIFO.
 *
 * This is a wrapper function that internally calls EZBL_FIFORead(), storing the
 * data linearly on the stack, then in turn calls EZBL_FIFOWrite(). Because the
 * stack is used to temporarily store all requested copy data, it is not
 * advisable to specify a very large dataLen unless you know that there is a
 * large amount of free space on the stack to safely complete the transfer.
 *
 * @param *destFIFO EZBL_FIFO pointer to write the copied data to. If the
 *                  srcFIFO underflows, the destFIFO will be written only with
 *                  the data actually returned during reading.
 *
 *                  Any applicable onWriteCallback will be triggered during one
 *                  or more write cycles.
 *
 *                  This pointer cannot be null (0x0000), even when
 *                  <i>dataLen</i> is 0.
 *
 * @param *srcFIFO EZBL_FIFO pointer to read from.
 *
 *                 Any applicable onReadCallback will be triggered during one
 *                 or more read cycles.
 *
 *                 This pointer cannot be null, even when <i>dataLen</i> is 0.
 *
 * @param copyLen Number of bytes to copy. This value must be smaller than the
 *                amount of free space on the stack, taking into account
 *                additional margin for the onRead and onWrite callbacks, plus
 *                any ISRs taking place.
 *
 *                Zero is allowed, resulting in no data transfer. However, the
 *                srcFIFO->onReadCallback() and destFIFO->onWriteCallback()
 *                function will still be called, if not null.
 *
 * @return Value returned by destFIFO->onWriteCallback() or the number of bytes
 *         written to the FIFO. When unchanged by the callback, this parameter
 *         will be less than <i>copyLen</i> when the source FIFO becomes empty
 *         or the destination FIFO becomes full.
 */
unsigned int EZBL_FIFO2FIFO(EZBL_FIFO *dest, EZBL_FIFO *source, unsigned int copyLen);

static unsigned int __attribute__((weakref("EZBL_FIFO2FIFO"))) EZBL_FIFO2FIFOW(EZBL_FIFO *dest, EZBL_FIFO *source, unsigned int copyLen);  // Weak version of EZBL_FIFO2FIFO()



#if defined(__XC16__)
/**
 * Polls the NVMCON WR bit until it is clear, indicating the NVMOp has completed
 * then clears NVMCON<WREN> and sets the NVMADRH:NVMADRL or transparent write
 * latch address to 0xFFFFFE before returning.
 *
 * @return void
 *
 *         NVMCON<WR> has returned to '0', NVMCON<WREN> is written to '0', write
 *         latch set to unimplemented address to prevent accidental harm if NVM
 *         erase/programming code executes without proper set up.
 */
void EZBL_WaitForNVMOpComplete(void);


#elif defined(__XC32__) || defined(__PIC32__)
static volatile unsigned int __attribute__((section("sfrs"), weakref("NVMCON")))    EZBL_NVMCON;
static volatile unsigned int __attribute__((section("sfrs"), weakref("NVMADDR")))   EZBL_NVMADDR;
/**
 * Macro polls the NVMCON WR bit until it is clear, indicating the NVMOp has 
 * completed. NVMCON<WREN> is then cleared and NVMADDR is written to the illegal
 * value 0x55555555 (to reduce risk of any inadvertent NVM operations) before
 * returning.
 *
 * @return NVMCON<WR> has returned to '0' and NVMCON<WREN> is written to '0'.
 */
static void inline __attribute__((always_inline, optimize(1))) EZBL_WaitForNVMOpComplete(void)
{
    while(EZBL_NVMCON & 0x8000);    // Poll NVMCON<WR> bit until clear
    (&EZBL_NVMCON)[1] = 0x4000;     // Clear NVMCON<WREN> for anti-bricking safety
    EZBL_NVMADDR = 0x55555555u;     // Put dummy illegal value in NVMADDR for more safety

}
#endif


/**
 * unsigned int EZBL_DisableInterrupts(void);
 *
 * Disables all interrupts, including IPL7 ones by setting the current CPU
 * execution level to IPL8 (IPL8 is allocated to the lowest priority Generic
 * Soft Error Trap).
 *
 * The return value should be saved and subsequently passed to
 * EZBL_RestoreInterrupts() to restore the original CPU IPL and interrupt
 * processing.
 *
 * This function differs from traditional methods of raising the CPU to IPL7 or
 * setting the INTCON2<GIE> bit to disable Interrupts because:
 *  - PIC24F/dsPIC33F/PIC24H devices don't have a GIE bit
 *  - Newer devices, like the dsPIC33EP64GS506 family, include W-REG array
 *    shadow sets which can be automatically invoked by setting the IPL to the
 *    preconfigured shadow context's activation level.
 * As IPL7 is a useful place to assign a shadow register context set and since
 * universal portability is desired, this function triggers a higher IPL8
 * state that cannot have a shadow register set assigned to it. Execution will
 * continue using the same context registers as were selected during the
 * EZBL_DisableInterrupts() call.
 *
 * As IPL8 is allocated as the Generic Soft Trap, any DO stack overflows,
 * Illegal opcodes executed, or other mupltiplexed traps implemented in INTCON3
 * will be queued but supressed after EZBL_DisableInterrupts() returns. When
 * EZBL_RestoreInterrupts() is called, any accumulated true Generic Soft Trap
 * events will trigger vectoring to the Generic Soft Trap interrupt handler.
 *
 * All other traps will continue to vector normally while interrupts are
 * disabled.
 *
 * This function executes in 13 cycles (dsPIC33E/PIC24E/dsPIC33C devices) or
 * 8 cycles (dsPIC30F/dsPIC33F/PIC24H/PIC24F devices), including the call and
 * return.
 *
 * @return Prior SR state (with IPL information) to be used by
 *         EZBL_RestoreInterrupts().
 *
 *         This function only uses w0. When calling from assembly, it is not
 *         necessary to preserve the w1-w7 registers.
 */
unsigned int EZBL_DisableInterrupts(void);


/**
 * void EZBL_RestoreInterrupts(unsigned int previousSRSave);
 *
 * Restores the prior CPU IPL setting returned from EZBL_DisableInterrupts(),
 * enabling any interrupts that were previously enabled. CORCON<IPL3> is
 * cleared.
 *
 * This function executes in 12 cycles (dsPIC33E/PIC24E/dsPIC33C devices) or
 * 7 cycles (dsPIC30F/dsPIC33F/PIC24H/PIC24F devices), including the call and
 * return.
 *
 * @param previousSRSave Original SR<IPL2:0> encoded value returned from
 *                       EZBL_DisableInterrupts();
 *
 * @return CORCON<IPL3> and SR<IPL> bits revert back to their original states.
 *
 *         This function only uses w0. When calling from assembly, it is not
 *         necessary to preserve the w1-w7 registers. w0 is destroyed though, so
 *         don't call EZBL_RestoreInterrupts() a second time expecting
 *         idempotent results unless you externally reprovide the same
 *         previousSRSave data.
 */
void EZBL_RestoreInterrupts(unsigned int previousSRSave);


/**
 * Reads the IFSx<y> Interrupt Flag state for the specified interrupt vector.
 *
 * @param irqNum IRQ number to query.
 *               On 16-bit targets, the IRQ number is the same as (Vector Number + 8).
 *               On 32-bit targets, IRQ and Vector Numbers are the same.
 *
 * @return <ul>
 *           <li>0 = No interrupt pending</li>
 *           <li>1 = Interrupt pending</li>
 *         </ul>
 */
int EZBL_RdIntFlag(int irqNum);


/**
 * Atomically writes the IFSx<y> Interrupt Flag bit for the specified interrupt
 * vector.
 *
 * @param irqNum IRQ number indicating which IFSx bit will be written.
 *               On 16-bit targets, the IRQ number is the same as (Vector Number + 8).
 *               On 32-bit targets, IRQ and Vector Numbers are the same.
 *
 * @param newIFSbitVal New setting to write to the IFSbit.
 *                     0 = Interrupt flag cleared to '0'
 *                     non-zero = interrupt flag set to '1'
 *
 * @return Prior IFSx Interrupt Flag state
 *          <ul>
 *              <li>0 = No interrupt was pending</li>
 *              <li>1 = Interrupt was pending</li>
 *          </ul>
 */
int EZBL_WrIntFlag(int irqNum, int newIFSbitVal);


/**
 * Atomically clears the IFSx<y> Interrupt Flag bit for the specified interrupt
 * vector.
 *
 * @param irqNum IRQ number indicating which IFSx bit will be cleared to '0'.
 *               On 16-bit targets, the IRQ number is the same as (Vector Number + 8).
 *               On 32-bit targets, IRQ and Vector Numbers are the same.
 *
 * @return Prior IFSx Interrupt Flag state
 *          <ul>
 *              <li>0 = No interrupt was pending</li>
 *              <li>1 = Interrupt was pending</li>
 *          </ul>
 */
int EZBL_ClrIntFlag(int irqNum);


/**
 * Atomically sets the IFSx<y> Interrupt Flag bit for the specified interrupt
 * vector.
 *
 * @param irqNum IRQ indicating which IFSx bit to set to '1'.
 *               On 16-bit targets, the IRQ number is the same as (Vector Number + 8).
 *               On 32-bit targets, IRQ and Vector Numbers are the same.
 *
 * @return Prior IFSx Interrupt Flag state
 *          <ul>
 *              <li>0 = No interrupt was pending</li>
 *              <li>1 = Interrupt was pending</li>
 *          </ul>
 */
int EZBL_SetIntFlag(int irqNum);


/**
 * Atomically toggles the IFSx<y> Interrupt Flag bit for the specified interrupt
 * vector.
 *
 * @param irqNum IRQ indicating which IECx bit to toggle/invert.
 *               On 16-bit targets, the IRQ number is the same as (Vector Number + 8).
 *               On 32-bit targets, IRQ and Vector Numbers are the same.
 *
 * @return Prior IFSx Interrupt Flag state
 *          <ul>
 *              <li>0 = No interrupt was pending</li>
 *              <li>1 = Interrupt was pending</li>
 *          </ul>
 */
int EZBL_InvIntFlag(int irqNum);



/**
 * Reads the IECx<y> Interrupt Enable bit for the specified interrupt vector.
 *
 * @param irqNum IRQ number to query.
 *               On 16-bit targets, the IRQ number is the same as (Vector Number + 8).
 *               On 32-bit targets, IRQ and Vector Numbers are the same.
 *
 * @return IECx Interrupt Enable state
 *          <ul>
 *              <li>0 = Interrupt is disabled (masked)</li>
 *              <li>1 = Interrupt enabled</li>
 *          </ul>
 */
int EZBL_RdIntEn(int irqNum);


/**
 * Atomically writes the IECx<y> Interrupt Enable bit for the specified
 * interrupt vector.
 *
 * @param irqNum IRQ number indicating which IECx bit will be written.
 *               On 16-bit targets, the IRQ number is the same as (Vector Number + 8).
 *               On 32-bit targets, IRQ and Vector Numbers are the same.
 *
 * @param newIECbitVal Value to write to the IECbit:
 *                     0 = disable (mask) this interrupt source;
 *                     non-zero = enable the interrupt
 *
 * @return Prior IECx Interrupt Enable state, not taking into account global
 *         interrupt enable/disable states, such as a DISI instruction or
 *         GIE == 0 condition.
 *          <ul>
 *              <li>0 = Interrupt Enable bit was previously clear</li>
 *              <li>1 = Interrupt Enable bit was previously set</li>
 *          </ul>
 */
int EZBL_WrIntEn(int irqNum, int newIECbitVal);


/**
 * Atomically clears the IECx<y> Interrupt Enable bit for the specified
 * interrupt vector.
 *
 * @param irqNum IRQ number indicating which IECx bit to clear to '0'.
 *               On 16-bit targets, the IRQ number is the same as (Vector Number + 8).
 *               On 32-bit targets, IRQ and Vector Numbers are the same.
 *
 * @return Prior IECx Interrupt Enable state, not taking into account global
 *         interrupt enable/disable states, such as a DISI instruction or
 *         GIE == 0 condition.
 *          <ul>
 *              <li>0 = Interrupt Enable bit was previously clear</li>
 *              <li>1 = Interrupt Enable bit was previously set</li>
 *          </ul>
 */
int EZBL_ClrIntEn(int irqNum);


/**
 * Atomically sets the IECx<y> Interrupt Enable bit for the specified
 * interrupt vector.
 *
 * @param irqNum IRQ number indicating which IECx bit to set to '1'.
 *               On 16-bit targets, the IRQ number is the same as (Vector Number + 8).
 *               On 32-bit targets, IRQ and Vector Numbers are the same.
 *
 * @return Prior IECx Interrupt Enable state, not taking into account global
 *         interrupt enable/disable states, such as a DISI instruction or
 *         GIE == 0 condition.
 *          <ul>
 *              <li>0 = Interrupt Enable bit was previously clear</li>
 *              <li>1 = Interrupt Enable bit was previously set</li>
 *          </ul>
 */
int EZBL_SetIntEn(int irqNum);


/**
 * Atomically toggles/inverts the IECx<y> Interrupt Enable bit for the specified
 * interrupt vector.
 *
 * @param irqNum IRQ number indicating which IECx bit to toggle.
 *               On 16-bit targets, the IRQ number is the same as (Vector Number + 8).
 *               On 32-bit targets, IRQ and Vector Numbers are the same.
 *
 * @return Prior IECx Interrupt Enable state, not taking into account global
 *         interrupt enable/disable states, such as a DISI instruction or
 *         GIE == 0 condition.
 *          <ul>
 *              <li>0 = Interrupt Enable bit was previously clear</li>
 *              <li>1 = Interrupt Enable bit was previously set</li>
 *          </ul>
 */
int EZBL_InvIntEn(int irqNum);


/**
 * Returns the IPCx<y> Interrupt Priority for the specified interrupt vector.
 * Sub-priority bits, if present, are not returned.
 *
 * @param irqNum IRQ number indicating which IPCx bits to read.
 *               On 16-bit targets, the IRQ number is the same as (Vector Number + 8).
 *               On 32-bit targets, IRQ and Vector Numbers are the same.
 *
 * @return Interrupt priority level, where 0 disables the interrupt (lowest
 *         priority) and 7 indicates maximum priority, preempting all IPL6 and
 *         below interrupts.
 */
unsigned int EZBL_RdIntPri(int irqNum);


/**
 * Writes the IPCx<y> Interrupt Priority for the specified interrupt vector.
 * Sub-priority bits, if supported by the interrupt controller, are left
 * unchanged.
 *
 * @param irqNum IRQ number indicating which IPCx bits to write to.
 *               On 16-bit targets, the IRQ number is the same as (Vector Number + 8).
 *               On 32-bit targets, IRQ and Vector Numbers are the same.
 *
 * @return Prior interrupt priority level, where 0 means the the interrupt was
 *         disabled (lowest priority) and 7 indicates maximum priority,
 *         preempting all IPL6 and below interrupts.
 */
unsigned int EZBL_WrIntPri(int irqNum, unsigned int newPriority);


/**
 * Calls a Interrupt Service Routine C function pointer at the same IPL as
 * currently set.
 *
 * This function sets up a fake ISR call frame on the stack such that when the
 * called ISR executes the RETFIE instruction, execution will return to the
 * instruction after the call instruction that called EZBL_CallISR() and retain
 * the same IPL as you called EZBL_CallISR() at.
 *
 * This function can be used to call ISRs of lower or higher priority than the
 * current CPU IPL. However, be very careful when doing this as the called ISR
 * can become self-reentrant if the ISR is enabled. Higher priority ISRs will
 * become self-reentrant if the hardware interrupt flag becomes set. Lower
 * priority ISRs will become self-reentrant if the ISR was already active when
 * this call to EZBL_CallISR() is made.
 *
 * @param ISRFunction C Function pointer of type void (*ISRFuncName)(void);
 *                          Ex: EZBL_CallISR(_U2TXInterrupt);
 *                    The _U2TXInterrupt() function prototype should have
 *                    already been declared just like when calling normal
 *                    functions.
 */
 void EZBL_CallISR(void (*ISRFunction)(void));


/**
 * Calls an Interrupt Service Routine at the same IPL as currently set. The ISR
 * function address is obtained by looking up the IRQ number in the IVT (or
 * AIVT if applicable and enabled), so the function called may not be valid or
 * be directed to a generic _DefaultInterrupt() function if unimplemented.
 *
 * This function sets up a fake ISR call frame on the stack such that when the
 * called ISR executes the RETFIE instruction, execution will return to the
 * instruction after the call instruction that called EZBL_CallIRQ() and retain
 * the same IPL as you called EZBL_CallIRQ() at.
 *
 * This function can be used to call ISRs of lower or higher priority than the
 * current CPU IPL. However, be very careful when doing this as the called ISR
 * can become self-reentrant if the ISR is enabled. Higher priority ISRs will
 * become self-reentrant if the hardware interrupt flag becomes set. Lower
 * priority ISRs will become self-reentrant if the ISR was already active when
 * this call to EZBL_CallIRQ() is made.
 *
 * @param interruptIRQ IRQ number of the ISR function to call. IRQ numbers
 *                     correspond to the bit position for the interrupt flag in
 *                     IFSx registers, or interrupt enable bits in IECx
 *                     registers.
 *
 *                     For example, on a dsPIC33EPXXGS50X family device, the
 *                     data sheet Interrupt Controller chapter shows IRQ 0 as
 *                     being the INT0 interrupt. It has IFS0<0>/IEC0<0> bits
 *                     dedicated to it. INT4 has IRQ 54 and is in
 *                     IFS3<6>/IEC3<6>: 3*16 + 6 = 54. '3' is the register
 *                     number, 16 is the bit width of a IFS/IEC register, and 6
 *                     is the bit position within the register.
 *
 *                     To call a trap, specify a negative IRQ number,
 *                     corresponding to the IVT ordering. Ex:
 *                     <ul>
 *                          <li>-1 = Reserved Trap (7)</li>
 *                          <li>-2 = Generic Soft Trap (6)</li>
 *                          <li>-3 = Reserved Trap (5)</li>
 *                          <li>-4 = Math Error Trap (4)</li>
 *                          <li>-5 = Stack Error Trap (3)</li>
 *                          <li>-6 = Generic Hard Trap (2)</li>
 *                          <li>-7 = Address Error Trap (1)</li>
 *                          <li>-8 = Oscillator Fail Trap (0)</li>
 *                     </ul>
 */
 void EZBL_CallIRQ(int interruptIRQ);

 
/**
 * void EZBL_CallLongPointer0(unsigned long functionAddress);
 *
 * Calls a long (23-bit) function pointer, passing no arguments (called function
 * should have void as the argument list). Related EZBL_CallLongPointer1() and
 * EZBL_CallLongPointer2() functions exist and can be instead called if you need
 * to pass arguments to the called function.
 *
 * For a null pointer, this function does nothing and returns immediately rather
 * than trying to branch to address 0x000000. This is particularly useful when
 * using 'weak' attributed function declarations and you want a function call to
 * disappear when the applicable function is not linked into the project (as
 * opposed to blindly calling a null pointer and effectively resetting the
 * device).
 *
 * This function first masks off the top 8 bits of the functionPointer so as to
 * be compatible with __builtin_tblpage() and #tblpage() which may set bit 24 on
 * devices with EDS instead of PSV architecture features (ex: dsPIC33E/PIC24E
 * and some PIC24F products).
 *
 * Return type is specified as void, but it will actually exactly match the
 * called function return type and have the called function's return data.
 *
 * Function optional parameters correspond to the parameters that the called
 * function will receive. They are realigned so that param1 goes to w0, param2 to
 * w1, etc. like the called function will normally expect.
 */
void EZBL_CallLongPointer0(unsigned long functionAddress);
void EZBL_CallLongPointer1(unsigned long functionAddress, unsigned int param1);                         // See EZBL_CallLongPointer0() help
void EZBL_CallLongPointer2(unsigned long functionAddress, unsigned int param1, unsigned int param2);    // See EZBL_CallLongPointer0() help


/**
 * Macro that calls a void function (with no arguments) by token name, assuming
 * it is linked into the project. You do not have to declare a prototype for the
 * function first.
 *
 * This macro internally defines the function symbol name as a weak, extern
 * symbol using inline assembly and then calls it using the
 * EZBL_CallLongPointer0() function.
 *
 * NOTE: any other accesses to this function will also be declared weak for the
 * same .c file when using this macro.
 *
 * @param function_name a function symbol name to call. The symbol name should
 *                      be the "C" name (i.e. without a leading underscore
 *                      prefix added). If the symbol doesn't exist at link time,
 *                      a few cycles are wasted without doing anything.
 *
 * @return unsigned int 16-bits of return data from the called function, if any.
 *         If the function has a void return type, the return value from this
 *         macro should be ignored. If the function has a 32-bit or greater
 *         return type, the values after the first 16-bit w0 contents are thrown
 *         away.
 *
 */
#define EZBL_CallFunctionIfPresent(function_name)                               \
    ({                                                                          \
        unsigned int ret;                                                       \
        __asm__("\n    .weak    _" STRINGIFY(function_name)                     \
                "\n    mov      #tbloffset(_" STRINGIFY(function_name) "), w0  "\
                "\n    mov      #tblpage(_" STRINGIFY(function_name) "), w1    "\
                "\n    call     _EZBL_CallLongPointer0                         "\
                "\n    mov      w0, %0                                         "\
                : "=g"(ret) : : "w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7", "cc", "memory");    \
        ret;                                                                    \
    })


/**
 * void EZBL_WriteNVMCON(unsigned int nvmconValue, unsigned int unlockKeyMultiplier, unsigned int blockUntilDone);
 *
 * Wait for NVMCON<WR> to be clear, disables all interrupts, computes the proper
 * NVMKEY, copies the computed values into NVMKEY, sets the NVMCON<WR> bit,
 * restores interrupts (if blockUntilDone == 0), polls for completion of the
 * erase/write operation (if blockUntilDone is non-zero), and then restores
 * interrupts (if not done already). For the unlock sequence to work correctly,
 * set:
 *     unlockKeyMultiplier = 0x03E5
 * and:
 *     EZBL_NVMKey = 0x03DF
 *
 * @param nvmconValue Value to write to NVMCON with bit 15 (NVMCON<WR> NOT yet
 *                    set). This function will issue a second write to set
 *                    NVMCON<WR> internally after performing correct key
 *                    sequencing. Bit 14, corresponding to NVMCON<WREN> should
 *                    be set if you intend to start an erase/write sequence.
 * @param unlockKeyMultiplier Set to value 0x03E5. All other values will result
 *                            in NVMCON<WR> not getting set and therefore
 *                            trigger no erase/write operation.
 * @param blockUntilDone <p>Flag indicating if this function should not return
 *                       until the NVMCON write is done and NVMCON<WR> self
 *                       clears by hardware. Should be 0x0001 (or any value with
 *                       bit 0 set) to force blocking on certain devices which
 *                       have an errata on the CPU not stalling correctly (ex:
 *                       dsPIC33EPXXX(GP/MC/MU)806/810/814 and
 *                       PIC24EPXXX(GP/GU)806/810/814 rev B1 devices). 0x0000
 *                       indicates the erase/write should be started and then
 *                       return asynchronously without waiting for hardware to
 *                       clear it.</p>
 *
 *                       <p>NOTE: this flag is only a software polling control
 *                       mechanism. If the underlying NVM hardware requires the
 *                       CPU to stall for the duration of the erase/write, then
 *                       this parameter will have no effect and the CPU will
 *                       block regardless.
 *
 * @return NVM erase/write is started (and optionally completed), interrupts are
 *         restored as appropraite, and EZBL_NVMKey is cleared to zero. If the
 *         CPU hardware stalls, <i>blockUntilDone</i> is set to '1', or the
 *         erase/write otherwise completes before the return code is reached,
 *         then the Write Enable bit, NVMCON<WREN>, is cleared before return.
 */
void EZBL_WriteNVMCON(unsigned int nvmconValue, unsigned int unlockKeyMultiplier, unsigned int blockUntilDone);


/**
 * Disables all interrupts by clearing all IECx registers, swaps to the
 * presently inactive partition, and branches to the 0x000000 reset vector.
 *
 * CORCON<IPL3>, SR<7:0>, and the Stack Frame Active bit (if present) are all
 * cleared atomically during the branch, resulting in the CPU starting
 * execution on the new partition in IPL0 (main() context) with the SR<RA>
 * Repeat Active status bit cleared, so it is possible to call this function
 * from within an ISR if needed to synchronize the swap to occur just after
 * some critical ISR code is done executing.
 *
 * Interrupts are NOT restored in this function. The needed IECx bits need to be
 * restored by the new partition's Application since we can't have any
 * interrupts occurring while doing the partition swap unlock and bootswap, nor
 * would it be safe to trigger an ISR while the new Application is busy
 * executing initialization code needed for any ISRs that have changed.
 *
 * To detect if execution is occurring in the new Application due to a genuine
 * reset or due to a partition swap event, check the NVMCON<SFTSWP> status bit.
 * It is zero on real reset and one if a partition swap has taken place.
 *
 * XC16 v1.30+ supports a callback function before executing the CRT
 * initializer to allow you to control if variable initializer code should
 * execute. This callback should be declared as int _crt_start_mode(void); with
 * the return value of 0 meaning to execute all initializer code as normal and 1
 * meaning to skip normal initialization for Live Update style partition swaps.
 * The latter case skips most variable initialization and branches to main()
 * sooner.
 *
 * A suggested implementation of this callback is:
 *     int __attribute__((optimize(1))) _crt_start_mode(void)
 *     {
 *         return _SFTSWP;
 *     }
 *
 * XC16 v1.30+ also supports a new 'priority(n)' function and variable
 * attribute, where n is an integer from 1 to 65535 and allows code fragments +
 * variables to execute in advance of ordinary CRT variable initialization
 * completion and also when _crt_start_mode() returns 1.
 *
 * The CRT initializes the stack pointer, SPLIM, DSRPAG, initializes any
 * variables attributed with 'priority(1)' and then calls any void functions
 * with 'priority(1)' also assigned as an attribute. 'priority(2)' variables are
 * then initialized followed by 'priority(2)' functions and so on until no more
 * priority tagged variables or functions are left, whereupon:
 *   if(_crt_start_mode() == 1)
 *      - user_init() functions are called, if any
 *      - main() is executed
 *   else   // when _crt_start_mode() == 0 or _crt_start_mode() is undefined
 *      - unattributed (ordinary) variables are initialized
 *      - uart_init() functions are called, if any
 *      - main() is executed
 *
 * Using the priority attributes allows you to selectively reenable any
 * time-critical tasks, such as specific Interrupts, in a controlled order so as
 * to avoid any unnecessary execution delay during a partition swap event as the
 * bulk of non-special variables gets initialized or zeroed.
 *
 * See the XC16 documentation for more information on the _crt_start_mode() and
 * and priority(n) features.
 *
 * NOTE: The FICD<BTSWP> Config word bit must be turned ON (cleared to '0') or
 * else this function will execute as described, but without actually swapping
 * partitions. Use #pragma config BTSWP = ON or EZBL_SET_CONF(_FICD, _BTSWP_ON)
 * to appropriately configure this.
 *
 * @return This function never returns. Execution branches through the reset
 *         vector, resulting in all normal SFRs retaining state, but the stack
 *         is reinitialized and RAM variables may (or may not) also be
 *         reinitialized.
 *
 *         This function's prototype has the __attribute__((noreturn)) option
 *         applied so the compiler can optimize away subsequent code (like a
 *         ulnk and return instruction) as dead code.
 */
void __attribute__((noreturn)) EZBL_PartitionSwap(void);
static void __attribute__((noreturn, weakref("EZBL_PartitionSwap"))) EZBL_PartitionSwapW(void);


/**
 * Restores any Bootloader components (defined in ".text.EZBLEraseRestoreTable"
 * code section when Bootloader was linked) that might have been erased by
 * a Flash erase operation.
 *
 * For example, on a device with device Config words located in flash at the end
 * of Application space but with Bootloader defined Config words present, an
 * erase of the last flash page must have Bootloader content reprogrammed before
 * any Application data can be written there to minimize bricking risk to the
 * Bootloader.
 *
 * It is safe to call this function multiple times if the restoreConfigBytes
 * parameter = 0, even if all other bootloader components are already
 * programmed to flash. The function first checks the destination address to see
 * if data is present before attempting to program it. On the other hand, if the
 * device has Config bytes (Config words at >= address 0xF80000) and the
 * restoreConfigBytes flag is non-zero, then this function should only be called
 * once when needing to restore these Config bytes. Non-Flash based Config bytes
 * are always reprogrammed since they don't have a detectable erased state
 * (like EEPROM, you just reprogram them without erasing first).
 *
 * If a Reserved Bit in a flash Config word exists that requires a '0' to
 * be programmed to it, this bit is written to '0' by the
 * __EZBL_TOOLS_COMPUTED_ERASE_RESTORE_TABLE element that applies to it
 * (assuming such an element was generated).
 *
 * @param restoreConfigBytes On devices with "Non-volatile" Config bytes (not
 *          flash based Config Words), a non-zero value for this flag specifies
 *          that any Bootloader defined Config bytes should be reprogrammed by
 *          this function. As noted above, these values are always programmed
 *          for every call to EZBL_RestoreAppErasable() when this flag is set,
 *          so minimize unneeded restore operations with this flag set to avoid
 *          wasting Config byte endurance.
 *
 *          On devices with Flash based Config words ("Volatile Config Words"),
 *          this flag is not applicable and ignored.
 *
 * @return Any Flash address that the Bootloader requires and has stored in the
 *         .text.EZBLEraseRestoreTable section is reprogrammed, assuming the
 *         destination flash doesn't already have mismatched data stored
 *         there. Additionally, if the device has "Non-volatile" Config bytes
 *         and the restoreConfigBytes parameter is non-zero, the Bootloader's
 *         Config bytes will be programmed.
 */
void EZBL_RestoreAppErasable(unsigned int restoreConfigBytes);


/**
 * signed int EZBL_ROMAddressInBootloader(unsigned long flashAddress);
 *
 * Determines if the given program memory address exists in a bootloader
 * reserved region. If so, EZBL_WriteROMChecked() will not be able to write to
 * this address (although EZBL_ErasePage() and EZBL_WriteROM() will still be
 * able to erase/write to this address if you really want to modify bootloader
 * space).
 *
 * This is implemented as a macro that calls EZBL_MaskBootloaderRegions() with
 * the flashAddress, a null write pointer, and length of 1 for API naming
 * compatibility reasons. Applications should call EZBL_MaskBootloaderRegions()
 * directly if an address range needs to be tested for bootloader overlap
 * (instead of a single memory address like this macro does).
 *
 * @param flashAddress Specifies the program memory address that should be
 *                     checked for overlap with any bootloader reserved region.
 *                     Bootloader reserved regions normally include any extra
 *                     Page boundary padding addresses as well.
 *
 * @return 0 if the address does not exist in bootloader space (normally implies
 *           this address is Application space, but illegal addresses will also
 *           return 0).
 *         1 if the address is in a Bootloader reserved Flash program region.
 */
#define EZBL_ROMAddressInBootloader(address)     EZBL_IsProgramRestricted((address))


#if defined(__XC16__)
/**
 * unsigned long EZBL_Mul3Div2(unsigned int addrToByteNumber);
 *
 * Multiplies a 16-bit unsigned integer number by 3/2 halfs. Normally, this is
 * needed when converting a Flash address into a contiguous byte offset. Each
 * Flash address increment is normally '2', which represents '3' data bytes.
 * Requires 9 instruction cycles, not including the call, but including the
 * return.
 *
 * @param addrToByteNumber unsigned 16-bit integer to be multiplied by 3/2.
 *
 * @return unsigned 32-bit long integer of addrToByteNumber multiplied by 3 and
 *         divided by 2. If an odd number is provided as input, the result
 *         has the fractional 0.5 return portion truncated off. Ex:
 *         1*(3/2) = 1, 3*(3/2) = 4, 5*(3/2) = 7, etc. Even numbered inputs
 *         always result in perfect precision since they are always integer
 *         divisible by 2 without data loss.
 */
unsigned long EZBL_Mul3Div2(unsigned int addrToByteNumber);
#define EZBL_Mul3Div2(addrToByteNumber) /* macro for inline compiler multiply and divide when addrToByteNumber is a constant */ \
    (__builtin_constant_p(addrToByteNumber) ? ((addrToByteNumber)*3ul/2u) : EZBL_Mul3Div2(addrToByteNumber))
#elif defined(__XC32__) || defined(__PIC32__)
#define EZBL_Mul3Div2(addrToByteNumber) ((unsigned long)(addrToByteNumber)) // Don't change the number on PIC32 devices since an address is a just as good as a byte offset on this architecture
#endif


#if defined(__XC16__)
/**
 * unsigned long EZBL_Mul3Div2L(unsigned long addrToByteNumber);
 *
 * Multiplies a 32-bit unsigned long number by 3/2 halfs. Normally, this is
 * needed when converting a Flash address offset into a byte count. Each
 * Flash address increment is normally '2', which represents '3' data bytes.
 *
 * Requires 11 instruction cycles, not including the call, but including the
 * return. addrToByteNumber > 2,863,311,530 will result in overflow (but the
 * return result will still be correct in modulo 32-bit math; i.e. just add
 * 0x100000000 if(addrToByteNumber > 2,863,311,530) to get the correct result)
 *
 * @param addrToByteNumber unsigned 32-bit long integer to be multiplied by 3/2.
 *                         If your input will always be 16-bits or less, the
 *                         EZBL_Mul3Div2() function can be used instead to
 *                         save a clock cycle or two.
 *
 * @return unsigned 32-bit long integer of addrToByteNumber multiplied by 3 and
 *         divided by 2. If an odd number is provided as input, the result
 *         has the fractional 0.5 return portion truncated off. Ex:
 *         1*(3/2) = 1, 3*(3/2) = 4, 5*(3/2) = 7, etc. Even numbered inputs
 *         always result in perfect precision since they are always integer
 *         divisible by 2 without data loss.
 */
unsigned long EZBL_Mul3Div2L(unsigned long addrToByteNumber);
#define EZBL_Mul3Div2L(addrToByteNumber) /* macro for inline compiler multiply and divide when addrToByteNumber is a constant */ \
    (__builtin_constant_p(addrToByteNumber) ? (((unsigned long)(addrToByteNumber))*3ul/2u) : EZBL_Mul3Div2L(addrToByteNumber))
#elif defined(__XC32__) || defined(__PIC32__)
#define EZBL_Mul3Div2L(addrToByteNumber)    ((unsigned long)(addrToByteNumber))  // Don't change the number on PIC32 devices since an address is a just as good as a byte offset on this architecture
#endif


#if defined(__XC16__)
/**
 * unsigned int EZBL_Div3Mul2(unsigned int byteToAddrNumber, unsigned int *remainder);
 *
 * Multiplies a 16-bit unsigned integer number by 2/3rds and optionally returns
 * the remainder of the divide by 3 operation (effectively before the multiply
 * by 2). Normally, this function is needed when converting a byte count into a
 * Flash address increment. Each Flash address increment is '2', which represents
 * '3' data bytes. If there are any remainder bytes that cannot fill a whole
 * Flash address increment of 2, then the remainder bytes can be returned via the
 * *remainder pointer. Specify a null pointer if the remainder is not needed.
 *
 * Requires 32 instruction cycles (on dsPIC33E/PIC24E) or 27 cycles (on
 * PIC24/dsPIC33F), including the return, but not the call.
 *
 * @param byteToAddrNumber unsigned 16-bit integer to be multiplied by 2 and
 *                         divided by 3. If your input can be larger than
 *                         16-bits, use the EZBL_Div3Mul2L() function
 *                         instead.
 *
 * @param *remainder    Optional pointer to an unsigned 16-bit integer to store
 *                      the division by 3 remainder. This parameter can be null
 *                      (0x0000) if the remainder is not needed. The remainder
 *                      is computed after the multiplication by 2 has already
 *                      been done to yield perfect precision. Ex:
 *                      1 * (2/3) = 0 Remainder 1, 2 * (2/3) = 1 Remainder 1,
 *                      3 * (2/3) = 2 Remainder 0, 4 * (2/3) = 2 Remainder 2,
 *                      etc.
 *
 * @return unsigned 16-bit integer result of the computation where the remainder
 *         (if any) is truncated off. If the remainder is needed, use the
 *         *remainder input option to retrieve it.
 */
unsigned int EZBL_Div3Mul2(unsigned int byteToAddrNumber, unsigned int *remainder);
// Optimized version of EZBL_Div3Mul2() that will evaluate constants at compile-time for no run-time function call or execution overhead. When parameters are not constant, this macro reverts to a standard EZBL_Div3Mul2() function call.
#define EZBL_Div3Mul2(byteToAddrNumber, remainder)  /* macro for inline compiler optimization when byteToAddrNumber is constant */    \
    ({                                                                          \
        unsigned int ret;                                                       \
                                                                                \
        if(__builtin_constant_p(byteToAddrNumber))                              \
        {                                                                       \
            if(!__builtin_constant_p(remainder))                                \
                *((unsigned int*)(remainder)) = (byteToAddrNumber) % 3u;        \
            ret = ((unsigned int)(((byteToAddrNumber)/3u)*2u));                 \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            ret = EZBL_Div3Mul2(byteToAddrNumber, remainder);                   \
        }                                                                       \
        ret;                                                                    \
    })
#elif defined(__XC32__) || defined(__PIC32__)
#define EZBL_Div3Mul2(byteToAddrNumber, remainder) ({if(remainder) *((unsigned int*)(remainder)) = 0u; ((unsigned int)(byteToAddrNumber));})
#endif


#if defined(__XC16__)
/**
 * unsigned long EZBL_Div3Mul2L(unsigned long byteToAddrNumber, unsigned int *remainder);
 *
 * Multiplies a 32-bit unsigned long number by 2/3rds and optionally returns the
 * remainder of the divide by 3 operation (effectively before the multiply by 2).
 * Normally, this function is needed when converting a byte count into a Flash
 * address increment. Each Flash address increment is '2', which represents '3'
 * data bytes. If there are any remainder bytes that cannot fill a whole Flash
 * address increment of 2, then the remainder bytes can be returned via the
 * *remainder pointer. Specify a null pointer if the remainder is not needed.
 * Multiplication and division is done iteratively with bit shifts.
 *
 * Requires around 570 instruction cycles (on dsPIC33E/PIC24E), up to about
 * (exactly?) 634 maximum, including the return, but not the call. Fewer cycles
 * are required on PIC24/dsPIC33F due to shorter by about 173 cycles (~397 total
 * typically) due to branch delay reduction.
 *
 * @param byteToAddrNumber unsigned 32-bit integer to be multiplied by 2 and
 *                         divided by 3. If your input will always be 16-bits or
 *                         less, use the EZBL_Div3Mul2() function instead for
 *                         greater efficiency.
 *
 * @param *remainder    Optional pointer to an unsigned 16-bit integer to store
 *                      the division by 3 remainder. This parameter can be null
 *                      (0x0000) if the remainder is not needed. The remainder
 *                      is computed after the multiplication by 2 has already
 *                      been done to yield perfect precision. Ex:
 *                      1 * (2/3) = 0 Remainder 1, 2 * (2/3) = 1 Remainder 1,
 *                      3 * (2/3) = 2 Remainder 0, 4 * (2/3) = 2 Remainder 2,
 *                      etc.
 *
 * @return unsigned 32-bit long integer result of the computation where the
 *         remainder (if any) is truncated off. If the remainder is needed, use
 *         the *remainder input option to retrieve it.
 */
unsigned long EZBL_Div3Mul2L(unsigned long byteToAddrNumber, unsigned int *remainder);
#define EZBL_Div3Mul2L(byteToAddrNumber, remainder)  /* macro for inline compiler optimization when byteToAddrNumber is constant */    \
    ({                                                                          \
        unsigned long ret;                                                      \
                                                                                \
        if(__builtin_constant_p(byteToAddrNumber))                              \
        {                                                                       \
            if(!__builtin_constant_p(remainder))                                \
                *((unsigned int*)(remainder)) = (byteToAddrNumber) % 3u;        \
            ret = ((((unsigned long)(byteToAddrNumber))/3u)*2u);                \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            ret = EZBL_Div3Mul2L(byteToAddrNumber, remainder);                  \
        }                                                                       \
        ret;                                                                    \
    })
#elif defined(__XC32__) || defined(__PIC32__)
#define EZBL_Div3Mul2L(byteToAddrNumber, remainder) ({if(remainder) *((unsigned int*)(remainder)) = 0u; ((unsigned long)(byteToAddrNumber));})
#endif


/**
 * unsigned long EZBL_ReadDEVID(void);
 *
 * @return Returns the 24-bit contents of the "DEVID" Device ID program
 *         memory location at 0xFF0000. Upper 8-bits returned are always zeros.
 */
unsigned long EZBL_ReadDEVID(void);


/**
 * unsigned long EZBL_ReadDEVREV(void);
 *
 * @return Returns the 24-bit contents of the "DEVREV" Device Revision program
 *         memory location at 0xFF0002. Upper 8-bits returned are always zeros.
 */
unsigned long EZBL_ReadDEVREV(void);


#if defined(__XC16__)
/**
 * Reads one instruction word (24 bits) from Flash at the specified Flash
 * program memory address. The returned uppermost 8 bits are always 0x00.
 *
 * Besides ordinary program memory locations, this function can be used to read
 * from Auxiliary Flash, Configuration fuse bytes, and other debug or
 * programming executive memory spaces. On applicable devices programmed for
 * multiple Flash partitions, this function can also be used to read from the
 * inactive Flash partition (starting at address 0x400000), but only when the
 * inactive Flash partition is idle (i.e. not undergoing an erase or programming
 * operation). If the inactive partition is busy with an erase or programming
 * operation, the return results will be indeterminate.
 *
 * Care should be taken to ensure that the target address is legal for the
 * device. Attempting to read from unimplemented memory locations will, in most
 * cases, result in an Address Error Trap rather than returning 0x000000 data.
 * Similarly, on devices that implement segmented Flash Code Protection
 * security, attempting to read from a Flash region that is disallowed will
 * trigger a device reset.
 *
 * This function must temporary change the TBLPAG register contents in order to
 * issue the read. Ensure no interrupts are implemented which require TBLPAG to
 * be always valid. Alternatively, disable interrupts before calling this
 * function. The TBLPAG contents are restored upon return.
 *
 * @param address   unsigned 32-bit long integer of the address to read from
 *                  (will actually be 24-bits max and the LSbit must be set to
 *                  0). The EZBL_SYM32() macro can be used if you wish to
 *                  discover the address of a link time symbol.
 *
 * @return  unsigned 32-bit long integer data read from the given program memory
 *          address. Since program data is always 24-bits wide, the upper 8 bits
 *          will always be 0x00.
 */
unsigned long EZBL_ReadFlash(unsigned long address);
unsigned long EZBL_ReadFlashInstruction(unsigned long address); // Historical longer name for EZBL_ReadFlash(). This is now just an alias to EZBL_ReadFlash().
#elif defined(__XC32__) || defined(__PIC32__)
#define EZBL_ReadFlash(address)                 (*((unsigned long*)((address) | 0x80000000ul)))
#define EZBL_ReadFlashInstruction(address)      EZBL_ReadFlash(address)
#endif


#if defined(__XC16__)
/**
 * void EZBL_ReadPackedFlash(void *dest, unsigned long baseAddress, unsigned int byteOffset, unsigned int byteCount);
 *
 * Copies Flash/program memory region contents into RAM.
 *
 * Besides ordinary program memory locations, this function can be used to read
 * from Auxiliary Flash, Configuration fuse bytes, and other debug or
 * programming executive memory spaces. On applicable devices programmed for
 * multiple Flash partitions, this function can also be used to read from the
 * inactive Flash partition (starting at address 0x400000), but only when the
 * inactive Flash partition is idle (i.e. not undergoing an erase or programming
 * operation). If the inactive partition is busy with an erase or programming
 * operation, the return results will be indeterminate.
 *
 * Care should be taken to ensure that the target address is legal for the
 * device. Attempting to read from unimplemented memory locations will, in most
 * cases, result in an Address Error Trap rather than returning 0x000000 data.
 * Similarly, on devices that implement segmented Flash Code Protection
 * security, attempting to read from a Flash region that is disallowed will
 * trigger a device reset.
 *
 * This function must temporary change the TBLPAG register contents in order to
 * issue the read. Ensure no interrupts are implemented which require TBLPAG to
 * be always valid. Alternatively, disable interrupts before calling this
 * function. The TBLPAG contents are restored upon return.
 *
 * @param *dest RAM pointer to write the read program memory data to. This
 *              pointer has no alignment requirements (i.e. can be odd). This
 *              parameter can be null only if byteCount is zero.
 *
 * @param baseAddress Unsigned 32-bit (24-bit) program memory address partially
 *                    specifying where to read from. This address must be even
 *                    (instruction aligned address). The byteOffset parameter is
 *                    added to this address (with proper 3 byte/2 address
 *                    translation) to generate the actual starting read
 *                    location.
 *
 * @param byteOffset A count of how many bytes to add to baseAddress before
 *                   beginning reading. Proper 3-byte/2-address conversion is
 *                   done. For example, if you want to read bits 8-23 of an
 *                   instruction word at address 0x001234 (throwing the first
 *                   8-bits of instruction word data away), you could specify
 *                   baseAddress = 0x001234 and byteOffset = 1.
 *                   Alternatively, baseAddress = 0x001230 and byteOffset = 7
 *                   would address the same byte (byteOffsets 0, 1, and 2 would
 *                   be the 24-bit data bits from address 0x001230; offsets 3,
 *                   4, and 5 would be data from 0x001232, and offset 6 would be
 *                   the least significant 8-bits of the data at address
 *                   0x001234.
 *
 * @param byteCount  Count of many bytes to read from Flash starting at the
 *                   specified address and byte offset. The *dest RAM must have
 *                   this much free-space allocated.
 *
 * @return  Program memory data read is written to the *dest pointer. The data
 *          is returned in naturally packed form (i.e. no "phantom" 0x00 bytes
 *          included).
 */
void EZBL_ReadPackedFlash(void *dest, unsigned long baseAddress, unsigned int byteOffset, unsigned int byteCount);
#elif defined(__XC32__) || defined(__PIC32__)
static void inline __attribute__((always_inline, optimize(1))) EZBL_ReadPackedFlash(void *dest, unsigned long baseAddress, unsigned int byteOffset, unsigned int byteCount)
{
    EZBL_RAMCopy(dest, (const void*)((baseAddress | 0x80000000u) + byteOffset), byteCount);
}
#endif


#if defined(__XC16__)
/**
 * Reads consecutive instructions words (24-bits each) from Flash program space,
 * stores them as 32-bit unsigned longs in RAM, incrementing the read
 * address upon return.
 *
 * TBLPAG is internally saved, used and restored before return.
 *
 * Function blocks if an outstanding NVMCON<WR> operation is underway (dual
 * partition devices).
 *
 * The read address range can straddle 64K TBLPAG boundaries.
 *
 * @param *params Pointer to EZBL_UNPACKED_READER structure that contains the
 *                desired read address and memory target for reading sequential
 *                instruction words into. Each 24-bit instruction word is
 *                written as a 32-bit value with 0x00 filler in the upper 8
 *                bits.
 *
 *                Upon return, the params->readPgmAddr element has been
 *                post incremented to the next program address after the data
 *                returned. i.e. params->readPgmAddr += (destLen/4) * 0x2;
 *
 * @return  First instruction word contents from the read (lower 24-bits) and
 *          0x00 padding in the upper 8-bits.
 *
 *          For read lengths of <= 3 bytes, all 32-bits of the first instruction
 *          word value are returned, even though nothing will be written to the
 *          *dest pointer and the readPgmAddr doesn't increment.
 *
 *          Destination addresses are written to with 0x00 padding in the upper
 *          byte of each 32-bit integer and 24-bits of data from flash in the
 *          lower 3 bytes. This includes the first unsigned long which is also
 *          the function return value.
 *
 *          The read program address is incremented in the params structure
 *          corresponding to the number of unsigned longs read/written (except
 *          for the <= 3 byte case).
 */
unsigned long EZBL_ReadUnpackedInd(EZBL_UNPACKED_READER *params);
#endif


#if defined(__XC16__)
/**
 * Reads consecutive instructions words (24-bits each) from flash program space,
 * storing them as 32-bit unsigned longs in RAM where the top "phantom" byte is
 * zeroed.
 *
 * TBLPAG is internally saved, used and restored before return.
 *
 * Function blocks if an outstanding NVMCON<WR> operation is underway (dual
 * partition devices).
 *
 * The read address range can straddle 64K TBLPAG boundaries.
 *
 * @param *dest Aligned RAM pointer to write the data read from flash. This
 *              pointer can be null only if destLen is <= 4.
 *
 * @param readPgmAddr Even-aligned flash program memory address to begin reading
 *                    from.
 *
 * @param destLen Number of bytes (not an unsigned long count) to maximally
 *                write to *dest. Non-integral divisble by 4 values are allowed
 *                but reading/writing will proceede only up to the last integral
 *                unsigned long length.
 *
 *                If destLen is < 4, then *dest is not written. However,
 *                precisely one program word will still be read and returned for
 *                the function return parameter.
 *
 * @return  First instruction word contents from the read (lower 24-bits) and
 *          0x00 padding in the upper 8-bits.
 *
 *          For read lengths of <= 3 bytes, all 32-bits of the first instruction
 *          word value are returned
 *
 *          *dest addresses are written to with 0x00 padding in the upper
 *          byte of each 32-bit integer and 24-bits of data from flash in the
 *          lower 3 bytes, little endian ordered. The first instruction word
 *          read from flash is included in the *dest write, even though it is
 *          also returned via the function return value.
 */
unsigned long EZBL_ReadUnpacked(unsigned long *dest, unsigned long readPgmAddr, unsigned int destLen);   // Direct parameter version of EZBL_ReadUnpackedInd(). Incremented destination address is thrown away.
#endif


#if defined(__XC16__)
/**
 * Reads two consecutive instructions words (24-bits each + 8-bits of filler on 
 * 16-bit devices, 64-bits total) from program space and increments a read
 * address pointer by 0x4 (for 16-bit devices) or 0x8 (32-bit devices).
 *
 * TBLPAG on 16-bit devices is internally saved, used and restored before return.
 *
 * Function blocks if an outstanding NVMCON<WR> operation is underway (16-bit 
 * dual partition devices).
 *
 * The read address range can straddle 64K TBLPAG boundaries.
 *
 * @param *pgmReadAddr Pointer to a RAM variable containing the address which
 *                     will be consecutively read from flash memory. Note: this
 *                     is effectively a RAM pointer to a flash pointer and the
 *                     function will return with *pgmReadAddr (the RAM pointer)
 *                     incremented.
 *
 * @return  2 x 32-bits of aligned data read from flash. The high bytes of both
 *          32-bit instruction words on 16-bit platforms are returned as 0x00.
 *          
 *          *pgmReadAddr is incremented by 0x4 or 0x8, as appropriate for the
 *          device architecture.
 */
unsigned long long EZBL_ReadTablePair(unsigned long *pgmReadAddr);
#elif defined(__XC32__) || defined(__PIC32__)
static inline unsigned long long __attribute__((always_inline)) EZBL_ReadTablePair(unsigned long *pgmReadAddr)
{
    EZBL_UNION_64 data;
    data.u32[0] = *pgmReadAddr++;
    data.u32[1] = *pgmReadAddr++;
    return data.u64;
}
#endif


/**
 * object_type EZBL_GetROMObj(object_type objType, unsigned long srcPgmAddr);
 *
 * Macro reads an arbitrary data type/structure/union/array from Flash program
 * space, returning the same data type in a temporary, stack allocated RAM
 * buffer in the calling scope.
 *
 * Data read from Flash fills the RAM object with all 24-bits of each Flash
 * instruction word without any padding added or 0x00 byte skipping required for
 * PSV accesses. For objects of 32-bits and larger, this macro therefore
 * performs data realignment, treating the program space memory as a byte array.
 *
 * This macro reads the number of bytes returned by the sizeof() operator
 * against the given data type from Flash. Care should be taken when reading
 * structures containing an array with undefined element count as the final
 * struct element.
 *
 * @param objType Raw data type to read from Flash program space. Can be C
 *                standard types (ex: char, short, int, long, unsigned int,
 *                float, double), or user defined structs/unions/enums (ex:
 *                EZBL_INSTALLED_FLAGS) and other typedefs.
 *
 * @param srcPgmAddr 24-bit Flash program space address to begin reading the
 *                   structure from.  All 24-bits of each instruction word are
 *                   consumed, not the lower 16-bits only as PSV accesses
 *                   return.
 *
 *                   Often this needs to be obtained using the EZBL_SYM32()
 *                   macro, instead of the addressof (&) operator on a
 *                   space(prog) PSV/EDS object. Normal XC16 pointers are
 *                   only 16-bits wide, while longer __eds__/__prog__/__psv__
 *                   pointers are not Flash program space addresses.
 *
 * @return A data-space object of the same data type as specified for objType.
 *
 *         The object is allocated locally as an auto variable in the calling
 *         scope, so for data types that have internal elements, use the dot (.)
 *         operator instead of the arrow operator (->), unless your object type
 *         is a pointer in itself. Ex:
 *              U1MODE.UEN = EZBL_ReadROMObj(U1MODEBITS, 0x002802).UEN;
 *         In this example, you would be loading an SFR field with an equivalent
 *         structural data value stored in Flash at address (0x002802), which
 *         could instead be a Flash pointer you are using to store EEPROM
 *         emulated data in Flash.
 *
 *         The returned object is an l-value, so you can use the address of (&)
 *         operator against it and pass the pointer to other functions, ex:
 *         memcpy(). However, if you wish to directly write the structure to a
 *         pointer, see the EZBL_ReadROMObj() macro.
 */
#define EZBL_GetROMObj(objType, srcPgmAddr)                                     \
    ({                                                                          \
        typeof(objType) s;                                                      \
        EZBL_ReadPackedFlash(&s, srcPgmAddr, 0, sizeof(s));                     \
        s; /* this returns the temporary structure in gcc as an aggregate */    \
    })


/**
 * void EZBL_ReadROMObj(object_type *destRAMObjPtr, unsigned long srcPgmAddr);
 *
 * Macro copys an arbitrary data type/structure/union/array from packed Flash
 * program space to a RAM pointer for the same type.
 *
 * Data read from Flash fills the RAM object with all 24-bits of each Flash
 * instruction word data without any padding added or 0x00 byte skipping done.
 * For objects of 32-bits and larger, this macro therefore performs data
 * realignment, treating the program space memory as a packed byte array.
 *
 * This macro reads the number of bytes returned by the sizeof() operator
 * against the given dereferenced pointer type.
 *
 * @param *destRAMObjPtr Pointer or array to the RAM destination. The number of
 *                       bytes to be read are defined by the dereferenced size
 *                       from the pointer.
 *
 * @param srcPgmAddr 24-bit Flash program space address to begin reading the
 *                   structure from.  All 24-bits of each instruction word are
 *                   consumed, not the lower 16-bits only as PSV accesses
 *                   return.
 *
 *                   Often this needs to be obtained using the EZBL_SYM32()
 *                   macro, instead of the addressof (&) operator on a
 *                   space(prog) PSV/EDS object. Normal XC16 pointers are
 *                   only 16-bits wide, while longer __eds__/__prog__/__psv__
 *                   pointers are not Flash program space addresses.
 *
 * @return Number of object bytes read into *destRAMObjPtr
 *
 *         If a temporary, inline-instanced copy in RAM of the object read from
 *         Flash is desired, use the EZBL_GetROMObj() macro instead.
 */
#define EZBL_ReadROMObj(destRAMObjPtr, srcPgmAddr)                                                  \
    ({                                                                                              \
        unsigned int byteLen;                                                                       \
        if(sizeof(destRAMObjPtr) != sizeof(void*))  /* array case */                                \
        {                                                                                           \
            byteLen = sizeof(destRAMObjPtr);                                                        \
            EZBL_ReadPackedFlash((destRAMObjPtr), (srcPgmAddr), 0, byteLen);                        \
        }                                                                                           \
        else    /* regular pointer case */                                                          \
        {                                                                                           \
            byteLen = sizeof(*(destRAMObjPtr));                                                     \
            EZBL_ReadPackedFlash((destRAMObjPtr), (srcPgmAddr), 0, sizeof(*(destRAMObjPtr)));       \
        }                                                                                           \
        byteLen;                                                                                    \
    })



/**
 * void EZBL_WriteROMObj(unsigned long destPgmAddr, object_type *srcRAMObjPtr);
 *
 * Macro read-modifies-writes a data object to Flash memory. Very slow and can
 * be very inefficient if called more than once per Flash page.
 *
 */
#define EZBL_WriteROMObj(destPgmAddr, srcRAMObjPtr)                                                 \
    {                                                                                               \
        EZBL_NVMKey = 0x03DF;                                                                       \
        if(sizeof(srcRAMObjPtr) != sizeof(void*))  /* array case */                                 \
            EZBL_ModifyROM((destPgmAddr), (srcRAMObjPtr), sizeof(srcRAMObjPtr));                    \
        else /* regular pointer case */                                                             \
            EZBL_ModifyROM((destPgmAddr), (srcRAMObjPtr), sizeof(*srcRAMObjPtr));                   \
    }


/**
 * Copies a byte array, generating a 0x00 "phantom" padding byte after every 3
 * input bytes (24-bit instruction word). This function converts packed RAM data
 * into a format more suitable for Flash/program space use.
 *
 * The EZBL_RemovePhantoms() function can be used to reverse the processes,
 * which skips every 4th input byte.
 *
 * @param *dest RAM pointer to write the unpacked byte array with phantom bytes
 *              added. The size of this buffer must be at least
 *              ((srcLen + 2)/3 * 4) bytes large to hold the output.
 *
 * @param *srcData RAM pointer to a byte array that has packed data (3 bytes per
 *                 0x2 program address, no 4th 0x00 "phantom" byte).
 *
 *                 If a srcLen is specified that does not end on a 24-bit
 *                 instruction word boundary (i.e. srcLen % 3 != 0), then 0xFF
 *                 filler bytes are generated in the output array (plus final
 *                 0x00 phantom byte) to make a complete instruction word.
 *
 * @param srcLen Number of bytes to unpack with phantom bytes at the *srcData
 *               location.
 *
 * @return ((srcLen + 2)/3 * 2), which is the number of program addresses now
 *         represented with the phantom bytes present. This is also exactly half
 *         of the number of bytes written to *dest, inclusive of the newly
 *         generated phantom bytes.
 */
unsigned int EZBL_AddPhantoms(void *dest, void *srcData, unsigned int srcLen);


/**
 * Copies a byte array, generating pre-alignment padding, 0x00 "phantom"
 * padding bytes after every 3 input bytes (24-bit instruction word), and
 * post-alignment padding needed for writing/testing program flash. Padding is
 * generated as all '1's (except phantom bytes) to obtain 0x4 program address
 * alignment/final block size only on devices that implement a flash double word
 * (48-bit) write block size.
 *
 * The EZBL_RemovePhantoms() function can be used to reverse the processes,
 * which skips every 4th input byte.
 *
 * @param *dest RAM pointer to write the unpacked byte array with phantom bytes
 *              added. The size of this buffer must be at least
 *              (destPgmAddrLSbits & 0x2)>>1 + (srcLen + 2)/3 elements large
 *              (32-bit output elements) when padding could occur, or just
 *              (srcLen + 2)/3 elements when the device supports programming
 *              in 3 byte/single instruction word blocks.
 *
 *              The *dest range can overlap the *srcData range only if
 *              sequential instruction word processing does not result in
 *              instantaneous overlap at the end. The algorithm requires at
 *              least 4 + 1/3 more RAM bytes allocated in the dest than the
 *              source and proceeds starting with the passed pointer positions.
 *
 * @param *srcData RAM pointer to a byte array that has packed data (3 bytes per
 *                 0x2 program address, no 4th 0x00 "phantom" byte).
 *
 *                 If a srcByteLen is specified that does not end on a 24-bit
 *                 instruction word boundary (i.e. srcLen % 3 != 0), then 0xFF
 *                 filler bytes are generated in the output array (plus final
 *                 0x00 phantom byte) to make a complete instruction word
 *                 without reading past the end of the *srcData array.
 *
 * @param srcByteLen Number of input bytes to align/unpack at the *srcData
 *                   location.
 *
 * @param destPgmAddrLSbits integer containing at least the 2 least significant
 *                          program address bits that this unpacked array will
 *                          be written/compared/evaluated against in the future.
 *                          When the device has a 0x4 address (6 byte) minimum
 *                          flash write block size, misaligned destPgmAddrLSbits
 *                          values will trigger 0x00FFFFFF to be written to
 *                          *dest before starting ordinary unpacking. This will
 *                          make the *dest array always aligned to the 0x4
 *                          program address requirements for writing.
 *
 * @return (destPgmAddrLSbits & 0x2) + ((srcLen + 2)/3 * 2), which is the number
 *         of program addresses now represented with pre-alignment, phantom
 *         bytes, and post-padding block alignment present. This is also exactly
 *         half of the number of bytes written to *dest, inclusive of the newly
 *         generated padding/phantom bytes.
 */
unsigned int EZBL_AddPhantomsEx(unsigned long *dest, void *srcData, unsigned int srcByteLen, unsigned int destPgmAddrLSbits);


/**
 * Copies or rewrites a byte array, removing every 4th byte from the source
 * array. This function is typically used to remove 0x00 "phantom" bytes after
 * every 3 useful bytes.
 *
 * To do the reverse and add phantom bytes, see EZBL_AddPhantoms().
 *
 * @param *dest RAM pointer to write the packed byte array without phantom
 *              padding bytes. The size of this buffer must be at least
 *              (srcDataAddrLen/2*3) bytes large to hold the output.
 *
 *              If dest == srcData, the data will be converted in place. If
 *              dest > srcData with overlap, the output is undefined.
 *              dest < srcData with overlap is accepted.
 *
 * @param *srcData Aligned 32-bit long int array containing the source data that
 *                 will be stripped of upper bytes.
 *
 * @param srcDataAddrLen Size of the srcData array, in program addresses. This
 *                       is equivalent to exactly half of the byte count of
 *                       srcData. This value should always be even as the input
 *                       array is specified in multiples of 4 bytes each.
 *
 * @return (srcDataAddrLen/2*3), which is the number of bytes written to *dest.
 */
unsigned int EZBL_RemovePhantoms(void *dest, const unsigned long *srcData, unsigned int srcDataAddrLen);


/**
 * Reads the present Flash contents and writes it in a .blob file format (with
 * zero SHA-1 hash field data) to a specified EZBL_FIFO.
 *
 * NOTE: Function and prototype are declared 'weak'. To call this function, you
 * must make a strong reference to it somewhere in your project by adding this
 * line at global scope to a .c file (after a #include for "ezbl.h"):
 *      EZBL_KeepSYM(EZBL_Flash2Blob);
 * Without a strong reference, EZBL_Flash2Blob() will resolve to a null function
 * pointer (behave similar to device reset at run time if not null checked).
 *
 * @param *blobOutputFile Pointer to an EZBL_FIFO to write the Flash contents
 *        formatted as a .blob file to.
 *
 * @return Number of bytes written to the *blobOutputFile destination.
 */
unsigned long EZBL_Flash2Blob(EZBL_FIFO *blobOutputFile);
static unsigned long __attribute__((weakref("EZBL_Flash2Blob"))) EZBL_Flash2BlobW(EZBL_FIFO *blobOutputFile);


#if defined(__XC16__)
/**
 * Erases the page containing the specified address. Be very careful if calling
 * this function as the function will erase any legal page, including ones
 * inside your bootloader and also possibly including the same page that this
 * function itself is stored on.
 *
 * EZBL_NVMKey must have been externally set to 0x03DF or this function will
 * do nothing. Immediately upon use or return, EZBL_NVMKey will be set to
 * 0x0000.
 *
 * Do not call this function on PIC24FxxKxxx, or dsPIC30F devices. They have
 * very different NVMCON bit encodings and are not supported at this time.
 *
 * NOTE: This function can take upwards of 45ms on some device families and
 * target pages to complete. At frequencies appreciably slower than the maximum
 * device frequency, even longer may be required. Therefore, you may need to
 * issue a ClrWdt() call before calling this function, assuming the Watchdog
 * timer is enabled. This also means that you should not configure a watchdog
 * timeout of less than ~64ms, even when you pre-clear the timeout. This
 * function does NOT internally clear the watchdog for you as it is packaged as
 * a library routine where not everyone would desire such behavior.
 *
 * @param addressOnPage 24-bit (unsigned long) specifying an address on the page
 *                      to be erased. Anywhere on the page will work as the
 *                      hardware will mask off the lower bits within the page.
 *
 * @return Page erase operation will have started if EZBL_NVMKey was properly
 *         set and no hardware Write Protect or other hardware features prevent
 *         it. This function will not block the CPU if the hardware supports a
 *         non-blocking page erase of the given address. You therefore should
 *         call EZBL_WaitForNVMOpComplete() before attempting to modify any
 *         NVMCON/NVMADR/NVMSRCADR/etc. state, including issuing any Table Write
 *         (tblwth, tblwtl) instructions in your code. All EZBL NVM erase/write
 *         operations include an internal check at their start that does the
 *         equivalent of EZBL_WaitForNVMOpComplete(), including this
 *         EZBL_ErasePage() function. Therefore, it is safe to call EZBL
 *         implemented NVM functions without doing an explicit call to
 *         EZBL_WaitForNVMOpComplete() beforehand.
 */
void EZBL_ErasePage(unsigned long addressOnPage);
static void __attribute__((weakref("EZBL_ErasePage"))) EZBL_ErasePageW(unsigned long addressOnPage);
#elif defined(__XC32__) || defined(__PIC32__)
// See pic32mm_flash_primitives.h instead for this function
#endif


/**
 * Erases all flash pages dedicated to the Application (address ranges contained
 * in the ".text.EZBLAppSpaceGeometry" section) in a piecewise fashion.
 *
 * Set <b>EZBL_NVMKey = 0xFC21</b> before each call to this function.
 *
 * Since erasing flash pages requires a long execution time, this function
 * returns between each page erase operation, allowing housekeeping and other
 * tasks to be serviced before starting another page erase operation.
 *
 * @param *eraseState pointer to an unsigned int to keep internal track of which
 *                    page was last erased/which page will be erased next.
 *
 *                    On first call, set this memory location to 0x0000. Ignore
 *                    the return value on subsequent calls, but return the same
 *                    value when continuing the erase sequence.
 * @return <ul>
 *              <li>1 - Erase sequence was initiated for a block of memory. One
 *                      or more calls to EZBL_EraseAppSpace() are required to
 *                      complete the erase.</li>
 *              <li>0 - Erase of all blocks in the App space geometry are
 *                      complete.</li>
 *         </ul>
 */
int EZBL_EraseAppSpace(unsigned int *eraseState);


/**
 * Erases all non-bootloader flash pages. The function generally blocks until
 * completion.
 *
 * Set <b>EZBL_NVMKey = 0xFC21</b> before calling this function.
 *
 * Pages that are already erased are skipped.
 * 
 * Device Configuration words in flash memory are also erased if the bootloader
 * has not reserved the entire last page. However, Config Word values defined in
 * the bootloader are automatically restored.
 * 
 * Config Bytes on devices with "Non-volatile" Config Words/Bytes are not erased
 * as they do not have an explicit erased state (i.e. they don't need to be
 * erased in order to reprogram them).
 *
 * @remarks EZBL_NVMKey
 *      The EZBL_NVMKey global variable must be set to a value of
 *      <b>0xFC21</b> before calling this function or the erase will be aborted.
 *      Upon success or abort return, EZBL_NVMKey will be cleared to 0x0000.
 *
 * @param clearWatchdog
 *      Flag specifying if the watchdog timer should be cleared before
 *      each page erase. A non-zero value will trigger a ClrWdt() call before
 *      each internal page erase operation.
 */
void EZBL_EraseAll(int clearWatchdog);


/**
 * Enumerates and erases all non-bootloader flash pages, returning between each
 * erase operation so the application has a chance to print status messages or
 * do other tasks instead of blocking continuously for the entire erase, which
 * could take a long time (ex: 7 seconds on a big device).
 * 
 * Set <b>EZBL_NVMKey = 0xFC21</b> before each call to this function.
 *
 * If any bootloader components get erased by this function, they are
 * automatically reprogrammed as soon as possible before returning. Flash 
 * configuration word values specified in the bootloader, assuming they are
 * allowed to be erased, would be restored.
 *
 * NOTE: This function can take upwards of 45ms on some device families and
 * target pages to complete (per call). At frequencies appreciably slower than
 * the maximum device frequency, even longer may be required. Therefore, you may
 * need to issue a ClrWdt() call before calling this function, assuming the
 * Watchdog timer is enabled. This also means that you should not configure a
 * watchdog timeout of less than ~64ms, even when you pre-clear the timeout.
 * This function does NOT internally clear the watchdog for you as it is
 * packaged as a library routine where not everyone would desire such behavior.
 *
 * @param lastAddressState
 *      Previous erase address state information returned by the prior call to
 *      EZBL_EraseAllPiecewise(). This is used to identify which Flash page
 *      should be erased. When calling for the first time, specify a value of 0.
 *
 * @remarks EZBL_NVMKey
 *      The EZBL_NVMKey global variable must be set to a value of
 *      <b>0xFC21</b> before calling this function or the erase will be aborted
 *      and the returned state information will not increment. Upon success or
 *      abort return, EZBL_NVMKey will be cleared to 0x0000.
 *
 * @return unsigned int
 *      Internal state information identifying what sector needs to be erased
 *      next. The order of returned addresses is not always sequential or
 *      guaranteed. However, on devices that have configuration words stored in
 *      flash and the bootloader allows erasing them, it is guaranteed that the
 *      page with the configuration words will be the last erased block.
 *
 *      The quantity of pages erased isn't constant as already erased pages are
 *      skipped.
 *
 *      When all pages have been erased, ((unsigned int)-1) will be returned,
 *      indicating the erase is complete and you should stop calling
 *      EZBL_EraseAllPiecewise().
 */
static inline unsigned int __attribute__((always_inline, optimize(1))) EZBL_EraseAllPiecewise(unsigned int lastAddressState)
{
    EZBL_EraseAppSpace(&lastAddressState);
    return lastAddressState;
}


/**
 * void EZBL_EraseInactivePartition(void);
 *
 * Starts erasing the entire inactive partition and then returns without waiting
 * for completion. NVMCON<WREN> remains set once the erase is done.
 *
 * Unlike most NVM functions, you do not have to set EZBL_NVMKey to any
 * particular value to allow the erase. You don't have to set it externally
 * since erasing the inactive partition won't normally brick your device. You
 * will have what you are currently running, so less paranoia is warranted.
 * Traditional single-boot applications shouldn't call this function, so the
 * linker will be able to remove this code and not store the NVM unlock sequence
 * so close to the NVM unlock operation.
 *
 * Upon function return, EZBL_NVMKey will be cleared to 0x0000.
 *
 * To check if the erase is done, check NVMCON<WR> or NVMCONbits.WR. Zero means
 * the erase is done.
 *
 * If this function is called while a prior NVM Flash erase or write operation is
 * already in progress, the function blocks until the prior operation completes.
 * The erase operation is then started before returning.
 */
void EZBL_EraseInactivePartition(void);
static void __attribute__((weakref("EZBL_EraseInactivePartition"))) EZBL_EraseInactivePartitionW(void);


#if defined(__XC16__)
/**
 * Programs an array of bytes into Flash or Config Word memory at the specified
 * Program Address.
 *
 * Before calling this function, set <b>EZBL_NVMKey = 0x03DF</b> to permit
 * the Flash write. Any other value in EZBL_NVMKey will cause all function
 * behavior to execute, but without actually writing anything to Flash.
 *
 *
 * The operation blocks for all but the last Flash write block's write
 * operation. If this function is called while a prior non-blocking NVM Flash
 * erase or write operation is already in progress, the function safely blocks
 * until the prior operation completes.
 *
 * Interrupts are disabled for a minimal time (estimated 19 to 38 instruction
 * cycles max, depending on target device) before starting each Flash
 * word/double word programming operation. This is done by temporarily raising
 * to CPU IPL8, so if you don't want interrupts to execute at all for the entire
 * write, use the INTCON2<GIE> bit or raise the CPU to IPL7 before calling this
 * function.
 *
 * In timing critical applications, an optional
 *     near unsigned int *EZBL_IntDisReqReg;
 * global variable should be declared and initialized to point to an IFSx
 * interrupt flags SFR address or RAM location to facilitate synchronization of
 * when precisely this function should disable interrupts and perform a queued
 * write operation. When enabled, bit 0 of the SFR or RAM location will be set
 * when EZBL_WriteROM[Ex]() needs to disable interrupts, and then the code will
 * block until someone outside the called context clears the same bit.
 * Generally, a time sensitive ISR will clear the bit each time it finishes
 * execution, allowing interrupts to be disabled for the NVM operation only when
 * an NVM write operation is least likely to interfere with concurrent
 * application functionality.
 *
 * At global scope, use:
 *     EZBL_SetSYM(EZBL_IntDisReqFlagPos, <i>i</i>);
 * where '<i>i</i>' is a link-time integer constant between 0 and 15 in order to
 * change the bit at EZBL_IntDisReqReg's pointed location which synchronization
 * is performed with.
 *
 *
 * In Bootloader or other applications where some of the device's program memory
 * map should never be programmed or altered, the applicable address range(s)
 * should be set by using the:
 *     EZBL_SetNoProgramRange(<i>startProgramAddr</i>, <i>endProgramAddr</i>);
 * macro declared at global scope. The start address is inclusive while the end
 * address is exclusive (i.e. the first address that IS programable).
 *
 * This macro will result in all EZBL_WriteROM[Ex]() calls being checked against
 * the given range and piecewise skipped if overlapping.
 * EZBL_SetNoProgramRange() can be instanced multiple times to define
 * discontinuous address ranges that need to be write-protected.
 *
 * Skipping occurs based on the minimum Flash write granularity applicable for
 * the device. For example, on devices that can program 48-bit single
 * instruction words, a no-program range of [0x000230, 0x000240), and a
 * EZBL_WriteROM[Ex]() call to program [0x000200, 0x000300) will result in data
 * being programmed to [0x000200, 0x000230) and [0x000240, 0x000300) with the
 * input source data for the no-program range being read but thrown away to
 * maintain logical stream synchronization.
 *
 *
 * This function can be used to program "Volatile" Flash based Configuration
 * Words stored in the last page of Flash memory. It will also program
 * "Non-volatile" type Config Words (exist in the 0xF8xxxx address range).
 *
 * This function will program Auxiliary Flash and Inactive Panel Flash if it
 * exists on the device.
 *
 * On devices that implement flash ECC, you must not attempt to program the same
 * Flash word location more than once per erase (i.e. all such bytes must be
 * masked as 0xFF if they exist in the destination address range). The
 * EZBL_WriteROMOnlyOnce() API can be used to help prevent writes to already
 * programmed flash words.
 *
 * On devices without ECC, programming a preexisting flash word a second time
 * might be permissable, but is illegal on many devices because it is outside
 * the specified flash operating characteristics I.e. doing so too many times
 * may degrade the flash cells and invalidate retention and endurance
 * specifications. Programming a Flash word 3+ times without erasing it first is
 * not allowed on any device (although this function does not prevent you from
 * doing so).
 *
 * @param destProgAddr
 *   Address in Program Memory to start writing to. This parameter must be
 *   a legal memory address aligned to the native flash word width (3 or 6 bytes,
 *   which equates to 0x2 or 0x4 addresses), or, if targeting non-volatile
 *   Config Words, aligned to 3 bytes/0x2 addresses. Bit 0 must be cleared.
 *
 * @param srcData
 *   Pointer to the source data in RAM to write to flash or Config Words. The
 *   data to write must be packed in RAM without "phantom bytes", ex:
 *       srcData[0]: first instruction (or first Config Word byte), low byte
 *       srcData[1]: first instruction (or dummy byte for Config Words), middle byte
 *       srcData[2]: first instruction (or dummy byte for Config Words), high byte
 *       srcData[3]: second instruction (or second Config Word byte), low byte
 *       srcData[4]: second instruction (or dummy byte for Config Words), middle byte
 *       srcData[5]: second instruction (or dummy byte for Config Words), high byte
 *
 * @param byteCount
 *   Number of bytes to write to Flash or Config Words. If the data length is
 *   not an integer multiple of the native Flash word or Config Word write size,
 *   the bytes in the last partial word are padded with 0xFF bytes and still
 *   programmed. This occurs without causing RAM addresses beyond the specified
 *   length from being accessed.
 *
 *   No error checking is done to ensure the destFlashAddress + effective number
 *   of addresses needed for byteCount is physically present on the device.
 *   Writing to unimplemented target addresses is normally harmlessly ignored,
 *   but can cause an Address Error Trap upon read-back.
 *
 * @return
 *   Returns the next sequential Flash word's memory address. In other words:
 *   (flashWriteAddress + (byteCount+2)/3*2) or
 *   (flashWriteAddress + (byteCount+5)/6*4) depending on the device and
 *   destination memory range.
 *
 *   The value contained in EZBL_NVMKey is cleared to 0x0000.
 *
 *   NVMCON<WR> and NVMCON<WREN> may be cleared upon return, depending upon if
 *   the final write has completed or not by the time the return code executes.
 *   Other status and control bits in NVMCON, if needed, should be preserved
 *   outside this function as they will also be cleared.
 */
unsigned long EZBL_WriteROM(unsigned long destProgAddr, const void *srcData, unsigned int byteCount);
static unsigned long __attribute__((weakref("EZBL_WriteROM"))) EZBL_WriteROMW(unsigned long destProgAddr, const void *srcData, unsigned int byteCount);   // Weak function reference to EZBL_WriteROM()
#elif defined(__XC32__) || defined(__PIC32__)


/**
 * Programs an array of data into Flash at the specified starting address.
 * Alignment, padding, boundary crossing, and existing data erase (with
 * preservation of adjacent data) are handled internally.
 *
 * @param targetAddr Target address to write to, expressed in integral form. It
 *                   is generally best to only write to Flash double word
 *                   aligned addresses, but most any legal memory location,
 *                   including RAM and SFR addresses can be targeted.
 *
 *                   Value can be a physical address or any kseg0 or kseg1
 *                   virtual addresses. The address will be converted to an
 *                   appropriate form for writing, depending upon the hardware
 *                   required to carry out the write.
 *
 * @param *data Source data array to copy from. Operation is undefined if this
 *              source address overlaps the target address or if the hardware
 *              cannot support the operation (ex: can't row-program Flash using
 *              another Flash address as the source data).
 *
 * @param dataLen Number of bytes to copy from *data to targetAddr. The actual
 *                number of bytes written to the target can be greater than this
 *                if padding/realignment are required to carry out the write.
 *
 * @return 1 on apparent success. However, a read-back operation is not
 *         performed to confirm the requested data now exists at the target, so
 *         do not rely on a positive return code alone.
 *
 *         0 or a negative value on failure.
 */
int EZBL_WriteMemory(unsigned long targetAddr, const void *data, unsigned long dataLen);

#define EZBL_WriteROM(destProgAddr, srcData, byteCount)  EZBL_WriteROMEx(destProgAddr, srcData, byteCount, 0, 0, 0)
#endif


/**
 * Programs an array of bytes into Flash or Config Word memory at the specified
 * Program Address.
 *
 * Before calling this function, set <b>EZBL_NVMKey = 0x03DF</b> to permit
 * the flash write. Any other value in EZBL_NVMKey will cause all function
 * behavior to execute, but without actually writing anything to Flash.
 *
 *
 * The operation blocks for all but the last flash write block's write
 * operation. If this function is called while a prior non-blocking NVM flash
 * erase or write operation is already in progress, the function blocks until
 * the prior operation completes.
 *
 * Interrupts are disabled for a minimal time (estimated 19 to 38 instruction
 * cycles, depending on target device) before starting each Flash
 * word/double word programming operation. This is done by temporarily raising
 * to CPU IPL8.
 * 
 * If you don't want interrupts to execute at all for the entire programming
 * sequence, call EZBL_DisableInterrupts() before calling this function.
 *
 * In timing critical applications, an optional
 *     volatile unsigned int * __attribute__((near)) EZBL_IntDisReqReg;
 * global variable should be declared and initialized to point to an IFSx
 * interrupt flags SFR address or RAM location to facilitate synchronization of
 * when precisely this function is allowed to disable interrupts and perform a
 * queued write operation. When enabled, bit 0 (or EZBL_IntDisReqFlagPos) of the
 * SFR/RAM pointer target will be set when EZBL_WriteROM[Ex]() needs to disable
 * interrupts, and then the code will block until something outside the
 * EZBL_WriteROM[Ex]() function clears the same bit.
 * 
 * Generally, a time sensitive ISR would/should clear the bit each time it
 * finishes execution, allowing interrupts to be disabled for the NVM operation
 * only when an NVM write operation is least likely to interfere with concurrent
 * application functionality.
 *
 * To specify which bit in (*EZBL_IntDisReqReg) will be used for synchronization 
 * (instead of the default of bit 0), at global scope, write:
 *     EZBL_SetSYM(EZBL_IntDisReqFlagPos, <i>i</i>);
 * where '<i>i</i>' is a link-time integer constant between 0 and 15.
 *
 *
 * In Bootloader or other applications where some of the device's program memory
 * map should never be programmed or altered, the applicable address range(s)
 * should be set by using the:
 *     EZBL_SetNoProgramRange(<i>startProgramAddr</i>, <i>endProgramAddr</i>);
 * macro declared at global scope. The start address is inclusive while the end
 * address is exclusive (i.e. the first address that IS programable).
 *
 * This macro will result in all EZBL_WriteROM[Ex]() calls being checked against
 * the given range and piecewise skipped if overlapping.
 * EZBL_SetNoProgramRange() can be instanced multiple times to define
 * discontinuous address ranges that need to be write-protected.
 *
 * Skipping occurs based on the minimum flash write granularity applicable for
 * the device. For example, on devices that can program 48-bit single
 * instruction words, a no-program range of [0x000230, 0x000240), and a
 * EZBL_WriteROM[Ex]() call to program [0x000200, 0x000300) will result in data
 * being programmed to [0x000200, 0x000230) and [0x000240, 0x000300) with the
 * input source data for the no-program range being read but thrown away to
 * maintain logical stream synchronization.
 *
 *
 * This function can be used to program "Volatile" flash based Configuration
 * Words stored in the last page of Flash memory. It will also program
 * "Non-volatile" type Config Words (exist in the 0xF8xxxx address range).
 *
 * This function will program Auxiliary Flash and Inactive Panel Flash if it
 * exists on the device and is not otherwise disallowed due to Config word
 * security settings.
 *
 * On devices that implement flash ECC, you must not attempt to program the same
 * flash word location more than once per erase. Do so will corrupt the internal
 * ECC bits and lead to a subsequent trap exception when attempting to execute 
 * or anything on the same flash word. The EZBL_WriteROMOnlyOnce() API can be
 * used to pre-read the flash contents and mask off write data that would lead
 * to an ECC violation.
 *
 * On devices without ECC, programming a flash word a second time might be
 * permissable, but is illegal on many devices because it is outside the
 * specified flash operating characteristics I.e. doing so too many times may
 * degrade the flash cells and invalidate retention/endurance specifications.
 * Programming a flash word 3+ times without erasing it first is not allowed on
 * any device (although this function does not prevent you from doing so).
 *
 * @param destProgAddr
 *   Address in Program Memory to start writing to. This parameter must be
 *   a legal memory address aligned to the native flash word width (0x2 or 0x4 
 *   addresses, which equates to 3 or 6 bytes), or, if targeting non-volatile
 *   Config Words, aligned to 3 bytes/0x2 addresses.
 *
 *   As all flash word addresses are even, this implies bit 0 of destProgAddr
 *   must always be '0'.
 *
 * @param srcData
 *   Pointer to the source data in RAM to write to flash or Config Words. The
 *   data to write must be packed in RAM without "phantom bytes", ex:
 *       srcData[0]: first instruction (or first Config Word byte), low byte
 *       srcData[1]: first instruction (or dummy byte for Config Words), middle byte
 *       srcData[2]: first instruction (or dummy byte for Config Words), high byte
 *       srcData[3]: second instruction (or second Config Word byte), low byte
 *       srcData[4]: second instruction (or dummy byte for Config Words), middle byte
 *       srcData[5]: second instruction (or dummy byte for Config Words), high byte
 *
 *   Use EZBL_RemovePhantoms() if your source data must first be converted to
 *   the required packed form.
 *
 * @param byteCount
 *   Number of bytes to write to Flash or Config Words. If the data length is
 *   not an integer multiple of the native Flash word or Config Word write size,
 *   the bytes in the last partial word are padded with 0xFF bytes and still
 *   programmed. This occurs without causing RAM addresses beyond the specified
 *   length from being accessed.
 *
 *   No error checking is done to ensure the destProgAddr + effective number
 *   of addresses needed for byteCount is physically present on the device.
 *   Writing to unimplemented target addresses is normally harmlessly ignored,
 *   but can cause an Address Error Trap upon read-back.
 *
 * @param flags
 *   Bitwise OR'd collection of flags to modify the writing behavior. Available
 *   options are:
 *   <ul>
 *      <li>EZBL_FLAG_CLRWDT (0x0400)           - Clear watchdog before each write</li>
 *      <li>EZBL_FLAG_LATE_INT_EN (0x0800)      - Restore interrupts only after NVMOP completes. By default interrupts are disabled only for the unlock sequence that sets NVMCON<WR>.</li>
 *      <li>EZBL_FLAG_IPARTITION (0x1000)       - Force 0x400000-0x7FFFFF Inactive Partition address range (set bit 22 of destProgAddr)</li>
 *      <li>EZBL_FLAG_FIXED_PARTITION (0x2000)  - Use fixed Partition 1/Partition 2 target address range for destProgAddr instead of Active/Inactive relative addresses (XOR bit 22 of flashWriteAddress with !NVMCON<10>, which is !P2ACTIV on Dual Partition capable devices)</li>
 *   </ul>
 *
 * @param extraNoProgRanges
 *   Number of address pairs at noProgRanges to use for excluding specific
 *   addresses from being written.
 *
 *   Every 1 extraNoProgRanges implies 8 bytes of RAM at *noProgRanges will
 *   be read. Set to 0 if the default .text.EZBLNoProgramRanges section data
 *   should be used without anything extra.
 *
 *   The special value 0xDEAD (-8531) can be given to disable all no-program
 *   range checking, ignoring all contents in .text.EZBLNoProgramRanges and
 *   ignoring the *noProgRanges parameter. Use this with caution as it will
 *   allow overwriting of any device address, including this function and
 *   bootloader code, potentially bricking the device.
 *
 * @param *noProgRanges pointer to array 32-bit address pairs in RAM which
 *                      should be skipped during writing flash if found to
 *                      overlap the dest address write range.
 *
 *                      This parameter is ignored if
 *                      extraNoProgRanges == 0 or 0xDEAD.
 *
 * @return
 *   Returns the next sequential flash word's memory address. In other words:
 *   (destProgAddr + (byteCount+2)/3*2) or
 *   (destProgAddr + (byteCount+5)/6*4) depending on the device and
 *   destination memory range.
 *
 *   The value contained in EZBL_NVMKey is cleared to 0x0000.
 *
 *   NVMCON<WR> and NVMCON<WREN> may be cleared upon return, depending upon if
 *   the final write has completed or not by the time the return code executes.
 *   Other status and control bits in NVMCON, if needed, should be preserved
 *   outside this function as they will also be cleared.
 */
unsigned long EZBL_WriteROMEx(unsigned long destProgAddr, const void *srcData, unsigned int byteCount, unsigned int flags, int extraNoProgRanges, const unsigned long *noProgRanges);
static unsigned long __attribute__((weakref("EZBL_WriteROMEx"))) EZBL_WriteROMExW(unsigned long destProgAddr, const void *srcData, unsigned int byteCount, unsigned int flags, int extraNoProgRanges, const unsigned long *noProgRanges);

#define EZBL_FLAG_CLRWDT            0x0400u // Clear watchdog before each write
#define EZBL_FLAG_LATE_INT_EN       0x0800u // Restore interrupts only after NVMOP completes
#define EZBL_FLAG_IPARTITION        0x1000u // Force 0x400000-0x7FFFFF Inactive Partition address range (set bit 22 of destFlashAddress)
#define EZBL_FLAG_FIXED_PARTITION   0x2000u // Use fixed Partition 1/Partition 2 target address range for destFlashAddress instead of Active/Inactive relative addresses (XOR bit 22 of flashWriteAddress with !NVMCON<10>, which is !P2ACTIV on Dual Partition capable devices)



#if defined(__XC16__)
/**
 * DEPRECATED: Use EZBL_WriteROM() or EZBL_WriteROMEx() instead, which do
 * address checking against .text.EZBLNoProgramRanges table data instead.
 *
 * Identical to EZBL_WriteROM() except that the address range is checked
 * first for overlap with any bootloader addresses. Like EZBL_WriteROM(),
 * set EZBL_NVMKey = 0x03DF to permit the write. Any other value in
 * EZBL_NVMKey will cause all function behavior to execute, but without
 * actually writing anything to Flash or Config Words.
 *
 * If any overlap in Flash memory is detected, the overlapped region data is
 * converted to all '1's which is then skipped when Flash programming to
 * preserve all bootloader memory region contents.
 *
 * If any overlap in Non-volatile Config Word memory forced by the bootloader is
 * detected (addresses in 0xF8xxxx range), the overlapped region data is
 * converted to match the values coded in the bootloader so as not to allow the
 * bootloader specified values to change. This is done instead of all '1's
 * masking since all '1's is a valid Configuration Word value and can't be
 * assumed to already exist due to a prior erase operation. I.e. writing all
 * '1's are not skipped during programming.
 *
 * @param destProgAddr 24-bit unsigned long program memory address to start
 *                     writing to. The LSbit must always be '0'. When
 *                     programming flash memory with a native 48-bit flash word
 *                     size, the second LSbit must also be '0' to align to the
 *                     needed 0x4 boundary.
 *
 * @param *srcData Pointer to source data in RAM which should be copied to Flash
 *                 or Config Words. This data should be packed in 24-bit chunks
 *                 without phantom bytes. srcData must be writable. The data can
 *                 change while masking off overlapped bootloader regions. This
 *                 pointer does not have any alignment requirements.
 *
 * @param byteCount unsigned integer specifying how many bytes to sequentially
 *                  copy from *srcData to the Program Memory address range. This
 *                  value need not result in write termination on a perfect
 *                  flash word or Config Word address boundary. Automatic 0xFF
 *                  padding will be done to fill the word before programming.
 *
 * @return 24-bit unsigned long program memory address after the just programmed
 *         Flash Word or Config Word. *srcData RAM contents may also have
 *         changed according to the bootloader masking behavior.
 */
unsigned long EZBL_WriteROMChecked(unsigned long destProgAddr, void *srcData, unsigned int byteCount);
static unsigned long __attribute__((weakref("EZBL_WriteROMChecked"))) EZBL_WriteROMCheckedW(unsigned long destProgAddr, void *srcData, unsigned int byteCount);
#elif defined(__XC32__) || defined(__PIC32__)
#endif


#if defined(__XC16__)
/**
 * Identical to EZBL_WriteROM() except that the target flash range is read first
 * and any already programmed flash words are individually masked to all '1's in
 * the input data to suppress their write effect.
 *
 * Set <b>EZBL_NVMKey = 0x03DF</b> before calling this function. Any other value
 * will suppress the Flash write.
 *
 * If the Flash already contains the correct data in the specified addresses, or
 * if it is not possible to write the correct data (i.e. requires clearing a
 * preexisting '0' back to '1'), then the Flash write for the given word (24 or
 * 48-bits, depending on device) is skipped. srcData must be in RAM and must be
 * writable. The data can change (changes to 0xFFFFFF or 0xFFFFFFFFFFFF anywhere
 * a Flash word cannot or does not need to be programmed).
 */
unsigned long EZBL_WriteROMOnlyOnce(unsigned long destProgAddr, void *srcData, unsigned int byteCount);
static unsigned long __attribute__((weakref("EZBL_WriteROMOnlyOnce"))) EZBL_WriteROMOnlyOnceW(unsigned long destProgAddr, void *srcData, unsigned int byteCount);
#elif defined(__XC32__) || defined(__PIC32__)
#endif


#if defined(__XC16__)
/**
 * Identical to EZBL_WriteROMEx() except that the target flash range is read
 * first and any already programmed flash words are individually masked to all
 * '1's in the input data to suppress their write effect.
 *
 * Set <b>EZBL_NVMKey = 0x03DF</b> before calling this function. Any other value
 * will suppress the Flash write.
 *
 * If the flash already contains the correct data in the specified addresses, or
 * if it is not possible to write the correct data (i.e. requires clearing a
 * preexisting '0' back to '1'), then the Flash write for the given word (24 or
 * 48-bits, depending on device) is skipped. srcData must be in RAM and must be
 * writable. The data can change (changes to 0xFFFFFF or 0xFFFFFFFFFFFF anywhere
 * a Flash word cannot or does not need to be programmed).
 */
unsigned long EZBL_WriteROMOnlyOnceEx(unsigned long destProgAddr, void *srcData, unsigned int byteCount, unsigned int flags, int extraNoProgRanges, unsigned long *noProgRanges);
static unsigned long __attribute__((weakref("EZBL_WriteROMOnlyOnceEx"))) EZBL_WriteROMOnlyOnceExW(unsigned long destProgAddr, void *srcData, unsigned int byteCount, unsigned int flags, int extraNoProgRanges, unsigned long *noProgRanges);
#elif defined(__XC32__) || defined(__PIC32__)
#endif


#if defined(__XC16__)
/**
 * Writes an array of data bytes to flash, preserving all data around the array
 * like a byte writable EEPROM or RAM memory. If required, sequential page
 * read-modify-erase-write operations are performed.
 *
 * To restrict writing/erase-rewriting to FlashHoles and Application space, set
 * <b>EZBL_NVMKey = 0x03DF;</b> prior to calling this function. To force writing
 * to any flash address, including Bootloader reserved ranges, set
 * EZBL_NVMKey = 0xDEAD;
 *
 * To preserve flash endurance, save execution time, and avoid potential data
 * loss on device reset, the destination Flash range is first compared with the
 * input data and the function returns immediately if the destination range
 * already exactly matches the input data.
 *
 * Likewise, if the flash range is blank (contains all 0xFFFFFF_FFFFFF flash
 * double words in the required write addresses), then the data is written
 * directly into position, skipping any page erase steps.
 *
 * If the destination range cannot be written due to existing mismatched data, a
 * full Read-Modify-Erase-Write sequence is performed on a page-by-page basis.
 * This entails allocating a buffer on the stack to contain a full flash erase
 * page worth of data, which is typically 1536 or 3072 bytes long, depending on
 * device (set by EZBL_ADDRESSES_PER_SECTOR linker symbol).
 *
 * Execution time may be quite long and if there is any risk of running out of
 * RAM, the caller should disable interrupts or take other precautions to ensure
 * the required stack RAM space is available.
 *
 * @param destPgmAddr 24-bit flash program space destination address. This
 *                    address must be an even, instruction word aligned, but can
 *                    straddle a native flash double word boundary on devices
 *                    that have ECC or program in 48-bit flash double word
 *                    sized chunks.
 *
 *                    To minimize flash endurance loss and execution time, it is
 *                    best if any object written is allocated to a double word
 *                    aligned address (i.e. multiple of 0x4 addresses). This
 *                    ensures no two objects will share a flash double word and
 *                    require extra erase/write penalties when writing the
 *                    objects via sequential EZBL_ModifyROM() calls.
 *
 * @param srcData Data space pointer to an array of bytes to write. If this
 *                parameter is null, no erase or write operation takes place.
 *
 * @param byteLen Number of bytes to write to flash. 0 is allowed, as are values
 *                less than the native minimum flash programming word size. In
 *                such cases, all existing data in the shared bytes within the
 *                programming word are preserved unchanged.
 *
 * @return Flash contains new data in the specified address range.
 *
 *         EZBL_NVMKey is reset to 0x0000, even if the early data compare test
 *         resulted in skipping of all Flash Erase/Write operations, byteLen is
 *         zero, or srcData is null.
 */
void EZBL_ModifyROM(unsigned long destPgmAddr, void *srcData, unsigned int byteLen);
#elif defined(__XC32__) || defined(__PIC32__)
/**
 * Writes an array of data bytes to flash, preserving all data around the array
 * like a byte writable EEPROM or RAM memory. If required, sequential page
 * read-modify-erase-write operations are performed.
 *
 * To restrict writing/erase-rewriting to FlashHoles and Application space, set
 * <b>EZBL_NVMKey = 0x03DF;</b> prior to calling this function. To force writing
 * to any flash address, including Bootloader reserved ranges, set
 * EZBL_NVMKey = 0xDEAD;
 *
 * To preserve flash endurance, save execution time, and avoid potential data
 * loss on device reset, the destination Flash range is first compared with the
 * input data and the function returns immediately if the destination range
 * already exactly matches the input data.
 *
 * Likewise, if the flash range is blank (contains all 0xFFFFFF_FFFFFF flash
 * double words in the required write addresses), then the data is written
 * directly into position, skipping any page erase steps.
 *
 * If the destination range cannot be written due to existing mismatched data, a
 * full Read-Modify-Erase-Write sequence is performed on a page-by-page basis.
 * This entails allocating a buffer on the stack to contain a full flash erase
 * page worth of data, which is typically 1536 or 3072 bytes long, depending on
 * device (set by EZBL_ADDRESSES_PER_SECTOR linker symbol).
 *
 * Execution time may be quite long and if there is any risk of running out of
 * RAM, the caller should disable interrupts or take other precautions to ensure
 * the required stack RAM space is available.
 *
 * @param destPgmAddr 24-bit flash program space destination address. This
 *                    address must be an even, instruction word aligned, but can
 *                    straddle a native flash double word boundary on devices
 *                    that have ECC or program in 48-bit flash double word
 *                    sized chunks.
 *
 *                    To minimize flash endurance loss and execution time, it is
 *                    best if any object written is allocated to a double word
 *                    aligned address (i.e. multiple of 0x4 addresses). This
 *                    ensures no two objects will share a flash double word and
 *                    require extra erase/write penalties when writing the
 *                    objects via sequential EZBL_ModifyROM() calls.
 *
 * @param srcData Data space pointer to an array of bytes to write. If this
 *                parameter is null, no erase or write operation takes place.
 *
 * @param byteLen Number of bytes to write to flash. 0 is allowed, as are values
 *                less than the native minimum flash programming word size. In
 *                such cases, all existing data in the shared bytes within the
 *                programming word are preserved unchanged.
 *
 * @return Flash contains new data in the specified address range.
 *
 *         EZBL_NVMKey is reset to 0x0000, even if the early data compare test
 *         resulted in skipping of all Flash Erase/Write operations, byteLen is
 *         zero, or srcData is null.
 */
//#define EZBL_ModifyROM(destPgmAddr, srcData, byteLen)   EZBL_WriteROMEx(destPgmAddr, srcData, byteLen)
#endif


/**
 * Tests if all bytes on a given Flash Page are erased. Returns 1 if the entire
 * Page has '1's on it, or 0 otherwise. No special address checking or
 * conditions are exempted.
 *
 * Execution time is about 18 + 9,728 cycles/0x400 sized Flash page on
 * dsPIC33E/PIC24E/dsPIC33C Flash, or about 11 + 5,632 cycles/0x400
 * sized Flash page on PIC24F/PIC24H/dsPIC33F/dsPIC33C-PRAM devices. These
 * assumes the page is blank. Non-blank pages will return earlier.
 *
 * NOTE: This function requires the _EZBL_ADDRESSES_PER_SECTOR link time symbol
 * to be set to the number of program addresses that represents one Flash Erase
 * Page on your hardware. On most devices, this is either 0x400 or 0x800 and
 * will be automatically set by ezbl_tools.jar in the project's .gld linker
 * script or by the Bootloader project you link to. However, if you are not
 * using ezbl_tools.jar or an existing EZBL Bootloader, you can manually define
 * this value by using the EZBL_SetSYM() macro in one of your C source files:
 *      <p><code>EZBL_SetSYM(EZBL_ADDRESSES_PER_SECTOR, 0x400);</code></p>
 *
 * 0x400 program addresses is equivalent to 1536 bytes, or 512 24-bit
 * instruction words. Be sure and check the size in the device data sheet when
 * manually assigning this value.
 *
 * @param programAddress Flash program memory address to check. This address
 *                       does not have to be page aligned. All bytes on the page
 *                       will still be checked if an unaligned (or odd) address
 *                       is provided.
 *
 * @return  <ul>
 *              <li>1 - Page is erased and contains all 0xFF bytes</li>
 *              <li>0 - Page contains one or more '0' bits on it</li>
 *          </ul>
 */
int EZBL_IsPageErased(unsigned long programAddress);
static int __attribute__((weakref("EZBL_IsPageErased"))) EZBL_IsPageErasedW(unsigned long programAddress);


#if defined(__XC16__)
/**
 * Finds the first Flash page containing all 0xFFFFFF contents (i.e. is
 * completely erased).
 *
 * @param startAddress Program Flash address to start searching from. The page
 *                     that contains this address is tested first.
 * @param searchDirection
 *                     > 1 Searching will ascend to bigger addresses and
 *                     stop at the maximum legal Flash address applicable on the
 *                     device or Flash partition (for devices in a Dual
 *                     Partition mode or with Auxiliary Flash) that the
 *                     startAddress resides on.
 *
 *                     == 0 Reserved
 *
 *                     <= -1 Searching will descend to lower addresses until
 *                     address 0x000000 is reached or the first legal address of
 *                     the startAddress's physical partition.
 *
 * @return             Address of the first address of the first Flash Page
 *                     encountered that is entirely blank. If no erased Flash
 *                     pages are found, 0xFFFFFFFF is returned.
 */
unsigned long EZBL_FindFirstErasedPage(unsigned long startAddress, int searchDirection);
static unsigned long __attribute__((weakref("EZBL_FindFirstErasedPage"))) EZBL_FindFirstErasedPageW(unsigned long startAddress, int searchDirection);
#elif defined(__XC32__) || defined(__PIC32__)
#endif



/**
 * Computes or continues a CRC-32 (IEEE 802.3 Ethernet version) over a stream of
 * bytes.
 *
 * The polynomial implemented is:
 * x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1
 * Commonly this is represented as 0xEDB88320 (or 0x04C11DB7 for big endian).
 *
 * @param crcSeed Starting value to use in the shift register. Set to 0x00000000
 *                when starting a new CRC32 computation.
 * @param data Pointer to memory to start reading from. There are no alignment
 *             restrictions.
 * @param byteLength Count of how many bytes to include in this CRC computation
 *                   starting at the specified pointer address.
 *
 * @return 32-bit CRC computed. Pass this value back in for the crcSeed
 *         parameter when continuing a checksum with more data.
 */
unsigned long EZBL_CRC32(unsigned long crcSeed, const void *data, unsigned int byteLength);


#if defined(__XC16__)
/**
 * Computes or continues a CRC-32 (IEEE 802.3 Ethernet version) over Flash data
 *
 * The polynomial implemented is:
 * x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1
 * Commonly this is represented as 0xEDB88320 (or 0x04C11DB7 for big endian).
 *
 * This function needs ~120 bytes peak of temporary stack RAM.
 *
 * @param crcSeed starting value to use in the shift register. Set to 0x00000000
 *                when starting a new CRC32 computation.
 * @param programAddress Program memory address to start reading from. This must
 *                       be aligned to an instruction boundary (i.e. always even
 *                       numbered) on 16-bit PICs/dsPICs.
 * @param byteLength unsigned long count of how many bytes to include in this
 *                   CRC computation starting at the specified address. This
 *                   need not be an integral multiple of the underlying Flash
 *                   storage instruction or word size.
 *
 * @return 32-bit CRC computed
 */
unsigned long EZBL_CRC32Prog(unsigned long crcSeed, unsigned long programAddress, unsigned long byteLength);
#elif defined(__XC32__) || defined(__PIC32__)
#define EZBL_CRC32Prog(crcSeed, readAddress, byteLength)     EZBL_CRC32(crcSeed, (const void*)(readAddress), byteLength)
#endif


/**
 * unsigned long EZBL_CRC32NVRegions(unsigned int typeOfRegions);
 *
 * Computes a CRC-32 (IEEE 802.3 Ethernet version) over all Flash and
 * non-volatile addresses containing the Bootloader and reserved data,
 * the Application (any non-Bootloader area), or the entire device's implemented
 * non-volatile memories (both).
 *
 * The .text.EZBLDevSpaceGeometry, .text.EZBLBootSpaceGeometry, and
 * .text.EZBLAppSpaceGeometry sections must be defined in your device linker
 * script for this function to be used. Otherwise a link-time error will be
 * triggered.
 *
 * @param typeOfRegions unsigned integer specifying which regions to include in
 *                      the CRC:
 *                          0 - all non-volatile addresses present on the device
 *                          1 - all bootloader occupied or reserved addresses
 *                          2 - all application occupied or free space addresses
 *                              (everything except bootloader occupied and
 *                              reserved addresses)
 *
 * @return Computed CRC32 over all of the regions occupied by the
 *         specified type.
 */
unsigned long EZBL_CRC32NVRegions(unsigned int typeOfRegions);


#if defined(__XC16__)
/**
 * Copies the specified program data from Master RAM into the specified slave
 * CPU's Program RAM (PRAM) address space.
 *
 * NOTE: The specified slave CPU core must be held in reset while this function
 *       executes. If reset has been released, use the EZBL_SlaveReset()
 *       function to reassert slave reset.
 *
 * @param cpuID Target Slave CPU identifier. Set to 1 for devices with only one
 *              slave CPU.
 *
 * @param targetPartition Target Partition on the Slave CPU to write the program
 *                        image to.
 *              <ul>
 *                  <li>0 - Write to default partition defined by the program
 *                          image data records.</li>
 *                  <li>1 - Ignore program image records and force the code into
 *                          the 0x000000 - 0x3FFFFF address range to target
 *                          Partition 1.</li>
 *                  <li>2 - Ignore program image records and force the code into
 *                          the 0x400000 - 0x7FFFFF address range to target
 *                          Partition 2.</li>
 *              </ul>
 *
 *
 * @param slaveDestAddress Address in the slave PRAM to write to. This value
 *                         must be 24-bit instruction word aligned (i.e. an even
 *                         number).
 *
 * @param srcData Pointer into master CPU RAM to read the source data from.
 *
 * @param byteLen Number of bytes to copy to the destination slave PRAM
 *                location. Due to ECC over each 48-bit PRAM word length,
 *                byteLen should be an integer multiple of 6 bytes.
 *                Non-multiples are allowed, but will result in 0xFF filler
 *                bytes being added to the beginning or end of the 48-bit PRAM
 *                word in order to commit the given bytes into the PRAM. These
 *                filler bytes can overwrite already existing data (which cannot
 *                be read beforehand), so ensure necessary alignment and padding
 *                is done outside this function if this behavior must be
 *                avoided.
 *
 * @return Number of bytes written to the slave Program-RAM.
 */
unsigned int EZBL_SlaveLoad(int cpuID, int targetPartition, unsigned long localSlvImageAddr);
unsigned int EZBL_SlaveLoadFromRAM(int cpuID, unsigned long slvTargetAddress, void *srcData, unsigned int dataLen);
#elif defined(__XC32__) || defined(__PIC32__)
#endif


#if defined(__XC16__)
/**
 * Asserts or deasserts reset on the specified slave CPU core.
 *
 * @param cpuID Slave CPU identifier. Must be set to 1 for devices with only one
 *              slave CPU.
 *
 * @param holdInReset <ul>
 *                      <li>0 - Slave released from reset and allowed to execute</li>
 *                      <li>1 - Slave held in reset</li>
 *                    </ul>
 *
 * @return Toggles the MSIxCON<SLVEN> bit as needed to hold the slave CPU
 *         core in reset or releases reset for ordinary slave execution. If the
 *         Slave is already in the requested state, then no change occurs.
 *
 *         Interrupts are globally disabled temporarily in order to set/clear
 *         the MSIxCON<SLVST> bit, which requires cycle exact/uninterrupted
 *         write access to the MSIxKEY register.
 */
void EZBL_SlaveReset(int cpuID, int holdInReset);
#elif defined(__XC32__) || defined(__PIC32__)
#endif


/**
 * extern volatile unsigned int __attribute__((near)) EZBL_NVMKey;
 *
 * RAM NVM Unlock Key Storage so that only code that is executing correctly will
 * be likely to be able to start a Flash erase/write operation.
 *
 * Set EZBL_NVMKey = 0x03DF to unlock EZBL Flash erase/write operations for:
 *      EZBL_ErasePage()
 *      EZBL_WriteROM()
 *      EZBL_WriteROMChecked()
 *      EZBL_WriteROMOnlyOnce()
 *      EZBL_WriteNVMCON()
 * Set EZBL_NVMKey = 0xFC21 for:
 *      EZBL_EraseAll()
 *      EZBL_EraseAllPiecewise()
 * All other values will prohibit any EZBL Flash erase/write command from
 * starting. EZBL_NVMKey will self-clear to 0x0000 immediately after
 * performing an NVMCON WR operation, so the proper key value must be set before
 * each applicable function call.
 *
 * Theory of operation:
 * When legal internal and external operational conditions exist for voltage,
 * temperature and frequency, the PIC will execute instructions 100% correctly
 * and reliably, allowing code to set EZBL_NVMKey to a needed value, execute a
 * number of subsequent instructions, and then start the hardware NVMCON
 * unlocking sequence to initiate a Flash erase or programming operation.
 *
 * When illegal but real life electrical conditions exist (such as:
 * over-clocking the PIC, high EMI/EMC/ESD/other noise events exist near by, BOR
 * disabled and VDD too low, extremely slow rising-falling-rising VDD ramp
 * conditions preventing POR, device temperature extremely high), it is
 * conceivable that the CPU will not stay in reset, but instead be attempting to
 * execute instructions. Some instructions read from Flash could have bit errors
 * and the ALU may compute wrong results, so a branch or call instruction
 * could branch to the wrong address (or perhaps the PC may increment by a value
 * other than 0x000002 addresses after executing an instruction). If the PC ever
 * ends up at the NVMCON manipulation code, having full unlock code there would
 * allow self-bricking via an unintended erase or programming operation
 * executing. By requiring this RAM state to be exactly right, using this
 * key state to compute the actual NVMCON hardware unlock sequence values, and
 * physically locating these pieces of code at different Flash addresses, a
 * considerably greater number of instructions must execute correctly before a
 * Flash command will succeed. The chance of field self-bricking failure is
 * theoretically reduced as only correct program flow will enable setting
 * NVMCON<WR>.
 */
extern volatile unsigned int XC16_NEAR EZBL_NVMKey;


#if defined(__XC16__)
/**
 * RAM bitfield controlling/indicating which interrupts that the Bootloader
 * implements should be passed to the Application's interrupt handler or
 * processed by the Bootloader's interrupt handler.
 *
 * This is a 32-bit global variable where each bit represents one IRQ/Vector
 * each, but pruned to only contain bits for interrupts actually implemented in
 * the Bootloader project. Both Application and Bootloader projects can read and
 * write bits within this variable at run time. At Bootloader reset, the CRT
 * will zero all bits in this "virtual RAM register", and at Application launch,
 * the contents defined by the Bootloader will stay unchanged (i.e. the
 * Application's CRT execution will not reset the contents of this virtual
 * register).
 *
 * - A '1' bit indicates the interrupt should be passed untouched and with
 *   minimal delay to the Application project. The Bootloader's ISR for the same
 *   interrupt will be ignored.
 * - A '0' bit indicates the interrupt would be handled by the Bootloader
 *   project's ISR and the Application project will never see the interrupt.
 * - All unused bits are don't care values.
 * - Being defined as an unsigned long, only 32-total Bootloader ISRs can be
 *   controlled with this global variable. If more than 32 ISRs are defined in
 *   your Bootloader project, linking of the Bootloader project will trigger a
 *   build-time error (and you should reconsider having so much code in your
 *   Bootloader project).
 * - The Application project always has dedicated access to all hardware
 *   interrupts that are not run-time selected through this virtual register, so
 *   implementing interrupt handlers in your Bootloader will never preclude
 *   future use of the same hardware in Application projects.
 *
 * As the bit positions and their meanings depend on the ISRs implemented in
 * your Bootloader project, the mapping of bitfields to named interrupts is only
 * pseudo constant. Therefore this variable should not be written to directly.
 * Instead, use the EZBL_ForwardIntToApp(), EZBL_ForwardIntToBoot(),
 * EZBL_ForwardAllIntToApp() and EZBL_ForwardAllIntToBoot() macros to manipulate
 * this pseudo register. However, it is possible to save and restore the whole
 * value as an atomic unit.
 *
 * The parameter required by the EZBL_ForwardIntTo[App/Boot]() macros is the
 * XC16 ISR name of the interrupt, but without a leading underscore and without
 * the 'Interrupt' suffix on it. Ex: an _U2RXInterrupt() ISR is referenced in
 * these macros by the name "U2RX", without quotation marks.
 *
 * To forward the U2RX Interrupt to the Application, you may write
 * your "begin forwarding" code as:
 *      <code>
 *      #include "ezbl_integration/ezbl.h"
 *      ...
 *      EZBL_ForwardIntToApp(U2RX);
 *      </code>
 *
 * To terminate forwarding and let the Bootloader handle the U2RX interrupt,
 * you would atomically clear the same bit without affecting the others:
 *      <code>
 *      EZBL_ForwardIntToBoot(U2RX);
 *      </code>
 *
 * Changes to the bits in this virtual register have immediate effect, assuming
 * global interrupts and the corresponding interrupt enable flag in the IECx
 * registers is set.
 */
extern volatile unsigned long XC16_NEAR __attribute__((keep)) EZBL_ForwardBootloaderISR;
#elif defined(__XC32__) || defined(__PIC32__)
    // PIC32MM products should use the following macros/functions to control
    // interrupt vectoring between Bootloader and Application projects instead
    // of EZBL_ForwardBootloaderISR:
    //      EZBL_ForwardInt(), EZBL_ForwardIntToBoot(), EZBL_ForwardIntToApp(),
    //      EZBL_ForwardAllInt(), EZBL_ForwardAllIntToBoot(), EZBL_ForwardAllIntToApp()
#endif


#if defined(__XC16__)
/**
 * Reads the EZBL_INSTALLED_FLAGS structure from the EZBL_appBootloadState Flash
 * address in application space (erasable). This structure contains checksums
 * or version information to help decide if an offered .blob should be
 * programmed or not.
 */
static inline void __attribute__((always_inline)) EZBL_ReadAppInstalledStruct(EZBL_INSTALLED_FLAGS *dest)
{
    // Fetch the address of the EZBL_appBootloadState structure
    EZBL_ReadPackedFlash(dest, EZBL_SYM32(EZBL_appBootloadState), 0, sizeof(EZBL_INSTALLED_FLAGS));
}
#elif defined(__XC32__) || defined(__PIC32__)
/**
 * Flash program location (address set in linker script) for keeping track of
 * erase/installed application success. This ROM memory address goes to 0xFFFF
 * (erased) when programming needed; Write to EZBL_APP_INSTALLED (0x12CD)
 * when application installed and ready. Other values besides EZBL_APP_ERASED
 * (0xFFFF) should be treated as suspicious (possible erase interruption,
 * warranting another erase).
 */
extern volatile const EZBL_INSTALLED_FLAGS EZBL_appBootloadState;


/**
 * Reads the EZBL_INSTALLED_FLAGS structure from the EZBL_appBootloadState Flash
 * address in application space (erasable). This structure contains checksums
 * or version information to help decide if an offered .blob should be
 * programmed or not.
 */
static inline void __attribute__((always_inline)) EZBL_ReadAppInstalledStruct(EZBL_INSTALLED_FLAGS *dest)
{
    *dest = EZBL_appBootloadState;
}
#endif


#define EZBL_APP_INSTALLED      0x12CDu
#define EZBL_APP_ERASED         0xFFFFu
#define EZBL_APP_ICSP_INSTALLED 0xB2CDu


/**
 * Flag for tracking if the current bootloader function was called from within
 * the bootloaded application, or from within the bootloader itself. Value is 0
 * when the Bootloader starts up and is changed to -1 just before
 * dispatching execution to the Application.
 */
#if defined(__XC16__)
/**
 * Flag for tracking if the current bootloader function was called from within
 * the bootloaded application, or from within the bootloader itself. Value is 0
 * when the Bootloader starts up and is changed to -1 just before
 * dispatching execution to the Application.
 */
extern short EZBL_appIsRunning;

#elif defined(__XC32__) || defined(__PIC32__)

/**
 * Macro checks if the current bootloader function was called from within the
 * Bootloader project prior to launching an Application, or if the API is being
 * called from a running Application.
 *
 * @return 0 if the Bootloader is running (App doesn't exist or hasn't been launched yet)
 *         1 if the Application has been launched
 */
#define EZBL_appIsRunning   (EZBL_MFC0(EZBL_CP0_EBase) != (unsigned int)&EZBL_BOOT_ebase_address)
#endif


// FIFO pointers for doing Bootloader communications. These can connect
// to a UART_TxFifo/UART_RxFifo (uart_fifo.c), I2C_TX_FIFO/I2C_RX_FIFO
// (i2c_fifo.c), or any other EZBL_FIFO structure you have implemented.
extern EZBL_FIFO XC16_NEAR *EZBL_COM_TX;
extern EZBL_FIFO XC16_NEAR *EZBL_COM_RX;


// Extern reference to ezbl_lib.a's default baud rate for UART interfaces when
// used for bootloading, but not explicitly specified in the top level project.
// The ezbl_lib.a default is -230400 baud (auto-baud enabled with 230400
// bits/sec set at UART_Reset() initialization for TX before receiving the
// auto-baud character)
extern const long EZBL_COMBaud;


// Extern reference to ezbl_lib.a's default I2C Slave address for I2C interfaces
// when used for bootloading, but not explicitly specified in the top level
// project. The ezbl_lib.a default is 0x60 (usually passed to I2C_Reset() in an
// initialization routine).
extern const unsigned int EZBL_i2cSlaveAddr;


// Extern reference to hardware initializer defined default interface for
// bootloader timeout/App launch and auto-baud checking. This points to the
// Rx_Fifo returned by a UART_Reset() or I2C_Reset() call (ex: &UART2_RxFifo).
extern EZBL_FIFO *EZBL_COMBootIF __attribute__((persistent));


// NOW_TASK structure for handling self-invoked calls to the
// EZBL_BootloaderTask() function via the NOW module's Timer ISR (but
// called at IPL 0 "main" level context).
//
// This structure is normally initialized by calling NOW_CreateRepeatingTask()
// and shouldn't be directly accessed. However, various NOW API's like
// NOW_EndTask() need a pointer to this structure for subsequent manipulation.
extern NOW_TASK EZBL_bootTask;

// Default named function callback that may be called by the EZBL_bootTask
// RAM structure and NOW_CreateRepeatingTask().
int EZBL_BootloaderTask(void);


#if defined(__XC16__)
/**
 * Checks if a valid Application has been flagged as existent in flash.
 *
 * This function reads the EZBL_appBootloadState flash address and returns the
 * (EZBL_INSTALLED_FLAGS.appInstalled == EZBL_APP_INSTALLED) evaluation. It does
 * not do comprehensive/pessimistic CRC/hash testing of the Application space.
 * I.e. use this function to determine if the last bootloading attempt completed
 * successfully with a non-corrupt Application image that was read-back
 * verified, not to test if the Application has self-destructed or been tampered
 * with.
 *
 * When EZBL_appBootloadState does not exist in the project (such as when
 * building a Dual Parition project), this function instead checks the FBTSEQ
 * validity on the inactive partition. An App is said to be present if
 * FBTSEQ<11:0> == ~FBTSEQ<23:12>.
 *
 * @return 1 if an Application is fully installed and ready to be executed.
 *         0 if an Application is absent or corrupt.
 */
int EZBL_IsAppPresent(void);
#elif defined(__XC32__) || defined(__PIC32__)
extern const unsigned char EZBL_BOOT_ebase_address; // Linker Symbol for CP0 EBase value when the Bootloader is executing. Do not reference as a value. Instead use ((const unsigned int)&EZBL_BOOT_ebase_address) to obtain this flash address.
extern const unsigned char EZBL_APP_ebase_address;  // Linker Symbol for CP0 EBase value when the Application is executing. Do not reference as a value. Instead use ((const unsigned int)&EZBL_App_ebase_address) to obtain this flash address.

/**
 * Checks if a valid Application has been flagged as existent in flash.
 *
 * This function reads the EZBL_appBootloadState flash address and returns the
 * (EZBL_INSTALLED_FLAGS.appInstalled == EZBL_APP_INSTALLED) evaluation. It does
 * not do comprehensive/pessimistic CRC/hash testing of the Application space.
 * I.e. use this function to determine if the last bootloading attempt completed
 * successfully with a non-corrupt Application image that was read-back
 * verified, not to test if the Application has self-destructed or been tampered
 * with.
 *
 * @return 1 if an Application is fully installed and ready to be executed.
 *         0 if an Application is absent or corrupt.
 */
static inline int __attribute__((always_inline, optimize(1))) EZBL_IsAppPresent(void)
{
    unsigned long flag = EZBL_appBootloadState.appInstalled;
    return (flag == EZBL_APP_INSTALLED) ||
           ((flag == EZBL_APP_ICSP_INSTALLED) && (*((volatile unsigned long *)(EZBL_SYM32(EZBL_AppEntry-1))) != 0xFFFFFFFFu));
}
#endif


/**
 * Launches the presently installed Application.
 *
 * If called from the Application itself, code flow is equivalent to executing a
 * "goto 0x000000" instruction in a standalone application without any
 * Bootloader existence. Application RAM variables will get reset (including the
 * stack pointer itself) by the CRT, but physical hardware SFRs won't change
 * state.
 *
 * This function will disable IPL6 and below interrupts for the first 256
 * instruction cycles after calling the Application so as to avoid the
 * possibility of an Interrupt occurring while the Application's CRT is
 * reinitializing the w15 stack pointer. If you have any IPL7 maximum priority
 * ISRs declared in your Bootloader, you should disable these manually prior
 * calling this function.
 *
 * If the EZBL_appIsRunning variable is declared in your project, it is written
 * with 0xFFFF just prior to calling the Application.
 *
 * @return This function returns without doing anything if no Application exists
 *         (i.e. EZBL_appBootloadState != EZBL_APP_INSTALLED). If the
 *         Application does exist and is launched, this function does not
 *         return.
 */
static inline void EZBL_StartAppIfPresent(void)
{
    // Read the EZBL_appBootloadState Flash address to see if an App valid flag exists
    if(!EZBL_IsAppPresent())
        return;

#if defined(__XC16__)
    __asm__(
        "\n     disi  #258"                     // Disable IPL6 and below interrupts for the first 256 instructions of the Application executing. The Application's CRT will manipulate a few core CPU registers, so we don't really want to execute one of our ISRs with these likely fine, but possibly different values.
        "\n     mov   #_EZBL_appIsRunning, w0"  // Flag that the App is now running in case if App calls back into a Bootloader function and
        "\n     setm  [w0]"                     // the Bootloader's function needs to handle things differently.
        "\n     call  _EZBL_APP_RESET_BASE"     // Start running application - NOTE: CALL does NOT waste any stack space because the App will just reinitialize the stack pointer. GOTO is not used since it won't clear the SR<SFA> "Stack Frame Active" bit on devices that implement SFA.
        "\n     reset"                          // Reset if the App returns (which they shouldn't/generally can't after stack pointer reinitialization, but could if specially written to do so)
        : : : "w0");
#elif defined(__XC32__) || defined(__PIC32__)
        void __attribute__((far, noreturn)) EZBL_AppEntry(void);    // Prototype for EZBL_AppEntry(), which isn't a real function, but instead is a linker symbol to the App's reset handler.

        __builtin_disable_interrupts();
        EZBL_MTC0(EZBL_CP0_EBase, (const unsigned int)&EZBL_APP_ebase_address); // Change Ebase for the App to receive NMI, General Exceptions, and of course Interrupts. The contents of this register are also used to decide if the Bootloader or Application is currently executing.
        EZBL_AppEntry();                                                        // Branch to the App. The EZBL_AppEntry symbol is declared in the .ld linker script file.
#endif
}


/**
 * Global function pointer typically implemented in a Bootloader project to 
 * notify a running Application (when running) that it is about to be 
 * terminated. This optionally also allows a running Application to pass a 
 * rejection status back to the Bootloader's EZBL_PreInstall() function to
 * request that the Bootloader abort the firmware update.
 *
 * This pointer should be set to 0 (null) at Bootloader start and the 
 * Application, upon initialization, should set this global pointer to the
 * address (or handle) of a function to handle pre-terminate clean up
 * operations. The Application will receive the notification only if the
 * Bootloader implements an EZBL_PreInstall() function which calls this pointer
 * (and should only do so if it sees that the global pointer has been written to
 * a non-null value).
 *
 * For more information, see EZBL_PreInstall().
 */
extern int (*EZBL_AppPreInstall)(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer);


/**
 * Optional Bootloader implemented callback function invoked when a .bl2 App
 * image is ready to be installed pending final acceptance by the Bootloader
 * project-level code (and/or existing Application code with Bootloader
 * implemented forwarding).
 *
 * Implement this function to gain a last minute chance to reject an offered
 * firmware update or cleanly shutdown any existing operations. If this function
 * is not implemented, it is assumed that any App erase/firmware update will be
 * acceptable at any time.
 * 
 * Rejecting the update maintains all flash contents unchanged (i.e. existing
 * App, if any, is not erased or halted).
 * 
 * Accepting the update signals back to the remote data provider that we are
 * going to consume the offering after completing erase operations. Sequential
 * page erase is then executed to remove any existing Application, if needed.
 *
 * @param *rxFromRemote Pointer to the RX EZBL_FIFO applicable to this offering.
 *
 *                      If the offering medium does not have an RX EZBL_FIFO
 *                      associated with it (Ex: FILEIO/USB thumbdrive or local
 *                      memory type Bootloaders), then this pointer is null.
 *
 * @param *txToRemote Pointer to the TX EZBL_FIFO applicable to this offering.
 *
 *                    If the offering medium does not have a TX EZBL_FIFO
 *                    associated with it (Ex: FILEIO/USB thumbdrive or local
 *                    memory type Bootloaders), then this pointer is null.
 *
 * @param fileSize Total file size of the .bl2 file being offered, including
 *                 the SYNC bytes and .bl2 header.
 *
 * @param *appIDVer Pointer to the EZBL_APPID_VER data contained in the offered
 *                  .bl2 file header.
 *
 *                  By the time this callback is called, the BOOTID_HASH has
 *                  already been confirmed as a match for this Bootloader.
 *                  Additionally, checking of EZBL_NO_APP_DOWNGRADE conformance
 *                  has passed, when enabled and an existing Application is
 *                  already installed.
 *
 * @return Return 0 to reject the offered firmware image or 1 to accept the
 *         image and proceed with erase/programming steps.
 *
 *         Because an existing Application may want to decide if it is in a
 *         suitable state for bootloading, or may wish to cleanly shut down
 *         before being erased, it is recommended that this callback be passed
 *         to the existing Application for additional notification/validation
 *         before returning to accept/reject the update. To accomplish this:
 *     <ol>
 *         <li> Declare a global function pointer of the same callback type. For
 *              example, place this declaration in a source file at file-level
 *              scope:
 *              <code><br>
 *                  int (*EZBL_AppPreInstall)(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer);
 *              </code><br></li>
 *         <li> In the Bootloader's initialization sequence, set
 *              <code><br>
 *                  EZBL_AppPreInstall = 0; // null pointer
 *              </code><br></li>
 *         <li> In the Application's initialization sequence, set
 *              <code><br>
 *                  EZBL_AppPreInstall = EZBL_BootloadRequest;
 *              </code><br>
 *              EZBL_BootloadRequest should be a name of a function declared and
 *              defined in the Application using the same function prototype of:
 *              <code><br>
 *                  int EZBL_BootloadRequest(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer);
 *              </code><br></li>
 *        <li>  In the Bootloader's EZBL_PreInstall() callback, check criteria 
 *              for acceptance, if applicable, and on rejection, return 0. If
 *              the Bootloader deems the offering acceptable (normal case), then
 *              test if EZBL_AppPreInstall pointer is still null, and if not
 *              call the Application's equivalent callback. Ex:
 *              <code><br>
 *                  int EZBL_PreInstall(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer)
 *                  {
 *                      if(EZBL_AppPreInstall)
 *                          return (*EZBL_AppPreInstall)(rxFromRemote, txToRemote, fileSize, appIDVer);
 *
 *                      return 1; // Bootloader always accepts update offering if no App installed or running right now
 *                  }
 *              </code><br></li>
 *         </ol>
 *
 *         NOTE: When this callback is executed, the remote node (if applicable)
 *         is left waiting and will timeout if a response takes too long to
 *         arrive. To ensure a communications timeout does not occur, limit code
 *         in this callback (plus time in the Application's callback, if
 *         applicable) to less than the remote node's communications timeout
 *         (typically around 1 second, but configurable).
 */
int EZBL_PreInstall(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer);


/**
 * Reads a .bl2 file from the communications medium, checks that the offering
 * has a matching BOOTID_HASH, optionally checks that the APPID_VER field is
 * acceptable, erases all App-space flash pages, programs and read-back verifies
 * the .bl2 data records in App-space, verifies overall file and communications
 * integrity, and programs a flag indicating the new Application is valid and
 * available for execution.
 *
 * If the target device supports Dual Partition flash hardware and is configured
 * for a Dual Partition mode, the EZBL_Install2IP() function may be appropriate
 * instead of this one. EZBL_Install2IP() explicitly targets only the Inactive
 * Partition and excludes some code applicable to Single Partition bootloading.
 *
 * This function is non-blocking and will return, even if an active bootload
 * operation is underway. To ensure bootload operations are completely handled,
 * this function should be called periodically anytime bootloader functionality
 * is desired.
 *
 * If this function is invoked while an existing Application is executing and
 * the observed .bl2 header matches the installation requirements, then all
 * non-bootloader ISRs are disabled, the existing Application is erased, and
 * this function changes to a blocking function that never returns. Instead,
 * at bootload termination (success or failure), a device reset is done to
 * reenter the bootloader with a clean stack and device state to allow new
 * Application launch (or bootload retry if failure occurred).
 *
 * Because reception, decryption erase and programming of a whole .bl2 file
 * could take an extended period, this function may internally execute the
 * CLRWDT opcode before NVM erase/programming operations. As communications
 * operations generally do not have deterministic timing, you must NOT enable
 * the watchdog timer in a Windowed mode.
 *
 * @param *s Pointer to caller allocated EZBL_INSTALL_CTX structure that must
 *           remain within scope anytime a bootload session occurs. Before
 *           calling EZBL_Install2Flash() for the first time, set the following
 *           two fields:
 *              s.state = SM_SYNC_INIT;
 *              s.timeout = (application defined NOW timeout value);
 *           For the timeout, specify the timeout in some combination of
 *           NOW_sec and/or NOW_ms, ex: s.timeout = NOW_sec*1u;
 *           for a one second communications idle timeout abort time. Higher
 *           latency communications mediums, such as wireless links, will likely
 *           require more than 1 second.
 *
 *           On subsequent calls to EZBL_Install2Flash(), pass the *s parameter
 *           back to the function with unchanged structure elements.
 *
 * @param BOOTID_HASH0, BOOTID_HASH1, BOOTID_HASH2, BOOTID_HASH3 project
 *        symbols. These are generated as a SHA-256 hash (truncated to 16-bytes)
 *        of the concatenated BOOTID_VENDOR, BOOTID_MODEL, BOOTID_NAME and
 *        BOOTID_OTHER strings defined in your ezbl Makefile
 *        (i.e.: 'ezbl_product_id.mk' with proper include of
 *        'ezbl_product_id.mk' in 'Makefile').
 *
 * @param APPID_VER_MAJOR, APPID_VER_MINOR, APPID_VER_BUILD project symbols
 *        defined in the last installed Application's ezbl Makefile (i.e.:
 *        'ezbl_product_id.mk' with proper include of 'ezbl_product_id.mk' in
 *        'Makefile' within the Application project). These get stored in the
 *        EZBL_appBootloadState flash address as part of the Application, so
 *        will not exist if the Application has been erased.
 *
 * @param EZBL_NO_APP_DOWNGRADE optional project symbol. Set to 1 using
 *        EZBL_SetSYM() if you wish to trigger pre-erase/programming abort when
 *        the offered .bl2 image file contains
 *        APPID_VER_MAJOR.APPID_VER_MINOR.APPID_VER_BUILD version meta data that
 *        indicates the firmware is less than the presently executing
 *        application version (as defined by EZBL_appIDVer.appIDVer64).
 *
 * @param EZBL_FLOW_THRESHOLD optional project symbol. Provides loose control
 *        over flow control advertisements, as a byte count. Anytime the
 *        EZBL_COM_RX fifo contains >= EZBL_FLOW_THRESHOLD bytes worth of
 *        freespace available, a flow control message is generated, so smaller
 *        values for EZBL_FLOW_THRESHOLD cause more flow control advertisements
 *        to be generated while larger values can reduce overhead (at the
 *        possible expense of communications "burstiness").
 *
 *        If unspecified, the default value for EZBL_FLOW_THRESHOLD is 32 bytes.
 *
 *        To set this value, use the EZBL_SetSYM() macro in a project level
 *        source file.
 *
 * @param *EZBL_COM_TX Global pointer to a communications transmit FIFO for flow
 *        control and status messages. As an example, to use the default UART
 *        implementation, set:
 *          EZBL_COM_TX = &UART_TxFifo;
 *        To use an I2C slave medium, set:
 *          EZBL_COM_TX = &I2C_TxFifo;
 *
 *        By default, this parameter can be ignored as the associated
 *        communications RX ISR(s) contains a BOOTLOADER_WAKE_KEY definition,
 *        which causes the ISR to automatically set both EZBL_COM_TX and
 *        EZBL_COM_RX at the correct time based on an incoming data
 *        synchronization match of at least 8 bytes.
 *
 * @param *EZBL_COM_RX Global pointer to a communications receive FIFO to read
 *        the .bl2 firmware image from. As an example, to use the default UART
 *        implementation, set:
 *          EZBL_COM_RX = &UART_RxFifo;
 *        To use an I2C slave medium, set:
 *          EZBL_COM_RX = &I2C_RxFifo;
 *
 *        By default, this parameter can be ignored as the associated
 *        communications RX ISR(s) contains a BOOTLOADER_WAKE_KEY definition,
 *        which causes the ISR to automatically set both EZBL_COM_TX and
 *        EZBL_COM_RX at the correct time based on an incoming data
 *        synchronization match of at least 8 bytes. After this initial 8 byte
 *        key match, this function internally requires a BOOT_ID match, ensuring
 *        at least 16 more bytes are valid EZBL communications data.
 *
 *        Upon termination of the bootload operation (due to idle timeout,
 *        bootload failure, offered version mismatch, successful reprogramming,
 *        or any other cause), EZBL_COM_RX is set to 0 (null) to place this
 *        EZBL_Install2Flash() function into a dormant/passive state (requires
 *        new ISR BOOTLOADER_WAKE_KEY match to wake up).
 *
 * @return <ul>
 *              <li>0 - normal operation. When in the passive state, this is the
 *                      immediate function response.</li>
 *              <li>1 - bootload-end event detected. A bootload-end event does
 *                      not necessarily indicate new firmware is available or
 *                      was attempted to be uploaded as COM RX idle timeouts are
 *                      also reported.</li>
 *         <ul>
 *
 *         When the return value is 1, read the s->bootCode parameter to
 *         determine the bootload-end cause.
 *
 *         For s->bootCode:
 *         Generally zero or greater is good, while negative values indicate a
 *         failure. Anything erased/programmed prior to a failure is left as it
 *         was upon failure detection (will automatically be cleaned up on next
 *         bootload attempt).
 *
 *         Specific return code meanings are:
 *         <ul>
 *          <li>EZBL_ERROR_SUCCESS (1): Operation completed successfully. The
 *              new pplication is ready for execution.</li>
 *          <li>EZBL_ERROR_COM_READ_TIMEOUT (-20): Bootloader communications
 *              timeout attempting to read firmware image.</li>
 *          <li>EZBL_ERROR_IMAGE_MALFORMED (-21): Bootloader rejected firmware
 *              as malformed or of unsupported type. Possible communications
 *              error.</li>
 *          <li>EZBL_ERROR_BOOTID_HASH_MISMATCH (-22): Bootloader rejected
 *              firmware as incompatible (with this Bootloader project).</li>
 *          </li>EZBL_ERROR_APPID_VER_MISMATCH (-23): Bootloader rejected
 *              firmware as out of the required programming order.
 *
 *              This error is only generated when the EZBL_NO_APP_DOWNGRADE
 *              symbol is non-zero and the received
 *              APPID_VER_MAJOR.APPID_VER_MINOR.APPID_VER_BUILD information in
 *              the file is less than the presently executing Application ID
 *              version. If no Application exists, this error will not be
 *              generated.</li>
 *          <li>EZBL_ERROR_HARD_VERIFY_ERROR (-24): Bootloader read-back
 *              verification failure.</li>
 *          <li>EZBL_ERROR_SOFT_VERIFY_ERROR (-25): Bootloader read-back
 *              verification mismatch in reserved address range.</li>
 *          <li>EZBL_ERROR_IMAGE_CRC (-26): Bootloader computed CRC mismatch
 *              with CRC contained in firmware image. Probable communications
 *              error.</li>
 *          <li>No return: Unhandled exception or unknown. (Image file may have
 *              records targeting unimplemented memory, causing an Address Error
 *              Trap upon verification, or electrical operating parameters are
 *              incorrect.) Ensure the image file was compiled specifically
 *              for this device and bootloader. For unhandled exceptions, the
 *              device will automatically reset back into the bootloader.</li>
 *         </ul>
 */
int EZBL_Install2Flash(EZBL_INSTALL_CTX *s);


#if defined(__XC16__)
/**
 * Reads a .bl2 file from the communications medium, checks that the offering
 * has a matching BOOTID_HASH, optionally checks that the APPID_VER field is
 * acceptable, erases the Inactive Partition, programs and read-back verifies
 * the .bl2 data records on the Inactive Partition, verifies overall file and
 * communications integrity, and optionally programs FBTSEQ on the Inactive
 * Partition indicating the new Application is valid and available for
 * execution.
 *
 * This function is non-blocking and will return, even if an active bootload
 * operation is underway. To ensure bootload operations are completely handled,
 * this function should be called periodically anytime bootloader functionality
 * is desired.
 *
 * @param *s Pointer to caller allocated EZBL_INSTALL_CTX structure that must
 *           remain within scope anytime a bootload session occurs. Before
 *           calling EZBL_Install2IP() for the first time, set the following
 *           fields:
 *              s.state = SM_SYNC_INIT;
 *              s.lastActivity = NOW_32();
 *              s.timeout = (application defined NOW timeout value);
 *           For the timeout, specify the timeout in some combination of
 *           NOW_sec and/or NOW_ms, ex: s.timeout = NOW_sec*1u;
 *           for a one second communications idle timeout abort time. Higher
 *           latency communications mediums, such as wireless links, will likely
 *           require more than 1 second.
 *
 *           On subsequent calls to EZBL_Install2IP(), pass the *s parameter
 *           back to the function with unchanged structure elements.
 *
 * @param EZBL_ADDRESSES_PER_SECTOR project symbol. This symbol must be defined
 *        to match the hardware's native Flash Page Erase size, expressed in
 *        flash addresses. Every 0x2 flash addresses is one 24-bit instruction
 *        word, or 3 bytes of .bl2 record data. 0x4 flash addresses represents
 *        one 48-bit flash double word, or 6 bytes of .bl2 record data.
 *
 *        Most hardware requires 0x400 or 0x800 for this definition (i.e. 1536
 *        bytes per page or 3072 bytes per page), with the needed value chosen
 *        from the device data sheet. If this symbol is not set, the default
 *        value of 0x400 addresses is used, which may or may not be correct for
 *        your hardware target.
 *
 *        To assign this parameter, within a project source file, write:
 *              EZBL_SetSYM(EZBL_ADDRESSES_PER_SECTOR, 0x400);
 *        or,
 *              EZBL_SetSYM(EZBL_ADDRESSES_PER_SECTOR, 0x800);
 *        This should be placed at global/file level scope with the ezbl.h header
 *        already #include'd.
 *
 * @param BOOTID_HASH0, BOOTID_HASH1, BOOTID_HASH2, BOOTID_HASH3 project
 *        symbols. These are generated as a SHA-256 hash (truncated to 16-bytes)
 *        of the concatenated BOOTID_VENDOR, BOOTID_MODEL, BOOTID_NAME and
 *        BOOTID_OTHER strings defined in your ezbl Makefile
 *        (i.e.: 'ezbl_product_id.mk' with proper include of
 *        'ezbl_product_id.mk' in 'Makefile').
 *
 * @param APPID_VER_MAJOR, APPID_VER_MINOR, APPID_VER_BUILD project symbols
 *        defined in the existing ezbl Makefile (i.e.: 'ezbl_product_id.mk'
 *        with proper include of 'ezbl_product_id.mk' in 'Makefile').
 *
 * @param EZBL_AUTO_PROG_FBTSEQ optional project symbol to enable automatic
 *        reprogramming of the Inactive Partition's FBTSEQ to be one less than
 *        the presently Active Partition's FBTSEQ, enabling the new firmware to
 *        go live on next reset or BOOTSWP.
 *
 *        To use this feature, within a project source file, write:
 *              EZBL_KeepSYM(EZBL_AUTO_PROG_FBTSEQ);
 *        This should be placed at global/file level scope.
 *
 *        FBTSEQ is programmed only if the entire programming sequence completes
 *        successfully, is verified to not be corrupted in transit, and reads
 *        back from flash as expected. In other cases, FBTSEQ is left either
 *        unprogrammed or programmed with the value contained in the original
 *        .hex file (.bl2 file).
 *
 *        When using this feature, do not create .bl2 files with the FBTSEQ
 *        Config word defined in it. If defined, it will be programmed as normal
 *        data and then require the EZBL_AUTO_PROG_FBTSEQ function to erase and
 *        reprogram the entire last page on the inactive partition - needlessly
 *        wasting flash endurance.
 *
 * @param EZBL_NO_APP_DOWNGRADE optional project symbol. Set to 1 using
 *        EZBL_SetSYM() if you wish to trigger pre-erase/programming abort when
 *        the offered .bl2 image file contains
 *        APPID_VER_MAJOR.APPID_VER_MINOR.APPID_VER_BUILD version meta data that
 *        indicates the firmware is less than the presently executing
 *        application version (as defined by EZBL_appIDVer.appIDVer64).
 *
 * @param EZBL_FLOW_THRESHOLD optional project symbol. Provides loose control
 *        over flow control advertisements, as a byte count. Anytime the
 *        EZBL_COM_RX fifo contains >= EZBL_FLOW_THRESHOLD bytes worth of
 *        freespace available, a flow control message is generated, so smaller
 *        values for EZBL_FLOW_THRESHOLD cause more flow control advertisements
 *        to be generated while larger values can reduce overhead (at the
 *        possible expense of communications "burstiness").
 *
 *        If unspecified, the default value for EZBL_FLOW_THRESHOLD is 32 bytes.
 *
 *        To set this value, use the EZBL_SetSYM() macro in a project level
 *        source file.
 *
 * @param *EZBL_COM_TX Global pointer to a communications transmit FIFO for flow
 *        control and status messages. As an example, to use the default UART
 *        implementation, set:
 *          EZBL_COM_TX = &UART_TxFifo;
 *        To use an I2C slave medium, set:
 *          EZBL_COM_TX = &I2C_TxFifo;
 *
 *        By default, this parameter can be ignored as the associated
 *        communications RX ISR(s) contains a BOOTLOADER_WAKE_KEY definition,
 *        which causes the ISR to automatically set both EZBL_COM_TX and
 *        EZBL_COM_RX at the correct time based on an incoming data
 *        synchronization match of at least 8 bytes.
 *
 * @param *EZBL_COM_RX Global pointer to a communications receive FIFO to read
 *        the .bl2 firmware image from. As an example, to use the default UART
 *        implementation, set:
 *          EZBL_COM_RX = &UART_RxFifo;
 *        To use an I2C slave medium, set:
 *          EZBL_COM_RX = &I2C_RxFifo;
 *
 *        By default, this parameter can be ignored as the associated
 *        communications RX ISR(s) contains a BOOTLOADER_WAKE_KEY definition,
 *        which causes the ISR to automatically set both EZBL_COM_TX and
 *        EZBL_COM_RX at the correct time based on an incoming data
 *        synchronization match of at least 8 bytes.
 *
 *        Upon termination of the bootload operation (due to idle timeout,
 *        bootload failure, offered version mismatch, successful reprogramming,
 *        or any other cause), EZBL_COM_RX is set to 0 (null) to place this
 *        EZBL_Install2IP() function into a dormant/passive state (requires
 *        new ISR BOOTLOADER_WAKE_KEY match to wake up).
 *
 * @return <ul>
 *              <li>0 - normal operation. When in the passive state, this is the
 *                      immediate function response.</li>
 *              <li>1 - bootload-end event detected. A bootload-end event does
 *                      not necessarily indicate new firmware is available or
 *                      was attempted to be uploaded as COM RX idle timeouts are
 *                      also reported.</li>
 *         <ul>
 *
 *         When the return value is 1, read the s->bootCode parameter to
 *         determine the bootload-end cause.
 *
 *         For s->bootCode:
 *         Generally zero or greater is good, while negative values indicate a
 *         failure. Anything erased/programmed prior to a failure is left as it
 *         was upon failure detection (will automatically be cleaned up on next
 *         bootload attempt).
 *
 *         Specific return code meanings are:
 *         <ul>
 *          <li>EZBL_ERROR_SUCCESS (1): Operation completed successfully. The
 *              Inactive Partition is ready for execution.</li>
 *          <li>EZBL_ERROR_SUCCESS_VER_GAP (2): Operation completed successfully,
 *              but the programmed image did not have an
 *              APPID_VER_MAJOR.APPID_VER_MINOR field that was +1 (minor code)
 *              from the existing application. Execution of the new code should
 *              proceed by resetting rather than a Live Update bootswap.</li>
 *          <li>EZBL_ERROR_COM_READ_TIMEOUT (-20): Bootloader communications
 *              timeout attempting to read firmware image.</li>
 *          <li>EZBL_ERROR_IMAGE_MALFORMED (-21): Bootloader rejected firmware
 *              as malformed or of unsupported type. Possible communications
 *              error.</li>
 *          <li>EZBL_ERROR_BOOTID_HASH_MISMATCH (-22): Bootloader rejected
 *              firmware as incompatible (with this Bootloader project).</li>
 *          </li>EZBL_ERROR_APPID_VER_MISMATCH (-23): Bootloader rejected
 *              firmware as out of the required programming order.
 *
 *              This error is only generated when the EZBL_NO_APP_DOWNGRADE
 *              symbol is non-zero and the received
 *              APPID_VER_MAJOR.APPID_VER_MINOR.APPID_VER_BUILD information in
 *              the file is less than the presently executing Application ID
 *              version.</li>
 *          <li>EZBL_ERROR_HARD_VERIFY_ERROR (-24): Bootloader read-back
 *              verification failure.</li>
 *          <li>EZBL_ERROR_SOFT_VERIFY_ERROR (-25): Bootloader read-back
 *              verification mismatch in reserved address range.</li>
 *          <li>EZBL_ERROR_IMAGE_CRC (-26): Bootloader computed CRC mismatch
 *              with CRC contained in firmware image. Probable communications
 *              error.</li>
 *          <li>No return: Unhandled exception or unknown. (Image file may have
 *              records targeting unimplemented memory, causing an Address Error
 *              Trap upon verification, or electrical operating parameters are
 *              incorrect.) Ensure the image file was compiled specifically
 *              for this device and bootloader.</li>
 *         </ul>
 */
int EZBL_Install2IP(EZBL_INSTALL_CTX *s);
#endif


/**
 * Reads a .bl2 file from the specified FILEIO communications handle, checks
 * that the offering has a matching BOOTID_HASH, optionally checks that the
 * APPID_VER field is acceptable, reads the whole file contents to verify a
 * CRC32 and ensure proper structure, seeks back to the beginning of the .bl2
 * file, erases all App-space flash pages, reprocesses the .bl2 file programming
 * and verifying the flash writes, reverifies the overall file CRC32 and FILEIO
 * communications integrity, and programs a flag indicating the new Application
 * is valid and available for execution.
 *
 * This function is blocking and will return only upon firmware rejection,
 * bootload completion, or if an error occurs.
 *
 * If this function is invoked while an existing Application is executing and
 * the observed .bl2 image matches the installation requirements, then all
 * non-bootloader ISRs are disabled, the existing Application is erased, and
 * this function will not return. Instead, at bootload termination (success or
 * failure), a device reset is done to reenter the bootloader with a clean stack
 * and device state to allow new Application launch (or bootload retry if
 * failure occurred).
 *
 * Because reception, decryption erase and programming of a whole .bl2 file
 * could take an extended period, this function internally executes the
 * CLRWDT opcode before NVM erase/programming operations. As the operations
 * generally do not have deterministic timing, you must NOT enable the watchdog
 * timer in a Windowed mode.
 *
 * @param *bl2FILEIOHandle FILEIO_OBJECT pointer opened externally via the
 *        FILEIO_Open() API. The file position pointer should point to the
 *        Start Of File (byte 0) of the .bl2 file that you wish to install.
 *
 * @param commit Set to 1 to enable flash erase/write and verification
 *        operations to take place for a normal bootload procedure.
 *
 *        0 and all other commit values will internally suppress all flash
 *        erase/write operations and blank expected read-back verification
 *        mismatches to allow complete file processing in a "simulation" mode.
 *        Use this option to pre-check the complete image availability,
 *        integrity, and data record target address sensibility to gain a "warm
 *        fuzzy feeling" before repeating the EZBL_InstallFILEIO2Flash() call
 *        with non-volatile consequences.
 *
 * @param BOOTID_HASH0, BOOTID_HASH1, BOOTID_HASH2, BOOTID_HASH3 project
 *        symbols. These are generated as a SHA-256 hash (truncated to 16-bytes)
 *        of the concatenated BOOTID_VENDOR, BOOTID_MODEL, BOOTID_NAME and
 *        BOOTID_OTHER strings defined in your ezbl Makefile
 *        (i.e.: 'ezbl_product_id.mk' with proper include of
 *        'ezbl_product_id.mk' in 'Makefile').
 *
 * @param APPID_VER_MAJOR, APPID_VER_MINOR, APPID_VER_BUILD project symbols
 *        defined in the last installed Application's ezbl Makefile (i.e.:
 *        'ezbl_product_id.mk' with proper include of 'ezbl_product_id.mk' in
 *        'Makefile' within the Application project). These get stored in the
 *        EZBL_appBootloadState flash address as part of the Application, so
 *        will not exist if the Application has been erased.
 *
 * @param EZBL_NO_APP_DOWNGRADE optional project symbol. Set to 1 using
 *        EZBL_SetSYM() if you wish to trigger pre-erase/programming abort when
 *        the offered .bl2 image file contains
 *        APPID_VER_MAJOR.APPID_VER_MINOR.APPID_VER_BUILD version meta data that
 *        indicates the firmware is less than the presently executing
 *        application version (as defined by EZBL_appIDVer.appIDVer64).
 *
 * @param *EZBL_COM_TX Global pointer to a communications transmit FIFO for flow
 *        control and status messages. As an example, to use the default UART
 *        implementation, set:
 *          EZBL_COM_TX = &UART_TxFifo;
 *        To use an I2C slave medium, set:
 *          EZBL_COM_TX = &I2C_TxFifo;
 *
 *        By default, this parameter can be ignored as the associated
 *        communications RX ISR(s) contains a BOOTLOADER_WAKE_KEY definition,
 *        which causes the ISR to automatically set both EZBL_COM_TX and
 *        EZBL_COM_RX at the correct time based on an incoming data
 *        synchronization match of at least 8 bytes.
 *
 * @param *EZBL_COM_RX Global pointer to a communications receive FIFO to read
 *        the .bl2 firmware image from. As an example, to use the default UART
 *        implementation, set:
 *          EZBL_COM_RX = &UART_RxFifo;
 *        To use an I2C slave medium, set:
 *          EZBL_COM_RX = &I2C_RxFifo;
 *
 *        By default, this parameter can be ignored as the associated
 *        communications RX ISR(s) contains a BOOTLOADER_WAKE_KEY definition,
 *        which causes the ISR to automatically set both EZBL_COM_TX and
 *        EZBL_COM_RX at the correct time based on an incoming data
 *        synchronization match of at least 8 bytes.
 *
 *        Upon termination of the bootload operation (due to idle timeout,
 *        bootload failure, offered version mismatch, successful reprogramming,
 *        or any other cause), EZBL_COM_RX is set to 0 (null) to place this
 *        EZBL_Install2Flash() function into a dormant/passive state (requires
 *        new ISR BOOTLOADER_WAKE_KEY match to wake up).
 *
 * @return Signed integer result code. Generally zero or greater is good, while
 *         negative values indicate a failure. Anything erased/programmed prior
 *         to a failure is left as it was upon failure detection (will
 *         automatically be cleaned up on next bootload attempt).
 *
 *         Specific return code meanings are:
 *         <ul>
 *          <li>EZBL_ERROR_SUCCESS (1): Operation completed successfully. The
 *              new application is ready for execution.</li>
 *          <li>EZBL_ERROR_ALREADY_INSTALLED (3): Offered firmware image already
 *              matches the existing target firmware.</li>
 *          <li>EZBL_ERROR_COM_READ_TIMEOUT (-20): Bootloader communications
 *              timeout attempting to read firmware image.</li>
 *          <li>EZBL_ERROR_IMAGE_MALFORMED (-21): Bootloader rejected firmware
 *              as malformed or of unsupported type. Possible communications
 *              error.</li>
 *          <li>EZBL_ERROR_BOOTID_HASH_MISMATCH (-22): Bootloader rejected
 *              firmware as incompatible (with this Bootloader project).</li>
 *          </li>EZBL_ERROR_APPID_VER_MISMATCH (-23): Bootloader rejected
 *              firmware as out of the required programming order.
 *
 *              This error is only generated when the EZBL_NO_APP_DOWNGRADE
 *              symbol is non-zero and the received
 *              APPID_VER_MAJOR.APPID_VER_MINOR.APPID_VER_BUILD information in
 *              the file is less than the presently executing Application ID
 *              version. If no Application exists, this error will not be
 *              generated.</li>
 *          <li>EZBL_ERROR_HARD_VERIFY_ERROR (-24): Bootloader read-back
 *              verification failure.</li>
 *          <li>EZBL_ERROR_SOFT_VERIFY_ERROR (-25): Bootloader read-back
 *              verification mismatch in reserved address range.</li>
 *          <li>EZBL_ERROR_IMAGE_CRC (-26): Bootloader computed CRC mismatch
 *              with CRC contained in firmware image. Probable communications
 *              error.</li>
 *          <li>No return: Unhandled exception or unknown. (Image file may have
 *              records targeting unimplemented memory, causing an Address Error
 *              Trap upon verification, or electrical operating parameters are
 *              incorrect.) Ensure the image file was compiled specifically
 *              for this device and bootloader. For unhandled exceptions, the
 *              device will automatically reset back into the bootloader.</li>
 *         </ul>
 */
int EZBL_InstallFILEIO2Flash(void *bl2FILEIOHandle, int commit);


// Common error code definitions used for communications and .bl2 file parsing.
// These are typically returned from the EZBL_Install2Flash(),
// EZBL_InstallFILEIO2Flash(), or EZBL_Install2IP() bootloader function and
// passed back to the master host for display to a user or for logging.
#define EZBL_ERROR_RESERVED                    (0)  // 0x0000 Reserved return code never generated by code. This code can be generated by a floating serial I/O pin or non-EZBL communications protocol.
#define EZBL_ERROR_SUCCESS                     (1)  // 0x0001 Operation completed successfully
#define EZBL_ERROR_SUCCESS_VER_GAP             (2)  // 0x0002 Operation completed successfully, but the programmed image did not have an APPID_VER_MAJOR.APPID_VER_MINOR field that was +1 (minor code) from the existing application.
#define EZBL_ERROR_ALREADY_INSTALLED           (3)  // 0x0003 Offered firmware image already matches the existing target firmware
#define EZBL_ERROR_RESERVED_XON_POS           (17)  // 0x0011 Reserved return code never generated by code. This code contains an XON character if XON/XOFF flow control is used, so this should not unintentionally be allocated as a status code.
#define EZBL_ERROR_RESERVED_XOFF_POS          (19)  // 0x0013 Reserved return code never generated by code. This code contains an XOFF character if XON/XOFF flow control is used, so this should not unintentionally be allocated as a status code.
#define EZBL_ERROR_COM_READ_TIMEOUT          (-20)  // 0xFFEC Bootloader signaled communications timeout while waiting for image data
#define EZBL_ERROR_IMAGE_MALFORMED           (-21)  // 0xFFEB Bootloader rejected firmware as malformed or of unsupported type. Possible communications error.
#define EZBL_ERROR_BOOTID_HASH_MISMATCH      (-22)  // 0xFFEA Bootloader rejected firmware as incompatible
#define EZBL_ERROR_APPID_VER_MISMATCH        (-23)  // 0xFFE9 Bootloader rejected firmware as out of the required programming order
#define EZBL_ERROR_HARD_VERIFY_ERROR         (-24)  // 0xFFE8 Bootloader read-back verification failure
#define EZBL_ERROR_SOFT_VERIFY_ERROR         (-25)  // 0xFFE7 Bootloader read-back verification mismatch in reserved address range
#define EZBL_ERROR_IMAGE_CRC                 (-26)  // 0xFFE6 Bootloader computed CRC mismatch with CRC contained in firmware image. Probable communications error.
#define EZBL_ERROR_IMAGE_REJECTED            (-27)  // 0xFFE5 Bootloader or running application rejected the offered image
#define EZBL_ERROR_CUSTOM_MESSAGE            (-28)  // 0xFFE4 Custom bootloader response message
#define EZBL_ERROR_NOT_ENOUGH_MEMORY        (-100)  // 0xFF9C Not enough bootloader stack memory available
#define EZBL_ERROR_INVALID_PARAMETER        (-101)  // 0xFF9B Invalid input parameter(s)
#define EZBL_ERROR_RESERVED_XOFF_NEG        (-237)  // 0xFF13 Bootloader page erase sequence busy/keep alive code (not generated as final status code). NOTE: 0x13 is the XOFF character, so if the remote hardware supports XOFF/XON software flow control, the remote node should halt its transmit stream until the bootloader erase sequence is complete.
#define EZBL_ERROR_RESERVED_XON_NEG         (-239)  // 0xFF11 Bootloader page erase sequence complete (not generated as final status code). NOTE: 0x11 is the XON character, so if the remote hardware supports XOFF/XON software flow control, the remote transmit stream should resume after the bootloader is done erasing.

// The following are reserved numbers due to historical EZBL v1.xx use/meaning. They are now deprecated in favor of the above similar errors with less differentiation against errors that are likely to have the same underlying cause.
#define EZBL_ERROR_TIMEOUT_IN_BLOB_HEADER   (-1)    // Communications timeout attempting to read the first 4 bytes of the .blob file (where the file's length is contained).
#define EZBL_ERROR_TIMEOUT_IN_RECORD_HEADER (-2)    // Communications timeout attempting to read a record header from the .blob file.
#define EZBL_ERROR_ILLEGAL_LENGTH           (-3)    // Communications corruption occurred or the .blob file contains an illegally long length field in a data record or the overall .blob header.
#define EZBL_ERROR_ILLEGAL_RECORD_ADDRESS   (-4)    // Communications corruption occurred or the .blob file contains an illegally high record address.
#define EZBL_ERROR_TIMEOUT_IN_RECORD_DATA   (-5)    // Communications timeout trying to read .blob record data.
#define EZBL_ERROR_TIMEOUT_IN_CRC           (-6)    // Communications timeout reading last 4 byte CRC field.
#define EZBL_ERROR_BLOB_CRC                 (-7)    // CRC of received .blob contents mismatch with CRC contained in .blob. Probable communications corruption.
#define EZBL_ERROR_READ_BACK_VERIFICATION   (-8)    // Read-back verification mismatch. Probable configuration error or write protected memory.
#define EZBL_ERROR_BOOTLOADER_MISMATCH      (-9)    // Read-back verification mismatch. All programming completed, but data in the existing bootloader does not match the bootloader copy in the uploaded image. Make sure you transmitted a correct .hex/.blob file that exactly matches and was built for the installed bootloader. The Application must be compiled with _merge.s and _merge.gld files generated when the bootloader was originally built and deployed.



#if defined(__XC16__)
/**
 * On a Dual Partition device, writes or rewrites a Partition's FBTSEQ value.
 *
 * This function can target either the Active Partition or the Inactive
 * Partition's FBTSEQ value. If targeting the Active
 * Partition, the FBTSEQ value in Flash must be blank (erase default value of
 * 0xFFFFFF) and be permitted by hardware. When targeting the Inactive
 * Partition's FBTSEQ value, any value can preexist, but success is still
 * dependent on hardware restrictions.
 *
 * The value written can be a relative offset from the currently Active
 * Partition's FBTSEQ value, or any legal 12-bit absolute FBTSEQ value. The
 * proper one's complement value in the upper 12 bits will be automatically
 * computed and programmed, so need not be provided.
 *
 * If the target partition's FBTSEQ value already matches that which is being
 * requested for programming, this function returns immediately.
 *
 * If the target partition is the Inactive Partition and it already has FBTSEQ
 * programmed (to a non-matching value), then the full Flash Page contents will
 * be pushed onto the stack, erased in Flash, and then restored back into the
 * Flash page with the new FBTSEQ value, allowing any value to be programmed at
 * any time the hardware allows it.
 *
 * This function is only available/applicable on devices with Dual Partition
 * hardware and with the Bootloader project compiled for Dual Partition mode.
 *
 * NOTE:
 * The hardware treats illegal FBTSEQ values (one with an unmatching one's
 * complement in the upper 12-bits of the 24-bit FBTSEQ instruction word) as
 * equivalent to 0x000FFF, the lowest possible boot priority. In the event both
 * partitions have illegal FBTSEQ values and/or the value 0x000FFF in them, then
 * Partition 1 will always be chosen for Activation on device reset. Therefore,
 * the first meaningful value that should be programmed with this function, is
 * 0x001FFE.
 *
 * @param target
 *      Flag specifying which partition's FBTSEQ value shall be programmed.
 *      Specify:
 *      <ul>
 *          <li>0 - Write Inactive Partition's FBTSEQ</li>
 *          <li>1 - Write Active Partition's FBTSEQ</li>
 *          <li>-1 - Write Partition 1's FBTSEQ</li>
 *          <li>-2 - Write Partition 2's FBTSEQ</li>
 *      </ul>
 * @param absoluteSEQ
 *      Absolute 12-bit sequence number to write to FBTSEQ. The upper 12-bit
 *      complement is internally computed to always be valid. Specify
 *      relativeSEQ as 0x0000 when using this parameter.
 *
 *      If you wish to program a relative FBTSEQ value, specify 0x0000 for this
 *      parameter.
 *
 * @param relativeSEQ
 *      signed integer added to the NON-targeted partition's FBTSEQ value.
 *      <ul>
 *          <li>Specify 0 when using the absoluteSEQ parameter instead.</li>
 *          <li>Specify -1 to make the target Partition Active at reset</li>
 *          <li>Specify +1 to make the target partition Inactive at reset</li>
 *          <li>Other values are legal, but are unlikely to be useful for
 *              anything.</li>
 *      </ul>
 *
 *      An error code will be returned without programming (or erasing) anything
 *      if the offset would result in underflow below 0xFFF000, the highest
 *      possible priority, the result would overflow above 0x000FFF, the lowest
 *      possible boot priority.
 *
 * @return
 *  <ul>
 *      <li> 1 if the target partition was successfully programmed. Reset
 *             the device or call EZBL_ResetCPU() to have the changes take
 *             effect.</li>
 *      <li>-1 if target partition did not program successfully (blocked by
 *             hardware due to Config segment write protection, or using
 *             Protected Dual Partition mode and targeting Partition 1)</li>
 *      <li>-2 if the target computed relative FBTSEQ value would have
 *             overflowed or underflowed</li>
 *      <li>-3 target parameter value invalid</li>
 *  </ul>
 */
int EZBL_WriteFBTSEQ(int target, int absoluteSEQ, int relativeSEQ);
static int __attribute__((weakref("EZBL_WriteFBTSEQ"))) EZBL_WriteFBTSEQW(int target, int absoluteSEQ, int relativeSEQ);
#endif


/**
 * Computes the closest UxBRG value for the specified baud rate, assuming a 16x
 * clock sampling divider (BRGH == 0).
 *
 * The return value will be rounded to minimize total baud error magnitude, so
 * will be up to +/- 0.5 UxBRG steps faster or slower when a perfect match is
 * impossible for the given peripheral input clock.
 *
 * The UxBRG value computed is not written to any SFR.
 *
 * @param peripheralClock Input clock speed to the peripheral (typically system
 *                        clock speed in MIPS, ex: NOW_Fcy).
 *
 *                        On dsPIC33C devices, the UART_Reset() API configures
 *                        the UART peripheral to run at Fosc (2*Fcy), but
 *                        peripheralClock should still be specified as Fcy. The
 *                        UxBRG value computed will be computed against 2x this
 *                        peripheralClock parameter value.
 *
 * @param baud Baud rate to compute for, specified in bits per second. Ex:
 *             115200, 230400, 460800, 921600, etc.
 *
 *             If baud is a negative value, the return value will also be
 *             negative, but the magnitude will be correct. Take the 2's
 *             complement before assigning it to a UxBRG register.
 *
 * @return Closest UxBRG value needed to achieve the specified baud rate.
 *
 *         If baud is negative, the correct UxBRG value will be returned in 2's
 *         complemented form.
 *
 *         If baud is 0, 0 is returned without triggering a divide by zero
 *         error.
 */
int EZBL_CalcUxBRG(unsigned long peripheralClock, long baud);


/**
 * Computes the closest UxBRG value for the specified baud rate, assuming a 4x
 * clock sampling divider (BRGH == 1).
 *
 * The return value will be rounded to minimize total baud error magnitude, so
 * will be up to +/- 0.5 UxBRG steps faster or slower when a perfect match is
 * impossible for the given peripheral input clock.
 *
 * The UxBRG value computed is not written to any SFR.
 *
 * @param peripheralClock Input clock speed to the peripheral (typically system
 *                        clock speed in MIPS, ex: NOW_Fcy).
 *
 *                        On dsPIC33C devices, the UART_Reset() API configures
 *                        the UART peripheral to run at Fosc (2*Fcy), but
 *                        peripheralClock should still be specified as Fcy. The
 *                        UxBRG value computed will be computed against 2x this
 *                        peripheralClock parameter value.
 *
 * @param baud Baud rate to compute for, specified in bits per second. Ex:
 *             115200, 230400, 460800, 921600, etc.
 *
 *             If baud is a negative value, the return value will also be
 *             negative, but the magnitude will be correct. Take the 2's
 *             complement before assigning it to a UxBRG register.
 *
 * @return Closest UxBRG value needed to achieve the specified baud rate.
 *
 *         If baud is negative, the correct UxBRG value will be returned in 2's
 *         complemented form.
 *
 *         If baud is 0, 0 is returned without triggering a divide by zero
 *         error.
 */
int EZBL_CalcUxBRGH(unsigned long peripheralClock, long baud);


/****************** Default Hardware Initializer APIs *************************/

/**
 * Initializes interrupt controller, NOW software timing module, UART and other
 * I/O pins
 *
 * @return System clock frequency
 */
unsigned long InitializeBoard(void);
unsigned long EZBL_InitializeBoard(void);


/**
 *  Toggles the state of an array of LEDs using a logical XOR of the ledBitField
 *  provided. I.e. only '1' bits toggle an LED state. '0' bits have no effect.
 *
 *  @param ledBitField A bitmask representing an array of LEDs who's state
 *                     should be toggled. Each '1' bit in the bit field toggles
 *                     the corresponding LED on or off according to the existing
 *                     state in the LATx register (does not read from PORTx).
 *
 *  @return Bitmask representing the LEDs that were previously on (as defined in
 *          the LATx register(s)) before XORing in the ledBitField value. Bits
 *          for LEDs that do not exist are returned as '0' values in the
 *          bitmask.
 */
unsigned int LEDToggle(unsigned int ledBitField);


/**
 * Turns off an array of LEDs using a logical NOT-AND of the ledBitField
 * provided. I.e. only '1' bits turn LEDs off, '0' bits have no effect.
 *
 * This function internally calls LEDToggle() twice, so LEDToggle() should be
 * defined to use this ezbl_lib API. If it is not defined, this function
 * accesses (all 16-bits that are implemented in) LATA instead.
 *
 * @param ledBitField Bitmask representing an array of LEDs that should be
 *                    turned off. Each '1' bit in the bit field turns the
 *                    corresponding LED off.
 *
 * @return Bitmask representing the LEDs that were previously off before
 *         NOT-ANDing in the ledBitField value. Bits for LEDs that do not exist
 *         are returned as '1' values in the bitmask.
 */
unsigned int LEDOff(unsigned int ledBitField);


/**
 * Turns on an array of LEDs using a logical OR of the ledBitField provided.
 * I.e. only '1' bits turn on LEDs, '0' bits have no effect.
 *
 * This function internally calls LEDToggle() twice, so LEDToggle() should be
 * defined to use this ezbl_lib API. If it is not defined, this function
 * accesses (all 16-bits that are implemented in) LATA instead.
 *
 * @param ledBitField Bitmask representing an array of LEDs that should be
 *                    turned on. Each '1' bit in the bit field turns the
 *                    corresponding LED on.
 *
 * @return Bitmask representing the LEDs that were previously on before ORing
 *         in the ledBitField value. Bits for LEDs that do not exist are
 *         returned as '0' values in the bitmask.
 */
unsigned int LEDOn(unsigned int ledBitField);


/**
 * Turns on or off all LEDs according to the supplied bitfield.
 *
 * This function internally calls LEDToggle() twice, so LEDToggle() should be
 * defined to use this ezbl_lib API. If it is not defined, this function
 * accesses (all 16-bits that are implemented in) LATA instead.
 *
 * @param ledBitField Bitmask representing an array of LEDs that should be
 *                    turned on and off. Each '1' bit in the bit field turns
 *                    the corresponding LED on while each '0' turns the LED
 *                    off.
 *
 * @return Bitmask representing the LEDs that were previously on before setting
 *         the new value. Bits for LEDs that do not exist are returned as '0'
 *         values in the bitmask.
 */
unsigned int LEDSet(unsigned int ledBitField);


extern unsigned int ButtonsLastState;   // Last observed bitfield button state returned by ButtonRead(). Use this variable to re-reference the same value without storing the first ButtonRead() value locally.
extern unsigned int ButtonsToggled;     // Bitfield of buttons that have changed state between the two most recent calls to ButtonRead().
extern unsigned int ButtonsPushed;      // Bitfield of buttons that have changed from '0' to '1' between the two most recent calls to ButtonRead().
extern unsigned int ButtonsReleased;    // Bitfield of buttons that have changed from '1' to '0' between the two most recent calls to ButtonRead().


/**
 * Reads the state of all buttons without updating any internal state.
 * ButtonsLastState, ButtonsPushed, ButtonsReleased, and ButtonsChanged are
 * unchanged.
 *
 * @return Bitmask representing the buttons that are currently pushed ('1' =
 *         pushed, '0' = released). Button positions that do not exist are
 *         returned as '0' or released.
 */
unsigned int ButtonPeek(void);


/**
 * Reads the state of all buttons into the ButtonsLastState, ButtonsToggled,
 * ButtonsPushed, and ButtonsReleased global bitmask variables and returns the
 * present button I/O states.
 *
 * Each '1' bit in the ButtonsToggled/Pushed/Released variables represents an
 * instantaneous event. For example, if ButtonRead() indicates a button that was
 * previously released is now pushed, then ButtonsToggled and ButtonsPushed will
 * have a '1' stored in the corresponding button bitmask position. '0' will be
 * returned in all variables if ButtonRead() is subsequently called and the user
 * is still holding down the button. Similarly, upon button release,
 * ButtonsToggled and ButtonsReleased will have a '1' stored in the
 * corresponding button bitmask position. These '1's will revert to '0's when
 * ButtonRead() is called again
 *
 * @return Bitmask representing the buttons that are currently pushed ('1' =
 *         pushed, '0' = released). Button positions that do not exist are
 *         returned as '0'.
 *
 *         If you wish to read the current I/O state without altering the
 *         ButtonsLastState/ButtonsToggled/ButtonsPushed/ButtonsReleased values,
 *         call ButtonPeek() instead of ButtonRead().
 */
unsigned int ButtonRead(void);


/************** End of Default Hardware Initializer APIs **********************/




/****************** Default UART FIFO APIs ************************************/





extern EZBL_FIFO __attribute__((persistent))                                UART_RxFifo;        /* Main structure for accessing the UART RX FIFO. Use a pointer to this in EZBL_FIFO*() calls. Note UART_RxFifo defaults to using the UART2 hardware peripheral, but does not generate function prefix numbes.  */
extern EZBL_FIFO __attribute__((persistent))                                UART_TxFifo;        /* Main structure for accessing the UART TX FIFO. Use a pointer to this in EZBL_FIFO*() calls  Note UART_TxFifo defaults to using the UART2 hardware peripheral, but does not generate function prefix numbes.  */
extern unsigned char __attribute__((section(".bss.UART_TxFifoBuffer")))     UART_TxFifoBuffer[];/* Memory array storing the actual FIFO data contents; Must be in the same named section that UART_TX_FIFO_GetSize() inline code uses. */
extern unsigned char __attribute__((section(".bss.UART_RxFifoBuffer")))     UART_RxFifoBuffer[];/* Memory array storing the actual FIFO data contents; Must be in the same named section that UART_RX_FIFO_GetSize() inline code uses. */


/**
 * See UART_ResetCalc(). This is the same function, but with
 * peripheralClockSpeed and baudRate pre-computed into a value that can be
 * written to the UxBRG register.
 */
void UART_ResetBRG(int UxBRGVal);


/**
 * Resets the hardware UART 2 peripheral and software FIFOs. All FIFO data is
 * lost. The UART is configured and enabled afterwards and ready for both TX and
 * RX. This function will disarm auto-baud if armed and >0 is specified for
 * baudRate.
 *
 * @param peripheralClockSpeed Frequency, in hertz, that the UART peripheral is
 *                             operating with. On PIC24 and dsPIC products, this
 *                             is the same as the instructions executed/second
 *                             while not dozing. Ex: set to 70000000 if running
 *                             at 70 MIPS.
 *
 * @param baudRate Desired baud rate for the UART RX and TX operations, or <= 0
 *                 for auto-baud mode. Ex: set to 115200 for 115200 bits/second
 *                 (11520 bytes/second with 1 start bit, 8 data bits, and 1 stop
 *                 bit). The actual baud rate programmed will be as close as
 *                 possible to the requested value, but could still result in
 *                 appreciable error if specifying very fast baud rates or when
 *                 operating at a slow peripheral clock speed. See the device 
 *                 data sheet or UART Family Reference Manual documentation to
 *                 understand the underlying error limits. This function always
 *                 enables the UART in high-speed (4x Baud-clock) mode to
 *                 minimize high-baud rate error.
 *
 *                 For auto-baud mode, a 0x55 character must be the first
 *                 character received for the hardware to properly compute the
 *                 baud clock divider.
 */
static inline void __attribute__((always_inline, optimize(1))) UART_ResetCalc(unsigned long peripheralClockSpeed, long baudRate)
{
    UART_ResetBRG(EZBL_CalcUxBRGH(peripheralClockSpeed, baudRate));
}


/**
 * Blocks execution until everything pending is finished being physically
 * transmitted or a specified NOW timeout is reached, whichever occurs first.
 *
 * This function will actively call the FIFO's interrupt if interrupts are
 * blocked or the specific applicable IECx bit is clear, allowing data to
 * continue to be transmitted at any time.
 *
 * @param *flushFIFO Pointer to the this TX FIFO that a flush request has been
 *                   made against.
 *
 *                   This parameter is included so the generic EZBL_FIFOFlush()
 *                   function can call a generic COM_TX_FIFO_Flush()
 *                   implementation shared by several run-time FIFOs via the
 *                   EZBL_FIFO.flushFunction() function pointer and still permit
 *                   special behavior for one or more FIFOs.
 *
 * @return If successful, the software TX FIFO and the hardware communications
 *         TX FIFO (if any) are drained to 0 bytes before returning with a value
 *         of 1. On timeout, a value of 1 is returned.
 */
unsigned int UART_TX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);


/**
 * Blocks execution until everything pending is finished being physically
 * transmitted. The software TX FIFO and the hardware UART TX FIFO are drained
 * to 0 bytes before returning.
 */
void UART_TX_FIFO_WaitUntilFlushed(void);


/**
 * Throws all data in this RX FIFO away and clears any error conditions.
 *
 * @param *flushFIFO Pointer to the this RX FIFO that a flush request has been
 *                   made against.
 *
 *                   This parameter is included so the generic EZBL_FIFOFlush()
 *                   function can call a generic flush function shared by
 *                   several run-time FIFOs via the EZBL_FIFO.flushFunction()
 *                   function pointer and still permit special behavior for one
 *                   or more FIFOs.
 *
 * @return RX FIFO's dataCount is cleared to zero and a successful return code
 *         of 1 is always returned.
 */
unsigned int UART_RX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);


/**
 * Enables interrupts used by the UART FIFO TX and RX routines by setting the
 * associated IECx bits.
 */
void UART_FIFO_EnableInterrupts(void);


/**
 * Disables all interrupts used by the UART FIFO routines by clearing the
 * associated IECx bits.
 */
void UART_FIFO_DisableInterrupts(void);


/**
 * Turns on hardware Auto-baud rate detection. Next character received must be
 * 0x55 ('U'), after which UART RX will begin operating normally. UART TX rate
 * can change if transmitting while the auto-baud character is received. The
 * 0x55 character used for baud rate detection is not returned in the software
 * RX FIFO (nor hardware RX FIFO either).
 */
void UART_FIFO_ArmAutoBaud(void);


/**
 * Tests if the RX hardware is armed for Auto-baud detection. If armed, the next
 * RX character is used to set the baud-rate and therefore must be the value
 * 0x55 ('U'). This character does not get inserted into the RX stream when
 * it is used for auto-baud.
 *
 * @return 1 if auto-baud mode is set (UART_FIFO_ArmAutoBaud() has been called
 *         and a timing character has not been received yet).
 *         0 if auto-baud has completed or has never been armed.
 */
unsigned int UART_FIFO_QueryAutoBaud(void);


/**
 * Callback function automatically invoked anytime a write is made against the
 * UART TX FIFO (with or without any data actually getting written). Writes
 * occur when the EZBL_FIFOWrite() function is called (possibly indirectly
 * through a wrapper function).
 *
 * This callback executes after the write has already taken place and is a
 * convenient place to trigger necessary hardware actions, spy on the data
 * passing through, tee it to another communications or storage interface, or
 * implement additional FIFO features, such as a blocking/almost guaranteed
 * write with timeout mechanism to simplify other communications code.
 *
 * If a write is made without enough FIFO space to store all of the data, this
 * callback executes after the FIFO has been completely filled and provides an
 * opportunity to process the residual data.
 *
 * The default callback implementation will block when the FIFO is full and
 * trigger the UART TX ISR as needed to ensure the data doesn't have to be
 * thrown away. However, blocking will abort and throw unbuffered data away
 * after a fixed timeout is reached (ex: UART's 'ON' bit is clear, resulting in
 * clearance of data from the FIFO) The default timeout is set to 250ms (2,880
 * bytes @ 115200 baud, 480 bytes @ 19200), which can be changed in the callback
 * code.
 *
 * @param bytesPushed Number of bytes that actually got written to the software
 *                    FIFO before this callback was invoked.
 * @param *writeSrc Pointer to the memory originally used when the FIFO write
 *                  call was made. As no other code has executed yet, ordinary
 *                  RAM data pointers can be re-read here to see or peek at all
 *                  of the requested write data.
 * @param regWriteLen Number of bytes that were requested to be written when the
 *                    FIFO write call was made. Generally this will match the
 *                    bytesPushed value unless the TX FIFO is completely full.
 * @param *destFIFO Pointer to the EZBL_FIFO that called this callback function.
 *
 * @return Number of bytes that you want to return back to the EZBL_FIFOWrite()
 *         caller as reportedly being successfully written. Generally you should
 *         return bytesPushed unless you've taken some action to transfer more
 *         data (or perhaps stolen some back out of the FIFO).
 */
unsigned int UART_TX_FIFO_OnWrite(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *writeFIFO);


/**
 * Callback function automatically invoked anytime a software read attempt is
 * made against the UART RX FIFO, but before the underlying EZBL_FIFORead() or
 * EZBL_FIFOPeek() call returns data to the caller. Provides an early
 * opportunity to block for more data being available, spy on the data passing
 * from the UART hardware into software, auto-generate flow control/status
 * information for the remote node, or potentially allows manipulation of
 * returned data.
 *
 * This function is called at the same IPL context as the caller for
 * EZBL_FIFORead()/EZBL_FIFOPeek().
 *
 * @param bytesPulled Number of bytes peeked or extracted from the FIFO and
 *                    being presently returned to the caller of
 *                    EZBL_FIFORead()/EZBL_FIFOPeek(). This is the number of
 *                    valid bytes present at *readDest.
 *
 * @param readDest Pointer to a caller allocated buffer where read data has been
 *                 written for the original call to
 *                 EZBL_FIFORead()/EZBL_FIFOPeek(). This pointer may be null if
 *                 the caller is discarding (flushing) data from the FIFO
 *                 without actually reading it.
 *
 * @param reqReadLen Number of bytes requested to be read when
 *                   EZBL_FIFORead()/EZBL_FIFOPeek() was called. This number
 *                   will be < bytesPulled if the FIFO did not have enough data
 *                   to satisfy the original request. This value may be zero.
 *
 * @param readFIFO Pointer to the EZBL_FIFO that EZBL_FIFORead()/EZBL_FIFOPeek()
 *                 was attempted against. As the same callback handler could
 *                 potentially be used for multiple FIFOs, this pointer provides
 *                 a context specific handle back to the FIFO that called this
 *                 callback function.
 *
 * @return Specify the number of bytes of data now available at *readDest for
 *         the caller to consume. If you do not add or subtract any data from
 *         the stream within this callback, always return bytesPulled,
 *         unchanged.
 *
 *         The default callback implementation blocks for up to 1 second if a
 *         read for data is made when insufficient data exists in the FIFO to
 *         fulfill the read request. While waiting, any just-received data is
 *         copied to the *readDest buffer to allow the original reqReadLen value
 *         to be met.
 *
 *         When blocking is required, this callback can force polled execution
 *         of the RX ISR if hardware has data available and the RX Interrupt is
 *         disabled or masked due to present execution above the RX ISR
 *         priority.
 */
unsigned int UART_RX_FIFO_OnRead(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *readFIFO);


/**
 * Flushes (deletes) all data from the software UART RX FIFO and reinitializes
 * default values in the UART_RxFifo structure.  The activity flags are all
 * cleared, except for 'other' which is set to '1'.
 */
static inline void __attribute__((always_inline, optimize(1)))                  UART_RX_FIFO_Reset(void)
{
    EZBL_FIFOReset(&UART_RxFifo, &UART_RxFifoBuffer, (unsigned int)__builtin_section_size(".bss.UART_RxFifoBuffer"), 0, UART_RX_FIFO_OnRead);
}

/**
 * Returns the instantaneous number of bytes of free space within the software
 * UART RX FIFO.
 *
 * This is an inline function equivalent to:
 *      (UART_RxFifo.fifoSize - UART_RxFifo.dataCount)
 */
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_WritableLength(void)
{
    return UART_RxFifo.fifoSize - UART_RxFifo.dataCount;
}

/**
 * Returns an instantaneous count of bytes in the software UART RX FIFO that are
 * ready to be immediately read or peeked without blocking.
 *
 * This is an inline function equivalent to reading:
 *      UART_RxFifo.dataCount
 */
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_ReadableLength(void)
{
    return UART_RxFifo.dataCount;
}


static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_Write(void *writeData, unsigned int dataLen)
{
    return EZBL_FIFOWrite(&UART_RxFifo, writeData, dataLen);
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_Write8(unsigned char writeData)
{
    return EZBL_FIFOWrite(&UART_RxFifo, &writeData, 1);
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_Write16(unsigned int writeData)
{
    return EZBL_FIFOWrite(&UART_RxFifo, &writeData, 2);
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_Write24(unsigned long writeData)
{
    return EZBL_FIFOWrite(&UART_RxFifo, &writeData, 3);
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_Write32(unsigned long writeData)
{
    return EZBL_FIFOWrite(&UART_RxFifo, &writeData, 4);
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_Write48(unsigned long long writeData)
{
    return EZBL_FIFOWrite(&UART_RxFifo, &writeData, 6);
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_Write64(unsigned long long writeData)
{
    return EZBL_FIFOWrite(&UART_RxFifo, &writeData, 8);
}


static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_Read(void *readData, unsigned int dataLen)
{
    return EZBL_FIFORead(readData, &UART_RxFifo, dataLen);
}
static inline unsigned char __attribute__((always_inline, optimize(1)))         UART_RX_FIFO_Read8(void)
{
    unsigned char readData;
    EZBL_FIFORead(&readData, &UART_RxFifo, 1);
    return readData;
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_Read16(void)
{
    unsigned int readData;
    EZBL_FIFORead(&readData, &UART_RxFifo, 2);
    return readData;
}
static inline unsigned long __attribute__((always_inline, optimize(1)))         UART_RX_FIFO_Read24(void)
{
    unsigned long readData;
    EZBL_FIFORead(&readData, &UART_RxFifo, 3);
    ((unsigned char *)&readData)[3] = 0x00;
    return readData;
}
static inline unsigned long __attribute__((always_inline, optimize(1)))         UART_RX_FIFO_Read32(void)
{
    unsigned long readData;
    EZBL_FIFORead(&readData, &UART_RxFifo, 4);
    return readData;
}
static inline unsigned long long __attribute__((always_inline, optimize(1)))    UART_RX_FIFO_Read48(void)
{
    unsigned long long readData;
    EZBL_FIFORead(&readData, &UART_RxFifo, 6);
    ((unsigned char *)&readData)[6] = 0x00;
    ((unsigned char *)&readData)[7] = 0x00;
    return readData;
}
static inline unsigned long long __attribute__((always_inline, optimize(1)))    UART_RX_FIFO_Read64(void)
{
    unsigned long long readData;
    EZBL_FIFORead(&readData, &UART_RxFifo, 8);
    return readData;
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_Peek(void *peekData, unsigned int dataLen)
{
    return EZBL_FIFOPeek(peekData, &UART_RxFifo, dataLen);
}
static inline unsigned char __attribute__((always_inline, optimize(1)))         UART_RX_FIFO_Peek8(void)
{
    unsigned char readData;
    EZBL_FIFOPeek(&readData, &UART_RxFifo, 1);
    return readData;
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_Peek16(void)
{
    unsigned int readData;
    EZBL_FIFOPeek(&readData, &UART_RxFifo, 2);
    return readData;
}
static inline unsigned long __attribute__((always_inline, optimize(1)))         UART_RX_FIFO_Peek24(void)
{
    unsigned long readData;
    EZBL_FIFOPeek(&readData, &UART_RxFifo, 3);
    ((unsigned char *)&readData)[3] = 0x00;
    return readData;
}
static inline unsigned long __attribute__((always_inline, optimize(1)))         UART_RX_FIFO_Peek32(void)
{
    unsigned long readData;
    EZBL_FIFOPeek(&readData, &UART_RxFifo, 4);
    return readData;
}
static inline unsigned long long __attribute__((always_inline, optimize(1)))    UART_RX_FIFO_Peek48(void)
{
    unsigned long long readData;
    EZBL_FIFOPeek(&readData, &UART_RxFifo, 6);
    ((unsigned char *)&readData)[6] = 0x00;
    ((unsigned char *)&readData)[7] = 0x00;
    return readData;
}
static inline unsigned long long __attribute__((always_inline, optimize(1)))    UART_RX_FIFO_Peek64(void)
{
    unsigned long long readData;
    EZBL_FIFOPeek(&readData, &UART_RxFifo, 8);
    return readData;
}
static inline void __attribute__((always_inline, optimize(1)))                  UART_TX_FIFO_Reset(void)
{
    EZBL_FIFOReset(&UART_TxFifo, &UART_TxFifoBuffer, (unsigned int)__builtin_section_size(".bss.UART_TxFifoBuffer"), UART_TX_FIFO_OnWrite, 0);
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_WritableLength(void)
{
    return UART_TxFifo.fifoSize - UART_TxFifo.dataCount;
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_ReadableLength(void)
{
    return UART_TxFifo.dataCount;
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_Write(void *writeData, unsigned int dataLen)
{
    return EZBL_FIFOWrite(&UART_TxFifo, writeData, dataLen);
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_Write8(unsigned char writeData)
{
    return EZBL_FIFOWrite(&UART_TxFifo, &writeData, 1);
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_Write16(unsigned int writeData)
{
    return EZBL_FIFOWrite(&UART_TxFifo, &writeData, 2);
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_Write24(unsigned long writeData)
{
    return EZBL_FIFOWrite(&UART_TxFifo, &writeData, 3);
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_Write32(unsigned long writeData)
{
    return EZBL_FIFOWrite(&UART_TxFifo, &writeData, 4);
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_Write48(unsigned long long writeData)
{
    return EZBL_FIFOWrite(&UART_TxFifo, &writeData, 6);
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_Write64(unsigned long long writeData)
{
    return EZBL_FIFOWrite(&UART_TxFifo, &writeData, 8);
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_WriteStr(char *str)
{
    return EZBL_FIFOWrite(&UART_TxFifo, str, __builtin_strlen(str) - 1u); /* __builtin_strlen() evaluates at compile time for string literals and automatically falls back to a strlen() call when run-time evaluation is required. */
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_Read(void *readData, unsigned int dataLen)   // Reads a specified number of bytes from the software TX FIFO and writes them to the given pointer. This is an inline wrapper that calls EZBL_FIFORead(). The return value is the number of bytes read, which can be less than the requested quantity if the FIFO did not contain enough data.
{
    return EZBL_FIFORead(readData, &UART_TxFifo, dataLen);
}
static inline unsigned char __attribute__((always_inline, optimize(1)))         UART_TX_FIFO_Read8(void)   // Reads and returns a 1-byte unsigned char quantity from the software TX FIFO. This is an inline wrapper that calls EZBL_FIFORead().
{
    unsigned char readData;
    EZBL_FIFORead(&readData, &UART_TxFifo, 1);
    return readData;
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_Read16(void)   // Reads and returns a 2-byte unsigned int quantity from the software TX FIFO. This is an inline wrapper that calls EZBL_FIFORead(). On 32-bit processors, the upper 2 bytes are returned as 0x0000.
{
    unsigned short readData;
    EZBL_FIFORead(&readData, &UART_TxFifo, 2);
    return readData;
}
static inline unsigned long __attribute__((always_inline, optimize(1)))         UART_TX_FIFO_Read24(void)   // Reads and returns a 3-byte unsigned long quantity from the software TX FIFO. This is an inline wrapper that calls EZBL_FIFORead(). The most significant byte is returned as 0x00.
{
    unsigned long readData;
    EZBL_FIFORead(&readData, &UART_TxFifo, 3);
    ((unsigned char *)&readData)[3] = 0x00;
    return readData;
}
static inline unsigned long __attribute__((always_inline, optimize(1)))         UART_TX_FIFO_Read32(void)   // Reads and returns a 4-byte unsigned long quantity from the software TX FIFO. This is an inline wrapper that calls EZBL_FIFORead().
{
    unsigned long readData;
    EZBL_FIFORead(&readData, &UART_TxFifo, 4);
    return readData;
}
static inline unsigned long long __attribute__((always_inline, optimize(1)))    UART_TX_FIFO_Read48(void)   // Reads and returns a 6-byte unsigned long long quantity from the software TX FIFO. This is an inline wrapper that calls EZBL_FIFORead(). The most significant two bytes are returned as 0x0000.
{
    unsigned long long readData;
    EZBL_FIFORead(&readData, &UART_TxFifo, 6);
    ((unsigned char *)&readData)[6] = 0x00;
    ((unsigned char *)&readData)[7] = 0x00;
    return readData;
}
static inline unsigned long long __attribute__((always_inline, optimize(1)))    UART_TX_FIFO_Read64(void)   // Reads and returns an 8-byte unsigned long long quantity from the software TX FIFO. This is an inline wrapper that calls EZBL_FIFORead().
{
    unsigned long long readData;
    EZBL_FIFORead(&readData, &UART_TxFifo, 8);
    return readData;
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_Peek(void *peekData, unsigned int dataLen)   // Reads (without removing from the FIFO) a specified number of bytes from the software TX FIFO and writes them to the given pointer. This is an inline wrapper that calls EZBL_FIFOPeek(). The return value is the number of bytes peeked, which can be less than the requested quantity if the FIFO did not contain enough data.
{
    return EZBL_FIFOPeek(peekData, &UART_TxFifo, dataLen);
}
static inline unsigned char __attribute__((always_inline, optimize(1)))         UART_TX_FIFO_Peek8(void)   // Reads and returns (without removing from the FIFO) a 1-byte unsigned char quantity from the software TX FIFO. This is an inline wrapper that calls EZBL_FIFOPeek().
{
    unsigned char peekData;
    EZBL_FIFOPeek(&peekData, &UART_TxFifo, 1);
    return peekData;
}
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_Peek16(void)   // Reads and returns (without removing from the FIFO) a 2-byte unsigned int quantity from the software TX FIFO. This is an inline wrapper that calls EZBL_FIFOPeek(). On 32-bit processors, the upper 2 bytes are returned as 0x0000.
{
    unsigned short peekData;
    EZBL_FIFOPeek(&peekData, &UART_TxFifo, 2);
    return peekData;
}
static inline unsigned long __attribute__((always_inline, optimize(1)))         UART_TX_FIFO_Peek24(void)   // Reads and returns (without removing from the FIFO) a 3-byte unsigned long quantity from the software TX FIFO. This is an inline wrapper that calls EZBL_FIFOPeek(). The most significant bytes is returned as 0x00.
{
    unsigned long peekData;
    EZBL_FIFOPeek(&peekData, &UART_TxFifo, 3);
    ((unsigned char *)&peekData)[3] = 0x00;
    return peekData;
}
static inline unsigned long __attribute__((always_inline, optimize(1)))         UART_TX_FIFO_Peek32(void)   // Reads and returns (without removing from the FIFO) a 4-byte unsigned long quantity from the software TX FIFO. This is an inline wrapper that calls EZBL_FIFOPeek().
{
    unsigned long peekData;
    EZBL_FIFOPeek(&peekData, &UART_TxFifo, 4);
    return peekData;
}
static inline unsigned long long __attribute__((always_inline, optimize(1)))    UART_TX_FIFO_Peek48(void)   // Reads and returns (without removing from the FIFO) a 6-byte unsigned long long quantity from the software TX FIFO. This is an inline wrapper that calls EZBL_FIFOPeek(). The most significant two bytes are returned as 0x0000.
{
    unsigned long long peekData;
    EZBL_FIFOPeek(&peekData, &UART_TxFifo, 6);
    ((unsigned char *)&peekData)[6] = 0x00;
    ((unsigned char *)&peekData)[7] = 0x00;
    return peekData;
}

// Reads and returns (without removing from the FIFO) an 8-byte unsigned long
// long quantity from the software TX FIFO. This is an inline wrapper that calls
// EZBL_FIFOPeek().
static inline unsigned long long __attribute__((always_inline, optimize(1)))    UART_TX_FIFO_Peek64(void)   
{
    unsigned long long peekData;
    EZBL_FIFOPeek(&peekData, &UART_TxFifo, 8);
    return peekData;
}


/**
 * Retrieves the allocated RAM for the software RX FIFO. This is equivalent to
 * reading UART_RxFifo.fifoSize.
 *
 * @return Number of bytes allocated for the FIFO, which excludes head/tail
 *         pointers and other FIFO state tracking variable size.
 */
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_GetSize(void)
{
    return UART_RxFifo.fifoSize;
}


/**
 * Copies a null-terminated string to the software RX FIFO. The null-character
 * is not transmitted.
 *
 * When a string literal is passed in, the string length is evaluated at compile
 * time. When the string's length cannot be proven at compile time, or when a
 * variable is passed in, run-time string length evaluation will occur via a
 * call to the strlen() C standard library function.
 *
 * Unlike printf(), '\n' characters are not expanded into "\r\n" sequences. The
 * string contents are transmitted as binary/non-translated data.
 *
 * @param *str Pointer to a null-terminated character string to write. The
 *             null-terminator is not written to the FIFO.
 *
 *             The string can be located in PSV flash memory or any data space
 *             location.
 *
 *             This parameter cannot be null.
 *
 * @return Number of bytes copied into the FIFO. This can be less than the
 *         string's length if the RX FIFO is full and the FIFO's OnWrite()
 *         callback does not block or otherwise arrange for the data to be
 *         consumed.
 *
 *         The default behavior will block until all bytes are successfully
 *         placed in the FIFO or an excessive blocking timeout occurs. See the
 *         RX FIFO's OnWrite() callback implementation for the specific timeout
 *         period.
 */
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_RX_FIFO_WriteStr(char *str)
{
    return EZBL_FIFOWrite(&UART_RxFifo, str, __builtin_strlen(str) - 1u); // __builtin_strlen() evaluates at compile time for string literals and automatically falls back to a strlen() call when run-time evaluation is required.
}


/**
 * Retrieves the allocated RAM for the software TX FIFO. This is equivalent to
 * reading UART_TxFifo.fifoSize.
 *
 * @return Number of bytes allocated for the TX FIFO, which excludes head/tail
 *         pointers and other FIFO state tracking variable size.
 */
static inline unsigned int __attribute__((always_inline, optimize(1)))          UART_TX_FIFO_GetSize(void)
{
    return UART_TxFifo.fifoSize;
}


/**** Using generator macros below to create many copies of COM functions and extern definitions ****/
// The macro's rename and duplicate APIs for different peripheral instances.
// For context sensitive API help, refer to the non-macro generated UART_*()
// API's above.

#define MAKE_FIFO_PROROTYPES(peripheralName)                                                                                                                                                                                                                                                \
extern EZBL_FIFO __attribute__((persistent)) peripheralName##_RxFifo;   /* Main structure for accessing the UART RX FIFO. Use a pointer to this in EZBL_FIFO*() calls. Note UART_RxFifo defaults to using the UART2 hardware peripheral, but does not generate function prefix numbes.  */  \
extern EZBL_FIFO __attribute__((persistent)) peripheralName##_TxFifo;   /* Main structure for accessing the UART TX FIFO. Use a pointer to this in EZBL_FIFO*() calls  Note UART_TxFifo defaults to using the UART2 hardware peripheral, but does not generate function prefix numbes.  */  \
extern unsigned char __attribute__((section(".bss." STRINGIFY(peripheralName) "_TxFifoBuffer")))  peripheralName##_TxFifoBuffer[];  /* Memory array storing the actual FIFO data contents; Must be in the same named section that UART_TX_FIFO_Reset() inline code uses. */                 \
extern unsigned char __attribute__((section(".bss." STRINGIFY(peripheralName) "_RxFifoBuffer")))  peripheralName##_RxFifoBuffer[];  /* Memory array storing the actual FIFO data contents; Must be in the same named section that UART_RX_FIFO_Reset() inline code uses. */                 \
                                                                                                                                                                                                                                                                                            \
void peripheralName##_TX_FIFO_WaitUntilFlushed(void);   /* Blocks until TX FIFO is empty (in both software and hardware). Use EZBL_FIFOFlush() instead if a timeout is needed. */                                                                                                           \
unsigned int peripheralName##_TX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);    /* Blocks until both the software and hardware TX FIFOs drop to 0 bytes, or the specified timeout is reached. */                                                    \
unsigned int peripheralName##_RX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);    /* Clears all data in the RX FIFO and any sticky errors recorded */                                                                                                 \
void peripheralName##_FIFO_EnableInterrupts(void);      /* Sets this module's UART TX and RX interrupt enable flags */                                                                                                                                                                      \
void peripheralName##_FIFO_DisableInterrupts(void);     /* Clears this module's UART TX and RX interrupt enable flags */                                                                                                                                                                    \
unsigned int peripheralName##_TX_FIFO_OnWrite(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *writeFIFO);                                                                                                                                                    \
unsigned int peripheralName##_RX_FIFO_OnRead(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *readFIFO);


#define MAKE_EXTRA_UART_FIFO_PROTOTYPES(peripheralName)                                                                                                                                                                                                 \
void peripheralName##_ResetBRG(int UxBRGVal);                                                                                                                                                                                                           \
static inline void __attribute__((always_inline, optimize(1))) peripheralName##_ResetCalc(unsigned long peripheralClockSpeed, long baudRate) /* Wrapper for UARTx_ResetBRG(), but computing the UxBRG value at run time rather than compile time. */    \
{                                                                                                                                                                                                                                                       \
    peripheralName##_ResetBRG(EZBL_CalcUxBRGH(peripheralClockSpeed, baudRate));                                                                                                                                                                         \
}                                                                                                                                                                                                                                                       \
void peripheralName##_FIFO_ArmAutoBaud(void);           /* Configures hardware to automatically set Baud rate on reception of next character (which must be 0x55) */                                                                                    \
unsigned int peripheralName##_FIFO_QueryAutoBaud(void); /* Returns 1 if Auto-baud mode is enabled (waiting for 0x55); 0 if auto-baud isn't armed or has already completed. */


#define MAKE_EXTRA_I2C_FIFO_PROTOTYPES(peripheralName)                                              \
void peripheralName##_ResetCalc(unsigned long peripheralClockSpeed, long baudRate, int slaveAddr);  \
void peripheralName##_ResetBRG(int I2CxBRGVal, int slaveAddr);


MAKE_FIFO_PROROTYPES(UART1)
MAKE_FIFO_PROROTYPES(UART2)
MAKE_FIFO_PROROTYPES(UART3)
MAKE_FIFO_PROROTYPES(UART4)
MAKE_FIFO_PROROTYPES(UART5)
MAKE_FIFO_PROROTYPES(UART6)
MAKE_EXTRA_UART_FIFO_PROTOTYPES(UART1)
MAKE_EXTRA_UART_FIFO_PROTOTYPES(UART2)
MAKE_EXTRA_UART_FIFO_PROTOTYPES(UART3)
MAKE_EXTRA_UART_FIFO_PROTOTYPES(UART4)
MAKE_EXTRA_UART_FIFO_PROTOTYPES(UART5)
MAKE_EXTRA_UART_FIFO_PROTOTYPES(UART6)
MAKE_FIFO_PROROTYPES(I2C1)
MAKE_FIFO_PROROTYPES(I2C2)
MAKE_FIFO_PROROTYPES(I2C3)
MAKE_EXTRA_I2C_FIFO_PROTOTYPES(I2C1)
MAKE_EXTRA_I2C_FIFO_PROTOTYPES(I2C2)
MAKE_EXTRA_I2C_FIFO_PROTOTYPES(I2C3)

        
#if defined(__dsPIC33C__) || defined(__dsPIC33CH__) || defined(EZBL_LIB16CH) || defined(U1MODEH)
#define UART_CLK_MULT   2u  // dsPIC33C devices support FCY*2 as a clock input option
#else
#define UART_CLK_MULT   1u
#endif


#if defined(__XC16__)
/**
 * EZBL_FIFO * UART_Reset(int peripheralNum, unsigned long peripheralClockSpeed, long baudRate, int useForStdio);
 *
 * Macro optimally calls UARTx_ResetBRG() or UARTx_ResetCalc() to initialize and
 * configure a UART hardware peripheral and pair of software FIFOs. This
 * generates an ezbl_lib.a reference which includes the UARTx_TxFifoBuffer[] and
 * UARTx_RxFifoBuffer[] RAM arrays as well as UARTx_TxFifo and UARTx_RxFifo
 * structures for maintaining the FIFO pointers. Macro returns a pointer to the
 * UARTx_RxFifo structure for the just-initialized UART peripheral.
 *
 * If peripheralClockSpeed and baudRate are both compile-time constants or
 * baudRate is zero (auto-baud), this macro resolves to a call to
 * UARTx_ResetBRG() to save code space.  Otherwise, it resolves to a call to the
 * UARTx_ResetCalc() wrapper function which precomputes the needed UxBRG value,
 * given run time values for peripheral clock speed and desired baud-rate.
 *
 * Calling this more than once against the same UART peripheralNum will reset
 * all hardware peripheral state and clear the software TX and RX FIFO contents.
 *
 * @param peripheralNum UART peripheral number to initialize and attach software
 *                      FIFOs to. ezbl_lib.a supports the following values:
 *                      <ul><li>1 - UART 1 peripheral</li>
 *                          <li>2 - UART 2 peripheral</li>
 *                          <li>3 - UART 3 peripheral</li>
 *                          <li>4 - UART 4 peripheral</li>
 *                          <li>5 - UART 5 peripheral</li>
 *                          <li>6 - UART 6 peripheral</li>
 *                      </ul>
 *                      If a peripheral number is specified which does not exist
 *                      on the target processor, a linking error will occur at
 *                      build time.
 *
 *                      This parameter must be a compile time constant. I.e. it
 *                      must be a literal number or macro expression that
 *                      evaluates to a literal number.
  *
 * @param peripheralClockSpeed Frequency that the UART hardware peripheral is
 *                             being externally clocked with, in Hertz. This
 *                             value is ignored if baudRate is set to 0,
 *                             indicating auto-baud without a pre-auto-baud TX
 *                             rate.
 *
 *                             Typically, for 16-bit devices, the peripheral's 
 *                             clock speed will match the execution rate (ex:
 *                             16 MIPS = 16000000 peripheralClockSpeed)
 *
 *                             On dsPIC33C devices, the UART peripheral is
 *                             configured to run at Fosc (2*Fcy) for best baud
 *                             resolution, but assumes peripheralClock is still
 *                             specified as Fcy. The UxBRG value computed will
 *                             be computed against 2x this "peripheralClock"
 *                             parameter value.
 * *
 * @param baudRate Baud rate to initialize the UART to, in bits per second.
 *                 Specify <= 0 if you wish to enable auto-baud (first character
 *                 received must be the 0x55 (or 'U' character) to set the baud
 *                 rate).
 *
 *                 Values less than zero will enable auto-baud, but also 
 *                 compute and set the UxBRG register to the value needed to 
 *                 achieve (-baudRate). I.e. the bit-rate if baudRate were 
 *                 specified as a positive number. This will typically allow
 *                 TX operations at a fixed, known baud rate until/unless an
 *                 RX character is received (should be the character 'U' or
 *                 binary byte 0x55) for hardware to complete auto-baud. For
 *                 example, setting baudRate to -230400 and then calling
 *                 EZBL_printf() can allow uni-directional console message
 *                 printing at 230400 bps without forcing a future
 *                 bi-directional bootloading session to use the fixed baud
 *                 rate.
 *
 *                 If interfacing to an MCP2221A, the baud rate should be <=
 *                 460800 to ensure interoperability. If interfacing to an
 *                 MCP2200, the baud rate should be <= 921600. In both cases,
 *                 when auto-baud is enabled, the actual baud rate is set by the
 *                 PC or remote communications partner. Non-standard baud rates
 *                 can be used with the MCP2221A/MCP2200 to reduce net
 *                 baud-error between the two nodes.
 *
 *                 The value for UxBRG is computed by rounding either up or down
 *                 so as to minimize the magnitude of baud rate error from the
 *                 requested value.
 *
 * @param useForStdio Flag to indicate if calls to the write() and read() 
 *                    functions should route to this communications resource. 
 *                    When useForStdio == 1, EZBL_STDOUT is set to &UARTx_TxFifo
 *                    and EZBL_STDIN is set to &UARTx_RxFifo, where 'x' is the
 *                    peripheralNum parameter value. Additionally,
 *                    write()/read() or _mon_putc()/_mon_getc() functions are
 *                    implemented to redirect compiler stdio data to these
 *                    software FIFOs.
 *                    
 *                    Using this communications resource for stdio affects 
 *                    functions like printf(), scanf(), as well as EZBL APIs 
 *                    that use the EZBL_STDOUT or EZBL_STDIN fifo pointers.
 *                    For example, a call to printf() will not write
 *                    to UARTx directly via UxTXREG. Instead, the printf()
 *                    statement will write to the UARTx_TxFifo with circular
 *                    buffering and non-blocking behavior. The data will then be
 *                    transmitted autonomously via the _UxTXInterrupt() attached
 *                    to the TX FIFO, allowing the printf() statement to return
 *                    quicker (unless the FIFO is full, in which case it will
 *                    begin blocking for buffer space availability or write
 *                    timeout).
 *
 *                    If at any time the output FIFO needs to be flushed to
 *                    emulate a normal blocking stdio write, the following call
 *                    can be used:
 *                      EZBL_FIFOFlush(EZBL_STDOUT, (unsigned long)-1);
 *
 *                    stdio can be reassigned at execution time by changing the
 *                    the EZBL_STDOUT/EZBL_STDIN pointers or by calling a
 *                    UART_Reset()/I2C_Reset()/etc. API that sets itself as the
 *                    stdio target.
 *
 * @return (EZBL_FIFO*) pointer to the UARTx_RxFifo implemented by the given
 *         UART FIFO library. This is equivalent to &UART1_RxFifo,
 *         &UART2_RxFifo, &UART3_RxFifo, etc., depending on the peripheralNum
 *         input parameter.
 *         
 *         UxMODE/UxSTAT/UxBRG registers are initialized/reinitialized for UART
 *         operation with 8N1, no flow control, UARTx_TxFifo and UARTx_RxFifo
 *         are initialized to contain no data (with backing buffers defined by
 *         UARTx_TxFifoBuffer[] and UARTx_RxFifoBuffer[]), and the UART TX and
 *         RX interrupts are enabled (with ISR handler defined to fill/consume
 *         from the respective FIFOs at low priority - default IPL1 for TX, IPL2
 *         for RX).
 *
 *         The I/O pins and PPS settings applicable to the UART TX/RX lines are
 *         NOT initialized by this function and must be handled externally,
 *         typically by assigning _RPyR = _RPOUT_UxTX and _UxRXR = z, where
 *         'x' is the peripheralNum, 'y' is the Remapable Pin number for UxTX
 *         and 'z' is the Remapable Pin number for UxRX. If any analog pin
 *         functionality is present on the RX I/O line, the pin must be changed
 *         to operate in digital mode.
 */
#define UART_Reset(peripheralNum, peripheralClockSpeed, baudRate, useForStdio)                                          \
({                                                                                                                      \
    __attribute__((always_inline, optimize(1))) EZBL_FIFO * UART_ResetFunc(void)                                        \
    {                                                                                                                   \
        if(((peripheralNum) >= 1) && ((peripheralNum) <= 6))                                                            \
        {                                                                                                               \
            if(__builtin_constant_p(baudRate) && ((baudRate) == 0))                                                     \
                UART##peripheralNum##_ResetBRG(0);                                                                      \
            else if(__builtin_constant_p(peripheralClockSpeed) && __builtin_constant_p(baudRate) && ((baudRate) > 0))   \
                UART##peripheralNum##_ResetBRG((int)(((((unsigned long)(peripheralClockSpeed))*UART_CLK_MULT) + ((unsigned long)(baudRate))*2u)/(4u*((unsigned long)(baudRate))) - 1u));    \
            else if(__builtin_constant_p(peripheralClockSpeed) && __builtin_constant_p(baudRate) && ((baudRate) < 0))                                                                       \
                UART##peripheralNum##_ResetBRG(-(int)(((((unsigned long)(peripheralClockSpeed))*UART_CLK_MULT) + ((unsigned long)-(baudRate))*2u)/(4u*((unsigned long)-(baudRate))) - 1u)); \
            else                                                                                                        \
                UART##peripheralNum##_ResetBRG(EZBL_CalcUxBRGH((peripheralClockSpeed)*UART_CLK_MULT, (baudRate)));      \
                                                                                                                        \
            if(useForStdio)                                                                                             \
            {                                                                                                           \
                EZBL_KeepSYM(EZBL_write);                                                                               \
                EZBL_KeepSYM(EZBL_read);                                                                                \
                EZBL_STDOUT = &UART##peripheralNum##_TxFifo;                                                            \
                /* EZBL_STDERR = &UART##peripheralNum##_TxFifo;  Not used in this version of EZBL. */                   \
                EZBL_STDIN  = &UART##peripheralNum##_RxFifo;                                                            \
            }                                                                                                           \
            return &UART##peripheralNum##_RxFifo; /* GNU return value for whole macro */                                \
        }                                                                                                               \
        else                                                                                                            \
        {                                                                                                               \
            __asm__(".error UART" STRINGIFY(peripheralNum) " is not supported in this version of ezbl_lib or is an unrecognized token. \nOnly literal numbers 1, 2, 3, 4 or preprocessor macros evaluating to a literal number are allowed (no variables)");    \
            return (void*)0;                                                                                            \
        }                                                                                                               \
    };                                                                                                                  \
    UART_ResetFunc();   /* calls inline macro and provides GNU macro return value */                                    \
})


#elif defined(__XC32__) || defined(__PIC32__)
/**
 * Turns on a UART hardware peripheral and enables associated TX and RX
 * interrupts. This macro can also be used to reset a UART peripheral to a new
 * buad rate, recompute the baud rate for a new peripheral clock, or to reassign
 * stdio pointers to this peripheral.
 *
 * @param peripheralNum
 *          UART peripheral instance to reset/initialize.
 *          Ex: 1, 2 or 3 (PIC32MMxxxxGPMxxx devices only)
 *
 *          NOTE: This parameter should be specified as a compile-time constant
 *                (literal). If non-constant, code for all UART peripherals will
 *                be linked into the project, wasting code space if you don't
 *                actually need all of the UARTs.
 *
 * @param peripheralClockSpeed Clock frequency, in hertz, that the UART
 *                             peripheral is clocked with for baud rate
 *                             generator scalar computation. Generally, this
 *                             should be the same as the CPU execution rate, ex:
 *                             24000000 for 24 MIPS.
 *
 * @param baudRate Desired peripheral baud rate setting. Set to <= 0 for
 *                 auto-baud, where negative integers represent a fixed default
 *                 baud rate for TX operations, but with the peripheral
 *                 configured for auto-baud so the first RX character will reset
 *                 the baud rate. Ex: -230400 is 230400 baud, but with auto-baud
 *                 enabled.
 *
 * @param useForStdio Flag indicating if stdout/stderr/stdin should map to this
 *                    communications resource. EZBL_STDOUT/EZBL_STDIN are also
 *                    reassigned when useForStdio is non-zero.
 *
 * @return void
 */
#define UART_Reset(peripheralNum, peripheralClockSpeed, baudRate, useForStdio)                                                                          \
({                                                                                                                                                      \
    void __attribute__((vector(_UART##peripheralNum##_RX_VECTOR), vector(_UART##peripheralNum##_TX_VECTOR))) UART##peripheralNum##_RXTXInterrupt(void); \
    EZBL_FIFO *ret = 0;                                                                                                                                 \
                                                                                                                                                        \
    if(((peripheralNum) >= 1) && ((peripheralNum) <= 3))                                                                                                \
    {                                                                                                                                                   \
        if(__builtin_constant_p(baudRate) && ((baudRate) == 0))                                                                                         \
            UART##peripheralNum##_ResetBRG(0);                                                                                                          \
        else if(__builtin_constant_p(peripheralClockSpeed) && __builtin_constant_p(baudRate) && ((baudRate) > 0))                                       \
            UART##peripheralNum##_ResetBRG((int)(((((unsigned long)(peripheralClockSpeed))*UART_CLK_MULT) + ((unsigned long)(baudRate))*2u)/(4u*((unsigned long)(baudRate))) - 1u));    \
        else if(__builtin_constant_p(peripheralClockSpeed) && __builtin_constant_p(baudRate) && ((baudRate) < 0))                                                                       \
            UART##peripheralNum##_ResetBRG(-(int)(((((unsigned long)(peripheralClockSpeed))*UART_CLK_MULT) + ((unsigned long)-(baudRate))*2u)/(4u*((unsigned long)-(baudRate))) - 1u)); \
        else                                                                                                                                            \
            UART##peripheralNum##_ResetBRG(EZBL_CalcUxBRGH((peripheralClockSpeed)*UART_CLK_MULT, (baudRate)));                                          \
                                                                                                                                                        \
        if(useForStdio)                                                                                                                                 \
        {                                                                                                                                               \
            EZBL_KeepSYM(EZBL_mon_putc, EZBL_mon_getc);                                                                                                 \
            EZBL_STDOUT = &UART##peripheralNum##_TxFifo;                                                                                                \
            /* EZBL_STDERR = &UART##peripheralNum##_TxFifo;  Not used in this version of EZBL. */                                                       \
            EZBL_STDIN  = &UART##peripheralNum##_RxFifo;                                                                                                \
        }                                                                                                                                               \
        ret = &UART##peripheralNum##_RxFifo; /* GNU return value for whole macro */                                                                     \
    }                                                                                                                                                   \
                                                                                                                                                        \
    ret;    /* macro return value */                                                                                                                    \
})
#endif



#define MAKE_FUNC_RX_FIFO(peripheralName)                                                                                                               \
unsigned int peripheralName##_RX_FIFO_OnRead(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *readFIFO);                   \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_GetSize(void)                                  \
{                                                                                                                                                       \
    return peripheralName##_RxFifo.fifoSize;                                                                                                            \
}                                                                                                                                                       \
static inline void __attribute__((always_inline, optimize(1)))                  peripheralName##_RX_FIFO_Reset(void)                                    \
{                                                                                                                                                       \
    EZBL_FIFOReset(&peripheralName##_RxFifo, &peripheralName##_RxFifoBuffer, (unsigned int)__builtin_section_size(".bss." STRINGIFY(peripheralName) "_RxFifoBuffer"), 0, peripheralName##_RX_FIFO_OnRead);     \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_WritableLength(void)                           \
{                                                                                                                                                       \
    return peripheralName##_RxFifo.fifoSize - peripheralName##_RxFifo.dataCount;                                                                        \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_ReadableLength(void)                           \
{                                                                                                                                                       \
    return peripheralName##_RxFifo.dataCount;                                                                                                           \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_Write(void *writeData, unsigned int dataLen)   \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_RxFifo, writeData, dataLen);                                                                                \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_Write8(unsigned char writeData)                \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_RxFifo, &writeData, 1);                                                                                     \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_Write16(unsigned int writeData)                \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_RxFifo, &writeData, 2);                                                                                     \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_Write24(unsigned long writeData)               \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_RxFifo, &writeData, 3);                                                                                     \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_Write32(unsigned long writeData)               \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_RxFifo, &writeData, 4);                                                                                     \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_Write48(unsigned long long writeData)          \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_RxFifo, &writeData, 6);                                                                                     \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_Write64(unsigned long long writeData)          \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_RxFifo, &writeData, 8);                                                                                     \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_WriteStr(char *str)                            \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_RxFifo, str, __builtin_strlen(str) - 1u); /* __builtin_strlen() evaluates at compile time for string literals and automatically falls back to a strlen() call when run-time evaluation is required. */  \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_Read(void *readData, unsigned int dataLen)     \
{                                                                                                                                                       \
    return EZBL_FIFORead(readData, &peripheralName##_RxFifo, dataLen);                                                                                  \
}                                                                                                                                                       \
static inline unsigned char __attribute__((always_inline, optimize(1)))         peripheralName##_RX_FIFO_Read8(void)                                    \
{                                                                                                                                                       \
    unsigned char readData;                                                                                                                             \
    EZBL_FIFORead(&readData, &peripheralName##_RxFifo, 1);                                                                                              \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_Read16(void)                                   \
{                                                                                                                                                       \
    unsigned int readData;                                                                                                                              \
    EZBL_FIFORead(&readData, &peripheralName##_RxFifo, 2);                                                                                              \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long __attribute__((always_inline, optimize(1)))         peripheralName##_RX_FIFO_Read24(void)                                   \
{                                                                                                                                                       \
    unsigned long readData;                                                                                                                             \
    EZBL_FIFORead(&readData, &peripheralName##_RxFifo, 3);                                                                                              \
    ((unsigned char *)&readData)[3] = 0x00;                                                                                                             \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long __attribute__((always_inline, optimize(1)))         peripheralName##_RX_FIFO_Read32(void)                                   \
{                                                                                                                                                       \
    unsigned long readData;                                                                                                                             \
    EZBL_FIFORead(&readData, &peripheralName##_RxFifo, 4);                                                                                              \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long long __attribute__((always_inline, optimize(1)))    peripheralName##_RX_FIFO_Read48(void)                                   \
{                                                                                                                                                       \
    unsigned long long readData;                                                                                                                        \
    EZBL_FIFORead(&readData, &peripheralName##_RxFifo, 6);                                                                                              \
    ((unsigned char *)&readData)[6] = 0x00;                                                                                                             \
    ((unsigned char *)&readData)[7] = 0x00;                                                                                                             \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long long __attribute__((always_inline, optimize(1)))    peripheralName##_RX_FIFO_Read64(void)                                   \
{                                                                                                                                                       \
    unsigned long long readData;                                                                                                                        \
    EZBL_FIFORead(&readData, &peripheralName##_RxFifo, 8);                                                                                              \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_Peek(void *peekData, unsigned int dataLen)     \
{                                                                                                                                                       \
    return EZBL_FIFOPeek(peekData, &peripheralName##_RxFifo, dataLen);                                                                                  \
}                                                                                                                                                       \
static inline unsigned char __attribute__((always_inline, optimize(1)))         peripheralName##_RX_FIFO_Peek8(void)                                    \
{                                                                                                                                                       \
    unsigned char readData;                                                                                                                             \
    EZBL_FIFOPeek(&readData, &peripheralName##_RxFifo, 1);                                                                                              \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_RX_FIFO_Peek16(void)                                   \
{                                                                                                                                                       \
    unsigned short readData;                                                                                                                              \
    EZBL_FIFOPeek(&readData, &peripheralName##_RxFifo, 2);                                                                                              \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long __attribute__((always_inline, optimize(1)))         peripheralName##_RX_FIFO_Peek24(void)                                   \
{                                                                                                                                                       \
    unsigned long readData;                                                                                                                             \
    EZBL_FIFOPeek(&readData, &peripheralName##_RxFifo, 3);                                                                                              \
    ((unsigned char *)&readData)[3] = 0x00;                                                                                                             \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long __attribute__((always_inline, optimize(1)))         peripheralName##_RX_FIFO_Peek32(void)                                   \
{                                                                                                                                                       \
    unsigned long readData;                                                                                                                             \
    EZBL_FIFOPeek(&readData, &peripheralName##_RxFifo, 4);                                                                                              \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long long __attribute__((always_inline, optimize(1)))    peripheralName##_RX_FIFO_Peek48(void)                                   \
{                                                                                                                                                       \
    unsigned long long readData;                                                                                                                        \
    EZBL_FIFOPeek(&readData, &peripheralName##_RxFifo, 6);                                                                                              \
    ((unsigned char *)&readData)[6] = 0x00;                                                                                                             \
    ((unsigned char *)&readData)[7] = 0x00;                                                                                                             \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long long __attribute__((always_inline, optimize(1)))    peripheralName##_RX_FIFO_Peek64(void)                                   \
{                                                                                                                                                       \
    unsigned long long readData;                                                                                                                        \
    EZBL_FIFOPeek(&readData, &peripheralName##_RxFifo, 8);                                                                                              \
    return readData;                                                                                                                                    \
}


#define MAKE_FUNC_TX_FIFO(peripheralName)                                                                                                               \
unsigned int peripheralName##_TX_FIFO_OnWrite(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *writeFIFO);                \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_GetSize(void)                                  \
{                                                                                                                                                       \
    return peripheralName##_TxFifo.fifoSize;                                                                                                            \
}                                                                                                                                                       \
static inline void __attribute__((always_inline, optimize(1)))                  peripheralName##_TX_FIFO_Reset(void)                                    \
{                                                                                                                                                       \
    EZBL_FIFOReset(&peripheralName##_TxFifo, &peripheralName##_TxFifoBuffer, (unsigned int)__builtin_section_size(".bss." STRINGIFY(peripheralName) "_TxFifoBuffer"), peripheralName##_TX_FIFO_OnWrite, 0);  \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_WritableLength(void)                           \
{                                                                                                                                                       \
    return peripheralName##_TxFifo.fifoSize - peripheralName##_TxFifo.dataCount;                                                                        \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_ReadableLength(void)                           \
{                                                                                                                                                       \
    return peripheralName##_TxFifo.dataCount;                                                                                                           \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_Write(void *writeData, unsigned int dataLen)   \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_TxFifo, writeData, dataLen);                                                                                \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_Write8(unsigned char writeData)                \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_TxFifo, &writeData, 1);                                                                                     \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_Write16(unsigned int writeData)                \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_TxFifo, &writeData, 2);                                                                                     \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_Write24(unsigned long writeData)               \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_TxFifo, &writeData, 3);                                                                                     \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_Write32(unsigned long writeData)               \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_TxFifo, &writeData, 4);                                                                                     \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_Write48(unsigned long long writeData)          \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_TxFifo, &writeData, 6);                                                                                     \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_Write64(unsigned long long writeData)          \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_TxFifo, &writeData, 8);                                                                                     \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_WriteStr(char *str)                            \
{                                                                                                                                                       \
    return EZBL_FIFOWrite(&peripheralName##_TxFifo, str, __builtin_strlen(str) - 1u); /* __builtin_strlen() evaluates at compile time for string literals and automatically falls back to a strlen() call when run-time evaluation is required. */  \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_Read(void *readData, unsigned int dataLen)     \
{                                                                                                                                                       \
    return EZBL_FIFORead(readData, &peripheralName##_TxFifo, dataLen);                                                                                  \
}                                                                                                                                                       \
static inline unsigned char __attribute__((always_inline, optimize(1)))         peripheralName##_TX_FIFO_Read8(void)                                    \
{                                                                                                                                                       \
    unsigned char readData;                                                                                                                             \
    EZBL_FIFORead(&readData, &peripheralName##_TxFifo, 1);                                                                                              \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_Read16(void)                                   \
{                                                                                                                                                       \
    unsigned short readData;                                                                                                                              \
    EZBL_FIFORead(&readData, &peripheralName##_TxFifo, 2);                                                                                              \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long __attribute__((always_inline, optimize(1)))         peripheralName##_TX_FIFO_Read24(void)                                   \
{                                                                                                                                                       \
    unsigned long readData;                                                                                                                             \
    EZBL_FIFORead(&readData, &peripheralName##_TxFifo, 3);                                                                                              \
    ((unsigned char *)&readData)[3] = 0x00;                                                                                                             \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long __attribute__((always_inline, optimize(1)))         peripheralName##_TX_FIFO_Read32(void)                                   \
{                                                                                                                                                       \
    unsigned long readData;                                                                                                                             \
    EZBL_FIFORead(&readData, &peripheralName##_TxFifo, 4);                                                                                              \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long long __attribute__((always_inline, optimize(1)))    peripheralName##_TX_FIFO_Read48(void)                                   \
{                                                                                                                                                       \
    unsigned long long readData;                                                                                                                        \
    EZBL_FIFORead(&readData, &peripheralName##_TxFifo, 6);                                                                                              \
    ((unsigned char *)&readData)[6] = 0x00;                                                                                                             \
    ((unsigned char *)&readData)[7] = 0x00;                                                                                                             \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long long __attribute__((always_inline, optimize(1)))    peripheralName##_TX_FIFO_Read64(void)                                   \
{                                                                                                                                                       \
    unsigned long long readData;                                                                                                                        \
    EZBL_FIFORead(&readData, &peripheralName##_TxFifo, 8);                                                                                              \
    return readData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_Peek(void *peekData, unsigned int dataLen)     \
{                                                                                                                                                       \
    return EZBL_FIFOPeek(peekData, &peripheralName##_TxFifo, dataLen);                                                                                  \
}                                                                                                                                                       \
static inline unsigned char __attribute__((always_inline, optimize(1)))         peripheralName##_TX_FIFO_Peek8(void)                                    \
{                                                                                                                                                       \
    unsigned char peekData;                                                                                                                             \
    EZBL_FIFOPeek(&peekData, &peripheralName##_TxFifo, 1);                                                                                              \
    return peekData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned int __attribute__((always_inline, optimize(1)))          peripheralName##_TX_FIFO_Peek16(void)                                   \
{                                                                                                                                                       \
    unsigned short peekData;                                                                                                                              \
    EZBL_FIFOPeek(&peekData, &peripheralName##_TxFifo, 2);                                                                                              \
    return peekData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long __attribute__((always_inline, optimize(1)))         peripheralName##_TX_FIFO_Peek24(void)                                   \
{                                                                                                                                                       \
    unsigned long peekData;                                                                                                                             \
    EZBL_FIFOPeek(&peekData, &peripheralName##_TxFifo, 3);                                                                                              \
    ((unsigned char *)&peekData)[3] = 0x00;                                                                                                             \
    return peekData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long __attribute__((always_inline, optimize(1)))         peripheralName##_TX_FIFO_Peek32(void)                                   \
{                                                                                                                                                       \
    unsigned long peekData;                                                                                                                             \
    EZBL_FIFOPeek(&peekData, &peripheralName##_TxFifo, 4);                                                                                              \
    return peekData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long long __attribute__((always_inline, optimize(1)))    peripheralName##_TX_FIFO_Peek48(void)                                   \
{                                                                                                                                                       \
    unsigned long long peekData;                                                                                                                        \
    EZBL_FIFOPeek(&peekData, &peripheralName##_TxFifo, 6);                                                                                              \
    ((unsigned char *)&peekData)[6] = 0x00;                                                                                                             \
    ((unsigned char *)&peekData)[7] = 0x00;                                                                                                             \
    return peekData;                                                                                                                                    \
}                                                                                                                                                       \
static inline unsigned long long __attribute__((always_inline, optimize(1)))    peripheralName##_TX_FIFO_Peek64(void)                                   \
{                                                                                                                                                       \
    unsigned long long peekData;                                                                                                                        \
    EZBL_FIFOPeek(&peekData, &peripheralName##_TxFifo, 8);                                                                                              \
    return peekData;                                                                                                                                    \
}


// Instance RX FIFO APIs from the above code for each of these peripherals
MAKE_FUNC_RX_FIFO(UART1)
MAKE_FUNC_RX_FIFO(UART2)
MAKE_FUNC_RX_FIFO(UART3)
MAKE_FUNC_RX_FIFO(UART4)
MAKE_FUNC_RX_FIFO(UART5)
MAKE_FUNC_RX_FIFO(UART6)
MAKE_FUNC_RX_FIFO(I2C1)
MAKE_FUNC_RX_FIFO(I2C2)
MAKE_FUNC_RX_FIFO(I2C3)

// Instance RX FIFO APIs from the above code for each of these peripherals
MAKE_FUNC_TX_FIFO(UART1)
MAKE_FUNC_TX_FIFO(UART2)
MAKE_FUNC_TX_FIFO(UART3)
MAKE_FUNC_TX_FIFO(UART4)
MAKE_FUNC_TX_FIFO(UART5)
MAKE_FUNC_TX_FIFO(UART6)
MAKE_FUNC_TX_FIFO(I2C1)
MAKE_FUNC_TX_FIFO(I2C2)
MAKE_FUNC_TX_FIFO(I2C3)




/**
 * Wrapper function for I2C_Reset() that pre-computes an appropriate I2CxBRG
 * value from a given clock speed and target bit rate.
 *
 * @param peripheralClockSpeed Frequency, in hertz, that the I2C peripheral is
 *                             operating with. On PIC24 and dsPIC products, this
 *                             is the same as the instructions executed/second
 *                             while not dozing. Ex: set to 70000000 if running
 *                             at 70 MIPS.
 *
 *                             If operating in slave mode, this parameter is
 *                             ignored.
 *
 * @param baudRate Desired baud rate for the I2C communications when operating
 *                 in master mode. Ex: set to 400000 for 400kHz (bits/second).
 *                 The actual baud rate programmed will be as close as possible
 *                 to the requested value, but could still result in appreciable
 *                 error if specifying very fast baud rates or when operating at
 *                 a slow peripheral clock speed. See the I2C chapter in the
 *                 device data sheet or Family Reference Manual documentation to
 *                 understand the underlying error limits.
 *
 *                 For slave mode, this parameter is ignored.
 *
 * @param slaveAddr I2C Slave address (7-bit or 10-bit integer) for this node.
 *
 *                 If there are no I2C bus masters that may talk to us in this
 *                 hardware design, this parameter can be set to any value, but
 *                 it should be chosen to avoid spurious events (ex: not all 1's
 *                 and not all 0's, which are both reserved addresses anyway).
 */
//void I2C_ResetCalc(unsigned long peripheralClockSpeed, unsigned long baudRate, unsigned int slaveAddr);


/**
 * Resets the hardware I2C peripheral and software FIFOs. All FIFO data, if any,
 * is discarded. The I2C is configured and enabled afterwards and ready for
 * communications/interrupts.
 *
 * This function does not initialize I2C I/O pins. Ensure port I/O and PPS
 * settings are set to a passive state and any multiplexed analog functions are
 * turned off so they don't interfere with the I2C hardware taking control of
 * the applicable SDA/SCL I/O pins.
 *
 * @param I2CxBRGVal Value to write to the I2CxBRG register to set the baud
 *                   clock for master-mode transmission. To have this value
 *                   automatically computed for a given clock speed and target
 *                   bit rate, call the I2C_Reset() wrapper function instead.
 *
 *                   If operating in slave mode, this parameter is ignored.
 *
 * @param slaveAddr I2C Slave address (7-bit or 10-bit integer) for this node.
 *
 *                 If there are no I2C bus masters that may talk to us in this
 *                 hardware design, this parameter can be set to any value, but
 *                 it should be chosen to avoid spurious events (ex: not all 1's
 *                 and not all 0's, which are both reserved addresses anyway).
 */
//void I2C_ResetBRG(unsigned int I2CxBRGVal, unsigned int slaveAddr);


/**
 * EZBL_FIFO * I2C_Reset(int peripheralNum, unsigned long peripheralClockSpeed, long baudRate, int slaveAddr, int useForStdio);
 *
 * Preferred macro to reset the hardware I2C and software FIFOs. All FIFO data,
 * if any, is discarded. The I2C is configured and enabled afterwards and ready
 * for communications/interrupts. Macro returns a pointer to the
 * just-initialized software RX FIFO (compiler throws this away if unused).
 *
 * This macro smartly chooses to call I2C_ResetBRG() or I2C_ResetCalc()
 * depending on compile-time ability to pre-compute the needed I2CxBRG value for
 * a given peripheral clock speed and desired I2C baud rate. The cheaper
 * I2C_ResetBRG() function is called if the baud-rate is 0 (normal for I2C
 * Slave-only operation), or both peripheralClockSpeed and baudRate are
 * constants at compile time.
 *
 * This function does not initialize I2C I/O pins. Ensure port I/O and PPS
 * settings are set to a passive state and any multiplexed analog functions are
 * turned off so they don't interfere with the I2C hardware taking control of
 * the applicable SDA/SCL I/O pins.
 *
 * @param peripheralNum I2C peripheral instance number to reset and initialize.
 *                      ezbl_lib.a supports the following values:
 *                      <ul><li>1 - I2C 1 Peripheral</li>
 *                          <li>2 - I2C 2 Peripheral</li>
 *                          <li>3 - I2C 3 Peripheral</li>
 *                      </ul>
 *
 * @param peripheralClockSpeed Frequency, in hertz, that the I2C peripheral is
 *                             operating with. On PIC24 and dsPIC products, this
 *                             is the same as the instructions executed/second
 *                             while not dozing. Ex: set to 70000000 if running
 *                             at 70 MIPS.
 *
 *                             When operating in slave mode only, this parameter
 *                             is ignored as the master generates the bus clock.
 *
 * @param baudRate Desired SCL clocking frequency for I2C Master mode
 *                 transmissions. The baudRate, in combination with the
 *                 peripheralClockSpeed input, are used to compute and assign
 *                 the required value for the I2CxBRG register.
 *
 *                 When operating in slave mode only, this parameter is ignored
 *                 as the master generates the bus clock.
 *
 *                 This is currently a reserved input as the EZBL I2C APIs do
 *                 not implement I2C Master mode read/write operations.
 *
 * @param slaveAddr I2C Slave address (7-bit or 10-bit integer) for this node.
 *
 *                 If there are no I2C bus masters that may talk to us in this
 *                 hardware design, this parameter can be set to any value, but
 *                 it should be chosen to avoid spurious events (ex: not all 1's
 *                 and not all 0's, which are both reserved I2C addresses).
 *
 *                 10-bit addressing is automatically selected if the slaveAddr
 *                 is too big to fit in 7 bits, regardless of if the address is
 *                 actually valid or not according to I2C specifications.
 *
 * @param useForStdio Flag that defines if the I2Cx_TxFifo should be the
 *                    destination of stdout writes and I2Cx_RxFifo should be the
 *                    source for stdin reads. Set to 1 if you wish to have
 *                    printf() and EZBL_printf() calls output on this
 *                    communications interface.
 *
 *                    If no communications FIFOs are initialized with this
 *                    parameter set to 1, printf() will exit through the
 *                    compiler's default, unbuffered UART2 or __C30_UART
 *                    interface.
 *
 * @return (EZBL_FIFO*) pointer to the RX FIFO associated with the
 *         just-initialized communications interface. This is equivalent to
 *         &I2C1_RxFifo or &I2C2_RxFifo, depending on the peripheralNum input
 *         parameter.
 */
#define I2C_Reset(peripheralNum, peripheralClockSpeed, baudRate, slaveAddr, useForStdio)                                \
({                                                                                                                      \
    __attribute__((always_inline, optimize(1))) EZBL_FIFO * I2C_ResetFunc(void)                                         \
    {                                                                                                                   \
        if(((peripheralNum) >= 1) && ((peripheralNum) <= 3))                                                            \
        {                                                                                                               \
            I2C##peripheralNum##_ResetBRG(0, slaveAddr);                                                                \
            if(useForStdio)                                                                                             \
            {                                                                                                           \
                EZBL_KeepSYM(EZBL_write);                                                                               \
                EZBL_KeepSYM(EZBL_read);                                                                                \
                EZBL_STDOUT = &I2C##peripheralNum##_TxFifo;                                                             \
                /* EZBL_STDERR = &I2C##peripheralNum##_TxFifo;  Not used in this version of EZBL. */                    \
                EZBL_STDIN  = &I2C##peripheralNum##_RxFifo;                                                             \
            }                                                                                                           \
            return &I2C##peripheralNum##_RxFifo;                                                                        \
        }                                                                                                               \
        else                                                                                                            \
        {                                                                                                               \
            __asm__(".error I2C" STRINGIFY(peripheralNum) " is not supported in this version of ezbl_lib or is an unrecognized token. \nOnly literal numbers 1 and 2 or preprocessor macros containing a literal number are allowed (no variables)");    \
            return (void*)0;                                                                                            \
        }                                                                                                               \
                                                                                                                        \
            /*if(__builtin_constant_p(baudRate) && ((baudRate) == 0))                                                 */\
            /*    I2C##peripheralNum##_ResetBRG(0, slaveAddr);                                                        */\
            /*else if(__builtin_constant_p(peripheralClockSpeed) && __builtin_constant_p(baudRate) && ((baudRate) != 0))*/\
            /*    I2C##peripheralNum##_ResetBRG((int)((unsigned int)(((double)(peripheralClockSpeed))/((double)(baudRate)) - ((double)(peripheralClockSpeed))/10e6 - 1)), slaveAddr); */ /* NOTE: Not real floating point. This is compile-time evaluated only, so is free. */ \
            /*else                                                                                                    */\
            /*    I2C##peripheralNum##_ResetCalc(peripheralClockSpeed, baudRate, slaveAddr);                          */\
    }                                                                                                                   \
    I2C_ResetFunc(); /* Calls inline function and provides GNU macro return value */                                    \
})


/**
 * Blocks execution until everything pending is finished being physically
 * transmitted. The software TX FIFO and the hardware I2C TX FIFO are drained
 * to 0 bytes before returning.
 */
//void I2C_TX_FIFO_WaitUntilFlushed(void);


/**
 * Enables interrupts used by the I2C FIFO TX and RX routines by setting the
 * associated IECx bits.
 */
//void I2C_FIFO_EnableInterrupts(void);


/**
 * Disables all interrupts used by the I2C FIFO routines by clearing the
 * associated IECx bits.
 */
//void I2C_FIFO_DisableInterrupts(void);


/**
 * Callback function automatically invoked anytime a write is made against the
 * I2C TX FIFO (with or without any data actually getting written).
 *
 * This function is called after the write has taken place and is
 * a convenient place to trigger necessary hardware actions, spy on the data
 * passing through, possibly teeing it to another communications interface, or
 * implement additional FIFO features, such as a blocking/almost guaranteed
 * write with timeout mechanism to simplify other communications code.
 *
 * @param bytesPushed Number of bytes that actually got written to the FIFO
 *                    before this callback was invoked.
 * @param *writeSrc Pointer to the memory originally used when the FIFO write
 *                  call was made. As no other code has executed yet, ordinary
 *                  RAM data sources can be re-read here to tee the data.
 * @param regWriteLen Number of bytes that were requested to be written when the
 *                    FIFO write call was made. Generally this will match the
 *                    bytesPushed value unless the TX FIFO is completely full.
 * @param *destFIFO Pointer to the EZBL_FIFO that called this callback function.
 *
 * @return Number of bytes that you want to return back to the EZBL_FIFOWrite()
 *         caller as reportedly being successfully written. Generally you should
 *         return bytesPushed unless you've taken some action to transfer more
 *         data (or perhaps stolen some back out of the FIFO).
 */
//unsigned int I2C_TX_FIFO_OnWrite(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *writeFIFO);// EZBL_FIFOWrite() OnWrite callback function


/**
 * Callback function automatically invoked anytime a software read attempt is
 * made against the RX FIFO, but before the underlying EZBL_FIFORead() call
 * returns data to the caller. Provides an early opportunity to block for
 * more data being available, spy on the data passing from the I2C hardware
 * into software, auto-generate flow control/status information for the remote
 * node, or potentially allows manipulation of returned data.
 *
 * This function is called at the same IPL context as the caller for
 * EZBL_FIFORead().
 *
 * @param bytesPulled Number of bytes extracted from the FIFO and being
 *                    presently returned to the caller of EZBL_FIFORead(). This
 *                    is the number of valid bytes present at *readDest.
 *
 * @param readDest Pointer to a caller allocated buffer where read data has been
 *                 written for the original call to EZBL_FIFORead(). This
 *                 pointer may be null if the caller is discarding data from the
 *                 FIFO without actually reading it.
 *
 * @param reqReadLen Number of bytes requested to be read when EZBL_FIFORead()
 *                   was called. This number will be < bytesPulled if the FIFO
 *                   did not have enough data to satisfy the original
 *                   EZBL_FIFORead() request. This value may be zero.
 *
 * @param readFIFO Pointer to the EZBL_FIFO that EZBL_FIFORead() was attempted
 *                 against. As the same callback handler could potentially be
 *                 used for multiple FIFOs, this pointer provides a context
 *                 specific handle back to the FIFO that called this callback
 *                 function.
 *
 * @return Specify the number of bytes of data now available at *readDest for
 *         the caller to consume. If you do not add or subtract any data from
 *         the stream within this callback, always return bytesPulled,
 *         unchanged.
 *
 *         This example callback implementation is non-blocking and will simply
 *         return with less than the requested number of bytes if a read is made
 *         for more than already exists in the FIFO.
 */
//unsigned int I2C_RX_FIFO_OnRead(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *readFIFO);   // EZBL_FIFORead() OnRead callback function


/******************* End of default UART & I2C FIFO APIs **********************/


#if defined(__XC16__)
/**
 * Measures an unknown-frequency clock source against another known-frequency
 * reference clock source and computes the unknown frequency.
 *
 * @param targetClockType
 *        <p>0x0 = 16-bit Timer/CCP/Counter in an SFR or RAM
 *                 location. *refClock will be dereferenced as a
 *                 volatile unsigned short to get "tick"
 *                 values. In the case of a RAM location, an
 *                 external ISR of higher priority than the present
 *                 CPU IPL must write the new values into the given
 *                 address as this function does not return until
 *                 the DCO measurements are complete.
 *        <p>0x1 = 32-bit Timer/CCP/Counter in an SFR or RAM.
 *                 *refClock will be dereferenced as a
 *                 volatile unsigned long to get "tick" values.
 *        <p>0x2 = 16-bit "tick" via function pointer return.
 *                 *refClock must point to an executable function
 *                 that takes no parameters and will return an
 *                 incrementing 16-bit integer type (unsigned int,
 *                 signed int, unsigned short, unsigned short int,
 *                 signed short, etc.). Generally, it is possible
 *                 to use this value even for 32-bit return valued
 *                 functions, but in such a case, the upper 16-bits
 *                 of information will always be discarded.
 *        <p>0x3 = 32-bit "tick" via function pointer return.
 *                 *refClock must point to an executable function
 *                 that takes no parameters and will return an
 *                 incrementing 16-bit integer type (unsigned int,
 *                 signed int, unsigned short, unsigned short int,
 *                 signed short, etc.).
 *
 * @param targetClock       Pointer to the specified clock resource. See
 *                          targetClockType parameter definition for exact
 *                          meaning of this pointer.
 *
 * @param refClockType      Same as targetClockType, but for the known-frequency
 *                          reference clock.
 *
 * @param refClock          Same as targetClock, but for the known-frequency
 *                          reference clock.
 *
 * @param refClockFrequency Frequency of the known-frequency refClock, in Hz.
 *
 * @param desiredPPMPrecision
 *                          Desired precision in measuring the target clock
 *                          frequency, in units of PPM (Parts Per Million).
 *                          1000 PPM is equivalent to +/-0.1% clock error
 *                          (1000/1000000 * 100%).
 *
 *                          Higher precision takes longer to measure,
 *                          potentially with a Gaussian tail, meaning a LOT
 *                          longer.
 *
 *                          NOTE: if either the reference, and to a lesser
 *                          extent, target clock frequency is unstable and
 *                          drifts faster than the desired precision can be
 *                          achieved, this function could block forever. To
 *                          avoid this, specify a timeout.
 *
 * @param milliTimeout      Maximum number of milliseconds allowed to try and
 *                          achieve the desiredPPMPrecision criteria. To disable
 *                          the timeout, specify 0.
 *
 * @return  Present frequency of the targetClock, in cycles-per-second (Hz). If an
 *          unrecognized parameter is given, 0xFFFFFFFF is returned.
 */
unsigned long NOW_MeasureAbsoluteClock(unsigned int targetClockType, void *targetClock, unsigned int refClockType, void *refClock, unsigned long refClockFrequency, unsigned int desiredPPMPrecision, unsigned int milliTimeout);


/**
 * Converts the difference between two NOW counts to absolute time in
 * milliseconds. This function is primarily intended for instances where you
 * wish to display a time interval to a human in decimal forms. It is, however,
 * not subject to the integral approximations that NOW_ms represent, so
 * can be used in other cases when absolute accuracy is critical and the device
 * operating frequency is very low (ex: 32.768kHz).
 *
 * nowCounts must be measured against the same clock frequency in use during
 * invocation of this function. In other words, a wrong result will be returned
 * if your collect two NOW counts at one clock frequency, do a run-time clock
 * switch, call NOW_SetTimeInterval(), and then pass the difference of the
 * original counts in for the nowCounts parameter.
 *
 * This function can be called from any CPU IPL and is reentrant capable.
 *
 * This function requires two 32x32 unsigned integer divide operations and
 * therefore requires a fair amount of execution time relative to most other
 * NOW APIs. If you just need to compare two timestamps where one value
 * represents the present and another value represents a constant timeout, it is
 * likely more efficient to use the NOW_sec and NOW_ms variables.
 *
 * @param nowCounts The number of NOW counts to convert. Acceptable range is 0
 *                  to 536,870,911 counts (or up to 2,684 ms @ 200MHz; longer
 *                  for slower clock frequencies). Specifying
 *                  nowCounts > 536,870,911 will return an undefined value. The
 *                  function will safely succeed, however.
 *
 * @return 32-bit unsigned long representing complete milliseconds elapsed.
 *         Rounding is not performed; partial milliseconds are truncated off.
 */
unsigned long NOW_Diff_to_ms(unsigned long nowCounts);


/**
 * Converts a time difference in NOW counts to absolute time in microseconds.
 *
 * The time difference in NOW counts should be obtained by subtracting the
 * return value of NOW_16()/NOW_32()/NOW_64() from a prior value returned from
 * the same function.
 *
 * Use this when you wish to display a time interval in human decimal form.
 * This function is subject to less precision loss, as compared to dividing the
 * time difference by NOW_us, when the NOW timebase is only a few MHz or slower.
 *
 * This function can be called from any CPU IPL and is reentrant capable.
 *
 * This function requires one 32x16 unsigned integer hardware divide plus one
 * 32x32 unsigned software integer divide operation and therefore typically
 * requires a few microseconds of execution time.
 *
 * @param nowCounts
 *          The number NOW counts to convert. Supported range is 0 to 67,108,863
 *          counts (up to 335.544ms @ 200MHz; 958.697ms @ 70 MHz; > 4 seconds @
 *          16 MHz). Specifying > 67,108,863 will return an undefined value, but
 *          safely succeed.
 *
 * @return 32-bit unsigned long representing complete microseconds elapsed.
 *         Rounding is not performed; partial microseconds are truncated off.
 *
 *         To generate a rounded result, add (NOW_us/2u) to the nowCounts
 *         parameter before calling this conversion function. This is equivalent
 *         to adding 0.5 to the internal result, which when represented as an
 *         integer, results in fractionals of >= 0.5us being increased to +1us
 *         and fractions of < 0.5 being truncated down to 0.
 */
unsigned long NOW_Diff_to_us(unsigned long nowCounts);


/**
 * Atomically returns the least significant 16 bits of the current NOW counter
 * value. This function is safe to be called from all ISRs (and main context),
 * and at any time. This function is reentrant safe.
 *
 * Execution time is 4 to 7 cycles on 16-bit PIC24 or dsPIC processors,
 * depending on device family. There are no call or return branches as this is
 * implemented as a static always inline function requiring 4 or 5 instructions.
 *
 * @return 16-bit NOW counter value, where 1 NOW count is equal to 1 clock cycle
 *         of the reference timer input. Use the NOW_sec, NOW_ms,
 *         NOW_us variable contents, or the NOW_Diff_to_ms() and
 *         NOW_Diff_to_us() functions in order to make physical sense of how
 *         long a NOW count is at run time.
 *
 *         Because this return value is only 16-bits wide and could increment as
 *         at up to the system device frequency, some clock sources will
 *         overflow 16-bits very easily. Ex: at 70MIPS and the CPU clock used
 *         for the NOW timer, the longest interval you could correctly measure
 *         with this function is 936us. Use the NOW_32() or NOW_64() functions
 *         if longer durations are being measured.
 */
static inline unsigned short __attribute__((always_inline, optimize(1))) NOW_16(void)
{
    extern volatile unsigned int *NOW_timerSFRPtr  XC16_NEAR __attribute__((weak, persistent));
    extern volatile unsigned int NOW_timeOffset __attribute__((weak, persistent));

    return *NOW_timerSFRPtr + NOW_timeOffset;
}
#elif defined(__XC32__) || defined(__PIC32__)
unsigned short NOW_16(void);
#endif


/**
 * Atomically returns the least significant 32 bits of the current NOW counter
 * value.
 *
 * This function is safe to be called from any IPL or ISRs with the return value
 * still being valid (i.e. reentrancy is allowed). When called from a higher
 * priority ISR or trap handler which blocks the _TxInterrupt() ISR from
 * executing, this function will automatically revert to a polling mode where
 * the _TxInterrupt() will be called as needed to ensure proper carry
 * propgataion into the MSbits from the hardware timer. This ensures that all
 * calls to NOW_32() will always be chronologically monotonic. I.e. this
 * function should never return a count that represents an earlier time than any
 * previous calls to NOW_32() or NOW_64() made within the last 2^32 counts (or
 * 2^64 counts for NOW_64()).
 *
 * Execution time is a fixed 21 or 27 cycles best (and normal) case, including
 * the call and return branches, depending on device. However, if you are very
 * unlucky and the lower 16-bits rolls over during the read (less than 0.01%
 * chance if no interrupt occurs), execution time will grow for a bounded, but
 * non-deterministic time.
 *
 * Both NOW_64() and NOW_32() are implemented as the same function with the C
 * compiler ignoring the upper 32-bits when NOW_32() is called instead of
 * NOW_64().
 *
 * @return 32-bit NOW counter value, where 1 NOW count is normally equal
 *         to 1 instruction cycle. Use the NOW_sec, NOW_ms,
 *         NOW_us variable contents, or the NOW_Diff_to_ms() and
 *         NOW_Diff_to_us() functions in order to make physical sense of how
 *         long a NOW count is at run time.
 *
 *         NOTE: the NOW_32() function overflows on the timescale of ~minute
 *         Ex: at 70MIPS, the longest interval you could correctly measure is
 *         61.356 seconds. Use the NOW_64() function if longer durations are
 *         being measured.
 */
unsigned long NOW_32(void);


/**
 * Atomically returns the full 64-bits of the NOW counter.
 *
 * This function is safe to be called from any IPL or ISRs with the return value
 * still being valid (i.e. reentrancy is allowed). When called from a higher
 * priority ISR or trap handler which blocks the _TxInterrupt() ISR from
 * executing, this function will automatically revert to a polling mode where
 * the _TxInterrupt() will be called as needed to ensure proper carry
 * propagation into high order bits. This ensures that all calls to
 * NOW_32()/NOW_64() will always be chronologically monotonic. I.e. this
 * function should never return a count that represents an earlier time than any
 * previous calls to NOW_32()/NOW_64() made within the last 2^32 counts (or 2^64
 * counts for NOW_64())
 *
 * Execution time is a fixed 21 or 27 cycles best (and normal) case, including
 * the call and return branches, depending on device. However, if you are very
 * unlucky and the lower 16-bits rolls over during the read (less than 0.01%
 * chance if no interrupt occurs), execution time will grow for a bounded, but
 * non-deterministic time.
 *
 * Both NOW_64() and NOW_32() are implemented as the same function with the C
 * compiler ignoring the upper 32-bits when NOW_32() is called instead of
 * NOW_64().
 *
 * @return 64-bit NOW counter value, where 1 NOW count is normally equal
 *         to 1 instruction cycle. Use the NOW_sec, NOW_ms,
 *         NOW_us variable contents, or the NOW_Diff_to_ms() and
 *         NOW_Diff_to_us() functions in order to make physical sense of how
 *         long a NOW count is at run time.
 *
 *         NOTE: because manipulation of 64-bit long long's requires more code,
 *         consider using the NOW_32() function anytime the duration or interval
 *         being measured is known to not exceed the bounds of a 32-bit NOW
 *         counter.
 */
unsigned long long NOW_64(void);


/**
 * Updates the NOW_sec (NOW_sec), NOW_ms (NOW_millisecond) and NOW_us
 * (NOW_microsecond) variables to correspond to the specified timer input clock
 * frequency.
 *
 * @param timerInputFrequency
 *          Number of timer input clock cycles per second. For example, use
 *          70000000 if your device is operating at 70 MIPS and using the system
 *          clock to clock the timer.
 */
void NOW_SetTimeIntervals(unsigned long timerInputFrequency);


/**
 * Macro updates the NOW_sec (NOW_second), NOW_ms (NOW_millisecond) and NOW_us
 * (NOW_microsecond) variables to correspond to the specified timer input clock
 *
 * This macro devolves to an inline computation and assignment of the
 * NOW_sec/NOW_ms/NOW_us values if timerInputFrequency is specified as a
 * compile-time constant or macro value. Otherwise, the NOW_SetTimeIntervals()
 * function is called to compute these values at run-time using a couple of
 * 32 x 32-bit software divides.
 *
 * @param timerInputFrequency
 *          Number of timer input clock cycles per second. For example, use
 *          70000000 if your device is operating at 70 MIPS and using the system
 *          clock to clock the timer.
 */
#define NOW_SetTimeIntervals(timerInputFrequency)                               \
    {                                                                           \
        if(__builtin_constant_p(timerInputFrequency))                           \
        {                                                                       \
            NOW_sec = (unsigned long)(timerInputFrequency);                     \
            NOW_ms  = (((unsigned long)(timerInputFrequency))+500u)/1000u;                          /* (500/1000) is addition of 0.5ms. I.e. rounds up or down to nearest millisecond count with least error magnitude. */          \
            NOW_us  = (unsigned short)((((unsigned long)(timerInputFrequency))+500000u)/1000000u);  /* (500000/1000000) is addition of 0.5us. I.e. rounds up or down to nearest microsecond count with least error magnitude. */    \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            NOW_SetTimeIntervals((unsigned long)(timerInputFrequency));         \
        }                                                                       \
    }


/**
 * Waits the specified number of NOW counts in a blocking loop doing nothing.
 *
 * On 16-bit devices, delays longer than 65535 NOW counts (~0.936ms @ 70 MIPS,
 * 4.096ms @ 16 MIPS) will spend the time in Idle() power saving mode until
 * <= 65535 NOW counts remain.
 *
 * @param waitTime Number of NOW counts to wait. Use any fraction or multiple
 *                 of NOW_sec, NOW_ms, NOW_us variables to specify absolute
 *                 human time frames.
 *
 * @return NOW_32() return value (the present time).
 */
unsigned long NOW_Wait(unsigned long waitTime);


#if defined(__XC16__)
/**
 * Adds, updates, or deletes a timer task which automatically executes a
 * function after the specified time elapses. The timer can be a one-shot event
 * or an automatically self-repeating timer.
 *
 * @param timerContext A pointer to a caller allocated NOW_TASK
 *          structure. If the pointer matches one previously given to
 *          NOW_CreateTask(), but for which the task has not expired yet, this
 *          API will update the existing context structure with the latest
 *          callbackFunction pointer and timeout parameters, effectively
 *          replacing the existing timer.
 *
 *          NOTE: the pointed to memory MUST stay resident until the callback is
 *          executed (for one-shot mode). If the timer is a self-repeating type,
 *          the pointed to memory MUST stay resident at all times. Normally,
 *          the timerContext structure should be allocated as static or at
 *          global scope to ensure this requirement is met.
 *
 * @param callbackFunction Pointer to a callback function to execute upon timer
 *          expiration. The callback function should take (void) parameters and
 *          return an (int).
 *
 *          The return value does nothing inside the NOW APIs, except get stored
 *          in the timerContext structure for retrieval by the main() loop or
 *          other tasks by calling NOW_GetTaskReturnCode(). Due to the 
 *          asynchronous nature of the task, the return code must be treated as
 *          a "lossy" communications method much like how two reads from a PORTx
 *          register can completely miss a short voltage spike on an I/O pin
 *          that falls in between the two read events.
 *
 * @param timeout
 *          >= 1: The number of NOW counts to wait until expiring a one-shot
 *                timer and executing the callback. This value is relative to
 *                the present time, so '1' always means: execute the callback
 *                right away, not when NOW_64()'s return value rolls past the
 *                value 0x1. Using a value of 'NOW_sec' means: execute the
 *                callback one second in the future.
 *
 *                In this one-shot timer mode, the timerContext memory will be
 *                freed immediately prior to calling the callbackFunction. It is
 *                therefore legal to restart another timer (possibly for
 *                the same callback function) from within the callback code and
 *                using the same timerContext memory.
 *
 *          == 0: Any existing timer task at the timerContext memory is ended.
 *                The timerContext memory is freed and can be reused for other
 *                purposes after this NOW_CreateTimer() API returns. The
 *                callbackFunction parameter is ignored.
 *
 *                If you wish to create a timer that executes immediately,
 *                specify timeout as '1' instead.
 *
 *          < 0:  Creates a self-repeating timer for executing the callback. The
 *                value specified is negated (2's complemented) in order to set
 *                the repeat inteveral, in NOW counts. For example, to create a
 *                periodic callback event every 100 milliseconds, and being
 *                called for the first time 100 milliseconds from now, specify:
 *                -(100u*NOW_ms).
 *
 * @return NOW_TimerCallbackHead/downstream timerContext linked list is updated.
 */
void NOW_CreateTask(NOW_TASK *timerContext, int (*callbackFunction)(void), long long timeout);


/**
 * void NOW_CreateRepeatingTask(NOW_TASK nowTaskPtr, int (*taskFuncPtr)(void), unsigned long long NOW_repeat_interval);
 *
 * Macro wrapper to automatically convert the NOW_repeat_interval, specified as
 * a char, int, long, or long long or its unsigned equivalents into the negative
 * signed long long value expected by NOW_CreateTask().
 *
 * Express NOW_repeat_interval as a positive quantity when using this macro.
 *
 * @param NOW_TASK_ptr
 *      Pointer to a caller allocated NOW_TASK structure. If the pointer matches
 *      one previously given to NOW_CreateTask()/NOW_CreateRepeatingTask(), this
 *      API will update the existing context structure with the latest
 *      callbackFunction pointer and timeout parameters, effectively replacing
 *      the existing task.
 *
 *      NOTE: the pointed to memory must stay resident at all times the
 *      repeating task is enabled. The backing NOW_TASK memory should therefore
 *      be declared static, with global scope, or otherwise not auto-allocated
 *      on the stack for a function that returns.
 *
 * @param taskFunc
 *      Pointer to a callback function to execute upon expiration of the task
 *      repeat interval. The callback function should take (void) parameters and
 *      return an (int), i.e., have the prototype of:
 *          int callbackFunction(void);
 *
 *      NOTE: to get a pointer to a function in C, simply specify the function's
 *      name (without any parenthesis!). Ex:
 *          NOW_CreateRepeatingTask(&task, callbackFunction, NOW_sec);
 *      If you mistakenly include parenthesis the callbackFunction() will be
 *      called and its return value will be used as the function pointer
 *      (causing a run-time error).
 *
 *      The callback's return value does nothing inside the NOW APIs, except get
 *      stored in the timerContext structure for retrieval by the main() loop or
 *      other tasks by calling NOW_GetTaskReturnCode(). Due to the asynchronous
 *      nature of the task, the return code must be treated as a "lossy"
 *      communications method much like how two reads from a PORTx register can
 *      completely miss a short voltage spike on an I/O pin that falls in
 *      between the two read events.
 *
 *      When creating the task (i.e. it has never executed to generate a return
 *      code), the return code is initialized to 0x8000 (-32768).
 *
 * @param repeat_interval
 *      Number of NOW counts between successive calls to the callbackFunction.
 *      Specify this using some combination of NOW_sec, NOW_ms, or NOW_us
 *      values, ex:
 *          NOW_ms*8u + NOW_us*333ul
 *      This will result in an 8.333ms approximate repeat period.
 *
 *      This parameter is casted to an unsigned short int, unsigned long, or
 *      unsigned long long with a maximum legal value of 2^48 - 1 NOW counts.
 *
 *      The repeat interval is an approximation that attempts to maintain long
 *      term average execution repeat accuracy, but which may be off by up to
 *      the NOW timer's ISR repeat interval (typically 65535 instruction cycles,
 *      or 4.096ms @ 16 MIPS and 0.9362ms @ 70 MIPS) for any single task call.
 *      As an example, if a task is designed to toggle an I/O pin every 8.333ms
 *      (60Hz sine wave zero crossings), specifying a repeat interval of
 *      (8ul*NOW_ms + 333ul*NOW_us) will maintain an average task frequency of
 *      120.05 Hz, but the task callback execution start times, in milliseconds
 *      for a series of calls using a 70 MHz NOW timer may be:
 *             Ideal         Actual    Delta    Absolute
 *          Callback      Execution    Error       Error
 *             0.000          0.000    0.000	   0.000
 *             8.333          8.426   -0.093      -0.093
 *            16.667         16.852   -0.093      -0.185
 *            25.000         25.278   -0.093      -0.278
 *            33.333         33.704   -0.093      -0.371
 *            41.667         42.130   -0.093      -0.464
 *            50.000         50.556   -0.093      -0.556
 *            58.333         58.982   -0.093      -0.649
 *            66.667         67.408   -0.093      -0.742
 *            75.000         75.835   -0.093      -0.835
 *            83.333         84.261   -0.093      -0.927
 *            91.667         91.750   +0.843      -0.084
 *           100.000        100.176   -0.093      -0.176
 *      Absolute timing error accumulates each period until enough exists to
 *      adjust by a full 65536 NOW counts, or one NOW timer interrupt interval.
 *
 *      Delta error will also increase with other concurrent interrupts and code
 *      executing within main(), resulting in less precise absolute error, but
 *      still maintaining average absolute error bounded by the 65536 NOW
 *      counts.
 *
 * @return void. The task executes for the first time after one repeat_interval elapses.
 */
#define NOW_CreateRepeatingTask(NOW_TASK_ptr, taskFunc, repeat_interval)                                                \
    { /* Note: C compiler evaluates this at compile time; it is not a costly mess like it appears */                    \
        typeof(repeat_interval) timeout = (repeat_interval);                                                            \
                                                                                                                        \
        if (__builtin_types_compatible_p (typeof(timeout), signed char))                                                \
            NOW_CreateTask(NOW_TASK_ptr, taskFunc, (long long)-((unsigned long long)(unsigned int)timeout));   \
        else if (__builtin_types_compatible_p (typeof(timeout), signed short))                                          \
            NOW_CreateTask(NOW_TASK_ptr, taskFunc, (long long)-((unsigned long long)(unsigned int)timeout));   \
        else if (__builtin_types_compatible_p (typeof(timeout), signed long))                                           \
            NOW_CreateTask(NOW_TASK_ptr, taskFunc, (long long)-((unsigned long long)(unsigned long)timeout));  \
        else                                                                                                            \
            NOW_CreateTask(NOW_TASK_ptr, taskFunc, (long long)-((unsigned long long)timeout));                 \
    }


/**
 * Deletes a specified task from being called again. Works on both one-shot
 * tasks and repeating ones.
 *
 * @param taskToStopCalling Pointer to the task to stop calling.
 * @return 0 if the task was not queued to be called.
 *         1 if the task was successfully deleted.
 */
int NOW_EndTask(NOW_TASK *taskToStopCalling);



extern void XC16_NEAR __attribute__((persistent)) *NOW_timerCallbackHead;           // Private variable for NOW_*() APIs. Do not reference directly.
extern volatile char XC16_NEAR __attribute__((persistent)) NOW_dispatcherActive;    // Private variable for NOW_*() APIs. Do not reference directly.

/**
 * Terminates all NOW tasks currently queued for execution, including repeating
 * tasks. The NOW timer ISR will no longer issue any background callbacks when
 * the timer interrupt occurs nor when a call to NOW_32()/NOW_64() is made from
 * IPL 0 with the NOW timer interrupt disabled.
 */
static inline void __attribute__((always_inline, optimize(1))) NOW_EndAllTasks(void)
{
    NOW_timerCallbackHead = (void*)0;
    NOW_dispatcherActive = 0;
}


/**
 * Schedules a task for execution at a earlier/later time than presently
 * scheduled.
 *
 * When the task is executed, the repeat interval will be added to the time at
 * execution to compute when the task will be run again (assuming the task
 * repeats). Therefore, this function will pull-in/push-out the whole train of
 * subsequent executions without affecting the repeat period for anything but
 * the present cycle.
 *
 * @param task A pointer to a statically or globally allocated NOW_TASK
 *             structure to modify.
 *
 * @param timeFromNow Number of now counts into the future when the task should
 *                    be executed next. Use NOW_sec, NOW_ms, and
 *                    NOW_us to choose this.
 * 
 *                    timeFromNow is subject to a minimum granularity of + 1 NOW 
 *                    ISR interval. For example, on a machine running at 70 
 *                    MIPS, the 16-bit NOW Timer/CCT ISR will trigger every
 *                    65536 instruction cycles, or ~936us. Therefore, a
 *                    timeFromNow of 1*NOW_ms would actually result in a true
 *                    next execution time of 1.000ms to 1.936ms, depending upon
 *                    when the Timer/CCT ISR last executed. A timeFromNow of 0
 *                    would cause the task to execute sometime between 0 and
 *                    0.936ms into the future.
 *
 *
 * @return Number of now counts pulled in (pushed out) from the original task's
 *         scheduled execution time. Time pulled in is represented as a positive
 *         number while negative numbers represent a pushed out time.
 */
unsigned long NOW_SetNextTaskTime(NOW_TASK *task, unsigned long timeFromNow);


/**
 * Inline function returns the most recent return result for the given task, if
 * any. The NOW task scheduler only saves the first 16-bits of return data from
 * the callback function (passed via the w0 register), so it is not possible to
 * retrieve any other return data that might be bigger.
 *
 * @return Lower 16-bits of data returned from the most recent call to the
 *         task's callback function. The default for functions that haven't
 *         executed yet is 0x8000 (-32768).
 */
static inline int __attribute__((always_inline, optimize(1))) NOW_GetTaskReturnCode(NOW_TASK *taskToReadReturnValue)
{
    return taskToReadReturnValue->returnSignal;
}


/**
 * Inline function sets up a NOW timer to call the specified function pointer
 * one time after the specified timeout elapses.
 *
 * To periodically call a function use NOW_CreateRepeatingTask() instead.
 *
 * NOTE: This function should be used with caution when used in a loop. Each
 * time this macro is referenced, a local static structure is created at the
 * site of the inline code, which gets added to a linked list. Calling it from a
 * loop will result in the timeout and function pointer being updated, which
 * will overwrite the previous one (not create another timer task).
 *
 * @param callbackFunction See NOW_CreateTask() documentation.
 * @param timeout See NOW_CreateTask() documentation.
 *
 * @return
 *      Pointer to the static NOW_TASK structure that was created for the task.
 *      If the task ever needs to be stopped or changed outside the callback
 *      function, this pointer should be saved for later use with other NOW
 *      APIs.
 */
static inline NOW_TASK * __attribute__((always_inline)) NOW_CreateOneTimeTask(int (*callbackFunction)(void), unsigned long long timeout)
{
    NOW_TASK *oneShotTasks;
    oneShotTasks = EZBL_AllocSRAM(oneShotTasks, 20, persist, preserved);
    NOW_CreateTask(oneShotTasks, callbackFunction, (signed long long)timeout);
    return oneShotTasks;
}
#elif defined(__XC32__) || defined(__PIC32__)
#endif



/**
 * Alias to NOW_sec implemented for backwards compatibility. See NOW_sec
 * documentation for more information.
 *
 * Number of NOW counts per 1 second of physical time (assuming the NOW_Reset()
 * macro was called to initialize the NOW timer).
 */
extern unsigned long NOW_second XC16_NEAR __attribute__((persistent));


/**
 * Number of NOW counts per second of physical time (assuming the NOW_Reset()
 * macro was called to initialize the NOW timer).
 *
 * If run time clock switching occurs externally and you are using the system or
 * peripheral clock for the timing reference to NOW, you must call the
 * NOW_SetTimeIntervals() function to recompute this variable. Any NOW APIs
 * which include clock switching internally do not carry this requirement since
 * they will automatically update NOW_us, NOW_ms and NOW_sec internally.
 *
 * This variable is typically the same value as the device clock frequency, so
 * will results in no rounding or truncation error.
 *
 * Note that this is an unsigned long value. You must up-cast this to an
 * (unsigned long long) if you are running at a high clock frequency and want to
 * compute an interval that would overflow a 32-bit integer. For example, if the
 * device clock is 70.0MHz, NOW_sec would be 70,000,000. If you want to
 * compute something that is two minutes (120 seconds), the correct number of
 * NOW counts would be 8,400,000,000, which can't fit in a 32-bit variable. For
 * this clock speed, any calculation (including intermediate ones) requiring 
 * more than 61.356 seconds will be invalid without an initial cast to 64-bits.
 * The NOW_64() API would typically also be needed when working with long
 * intervals.
 */
extern unsigned long NOW_sec XC16_NEAR __attribute__((persistent));


/**
 * Alias to NOW_ms implemented for backwards compatibility. See NOW_ms
 * documentation for more information.
 *
 * Number of NOW counts per millisecond of physical time (0.001 seconds,
 * assuming the NOW_Reset() macro was called to initialize the NOW timer).
 */
extern unsigned long NOW_millisecond XC16_NEAR __attribute__((persistent));


/**
 * Number of NOW counts per millisecond of physical time (0.001 seconds,
 * assuming the NOW_Reset() macro was called to initialize the NOW timer).
 *
 * If run time clock switching occurs externally and you are using the system or
 * peripheral clock for the timing reference to NOW, you must call the
 * NOW_SetTimeIntervals() function to recompute this variable. Any NOW APIs
 * which include clock switching internally do not carry this requirement since
 * they will automatically update NOW_us, NOW_ms, and NOW_sec for you.
 *
 * Note that this variable is an integral number, rounded to result in the least
 * amount of error, either positive or negative. Ex: if the clock
 * frequency is 32.768kHz, one NOW count would represent 1/32768Hz or
 * 0.030517578125 milliseconds. Therefore, NOW_ms, the number of NOW
 * counts/millisecond, would be 32.768 if precision were infinite. Since
 * rounding instead of truncation occurs, this will result in NOW_ms
 * returning 33 instead of 32 (-0.708% error versus +2.344% error).
 */
extern unsigned long NOW_ms XC16_NEAR __attribute__((persistent));


/**
 * Alias to NOW_us implemented for backwards compatibility. See NOW_us
 * documentation for more information.
 *
 * Number of NOW counts per microsecond of physical time (0.000001 seconds,
 * assuming the NOW_Reset() macro is called with a suitably fast timer clock
 * to provide microsecond scale resolution).
 */
extern unsigned short NOW_microsecond XC16_NEAR __attribute__((persistent));


/**
 * Number of NOW counts per microsecond of physical time (0.000001 seconds,
 * assuming the NOW_Reset() macro is called with a suitably fast timer clock
 * to provide microsecond scale resolution).
 *
 * NOTE: Unlike NOW_sec and NOW_ms, this value is a 16-bit unsigned short. When
 * attempting to measure or time a longer interval with a fast timer base
 * frequency (ex: 937u*NOW_us @ 70 MIPS), ensure that final/intermediate
 * arithmetic is applied using an up-casted (unsigned long) NOW_us value to
 * avoid overflow. Ex: (937ul*NOW_us).
 *
 * 31/32kHz LPRC and 32.768kHz SOSC clocks have a period of over 30us, so DO NOT
 * USE NOW_us with such clocks. The value would always be zero. Use a NOW_ms or
 * NOW_sec fractional value instead.
 *
 * If run time clock switching occurs externally and you are using the system or
 * peripheral clock for the timing reference to NOW, you must call the
 * NOW_SetTimeIntervals() function to recompute this variable. Any NOW APIs
 * which include clock switching internally do not have this requirement as
 * they will automatically update NOW_us, NOW_ms and NOW_sec internally.
 *
 * Note that this variable is an integral number, rounded to result in the least
 * amount of error, either positive or negative. Ex: if the clock
 * frequency is 1.500001MHz, one NOW count would represent 1/1500001Hz or 0.6667
 * microseconds. Therefore, NOW_us, the number of NOW
 * counts/microsecond, would be 1.500001 if precision were infinite. Since
 * rounding instead of truncation occurs, this will result in NOW_us
 * returning 2 instead of 1 (-33.33324% error instead of +33.33338%).
 *
 * Be very cautious using this value if your clock frequency is under 500kHz. In
 * this case, 0 will be returned, which could cause a divide by zero exception
 * when divided or 100% error condition when multiplied.
 */
extern unsigned short NOW_us XC16_NEAR __attribute__((persistent));


/**
 * Alias to NOW_Fcy implemented for backwards compatibility.
 *
 * System instruction clock frequency, in Hz. Normally this will match
 * NOW_sec, but this value could differ if the NOW Timer clock source is
 * different from the system execution clock.
 *
 * This value is set when the NOW_Reset() macro is called, however, you may
 * write to this variable if a different value is needed as the NOW_*() APIs do
 * not directly use this value.
 */
extern unsigned long NOW_systemFrequency __attribute__((persistent));


/**
 * System instruction clock frequency, in Hz. Normally this will match
 * NOW_sec, but this value could differ if the NOW Timer clock source is
 * different from the system execution clock. 
 * 
 * This value is set when the NOW_Reset() macro is called, however, you may
 * write to this variable if a different value is needed as the NOW_*() APIs do
 * not directly use this value.
 */
extern unsigned long NOW_Fcy __attribute__((persistent));


/**
 * Legacy API equivalent to:
 *      XC16 projects: NOW_Reset(TMR1, timerInputFrequency);
 *      XC32 projects: NOW_Reset(CORETMR, timerInputFrequency);
 *
 * @param timerInputFrequency
 *          Number of input timer clock cycles per second of real time. For
 *          example, use 70000000 if your device is operating at 70 MIPS and you
 *          are using the system instruction clock to clock the timer. LPRC's
 *          will need ~31000 for 31kHz, and of course 32.768kHz SOSC crystals
 *          are also normally used with 32768.
 *
 *          However, if you know your crystal or clock input is biased above or
 *          below the optimal crystal spec, you can deliberately provide a value
 *          slightly faster or slower to automatically correct for this
 *          oscillation frequency error in software computations. At room
 *          temperature, crystals will oscillate slower than intended when you
 *          have oversized crystal loading capacitors (ex: more parasitic pin
 *          capacitance than you calculated for), and similarly, run fast when
 *          there is below optimal capacitance.
 */
void NOW_Init(unsigned long timerInputFrequency);


#if defined(__XC16__)
/**
 * Initializes a Timer for time keeping with the NOW_* API. Timing is
 * implemented relative to the timer's selected input clock frequency. This
 * function uses this value to set the values stored in NOW_sec, NOW_ms and
 * NOW_us for direct use in calling code to translate run-time adjustable NOW
 * counts to physical seconds and other human meaningful times.
 *
 * This function defaults to enabling the Timer/CCP Interrupt at priority
 * level 4 on 16-bit devices and no interrupt on PIC32MM devices which use the 
 * MIPS Core Timer. On 16-bit devices, the interrupt will trigger every 65536
 * instructions.
 *
 *
 * @param FCYClock
 *          Number of input timer clock cycles per second of real time. For
 *          example, use 70000000 if your device is operating at 70 MIPS and you
 *          are using the system instruction clock to clock the timer. LPRC's
 *          will need ~31000 for 31kHz, and of course 32.768kHz SOSC crystals
 *          are also normally used with 32768.
 *
 *          However, if you know your crystal or clock input is biased above or
 *          below the optimal crystal spec, you can deliberately provide a value
 *          slightly faster or slower to automatically correct for this
 *          oscillation frequency error in software computations. At room
 *          temperature, crystals will oscillate slower than intended when you
 *          have oversized crystal loading capacitors (ex: more parasitic pin
 *          capacitance than you calculated for), and similarly, run fast when
 *          there is below optimal capacitance.
 */
void NOW_TMR1_ResetCalc(unsigned long FCYClock);
void NOW_TMR2_ResetCalc(unsigned long FCYClock);
void NOW_TMR3_ResetCalc(unsigned long FCYClock);
void NOW_TMR4_ResetCalc(unsigned long FCYClock);
void NOW_TMR5_ResetCalc(unsigned long FCYClock);
void NOW_TMR6_ResetCalc(unsigned long FCYClock);
void NOW_CCP1_ResetCalc(unsigned long FCYClock);
void NOW_CCP2_ResetCalc(unsigned long FCYClock);
void NOW_CCP3_ResetCalc(unsigned long FCYClock);
void NOW_CCP4_ResetCalc(unsigned long FCYClock);
void NOW_CCP5_ResetCalc(unsigned long FCYClock);
void NOW_CCP6_ResetCalc(unsigned long FCYClock);
void NOW_CCP7_ResetCalc(unsigned long FCYClock);
void NOW_CCP8_ResetCalc(unsigned long FCYClock);
#elif defined(__PIC32__) || defined(__XC32__)
void NOW_CORETMR_ResetCalc(unsigned long FCYClock);
#endif


#if defined(__XC16__)
/**
 * Initializes a Timer for time keeping with the NOW_* API. Timing is
 * implemented relative to the timer's selected input clock frequency. This
 * function uses this value to set the values stored in NOW_sec,
 * NOW_ms and NOW_us for direct use in calling code to
 * translate run-time adjustable NOW counts to physical seconds and other human
 * meaningful times.
 *
 * This function defaults to enabling the Timer/CCP Interrupt at priority
 * level 4 on 16-bit devices and no interrupt on PIC32MM devices which use the
 * MIPS Core Timer. On 16-bit devices, the interrupt will fire every 65536
 * instructions and need around ~15 or 30 cycles per interrupt event on 16-bit
 * processors. i.e. less than 0.05% CPU use.
 *
 * Most of the NOW APIs should only be called at IPL levels below this ISR
 * priority if accurate and monotonic NOW counts are needed. If you wish to use
 * these APIs in higher priority ISRs, increase the timer ISR priority level
 * to one higher than the highest caller.
 */
void NOW_TMR1_ResetConst(void);
void NOW_TMR2_ResetConst(void);
void NOW_TMR3_ResetConst(void);
void NOW_TMR4_ResetConst(void);
void NOW_TMR5_ResetConst(void);
void NOW_TMR6_ResetConst(void);
void NOW_CCP1_ResetConst(void);
void NOW_CCP2_ResetConst(void);
void NOW_CCP3_ResetConst(void);
void NOW_CCP4_ResetConst(void);
void NOW_CCP5_ResetConst(void);
void NOW_CCP6_ResetConst(void);
void NOW_CCP7_ResetConst(void);
void NOW_CCP8_ResetConst(void);
#elif defined(__PIC32__) || defined(__XC32__)
void NOW_CORETMR_ResetConst(void);
#endif

#if defined(__XC16__)
/**
 * void NOW_Reset(const_token timerTypeNumber, unsigned long FCYClock);
 *
 * Macro enables a 16-bit Timer to use for the NOW_16(), NOW_32(), NOW_64(), 
 * NOW_CreateTask(), NOW_CreateRepeatingTask() and any other NOW timer APIs that 
 * depend on an a real time value up to 64-bits long. The NOW_Fcy,
 * NOW_sec, NOW_ms and NOW_us global variables are also
 * configured.
 * 
 * This macro selects the correct NOW_x_ResetConst() API to call corresponding
 * to the given timer type + module instance number and optimally computes the
 * values for NOW_Fcy, NOW_sec, etc.
 * 
 * When FCYClock is a compile time constant, the derived NOW_sec/NOW_ms/NOW_us
 * values are computed at compile-time. The NOW_x_ResetConst() API is called to
 * save code space and run-time computation.
 *
 * When FCYClock is not constant, NOW_x_ResetCalc() is called which derives the
 * needed variables by doing run-time division against the FCYClock.
 *
 * @param timerTypeNumber
 *      Literal token that specifies which 16-bit hardware timer resource to use
 *      for the NOW_*() APIs. This timer will not be available for other uses
 *      and will have its interrupt implemented and enabled for execution every
 *      65536 instruction clocks.
 *
 *      Legal tokens are:
 *          TMR1 - Timer 1 operated with a period of 0x0000FFFF if implemented in hardware as 32-bit timer
 *          TMR2 - Timer 2 configured as a 16-bit timer
 *          TMR3 - Timer 3 configured as a 16-bit timer (Timer 2 cannot join to Timer 3 for a 32-bit timer)
 *          TMR4 - Timer 4 configured as a 16-bit timer
 *          TMR5 - Timer 5 configured as a 16-bit timer (Timer 4 cannot join to Timer 5 for a 32-bit timer)
 *          TMR6 - Timer 6 configured as a 16-bit timer
 *          CCP1 - CCP 1 operated in dual 16-bit timer mode (_CCT1Interrupt(), CCP1TMRH, CCP1PRH clobbered)
 *          CCP2 - CCP 2 operated in dual 16-bit timer mode (_CCT2Interrupt(), CCP2TMRH, CCP2PRH clobbered)
 *          CCP3 - CCP 3 operated in dual 16-bit timer mode (_CCT3Interrupt(), CCP3TMRH, CCP3PRH clobbered)
 *          CCP4 - CCP 4 operated in dual 16-bit timer mode (_CCT4Interrupt(), CCP4TMRH, CCP4PRH clobbered)
 *          CCP5 - CCP 5 operated in dual 16-bit timer mode (_CCT5Interrupt(), CCP5TMRH, CCP5PRH clobbered)
 *          CCP6 - CCP 6 operated in dual 16-bit timer mode (_CCT6Interrupt(), CCP6TMRH, CCP6PRH clobbered)
 *          CCP7 - CCP 7 operated in dual 16-bit timer mode (_CCT7Interrupt(), CCP7TMRH, CCP7PRH clobbered)
 *          CCP8 - CCP 8 operated in dual 16-bit timer mode (_CCT8Interrupt(), CCP8TMRH, CCP8PRH clobbered)
 *
 *      An example calling sequence is:
 *          NOW_Reset(TMR2, 16000000);
 *      This will initialize Timer 2 and implement the _T2Interrupt() ISR. For 
 *      CCP2 instead, use:
 *          NOW_Reset(CCP2, 16000000);
 *
 * @param FCYClock
 *      Instruction execution clock, in Hertz. Ex: 16000000 for 16 MIPS.
 *
 *      If possible, specify FCYClock as a compile-time constant to minimize
 *      code size.
 *
 * @return void
 *         All NOW_*() APIs will be usable and the NOW_Fcy,
 *         NOW_sec, NOW_ms, and NOW_us global variables
 *         will be set.
 *
 *         The timer's interrupt will be enabled on return.
 *
 *         Automatic NOW task dispatching will be started, if any code calls
 *         NOW_CreateTask() or NOW_CreateRepeatingTask().
 *
 *         Besides start up initialization, this function may later be called
 *         during application execution to reinitialize the timer SFRs. However,
 *         the TMRx/CCPxTMRH register is never changed, nor the internally
 *         stored extended bits for 64-bit emulation, so absolute forward time
 *         progression will be continue, subject to a one time monotonicity
 *         error in the low order 16-bits (i.e. time can temporarily be up to
 *         65535 counts less than it previously was when the timer was disabled
 *         by application code). When calling NOW_Reset() 2+ times, the
 *         timerTypeNumber parameter must be the same.
 */
#define NOW_Reset(timerTypeNumber, FCYClock)                        \
{                                                                   \
    if(__builtin_constant_p(FCYClock))                              \
    {                                                               \
        NOW_Fcy = (FCYClock);                                       \
        NOW_sec = (FCYClock);                                       \
        NOW_ms  = ((FCYClock)+500u)/1000u;                          \
        NOW_us  = ((FCYClock)+500000u)/1000000u;                    \
        NOW_##timerTypeNumber##_ResetConst();                       \
    }                                                               \
    else                                                            \
    {                                                               \
        NOW_##timerTypeNumber##_ResetCalc(FCYClock);                \
    }                                                               \
}

#elif defined(__PIC32__) || defined(__XC32__)
/**
 * void NOW_Reset(const_token timerTypeNumber, unsigned long FCYClock);
 *
 * Macro enables the PIC32MM MIPS CP0 CoreTimer for implementing the NOW_16(),
 * NOW_32(), and NOW_64() NOW timer APIs that depend on an a real time value up
 * to 64-bits long. The NOW_Fcy, NOW_sec, NOW_ms and NOW_us global variables are
 * also configured.
 *
 * This macro selects the correct NOW_x_ResetConst() API to call corresponding
 * to the given timer type + module instance number and optimally computes the
 * values for NOW_Fcy, NOW_sec, etc.
 *
 * When FCYClock is a compile time constant, the derived NOW_sec/NOW_ms/NOW_us
 * values are computed at compile-time. The NOW_x_ResetConst() API is called to
 * save code space and run-time computation.
 *
 * When FCYClock is not constant, NOW_x_ResetCalc() is called which derives the
 * needed variables by doing run-time division against the FCYClock.
 *
 * @param timerTypeNumber
 *      Literal token that specifies which hardware timer resource to use
 *      for the NOW_*() APIs. This timer should not be written or required by
 *      other software.
 *
 *      Legal tokens are:
 *          CORETMR - MIPS CP0 Core Timer implemented as 32-bit timer,
 *                    incrementing at SYS_CLK/2, period 0xFFFFFFFF + 1, and no
 *                    interrupts (polling sufficient)
 *
 *      An example calling sequence is:
 *          NOW_Reset(CORETMR, 24000000);
 *
 * @param FCYClock
 *      Instruction execution clock, in Hertz. Ex: 24000000 for 24 MIPS.
 *
 *      If possible, specify FCYClock as a compile-time constant to minimize
 *      code size.
 *
 * @return void
 *         All NOW_*() APIs will be usable and the NOW_Fcy,
 *         NOW_sec, NOW_ms, and NOW_us global variables
 *         will be set.
 *
 *         Besides start up initialization, this function may later be called
 *         during application execution to reinitialize the timer SFRs. However,
 *         the TMRx/CCPxTMRH register is never changed, nor the internally
 *         stored extended bits for 64-bit emulation, so absolute forward time
 *         progression will be continue, subject to a one time monotonicity
 *         error in the low order 16-bits (i.e. time can temporarily be up to
 *         65535 counts less than it previously was when the timer was disabled
 *         by application code). When calling NOW_Reset() 2+ times, the
 *         timerTypeNumber parameter must be the same.
 */
#define NOW_Reset(timerTypeNumber, FCYClock)                        \
{                                                                   \
    if(__builtin_constant_p(FCYClock))                              \
    {                                                               \
        NOW_Fcy = (FCYClock);                                       \
        NOW_sec = (FCYClock)/2u;                                    \
        NOW_ms  = ((FCYClock)+250u)/500u;                           \
        NOW_us  = ((FCYClock)+250000u)/500000u;                     \
        NOW_##timerTypeNumber##_ResetConst();                       \
    }                                                               \
    else                                                            \
    {                                                               \
        NOW_##timerTypeNumber##_ResetCalc(FCYClock);                \
    }                                                               \
}
#endif


/**
 * Sets the Interrupt Enable bit (ex: IFSx<T1IE> or IFSx<CCT1IE>) used by the
 * NOW timing module and scheduler.
 *
 * If you leave the interrupt disabled for too long and do not poll NOW_32() or
 * NOW_64() frequently enough, it is possible for the 16-bit hardware timer to
 * have rolled over more than once. In this case, you will have lost time,
 * effectively making various stored NOW_16()/NOW_32()/NOW_64() return values
 * less accurate and potentially non-monotonic. It is therefore recommended that
 * the NOW interrupt be disabled or masked for less than 128K instruction
 * cycles max.
 *
 * @return 0 if the prior state of the interrupt was disabled
 *         1 if the prior state of the interrupt was enabled
 *
 *         Timer/CCP Timer interrupt enable bit in IECx register is set to '1'.
 *
 *         If the NOW_Reset() macro or a NOW_TMRx_ResetCalc(),
 *         NOW_TMRx_ResetConst(), NOW_CCPx_ResetCalc(), or NOW_CCPx_ResetConst()
 *         function is never called anywhere in the project (or EZBL bootloader
 *         project), this function will effectively resolve to a series of
 *         no-operations without causing a linking error. I.e. No SFR writes
 *         will take place and no harm will be done, with a mere 0 always being
 *         returned.
 */
int NOW_IntEnable(void);
int NOW_EnableInterrupts(void); // Alias to NOW_IntEnable() for backwards compatibility with EZBL v1.xx


/**
 * Clear the Interrupt Enable bit (ex: IFSx<T1IE> or IFSx<CCT1IE>) used by the
 * NOW timing module and scheduler.
 *
 * If you leave the interrupt disabled for too long and do not poll NOW_32() or
 * NOW_64() frequently enough, it is possible for the 16-bit hardware timer to
 * have rolled over more than once. In this case, you will have lost time,
 * effectively making various stored NOW_16()/NOW_32()/NOW_64() return values
 * less accurate and potentially non-monotonic. It is therefore recommended that
 * the NOW interrupt be disabled or masked for less than 128K instruction
 * cycles max.
 *
 * @return 0 if the prior state of the interrupt was disabled
 *         1 if the prior state of the interrupt was enabled
 *
 *         Timer/CCP Timer interrupt enable bit in IECx register is set to '0'.
 *
 *         If the NOW_Reset() macro or a NOW_TMRx_ResetCalc(),
 *         NOW_TMRx_ResetConst(), NOW_CCPx_ResetCalc(), or NOW_CCPx_ResetConst()
 *         function is never called anywhere in the project (or EZBL bootloader
 *         project), this function will effectively resolve to a series of
 *         no-operations without causing a linking error. I.e. No SFR writes
 *         will take place and no harm will be done, with a mere 0 always being
 *         returned.
 */
int NOW_IntDisable(void);
int NOW_DisableInterrupts(void);    // Alias to NOW_IntEnable() for backwards compatibility with EZBL v1.xx


/**
 * Returns the current state of the Interrupt Enable bit (ex: IFSy<TxIE> or
 * IFSy<CCTxIE>) used by the NOW timing module and scheduler.
 *
 * @return 0 if the interrupt is disabled
 *         1 if the interrupt is enabled
 *
 *         If the NOW_Reset() macro or a NOW_TMRx_ResetCalc(),
 *         NOW_TMRx_ResetConst(), NOW_CCPx_ResetCalc(), or NOW_CCPx_ResetConst()
 *         function is never called anywhere in the project (or EZBL bootloader
 *         project), this function will effectively resolve to a series of
 *         no-operations without causing a linking error. I.e. No SFR writes
 *         will take place and no harm will be done, with a mere 0 always being
 *         returned.
 */
int NOW_RdIntEn(void);


/**
 * Writes an array of bytes to an SPI peripheral (SPIxBUF register), optionally
 * returning all data shifted in during the same transfer. Data is shifted out
 * as fast as possible (normally back-to-back operation is achieved) with the
 * function blocking until all specified bytes make it completely out of the
 * device.
 *
 * @param SPIInstanceNum SPI hardware peripheral number to perform the exchange
 *                       on. Set to 1 for SPI1, 2 for SPI2, 3 for SPI3, etc. Do
 *                       not specify an unimplemented peripheral number as this
 *                       function treats this number like a pointer (i.e.
 *                       incorrect memory locations will be read/written if
 *                       SPIInstanceNum is invalid).
 *
 * @param *rxDataDest Pointer to RAM to write bytes read from the SPIxBUF
 *                    register. This pointer can match *txDataSrc to write the
 *                    RX data to the same buffer used for the the TX data or can
 *                    be set to null if no RX data should be saved (SPIxBUF will
 *                    still be popped a correct number of times to ensure SPIROV
 *                    doesn't become set and no extra data is left in the
 *                    SPIxBUF path).
 *
 * @param *txDataSrc Pointer to array of bytes to write to the SPIxBUF register.
 *
 * @param byteLength Number of bytes to read and write.
 *
 * @return Pointers are read/written by exactly byteLength elements and SPIxBUF
 *         is written/read by exactly byteLength elements.
 *
 *         The SPI interrupt flag(s) are not accessed (read or written).
 */
void EZBL_SPIExchange(unsigned int SPIInstanceNum, void *rxDataDest, const void *txDataSrc, unsigned int byteLength);


/**
 * Writes an array of bytes to an SPI peripheral (SPIxBUF register), reading,
 * but throwing away data shifted in during the same transfer. Data is shifted
 * out as fast as possible (normally back-to-back operation is achieved) with
 * the function blocking until all specified bytes make it completely out of the
 * device.
 *
 * This is a trivial wrapper function for EZBL_SPIExchange(), using a null
 * pointer for rxDataDest.
 *
 * @param SPIInstanceNum SPI hardware peripheral number to perform the write
 *                       on. Set to 1 for SPI1, 2 for SPI2, 3 for SPI3, etc. Do
 *                       not specify an unimplemented peripheral number as this
 *                       function treats this number like a pointer (i.e.
 *                       incorrect memory locations will be read/written if
 *                       SPIInstanceNum is invalid).
 *
 * @param *txDataSrc Pointer to array of bytes to write to the SPIxBUF register.
 *
 * @param byteLength Number of bytes to write (and read+discard).
 *
 * @return Pointer is read by exactly byteLength elements and SPIxBUF
 *         is written/read by exactly byteLength elements.
 *
 *         The SPI interrupt flag(s) are not accessed (read or written).
 */
void EZBL_SPIWrite(unsigned int SPIInstanceNum, const void *txDataSrc, unsigned int byteLength);


/**
 * Repeatedly writes a single character to an SPI peripheral (SPIxBUF register),
 * returning all data shifted in during the same transfer. Data is shifted in
 * as fast as possible (normally back-to-back operation is achieved) with the
 * function blocking until all specified bytes are read.
 *
 * @param SPIInstanceNum SPI hardware peripheral number to perform the read
 *                       on. Set to 1 for SPI1, 2 for SPI2, 3 for SPI3, etc. Do
 *                       not specify an unimplemented peripheral number as this
 *                       function treats this number like a pointer (i.e.
 *                       incorrect memory locations will be read/written if
 *                       SPIInstanceNum is invalid).
 *
 * @param *rxDataDest Pointer to RAM to write bytes read from the SPIxBUF
 *                    register. This pointer can be set to null if no RX data
 *                    should be saved (i.e. just generates SPI clocks for the
 *                    slave device to process something internally).
 *
 * @param txVal 8-bit character value to transmit in order to generate SCK
 *              clocks to receive data. This character is repeated byteLength
 *              times.
 *
 * @param byteLength Number of bytes to read.
 *
 * @return Pointer is written by exactly byteLength elements (when not null) and
 *         SPIxBUF is written/read by exactly byteLength elements.
 *
 *         The SPI interrupt flag(s) are not accessed (read or written).
 */
void EZBL_SPIRead(unsigned int SPIInstanceNum, void *rxDataDest, unsigned char txVal, unsigned int byteLength);


/**
 * Macro resets an ANSI terminal connected to EZBL_STDOUT by transmitting:
 *      \x0F\x1B[0m
 *
 * "\x0F" is a SI "Shift In" control code so terminal emulators go back to the
 * correct character set if they got changed while we were being programmed or
 * reset
 *
 * "\x1B[0m" resets all attributes such as text coloring
 */
#define EZBL_ConsoleReset()     EZBL_FIFOWriteStr(EZBL_STDOUT, "\x0F\x1B[0m")


/**
 * Macro resets and clears an ANSI terminal connected to EZBL_STDOUT by transmitting:
 *      \x0F\x1B[2J\x1B[1;1f\x1B[0m
 * "\x0F" is a SI "Shift In" control code so terminal emulators go back to the
 * correct character set if they got changed while we were being programmed or
 * reset
 *
 * "\x1B[2J" ANSI sequence clears the screen (scroll back buffer retained)
 *
 * "\x1B[1;1f" places that cursor at row 1 column 1
 *
 * "\x1B[0m" resets all attributes such as text coloring
 */
#define EZBL_ConsoleClear()     EZBL_FIFOWriteStr(EZBL_STDOUT, "\x0F\x1B[2J\x1B[1;1f\x1B[0m")


/**
 * Macro changes ANSI terminal connected to EZBL_STDOUT to begin printing text
 * in red font face.
 *
 * This macro is equivalent to writing the following string to stdout:
 *      \x1B[31m
 */
#define EZBL_ConsoleRed()       EZBL_FIFOWriteStr(EZBL_STDOUT, ("\x1B[31m")


#if defined(__XC16__)
/**
 * Powerful trap exception ISR handler to help debug run-time exception
 * traps. This implementation captures and prints a greater than normal amount
 * of potentially useful system state to a console/UART via stdout.
 *
 * Before a trap occurs, a UART or other interface must be initialized to handle
 * stdout. This can be accomplished with the UART_Reset() ezbl_lib.a macro.
 *
 * Example captured data includes:
 *   - w0-w15 at time of the trap (not modified as normally happens for a C
 *     handler)
 *   - SR, RCON, INTCON1, PSVPAG/DSRPAG+DSWPAG, TBLPAG, RCOUNT and trap
 *     return address
 *   - The opcodes immediately near the trap's return address.
 *   - IFS/IEC/IPC SFR state bits, with interrupts having both the IEC Interrupt
 *     Enable bit set and IFS Interrupt Flag bit set highlighted as red text.
 *   - RAM and Stack contents
 *   - Flash reset/interrupt vector contents and some code
 *
 * To use this handler, at global file scope within a .c file in the project,
 * insert:
 *      EZBL_KeepSYM(_DefaultInterrupt);
 *      EZBL_KeepSYM(_AddressError);
 *      EZBL_KeepSYM(_MathError);
 *      etc.
 * Each statement hooks the corresponding IVT entry for the trap to the common
 * EZBL_TrapHandler() ISR function, assuming a named trap handler ISR function
 * doesn't already exist that matches the kept symbol.
 *
 * @return void via RETFIE and with the trap flag bits in INTCON1 cleared.
 *
 */
void EZBL_TrapHandler(void);    // Actually attributed __attribute__((keep, auto_psv, interrupt)), but this generates unnecessary compiler warnings since "EZBL_TrapHandler" isn't a device implemented ISR.

#elif defined(__XC32__) || defined(__PIC32__)

// "+--+--+-----+--+---+--+--+--+--+----+-----+-------------+-------+-+--------+---+\n"
// "|BD|TI|   CE|DC|PCI|IC|AP|IV|WP|FDCI|    0|RIPL17.RIPL10|IP1.IP0|0|Exc Code|  0|\n"
// "|31|30|29 28|27| 26|25|24|23|22|  21|20 18|17         10|9     8|7|6      2|1 0|\n"
// "+--+--+-----+--+---+--+--+--+--+----+-----+-------------+-------+-+--------+---+\n"
typedef union
{
    unsigned int val;
    struct
    {
        unsigned int :2;
        enum
        {
            Interrupt = 0x00,
            AddressError_LoadOrInstructionFetch = 0x4,
            AddressError_Store = 0x05,
            BusError_InstructionFetch = 0x06,
            BusError_DataLoadStoreReference = 0x07,
            SyscallException = 0x08,
            BreakpointException = 0x09,
            ReservedInstructionException = 0x0A,
            CoprocessorUnusableException = 0x0B,
            ArithmeticOverflowException = 0x0C,
            TrapException = 0x0D,
            MPUException = 0x1D
        } ExceptionCode:5;
        unsigned int :1;
        unsigned int SoftwareInterruptRequest0:1;
        unsigned int SoftwareInterruptRequest1:1;
        unsigned int RequestedInterruptPriorityLevel:8;
        unsigned int :3;
        unsigned int FastDebugChannelIF:1;          // Fast Debug Channel Interrupt flag
        unsigned int WatchExceptionDeferred:1;      // Watch exception was deferred
        unsigned int SpecialInterruptVector:1;      // 0 = Interrupts use General Exception Vector; 1 = Interrupts use Special Interrupt Vector
        unsigned int AutoIntPrologueException:1;    // Exception occurred during Interrupt Auto-Prologue execution
        unsigned int InterruptChaining:1;           // Interrupt Chaining occurred on last IRET
        unsigned int PerformanceCounterIF:1;        // Performance Counter Interrupt flag
        unsigned int DisableCount:1;                // Disable CP0 Core Timer counting
        unsigned int COPRefOnCOPUnusable:2;         // Coprocessor referenced when a COP Unusable exception taken
        unsigned int CoreTimerIF:1;                 // Core Timer Interrupt flag
        unsigned int ExceptionInBranchDelay:1;      // Exception in Branch Delay slot (1 = in delay; 0 = not in delay)
    };
} EZBL_MIPS_CAUSE;


// MIPS Coprocessor 0 Exception Status register
// "+--------+--+--+--+--+--+---+--+--+---+---+---+---------+---+--+-+---+---+--+\n"
// "|CU3..CU0|RP|FR|RE|MX| R|BEV|TS|SR|NMI|IPL|CEE|      IPL|  R|UM|R|ERL|EXL|IE|\n"
// "|31    28|27|26|25|24|23| 22|21|20| 19| 18| 17|16 10 9 8|7 5| 4|3|  2|  1| 0|\n"
// "+--------+--+--+--+--+--+---+--+--+---+---+---+---------+---+--+-+---+---+--+\n"
typedef union
{
    unsigned int val;
    struct
    {
        unsigned int GlobalInterruptEnable:1;   // Interrupt Enable (1 = interrupts enabled; 0 = interrupts disabled)
        unsigned int ExceptionLevel:1;          // Exception Level (1 = Exception level; 0 = Normal level)
        unsigned int ErrorLevel:1;              // Error Level (1 = Error level; 0 = Normal level)
        unsigned int :1;                        // Reserved
        unsigned int UserMode:1;                // Base operating mode (0 = Kernel mode; 1 = User mode)
        unsigned int :2;                        // Reserved
        unsigned int IPL0:4;                    // Current CPU Interrupt Priority Level, bits 0 though 3
        unsigned int CoreExtendedEnable:1;      // Reserved
        unsigned int IPL7:1;                    // IPL<7> bit
        unsigned int NonMaskableInterrupt:1;    // Non-Maskable Interrupt occurred
        unsigned int SoftReset:1;               // Soft Reset occurred
        unsigned int TLBShutdown:1;             // Reserved
        unsigned int BootstrapExceptionVectors:1;   // Controls exception vector (0 = Normal; 1 = Bootstrap)
        unsigned int :1;                        // Reserved
        unsigned int MIPSDSPExtension:1;        // Reserved
        unsigned int FloatingPointMode:1;       // Reserved
        unsigned int ReducedPower:1;            // Enables reduced power mode (1 = WAIT instruction causes Sleep; 0 = WAIT instruction causes Idle)
        unsigned int Coprocessor0Access:1;      // Access to coprocessor 0 allowed (always 1)
        unsigned int Coprocessor1Access:1;      // Reserved: Access to coprocessor 1 allowed (always 0)
        unsigned int Coprocessor2Access:1;      // Reserved: Access to coprocessor 2 allowed (always 0)
        unsigned int Coprocessor3Access:1;      // Reserved: Access to coprocessor 3 allowed (always 0)
    };
} EZBL_MIPS_STATUS;


typedef struct
{
    unsigned int v[2];        // Offset 0x00: Return Values
    unsigned int a[4];        // Offset 0x08: Arguments
    unsigned int t[10];       // Offset 0x18: Temporaries
    unsigned int s[8];        // Offset 0x40: Saved
    unsigned int k[2];        // Offset 0x60: Kernel temporaries
    unsigned int gp;          // Offset 0x68: Global Pointer to RAM small data
    unsigned int sp;          // Offset 0x6C: Stack Pointer
    unsigned int fp;          // Offset 0x70: Frame Pointer
    unsigned int ra;          // Offset 0x74: Return Address
    unsigned long long hilo;  // Offset 0x78: HI/LO multiply/divide 64-bit register
    unsigned int at;          // Offset 0x80: Assembler temporary
} EZBL_CPU_REGS;

typedef struct
{
    unsigned int BadVAddr;          // Reports the address for the most recent address-related exception
    unsigned int BadInstr;          // Reports the instruction that caused the most recent exception
    unsigned int BadInstrP;         // Reports the branch instruction if a delay slot caused the most recent exception
    unsigned int Count;             // Core Timer (processor cycle count/2)
    unsigned int Compare;           // Core Timer period/compare interrupt value
    EZBL_MIPS_STATUS Status;        // Processor Status and control
    unsigned int IntCtl;            // Interrupt system status and control
    unsigned int SRSCtl;            // Shadow Register Sets status and control
    unsigned int SRSMap1;           // Shadow set IPL mapping
    unsigned int View_IPL;          // Contiguous view of IM and IPL fields
    unsigned int SRSMap2;           // Shadow set IPL mapping
    EZBL_MIPS_CAUSE Cause;          // Cause of last exception
    unsigned int View_RIPL;
    unsigned int NestedExc;
    unsigned int EPC;               // Program counter at last exception
    unsigned int NestedEPc;         // Program counter at last exception
    unsigned int PRId;              // Processor identification and revision
    unsigned int EBase;             // Exception base address
    unsigned int CDMMBase;          // Common Device Memory Map Base register
    unsigned int Config;            // Configuration register 0
    unsigned int Config1;           // Configuration register 1
    unsigned int Config2;           // Configuration register 2
    unsigned int Config3;           // Configuration register 3
    unsigned int Config4;           // Configuration register 4
    unsigned int Config5;           // Configuration register 5
    unsigned int Config7;           // Configuration register 7
    unsigned int LLAddr;            // Load Linked Address
    unsigned int WatchLo;           // Watchpoint Address low order bits
    unsigned int WatchHi;           // Watchpoint Address high order bits
    unsigned int Debug;             // EJTAG Debug register
    unsigned int Debug2;            // EJTAG Debug register 2
    unsigned int DEPC;              // Program counter at last debug exception
    unsigned int PerfCtl0;          // Performance Counter 0 Control
    unsigned int PerfCnt0;          // Performance Counter 0
    unsigned int PerfCtl1;          // Performance Counter 1 Control
    unsigned int PerfCnt1;          // Performance Counter 1
    unsigned int ErrorEPC;          // Program counter at last error
    unsigned int DeSAVE;            // Debug handler scratchpad register
    unsigned int KScratch1;         // Scratch Register 1 for Kernel Mode
    unsigned int KScratch2;         // Scratch Register 2 for Kernel Mode
} EZBL_CP0_REGS;


/**
 * EZBL general exception/trap handler for PIC32MM devices which will collect
 * all CPU + CP0 registers at the point of the exception and dump them, along
 * with various other RAM, Flash and SFR state information in human readable
 * text form to stdout/EZBL_STDOUT.
 *
 * This function forces inclusion of the EZBL_general_exception_context()
 * function, not the default _general_exception_context function provided by the
 * XC32 toolchain.
 *
 * To use this trap handler, place this statement in a C source file at
 * file-scope:
 *      EZBL_KeepSYM(EZBL_TrapHandler);
 * EZBL_general_exception(), if present, will be called after executing this
 * trap handler. After EZBL_general_exception() returns, assuming it exists,
 * _general_exception_handler(), will be called, if present. If
 * EZBL_general_exception() does not exist, _general_exception_handler() will be
 * called, if present after returning from this EZBL_TrapHandler() function.
 *
 * @param cause Coprocessor 0 Cause register, in an easily evaluated bitfield
 *              structure. Access cause.val to retrieve the undecoded value
 *              normally available in a _general_exception_handler().
 *
 * @param status Coprocessor 0 Status register, in an easily evaluated bitfield
 *              structure. Access status.val to retrieve the undecoded value
 *              normally available in a _general_exception_handler().
 *
 * @param *cpuRegs Pointer to an EZBL_CPU_REGS structure allocated temporarily
 *                 on the stack that contains all of the MIPS CPU register
 *                 contents when the exception occurred. When this function
 *                 returns, the contents of this structure is deallocated and
 *                 non-saved/temporary register values (t0-t9, v0-v1, a0-a3, a0,
 *                 k0-k1)will be written back into the CPU registers.
 *
 * @param *cp0Regs Pointer to an EZBL_CP0_REGS structure allocated temporarily
 *                 on the stack that contains all of the Coprocessor 0 register
 *                 contents when the exception occurred. When this function
 *                 returns, the contents of this structure is deallocated and
 *                 any values written into it will also be discarded (i.e. they
 *                 are not written back to CP0 at exception return).
 */
void EZBL_TrapHandler(EZBL_MIPS_CAUSE cause, EZBL_MIPS_STATUS status, EZBL_CPU_REGS *cpuRegs, EZBL_CP0_REGS *cp0Regs);


/**
 * EZBL general exception/trap handler for PIC32MM devices which will collect
 * all CPU + CP0 registers at the point of the exception, allowing more thorough
 * analysis of the cause of the exception without loss of state information by
 * the compiler reusing registers in a typical _general_exception_handler()
 * routine.
 *
 * This function requires the EZBL_general_exception_context() function  to be
 * linked, not the default _general_exception_context function provided by the
 * XC32 toolchain.
 *
 * To implement this exception handler, place this statement in a C source file
 * at file-scope:
 *      EZBL_KeepSYM(EZBL_general_exception_context);
 * Then, implement a function named EZBL_general_exception() and matching this
 * function signature:
 *      void EZBL_general_exception(EZBL_MIPS_CAUSE cause, EZBL_MIPS_STATUS status, EZBL_CPU_REGS *cpuRegs, EZBL_CP0_REGS *cp0Regs);
 *
 * At return, _general_exception_handler(), will be called, if present. When
 * absent, execution returns to the code that was executing when the exception
 * took place.
 *
 * @param cause Coprocessor 0 Cause register, in an easily evaluated bitfield
 *              structure. Access cause.val to retrieve the undecoded value
 *              normally available in a _general_exception_handler().
 *
 * @param status Coprocessor 0 Status register, in an easily evaluated bitfield
 *              structure. Access status.val to retrieve the undecoded value
 *              normally available in a _general_exception_handler().
 *
 * @param *cpuRegs Pointer to an EZBL_CPU_REGS structure allocated temporarily
 *                 on the stack that contains all of the MIPS CPU register
 *                 contents when the exception occurred. When this function
 *                 returns, the contents of this structure is deallocated and
 *                 non-saved/temporary register values (t0-t9, v0-v1, a0-a3, a0,
 *                 k0-k1)will be written back into the CPU registers.
 *
 * @param *cp0Regs Pointer to an EZBL_CP0_REGS structure allocated temporarily
 *                 on the stack that contains all of the Coprocessor 0 register
 *                 contents when the exception occurred. When this function
 *                 returns, the contents of this structure is deallocated and
 *                 any values written into it will also be discarded (i.e. they
 *                 are not written back to CP0 at exception return).
 */
void EZBL_general_exception(EZBL_MIPS_CAUSE cause, EZBL_MIPS_STATUS status, EZBL_CPU_REGS *cpuRegs, EZBL_CP0_REGS *cp0Regs);


/**
 * Prints the individually decoded value and meaning of each bitfield in the
 * MIPS cause CPU register to stdout.
 *
 * @param status MIPS cause CPU register value to decode. This integer is
 *               obtained as a parameter from a trap handler function.
 */
void EZBL_DumpDecodedCause(unsigned int cause);


/**
 * Prints the individually decoded value and meaning of each bitfield in the
 * MIPS Status CPU register to stdout.
 *
 * @param status MIPS Status CPU register value to decode. This integer is
 *               obtained as a parameter from a trap handler function.
 */
void EZBL_DumpDecodedStatus(unsigned int status);

#endif


#endif	/* EZBL_H */


/*************************************************************************************************
 * @file   p33c_ccp.h
 * @author I62419
 * @date   Created on October 12, 2020, 5:41 PM
 ************************************************************************************************/

/*************************************************************************************************
 * @brief Generic Capture Compare Driver Module (header file)
 * @details
 * This additional header file contains defines for all required bit-settings of all related 
 * special function registers of a peripheral module and/or instance. 
 * This file is an additional header file on top of the generic device header file.
 ************************************************************************************************/

#ifndef P33C_CCP_SFR_ABSTRACTION_H
#define	P33C_CCP_SFR_ABSTRACTION_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types


#ifndef P33C_CCP_INSTANCE_SFRSET_s     

/***********************************************************************************
 * @ingroup lib-layer-pral-properties-public-ccp
 * @struct P33C_CCP_INSTANCE_SFRSET_s
 * @brief Abstracted set of Special Function Registers of a Capture Compare peripheral instance
 * @details
 * This structure defines an abstracted set of Special Function Registers of a 
 * Input Capture/Output Compare peripheral instance. Users can use this abstracted
 * set of registers to capture register settings from or write generic register 
 * settings to a specific instance of this peripheral.
 * 
 **********************************************************************************/

    struct P33C_CCP_INSTANCE_SFRSET_s {

        union {
            struct tagCCP1CON1LBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxCON1L; // CCPxCON1L: CCPx CONTROL 1 LOW REGISTERS

        union {
            struct tagCCP1CON1HBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxCON1H; // CCPxCON1H: CCPx CONTROL 1 HIGH REGISTERS

        union {
            struct tagCCP1CON2LBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxCON2L; // CCPxCON2L: CCPx CONTROL 2 LOW REGISTERS

        union {
            struct tagCCP1CON2HBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxCON2H; // CCPxCON2H: CCPx CONTROL 2 HIGH REGISTERS

        #if defined (CCP9CON3L)
        union {
            struct tagCCP9CON3LBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxCON3L; // CCPxCON3L: CCPx CONTROL 3 LOW REGISTERS (implemented in MCCP9 module only)
        #else
        volatile unsigned :16;  // (reserved) Memory gap between CCPxCON2H and CCPxCON3H
        #endif
        
        union {
            struct tagCCP1CON3HBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxCON3H; // CCPxCON3H: CCPx CONTROL 3 HIGH REGISTERS

        union {
            struct tagCCP1STATLBITS bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxSTATL; // CCPxSTATL: CCPx STATUS REGISTER

        volatile unsigned :16; // (reserved)
        
        union {
            struct {
                volatile uint16_t CCPTMRL : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxTMRL; // CCPxTMRL: CCPx TIMER COUTER REGISTER LOW 

        union {
            struct {
                volatile uint16_t CCPTMRH : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxTMRH; // CCPxTMRH: CCPx TIMER COUTER REGISTER HIGH

        union {
            struct {
                volatile uint16_t CCPPRL : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxPRL; // CCPx TIMER PERIOD REGISTER LOW

        union {
            struct {
                volatile uint16_t CCPPRH : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxPRH; // CCPx TIMER PERIOD REGISTER HIGH

        union {
            struct {
                volatile uint16_t CCPRAL : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxRAL; // CCPxRAL: CCPx PRIMARY OUTPUT COMPARE DATA BUFFER LOW

        volatile unsigned :16;
        
        union {
            struct {
                volatile uint16_t CCPRBL : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxRBL; // CCPxRBL: CCPx SECONDARY OUTPUT COMPARE DATA BUFFER LOW

        volatile unsigned :16;
        
        union {
            struct {
                volatile uint16_t CCPBUFL : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxBUFL; // CCPxBUFL: CCPx INPUT CAPTURE BUFFER LOW

        union {
            struct {
                volatile uint16_t CCPBUFH : 16; // Register bit-field
            } bits; // Register bit-field
            uint16_t value; // 16-bit wide register value
        } CCPxBUFH; // CCPxBUFH: CCPx INPUT CAPTURE BUFFER HIGH
        
    } __attribute__((packed));
    
    typedef struct P33C_CCP_INSTANCE_SFRSET_s P33C_CCP_MODULE_SFRSET_t;  ///< CCP INSTANCE SPECIAL FUNCTION REGISTER SET
    
/*********************************************************************************
 * @ingroup lib-layer-pral-properties-public-ccp
 * @def     P33C_CCPGEN_SFR_OFFSET
 * @brief   Derives the address offset between two peripheral instances
 * @details
 * This macro derives the address offset between two peripheral instances. 
 * Users can use this address offset to derive the start address to/from which
 * the register set should be written or read.
 **********************************************************************************/
    #define P33C_CCPGEN_SFR_OFFSET  ((volatile uint16_t)&CCP2CON1L - (volatile uint16_t)&CCP1CON1L) ///< CCPx SFR set instance offset

#endif
    
/* GLOBAL FUNCTION CALL PROTOTYPES */

extern volatile struct P33C_CCP_INSTANCE_SFRSET_s* p33c_CcpInstance_GetHandle(volatile uint16_t ccpInstance);
extern volatile struct P33C_CCP_INSTANCE_SFRSET_s  p33c_CcpInstance_ConfigRead(volatile uint16_t ccpInstance);
extern volatile uint16_t p33c_CcpInstance_ConfigWrite(
                volatile uint16_t ccpInstance, 
                volatile struct P33C_CCP_INSTANCE_SFRSET_s ccpConfig
            );


#endif	/* P33C_CCP_SFR_ABSTRACTION_H */
// END OF FILE

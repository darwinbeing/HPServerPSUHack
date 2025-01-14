/* Software License Agreement
 * ************************************************************************************************
 *
 * Software License Agreement
 *
 * Copyright © 2020 Microchip Technology Inc.  All rights reserved. Microchip licenses to you the
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

// Include Header Files
#include "p33c_pps.h"


/* ************************************************************************************************
 * @file p33c_pps.c
 * @brief Generic Peripheral Pin Select (PPS) Driver Module
 * @see p33c_pps.h
 *
 * @details
 * Some low-pin count devices have the capability to assign digital functions to a range of 
 * free configurable pins (RPx). This driver module offers functions to read from and write to
 * the configuration registers, which will connect a digital peripheral bock with a selected 
 * pin.
 *
 * @ref dsPIC33F Reference Manual, Section 10: I/O Ports, Chapter 10.3: Peripheral Multiplexing
 *
 * Author:	M91406
 * Version: 1.3
 *
 * History:
 *	11/04/2008	First Release
 *  08/25/2009	Added definitions for new peripherals
 *  02/05/2012	Format transfer
 *  04/11/2016	Added definitions and compile switches for dsPIC33EP GS
 *  05/29/2019  Added RPx pin configuration lock/unlock sequence for dsPIC33C
 *  10/14/2020  Removed support for dsPIC33F and dsPIC33E
 * ***********************************************************************************************/

/*************************************************************************************************
 * @fn uint16_t PPS_LockIO(void)
 * @ingroup lib-layer-pral-functions-public-pps
 * @brief Locks the Peripheral Pin Select Configuration registers against accidental changes
 * @return
 *  unsigned integer 
 *  1: Success
 *  0: Error
 *
 * @details
 * This inline-assembly routine locks the Port Multiplexing Configuration registers by keeping
 * the required number of cycles during the Lock pocess. This function has to be called once 
 * before digital functions are mapped to a specific pin. Once called, a series of assignments
 * can be executed.
 * 
 * @note If bit IOL1WAY in the Oscillator Configuration Bits (FOSC) is set, only one pin assignment
 * operation will be allowed. all following calls of Unlock() or Lock() will be ignored. 
 *
 * @see p33c_pps.h
 * 
 ************************************************************************************************/
volatile uint16_t PPS_LockIO(void){

    __builtin_write_RPCON(0x0800); // lock PPS
    return(RPCONbits.IOLOCK);
    
}

/*************************************************************************************************
 * @fn uint16_t PPS_UnlockIO(void)
 * @ingroup lib-layer-pral-functions-public-pps
 * @brief Unlocks the Peripheral Pin Select Configuration registers to enable changes
 * @return
 *  unsigned integer 
 *  1: Success
 *  0: Error
 *
 * @details
 * This inline-assembly routine unlocks the Port Multiplexing Configuration registers by keeping
 * the required number of cycles during the unlock process. This function has to be called once 
 * after digital functions have been mapped to a specific pin, to prevent accidental changes. 
 * 
 * @note If bit IOL1WAY in the Oscillator Configuration Bits (FOSC) is set, only one pin assignment
 * operation will be allowed. all following calls of Unlock() or Lock() will be ignored. 
 *
 * @see p33c_pps.h
 * 
 ************************************************************************************************/
volatile uint16_t PPS_UnlockIO(void){

    __builtin_write_RPCON(0x0000); // unlock PPS
    return(1 - RPCONbits.IOLOCK);
    
}

/*************************************************************************************************
 * @fn uint16_t PPS_RemapOutput(volatile uint8_t pinno, volatile uint8_t peripheral)
 * @ingroup lib-layer-pral-functions-public-pps
 * @brief Assigns a digital function output to a pin
 * @param pinno: Index number of the RPx-pin of type uint8_t, which should be assigned to the function
 * @param peripheral: Peripheral/Function ID of type uint8_t, which should be assigned to the pin
 * @return
 *  unsigned integer 
 *  1: Success
 *  0: Error
 *
 * @details
 * Any supported digital function output (e.g. UART TxD) can be assigned to one of the RPx pins
 * of the MCU/DSC. To assign a function output to a pin, call 
 *
 *	smpsPPS_RemapOutput([RP-NUMBER], [FUNCTION])
 * 
 * @code{.c}
 *	lres |= smpsPPS_RemapOutput(PIN_RP9 , PPSOUT_SYNCO1);	// Assign RP9 to PWMSyncClkOutput
 * @endcode
 *
 * @see p33c_pps.h
 * 
 *************************************************************************************************/
volatile uint16_t PPS_RemapOutput(volatile uint8_t pinno, volatile uint8_t peripheral){
	
    volatile uint16_t retval = 0;
    volatile uint8_t *regptr;
    volatile uint8_t pin_offset=0;
    
    pin_offset = (pinno - RP_PINNO_MIN);    // Determine pin-offset
    regptr = (volatile uint8_t *)&RPOR0;    // get register block base address
    regptr += (volatile uint8_t)pin_offset; // add offset
    *regptr = (volatile uint8_t)peripheral;	// copy configuration into register location
   retval = (bool)(*regptr == (volatile uint8_t)peripheral);	// Check if contents have been written correctly
    
	return (retval);

}

/*************************************************************************************************
 * @fn uint16_t PPS_RemapInput(volatile uint8_t pinno, volatile uint8_t *peripheral)
 * @ingroup lib-layer-pral-functions-public-pps
 * @brief Assigns a pin to a digital function input
 * @param pinno: Index number of the RPx-pin of type uint8_t, which should be assigned to the function
 * @param peripheral: Pointer to peripheral/function of type uint8_t, which should be assigned to the pin
 * @return 
 *  unsigned integer 
 *  1: Success
 *  0: Error
 *
 * @details
 * Any RPx pin can be assigned to a supported digital function input (e.g. UART RxD). To assign 
 * a pin to a function input, call 
 *
 *	smpsPPS_RemapInput([RP-NUMBER], [FUNCTION])
 * 
 * @code{.c}
 *	lres |= smpsPPS_RemapInput(PIN_RP10, PPSIN_U1RX);		// Assign RP10 to UART1 RxD
 * @endcode
 *
 * @see p33SMPS_pps.h, FOSC, IOL1WAY, IOL1WAY_ON, IOL1WAY_OFF, smpsPPS_LockIO, smpsPPS_UnlockIO, smpsPPS_RemapOutput, 
 *  smpsPPS_UnmapInput, smpsPPS_UnmapOutput
 * 
 * ***********************************************************************************************/
volatile uint16_t PPS_RemapInput(volatile uint8_t pinno, volatile uint8_t *peripheral)
{

	// Map selected pin function
    *peripheral = pinno;

	return 1;
  
}

/*************************************************************************************************
 * @fn uint16_t PPS_UnmapOutput(volatile uint8_t pinno)
 * @ingroup lib-layer-pral-functions-public-pps
 * @brief   Disconnects a pin from a digital function output
 * @param   pinno: Index number of the RPx-pin of type uint8_t, which should be assigned to the function
 * @return
 *  unsigned integer  
 *	1: Success
 *  0: Error
 *
 * @details
 * An existing assignment between any RPx pin and a supported digital function output will be
 * dissolved.
 *
 *	smpsPPS_UnmapOutput([RP-NUMBER])
 * 
 * @code{.c}
 *	lres |= smpsPPS_UnmapOutput(PIN_RP10);		// Dissolve RP10 assignment
 * @endcode
 *
 * @see p33c_pps.h
 * 
 ************************************************************************************************/
volatile uint16_t PPS_UnmapOutput(volatile uint8_t pinno)
{
    volatile uint16_t retval=0;

	// Unmap selected pin function
	retval = PPS_RemapOutput(pinno, PPSPIN_NULL);
	return retval;

}

/************************************************************************************************
 * @fn uint16_t PPS_UnmapInput(volatile uint8_t *peripheral)
 * @ingroup lib-layer-pral-functions-public-pps
 * @brief  Disconnects a pin from a digital function input
 * @param  peripheral: Pointer to peripheral of type uint8_t, which should be assigned to the pin
 * @return 
 *  unsigned integer  
 *	1: Success
 *  0: Error
 *
 * @details
 * An existing assignment between any RPx pin and a supported digital function input will be
 * dissolved.
 *
 *	smpsPPS_UnmapInput([RP-NUMBER])
 * 
 * @code{.c}
 *	lres |= smpsPPS_UnmapInput(PIN_RP10);		// Dissolve RP10 assignment
 * @endcode
 *
 * @see p33c_pps.h
 * 
 ************************************************************************************************/
volatile uint16_t PPS_UnmapInput(volatile uint8_t *peripheral)
{
    volatile uint16_t retval=0;

	// Unmap selected pin function
	retval = PPS_RemapInput(PPSPIN_NULL, peripheral);
	return retval;

}


// end of file

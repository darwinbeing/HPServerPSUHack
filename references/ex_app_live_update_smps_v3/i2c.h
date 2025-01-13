// DOM-IGNORE-BEGIN
/*******************************************************************************
  Copyright (C) 2016 Microchip Technology Inc.

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
// DOM-IGNORE-END
#ifndef I2C_H
#define	I2C_H


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    
#include "h/Define.h"
#include "h/load_config.h"
#include "ezbl_integration/ezbl.h"

extern volatile uint8_t I2CLoadWrite;
extern EZBL_FIFO I2C_Tx;
extern unsigned char  __attribute__((noload, section(".bss.I2C_TxBuffer"))) I2C_TxBuffer[];


int I2CSetResistorLoad(unsigned char newLoad);


/**
 * Queues an array of data for asynchronous generation of a Master-mode I2C
 * bus write transaction of 0 or more bytes to a specified 7 or 10-bit
 * I2C slave address.
 *
 * This function always returns without blocking. The I2C bus transmission will
 * complete in the background via the _MI2Cx_Interrupt() ISR, which is enabled
 * by this write call if the I2C peripheral was previously idle.
 *
 * It is legal to call this function an arbitrary number of times without the
 * first I2C bus transaction completing yet. Assuming sufficient I2C_Tx FIFO
 * space is available, each call will be queued in order.
 *
 * @param slaveAddress 7 or 10-bit I2C slave address to transmit to. Any address
 *                     that fits in 7-bits is assumed to be a 7-bit address,
 *                     while >= 128 (0x80+) is assumed to be a 10-bit I2C target
 *                     address.
 *
 * @param *writeData pointer to an array of data to write to the I2C_Tx FIFO for
 *                   asynchronous (interrupt driven) transmission to the I2C
 *                   slave.
 *
 *                   This pointer cannot be null unless dataCount is zero.
 *
 * @param dataCount Number of *writeData bytes to write to the I2C Tx FIFO.
 *
 * @return <ul>
 *              <li>-1 if FIFO currently has insufficient space to complete the
 *                  write asynchronously right now. FIFO space required is equal
 *                  to 2*sizeof(int) + dataCount.</li>
 *              <li> >= 0 indicates successful queuing of the transmit data.</li>
 *         </ul>
 */
int MI2C_Write(unsigned int slaveAddress, void *writeData, unsigned int dataCount);



static inline int __attribute__((always_inline, optimize(1))) MI2C_Write8(unsigned int slaveAddress, unsigned char writeData)
{
    return MI2C_Write(slaveAddress, &writeData, 1);
}


static inline int __attribute__((always_inline, optimize(1))) MI2C_Write16(unsigned int slaveAddress, unsigned short writeData)
{
    return MI2C_Write(slaveAddress, &writeData, 2);
}


static inline int __attribute__((always_inline, optimize(1))) MI2C_Write24(unsigned int slaveAddress, unsigned long writeData)
{
    return MI2C_Write(slaveAddress, &writeData, 3);
}


static inline int __attribute__((always_inline, optimize(1))) MI2C_Write32(unsigned int slaveAddress, unsigned long writeData)
{
    return MI2C_Write(slaveAddress, &writeData, 4);
}


static inline int __attribute__((always_inline, optimize(1))) MI2C_Write48(unsigned int slaveAddress, unsigned long long writeData)
{
    return MI2C_Write(slaveAddress, &writeData, 6);
}


static inline int __attribute__((always_inline, optimize(1))) MI2C_Write64(unsigned int slaveAddress, unsigned long long writeData)
{
    return MI2C_Write(slaveAddress, &writeData, 8);
}


// Internal callback function executed anytime MI2C_Write() or
// EZBL_FIFOWrite(xxx, &I2C_Tx, xxx) is called.
//
// This callback initializes the I2C Master mode TX state machine and enables
// the MI2Cx Interrupt, if needed to start a new bus transaction from an I2C
// peripheral idle state.
unsigned int MI2C_OnWrite(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO);

void __attribute__((__interrupt__, no_auto_psv)) _MI2C1Interrupt(void);


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */


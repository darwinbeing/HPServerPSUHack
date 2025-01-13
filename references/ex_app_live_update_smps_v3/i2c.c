/* 
 * File:   i2c.c
 * Author: C12128
 *
 * Created on April 5, 2016, 4:24 PM
 */
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

#include <xc.h>
#include <stdint.h>
#include "i2c.h"
#include "ezbl_integration/ezbl.h"

#define I2C_TX_FIFO_SIZE    14u   // In bytes. Header takes 4 bytes, so this allows ~2 effective queue depth, plus the one we are working on


typedef struct
{
    unsigned int slaveAddress;
    unsigned int packetDataLen;
} I2C_WR_HDR;

typedef enum
{
    SM_I2C_START = 0,
    SM_I2C_ADDRESS,     // 7-bit address doesn't use SM_I2C_ADDRESS10 state
    SM_I2C_ADDRESS10,   // 10-bit address always has '111110'98(R/~W) + 76543210 bit ordering, so only the two MSbits 9 and 8 are sent in the SM_I2C_ADDRESS state
    SM_I2C_DATA,
} SM_I2C_STATE;


// Global variables
volatile uint8_t __attribute__((persistent)) I2CLoadWrite;
EZBL_FIFO __attribute__((persistent)) I2C_Tx;
unsigned char  __attribute__((section(".bss.I2C_TxBuffer"), noload)) I2C_TxBuffer[I2C_TX_FIFO_SIZE];

// Local ISR state variables
static unsigned int __attribute__((persistent)) packetBytesConsumed;
static SM_I2C_STATE __attribute__((persistent)) transactionState;
static I2C_WR_HDR __attribute__((persistent)) lastI2CWrite;


void InitI2C1(void)
{
    // Hard reset the I2C1 peripheral by setting it's Peripheral Module Disable bit. This resets all internal state and the SFRs within the peripheral (but not the ones scattered around, like the interrupt ones)
    _I2C1MD  = 1;
    _MI2C1IE = 0;
    _MI2C1IF = 0;
    _I2C1MD  = 0;               // Bring the I2C1 module back, freshly reset

    EZBL_FIFOReset(&I2C_Tx, I2C_TxBuffer, sizeof(I2C_TxBuffer), MI2C_OnWrite, 0);

    // Configure I2C as MASTER to communicate with Load IC
    I2C1CON1bits.A10M = 0;      // 7-bit slave address/data
    I2C1CON1bits.DISSLW = 1;    // Slew rate control disabled

    I2C1BRG = (unsigned int)((1.0/I2CBAUDRATE - 120e-9) * (FCY/2u)) - 2u;   // dsPIC33EP64GS50x formula; 343 = 100kHz @ 70 MIPS: (((1/100k) - 110n) * 70M) - 2; 81 ~= 400kHz, 70 MIPS
    //I2C1BRG = (unsigned int)((1.0/I2CBAUDRATE - 120e-9) * FCY) - 2u;      // New devices with BRG reload calculation in data sheet and supporting 2x clocking

    I2C1CON1bits.I2CEN = 1;     // Enable I2C Module

    // Code Configures MCP23008 and sets up the initial load
    MI2C_Write16(SLAVEADDRESS, IOCON_REGADDR | (0x20u<<8));         // Disable sequential operation
    MI2C_Write16(SLAVEADDRESS, IODIR_REGADDR | (0x00u<<8));         // Configures all I/O pins as output
    I2CLoadWrite = 0xFF;                                            // Illegal load value to force first load write to be written when I2CSetResistorLoad() called
    I2CSetResistorLoad(I2CBUCKLOAD1WRITE | I2CBOOSTLOAD1WRITE);     // Set default loads
}


int I2CSetResistorLoad(unsigned char newLoad)
{
    int ret;

#if defined(__DEBUG)
    // When compiled for debugging, limit on-board resistive power supply load
    // to ~0.5W on buck supply only so we minimize chance of thermal excursion
    // or smoke leakage
    if(newLoad > LOADBANK1)
        newLoad = LOADBANK1;
#endif

    // No need to write the same value into the FIFO back-to-back
    if(I2CLoadWrite == newLoad)
        return 0;

    ret = MI2C_Write16((SLAVEADDRESS), ((unsigned int)(GPIO_REGADDR)) | (((unsigned int)(newLoad))<<8));
    if(ret == 2u)
        I2CLoadWrite = newLoad;

    return ret;
}


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
 *
 */
int MI2C_Write(unsigned int slaveAddress, void *writeData, unsigned int dataCount)
{
    I2C_WR_HDR hdr;

    // Abort if insufficient FIFO space to store the whole write transaction
    if(I2C_Tx.fifoSize - I2C_Tx.dataCount >= sizeof(hdr) + dataCount)
    {
        hdr.packetDataLen = dataCount;
        hdr.slaveAddress = slaveAddress;
        EZBL_FIFOWrite(&I2C_Tx, &hdr, sizeof(hdr));
        return EZBL_FIFOWrite(&I2C_Tx, writeData, dataCount);
    }
    return -1;
}


// Internal callback function executed anytime MI2C_Write() or
// EZBL_FIFOWrite(xxx, &I2C_Tx, xxx) is called.
//
// This callback initializes the I2C Master mode TX state machine and enables
// the MI2Cx Interrupt, if needed to start a new bus transaction from an I2C
// peripheral idle state.
unsigned int MI2C_OnWrite(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO)
{
    // If we were idle and now have enough data to start a transaction of at
    // least 0 bytes, move the header out of the FIFO and into a static
    // variable for the ISR state machine. This will be a new I2C bus
    // transaction.
    if((destFIFO->dataCount >= sizeof(I2C_WR_HDR)) && (_MI2C1IE == 0u))
    {
        EZBL_FIFORead(&lastI2CWrite, &I2C_Tx, sizeof(I2C_WR_HDR));
        transactionState = SM_I2C_START;
    }
    _MI2C1IF = 1;   // Retrigger the I2C ISR if we went idle during a transaction, waiting for more data to appear in the I2CTx FIFO
    _MI2C1IE = 1;

    return bytesPushed;
}


void __attribute__((__interrupt__, no_auto_psv)) _MI2C1Interrupt(void)
{
    unsigned int status;

    _MI2C1IF = 0;
    
    // Wait for any prior transfers to complete and the bus to be idle
    status = I2C1STAT;
    if(status & _I2C1STAT_BCL_MASK)         // Master bus collision - How dare they!
    {
        // Abort what we were doing and send a stop edge to release the bus
        I2C1CONL &= ~(_I2C1CONL_SEN_MASK | _I2C1CONL_RSEN_MASK | _I2C1CONL_RCEN_MASK | _I2C1CONL_ACKEN_MASK);
        I2C1STATbits.BCL = 0;               // Clear the collision status
        transactionState = SM_I2C_START;    // Start all over again
        return;
    }

    // Can't send anything new when already sending something
    if(I2C1CONL & (_I2C1CONL_SEN_MASK | _I2C1CONL_RSEN_MASK | _I2C1CONL_PEN_MASK | _I2C1CONL_ACKEN_MASK))
        return;
    if(I2C1STAT & (_I2C1STAT_TRSTAT_MASK | _I2C1STAT_TBF_MASK)) // Don't do anything when we are already busy transmitting
        return;

    // Decide what to transmit next
    do
    {
        switch(transactionState)
        {
            case SM_I2C_START:
                I2C1CONLbits.SEN = 1;                       // Send START condition
                packetBytesConsumed = 0;
                transactionState = SM_I2C_ADDRESS;
                return;

            case SM_I2C_ADDRESS:
                if(lastI2CWrite.slaveAddress & 0xFF80)      // Bits above 7 set, this must be a 10-bit address. If you wish to force 10-bit address mode for a 10-bit address that does not have any of the top 3 MSbits set, then simply OR in 0x8000 to the address to trigger this check
                {   // 10-bit Address
                    I2C1TRN = ((lastI2CWrite.slaveAddress>>7) | 0xF8u) & ~0x09;  // Bit 0 is the Write indicator. Bits 2 and 1 are the MSbits 9 and 8 of the 10-bit Slave address. Bits 7 to 3 are the special value '1111 10' indicating this is a 10-bit address.
                    transactionState = SM_I2C_ADDRESS10;
                }
                else
                {   // Classic 7-bit Address
                    I2C1TRN = lastI2CWrite.slaveAddress<<1; // Write mode always has the LSbit=0; the address is actually in the upper 7-bits, so we need to shift the given slave address over. Read mode would have the same shift, but with a '1' ORed into the LSbit
                    transactionState = SM_I2C_DATA;
                }
                return;

            case SM_I2C_ADDRESS10:
                // 10-bit Address bits [7:0]
                I2C1TRN = lastI2CWrite.slaveAddress & 0xFFu;
                transactionState = SM_I2C_DATA;
                return;

            case SM_I2C_DATA:
                if(status & _I2C1STAT_ACKSTAT_MASK)         // All done transmitting a data byte, so check for ACK
                {   // NACK received
                    if(packetBytesConsumed) // Were we in the address phase or the data phase?
                    {   // Data phase NACK
                        packetBytesConsumed = 0;                // Roll back for retry
                        lastI2CWrite.packetDataLen++;           // Roll back for retry
                    }
                    else
                    {
                        // Address phase NACK
                        transactionState = (lastI2CWrite.slaveAddress & 0xFF80) ? SM_I2C_ADDRESS10 : SM_I2C_ADDRESS;
                        continue;
                    }
                }

                if(packetBytesConsumed)
                {
                    EZBL_FIFORead(0, &I2C_Tx, packetBytesConsumed);    // Throw away the last peeked byte, if any
                    packetBytesConsumed = 0;
                }

                if(lastI2CWrite.packetDataLen == 0u)    // Finished sending a complete write transaction?
                {
                    I2C1CONLbits.PEN = 1;               // Send STOP condition
                    if(I2C_Tx.dataCount < sizeof(I2C_WR_HDR))
                    {   // No more transactions ready, so halt further interrupts until new work written to the FIFO
                        _MI2C1IE = 0;
                        return;                    
                    }
                    transactionState = SM_I2C_START;
                    EZBL_FIFORead(&lastI2CWrite, &I2C_Tx, sizeof(lastI2CWrite));    // Get the next action header
                    return;
                }

                if(I2C_Tx.dataCount == 0u)  // Nothing available to send right now for this write transaction, so just wait for software to catch up
                    return;

                I2C1TRN = EZBL_FIFOPeek8(&I2C_Tx);
                lastI2CWrite.packetDataLen--;
                packetBytesConsumed = 1;
                return;

            default:    // This should never happen
                __builtin_software_breakpoint();
                break;
        }
    } while(0);
}

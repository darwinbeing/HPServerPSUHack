/* 
 * File:   ezbl_uart_dual_partition.c
 *
 * Implements EZBL bootloading functionality in an Application project 
 * configured to operate in Dual Partition mode (requires flash hardware 
 * support).
 *
 * Interrupt Service Routines implemented in this project are:
 *  - One 16-bit Timer or CCT Interrupt, selected in the hardware initializer file (ISR implementation defined in ezbl_lib.a, see ezbl_lib -> weak_defaults/_TxInterrupt.s (or _CCTxInterrupt.s))
 *  - UART 2 RX (defined in ezbl_lib.a, see ezbl_lib -> weak_defaults/uart2_fifo.c)
 *  - UART 2 TX (defined in ezbl_lib.a, see ezbl_lib -> weak_defaults/uart2_fifo.c)
 */

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

#include <xc.h>
#include "ezbl_integration/ezbl.h"


// EZBL ezbl_lib.a link time options:
//EZBL_SetSYM(EZBL_NO_APP_DOWNGRADE, 1);        // 1 = Disallow programming of firmware images compiled earlier than the firmware that is already executing
EZBL_KeepSYM(EZBL_AUTO_PROG_FBTSEQ);            // Upon successful validation and programming of the entire firmware image (with communications CRC check), decrement the FBTSEQ value on the just programmed partition to cause it to be used on next reset or POR
EZBL_SetSYM(EZBL_FLOW_THRESHOLD, 24);           // Optional water mark to adjust when outbound flow control advertisements are generated. Small values consume more TX bandwidth by generating more messages. See EZBL_Install2IP() API documentation for additional information.
//EZBL_KeepSYM(EZBL_CRTReset);                  // Optionally use a slightly faster EZBL crt0 reset initializer implementation rather than XC16's

// EZBL ezbl_lib.a link time parameters for EZBL_WriteROMEx()/EZBL_VerifyROM() functions called from EZBL_Install2IP()
EZBL_SetNoProgramRange(0x000000, 0x400000);     // Since this is Dual Partition, disallow self-bricking chances and block all erase/write attempts to the active partition address range
EZBL_SetNoProgramRange(0x800000, 0xFFFFFE);     // FBOOT is read-only in RTSP, so indicate that this location can't be written to
EZBL_SetNoVerifyRange(0x800000, 0xFFFFFE);      // FBOOT may also read back differently due to mismatch between what .hex file contains and what REAL ICE/ICD/PICkit mask don't care upper bits to, so just tag these as unimportant addresses



// General Bootloader timeout governing reset idle time to initial Application
// launch and bootload termination when communications are broken.
#define BOOTLOADER_TIMEOUT          (NOW_sec * 2u)


// Uncomment for faster communications/bootloading. Buffer default size is 96
// bytes in ezbl_lib, which conserves static RAM, but reduces peak throughput.
// Any size >= 64 bytes is valid for bootloading.
unsigned char UART2_RxFifoBuffer[1024] __attribute__((noload, section(".bss.UART2_RxFifoBuffer")));


// Bootloader global variables and flags
NOW_TASK            EZBL_bootTask           __attribute__((persistent));    // Task callback pointers/timers to background execute the bootloader
EZBL_INSTALL_CTX    EZBL_bootCtx            __attribute__((persistent));    // State variables for piecewise handling a complete bootload session
unsigned long       EZBL_lastCOMActivity    __attribute__((persistent));    // Timer to detect long periods of idle and switch back to auto-baud (if enabled)
volatile int        timeForPartitionSwap    __attribute__((persistent));    // Global variable signaling when we complete a successful firmware update on the Inactive Partition

uint8_t eraseFlashStart = 0;

#if !defined(__DEBUG)
// When NOT compiled for debugging, allocate a 0x50 byte hole at the start of 
// RAM so no variables get allocated there. This provides more consistent 
// placement of variables in RAM and is important if you wish to later debug a 
// second project that you run-time partition swap into.
char __attribute__((persistent, noload, keep, unused, address(__DATA_BASE), section(".icd.reservedRAM"))) icdReservedRAM[0x50];
#endif



/**
 * Bootloader initialization code. This should execute early in the device reset
 * sequence, right at the start of main() to minimize bricking possibilities.
 *
 * Code will block for 1 second on power up (not partition swap, however) to give 
 * an opportunity to try and unbrick the device if something is severely wrong 
 * with the Application (ex: App disables global interrupts or triggers a trap 
 * right away).
 */
void EZBL_BootloaderInit(void)
{
    unsigned long startTime;

    // Initialize all persistent global/static variables manually since the CRT
    // will not initialize persistent variables and regular variables won't be
    // initialized yet since this is a priority() function.
    timeForPartitionSwap = 0;
    EZBL_NVMKey = 0;
    EZBL_COM_RX = 0;
    EZBL_COM_TX = 0;

    // Initialize I/O pins and NOW timekeeping API (requires 1 16-bit hardware timer)
    InitializeBoard();

    // Now that the clock, UART, and NOW timer are initialized, return if device
    // is programmed for Single Partition mode. This Bootloader is for Dual
    // Partition configurations. Only the clock set up, COM FIFOing, and NOW
    // timer features can be used in this bootloader while operating in Single
    // Partition mode.
    if((((unsigned int)EZBL_ReadFlash(__FBOOT_BASE)) & 0x3u) == 0x3u)
        return;
    
    unsigned long FBTSEQ = EZBL_ReadFlash(__FBTSEQ_BASE);
    unsigned long FBOOT = EZBL_ReadFlash(_FBOOT);
    unsigned long FSEC = EZBL_ReadFlash(_FSEC);
    unsigned long FOSCSEL = EZBL_ReadFlash(_FOSCSEL);
    unsigned long FOSC = EZBL_ReadFlash(_FOSC);
    unsigned long FWDT = EZBL_ReadFlash(_FWDT);
    unsigned long FPOR = EZBL_ReadFlash(_FPOR);
    unsigned long FICD = EZBL_ReadFlash(_FICD);
    unsigned long FDEVOPT1 = EZBL_ReadFlash(_FDEVOPT1);


    // Set initial state for executing EZBL_BootloaderTask() and
    // EZBL_Install2IP(). See EZBL_Install2IP() docs.
    EZBL_bootCtx.state = SM_SYNC_INIT;
    EZBL_bootCtx.timeout = BOOTLOADER_TIMEOUT;

    // Allow the Bootloader to run and block the main() application from 
    // starting for the first BOOTLOADER_TIMEOUT seconds after reset so we
    // minimize risk of bricking since this Boot code is intermingled with
    // Application code. This blocking delay is extended if someone sends us the
    // BOOTLOADER_WAKE_KEY to set EZBL_COM_RX to a non-null value (i.e. they are
    // using the bootloader).
    if(!_SFTSWP)
    {
        for(startTime = NOW_32(); EZBL_COM_RX || (NOW_32() - startTime < EZBL_bootCtx.timeout); /* no post op */)
        {
            EZBL_BootloaderTask();
        }
    }

    // Now we are going to launch the main() Application. To ensure the
    // Bootloader still gets some periodic, low priority CPU time, create the
    // scheduled NOW "thread" for it
    NOW_CreateRepeatingTask(&EZBL_bootTask, EZBL_BootloaderTask, 32u*NOW_ms);
}


/**
 * Periodically invoked callback function called at IPL 0 ("main" context) as a 
 * result of the NOW_CreateRepeatingTask() call above.
 */
int EZBL_BootloaderTask(void)
{
    if (NVMCONbits.WR == 1 ) {
        EZBL_printf("erase in progress\n");
        eraseFlashStart = 1;
    }
    else { 
        if(eraseFlashStart) {
        EZBL_printf("erase finished\n");
        eraseFlashStart = 0;
        }
    }
    // Run the main Bootload state machine
    if(EZBL_Install2IP(&EZBL_bootCtx))
    {   // Non-zero return code means a Bootload "event" has completed.
        // bootCode and appVer available right now (only now)
        timeForPartitionSwap = (EZBL_bootCtx.bootCode == EZBL_ERROR_SUCCESS);
        if(timeForPartitionSwap)
            EZBL_FIFOFlush(EZBL_COM_TX, NOW_ms*32u);    // Wait for final status code to go out; I2C could be slow because it depends on the host polling us.
    }

#if defined(XPRJ_uart) || defined(XPRJ_default) || defined(EZBL_INIT_UART)   // Defined by MPLAB X on command line when compiling "uart" or "default" Build Configurations or a EZBL_INIT_UART macro exists someplace
    // Check if COMs are idle for extended period and we should return to auto-baud mode.
    // EZBL_FIFOSetBaud() must not be called if EZBL_COMBootIF is an I2C FIFO.
    if(EZBL_COMBaud <= 0)
    {
        unsigned long now;

        now = NOW_32();
        if(EZBL_COMBootIF->activity.any)
        {   // RX activity happened, so reset auto-baud timer to full interval
            EZBL_COMBootIF->activity.any = 0x0000;
            EZBL_lastCOMActivity = now;
        }
        if((now - EZBL_lastCOMActivity > EZBL_bootCtx.timeout) || EZBL_COMBootIF->activity.framingError)
        {   // Idle timeout - switch back to default baud rate or auto-baud mode (when EZBL_COMBaud <= 0)
            EZBL_FIFOSetBaud(EZBL_COMBootIF, EZBL_COMBaud);
        }
    }
#endif

    // When Bootloader is communicating, increase task frequency for lower 
    // communications latency and better throughput. EZBL_Install2IP() sets
    // EZBL_COM_RX to null when a rejection, timeout, error or successful
    // bootload completion occurs. EZBL_COM_RX is set in the first place in the
    // communications ISR based on reception of the BOOTLOADER_WAKE_KEY
    // characters.
    if(EZBL_COM_RX)
    {
        NOW_SetNextTaskTime(&EZBL_bootTask, NOW_ms);
    }

    return 0;
}


// Callback function executed by EZBL_Install2IP() just before the Inactive
// Partition is erased, permitting last minute rejection of a firmware update.
// See callback documentation for EZBL_PreInstall() in ezbl.h for more
// information.
int EZBL_PreInstall(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer)
{
    return 1; // Default implementation: always accept valid firmware uploads
}

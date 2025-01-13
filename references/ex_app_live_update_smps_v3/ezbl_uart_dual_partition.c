/* 
 * File:   ezbl_uart_dual_partition.c
 *
 * Implements EZBL bootloading functionality in an Application project 
 * configured to operate in Dual Partition mode (requires flash hardware 
 * support).
 *
 * Interrupt Service Routines implemented in this project are:
 *  - One 16-bit Timer or CCT Interrupt, selected in the hardware initializer file (ISR implementation defined in ezbl_lib.a, see ezbl_lib -> weak_defaults/_TxInterrupt.s (or _CCTxInterrupt.s))
 *  - UART 1 RX (defined in ezbl_lib.a, see ezbl_lib -> weak_defaults/uart1_fifo.c)
 *  - UART 1 TX (defined in ezbl_lib.a, see ezbl_lib -> weak_defaults/uart1_fifo.c)
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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ezbl_integration/ezbl.h"
#include "define.h"                         // For FCY definition.
#include "lcd.h"

#if !defined(FCY)
#define FCY 70000000ul
#endif

// EZBL ezbl_lib.a link time options:
EZBL_SetSYM(EZBL_ADDRESSES_PER_SECTOR, 0x400);  // Flash page size: 0x400 program addresses = 1536 Flash bytes/512 instruction words/256 Flash double words. This is the correct size for dsPIC33EP64GS506, dsPIC33EP128GS808, PIC24FJ256GB412/GA412 family devices; PIC24FJ1024GB610/GA610 family devices use a 0x800 flash page size.
//EZBL_SetSYM(EZBL_NO_APP_DOWNGRADE, 1);        // 1 = Disallow programming of firmware images compiled earlier than the firmware that is already executing
EZBL_KeepSYM(EZBL_AUTO_PROG_FBTSEQ);            // Upon successful validation and programming of the entire firmware image (with communications CRC check), decrement the FBTSEQ value on the just programmed partition to cause it to be used on next reset or POR
EZBL_SetSYM(EZBL_FLOW_THRESHOLD, 42);           // Optional water mark to adjust when outbound flow control advertisements are generated. Small values consume more TX bandwidth by generating more messages. See EZBL_Install2IP() API documentation for additional information.
EZBL_KeepSYM(EZBL_CRTReset);                    // Optionally use a slightly faster EZBL crt0 reset initializer implementation rather than XC16's

// EZBL ezbl_lib.a link time parameters for EZBL_WriteROMEx()/EZBL_VerifyROM() functions called from EZBL_Install2IP()
EZBL_SetNoProgramRange(0x000000, 0x400000);     // Since this is Dual Partition, disallow self-bricking chances and block all erase/write attempts to the active partition address range
EZBL_SetNoProgramRange(0x800000, 0xFFFFFE);     // FBOOT is read-only in RTSP, so indicate that this location can't be written to
EZBL_SetNoVerifyRange(0x800000, 0xFFFFFE);      // FBOOT may also read back differently due to mismatch between what .hex file contains and what REAL ICE/ICD/PICkit write upper bits to (upper bits are don't cares), so just tag these as unimportant addresses


// General Bootloader timeout governing reset idle time to initial Application
// launch and bootload termination when communications are broken.
#define BOOTLOADER_TIMEOUT          (NOW_sec * 1u)


// Uncomment for faster communications/bootloading. Buffer default size is 96
// bytes in ezbl_lib, which conserves static RAM, but reduces peak throughput.
// Any size >= 64 bytes is valid for bootloading.
//unsigned char UART1_RxFifoBuffer[512] __attribute__((noload, section(".bss.UART1_RxFifoBuffer")));


// Bootloader global variables and flags
const long          EZBL_COMBaud = -230400;                                 // Communications baud rate: <= 0 means auto-baud with default value (for TX) set by the 2's complemented value; otherwise baud in bits/sec (ex: 460800)
EZBL_FIFO           *EZBL_COMBootIF         __attribute__((persistent));    // Pointer to UART or I2C RX FIFO used for bootloading (obtained from UART_Reset()/I2C_Reset() call)
NOW_TASK            EZBL_bootTask           __attribute__((persistent));    // Task callback pointers/timers to background execute the bootloader
EZBL_INSTALL_CTX    EZBL_bootCtx            __attribute__((persistent));    // State variables for piecewise handling a complete bootload session
unsigned long       EZBL_lastCOMActivity    __attribute__((persistent));    // Timer to detect long periods of idle and switch back to auto-baud (if enabled)
extern volatile uint16_t timeForPartitionSwap;                              // Global variable signaling when we complete a successful firmware update on the Inactive Partition


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

    if(!_SFTSWP)
    {    
        // Switch to FRC clock (no PLL), in case if the PLL is currently in use.
        // We are not allowed to change the PLL prescalar, postscalar or feedback
        // divider while it is running.
        __builtin_write_OSCCONH(0x00);
        __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);

        while(_OSWEN);  // Wait for clock switch to complete

        // Configure PLL for Fosc = 140MHz/Fcy = 70MIPS using 7.37 MHz internal FRC oscillator
        CLKDIV = 0xB000; // ROI = 1, DOZE = 8:1, FRCDIV = 1:1, PLLPOST = 2:1, PLLPRE = 2:1
        PLLFBD = (FCY * 2u * 2u * 2u + 7370000u / 2u) / 7370000u - 2u; // 74 @ 70 MIPS (7.37 MHz input clock from FRC)
        __builtin_write_OSCCONH(0x01);      // Initiate Clock Switch to use the FRC Oscillator + PLL (NOSC = 0b001)
        __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);

        while(_OSWEN);  // Wait for clock switch to complete
    }
    
    // Initialize NOW timekeeping API (requires 1 16-bit hardware timer)
    NOW_Reset(TMR1, FCY);

    // Initialize UART1 and two FIFOs on it (declared in ezbl_lib.a) for bootloading
    // Configure IO pins U1TX and U1RX in PPS
    IOCON3bits.PENL = 0;
    IOCON2bits.PENH = 0;
    _CNPUB12 = 1;                           // Enable Weak pull up on U1RX line so we don't receive spurious data while nobody is connected
    _U1RXR = 44;                            // U1RX on "GPIO1", TCK/PWM3L/RP44/RB12, pin 22 - dedicated
    _RP45R = _RPOUT_U1TX;                   // U1TX on "GPIO2", PWM2H/RP45/RB13, pin 23 - also controls Select for Buck/Boost Temp sense (AN7/pin 10)
    EZBL_SetSYM(UART1_TX_ISR_PRIORITY, 1);  // Optionally change the U1TX Interrupt priority. The default is IPL 1 (lowest possible priority).
    EZBL_SetSYM(UART1_RX_ISR_PRIORITY, 2);  // Optionally change the U1RX Interrupt priority. The default is IPL 2 (low priority).
    EZBL_COMBootIF = UART_Reset(1, FCY, EZBL_COMBaud, 1);

//    // Configure UART2 on PGED2/RP35/RB3 and PGEC2/RP36/RB4 for extra debug
//    // purposes (like printing console messages while bootloading). These pins
//    // are used as PGED2/PGEC2, so you must unplug any air-wired UART hardware
//    // while doing ICSP programming or debug.
//    //
//    // Function     dsPIC33EP64GS502 Device Pins
//    // UART2        PIC#, name
//    // U2RX  (in)     11, "PGD2" - PGED2/AN18/DACOUT1/INT0/RP35/RB3
//    // U2TX  (out)    12, "PGC2" - PGEC2/ADTRG31/EXTREF1/RP36/RB4
//    //_CNPUB3 = 1;            // Enable Weak pull up on U2RX line (PGD2) so we don't receive spurious data while nobody is connected
//    //_ANSB3  = 0;
//    _ANSB4  = 0;
//    _U2RXR  = 35;           // U2RX on RP35
//    _RP36R  = _RPOUT_U2TX;  // U2TX on RP36
//    UART_Reset(2, FCY, 460800, 1);
//    EZBL_ConsoleReset();
//    EZBL_printf("\n\n\nHello World! from %s()"
//                "\n  RCON = 0x%04X"
//                "\n  Current Partition = %X", __func__, RCON, 1u + _P2ACTIV);
    

    // Now that the clock, UART, and NOW timer are initialized, return if device
    // is programmed for Single Partition mode. This Bootloader is for Dual
    // Partition configurations. Only the clock set up, COM FIFOing, and NOW
    // timer features can be used in this bootloader while operating in Single
    // Partition mode.
    if((((unsigned int)EZBL_ReadFlash(__FBOOT_BASE)) & 0x3u) == 0x3u)
        return;


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
extern uint8_t uartBusIdle;

int EZBL_BootloaderTask(void)
{
    // Run the main bootload state machine
    if(EZBL_Install2IP(&EZBL_bootCtx))
    {   // Non-zero return code means a bootload "event" has completed.
        // bootCode and appVer available right now (only now)
        timeForPartitionSwap = (EZBL_bootCtx.bootCode == EZBL_ERROR_SUCCESS);

        if(EZBL_bootCtx.bootCode == EZBL_ERROR_SUCCESS_VER_GAP)
        {   // For bootload completion with non +1 minor incremental major+minor
            // appIDVersion code, do a device reset instead of attempting a hot
            // partition swap.
            lcd_printf("Version mismatch\r\nresetting\r");
            NOW_Wait(NOW_sec);
            EZBL_ResetCPU();
        }
    }

    

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
    else if(EZBL_COMBaud <= 0)  // When no bootload session in progress, check if COMs are idle for extended period and we should return to auto-baud mode
    {
        unsigned long now;

        now = NOW_32();
        if(EZBL_COMBootIF->activity.any)
        {   // RX activity happened, so reset auto-baud timer to full interval
            EZBL_COMBootIF->activity.any = 0x0000;
            EZBL_lastCOMActivity = now;
        }
        if(now - EZBL_lastCOMActivity > EZBL_bootCtx.timeout)
        {   // Idle timeout - switch back to default baud rate or auto-baud mode (when EZBL_COMBaud <= 0)
            EZBL_FIFOSetBaud(EZBL_COMBootIF, EZBL_COMBaud);
            uartBusIdle = 1;
        }
    }

    return 0;
}


// Callback function executed by EZBL_Install2IP() just before the Inactive
// Partition is erased, permitting last minute rejection of a firmware update.
// See callback documentation for EZBL_PreInstall() in ezbl.h for more
// information.
int EZBL_PreInstall(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer)
{
    // Reject same version uploads so end we don't risk state loss/corruption by
    // LiveUpdating from ourself to ourself (restriction does not apply if the
    // build number changes due to recompilation).
    if(appIDVer->appIDVer64 == EZBL_appIDVer.appIDVer64)
    {
        EZBL_FIFOWrite32(txToRemote, (((unsigned long)(EZBL_ERROR_ALREADY_INSTALLED))<<16) | 0x0000u);  // Send 0x0003 "Offered firmware image already matches the existing target firmware" back to host
        //EZBL_FIFOWrite32(txToRemote, (((unsigned long)(EZBL_ERROR_CUSTOM_MESSAGE))<<16) | 0x0000u);
        //EZBL_FIFOprintf(txToRemote, "Aborting: v%d.%02u.%04lu already installed\n", appIDVer->appIDVerMajor, appIDVer->appIDVerMinor, appIDVer->appIDVerBuild);
        return -1;  // Reject the offer
    }

    return 1; // Default: accept valid firmware uploads
}

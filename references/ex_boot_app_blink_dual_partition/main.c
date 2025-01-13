/**
 * EZBL Dual Partition Bootloader-and-Application-in-one using UART
 * communications and demonstrating push buttons to manually swap
 * partitions/write FBTSEQ.
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
#include <p24FJ1024GB610.h>
#include "ezbl_integration/ezbl.h"

void printConfigRegisters();
void printConfigRealRegistersPartition1();
void printConfigRealRegistersPartition2();
void printConfigRealCalculatedRegistersPartition1();

extern volatile int timeForPartitionSwap __attribute__((persistent));    // Global variable signaling when we complete a successful firmware update on the Inactive Partition
void EZBL_BootloaderInit(void);


int main(void)
{
    unsigned long ledBlinkTimer;
    int i;
    
    uint8_t eraseFlashStart = 0;

    EZBL_BootloaderInit();
    
    if(!_SFTSWP)
    {
        EZBL_ConsoleClear();    // Writes "Shift In", "Clear Screen" and "Reset Attributes" ANSI control codes to EZBL_STDOUT
    }
    
#if defined (__DEBUG)
    __builtin_software_breakpoint();
#endif
    
    EZBL_printf("\n"
                "\nPartition %X starting up..."
                "\n  NVMCON<SFTSWP> = %X"
                "\n  Active   Partition FBTSEQ = 0x%06lX"
                "\n  Inactive Partition FBTSEQ = 0x%06lX",
                _P2ACTIV + 1u,
                _SFTSWP,
                EZBL_ReadFlash(__FBTSEQ_BASE),
                EZBL_ReadFlash(0x400000u | __FBTSEQ_BASE));

    
    ledBlinkTimer = NOW_32();
    
    printConfigRegisters();
    
    printConfigRealRegistersPartition1();
    printConfigRealCalculatedRegistersPartition1();
    printConfigRealRegistersPartition2();
    
    while(1)
    {
        ClrWdt();
        
//        if (NVMCONbits.WR == 1 ) {
//        EZBL_printf("erase in progress\n");
//        eraseFlashStart = 1;
//        }
//        else { 
//            if(eraseFlashStart) {
//            EZBL_printf("erase finished\n");
//            eraseFlashStart = 0;
//            }
//        }

        // Every half second toggle an LED (1 Hz blink rate) to indicate we are alive
        if(NOW_32() - ledBlinkTimer > NOW_sec/2u)
        {
            LEDToggle(0x01);
            ledBlinkTimer += NOW_sec/2u;
//            EZBL_printf("\nBlinking led.\n");
        }


        if(timeForPartitionSwap)
        {
            EZBL_printf("\n\nNew firmware detected. Changing partitions now.");
            EZBL_FIFOFlush(EZBL_STDOUT, NOW_sec);       // Flush all TX status messages from printf() statements
            EZBL_PartitionSwap();                       // Perform the partition swap and branch to 0x000000 on the (presently) Inactive Partition
        }

        ButtonRead();
        if(ButtonsReleased & 0x7)       // Check if user pushed then released S4 (right-most Explorer 16/32 button)
        {
            EZBL_printf("\n\nButton push detected: swapping partitions manually");
            if(ButtonsLastState & 0x8)  // Also check if user is holding down S3 (left-most Explorer 16/32 button)
            {
                EZBL_printf("\nAlso second button held:"
                            "\n  Decrementing FBTSEQ on Inactive Partition so it is reset active...");
                i = EZBL_WriteFBTSEQ(0, 0, -1);
                EZBL_printf(i == 1 ? "success" : "failed (%d)", i);
                // NOTE: if you want to change the EZBL_WriteFBTSEQ() call to
                // program the Active Partition's FBTSEQ value, you must remove
                // FBTSEQ's address on the Active Partition from this line at
                // the top of ezbl_uart_dual_partition.c:
                //     EZBL_SetNoProgramRange(0x000000, 0x400000);
            }
            EZBL_FIFOFlush(EZBL_STDOUT, NOW_sec);       // Flush all TX status messages from printf() statements
            EZBL_PartitionSwap();                       // Perform the partition swap and branch to 0x000000 on the (presently) Inactive Partition
        }
    }
}


// Optional trap handler.
//
// For a trap handler that dumps useful debugging data to stdout (such as the
// exception return address, stack and frame pointers, stack contents,
// w-registers, RAM contents, etc.), delete this _DefaultInterrupt() ISR and
// instead use (at file-level scope):
//     EZBL_KeepSYM(EZBL_TrapHandler);
// Ensure that UART_Reset() is called before any trap occurs to initialize the
// EZBL_STDOUT pointer and that interrupts for the FIFO stay forwarded to the
// correct ISR handler (communications ISRs are polled when masked by IPL8+ to
// still TX progression.
void __attribute__((interrupt, no_auto_psv)) _DefaultInterrupt(void)
{
    // If this interrupt is unexpected, you should check all of the INTCON1 bits
    // to see if a trap is responsible.
    __builtin_software_breakpoint();

    // Clear trap(s), if any, so we can return to see where the PC was when this
    // interrupt was triggered
    INTCON1 &= 0x8700;
}


void printConfigRegisters(){
    EZBL_printf("\n  _FSEC initial config = 0x%04X", BWRP_OFF & BSS_OFF & BSEN_OFF & GWRP_OFF & GSS_OFF & CWRP_OFF & CSS_DIS & AIVTDIS_DISABLE);
    EZBL_printf("\n  _FOSCSEL initial config = 0x%04X", FNOSC_FRC & PLLMODE_PLL96DIV2 & IESO_OFF);
    EZBL_printf("\n  _FOSC initial config = 0x%04X", POSCMD_XT & OSCIOFCN_ON & SOSCSEL_ON & PLLSS_PLL_PRI & IOL1WAY_OFF & FCKSM_CSECME);
    EZBL_printf("\n  _FWDT initial config = 0x%04X", WDTPS_PS1024 & FWPSA_PR32 & SWON & WINDIS_OFF & WDTCMX_LPRC & WDTCLK_LPRC);
    EZBL_printf("\n  _FPOR initial config = 0x%04X", BOREN_ON & LPCFG_ON & DNVPEN_ENABLE);
    EZBL_printf("\n  _FICD initial config = 0x%04X", PGX2 & JTAGEN_OFF & BTSWP_ON);
    EZBL_printf("\n  _FDEVOPT1 initial config = 0x%04X", ALTCMPI_DISABLE & TMPRPIN_OFF & SOSCHP_ON & ALTVREF_ALTVREFDIS);
    EZBL_printf("\n  _FBOOT initial config = 0x%04X", BTMODE_DUAL);  
    EZBL_printf("\n  _RCON = 0x%04X", RCON);  
}

void printConfigRealRegistersPartition1(){
    EZBL_printf("\n  ------------------------------------- ");
    EZBL_printf("\n  _FSEC real config = 0x%04X", EZBL_ReadFlash(0x055f00));
    EZBL_printf("\n  _FOSCSEL real config = 0x%04X", EZBL_ReadFlash(_FOSCSEL));
    EZBL_printf("\n  _FOSC real config = 0x%04X", EZBL_ReadFlash(_FOSC));
    EZBL_printf("\n  _FWDT real config = 0x%04X", EZBL_ReadFlash(_FWDT));
    EZBL_printf("\n  _FPOR real config = 0x%04X", EZBL_ReadFlash(_FPOR));
    EZBL_printf("\n  _FICD real config = 0x%04X", EZBL_ReadFlash(_FICD));
    EZBL_printf("\n  _FDEVOPT1 real config = 0x%04X", EZBL_ReadFlash(_FDEVOPT1));
    EZBL_printf("\n  _FBOOT real config = 0x%04X", EZBL_ReadFlash(_FBOOT));  
}

void printConfigRealCalculatedRegistersPartition1(){
    EZBL_printf("\n  ------------------------------------- ");
//    EZBL_printf("\n  _FSEC real config = 0x%04X", EZBL_ReadFlash(_FSEC));
//    EZBL_printf("\n  _FOSCSEL real config = 0x%04X", EZBL_ReadFlash(_FOSCSEL));
//    EZBL_printf("\n  _FOSC real config = 0x%04X", EZBL_ReadFlash(_FOSC));
//    EZBL_printf("\n  _FWDT with calculated address 1 = 0x%04X", EZBL_ReadFlash(0x0A0040BE));
//    EZBL_printf("\n  _FWDT with calculated address 2 = 0x%04X", EZBL_ReadFlash(0x0A40BE));
    EZBL_printf("\n  _FWDT with calculated address 3 = 0x%04X", EZBL_ReadFlash(0x40BE));
//    EZBL_printf("\n  _FPOR real config = 0x%04X", EZBL_ReadFlash(_FPOR)); 
//    EZBL_printf("\n  _FICD real config = 0x%04X", EZBL_ReadFlash(_FICD));
//    EZBL_printf("\n  _FDEVOPT1 real config = 0x%04X", EZBL_ReadFlash(_FDEVOPT1));
//    EZBL_printf("\n  _FBOOT real config = 0x%04X", EZBL_ReadFlash(_FBOOT));  
}

void printConfigRealRegistersPartition2(){
    EZBL_printf("\n  ------------------------------------- ");
    EZBL_printf("\n  _FSEC real config = 0x%04X", EZBL_ReadFlash(_FSEC+0x400000));
    EZBL_printf("\n  _FOSCSEL real config = 0x%04X", EZBL_ReadFlash(_FOSCSEL+0x400000));
    EZBL_printf("\n  _FOSC real config = 0x%04X", EZBL_ReadFlash(_FOSC+0x400000));
    EZBL_printf("\n  _FWDT real config = 0x%04X", EZBL_ReadFlash(_FWDT+0x400000));
    EZBL_printf("\n  _FPOR real config = 0x%04X", EZBL_ReadFlash(_FPOR+0x400000));
    EZBL_printf("\n  _FICD real config = 0x%04X", EZBL_ReadFlash(_FICD+0x400000));
    EZBL_printf("\n  _FDEVOPT1 real config = 0x%04X", EZBL_ReadFlash(_FDEVOPT1+0x400000));
    EZBL_printf("\n  _FBOOT real config = 0x%04X", EZBL_ReadFlash(_FBOOT+0x400000));
}

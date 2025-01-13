////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Microchip Technology Inc.
//
// MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
// derivatives created by any person or entity by or on your behalf, exclusively
// with Microchip's products.  Microchip and its licensors retain all ownership
// and intellectual property rights in the accompanying software and in all
// derivatives here to.
//
// This software and any accompanying information is for suggestion only.  It
// does not modify Microchip's standard warranty for its products.  You agree
// that you are solely responsible for testing the software and determining its
// suitability.  Microchip has no obligation to modify, test, certify, or
// support the software.
//
// THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
// EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
// PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
// COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
//
// IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
// (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
// INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
// EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
// ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
// MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
// TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
// CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
// FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
//
// MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
// TERMS.
////////////////////////////////////////////////////////////////////////////////

#include "isr.h"
#include "../i2c.h"

extern volatile uint16_t __attribute__((near)) criticalISRDone;

extern uint16_t LCD_BoostLoad, LCD_BuckLoad;

extern int16_t buckErrorHistory3P3Z[4]    __attribute__ ((space(ymemory), far, preserved));
extern int16_t buckControlHistory3P3Z[3]  __attribute__ ((space(ymemory), far, preserved));

extern int16_t boostErrorHistory[4]   __attribute__ ((space(ymemory), far, preserved));
extern int16_t boostControlHistory[3] __attribute__ ((space(ymemory), far, preserved));

// Variables written in ISRs, so are declared volatile to not optimize read statements away when a cached value already exists in a CPU register
volatile uint16_t buckVoltage  = 0, buckCurrent  = 0, inputVoltage = 0;
volatile uint16_t boostVoltage = 0, boostCurrent = 0;
volatile uint16_t tempSense    = 0;

int16_t __attribute__ ((preserved)) buckControlReference = 0, boostControlReference = 0;
DPSK_FAULTS __attribute__ ((preserved)) dpskFaultFlags;
DPSK_FLAGS  __attribute__ ((preserved)) dpskFlags;


// Timer for Dynamic Load switching after soft start and disabling/restoring
// loads due to fault conditions (ex: over-temperature shutdown/restart,
// under/over voltage shutdown/restart)
// Executes at IPL 4
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt()
{
    unsigned char newBuckLoad = I2CBUCKLOAD1WRITE;   // Default light loads when doing soft start or not supporting dynamic load switching
    unsigned char newBoostLoad = I2CBOOSTLOAD1WRITE;
    unsigned char newLoads;
    
    #if(BUCKDYNAMICLOAD == ENABLED)
    if(dpskFlags.buckSSActive == DISABLED)
    {
        // Code to switch on-board load for dynamic load testing
        if(dpskFlags.buckDynLoad)
            newBuckLoad  = I2CBUCKLOAD2WRITE;
        LCD_BuckLoad = (dpskFlags.buckDynLoad ? BUCKLOAD1LCD : BUCKLOAD2LCD);
        dpskFlags.buckDynLoad ^= 1;
    }
    #endif

    #if(BOOSTDYNAMICLOAD == ENABLED)
    if(dpskFlags.boostSSActive == DISABLED)
    {
        // Code to switch on-board load for dynamic load testing
        if(dpskFlags.boostDynLoad)
            newBoostLoad = I2CBOOSTLOAD2WRITE;
        LCD_BoostLoad = (dpskFlags.boostDynLoad ? BOOSTLOAD1LCD : BOOSTLOAD2LCD);
        dpskFlags.boostDynLoad ^= 1;
    }
    #endif


    // Fault overrides
    if(dpskFaultFlags.buckSoftStart)    // Buck soft start regulation fault, terminate buck loading
        newBuckLoad = 0;
    if(dpskFaultFlags.boostSoftStart)   // Boost soft start regulation fault, terminate boost loading
        newBoostLoad = 0;

    // Combine both loads and override both loads to full off if global fault exists
    newLoads = newBoostLoad | newBuckLoad;
    if(dpskFaultFlags.overTemp)         // Over maximum temperature
        newLoads = 0;
    if(dpskFaultFlags.inputVoltage)     // Input voltage above or below acceptable range
        newLoads = 0;

    // Write the new load configuration. Function will ignore newLoads if it is
    // the same as the last written load value.
    I2CSetResistorLoad(newLoads);

    _T3IF = 0;
}


// Interrupt for ADC Digital Comparator - handles temperature related
// warning and shutdown/restart of loads
// Executes at IPL 4
void __attribute__((__interrupt__, no_auto_psv)) _ADCMP0Interrupt()
{
    volatile int dummy;
  
    dummy = ADCMP0CONbits.CHNL;     /* Clears logic so not to enter multiple times */

    // See if we were last monitoring for the temperature warning threshold
    if(ADCMP0HI == WARNINGTEMPADC)
    {
        // Change digital cmp settings
        ADCMP0CONbits.LOLO = 1;     // Begin interrupts for for ADCBUF7 < ADCMP0LO (TEMPREINITADC)
        ADCMP0LO = TEMPREINITADC;   // Monitor for temperature dropping into the allowed operation range
        ADCMP0HI = TEMPMAXADC;      // Raise high temperature monitoring limit to the over-temp shutdown threshold

        if(dpskFaultFlags.overTemp == 0)
        {
            dpskFaultFlags.tempWarning = 1;
        }
    }

    // Temperature too high for safe operation? If so shutdown all loads and the supply to protect ourselves.
    if (ADCBUF7 >= TEMPMAXADC)
    {
       // Change digital cmp settings
        ADCMP0CONbits.HIHI         = 0;     // End interrupts for excessive temperature; we are shutting down until ADCBUF7 < TEMPREINITADC (ADCMP0LO w/LOLO == '1')

        dpskFaultFlags.overTemp    = 1;     // Disables all loads on next load write
        dpskFaultFlags.tempWarning = 0;
    }
    else if (ADCBUF7 <= TEMPREINITADC)      // Temperature dropped below the minimum needed to keep operating normally
    {
        // Clear over-temp warnings and shutdown states. Reset monitoring against the WARNINGTEMPADC upper limit.
        ADCMP0CONbits.LOLO = 0;             // End interrupts for ADCBUF7 < TEMPREINITADC (ADCMP0LO w/LOLO == '1')
        ADCMP0CONbits.HIHI = 1;             // Restore interrupts for ADCBUF7 >= WARNINGTEMPADC (ADCMP0HI)
        ADCMP0HI = WARNINGTEMPADC;

        dpskFaultFlags.tempWarning = 0; // Clear bit to disable 'HI TEMP WARNING' on display
        dpskFaultFlags.overTemp = 0;    // Clear bit to disable 'OVER TEMP FAULT' on display and restore loads on next load write (assuming no other faults)
    }
   _ADCMP0IF = 0;
}


// ADC Digital Comparator Interrupt - disables PWMs and loads for input
// overvoltage and restores PWMs with soft start + loads when back in range
// Executes at IPL 5
void __attribute__((__interrupt__, no_auto_psv)) _ADCMP1Interrupt()
{
    volatile int dummy;
  
    dummy = ADCMP1CONbits.CHNL;     /* Clears logic so not to enter multiple times */
    
    if(ADCMP1HI == INPUTOVERVOLTAGEADC) // Comparator last configured to monitor INPUTUNDERVOLTAGEADC and INPUTOVERVOLTAGEADC, making this interrupt indicate an excursion
    {
        IOCON1bits.OVRENH = 1;          // Override the PWM1H/L - Buck FETs to inactive state
        IOCON1bits.OVRENL = 1;
        IOCON2bits.OVRENL = 1;          // Override the PWM2L - Boost FET to inactive state

        _ADCAN1IE = 0;                  // Disable Buck compensator ISR
        _ADCAN3IE = 0;                  // Disable Boost compensator ISR
        PDC1 = 0;
        PDC2 = 0;

        dpskFaultFlags.inputVoltage = 1;// Forces all loads off on next load write

        // Change digital comparator settings to look for a narrowed voltage range to give restart hysteresis
        ADCMP1CONbits.HIHI = 0;
        ADCMP1CONbits.LOLO = 0;
        ADCMP1HI = INPUTVOLTMAXHYST;
        ADCMP1LO = INPUTVOLTMINHYST;
        ADCMP1CONbits.BTWN = 1;             // 1 = Generates a comparator event when ADCMPnLO <= ADCBUFx < ADCMPnHI
    }
    else    // Input voltage now in acceptable range
    {
        // Change digital comparator settings back to the wider undervoltage/overvoltage shutdown thresholds
        ADCMP1CONbits.BTWN = 0;             // 0 = Does not generate a digital comparator event when ADCMPnLO <= ADCBUFx < ADCMPnHI
        ADCMP1HI = INPUTOVERVOLTAGEADC;
        ADCMP1LO = INPUTUNDERVOLTAGEADC;
        ADCMP1CONbits.HIHI = 1;             // 1 = Generates a digital comparator event when ADCBUFx >= ADCMPnHI
        ADCMP1CONbits.LOLO = 1;             // 1 = Generates a digital comparator event when ADCBUFx < ADCMPnLO

        dpskFaultFlags.inputVoltage = 0;    // Restores loads on next load write (unless other faults exist)

        #if(BUCK == ENABLED)
            MACRO_CLR_BUCKHISTORY();
            buckControlReference   = 0;

            _ADCAN1IE              = 1;     // Restore Buck compensator
            IOCON1bits.OVRENH      = 0;     // Remove Override
            IOCON1bits.OVRENL      = 0;

            dpskFlags.buckSSActive = 1;     // Reinitiate softstart
        #endif

        #if(BOOST == ENABLED)
            MACRO_CLR_BOOSTHISTORY();

            // SW Trigger AN3 for initial Boost output voltage Measurement
            ADTRIG0Hbits.TRGSRC3 = 1;
            ADCON3Lbits.SWCTRG   = 1;
            Nop();
            while(!_AN3RDY);
            boostControlReference = ADCBUF3;

            ADTRIG0Hbits.TRGSRC3 = 6;       // Change trigger back to PWM2
            _ADCAN3IE            = 1;       // Restore Boost compensator
            IOCON2bits.OVRENL    = 0;       // Remove Override

            dpskFlags.boostSSActive = 1;    // Reinitiate softstart
         #endif

        // Fire up soft start timer
        TMR2          = 0;
        T2CONbits.TON = 1;
    }

    _ADCMP1IF = 0;
}

// ADC AN7 interrupt sampling input voltage & temperature
// Executes at IPL 5
void __attribute__((__interrupt__, no_auto_psv)) _ADCAN7Interrupt()
{
    // Read Vin and Temperature for LCD display, fault management performed
    // in digital comparator ISRs
    
    inputVoltage = ADCBUF4;
    tempSense    = ADCBUF7;

    _ADCAN7IF    = 0;
}


// Timer interrupt for PWM softstart. If regulation seems to be taking too long,
// terminate switching, log output fault, eliminate loading
// Executes at IPL 4
static uint8_t buckErrorCount = 0;
static uint8_t boostErrorCount = 0;
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt()
{
    #if(BUCK == ENABLED)
    //static uint8_t buckErrorCount = 0;   // Moved to file scope for Live Update compiler tracking

    if(dpskFlags.buckSSActive)
    {
        if(buckControlReference < BUCKVOLTAGEREFERENCEADC)
        {
            buckControlReference += BUCKINCREMENT;

            if((buckErrorHistory3P3Z[0]) > (BUCK_SS_ERROR)) // Used to check feedback loop (is feedback loop open?)
            {                                               // and ensure that output follows Vreference
                if(++buckErrorCount > 5u)
                {
                    // Disable Buck converter PWMs
                    buckErrorCount = 0;
                    IOCON1bits.OVRENH = 1;                  // Override PWM1H to inactive state
                    IOCON1bits.OVRENL = 1;                  // Override PWM1L to inactive state
                    _ADCAN1IE = 0;
                    dpskFaultFlags.buckSoftStart = 1;       // Display: Bck Output Fault and terminate buck loading on next load write
                    dpskFlags.buckSSActive = 0;
                }
            }
        }
        else    // Soft-start is complete
        {
            buckErrorCount = 0;
            buckControlReference = BUCKVOLTAGEREFERENCEADC;
            dpskFlags.buckSSActive = 0;
            dpskFaultFlags.buckSoftStart = 0;               // Clear logged soft start fault, if any (could exist due to input over/under voltage fault at initial power up)
        }
    }
    #endif

    #if(BOOST == ENABLED)
    //static uint8_t boostErrorCount = 0;   // Moved to file scope for Live Update compiler tracking

    if(dpskFlags.boostSSActive)
    {
        if(boostControlReference < BOOSTVOLTAGEREFERENCEADC)
        {
            boostControlReference += BOOSTINCREMENT;

            if((boostErrorHistory[0]) > (BOOST_SS_ERROR))       // Used to check feedback loop (is feedback loop open?)
            {                                                   // and ensure that output follows Vreference
                if(++boostErrorCount > 5u)
                {
                    // Disable Boost converter PWM
                    boostErrorCount = 0;
                    IOCON2bits.OVRENL = 1;                      // Override PWM2L to inactive state
                    _ADCAN3IE = 0;
                    dpskFaultFlags.boostSoftStart = 1;          // Display: Bst Output Fault and terminate boost loading on next load write
                    dpskFlags.boostSSActive = 0;
                }
            }
        }
        else    // Soft-start is complete
        {
            boostErrorCount = 0;
            boostControlReference = BOOSTVOLTAGEREFERENCEADC;
            dpskFlags.boostSSActive = 0;
            dpskFaultFlags.boostSoftStart = 0;                  // Clear logged soft start fault, if any (could exist due to input over/under voltage fault at initial power up)
        }
    }

#endif


   // Disable Timer once both softstart flags are disabled
   if(!dpskFlags.buckSSActive && !dpskFlags.boostSSActive)
   {
       T2CONbits.TON = 0;
   }

    _T2IF = 0;
}

/*********************************************************************
 * Function:        void __attribute__((weak, vector(_UARTx_VECTOR), interrupt(IPLrxSOFT), keep)) UxRXInterrupt(void)
 *                  void _ISR _UxRXInterrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Records hardware overflow/framing/etc errors
 *                  into UART1_RxFifoErrors.
 *
 * Side Effects:    None
 *
 * Overview:        Receives a physical RX byte from the UART and
 *                  places it in a local RAM FIFO for software to
 *                  read it at its leisure.
 *
 * Note:            Executes at IPL 2
 ********************************************************************/

// Exact match RX character sequence to trigger assignment of EZBL_COM_RX to
// this UART. Comment this line out to disable auto-bootloader wake up on this UART.
#define BOOTLOADER_WAKE_KEY     {0x434D, 0x5055, 0x4348, 0x454D} //{'M','C','U','P','H','C','M','E'}
unsigned int UART1_wakeKeyIndex;    // Bootloader wake up key decoder position

uint8_t uartBusIdle = 0;

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
    uartBusIdle = 0;
    U1STAbits.UTXINV = 0;
    
    unsigned int i;
    unsigned int status;
    EZBL_COM_ACTIVITY newFlags = {.any = EZBL_COM_ACTIVITY_RX_MASK};    // Set activity.rx = 1 always

    // Clear the interrupt flag so we don't keep entering this ISR
    _U1RXIF = 0;

    // Read all available bytes. This is checked before reading anything because
    // in the event of an auto-baud 0x55 reception, we'll get this RX interrupt,
    // but won't actually have any data to put into the software RX FIFO.
    while(U1STAbits.URXDA)
    {
        // Get the byte
        i = U1RXREG;
        
        // Collect any Parity (not used, so never should be set), Framing, 
        // and Overrun errors. Parity/Framing errors persist only while the 
        // applicable byte is at the top of the hardware RX FIFO, so we need to 
        // collect it for every byte.
        status = U1STA;
        if(status & _U1STA_OERR_MASK)
        {
            U1STAbits.OERR = 0;    // Clear overflow flag if it it has become set. This bit prevents future reception so it must be cleared.
            newFlags.rxOverflow = 1;
        }
        if(status & _U1STA_FERR_MASK)
            newFlags.framingError = 1;
        if(status & _U1STA_PERR_MASK)
            newFlags.parityError = 1;

#if defined(BOOTLOADER_WAKE_KEY)
        {
            // Check if Bootloader needs waking up
            short bootloaderWakeKey[] = BOOTLOADER_WAKE_KEY; // An exact-match RX string to allow EZBL Bootloader code to come alive and start Bootloader command processing when the Application is running.

            UART1_wakeKeyIndex &= 0x0007;                    // Restrict index to 0-7 so we don't have to bounds check if no crt data initialization is performed (saves perhaps 200 bytes of flash to disable this linker option)
            if(((char)i) == ((char*)bootloaderWakeKey)[UART1_wakeKeyIndex])
            {   // Might be external node trying to contact Bootloader, so decode the "MCUPHCME" first few bytes to decide if we should activate Bootloader command processing or not
                if(++UART1_wakeKeyIndex >= sizeof(bootloaderWakeKey))
                {   // All 8 bytes matched exactly, enable the EZBL command processor task
                    newFlags.bootkeyRx = 1;
                    EZBL_COM_RX = &UART1_RxFifo;
                    EZBL_COM_TX = &UART1_TxFifo;
                }
            }
            else
            {
                UART1_wakeKeyIndex = 0;
                if(((char)i) == ((char*)bootloaderWakeKey)[0])
                    UART1_wakeKeyIndex = 1;
            }
        }
#endif

        // Throw this byte away if it would cause overflow
        if(UART1_RxFifo.dataCount >= UART1_RxFifo.fifoSize)
        {
            newFlags.rxOverflow = 1;
            continue;
        }

        // Copy the byte into the local FIFO
        // NOTE: The FIFO internal data structures are being accessed directly
        // here rather than calling the UART1_RX_FIFO_Write*() functions because
        // any function call in an ISR will trigger a whole lot of compiler
        // context saving overhead. The compiler has no way of knowing what
        // registers any given function will clobber, so it has to save them
        // all. For efficiency, the needed write-one-byte code is duplicated
        // here.
        *UART1_RxFifo.headPtr++ = (unsigned char)i;
        if(UART1_RxFifo.headPtr >= UART1_RxFifo.fifoRAM + UART1_RxFifo.fifoSize)
        {
            UART1_RxFifo.headPtr = UART1_RxFifo.fifoRAM;
        }
        EZBL_ATOMIC_ADD(UART1_RxFifo.dataCount, 1);
    }
    
    // Update all the activity flags we accumulated, including RxFifo.activity.rx = 1
    UART1_RxFifo.activity.any |= newFlags.any;
}

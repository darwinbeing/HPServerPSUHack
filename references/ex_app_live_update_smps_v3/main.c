////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018 Microchip Technology Inc.
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

#include <xc.h>
#include <stdio.h>
#include "main.h"
#include "i2c.h"
#include "ezbl_integration/ezbl.h"
#include "buck_defines.h"


// Set power up device Configuration Words
EZBL_SET_CONF(_FSEC, BWRP_OFF & BSS_DISABLED & BSEN_OFF & GWRP_OFF & GSS_DISABLED & CWRP_OFF & CSS_DISABLED & AIVTDIS_OFF)
EZBL_SET_CONF(_FOSCSEL, FNOSC_FRC & IESO_OFF)
EZBL_SET_CONF(_FOSC, POSCMD_NONE & FCKSM_CSECME & OSCIOFNC_ON & IOL1WAY_OFF & PLLKEN_ON)
EZBL_SET_CONF(_FWDT, WDTPOST_PS1024 & WDTPRE_PR32 &  WDTEN_ON_SWDTEN)   // ~1 second watchdog, but only when software sets _SWDTEN = 1
EZBL_SET_CONF(_FDEVOPT, PWMLOCK_OFF)
EZBL_SET_CONF(_FALTREG, CTXT1_IPL7 & CTXT2_IPL6)
#if defined(__DUAL_PARTITION) && defined(__DEBUG)
EZBL_SET_CONF(_FICD, ICS_PGD2 & JTAGEN_OFF & BTSWP_ON & 0xFF7F) // Bit 7 = '0' enables the PGECx/PGEDx pins to maintain debug session if code programmed via bootloader (normally bit 7 is overridden during ICSP programming for debug, so doesn't need touching). Bit 7 = '1' disables the debugger (use for production).
#else   // Single partition or Production mode
EZBL_SET_CONF(_FICD, ICS_PGD2 & JTAGEN_OFF & BTSWP_ON)
#endif
#if defined(__DUAL_PARTITION)
EZBL_SET_CONF(_FBOOT, BTMODE_DUAL)  // Default is BTMODE_SINGLE, BTMODE_DUAL is for ping-ponging, BTMODE_DUALPROT is for statically write-protecting Partition 1, BTMODE_DUALPRIV is reserved/do not use
//EZBL_SET_CONF_FBTSEQ(~0)          // Use this is if you need to explicitly set the FBTSEQ value in code. Generally, you should NOT have this defined as it will be computed at run-time based on the programming order and predefined values will force an erase-modify-write of the whole Flash page contents.
#endif


// Bootloader prototypes
void EZBL_BootloaderInit(void);

volatile uint16_t                                   timeForPartitionSwap = 0;   // Flag indicating the bootloader successfully programmed the Inactive Partition with a new firmware image
volatile uint16_t __attribute__((near))             criticalISRDone;            // Flag indicating best window of time for issuing a partition swap
unsigned int __attribute__((preserved, persistent)) partitionSwapTimestamp;     // NOW_16() value shortly before disabling critical interrupts to partition swap
uint16_t __attribute__((persistent))                LCDTime1, LCDTime2;         // Partition swap timing measurements: LCDTime1 = control loop resume latency, LCDTime2 = main(), while(1) resume latency

uint8_t  LCDselect = 0, faultDisplayCnt = 0;
uint16_t LCD_BuckLoad = 0, LCD_BoostLoad = 0;

uint8_t __attribute__((persistent)) liveUpdateOccuredFlag;

extern volatile uint16_t buckVoltage, boostVoltage, inputVoltage, tempSense;

extern int16_t     __attribute__((preserved)) boostControlReference;
extern DPSK_FLAGS  __attribute__((preserved)) dpskFlags;
extern DPSK_FAULTS __attribute__((preserved)) dpskFaultFlags;


//EZBL_KeepSYM(EZBL_TrapHandler); // More advanced trap handler with lots of debug data printed to EZBL_STDOUT
void __attribute__((interrupt, no_auto_psv)) _DefaultInterrupt(void)
{
    __builtin_software_breakpoint();

    // Clear trap(s), if any, so we can return to see where the PC was when this
    // interrupt was triggered
    INTCON1 &= 0x8700;
}


void __attribute__((priority(100), optimize(1))) CriticalInit(void)
{
    // Don't execute any of this code unless we've done a bootswap. Ordinarily 
    // reset initialization is handled in main().
    if(!_SFTSWP)    
        return;

    // Reinitialize anything time critical that needs to change relative to the 
    // last application in a live-update situation. This is the highest priority 
    // initializer and will execute before any non-priority(1) variables have 
    // been written by the CRT, so use care to not reference any 'update' or 
    // non-'preserved' variables in this code (or the ISRs enabled by this 
    // function).
    
    // Show on an I/O pin that we are now executing on the previously Inactive 
    // Partition and take a timestamp so we can calculate exactly how long it 
    // took to get here.
    _LATB4 = 0;
    LCDTime1 = TMR1 - partitionSwapTimestamp;   // Elapsed instruction cycles
    
    // Turn on compensator ISR(s) since we didn't actually update the code for these in ex_app_live_update_smsps_v3 (relative to v2)
    _ADCAN3IE = 1;  // Boost compensator
    _ADCAN1IE = 1;  // Buck compensator
}


void __attribute__((priority(200))) SecondaryInit(void)
{
    // Don't execute any of this code unless we've done a bootswap. Ordinarily 
    // reset initialization is handled in main().
    if(!_SFTSWP)    
        return;

    // Turn on individual interrupts for other high-priority functions. All of 
    // the peripheral IECx<yyyxIE> flags were turned off before partition 
    // swapping, so we can optionally reconfigure them here following the same 
    // rules of not making dependencies on any variables that have not been 
    // preserved or have a higher or unspecified 'priority' attribute.
    
    // High priority but non-critical reinitialization code here. We didn't 
    // change the code for these other interrupts and will have preserved RAM 
    // state, so reinitialization entails just restoring the ISRs.
    _ADCMP0IE = 1;  // ADC Digital Comparator 0 Warning and Over-temperature detect code
    _ADCMP1IE = 1;  // ADC Digital Comparator 1 Input overvoltage/undervoltage detect code 
    _ADCAN7IE = 1;  // ADC AN7 Input Voltage and Temperature capture code

    liveUpdateOccuredFlag = 1;
    
    _LATB4 = 1;    // Signal normal critical partition swap ISR down-time has ended (for debug only)         
}



int main(void)
{
    unsigned long lcdRefreshTimer;

    // Check if we are entering main via an ordinary device reset or via a hot 
    // Bootloader partition swap
    if(!_SFTSWP)    // Ordinary reset initialization - do not execute for live update
    {
        liveUpdateOccuredFlag = 0;
        
        // Initialize the bootloader as the very first thing to minimize bricking 
        // possibilities. This will block for 1 second to give a small bootloading 
        // "guaranteed" window.
        EZBL_BootloaderInit();

        dpskFaultFlags.systemFaults = 0;    // Clear all power supply flags

        InitClock();            // Init Primary and Auxiliary oscillators
        InitI2C();
        InitTimer();
        InitADC();              // Measure Voltage/Currents
        InitCMP();
        InitADCMP();

        _ANSB4  = 0;
        _TRISB4 = 0;        // Set RB4 as output - going to use as partition swap indicator

        _ANSB3 = 0;
        _TRISB3 = 0;        // Set RB3 as output - going to use as main control loop ISR indicator

        // This function also displays initial startup messages
        LCD_Init();             // Initialize LCD

        // Setup PWM resolution and Period
        PTPER = PERIODVALUE;    // Switching Frequency of Buck/Boost Converters
        PTCON2bits.PCLKDIV = (PWMRESOLUTION_REG-1); // PWM Resolution

        InitPWM3();

        #if (BUCK == ENABLED)
        InitBuckPWM();          // PWM Setup for Buck converter
        InitBuckComp();         // Initialize Buck compensator
        dpskFlags.buckSSActive = 1;
        #else
        // Need to clear IOCON to ensure PWML is not 100% DC as PENL enabled at
        // device reset
        IOCON1 = 0x0000;
        #endif

        #if (BOOST == ENABLED)
        InitBoostPWM();             // PWM Setup for Boost converter
        InitBoostComp();            // Initialize Boost compensator
        dpskFlags.boostSSActive = 1;
        #else
        // Need to clear IOCON to ensure PWML is not 100% DC as PENL enabled at
        // device reset
        IOCON2 = 0x0000;
        #endif

        PTCONbits.PTEN = 1;         // Enable the PWM
        NOW_Wait(NOW_us*10u);

        #if (BUCK == ENABLED)
        // To eliminate PWM glitch at start-up (Errata), enable PWM module
        // and then give PWM ownership of the pin
        IOCON1bits.PENH  = 1;       // PWM1H is controlled by PWM module
        IOCON1bits.PENL  = 1;       // PWM1L is controlled by PWM module

#if defined(__DEBUG)
        IOCON1bits.PENH  = 0;       // PWM1H is DISABLED
        IOCON1bits.PENL  = 0;       // PWM1L is DISABLED
#endif

        _ADCAN1IE = 1;              // Enable ADC AN1 interrupt
        ADIELbits.IE1 = 1;
        #endif

        #if(BOOST == ENABLED)
        IOCON2bits.PENL  = 1;               // PWM2L is controlled by PWM module
#if defined(__DEBUG)
        IOCON2bits.PENL  = 0;               // PWM2L is DISABLED
#endif

        // SW Trigger AN3 for initial Boost output voltage Measurement
        ADCON3Lbits.SWCTRG = 1;
        Nop();
        while(_AN3RDY != 1);
        boostControlReference = ADCBUF3;
        _TRGSRC3              = 6;          // Change trigger back to PWM2
        _ADCAN3IE             = 1;          // Enable AN3 interrupt
        ADIELbits.IE3         = 1;
        #endif
        
        // Initial load settings to be displayed on LCD
        #if(BOOSTLOAD1 != EXTERNALLOAD)
        LCD_BoostLoad = BOOSTLOAD1LCD;
        #endif

        #if(BUCKLOAD1 != EXTERNALLOAD)
        LCD_BuckLoad = BUCKLOAD1LCD;
        #endif

        // Enable Timer 2 for SoftStart
        T2CONbits.TON = 1;
    }
    else
    {   // Live Update - entered main() via partition swap

        EZBL_BootloaderInit();
        _T2IE = 1;      // Softstart code interrupt enabled (if needed)
        InitI2C();      // Reinitialize I2C 1 Master-mode dynamic load switching communications code
        _T3IE = 1;      // Timer 3 Dynamic load switching code
        _SFTSWP = 0;
        
        _LATB4 = 0;
        LCDTime2 = TMR1 - partitionSwapTimestamp;   // Total instruction cycles from old partition main() loop to this main() loop
        LCD_Init();     // Initialize LCD
    }

    lcdRefreshTimer = NOW_32() - NOW_ms*750u;

    // Start (or resume) all ordinary and periodic functions
    while(1)
    {
        ClrWdt();

        // Partition swap operations can be done anywhere, but it is easiest to
        // manage when the stack has nothing on it and minimal operations
        // would be interrupted. Therefore, we should do any pending partition
        // swap at the very beginning (or end) of this while(1) loop.
        if(timeForPartitionSwap)
        {
            timeForPartitionSwap = 0;
            
            // Disable all interrupts except for the critical ADCAN1/3 interrupt
            // which we want to synchronize the bootswap with
            _T1IE = 0;      // Timer 1 NOW tick timing code (ezbl_lib.a)
            _T2IE = 0;      // Timer 2 Soft start code
            _T3IE = 0;      // Timer 3 Dynamic load switching code
            _U1TXIE = 0;    // UART 1 TX EZBL Bootloader FIFO code
            _U1RXIE = 0;    // UART 1 RX EZBL Bootloader FIFO code
            _MI2C1IE = 0;   // I2C 1 Master-mode dynamic load switching communications code
            _ADCMP0IE = 0;  // ADC Digital Comparator 0 Warning and Over-temperature detect code
            _ADCMP1IE = 0;  // ADC Digital Comparator 1 Input overvoltage/undervoltage detect code 
            _ADCAN7IE = 0;  // ADC AN7 Input Voltage and Temperature capture code

            // criticalISRDone is always written to 0xFFFF in the periodic and
            // critical ADCAN3 interrupt. By synchronizing the partition swap
            // operation so it takes place immediately after the AN1 buck or AN3
            // boost interrupt is serviced, we can have less down time in the
            // SMPS control loop as we initialize new variables and
            // execute initialization routines in the new partition's code.
            if(_ADCAN1IE | _ADCAN3IE)   // Ignore synchronization when faults have the SMPS's offline
            {
                criticalISRDone = 0;            // Set a flag
                while(criticalISRDone == 0u);   // Wait for the flag to change, indicating the ISR has just finished
            }
            _LATB4 = 1;                         // Signal critical partition swap ISR down-time is starting (for debug only)
            partitionSwapTimestamp = NOW_16();  // Timestamp will tell us how much time has elapsed in new code since the hardware timer stays running
            EZBL_PartitionSwap();               // Perform the partition swap and branch to 0x000000 on the (presently) Inactive Partition
        }

        
        checkButton_SW1();          // Button pressed?

        // Check if it is time to refresh the LCD display
        if(NOW_32() - lcdRefreshTimer >= NOW_ms*750u)   // Update LCD every 750ms
        {
            lcdRefreshTimer += NOW_ms*750u;

            if((dpskFaultFlags.systemFaults) && (faultDisplayCnt++ >= 4u))
            {
                faultDisplayCnt = 0;
                if(dpskFaultFlags.inputVoltage)
                {
                    LCDselect = 1;     // During input voltage fault, force LCD to show fault & Vin/temp screen
                }
                LCD_DisplayFault();
            }
            else
            {
                LCD_Refresh();
            }
        }
        
        // Go Idle for a while to save power - precision timing and warp-speed 
        // processing of button pushes isn't necessary. Peripherals, 
        // interrupts and Bootloader processing via its NOW_TASK all remain 
        // active, exiting and returning to Idle in NOW_Wait() until the 
        // specified wait time is achieved.
        NOW_Wait(8u*NOW_ms);
    } // End of while(1) loop
}


///////////////////////////////////////////////////////////////////////////////
 // Function: checkButton_SW1()
 //
 // Preconditions : None
 //
 // Overview: Check if the push button SW1 is pressed
 //
 // Input: None.
 //
 // Output: None.
 //
 //////////////////////////////////////////////////////////////////////////////
void checkButton_SW1()
{
    unsigned long now;
    unsigned long debounceTimer;
    unsigned long resetTimer;
    unsigned int resetCountDown;
    
    // Button Changes LCD display information every time it is pressed
    if(SW1) // if S1 is released
        return;

    // Advance screen the LCD is showing and immediately refresh it
    LCDselect++;    // Overflow handled in LCD_Refresh()
    LCD_Refresh();

     // Wait for button release and no re-push detected for 32ms
    now = NOW_32();
    resetTimer = now;
    debounceTimer = now;
    resetCountDown = 4;
    do
    {
        now = NOW_32();
        if(!SW1)
        {
            debounceTimer = now;   // Continually restart debounce hold off window
            if(now - resetTimer > NOW_sec)  // If held down, offer device reset
            {
                resetTimer += NOW_sec;
                lcd_printf("SW pushed:\r\n"
                           "reset in %u\r", resetCountDown);
                if(resetCountDown-- == 0u)
                {
                    EZBL_ResetCPU();
                }
            }
        }   
    } while(now - debounceTimer < ((unsigned int)DEBOUNCE_DELAY)*NOW_ms);
}


////////////////////////////////////////////////////////////////////////////////
 // Function: LCD_DisplayFault()
 //
 // Preconditions : None.
 //
 // Overview: Display fault condition or warnings on LCD screen when fault
 //           occurs.
 //
 // Input: None.
 //
 // Output: None.
 //
 ///////////////////////////////////////////////////////////////////////////////
void LCD_DisplayFault()
{  
    const char *message = 0;
    
    // If both input voltage and Temp faults occur LCD will only display fault
    // for input voltage as this is the more critical fault. Loading will still 
    // be disabled though if both occur simultaneously.
    if(dpskFaultFlags.tempWarning)
    {
        message = "HIGH TEMPERATURE\r\n"
                  "\t WARNING\r";
    }

    if(dpskFaultFlags.overTemp)             // All loads disabled in this state
    {
        message = " OVER TEMP FAULT\r\n"
                  "  LOADS DISABLED\r";
    }

    if(dpskFaultFlags.inputVoltage)         // All loads disabled in this state too
    {
        message = " INPUT VOLTAGE\r\n"
                   "\t FAULT\r";
    }

    if(message)
        lcd_printf("%s", message);
}


///////////////////////////////////////////////////////////////////////////////
 // Function: LCD_Refresh()
 //
 // Preconditions : LCD_init()
 //
 // Overview: The function measures and updates LCD displayed information
 //
 // Input: None.
 // Output: None.
 //////////////////////////////////////////////////////////////////////////////
extern uint8_t uartBusIdle;
void LCD_Refresh()
{
    uint16_t LCDBuckVo;
    uint16_t LCDBoostVo;
    uint16_t LCDVinMeasured;
    uint16_t LCDTempC;
    uint16_t i;
    uint16_t displayLCDTime1;
    uint16_t displayLCDTime2;

    if(LCDselect > 3u)
        LCDselect = 0;

    switch(LCDselect)
    {
        case 1: // Display Input Voltage and Board Temperature
            // LCD Line 1: "V_Input:   0.00V"
            // LCD Line 2: "Boost Temp:  0 C"

            // Calculate Input Voltage & Temperature Conversion
            LCDVinMeasured = __builtin_mulsu(inputVoltage, VIN_FACTOR)>>15;
            LCDTempC = __builtin_mulss(tempSense - V_0C, TEMPCOEFFICIENT)>>15;

            if ((BUCK == DISABLED) && (BOOST == DISABLED))
            {
                lcd_printf("V_Input:% 4u.%02uV\r"
                         "\nAmb Temp:% 5d C\r", LCDVinMeasured / 100u, LCDVinMeasured % 100u, LCDTempC);
            }
            else
            {
                lcd_printf("V_Input:% 4u.%02uV\r"
                         "\n%s Temp:% 3d C\r", LCDVinMeasured / 100u, LCDVinMeasured % 100u, (_RB13 == 0) ? "Buck " : "Boost", LCDTempC);
            }

            // Toggle UART TX line level to change boost/buck temperature sampling (but try not to interfere with Bootloading sessions, so check flag first)
            if(uartBusIdle)
                __asm__("btg %0, #%1" : /* no outputs */ : "U"(U1STA), "i"(_U1STA_UTXINV_POSITION) : /* clobbers nothing */);  

            break;

        case 2: // Current Application version and hardware partition
            lcd_printf("App v%u.%04lu  %s\r\nPartition: %c\r", EZBL_appIDVer.appIDVerMinor, EZBL_appIDVer.appIDVerBuild, liveUpdateOccuredFlag ? "SWP" : "", '1'+_P2ACTIV);
            break;

        case 3: // After a live update partition swap, display time required for the change over
            if(liveUpdateOccuredFlag)
            {
                // Subtract off overhead of getting timeStamps and convert to integer units of nanoseconds/microseconds
                displayLCDTime1  = __builtin_divmodud(LCDTime1 - 5u, NOW_us, &i) * 1000u; // SwapTime: ns. Computes microseconds and scales to nanoseconds (thousands digits). Sub-microsecond remainder is saved.
                displayLCDTime1 += __builtin_divud(i * 1000u, NOW_us);                    //               Add in remainder, pre-scaled by 1000 to compute hundreds/tens/ones place nanosecond digits.
                displayLCDTime2  = __builtin_divud(LCDTime2 - 5u - 2u, NOW_us);           // TotalTime: us
                lcd_printf("SwapTime:% 5uns\r"
                         "\nTotalTime:% 4uus\r", displayLCDTime1, displayLCDTime2);
                break;
            }

            // Hide this screen and jump home if we haven't done a partition swap
            LCDselect = 0;
            // no break - default case is LCDselect = 0 case, and we don't want a button push to get ignored on this state

        default: // case 0: Default LCD Display
            // Write to strings template and update with measurements
            // LCD Line 1: "Buck 0.00V 0.00W"
            // LCD Line 2: "Bst  0.00V 0.00W"

            // Clear loads if over-temperature fault is set
            if(dpskFaultFlags.overTemp)
            {
                LCD_BoostLoad = NOLOAD;
                LCD_BuckLoad  = NOLOAD;
            }

            // Write first line of LCD containing the Buck converter status
            #if(BUCK == ENABLED)
                if(dpskFaultFlags.buckSoftStart)
                {
                    lcd_printf("Bck Output Fault\r");
                }
                else
                {
                    // Factor to display Buck/Boost Voltage on LCD
                    LCDBuckVo = __builtin_mulss(buckVoltage, VBUCK_LCD_FACTOR)>>15;
                    #if(BUCKLOAD1 == EXTERNALLOAD)
                        lcd_printf("Buck%2u.%02uV EXTRN\r", LCDBuckVo / 100u, LCDBuckVo % 100u);
                    #else
                        if(BUCKDYNAMICLOAD == DISABLED)
                            LCD_BuckLoad = BUCKLOAD1LCD;
                        lcd_printf("Buck%2u.%02uV% 2u.%02uW\r", LCDBuckVo / 100u, LCDBuckVo % 100u, LCD_BuckLoad / 100u, LCD_BuckLoad % 100u);
                    #endif
                }
            #else
                lcd_printf("Buck: Disabled\r");
            #endif


            #if(BOOST == ENABLED)
                if(dpskFaultFlags.boostSoftStart)
                {
                    lcd_printf("\nBst Output Fault\r");
                }
                else
                {
                    LCDBoostVo  = __builtin_mulss(boostVoltage, VBOOST_LCD_FACTOR)>>15;
                    #if(BOOSTLOAD1 == EXTERNALLOAD)
                        lcd_printf("\nBst%3u.%02uV EXTRN\r", LCDBoostVo / 100u, LCDBoostVo % 100u);
                    #else
                        if(BOOSTDYNAMICLOAD == DISABLED)
                            LCD_BoostLoad = BOOSTLOAD1LCD;
                        lcd_printf("\nBst%3u.%02uV% 2u.%02uW\r", LCDBoostVo / 100u, LCDBoostVo % 100u, LCD_BoostLoad / 100u, LCD_BoostLoad % 100u);
                    #endif
                }
            #else
                lcd_printf("\nBoost: Disabled\r";
            #endif
            break;
    }
}

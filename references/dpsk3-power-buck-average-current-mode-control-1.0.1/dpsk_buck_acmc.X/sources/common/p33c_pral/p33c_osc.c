/*
 * File:   p33SMPS_oscillator.c
 * Author: M91406
 *
 * Created on October 27, 2017, 11:24 AM
 */

/* ************************************************************************************************
 * PRIVATE DEFINES
 * ************************************************************************************************/

// Include Header Files
#include "p33c_osc.h"

/* ************************************************************************************************
 * PRIVATE DEFINES
 * ************************************************************************************************/

/**
 * @def OSC_CLKSW_TIMEOUT
 * @ingroup lib-layer-pral-properties-private-osc
 */
#define OSC_CLKSW_TIMEOUT	50000	///< value to set the timeout for clock switching operations

/* ************************************************************************************************
 * PRIVATE VARIABLES
 * ************************************************************************************************/
/**
 * @var struct OSCILLATOR_SYSTEM_FREQUENCIES_s system_frequencies
 * @ingroup lib-layer-pral-properties-private-osc
 */
volatile struct OSCILLATOR_SYSTEM_FREQUENCIES_s  system_frequencies;

/*************************************************************************************************
 * @fn uint16_t p33c_OscFrc_DefaultInitialize(volatile enum CPU_SPEED_DEFAULTS_e cpu_speed)
 * @ingroup lib-layer-pral-functions-public-osc
 * @param   cpu_speed CPU speed setting of type enum CPU_SPEED_DEFAULTS_e
 * @return  unsigned integer
 * 0 = unspecified clock failure detected
 * 1 = clock switch successful
 * 2 = clock switch failed
 * 4 = currently selected clock differs from selected clock source
 * 8 = PLL didn't lock in within given time frame
 * @brief Initializes the major oscillator and the PLL module step by step by using clock switching
 * in software. Each step is tested and verified. PLL settings here are pulled from pre-defined
 * settings set for default CPU speeds from 40 MIPS to 100 MIPS
 *
 * @details
 * Microchip's 16-Bit devices offer a safe 2-step start-up mode, using the internal FRC during power up, 
 * followed by a user defined switch-over to the desired oscillator. 
 * Though this can also be done in hardware automatically, this software-version of the switch-over offers
 * a better solution to verify each step and enables the user to implement some error handling in the case
 * of failure.
 * This function can be used to select a new oscillator at runtime. Each configuration step
 * will be verified before the next step is performed.
 *
 * @note
 * If a oscillator switch-over is performed, additional settings in the _FOSCSEL and _FOSC
 * registers of the configuration bits may be required
 * ************************************************************************************************/

volatile uint16_t p33c_OscFrc_DefaultInitialize(volatile enum CPU_SPEED_DEFAULTS_e cpu_speed)
{
    volatile int16_t retval = 0;
    volatile struct OSC_CONFIG_s osc;
    
    osc.osc_type = OSCCON_xOSC_FRCPLL;
    osc.N1 = CLKDIV_PLLDIV_N1_1;
    
    switch(cpu_speed)
    {
        case CPU_SPEED_20_MIPS:
            osc.M = PLLFBD_PLLFBDIV_M_20;
            break;
        case CPU_SPEED_30_MIPS:
            osc.M = PLLFBD_PLLFBDIV_M_30;
            break;
        case CPU_SPEED_40_MIPS:
            osc.M = PLLFBD_PLLFBDIV_M_40;
            break;
        case CPU_SPEED_50_MIPS:
            osc.M = PLLFBD_PLLFBDIV_M_50;
            break;
        case CPU_SPEED_60_MIPS:
            osc.M = PLLFBD_PLLFBDIV_M_60;
            break;
        case CPU_SPEED_70_MIPS:
            osc.M = PLLFBD_PLLFBDIV_M_70;
            break;
        case CPU_SPEED_80_MIPS:
            osc.M = PLLFBD_PLLFBDIV_M_80;
            break;
        case CPU_SPEED_90_MIPS:
            osc.M = PLLFBD_PLLFBDIV_M_90;
            break;
        case CPU_SPEED_100_MIPS:
            osc.M = PLLFBD_PLLFBDIV_M_100;
            break;
        default:
            return(0);  
            break;
    }
    osc.N2 = PLLDIV_POST2DIV_N2N3_2;
    osc.N3 = PLLDIV_POST2DIV_N2N3_1; 
    
    retval = p33c_Osc_Initialize(osc);

    return(retval);
}

/*************************************************************************************************
 * @fn uint16_t p33c_OscFrc_Initialize(volatile enum CLKDIV_FRCDIVN_e frc_div, volatile enum OSCTUN_TUN_e frc_tune)
 * @ingroup lib-layer-pral-functions-public-osc
 * @brief Initializes the internal RC oscillator divider and tuning register
 * @param frc_div  Internal RC Oscillator frequency divider of type enum CLKDIV_FRCDIVN_e
 * @param frc_tune Internal RC Oscillator tuning register value of type enum OSCTUN_TUN_e 
 * @return
 *  0 = unspecified clock failure detected
 *  1 = clock switch successful
 *  2 = clock switch failed
 *  4 = currently selected clock differs from selected clock source
 *  8 = PLL didn't lock in within given time frame
 *
 * @details
 * Microchip's 16-Bit devices offer a safe 2-step start-up mode, using the internal FRC during power up, 
 * followed by a user defined switch-over to the desired oscillator. 
 * Though this can also be done in hardware automatically, this software-version of the switch-over offers
 * a better solution to verify each step and enables the user to implement some error handling in the case
 * of failure.
 * This function can be used to select a new oscillator at runtime. Each configuration step
 * will be verified before the next step is performed.
 *
 * @note
 * If a oscillator switch-over is performed, additional settings in the _FOSCSEL and _FOSC
 * registers of the configuration bits may be required
 *************************************************************************************************/

volatile uint16_t p33c_OscFrc_Initialize(volatile enum CLKDIV_FRCDIVN_e frc_div, volatile enum OSCTUN_TUN_e frc_tune)
{
#if defined (__P33SMPS_CH_MSTR__) || defined (__P33SMPS_CK__)

// Slave cores do not have access to the FRC oscillator tuning register
    
    volatile uint16_t err=0;

    // Set oscillator tuning
    // => FRC Oscillator tuning is only available on single core devices and 
    //    the master core of dual core devices
    OSCTUNbits.TUN = frc_tune;      // Set Tuning Register for FRC Oscillator

    // Set FRC divider
    CLKDIVbits.FRCDIV = frc_div;    // Set FRC frequency divider

    return(err);
    
#else
    return(1);
#endif
    
}

/*************************************************************************************************
 * @fn uint16_t p33c_Osc_Initialize(volatile struct P33C_OSC_CONFIG_s osc_config)
 * @ingroup lib-layer-pral-functions-public-osc
 * @brief
 * Initializes the major oscillator and the PLL module step by step by using clock switching
 * in software. Each step is tested and verified
 *
 * @param  osc_config  Main oscillator and PLL configuration of type struct P33C_OSC_CONFIG_s
 *
 * @return unsigned integer
 *  0 = unspecified clock failure detected
 *  1 = clock switch successful
 *  2 = clock switch failed
 *  4 = currently selected clock differs from selected clock source
 *  8 = PLL didn't lock in within given time frame
 *
 * @details
 * Microchip's 16-Bit devices offer a safe 2-step start-up mode, using the internal FRC during power up, 
 * followed by a user defined switch-over to the desired oscillator. 
 * Though this can also be done in hardware automatically, this software-version of the switch-over offers
 * a better solution to verify each step and enables the user to implement some error handling in the case
 * of failure.
 * This function can be used to select a new oscillator at runtime. Each configuration step
 * will be verified before the next step is performed.
 *
 * @note
 * If a oscillator switch-over is performed, additional settings in the _FOSCSEL and _FOSC
 * registers of the configuration bits may be required
 *************************************************************************************************/

volatile uint16_t p33c_Osc_Initialize(volatile struct OSC_CONFIG_s osc_config)
{

    volatile uint16_t _n=0;
    volatile uint16_t err=0;

    // =============================================================================================
    // First make sure we are not running from a PLL derived source, when modifying the PLL settings.
    // This can be accomplished by intentionally switching to a known non-PLL setting, such as FRC.
    // Switch to FRC (no divider, no PLL), assuming we aren't already running from that.
    // =============================================================================================

    if(OSCCONbits.COSC != 0b000)
    {
        // NOSC = 0b000 = FRC without divider or PLL
        __builtin_write_OSCCONH(OSCCON_xOSC_FRC);  
        // Clear CLKLOCK and assert OSWEN = 1 to initiate switch-over
        __builtin_write_OSCCONL((OSCCON & 0x7E) | 0x01); 
        //Wait for switch over to complete.
        while((OSCCONbits.COSC != OSCCONbits.NOSC) && (_n++ < OSC_CLKSW_TIMEOUT)); 
        if (_n >= OSC_CLKSW_TIMEOUT) err = OSCERR_RST;
    }

    // Switch to target oscillator
	if ((OSCCONbits.COSC != osc_config.osc_type) && (OSCCONbits.CLKLOCK == 0))
	{
	// Switch to desired system clock, if clock switching is enabled

        #if defined (__P33SMPS_CH__) || defined (__P33SMPS_CK__)
    	// Configure PLL pre-scaler, PLL post-scaler, PLL divisor

        // Clock switch to desired CPU frequency with the PLL enabled in advance
        // Configure PLL pre-scaler, both PLL post-scalers, and PLL feedback divider
        PLLDIVbits.VCODIV = osc_config.VCODIV;    // FVCO Scaler 1:n
        CLKDIVbits.PLLPRE = osc_config.N1;   // N1 = PLL pre-scaler
        PLLFBDbits.PLLFBDIV = osc_config.M;  // M  = PLL feedback divider
        PLLDIVbits.POST1DIV = osc_config.N2; // N2 = PLL post-scalers #1
        PLLDIVbits.POST2DIV = osc_config.N3; // N3 = PLL post-scalers #2

        #elif defined (__P33SMPS_FJ__) || defined (__P33SMPS_FJA__) || defined (__P33SMPS_FJC__) || \
              defined (__P33SMPS_EP__)

        CLKDIVbits.PLLPRE	= osc_config.N1;		// N1 (non zero)
        PLLFBD				= (osc_config.M - 2);   // M  = PLLFBD 
        CLKDIVbits.PLLPOST	= osc_config.N2;        // N2 (non zero)

        #endif
        
        // Initiate Clock Switch to FRC Oscillator with PLL (NOSC=0b011)
        __builtin_write_OSCCONH(osc_config.osc_type);
        if(OSCCONbits.COSC != OSCCONbits.NOSC)
        {
            // Assert OSWEN and make sure CLKLOCK is clear, to initiate the switching operation
            __builtin_write_OSCCONL((OSCCON & 0x7F) | 0x01);    
            // Wait for clock switch to finish
            while((OSCCONbits.COSC != OSCCONbits.NOSC) && (_n++ < OSC_CLKSW_TIMEOUT));  
            if ((OSCCONbits.COSC != OSCCONbits.NOSC) || (_n >= OSC_CLKSW_TIMEOUT))
            { err = OSCERR_CSF; }
        }

	}
	else if ((OSCCONbits.COSC != osc_config.osc_type) && (OSCCONbits.CLKLOCK == 1))
	{ // If clock switching is disabled and the current clock differs from the desired clock,
	  // return error code
			err = OSCERR_CSD;
	}
        
    // Lock registers against accidental changes
    OSCCONbits.CLKLOCK = 1;
    
    while((OSCCONbits.LOCK != 1) && (_n++ < OSC_CLKSW_TIMEOUT)); // Wait n while loops for PLL to Lock
	if ((OSCCONbits.LOCK != 1) || (_n >= OSC_CLKSW_TIMEOUT)) // Error occurred? 
	{ err = OSCERR_PLL_LCK; } // => If so, return error code
    
// Return Success/Failure
    if (err == 0)
    { 
        return((1 - OSCCONbits.CF));					// Return oscillator fail status bit
    }													// (1=Success, 0=Failure)
    else
    {  return(err); }                                   // Return error code

}

/*************************************************************************************************
 * @fn      uint16_t p33c_OscAuxClk_Initialize(volatile struct AUXOSC_CONFIG_s aux_clock_config)
 * @ingroup lib-layer-pral-functions-public-osc
 * @brief
 * Initializes the auxiliary clock and its PLL module step by step 
 * in software. Each step is tested and verified
 *
 * @param aux_clock_config  Auxiliary oscillator configuration of type struct AUXOSC_CONFIG_s
 * 
 * @return
 *  0 = unspecified clock failure detected
 *  1 = clock switch successful
 *  2 = clock switch failed
 *  4 = currently selected clock differs from selected clock source
 *  8 = PLL didn't lock in within given time frame
 *
 * @details
 * The auxiliary clock is generated by a separate PLL module, which is driven
 * from one of the main clock signals or PLL outputs. This auxiliary clock can
 * be used to drive ADCs, PWM, DACs and other peripherals. Each of them might
 * have individual requirements. Please refer to the specific peripheral sections
 * in the device data sheet to learn how to configure the auxiliary clock.
 *
 *************************************************************************************************/

volatile uint16_t p33c_OscAuxClk_Initialize(volatile struct AUXOSC_CONFIG_s aux_clock_config)
{
    
    // Set AVCO divider of Auxiliary PLL 
    APLLDIV1bits.AVCODIV   = aux_clock_config.AVCODIV;  // AVCO Scaler 1:n

    // Configure APLL pre-scaler, APLL post-scaler, APLL divisor
    ACLKCON1bits.APLLPRE   = aux_clock_config.N1;		// N1 (non zero)
	APLLFBD1bits.APLLFBDIV = aux_clock_config.M;        // M  = APLLFBD 
    APLLDIV1bits.APOST1DIV = aux_clock_config.N2;       // N2 (non zero)
    APLLDIV1bits.APOST2DIV = aux_clock_config.N3;       // N3 (non zero)

    // Select clock input source (either primary oscillator or internal FRC)
    ACLKCON1bits.FRCSEL = aux_clock_config.FRCSEL;
	
    // Set Enable-bit of Auxiliary PLL 
    ACLKCON1bits.APLLEN = aux_clock_config.APLLEN;

    // if user has not enabled the APLL module, exit here
    if(!aux_clock_config.APLLEN)
    { return(0); }
    
    return(ACLKCON1bits.APLLEN);
    
}
 
/*************************************************************************************************
 * @fn uint16_t p33c_OscAuxClk_DefaultInitialize(volatile enum AUX_PLL_DEFAULTS_e afpllo_frequency)
 * @ingroup lib-layer-pral-functions-public-osc
 * @brief
 * Initializes the auxiliary clock and its PLL module step by step 
 * in software. Each step is tested and verified
 *
 * @param afpllo_frequency  Auxiliary PLL output frequency of type enum AUX_PLL_DEFAULTS_e
 *
 * @return
 *  0 = unspecified clock failure detected
 *  1 = clock switch successful
 *  2 = clock switch failed
 *  4 = currently selected clock differs from selected clock source
 *  8 = PLL didn't lock in within given time frame
 *
 * @details
 * The auxiliary clock is generated by a separate PLL module, which is driven
 * from one of the main clock signals or PLL outputs. This auxiliary clock can
 * be used to drive ADCs, PWM, DACs and other peripherals. Each of them might
 * have individual requirements. Please refer to the specific peripheral sections
 * in the device data sheet to learn how to configure the auxiliary clock.
 *
 *************************************************************************************************/

 volatile uint16_t p33c_OscAuxClk_DefaultInitialize(volatile enum AUX_PLL_DEFAULTS_e afpllo_frequency)
 {
    volatile uint16_t retval = 1;
    volatile struct AUXOSC_CONFIG_s aux_clock_config;

    // Set FRC as clock input to auxiliary PLL module
    aux_clock_config.FRCSEL = PLLDIV_ACLKCON_FRCSEL_FRC;
    
    // Set auxiliary PLL VCO divider to 1:4
    aux_clock_config.AVCODIV = APLLDIV_AVCODIV_FVCO_DIV_BY_4;
    
    // Select PLL dividers and multiplier in accordance with user parameter 
    if(afpllo_frequency <= 800) {
        aux_clock_config.N1 = ACLKCON_APLLDIV_N1_1;     // Default divider of 1
        aux_clock_config.M = (afpllo_frequency >> 2);   // frequency divided by 4
        aux_clock_config.N2 = APLLDIV_POST2DIV_N2N3_2;  // Default divider of 2
        aux_clock_config.N3 = APLLDIV_POST2DIV_N2N3_1;  // Default divider of 1
    }
    else {
        return(0);  // When frequency is out of range, return failure
                    // Most recent APLL setting remains unchanged
    }
    
    // Enable auxiliary PLL
    aux_clock_config.APLLEN = ACLKCON_APLLEN_ENABLED;
    
    // Call auxiliary PLL configuration to apply new settings
    retval &= p33c_OscAuxClk_Initialize(aux_clock_config);
    
    return(retval);
 }


/*!smpsOSC_GetFrequencies()
 * ************************************************************************************************
 * @fn uint16_t p33c_Osc_GetFrequencies(volatile uint32_t main_osc_frequency)
 * @ingroup lib-layer-pral-functions-public-osc
 * @brief
 * This routine reads all oscillator related SFRs recalculating the various frequencies
 * across clock domains. These frequencies are broadcasted in the global data structure 
 * 'system_frequencies'.
 *
 * @param main_osc_frequency  Frequency of external oscillator frequency in [Hz] of type unsigned integer.
 *                            Set to '0' if no external oscillator is used.
 *
 * @return
 * unsigned integer
 * 0 = reading oscillator settings failed
 * 1 = reading oscillator settings successfully
 *
 * @details
 * Microchip's 16-Bit devices offer multiple clock sources to clock the CPU. This routine 
 * reads the most recent, oscillator related Special Function Registers (SFR) and determines
 * the recently active main clock and its frequency and calculates the resulting clock frequencies
 * of other clock domains. 
 * 
 * The results are broadcasted through the 'system_frequencies' data structure which is globally 
 * accessible in user code and can be used to calculate other oscillator dependent settings such as
 * timer periods or baud rates of communication interfaces
 * 
 * Please note:
 * The contents of data structure 'system_frequencies' is NOT updated automatically. It is 
 * recommended to call the function 'osc_get_frequencies' in user code every time after 
 * clock settings have been modified. 
 *
 * ************************************************************************************************/
volatile uint16_t p33c_Osc_GetFrequencies(volatile uint32_t main_osc_frequency) {
    
    volatile int32_t freq=0;
    volatile uint16_t vbuf=0;
    volatile OSCCON_xOSC_TYPE_e otype;
    
    // Copy oscillator frequency given as unsigned 32-bit integer into signed 32-bit variable
    freq = (volatile int32_t)main_osc_frequency;

    // Capture external oscillator frequency
    if (main_osc_frequency > 0) {
        system_frequencies.fpri = (volatile uint32_t)freq;
    }
    
    // read currently selected oscillator
    otype = OSCCONbits.COSC; 
    
    // Copy Base FRC frequency into data structure
    system_frequencies.frc = (volatile int32_t)OSC_FRC_FREQ;   // Set default FRC oscillator frequency 
    
    // Depending on detected oscillator type, set/override oscillator frequency
    
    // For all modes using the internal Fast RC Oscillator (FRC), check input divider and tuning register
    if ((otype == OSCCON_xOSC_FRC) || (otype == OSCCON_xOSC_BFRC) || (otype == OSCCON_xOSC_FRCPLL) || (otype == OSCCON_xOSC_FRCDIVN)) {
        
        freq = (volatile int32_t)OSC_FRC_FREQ;   // Oscillator frequency is 8 MHz
        
        #if defined (__P33SMPS_CK__) || defined (__P33SMPS_CH_MSTR__)
        // FRC tuning register does not affect Backup FRC clock
        if(otype != OSCCON_xOSC_BFRC) {
            freq += OSC_TUN_STEP_FREQUENCY * (volatile int32_t)(OSCTUNbits.TUN); // Add oscillator tuning value (is singed)
            system_frequencies.frc = freq;    // Copy updated fast RC oscillator frequency after tuning
        }
        #endif
        
        // Including FRC divider requires the FRCDIV oscillator to be selected
        if (otype == OSCCON_xOSC_FRCDIVN) {        // If oscillator is using internal divider...
            vbuf = (CLKDIVbits.FRCDIV & 0x0003);    // Copy divider SFR bits
            freq >>= vbuf; // Right-shift oscillator frequency by divider bits (FRCDIV)
        }
        
    }
    
    // Internal Low-Power RC Oscillator is always fixed to 32 kHz
    else if (otype == OSCCON_xOSC_LPRC) {
        freq = (volatile int32_t)32000;        // Oscillator frequency is 32 kHz
    }
    
    // If external clock modes are set, check if given frequency is non-zero
    else { // If no oscillator frequency is given, it's assumed FRC oscillator is used
        if (freq == 0) return(0);   // Error: no oscillator frequency given
    }
    
    // Capture system root clock
    system_frequencies.fclk = (volatile uint32_t)freq;

    // Check for PLL usage and calculate oscillator frequency based on recent settings
    if ( (otype == OSCCON_xOSC_FRCPLL) || (otype == OSCCON_xOSC_PRIPLL) ) {
        
        // Check if PLL is locked in and stable
        if (!OSCCONbits.LOCK) return(0);    // if incorrect/not valid, return error 
        
        // Check PLL divider N1 for valid value
        vbuf = (CLKDIVbits.PLLPRE & 0x000F);
        if((vbuf > 8) || (vbuf == 0)) return (0);  // if incorrect/not valid, return error
        freq /= vbuf;   // Divide frequency by divider N1
        
        // Check PLL multiplier M
        vbuf = (PLLFBDbits.PLLFBDIV & 0x00FF);
        if((vbuf > 200) || (vbuf < 3)) return (0);  // if incorrect/not valid, return error
        freq *= vbuf; // Multiply frequency by Multiplier M

        // Capture VCO frequency
        vbuf = (PLLDIVbits.VCODIV & 0x0003);
        if(vbuf > 3) return (0);  // if incorrect/not valid, return error
        vbuf = 4-vbuf;  // Subtract value from 4 to get divider ratio
        system_frequencies.fvco = (volatile uint32_t)(freq/vbuf);   // Divide frequency by VCO divider
        
        // Check PLL divider N2 for valid value
        vbuf = (PLLDIVbits.POST1DIV & 0x0007);
        if((vbuf > 8) || (vbuf == 0)) return (0);  // if incorrect/not valid, return error
        freq /= vbuf;   // Divide frequency by divider N2

        // Check PLL divider N3 for valid value
        vbuf = (PLLDIVbits.POST2DIV & 0x0007);
        if((vbuf > 8) || (vbuf == 0)) return (0);  // if incorrect/not valid, return error
        freq /= vbuf;   // Divide frequency by divider N3
        
    }
    
    // Capture PLL output frequency
    system_frequencies.fpllo = (volatile uint32_t)freq;

    // CPU Clock Divider
    freq >>= 1;   // Divide frequency by 2
    system_frequencies.fosc = (volatile uint32_t)freq;
    
    // Peripheral Bus Divider
    freq >>= 1;   // Divide frequency by 2
    system_frequencies.fp = (volatile uint32_t)freq;

    // Reading DOZE setting
    if (CLKDIVbits.DOZEN) {                 // If DOZE divider is enabled
        vbuf = (CLKDIVbits.DOZE & 0x0003);  // Copy divider SFR bits
        freq >>= vbuf; // Right-shift oscillator frequency by divider bits (DOZE)
    }
    system_frequencies.fcy = (volatile uint32_t)freq;
    
    // Calculate CPU clock period
    system_frequencies.tcy = 1.0/((float)system_frequencies.fcy);

    // Calculate peripheral clock period
    system_frequencies.tp = 1.0/((float)system_frequencies.fp);

    // -----------------------------------------------
    // Capture APLL frequencies
    // -----------------------------------------------
    
    if (ACLKCON1bits.APLLEN) {  // APLL is enabled...
    
        // Select input frequency
        if(ACLKCON1bits.FRCSEL) { freq = system_frequencies.frc; }
        else { freq = system_frequencies.fpri; }
        
        // Check APLL divider N1 for valid value
        vbuf = (ACLKCON1bits.APLLPRE & 0x000F);
        if((vbuf > 8) || (vbuf == 0)) return (0);  // if incorrect/not valid, return error
        freq /= vbuf;   // Divide frequency by divider N1

        // Check PLL multiplier M
        vbuf = (APLLFBD1bits.APLLFBDIV & 0x00FF);
        if((vbuf > 200) || (vbuf < 3)) return (0);  // if incorrect/not valid, return error
        freq *= vbuf; // Multiply frequency by Multiplier M

        // Capture VCO frequency
        vbuf = (APLLDIV1bits.AVCODIV & 0x0003);
        if(vbuf > 3) return (0);  // if incorrect/not valid, return error
        vbuf = 4-vbuf;  // Subtract value from 4 to get divider ratio
        system_frequencies.afvco = (volatile uint32_t)(freq/vbuf);   // Divide frequency by VCO divider

        // Check PLL divider N2 for valid value
        vbuf = (APLLDIV1bits.APOST1DIV & 0x0007);
        if((vbuf > 8) || (vbuf == 0)) return (0);  // if incorrect/not valid, return error
        freq /= vbuf;   // Divide frequency by divider N2

        // Check PLL divider N3 for valid value
        vbuf = (APLLDIV1bits.APOST2DIV & 0x0007);
        if((vbuf > 8) || (vbuf == 0)) return (0);  // if incorrect/not valid, return error
        freq /= vbuf;   // Divide frequency by divider N3
        system_frequencies.afpllo = freq;    // Capture auxiliary PLL output
        
    }
    else {  // APLL is disabled...
        system_frequencies.afpllo = 0;    // Reset auxiliary PLL output
        system_frequencies.afvco = 0;     // Reset auxiliary PLL VCO output
    }
        
    // Return 1=Success, 0=Failure
    return(1);
}


// end of file


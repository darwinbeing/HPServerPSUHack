/*
 * File:   drv_buck_pconfig.c
 * Author: M91406
 *
 * Created on March 12, 2020, 4:31 PM
 */

#if (__XC16_VERSION__ > 1500)
  #pragma message ("Warning: Library file '" __FILE__ "' has not been tested with the recently selected compiler version")
#endif

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer types header file
#include <stdbool.h> // include standard boolean types header file

#include "dev_buck_pconfig.h"
#include "dev_buck_templates.h"

/* PRIVATE VARIABLES */
/**
 * @var adcore_mask
 * @ingroup lib-layer-buck-pconfig-properties-variables
 * @brief This variable is use to set the ADC core mask
 */
volatile uint16_t adcore_mask=0;        

/**
 * @var adcore_diff_mask
 * @ingroup lib-layer-buck-pconfig-properties-variables
 * @brief This variable is use to set the ADC core mask
 */
volatile uint16_t adcore_diff_mask=0;   

/* PRIVATE FUNCTION CALL PROTOTYPES */

volatile uint16_t buckGPIO_PrivateInitialize(volatile struct BUCK_GPIO_INSTANCE_s* buckGPIOInstance);

/*******************************************************************************
 * @fn	    uint16_t buckPWM_ModuleInitialize(volatile struct BUCK_CONVERTER_s* buckInstance)
 * @ingroup lib-layer-buck-pconfig-functions
 * @brief   Initializes the buck PWM module by resetting its registers to default
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 *    This function initializes the buck PWM module base registers with default
 *     values for maximum performance. 
 * 
 *     Default configuration:
 *         - all PWM peripheral power is enabled
 *         - all PWM generators are disabled
 *         - default PWM Module configurations are written in buck PWM module base registers 
 *********************************************************************************/
volatile uint16_t buckPWM_ModuleInitialize(volatile struct BUCK_CONVERTER_s* buckInstance)
{
    volatile uint16_t retval=1;
    volatile struct P33C_PWM_MODULE_s* pwm;
    
    // Make sure power to the peripheral is enabled
    volatile bool _pmdlock = PMDCONbits.PMDLOCK; // Copy state of PMD lock bit
    PMDCONbits.PMDLOCK = 0; // Unlock PMD register writes
    PMD1bits.PWMMD = 0; // PWM Module Disable: PWM module is enabled
    PMDCONbits.PMDLOCK = _pmdlock; // set previous state of PMD lock
    
    // DISABLE ALL PWM GENERATORS
    PG1CONLbits.ON = 0; // PWM Generator #1 Enable: PWM Generator is not enabled
    PG2CONLbits.ON = 0; // PWM Generator #2 Enable: PWM Generator is not enabled
    PG3CONLbits.ON = 0; // PWM Generator #3 Enable: PWM Generator is not enabled
    PG4CONLbits.ON = 0; // PWM Generator #4 Enable: PWM Generator is not enabled
    
    #if defined (PG5CONL)
    PG5CONLbits.ON = 0; // PWM Generator #5 Enable: PWM Generator is not enabled
    #endif
    #if defined (PG6CONL)
    PG6CONLbits.ON = 0; // PWM Generator #6 Enable: PWM Generator is not enabled
    #endif
    #if defined (PG7CONL)
    PG7CONLbits.ON = 0; // PWM Generator #7 Enable: PWM Generator is not enabled
    #endif
    #if defined (PG8CONL)
    PG8CONLbits.ON = 0; // PWM Generator #8 Enable: PWM Generator is not enabled
    #endif

    // Write default PWM Module configuration to PWM module base registers
    pwm = p33c_PwmModule_GetHandle();
    retval &= p33c_PwmModule_ConfigWrite(buckPwmModuleConfig);

    // If buck converter has been configured in MASTER PERIOD mode
    if (buckInstance->sw_node[0].master_period_enable) // If master period mode is enabled...
        pwm->vMPER.value = buckInstance->sw_node[0].period; // Set Period of phase #1

    return(retval);    
} 

/*******************************************************************************
 * @fn      uint16_t buckPWM_ChannelInitialize(volatile struct BUCK_CONVERTER_s* buckInstance)
 * @ingroup lib-layer-buck-pconfig-functions
 * @brief   This function initializes the output pins for the PWM output and the default buck PWM settings
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 *    This function initializes the buck PWM channel with default values for maximum performance. 
 * 
 *     Default configuration:
 *         - selected PWM outputs are enabled 
 *         - PWM timing settings are loaded (i.e., duty cycle, period, dead-times, blanking)
 *         - PWM synchronization is established
 *********************************************************************************/
volatile uint16_t buckPWM_ChannelInitialize(volatile struct BUCK_CONVERTER_s* buckInstance)
{
    volatile uint16_t retval=1;
    volatile uint16_t _i=0;

    volatile struct P33C_GPIO_INSTANCE_s* gpio;
    volatile struct P33C_PWM_GENERATOR_s* pg;
    volatile uint16_t pwm_Instance;
    volatile uint16_t gpio_Instance;

    for (_i=0; _i<buckInstance->set_values.no_of_phases; _i++) {
    
        // LOAD PERIPHERAL INSTANCES FROM BUCK CONVERTER OBJECT
        pwm_Instance = buckInstance->sw_node[_i].pwm_instance;
        gpio_Instance = buckInstance->sw_node[_i].gpio_instance;

        // CAPTURE MEMORY ADDRESS OF GIVEN PWM GENERATOR INSTANCE
        gpio = p33c_GpioInstance_GetHandle(gpio_Instance);

        // WRITE GPIO CONFIGURATION OF PWM OUTPUT(S)
        gpio->LATx.value  &= ~(0x0001 << buckInstance->sw_node[_i].gpio_high); // Clear PWMxH output LOW
        gpio->LATx.value  &= ~(0x0001 << buckInstance->sw_node[_i].gpio_low);  // Clear PWMxL output LOW
        gpio->TRISx.value &= ~(0x0001 << buckInstance->sw_node[_i].gpio_high); // Clear PWMxH output to OUTPUT
        gpio->TRISx.value &= ~(0x0001 << buckInstance->sw_node[_i].gpio_low);  // Clear PWMxL output to OUTPUT
        gpio->CNPDx.value |= (0x0001 << buckInstance->sw_node[_i].gpio_high);  // Enable intern pull down register (PWM1H)
        gpio->CNPDx.value |= (0x0001 << buckInstance->sw_node[_i].gpio_low);   // Enable intern pull down register (PWM1L)
    
        // COPY CONFIGURATION FROM TEMPLATE TO PWM GENERATOR x CONTROL REGISTERS
        pg = p33c_PwmGenerator_GetHandle(pwm_Instance);
        retval &= p33c_PwmGenerator_ConfigWrite(buckInstance->sw_node[_i].pwm_instance, buckPwmGeneratorConfig);

        // LOAD PWM GENERATOR TIMING SETTINGS FROM BUCK CONVERTER OBJECT
        pg->PGxCONH.bits.MPERSEL = (uint16_t)buckInstance->sw_node[_i].master_period_enable;
        pg->PGxCONL.bits.HREN = (uint16_t)buckInstance->sw_node[_i].high_resolution_enable;
        
        pg->PGxDC.value = buckInstance->sw_node[_i].duty_ratio_min; // PGxDC: PWM GENERATOR x DUTY CYCLE REGISTER
        pg->PGxPER.value = buckInstance->sw_node[_i].period; // PGxPER: PWM GENERATOR x PERIOD REGISTER
        pg->PGxDTL.value = buckInstance->sw_node[_i].dead_time_falling; // PGxDTL: PWM GENERATOR x DEAD-TIME REGISTER LOW
        pg->PGxDTH.value = buckInstance->sw_node[_i].dead_time_rising; // PGxDTH: PWM GENERATOR x DEAD-TIME REGISTER HIGH
        pg->PGxLEBL.value = buckInstance->sw_node[_i].leb_period; // PWM GENERATOR x LEADING-EDGE BLANKING REGISTER LOW 
        
        // ToDo: PWM Synchronization needs to be more universaL
        // PWM synchronization only work within groups of 4 (PG1-PG4 or PG5-PG8)
        // Multiphase boost converter auto PWM phase synchronization
        if ((_i == 0) && ((uint16_t)(buckInstance->set_values.no_of_phases) > 1U))
        { // First phase is always master phase
            pg->PGxCONH.bits.MSTEN = 1; // Enable Master synchronization mode
            pg->PGxCONH.bits.SOCS = 0b0000; // Master PWM always triggers itself
            pg->PGxEVTL.bits.PGTRGSEL = 0b011; // PGxTRIGC is always used as PWM synchronization trigger output
            pg->PGxTRIGC.value = buckInstance->sw_node[_i+1].phase; // Set phase shift between master phase and first synchronized phase
            pg->PGxCONH.bits.UPDMOD = 0b001; // Immediate update
            pg->PGxEVTL.bits.UPDTRG = 0b11; // A write of the PGxTRIGA register automatically sets the UPDATE bit
        }
        else if ((0 < _i) && (_i < (buckInstance->set_values.no_of_phases-1)))
        { // Every synchronized phase is synchronized to previous phase while being master to following
            pg->PGxCONH.bits.MSTEN = 1; // Enable Master synchronization mode
            pg->PGxCONH.bits.SOCS = buckInstance->sw_node[_i-1].pwm_instance; // synchronized PWM is always triggered by previous generator while providing trigger for following
            pg->PGxEVTL.bits.PGTRGSEL = 0b011; // PGxTRIGC is always used as PWM synchronization trigger output
            pg->PGxTRIGC.value = buckInstance->sw_node[_i+1].phase; // Set phase shift between master phase and first synchronized phase
            pg->PGxCONH.bits.UPDMOD = 0b011; // Sync immediate update
            pg->PGxEVTL.bits.UPDTRG = 0; // User must set the UPDREQ bit (PGxSTAT[3]) manually
        }
        else if ((0 < _i) && (_i == (buckInstance->set_values.no_of_phases-1)))
        { // Last phase does not provide any master trigger
            pg->PGxCONH.bits.MSTEN = 0; // Disable Master synchronization mode
            pg->PGxCONH.bits.SOCS = buckInstance->sw_node[_i-1].pwm_instance; // synchronized PWM is always triggered by previous generator while providing trigger for following
            pg->PGxEVTL.bits.PGTRGSEL = 0b000; // EOC is the general trigger output
            pg->PGxTRIGC.value = 0x0000; // Clear phase shift value 
            pg->PGxCONH.bits.UPDMOD = 0b011; // Sync immediate update
            pg->PGxEVTL.bits.UPDTRG = 0; // User must set the UPDREQ bit (PGxSTAT[3]) manually
        }
        else if (buckInstance->set_values.no_of_phases == 1U)
        { // This is only a single phase system (no PWM dependencies)
            pg->PGxCONH.bits.MSTEN = 0; // Disable Master synchronization mode
            pg->PGxCONH.bits.SOCS = 0b0000; // Master PWM always triggers itself
            pg->PGxEVTL.bits.PGTRGSEL = 0b011; // PGxTRIGC is always used as PWM synchronization trigger output
            pg->PGxTRIGC.value = 0x0000; // Clear phase shift between master phase and first synchronized phase
            pg->PGxCONH.bits.UPDMOD = 0b001; // Immediate update
            pg->PGxEVTL.bits.UPDTRG = 0b11; // A write of the PGxTRIGA register automatically sets the UPDATE bit
        }
        else { /* continue */ }
        
        // Update PWM generator timing registers
        pg->PGxSTAT.bits.UPDREQ = 1; // Manually set the Update Request bit 
        pg->PGxCONH.bits.TRGMOD = 1; // all PWM generators are in retriggerable mode
        
    }
        
    return(retval);    
} 

/*******************************************************************************
 * @fn	    uint16_t buckPWM_Start(volatile struct BUCK_CONVERTER_s* buckInstance)
 * @ingroup lib-layer-buck-pconfig-functions
 * @brief   This function enables the buck PWM operation
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 * This function starts the operation of the buck PWM by enabling the PWM 
 * generator and its output pins.
 *********************************************************************************/
volatile uint16_t buckPWM_Start(volatile struct BUCK_CONVERTER_s* buckInstance) 
{
    volatile uint16_t retval=1;
    volatile uint16_t _i=0;
    volatile uint16_t timeout=0;
    volatile uint16_t pwm_Instance=0;
    volatile uint16_t sync_sw_mask=0;
    volatile struct P33C_PWM_GENERATOR_s* pg;

    // Enable all PWM channels of the recent buck converter configuration
    for(_i=0; _i< buckInstance->set_values.no_of_phases; _i++) {
    
        // Capture PWM instance of the recent channel
        pwm_Instance = buckInstance->sw_node[_i].pwm_instance;
        
        // CAPTURE MEMORY ADDRESS OF GIVEN PWM GENERATOR INSTANCE
        pg = p33c_PwmGenerator_GetHandle(pwm_Instance);

        pg->PGxIOCONL.value |= P33C_PGxIOCONL_OVREN_SYNC; // PWMxH/L Output Override Enable: PWM generator controls the PWMxH output pin
        pg->PGxIOCONH.value &= ~(P33C_PGxIOCONH_PEN_SYNC); // PWMxH/L Output Port Disable: PWM generator controls the PWMxH output pin

        pg->PGxCONL.bits.ON = 1;  // PWM Generator Enable: PWM Generator is enabled
        pg->PGxSTAT.bits.UPDREQ = 1;  // Update all PWM timing registers
        
        if(pg->PGxCONL.value & P33C_PGxCONL_HRES_EN) // If high resolution is enabled
        {
            while((!PCLKCONbits.HRRDY) && (timeout++ < 5000));  // wait for high resolution to get ready
            if ((timeout >= 5000) || (PCLKCONbits.HRERR))       // if there is an error ...
                return(0);                                      // ... exit returning ERROR
        }

        // Select the control bits for either synchronous or asynchronous PWM drive
        // note: swapping PWMs changes H/L assignments and the 'active' pin has to 
        //       selected in asynchronous mode
        if (buckInstance->sw_node[_i].sync_drive)
            sync_sw_mask = P33C_PGxIOCONH_PEN_SYNC;
        else
        {   
            if (buckInstance->sw_node[_i].swap_outputs)
                sync_sw_mask = P33C_PGxIOCONH_PEN_ASYNC_SWAP;
            else
                sync_sw_mask = P33C_PGxIOCONH_PEN_ASYNC;
        }
        
        // PWMxH/L Output Port Enable: PWM generator controls the PWMxH output pin
        pg->PGxIOCONH.value |= sync_sw_mask; 

        // Turn on PWM generator
        retval &= (volatile uint16_t)(pg->PGxCONL.bits.ON);

    }
    
    return(retval);    
} 

/*******************************************************************************
 * @fn	    uint16_t buckPWM_Stop(volatile struct BUCK_CONVERTER_s* buckInstance)
 * @ingroup lib-layer-buck-pconfig-functions
 * @brief   This function stops the buck PWM output
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 * This function shuts down the PWM output by disabling all PWM channels of the recent buck
 * converter object, overriding the output pins and resetting the buck PWM duty cycle to
 * its minimum duty ratio. 
 * 
 * If the Power Good output is enabled, this output pin will also be reset.
 *********************************************************************************/
volatile uint16_t buckPWM_Stop(volatile struct BUCK_CONVERTER_s* buckInstance) 
{
    volatile uint16_t retval=1;
    volatile uint16_t _i=0;
    volatile uint16_t pwm_Instance=0;
    volatile struct P33C_PWM_GENERATOR_s* pg;

    // Disable all PWM channels of the recent buck converter configuration
    for(_i=0; _i< buckInstance->set_values.no_of_phases; _i++) {
    
            // Capture PWM instance of the recent channel
        pwm_Instance = buckInstance->sw_node[_i].pwm_instance;

        // CAPTURE MEMORY ADDRESS OF GIVEN PWM GENERATOR INSTANCE
        pg = p33c_PwmGenerator_GetHandle(pwm_Instance);

        pg->PGxIOCONL.value |= P33C_PGxIOCONL_OVREN_SYNC;  // PWMxH/L Output Override Enable
        pg->PGxIOCONH.value &= ~(P33C_PGxIOCONH_PEN_SYNC); // PWMxH/L Output Pint Control Disable
        pg->PGxCONL.value &= ~(P33C_PGxCONL_PWM_ON);  // PWM Generator Disable
        pg->PGxDC.value = buckInstance->sw_node[_i].duty_ratio_min; // Reset Duty Cycle
        pg->PGxSTAT.value |= P33C_PGxSTAT_UPDREQ;     // Set the Update Request bit to update PWM timing
        
        retval &= (volatile uint16_t)((volatile bool)(pg->PGxCONL.bits.ON == 0));

    }

    // If defined, reset POWER_GOOD output
    if(buckInstance->gpio.PowerGood.enabled)
        retval &= buckGPIO_Clear(&buckInstance->gpio.PowerGood);
    
    return(retval);    
    
} 

/*******************************************************************************
 * @fn	    uint16_t buckPWM_Suspend(volatile struct BUCK_CONVERTER_s* buckInstance) 
 * @ingroup lib-layer-buck-pconfig-functions
 * @brief   This function disables the PWM generator IOs
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 * This function suspends the buck PWM operation by disabling all PWM outputs of the recent 
 * buck converter configuration, overriding the PWM output pins and setting the 
 * duty cycle to 0.
 *********************************************************************************/
volatile uint16_t buckPWM_Suspend(volatile struct BUCK_CONVERTER_s* buckInstance) 
{
    volatile uint16_t retval=1;
    volatile uint16_t _i=0;
    volatile uint16_t pwm_Instance;
    volatile struct P33C_PWM_GENERATOR_s* pg;

    // Disable all PWM outputs of the recent buck converter configuration
    for(_i=0; _i< buckInstance->set_values.no_of_phases; _i++) {
    
        // Capture PWM instance of the recent channel
        pwm_Instance = buckInstance->sw_node[_i].pwm_instance;

        // CAPTURE MEMORY ADDRESS OF GIVEN PWM GENERATOR INSTANCE
        pg = p33c_PwmGenerator_GetHandle(pwm_Instance);
        
        pg->PGxIOCONL.value |= P33C_PGxIOCONL_OVREN_SYNC; // PWMxH/L Output Override Enable
        pg->PGxDC.value = 0;  // Reset Duty Cycle
        pg->PGxSTAT.value |= P33C_PGxSTAT_UPDREQ; // Set the Update Request bit to update PWM timing

        retval &= (bool)(pg->PGxIOCONL.value & P33C_PGxIOCONL_OVREN_SYNC);

    }
    
    // If defined, reset POWER_GOOD output
    if(buckInstance->gpio.PowerGood.enabled)
        retval &= buckGPIO_Clear(&buckInstance->gpio.PowerGood);
    
    return(retval);    
    
} 

/*******************************************************************************
 * @fn	    uint16_t buckPWM_Resume(volatile struct BUCK_CONVERTER_s* buckInstance)
 * @ingroup lib-layer-buck-pconfig-functions
 * @brief   This function resumes the buck PWM operation
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 * THis function updates the PWM timing bit and the PWM output pins are enabled.
 *********************************************************************************/
volatile uint16_t buckPWM_Resume(volatile struct BUCK_CONVERTER_s* buckInstance) 
{
    volatile uint16_t retval=1;
    volatile uint16_t _i=0;
    volatile bool sync_mode=false;
    volatile uint16_t pwm_Instance=0;
    volatile uint16_t sync_sw_mask=0;
    volatile struct P33C_PWM_GENERATOR_s* pg;

    // Disable all PWM channels of the recent buck converter configuration
    for(_i=0; _i< buckInstance->set_values.no_of_phases; _i++) {
    
            // Capture PWM instance of the recent channel
        pwm_Instance = (uint16_t)buckInstance->sw_node[_i].pwm_instance;

        // CAPTURE MEMORY ADDRESS OF GIVEN PWM GENERATOR INSTANCE
        pg = p33c_PwmGenerator_GetHandle(pwm_Instance);
    
        // Select the control bits for either synchronous or asynchronous PWM drive
        sync_mode = buckInstance->sw_node[_i].sync_drive; 
        sync_mode &= (~buckInstance->status.bits.async_mode);
        
        // Select the control bits for either synchronous or asynchronous PWM drive
        // note: swapping PWMs changes H/L assignments and the 'active' pin has to 
        //       selected in asynchronous mode
        if (sync_mode)
        {   sync_sw_mask = P33C_PGxIOCONL_OVREN_SYNC; }
        else
        {   
            if (buckInstance->sw_node[_i].swap_outputs)
                sync_sw_mask = P33C_PGxIOCONL_OVREN_ASYNC_SWAP;
            else
                sync_sw_mask = P33C_PGxIOCONL_OVREN_ASYNC;
        }
        
        // Clear selected override bits
        pg->PGxSTAT.bits.UPDREQ = 1; // Set the Update Request bit to update PWM timing
        pg->PGxIOCONL.value &= (volatile uint16_t)(~(sync_sw_mask)); // PWMxH/L Output Override Disable

        retval &= (uint16_t)((bool)(!(pg->PGxIOCONL.value & sync_sw_mask)));

    }
        
    return(retval);  
    
}

/*******************************************************************************
 * @fn	    uint16_t buckADC_ModuleInitialize(void) 
 * @ingroup lib-layer-buck-pconfig-functions
 * @brief   This fucntion initializes the buck by resetting all its registers to default
 * @param	void
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 * The ADC initialization covers basic configurations like data format, clock sources and dividers
 * as well as specific configurations for ADC cores. These settings are general, basic settings
 * and not related to specific analog inputs. The standard configuration set here sets up the 
 * ADC module and ADC cores for maximum performance.
 *********************************************************************************/
volatile uint16_t buckADC_ModuleInitialize(void) 
{
    volatile uint16_t retval=1;
    
    // Make sure power to peripheral is enabled
    volatile bool _pmdlock = PMDCONbits.PMDLOCK; // Copy state of PMD lock bit
    PMDCONbits.PMDLOCK = 0; // Unlock PMD register writes
    PMD1bits.ADC1MD = 0; // ADC Module Power Disable: ADC module power is enabled
    PMDCONbits.PMDLOCK = _pmdlock; // set previous state of PMD lock
    
    // ADCON1L: ADC CONTROL REGISTER 1 LOW
    ADCON1Lbits.ADON = 0; // ADC Enable: ADC module is off during configuration
    ADCON1Lbits.ADSIDL = 0; // ADC Stop in Idle Mode: Continues module operation in Idle mode
    
    // ADCON1H: ADC CONTROL REGISTER 1 HIGH
    ADCON1Hbits.SHRRES = 0b11; // Shared ADC Core Resolution Selection: 12-bit resolution ADC resolution = 12-bit (0...4095 ticks)
    ADCON1Hbits.FORM = 0; // Fractional Data Output Format: Integer

    // ADCON2L: ADC CONTROL REGISTER 2 LOW
    ADCON2Lbits.REFCIE = 0;; // Band Gap and Reference Voltage Ready Common Interrupt Enable: Common interrupt is disabled for the band gap ready event
    ADCON2Lbits.REFERCIE = 0; // Band Gap or Reference Voltage Error Common Interrupt Enable: Disabled
    ADCON2Lbits.EIEN = 1; // Early Interrupts Enable: The early interrupt feature is enabled
    ADCON2Lbits.SHREISEL = 0b111; // Shared Core Early Interrupt Time Selection: Early interrupt is set and interrupt is generated 8 TADCORE clocks prior to when the data are ready
    ADCON2Lbits.SHRADCS = 0b0000001; // Shared ADC Core Input Clock Divider: 2:1 (minimum)

    // ADCON2H: ADC CONTROL REGISTER 2 HIGH
    ADCON2Hbits.SHRSAMC = 8; // Shared ADC Core Sample Time Selection: 8x TADs sampling time 
    ADCON2Hbits.REFERR = 0; // reset error flag
    ADCON2Hbits.REFRDY = 0; // reset bandgap status bit

    // ADCON3L: ADC CONTROL REGISTER 3 LOW
    ADCON3Lbits.REFSEL = 0b000; // ADC Reference Voltage Selection: AVDD-toAVSS
    ADCON3Lbits.SUSPEND = 0; // All ADC Core Triggers Disable: All ADC cores can be triggered
    ADCON3Lbits.SUSPCIE = 0; // Suspend All ADC Cores Common Interrupt Enable: Common interrupt is not generated for suspend ADC cores
    ADCON3Lbits.SUSPRDY = 0; // All ADC Cores Suspended Flag: ADC cores have previous conversions in progress
    ADCON3Lbits.SHRSAMP = 0; // Shared ADC Core Sampling Direct Control: use hardware trigger
    ADCON3Lbits.CNVRTCH = 0; // Software Individual Channel Conversion Trigger: Next individual channel conversion trigger can be generated (not used)
    ADCON3Lbits.SWLCTRG = 0; // Software Level-Sensitive Common Trigger: No software, level-sensitive common triggers are generated (not used)
    ADCON3Lbits.SWCTRG = 0; // Software Common Trigger: Ready to generate the next software common trigger (not used)
    ADCON3Lbits.CNVCHSEL = 0; // Channel Number Selection for Software Individual Channel Conversion Trigger: AN0 (not used)
    
    // ADCON3H: ADC CONTROL REGISTER 3 HIGH
    ADCON3Hbits.CLKSEL = 0b01; // ADC Module Clock Source Selection: AVCODIV
    ADCON3Hbits.CLKDIV = 0b000000; // ADC Module Clock Source Divider: 1 Source Clock Period
    ADCON3Hbits.SHREN = 0; // Shared ADC Core Enable: Shared ADC core is disabled
    ADCON3Hbits.C0EN = 0; // Dedicated ADC Core 0 Enable: Dedicated ADC Core 0 is disabled
    ADCON3Hbits.C1EN = 0; // Dedicated ADC Core 1 Enable: Dedicated ADC Core 1 is disabled
    
    // ADCON4L: ADC CONTROL REGISTER 4 LOW
    ADCON4Lbits.SAMC0EN = 0;  // Dedicated ADC Core 0 Conversion Delay Enable: Immediate conversion
    ADCON4Lbits.SAMC1EN = 0;  // Dedicated ADC Core 1 Conversion Delay Enable: Immediate conversion
    
    // ADCON4H: ADC CONTROL REGISTER 4 HIGH
    ADCON4Hbits.C0CHS = 0b00; // Dedicated ADC Core 0 Input Channel Selection: AN0
    ADCON4Hbits.C1CHS = 0b00; // Dedicated ADC Core 1 Input Channel Selection: AN1

    // ADCON5L: ADC CONTROL REGISTER 5 LOW
    // ADCON5Lbits.SHRRDY: Shared ADC Core Ready Flag (read only)
    // ADCON5Lbits.C0RDY: Dedicated ADC Core 0 Ready Flag (read only)
    // ADCON5Lbits.C1RDY: Dedicated ADC Core 1 Ready Flag (read only)
    ADCON5Lbits.SHRPWR = 0; // Shared ADC Core Power Enable: ADC core is off
    ADCON5Lbits.C0PWR = 0; // Dedicated ADC Core 0 Power Enable: ADC core is off
    ADCON5Lbits.C1PWR = 0; // Dedicated ADC Core 1 Power Enable: ADC core is off
  
    // ADCON5H: ADC CONTROL REGISTER 5 HIGH
    ADCON5Hbits.WARMTIME = 0b1111; // ADC Dedicated Core x Power-up Delay: 32768 Source Clock Periods
    ADCON5Hbits.SHRCIE = 0; // Shared ADC Core Ready Common Interrupt Enable: Common interrupt is disabled for an ADC core ready event
    ADCON5Hbits.C0CIE = 0; // C1CIE: Dedicated ADC Core 0 Ready Common Interrupt Enable: Common interrupt is disabled
    ADCON5Hbits.C1CIE = 0; // C1CIE: Dedicated ADC Core 1 Ready Common Interrupt Enable: Common interrupt is disabled
    
    // ADCORExL: DEDICATED ADC CORE x CONTROL REGISTER LOW
    ADCORE1Lbits.SAMC = 0b0000000000;   // Dedicated ADC Core 1 Conversion Delay Selection: 2 TADCORE (minimum)
    ADCORE0Lbits.SAMC = 0b0000000000;   // Dedicated ADC Core 0 Conversion Delay Selection: 2 TADCORE (minimum)

    // ADCORExH: DEDICATED ADC CORE x CONTROL REGISTER HIGH
    ADCORE0Hbits.RES = 0b11; // ADC Core x Resolution Selection: 12 bit
    ADCORE0Hbits.ADCS = 0b0000000; // ADC Core x Input Clock Divider: 2 Source Clock Periods
    ADCORE0Hbits.EISEL = 0b111; // Early interrupt is set and an interrupt is generated 8 TADCORE clocks prior

    ADCORE1Hbits.RES = 0b11; // ADC Core x Resolution Selection: 12 bit
    ADCORE1Hbits.ADCS = 0b0000000; // ADC Core x Input Clock Divider: 2 Source Clock Periods
    ADCORE1Hbits.EISEL = 0b111; // Early interrupt is set and an interrupt is generated 8 TADCORE clocks prior
    
    return(retval);   
    
} 

/*******************************************************************************
 * @fn	    uint16_t buckADC_ChannelInitialize(volatile struct BUCK_ADC_INPUT_SETTINGS_s* adcInstance)
 * @ingroup lib-layer-buck-pconfig-functions
 * @brief   This function initializes the settings for the ADC channel
 * @param	adcInstance  Pointer to an ADC Input Configuration data object of type struct BUCK_ADC_INPUT_SETTINGS_s
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 * This function initializes the ADC input registers based on the selected ADC channel. 
 * This function sets the input channel trigger source, input mode, and the ADC core 
 * connected to the selected channel. 
 *********************************************************************************/
volatile uint16_t buckADC_ChannelInitialize(volatile struct BUCK_ADC_INPUT_SETTINGS_s* adcInstance) 
{
    volatile uint16_t retval=1;
    volatile uint8_t* ptrADCRegister;
    volatile uint8_t bit_offset;
    
    // Initialize ADC input registers
    if (adcInstance->enabled) {

        // Write level trigger setting
        if (adcInstance->adc_input < 16) {
            ADLVLTRGL |= ((uint16_t)(adcInstance->level_trigger) << adcInstance->adc_input);
            ADEIEL |= ((uint16_t)(adcInstance->early_interrupt_enable) << adcInstance->adc_input);
            ADIEL |= ((uint16_t)(adcInstance->interrupt_enable) << adcInstance->adc_input);
        }
        else if (adcInstance->adc_input < 32) {
            ADLVLTRGH |= ((uint16_t)(adcInstance->level_trigger) << (adcInstance->adc_input - 16));
            ADEIEH |= ((uint16_t)(adcInstance->early_interrupt_enable) << (adcInstance->adc_input - 16));
            ADIEH |= ((uint16_t)(adcInstance->interrupt_enable) << (adcInstance->adc_input - 16));
        }
        else {
            return(0); // ADC input number out of range
        }

        // write input mode setting
        if (adcInstance->adc_input < 8)
            bit_offset = (2 * adcInstance->adc_input);
        else if (adcInstance->adc_input < 16)
            bit_offset = (2 * (adcInstance->adc_input-8));
        else if (adcInstance->adc_input < 24)
            bit_offset = (2 * (adcInstance->adc_input-16));
        else if (adcInstance->adc_input < 32)
            bit_offset = (2 * (adcInstance->adc_input-24));
        else
            return(0); // ADC input number out of range

        ptrADCRegister = (volatile uint8_t *)
            ((volatile uint8_t *)&ADMOD0L + (volatile uint8_t)(adcInstance->adc_input >> 8));
        
        *ptrADCRegister |= ((unsigned int)adcInstance->signed_result << bit_offset);
        *ptrADCRegister |= ((unsigned int)adcInstance->differential_input << (bit_offset + 1));
       
        // Write ADC trigger source setting
        ptrADCRegister = (volatile uint8_t *)
            ((volatile uint8_t *)&ADTRIG0L + (volatile uint8_t)adcInstance->adc_input);
        
        *ptrADCRegister = (volatile uint8_t)adcInstance->trigger_source;
        
        // Register ADC core to be active
        switch (adcInstance->adc_core) {
            case 0:
                adcore_mask |= ADC_CORE0_MASK_INDEX;
                if (adcInstance->differential_input)
                    adcore_diff_mask |= ADC_CORE0_MASK_INDEX;
                break;
            case 1:
                adcore_mask |= ADC_CORE1_MASK_INDEX;
                if (adcInstance->differential_input)
                    adcore_diff_mask |= ADC_CORE1_MASK_INDEX;
                break;
            case 2:
                adcore_mask |= ADC_CORE2_MASK_INDEX;
                if (adcInstance->differential_input)
                    adcore_diff_mask |= ADC_CORE2_MASK_INDEX;
                break;
            case 3:
                adcore_mask |= ADC_CORE3_MASK_INDEX;
                if (adcInstance->differential_input)
                    adcore_diff_mask |= ADC_CORE3_MASK_INDEX;
                break;
            default:
                adcore_mask |= ADC_SHRCORE_MASK_INDEX;
                if (adcInstance->differential_input)
                    adcore_diff_mask |= ADC_SHRCORE_MASK_INDEX;
                break;
        }
        
    }
    
    return(retval);
    
} 

/*******************************************************************************
 * @fn	    uint16_t buckADC_Start(void) 
 * @ingroup lib-layer-buck-pconfig-functions
 * @brief   This function enables the ADC module and starts the ADC cores analog inputs for the required input signals 
 * @param	void
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 * This function enables the ADC module, powers-up and enables the ADC cores used and waits 
 * until ADC cores are ready.
 *********************************************************************************/
volatile uint16_t buckADC_Start(void) 
{
    volatile uint16_t retval=1;
    volatile uint16_t timeout=0;
    volatile uint16_t adcore_mask_compare=0;
    
    // Turn on ADC module
    ADCON1Lbits.ADON = 1;

    ADCON5L = adcore_mask;    // Enable power to all used ADC cores
    adcore_mask_compare = ((adcore_mask << 8) | adcore_mask); // Set ADC Core Ready Bit Mask
    
    while ((ADCON5L != adcore_mask_compare) & (timeout++ < ADC_POWERUP_TIMEOUT)); // Wait until ADC cores are ready
    if (timeout >= ADC_POWERUP_TIMEOUT) return(0); // Skip if powering up ADC cores was unsuccessful
    ADCON3H = adcore_mask; // Enable ADC cores


    return(retval);    
    
} 

/*******************************************************************************
 * @fn	    uint16_t buckGPIO_Set(volatile struct BUCK_GPIO_INSTANCE_s* buckGPIOInstance)
 * @ingroup lib-layer-buck-pconfig-functions
 * @brief   This function sets the selected general purpose input/ouput pins 
 * @param	buckGPIOInstance  Pointer to a GPIO instance data object of type struct BUCK_GPIO_INSTANCE_s
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 * This function captures the user selected pin to be activated through LAT register.
 *********************************************************************************/
volatile uint16_t buckGPIO_Set(volatile struct BUCK_GPIO_INSTANCE_s* buckGPIOInstance)
{
    volatile uint16_t retval=1;
    volatile uint16_t filter_mask=0;
    volatile struct P33C_GPIO_INSTANCE_s* gpio;

    // Capture register of GPIO port
    gpio = p33c_GpioInstance_GetHandle(buckGPIOInstance->port);
    
    // Capture filter mask
    filter_mask = (0x0001 << buckGPIOInstance->pin);
        
    // Set pin to ACTIVE state
    if (buckGPIOInstance->polarity == 0)
        gpio->LATx.value |= (filter_mask); // Set pin bit in register
    else
        gpio->LATx.value &= ~(filter_mask); // Clear pin bit in register            

    // Verifying the set state is applied at the pin
    retval = (bool)((gpio->LATx.value & filter_mask) == (gpio->PORTx.value & filter_mask));
    
    return(retval);
    
}

/*******************************************************************************
 * @fn	    uint16_t buckGPIO_Clear(volatile struct BUCK_GPIO_INSTANCE_s* buckGPIOInstance)
 * @ingroup lib-layer-buck-pconfig-functions
 * @brief   This function clears the selected general purpose input/output pin
 * @param	buckGPIOInstance  Pointer to a GPIO instance data object of type struct BUCK_GPIO_INSTANCE_s
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 * This function captures the pin that the use desired to be put in inactive state. 
 *********************************************************************************/
volatile uint16_t buckGPIO_Clear(volatile struct BUCK_GPIO_INSTANCE_s* buckGPIOInstance)
{
    volatile uint16_t retval=1;
    volatile uint16_t filter_mask=0;
    volatile struct P33C_GPIO_INSTANCE_s* gpio;

    // Capture register of GPIO port
    gpio = p33c_GpioInstance_GetHandle(buckGPIOInstance->port);
    
    // Capture filter mask
    filter_mask = (0x0001 << buckGPIOInstance->pin);
        
    // Set pin to INACTIVE state
    if (buckGPIOInstance->polarity == 0)
        gpio->LATx.value &= ~(filter_mask); // Clear pin bit in register            
    else
        gpio->LATx.value |= (filter_mask); // Set pin bit in register
    
    // Verifying the set state is applied at the pin
    retval = (bool)((gpio->LATx.value & filter_mask) == (gpio->PORTx.value & filter_mask));
    
    return(retval);
    
}

/*******************************************************************************
 * @fn	    bool buckGPIO_GetPinState(volatile struct BUCK_GPIO_INSTANCE_s* buckGPIOInstance)
 * @ingroup lib-layer-buck-pconfig-functions
 * @brief   This function gets the state of the selected pin
 * @param	buckGPIOInstance  Pointer to a GPIO instance data object of type struct BUCK_GPIO_INSTANCE_s
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 * This function captures the selected pin and read its state.
 *********************************************************************************/
volatile bool buckGPIO_GetPinState(volatile struct BUCK_GPIO_INSTANCE_s* buckGPIOInstance)
{
    volatile bool retval=1;
    volatile P33C_GPIO_INSTANCE_t* gpio;

    // Capture register of GPIO port
    gpio = p33c_GpioInstance_GetHandle(buckGPIOInstance->port);
    
    // Read pin 
    retval = (bool)(gpio->PORTx.value & (0x0001 << buckGPIOInstance->pin));
    
    // If polarity is inverted (ACTIVE LOW), invert result
    if(buckGPIOInstance->polarity == 1)
        retval = (1-retval);
    
    return(retval);
    
} 


/*******************************************************************************
 * @fn	    uint16_t buckGPIO_Initialize(volatile struct BUCK_CONVERTER_s* buckInstance)
 * @ingroup lib-layer-buck-pconfig-functions
 * @brief   This function initializes the buck input pins
 * @param	buckInstance  Pointer to a Buck Converter data object of type struct BUCK_CONVERTER_s
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 * This function initializes the ENABLE input pin and POWER GOOD output pin using the 
 * buck GPIO_PrivateInitialize (URL = @ref buckGPIO_PrivateInitialize).
 *********************************************************************************/
volatile uint16_t buckGPIO_Initialize(volatile struct BUCK_CONVERTER_s* buckInstance)
{
    volatile uint16_t retval=1;

    // Initialize ENABLE input pin
    if(buckInstance->gpio.EnableInput.enabled)
        retval = buckGPIO_PrivateInitialize(&buckInstance->gpio.EnableInput);
    
    // Initialize POWER GOOD output pin
    if(buckInstance->gpio.PowerGood.enabled)
        retval = buckGPIO_PrivateInitialize(&buckInstance->gpio.PowerGood);

    // If defined, reset POWER_GOOD output
    if(buckInstance->gpio.PowerGood.enabled)
        retval &= buckGPIO_Clear(&buckInstance->gpio.PowerGood);

    return(retval);
    
} 


/*******************************************************************************
 * @fn	    uint16_t buckGPIO_PrivateInitialize(volatile struct BUCK_GPIO_INSTANCE_s* buckGPIOInstance)
 * @ingroup lib-layer-buck-pconfig-functions
 * @brief   This function sets the pin as input or output 
 * @param	buckGPIOInstance  Pointer to a GPIO instance data object of type struct BUCK_GPIO_INSTANCE_s
 * @return  unsigned integer (0=failure, 1=success)
 *  
 * @details
 * This function captures the user selected pin then set the pin to inactive and
 * set it as digital input or output pin. 
 *********************************************************************************/
volatile uint16_t buckGPIO_PrivateInitialize(volatile struct BUCK_GPIO_INSTANCE_s* buckGPIOInstance)
{
    volatile uint16_t retval=1;
    volatile struct P33C_GPIO_INSTANCE_s* gpio;

    // Capture register of GPIO port
    gpio = p33c_GpioInstance_GetHandle(buckGPIOInstance->port);
    
    // Set pin to INACTIVE state
    if (buckGPIOInstance->polarity == 0)
        gpio->LATx.value &= ~(0x0001 << buckGPIOInstance->pin); // Clear pin bit in register            
    else
        gpio->LATx.value |= (0x0001 << buckGPIOInstance->pin); // Set pin bit in register

    // Set INPUT or OUTPUT in TRIS register
    if(buckGPIOInstance->io_type == 1)
        gpio->TRISx.value |= (0x0001 << buckGPIOInstance->pin); // Set pin bit in register
    else 
        gpio->TRISx.value &= ~(0x0001 << buckGPIOInstance->pin); // Clear pin bit in register

    // Set Pin in Open Drain Configuration
    if(buckGPIOInstance->io_type == 2)
        gpio->ODCx.value |= (0x0001 << buckGPIOInstance->pin); // Set pin bit in register
    else
        gpio->ODCx.value &= ~(0x0001 << buckGPIOInstance->pin); // Clear pin bit in register
    
    // Set pin as DIGITAL IO
    gpio->ANSELx.value &= ~(0x0001 << buckGPIOInstance->pin); // Clear pin bit in register

    // ToDo: Enable register value testing to validate function result
    retval = 1;

    return(retval);
}

// end of file

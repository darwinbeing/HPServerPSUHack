/*
 * File:   dev_buck_ptemp_dac.c
 * Author: M91406
 *
 * Created on October 29, 2020, 10:31 AM
 */

#include "xc16_pral.h" // include peripehral register abstraction layer drivers
#include "dev_buck_ptemp_dac.h" // include peripheral register configuration values header

/****************************************************************************************************
 * @ingroup lib-layer-buck-ptemplate-properties-variables
 * @var buckDacModuleConfig
 * @brief DAC module default configuration
 *****************************************************************************************************/
volatile struct P33C_DAC_MODULE_s buckDacModuleConfig = 
{
    .DacModuleCtrl1L.value = 0x0000, ///< DACCTRL1L: DAC CONTROL 1 LOW REGISTER
    .DacModuleCtrl2L.value = 0x0000, ///< DACCTRL2L: DAC CONTROL 2 LOW REGISTER
    .DacModuleCtrl2H.value = 0x0000  ///< DACCTRL2H: DAC CONTROL 2 HIGH REGISTER
};


/****************************************************************************************************
 * @ingroup lib-layer-buck-ptemplate-properties-variables
 * @var buckDacInstanceConfig
 * @brief DAC generator default configuration
 *****************************************************************************************************/
volatile struct P33C_DAC_INSTANCE_s buckDacInstanceConfig = 
{
    .DACxCONL.value = REG_DACxCONL, ///< DACxCONL: DACx CONTROL LOW REGISTER
    .DACxCONH.value = REG_DACxCONH, ///< DACxCONH: DACx CONTROL HIGH REGISTER
    .DACxDATL.value = 0x0000,       ///< DACxDATH: DACx DATA HIGH REGISTER
    .DACxDATH.value = 0x0000,       ///< DACxDATL: DACx DATA LOW REGISTER
    .SLPxCONL.value = REG_SLPxCONL, ///< SLPxCONL: DACx SLOPE CONTROL LOW REGISTE
    .SLPxCONH.value = REG_SLPxCONH, ///< SLPxCONH: DACx SLOPE CONTROL HIGH REGISTE
    .SLPxDAT.value = 0x0000         ///< SLPxCONL: DACx SLOPE CONTROL LOW REGISTER
};

// end of file

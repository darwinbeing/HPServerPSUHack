/*
 * File:   dev_buck_ptemp_pwm.c
 * Author: M91406
 *
 * Created on October 29, 2020, 10:31 AM
 */

#include "xc16_pral.h" // include peripehral register abstraction layer drivers
#include "dev_buck_ptemp_pwm.h" // include peripheral register configuration values header

/****************************************************************************************************
 * @ingroup lib-layer-buck-ptemplate-properties-variables
 * @var buckPwmModuleConfig
 * @brief PWM module default configuration
 *****************************************************************************************************/
volatile struct P33C_PWM_MODULE_s buckPwmModuleConfig = 
{
        .vPCLKCON.value = REG_PCLKCON,  ///< PCLKCON: PWM CLOCK CONTROL REGISTER
        .vFSCL.value = 0x0000,          ///< FSCL: FREQUENCY SCALE REGISTER
        .vFSMINPER.value = 0x0000,      ///< FSMINPER: FREQUENCY SCALING MINIMUM PERIOD REGISTER
        .vMPHASE.value = 0x0000,        ///< MPHASE: MASTER PHASE REGISTER
        .vMDC.value = 0x0000,           ///< MDC: MASTER DUTY CYCLE REGISTER
        .vMPER.value = 0x0000,          ///< MPER: MASTER PERIOD REGISTER
        .vLFSR.value = 0x0000,          ///< LFSR: LINEAR FEEDBACK SHIFT REGISTER
        .vCMBTRIGL.value = REG_CMBTRIGL,///< CMBTRIGL: COMBINATIONAL TRIGGER REGISTER LOW
        .vCMBTRIGH.value = REG_CMBTRIGH,///< CMBTRIGH: COMBINATIONAL TRIGGER REGISTER HIGH
        .LOGCON_A.value = REG_LOGCONA,  ///< LOGCONA: COMBINATORIAL PWM LOGIC CONTROL REGISTER A
        .LOGCON_B.value = REG_LOGCONB,  ///< LOGCONB: COMBINATORIAL PWM LOGIC CONTROL REGISTER B
        .LOGCON_C.value = REG_LOGCONC,  ///< LOGCONC: COMBINATORIAL PWM LOGIC CONTROL REGISTER C
        .LOGCON_D.value = REG_LOGCOND,  ///< LOGCOND: COMBINATORIAL PWM LOGIC CONTROL REGISTER D
        .LOGCON_E.value = REG_LOGCONE,  ///< LOGCONE: COMBINATORIAL PWM LOGIC CONTROL REGISTER E
        .LOGCON_F.value = REG_LOGCONF,  ///< LOGCONF: COMBINATORIAL PWM LOGIC CONTROL REGISTER F
        .PWMEVT_A.value = REG_PWMEVTA,  ///< PWMEVTA: PWM EVENT OUTPUT CONTROL REGISTER A
        .PWMEVT_B.value = REG_PWMEVTB,  ///< PWMEVTB: PWM EVENT OUTPUT CONTROL REGISTER B
        .PWMEVT_C.value = REG_PWMEVTC,  ///< PWMEVTC: PWM EVENT OUTPUT CONTROL REGISTER C
        .PWMEVT_D.value = REG_PWMEVTD,  ///< PWMEVTD: PWM EVENT OUTPUT CONTROL REGISTER D
        .PWMEVT_E.value = REG_PWMEVTE,  ///< PWMEVTE: PWM EVENT OUTPUT CONTROL REGISTER E
        .PWMEVT_F.value = REG_PWMEVTF   ///< PWMEVTF: PWM EVENT OUTPUT CONTROL REGISTER F
    };


/****************************************************************************************************
 * @ingroup lib-layer-buck-ptemplate-properties-variables
 * @var buckPwmGeneratorConfig
 * @brief PWM generator default configuration
 *****************************************************************************************************/
volatile struct P33C_PWM_GENERATOR_s buckPwmGeneratorConfig = 
{
        .PGxCONL.value = REG_PGxCONL,       ///< PGxCONL: PWM GENERATOR x CONTROL REGISTER LOW
        .PGxCONH.value = REG_PGxCONH,       ///< PGxCONH: PWM GENERATOR x CONTROL REGISTER HIGH
        .PGxSTAT.value = 0x0000,            ///< PGxSTAT: Clear status bits 
        .PGxIOCONL.value = REG_PGxIOCONL,   ///< PGxIOCONL: PWM GENERATOR x I/O CONTROL REGISTER LOW
        .PGxIOCONH.value = REG_PGxIOCONH,   ///< PGxIOCONL: PWM GENERATOR x I/O CONTROL REGISTER HIGH
        .PGxEVTL.value = REG_PGxEVTL,       ///< PGxEVTL: PWM GENERATOR x EVENT REGISTER LOW
        .PGxEVTH.value = REG_PGxEVTH,       ///< PGxEVTH: PWM GENERATOR x EVENT REGISTER HIGH
        .PGxCLPCIL.value = REG_PGxCLPCIL,   ///< PGxLCPCIL: PWM GENERATOR x CURRENT LIMIT PCI REGISTER LOW
        .PGxCLPCIH.value = REG_PGxCLPCIH,   ///< PGxLCPCIL: PWM GENERATOR x CURRENT LIMIT PCI REGISTER HIGH
        .PGxFPCIL.value = REG_PGxFPCIL,     ///< PGxFPCIL: PWM GENERATOR x FAULT PCI REGISTER LOW
        .PGxFPCIH.value = REG_PGxFPCIH,     ///< PGxFPCIL: PWM GENERATOR x FAULT PCI REGISTER HIGH
        .PGxFFPCIL.value = REG_PGxFFPCIL,   ///< PGxFFPCIL: PWM GENERATOR x FEED FORWARD PCI REGISTER LOW
        .PGxFFPCIH.value = REG_PGxFFPCIH,   ///< PGxFFPCIL: PWM GENERATOR x FEED FORWARD PCI REGISTER HIGH
        .PGxSPCIL.value = REG_PGxSPCIL,     ///< PGxSPCIL: PWM GENERATOR x SOFTWARE PCI REGISTER LOW
        .PGxSPCIH.value = REG_PGxSPCIH,     ///< PGxSPCIL: PWM GENERATOR x SOFTWARE PCI REGISTER HIGH
        .PGxLEBL.value = REG_PGxLEBL,       ///< PGxLEBL: PWM GENERATOR x LEADING-EDGE BLANKING REGISTER LOW
        .PGxLEBH.value = REG_PGxLEBH,       ///< PGxLEBL: PWM GENERATOR x LEADING-EDGE BLANKING REGISTER HIGH
        .PGxPHASE.value = 0x0000,           ///< PGxPHASE: PWM GENERATOR x PHASE REGISTER
        .PGxDC.value = 0x0000,              ///< PGxDC: PWM GENERATOR x DUTY CYCLE REGISTER
        .PGxDCA.value = 0x0000,             ///< PGxDCA: PWM GENERATOR x DUTY CYCLE ADJUSTMENT REGISTER
        .PGxPER.value = 0x0000,             ///< PGxPER: PWM GENERATOR x PERIOD REGISTER
        .PGxTRIGA.value = 0x0000,           ///< PGxTRIGA: PWM GENERATOR x TRIGGER A REGISTER
        .PGxTRIGB.value = 0x0000,           ///< PGxTRIGA: PWM GENERATOR x TRIGGER B REGISTER
        .PGxTRIGC.value = 0x0000,           ///< PGxTRIGA: PWM GENERATOR x TRIGGER C REGISTER
        .PGxDTL.value = 0x0000,             ///< PGxDTL: PWM GENERATOR x DEAD TIME REGISTER LOW
        .PGxDTH.value = 0x0000,             ///< PGxDTH: PWM GENERATOR x DEAD TIME REGISTER HIGH
        .PGxCAP.value = 0x0000              ///< PGxCAP: PWM GENERATOR x CAPTURE REGISTER
};

// end of file


#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/LLC_ACMC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/LLC_ACMC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../SRDutyCycle.c ../alt_wreg_setup.S ../delay.s ../init.c ../isr.S ../main.c ../protection.c ../smps_2p2z_dspic_v2.s ../softstart.c ../traps.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/SRDutyCycle.o ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o ${OBJECTDIR}/_ext/1472/delay.o ${OBJECTDIR}/_ext/1472/init.o ${OBJECTDIR}/_ext/1472/isr.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/protection.o ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o ${OBJECTDIR}/_ext/1472/softstart.o ${OBJECTDIR}/_ext/1472/traps.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/SRDutyCycle.o.d ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.d ${OBJECTDIR}/_ext/1472/delay.o.d ${OBJECTDIR}/_ext/1472/init.o.d ${OBJECTDIR}/_ext/1472/isr.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/protection.o.d ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o.d ${OBJECTDIR}/_ext/1472/softstart.o.d ${OBJECTDIR}/_ext/1472/traps.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/SRDutyCycle.o ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o ${OBJECTDIR}/_ext/1472/delay.o ${OBJECTDIR}/_ext/1472/init.o ${OBJECTDIR}/_ext/1472/isr.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/protection.o ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o ${OBJECTDIR}/_ext/1472/softstart.o ${OBJECTDIR}/_ext/1472/traps.o

# Source Files
SOURCEFILES=../SRDutyCycle.c ../alt_wreg_setup.S ../delay.s ../init.c ../isr.S ../main.c ../protection.c ../smps_2p2z_dspic_v2.s ../softstart.c ../traps.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/LLC_ACMC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP16GS502
MP_LINKER_FILE_OPTION=,--script=p33EP16GS502.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/SRDutyCycle.o: ../SRDutyCycle.c  .generated_files/flags/default/7eceec5d566fc074a5ecc31f9b90fc738f1d016f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/SRDutyCycle.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/SRDutyCycle.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SRDutyCycle.c  -o ${OBJECTDIR}/_ext/1472/SRDutyCycle.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/SRDutyCycle.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/init.o: ../init.c  .generated_files/flags/default/70dfc2a13967ca2cf92ea8cbf6fb11c5e3e1d1a8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../init.c  -o ${OBJECTDIR}/_ext/1472/init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/init.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/4ef4a4729bfd66a92219b067ffbb19317e27bb89 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/protection.o: ../protection.c  .generated_files/flags/default/7081b05aa591ecd0f77fbe7e7a7f1d9bd0d8df50 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/protection.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/protection.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../protection.c  -o ${OBJECTDIR}/_ext/1472/protection.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/protection.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/softstart.o: ../softstart.c  .generated_files/flags/default/335752cdb89dc173336a258344e3e38928925baf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/softstart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/softstart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../softstart.c  -o ${OBJECTDIR}/_ext/1472/softstart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/softstart.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/traps.o: ../traps.c  .generated_files/flags/default/e5eeace20fadbfaa7eb052d3ef0358b50ad75815 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/traps.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../traps.c  -o ${OBJECTDIR}/_ext/1472/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/traps.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
else
${OBJECTDIR}/_ext/1472/SRDutyCycle.o: ../SRDutyCycle.c  .generated_files/flags/default/909a1273eeba3a22145e75742e7c98488497d7fb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/SRDutyCycle.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/SRDutyCycle.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SRDutyCycle.c  -o ${OBJECTDIR}/_ext/1472/SRDutyCycle.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/SRDutyCycle.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/init.o: ../init.c  .generated_files/flags/default/98601fdfe601f7b6a86f50ffec5c7f56f65ebd56 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../init.c  -o ${OBJECTDIR}/_ext/1472/init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/init.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/a1a3958f092eaa84f2a305ad8b5c6e2236db14d9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/protection.o: ../protection.c  .generated_files/flags/default/e4db5eaa887b071992e444f6f50765fb09cab9f4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/protection.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/protection.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../protection.c  -o ${OBJECTDIR}/_ext/1472/protection.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/protection.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/softstart.o: ../softstart.c  .generated_files/flags/default/f755094b9275a0b65de82601d185cf8d59b28cdb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/softstart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/softstart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../softstart.c  -o ${OBJECTDIR}/_ext/1472/softstart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/softstart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/traps.o: ../traps.c  .generated_files/flags/default/d71875855cd649b01452904b08db2a9cb43d599 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/traps.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../traps.c  -o ${OBJECTDIR}/_ext/1472/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/traps.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/delay.o: ../delay.s  .generated_files/flags/default/5ae56e7236fc9e545783c75fbbbbaa834bf2a49e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/delay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/delay.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../delay.s  -o ${OBJECTDIR}/_ext/1472/delay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/_ext/1472/delay.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,-g,--no-relax$(MP_EXTRA_AS_POST) 
	
${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o: ../smps_2p2z_dspic_v2.s  .generated_files/flags/default/2cdceb41e1a78b119e765c6d0e4694ab606ef7cc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../smps_2p2z_dspic_v2.s  -o ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,-g,--no-relax$(MP_EXTRA_AS_POST) 
	
else
${OBJECTDIR}/_ext/1472/delay.o: ../delay.s  .generated_files/flags/default/f75a79c568788a80fdd1d3c4ef279ce4c6b52f28 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/delay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/delay.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../delay.s  -o ${OBJECTDIR}/_ext/1472/delay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/_ext/1472/delay.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST) 
	
${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o: ../smps_2p2z_dspic_v2.s  .generated_files/flags/default/b00ff92cd76e8477dbd80cd2cb4fb520a957c890 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../smps_2p2z_dspic_v2.s  -o ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/alt_wreg_setup.o: ../alt_wreg_setup.S  .generated_files/flags/default/a0854a45414e3ffce83be09d58cf9d2f3a7dd11c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../alt_wreg_setup.S  -o ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.d"  -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,-g,--no-relax$(MP_EXTRA_AS_POST) 
	
${OBJECTDIR}/_ext/1472/isr.o: ../isr.S  .generated_files/flags/default/4aabbefcbe443bd8ba42de2dae35ae93e8947285 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/isr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/isr.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../isr.S  -o ${OBJECTDIR}/_ext/1472/isr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/isr.o.d"  -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/_ext/1472/isr.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,-g,--no-relax$(MP_EXTRA_AS_POST) 
	
else
${OBJECTDIR}/_ext/1472/alt_wreg_setup.o: ../alt_wreg_setup.S  .generated_files/flags/default/fc5025b420e1b1f44ec6378981a22d4c36b8cbe8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../alt_wreg_setup.S  -o ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.d"  -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST) 
	
${OBJECTDIR}/_ext/1472/isr.o: ../isr.S  .generated_files/flags/default/6292294465c10e859b02999bd26a369f6785199d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/isr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/isr.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../isr.S  -o ${OBJECTDIR}/_ext/1472/isr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/isr.o.d"  -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/_ext/1472/isr.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/LLC_ACMC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/LLC_ACMC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	
else
${DISTDIR}/LLC_ACMC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/LLC_ACMC.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	${MP_CC_DIR}/xc16-bin2hex ${DISTDIR}/LLC_ACMC.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

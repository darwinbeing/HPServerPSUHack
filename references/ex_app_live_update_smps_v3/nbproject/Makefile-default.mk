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
FINAL_IMAGE=${DISTDIR}/ex_app_live_update_smps_v3.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/ex_app_live_update_smps_v3.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=./src/init.c ./src/isr.c ./src/lcd.c src/smps_3p3z_dspic.s src/init_Alt_W_Registers.S ezbl_uart_dual_partition.c i2c.c main.c src/compensator_isr.S

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/init.o ${OBJECTDIR}/src/isr.o ${OBJECTDIR}/src/lcd.o ${OBJECTDIR}/src/smps_3p3z_dspic.o ${OBJECTDIR}/src/init_Alt_W_Registers.o ${OBJECTDIR}/ezbl_uart_dual_partition.o ${OBJECTDIR}/i2c.o ${OBJECTDIR}/main.o ${OBJECTDIR}/src/compensator_isr.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/init.o.d ${OBJECTDIR}/src/isr.o.d ${OBJECTDIR}/src/lcd.o.d ${OBJECTDIR}/src/smps_3p3z_dspic.o.d ${OBJECTDIR}/src/init_Alt_W_Registers.o.d ${OBJECTDIR}/ezbl_uart_dual_partition.o.d ${OBJECTDIR}/i2c.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/src/compensator_isr.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/init.o ${OBJECTDIR}/src/isr.o ${OBJECTDIR}/src/lcd.o ${OBJECTDIR}/src/smps_3p3z_dspic.o ${OBJECTDIR}/src/init_Alt_W_Registers.o ${OBJECTDIR}/ezbl_uart_dual_partition.o ${OBJECTDIR}/i2c.o ${OBJECTDIR}/main.o ${OBJECTDIR}/src/compensator_isr.o

# Source Files
SOURCEFILES=./src/init.c ./src/isr.c ./src/lcd.c src/smps_3p3z_dspic.s src/init_Alt_W_Registers.S ezbl_uart_dual_partition.c i2c.c main.c src/compensator_isr.S



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/ex_app_live_update_smps_v3.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ64GS606
MP_LINKER_FILE_OPTION=,--script=p33FJ64GS606.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/init.o: ./src/init.c  .generated_files/flags/default/e8a845acdc376c889a4c2eea2bd90b7061a00be5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/init.o.d 
	@${RM} ${OBJECTDIR}/src/init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ./src/init.c  -o ${OBJECTDIR}/src/init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/init.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"h" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/isr.o: ./src/isr.c  .generated_files/flags/default/19a84822d0f0afb7a177222ad94711b617aed54d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/isr.o.d 
	@${RM} ${OBJECTDIR}/src/isr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ./src/isr.c  -o ${OBJECTDIR}/src/isr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/isr.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"h" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/lcd.o: ./src/lcd.c  .generated_files/flags/default/c888fbc4c8ba9a31b3feb4fb7c5cb5f46792eb87 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/lcd.o.d 
	@${RM} ${OBJECTDIR}/src/lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ./src/lcd.c  -o ${OBJECTDIR}/src/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/lcd.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"h" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/ezbl_uart_dual_partition.o: ezbl_uart_dual_partition.c  .generated_files/flags/default/35c3e1c2f73518ae96fcfd7f0795b34e0d961300 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ezbl_uart_dual_partition.o.d 
	@${RM} ${OBJECTDIR}/ezbl_uart_dual_partition.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ezbl_uart_dual_partition.c  -o ${OBJECTDIR}/ezbl_uart_dual_partition.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ezbl_uart_dual_partition.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"h" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/i2c.o: i2c.c  .generated_files/flags/default/4c8a667fb8cf7cffa55665c0082018d754e178bb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/i2c.o.d 
	@${RM} ${OBJECTDIR}/i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  i2c.c  -o ${OBJECTDIR}/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/i2c.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"h" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/116d4e098f52ab0d8a2bb8ed7e34c64ff9b9a108 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"h" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/src/init.o: ./src/init.c  .generated_files/flags/default/89b9cce9b67db8fff69ab330766035c65474f083 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/init.o.d 
	@${RM} ${OBJECTDIR}/src/init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ./src/init.c  -o ${OBJECTDIR}/src/init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/init.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"h" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/isr.o: ./src/isr.c  .generated_files/flags/default/1b790985dac34427b3cb77c4047de98011375957 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/isr.o.d 
	@${RM} ${OBJECTDIR}/src/isr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ./src/isr.c  -o ${OBJECTDIR}/src/isr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/isr.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"h" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/lcd.o: ./src/lcd.c  .generated_files/flags/default/c706153689d1599cdf76974243d0995b1822227b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/lcd.o.d 
	@${RM} ${OBJECTDIR}/src/lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ./src/lcd.c  -o ${OBJECTDIR}/src/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/lcd.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"h" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/ezbl_uart_dual_partition.o: ezbl_uart_dual_partition.c  .generated_files/flags/default/3e3bf8d803dd5718c5caece862a997cb5a743e53 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ezbl_uart_dual_partition.o.d 
	@${RM} ${OBJECTDIR}/ezbl_uart_dual_partition.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ezbl_uart_dual_partition.c  -o ${OBJECTDIR}/ezbl_uart_dual_partition.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ezbl_uart_dual_partition.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"h" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/i2c.o: i2c.c  .generated_files/flags/default/d3c8db08c7fcda0ae05542b08b745e3bf6af24e8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/i2c.o.d 
	@${RM} ${OBJECTDIR}/i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  i2c.c  -o ${OBJECTDIR}/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/i2c.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"h" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/4eb25e17644d423b4274a3aff4094d46061301a5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -fdata-sections -O1 -I"h" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/smps_3p3z_dspic.o: src/smps_3p3z_dspic.s  .generated_files/flags/default/86f99565f99e1e4591b5e3dc23246d854ba3fe3e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/smps_3p3z_dspic.o.d 
	@${RM} ${OBJECTDIR}/src/smps_3p3z_dspic.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/smps_3p3z_dspic.s  -o ${OBJECTDIR}/src/smps_3p3z_dspic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/src/smps_3p3z_dspic.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/src/smps_3p3z_dspic.o: src/smps_3p3z_dspic.s  .generated_files/flags/default/bbcc9aeed459d11e9b38d1d1e8accd2efa9ed291 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/smps_3p3z_dspic.o.d 
	@${RM} ${OBJECTDIR}/src/smps_3p3z_dspic.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/smps_3p3z_dspic.s  -o ${OBJECTDIR}/src/smps_3p3z_dspic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/src/smps_3p3z_dspic.o.d",--defsym=__MPLAB_BUILD=1,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/init_Alt_W_Registers.o: src/init_Alt_W_Registers.S  .generated_files/flags/default/cf254335eb98b625f6c52aa01ae2533748a517b0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/init_Alt_W_Registers.o.d 
	@${RM} ${OBJECTDIR}/src/init_Alt_W_Registers.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/init_Alt_W_Registers.S  -o ${OBJECTDIR}/src/init_Alt_W_Registers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/init_Alt_W_Registers.o.d"  -D__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/src/init_Alt_W_Registers.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/compensator_isr.o: src/compensator_isr.S  .generated_files/flags/default/154425a705d56def6a7b6a9f828c07ed0e73ea1d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/compensator_isr.o.d 
	@${RM} ${OBJECTDIR}/src/compensator_isr.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/compensator_isr.S  -o ${OBJECTDIR}/src/compensator_isr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/compensator_isr.o.d"  -D__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/src/compensator_isr.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/src/init_Alt_W_Registers.o: src/init_Alt_W_Registers.S  .generated_files/flags/default/e4816d0d3954146365de8a6a34204314f35075de .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/init_Alt_W_Registers.o.d 
	@${RM} ${OBJECTDIR}/src/init_Alt_W_Registers.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/init_Alt_W_Registers.S  -o ${OBJECTDIR}/src/init_Alt_W_Registers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/init_Alt_W_Registers.o.d"  -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/src/init_Alt_W_Registers.o.asm.d",--defsym=__MPLAB_BUILD=1,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/compensator_isr.o: src/compensator_isr.S  .generated_files/flags/default/9ef057e475c6e9429c4df7e8457f037ccc9ed130 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/compensator_isr.o.d 
	@${RM} ${OBJECTDIR}/src/compensator_isr.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/compensator_isr.S  -o ${OBJECTDIR}/src/compensator_isr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/compensator_isr.o.d"  -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/src/compensator_isr.o.asm.d",--defsym=__MPLAB_BUILD=1,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/ex_app_live_update_smps_v3.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ezbl_integration/ezbl_lib.a  
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/ex_app_live_update_smps_v3.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ezbl_integration/ezbl_lib.a  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/ex_app_live_update_smps_v3.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ezbl_integration/ezbl_lib.a 
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/ex_app_live_update_smps_v3.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ezbl_integration/ezbl_lib.a  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}/xc16-bin2hex ${DISTDIR}/ex_app_live_update_smps_v3.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif

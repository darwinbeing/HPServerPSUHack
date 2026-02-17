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
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/LLC_VMC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/LLC_VMC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/LLC_VMC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP16GS502
MP_LINKER_FILE_OPTION=,--script=p33EP16GS502.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/SRDutyCycle.o: ../SRDutyCycle.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/SRDutyCycle.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/SRDutyCycle.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SRDutyCycle.c  -o ${OBJECTDIR}/_ext/1472/SRDutyCycle.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/SRDutyCycle.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-data -O0 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/SRDutyCycle.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/init.o: ../init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../init.c  -o ${OBJECTDIR}/_ext/1472/init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/init.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-data -O0 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/init.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-data -O0 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/protection.o: ../protection.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/protection.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/protection.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../protection.c  -o ${OBJECTDIR}/_ext/1472/protection.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/protection.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-data -O0 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/protection.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/softstart.o: ../softstart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/softstart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/softstart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../softstart.c  -o ${OBJECTDIR}/_ext/1472/softstart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/softstart.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-data -O0 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/softstart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/traps.o: ../traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/traps.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../traps.c  -o ${OBJECTDIR}/_ext/1472/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/traps.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-data -O0 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/traps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1472/SRDutyCycle.o: ../SRDutyCycle.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/SRDutyCycle.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/SRDutyCycle.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SRDutyCycle.c  -o ${OBJECTDIR}/_ext/1472/SRDutyCycle.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/SRDutyCycle.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-data -O0 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/SRDutyCycle.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/init.o: ../init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../init.c  -o ${OBJECTDIR}/_ext/1472/init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/init.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-data -O0 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/init.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-data -O0 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/protection.o: ../protection.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/protection.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/protection.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../protection.c  -o ${OBJECTDIR}/_ext/1472/protection.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/protection.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-data -O0 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/protection.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/softstart.o: ../softstart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/softstart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/softstart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../softstart.c  -o ${OBJECTDIR}/_ext/1472/softstart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/softstart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-data -O0 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/softstart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/traps.o: ../traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/traps.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../traps.c  -o ${OBJECTDIR}/_ext/1472/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/traps.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-data -O0 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/traps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/delay.o: ../delay.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/delay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/delay.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../delay.s  -o ${OBJECTDIR}/_ext/1472/delay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,-MD,"${OBJECTDIR}/_ext/1472/delay.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/delay.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o: ../smps_2p2z_dspic_v2.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../smps_2p2z_dspic_v2.s  -o ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,-MD,"${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1472/delay.o: ../delay.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/delay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/delay.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../delay.s  -o ${OBJECTDIR}/_ext/1472/delay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,-MD,"${OBJECTDIR}/_ext/1472/delay.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/delay.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o: ../smps_2p2z_dspic_v2.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../smps_2p2z_dspic_v2.s  -o ${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,-MD,"${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/smps_2p2z_dspic_v2.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/alt_wreg_setup.o: ../alt_wreg_setup.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../alt_wreg_setup.S  -o ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.d"  -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,-MD,"${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.d" "${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/isr.o: ../isr.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/isr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/isr.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../isr.S  -o ${OBJECTDIR}/_ext/1472/isr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/isr.o.d"  -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,-MD,"${OBJECTDIR}/_ext/1472/isr.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/isr.o.d" "${OBJECTDIR}/_ext/1472/isr.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1472/alt_wreg_setup.o: ../alt_wreg_setup.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../alt_wreg_setup.S  -o ${OBJECTDIR}/_ext/1472/alt_wreg_setup.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.d"  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,-MD,"${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.d" "${OBJECTDIR}/_ext/1472/alt_wreg_setup.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/isr.o: ../isr.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/isr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/isr.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../isr.S  -o ${OBJECTDIR}/_ext/1472/isr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/isr.o.d"  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,-MD,"${OBJECTDIR}/_ext/1472/isr.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/isr.o.d" "${OBJECTDIR}/_ext/1472/isr.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/LLC_VMC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/LLC_VMC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/LLC_VMC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/LLC_VMC.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/LLC_VMC.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif

################################################################################
# Easy Bootloader Library Dual Partition Bootloader + Application make script  #
################################################################################
# To use, put "include ezbl_integration/ezbl_dual_partition.mk" at the very 
# bottom of your combined dual partition bootloader + application project's 
# Makefile, immediately after the nbproject/Makefile-local-${CONF}.mk file has 
# been included.

# Bootloader product identification data to avoid bricking through upload of 
# non-applicable Application firmware images. These strings will combine into a 
# static, globally unique identification hash to represent this booloader.
#
# Any Application firmware sent to the Bootloader will be tested for 
# compatibility with the system hardware (via the BOOTID_HASH) in advance of 
# erasing or programming anything.
BOOTID_VENDOR = "OMIXOM SRL"
BOOTID_MODEL  = "OMX-RealTime-FOTA"
BOOTID_NAME   = "Mgleria development board"
BOOTID_OTHER  = "dual flash partition device"


# Application version for optional version order testing before erasing an
# existing App. The APPID_VER_BUILD number auto pre-increments by
# APPID_VER_BUILD_INC on each make and can be set to 0 to suppress automatic
# changes. APPID_VER_MAJOR/APPID_VER_MINOR are manually set here and
# APPID_VER_BUILD can also be changed manually here to define a starting
# integer.
APPID_VER_MAJOR = 0
APPID_VER_MINOR = 1
APPID_VER_BUILD = 572

APPID_VER_BUILD_INC = 1




# Post-build code to generate .bl2 file from .elf and attempt upload to bootloader
ezbl_post_build: .build-impl
# Create a binary .bl2 file from the .elf file and also if a loadable exists, convert he unified .hex file. The .elf file is needed so it works in debug mode and also so we can obtain the BOOTID_HASH meta-data.
	@echo EZBL: Converting .elf/.hex file to a binary image
	-test "${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.bl2" -nt "${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.elf" || ${MP_JAVA_PATH}java -jar "${thisMakefileDir}ezbl_tools.jar" --blobber -artifact="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.elf"
	@test "${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.unified.hex" -nt "${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.unified.bl2" && ${MP_JAVA_PATH}java -jar "${thisMakefileDir}ezbl_tools.jar" --blobber -artifact="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.unified.hex" || true

ifneq (,${filter default uart,${CONF}})   # Check if "default" or "uart" MPLAB project build profile is used
	@echo EZBL: Attempting to send to bootloader via UART
	${MP_JAVA_PATH}java -jar "${thisMakefileDir}ezbl_tools.jar" --communicator -com=COM13 -baud=115200 -timeout=1100 -artifact="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.bl2" 1>&2
else ifneq (,${filter i2c,${CONF}})	  # Check if "i2c" MPLAB project build profile is used. If so, upload via MCP2221 I2C.
	@echo EZBL: Attempting to send to bootloader via I2C
	${MP_JAVA_PATH}java -jar "${thisMakefileDir}ezbl_tools.jar" --communicator -com=I2C -i2c_address=0x60 -baud=400000 -timeout=1100 -artifact="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.bl2" 1>&2
else ifneq (,${filter usb_msd,${CONF}})	# Check if "usb_msd" MPLAB project build profile is used. Make a nice FIRMWARE.BL2 file to copy to the drive.
	@test -e "${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.bl2" && cp "${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.bl2" "${DISTDIR}/FIRMWARE.BL2" || true
endif



# Dynamically find the path to this ezbl_boot.mk file so we can pass it to the
# ezbl_tools.jar --make_editor -updateid="" option. This handles different
# directory structures better across projects and user preferences.
thisMakefilePath := ${word ${words ${MAKEFILE_LIST}},${MAKEFILE_LIST}}
thisMakefileDir  := ${dir ${thisMakefilePath}}


# Pre-build code to compute the BOOTID hash and do other build-time 
# processing need to create an EZBL bootloader. Note: build
# number is incremented each time regardless of if the final .hex/.bl2 files
# are rebuilt to contain and use them, so there will potentially be gaps in
# the build number sequence.
ifeq (${MAKE_RESTARTS},)
ifeq (${MAKECMDGOALS},)
${thisMakefilePath}:
	@${MP_JAVA_PATH}java -jar "${thisMakefileDir}ezbl_tools.jar" --make_editor -updateid="${thisMakefilePath}" || echo EZBL: BOOTID_HASH/APPID_VER in ${thisMakefilePath} updated

ezbl_start_build:
endif
endif


# Pre-clean code (helps MPLAB X IDE continue if clean fails normally on Windows due to file system file locks). Must have "Use "clean" target from the makefile." checked in the MPLAB X IDE Embedded Project settings to invoke this.
ezbl_clean:
	(test -e "dist/${CONF}/${IMAGE_TYPE}" && rm -f "dist/${CONF}/${IMAGE_TYPE}/*") && (test "dist/${CONF}" 1>NUL && rm -rf "dist/${CONF}") | (test "build/${CONF}/${IMAGE_TYPE}" && rm -rfd "build/${CONF}/${IMAGE_TYPE}/*" 1>NUL && rm -rf "build/${CONF}" 1>NUL) | (test "debug/${CONF}" && rm -rf "debug/${CONF}/${IMAGE_TYPE}/*" 1>NUL && rm -rfd "debug/${CONF}" 1>NUL) | (test "disassembly" && rm -rfd "disassembly" 1>NUL) || true



.PHONY: ezbl_clean ezbl_start_build  ${thisMakefilePath}
.build-impl: ezbl_start_build
.build-post: ezbl_post_build
.clean-pre: ezbl_clean



# Auto-generated SHA-256 hash of vendor, model, name and other BOOTID strings
# at the top of this file, including the quotations marks. This must be kept
# here so the computed values may be passed on the command line to the linker
# to be tested by the code when the App uploads a .bl2 file. Also, this data
# is passed along in the .merge.S file so the Application is built to target 
# this bootloader.
BOOTID_HASH  = F8917E0964A3C209C7149811527FF3ACF4363EBCBDFF5CDCED656830855E312A
BOOTID_HASH3 = 0xF4363EBC
BOOTID_HASH2 = 0xBDFF5CDC
BOOTID_HASH1 = 0xED656830
BOOTID_HASH0 = 0x855E312A



# Required addition to pass the BOOTID hash and APPID numbers to the linker for 
# .bl2 metadata inclusion and run-time Bootloader upload image evaluation.
MP_EXTRA_LD_POST += ,--defsym=_BOOTID_HASH0=${BOOTID_HASH0},--defsym=_BOOTID_HASH1=${BOOTID_HASH1},--defsym=_BOOTID_HASH2=${BOOTID_HASH2},--defsym=_BOOTID_HASH3=${BOOTID_HASH3},--defsym=_APPID_VER_MAJOR=${APPID_VER_MAJOR},--defsym=_APPID_VER_MINOR=${APPID_VER_MINOR},--defsym=_APPID_VER_BUILD=${APPID_VER_BUILD}
unexport MP_EXTRA_LD_POST


# Suppress make[x] Entering directory messages
MAKEFLAGS += --no-print-directory


# Miscellaneous includes so we can use the ${CONF}, ${MP_JAVA_PATH},
# ${MP_PROCESSOR_OPTION}, ${DISTDIR}, etc. MPLAB generated make variables in
# this make script.
ifeq (,$(findstring nbproject/Makefile-impl.mk,$(MAKEFILE_LIST)))
include nbproject/Makefile-impl.mk
endif
ifeq (,$(findstring nbproject/Makefile-variables.mk,$(MAKEFILE_LIST)))
include nbproject/Makefile-variables.mk
endif
ifeq (,$(findstring nbproject/Makefile-${CONF}.mk,$(MAKEFILE_LIST)))
include nbproject/Makefile-local-${CONF}.mk
endif
ifeq (,$(findstring nbproject/Makefile-${CONF}.mk,$(MAKEFILE_LIST)))
IGNORE_LOCAL:=TRUE
include nbproject/Makefile-${CONF}.mk
IGNORE_LOCAL:=FALSE
endif
export
unexport IGNORE_LOCAL

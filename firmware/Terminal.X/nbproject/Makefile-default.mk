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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=keyboard.c main.c "USB/Microchip/USB/CDC Device Driver/usb_function_cdc.c" USB/Microchip/USB/usb_device.c USB/usb_descriptors.c screen.c font.c terminal_config.c terminal_config_ui.c terminal.c terminal_keyboard.c terminal_screen.c terminal_uart.c ps2.c vga.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/keyboard.o ${OBJECTDIR}/main.o "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o" ${OBJECTDIR}/USB/Microchip/USB/usb_device.o ${OBJECTDIR}/USB/usb_descriptors.o ${OBJECTDIR}/screen.o ${OBJECTDIR}/font.o ${OBJECTDIR}/terminal_config.o ${OBJECTDIR}/terminal_config_ui.o ${OBJECTDIR}/terminal.o ${OBJECTDIR}/terminal_keyboard.o ${OBJECTDIR}/terminal_screen.o ${OBJECTDIR}/terminal_uart.o ${OBJECTDIR}/ps2.o ${OBJECTDIR}/vga.o
POSSIBLE_DEPFILES=${OBJECTDIR}/keyboard.o.d ${OBJECTDIR}/main.o.d "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o.d" ${OBJECTDIR}/USB/Microchip/USB/usb_device.o.d ${OBJECTDIR}/USB/usb_descriptors.o.d ${OBJECTDIR}/screen.o.d ${OBJECTDIR}/font.o.d ${OBJECTDIR}/terminal_config.o.d ${OBJECTDIR}/terminal_config_ui.o.d ${OBJECTDIR}/terminal.o.d ${OBJECTDIR}/terminal_keyboard.o.d ${OBJECTDIR}/terminal_screen.o.d ${OBJECTDIR}/terminal_uart.o.d ${OBJECTDIR}/ps2.o.d ${OBJECTDIR}/vga.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/keyboard.o ${OBJECTDIR}/main.o ${OBJECTDIR}/USB/Microchip/USB/CDC\ Device\ Driver/usb_function_cdc.o ${OBJECTDIR}/USB/Microchip/USB/usb_device.o ${OBJECTDIR}/USB/usb_descriptors.o ${OBJECTDIR}/screen.o ${OBJECTDIR}/font.o ${OBJECTDIR}/terminal_config.o ${OBJECTDIR}/terminal_config_ui.o ${OBJECTDIR}/terminal.o ${OBJECTDIR}/terminal_keyboard.o ${OBJECTDIR}/terminal_screen.o ${OBJECTDIR}/terminal_uart.o ${OBJECTDIR}/ps2.o ${OBJECTDIR}/vga.o

# Source Files
SOURCEFILES=keyboard.c main.c USB/Microchip/USB/CDC Device Driver/usb_function_cdc.c USB/Microchip/USB/usb_device.c USB/usb_descriptors.c screen.c font.c terminal_config.c terminal_config_ui.c terminal.c terminal_keyboard.c terminal_screen.c terminal_uart.c ps2.c vga.c



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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX270F256B
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/keyboard.o: keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/keyboard.o.d 
	@${RM} ${OBJECTDIR}/keyboard.o 
	@${FIXDEPS} "${OBJECTDIR}/keyboard.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/keyboard.o.d" -o ${OBJECTDIR}/keyboard.o keyboard.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/USB/Microchip/USB/CDC\ Device\ Driver/usb_function_cdc.o: USB/Microchip/USB/CDC\ Device\ Driver/usb_function_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver" 
	@${RM} "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o".d 
	@${RM} "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o" 
	@${FIXDEPS} "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o.d" -o "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o" "USB/Microchip/USB/CDC Device Driver/usb_function_cdc.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/USB/Microchip/USB/usb_device.o: USB/Microchip/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB/Microchip/USB" 
	@${RM} ${OBJECTDIR}/USB/Microchip/USB/usb_device.o.d 
	@${RM} ${OBJECTDIR}/USB/Microchip/USB/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/USB/Microchip/USB/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/USB/Microchip/USB/usb_device.o.d" -o ${OBJECTDIR}/USB/Microchip/USB/usb_device.o USB/Microchip/USB/usb_device.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/USB/usb_descriptors.o: USB/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB" 
	@${RM} ${OBJECTDIR}/USB/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/USB/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/USB/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/USB/usb_descriptors.o.d" -o ${OBJECTDIR}/USB/usb_descriptors.o USB/usb_descriptors.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/screen.o: screen.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/screen.o.d 
	@${RM} ${OBJECTDIR}/screen.o 
	@${FIXDEPS} "${OBJECTDIR}/screen.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/screen.o.d" -o ${OBJECTDIR}/screen.o screen.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/font.o: font.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${RM} ${OBJECTDIR}/font.o 
	@${FIXDEPS} "${OBJECTDIR}/font.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/font.o.d" -o ${OBJECTDIR}/font.o font.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/terminal_config.o: terminal_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_config.o.d 
	@${RM} ${OBJECTDIR}/terminal_config.o 
	@${FIXDEPS} "${OBJECTDIR}/terminal_config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/terminal_config.o.d" -o ${OBJECTDIR}/terminal_config.o terminal_config.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/terminal_config_ui.o: terminal_config_ui.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_config_ui.o.d 
	@${RM} ${OBJECTDIR}/terminal_config_ui.o 
	@${FIXDEPS} "${OBJECTDIR}/terminal_config_ui.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/terminal_config_ui.o.d" -o ${OBJECTDIR}/terminal_config_ui.o terminal_config_ui.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/terminal.o: terminal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal.o.d 
	@${RM} ${OBJECTDIR}/terminal.o 
	@${FIXDEPS} "${OBJECTDIR}/terminal.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/terminal.o.d" -o ${OBJECTDIR}/terminal.o terminal.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/terminal_keyboard.o: terminal_keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_keyboard.o.d 
	@${RM} ${OBJECTDIR}/terminal_keyboard.o 
	@${FIXDEPS} "${OBJECTDIR}/terminal_keyboard.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/terminal_keyboard.o.d" -o ${OBJECTDIR}/terminal_keyboard.o terminal_keyboard.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/terminal_screen.o: terminal_screen.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_screen.o.d 
	@${RM} ${OBJECTDIR}/terminal_screen.o 
	@${FIXDEPS} "${OBJECTDIR}/terminal_screen.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/terminal_screen.o.d" -o ${OBJECTDIR}/terminal_screen.o terminal_screen.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/terminal_uart.o: terminal_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_uart.o.d 
	@${RM} ${OBJECTDIR}/terminal_uart.o 
	@${FIXDEPS} "${OBJECTDIR}/terminal_uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/terminal_uart.o.d" -o ${OBJECTDIR}/terminal_uart.o terminal_uart.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/ps2.o: ps2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ps2.o.d 
	@${RM} ${OBJECTDIR}/ps2.o 
	@${FIXDEPS} "${OBJECTDIR}/ps2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ps2.o.d" -o ${OBJECTDIR}/ps2.o ps2.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/vga.o: vga.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/vga.o.d 
	@${RM} ${OBJECTDIR}/vga.o 
	@${FIXDEPS} "${OBJECTDIR}/vga.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/vga.o.d" -o ${OBJECTDIR}/vga.o vga.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
else
${OBJECTDIR}/keyboard.o: keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/keyboard.o.d 
	@${RM} ${OBJECTDIR}/keyboard.o 
	@${FIXDEPS} "${OBJECTDIR}/keyboard.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/keyboard.o.d" -o ${OBJECTDIR}/keyboard.o keyboard.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/USB/Microchip/USB/CDC\ Device\ Driver/usb_function_cdc.o: USB/Microchip/USB/CDC\ Device\ Driver/usb_function_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver" 
	@${RM} "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o".d 
	@${RM} "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o" 
	@${FIXDEPS} "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o.d" -o "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o" "USB/Microchip/USB/CDC Device Driver/usb_function_cdc.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/USB/Microchip/USB/usb_device.o: USB/Microchip/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB/Microchip/USB" 
	@${RM} ${OBJECTDIR}/USB/Microchip/USB/usb_device.o.d 
	@${RM} ${OBJECTDIR}/USB/Microchip/USB/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/USB/Microchip/USB/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/USB/Microchip/USB/usb_device.o.d" -o ${OBJECTDIR}/USB/Microchip/USB/usb_device.o USB/Microchip/USB/usb_device.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/USB/usb_descriptors.o: USB/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB" 
	@${RM} ${OBJECTDIR}/USB/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/USB/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/USB/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/USB/usb_descriptors.o.d" -o ${OBJECTDIR}/USB/usb_descriptors.o USB/usb_descriptors.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/screen.o: screen.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/screen.o.d 
	@${RM} ${OBJECTDIR}/screen.o 
	@${FIXDEPS} "${OBJECTDIR}/screen.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/screen.o.d" -o ${OBJECTDIR}/screen.o screen.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/font.o: font.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${RM} ${OBJECTDIR}/font.o 
	@${FIXDEPS} "${OBJECTDIR}/font.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/font.o.d" -o ${OBJECTDIR}/font.o font.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/terminal_config.o: terminal_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_config.o.d 
	@${RM} ${OBJECTDIR}/terminal_config.o 
	@${FIXDEPS} "${OBJECTDIR}/terminal_config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/terminal_config.o.d" -o ${OBJECTDIR}/terminal_config.o terminal_config.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/terminal_config_ui.o: terminal_config_ui.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_config_ui.o.d 
	@${RM} ${OBJECTDIR}/terminal_config_ui.o 
	@${FIXDEPS} "${OBJECTDIR}/terminal_config_ui.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/terminal_config_ui.o.d" -o ${OBJECTDIR}/terminal_config_ui.o terminal_config_ui.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/terminal.o: terminal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal.o.d 
	@${RM} ${OBJECTDIR}/terminal.o 
	@${FIXDEPS} "${OBJECTDIR}/terminal.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/terminal.o.d" -o ${OBJECTDIR}/terminal.o terminal.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/terminal_keyboard.o: terminal_keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_keyboard.o.d 
	@${RM} ${OBJECTDIR}/terminal_keyboard.o 
	@${FIXDEPS} "${OBJECTDIR}/terminal_keyboard.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/terminal_keyboard.o.d" -o ${OBJECTDIR}/terminal_keyboard.o terminal_keyboard.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/terminal_screen.o: terminal_screen.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_screen.o.d 
	@${RM} ${OBJECTDIR}/terminal_screen.o 
	@${FIXDEPS} "${OBJECTDIR}/terminal_screen.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/terminal_screen.o.d" -o ${OBJECTDIR}/terminal_screen.o terminal_screen.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/terminal_uart.o: terminal_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_uart.o.d 
	@${RM} ${OBJECTDIR}/terminal_uart.o 
	@${FIXDEPS} "${OBJECTDIR}/terminal_uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/terminal_uart.o.d" -o ${OBJECTDIR}/terminal_uart.o terminal_uart.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/ps2.o: ps2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ps2.o.d 
	@${RM} ${OBJECTDIR}/ps2.o 
	@${FIXDEPS} "${OBJECTDIR}/ps2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ps2.o.d" -o ${OBJECTDIR}/ps2.o ps2.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
${OBJECTDIR}/vga.o: vga.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/vga.o.d 
	@${RM} ${OBJECTDIR}/vga.o 
	@${FIXDEPS} "${OBJECTDIR}/vga.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/vga.o.d" -o ${OBJECTDIR}/vga.o vga.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml,-allow-multiple-definition
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml,-allow-multiple-definition
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif

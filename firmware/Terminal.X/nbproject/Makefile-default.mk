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
SOURCEFILES_QUOTED_IF_SPACED=keyboard.c main.c screen.c font.c terminal_config.c terminal_config_ui.c terminal.c terminal_keyboard.c terminal_screen.c terminal_uart.c ps2.c vga.c luminance.c "USB/Microchip/USB/CDC Device Driver/usb_function_cdc.c" USB/Microchip/USB/usb_device.c USB/usb_descriptors.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/keyboard.o ${OBJECTDIR}/main.o ${OBJECTDIR}/screen.o ${OBJECTDIR}/font.o ${OBJECTDIR}/terminal_config.o ${OBJECTDIR}/terminal_config_ui.o ${OBJECTDIR}/terminal.o ${OBJECTDIR}/terminal_keyboard.o ${OBJECTDIR}/terminal_screen.o ${OBJECTDIR}/terminal_uart.o ${OBJECTDIR}/ps2.o ${OBJECTDIR}/vga.o ${OBJECTDIR}/luminance.o "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o" ${OBJECTDIR}/USB/Microchip/USB/usb_device.o ${OBJECTDIR}/USB/usb_descriptors.o
POSSIBLE_DEPFILES=${OBJECTDIR}/keyboard.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/screen.o.d ${OBJECTDIR}/font.o.d ${OBJECTDIR}/terminal_config.o.d ${OBJECTDIR}/terminal_config_ui.o.d ${OBJECTDIR}/terminal.o.d ${OBJECTDIR}/terminal_keyboard.o.d ${OBJECTDIR}/terminal_screen.o.d ${OBJECTDIR}/terminal_uart.o.d ${OBJECTDIR}/ps2.o.d ${OBJECTDIR}/vga.o.d ${OBJECTDIR}/luminance.o.d "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o.d" ${OBJECTDIR}/USB/Microchip/USB/usb_device.o.d ${OBJECTDIR}/USB/usb_descriptors.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/keyboard.o ${OBJECTDIR}/main.o ${OBJECTDIR}/screen.o ${OBJECTDIR}/font.o ${OBJECTDIR}/terminal_config.o ${OBJECTDIR}/terminal_config_ui.o ${OBJECTDIR}/terminal.o ${OBJECTDIR}/terminal_keyboard.o ${OBJECTDIR}/terminal_screen.o ${OBJECTDIR}/terminal_uart.o ${OBJECTDIR}/ps2.o ${OBJECTDIR}/vga.o ${OBJECTDIR}/luminance.o ${OBJECTDIR}/USB/Microchip/USB/CDC\ Device\ Driver/usb_function_cdc.o ${OBJECTDIR}/USB/Microchip/USB/usb_device.o ${OBJECTDIR}/USB/usb_descriptors.o

# Source Files
SOURCEFILES=keyboard.c main.c screen.c font.c terminal_config.c terminal_config_ui.c terminal.c terminal_keyboard.c terminal_screen.c terminal_uart.c ps2.c vga.c luminance.c USB/Microchip/USB/CDC Device Driver/usb_function_cdc.c USB/Microchip/USB/usb_device.c USB/usb_descriptors.c



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
${OBJECTDIR}/keyboard.o: keyboard.c  .generated_files/a3475f0d67ae7ecf2e4174f60cf1ce45950053e2.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/keyboard.o.d 
	@${RM} ${OBJECTDIR}/keyboard.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/keyboard.o.d" -o ${OBJECTDIR}/keyboard.o keyboard.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/main.o: main.c  .generated_files/56f1c909d44dbdf3478fee1b01714dbe2a0dbab9.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/screen.o: screen.c  .generated_files/af9ac83f4baf00bea0d6ee504c138da84cd322f2.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/screen.o.d 
	@${RM} ${OBJECTDIR}/screen.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/screen.o.d" -o ${OBJECTDIR}/screen.o screen.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/font.o: font.c  .generated_files/84fab7004efec5b4416cd8761040ed0dea083c94.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${RM} ${OBJECTDIR}/font.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/font.o.d" -o ${OBJECTDIR}/font.o font.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/terminal_config.o: terminal_config.c  .generated_files/a90361766f467c5b82ad2dd5ddabf97526fb0385.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_config.o.d 
	@${RM} ${OBJECTDIR}/terminal_config.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/terminal_config.o.d" -o ${OBJECTDIR}/terminal_config.o terminal_config.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/terminal_config_ui.o: terminal_config_ui.c  .generated_files/c150e2a2fb6598c9b0ebe2eb87bc17e8288e7302.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_config_ui.o.d 
	@${RM} ${OBJECTDIR}/terminal_config_ui.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/terminal_config_ui.o.d" -o ${OBJECTDIR}/terminal_config_ui.o terminal_config_ui.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/terminal.o: terminal.c  .generated_files/6482586ed637a5b571e184f9eb2145e81825e8ce.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal.o.d 
	@${RM} ${OBJECTDIR}/terminal.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/terminal.o.d" -o ${OBJECTDIR}/terminal.o terminal.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/terminal_keyboard.o: terminal_keyboard.c  .generated_files/924db10d029d66f2b30f28abeca5cbfc4d273352.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_keyboard.o.d 
	@${RM} ${OBJECTDIR}/terminal_keyboard.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/terminal_keyboard.o.d" -o ${OBJECTDIR}/terminal_keyboard.o terminal_keyboard.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/terminal_screen.o: terminal_screen.c  .generated_files/5bbb6c7ba7b42662cb5aaba1c98a7dba237f5008.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_screen.o.d 
	@${RM} ${OBJECTDIR}/terminal_screen.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/terminal_screen.o.d" -o ${OBJECTDIR}/terminal_screen.o terminal_screen.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/terminal_uart.o: terminal_uart.c  .generated_files/ad7fe0665d885ae65bfac6ef5e450f68fd7e1d7c.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_uart.o.d 
	@${RM} ${OBJECTDIR}/terminal_uart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/terminal_uart.o.d" -o ${OBJECTDIR}/terminal_uart.o terminal_uart.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ps2.o: ps2.c  .generated_files/fa98c5dfaa97021ef6279369185576c0b01d9238.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ps2.o.d 
	@${RM} ${OBJECTDIR}/ps2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ps2.o.d" -o ${OBJECTDIR}/ps2.o ps2.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/vga.o: vga.c  .generated_files/40f0acabb755078fce66497d5e59e6f8548ff306.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/vga.o.d 
	@${RM} ${OBJECTDIR}/vga.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/vga.o.d" -o ${OBJECTDIR}/vga.o vga.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/luminance.o: luminance.c  .generated_files/dfef54160ae3814bfc18252411ee2cd74c41b859.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/luminance.o.d 
	@${RM} ${OBJECTDIR}/luminance.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/luminance.o.d" -o ${OBJECTDIR}/luminance.o luminance.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/USB/Microchip/USB/CDC\ Device\ Driver/usb_function_cdc.o: USB/Microchip/USB/CDC\ Device\ Driver/usb_function_cdc.c  .generated_files/1b6596ca12406c59e2b09fd2cba9b0e774dcdfc6.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver" 
	@${RM} "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o".d 
	@${RM} "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o" 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o.d" -o "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o" "USB/Microchip/USB/CDC Device Driver/usb_function_cdc.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/USB/Microchip/USB/usb_device.o: USB/Microchip/USB/usb_device.c  .generated_files/7648959f0429876dcecfe2f4cc2a0eaa92769efd.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/USB/Microchip/USB" 
	@${RM} ${OBJECTDIR}/USB/Microchip/USB/usb_device.o.d 
	@${RM} ${OBJECTDIR}/USB/Microchip/USB/usb_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USB/Microchip/USB/usb_device.o.d" -o ${OBJECTDIR}/USB/Microchip/USB/usb_device.o USB/Microchip/USB/usb_device.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/USB/usb_descriptors.o: USB/usb_descriptors.c  .generated_files/afb93d5b8695a3aed456762307311aebcdf16ef6.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/USB" 
	@${RM} ${OBJECTDIR}/USB/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/USB/usb_descriptors.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USB/usb_descriptors.o.d" -o ${OBJECTDIR}/USB/usb_descriptors.o USB/usb_descriptors.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/keyboard.o: keyboard.c  .generated_files/b46570fc229bdb29f6a402371437fbe00efbb431.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/keyboard.o.d 
	@${RM} ${OBJECTDIR}/keyboard.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/keyboard.o.d" -o ${OBJECTDIR}/keyboard.o keyboard.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/main.o: main.c  .generated_files/2e08eb3179ad20d6b0daf9c72a7701b4250c23dd.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/screen.o: screen.c  .generated_files/16bdb658ec31ff599fdd3f8299473e4e95c86b31.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/screen.o.d 
	@${RM} ${OBJECTDIR}/screen.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/screen.o.d" -o ${OBJECTDIR}/screen.o screen.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/font.o: font.c  .generated_files/3a4ae8e88f5f34dae744f34bb033795ffc16b26b.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${RM} ${OBJECTDIR}/font.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/font.o.d" -o ${OBJECTDIR}/font.o font.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/terminal_config.o: terminal_config.c  .generated_files/863b50d9bfc56345adcbb27a138dec40e586ca94.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_config.o.d 
	@${RM} ${OBJECTDIR}/terminal_config.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/terminal_config.o.d" -o ${OBJECTDIR}/terminal_config.o terminal_config.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/terminal_config_ui.o: terminal_config_ui.c  .generated_files/c5bd57a5dba9d16f84514b31e97a4724bc28b766.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_config_ui.o.d 
	@${RM} ${OBJECTDIR}/terminal_config_ui.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/terminal_config_ui.o.d" -o ${OBJECTDIR}/terminal_config_ui.o terminal_config_ui.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/terminal.o: terminal.c  .generated_files/d71471f9cf2ad445df97e8daa6b616aaa977c7e4.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal.o.d 
	@${RM} ${OBJECTDIR}/terminal.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/terminal.o.d" -o ${OBJECTDIR}/terminal.o terminal.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/terminal_keyboard.o: terminal_keyboard.c  .generated_files/64a313c1bf2ce7a3b14746a30abb5aa09755c242.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_keyboard.o.d 
	@${RM} ${OBJECTDIR}/terminal_keyboard.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/terminal_keyboard.o.d" -o ${OBJECTDIR}/terminal_keyboard.o terminal_keyboard.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/terminal_screen.o: terminal_screen.c  .generated_files/798b8473cd9846d5b2879e345e36e65420ab73e2.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_screen.o.d 
	@${RM} ${OBJECTDIR}/terminal_screen.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/terminal_screen.o.d" -o ${OBJECTDIR}/terminal_screen.o terminal_screen.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/terminal_uart.o: terminal_uart.c  .generated_files/8ce43f01974c15b5ddcfd3cc06db4d44b8a80488.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_uart.o.d 
	@${RM} ${OBJECTDIR}/terminal_uart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/terminal_uart.o.d" -o ${OBJECTDIR}/terminal_uart.o terminal_uart.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ps2.o: ps2.c  .generated_files/38a018eb5ba71f575e94ba68ae3e5a250472dadb.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ps2.o.d 
	@${RM} ${OBJECTDIR}/ps2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ps2.o.d" -o ${OBJECTDIR}/ps2.o ps2.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/vga.o: vga.c  .generated_files/b9e09c25346610b0270b0b33a68a59fe7ef02863.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/vga.o.d 
	@${RM} ${OBJECTDIR}/vga.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/vga.o.d" -o ${OBJECTDIR}/vga.o vga.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/luminance.o: luminance.c  .generated_files/4105e1a18bc257f46684c8b52c987bde4254c532.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/luminance.o.d 
	@${RM} ${OBJECTDIR}/luminance.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/luminance.o.d" -o ${OBJECTDIR}/luminance.o luminance.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/USB/Microchip/USB/CDC\ Device\ Driver/usb_function_cdc.o: USB/Microchip/USB/CDC\ Device\ Driver/usb_function_cdc.c  .generated_files/c04851ad0f81c41fbd45b0bfba6f77870a3bc6c.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver" 
	@${RM} "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o".d 
	@${RM} "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o" 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o.d" -o "${OBJECTDIR}/USB/Microchip/USB/CDC Device Driver/usb_function_cdc.o" "USB/Microchip/USB/CDC Device Driver/usb_function_cdc.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/USB/Microchip/USB/usb_device.o: USB/Microchip/USB/usb_device.c  .generated_files/979dcd567fc380b8373d50c586bc4463099775fc.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/USB/Microchip/USB" 
	@${RM} ${OBJECTDIR}/USB/Microchip/USB/usb_device.o.d 
	@${RM} ${OBJECTDIR}/USB/Microchip/USB/usb_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USB/Microchip/USB/usb_device.o.d" -o ${OBJECTDIR}/USB/Microchip/USB/usb_device.o USB/Microchip/USB/usb_device.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/USB/usb_descriptors.o: USB/usb_descriptors.c  .generated_files/147688d54ec45403afdb50c481d6e007e6f71e43.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/USB" 
	@${RM} ${OBJECTDIR}/USB/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/USB/usb_descriptors.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USB/usb_descriptors.o.d" -o ${OBJECTDIR}/USB/usb_descriptors.o USB/usb_descriptors.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -DTERMINAL_SERIAL_INVERTED -mdfp="${DFP_DIR}"  
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=0,--defsym=_min_stack_size=0,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml,-allow-multiple-definition -mdfp="${DFP_DIR}"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=0,--defsym=_min_stack_size=0,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml,-allow-multiple-definition -mdfp="${DFP_DIR}"
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

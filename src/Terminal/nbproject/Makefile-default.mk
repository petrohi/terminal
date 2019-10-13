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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED="../USB/Microchip/USB/CDC Device Driver/usb_function_cdc.c" ../USB/Microchip/USB/usb_device.c ../USB/usb_descriptors.c ../Main.c "T:/Programming - PIC/41 -ASCII Terminal/Source/Terminal/Keyboard.c" "T:/Programming - PIC/41 -ASCII Terminal/Source/Terminal/Video.c" "T:/Programming - PIC/41 -ASCII Terminal/Source/Terminal/vt100.c"

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/624290896/usb_function_cdc.o ${OBJECTDIR}/_ext/647448523/usb_device.o ${OBJECTDIR}/_ext/1360907413/usb_descriptors.o ${OBJECTDIR}/_ext/1472/Main.o ${OBJECTDIR}/_ext/2004981968/Keyboard.o ${OBJECTDIR}/_ext/2004981968/Video.o ${OBJECTDIR}/_ext/2004981968/vt100.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/624290896/usb_function_cdc.o.d ${OBJECTDIR}/_ext/647448523/usb_device.o.d ${OBJECTDIR}/_ext/1360907413/usb_descriptors.o.d ${OBJECTDIR}/_ext/1472/Main.o.d ${OBJECTDIR}/_ext/2004981968/Keyboard.o.d ${OBJECTDIR}/_ext/2004981968/Video.o.d ${OBJECTDIR}/_ext/2004981968/vt100.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/624290896/usb_function_cdc.o ${OBJECTDIR}/_ext/647448523/usb_device.o ${OBJECTDIR}/_ext/1360907413/usb_descriptors.o ${OBJECTDIR}/_ext/1472/Main.o ${OBJECTDIR}/_ext/2004981968/Keyboard.o ${OBJECTDIR}/_ext/2004981968/Video.o ${OBJECTDIR}/_ext/2004981968/vt100.o

# Source Files
SOURCEFILES=../USB/Microchip/USB/CDC Device Driver/usb_function_cdc.c ../USB/Microchip/USB/usb_device.c ../USB/usb_descriptors.c ../Main.c T:/Programming - PIC/41 -ASCII Terminal/Source/Terminal/Keyboard.c T:/Programming - PIC/41 -ASCII Terminal/Source/Terminal/Video.c T:/Programming - PIC/41 -ASCII Terminal/Source/Terminal/vt100.c


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
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX250F128B
MP_LINKER_FILE_OPTION=,--script="T:\Programming - PIC\41 -ASCII Terminal\Source\Terminal\Linker Script\32MX250F128B.ld"
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
${OBJECTDIR}/_ext/624290896/usb_function_cdc.o: ../USB/Microchip/USB/CDC\ Device\ Driver/usb_function_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/624290896 
	@${RM} ${OBJECTDIR}/_ext/624290896/usb_function_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/624290896/usb_function_cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/624290896/usb_function_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/624290896/usb_function_cdc.o.d" -o ${OBJECTDIR}/_ext/624290896/usb_function_cdc.o "../USB/Microchip/USB/CDC Device Driver/usb_function_cdc.c"   
	
${OBJECTDIR}/_ext/647448523/usb_device.o: ../USB/Microchip/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/647448523 
	@${RM} ${OBJECTDIR}/_ext/647448523/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/647448523/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/647448523/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/647448523/usb_device.o.d" -o ${OBJECTDIR}/_ext/647448523/usb_device.o ../USB/Microchip/USB/usb_device.c   
	
${OBJECTDIR}/_ext/1360907413/usb_descriptors.o: ../USB/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360907413 
	@${RM} ${OBJECTDIR}/_ext/1360907413/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360907413/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360907413/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360907413/usb_descriptors.o.d" -o ${OBJECTDIR}/_ext/1360907413/usb_descriptors.o ../USB/usb_descriptors.c   
	
${OBJECTDIR}/_ext/1472/Main.o: ../Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Main.o.d" -o ${OBJECTDIR}/_ext/1472/Main.o ../Main.c   
	
${OBJECTDIR}/_ext/2004981968/Keyboard.o: T:/Programming\ -\ PIC/41\ -ASCII\ Terminal/Source/Terminal/Keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2004981968 
	@${RM} ${OBJECTDIR}/_ext/2004981968/Keyboard.o.d 
	@${RM} ${OBJECTDIR}/_ext/2004981968/Keyboard.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2004981968/Keyboard.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2004981968/Keyboard.o.d" -o ${OBJECTDIR}/_ext/2004981968/Keyboard.o "T:/Programming - PIC/41 -ASCII Terminal/Source/Terminal/Keyboard.c"   
	
${OBJECTDIR}/_ext/2004981968/Video.o: T:/Programming\ -\ PIC/41\ -ASCII\ Terminal/Source/Terminal/Video.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2004981968 
	@${RM} ${OBJECTDIR}/_ext/2004981968/Video.o.d 
	@${RM} ${OBJECTDIR}/_ext/2004981968/Video.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2004981968/Video.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2004981968/Video.o.d" -o ${OBJECTDIR}/_ext/2004981968/Video.o "T:/Programming - PIC/41 -ASCII Terminal/Source/Terminal/Video.c"   
	
${OBJECTDIR}/_ext/2004981968/vt100.o: T:/Programming\ -\ PIC/41\ -ASCII\ Terminal/Source/Terminal/vt100.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2004981968 
	@${RM} ${OBJECTDIR}/_ext/2004981968/vt100.o.d 
	@${RM} ${OBJECTDIR}/_ext/2004981968/vt100.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2004981968/vt100.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2004981968/vt100.o.d" -o ${OBJECTDIR}/_ext/2004981968/vt100.o "T:/Programming - PIC/41 -ASCII Terminal/Source/Terminal/vt100.c"   
	
else
${OBJECTDIR}/_ext/624290896/usb_function_cdc.o: ../USB/Microchip/USB/CDC\ Device\ Driver/usb_function_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/624290896 
	@${RM} ${OBJECTDIR}/_ext/624290896/usb_function_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/624290896/usb_function_cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/624290896/usb_function_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/624290896/usb_function_cdc.o.d" -o ${OBJECTDIR}/_ext/624290896/usb_function_cdc.o "../USB/Microchip/USB/CDC Device Driver/usb_function_cdc.c"   
	
${OBJECTDIR}/_ext/647448523/usb_device.o: ../USB/Microchip/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/647448523 
	@${RM} ${OBJECTDIR}/_ext/647448523/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/647448523/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/647448523/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/647448523/usb_device.o.d" -o ${OBJECTDIR}/_ext/647448523/usb_device.o ../USB/Microchip/USB/usb_device.c   
	
${OBJECTDIR}/_ext/1360907413/usb_descriptors.o: ../USB/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360907413 
	@${RM} ${OBJECTDIR}/_ext/1360907413/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360907413/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360907413/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360907413/usb_descriptors.o.d" -o ${OBJECTDIR}/_ext/1360907413/usb_descriptors.o ../USB/usb_descriptors.c   
	
${OBJECTDIR}/_ext/1472/Main.o: ../Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Main.o.d" -o ${OBJECTDIR}/_ext/1472/Main.o ../Main.c   
	
${OBJECTDIR}/_ext/2004981968/Keyboard.o: T:/Programming\ -\ PIC/41\ -ASCII\ Terminal/Source/Terminal/Keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2004981968 
	@${RM} ${OBJECTDIR}/_ext/2004981968/Keyboard.o.d 
	@${RM} ${OBJECTDIR}/_ext/2004981968/Keyboard.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2004981968/Keyboard.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2004981968/Keyboard.o.d" -o ${OBJECTDIR}/_ext/2004981968/Keyboard.o "T:/Programming - PIC/41 -ASCII Terminal/Source/Terminal/Keyboard.c"   
	
${OBJECTDIR}/_ext/2004981968/Video.o: T:/Programming\ -\ PIC/41\ -ASCII\ Terminal/Source/Terminal/Video.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2004981968 
	@${RM} ${OBJECTDIR}/_ext/2004981968/Video.o.d 
	@${RM} ${OBJECTDIR}/_ext/2004981968/Video.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2004981968/Video.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2004981968/Video.o.d" -o ${OBJECTDIR}/_ext/2004981968/Video.o "T:/Programming - PIC/41 -ASCII Terminal/Source/Terminal/Video.c"   
	
${OBJECTDIR}/_ext/2004981968/vt100.o: T:/Programming\ -\ PIC/41\ -ASCII\ Terminal/Source/Terminal/vt100.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2004981968 
	@${RM} ${OBJECTDIR}/_ext/2004981968/vt100.o.d 
	@${RM} ${OBJECTDIR}/_ext/2004981968/vt100.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2004981968/vt100.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2004981968/vt100.o.d" -o ${OBJECTDIR}/_ext/2004981968/vt100.o "T:/Programming - PIC/41 -ASCII Terminal/Source/Terminal/vt100.c"   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    T:/Programming\ -\ PIC/41\ -ASCII\ Terminal/Source/Terminal/Linker\ Script/32MX250F128B.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}           -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC00BEF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,--defsym=_min_heap_size=0,--defsym=_min_stack_size=1024,-G4
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   T:/Programming\ -\ PIC/41\ -ASCII\ Terminal/Source/Terminal/Linker\ Script/32MX250F128B.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=0,--defsym=_min_stack_size=1024,-G4
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/Terminal.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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

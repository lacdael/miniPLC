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
ifeq "$(wildcard nbproject/Makefile-local-uart.mk)" "nbproject/Makefile-local-uart.mk"
include nbproject/Makefile-local-uart.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=uart
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=hardware_initializers/pic32mm0256gpm064_explorer_16.c hardware_initializers/pic32mm0064gpl036_explorer_16.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o ${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o.d ${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o ${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=hardware_initializers/pic32mm0256gpm064_explorer_16.c hardware_initializers/pic32mm0064gpl036_explorer_16.c main.c



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
	${MAKE}  -f nbproject/Makefile-uart.mk dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MM0064GPL028
MP_LINKER_FILE_OPTION=,--script="ezbl_integration/ezbl_pic32mm.ld"
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
${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o: hardware_initializers/pic32mm0256gpm064_explorer_16.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hardware_initializers" 
	@${RM} ${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o.d 
	@${RM} ${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -Wall -MMD -MF "${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o.d" -o ${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o hardware_initializers/pic32mm0256gpm064_explorer_16.c    -DXPRJ_uart=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -G 8192 -mbranch-cost=1   
	
${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o: hardware_initializers/pic32mm0064gpl036_explorer_16.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hardware_initializers" 
	@${RM} ${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o.d 
	@${RM} ${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -Wall -MMD -MF "${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o.d" -o ${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o hardware_initializers/pic32mm0064gpl036_explorer_16.c    -DXPRJ_uart=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -G 8192 -mbranch-cost=1   
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -Wall -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_uart=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -G 8192 -mbranch-cost=1   
	
else
${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o: hardware_initializers/pic32mm0256gpm064_explorer_16.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hardware_initializers" 
	@${RM} ${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o.d 
	@${RM} ${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -Wall -MMD -MF "${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o.d" -o ${OBJECTDIR}/hardware_initializers/pic32mm0256gpm064_explorer_16.o hardware_initializers/pic32mm0256gpm064_explorer_16.c    -DXPRJ_uart=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -G 8192 -mbranch-cost=1   
	
${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o: hardware_initializers/pic32mm0064gpl036_explorer_16.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/hardware_initializers" 
	@${RM} ${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o.d 
	@${RM} ${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -Wall -MMD -MF "${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o.d" -o ${OBJECTDIR}/hardware_initializers/pic32mm0064gpl036_explorer_16.o hardware_initializers/pic32mm0064gpl036_explorer_16.c    -DXPRJ_uart=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -G 8192 -mbranch-cost=1   
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -Wall -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_uart=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -G 8192 -mbranch-cost=1   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ezbl_integration/ezbl_lib32mm.a  ezbl_integration/ezbl_pic32mm.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
###<EZBL MODIFIED 0>###
	@echo EZBL: Starting linking pass 1 of 2
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o "/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_partial_link.elf" ${OBJECTFILES_QUOTED_IF_SPACED}    ezbl_integration/ezbl_lib32mm.a      -DXPRJ_uart=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC016FF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,-DEZBL_PASS_1_LINK,--no-check-sections,--defsym=EZBL_BOOT_PROJECT=1,-DEZBL_BOOT_PROJECT=EZBL_BOOT_PROJECT,-D__$(MP_PROCESSOR_OPTION)__=1,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_stack_size=3072,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml  1>/dev/null
	$(MP_JAVA_PATH)java -cp "/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --dump_parser -pass=1 -elf_artifact="/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_partial_link.elf"  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="$(FINAL_IMAGE)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	@echo EZBL: Starting linking pass 2 of 2
###</EZBL MODIFIED 0>###
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.$(DEBUGGABLE_SUFFIX) ${OBJECTFILES_QUOTED_IF_SPACED}    ezbl_integration/ezbl_lib32mm.a      -DXPRJ_uart=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC016FF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=EZBL_BOOT_PROJECT=1,-DEZBL_BOOT_PROJECT=EZBL_BOOT_PROJECT,-D__$(MP_PROCESSOR_OPTION)__=1,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_stack_size=3072,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml 
###<EZBL MODIFIED 0>###
#	Run the ezbl_tools.jar dump processor again so we can test if the Bootloader geometry changed
	@echo $(MP_JAVA_PATH)java -cp "/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --dump_parser -pass=2 -elf_artifact="dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.$(DEBUGGABLE_SUFFIX)"  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="$(FINAL_IMAGE)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	@echo 	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.$(DEBUGGABLE_SUFFIX) ${OBJECTFILES_QUOTED_IF_SPACED}    ezbl_integration/ezbl_lib32mm.a      -DXPRJ_uart=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC016FF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=EZBL_BOOT_PROJECT=1,-DEZBL_BOOT_PROJECT=EZBL_BOOT_PROJECT,-D__$(MP_PROCESSOR_OPTION)__=1,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_stack_size=3072,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml  | $(MP_JAVA_PATH)java -cp "/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --dump_parser -pass=2 -elf_artifact="dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.$(DEBUGGABLE_SUFFIX)"  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="$(FINAL_IMAGE)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	$(MP_JAVA_PATH)java -cp "/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --blobber -generate_merge  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.$(DEBUGGABLE_SUFFIX)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	@test -e "/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_partial_link.elf" && rm "/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_partial_link.elf" || true
###</EZBL MODIFIED 0>###
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ezbl_integration/ezbl_lib32mm.a ezbl_integration/ezbl_pic32mm.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
###<EZBL MODIFIED 0>###
	@echo EZBL: Starting linking pass 1 of 2
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o "/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_partial_link.elf" ${OBJECTFILES_QUOTED_IF_SPACED}    ezbl_integration/ezbl_lib32mm.a      -DXPRJ_uart=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,-DEZBL_PASS_1_LINK,--no-check-sections,--defsym=EZBL_BOOT_PROJECT=1,-DEZBL_BOOT_PROJECT=EZBL_BOOT_PROJECT,-D__$(MP_PROCESSOR_OPTION)__=1,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_stack_size=3072,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml  1>/dev/null
	$(MP_JAVA_PATH)java -cp "/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --dump_parser -pass=1 -elf_artifact="/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_partial_link.elf"  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="$(FINAL_IMAGE)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	@echo EZBL: Starting linking pass 2 of 2
###</EZBL MODIFIED 0>###
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ezbl_integration/ezbl_lib32mm.a      -DXPRJ_uart=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=EZBL_BOOT_PROJECT=1,-DEZBL_BOOT_PROJECT=EZBL_BOOT_PROJECT,-D__$(MP_PROCESSOR_OPTION)__=1,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_stack_size=3072,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml 
###<EZBL MODIFIED 0>###
#	Run the ezbl_tools.jar dump processor again so we can test if the Bootloader geometry changed
	@echo $(MP_JAVA_PATH)java -cp "/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --dump_parser -pass=2 -elf_artifact="dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}"  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="$(FINAL_IMAGE)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	@echo 	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ezbl_integration/ezbl_lib32mm.a      -DXPRJ_uart=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=EZBL_BOOT_PROJECT=1,-DEZBL_BOOT_PROJECT=EZBL_BOOT_PROJECT,-D__$(MP_PROCESSOR_OPTION)__=1,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_stack_size=3072,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml  | $(MP_JAVA_PATH)java -cp "/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --dump_parser -pass=2 -elf_artifact="dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}"  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="$(FINAL_IMAGE)" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	$(MP_JAVA_PATH)java -cp "/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_tools.jar" com.microchip.apps.ezbl.Main --blobber -generate_merge  -conf=$(CONF) -mcpu=$(MP_PROCESSOR_OPTION) -artifact="dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" -linkscript=$(MP_LINKER_FILE_OPTION) -compiler_folder=$(MP_CC_DIR) -java=$(MP_JAVA_PATH) -project_name="$(PROJECTNAME)" -state="dist/$(CND_CONF)/$(IMAGE_TYPE)/ezbl_build_state.bin"
	@test -e "/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_partial_link.elf" && rm "/home/christian/Documents/WORK/WISDEL/PRT/W-BT/w-bt_boot.X/ezbl_integration/ezbl_partial_link.elf" || true
###</EZBL MODIFIED 0>###
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/w-bt_boot.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/uart
	${RM} -r dist/uart

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif

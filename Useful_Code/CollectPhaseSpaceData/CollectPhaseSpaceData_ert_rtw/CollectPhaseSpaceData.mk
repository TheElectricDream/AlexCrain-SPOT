###########################################################################
## Makefile generated for Simulink model 'CollectPhaseSpaceData'. 
## 
## Makefile     : CollectPhaseSpaceData.mk
## Generated on : Fri Nov 11 11:02:36 2022
## MATLAB Coder version: 5.0 (R2020a)
## 
## Build Info:
## 
## Final product: $(RELATIVE_PATH_TO_ANCHOR)/CollectPhaseSpaceData.elf
## Product type : executable
## Build type   : Top-Level Standalone Executable
## 
###########################################################################

###########################################################################
## MACROS
###########################################################################

# Macro Descriptions:
# PRODUCT_NAME            Name of the system to build
# MAKEFILE                Name of this makefile

PRODUCT_NAME              = CollectPhaseSpaceData
MAKEFILE                  = CollectPhaseSpaceData.mk
MATLAB_ROOT               = $(MATLAB_WORKSPACE)/C/Program_Files/MATLAB/R2020a
MATLAB_BIN                = $(MATLAB_WORKSPACE)/C/Program_Files/MATLAB/R2020a/bin
MATLAB_ARCH_BIN           = $(MATLAB_BIN)/win64
MASTER_ANCHOR_DIR         = 
START_DIR                 = $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Useful_Code/CollectPhaseSpaceData
SOLVER                    = 
SOLVER_OBJ                = 
CLASSIC_INTERFACE         = 0
TGT_FCN_LIB               = ISO_C++
MODEL_HAS_DYNAMICALLY_LOADED_SFCNS = 0
RELATIVE_PATH_TO_ANCHOR   = ..
C_STANDARD_OPTS           = 
CPP_STANDARD_OPTS         = 

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

# Toolchain Name:          GNU GCC Raspberry Pi
# Supported Version(s):    
# ToolchainInfo Version:   2020a
# Specification Revision:  1.0
# 

#-----------
# MACROS
#-----------

CCOUTPUTFLAG = --output_file=
LDOUTPUTFLAG = --output_file=

TOOLCHAIN_SRCS = 
TOOLCHAIN_INCS = 
TOOLCHAIN_LIBS = -lm -lm -lstdc++

#------------------------
# BUILD TOOL COMMANDS
#------------------------

# Assembler: GNU GCC Raspberry Pi Assembler
AS = as

# C Compiler: GNU GCC Raspberry Pi C Compiler
CC = gcc

# Linker: GNU GCC Raspberry Pi Linker
LD = gcc

# C++ Compiler: GNU GCC Raspberry Pi C++ Compiler
CPP = g++

# C++ Linker: GNU GCC Raspberry Pi C++ Linker
CPP_LD = g++

# Archiver: GNU GCC Raspberry Pi Archiver
AR = ar

# MEX Tool: MEX Tool
MEX_PATH = $(MATLAB_ARCH_BIN)
MEX = "$(MEX_PATH)/mex"

# Download: Download
DOWNLOAD =

# Execute: Execute
EXECUTE = $(PRODUCT)

# Builder: Make Tool
MAKE = make


#-------------------------
# Directives/Utilities
#-------------------------

ASDEBUG             = -g
AS_OUTPUT_FLAG      = -o
CDEBUG              = -g
C_OUTPUT_FLAG       = -o
LDDEBUG             = -g
OUTPUT_FLAG         = -o
CPPDEBUG            = -g
CPP_OUTPUT_FLAG     = -o
CPPLDDEBUG          = -g
OUTPUT_FLAG         = -o
ARDEBUG             =
STATICLIB_OUTPUT_FLAG =
MEX_DEBUG           = -g
RM                  =
ECHO                = echo
MV                  =
RUN                 =

#--------------------------------------
# "Faster Runs" Build Configuration
#--------------------------------------

ARFLAGS              = -r
ASFLAGS              = -c \
                       $(ASFLAGS_ADDITIONAL) \
                       $(INCLUDES)
CFLAGS               = -c \
                       -MMD -MP -MF"$(@:%.o=%.dep)" -MT"$@"  \
                       -O2
CPPFLAGS             = -c \
                       -MMD -MP -MF"$(@:%.o=%.dep)" -MT"$@"  \
                       -fpermissive  \
                       -O2
CPP_LDFLAGS          = -lrt -lpthread -ldl
CPP_SHAREDLIB_LDFLAGS  = -shared  \
                         -lrt -lpthread -ldl
DOWNLOAD_FLAGS       =
EXECUTE_FLAGS        =
LDFLAGS              = -lrt -lpthread -ldl
MEX_CPPFLAGS         =
MEX_CPPLDFLAGS       =
MEX_CFLAGS           =
MEX_LDFLAGS          =
MAKE_FLAGS           = -f $(MAKEFILE)
SHAREDLIB_LDFLAGS    = -shared  \
                       -lrt -lpthread -ldl



###########################################################################
## OUTPUT INFO
###########################################################################

PRODUCT = $(RELATIVE_PATH_TO_ANCHOR)/CollectPhaseSpaceData.elf
PRODUCT_TYPE = "executable"
BUILD_TYPE = "Top-Level Standalone Executable"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = -I$(START_DIR) -I$(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/include -I$(MATLAB_ROOT)/toolbox/shared/spc/src_ml/extern/include -I$(START_DIR)/CollectPhaseSpaceData_ert_rtw -I$(MATLAB_ROOT)/extern/include -I$(MATLAB_ROOT)/simulink/include -I$(MATLAB_ROOT)/rtw/c/src -I$(MATLAB_ROOT)/rtw/c/src/ext_mode/common -I$(MATLAB_ROOT)/rtw/c/ert -I$(MATLAB_ROOT)/toolbox/dsp/include -I$(MATLAB_ROOT)/toolbox/dsp/extern/src/export/include/src -I$(MATLAB_ROOT)/toolbox/dsp/extern/src/export/include -I$(MATLAB_ROOT)/toolbox/shared/dsp/vision/matlab/include -I$(MATLAB_WORKSPACE)/C/ProgramData/MATLAB/SupportPackages/R2020a/toolbox/realtime/targets/raspi/include -I$(MATLAB_WORKSPACE)/C/ProgramData/MATLAB/SupportPackages/R2020a/toolbox/realtime/targets/raspi/server -I$(MATLAB_ROOT)/toolbox/coder/rtiostream/src/utils -I$(MATLAB_ROOT)/toolbox/target/codertarget/rtos/inc

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_ = -DMODEL=CollectPhaseSpaceData -DNUMST=2 -DNCSTATES=0 -DHAVESTDIO -DMODEL_HAS_DYNAMICALLY_LOADED_SFCNS=0 -DCLASSIC_INTERFACE=0 -DALLOCATIONFCN=0 -DTID01EQ=1 -DTERMFCN=1 -DONESTEPFCN=1 -DMAT_FILE=0 -DMULTI_INSTANCE_CODE=0 -DINTEGER_CODE=0 -DMT=0 -D__linux__ -DARM_PROJECT -D_USE_TARGET_UDP_ -D_RUNONTARGETHARDWARE_BUILD_ -DSTACK_SIZE=64 -D__MW_TARGET_USE_HARDWARE_RESOURCES_H__ -DRT
DEFINES_BUILD_ARGS = -DCLASSIC_INTERFACE=0 -DALLOCATIONFCN=0 -DTERMFCN=1 -DONESTEPFCN=1 -DMAT_FILE=0 -DMULTI_INSTANCE_CODE=0 -DINTEGER_CODE=0 -DMT=0
DEFINES_CUSTOM = 
DEFINES_OPTS = -DTID01EQ=1
DEFINES_SKIPFORSIL = -D__linux__ -DARM_PROJECT -D_USE_TARGET_UDP_ -D_RUNONTARGETHARDWARE_BUILD_ -DSTACK_SIZE=64 -DRT
DEFINES_STANDARD = -DMODEL=CollectPhaseSpaceData -DNUMST=2 -DNCSTATES=0 -DHAVESTDIO -DMODEL_HAS_DYNAMICALLY_LOADED_SFCNS=0

DEFINES = $(DEFINES_) $(DEFINES_BUILD_ARGS) $(DEFINES_CUSTOM) $(DEFINES_OPTS) $(DEFINES_SKIPFORSIL) $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/src/socket.cpp $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/src/owl_rpd.cpp $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/src/owl.cpp $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/src/libowl_socket.cpp $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/src/libowl.cpp $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/src/filters.cpp $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/src/phasespace_functions.cpp $(MATLAB_ROOT)/toolbox/shared/spc/src_ml/extern/src/DAHostLib_Network.c $(MATLAB_ROOT)/toolbox/shared/spc/src_ml/extern/src/DAHostLib_rtw.c $(START_DIR)/CollectPhaseSpaceData_ert_rtw/CollectPhaseSpaceData.cpp $(START_DIR)/CollectPhaseSpaceData_ert_rtw/CollectPhaseSpaceData_data.cpp $(MATLAB_WORKSPACE)/C/ProgramData/MATLAB/SupportPackages/R2020a/toolbox/realtime/targets/raspi/server/MW_raspi_init.c $(MATLAB_WORKSPACE)/C/ProgramData/MATLAB/SupportPackages/R2020a/toolbox/realtime/targets/raspi/src/periphs/MW_Pyserver_control.c $(MATLAB_ROOT)/toolbox/target/codertarget/rtos/src/linuxinitialize.cpp $(MATLAB_WORKSPACE)/C/ProgramData/MATLAB/SupportPackages/R2020a/toolbox/target/supportpackages/raspberrypi/src/linuxUDP.c

MAIN_SRC = $(START_DIR)/CollectPhaseSpaceData_ert_rtw/ert_main.cpp

ALL_SRCS = $(SRCS) $(MAIN_SRC)

###########################################################################
## OBJECTS
###########################################################################

OBJS = socket.cpp.o owl_rpd.cpp.o owl.cpp.o libowl_socket.cpp.o libowl.cpp.o filters.cpp.o phasespace_functions.cpp.o DAHostLib_Network.c.o DAHostLib_rtw.c.o CollectPhaseSpaceData.cpp.o CollectPhaseSpaceData_data.cpp.o MW_raspi_init.c.o MW_Pyserver_control.c.o linuxinitialize.cpp.o linuxUDP.c.o

MAIN_OBJ = ert_main.cpp.o

ALL_OBJS = $(OBJS) $(MAIN_OBJ)

###########################################################################
## PREBUILT OBJECT FILES
###########################################################################

PREBUILT_OBJS = 

###########################################################################
## LIBRARIES
###########################################################################

LIBS = 

###########################################################################
## SYSTEM LIBRARIES
###########################################################################

SYSTEM_LIBS = 

###########################################################################
## ADDITIONAL TOOLCHAIN FLAGS
###########################################################################

#---------------
# C Compiler
#---------------

CFLAGS_SKIPFORSIL =   -fpermissive
CFLAGS_BASIC = $(DEFINES) $(INCLUDES)

CFLAGS += $(CFLAGS_SKIPFORSIL) $(CFLAGS_BASIC)

#-----------------
# C++ Compiler
#-----------------

CPPFLAGS_SKIPFORSIL =   -fpermissive
CPPFLAGS_BASIC = $(DEFINES) $(INCLUDES)

CPPFLAGS += $(CPPFLAGS_SKIPFORSIL) $(CPPFLAGS_BASIC)

#---------------
# C++ Linker
#---------------

CPP_LDFLAGS_ = -ldl
CPP_LDFLAGS_SKIPFORSIL =  

CPP_LDFLAGS += $(CPP_LDFLAGS_) $(CPP_LDFLAGS_SKIPFORSIL)

#------------------------------
# C++ Shared Library Linker
#------------------------------

CPP_SHAREDLIB_LDFLAGS_ = -ldl
CPP_SHAREDLIB_LDFLAGS_SKIPFORSIL =  

CPP_SHAREDLIB_LDFLAGS += $(CPP_SHAREDLIB_LDFLAGS_) $(CPP_SHAREDLIB_LDFLAGS_SKIPFORSIL)

#-----------
# Linker
#-----------

LDFLAGS_ = -ldl
LDFLAGS_SKIPFORSIL =  

LDFLAGS += $(LDFLAGS_) $(LDFLAGS_SKIPFORSIL)

#--------------------------
# Shared Library Linker
#--------------------------

SHAREDLIB_LDFLAGS_ = -ldl
SHAREDLIB_LDFLAGS_SKIPFORSIL =  

SHAREDLIB_LDFLAGS += $(SHAREDLIB_LDFLAGS_) $(SHAREDLIB_LDFLAGS_SKIPFORSIL)

###########################################################################
## INLINED COMMANDS
###########################################################################


DERIVED_SRCS = $(subst .o,.dep,$(OBJS))

build:

%.dep:



-include codertarget_assembly_flags.mk
-include *.dep


###########################################################################
## PHONY TARGETS
###########################################################################

.PHONY : all build buildobj clean info prebuild download execute


all : build
	echo "### Successfully generated all binary outputs."


build : prebuild $(PRODUCT)


buildobj : prebuild $(OBJS) $(PREBUILT_OBJS)
	echo "### Successfully generated all binary outputs."


prebuild : 


download : $(PRODUCT)


execute : download
	echo "### Invoking postbuild tool "Execute" ..."
	$(EXECUTE) $(EXECUTE_FLAGS)
	echo "### Done invoking postbuild tool."


###########################################################################
## FINAL TARGET
###########################################################################

#-------------------------------------------
# Create a standalone executable            
#-------------------------------------------

$(PRODUCT) : $(OBJS) $(PREBUILT_OBJS) $(MAIN_OBJ)
	echo "### Creating standalone executable "$(PRODUCT)" ..."
	$(CPP_LD) $(CPP_LDFLAGS) -o $(PRODUCT) $(OBJS) $(MAIN_OBJ) $(SYSTEM_LIBS) $(TOOLCHAIN_LIBS)
	echo "### Created: $(PRODUCT)"


###########################################################################
## INTERMEDIATE TARGETS
###########################################################################

#---------------------
# SOURCE-TO-OBJECT
#---------------------

%.c.o : %.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.s.o : %.s
	$(AS) $(ASFLAGS) -o "$@" "$<"


%.cpp.o : %.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.c.o : $(RELATIVE_PATH_TO_ANCHOR)/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.s.o : $(RELATIVE_PATH_TO_ANCHOR)/%.s
	$(AS) $(ASFLAGS) -o "$@" "$<"


%.cpp.o : $(RELATIVE_PATH_TO_ANCHOR)/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.c.o : $(START_DIR)/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.s.o : $(START_DIR)/%.s
	$(AS) $(ASFLAGS) -o "$@" "$<"


%.cpp.o : $(START_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.c.o : $(MATLAB_ROOT)/toolbox/shared/spc/src_ml/extern/src/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.s.o : $(MATLAB_ROOT)/toolbox/shared/spc/src_ml/extern/src/%.s
	$(AS) $(ASFLAGS) -o "$@" "$<"


%.cpp.o : $(MATLAB_ROOT)/toolbox/shared/spc/src_ml/extern/src/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.c.o : $(START_DIR)/CollectPhaseSpaceData_ert_rtw/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.s.o : $(START_DIR)/CollectPhaseSpaceData_ert_rtw/%.s
	$(AS) $(ASFLAGS) -o "$@" "$<"


%.cpp.o : $(START_DIR)/CollectPhaseSpaceData_ert_rtw/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.c.o : $(MATLAB_ROOT)/rtw/c/src/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.s.o : $(MATLAB_ROOT)/rtw/c/src/%.s
	$(AS) $(ASFLAGS) -o "$@" "$<"


%.cpp.o : $(MATLAB_ROOT)/rtw/c/src/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.c.o : $(MATLAB_ROOT)/simulink/src/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.s.o : $(MATLAB_ROOT)/simulink/src/%.s
	$(AS) $(ASFLAGS) -o "$@" "$<"


%.cpp.o : $(MATLAB_ROOT)/simulink/src/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


socket.cpp.o : $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/src/socket.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


owl_rpd.cpp.o : $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/src/owl_rpd.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


owl.cpp.o : $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/src/owl.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


libowl_socket.cpp.o : $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/src/libowl_socket.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


libowl.cpp.o : $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/src/libowl.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


filters.cpp.o : $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/src/filters.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


phasespace_functions.cpp.o : $(MATLAB_WORKSPACE)/C/Users/SPOT/Documents/GitHub/AlexCrain-SPOT/Custom_Library/PhaseSpase_Cameras/PHASESPACE/src/phasespace_functions.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


DAHostLib_Network.c.o : $(MATLAB_ROOT)/toolbox/shared/spc/src_ml/extern/src/DAHostLib_Network.c
	$(CC) $(CFLAGS) -o "$@" "$<"


DAHostLib_rtw.c.o : $(MATLAB_ROOT)/toolbox/shared/spc/src_ml/extern/src/DAHostLib_rtw.c
	$(CC) $(CFLAGS) -o "$@" "$<"


ert_main.cpp.o : $(START_DIR)/CollectPhaseSpaceData_ert_rtw/ert_main.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


CollectPhaseSpaceData.cpp.o : $(START_DIR)/CollectPhaseSpaceData_ert_rtw/CollectPhaseSpaceData.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


CollectPhaseSpaceData_data.cpp.o : $(START_DIR)/CollectPhaseSpaceData_ert_rtw/CollectPhaseSpaceData_data.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


MW_raspi_init.c.o : $(MATLAB_WORKSPACE)/C/ProgramData/MATLAB/SupportPackages/R2020a/toolbox/realtime/targets/raspi/server/MW_raspi_init.c
	$(CC) $(CFLAGS) -o "$@" "$<"


MW_Pyserver_control.c.o : $(MATLAB_WORKSPACE)/C/ProgramData/MATLAB/SupportPackages/R2020a/toolbox/realtime/targets/raspi/src/periphs/MW_Pyserver_control.c
	$(CC) $(CFLAGS) -o "$@" "$<"


linuxinitialize.cpp.o : $(MATLAB_ROOT)/toolbox/target/codertarget/rtos/src/linuxinitialize.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


linuxUDP.c.o : $(MATLAB_WORKSPACE)/C/ProgramData/MATLAB/SupportPackages/R2020a/toolbox/target/supportpackages/raspberrypi/src/linuxUDP.c
	$(CC) $(CFLAGS) -o "$@" "$<"


###########################################################################
## DEPENDENCIES
###########################################################################

$(ALL_OBJS) : rtw_proj.tmw $(MAKEFILE)


###########################################################################
## MISCELLANEOUS TARGETS
###########################################################################

info : 
	echo "### PRODUCT = $(PRODUCT)"
	echo "### PRODUCT_TYPE = $(PRODUCT_TYPE)"
	echo "### BUILD_TYPE = $(BUILD_TYPE)"
	echo "### INCLUDES = $(INCLUDES)"
	echo "### DEFINES = $(DEFINES)"
	echo "### ALL_SRCS = $(ALL_SRCS)"
	echo "### ALL_OBJS = $(ALL_OBJS)"
	echo "### LIBS = $(LIBS)"
	echo "### MODELREF_LIBS = $(MODELREF_LIBS)"
	echo "### SYSTEM_LIBS = $(SYSTEM_LIBS)"
	echo "### TOOLCHAIN_LIBS = $(TOOLCHAIN_LIBS)"
	echo "### ASFLAGS = $(ASFLAGS)"
	echo "### CFLAGS = $(CFLAGS)"
	echo "### LDFLAGS = $(LDFLAGS)"
	echo "### SHAREDLIB_LDFLAGS = $(SHAREDLIB_LDFLAGS)"
	echo "### CPPFLAGS = $(CPPFLAGS)"
	echo "### CPP_LDFLAGS = $(CPP_LDFLAGS)"
	echo "### CPP_SHAREDLIB_LDFLAGS = $(CPP_SHAREDLIB_LDFLAGS)"
	echo "### ARFLAGS = $(ARFLAGS)"
	echo "### MEX_CFLAGS = $(MEX_CFLAGS)"
	echo "### MEX_CPPFLAGS = $(MEX_CPPFLAGS)"
	echo "### MEX_LDFLAGS = $(MEX_LDFLAGS)"
	echo "### MEX_CPPLDFLAGS = $(MEX_CPPLDFLAGS)"
	echo "### DOWNLOAD_FLAGS = $(DOWNLOAD_FLAGS)"
	echo "### EXECUTE_FLAGS = $(EXECUTE_FLAGS)"
	echo "### MAKE_FLAGS = $(MAKE_FLAGS)"


clean : 
	$(ECHO) "### Deleting all derived files..."
	$(RM) $(PRODUCT)
	$(RM) $(ALL_OBJS)
	$(RM) *.c.dep
	$(RM) *.cpp.dep
	$(ECHO) "### Deleted all derived files."



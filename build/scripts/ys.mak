#-------------------------------------------------------------#
#Makefile for product TF - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#
ifeq ($(TARGET),4gmx)
CCysFLAGS=-DTF -DYS -DDEBUG -DYS_LTE_PAL -DLTE_PAL_ENB -DYS_PERF_MEAS -DLCCTF -DLCYSUITFU -DLCTFU -DLCTFUITFU -DTFU_VER_2 \
          -UTFU_TDD -DRG -DLCYSUICTF -DAPI_MAIN -DYS_MSPD -UYS_MS_PHY -DYS_MIB_WARND  -DUSE_UDP
else # arm and x86 -- for FT only
CCysFLAGS=-DTF -DYS -DDEBUG -DYS_LTE_PAL -DLTE_PAL_ENB -DYS_PERF_MEAS -DLCCTF -DLCYSUITFU -DLCTFU -DLCTFUITFU -DTFU_VER_2 \
          -UTFU_TDD -DRG -DLCYSUICTF -DAPI_MAIN -DYS_MSPD -UYS_MS_PHY -UYS_MIB_WARND  -DUSE_UDP -DLCLYS -DLCYSMILYS -DUSE_SSI_TMR \
          -DYS_JMP_PHY_RUN_STE -DDEBUGP -URNT_ARM_CC_UNSUPP
endif

#CCysFLAGS=-DTF -DYS -DDEBUG -UYS_LTE_PAL -ULTE_PAL_ENB \
 #         -DYS_PERF_MEAS -DLCCTF -DLCYSUITFU -DLCTFU -DLCTFUITFU -DTFU_VER_2 \
  #        -UTFU_TDD -DRG -DLCYSUICTF -DAPI_MAIN -DLCYSMILYS -DYS_WIRESHARK_LOG \
   #       -DLCLYS \
    #      -DYS_MSPD -DYS_MSPD_PHYSTUB -DYS_MSPD_PHYSTUB_SS_TMR -DYS_MSPD_OSAL
ifeq ($(PWAV_MACPAL), 1)
PALFLAGS=-DLINUX -DPN_MEM_DEBUG -DPN_USE_MSGQ -DPAL_VERSION=1  
PAL_DIR=$(IN_DIR)/../pseudophy
endif

BUILD_PREFIX            := "==> Building"
V                       := @

# Product sources-------------------------------------------------------
ysSRCS= \
    $(IN_DIR)/ys_ptui.c    \
    $(IN_DIR)/ys_ptmi.c    \
    $(IN_DIR)/ys_utl.c     \
    $(IN_DIR)/ys_init.c    \
    $(IN_DIR)/ys_mi.c      \
    $(IN_DIR)/ys_db.c      \
    $(IN_DIR)/ys_ms_utl.c  \
    $(IN_DIR)/ys_ms_cfg.c  \
    $(IN_DIR)/ys_ms_ui.c   \
    $(IN_DIR)/ys_ms_li.c   \
    $(IN_DIR)/ys_ms_map.c  \
    $(IN_DIR)/ys_ms_ul.c   \
    $(IN_DIR)/ys_ms_dl.c   \
    $(IN_DIR)/ys_ms_pal.c   \
    $(IN_DIR)/ys_ex_ms.c   \
    $(IN_DIR)/ys_id.c   

ifeq ($(PWAV_MACPAL), 1)
ysSRCS+= \
    $(PAL_DIR)/Logger.c   \
    $(PAL_DIR)/MemoryDebug.c  \
    $(PAL_DIR)/MessageQueue.c  \
    $(PAL_DIR)/PW_PALCommon.c    \
    $(PAL_DIR)/PW_PALInterface.c \
    $(PAL_DIR)/PW_PALMessageBuilder.c \
    $(PAL_DIR)/PW_PALMessageParser.c \
    $(PAL_DIR)/PW_PALMessageQueue.c \
    $(PAL_DIR)/PW_PALParameterBuilder.c \
    $(PAL_DIR)/PW_PALParameterParser.c \
    $(PAL_DIR)/PW_PALStateMachine.c \
    $(PAL_DIR)/PW_PALTimer.c \
    $(PAL_DIR)/PW_PALSocket.c \
    $(PAL_DIR)/Timer.c \
    $(PAL_DIR)/PW_PALProtocolStackShared.c 
endif
#    $(IN_DIR)/ys_dft.c     \

ysStubSRCS= $(IN_DIR)/ys_ms_stub.c

# Product objects-------------------------------------------------------
ysOBJS= \
    $(OUT_DIR)/ys_ptui.$(OBJ)    \
    $(OUT_DIR)/ys_ptmi.$(OBJ)    \
    $(OUT_DIR)/ys_utl.$(OBJ)     \
    $(OUT_DIR)/ys_init.$(OBJ)    \
    $(OUT_DIR)/ys_mi.$(OBJ)      \
    $(OUT_DIR)/ys_db.$(OBJ)      \
    $(OUT_DIR)/ys_ms_utl.$(OBJ)  \
    $(OUT_DIR)/ys_ms_cfg.$(OBJ)  \
    $(OUT_DIR)/ys_ms_ui.$(OBJ)   \
    $(OUT_DIR)/ys_ms_li.$(OBJ)   \
    $(OUT_DIR)/ys_ms_map.$(OBJ)  \
    $(OUT_DIR)/ys_ms_ul.$(OBJ)   \
    $(OUT_DIR)/ys_ms_dl.$(OBJ)   \
    $(OUT_DIR)/ys_ms_pal.$(OBJ)   \
    $(OUT_DIR)/ys_ex_ms.$(OBJ)   \
    $(OUT_DIR)/ys_id.$(OBJ)      

ifeq ($(PWAV_MACPAL), 1)
ysOBJS+= \
    $(OUT_DIR)/Logger.$(OBJ)    \
    $(OUT_DIR)/MemoryDebug.$(OBJ)    \
    $(OUT_DIR)/MessageQueue.$(OBJ)     \
    $(OUT_DIR)/PW_PALCommon.$(OBJ)    \
    $(OUT_DIR)/PW_PALInterface.$(OBJ)      \
    $(OUT_DIR)/PW_PALMessageBuilder.$(OBJ)      \
    $(OUT_DIR)/PW_PALMessageParser.$(OBJ)  \
    $(OUT_DIR)/PW_PALMessageQueue.$(OBJ)  \
    $(OUT_DIR)/PW_PALParameterBuilder.$(OBJ)   \
    $(OUT_DIR)/PW_PALParameterParser.$(OBJ)   \
    $(OUT_DIR)/PW_PALStateMachine.$(OBJ)  \
    $(OUT_DIR)/PW_PALTimer.$(OBJ)   \
    $(OUT_DIR)/PW_PALSocket.$(OBJ)   \
    $(OUT_DIR)/Timer.$(OBJ) \
    $(OUT_DIR)/PW_PALProtocolStackShared.$(OBJ) 
endif
#    $(OUT_DIR)/ys_dft.$(OBJ)     \

ysStubOBJS= $(OUT_DIR)/ys_ms_stub.$(OBJ)

YSCM_INC=$(IN_DIR)/ys_ms.h $(IN_DIR)/ys_err.h $(IN_DIR)/ys_ms.x $(IN_DIR)/ys_ms_pal.x $(PHY_DIR)/tcb.h

ifeq ($(PWAV_MACPAL), 1)
PAL_INC=-I$(IN_DIR)/../pseudophy -I$(IN_DIR)
endif


ALL_INC=$(YSCM_INC) $(CM_INC)

#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/ys_ptui.$(OBJ): $(IN_DIR)/ys_ptui.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ptui.c -o \
	$(OUT_DIR)/ys_ptui.$(OBJ)

$(OUT_DIR)/ys_ptmi.$(OBJ): $(IN_DIR)/ys_ptmi.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ptmi.c -o \
	$(OUT_DIR)/ys_ptmi.$(OBJ)

$(OUT_DIR)/ys_utl.$(OBJ): $(IN_DIR)/ys_utl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_utl.c -o \
	$(OUT_DIR)/ys_utl.$(OBJ)

$(OUT_DIR)/ys_init.$(OBJ): $(IN_DIR)/ys_init.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_init.c -o \
	$(OUT_DIR)/ys_init.$(OBJ)

$(OUT_DIR)/ys_mi.$(OBJ): $(IN_DIR)/ys_mi.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_mi.c -o \
	$(OUT_DIR)/ys_mi.$(OBJ)

$(OUT_DIR)/ys_db.$(OBJ): $(IN_DIR)/ys_db.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_db.c -o \
	$(OUT_DIR)/ys_db.$(OBJ)

$(OUT_DIR)/ys_ms_utl.$(OBJ): $(IN_DIR)/ys_ms_utl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_utl.c -o \
	$(OUT_DIR)/ys_ms_utl.$(OBJ)

$(OUT_DIR)/ys_ms_cfg.$(OBJ): $(IN_DIR)/ys_ms_cfg.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_cfg.c -o \
	$(OUT_DIR)/ys_ms_cfg.$(OBJ)

$(OUT_DIR)/ys_ms_ui.$(OBJ): $(IN_DIR)/ys_ms_ui.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_ui.c -o \
	$(OUT_DIR)/ys_ms_ui.$(OBJ)

$(OUT_DIR)/ys_ms_li.$(OBJ): $(IN_DIR)/ys_ms_li.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_li.c -o \
	$(OUT_DIR)/ys_ms_li.$(OBJ)

$(OUT_DIR)/ys_ms_map.$(OBJ): $(IN_DIR)/ys_ms_map.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_map.c -o \
	$(OUT_DIR)/ys_ms_map.$(OBJ)

$(OUT_DIR)/ys_ms_ul.$(OBJ): $(IN_DIR)/ys_ms_ul.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_ul.c -o \
	$(OUT_DIR)/ys_ms_ul.$(OBJ)

$(OUT_DIR)/ys_ms_dl.$(OBJ): $(IN_DIR)/ys_ms_dl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_dl.c -o \
	$(OUT_DIR)/ys_ms_dl.$(OBJ)

$(OUT_DIR)/ys_ms_pal.$(OBJ): $(IN_DIR)/ys_ms_pal.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_pal.c -o \
	$(OUT_DIR)/ys_ms_pal.$(OBJ)

$(OUT_DIR)/ys_ms_stub.$(OBJ): $(IN_DIR)/ys_ms_stub.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_stub.c -o \
	$(OUT_DIR)/ys_ms_stub.$(OBJ)

$(OUT_DIR)/ys_dft.$(OBJ): $(IN_DIR)/ys_dft.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_dft.c -o \
	$(OUT_DIR)/ys_dft.$(OBJ)

$(OUT_DIR)/ys_ex_ms.$(OBJ): $(IN_DIR)/ys_ex_ms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ex_ms.c -o \
	$(OUT_DIR)/ys_ex_ms.$(OBJ)

$(OUT_DIR)/ys_id.$(OBJ): $(IN_DIR)/ys_id.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_id.c -o \
	$(OUT_DIR)/ys_id.$(OBJ)

ifeq ($(PWAV_MACPAL), 1)
$(OUT_DIR)/Logger.$(OBJ): $(PAL_DIR)/Logger.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/Logger.c -o \
	$(OUT_DIR)/Logger.$(OBJ)

$(OUT_DIR)/MemoryDebug.$(OBJ): $(PAL_DIR)/MemoryDebug.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/MemoryDebug.c -o \
	$(OUT_DIR)/MemoryDebug.$(OBJ)

$(OUT_DIR)/MessageQueue.$(OBJ): $(PAL_DIR)/MessageQueue.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/MessageQueue.c -o \
	$(OUT_DIR)/MessageQueue.$(OBJ)

$(OUT_DIR)/PW_PALCommon.$(OBJ): $(PAL_DIR)/PW_PALCommon.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/PW_PALCommon.c -o \
	$(OUT_DIR)/PW_PALCommon.$(OBJ)

$(OUT_DIR)/PW_PALMessageBuilder.$(OBJ): $(PAL_DIR)/PW_PALMessageBuilder.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/PW_PALMessageBuilder.c -o \
	$(OUT_DIR)/PW_PALMessageBuilder.$(OBJ)

$(OUT_DIR)/PW_PALMessageParser.$(OBJ): $(PAL_DIR)/PW_PALMessageParser.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/PW_PALMessageParser.c -o \
	$(OUT_DIR)/PW_PALMessageParser.$(OBJ)

$(OUT_DIR)/PW_PALMessageQueue.$(OBJ): $(PAL_DIR)/PW_PALMessageQueue.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/PW_PALMessageQueue.c -o \
	$(OUT_DIR)/PW_PALMessageQueue.$(OBJ)

$(OUT_DIR)/PW_PALParameterBuilder.$(OBJ): $(PAL_DIR)/PW_PALParameterBuilder.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/PW_PALParameterBuilder.c -o \
	$(OUT_DIR)/PW_PALParameterBuilder.$(OBJ)

$(OUT_DIR)/PW_PALParameterParser.$(OBJ): $(PAL_DIR)/PW_PALParameterParser.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/PW_PALParameterParser.c -o \
	$(OUT_DIR)/PW_PALParameterParser.$(OBJ)

$(OUT_DIR)/PW_PALStateMachine.$(OBJ): $(PAL_DIR)/PW_PALStateMachine.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/PW_PALStateMachine.c -o \
	$(OUT_DIR)/PW_PALStateMachine.$(OBJ)

$(OUT_DIR)/PW_PALTimer.$(OBJ): $(PAL_DIR)/PW_PALTimer.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/PW_PALTimer.c -o \
	$(OUT_DIR)/PW_PALTimer.$(OBJ)

$(OUT_DIR)/PW_PALSocket.$(OBJ): $(PAL_DIR)/PW_PALSocket.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/PW_PALSocket.c -o \
	$(OUT_DIR)/PW_PALSocket.$(OBJ)

$(OUT_DIR)/Timer.$(OBJ): $(PAL_DIR)/Timer.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/Timer.c -o \
	$(OUT_DIR)/Timer.$(OBJ)	
	
$(OUT_DIR)/PW_PALInterface.$(OBJ): $(PAL_DIR)/PW_PALInterface.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/PW_PALInterface.c \
	-o $(OUT_DIR)/PW_PALInterface.$(OBJ)

$(OUT_DIR)/PW_PALProtocolStackShared.$(OBJ): $(PAL_DIR)/PW_PALProtocolStackShared.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(PAL_DIR)/PW_PALProtocolStackShared.c \
	-o $(OUT_DIR)/PW_PALProtocolStackShared.$(OBJ)
endif
#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/liblnxys.a:$(ysOBJS) $(ysStubOBJS)
	ar -cr $(LIB_DIR)/libtf.a $(ysStubOBJS) $(ysOBJS) 

$(LIB_DIR)/liblnxysms.a:$(ysOBJS)
	ar -cr $(LIB_DIR)/libtf.a $(ysOBJS) 

$(LIB_DIR)/libcavys.a:$(ysOBJS)
	ar -cr $(LIB_DIR)/libtf.a $(ysOBJS) 

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	rm -f $(LIB_DIR)/libtf.a $(ysStubOBJS) $(ysOBJS)

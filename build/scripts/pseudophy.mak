#-------------------------------------------------------------#
#Makefile for product TF - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

PALFLAGS=-DLINUX -DPN_MEM_DEBUG -DPN_USE_MSGQ -DPAL_VERSION=1  


BUILD_PREFIX            := "==> Building"
V                       := @

# Product sources-------------------------------------------------------
ppSRCS= \
    $(IN_DIR)/Logger.c   \
    $(IN_DIR)/MemoryDebug.c  \
    $(IN_DIR)/MessageQueue.c  \
    $(IN_DIR)/PW_PALCommon.c    \
    $(IN_DIR)/PW_PALInterface.c \
    $(IN_DIR)/PW_PALMessageBuilder.c \
    $(IN_DIR)/PW_PALMessageParser.c \
    $(IN_DIR)/PW_PALMessageQueue.c \
    $(IN_DIR)/PW_PALParameterBuilder.c \
    $(IN_DIR)/PW_PALParameterParser.c \
    $(IN_DIR)/PW_PALStateMachine.c \
    $(IN_DIR)/PW_PALTimer.c \
    $(IN_DIR)/PW_PALSocket.c \
    $(IN_DIR)/Timer.c \
    $(IN_DIR)/PW_PALProtocolStackShared.c \
    $(IN_DIR)/yswrapper.c 


# Product objects-------------------------------------------------------

ppOBJS= \
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
    $(OUT_DIR)/PW_PALProtocolStackShared.$(OBJ) \
    $(OUT_DIR)/yswrapper.$(OBJ) 



PP_INC=$(IN_DIR)/../macphycl/ys_ms.h $(IN_DIR)/../macphycl/ys_err.h $(IN_DIR)/../macphycl/ys_ms.x $(IN_DIR)/../macphycl/ys_ms_pal.x


PAL_INC=-I$(IN_DIR) -I$(IN_DIR)/../macphycl



ALL_INC=$(PP_INC) $(CM_INC)

#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#


$(OUT_DIR)/Logger.$(OBJ): $(IN_DIR)/Logger.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/Logger.c -o \
	$(OUT_DIR)/Logger.$(OBJ)
	
$(OUT_DIR)/yswrapper.$(OBJ): $(IN_DIR)/yswrapper.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/yswrapper.c -o \
	$(OUT_DIR)/yswrapper.$(OBJ)

$(OUT_DIR)/MemoryDebug.$(OBJ): $(IN_DIR)/MemoryDebug.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/MemoryDebug.c -o \
	$(OUT_DIR)/MemoryDebug.$(OBJ)

$(OUT_DIR)/MessageQueue.$(OBJ): $(IN_DIR)/MessageQueue.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/MessageQueue.c -o \
	$(OUT_DIR)/MessageQueue.$(OBJ)

$(OUT_DIR)/PW_PALCommon.$(OBJ): $(IN_DIR)/PW_PALCommon.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/PW_PALCommon.c -o \
	$(OUT_DIR)/PW_PALCommon.$(OBJ)

$(OUT_DIR)/PW_PALMessageBuilder.$(OBJ): $(IN_DIR)/PW_PALMessageBuilder.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/PW_PALMessageBuilder.c -o \
	$(OUT_DIR)/PW_PALMessageBuilder.$(OBJ)

$(OUT_DIR)/PW_PALMessageParser.$(OBJ): $(IN_DIR)/PW_PALMessageParser.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/PW_PALMessageParser.c -o \
	$(OUT_DIR)/PW_PALMessageParser.$(OBJ)

$(OUT_DIR)/PW_PALMessageQueue.$(OBJ): $(IN_DIR)/PW_PALMessageQueue.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/PW_PALMessageQueue.c -o \
	$(OUT_DIR)/PW_PALMessageQueue.$(OBJ)

$(OUT_DIR)/PW_PALParameterBuilder.$(OBJ): $(IN_DIR)/PW_PALParameterBuilder.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/PW_PALParameterBuilder.c -o \
	$(OUT_DIR)/PW_PALParameterBuilder.$(OBJ)

$(OUT_DIR)/PW_PALParameterParser.$(OBJ): $(IN_DIR)/PW_PALParameterParser.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/PW_PALParameterParser.c -o \
	$(OUT_DIR)/PW_PALParameterParser.$(OBJ)

$(OUT_DIR)/PW_PALStateMachine.$(OBJ): $(IN_DIR)/PW_PALStateMachine.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/PW_PALStateMachine.c -o \
	$(OUT_DIR)/PW_PALStateMachine.$(OBJ)

$(OUT_DIR)/PW_PALTimer.$(OBJ): $(IN_DIR)/PW_PALTimer.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/PW_PALTimer.c -o \
	$(OUT_DIR)/PW_PALTimer.$(OBJ)

$(OUT_DIR)/PW_PALSocket.$(OBJ): $(IN_DIR)/PW_PALSocket.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/PW_PALSocket.c -o \
	$(OUT_DIR)/PW_PALSocket.$(OBJ)

$(OUT_DIR)/Timer.$(OBJ): $(IN_DIR)/Timer.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/Timer.c -o \
	$(OUT_DIR)/Timer.$(OBJ)	
	
$(OUT_DIR)/PW_PALInterface.$(OBJ): $(IN_DIR)/PW_PALInterface.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/PW_PALInterface.c \
	-o $(OUT_DIR)/PW_PALInterface.$(OBJ)
	
	
$(OUT_DIR)/PW_PALProtocolStackShared.$(OBJ): $(IN_DIR)/PW_PALProtocolStackShared.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(PALFLAGS) $(PAL_INC) $(IN_DIR)/PW_PALProtocolStackShared.c \
	-o $(OUT_DIR)/PW_PALProtocolStackShared.$(OBJ)
	

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/liblnxphy.a:$(ppOBJS) $(ppStubOBJS)
	ar -cr $(LIB_DIR)/libphy.a $(ppOBJS) 


#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	rm -f $(LIB_DIR)/libpphy.a $(ppOBJS)


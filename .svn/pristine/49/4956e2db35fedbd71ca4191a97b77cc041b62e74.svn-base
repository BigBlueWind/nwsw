#-------------------------------------------------------------#
#Makefile for product mt - script generated.
#Only the $(CCmtFLAGS) may be modified.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

# CONSTDIO CONVAL are added to solve bs.sh script issue. (Jira Id BS-703)
CCmtFLAGS=-DBIT_64 -DSS_DRVR_SUPPORT  -DSS_MT_TMR -USS_LOGGER_SUPPORT -DCONSTDIO #-DCONAVL #<---Insert mt specific defines here

#-------------------------------------------------------------#
#File/Obj macros             
#-------------------------------------------------------------#
MT_SRC_FILES= \
	mt_id.c\
	mt_ss.c\
	ss_gen.c\
	ss_id.c\
	ss_mem.c\
	ss_msg.c\
	ss_pack.c\
	ss_queue.c\
	ss_strm.c\
	ss_task.c\
	ss_drvr.c\
	ss_timer.c

ifneq ($(EXE), l2sim)
MT_SRC_FILES += mt_4gmx.c
endif

MT_OBJ_FILES  := $(patsubst %.c, $(OUT_DIR)/%.o, $(MT_SRC_FILES) )

MT_INC=\
   $(IN_DIR)/ss_msg.x $(IN_DIR)/ss_drvr.x \
   $(IN_DIR)/ss_strm.h $(IN_DIR)/ss_strm.x  $(IN_DIR)/ss_task.h \
   $(IN_DIR)/ss_task.x $(IN_DIR)/ss_timer.x $(IN_DIR)/ss_gen.x \
   $(IN_DIR)/mt_ss.x


ALL_INC=$(MT_INC) $(CM_INC)

BUILD_PREFIX            := "==> Building"
V                       := @

#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
#MT ObjectFiles
$(OUT_DIR)/%.o: $(IN_DIR)/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $< -o $@

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/libmt_$(EXE).a:$(MT_OBJ_FILES) #$(ALL_INC)
	$(V) $(AR) -cr $(LIB_DIR)/libmt_$(EXE).a $(MT_OBJ_FILES)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(LIB_DIR)/libmt_$(EXE).a $(MT_OBJ_FILES)


#-------------------------------------------------------------#
#Makefile for product TF - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCysFLAGS=-DTF -DYS -DDEBUG -DYS_LTE_PAL -DLTE_PAL_ENB -DYS_PERF_MEAS -DLCCTF -DLCYSUITFU -DLCTFU -DLCTFUITFU -DTFU_VER_2 \
          -UTFU_TDD -DRG -DLCYSUICTF -DAPI_MAIN

# Product sources-------------------------------------------------------
ysSRCS= \
    $(IN_DIR)/ys_ui.$(SRC) \
    $(IN_DIR)/ys_pal_li.$(SRC) \
    $(IN_DIR)/ys_pal_ui.$(SRC) \
    $(IN_DIR)/ys_ptui.$(SRC) \
    $(IN_DIR)/ys_pal_ptui.$(SRC) \
    $(IN_DIR)/ys_pal_msg.$(SRC) \
    $(IN_DIR)/ys_tmr.$(SRC) \
    $(IN_DIR)/ys_ex_ms.$(SRC) \
    $(IN_DIR)/ys_init.$(SRC) \
    $(IN_DIR)/ys_utl.$(SRC) \
    $(IN_DIR)/ys_pal_utl.$(SRC) 
ifeq ($(YS_MAIN_FLAG),YES)
    ysSRCS+=$(IN_DIR)/ys_main.$(SRC) 
endif


# Product objects-------------------------------------------------------
ysOBJS= \
    $(OUT_DIR)/ys_ui.$(OBJ) \
    $(OUT_DIR)/ys_pal_li.$(OBJ) \
    $(OUT_DIR)/ys_pal_ui.$(OBJ) \
    $(OUT_DIR)/ys_ptui.$(OBJ) \
    $(OUT_DIR)/ys_pal_ptui.$(OBJ) \
    $(OUT_DIR)/ys_pal_msg.$(OBJ) \
    $(OUT_DIR)/ys_tmr.$(OBJ) \
    $(OUT_DIR)/ys_ex_ms.$(OBJ) \
    $(OUT_DIR)/ys_init.$(OBJ) \
    $(OUT_DIR)/ys_utl.$(OBJ) \
    $(OUT_DIR)/ys_pal_utl.$(OBJ) 
ifeq ($(YS_MAIN_FLAG),YES)
    ysOBJS+=$(OUT_DIR)/ys_main.$(OBJ) 
endif

#YSCM_INC=$(IN_DIR)/ys.h $(IN_DIR)/ys.x

#ALL_INC=$(YSCM_INC) $(CM_INC)
ALL_INC=$(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/ys_ui.$(OBJ): $(IN_DIR)/ys_ui.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ui.$(SRC) -o \
	$(OUT_DIR)/ys_ui.$(OBJ)

$(OUT_DIR)/ys_pal_li.$(OBJ): $(IN_DIR)/ys_pal_li.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_pal_li.$(SRC) -o \
	$(OUT_DIR)/ys_pal_li.$(OBJ)

$(OUT_DIR)/ys_pal_ui.$(OBJ): $(IN_DIR)/ys_pal_ui.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_pal_ui.$(SRC) -o \
	$(OUT_DIR)/ys_pal_ui.$(OBJ)

$(OUT_DIR)/ys_ptui.$(OBJ): $(IN_DIR)/ys_ptui.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ptui.$(SRC) -o \
	$(OUT_DIR)/ys_ptui.$(OBJ)

$(OUT_DIR)/ys_pal_ptui.$(OBJ): $(IN_DIR)/ys_pal_ptui.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_pal_ptui.$(SRC) -o \
	$(OUT_DIR)/ys_pal_ptui.$(OBJ)

$(OUT_DIR)/ys_pal_msg.$(OBJ): $(IN_DIR)/ys_pal_msg.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_pal_msg.$(SRC) -o \
	$(OUT_DIR)/ys_pal_msg.$(OBJ)

$(OUT_DIR)/ys_tmr.$(OBJ): $(IN_DIR)/ys_tmr.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_tmr.$(SRC) -o \
	$(OUT_DIR)/ys_tmr.$(OBJ)

$(OUT_DIR)/ys_ex_ms.$(OBJ): $(IN_DIR)/ys_ex_ms.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ex_ms.$(SRC) -o \
	$(OUT_DIR)/ys_ex_ms.$(OBJ)

$(OUT_DIR)/ys_init.$(OBJ): $(IN_DIR)/ys_init.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_init.$(SRC) -o \
	$(OUT_DIR)/ys_init.$(OBJ)

$(OUT_DIR)/ys_utl.$(OBJ): $(IN_DIR)/ys_utl.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_utl.$(SRC) -o \
	$(OUT_DIR)/ys_utl.$(OBJ)

$(OUT_DIR)/ys_pal_utl.$(OBJ): $(IN_DIR)/ys_pal_utl.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_pal_utl.$(SRC) -o \
	$(OUT_DIR)/ys_pal_utl.$(OBJ)

$(OUT_DIR)/ys_main.$(OBJ): $(IN_DIR)/ys_main.$(SRC) $(ALL_INC)
ifeq ($(YS_MAIN_FLAG),YES)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_main.$(SRC) -o \
	$(OUT_DIR)/ys_main.$(OBJ) 
endif

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#

$(OUT_DIR)/liblnxys.a:$(ysOBJS)
	ar -cr $(OUT_DIR)/libtf.a $(ysOBJS) 

$(OUT_DIR)/ysperpro:$(ysOBJS)

$(OUT_DIR)/libcavys.a:$(ysOBJS)
	ar -cr $(OUT_DIR)/libtf.a $(ysOBJS) 

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libtf.a $(tfOBJS)


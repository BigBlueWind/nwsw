#-------------------------------------------------------------#
#Makefile for product TF - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCysFLAGS=-DTF -DYS -DDEBUG -UYS_LTE_PAL -ULTE_PAL_ENB \
          -DYS_PERF_MEAS -DLCCTF -DLCYSUITFU -DLCTFU -DLCTFUITFU -DTFU_VER_2 \
          -UTFU_TDD -DRG -DLCYSUICTF -DAPI_MAIN -DLCYSMILYS -UYS_WIRESHARK_LOG \
          -DLCLYS \
          -DYS_MSPD -DYS_MSPD_PHYSTUB -DYS_MSPD_PHYSTUB_SS_TMR -DYS_MSPD_OSAL

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
#    $(OUT_DIR)/ys_dft.$(OBJ)     \

ysStubOBJS= $(OUT_DIR)/ys_ms_stub.$(OBJ)

YSCM_INC=$(IN_DIR)/ys_ms.h $(IN_DIR)/ys_err.h $(IN_DIR)/ys_ms.x $(IN_DIR)/ys_ms_pal.x

ALL_INC=$(YSCM_INC) $(CM_INC)

#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/ys_ptui.$(OBJ): $(IN_DIR)/ys_ptui.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ptui.c -o \
	$(OUT_DIR)/ys_ptui.$(OBJ)

$(OUT_DIR)/ys_ptmi.$(OBJ): $(IN_DIR)/ys_ptmi.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ptmi.c -o \
	$(OUT_DIR)/ys_ptmi.$(OBJ)

$(OUT_DIR)/ys_utl.$(OBJ): $(IN_DIR)/ys_utl.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_utl.c -o \
	$(OUT_DIR)/ys_utl.$(OBJ)

$(OUT_DIR)/ys_init.$(OBJ): $(IN_DIR)/ys_init.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_init.c -o \
	$(OUT_DIR)/ys_init.$(OBJ)

$(OUT_DIR)/ys_mi.$(OBJ): $(IN_DIR)/ys_mi.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_mi.c -o \
	$(OUT_DIR)/ys_mi.$(OBJ)

$(OUT_DIR)/ys_db.$(OBJ): $(IN_DIR)/ys_db.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_db.c -o \
	$(OUT_DIR)/ys_db.$(OBJ)

$(OUT_DIR)/ys_ms_utl.$(OBJ): $(IN_DIR)/ys_ms_utl.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_utl.c -o \
	$(OUT_DIR)/ys_ms_utl.$(OBJ)

$(OUT_DIR)/ys_ms_cfg.$(OBJ): $(IN_DIR)/ys_ms_cfg.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_cfg.c -o \
	$(OUT_DIR)/ys_ms_cfg.$(OBJ)

$(OUT_DIR)/ys_ms_ui.$(OBJ): $(IN_DIR)/ys_ms_ui.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_ui.c -o \
	$(OUT_DIR)/ys_ms_ui.$(OBJ)

$(OUT_DIR)/ys_ms_li.$(OBJ): $(IN_DIR)/ys_ms_li.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_li.c -o \
	$(OUT_DIR)/ys_ms_li.$(OBJ)

$(OUT_DIR)/ys_ms_map.$(OBJ): $(IN_DIR)/ys_ms_map.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_map.c -o \
	$(OUT_DIR)/ys_ms_map.$(OBJ)

$(OUT_DIR)/ys_ms_ul.$(OBJ): $(IN_DIR)/ys_ms_ul.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_ul.c -o \
	$(OUT_DIR)/ys_ms_ul.$(OBJ)

$(OUT_DIR)/ys_ms_dl.$(OBJ): $(IN_DIR)/ys_ms_dl.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_dl.c -o \
	$(OUT_DIR)/ys_ms_dl.$(OBJ)

$(OUT_DIR)/ys_ms_pal.$(OBJ): $(IN_DIR)/ys_ms_pal.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_pal.c -o \
	$(OUT_DIR)/ys_ms_pal.$(OBJ)

$(OUT_DIR)/ys_ms_stub.$(OBJ): $(IN_DIR)/ys_ms_stub.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ms_stub.c -o \
	$(OUT_DIR)/ys_ms_stub.$(OBJ)

$(OUT_DIR)/ys_dft.$(OBJ): $(IN_DIR)/ys_dft.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_dft.c -o \
	$(OUT_DIR)/ys_dft.$(OBJ)

$(OUT_DIR)/ys_ex_ms.$(OBJ): $(IN_DIR)/ys_ex_ms.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ex_ms.c -o \
	$(OUT_DIR)/ys_ex_ms.$(OBJ)

$(OUT_DIR)/ys_id.$(OBJ): $(IN_DIR)/ys_id.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_id.c -o \
	$(OUT_DIR)/ys_id.$(OBJ)

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/liblnxys.a:$(ysOBJS) $(ysStubOBJS)
	$(AR) -cr $(OUT_DIR)/libtf.a $(ysStubOBJS) $(ysOBJS) 

$(OUT_DIR)/liblnxysms.a:$(ysOBJS)
	$(AR) -cr $(OUT_DIR)/libtf.a $(ysOBJS) 

$(OUT_DIR)/libcavys.a:$(ysOBJS)
	$(AR) -cr $(OUT_DIR)/libtf.a $(ysOBJS) 

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libtf.a $(tfOBJS)


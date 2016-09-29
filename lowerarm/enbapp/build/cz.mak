#-------------------------------------------------------------#
#Makefile for product CZ - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

#product specific options should be identified.
CCczFLAGS=-DLCLCZ -DLCSCT -DCZ -DSM 

# Product sources-------------------------------------------------------
czSRCS= \
    $(IN_DIR)/cz_db.$(SRC) \
    $(IN_DIR)/cz_lim.$(SRC) \
    $(IN_DIR)/cz_nmm.$(SRC) \
    $(IN_DIR)/cz_ex_ms.$(SRC) \
    $(IN_DIR)/cz_id.$(SRC) \
    $(IN_DIR)/cz_lmm.$(SRC) \
    $(IN_DIR)/cz_ptli.$(SRC) \
    $(IN_DIR)/cz_ptmi.$(SRC) \
    $(IN_DIR)/cz_ptui.$(SRC) \
    $(IN_DIR)/cz_tmr.$(SRC) \
    $(IN_DIR)/cz_uim.$(SRC) \
    $(IN_DIR)/cz_utl.$(SRC) \
    $(IN_DIR)/cz_tpm.$(SRC) \
    $(IN_DIR)/cz_bmp.$(SRC) \
    $(IN_DIR)/cz_edm.$(SRC) \
    $(IN_DIR)/cz_gpm.$(SRC) \
    $(IN_DIR)/cz_ldm.$(SRC) \


# Product objects-------------------------------------------------------
czOBJS= \
    $(OUT_DIR)/cz_db.$(OBJ) \
    $(OUT_DIR)/cz_lim.$(OBJ) \
    $(OUT_DIR)/cz_nmm.$(OBJ) \
    $(OUT_DIR)/cz_ex_ms.$(OBJ) \
    $(OUT_DIR)/cz_id.$(OBJ) \
    $(OUT_DIR)/cz_lmm.$(OBJ) \
    $(OUT_DIR)/cz_ptli.$(OBJ) \
    $(OUT_DIR)/cz_ptmi.$(OBJ) \
    $(OUT_DIR)/cz_ptui.$(OBJ) \
    $(OUT_DIR)/cz_tmr.$(OBJ) \
    $(OUT_DIR)/cz_uim.$(OBJ) \
    $(OUT_DIR)/cz_utl.$(OBJ) \
    $(OUT_DIR)/cz_tpm.$(OBJ) \
    $(OUT_DIR)/cz_bmp.$(OBJ) \
    $(OUT_DIR)/cz_edm.$(OBJ) \
    $(OUT_DIR)/cz_gpm.$(OBJ) \
    $(OUT_DIR)/cz_ldm.$(OBJ) 


CZCM_INC=$(VS_DIR)/cz.h $(VS_DIR)/cz.x $(VS_DIR)/cz_err.h

ALL_INC=$(CZCM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/cz_db.$(OBJ): $(IN_DIR)/cz_db.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/cz_db.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) \
	$(IN_DIR)/cz_db.$(SRC)
$(OUT_DIR)/cz_lim.$(OBJ): $(IN_DIR)/cz_lim.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_lim.$(SRC) -o \
	$(OUT_DIR)/cz_lim.$(OBJ)
$(OUT_DIR)/cz_nmm.$(OBJ): $(IN_DIR)/cz_nmm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_nmm.$(SRC) -o \
	$(OUT_DIR)/cz_nmm.$(OBJ)
$(OUT_DIR)/cz_ex_ms.$(OBJ): $(IN_DIR)/cz_ex_ms.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_ex_ms.$(SRC) -o \
	$(OUT_DIR)/cz_ex_ms.$(OBJ)
$(OUT_DIR)/cz_id.$(OBJ): $(IN_DIR)/cz_id.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_id.$(SRC) -o \
	$(OUT_DIR)/cz_id.$(OBJ)
$(OUT_DIR)/cz_lmm.$(OBJ): $(IN_DIR)/cz_lmm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_lmm.$(SRC) -o \
	$(OUT_DIR)/cz_lmm.$(OBJ)
$(OUT_DIR)/cz_ptli.$(OBJ): $(IN_DIR)/cz_ptli.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_ptli.$(SRC) -o \
	$(OUT_DIR)/cz_ptli.$(OBJ)
$(OUT_DIR)/cz_ptmi.$(OBJ): $(IN_DIR)/cz_ptmi.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_ptmi.$(SRC) -o \
	$(OUT_DIR)/cz_ptmi.$(OBJ)
$(OUT_DIR)/cz_ptui.$(OBJ): $(IN_DIR)/cz_ptui.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_ptui.$(SRC) -o \
	$(OUT_DIR)/cz_ptui.$(OBJ)
$(OUT_DIR)/cz_tmr.$(OBJ): $(IN_DIR)/cz_tmr.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_tmr.$(SRC) -o \
	$(OUT_DIR)/cz_tmr.$(OBJ)
$(OUT_DIR)/cz_uim.$(OBJ): $(IN_DIR)/cz_uim.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_uim.$(SRC) -o \
	$(OUT_DIR)/cz_uim.$(OBJ)
$(OUT_DIR)/cz_utl.$(OBJ): $(IN_DIR)/cz_utl.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_utl.$(SRC) -o \
	$(OUT_DIR)/cz_utl.$(OBJ)
$(OUT_DIR)/cz_tpm.$(OBJ): $(IN_DIR)/cz_tpm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_tpm.$(SRC) -o \
	$(OUT_DIR)/cz_tpm.$(OBJ)
$(OUT_DIR)/cz_bmp.$(OBJ): $(IN_DIR)/cz_bmp.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_bmp.$(SRC) -o \
	$(OUT_DIR)/cz_bmp.$(OBJ)
$(OUT_DIR)/cz_edm.$(OBJ): $(IN_DIR)/cz_edm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_edm.$(SRC) -o \
	$(OUT_DIR)/cz_edm.$(OBJ)
$(OUT_DIR)/cz_gpm.$(OBJ): $(IN_DIR)/cz_gpm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_gpm.$(SRC) -o \
	$(OUT_DIR)/cz_gpm.$(OBJ)
$(OUT_DIR)/cz_ldm.$(OBJ): $(IN_DIR)/cz_ldm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_ldm.$(SRC) -o \
	$(OUT_DIR)/cz_ldm.$(OBJ)

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libcz.a:$(czOBJS)
	$(AR) -cr $(OUT_DIR)/libcz.a $(czOBJS)	

$(OUT_DIR)/czperpro:$(czOBJS)

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libcz.a $(czOBJS)


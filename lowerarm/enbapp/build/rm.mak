#-------------------------------------------------------------#
#Makefile for RRM
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#
CCrmFLAGS= -DSS_MT_TMR

# Product sources-------------------------------------------------------
rmSRCS= \
    $(IN_DIR)/rm_tmr.$(SRC) \
    $(IN_DIR)/rm_init.$(SRC) \
    $(IN_DIR)/rm_cac.$(SRC) \
    $(IN_DIR)/rm_cac_acb.$(SRC) \
    $(IN_DIR)/rm_cac_adm.$(SRC) \
    $(IN_DIR)/rm_cac_util.$(SRC) \
    $(IN_DIR)/rm_ccm.$(SRC) \
    $(IN_DIR)/rm_ccm_cell.$(SRC) \
    $(IN_DIR)/rm_ccm_enb.$(SRC) \
    $(IN_DIR)/rm_ccm_ue.$(SRC) \
    $(IN_DIR)/rm_rbc.$(SRC) \
    $(IN_DIR)/rm_rcm.$(SRC) \
    $(IN_DIR)/rm_rcm_pcqi.$(SRC) \
    $(IN_DIR)/rm_util.$(SRC)


# Product objects-------------------------------------------------------
rmOBJS= \
    $(OUT_DIR)/rm_tmr.$(OBJ) \
    $(OUT_DIR)/rm_init.$(OBJ) \
    $(OUT_DIR)/rm_cac.$(OBJ) \
    $(OUT_DIR)/rm_cac_acb.$(OBJ) \
    $(OUT_DIR)/rm_cac_adm.$(OBJ) \
    $(OUT_DIR)/rm_cac_util.$(OBJ) \
    $(OUT_DIR)/rm_ccm.$(OBJ) \
    $(OUT_DIR)/rm_ccm_cell.$(OBJ) \
    $(OUT_DIR)/rm_ccm_enb.$(OBJ) \
    $(OUT_DIR)/rm_ccm_ue.$(OBJ) \
    $(OUT_DIR)/rm_rbc.$(OBJ) \
    $(OUT_DIR)/rm_rcm.$(OBJ) \
    $(OUT_DIR)/rm_rcm_pcqi.$(OBJ) \
    $(OUT_DIR)/rm_util.$(OBJ)


RM_INC=$(VS_DIR)/rm.h $(VS_DIR)/rm_wr.h $(VS_DIR)/rm_rbc.h $(VS_DIR)/rm_rcm.h $(VS_DIR)/rm_cac.h $(VS_DIR)/rm_tmr.h

ALL_INC=$(RM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/rm_tmr.$(OBJ): $(IN_DIR)/rm_tmr.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/rm_tmr.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) \
	$(IN_DIR)/rm_tmr.$(SRC)
$(OUT_DIR)/rm_init.$(OBJ): $(IN_DIR)/rm_init.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/rm_init.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) \
	$(IN_DIR)/rm_init.$(SRC)
$(OUT_DIR)/rm_cac.$(OBJ): $(IN_DIR)/rm_cac.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/rm_cac.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) \
	$(IN_DIR)/rm_cac.$(SRC)
$(OUT_DIR)/rm_cac_acb.$(OBJ): $(IN_DIR)/rm_cac_acb.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_cac_acb.$(SRC) -o \
	$(OUT_DIR)/rm_cac_acb.$(OBJ)
$(OUT_DIR)/rm_cac_adm.$(OBJ): $(IN_DIR)/rm_cac_adm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_cac_adm.$(SRC) -o \
	$(OUT_DIR)/rm_cac_adm.$(OBJ)
$(OUT_DIR)/rm_cac_util.$(OBJ): $(IN_DIR)/rm_cac_util.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_cac_util.$(SRC) -o \
	$(OUT_DIR)/rm_cac_util.$(OBJ)
$(OUT_DIR)/rm_ccm.$(OBJ): $(IN_DIR)/rm_ccm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_ccm.$(SRC) -o \
	$(OUT_DIR)/rm_ccm.$(OBJ)
$(OUT_DIR)/rm_ccm_cell.$(OBJ): $(IN_DIR)/rm_ccm_cell.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_ccm_cell.$(SRC) -o \
	$(OUT_DIR)/rm_ccm_cell.$(OBJ)
$(OUT_DIR)/rm_ccm_enb.$(OBJ): $(IN_DIR)/rm_ccm_enb.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_ccm_enb.$(SRC) -o \
	$(OUT_DIR)/rm_ccm_enb.$(OBJ)
$(OUT_DIR)/rm_ccm_ue.$(OBJ): $(IN_DIR)/rm_ccm_ue.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_ccm_ue.$(SRC) -o \
	$(OUT_DIR)/rm_ccm_ue.$(OBJ)
$(OUT_DIR)/rm_rbc.$(OBJ): $(IN_DIR)/rm_rbc.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_rbc.$(SRC) -o \
	$(OUT_DIR)/rm_rbc.$(OBJ)
$(OUT_DIR)/rm_rcm.$(OBJ): $(IN_DIR)/rm_rcm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_rcm.$(SRC) -o \
	$(OUT_DIR)/rm_rcm.$(OBJ)
$(OUT_DIR)/rm_rcm_pcqi.$(OBJ): $(IN_DIR)/rm_rcm_pcqi.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_rcm_pcqi.$(SRC) -o \
	$(OUT_DIR)/rm_rcm_pcqi.$(OBJ)
$(OUT_DIR)/rm_util.$(OBJ): $(IN_DIR)/rm_util.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_util.$(SRC) -o \
	$(OUT_DIR)/rm_util.$(OBJ)
#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/librm.a:$(rmOBJS)
	$(AR) -cr $(OUT_DIR)/librm.a $(rmOBJS)	

$(OUT_DIR)/rmperpro:$(rmOBJS)

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/librm.a $(rmOBJS)


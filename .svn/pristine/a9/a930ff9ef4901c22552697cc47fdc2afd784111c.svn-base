#-------------------------------------------------------------#
#Makefile for product RG - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#
# Including RG_PHASE2_SCHED for supporting more than one schedulers 
# supported by mac
CCrgFLAGS=-DTFU_VER_2 -UTFU_TDD -USM -ULTE_TDD -DTF -DSS_FLOAT -DSS_MT_TMR \
           -URG_DEBUG -DxRG_PHASE2_SCHED -DxRGR_V1 -DxRG_UL_DELTA=2  -UTFU_UPGRADE 

# Product sources-------------------------------------------------------
rgSRCS= \
    $(IN_DIR)/rg_cfg.$(SRC) \
    $(IN_DIR)/rg_com.$(SRC) \
    $(IN_DIR)/rg_dbm.$(SRC) \
    $(IN_DIR)/rg_dhm.$(SRC) \
    $(IN_DIR)/rg_dux.$(SRC) \
    $(IN_DIR)/rg_ex_ms.$(SRC) \
    $(IN_DIR)/rg_id.$(SRC) \
    $(IN_DIR)/rg_lim.$(SRC) \
    $(IN_DIR)/rg_lmm.$(SRC) \
    $(IN_DIR)/rg_mux.$(SRC) \
    $(IN_DIR)/rg_ptli.$(SRC) \
    $(IN_DIR)/rg_ptmi.$(SRC) \
    $(IN_DIR)/rg_ptui.$(SRC) \
    $(IN_DIR)/rg_ram.$(SRC) \
    $(IN_DIR)/rg_rom.$(SRC) \
    $(IN_DIR)/rg_l2m.$(SRC) \
    $(IN_DIR)/rg_sch.$(SRC) \
    $(IN_DIR)/rg_sch_l2m.$(SRC) \
    $(IN_DIR)/rg_tom.$(SRC) \
    $(IN_DIR)/rg_uhm.$(SRC) \
    $(IN_DIR)/rg_uim.$(SRC) \
    $(IN_DIR)/rg_utl.$(SRC) \
    $(IN_DIR)/rg_sch_cfg.$(SRC) \
    $(IN_DIR)/rg_sch_cmn.$(SRC) \
    $(IN_DIR)/rg_sch_dbm.$(SRC) \
    $(IN_DIR)/rg_sch_gom.$(SRC) \
    $(IN_DIR)/rg_sch_sc1.$(SRC) \
    $(IN_DIR)/rg_sch_dhm.$(SRC) \
    $(IN_DIR)/rg_sch_dlfs.$(SRC) \
    $(IN_DIR)/rg_sch_ex_ms.$(SRC) \
    $(IN_DIR)/rg_sch_inf.$(SRC) \
    $(IN_DIR)/rg_sch_lmm.$(SRC) \
    $(IN_DIR)/rg_sch_maxci.$(SRC) \
    $(IN_DIR)/rg_sch_mga.$(SRC) \
    $(IN_DIR)/rg_sch_pfs.$(SRC) \
    $(IN_DIR)/rg_sch_pt.$(SRC) \
    $(IN_DIR)/rg_sch_pwr.$(SRC) \
    $(IN_DIR)/rg_sch_ram.$(SRC) \
    $(IN_DIR)/rg_sch_rr.$(SRC) \
    $(IN_DIR)/rg_sch_tmr.$(SRC) \
    $(IN_DIR)/rg_sch_uhm.$(SRC) \
    $(IN_DIR)/rg_sch_utl.$(SRC) \
    $(IN_DIR)/rg_sch_tom.$(SRC) \
    $(IN_DIR)/rg_sch_utl_clist.$(SRC)\
    $(IN_DIR)/rg_sch_drx.$(SRC) 



# Product objects-------------------------------------------------------
rgOBJS= \
    $(OUT_DIR)/rg_cfg.$(OBJ) \
    $(OUT_DIR)/rg_com.$(OBJ) \
    $(OUT_DIR)/rg_dbm.$(OBJ) \
    $(OUT_DIR)/rg_dhm.$(OBJ) \
    $(OUT_DIR)/rg_dux.$(OBJ) \
    $(OUT_DIR)/rg_ex_ms.$(OBJ) \
    $(OUT_DIR)/rg_id.$(OBJ) \
    $(OUT_DIR)/rg_lim.$(OBJ) \
    $(OUT_DIR)/rg_lmm.$(OBJ) \
    $(OUT_DIR)/rg_mux.$(OBJ) \
    $(OUT_DIR)/rg_ptli.$(OBJ) \
    $(OUT_DIR)/rg_ptmi.$(OBJ) \
    $(OUT_DIR)/rg_ptui.$(OBJ) \
    $(OUT_DIR)/rg_ram.$(OBJ) \
    $(OUT_DIR)/rg_rom.$(OBJ) \
    $(OUT_DIR)/rg_l2m.$(OBJ) \
    $(OUT_DIR)/rg_sch.$(OBJ) \
    $(OUT_DIR)/rg_sch_l2m.$(OBJ) \
    $(OUT_DIR)/rg_tom.$(OBJ) \
    $(OUT_DIR)/rg_uhm.$(OBJ) \
    $(OUT_DIR)/rg_uim.$(OBJ) \
    $(OUT_DIR)/rg_utl.$(OBJ)  \
    $(OUT_DIR)/rg_sch_cfg.$(OBJ) \
    $(OUT_DIR)/rg_sch_cmn.$(OBJ) \
    $(OUT_DIR)/rg_sch_dbm.$(OBJ) \
    $(OUT_DIR)/rg_sch_gom.$(OBJ)\
    $(OUT_DIR)/rg_sch_sc1.$(OBJ)\
    $(OUT_DIR)/rg_sch_dhm.$(OBJ)\
    $(OUT_DIR)/rg_sch_dlfs.$(OBJ) \
    $(OUT_DIR)/rg_sch_ex_ms.$(OBJ) \
    $(OUT_DIR)/rg_sch_inf.$(OBJ) \
    $(OUT_DIR)/rg_sch_lmm.$(OBJ) \
    $(OUT_DIR)/rg_sch_maxci.$(OBJ) \
    $(OUT_DIR)/rg_sch_mga.$(OBJ) \
    $(OUT_DIR)/rg_sch_pfs.$(OBJ) \
    $(OUT_DIR)/rg_sch_pt.$(OBJ) \
    $(OUT_DIR)/rg_sch_pwr.$(OBJ) \
    $(OUT_DIR)/rg_sch_ram.$(OBJ) \
    $(OUT_DIR)/rg_sch_rr.$(OBJ) \
    $(OUT_DIR)/rg_sch_tmr.$(OBJ) \
    $(OUT_DIR)/rg_sch_uhm.$(OBJ) \
    $(OUT_DIR)/rg_sch_utl.$(OBJ) \
    $(OUT_DIR)/rg_sch_tom.$(OBJ) \
    $(OUT_DIR)/rg_sch_utl_clist.$(OBJ) \
    $(OUT_DIR)/rg_sch_drx.$(OBJ)




RGCM_INC=$(VS_DIR)/rg.h $(VS_DIR)/rg.x $(VS_DIR)/rg_err.h $(VS_DIR)/rg_env.h

ALL_INC=$(RGCM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/rg_cfg.$(OBJ): $(IN_DIR)/rg_cfg.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/rg_cfg.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) \
	$(IN_DIR)/rg_cfg.$(SRC)
$(OUT_DIR)/rg_com.$(OBJ): $(IN_DIR)/rg_com.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_com.$(SRC) -o \
	$(OUT_DIR)/rg_com.$(OBJ)
$(OUT_DIR)/rg_dbm.$(OBJ): $(IN_DIR)/rg_dbm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_dbm.$(SRC) -o \
	$(OUT_DIR)/rg_dbm.$(OBJ)
$(OUT_DIR)/rg_dhm.$(OBJ): $(IN_DIR)/rg_dhm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_dhm.$(SRC) -o \
	$(OUT_DIR)/rg_dhm.$(OBJ)
$(OUT_DIR)/rg_dux.$(OBJ): $(IN_DIR)/rg_dux.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_dux.$(SRC) -o \
	$(OUT_DIR)/rg_dux.$(OBJ)
$(OUT_DIR)/rg_ex_ms.$(OBJ): $(IN_DIR)/rg_ex_ms.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_ex_ms.$(SRC) -o \
	$(OUT_DIR)/rg_ex_ms.$(OBJ)
$(OUT_DIR)/rg_id.$(OBJ): $(IN_DIR)/rg_id.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_id.$(SRC) -o \
	$(OUT_DIR)/rg_id.$(OBJ)
$(OUT_DIR)/rg_lim.$(OBJ): $(IN_DIR)/rg_lim.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_lim.$(SRC) -o \
	$(OUT_DIR)/rg_lim.$(OBJ)
$(OUT_DIR)/rg_lmm.$(OBJ): $(IN_DIR)/rg_lmm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_lmm.$(SRC) -o \
	$(OUT_DIR)/rg_lmm.$(OBJ)
$(OUT_DIR)/rg_mux.$(OBJ): $(IN_DIR)/rg_mux.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_mux.$(SRC) -o \
	$(OUT_DIR)/rg_mux.$(OBJ)
$(OUT_DIR)/rg_ptli.$(OBJ): $(IN_DIR)/rg_ptli.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_ptli.$(SRC) -o \
	$(OUT_DIR)/rg_ptli.$(OBJ)
$(OUT_DIR)/rg_ptmi.$(OBJ): $(IN_DIR)/rg_ptmi.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_ptmi.$(SRC) -o \
	$(OUT_DIR)/rg_ptmi.$(OBJ)
$(OUT_DIR)/rg_ptui.$(OBJ): $(IN_DIR)/rg_ptui.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_ptui.$(SRC) -o \
	$(OUT_DIR)/rg_ptui.$(OBJ)
$(OUT_DIR)/rg_ram.$(OBJ): $(IN_DIR)/rg_ram.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_ram.$(SRC) -o \
	$(OUT_DIR)/rg_ram.$(OBJ)
$(OUT_DIR)/rg_rom.$(OBJ): $(IN_DIR)/rg_rom.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_rom.$(SRC) -o \
	$(OUT_DIR)/rg_rom.$(OBJ)
$(OUT_DIR)/rg_l2m.$(OBJ): $(IN_DIR)/rg_l2m.$(SRC) $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_l2m.$(SRC) -o \
	$(OUT_DIR)/rg_l2m.$(OBJ)
$(OUT_DIR)/rg_sch.$(OBJ): $(IN_DIR)/rg_sch.$(SRC) $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch.$(SRC) -o \
	$(OUT_DIR)/rg_sch.$(OBJ)
$(OUT_DIR)/rg_sch_l2m.$(OBJ): $(IN_DIR)/rg_sch_l2m.$(SRC) $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_l2m.$(SRC) -o \
	$(OUT_DIR)/rg_sch_l2m.$(OBJ)
$(OUT_DIR)/rg_tom.$(OBJ): $(IN_DIR)/rg_tom.$(SRC) $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_tom.$(SRC) -o \
	$(OUT_DIR)/rg_tom.$(OBJ)
$(OUT_DIR)/rg_uhm.$(OBJ): $(IN_DIR)/rg_uhm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_uhm.$(SRC) -o \
	$(OUT_DIR)/rg_uhm.$(OBJ)
$(OUT_DIR)/rg_uim.$(OBJ): $(IN_DIR)/rg_uim.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_uim.$(SRC) -o \
	$(OUT_DIR)/rg_uim.$(OBJ)
$(OUT_DIR)/rg_utl.$(OBJ): $(IN_DIR)/rg_utl.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_utl.$(SRC) -o \
	$(OUT_DIR)/rg_utl.$(OBJ)

$(OUT_DIR)/rg_sch_sc1.$(OBJ): $(IN_DIR)/rg_sch_sc1.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_sc1.$(SRC) -o \
	$(OUT_DIR)/rg_sch_sc1.$(OBJ)
$(OUT_DIR)/rg_sch_gom.$(OBJ): $(IN_DIR)/rg_sch_gom.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_gom.$(SRC) -o \
	$(OUT_DIR)/rg_sch_gom.$(OBJ)
$(OUT_DIR)/rg_sch_inf.$(OBJ): $(IN_DIR)/rg_sch_inf.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_inf.$(SRC) -o \
	$(OUT_DIR)/rg_sch_inf.$(OBJ)
$(OUT_DIR)/rg_sch_lmm.$(OBJ): $(IN_DIR)/rg_sch_lmm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_lmm.$(SRC) -o \
	$(OUT_DIR)/rg_sch_lmm.$(OBJ)
$(OUT_DIR)/rg_sch_cfg.$(OBJ): $(IN_DIR)/rg_sch_cfg.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_cfg.$(SRC) -o \
	$(OUT_DIR)/rg_sch_cfg.$(OBJ)
$(OUT_DIR)/rg_sch_cmn.$(OBJ): $(IN_DIR)/rg_sch_cmn.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_cmn.$(SRC) -o \
	$(OUT_DIR)/rg_sch_cmn.$(OBJ)
$(OUT_DIR)/rg_sch_dbm.$(OBJ): $(IN_DIR)/rg_sch_dbm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_dbm.$(SRC) -o \
	$(OUT_DIR)/rg_sch_dbm.$(OBJ)
$(OUT_DIR)/rg_sch_dhm.$(OBJ): $(IN_DIR)/rg_sch_dhm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_dhm.$(SRC) -o \
	$(OUT_DIR)/rg_sch_dhm.$(OBJ)
$(OUT_DIR)/rg_sch_dlfs.$(OBJ): $(IN_DIR)/rg_sch_dlfs.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_dlfs.$(SRC) -o \
	$(OUT_DIR)/rg_sch_dlfs.$(OBJ)
$(OUT_DIR)/rg_sch_ex_ms.$(OBJ): $(IN_DIR)/rg_sch_ex_ms.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_ex_ms.$(SRC) -o \
	$(OUT_DIR)/rg_sch_ex_ms.$(OBJ)

$(OUT_DIR)/rg_sch_maxci.$(OBJ): $(IN_DIR)/rg_sch_maxci.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_maxci.$(SRC) -o \
	$(OUT_DIR)/rg_sch_maxci.$(OBJ)
$(OUT_DIR)/rg_sch_mga.$(OBJ): $(IN_DIR)/rg_sch_mga.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_mga.$(SRC) -o \
	$(OUT_DIR)/rg_sch_mga.$(OBJ)
$(OUT_DIR)/rg_sch_pfs.$(OBJ): $(IN_DIR)/rg_sch_pfs.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_pfs.$(SRC) -o \
	$(OUT_DIR)/rg_sch_pfs.$(OBJ)
$(OUT_DIR)/rg_sch_pt.$(OBJ): $(IN_DIR)/rg_sch_pt.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_pt.$(SRC) -o \
	$(OUT_DIR)/rg_sch_pt.$(OBJ)
$(OUT_DIR)/rg_sch_pwr.$(OBJ): $(IN_DIR)/rg_sch_pwr.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_pwr.$(SRC) -o \
	$(OUT_DIR)/rg_sch_pwr.$(OBJ)
$(OUT_DIR)/rg_sch_ram.$(OBJ): $(IN_DIR)/rg_sch_ram.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_ram.$(SRC) -o \
	$(OUT_DIR)/rg_sch_ram.$(OBJ)
$(OUT_DIR)/rg_sch_rr.$(OBJ): $(IN_DIR)/rg_sch_rr.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_rr.$(SRC) -o \
	$(OUT_DIR)/rg_sch_rr.$(OBJ)
$(OUT_DIR)/rg_sch_tmr.$(OBJ): $(IN_DIR)/rg_sch_tmr.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_tmr.$(SRC) -o \
	$(OUT_DIR)/rg_sch_tmr.$(OBJ)
$(OUT_DIR)/rg_sch_tom.$(OBJ): $(IN_DIR)/rg_sch_tom.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_tom.$(SRC) -o \
	$(OUT_DIR)/rg_sch_tom.$(OBJ)
$(OUT_DIR)/rg_sch_uhm.$(OBJ): $(IN_DIR)/rg_sch_uhm.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_uhm.$(SRC) -o \
	$(OUT_DIR)/rg_sch_uhm.$(OBJ)
$(OUT_DIR)/rg_sch_utl.$(OBJ): $(IN_DIR)/rg_sch_utl.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_utl.$(SRC) -o \
	$(OUT_DIR)/rg_sch_utl.$(OBJ)
$(OUT_DIR)/rg_sch_utl_clist.$(OBJ): $(IN_DIR)/rg_sch_utl_clist.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_utl_clist.$(SRC) -o \
	$(OUT_DIR)/rg_sch_utl_clist.$(OBJ)
$(OUT_DIR)/rg_sch_drx.$(OBJ): $(IN_DIR)/rg_sch_drx.$(SRC) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_drx.$(SRC) -o \
	$(OUT_DIR)/rg_sch_drx.$(OBJ)
#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/librg.a:$(rgOBJS)
	$(AR) -cr $(OUT_DIR)/librg.a $(rgOBJS)	

$(OUT_DIR)/rgperpro:$(rgOBJS)

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/librg.a $(rgOBJS)


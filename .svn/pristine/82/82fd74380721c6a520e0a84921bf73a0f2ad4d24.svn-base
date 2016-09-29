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
    $(IN_DIR)/rg_cfg.c \
    $(IN_DIR)/rg_com.c \
    $(IN_DIR)/rg_dbm.c \
    $(IN_DIR)/rg_dhm.c \
    $(IN_DIR)/rg_dux.c \
    $(IN_DIR)/rg_ex_ms.c \
    $(IN_DIR)/rg_id.c \
    $(IN_DIR)/rg_lim.c \
    $(IN_DIR)/rg_lmm.c \
    $(IN_DIR)/rg_mux.c \
    $(IN_DIR)/rg_ptli.c \
    $(IN_DIR)/rg_ptmi.c \
    $(IN_DIR)/rg_ptui.c \
    $(IN_DIR)/rg_ram.c \
    $(IN_DIR)/rg_rom.c \
    $(IN_DIR)/rg_sch.c \
    $(IN_DIR)/rg_tom.c \
    $(IN_DIR)/rg_uhm.c \
    $(IN_DIR)/rg_uim.c \
    $(IN_DIR)/rg_utl.c \
    $(IN_DIR)/rg_sch_cfg.c \
    $(IN_DIR)/rg_sch_cmn.c \
    $(IN_DIR)/rg_sch_dbm.c \
    $(IN_DIR)/rg_sch_gom.c \
    $(IN_DIR)/rg_sch_sc1.c \
    $(IN_DIR)/rg_sch_dhm.c \
    $(IN_DIR)/rg_sch_dlfs.c \
    $(IN_DIR)/rg_sch_ex_ms.c \
    $(IN_DIR)/rg_sch_inf.c \
    $(IN_DIR)/rg_sch_lmm.c \
    $(IN_DIR)/rg_sch_maxci.c \
    $(IN_DIR)/rg_sch_mga.c \
    $(IN_DIR)/rg_sch_pfs.c \
    $(IN_DIR)/rg_sch_pt.c \
    $(IN_DIR)/rg_sch_pwr.c \
    $(IN_DIR)/rg_sch_ram.c \
    $(IN_DIR)/rg_sch_rr.c \
    $(IN_DIR)/rg_sch_tmr.c \
    $(IN_DIR)/rg_sch_uhm.c \
    $(IN_DIR)/rg_sch_utl.c \
    $(IN_DIR)/rg_sch_tom.c \
    $(IN_DIR)/rg_sch_utl_clist.c 



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
    $(OUT_DIR)/rg_sch.$(OBJ) \
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
    $(OUT_DIR)/rg_sch_utl_clist.$(OBJ)




RGCM_INC=$(IN_DIR)/rg.h $(IN_DIR)/rg.x $(IN_DIR)/rg_err.h $(IN_DIR)/rg_env.h

ALL_INC=$(RGCM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/rg_cfg.$(OBJ): $(IN_DIR)/rg_cfg.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/rg_cfg.o $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) \
	$(IN_DIR)/rg_cfg.c
$(OUT_DIR)/rg_com.$(OBJ): $(IN_DIR)/rg_com.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_com.c -o \
	$(OUT_DIR)/rg_com.$(OBJ)
$(OUT_DIR)/rg_dbm.$(OBJ): $(IN_DIR)/rg_dbm.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_dbm.c -o \
	$(OUT_DIR)/rg_dbm.$(OBJ)
$(OUT_DIR)/rg_dhm.$(OBJ): $(IN_DIR)/rg_dhm.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_dhm.c -o \
	$(OUT_DIR)/rg_dhm.$(OBJ)
$(OUT_DIR)/rg_dux.$(OBJ): $(IN_DIR)/rg_dux.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_dux.c -o \
	$(OUT_DIR)/rg_dux.$(OBJ)
$(OUT_DIR)/rg_ex_ms.$(OBJ): $(IN_DIR)/rg_ex_ms.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_ex_ms.c -o \
	$(OUT_DIR)/rg_ex_ms.$(OBJ)
$(OUT_DIR)/rg_id.$(OBJ): $(IN_DIR)/rg_id.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_id.c -o \
	$(OUT_DIR)/rg_id.$(OBJ)
$(OUT_DIR)/rg_lim.$(OBJ): $(IN_DIR)/rg_lim.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_lim.c -o \
	$(OUT_DIR)/rg_lim.$(OBJ)
$(OUT_DIR)/rg_lmm.$(OBJ): $(IN_DIR)/rg_lmm.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_lmm.c -o \
	$(OUT_DIR)/rg_lmm.$(OBJ)
$(OUT_DIR)/rg_mux.$(OBJ): $(IN_DIR)/rg_mux.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_mux.c -o \
	$(OUT_DIR)/rg_mux.$(OBJ)
$(OUT_DIR)/rg_ptli.$(OBJ): $(IN_DIR)/rg_ptli.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_ptli.c -o \
	$(OUT_DIR)/rg_ptli.$(OBJ)
$(OUT_DIR)/rg_ptmi.$(OBJ): $(IN_DIR)/rg_ptmi.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_ptmi.c -o \
	$(OUT_DIR)/rg_ptmi.$(OBJ)
$(OUT_DIR)/rg_ptui.$(OBJ): $(IN_DIR)/rg_ptui.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_ptui.c -o \
	$(OUT_DIR)/rg_ptui.$(OBJ)
$(OUT_DIR)/rg_ram.$(OBJ): $(IN_DIR)/rg_ram.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_ram.c -o \
	$(OUT_DIR)/rg_ram.$(OBJ)
$(OUT_DIR)/rg_rom.$(OBJ): $(IN_DIR)/rg_rom.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_rom.c -o \
	$(OUT_DIR)/rg_rom.$(OBJ)
$(OUT_DIR)/rg_sch.$(OBJ): $(IN_DIR)/rg_sch.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch.c -o \
	$(OUT_DIR)/rg_sch.$(OBJ)
$(OUT_DIR)/rg_tom.$(OBJ): $(IN_DIR)/rg_tom.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_tom.c -o \
	$(OUT_DIR)/rg_tom.$(OBJ)
$(OUT_DIR)/rg_uhm.$(OBJ): $(IN_DIR)/rg_uhm.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_uhm.c -o \
	$(OUT_DIR)/rg_uhm.$(OBJ)
$(OUT_DIR)/rg_uim.$(OBJ): $(IN_DIR)/rg_uim.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_uim.c -o \
	$(OUT_DIR)/rg_uim.$(OBJ)
$(OUT_DIR)/rg_utl.$(OBJ): $(IN_DIR)/rg_utl.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_utl.c -o \
	$(OUT_DIR)/rg_utl.$(OBJ)

$(OUT_DIR)/rg_sch_sc1.$(OBJ): $(IN_DIR)/rg_sch_sc1.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_sc1.c -o \
	$(OUT_DIR)/rg_sch_sc1.$(OBJ)
$(OUT_DIR)/rg_sch_gom.$(OBJ): $(IN_DIR)/rg_sch_gom.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_gom.c -o \
	$(OUT_DIR)/rg_sch_gom.$(OBJ)
$(OUT_DIR)/rg_sch_inf.$(OBJ): $(IN_DIR)/rg_sch_inf.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_inf.c -o \
	$(OUT_DIR)/rg_sch_inf.$(OBJ)
$(OUT_DIR)/rg_sch_lmm.$(OBJ): $(IN_DIR)/rg_sch_lmm.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_lmm.c -o \
	$(OUT_DIR)/rg_sch_lmm.$(OBJ)
$(OUT_DIR)/rg_sch_cfg.$(OBJ): $(IN_DIR)/rg_sch_cfg.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_cfg.c -o \
	$(OUT_DIR)/rg_sch_cfg.$(OBJ)
$(OUT_DIR)/rg_sch_cmn.$(OBJ): $(IN_DIR)/rg_sch_cmn.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_cmn.c -o \
	$(OUT_DIR)/rg_sch_cmn.$(OBJ)
$(OUT_DIR)/rg_sch_dbm.$(OBJ): $(IN_DIR)/rg_sch_dbm.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_dbm.c -o \
	$(OUT_DIR)/rg_sch_dbm.$(OBJ)
$(OUT_DIR)/rg_sch_dhm.$(OBJ): $(IN_DIR)/rg_sch_dhm.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_dhm.c -o \
	$(OUT_DIR)/rg_sch_dhm.$(OBJ)
$(OUT_DIR)/rg_sch_dlfs.$(OBJ): $(IN_DIR)/rg_sch_dlfs.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_dlfs.c -o \
	$(OUT_DIR)/rg_sch_dlfs.$(OBJ)
$(OUT_DIR)/rg_sch_ex_ms.$(OBJ): $(IN_DIR)/rg_sch_ex_ms.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_ex_ms.c -o \
	$(OUT_DIR)/rg_sch_ex_ms.$(OBJ)

$(OUT_DIR)/rg_sch_maxci.$(OBJ): $(IN_DIR)/rg_sch_maxci.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_maxci.c -o \
	$(OUT_DIR)/rg_sch_maxci.$(OBJ)
$(OUT_DIR)/rg_sch_mga.$(OBJ): $(IN_DIR)/rg_sch_mga.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_mga.c -o \
	$(OUT_DIR)/rg_sch_mga.$(OBJ)
$(OUT_DIR)/rg_sch_pfs.$(OBJ): $(IN_DIR)/rg_sch_pfs.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_pfs.c -o \
	$(OUT_DIR)/rg_sch_pfs.$(OBJ)
$(OUT_DIR)/rg_sch_pt.$(OBJ): $(IN_DIR)/rg_sch_pt.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_pt.c -o \
	$(OUT_DIR)/rg_sch_pt.$(OBJ)
$(OUT_DIR)/rg_sch_pwr.$(OBJ): $(IN_DIR)/rg_sch_pwr.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_pwr.c -o \
	$(OUT_DIR)/rg_sch_pwr.$(OBJ)
$(OUT_DIR)/rg_sch_ram.$(OBJ): $(IN_DIR)/rg_sch_ram.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_ram.c -o \
	$(OUT_DIR)/rg_sch_ram.$(OBJ)
$(OUT_DIR)/rg_sch_rr.$(OBJ): $(IN_DIR)/rg_sch_rr.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_rr.c -o \
	$(OUT_DIR)/rg_sch_rr.$(OBJ)
$(OUT_DIR)/rg_sch_tmr.$(OBJ): $(IN_DIR)/rg_sch_tmr.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_tmr.c -o \
	$(OUT_DIR)/rg_sch_tmr.$(OBJ)
$(OUT_DIR)/rg_sch_tom.$(OBJ): $(IN_DIR)/rg_sch_tom.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_tom.c -o \
	$(OUT_DIR)/rg_sch_tom.$(OBJ)
$(OUT_DIR)/rg_sch_uhm.$(OBJ): $(IN_DIR)/rg_sch_uhm.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_uhm.c -o \
	$(OUT_DIR)/rg_sch_uhm.$(OBJ)
$(OUT_DIR)/rg_sch_utl.$(OBJ): $(IN_DIR)/rg_sch_utl.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_utl.c -o \
	$(OUT_DIR)/rg_sch_utl.$(OBJ)
$(OUT_DIR)/rg_sch_utl_clist.$(OBJ): $(IN_DIR)/rg_sch_utl_clist.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $(IN_DIR)/rg_sch_utl_clist.c -o \
	$(OUT_DIR)/rg_sch_utl_clist.$(OBJ)
#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/librg.a:$(rgOBJS)
	$(AR) -cr $(OUT_DIR)/librg.a $(rgOBJS)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/librg.a $(rgOBJS)


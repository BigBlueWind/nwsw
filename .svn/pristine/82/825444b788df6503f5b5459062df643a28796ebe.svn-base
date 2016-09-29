#-------------------------------------------------------------#
#Makefile for product RG - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#
# Including RG_PHASE2_SCHED for supporting more than one schedulers 
# supported by mac
CCrgFLAGS=-DTFU_VER_2 -UTFU_TDD -USM -ULTE_TDD -DTF -DSS_FLOAT -DSS_MT_TMR \
           -URG_DEBUG -DxRG_PHASE2_SCHED  -DxRG_UL_DELTA=2  -UTFU_UPGRADE 
           
ifeq ($(TARGET),4gmx)
CCrgFLAGS += -DxRGR_V1
endif

# Product sources-------------------------------------------------------
MAC_SRC_FILES= \
    rg_cfg.c \
    rg_com.c \
    rg_dbm.c \
    rg_dhm.c \
    rg_dux.c \
    rg_ex_ms.c \
    rg_id.c \
    rg_lim.c \
    rg_lmm.c \
    rg_mux.c \
    rg_ptli.c \
    rg_ptmi.c \
    rg_ptui.c \
    rg_ram.c \
    rg_rom.c \
    rg_sch.c \
    rg_tom.c \
    rg_uhm.c \
    rg_uim.c \
    rg_utl.c \
    rg_sch_cfg.c \
    rg_sch_cmn.c \
    rg_sch_dbm.c \
    rg_sch_gom.c \
    rg_sch_sc1.c \
    rg_sch_dhm.c \
    rg_sch_dlfs.c \
    rg_sch_ex_ms.c \
    rg_sch_inf.c \
    rg_sch_lmm.c \
    rg_sch_maxci.c \
    rg_sch_mga.c \
    rg_sch_pfs.c \
    rg_sch_pt.c \
    rg_sch_pwr.c \
    rg_sch_ram.c \
    rg_sch_rr.c \
    rg_sch_tmr.c \
    rg_sch_uhm.c \
    rg_sch_utl.c \
    rg_sch_tom.c \
    rg_sch_utl_clist.c \
    rg_sch_hdfdd.c \
		rg_sch_l2m.c
    
MAC_OBJ_FILES  := $(patsubst %.c, $(OUT_DIR)/%.o, $(MAC_SRC_FILES) )

RGCM_INC=$(IN_DIR)/rg.h $(IN_DIR)/rg.x $(IN_DIR)/rg_err.h $(IN_DIR)/rg_env.h $(IN_DIR)/rg_sch_hdfdd.h $(IN_DIR)/rg_sch_hdfdd.x $(IN_DIR)/rgac_acc.h $(IN_DIR)/rgac_acc.x $(IN_DIR)/rgac_crg.h $(IN_DIR)/rgac_crg.x $(IN_DIR)/rgac_lrg.h $(IN_DIR)/rgac_lrg.x $(IN_DIR)/rgac_rgr.h $(IN_DIR)/rgac_rgr.x $(IN_DIR)/rgac_rgu.h $(IN_DIR)/rgac_rgu.x

ALL_INC=$(RGCM_INC) $(CM_INC)

BUILD_PREFIX            := "==> Building"
V                       := @
   
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
#MAC ObjectFiles
$(OUT_DIR)/%.o: $(IN_DIR)/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCrgFLAGS) $< -o $@

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/librg.a:$(MAC_OBJ_FILES) $(ALL_INC)
	$(V) $(AR) -cr $(LIB_DIR)/librg.a $(MAC_OBJ_FILES)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	rm -f $(LIB_DIR)/librg.a $(MAC_OBJ_FILES)


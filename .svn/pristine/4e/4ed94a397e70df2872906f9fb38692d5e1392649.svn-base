#-------------------------------------------------------------#
#Makefile for product SZ - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCuehFLAGS= -DEGTP_U -DNX -DUZ -DEU -DKW_PDCP -DDEBUGP  -DTFU_VER_2 -DLCEGUIEGT -DLCUEHLICTF -DLCUEHLINHU -DLCPJUIPJU -DLCUEHLIRGR

uehE2eLnxSRCS= \
    $(IN_DIR)/../sm/sm_cfg_enodeb.c \
    $(IN_DIR)/../sm/sm_nh_cntrl.c \
    $(IN_DIR)/../sm/sm_bsmsm_exms.c \
    $(IN_DIR)/../sm/sm_pmhsm_exms.c \
    $(IN_DIR)/../sm/sm_lthsm_exms.c \
    $(IN_DIR)/../sm/sm_nh_exms.c \
    $(IN_DIR)/../sm/sm_nh_ptmi.c \
    $(IN_DIR)/../sm/sm_kw_cntrl.c \
    $(IN_DIR)/../sm/sm_kw_exms.c \
    $(IN_DIR)/../sm/sm_kw_ptmi.c \
    $(IN_DIR)/../sm/sm_rg_cntrl.c \
    $(IN_DIR)/../sm/sm_rg_exms.c \
    $(IN_DIR)/../sm/sm_rg_ptmi.c \
    $(IN_DIR)/../sm/sm_sz_cntrl.c \
    $(IN_DIR)/../sm/sm_sz_exms.c \
    $(IN_DIR)/../sm/sm_sz_ptmi.c \
    $(IN_DIR)/../sm/sm_sb_cntrl.c \
    $(IN_DIR)/../sm/sm_sb_exms.c \
    $(IN_DIR)/../sm/sm_sb_ptmi.c \
    $(IN_DIR)/../sm/sm_hi_cntrl.c \
    $(IN_DIR)/../sm/sm_hi_exms.c \
    $(IN_DIR)/../sm/sm_hi_ptmi.c \
    $(IN_DIR)/../sm/sm_eg_cntrl.c \
    $(IN_DIR)/../sm/sm_udh_cntrl.c \
    $(IN_DIR)/../sm/sm_eg_exms.c \
    $(IN_DIR)/../sm/sm_eg_ptmi.c \
    $(IN_DIR)/../sm/sm_cntrl.c \
    $(IN_DIR)/../sm/sm_rrm_ex_ms.c \
    $(IN_DIR)/../sm/sm_ptmi.c \
    $(IN_DIR)/../sm/sm_pj_cntrl.c \
    $(IN_DIR)/../sm/sm_pj_exms.c \
    $(IN_DIR)/../sm/sm_pj_ptmi.c \
    $(IN_DIR)/../sm/sm_ys_cntrl.c \
    $(IN_DIR)/../sm/sm_ys_exms.c \
    $(IN_DIR)/../sm/sm_ys_ptmi.c \
    $(IN_DIR)/../sm/sm_udh_exms.c


# Product objects-------------------------------------------------------
uehE2eLnxOBJS= \
    $(OUT_DIR)/sm_cfg_enodeb.$(OBJ) \
    $(OUT_DIR)/sm_nh_cntrl.$(OBJ) \
    $(OUT_DIR)/sm_nh_exms.$(OBJ) \
    $(OUT_DIR)/sm_bsmsm_exms.$(OBJ) \
    $(OUT_DIR)/sm_pmhsm_exms.$(OBJ) \
    $(OUT_DIR)/sm_lthsm_exms.$(OBJ) \
    $(OUT_DIR)/sm_nh_ptmi.$(OBJ) \
    $(OUT_DIR)/sm_kw_cntrl.$(OBJ) \
    $(OUT_DIR)/sm_kw_exms.$(OBJ) \
    $(OUT_DIR)/sm_kw_ptmi.$(OBJ) \
    $(OUT_DIR)/sm_rg_cntrl.$(OBJ) \
    $(OUT_DIR)/sm_rg_exms.$(OBJ) \
    $(OUT_DIR)/sm_rg_ptmi.$(OBJ) \
    $(OUT_DIR)/sm_sz_cntrl.$(OBJ) \
    $(OUT_DIR)/sm_sz_exms.$(OBJ) \
    $(OUT_DIR)/sm_sz_ptmi.$(OBJ) \
    $(OUT_DIR)/sm_sb_cntrl.$(OBJ) \
    $(OUT_DIR)/sm_sb_exms.$(OBJ) \
    $(OUT_DIR)/sm_sb_ptmi.$(OBJ) \
    $(OUT_DIR)/sm_hi_cntrl.$(OBJ) \
    $(OUT_DIR)/sm_hi_exms.$(OBJ) \
    $(OUT_DIR)/sm_hi_ptmi.$(OBJ) \
    $(OUT_DIR)/sm_eg_cntrl.$(OBJ) \
    $(OUT_DIR)/sm_udh_cntrl.$(OBJ) \
    $(OUT_DIR)/sm_eg_exms.$(OBJ) \
    $(OUT_DIR)/sm_eg_ptmi.$(OBJ) \
    $(OUT_DIR)/sm_cntrl.$(OBJ) \
    $(OUT_DIR)/sm_rrm_ex_ms.$(OBJ) \
    $(OUT_DIR)/sm_ptmi.$(OBJ) \
    $(OUT_DIR)/sm_pj_cntrl.$(OBJ) \
    $(OUT_DIR)/sm_pj_exms.$(OBJ) \
    $(OUT_DIR)/sm_pj_ptmi.$(OBJ) \
    $(OUT_DIR)/sm_ys_cntrl.$(OBJ) \
    $(OUT_DIR)/sm_ys_exms.$(OBJ) \
    $(OUT_DIR)/sm_ys_ptmi.$(OBJ)  \
    $(OUT_DIR)/sm_udh_exms.$(OBJ)

UEHCM_INC=$(IN_DIR)/inc/ueh.h $(IN_DIR)/inc/sctp_cfg_table.h $(IN_DIR)/inc/s1ap_cfg_table.h  $(IN_DIR)/inc/ueh_perf.h $(IN_DIR)/../../sfi/sfi.h

ALL_INC=$(UEHCM_INC) $(CM_INC) $(UEH_DIR)

BUILD_PREFIX            := "==> Building"
V                       := #@

UEH_INC_DIR	= -I$(IN_DIR)/../s1mgr/ -I$(IN_DIR)/sec/ -I$(IN_DIR)/../../rrc/ -I$(IN_DIR)/../udh/ -I$(IN_DIR)/../../inc/ -I$(IN_DIR)/../../pseudophy/ -I$(IN_DIR)/../ueh/inc/ 
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/sm_cfg_enodeb.$(OBJ): $(IN_DIR)/../sm/sm_cfg_enodeb.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/sm_cfg_enodeb.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../sm/sm_cfg_enodeb.c
	
$(OUT_DIR)/sm_nh_cntrl.$(OBJ): $(IN_DIR)/../sm/sm_nh_cntrl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/sm_nh_cntrl.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../sm/sm_nh_cntrl.c
$(OUT_DIR)/sm_bsmsm_exms.$(OBJ): $(IN_DIR)/../sm/sm_bsmsm_exms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMNHMILNH -DLCLNH $(IN_DIR)/../sm/sm_bsmsm_exms.c -o \
	$(OUT_DIR)/sm_bsmsm_exms.$(OBJ)	
$(OUT_DIR)/sm_pmhsm_exms.$(OBJ): $(IN_DIR)/../sm/sm_pmhsm_exms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMNHMILNH -DLCLNH $(IN_DIR)/../sm/sm_pmhsm_exms.c -o \
	$(OUT_DIR)/sm_pmhsm_exms.$(OBJ)
$(OUT_DIR)/sm_lthsm_exms.$(OBJ): $(IN_DIR)/../sm/sm_lthsm_exms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMNHMILNH -DLCLNH $(IN_DIR)/../sm/sm_lthsm_exms.c -o \
	$(OUT_DIR)/sm_lthsm_exms.$(OBJ)
$(OUT_DIR)/sm_nh_exms.$(OBJ): $(IN_DIR)/../sm/sm_nh_exms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMNHMILNH -DLCLNH -DNH_TRACE_ENABLED $(IN_DIR)/../sm/sm_nh_exms.c -o \
	$(OUT_DIR)/sm_nh_exms.$(OBJ)
$(OUT_DIR)/sm_nh_ptmi.$(OBJ): $(IN_DIR)/../sm/sm_nh_ptmi.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMNHMILNH -DLCLNH $(IN_DIR)/../sm/sm_nh_ptmi.c -o \
	$(OUT_DIR)/sm_nh_ptmi.$(OBJ)

$(OUT_DIR)/sm_kw_cntrl.$(OBJ): $(IN_DIR)/../sm/sm_kw_cntrl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/sm_kw_cntrl.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../sm/sm_kw_cntrl.c
$(OUT_DIR)/sm_kw_exms.$(OBJ): $(IN_DIR)/../sm/sm_kw_exms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCKWMILKW -DLCPJMILPJ -DLCLKW -DLCLPJ $(IN_DIR)/../sm/sm_kw_exms.c -o \
	$(OUT_DIR)/sm_kw_exms.$(OBJ)
$(OUT_DIR)/sm_kw_ptmi.$(OBJ): $(IN_DIR)/../sm/sm_kw_ptmi.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCKWMILKW -DLCPJMILPJ -DLCLKW $(IN_DIR)/../sm/sm_kw_ptmi.c -o \
	$(OUT_DIR)/sm_kw_ptmi.$(OBJ)

$(OUT_DIR)/sm_rg_cntrl.$(OBJ): $(IN_DIR)/../sm/sm_rg_cntrl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/sm_rg_cntrl.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../sm/sm_rg_cntrl.c
$(OUT_DIR)/sm_rg_exms.$(OBJ): $(IN_DIR)/../sm/sm_rg_exms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCRGMILRG -DLCLRG $(IN_DIR)/../sm/sm_rg_exms.c -o \
	$(OUT_DIR)/sm_rg_exms.$(OBJ)
$(OUT_DIR)/sm_rg_ptmi.$(OBJ): $(IN_DIR)/../sm/sm_rg_ptmi.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMMILRG -DLCRGMILRG -DLCLRG $(IN_DIR)/../sm/sm_rg_ptmi.c -o \
	$(OUT_DIR)/sm_rg_ptmi.$(OBJ)
$(OUT_DIR)/sm_sz_cntrl.$(OBJ): $(IN_DIR)/../sm/sm_sz_cntrl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSZMILSZ -DLCLSZ -DSZ_ENB $(IN_DIR)/../sm/sm_sz_cntrl.c -o \
	$(OUT_DIR)/sm_sz_cntrl.$(OBJ)
$(OUT_DIR)/sm_sz_exms.$(OBJ): $(IN_DIR)/../sm/sm_sz_exms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSZMILSZ -DLCLSZ -DSZ_ENB $(IN_DIR)/../sm/sm_sz_exms.c -o \
	$(OUT_DIR)/sm_sz_exms.$(OBJ)
$(OUT_DIR)/sm_sz_ptmi.$(OBJ): $(IN_DIR)/../sm/sm_sz_ptmi.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMSZMILSZ -DLCLSZ -DSZ_ENB $(IN_DIR)/../sm/sm_sz_ptmi.c -o \
	$(OUT_DIR)/sm_sz_ptmi.$(OBJ)

$(OUT_DIR)/sm_sb_cntrl.$(OBJ): $(IN_DIR)/../sm/sm_sb_cntrl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/sm_sb_cntrl.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(SBOPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../sm/sm_sb_cntrl.c
$(OUT_DIR)/sm_sb_exms.$(OBJ): $(IN_DIR)/../sm/sm_sb_exms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(SBOPTS) $(CCuehFLAGS) $(IN_DIR)/../sm/sm_sb_exms.c -o \
	$(OUT_DIR)/sm_sb_exms.$(OBJ)
$(OUT_DIR)/sm_sb_ptmi.$(OBJ): $(IN_DIR)/../sm/sm_sb_ptmi.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMSBMILSB -DLSB2 -DLSB3 -DLSB6 -DLCLSB $(IN_DIR)/../sm/sm_sb_ptmi.c -o \
	$(OUT_DIR)/sm_sb_ptmi.$(OBJ)

$(OUT_DIR)/sm_hi_cntrl.$(OBJ): $(IN_DIR)/../sm/sm_hi_cntrl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/sm_hi_cntrl.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../sm/sm_hi_cntrl.c
$(OUT_DIR)/sm_hi_exms.$(OBJ): $(IN_DIR)/../sm/sm_hi_exms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(HIOPTS) $(CCuehFLAGS) $(IN_DIR)/../sm/sm_hi_exms.c -o \
	$(OUT_DIR)/sm_hi_exms.$(OBJ)

$(OUT_DIR)/sm_hi_ptmi.$(OBJ): $(IN_DIR)/../sm/sm_hi_ptmi.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMHIMILHI $(IN_DIR)/../sm/sm_hi_ptmi.c -o \
	$(OUT_DIR)/sm_hi_ptmi.$(OBJ)

$(OUT_DIR)/sm_eg_cntrl.$(OBJ): $(IN_DIR)/../sm/sm_eg_cntrl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/sm_eg_cntrl.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../sm/sm_eg_cntrl.c
$(OUT_DIR)/sm_udh_cntrl.$(OBJ): $(IN_DIR)/../sm/sm_udh_cntrl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/sm_udh_cntrl.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../sm/sm_udh_cntrl.c
$(OUT_DIR)/sm_eg_exms.$(OBJ): $(IN_DIR)/../sm/sm_eg_exms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCEGMILEG -DLCLEG $(IN_DIR)/../sm/sm_eg_exms.c -o \
	$(OUT_DIR)/sm_eg_exms.$(OBJ)
$(OUT_DIR)/sm_eg_ptmi.$(OBJ): $(IN_DIR)/../sm/sm_eg_ptmi.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS)  -DLCSMEGMILEG -DLCLEG $(IN_DIR)/../sm/sm_eg_ptmi.c -o \
	$(OUT_DIR)/sm_eg_ptmi.$(OBJ)

$(OUT_DIR)/sm_cntrl.$(OBJ): $(IN_DIR)/../sm/sm_cntrl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/sm_cntrl.o $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../sm/sm_cntrl.c
$(OUT_DIR)/sm_rrm_ex_ms.$(OBJ): $(IN_DIR)/../sm/sm_rrm_ex_ms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCUEHMILUEH -DLCLUEH $(IN_DIR)/../sm/sm_rrm_ex_ms.c -o \
	$(OUT_DIR)/sm_rrm_ex_ms.$(OBJ)
$(OUT_DIR)/sm_ptmi.$(OBJ): $(IN_DIR)/../sm/sm_ptmi.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMUEHMILUEH -DLCLUEH $(IN_DIR)/../sm/sm_ptmi.c -o \
	$(OUT_DIR)/sm_ptmi.$(OBJ)

$(OUT_DIR)/sm_pj_cntrl.$(OBJ): $(IN_DIR)/../sm/sm_pj_cntrl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/sm_pj_cntrl.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../sm/sm_pj_cntrl.c
$(OUT_DIR)/sm_pj_exms.$(OBJ): $(IN_DIR)/../sm/sm_pj_exms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMPJMILPJ -DLCPJMILPJ -DLCLPJ $(IN_DIR)/../sm/sm_pj_exms.c -o \
	$(OUT_DIR)/sm_pj_exms.$(OBJ)
$(OUT_DIR)/sm_pj_ptmi.$(OBJ): $(IN_DIR)/../sm/sm_pj_ptmi.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMPJMILPJ -DLCPJMILPJ -DLCLPJ $(IN_DIR)/../sm/sm_pj_ptmi.c -o \
	$(OUT_DIR)/sm_pj_ptmi.$(OBJ)


$(OUT_DIR)/sm_ys_cntrl.$(OBJ): $(IN_DIR)/../sm/sm_ys_cntrl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/sm_ys_cntrl.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../sm/sm_ys_cntrl.c

$(OUT_DIR)/sm_ys_exms.$(OBJ): $(IN_DIR)/../sm/sm_ys_exms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/../sm/sm_ys_exms.c -o \
	$(OUT_DIR)/sm_ys_exms.$(OBJ)
$(OUT_DIR)/sm_ys_ptmi.$(OBJ): $(IN_DIR)/../sm/sm_ys_ptmi.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/../sm/sm_ys_ptmi.c -o \
	$(OUT_DIR)/sm_ys_ptmi.$(OBJ)

$(OUT_DIR)/sm_udh_exms.$(OBJ): $(IN_DIR)/../sm/sm_udh_exms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/sm_udh_exms.o  $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../sm/sm_udh_exms.c
    
	
#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/libsm.a:$(uehE2eLnxOBJS)
	ar -cr $(LIB_DIR)/libsm.a $(uehE2eLnxOBJS)	
#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	rm -f $(LIB_DIR)/libsm.a $(uehE2eLnxOBJS)


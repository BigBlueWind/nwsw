#-------------------------------------------------------------#
#Makefile for product SZ - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCuehFLAGS= -DEGTP_U -DNX -DUZ -DEU -DKW_PDCP -DDEBUGP -Wall -Werror -DTFU_VER_2 -DLCEGUIEGT -DLCUEHLICTF -DLCUEHLINHU -DLCPJUIPJU -DLCUEHLIRGR -UCZ -DLCNHUINHU -DRRM_PRL_FLAG 
CCveFLAGS=  -DEGTP_U -DNX -DUZ -DEU -DKW_PDCP -DDEBUGP  -DTFU_VER_2 -DLCEGUIEGT -DLCVELICTF -DLCVELINHU -DLCPJUIPJU -DLCVELIRGR -DLCSMNHMILNH  -DPX -DPTNXLIPJU -DPTVELIRGR


uehE2eLnxSRCS= \
    $(IN_DIR)/ueh_bldmsg.c \
    $(IN_DIR)/ueh_mac_sch_cfg.c \
    $(IN_DIR)/../s1mgr/s1ap_cmn_proc.c \
    $(IN_DIR)/../s1mgr/s1ap_wrap.c \
    $(IN_DIR)/../s1mgr/s1ap_unwrap.c \
    $(IN_DIR)/../s1mgr/s1ap_mgr.c \
    $(IN_DIR)/../s1mgr/s1ap_sm.c \
    $(IN_DIR)/../s1mgr/s1ap_err.c \
    $(IN_DIR)/ueh_cell.c \
    $(IN_DIR)/ueh_db.c \
    $(IN_DIR)/ueh_ct_ptli.c \
    $(IN_DIR)/ueh_ex_ms.c \
    $(IN_DIR)/ueh_li.c \
    $(IN_DIR)/ueh_names.c \
    $(IN_DIR)/ueh_cmn_sec.c \
    $(IN_DIR)/hmac_sha2.c \
    $(IN_DIR)/sha2.c \
    $(IN_DIR)/ueh_s1ap_cm.c \
    $(IN_DIR)/ueh_li_hdl.c \
    $(IN_DIR)/ueh_l2m.c \
    $(IN_DIR)/ueh_mi.c \
    $(IN_DIR)/ueh_nh_ptli.c \
    $(IN_DIR)/ueh_pj_ptli.c \
    $(IN_DIR)/ueh_ptmi.c \
    $(IN_DIR)/ueh_relay.c \
    $(IN_DIR)/ueh_rg_ptli.c \
    $(IN_DIR)/ueh_sz_ptli.c \
    $(IN_DIR)/ueh_sib.c \
    $(IN_DIR)/ueh_tmr.c \
    $(IN_DIR)/ueh_utils.c \
    $(IN_DIR)/rrm_sfi_util.c \
    $(IN_DIR)/cp_main.c \
    $(IN_DIR)/radioup_main.c \
    $(IN_DIR)/ueh_crm.c\
    $(IN_DIR)/ueh_crm_sm.c\
    $(IN_DIR)/ueh_srs_res_mgmt.c\
    $(IN_DIR)/ueh_pucch_res_mgmt.c\
    $(IN_DIR)/ueh_udh_ptli.c
   
#    $(IN_DIR)/ueh_dflt.c

# Product objects-------------------------------------------------------
uehE2eLnxOBJS= \
    $(OUT_DIR)/ueh_bldmsg.$(OBJ) \
    $(OUT_DIR)/ueh_mac_sch_cfg.$(OBJ) \
    $(OUT_DIR)/s1ap_cmn_proc.$(OBJ) \
    $(OUT_DIR)/s1ap_wrap.$(OBJ) \
    $(OUT_DIR)/s1ap_unwrap.$(OBJ) \
    $(OUT_DIR)/s1ap_mgr.$(OBJ) \
    $(OUT_DIR)/s1ap_sm.$(OBJ) \
    $(OUT_DIR)/s1ap_err.$(OBJ) \
    $(OUT_DIR)/ueh_cell.$(OBJ) \
    $(OUT_DIR)/ueh_db.$(OBJ) \
    $(OUT_DIR)/ueh_ct_ptli.$(OBJ) \
    $(OUT_DIR)/ueh_ex_ms.$(OBJ) \
    $(OUT_DIR)/hmac_sha2.$(OBJ) \
    $(OUT_DIR)/sha2.$(OBJ) \
    $(OUT_DIR)/ueh_cmn_sec.$(OBJ) \
    $(OUT_DIR)/ueh_s1ap_cm.$(OBJ) \
    $(OUT_DIR)/ueh_li.$(OBJ) \
    $(OUT_DIR)/ueh_names.$(OBJ) \
    $(OUT_DIR)/ueh_li_hdl.$(OBJ) \
    $(OUT_DIR)/ueh_l2m.$(OBJ) \
    $(OUT_DIR)/ueh_mi.$(OBJ) \
    $(OUT_DIR)/ueh_nh_ptli.$(OBJ) \
    $(OUT_DIR)/ueh_pj_ptli.$(OBJ) \
    $(OUT_DIR)/ueh_ptmi.$(OBJ) \
    $(OUT_DIR)/ueh_relay.$(OBJ) \
    $(OUT_DIR)/ueh_rg_ptli.$(OBJ) \
    $(OUT_DIR)/ueh_sz_ptli.$(OBJ) \
    $(OUT_DIR)/ueh_sib.$(OBJ) \
    $(OUT_DIR)/ueh_tmr.$(OBJ) \
    $(OUT_DIR)/ueh_utils.$(OBJ) \
    $(OUT_DIR)/rrm_sfi_util.$(OBJ) \
    $(OUT_DIR)/cp_main.$(OBJ) \
    $(OUT_DIR)/radioup_main.$(OBJ) \
    $(OUT_DIR)/ueh_crm.$(OBJ)\
    $(OUT_DIR)/ueh_crm_sm.$(OBJ)\
    $(OUT_DIR)/ueh_srs_res_mgmt.$(OBJ)\
    $(OUT_DIR)/ueh_pucch_res_mgmt.$(OBJ)\
    $(OUT_DIR)/ueh_udh_ptli.$(OBJ)

#    $(OUT_DIR)/ueh_dflt.$(OBJ)

UEHCM_INC=$(IN_DIR)/inc/ueh.h $(IN_DIR)/inc/sctp_cfg_table.h $(IN_DIR)/inc/s1ap_cfg_table.h  $(IN_DIR)/inc/ueh_perf.h $(IN_DIR)/../../sfi/sfi.h

ALL_INC=$(UEHCM_INC) $(CM_INC) $(UEH_DIR)

BUILD_PREFIX            := "==> Building"
V                       := @

UEH_INC_DIR	= -I$(IN_DIR)/../s1mgr/ -I$(IN_DIR)/ -I$(IN_DIR)/../../rrc/ -I$(IN_DIR)/../udh/ -I$(IN_DIR)/../../inc/ -I$(IN_DIR)/../../pseudophy/ -I$(IN_DIR)/../../libRRMprint/ -L$(LIB_DIR)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/ueh_bldmsg.$(OBJ): $(IN_DIR)/ueh_bldmsg.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_bldmsg.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_bldmsg.c

$(OUT_DIR)/ueh_mac_sch_cfg.$(OBJ): $(IN_DIR)/ueh_mac_sch_cfg.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_mac_sch_cfg.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_mac_sch_cfg.c

$(OUT_DIR)/s1ap_cmn_proc.$(OBJ): $(IN_DIR)/../s1mgr/s1ap_cmn_proc.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/s1ap_cmn_proc.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../s1mgr/s1ap_cmn_proc.c
$(OUT_DIR)/s1ap_wrap.$(OBJ): $(IN_DIR)/../s1mgr/s1ap_wrap.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/s1ap_wrap.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../s1mgr/s1ap_wrap.c
$(OUT_DIR)/s1ap_unwrap.$(OBJ): $(IN_DIR)/../s1mgr/s1ap_unwrap.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/s1ap_unwrap.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../s1mgr/s1ap_unwrap.c
$(OUT_DIR)/s1ap_mgr.$(OBJ): $(IN_DIR)/../s1mgr/s1ap_mgr.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/s1ap_mgr.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../s1mgr/s1ap_mgr.c
$(OUT_DIR)/s1ap_sm.$(OBJ): $(IN_DIR)/../s1mgr/s1ap_sm.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/s1ap_sm.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../s1mgr/s1ap_sm.c
$(OUT_DIR)/s1ap_err.$(OBJ): $(IN_DIR)/../s1mgr/s1ap_err.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/s1ap_err.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../s1mgr/s1ap_err.c
$(OUT_DIR)/ueh_cell.$(OBJ): $(IN_DIR)/ueh_cell.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_cell.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_cell.c
$(OUT_DIR)/ueh_db.$(OBJ): $(IN_DIR)/ueh_db.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_db.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_db.c	
$(OUT_DIR)/ueh_ct_ptli.$(OBJ): $(IN_DIR)/ueh_ct_ptli.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_ct_ptli.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_ct_ptli.c
$(OUT_DIR)/ueh_dflt.$(OBJ): $(IN_DIR)/ueh_dflt.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_dflt.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_dflt.c
$(OUT_DIR)/ueh_ex_ms.$(OBJ): $(IN_DIR)/ueh_ex_ms.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_ex_ms.o  $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_ex_ms.c
$(OUT_DIR)/sha2.$(OBJ): $(IN_DIR)/sha2.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/sha2.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/sha2.c
$(OUT_DIR)/hmac_sha2.$(OBJ): $(IN_DIR)/hmac_sha2.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/hmac_sha2.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/hmac_sha2.c
$(OUT_DIR)/ueh_cmn_sec.$(OBJ): $(IN_DIR)/ueh_cmn_sec.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_cmn_sec.o -I$(IN_DIR)/../s1mgr/ $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_cmn_sec.c	
$(OUT_DIR)/ueh_s1ap_cm.$(OBJ): $(IN_DIR)/ueh_s1ap_cm.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_s1ap_cm.o -I$(IN_DIR)/../s1mgr/ $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_s1ap_cm.c
$(OUT_DIR)/ueh_li.$(OBJ): $(IN_DIR)/ueh_li.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_li.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_li.c

$(OUT_DIR)/ueh_names.$(OBJ): $(IN_DIR)/ueh_names.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_names.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_names.c

$(OUT_DIR)/ueh_li_hdl.$(OBJ): $(IN_DIR)/ueh_li_hdl.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_li_hdl.o -I$(IN_DIR)/../s1mgr/ $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_li_hdl.c

$(OUT_DIR)/ueh_l2m.$(OBJ): $(IN_DIR)/ueh_l2m.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_l2m.o -I$(IN_DIR)/../s1mgr/ $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_l2m.c

$(OUT_DIR)/ueh_mi.$(OBJ): $(IN_DIR)/ueh_mi.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_mi.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_mi.c

$(OUT_DIR)/ueh_nh_ptli.$(OBJ): $(IN_DIR)/ueh_nh_ptli.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_nh_ptli.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_nh_ptli.c
$(OUT_DIR)/ueh_pj_ptli.$(OBJ): $(IN_DIR)/ueh_pj_ptli.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_pj_ptli.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_pj_ptli.c
$(OUT_DIR)/ueh_ptmi.$(OBJ): $(IN_DIR)/ueh_ptmi.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_ptmi.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_ptmi.c
$(OUT_DIR)/ueh_relay.$(OBJ): $(IN_DIR)/ueh_relay.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_relay.o -I.$(IN_DIR)/../s1mgr/ -I$(IN_DIR)/ -I$(IN_DIR)/ $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_relay.c
$(OUT_DIR)/ueh_rg_ptli.$(OBJ): $(IN_DIR)/ueh_rg_ptli.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_rg_ptli.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_rg_ptli.c
$(OUT_DIR)/ueh_sz_ptli.$(OBJ): $(IN_DIR)/ueh_sz_ptli.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_sz_ptli.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_sz_ptli.c
$(OUT_DIR)/ueh_sib.$(OBJ): $(IN_DIR)/ueh_sib.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_sib.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_sib.c
$(OUT_DIR)/ueh_tmr.$(OBJ): $(IN_DIR)/ueh_tmr.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_tmr.o -I$(IN_DIR)/../s1mgr/ $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_tmr.c
$(OUT_DIR)/ueh_utils.$(OBJ): $(IN_DIR)/ueh_utils.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/ueh_utils.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/ueh_utils.c

$(OUT_DIR)/rrm_sfi_util.$(OBJ): $(IN_DIR)/rrm_sfi_util.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/rrm_sfi_util.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/rrm_sfi_util.c
#$(OUT_DIR)/gtp_main.$(OBJ): $(IN_DIR)/gtp_main.c $(ALL_INC)
#	@echo $(BUILD_PREFIX) $@
#	$(V) $(CC) -c -o$(OUT_DIR)/gtp_main.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/gtp_main.c
$(OUT_DIR)/cp_main.$(OBJ): $(IN_DIR)/cp_main.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/cp_main.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/cp_main.c
$(OUT_DIR)/radioup_main.$(OBJ): $(IN_DIR)/radioup_main.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/radioup_main.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/radioup_main.c

$(OUT_DIR)/ueh_pucch_res_mgmt.$(OBJ): $(IN_DIR)/ueh_pucch_res_mgmt.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/ueh_pucch_res_mgmt.c -o \
	$(OUT_DIR)/ueh_pucch_res_mgmt.$(OBJ)

$(OUT_DIR)/ueh_srs_res_mgmt.$(OBJ): $(IN_DIR)/ueh_srs_res_mgmt.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/ueh_srs_res_mgmt.c -o \
	$(OUT_DIR)/ueh_srs_res_mgmt.$(OBJ)

$(OUT_DIR)/ueh_crm.$(OBJ): $(IN_DIR)/ueh_crm.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/ueh_crm.c -o \
	$(OUT_DIR)/ueh_crm.$(OBJ)
	
$(OUT_DIR)/ueh_crm_sm.$(OBJ): $(IN_DIR)/ueh_crm_sm.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR) -I$(IN_DIR)/../s1mgr/   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/ueh_crm_sm.c -o \
	$(OUT_DIR)/ueh_crm_sm.$(OBJ)
	
#$(OUT_DIR)/udh_func.$(OBJ): $(IN_DIR)/../udh/udh_func.c $(ALL_INC)
#	@echo $(BUILD_PREFIX) $@
#	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/../udh/udh_func.c -o \
	$(OUT_DIR)/udh_func.$(OBJ)
	
#$(OUT_DIR)/udh_lm.$(OBJ): $(IN_DIR)/../udh/udh_lm.c $(ALL_INC)
#	@echo $(BUILD_PREFIX) $@
#	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/../udh/udh_lm.c -o \
	$(OUT_DIR)/udh_lm.$(OBJ)
	
#$(OUT_DIR)/udh_main.$(OBJ): $(IN_DIR)/../udh/udh_main.c $(ALL_INC)
#	@echo $(BUILD_PREFIX) $@
#	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/../udh/udh_main.c -o \
	$(OUT_DIR)/udh_main.$(OBJ)

$(OUT_DIR)/ueh_udh_ptli.$(OBJ): $(IN_DIR)/ueh_udh_ptli.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/ueh_udh_ptli.c -o \
	$(OUT_DIR)/ueh_udh_ptli.$(OBJ)

#$(OUT_DIR)/sm_udh_exms.$(OBJ): $(IN_DIR)/../sm/sm_udh_exms.c $(ALL_INC)\
	@echo $(BUILD_PREFIX) $@\
	$(V) $(CC) -c -o$(OUT_DIR)/sm_udh_exms.o  $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/../sm/sm_udh_exms.c
    
	
#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/libueh.a:$(uehE2eLnxOBJS)
	ar -cr $(LIB_DIR)/libueh.a $(uehE2eLnxOBJS)	
#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(LIB_DIR)/libueh.a $(uehE2eLnxOBJS)


#-------------------------------------------------------------#
#Makefile for product SZ - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

ifeq (${BLD},seum)
CCveFLAGS=-DVE_RELAY -DEU_DAT_APP -DEGTP_U -DNX -DPX -DUZ -DEU -DKW_PDCP -UPTNXLIPJU -UPTVELIRGR -DDEBUGP -UTFU_TDD -DTFU_VER_2 -DLCEGUIEGT -DLCVELICTF -DLCVELINHU -ULCPJUIPJU -DLCVELIRGR -ULTE_TDD -UVE_SRB2_SUPPORT -UTA_NEW -USI_NEW -URGR_SI_SCH
else
CCveFLAGS=-DVE_RELAY -DEU_DAT_APP -DEGTP_U -DNX -DPX -DUZ -DEU -DKW_PDCP -DKW -UPTNXLIPJU -UPTVELIRGR -DDEBUGP -UTFU_TDD -DTFU_VER_2 -DLCEGUIEGT -DLCVELICTF -DLCVELINHU -ULCPJUIPJU -DLCVELIRGR -ULTE_TDD -UVE_SRB2_SUPPORT -UTA_NEW -USI_NEW -URGR_SI_SCH -UVE_SM_LOG_TO_FILE
endif

CCsmFLAGS=-DHI -DSB -DEG -DVE -DKW -DSZ -DYS -DRG -DPJ
SS_DIR=/u/prd/rtr/mt

# Product sources-------------------------------------------------------
vePerfLnxSRCS= \
    $(IN_DIR)/ve_bldmsg.c \
    $(IN_DIR)/ve_cell.c \
    $(IN_DIR)/ve_perf_li.c \
    $(IN_DIR)/ve_rout.c \
    $(IN_DIR)/ve_ct_ptli.c \
    $(IN_DIR)/ve_data_app_hdl.c \
    $(IN_DIR)/ve_dflt.c \
    $(IN_DIR)/ve_eg_ptli.c \
    $(IN_DIR)/ve_eut.c \
    $(IN_DIR)/ve_ex_ms.c \
    $(IN_DIR)/ve_li.c \
    $(IN_DIR)/ve_li_hdl.c \
    $(IN_DIR)/ve_mi.c \
    $(IN_DIR)/ve_nh_ptli.c \
    $(IN_DIR)/ve_pj_ptli.c \
    $(IN_DIR)/ve_ptmi.c \
    $(IN_DIR)/ve_relay.c \
    $(IN_DIR)/ve_rg_ptli.c \
    $(IN_DIR)/ve_sz_ptli.c \
    $(IN_DIR)/ve_sib.c \
    $(IN_DIR)/ve_tmr.c \
    $(IN_DIR)/ve_utils.c \
    $(IN_DIR)/ve_sm_init_merged.c \
    $(IN_DIR)/ve_sm_nh_cntrl.c \
    $(IN_DIR)/ve_sm_nh_exms.c \
    $(IN_DIR)/ve_sm_nh_ptmi.c \
    $(IN_DIR)/ve_sm_kw_cntrl.c \
    $(IN_DIR)/ve_sm_kw_exms.c \
    $(IN_DIR)/ve_sm_kw_ptmi.c \
    $(IN_DIR)/ve_sm_rg_cntrl.c \
    $(IN_DIR)/ve_sm_rg_exms.c \
    $(IN_DIR)/ve_sm_rg_ptmi.c \
    $(IN_DIR)/ve_sm_sz_cntrl.c \
    $(IN_DIR)/ve_sm_sz_exms.c \
    $(IN_DIR)/ve_sm_sz_ptmi.c \
    $(IN_DIR)/ve_sm_sb_cntrl.c \
    $(IN_DIR)/ve_sm_sb_exms.c \
    $(IN_DIR)/ve_sm_sb_ptmi.c \
    $(IN_DIR)/ve_sm_hi_cntrl.c \
    $(IN_DIR)/ve_sm_hi_exms.c \
    $(IN_DIR)/ve_sm_hi_ptmi.c \
    $(IN_DIR)/ve_sm_eg_cntrl.c \
    $(IN_DIR)/ve_sm_eg_exms.c \
    $(IN_DIR)/ve_sm_eg_ptmi.c \
    $(IN_DIR)/ve_sm_ve_cntrl.c \
    $(IN_DIR)/ve_sm_ve_exms.c \
    $(IN_DIR)/ve_sm_ve_ptmi.c \
    $(IN_DIR)/ve_sm_pj_cntrl.c \
    $(IN_DIR)/ve_sm_pj_exms.c \
    $(IN_DIR)/ve_sm_pj_ptmi.c \
    $(IN_DIR)/ve_sm_ys_cntrl.c \
    $(IN_DIR)/ve_sm_ys_exms.c \
    $(IN_DIR)/ve_sm_ys_ptmi.c 


# Product sources-------------------------------------------------------
vePerfLnxSRCSUU= \
    $(IN_DIR)/ve_bldmsg.c \
    $(IN_DIR)/ve_cell.c \
    $(IN_DIR)/ve_perf_li.c \
    $(IN_DIR)/ve_ct_ptli.c \
    $(IN_DIR)/ve_data_app_hdl.c \
    $(IN_DIR)/ve_dflt.c \
    $(IN_DIR)/ve_ex_ms.c \
    $(IN_DIR)/ve_li.c \
    $(IN_DIR)/ve_li_hdl.c \
    $(IN_DIR)/ve_mi.c \
    $(IN_DIR)/ve_nh_ptli.c \
    $(IN_DIR)/ve_pj_ptli.c \
    $(IN_DIR)/ve_ptmi.c \
    $(IN_DIR)/ve_relay.c \
    $(IN_DIR)/ve_rg_ptli.c \
    $(IN_DIR)/ve_sib.c \
    $(IN_DIR)/ve_tmr.c \
    $(IN_DIR)/ve_utils.c \
    $(IN_DIR)/ve_sm_init_merged.c \
    $(IN_DIR)/ve_sm_nh_cntrl.c \
    $(IN_DIR)/ve_sm_nh_exms.c \
    $(IN_DIR)/ve_sm_nh_ptmi.c \
    $(IN_DIR)/ve_sm_kw_cntrl.c \
    $(IN_DIR)/ve_sm_kw_exms.c \
    $(IN_DIR)/ve_sm_kw_ptmi.c \
    $(IN_DIR)/ve_sm_rg_cntrl.c \
    $(IN_DIR)/ve_sm_rg_exms.c \
    $(IN_DIR)/ve_sm_rg_ptmi.c \
    $(IN_DIR)/ve_sm_ve_cntrl.c \
    $(IN_DIR)/ve_sm_ve_exms.c \
    $(IN_DIR)/ve_sm_ve_ptmi.c \
    $(IN_DIR)/ve_sm_pj_cntrl.c \
    $(IN_DIR)/ve_sm_pj_exms.c \
    $(IN_DIR)/ve_sm_pj_ptmi.c \
    $(IN_DIR)/ve_sm_ys_cntrl.c \
    $(IN_DIR)/ve_sm_ys_exms.c \
    $(IN_DIR)/ve_sm_ys_ptmi.c 

veE2eLnxSRCS= \
    $(IN_DIR)/ve_bldmsg.c \
    $(IN_DIR)/ve_cell.c \
    $(IN_DIR)/ve_ct_ptli.c \
    $(IN_DIR)/ve_data_app_hdl.c \
    $(IN_DIR)/ve_dflt.c \
    $(IN_DIR)/ve_eg_ptli.c \
    $(IN_DIR)/ve_eut.c \
    $(IN_DIR)/ve_ex_ms.c \
    $(IN_DIR)/ve_li.c \
    $(IN_DIR)/ve_li_hdl.c \
    $(IN_DIR)/ve_mi.c \
    $(IN_DIR)/ve_nh_ptli.c \
    $(IN_DIR)/ve_pj_ptli.c \
    $(IN_DIR)/ve_ptmi.c \
    $(IN_DIR)/ve_relay.c \
    $(IN_DIR)/ve_rg_ptli.c \
    $(IN_DIR)/ve_sz_ptli.c \
    $(IN_DIR)/ve_sib.c \
    $(IN_DIR)/ve_tmr.c \
    $(IN_DIR)/ve_utils.c \
    $(IN_DIR)/ve_sm_init_merged.c \
    $(IN_DIR)/ve_sm_nh_cntrl.c \
    $(IN_DIR)/ve_sm_nh_exms.c \
    $(IN_DIR)/ve_sm_nh_ptmi.c \
    $(IN_DIR)/ve_sm_kw_cntrl.c \
    $(IN_DIR)/ve_sm_kw_exms.c \
    $(IN_DIR)/ve_sm_kw_ptmi.c \
    $(IN_DIR)/ve_sm_rg_cntrl.c \
    $(IN_DIR)/ve_sm_rg_exms.c \
    $(IN_DIR)/ve_sm_rg_ptmi.c \
    $(IN_DIR)/ve_sm_sz_cntrl.c \
    $(IN_DIR)/ve_sm_sz_exms.c \
    $(IN_DIR)/ve_sm_sz_ptmi.c \
    $(IN_DIR)/ve_sm_sb_cntrl.c \
    $(IN_DIR)/ve_sm_sb_exms.c \
    $(IN_DIR)/ve_sm_sb_ptmi.c \
    $(IN_DIR)/ve_sm_hi_cntrl.c \
    $(IN_DIR)/ve_sm_hi_exms.c \
    $(IN_DIR)/ve_sm_hi_ptmi.c \
    $(IN_DIR)/ve_sm_eg_cntrl.c \
    $(IN_DIR)/ve_sm_eg_exms.c \
    $(IN_DIR)/ve_sm_eg_ptmi.c \
    $(IN_DIR)/ve_sm_ve_cntrl.c \
    $(IN_DIR)/ve_sm_ve_exms.c \
    $(IN_DIR)/ve_sm_ve_ptmi.c \
    $(IN_DIR)/ve_sm_pj_cntrl.c \
    $(IN_DIR)/ve_sm_pj_exms.c \
    $(IN_DIR)/ve_sm_pj_ptmi.c \
    $(IN_DIR)/ve_sm_ys_cntrl.c \
    $(IN_DIR)/ve_sm_ys_exms.c \
    $(IN_DIR)/ve_sm_ys_ptmi.c 




# Product objects-------------------------------------------------------
veE2eLnxOBJS= \
    $(OUT_DIR)/ve_bldmsg.$(OBJ) \
    $(OUT_DIR)/ve_cell.$(OBJ) \
    $(OUT_DIR)/ve_ct_ptli.$(OBJ) \
    $(OUT_DIR)/ve_data_app_hdl.$(OBJ) \
    $(OUT_DIR)/ve_dflt.$(OBJ) \
    $(OUT_DIR)/ve_eg_ptli.$(OBJ) \
    $(OUT_DIR)/ve_eut.$(OBJ) \
    $(OUT_DIR)/ve_ex_ms.$(OBJ) \
    $(OUT_DIR)/ve_li.$(OBJ) \
    $(OUT_DIR)/ve_li_hdl.$(OBJ) \
    $(OUT_DIR)/ve_mi.$(OBJ) \
    $(OUT_DIR)/ve_nh_ptli.$(OBJ) \
    $(OUT_DIR)/ve_pj_ptli.$(OBJ) \
    $(OUT_DIR)/ve_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_relay.$(OBJ) \
    $(OUT_DIR)/ve_rg_ptli.$(OBJ) \
    $(OUT_DIR)/ve_sz_ptli.$(OBJ) \
    $(OUT_DIR)/ve_sib.$(OBJ) \
    $(OUT_DIR)/ve_tmr.$(OBJ) \
    $(OUT_DIR)/ve_utils.$(OBJ) \
    $(OUT_DIR)/ve_sm_init_merged.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_sz_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_sz_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_sz_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_sb_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_sb_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_sb_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_hi_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_hi_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_hi_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_eg_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_eg_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_eg_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_ptmi.$(OBJ)

vePerfLnxOBJS= \
    $(OUT_DIR)/ve_bldmsg.$(OBJ) \
    $(OUT_DIR)/ve_cell.$(OBJ) \
    $(OUT_DIR)/ve_perf_li.$(OBJ) \
    $(OUT_DIR)/ve_rout.$(OBJ) \
    $(OUT_DIR)/ve_ct_ptli.$(OBJ) \
    $(OUT_DIR)/ve_data_app_hdl.$(OBJ) \
    $(OUT_DIR)/ve_dflt.$(OBJ) \
    $(OUT_DIR)/ve_eg_ptli.$(OBJ) \
    $(OUT_DIR)/ve_eut.$(OBJ) \
    $(OUT_DIR)/ve_ex_ms.$(OBJ) \
    $(OUT_DIR)/ve_li.$(OBJ) \
    $(OUT_DIR)/ve_li_hdl.$(OBJ) \
    $(OUT_DIR)/ve_mi.$(OBJ) \
    $(OUT_DIR)/ve_nh_ptli.$(OBJ) \
    $(OUT_DIR)/ve_pj_ptli.$(OBJ) \
    $(OUT_DIR)/ve_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_relay.$(OBJ) \
    $(OUT_DIR)/ve_rg_ptli.$(OBJ) \
    $(OUT_DIR)/ve_sz_ptli.$(OBJ) \
    $(OUT_DIR)/ve_sib.$(OBJ) \
    $(OUT_DIR)/ve_tmr.$(OBJ) \
    $(OUT_DIR)/ve_utils.$(OBJ) \
    $(OUT_DIR)/ve_sm_init_merged.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_sz_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_sz_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_sz_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_sb_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_sb_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_sb_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_hi_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_hi_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_hi_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_eg_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_eg_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_eg_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_ptmi.$(OBJ)

vePerfLnxOBJSUU= \
    $(OUT_DIR)/ve_bldmsg.$(OBJ) \
    $(OUT_DIR)/ve_cell.$(OBJ) \
    $(OUT_DIR)/ve_perf_li.$(OBJ) \
    $(OUT_DIR)/ve_rout.$(OBJ) \
    $(OUT_DIR)/ve_ct_ptli.$(OBJ) \
    $(OUT_DIR)/ve_data_app_hdl.$(OBJ) \
    $(OUT_DIR)/ve_dflt.$(OBJ) \
    $(OUT_DIR)/ve_ex_ms.$(OBJ) \
    $(OUT_DIR)/ve_li.$(OBJ) \
    $(OUT_DIR)/ve_li_hdl.$(OBJ) \
    $(OUT_DIR)/ve_mi.$(OBJ) \
    $(OUT_DIR)/ve_nh_ptli.$(OBJ) \
    $(OUT_DIR)/ve_pj_ptli.$(OBJ) \
    $(OUT_DIR)/ve_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_relay.$(OBJ) \
    $(OUT_DIR)/ve_rg_ptli.$(OBJ) \
    $(OUT_DIR)/ve_sib.$(OBJ) \
    $(OUT_DIR)/ve_tmr.$(OBJ) \
    $(OUT_DIR)/ve_utils.$(OBJ) \
    $(OUT_DIR)/ve_sm_init_merged.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_ptmi.$(OBJ)


vePerfCavSRCS= \
    $(IN_DIR)/ve_bldmsg.c \
    $(IN_DIR)/ve_cell.c \
    $(IN_DIR)/ve_perf_li.c \
    $(IN_DIR)/ve_ct_ptli.c \
    $(IN_DIR)/ve_data_app_hdl.c \
    $(IN_DIR)/ve_dflt.c \
    $(IN_DIR)/ve_eg_ptli.c \
    $(IN_DIR)/ve_eut.c \
    $(IN_DIR)/ve_ex_ms.c \
    $(IN_DIR)/ve_li.c \
    $(IN_DIR)/ve_li_hdl.c \
    $(IN_DIR)/ve_mi.c \
    $(IN_DIR)/ve_nh_ptli.c \
    $(IN_DIR)/ve_pj_ptli.c \
    $(IN_DIR)/ve_ptmi.c \
    $(IN_DIR)/ve_relay.c \
    $(IN_DIR)/ve_rg_ptli.c \
    $(IN_DIR)/ve_sz_ptli.c \
    $(IN_DIR)/ve_sib.c \
    $(IN_DIR)/ve_tmr.c \
    $(IN_DIR)/ve_utils.c \
    $(IN_DIR)/ve_sm_init.c \
    $(IN_DIR)/ve_sm_nh_cntrl.c \
    $(IN_DIR)/ve_sm_nh_exms.c \
    $(IN_DIR)/ve_sm_nh_ptmi.c \
    $(IN_DIR)/ve_sm_kw_cntrl.c \
    $(IN_DIR)/ve_sm_kw_exms.c \
    $(IN_DIR)/ve_sm_kw_ptmi.c \
    $(IN_DIR)/ve_sm_rg_cntrl.c \
    $(IN_DIR)/ve_sm_rg_exms.c \
    $(IN_DIR)/ve_sm_rg_ptmi.c \
    $(IN_DIR)/ve_sm_sz_cntrl.c \
    $(IN_DIR)/ve_sm_sz_exms.c \
    $(IN_DIR)/ve_sm_sz_ptmi.c \
    $(IN_DIR)/ve_sm_sb_cntrl.c \
    $(IN_DIR)/ve_sm_sb_exms.c \
    $(IN_DIR)/ve_sm_sb_ptmi.c \
    $(IN_DIR)/ve_sm_hi_cntrl.c \
    $(IN_DIR)/ve_sm_hi_exms.c \
    $(IN_DIR)/ve_sm_hi_ptmi.c \
    $(IN_DIR)/ve_sm_eg_cntrl.c \
    $(IN_DIR)/ve_sm_eg_exms.c \
    $(IN_DIR)/ve_sm_eg_ptmi.c \
    $(IN_DIR)/ve_sm_ve_cntrl.c \
    $(IN_DIR)/ve_sm_ve_exms.c \
    $(IN_DIR)/ve_sm_ve_ptmi.c \
    $(IN_DIR)/ve_sm_pj_cntrl.c \
    $(IN_DIR)/ve_sm_pj_exms.c \
    $(IN_DIR)/ve_sm_pj_ptmi.c \
    $(IN_DIR)/ve_sm_ys_cntrl.c \
    $(IN_DIR)/ve_sm_ys_exms.c \
    $(IN_DIR)/ve_sm_ys_ptmi.c 

veE2eCavSRCS= \
    $(IN_DIR)/ve_bldmsg.c \
    $(IN_DIR)/ve_cell.c \
    $(IN_DIR)/ve_ct_ptli.c \
    $(IN_DIR)/ve_data_app_hdl.c \
    $(IN_DIR)/ve_dflt.c \
    $(IN_DIR)/ve_eg_ptli.c \
    $(IN_DIR)/ve_eut.c \
    $(IN_DIR)/ve_ex_ms.c \
    $(IN_DIR)/ve_li.c \
    $(IN_DIR)/ve_li_hdl.c \
    $(IN_DIR)/ve_mi.c \
    $(IN_DIR)/ve_nh_ptli.c \
    $(IN_DIR)/ve_pj_ptli.c \
    $(IN_DIR)/ve_ptmi.c \
    $(IN_DIR)/ve_relay.c \
    $(IN_DIR)/ve_rg_ptli.c \
    $(IN_DIR)/ve_sz_ptli.c \
    $(IN_DIR)/ve_sib.c \
    $(IN_DIR)/ve_tmr.c \
    $(IN_DIR)/ve_utils.c \
    $(IN_DIR)/ve_sm_init.c \
    $(IN_DIR)/ve_sm_nh_cntrl.c \
    $(IN_DIR)/ve_sm_nh_exms.c \
    $(IN_DIR)/ve_sm_nh_ptmi.c \
    $(IN_DIR)/ve_sm_kw_cntrl.c \
    $(IN_DIR)/ve_sm_kw_exms.c \
    $(IN_DIR)/ve_sm_kw_ptmi.c \
    $(IN_DIR)/ve_sm_rg_cntrl.c \
    $(IN_DIR)/ve_sm_rg_exms.c \
    $(IN_DIR)/ve_sm_rg_ptmi.c \
    $(IN_DIR)/ve_sm_sz_cntrl.c \
    $(IN_DIR)/ve_sm_sz_exms.c \
    $(IN_DIR)/ve_sm_sz_ptmi.c \
    $(IN_DIR)/ve_sm_sb_cntrl.c \
    $(IN_DIR)/ve_sm_sb_exms.c \
    $(IN_DIR)/ve_sm_sb_ptmi.c \
    $(IN_DIR)/ve_sm_hi_cntrl.c \
    $(IN_DIR)/ve_sm_hi_exms.c \
    $(IN_DIR)/ve_sm_hi_ptmi.c \
    $(IN_DIR)/ve_sm_eg_cntrl.c \
    $(IN_DIR)/ve_sm_eg_exms.c \
    $(IN_DIR)/ve_sm_eg_ptmi.c \
    $(IN_DIR)/ve_sm_ve_cntrl.c \
    $(IN_DIR)/ve_sm_ve_exms.c \
    $(IN_DIR)/ve_sm_ve_ptmi.c \
    $(IN_DIR)/ve_sm_pj_cntrl.c \
    $(IN_DIR)/ve_sm_pj_exms.c \
    $(IN_DIR)/ve_sm_pj_ptmi.c \
    $(IN_DIR)/ve_sm_ys_cntrl.c \
    $(IN_DIR)/ve_sm_ys_exms.c \
    $(IN_DIR)/ve_sm_ys_ptmi.c 




# Product objects-------------------------------------------------------
veE2eCavOBJS= \
    $(OUT_DIR)/ve_bldmsg.$(OBJ) \
    $(OUT_DIR)/ve_cell.$(OBJ) \
    $(OUT_DIR)/ve_ct_ptli.$(OBJ) \
    $(OUT_DIR)/ve_data_app_hdl.$(OBJ) \
    $(OUT_DIR)/ve_dflt.$(OBJ) \
    $(OUT_DIR)/ve_eg_ptli.$(OBJ) \
    $(OUT_DIR)/ve_eut.$(OBJ) \
    $(OUT_DIR)/ve_ex_ms.$(OBJ) \
    $(OUT_DIR)/ve_li.$(OBJ) \
    $(OUT_DIR)/ve_li_hdl.$(OBJ) \
    $(OUT_DIR)/ve_mi.$(OBJ) \
    $(OUT_DIR)/ve_nh_ptli.$(OBJ) \
    $(OUT_DIR)/ve_pj_ptli.$(OBJ) \
    $(OUT_DIR)/ve_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_relay.$(OBJ) \
    $(OUT_DIR)/ve_rg_ptli.$(OBJ) \
    $(OUT_DIR)/ve_sz_ptli.$(OBJ) \
    $(OUT_DIR)/ve_sib.$(OBJ) \
    $(OUT_DIR)/ve_tmr.$(OBJ) \
    $(OUT_DIR)/ve_utils.$(OBJ) \
    $(OUT_DIR)/ve_sm_init.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_sz_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_sz_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_sz_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_sb_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_sb_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_sb_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_hi_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_hi_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_hi_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_eg_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_eg_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_eg_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_ptmi.$(OBJ)

vePerfCavOBJS= \
    $(OUT_DIR)/ve_bldmsg.$(OBJ) \
    $(OUT_DIR)/ve_cell.$(OBJ) \
    $(OUT_DIR)/ve_perf_li.$(OBJ) \
    $(OUT_DIR)/ve_ct_ptli.$(OBJ) \
    $(OUT_DIR)/ve_data_app_hdl.$(OBJ) \
    $(OUT_DIR)/ve_dflt.$(OBJ) \
    $(OUT_DIR)/ve_eg_ptli.$(OBJ) \
    $(OUT_DIR)/ve_eut.$(OBJ) \
    $(OUT_DIR)/ve_ex_ms.$(OBJ) \
    $(OUT_DIR)/ve_li.$(OBJ) \
    $(OUT_DIR)/ve_li_hdl.$(OBJ) \
    $(OUT_DIR)/ve_mi.$(OBJ) \
    $(OUT_DIR)/ve_nh_ptli.$(OBJ) \
    $(OUT_DIR)/ve_pj_ptli.$(OBJ) \
    $(OUT_DIR)/ve_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_relay.$(OBJ) \
    $(OUT_DIR)/ve_rg_ptli.$(OBJ) \
    $(OUT_DIR)/ve_sz_ptli.$(OBJ) \
    $(OUT_DIR)/ve_sib.$(OBJ) \
    $(OUT_DIR)/ve_tmr.$(OBJ) \
    $(OUT_DIR)/ve_utils.$(OBJ) \
    $(OUT_DIR)/ve_sm_init.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_sz_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_sz_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_sz_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_sb_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_sb_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_sb_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_hi_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_hi_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_hi_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_eg_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_eg_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_eg_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_ptmi.$(OBJ)

# Product sources-------------------------------------------------------
vePerfCavSRCSUU= \
    $(IN_DIR)/ve_bldmsg.c \
    $(IN_DIR)/ve_cell.c \
    $(IN_DIR)/ve_perf_li.c \
    $(IN_DIR)/ve_ct_ptli.c \
    $(IN_DIR)/ve_data_app_hdl.c \
    $(IN_DIR)/ve_dflt.c \
    $(IN_DIR)/ve_ex_ms.c \
    $(IN_DIR)/ve_li.c \
    $(IN_DIR)/ve_li_hdl.c \
    $(IN_DIR)/ve_mi.c \
    $(IN_DIR)/ve_nh_ptli.c \
    $(IN_DIR)/ve_pj_ptli.c \
    $(IN_DIR)/ve_ptmi.c \
    $(IN_DIR)/ve_relay.c \
    $(IN_DIR)/ve_rg_ptli.c \
    $(IN_DIR)/ve_sib.c \
    $(IN_DIR)/ve_tmr.c \
    $(IN_DIR)/ve_utils.c \
    $(IN_DIR)/ve_sm_init.c \
    $(IN_DIR)/ve_sm_nh_cntrl.c \
    $(IN_DIR)/ve_sm_nh_exms.c \
    $(IN_DIR)/ve_sm_nh_ptmi.c \
    $(IN_DIR)/ve_sm_kw_cntrl.c \
    $(IN_DIR)/ve_sm_kw_exms.c \
    $(IN_DIR)/ve_sm_kw_ptmi.c \
    $(IN_DIR)/ve_sm_rg_cntrl.c \
    $(IN_DIR)/ve_sm_rg_exms.c \
    $(IN_DIR)/ve_sm_rg_ptmi.c \
    $(IN_DIR)/ve_sm_ve_cntrl.c \
    $(IN_DIR)/ve_sm_ve_exms.c \
    $(IN_DIR)/ve_sm_ve_ptmi.c \
    $(IN_DIR)/ve_sm_pj_cntrl.c \
    $(IN_DIR)/ve_sm_pj_exms.c \
    $(IN_DIR)/ve_sm_pj_ptmi.c \
    $(IN_DIR)/ve_sm_ys_cntrl.c \
    $(IN_DIR)/ve_sm_ys_exms.c \
    $(IN_DIR)/ve_sm_ys_ptmi.c 

vePerfCavOBJSUU=\
    $(OUT_DIR)/ve_bldmsg.$(OBJ) \
    $(OUT_DIR)/ve_cell.$(OBJ) \
    $(OUT_DIR)/ve_perf_li.$(OBJ) \
    $(OUT_DIR)/ve_ct_ptli.$(OBJ) \
    $(OUT_DIR)/ve_data_app_hdl.$(OBJ) \
    $(OUT_DIR)/ve_dflt.$(OBJ) \
    $(OUT_DIR)/ve_ex_ms.$(OBJ) \
    $(OUT_DIR)/ve_li.$(OBJ) \
    $(OUT_DIR)/ve_li_hdl.$(OBJ) \
    $(OUT_DIR)/ve_mi.$(OBJ) \
    $(OUT_DIR)/ve_nh_ptli.$(OBJ) \
    $(OUT_DIR)/ve_pj_ptli.$(OBJ) \
    $(OUT_DIR)/ve_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_relay.$(OBJ) \
    $(OUT_DIR)/ve_rg_ptli.$(OBJ) \
    $(OUT_DIR)/ve_sib.$(OBJ) \
    $(OUT_DIR)/ve_tmr.$(OBJ) \
    $(OUT_DIR)/ve_utils.$(OBJ) \
    $(OUT_DIR)/ve_sm_init.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_nh_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_kw_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_rg_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_ve_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_pj_ptmi.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_cntrl.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_exms.$(OBJ) \
    $(OUT_DIR)/ve_sm_ys_ptmi.$(OBJ)



VECM_INC=$(IN_DIR)/ve.h $(IN_DIR)/ve.x $(IN_DIR)/ve_perf.h

ALL_INC=$(VECM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/ve_bldmsg.$(OBJ): $(IN_DIR)/ve_bldmsg.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_bldmsg.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_bldmsg.c
$(OUT_DIR)/ve_cell.$(OBJ): $(IN_DIR)/ve_cell.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_cell.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_cell.c
$(OUT_DIR)/ve_perf_li.$(OBJ): $(IN_DIR)/ve_perf_li.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_perf_li.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_perf_li.c
$(OUT_DIR)/ve_rout.$(OBJ): $(IN_DIR)/ve_rout.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_rout.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_rout.c
$(OUT_DIR)/ve_ct_ptli.$(OBJ): $(IN_DIR)/ve_ct_ptli.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_ct_ptli.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_ct_ptli.c
$(OUT_DIR)/ve_data_app_hdl.$(OBJ): $(IN_DIR)/ve_data_app_hdl.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_data_app_hdl.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_data_app_hdl.c
$(OUT_DIR)/ve_dflt.$(OBJ): $(IN_DIR)/ve_dflt.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_dflt.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_dflt.c
$(OUT_DIR)/ve_eg_ptli.$(OBJ): $(IN_DIR)/ve_eg_ptli.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_eg_ptli.o  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_eg_ptli.c
$(OUT_DIR)/ve_eut.$(OBJ): $(IN_DIR)/ve_eut.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_eut.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_eut.c
$(OUT_DIR)/ve_ex_ms.$(OBJ): $(IN_DIR)/ve_ex_ms.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_ex_ms.o  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_ex_ms.c
$(OUT_DIR)/ve_li.$(OBJ): $(IN_DIR)/ve_li.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_li.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_li.c
$(OUT_DIR)/ve_li_hdl.$(OBJ): $(IN_DIR)/ve_li_hdl.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_li_hdl.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_li_hdl.c
$(OUT_DIR)/ve_mi.$(OBJ): $(IN_DIR)/ve_mi.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_mi.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_mi.c

$(OUT_DIR)/ve_nh_ptli.$(OBJ): $(IN_DIR)/ve_nh_ptli.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_nh_ptli.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_nh_ptli.c
$(OUT_DIR)/ve_pj_ptli.$(OBJ): $(IN_DIR)/ve_pj_ptli.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_pj_ptli.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_pj_ptli.c
$(OUT_DIR)/ve_ptmi.$(OBJ): $(IN_DIR)/ve_ptmi.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_ptmi.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_ptmi.c
$(OUT_DIR)/ve_relay.$(OBJ): $(IN_DIR)/ve_relay.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_relay.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_relay.c
$(OUT_DIR)/ve_rg_ptli.$(OBJ): $(IN_DIR)/ve_rg_ptli.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_rg_ptli.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_rg_ptli.c
$(OUT_DIR)/ve_sz_ptli.$(OBJ): $(IN_DIR)/ve_sz_ptli.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_sz_ptli.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_sz_ptli.c
$(OUT_DIR)/ve_sib.$(OBJ): $(IN_DIR)/ve_sib.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_sib.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_sib.c

$(OUT_DIR)/ve_tmr.$(OBJ): $(IN_DIR)/ve_tmr.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_tmr.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_tmr.c

$(OUT_DIR)/ve_utils.$(OBJ): $(IN_DIR)/ve_utils.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_utils.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_utils.c



$(OUT_DIR)/ve_sm_init.$(OBJ): $(IN_DIR)/ve_sm_init.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_sm_init.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_sm_init.c
$(OUT_DIR)/ve_sm_init_merged.$(OBJ): $(IN_DIR)/ve_sm_init_merged.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_sm_init_merged.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_sm_init_merged.c
$(OUT_DIR)/ve_sm_nh_cntrl.$(OBJ): $(IN_DIR)/ve_sm_nh_cntrl.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_sm_nh_cntrl.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_sm_nh_cntrl.c
$(OUT_DIR)/ve_sm_nh_exms.$(OBJ): $(IN_DIR)/ve_sm_nh_exms.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -ULCSMNHMILNH -DLCLNH $(IN_DIR)/ve_sm_nh_exms.c -o \
	$(OUT_DIR)/ve_sm_nh_exms.$(OBJ)
$(OUT_DIR)/ve_sm_nh_ptmi.$(OBJ): $(IN_DIR)/ve_sm_nh_ptmi.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -ULCSMNHMILNH -DLCLNH $(IN_DIR)/ve_sm_nh_ptmi.c -o \
	$(OUT_DIR)/ve_sm_nh_ptmi.$(OBJ)

$(OUT_DIR)/ve_sm_kw_cntrl.$(OBJ): $(IN_DIR)/ve_sm_kw_cntrl.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_sm_kw_cntrl.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_sm_kw_cntrl.c
$(OUT_DIR)/ve_sm_kw_exms.$(OBJ): $(IN_DIR)/ve_sm_kw_exms.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCKWMILKW -DLCPJMILPJ -DLCLKW -DLCLPJ $(IN_DIR)/ve_sm_kw_exms.c -o \
	$(OUT_DIR)/ve_sm_kw_exms.$(OBJ)
$(OUT_DIR)/ve_sm_kw_ptmi.$(OBJ): $(IN_DIR)/ve_sm_kw_ptmi.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCKWMILKW -DLCPJMILPJ -DLCLKW $(IN_DIR)/ve_sm_kw_ptmi.c -o \
	$(OUT_DIR)/ve_sm_kw_ptmi.$(OBJ)

$(OUT_DIR)/ve_sm_rg_cntrl.$(OBJ): $(IN_DIR)/ve_sm_rg_cntrl.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_sm_rg_cntrl.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_sm_rg_cntrl.c
$(OUT_DIR)/ve_sm_rg_exms.$(OBJ): $(IN_DIR)/ve_sm_rg_exms.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCRGMILRG -DLCLRG $(IN_DIR)/ve_sm_rg_exms.c -o \
	$(OUT_DIR)/ve_sm_rg_exms.$(OBJ)
$(OUT_DIR)/ve_sm_rg_ptmi.$(OBJ): $(IN_DIR)/ve_sm_rg_ptmi.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCSMMILRG -DLCRGMILRG -DLCLRG $(IN_DIR)/ve_sm_rg_ptmi.c -o \
	$(OUT_DIR)/ve_sm_rg_ptmi.$(OBJ)

$(OUT_DIR)/ve_sm_sz_cntrl.$(OBJ): $(IN_DIR)/ve_sm_sz_cntrl.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCSZMILSZ -DLCLSZ -DSZ_ENB $(IN_DIR)/ve_sm_sz_cntrl.c -o \
	$(OUT_DIR)/ve_sm_sz_cntrl.$(OBJ)

$(OUT_DIR)/ve_sm_sz_exms.$(OBJ): $(IN_DIR)/ve_sm_sz_exms.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCSZMILSZ -DLCLSZ -DSZ_ENB $(IN_DIR)/ve_sm_sz_exms.c -o \
	$(OUT_DIR)/ve_sm_sz_exms.$(OBJ)
$(OUT_DIR)/ve_sm_sz_ptmi.$(OBJ): $(IN_DIR)/ve_sm_sz_ptmi.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCSMSZMILSZ -DLCLSZ -DSZ_ENB $(IN_DIR)/ve_sm_sz_ptmi.c -o \
	$(OUT_DIR)/ve_sm_sz_ptmi.$(OBJ)

$(OUT_DIR)/ve_sm_sb_cntrl.$(OBJ): $(IN_DIR)/ve_sm_sb_cntrl.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_sm_sb_cntrl.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(SBOPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_sm_sb_cntrl.c
$(OUT_DIR)/ve_sm_sb_exms.$(OBJ): $(IN_DIR)/ve_sm_sb_exms.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(SBOPTS) $(CCveFLAGS) $(IN_DIR)/ve_sm_sb_exms.c -o \
	$(OUT_DIR)/ve_sm_sb_exms.$(OBJ)
$(OUT_DIR)/ve_sm_sb_ptmi.$(OBJ): $(IN_DIR)/ve_sm_sb_ptmi.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCSMSBMILSB -DLCLSB $(IN_DIR)/ve_sm_sb_ptmi.c -o \
	$(OUT_DIR)/ve_sm_sb_ptmi.$(OBJ)

$(OUT_DIR)/ve_sm_hi_cntrl.$(OBJ): $(IN_DIR)/ve_sm_hi_cntrl.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_sm_hi_cntrl.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_sm_hi_cntrl.c
$(OUT_DIR)/ve_sm_hi_exms.$(OBJ): $(IN_DIR)/ve_sm_hi_exms.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(HIOPTS) $(CCveFLAGS) $(IN_DIR)/ve_sm_hi_exms.c -o \
	$(OUT_DIR)/ve_sm_hi_exms.$(OBJ)

$(OUT_DIR)/ve_sm_hi_ptmi.$(OBJ): $(IN_DIR)/ve_sm_hi_ptmi.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCSMHIMILHI $(IN_DIR)/ve_sm_hi_ptmi.c -o \
	$(OUT_DIR)/ve_sm_hi_ptmi.$(OBJ)

$(OUT_DIR)/ve_sm_eg_cntrl.$(OBJ): $(IN_DIR)/ve_sm_eg_cntrl.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_sm_eg_cntrl.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_sm_eg_cntrl.c
$(OUT_DIR)/ve_sm_eg_exms.$(OBJ): $(IN_DIR)/ve_sm_eg_exms.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCEGMILEG -DLCLEG $(IN_DIR)/ve_sm_eg_exms.c -o \
	$(OUT_DIR)/ve_sm_eg_exms.$(OBJ)
$(OUT_DIR)/ve_sm_eg_ptmi.$(OBJ): $(IN_DIR)/ve_sm_eg_ptmi.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS)  -DLCSMEGMILEG -DLCLEG $(IN_DIR)/ve_sm_eg_ptmi.c -o \
	$(OUT_DIR)/ve_sm_eg_ptmi.$(OBJ)

$(OUT_DIR)/ve_sm_ve_cntrl.$(OBJ): $(IN_DIR)/ve_sm_ve_cntrl.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_sm_ve_cntrl.o -I$(SS_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_sm_ve_cntrl.c
$(OUT_DIR)/ve_sm_ve_exms.$(OBJ): $(IN_DIR)/ve_sm_ve_exms.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCVEMILVE -DLCLVE $(IN_DIR)/ve_sm_ve_exms.c -o \
	$(OUT_DIR)/ve_sm_ve_exms.$(OBJ)
$(OUT_DIR)/ve_sm_ve_ptmi.$(OBJ): $(IN_DIR)/ve_sm_ve_ptmi.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCSMVEMILVE -DLCLVE $(IN_DIR)/ve_sm_ve_ptmi.c -o \
	$(OUT_DIR)/ve_sm_ve_ptmi.$(OBJ)

$(OUT_DIR)/ve_sm_pj_cntrl.$(OBJ): $(IN_DIR)/ve_sm_pj_cntrl.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_sm_pj_cntrl.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_sm_pj_cntrl.c
$(OUT_DIR)/ve_sm_pj_exms.$(OBJ): $(IN_DIR)/ve_sm_pj_exms.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCSMPJMILPJ -DLCPJMILPJ -DLCLPJ $(IN_DIR)/ve_sm_pj_exms.c -o \
	$(OUT_DIR)/ve_sm_pj_exms.$(OBJ)
$(OUT_DIR)/ve_sm_pj_ptmi.$(OBJ): $(IN_DIR)/ve_sm_pj_ptmi.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCSMPJMILPJ -DLCPJMILPJ -DLCLPJ $(IN_DIR)/ve_sm_pj_ptmi.c -o \
	$(OUT_DIR)/ve_sm_pj_ptmi.$(OBJ)


$(OUT_DIR)/ve_sm_ys_cntrl.$(OBJ): $(IN_DIR)/ve_sm_ys_cntrl.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/ve_sm_ys_cntrl.o -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) \
	$(IN_DIR)/ve_sm_ys_cntrl.c

$(OUT_DIR)/ve_sm_ys_exms.$(OBJ): $(IN_DIR)/ve_sm_ys_exms.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/ve_sm_ys_exms.c -o \
	$(OUT_DIR)/ve_sm_ys_exms.$(OBJ)
$(OUT_DIR)/ve_sm_ys_ptmi.$(OBJ): $(IN_DIR)/ve_sm_ys_ptmi.c $(ALL_INC)
	$(CC) -c -I$(SS_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCveFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/ve_sm_ys_ptmi.c -o \
	$(OUT_DIR)/ve_sm_ys_ptmi.$(OBJ)


#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libperflnxve.a:$(vePerfLnxOBJS)
	ar -cr $(OUT_DIR)/libperflnxve.a $(vePerfLnxOBJS)	

$(OUT_DIR)/libperflnxveuu.a:$(vePerfLnxOBJSUU)
	ar -cr $(OUT_DIR)/libperflnxve.a $(vePerfLnxOBJSUU)	

$(OUT_DIR)/libe2elnxve.a:$(veE2eLnxOBJS)
	ar -cr $(OUT_DIR)/libe2elnxve.a $(veE2eLnxOBJS)	

$(OUT_DIR)/libperfcavve.a:$(vePerfCavOBJS)
	ar -cr $(OUT_DIR)/libperfcavve.a $(vePerfCavOBJS)	

$(OUT_DIR)/libperfcavveuu.a:$(vePerfCavOBJSUU)
	ar -cr $(OUT_DIR)/libperfcavve.a $(vePerfCavOBJSUU)	

$(OUT_DIR)/libe2ecavve.a:$(veE2eCavOBJS)
	ar -cr $(OUT_DIR)/libe2ecavve.a $(veE2eCavOBJS)	
#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libve.a $(veOBJS)


include make.opt
EXE               := all_exe
# File Listings
# Generated files from script
VERSION_H              := $(INC_DIR)/version.h
VERSION_X              := $(INC_DIR)/version.x

#The following files are generated to see the message names and module names in logging
MSG_NAMES_C            := $(SRC_DIR)/libimc/msg_names.c
MOD_NAMES_C            := $(SRC_DIR)/libimc/mod_names.c
TIMER_NAMES_C          := $(SRC_DIR)/libimc/timer_names.c
VERSION_C              := $(SRC_DIR)/libsvnversion/version.c
UEH_NAMES_C            := $(SRC_DIR)/cprm/ueh/ueh_names.c

#enumstr_bserr.c will be generated from bs_err.h
ENUM_TO_STR_BSERR_C     := $(SRC_DIR)/libenumstr/enumstr_bserr.c

#enumstr_modId.c will be generated from bs_modules.h
ENUM_TO_STR_MODID_C     := $(SRC_DIR)/libenumstr/enumstr_modid.c

# Framework Library: Used by ALL modules
IMC_SRC_FILES           := imc.c  imc_module.c mod_names.c imc_udp.c msg_names.c  
IMC_SRC_FILES           += imc_proc.c imc_util.c imc_sig.c imc_sched.c imc_timer.c imc_msg_desc.c timer_names.c 
IMC_SRC_FILES           += imc_cmd_arg.c imc_err.c imc_list.c imc_cmn_format.c imc_unix.c
IMC_OBJ_FILES           := $(patsubst %.c, $(OBJ_DIR)/%.o, $(IMC_SRC_FILES) )
IMC_LIB                 := $(LIB_DIR)/libimc.a
IMC_CFLAGS              := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/libimc/

# Logger Library: Used by ALL modules
LOG_SRC_FILES           := log.c shm.c 
LOG_OBJ_FILES           := $(patsubst %.c, $(OBJ_DIR)/%.o, $(LOG_SRC_FILES) )
LOG_LIB                 := $(LIB_DIR)/liblog.a
LOG_CFLAGS              := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/liblog/

# svnversion Library: Used by ALL modules
SVNVERSION_SRC_FILES       := version.c start_indication.c
SVNVERSION_OBJ_FILES       := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SVNVERSION_SRC_FILES) )
SVNVERSION_LIB             := $(LIB_DIR)/libsvnversion.a
SVNVERSION_CFLAGS          := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/libsvnversion/

# LOGUTIL Module 
LOGUTIL_SRC_FILES           := logutil.c shm.c 
LOGUTIL_OBJ_FILES           := $(patsubst %.c, $(OBJ_DIR)/%.o, $(LOGUTIL_SRC_FILES))
LOGUTIL_CFLAGS              := $(CFLAGS) $(CMN_CFLAGS_OAM)  $(C_INC_DIR) -I $(SRC_DIR)/logutil/
LOGUTIL_LFLAGS              := -lpthread
LOGUTIL_MAIN_EXE            := $(BIN_DIR)/logutil.exe

# GTPUTIL Module 
GTPUTIL_SRC_FILES           := gtputil.c
GTPUTIL_OBJ_FILES           := $(patsubst %.c, $(OBJ_DIR)/%.o, $(GTPUTIL_SRC_FILES))
#GTPUTIL_LFLAGS	             := -limc
#GTPUTIL_LFLAGS              := -L$(LIB_DIR) -limc -lcfg 
GTPUTIL_CFLAGS              := $(CFLAGS) $(C_INC_DIR) -I $(ROOT_DIR)/../tools/gtputil/
GTPUTIL_MAIN_EXE            := $(BIN_DIR)/gtputil.exe

# Platform Library: Used by DEH module  
LIB_PLATFORM_SRC_FILES      := lte_bb_bist.c lte_bb_eth_mdio.c lte_bb_gps_util.c lte_bb_nor_util.c lte_bb_platform.c lte_bb_spi_api.c lte_bb_version.c lte_bb_switch.c
LIB_PLATFORM_OBJ_FILES      := $(patsubst %.c, $(OBJ_DIR)/%.o, $(LIB_PLATFORM_SRC_FILES))
LIB_PLATFORM_CFLAGS         := $(CFLAGS) $(CMN_CFLAGS_OAM)  $(C_INC_DIR) -I $(SRC_DIR)/$(LIB_PF_SRC)
LIB_PLATFORM_LIB            := $(LIB_DIR)/libplatform.a


# For Framework Testing Buffer Testing
IMC_MT_SRC_FILES        := imc_ut.c imc_buffer.c imc_queue.c
IMC_MT_OBJ_FILES        := $(patsubst %.c, $(OBJ_DIR)/%.o, $(IMC_MT_SRC_FILES))
IMC_MT_EXE              := $(BIN_DIR)/imc_mt.exe

#libCfg : Useb by all modules
LIB_CFG_SRC_FILES       := oam_cfg_table.c oper_config_read_write.c pre_config_read_write.c counter_write_functions.c  
LIB_CFG_SRC_FILES       += fth_cfg_table.c version_cfg_table.c enb_limits_cfg_table.c alarm_db.c
LIB_CFG_SRC_FILES       += process_start_cfg_table.c module_stop_cfg_table.c sw_package_table.c  
LIB_CFG_SRC_FILES       += pm_log_file_table.c imc_cfg_read.c gtp_util_config_read.c
LIB_CFG_OBJ_FILES       := $(patsubst %.c, $(OBJ_DIR)/%.o, $(LIB_CFG_SRC_FILES) )
LIB_CFG_LIB             := $(LIB_DIR)/libcfg.a
LIB_CFG_CFLAGS          := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/libCfg/ -I $(SRC_DIR)/prm/ -I $(SRC_DIR)/bsm/ -I $(SRC_DIR)/deh/ -I $(SRC_DIR)/$(LIB_PF_SRC)

#  RRM Message Print:Source Files to Print messages
RRM_STACK_API_PRINT_FILES := Trace_RRM_RRC.c Trace_RRM_MAC.c Trace_RRM_PHY.c Trace_RRM_S1AP.c 
RRM_PRINT_OBJ_FILES     := $(patsubst %.c, $(OBJ_DIR)/%.o, $(RRM_STACK_API_PRINT_FILES))
RRM_PRINT_LIB_NAME      := RRMprint
RRM_PRINT_LIB           := $(LIB_DIR)/lib$(RRM_PRINT_LIB_NAME).a
RRM_PRINT_CFLAGS        := $(CFLAGS) $(CCLNXMTOPTS) $(C_INC_DIR) -I $(LNXIOPTS) -I $(MT_DIR)
#RRM_PRINT_CFLAGS        := $(CFLAGS) $(LNXENV) $(C_INC_DIR) $(CMENBE2EOPTS) $(MTOPTS) -I $(LNXIOPTS) -I $(MT_DIR)

#libtlv used by CIH, SIH, WIH and BSM
LIB_TLV_OAM_SRC_FILES   := tlv_dec.c tlv_enc.c basic_dec.c basic_enc.c 
LIB_TLV_OAM_OBJ_FILES   := $(patsubst %.c, $(OBJ_DIR)/%.o, $(LIB_TLV_OAM_SRC_FILES) )
LIB_TLV_OAM_LIB         := $(LIB_DIR)/libtlv.a
LIB_TLV_OAM_CFLAGS      := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/libtlv/ 

	
#libenumstr used by CIH, SIH  
LIB_ENUM_TO_STR_SRC_FILES   := enumstr_oamtables.c enumstr_notimsg.c enumstr_bserr.c enumstr_modid.c enumstr_counterIds.c
LIB_ENUM_TO_STR_OBJ_FILES   := $(patsubst %.c, $(OBJ_DIR)/%.o, $(LIB_ENUM_TO_STR_SRC_FILES) )
LIB_ENUM_TO_STR_LIB         := $(LIB_DIR)/libenumstr.a
LIB_ENUM_TO_STR_CFLAGS      := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/libenumstr/ 

#library - OAM table related all functions. 
LIB_TABLE_SRC_FILES   := generic_table.c init_tables.c empty_tables.c fill_tlv_func.c get_opr_cmd_func.c get_status_func.c 
LIB_TABLE_SRC_FILES   += get_opr_cfg_func.c get_advanced_cfg_func.c 
LIB_TABLE_SRC_FILES   += set_opr_cmd_func.c set_opr_cfg_func.c set_advanced_cfg_func.c
LIB_TABLE_SRC_FILES   += noti_msg_get_set.c
LIB_TABLE_OBJ_FILES   := $(patsubst %.c, $(OBJ_DIR)/%.o, $(LIB_TABLE_SRC_FILES) )
LIB_TABLE_LIB         := $(LIB_DIR)/libtable.a
LIB_TABLE_CFLAGS      := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/libtable/ 

# LTH Module 
LTH_SRC_FILES           := lth.c lth_main.c lth_func.c 
LTH_OBJ_FILES           := $(patsubst %.c, $(LTH_OBJ_DIR)/%.o, $(LTH_SRC_FILES))
LTH_CFLAGS              := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/lth/
LTH_MAIN_EXE            := $(BIN_DIR)/lth.exe

# PMH Module 
PMH_SRC_FILES           := pmh.c pmh_main.c pmh_cmn.c pmh_timers.c pmh_get_table.c pmh_mac_if.c pmh_l2_if.c
PMH_SRC_FILES           += pmh_gtp_if.c pmh_sctp_if.c pmh_s1ap_if.c pmh_rrc_if.c pmh_rlc_if.c pmh_pdcp_if.c 
PMH_SRC_FILES           += pmh_ethernet_if.c pmh_ip_if.c pmh_rrm_if.c pmh_tfu_if.c pmh_set_table.c
PMH_OBJ_FILES           := $(patsubst %.c, $(PMH_OBJ_DIR)/%.o, $(PMH_SRC_FILES))
PMH_CFLAGS              := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/pmh/
PMH_LFLAGS              := $(LFLAGS)
PMH_MAIN_EXE            := $(BIN_DIR)/pmh.exe

# WIH Module 
WIH_SRC_FILES           := wih_oamInterface.cpp wih_webResponse.cpp wih_loginCmdSession.cpp wih_getCmdSession.cpp wih_setCmdSession.cpp
WIH_SRC_FILES           += wih_cmdSession.cpp wih_sessionMgr.cpp wih_commandInfo.cpp wih_wsInterface.cpp wih_main.cpp wih.cpp
WIH_OBJ_FILES           := $(patsubst %.cpp, $(WIH_OBJ_DIR)/%.o, $(WIH_SRC_FILES))
WIH_CFLAGS              := -c -g -Wall -Wextra -Wundef -Winline -fPIC $(CMN_CFLAGS_OAM) $(C_INC_DIR) 
WIH_CFLAGS              += -I $(SRC_DIR)/wih/
WIH_LFLAGS              := $(LFLAGS) -lfcgi -lpthread -lcrypt 
WIH_MAIN_EXE            := $(BIN_DIR)/wih.exe
ifeq ($(ENB_SUPPORT_CODE_COVERAGE),true)
  WIH_CFLAGS            += -ftest-coverage -fprofile-arcs 
endif

# BSM Module 
BSM_SRC_FILES  := bsm.c bsm_main.c bsm_read_write_cfg.c bsm_init_exit.c bsm_timers.c bsm_consistency_check.c
BSM_SRC_FILES  += bsm_cmn_func.c bsm_device_init.c bsm_download_upload.c bsm_sw_activate.c
BSM_SRC_FILES  += bsm_cmd_set.c bsm_cmd_get.c bsm_oper_cmd_table.c  bsm_set_opr_cfg_table.c bsm_set_pre_cfg_table.c 
BSM_SRC_FILES  += bsm_cmd_reboot.c bsm_cmd_restart.c bsm_health_check.c
BSM_SRC_FILES  += bsm_cmd_unlock.c bsm_cmd_lock.c bsm_enb_init_cfg.c bsm_enb_cfg.c bsm_sw_install.c
BSM_SRC_FILES  += bsm_alarm_func.c bsm_event_func.c bsm_cell_bar_unbar.c 
BSM_SRC_FILES  += bsm_cmd_save_rollback_cfg.c bsm_ems_reg.c bsm_counters.c

BSM_OBJ_FILES           := $(patsubst %.c, $(BSM_OBJ_DIR)/%.o, $(BSM_SRC_FILES))
BSM_CFLAGS              := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/bsm/
ifeq ($(TARGET),x86)
BSM_CFLAGS              += -DHOST_ENV
endif
BSM_MAIN_EXE            := $(BIN_DIR)/bsm.exe

#DEH Module
DEH_SRC_FILES           := deh.c deh_main.c deh_cmn_func.c deh_timers.c rh_enc.c rh_dec.c deh_gps.c deh_led.c
DEH_SRC_FILES           += deh_get_set_func.c deh_rh_fillFunc.c deh_rh_func.c deh_init.c rh_msgHandlers.c
DEH_SRC_FILES           += deh_board.c deh_ethernet.c deh_led.c deh_tempsensor.c deh_clocks.c
DEH_OBJ_FILES           := $(patsubst %.c, $(DEH_OBJ_DIR)/%.o, $(DEH_SRC_FILES))
DEH_CFLAGS              := $(CFLAGS) $(CMN_CFLAGS_OAM)  $(C_INC_DIR) -I $(SRC_DIR)/deh
DEH_LFLAGS              := $(LFLAGS) -lplatform -lpthread
DEH_MAIN_EXE            := $(BIN_DIR)/deh.exe


# RH_SIM Module
RH_SIM_SRC_FILES        := rh_sim.c rh_sim_main.c
RH_SIM_OBJ_FILES        := $(patsubst %.c, $(OBJ_DIR)/%.o, $(RH_SIM_SRC_FILES))
RH_SIM_CFLAGS           := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/rh_sim -I $(SRC_DIR)/deh -I $(SRC_DIR)/inc 
RH_SIM_MAIN_EXE         := $(BIN_DIR)/rh_sim.exe

# EMS_SIM Module
EMS_SIM_SRC_FILES        := ems_sim.c 
EMS_SIM_OBJ_FILES        := $(patsubst %.c, $(OBJ_DIR)/%.o, $(EMS_SIM_SRC_FILES))
EMS_SIM_CFLAGS           := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/ems_sim
EMS_SIM_MAIN_EXE         := $(BIN_DIR)/ems_sim.exe

# L2SIM Module
L2SIM_SRC_FILES       := l2sim.c kw_ex_ms.c kw_pj_lmm.c kw_uim.c lkw.c rg_lmm.c rg_sch.c rg_uim.c ys_ex_ms.c ys_ms_ui.c
L2SIM_SRC_FILES       += kw_lmm.c kw_pj_uim.c lpj.c lrg.c rg_ex_ms.c rg_ptmi.c rg_sch_ex_ms.c ys_mi.c ys_ptmi.c 
L2SIM_SRC_FILES       += kw_lim.c kw_id.c rg_id.c
L2SIM_SRC_FILES       += kw_ptui.c kw_ptmi.c rg_cfg.c rg_lim.c rg_utl.c rg_sch_lmm.c rg_sch_utl.c ys_ptui.c kw_tmr.c
L2SIM_SRC_FILES       += kw_ptli.c rg_ptui.c rg_ptli.c ys_init.c ys_utl.c ys_id.c ys_ms_li.c
L2SIM_OBJ_FILES       := $(patsubst %.c, $(L2SIM_OBJ_DIR)/%.o, $(L2SIM_SRC_FILES))
L2SIM_CFLAGS          := $(CFLAGS) $(C_INC_DIR) -I $(SRC_DIR)/l2sim $(LNXIOPTS) $(LNXENV) $(RGOPTS) $(CLOPTS) $(STOPTS) $(RGROPTS) $(CMOPTS) -DPWAV_L2SIM -DKW_PDCP -DPWAV -DYS_MSPD -DPWAV_HC -DYS  -DLCYSUICTF -DLCYSMILYS -DCTFOPTS -DUEHOPTS -DTF -DLWLCRGLITFU -DLCYSUITFU -DLWLCYSUITFU $(L2SIM_UEHLCOPTS) $(KWOPTS) -I $(SRC_DIR)/inc 

# CIH Module
CIH_SRC_FILES           := cih.c cih_func.c cih_tecla.c  cih_help.c  cih_display.c  cih_timers.c
CIH_SRC_FILES           += cih_oamtable_func.c cih_cmd_rsp_proc.c
CIH_OBJ_FILES           := $(patsubst %.c, $(CIH_OBJ_DIR)/%.o, $(CIH_SRC_FILES))
CIH_CFLAGS              := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/cih
CIH_LFLAGS              := $(LFLAGS) -ltecla  
CIH_MAIN_EXE            := $(BIN_DIR)/cih.exe


# FTH Module 
FTH_SRC_FILES           := fth_download_upload.c fth_msg_hand.c fth_main.c fth.c fth_cmn.c fth_get_tables.c fth_timers.c
FTH_OBJ_FILES           := $(patsubst %.c, $(FTH_OBJ_DIR)/%.o, $(FTH_SRC_FILES))
FTH_CFLAGS              := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(INC_DIR)/libcurl -I $(SRC_DIR)/fth/
FTH_LFLAGS              := $(LFLAGS) -lcurl -lrt
ifeq ($(FTH_SUPPORT_SECURE_TRANSFER),true)
 FTH_LFLAGS            += -lssh2 -lssl -lcrypto -lz -ldl
 FTH_CFLAGS            += -D FTH_SUPPORT_SECURE_TRANSFER
endif
FTH_MAIN_EXE            := $(BIN_DIR)/fth.exe

# IMC module
IMCS_SRC_FILES          := imcs_main.c imcs.c imcs_func.c
IMCS_OBJ_FILES          := $(patsubst %.c, $(OBJ_DIR)/%.o, $(IMCS_SRC_FILES))
IMCS_CFLAGS             := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/imcs/
IMCS_MAIN_EXE           := $(BIN_DIR)/imcs.exe

# IMC - Module Testing
IMCS_MT_SRC_FILES       := imcs_mt.c
IMCS_MT_OBJ_FILES       := $(patsubst %.c, $(OBJ_DIR)/%.o, $(IMCS_MT_SRC_FILES))
IMCS_CFLAGS             := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/imcs 
IMCS_MT_EXE             := $(BIN_DIR)/imcs_mt.exe

# PRM Module 
PRM_SRC_FILES           := prm.c prm_main.c prm_cmn.c prm_read_write_cfg.c prm_init.c prm_exit.c 
PRM_SRC_FILES           += prm_timers.c prm_hc.c prm_restart.c prm_reboot.c prm_sw_act.c   
PRM_SRC_FILES           += prm_get_table.c prm_msg_trace.c prm_sw_install.c 
PRM_OBJ_FILES           := $(patsubst %.c, $(PRM_OBJ_DIR)/%.o, $(PRM_SRC_FILES))
PRM_CFLAGS              := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/prm/
ifeq ($(TARGET),arm)
PRM_CFLAGS              += -DARM
endif
PRM_MAIN_EXE            := $(BIN_DIR)/prm.exe

# UEH Module
UEH_SRC_FILES           := ueh_main.c ueh.c
UEH_OBJ_FILES           := $(patsubst %.c, $(OBJ_DIR)/%.o, $(UEH_SRC_FILES))
UEH_CFLAGS              := $(CFLAGS) $(C_INC_DIR) -I $(SRC_DIR)/ueh/
UEH_MAIN_EXE            := $(BIN_DIR)/ueh.exe

# CPRM MT Module 
CPRMMT_SRC_FILES           := cprm_mt.c
CPRMMT_OBJ_FILES           := $(patsubst %.c, $(OBJ_DIR)/%.o, $(CPRMMT_SRC_FILES))
CPRMMT_CFLAGS              := -c -g -Wall -Wextra -Wundef -Waggregate-return -Winline -Wunreachable-code -fPIC $(C_INC_DIR) -I$(SRC_DIR)/bsm
CPRMMT_LFLAGS              := $(LFLAGS) 
CPRMMT_MAIN_EXE            := $(BIN_DIR)/cprm_mt.exe


# Message Print : Source Files to Print messages
MSG_PRINT_SRC_FILES     := msg_desc.c print_tlv.c print_noti.c print_cmn.c bsm_fth_msg_print.c  
MSG_PRINT_SRC_FILES     += bsm_prm_msg_print.c   bsm_egtp_msg_print.c bsm_lth_msg_print.c 
MSG_PRINT_SRC_FILES     += bsm_s1ap_msg_print.c  bsm_sctp_msg_print.c bsm_ueh_msg_print.c  
MSG_PRINT_SRC_FILES     += bsm_deh_msg_print.c   bsm_sih_msg_print.c  deh_rh_msg_print.c    
MSG_PRINT_SRC_FILES     += pmh_rlc_msg_print.c   pmh_gtp_msg_print.c pmh_mac_msg_print.c 
MSG_PRINT_SRC_FILES     += bsm_pmh_msg_print.c   pmh_pdcp_msg_print.c pmh_sctp_msg_print.c
MSG_PRINT_SRC_FILES     += pmh_rrc_msg_print.c   bsm_udh_msg_print.c  pmh_s1ap_msg_print.c
MSG_PRINT_SRC_FILES     += pmh_ueh_msg_print.c   pmh_udh_print.c      prm_deh_msg_print.c
MSG_PRINT_OBJ_FILES     := $(patsubst %.c, $(OBJ_DIR)/%.o, $(MSG_PRINT_SRC_FILES))
MSG_PRINT_LIB_NAME      := print
MSG_PRINT_LIB           := $(LIB_DIR)/lib$(MSG_PRINT_LIB_NAME).a
MSG_PRINT_CFLAGS        := $(CFLAGS) $(CMN_CFLAGS_OAM) $(C_INC_DIR) -I $(SRC_DIR)/tput_tester 

SSR_OBJ_FILE            := ipc.o
# Make rules
.PHONY:  doc tools 

all: 4gmx
#all:        x86 arm 4gmx

x86: 
	$(MAKE) -f Makefile $(EXE) TARGET=x86 
arm:
	$(MAKE) -f Makefile $(EXE) TARGET=arm
4gmx: 
	$(MAKE) -f Makefile $(EXE) TARGET=4gmx

all_exe:    tools log imc svnversion libs simulators bins cpup
libs:       libcfg libtlv libRRMprint libprint libenumstr libtable libplatform
bins:       prm bsm cih deh lth pmh fth logutil wih sih 
simulators: ems_sim rh_sim


turner:
	cd $(MS_PATH) && make && rm -f main.o && cd - && mv $(MS_PATH)/BSP/LINUX/libbsp.a $(LIB_DIR) && mv $(MS_PATH)/OSAL/LINUX/libosal.a $(LIB_DIR);

tools:
ifeq ($(TARGET),arm)
	cd ../tools && test -d CodeSourcery || \
	tar -xvzf CodeSourcery.tgz
endif
ifeq ($(TARGET),4gmx)
	cd ../tools && test -d CodeSourcery || \
	tar -xvzf CodeSourcery.tgz
endif

log: $(LOG_LIB)
$(LOG_LIB): $(LOG_OBJ_FILES)
	$(V) $(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/liblog/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(LOG_CFLAGS) -o $@ $<
	
imc: $(IMC_LIB)
$(IMC_LIB): $(IMC_OBJ_FILES)
	$(V) $(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/libimc/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(IMC_CFLAGS) -o $@ $<

$(OBJ_DIR)/msg_names.o: $(MSG_NAMES_C) 
$(OBJ_DIR)/mod_names.o: $(MOD_NAMES_C)
$(OBJ_DIR)/timer_names.o: $(TIMER_NAMES_C) 

# To Generate version library
svnversion: $(SVNVERSION_LIB)

# Generate version.c file
$(VERSION_C): $(VERSION_X) 
	perl $(ROOT_DIR)/build/scripts/gen_module_version.pl $< > $@
$(VERSION_X):
	perl $(ROOT_DIR)/build/scripts/gen_module_version_def.pl $< > $@

$(SVNVERSION_LIB): $(SVNVERSION_OBJ_FILES)
	$(V) $(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/libsvnversion/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(SVNVERSION_CFLAGS) -o $@ $<

# Generate Libraries
libcfg: $(LIB_CFG_LIB)
$(LIB_CFG_LIB): $(LIB_CFG_OBJ_FILES)
	$(V) $(AR) $(ARFLAGS) $@ $^		

$(OBJ_DIR)/%.o: $(SRC_DIR)/libcfg/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(LIB_CFG_CFLAGS) -o $@ $<
	
libtlv: $(LIB_TLV_OAM_LIB)
$(LIB_TLV_OAM_LIB): $(LIB_TLV_OAM_OBJ_FILES)
	$(V) $(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/libtlv/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(LIB_TLV_OAM_CFLAGS) -o $@ $<
	
# To Generate Message Printing Library
libprint: $(MSG_PRINT_LIB)
$(MSG_PRINT_LIB): $(MSG_PRINT_OBJ_FILES)
	$(V) $(AR) $(ARFLAGS) $@ $^	

$(OBJ_DIR)/%.o: $(SRC_DIR)/libprint/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(MSG_PRINT_CFLAGS) -o $@ $<
	
# To Generate RRM_Message Printing Library
libRRMprint: $(RRM_PRINT_LIB)
$(RRM_PRINT_LIB): $(RRM_PRINT_OBJ_FILES)
	$(V) $(AR) $(ARFLAGS) $@ $^	

$(OBJ_DIR)/%.o: $(SRC_DIR)/libRRMprint/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(RRM_PRINT_CFLAGS) -o $@ $<
	
libenumstr: $(LIB_ENUM_TO_STR_LIB)
$(LIB_ENUM_TO_STR_LIB): $(LIB_ENUM_TO_STR_OBJ_FILES)
	$(AR) $(ARFLAGS) $@ $^
	
$(ENUM_TO_STR_BSERR_C): $(INC_DIR)/bs_err.h
	perl $(ROOT_DIR)/build/scripts/gen_bs_err_string.pl $< > $@
	
$(ENUM_TO_STR_MODID_C): $(INC_DIR)/bs_modules.h
	perl $(ROOT_DIR)/build/scripts/gen_modId_string.pl $< > $@
	 
$(OBJ_DIR)/enumstr_bserr.o: $(ENUM_TO_STR_BSERR_C)
$(OBJ_DIR)/enumstr_modid.o: $(ENUM_TO_STR_MODID_C)

$(OBJ_DIR)/%.o: $(SRC_DIR)/libenumstr/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(LIB_ENUM_TO_STR_CFLAGS) -o $@ $<
		
libtable: $(LIB_TABLE_LIB)
$(LIB_TABLE_LIB): $(LIB_TABLE_OBJ_FILES)
	$(V) $(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/libtable/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(LIB_TABLE_CFLAGS) -o $@ $<
		
libplatform: $(LIB_PLATFORM_LIB)
$(LIB_PLATFORM_LIB): $(LIB_PLATFORM_OBJ_FILES)
	$(V) $(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/$(LIB_PF_SRC)/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(LIB_PLATFORM_CFLAGS) -o $@ $<
	
# BINS
# PRM Wrapper
prm: CREATE_PRM_DIR $(PRM_MAIN_EXE)

CREATE_PRM_DIR: 
	mkdir -p $(PRM_OBJ_DIR)

$(PRM_MAIN_EXE): $(PRM_OBJ_FILES) $(COMMON_LIB_FILES)
	$(CC) $^ -o $@  $(LFLAGS)

$(PRM_OBJ_DIR)/%.o: $(SRC_DIR)/prm/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(PRM_CFLAGS) -o $@ $<	

# BSM Module Executable
bsm: CREATE_BSM_DIR $(BSM_MAIN_EXE)

CREATE_BSM_DIR: 
	mkdir -p $(BSM_OBJ_DIR)

$(BSM_MAIN_EXE): $(BSM_OBJ_FILES) $(COMMON_LIB_FILES)  
	$(CC) $^ -o $@ $(LFLAGS)    

$(BSM_OBJ_DIR)/%.o: $(SRC_DIR)/bsm/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(BSM_CFLAGS) -o $@ $<

#CIH Module
cih: CREATE_CIH_DIR $(CIH_MAIN_EXE)

CREATE_CIH_DIR: 
	mkdir -p $(CIH_OBJ_DIR)

$(CIH_MAIN_EXE): $(CIH_OBJ_FILES) $(COMMON_LIB_FILES)
	$(CC) $^ -o $@ $(CIH_LFLAGS)

$(CIH_OBJ_DIR)/%.o: $(SRC_DIR)/cih/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(CIH_CFLAGS) $< -o $@

# Device Handler
deh: CREATE_DEH_DIR $(DEH_MAIN_EXE)
CREATE_DEH_DIR: 
	mkdir -p $(DEH_OBJ_DIR)

$(DEH_MAIN_EXE): $(DEH_OBJ_FILES) $(COMMON_LIB_FILES) $(LIB_PLATFORM_LIB)
	$(CC) $^ -o $@ $(DEH_LFLAGS)
$(DEH_OBJ_DIR)/%.o: $(SRC_DIR)/deh/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(DEH_CFLAGS) -o $@ $<
	
# LTH Module Executable
lth: CREATE_LTH_DIR $(LTH_MAIN_EXE) 
CREATE_LTH_DIR: 
	mkdir -p $(LTH_OBJ_DIR)

$(LTH_MAIN_EXE): $(LTH_OBJ_FILES) $(COMMON_LIB_FILES)
	$(CC) $^ -o $@ $(LFLAGS)

$(LTH_OBJ_DIR)/%.o: $(SRC_DIR)/lth/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(LTH_CFLAGS) -o $@ $<
	
# PMH Module Executable
pmh: CREATE_PMH_DIR $(PMH_MAIN_EXE) 
CREATE_PMH_DIR: 
	mkdir -p $(PMH_OBJ_DIR)

$(PMH_MAIN_EXE): $(PMH_OBJ_FILES) $(COMMON_LIB_FILES)
	$(CC) $^ -o $@ $(PMH_LFLAGS)

$(PMH_OBJ_DIR)/%.o: $(SRC_DIR)/pmh/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(PMH_CFLAGS) -o $@ $<

# FTH Module
fth: CREATE_FTH_DIR $(FTH_MAIN_EXE)
CREATE_FTH_DIR: 
	mkdir -p $(FTH_OBJ_DIR)

$(FTH_MAIN_EXE): $(FTH_OBJ_FILES) $(COMMON_LIB_FILES)
	$(CC) $^ -o $@  $(FTH_LFLAGS)

$(FTH_OBJ_DIR)/%.o: $(SRC_DIR)/fth/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(FTH_CFLAGS) -o $@ $<

# GTPUTIL Module Executable
gtputil: $(GTPUTIL_MAIN_EXE)

$(GTPUTIL_MAIN_EXE): $(GTPUTIL_OBJ_FILES) $(COMMON_LIB_FILES)
	$(CC) $^ -o $@ $(LFLAGS)

$(OBJ_DIR)/%.o: $(ROOT_DIR)/../tools/gtputil/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(GTPUTIL_CFLAGS) -o $@ $<

# LOGUTIL Module Executable
logutil: $(LOGUTIL_MAIN_EXE)

$(LOGUTIL_MAIN_EXE): $(LOGUTIL_OBJ_FILES)  
	$(CC) $^ -o $@ $(LOGUTIL_LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/logutil/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(LOGUTIL_CFLAGS) -o $@ $<

# WIH Module Executable
wih: CREATE_WIH_DIR $(WIH_MAIN_EXE) 
CREATE_WIH_DIR: 
	mkdir -p $(WIH_OBJ_DIR)

$(WIH_MAIN_EXE): $(COMMON_LIB_FILES) $(WIH_OBJ_FILES)
	$(CXX) $^ -o $@ $(WIH_LFLAGS)

$(WIH_OBJ_DIR)/%.o: $(SRC_DIR)/wih/%.cpp
	@echo $(BUILD_PREFIX) $@
	$(V) $(CXX) $(WIH_CFLAGS) -o $@ $<
	
# Copy the webserver and spawn-fcgi executables
	cp -f $(SRC_DIR)/wih/websvr/$(TARGET)/* $(BIN_DIR)/.

# SNMP/SIH Module: This module is having its own compilation flags
################################################################################
ifeq ($(TARGET),arm)
ARCH-OS-TYPE  := arm-$(OS)
else ifeq ($(TARGET),4gmx)
ARCH-OS-TYPE  := arm-$(OS)
else
ARCH-OS-TYPE  := i686-pc-$(OS)
endif
export CFLAGS :=  -DPWAV_SIH
#export LDFLAGS  += -L$(LIB_DIR)
export LIBS  += -limc -llog -lsvnversion -lprint -lenumstr -ltlv -lcheck  -ltable -lconfig -lcfg -lpthread
export  CPPFLAGS  +=  $(C_INC_DIR)

ifeq ($(ENB_SUPPORT_CODE_COVERAGE),true)
  export LIBS  += -lgcov
endif

#SIH Module
sih: $(COMMON_LIB_FILES) net-snmp-build sih-build

net-snmp-build:
	#Using build.$(TARGET) file to check the previous build type	
	if [ -e $(SNMP_DIR)/build.$(TARGET) -a -e $(SNMP_DIR)/config.status ] ; then echo Previous build for $(TARGET) exists; else cd $(SNMP_DIR); make clean; $(RM) -f build.x86 build.4gmx build.arm config.status; touch build.$(TARGET); touch $(ROOT_DIR)/build/log/snmpd.log ; chmod +x configure; sh configure --build=$(BUILD-ARCH-OS-TYPE) --host=$(ARCH-OS-TYPE) --target=$(ARCH-OS-TYPE) --with-cc=$(CC) --with-linkcc=$(CC) --with-ar=$(AR) --with-default-snmp-version=$(SNMP-DEFAULT-VERSION) --with-sys-contact=$(SNMP-SYS-CONTACT) --with-sys-location= $(SNMP-SYS-LOCATION) --with-logfile=$(SNMP-DEFAULT-LOGFILE-LOCATION) --with-persistent-directory=$(SNMP-DEFAULT-PERSISTENT-DIR-LOCATION) --with-defaults --with-mib-modules=$(SNMP-MODULES) --enable-shared=no --enable-static=yes --prefix=$(ROOT_DIR)/build/$(TARGET) --disable-option-checking --enable-silent-libtool --disable-manuals --quiet --disable-applications --disable-scripts --with-ldflags='-L$(LIB_DIR)'; cd -; fi 
		
sih-build:
	cd $(SNMP_DIR) && chmod 755 config.status && make && cd - ;
	cp -f $(SNMP_DIR)/agent/snmpd $(BIN_DIR)/sih.exe

################################################################################

#SCRIPTS
# Generate files for good logging
$(MSG_NAMES_C): $(INC_DIR)/msg_ids.h
	perl $(ROOT_DIR)/build/scripts/gen_msg_name.pl $< > $@
 
$(MOD_NAMES_C): $(INC_DIR)/bs_modules.h
	perl $(ROOT_DIR)/build/scripts/gen_mod_name.pl $< > $@

$(TIMER_NAMES_C): $(INC_DIR)/bs_tables.h
	perl $(ROOT_DIR)/build/scripts/gen_timer_name.pl $< > $@
	
# Framework testing
imc_mt: $(CHECK_LIB) $(IMC_MT_EXE)

$(IMC_MT_EXE): $(IMC_MT_OBJ_FILES)
	$(CC) $^ -o $@ $(IMC_MT_LFLAGS)
	
#SIMULATORS		
# CPRMMT Module Executable
cprm_mt: $(CPRMMT_MAIN_EXE) 

$(CPRMMT_MAIN_EXE): $(CPRMMT_OBJ_FILES)
	$(CC) $^ -o $@ $(CPRMMT_LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/cprm/cprm_mt/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(CPRMMT_CFLAGS) -o $@ $<
	
# EMS Simulator Module
ems_sim: $(EMS_SIM_MAIN_EXE)

$(EMS_SIM_MAIN_EXE): $(EMS_SIM_OBJ_FILES) $(COMMON_LIB_FILES)
	$(CC) $^ -o $@  $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/ems_sim/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(EMS_SIM_CFLAGS) -o $@ $<
	
# Copy the webserver and spawn-fcgi executables
	cp -f $(SRC_DIR)/ems_sim/utils/$(TARGET)/* $(BIN_DIR)/.

# RH Simulator Module
rh_sim: $(RH_SIM_MAIN_EXE)

$(RH_SIM_MAIN_EXE): $(RH_SIM_OBJ_FILES) $(COMMON_LIB_FILES)
	$(CC) $^ -o $@  $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/rh_sim/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(RH_SIM_CFLAGS) -o $@ $<

#rules to make objs
CPUP_CMN_OBJS: cm sfi

ENB_GTP_OBJS:  turner mt_gtp ssr_gtp tucl udh egtp
    
ENB_CP_OBJS: mt_cp ssr_cp rrc s1ap sctp sm ueh
    
ENB_RADIOUP_OBJS: mt_gtp ssr_gtp ys kw rg sm ueh phy


ifeq ($(TARGET), 4gmx)      
CPUP_EXE = cp_exe gtp_exe 
else
CPUP_EXE = cp_exe gtp_exe radioup
endif

cpup: $(CPUP_EXE)

cp:  $(COMMON_LIB_FILES) CPUP_CMN_OBJS ENB_CP_OBJS  $(LNXENBE2EBINCP)
gtp: $(COMMON_LIB_FILES) CPUP_CMN_OBJS  ENB_GTP_OBJS    $(LNXENBE2EBINGTP)  
radioup: $(COMMON_LIB_FILES) CPUP_CMN_OBJS ENB_RADIOUP_OBJS $(LNXENBE2EBINRADIOUP)  
cp_exe:
	make -f Makefile cp EXE=cp  TARGET=$(TARGET)
gtp_exe: 
	make -f Makefile gtp EXE=gtp TARGET=$(TARGET)

# Data Plane EXE -- gtp.exe
GTP_CMN_OBJS := $(UDH_OBJ_DIR)/gtp_main.o $(UDH_OBJ_DIR)/rrm_sfi_util.o $(UDH_OBJ_DIR)/udh_func.o
GTP_CMN_OBJS += $(UDH_OBJ_DIR)/udh_lm.o  $(UDH_OBJ_DIR)/udh_main.o $(SSR_GTP_OBJ_DIR)/$(SSR_OBJ_FILE) 

ifeq ($(TARGET), 4gmx)
GTP_OBJS  =  $(GTP_CMN_OBJS) $(MS_TP_DIR)/*.o $(LNXMSLIBPATH) -L$(LIB_DIR) $(LNXMSLOPTS) $(LNXLIBPATH)	-lsfi -leg -lhi -lmt_gtp -lcm -lbsp
else
GTP_OBJS   =  $(GTP_CMN_OBJS) $(LNXLIBPATH) $(LNXLOPTS) -L$(LIB_DIR) -lsfi -leg -lhi -lmt_gtp -lcm 
endif
			
$(LNXENBE2EBINGTP):
	@echo $(BUILD_PREFIX) "GTP" $@
	$(V) $(CC)  $(GTP_OBJS)  -export-dynamic -o $(BIN_DIR)/gtp.exe 
	
# L2SIM Module objects 
BLD_L2SIM_OBJS : CREATE_L2SIM_OBJ_DIR L2SIM_SFI_OBJ_FILE $(L2SIM_OBJ_FILES)

CREATE_L2SIM_OBJ_DIR:
	mkdir -p $(L2SIM_OBJ_DIR)

$(L2SIM_OBJ_DIR)/%.o: $(SRC_DIR)/l2sim/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) $(L2SIM_CFLAGS) -o $@ $<

CCuehFLAGS= -DEGTP_U -DNX -DUZ -DEU -DKW_PDCP -DDEBUGP  -DTFU_VER_2 -DLCEGUIEGT -DLCUEHLICTF -DLCUEHLINHU -DLCPJUIPJU -DLCUEHLIRGR

UEH_SFI_INC_DIR	= -I$(IN_DIR)/../s1mgr/ -I$(IN_DIR)/sec/ -I$(IN_DIR)/../../rrc/ -I$(IN_DIR)/../udh/ -I$(IN_DIR)/../../inc/ -I$(IN_DIR)/../../pseudophy/


L2SIM_SFI_OBJ_FILE: $(UEH_DIR)/rrm_sfi_util.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(L2SIM_OBJ_DIR)/rrm_sfi_util.o $(UEH_SFI_INC_DIR) $(CCLNXHIOPTS) $(LNXCOPTS) $(LNXIOPTS) $(CCuehFLAGS) \
	$(UEH_DIR)/rrm_sfi_util.c

L2SIM_SFI_OBJ_FILES = $(L2SIM_OBJ_DIR)/rrm_sfi_util.o

L2SIM_CMN_OBJS := $(SSR_L2SIM_OBJ_DIR)/$(SSR_OBJ_FILE) $(L2SIM_SFI_OBJ_FILES) 

# l2sim EXE -- l2sim.exe
l2sim :  $(COMMON_LIB_FILES) BLD_L2SIM_OBJS mt_l2sim sfi cm ssr_l2sim $(LNXENBE2EBINL2SIM)

L2SIM_OBJS  = $(L2SIM_OBJ_FILES) $(L2SIM_CMN_OBJS) -lmt_l2sim -L$(LIB_DIR) $(LNXLOPTS) $(LNXLIBPATH)  -lsfi -lcm 

$(LNXENBE2EBINL2SIM) : 
	@echo $(BUILD_PREFIX) "L2SIM" $@
	$(V) $(CC)  $(L2SIM_OBJS)  -export-dynamic -o $(BIN_DIR)/l2sim.exe 

	
#	$(UEH_OBJ_DIR)/ueh_dflt.o
# Control Plane EXE -- cp.exe
CP_CMN_OBJS = $(UEH_OBJ_DIR)/cp_main.o \
	$(SSR_CP_OBJ_DIR)/$(SSR_OBJ_FILE) \
	$(SM_OBJ_DIR)/sm_bsmsm_exms.o \
	$(SM_OBJ_DIR)/sm_pmhsm_exms.o \
	$(SM_OBJ_DIR)/sm_lthsm_exms.o \
	$(UEH_OBJ_DIR)/hmac_sha2.o \
	$(UEH_OBJ_DIR)/sha2.o \
	$(UEH_OBJ_DIR)/ueh_bldmsg.o \
	$(UEH_OBJ_DIR)/ueh_cell.o \
	$(UEH_OBJ_DIR)/ueh_cmn_sec.o \
	$(UEH_OBJ_DIR)/ueh_crm.o \
	$(UEH_OBJ_DIR)/ueh_crm_sm.o \
	$(UEH_OBJ_DIR)/ueh_db.o \
	$(UEH_OBJ_DIR)/ueh_ex_ms.o \
	$(UEH_OBJ_DIR)/ueh_li_hdl.o \
	$(UEH_OBJ_DIR)/ueh_l2m.o \
	$(UEH_OBJ_DIR)/ueh_li.o \
	$(UEH_OBJ_DIR)/ueh_names.o \
	$(UEH_OBJ_DIR)/rrm_sfi_util.o \
	$(UEH_OBJ_DIR)/ueh_mi.o \
	$(UEH_OBJ_DIR)/ueh_ptmi.o \
	$(UEH_OBJ_DIR)/ueh_pucch_res_mgmt.o \
	$(UEH_OBJ_DIR)/ueh_relay.o \
	$(UEH_OBJ_DIR)/ueh_s1ap_cm.o \
	$(UEH_OBJ_DIR)/ueh_sib.o \
	$(SM_OBJ_DIR)/sm_cfg_enodeb.o \
	$(SM_OBJ_DIR)/sm_rrm_ex_ms.o \
	$(SM_OBJ_DIR)/sm_hi_exms.o \
	$(SM_OBJ_DIR)/sm_hi_ptmi.o \
	$(SM_OBJ_DIR)/sm_nh_exms.o \
	$(SM_OBJ_DIR)/sm_nh_ptmi.o \
	$(SM_OBJ_DIR)/sm_sb_exms.o \
	$(SM_OBJ_DIR)/sm_sb_ptmi.o \
	$(SM_OBJ_DIR)/sm_sz_exms.o \
	$(SM_OBJ_DIR)/sm_sz_ptmi.o \
	$(UEH_OBJ_DIR)/ueh_srs_res_mgmt.o \
	$(UEH_OBJ_DIR)/ueh_tmr.o \
	$(UEH_OBJ_DIR)/ueh_utils.o \
	$(UEH_OBJ_DIR)/s1ap_mgr.o \
	$(UEH_OBJ_DIR)/s1ap_sm.o \
	$(UEH_OBJ_DIR)/s1ap_unwrap.o \
	$(UEH_OBJ_DIR)/s1ap_wrap.o \
	$(UEH_OBJ_DIR)/s1ap_cmn_proc.o \
	$(UEH_OBJ_DIR)/s1ap_err.o \
	$(SM_OBJ_DIR)/sm_rg_exms.o \
	$(SM_OBJ_DIR)/sm_kw_exms.o \
	$(SM_OBJ_DIR)/sm_udh_exms.o \
	$(SM_OBJ_DIR)/sm_ys_exms.o \
	$(SM_OBJ_DIR)/sm_eg_exms.o \
	$(SM_OBJ_DIR)/sm_eg_ptmi.o \
	$(SM_OBJ_DIR)/sm_rg_ptmi.o \
	$(SM_OBJ_DIR)/sm_ys_ptmi.o \
	$(SM_OBJ_DIR)/sm_pj_ptmi.o \
	$(SM_OBJ_DIR)/sm_ptmi.o \
	$(SM_OBJ_DIR)/sm_kw_ptmi.o \
	$(UEH_OBJ_DIR)/ueh_nh_ptli.o \
	$(UEH_OBJ_DIR)/ueh_pj_ptli.o \
	$(UEH_OBJ_DIR)/ueh_ct_ptli.o \
	$(UEH_OBJ_DIR)/ueh_rg_ptli.o \
	$(UEH_OBJ_DIR)/ueh_sz_ptli.o \
	$(UEH_OBJ_DIR)/ueh_udh_ptli.o \
	$(SM_OBJ_DIR)/sm_cntrl.o \
	$(SM_OBJ_DIR)/sm_eg_cntrl.o \
	$(SM_OBJ_DIR)/sm_hi_cntrl.o \
	$(SM_OBJ_DIR)/sm_kw_cntrl.o \
	$(SM_OBJ_DIR)/sm_nh_cntrl.o \
	$(SM_OBJ_DIR)/sm_pj_cntrl.o \
	$(SM_OBJ_DIR)/sm_rg_cntrl.o \
	$(SM_OBJ_DIR)/sm_sb_cntrl.o \
	$(SM_OBJ_DIR)/sm_sz_cntrl.o \
	$(SM_OBJ_DIR)/sm_udh_cntrl.o \
	$(SM_OBJ_DIR)/sm_ys_cntrl.o \
	$(UEH_OBJ_DIR)/ueh_mac_sch_cfg.o 

ifeq ($(TARGET), 4gmx)
CP_OBJS   =  $(CP_CMN_OBJS) -L$(LIB_DIR) $(LNXLOPTS)  $(LNXLIBPATH) \
	-lsfi -lsz -lsb -lnh -lmt_cp -lcm 
else
CP_OBJS   =  $(CP_CMN_OBJS) $(LNXLIBPATH) $(LNXMSLIBPATH) $(LNXLOPTS)  -L$(LIB_DIR) -lsfi -losal -lsz -lsb -lnh -lmt_cp -lcm 
endif

$(LNXENBE2EBINCP):  
	@echo $(BUILD_PREFIX) "CP" $@
	$(V) $(CC)  $(CP_OBJS) -export-dynamic -o $(BIN_DIR)/cp.exe 

$(LNXENBE2EBINRADIOUP): 
	@echo $(BUILD_PREFIX) "RADIOUP" $@
	$(V) $(CC)  $(UEH_OBJ_DIR)/radioup_main.o \
	$(UEH_OBJ_DIR)/rrm_sfi_util.o \
	$(SSR_GTP_OBJ_DIR)/$(SSR_OBJ_FILE) \
	$(SFI_OBJ_DIR)/sfi.o \
	$(SFI_OBJ_DIR)/sfi_gen.o \
	$(SFI_OBJ_DIR)/event_names.o \
	$(LNXLIBPATH) $(LNXMSLIBPATH) -L$(LIB_DIR) -lkw -lrg -ltf -lmt_gtp -lcm -losal -lphy -export-dynamic  $(LNXLOPTS) -o $(BIN_DIR)/radioup.exe 
	
#-----------------------------------------------------------------------
# UNIX link - absolute object file depends on relocatable object
# (.o) files

#
# Making Common Libraries
#
cm: CREATE_CM_DIR $(BLD_LNX_CM_OBJS)
CREATE_CM_DIR: 
	mkdir -p $(CM_OBJ_DIR)
$(BLD_LNX_CM_OBJS):
	$(MAKE) -f $(SCRIPTS_DIR)/cm.mak $(LIB_DIR)/libcm.a COPTS='$(LNXCOPTS)' AR='$(AR)' \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXCMOPTS)' IOPTS='$(LNX_CM_IOPTS)' IN_DIR='$(CM_DIR)' \
	OUT_DIR='$(CM_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)'

#
#Making eNodeB Sample application
#
UEHLCOPTS=-DUEH -DLCLUEH -DLCSMMILUEH -DLCUEHMILUEH -DLCNHU -ULCNHUINHU \
          -DLCCTF -DLCUEHLICTF -DLCTFUICTF -DLCUEHLIRGR -DLCRGR -DLWLCUEHLIRGR -DLCRGUIRGR \
          -DLCPJU  -DLCUEHLIPJU -DLCPJUIPJU \
          -DSZ -DLCUZLISZT -DLWLCUZLISZT -DLCSZUISZT -DLCSZT -DPTSZUISZT -DEBUGP \
          -DLCEGT -DLCEGUIEGT -DCCPU_OPT -DLCEULIEGT -DSM -DNX -DNH -DNHU_VER1
          
CCUEHOPTLC= $(LNXENV) $(UEHLCOPTS)  $(STOPTS) $(CMOPTS) 
#ueh: CREATE_UEH_DIR $(BLD_LNX_UEH_E2E_OBJS)
ueh: CREATE_UEH_DIR GENERATE_EVENTHISTORY_NAMES $(BLD_LNX_UEH_E2E_OBJS)
CREATE_UEH_DIR: 
	mkdir -p $(UEH_OBJ_DIR)
GENERATE_EVENTHISTORY_NAMES: $(UEH_NAMES_C)
$(UEH_NAMES_C): $(INC_DIR)/bs_tables.h
	perl $(ROOT_DIR)/build/scripts/gen_eventnames.pl $< > $@
$(BLD_LNX_UEH_E2E_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/ueh.mak $(LIB_DIR)/libueh.a COPTS='$(LNXCOPTS)' \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCUEHOPTLC)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(UEH_DIR)' \
	OUT_DIR='$(UEH_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)' 

ifeq ($(L2SIM), on)
CCUEHOPTLC += -DPWAV_L2SIM
endif

udh: CREATE_UDH_DIR $(BLD_LNX_UDH_OBJS)
CREATE_UDH_DIR: 
	mkdir -p $(UDH_OBJ_DIR)
$(BLD_LNX_UDH_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/udh.mak $(LIB_DIR)/libudh.a COPTS='$(LNXCOPTS)' \
        LOPTS='$(LNXLOPTS)' POPTS='$(CCUEHOPTLC)' SMPOPTS='$(CCLNXSMOPTS)' \
        HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(UEH_DIR)' \
        OUT_DIR='$(UDH_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)'

sm: CREATE_SM_DIR $(BLD_LNX_SM_OBJS)
CREATE_SM_DIR: 
	mkdir -p $(SM_OBJ_DIR)
$(BLD_LNX_SM_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/sm.mak $(LIB_DIR)/libsm.a COPTS='$(LNXCOPTS)' \
        LOPTS='$(LNXLOPTS)' POPTS='$(CCUEHOPTLC)' SMPOPTS='$(CCLNXSMOPTS)' \
        HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(UEH_DIR)' \
        OUT_DIR='$(SM_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)'

#
#Making LTE RRC Layer
#
rrc: CREATE_NH_DIR $(BLD_LNX_NH_OBJS)
CREATE_NH_DIR: 
	mkdir -p $(NH_OBJ_DIR)
$(BLD_LNX_NH_OBJS):
	$(MAKE) -f $(SCRIPTS_DIR)/nh.mak $(LIB_DIR)/libnh.a COPTS='$(LNXCOPTS)' AR='$(AR)' \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXNHOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(NH_DIR)' \
	OUT_DIR='$(NH_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)'

#
#Making SFI Layer
#

sfi: CREATE_SFI_DIR $(BLD_LNX_SFI_OBJS)
CREATE_SFI_DIR: 
	mkdir -p $(SFI_OBJ_DIR)
$(BLD_LNX_SFI_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/sfi.mak $(LIB_DIR)/libsfi.a COPTS='$(LNXCOPTS)' \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXSFIOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(SFI_DIR)' \
	OUT_DIR='$(SFI_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)'
	
#
#Making SSR Layer for CP
#

ssr_cp: CREATE_SSR_CP_DIR $(BLD_LNX_SSR_CP_OBJS)
CREATE_SSR_CP_DIR: 
	mkdir -p $(SSR_CP_OBJ_DIR)
$(BLD_LNX_SSR_CP_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/ssr.mak $(LIB_DIR)/libssr_cp.a COPTS='$(LNXCOPTS)' \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXSSROPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(SSR_DIR)' \
	OUT_DIR='$(SSR_CP_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)' EXE='cp'

#
#Making SSR Layer for GTP
#

ssr_gtp: CREATE_SSR_GTP_DIR $(BLD_LNX_SSR_GTP_OBJS)
CREATE_SSR_GTP_DIR: 
	mkdir -p $(SSR_GTP_OBJ_DIR)
$(BLD_LNX_SSR_GTP_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/ssr.mak $(LIB_DIR)/libssr_gtp.a COPTS='$(LNXCOPTS)' \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXSSROPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(SSR_DIR)' \
	OUT_DIR='$(SSR_GTP_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)' EXE='gtp'

#
#Making SSR Layer for L2SIM
#

#ssr_l2sim: CREATE_SSR_L2SIM_DIR $(BLD_LNX_SSR_L2SIM_OBJS)
L2SIM_CCLNXSSROPTS = $(CCLNXSSROPTS) -DPWAV_L2SIM
ssr_l2sim: CREATE_SSR_L2SIM_DIR $(SSR_L2SIM_OBJ_DIR)/$(SSR_OBJ_FILE)
CREATE_SSR_L2SIM_DIR:
	mkdir -p $(SSR_L2SIM_OBJ_DIR)
$(SSR_L2SIM_OBJ_DIR)/$(SSR_OBJ_FILE):
	@$(MAKE) -f $(SCRIPTS_DIR)/ssr.mak $(LIB_DIR)/libssr_l2sim.a COPTS='$(LNXCOPTS)' \
	LOPTS='$(LNXLOPTS)' POPTS='$(L2SIM_CCLNXSSROPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(SSR_DIR)' \
	OUT_DIR='$(SSR_L2SIM_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)' EXE='l2sim'

#
#Making RLC Layer
#
kw: CREATE_KW_DIR $(BLD_LNX_KW_OBJS)
CREATE_KW_DIR: 
	mkdir -p $(KW_OBJ_DIR)
$(BLD_LNX_KW_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/kw.mak $(LIB_DIR)/libkw.a COPTS='$(LNXCOPTS)' AR='$(AR)' \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXKWOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(KW_DIR)' \
	OUT_DIR='$(KW_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)'
	
#
#Making MAC Layer
#
CCRGOPTLCSM=$(LNXENV) $(STOPTS) $(CMOPTS) -DRG $(RGUOPTS) $(CRGOPTS) $(LRGOPTS) $(RGROPTS) \
            $(TFUOPTS) $(RGOPTS) -URG_DEBUG -UTTI_PROC -UPROC_DL -USCH_TTI_PROC -UTOM -UPAL_MAC_PROC \
            -UUL_PROC_MAC -DLCRGLITFU -DLCSMMILRG -DLCLRG -DRG -DLCRGMILRG

rg: CREATE_RG_DIR $(BLD_LNX_RG_OBJS)
CREATE_RG_DIR: 
	mkdir -p $(RG_OBJ_DIR)
$(BLD_LNX_RG_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/rg.mak $(LIB_DIR)/librg.a COPTS='$(LNXCOPTS)' AR='$(AR)' \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCRGOPTLCSM)' IOPTS='$(LNXIOPTS)' IN_DIR='$(RG_DIR)' \
	OUT_DIR='$(RG_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)'
	
#
#Making CL Layer
#

TFLCOPTS=-DTF -DLCCTF -DLCUEHLICTF -DLCTFUICTF $(RGROPTS) $(RGOPTS) -DLCTFU -DLCTFUITFU -DTFU_VER_2 -UTFU_TDD -DLTE_PAL_ENB
CCHIOPTLCSM= $(LNXENV) $(TFLCOPTS) $(STOPTS) $(CMOPTS)

ys: CREATE_YS_DIR $(BLD_LNX_YS_OBJS)
CREATE_YS_DIR: 
	mkdir -p $(YS_OBJ_DIR)
$(BLD_LNX_YS_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/ys.mak $(LIB_DIR)/liblnxys.a COPTS='$(LNXCOPTS)' \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXTFOPTS)' IOPTS='$(LNX_FT_IOPTS)' IN_DIR='$(YS_DIR)' \
	OUT_DIR='$(YS_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' PWAV_MACPAL='$(PWAV_MACPAL)' LIB_DIR='$(LIB_DIR)' PHY_DIR='$(MS_LC_DIR)'
	
phy: CREATE_PHY_DIR $(BLD_LNX_PP_OBJS)
CREATE_PHY_DIR: 
	mkdir -p $(PHY_OBJ_DIR)
$(BLD_LNX_PP_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/pseudophy.mak $(LIB_DIR)/liblnxphy.a COPTS='$(LNXCOPTS)' \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXTFOPTS)' IOPTS='$(LNX_FT_IOPTS)' IN_DIR='$(PP_DIR)' \
	OUT_DIR='$(PHY_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)'  
	
$(BLD_LNX_YS_MS_OBJS):
@$(MAKE) -f ys.mak $(LIB_DIR)/liblnxysms.a COPTS=$(LNXCOPTS) \
        LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXMSTFOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(YS_DIR)' \
        OUT_DIR='$(YS_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'
	
#
#
#Making S1AP Layer
#
s1ap: CREATE_SZ_DIR $(BLD_LNX_SZ_OBJS)
CREATE_SZ_DIR: 
	mkdir -p $(SZ_OBJ_DIR)
$(BLD_LNX_SZ_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/sz.mak $(LIB_DIR)/libsz.a COPTS='$(LNXCOPTS)' AR='$(AR)' \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSZOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(SZ_DIR)' \
	OUT_DIR='$(SZ_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)'
	
#
#Making SCTP Layer
#
sctp: CREATE_SB_DIR $(BLD_LNX_SB_OBJS)
CREATE_SB_DIR: 
	mkdir -p $(SB_OBJ_DIR)
$(BLD_LNX_SB_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/sb.mak $(LIB_DIR)/libsb.a COPTS='$(LNXCOPTS)' AR='$(AR)' \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSBOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(SB_DIR)' \
	OUT_DIR='$(SB_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)'
	
#
#Making TUCL Layer
#
CCHIOPTLCSM= $(LNXENV) $(STOPTS) $(CMOPTS) $(HITOPTS) -DLCHIMILHI -DLCLHI -DHI -DEG -DLCHIMILHI -DLCSMHIMILHI 
tucl: CREATE_HI_DIR $(BLD_LNX_HI_OBJS)
CREATE_HI_DIR: 
	mkdir -p $(HI_OBJ_DIR)
$(BLD_LNX_HI_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/hi.mak $(LIB_DIR)/libhi.a COPTS='$(LNXCOPTS)' AR='$(AR)' \
	LOPTS='$(LOPTS)' POPTS='$(CCHIOPTLCSM)' IOPTS='$(LNXIOPTS)' IN_DIR='$(HI_DIR)' \
	OUT_DIR='$(HI_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)'
	
#
#Making eGTP Layer
#
CCEGOPTLCSM=-DSUNOS -DSS -DSS_MT -DANSI -D_GNU_SOURCE -DSS_LINUX -D_REENTRANT -D__EXTENSIONS__ \
    -DDEBUGNOEXIT -DEG -DLCEGT -DLCEGUIEGT -DCCPU_OPT -DLCEULIEGT \
    -DLCSMMILEG -DLCLEG -DLCEGMILEG -DLCSMEGMILEG -UEG_DEBUG -DLCEGLIHIT -DLCHIUIHIT -UHI $(STOPTS) $(CMOPTS)
egtp: CREATE_EG_DIR $(BLD_LNX_EG_OBJS)
CREATE_EG_DIR: 
	mkdir -p $(EG_OBJ_DIR)
$(BLD_LNX_EG_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/eg.mak $(LIB_DIR)/libeg.a COPTS='$(LNXCOPTS)' AR='$(AR)' \
	LOPTS='$(LOPTS)' POPTS='$(CCEGOPTLCSM)' IOPTS='$(LNXIOPTS)' IN_DIR='$(EG_DIR)' \
	OUT_DIR='$(EG_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)'

#
#
#Making X2AP Layer
#
x2ap: CREATE_X2AP_DIR
	@$(MAKE) -f cz.mak $(LIB_DIR)/libcz.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXCZOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(CZ_DIR)' \
	OUT_DIR='$(X2AP_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)'
CREATE_X2AP_DIR:
	mkdir -p $(X2AP_OBJ_DIR)

#
# making Multi Threaded SSI for CP
#
mt_cp: CREATE_MT_CP_DIR $(BLD_LNX_MT_CP_OBJS)
CREATE_MT_CP_DIR: 
	mkdir -p $(MT_CP_OBJ_DIR)
$(BLD_LNX_MT_CP_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/mt.mak $(LIB_DIR)/libmt_cp.a COPTS='$(LNXCOPTS)' AR='$(AR)' \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXMTOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(MT_DIR)' \
   OUT_DIR='$(MT_CP_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  \
   CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)' EXE='cp'

#
# making Multi Threaded SSI for l2sim
#
L2SIM_CCLNXMTOPTS = $(CCLNXMTOPTS) -DPWAV_L2SIM
mt_l2sim: CREATE_MT_L2SIM_DIR $(BLD_LNX_MT_L2SIM_OBJS)
CREATE_MT_L2SIM_DIR: 
	mkdir -p $(MT_L2SIM_OBJ_DIR)
$(BLD_LNX_MT_L2SIM_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/mt.mak $(LIB_DIR)/libmt_l2sim.a COPTS='$(LNXCOPTS)' AR='$(AR)' \
	LOPTS='$(LOPTS)' POPTS='$(L2SIM_CCLNXMTOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(MT_DIR)' \
   OUT_DIR='$(MT_L2SIM_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  \
   CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)' EXE='l2sim'

#
#
# making Multi Threaded SSI for GTP
#
ifeq ($(L2SIM), on)
CCLNXMTOPTS += -DPWAV_L2SIM
endif
mt_gtp: CREATE_MT_GTP_DIR $(BLD_LNX_MT_GTP_OBJS)
CREATE_MT_GTP_DIR: 
	mkdir -p $(MT_GTP_OBJ_DIR)
$(BLD_LNX_MT_GTP_OBJS):
	@$(MAKE) -f $(SCRIPTS_DIR)/mt.mak $(LIB_DIR)/libmt_gtp.a COPTS='$(LNXCOPTS)' AR='$(AR)' \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXMTOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(MT_DIR)' \
   OUT_DIR='$(MT_GTP_OBJ_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  \
   CM_INC='$(CM_INC)' LIB_DIR='$(LIB_DIR)' EXE='gtp'

# DO NOT DELETE THIS LINE -- make depend depends on it.	
	
#clean
#Usage of clean: $make <clean> TARGET=<target>
clean: clean_x86 clean_arm clean_4gmx
clean_x86:
	make -f Makefile clean_others clean_cp clean_gtp clean_radioup clean_oam  TARGET=x86 
clean_arm:
	make -f Makefile clean_others clean_cp clean_gtp clean_oam  TARGET=arm 
clean_4gmx:
	make -f Makefile clean_others clean_cp clean_gtp clean_oam clean_l2sim TARGET=4gmx 

clean_others:
	$(V) $(RM) -r $(OBJ_DIR)/*.o $(BIN_DIR)/*.exe  
	$(V) $(RM) -r $(OBJ_DIR)/*.gcda $(OBJ_DIR)/*.gcno $(OBJ_DIR)/*.c.gcov $(OBJ_DIR)/coverage/  
	$(V) $(RM) -r $(LIB_DIR)/libimc.a $(LIB_DIR)/libcfg.a $(LIB_DIR)/libtlv.a $(LIB_DIR)/libprint.a $(LIB_DIR)/libRRMprint.a
	$(V) $(RM) -r $(LIB_DIR)/libtable.a $(LIB_DIR)/libplatform.a $(LIB_DIR)/liblog.a $(LIB_DIR)/libsvnversion.a $(LIB_DIR)/libenumstr.a
	$(V) $(RM) -r $(SRC_DIR)/libimc/mod_names.c $(SRC_DIR)/libimc/msg_names.c $(SRC_DIR)/libimc/timer_names.c $(SRC_DIR)/libsvnversion/version.c
	$(V) $(RM) -r $(SRC_DIR)/libenumstr/enumstr_bserr.c $(SRC_DIR)/libenumstr/enumstr_modid.c
	$(V) $(RM) -r $(ROOT_DIR)/*.log $(ROOT_DIR)/*.xml $(ROOT_DIR)/*.zip
	$(V) $(RM) -r $(ROOT_DIR)/doc/generated/*
	$(V) $(RM) -r $(BUILD_DIR)/scripts/tempDepend
	
	
clean_gtp: 
	$(MAKE) -f Makefile clean_udh clean_tucl clean_egtp clean_cm clean_mt_gtp clean_ssr_gtp clean_sfi clean_turner
	rm -f $(BIN_DIR)/gtp.exe

clean_cp: 
	$(MAKE) -f Makefile clean_ueh clean_sm clean_rrc clean_s1ap clean_sctp clean_tucl clean_cm clean_mt_cp clean_sfi clean_ssr_cp
	rm -f $(BIN_DIR)/cp.exe

clean_radioup:
	rm -f \
	$(UEH_OBJ_DIR)/radioup_main.o \
        $(UEH_OBJ_DIR)/rrm_sfi_util.o \
        $(SSR_GTP_OBJ_DIR)/$(SSR_OBJ_FILE) \
        $(SFI_OBJ_DIR)/sfi.o \
        $(SFI_OBJ_DIR)/sfi_gen.o \
        $(SFI_OBJ_DIR)/event_names.o
	rm -rf $(KW_OBJ_DIR) $(YS_OBJ_DIR) $(RG_OBJ_DIR) $(PHY_OBJ_DIR)
	rm -f $(LIB_DIR)/libkw.a $(LIB_DIR)/libphy.a $(LIB_DIR)/librg.a $(LIB_DIR)/libtf.a $(BIN_DIR)/radioup.exe
	$(MAKE) -f Makefile clean_cm clean_mt_gtp clean_ssr_gtp

clean_turner:
	rm -f $(LIB_DIR)/libbsp.a $(LIB_DIR)/libosal.a
	cd $(MS_PATH) && make clean && cd -
	rm -f $(MS_TP_DIR)/*.o

#Usage of clean_cm: $make clean_cm TARGET=<target>
clean_cm:
	$(MAKE) -f $(SCRIPTS_DIR)/cm.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(CM_OBJ_DIR)' OBJ='$(OBJ)'
	rm -rf $(CM_OBJ_DIR)

clean_ueh:
	$(MAKE) -f $(SCRIPTS_DIR)/ueh.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(UEH_OBJ_DIR)' OBJ='$(OBJ)'
	rm -rf $(UEH_OBJ_DIR)

clean_udh:
	$(MAKE) -f $(SCRIPTS_DIR)/udh.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(UDH_OBJ_DIR)' OBJ='$(OBJ)'
	rm -rf $(UDH_OBJ_DIR)

clean_sm:
	$(MAKE) -f $(SCRIPTS_DIR)/sm.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(SM_OBJ_DIR)' OBJ='$(OBJ)'
	rm -rf $(SM_OBJ_DIR)

clean_rrc:
	$(MAKE) -f $(SCRIPTS_DIR)/nh.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(NH_OBJ_DIR)' OBJ='$(OBJ)'
	rm -rf $(NH_OBJ_DIR)

clean_s1ap:
	$(MAKE) -f $(SCRIPTS_DIR)/sz.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(SZ_OBJ_DIR)' OBJ='$(OBJ)'
	rm -rf $(SZ_OBJ_DIR)

clean_sctp:
	$(MAKE) -f $(SCRIPTS_DIR)/sb.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(SB_OBJ_DIR)' OBJ='$(OBJ)'
	rm -rf $(SB_OBJ_DIR)

clean_egtp:
	$(MAKE) -f $(SCRIPTS_DIR)/eg.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(EG_OBJ_DIR)' OBJ='$(OBJ)'
	rm -rf $(EG_OBJ_DIR)

clean_tucl:
	$(MAKE) -f $(SCRIPTS_DIR)/hi.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(HI_OBJ_DIR)' OBJ='$(OBJ)'
	rm -rf $(HI_OBJ_DIR)

clean_sfi:
	$(MAKE) -f $(SCRIPTS_DIR)/sfi.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(SFI_OBJ_DIR)' OBJ='$(OBJ)'
	rm -rf $(SFI_OBJ_DIR)

clean_ssr_cp:
	$(MAKE) -f $(SCRIPTS_DIR)/ssr.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(SSR_CP_OBJ_DIR)' OBJ='$(OBJ)' EXE='cp'
	rm -rf $(SSR_CP_OBJ_DIR)
clean_ssr_gtp:
	$(MAKE) -f $(SCRIPTS_DIR)/ssr.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(SSR_GTP_OBJ_DIR)' OBJ='$(OBJ)' EXE='gtp'
	rm -rf $(SSR_GTP_OBJ_DIR)
clean_ssr_l2sim:
	$(MAKE) -f $(SCRIPTS_DIR)/ssr.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(SSR_L2SIM_OBJ_DIR)' OBJ='$(OBJ)' EXE='l2sim'
	rm -rf $(SSR_L2SIM_OBJ_DIR)

clean_mt_cp:
	$(MAKE) -f $(SCRIPTS_DIR)/mt.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(MT_CP_OBJ_DIR)' OBJ='$(OBJ)' EXE='cp'
	rm -rf $(MT_CP_OBJ_DIR)
clean_mt_gtp:
	$(MAKE) -f $(SCRIPTS_DIR)/mt.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(MT_GTP_OBJ_DIR)' OBJ='$(OBJ)' EXE='gtp'
	rm -rf $(MT_GTP_OBJ_DIR)
clean_mt_l2sim:
	$(MAKE) -f $(SCRIPTS_DIR)/mt.mak clean LIB_DIR='$(LIB_DIR)' OUT_DIR='$(MT_L2SIM_OBJ_DIR)' OBJ='$(OBJ)' EXE='l2sim'
	rm -rf $(MT_L2SIM_OBJ_DIR)

clean_oam:
	$(MAKE) -f Makefile clean_bsm clean_prm clean_cih clean_deh clean_fth clean_lth clean_pmh clean_wih clean_sih

clean_bsm:
	rm -rf $(BSM_OBJ_DIR) $(BIN_DIR)/bsm.exe

clean_prm:
	rm -rf $(PRM_OBJ_DIR) $(BIN_DIR)/prm.exe

clean_cih:
	rm -rf $(CIH_OBJ_DIR) $(BIN_DIR)/cih.exe

clean_deh:
	rm -rf $(DEH_OBJ_DIR) $(BIN_DIR)/deh.exe

clean_fth:
	rm -rf $(FTH_OBJ_DIR) $(BIN_DIR)/fth.exe

clean_lth:
	rm -rf $(LTH_OBJ_DIR) $(BIN_DIR)/lth.exe

clean_pmh:
	rm -rf $(PMH_OBJ_DIR) $(BIN_DIR)/pmh.exe

clean_wih:
	rm -rf $(WIH_OBJ_DIR) $(BIN_DIR)/wih.exe

clean_sih: 
	cd $(SNMP_DIR); rm -rf build.* config.status $(BIN_DIR)/snmpset $(LOG_DIR)/* ; cd -;

clean_l2sim:
	$(MAKE) -f Makefile clean_cm clean_mt_l2sim clean_sfi clean_ssr_l2sim clean_rrc clean_sm
	rm -rf $(L2SIM_OBJ_DIR) $(BIN_DIR)/l2sim.exe 
doc:
	doxygen $(ROOT_DIR)/build/config/bs_doxy_conf_file

help:
	@echo "all                                   -- builds all for x86 arm and 4gmx"
	@echo "x86                                   -- builds all for x86 only"
	@echo "arm                                   -- builds all for arm only"
	@echo "4gmx                                  -- builds all for 4gmx only i.e creates 4gmx cp.exe and gtp.exe, before compiling for "4gmx", "arm" need to be compiled"
	@echo "x86  EXE=<exe_name>                   -- builds only given exe for x86 , exe_name can be one of prm,bsm,cih,deh,lth,pmh,fth,wih,sih,cpup(cp,gtp,radioup),ems_sim,rh_sim"
	@echo "arm  EXE=<exe_name>                   -- builds only given exe for arm , exe_name can be one of prm,bsm,cih,deh,lth,pmh,fth,wih,sih,cpup(cp,gtp,radioup),ems_sim,rh_sim"
	@echo "4gmx  EXE=<exe_name>                  -- builds only given exe for 4gmx , exe_name can be one of prm,bsm,cih,deh,lth,pmh,fth,wih,sih,cpup(cp,gtp),ems_sim,rh_sim"
	@echo "<mod_name> TARGET=<4gmx/x86/arm>      -- builds only objectfiles and libraries of given module, mod can be cm, ueh, udh, sm, rrc, s1ap, sctp, egtp, tucl, sfi, ssr_cp, ssr_gtp, mt_cp, mt_gtp, turner and platform can be x86, arm, 4gmx"
	@echo "clean                                 -- clean all obj files for x86, 4gmx and arm"
	@echo "clean_x86                             -- clean obj, libraries and exe files for platform x86"
	@echo "clean_arm                             -- clean obj, libraries and exe files for platform arm"
	@echo "clean_4gmx                            -- clean obj, libraries and exe files for platform 4gmx"
	@echo "clean_sih                             -- clean obj, library of module sih"
	@echo "clean_gtp                             -- clean obj, library of module gtp"
	@echo "clean_cp                              -- clean obj, library of module cp"
	@echo "clean_radioup                         -- clean obj, library of module radioup"
	@echo "clean_cm                              -- clean obj, library of module cm"
	@echo "clean_ueh                             -- clean obj, library of module ueh"
	@echo "clean_udh                             -- clean obj, library of module udh"
	@echo "clean_sm                              -- clean obj, library of module sm"
	@echo "clean_rrc                             -- clean obj, library of module rrc"
	@echo "clean_s1ap                            -- clean obj, library of module s1ap"
	@echo "clean_sctp                            -- clean obj, library of module sctp"
	@echo "clean_egtp                            -- clean obj, library of module egtp"
	@echo "clean_tucl                            -- clean obj, library of module tucl"
	@echo "clean_sfi                             -- clean obj, library of module sfi"
	@echo "clean_ssr_cp                          -- clean obj, library of module ssr_cp"
	@echo "clean_ssr_gtp                         -- clean obj, library of module ssr_gtp"
	@echo "clean_mt_cp                           -- clean obj, library of module mt_cp"
	@echo "clean_mt_gtp                          -- clean obj, library of module mt_gtp"
	@echo "clean_oam                             -- clean obj, library of module oam"
	@echo "clean_bsm                             -- clean obj, library and exe files of module bsm"
	@echo "clean_prm                             -- clean obj, library and exe files of module prm"
	@echo "clean_cih                             -- clean obj, library and exe files of module cih"
	@echo "clean_deh                             -- clean obj, library and exe files of module deh"
	@echo "clean_fth                             -- clean obj, library and exe files of module fth"
	@echo "clean_lth                             -- clean obj, library and exe files of module lth"
	@echo "clean_pmh                             -- clean obj, library and exe files of module pmh"
	@echo "clean_wih                             -- clean obj, library and exe files of module wih"
	@echo "clean_<mod_name> TARGET=<4gmx/x86/arm>-- clean obj, library and exe files of module specified for the target mentioned, mod can be cm, ueh, udh, sm, rrc, s1ap, sctp, egtp, tucl, sfi, ssr_cp, ssr_gtp, mt_cp, mt_gtp, turner and platform can be x86, arm, 4gmx"

#-------------------------------------------------------------#
#Makefile for product SZ - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCuehFLAGS= -DEGTP_U -DNX -DUZ -DEU -DKW_PDCP -DDEBUGP  -DTFU_VER_2 -DLCEGUIEGT -DLCUEHLICTF -DLCUEHLINHU -DLCPJUIPJU -DLCUEHLIRGR

uehE2eLnxSRCS= \
    $(IN_DIR)/gtp_main.c \
    $(IN_DIR)/rrm_sfi_util.c \
    $(IN_DIR)/../udh/udh_func.c\
    $(IN_DIR)/../udh/udh_lm.c\
    $(IN_DIR)/../udh/udh_main.c\
#    $(IN_DIR)/cfgctl/ueh_udh_ptli.c

# Product objects-------------------------------------------------------
uehE2eLnxOBJS= \
    $(OUT_DIR)/gtp_main.$(OBJ) \
    $(OUT_DIR)/rrm_sfi_util.$(OBJ) \
    $(OUT_DIR)/udh_func.$(OBJ)\
    $(OUT_DIR)/udh_lm.$(OBJ)\
    $(OUT_DIR)/udh_main.$(OBJ)\
#   	$(OUT_DIR)/ueh_udh_ptli.$(OBJ)


UEHCM_INC=$(IN_DIR)/inc/ueh.h $(IN_DIR)/inc/sctp_cfg_table.h $(IN_DIR)/inc/s1ap_cfg_table.h  $(IN_DIR)/inc/ueh_perf.h $(IN_DIR)/../../sfi/sfi.h

ALL_INC=$(UEHCM_INC) $(CM_INC) $(UEH_DIR)

BUILD_PREFIX            := "==> Building"
V                       := @

UEH_INC_DIR	= -I$(IN_DIR)/../s1mgr/ -I$(IN_DIR)/ -I$(IN_DIR)/../../rrc/ -I$(IN_DIR)/../udh/ -I$(IN_DIR)/../../inc/ -I$(IN_DIR)/../../pseudophy/ 
 
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/gtp_main.$(OBJ): $(IN_DIR)/gtp_main.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/gtp_main.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/gtp_main.c
$(OUT_DIR)/rrm_sfi_util.$(OBJ): $(IN_DIR)/rrm_sfi_util.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c -o$(OUT_DIR)/rrm_sfi_util.o $(UEH_INC_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) \
	$(IN_DIR)/rrm_sfi_util.c
$(OUT_DIR)/udh_func.$(OBJ): $(IN_DIR)/../udh/udh_func.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/../udh/udh_func.c -o \
	$(OUT_DIR)/udh_func.$(OBJ)
	
$(OUT_DIR)/udh_lm.$(OBJ): $(IN_DIR)/../udh/udh_lm.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/../udh/udh_lm.c -o \
	$(OUT_DIR)/udh_lm.$(OBJ)
	
$(OUT_DIR)/udh_main.$(OBJ): $(IN_DIR)/../udh/udh_main.c $(ALL_INC)
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/../udh/udh_main.c -o \
	$(OUT_DIR)/udh_main.$(OBJ)

#$(OUT_DIR)/ueh_udh_ptli.$(OBJ): $(IN_DIR)/cfgctl/ueh_udh_ptli.c $(ALL_INC)
#	@echo $(BUILD_PREFIX) $@
#	$(V) $(CC) -c $(UEH_INC_DIR)   $(COPTS) $(IOPTS) $(POPTS) $(CCuehFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/cfgctl/ueh_udh_ptli.c -o \
	$(OUT_DIR)/ueh_udh_ptli.$(OBJ)

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/libudh.a:$(uehE2eLnxOBJS)
	ar -cr $(LIB_DIR)/libudh.a $(uehE2eLnxOBJS)	
#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	rm -f $(LIB_DIR)/libudh.a $(uehE2eLnxOBJS)

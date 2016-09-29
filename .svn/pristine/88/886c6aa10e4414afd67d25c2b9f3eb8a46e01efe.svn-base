#-------------------------------------------------------------#
#Makefile for product ib - script generated.
#Only the $(CCib) may be modified.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCkwFLAGS= -DKW_PDCP -UNH -USM -DPTPJLIB -DLTE_HENB  -UKW_BG_DL_PROC

# Product objects-------------------------------------------------------
RLCPDCP_SRC_FILES= \
    kw_amm.c \
    kw_cfg.c \
    kw_dbm.c \
    kw_ex_ms.c \
    kw_id.c \
    kw_lim.c \
    kw_lmm.c \
    kw_ptli.c \
    kw_ptmi.c \
    kw_ptui.c \
    kw_tmm.c \
    kw_tmr.c \
    kw_uim.c \
    kw_umm.c \
    kw_pj_lim.c \
    kw_pj_uim.c \
    kw_pj_lmm.c \
    kw_pj_cfg.c \
    kw_pj_dbm.c \
    kw_pj_dlm.c \
    kw_pj_ulm.c \
    kw_pj_utl.c \
    kw_pj_tmr.c \
    kw_pj_lib.c \
    kw_ptlib.c \
    kw_utl.c

RLCPDCP_OBJ_FILES  := $(patsubst %.c, $(OUT_DIR)/%.o, $(RLCPDCP_SRC_FILES) )

KWCM_INC=$(IN_DIR)/kw.h   $(IN_DIR)/kw.x $(IN_DIR)/kw_env.h $(IN_DIR)/kw_err.h

ALL_INC=$(KWCM_INC) $(CM_INC)
   
BUILD_PREFIX            := "==> Building"
V                       := @
   
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
#EGTP ObjectFiles
$(OUT_DIR)/%.o: $(IN_DIR)/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCkwFLAGS) $< -o $@

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/libkw.a:$(RLCPDCP_OBJ_FILES) $(ALL_INC)
	$(V) $(AR) -cr $(LIB_DIR)/libkw.a $(RLCPDCP_OBJ_FILES)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	rm -f $(LIB_DIR)/libkw.a $(RLCPDCP_OBJ_FILES)


#-------------------------------------------------------------#
#Makefile for product SZ - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCszFLAGS=-DLCLSZ -DLCSCT -DSZ -DSM -US1AP_REL851 -ULSZV1 -DS1AP_REL8A0

# Product sources-------------------------------------------------------
S1AP_SRC_FILES= \
    sz_db.c \
    sz_li.c \
    sz_mgmt.c \
    sz_mi.c \
    sz_msgfn.c \
    sz_ex_ms.c \
    sz_ndb.c \
    sz_id.c \
    sz_sm.c \
    sz_smlienb.c \
    sz_smuienb.c \
    sz_ptli.c \
    sz_ptmi.c \
    sz_ptui.c \
    sz_tmr.c \
    sz_ui.c \
    sz_utils.c 

S1AP_OBJ_FILES  := $(patsubst %.c, $(OUT_DIR)/%.o, $(S1AP_SRC_FILES) )

SZCM_INC=$(IN_DIR)/sz.h $(IN_DIR)/sz.x $(IN_DIR)/sz_err.h

ALL_INC=$(SZCM_INC) $(CM_INC)

BUILD_PREFIX            := "==> Building"
V                       := @
   
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
#S1AP Object Files
$(OUT_DIR)/%.o: $(IN_DIR)/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $< -o $@

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/libsz.a:$(S1AP_OBJ_FILES) $(ALL_INC)
	$(V) $(AR) -cr $(LIB_DIR)/libsz.a $(S1AP_OBJ_FILES)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	rm -f $(LIB_DIR)/libsz.a $(S1AP_OBJ_FILES)


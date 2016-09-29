#-------------------------------------------------------------#
#Makefile for product NH - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCnhFLAGS=-UKW -UPJ

#-------------------------------------------------------------#
#File/Obj macros             
#-------------------------------------------------------------#
RRC_SRC_FILES= \
        nh_brm.c \
	    nh_ccm.c \
        nh_cgm.c \
        nh_cpm.c \
        nh_db.c \
        nh_3gdb.c \
        nh_dbm.c\
        nh_edm.c \
        nh_ex_ms.c \
        nh_id.c \
        nh_init.c \
        nh_lim.c \
        nh_lmm.c \
        nh_pim.c \
        nh_ptli.c \
        nh_ptmi.c \
        nh_ptui.c \
        nh_rhm.c \
        nh_rtm.c \
        nh_uim.c \
        nh_utl.c 

RRC_OBJ_FILES  := $(patsubst %.c, $(OUT_DIR)/%.o, $(RRC_SRC_FILES) )

NHCM_INC= $(IN_DIR)/nh.h $(IN_DIR)/nh.x $(IN_DIR)/nh_db.x $(IN_DIR)/nh_3gdb.x 

ALL_INC=$(NHCM_INC) $(CM_INC)

BUILD_PREFIX            := "==> Building"
V                       := @
   
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
#RRC ObjectFiles
$(OUT_DIR)/%.o: $(IN_DIR)/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) $< -o $@

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/libnh.a:$(RRC_OBJ_FILES) $(ALL_INC)
	$(V) $(AR) -cr $(LIB_DIR)/libnh.a $(RRC_OBJ_FILES)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(LIB_DIR)/libnh.a $(RRC_OBJ_FILES)


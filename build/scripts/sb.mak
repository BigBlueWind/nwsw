#-------------------------------------------------------------#
#Makefile for product SB - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCsbFLAGS=

# Product sources-------------------------------------------------------
SCTP_SRC_FILES= \
    sb_bdy1.c \
    sb_bdy2.c \
    sb_bdy3.c \
    sb_bdy4.c \
    sb_ex_ms.c \
    sb_id.c \
    sb_ptmi.c \
    sb_ptui.c \
    sb_ptli.c \
    sb_mtu.c \
    sb_port.c

SCTP_OBJ_FILES  := $(patsubst %.c, $(OUT_DIR)/%.o, $(SCTP_SRC_FILES)  )

SBCM_INC=$(IN_DIR)/sb.h $(IN_DIR)/sb.x $(IN_DIR)/sb_mtu.h $(IN_DIR)/sb_mtu.x $(IN_DIR)/sb_port.h $(IN_DIR)/sb_port.x $(IN_DIR)/sb_err.h

ALL_INC=$(SBCM_INC) $(CM_INC)

BUILD_PREFIX            := "==> Building"
V                       := @
   
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
#SCTP ObjectFiles
$(OUT_DIR)/%.o: $(IN_DIR)/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $< -o $@

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/libsb.a:$(SCTP_OBJ_FILES) $(ALL_INC)
	$(V) $(AR) -cr $(LIB_DIR)/libsb.a $(SCTP_OBJ_FILES)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	rm -f $(LIB_DIR)/libsb.a $(SCTP_OBJ_FILES)


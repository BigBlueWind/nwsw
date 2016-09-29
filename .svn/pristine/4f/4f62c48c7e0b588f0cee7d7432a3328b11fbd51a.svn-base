#-------------------------------------------------------------#
#Makefile for product HI - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CChiFLAGS=-USZ

# Product sources-------------------------------------------------------
TUCL_SRC_FILES= \
    hi_bdy1.c \
    hi_bdy2.c \
    hi_bdy3.c \
    hi_ex_ms.c \
    hi_id.c \
    hi_ptmi.c \
    hi_ptui.c 

TUCL_OBJ_FILES  := $(patsubst %.c, $(OUT_DIR)/%.o, $(TUCL_SRC_FILES) )

HICM_INC=$(IN_DIR)/hi.h $(IN_DIR)/hi.x $(IN_DIR)/hi_err.h

ALL_INC=$(HICM_INC) $(CM_INC)

BUILD_PREFIX            := "==> Building"
V                       := @
   
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
#TUCL ObjectFiles
$(OUT_DIR)/%.o: $(IN_DIR)/%.c
	@echo $(BUILD_PREFIX) $@
	$(V) $(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CChiFLAGS) $< -o $@

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/libhi.a:$(TUCL_OBJ_FILES) $(ALL_INC)
	$(V) $(AR) -cr $(LIB_DIR)/libhi.a $(TUCL_OBJ_FILES)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	rm -f $(LIB_DIR)/libhi.a $(TUCL_OBJ_FILES)


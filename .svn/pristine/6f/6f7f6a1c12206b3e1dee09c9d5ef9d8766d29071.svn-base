#-------------------------------------------------------------#
#Makefile for product HI - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CChiFLAGS=

# Product sources-------------------------------------------------------
hiSRCS= \
    $(IN_DIR)/hi_bdy1.c \
    $(IN_DIR)/hi_bdy2.c \
    $(IN_DIR)/hi_bdy3.c \
    $(IN_DIR)/hi_ex_ms.c \
    $(IN_DIR)/hi_id.c \
    $(IN_DIR)/hi_ptmi.c \
    $(IN_DIR)/hi_ptui.c 


# Product objects-------------------------------------------------------
hiOBJS= \
    $(OUT_DIR)/hi_bdy1.$(OBJ) \
    $(OUT_DIR)/hi_bdy2.$(OBJ) \
    $(OUT_DIR)/hi_bdy3.$(OBJ) \
    $(OUT_DIR)/hi_ex_ms.$(OBJ) \
    $(OUT_DIR)/hi_id.$(OBJ) \
    $(OUT_DIR)/hi_ptmi.$(OBJ) \
    $(OUT_DIR)/hi_ptui.$(OBJ) 


HICM_INC=$(IN_DIR)/hi.h $(IN_DIR)/hi.x $(IN_DIR)/hi_err.h

ALL_INC=$(HICM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/hi_bdy1.$(OBJ): $(IN_DIR)/hi_bdy1.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/hi_bdy1.o $(COPTS) $(IOPTS) $(POPTS) $(CChiFLAGS) \
	$(IN_DIR)/hi_bdy1.c
$(OUT_DIR)/hi_bdy2.$(OBJ): $(IN_DIR)/hi_bdy2.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CChiFLAGS) $(IN_DIR)/hi_bdy2.c -o \
	$(OUT_DIR)/hi_bdy2.$(OBJ)
$(OUT_DIR)/hi_bdy3.$(OBJ): $(IN_DIR)/hi_bdy3.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CChiFLAGS) $(IN_DIR)/hi_bdy3.c -o \
	$(OUT_DIR)/hi_bdy3.$(OBJ)
$(OUT_DIR)/hi_ex_ms.$(OBJ): $(IN_DIR)/hi_ex_ms.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CChiFLAGS) $(IN_DIR)/hi_ex_ms.c -o \
	$(OUT_DIR)/hi_ex_ms.$(OBJ)
$(OUT_DIR)/hi_id.$(OBJ): $(IN_DIR)/hi_id.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CChiFLAGS) $(IN_DIR)/hi_id.c -o \
	$(OUT_DIR)/hi_id.$(OBJ)
$(OUT_DIR)/hi_ptmi.$(OBJ): $(IN_DIR)/hi_ptmi.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CChiFLAGS) $(IN_DIR)/hi_ptmi.c -o \
	$(OUT_DIR)/hi_ptmi.$(OBJ)
$(OUT_DIR)/hi_ptui.$(OBJ): $(IN_DIR)/hi_ptui.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CChiFLAGS) $(IN_DIR)/hi_ptui.c -o \
	$(OUT_DIR)/hi_ptui.$(OBJ)

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libhi.a:$(hiOBJS)
	$(AR) -cr $(OUT_DIR)/libhi.a $(hiOBJS)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libhi.a $(hiOBJS)


#-------------------------------------------------------------#
#Makefile for product SB - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCsbFLAGS=

# Product sources-------------------------------------------------------
sbSRCS= \
    $(IN_DIR)/sb_bdy1.c \
    $(IN_DIR)/sb_bdy2.c \
    $(IN_DIR)/sb_bdy3.c \
    $(IN_DIR)/sb_bdy4.c \
    $(IN_DIR)/sb_ex_ms.c \
    $(IN_DIR)/sb_id.c \
    $(IN_DIR)/sb_ptmi.c \
    $(IN_DIR)/sb_ptui.c \
    $(IN_DIR)/sb_ptli.c \
    $(IN_DIR)/sb_mtu.c \
    $(IN_DIR)/sb_port.c


# Product objects-------------------------------------------------------
sbOBJS= \
    $(OUT_DIR)/sb_bdy1.$(OBJ) \
    $(OUT_DIR)/sb_bdy2.$(OBJ) \
    $(OUT_DIR)/sb_bdy3.$(OBJ) \
    $(OUT_DIR)/sb_bdy4.$(OBJ) \
    $(OUT_DIR)/sb_ex_ms.$(OBJ) \
    $(OUT_DIR)/sb_id.$(OBJ) \
    $(OUT_DIR)/sb_ptmi.$(OBJ) \
    $(OUT_DIR)/sb_ptui.$(OBJ) \
    $(OUT_DIR)/sb_ptli.$(OBJ) \
    $(OUT_DIR)/sb_mtu.$(OBJ) \
    $(OUT_DIR)/sb_port.$(OBJ) 


SBCM_INC=$(IN_DIR)/sb.h $(IN_DIR)/sb.x $(IN_DIR)/sb_mtu.h $(IN_DIR)/sb_mtu.x $(IN_DIR)/sb_port.h $(IN_DIR)/sb_port.x $(IN_DIR)/sb_err.h

ALL_INC=$(SBCM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/sb_bdy1.$(OBJ): $(IN_DIR)/sb_bdy1.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/sb_bdy1.o $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) \
	$(IN_DIR)/sb_bdy1.c
$(OUT_DIR)/sb_bdy2.$(OBJ): $(IN_DIR)/sb_bdy2.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_bdy2.c -o \
	$(OUT_DIR)/sb_bdy2.$(OBJ)
$(OUT_DIR)/sb_bdy3.$(OBJ): $(IN_DIR)/sb_bdy3.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_bdy3.c -o \
	$(OUT_DIR)/sb_bdy3.$(OBJ)
$(OUT_DIR)/sb_bdy4.$(OBJ): $(IN_DIR)/sb_bdy4.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_bdy4.c -o \
	$(OUT_DIR)/sb_bdy4.$(OBJ)
$(OUT_DIR)/sb_ex_ms.$(OBJ): $(IN_DIR)/sb_ex_ms.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_ex_ms.c -o \
	$(OUT_DIR)/sb_ex_ms.$(OBJ)
$(OUT_DIR)/sb_id.$(OBJ): $(IN_DIR)/sb_id.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_id.c -o \
	$(OUT_DIR)/sb_id.$(OBJ)
$(OUT_DIR)/sb_ptmi.$(OBJ): $(IN_DIR)/sb_ptmi.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_ptmi.c -o \
	$(OUT_DIR)/sb_ptmi.$(OBJ)
$(OUT_DIR)/sb_ptui.$(OBJ): $(IN_DIR)/sb_ptui.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_ptui.c -o \
	$(OUT_DIR)/sb_ptui.$(OBJ)
$(OUT_DIR)/sb_ptli.$(OBJ): $(IN_DIR)/sb_ptli.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_ptli.c -o \
	$(OUT_DIR)/sb_ptli.$(OBJ)
$(OUT_DIR)/sb_mtu.$(OBJ): $(IN_DIR)/sb_mtu.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_mtu.c -o \
	$(OUT_DIR)/sb_mtu.$(OBJ)
$(OUT_DIR)/sb_port.$(OBJ): $(IN_DIR)/sb_port.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_port.c -o \
	$(OUT_DIR)/sb_port.$(OBJ)

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libsb.a:$(sbOBJS)
	$(AR) -cr $(OUT_DIR)/libsb.a $(sbOBJS)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libsb.a $(sbOBJS)


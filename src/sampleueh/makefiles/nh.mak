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

nhSRCS= \
        $(IN_DIR)/nh_brm.c \
        $(IN_DIR)/nh_ccm.h \
        $(IN_DIR)/nh_cgm.c \
        $(IN_DIR)/nh_cpm.c \
        $(IN_DIR)/nh_db.c \
        $(IN_DIR)/nh_3gdb.c \
        $(IN_DIR)/nh_dbm.c\
        $(IN_DIR)/nh_edm.c \
        $(IN_DIR)/nh_ex_ms.c \
        $(IN_DIR)/nh_id.c \
        $(IN_DIR)/nh_init.c \
        $(IN_DIR)/nh_lim.c \
        $(IN_DIR)/nh_lmm.c \
        $(IN_DIR)/nh_pim.c \
        $(IN_DIR)/nh_ptli.c \
        $(IN_DIR)/nh_ptmi.c \
        $(IN_DIR)/nh_ptui.c \
        $(IN_DIR)/nh_rhm.c \
        $(IN_DIR)/nh_rtm.c \
        $(IN_DIR)/nh_uim.c \
        $(IN_DIR)/nh_utl.c 

nhOBJS= \
	$(OUT_DIR)/nh_brm.$(OBJ) \
	$(OUT_DIR)/nh_ccm.$(OBJ) \
	$(OUT_DIR)/nh_cgm.$(OBJ) \
	$(OUT_DIR)/nh_cpm.$(OBJ) \
	$(OUT_DIR)/nh_db.$(OBJ) \
	$(OUT_DIR)/nh_3gdb.$(OBJ) \
	$(OUT_DIR)/nh_dbm.$(OBJ) \
	$(OUT_DIR)/nh_edm.$(OBJ) \
	$(OUT_DIR)/nh_ex_ms.$(OBJ) \
	$(OUT_DIR)/nh_id.$(OBJ) \
	$(OUT_DIR)/nh_init.$(OBJ) \
	$(OUT_DIR)/nh_lim.$(OBJ) \
	$(OUT_DIR)/nh_lmm.$(OBJ) \
	$(OUT_DIR)/nh_pim.$(OBJ) \
	$(OUT_DIR)/nh_ptli.$(OBJ) \
	$(OUT_DIR)/nh_ptmi.$(OBJ) \
	$(OUT_DIR)/nh_ptui.$(OBJ) \
	$(OUT_DIR)/nh_rhm.$(OBJ) \
	$(OUT_DIR)/nh_rtm.$(OBJ) \
	$(OUT_DIR)/nh_uim.$(OBJ) \
	$(OUT_DIR)/nh_utl.$(OBJ)
	
NHCM_INC= \
    $(IN_DIR)/nh.h $(IN_DIR)/nh.x $(IN_DIR)/nh_db.x $(IN_DIR)/nh_3gdb.x

ALL_INC=$(NHCM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
$(OUT_DIR)/nh_brm.$(OBJ): $(IN_DIR)/nh_brm.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_brm.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_brm.c
$(OUT_DIR)/nh_ccm.$(OBJ): $(IN_DIR)/nh_ccm.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_ccm.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_ccm.c
$(OUT_DIR)/nh_cgm.$(OBJ): $(IN_DIR)/nh_cgm.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_cgm.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_cgm.c
$(OUT_DIR)/nh_cpm.$(OBJ): $(IN_DIR)/nh_cpm.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_cpm.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_cpm.c
$(OUT_DIR)/nh_db.$(OBJ): $(IN_DIR)/nh_db.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_db.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_db.c
$(OUT_DIR)/nh_3gdb.$(OBJ): $(IN_DIR)/nh_3gdb.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_3gdb.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
        $(IN_DIR)/nh_3gdb.c

$(OUT_DIR)/nh_dbm.$(OBJ): $(IN_DIR)/nh_dbm.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_dbm.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_dbm.c
$(OUT_DIR)/nh_edm.$(OBJ): $(IN_DIR)/nh_edm.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_edm.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_edm.c
$(OUT_DIR)/nh_ex_ms.$(OBJ): $(IN_DIR)/nh_ex_ms.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_ex_ms.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_ex_ms.c
$(OUT_DIR)/nh_id.$(OBJ): $(IN_DIR)/nh_id.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_id.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_id.c
$(OUT_DIR)/nh_init.$(OBJ): $(IN_DIR)/nh_init.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_init.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_init.c
$(OUT_DIR)/nh_lim.$(OBJ): $(IN_DIR)/nh_lim.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_lim.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_lim.c
$(OUT_DIR)/nh_lmm.$(OBJ): $(IN_DIR)/nh_lmm.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_lmm.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_lmm.c
$(OUT_DIR)/nh_pim.$(OBJ): $(IN_DIR)/nh_pim.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_pim.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_pim.c
$(OUT_DIR)/nh_ptli.$(OBJ): $(IN_DIR)/nh_ptli.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_ptli.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_ptli.c
$(OUT_DIR)/nh_ptmi.$(OBJ): $(IN_DIR)/nh_ptmi.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_ptmi.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_ptmi.c
$(OUT_DIR)/nh_ptui.$(OBJ): $(IN_DIR)/nh_ptui.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_ptui.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_ptui.c
$(OUT_DIR)/nh_rhm.$(OBJ): $(IN_DIR)/nh_rhm.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_rhm.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_rhm.c
$(OUT_DIR)/nh_rtm.$(OBJ): $(IN_DIR)/nh_rtm.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_rtm.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_rtm.c
$(OUT_DIR)/nh_uim.$(OBJ): $(IN_DIR)/nh_uim.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_uim.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_uim.c
$(OUT_DIR)/nh_utl.$(OBJ): $(IN_DIR)/nh_utl.c $(ALL_INC)
	$(CC) -c -o$(OUT_DIR)/nh_utl.o $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_utl.c


#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libnh.a:$(nhOBJS)
	$(AR) -cr $(OUT_DIR)/libnh.a $(nhOBJS)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libnh.a $(nhOBJS)


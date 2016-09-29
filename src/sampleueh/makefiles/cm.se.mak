#-------------------------------------------------------------#
#Makefile for product cm - script generated.
#Only the $(CCcmFLAGS) may be modified.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCcmFLAGS=-DSS_MT_TMR -ULTE_TDD -DSS_FLOAT#<---Insert cm specific defines here

VE_DIR=../lteenb
#-------------------------------------------------------------#
#File/Obj macros             
#-------------------------------------------------------------#

cmOBJS= \
	$(OUT_DIR)/ckw.$(OBJ)\
	$(OUT_DIR)/cm_bdy5.$(OBJ)\
	$(OUT_DIR)/cm_gen.$(OBJ)\
	$(OUT_DIR)/cm_hash.$(OBJ)\
	$(OUT_DIR)/cm_math.$(OBJ)\
	$(OUT_DIR)/cm_lib.$(OBJ)\
	$(OUT_DIR)/cm_llist.$(OBJ)\
	$(OUT_DIR)/cm_lte.$(OBJ)\
	$(OUT_DIR)/cm_mblk.$(OBJ)\
	$(OUT_DIR)/ve_se_tst.$(OBJ)\
	$(OUT_DIR)/cpj.$(OBJ)\
	$(OUT_DIR)/crg.$(OBJ)\
	$(OUT_DIR)/kwu.$(OBJ)\
	$(OUT_DIR)/lkw.$(OBJ)\
	$(OUT_DIR)/lrg.$(OBJ)\
	$(OUT_DIR)/pju.$(OBJ)\
	$(OUT_DIR)/rgu.$(OBJ)\
	$(OUT_DIR)/rgr.$(OBJ)\
	$(OUT_DIR)/ctf.$(OBJ)\
	$(OUT_DIR)/tfu.$(OBJ)\
	$(OUT_DIR)/lys.$(OBJ)\
	$(OUT_DIR)/lpj.$(OBJ)

SCTOPTS=-DLCSCT 
LRGOPTS= -DLCSMMILRG -DLCLRG -DSM -DRG -DLCRGMILRG
RGROPTS=-DRG -DLCVELIRGR -DLCRGR -DLWLCVELIRGR -DLCRGUIRGR
CTFOPTS=-DLCCTF -DVE -DLCVELICTF -DLCYSUICTF
LPJOPTS=-DPJ -DSM -DLCLPJ -DLCSMMILPJ -DLCPJMILPJ -DKW_PDCP
LKWOPTS=-DKW -DSM -DLCLKW -DLCSMMILKW -DLCKWMILKW -DKW_PDCP
PJUOPTS=-DPJ -DLCPJU -DVE -DNX -DNH -DLCVELIPJU -DLCPJUIPJU
CKWOPTS=-DLCNHLICKW -DLCCKW -DLCKWUICKW -DKW
CPJOPTS=-DPJ -DLCCPJ -DNH -DLCNHLICPJ -DLCPJUICPJ
CRGOPTS= -DLCNHLICRG -DLCCRG -DLCRGUICRG -DRG
KWUOPTS=-DKW -DNH -DLCKWUIKWU -DLCKWU -DLCNHLIKWU -DCCPU_OPT
RGUOPTS= -DRG -DKW -DLCKWLIRGU -DLCRGU -DLCRGUIRGU -DCCPU_OPT
TFUOPTS=-UTFU_TDD -DTFU_VER_2 -DLCTFU
LYSOPTS=-DYS -DLCLYS -DLCSMMILYS -DLCYSMILYS

#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
$(OUT_DIR)/ckw.$(OBJ): $(IN_DIR)/ckw.c $(CM_INC)
	$(CC) -c -o $(OUT_DIR)/ckw.o $(COPTS) $(IOPTS) $(POPTS) $(CKWOPTS) $(CCcmFLAGS) $(IN_DIR)/ckw.c
$(OUT_DIR)/cm_bdy5.$(OBJ): $(IN_DIR)/cm_bdy5.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_bdy5.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_bdy5.c
$(OUT_DIR)/cm_dns.$(OBJ): $(IN_DIR)/cm_dns.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_dns.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_dns.c
$(OUT_DIR)/cm_gen.$(OBJ): $(IN_DIR)/cm_gen.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_gen.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_gen.c
$(OUT_DIR)/cm_hash.$(OBJ): $(IN_DIR)/cm_hash.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_hash.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_hash.c
$(OUT_DIR)/cm_math.$(OBJ): $(IN_DIR)/cm_math.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_math.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_math.c
$(OUT_DIR)/cm_inet.$(OBJ): $(IN_DIR)/cm_inet.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_inet.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_inet.c
$(OUT_DIR)/cm_lib.$(OBJ): $(IN_DIR)/cm_lib.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_lib.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_lib.c
$(OUT_DIR)/cm_llist.$(OBJ): $(IN_DIR)/cm_llist.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_llist.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_llist.c
$(OUT_DIR)/cm_lte.$(OBJ): $(IN_DIR)/cm_lte.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_lte.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_lte.c
$(OUT_DIR)/cm_mblk.$(OBJ): $(IN_DIR)/cm_mblk.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_mblk.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_mblk.c
$(OUT_DIR)/cm_pasn1.$(OBJ): $(IN_DIR)/cm_pasn1.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_pasn1.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_pasn1.c
$(OUT_DIR)/cm_pasn2.$(OBJ): $(IN_DIR)/cm_pasn2.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_pasn2.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_pasn2.c
$(OUT_DIR)/cm_pasn3.$(OBJ): $(IN_DIR)/cm_pasn3.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_pasn3.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_pasn3.c
$(OUT_DIR)/cm_pasn4.$(OBJ): $(IN_DIR)/cm_pasn4.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_pasn4.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_pasn4.c
$(OUT_DIR)/cm_tkns.$(OBJ): $(IN_DIR)/cm_tkns.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_tkns.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_tkns.c
$(OUT_DIR)/cm_tpt.$(OBJ): $(IN_DIR)/cm_tpt.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_tpt.o $(COPTS) $(IOPTS) $(SCTOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_tpt.c
$(OUT_DIR)/cpj.$(OBJ): $(IN_DIR)/cpj.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cpj.o $(COPTS) $(IOPTS) $(POPTS) $(CPJOPTS) $(CCcmFLAGS) $(IN_DIR)/cpj.c
$(OUT_DIR)/ve_se_tst.$(OBJ): $(VE_DIR)/ve_se_tst.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/ve_se_tst.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(VE_DIR)/ve_se_tst.c
$(OUT_DIR)/crg.$(OBJ): $(IN_DIR)/crg.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/crg.o $(COPTS) $(IOPTS) $(POPTS) $(CRGOPTS) $(CCcmFLAGS) $(IN_DIR)/crg.c
$(OUT_DIR)/kwu.$(OBJ): $(IN_DIR)/kwu.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/kwu.o $(COPTS) $(IOPTS) $(POPTS) $(KWUOPTS) $(CCcmFLAGS) $(IN_DIR)/kwu.c
$(OUT_DIR)/lkw.$(OBJ): $(IN_DIR)/lkw.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/lkw.o $(COPTS) $(IOPTS) $(POPTS) $(LKWOPTS) $(CCcmFLAGS) $(IN_DIR)/lkw.c
$(OUT_DIR)/lrg.$(OBJ): $(IN_DIR)/lrg.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/lrg.o $(COPTS) $(IOPTS) $(POPTS) $(LRGOPTS) $(CCcmFLAGS) $(IN_DIR)/lrg.c
$(OUT_DIR)/pju.$(OBJ): $(IN_DIR)/pju.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/pju.o $(COPTS) $(IOPTS) $(POPTS) $(PJUOPTS) $(CCcmFLAGS) $(IN_DIR)/pju.c
$(OUT_DIR)/rgu.$(OBJ): $(IN_DIR)/rgu.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/rgu.o $(COPTS) $(IOPTS) $(POPTS) $(RGUOPTS) $(CCcmFLAGS) $(IN_DIR)/rgu.c
$(OUT_DIR)/rgr.$(OBJ): $(IN_DIR)/rgr.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/rgr.o $(COPTS) $(IOPTS) $(POPTS) $(RGROPTS) $(CCcmFLAGS) $(IN_DIR)/rgr.c
$(OUT_DIR)/ctf.$(OBJ): $(IN_DIR)/ctf.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/ctf.o $(COPTS) $(IOPTS) $(POPTS) $(CTFOPTS) $(CCcmFLAGS) $(IN_DIR)/ctf.c
$(OUT_DIR)/tfu.$(OBJ): $(IN_DIR)/tfu.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/tfu.o $(COPTS) $(IOPTS) $(POPTS) $(TFUOPTS) $(CCcmFLAGS) $(IN_DIR)/tfu.c
$(OUT_DIR)/lpj.$(OBJ): $(IN_DIR)/lpj.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/lpj.o $(COPTS) $(IOPTS) $(POPTS) $(LPJOPTS) $(CCcmFLAGS) $(IN_DIR)/lpj.c
$(OUT_DIR)/lys.$(OBJ): $(IN_DIR)/lys.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/lys.o $(COPTS) $(IOPTS) $(POPTS) $(LYSOPTS) $(CCcmFLAGS) $(IN_DIR)/lys.c


#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libcm.a:$(cmOBJS)
	$(AR) -cr libcm.a $(cmOBJS)	
#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libcm.a $(cmOBJS)


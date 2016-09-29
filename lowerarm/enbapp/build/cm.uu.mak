#-------------------------------------------------------------#
#Makefile for product cm - script generated.
#Only the $(CCcmFLAGS) may be modified.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCcmFLAGS=-DSS_MT_TMR -ULTE_TDD -DSS_FLOAT -DxRGR_V1#<---Insert cm specific defines here

CCsmFLAGS=-UHI -USB -UEG -DVE -DKW -DKW_PDCP -DPJ -USZ -DYS -DRG -DNH
#-------------------------------------------------------------#
#File/Obj macros             
#-------------------------------------------------------------#

cmOBJS= \
	$(OUT_DIR)/ckw.$(OBJ)\
	$(OUT_DIR)/cm_bdy5.$(OBJ)\
	$(OUT_DIR)/sm_bdy1.$(OBJ)\
	$(OUT_DIR)/sm_ex_ms.$(OBJ)\
	$(OUT_DIR)/cm_mem.$(OBJ)\
	$(OUT_DIR)/cm_dns.$(OBJ)\
	$(OUT_DIR)/cm_gen.$(OBJ)\
	$(OUT_DIR)/cm_hash.$(OBJ)\
	$(OUT_DIR)/cm_math.$(OBJ)\
	$(OUT_DIR)/cm_inet.$(OBJ)\
	$(OUT_DIR)/cm_lib.$(OBJ)\
	$(OUT_DIR)/cm_llist.$(OBJ)\
	$(OUT_DIR)/cm_lte.$(OBJ)\
	$(OUT_DIR)/cm_mblk.$(OBJ)\
	$(OUT_DIR)/cm_os.$(OBJ)\
	$(OUT_DIR)/cm_pasn1.$(OBJ)\
	$(OUT_DIR)/cm_pasn2.$(OBJ)\
	$(OUT_DIR)/cm_pasn3.$(OBJ)\
	$(OUT_DIR)/cm_pasn4.$(OBJ)\
	$(OUT_DIR)/cm_tkns.$(OBJ)\
	$(OUT_DIR)/cm_tpt.$(OBJ)\
	$(OUT_DIR)/cpj.$(OBJ)\
	$(OUT_DIR)/crg.$(OBJ)\
	$(OUT_DIR)/kwu.$(OBJ)\
	$(OUT_DIR)/lkw.$(OBJ)\
	$(OUT_DIR)/lnh.$(OBJ)\
	$(OUT_DIR)/lrg.$(OBJ)\
	$(OUT_DIR)/leg.$(OBJ)\
	$(OUT_DIR)/egt.$(OBJ)\
	$(OUT_DIR)/lhi.$(OBJ)\
	$(OUT_DIR)/hit.$(OBJ)\
	$(OUT_DIR)/lsb.$(OBJ)\
	$(OUT_DIR)/sct.$(OBJ)\
	$(OUT_DIR)/lsz.$(OBJ)\
	$(OUT_DIR)/szt.$(OBJ)\
	$(OUT_DIR)/szt_pk.$(OBJ)\
	$(OUT_DIR)/szt_3gpk.$(OBJ)\
	$(OUT_DIR)/szt_unpk.$(OBJ)\
	$(OUT_DIR)/szt_3gunpk.$(OBJ)\
	$(OUT_DIR)/nhu.$(OBJ)\
	$(OUT_DIR)/nhu_pk.$(OBJ)\
	$(OUT_DIR)/nhu_unpk.$(OBJ)\
	$(OUT_DIR)/nhu_3gpk.$(OBJ)\
	$(OUT_DIR)/nhu_3gunpk.$(OBJ)\
	$(OUT_DIR)/pju.$(OBJ)\
	$(OUT_DIR)/rgu.$(OBJ)\
	$(OUT_DIR)/rgr.$(OBJ)\
	$(OUT_DIR)/ctf.$(OBJ)\
	$(OUT_DIR)/tfu.$(OBJ)\
	$(OUT_DIR)/lys.$(OBJ)\
	$(OUT_DIR)/lve.$(OBJ)\
	$(OUT_DIR)/lpj.$(OBJ)

cmUUOBJS= \
	$(OUT_DIR)/ckw.$(OBJ)\
	$(OUT_DIR)/cm_bdy5.$(OBJ)\
	$(OUT_DIR)/sm_bdy1.$(OBJ)\
	$(OUT_DIR)/sm_ex_ms.$(OBJ)\
	$(OUT_DIR)/cm_mem.$(OBJ)\
	$(OUT_DIR)/cm_dns.$(OBJ)\
	$(OUT_DIR)/cm_gen.$(OBJ)\
	$(OUT_DIR)/cm_hash.$(OBJ)\
	$(OUT_DIR)/cm_math.$(OBJ)\
	$(OUT_DIR)/cm_inet.$(OBJ)\
	$(OUT_DIR)/cm_lib.$(OBJ)\
	$(OUT_DIR)/cm_llist.$(OBJ)\
	$(OUT_DIR)/cm_lte.$(OBJ)\
	$(OUT_DIR)/cm_mblk.$(OBJ)\
	$(OUT_DIR)/cm_os.$(OBJ)\
	$(OUT_DIR)/cm_pasn1.$(OBJ)\
	$(OUT_DIR)/cm_pasn2.$(OBJ)\
	$(OUT_DIR)/cm_pasn3.$(OBJ)\
	$(OUT_DIR)/cm_pasn4.$(OBJ)\
	$(OUT_DIR)/cm_tkns.$(OBJ)\
	$(OUT_DIR)/cm_tpt.$(OBJ)\
	$(OUT_DIR)/cpj.$(OBJ)\
	$(OUT_DIR)/crg.$(OBJ)\
	$(OUT_DIR)/kwu.$(OBJ)\
	$(OUT_DIR)/lkw.$(OBJ)\
	$(OUT_DIR)/lnh.$(OBJ)\
	$(OUT_DIR)/lrg.$(OBJ)\
	$(OUT_DIR)/nhu.$(OBJ)\
	$(OUT_DIR)/nhu_pk.$(OBJ)\
	$(OUT_DIR)/nhu_unpk.$(OBJ)\
	$(OUT_DIR)/nhu_3gpk.$(OBJ)\
	$(OUT_DIR)/nhu_3gunpk.$(OBJ)\
	$(OUT_DIR)/pju.$(OBJ)\
	$(OUT_DIR)/rgu.$(OBJ)\
	$(OUT_DIR)/rgr.$(OBJ)\
	$(OUT_DIR)/ctf.$(OBJ)\
	$(OUT_DIR)/tfu.$(OBJ)\
	$(OUT_DIR)/lys.$(OBJ)\
	$(OUT_DIR)/lve.$(OBJ)\
	$(OUT_DIR)/lpj.$(OBJ)

EG_DIR= ../egtpu
SZ_DIR= ../s1ap
EGTOPTS=-DEG -DLCEGT -DLCEGUIEGT -DEGTP_U
LEGOPTS=-DLCSMMILEG -DLCLEG -DSM -DEG -DLCEGMILEG -DLCSMEGMILEG -DEGTP_U
LHIOPTS=-DLCHIMILHI -DSM -DLCLHI
HITOPTS=-DLCHIT -DLCSBLIHIT -DLCHIUIHIT -DHI_REL_1_4 -DLCEGLIHIT 
SCTOPTS=-DLCSCT 
LRGOPTS= -DLCSMMILRG -DLCLRG -DSM -DRG -DLCRGMILRG
LSBOPTS=-DLCLSB -DSM -DSB -DLSB4  -DLSB8 -DHI_REL_1_4
LSZOPTS=-DLCLSZ -DLCSCT -DSM -DSZ -DLCSMSZMILSZ  -DLCSZMILSZ -DSZ_ENB
SZTOPTS=-DSZ -DUZ -DLCUZLISZT -DLWLCUZLISZT -DLCSZUISZT -DLCSZT -DPTSZUISZT
RGROPTS=-DRG -DLCVELIRGR -DLCRGR -DLWLCVELIRGR -DLCRGUIRGR
CTFOPTS=-DLCCTF -DVE -DLCVELICTF -DLCYSUICTF
LNHOPTS=-DNH -DSM -DLCLNH -DLCSMMILNH -DLCSMNHMILNH -DLCNHMILNH
NHUOPTS=-DLCNHU -DVE -DNX -DLCVELINHU -DLCNXLINHU -DLCNHUINHU
LVEOPTS=-DVE -DSM -DLCLVE -DLCSMMILVE -DLCVEMILVE -DVE_RELAY -DEU_DAT_APP
LPJOPTS=-DPJ -DSM -DLCLPJ -DLCSMMILPJ -DLCPJMILPJ -DKW_PDCP
LKWOPTS=-DKW -DSM -DLCLKW -DLCSMMILKW -DLCKWMILKW -DKW_PDCP
PJUVEOPTS=-DPJ -DLCPJU -DVE -DNX -DNH -DLCVELIPJU -DLCPJUIPJU -DLCNHLIPJU
CKWOPTS=-DLCNHLICKW -DLCCKW -DLCKWUICKW -DKW -DNH
CPJOPTS=-DPJ -DLCCPJ -DNH -DLCNHLICPJ -DLCPJUICPJ
CRGOPTS= -DLCNHLICRG -DLCCRG -DLCRGUICRG -DRG
KWUOPTS=-DKW -DNH -DLCKWUIKWU -DLCKWU -DLCNHLIKWU -DCCPU_OPT
RGUOPTS= -DRG -DKW -DLCKWLIRGU -DLCRGU -DLCRGUIRGU -DCCPU_OPT
TFUOPTS=-UTFU_TDD -DTFU_VER_2 -DLCTFU 
LYSOPTS=-DYS -DLCLYS -DLCSMMILYS -DLCYSMILYS

LYSOPTS=-DYS -DLCLYS -DLCSMMILYS -DLCYSMILYS
#EGT_INC= \
#    $(EG_DIR)/eg.h $(EG_DIR)/eg_util.h $(EG_DIR)/eg_util.x
	
#LSZ_INC= \
#    $(SZ_DIR)/sz.h $(SZ_DIR)/sz_err.h 
	
EGALL_INC=$(EGT_INC) $(CM_INC)

#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
$(OUT_DIR)/ckw.$(OBJ): $(IN_DIR)/ckw.c $(CM_INC)
	$(CC) -c -o $(OUT_DIR)/ckw.o $(COPTS) $(IOPTS) $(POPTS) $(CKWOPTS) $(CCcmFLAGS) $(IN_DIR)/ckw.c
$(OUT_DIR)/cm_bdy5.$(OBJ): $(IN_DIR)/cm_bdy5.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_bdy5.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_bdy5.c
$(OUT_DIR)/sm_bdy1.$(OBJ): $(IN_DIR)/sm_bdy1.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/sm_bdy1.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(CCsmFLAGS) $(IN_DIR)/sm_bdy1.c
$(OUT_DIR)/sm_ex_ms.$(OBJ): $(IN_DIR)/sm_ex_ms.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/sm_ex_ms.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(CCsmFLAGS) $(IN_DIR)/sm_ex_ms.c
$(OUT_DIR)/cm_mem.$(OBJ): $(IN_DIR)/cm_mem.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_mem.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_mem.c
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
$(OUT_DIR)/cm_os.$(OBJ): $(IN_DIR)/cm_os.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/cm_os.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_os.c
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
$(OUT_DIR)/crg.$(OBJ): $(IN_DIR)/crg.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/crg.o $(COPTS) $(IOPTS) $(POPTS) $(CRGOPTS) $(CCcmFLAGS) $(IN_DIR)/crg.c
$(OUT_DIR)/kwu.$(OBJ): $(IN_DIR)/kwu.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/kwu.o $(COPTS) $(IOPTS) $(POPTS) $(KWUOPTS) $(CCcmFLAGS) $(IN_DIR)/kwu.c
$(OUT_DIR)/lkw.$(OBJ): $(IN_DIR)/lkw.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/lkw.o $(COPTS) $(IOPTS) $(POPTS) $(LKWOPTS) $(CCcmFLAGS) $(IN_DIR)/lkw.c
$(OUT_DIR)/lnh.$(OBJ): $(IN_DIR)/lnh.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/lnh.o $(COPTS) $(IOPTS) $(POPTS) $(LNHOPTS) $(CCcmFLAGS) $(IN_DIR)/lnh.c
$(OUT_DIR)/lrg.$(OBJ): $(IN_DIR)/lrg.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/lrg.o $(COPTS) $(IOPTS) $(POPTS) $(LRGOPTS) $(CCcmFLAGS) $(IN_DIR)/lrg.c
$(OUT_DIR)/nhu.$(OBJ): $(IN_DIR)/nhu.c $(CM_INC) 
	$(CC) -c  -o $(OUT_DIR)/nhu.o $(COPTS) $(IOPTS) -DNX $(POPTS) $(NHUOPTS) $(CCcmFLAGS) $(IN_DIR)/nhu.c
$(OUT_DIR)/nhu_pk.$(OBJ): $(IN_DIR)/nhu_pk.c $(CM_INC) 
	$(CC) -c  -o $(OUT_DIR)/nhu_pk.o $(COPTS) $(IOPTS) -DNX $(POPTS) $(NHUOPTS) $(CCcmFLAGS) $(IN_DIR)/nhu_pk.c
$(OUT_DIR)/nhu_unpk.$(OBJ): $(IN_DIR)/nhu_unpk.c $(CM_INC) 
	$(CC) -c  -o $(OUT_DIR)/nhu_unpk.o $(COPTS) $(IOPTS) -DNX $(POPTS) $(NHUOPTS) $(CCcmFLAGS) $(IN_DIR)/nhu_unpk.c
$(OUT_DIR)/nhu_3gpk.$(OBJ): $(IN_DIR)/nhu_3gpk.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/nhu_3gpk.o $(COPTS) $(IOPTS) -DNX $(POPTS) $(NHUOPTS) $(CCcmFLAGS) $(IN_DIR)/nhu_3gpk.c
$(OUT_DIR)/nhu_3gunpk.$(OBJ): $(IN_DIR)/nhu_3gunpk.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/nhu_3gunpk.o $(COPTS) $(IOPTS) -DNX $(POPTS) $(NHUOPTS) $(CCcmFLAGS) $(IN_DIR)/nhu_3gunpk.c

$(OUT_DIR)/pju.$(OBJ): $(IN_DIR)/pju.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/pju.o $(COPTS) $(IOPTS) $(POPTS) $(PJUVEOPTS) $(CCcmFLAGS) $(IN_DIR)/pju.c
$(OUT_DIR)/rgu.$(OBJ): $(IN_DIR)/rgu.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/rgu.o $(COPTS) $(IOPTS) $(POPTS) $(RGUOPTS) $(CCcmFLAGS) $(IN_DIR)/rgu.c
$(OUT_DIR)/rgr.$(OBJ): $(IN_DIR)/rgr.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/rgr.o $(COPTS) $(IOPTS) $(POPTS) $(RGROPTS) $(CCcmFLAGS) $(IN_DIR)/rgr.c
$(OUT_DIR)/ctf.$(OBJ): $(IN_DIR)/ctf.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/ctf.o $(COPTS) $(IOPTS) $(POPTS) $(CTFOPTS) $(CCcmFLAGS) $(IN_DIR)/ctf.c
$(OUT_DIR)/tfu.$(OBJ): $(IN_DIR)/tfu.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/tfu.o $(COPTS) $(IOPTS) $(POPTS) $(TFUOPTS) $(CCcmFLAGS) $(IN_DIR)/tfu.c
$(OUT_DIR)/lve.$(OBJ): $(IN_DIR)/lve.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/lve.o $(COPTS) $(IOPTS) $(POPTS) $(LVEOPTS) $(CCcmFLAGS) $(IN_DIR)/lve.c
$(OUT_DIR)/lpj.$(OBJ): $(IN_DIR)/lpj.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/lpj.o $(COPTS) $(IOPTS) $(POPTS) $(LPJOPTS) $(CCcmFLAGS) $(IN_DIR)/lpj.c
$(OUT_DIR)/lys.$(OBJ): $(IN_DIR)/lys.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/lys.o $(COPTS) $(IOPTS) $(POPTS) $(LYSOPTS) $(CCcmFLAGS) $(IN_DIR)/lys.c
$(OUT_DIR)/leg.$(OBJ): $(IN_DIR)/leg.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/leg.o $(COPTS) $(IOPTS) $(POPTS) $(LEGOPTS) $(CCcmFLAGS) $(IN_DIR)/leg.c
$(OUT_DIR)/egt.$(OBJ): $(IN_DIR)/egt.c $(EGALL_INC)
	$(CC) -c  -o $(OUT_DIR)/egt.o $(COPTS) $(IOPTS) $(POPTS) $(EGTOPTS) $(CCcmFLAGS) $(IN_DIR)/egt.c
$(OUT_DIR)/lhi.$(OBJ): $(IN_DIR)/lhi.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/lhi.o $(COPTS) $(IOPTS) $(POPTS) $(LHIOPTS) $(CCcmFLAGS) $(IN_DIR)/lhi.c
$(OUT_DIR)/hit.$(OBJ): $(IN_DIR)/hit.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/hit.o $(COPTS) $(IOPTS) $(POPTS) $(HITOPTS) $(CCcmFLAGS) $(IN_DIR)/hit.c
$(OUT_DIR)/lsb.$(OBJ): $(IN_DIR)/lsb.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/lsb.o $(COPTS) $(IOPTS) $(POPTS) $(LSBOPTS) $(CCcmFLAGS) $(IN_DIR)/lsb.c
$(OUT_DIR)/sct.$(OBJ): $(IN_DIR)/sct.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/sct.o $(COPTS) $(IOPTS) $(POPTS) $(SCTOPTS) $(CCcmFLAGS) $(IN_DIR)/sct.c
$(OUT_DIR)/lsz.$(OBJ): $(IN_DIR)/lsz.c $(CM_INC) $(LSZ_INC)
	$(CC) -c  -o $(OUT_DIR)/lsz.o $(COPTS) $(IOPTS) $(POPTS) $(LSZOPTS) $(CCcmFLAGS) $(IN_DIR)/lsz.c
$(OUT_DIR)/szt.$(OBJ): $(IN_DIR)/szt.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/szt.o $(COPTS) $(IOPTS) $(POPTS) $(SZTOPTS) $(CCcmFLAGS) $(IN_DIR)/szt.c
$(OUT_DIR)/szt_pk.$(OBJ): $(IN_DIR)/szt_pk.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/szt_pk.o $(COPTS) $(IOPTS) $(POPTS) $(SZTOPTS) $(CCcmFLAGS) $(IN_DIR)/szt_pk.c
$(OUT_DIR)/szt_3gpk.$(OBJ): $(IN_DIR)/szt_3gpk.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/szt_3gpk.o $(COPTS) $(IOPTS) $(POPTS) $(SZTOPTS) $(CCcmFLAGS) $(IN_DIR)/szt_3gpk.c
$(OUT_DIR)/szt_unpk.$(OBJ): $(IN_DIR)/szt_unpk.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/szt_unpk.o $(COPTS) $(IOPTS) $(POPTS) $(SZTOPTS) $(CCcmFLAGS) $(IN_DIR)/szt_unpk.c
$(OUT_DIR)/szt_3gunpk.$(OBJ): $(IN_DIR)/szt_3gunpk.c $(CM_INC)
	$(CC) -c  -o $(OUT_DIR)/szt_3gunpk.o $(COPTS) $(IOPTS) $(POPTS) $(SZTOPTS) $(CCcmFLAGS) $(IN_DIR)/szt_3gunpk.c





#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libcm.a:$(cmOBJS)
	ar -cr $(OUT_DIR)/libcm.a $(cmOBJS)	
$(OUT_DIR)/libcmuu.a:$(cmUUOBJS)
	ar -cr $(OUT_DIR)/libcm.a $(cmUUOBJS)	
#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libcm.a $(cmOBJS)


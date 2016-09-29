#/********************************************************************16**
#
#                         (c) COPYRIGHT 2009 by
#                         Continuous Computing Corporation.
#                         All rights reserved.
#
#     This software is confidential and proprietary to Continuous Computing
#     Corporation (CCPU).  No part of this software may be reproduced,
#     stored, transmitted, disclosed or used in any form or by any means
#     other than as expressly provided by the written Software License
#     Agreement between CCPU and its licensee.
#
#     CCPU warrants that for a period, as provided by the written
#     Software License Agreement between CCPU and its licensee, this
#     software will perform substantially to CCPU specifications as
#     published at the time of shipment, exclusive of any updates or
#     upgrades, and the media used for delivery of this software will be
#     free from defects in materials and workmanship.  CCPU also warrants
#     that has the corporate authority to enter into and perform under the
#     Software License Agreement and it is the copyright owner of the software
#     as originally delivered to its licensee.
#
#     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
#     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
#     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
#     MATERIALS.
#
#     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
#     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
#     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
#     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
#     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
#     OF THE POSSIBILITY OF SUCH DAMAGE.
#
#                       Restricted Rights Legend
#
#     This software and all related materials licensed hereby are
#     classified as "restricted computer software" as defined in clause
#     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
#     developed entirely at private expense for nongovernmental purposes,
#     are commercial in nature and have been regularly used for
#     nongovernmental purposes, and, to the extent not published and
#     copyrighted, are trade secrets and confidential and are provided
#     with all rights reserved under the copyright laws of the United
#     States.  The government's rights to the software and related
#     materials are limited and restricted as provided in clause
#     52.227-19 of the FAR.
#
#                    IMPORTANT LIMITATION(S) ON USE
#
#     The use of this software is limited to the use set
#     forth in the written Software License Agreement between CCPU and
#     its Licensee. Among other things, the use of this software
#     may be limited to a particular type of Designated Equipment, as
#     defined in such Software License Agreement.
#     Before any installation, use or transfer of this software, please
#     consult the written Software License Agreement or contact CCPU at
#     the location set forth below in order to confirm that you are
#     engaging in a permissible use of the software.
#
#                    Continuous Computing Corporation
#                    9380, Carroll Park Drive
#                    San Diego, CA-92121, USA
#
#                    Tel: +1 (858) 882 8800
#                    Fax: +1 (858) 777 3388
#
#                    Email: support@trillium.com
#                    Web: http://www.ccpu.com
#
#*********************************************************************17*/
#
#********************************************************************20**
#
#       Name:   LTE eNodeB Sample Application
#
#       Type:   make file
#
#       Desc:   Compile, assemble and link product software for
#               various configurations. Further comments are
#               embedded in the file.
#
#               This file supports a variety of environments and
#               build targets. The default build target will be the
#               portable target for the Linu 2.4.x with GNU C(gcc)
#
#       Env:    Linux 2.4.x with GNU C (gcc)
#
#               BUILD TARGETS:
#
#               clean            remove all object files
#
#       File:  stack.mak
#
#
#       Prg:   an
#
#********************************************************************21*/
#=======================================================================

#-----------------------------------------------------------------------
# macro for output file name and makefile name
#
ROOT=enodeb
SY_DIR=./obj
SY_DIR3=$(SY_DIR)

SY_LIB_DIR=.
STOPTS=-DENB

OUTNAME=$(ROOT)
OUTNAME1=$(ROOT1)
OUTNAME2=$(ROOT2)

#-----------------------------------------------------------------------
# macros for td specific options
#

# MTSS defines
MTOPTS=-DSS -DSS_MT -DCONRD -DNOFILESYS
MTOPTS2=-DSS -DSS_MT -DCONRD

#cavium includes
OBJ_DIR=./obj1

# MOS defines
MSOPTS=-DMS

#SSI Files
SSOPTS=-DSS

CMFLAGS=-DSS_MT_TMR -DSS_FLOAT -ULTE_TDD -DMSPD#<---Insert cm specific defines here
SMFLAGS=-UHI -USB -UEG -UVE -DKW -DKW_PDCP -DPJ -USZ -DYS -DRG -DNH
MTFLAGS=-DBIT_64 -DSS_MT_TMR -DAPI_MAIN -DSS_TICKS_SEC=1000 #<---Insert mt specific defines here
YSFLAGS=-DTF -DYS -DYS_LTE_PAL -DLTE_PAL_ENB -UYS_PERF_MEAS -DLCCTF -DYS_MSPD\
        -DLCYSUITFU -DLCTFU -DLCTFUITFU -DTFU_VER_2 \
        -UTFU_TDD -DRG -DLTE_PAL_ENB -DLCYSUICTF -DAPI_MAIN \
	-DLCYSMILYS -DLCLYS -UYS_MS_WAIT_FOR_TXCFM\
	-DYS_MS_PHY  -UYS_JMP_PHY_RUN_STE -DYS_MIB_WARND -DYS_PHY_STOP_AUTO -UYS_SIMUL_PUSCH_RECP -DYS_ALLOC_ON_NEED -UYS_RRC_CONN_SIMUL -UGEN_DUMMY_SR -ULTEMAC_MIMO -UADI_BAND_AUTOMATE
KWFLAGS= -DKW_PDCP -UNH -USM -DPTPJLIB -DLTE_HENB 
RGFLAGS=-DTFU_VER_2 -UTFU_TDD -USM -ULTE_TDD -DTF -DSS_FLOAT -DSS_MT_TMR -DRGR_RRM_TICK
C4GMXENBE2EOPTS= -DMX_ARM_ENABLED -DLOWERARM -DSCHEDULER -DUL_CEVA -DUSECEVA\
                 -DUSEFEC -DLTE -DLIST_BASED -D_MX_DEBUG -D_APP_ASSERT_HANDLER\
		  $(YSFLAGS) $(CMFLAGS) $(SMFLAGS) $(MTFLAGS) $(KWFLAGS) \
                  $(RGFLAGS) -USS_4GMX_SSI_PORTING -USSI_INCORRECT_FREE -USS_HISTOGRAM_SUPPORT -URUN_CL_ONLY -URUN_ICPU_ONLY $(MTOPTS) -USS_CAVIUM \
		  -DCMFILE_REORG_1 -DYS_IP_CFG_FRM_HDR_FILE -DNO_ERRCLS \
		  -DCMFIlE_REORG_2 -DSSINT2 -DCMKV2 -DSS_PERF -D SS_WL_REGION=1\
                  -DNOERRCHK -UCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU -UDEBUGP\
	          -ULTE_START_UL_DL_DATA -DLCYSUICTF -DCCPU_MLOG \
	          -UYS_ALL_PERF


# The options below are used to pass on to the actual product compilations
# Just this one variable is passed for individual compilations and hence
# all the needed options are added from various other defined option macros

#ALL PAL layer options\
TFUOPTS=-DTFU_VER_2 -UTFU_TDD -DLCTFU

# All LTE MAC product options
CRGOPTS= -DLCNHLICRG -DLCCRG -DLCRGUICRG
CRGOPTS1= -DLCNHLICRG -DLCCRG -DLCRGUICRG -DRG
RGUOPTS=-DRG -DKW -DLCKWLIRGU -DLCRGU -DLCRGUIRGU -DCCPU_OPT
LRGOPTS=-DLCSMMILRG -DLCLRG -DSM -DRG -DLCRGMILRG
RGROPTS=-DRG -DLCVELIRGR -DLCRGR -DLWLCVELIRGR -DLCRGUIRGR
RGRSEUMOPTS=-DLCVELIRGR -DLCRGR -DLWLCVELIRGR -DLCRGUIRGR
RGOPTS=-DRG -DRG_PHASE2_SCHED $(RGUOPTS) $(CRGOPTS) $(LRGOPTS) $(RGROPTS) $(TFUOPTS) -URG_DEBUG -UTTI_PROC -UPROC_DL -USCH_TTI_PROC -UTOM -UPAL_MAC_PROC -UUL_PROC_MAC -DLCRGLITFU -DRG_DL_DELTA_CHANGE -DRG_CMNCH_SCHDELTA_CHANGE -DRG_ULSCHED_AT_CRC -DTFU_RECPREQ_DELTA_CHANGE -DTFU_DL_DELTA_CHANGE -DTFU_UL_DELTA_CHANGE -DTFU_DLHQFBK_DELTA_CHANGE -UBUG_FIX

#All Convergence Layer options
LCLOPTS=-DLCLCL -DSM -DCL
CLOPTS=-DCL $(LCLOPTS) -DLCCLMILCL

#All RLC product options
CKWOPTS=-DLCNHLICKW -DLCCKW -DLCKWUICKW
CKWOPTS1=-DLCNHLICKW -DLCCKW -DLCKWUICKW -DKW
CKWOPTS2=-DLCNHLICKW -DLCCKW -DLCKWUICKW -DKW -DNH
KWUOPTS=-DKW -DNH -DLCKWUIKWU -DLCKWU -DLCNHLIKWU -DCCPU_OPT
LKWOPTS=-DSM -DKW -DLCKWMILKW -DLCLKW
LPJOPTS=-DSM -DPJ -DLCPJMILPJ -DLCLPJ
PJUVEOPTS=-DPJ -DNH -DLCPJU  -DLCVELIPJU -DLCPJUIPJU
PJUVESEUMOPTS=-DNH -DLCPJU  -DLCVELIPJU -DLCPJUIPJU
PJUVEOPTS2=-DPJ -DLCPJU -UVE -DPX -DNX -DNH -DLCVELIPJU -DLCPJUIPJU
PJUNHOPTS=-DPJ  -DLCPJU  -DLCNHLIPJU -DLCPJUIPJU
PJUNHOPTS2=-DPJ -DLCPJU -UVE -DPX -DNX -DNH -DLCNHLIPJU -DLCPJUIPJU
CPJOPTS=-DLCCPJ -DNH -DKW -DLCNHLICPJ -DLCPJUICPJ
KWOPTS=-DKW $(CKWOPTS) $(KWUOPTS) $(CPJOPTS) $(LPJOPTS) $(LKWOPTS) $(RGUOPTS) $(PJUNHOPTS) $(PJUVEOPTS) -URLC_STATUS_GEN -UUL_PROC_MAC
KWOPTS1=-DKW $(CKWOPTS1) $(KWUOPTS) $(CPJOPTS) $(LPJOPTS) $(LKWOPTS) $(RGUOPTS) $(PJUNHOPTS) $(PJUVEOPTS) -URLC_STATUS_GEN -UUL_PROC_MAC
KWOPTS2=-DKW $(CKWOPTS2) $(KWUOPTS) $(CPJOPTS) $(LPJOPTS) $(LKWOPTS) $(RGUOPTS) $(PJUNHOPTS2) $(PJUVEOPTS2) -URLC_STATUS_GEN -UUL_PROC_MAC

#-----------------------------------------------------------------------
# macros for sw and include directories
#
CODEPATH=../../
MTSE_DIR=$(CODEPATH)mt4gmx
CM_DIR=$(CODEPATH)cm
VE_DIR=$(CODEPATH)lteenb/src
YS_DIR=$(CODEPATH)ltecl
RG_DIR=$(CODEPATH)ltemac
KW_DIR=$(CODEPATH)lterlcpdcp
#MS_DIR=$(CODEPATH)/ltephy/mindSpeed/inc/
MS_CM_DIR=$(CODEPATH)/Common/
MS_PHYLTE_DIR=$(CODEPATH)ltephy/mindspeed/uppercore/LTE/C-Code/
MS_TP_DIR=$(CODEPATH)ltephy/mindspeed/uppercore/TurnerProject/
#CM_CMINC=../../common/include
L_CORE_DIR=$(CODEPATH)ltephy/mindspeed/lowercore/

BLD_4GMX_CM_OBJS=$(SY_DIR3)/lib4gmxcm.a
BLD_4GMX_SS_OBJS=$(SY_DIR3)/lib4gmxmt.a
BLD_4GMX_KW_OBJS=$(SY_DIR3)/lib4gmxkw.a
BLD_4GMX_RG_OBJS=$(SY_DIR3)/lib4gmxrg.a
BLD_4GMX_YS_OBJS=$(SY_DIR3)/lib4gmxys.a


CM_INC1=\
        -I$(CM_DIR)/envdep.h         -I$(CM_DIR)/envind.h         \
        -I$(CM_DIR)/envopt.h                                    \
        -I$(CM_DIR)/gen.h            -I$(CM_DIR)/gen.x            \
        -I$(CM_DIR)/cm_hash.x        -I$(CM_DIR)/cm_hash.h        \
        -I$(CM_DIR)/cm_lte.x        -I$(CM_DIR)/cm_lte.h        \
        -I$(CM_DIR)/cm5.h            -I$(CM_DIR)/cm5.x            \
        -I$(CM_DIR)/cm_umts.h        -I$(CM_DIR)/cm_umts.x        \
        -I$(CM_DIR)/cm_err.h         -I$(CM_DIR)/cm_lib.x         \
        -I$(CM_DIR)/cm_mblk.h        -I$(CM_DIR)/cm_mblk.x        \
        -I$(CM_DIR)/cm_tkns.h        -I$(CM_DIR)/cm_tkns.x        \
        -I$(CM_DIR)/cm_llist.h       -I$(CM_DIR)/cm_llist.x       \
        -I$(CM_DIR)/cm_hash.h        -I$(CM_DIR)/cm_hash.x        \
        -I$(CM_DIR)/cm_gen.h         -I$(CM_DIR)/cm_gen.x         \
        -I$(CM_DIR)/cm_tpt.h         -I$(CM_DIR)/cm_tpt.x         \
        -I$(CM_DIR)/cm_dns.h         -I$(CM_DIR)/cm_dns.x         \
        -I$(CM_DIR)/ssi.h            -I$(CM_DIR)/ssi.x            \
        -I$(CM_DIR)/nhu.h            -I$(CM_DIR)/nhu.x            \
        -I$(CM_DIR)/crg.h            -I$(CM_DIR)/crg.x            \
        -I$(CM_DIR)/kwu.h            -I$(CM_DIR)/kwu.x            \
        -I$(CM_DIR)/tfu.h            -I$(CM_DIR)/tfu.x            \
        -I$(CM_DIR)/rgr.h            -I$(CM_DIR)/rgr.x            \
        -I$(CM_DIR)/pju.h            -I$(CM_DIR)/pju.x            \
        -I$(CM_DIR)/cm_pint.h        -I$(CM_DIR)/cm_pint.x        \
        -I$(CM_DIR)/cm_perr.h                                   \
        -I$(CM_DIR)/lkw.h            -I$(CM_DIR)/lkw.x            \
        -I$(CM_DIR)/lrg.h            -I$(CM_DIR)/lrg.x            \
        -I$(CM_DIR)/lpj.h            -I$(CM_DIR)/lpj.x            \
        -I$(CM_DIR)/cm_pasn.h        -I$(CM_DIR)/cm_pasn.x        \
        -I$(CM_DIR)/cm_mem.h         -I$(CM_DIR)/cm_mem.x         \

#-----------------------------------------------------------------------
# macros for compile and link options

#LNXENV=-DSUNOS -DSS -DSS_MT -DANSI -D_GNU_SOURCE -DSS_LINUX -D_REENTRANT -D__EXTENSIONS__ \
    -DDEBUGNOEXIT

C4GMXENV=-DSUNOS -DSS -DSS_MT -DANSI -D_GNU_SOURCE -DSS_LINUX -D_REENTRANT -D__EXTENSIONS__ \
    -DDEBUGNOEXIT -DRB_DEBUG -DSS_4GMX_LCORE -DSS_DRVR_SUPPORT -DRB_SINGLE_CORE -DSS_TTI_TMR

# compiler:
#Changes for ARM-4GMX compilation

LNXCC = armcc
ARMAR = armar
LNXLD = armlink
LNXAS = armasm
CC=LNXCC

#LNXCOPTS=" -g3 -pipe -pedantic -Wall -Wno-comment -Wshadow -Wcast-qual \
-Wstrict-prototypes -Wmissing-prototypes -fno-strict-aliasing"

C4GMXCOPTS=" -O3 -pipe -Wall -Wno-comment -Wshadow -Wcast-qual \
-Wstrict-prototypes -Wmissing-prototypes -fno-strict-aliasing"

# object files:
# o     UNIX
OBJ=o                                    # object files, sunos

#######################################################################
# library path
#LNXLIBPATH= -L$(MS_PATH)/OSAL/LINUX
#C4GMXLIB1= \
	  $(L_CORE_DIR)/lower-core/lib/4GMXLIB_D.a \
	  $(L_CORE_DIR)/lower-core/lib/SYSCORE_D.a \
          $(L_CORE_DIR)/lower-core/lib/liblte.a
#######################################################################

# linker options:

#path for  MS libs and objs
#MS_PATH=../../Branch_v1_4-CCPU/TurnerProject
#MS_PATH=../../ltephy/mindspeed/uppercore/TurnerProject
MS_PATH=../../ltephy/mindspeed/lowerarm/common

C4GMXLOPTS=" lib/4GMXLIB_D.a lib/SYSCORE_D.a"
	   # ../lib/liblte.a
# include options:
#
# C4GMXIOPTS =  -I../inc -I../../../common -I../../common -I../common \
             -I../dsp-core/common/C-Code -I../dsp-core/lte/C-Code \
             -I../dsp-core/wimax/C-Code -I../../cm -I$(MS_PATH) -I$(MS_CM_DIR)\
             -I$(MS_PHYLTE_DIR) -I$(MS_TP_DIR) -I$(L_CORE_DIR)/inc/\
             -I$(KW_DIR) -I$(MTSE_DIR) -I$(RG_DIR) -I$(YS_DIR) -I$(CM_DIR)\
             -I$(L_CORE_DIR)/app/\

C4GMXIOPTS =  \
              -I$(L_CORE_DIR)/common \
             -I../../cm  \
             -I$(L_CORE_DIR)/T4KDev/4GMX/Inc \
             -I$(L_CORE_DIR)/T4KDev/SYSCORE/SERVICES \
             -I$(L_CORE_DIR)/T4KDev/4GMX/arm/inc \
             -I$(L_CORE_DIR)/T4KDev/SYSCORE/DRIVERs/ \
             -I$(L_CORE_DIR)/T4KDev/SYSCORE/DRIVERs/fec \
             -I$(L_CORE_DIR)/T4KDev/SYSCORE/DRIVERs/map \
             -I$(L_CORE_DIR)/T4KDev/SYSCORE/DRIVERs/cpri \
             -I$(L_CORE_DIR)/phy/lte/arm/ \
             -I$(L_CORE_DIR)/phy/lte/c-code/ \
             -I$(KW_DIR) -I$(MTSE_DIR) -I$(RG_DIR) -I$(YS_DIR) -I$(CM_DIR)

#             -I$(L_CORE_DIR)/lower-core/app/\

# compiler options:

# MTSS defines
CC4GMXMTOPTS=$(DEBUGPOPTS) $(C4GMXENV) $(MTOPTS) $(CMOPTS) $(STOPTS) -DOP_SSI_SAM

#KW defines
CC4GMXKWOPTS= $(C4GMXENV) $(KWOPTS)  $(STOPTS) $(CMOPTS) -DSS_4GMX_SSI_PORTING

#RG defines
CC4GMXRGOPTS= $(C4GMXENV) $(RGOPTS)  $(STOPTS) $(CMOPTS)

#common options
CC4GMXCMOPTS= $(C4GMXENV) $(CMOPTS) $(SMOPTS) \
         $(MKUOPTS) $(CMKOPTS)  \
         $(CRLOPTS) $(RLUOPTS)  $(STOPTS) \
         $(CTCOPTS) $(TCUOPTS) $(LRXOPTS)

ARFLAGS= --create -r
MSFLAGS4GMX = \
	     --apcs=norwpi/noswst \
             --library_interface=aeabi_clib \
             --arm \
	     --li \
             --cpu=Cortex-A9.no_neon.no_vfp \
	     --fpu=softvfp \
             --no_unaligned_access \
	     -g \
	     -O0 \
	     -c \
	     -Otime \
	     -Ono_memcpy

#CFLAGS4GMX = $(COPTS) $(C4GMXENBE2EOPTS) $(CC4GMXMTOPTS) $(CC4GMXKWOPTS) $(CC4GMXRGOPTS) $(CC4GMXCMOPTS) $(MSFLAGS4GMX) $(C4GMXIOPTS)
CFLAGS4GMX =  $(C4GMXENBE2EOPTS) $(CC4GMXMTOPTS) $(CC4GMXKWOPTS) $(CC4GMXRGOPTS) $(CC4GMXCMOPTS) $(MSFLAGS4GMX) $(C4GMXIOPTS)

ASFLAGS4GMX = --apcs=norwpi/noswst --cpu=Cortex-A9.no_neon.no_vfp -g


LDFLAGS= 						\
		--xref 					\
		--map 					\
		--list _lst.lst 			\
		--symdefs _sym.sym 			\
		--errors _lnk.err 			\
		--symbols 				\
		--Output $(BIN) 			\
		--remove 				\
	        --scatter prj.sct                       \
		--bestdebug

#		--via $(BIN).via			\
#		--entry 0x0+vectors_l.o(LVectors)


C4GMXPRDENB_E2E_OBJS: \
    $(BLD_4GMX_YS_OBJS) \
    $(BLD_4GMX_CM_OBJS) \
    $(BLD_4GMX_SS_OBJS) \
    $(BLD_4GMX_KW_OBJS) \
    $(BLD_4GMX_RG_OBJS)

C4GMXENBE2EBIN=enb_4gmx_e2e

BASENAME=enodeb
MAKENAME=$(BASENAME).mak

BIN4GMXE2E=4gmx_e2e
4gmx_e2e : enodeb.elf

BIN4GMXE2ELIB=4gmx_e2e_lib
4gmx_e2e_lib : enodeb.a

#
#
# Linux Default GNU acceptance test build
#
acc:
	@$(MAKE) -f $(MAKENAME) $(BLDENV)

#
#Set the parameters
COPTS=$(C4GMXCOPTS)
LOPTS='$(C4GMXLOPTS)'
POPTS='$(CC4GMXTFOPTS)'
IOPTS='$(C4GMXIOPTS)'
IN_DIR='$(YS_DIR)'
OUT_DIR='$(SY_DIR3)'

CMSRCS= \
	$(CM_DIR)/ckw.c\
	$(CM_DIR)/cm_bdy5.c\
	$(CM_DIR)/cm_gen.c\
	$(CM_DIR)/cm_hash.c\
	$(CM_DIR)/cm_math.c\
	$(CM_DIR)/cm_lib.c\
	$(CM_DIR)/cm_llist.c\
	$(CM_DIR)/cm_lte.c\
	$(CM_DIR)/cm_mblk.c\
	$(CM_DIR)/cm_pasn1.c\
	$(CM_DIR)/cm_pasn2.c\
	$(CM_DIR)/cm_pasn3.c\
	$(CM_DIR)/cm_pasn4.c\
	$(CM_DIR)/cm_tkns.c\
	$(CM_DIR)/cm_tpt.c\
	$(CM_DIR)/cpj.c\
	$(CM_DIR)/crg.c\
	$(CM_DIR)/kwu.c\
	$(CM_DIR)/lkw.c\
	$(CM_DIR)/lrg.c\
	$(CM_DIR)/nhu_pk.c\
	$(CM_DIR)/nhu_unpk.c\
	$(CM_DIR)/nhu_3gpk.c\
	$(CM_DIR)/nhu_3gunpk.c\
	$(CM_DIR)/pju.c\
	$(CM_DIR)/rgu.c\
	$(CM_DIR)/rgr.c\
	$(CM_DIR)/ctf.c\
	$(CM_DIR)/tfu.c\
	$(CM_DIR)/lys.c\
	$(CM_DIR)/lpj.c\
	$(CM_DIR)/ve_se_tst.c

MTSRCS= \
	$(CM_DIR)/cm_mem.c\
	$(MTSE_DIR)/rb_id.c\
	$(MTSE_DIR)/rb_ss.c\
	$(MTSE_DIR)/rb_icore.c\
	$(MTSE_DIR)/rb_icpu.c\
	$(MTSE_DIR)/rb_log.c\
	$(MTSE_DIR)/rb_timer.c\
	$(MTSE_DIR)/ss_mem.c\
	$(MTSE_DIR)/ss_msg.c\
	$(MTSE_DIR)/ss_gen.c\
	$(MTSE_DIR)/ss_pack.c\
	$(MTSE_DIR)/ss_strm.c\
	$(MTSE_DIR)/ss_task.c\
	$(MTSE_DIR)/ss_drvr.c\
	$(MTSE_DIR)/ss_timer.c

YSSRCS= \
    $(YS_DIR)/ys_ptui.c    \
    $(YS_DIR)/ys_ptmi.c    \
    $(YS_DIR)/ys_utl.c     \
    $(YS_DIR)/ys_init.c    \
    $(YS_DIR)/ys_mi.c      \
    $(YS_DIR)/ys_db.c      \
    $(YS_DIR)/ys_ms_utl.c  \
    $(YS_DIR)/ys_ms_cfg.c  \
    $(YS_DIR)/ys_ms_ui.c   \
    $(YS_DIR)/ys_ms_li.c   \
    $(YS_DIR)/ys_ms_map.c  \
    $(YS_DIR)/ys_ms_ul.c   \
    $(YS_DIR)/ys_ms_dl.c   \
    $(YS_DIR)/ys_dft.c     \
    $(YS_DIR)/ys_ex_ms.c   \
    $(YS_DIR)/ys_ms_siml.c   \
    $(YS_DIR)/ys_id.c

#    $(L_CORE_DIR)/lower-core/app/cevabincode.c

KWSRCS= \
    $(KW_DIR)/kw_amm.c \
    $(KW_DIR)/kw_cfg.c \
    $(KW_DIR)/kw_dbm.c \
    $(KW_DIR)/kw_ex_ms.c \
    $(KW_DIR)/kw_id.c \
    $(KW_DIR)/kw_lim.c \
    $(KW_DIR)/kw_lmm.c \
    $(KW_DIR)/kw_ptli.c \
    $(KW_DIR)/kw_ptmi.c \
    $(KW_DIR)/kw_ptui.c \
    $(KW_DIR)/kw_tmm.c \
    $(KW_DIR)/kw_tmr.c \
    $(KW_DIR)/kw_uim.c \
    $(KW_DIR)/kw_umm.c \
    $(KW_DIR)/kw_pj_lim.c \
    $(KW_DIR)/kw_pj_uim.c \
    $(KW_DIR)/kw_pj_lmm.c \
    $(KW_DIR)/kw_pj_cfg.c \
    $(KW_DIR)/kw_pj_dbm.c \
    $(KW_DIR)/kw_pj_dlm.c \
    $(KW_DIR)/kw_pj_ulm.c \
    $(KW_DIR)/kw_pj_utl.c \
    $(KW_DIR)/kw_pj_tmr.c \
    $(KW_DIR)/kw_pj_lib.c \
    $(KW_DIR)/kw_ptlib.c \
    $(KW_DIR)/kw_utl.c

RGSRCS= \
    $(RG_DIR)/rg_cfg.c \
    $(RG_DIR)/rg_com.c \
    $(RG_DIR)/rg_dbm.c \
    $(RG_DIR)/rg_dhm.c \
    $(RG_DIR)/rg_dux.c \
    $(RG_DIR)/rg_ex_ms.c \
    $(RG_DIR)/rg_id.c \
    $(RG_DIR)/rg_lim.c \
    $(RG_DIR)/rg_lmm.c \
    $(RG_DIR)/rg_mux.c \
    $(RG_DIR)/rg_ptli.c \
    $(RG_DIR)/rg_ptmi.c \
    $(RG_DIR)/rg_ptui.c \
    $(RG_DIR)/rg_ram.c \
    $(RG_DIR)/rg_rom.c \
    $(RG_DIR)/rg_sch.c \
    $(RG_DIR)/rg_tom.c \
    $(RG_DIR)/rg_uhm.c \
    $(RG_DIR)/rg_uim.c \
    $(RG_DIR)/rg_utl.c \
    $(RG_DIR)/rg_sch_cfg.c \
    $(RG_DIR)/rg_sch_cmn.c \
    $(RG_DIR)/rg_sch_dbm.c \
    $(RG_DIR)/rg_sch_gom.c \
    $(RG_DIR)/rg_sch_sc1.c \
    $(RG_DIR)/rg_sch_dhm.c \
    $(RG_DIR)/rg_sch_dlfs.c \
    $(RG_DIR)/rg_sch_ex_ms.c \
    $(RG_DIR)/rg_sch_inf.c \
    $(RG_DIR)/rg_sch_lmm.c \
    $(RG_DIR)/rg_sch_maxci.c \
    $(RG_DIR)/rg_sch_mga.c \
    $(RG_DIR)/rg_sch_pfs.c \
    $(RG_DIR)/rg_sch_pt.c \
    $(RG_DIR)/rg_sch_pwr.c \
    $(RG_DIR)/rg_sch_ram.c \
    $(RG_DIR)/rg_sch_rr.c \
    $(RG_DIR)/rg_sch_tmr.c \
    $(RG_DIR)/rg_sch_uhm.c \
    $(RG_DIR)/rg_sch_utl.c \
    $(RG_DIR)/rg_sch_tom.c \
    $(RG_DIR)/rg_sch_utl_clist.c

SRCS = $(CMSRCS)\
       $(MTSRCS)\
       $(YSSRCS)\
       $(KWSRCS)\
       $(RGSRCS)

OBJ4GMX = $(SRCS:.c=.o)

.c.o :
	$(LNXCC) $(CFLAGS4GMX) -c $< -o $@

#.s.o :
#	$(LNXAS) $(ASFLAGS4GMX) $< -o $@


enodeb.elf : $(C4GMXLIB) $(OBJ4GMX)
	$(LNXLD) $(LDFLAGS) $(C4GMXLIB) $(OBJ4GMX) -o $@

#enodeb.a : $(C4GMXLIB) $(OBJ4GMX)
	#$(ARMAR) $(ARFLAGS) ./enodeb.a $(C4GMXLIB) $(OBJ4GMX)

enodeb.a : $(OBJ4GMX)
	$(ARMAR) $(ARFLAGS) ./enodeb.a $(OBJ4GMX)


# Portable compilation -> Point to acc directly
# Portability is not supported

#-----------------------------------------------------------------------
# UNIX link - absolute object file depends on relocatable object
# (.o) files

#
# Making Common Libraries
#
#$(BLD_4GMX_CM_OBJS):
#	@$(MAKE) -f cm.mak $(SY_DIR3)/libcm.a COPTS=$(C4GMXCOPTS) \
	LOPTS='$(C4GMXLOPTS)' POPTS='$(CC4GMXCMOPTS)' IOPTS='$(C4GMXIOPTS)' IN_DIR='$(CM_DIR)' \
	OUT_DIR='$(SY_DIR3)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  CM_INC='$(CM_INC1)'

#Making RLC Layer
#
#$(BLD_4GMX_KW_OBJS):
#	@$(MAKE) -f kw.mak $(SY_DIR3)/libkw.a COPTS=$(C4GMXCOPTS) \
	LOPTS='$(C4GMXLOPTS)' POPTS='$(CC4GMXKWOPTS)' IOPTS='$(C4GMXIOPTS)' IN_DIR='$(KW_DIR)' \
	OUT_DIR='$(SY_DIR3)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

#
#Making MAC Layer
#
#$(BLD_4GMX_RG_OBJS):
#	@$(MAKE) -f rg.mak $(SY_DIR3)/librg.a COPTS=$(C4GMXCOPTS) \
	LOPTS='$(C4GMXLOPTS)' POPTS='$(CC4GMXRGOPTS)' IOPTS='$(C4GMXIOPTS)' IN_DIR='$(RG_DIR)' \
	OUT_DIR='$(SY_DIR3)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

#
#Making CL Layer
#
#$(BLD_4GMX_YS_OBJS):
#	@$(MAKE) -f ys.mak $(SY_DIR3)/lib4gmxys.a COPTS=$(C4GMXCOPTS) \
	LOPTS='$(C4GMXLOPTS)' POPTS='$(CC4GMXTFOPTS)' IOPTS='$(C4GMXIOPTS)' IN_DIR='$(YS_DIR)' \
	OUT_DIR='$(SY_DIR3)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

# making Multi Threaded SSI
#
#$(BLD_4GMX_SS_OBJS):
#	@$(MAKE) -f mtse.mak $(SY_DIR3)/libmt.a COPTS=$(C4GMXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CC4GMXMTOPTS)' IOPTS='$(C4GMXIOPTS)' IN_DIR='$(MTSE_DIR)' \
   OUT_DIR='$(SY_DIR3)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  \
   CM_INC='$(CM_INC1)'


clean:
	`del $(KW_DIR)/*.o $(RG_DIR)/*.o $(YS_DIR)/*.o $(CM_DIR)/*.o $(MTSE_DIR)/*.o`

# DO NOT DELETE THIS LINE -- make depend depends on it.

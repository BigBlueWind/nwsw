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
ifndef OCTEON_ROOT
OCTEON_ROOT=/usr/local/Cavium_Networks/1.9.0/OCTEON-SDK
endif
ifndef MATCHED_MODEL
MATCHED_MODEL=OCTEON_CN56XX
endif
ROOT=enodeb
ROOT1=enb_se
ROOT2=enb_seum
SY_DIR=./obj
SY_DIR1=./obj1
SY_DIR2=./obj2
SY_DIR3=$(SY_DIR)
SY_DIR4=./wr_obj_pal

ifeq (${BLDENV},4gmx_e2e_lib)
SY_LIB_DIR=.
else
SY_LIB_DIR=./obj
endif
STOPTS=-DENB 

OUTNAME=$(ROOT)
OUTNAME1=$(ROOT1)
OUTNAME2=$(ROOT2)
#ifndef DEBUG
DEBUG=1
#endif
ifeq (${BLDENV}, cav_seum_pico) 
PICO = pico_inc
else 
ifeq (${BLDENV}, cav_se_pico)
PICO = pico_inc
endif
endif
#-----------------------------------------------------------------------
# macros for td specific options
#

# MTSS defines
ifeq (${BLDENV},4gmx_e2e_lib)
MTOPTS=-DSS -DSS_MT -DCONRD -DNOFILESYS
MTOPTS2=-DSS -DSS_MT -DCONRD
else
MTOPTS=-DSS -DSS_MT -UCONRD -UNOFILESYS
MTOPTS1=-DSS -DSS_MT -UCONRD -DNOFILESYS -DSS_DBLK_FREE_RTN
MTOPTS2=-DSS -DSS_MT -DCONRD -DSS_DBLK_FREE_RTN
endif

#cavium includes
OBJ_DIR=./obj1
#LIB_DIR=./cavselibs
ifeq (${BLDENV}, lnx_e2e)
OBJ_DIR=./obj
LIB_DIR=./lnxlibs/32bit
#LIB_DIR=./lnxlibs/64bit
endif
ifeq (${BLDENV}, cav_seum_perf)
OBJ_DIR=./obj2
endif
ifeq (${BLDENV}, cav_seum_perf_uu)
OBJ_DIR=./obj2
endif
ifeq (${BLDENV}, cav_seum_e2e)
OBJ_DIR=./obj2
#LIB_DIR=./cavseumlibs/64bit
endif
ifeq (${BLDENV}, cav_seum_pico)
OBJ_DIR=./obj2
#LIB_DIR=./cavseumlibs/32bit
endif
CVMX_CONFIG_DIR=../cavsdk/config
CVMX_INC=-I$(OCTEON_ROOT)/target/include -I$(CVMX_CONFIG_DIR)
CFLAGS_GLOBAL += $(CVMX_INC)
CFLAGS_GLOBAL += $(OCTEON_CPPFLAGS_GLOBAL_ADD)

#cavium defines
#  application config check and rules

CVMX_CONFIG = ../cavsdk/config/cvmx-config.h
CVMX_OTHER_CONFIGS := ../cavsdk/config/*-config.h
CVMX_OTHER_CONFIGS := $(shell echo $(CVMX_OTHER_CONFIGS) | sed 's/config\/cvmx-config.h//')

#$(CVMX_CONFIG): $(CVMX_OTHER_CONFIGS)
#	cvmx-config $(CVMX_OTHER_CONFIGS)

#  special rule to re-compile if important environment variables change
MATCH=${findstring DUSE_RUNTIME_MODEL_CHECKS=1, ${OCTEON_CPPFLAGS_GLOBAL_ADD}}
ifeq (${MATCH}, DUSE_RUNTIME_MODEL_CHECKS=1)
#  We are using runtime model detection, so use "runtime" as model to avoid
#    a re-compile if only OCTEON_MODEL used for simulation changes
MADE_WITH_OCTEON_MODEL = "runtime"
else
MADE_WITH_OCTEON_MODEL = $(OCTEON_MODEL)
endif
#  set special filename for target and change any spaces in it to commas
MADE_WITH = $(shell echo "$(OBJ_DIR)/made_with-OCTEON_MODEL=$(MADE_WITH_OCTEON_MODEL)=-OCTEON_CPPFLAGS_GLOBAL_ADD=$(OCTEON_CPPFLAGS_GLOBAL_ADD)=." | sed 's/\ /,/g')

# Add profile-feedback flags.
ifdef FDO_PASS
ifeq ($(FDO_PASS), 1)
CFLAGS_GLOBAL += -fprofile-generate
LDFLAGS_GLOBAL += -fprofile-generate
else # 1
ifeq ($(FDO_PASS), 2)
CFLAGS_GLOBAL += -fprofile-use
LDFLAGS_GLOBAL += -fprofile-use
else # 2
$(error FDO_PASS should either be 1 or 2)
endif # 2
endif # 1
endif # FDO_PASS

ifdef OCTEON_DISABLE_BACKTRACE
CFLAGS_GLOBAL += -fno-asynchronous-unwind-tables -DOCTEON_DISABLE_BACKTRACE
endif

ifndef OCTEON_TARGET
ifeq (${BLDENV}, cav_se_perf)
  OCTEON_TARGET=cvmx_64
else
ifeq (${BLDENV},cav_se_e2e)
  OCTEON_TARGET=cvmx_64
else
ifeq (${BLDENV},cav_se_pico)
OCTEON_TARGET=cvmx_64
else
  OCTEON_TARGET=linux_64
endif
endif
endif
endif

SUPPORTED_TARGETS=linux_64 linux_n32 linux_uclibc linux_o32 cvmx_n32 cvmx_64
ifeq ($(findstring $(OCTEON_TARGET), $(SUPPORTED_TARGETS)),)
    ${error Invalid value for OCTEON_TARGET. Supported values: ${SUPPORTED_TARGETS}}
endif

ifeq (${OCTEON_TARGET},linux_64)
    PREFIX=-linux_64
    CFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=64 -march=octeon -msoft-float -Dmain=appmain
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=64 -march=octeon -msoft-float -Dmain=appmain
    LDFLAGS_GLOBAL += -mabi=64 -static -lrt -Xlinker -T -Xlinker $(OCTEON_ROOT)/target/lib/cvmx-shared-linux.ld
    #LDFLAGS_GLOBAL += -Xlinker $(OBJ_DIR)/libcvmx.a

else # linux_64
ifeq (${OCTEON_TARGET},linux_n32)
    PREFIX=-linux_n32
    CFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=n32 -march=octeon -msoft-float -Dmain=appmain
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=n32 -march=octeon -msoft-float -Dmain=appmain
    LDFLAGS_GLOBAL += -mabi=n32 -static -lrt -Xlinker -T -Xlinker $(OCTEON_ROOT)/target/lib/cvmx-shared-linux-n32.ld
else # linux_n32
ifeq (${OCTEON_TARGET},linux_uclibc)
    PREFIX=-linux_uclibc
    CFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -muclibc -march=octeon -msoft-float -Dmain=appmain
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -muclibc -march=octeon -msoft-float -Dmain=appmain
    LDFLAGS_GLOBAL += -muclibc -static -lrt -Xlinker -T -Xlinker $(OCTEON_ROOT)/target/lib/cvmx-shared-linux-n32.ld
else # linux_uclibc
ifeq (${OCTEON_TARGET},linux_o32)
    PREFIX=-linux_o32
    CFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=32 -march=octeon -msoft-float -Dmain=appmain
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=32 -march=octeon -msoft-float -Dmain=appmain
    LDFLAGS_GLOBAL += -mabi=32 -static -lrt -Xlinker -T -Xlinker $(OCTEON_ROOT)/target/lib/cvmx-shared-linux-o32.ld
else # linux_o32
ifeq (${OCTEON_TARGET},cvmx_n32)
    CFLAGS_GLOBAL += -DOCTEON_TARGET=cvmx_n32 -mabi=n32
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=cvmx_n32 -mabi=n32
    LDFLAGS_GLOBAL += -mabi=n32
    PREFIX=-cvmx_n32
else # cvmx_n32
ifeq (${OCTEON_TARGET},cvmx_64)
    CFLAGS_GLOBAL += -DOCTEON_TARGET=cvmx_64
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=cvmx_64
else # cvmx_64
    ${error Invalid value for OCTEON_TARGET. Supported values: ${SUPPORTED_TARGETS}}
endif # cvmx_64
endif # cvmx_n32
endif # linux_o32
endif # linux_uclibc
endif # linux_n32
endif # linux_64
ifeq (${BLDENV}, cav_seum_pico)
CFLAGS_GLOBAL += -DYS_PICO
endif
ifeq (${BLDENV}, cav_se_pico)
CFLAGS_GLOBAL += -DYS_PICO
endif

CFLAGS_GLOBAL += -DOCTEON_MODEL=$(MATCHED_MODEL)

CVMXOPTS += $(CFLAGS_GLOBAL)

# MOS defines
MSOPTS=-DMS
#Handover Support flag
LTE_HO_SUPPORT = -DLTE_HO_SUPPORT

#SSI Files
SSOPTS=-DSS

ifeq (${BLDENV}, 4gmx_e2e_lib)
CMFLAGS=-DSS_MT_TMR -DSS_FLOAT -ULTE_TDD -DMSPD -DMSPD_MEMCPY -DLTEPHY_MSPD#<---Insert cm specific defines here
SMFLAGS=-UHI -USB -UEG -UWR -DKW -DKW_PDCP -DPJ -USZ -DYS -DRG -DNH
MTFLAGS=-DBIT_64 -DSS_MT_TMR -DAPI_MAIN #<---Insert mt specific defines here
YSFLAGS=-DTF -DYS -DYS_LTE_PAL -DLTE_PAL_ENB -UYS_PERF_MEAS -DLCCTF -DYS_MSPD\
        -DLCYSUITFU -DLCTFU -DLCTFUITFU -DTFU_VER_2 \
        -UTFU_TDD -DRG -DLTE_PAL_ENB -DLCYSUICTF -DAPI_MAIN \
	-DLCYSMILYS -DLCLYS -UYS_MS_WAIT_FOR_TXCFM\
	-DYS_MS_PHY  -UYS_JMP_PHY_RUN_STE -DYS_MIB_WARND -DYS_PHY_STOP_AUTO -UYS_SIMUL_PUSCH_RECP -DYS_ALLOC_ON_NEED -UYS_RRC_CONN_SIMUL -UGEN_DUMMY_SR
KWFLAGS= -DKW_PDCP -UNH -USM -DPTPJLIB -DLTE_HENB 
RGFLAGS=-DTFU_VER_2 -UTFU_TDD -USM -ULTE_TDD -DTF -DSS_FLOAT -DSS_MT_TMR -DRGR_RRM_TICK
C4GMXENBE2EOPTS= -DMX_ARM_ENABLED -DLOWERARM -DSCHEDULER -DUL_CEVA -DUSECEVA\
                 -DUSEFEC -DLTE -DLIST_BASED -D_MX_DEBUG -D_APP_ASSERT_HANDLER\
		  $(YSFLAGS) $(CMFLAGS) $(SMFLAGS) $(MTFLAGS) $(KWFLAGS) \
                  $(RGFLAGS) -DSS_4GMX_SSI_PORTING -USSI_INCORRECT_FREE -USS_HISTOGRAM_SUPPORT -URUN_CL_ONLY -URUN_ICPU_ONLY $(MTOPTS) -USS_CAVIUM \
		  -DCMFILE_REORG_1 -DYS_IP_CFG_FRM_HDR_FILE -DNO_ERRCLS \
		  -DCMFIlE_REORG_2 -DSSINT2 -DCMKV2 -DSS_PERF -DSS_WL_REGION=1\
                  -DNOERRCHK -UCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU -UDEBUGP\
	          -ULTE_START_UL_DL_DATA -DLCYSUICTF -DCCPU_MLOG \
	          -UYS_ALL_PERF

endif
## enodeB compilation options details:
# LTE_ENB_PERF     : Enable for Uu performance setup without core network
#                    contact.
# VE_LNX_PCAP_PERF     : Enable LibPcap functionality at eNodeB, to be used 
#                    along with VE_PERF_MEAS flag. 
# YS_PAL_PERF_MEAS : Enable for measuring the performance [DL data rate] at PAL
#                    level.
# TFU_TDD, LTE_TDD : Enable to support TDD mode of operation for MAC & CL
#                    layers.
# VE_SUPPORT_RLC_UM_MODE: Enable to support RLC functioning in UM mode.
# LTE_LNX_AFFINITY: Enable to set a particular affinity [CPU/processor] for
#                   different system threads on Linux.
# SS_MULTICORE_SUPPORT SS_M_PROTO_REGION: Enable to support multicore 
#                   and multiregion feature from SSI on Linux platform. 


ifeq ($(BLDENV),lnx_perf_msstub)
PALOPTS=-ULTE_ENB_PAL -DYS_MSPD
else
ifeq ($(BLDENV),lnx_e2e_msstub)
PALOPTS=-ULTE_ENB_PAL -DYS_MSPD
else
ifeq ($(BLDENV),lnx_e2e)
PALOPTS=-DLTE_ENB_PAL
else
ifeq ($(BLDENV),lnx_perf)
PALOPTS=-DLTE_ENB_PAL
else
PALOPTS=-DLTE_ENB_PAL
endif # lnx_perf
endif # lnx_e2e
endif # lnx_e2e_msstub
endif # lnx_perf_msstub

CMENBOPTS= -DCMINET_BSDCOMPAT -DSS_TICKS_SEC=10 -DCMFILE_REORG_1 -DCM_INET2 \
           -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -DNO_ERRCLS -DNOERRCHK -DxSS_M_PROTO_REGION \
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU -DDEBUGP \
          -DLTE_ENB_PERF -UVE_PERF_MEAS -UCM_PASN_DBG -DREL_850 \
          -ULTE_START_UL_DL_DATA -UUSE_PURE

# EnodeB Performance Compile Options
CMENBPERFOPTS=-DCMINET_BSDCOMPAT -DSS_TICKS_SEC=10 -DCMFILE_REORG_1 -DCM_INET2 \
           -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -DNO_ERRCLS -DNOERRCHK -DFP_RELEASE6 \
          -DCCPU_DEBUG_TRACE -DDEBUGP \
          -DLTE_ENB_PERF -DVE_PERF_MEAS -UCM_PASN_DBG -DREL_850 -UVE_LNX_PCAP_PERF \
          -DYS_PAL_PERF_MEAS -UVE_SUPPORT_RLC_UM_MODE -UNOFILESYS \
          -ULTE_LNX_AFFINITY \
          -DVE_PERF_DL_DATA -DVE_PERF_UL_DATA -URGR_RRM_TICK $(PALOPTS) -DSS_FLOAT \
          -UUSE_PURE -DSLES9_PLUS -DRGR_V1 \
          -USS_MULTICORE_SUPPORT -USS_M_PROTO_REGION 

# EnodeB End to End Compile Options
CMENBE2EOPTS=-DCMINET_BSDCOMPAT -DSS_TICKS_SEC=10 -DCMFILE_REORG_1 -DCM_INET2 \
          -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED  -DYS_ENB_CFG\
          -DSS_PERF -DSS_WL_REGION=1 -DNO_ERRCLS -DNOERRCHK -DxSS_M_PROTO_REGION \
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU -DDEBUGP \
          -ULTE_ENB_PERF -UVE_PERF_MEAS -UCM_PASN_DBG -DREL_850 -UUE_RAD_CAP \
          -ULTE_START_UL_DL_DATA -DSS_FLOAT -DRGR_RRM_TICK $(PALOPTS) \
          -UUSE_PURE -DSLES9_PLUS -DTA_NEW -DSI_NEW -DRGR_SI_SCH -DVE_SRB2_SUPPORT \
          -UVE_SUPPORT_RLC_UM_MODE -UNOFILESYS -ULTE_LNX_AFFINITY -DRGR_V1 -ULTEMAC_MIMO \
          $(LTE_HO_SUPPORT) -DRS -DKW_BG_DL_PROC -DKW_BG_UL_PROC -DWR_SB_SCTP -DLTERRC_REL9 \
          -UCTF_VER3 -USSI_DEBUG_LEVEL1 -DMME_LDBAL -DSZ_ENB \
          -DEGTP_U_REL_9 -DLEGV2 -DYS_WIRESHARK_LOG -DMAC_SCH_STATS -DPHY_ERROR_LOGING -DSS_DIAG \
			 -DxLTE_L2_MEAS -DSZTV2 -DCZTV1 -URGR_CQI_REPT

CMENBPERFSEOPTS= -DCMINET_BSDCOMPAT -DSS_TICKS_SEC=10 -DCMFILE_REORG_1 -DCM_INET2 \
           -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -UNO_ERRCLS -UNOERRCHK -DSS_M_PROTO_REGION \
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU  -DDEBUGP \
          -DLTE_ENB_PERF -DREL_850 -DVE_PERF_DL_DATA -UVE_PERF_UL_DATA -DVE_PERF_MEAS \
          -DLTE_ENB_PAL -DENB_CPU_MEASUREMENT -DYS_PAL_PERF_MEAS -DRGR_V1

ifeq (${PICO}, pico_inc)
CMENBE2ESEOPTS= -DCMINET_BSDCOMPAT -DSS_TICKS_SEC=10 -DCMFILE_REORG_1 -DCM_INET2 \
           -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -UNO_ERRCLS -UNOERRCHK -DSS_M_PROTO_REGION -UVE_SRB2_SUPPORT \
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU  -UDEBUGP -DRGR_V1 \
          -ULTE_ENB_PERF -DREL_850 -ULTE_ENB_PAL -DSS_FLOAT -DRGR_RRM_TICK -UTA_NEW -DSI_NEW -DRGR_SI_SCH
else
CMENBE2ESEOPTS= -DCMINET_BSDCOMPAT -DSS_TICKS_SEC=10 -DCMFILE_REORG_1 -DCM_INET2 \
           -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -UNO_ERRCLS -UNOERRCHK -DSS_M_PROTO_REGION -DVE_SRB2_SUPPORT \
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU  -DDEBUGP -DRGR_V1 \
          -ULTE_ENB_PERF -DREL_850 -DLTE_ENB_PAL -DSS_FLOAT -DRGR_RRM_TICK -UTA_NEW -USI_NEW -URGR_SI_SCH
endif

CMENBPERFSEUMOPTS= -DCMINET_BSDCOMPAT -DSS_TICKS_SEC=10 -DCMFILE_REORG_1 -DCM_INET2 \
           -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -DNO_ERRCLS -DNOERRCHK -DSS_M_PROTO_REGION\
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU -DDEBUGP -DRGR_V1 \
          -DLTE_ENB_PERF -DVE_PERF_DL_DATA -UVE_PERF_UL_DATA -DVE_PERF_MEAS -DLTE_ENB_PAL

ifeq (${PICO}, pico_inc)
CMENBE2ESEUMOPTS= -DCMINET_BSDCOMPAT -DSS_TICKS_SEC=10 -DCMFILE_REORG_1 -DCM_INET2 \
           -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -DNO_ERRCLS -DNOERRCHK -DSS_M_PROTO_REGION -UVE_SRB2_SUPPORT \
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU -UDEBUGP -DRGR_V1 \
          -ULTE_ENB_PERF  -ULTE_ENB_PAL -DRGR_RRM_TICK -UTA_NEW -DSI_NEW -DRGR_SI_SCH -DUE_RAD_CAP
else
CMENBE2ESEUMOPTS= -DCMINET_BSDCOMPAT -DSS_TICKS_SEC=10 -DCMFILE_REORG_1 -DCM_INET2 \
           -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -DNO_ERRCLS -DNOERRCHK -DSS_M_PROTO_REGION -DVE_SRB2_SUPPORT \
          -DFP_RELEASE6 -DCCPU_DEBUG_TRACE -DCCPU_MEAS_CPU -DDEBUGP -DRGR_V1 \
          -ULTE_ENB_PERF  -DLTE_ENB_PAL -DRGR_RRM_TICK -UTA_NEW -USI_NEW -URGR_SI_SCH -DWR_SB_SCTP
endif

# The options below are used to pass on to the actual product compilations
# Just this one variable is passed for individual compilations and hence
# all the needed options are added from various other defined option macros

#ALL PAL layer options
TFUOPTS=-DTFU_VER_2 -UTFU_TDD -DLCTFU
TFUOPTS1=-DTFU_VER_2 -UTFU_TDD 
TFUOPTS2=-DTF -DLCRGLITFU -DLCTFU -DLCTFUITFU
LTFOPTS=-DSM -DLCTFMILTF -DLCLTF
TFOPTS=-DTF $(TFUOPTS) $(LTFOPTS) -DDL_STS -UTTI_PROC -URLC_STATUS_GEN \
        -UUL_PROC
TFOPTS1=-DTF $(TFUOPTS1) $(LTFOPTS) -DDL_STS -UTTI_PROC -URLC_STATUS_GEN \
        -UUL_PROC
TFOPTS2=-DTF $(TFUOPTS2) $(LTFOPTS) -DDL_STS -UTTI_PROC -URLC_STATUS_GEN \
        -UUL_PROC


# All LTE MAC product options
CRGOPTS= -DLCNHLICRG -DLCCRG -DLCRGUICRG
CRGOPTS1= -DLCNHLICRG -DLCCRG -DLCRGUICRG -DRG
RGUOPTS=-DRG -DKW -DLCKWLIRGU -DLCRGU -DLCRGUIRGU -DCCPU_OPT
LRGOPTS=-DLCSMMILRG -DLCLRG -DSM -DRG -DLCRGMILRG
RGROPTS=-DRG -DLCWRLIRGR -DLCRGR -DLWLCWRLIRGR -DLCRGUIRGR
RGRSEUMOPTS=-DLCWRLIRGR -DLCRGR -DLWLCWRLIRGR -DLCRGUIRGR
ifeq (${BLDENV},4gmx_e2e_lib)
RGOPTS=-DRG -DRG_PHASE2_SCHED $(RGUOPTS) $(CRGOPTS) $(LRGOPTS) $(RGROPTS) $(TFUOPTS) -URG_DEBUG -UTTI_PROC -UPROC_DL -USCH_TTI_PROC -UTOM -UPAL_MAC_PROC -UUL_PROC_MAC -DLCRGLITFU -DRG_DL_DELTA_CHANGE -DRG_CMNCH_SCHDELTA_CHANGE -DRG_ULSCHED_AT_CRC -DTFU_RECPREQ_DELTA_CHANGE -DTFU_DL_DELTA_CHANGE -DTFU_UL_DELTA_CHANGE -DTFU_DLHQFBK_DELTA_CHANGE -UBUG_FIX
else
RGOPTS=-DRG -DRG_PHASE2_SCHED $(RGUOPTS) $(CRGOPTS) $(LRGOPTS) $(RGROPTS) $(TFUOPTS) -URG_DEBUG -UTTI_PROC -UPROC_DL -USCH_TTI_PROC -UTOM -UPAL_MAC_PROC -UUL_PROC_MAC -DLCRGLITFU
endif

RGOPTS1=-DRG $(RGUOPTS) $(CRGOPTS1) $(LRGOPTS) $(RGROPTS) $(TFUOPTS) -URG_DEBUG -UTTI_PROC -UPROC_DL -USCH_TTI_PROC -UTOM -UPAL_MAC_PROC -UUL_PROC_MAC 

# All LTE EGTP-U product options
EGTOPTS=-DEG -DLCEGT -DLCEGUIEGT -DCCPU_OPT -DLCEULIEGT
LEGOPTS=-DLCSMMILEG -DLCLEG -DSM -DEG -DLCEGMILEG -DLCSMEGMILEG
EGOPTS=-DEG $(EGTOPTS) $(LEGOPTS) -UEG_DEBUG -DLCEGLIHIT 


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
PJUVEOPTS=-DPJ -DNH -DLCPJU  -DLCWRLIPJU -DLCPJUIPJU
PJUVESEUMOPTS=-DNH -DLCPJU  -DLCWRLIPJU -DLCPJUIPJU
ifeq (${BLDENV},4gmx_e2e_lib)
PJUVEOPTS2=-DPJ -DLCPJU -UWR -DPX -DNX -DNH -DLCWRLIPJU -DLCPJUIPJU
PJUNHOPTS=-DPJ  -DLCPJU  -DLCNHLIPJU -DLCPJUIPJU
PJUNHOPTS2=-DPJ -DLCPJU -UWR -DPX -DNX -DNH -DLCNHLIPJU -DLCPJUIPJU
else
PJUVEOPTS2=-DPJ -DLCPJU -DWR -DPX -DNX -DNH -DLCWRLIPJU -DLCPJUIPJU
PJUNHOPTS=-DPJ  -DLCPJU  -DLCNHLIPJU -DLCPJUIPJU
PJUNHOPTS2=-DPJ -DLCPJU -DWR -DPX -DNX -DNH -DLCNHLIPJU -DLCPJUIPJU
endif
CPJOPTS=-DLCCPJ -DNH -DKW -DLCNHLICPJ -DLCPJUICPJ
KWOPTS=-DKW $(CKWOPTS) $(KWUOPTS) $(CPJOPTS) $(LPJOPTS) $(LKWOPTS) $(RGUOPTS) $(PJUNHOPTS) $(PJUVEOPTS) -URLC_STATUS_GEN -UUL_PROC_MAC
KWOPTS1=-DKW $(CKWOPTS1) $(KWUOPTS) $(CPJOPTS) $(LPJOPTS) $(LKWOPTS) $(RGUOPTS) $(PJUNHOPTS) $(PJUVEOPTS) -URLC_STATUS_GEN -UUL_PROC_MAC
KWOPTS2=-DKW $(CKWOPTS2) $(KWUOPTS) $(CPJOPTS) $(LPJOPTS) $(LKWOPTS) $(RGUOPTS) $(PJUNHOPTS2) $(PJUVEOPTS2) -URLC_STATUS_GEN -UUL_PROC_MAC
#ALL RRC product options
LNHOPTS=-DNH -DSM -DLCLNH -DLCSMMILNH -DLCSMNHMILNH -DLCNHMILNH
NHUOPTS=-DLCNHU -DWR -DNX -DLCWRLINHU -DLCNHUINHU -DLCNXLINHU -DLWLCNHUINHU -DLWLCNXLINHU -DLCNHMILNH -DLCSMNHMILNH
NHOPTS=-DNH $(LNHOPTS) $(NHUOPTS) $(CRGOPTS) $(CKWOPTS) $(CPJOPTS) $(KWUOPTS) $(PJUNHOPTS)\
       -DRX -DRNC_INTEG_CCPU 
NHOPTS1=-DNH $(LNHOPTS) $(NHUOPTS) $(CRGOPTS1) $(CKWOPTS1) $(CPJOPTS) $(KWUOPTS) $(PJUNHOPTS)\
       -DRX -DRNC_INTEG_CCPU 

#ALL RRC application product options
LWROPTS=-DWR -DSM -ULCLWR -ULCSMMILWR -ULCWRMILWR
CTFOPTS=-DLCCTF -DWR -DLCWRLICTF -DLCTFUICTF -DLCNHLICTF
CTFOPTS1=-DLCCTF -DLCWRLICTF -DLCTFUICTF -DLCNHLICTF
WROPTS=-DWR -DLCCZT $(LWROPTS) $(NHUOPTS) $(CTFOPTS) $(RGROPTS) $(PJUVEOPTS) $(SZTOPTS) $(EGTOPTS) $(CZTOPTS)
WRSEUMOPTS=-DWR $(LWROPTS) $(NHUOPTS) $(CTFOPTS) $(RGRSEUMOPTS) $(PJUVESEUMOPTS) $(SZTOPTS) $(EGTOPTS) 
WROPTS1=-DWR $(LWROPTS) $(NHUOPTS) $(CTFOPTS1) $(RGROPTS) $(PJUVEOPTS) 

#All product options
SZTOPTS=-DSZ -DUZ -DLCUZLISZT -DLWLCUZLISZT -DLCSZUISZT -DLCSZT -DPTSZUISZT -US1AP_REL851 -DLSZV1 -DSZTV1 -DSZTV2
LSZOPTS=-DLCLSZ -DLCSCT -DSM -DSZ -DLCSMSZMILSZ  -DLCSZMILSZ
LSZOPTS2=-DLCLSZ -DSM -DSZ -DLCSMSZMILSZ  -DLCSZMILSZ 

TFOPTS=-DTF $(CTFOPTS) $(RGROPTS) -DLCTFU -DLCTFUITFU -DTFU_VER_2 -UTFU_TDD -DRG -DLTE_PAL_ENB
TFOPTS1=-DTF $(CTFOPTS1) $(RGROPTS)

#All X2AP options
CZTOPTS=-DCZ -DRS -DLCRSLICZT -DLWLCRSLICZT -DLCCZUICZT -DLCCZT -DPTCZUICZT
LCZOPTS=-DLCLCZ -DLCSCT -DSM -DCZ -DLCSMCZMILCZ  -DLCSZMILCZ
LCZOPTS2=-DLCLCZ -DSM -DCZ -DLCSMCZMILCZ  -DLCCZMILCZ

#All SCTP product options
LSBOPTS=-DLCLSB -DSM -DSB -DLSB4  -DLSB8 -DHI_REL_1_4
SCTOPTS=-DSB  -DLCSBUISCT -DLCSZLISCT -DLCCZLISCT
HITOPTS=-DLCSBLIHIT -DLCHIUIHIT -DHI_REL_1_4 -DLCEGLIHIT 
SBOPTS = $(LSBOPTS) $(HITOPTS) $(SCTOPTS) -DLCSBMILSB -DLCSMSBMILSB -DSB_CHECKSUM -DSB_CHECKSUM_CRC

SZOPTS=-DSZ $(SZTOPTS) $(LSZOPTS) $(SCTOPTS) -DLCSZMILSZ -DSM \
	  -DSZ_ENB -DSZ_DYNAMIC_PEER -DSZ_USTA
SZOPTS2=-DSZ $(SZTOPTS) $(LSZOPTS2) $(SCTOPTS) -DLCSZMILSZ -DSM \
	  -DSZ_ENB -DSZ_DYNAMIC_PEER -DSZ_USTA

CZOPTS=-DCZ $(CZTOPTS) $(LCZOPTS) $(SCTOPTS) -DLCCZMILCZ -DSM \
        -DCZ_ENB -DCZ_DYNAMIC_PEER -DCZ_USTA
CZOPTS2=-DCZ $(CZTOPTS) $(LCZOPTS) $(SCTOPTS) -DLCCZMILCZ -DSM \
        -DCZ_ENB -DCZ_DYNAMIC_PEER -DCZ_USTA

#All TUCL product options
LHIOPTS=-DLCHIMILHI -DSM -DLCLHI
HIOPTS= $(LHIOPTS) $(HITOPTS) -DHI -DLCHIMILHI -DSM -DLCSMHIMILHI -DEG

#ALL RRC product options
LRXOPTS=-DRX -DSM -DLCLRX

SMOPTS= -DLCSMMILCL -DLCCLMILCL

#-----------------------------------------------------------------------
# macros for sw and include directories
#
ifeq (${BLDENV},4gmx_e2e_lib)
CODEPATH=../../
else
CODEPATH=../../
endif
ifeq (${BLDENV},4gmx_e2e_lib)
MTSE_DIR=$(CODEPATH)mt4gmx
else
MTSE_DIR=$(CODEPATH)/mtse
endif
MTSEUM_DIR=$(CODEPATH)/mt
CM_DIR=$(CODEPATH)/cm
ifeq (${BLDENV},4gmx_e2e_lib)
WR_DIR=$(CODEPATH)/enbapp/src
else
WR_DIR=$(CODEPATH)/enbapp/src
endif
ifeq (${BLDENV},4gmx_e2e_lib)
YS_DIR=$(CODEPATH)ltecl
#MS_DIR=$(CODEPATH)/ltephy/mindSpeed/inc/
MS_CM_DIR=$(CODEPATH)/Common/
MS_PHYLTE_DIR=$(CODEPATH)ltephy/mindSpeed/uppercore/LTE/C-Code/
MS_TP_DIR=$(CODEPATH)ltephy/mindSpeed/uppercore/TurnerProject/
#CM_CMINC=../../common/include
L_CORE_DIR=$(CODEPATH)ltephy/mindSpeed/lowercore/
BLD_4GMX_CM_OBJS=$(SY_DIR3)/lib4gmxcm.a
BLD_4GMX_SS_OBJS=$(SY_DIR3)/lib4gmxmt.a
BLD_4GMX_KW_OBJS=$(SY_DIR3)/lib4gmxkw.a
BLD_4GMX_RG_OBJS=$(SY_DIR3)/lib4gmxrg.a
BLD_4GMX_YS_OBJS=$(SY_DIR3)/lib4gmxys.a
endif

RM_DIR=$(CODEPATH)/enbapp/lterrm
TF_DIR=$(CODEPATH)/ltepal
YS_PAL_DIR=$(CODEPATH)/lteclpal
YS_PC_DIR=$(CODEPATH)/lteclpc
YS_MS_DIR=$(CODEPATH)/lteclms
NH_DIR=$(CODEPATH)/lterrc
SZ_DIR=$(CODEPATH)/s1ap
CZ_DIR=$(CODEPATH)/x2ap
SB_DIR=$(CODEPATH)/sctp
HI_DIR=$(CODEPATH)/tucl
EG_DIR=$(CODEPATH)/egtpu
RG_DIR=$(CODEPATH)/ltemac
KW_DIR=$(CODEPATH)/lterlcpdcp
ifeq (${PICO}, pico_inc)
CM_INC_DIR=../ltephy/picochip/common/include
CM_CM_INC_DIR=../../common/include
CL2_DIR=../ltephy/picochip/ltecl2/cl2

PA_DIR=../ltephy/picochip/ltecl2/pa_driver
TST_DIR=../../ltephy/picochip/test
DBG_DIR=../ltephy/picochip/common/debug
SECM_DIR=../ltephy/picochip/common


CM_LIB_DIR=.
WR_LIB_DIR=.
else
CL2_DIR=$(CODEPATH)/cl2
CM_CMINC=../../common/include

CM_LIB_DIR=$(CODEPATH)/cm/
WR_LIB_DIR=$(CODEPATH)/enbapp/
endif
BLD_LNX_RM_OBJS=$(SY_DIR)/liblnxrm.a
BLD_LNX_CM_OBJS=$(SY_DIR)/liblnxcm.a
BLD_LNX_CM_PERF_OBJS=$(SY_DIR)/liblnxcmuu.a
BLD_LNX_SS_OBJS=$(SY_DIR)/liblnxmt.a
BLD_LNX_KW_OBJS=$(SY_DIR)/liblnxkw.a
BLD_LNX_RG_OBJS=$(SY_DIR)/liblnxrg.a
BLD_LNX_YS_OBJS=$(SY_DIR)/liblnxys.a
BLD_LNX_YS_MS_OBJS=$(SY_DIR)/liblnxysms.a
BLD_LNX_WR_PERF_OBJS=$(SY_DIR)/liblnxperfwr.a
BLD_LNX_WR_UU_PERF_OBJS=$(SY_DIR)/liblnxperfwruu.a
BLD_LNX_WR_E2E_OBJS=$(SY_DIR)/liblnxe2ewr.a
#for pc
BLD_CAV_WR_PICO_OBJS2=$(SY_DIR2)/libcave2ewr2.a
BLD_LNX_YS_PICO_OBJS=$(SY_DIR)/liblnxpcys.a
BLD_CAV_YS_PICO_OBJS=$(SY_DIR1)/libcavpcys.a
BLD_CAV_CL_OBJS=$(SY_DIR1)/libcl.a
BLD_CAV_PA_OBJS=$(SY_DIR1)/libpadriver.a
BLD_CAV_TST_OBJS=$(SY_DIR1)/libtst.a
BLD_CAV_SE_CM_OBJS=$(SY_DIR1)/libsecom.a
BLD_LNX_NH_OBJS=$(SY_DIR)/liblnxnh.a
BLD_LNX_SZ_OBJS=$(SY_DIR)/liblnxsz.a
ifeq ($(LTE_HO_SUPPORT),-DLTE_HO_SUPPORT)
BLD_LNX_CZ_OBJS=$(SY_DIR)/liblnxcz.a
endif
BLD_LNX_SB_OBJS=$(SY_DIR)/liblnxsb.a
BLD_LNX_HI_OBJS=$(SY_DIR)/liblnxhi.a
BLD_LNX_EG_OBJS=$(SY_DIR)/liblnxeg.a
BLD_LNX_YS_OBJS1=$(SY_DIR)/liblnxys.a
BLD_LNX_KW_OBJS1=$(SY_DIR)/libkw.a
BLD_LNX_RG_OBJS1=$(SY_DIR)/librg.a
BLD_LNX_NH_OBJS1=$(SY_DIR)/libnh.a
BLD_LNX_SZ_OBJS1=$(SY_DIR)/libsz.a
ifeq ($(LTE_HO_SUPPORT),-DLTE_HO_SUPPORT)
BLD_LNX_CZ_OBJS1=$(SY_DIR)/libcz.a
endif
BLD_LNX_SB_OBJS1=$(SY_DIR)/libsb.a
BLD_LNX_HI_OBJS1=$(SY_DIR)/libhi.a
BLD_LNX_WR_E2E_OBJS1=$(SY_DIR)/libe2elnxwr.a
BLD_LNX_EG_OBJS1=$(SY_DIR)/libeg.a
BLD_LNX_RM_OBJS1=$(SY_DIR)/librm.a
BLD_LNX_YS_PREPROC=$(SY_DIR)/ysperpro
BLD_LNX_KW_PREPROC=$(SY_DIR)/kwperpro
BLD_LNX_RG_PREPROC=$(SY_DIR)/rgperpro
BLD_LNX_NH_PREPROC=$(SY_DIR)/nhperpro
BLD_LNX_SZ_PREPROC=$(SY_DIR)/szperpro
BLD_LNX_CZ_PREPROC=$(SY_DIR)/czperpro
BLD_LNX_SB_PREPROC=$(SY_DIR)/sbperpro
BLD_LNX_HI_PREPROC=$(SY_DIR)/hiperpro
BLD_LNX_WR_E2E_PREPROC=$(SY_DIR)/wrperpro
BLD_LNX_EG_PREPROC=$(SY_DIR)/egperpro
BLD_LNX_RM_PREPROC=$(SY_DIR)/rmperpro
BLD_CAV_CM_OBJS=$(SY_DIR1)/libcavcm.a
BLD_CAV_SS_OBJS=$(SY_DIR1)/libcavmt.a
BLD_CAV_KW_OBJS=$(SY_DIR1)/libcavkw.a
BLD_CAV_RG_OBJS=$(SY_DIR1)/libcavrg.a
BLD_CAV_YS_OBJS=$(SY_DIR1)/libcavys.a
BLD_CAV_WR_OBJS=$(SY_DIR1)/libcavwr.a
BLD_CAV_NH_OBJS=$(SY_DIR1)/libcavnh.a
BLD_CAV_SZ_OBJS=$(SY_DIR1)/libcavsz.a
ifeq ($(LTE_HO_SUPPORT),-DLTE_HO_SUPPORT)
BLD_CAV_CZ_OBJS=$(SY_DIR1)/libcavcz.a
endif
BLD_CAV_SB_OBJS=$(SY_DIR1)/libcavsb.a
BLD_CAV_HI_OBJS=$(SY_DIR1)/libcavhi.a
BLD_CAV_EG_OBJS=$(SY_DIR1)/libcaveg.a

BLD_CAV_CM_OBJS2=$(SY_DIR2)/libcavcm2.a
BLD_CAV_CM_OBJS2_PERF=$(SY_DIR2)/libcavcm2uu.a
BLD_CAV_SS_OBJS2=$(SY_DIR2)/libcavmt2.a
BLD_CAV_WR_PERF_OBJS2=$(SY_DIR2)/libcavperfwr2.a
BLD_CAV_WR_PERF_OBJS2_UU=$(SY_DIR2)/libcavperfwr2uu.a
BLD_CAV_WR_E2E_OBJS2=$(SY_DIR2)/libcave2ewr2.a
BLD_CAV_NH_OBJS2=$(SY_DIR2)/libcavnh2.a
BLD_CAV_SZ_OBJS2=$(SY_DIR2)/libcavsz2.a
ifeq ($(LTE_HO_SUPPORT),-DLTE_HO_SUPPORT)
BLD_CAV_CZ_OBJS2=$(SY_DIR2)/libcavcz2.a
endif
BLD_CAV_SB_OBJS2=$(SY_DIR2)/libcavsb2.a
BLD_CAV_HI_OBJS2=$(SY_DIR2)/libcavhi2.a
BLD_CAV_EG_OBJS2=$(SY_DIR2)/libcaveg2.a
# make the list for .x and .h dependent

# common include files(no makedepend)
CM_INC=\
	$(CM_DIR)/envdep.h         $(CM_DIR)/envind.h         \
	$(CM_DIR)/envopt.h                                    \
	$(CM_DIR)/gen.h            $(CM_DIR)/gen.x            \
	$(CM_DIR)/cm_hash.x        $(CM_DIR)/cm_hash.h        \
	$(CM_DIR)/cm_lte.x        $(CM_DIR)/cm_lte.h        \
	$(CM_DIR)/cm5.h            $(CM_DIR)/cm5.x            \
	$(CM_DIR)/cm_umts.h        $(CM_DIR)/cm_umts.x        \
	$(CM_DIR)/cm_err.h         $(CM_DIR)/cm_lib.x         \
	$(CM_DIR)/cm_mblk.h        $(CM_DIR)/cm_mblk.x        \
	$(CM_DIR)/cm_tkns.h        $(CM_DIR)/cm_tkns.x        \
	$(CM_DIR)/cm_llist.h       $(CM_DIR)/cm_llist.x       \
	$(CM_DIR)/cm_hash.h        $(CM_DIR)/cm_hash.x        \
	$(CM_DIR)/cm_inet.h        $(CM_DIR)/cm_inet.x        \
	$(CM_DIR)/cm_gen.h         $(CM_DIR)/cm_gen.x         \
	$(CM_DIR)/cm_tpt.h         $(CM_DIR)/cm_tpt.x         \
	$(CM_DIR)/cm_dns.h         $(CM_DIR)/cm_dns.x         \
	$(CM_DIR)/ssi.h            $(CM_DIR)/ssi.x            \
	$(CM_DIR)/nhu.h            $(CM_DIR)/nhu.x            \
	$(CM_DIR)/crg.h            $(CM_DIR)/crg.x            \
	$(CM_DIR)/kwu.h            $(CM_DIR)/kwu.x            \
	$(CM_DIR)/tfu.h            $(CM_DIR)/tfu.x            \
	$(CM_DIR)/rgr.h            $(CM_DIR)/rgr.x            \
	$(CM_DIR)/pju.h            $(CM_DIR)/pju.x            \
	$(CM_DIR)/cm_pint.h        $(CM_DIR)/cm_pint.x        \
	$(CM_DIR)/cm_perr.h                                   \
        $(CM_DIR)/nhu_pk.x         $(CM_DIR)/nhu_unpk.x       \
        $(CM_DIR)/nhu_asn.h         $(CM_DIR)/nhu_asn.x       \
        $(CM_DIR)/lkw.h            $(CM_DIR)/lkw.x            \
        $(CM_DIR)/lnh.h            $(CM_DIR)/lnh.x            \
        $(CM_DIR)/lys.h            $(CM_DIR)/lys.x            \
        $(CM_DIR)/lrg.h            $(CM_DIR)/lrg.x            \
        $(CM_DIR)/lpj.h            $(CM_DIR)/lpj.x            \
	$(CM_DIR)/cm_pasn.x                                   \
	$(CM_DIR)/cm_mem.h         $(CM_DIR)/cm_mem.x         \

CM_INC1=\
        $(CM_DIR)/envdep.h         $(CM_DIR)/envind.h         \
        $(CM_DIR)/envopt.h                                    \
        $(CM_DIR)/gen.h            $(CM_DIR)/gen.x            \
        $(CM_DIR)/cm_hash.x        $(CM_DIR)/cm_hash.h        \
        $(CM_DIR)/cm_lte.x        $(CM_DIR)/cm_lte.h        \
        $(CM_DIR)/cm5.h            $(CM_DIR)/cm5.x            \
        $(CM_DIR)/cm_umts.h        $(CM_DIR)/cm_umts.x        \
        $(CM_DIR)/cm_err.h         $(CM_DIR)/cm_lib.x         \
        $(CM_DIR)/cm_mblk.h        $(CM_DIR)/cm_mblk.x        \
        $(CM_DIR)/cm_tkns.h        $(CM_DIR)/cm_tkns.x        \
        $(CM_DIR)/cm_llist.h       $(CM_DIR)/cm_llist.x       \
        $(CM_DIR)/cm_hash.h        $(CM_DIR)/cm_hash.x        \
        $(CM_DIR)/cm_inet.h        $(CM_DIR)/cm_inet.x        \
        $(CM_DIR)/cm_gen.h         $(CM_DIR)/cm_gen.x         \
        $(CM_DIR)/cm_tpt.h         $(CM_DIR)/cm_tpt.x         \
        $(CM_DIR)/cm_dns.h         $(CM_DIR)/cm_dns.x         \
        $(CM_DIR)/ssi.h            $(CM_DIR)/ssi.x            \
        $(CM_DIR)/nhu.h            $(CM_DIR)/nhu.x            \
        $(CM_DIR)/crg.h            $(CM_DIR)/crg.x            \
        $(CM_DIR)/kwu.h            $(CM_DIR)/kwu.x            \
        $(CM_DIR)/tfu.h            $(CM_DIR)/tfu.x            \
        $(CM_DIR)/rgr.h            $(CM_DIR)/rgr.x            \
        $(CM_DIR)/pju.h            $(CM_DIR)/pju.x            \
        $(CM_DIR)/cm_pint.h        $(CM_DIR)/cm_pint.x        \
        $(CM_DIR)/cm_perr.h                                   \
        $(CM_DIR)/lkw.h            $(CM_DIR)/lkw.x            \
        $(CM_DIR)/lrg.h            $(CM_DIR)/lrg.x            \
        $(CM_DIR)/lpj.h            $(CM_DIR)/lpj.x            \
        $(CM_DIR)/cm_pasn.h        $(CM_DIR)/cm_pasn.x        \
        $(CM_DIR)/cm_mem.h         $(CM_DIR)/cm_mem.x         \

# Addign this for the Lib Rule 
CM_INC2=\
	$(CM_LIB_DIR)/envdep.h         $(CM_LIB_DIR)/envind.h         \
	$(CM_LIB_DIR)/envopt.h                                    \
	$(CM_LIB_DIR)/gen.h            $(CM_LIB_DIR)/gen.x            \
	$(CM_LIB_DIR)/cm_hash.x        $(CM_LIB_DIR)/cm_hash.h        \
	$(CM_LIB_DIR)/cm_lte.x        $(CM_LIB_DIR)/cm_lte.h        \
	$(CM_LIB_DIR)/cm5.h            $(CM_LIB_DIR)/cm5.x            \
	$(CM_LIB_DIR)/cm_err.h         $(CM_LIB_DIR)/cm_lib.x         \
	$(CM_LIB_DIR)/cm_mblk.h        $(CM_LIB_DIR)/cm_mblk.x        \
	$(CM_LIB_DIR)/cm_tkns.h        $(CM_LIB_DIR)/cm_tkns.x        \
	$(CM_LIB_DIR)/cm_llist.h       $(CM_LIB_DIR)/cm_llist.x       \
	$(CM_LIB_DIR)/cm_hash.h        $(CM_LIB_DIR)/cm_hash.x        \
	$(CM_LIB_DIR)/cm_inet.h        $(CM_LIB_DIR)/cm_inet.x        \
	$(CM_LIB_DIR)/cm_tpt.h         $(CM_LIB_DIR)/cm_tpt.x         \
	$(CM_LIB_DIR)/ssi.h            $(CM_LIB_DIR)/ssi.x            \
	$(CM_LIB_DIR)/nhu.h            $(CM_LIB_DIR)/nhu.x            \
	$(CM_LIB_DIR)/egt.h            $(CM_LIB_DIR)/egt.x            \
	$(CM_LIB_DIR)/szt.h            $(CM_LIB_DIR)/szt.x            \
	$(CM_LIB_DIR)/rgr.h            $(CM_LIB_DIR)/rgr.x            \
	$(CM_LIB_DIR)/cm_perr.h                                   \
        $(CM_LIB_DIR)/nhu_pk.x         $(CM_LIB_DIR)/nhu_unpk.x       \
        $(CM_LIB_DIR)/nhu_asn.h         $(CM_LIB_DIR)/nhu_asn.x       \
        $(CM_LIB_DIR)/szt_pk.x         $(CM_LIB_DIR)/szt_unpk.x       \
        $(CM_LIB_DIR)/szt_asn.h         $(CM_LIB_DIR)/szt_asn.x       \
        $(CM_LIB_DIR)/leg.h            $(CM_LIB_DIR)/leg.x            \
        $(CM_LIB_DIR)/lhi.h            $(CM_LIB_DIR)/lhi.x            \
        $(CM_LIB_DIR)/lkw.h            $(CM_LIB_DIR)/lkw.x            \
        $(CM_LIB_DIR)/lnh.h            $(CM_LIB_DIR)/lnh.x            \
        $(CM_LIB_DIR)/lys.h            $(CM_LIB_DIR)/lys.x            \
        $(CM_LIB_DIR)/lrg.h            $(CM_LIB_DIR)/lrg.x            \
        $(CM_LIB_DIR)/lsb.h            $(CM_LIB_DIR)/lsb.x            \
        $(CM_LIB_DIR)/lsz.h            $(CM_LIB_DIR)/lsz.x            \
        $(CM_LIB_DIR)/lpj.h            $(CM_LIB_DIR)/lpj.x            \
	$(CM_LIB_DIR)/cm_pasn.x                                   \
	$(CM_LIB_DIR)/cm_mem.h         $(CM_LIB_DIR)/cm_mem.x         \

#-----------------------------------------------------------------------
# macros for compile and link options

LNXENV=-DSUNOS -DSS -DSS_MT -DANSI -D_GNU_SOURCE -DSS_LINUX -D_REENTRANT -D__EXTENSIONS__ \
    -DDEBUGNOEXIT
CAVENV=-DSUNOS -DSS -DSS_MT -DANSI -D_GNU_SOURCE -DSS_LINUX -D_REENTRANT -D__EXTENSIONS__ \
    -DDEBUGNOEXIT -DALIGN_64BIT -DBIT_64  $(CVMXOPTS) -DDEBUG_LEVEL=$(DEBUG)
C4GMXENV=-DSUNOS -DSS -DSS_MT -DANSI -D_GNU_SOURCE -DSS_LINUX -D_REENTRANT -D__EXTENSIONS__ \
    -DDEBUGNOEXIT

# compiler:
QUANTIFY=/usr/ccpu/rational/releases/quantify.i386_linux2.7.0/quantify

ifeq (linux,$(findstring linux,$(OCTEON_TARGET)))
    LNXCC = mips64-octeon-linux-gnu-gcc
    AR = mips64-octeon-linux-gnu-ar
    LNXLD = mips64-octeon-linux-gnu-ld
    STRIP = mips64-octeon-linux-gnu-strip
    OBJDUMP = mips64-octeon-linux-gnu-objdump
    NM = mips64-octeon-linux-gnu-nm
else
    LNXCC = mipsisa64-octeon-elf-gcc
    AR = mipsisa64-octeon-elf-ar
    LNXLD = mipsisa64-octeon-elf-ld
    STRIP = mipsisa64-octeon-elf-strip
    OBJDUMP = mipsisa64-octeon-elf-objdump
    NM = mipsisa64-octeon-elf-nm
endif

#LNX_CC="/usr/ccpu/rational/releases/quantify.i386_linux2.7.0.0.0-008/quantify gcc -m32"
LNX_CC="gcc -m32"
LNX_LD="gcc -m32"
LNX_AR="ar"

# Enable for 64bit compilation
#LNX_CC="gcc -m64 -DALIGN_64BIT -DBIT_64"
#LNX_LD="gcc -m64"

ARCH = cortex-a9
ARM_CC="arm-none-linux-gnueabi-gcc -mcpu=$(ARCH) "
ARM_LD="arm-none-linux-gnueabi-gcc -mcpu=$(ARCH) "

#LNX_CC="purify -always-use-cache-dir --cache-dir=/tmp gcc -m32"
#LNX_LD="purify -always-use-cache-dir --cache-dir=/tmp gcc -m32"

ifeq (${BLDENV},4gmx_e2e_lib)
LNXCC = armcc
ARMAR = armar
LNXLD = armlink
LNXAS = armasm
CC=LNXCC
endif

CAVCOPTS=" -O3 -fno-builtin-memcpy -pipe -Wall -Wno-comment -Wshadow -Wcast-qual \
-Wstrict-prototypes -Wmissing-prototypes -fno-strict-aliasing"

LNXCOPTS=" -g3 -pipe -pedantic -Wall -Wno-comment -Wshadow -Wcast-qual \
-Wstrict-prototypes -Wmissing-prototypes -fno-strict-aliasing -fsigned-char --std=c99"

C4GMXCOPTS=" -O3 -pipe -Wall -Wno-comment -Wshadow -Wcast-qual \
-Wstrict-prototypes -Wmissing-prototypes -fno-strict-aliasing"

# FOR gprof
#LNXCOPTS=" -g3 -pipe -pedantic -Wall -Wno-comment -Wshadow -Wcast-qual \
#-Wstrict-prototypes -Wmissing-prototypes -fno-strict-aliasing -fprofile-arcs -pg"

# object files:
# o     UNIX
OBJ=o                                    # object files, sunos

#######################################################################
# library path
CAVLIBPATH= -L$(OCTEON_ROOT)/target/lib -L$(OBJ_DIR)
ifeq (${BLDENV},4gmx_e2e_lib)
#LNXLIBPATH= -L$(MS_PATH)/OSAL/LINUX
else
LNXLIBPATH= -Llib/x86
endif
ARMLIBPATH= -Llib/arm

ifeq (${PLD}, CCPU)
LNXLIBPATH += -L$(PLD_PATH)
endif

#C4GMXLIB1= \
	  $(L_CORE_DIR)/lower-core/lib/4GMXLIB_D.a \
	  $(L_CORE_DIR)/lower-core/lib/SYSCORE_D.a \
          $(L_CORE_DIR)/lower-core/lib/liblte.a
#######################################################################

# linker options:
CAVSELOPTS=$(LDFLAGS_GLOBAL) -Xlinker -lm 
CAVSEUMLOPTS=$(LDFLAGS_GLOBAL) -Xlinker -lpthread -lc -lnss_files -lnss_dns -lresolv -lm
LNXLOPTS= -lpthread -lnsl -lrt -lm -export-dynamic
ifeq (${PLD}, CCPU)
	LNXLOPTS += -lccpupld 
endif

ifeq ($(BLDENV),lnx_perf_msstub)
	LNXLOPTS += -losal
else
ifeq ($(BLDENV),lnx_e2e_msstub)
	LNXLOPTS += -losal
else
ifeq ($(BLDENV),lnx_e2e_ms)
	LNXLOPTS += -losal
else
ifeq ($(BLDENV),arm_perf_msstub)
	LNXLOPTS += -losal
else
ifeq ($(BLDENV),arm_e2e_msstub)
	LNXLOPTS += -losal
endif
endif
endif
endif
endif

# for gprof
#LNXLOPTS=-lpthread -lnsl -lrt -lm -losal -lsctp -lgcov -pg
#LOPTS=$(LDFLAGS_GLOBAL) -lpthread -lnsl -lc -lnss_files -lnss_dns -lresolv

# include options:
LNXIOPTS  =  -I$(MTSEUM_DIR) -I$(CM_DIR) -I$(EG_DIR) -I$(SZ_DIR) -I$(KW_DIR) -I$(CZ_DIR) -I$(WR_DIR) -I$(RM_DIR)
LNXLIBIOPTS  =  -I$(CM_LIB_DIR)
ifeq (${PICO}, pico_inc)
CAVIOPTS  = -I$(CM_DIR) -I$(MTSE_DIR) -I$(EG_DIR) -I$(SZ_DIR) -I$(KW_DIR) $(CVMX_INC) -I$(TF_DIR)  \
			-I$(YS_PC_DIR) -I$(CL2_DIR) -I$(PA_DIR) -I$(CM_INC_DIR) -I$(CM_CM_INC_DIR) -I$(DBG_DIR) -I$(CZ_DIR)
CAVIOPTS2  =  -I$(MTSEUM_DIR) -I$(CM_DIR) -I$(EG_DIR) -I$(SZ_DIR) $(CVMX_INC) -I$(CM_INC_DIR) 
else
CAVIOPTS  =  -I$(CM_DIR) -I$(MTSE_DIR) -I$(EG_DIR) -I$(SZ_DIR) -I$(KW_DIR) $(CVMX_INC) -I$(TF_DIR) -I$(CZ_DIR)
CAVIOPTS2  =  -I$(MTSEUM_DIR) -I$(CM_DIR) -I$(EG_DIR) -I$(SZ_DIR) $(CVMX_INC) -I$(CM_CMINC) -I$(CZ_DIR)
endif	


#ifeq (${BLDENV}, cav_se_e2e_n32)
#COMPILE = $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

#ASSEMBLE = $(CC) $(ASFLAGS_GLOBAL) $(ASFLAGS_LOCAL) -MD -c -o $@ $<

#CAVENV=-DSUNOS -DSS -DSS_MT -DANSI -D_GNU_SOURCE -DSS_LINUX -D_REENTRANT -D__EXTENSIONS__ \
    -DDEBUGNOEXIT -UALIGN_64BIT -UBIT_64  $(CVMXOPTS) 

#dir := $(OCTEON_ROOT)/executive
#include $(dir)/cvmx.mk
#endif

#ifeq (${BLDENV}, cav_seum_e2e_n32)
#COMPILE = $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

#ASSEMBLE = $(CC) $(ASFLAGS_GLOBAL) $(ASFLAGS_LOCAL) -MD -c -o $@ $<

#CAVENV=-DSUNOS -DSS -DSS_MT -DANSI -D_GNU_SOURCE -DSS_LINUX -D_REENTRANT -D__EXTENSIONS__ \
    -DDEBUGNOEXIT -UALIGN_64BIT -UBIT_64  $(CVMXOPTS) 

#dir := $(OCTEON_ROOT)/executive
#include $(dir)/cvmx.mk
#endif

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
             -I$(L_CORE_DIR)/T4KDev/SYSCORE/SERVICES/CEVA/DRIVERS \
             -I$(L_CORE_DIR)/T4KDev/SYSCORE/SERVICES/CEVA/RESXSERV \
             -I$(L_CORE_DIR)/T4KDev/SYSCORE/SERVICES \
             -I$(L_CORE_DIR)/T4KDev/4GMX/arm/inc \
             -I$(L_CORE_DIR)/T4KDev/SYSCORE/DRIVERs/ \
             -I$(L_CORE_DIR)/T4KDev/SYSCORE/DRIVERs/fec \
             -I$(L_CORE_DIR)/T4KDev/SYSCORE/DRIVERs/map \
				 -I$(L_CORE_DIR)/T4KDev/SYSCORE/DRIVERs/cpri \
				 -I$(L_CORE_DIR)/PHY/LTE/Arm/Prj/LtePhyRt/ \
				 -I$(L_CORE_DIR)/PHY/LTE/C-Code/ \
             -I$(KW_DIR) -I$(MTSE_DIR) -I$(RM_DIR) -I$(RG_DIR) -I$(YS_DIR) -I$(CM_DIR)

#             -I$(L_CORE_DIR)/lower-core/app/\



# compiler options:

#RRM defines 
CCLNXRMOPTS= $(LNXENV) $(RMOPTS)  $(STOPTS) $(CMOPTS)

# MTSS defines
CCLNXMTOPTS=$(DEBUGPOPTS) $(LNXENV) $(MTOPTS) $(CMOPTS) $(STOPTS) -DOP_SSI_SAM
CCCAVSEMTOPTS=$(DEBUGPOPTS) $(CAVENV) $(MTOPTS1) $(CMOPTS) $(STOPTS) -DOP_SSI_SAM -DSS_CAVIUM -DAPI_MAIN
CCCAVSEUMMTOPTS=$(DEBUGPOPTS) $(CAVENV) $(MTOPTS2) $(CMOPTS) $(STOPTS) -DOP_SSI_SAM -DSS_SEUM_CAVIUM
CC4GMXMTOPTS=$(DEBUGPOPTS) $(C4GMXENV) $(MTOPTS) $(CMOPTS) $(STOPTS) -DOP_SSI_SAM

# SM defines 
CCLNXSMOPTS= $(LNXENV) $(SMOPTS) $(CMOPTS) $(STOPTS)
CCCAVSESMOPTS= $(CAVENV) $(SMOPTS) $(CMOPTS) $(STOPTS) -DSS_CAVIUM
CCCAVSEUMSMOPTS= $(CAVENV) $(SMOPTS) $(CMOPTS) $(STOPTS)

# NH defines 
CCLNXNHOPTS=  $(LNXENV) $(NHOPTS) $(CMOPTS) $(STNHOPTS)
CCCAVSENHOPTS=  $(CAVENV) $(NHOPTS) $(CMOPTS) $(STNHOPTS) -DSS_CAVIUM
CCCAVSEUMNHOPTS=  $(CAVENV) $(NHOPTS) $(CMOPTS) $(STNHOPTS)

#CAV_SUPPORT needs to be enabled for the TB size restriction for picochip PHY 0.3
#KW defines
CCLNXKWOPTS= $(LNXENV) $(KWOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSEKWOPTS= $(CAVENV) $(KWOPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM -UCAV_SUPPORT 
CCCAVSEUMKWOPTS= $(CAVENV) $(KWOPTS)  $(STOPTS) $(CMOPTS)
CC4GMXKWOPTS= $(C4GMXENV) $(KWOPTS)  $(STOPTS) $(CMOPTS) -DSS_4GMX_SSI_PORTING

#RG defines 
CCLNXRGOPTS= $(LNXENV) $(RGOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSERGOPTS= $(CAVENV) $(RGOPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM -UCAV_SUPPORT -DRG_TBSZ_RES
CCCAVSEUMRGOPTS= $(CAVENV) $(RGOPTS)  $(STOPTS) $(CMOPTS)
CC4GMXRGOPTS= $(C4GMXENV) $(RGOPTS)  $(STOPTS) $(CMOPTS)
#YS_PICO_PHY_V05 needs to be disabled to work with PC PHY 0.3. Enable it for 0.5 and 1.0.0
#TF defines 
CCLNXTFOPTS= $(LNXENV) $(TFOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSETFOPTS= $(CAVENV) $(TFOPTS1)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM -DNOFILESYS -DYS_PICO_PHY_V05
CCCAVSEUMTFOPTS= $(CAVENV) $(TFOPTS1)  $(STOPTS) $(CMOPTS)

#EG defines 
CCLNXEGOPTS= $(LNXENV) $(EGOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSEEGOPTS= $(CAVENV) $(EGOPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM
CCCAVSEUMEGOPTS= $(CAVENV) $(EGOPTS)  $(STOPTS) $(CMOPTS)

#SZ defines
CCLNXSZOPTS= $(LNXENV) $(SZOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSESZOPTS= $(CAVENV) $(SZOPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM
CCCAVSEUMSZOPTS= $(CAVENV) $(SZOPTS)  $(STOPTS) $(CMOPTS)

#CZ defines
CCLNXCZOPTS= $(LNXENV) $(CZOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSECZOPTS= $(CAVENV) $(CZOPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM
CCCAVSEUMCZOPTS= $(CAVENV) $(CZOPTS)  $(STOPTS) $(CMOPTS)

#WR defines 
CCLNXWROPTS= $(LNXENV) $(WROPTS)  $(STOPTS) $(CMOPTS)
CCCAVSEWROPTS= $(CAVENV) $(WROPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM
CCCAVSEUMWROPTS= $(CAVENV) $(WRSEUMOPTS)  $(STOPTS) $(CMOPTS) -DSS_SEUM_CAVIUM

#SB defines 
CCLNXSBOPTS= $(LNXENV) $(SBOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSESBOPTS= $(CAVENV) $(SBOPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM
CCCAVSEUMSBOPTS= $(CAVENV) $(SBOPTS)  $(STOPTS) $(CMOPTS)

#HI defines 
CCLNXHIOPTS= $(LNXENV) $(HIOPTS)  $(STOPTS) $(CMOPTS)
CCCAVSEHIOPTS= $(CAVENV) $(HIOPTS)  $(STOPTS) $(CMOPTS) -DSS_CAVIUM
CCCAVSEUMHIOPTS= $(CAVENV) $(HIOPTS)  $(STOPTS) $(CMOPTS)

#common options 
CCLNXCMOPTS= $(LNXENV) $(CMOPTS) $(SMOPTS) \
         $(MKUOPTS) $(CMKOPTS)  \
         $(CRLOPTS) $(RLUOPTS)  $(STOPTS) \
         $(CTCOPTS) $(TCUOPTS) $(LRXOPTS) 

CC4GMXCMOPTS= $(C4GMXENV) $(CMOPTS) $(SMOPTS) \
         $(MKUOPTS) $(CMKOPTS)  \
         $(CRLOPTS) $(RLUOPTS)  $(STOPTS) \
         $(CTCOPTS) $(TCUOPTS) $(LRXOPTS)

CCCAVSECMOPTS= $(CAVENV) $(CMOPTS) $(SMOPTS) \
         $(MKUOPTS) $(CMKOPTS)  \
         $(CRLOPTS) $(RLUOPTS)  $(STOPTS) \
         $(CTCOPTS) $(TCUOPTS) $(LRXOPTS) -DSS_CAVIUM 

CCCAVSEUMCMOPTS= $(CAVENV) $(CMOPTS) $(SMOPTS) \
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
ifeq (${BLDENV}, cav_se_perf)
COMPILE = $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

ASSEMBLE = $(CC) $(ASFLAGS_GLOBAL) $(ASFLAGS_LOCAL) -MD -c -o $@ $<

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk
endif

ifeq (${BLDENV}, cav_seum_perf)
COMPILE = $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

ASSEMBLE = $(CC) $(ASFLAGS_GLOBAL) $(ASFLAGS_LOCAL) -MD -c -o $@ $<

#dir := $(OCTEON_ROOT)/executive
dir := ../cavsdk/executive
include $(dir)/cvmx.mk
endif

ifeq (${BLDENV}, cav_seum_perf_uu)
COMPILE = $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

ASSEMBLE = $(CC) $(ASFLAGS_GLOBAL) $(ASFLAGS_LOCAL) -MD -c -o $@ $<

#dir := $(OCTEON_ROOT)/executive
dir := ../cavsdk/executive
include $(dir)/cvmx.mk
endif

ifeq (${BLDENV}, cav_se_e2e)
COMPILE = $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

ASSEMBLE = $(CC) $(ASFLAGS_GLOBAL) $(ASFLAGS_LOCAL) -MD -c -o $@ $<

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk
endif


ifeq (${BLDENV}, cav_seum_e2e)
COMPILE = $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

ASSEMBLE = $(CC) $(ASFLAGS_GLOBAL) $(ASFLAGS_LOCAL) -MD -c -o $@ $<

#dir := $(OCTEON_ROOT)/executive
dir := ../cavsdk/executive
include $(dir)/cvmx.mk
endif
ifeq (${BLDENV}, cav_se_pico)
COMPILE = $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

ASSEMBLE = $(CC) $(ASFLAGS_GLOBAL) $(ASFLAGS_LOCAL) -MD -c -o $@ $<

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk
endif


ifeq (${BLDENV}, cav_seum_pico)
COMPILE = $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<

ASSEMBLE = $(CC) $(ASFLAGS_GLOBAL) $(ASFLAGS_LOCAL) -MD -c -o $@ $<

#dir := $(OCTEON_ROOT)/executive
dir := ../cavsdk/executive
include $(dir)/cvmx.mk
endif


LNXPRDENB_PERF_OBJS: \
    $(BLD_LNX_RM_OBJS) \
    $(BLD_LNX_YS_OBJS) \
    $(BLD_LNX_CM_PERF_OBJS) \
    $(BLD_LNX_SS_OBJS) \
    $(BLD_LNX_KW_OBJS) \
    $(BLD_LNX_RG_OBJS) \
    $(BLD_LNX_NH_OBJS) \
    $(BLD_LNX_WR_PERF_OBJS)

LNXPRDENB_E2E_PRES: \
    $(BLD_LNX_YS_PREPROC) \
    $(BLD_LNX_KW_PREPROC) \
    $(BLD_LNX_RG_PREPROC) \
    $(BLD_LNX_NH_PREPROC) \
    $(BLD_LNX_SZ_PREPROC) \
    $(BLD_LNX_CZ_PREPROC) \
    $(BLD_LNX_SB_PREPROC) \
    $(BLD_LNX_HI_PREPROC) \
    $(BLD_LNX_WR_E2E_PREPROC) \
    $(BLD_LNX_EG_PREPROC)\
    $(BLD_LNX_RM_PREPROC)

LNXPRDENB_E2E_OBJS: \
    $(BLD_LNX_CM_OBJS) \
    $(BLD_LNX_SS_OBJS) \
    $(BLD_LNX_YS_OBJS1) \
    $(BLD_LNX_KW_OBJS1) \
    $(BLD_LNX_RG_OBJS1) \
    $(BLD_LNX_NH_OBJS1) \
    $(BLD_LNX_SZ_OBJS1) \
    $(BLD_LNX_CZ_OBJS1) \
    $(BLD_LNX_SB_OBJS1) \
    $(BLD_LNX_HI_OBJS1) \
    $(BLD_LNX_WR_E2E_OBJS1) \
    $(BLD_LNX_EG_OBJS1) \
    $(BLD_LNX_RM_OBJS1)

LNXPRDENB_E2E_OBJS_KWORKS: \
    $(BLD_LNX_CM_OBJS) \
    $(BLD_LNX_SS_OBJS) \
    $(BLD_LNX_YS_OBJS) \
    $(BLD_LNX_KW_OBJS) \
    $(BLD_LNX_RG_OBJS) \
    $(BLD_LNX_NH_OBJS) \
    $(BLD_LNX_SZ_OBJS) \
    $(BLD_LNX_CZ_OBJS) \
    $(BLD_LNX_SB_OBJS) \
    $(BLD_LNX_HI_OBJS) \
    $(BLD_LNX_WR_E2E_OBJS) \
    $(BLD_LNX_EG_OBJS) \
    $(BLD_LNX_RM_OBJS) 

ARMPRDENB_PERF_OBJS: \
    $(BLD_LNX_RM_OBJS) \
    $(BLD_LNX_YS_OBJS) \
    $(BLD_LNX_CM_PERF_OBJS) \
    $(BLD_LNX_SS_OBJS) \
    $(BLD_LNX_KW_OBJS) \
    $(BLD_LNX_RG_OBJS) \
    $(BLD_LNX_NH_OBJS) \
    $(BLD_LNX_WR_PERF_OBJS) 
 
ARMPRDENB_E2E_OBJS: \
    $(BLD_LNX_RM_OBJS) \
    $(BLD_LNX_YS_OBJS) \
    $(BLD_LNX_CM_OBJS) \
    $(BLD_LNX_SS_OBJS) \
    $(BLD_LNX_KW_OBJS) \
    $(BLD_LNX_RG_OBJS) \
    $(BLD_LNX_NH_OBJS) \
    $(BLD_LNX_SZ_OBJS) \
    $(BLD_LNX_CZ_OBJS) \
    $(BLD_LNX_SB_OBJS) \
    $(BLD_LNX_HI_OBJS) \
    $(BLD_LNX_WR_E2E_OBJS) \
    $(BLD_LNX_EG_OBJS)


LNXPRDENB_E2E_MS_OBJS: \
    $(BLD_LNX_RM_OBJS) \
    $(BLD_LNX_YS_MS_OBJS) \
    $(BLD_LNX_CM_OBJS) \
    $(BLD_LNX_SS_OBJS) \
    $(BLD_LNX_KW_OBJS) \
    $(BLD_LNX_RG_OBJS) \
    $(BLD_LNX_NH_OBJS) \
    $(BLD_LNX_SZ_OBJS) \
    $(BLD_LNX_CZ_OBJS) \
    $(BLD_LNX_SB_OBJS) \
    $(BLD_LNX_HI_OBJS) \
    $(BLD_LNX_WR_E2E_OBJS) \
    $(BLD_LNX_EG_OBJS)


CAVPRDENB_PERF_SE_OBJS: \
    $(CVMX_CONFIG) \
    $(BLD_CAV_YS_OBJS) \
    $(BLD_CAV_CM_OBJS) \
    $(BLD_CAV_SS_OBJS) \
    $(BLD_CAV_KW_OBJS) \
    $(BLD_CAV_RG_OBJS) \
    $(LIBS_LIST)

CAVPRDENB_PERF_SEUM_OBJS: \
    $(BLD_CAV_CM_OBJS2_PERF) \
    $(BLD_CAV_SS_OBJS2) \
    $(BLD_CAV_NH_OBJS2) \
    $(BLD_CAV_WR_PERF_OBJS2) \
    $(LIBS_LIST)

CAVPRDENB_E2E_SE_OBJS: \
    $(CVMX_CONFIG) \
    $(BLD_CAV_YS_OBJS) \
    $(BLD_CAV_CM_OBJS) \
    $(BLD_CAV_SS_OBJS) \
    $(BLD_CAV_KW_OBJS) \
    $(BLD_CAV_RG_OBJS) \
    $(LIBS_LIST)

CAVPRDENB_E2E_SEUM_OBJS: \
    $(BLD_CAV_CM_OBJS2) \
    $(BLD_CAV_SS_OBJS2) \
    $(BLD_CAV_NH_OBJS2) \
    $(BLD_CAV_SZ_OBJS2) \
    $(BLD_CAV_CZ_OBJS2) \
    $(BLD_CAV_SB_OBJS2) \
    $(BLD_CAV_HI_OBJS2) \
    $(BLD_CAV_WR_E2E_OBJS2) \
    $(BLD_CAV_EG_OBJS2) \
    $(LIBS_LIST)
#for pc
LNXPRDENB_PICO_OBJS: \
    $(BLD_LNX_RM_OBJS) \
    $(BLD_LNX_YS_PICO_OBJS) \
    $(BLD_LNX_CM_OBJS) \
    $(BLD_LNX_SS_OBJS) \
    $(BLD_LNX_KW_OBJS) \
    $(BLD_LNX_RG_OBJS) \
    $(BLD_LNX_NH_OBJS) \
    $(BLD_LNX_SZ_OBJS) \
    $(BLD_LNX_SB_OBJS) \
    $(BLD_LNX_HI_OBJS) \
    $(BLD_LNX_WR_E2E_OBJS) \
    $(BLD_LNX_EG_OBJS)

CAVPRDENB_PICO_SE_OBJS: \
    $(CVMX_CONFIG) \
    $(BLD_CAV_CL_OBJS) \
    $(BLD_CAV_PA_OBJS) \
    $(BLD_CAV_SE_CM_OBJS) \
    $(BLD_CAV_YS_PICO_OBJS) \
    $(BLD_CAV_CM_OBJS) \
    $(BLD_CAV_SS_OBJS) \
    $(BLD_CAV_KW_OBJS) \
    $(BLD_CAV_RG_OBJS) \
    $(LIBS_LIST)

CAVPRDENB_PICO_SEUM_OBJS: \
	$(BLD_CAV_CM_OBJS2) \
	$(BLD_CAV_SS_OBJS2) \
	$(BLD_CAV_NH_OBJS2) \
	$(BLD_CAV_SZ_OBJS2) \
	$(BLD_CAV_CZ_OBJS2) \
	$(BLD_CAV_SB_OBJS2) \
	$(BLD_CAV_HI_OBJS2) \
	$(BLD_CAV_VE_PICO_OBJS2) \
	$(BLD_CAV_EG_OBJS2) \
	$(LIBS_LIST)


CAVENBBIN1=enb_se
CAVENBBIN2=enb_seum
LNXENBPERFBIN=enb_perf
LNXENBE2EBIN=enb_e2e
LNXENBE2EPRE=enb_pre
LNXENBE2EKWORKSBIN=enb_e2e_kworks
LNXENBE2EBIN_MS=enb_e2e_ms
ARMENBPERFBIN=enb_arm_perf
ARMENBE2EBIN=enb_arm_e2e
CAVENBPERFSEBIN=enb_se_perf
CAVENBPERFSEUMBIN=enb_seum_perf
CAVENBPERFSEUMBINUU=enb_seum_perf_uu
CAVENBE2ESEBIN=enb_se_e2e
#CAVENBE2ESEBINN32=enb_se_e2e_n32
CAVENBE2ESEUMBIN=enb_seum_e2e
#CAVENBE2ESEUMBINN32=enb_seum_e2e_n32
LNXLIBBIN=lnxlib
#for pc
LNXENBPICOBIN=enb_pico
CAVENBPICOSEBIN=enb_se_pico
CAVENBPICOSEUMBIN=enb_seum_pico

BASENAME=enodeb_pal_lnx
MAKENAME=$(BASENAME).mak

ifeq ($(BLDENV),lnx_perf_msstub)
BINLNXPERF=lnx_perf_msstub
else
BINLNXPERF=lnx_perf
endif
ifeq ($(BLDENV),lnx_e2e_msstub)
BINLNXE2E=lnx_e2e_msstub
else
BINLNXE2E=lnx_e2e
PRELNXE2E=lnx_pre
endif
BINLNXE2E_KWORKS=lnx_kworks
BINLNXE2E_MS=lnx_e2e_ms
BINARMPERF=arm_perf_msstub
BINARME2E=arm_e2e_msstub
BINCAVPERFSE=cav_se_perf
BINCAVPERFSEUM=cav_seum_perf
BINCAVE2ESE=cav_se_e2e
#BINCAVE2ESEN32=cav_se_e2e_n32
BINCAVE2ESEUM=cav_seum_e2e
#BINCAVE2ESEUMN32=cav_seum_e2e_n32
BINCAVACC=cav_acc
BINLNXLIB=lnx_lib

BIN4GMXE2E=4gmx_e2e
4gmx_e2e : enodeb.elf

BIN4GMXE2ELIB=4gmx_e2e_lib
4gmx_e2e_lib : enodeb.a

ifeq (${PICO}, pico_inc)
#FOR PC
BINLNXPICO=lnx_pico
BINCAVPICOSE=cav_se_pico
BINCAVPICOSEUM=cav_seum_pico
BLDPICOSEUM=picoseum
endif
BLDSE=se
BLDSEUM=seum
#
#
# Linux Default GNU acceptance test build 
#
acc:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) 

#	@echo "Linux Performance Build"
$(BINLNXPERF):
	@$(MAKE) -f $(MAKENAME) $(LNXENBPERFBIN) CC=$(LNX_CC) AR=$(LNX_AR) \
	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBPERFOPTS)' 


#  @echo "Linux End To End with Purify"
#$(BINLNXE2E):
#	@$(MAKE) -f $(MAKENAME) $(LNXENBE2EBIN) CC=$(LNX_CC) AR=$(LNX_AR) \
#	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBE2EOPTS)' 


#	@echo "Linux End To End Build"
$(BINLNXE2E):
	@mkdir -p $(SY_DIR)
	@$(MAKE) -f $(MAKENAME) $(LNXENBE2EBIN) CC=$(LNX_CC) AR=$(LNX_AR) \
	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBE2EOPTS)' 

$(PRELNXE2E):
	@$(MAKE) -f $(MAKENAME) $(LNXENBE2EPRE) CC=$(LNX_CC) AR=$(LNX_AR) \
	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBE2EOPTS)' 


$(BINLNXE2E_KWORKS):
	@mkdir -p $(SY_DIR)
	@$(MAKE) -f $(MAKENAME) $(LNXENBE2EKWORKSBIN) CC=$(LNX_CC) AR=$(LNX_AR) \
	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBE2EOPTS)' 

#	@echo "Linux End To End Build"
$(BINLNXE2E_MS):
	@$(MAKE) -f $(MAKENAME) $(LNXENBE2EBIN_MS) CC=$(LNX_CC) \
	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBE2EOPTS)' 

#	@echo "ARM Performance Build"
$(BINARMPERF):
	@$(MAKE) -f $(MAKENAME) $(ARMENBPERFBIN) CC=$(ARM_CC) \
	LL=$(ARM_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBPERFOPTS)' 

#	@echo "ARM End To End Build"
$(BINARME2E):
	@$(MAKE) -f $(MAKENAME) $(ARMENBE2EBIN) CC=$(ARM_CC) \
	LL=$(ARM_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBE2EOPTS)' 


$(BINCAVPERFSE):
	@$(MAKE) -f $(MAKENAME) $(CAVENBPERFSEBIN) CC=$(LNXCC) \
	LL=$(LNXLD) COPTS=$(CAVCOPTS) CMOPTS='$(CMENBPERFSEOPTS)' 

$(BINCAVPERFSEUM):
	@$(MAKE) -f $(MAKENAME) $(CAVENBPERFSEUMBIN) CC=$(LNXCC) \
	LL=$(LNXLD) COPTS=$(CAVCOPTS) CMOPTS='$(CMENBPERFSEUMOPTS)' 

$(BINCAVE2ESE):
	@$(MAKE) -f $(MAKENAME) $(CAVENBE2ESEBIN) CC=$(LNXCC) \
	LL=$(LNXLD) COPTS=$(CAVCOPTS) CMOPTS='$(CMENBE2ESEOPTS)' 

#$(BINCAVE2ESEN32):
#	@$(MAKE) -f $(MAKENAME) $(CAVENBE2ESEBINN32) CC=$(LNXCC) \
#	LL=$(LNXLD) COPTS=$(CAVCOPTS) CMOPTS='$(CMENBE2ESEOPTS)' 


$(BINCAVE2ESEUM):
	@$(MAKE) -f $(MAKENAME) $(CAVENBE2ESEUMBIN) CC=$(LNXCC) \
	LL=$(LNXLD) COPTS=$(CAVCOPTS) CMOPTS='$(CMENBE2ESEUMOPTS)' 

#$(BINCAVE2ESEUMN32):
#	@$(MAKE) -f $(MAKENAME) $(CAVENBE2ESEUMBINN32) CC=$(LNXCC) \
#	LL=$(LNXLD) COPTS=$(CAVCOPTS) CMOPTS='$(CMENBE2ESEUMOPTS)' 
#for picochip
$(BINLNXPICO):
	@echo "Linux PICO Build"
	@$(MAKE) -f $(MAKENAME) $(LNXENBPICOBIN) CC=$(LNX_CC) \
	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBE2EOPTS)' 
  
$(BINCAVPICOSE):
	@echo "Cav PICO se Build"
	@$(MAKE) -f $(MAKENAME) $(CAVENBPICOSEBIN) CC=$(LNXCC) \
	LL=$(LNXLD) COPTS=$(CAVCOPTS) CMOPTS='$(CMENBE2ESEOPTS)' 

$(BINCAVPICOSEUM):
	@$(MAKE) -f $(MAKENAME) $(CAVENBPICOSEUMBIN) CC=$(LNXCC) \
	LL=$(LNXLD) COPTS=$(CAVCOPTS) CMOPTS='$(CMENBE2ESEUMOPTS)' 


$(BINLNXLIB):
	@$(MAKE) -f $(MAKENAME) $(LNXLIBBIN) CC=$(LNX_CC) \
	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBE2EOPTS)' 

$(LNXENBPERFBIN):LNXPRDENB_PERF_OBJS 
	$(CC)  $(SY_DIR)/*.o $(LNXLIBPATH) $(LNXLOPTS) -o ./$(SY_DIR)/$(OUTNAME)

$(LNXENBE2EBIN):LNXPRDENB_E2E_OBJS 
	$(CC)  $(SY_DIR)/*.o $(SY_DIR4)/*.o $(LNXLIBPATH) $(LNXLOPTS) -o ./$(SY_DIR)/$(OUTNAME)

$(LNXENBE2EKWORKSBIN):LNXPRDENB_E2E_OBJS_KWORKS
	$(CC)  $(SY_DIR)/*.o $(LNXLIBPATH) $(LNXLOPTS) -o ./$(SY_DIR)/$(OUTNAME)

$(LNXENBE2EPRE):LNXPRDENB_E2E_PRES 

$(ARMENBPERFBIN):ARMPRDENB_PERF_OBJS 
	$(CC)  $(SY_DIR)/*.o $(ARMLIBPATH) $(LNXLOPTS) -o ./$(SY_DIR)/$(OUTNAME)

$(ARMENBE2EBIN):ARMPRDENB_E2E_OBJS 
	$(CC)  $(SY_DIR)/*.o $(ARMLIBPATH) $(LNXLOPTS) -o ./$(SY_DIR)/$(OUTNAME)

$(LNXENBE2EBIN_MS):LNXPRDENB_E2E_MS_OBJS 
	$(CC)  $(SY_DIR)/*.o $(LNXLIBPATH) $(LNXLOPTS) -o ./$(SY_DIR)/$(OUTNAME)

$(CAVENBPERFSEBIN):CAVPRDENB_PERF_SE_OBJS
	$(CC)  $(SY_DIR1)/*.o $(CAVLIBPATH) $(CAVSELOPTS) -o ./$(SY_DIR1)/$(OUTNAME1)

$(CAVENBPERFSEUMBIN):CAVPRDENB_PERF_SEUM_OBJS 
	$(CC)  $(SY_DIR2)/*.o $(CAVLIBPATH) $(CAVSEUMLOPTS) -o ./$(SY_DIR2)/$(OUTNAME2)

$(CAVENBE2ESEBIN):CAVPRDENB_E2E_SE_OBJS 
	$(CC)  $(SY_DIR1)/*.o $(CAVLIBPATH) $(CAVSELOPTS) -o ./$(SY_DIR1)/$(OUTNAME1)

#$(CAVENBE2ESEBINN32):CAVPRDENB_E2E_SE_OBJS 
#	$(CC)  $(SY_DIR1)/*.o $(CAVLIBPATH) $(CAVSELOPTS) -o ./$(SY_DIR1)/$(OUTNAME1)

ifeq (${BLDENV},4gmx_e2e_lib)
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
	$(CM_DIR)/lnh.c\
	$(CM_DIR)/lrg.c\
	$(CM_DIR)/nhu.c\
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
	$(CM_DIR)/ve_se_tst.c\

MTSRCS= \
	$(MTSE_DIR)/jz_mem.c\
	$(MTSE_DIR)/jz_cmem.c\
	$(MTSE_DIR)/jz_id.c\
	$(MTSE_DIR)/jz_msg.c\
	$(MTSE_DIR)/jz_ss.c\
	$(MTSE_DIR)/jz_gen.c\
	$(MTSE_DIR)/jz_pack.c\
	$(MTSE_DIR)/jz_queue.c\
	$(MTSE_DIR)/jz_strm.c\
	$(MTSE_DIR)/jz_task.c\
	$(MTSE_DIR)/jz_timer.c

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
endif


$(CAVENBE2ESEUMBIN):CAVPRDENB_E2E_SEUM_OBJS 
	$(CC)  $(SY_DIR2)/*.o $(CAVLIBPATH) $(CAVSEUMLOPTS) -o ./$(SY_DIR2)/$(OUTNAME2)

#for pc
$(LNXENBPICOBIN):LNXPRDENB_PICO_OBJS
	$(CC)  $(SY_DIR)/*.o $(LNXLIBPATH) $(LNXLOPTS) -o ./$(SY_DIR)/$(OUTNAME)

$(CAVENBPICOSEBIN):CAVPRDENB_PICO_SE_OBJS
	$(CC)  $(SY_DIR1)/*.o $(CAVLIBPATH) $(CAVSELOPTS) -o ./$(SY_DIR1)/$(OUTNAME1)

#$(CAVENBE2ESEUMBINN32):CAVPRDENB_E2E_SEUM_OBJS 
#	$(CC)  $(SY_DIR2)/*.o $(CAVLIBPATH) $(CAVSEUMLOPTS) -o ./$(SY_DIR2)/$(OUTNAME2)

$(CAVENBPICOSEUMBIN):CAVPRDENB_PICO_SEUM_OBJS
	$(CC)  $(SY_DIR2)/*.o $(CAVLIBPATH) $(CAVSEUMLOPTS) -o ./$(SY_DIR2)/$(OUTNAME2)
# Portable compilation -> Point to acc directly
# Portability is not supported
pt:acc

$(BINLNXPT):
	@$(MAKE) -f $(MAKENAME) $(ENBBIN)  CC=$(LNXCC) \
	LL=$(LNXLD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBOPTS)'

#-----------------------------------------------------------------------
# UNIX link - absolute object file depends on relocatable object
# (.o) files

#
# Making Common Libraries
#
#$(BLD_LNX_RM_OBJS):
#	@$(MAKE) -f rm.mak $(SY_DIR)/librm.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
#	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXRMOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(RM_DIR)' \
#	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_RM_PREPROC):
	@$(MAKE) -f rm.mak $(SY_DIR)/rmperpro COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXRMOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(RM_DIR)'  IN_DIR='$(RM_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_RM_OBJS1):
	@$(MAKE) -f rm.mak $(SY_DIR)/librm.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXRMOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(RM_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="i"  CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_RM_OBJS):
	@$(MAKE) -f rm.mak $(SY_DIR)/librm.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXRMOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(RM_DIR)' IN_DIR='$(RM_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c"  CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

#
# Making Common Libraries
#
#$(BLD_4GMX_CM_OBJS):
#	@$(MAKE) -f cm.mak $(SY_DIR3)/libcm.a COPTS=$(C4GMXCOPTS) \
	LOPTS='$(C4GMXLOPTS)' POPTS='$(CC4GMXCMOPTS)' IOPTS='$(C4GMXIOPTS)' IN_DIR='$(CM_DIR)' \
	OUT_DIR='$(SY_DIR3)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  CM_INC='$(CM_INC1)'

$(BLD_LNX_CM_OBJS):
	@$(MAKE) -f cm.mak $(SY_DIR)/libcm.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXCMOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(CM_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  CM_INC='$(CM_INC)'

$(BLD_LNX_CM_PERF_OBJS):
	@$(MAKE) -f cm.uu.mak $(SY_DIR)/libcmuu.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXCMOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(CM_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  CM_INC='$(CM_INC)'

$(BLD_CAV_CM_OBJS):
	@$(MAKE) -f cm.se.mak $(SY_DIR1)/libcm.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSECMOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(CM_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  CM_INC='$(CM_INC1)'

$(BLD_CAV_CM_OBJS2):
	@$(MAKE) -f cm.mak $(SY_DIR2)/libcm.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMCMOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(CM_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  CM_INC='$(CM_INC1)'

$(BLD_CAV_CM_OBJS2_PERF):
	@$(MAKE) -f cm.uu.mak $(SY_DIR2)/libcmuu.a COPTS=$(CAVCOPTS)  AR=$(AR)\
        LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMCMOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(CM_DIR)' \
        OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  CM_INC='$(CM_INC1)'
#
#Making eNodeB Sample application
#
$(BLD_LNX_WR_PERF_OBJS):
#	@$(MAKE) -f wr.mak $(SY_DIR)/libperflnxwruu.a COPTS=$(LNXCOPTS)  AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXWROPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(WR_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' 

#$(BLD_LNX_WR_E2E_OBJS):
#	@$(MAKE) -f wr.mak $(SY_DIR)/libe2elnxwr.a COPTS=$(LNXCOPTS)  AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXWROPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(WR_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)' 

$(BLD_LNX_WR_E2E_PREPROC):
#	@$(MAKE) -f wr.mak $(SY_DIR)/wrperpro COPTS=$(LNXCOPTS)  AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXWROPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(WR_DIR)' IN_DIR='$(WR_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ="i" SRC="c" CC="$(CC) -E" LL='$(LL)' CM_INC='$(CM_INC)'  

$(BLD_LNX_WR_E2E_OBJS1):
#	@$(MAKE) -f wr.mak $(SY_DIR)/libe2elnxwr.a COPTS=$(LNXCOPTS)  AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXWROPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(WR_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="i" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'  

$(BLD_LNX_WR_E2E_OBJS):
#	@$(MAKE) -f wr.mak $(SY_DIR)/libe2elnxwr.a COPTS=$(LNXCOPTS)  AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXWROPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(WR_DIR)' IN_DIR='$(WR_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'  

$(LNXLIBBIN):
#	@$(MAKE) -f wr.mak $(SY_LIB_DIR)/libe2elnxwr.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXWROPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXLIBIOPTS)' IN_DIR='$(WR_LIB_DIR)' \
	OUT_DIR='$(SY_LIB_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC2)' 

$(BLD_CAV_WR_OBJS):
#	@$(MAKE) -f wr.mak $(SY_DIR1)/libwr.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEWROPTS)' SMPOPTS='$(CCCAVSESMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(WR_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)' 

$(BLD_CAV_WR_PERF_OBJS2):
#	@$(MAKE) -f wr.mak $(SY_DIR2)/libperfcavwruu.a BLD='$(BLDSEUM)' COPTS=$(CAVCOPTS)  AR=$(AR)\
        LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMWROPTS)' SMPOPTS='$(CCCAVSEUMSMOPTS)' \
        HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(WR_DIR)' \
        OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)' 

$(BLD_CAV_WR_E2E_OBJS2):
#	@$(MAKE) -f wr.mak $(SY_DIR2)/libe2ecavwr.a BLD='$(BLDSEUM)' COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMWROPTS)' SMPOPTS='$(CCCAVSEUMSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(WR_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)' 

ifeq (${BLDENV}, cav_seum_pico)
$(BLD_CAV_WR_PICO_OBJS2):
	@echo "build WR Pico OBJS"       
	@$(MAKE) -f wr.mak $(SY_DIR2)/libe2ecavwr.a BLD='$(BLDPICOSEUM)' COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMWROPTS)' SMPOPTS='$(CCCAVSEUMSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(WR_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)' 

endif
#
#Making LTE RRC Layer
#
$(BLD_LNX_NH_PREPROC):
	@$(MAKE) -f nh.mak $(SY_DIR)/nhperpro COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXNHOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(NH_DIR)' IN_DIR='$(NH_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_NH_OBJS1):
	@$(MAKE) -f nh.mak $(SY_DIR)/libnh.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXNHOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(NH_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="i" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_NH_OBJS):
	@$(MAKE) -f nh.mak $(SY_DIR)/libnh.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXNHOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(NH_DIR)' IN_DIR='$(NH_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_CAV_NH_OBJS):
	@$(MAKE) -f nh.mak $(SY_DIR1)/libnh.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSENHOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(NH_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_CAV_NH_OBJS2):
	@$(MAKE) -f nh.mak $(SY_DIR2)/libnh.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMNHOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(NH_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

#
#Making RLC Layer
#
#$(BLD_4GMX_KW_OBJS):
#	@$(MAKE) -f kw.mak $(SY_DIR3)/libkw.a COPTS=$(C4GMXCOPTS) \
	LOPTS='$(C4GMXLOPTS)' POPTS='$(CC4GMXKWOPTS)' IOPTS='$(C4GMXIOPTS)' IN_DIR='$(KW_DIR)' \
	OUT_DIR='$(SY_DIR3)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_LNX_KW_PREPROC):
	@$(MAKE) -f kw.mak $(SY_DIR)/kwperpro COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXKWOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(KW_DIR)' IN_DIR='$(KW_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_KW_OBJS1):
	@$(MAKE) -f kw.mak $(SY_DIR)/libkw.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXKWOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(KW_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="i" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_LNX_KW_OBJS):
	@$(MAKE) -f kw.mak $(SY_DIR)/libkw.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXKWOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(KW_DIR)' IN_DIR='$(KW_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_CAV_KW_OBJS):
	@$(MAKE) -f kw.mak $(SY_DIR1)/libkw.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEKWOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(KW_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
   
$(BLD_CAV_KW_OBJS2):
	@$(MAKE) -f kw.mak $(SY_DIR2)/libkw.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMKWOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(KW_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

#
#Making MAC Layer
#
#$(BLD_4GMX_RG_OBJS):
#	@$(MAKE) -f rg.mak $(SY_DIR3)/librg.a COPTS=$(C4GMXCOPTS) \
	LOPTS='$(C4GMXLOPTS)' POPTS='$(CC4GMXRGOPTS)' IOPTS='$(C4GMXIOPTS)' IN_DIR='$(RG_DIR)' \
	OUT_DIR='$(SY_DIR3)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_LNX_RG_PREPROC):
	@$(MAKE) -f rg.mak $(SY_DIR)/rgperpro COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXRGOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(RG_DIR)' IN_DIR='$(RG_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_RG_OBJS1):
	@$(MAKE) -f rg.mak $(SY_DIR)/librg.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXRGOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(RG_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="i" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_RG_OBJS):
	@$(MAKE) -f rg.mak $(SY_DIR)/librg.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXRGOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(RG_DIR)' IN_DIR='$(RG_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_CAV_RG_OBJS):
	@$(MAKE) -f rg.mak $(SY_DIR1)/librg.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSERGOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(RG_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_CAV_RG_OBJS2):
	@$(MAKE) -f rg.mak $(SY_DIR2)/librg.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMRGOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(RG_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
#
#Making CL Layer
#
#$(BLD_4GMX_YS_OBJS):
#	@$(MAKE) -f ys.mak $(SY_DIR3)/lib4gmxys.a COPTS=$(C4GMXCOPTS) \
	LOPTS='$(C4GMXLOPTS)' POPTS='$(CC4GMXTFOPTS)' IOPTS='$(C4GMXIOPTS)' IN_DIR='$(YS_DIR)' \
	OUT_DIR='$(SY_DIR3)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_LNX_TF_OBJS):
	@$(MAKE) -f tf.mak $(SY_DIR)/libtf.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXTFOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(TF_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_CAV_TF_OBJS):
	@$(MAKE) -f tf.mak $(SY_DIR1)/libtf.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSETFOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(TF_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_CAV_TF_OBJS2):
	@$(MAKE) -f tf.mak $(SY_DIR2)/libtf.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMTFOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(TF_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

ifeq ($(BLDENV),lnx_e2e_msstub)
$(BLD_LNX_YS_OBJS):
	@$(MAKE) -f ys_ms.mak $(SY_DIR)/liblnxys.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXTFOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(YS_MS_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'
else
ifeq ($(BLDENV),lnx_perf_msstub)	
$(BLD_LNX_YS_OBJS):
	@$(MAKE) -f ys_ms.mak $(SY_DIR)/liblnxys.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXTFOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(YS_MS_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'
else
ifeq ($(BLDENV),arm_perf_msstub)
$(BLD_LNX_YS_OBJS):
	@$(MAKE) -f ys_ms.mak $(SY_DIR)/liblnxys.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXTFOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(YS_MS_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'
else
ifeq ($(BLDENV),arm_e2e_msstub)
$(BLD_LNX_YS_OBJS):
	@$(MAKE) -f ys_ms.mak $(SY_DIR)/liblnxys.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXTFOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(YS_MS_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'
else
$(BLD_LNX_YS_PREPROC):
	@$(MAKE) -f ys.mak $(SY_DIR)/ysperpro COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXTFOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(YS_PAL_DIR)' IN_DIR='$(YS_PAL_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_YS_OBJS1):
	@$(MAKE) -f ys.mak $(SY_DIR)/liblnxys.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXTFOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(YS_PAL_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="i" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'
endif #end of arm_e2e_msstub
endif #end of arm_perf_msstub
endif #end of lnx_perf_msstub
endif #end of lnx_e2e_msstub	
$(BLD_LNX_YS_MS_OBJS):
	@$(MAKE) -f ys_ms.mak $(SY_DIR)/liblnxysms.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXTFOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(YS_MS_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'

# Support Cavium build
$(BLD_CAV_YS_OBJS):
	@$(MAKE) -f ys.mak $(SY_DIR1)/libcavys.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSETFOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(YS_PAL_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)' YS_MAIN_FLAG=YES 

$(BLD_CAV_YS_OBJS2):
	@$(MAKE) -f ys.mak $(SY_DIR2)/libcavys.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSEUMTFOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(YS_PAL_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)' YS_MAIN_FLAG=YES

$(BLD_LNX_YS_PICO_OBJS):
	@$(MAKE) -f ys_pc.mak $(SY_DIR)/liblnxpcys.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXTFOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(YS_PC_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_CAV_YS_PICO_OBJS):
	@$(MAKE) -f ys_pc.mak $(SY_DIR1)/libcavpcys.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSETFOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(YS_PC_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_LNX_YS_OBJS):
	@$(MAKE) -f ys.mak $(SY_DIR)/liblnxys.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXTFOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(YS_PAL_DIR)' IN_DIR='$(YS_PAL_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

#
#Making S1AP Layer
#
$(BLD_LNX_SZ_PREPROC):
	@$(MAKE) -f sz.mak $(SY_DIR)/szperpro COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSZOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(SZ_DIR)' IN_DIR='$(SZ_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_SZ_OBJS1):
	@$(MAKE) -f sz.mak $(SY_DIR)/libsz.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSZOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(SZ_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="i" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_SZ_OBJS):
	@$(MAKE) -f sz.mak $(SY_DIR)/libsz.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSZOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(SZ_DIR)' IN_DIR='$(SZ_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_CAV_SZ_OBJS):
	@$(MAKE) -f sz.mak $(SY_DIR1)/libsz.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSESZOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(SZ_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
	
$(BLD_CAV_SZ_OBJS2):
	@$(MAKE) -f sz.mak $(SY_DIR2)/libsz.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEUMSZOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(SZ_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

#
#
#Making X2AP Layer
#
$(BLD_LNX_CZ_PREPROC):
	@$(MAKE) -f cz.mak $(SY_DIR)/czperpro COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXCZOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(CZ_DIR)' IN_DIR='$(CZ_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_CZ_OBJS1):
	@$(MAKE) -f cz.mak $(SY_DIR)/libcz.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXCZOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(CZ_DIR)'  IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="i" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_CZ_OBJS):
	@$(MAKE) -f cz.mak $(SY_DIR)/libcz.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXCZOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(CZ_DIR)'  IN_DIR='$(CZ_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_CAV_CZ_OBJS):
	@$(MAKE) -f cz.mak $(SY_DIR1)/libcz.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSECZOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(CZ_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
	
$(BLD_CAV_CZ_OBJS2):
	@$(MAKE) -f cz.mak $(SY_DIR2)/libcz.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEUMCZOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(CZ_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

#
#Making SCTP Layer
#
$(BLD_LNX_SB_PREPROC):
	@$(MAKE) -f sb.mak $(SY_DIR)/sbperpro COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(SB_DIR)' IN_DIR='$(SB_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_SB_OBJS1):
	@$(MAKE) -f sb.mak $(SY_DIR)/libsb.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(SB_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="i" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_SB_OBJS):
	@$(MAKE) -f sb.mak $(SY_DIR)/libsb.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(SB_DIR)' IN_DIR='$(SB_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_CAV_SB_OBJS):
	@$(MAKE) -f sb.mak $(SY_DIR1)/libsb.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSESBOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(SB_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
	
$(BLD_CAV_SB_OBJS2):
	@$(MAKE) -f sb.mak $(SY_DIR2)/libsb.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEUMSBOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(SB_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
#
#Making TUCL Layer
#
$(BLD_LNX_HI_PREPROC):
	@$(MAKE) -f hi.mak $(SY_DIR)/hiperpro COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXHIOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(HI_DIR)' IN_DIR='$(HI_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_HI_OBJS1):
	@$(MAKE) -f hi.mak $(SY_DIR)/libhi.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXHIOPTS)' IOPTS='$(LNXIOPTS)'  VS_DIR='$(HI_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="i" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_HI_OBJS):
	@$(MAKE) -f hi.mak $(SY_DIR)/libhi.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXHIOPTS)' IOPTS='$(LNXIOPTS)'  VS_DIR='$(HI_DIR)' IN_DIR='$(HI_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'
	
$(BLD_CAV_HI_OBJS):
	@$(MAKE) -f hi.mak $(SY_DIR1)/libhi.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEHIOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(HI_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_CAV_HI_OBJS2):
	@$(MAKE) -f hi.mak $(SY_DIR2)/libhi.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEUMHIOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(HI_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'
#
#Making eGTP Layer
#
$(BLD_LNX_EG_PREPROC):
	@$(MAKE) -f eg.mak $(SY_DIR)/egperpro COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXEGOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(EG_DIR)' IN_DIR='$(EG_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_EG_OBJS1):
	@$(MAKE) -f eg.mak $(SY_DIR)/libeg.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXEGOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(EG_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="i" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_EG_OBJS):
	@$(MAKE) -f eg.mak $(SY_DIR)/libeg.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXEGOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(EG_DIR)' IN_DIR='$(EG_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_CAV_EG_OBJS):
	@$(MAKE) -f eg.mak $(SY_DIR1)/libeg.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEEGOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(EG_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

$(BLD_CAV_EG_OBJS2):
	@$(MAKE) -f eg.mak $(SY_DIR2)/libeg.a COPTS=$(CAVCOPTS)  AR=$(AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEUMEGOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(EG_DIR)' \
	OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC1)'

#
# making Multi Threaded SSI
#
#$(BLD_4GMX_SS_OBJS):
#	@$(MAKE) -f mtse.mak $(SY_DIR3)/libmt.a COPTS=$(C4GMXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CC4GMXMTOPTS)' IOPTS='$(C4GMXIOPTS)' IN_DIR='$(MTSE_DIR)' \
   OUT_DIR='$(SY_DIR3)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  \
   CM_INC='$(CM_INC1)'

$(BLD_LNX_SS_OBJS):
	@$(MAKE) -f mt.mak $(SY_DIR)/libmt.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXMTOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(MTSEUM_DIR)' \
   OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  \
   CM_INC='$(CM_INC)'

$(BLD_CAV_SS_OBJS):
	@$(MAKE) -f mtse.mak $(SY_DIR1)/libmt.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEMTOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(MTSE_DIR)' \
   OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  \
   CM_INC='$(CM_INC1)'

$(BLD_CAV_SS_OBJS2):
	@$(MAKE) -f mt.mak $(SY_DIR2)/libmt.a COPTS=$(CAVCOPTS) AR=$(AR) \
	LOPTS='$(LOPTS)' POPTS='$(CCCAVSEUMMTOPTS)' IOPTS='$(CAVIOPTS2)' IN_DIR='$(MTSEUM_DIR)' \
   OUT_DIR='$(SY_DIR2)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  \
   CM_INC='$(CM_INC1)'

$(BLD_CAV_CL_OBJS):
	@echo "Build CAV libs"
	@$(MAKE) -f ../ltephy/picochip/ltecl2/cl2.mak $(SY_DIR1)/libcl.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSETFOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(CL2_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_CAV_PA_OBJS):
	@$(MAKE) -f ../ltephy/picochip/ltecl2/pa_driver.mak $(SY_DIR1)/libpadriver.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSETFOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(PA_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_CAV_TST_OBJS):
	@$(MAKE) -f ../ltephy/picochip/test/tst.mak $(SY_DIR1)/libtst.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSETFOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(TST_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_CAV_SE_CM_OBJS):
	@$(MAKE) -f ../ltephy/picochip/common/common.mak $(SY_DIR1)/libsecom.a COPTS=$(CAVCOPTS) \
	LOPTS='$(CAVLOPTS)' POPTS='$(CCCAVSETFOPTS)' IOPTS='$(CAVIOPTS)' IN_DIR='$(SECM_DIR)' \
	OUT_DIR='$(SY_DIR1)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'


ifeq (${BLDENV}, 4gmx_e2e_lib)
clean:
	`del $(RM_DIR)/*.o $(KW_DIR)/*.o $(RG_DIR)/*.o $(YS_DIR)/*.o $(CM_DIR)/*.o $(MTSE_DIR)/*.o`
else
clean:
	\rm -f $(SY_DIR)/$(OUTNAME) $(SY_DIR)/* $(SY_DIR1)/* $(SY_LIB_DIR)/*.o $(SY_DIR2)/* $(SY_DIR1)/*.a $(SY_DIR2)/*.a $(SY_DIR)/*.a $(SY_DIR)/core $(SY_DIR)/*~ \
	$(SY_DIR)/*.bak $(SY_DIR)/*.err TeNB_Diag_db.txt
endif   
ifeq (${BLDENV}, cav_seum_pico)
cleanstack:
	@$(MAKE) -f rm.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR1)'
	@$(MAKE) -f mt.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR2)'
	@$(MAKE) -f mtse.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR1)'
	@$(MAKE) -f rg.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR1)'
	@$(MAKE) -f kw.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR1)'
	@$(MAKE) -f cm.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR1)'
	@$(MAKE) -f ys.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR1)'
	@$(MAKE) -f ys_pc.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR1)'
#	@$(MAKE) -f cl2.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR1)'
	@$(MAKE) -f nh.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR2)'
	@$(MAKE) -f hi.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR2)'
	@$(MAKE) -f sb.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR2)'
	@$(MAKE) -f sz.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR2)'
	@$(MAKE) -f wr.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR2)'
	@$(MAKE) -f eg.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR2)'
	@$(MAKE) -f cm.mak clean OBJ='$(OBJ)' OUT_DIR='$(SY_DIR2)'
	\rm -f $(SY_DIR1)/enb_se
	\rm -f $(SY_DIR2)/enb_seum
endif

# DO NOT DELETE THIS LINE -- make depend depends on it.

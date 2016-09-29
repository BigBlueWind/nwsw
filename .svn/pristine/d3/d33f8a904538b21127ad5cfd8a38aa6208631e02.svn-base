#/********************************************************************16**
#
#                         (c) COPYRIGHT 2011 by 
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
#     WITHOUT LIMITATION WARRANTIES OF MERCHAN	ILITY OR FITNESS FOR
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
#                    9450, Carroll Park Drive
#                    San Diego, CA-92121, USA
#
#                    Tel: +1 (858) 882 8800
#                    Fax: +1 (858) 777 3388
#
#                    Email: support@ccpu.com
#                    Web: http://www.ccpu.com
#
#*********************************************************************17*/
EXT_OPT=rg0021

RG_DIR=.

CHILD_LST= rg_cfg.c rg_com.c rg_dbm.c rg_dhm.c rg_dux.c rg_ex_ms.c rg_ex_pt.c \
			  rg_lim.c rg_lmm.c rg_mux.c rg_ptli.c rg_ptmi.c rg_ptui.c \
			  rg_ram.c rg_rom.c rg_sch.c rg_sch_cfg.c rg_sch_cmn.c rg_sch_dbm.c \
			  rg_sch_dhm.c rg_sch_ex_ms.c rg_sch_gom.c rg_sch_inf.c rg_sch_lmm.c \
			  rg_sch_mga.c rg_sch_pwr.c rg_sch_ram.c  rg_sch_sc1.c rg_sch_tmr.c \
			  rg_sch_tom.c rg_sch_uhm.c rg_sch_utl.c rg_tmr.c rg_tom.c rg_tst.c rg_uhm.c \
			  rg_uim.c rg_utl.c rg_sch_utl_clist.c rg_sch_pt.c  rg_sch_hdfdd.c rg_sch_l2m.c \
           rg_sch_drx.c rg_l2m.c rg_sch_sps.c
			  
all: $(CHILD_LST)

clean:
	/bin/rm -f $(CHILD_LST)


rg_cfg.c: gk_cfg.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_cfg.c rg_cfg.c -d -l

rg_com.c: gk_com.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_com.c rg_com.c -d -l

rg_dbm.c: gk_dbm.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_dbm.c rg_dbm.c -d -l

rg_dhm.c: gk_dhm.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_dhm.c rg_dhm.c -d -l

rg_dux.c: gk_dux.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_dux.c rg_dux.c -d -l

rg_ex_ms.c: gk_ex_ms.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_ex_ms.c rg_ex_ms.c -d -l

rg_gom.c: gk_gom.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_gom.c rg_gom.c -d -l

rg_lim.c: gk_lim.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_lim.c rg_lim.c -d -l

rg_lmm.c: gk_lmm.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_lmm.c rg_lmm.c -d -l

rg_mux.c: gk_mux.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_mux.c rg_mux.c -d -l

rg_ptli.c: gk_ptli.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_ptli.c rg_ptli.c -d -l

rg_ptmi.c: gk_ptmi.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_ptmi.c rg_ptmi.c -d -l

rg_ptui.c: gk_ptui.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_ptui.c rg_ptui.c -d -l

rg_ram.c: gk_ram.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_ram.c rg_ram.c -d -l

rg_rom.c: gk_rom.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_rom.c rg_rom.c -d -l


rg_sch.c: gk_sch.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch.c rg_sch.c -d -l

rg_sch_cfg.c: gk_sch_cfg.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_cfg.c rg_sch_cfg.c -d -l

rg_sch_cmn.c: gk_sch_cmn.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_cmn.c rg_sch_cmn.c -d -l

rg_sch_dbm.c: gk_sch_dbm.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_dbm.c rg_sch_dbm.c -d -l

rg_sch_dhm.c: gk_sch_dhm.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_dhm.c rg_sch_dhm.c -d -l

rg_sch_ex_ms.c: gk_sch_ex_ms.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_ex_ms.c rg_sch_ex_ms.c -d -l

rg_sch_gom.c: gk_sch_gom.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_gom.c rg_sch_gom.c -d -l

rg_sch_inf.c: gk_sch_inf.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_inf.c rg_sch_inf.c -d -l

rg_sch_lmm.c: gk_sch_lmm.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_lmm.c rg_sch_lmm.c -d -l

rg_sch_mga.c: gk_sch_mga.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_mga.c rg_sch_mga.c -d -l

rg_sch_pwr.c: gk_sch_pwr.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_pwr.c rg_sch_pwr.c -d -l

rg_sch_pt.c: gk_sch_pt.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_pt.c rg_sch_pt.c -d -l

rg_sch_ram.c: gk_sch_ram.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_ram.c rg_sch_ram.c -d -l

rg_sch_sc1.c: gk_sch_sc1.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_sc1.c rg_sch_sc1.c -d -l

rg_sch_tmr.c: gk_sch_tmr.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_tmr.c rg_sch_tmr.c -d -l

rg_sch_tom.c: gk_sch_tom.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_tom.c rg_sch_tom.c -d -l

rg_sch_uhm.c: gk_sch_uhm.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_uhm.c rg_sch_uhm.c -d -l

rg_sch_utl.c: gk_sch_utl.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_utl.c rg_sch_utl.c -d -l

rg_sch_utl_clist.c: gk_sch_utl_clist.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_utl_clist.c rg_sch_utl_clist.c -d -l

rg_tmr.c: gk_tmr.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_tmr.c rg_tmr.c -d -l

rg_tom.c: gk_tom.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_tom.c rg_tom.c -d -l

rg_tst.c: gk_tst.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_tst.c rg_tst.c -d -l

rg_uhm.c: gk_uhm.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_uhm.c rg_uhm.c -d -l

rg_uim.c: gk_uim.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_uim.c rg_uim.c -d -l

rg_utl.c: gk_utl.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_utl.c rg_utl.c -d -l

rg_ex_pt.c: gk_ex_pt.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_ex_pt.c rg_ex_pt.c -d -l

rg_l2m.c: gk_l2m.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_l2m.c rg_l2m.c -d -l

rg_sch_l2m.c: gk_sch_l2m.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_l2m.c rg_sch_l2m.c -d -l

rg_sch_drx.c: gk_sch_drx.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_drx.c rg_sch_drx.c -d -l

rg_sch_hdfdd.c: gk_sch_hdfdd.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_hdfdd.c rg_sch_hdfdd.c -d -l

rg_sch_sps.c: gk_sch_sps.c
	extprg $(EXT_OPT) $(RG_DIR)/gk_sch_sps.c rg_sch_sps.c -d -l


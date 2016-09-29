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
#
#                    Continuous Computing Corporation
#                    9450, Carroll Park Drive
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
#       Name:   LTE RRC
#
#       Type:   make file
#
#       Desc:   extract
#
#       File:   donh.mak.00
#  
#       Sid:    $SID$
#
#       Prg:    vk
#
#********************************************************************21*/
#
EXT_OPT=nh0011

NI_DIR=.

CHILD_LST=nh_brm.c nh_ccm.c nh_cgm.c nh_cpm.c nh_db.c nh_dbm.c nh_edm.c \
nh_ex_ms.c nh_ex_pt.c nh_id.c nh_init.c nh_lim.c nh_lmm.c nh_pim.c \
nh_ptli.c nh_ptmi.c nh_ptui.c nh_rhm.c nh_rtm.c nh_tst.c nh_uim.c nh_utl.c \
nh_3gdb.c

all: $(CHILD_LST)

clean:
	/bin/rm -f $(CHILD_LST)

nh_brm.c: ni_brm.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_brm.c nh_brm.c -d -l

nh_ccm.c: ni_ccm.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_ccm.c nh_ccm.c -d -l

nh_cgm.c: ni_cgm.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_cgm.c nh_cgm.c -d -l

nh_cpm.c: ni_cpm.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_cpm.c nh_cpm.c -d -l

nh_db.c: ni_db.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_db.c nh_db.c -d -l

nh_dbm.c: ni_dbm.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_dbm.c nh_dbm.c -d -l

nh_edm.c: ni_edm.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_edm.c nh_edm.c -d -l

nh_ex_ms.c: ni_ex_ms.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_ex_ms.c nh_ex_ms.c -d -l

nh_ex_pt.c: ni_ex_pt.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_ex_pt.c nh_ex_pt.c -d -l

nh_id.c: ni_id.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_id.c nh_id.c -d -l

nh_init.c: ni_init.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_init.c nh_init.c -d -l

nh_lim.c: ni_lim.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_lim.c nh_lim.c -d -l

nh_lmm.c: ni_lmm.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_lmm.c nh_lmm.c -d -l

nh_pim.c: ni_pim.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_pim.c nh_pim.c -d -l

nh_ptli.c: ni_ptli.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_ptli.c nh_ptli.c -d -l

nh_ptmi.c: ni_ptmi.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_ptmi.c nh_ptmi.c -d -l

nh_ptui.c: ni_ptui.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_ptui.c nh_ptui.c -d -l

nh_rhm.c: ni_rhm.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_rhm.c nh_rhm.c -d -l

nh_rtm.c: ni_rtm.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_rtm.c nh_rtm.c -d -l

nh_tst.c: ni_tst.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_tst.c nh_tst.c -d -l

nh_uim.c: ni_uim.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_uim.c nh_uim.c -d -l

nh_utl.c: ni_utl.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_utl.c nh_utl.c -d -l

nh_3gdb.c: ni_3gdb.c
	extprg $(EXT_OPT) $(NI_DIR)/ni_3gdb.c nh_3gdb.c -d -l

#/********************************************************************16**
#
#            (c) Copyright 2012 by RadiSys Corporation. All rights reserved.
#
#     This software is confidential and proprietary to RadiSys Corporation.
#     No part of this software may be reproduced, stored, transmitted, 
#     disclosed or used in any form or by any means other than as expressly
#     provided by the written Software License Agreement between Radisys 
#     and its licensee.
#
#     Radisys warrants that for a period, as provided by the written
#     Software License Agreement between Radisys and its licensee, this
#     software will perform substantially to Radisys specifications as
#     published at the time of shipment, exclusive of any updates or 
#     upgrades, and the media used for delivery of this software will be 
#     free from defects in materials and workmanship.  Radisys also warrants 
#     that has the corporate authority to enter into and perform under the 
#     Software License Agreement and it is the copyright owner of the software 
#     as originally delivered to its licensee.
#
#     RADISYS MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
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
#                       Restricted Rights Legend:
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
#     forth in the written Software License Agreement between Radisys and
#     its Licensee. Among other things, the use of this software
#     may be limited to a particular type of Designated Equipment, as 
#     defined in such Software License Agreement.
#     Before any installation, use or transfer of this software, please
#     consult the written Software License Agreement or contact Radisys at
#     the location set forth below in order to confirm that you are
#     engaging in a permissible use of the software.
#
#                    RadiSys Corporation
#                    Tel: +1 (858) 882 8800
#                    Fax: +1 (858) 777 3388
#                    Email: support@trillium.com
#                    Web: http://www.radisys.com 
# 
#*********************************************************************17*/

EXT_OPT=./sz0011.cfg

SQ_DIR=.

CHILD_LST=sz_db.c sz_ex_pt.c sz_li.c sz_mi.c sz_ndb.c sz_ptmi.c \
sz_sm.c sz_smlimme.c  sz_smuimme.c  sz_tst.c  sz_utils.c \
sz_ex_ms.c sz_id.c sz_mgmt.c  sz_msgfn.c  sz_ptli.c \
sz_ptui.c sz_smlienb.c sz_smuienb.c sz_tmr.c sz_ui.c

all: $(CHILD_LST)

clean:
	/bin/rm -f $(CHILD_LST)


sz_db.c: sq_db.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_db.c sz_db.c -d -l

sz_ex_pt.c: sq_ex_pt.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_ex_pt.c sz_ex_pt.c -d -l

sz_li.c: sq_li.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_li.c sz_li.c -d -l

sz_mi.c: sq_mi.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_mi.c sz_mi.c -d -l

sz_ndb.c: sq_ndb.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_ndb.c sz_ndb.c -d -l

sz_ptmi.c: sq_ptmi.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_ptmi.c sz_ptmi.c -d -l

sz_sm.c: sq_sm.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_sm.c sz_sm.c -d -l

sz_smlimme.c: sq_smlimme.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_smlimme.c sz_smlimme.c -d -l

sz_smuimme.c: sq_smuimme.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_smuimme.c sz_smuimme.c -d -l

sz_tst.c: sq_tst.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_tst.c sz_tst.c -d -l

sz_utils.c: sq_utils.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_utils.c sz_utils.c -d -l

sz_ex_ms.c: sq_ex_ms.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_ex_ms.c sz_ex_ms.c -d -l

sz_id.c: sq_id.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_id.c sz_id.c -d -l

sz_mgmt.c: sq_mgmt.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_mgmt.c sz_mgmt.c -d -l

sz_msgfn.c: sq_msgfn.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_msgfn.c sz_msgfn.c -d -l

sz_ptli.c: sq_ptli.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_ptli.c sz_ptli.c -d -l

sz_ptui.c: sq_ptui.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_ptui.c sz_ptui.c -d -l

sz_smlienb.c: sq_smlienb.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_smlienb.c sz_smlienb.c -d -l

sz_smuienb.c: sq_smuienb.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_smuienb.c sz_smuienb.c -d -l

sz_tmr.c: sq_tmr.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_tmr.c sz_tmr.c -d -l

sz_ui.c: sq_ui.c
	extprg $(EXT_OPT) $(SQ_DIR)/sq_ui.c sz_ui.c -d -l

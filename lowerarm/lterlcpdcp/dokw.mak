#********************************************************************16**
#
#       (c) COPYRIGHT 1989-2000 by Trillium Digital Systems, Inc.
#                         All rights reserved.
#
#    This software is confidential and proprietary to Trillium
#    Digital Systems, Inc.  No part of this software may be reproduced,
#    stored, transmitted, disclosed or used in any form or by any means
#    other than as expressly provided by the written license agreement
#    between Trillium and its licensee.
#
#    Trillium warrants that for a period, as provided by the written
#    license agreement between Trillium and its licensee, this
#    software will perform substantially to Trillium specifications as
#    published at the time of shipment and the media used for delivery
#    of this software will be free from defects in materials and
#    workmanship.
#
#    TRILLIUM MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
#    WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
#    A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE OR ANY RELATED
#    MATERIALS.
#
#    IN NO EVENT SHALL TRILLIUM BE LIABLE FOR ANY INDIRECT, SPECIAL,
#    OR CONSEQUENTIAL DAMAGES IN CONNECTION WITH OR ARISING OUT OF
#    THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
#    ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
#    LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
#    OF THE POSSIBILITY OF SUCH DAMAGE.
#
#                      Restricted Rights Legend
#
#    This software and all related materials licensed hereby are
#    classified as "restricted computer software" as defined in clause
#    52.227-19 of the Federal Acquisition Regulation ("FAR") and were
#    developed entirely at private expense for nongovernmental purposes,
#    are commercial in nature and have been regularly used for
#    nongovernmental purposes, and, to the extent not published and
#    copyrighted, are trade secrets and confidential and are provided
#    with all rights reserved under the copyright laws of the United
#    States.  The Government's rights to the software and related
#    materials are limited and restricted as provided in clause
#    52.227-19 of the FAR.
#
#
#                   IMPORTANT LIMITATION(S) ON USE
#
#    The use of this software is limited to the Use set
#    forth in the written License Agreement between Trillium and
#    its Licensee. Among other things, the Use of this software
#    may be limited to a particular type of Designated Equipment.
#    Before any installation, use or transfer of this software, please
#    consult the written License Agreement or contact Trillium at
#    the location set forth below in order to confirm that you are
#    engaging in a permissible Use of the software.
#
#                   Trillium Digital Systems, Inc.
#                 12100 Wilshire Boulevard, suite 1800
#                   Los Angeles, CA 90025-7118, USA
#
#                       Tel: +1 (310) 442-9222
#                       Fax: +1 (310) 442-1162
#
#                  Email: tech_support@trillium.com
#                    Web: http://www.trillium.com
#
#********************************************************************17*/
#
#********************************************************************20**
#
#       Name:   extract
#
#       Type:   make file
#
#       Desc:   extract
#
#       File:   dokw.mak
#  
#       Sid:      dokw.mak@@/main/2 - Fri Nov 13 14:13:20 2009
#
#       Prg:    kw
#
#********************************************************************21*/
#
#ifdef OPTION_00
EXT_OPT=kw0021
#endif /* OPTION_00 */

KW_DIR=.

CHILD_LST= kw_amm.c kw_cfg.c kw_dbm.c kw_lmm.c \
kw_tmm.c kw_tmr.c kw_uim.c kw_lim.c kw_umm.c kw_utl.c \
kw_ptmi.c kw_ptui.c kw_ex_ms.c


all: $(CHILD_LST)

clean:
	/bin/rm -f $(CHILD_LST)

kw_amm.c: $(KW_DIR)/gp_amm.c
	extprg $(EXT_OPT) $(KW_DIR)/gp_amm.c kw_amm.c -d -l
kw_cfg.c: $(KW_DIR)/gp_cfg.c
	extprg $(EXT_OPT) $(KW_DIR)/gp_cfg.c kw_cfg.c -d -l
kw_dbm.c: $(KW_DIR)/gp_dbm.c
	extprg $(EXT_OPT) $(KW_DIR)/gp_dbm.c kw_dbm.c -d -l
kw_lim.c: $(KW_DIR)/gp_lim.c
	extprg $(EXT_OPT) $(KW_DIR)/gp_lim.c kw_lim.c -d -l
kw_lmm.c: $(KW_DIR)/gp_lmm.c
	extprg $(EXT_OPT) $(KW_DIR)/gp_lmm.c kw_lmm.c -d -l
kw_tmm.c: $(KW_DIR)/gp_tmm.c
	extprg $(EXT_OPT) $(KW_DIR)/gp_tmm.c kw_tmm.c -d -l
kw_tmr.c: $(KW_DIR)/gp_tmr.c
	extprg $(EXT_OPT) $(KW_DIR)/gp_tmr.c kw_tmr.c -d -l
kw_uim.c: $(KW_DIR)/gp_uim.c
	extprg $(EXT_OPT) $(KW_DIR)/gp_uim.c kw_uim.c -d -l
kw_umm.c: $(KW_DIR)/gp_umm.c
	extprg $(EXT_OPT) $(KW_DIR)/gp_umm.c kw_umm.c -d -l
kw_utl.c: $(KW_DIR)/gp_utl.c
	extprg $(EXT_OPT) $(KW_DIR)/gp_utl.c kw_utl.c -d -l
kw_ptmi.c: $(KW_DIR)/gp_ptmi.c
	extprg $(EXT_OPT) $(KW_DIR)/gp_ptmi.c kw_ptmi.c -d -l
kw_ptui.c: $(KW_DIR)/gp_ptui.c
	extprg $(EXT_OPT) $(KW_DIR)/gp_ptui.c kw_ptui.c -d -l
kw_ex_ms.c: $(KW_DIR)/gp_ex_ms.c
	extprg $(EXT_OPT) $(KW_DIR)/gp_ex_ms.c kw_ex_ms.c -d -l

#
#********************************************************************30**
#  
#        End of file:     dokw.mak@@/main/2 - Fri Nov 13 14:13:20 2009
#    
#********************************************************************31*/
#
#********************************************************************40**
#  
#        Notes:
#  
#********************************************************************41*/
#
#********************************************************************50**
#
#********************************************************************51*/
#
#********************************************************************60**
#  
#       Revision history:
#  
#********************************************************************61*/
#  
#********************************************************************90**
#
#    ver       pat    init                  description
#----------- -------- ---- -----------------------------------------------
#/main/1      ---      gk   1. initial release.
#/main/2      ---      gk   1. LTE RLC Release 2.1
#********************************************************************91*/

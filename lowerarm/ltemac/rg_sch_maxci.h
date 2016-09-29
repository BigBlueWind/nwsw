/********************************************************************16**

                         (c) COPYRIGHT 2011 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9450, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@ccpu.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/**********************************************************************
 
    Name:   LTE MAC MAX C/I scheduler
 
    Type:   C include file
 
    Desc:   Defines required by MAX C/I scheduler
 
    File:   rg_sch_maxci.h
 
    Sid:      rg_sch_maxci.h@@/main/2 - Sat Jul 30 02:22:07 2011
 
    Prg:    apany
 
**********************************************************************/
 

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000372       LTE-MAC
*
*/


#ifndef __RGSCHMAXCIH__
#define __RGSCHMAXCIH__

/* OPTIMIZE rg001.201: This is to go to rg_env.h */
#define RG_SCH_MAXCIUL_NUM_CQI_LVLS 16 /* Num of queues is one less than this */
#define RG_SCH_MAXCIDL_NUM_CQI_LVLS 16 /* Num of queues is one less than this */

/* SR Grant */
#define RG_SCH_MAXCI_SR_BYTES 4
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependency on MIMO compile-time flag */
/* maximium priority levels,it varies from 0 to 15 */
#define RG_SCH_MAXCI_MIN_CQI_VAL    0
#define RG_SCH_MAXCI_MAX_CQI_VAL    15
#define RG_SCH_MAXCI_MAX_CWS_PERUE  2
/* ERROR CODES */
#define RGSCHERR_MAXCI_MEMERR          1   /* MEMORY ERROR */

/* MACROS */
#define RG_SCH_MAXCIUL_GET_UE(ue) (RgSchMaxciUlUe*)((RgSchCmnUe*)((ue)->sch))->ul.schSpfc
#define RG_SCH_MAXCIUL_GET_CELL(cell) (RgSchMaxciUlCellCb*)((RgSchCmnCell*)((cell)->sc.sch))->ul.schSpfc


#define RG_SCH_MAXCIDL_GET_CELL(_cell) (RgSchMaxciDlCell *)((RgSchCmnCell*)((_cell)->sc.sch))->dl.schSpfc

#define RG_SCH_MAXCIDL_GET_UE(_ue) (RgSchMaxciDlUe *)((RgSchCmnUe *)((_ue)->sch))->dl.schSpfc
#define RG_SCH_MAXCIDL_GET_HQP(hqP) (RgSchMaxciDlHqProc *)((RgSchCmnDlHqProc *)((hqP)->sch))->schSpfc

#define RG_SCH_MAXCIDL_GET_LC(_lc) (RgSchMaxciDlLc *)((RgSchCmnDlSvc *)((_lc)->sch))->schSpfc
#define RG_SCH_MAXCIDL_IS_SRB(_lc) (_lc->lcId  == 1 || _lc->lcId == 2)
#define RG_SCH_MAXCIDL_INIT_LNK(_lstLnk, _node)\
{\
      (_lstLnk)->node = (PTR)_node;\
      (_lstLnk)->prev = NULLP;\
      (_lstLnk)->next = NULLP;\
}

#endif /* __RGSCHMAXCIH__ */


/**********************************************************************
         End of file:     rg_sch_maxci.h@@/main/2 - Sat Jul 30 02:22:07 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      apany  1. LTE MAC 2.1 release
             rg001.201 sm 1. Removed extra comments. 
           rg007.201   ap  1. Added support for MIMO
           rg008.201   sd  1.Removed dependency on MIMO compile-time flag
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
*********************************************************************91*/

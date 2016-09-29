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
 
    Name:   LTE MAC PFS scheduler
 
    Type:   C include file
 
    Desc:   Defines required by PFS scheduler
 
    File:   rg_sch_pfs.h
 
    Sid:      rg_sch_pfs.h@@/main/2 - Sat Jul 30 02:22:08 2011
 
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


#ifndef __RGSCHPFSH__
#define __RGSCHPFSH__

/* To be in rg_env.h */
#define RG_SCH_PFS_UL_NUM_CQI_LVLS    16
/* MS_WORKAROUND : syed Increasing the number of 
 * Queues for better distribution of UEs */
#define RG_SCH_PFS_DL_NUM_GBR_QUEUES  8
#define RG_SCH_PFS_DL_NUM_MBR_QUEUES  4
#define RG_SCH_PFS_DL_NUM_AMBR_QUEUES 64 
#define RG_SCH_PFS_UL_NUM_QUEUES      64


#define RG_SCH_PFS_DL_NUM_CQI_LVLS 16

#define RG_SCH_PFS_LC_TYPE_SRB  0
#define RG_SCH_PFS_LC_TYPE_DRB0 1
#define RG_SCH_PFS_LC_TYPE_GBR  2
#define RG_SCH_PFS_LC_TYPE_AMBR 3

/* There is 'RGR_PFS_FRAC_PRIO_LVLS' which is common for both
 * uplink and downlink, and defines the number of priority
 * levels for rem bitrate priority */

#define RG_SCH_PFS_MAX_REMRATE_PRIO_VAL    (RGR_PFS_FRAC_PRIO_LVLS-1)


#define RG_SCH_PFS_GET_PFSCELLDL(cell) \
   (RgSchPfsDlCell *)((RgSchCmnCell *)cell->sc.sch)->dl.schSpfc
#define RG_SCH_PFS_GET_PFSCELLUL(cell) \
   (RgSchPfsUlCell *)((RgSchCmnCell *)cell->sc.sch)->ul.schSpfc
#define RG_SCH_PFS_GET_PFSUEDL(ue) \
   (RgSchPfsDlUe *)((RgSchCmnUe *)ue->sch)->dl.schSpfc
#define RG_SCH_PFS_GET_PFSUEUL(ue) \
   (RgSchPfsUlUe *)((RgSchCmnUe *)ue->sch)->ul.schSpfc
#define RG_SCH_PFS_GET_PFSPROCDL(proc) \
   (RgSchPfsDlHqProc *)((RgSchCmnDlHqProc *)proc->sch)->schSpfc
#define RG_SCH_PFS_GET_PFSPROCUL(proc) \
   (RgSchPfsUlHqProc *)((RgSchCmnUlHqProc *)proc->sch)->schSpfc
#define RG_SCH_PFS_GET_PFSDLLC(lc) \
   (RgSchPfsDlLc *)((RgSchCmnDlSvc *)lc->sch)->schSpfc


#endif /* __RGSCHPFSH__ */


/**********************************************************************
         End of file:     rg_sch_pfs.h@@/main/2 - Sat Jul 30 02:22:08 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      apany  1. LTE MAC 2.1 release
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
*********************************************************************91*/

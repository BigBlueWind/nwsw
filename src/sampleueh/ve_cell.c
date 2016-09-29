/********************************************************************16**

                         (c) COPYRIGHT 2009 by 
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

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/**********************************************************************

     Name:     LTE-RRC User
  
     Type:     C Source file
  
     Desc:     Cell Configuration

     Ret :     ROK - success
               RFAILED - failure

     File:     ve_cell.c

     Sid:      ve_cell.c@@/main/2 - Wed Dec  1 08:44:22 2010

     Prg:      

**********************************************************************/

#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_inet.h"
#include "cm_tpt.h"

#include "nhu_asn.h"
#include "nhu.h"           /* NHU interface defines */
#include "pju.h"           /* PDCP data interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"           /* PHY RRM control Interface */
#include "szt.h"           /* S1AP RRM control Interface */
#ifdef EU_DAT_APP
#include "egt.h"           /* S1AP RRM control Interface */
#endif 
#include "lrg.h"           /* MAC RRM control Interface */
#include "ve_eut.h" 

#ifdef LTE_HO_SUPPORT
#include "czt.h"
#include "czt_asn.h"
#endif

/* RRM related includes */
#include "ve.h" 
#include "lve.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_lib.x"
#include "cm_inet.x"
#include "cm_tpt.x"

#include "nhu_asn.x"
#include "nhu.x"           /* NHU Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "ctf.x"           /* PHY RRM control Interface */
#include "szt.x"           /* S1AP RRM control Interface */

#ifdef LTE_HO_SUPPORT
#include "czt.x"
#include "czt_asn.x"
#endif

#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "lrg.x"           /* MAC RRM control Interface */

/* RRM related includes */
#include "lve.x" 
#include "ve_eut.x" 
#include "ve.x" 
/* MSPD changes */
#ifndef MSPD
#include "ve_dflt.h"
#include "ve_dflt_pal.x"
#else
#include "ve_dflt_ms.x"
#endif

#ifdef VE_DONGWON_OAM
U32 enumMapNhuSysBw[]=
{/*6,15,25,50,75,100*/

   NHU_SYS_BW_N6,
   NHU_SYS_BW_N15,
   NHU_SYS_BW_N25,
   NHU_SYS_BW_N50,
   NHU_SYS_BW_N75,
   NHU_SYS_BW_N100

};
#endif /* VE_DONGWON_OAM */ 
EXTERN S16 VeLiNhuCellCfgReq  ARGS(( Pst *pst, SpId spId, NhuCellCfgReqSdus *nhuCellCfgReqSdus ));
EXTERN S16 VeLiRgrCfgReq ARGS(( Pst* pst, SpId spId, RgrCfgTransId transId, RgrCfgReqInfo * cfgReqInfo));

/***********************************************************************
                    NHU related functions
************************************************************************/

/*
*
*       Fun:   veFillCellRbCfg
*
*       Desc:  This function Cell RB Configuration 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_cell.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillCellRbCfg
(
NhuRbCfgList  **rbCfgList
)
#else
PUBLIC S16 veFillCellRbCfg(rbCfgList)
NhuRbCfgList  **rbCfgList;
#endif
{
   NhuRbCfgList *lclRbCfgList = NULLP;
   U8 dlrlcCfgType;
   U8 ulrlcCfgType;

   TRC2(veFillCellRbCfg);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillCellRbCfg:\
            Configuring Cell RBs\n"));
#endif

   lclRbCfgList = *rbCfgList;

   /* Filling RB for BCCH (SIB1) to DL-SCH Mapping */
   {
      lclRbCfgList->rbCfgInfoList[0].rbId.rbId =  VE_BCCH_SIB1_ID;
      /* Bit Mask (UL, DL Cfg present) */ 
      lclRbCfgList->rbCfgInfoList[0].rbCfgBitMsk = NHU_RB_CFG_DL;

      /* DlRbCfg */
      {
         lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlRbBitMsk =
            (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);

         /* dlLogChMapInfo */
         {
            /* Logical Channel ID and type */
            lclRbCfgList->rbCfgInfoList[0].lcId.lcId = 
               VE_BCCH_SIB1_ID;
            lclRbCfgList->rbCfgInfoList[0].lcId.logChType = 
               CM_LTE_LCH_BCCH;
            /* Transport Channel Type */
            lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlLogChMap.trChType = 
               CM_LTE_TRCH_DL_SCH;
         }
         /* DL-RLC mode */
         {
            dlrlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlRlcMode.rlcCfgType =
               dlrlcCfgType;
            lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlRlcMode.mode = \
                                                                    NHU_RLC_MODE_TM;
            /* set lclRbCfgList->rbCfgInfoList[3].dlRbCfg.dlRlcMode.mode HERE */
            veFillDlRlcModeInfo(
                  &(lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlRlcMode), VE_BCCH_SIB1_ID);
         }
      }
   }

   /* Filling RB for PCCH to PCH  Mapping */
   {
      lclRbCfgList->rbCfgInfoList[1].rbId.rbId = VE_PCCH_ID;
      /* Bit Mask (UL, DL Cfg present) */ 
      lclRbCfgList->rbCfgInfoList[1].rbCfgBitMsk = NHU_RB_CFG_DL;

      /* DlRbCfg */
      {
         lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlRbBitMsk 
            = (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);

         /* dlLogChMapInfo */
         {
            /* Logical Channel ID and type */
            lclRbCfgList->rbCfgInfoList[1].lcId.lcId = 
               VE_PCCH_ID; 
            lclRbCfgList->rbCfgInfoList[1].lcId.logChType = 
               CM_LTE_LCH_PCCH;
            /* Transport Channel Type */
            lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlLogChMap.trChType =
               CM_LTE_TRCH_PCH;
         }
         /* DL-RLC mode */
         {
            dlrlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlRlcMode.rlcCfgType =
               NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlRlcMode.mode =\
                                                                   NHU_RLC_MODE_TM;
            /* set lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlRlcMode.mode HERE */
            veFillDlRlcModeInfo(
                  &(lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlRlcMode), VE_PCCH_ID);
         }
      }
   }

   /* Filling RB for BCCH to BCH Mapping */
   {
      lclRbCfgList->rbCfgInfoList[2].rbId.rbId =  VE_BCCH_BCH_ID;
      /* Bit Mask (UL, DL Cfg present) */ 
      lclRbCfgList->rbCfgInfoList[2].rbCfgBitMsk = NHU_RB_CFG_DL;

      /* DlRbCfg */
      {
         lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlRbBitMsk = 
            (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);

         /* dlLogChMapInfo */
         {
            /* Logical Channel ID and type */
            lclRbCfgList->rbCfgInfoList[2].lcId.lcId = 
               VE_BCCH_BCH_ID; 
            lclRbCfgList->rbCfgInfoList[2].lcId.logChType = 
               CM_LTE_LCH_BCCH;
            /* Transport Channel Type */
            lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlLogChMap.trChType =
               CM_LTE_TRCH_BCH;
         }
         /* DL-RLC mode */
         {
            dlrlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlRlcMode.rlcCfgType = 
               dlrlcCfgType;
            lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlRlcMode.mode \
               = NHU_RLC_MODE_TM;
            /* set lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlRlcMode.mode HERE */
            veFillDlRlcModeInfo(
                  &(lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlRlcMode), VE_BCCH_BCH_ID);
         }
      }
   }

   /* Filling RB for CCCH to UL-SCH Mapping */
   {
      lclRbCfgList->rbCfgInfoList[3].rbId.rbId =  VE_CCCH_UL_ID;
      /* Bit Mask (UL, DL Cfg present) */ 
      lclRbCfgList->rbCfgInfoList[3].rbCfgBitMsk = NHU_RB_CFG_UL;

      /* UlRbCfg */
      {
         /* Bit mask calculate properly */
         lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulRbBitMsk = 
            (NHU_RB_CFG_LOGCH_MAP_UL | NHU_RB_CFG_RLC_MODE_UL);

         /* ulLogChMapInfo */
         {
            /* Transport Channel type */
            lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulLogChMap.trChType\
               = CM_LTE_TRCH_UL_SCH;

            /* Logical Channel Id and Type */
            lclRbCfgList->rbCfgInfoList[3].lcId.lcId =
               VE_CCCH_UL_ID; 
            lclRbCfgList->rbCfgInfoList[3].lcId.logChType = 
               CM_LTE_LCH_CCCH;

            /* Fill Logical Channel Cfg */
            veFillUlLogChCfg(
                  &(lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulLogChMap), VE_CCCH_UL_ID);
         }
         /* ulRlcMode */
         {
            ulrlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulRlcMode.rlcCfgType = 
               ulrlcCfgType;
            lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulRlcMode.mode \
               = NHU_RLC_MODE_TM;
            /* set lclRbCfgList->rbCfgInfoList[0].ulRbCfg.ulRlcMode.mode HERE */
            veFillUlRlcModeInfo(
                  &(lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulRlcMode), VE_CCCH_UL_ID);
         } /* ulRlcMode */
      }
   }

   /* Filling RB for CCCH to DL-SCH Mapping */
   {
      lclRbCfgList->rbCfgInfoList[4].rbId.rbId =  VE_CCCH_DL_ID;
      /* Bit Mask (UL, DL Cfg present) */ 
      lclRbCfgList->rbCfgInfoList[4].rbCfgBitMsk = NHU_RB_CFG_DL;

      /* DlRbCfg */
      {
         lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlRbBitMsk |= 
            (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);

         /* dlLogChMapInfo */
         {
            /* Logical Channel ID and type */
            lclRbCfgList->rbCfgInfoList[4].lcId.lcId = 
               VE_CCCH_DL_ID; 
            lclRbCfgList->rbCfgInfoList[4].lcId.logChType = 
               CM_LTE_LCH_CCCH;
            /* Transport Channel Type */
            lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlLogChMap.trChType = 
               CM_LTE_TRCH_DL_SCH;
         }
         /* DL-RLC mode */
         {
            dlrlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlRlcMode.rlcCfgType = 
               dlrlcCfgType;
            lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlRlcMode.mode\
               = NHU_RLC_MODE_TM;
            /* set lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlRlcMode.mode */
            veFillDlRlcModeInfo(
                  &(lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlRlcMode), VE_CCCH_DL_ID);
         }
      }
   }

   /* Filling RB for BCCH (non-SIB1) to DL-SCH Mapping */
   {
      lclRbCfgList->rbCfgInfoList[5].rbId.rbId =  VE_BCCH_SIB_OTHERS_ID;
      /* Bit Mask (UL, DL Cfg present) */ 
      lclRbCfgList->rbCfgInfoList[5].rbCfgBitMsk = NHU_RB_CFG_DL;

      /* DlRbCfg */
      {
         lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlRbBitMsk =
            (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);

         /* dlLogChMapInfo */
         {
            /* Logical Channel ID and type */
            lclRbCfgList->rbCfgInfoList[5].lcId.lcId = 
               VE_BCCH_SIB_OTHERS_ID; 
            lclRbCfgList->rbCfgInfoList[5].lcId.logChType = 
               CM_LTE_LCH_BCCH;
            /* Transport Channel Type */
            lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlLogChMap.trChType = 
               CM_LTE_TRCH_DL_SCH;
         }
         /* DL-RLC mode */
         {
            dlrlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlRlcMode.rlcCfgType =
               dlrlcCfgType;
            lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlRlcMode.mode = \
                                                                    NHU_RLC_MODE_TM;
            /* set lclRbCfgList->rbCfgInfoList[3].dlRbCfg.dlRlcMode.mode HERE */
            veFillDlRlcModeInfo(
                  &(lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlRlcMode), (VE_BCCH_SIB_OTHERS_ID));
         }
      }
   }

   RETVALUE(ROK);
} /* End of veFillCellRbCfg */


/*
*
*       Fun:   veSndNhuCellCfgReq 
*
*       Desc:  Trigger NHU Cell Cfg Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_cell.c 
*
*/
#ifdef ANSI
PUBLIC S16 veSndNhuCellCfgReq
(
U32  cellId
)
#else
PUBLIC S16 veSndNhuCellCfgReq(cellId)
U32  cellId;
#endif
{
   Pst               *pst = NULLP;
   NhuCellCfgReqSdus *nhuCellCfgReqSdus = NULLP;
   NhuRbCfgList      *rbCfgList = NULLP;
   NhuRachCfg        *rachCfg = NULLP;

   TRC2(veSndNhuCellCfgReq);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veSndNhuCellCfgReq:\
            Triggering Cell Cfg Req over the NHU Interface\n"));
#endif

   /* Alloc event structure */
   VE_ALLOCEVNT(&nhuCellCfgReqSdus, sizeof(NhuCellCfgReqSdus));
   /* Copying the pst structure */
   pst = &(veCb.nhuSap[0]->pst);
   pst->event = EVTNHUCELLCFGREQ;

   /* Filling Header */
   nhuCellCfgReqSdus->hdr.cellId = (U16)cellId;
#ifdef VE_DONGWON_OAM
      /*OAM_FIX : Replaced the hardcoded values with the default variables*/
   nhuCellCfgReqSdus->hdr.ueId = veDfltUeIdUsage;
#else
   nhuCellCfgReqSdus->hdr.ueId = VE_VAL_ZERO;
#endif
   nhuCellCfgReqSdus->hdr.transId = VE_VAL_ONE;

   nhuCellCfgReqSdus->sdu.cfgType = NHU_CFGTYPE_SETUP;
   /* Cyclic Prefix - TRUE indicates NORMAL */
   /*nhuCellCfgReqSdus->sdu.u.cfgInfo.duplexMode = NHU_MODE_TDD;*/
   nhuCellCfgReqSdus->sdu.u.cfgInfo.duplexMode =
         (NhuDuplexMode) ((veCb.cellCb[0]->duplexMode == VE_MODE_FDD) ?
           VE_MODE_FDD : VE_MODE_TDD);
#ifndef VE_DONGWON_OAM
#ifdef YS_PICO
   nhuCellCfgReqSdus->sdu.u.cfgInfo.sysBwCfg.ulSysBw = veDfltULTotalBw;
   nhuCellCfgReqSdus->sdu.u.cfgInfo.sysBwCfg.dlSysBw = veDfltDLTotalBw;
#else/* g++ compilation fix */
   nhuCellCfgReqSdus->sdu.u.cfgInfo.sysBwCfg.ulSysBw = (NhuSysBw)veDfltULTotalBw;
   nhuCellCfgReqSdus->sdu.u.cfgInfo.sysBwCfg.dlSysBw = (NhuSysBw)veDfltDLTotalBw;
#endif
#else
        /*OAM_FIX : Mapped the enum to its corresponding value*/
#ifdef YS_PICO
   nhuCellCfgReqSdus->sdu.u.cfgInfo.sysBwCfg.ulSysBw = enumMapNhuSysBw[veDfltULTotalBw];
   nhuCellCfgReqSdus->sdu.u.cfgInfo.sysBwCfg.dlSysBw = enumMapNhuSysBw[veDfltDLTotalBw];
#else/* g++ compilation fix */
   nhuCellCfgReqSdus->sdu.u.cfgInfo.sysBwCfg.ulSysBw = enumMapNhuSysBw[veDfltULTotalBw];
   nhuCellCfgReqSdus->sdu.u.cfgInfo.sysBwCfg.dlSysBw = enumMapNhuSysBw[veDfltDLTotalBw];
#endif
#endif
   /* cellCfgBitMask - to indicate presence of Cell Config Info */
   nhuCellCfgReqSdus->sdu.u.cfgInfo.cellCfgBitMsk = (NHU_CELL_CFG_RB |\
         NHU_CELL_CFG_MAC_CMN);

   /* 
    * Fill RB Configuration 
    */
   rbCfgList = &(nhuCellCfgReqSdus->sdu.u.cfgInfo.rbToAddCfgList);
   /* 5 cellRbs are configured by default */
#ifdef VE_DONGWON_OAM
      /*OAM_FIX : Replaced the hardcoded values with the default variables*/
   rbCfgList->numOfRbCfgInfo = veDfltNumOfRbCfgInfo;;    
#else
   rbCfgList->numOfRbCfgInfo = VE_CELL_CFG_DFLT_RBS;    
#endif
   VE_GET_MEM(nhuCellCfgReqSdus, sizeof(NhuRbCfgInfo)*rbCfgList->numOfRbCfgInfo,
         &(rbCfgList->rbCfgInfoList));
   /* filling RB Cfg. maapping */
   veFillCellRbCfg(&rbCfgList);

   /* 
    * MAC common configuration 
    * Fill RACH Configuration 
    */
   rachCfg = &(nhuCellCfgReqSdus->sdu.u.cfgInfo.macCmnCfg.rachCfg);
   rachCfg->maxMsg3Tx = (U8)veDfltMaxHARQMsg3Tx;/* klock warning fixed */

   if(ROK != VeLiNhuCellCfgReq(pst, veCb.nhuSap[0]->spId, nhuCellCfgReqSdus))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiNhuCellCfgReq failed.\n"));      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndNhuCellCfgReq */


/***********************************************************************
                    CTF related functions
************************************************************************/
/*
*
*       Fun:   veSndCtfCellCfgReq 
*
*       Desc:  Trigger Cell Cfg 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_cell.c 
*
*/
#ifdef ANSI
PUBLIC S16 veSndCtfCellCfgReq
(
U32  cellId
)
#else
PUBLIC S16 veSndCtfCellCfgReq(cellId)
U32  cellId;
#endif
{
   CtfCfgReqInfo    *cellCfgReq = NULLP;
   Pst              *pst = NULLP;
   CtfCfgTransId    transId;

#ifdef YS_PICO
   VePicoSpecific picoCellCfgReq;
#endif

#ifdef MSPD
   VeMspdSpecific mspdCellCfgReq;
#endif

   TRC2(veSndCtfCellCfgReq);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veSndCtfCellCfgReq:\
            Trigger Cell Cfg Req. Over CTF Interface \n"));
#endif

   /* Copying the pst structure */
   pst = &veCb.ctfSap[0]->pst;
   pst->event   = EVTCTFCFGREQ;

   VE_ALLOC(&cellCfgReq, sizeof(CtfCfgReqInfo));
   /* klock warning fixed */
   if(cellCfgReq == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndCtfCellCfgReq:\
               Failure in the allocation of configuration info\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

 
#ifdef CTF_VER3
#ifdef VE_PICO
   VE_ALLOC(&cellCfgReq->vendorParams.paramBuffer, sizeof(VePicoSpecific));
   if(cellCfgReq->vendorParams.paramBuffer == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndCtfCellCfgReq:\
               Failure in the allocation\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */
   cmMemset((U8 *)&picoCellCfgReq, 0, sizeof(VePicoSpecific));
#endif

#ifdef MSPD
   VE_ALLOC(&cellCfgReq->vendorParams.paramBuffer, sizeof(mspdCellCfgReq));
   if(cellCfgReq->vendorParams.paramBuffer == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndCtfCellCfgReq:\
               Failure in the allocation\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */
#endif  
#endif  

   cmMemset((U8 *)&transId, '\0', sizeof(CtfCfgTransId));

   cellCfgReq->cfgType = CTF_CONFIG;
   cellCfgReq->u.cfg.cfgElem = CTF_CELL_CFG;
   cellCfgReq->u.cfg.u.cellCfg.cellId = (U16)cellId; /* klock warning fixed */
   cellCfgReq->u.cfg.u.cellCfg.cellIdGrpId = VE_VAL_ZERO;
/* MSPD changes */
#ifdef MSPD
   /* AGHOSH : changed cellCfgReq->u.cfg.u.cellCfg.physCellId = cellId; */
   cellCfgReq->u.cfg.u.cellCfg.physCellId = VE_VAL_ZERO;
#else
#ifdef VE_PICO
   picoCellCfgReq.phyCellIdPres = PRSNT_NODEF;
   picoCellCfgReq.bwCfgPres = PRSNT_NODEF;
   picoCellCfgReq.txCfgPres = PRSNT_NODEF;
#endif
   cellCfgReq->u.cfg.u.cellCfg.physCellId = cellId;
#endif
   /* Bandwidth Config */
   cellCfgReq->u.cfg.u.cellCfg.bwCfg.pres = PRSNT_NODEF ;
   cellCfgReq->u.cfg.u.cellCfg.bwCfg.dlBw = veDfltCellDlBW;
   cellCfgReq->u.cfg.u.cellCfg.bwCfg.ulBw = veDfltCellUlBW;
   cellCfgReq->u.cfg.u.cellCfg.bwCfg.eUtraBand \
      = veCb.cellCb[0]->sysRelInfo.freqBandInd;

   /* Basic transmission scheme */
   cellCfgReq->u.cfg.u.cellCfg.txCfg.pres = PRSNT_NODEF ;
   cellCfgReq->u.cfg.u.cellCfg.txCfg.duplexMode = 
      (CtfDuplexMode)((veCb.cellCb[0]->duplexMode == VE_MODE_FDD) ? 
      VE_MODE_FDD : VE_MODE_TDD);
   cellCfgReq->u.cfg.u.cellCfg.txCfg.scSpacing = veDfltScSpacing;
   cellCfgReq->u.cfg.u.cellCfg.txCfg.cycPfx = veDfltCycPfx;
   /* Antenna config */ 
/* MSPD changes */
#ifdef MSPD
   /* AGHOSH : changed cellCfgReq->u.cfg.u.cellCfg.antennaCfg.antPortsCnt = CTF_AP_CNT_2; */
   /* MS_FIX : set antenna ports correctly */
   cellCfgReq->u.cfg.u.cellCfg.antennaCfg.antPortsCnt = veDfltCtfAntPort;
#else
#ifndef VE_DONGWON_OAM
   cellCfgReq->u.cfg.u.cellCfg.antennaCfg.antPortsCnt = CTF_AP_CNT_2;
#else
      /*OAM_FIX : Replaced the hardcoded values with the default variables*/
   switch(veDfltNumOfTxAnt)
   {
        case NhuAntennaInfoCmnantennaPortsCountan1Enum:
                cellCfgReq->u.cfg.u.cellCfg.antennaCfg.antPortsCnt = 1;
                break;
        case NhuAntennaInfoCmnantennaPortsCountan2Enum:
                cellCfgReq->u.cfg.u.cellCfg.antennaCfg.antPortsCnt = 2;
                break;
        case NhuAntennaInfoCmnantennaPortsCountan4Enum:
                cellCfgReq->u.cfg.u.cellCfg.antennaCfg.antPortsCnt = 4;
                break;
   }
#endif
#endif
   /* PRACH config */
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.rootSequenceIndex \
      = veDfltRootSequenceIdx;
   /* klock warning fixed */
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.prachCfgIndex = (U8)veDfltPrachCfgIdx;
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.zeroCorrelationZoneCfg \
      = (U8)veDfltZeroCorrelZoneCfg;
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.highSpeedFlag = (Bool)veDfltHighSpeedFlag;
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.prachFreqOffset \
      = (U8)veDfltPrachFreqOffset;
   /* PDSCH config */ /* klock warning fixed */
   cellCfgReq->u.cfg.u.cellCfg.pdschCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.pdschCfg.refSigPwr = (S8)veDfltRefSignalPower;
   cellCfgReq->u.cfg.u.cellCfg.pdschCfg.p_b = (U8)veDfltPDSCHCfgPB;
   cellCfgReq->u.cfg.u.cellCfg.priSigPwr= (S16)veDfltPriSignalPower;
   cellCfgReq->u.cfg.u.cellCfg.secSigPwr = (S16)veDfltSecSignalPower;
   /* PUSCH config */
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.pres = PRSNT_NODEF;

   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschBasicCfg.noOfsubBands \
      = (U8)veDfltNSB;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschBasicCfg.hoppingMode \
      = veDfltHoppingMode;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschBasicCfg.hoppingOffset\
      = (U8)veDfltPuschHoppingOff;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschBasicCfg.enable64QAM\
      = (Bool)veDfltEnable64QAM;

   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschUlRS.grpHopEnabled\
      = (Bool)veDfltGroupHoppingEnabled;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschUlRS.seqHopEnabled\
      = (Bool)veDfltSeqHoppingEnabled;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschUlRS.grpNum\
      = (U8)veDfltGroupAssignPUSCH;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschUlRS.cycShift\
      = (U8)veDfltCyclicShift;
   /* PHICH config */
   cellCfgReq->u.cfg.u.cellCfg.phichCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.phichCfg.duration = veDfltPhichDur;
   cellCfgReq->u.cfg.u.cellCfg.phichCfg.resource = veDfltPhichRes;
   /* PUCCH config */
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.pres = PRSNT_NODEF;
   /* klock warning fixed */
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.nRB = (U8)veDfltPUCCHnRBCQI;
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.nCS = (U8)veDfltPUCCHnCSAN;
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.n1PUCCH = veDfltNlPUCCHAn;
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.deltaShift = veDfltPUCCHDeltaShift;
   /* SRS UL config, setup case */
   cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.srsSetup.srsBw = 0 ;
   cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.srsSetup.sfCfg = 15;
   cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.srsSetup.srsANSimultTx = 0;
#ifdef LTE_TDD
   cellCfgReq->u.cfg.u.cellCfg.tddSfCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.tddSfCfg.sfAssignment \
      = veDfltSubframeAssignment;
   cellCfgReq->u.cfg.u.cellCfg.tddSfCfg.spclSfPatterns \
      = veDfltSpecialSubfrmPatterns;
#else /* LTE_TDD */
   cellCfgReq->u.cfg.u.cellCfg.tddSfCfg.pres = NOTPRSNT;
#endif /* LTE_TDD */

   /* Make changes to take out the parameters which were guarded 
    * under YS_PICO flag for mindspeed */               
#ifdef YS_PICO

   picoCellCfgReq.syncSigPowOsPres = PRSNT_NODEF;
   picoCellCfgReq.syncSigPowOs = 6000;
    
   picoCellCfgReq.cfiPowOsPres = PRSNT_NODEF;
   picoCellCfgReq.cfiPowOs = 6000;

   picoCellCfgReq.dciPowOsPres = PRSNT_NODEF;
   picoCellCfgReq.dciPowOs = 6000;

   picoCellCfgReq.extWinMarginPres = PRSNT_NODEF;
   picoCellCfgReq.extWinMargin = 5;

   picoCellCfgReq.pucchNoiseGammaPres = PRSNT_NODEF;
   picoCellCfgReq.pucchNoiseGamma = 0x2000;

   picoCellCfgReq.prachPkRatio4Pres = PRSNT_NODEF;
   picoCellCfgReq.prachPkRatio4 = 0x24eb;

   picoCellCfgReq.prachPkRatio0Pres = PRSNT_NODEF;
   picoCellCfgReq.prachPkRatio0 = 0x38cc;

   picoCellCfgReq.srsDopEstFactorPres = PRSNT_NODEF;
   picoCellCfgReq.srsDopEstFactor = 0xffff;

   picoCellCfgReq.puschProbDtxAckPres = PRSNT_NODEF;
   picoCellCfgReq.puschProbDtxAck = 22;

   picoCellCfgReq.pucchProbDtxAckPres = PRSNT_NODEF;
   picoCellCfgReq.pucchProbDtxAck = 25;

   picoCellCfgReq.txAntennaPortsPres = PRSNT_NODEF;
   picoCellCfgReq.txAntennaPorts = 1;

   picoCellCfgReq.rxAntennaPortsPres = PRSNT_NODEF;
   picoCellCfgReq.rxAntennaPorts = 2;

   picoCellCfgReq.phySyncModePres= PRSNT_NODEF;
   picoCellCfgReq.phySyncMode = 0x02;

   picoCellCfgReq.dataRepModePres = PRSNT_NODEF;
   picoCellCfgReq.dataRepMode = 0x00;

   picoCellCfgReq.rachSrRepModePres = PRSNT_NODEF;
   picoCellCfgReq.rachSrRepMode = 0;
#ifdef CTF_VER3
   cellCfgReq->vendorParams.buffLen = sizeof(picoCellCfgReq);
   cmMemcpy((U8*)(cellCfgReq->vendorParams.paramBuffer),(U8*)&picoCellCfgReq,sizeof(VePicoSpecific));
#endif
#else
   cellCfgReq->u.cfg.u.cellCfg.syncSigPowOsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.syncSigPowOs = 6000;

   cellCfgReq->u.cfg.u.cellCfg.cfiPowOsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.cfiPowOs = 6000;
   
   cellCfgReq->u.cfg.u.cellCfg.dciPowOsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.dciPowOs = 6000;

   cellCfgReq->u.cfg.u.cellCfg.extWinMarginPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.extWinMargin = 5;

   cellCfgReq->u.cfg.u.cellCfg.pucchNoiseGammaPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.pucchNoiseGamma = 0x2000;

   cellCfgReq->u.cfg.u.cellCfg.prachPkRatio4Pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.prachPkRatio4 = 0x24eb;

   cellCfgReq->u.cfg.u.cellCfg.prachPkRatio0Pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.prachPkRatio0 = 0x38cc;

   cellCfgReq->u.cfg.u.cellCfg.srsDopEstFactorPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.srsDopEstFactor = 0xffff;

   cellCfgReq->u.cfg.u.cellCfg.puschProbDtxAckPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.puschProbDtxAck = 22;

   cellCfgReq->u.cfg.u.cellCfg.pucchProbDtxAckPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.pucchProbDtxAck = 25;

/* MS_FIX */
   cellCfgReq->u.cfg.u.cellCfg.txAntennaPortsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.txAntennaPorts = veDfltCtfAntPort;


   cellCfgReq->u.cfg.u.cellCfg.rxAntennaPortsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.rxAntennaPorts = 2;

   cellCfgReq->u.cfg.u.cellCfg.phySyncModePres= PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.phySyncMode = 0x02;

   cellCfgReq->u.cfg.u.cellCfg.dataRepModePres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.dataRepMode = 0x00;

   cellCfgReq->u.cfg.u.cellCfg.rachSrRepModePres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.rachSrRepMode = 0 ;
#endif

   /* PHY configuration parameters *//* add for MSPD structure */
#ifdef MSPD
#ifdef CTF_VER3
   cmMemset((U8*)&(mspdCellCfgReq),0,sizeof(VeMspdSpecific));
   mspdCellCfgReq.opMode  = veCb.cellCb[0]->opMode;
   mspdCellCfgReq.counter = veCb.cellCb[0]->counter;
   mspdCellCfgReq.period  = veCb.cellCb[0]->period;
   cellCfgReq->vendorParams.buffLen = sizeof(mspdCellCfgReq);
   cmMemcpy((U8*)(cellCfgReq->vendorParams.paramBuffer),(U8*)&mspdCellCfgReq,sizeof(VeMspdSpecific));
#else
   cellCfgReq->u.cfg.u.cellCfg.opMode  = veCb.cellCb[0]->opMode;
   cellCfgReq->u.cfg.u.cellCfg.counter = veCb.cellCb[0]->counter;
   cellCfgReq->u.cfg.u.cellCfg.period  = veCb.cellCb[0]->period;
#endif   
#endif

   if(ROK != VeLiCtfCfgReq(pst, veCb.ctfSap[0]->spId, transId,  cellCfgReq))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCtfCfgReq failed.\n"));      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndCtfCellCfgReq */


/***********************************************************************
                    RGR related functions
************************************************************************/


/*
*
*       Fun:   veFillRgrDlHqCfg 
*
*       Desc:  Populating Dl Harq Cfg
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_cell.c 
*
*/
#ifdef ANSI
PUBLIC S16 veFillRgrDlHqCfg
(
RgrDlHqCfg *cfg
)
#else
PUBLIC S16 veFillRgrDlHqCfg(cfg)
RgrDlHqCfg *cfg;
#endif
{
   TRC2(veFillRgrDlHqCfg);

   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrDlHqCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   cfg->maxDlHqTx = veDfltMaxDlHqTx;
   cfg->maxMsg4HqTx = veDfltMaxMsg4HqTx;

   RETVALUE(ROK);
} /* veFillRgrDlHqCfg */ 

/*
*
*       Fun:   veFillRgrRntiCfg 
*
*       Desc:  Populating RNTI Cfg 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  
*
*/
#ifdef ANSI
PUBLIC S16 veFillRgrRntiCfg
(
RgrRntiCfg *cfg
)
#else
PUBLIC S16 veFillRgrRntiCfg(cfg)
RgrRntiCfg *cfg;
#endif
{
   TRC2(veFillRgrRntiCfg);

   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrRntiCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }
   cfg->startRnti = veCb.cellCb[0]->veDfltStartRnti;
   cfg->size = veCb.cellCb[0]->veDfltMacRntisize;
   

   RETVALUE(ROK);
} /* veFillRgrRntiCfg */ 

/*
*
*       Fun:   veFillRgrCfiCfg 
* 
*       Desc:  Populating CFI Cfg
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  
*
*/
#ifdef ANSI
PUBLIC S16 veFillRgrCfiCfg
(
RgrCfiCfg *cfg
)
#else
PUBLIC S16 veFillRgrCfiCfg(cfg)
RgrCfiCfg *cfg;
#endif
{
   TRC2(veFillRgrCfiCfg);
   
   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrCfiCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   cfg->cfi = veDfltCfiCfg;

   RETVALUE(ROK);
} /* veFillRgrCfiCfg */


/*
 *
 *      Fun:   veFillRgrUlTrgCqiCfg
 *
 *      Desc:  Populate RgrUlTrgCqiCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrUlTrgCqiCfg
(
RgrUlTrgCqiCfg *cfg
)
#else
PUBLIC S16 veFillRgrUlTrgCqiCfg(cfg)
RgrUlTrgCqiCfg *cfg;
#endif
{
   TRC2(veFillRgrUlTrgCqiCfg);

   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrUltrgCqiCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   cfg->trgCqi = veDfltTrgUlCqi;

   RETVALUE(ROK);
} /* veFillRgrUlTrgCqiCfg */

/*
 *
 *      Fun:   veFillRgrDlCmnCodeRateCfg
 *
 *      Desc:  Populate RgrDlCmnCodeRateCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrDlCmnCodeRateCfg
(
RgrDlCmnCodeRateCfg *cfg
)
#else
PUBLIC S16 veFillRgrDlCmnCodeRateCfg(cfg)
RgrDlCmnCodeRateCfg *cfg;
#endif
{
   TRC2(veFillRgrDlCmnCodeRateCfg);
   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrDlCmnCodeRateCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }
   cfg->bcchPchRaCodeRate = veDfltBcchPchRaCodeRate;
   cfg->pdcchCodeRate = veDfltPdcchCodeRate;
   cfg->ccchCqi = veDfltDlCmnCodRateCcchCqi;

   RETVALUE(ROK);
} /* veFillRgrDlCmnCodeRateCfg */

/*
 *
 *      Fun:   veFillRgrPuschSubBandCfg
 *
 *      Desc:  Populate RgrPuschSubBandCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrPuschSubBandCfg
(
RgrPuschSubBandCfg *cfg
)
#else
PUBLIC S16 veFillRgrPuschSubBandCfg(cfg)
RgrPuschSubBandCfg *cfg;
#endif
{
   U8 dmrsCnt = 0;

   TRC2(veFillRgrPuschSubBandCfg);
  
   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrPuschSubBandCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   cfg->subbandStart = veDfltPuschSubBndStrt;
   cfg->numSubbands = veDfltPuschNoSubbnds;
   cfg->size = veDfltPuschSubBandSz;
   
   if(cfg->size > RGR_MAX_SUBBANDS)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrPuschSubBandCfg failed: \
               Number of equal sized sub-bands invalid \n"));
      RETVALUE(RFAILED);
   }

   for(dmrsCnt = 0; dmrsCnt < cfg->numSubbands; dmrsCnt++)
   {
      /* Sample values for the following */
/* MSPD changes */
#ifdef MSPD
      cfg->dmrs[dmrsCnt] = 0;
#else
      cfg->dmrs[dmrsCnt] = veDfltPuschSubBandDmrs + dmrsCnt;
#endif
   }

   RETVALUE(ROK);
} /* veFillRgrPuschSubBandCfg */


/*
 *
 *      Fun:   veFillRgrUlCmnCodeRateCfg
 *
 *      Desc:  Populate RgrUlCmnCodeRateCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrUlCmnCodeRateCfg
(
RgrUlCmnCodeRateCfg *cfg
)
#else
PUBLIC S16 veFillRgrUlCmnCodeRateCfg(cfg)
RgrUlCmnCodeRateCfg *cfg;
#endif
{
   TRC2(veFillRgrUlCmnCodeRateCfg);

   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrUlCmnCodeRateCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   cfg->ccchCqi = veDfltULCmnCodRateCcchCqi;

   RETVALUE(ROK);
} /* veFillRgrUlCmnCodeRateCfg */

/*
 *
 *      Fun:   veFillRgrDlfsCfg
 *
 *      Desc:  Populate RgrDlfsCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrDlfsCfg
(
RgrDlfsCfg *cfg
)
#else
PUBLIC S16 veFillRgrDlfsCfg(cfg)
RgrDlfsCfg *cfg;
#endif
{
   TRC2(veFillRgrDlfsCfg);

  if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrDlfsCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   cfg->isDlFreqSel = veDfltIsDlFreqSel;
   cfg->thresholdCqi = veDfltThresholdCqi;

   RETVALUE(ROK);
} /* veFillRgrDlfsCfg */


/*
 *
 *      Fun:   veFillRgrBwCfg
 *
 *      Desc:  Populate RgrBwCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrBwCfg
(
RgrBwCfg *cfg
)
#else
PUBLIC S16 veFillRgrBwCfg(cfg)
RgrBwCfg *cfg;
#endif
{
   TRC2(veFillRgrBwCfg);

   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrBwCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

#ifndef VE_DONGWON_OAM
   cfg->dlTotalBw = veDfltDLTotalBw;
   cfg->ulTotalBw = veDfltULTotalBw;
#else
   /*OAM_FIX : Mapped the enum to its corresponding value*/
     cfg->dlTotalBw =  enumMapNhuSysBw[veDfltDLTotalBw];
     cfg->ulTotalBw = enumMapNhuSysBw[veDfltULTotalBw];
#endif
   RETVALUE(ROK);
} /* veFillRgrBwCfg */


/*
 *
 *      Fun:   veFillRgrPhichCfg
 *
 *      Desc:  Populate RgrPhichCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrPhichCfg
(
RgrPhichCfg *cfg
)
#else
PUBLIC S16 veFillRgrPhichCfg(cfg)
RgrPhichCfg *cfg;
#endif
{
   TRC2(veFillRgrPhichCfg);

   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrPhichCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   /* g++ compilation fix */
   cfg->ngEnum = (RgrPhichNg)veDfltNgEnum;
   cfg->isDurExtend = veDfltIsDurExt; 

   RETVALUE(ROK);
} /* veFillRgrPhichCfg */


/*
 *
 *      Fun:   veFillRgrPucchCfg
 *
 *      Desc:  Populate RgrPucchCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrPucchCfg
(
RgrPucchCfg *cfg
)
#else
PUBLIC S16 veFillRgrPucchCfg(cfg)
RgrPucchCfg *cfg;
#endif
{
   TRC2(veFillRgrPucchCfg);

   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrPucchCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   cfg->resourceSize = veDfltPUCCHResSz;
   cfg->n1PucchAn = veDfltN1PucchAn;
#ifdef VE_DONGWON_OAM
   /*OAM_FIX : Replaced the enum to its corresponding value*/
   switch(veDfltDeltaShift)
   {
        case NhuPUCCH_ConfigCmndeltaPUCCH_Shiftds1Enum:
                cfg->deltaShift = 1;
                break;
        case NhuPUCCH_ConfigCmndeltaPUCCH_Shiftds2Enum:
                cfg->deltaShift = 2;
                break;
        case NhuPUCCH_ConfigCmndeltaPUCCH_Shiftds3Enum:
                cfg->deltaShift = 3;
                break;
   }
#else
   cfg->deltaShift = veDfltDeltaShift;
#endif
   cfg->cyclicShift = veDfltPUCCHCycShift;

   RETVALUE(ROK);
} /* veFillRgrPucchCfg */


/*
 *
 *      Fun:   veFillRgrSrsCfg
 *
 *      Desc:  Populate RgrSrsCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrSrsCfg
(
RgrSrsCfg *cfg
)
#else
PUBLIC S16 veFillRgrSrsCfg(cfg)
RgrSrsCfg *cfg;
#endif
{
   TRC2(veFillRgrSrsCfg);

   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrSrsCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   /* g++ compilation fix */
   cfg->srsCfgPrdEnum =(RgrSrsCfgPrd)veDfltSrsCfgPrdEnum;
   cfg->srsBwEnum = (RgrSrsBwCfg)veDfltSrsBwEnum;
   cfg->srsSubFrameCfg = veDfltSrsSubFrameCfg;

   RETVALUE(ROK);
} /* veFillRgrSrsCfg */


/*
 *
 *      Fun:   veFillRgrSiCfg
 *
 *      Desc:  Populate RgrSiCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrSiCfg
(
RgrSiCfg *cfg
)
#else
PUBLIC S16 veFillRgrSiCfg(cfg)
RgrSiCfg *cfg;
#endif
{
   TRC2(veFillRgrSiCfg);

   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrSiCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   cfg->siWinSize = veDfltSiWinSize;
   cfg->retxCnt = veDfltRetxCnt;
#ifdef SI_NEW
   /* TIC_ID:ccpu00116701: MOD: Fixed error for g++ compilation on Linux */
   cfg->modPrd = (RgrModPeriodicity)veDfltModPrd;
   /* TIC_ID:ccpu00116701: MOD_END: Fixed error for g++ compilation on Linux */
   cfg->numSi  = veDfltNumSi;
   /* TIC_ID:ccpu00116701: MOD: Fixed error for g++ compilation on Linux */
   cfg->siPeriodicity[0] = (RgrSiPeriodicity)VE_SIB2_PERIOD;
   cfg->siPeriodicity[1] = (RgrSiPeriodicity)VE_SIB3_PERIOD;
   cfg->siPeriodicity[2] = (RgrSiPeriodicity)VE_SIB4_PERIOD;
   cfg->siPeriodicity[3] = (RgrSiPeriodicity)VE_SIB5_PERIOD;
   /* TIC_ID:ccpu00116701: MOD_END: Fixed error for g++ compilation on Linux */
#endif

   RETVALUE(ROK);
} /* veFillRgrSiCfg */

/*
 *
 *      Fun:   veFillRgrUlPwrCfg
 *
 *      Desc:  Populate RgrUlPwrCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrUlPwrCfg
(
RgrUlPwrCfg *cfg
)
#else
PUBLIC S16 veFillRgrUlPwrCfg(cfg)
RgrUlPwrCfg *cfg;
#endif
{
   TRC2(veFillRgrUlPwrCfg);

   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrUlPwrCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   cfg->p0NominalPusch = veDfltP0NominalPusch;
   cfg->alpha = (RgrPwrAlpha)veDfltUlPwrCfgAlpha;/* g++ compilation fix */
   cfg->p0NominalPucch = veDfltP0NominalPucch;
   cfg->deltaPreambleMsg3 = veDfltDeltaPreambleMsg3;
   cfg->pucchPwrFmt3.startTpcRnti = veDfltPucchPwrFmt3RntiSt;
   cfg->pucchPwrFmt3.size = veDfltPucchPwrFmt3RntiSz;
   cfg->pucchPwrFmt3a.startTpcRnti = veDfltPucchPwrFmt3aRntiSt;
   cfg->pucchPwrFmt3a.size = veDfltPucchPwrFmt3aRntiSz;
   cfg->puschPwrFmt3.startTpcRnti = veDfltPuschPwrFmt3RntiSt;
   cfg->puschPwrFmt3.size = veDfltPuschPwrFmt3RntiSz;
   cfg->puschPwrFmt3a.startTpcRnti = veDfltPuschPwrFmt3aRntiSt;
   cfg->puschPwrFmt3a.size = veDfltPuschPwrFmt3aRntiSz;

   RETVALUE(ROK);
} /* veFillRgrUlPwrCfg */

/*
 *
 *      Fun:   veFillRgrPuschCfg
 *
 *      Desc:  Populate RgrPuschCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrPuschCfg
(
RgrPuschCfg *cfg
)
#else
PUBLIC S16 veFillRgrPuschCfg(cfg)
RgrPuschCfg *cfg;
#endif
{
   TRC2(veFillRgrPuschCfg);

   if(NULLP == cfg)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrPuschCfg:cfg is NULL.\n"));
#endif
      RETVALUE(RFAILED);   
   }

   cfg->numSubBands = veDfltPuschCfgNoSubBands;
   cfg->isIntraHop = veDfltPuschCfgIsIntraHop;
   cfg->hopOffst = veDfltPuschCfgHopOffst;

   RETVALUE(ROK);
} /* veFillRgrPuschCfg */

/*
 *
 *      Fun:   veFillRgrPreambleSetCfg
 *
 *      Desc:  Populate RgrPreambleSetCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrPreambleSetCfg
(
RgrPreambleSetCfg *cfg
)
#else
PUBLIC S16 veFillRgrPreambleSetCfg(cfg)
RgrPreambleSetCfg *cfg;
#endif
{
   TRC2(veFillRgrPreambleSetCfg);

   if(NULLP == cfg)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrPreambleSetCfg :cfg is NULL.\n"));
#endif
      RETVALUE(RFAILED);
   }

   cfg->pres = veDfltPreambleSetCfgPres;
   cfg->start = veDfltPreambleSetCfgStart;
   cfg->size = veDfltPreambleSetCfgSz;

   RETVALUE(ROK);
} /* veFillRgrPreambleSetCfg */

#ifdef LTE_TDD
/*
 *
 *      Fun:   veFillRgrTddPrachRscInfo
 *
 *      Desc:  Populate RgrTddPrachRscInfo Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrTddPrachRscInfo
(
RgrTddPrachRscInfo *cfg
)
#else
PUBLIC S16 veFillRgrTddPrachRscInfo(cfg)
RgrTddPrachRscInfo *cfg;
#endif
{
   U8 idx = 0;

   TRC2(veFillRgrTddPrachRscInfo);

   cfg->numRsc = veDfltPRACHNumRes;

   if(cfg->numRsc > RGR_TDD_MAX_FREQ_RSRC)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrTddPrachRscInfo failed: \
               Number of PRACH resources invalid \n"));
      RETVALUE(RFAILED);
   }
   
   for(idx = 0; idx < cfg->numRsc; idx++)
   {
      /* Sample values */
      cfg->prachInfo[idx].freqIdx = idx;
      cfg->prachInfo[idx].sfn = idx + 1;
      cfg->prachInfo[idx].halfFrm = idx;
      cfg->prachInfo[idx].ulStartSfIdx = idx;
   }

   RETVALUE(ROK);
} /* veFillRgrTddPrachRscInfo */

#endif /* LTE_TDD */

/*
 *
 *      Fun:   veFillRgrRachCfg
 *
 *      Desc:  Populate RgrRachCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrRachCfg
(
RgrRachCfg *cfg
)
#else
PUBLIC S16 veFillRgrRachCfg(cfg)
RgrRachCfg *cfg;
#endif
{
   U8 idx = 0;

   TRC2(veFillRgrRachCfg);

   if(NULLP == cfg)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrRachCfg: cfg is NULL.\n"));
#endif
      RETVALUE(RFAILED);
   }

   cfg->preambleFormat = veDfltPreambleFrmt;
   cfg->raWinSize = veDfltRaWinSz;

   cfg->raOccasion.size = veDfltRaOccSz;
   cfg->raOccasion.sfnEnum =(RgrRaSfn)veDfltRaOccSfnEnum;/* g++ compilation fix */
   
   if(cfg->raOccasion.size > RGR_MAX_SUBFRAME_NUM)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrRachCfg failed: \
               Number of subframe numbers invalid \n"));
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < cfg->raOccasion.size; idx++)
   {
      cfg->raOccasion.subFrameNum[idx] = idx + 1;
   }

   cfg->maxMsg3Tx = veDfltMaxMsg3Tx;
#ifdef LTE_HO_SUPPORT
   cfg->numRaPreamble = veCb.cellCb[0]->veDfltNumRaPreamble;
   if(veDfltSzRaPreambleGrpA > veCb.cellCb[0]->veDfltNumRaPreamble)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrRachCfg failed: \
               Number of Randon Access Preambles invalid \n"));
      RETVALUE(RFAILED);
   }
#else
   cfg->numRaPreamble = veDfltNumRaPreamble;
#endif
   cfg->sizeRaPreambleGrpA = veDfltSzRaPreambleGrpA;
   cfg->msgSizeGrpA = veDfltMsgSzGrpA;
   cfg->prachResource = (U8)veDfltPrachResource;/* klock warning fixed */

   RETVALUE(ROK);
} /* veFillRgrRachCfg */

/*
 *
 *      Fun:   veFillRgrCellDlPfs
 *
 *      Desc:  Populate RgrCellDlPfs Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrCellDlPfs
(
RgrCellDlPfs *cfg
)
#else
PUBLIC S16 veFillRgrCellDlPfs(cfg)
RgrCellDlPfs *cfg;
#endif
{
   U8 idx = 0;

   TRC2(veFillRgrCellDlPfs);

   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrCellDlPfs : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < RGR_PFS_PRIO_LVLS; idx++)
   {
      cfg->cqiPrios[idx] = idx + 2; /* Sample value from acc. */
      cfg->fracPrios[idx] = idx + 2; /* Sample value from acc. */
      cfg->svcPrioPrios[idx] = idx + 2; /* Sample value from acc. */
   }

   RETVALUE(ROK);
} /* veFillRgrCellDlPfs */

/*
 *
 *      Fun:   veFillRgrCellUlPfs
 *
 *      Desc:  Populate RgrCellUlPfs Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrCellUlPfs
(
RgrCellUlPfs *cfg
)
#else
PUBLIC S16 veFillRgrCellUlPfs(cfg)
RgrCellUlPfs *cfg;
#endif
{
   U8 idx = 0;

   TRC2(veFillRgrCellUlPfs);

   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrCellUlPfs : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }


   for(idx = 0; idx < RGR_PFS_PRIO_LVLS; idx++)
   {
      cfg->cqiPrios[idx] = idx + 2; /* Sample value from acc. */ 
      cfg->fracPrios[idx] = idx + 2; /* Sample value from acc. */
   }

   for(idx = 0; idx < 3; idx++)
   {
      cfg->lcgPrios[idx] = idx + 2; /* Sample value from acc. */
   }

   RETVALUE(ROK);
} /* veFillRgrCellUlPfs */

/*
 *
 *      Fun:   veFillRgrCmnLcCfg
 *
 *      Desc:  Populate RgrCellCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrCmnLcCfg
(
RgrCellCfg *cfg
)
#else
PUBLIC S16 veFillRgrCmnLcCfg(cfg)
RgrCellCfg *cfg;
#endif
{
   TRC2(veFillRgrCmnLcCfg);

   if(NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrCmnLcCfg : cfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   cfg->numCmnLcs = veDfltNumCmnLcs;

   if(cfg->numCmnLcs > RGR_MAX_CMN_LC_PER_CELL)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrTddPrachRscInfo failed:\
               Number of PRACH resources invalid \n"));
      RETVALUE(RFAILED);
   }

   {
      cfg->cmnLcCfg[0].lcId = VE_BCCH_SIB1_ID;
      cfg->cmnLcCfg[0].lcType = CM_LTE_LCH_BCCH;
      cfg->cmnLcCfg[0].dir = RGR_DIR_TX;
      cfg->cmnLcCfg[0].dlTrchType = CM_LTE_TRCH_DL_SCH;
      cfg->cmnLcCfg[0].ulTrchType = VE_VAL_ZERO;
   }
   {
      cfg->cmnLcCfg[1].lcId = VE_PCCH_ID;
      cfg->cmnLcCfg[1].lcType = CM_LTE_LCH_PCCH;
      cfg->cmnLcCfg[1].dir = RGR_DIR_TX;
      cfg->cmnLcCfg[1].dlTrchType = VE_VAL_ZERO;
      cfg->cmnLcCfg[1].ulTrchType = VE_VAL_ZERO;
   }
   {
      cfg->cmnLcCfg[2].lcId = VE_BCCH_BCH_ID;
      cfg->cmnLcCfg[2].lcType = CM_LTE_LCH_BCCH;
      cfg->cmnLcCfg[2].dir = RGR_DIR_TX;
      cfg->cmnLcCfg[2].dlTrchType = CM_LTE_TRCH_BCH;
      cfg->cmnLcCfg[2].ulTrchType = VE_VAL_ZERO;
   }

   {
      cfg->cmnLcCfg[3].lcId = VE_CCCH_UL_ID;
      cfg->cmnLcCfg[3].lcType = CM_LTE_LCH_CCCH;
      cfg->cmnLcCfg[3].dir = RGR_DIR_RX;
      cfg->cmnLcCfg[3].dlTrchType = CM_LTE_TRCH_DL_SCH;
      cfg->cmnLcCfg[3].ulTrchType = CM_LTE_TRCH_UL_SCH;
   }

   {
      cfg->cmnLcCfg[4].lcId = VE_CCCH_DL_ID;
      cfg->cmnLcCfg[4].lcType = CM_LTE_LCH_CCCH;
      cfg->cmnLcCfg[4].dir = RGR_DIR_TX;
      cfg->cmnLcCfg[4].dlTrchType = CM_LTE_TRCH_DL_SCH;
      cfg->cmnLcCfg[4].ulTrchType = CM_LTE_TRCH_UL_SCH;
   }

   {
      cfg->cmnLcCfg[5].lcId = VE_BCCH_SIB_OTHERS_ID;
      cfg->cmnLcCfg[5].lcType = CM_LTE_LCH_BCCH;
      cfg->cmnLcCfg[5].dir = RGR_DIR_TX;
      cfg->cmnLcCfg[5].dlTrchType = CM_LTE_TRCH_DL_SCH;
      cfg->cmnLcCfg[5].ulTrchType = VE_VAL_ZERO;
   }

   RETVALUE(ROK);

} /* veFillRgrCmnLcCfg */

/*
*
*       Fun:   veHdlRgrCellCfgReq 
*
*       Desc:  Trigger Cell Cfg 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_cell.c 
*
*/
#ifdef ANSI
PUBLIC S16 veHdlRgrCellCfgReq
(
U32            cellId
)
#else
PUBLIC S16 veHdlRgrCellCfgReq(cellId)
U32            cellId;
#endif
{
   Pst           *pst = NULLP;
   RgrCfgReqInfo *cfgReq = NULLP;
   RgrCellCfg    *cfg = NULLP;
   RgrCfgTransId transId;

   TRC2(veHdlRgrCellCfgReq);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHdlRgrCellCfgReq:\
            Triggering Cell Cfg over RGR\n"));
#endif

   /* Assign Pst */
   pst = &veCb.rgrSap[0]->pst;
   pst->event = EVTRGRCFGREQ;

   /* Trans Id */
   VE_SET_ZERO(&transId , sizeof(RgrCfgTransId));

   /* Allocate RGR Cfg struct */
   VE_ALLOC(&cfgReq, sizeof(RgrCfgReqInfo));
   /* klock warning fixed */
   if(cfgReq == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq:\
               Failure in the allocation of configuration req\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */
   /* Populate the cfg. values */
   cfgReq->action = RGR_CONFIG;
   cfgReq->u.cfgInfo.cfgType = RGR_CELL_CFG;

   cfg = &(cfgReq->u.cfgInfo.u.cellCfg);

   /* Populate RgrCellCfg */
   cfg->cellId = (U16)cellId;
   /* MAC instance */
   cfg->macInst = veDfltMacInst;
   /* Maximum UEs that may be scheduled UL and DL */ 
   cfg->maxUePerUlSf = veDfltMaxUePerUlSf;
   cfg->maxUePerDlSf = veDfltMaxUePerDlSf;
   /* Maximum uplink & downlink bandwidth */
#ifdef YS_PICO   
   cfg->maxUlBwPerUe = 30;
#else
   cfg->maxUlBwPerUe = veDfltMaxUlBwPerUe;
#endif
   /* klock warning fixed */
   cfg->maxDlBwPerUe = (U8)veDfltMaxDlBwPerUe;
   /* Maximum DL retransmissions B/W in RBs */
   cfg->maxDlRetxBw = veDfltMaxDlRetxBw;
   /* Number UE's per TTI for new transmissions */
   cfg->maxDlUeNewTxPerTti = veDfltMaxDlUeNwTxPerTti;
   /* Number of UEs per TTI for UL
     - correcting the assignment with actual configured */
   cfg->maxUlUeNewTxPerTti = veDfltMaxUlUeNwTxPerTti; 
   /* Number of Transmission antenna ports */
   /* klock warning fixed */
#ifdef VE_DONGWON_OAM
   /*OAM_FIX : Replaced as part of OAM changes*/
   switch(veDfltNumOfTxAnt)
   {
        case NhuAntennaInfoCmnantennaPortsCountan1Enum:
                cfg->numTxAntPorts = 1;
                break;
        case NhuAntennaInfoCmnantennaPortsCountan2Enum:
                cfg->numTxAntPorts = 2;
                break;
        case NhuAntennaInfoCmnantennaPortsCountan4Enum:
                cfg->numTxAntPorts = 4;
                break;
   }
#else
   cfg->numTxAntPorts = (U8)veDfltNumOfTxAnt;
#endif
   /* Cyclic prefix */
   cfg->isCpUlExtend = (Bool)veDfltIsCycPrefULExt;
   cfg->isCpDlExtend = (Bool)veDfltIsCycPrefDLExt;
   /* Cell Midulation Scheme */
   cfg->cellModSchm = (Bool)veDfltCellModSchm;
   /* Cell Uplink transmission power */
   cfg->pMax = (S8)veDfltPMax;

   /* DL scheduler to be used */
   cfg->dlSchdType = veDfltDlSchdType;
   /* DL PFS Scheduler */
   if(veFillRgrCellDlPfs(&cfg->dlSchInfo.cellDlPfs) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrCellDlPfs failed.\n"));
      RETVALUE(RFAILED);
   }

   /* UL scheduler to be used */
   cfg->ulSchdType = veDfltUlSchdType;
   /* UL PFS Scheduler */
   if(veFillRgrCellUlPfs(&cfg->ulSchInfo.cellUlPfs) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrCellUlPfs failed.\n"));
      RETVALUE(RFAILED);
   }
   /* DLFS scheduler to use */
   cfg->dlfsSchdType = veDfltDLFSSchdTyp;
   /* HARQ related configuration */
   if(veFillRgrDlHqCfg(&cfg->dlHqCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrDlHqCfg failed.\n"));
      RETVALUE(RFAILED);
   }

   /* Range of RNTIs to be managed by MAC */
   if(veFillRgrRntiCfg(&cfg->macRnti) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrRntiCfg failed.\n"));
      RETVALUE(RFAILED);
   }

   /* CFI for PDCCH */
   if(veFillRgrCfiCfg(&cfg->cfiCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrCfiCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* Target UL CQI */
   if(veFillRgrUlTrgCqiCfg(&cfg->trgUlCqi) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrUlTrgCqiCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* Coding rate for common DL channels */
   if(veFillRgrDlCmnCodeRateCfg(&cfg->dlCmnCodeRate) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrDlCmnCodeRateCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* UL sub-band information */
   if(veFillRgrPuschSubBandCfg(&cfg->puschSubBand) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrPuschSubBandCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* Coding rate for common UL channels */
   if(veFillRgrUlCmnCodeRateCfg(&cfg->ulCmnCodeRate) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrUlCmnCodeRateCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* Configuration for DLFS scheduler */
   if(veFillRgrDlfsCfg(&cfg->dlfsCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrDlfsCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* Bandwidth configuration */ 
   if(veFillRgrBwCfg(&cfg->bwCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrBwCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* PHICH configuration information */
   if(veFillRgrPhichCfg(&cfg->phichCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrPhichCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* PUCCH configuration information */
   if(veFillRgrPucchCfg(&cfg->pucchCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrPucchCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* SRS configuration information */ 
   if(veFillRgrSrsCfg(&cfg->srsCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrSrsCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* RACH configuration */
   if(veFillRgrRachCfg(&cfg->rachCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrRachCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }
   /* SI configuration */
   if(veFillRgrSiCfg(&cfg->siCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrSiCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* Cell-specific power configuration */ 
   if(veFillRgrUlPwrCfg(&cfg->pwrCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrUlPwrCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* Cell specific hopping configuration  */
   if(veFillRgrPuschCfg(&cfg->puschCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrPuschCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* PDCCH Order Preamble Id's */
   if(veFillRgrPreambleSetCfg(&cfg->macPreambleSet) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrPreambleSetCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* Configuration for common logical channels */
   if(veFillRgrCmnLcCfg(cfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrCmnLcCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

#ifdef LTE_TDD
   /* UL-DL configuration index */
   cfg->ulDlCfgIdx = veUlDlCfgIdx;
   /* Special Subframe configuration index */
   cfg->spclSfCfgIdx = veSpclSfCfgIdx;
   /* PRACH information */
   if(veFillRgrTddPrachRscInfo(&cfg->prachRscInfo) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: veFillRgrTddPrachRscInfo\
               failed.\n"));
      RETVALUE(RFAILED);
   }
#endif
#ifdef RGR_RRM_TICK
   cfg->rrmTtiIndPrd = 1;
#endif

   /* Fill appropriate TransId details 
    * here since lcId & crnti doesnt hold they are populated
    * as zero */
   VE_SET_RGR_TRANS(transId.trans, RGR_CELL_CFG, VE_VAL_ZERO, VE_VAL_ZERO);

   if(ROK != VeLiRgrCfgReq(pst, veCb.rgrSap[0]->spId, transId, cfgReq))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlRgrCellCfgReq: VeLiRgrCfgReq primitive\
               failed.\n"));      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veHdlRgrCellCfgReq */

/* ve004.102: added new function to support CTF cell reconfgiuration */
/*
*
*       Fun:   veSndCtfCellReCfgReq
*
*       Desc:  Trigger Cell reCfg. Fill the present filed to PRSNT_NODEF which needs to be reconfigured
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_cell.c
*
*/
#ifdef ANSI
PUBLIC S16 veSndCtfCellReCfgReq
(
U32  cellId
)
#else
PUBLIC S16 veSndCtfCellReCfgReq(cellId)
U32  cellId;
#endif
{
   CtfCfgReqInfo    *cellCfgReq = NULLP;
   Pst              *pst = NULLP;
   CtfCfgTransId    transId;

#ifdef YS_PICO   
   VePicoSpecific picoCellCfgReq;
#endif   

   TRC2(veSndCtfCellReCfgReq);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veSndCtfCellReCfgReq:\
            Trigger Cell ReCfg Req. Over CTF Interface \n"));
#endif

   /* Copying the pst structure */
   pst = &veCb.ctfSap[0]->pst;
   pst->event   = EVTCTFCFGREQ;

   VE_ALLOC(&cellCfgReq, sizeof(CtfCfgReqInfo));
   if(cellCfgReq == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndCtfReCellCfgReq:\
               Failure in the allocation of configuration info\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

#ifdef CTF_VER3
    VE_ALLOC(&cellCfgReq->vendorParams.paramBuffer, sizeof(VePicoSpecific));
   if(cellCfgReq->vendorParams.paramBuffer == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndCtfCellReCfgReq:\
               Failure in the allocation \n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */    
#endif
   cmMemset((U8 *)&transId, '\0', sizeof(CtfCfgTransId));
#ifdef YS_PICO   
   cmMemset((U8 *)&picoCellCfgReq, 0, sizeof(VePicoSpecific));
#endif   

   cellCfgReq->cfgType = CTF_RECONFIG;
   cellCfgReq->u.reCfg.cfgElem = CTF_CELL_CFG;
   cellCfgReq->u.reCfg.u.cellRecfg.cellId = cellId;

   /* Make changes to take out the parameters which were guarded 
    * under YS_PICO flag for mindspeed */               
#ifdef YS_PICO
   picoCellCfgReq.ctfReCfgType = VE_CTF_MINOR_RECFG; 
   picoCellCfgReq.phyCellIdPres = NOTPRSNT;
   picoCellCfgReq.phyCellId = cellId;
  /* Bandwidth Config */
   picoCellCfgReq.bwCfgPres = NOTPRSNT;
   picoCellCfgReq.bwCfg.dlBw = veDfltCellDlBW;
   picoCellCfgReq.bwCfg.ulBw = veDfltCellUlBW;
   picoCellCfgReq.bwCfg.eUtraBand = veCb.cellCb[0]->sysRelInfo.freqBandInd;

   /* Basic transmission scheme */
   picoCellCfgReq.txCfgPres = NOTPRSNT;
   picoCellCfgReq.txCfg.duplexMode =
      (veCb.cellCb[0]->duplexMode == CTF_DUPMODE_FDD) ?
      CTF_DUPMODE_FDD : CTF_DUPMODE_TDD;
   picoCellCfgReq.txCfg.scSpacing = veDfltScSpacing;
   picoCellCfgReq.txCfg.cycPfx = veDfltCycPfx;
   /* Antenna config */
   picoCellCfgReq.antennaCfg.antPortsCnt = CTF_AP_CNT_2;
#else
   cellCfgReq->u.reCfg.u.cellRecfg.ctfReCfgType = CTF_MINOR_RECFG ;
   cellCfgReq->u.reCfg.u.cellRecfg.physCellIdPres = NOTPRSNT ;
   cellCfgReq->u.reCfg.u.cellRecfg.physCellId = cellId; 
  /* Bandwidth Config */
   cellCfgReq->u.reCfg.u.cellRecfg.bwCfg.pres = NOTPRSNT ;
   cellCfgReq->u.reCfg.u.cellRecfg.bwCfg.dlBw = veDfltCellDlBW;
   cellCfgReq->u.reCfg.u.cellRecfg.bwCfg.ulBw = veDfltCellUlBW;
   cellCfgReq->u.reCfg.u.cellRecfg.bwCfg.eUtraBand = veCb.cellCb[0]->sysRelInfo.freqBandInd;

   /* Basic transmission scheme */
   cellCfgReq->u.reCfg.u.cellRecfg.txCfg.pres = NOTPRSNT ;
   cellCfgReq->u.reCfg.u.cellRecfg.txCfg.duplexMode =
      (veCb.cellCb[0]->duplexMode == CTF_DUPMODE_FDD) ?
      CTF_DUPMODE_FDD : CTF_DUPMODE_TDD;
   cellCfgReq->u.reCfg.u.cellRecfg.txCfg.scSpacing = veDfltScSpacing;
   cellCfgReq->u.reCfg.u.cellRecfg.txCfg.cycPfx = veDfltCycPfx;
   /* Antenna config */
   cellCfgReq->u.reCfg.u.cellRecfg.antennaCfg.antPortsCnt = CTF_AP_CNT_2; 
#endif   

   /* PRACH config */
   cellCfgReq->u.reCfg.u.cellRecfg.prachCfg.pres =NOTPRSNT;
   cellCfgReq->u.reCfg.u.cellRecfg.prachCfg.rootSequenceIndex \
      = veDfltRootSequenceIdx;
   cellCfgReq->u.reCfg.u.cellRecfg.prachCfg.prachCfgIndex = (U8)veDfltPrachCfgIdx;
   cellCfgReq->u.reCfg.u.cellRecfg.prachCfg.zeroCorrelationZoneCfg \
      = (U8)veDfltZeroCorrelZoneCfg;
   cellCfgReq->u.reCfg.u.cellRecfg.prachCfg.highSpeedFlag = (U8)veDfltHighSpeedFlag;
   cellCfgReq->u.reCfg.u.cellRecfg.prachCfg.prachFreqOffset \
      = (U8)veDfltPrachFreqOffset;
   /* PDSCH config */
   cellCfgReq->u.reCfg.u.cellRecfg.pdschCfg.pres =NOTPRSNT;
   cellCfgReq->u.reCfg.u.cellRecfg.pdschCfg.refSigPwr = (U8)veDfltRefSignalPower;
   cellCfgReq->u.reCfg.u.cellRecfg.pdschCfg.p_b = (U8)veDfltPDSCHCfgPB;
   /* PUSCH config */
   cellCfgReq->u.reCfg.u.cellRecfg.puschCfg.pres = NOTPRSNT;

   cellCfgReq->u.reCfg.u.cellRecfg.puschCfg.puschBasicCfg.noOfsubBands \
      = (U8)veDfltNSB;
   cellCfgReq->u.reCfg.u.cellRecfg.puschCfg.puschBasicCfg.hoppingMode \
      = veDfltHoppingMode;
   cellCfgReq->u.reCfg.u.cellRecfg.puschCfg.puschBasicCfg.hoppingOffset\
      = (U8)veDfltPuschHoppingOff;
   cellCfgReq->u.reCfg.u.cellRecfg.puschCfg.puschBasicCfg.enable64QAM\
      = (U8)veDfltEnable64QAM;

   cellCfgReq->u.reCfg.u.cellRecfg.puschCfg.puschUlRS.grpHopEnabled\
      = (U8)veDfltGroupHoppingEnabled;
   cellCfgReq->u.reCfg.u.cellRecfg.puschCfg.puschUlRS.seqHopEnabled\
      = (U8)veDfltSeqHoppingEnabled;
   cellCfgReq->u.reCfg.u.cellRecfg.puschCfg.puschUlRS.grpNum\
      = (U8)veDfltGroupAssignPUSCH;
   cellCfgReq->u.reCfg.u.cellRecfg.puschCfg.puschUlRS.cycShift\
      = (U8)veDfltCyclicShift;
   /* PHICH config */
   cellCfgReq->u.reCfg.u.cellRecfg.phichCfg.pres = NOTPRSNT;
   cellCfgReq->u.reCfg.u.cellRecfg.phichCfg.duration = veDfltPhichDur;
   cellCfgReq->u.reCfg.u.cellRecfg.phichCfg.resource = veDfltPhichRes;
   /* PUCCH config */
   cellCfgReq->u.reCfg.u.cellRecfg.pucchCfg.pres = NOTPRSNT;
   cellCfgReq->u.reCfg.u.cellRecfg.pucchCfg.nRB = (U8)veDfltPUCCHnRBCQI;
   cellCfgReq->u.reCfg.u.cellRecfg.pucchCfg.nCS = (U8)veDfltPUCCHnCSAN;
   cellCfgReq->u.reCfg.u.cellRecfg.pucchCfg.n1PUCCH = veDfltNlPUCCHAn;
   cellCfgReq->u.reCfg.u.cellRecfg.pucchCfg.deltaShift = veDfltPUCCHDeltaShift;
   /* SRS UL config, setup case */
   cellCfgReq->u.reCfg.u.cellRecfg.srsUlCfg.pres = NOTPRSNT;
#ifdef LTE_TDD
   cellCfgReq->u.reCfg.u.cellRecfg.tddSfCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.reCfg.u.cellRecfg.tddSfCfg.sfAssignment \
      = veDfltSubframeAssignment;
   cellCfgReq->u.reCfg.u.cellRecfg.tddSfCfg.spclSfPatterns \
      = veDfltSpecialSubfrmPatterns;
#else /* LTE_TDD */
   cellCfgReq->u.reCfg.u.cellRecfg.tddSfCfg.pres = NOTPRSNT;
#endif /* LTE_TDD */
   /* Make changes to take out the parameters which were guarded 
    * under YS_PICO flag for mindspeed */               
#ifdef YS_PICO
   picoCellCfgReq.syncSigPowOsPres = NOTPRSNT;
   picoCellCfgReq.syncSigPowOs = 6000;
    
   picoCellCfgReq.cfiPowOsPres = PRSNT_NODEF;
   picoCellCfgReq.cfiPowOs = 6005;

   picoCellCfgReq.dciPowOsPres = NOTPRSNT;
   picoCellCfgReq.dciPowOs = 6000;

   picoCellCfgReq.extWinMarginPres = NOTPRSNT;
   picoCellCfgReq.extWinMargin = 5;

   picoCellCfgReq.pucchNoiseGammaPres = NOTPRSNT;
   picoCellCfgReq.pucchNoiseGamma = 0x2000;

   picoCellCfgReq.prachPkRatio4Pres = NOTPRSNT;
   picoCellCfgReq.prachPkRatio4 = 0x24eb;

   picoCellCfgReq.prachPkRatio0Pres = NOTPRSNT;
   picoCellCfgReq.prachPkRatio0 = 0x38cc;

   picoCellCfgReq.srsDopEstFactorPres = NOTPRSNT;
   picoCellCfgReq.srsDopEstFactor = 0xffff;

   picoCellCfgReq.puschProbDtxAckPres = NOTPRSNT;
   picoCellCfgReq.puschProbDtxAck = 22;

   picoCellCfgReq.pucchProbDtxAckPres = NOTPRSNT;
   picoCellCfgReq.pucchProbDtxAck = 25;

   picoCellCfgReq.txAntennaPortsPres = NOTPRSNT;
   picoCellCfgReq.txAntennaPorts = 1;

   picoCellCfgReq.rxAntennaPortsPres = NOTPRSNT;
   picoCellCfgReq.rxAntennaPorts = 25;

   picoCellCfgReq.phySyncModePres= NOTPRSNT;
   picoCellCfgReq.phySyncMode = 0x02;

   picoCellCfgReq.dataRepModePres = NOTPRSNT;
   picoCellCfgReq.dataRepMode = 0x00;

   picoCellCfgReq.rachSrRepModePres = NOTPRSNT;
   picoCellCfgReq.rachSrRepMode = 0;

#ifdef CTF_VER3
   cellCfgReq->vendorParams.buffLen = sizeof(picoCellCfgReq);
   cmMemcpy((U8*)(cellCfgReq->vendorParams.paramBuffer),(U8*)&picoCellCfgReq,sizeof(VePicoSpecific));
#endif   
#else   
   cellCfgReq->u.reCfg.u.cellRecfg.syncSigPowOsPres = NOTPRSNT;
   cellCfgReq->u.reCfg.u.cellRecfg.syncSigPowOs = 6000;

   cellCfgReq->u.reCfg.u.cellRecfg.cfiPowOsPres = PRSNT_NODEF;
   cellCfgReq->u.reCfg.u.cellRecfg.cfiPowOs = 6005;

   cellCfgReq->u.reCfg.u.cellRecfg.dciPowOsPres = NOTPRSNT ;
   cellCfgReq->u.reCfg.u.cellRecfg.dciPowOs = 6000;

   cellCfgReq->u.reCfg.u.cellRecfg.extWinMarginPres = NOTPRSNT ;
   cellCfgReq->u.reCfg.u.cellRecfg.extWinMargin = 5;

   cellCfgReq->u.reCfg.u.cellRecfg.pucchNoiseGammaPres = NOTPRSNT ;
   cellCfgReq->u.reCfg.u.cellRecfg.pucchNoiseGamma = 0x2000;

   cellCfgReq->u.reCfg.u.cellRecfg.prachPkRatio4Pres = NOTPRSNT ;
   cellCfgReq->u.reCfg.u.cellRecfg.prachPkRatio4 = 0x24eb;

   cellCfgReq->u.reCfg.u.cellRecfg.prachPkRatio0Pres = NOTPRSNT ;
   cellCfgReq->u.reCfg.u.cellRecfg.prachPkRatio0 = 0x38cc;

   cellCfgReq->u.reCfg.u.cellRecfg.srsDopEstFactorPres = NOTPRSNT ;
   cellCfgReq->u.reCfg.u.cellRecfg.srsDopEstFactor = 0xffff;

   cellCfgReq->u.reCfg.u.cellRecfg.puschProbDtxAckPres = NOTPRSNT;
   cellCfgReq->u.reCfg.u.cellRecfg.puschProbDtxAck = 22;

   cellCfgReq->u.reCfg.u.cellRecfg.pucchProbDtxAckPres = NOTPRSNT;
   cellCfgReq->u.reCfg.u.cellRecfg.pucchProbDtxAck = 25;

/* MS_FIX */
   cellCfgReq->u.reCfg.u.cellRecfg.txAntennaPortsPres = NOTPRSNT ;
   cellCfgReq->u.reCfg.u.cellRecfg.txAntennaPorts = veDfltCtfAntPort;


   cellCfgReq->u.reCfg.u.cellRecfg.rxAntennaPortsPres = NOTPRSNT;
   cellCfgReq->u.reCfg.u.cellRecfg.rxAntennaPorts = 25;

   cellCfgReq->u.reCfg.u.cellRecfg.phySyncModePres= NOTPRSNT ;
   cellCfgReq->u.reCfg.u.cellRecfg.phySyncMode = 0x02;

   cellCfgReq->u.reCfg.u.cellRecfg.dataRepModePres = NOTPRSNT ;
   cellCfgReq->u.reCfg.u.cellRecfg.dataRepMode = 0x00;

   cellCfgReq->u.reCfg.u.cellRecfg.rachSrRepModePres = NOTPRSNT ;
   cellCfgReq->u.reCfg.u.cellRecfg.rachSrRepMode = 0 ;
#endif

   if(ROK != VeLiCtfCfgReq(pst, veCb.ctfSap[0]->spId, transId,  cellCfgReq))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCtfCfgReq failed.\n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndCtfCellReCfgReq */


/**********************************************************************31
         End of file:     ve_cell.c@@/main/2 - Wed Dec  1 08:44:22 2010

*********************************************************************31*/


/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
$SID$      ----       mnawas        1. eNodeb 3.1 Release
*********************************************************************91*/

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


/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_sch_dhm.c
  
     Sid:      gk_sch_dhm.c@@/main/2 - Sat Jul 30 02:21:38 2011
  
     Prg:      ps
  
**********************************************************************/

/** @file rg_sch_dhm.c
@brief APIs related to Downlink HARQ for the scheduler.
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm5.h"           /* common timers */
#include "cm_hash.h"       /* common hash list */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_err.h"
#include "rg_sch_inf.h"         /* typedefs for Scheduler */
#include "rg_sch.h"
#include "rg_sch_cmn.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system service interface */
#include "cm5.x"           /* common timers */
#include "cm_lib.x"        /* common library */
#include "cm_hash.x"       /* common hash list */
#include "cm_llist.x"      /* common linked list library */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common LTE */
#include "lrg.x"
#include "rgr.x"
#include "tfu.x"
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "rg_sch.x"
#include "rg_sch_cmn.x"

#ifdef MAC_SCH_STATS
PUBLIC RgSchNackAckStats hqFailStats;
PUBLIC RgSchHqRetxStats  hqRetxStats;
#endif /* MAC_SCH_STATS */

/* local typedefs */
 
/* local externs */

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/*rg002.301 ccpu00119453 MOD added extra input parameter */
PRIVATE Void rgSCHDhmFdbkIndHndlTa ARGS((RgSchDlHqProcCb *hqP, U8 tbIdx, U8 fdbk,
            Bool maxHqRetxReached));

#ifndef LTE_TDD
PRIVATE S16 rgSCHDhmProcHqFdbkAckNackRep ARGS((
RgSchDlHqProcCb      *hqP,
RgSchDlSf            *sf,
U8                   tbCnt,
U8                   *isAck
));
#endif

/* rg003.301 -Add-[ccpu00120794] - Freeing up the HARQ proc blocked for
 * indefinite time in case of Retx */
PUBLIC S16 rgSCHDhmDlRetxAllocFail ARGS((
RgSchUeCb        *ue, 
RgSchDlHqProcCb  *proc
));

/* forward references */

/**
 * @brief This function initializes the DL HARQ Entity of UE.
 *
 * @details
 *
 *     Function: rgSCHDhmHqEntInit
 *     Purpose:  This function initializes the DL HARQ entity of 
 *               UE control block. This is performed at the time
 *               of creating UE control block.
 *     
 *     Invoked by: configuration module
 *     
 *  @param[in]  RgSchCellCb*  cell
 *  @return  RgSchDlHqEnt *
 *
 **/
/*MS_WORKAROUND for ccpu00122893*/
#ifdef ANSI
PUBLIC Void rgSCHDhmHqEntReset
(
   RgSchDlHqEnt      *hqE
)
#else
PUBLIC Void rgSCHDhmHqEntReset(hqE)
   RgSchDlHqEnt      *hqE;
#endif
{
   RgSchDlHqProcCb   *hqP;
   U8                i;
   TRC2(rgSCHDhmHqEntReset)
   cmLListInit(&hqE->inUse);	   
   cmLListInit(&hqE->free);	   
   for (i=0; i < hqE->numHqPrcs; i++)
   {
      hqP              = &hqE->procs[i];
      hqP->hqE         = hqE;
      hqP->procId      = i;
      /* MS_FIX  - reset numLch */
      hqP->tbInfo[0].numLch      = 0;
      hqP->tbInfo[1].numLch      = 0;
      hqP->tbInfo[0].txCntr      = 0;
      hqP->tbInfo[0].ndi         = 0;  /* Initialize the NDI to Zero */
      hqP->tbInfo[1].txCntr      = 0;
      hqP->tbInfo[1].ndi         = 0;  /* Initialize the NDI to Zero */
      hqP->tbInfo[0].tbIdx       = 0;
      hqP->tbInfo[1].tbIdx       = 1;
      hqP->tbInfo[0].hqP         = hqP;
      hqP->tbInfo[1].hqP         = hqP;
      hqP->tbInfo[0].state       = HQ_TB_ACKED;
      hqP->tbInfo[1].state       = HQ_TB_ACKED;
      hqP->lnk.node = (PTR)hqP;
      cmLListAdd2Tail(&hqE->free, &hqP->lnk);
      hqP->tbInfo[0].lchSchdData = hqP->tbInfo[0].lchSchdDataArr;
      hqP->tbInfo[1].lchSchdData = hqP->tbInfo[1].lchSchdDataArr;
#ifdef LTEMAC_DRX
      hqP->drxCb.rttIndx  = DRX_INVALID;
      hqP->drxCb.reTxIndx = DRX_INVALID;
#endif /*LTEMAC_DRX */
      hqP->tbInfo[0].cntrRetxAllocFail = 0;
      hqP->tbInfo[1].cntrRetxAllocFail = 0;
   }
   RETVOID;
} /* rgSCHDhmHqEntReset */

/**
 * @brief This function assigns dlHqEnt of raCb to ueCb.
 *
 * @details
 *
 *     Function: rgSCHDhmAssgnUeHqEntFrmRaCb 
 *     Purpose:  This function assigns dlHqEnt of raCb to ueCb.
 *     
 *     Invoked by: configuration module
 *     
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  RgSchRaCb       *raCb
 *  @return  Void 
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHDhmAssgnUeHqEntFrmRaCb 
(
RgSchUeCb       *ue,
RgSchRaCb       *raCb
)
#else
PUBLIC Void rgSCHDhmAssgnUeHqEntFrmRaCb(ue, raCb)
RgSchUeCb       *ue;
RgSchRaCb       *raCb;
#endif
{
   TRC2(rgSCHDhmAssgnUeHqEntFrmRaCb)

   ue->dl.hqEnt = raCb->dlHqE;
   ue->dl.hqEnt->ue = ue;
   raCb->dlHqE = NULLP;
   /* MS_FIX : set UE active in DL as UE initialization completed */
   ue->dl.dlInactvMask &= ~(RG_HQENT_INACTIVE);
   ue->ul.ulInactvMask &= ~(RG_HQENT_INACTIVE);
   rgSCHCmnDlInitHqEnt(ue->cell, ue);
   
   RETVOID;
}

/**
 * @brief This function deletes the dlHqEnt.
 *
 * @details
 *
 *     Function: rgSCHDhmDelHqEnt 
 *     Purpose:  This function deletes the dlHqEnt.
 *     
 *     Invoked by: configuration module
 *     
 *  @param[in]  RgSchCellCb       *cell 
 *  @param[in]  RgSchDlHqEnt      **hqE 
 *  @return  Void 
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHDhmDelHqEnt 
(
RgSchCellCb       *cell,
RgSchDlHqEnt      **hqE
)
#else
PUBLIC Void rgSCHDhmDelHqEnt(cell, hqE)
RgSchCellCb       *cell;
RgSchDlHqEnt      **hqE;
#endif
{
   TRC2(rgSCHDhmDelHqEnt)

   if (!(*hqE))
   {
      RETVOID;	   
   }

   rgSCHCmnDlDeInitHqEnt(cell, *hqE);

   rgSCHUtlFreeSBuf(cell->instIdx, (Data **)hqE, 
       sizeof(RgSchDlHqEnt));
   
   RETVOID;
}

#ifdef ANSI
PUBLIC RgSchDlHqEnt *rgSCHDhmHqEntInit
(
RgSchCellCb           *cell
)
#else
PUBLIC RgSchDlHqEnt *rgSCHDhmHqEntInit(cell)
RgSchCellCb           *cell;
#endif
{
   RgSchDlHqEnt      *hqE;
   Inst              inst = cell->instIdx;

   TRC2(rgSCHDhmHqEntInit)

   /* Init the HARQ data structure */
   if (rgSCHUtlAllocSBuf(inst, (Data **)&hqE, sizeof(RgSchDlHqEnt)) != ROK)
   {
      RETVALUE(NULLP);
   }
#ifdef LTE_TDD
   /* Init the HARQ processes */
   hqE->numHqPrcs = rgSchTddDlNumHarqProcTbl[cell->ulDlCfgIdx];
   if (rgSCHUtlAllocSBuf(inst, (Data **)&hqE->procs, 
                           hqE->numHqPrcs * sizeof(RgSchDlHqProcCb)) != ROK)
   {
      RETVALUE(NULLP);
   }
#else
   hqE->numHqPrcs = RGSCH_NUM_DL_HQ_PROC;
#endif

   /* Initialize maximum tranmission counter */
   hqE->maxHqTx = cell->dlHqCfg.maxDlHqTx;
   
   RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),"\n rgSCHDhmHqEntInit():\
                    numHqPrcs=%d maxHqTx=%d",hqE->numHqPrcs,hqE->maxHqTx));

   /* MW_WORKAROUND for ccpu00122893 */
   rgSCHDhmHqEntReset(hqE); 

   RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),"\n rgSCHDhmHqEntInit():\
            Harq Entity Initialized with free_cnt=%lu and inUse_cnt=%lu",\
            hqE->free.count,hqE->inUse.count));

   RETVALUE(hqE);
} /* rgSCHDhmHqEntInit */

/**
 * @brief This function gets an available HARQ process.
 *
 * @details
 *
 *     Function: rgSCHDhmGetAvlHqProc
 *     Purpose:  This function returns an available HARQ process in 
 *               the DL direction. All HARQ processes are maintained
 *               in queues of free and inuse.
 *
 *               1. Check if the free queue is empty. If yes, return
 *                  RFAILED
 *               2. If not empty, update the proc variable with the
 *                  first process in the queue. Return ROK.
 *     
 *     Invoked by: scheduler
 *     
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  CmLteTimingInfo  timingInfo
 *  @param[out] RgSchDlHqProc       **hqP
 *  @return  S16       
 *         -#ROK     if successful
 *         -#RFAILED otherwise
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDhmGetAvlHqProc
(
RgSchUeCb               *ue,
CmLteTimingInfo         timingInfo,
RgSchDlHqProcCb         **hqP
)
#else
PUBLIC S16 rgSCHDhmGetAvlHqProc (ue, timingInfo, hqP)
RgSchUeCb               *ue;
CmLteTimingInfo         timingInfo;
RgSchDlHqProcCb         **hqP;
#endif
{
   RgSchDlHqProcCb   *tmpHqProc;
   CmLList           *tmp;
#ifdef DEBUGP
   Inst              inst = ue->cell->instIdx;
#endif
   TRC2(rgSCHDhmGetAvlHqProc);
   
   RGSCHDBGINFO(inst, (rgSchPBuf(inst),"\n rgSCHDhmGetAvlHqProc():\
           UEID=%d,timingInfo sfn=%d,sf=%d,free hq cnt=%lu,used hq cnt=%lu",\
           ue->ueId,timingInfo.sfn,timingInfo.subframe,ue->dl.hqEnt->free.count,
           ue->dl.hqEnt->inUse.count));

   CM_LLIST_FIRST_NODE(&(ue->dl.hqEnt->free), tmp);

   if (NULLP == tmp)
   {
      MSPD_DBG("FAILED UE(%d) (%d:%d)\n", ue->ueId, ue->dl.hqEnt->free.count, ue->dl.hqEnt->inUse.count);	   
      RGSCHDBGINFO(inst, (rgSchPBuf(inst), "No Free Harq Procs Available.\n"));
      /* No Harq Process available in the free queue. */
      RETVALUE(RFAILED);
   }

   tmpHqProc = (RgSchDlHqProcCb *)(tmp->node);

#ifdef LTEMAC_SPS
   /* If SPS HARQ procs are in use, do not use SPS harq procs for non-SPS
    * transmissions */
   if (ue->dl.isSpsHqPInUse)
   {
      while (tmpHqProc->procId < ue->dl.dlSpsCfg.numSpsHqProc)
      {
         CM_LLIST_NEXT_NODE(&(ue->dl.hqEnt->free), tmp);
         if (!tmp)
         {
            break;
         }
         tmpHqProc = (RgSchDlHqProcCb *)(tmp->node);
      }
      if (!tmp)
      {
         RGSCHDBGINFO(inst, (rgSchPBuf(inst), "No Free Harq Procs Available"
                  ".\n"));
         /* No Harq Process available in the free queue. */
         RETVALUE(RFAILED);
      }
   }
#endif


   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependncy on MIMO compile-time flag */
   tmpHqProc->tbInfo[0].timingInfo = timingInfo;
   tmpHqProc->tbInfo[1].timingInfo = timingInfo;

   /* Remove the element from the free Queue */
   cmLListDelFrm(&ue->dl.hqEnt->free, tmp);

   /* Add the element into the inUse Queue as well */
   cmLListAdd2Tail(&ue->dl.hqEnt->inUse, &tmpHqProc->lnk);

   *hqP = tmpHqProc;

   RETVALUE(ROK);
} /* rgSCHDhmGetAvlHqProc */

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/**
 * @brief This function adds HARQ process for a given TB in to 
 *        the inuse queue upon retx.
 *
 * @details
 *
 *     Function: rgSCHDhmHqTbRetx
 *     Purpose:  This function handles when a HARQ process is scheduled
 *               for retransmission. It adds the HARQ procss to inuse
 *               queue.
 *               1. Check if this HqP is already added to the inUse
 *                  queue as part of this function call for other
 *                  TB's retransmission.
 *               2. If already present in inUse Q then do not add.
 *     
 *     Invoked by: scheduler
 *     
 *  @param[in]  RgSchDlHqEnt     *hqE
 *  @param[in]  CmLteTimingInfo  timingInfo
 *  @param[out] RgDlHqProc       *hqP
 *  @param[in]  U8               tbIdx
 *  @return  Void      
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHDhmHqTbRetx
(
RgSchDlHqEnt            *hqE,
CmLteTimingInfo         timingInfo,
RgSchDlHqProcCb         *hqP,
U8                      tbIdx
)
#else
PUBLIC Void rgSCHDhmHqTbRetx(hqE, timingInfo, hqP, tbIdx)
RgSchDlHqEnt            *hqE;
CmLteTimingInfo         timingInfo;
RgSchDlHqProcCb         *hqP;
U8                      tbIdx;
#endif
{
   U8 othrTbIdx = tbIdx ^ 1;
   TRC2(rgSCHDhmHqTbRetx)

   hqP->tbInfo[tbIdx].timingInfo = timingInfo;

   /* rg001.301 - ADD - [ccpu00118633] fix for No Hq proc Avl start*/
   if (hqE->msg4Proc == hqP)
   {
       RETVOID;
   }
   /* fix for ccpu00118633 No Hq proc Avl end*/
      
   /* Extra:check if Harq process is already linked to in-use
      Queue by means of other TB handling. */
   if (hqP->tbInfo[othrTbIdx].state != HQ_TB_WAITING)
   {
      /*MS_FIX FIXME */
      if (hqE->msg4Proc != hqP)
      {
         cmLListAdd2Tail(&hqE->inUse, &hqP->lnk);
      }
   }

    hqP->tbInfo[tbIdx].cntrRetxAllocFail = 0;

   RETVOID;
} /* rgSCHDhmHqTbRetx */

/**
 * @brief This function returns last scheduled HARQ process for
 *        a UE's HARQ entity.
 *
 * @details
 *
 *     Function: rgSCHDhmLastSchedHqProc
 *     Purpose:  This function returns the last (most recent)
 *               process in the inUse list, which corresponds
 *               to the last scheduled process. Returns NULLP
 *               if list is empty.
 *               The reason for introducing this is to have
 *               an ability to check if UE was scheduled
 *               in the current subframe (scheduling would
 *               have caused a proc to be added to the end
 *               of the list, and checking time (only subframe
 *               number probably works) would confirm this.
 *
 *     Invoked by: scheduler
 *     
 *  @param[in]  RgSchDlHqEnt        *hqE
 *  @return  RgSchDlHqProcCb *
 *
 **/
#ifdef ANSI
PUBLIC RgSchDlHqProcCb * rgSCHDhmLastSchedHqProc
(
RgSchDlHqEnt            *hqE
)
#else
PUBLIC RgSchDlHqProcCb * rgSCHDhmLastSchedHqProc(hqE)
RgSchDlHqEnt            *hqE;
#endif
{
   TRC2(rgSCHDhmLastSchedHqProc);

   /* MS_FIX: UE context will not hold a valid hqE,
    * until rach procedure is completed */
   if ((hqE == NULLP) || (hqE->inUse.last == NULLP)) 
   {
      RETVALUE(NULLP);
   }
   RETVALUE((RgSchDlHqProcCb *)hqE->inUse.last->node);
} /* rgSCHDhmLastSchedHqProc */

#ifdef RGR_V1
/*rg009.201 - CR timer implementation changes*/
/**
 * @brief This function gets an available HARQ process for MSG 4.
 *
 * @details
 *
 *     Function: rgSCHDhmGetCcchSduHqProc
 *     Purpose:  This function returns an available HARQ process in 
 *               the DL direction. All HARQ processes are maintained
 *               in queues of free and inuse.
 *
 *               1. Check if the free queue is empty. If yes, return
 *                  RFAILED.
 *               2. If not empty, update the proc variable with the
 *                  first process in the queue. Return ROK.
 *     
 *     Invoked by: scheduler
 *     
 *  @param[in]  RgSchRaCb           *raCb
 *  @param[in]  CmLteTimingInfo  timingInfo
 *  @param[out] RgSchDlHqProcCb  **hqP
 *  @return  S16       
 *         -#ROK     if successful
 *         -#RFAILED otherwise
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDhmGetCcchSduHqProc
(
RgSchUeCb               *ueCb,
CmLteTimingInfo         timingInfo,
RgSchDlHqProcCb         **hqP
)
#else
PUBLIC S16 rgSCHDhmGetCcchSduHqProc (ueCb, timingInfo, hqP)
RgSchUeCb               *ueCb;
CmLteTimingInfo         timingInfo;
RgSchDlHqProcCb         **hqP;
#endif
{
   RgSchDlHqProcCb      *tmpHqProc;
   CmLList              *tmp;
   RgSchDlHqEnt         *hqE;

   TRC2(rgSCHDhmGetCcchSduHqProc)

   hqE = ueCb->dl.hqEnt;
   CM_LLIST_FIRST_NODE(&(hqE->free), tmp);
   if (NULLP == tmp)
   {
      /* No Harq Process available in the free queue. */
      RETVALUE(RFAILED);
   }

   /* Remove the element from the free Queue and */
   /* set the MSG 4 HARQ proc pointer            */
   cmLListDelFrm(&hqE->free, tmp);
   tmpHqProc             = (RgSchDlHqProcCb *)(tmp->node);
   tmpHqProc->tbInfo[0].timingInfo = timingInfo;
   /* MS_FIX : syed minor code reorg */
   *hqP = tmpHqProc;
   /*Updating ccchSduProc to identify feedback for CCCH SDU sent without 
    * Cont Res CE*/
   hqE->ccchSduProc         = tmpHqProc;
   cmLListAdd2Tail(&hqE->inUse, &tmpHqProc->lnk);
   
   RETVALUE(ROK);
} /* rgSCHDhmGetCcchSduHqProc */
#endif

/**
 * @brief This function gets an available HARQ process for MSG 4.
 *
 * @details
 *
 *     Function: rgSCHDhmGetMsg4HqProc
 *     Purpose:  This function returns an available HARQ process in 
 *               the DL direction. All HARQ processes are maintained
 *               in queues of free and inuse.
 *
 *               1. Check if the free queue is empty. If yes, return
 *                  RFAILED.
 *               2. If not empty, update the proc variable with the
 *                  first process in the queue. Return ROK.
 *     
 *     Invoked by: scheduler
 *     
 *  @param[in]  RgSchRaCb           *raCb
 *  @param[in]  CmLteTimingInfo  timingInfo
 *  @param[out]  RgDlHqProc       **hqP
 *  @return  S16       
 *         -#ROK     if successful
 *         -#RFAILED otherwise
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDhmGetMsg4HqProc
(
RgSchRaCb               *raCb,
CmLteTimingInfo         timingInfo
)
#else
PUBLIC S16 rgSCHDhmGetMsg4HqProc (raCb, timingInfo)
RgSchRaCb               *raCb;
CmLteTimingInfo         timingInfo;
#endif
{
   RgSchDlHqProcCb      *tmpHqProc;
   CmLList              *tmp;
   RgSchDlHqEnt         *hqE;

   TRC2(rgSCHDhmGetMsg4HqProc)

   hqE = raCb->dlHqE;
   CM_LLIST_FIRST_NODE(&(hqE->free), tmp);
   if (NULLP == tmp)
   {
      /* No Harq Process available in the free queue. */
      RETVALUE(RFAILED);
   }

   /* Remove the element from the free Queue and */
   /* set the MSG 4 HARQ proc pointer            */
   cmLListDelFrm(&hqE->free, tmp);
   tmpHqProc             = (RgSchDlHqProcCb *)(tmp->node);
   tmpHqProc->tbInfo[0].timingInfo = timingInfo;
   hqE->msg4Proc         = tmpHqProc;

   RETVALUE(ROK);
} /* rgSCHDhmGetMsg4HqProc */

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/* rg001.301 -MOD-[ccpu00118350] Correcting NDI manipulation of Harq */
/**
 * @brief This function releases a HARQ process.
 *
 * @details
 *
 *     Function: rgSCHDhmRlsHqpTb 
 *     Purpose:  This function resets the TB specific fields
 *               Based on the other TBs state, this HqProcess
 *               is returned to the HqEnt.
 *     
 *               1. Add the HARQ process to the free queue.
 *     Invoked by: scheduler and HARQ processing
 *     
 *  @param[in]  RgDlHqProc    *hqP
 *  @param[in]  U8            tbIdx
 *  @param[in]  Bool          togNdi 
 *  @return  Void      
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHDhmRlsHqpTb
(
RgSchDlHqProcCb         *hqP,
U8                      tbIdx,
Bool                    togNdi
) 
#else
PUBLIC Void rgSCHDhmRlsHqpTb(hqP, tbIdx, togNdi) 
RgSchDlHqProcCb         *hqP;
U8                      tbIdx;
Bool                    togNdi;
#endif
{
   RgSchDlHqEnt         *hqE;
   U8                   othrTbIdx = tbIdx ^ 1;

   TRC2(rgSCHDhmRlsHqpTb)

   /* Reset all tbInfo values */

   hqE = hqP->hqE;

#ifdef MAC_SCH_STATS
   if (hqP->hqE->ue != NULLP)
   {
      RgSchUeCb      *ueCb  = hqP->hqE->ue;
      RgSchCmnUe     *cmnUe = (RgSchCmnUe*)ueCb->sch;
      RgSchCmnDlUe   *dlUe  = &(cmnUe->dl);
      U8             cqi    = dlUe->mimoInfo.cwInfo[0].cqi;
      /* to get retransmission, decreasing transmission counter */
      U16            numDlRetx = hqP->tbInfo[0].txCntr-1; 
      U8             tbs       = dlUe->mimoInfo.cwInfo[0].iTbs[0];
      static U32 retxCnt1      = 0;

      RG_SCH_CMN_TBS_TO_MCS(tbs, \
            hqRetxStats.dlCqiStat[(cqi - 1)].mcs);

      switch (numDlRetx)
      {
         case 1:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_1++;
            break;
         case 2:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_2++;
            break;
         case 3:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_3++;
            break;
         case 4:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_4++;
            break;
      }
      hqRetxStats.dlCqiStat[(cqi - 1)].totalTx = \
             hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_1 + \
            (hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_2 * 2) + \
            (hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_3 * 3) + \
            (hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_4 * 4); 

      retxCnt1 += numDlRetx;
      /* MSPD_DBG("cqi=%d, mcs=%d, ueId=(%d), procId (%d), numDlRetx=%d , \
                  retxCnt=%d\n", cqi, hqRetxStats.dlCqiStat[(cqi - 1)].mcs, \
                     ueCb->ueId, hqP->procId, numDlRetx, retxCnt1);*/
   }
#endif /* MAC_SCH_STATS */
        /* rg001.301 -MOD-[ccpu00118350] Correcting NDI manipulation of Harq */
   /* Toggle ndi */
   if(togNdi == TRUE)
   {
      hqP->tbInfo[tbIdx].ndi          ^= 1;
   }

   /* Initialization */
   hqP->tbInfo[tbIdx].tbSz           = 0;
   hqP->tbInfo[tbIdx].numLch         = 0;
   hqP->tbInfo[tbIdx].txCntr         = 0;
   /* FOR ACK NACK REP */
   hqP->tbInfo[tbIdx].fbkRepCntr     = 0;
   hqP->tbInfo[tbIdx].fbkRecpRepCntr = 0;
   hqP->tbInfo[tbIdx].ackCount       = 0;
   hqP->tbInfo[tbIdx].subFrm         = NULLP;
   hqP->tbInfo[tbIdx].pdcch          = NULLP;
   hqP->tbInfo[tbIdx].state          = HQ_TB_ACKED;
   /* rg004.201 patch - ccpu00110457 DTX Changes Start */
   hqP->tbInfo[tbIdx].isAckNackDtx   = 0;
   hqP->tbInfo[tbIdx].nackCount      = 0;
   hqP->tbInfo[tbIdx].dtxCount       = 0;
   /* rg004.201 patch - ccpu00110457 DTX Changes End */
   /* rg007.201 -  Initializing TA */
   hqP->tbInfo[tbIdx].schdTa.pres    = NOTPRSNT;
   /* Handling msg4 hqProc */
   if (hqE->msg4Proc == hqP)
   {
      hqE->msg4Proc = NULLP;
      /* Add the proc to the free list */
      cmLListAdd2Tail(&hqE->free, &hqP->lnk);
      RETVOID;
   }
#ifdef RGR_V1
   /*rg009.201 - CR timer implementation changes*/
   /* MS_WORKAROUND : syed The check (hqE->ccchSduProc != NULLP)
    * is dangerous and it expects ccchSduProc is the first
    * DL allocation for a UE, and considering a scenario
    * of multiple UEs contending and 1 UE per TTI, this 
    * assumption can be wronged, leading to inUse list
    * corruption. Hence altering this check.
    * A better approach would be do avoid having this 
    * special handling for ccchSduProc, streamline 
    * it with the usual approach. */
   if (hqE->ccchSduProc == hqP)
   {
      hqE->ccchSduProc = NULLP;
	  /*MS_FIX for ccpu00123937*/
      cmLListDelFrm(&hqE->inUse, &hqP->lnk);
      /* Add the proc to the free list */
      cmLListAdd2Tail(&hqE->free, &hqP->lnk);
      RETVOID;
   }
#endif
      
   /* extra:check if other TB is also free for allocation then 
    * add it to FREE List */
   switch(hqP->tbInfo[othrTbIdx].state)
   {
      case HQ_TB_ACKED:
         /* Remove the element from the inUse Queue */
         /* rg003.301 -Add-[ccpu00120794] - Freeing up the HARQ proc blocked for
          * indefinite time in case of Retx */
         if (hqP->tbInfo[tbIdx].cntrRetxAllocFail != RG_SCH_MAX_RETX_ALLOC_FAIL)
         {
            cmLListDelFrm(&hqE->inUse, &hqP->lnk);
         }
         /* Add the proc to the free list */
         cmLListAdd2Tail(&hqE->free, &hqP->lnk);
         break;
         case HQ_TB_NACKED:
         /* Remove the element from the inUse Queue */
         /* rg003.301 -Add-[ccpu00120794] - Freeing up the HARQ proc blocked for
          * indefinite time in case of Retx */
         if (hqP->tbInfo[othrTbIdx].cntrRetxAllocFail == 0)
         {
            cmLListDelFrm(&hqE->inUse, &hqP->lnk);
         }
         break;
         case HQ_TB_WAITING:
            /* Do nothing */
         break;
   }
   
   /* rg003.301 -Add-[ccpu00120794] - Freeing up the HARQ proc blocked for
    * indefinite time in case of Retx */
   hqP->tbInfo[tbIdx].cntrRetxAllocFail = 0;

   RETVOID;
} /* rgSCHDhmRlsHqpTb */

/**
 * @brief This function releases a HARQ process.
 *
 * @details
 *
 *     Function: rgSCHDhmRlsHqProc
 *     Purpose:  This function returns a HARQ process to HARQ Entity 
 *               in the DL direction.
 *     
 *               1. Add the HARQ process to the free queue.
 *     Invoked by: scheduler and HARQ processing
 *     
 *  @param[in]  RgDlHqProc    *hqP
 *  @return  Void      
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHDhmRlsHqProc
(
RgSchDlHqProcCb         *hqP
)
#else
PUBLIC Void rgSCHDhmRlsHqProc(hqP)
RgSchDlHqProcCb         *hqP;
#endif
{
   RgSchDlHqEnt         *hqE;

   TRC2(rgSCHDhmRlsHqProc)

   hqE = hqP->hqE;

#ifdef MAC_SCH_STATS
    /* THIS FUNCTION IS NOT CALLED */
   if (hqP->hqE->ue != NULLP)
   {
      RgSchUeCb      *ueCb  = hqP->hqE->ue;
      RgSchCmnUe     *cmnUe = (RgSchCmnUe*)ueCb->sch;
      RgSchCmnDlUe   *dlUe  = &(cmnUe->dl);
      U8             cqi    = dlUe->mimoInfo.cwInfo[0].cqi;
      /* to get retransmission, decreasing transmission counter */
      U16            numDlRetx = hqP->tbInfo[0].txCntr-1; 
      U8             tbs       = dlUe->mimoInfo.cwInfo[0].iTbs[0];

      RG_SCH_CMN_TBS_TO_MCS(tbs, hqRetxStats.dlCqiStat[(cqi - 1)].mcs);
      /*MSPD_DBG("cqi=%d, mcs=%d, numDlRetx=%d \n", \
                  cqi, hqRetxStats.dlCqiStat[(cqi - 1)].mcs, numDlRetx);*/

      switch (numDlRetx)
      {
         case 1:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_1++;
            break;
         case 2:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_2++;
            break;
         case 3:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_3++;
            break;
         case 4:
            hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_4++;
            break;
      }
      hqRetxStats.dlCqiStat[(cqi - 1)].totalTx = \
             hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_1 + \
            (hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_2 * 2) + \
            (hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_3 * 3) + \
            (hqRetxStats.dlCqiStat[(cqi - 1)].numOfHQ_4 * 4);
   }
#endif /* MAC_SCH_STATS */
      cmLListDelFrm(&hqE->inUse, &hqP->lnk);
      cmLListAdd2Tail(&hqE->free, &hqP->lnk);
#ifdef TFU_UPGRADE
   /*rg002.301 - Del -  [ccpu00120233] rcptDone ADD - TB Count reset to 0 */
   hqP->tbCnt = 0;
#endif

   RETVOID;
} /* rgSCHDhmRlsHqProc */

#ifdef LTEMAC_SPS
/**
 * @brief This function gets HARQ process with the given ID.
 *
 * @details
 *
 *     Function: rgSCHDhmGetHqProcFrmId
 *     Purpose:  This function returns the HARQ process with the given ID.
 *     Invoked by: ROM
 *     
 *  @param[in]  RgSchUeCb     *ue
 *  @param[in]  U8            idx
 *  @param[in]  RgDlHqProc    **hqP
 *  @return  S16       
 *         -#   ROK     if successful
 *         -#   RFAILED otherwise
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDhmGetHqProcFrmId
(
RgSchUeCb               *ue,
U8                      idx,
RgSchDlHqProcCb         **hqP
)
#else
PUBLIC S16 rgSCHDhmGetHqProcFrmId(ue, idx, hqP)
RgSchUeCb               *ue;
U8                      idx;
RgSchDlHqProcCb         **hqP;
#endif
{
#ifdef DEBUGP
   Inst    inst = ue->cell->instIdx;
#endif
   TRC2(rgSCHDhmGetHqProcFrmId)

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Fetching the harq proc for "
               "index(%d)\n", idx));
   /* Pick the proc based on the index provided */
   *hqP = &(ue->dl.hqEnt->procs[idx]);

   RETVALUE(ROK);
} /* rgSCHDhmGetHqProcFrmId */

/**
 * @brief This function gets SPS HARQ process from the given time
 *
 * @details
 *
 *     Function: rgSCHDhmSpsDlGetHqProc
 *     Purpose:  This function returns the SPS HARQ process for the given time
 *     
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  CmLteTimingInfo timingInfo
 *  @return  RgSchDlHqProcCb control block 
 *
 **/
#ifdef ANSI
PUBLIC RgSchDlHqProcCb* rgSCHDhmSpsDlGetHqProc
(
RgSchUeCb               *ue,
CmLteTimingInfo         timingInfo
)
#else
PUBLIC RgSchDlHqProcCb* rgSCHDhmSpsDlGetHqProc(ue, timingInfo)
RgSchUeCb               *ue;
CmLteTimingInfo         timingInfo;
#endif
{
   U8                    idx;
   RgSchDlHqProcCb       *hqProc = NULLP;
   CmLList               *tmp = NULLP;

#ifdef DEBUGP
   Inst    inst = ue->cell->instIdx;
#endif
   TRC2(rgSCHDhmSpsDlGetHqProc);

   CM_LLIST_FIRST_NODE(&(ue->dl.hqEnt->free), tmp);

   if (NULLP == tmp)
   {
      RGSCHDBGINFO(inst, (rgSchPBuf(inst), "No Free Harq Procs Available.\n"));
      /* No Harq Process available in the free queue. */
      RETVALUE(NULLP);
   }

   idx = ((timingInfo.sfn * 10 + timingInfo.subframe)/
         ue->dl.dlSpsCfg.dlSpsPrdctyEnum) % ue->dl.dlSpsCfg.numSpsHqProc;

   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Fetching the SPS harq proc for "
            "index(%d)\n", idx));

   hqProc = (RgSchDlHqProcCb *)(tmp->node);

   /* If the  HARQ process is in the free list, retrieve the process */
   while (hqProc->procId != idx)
   {
      CM_LLIST_NEXT_NODE(&(ue->dl.hqEnt->free), tmp);
      if (!tmp)
      {
         break;
      }
      hqProc = (RgSchDlHqProcCb *)(tmp->node);
   }

   if (!tmp)
   {
      RGSCHDBGINFO(inst, (rgSchPBuf(inst), "No Free SPS Harq Procs Available.\n"));
      /* No Harq Process available in the free queue. */
      RETVALUE(NULLP);
   }

   hqProc->tbInfo[0].timingInfo = timingInfo;
   hqProc->tbInfo[1].timingInfo = timingInfo;

   /* Remove the element from the free Queue */
   cmLListDelFrm(&ue->dl.hqEnt->free, tmp);

   /* Add the element into the inUse Queue as well */
   cmLListAdd2Tail(&ue->dl.hqEnt->inUse, &hqProc->lnk);

   RETVALUE(hqProc);
} /* rgSCHDhmSpsDlGetHqProc */
#endif /* LTEMAC_SPS */


/*rg002.301 ccpu00119453 MOD changed handling in case maxhqretx is reached for TA */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/** * @brief Handler for handling TA. 
 *
 * @details
 *
 *     Function : rgSCHDhmFdbkIndHndlTa
 *     
 *     This function handles the TA state and values based on the 
 *     feedback indication received.
 *           
 *  @param[in]  RgSchDlHqProcCb   *hqP
 *  @param[in]  U8                tbIdx
 *  @param[in]  U8                fdbk
 *  @return     Void
 *      -# None.
 **/
#ifdef ANSI
PRIVATE Void rgSCHDhmFdbkIndHndlTa
(
RgSchDlHqProcCb   *hqP,
U8                tbIdx,
U8                fdbk,
Bool              maxHqRetxReached
)
#else
PRIVATE Void rgSCHDhmFdbkIndHndlTa(hqP, tbIdx, fdbk,maxHqRetxReached)
RgSchDlHqProcCb   *hqP;
U8                tbIdx;
U8                fdbk;
Bool              maxHqRetxReached;
#endif
{
   RgSchUeCb      *ueCb;
   RgSchCellCb    *cell;
#ifdef DEBUGP
   Inst           inst = cell->instIdx;
#endif
   TRC2(rgSCHDhmFdbkIndHndlTa)

   ueCb  = hqP->hqE->ue;
   cell  = ueCb->cell;

   switch(fdbk)
   {
      case TRUE:
         hqP->tbInfo[tbIdx].taSnt =  FALSE;
         ueCb->taLnk.node         =  (PTR)ueCb;

         cmLListAdd2Tail(&cell->taUeLst, &ueCb->taLnk);
         RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Ack Rcvd. Adding ue to TA "
              "timer list\n"));
         break;

      case FALSE:
         /* If Ta was sent and its the final NACK, then reset only the 
          * taState to IDLE and not the value */
         /*rg002.301 ccpu00119453 MOD changed handling in case maxhqretx is reached for TA */
         if(TRUE == maxHqRetxReached)
         {
            hqP->tbInfo[tbIdx].taSnt    =  FALSE;
            hqP->hqE->ue->dl.taCb.state =  RGSCH_TA_IDLE;

            rgSCHUtlDlTARpt(cell, ueCb); 
            RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Nack Rcvd for TA. Max Tries"
                " Attempted\n"));
         }
         break;
      /* rg004.201 patch - ccpu00110457 DTX Change */
      case TFU_HQFDB_DTX:
         /* If Ta was sent and its the final NACK, then reset only the 
          * taState to IDLE and not the value */
         /*rg002.301 ccpu00119453 MOD changed handling in case maxhqretx is reached for TA */
         if(TRUE == maxHqRetxReached)
         {
            hqP->tbInfo[tbIdx].taSnt    =  FALSE;
            hqP->hqE->ue->dl.taCb.state =  RGSCH_TA_IDLE;

            rgSCHUtlDlTARpt(cell, ueCb); 
            RGSCHDBGINFO(inst, (rgSchPBuf(inst), "DTX Rcvd for TA. Max Tries"
                " Attempted\n"));
         }
         break;

      default:
         break;
   }

   RETVOID;
} /* rgSCHDhmFdbkIndHndlTa */

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/* 3.1 MIMO: TA cmd details at TB level rather than Hq Level */
/** * @brief Handler for scheduling TA.
 *
 * @details
 *
 *     Function : rgSCHDhmShcdTa
 *     
 *     This function is called by scheduler when resource allocation
 *     for TA transmission is done.
 *           
 *  @param[in]  RgSchUeCb         *ue
 *  @param[out] RgSchDlHqTbCb     *tbInfo
 *  @return     Void
 *      -# None.
 **/
#ifdef ANSI
PUBLIC Void rgSCHDhmSchdTa
(
RgSchUeCb         *ueCb,
RgSchDlHqTbCb     *tbInfo
)
#else
PUBLIC Void rgSCHDhmSchdTa(ueCb, tbInfo)
RgSchUeCb         *ueCb;
RgSchDlHqTbCb     *tbInfo;
#endif
{

   TRC2(rgSCHDhmSchdTa)

   ueCb->dl.taCb.state = RGSCH_TA_SCHEDULED;
   /* rg008.201.ccpu00114300.lpr - taking 00113186 fixes in 
   mimo code ([ccpu00113186] Corrected  TA handling) */
/*   ueCb->dl.taCb.numRemSf  =
      (2 - (RG_UL_DELTA))<=0?0:(2-(RG_UL_DELTA)); */
    ueCb->dl.taCb.numRemSf  =  2; 
   tbInfo->schdTa.pres    = PRSNT_NODEF;
#ifdef MSPD
   MSPD_DBG(" *********** TA scheduled at MAC value : %d rnti %u hqP %u \n", 
   ueCb->dl.taCb.ta, ueCb->ueId, tbInfo->hqP->procId);
#endif
   tbInfo->schdTa.val     = ueCb->dl.taCb.ta;

   RETVOID;
} /* rgSCHDhmSchdTa */

#ifdef LTE_TDD
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/** * @brief Handler for fetching Harq Proc given the feeback information.
 *
 * @details
 *
 *     Function : rgSCHDhmHqProcByFdbkTime
 *
 *     This function shall fetch all the harq proc having the feedback
 *     timing information.
 *
 *  @param[in]  RgSchDlHqEnt       *hqE
 *  @param[in]  CmLteTimingInfo    timeInfo
 *  @param[in]  Bool               *isMsg4
 *  @param[out] RgSchDlHqProcCb    **hqPrcs
 *  @param[out] U8                 *numTbs
 *  @param[out] S8                 *tbStrtIdx
 *  @param[out] U8                 *cntHqPrcs
 *  @return     S16
 **/
#ifdef ANSI
PRIVATE S16 rgSCHDhmHqProcByFdbkTime
(
RgSchDlHqEnt            *hqE,
CmLteTimingInfo         timeInfo,
Bool                    *isMsg4,
RgSchDlHqProcCb         **hqPrcs,
U8                      *numTbs,
S8                      *tbStrtIdx,
U8                      *cntHqPrcs
)
#else
PRIVATE S16 rgSCHDhmHqProcByFdbkTime(hqE, timeInfo, isMsg4, hqPrcs,
                                     numTbs, tbStrtIdx, cntHqPrcs)
RgSchDlHqEnt            *hqE;
CmLteTimingInfo         timeInfo;
Bool                    *isMsg4;
RgSchDlHqProcCb         **hqPrcs;
U8                      *numTbs;
S8                      *tbStrtIdx;
U8                      *cntHqPrcs;
#endif
{
   CmLList              *tmp;
   RgSchDlHqProcCb      *hqP;

   *cntHqPrcs = 0;

   tmp = hqE->inUse.first;
   while(tmp != NULLP)
   {
      U8 i=0;
      hqP = (RgSchDlHqProcCb *)(tmp->node);
      numTbs[*cntHqPrcs] = 0;
      tbStrtIdx[*cntHqPrcs] = -1;
      for (i = 0; i < 2; i++)
      {
         /* Extra:check which TB is waiting for feedback */
         if ((hqP->tbInfo[i].state == HQ_TB_WAITING) && 
             (RGSCH_TIMEINFO_SAME(hqP->tbInfo[i].fdbkTime, timeInfo)))
         {
            if (tbStrtIdx[*cntHqPrcs] == -1)
            {
               tbStrtIdx[*cntHqPrcs] = i;
            }
            numTbs[*cntHqPrcs]++;
         }
      }
      if (numTbs[*cntHqPrcs] > 0)
      {
          hqPrcs[*cntHqPrcs] = hqP;
          (*cntHqPrcs)++;
      }
      tmp = tmp->next;
   }

   if (hqE->msg4Proc)
   {
      if (RGSCH_TIMEINFO_SAME(hqE->msg4Proc->tbInfo[0].fdbkTime, timeInfo))
      {
          *isMsg4 = TRUE;
          hqPrcs[*cntHqPrcs] = hqE->msg4Proc;
          tbStrtIdx[*cntHqPrcs] = 0;
          numTbs[*cntHqPrcs] = 1;
          (*cntHqPrcs)++;
          RETVALUE(ROK);
      }
   }

   RETVALUE(ROK);
}
#else  /* LTE_TDD */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/** * @brief Handler for fetching Harq Proc given the timming information.
 *
 * @details
 *
 *     Function : rgSCHDhmHqProcByTime
 *     
 *     This function shall fetch the harq proc using the timing information.
 *           
 *  @param[in]  RgSchDlHqEnt    *hqE
 *  @param[in]  CmLteTimingInfo timeInfo
 *  @param[in]  Bool            *isMsg4
 *  @param[out] U8              *numTbs
 *  @param[out] S8              *tbStrtIdx 
 *  @return     RgSchDlHqProcCb*
 *      -# RgSchDlHqProcCb* 
 *      -# NULLP 
 **/
#ifdef ANSI
PRIVATE RgSchDlHqProcCb *rgSCHDhmHqProcByTime
(
RgSchDlHqEnt            *hqE,
CmLteTimingInfo         timeInfo,
Bool                    *isMsg4,
U8                      *numTbs,
S8                      *tbStrtIdx
)
#else
PRIVATE RgSchDlHqProcCb *rgSCHDhmHqProcByTime(hqE, timeInfo,
                                              isMsg4, numTbs, tbStrtIdx)
RgSchDlHqEnt            *hqE;
CmLteTimingInfo         timeInfo;
Bool                    *isMsg4;
U8                      *numTbs;
S8                      *tbStrtIdx;
#endif
{
   CmLList              *tmp;
   RgSchDlHqProcCb      *hqP;
   U8                   tbCnt;

   if (hqE->msg4Proc)
   {
      if (RGSCH_TIMEINFO_SAME(hqE->msg4Proc->tbInfo[0].timingInfo, timeInfo))
      {
          *isMsg4    = TRUE;
          *numTbs    = 1;
          *tbStrtIdx = 0;
          RETVALUE(hqE->msg4Proc);
      }
   }

   tmp = hqE->inUse.first;
   while(tmp != NULLP)
   {
      hqP        = (RgSchDlHqProcCb *)(tmp->node);
      *numTbs    = 0;
      *tbStrtIdx = -1;
      for (tbCnt = 0; tbCnt < 2; tbCnt++)
      {
         if ((hqP->tbInfo[tbCnt].state == HQ_TB_WAITING) && 
             (RGSCH_TIMEINFO_SAME(hqP->tbInfo[tbCnt].timingInfo, timeInfo)))
         {
            /* extra: check if tbStrtIdx is already updated */
            if (*tbStrtIdx == -1)
            {
               *tbStrtIdx = tbCnt;
            }
            *numTbs = *numTbs + 1;
         }
      }
      if (*numTbs)
      {
         RETVALUE(hqP);
      }
      tmp = tmp->next;
   }

   RETVALUE(NULLP);
}
#endif

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/** * @brief Handler for handling the harq transaction failure.
 *
 * @details
 *
 *     Function : rgSCHDhmHqTbTrnsFail
 *     
 *     This function handled the harq TB transaction failure :
 *     - If retries have not reached maximum, add to the reTx Q.
 *     - else do error recovery.
 *           
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchDlHqProcCb         *hqP
 *  @param[in]  U8                      tbCnt
 *  @param[out] Bool                    *isMaxRetx
 *  @return    Void
 *             -#None.
 *      
 **/
#ifdef ANSI
PRIVATE Void rgSCHDhmHqTbTrnsFail
(
RgSchCellCb             *cell,
RgSchDlHqProcCb         *hqP,
U8                      tbCnt,
Bool                    *isMaxRetx
)
#else
PRIVATE Void rgSCHDhmHqTbTrnsFail(cell, hqP, tbCnt, isMaxRetx)
RgSchCellCb             *cell;
RgSchDlHqProcCb         *hqP;
U8                      tbCnt;
Bool                    *isMaxRetx;
#endif
{
   RgSchDlHqEnt         *hqE;
   U8                   maxHqTx;

   TRC2(rgSCHDhmHqTbTrnsFail)

   hqE = hqP->hqE;

   /* Fetch the maximum number of harq transmissions */
   if (hqE->msg4Proc == hqP)
   {
#ifdef RGR_V1
      /*rg009.201 - CR timer changes*/
      if(hqP->hqE->raCb->expiryTime.sfn == RGSCH_CONTRES_EXP)
      {
         rgSCHRamMsg4Done(cell, (RgSchRaCb *)hqP->hqE->raCb);
         RETVOID;
      }
#endif
      maxHqTx  = cell->dlHqCfg.maxMsg4HqTx;
   }
   else
   {
      maxHqTx  = hqE->maxHqTx;
   }

#ifdef MAC_SCH_STATS
   if (hqE->ue != NULLP)
   {
      RgSchUeCb      *ueCb  = hqE->ue;
      RgSchCmnUe     *cmnUe = (RgSchCmnUe*)ueCb->sch;
      RgSchCmnDlUe   *dlUe  = &(cmnUe->dl);
      U8             tbs    = dlUe->mimoInfo.cwInfo[0].iTbs[0];
      static U32 retxCnt    = 0;

      {
         ++retxCnt;
         /*MSPD_DBG("DL Nacks=%d cqi=%d, retxCnt=%d, procId=%d\n", \
         hqFailStats.dlCqiStat[(dlUe->mimoInfo.cwInfo[0].cqi - 1)].numOfNacks, \
            dlUe->mimoInfo.cwInfo[0].cqi, retxCnt, hqP->procId);*/
         hqFailStats.dlCqiStat[(dlUe->mimoInfo.cwInfo[0].cqi - 1)].numOfNacks++;
      }
      RG_SCH_CMN_TBS_TO_MCS(tbs, 
               (hqFailStats.dlCqiStat[(dlUe->mimoInfo.cwInfo[0].cqi - 1)].mcs));
   }
#endif /* MAC_SCH_STATS */ 

   if (hqP->tbInfo[tbCnt].txCntr < maxHqTx)
   {
      hqP->tbInfo[tbCnt].state = HQ_TB_NACKED;

#ifdef LTEMAC_DRX
      if((hqE->ue != NULLP) && (hqE->ue->isDrxEnabled == TRUE))
      {

         /*If DRX is enabled for the UE, we need to start the HARQ RTT timer
          * for the UE. Addtion to the retransmission queue will be done on
          * HARQ RTT timer expiry.--*/
         switch(hqP->tbInfo[tbCnt ^ 1].state)
         {
            case HQ_TB_ACKED:
               /*As the first TB is ACKED we have not started HARQ RTT for the
                * HqP, so start it here.*/
               cmLListDelFrm(&hqE->inUse, &hqP->lnk);
               rgSCHDrxStartHarqRTTTmr(cell, hqP, tbCnt);
               break;
            case HQ_TB_NACKED:
               /*As the first TB is NACKED we have already started HARQ RTT for the
                * HqP, so dont start it here, just delete from in use queue.*/
               cmLListDelFrm(&hqE->inUse, &hqP->lnk);
               break;
            case HQ_TB_WAITING:
               /*As this is the first TB to be fed back and is NACKED start
                * the HARQ RTT here.*/
               rgSCHDrxStartHarqRTTTmr(cell, hqP,tbCnt);
               break;
         }
         RETVOID;
      }
#endif /*LTEMAC_DRX*/
      /* extra:check if already removed as part of other TB processing
       * then donot remove from InUse Q */
      /* Check if other TB is not waiting for feedback.
       * Makinf sure hqP is present in inUse Queue until
       * it is fedback for all its TBs */
      switch(hqP->tbInfo[tbCnt ^ 1].state)
      {
         case HQ_TB_ACKED:
            /*Fix for ccpu00113296 - Do not delete for Msg4 Harq Entities*/
            if(hqE->msg4Proc != hqP)
            {
               if (!hqE->inUse.count || !hqP->lnk.node)
               {
                  MSPD_DBG("<BUG#71818> ue %u procId %u hqP->lnk %0x hqE->inUse %u hqE->free %u\n", 
				  	hqP->hqE->ue->ueId, hqP->procId, hqP->lnk.node, hqE->inUse.count, hqE->free.count);
				  MSPD_DBG("Caller %0x\n",(__return_address() - 4));
			   }
               cmLListDelFrm(&hqE->inUse, &hqP->lnk);
            }
            /* Retransmission needs to be done. Add to the scheduler Q */
            rgSCHUtlDlProcAddToRetx(cell, hqP);
            break;
         case HQ_TB_NACKED:
            /*Fix for ccpu00113296 - Do not delete for Msg4 Harq Entities*/
            if(hqE->msg4Proc != hqP)
            {
               if (!hqE->inUse.count || !hqP->lnk.node)
               {
                  MSPD_DBG("<BUG#71818> ue %u procId %u hqP->lnk %0x hqE->inUse %u hqE->free %u\n", 
				  	hqP->hqE->ue->ueId, hqP->procId, hqP->lnk.node, hqE->inUse.count, hqE->free.count);
				  MSPD_DBG("Caller %0x\n", (__return_address() - 4));
			   }
               cmLListDelFrm(&hqE->inUse, &hqP->lnk);
            }
            break;
         case HQ_TB_WAITING:
            /* Retransmission needs to be done. Add to the scheduler Q */
            rgSCHUtlDlProcAddToRetx(cell, hqP);
            break;
      }
      *isMaxRetx = FALSE; 
   }
   else
   {
      /* Failure Notification */
      if (hqE->msg4Proc == hqP)
      {
         /* Perform RAM MSG4 done processing */
         rgSCHRamMsg4Done(cell, (RgSchRaCb *)hqP->hqE->raCb);
      }
      else
      {
         /* Release the Harq Proc */
        /* rg001.301 -MOD-[ccpu00118350] Correcting NDI manipulation of Harq */
         rgSCHDhmRlsHqpTb(hqP, tbCnt, TRUE);
         /* Set the need for Aperiodic Cqi from UE */
         /*rg002.301 ccpu00119953 MOD added check if aprd CQI config is present*/
         if(hqE->ue->dl.ueDlCqiCfg.aprdCqiCfg.pres)
         {
            hqE->ue->dl.reqForCqi = TRUE;
         }
      }
      *isMaxRetx = TRUE; 
   }

   RETVOID;
} /* rgSCHDhmHqTbTrnsFail */

#ifdef LTE_TDD
#ifdef LTEMAC_SPS
/*rg002.301: ccpu00120264-MOD-changed HARQ Feedback handling code */
/** * @brief Handler for HARQ feedback received for DL transmission.
 *
 * @details
 *
 *     Function : rgSCHDhmHqFdbkInd
 *
 *     This function shall act on the feedback received from TOM for DL
 *     transmission. If the feedback for msg4 is final (after max transmissions
 *     or ACK) inform RAM that Msg4 transmission is done.
 *
 *
 *  @param[in]  Void                *cb
 *  @param[in]  U8                  cbType
 *  @param[in]  RgSchCellCb         cellCb
 *  @param[in]  CmLteTimingInfo     timeInfo 
 *  @param[in]  TfuHqInfo           *fdbk
 *  @param[in]  RgSchErrInfo        *err
 *  @return     S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDhmHqFdbkInd
(
Void                 *cb,
U8                   cbType,
RgSchCellCb          *cellCb,
CmLteTimingInfo      timeInfo,
TfuHqInfo            *fdbk,
RgInfRlsHqInfo       *rlsHqBufs,
RgSchErrInfo         *err
)
#else
PUBLIC S16 rgSCHDhmHqFdbkInd(cb, cbType, cellCb, timeInfo, fdbk, rlsHqBufs,
                                               err)
Void                    *cb;
U8                      cbType;
RgSchCellCb             *cellCb;
CmLteTimingInfo         timeInfo;
TfuHqInfo               *fdbk;
RgInfRlsHqInfo          *rlsHqBufs;
RgSchErrInfo            *err;
#endif
{
   RgSchDlHqEnt            *hqE;
   RgSchUeCb               *ue = NULLP;
   S16                     ret = ROK;
   RgSchDlSf               *sf;
   Bool                    isMsg4 = FALSE;
   RgSchRaCb               *raCb = NULLP;
   U16                     rnti=0;
   /* Maximum possible HARQ processes in UL-DL configuration 5 that is
    * given feedback at a time */
   RgSchDlHqProcCb         *hqPrcs[RGSCH_NUM_SUB_FRAMES-1] = {0};
   U8                      numTb[RGSCH_NUM_SUB_FRAMES-1];
   S8                      tbStrtIdx[RGSCH_NUM_SUB_FRAMES-1];
   U8                      hqCnt;
   U8                      idx;
   RgSchTddANInfo          *anInfo = NULLP;
   U8                      isAck;
   RgrTddAckNackMode       ackNackMode;
   Bool                    hqRls = FALSE;
   RgSchDlSf               *nxtDlsf = NULLP;
   U8                      rcvCnt = 0;
   CmLteTimingInfo         nxtfrm = {0,0};
   Bool                    anUpd = FALSE;
   RgSchTddDlAscSetIdxK    ascIdx;
   U8                      noFdbks;
   U8                      tmpIdx;
   U8                      hIdx;
   Bool                    isSpsRnti = FALSE;
   Bool                    isPusch   = FALSE;
   Bool                    hasRelPdcch = FALSE;
   U8                      tbCnt = 0;
   /*rg002.301 ccpu00119453 ADD added new var to chk maxhqretx*/
   Bool                    maxHqRetxReached = FALSE;
   /* Subframes in which transmissions are scheduled and whose feedback can come
    * in this subframe. Used only for Multiplexing mode */
   CmLteTimingInfo         schdSfTime[RGSCH_TDD_MAX_FDBK];

   TRC2(rgSCHDhmHqFdbkInd)

      if (cbType == RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB)
      {
         RGSCHDBGINFO(cellCb->instIdx,
               (rgSchPBuf(cellCb->instIdx), "FdbkInd invoked for RA Cb\n"));
         raCb     = (RgSchRaCb *)(cb);
         ackNackMode = RGR_TDD_ACKNACK_MODE_BUNDL;
         hqE      = raCb->dlHqE;
      }
      else
      {
         RGSCHDBGINFO(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), "FdbkInd invoked for UE\n"));
         ue       = (RgSchUeCb *)(cb);
         ackNackMode = ue->dl.ackNackMode;
         hqE      = ue->dl.hqEnt;
         /*rg002.301 - MOD - [ccpu00119223] Assigned the ue->ueId for rnti instead of rntiLnk->rnti */
         rnti     = ue->ueId;
         isSpsRnti = (ue->spsRnti == fdbk->rnti) ? TRUE: FALSE;
#ifdef TFU_UPGRADE
#ifdef LTEMAC_SPS
         isPusch = fdbk->isPusch;
#endif
#endif
         if (ue && RGSCH_TIMEINFO_SAME(timeInfo, ue->relPdcchFbkTiming))
         {
            hasRelPdcch = TRUE;
         }
      }

   /* Calculate the subframe time at which transmissions should have happened to
    * receive feedback in this subframe */
   if (ackNackMode == RGR_TDD_ACKNACK_MODE_MULT)
   {
      ascIdx = rgSchTddDlAscSetIdxKTbl[cellCb->ulDlCfgIdx][timeInfo.subframe];
      noFdbks = ascIdx.numFdbkSubfrms;
      for(idx=0; idx<noFdbks; idx++)
      {
         /*rg002.201 ccpu00120264-MOD-changed logic for filling up schdSfTime */
          RGSCHSUBFRMCRNTTIME(timeInfo,schdSfTime[idx],ascIdx.subfrmNum[idx]);
      }
   }

   rgSCHDhmHqProcByFdbkTime(hqE, timeInfo, &isMsg4, hqPrcs, numTb, \
         tbStrtIdx, &hqCnt);
#ifdef TFU_TDD
   rcvCnt = fdbk->M;
#endif
   /* Check if feedback came on configured UL SPS grant in Muxing mode */
  if((ackNackMode == RGR_TDD_ACKNACK_MODE_MULT) && 
        (isPusch == TRUE) )
   {
      hIdx = 0;
      /* Pick the valid feedbacks out of M feedbacks */
      for(idx=0; idx<noFdbks; idx++)
      {
         /* Skip the feedback if hqProc[idx] did not transmit in schdSf[idx] 
          * and no release pdcch is sent in prevTime */
         /* TODO: SPS_TDD: Check this while testing */
         if ((hqPrcs[hIdx] && !RGSCH_TIMEINFO_SAME(hqPrcs[hIdx]->\
                     tbInfo[(S16)(tbStrtIdx[hIdx])].timingInfo,\
                        schdSfTime[idx])) &&
               !RGSCH_TIMEINFO_SAME(ue->relPdcchTxTime, schdSfTime[idx]))
         { 
            /* Discard the feedback which is corresponding to a subframe in
             * which no DL transmission took place */
            tmpIdx = idx+1;
            while(tmpIdx < noFdbks)
            {
               fdbk->hqFbk[0][tmpIdx-1] = fdbk->hqFbk[0][tmpIdx];
               fdbk->hqFbk[1][tmpIdx-1] = fdbk->hqFbk[1][tmpIdx];
               tmpIdx++;
            }
            rcvCnt--;
            ++hIdx;
         }
      } /* end of for loop */
   } /* end of configured UL SPS grant check */
   
  if (hqPrcs[0])
   {
      if(hqPrcs[0]->hqE->ue)
      {
         anInfo = rgSCHUtlGetUeANFdbkInfo(hqPrcs[0]->hqE->ue, &timeInfo);
         if(anInfo == NULLP)
         {
            RGSCHDBGINFO(cellCb->instIdx,
                  (rgSchPBuf(cellCb->instIdx), 
                   "Ack Rcvd. No Ack/Nack feedback available \n"));
            RETVALUE(RFAILED);
         }
      }
   }

   /* rg002.301:ccpu00120264:-MOD-changed SPS TDD Harq fdbk code
    * to be more in line with normal TDD Harq feedback code */
   for(idx=0; (idx < hqCnt) && (idx < rcvCnt); idx++)
   {
      tbCnt = 0;
      rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs = 0;
      for (tbCnt = tbStrtIdx[idx]; (tbCnt-tbStrtIdx[idx]) < numTb[idx]; tbCnt++)
      {
         /* Remove the harq process from the subframe */
         sf = rgSCHUtlSubFrmGet(cellCb, hqPrcs[idx]->tbInfo[tbCnt].timingInfo);

         /* Process either the ACK received or max retries have occurred */
         /*Note: Only one codeword considered now */
         if(RGR_TDD_ACKNACK_MODE_BUNDL == ackNackMode)
         {
            /* Only one index is present */
#ifdef TFU_TDD
            if(anInfo != NULLP)
            {
               isAck = fdbk->hqFbk[tbCnt][anInfo->latestMIdx];
            }
            else
            {
               isAck = fdbk->hqFbk[tbCnt][hqPrcs[idx]->tbInfo[tbCnt].m];
            }
#endif   
         }
         else
         {
#ifdef TFU_TDD
            isAck = fdbk->hqFbk[tbCnt][hqPrcs[idx]->tbInfo[tbCnt].m];
#endif   
         }
         /* rg004.201 patch - ccpu00110457 DTX Change */
         /* DTX Change: Mentaining FB at the Harq level */
         hqPrcs[idx]->tbInfo[tbCnt].isAckNackDtx = isAck;
         if(cellCb->ulDlCfgIdx != 5)
         {
            rgSCHUtlGetNxtDlSfInfo(hqPrcs[idx]->tbInfo[tbCnt].timingInfo,\
                                   cellCb, sf, &nxtDlsf, &nxtfrm);
         }
         /* Keep a tab on how many ACKs or NACKs we have received */
         /* rg004.201 patch - ccpu00110457 DTX Changes Start */
         if (isAck == TFU_HQFDB_ACK)
         {
            hqPrcs[idx]->tbInfo[tbCnt].ackCount += 1; /* Ack counter */ 
            rgSCHUtlHdlUlTransInd(cellCb, ue, timeInfo);
         }
         else if (isAck == TFU_HQFDB_NACK)
         {
            hqPrcs[idx]->tbInfo[tbCnt].nackCount += 1; /* Nack Counter */ 
            rgSCHUtlHdlUlTransInd(cellCb, ue, timeInfo);
         }
         else
         {
            hqPrcs[idx]->tbInfo[tbCnt].dtxCount += 1;  /* DTX Counter*/
         }
         /* rg004.201 patch - ccpu00110457 DTX Changes End */
         /* Check if this is repeating UE */
         if (hqPrcs[idx]->tbInfo[tbCnt].fbkRepCntr != 0)
         {
            rgSCHUtlDlHqPTbRmvFrmTx(sf, hqPrcs[idx], tbCnt, TRUE);
            /* Check if last repetition */
            if (--hqPrcs[idx]->tbInfo[tbCnt].fbkRepCntr)
            {
               /* Update feedback time for this hqP TB so that 
                * next subframe its picked up */
               RGSCH_UPD_HQAN_FDBKTIME(&hqPrcs[idx]->tbInfo[tbCnt],\
                                       nxtDlsf, nxtfrm);
               RGSCH_UPD_ANINFO_WITH_HQ(anInfo, &hqPrcs[idx]->tbInfo[tbCnt]);
               anUpd = TRUE;
               continue;
            }
            /* For a repeating UE take the decision here */
            /* rg004.201 patch - ccpu00110457 DTX Changes Start */
            if (((hqPrcs[idx]->tbInfo[tbCnt].ackCount) > (hqPrcs[idx]->tbInfo[tbCnt].nackCount)) && 
               ((hqPrcs[idx]->tbInfo[tbCnt].ackCount) > (hqPrcs[idx]->tbInfo[tbCnt].dtxCount)))
            {
               isAck = TFU_HQFDB_ACK; 
            }
            else if (((hqPrcs[idx]->tbInfo[tbCnt].dtxCount) > (hqPrcs[idx]->tbInfo[tbCnt].nackCount)) && 
                  ((hqPrcs[idx]->tbInfo[tbCnt].dtxCount) > (hqPrcs[idx]->tbInfo[tbCnt].ackCount)))
            {
               isAck = TFU_HQFDB_DTX; 
            }
            else
            {
               isAck = TFU_HQFDB_NACK; 
            }
            hqPrcs[idx]->tbInfo[tbCnt].isAckNackDtx = isAck;
            /* rg004.201 patch - ccpu00110457 DTX Changes End */
         }
         else
         {
            rgSCHUtlDlHqPTbRmvFrmTx(sf, hqPrcs[idx], tbCnt, FALSE);
         }

         if(TFU_HQFDB_ACK == isAck)
         {
            if (isMsg4 == TRUE)
            {
               if (raCb == NULLP)
               {
                  raCb = rgSCHDbmGetRaCb(cellCb, rnti);
               }
			   #ifdef PWAV
			   MSPD_DBG("ACK received for MSG4:%d @ (%d, %d)\n", rnti, cellCb->crntTime.sfn, cellCb->crntTime.subframe);
			   #endif
               RGSCHDBGINFO(cellCb->instIdx,
                     (rgSchPBuf(cellCb->instIdx), "Ack Rcvd. FdbkInd for Msg4Done\n"));
               /* Inform Random Access Module regarding the ack received */
               if (raCb != NULLP)
               {
#ifdef RGR_V1
                  /*rg009.201 - CR timer implementation changes*/
                  /*Msg4 done, schedule if CCCH SDU received*/
                  /*MS_FIX:Use ue->dlCcchInfo.bo instead of additional ccchSduBo in raCb*/
                  ue = rgSCHDbmGetUeCb(cellCb, fdbk->rnti);
                  if((ue != NULLP ) && (ue->dlCcchInfo.bo))
                  {
                     rgSCHUtlAddUeToCcchSduLst(cellCb, ue);
                  }
                  
#endif

                  ret = rgSCHRamMsg4Done(cellCb, raCb);
               }
            }
            else /*ccpu00114124- HARQ Release for Msg4 */
            {
               /* Push this harq process back to the free queue */
               /* rg001.301 -MOD-[ccpu00118350] Correcting NDI manipulation of Harq */
               rgSCHDhmRlsHqpTb(hqPrcs[idx], tbCnt, TRUE);
            }
            hqRls = TRUE;
         }
         else
         {
#ifdef LTEMAC_SPS
            /*rg002.301 ccpu00119494-MOD-check for NULLP as this could be 
             * Msg4 DTX and Msg4 HQ proc would not be inUseLst */
            if((TFU_HQFDB_DTX == isAck) && 
                (hqPrcs[idx]->sch != (RgSchCmnDlHqProc *)NULLP) &&
                (RG_SCH_CMN_SPS_DL_IS_SPS_HQP(hqPrcs[idx]))
               )

            {
                rgSCHUtlDlProcDtx(cellCb, hqPrcs[idx]);
            }
            else
#endif
            {
                rgSCHDhmHqTbTrnsFail(cellCb, hqPrcs[idx], tbCnt, &hqRls);
                /*rg002.301 ccpu00119453 ADD updating the new var */
                maxHqRetxReached = hqRls ;
            }
         }
      }

      if(TRUE == hqRls)
      {
#ifdef LTE_L2_MEAS
         if (isAck)
         {
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].status[\
               rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = TRUE;
         }
         else
         {
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].status[\
               rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = FALSE;
         }
#endif /* Lte_L2_MEAS */
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].tbId[\
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = tbCnt + 1;
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs++;
      }

      /* Handle the TA */
      if (hqPrcs[idx]->tbInfo[tbCnt].taSnt == TRUE)
      {
         /*rg002.301 ccpu00119453 MOD*/
         rgSCHDhmFdbkIndHndlTa(hqPrcs[idx], tbCnt, isAck, maxHqRetxReached);
      }
      
      if(rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs)
      {
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].rnti = rnti;
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].hqProcId = 
            hqPrcs[idx]->procId;
         rlsHqBufs->numUes++;
      }
   }

  /*it is possible for some TDD configurations (like TDD cfg 5) 
   * to have multiple feedback for 13 subframes before. It is 
   * possible in such a case to have a release sent after data
   * thus running into a situation where we are receiving feedback
   * for both data and relese pdcch 
   */

   if ( (hqCnt == 0) ||
        (hasRelPdcch && rcvCnt > hqCnt)
      ) 
   {
      Bool found      = FALSE;
      U8   ulDlCfgIdx = 0;
      U8   maxFdbks   = 0;
      U8   itr        = 0;


      if ( hasRelPdcch )
      {
         sf = rgSCHUtlSubFrmGet(cellCb, ue->relPdcchTxTime);

         if ( !sf->relPdcch)
         {
            found = FALSE;
         }
          
         ulDlCfgIdx  = cellCb->ulDlCfgIdx;
         maxFdbks = rgSchTddDlAscSetIdxKTbl[ulDlCfgIdx]
                                          [timeInfo.subframe].
                                          numFdbkSubfrms;

         for(itr=0; itr< maxFdbks; itr++)
         {
            if ( rgSchTddDlAscSetIdxKTbl[ulDlCfgIdx][timeInfo.subframe].
                                             subfrmNum[itr]  == 
                                             ue->relPdcchTxTime.subframe )
            {
               rgSCHUtlDlRelPdcchFbk(cellCb, ue, fdbk->hqFbk[0][itr]);

               /* Remove release PDCCH from the subframe */
               rgSCHUtlPdcchPut(cellCb, &sf->pdcchInfo, sf->relPdcch);
               sf->relPdcch = NULLP;
               found = TRUE;
               break;
            }

         }/*end of for */

      }/*hasRelPdcch*/
      
     if ( found == FALSE ) 
      {
         RGSCHDBGERR(cellCb->instIdx,
                  (rgSchPBuf(cellCb->instIdx),"NO HARQ proc available for\
                   feedback: Ue %d timeInfo: snf %d,subframe %d\n",
                   ue->ueId,timeInfo.sfn, timeInfo.subframe));
         MSPD_ERR("NO HARQ proc available for\
                   feedback: Ue %d timeInfo: snf %d,subframe %d\n",
                   ue->ueId,timeInfo.sfn, timeInfo.subframe);
         err->errType   = RGSCHERR_DHM_FDBK_IND;
         err->errCause  = RGSCHERR_DHM_FDBK_IND_INVALID_CB;
         RETVALUE(RFAILED);
      }
   }/*if(hqCnt==0)*/

   /* Initialise the Ack/Nack feedback */
   if(hqPrcs[0] && (hqPrcs[0]->hqE->ue) && (!anUpd))
   {
      rgSCHUtlInitUeANFdbkInfo(anInfo);
   }


   RETVALUE(ROK);
}  /* rgSCHDhmHqFdbkInd */
#else /* LTEMAC_SPS */ /* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/*
 * @brief Handler for HARQ feedback received for DL transmission.
 *
 * @details
 *
 *     Function : rgSCHDhmHqFdbkInd
 *
 *     This function shall act on the feedback received from TOM for DL
 *     transmission. If the feedback for msg4 is final (after max transmissions
 *     or ACK) inform RAM that Msg4 transmission is done.
 *
 *
 *  @param[in]  Void                *cb
 *  @param[in]  U8                  cbType
 *  @param[in]  RgSchCellCb         cellCb
 *  @param[in]  CmLteTimingInfo     timeInfo 
 *  @param[in]  TfuHqInfo           *fdbk
 *  @param[in]  RgSchErrInfo        *err
 *  @return     S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDhmHqFdbkInd
(
Void                 *cb,
U8                   cbType,
RgSchCellCb          *cellCb,
CmLteTimingInfo      timeInfo,
TfuHqInfo            *fdbk,
RgInfRlsHqInfo       *rlsHqBufs,
RgSchErrInfo         *err
)
#else
PUBLIC S16 rgSCHDhmHqFdbkInd(cb, cbType, cellCb, timeInfo, fdbk, rlsHqBufs, err)
Void                    *cb;
U8                      cbType;
RgSchCellCb             *cellCb;
CmLteTimingInfo         timeInfo;
TfuHqInfo               *fdbk;
RgInfRlsHqInfo          *rlsHqBufs;
RgSchErrInfo            *err;
#endif
{
   RgSchDlHqEnt      *hqE;
   RgSchUeCb         *ue;
   S16               ret = ROK;
   RgSchDlSf         *sf;
   Bool              isMsg4 = FALSE;
   RgSchRaCb         *raCb = NULLP;
   U16               rnti=0;
   /* Maximum possible HARQ processes in UL-DL configuration 5 that is
    * given feedback at a time */
   RgSchDlHqProcCb   *hqPrcs[RGSCH_NUM_SUB_FRAMES-1];
   U8                numTb[RGSCH_NUM_SUB_FRAMES-1];
   S8                tbStrtIdx[RGSCH_NUM_SUB_FRAMES-1];
   U8                hqCnt;
   U8                idx;
   RgSchTddANInfo    *anInfo = NULLP;
   U8                isAck;
   RgrTddAckNackMode ackNackMode;
   Bool              hqRls = FALSE;
   RgSchDlSf         *nxtDlsf = NULLP;
   U8                rcvCnt = 0;
   CmLteTimingInfo   nxtfrm = {0,0};
   Bool              anUpd = FALSE;
   /*rg002.301 ccpu00119453 ADD added new var*/
   Bool              maxHqRetxReached;

   TRC2(rgSCHDhmHqFdbkInd)

   if (cbType == RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB)
   {
      RGSCHDBGINFO(cellCb->instIdx,
            (rgSchPBuf(cellCb->instIdx), "FdbkInd invoked for RA Cb\n"));
      raCb     = (RgSchRaCb *)(cb);
      ackNackMode = RGR_TDD_ACKNACK_MODE_BUNDL;
      hqE      = raCb->dlHqE;
   }
   else
   {
      RGSCHDBGINFO(cellCb->instIdx, (rgSchPBuf(cellCb->instIdx), "FdbkInd invoked for UE\n"));
      ue       = (RgSchUeCb *)(cb);
      ackNackMode = ue->dl.ackNackMode;
      hqE      = ue->dl.hqEnt;
      /*rg002.301 - MOD - [ccpu00119223] Assigned the ue->ueId for rnti instead of rntiLnk->rnti */
      rnti     = ue->ueId;
   }

   rgSCHDhmHqProcByFdbkTime(hqE, timeInfo, &isMsg4, hqPrcs, numTb, \
                            tbStrtIdx, &hqCnt);
   /* Fetch the harqProc from the inUse list */
   if(hqCnt == 0)
   {
      err->errType   = RGSCHERR_DHM_FDBK_IND;
      err->errCause  = RGSCHERR_DHM_FDBK_IND_INVALID_CB;
      RETVALUE(RFAILED);
   }

   if(hqPrcs[0]->hqE->ue)
   {
      anInfo = rgSCHUtlGetUeANFdbkInfo(hqPrcs[0]->hqE->ue, &timeInfo);
      if(anInfo == NULLP)
      {
        RGSCHDBGINFO(cellCb->instIdx,
              (rgSchPBuf(cellCb->instIdx), "Ack Rcvd. No Ack/Nack feedback available \n"));
        RETVALUE(RFAILED);
      }
   }
#ifdef TFU_TDD
   rcvCnt = fdbk->M;
#endif
   for(idx=0; (idx < hqCnt) && (idx < rcvCnt); idx++)
   {
      U8 tbCnt;
      rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs = 0;
      for (tbCnt = tbStrtIdx[idx]; (tbCnt-tbStrtIdx[idx]) < numTb[idx]; tbCnt++)
      {
         /* Remove the harq process from the subframe */
         sf = rgSCHUtlSubFrmGet(cellCb, hqPrcs[idx]->tbInfo[tbCnt].timingInfo);

         /* Process either the ACK received or max retries have occurred */
         /*Note: Only one codeword considered now */
         if(RGR_TDD_ACKNACK_MODE_BUNDL == ackNackMode)
         {
            /* Only one index is present */
#ifdef TFU_TDD
            if(anInfo != NULLP)
            {
               isAck = fdbk->hqFbk[tbCnt][anInfo->latestMIdx];
            }
            else
            {
               isAck = fdbk->hqFbk[tbCnt][hqPrcs[idx]->tbInfo[tbCnt].m];
            }
#endif   
         }
         else
         {
#ifdef TFU_TDD
            isAck = fdbk->hqFbk[tbCnt][hqPrcs[idx]->tbInfo[tbCnt].m];
#endif   
         }
         /* rg004.201 patch - ccpu00110457 DTX Change */
         /* DTX Change: Mentaining FB at the Harq level */
         hqPrcs[idx]->tbInfo[tbCnt].isAckNackDtx = isAck;
         if(cellCb->ulDlCfgIdx != 5)
         {
            rgSCHUtlGetNxtDlSfInfo(hqPrcs[idx]->tbInfo[tbCnt].timingInfo,\
                                   cellCb, sf, &nxtDlsf, &nxtfrm);
         }
         /* Keep a tab on how many ACKs or NACKs we have received */
         /* Keep a tab on how many ACKs or NACKs we have received */
         /* rg004.201 patch - ccpu00110457 DTX Changes Start */
         if (isAck == TFU_HQFDB_ACK)
         {
            hqPrcs[idx]->tbInfo[tbCnt].ackCount += 1; /* Ack counter */ 
            rgSCHUtlHdlUlTransInd(cellCb, ue, timeInfo);
         }
         else if (isAck == TFU_HQFDB_NACK)
         {
            hqPrcs[idx]->tbInfo[tbCnt].nackCount += 1; /* Nack Counter */ 
            rgSCHUtlHdlUlTransInd(cellCb, ue, timeInfo);
         }
         else
         {
            hqPrcs[idx]->tbInfo[tbCnt].dtxCount += 1;  /* DTX Counter*/
         }
         /* rg004.201 patch - ccpu00110457 DTX Changes End */
         /* Check if this is repeating UE */
         if (hqPrcs[idx]->tbInfo[tbCnt].fbkRepCntr != 0)
         {
            rgSCHUtlDlHqPTbRmvFrmTx(sf, hqPrcs[idx], tbCnt, TRUE);
            /* Check if last repetition */
            if (--hqPrcs[idx]->tbInfo[tbCnt].fbkRepCntr)
            {
               /* Update feedback time for this hqP TB so that 
                * next subframe its picked up */
               RGSCH_UPD_HQAN_FDBKTIME(&hqPrcs[idx]->tbInfo[tbCnt],\
                                       nxtDlsf, nxtfrm);
               RGSCH_UPD_ANINFO_WITH_HQ(anInfo, &hqPrcs[idx]->tbInfo[tbCnt]);
               anUpd = TRUE;
               continue;
            }
            /* For a repeating UE take the decision here */
            /* For a repeating UE take the decision here */
            /* rg004.201 patch - ccpu00110457 DTX Changes Start */
            if (((hqPrcs[idx]->tbInfo[tbCnt].ackCount) > (hqPrcs[idx]->tbInfo[tbCnt].nackCount)) && 
               ((hqPrcs[idx]->tbInfo[tbCnt].ackCount) > (hqPrcs[idx]->tbInfo[tbCnt].dtxCount)))
            {
               isAck = TFU_HQFDB_ACK; 
            }
            else if (((hqPrcs[idx]->tbInfo[tbCnt].dtxCount) > (hqPrcs[idx]->tbInfo[tbCnt].nackCount)) && 
                  ((hqPrcs[idx]->tbInfo[tbCnt].dtxCount) > (hqPrcs[idx]->tbInfo[tbCnt].ackCount)))
            {
               isAck = TFU_HQFDB_DTX; 
            }
            else
            {
               isAck = TFU_HQFDB_NACK; 
            }
            hqPrcs[idx]->tbInfo[tbCnt].isAckNackDtx = isAck;
            /* rg004.201 patch - ccpu00110457 DTX Changes End */
         }
         else
         {
            rgSCHUtlDlHqPTbRmvFrmTx(sf, hqPrcs[idx], tbCnt, FALSE);
         }

         if(TFU_HQFDB_ACK == isAck)
         {
            if (isMsg4 == TRUE)
            {
               if (raCb == NULLP)
               {
                  raCb = rgSCHDbmGetRaCb(cellCb, rnti);
               }
               RGSCHDBGINFO(cellCb->instIdx,
                     (rgSchPBuf(cellCb->instIdx), "Ack Rcvd. FdbkInd for Msg4Done\n"));
               /* Inform Random Access Module regarding the ack received */
               if (raCb != NULLP)
               {
#ifdef RGR_V1
                  /*rg009.201 - CR timer implementation changes*/
                  /*Msg4 done, schedule if CCCH SDU received*/
                  /*MS_FIX:Use ue->dlCcchInfo.bo instead of additional ccchSduBo in raCb*/
                  ue = rgSCHDbmGetUeCb(cellCb, fdbk->rnti);
                  if((ue != NULLP ) && (ue->dlCcchInfo.bo))
                  {
                     rgSCHUtlAddUeToCcchSduLst(cellCb, ue);
                  }
#endif

                  ret = rgSCHRamMsg4Done(cellCb, raCb);
               }
            }
            else /*ccpu00114124- HARQ Release for Msg4 */
            {
               /* Push this harq process back to the free queue */
               /* rg001.301 -MOD-[ccpu00118350] Correcting NDI manipulation of Harq */
               rgSCHDhmRlsHqpTb(hqPrcs[idx], tbCnt, TRUE);
            }
            hqRls = TRUE;
         }
         else
         {
            /*rg002.301 ccpu00119494-MOD-if this Msg4 DTX, there will be 
             * no DlHqProc as it has its own HarqProc */
#ifdef LTEMAC_SPS
            if((TFU_HQFDB_DTX == isAck) && 
                  (tmpHqProc->sch != (RgSchCmnDlHqProc *)NULLP) &&
                    (RG_SCH_CMN_SPS_DL_IS_SPS_HQP(hqPrcs[idx]))
              )

            {
                rgSCHUtlDlProcDtx(cellCb, hqPrcs[idx]);
            }
            else
#endif
            {
               rgSCHDhmHqTbTrnsFail(cellCb, hqPrcs[idx], tbCnt, &hqRls);
               /*rg002.301 ccpu00119453 ADD */
               maxHqRetxReached = hqRls;
            }
         }

         if(TRUE == hqRls)
         {
     /* MS_WORKAROUND: to increase Harq Fail Counter .
       The status field is required for tracking the number of harq faliures at MAC.
       As this is already present under L2_MEAS flag, we have replaced it with MSPD flag*/    
#ifdef MSPD
         if (isAck)
         {
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].status[\
               rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = TRUE;
         }
         else
         {
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].status[\
               rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = FALSE;
         }
#endif /* MSPD*/
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].tbId[\
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = tbCnt + 1;
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs++;
         }

         /* Handle the TA */
         if (hqPrcs[idx]->tbInfo[tbCnt].taSnt == TRUE)
         {
             /*rg002.301 ccpu00119453 MOD*/
            rgSCHDhmFdbkIndHndlTa(hqPrcs[idx], tbCnt, isAck, maxHqRetxReached);
         }
      }
      if(rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs)
      {
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].rnti = rnti;
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].hqProcId = 
            hqPrcs[idx]->procId;
         rlsHqBufs->numUes++;
      }
   }

   /* Initialise the Ack/Nack feedback */
   if((hqPrcs[0]->hqE->ue) && (!anUpd))
   {
      rgSCHUtlInitUeANFdbkInfo(anInfo);
   }


   RETVALUE(ROK);
}
#endif /* LTEMAC_SPS */

#else /* LTE_TDD */
/** * @brief Handler for HARQ feedback received for DL transmission.
 *
 * @details
 *
 *     Function : rgSCHDhmHqFdbkInd
 *     
 *     This function shall act on the feedback received from TOM for DL
 *     transmission. If the feedback for msg4 is final (after max transmissions
 *     or ACK) inform RAM that Msg4 transmission is done. 
 *     
 *           
 *  @param[in]  Void                *cb
 *  @param[in]  U8                  cbType
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgTfuHarqAckIndInfo *fdbk
 *  @param[in]  RgSchErrInfo        *err
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDhmHqFdbkInd
(
Void                 *cb, 
U8                   cbType,
RgSchCellCb          *cell,
CmLteTimingInfo      timingInfo,
RgTfuHqInfo          *fdbk,
RgInfRlsHqInfo       *rlsHqBufs,
RgSchErrInfo         *err
)
#else
PUBLIC S16 rgSCHDhmHqFdbkInd(cb, cbType, cell, timingInfo, fdbk, rlsHqBufs, err)
Void                    *cb;
U8                      cbType;
RgSchCellCb             *cell;
CmLteTimingInfo         timingInfo;
RgTfuHqInfo             *fdbk;
RgInfRlsHqInfo          *rlsHqBufs;
RgSchErrInfo            *err;
#endif
{
   RgSchDlHqEnt         *hqE;
   RgSchDlHqProcCb      *hqP;
  /* Create and Initialize Ue it so that Its not Deferenced Unnecessarily */
   RgSchUeCb            *ue = NULLP;

   S16                  ret = ROK;
   RgSchDlSf            *sf;
   Bool                 isMsg4 = FALSE;
   RgSchRaCb            *raCb = NULLP;
   U16                  rnti=0;
  /* rg009.201 Added Insure Fixes Of UR.Initialized procId  */
   U16                  procId=0;
   Bool                 hqRls=FALSE;
   Bool                 hqFreed =FALSE;
   /* DTX Change: Bool is converted into U8*/
   U8                   isAck = FALSE;
   U8                   numTbs;
   S8                   tbStrtIdx;
   U8                   tbCnt;
   /*rg002.301 ccpu00119453 ADD added new var*/
   Bool                 maxHqRetxReached = FALSE;
   RgSchCmnDlUe         *ueDl = NULLP;
#ifdef DEBUGP
   Inst    inst = cell->instIdx;
#endif

#ifdef LTEMAC_SPS
   CmLteTimingInfo      fdbkRcptTime = timingInfo;
#endif
   TRC2(rgSCHDhmHqFdbkInd)

   /* Get the subframe associated with the feedback */
   RGSCHSUBFRMCRNTTIME(timingInfo, timingInfo, 4); 

   if (cbType == RGSCH_HQ_FDB_IND_CB_TYPE_RA_CB)
   {
      raCb     = (RgSchRaCb *)(cb);
      hqE      = raCb->dlHqE;
      hqP      = rgSCHDhmHqProcByTime(hqE, timingInfo, &isMsg4,\
                                      &numTbs, &tbStrtIdx);
      rnti     = raCb->tmpCrnti;
   }
   else
   {
      ue       = (RgSchUeCb *)(cb);
      ueDl     = RG_SCH_CMN_GET_DL_UE(ue);
      hqE      = ue->dl.hqEnt;
      hqP      = rgSCHDhmHqProcByTime(hqE, timingInfo, &isMsg4,\
                                      &numTbs, &tbStrtIdx);
      /*rg002.301 - MOD - [ccpu00119223] Assigned the ue->ueId for rnti instead of rntiLnk->rnti */
      rnti     = ue->ueId;
   }
   /*
      TO ADD STATS 
      from Harq Proc get ueCb = hqP->hqEnt->ueCb
      from ueCb get cmnUecb = (RgSchCmnUe *)ueCb->sch;
      from ueCb get dlUe = (RgSchCmnDlUe)cmnUeCb->dl
      from get cmInfo "RgSchCmnDlUeCwInfo" dlUe->mimoInfo->cwInfo[0] 
      from get CQI from cmInfo->cqi 
      from cmInfo get iTbs cmInfo->iTbs[0]
      call RG_SCH_CMN_TBS_TO_MCS to map iTbs=>MCS 
      Update stats in cellCb
      cellCb->hqFailStats[cmInfo->cqi].mcs = RG_SCH_CMN_TBS_TO_MCS(cmInfo->iTbs[0]);
      if (fdbk->isAck == TRUE)
      cellCb->hqFailStats[cmInfo->cqi].numOfNacks += 1;
      else
      cellCb->hqFailStats[cmInfo->cqi].numOfAcks += 1;
      DL Ack/Nack statistics
    */	
#ifdef MAC_SCH_STATS
   if (hqE->ue != NULLP)
   {
      RgSchUeCb      *ueCb  = hqE->ue;
      RgSchCmnUe     *cmnUe = (RgSchCmnUe*)ueCb->sch;
      RgSchCmnDlUe   *dlUe  = &(cmnUe->dl);
      U8             tbs    = dlUe->mimoInfo.cwInfo[0].iTbs[0];
      static U32 retxCnt    = 0;

      if (fdbk->isAck[0] == TFU_HQFDB_ACK)
      {
         /*MSPD_DBG("DL Acks=%d cqi=%d, procId=%d\n", \
                 hqFailStats.dlCqiStat[(dlUe->mimoInfo.cwInfo[0].cqi - 1)].numOfAcks, \\
                 dlUe->mimoInfo.cwInfo[0].cqi, hqP->procId); */
         hqFailStats.dlCqiStat[(dlUe->mimoInfo.cwInfo[0].cqi - 1)].numOfAcks++;
      }
      else
      {
               ++retxCnt;
	     /*MSPD_DBG("DL Nacks=%d cqi=%d, retxCnt=%d, procId=%d\n", \
                hqFailStats.dlCqiStat[(dlUe->mimoInfo.cwInfo[0].cqi - 1)].numOfNacks, \
                   dlUe->mimoInfo.cwInfo[0].cqi, retxCnt, hqP->procId);*/
         hqFailStats.dlCqiStat[(dlUe->mimoInfo.cwInfo[0].cqi - 1)].numOfNacks++;
      }
      RG_SCH_CMN_TBS_TO_MCS(tbs, 
               (hqFailStats.dlCqiStat[(dlUe->mimoInfo.cwInfo[0].cqi - 1)].mcs));
   }
#endif /* MAC_SCH_STATS */

   /* Fetch the harqProc from the inUse list */
   if (hqP == NULLP)
   {
#ifdef LTEMAC_SPS
      /* Check if the feedback timing matches with ue->relPdcchFbkTiming*/
      /* Call Common module with the feedback information */
      if (ue && (ue->relPdcchFbkTiming.sfn != (RGSCH_MAX_SFN + 1)))
      {
         if (RGSCH_TIMEINFO_SAME(fdbkRcptTime, ue->relPdcchFbkTiming))
         {
            sf = rgSCHUtlSubFrmGet(cell, timingInfo);

            if (!sf->relPdcch)
            {
               RGSCHDBGERR(cell->instIdx,
                     (rgSchPBuf(cell->instIdx), "NO HARQ proc available for"
                      "feedback: Ue %d TimingInfo: sfn %d subframe %d", 
                      ue->ueId, timingInfo.sfn, timingInfo.subframe));
               MSPD_ERR("NO HARQ proc available for"
                      "feedback: Ue %d TimingInfo: sfn %d subframe %d", 
                      ue->ueId, timingInfo.sfn, timingInfo.subframe);
               RETVALUE(RFAILED);

            }

            isAck = fdbk->isAck[0];
            /* Note: Since relPdcchFbkTimimg matches with the recieved
             * feedback, assumed that feedback is for release PDCCH */
            rgSCHUtlDlRelPdcchFbk(cell, ue, isAck);

            /* Remove release PDCCH from the subframe */
            rgSCHUtlPdcchPut(cell, &sf->pdcchInfo, sf->relPdcch);
            sf->relPdcch = NULLP;
            RETVALUE(ROK);
         }
      }
#endif /* LTEMAC_SPS */
      err->errType   = RGSCHERR_DHM_FDBK_IND; 
      err->errCause  = RGSCHERR_DHM_FDBK_IND_INVALID_CB;
#ifdef MSPD
      MSPD_DBG("<HARQ> GET HQ Proc By time failed for [%d:%d] UE(%d)\n", timingInfo.sfn, timingInfo.subframe,rnti);
#endif
      RETVALUE(RFAILED);
   }

/*rg008.201 - Fix for Insure*/
   RGSCHDBGINFO(inst, (rgSchPBuf(inst), "rgSCHDhmHqFdbkInd():\
               HqProcId=%d",hqP->procId));
   procId = hqP->procId;
   /* Remove the harq process from the subframe */
   sf = rgSCHUtlSubFrmGet(cell, timingInfo);
   RGSCHADDTOCRNTTIME(timingInfo, timingInfo, 1);

   rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs = 0;
   for (tbCnt = tbStrtIdx; (tbCnt-tbStrtIdx) < numTbs; tbCnt++)
   {
      hqRls   = FALSE;
      hqFreed = FALSE;


      /* rg007.201: Added isAck assignment */
      isAck = fdbk->isAck[tbCnt];
      RGSCHDBGINFO(inst, (rgSchPBuf(inst), "rgSCHDhmHqFdbkInd():\
                   tbCnt=%d , isAck=%d",tbCnt,isAck));
      if (isAck == TFU_HQFDB_ACK)
      {
         hqP->tbInfo[tbCnt].ackCount += 1; /* Ack counter */ 
         if(!isMsg4)
         rgSCHUtlHdlUlTransInd(cell, ue, timingInfo);
      }
      else if (isAck == TFU_HQFDB_NACK)
      {
#ifdef MSPD
         MSPD_DBG("<HARQ> HqP[%d:%d] NACKED ue(%d)\n", hqP->procId, tbCnt, hqP->hqE->ue->ueId);
#endif
         hqP->tbInfo[tbCnt].nackCount += 1; /* Nack Counter */ 
         if(!isMsg4)
         rgSCHUtlHdlUlTransInd(cell, ue, timingInfo);
      }
      else
      {
#ifdef MSPD
	 MSPD_DBG("<HARQ> HqP[%d:%d] DTXED UE (%d)\n", hqP->procId, tbCnt,hqP->hqE->ue->ueId);
#endif
         hqP->tbInfo[tbCnt].dtxCount += 1;  /* DTX Counter*/
		 if(!isMsg4)
		 {
            ueDl->mimoInfo.cwInfo[tbCnt].dtxCnt++;
		 }
      }

      /* Check if this is repeating UE */
      if (hqP->tbInfo[tbCnt].fbkRepCntr != 0)
      {
         if((rgSCHDhmProcHqFdbkAckNackRep(hqP,sf,tbCnt,&isAck)) != ROK)
         {
             continue;
         }
      }
      else
      {
         /* For a Normal UE take the decision here */
         hqP->tbInfo[tbCnt].isAckNackDtx = fdbk->isAck[tbCnt];
         isAck = fdbk->isAck[tbCnt];
         rgSCHUtlDlHqPTbRmvFrmTx(sf, hqP, tbCnt, FALSE);
      }

      /* rg001.201: To be changed for MIMO */
      /* Process either the ACK received or max retries have occurred */
      /* Assuming for Repetition that 2 ACKs and 2 NACKs make an NACK */
      if (TFU_HQFDB_ACK == isAck)
      {
         if (isMsg4 == TRUE) 
         {
            if (raCb == NULLP)
            {
               raCb = rgSCHDbmGetRaCb(cell, rnti);
            }
            RGSCHDBGINFO(cell->instIdx,
                  (rgSchPBuf(cell->instIdx), "Ack Rcvd. FdbkInd for Msg4Done\n"));
            /* Inform Random Access Module regarding the ack received */
            if (raCb != NULLP)
            {
#ifdef RGR_V1
               /*rg009.201 - CR timer implementation changes*/
               /*Msg4 done, schedule if CCCH SDU received*/
               /*MS_FIX:Use ue->dlCcchInfo.bo instead of additional ccchSduBo in raCb*/
               ue = rgSCHDbmGetUeCb(cell, fdbk->rnti);
               if((ue != NULLP ) && (ue->dlCcchInfo.bo))
               {
                  rgSCHUtlAddUeToCcchSduLst(cell, ue);
               }
#endif
               ret = rgSCHRamMsg4Done(cell, raCb);
               hqFreed = TRUE;
            }
         }
         else /*ccpu00114124- HARQ Release for Msg4 */
         {
            /* Push this harq process back to the free queue */
            /* rg001.301 -MOD- [ccpu00118350] : Correcting NDI manipulation of Harq */
            ueDl->mimoInfo.cwInfo[tbCnt].ackCnt++;
            rgSCHDhmRlsHqpTb(hqP, tbCnt, TRUE);
         }
         hqRls = TRUE;
      }
      else
      {
#ifdef LTEMAC_SPS
       /*rg002.301 ccpu00119494-MOD-check for NULLP as this could be Msg4 DTX
        * and Msg4 HQ proc would not be inUseLst */
         if( TFU_HQFDB_DTX == isAck &&
            (hqP->sch != (RgSchCmnDlHqProc *)NULLP) &&
            (RG_SCH_CMN_SPS_DL_IS_SPS_HQP(hqP))
           )

         {
            rgSCHUtlDlProcDtx(cell, hqP);
         }
         else
#endif
         {
            if(!isMsg4)
            {
               ueDl->mimoInfo.cwInfo[tbCnt].nackCnt++;
            }
            rgSCHDhmHqTbTrnsFail(cell, hqP, tbCnt, &hqRls);
            /*rg002.301 ccpu00119453 ADD*/
            maxHqRetxReached = hqRls;
         }
      }

      if(hqRls == TRUE)
      {
      /* MS_WORKAROUND: to increase Harq Fail Counter .
       The status field is required for tracking the number of harq faliures at MAC.
       As this is already present under L2_MEAS flag, we have replaced it with MSPD flag*/
#ifdef MSPD
         if (isAck)
         {
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].status[\
               rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = TRUE;
         }
         else
         {
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].status[\
               rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = FALSE;
         }
#endif /* MSPD */
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].tbId[\
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs] = tbCnt + 1;
         rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs++;
      }
      
      /* Handle the TA */
      if (hqFreed == FALSE && hqP->tbInfo[tbCnt].taSnt == TRUE)
      {
          /*rg002.301 ccpu00119453 MOD*/
         rgSCHDhmFdbkIndHndlTa(hqP, tbCnt, (U8)fdbk->isAck[tbCnt], maxHqRetxReached);
      }
   }
   if(rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs)
   {
      rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].rnti = rnti;
      rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].hqProcId = 
         (U8)procId;
      rlsHqBufs->numUes++;
   }
   RETVALUE(ret);
}  /* rgSCHDhmHqFdbkInd */
#endif /* ifdef LTE_TDD */


/**
 * @brief Handler for Harq related UE configuration.
 *
 * @details
 *
 *     Function : rgSCHDhmRgrUeCfg
 *     
 *     This function shall fetch the harq related information into the
 *     respective ueCb from the UE configuration as provided by the 
 *     upper layers. 
 *           
 *  @param[in]  RgSchCellCb   *cell 
 *  @param[in]  RgSchUeCb     *ueCb 
 *  @param[in]  RgrUeCfg      *ueCfg 
 *  @param[out] RgSchErrInfo     *err
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC Void rgSCHDhmRgrUeCfg
(
RgSchCellCb       *cell,
RgSchUeCb         *ueCb,
RgrUeCfg          *ueCfg,
RgSchErrInfo      *err
)
#else
PUBLIC Void rgSCHDhmRgrUeCfg(cell, ueCb, ueCfg, err)
RgSchCellCb       *cell;
RgSchUeCb         *ueCb;
RgrUeCfg          *ueCfg;
RgSchErrInfo      *err;
#endif
{
   TRC2(rgSCHDhmRgrUeCfg)

   UNUSED(err);

   /* Initialize the TA Timer */
   cmInitTimers(&ueCb->taTmr, 1);

   /* rg006.201: [ccpu00113186] Corrected  TA handling */
   /* Setting these values irrespective of taTmr value */
   ueCb->dl.taCb.state = RGSCH_TA_IDLE;
   /*rg002.301: [ccpu00118468]-MOD-corrected default value of TA
    * as per 36.213, 4.2.3 */
   ueCb->dl.taCb.ta = RGSCH_NO_TA_RQD;
   /* Start TA timer only if cfgd as FINITE value */
   if (ueCfg->ueTaTmrCfg.pres)
   {
      /* Configuring taTmr with 30 deficit, to enable eNodeB sending
       * TA command before the expiry of TA at UE. Also considering for 
       * possible retx for this TA command */
      ueCb->dl.taCb.cfgTaTmr = ueCfg->ueTaTmrCfg.taTmr - 30; 
      rgSCHTmrStartTmr (cell, ueCb, RG_SCH_TMR_TA, ueCb->dl.taCb.cfgTaTmr);
   }
   RETVOID;
}  /* rgSCHDhmRgrUeCfg */
 
/**
 * @brief Handler for HARQ related UE Reconfiguration.
 *
 * @details
 *
 *     Function : rgSCHDhmRgrUeRecfg
 *     
 *     This function shall fetch the HARQ related information into the
 *     respective ueCb from the UE configuration as provided by the 
 *     upper layers. 
 *           
 *  @param[in]  RgSchCellCb   *cell 
 *  @param[in]  RgSchUeCb     *ueCb 
 *  @param[in]  RgrUeCfg      *ueCfg 
 *  @param[out] RgSchErrInfo     *errInfo 
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC Void rgSCHDhmRgrUeRecfg
(
RgSchCellCb       *cell,
RgSchUeCb         *ueCb,
RgrUeRecfg        *ueCfg,
RgSchErrInfo      *err
)
#else
PUBLIC Void rgSCHDhmRgrUeRecfg(cell, ueCb, ueCfg, err)
RgSchCellCb       *cell;
RgSchUeCb         *ueCb;
RgrUeRecfg        *ueCfg;
RgSchErrInfo      *err;
#endif
{
   TRC2(rgSCHDhmRgrUeRecfg)

   UNUSED(err);

   /* Update the DL Harq related information */
   ueCb->dl.hqEnt->maxHqTx = cell->dlHqCfg.maxDlHqTx;

   /* Update the TA related information */
   
   if (ueCfg->ueTaTmrRecfg.pres)
   {
      /* rg006.201: [ccpu00113186] Corrected  TA handling */
      /* Configuring taTmr with 30 deficit, to enable eNodeB sending
       * TA command before the expiry of TA at UE. Also considering for 
       * possible retx for this TA command */
      ueCb->dl.taCb.cfgTaTmr = ueCfg->ueTaTmrRecfg.taTmr - 30;
      /* If Tatmr was previously configured as Infinity then start Tmr now */
      if (ueCb->taTmr.tmrEvnt == TMR_NONE)
      {
         rgSCHTmrStartTmr (cell, ueCb, RG_SCH_TMR_TA, ueCb->dl.taCb.cfgTaTmr);
      }
   }
   RETVOID;
}  /* rgSCHDhmRgrUeRecfg */
/**
 * @brief Handler for HARQ related UE Reconfiguration
 *
 * @details
 *
 *     Function : rgSCHDhmRgrCellCfg
 *     
 *     This function shall fetch the HARQ related information into the
 *     respective ueCb from the UE configuration as provided by the 
 *     upper layers. 
 *           
 *  @param[in]  RgSchCellCb     *cell 
 *  @param[in]  RgrCellCfg   *cellCfg
 *  @param[out] RgSchErrInfo    *err 
 *
 *  @return     S16
 *                -# ROK 
 *                -# RFAILED 
 **/
#ifdef ANSI
PUBLIC Void rgSCHDhmRgrCellCfg
(
RgSchCellCb    *cell,
RgrCellCfg     *cellCfg,
RgSchErrInfo   *err
)
#else
PUBLIC Void rgSCHDhmRgrCellCfg(cell, cellCfg, err)
RgSchCellCb    *cell;
RgrCellCfg     *cellCfg;
RgSchErrInfo   *err;
#endif
{
   PTR         pUeCb;/* previous UE Control block */
   PTR         nUeCb;/* next UE control block */
   S16         ret;
   U8          idx;

   TRC2(rgSCHDhmRgrCellCfg)

   UNUSED(err);

   pUeCb = NULLP;
   
   cell->dlHqCfg = cellCfg->dlHqCfg;
   for (;;)
   {
      ret = cmHashListGetNext(&(cell->ueLst), pUeCb, &nUeCb);
      if (ret != ROK)
      {
         break;
      }
      else
      {
         pUeCb = nUeCb;
         /* Update the DL Harq related information */
         ((RgSchUeCb *)nUeCb)->dl.hqEnt->maxHqTx = cell->dlHqCfg.maxDlHqTx;
      }
   }
   /* Initializing the list for ueCbs that would have ta */
   cmLListInit(&cell->taUeLst);
#ifdef RGR_V1
   /*rg009.201 - CR timer implementation changes*/
   cmLListInit(&cell->ccchSduUeLst);
   cmLListInit(&cell->contResGrdTmrLst);
   cmLListInit(&cell->contResTmrLst);
#endif

   /* Initializing the timer queue */
   cell->tqCp.nxtEnt = 0;
   cell->tqCp.tmrLen = RGSCH_UE_TQ_SIZE;

   for (idx = 0; idx < RGSCH_UE_TQ_SIZE; idx++)
   {
     cell->tq[idx].first = NULLP;
     cell->tq[idx].tail  = NULLP;
   }
   RETVOID;
}  /* rgSCHDhmRgrCellCfg */

/**
 * @brief Handler for Updating HARQ Information from Cell Reconfiguration 
 *
 * @details
 *
 *     Function : rgSCHDhmRgrCellRecfg
 *     
 *     This function shall fetch the HARQ related information into the
 *     respective ueCb from the UE configuration as provided by the 
 *     upper layers. 
 *           
 *  @param[in]  RgSchCellCb     *cell 
 *  @param[in]  RgrCellRecfg *cellRecfg
 *  @param[out] RgSchErrInfo    *err 
 *
 *  @return     S16
 *                -# ROK 
 *                -# RFAILED 
 **/
#ifdef ANSI
PUBLIC Void rgSCHDhmRgrCellRecfg
(
RgSchCellCb    *cell,
RgrCellRecfg   *cellRecfg,
RgSchErrInfo   *err
)
#else
PUBLIC Void rgSCHDhmRgrCellRecfg(cell, cellRecfg, err)
RgSchCellCb    *cell;
RgrCellRecfg   *cellRecfg;
RgSchErrInfo   *err;
#endif
{
   PTR         pUeCb;/* previous UE Control block */
   PTR         nUeCb;/* next UE control block */
   S16         ret;

   TRC2(rgSCHDhmRgrCellRecfg)

   UNUSED(err);

   pUeCb = NULLP;
   
   /* Update the cell with recieved configuration */
   if (cellRecfg->recfgTypes & RGR_CELL_DL_HARQ_RECFG)
   {
      cell->dlHqCfg  =  cellRecfg->dlHqRecfg;

      for (;;)
      {
         ret = cmHashListGetNext(&(cell->ueLst), pUeCb, &nUeCb);
         if (ret != ROK)
         {
            break;
         }
         else
         {
            pUeCb = nUeCb;
            /* Update the DL Harq related information */
            ((RgSchUeCb *)nUeCb)->dl.hqEnt->maxHqTx = cell->dlHqCfg.maxDlHqTx;
         }
      }
   }
   RETVOID;
}  /* rgSCHDhmRgrCellRecfg */

/**
 * @brief Handler for freeing up the HARQ related information from ueCb
 *
 * @details
 *
 *     Function : rgSCHDhmFreeUe
 *     
 *     This function shall free up the HARQ specific information from ueCb.
 *           
 *  @param[in]  RgSchUeCb     *ueCb 
 *
 *  @return     None.
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHDhmFreeUe
(
RgSchUeCb          *ueCb
)
#else
PUBLIC Void rgSCHDhmFreeUe(ueCb)
RgSchUeCb          *ueCb;
#endif
{

   TRC2(rgSCHDhmFreeUe)

   /* If TA Timer is running. Stop it */
   if (ueCb->taTmr.tmrEvnt != TMR_NONE)
   {
      rgSCHTmrStopTmr(ueCb->cell, ueCb->taTmr.tmrEvnt, ueCb);
   }

   /* ccpu00118357 - ADD - stop the periodic BSR timer so it 
    * doesn't expire after UE is deleted */
#ifdef RGR_V1
   if (ueCb->bsrTmr.tmrEvnt != TMR_NONE)
   {
      rgSCHTmrStopTmr(ueCb->cell, ueCb->bsrTmr.tmrEvnt, ueCb);
   }
#endif /* ifdef RGR_V1*/

   if (ueCb->dl.hqEnt)
   {
#ifdef LTE_TDD
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(ueCb->cell->instIdx, (Data **)(&(ueCb->dl.hqEnt->procs)),
            sizeof(RgSchDlHqProcCb) * ueCb->dl.hqEnt->numHqPrcs);
#endif
      rgSCHDhmDelHqEnt(ueCb->cell, &ueCb->dl.hqEnt);
   }

   /* This UE needs to be removed from its entry into cell's taUeLst */
   /*Fix for ccpu00113622 - Delete Only when taLnk Node exists*/
   if(ueCb->taLnk.node)
   {
      cmLListDelFrm(&(ueCb->cell->taUeLst), &ueCb->taLnk);
      ueCb->taLnk.node = NULLP; 
   }

    /* rg002.301-ADD-[ccpu00120284] UE should be removed from the TaLst */
   if (ueCb->dlTaLnk.node != NULLP)
   {
      /* MS_FIX: syed Need to raise a CR for not calling CMN or specific scheduler
       * function directly from other modules. APIs should be defined and/or used
       * instead. Please check for other possible incorrect usage. */	   
      rgSCHCmnRmvFrmTaLst(ueCb->cell, ueCb);
   }

   RETVOID;

}  /* rgSCHDhmFreeUe */

/**
 * @brief Handler for updating the TA.
 *
 * @details
 *
 *     Function : rgSCHDhmUpdTa
 *     
 *     This function shall update the TA received. 
 *           
 *  @param[in]  RgSchCellCb   *cell 
 *  @param[in]  RgSchUeCb     *ueCb 
 *  @param[in]  U8         ta
 *
 *  @return     None.
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHDhmUpdTa
(
RgSchCellCb    *cell,
RgSchUeCb      *ueCb,
U8             ta
)
#else
PUBLIC Void rgSCHDhmUpdTa(cell, ueCb, ta)
RgSchCellCb    *cell;
RgSchUeCb      *ueCb;
U8             ta;
#endif
{
   TRC2(rgSCHDhmUpdTa)

   if (ueCb->dl.taCb.state == RGSCH_TA_IDLE)
   {
      ueCb->dl.taCb.state     =  RGSCH_TA_TOBE_SCHEDULED;
      ueCb->dl.taCb.numRemSf  =  2;
      rgSCHUtlDlTARpt(cell, ueCb);  
      /* If TA Timer is running. Stop it */
      if (ueCb->taTmr.tmrEvnt != TMR_NONE)
      {
         rgSCHTmrStopTmr(cell, ueCb->taTmr.tmrEvnt, ueCb);
      }
   }
   ueCb->dl.taCb.ta        =  ta;

   RETVOID;
}  /* rgSCHDhmUpdTa */

 /** @brief This function handles the TA timer expiry.
  *
  * @details
  *
  *     Function: This function handled the TA Expiry.
  *
  *         Processing steps:
  *         - 
  *
  *
  * @param[in] RgSchUeCb  *ueCb  
  *
  * @return Void
  *      -#None.
  */
#ifdef ANSI
PUBLIC Void rgSCHDhmProcTAExp 
(
 RgSchUeCb  *ueCb
 )
#else
PUBLIC Void rgSCHDhmProcTAExp (ueCb)
 RgSchUeCb  *ueCb;
#endif
{
   TRC2(rgSCHDhmProcTAExp);

   /* Ask scheduler to schedule this UE */
   ueCb->dl.taCb.state     =  RGSCH_TA_TOBE_SCHEDULED;
   rgSCHUtlDlTARpt(ueCb->cell, ueCb);  
   RETVOID;
} /* end of rgSCHDhmProcTAExp */ 

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/* 3.1 MIMO: LC details at TB level rather than Hq Level */
/**
 * @brief Handler for Adding scheduled logical channel data information  
 *        to harqProc.
 *                    
 * @details
 *
 *     Function : rgSCHDhmAddLcData
 *     
 *     This function shall add the scheduled logical channel data 
 *     information to the HARQ process. 
 *           
 *  @param[in]  RgSchLchAllocInfo  *lchData
 *  @param[in]  RgSchDlHqTbCb     *tbInfo
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDhmAddLcData
(
Inst               inst,
RgSchLchAllocInfo  *lchData,
RgSchDlHqTbCb      *tbInfo
)
#else
PUBLIC S16 rgSCHDhmAddLcData(inst, lchData, tbInfo)
Inst               inst;
RgSchLchAllocInfo  *lchData;
RgSchDlHqTbCb      *tbInfo;
#endif
{

   TRC2(rgSCHDhmAddLcData)

   if(tbInfo->numLch >= RGSCH_MAX_NUM_DED_LC)
   {
     RETVALUE(RFAILED);
   } 

   tbInfo->lchSchdDataArr[tbInfo->numLch]   = *lchData;

   tbInfo->numLch++;

   RETVALUE(ROK);

}  /* rgSCHDhmAddLcData */

#ifdef LTE_TDD
/*
 * @brief Handler for releaseing the subframe allocation.
 *
 * @details
 *
 *     Function : rgSCHDhmTddRlsSubFrm
 *
 *     This function shall be invoked to release the DL Sf 
 *     allocations for which HARQ feedback time has expired.
 *
 *  @param[in]  RgSchCellCb      *cellCb
 *  @return     S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDhmTddRlsSubFrm
(
RgSchCellCb          *cellCb
)
#else
PUBLIC S16 rgSCHDhmTddRlsSubFrm(cellCb)
RgSchCellCb          *cellCb;
#endif
{
   CmLteTimingInfo         prevTime;
   CmLteTimingInfo         pastTime;
   RgSchTddDlAscSetIdxK    ascIdx;
   U8                      noFdbks;
   S16                     subfrm;
   U8                      i;
   U8                      decrVal;

   TRC2(rgSCHDhmTddRlsSubFrm)
   decrVal=
      rgSchTddRlsDlSubfrmTbl[cellCb->ulDlCfgIdx][cellCb->crntTime.subframe];
   RGSCHSUBFRMCRNTTIME(cellCb->crntTime, pastTime, decrVal)

   ascIdx = 
      rgSchTddDlAscSetIdxKTbl[cellCb->ulDlCfgIdx][pastTime.subframe];
   noFdbks = ascIdx.numFdbkSubfrms;
   for(i=0; i < noFdbks; i++)
   {
      /* Get the subframe and sfn for which HARQ Ack/Nack
       * has to be sent */

      prevTime = pastTime;

      subfrm = (prevTime.sfn * RGSCH_NUM_SUB_FRAMES) + prevTime.subframe;

      /* Subtract the subframe offset */
      subfrm = subfrm - ascIdx.subfrmNum[i];

      if(subfrm < 0)
      {
         subfrm = (RGSCH_MAX_SFN * RGSCH_NUM_SUB_FRAMES) + subfrm;
         prevTime.sfn = subfrm / RGSCH_NUM_SUB_FRAMES;
         prevTime.subframe = subfrm % RGSCH_NUM_SUB_FRAMES;
      }
      else
      {
         prevTime.sfn = subfrm / RGSCH_NUM_SUB_FRAMES;
         prevTime.subframe = subfrm % RGSCH_NUM_SUB_FRAMES;
      }

      rgSCHUtlDlRlsSubFrm(cellCb, prevTime);
   }
   RETVALUE(ROK);
}/* rgSCHDhmTddRlsSubFrm */

/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/**
 * @brief Handler for Removing the HARQ process from a dlsf.
 *
 * @details
 *
 *     Function : rgSCHDhmRlsDlsfHqProc
 *
 *     This function shall be invoked for every tti. It goes back to
 *     to the sixth last subframe to check whether it still exists. If
 *     that exists this function traverses through the entire harq
 *     proc list associated and frees up all of them.
 *
 *  @param[in]  RgSchCellCb      *cellCb
 *  @param[in]  CmLteTimingInfo  timingInfo
 *  @return     S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDhmRlsDlsfHqProc
(
RgSchCellCb          *cellCb,
CmLteTimingInfo      timingInfo
)
#else
PUBLIC S16 rgSCHDhmRlsDlsfHqProc(cellCb, timingInfo)
RgSchCellCb          *cellCb;
CmLteTimingInfo      timingInfo;
#endif
{
   RgSchDlSf               *dlSf;
   CmLteTimingInfo         prevTime;
   CmLteTimingInfo         nxtfrm = {0,0};
   RgSchDlHqProcCb         *tmpHqProc;
   RgSchTddDlAscSetIdxK    ascIdx;
   U8                      noFdbks;
   S16                     subfrm;
   S16                     i;
   RgSchDlSf               *nxtDlsf = NULLP;
   CmLList                 *node;
   U8                      maxRetx;
   RgSchTddANInfo          *anInfo = NULLP;
   RgSchDlHqTbCb           *tbCb;
   U8                      txCntr = 0;

   TRC2(rgSCHDhmRlsDlsfHqProc)

   ascIdx = 
      rgSchTddDlAscSetIdxKTbl[cellCb->ulDlCfgIdx][timingInfo.subframe];
   noFdbks = ascIdx.numFdbkSubfrms;
   for(i=0; i < noFdbks; i++)
   {
      /* Get the subframe and sfn for which HARQ Ack/Nack
       * has to be sent */

      prevTime = timingInfo;

      subfrm = (prevTime.sfn * RGSCH_NUM_SUB_FRAMES) + prevTime.subframe;

      /* Subtract the subframe offset */
      subfrm = subfrm - ascIdx.subfrmNum[i];

      if(subfrm < 0)
      {
         subfrm = (RGSCH_MAX_SFN * RGSCH_NUM_SUB_FRAMES) + subfrm;
         prevTime.sfn = subfrm / RGSCH_NUM_SUB_FRAMES;
         prevTime.subframe = subfrm % RGSCH_NUM_SUB_FRAMES;
      }
      else
      {
         prevTime.sfn = subfrm / RGSCH_NUM_SUB_FRAMES;
         prevTime.subframe = subfrm % RGSCH_NUM_SUB_FRAMES;
      }

      dlSf = rgSCHUtlSubFrmGet (cellCb, prevTime);
      if(cellCb->ulDlCfgIdx != 5)
      {
         rgSCHUtlGetNxtDlSfInfo(prevTime, cellCb, dlSf, &nxtDlsf, &nxtfrm);
      }
      /* Subframe is present. Delete all the harq associations from 
       * this subframe.
       */
      node = dlSf->tbs.first;
      while (node)
      {
         tbCb = (RgSchDlHqTbCb *)(node->node);
         tmpHqProc = tbCb->hqP;

         txCntr = tmpHqProc->txCntr;
        
         /*rg002.301:ccpu000119494-MOD-treat no harq as DTX */
          tbCb->dtxCount++;
          tbCb->isAckNackDtx = TFU_HQFDB_DTX;
         
          node = node->next;
         
          if (tbCb->fbkRepCntr != 0)
         {
            /* Update feedback time for this process so that 
             * next subframe its picked up */
            anInfo = rgSCHUtlGetUeANFdbkInfo(tmpHqProc->hqE->ue, 
                                                      &tbCb->fdbkTime);
            if(anInfo == NULLP)
            {
               RGSCHDBGINFO(cellCb->instIdx,
                     (rgSchPBuf(cellCb->instIdx), "Ack Rcvd. No Ack/Nack feedback available \n"));
               RETVALUE(RFAILED);
            }
            RGSCH_UPD_HQAN_FDBKTIME(tbCb, nxtDlsf, nxtfrm);
            RGSCH_UPD_ANINFO_WITH_HQ(anInfo, tbCb);
            rgSCHUtlDlHqPTbRmvFrmTx(dlSf, tmpHqProc, tbCb->tbIdx, TRUE);
            tbCb->fbkRepCntr--;
            continue;
         }
         rgSCHUtlDlHqPTbRmvFrmTx(dlSf, tmpHqProc, tbCb->tbIdx, FALSE);
         /*ccpu000119494-ADD- for SPS, call SPS specific DTX handler */
#ifdef LTEMAC_SPS
       /*rg002.301 ccpu00119494-MOD-check for NULLP as this could be Msg4 DTX
        * and Msg4 HQ proc would not be inUseLst */
        if (  (tmpHqProc->sch != (RgSchCmnDlHqProc *)NULLP) && 
              (RG_SCH_CMN_SPS_DL_IS_SPS_HQP(tmpHqProc)) 
           )
        {
           rgSCHUtlDlProcDtx(cellCb,tmpHqProc);
           cmLListDelFrm(&(tmpHqProc->hqE->inUse),&tmpHqProc->lnk);
        }
        else 
#endif
        {
           /* Delete the Harq Association. Release the Harq Process */
           rgSCHDhmHqTbTrnsFail(cellCb, tmpHqProc, tbCb->tbIdx, &maxRetx);
        }
         /*rg007.201: Consider TA TX */
         if (tbCb->taSnt == TRUE)
         {
            /* [ccpu00127148] Correcting the check */
            if (TRUE == maxRetx)
            {
               tbCb->taSnt                  =  FALSE;
               /* state should be IDLE here */
               tmpHqProc->hqE->ue->dl.taCb.ta =  RGSCH_NO_TA_RQD;
               tmpHqProc->hqE->ue->dl.taCb.state =  RGSCH_TA_IDLE;
               /* Start the timer only if TA is cfgd as FINITE value */
               if (tmpHqProc->hqE->ue->dl.taCb.cfgTaTmr)
               {
                  rgSCHTmrStartTmr (cellCb, tmpHqProc->hqE->ue, RG_SCH_TMR_TA, 
                                         tmpHqProc->hqE->ue->dl.taCb.cfgTaTmr);
               }
             }
         }
      }

      node = dlSf->ackNakRepQ.first;
      while (node)
      {
         tbCb = (RgSchDlHqTbCb *)(node->node);
         /* rg007.201 - Updating txCntr */
         txCntr = tbCb->txCntr;
         tmpHqProc = tbCb->hqP;
         
         /*rg002.301:ccpu000119494-MOD-treat no harq as DTX */
         tbCb->dtxCount++;

         node = node->next;
         /* If This is not the last repetition */
         if (tbCb->fbkRepCntr > 1)
         {
            /* Update feedback time for this process so that 
             * next subframe its picked up */
            anInfo = rgSCHUtlGetUeANFdbkInfo(tmpHqProc->hqE->ue, 
                                                      &tbCb->fdbkTime);
            if(anInfo == NULLP)
            {
               RGSCHDBGINFO(cellCb->instIdx,
                     (rgSchPBuf(cellCb->instIdx), "Ack Rcvd. No Ack/Nack feedback available \n"));
               RETVALUE(RFAILED);
            }
            RGSCH_UPD_HQAN_FDBKTIME(tbCb, nxtDlsf, nxtfrm);
            RGSCH_UPD_ANINFO_WITH_HQ(anInfo, tbCb);
            rgSCHUtlDlHqPTbRmvFrmTx(dlSf,tmpHqProc,tbCb->tbIdx, TRUE);
            tbCb->fbkRepCntr--;
            continue;
         }
         else
         {
            rgSCHUtlDlHqPTbRmvFrmTx(dlSf,tmpHqProc,tbCb->tbIdx, TRUE);

            /*rg002.301:ccpu000119494 s-MOD-changed the logic to determine
             * ACK, NACK or DTX */
            if (((tbCb->nackCount + tbCb->dtxCount) >= tbCb->ackCount))
            {
               /*even if one NACK, we consider the feedback
                * on a whole as NACk */
               if ( tbCb->nackCount != 0 )
               {
                  tbCb->isAckNackDtx = TFU_HQFDB_NACK;
               }
               else
               {
                  tbCb->isAckNackDtx = TFU_HQFDB_DTX;
               }
              
#ifdef LTEMAC_SPS
           /*ccpu00119494-MOD-if this Msg4 DTX, there will be no
            * DlHqProc as it has its own HarqProc */
            if ( (tbCb->isAckNackDtx == TFU_HQFDB_DTX) &&
                  (tmpHqProc->sch != (RgSchCmnDlHqProc *)NULLP) && 
                  (RG_SCH_CMN_SPS_DL_IS_SPS_HQP(tmpHqProc)) 
               )
            {
               rgSCHUtlDlProcDtx(cellCb,tmpHqProc);
            }
            else 
#endif    
            {
               /* Delete the Harq Association. Release the Harq Process */
              rgSCHDhmHqTbTrnsFail(cellCb, tmpHqProc, tbCb->tbIdx, &maxRetx);
            }
           }/*if(((tbCb->nackCount+....*/
         }/*else....*/

         /* rg007.201 - Consider TA TX */
         if (tbCb->taSnt == TRUE)
         {
            /* [ccpu00127148] Correcting the check */
            if (TRUE == maxRetx)
            {
               tbCb->taSnt                  =  FALSE;
               /* state should be IDLE here */
               tmpHqProc->hqE->ue->dl.taCb.ta =  RGSCH_NO_TA_RQD;
               tmpHqProc->hqE->ue->dl.taCb.state =  RGSCH_TA_IDLE;
               /* Start the timer only if TA is cfgd as FINITE value */
               if (tmpHqProc->hqE->ue->dl.taCb.cfgTaTmr)
               {
                  rgSCHTmrStartTmr (cellCb, tmpHqProc->hqE->ue, RG_SCH_TMR_TA, 
                                         tmpHqProc->hqE->ue->dl.taCb.cfgTaTmr);
               }
             }
         }
      }
   }
   RETVALUE(ROK);
}/* rgSCHDhmRlsDlsfHqProc */
#else /* ifdef LTE_TDD */
/* rg007.201 - Changes for MIMO feature addition */
/* rg008.201 - Removed dependncy on MIMO compile-time flag */
/**
 * @brief Handler for Removing the HARQ process from a dlsf.
 *                    
 * @details
 *
 *     Function : rgSCHDhmRlsDlsfHqProc
 *     
 *     This function shall be invoked for every tti. It goes back to
 *     to the sixth last subframe to check whether it still exists. If
 *     that exists this function traverses through the entire harq 
 *     proc list associated and frees up all of them.  
 *           
 *  @param[in]  RgSchCellCb     *cell 
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDhmRlsDlsfHqProc
(
RgSchCellCb          *cell,
CmLteTimingInfo      timingInfo
)
#else
PUBLIC S16 rgSCHDhmRlsDlsfHqProc(cell, timingInfo)
RgSchCellCb          *cell;
CmLteTimingInfo      timingInfo;
#endif
{
   RgSchDlSf         *sf;
   CmLteTimingInfo   frm;
   RgSchDlHqProcCb   *tmpHqProc;
   Bool              maxRetx = FALSE;
   CmLList           *node;
   RgSchDlHqTbCb     *tbCb;
   /* rg007.201 -  Added txCntr */
   U8                txCntr = 0;

   TRC2(rgSCHDhmRlsDlsfHqProc)

   /* Fetch the current timing info. Modify it to Last sf to be rlsd.*/
   RGSCHSUBFRMCRNTTIME(timingInfo, frm, RG_SCH_CMN_HARQ_INTERVAL); 
   
   RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),"\n rgSCHDhmRlsDlsfHqProc():\
                  frm timing info sfn=%d subframe=%d",frm.sfn,frm.subframe));

   /* Get the required Last subframe */
   sf = rgSCHUtlSubFrmGet(cell, frm);

   /* Subframe is present. Delete all the harq associations from 
    * this subframe.
    */

   node = sf->tbs.first;
   while (node)
   {
      tbCb = (RgSchDlHqTbCb *)(node->node);
      txCntr = tbCb->txCntr;
      tmpHqProc = tbCb->hqP;
      
      RGSCHDBGINFO(cell->instIdx, (rgSchPBuf(cell->instIdx),"\n rgSCHDhmRlsDlsfHqProc():\
                    txCntr=%d tmpHqProc=%d",tbCb->txCntr,tmpHqProc->procId));
      
      /*rg002.301 ccpu000119494 s-MOD-treat no harq as DTX */
       tbCb->dtxCount++;
       tbCb->isAckNackDtx = TFU_HQFDB_DTX;
      
       node = node->next;
      if (tbCb->fbkRepCntr != 0)
      {
         /* Update timingInfo for this hqP so that next subframe its picked up */
         RGSCHADDTOCRNTTIME(tbCb->timingInfo, tbCb->timingInfo, 1);
         rgSCHUtlDlHqPTbRmvFrmTx(sf,tmpHqProc,tbCb->tbIdx, TRUE);
         tbCb->fbkRepCntr--;
         continue;
      }
       rgSCHUtlDlHqPTbRmvFrmTx(sf,tmpHqProc,tbCb->tbIdx, FALSE);
      /*rg002.301 cpu00119494-ADD- for SPS, call SPS specific DTX handler */
#ifdef LTEMAC_SPS
      /*rg002.301 ccpu00119494-MOD-if this Msg4 DTX, there will be no
       * DlHqProc as it has its own HarqProc */
      if ( (tmpHqProc->sch != (RgSchCmnDlHqProc *)NULLP) && 
            RG_SCH_CMN_SPS_DL_IS_SPS_HQP(tmpHqProc) )
      {
         rgSCHUtlDlProcDtx(cell,tmpHqProc);
      }
      else 
#endif
      {
         /* Delete the Harq Association. Release the Harq Process */
         rgSCHDhmHqTbTrnsFail(cell, tmpHqProc, tbCb->tbIdx, &maxRetx);
      }
      /* rg007.201 -  Consider TA TX */
      if (tbCb->taSnt == TRUE)
      {
         /* [ccpu00127148] Correcting the check */
         if (TRUE == maxRetx)
         {
            tbCb->taSnt                  =  FALSE;
            /* state should be IDLE here */
            tmpHqProc->hqE->ue->dl.taCb.ta =  RGSCH_NO_TA_RQD;
            tmpHqProc->hqE->ue->dl.taCb.state =  RGSCH_TA_IDLE;
            /* Start the timer only if TA is cfgd as FINITE value */
            if (tmpHqProc->hqE->ue->dl.taCb.cfgTaTmr)
            {
               rgSCHTmrStartTmr (cell, tmpHqProc->hqE->ue, RG_SCH_TMR_TA, 
                                      tmpHqProc->hqE->ue->dl.taCb.cfgTaTmr);
            }
          }
      }
   }

   node = sf->ackNakRepQ.first;
   while (node)
   {
      tbCb = (RgSchDlHqTbCb *)(node->node);
      /* rg007.201 -  Updating txCntr */
      txCntr = tbCb->txCntr;
      tmpHqProc = tbCb->hqP;
       
      /*rg002.301 ccpu000119494-MOD-treat no harq as DTX */
       tbCb->dtxCount++;
      
       node = node->next;
      
       /* If This is not the last repetition */
      if (tbCb->fbkRepCntr > 1)
      {
         /* Update timingInfo for this hqP so that next subframe its picked up */
         RGSCHADDTOCRNTTIME(tbCb->timingInfo, tbCb->timingInfo, 1);
         rgSCHUtlDlHqPTbRmvFrmTx(sf,tmpHqProc,tbCb->tbIdx, TRUE);
         tbCb->fbkRepCntr--;
         continue;
      }
      else
      {
         rgSCHUtlDlHqPTbRmvFrmTx(sf,tmpHqProc,tbCb->tbIdx, TRUE);
         
         /*rg002.301 ccpu000119494-MOD-changed the logic to determine
          * ACK, NACK or DTX */
         if (((tbCb->nackCount + tbCb->dtxCount) >= tbCb->ackCount))
         {
            if ( tbCb->nackCount != 0 )
            {
               tbCb->isAckNackDtx = TFU_HQFDB_NACK;
            }
            else
            {
               tbCb->isAckNackDtx = TFU_HQFDB_DTX;
            }

#ifdef LTEMAC_SPS
           /*rg002.301 ccpu00119494-MOD-if this Msg4 DTX, there will be no
            * DlHqProc as it has its own HarqProc */
            if ( (tbCb->isAckNackDtx == TFU_HQFDB_DTX)    &&
                  (tmpHqProc->sch != (RgSchCmnDlHqProc *)NULLP) &&
                  RG_SCH_CMN_SPS_DL_IS_SPS_HQP(tmpHqProc) )
            {
               rgSCHUtlDlProcDtx(cell,tmpHqProc);
            }
            else 
#endif
            {
               /* Delete the Harq Association. Release the Harq Process */
               rgSCHDhmHqTbTrnsFail(cell, tmpHqProc, tbCb->tbIdx, &maxRetx);
            }
         }/*if (tbCb->nackCount +....*/
      }
      /* rg007.201 Consider TA TX */
      if (tbCb->taSnt == TRUE)
      {
         if (maxRetx == TRUE)
         {
            tbCb->taSnt                  =  FALSE;
            /* state should be IDLE here */
            tmpHqProc->hqE->ue->dl.taCb.ta =  RGSCH_NO_TA_RQD;
            tmpHqProc->hqE->ue->dl.taCb.state =  RGSCH_TA_IDLE;
            /* Start the timer only if TA is cfgd as FINITE value */
            if (tmpHqProc->hqE->ue->dl.taCb.cfgTaTmr)
            {
               rgSCHTmrStartTmr (cell, tmpHqProc->hqE->ue, RG_SCH_TMR_TA, 
                                      tmpHqProc->hqE->ue->dl.taCb.cfgTaTmr);
            }
          }
      }
   }
   RETVALUE(ROK);
}  /* rgSCHDhmRlsDlsfHqProc */
#endif
#ifdef LTEMAC_SPS
#ifdef RG_UNUSED
/**
 * @brief This function marks the HARQ process with a given ID as SPS HARQ
 * proc
 *
 * @details
 *
 *     Function: rgSCHDhmMarkSpsHqProc
 *     Purpose:  This function returns the HARQ process with the given ID.
 *     Invoked by: SPS Module
 *     Processing steps: 
 *     - Get the HARQ process by index from the UE
 *     - Set isSpsHqProc = TRUE
 *     
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  U8            idx
 *  @return  S16       
 *         -#   ROK     if successful
 *         -#   RFAILED otherwise
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDhmMarkSpsHqProc
(
RgSchUeCb               *ue,
U8                      idx
)
#else
PUBLIC S16 rgSCHDhmMarkSpsHqProc(ue, idx)
RgSchUeCb               *ue;
U8                      idx;
#endif
{
   RgSchDlHqProcCb      *hqP;
#ifdef DEBUGP
   Inst    inst = ue->cell->instIdx;
#endif
   TRC2(rgSCHDhmMarkSpsHqProc)

      RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Marking SPS harq Proc for "
               "index(%d)\n", idx));
   /* Pick the proc based on the index provided */
   rgSCHDhmGetHqProcFrmId(ue, idx, &hqP);

   RETVALUE(ROK);
} /* rgSCHDhmMarkSpsHqProc */
#endif /* RG_UNUSED */
#endif /* LTEMAC_SPS */

#ifndef LTE_TDD
/** * @brief Handler for HARQ feedback received for DL AckNack rep enabled UE
 *
 * @details
 *
 *     Function : rgSCHDhmProcHqFdbkAckNackRep
 *     
 *     This function shall act on the feedback received from TOM for DL
 *     transmission. 
 *     
 *           
 *  @param[in]  RgSchDlHqProcCb     *hqP
 *  @param[in]  RgSchDlSf           *sf                  
 *  @param[in]  U8                  tbCnt
 *  @param[in]  U8                  *isAck
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHDhmProcHqFdbkAckNackRep
(
RgSchDlHqProcCb      *hqP,
RgSchDlSf            *sf,
U8                   tbCnt,
U8                   *isAck
)
#else
PRIVATE S16 rgSCHDhmProcHqFdbkAckNackRep(hqP,sf,tbCnt,isAck)
RgSchDlHqProcCb      *hqP;
RgSchDlSf            *sf;
U8                   tbCnt;
U8                   *isAck;
#endif
{
    TRC2(rgSCHDhmProcHqFdbkAckNackRep)
      /* Check if this is repeating UE */
    rgSCHUtlDlHqPTbRmvFrmTx(sf, hqP, tbCnt, TRUE);
    /* Check if last repetition */
    if (--hqP->tbInfo[tbCnt].fbkRepCntr)
    {
        /* Update timingInfo for this hqP so that next subframe its picked up */
        RGSCHADDTOCRNTTIME(hqP->tbInfo[tbCnt].timingInfo, \
                               hqP->tbInfo[tbCnt].timingInfo, 1);
        RETVALUE(RFAILED);
    }
    
    /*rg002.301 ccpu00119494-MOD-take decision here based on the number
     * of DTX's,NACK's and ACK's received
     */
    if (((hqP->tbInfo[tbCnt].ackCount) > (hqP->tbInfo[tbCnt].nackCount) + 
                (hqP->tbInfo[tbCnt].dtxCount)))
    {
        *isAck = TFU_HQFDB_ACK; 
    }
    /*even a single NACK indicates that UE received
     * the transmission. 
     */
    else if ( hqP->tbInfo[tbCnt].nackCount != 0 )
    {
        *isAck = TFU_HQFDB_NACK;
    }
    else
    {
        *isAck = TFU_HQFDB_DTX;
    }
      
       
    hqP->tbInfo[tbCnt].isAckNackDtx = *isAck; 
   /* rg004.201 patch - ccpu00110457 DTX Changes End */
    RETVALUE(ROK);
}
#endif /* ifndef LTE_TDD */


/* rg003.301 -Add-[ccpu00120794] - Freeing up the HARQ proc blocked for
 * indefinite time in case of Retx */
/**
 * @brief This function handles the scenario in case Retx allocation is failed.
 *
 * @details
 *
 *     Function: rgSCHDhmDlRetxAllocFail
 *     Purpose:  
 *                
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  RgSchDlHqProcCb       *hqP
 *  @return  Void 
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDhmDlRetxAllocFail 
(
RgSchUeCb               *ue,
RgSchDlHqProcCb         *hqP
)
#else
PUBLIC S16 rgSCHDhmDlRetxAllocFail(ue, hqP)
RgSchUeCb               *ue;
RgSchDlHqProcCb         *hqP;
#endif
{
   RgSchCellCb      *cell;
   RgInfRlsHqInfo   *rlsHqInfo;
   Pst              pst;   
   Bool             maxRetx = FALSE;

   TRC2(rgSCHDhmDlRetxAllocFail);

   cell = ue->cell;
   rlsHqInfo = &(cell->rlsHqArr[cell->crntHqIdx]);
 
   /* If retx was attempted for 1st TB, increment its retx alloc fail counter */
   if (hqP->tbInfo[0].state == HQ_TB_NACKED)
   {
      hqP->tbInfo[0].cntrRetxAllocFail++;
   }

   /* If retx was attempted for 2nd TB, increment its retx alloc fail counter */
   if (hqP->tbInfo[1].state == HQ_TB_NACKED)
   {
      hqP->tbInfo[1].cntrRetxAllocFail++;
   }

   /* initialize MAC-SCH interface HARQ release info */
   rlsHqInfo->numUes = 0;
   rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs = 0;

   /* Release HARQ proc for TB1 if Retx alloc failure count has reached max */
   if (hqP->tbInfo[0].cntrRetxAllocFail == RG_SCH_MAX_RETX_ALLOC_FAIL)
   {
      if (hqP->hqE->msg4Proc == hqP)
      {
         hqP->tbInfo[0].txCntr = cell->dlHqCfg.maxMsg4HqTx;
      }
      else
      {
         hqP->tbInfo[0].txCntr = hqP->hqE->maxHqTx;
      }

      rgSCHDhmHqTbTrnsFail(cell, hqP, hqP->tbInfo[0].tbIdx, &maxRetx);

#ifdef LTE_L2_MEAS
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].status[\
               rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs] = FALSE;
#endif /* Lte_L2_MEAS */
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].tbId[\
               rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs] = 1;
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs++;
   }

   /* Release HARQ proc for TB2 if Retx alloc failure count has reached max */
   if (hqP->tbInfo[1].cntrRetxAllocFail == RG_SCH_MAX_RETX_ALLOC_FAIL)
   {
      if (hqP->hqE->msg4Proc == hqP)
      {
         hqP->tbInfo[1].txCntr = cell->dlHqCfg.maxMsg4HqTx;
      }
      else
      {
         hqP->tbInfo[1].txCntr = hqP->hqE->maxHqTx;
      }

      rgSCHDhmHqTbTrnsFail(cell, hqP, hqP->tbInfo[1].tbIdx, &maxRetx);

#ifdef LTE_L2_MEAS
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].status[\
               rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs] = FALSE;
#endif /* Lte_L2_MEAS */
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].tbId[\
               rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs] = 2;
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs++;
   }

   /* MS_WORKAROUND for ccpu00122892 Temp fix for erroeneous RETX Harq release by rgSCHCmnDlAllocRetxRb */
   
   if ((hqP->tbInfo[0].state != HQ_TB_NACKED) &&
       (hqP->tbInfo[1].state != HQ_TB_NACKED))	  
   {
      if (cell->dlSchdType == RGSCH_TYPE_RR)
      {
         rgSCHRrDlRmvProcFrmRetxLst(cell, hqP);		 
      }
      else if (cell->dlSchdType == RGSCH_TYPE_PFS)
      {
         rgSCHPfsDlProcRmvFrmRetx(cell, ue, hqP);		 
      }
   }

   /* send HARQ release to MAC */
   if (rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].numOfTBs > 0)
   {
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].rnti = ue->rntiLnk->rnti;
      rlsHqInfo->ueHqInfo[rlsHqInfo->numUes].hqProcId = hqP->procId;
      rlsHqInfo->numUes = 1;

      rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
      RgSchMacRlsHq(&pst, rlsHqInfo);
   }

   RETVALUE(ROK);
}


/**********************************************************************
 
         End of file:     gk_sch_dhm.c@@/main/2 - Sat Jul 30 02:21:38 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       ps 1. LTE MAC 2.1 release
             rg001.201 sm 1. Removed extra comments. 
             rg003.201 ns 1. Trace added for functions.
             rg004.201 ms 1. DL DTX Handling
             rg006.201 sd 1. [ccpu00113186]: Corrected TA handling
             rg007.201 ap 1. Added support for MIMO
                          2. NDI toggling fix
                          3. Added TA related fixes
             rg008.201 dv 1. Fix for ccpu00113622 - Delete Only when taLnk Node exists
                          2. Fix for ccpu00113296 - Do not delete from 'inUse' for Msg4 Harq Entities 
                          3. Fix for ccpu00114124 - HARQ release for Msg4 HARQ Entities   
             rg009.201 rsharon 1. [ccpu00112372]:Changes for CR timer.
             rg009.201 dv 1. Added Changes of TFU_UPGRADE.
/main/2      ---         gvj 1. Updated for LTE MAC Rel 3.1
             rg001.301 nudupi  1. ccpu00118350:Correcting NDI manipulation of Harq.
                               2. ccpu00118633 : Corrections for Harq.
             rg002.301 asehgal 1. ccpu00118468: corrected default value of TA.
                               2. ccpu00120264: changed SPS TDD Harq feedback code
                                                 to be more inline with normal TDD
                                                 Harq feedback code.
                               3. ccpu00119494: changed the default feedback to DTX
                                                from NACK in case no feedback is
                                                received from UE.
                               4. ccpu00119223: Assigned the rnti with ueId 
                                  instead of assigning it with rntiLnk->rnti.
                               5. ccpu00120233: TB Count reset during Release.
                               6.ccpu00119953:setting of cqireq flag
                               7.ccpu00119543:harq bug in TA
                               8.ccpu00118351: seperate UL & DL deltas.
                               9. ccpu00120284: UE should be removed from the TaLst
      rg003.301   hsingh       1. ccpu00120794-Freeing up the HARQ proc blocked 
                                  for indefinite time in case of Retx
*********************************************************************91*/

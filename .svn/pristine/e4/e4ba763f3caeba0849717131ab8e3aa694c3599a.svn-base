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
  
     Desc:     C source code for L2 Measurement fucntions
  
     File:     rg_sch_l2m.c
  
     Sid:      gk_sch_l2m.c@@/main/1 - Sat Jul 30 02:21:43 2011
  
     Prg:      AP
  
**********************************************************************/

/** @file rg_sch_l2m.c
@brief This file implements the L2 Measurement feature code.
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch.h"
#include "rg_sch_cmn.h"
#include "rg_sch_inf.h"         /* typedefs for Scheduler */
#include "rg_sch_err.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common tokens */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"    /* typedefs for Scheduler */
/* local defines */
#ifdef LTE_L2_MEAS
PRIVATE S16 rgSchL2mInsertMeasCb ARGS((
         RgSchCellCb       *cell,
         RgSchL2MeasCb     *measCb,
         LrgSchMeasReqInfo *measInfo ));

PRIVATE RgSchL2MeasCb * rgSchL2mAllocMeasCb ARGS((
         RgSchCellCb       *cell,
         LrgSchMeasReqInfo *measInfo,
         RgSchErrInfo      err));

/* Function definitions */

/** @brief This function fills the L2 measurement confirm structure
 *
 * @details
 *
 *     Function: rgSchFillL2MeasCfm
 *
 * @param  [in] RgSchCellCb *cell 
 * @param  [in] RgSchL2MeasCb *measCb
 * @param  [out] LrgSchMeasCfmInfo *measCfm
 * @return  Void
 */
#ifdef ANSI
PUBLIC S16 rgSchFillL2MeasCfm
(
RgSchCellCb       *cell,
RgSchL2MeasCb     *measCb,
LrgSchMeasCfmInfo *cfm
)
#else
PUBLIC S16 rgSchFillL2MeasCfm(cell, measCb, cfm)
RgSchCellCb       *cell;
RgSchL2MeasCb     *measCb;
LrgSchMeasCfmInfo *cfm;
#endif
{
   U8                 idx;
   LrgSchMeasReqInfo  *measInfo;
   U8                 qciVal = 0;
   U8                 sampOc = 0;

   TRC3(rgSchFillL2MeasCfm)
   
   measInfo = &measCb->measReq;   

   cfm->hdr.transId  = measInfo->hdr.transId;
   cfm->measType     = measInfo->measType;
   cfm->cellId       = measInfo->cellId;
   cfm->cfm.status   = LCM_PRIM_OK;
   if(measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_DL)
   {
       cfm->avgPrbDl.prbPerc = ((cell->avgPrbDl.prbCount * 100) /
                                (cell->bwCfg.dlTotalBw * 
                                 measCb->measReq.timePrd));
#ifdef MSPD 
       MSPD_DBG("LRG_L2MEAS_AVG_PRB_DL prbCount=[%d],dlTotalBw=[%d],timePrd=[%d] \n",
                cell->avgPrbDl.prbCount,cell->bwCfg.dlTotalBw,measCb->measReq.timePrd);
#endif
       /* Resetting the prbCount to 0,  fix for ccpu00125002 */
       cell->avgPrbDl.prbCount = 0;
   }
   if((measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_UL) &&
       (cell->ulAvailBw > 0))
   {
       cfm->avgPrbUl.prbPerc = ((cell->avgPrbUl.prbCount * 100) /
       (cell->ulAvailBw * measCb->measReq.timePrd));
#ifdef MSPD
       MSPD_DBG("LRG_L2MEAS_AVG_PRB_UL prbCount=[%d],ulAvailBw=[%d],timePrd=[%d] \n",
                cell->avgPrbDl.prbCount,cell->ulAvailBw,measCb->measReq.timePrd);
#endif
       /* Resetting the prbCount to 0,  fix for ccpu00125002 */
       cell->avgPrbUl.prbCount = 0;
   }
   if(measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL)
   {
       cfm->avgPrbQciDlCfm.numQci = measCb->measReq.avgPrbQciDl.numQci;
       for(idx = 0; idx < measCb->measReq.avgPrbQciDl.numQci; idx++)
       {
          qciVal = measCb->measReq.avgPrbQciDl.qci[idx];
          cfm->avgPrbQciDlCfm.prbPercQci[idx].prbPercQci = 
          ((cell->qciArray[qciVal].dlPrbCount * 100) /
           (cell->bwCfg.dlTotalBw * measCb->measReq.timePrd));
            cfm->avgPrbQciDlCfm.prbPercQci[idx].qciValue = qciVal;
#ifdef MSPD
          MSPD_DBG("LRG_L2MEAS_AVG_PRB_PER_QCI_DL dlPrbCount[%d]=[%d],ulAvailBw=[%d],timePrd=[%d] \n",
                   qciVal, cell->qciArray[qciVal].dlPrbCount, cell->bwCfg.dlTotalBw,measCb->measReq.timePrd);
#endif
          cell->qciArray[qciVal].dlPrbCount = 0;
       }
   }
   if(measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL)
   {
       cfm->avgPrbQciUlCfm.numQci = measCb->measReq.avgPrbQciUl.numQci;
       for(idx = 0; idx < measCb->measReq.avgPrbQciUl.numQci; idx++)
       {
          cfm->avgPrbQciUlCfm.prbPercQci[idx].qciValue = 
                                 measCb->avgPrbQciUl.prbPercQci[idx].qciValue;
          cfm->avgPrbQciUlCfm.prbPercQci[idx].prbPercQci = 
                                 measCb->avgPrbQciUl.prbPercQci[idx].prbPercQci;
#ifdef MSPD
          MSPD_DBG("LRG_L2MEAS_AVG_PRB_PER_QCI_UL prbPercQci[%d]=[%d] \n",
                   idx, cfm->avgPrbQciUlCfm.prbPercQci[idx]);
#endif
       }
   }
   if(measCb->measReq.measType & LRG_L2MEAS_RA_PREAMBLE)
   {
       cfm->raPrmbsCfm.dedPreambles        = cell->raPrmbs.dedPream;
       cfm->raPrmbsCfm.randSelPreLowRange  = cell->raPrmbs.preamGrpA;
       cfm->raPrmbsCfm.randSelPreHighRange = cell->raPrmbs.preamGrpB;
#ifdef MSPD
       MSPD_DBG("\n LRG_L2MEAS_RA_PREAMBLE ddedPreambles=[%d] \n",cfm->raPrmbsCfm.dedPreambles );
       MSPD_DBG("\n LRG_L2MEAS_RA_PREAMBLE drandSelPreLowRange=[%d] \n",cfm->raPrmbsCfm.randSelPreLowRange);
       MSPD_DBG("\n LRG_L2MEAS_RA_PREAMBLE randSelPreHighRange=[%d] \n",cfm->raPrmbsCfm.randSelPreHighRange);
#endif
       cell->raPrmbs.dedPream              = 0;
       cell->raPrmbs.preamGrpA             = 0;
       cell->raPrmbs.preamGrpB             = 0;
   }
   if(measCb->measReq.measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL)
   {
       cfm->numUeQciDlCfm.numQci = measInfo->nmbActvUeQciDl.numQci;
       sampOc = (measInfo->timePrd / measInfo->nmbActvUeQciDl.sampPrd);
       for(idx = 0; idx < measCb->measReq.nmbActvUeQciDl.numQci; idx++)
       {
          qciVal = measCb->measReq.nmbActvUeQciDl.qci[idx];
          cfm->numUeQciDlCfm.numActvUeQci[idx].numActvUeQci =
          cell->qciArray[qciVal].dlTotal_UeCount / sampOc;
          cfm->numUeQciDlCfm.numActvUeQci[idx].qciValue = qciVal;
#ifdef MSPD
          MSPD_DBG("\n LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL dlTotal_UeCount[%d],sampOc=[%d], QciVal=[%d] \n",
          cell->qciArray[qciVal].dlTotal_UeCount,sampOc,qciVal);
#endif
          cell->qciArray[qciVal].dlTotal_UeCount = 0;
       }
   }
   if(measCb->measReq.measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL)
   {
       cfm->numUeQciUlCfm.numQci = measInfo->nmbActvUeQciUl.numQci;
       sampOc = (measInfo->timePrd / measInfo->nmbActvUeQciUl.sampPrd);
       for(idx = 0; idx < measCb->measReq.nmbActvUeQciUl.numQci; idx++)
       {
          qciVal = measCb->measReq.nmbActvUeQciUl.qci[idx]; 
          cfm->numUeQciUlCfm.numActvUeQci[idx].numActvUeQci =
          cell->qciArray[qciVal].ulTotal_UeCount/ sampOc;
          cfm->numUeQciUlCfm.numActvUeQci[idx].qciValue = qciVal;
#ifdef MSPD
          MSPD_DBG("\n LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL dlTotal_UeCount[%d],sampOc=[%d], QciVal=[%d] \n", 
          cell->qciArray[qciVal].ulTotal_UeCount,sampOc,qciVal);
#endif
          cell->qciArray[qciVal].ulTotal_UeCount = 0;
       }
   }

   RETVALUE(ROK);
} /* rgSchFillL2MeasCfm */

/** @brief This function sends the L2 measurement confirm to LM 
 * from Shceduler
 *
 * @details
 *
 *     Function: rgSchL2mSndCfm
 *
 * @param  [in] Pst   *pst
 * @param  [in] RgSchL2MeasCb *measCb
 * @param  [in] Bool     isErr
 * @return  Void
 */
#ifdef ANSI
PUBLIC S16 rgSchL2mSndCfm
(
Pst               *pst,
RgSchL2MeasCb     *measCb,
LrgSchMeasReqInfo *measInfo,
Bool              isErr
)
#else
PUBLIC S16 rgSchL2mSndCfm(pst, measCb, measInfo, isErr)
Pst               *pst;
RgSchL2MeasCb     *measCb;
LrgSchMeasReqInfo *measInfo;
Bool              isErr;
#endif
{
   LrgSchMeasCfmInfo   cfm;

   TRC3(rgSchL2mSndCfm)

   cmMemset((U8 *)&cfm, (U8)0, sizeof(LrgSchMeasCfmInfo));
   cfm.hdr.transId  = measInfo->hdr.transId;
   cfm.measType     = measInfo->measType;
   cfm.cellId       = measInfo->cellId;
   cfm.cfm.status   = LCM_PRIM_OK;
   if(isErr == TRUE)
   {
      cfm.cfm.status   = LCM_PRIM_NOK;
      cfm.cfm.reason   = LCM_REASON_INVALID_PAR_VAL;
      RgMiLrgSchL2MeasCfm(pst, &cfm);
      RETVALUE(ROK);
   }
   RETVALUE(ROK);
} /* rgSchL2mSndCfm */

/** @brief This function fills the LM confirmation pst structure 
 *
 * @details
 *
 *     Function: rgSchL2mFillCfmPst
 *
 * @param  [in] Pst    *pst
 * @param  [out] Pst    *cfmPst
 * @param  [in] LrgSchMeasReqInfo *measInfo
 * @return  Void
 */
#ifdef ANSI
PUBLIC Void rgSchL2mFillCfmPst
(
Pst    *pst,
Pst    *cfmPst,
LrgSchMeasReqInfo *measInfo 
)
#else
PUBLIC Void rgSchL2mFillCfmPst(pst, cfmPst, measInfo)
Pst    *pst;
Pst    *cfmPst;
LrgSchMeasReqInfo *measInfo;
#endif
{
   Inst inst;

   TRC3(rgSchL2mFillCfmPst)

   inst = (pst->dstInst - RGSCH_INST_START);

   cfmPst->srcEnt    = pst->dstEnt;
   cfmPst->srcInst   = pst->dstInst;
   cfmPst->srcProcId = pst->dstProcId;
   cfmPst->dstEnt    = pst->srcEnt;
   cfmPst->dstInst   = pst->srcInst;
   cfmPst->dstProcId = pst->srcProcId;

   cfmPst->selector  = measInfo->hdr.response.selector;
   cfmPst->prior     = measInfo->hdr.response.prior;
   cfmPst->route     = measInfo->hdr.response.route;
   cfmPst->region    = measInfo->hdr.response.mem.region;
   cfmPst->pool      = measInfo->hdr.response.mem.pool;

   RETVOID;
} /* rgSchL2mFillCfmPst */

/** @brief This function inserts the MeasCb in to data base
 *
 * @details
 *
 *     Function: rgSchL2mInsertMeasCb
 *
 * @param  [in] RgSchCellCb       *cell
 * @param  [in] RgSchL2MeasCb     *measCb
 * @param  [in] LrgSchMeasReqInfo *measInfo
 * @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgSchL2mInsertMeasCb
(
RgSchCellCb       *cell,
RgSchL2MeasCb     *measCb,
LrgSchMeasReqInfo *measInfo
)
#else
PRIVATE S16 rgSchL2mInsertMeasCb(cell, measCb, measInfo)
RgSchCellCb       *cell;
RgSchL2MeasCb     *measCb;
LrgSchMeasReqInfo *measInfo;
#endif
{
   CmLList   *lnk, *node;
   RgSchL2MeasCb   *oldMeasCb;
   U16              diffTime;
   
   TRC3(rgSchL2mInsertMeasCb)
   /* 
    * 1. Check if l2mList has any entries.
    * 2. If yes 
    *       1. Take the first entrie's time period and find the diff with
    *       cell->crntTime.
    *       2. If the diff is > measInfo->timePeriod then insert before this
    *       entry.
    *       3. Else take the next entry in list
    *       4. If reached without adding to list . Append at the end of list.
    * 3. If no entries in l2mList add at the first.
    */
   lnk = cell->l2mList.first;
   node = &measCb->measLnk;
   node->node = (PTR)measCb;
   while(lnk != NULLP )
   {
      oldMeasCb = (RgSchL2MeasCb *)lnk->node;
      diffTime = (oldMeasCb->measReq.timePrd - 
                 (RGSCH_CALC_SF_DIFF(cell->crntTime, oldMeasCb->startTime)));
      if (diffTime > measInfo->timePrd)
      {
         cmLListInsCrnt(&(cell->l2mList), node);
         RETVALUE(ROK);
      }
      else
      {
         lnk = lnk->next;
      }
   }  /* End of While */
   cmLListAdd2Tail(&(cell->l2mList), node);
   RETVALUE(ROK);
} /* rgSchL2mInsertMeasCb */

/** @brief This function calculates the Down link prb count 
 * for a DlSf
 *
 * @details
 *
 *     Function: rgSchL2CalDlPrbCount
 *
 * @param  [in] RgSchCellCb       *cell
 */
#ifdef ANSI
PRIVATE Void rgSchL2CalDlPrbCount
(
RgSchCellCb       *cell
)
#else
PRIVATE Void rgSchL2CalDlPrbCount(cell)
RgSchCellCb       *cell;
#endif
{
   CmLteTimingInfo    frm;
   RgSchDlSf          *sf = NULLP;
#ifdef LTE_TDD
   U8                 idx;
#endif

   TRC3(rgSchL2CalDlPrbCount)

   frm   = cell->crntTime;
   RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA);
   sf = rgSCHUtlSubFrmGet(cell, frm);
#ifdef LTE_TDD
   idx = (cell->crntTime.subframe + RG_SCH_CMN_DL_DELTA) % 
                  RGSCH_NUM_SUB_FRAMES;
   if(rgSchTddUlDlSubfrmTbl[cell->ulDlCfgIdx][idx] ==
       RG_SCH_TDD_DL_SUBFRAME)
    {
      cell->avgPrbDl.prbCount += sf->bwAssigned;
    }
#else
   cell->avgPrbDl.prbCount += sf->bwAssigned;
#endif
   RETVOID;
}

/** @brief This function calculates the up link prb count 
 * for a UlSf
 *
 * @details
 *
 *     Function: rgSchL2CalUlPrbCount
 *
 * @param  [in] RgSchCellCb       *cell
 */
#ifdef ANSI
PRIVATE Void rgSchL2CalUlPrbCount
(
RgSchCellCb       *cell
)
#else
PRIVATE Void rgSchL2CalUlPrbCount(cell)
RgSchCellCb       *cell;
#endif
{
   RgSchUlSf        *sf = NULLP;
   RgSchCmnUlCell   *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
#ifdef LTE_TDD
   U8                 idx;
#endif

   TRC3(rgSchL2CalUlPrbCount)

#ifdef LTE_TDD
   idx = cellUl->schdIdx;
   if(idx < cellUl->numUlSubfrms)
   {
      sf = &cellUl->ulSfArr[idx];
      cell->avgPrbUl.prbCount += sf->totPrb;
   }
#else
   sf = &cellUl->ulSfArr[cellUl->schdIdx];
   cell->avgPrbUl.prbCount += sf->totPrb;
#endif
   RETVOID;
}
/** @brief This function allocates memory from the heap
 *
 * @details
 *
 *     Function: rgSchL2mAllocMeasCb
 *
 * @param  [in] RgSchCellCb       *cell
 * @param  [in] RgSchL2MeasCb     *measInfo
 * @param  [out] RgSchErrInfo      *err
 * @return  RgSchL2MeasCb *
 */
#ifdef ANSI
PRIVATE RgSchL2MeasCb * rgSchL2mAllocMeasCb
(
RgSchCellCb       *cell,
LrgSchMeasReqInfo *measInfo,
RgSchErrInfo      err
)
#else
PRIVATE RgSchL2MeasCb * rgSchL2mAllocMeasCb(cell, measInfo, err)
RgSchCellCb       *cell;
LrgSchMeasReqInfo *measInfo;
RgSchErrInfo      err;
#endif
{
   RgSchL2MeasCb       *measCb = NULLP;
   Inst                inst = cell->instIdx;
   UNUSED(err);
   TRC3(rgSchL2mAllocMeasCb)

   if((rgSCHUtlAllocSBuf(inst, (Data **)&measCb,
                   sizeof(RgSchL2MeasCb))) == RFAILED)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "rgSchL2mAllocMeasCb():"
                  "Allocation of RgSchL2MeasCb failed\n"));
      RETVALUE(NULLP);
   }
   cmMemcpy((U8 *)&measCb->measReq, (U8 *)measInfo, sizeof(LrgSchMeasReqInfo));
   RGSCHCPYTIMEINFO(cell->crntTime, measCb->startTime);

   RETVALUE(measCb);
} /* rgSchL2mAllocMeasCb */

/**
 * @brief Layer Manager Measurement request handler. 
 *
 * @details
 *
 *     Function : rgSchL2mMeasReq
 *     
 *     This function handles  measurement request received at scheduler instance
 *     from the Layer Manager.
 *     -# Measurement request will be stored in the list in ascending order of
 *     their time period.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  LrgSchMeasReqInfo *measInfo, the measurement request structure
 *  @param[out] RgSchErrInfo   *err, error information
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSchL2mMeasReq 
(
RgSchCellCb       *cell,
LrgSchMeasReqInfo *measInfo,
RgSchErrInfo      err
)
#else
PUBLIC S16 rgSchL2mMeasReq(cell, measInfo, err)
RgSchCellCb       *cell;
LrgSchMeasReqInfo *measInfo;
RgSchErrInfo      err;
#endif    
{
   RgSchL2MeasCb *measCb;
   U8            idx;
   U8            qciVal;
#ifdef DEBUGP
   Inst          inst = cell->instIdx;
#endif

   TRC3(rgSchL2mMeasReq)

   qciVal = 0;
   if ((measCb = rgSchL2mAllocMeasCb(cell, measInfo, err)) == NULLP)
   {
       RGSCHFILLERR(err, RGSCHERR_L2M_MEASREQ,
                    RGSCHERR_SCH_ALLOC_FAILED);
       RGSCHDBGERR(inst, (rgSchPBuf(inst), "rgSchL2mMeasReq():"
                "Allocation of RgSchL2MeasCb failed\n"));
       RETVALUE(RFAILED);
   }
   cmMemcpy((U8 *)&measCb->measReq, (CONSTANT U8 *)measInfo,\
             sizeof(LrgSchMeasReqInfo));
   rgSchL2mInsertMeasCb(cell, measCb, measInfo);
   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL)
   {
     for (idx = 0; idx < measInfo->avgPrbQciDl.numQci; idx++)
     {
        qciVal = measInfo->avgPrbQciDl.qci[idx];
        cell->qciArray[qciVal].qci = qciVal;
     }
   }
   /* Here post the message to MAC */
   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL)
   {
      RgInfL2MeasReq    measReq;
      Pst               pst;
      cmMemset((U8 *)&measReq, 0, sizeof(RgInfL2MeasReq));
      measReq.transId  = measInfo->hdr.transId;
      measReq.measType = measInfo->measType;
      measReq.timePrd  = measInfo->timePrd;
      measReq.cellId   = measInfo->cellId;
      measReq.t.prbReq.numQci = measInfo->avgPrbQciUl.numQci;
      for (idx = 0; idx < measInfo->avgPrbQciUl.numQci; idx++)
      {
        qciVal = measInfo->avgPrbQciUl.qci[idx];
        cell->qciArray[qciVal].qci = measInfo->avgPrbQciUl.qci[idx];
        measReq.t.prbReq.qci[idx] = qciVal;
      }
      /* Send measReq to MAC */
      rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
      RgSchMacL2Meas(&pst, &measReq);
   }
   if(measInfo->measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL)
   {
     for (idx = 0; idx < measInfo->nmbActvUeQciUl.numQci; idx++)
     {
        qciVal = measInfo->nmbActvUeQciUl.qci[idx];
        cell->qciArray[qciVal].qci  = measInfo->nmbActvUeQciUl.qci[idx];
     }
   }
   if(measInfo->measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL)
   {
     for (idx = 0; idx < measInfo->nmbActvUeQciDl.numQci; idx++)
     {
        qciVal = measInfo->nmbActvUeQciDl.qci[idx];
        cell->qciArray[qciVal].qci = measInfo->nmbActvUeQciDl.qci[idx];
     }
   }
   RETVALUE(ROK);
} /* rgSchL2mMeasReq */

/**
 * @brief This function calculates the measurement for differnt measurement type
 * and send the end result to the layer manager
 *
 * @details
 *
 *     Function : rgSCHL2Meas
 *     
 *  @param[in] RgSchCellCb  *cell
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHL2Meas
(
RgSchCellCb  *cell
)
#else
PUBLIC S16 rgSCHL2Meas(cell)
RgSchCellCb  *cell;
#endif
{
   CmLList           *node = NULLP;
   RgSchL2MeasCb     *measCb = NULLP;
   U8                idx;
   LrgSchMeasCfmInfo measCfm;
   U8                qciVal = 0;
   U8                sfDiff;

   TRC3(rgSCHL2Meas)

   node = cell->l2mList.first;
   cmMemset((U8 *)&measCfm, 0, sizeof(LrgSchMeasCfmInfo));
   while(node != NULLP)
   {
      measCb = (RgSchL2MeasCb *)node->node;
      if (RGSCH_CALC_SF_DIFF(cell->crntTime, measCb->startTime) >= 
          measCb->measReq.timePrd)
      {
         if((measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL))
         {
            if(measCb->cfmRcvd)
            {
               rgSchFillL2MeasCfm(cell, measCb, &measCfm);
            }
            else
            {
               node = node->next;
               continue;
            }
         }
         else
         {
            rgSchFillL2MeasCfm(cell, measCb, &measCfm);
         }
         RgMiLrgSchL2MeasCfm(&(rgSchCb[cell->instIdx].rgSchInit.lmPst),
                             &measCfm);
         cmMemset((U8 *)&measCfm, 0, sizeof(LrgSchMeasCfmInfo));
         /* Delete this measCb from the list */
         cmLListDelFrm(&cell->l2mList, &measCb->measLnk);
         node = node->next;
          /* ccpu00117052 - MOD - Passing double pointer
          for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&measCb,
               sizeof(RgSchL2MeasCb));
      }
      else
      {
         if((measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_DL))
         {
            rgSchL2CalDlPrbCount(cell);
         }
         if((measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_UL))
         {
            rgSchL2CalUlPrbCount(cell);
         }
         if(measCb->measReq.measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL)
         {
            sfDiff = RGSCH_CALC_SF_DIFF(cell->crntTime, measCb->startTime);

            if((sfDiff % measCb->measReq.nmbActvUeQciDl.sampPrd) == 0)
            {
               for (idx = 0; idx < measCb->measReq.nmbActvUeQciDl.numQci; 
                    idx++)
               {
                  qciVal = measCb->measReq.nmbActvUeQciDl.qci[idx];
                  cell->qciArray[qciVal].dlTotal_UeCount +=
                  cell->qciArray[qciVal].dlUeCount;
                  cell->qciArray[qciVal].dlUeCount = 0;
               }
            }
         }
         if(measCb->measReq.measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL)
         {
            sfDiff = RGSCH_CALC_SF_DIFF(cell->crntTime , measCb->startTime);
            if((sfDiff % measCb->measReq.nmbActvUeQciUl.sampPrd) == 0)
            {
               for (idx = 0; idx < measCb->measReq.nmbActvUeQciUl.numQci; 
                    idx++)
               {
                   qciVal = measCb->measReq.nmbActvUeQciUl.qci[idx];
                   cell->qciArray[qciVal].ulTotal_UeCount += 
                   cell->qciArray[qciVal].ulUeCount;
                   cell->qciArray[qciVal].ulUeCount = 0;
               }
            }
         }
         node = node->next;
      }/* end of else */
   }/* end of while */
   RETVALUE(ROK);
} /* rgSCHL2MEas */
#endif /* LTE_L2_MEAS */
/**********************************************************************
 
         End of file:     gk_sch_l2m.c@@/main/1 - Sat Jul 30 02:21:43 2011
 
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
/main/1      ---        ap               1. Created for LTE MAC Rel 3.1
*********************************************************************91*/

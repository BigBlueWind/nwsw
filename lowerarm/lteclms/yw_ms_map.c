/********************************************************************16**

                         (c) COPYRIGHT 2010 by 
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

     Name:     LTE-SCL Layer 
  
     Type:     C souce file
  
     Desc:     SCL for Trillium to Silicon Rx Mapping. 

     File:     ys_rxmap.c 

     Sid:      

     Prg:      mraj 

**********************************************************************/

/* Trillium Includes */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
  
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list defines  */
#include "cm_hash.h"       /* Common hashlist defines */
#include "cm_lte.h"        /* Common LTEE defines */
#include "tfu.h"
#include "ctf.h"
#include "lys.h"

#ifdef TENB_AS_SECURITY
#include "spaccdrv.h" /*sumanth*/
#endif

#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"        /* Common LTE includes */
#include "cm_lib.x"
#include "tfu.x"
#include "ctf.x"
#include "lys.x"

/* Silicon Includes */
#include "LtePhyL2Api.h"
#include "apidefs.h"

#include "ys_ms_err.h"        /* CL error header file */
#include "ys_ms.h"
#include "ys_ms.x"

/*ys004.102 :  Merged MSPD code with phy 1.7 */

U8 ysUeModulation[YS_NUM_MODULATION]={0, BPSK, QPSK, QPSK, QPSK, QPSK};


/**
 *     @brief This function is used to 
 *            
 *            
 *     @param[in] ulSubFrDesc  [PHY]Sub Frame Descriptor
 *     @param[in] l2Msg        [MAC]Message from MAC
 *     @param[in] cellCb       [CL] Convergence Layer
 *     @param[in] event        [CL] Event triggered
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 *
 **********************************************************/


#define TODO 0x01;

/* Maps from TFU values to PHY values */
enum ModulationOptions ModulationMap[3] = 
{
	QPSK, QAM16, QAM64
};

#ifdef TFU_UPGRADE
PRIVATE Void ysMsMapUeCtrlOnPusch ARGS((
YsCellCb           *ysCellCb,
YsUeCb             *ysUeCb,
TfuUePuschRecpReq  *puschRecpReq,
PULCHANDESC        pPuschChan
));
#endif
#ifndef TFU_UPGRADE
PRIVATE S16 ysMsResetUeSfInfo ARGS((YsUeSubFrInfo *ueSubFrInfo));
PRIVATE Void ysMsMapUeCtrlOnPusch ARGS((YsCellCb *ysCellCb,YsUeCb *ysUeCb,PULCHANDESC pPuschChan, U8 recpsf));

#if 0
/* 
*
*       Fun:   ysMsMapRachCtrl
*  
*       Desc:  Map RACH Control parameters
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE void ysMsMapRachCtrl
(
RACHCTRL         *rachCtl,
CtfCellCfgInfo   *cellCfg
)
#else
PRIVATE void ysMsMapRachCtrl(srsDed, cellCfg)
RACHCTRL         *rachCtl;
CtfCellCfgInfo   *cellCfg;
#endif
{
   CtfPrachCfgInfo          *prachCfg;

   TRC2(ysMsMapRachCtrl)

   if ((rachCtl->prachEnable = cellCfg->prachCfg.pres))
   {
      prachCfg = &cellCfg->prachCfg;
      rachCtl->rootSequenceIndex         = prachCfg->rootSequenceIndex;
      rachCtl->prachConfigIndex          = prachCfg->prachCfgIndex;
      rachCtl->highSpeedFlag             = prachCfg->highSpeedFlag;
      rachCtl->zeroCorrelationZoneConfig = prachCfg->zeroCorrelationZoneCfg;
      rachCtl->prachFreqOffset           = prachCfg->prachFreqOffset;
   }
   RETVOID;
} /* ysMsMapRachCtrl */
#endif


/* 
*
*       Fun:   ysMsMapSrsDedInfo 
*  
*       Desc:  Mapping of UE specific SR
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE void ysMsMapSrsDedInfo
(
YsUeCb           *ysUeCb,
CtfCellCfgInfo   *cellCfg,
SRSDED           *srsDed
)
#else
PRIVATE void  ysMsMapSrsDedInfo(srsDed, cellCfg, ysUeCb)
YsUeCb           *ysUeCb;
CtfCellCfgInfo   *cellCfg;
SRSDED           *srsDed;
#endif
{
   CtfDedSrsUlCfgInfo           *srsDedCfg;

   TRC2(ysMsMapSrsDedInfo)

   if (srsDed->enableDisable = ysUeCb->ueCfg.srsUlCfg.pres)
   {
      srsDedCfg = &ysUeCb->ueCfg.srsUlCfg;
      srsDed->srsBandwidth        = srsDedCfg->dedSrsSetup.srsBw;
      srsDed->srsHoppingBandwidth = srsDedCfg->dedSrsSetup.srsHopngBw;
      srsDed->transmissionComb    = srsDedCfg->dedSrsSetup.txComb;
      srsDed->cyclicShift         = srsDedCfg->dedSrsSetup.cyclicShift;
      srsDed->freqDomainPosition  = srsDedCfg->dedSrsSetup.freqDmnPos;
      srsDed->duration            = srsDedCfg->dedSrsSetup.duration;
      srsDed->srsConfigIndex      = srsDedCfg->dedSrsSetup.srsCfgIdx;
   }

   RETVOID;
} /* ysMsMapSrsDedInfo */

/*
*       Fun:  ysMsGetAndResetCwCount
*  
*       Desc:
*
*       Ret:
*
*       Notes: None
*
*       File:
*/
#ifdef ANSI
PRIVATE U8 ysMsGetAndResetCwCount
(
YsUeCb           *ysUeCb,
U8                recpSubframe
)
#else
PRIVATE U8 ysMsGetAndResetCwCount(ysUeCb, recpSubframe)
YsUeCb           *ysUeCb;
U8                recpSubframe;
#endif
{
   U8  cwCount;
   U8  dataSubframe = recpSubframe + YS_NUM_SUB_FRAMES - 4;
   if (dataSubframe >= YS_NUM_SUB_FRAMES)
      dataSubframe -= YS_NUM_SUB_FRAMES;
   cwCount = ysUeCb->cwCount[dataSubframe];
#ifndef HARQ_STATISTICS
   ysUeCb->cwCount[dataSubframe] = 0;
#endif
   return cwCount;
}

/*
*
*       Fun:  ysMsSetPucchParams
*  
*       Desc:  Calculate Pucch Format type
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE S16 ysMsSetPucchParams
(
YsUeSubFrInfo    *ueSubFrInfo,
Bool              simultAckNack,
PUCCHDEDCTL      *pucchDedCtlInfo,
YsUeCb           *ysUeCb,
U8                recpSubFrameNo
)
#else
PRIVATE S16 ysMsSetPucchParams(ueSubFrInfo, simultAckNack, pucchDedCtlInfo,ysUeCb,recpSubFrameNo)
YsUeSubFrInfo    *ueSubFrInfo;
Bool              simultAckNack;
PUCCHDEDCTL      *pucchDedCtlInfo ;
YsUeCb           *ysUeCb ;
U8                recpSubFrameNo ;
#endif
{
   /* 36.211 Table 5.4-1 */

   TRC2(ysMsSetPucchParams)

   pucchDedCtlInfo->simSRHarq = 0;
   if (ueSubFrInfo->harqReqInfo)
   {
      U8 cwCount = ysMsGetAndResetCwCount(ysUeCb, recpSubFrameNo);
      if (ueSubFrInfo->cqiPres && simultAckNack)
      {

         if (cwCount > 1)
         {
            ueSubFrInfo->pucchFormat = FORMAT2B;
            pucchDedCtlInfo->formatType = FORMAT2B;
         }
         else
         {
            ueSubFrInfo->pucchFormat = FORMAT2A;
            pucchDedCtlInfo->formatType = FORMAT2A;
         }
      } /* harq + cqi */
      else 
      {
         if (cwCount > 1)
         {
            ueSubFrInfo->pucchFormat = FORMAT1B;
            pucchDedCtlInfo->formatType = FORMAT1B;
         }
         else
         {
            ueSubFrInfo->pucchFormat = FORMAT1A;
            pucchDedCtlInfo->formatType = FORMAT1A;
         }
      }/* only Harq */
	  if (ueSubFrInfo->srPres)
	  {
	     pucchDedCtlInfo->simSRHarq = 1;
	  }
	  RETVALUE(ROK);
   }
   if (ueSubFrInfo->cqiPres)
   {  
      ueSubFrInfo->pucchFormat = FORMAT2;
      pucchDedCtlInfo->formatType = FORMAT2;
	  RETVALUE(ROK);
   } /* Only Cqi */
   if(ueSubFrInfo->srPres)
   {
      ueSubFrInfo->pucchFormat = FORMAT1;
      pucchDedCtlInfo->formatType = FORMAT1;
	  RETVALUE(ROK);
   } /* Only Sr */

   /* Negative SR Case + Format 2b case unhandled */
   /* Format 1b unsupported */
   ueSubFrInfo->pucchFormat = FORMAT1;
   pucchDedCtlInfo->formatType = FORMAT1;
   RETVALUE(ROK);

} /* ysMsSetPucchParams */

/* 
*
*       Fun:  ysMsMapPucchDedCtl
*  
*       Desc:  Map Ue Specific Pucch Control
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/

#ifdef ANSI
PRIVATE void ysMsMapPucchDedCtl
(
YsUeCb           *ysUeCb,
CtfCellCfgInfo   *cellCfg,
PUCCHDEDCTL      *pucchDedCtl,
U8                recpSubframeNo
)
#else
PRIVATE void  ysMsMapPucchDedCtl(ysUeCb, cellCfg, pucchDedCtl,recpSubframeNo)
YsUeCb           *ysUeCb;
CtfCellCfgInfo   *cellCfg;
PUCCHDEDCTL      *pucchDedCtl;
U8                recpSubframeNo
#endif
{
   CtfDedPucchCfgInfo        *pucchDedCfg;
   CtfCqiRptModePeriodic     *periodicRpt = NULLP;
   YsUeSubFrInfo             *ueSubFrInfo;
   Bool                       simultAckNack = FALSE;

   TRC2(ysMsMapPucchDedCtl)

   ueSubFrInfo = &ysUeCb->subFrInfo;

   if (ysUeCb->ueCfg.cqiRptCfg.pres && ysUeCb->ueCfg.cqiRptCfg.reportingMode == \
                  CTF_CQI_RPTMODE_PRDIOC && ueSubFrInfo->cqiPres )
   {
      periodicRpt = &ysUeCb->ueCfg.cqiRptCfg.reportMode.periodicRpt;
      simultAckNack = periodicRpt->cqiSetup.simultaneousAckNack;
   }
   if (ysUeCb->ueCfg.pucchCfg.pres)
   {
      pucchDedCfg =  &ysUeCb->ueCfg.pucchCfg;

      /* Tobe runtime */
      ysMsSetPucchParams(ueSubFrInfo, simultAckNack, pucchDedCtl,ysUeCb,recpSubframeNo);

      /* RepFact E {2,4,6} */ 
      pucchDedCtl->ackNackRepetition     = pucchDedCfg->pucchSetup.repFact?1:0; 
      pucchDedCtl->repetitionFactor      = pucchDedCfg->pucchSetup.repFact;
      /* TODO get from recep req */
      pucchDedCtl->n1PucchANRep          = pucchDedCfg->pucchSetup.n1PUCCHRep;
	  pucchDedCtl->msPucchSrSel          = 0;
   }/* if pucch pres */
   if (periodicRpt)
   {
      pucchDedCtl->cqiPUCCHResourceIndex = \
                                     periodicRpt->cqiSetup.cqiPUCCHRsrcIndx;
      pucchDedCtl->dlCqiPmiSizeBits = 4; /* currently for wideband CQI */
   }/* Cqi Pres */
   if (ysUeCb->ueCfg.dedSRCfg.pres && ueSubFrInfo->srPres)
   {
      pucchDedCtl->srPUCCHResourceIndex  = \
                                     ysUeCb->ueCfg.dedSRCfg.dedSrSetup.srPUCCHRi;
   } /* Sr pres */

   RETVOID;
} /* ysMsMapPucchDedCtl */


/*
*       Fun:  ysMsMapPucchDedRb
*  
*       Desc:  Determine PUCCH RB for the UE
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE void ysMsMapPucchDedRb
(
YsCellCb         *ysCellCb,
YsUeCb           *ysUeCb,
ULCTRLCHDESC     *ulCtlChDesc
)
#else
PRIVATE void  ysMsMapPucchDedRb(ysCellCb, ysUeCb, ulCtlChDesc)
YsCellCb         *ysCellCb;
YsUeCb           *ysUeCb;
ULCTRLCHDESC     *ulCtlChDesc;
#endif
{
  
   YsUeSubFrInfo  *ysSubFrInfo;
   U8             c = ysCellCb->cellCfg.txCfg.cycPfx == CTF_CP_NORMAL ? 3:2;
   U32            n1Pucch = 0; 
   U32            n2Pucch = 0;
   Bool           isFormat1 =FALSE;
   U8             nScPerRb = 12;
   U8             n2Rb = ysCellCb->preDefVal.ulSubFrCmnCtrl.nRBCQI;
   U8             n1Cs = ysCellCb->preDefVal.ulSubFrCmnCtrl.nCSAn;
   U8             deltaShiftPucch = ysCellCb->preDefVal.ulSubFrCmnCtrl.deltaPUCCHShift;
   U8             ulBw = ysCellCb->cellInfo.ulBw;
   U8             mVal;

   TRC2(ysMsMapPucchDedRb)

   ysSubFrInfo = &ysUeCb->subFrInfo;

   switch(ysSubFrInfo->pucchFormat)
   {
      case FORMAT1: 
      {
         n1Pucch = ysUeCb->ueCfg.dedSRCfg.dedSrSetup.srPUCCHRi;
		 ulCtlChDesc->pucchDedCtl.harqPucchIndex = n1Pucch;
         isFormat1 = TRUE;  
         break;
      }
      case FORMAT1A:
      {
         n1Pucch = ysSubFrInfo->harqReqInfo->t.pucchRecpReq.t.nCce + 
            ysCellCb->preDefVal.ulSubFrCmnCtrl.n1PucchAN;
		 ulCtlChDesc->pucchDedCtl.harqPucchIndex = n1Pucch;
		 ulCtlChDesc->pucchDedCtl.harqSizebits = 1;
         isFormat1 = TRUE;
         break;
      }
      case FORMAT1B:
      {
         n1Pucch = ysSubFrInfo->harqReqInfo->t.pucchRecpReq.t.nCce + 
            ysCellCb->preDefVal.ulSubFrCmnCtrl.n1PucchAN;
		 ulCtlChDesc->pucchDedCtl.harqPucchIndex = n1Pucch;
		 ulCtlChDesc->pucchDedCtl.harqSizebits = 2;
         isFormat1 = TRUE;
         break;
      }
      case FORMAT2:
      case FORMAT2A:
      case FORMAT2B:
      { 
         n2Pucch = ysUeCb->ueCfg.cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPUCCHRsrcIndx;
         break;   
      }   
   }

   if (isFormat1)
   {
      U8 shift = (c * n1Cs/deltaShiftPucch);
	  mVal = n1Pucch < shift ? n2Rb : ((n1Pucch - shift)/(c * nScPerRb/deltaShiftPucch) + n2Rb + (n1Cs + 7)/8);
	  ulCtlChDesc->pucchDedCtl.n1PUCCHResourceIndex = n1Pucch;
	  if (mVal != 1)
	  	  MSPD_DBG("mVal %u n1pucch %u\n", mVal, n1Pucch);
   }
   else
   {
       mVal = n2Pucch/nScPerRb;
   }

   if (mVal % 2 == 0)
   {
      ulCtlChDesc->startRes = mVal/2;
   }
   else
   {
      ulCtlChDesc->startRes = ulBw - 1 - (mVal/2);
   }
   
   RETVOID;
} /* ysMsMapPucchDedRb*/

/* 
*
*       Fun:  ysMsMapCqiPmiRpt 
*  
*       Desc:  Map Ue Specific Pucch Control
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE void ysMsMapCqiPmiRpt
(
YsUeCb           *ysUeCb,
CQIPMIRIRPT      *cqiPmiRpt
)
#else
PRIVATE void ysMsMapCqiPmiRpt(ysUeCb, cqiPmiRpt)
YsUeCb           *ysUeCb;
CQIPMIRIRPT      *cqiPmiRpt;
#endif
{

   TRC2(ysMsMapCqiPmiRpt)

   RETVOID;
} /* ysMsMapCqiPmiRpt */

/* 
*
*       Fun:  ysMsMapCtlChanDesc
*  
*       Desc:  Map Control Channel Descriptor
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE void ysMsMapCtlChanDesc
(
YsCellCb         *ysCellCb,
YsUeCb           *ysUeCb,
ULCTRLCHDESC     *ulCtlChDesc,
U8                recpSubfameNo
)
#else
PRIVATE void ysMsMapCtlChanDesc(ulCtlChDesc, ysUeCb,cellCfg,recpSubfameNo)
YsCellCb         *ysCellCb;
YsUeCb           *ysUeCb;
ULCTRLCHDESC     *ulCtlChDesc;
U8                recpSubfameNo ;
#endif
{
   TRC2(ysMsMapCtlChanDesc)

   ulCtlChDesc->txpowerControl         = YS_TX_PWR_CNTRL;
   ulCtlChDesc->crcLength              = YS_PUCCH_CRC_LEN;
   ulCtlChDesc->channelType            = PUCCH;
   ulCtlChDesc->reserved               = PAD;
   /* 36.211 Section 5.4 */

   /*Changing it to C-RNTI for UE using ueId, replacing ysCellCb->cellId */
   ulCtlChDesc->scrmblerInitValue      = ysUeCb->ueId;
   ulCtlChDesc->codingDescriptor       = YS_PUCCH_COD_DES;
   ulCtlChDesc->blockCodeConcatenation = YS_PUCCH_BLK_CAT;
   ulCtlChDesc->mcsType                = YS_PUCCH_MCS_TYP;
   /* MS_WORKAROUND: Pass recpSubfameNo to figure out number of harq bits, not needed once using TFU_UPGRADE */
   ysMsMapPucchDedCtl(ysUeCb, &ysCellCb->cellCfg, &ulCtlChDesc->pucchDedCtl,recpSubfameNo);
   ulCtlChDesc->modulationType 
      = ysUeModulation[ysUeCb->subFrInfo.pucchFormat];

   /* The entire structure ulCtlChDesc->cqiPmiRiRpt is unused, so don't
    * care about it */

   ulCtlChDesc->numberofEntries = YS_SINGLE_MAPPING;

   ysMsMapPucchDedRb(ysCellCb, ysUeCb, ulCtlChDesc);
  
   ulCtlChDesc->numRes          = 0;
   cmMemcpy((U8 *)&ulCtlChDesc->timAdvErrInfo, (U8 *)&ysUeCb->timingAdvErrInfo, sizeof(TIMADVERRINF));
   
   RETVOID;
} /*ysMsMapCtlChanDesc*/

/* 
*
*       Fun:  ysMsMapMapInfo
*  
*       Desc:  Map  Rb Start numRb
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE void ysMsMapMapInfo
(
TfuUePuschRecpReq *puschRecpReq,
MAPPINGINFO       *mapInfo
)
#else
PRIVATE void  ysMsMapMapInfo(puschRecpReq, mapInfo)
TfuUePuschRecpReq *puschRecpReq;
MAPPINGINFO       *mapInfo;
#endif
{
   U16       idx;

   TRC2(ysMsMapMapInfo)

   mapInfo->numberofEntries = YS_SINGLE_MAPPING;

   for (idx=0; idx<mapInfo->numberofEntries; idx++)
   {
      mapInfo->reselmInfo[idx].startRes = puschRecpReq->rbStart;
      mapInfo->reselmInfo[idx].numRes   = puschRecpReq->numRb;
   }

   RETVOID;
} /*ysMsMapMapInfo*/

/* 
*
*       Fun:  ysMsMapMsg3MapInfo
*  
*       Desc:  Map  Rb Start numRb
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE void ysMsMapMsg3MapInfo
(
TfuUeMsg3RecpReq  *msg3RecpReq,
MAPPINGINFO       *mapInfo
)
#else
PRIVATE void  ysMsMapMsg3MapInfo(msg3RecpReq, mapInfo)
TfuUePuschRecpReq *msg3RecpReq;
MAPPINGINFO       *mapInfo;
#endif
{
   U16       idx;

   TRC2(ysMsMapMapInfo)

   mapInfo->numberofEntries = YS_SINGLE_MAPPING;

   for (idx=0; idx<mapInfo->numberofEntries; idx++)
   {
      mapInfo->reselmInfo[idx].startRes = msg3RecpReq->rbStart;
      mapInfo->reselmInfo[idx].numRes   = msg3RecpReq->numRb;
   }

   RETVOID;
} /*ysMsMapMsg3MapInfo*/


/* 
*
*       Fun:  ysMsMapPuschDed
*  
*       Desc:  Map  Ue specific Pusch Details
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE void ysMsMapPuschDed
(
YsUeCb           *ysUeCb,
TfuUeRecpReqInfo *recpReqInfo,
PUSCHDED         *puschDed
)
#else
PRIVATE void ysMsMapPuschDed(ysUeCb, recpReqInfo, puschDed)
YsUeCb           *ysUeCb;
TfuUeRecpReqInfo *recpReqInfo;
PUSCHDED         *puschDed;
#endif
{
   CtfDedPuschCfgInfo     *puschDedCfg;
   U8                     n2DmrsArr[8] = {0,6,3,4,2,8,10,9};
   TRC2(ysMsMapPuschDed)

   puschDedCfg = &ysUeCb->ueCfg.puschCfg;
   puschDed->betaOffsetACKIndex = puschDedCfg->betaOffsetAckIdx;
   puschDed->betaOffsetCQIIndex = puschDedCfg->betaOffsetCqiIdx;
   puschDed->betaOffsetRIIndex  = puschDedCfg->betaOffsetRiIdx;

   switch (recpReqInfo->type)
   {
      case TFU_RECP_REQ_PUSCH:
	  	puschDed->nDMRS2 = n2DmrsArr[recpReqInfo->t.puschRecpReq.nDmrs];
		break;
      case TFU_RECP_REQ_MSG3:
        puschDed->nDMRS2 = 0;
      break;
   }
   RETVOID;
} /* ysMsMapPuschDed */

/* 
*
*       Fun:  ysMsMapCrcInfo
*  
*       Desc:  Map  Ue specific CRC Info
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE void ysMsMapCrcInfo
(
YsUeCb           *ysUeCb,
CRCINFO          *crcInfo
)
#else
PRIVATE void ysMsMapCrcInfo(ysUeCb)
YsUeCb           *ysUeCb;
CRCINFO          *crcInfo;
#endif
{
   TRC2(ysMsMapCrcInfo)

   crcInfo->crcLength         = TWTYFOUR; /*LtePhyApi.h*/
   crcInfo->crcScrambling     = YS_CRC_SCRAMBLING;

   RETVOID;
} /* ysMsMapCrcInfo */

#if 0
/* 
*
*       Fun:  ysMsMapScrInfo
*  
*       Desc:  Map  Ue specific Scrambling Info
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE void ysMsMapScrInfo
(
YsCellCb         *cellCb,
YsUeCb           *ysUeCb,
SCRAMBLERINFO    *scrInfo
)
#else
PRIVATE void  ysMsMapScrInfo(cellCb, ysUeCb, scrInfo)
YsCellCb         *cellCb;
YsUeCb           *ysUeCb;
SCRAMBLERINFO    *scrInfo;
#endif
{
   TRC2(ysMsMapScrInfo)

   scrInfo->scramblerType     = 0; /*LtePhyApi.h*/
   scrInfo->scrinitValueinput = cellCb->cellId;

   RETVOID;
} /*ysMsMapScrInfo*/
#endif

/* 
*
*       Fun:  ysMsMapHarqInfo
*  
*       Desc:  Map  Ue specific Harq Info
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE void ysMsMapHarqInfo
(
TfuUePuschRecpReq  *puschRecpReq,
HARQINFO           *harqInfo
)
#else
PRIVATE void  ysMsMapHarqInfo(puschRecpReq, harqInfo)
TfuUePuschRecpReq  *puschRecpReq;
HARQINFO           *harqInfo;
#endif
{
   TRC2(ysMsMapHarqInfo)

   harqInfo->reserved = PAD;
   harqInfo->rV       = puschRecpReq->rv;
   harqInfo->nDi      = puschRecpReq->ndi;
   /*hardcoding to 1 for now due to a bug in PHY*/
   /* HARQ_UL: Removing hard coding of NDI below */
   /* HARQ_UL: NDI = 1 implies transmission while 0 means re-transmission */
   harqInfo->nDi      = !puschRecpReq->isRtx;
   
   harqInfo->flushReq = !puschRecpReq->isRtx;

   RETVOID;
} /* ysMsMapHarqInfo */

/* 
*
*       Fun:  ysMsMapMsg3HarqInfo
*  
*       Desc:  Map  Ue specific Harq Info
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE void ysMsMapMsg3HarqInfo
(
TfuUeMsg3RecpReq   *msg3RecpReq,
HARQINFO           *harqInfo
)
#else
PRIVATE void  ysMsMapMsg3HarqInfo(msg3RecpReq, harqInfo)
TfuUeMsg3RecpReq   *msg3RecpReq;
HARQINFO           *harqInfo;
#endif
{
   TRC2(ysMsMapHarqInfo)

   harqInfo->reserved = PAD;

   /* Invalid */
   harqInfo->rV       = 0;
   harqInfo->nDi      = 1;

   /* Flush Request mapping to Retransmission */
   harqInfo->flushReq = !msg3RecpReq->isRtx;

   RETVOID;
} /* ysMsMapHarqInfo */



#if 0
/* 
 *
 *       Fun:  ysMsUtlMapMcInfo
 *  
 *       Desc:  Map  Ue specific Modulation&Coding Info
 *
 *       Ret:   void 
 *
 *       Notes: None
 *
 *       File:  ys_ms_map.c
 *
 * */
#ifdef ANSI
PRIVATE void ysMsUtlMapMcInfo
(
YsUeCb           *ysUeCb,
CtfCellCfgInfo   *cellCfg,
MCINFO           *mcInfo
)
#else
PRIVATE void  ysMsUtlMapMcInfo(ysUeCb, cellCfg, mcInfo)
YsUeCb           *ysUeCb
CtfCellCfgInfo   *cellCfg;
MCINFO           *mcInfo;
#endif
{
   TRC2(ysMsUtlMapMcInfo)

   mcInfo->codingDescriptor       = TURBOCDR;
   mcInfo->blockCodeConcatenation = YS_BLK_CODE_CAT;
   mcInfo->modulationType         = TODO;
   mcInfo->mcsType                = TODO;
} /* End of ysMsUtlMapMcInfo */
#endif

/* 
 *
 *       Fun:  ysMsMapSubChInfoMsg3
 *  
 *       Desc:  Map  Ue specific SubChannel Info
 *
 *       Ret:   void 
 *
 *       Notes: None
 *
 *       File:  ys_ms_map.c
 *
 */
#ifdef ANSI
PRIVATE void ysMsMapSubChInfoMsg3
(
YsCellCb         *cellCb,
YsSubFrInfo      *ysSubFrInfo,
ULSUBCHINFO      *ulChanDesc,
U8               chIdx
)
#else
PRIVATE void  ysMsMapSubChInfoMsg3(cellCb, ysSubFrInfo, ulChanDesc, chIdx)
YsCellCb         *cellCb,
YsSubFrInfo      *ysSubFrInfo;
ULSUBCHINFO      *ulChanDesc;
U8               chIdx;
#endif
{
   TfuUeRecpReqInfo    *recpReqInfo;
   TfuUeMsg3RecpReq    *msg3RecpReq;
   MCINFO              *mcInfo;

   TRC2(ysMsMapSubChInfoMsg3)

   recpReqInfo      = &(ysSubFrInfo->msg3Lst.msg3RecpLst[chIdx]);

   mcInfo           =  &ulChanDesc->mcinfo;
   msg3RecpReq      =  &(recpReqInfo->t.msg3RecpReq);

   cmMemset((U8*)&(ulChanDesc->puschDed), 0, sizeof(PUSCHDED));
   cmMemset((U8*)&(ulChanDesc->cqiPmiRiRpt), 0, sizeof(CQIPMIRIRPT));

   mcInfo->codingDescriptor       = TURBOCDR;
   mcInfo->blockCodeConcatenation = YS_BLK_CODE_CAT;

   mcInfo->modulationType         = ModulationMap[(msg3RecpReq->modType/2) - 1];

   mcInfo->mcsType                = msg3RecpReq->mcs;

   ysMsMapMsg3HarqInfo(msg3RecpReq, &ulChanDesc->harqInfo);
   ysMsMapMsg3MapInfo(msg3RecpReq, &(ulChanDesc->mapInfo));


   ulChanDesc->crcInfo.crcLength         = TWTYFOUR; /*LtePhyApi.h*/
   ulChanDesc->crcInfo.crcScrambling     = YS_CRC_SCRAMBLING;

   ulChanDesc->scrInfo.scramblerType     = 0; /*LtePhyApi.h*/
   ulChanDesc->scrInfo.scrinitValueinput = recpReqInfo->rnti;

   ulChanDesc->reserved = PAD;

   //uart_printf("\n ysMsMapMsg3MapInfo sending MSG3 RX Vector (%d) (%d) \n", 
    //     cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);

} /* End of ysMsMapSubChInfoMsg3 */


/* 
 *
 *       Fun:  ysMsMapSubChInfo
 *  
 *       Desc:  Map  Ue specific SubChannel Info
 *
 *       Ret:   void 
 *
 *       Notes: None
 *
 *       File:  ys_ms_map.c
 *
 */
#ifdef ANSI
PRIVATE void ysMsMapSubChInfo
(
YsCellCb         *cellCb,
YsUeCb           *ysUeCb,
ULSUBCHINFO      *ulChanDesc
)
#else
PRIVATE void  ysMsMapSubChInfo(cellCb, ysUeCb, ulChanDesc)
YsCellCb         *cellCb,
YsUeCb           *ysUeCb,
ULSUBCHINFO      *ulChanDesc;
#endif
{
   TfuUeRecpReqInfo    *recpReqInfo;
   TfuUeMsg3RecpReq    *msg3RecpReq;
   TfuUePuschRecpReq   *puschRecpReq;
   MCINFO              *mcInfo;

   TRC2(ysMsMapSubChInfo)

   recpReqInfo      = ysUeCb->subFrInfo.schReqInfo;

   mcInfo           =  &ulChanDesc->mcinfo;
   puschRecpReq     =  &(recpReqInfo->t.puschRecpReq);
   msg3RecpReq      =  &(recpReqInfo->t.msg3RecpReq);

   cmMemset((U8*)&(ulChanDesc->puschDed), 0, sizeof(PUSCHDED));
   cmMemset((U8*)&(ulChanDesc->cqiPmiRiRpt), 0, sizeof(CQIPMIRIRPT));

   switch (recpReqInfo->type)
   {
      case TFU_RECP_REQ_PUSCH:
         mcInfo->codingDescriptor       = TURBOCDR;
         mcInfo->blockCodeConcatenation = YS_BLK_CODE_CAT;

         mcInfo->modulationType         = ModulationMap[(puschRecpReq->modType/2) - 1];

         mcInfo->mcsType                = puschRecpReq->mcs;
         ysMsMapHarqInfo(&recpReqInfo->t.puschRecpReq, &ulChanDesc->harqInfo);
		 if (ulChanDesc->harqInfo.rV)
         {
#ifndef HARQ_STATISTICS
   	        MSPD_DBG("UL Re-transmission @ (%d,%d) UeId(%d) hqPId(%d) rv(%d)\n",
             cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
             recpReqInfo->rnti,
		 	 recpReqInfo->t.puschRecpReq.harqProcId, 
		 	 ulChanDesc->harqInfo.rV);
#endif
         } 
         ysMsMapMapInfo(&(recpReqInfo->t.puschRecpReq), &(ulChanDesc->mapInfo));
      break;
      case TFU_RECP_REQ_MSG3:
         mcInfo->codingDescriptor       = TURBOCDR;
         mcInfo->blockCodeConcatenation = YS_BLK_CODE_CAT;
         mcInfo->modulationType         = msg3RecpReq->modType;
         mcInfo->mcsType                = msg3RecpReq->mcs;
         ysMsMapMsg3HarqInfo(msg3RecpReq, &ulChanDesc->harqInfo);
         ysMsMapMsg3MapInfo(msg3RecpReq, &(ulChanDesc->mapInfo));
      break;
   }
   ysMsMapCrcInfo(ysUeCb, &ulChanDesc->crcInfo);

   ulChanDesc->scrInfo.scramblerType     = 0; /*LtePhyApi.h*/
   ulChanDesc->scrInfo.scrinitValueinput = recpReqInfo->rnti;

   ulChanDesc->reserved = PAD;

   ysMsMapPuschDed(ysUeCb, recpReqInfo, &ulChanDesc->puschDed);

   if (ysUeCb->subFrInfo.cqiPres)
   {
      ysMsMapCqiPmiRpt(ysUeCb, &ulChanDesc->cqiPmiRiRpt);
   }
} /* End of ysMsMapSubChInfo */

/* 
 *
 *       Fun:  ysMsMapChanDesc
 *  
 *       Desc:  Map  Ue specific Dedicated Channel Descriptor
 *
 *       Ret:   void 
 *
 *       Notes: None
 *
 *       File:  ys_ms_map.c
 *
 */
#ifdef ANSI
PRIVATE void ysMsMapChanDesc
(
YsCellCb         *cellCb,
YsUeCb           *ysUeCb,
ULCHANDESC       *ulChanDesc,
U8               numCh
)
#else
PRIVATE void  ysMsMapChanDesc(cellCb, ysUeCb, ulChanDesc, numCh)
YsCellCb         *cellCb;
YsUeCb           *ysUeCb
ULSUBFRCMNCTRL   *ulChanDesc;
U8               numCh;
#endif
{
   TRC2(ysMsMapChanDesc)

   ulChanDesc->offsetNextCh    = 0;
   ulChanDesc->txpowerControl  = YS_TX_PWR_CNTRL;

   /* Warning: Hardcoding values */
   ulChanDesc->persistEnable   = 1;
   ulChanDesc->repeatCycle     = 0x01;
   ulChanDesc->channelType     = PUSCH;
   ulChanDesc->halfIterations  = 0x10;
   cmMemcpy((U8 *)&ulChanDesc->timAdvErrInfo, (U8 *)&ysUeCb->timingAdvErrInfo, sizeof(TIMADVERRINF));
   
   ysMsMapSubChInfo(cellCb, ysUeCb, &ulChanDesc->ulSubChInfo);

   RETVOID;
} /*ysMsMapChanDesc*/

/* 
 *
 *       Fun:  ysMsMapChanDescMsg3
 *  
 *       Desc:  Map  Ue specific Dedicated Channel Descriptor
 *
 *       Ret:   void 
 *
 *       Notes: None
 *
 *       File:  ys_ms_map.c
 *
 */
#ifdef ANSI
PRIVATE void ysMsMapChanDescMsg3
(
YsCellCb         *cellCb,
YsSubFrInfo      *ysSubFrInfo,
ULCHANDESC       *ulChanDesc,
U8               chIdx
)
#else
PRIVATE void  ysMsMapChanDescMsg3 (cellCb, ysSubFrInfo, ulChanDesc, chIdx)
YsCellCb         *cellCb;
ysSubFrInfo      *ysSubFrInfo;
ULSUBFRCMNCTRL   *ulChanDesc;
U8               chIdx;
#endif
{
   TRC2(ysMsMapChanDescMsg3)

   ulChanDesc->offsetNextCh    = 0;
   ulChanDesc->txpowerControl  = YS_TX_PWR_CNTRL;

   /* Warning: Hardcoding values */
   ulChanDesc->persistEnable   = 1;
   ulChanDesc->repeatCycle     = 0x01;
   ulChanDesc->channelType     = PUSCH;
   ulChanDesc->halfIterations  = 0x10;
   ulChanDesc->timAdvErrInfo.mSetDefaults = 1;

   ysMsMapSubChInfoMsg3(cellCb, ysSubFrInfo, &ulChanDesc->ulSubChInfo,chIdx);

   RETVOID;
} /*ysMsMapChanDescMsg3*/

/* HARQ_UL: Added timing information for UL HARQ handling */
/* 
 *
 *       Fun:   ysMsMapUlSubFrDesc
 *  
 *       Desc:  Fill UL SubFrame Descriptor
 *
 *       Ret:   void 
 *
 *       Notes: None
 *
 *       File:  ys_ms_map.c
 *
 */
#ifdef ANSI
PUBLIC S16 ysMsMapUlSubFrDesc
(
YsCellCb         *cellCb,
YsSubFrInfo      *ysSubFrInfo,
ULSUBFRDESC      *ulSubFrDesc, 
MsgLen           *msgLen, 
CmLteTimingInfo  timingInfo
)
#else
PUBLIC S16 ysMsMapUlSubFrDesc(cellCb, ysSubFrInfo, ulSubFrDesc, msgLen, timingInfo)
YsCellCb         *cellCb;
YsSubFrInfo      *ysSubFrInfo;
ULSUBFRDESC      *ulSubFrDesc; 
MsgLen           *msgLen;
CmLteTimingInfo  timingInfo;
#endif
{
   CtfCellCfgInfo              *cellCfg;
   YsUeLst                     *ueLst;
   YsUeCb                      *ueCb;
   U8                           idx;
   U8                          rxSduSf;
   U32                         numPuschCh = 0;
   U32                         numPucchCh = 0;
   U32                         numCh = 0;
   ULCHANDESC                  *ulCh;
   ULCTRLCHDESC                *ulCtlCh;
   SRSDED                      *srsInfo;
   RACHCTRL                    *rachCtrl;
  
   
   TRC2(ysMsMapUlSubFrDesc)

   ueLst                       = &ysSubFrInfo->ueLst;
   cellCfg                     = &cellCb->cellCfg;
   ulSubFrDesc->frameNumber    = timingInfo.sfn;
   ulSubFrDesc->subframeNumber = timingInfo.subframe;
   ulSubFrDesc->subframeType   = ULRX;

   YS_DBG_INFO((_ysp, "ysMsMapUlSubFrDesc(): (%d) (%d);\n", 
            cellCb->timingInfo.sfn, cellCb->timingInfo.subframe));

   ulSubFrDesc->antennaPortcount = YS_MS_GET_NUM_TX_ANT(cellCfg->antennaCfg); 

   /* Getting values from stored predefined configuration */
   cmMemcpy((U8*) &ulSubFrDesc->ulSfrCtrl,
            (U8*) &cellCb->preDefVal.ulSubFrCmnCtrl,
            sizeof(ULSUBFRCMNCTRL));
   
   ulCh    = ulSubFrDesc->ulCh;
   ulCtlCh = (ULCTRLCHDESC *)&ulSubFrDesc->ulCh[ysSubFrInfo->msg3Lst.numMsg3 + ueLst->numPusch];
   srsInfo = (SRSDED *)&ulCtlCh[ueLst->numPucch];

   /* HARQ_TODO: Remove hardcoding */
   rxSduSf = (timingInfo.subframe + 2) % YS_NUM_SUB_FRAMES;
    for (idx = 0; idx < ysSubFrInfo->msg3Lst.numMsg3; idx++)
   {
      YS_DBG_PARAM((_ysp, "Received reception request for Msg3 gotRACH = (%d) \n", cellCb->gotRACH));
      ysMsMapChanDescMsg3(cellCb, ysSubFrInfo, 
            &ulCh[numPuschCh], idx);
      ulCtlCh[numPuschCh].offsetNextCh = 
         (U32)&ulCh[numPuschCh]-(U32)ulSubFrDesc;
      ysMsMapRntiToChan(cellCb,ysSubFrInfo->msg3Lst.msg3RecpLst[idx].rnti, timingInfo.subframe, numCh);
      ulCh[numPuschCh].channelId       = numCh;
      numPuschCh++;
      numCh++;
      cellCb->isCrcExptd[rxSduSf] = TRUE;
    }

   for (idx=0; idx<ueLst->numPusch; idx++)
   {
      ueCb = ueLst->puschLst[idx];

      /* HARQ_UL: If PUSCH/Msg3 reception request, mark CRC information */
      ysMsMapChanDesc(cellCb, ueCb, &ulCh[numPuschCh], numPuschCh);
      ulCtlCh[numPuschCh].offsetNextCh = 
         (U32)&ulCh[numPuschCh]-(U32)ulSubFrDesc;
      if (ueCb->subFrInfo.harqReqInfo || ueCb->subFrInfo.cqiPres)
      {
         ysMsMapUeCtrlOnPusch(cellCb, ueCb, &ulCh[numPuschCh], timingInfo.subframe);
      }
      cellCb->isCrcExptd[rxSduSf] = TRUE;
    
      ysMsMapRntiToChan(cellCb, ueCb->ueId, timingInfo.subframe, numCh);
      ulCh[numPuschCh].channelId = numCh;
      numPuschCh++;
      numCh++;
      
      cmMemcpy((U8*)&ueCb->ueSubFrInfo[cellCb->timingInfo.subframe], 
                           (U8*)&ueCb->subFrInfo, sizeof(YsUeSubFrInfo));
      ysMsResetUeSfInfo(&ueCb->subFrInfo);
   }

   for (idx=0; idx<ueLst->numPucch; idx++)
   {
      ueCb = ueLst->pucchLst[idx];

      /* MS_WORKAROUND: Pass recpSubfameNo to figure out number of harq bits, not needed once using TFU_UPGRADE */
      ysMsMapCtlChanDesc(cellCb, ueCb, &ulCtlCh[numPucchCh],timingInfo.subframe);
      ulCtlCh[numPucchCh].offsetNextCh = 
         (U32)&ulCtlCh[numPucchCh]-(U32)ulSubFrDesc;
      ulCtlCh[numPucchCh].channelId = numCh;
      ysMsMapRntiToChan(cellCb, ueCb->ueId, timingInfo.subframe, numCh);
      numPucchCh++;
      numCh++;

      cmMemcpy((U8*)&ueCb->ueSubFrInfo[cellCb->timingInfo.subframe], 
                           (U8*)&ueCb->subFrInfo, sizeof(YsUeSubFrInfo));
      ysMsResetUeSfInfo(&ueCb->subFrInfo);
   }

   for (idx=0; idx<ueLst->numSrs; idx++)
   {
      ueCb = ueLst->srsLst[idx];

      ysMsMapSrsDedInfo(ueCb, cellCfg, &srsInfo[ysSubFrInfo->numSrs]);
      ysSubFrInfo->numSrs++;

      cmMemcpy((U8*)&ueCb->ueSubFrInfo[cellCb->timingInfo.subframe], 
                           (U8*)&ueCb->subFrInfo, sizeof(YsUeSubFrInfo));
      ysMsResetUeSfInfo(&ueCb->subFrInfo);
   }

   ulCtlCh[numPucchCh - 1].offsetNextCh = 0x04;
   ulCh[numPuschCh - 1].offsetNextCh = 0x04;

   rachCtrl = (RACHCTRL *)&srsInfo[ysSubFrInfo->numSrs];

   ysSubFrInfo->numCh = numCh;
   ysSubFrInfo->numCtrl = numPucchCh;
   ulSubFrDesc->numberofChannelDescriptors =  ysSubFrInfo->numCh;
   ulSubFrDesc->numberOfCtrlChannelDescriptors  = ysSubFrInfo->numCtrl;
   ulSubFrDesc->numberSrsinSf                   = ysSubFrInfo->numSrs;

   ulSubFrDesc->offsetULCtrlChannels            = (U32)ulCtlCh - (U32)ulSubFrDesc;
   ulSubFrDesc->offsetsrsInfo                   = (U32)srsInfo - (U32)ulSubFrDesc;
   ulSubFrDesc->offsetRachCtrlStruct            = (U32)rachCtrl - (U32)ulSubFrDesc;
   ulSubFrDesc->offsetCustomFeatures            = 0;


   RETVALUE(ROK);
} /* ysMsMapUlSubFrDesc */


/* HARQ_UL: Added timing information for UL HARQ handling */
/* 
*
*       Fun:   ysMsMapRxVector
*  
*       Desc:  Create Rx Vector
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsUtlMapRxVector
(
YsCellCb         *cellCb,
YsSubFrInfo      *subFrInfo,
PULSUBFRDESC     *ulSubFrDesc,
CmLteTimingInfo  timingInfo
)
#else
PUBLIC S16 ysMsUtlMapRxVector(cellCb, subFrInfo, ulSubFrDesc, timingInfo)
YsCellCb       *cellCb;
YsSubFrInfo    *subFrInfo;
PULSUBFRDESC   *ulSubFrDesc;
CmLteTimingInfo timingInfo;
#endif
{
   MsgLen                    len;

   TRC2(ysMsUtlMapRxVector)

   ysMsMapUlSubFrDesc(cellCb, subFrInfo, (ULSUBFRDESC *)ulSubFrDesc, &len, timingInfo);

   RETVALUE(ROK);
} /* ysMsUtlMapRxVector */


/* 
*
*       Fun:   ysMsResetUeSfInfo
*  
*       Desc:  Reset subframe inforamtion per UE
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE S16 ysMsResetUeSfInfo
(
YsUeSubFrInfo      *ueSubFrInfo
)
#else
PRIVATE S16 ysMsResetUeSfInfo(ueSubFrInfo)
YsUeSubFrInfo    *ueSubFrInfo;
#endif
{
   cmMemset((U8*)ueSubFrInfo, 0, sizeof(*ueSubFrInfo));
   RETVALUE(ROK);
} /* ysMsResetUeSfInfo */

/* 
*
*       Fun:  ysMsMapUeCtrlOnPusch
*  
*       Desc:  Maps control information of the UE on PUSCH
*
*       Ret:  Void
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE Void ysMsMapUeCtrlOnPusch
(
YsCellCb         *ysCellCb,
YsUeCb           *ysUeCb,
PULCHANDESC      pPuschChan,
U8               recpsf
)
#else
PRIVATE Void ysMsMapUeCtrlOnPusch(ysCellCb, ysUeCb, pPuschChan)
YsCellCb         *ysCellCb;
YsUeCb           *ysUeCb;
PULCHANDESC      pPuschChan;
U8               recpsf;
#endif
{
   TRC2(ysMsMapUeCtrlOnPusch)

   pPuschChan->ulSubChInfo.puschDed.nsymi = 12;
   pPuschChan->ulSubChInfo.puschDed.nrbi = pPuschChan->ulSubChInfo.mapInfo.reselmInfo[0].numRes;
   if (ysUeCb->subFrInfo.harqReqInfo)
   {
      U8 cwCount = ysMsGetAndResetCwCount(ysUeCb, recpsf);
      cwCount = cwCount > 1 ? 2 : 1;
      pPuschChan->ulSubChInfo.puschDed.nACK = cwCount;
   }
   if (ysUeCb->subFrInfo.cqiPres)
   {
      /* pPuschChan->ulSubChInfo.puschDed.betaOffsetCQIIndex already updated */
      pPuschChan->ulSubChInfo.puschDed.nr1CQI = 4; /* '4' for wideband cqi */
   }
   
   RETVOID;
} /*ysMsMapUeCtrlOnPusch*/

#else /* TFU_UPGRADE*/


/* 
*
*       Fun:  ysMsMapUeCtrlOnPusch
*  
*       Desc:  Maps control information of the UE on PUSCH
*
*       Ret:  Void
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE Void ysMsMapUeCtrlOnPusch
(
YsCellCb           *ysCellCb,
YsUeCb             *ysUeCb,
TfuUePuschRecpReq  *puschRecpReq,
PULCHANDESC        pPuschChan
)
#else
PRIVATE Void ysMsMapUeCtrlOnPusch(ysCellCb, ysUeCb, puschRecpReq, pPuschChan)
YsCellCb          *ysCellCb;
YsUeCb            *ysUeCb;
TfuUePuschRecpReq *puschRecpReq;
PULCHANDESC       pPuschChan;
#endif
{
   PPUSCHDED pPuschDed = &pPuschChan->ulSubChInfo.puschDed;

   TRC2(ysMsMapUeCtrlOnPusch)   
   	
   pPuschDed->nsymi = 12;
   pPuschDed->nrbi = pPuschChan->ulSubChInfo.mapInfo.reselmInfo[0].numRes;
   pPuschDed->reserved = PAD;
   
   if (puschRecpReq->rcpInfo == TFU_PUSCH_DATA)
   {
      RETVOID;
   }

   /* Handle multiplexed Data and HARQ */
   if (puschRecpReq->rcpInfo == TFU_PUSCH_DATA_HARQ || 
   	   puschRecpReq->rcpInfo == TFU_PUSCH_DATA_HARQ_SRS ||
   	   puschRecpReq->rcpInfo == TFU_PUSCH_DATA_CQI_HARQ ||
   	   puschRecpReq->rcpInfo == TFU_PUSCH_DATA_CQI_HARQ_SRS
   	   )
   {
      TfuUePuschHqRecpInfo   *hqInfo = &puschRecpReq->hqInfo;
      pPuschDed->nACK = hqInfo->hqSz;      
	  pPuschDed->betaOffsetACKIndex = hqInfo->hqBetaOff;   
   }
   /* Handle multiplexed Data and CQI */
   if (puschRecpReq->rcpInfo == TFU_PUSCH_DATA_CQI ||
   	   puschRecpReq->rcpInfo == TFU_PUSCH_DATA_CQI_SRS ||
   	   puschRecpReq->rcpInfo == TFU_PUSCH_DATA_CQI_HARQ ||
   	   puschRecpReq->rcpInfo == TFU_PUSCH_DATA_CQI_HARQ_SRS
   	   )           
   {
	  TfuUePuschCqiRecpInfo  *cqiRiInfo = &puschRecpReq->cqiRiInfo;
	  pPuschDed->nr1CQI = cqiRiInfo->cqiPmiSzR1;
	  pPuschDed->nrg1CQI = cqiRiInfo->cqiPmiSzRn1;      
	  pPuschDed->betaOffsetCQIIndex = cqiRiInfo->cqiBetaOff;
	  pPuschDed->betaOffsetRIIndex = cqiRiInfo->riBetaOff;
	  if (cqiRiInfo->riSz.pres)
	     pPuschDed->nRI = cqiRiInfo->riSz.val;
	  pPuschChan->ulSubChInfo.cqiPmiRiRpt.cqiPUCCHResourceIndex = 0;
	  pPuschChan->ulSubChInfo.cqiPmiRiRpt.cqiFormatIndicatorPeriodic = 0;
	  pPuschChan->ulSubChInfo.cqiPmiRiRpt.cqiReportPeriodicEnable = 1;
	  /* TFU_UPGRADE_TODO */
	  #if 0
	  pPuschChan->ulSubChInfo.ulSubChInfo.cqiPmiRiRpt.cqiReportModeAperiodic = 0;
      pPuschChan->ulSubChInfo.cqiPmiRiRpt.nomPDSCHRSEPREOffset = 0;           
      pPuschChan->ulSubChInfo.cqiPmiRiRpt.cqipmiConfigIndex = 0;
      pPuschChan->ulSubChInfo.cqiPmiRiRpt.K = 0;
      pPuschChan->ulSubChInfo.cqiPmiRiRpt.riConfigIndex = 0;
      pPuschChan->ulSubChInfo.cqiPmiRiRpt.simultaneousAckNackAndCQI = 0;
      pPuschChan->ulSubChInfo.cqiPmiRiRpt.padding = PAD;
	  #endif
   }
 
   
   RETVOID;
} /*ysMsMapUeCtrlOnPusch*/

/* 
 *
 *       Fun:  ysMsMapSubChInfo
 *  
 *       Desc:  Map  Ue specific SubChannel Info
 *
 *       Ret:   void 
 *
 *       Notes: None
 *
 *       File:  ys_ms_map.c
 *
 */
#ifdef ANSI
PRIVATE void ysMsMapSubChInfo
(
YsCellCb             *cellCb,
YsUeCb               *ysUeCb,
TfuUePuschRecpReq    *puschRecpReq,
ULSUBCHINFO          *ulChanDesc
)
#else
PRIVATE void  ysMsMapSubChInfo(cellCb, ysUeCb, puschRecpReq, ulChanDesc)
YsCellCb             *cellCb;
YsUeCb               *ysUeCb;
TfuUePuschRecpReq    *puschRecpReq;
ULSUBCHINFO          *ulChanDesc;
#endif
{
   U8                     n2DmrsArr[8] = {0,6,3,4,2,8,10,9};
   U8                     idx;
   

   TRC2(ysMsMapSubChInfo)

   cmMemset((U8*)&(ulChanDesc->puschDed), 0, sizeof(PUSCHDED));
   cmMemset((U8*)&(ulChanDesc->cqiPmiRiRpt), 0, sizeof(CQIPMIRIRPT));

   ulChanDesc->mcinfo.codingDescriptor       = TURBOCDR;
   ulChanDesc->mcinfo.blockCodeConcatenation = YS_BLK_CODE_CAT;
   ulChanDesc->mcinfo.modulationType = ModulationMap[(puschRecpReq->ulSchInfo.modType/2) - 1];   
   
   ulChanDesc->mcinfo.mcsType = puschRecpReq->ulSchInfo.mcs;
  
   ulChanDesc->harqInfo.reserved = PAD;
   ulChanDesc->harqInfo.rV       = puschRecpReq->ulSchInfo.rv;
   if (ulChanDesc->harqInfo.rV)
   {
      MSPD_DBG("UL Re-transmission @ (%d,%d) UeId(%d) hqPId(%d) rv(%d)\n",
             cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
             ysUeCb->ueId,
		 	 puschRecpReq->ulSchInfo.harqProcId, 
		 	 ulChanDesc->harqInfo.rV);
   } 

   /* NDI = 1 implies transmission while 0 means re-transmission */
   ulChanDesc->harqInfo.nDi      = !puschRecpReq->ulSchInfo.isRtx;
   ulChanDesc->harqInfo.flushReq = !puschRecpReq->ulSchInfo.isRtx;

   ulChanDesc->crcInfo.crcLength         = TWTYFOUR; /*LtePhyApi.h*/
   ulChanDesc->crcInfo.crcScrambling     = YS_CRC_SCRAMBLING;

   ulChanDesc->scrInfo.scramblerType     = 0; /*LtePhyApi.h*/
   ulChanDesc->reserved = PAD;
 
   ulChanDesc->puschDed.nDMRS2 = n2DmrsArr[puschRecpReq->ulSchInfo.nDmrs];
   

/* TFU_UPGRADE_TODO:
   ulChanDesc->cqiPmiRiRpt.cqiReportModeAperiodic = 0;
   ulChanDesc->cqiPmiRiRpt.nomPDSCHRSEPREOffset = 0;
   ulChanDesc->cqiPmiRiRpt.cqiReportPeriodicEnable = 0;
   ulChanDesc->cqiPmiRiRpt.cqiPUCCHResourceIndex = 0;
   ulChanDesc->cqiPmiRiRpt.cqipmiConfigIndex = 0;
   ulChanDesc->cqiPmiRiRpt.cqiFormatIndicatorPeriodic = 0;
   ulChanDesc->cqiPmiRiRpt.K = 0;
   ulChanDesc->cqiPmiRiRpt.riConfigIndex = 0;
   ulChanDesc->cqiPmiRiRpt.simultaneousAckNackAndCQI = 0;
   ulChanDesc->cqiPmiRiRpt.padding = PAD;
   */

   ulChanDesc->mapInfo.numberofEntries = YS_SINGLE_MAPPING;
   for (idx=0; idx<ulChanDesc->mapInfo.numberofEntries; idx++)
   {
      ulChanDesc->mapInfo.reselmInfo[idx].startRes = puschRecpReq->ulSchInfo.rbStart;
      ulChanDesc->mapInfo.reselmInfo[idx].numRes   = puschRecpReq->ulSchInfo.numRb;
   }
   
} /* End of ysMsMapSubChInfo */


/* 
 *
 *       Fun:  ysMsMapChanDesc
 *  
 *       Desc:  Map  Ue specific Dedicated Channel Descriptor
 *
 *       Ret:   void 
 *
 *       Notes: None
 *
 *       File:  ys_ms_map.c
 *
 */
#ifdef ANSI
PRIVATE void ysMsMapChanDesc
(
YsCellCb             *cellCb,
YsUeCb               *ysUeCb,
TfuUePuschRecpReq    *puschRecpReq,  /*!< Reception request for PUSCH */
ULCHANDESC           *ulChanDesc
)
#else
PRIVATE void  ysMsMapChanDesc(cellCb, ysUeCb, puschRecpReq, ulChanDesc)
YsCellCb             *cellCb;
YsUeCb               *ysUeCb;
TfuUePuschRecpReq    *puschRecpReq;  /*!< Reception request for PUSCH */
ULCHANDESC           *ulChanDesc;
#endif
{
   TRC2(ysMsMapChanDesc)

   ulChanDesc->offsetNextCh    = 0;
   ulChanDesc->txpowerControl  = YS_TX_PWR_CNTRL;

   /* Warning: Hardcoding values */
   ulChanDesc->persistEnable   = 1;
   ulChanDesc->repeatCycle     = 0x01;
   ulChanDesc->channelType     = PUSCH;
   ulChanDesc->halfIterations  = 0x10;

   /* TFU_UPGRADE_TODO */
   if (!ysUeCb)
   {
      ulChanDesc->timAdvErrInfo.mSetDefaults = 1;
   }
   else
   {
      cmMemcpy((U8 *)&ulChanDesc->timAdvErrInfo, (U8 *)&ysUeCb->timingAdvErrInfo, sizeof(TIMADVERRINF));
   }

   ysMsMapSubChInfo(cellCb, ysUeCb, puschRecpReq, &ulChanDesc->ulSubChInfo);
   ysMsMapUeCtrlOnPusch(cellCb, ysUeCb, puschRecpReq, ulChanDesc);
	
   RETVOID;
} /*ysMsMapChanDesc*/


/* 
*
*       Fun:  ysMsMapPucchDedCtl
*  
*       Desc:  Map Ue Specific Pucch Control
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/

#ifdef ANSI
PRIVATE void ysMsMapPucchDedCtl
(
TfuUePucchRecpReq    *pucchRecpReq,  /*!< Reception request for PUCCH */
PUCCHDEDCTL          *pucchDedCtl
)
#else
PRIVATE void  ysMsMapPucchDedCtl(pucchRecpReq, pucchDedCtl)
TfuUePucchRecpReq    *pucchRecpReq;  /*!< Reception request for PUCCH */
PUCCHDEDCTL          *pucchDedCtl;
#endif
{
   TRC2(ysMsMapPucchDedCtl)

   pucchDedCtl->simSRHarq = 0;
   if(pucchRecpReq->uciInfo == TFU_PUCCH_HARQ)
   {
      pucchDedCtl->formatType = FORMAT1A;
      if (pucchRecpReq->hqInfo.hqSz == 2)
         pucchDedCtl->formatType = FORMAT1B;
   }
   else if(pucchRecpReq->uciInfo == TFU_PUCCH_SR)
   {
      pucchDedCtl->formatType = FORMAT1;
   }
   else if(pucchRecpReq->uciInfo == TFU_PUCCH_SRS)
   {
      pucchDedCtl->formatType = FORMAT1;
   }
   else if(pucchRecpReq->uciInfo == TFU_PUCCH_CQI)
   {
      pucchDedCtl->formatType = FORMAT2;
   }
   else if(pucchRecpReq->uciInfo == TFU_PUCCH_HARQ_SR)
   {
	   pucchDedCtl->formatType = FORMAT1A;
	   if (pucchRecpReq->hqInfo.hqSz == 2)
		  pucchDedCtl->formatType = FORMAT1B;
      pucchDedCtl->simSRHarq = 1;
   }
   else if(pucchRecpReq->uciInfo == TFU_PUCCH_HARQ_SRS)
   {
	   pucchDedCtl->formatType = FORMAT1A;
	   if (pucchRecpReq->hqInfo.hqSz == 2)
		  pucchDedCtl->formatType = FORMAT1B;
   }
   else if(pucchRecpReq->uciInfo == TFU_PUCCH_HARQ_CQI)
   {
      pucchDedCtl->formatType = FORMAT2A;
      if (pucchRecpReq->hqInfo.hqSz == 2)
         pucchDedCtl->formatType = FORMAT2B;
   }
   else if(pucchRecpReq->uciInfo == TFU_PUCCH_HARQ_SR_SRS)
   {
	   pucchDedCtl->formatType = FORMAT1A;
	   if (pucchRecpReq->hqInfo.hqSz == 2)
		  pucchDedCtl->formatType = FORMAT1B;
      pucchDedCtl->simSRHarq = 1;
   }
   else if(pucchRecpReq->uciInfo == TFU_PUCCH_HARQ_SR_CQI)
   {
	   pucchDedCtl->formatType = FORMAT2A;
	   if (pucchRecpReq->hqInfo.hqSz == 2)
		  pucchDedCtl->formatType = FORMAT2B;
       pucchDedCtl->simSRHarq = 1;
   }
   else if(pucchRecpReq->uciInfo == TFU_PUCCH_SR_SRS)
   {
      pucchDedCtl->formatType = FORMAT1;
   }
   else if(pucchRecpReq->uciInfo == TFU_PUCCH_SR_CQI)
   {
      pucchDedCtl->formatType = FORMAT2;
	  pucchDedCtl->simSRHarq = 1;
   }
   else if(pucchRecpReq->uciInfo == TFU_PUCCH_HARQ_SR_CQI_SRS)
   {
      pucchDedCtl->formatType = FORMAT2A;
      if (pucchRecpReq->hqInfo.hqSz == 2)
         pucchDedCtl->formatType = FORMAT2B;
      pucchDedCtl->simSRHarq = 1;
   }
   else if(pucchRecpReq->uciInfo == TFU_PUCCH_SR_CQI_SRS)
   {
      pucchDedCtl->formatType = FORMAT2;
	  pucchDedCtl->simSRHarq = 1;
   }

   pucchDedCtl->ackNackRepetition = 0;
   pucchDedCtl->repetitionFactor = 0;
   pucchDedCtl->msPucchSrSel = 0;
   pucchDedCtl->n1PucchANRep = 0;
   pucchDedCtl->srPUCCHResourceIndex = pucchRecpReq->srInfo.n1PucchIdx;
   pucchDedCtl->cqiPUCCHResourceIndex = pucchRecpReq->cqiInfo.n2PucchIdx;
   pucchDedCtl->dlCqiPmiSizeBits = pucchRecpReq->cqiInfo.cqiPmiSz;
   pucchDedCtl->harqPucchIndex = pucchRecpReq->hqInfo.pucchIdx;
   pucchDedCtl->harqSizebits = pucchRecpReq->hqInfo.hqSz;
   pucchDedCtl->n1PUCCHResourceIndex = 0;

   RETVOID;
} /* ysMsMapPucchDedCtl */

/*
*       Fun:  ysMsMapPucchDedRb
*  
*       Desc:  Determine PUCCH RB for the UE
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE void ysMsMapPucchDedRb
(
YsCellCb         *ysCellCb,
YsUeCb           *ysUeCb,
ULCTRLCHDESC     *ulCtlChDesc
)
#else
PRIVATE void  ysMsMapPucchDedRb(ysCellCb, ysUeCb, ulCtlChDesc)
YsCellCb         *ysCellCb;
YsUeCb           *ysUeCb;
ULCTRLCHDESC     *ulCtlChDesc;
#endif
{

   PUCCHDEDCTL    *pucchDedCtl;
   U8             c = ysCellCb->cellCfg.txCfg.cycPfx ? 3:2;
   U32            n1Pucch = 0;
   U32            n2Pucch = 0;
   Bool           isFormat1 =FALSE;
   U8             nScPerRb = 12;
   U8             n2Rb = ysCellCb->preDefVal.ulSubFrCmnCtrl.nRBCQI;
   U8             n1Cs = ysCellCb->preDefVal.ulSubFrCmnCtrl.nCSAn;
   U8             deltaShiftPucch = ysCellCb->preDefVal.ulSubFrCmnCtrl.deltaPUCCHShift;
   U8             ulBw = ysCellCb->cellInfo.ulBw;
   U8             mVal;

   TRC2(ysMsMapPucchDedRb)

   pucchDedCtl = &ulCtlChDesc->pucchDedCtl;

   switch(pucchDedCtl->formatType)
   {
      case FORMAT1: 
      {
         n1Pucch = pucchDedCtl->srPUCCHResourceIndex;
		 pucchDedCtl->harqPucchIndex = n1Pucch;
         isFormat1 = TRUE;  
         break;
      }
      case FORMAT1A:
      case FORMAT1B:
      {
         n1Pucch = pucchDedCtl->harqPucchIndex;
         isFormat1 = TRUE;
         break;
      }
      case FORMAT2:
      case FORMAT2A:
      case FORMAT2B:
      { 
         n2Pucch = pucchDedCtl->cqiPUCCHResourceIndex;
         break;   
      }   
   }

   if (isFormat1)
   {
      U8 shift = (c * n1Cs/deltaShiftPucch);
	  mVal = n1Pucch < shift ? n2Rb : ((n1Pucch - shift)/(c * nScPerRb/deltaShiftPucch) + n2Rb + (n1Cs + 7)/8);
	  ulCtlChDesc->pucchDedCtl.n1PUCCHResourceIndex = n1Pucch;
   }
   else
   {
       mVal = n2Pucch/nScPerRb;
   }

   if (mVal % 2 == 0)
   {
      ulCtlChDesc->startRes = mVal/2;
   }
   else
   {
      ulCtlChDesc->startRes = ulBw - 1 - (mVal/2);
   }
   
   RETVOID;
} /* ysMsMapPucchDedRb*/
/* 
*
*       Fun:  ysMsMapCtlChanDesc
*  
*       Desc:  Map Control Channel Descriptor
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PRIVATE void ysMsMapCtlChanDesc
(
YsCellCb             *ysCellCb,
YsUeCb               *ueCb,
TfuUePucchRecpReq    *pucchRecpReq,  /*!< Reception request for PUCCH */
ULCTRLCHDESC         *ulCtlChDesc
)
#else
PRIVATE void ysMsMapCtlChanDesc(ysCellCb, ueCb, pucchRecpReq, ulCtlChDesc)
YsCellCb             *ysCellCb;
YsUeCb               *ueCb;
TfuUePucchRecpReq    *pucchRecpReq;  /*!< Reception request for PUCCH */
ULCTRLCHDESC         *ulCtlChDesc;
#endif
{
   TRC2(ysMsMapCtlChanDesc)

   ulCtlChDesc->txpowerControl         = YS_TX_PWR_CNTRL;
   ulCtlChDesc->crcLength              = YS_PUCCH_CRC_LEN;
   ulCtlChDesc->channelType            = PUCCH;
   ulCtlChDesc->reserved               = PAD;
   /* 36.211 Section 5.4 */

   ulCtlChDesc->scrmblerInitValue      = ueCb->ueId;
   ulCtlChDesc->codingDescriptor       = YS_PUCCH_COD_DES;
   ulCtlChDesc->blockCodeConcatenation = YS_PUCCH_BLK_CAT;
   ulCtlChDesc->mcsType                = YS_PUCCH_MCS_TYP;
   ysMsMapPucchDedCtl(pucchRecpReq, &ulCtlChDesc->pucchDedCtl);
   ulCtlChDesc->modulationType = ysUeModulation[ulCtlChDesc->pucchDedCtl.formatType];

   /* The entire structure ulCtlChDesc->cqiPmiRiRpt is unused, so don't
    * care about it */

   ulCtlChDesc->numberofEntries = YS_SINGLE_MAPPING;

  ysMsMapPucchDedRb(ysCellCb, ueCb, ulCtlChDesc);
   ulCtlChDesc->numRes    = 0;

   
   cmMemcpy((U8 *)&ulCtlChDesc->timAdvErrInfo, (U8 *)&ueCb->timingAdvErrInfo, sizeof(TIMADVERRINF));


   
   RETVOID;
} /*ysMsMapCtlChanDesc*/


/* 
*
*       Fun:   ysMsMapRxVector
*  
*       Desc:  Create Rx Vector
*
*       Ret:   void 
*
*       Notes: None
*
*       File:  ys_ms_map.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsUtlMapRxVector
(
YsCellCb         *cellCb,
TfuRecpReqInfo   *recpReq,
PULSUBFRDESC     ulSubFrDesc,
CmLteTimingInfo  timingInfo
)
#else
PUBLIC S16 ysMsUtlMapRxVector(cellCb, recpReq, ulSubFrDesc, timingInfo)
YsCellCb       *cellCb;
TfuRecpReqInfo *recpReq;
PULSUBFRDESC   ulSubFrDesc;
CmLteTimingInfo timingInfo;
#endif
{
   YsUeCb               *ueCb = NULLP;
   U8                   rxSduSf;
   U32                  numPuschCh = 0;
   U32                  numPucchCh = 0;
   U32                  numCh = 0;
   U32                  numSrs = 0;
   CmLList              *cmLstEnt;
   TfuUeRecpReqInfo     *recpReqInfo;
   CtfCellCfgInfo       *cellCfg;
   
   TRC2(yysMsUtlMapRxVector)

   cellCfg                     = &cellCb->cellCfg;
   cmLstEnt = recpReq->ueRecpReqLst.first;

   YS_DBG_INFO((_ysp, "ysMsUtlMapRxVector(): (%d) (%d);\n", 
            cellCb->timingInfo.sfn, cellCb->timingInfo.subframe));

   /* Filling ULSUBFRDESC */
   ulSubFrDesc->frameNumber    = cellCb->timingInfo.sfn;
   ulSubFrDesc->subframeNumber = cellCb->timingInfo.subframe;
   ulSubFrDesc->subframeType   = ULRX;
   ulSubFrDesc->antennaPortcount = YS_MS_GET_NUM_TX_ANT(cellCfg->antennaCfg); 

   /* Getting values from stored predefined configuration */
   cmMemcpy((U8*) &ulSubFrDesc->ulSfrCtrl,
            (U8*) &cellCb->preDefVal.ulSubFrCmnCtrl,
            sizeof(ULSUBFRCMNCTRL));
   
   rxSduSf = (timingInfo.subframe + 2) % YS_NUM_SUB_FRAMES;
   while (cmLstEnt != NULLP)
   {
      recpReqInfo = (TfuUeRecpReqInfo*)cmLstEnt->node;
      ueCb = ysMsCfgGetUe(cellCb, recpReqInfo->rnti);
      if(ueCb == NULLP)
      {
	      MSPD_DBG("UeCb not found for rnti(%u)\n", recpReqInfo->rnti);
      }

      /* PUSCH */
      if (recpReqInfo->type == TFU_RECP_REQ_PUSCH)
      {
         /* HARQ_UL: If PUSCH reception request, mark CRC information */
         ysMsMapChanDesc(cellCb, ueCb, &recpReqInfo->t.puschRecpReq, &ulSubFrDesc->ulCh[numPuschCh]);
         ulSubFrDesc->ulCtlCh[numPuschCh].offsetNextCh = 
            (U32)&(ulSubFrDesc->ulCh[numPuschCh])-(U32)ulSubFrDesc;
		  cellCb->isCrcExptd[rxSduSf] = TRUE;
		 
	     ulSubFrDesc->ulCh[numPuschCh].ulSubChInfo.scrInfo.scrinitValueinput = recpReqInfo->rnti;
	     ysMsMapRntiToChan(cellCb, recpReqInfo->rnti, timingInfo.subframe, numCh);
         ulSubFrDesc->ulCh[numPuschCh].channelId = numCh;
         numPuschCh++;
         numCh++;
		 
      } /* schReqInfo */
      /* PUCCH */
      else
      {
	     if (recpReqInfo->t.pucchRecpReq.uciInfo != TFU_PUCCH_SRS)
	     {
            ysMsMapCtlChanDesc(cellCb, ueCb, &recpReqInfo->t.pucchRecpReq, &ulSubFrDesc->ulCtlCh[numPucchCh]);
            ulSubFrDesc->ulCtlCh[numPucchCh].offsetNextCh = 
              (U32)&ulSubFrDesc->ulCtlCh[numPucchCh]-(U32)ulSubFrDesc;
            ulSubFrDesc->ulCtlCh[numPucchCh].channelId = numCh;
 		    ysMsMapRntiToChan(cellCb, recpReqInfo->rnti, timingInfo.subframe, numCh);
            numPucchCh++;
            numCh++;
	     }
      } /* CtlChanInfo */

      cmLstEnt = cmLstEnt->next;
   } /* cmLstEnt while */

   ulSubFrDesc->ulCtlCh[numPucchCh - 1].offsetNextCh = 0x04;
   ulSubFrDesc->ulCh[numPuschCh - 1].offsetNextCh = 0x04;

   ulSubFrDesc->numberofChannelDescriptors =  numCh;
   ulSubFrDesc->numberOfCtrlChannelDescriptors  = numPucchCh;
   ulSubFrDesc->numberSrsinSf                   = numSrs;

   ulSubFrDesc->offsetRachCtrlStruct            = (U32)&ulSubFrDesc->rachCtrl - (U32)ulSubFrDesc;
   ulSubFrDesc->offsetULCtrlChannels            = (U32)&ulSubFrDesc->ulCtlCh - (U32)ulSubFrDesc;
   ulSubFrDesc->offsetsrsInfo                   = (U32)&ulSubFrDesc->srsInfo - (U32)ulSubFrDesc;
   ulSubFrDesc->offsetCustomFeatures            = 0;

   RETVALUE(ROK);
} /* ysMsUtlMapRxVector */


#endif /* TFU_UPGRADE*/
/********************************************************************30**
  
         End of file:     ys_ms_map.c
  
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
----------- -------- ---- -----------------------------------------------
/main/1      ---      mraj   1. initial release.
/main/1    ys004.102  vr   1. Merged MSPD code with phy 1.7
*********************************************************************91*/

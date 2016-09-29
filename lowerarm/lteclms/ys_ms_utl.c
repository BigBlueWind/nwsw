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

/************************************************************************

     Name:     LTE MAC Convergence Layer

     Type:     C source file

     Desc:     C source code for Entry point fucntions

     File:     ys_ms_utl.c

     Sid:

     Prg:      rp

**********************************************************************/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_lte.h"
#include "ctf.h"           /* CTF defines */
#include "lys.h"           /* layer management defines for LTE-CL */
#include "tfu.h"
#include "ys_ms.h"         /* defines and macros for CL */
#include "ys_ms_err.h"        /* YS error defines */

#ifdef TENB_AS_SECURITY
#include "spaccdrv.h" /*sumanth*/
#endif

/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"
#include "ctf.x"           /* CTF types */
#include "lys.x"           /* layer management typedefs for CL */
#include "tfu.x"

/* Silicon Includes */
#include "LtePhyL2Api.h"
#include "apidefs.h"
#include "appinit.h"

#include "ys_ms.x"            /* typedefs for CL */

/*ys004.102 :  Merged MSPD code with phy 1.7 */

U32 numCo;

/* header/extern include files (.x) */
/* header include files (.h) */

/* Static variables */
PRIVATE enum ModulationOptions ModulationType[32] =
{
QPSK, QPSK, QPSK, QPSK, QPSK, QPSK, QPSK, QPSK,
QPSK, QPSK, QAM16, QAM16, QAM16, QAM16, QAM16, QAM16,
QAM16, QAM64, QAM64, QAM64, QAM64, QAM64, QAM64, QAM64,
QAM64, QAM64, QAM64, QAM64, QAM64, QPSK, QAM16, QAM64
};

/* Bit masks used for set/get bit range macros */
U8 YsMsBitMaskArr1[8] =
{
   0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01
};
U8 YsMsBitMaskArr2[8] =
{
   0x80, 0xb0, 0xe0, 0xf0, 0xf8, 0xfb, 0xfe, 0xff
};

U8 pmiNumOfLayerMap2AntPortDCI2[2][8] = {{2,1,1,1,1,1,1,0},{2,2,2,0,0,0,0,0}} ;

U8 pmiNumOfLayerMap4AntPortDCI2[2][64] = {
					{4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,\
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
                                        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,\
				        4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0}
 				      } ;

U8 pmiNumOfLayerMap4AntPortDCI2A[2][4] = {{4,2,0,0},{2,3,4,0}} ;

#ifdef ANSI
PUBLIC Void ysMsSetBitRange
(
 U8      *byteArr,
 U8      bitIndex,
 U8      len,
 U32     val
)
#else
PUBLIC Void ysMsSetBitRange()
#endif
{
   U8    octIndx;
   U8    bitStartOffset;

   octIndx = bitIndex / 8;
   bitStartOffset = bitIndex % 8;

   if ((bitStartOffset + len) > 8)
   {
      U8 tmpVal;
      /* the field straddles the byte boundary */

      tmpVal = val >> (len - (8 - bitStartOffset));
      byteArr[octIndx] |= tmpVal;

      tmpVal = val << (8 -(len -(8 - bitStartOffset)));
      byteArr[octIndx + 1] |= tmpVal;

   }
   else
   {
      U8 tmpVal = val << (8 - bitStartOffset - len);
      byteArr[octIndx] |= tmpVal;
   }
}

/*
*
*       Fun:   ysMsUtlFillCfgReq
*
*       Desc:
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsUtlFillCfgReq
(
INITPARM      *msCfgReq,
YsCellCb      *cellCb
)
#else
PUBLIC S16 ysMsUtlFillCfgReq(msCfgReq, cellCb)
INITPARM      *msCfgReq;
YsCellCb      *cellCb;
#endif
{
   CtfCellCfgInfo *cellCfg;

   TRC2(ysMsUtlFillCfgReq);

   cellCfg = &cellCb->cellCfg;

   /*TODO:Setting this for the time being as per Vitaliy input.Might be 
     reverted at later stage */
   msCfgReq->channelBandwidth = YS_MS_GET_CH_BW(cellCfg->bwCfg);

   msCfgReq->frtypeDuplexmode = YS_MS_GET_DUP_MODE(cellCfg->txCfg);

   msCfgReq->radioAccessMode = YS_MS_ACCESS_MODE_OFDMA;

   msCfgReq->physicalResourceBandwidth = YS_MS_GET_RSRC_BW(cellCfg->txCfg);

   msCfgReq->numberResourceBlocksperSlot = YS_MS_GET_DL_BW(cellCfg->bwCfg);

   msCfgReq->phyLayerCellIdGroup = cellCfg->cellIdGrpId;
   msCfgReq->phyLayerId = cellCfg->physCellId;

   msCfgReq->txAntennaPortCount = YS_MS_GET_NUM_TX_ANT(cellCfg->antennaCfg);

   msCfgReq->fastfwdorPreclockingNumber = YS_MS_FAST_FW_PRE_LOCK_NUM;

   msCfgReq->fftSize = YS_MS_GET_FFT_SIZE(cellCfg->bwCfg);

   msCfgReq->numberUsedSubcarriers = YS_MS_GET_DL_BW(cellCfg->bwCfg) * \
                                     YS_MS_GET_RSRC_BW(cellCfg->txCfg);

   msCfgReq->nMaxDlRb = YS_MS_GET_DL_BW(cellCfg->bwCfg);
   msCfgReq->nMaxUlRb = YS_MS_GET_UL_BW(cellCfg->bwCfg);


   msCfgReq->referenceSignalPower = cellCfg->pdschCfg.refSigPwr;
   msCfgReq->primarySyncSignalPower= cellCfg->priSigPwr;
   msCfgReq->secondarySyncSignalPower = cellCfg->secSigPwr;
   MSPD_DBG("msCfgReq->referenceSignalPower = %d, \
         msCfgReq->primarySyncSignalPower=%d, \
         msCfgReq->secondarySyncSignalPower=%d",
         msCfgReq->referenceSignalPower, msCfgReq->primarySyncSignalPower,
         msCfgReq->secondarySyncSignalPower);
   msCfgReq->numDataRePerPRB = YS_MS_GET_RE_PER_PRB(cellCfg->antennaCfg);
   msCfgReq->cyclicPrefixType = YS_MS_GET_CP_TYPE(cellCfg->txCfg);

   msCfgReq->rxAntennaPortCount = YS_MS_GET_NUM_TX_ANT(cellCfg->antennaCfg);

   msCfgReq->phyCfg =
      (PHYINIT_NEW_MAC_TO_PHY_INTERFACE | PHYINIT_PHY_MAINTAINS_PBCH | 
       PHYINIT_ROBUST_CONTROL | PHYINIT_DONT_FREE_NEXT_MISSED_VECTOR 
#ifdef LOWLATENCY
       | PHYINIT_LOW_LATENCY_PATH 
#endif
#ifdef DLHQ_RTT_OPT
       | PHYINIT_TWO_MUXCONTROL_LISTS
#endif
      );

   msCfgReq->reserved = 0;

   msCfgReq->pb = cellCfg->pdschCfg.p_b;

#ifndef YS_MS_NO_TA
   msCfgReq->rxSduLineMonitorInfoenable = 1;
#endif

   msCfgReq->customExtensionReportEn = 0;
   /*TODO:setting this to INDV for the time being as per Vitaliy input, 
     might b changed at a later stage.*/
   msCfgReq->rachReportMode = YS_RACH_REPORT_MODE_INDV;
   msCfgReq->txSduConfEnable = YS_TX_SDU_CFM_DIS;
   /*Setting these parameters so that CL need not wait for PHY
     to send TX_START_CFM before CL can start transmission of
     TX SUDs.*/
   msCfgReq->txStartConfDisable = 1;
   msCfgReq->rxStartConfDisable = 1;

   msCfgReq->sduConfig = YS_SDU_CFG_NO_SHARED_MEM;

   msCfgReq->radioFrameNumber = YS_PHY_INIT_SFN;
   msCfgReq->subframeNumber = YS_PHY_INIT_SF;
   msCfgReq->slotNumber = YS_PHY_INIT_SLOT;

   if(cellCfg->srsUlCfg.pres == TRUE)
   {

      /* changed according to MS values */
      msCfgReq->srsBandwidthConfig = cellCfg->srsUlCfg.srsSetup.srsBw;
      msCfgReq->srsSubframeConfig = cellCfg->srsUlCfg.srsSetup.sfCfg;
      msCfgReq->srsSimultaneousANandSRS = cellCfg->srsUlCfg.srsSetup.srsANSimultTx;
      MSPD_DBG("msCfgReq->srsBandwidthConfig=%d, \
      msCfgReq->srsSubframeConfig = %d,msCfgReq->srsSimultaneousANandSRS=%d",
      msCfgReq->srsBandwidthConfig,msCfgReq->srsSubframeConfig,
      msCfgReq->srsSimultaneousANandSRS);
   }
   /*TODO:Setting this to 15 irrespective of srsUlCfg.pres flag, this can be 
     removed later */
      msCfgReq->srsSubframeConfig = 15;

   if(cellCfg->prachCfg.pres == TRUE)
   {
      msCfgReq->prachConfigurationIndex = cellCfg->prachCfg.prachCfgIndex;
      msCfgReq->prachFrequencyOffset = cellCfg->prachCfg.prachFreqOffset;
      msCfgReq->prachHighSpeedFlag = cellCfg->prachCfg.highSpeedFlag;
      msCfgReq->prachCyclicShiftConfig = cellCfg->prachCfg.zeroCorrelationZoneCfg;
      msCfgReq->prachRootSequenceIndex = cellCfg->prachCfg.rootSequenceIndex;
      MSPD_DBG("msCfgReq->prachConfigurationIndex = %d,\
            msCfgReq->prachFrequencyOffset=%d,msCfgReq->prachHighSpeedFlag=%d,\
            msCfgReq->prachCyclicShiftConfig=%d, \
            msCfgReq->prachRootSequenceIndex=%d\n", 
            msCfgReq->prachConfigurationIndex, msCfgReq->prachFrequencyOffset,
            msCfgReq->prachHighSpeedFlag,msCfgReq->prachCyclicShiftConfig, 
            msCfgReq->prachRootSequenceIndex);
      msCfgReq->phichResource = cellCfg->phichCfg.resource;
      msCfgReq->phichDuration = cellCfg->phichCfg.duration;
   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   ysMsUtlFillTxVector
*
*       Desc:
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsUtlFillTxVector
(
TfuCntrlReqInfo *tfuCntrlReq,
YsCellCb        *cellCb,
PDLSUBFRDESC    pTxVect
)
#else
PUBLIC S16 ysMsUtlFillTxVector(tfuCntrlReq, cellCb, pTxVect)
TfuCntrlReqInfo *tfuCntrlReq;
YsCellCb        *cellCb;
PDLSUBFRDESC    pTxVect;
#endif
{
   CmLList      *cmLstEnt;
   TfuPdcchInfo *pdcchInfo;
   U8           chIdx;
   U8           idx;
   U8           numDlCh;
   DCICHANNELDESC *dciCh;
   DLCMNTXPWRCTL  *dlCmnPwrCtl;


   TRC2(ysMsUtlFillTxVector)

   pTxVect->subframeNumber   = tfuCntrlReq->dlTiming.subframe;
   pTxVect->frameNumber      = tfuCntrlReq->dlTiming.sfn;

   pTxVect->subframeType     = DLTX;
   pTxVect->antennaPortcount = YS_MS_GET_NUM_TX_ANT(cellCb->cellCfg.antennaCfg);

   /* Fill DlSubframeCommonControlStructure */
   pTxVect->numCtrlSymbols        = tfuCntrlReq->cfi;
   pTxVect->phichResource         = cellCb->cellCfg.phichCfg.resource;
   pTxVect->phichDuration         = cellCb->cellCfg.phichCfg.duration;
   YS_DBG_INFO((_ysp, "celCfg PHICH values (%d) (%d)", pTxVect->phichResource,
   pTxVect->phichDuration));

   /* HARQ: DCI0_PHICH: No DCI0 and PHICH channels in txVector */
   pTxVect->numberOfPhichChannels = 0;
   pTxVect->numCtrlChannels       = tfuCntrlReq->dlPdcchLst.count;

   numDlCh = chIdx = pTxVect->numberofChannelDescriptors;

   /* Leave space for four PDSCH and start filling DCIs thereafer,
    * later probably fill DCIs after filling PDSCHs so the
    * change becomes more generic and reliable */
   dciCh = (DCICHANNELDESC *)&pTxVect->dlCh[numDlCh+7];
   pTxVect->offsetDCIChannels     = (U32)dciCh - (U32)pTxVect;

   /* Fill the DCI Chan Desc structure with dlPdcchLst, DCI FORMAT 1A */
   for (idx = 0, cmLstEnt = tfuCntrlReq->dlPdcchLst.first;
        chIdx < MAXCHSUBFRAME && cmLstEnt;
        cmLstEnt = cmLstEnt->next, idx++, chIdx++)
   {
      pdcchInfo = (TfuPdcchInfo*)(cmLstEnt->node);

      if (ysMsUtlFillDCIChanDesc(pdcchInfo, cellCb,
         &dciCh[idx], chIdx, tfuCntrlReq->dlTiming) == RFAILED)
         {
            /* HARQ_TODO: Need to see what can be done here */
            RETVALUE(RFAILED);
         }
      pTxVect->dciCh[idx].offsetNextCh = 0;
   }

   /* As is the case for DCIs (starting right after dlChs), power
    * control info may also start right after DCIs */
   dlCmnPwrCtl = (DLCMNTXPWRCTL* )&dciCh[idx];
   pTxVect->offsetPowerCtrl       = (U32)dlCmnPwrCtl - (U32)pTxVect;


   if (idx)
   {
      pTxVect->dciCh[idx - 1].offsetNextCh = 0;
   }


   idx = pTxVect->numberofChannelDescriptors;

   if (idx)
   {
      pTxVect->dlCh[idx - 1].offsetNextCh = 0;
   }
   /*Fix for ccpu00123185 */
   /* dlCmnPwrCtl->prsRa          = cellCb->cellCfg.pdschCfg.refSigPwr; */
   dlCmnPwrCtl->pilotPower     = cellCb->cellCfg.pilotSigPwr;
   dlCmnPwrCtl->psyncPower     = cellCb->cellCfg.priSigPwr;
   dlCmnPwrCtl->ssyncPower     = cellCb->cellCfg.secSigPwr;

   /*settting it to 0 for temp testing (IQ sample)
     TODO:Find out if this parameter value can be derived
     from MAC.*/
   dlCmnPwrCtl->ciPower        = 0;
   //dlCmnPwrCtl->paprControl    = 0;
   //dlCmnPwrCtl->paprThreshold  = 0;

   pTxVect->numberofChannelDescriptors = chIdx;
   cellCb->dlEncL1Msgs[tfuCntrlReq->dlTiming.subframe].numChannels= chIdx;
   pTxVect->offsetCustomFeatures       = 0; /* This entry is absent */

   RETVALUE(ROK);
} /* end of ysMsUtlFillTxVector */


/* HARQ: DCI0_PHICH */
/*
*
*       Fun:   ysMsUtlFillAndAddDci0HiInfo
*
*       Desc:
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsUtlFillAndAddDci0HiInfo
(
TfuCntrlReqInfo  *tfuCntrlReq,
YsCellCb         *cellCb,
PHIADCIULMSGDESC pDci0HiVector
)
#else
PUBLIC S16 ysMsUtlFillAndAddDci0HiInfo(tfuCntrlReq, cellCb, pDci0HiVector)
TfuCntrlReqInfo *tfuCntrlReq;
YsCellCb        *cellCb;
PHIADCIULMSGDESC pDci0HiVector;
#endif
{
   CmLList      *cmLstEnt;
   TfuPhichInfo *phichInfo;
   YsPhyTxSdu   phyTxSdu;
   U32          channelIdx;
   TfuPdcchInfo *pdcchInfo;
   /* CmLListCp    *dlMsgLst;*/
   PMAC2PHY_QUEUE_EL pElem;
   YsPhyLstCp        *pUlCntrl;
   U8                sfNum;

   TRC2(ysMsUtlFillAndAddDci0HiInfo)

   pDci0HiVector->msgLength = sizeof(HIADCIULMSGDESC);
   pDci0HiVector->msgType     = PHY_TXHIADCIUL_REQ;
   pDci0HiVector->phyEntityId = YS_PHY_INST_ID;
   pDci0HiVector->subFrameNumber = tfuCntrlReq->ulTiming.subframe;
   pDci0HiVector->frameNumber = tfuCntrlReq->ulTiming.sfn;
   sfNum = pDci0HiVector->subFrameNumber;

   pDci0HiVector->numHiSdus             = tfuCntrlReq->phichLst.count;
   pDci0HiVector->numDciUlSdus          = tfuCntrlReq->ulPdcchLst.count;
   pDci0HiVector->reserved              = 0;
   YS_DBG_INFO((_ysp, "Num of PHICHs: %d and Num of DCI0: %d values (%d) (%d) ",
   pDci0HiVector->numHiSdus, pDci0HiVector->numDciUlSdus));


   pUlCntrl = &cellCb->dlEncL1Msgs[sfNum].ulCntrl;
   channelIdx = cellCb->dlEncL1Msgs[sfNum].numChannels;

   /* Add HiDci0 vector to the linked list of PHY messages */
   pElem = ysMsUtlGetPhyListElem();
   if (!pElem)
   {
     /* HARQ_DBG */
      MSPD_DBG("ysMsUtlAllocSvsrMsg failed for PHY list element for HiDci0 Vector\
     at time (%d,%d)\n",
     cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
	  RETVALUE(RFAILED);
   }
   YS_MS_FILL_PHY_LIST_ELEM(pElem, pDci0HiVector->frameNumber, 
   pDci0HiVector->subFrameNumber,
			 pDci0HiVector, sizeof(HIADCIULMSGDESC), PHY_TXHIADCIUL_REQ);
   ysMsUtlAddToPhyList(pUlCntrl,pElem);


	/* Fill the structure with PHICH list, PHICHs */
   for (cmLstEnt = tfuCntrlReq->phichLst.first;cmLstEnt;
             cmLstEnt = cmLstEnt->next, channelIdx++)
   {
      phichInfo = (TfuPhichInfo*)(cmLstEnt->node);
      YS_DBG_FATAL((_ysp, "\n ysMsUtlFillAndAddDci0HiInfo filling PHICH at : \
      (%d,%d) \n", cellCb->timingInfo.sfn, cellCb->timingInfo.subframe));

      if (ysMsUtlFillDci0HiSdu(&phyTxSdu, phichInfo, NULLP, PHICH,
      			channelIdx, cellCb) != ROK)
      {
      	/* YS_MS_FREE(phyTxSdu, sizeof(YsPhyTxSdu)); */
      	RETVALUE(RFAILED);
      }

      /* Add Hi SDUs to the linked list of PHY messages */

      pElem = ysMsUtlGetPhyListElem();
      if (!pElem)
      {
         /* HARQ_DBG :TODO: need to free the allocated messages */
         YS_DBG_ERR((_ysp,"ysMsUtlAllocSvsrMsg failed for PHY list element \
                  for HI SDU at time(%d,%d)\n",
                  cellCb->timingInfo.sfn, cellCb->timingInfo.subframe));
         RETVALUE(RFAILED);
      }
      YS_MS_FILL_PHY_LIST_ELEM(pElem, pDci0HiVector->frameNumber, 
            pDci0HiVector->subFrameNumber,
      		  phyTxSdu.txSduReq, sizeof(HIINFOMSGDESC), PHY_TXHISDU_REQ);
      ysMsUtlAddToPhyList(pUlCntrl,pElem);
      phyTxSdu.txSduReq = NULLP;
      phyTxSdu.sduLen = 0;

   }


   /* Fill the structure with ulPdcchLst, DCI Format 0 */
   for (cmLstEnt = tfuCntrlReq->ulPdcchLst.first; cmLstEnt;
   cmLstEnt = cmLstEnt->next, channelIdx++)
   {
      pdcchInfo = (TfuPdcchInfo*)(cmLstEnt->node);

      YS_DBG_FATAL((_ysp, "\n ysMsUtlFillAndAddDci0HiInfo filling PHICH at : \
      (%d,%d) \n", cellCb->timingInfo.sfn, cellCb->timingInfo.subframe));

      if (ysMsUtlFillDci0HiSdu(&phyTxSdu, NULLP, pdcchInfo, PDCCH,
      			channelIdx, cellCb) != ROK)
      {
         /* YS_MS_FREE(phyTxSdu, sizeof(YsPhyTxSdu)); */
         RETVALUE(RFAILED);
      }
      /* Add DCI0 SDUs to the linked list of PHY messages */
      pElem = ysMsUtlGetPhyListElem();
      if (!pElem)
      {
         /* HARQ_DBG :TODO: need to free the allocated messages */
         YS_DBG_ERR((_ysp,"ysMsUtlAllocSvsrMsg failed for PHY list element\
               for DCI0 SDU\n", cellCb->timingInfo.sfn, 
               cellCb->timingInfo.subframe));
         RETVALUE(RFAILED);
      }
      YS_MS_FILL_PHY_LIST_ELEM(pElem, pDci0HiVector->frameNumber, 
            pDci0HiVector->subFrameNumber,
      		  phyTxSdu.txSduReq, sizeof(DCIULSDUMSG), PHY_TXDCIULSDU_REQ);
      ysMsUtlAddToPhyList(pUlCntrl,pElem);

      phyTxSdu.txSduReq = NULLP;
      phyTxSdu.sduLen = 0;
   }

   cellCb->dlEncL1Msgs[sfNum].numChannels = channelIdx;
   RETVALUE(ROK);
} /* end of ysMsUtlFillAndAddDci0HiInfo */

/*
*
*       Fun:   ysMsUtlFillMappingInfo
*
*       Desc:
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PUBLIC Void ysMsUtlFillMappingInfo
(
YsCellCb        *cellCb,
U32             riv,
U8              *startRb,
U8              *numRb
)
#else
PUBLIC Void ysMsUtlFillMappingInfo (cellCb, riv, startRb, numRb)
YsCellCb        *cellCb;
U32             riv;
U8              *startRb;
U8              *numRb;
#endif
{
   U8 numRb1, startRb1;
   U8 bW = cellCb->cellInfo.dlBw;

   /*-- Logic for obtaining startRes, numRes can be found in R1-072119 --*/
   numRb1 = (riv / bW) + 1;
   startRb1 = riv % bW;

   if ((numRb1 + startRb1) > bW)
   {
      *numRb = bW + 2 - numRb1;
      *startRb = bW -1 - startRb1;
   }
   else
   {
      *numRb = numRb1;
      *startRb = startRb1;
   }

   RETVOID;
}

/*
*
*       Fun:   ysMsUtlFillMappingInfoForRaType0
*
*       Desc:
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PRIVATE Void ysMsUtlFillMappingInfoForRaType0
(
YsCellCb        *cellCb,
U8              *resAllocMap,
MAPPINGINFO     *phymap
)
#else
PRIVATE Void ysMsUtlFillMappingInfoForRaType0(cellCb, resAllocMap, phymap)
YsCellCb        *cellCb;
U8              *resAllocMap;
MAPPINGINFO     *phymap;
#endif
{
#define IS_BIT_SET(_a, _i)  ((_a) & (1<<(_i)))
   U32  i, j;
   U32  n;
   U8   rbgSize = cellCb->cellInfo.rbgSize;
   U8   dlBw    = cellCb->cellInfo.dlBw;

   n = 0;
   for (i = 0; i < TFU_MAX_ALLOC_BYTES; ++i)
   {
      U8  base = i * 8;
      if (!resAllocMap[i])
         continue;
      for (j = 0; j < 8; ++j)
      {
         if (IS_BIT_SET(resAllocMap[i], 7-j))
         {
            phymap->reselmInfo[n].startRes = (base + j) * rbgSize;
            phymap->reselmInfo[n].numRes   = rbgSize;
            ++n;
         }
      }
   }
   if (phymap->reselmInfo[n-1].startRes + phymap->reselmInfo[n-1].numRes > dlBw)
      phymap->reselmInfo[n-1].numRes =  dlBw - phymap->reselmInfo[n-1].startRes;
   phymap->numberofEntries = n;
   if (phymap->numberofEntries == 0)
   	MSPD_DBG("<TX SDU size 0>Invalid Tx vector being sent\n");
}

/*
*
*       Fun:   ysMsUtlFillDlChanDesc
*
*       Desc:
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsUtlFillDlChanDesc
(
TfuDatReqPduInfo *tfuDatReqPdu,
TfuPhichInfo    *phichInfo,
YsCellCb        *cellCb,
PDLCHANDESC     dlCh,
U8              channelType,
U16             channelId,
CmLteTimingInfo timingInfo
)
#else
PUBLIC S16 ysMsUtlFillDlChanDesc (tfuDatReqPdu, phichInfo, cellCb, dlCh, 
      channelType,channelId, timingInfo)
TfuDatReqPduInfo *tfuDatReqPdu;
TfuPhichInfo    *phichInfo;
YsCellCb        *cellCb;
PDLCHANDESC     dlCh;
U8              channelType;
U16             channelId;
CmLteTimingInfo timingInfo;
#endif
{
   /*-- Only one sub-channel info is filled as numCodeWords is always one --*/
   PDLSUBCHINFO subChInfo = &dlCh->subChInfo[0];
   YsUeCb     *ueCb;
   PDLSUBCHINFO subChInfoCw2 = &dlCh->subChInfo[1];
   U8 cw1index = 0;
   U8 cw2index = 0;

   /* temp changes to see if CQI-14 CRC error goes away */
   cmMemset((U8 *)&dlCh->subChInfo[1], 0, sizeof(DLSUBCHINFO));
  /* dlMsgLst = &(cellCb->dlEncL1Msgs[(cellCb->timingInfo.subframe + TFU_DELTA)
         % YS_NUM_SUB_FRAMES].txSduLst); */
   dlCh->txpowerControl = 0; /*-768*/
   /* Changed these values on inputs from MSPD */
   dlCh->persistEnable = TRUE;
   dlCh->repeatCycle = 1;
   dlCh->channelType = channelType;
   dlCh->numCodeWords = 1;
   dlCh->nLayers = cellCb->cellInfo.numAntPorts;
   dlCh->transmissionMode = cellCb->cellInfo.cmnTransMode;
   /* Adding channel Id */
   dlCh->channelId = channelId;
   /* initialize the num entries for mapping */
   dlCh->mapInfo.numberofEntries = 0;
   dlCh->dlPrecoderInfo.cddType = NOCDD;
   dlCh->dlPrecoderInfo.codeBookIdx = 0;
   cmMemset(dlCh->dlPrecoderInfo.codebookSubsetRestriction, 0, 8);

   switch(channelType)
	{
		case PDSCH:
			ueCb = ysMsCfgGetUe(cellCb, tfuDatReqPdu->rnti);
			dlCh->ueCategory =  ueCb->ueCfg.ueCatCfg.ueCategory;
			dlCh->crcInfo.crcLength = TWTYFOUR; /*-- 36.212 - 5.3.2.1 --*/
			dlCh->crcInfo.crcScrambling = 1;
			dlCh->scrInfo.scrinitValueinput = tfuDatReqPdu->rnti;
			subChInfo->codingDescriptor = TURBOCDR;
			subChInfo->blockCodeConcatenation = 1;

			/*-- based on the DCI format we fill sub channel info --*/
			switch(tfuDatReqPdu->dciInfo.format)
			{
				case TFU_DCI_FORMAT_1A:
					{
                  TfuDciFormat1aAllocInfo *format1a = 
                     &tfuDatReqPdu->dciInfo.u.format1aAllocInfo;
						dlCh->hCid = format1a->harqProcId.val;
						/* MS_FIX: syed If it is a BCCH/PCCH/RAR transmission
						 * then set modulation Type to QPSK regardless of iMcs */
						if ((tfuDatReqPdu->rnti == YS_SI_RNTI) || 
						    (tfuDatReqPdu->rnti == YS_P_RNTI) || 
						    (tfuDatReqPdu->rnti <= YS_MAX_RARNTI))
						{
						   subChInfo->modulationType = QPSK;
						}
						else
						{
						   subChInfo->modulationType = ModulationType[format1a->mcs];
						}
						subChInfo->mcsType = format1a->mcs;
						subChInfo->nDi = format1a->ndi;
						subChInfo->rV = format1a->rv;
						subChInfo->flushReq = 0;
						dlCh->mapInfo.numberofEntries = 1;
						ysMsUtlFillMappingInfo(cellCb, format1a->alloc.u.riv,
								&dlCh->mapInfo.reselmInfo[0].startRes,
								&dlCh->mapInfo.reselmInfo[0].numRes);
						break;
					}
#ifdef NOT_SUPPORTED_BY_MAC
				case TFU_DCI_FORMAT_1:
					{
                  TfuDciFormat1AllocInfo *format1 = 
                     &tfuDatReqPdu->dciInfo.u.format1AllocInfo;
						dlCh->hCid = format1->harqProcId;
						subChInfo->mcsType = format1->mcs;
                  /*TODO:Changing this for the moment as per Vitaliy inputs, 
                    revert it if this chnage is not permanent.*/
						subChInfo->nDi = format1->ndi;
						subChInfo->rV = format1->rv;
						subChInfo->flushReq = 0;
						subChInfo->modulationType = ModulationType[format1->mcs];
						break;
					}
				case TFU_DCI_FORMAT_1B:
					{
                  TfuDciFormat1bAllocInfo *format1b = 
                     &tfuDatReqPdu->dciInfo.u.format1bAllocInfo;
						dlCh->hCid = format1b->harqProcId;
						subChInfo->mcsType = format1b->mcs;
						subChInfo->modulationType = ModulationType[format1b->mcs];
                  /*TODO:Changing this for the moment as per Vitaliy inputs, 
                    revert it if this chnage is not permanent.*/
						subChInfo->nDi = format1b->ndi;
						subChInfo->rV = format1b->rv;
						subChInfo->flushReq = 0;
						break;
					}
				case TFU_DCI_FORMAT_1C:
					{
						subChInfo->modulationType = BPSK;
						break;
					}
				case TFU_DCI_FORMAT_1D:
					{
                  TfuDciFormat1dAllocInfo *format1d = 
                     &tfuDatReqPdu->dciInfo.u.format1dAllocInfo;
						subChInfo->mcsType = format1d->mcs;
						subChInfo->modulationType = ModulationType[format1d->mcs];
						subChInfo->rV = format1d->rv;
						break;
					}
#endif
				case TFU_DCI_FORMAT_2:
					{
                  TfuDciFormat2AllocInfo *format2AllocInfo = 
                     &tfuDatReqPdu->dciInfo.u.format2AllocInfo ;

						subChInfo->mcsType = format2AllocInfo->tbInfo[0].mcs;
						subChInfo->modulationType = ModulationType[subChInfo->mcsType];
						subChInfo->rV = format2AllocInfo->tbInfo[0].rv;
						subChInfo->nDi = format2AllocInfo->tbInfo[0].ndi;
						subChInfo->flushReq = 0 ;/* need to revisit this */


                                                dlCh->numCodeWords = 1 ;
/* mimo_comment : remove this check*/
                  if( !((format2AllocInfo->tbInfo[1].mcs == 0 ) && 
                           (format2AllocInfo->tbInfo[1].rv == 1)))
						{
                                                   dlCh->numCodeWords++ ;

                     /* adding the second subchannel descriptor for the 
                        second CW if presetn */
					           subChInfoCw2->codingDescriptor = TURBOCDR;
						   subChInfoCw2->blockCodeConcatenation = 1;
						   subChInfoCw2->mcsType = format2AllocInfo->tbInfo[1].mcs;
                     subChInfoCw2->modulationType = 
                        ModulationType[subChInfoCw2->mcsType];
						   subChInfoCw2->rV = format2AllocInfo->tbInfo[1].rv;
						   subChInfoCw2->nDi = format2AllocInfo->tbInfo[1].ndi;
						   subChInfoCw2->flushReq = 0 ;/* need to revisit this */

						}

                                                if (cellCb->cellInfo.numAntPorts == 4 )
                                                {
                     dlCh->nLayers = 
                        pmiNumOfLayerMap4AntPortDCI2[dlCh->numCodeWords - 1 ]
                        [format2AllocInfo->precoding];
                                                }
                                                else if (cellCb->cellInfo.numAntPorts == 2 )
                                                {
                     dlCh->nLayers = 
                        pmiNumOfLayerMap2AntPortDCI2[dlCh->numCodeWords - 1 ]
                        [format2AllocInfo->precoding];
                                                }

                /* TODO MIMO set dlCh->dlPrecoderInfo.codeBookIdx */
						break;
					}
				case TFU_DCI_FORMAT_2A:
					{
                  TfuDciFormat2AAllocInfo *format2AAllocInfo = 
                     &tfuDatReqPdu->dciInfo.u.format2AAllocInfo ;
						if (format2AAllocInfo->transSwap)
						{
                           cw1index = 1;
						}
						else
						{
                           cw2index = 1;
						}

                  if(1 != tfuDatReqPdu->nmbOfTBs)
                  {
                     dlCh->dlPrecoderInfo.cddType = LARGECDD;
							subChInfo->mcsType = format2AAllocInfo->tbInfo[cw1index].mcs;
                     subChInfo->modulationType = 
                        ModulationType[subChInfo->mcsType];
							subChInfo->rV = format2AAllocInfo->tbInfo[cw1index].rv;
							subChInfo->nDi = format2AAllocInfo->tbInfo[cw1index].ndi;
                     subChInfo->flushReq = 0 ;/* need to revisit this */

                     dlCh->numCodeWords = 1 ;
                     /* mimo_comment : merge these blocks */
                     if( !((format2AAllocInfo->tbInfo[1].mcs == 0 ) && 
                              (format2AAllocInfo->tbInfo[1].rv == 1)))
                     {
                        dlCh->numCodeWords++ ;

                        subChInfoCw2->codingDescriptor = TURBOCDR;
                        subChInfoCw2->blockCodeConcatenation = 1;
                        subChInfoCw2->mcsType = 
                           format2AAllocInfo->tbInfo[cw2index].mcs;
                        subChInfoCw2->modulationType = 
                           ModulationType[subChInfoCw2->mcsType];
								subChInfoCw2->rV = format2AAllocInfo->tbInfo[cw2index].rv;
                        subChInfoCw2->nDi = 
                           format2AAllocInfo->tbInfo[cw2index].ndi;
                        subChInfoCw2->flushReq = 0 ;/* need to revisit this */
                        /* Keep track of 2 codewords for HARQ, won't be 
                           needed post TFU UPGRADE */
#ifndef TFU_UPGRADE
                        ueCb->cwCount[timingInfo.subframe] = 2;
#endif
                        dlCh->transmissionMode = OPNLPSPMUX;
                     }
                  }
                  else if((format2AAllocInfo->tbInfo[1].mcs == 0 ) && 
                        (format2AAllocInfo->tbInfo[1].rv == 1))
                  {
                     dlCh->dlPrecoderInfo.cddType = LARGECDD;
                     subChInfo->mcsType = format2AAllocInfo->tbInfo[0].mcs;
                     subChInfo->modulationType = 
                        ModulationType[subChInfo->mcsType];
                     subChInfo->rV = format2AAllocInfo->tbInfo[0].rv;
                     subChInfo->nDi = format2AAllocInfo->tbInfo[0].ndi;
                     subChInfo->flushReq = 0 ;/* need to revisit this */

                     dlCh->numCodeWords = 1 ;
                     dlCh->transmissionMode = TXDIVERSITY;
                  }
                  else
                  {
                     MSPD_DBG("\n Retransmitting 1st TB alone with TM2 mode \
                           Filling TX vector\n");
                     dlCh->dlPrecoderInfo.cddType = LARGECDD;
                     subChInfo->mcsType = format2AAllocInfo->tbInfo[1].mcs;
                     subChInfo->modulationType = 
                        ModulationType[subChInfo->mcsType];
                     subChInfo->rV = format2AAllocInfo->tbInfo[1].rv;
                     subChInfo->nDi = format2AAllocInfo->tbInfo[1].ndi;
                     subChInfo->flushReq = 0 ;/* need to revisit this */

                     dlCh->numCodeWords = 1 ;
                     dlCh->transmissionMode = TXDIVERSITY;
                  }

                  if (cellCb->cellInfo.numAntPorts == 4 )
                  {
                     dlCh->nLayers = 
                        pmiNumOfLayerMap4AntPortDCI2A[dlCh->numCodeWords - 1 ]
                        [format2AAllocInfo->precoding];
                  }
                  else
                  {
                     dlCh->nLayers = dlCh->numCodeWords ;
                  }
                  /* TODO MIMO set dlCh->dlPrecoderInfo.codeBookIdx */
                  if (format2AAllocInfo->isAllocType0)
                     ysMsUtlFillMappingInfoForRaType0(cellCb, 
                           format2AAllocInfo->resAllocMap,
                           &dlCh->mapInfo);
                  else
                     YS_DBG_ERR((_ysp,"CL: Unhandled resource allocation type\n"));
               }
               break;
               default:
               {
               YS_DBG_INFO((_ysp, "Unhandled DCI format"));
               }
      }
      break;
		case PBCH:
			subChInfo->codingDescriptor = TBVITCC;
			subChInfo->blockCodeConcatenation = YS_BLK_CODE_CAT_DIS;
			subChInfo->mcsType = 4;

			subChInfo->modulationType = QPSK;  /*  36.211 - 6.6.2 */
			dlCh->crcInfo.crcLength = SIXTEEN; /*-- 36.212 - 5.3.1 --*/
			dlCh->crcInfo.crcScrambling = 1;
			/* Mindspeed requires this value to be 0 as of now */
         /*TODO:Can compute this value one time and store it in a 
           parmeteter in Cell CB so that it doesn't
			 * gets computed everytime*/
			dlCh->scrInfo.scrinitValueinput = cellCb->cmnChScrInit;

			break;
	}
   dlCh->scrInfo.scramblerType = DLSCR;
   RETVALUE(ROK);
}

/*
*
*       Fun:   ysMsUtlFillDCIChanDesc
*
*       Desc:
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsUtlFillDCIChanDesc
(
TfuPdcchInfo    *pdcchInfo,
YsCellCb        *cellCb,
PDCICHANNELDESC  dciCh,
U16              channelId,
CmLteTimingInfo  timingInfo
)
#else
PUBLIC S16 ysMsUtlFillDCIChanDesc (pdcchInfo, cellCb, dciCh,channelId, timingInfo)
TfuPdcchInfo    *pdcchInfo;
YsCellCb        *cellCb;
PDCICHANNELDESC dciCh;
U16             channelId;
CmLteTimingInfo timingInfo;
#endif
{
   /* HARQ */
   YsPhyTxSdu phyTxSdu;

   /*presently we do not have any parmaeter on CTF/TFU interface to derive
    * txpowerControl value, hence hardcoding it. We would eventually need to get
    * this parameter value from RRM/MAC. */
   dciCh->txpowerControl = 0; /* dci.u.format0Info.tpcCmd? */
   dciCh->reserved = 0;
   dciCh->reserved1 = 0;
   dciCh->numCodeWords = 1;
   dciCh->nLayers = cellCb->cellInfo.numAntPorts;
   dciCh->transmissionMode = cellCb->cellInfo.cmnTransMode;
   /*-- number of entries in the resource mapping info --*/
   dciCh->numberofEntries = 1;

   dciCh->channelType = PDCCH;
   dciCh->channelId = channelId;

   dciCh->crcLength = SIXTEEN;

   dciCh->startRes = pdcchInfo->nCce;
   switch(pdcchInfo->aggrLvl)
   {
      case CM_LTE_AGGR_LVL1:
         dciCh->numRes = 1;
         break;
      case CM_LTE_AGGR_LVL2:
         dciCh->numRes = 2;
         break;
      case CM_LTE_AGGR_LVL4:
         dciCh->numRes = 4;
         break;
      case CM_LTE_AGGR_LVL8:
         dciCh->numRes = 8;
         break;
      default:
         MSPD_DBG("Invalid aggrLvl\n");
         uart_printf("Invalid aggrLvl\n");
         break;
   }

   switch(pdcchInfo->dci.dciFormat)
   {
      case TFU_DCI_FORMAT_0:
         dciCh->crcScrambling = 1; /* 0 = Disabled or 1 = RNTI */
         dciCh->scrmblerInitValue = pdcchInfo->rnti;
         dciCh->reserved1 = 0; /*pdcchInfo->dci.u.format0Info.mcs; -- 
                                 temp change to see if CQI 14 CRC error goes away*/
         break;
      case TFU_DCI_FORMAT_1A:
         dciCh->crcScrambling = 1; /* 0 = Disabled or 1 = RNTI */
         dciCh->scrmblerInitValue = pdcchInfo->rnti;

         dciCh->reserved1 = 0; /* pdcchInfo->dci.u.format1aInfo.t.pdschInfo.
                                  allocInfo.mcs; -- temp changes to see if 
                                  crc error on CQI 14 goes away */
         break;
      case TFU_DCI_FORMAT_2:
         {

            dciCh->crcScrambling = 1; /* 0 = Disabled or 1 = RNTI */
            dciCh->scrmblerInitValue = pdcchInfo->rnti;
            /* Doing work around for issuing the proper PUCCH format in the 
               reception request for multi TB case */
            /* TODO MIMO: codeword stuff should be handled as part of PDSCH, 
               not here */
            /* ueCb->ueSubFrInfo[timingInfo.subframe].cwCount = 
               dciCh->numCodeWords ; */
            break ;
         }

      case TFU_DCI_FORMAT_2A:
         {

            dciCh->crcScrambling = 1; /* 0 = Disabled or 1 = RNTI */
            dciCh->scrmblerInitValue = pdcchInfo->rnti;

            break;
         }
	  case TFU_DCI_FORMAT_3:
	  	{
			dciCh->crcScrambling = 1;
			dciCh->scrmblerInitValue = pdcchInfo->rnti;
			break;
      	}
      case TFU_DCI_FORMAT_3A:
	  	{
			dciCh->crcScrambling = 1;
			dciCh->scrmblerInitValue = pdcchInfo->rnti;
			break;
      	}
      case TFU_DCI_FORMAT_1:
      case TFU_DCI_FORMAT_1B:
      case TFU_DCI_FORMAT_1C:
      case TFU_DCI_FORMAT_1D:
	  	{
         YS_DBG_INFO((_ysp, "Unhandled DCI format"));
         break;
      	}
   }
   /*-- Fill the TxSdu --*/
   if (ysMsUtlFillTxSdu(&phyTxSdu, NULLP, NULLP,
            NULLP, pdcchInfo, PDCCH,
            dciCh->channelId, cellCb,0) != ROK)
   {
      RETVALUE(ROK);
   }
   /* Add this to the linked list of PHY messages */
   {
      PMAC2PHY_QUEUE_EL pElem = ysMsUtlGetPhyListElem();
      YsPhyLstCp        *pDlCntrl = &cellCb->dlEncL1Msgs[timingInfo.subframe].dlCntrl;
      PTXSDUREQ         pTxSduReq       = (PTXSDUREQ)phyTxSdu.txSduReq;
      if (!pElem)
      {
         /* HARQ_DBG */
         uart_printf("ysMsUtlAllocSvsrMsg failed for PHY list element \
               for PDCCH SDU at time (%d,%d)\n",
         cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
         RETVALUE(RFAILED);
      }
      YS_MS_FILL_PHY_LIST_ELEM(pElem, timingInfo.sfn, timingInfo.subframe,
            pTxSduReq, phyTxSdu.sduLen, PHY_TXSDU_REQ);
      ysMsUtlAddToPhyList(pDlCntrl,pElem);
   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   ysMsUtlPrntErrInfo
*
*       Desc:  Prints the Error Information
*
*       Ret:   void
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PUBLIC Void ysMsUtlPrntErrInfo
(
U8         errCode
)
#else
PUBLIC Void ysMsUtlPrntErrInfo(errCode)
U8         errCode;
#endif
{

   TRC2(ysMsUtlPrntErrInfo);

   switch(errCode)
   {
      case 1:
         YS_DBG_INFO((_ysp, "\nError Code : %d Primitive is not Supported\n", 
                  errCode));
         break;
      case 2:
         YS_DBG_INFO((_ysp, "\nError Code : %d FEC Code Type is not Supported\n",
                  errCode));
         break;
      case 3:
         YS_DBG_INFO((_ysp, "\nError Code : %d Overrun\n", errCode));
         break;
      case 4:
         YS_DBG_INFO((_ysp, "\nError Code : %d Underrun\n", errCode));
         break;
      case 5:
         YS_DBG_INFO((_ysp, "\nError Code : %d Transport Media Error\n", errCode));
         break;
      case 6:
         YS_DBG_INFO((_ysp, "\nError Code : %d TX data size does not match TXVECTOR\n",
                               errCode));
         break;
      case 7:
         YS_DBG_INFO((_ysp, "\nError Code : %d Invalid TXVECTOR format\n", 
                  errCode));
         break;
      case 8:
         YS_DBG_INFO((_ysp, "\nError Code : %d Invalid RXVECTOR format\n", 
                  errCode));
         break;
      case 9:
         YS_DBG_INFO((_ysp, "\nError Code : %d RX CRC Error\n", errCode));
         break;
      case 10:
         YS_DBG_INFO((_ysp, "\nError Code : %d Transmit timeout with missing \
                  TXSDU\n", errCode));
         break;
      case 11:
         YS_DBG_INFO((_ysp, "\nError Code : %d Receiver did not match RXSDU \
                  in RXVECTOR\n", errCode));
         break;
      case 12:
         YS_DBG_INFO((_ysp, "\nError Code : %d Timeout on TXSTART.req\n", 
                  errCode));
         break;
      case 13:
         YS_DBG_INFO((_ysp, "\nError Code : %d Timeout on RXSTART.req\n", 
                  errCode));
         break;
      case 14:
         YS_DBG_INFO((_ysp, "\nError Code : %d No Custom Extensions Handler \
                  Registered\n", errCode));
         break;
      default:
         YS_DBG_INFO((_ysp, "\nError Code : %d Unknown Error Code\n", errCode));
         break;
   }

   RETVOID;

} /* end of ysMsUtlPrntErrInfo */


/***********************************************************
 *
 *     Func : ysMsCalcRiv
 *
 *     Desc : This function calculates RIV.
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U16 ysMsCalcRiv
(
U8           bw,
U8           rbStart,
U8           numRb
)
#else
PRIVATE U16 ysMsCalcRiv (bw, rbStart, numRb)
U8           bw;
U8           rbStart;
U8           numRb;
#endif
{
   U8           numRbMinus1 = numRb - 1;
   U16          riv;

   TRC2(ysMsCalcRiv);

   if (numRbMinus1 <= bw/2)
   {
      riv = bw * numRbMinus1 + rbStart;
   }
   else
   {
      riv = bw * (bw - numRbMinus1) + (bw - rbStart - 1);
   }
   RETVALUE(riv);
} /* rgMUXCalcRiv */

/* EXTERN Bool capDbg; */
/*
*
*       Fun:   ysMsUtlFillTxSdu
*
*       Desc:
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsUtlFillTxSdu
(
YsPhyTxSdu     *phyTxSdu,
TfuDatReqInfo *bchDatReq,
TfuDatReqPduInfo *datReq,
TfuPhichInfo  *phichInfo,
TfuPdcchInfo  *pdcchInfo,
U8            channelType,
U16           channelId,
YsCellCb      *cellCb,
U8            tbIndex
)
#else
PUBLIC S16 ysMsUtlFillTxSdu( phyTxSdu, bchDatReq, datReq, phichInfo, 
      pdcchInfo, channelType, channelId, cellCb,tbIndex)
YsPhyTxSdu     *phyTxSdu;
TfuDatReqInfo *bchDatReq;
TfuDatReqPduInfo *datReq;
TfuPhichInfo  *phichInfo;
TfuPdcchInfo  *pdcchInfo;
U8            channelType;
U16           channelId;
YsCellCb      *cellCb;
U8            tbIndex;
#endif
{

   PGENMSGDESC pMsgDesc;
   PTXSDUREQ  txSduReq;
   MsgLen     bufLen = 0;
   MsgLen     cLen = 0;
   S16        ret = ROK;
   U8         dci[8];
   U16        dciSize;
   U8         *pDci = dci;
#ifdef MSPD_MLOG_NEW
   U32        startTick = 0;
   U32        t=0;
#endif
   TRC2(ysMsUtlFillTxSdu)
#if (ERRCLASS & ERRCLS_DEBUG)
   if (datReq == NULLP)
   {
      YS_LOGERR_DBG(EYSXXX, 0, "ysMsUtlFillTxSdu(): invalid ptr\n");
      RETVALUE (RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */
#ifdef MSPD_MLOG_NEW
      startTick = GetTIMETICK();
#endif /* MSPD_MLOG_NEW */


   pMsgDesc = (PGENMSGDESC)ysMsUtlAllocSvsrMsg(TRUE);
   if (pMsgDesc == NULLP)
   {
      YS_DBG_ERR((_ysp, "Failed to allocate memory for TXSDU"));
      RETVALUE (RFAILED);
   }

   txSduReq = (PTXSDUREQ)(pMsgDesc);

   switch(channelType)
	{
		case PHICH:
#ifdef MSPD_MLOG_NEW
			t = GetTIMETICK();
#endif
			if (phichInfo)
			{
            /*Start of changes to fill the correct PHICH group number*/
				double nGPhich;
				double nG;
				/* HARQ_UL: Added computation for sequence index */
				int seqIdx;
				double nSfPhich;
            U8 bw = cellCb->cellCfg.bwCfg.dlBw;

				txSduReq->nackAck = phichInfo->isAck;

				/* HARQ_UL: Removed the incorrect assigment of phichSeqIndex*/

				switch(cellCb->cellCfg.phichCfg.resource)
				{
					case CTF_PHICH_RSRC_ONESIXTH:
						nG= 1.0/6.0;
						break;
					case CTF_PHICH_RSRC_HALF:
						nG= 1.0/2.0;
						break;
					case CTF_PHICH_RSRC_ONE:
						nG= 1.0;
						break;
					case CTF_PHICH_RSRC_TWO:
						nG= 2.0;
						break;

					default:
						break;
				}
				if(CTF_PHICH_DUR_NORMAL == cellCb->cellCfg.phichCfg.duration)
				{
					nGPhich = (nG * ((bw + (bw -1)) / 8.0));
					nSfPhich = 4.0;
				}
				else
				{
					nGPhich = 2.0 * (nG * ((bw + (bw -1)) / 8.0));
					nSfPhich = 2.0;
				}
            /*Updated the phichGrpNumber as per 36213-9.1.2 */
            txSduReq->phichGrpNumber = (phichInfo->rbStart + phichInfo->nDmrs)
               %  (U16)nGPhich + 0 * (U16)nGPhich;
                /* HARQ_UL */
            seqIdx =  (int)((int)((phichInfo->rbStart/nG) + phichInfo->nDmrs)
                  % (int)(2 * nSfPhich));
                txSduReq->phichSeqIndex = seqIdx;
				txSduReq->msgLen = sizeof(txSduReq->pTxSdu);
				phyTxSdu->sduLen = sizeof(TXSDUREQ);

			}
#ifdef MSPD_MLOG_NEW
			MLogTask(PID_CL_PHICH_DAT_REQ, RESOURCE_LARM, t, GetTIMETICK());
#endif
			break;

		case PDCCH:
#ifdef MSPD_MLOG_NEW
			t = GetTIMETICK();
#endif
			cmMemset(pDci, 0, 8);
			switch(pdcchInfo->dci.dciFormat)
			{
				case TFU_DCI_FORMAT_0:
					{
						U16 bW, bits, idx = 0;
						U16 bitIdx = 0 ;
						U16          riv;
						TfuDciFormat0Info *format0 = &pdcchInfo->dci.u.format0Info;

						bW = YS_MS_GET_UL_BW(cellCb->cellCfg.bwCfg);

						/*-- Calculate resource block assignment bits need to be set
						  Which is ln(N(N+1)/2) 36.212 5.3.3.1 --*/
						bits = (bW * (bW + 1) / 2);
						while ((bits & 0x8000) == 0)
						{
							bits <<= 1;
							idx++;
						}
						bits = 16 - idx;

						YS_MS_SETBITRANGE(pDci, bitIdx, 1, 0); bitIdx++; /*-- Format 0 --*/
						YS_MS_SETBITRANGE(pDci, bitIdx, 1, format0->hoppingEnbld);
						bitIdx++;
						riv = ysMsCalcRiv(bW, format0->rbStart, format0->numRb);
						if (format0->hoppingEnbld)
						{
							/*-- Resource block assignment bits need to be set
							  Which is ln(N(N+1)/2) --*/
                     /*-- How to calculate this --*/
                     YS_MS_SETBITRANGE(pDci, bitIdx, format0->hoppingBits, 0);  
							bitIdx += format0->hoppingBits;
                     /*-- How to calculate this --*/
                     YS_MS_SETBITRANGE(pDci, bitIdx, 
                     (bits - format0->hoppingBits), 0); 
							bitIdx += (bits - format0->hoppingBits);
						}
						else
						{
                     /*-- How to calculate this --*/
                     YS_MS_SETBITRANGE(pDci, bitIdx, bits, riv); 
							bitIdx += (bits);
						}

						YS_MS_SETBITRANGE(pDci, bitIdx, 5, format0->mcs);
						bitIdx += 5;
						YS_MS_SETBITRANGE(pDci, bitIdx, 1, format0->ndi);

                  YS_DBG_FATAL((_ysp, "\n ysMsUtlFillDCIChanDesc filling \
                           PDCCH format 0 at : (%d,%d) ndi (%d) \n",
                           cellCb->timingInfo.sfn, 
                           cellCb->timingInfo.subframe, format0->ndi));

						bitIdx++;
						YS_MS_SETBITRANGE(pDci, bitIdx, 2, format0->tpcCmd);
						bitIdx += 2;
						YS_MS_SETBITRANGE(pDci, bitIdx, 3, format0->nDmrs);
						bitIdx += 3;
						YS_MS_SETBITRANGE(pDci, bitIdx, 1, format0->cqiReq);
						bitIdx++;
                  /* fix for dci size () missing */
						dciSize = (bitIdx / 8) + ((bitIdx % 8)?1:0);
                  /*fix for decoding on WJ.*/
                  /*Adding two bits for format 0 to make it equat to size for
						 * format 1A, i.e. 27 bits */
						txSduReq->maxBitsperCw = bitIdx+2;
						break;
					}
				case TFU_DCI_FORMAT_1A:
					{
						U16 bW, bits, idx = 0;
						U16 bitIdx = 0;
						Tfudciformat1aPdsch *format1a;

						bW = cellCb->cellInfo.dlBw;

						/*-- Calculate resource block assignment bits need to be set
						  Which is ln(N(N+1)/2) --*/
						bits = (bW * (bW + 1) / 2);
						while ((bits & 0x8000) == 0)
						{
							bits <<= 1;
							idx++;
						}
						bits = 16 - idx;


						YS_MS_SETBITRANGE(pDci, bitIdx, 1, 1); bitIdx++; /*-- Format 1A --*/
						if (pdcchInfo->dci.u.format1aInfo.isPdcchOrder)
						{
							YS_MS_SETBITRANGE(pDci, bitIdx, 1, 0); bitIdx++;
							/*-- Resource block assignment bits need to be set
							  Which is ln(N(N+1)/2) --*/
							YS_MS_SETBITRANGE(pDci, bitIdx, bits, 0xffff);
							bitIdx += bits;

                     YS_MS_SETBITRANGE(pDci, bitIdx, 6, 
                           pdcchInfo->dci.u.format1aInfo.t.
									pdcchOrder.preambleIdx);
							bitIdx += 6;

                     YS_MS_SETBITRANGE(pDci, bitIdx, 4, 
                           pdcchInfo->dci.u.format1aInfo.t.
									pdcchOrder.prachMaskIdx);
							bitIdx += 4;
						}
						else
						{
							format1a = &pdcchInfo->dci.u.format1aInfo.t.pdschInfo;
							if (format1a->allocInfo.isLocal)
							{
								YS_MS_SETBITRANGE(pDci, bitIdx, 1, 0); bitIdx++;
							}
							else
							{
								YS_MS_SETBITRANGE(pDci, bitIdx, 1, 1); bitIdx++;
							}
							/*-- Resource block assignment bits need to be set
							  Which is ln(N(N+1)/2) --*/
                     YS_MS_SETBITRANGE(pDci, bitIdx, bits, 
                           format1a->allocInfo.alloc.u.riv);
							bitIdx += bits;

							YS_MS_SETBITRANGE(pDci, bitIdx, 5, format1a->allocInfo.mcs);
							bitIdx += 5;
                     YS_MS_SETBITRANGE(pDci, bitIdx, 3, 
                           format1a->allocInfo.harqProcId.val);
							bitIdx += 3;
							YS_MS_SETBITRANGE(pDci, bitIdx, 1, format1a->allocInfo.ndi);
							bitIdx++;
							YS_MS_SETBITRANGE(pDci, bitIdx, 2, format1a->allocInfo.rv);
							bitIdx += 2;
							YS_MS_SETBITRANGE(pDci, bitIdx, 2, format1a->tpcCmd);
							bitIdx += 2;
						}
						dciSize = (bitIdx / 8) + ((bitIdx % 8)?1:0);

                  /*fix for decoding on WJ.*/
						txSduReq->maxBitsperCw = bitIdx+1;
						break;
					}
				case TFU_DCI_FORMAT_1:
					YS_DBG_INFO((_ysp, "DCI format 1 unhandled"));
					RETVALUE(RFAILED);
				case TFU_DCI_FORMAT_1B:
					YS_DBG_INFO((_ysp, "DCI format 1B unhandled"));
					RETVALUE(RFAILED);
				case TFU_DCI_FORMAT_1C:
					YS_DBG_INFO((_ysp, "DCI format 1C unhandled"));
					RETVALUE(RFAILED);
				case TFU_DCI_FORMAT_1D:
					YS_DBG_INFO((_ysp, "DCI format 1D unhandled"));
					RETVALUE(RFAILED);
				case TFU_DCI_FORMAT_2:
					{
                                  U16 bW = 0 ;
                                  U16 bitIdx = 0 ;
                                  U16 resMapLen = 0 ;
                                  U8 rbgSize = 0 ;
				  U8 bitCount = 0 ;
				  U8 tempSize = 0 ;
				  U8 rbgPerBwSize = 0 ;
				  U8 bytesPerMapLen = 0 ;
				  U8 resMapCount = 0 ;

                                  TfuDciFormat2Info *format2 = &pdcchInfo->dci.u.format2Info ;
                  TfuDciFormat2AllocInfo *format2AllocInfo = 
                     &pdcchInfo->dci.u.format2Info.allocInfo ;

                                  bW = cellCb->cellInfo.dlBw;
                                  rbgSize = cellCb->cellInfo.rbgSize ;

                                  /* Resource allocation header is present iff the BW is > 10 PRBS */
                                  if(bW > 10 )
                                  {
                                     /* type 0 - 0 type 1 -1 */
                     YS_MS_SETBITRANGE(pDci, bitIdx, 1, 
                           !(format2AllocInfo->isAllocType0) );
                                     bitIdx++ ;
                                  }

                                  if( format2AllocInfo->isAllocType0 )
                                  {/* Type 0 : size is bw/P */
/* mimo_comment: ceiling*/
                     resMapLen = ( bW % rbgSize )? ( bW / rbgSize):
                        (( bW / rbgSize) + 1 )  ;
                                  }else
                                  {/* Type 1 : size is ln(p) + 1 + ((bw/p) - ln(p) - 1) */
                                     bitCount = 1 ;
                                     while ( bitCount <= rbgSize)
                                     {
                                        bitCount <<= 1 ;
                                        tempSize++ ;
                                     }
/* mimo_comment: ceiling*/
                     rbgPerBwSize = ( bW % rbgSize )? ( bW / rbgSize):
                        (( bW / rbgSize) + 1 )  ;
                                     resMapLen = tempSize + 1 + ( rbgPerBwSize - tempSize - 1 ) ;
                                  }
                                  /* Resource allocation map field */
  				  bytesPerMapLen = (resMapLen >> 3) + ((resMapLen % 8)?1:0);
/* mimo_comment: process the last byte outside the loop*/
                  for ( resMapCount = 0 ; resMapCount < bytesPerMapLen - 1; 
                        resMapCount++)
                                  {
                     YS_MS_SETBITRANGE(pDci, bitIdx, 8 ,
                           format2AllocInfo->resAllocMap[resMapCount]);
					bitIdx += 8 ;
                                  }

				  /* filling from the last byte info */
                                 if (!( resMapLen % 8 ))
                                 {
                     YS_MS_SETBITRANGE(pDci, bitIdx, 8 ,
                           format2AllocInfo->resAllocMap[resMapCount]);
				    bitIdx += 8 ;
                                 }else
			 	 {
                     YS_MS_SETBITRANGE(pDci, bitIdx, (resMapLen % 8 ) ,
                           format2AllocInfo->resAllocMap[resMapCount]);
				    bitIdx += (resMapLen % 8 ) ;
			  	 }

                                  YS_MS_SETBITRANGE(pDci, bitIdx, 2 ,format2->tpcCmd);
                                  bitIdx += 2 ;
                                  /* TODO::Fill DAI for TDD */


                                  YS_MS_SETBITRANGE(pDci, bitIdx, 3 ,format2AllocInfo->harqProcId);
                                  bitIdx += 3 ;

                                  YS_MS_SETBITRANGE(pDci, bitIdx, 1 ,format2AllocInfo->transSwap);
                                  bitIdx += 1 ;

                  YS_MS_SETBITRANGE(pDci, bitIdx, 5 ,
                        format2AllocInfo->tbInfo[0].mcs);
                                  bitIdx += 5 ;

                  YS_MS_SETBITRANGE(pDci, bitIdx, 1 ,
                        format2AllocInfo->tbInfo[0].ndi);
                                  bitIdx += 1 ;

                                  YS_MS_SETBITRANGE(pDci, bitIdx, 2 ,format2AllocInfo->tbInfo[0].rv);
                                  bitIdx += 2 ;

                  YS_MS_SETBITRANGE(pDci, bitIdx, 5 ,
                        format2AllocInfo->tbInfo[1].mcs);
                                  bitIdx += 5 ;

                  YS_MS_SETBITRANGE(pDci, bitIdx, 1 ,
                        format2AllocInfo->tbInfo[1].ndi);
                                  bitIdx += 1 ;

                                  YS_MS_SETBITRANGE(pDci, bitIdx, 2 ,format2AllocInfo->tbInfo[1].rv);
                                  bitIdx += 2 ;

                  /* if antennaPortCount = 2, precoding size is 3, 
                     if antennaPortCount = 4 precoding size is 6*/
                  if( YS_MS_GET_NUM_TX_ANT(cellCb->cellCfg.antennaCfg) == 2 ) 
                                  {
                     /* Get the antennaCount value */
                                     YS_MS_SETBITRANGE(pDci, bitIdx, 3 ,format2AllocInfo->precoding);
                                     bitIdx += 3 ;
                                  }else if( YS_MS_GET_NUM_TX_ANT(cellCb->cellCfg.antennaCfg) == 4 )
                                  {
                                     YS_MS_SETBITRANGE(pDci, bitIdx, 6 ,format2AllocInfo->precoding);
                                     bitIdx += 6 ;
                                  }
				  txSduReq->maxBitsperCw = bitIdx+1;
  				  dciSize = (bitIdx / 8) + ((bitIdx % 8)?1:0);
				  break ;
					}
				case TFU_DCI_FORMAT_2A:
					{
                                  U16 bW = 0 ;
                                  U16 bitIdx = 0 ;
                                  U16 resMapLen = 0 ;
                                  U8 rbgSize = 0 ;
				  U8 bitCount = 0 ;
				  U8 tempSize = 0 ;
				  U8 rbgPerBwSize = 0 ;
				  U8 resMapBytes = 0 ;

                                  TfuDciFormat2AInfo *format2a = &pdcchInfo->dci.u.format2AInfo ;
                  TfuDciFormat2AAllocInfo *format2AAllocInfo = 
                     &pdcchInfo->dci.u.format2AInfo.allocInfo ;

                                  bW = cellCb->cellInfo.dlBw;
                                  rbgSize = cellCb->cellInfo.rbgSize ;

                                  /* Resource allocation header is present iff the BW is > 10 PRBS */
                                  if(bW > 10 )
                                  {
                                     /* type 0 - 0 type 1 -1 */
                     YS_MS_SETBITRANGE(pDci, bitIdx, 1,
                           !(format2AAllocInfo->isAllocType0) );
                                     bitIdx++ ;
                                  }

                                  if( format2AAllocInfo->isAllocType0 )
                                  {/* Type 0 : size is bw/P */
                                     /* TODO MIMO: precompute */
                                     resMapLen = YS_MS_CEIL(bW, rbgSize);
                                  }else
                                  {/* Type 1 : size is ln(p) + 1 + ((bw/p) - ln(p) - 1) */
                                     bitCount = 1 ;
                                     while ( bitCount <= rbgSize)
                                     {
                                        bitCount <<= 1 ;
                                        tempSize++ ;
                                     }
                     rbgPerBwSize = ( bW % rbgSize )? ( bW / rbgSize):
                        (( bW / rbgSize) + 1 )  ;
                                     resMapLen = tempSize + 1 + ( rbgPerBwSize - tempSize - 1 ) ;
                                  }

                                  /* Resource allocation map field */
                                  if (bitIdx)
                                  {
                                     /* Shift by one and copy res map bits */
                                     U8 dciidx = 0;
                                     U8 i;
                                     U8 resmap = format2AAllocInfo->resAllocMap[0];
                                     resMapBytes = (resMapLen-1) / 8;
                                     for (i = 0; i < resMapBytes; i++)
                                     {
                                        resmap = format2AAllocInfo->resAllocMap[i];
                                        pDci[dciidx] |= (resmap >> 1);
                                        pDci[dciidx+1] |= (resmap << 7);
                                        ++dciidx;
                                     }
                                     resmap = format2AAllocInfo->resAllocMap[i];
                                     pDci[dciidx++] |= (resmap >> 1);
                                  }
                                  else
                                  {
                                     U32 i;
                                     for (i = 0; i < TFU_MAX_ALLOC_BYTES; ++i)
                                        pDci[i] = format2AAllocInfo->resAllocMap[i];
                                  }
                                  bitIdx += resMapLen;

                                  YS_MS_SETBITRANGE(pDci, bitIdx, 2 ,format2a->tpcCmd);
                                  bitIdx += 2 ;
                                  /* TODO::Fill DAI for TDD */

                  YS_MS_SETBITRANGE(pDci, bitIdx, 3 ,
                        format2AAllocInfo->harqProcId);
                                  bitIdx += 3 ;

                                  YS_MS_SETBITRANGE(pDci, bitIdx, 1 ,format2AAllocInfo->transSwap);
                                  bitIdx += 1 ;

                  YS_MS_SETBITRANGE(pDci, bitIdx, 5 ,
                        format2AAllocInfo->tbInfo[0].mcs);
                                  bitIdx += 5 ;

                  YS_MS_SETBITRANGE(pDci, bitIdx, 1 ,
                        (format2AAllocInfo->tbInfo[0].ndi & 0x01));
                                  bitIdx += 1 ;

                  YS_MS_SETBITRANGE(pDci, bitIdx, 2 ,
                        format2AAllocInfo->tbInfo[0].rv);
                                  bitIdx += 2 ;

                  YS_MS_SETBITRANGE(pDci, bitIdx, 5 ,
                        format2AAllocInfo->tbInfo[1].mcs);
                                  bitIdx += 5 ;

                  YS_MS_SETBITRANGE(pDci, bitIdx, 1 ,
                        (format2AAllocInfo->tbInfo[1].ndi & 0x01));
                                  bitIdx += 1 ;

                  YS_MS_SETBITRANGE(pDci, bitIdx, 2 ,
                        format2AAllocInfo->tbInfo[1].rv);
                                  bitIdx += 2 ;

                  /* if antennaPortCount = 2, precoding size is 3, 
                  if antennaPortCount = 4 precoding size is 6*/
                  if( YS_MS_GET_NUM_TX_ANT(cellCb->cellCfg.antennaCfg) == 4) 
                                  {
                     /* Get the antennaCount value */
                                     YS_MS_SETBITRANGE(pDci, bitIdx, 2 ,format2AAllocInfo->precoding);
                                     bitIdx += 2 ;
                                  }
				  txSduReq->maxBitsperCw = bitIdx+1;
  				  dciSize = (bitIdx / 8) + ((bitIdx % 8)?1:0);
				  break ;
					}
				case TFU_DCI_FORMAT_3:
					{
						U16 bitIdx = 0 ;
						U8  idx =1;
		                TfuDciFormat3Info *format3 = &pdcchInfo->dci.u.format3Info;	
                        /* GRP_PWR: TODO. Make computation for number of indices function of dci format 0 size */ 
						for (; idx < TFU_MAX_2BIT_TPC - 2 ; ++idx)
						{
						   YS_MS_SETBITRANGE(pDci, bitIdx, 2, format3->tpcCmd[idx]);
						   bitIdx += 2;
						}	
						dciSize = (bitIdx / 8) + ((bitIdx % 8)?1:0);
						txSduReq->maxBitsperCw = bitIdx+1;
					   break;
					}
				case TFU_DCI_FORMAT_3A:
					{
						U16 bitIdx = 0 ;
						U8  idx =1;
		                TfuDciFormat3AInfo *format3a = &pdcchInfo->dci.u.format3AInfo;	
                        /* GRP_PWR: TODO. Make computation for number of indices function of dci format 0 size */ 
						for (; idx < TFU_MAX_1BIT_TPC - 5 ; ++idx)
						{
						   YS_MS_SETBITRANGE(pDci, bitIdx, 1, format3a->tpcCmd[idx]);
						   bitIdx += 1;
						}		
						dciSize = (bitIdx / 8) + ((bitIdx % 8)?1:0);
						txSduReq->maxBitsperCw = bitIdx + 1;
					   break;
					}
				default:
					YS_DBG_ERR((_ysp, "Unknown DCI format"));
					RETVALUE(RFAILED);
			}
			if(dciSize > sizeof(txSduReq->pTxSdu))
			{
				phyTxSdu->sduLen =
					sizeof(TXSDUREQ)+dciSize - sizeof(txSduReq->pTxSdu) ;
			}
			else
			{
				phyTxSdu->sduLen = sizeof(TXSDUREQ);
			}

         /*fix for copying the TX SDU */
			_memcpy((U8 *)((U8 *)txSduReq + sizeof(TXSDUREQ)), pDci, dciSize);
			txSduReq->msgLen = dciSize;
			/* Protection for not sending TxSdu len zero */
			if (0 == txSduReq->msgLen)
			{
            stop_printf("Trying to send txSduReq->msgLen as zero for DCI \
                  Txsdu @celltime (%u, %u)\n",cellCb->timingInfo.sfn,
						cellCb->timingInfo.subframe);
			}

         /*For PDCCH, this parameter isin't relevant. */
			txSduReq->phichGrpNumber = 0;

         /*For PDCCH channel these parameters are not relevant, 
           hence setting them to 0.*/
			txSduReq->nackAck = 0;
			txSduReq->uciFormat = 0;
#ifdef MSPD_MLOG_NEW
			MLogTask(PID_CL_PDCCH_DAT_REQ, RESOURCE_LARM, t, GetTIMETICK());
#endif
			break;

		case PDSCH:
#ifdef MSPD_MLOG_NEW
			t = GetTIMETICK();
#endif
			if (!datReq->mBuf[tbIndex])
				break;
			ret = SFndLenMsg(datReq->mBuf[tbIndex], &bufLen);
			if (ret != ROK)
			{
				RETVALUE(RFAILED);
			}
                        {
                           extern U32 dlrate_tfu;
                           dlrate_tfu += bufLen;
                        }

         /*For PDSCH channel these parameters are not relevant, 
           hence setting them to 0.*/
			txSduReq->nackAck = 0;
			txSduReq->uciFormat = 0;

			txSduReq->msgLen = bufLen;
			/* Protection for not sending TxSdu len zero */
			if (0 == txSduReq->msgLen)
			{
            stop_printf("Trying to send txSduReq->msgLen as zero for Data\
                  Txsdu @celltime (%u, %u)\n",cellCb->timingInfo.sfn,
						cellCb->timingInfo.subframe);
			}
			txSduReq->maxBitsperCw = (bufLen * 8);
         /*For PDSCH, this parameter isin't relevant. */
			txSduReq->phichGrpNumber = 0;
			phyTxSdu->sduLen = sizeof(TXSDUREQ) + bufLen;

			/* Copy the mBuf into the flat buffer */
         /*offset fix */
			if (SCpyMsgFix(datReq->mBuf[tbIndex], (MsgLen)0, (MsgLen)bufLen,
					(Data*) (txSduReq + 1), (MsgLen*)&cLen) != ROK)
			{
	    		   YS_MS_FREE(txSduReq, phyTxSdu->sduLen);
			   RETVALUE(RFAILED);
			}
#ifdef MSPD_MLOG_NEW
			MLogTask(PID_CL_PDSCH_DAT_REQ, RESOURCE_LARM, t, GetTIMETICK());
#endif
			break;

		case PBCH:
#ifdef MSPD_MLOG_NEW
			t = GetTIMETICK();
#endif
			ret = SFndLenMsg(bchDatReq->bchDat.val, &bufLen);

			if (ret != ROK)
			{
				RETVALUE(RFAILED);
			}

         /*Length of MIB should be 24 bits, i.e. 3 bytes. Since word size is 
          * 4 bytes
          * however bufLen is set to 4, But we can set bufLen as 3 here 
          * to copy just 3 octets into pTxSdu.*/
			bufLen -= 1;

         /*For PBCH channel these parameters are not relevant, 
           hence setting them to 0.*/
			txSduReq->nackAck = 0;
			txSduReq->uciFormat = 0;

			txSduReq->msgLen = bufLen;
			txSduReq->maxBitsperCw = (bufLen * 8);
			phyTxSdu->sduLen = sizeof(TXSDUREQ) + bufLen;

			/*-- Copy the mBuf into the flat buffer --*/
         /*Offset fix*/
			if (SCpyMsgFix(bchDatReq->bchDat.val, (MsgLen)0, (MsgLen)bufLen,
						(Data*) (txSduReq + 1), (MsgLen*)&cLen) != ROK)
				{
					YS_MS_FREE(txSduReq, phyTxSdu->sduLen);
					RETVALUE(RFAILED);
				}
#ifdef MSPD_MLOG_NEW
			MLogTask(PID_CL_PBCH_DAT_REQ, RESOURCE_LARM, t, GetTIMETICK());
#endif
			break;

		default:
			break;
	}
   txSduReq->chanId = channelId;
   txSduReq->msgType = PHY_TXSDU_REQ;
   txSduReq->phyEntityId = YS_PHY_INST_ID;
   txSduReq->channelType = channelType;
   txSduReq->cwId = 0; /* Overridden by caller if needed (for mimo) */

   phyTxSdu->txSduReq = pMsgDesc;
#ifdef MSPD_MLOG_NEW
      MLogTask(PID_CL_FILL_TXSDU , RESOURCE_LARM, startTick, GetTIMETICK());
#endif
   RETVALUE(ROK);
} /* end of ysMsUtlFillTxSdu */

/* HARQ_UL: Function to form HI and DCI0 SDUs */
/*
*
*       Fun:   ysMsUtlFillDci0HiSdu
*
*       Desc:
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsUtlFillDci0HiSdu
(
YsPhyTxSdu     *phyTxSdu,
TfuPhichInfo  *phichInfo,
TfuPdcchInfo  *pdcchInfo,
U8            channelType,
U16           channelId,
YsCellCb      *cellCb
)
#else
PUBLIC S16 ysMsUtlFillDci0HiSdu(YsPhyTxSdu * phyTxSdu,
      TfuPhichInfo * phichInfo,TfuPdcchInfo * pdcchInfo,U8 channelType,
      U16 channelId,YsCellCb * cellCb)( phyTxSdu, phichInfo, pdcchInfo, 
         channelType, channelId, cellCb)
YsPhyTxSdu     *phyTxSdu;
TfuPhichInfo  *phichInfo;
TfuPdcchInfo  *pdcchInfo;
U8            channelType;
U16           channelId;
YsCellCb      *cellCb;
#endif
{

   PGENMSGDESC pMsgDesc;
   PHIINFOMSGDESC  hiSdu;
   PDCIULSDUMSG    dci0Sdu;


   TRC2(ysMsUtlFillDci0HiSdu)

   pMsgDesc = (PGENMSGDESC)ysMsUtlAllocSvsrMsg(TRUE);
   if (pMsgDesc == NULLP)
   {
      YS_DBG_ERR((_ysp, "Failed to allocate memory for DCI0HISDU"));
      MSPD_ERR("Failed to allocate memory for DCI0HISDU");
      RETVALUE (RFAILED);
   }

   switch(channelType)
	{
		case PHICH:
		{
			hiSdu= (PHIINFOMSGDESC)(pMsgDesc);
			if (phichInfo)
			{

				U8     nGPhich;
				U8     sixNg = cellCb->cellInfo.phichSixNg;
				U8     seqIdx;
				U8     twoNSfPhich;
                U8     bw = cellCb->cellInfo.dlBw;

				/* HARQ_UL: Removed the incorrect assigment of phichSeqIndex*/

				if(CTF_PHICH_DUR_NORMAL == cellCb->cellCfg.phichCfg.duration)
				{
					nGPhich = ((sixNg * ((bw + 7)/ 8) + 5)/6);
					twoNSfPhich = 8;
				}
				else
				{
					nGPhich = 2 * ((sixNg * ((bw + 7)/ 8) + 5)/6);
				    twoNSfPhich = 4;
				}
               seqIdx = ((phichInfo->rbStart/nGPhich) + phichInfo->nDmrs) % 
                  (twoNSfPhich);

	         hiSdu->channelId = channelId;
				hiSdu->msgType = PHY_TXHISDU_REQ;
            hiSdu->msgLength = sizeof(HIINFOMSGDESC);
            hiSdu->nackAck = phichInfo->isAck;
               hiSdu->phichGroupNumber = 
                  (phichInfo->rbStart + phichInfo->nDmrs) % nGPhich;
            hiSdu->phichSequenceIndex = seqIdx;
				hiSdu->transmissionMode = cellCb->cellInfo.cmnTransMode;
				hiSdu->txPowerControl = 0 /* HARQ_TODO */;
            hiSdu->phyEntityId = YS_PHY_INST_ID;
			}
         break;
	   }

		case PDCCH:
			switch(pdcchInfo->dci.dciFormat)
			{
				case TFU_DCI_FORMAT_0:
					{
						U16 bW, bits, idx = 0;
						U16 bitIdx = 0 ;
						U16          riv;
                  U8         dci[8];
                  U16        dciSize;
                  U8         *pDci = dci;
						TfuDciFormat0Info *format0 = &pdcchInfo->dci.u.format0Info;
						dci0Sdu= (PDCIULSDUMSG)(pMsgDesc);


						bW = YS_MS_GET_UL_BW(cellCb->cellCfg.bwCfg);

						/*-- Calculate resource block assignment bits need to be set
						  Which is ln(N(N+1)/2) 36.212 5.3.3.1 --*/
						bits = (bW * (bW + 1) / 2);
						while ((bits & 0x8000) == 0)
						{
							bits <<= 1;
							idx++;
						}
						bits = 16 - idx;

						cmMemset(pDci, 0, 8);
						YS_MS_SETBITRANGE(pDci, bitIdx, 1, 0); bitIdx++; /*-- Format 0 --*/
						YS_MS_SETBITRANGE(pDci, bitIdx, 1, format0->hoppingEnbld);
						bitIdx++;
						riv = ysMsCalcRiv(bW, format0->rbStart, format0->numRb);
						if (format0->hoppingEnbld)
						{
							/*-- Resource block assignment bits need to be set
							  Which is ln(N(N+1)/2) --*/
                     YS_MS_SETBITRANGE(pDci, bitIdx, format0->hoppingBits, 0);  
							bitIdx += format0->hoppingBits;
                     YS_MS_SETBITRANGE(pDci, bitIdx, 
                     (bits - format0->hoppingBits), 0); 
							bitIdx += (bits - format0->hoppingBits);
						}
						else
						{
                     YS_MS_SETBITRANGE(pDci, bitIdx, bits, riv); 
							bitIdx += (bits);
						}


						YS_MS_SETBITRANGE(pDci, bitIdx, 5, format0->mcs);
						bitIdx += 5;
						YS_MS_SETBITRANGE(pDci, bitIdx, 1, format0->ndi);

                  YS_DBG_FATAL((_ysp, "\n ysMsUtlFillDCIChanDesc filling \
                           PDCCH format 0 at : (%d,%d) ndi (%d) \n",
                           cellCb->timingInfo.sfn, 
                           cellCb->timingInfo.subframe, format0->ndi));

						bitIdx++;
						YS_MS_SETBITRANGE(pDci, bitIdx, 2, format0->tpcCmd);
						bitIdx += 2;
						YS_MS_SETBITRANGE(pDci, bitIdx, 3, format0->nDmrs);
						bitIdx += 3;
						YS_MS_SETBITRANGE(pDci, bitIdx, 1, format0->cqiReq);
						bitIdx++;
						dciSize = (bitIdx / 8) + ((bitIdx % 8)?1:0);
                  /*fix for decoding on WJ.*/
						/* Adding two bits for format 0 to make it equat to size for
						 * format 1, i.e. 27 bits */
						dci0Sdu->channelId = channelId;
                  dci0Sdu->msgType = PHY_TXDCIULSDU_REQ;
                  dci0Sdu->msgLength = sizeof(DCIULSDUMSG);
						dci0Sdu->numBitsDciUL = bitIdx+2;
					   dci0Sdu->phyEntityId = YS_PHY_INST_ID;

                  switch(pdcchInfo->aggrLvl)
         			{
         			   case CM_LTE_AGGR_LVL1:
         					dci0Sdu->numCCE = 1 ;
         					break;
         				case CM_LTE_AGGR_LVL2:
         					dci0Sdu->numCCE = 2;
         					break;
         				case CM_LTE_AGGR_LVL4:
         					dci0Sdu->numCCE = 4;
         					break;
         				case CM_LTE_AGGR_LVL8:
         					dci0Sdu->numCCE = 8;
         					break;
         				default:
         					break;
         			}

						dci0Sdu->rnti = pdcchInfo->rnti;
						dci0Sdu->startCCE = pdcchInfo->nCce;
						dci0Sdu->numPayloadBytes = 0; /* HARQ_TODO: Chk if needed*/
						dci0Sdu->transmissionMode = cellCb->cellInfo.cmnTransMode;
      				dci0Sdu->txPowerControl = 0 /* HARQ_TODO */;
						/* HARQ_TODO: Correct after APIs are frozen */

                  cmMemcpy((U8 *)((U8 *)dci0Sdu + sizeof(DCIULSDUMSG)), pDci,
                        dciSize);

						break;
					}

				default:
					YS_DBG_ERR((_ysp, "Unknown DCI format"));
					RETVALUE(RFAILED);
			}
			break;

		default:
			break;
	}


   phyTxSdu->txSduReq = pMsgDesc;

   RETVALUE(ROK);
} /* end of ysMsUtlFillDci0HiSdu */


/*
*
*       Fun:   ysMsUtlEmptyLst
*
*       Desc:  This function is used to empty and free a given linked list
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsUtlEmptyLst
(
CmLListCp *lst
)
#else
PUBLIC S16 ysMsUtlEmptyLst (lst)
CmLListCp *lst;
#endif
{

   CmLList *cmLstEnt;
   YsPhyTxSdu *phyTxSdu;


   TRC2(ysMsUtlEmptyLst)

   cmLstEnt = lst->first;

   while (cmLstEnt)
   {

      phyTxSdu = (YsPhyTxSdu*)cmLstEnt->node;
      cmLstEnt = cmLstEnt->next;
      cmLListDelFrm(lst, &phyTxSdu->lnk);
      /* TODO: need to free the TXSDU also */
      YS_MS_FREE(phyTxSdu, sizeof(YsPhyTxSdu));
   }


   RETVALUE (ROK);

}

/*
*
*       Fun:   ysMsUtlFillStartReq
*
*       Desc:  This primitive is used to fill start request
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsUtlFillStartReq
(
PSTARTREQ      startReq,
YsCellCb       *cellCb
)
#else
PUBLIC S16 ysMsUtlFillStartReq (startReq, cellCb)
PSTARTREQ      startReq;
YsCellCb       *cellCb;
#endif
{
   TRC2(ysMsUtlFillStartReq)
   if (startReq == NULLP || cellCb == NULLP)
   {
      YS_LOGERR_DBG(EYSXXX, 0, "ysMsUtlFillStartReq(): Invalid ptr\n");
      RETVALUE (RFAILED);
   }
   uart_printf("PHY Configuration opMode=%d, counter=%d, period=%d\n", 
         cellCb->cellCfg.opMode, cellCb->cellCfg.counter, 
         cellCb->cellCfg.period);

   /* Configuring PHY from configuration file */
   startReq->mode        = cellCb->cellCfg.opMode;
   startReq->msgType     = PHY_START_REQ;
   startReq->phyEntityId = YS_PHY_INST_ID;
   startReq->count       = cellCb->cellCfg.counter;
   startReq->period      = cellCb->cellCfg.period;

   RETVALUE (ROK);
} /* ysMsUtlFillStartReq */

/*
*
*       Fun:   ysMsUtlFillStopReq
*
*       Desc:  This primitive is used to fill stop request
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsUtlFillStopReq
(
PGENMSGDESC    stopReq,
YsCellCb       *cellCb
)
#else
PUBLIC S16 ysMsUtlFillStopReq (stopReq, cellCb)
PGENMSGDESC    stopReq;
YsCellCb       *cellCb;
#endif
{
   TRC2(ysMsUtlFillStopReq)
   if (stopReq == NULLP || cellCb == NULLP)
   {
      YS_LOGERR_DBG(EYSXXX, 0, "ysMsUtlFillStopReq(): Invalid ptr\n");
      RETVALUE (RFAILED);
   }

   stopReq->msgSpecific    = 0; /* Need to find what to fill */
   stopReq->msgType        = PHY_STOP_REQ;
   stopReq->phyEntityId    = YS_PHY_INST_ID;

   RETVALUE (ROK);
} /* ysMsUtlFillStopReq */


/* HARQ */
/*
*
*       Fun:   ysMsUtlAllocSvsrMsg
*
*       Desc:  This primitive is used to allocate svsrMsg
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PUBLIC PGENMSGDESC ysMsUtlAllocSvsrMsg
(
Bool isSdu
)
#else
PUBLIC PGENMSGDESC ysMsUtlAllocSvsrMsg(isSdu)
#endif
{
  PGENMSGDESC msg;
  volatile int t = GetTIMETICK();
  if (!isSdu)
  {
     msg = (PGENMSGDESC)SvsrAllocMsgEx(YS_MS_MAX_SDU_SZ);
     if (msg == NULLP)
     {
        YS_DBG_ERR((_ysp,"Svsr FAILED Invalid memory %x...Exiting...\n", msg));
        stop_printf("Svsr FAILED Invalid memory %x...Exiting...\n", msg);
	    exit(-1);
      }
#ifdef MSPD_MLOG_NEW
	  MLogTask(PID_SVSRALLOC_MSG, RESOURCE_LARM, t, GetTIMETICK());
#endif
      RETVALUE(msg);
  }
  if ((msg = (PGENMSGDESC)SvsrAllocMsg()) != NULLP)
  {
#ifdef MSPD_MLOG_NEW
	  MLogTask(PID_SVSRALLOC_MSG, RESOURCE_LARM, t, GetTIMETICK());
#endif
      RETVALUE(msg);
  }

   YS_DBG_ERR((_ysp,"Svsr FAILED......Exiting......\n"));
   stop_printf("Svsr FAILED......Exiting......\n");
   exit(-1);
}

/*
*
*       Fun:   ysMsUtlGetPhyListElem
*
*       Desc:  This primitive is used to allocate and get a PHY list element
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PUBLIC PMAC2PHY_QUEUE_EL ysMsUtlGetPhyListElem
(
)
#else
PUBLIC PMAC2PHY_QUEUE_EL ysMsUtlGetPhyListElem()
#endif
{
   PGENMSGDESC 	  pMsgDesc;
   PMAC2PHY_QUEUE_EL pElem = NULLP;

   /* Allocate a message towards Phy using sysCore */
   pMsgDesc = (PGENMSGDESC) ysMsUtlAllocSvsrMsg (FALSE);
   if (pMsgDesc == NULLP)
   {
      YS_DBG_ERR((_ysp, "Unable to allocate memory for PhyListElem"));
      RETVALUE (NULLP);
   }
   pElem = (PMAC2PHY_QUEUE_EL)(pMsgDesc);

   RETVALUE(pElem);

}

/* HARQ */
/*
*
*       Fun:   ysMsUtlAddToPhyList
*
 *       Desc:  This primitive is used to add an element to the beginning 
 *              of the linked list
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsUtlAddToPhyList
(
YsPhyLstCp        *lstCp,
PMAC2PHY_QUEUE_EL lstElem
)
#else
PUBLIC S16 ysMsUtlAddToPhyList(lstCp, lstElem)
YsPhyLstCp        *lstCp;
PMAC2PHY_QUEUE_EL lstElem;
#endif
{
  PMAC2PHY_QUEUE_EL lstStart = lstCp->head;
  lstCp->count++;
  if (!lstStart)
  {
     lstCp->head = lstCp->tail = lstElem;
	 RETVALUE(ROK);
  }

   lstElem->Next = lstStart;
   /* lstStart->Next = lstElem; */
   lstCp->head = lstElem;

   RETVALUE(ROK);

}


/*
*
*       Fun:   ysMsUtlCatPhyList
*
 *       Desc:  This primitive is used to concatenate two lists. 
 *              It concatenates in order L1L2
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsUtlCatPhyList
(
YsPhyLstCp        *lstCp1,
YsPhyLstCp        *lstCp2
)
#else
PUBLIC S16 ysMsUtlCatPhyList(lstCp1, lstCp2)
YsPhyLstCp        *lstCp1;
YsPhyLstCp        *lstCp2;
#endif
{
   PMAC2PHY_QUEUE_EL lstTail1 = lstCp1->tail;
   PMAC2PHY_QUEUE_EL lstHead2 = lstCp2->head;

   if (lstTail1)
   {
      lstTail1->Next = lstHead2;
      lstCp1->tail = lstCp2->tail;
      lstCp1->count += lstCp2->count;
   }
   else
   {
      *lstCp1 = *lstCp2;
   }
   RETVALUE(ROK);

}


/* HARQ */
/*
*
*       Fun:   ysMsUtlInitPhyList
*
*       Desc:  This primitive is used to initialize the lists
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsUtlInitPhyList
(
YsPhyLstCp        *lstCp
)
#else
PUBLIC S16 ysMsUtlInitPhyList(lstCp)
YsPhyLstCp        *lstCp;
#endif
{

 lstCp->count = 0;
 lstCp->head = NULLP;
 lstCp->tail = NULLP;
 RETVALUE(ROK);

}

/*
*
*       Fun:   ysMsUtlFreePhyList
*
*       Desc:  This primitive is used to free the list
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_utl.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsUtlFreePhyList
(
YsPhyLstCp        *lstCp
)
#else
PUBLIC S16 ysMsUtlFreePhyList(lstCp)
YsPhyLstCp        *lstCp;
#endif
{
 PMAC2PHY_QUEUE_EL tmp = lstCp->head;
 PMAC2PHY_QUEUE_EL next = NULLP;

 while (tmp)
 {
   next = tmp->Next;
   SvsrFreeMsg(tmp->MessagePtr);
   SvsrFreeMsg(tmp);
   tmp = next;
 }

 lstCp->count = 0;
 lstCp->head = NULLP;
 lstCp->tail = NULLP;
 RETVALUE(ROK);

}

/*
 * *
 * *       Fun:   ysSendCrcToMac
 * *
 * *       Desc:
 * *
 * *       Ret:
 * *
 * *       Notes: None
 * *
 * *       File:  ys_ms_utl.c
 * *
 * */
#ifdef ANSI
PUBLIC Void ysSendCrcToMac
(
 YsCellCb          *cellCb,
 TfuCrcIndInfo     *crcIndInfo
 )
#else
PUBLIC Void ysSendCrcToMac(cellCb, crcIndInfo)
   YsCellCb          *cellCb;
   TfuCrcIndInfo     *crcIndInfo;
#endif
{
   U8 dlCntrlsubframe = 0;
   YsUiTfuCrcInd(&cellCb->schTfuSap->sapPst, cellCb->schTfuSap->suId,
         crcIndInfo);
   /* Check whether we are expecting DatReq for (currenttime + TFU_DELTA)th 
    * subframe
    * or not. If not
    * 1. Append the UlCntrlReq elements to PHY message List.
    * 2. Then send the TX&RX vector down
    * else
    * Do nothing*/
   dlCntrlsubframe =
	   (cellCb->timingInfo.subframe + TFU_DELTA) % YS_NUM_SUB_FRAMES;
   if (FALSE == cellCb->dlEncL1Msgs[dlCntrlsubframe].isdatReqPres)
   {
        ysMsDlmSndVectSDU(cellCb);
   }

   return;
}

PUBLIC Void rsysStopHndlr()
{
   U32  memavail;
   extern SysStayInfo syscoreinfo [4];
   U8 coreId = 1;

   MSPD_DBG("PHY stopped, showing memory info\n");
   SRegInfoShow(0, &memavail);
   for (;coreId <= 3; ++coreId)
   {
      MSPD_DBG("syscoreinfo[%u].mBuf 0x%x\n", coreId, syscoreinfo[coreId].mBuf);
      MSPD_DBG("syscoreinfo[%u].event %u\n", coreId,syscoreinfo[coreId].event);
      MSPD_DBG("syscoreinfo[%u].dstEnt %u 0x%x\n",coreId,syscoreinfo[coreId].dstEnt, syscoreinfo[coreId].dstEnt);
      MSPD_DBG("syscoreinfo[%u].dstInst %u\n",coreId,syscoreinfo[coreId].dstInst);
      MSPD_DBG("syscoreinfo[%u].srcEnt %u 0x%x\n",coreId,syscoreinfo[coreId].srcEnt, syscoreinfo[coreId].srcEnt);
      MSPD_DBG("syscoreinfo[%u].timestamp 0x%x\n",coreId, syscoreinfo[coreId].timestamp);
      MSPD_DBG("syscoreinfo[%u].caller[0] 0x%x\n",coreId, syscoreinfo[coreId].caller[0]);
	  MSPD_DBG("syscoreinfo[%u].caller[1] 0x%x\n",coreId, syscoreinfo[coreId].caller[1]);
	  MSPD_DBG("syscoreinfo[%u].caller[2] 0x%x\n",coreId, syscoreinfo[coreId].caller[2]);
	  MSPD_DBG("syscoreinfo[%u].caller[3] 0x%x\n",coreId, syscoreinfo[coreId].caller[3]);
      MSPD_DBG("syscoreinfo[%u].res[0] %u\n",coreId, syscoreinfo[coreId].res[0]); /* 1 is for Icore msg */
      MSPD_DBG("syscoreinfo[%u].res[1] %u\n",coreId,syscoreinfo[coreId].res[1]); /* 1 is for start of the task */
   }
}

#ifndef YS_MS_NO_TA
/*
 *       Fun:   ysMsUtlIncrTime
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
PUBLIC Void ysMsUtlIncrTime
(
 CmLteTimingInfo   *t,
 U32                delta
 )
#else
PUBLIC Void ysMsUtlIncrTime(t, delta)
 CmLteTimingInfo   *t;
 U32                delta;
#endif
{
   U32   sfnincr = delta / 10;
   U32   sfincr  = delta % 10;
   U32   sf      = (U32)t->subframe;

   if ((sf = sf + sfincr) > 10)
   {
      sf -= 10;
      ++sfnincr;
   }
   t->subframe = sf;
   t->sfn = (t->sfn + sfnincr) % 1024;
   return;
}


/*
 *       Fun:   ysMsUtlAddToTaLst
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
PUBLIC Void ysMsUtlAddToTaLst
(
 YsCellCb          *cellCb,
 YsUeCb            *ueCb
)
#else
PUBLIC Void ysMsUtlAddToTaLst(cellCb)
   YsCellCb          *cellCb;
   YsUeCb            *ueCb;
#endif
{
   CmLListCp  *lst = &cellCb->taUeLst;

   /* Add to head */
   lst->crnt = lst->first;
   ueCb->tarptInfo.lnk.node = (PTR)ueCb;
   cmLListInsCrnt(lst, &ueCb->tarptInfo.lnk);
   return;
}


/*
 *       Fun:   ysMsUtlTmrProc
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
PUBLIC Void ysMsUtlTmrProc
(
 YsCellCb          *cellCb
 )
#else
PUBLIC Void ysMsUtlTmrProc(cellCb)
   YsCellCb          *cellCb;
#endif
{
   CmLListCp  *lst = &cellCb->taUeLst;
   CmLList    *n   = lst->first;
   while (n)
   {
      YsUeCb    *ue = (YsUeCb*)(n->node);
      n = n->next;
      if (YS_TIMING_INFO_SAME(ue->tarptInfo.throttleExp, cellCb->timingInfo))
      {
         cmLListDelFrm(lst, &ue->tarptInfo.lnk);
         ue->tarptInfo.lnk.node = NULL;
      }
   }
   return;
}
#endif

PUBLIC Void ysMsMapRntiToChan(YsCellCb * cellCb, CmLteRnti rnti, U8 subframe, 
U16 channelId)
{
   cellCb->chan2Rnti[subframe][channelId].rnti = rnti;

   RETVOID;
}

PUBLIC Void ysMsGetRntiFrmChanId(YsCellCb * cellCb, CmLteRnti *rnti, 
      U8 subframe, U16 channelId)
{  
   *rnti = cellCb->chan2Rnti[subframe][channelId].rnti;

   RETVOID;
}


PUBLIC S16 ysMsSndMissedCrcInd(YsCellCb * cellCb)
{
   CmLteTimingInfo rxSduSf = {0};
   CmLteTimingInfo timingInfo = {0};
   S16 ret = 0;

   timingInfo = cellCb->timingInfo;
     
   rxSduSf.subframe = timingInfo.subframe ? (timingInfo.subframe - 1): 9;

   if (!cellCb->crcSent[rxSduSf.subframe])
   {
      TfuCrcIndInfo     *crcIndInfo = NULLP;
      /* Send the CRC Indication to MAC for the corresponding subframe */
      ret = ysUtlAllocEventMem((Ptr *)&crcIndInfo,
      sizeof(TfuCrcIndInfo));
      if(ret == RFAILED)
      {
         YS_DBG_ERR((_ysp, "ysMsSndMissedCrcInd(): Memory allocation failed for\
	    CrcInd\n"));
         MSPD_ERR("Memory allocation failed for CrcInd\n");
         RETVALUE(RFAILED);
      } /* end of if statement */

      cmLListInit(&(crcIndInfo->crcLst));
      crcIndInfo->cellId = ysCb.cellId;
      YS_MS_DECR_TIME(timingInfo, rxSduSf, 1);
      crcIndInfo->timingInfo.sfn = rxSduSf.sfn;
      crcIndInfo->timingInfo.subframe = rxSduSf.subframe;

      MSPD_DBG("Sending missed CRC for the previous TTI (%d,%d) at (%d, %d)\n",\
         crcIndInfo->timingInfo.sfn,crcIndInfo->timingInfo.subframe , \
	     cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
		 
      ysSendCrcToMac(cellCb, crcIndInfo);
      cellCb->crcSent[rxSduSf.subframe] = TRUE;
      cellCb->isCrcExptd[rxSduSf.subframe] = FALSE;     
   }
   RETVALUE(ROK);
}
/********************************************************************30**

         End of file:

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
/main/1      ---      rp   1. initial release.
/main/1    ys004.102  vr   1. Merged MSPD code with phy 1.7
*********************************************************************91*/

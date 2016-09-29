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

     Name:     LTE-MAC Convergence Layer

     Type:     C souce file

     Desc:     CL for Trillium MAC to PHY TX

     File:     ys_ms_dl.c

     Sid:

     Prg:      rp

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
#include "ctf.h"           /*-- CTF interface header file --*/
#include "tfu.h"           /*-- TFU interface header file --*/
#include "lys.h"           /*-- LYS interface header file --*/
#include "ys_ms_err.h"        /* CL error header file */
#include "ys_ms.h"         /* CL header file */
#include "ss_diag.h"        /* Common log file */

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
#include "ctf.x"           /*-- CTF interface header file --*/
#include "tfu.x"           /*-- TFU interface header file --*/
#include "lys.x"           /*-- LYS interface header file --*/

/* Silicon Includes */
#include "LtePhyL2Api.h"
#include "apidefs.h"

/* Global variables for HARQ_STATICS logs */
#ifdef HARQ_STATISTICS
U32 ulAcks, ulNacks, dlAcks, dlNacks, totl_ulTbs, totl_dlTbs, totl_ulAcks, \
	    totl_ulNacks, totl_dlAcks, totl_dlNacks, totlUlTb, totlDlTb, Secs;
#endif
U32 startTti;
U32 robLogs[10];
PUBLIC U32 numeTti = 0;
/*TODO:including this file to expose HANDLE for temp debugging,
 * remove this once done.*/
#include "4gmx_types.h"

#include "ys_ms.x"         /*-- CL header file --*/
#include "appinit.h"


#define STAT_PERIOD  1
#define STAT_THRESH  1
#define to_kb(x) (((x)*8)/(1000*STAT_PERIOD))

/*variable introduced for temp testing */
/*ys004.102 :  Merged MSPD code with phy 1.7 */

volatile U32 __align(32) prev_sf = 30;
volatile U32 __align(32) prev_sfn = 1024;

#ifdef YS_MIB_WARND
Bool  acceptPrimsFrmMac = FALSE;
#endif

/*TODO:exposing this varialbel to this file for temp debugging...
 * need to remove it later*/
extern HANDLE  SvsrMsgPart;
extern int delayedHarq;
#ifdef CCPU_MLOG
PUBLIC Void L2PrintFunc (U8		*str);
#endif

U32		dlPacketCounter = 0;

#ifdef YS_PHY_STOP_AUTO
extern S16 ysMsSmStRunEvtStop ARGS((YsCellCb *cellCb, Void *status));
#endif

extern S16 DiagDumpPhyApi(PTR vector);
#ifdef RUN_ICPU_ONLY
PUBLIC Void genUARMMsg (Bool fromUARM );
#endif
PUBLIC S16 ysMsDlmSndVectSDU (
YsCellCb        *cellCb);
PRIVATE BOOL ysMsDlmIsDataPres ARGS((
TfuCntrlReqInfo *cntrlReq
));

U32 DelyedCrcInd;
#ifndef YS_MS_NO_TA
PUBLIC Void ysMsUtlIncrTime ARGS ((
CmLteTimingInfo   *t,
U32                delta
));
PUBLIC Void ysMsUtlAddToTaLst ARGS ((
YsCellCb          *cellCb,
YsUeCb            *ueCb
));
#endif

/*
*
*       Fun:   ysMsDlmPrcDatReq
*
*       Desc:  Dat request handler
*
*       Ret:   ROK/RFAILED
*
*       Notes: None
*
*       File:  ys_ms_dl.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsDlmPrcDatReq
(
YsCellCb        *cellCb,
TfuDatReqInfo   *datReq
)
#else
PUBLIC S16 ysMsDlmPrcDatReq(cellCb, datReq)
YsCellCb        *cellCb;
TfuDatReqInfo   *datReq;
#endif
{
   PGENMSGDESC      pMsgDesc = NULLP;
   DLSUBFRDESC      *txVect;
   TfuDatReqPduInfo *pduInfo;
   CmLteTimingInfo  ulCntrlTiming;
   YsPhyLstCp *pUlCntrl = NULLP;

   CmLList          *cmLstEnt;
   S16              ret = ROK;
   U8               idx = 0;
   U8               chIdx = 0;
   /* HARQ */
   YsPhyLstCp        *pDlData;
   YsPhyLstCp        *pUlDlLst;
   PMAC2PHY_QUEUE_EL pElem;
   U8                sfNum;
   YsPhyTxSdu       phyTxSdu;
   CmLteTimingInfo  timingInfo;
   Bool cwSwapFlag  = FALSE;
   U8 tbCount ;
   U8 tbIdx = 0;
   YsUeCb      *ueCb = NULL;
   double timeDiff = 0;
   U32    ueCnt = 0;
   TRC2(ysMsDlmPrcDatReq)

   /* Dropping the previous TTI's DatReq and the delayed DatReq for the current TTI */
	 
   timeDiff = YS_MS_TTI_DIFF(GetTIMETICK(),startTti);	
   if (((prev_sf == cellCb->timingInfo.subframe) &&
			   (prev_sfn == cellCb->timingInfo.sfn))) 
   {
       MSPD_LOG("Dropping duplicate DatReq(%d,%d)\n",
                 datReq->timingInfo.sfn, datReq->timingInfo.subframe);
       RETVALUE(ROK);
   }
   else if(timeDiff > YS_MAX_DAT_REQ_DELAY)
   {
       MSPD_LOG("Dropping the Delayed DatReq (Processed TTI time: %f)\n",timeDiff);
       robLogs[5]++;
       RETVALUE(ROK);
   }

   timingInfo = datReq->timingInfo;
   sfNum = datReq->timingInfo.subframe;
   pDlData = &cellCb->dlEncL1Msgs[sfNum].dlData;


   /* dlMsgLst = &(cellCb->dlEncL1Msgs[sfNum].txSduLst); */

   /* at this point it is possible that Cntrl Req has arrived and
    * we have already allocated the txVector - as it is loosely coupled it
    * is also possible that CntrlReq hasn't arrived yet - so we should
    * check and allocated
    */
   pMsgDesc = cellCb->dlEncL1Msgs[sfNum].txVector;

   if (NULLP == pMsgDesc)
   {
   	/* Allocate a message towards Phy using sysCore */
   	pMsgDesc = (PGENMSGDESC) ysMsUtlAllocSvsrMsg (FALSE);
   	if (pMsgDesc == NULLP)
   	{
   		uart_printf("Unable to allocate memory for TxVector\n");
   		YS_DBG_ERR((_ysp, "Unable to allocate memory for TxVector"));
   		MSPD_ERR("Unable to allocate memory for TxVector");
   		RETVALUE (RFAILED);
   	}
   	cellCb->dlEncL1Msgs[sfNum].txVector = pMsgDesc;
   	pMsgDesc->msgType     = PHY_TXSTART_REQ;
   	pMsgDesc->phyEntityId = YS_PHY_INST_ID;
   	pMsgDesc->msgSpecific = sizeof(DLSUBFRDESC);
   	txVect   = (PDLSUBFRDESC)(pMsgDesc + 1);
   	txVect->numberofChannelDescriptors = 0;
   	txVect->numberOfPhichChannels = 0;
      /* TODO:Setting this for the time being as per Vitaliy input.
         Can be reverted at later stage */
   	txVect->dlCh[0].offsetNextCh = 0;
      /* Add message to the linked list of PHY messages */

      pElem = ysMsUtlGetPhyListElem();
      if (!pElem)
      {
         /* HARQ_DBG */
         uart_printf("ysMsUtlAllocSvsrMsg failed for PHY list element for TXVECTOR\
               in DatReq at time(%d,%d)\n", cellCb->timingInfo.sfn,
               cellCb->timingInfo.subframe);
         RETVALUE(RFAILED);
      }
      YS_MS_FILL_PHY_LIST_ELEM(pElem, txVect->frameNumber, txVect->subframeNumber,
   		  pMsgDesc, sizeof(GENMSGDESC) + sizeof(DLSUBFRDESC), PHY_TXSTART_REQ);
      ysMsUtlAddToPhyList(pDlData,pElem);
   }


   txVect   = (PDLSUBFRDESC)(pMsgDesc + 1);
   chIdx = txVect->numberofChannelDescriptors;

   idx = txVect->numberOfPhichChannels;



   /* Process BCH data */
   if (datReq->bchDat.pres)
   {

      static U32 bchCnt = 0;
      /* Fill the TXSDU for PBCH */
      if (ysMsUtlFillTxSdu(&phyTxSdu, datReq, NULLP,
                           NULLP, NULLP, PBCH, chIdx, cellCb,0) != ROK)
      {
         uart_printf("Failed to build PBCH TXSDU\n");
         YS_DBG_ERR((_ysp, "Failed to build PBCH TXSDU"));
         RETVALUE(RFAILED);
      }

      /* Fill the channel descriptor for PBCH */
      ysMsUtlFillDlChanDesc(NULLP, NULLP, cellCb, &txVect->dlCh[idx], PBCH,
            chIdx, timingInfo);

      txVect->dlCh[idx].offsetNextCh = 0;
      chIdx++;
      idx++;

       /* Add DL SDUs to the linked list of PHY messages */
       pElem = ysMsUtlGetPhyListElem();
       if (!pElem)
       {
          /* HARQ_DBG :TODO: need to free the allocated messages */
          uart_printf("ysMsUtlAllocSvsrMsg failed for PHY list element\
          for TXSDU for BCH at time(%d,%d))\n",
          cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
      	 RETVALUE(RFAILED);
       }
       YS_MS_FILL_PHY_LIST_ELEM(pElem,timingInfo.sfn,timingInfo.subframe,
      				  phyTxSdu.txSduReq, (sizeof(TXSDUREQ)),PHY_TXSDU_REQ);
       ysMsUtlAddToPhyList(pDlData,pElem);

       phyTxSdu.txSduReq = NULLP;
       phyTxSdu.sduLen = 0;
      /* fix - need to increment the number of channel descriptors
            * for each channel added */
      txVect->numberofChannelDescriptors++;
      if(bchCnt % 1000 == 0)
         MSPD_DBG("===== MIB Bch Message Send out in Frame (%d:%d) current time \
               (%d:%d)=========\n",timingInfo.sfn,timingInfo.subframe,
               cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
      bchCnt++;
   }


   /* Process PDSCH data */
   cmLstEnt = datReq->pdus.first;

   while (cmLstEnt != NULLP)
   {
      pduInfo = (TfuDatReqPduInfo*)cmLstEnt->node;
     if (pduInfo->rnti != YS_SI_RNTI)
     {
#ifdef HARQ_STATISTICS
	   totl_dlTbs += pduInfo->nmbOfTBs;	  
#endif
       ++ueCnt;	   
     }

      ueCb = ysMsCfgGetUe(cellCb, pduInfo->rnti);
      if (pduInfo->rnti != YS_SI_RNTI && !ueCb)
      {
         YS_DBG_ERR((_ysp, "ysMsDlmPrcDatReq(): UE doesnt exist\n"));
         MSPD_DBG("UE doesnt exist (%u)\n", pduInfo->rnti);
      }
#ifndef YS_MS_NO_TA
      if (pduInfo->isTApres && ueCb)
      {
         ueCb->tarptInfo.throttleExp.sfn    = datReq->timingInfo.sfn;
         ueCb->tarptInfo.throttleExp.subframe = datReq->timingInfo.subframe;
         ysMsUtlIncrTime(&ueCb->tarptInfo.throttleExp, 30);
         if (ueCb->tarptInfo.lnk.node == NULLP) /*  MS_FIX for ccpu00123413 */
         {
            ysMsUtlAddToTaLst(cellCb, ueCb);
            /* MSPD_DBG("Transmitting TA at DatReqtime(%d,%d) celltime(%d,%d)\n",
                *datReq->timingInfo.sfn, datReq->timingInfo.subframe,
                *cellCb->timingInfo.sfn, cellCb->timingInfo.subframe); */
         }
      }
#endif
      if(( pduInfo->dciInfo.format == TFU_DCI_FORMAT_2A ) &&
            (1 == pduInfo->nmbOfTBs))
      {
         /* We need to get the correct TB based on which is enabled */
         if ((pduInfo->dciInfo.u.format2AAllocInfo.tbInfo[0].rv == 1) &&
              (pduInfo->dciInfo.u.format2AAllocInfo.tbInfo[0].mcs == 0))
         {
           tbIdx = 1;
           /* MSPD_DBG("Retransmitting 1st TB alone with TM2 mode\n"); */
         }
         else
         {
           tbIdx = 0;
           /* MSPD_DBG("Retransmitting 0th TB alone with TM2 mode\n"); */
         }
         /* Fill in the TX SDU from tbIdx selected */
         if (ysMsUtlFillTxSdu(&phyTxSdu, datReq, pduInfo,
               NULLP, NULLP, PDSCH, chIdx, cellCb, tbIdx) != ROK)
         {
            YS_DBG_ERR((_ysp, "Failed to build PDSCH TXSDU"));
            MSPD_ERR("Failed to build PDSCH TXSDU");
            RETVALUE(RFAILED);
         }
         pElem = ysMsUtlGetPhyListElem();

         if (!pElem)
         {
            /* HARQ_DBG :TODO: need to free the allocated messages */
            uart_printf("ysMsUtlAllocSvsrMsg failed for PHY list element for \
                  TXSDU at cellTime(%d,%d)\n",
            cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
            RETVALUE(RFAILED);
         }

         YS_MS_FILL_PHY_LIST_ELEM(pElem,timingInfo.sfn,timingInfo.subframe,
       	    phyTxSdu.txSduReq, sizeof(TXSDUREQ), PHY_TXSDU_REQ);

         /* Add DL SDUs to the linked list of PHY messages */
         ysMsUtlAddToPhyList(pDlData,pElem);

         phyTxSdu.txSduReq = NULLP;
      }
      else
      {
      for( tbCount = 0 ; tbCount < pduInfo->nmbOfTBs ; tbCount++ )
      {

#ifdef LTEMAC_MIM0
        if (!pduInfo->mBuf[tbCount])
        {
         stop_printf("\npduInfo->mBuf[%d] details:\n\
         pduInfo->rnti : %d \n\
         pduInfo->dciInfo.format : %d \n\
         pduInfo->dciInfo.u.format2AAllocInfo.isAllocType0 : %d\n\
         pduInfo->dciInfo.u.format2AAllocInfo.resAllocMap : %s\n\
         pduInfo->dciInfo.u.format2AAllocInfo.harqProcId : %d\n\
         pduInfo->dciInfo.u.format2AAllocInfo.tbInfo[0].ndi : %d\n\
         pduInfo->dciInfo.u.format2AAllocInfo.tbInfo[0].rv  : %d\n\
         pduInfo->dciInfo.u.format2AAllocInfo.tbInfo[0].mcs : %d\n\
         pduInfo->dciInfo.u.format2AAllocInfo.tbInfo[1].ndi : %d\n\
         pduInfo->dciInfo.u.format2AAllocInfo.tbInfo[1].rv  : %d\n\
         pduInfo->dciInfo.u.format2AAllocInfo.tbInfo[1].mcs : %d\n\
         pduInfo->dciInfo.u.format2AAllocInfo.precoding: %d\n\
         pduInfo->dciInfo.u.format2AAllocInfo.transSwap: %d\n\
         pduInfo->nmbOfTBs : %d \n\
         pduInfo->mBuff[0] : %x \n\
         pduInfo->mBuff[1] : %x \n\
         pduInfo->puschRptUsd : %d \n\
         pduInfo->puschPmiInfo.mode : %d\n\
         pduInfo->transMode : %d \n\
         pduInfo->doa : %d \n ",
         tbCount,pduInfo->rnti,\
                        pduInfo->dciInfo.format,
                     pduInfo->dciInfo.u.format2AAllocInfo.isAllocType0,
                     pduInfo->dciInfo.u.format2AAllocInfo.resAllocMap, \
                        pduInfo->dciInfo.u.format2AAllocInfo.harqProcId,
                     pduInfo->dciInfo.u.format2AAllocInfo.tbInfo[0].ndi,
                     pduInfo->dciInfo.u.format2AAllocInfo.tbInfo[0].rv,\
                        pduInfo->dciInfo.u.format2AAllocInfo.tbInfo[0].mcs,
                     pduInfo->dciInfo.u.format2AAllocInfo.tbInfo[1].ndi,
                     pduInfo->dciInfo.u.format2AAllocInfo.tbInfo[1].rv, \
                        pduInfo->dciInfo.u.format2AAllocInfo.tbInfo[1].mcs,
                     pduInfo->dciInfo.u.format2AAllocInfo.precoding,
                     pduInfo->dciInfo.u.format2AAllocInfo.transSwap,\
                        pduInfo->nmbOfTBs,  pduInfo->mBuf[0], pduInfo->mBuf[1],
                     pduInfo->puschRptUsd, pduInfo->puschPmiInfo.mode, \
         pduInfo->transMode, pduInfo->doa);

        }
#endif
         if (ysMsUtlFillTxSdu(&phyTxSdu, datReq, pduInfo,
               NULLP, NULLP, PDSCH, chIdx, cellCb, tbCount) != ROK)
         {
            YS_DBG_ERR((_ysp, "Failed to build PDSCH TXSDU"));
            MSPD_ERR( "Failed to build PDSCH TXSDU");
            RETVALUE(RFAILED);
         }
	if((pduInfo->rnti == 0xfffe))
	{
               MSPD_DBG("===== Paging/SIB Message Send out in Frame (%d:%d) \
               current time (%d:%d) rnti %x=========\n",timingInfo.sfn,
               timingInfo.subframe, cellCb->timingInfo.sfn,
               cellCb->timingInfo.subframe,pduInfo->rnti);
	}

         /* Filling the cwId */
	 /* Get the DCI format
	    Get the cwSwap flag
         */
         if( pduInfo->dciInfo.format == TFU_DCI_FORMAT_2 )
         {
            cwSwapFlag = pduInfo->dciInfo.u.format2AllocInfo.transSwap ;
         }
         else if( pduInfo->dciInfo.format == TFU_DCI_FORMAT_2A )
         {
            cwSwapFlag = pduInfo->dciInfo.u.format2AAllocInfo.transSwap ;
         }

            if( ( pduInfo->dciInfo.format == TFU_DCI_FORMAT_2 ) ||
                  ( pduInfo->dciInfo.format == TFU_DCI_FORMAT_2A ) )
         {
 /* mimo_comment */
               ((PTXSDUREQ)(phyTxSdu.txSduReq))->cwId = (cwSwapFlag) ?
                  ((tbCount) ? 0:1 ) : tbCount ;
  	        if (FALSE != cwSwapFlag && ueCb)
	        {
	           /* Keep track of TBtoCW swapped subframes */
#ifndef TFU_UPGRADE
	           ueCb->cwSwpflg[datReq->timingInfo.subframe] = TRUE;
#endif
	        }
         }
         pElem = ysMsUtlGetPhyListElem();

         if (!pElem)
         {
            /* HARQ_DBG :TODO: need to free the allocated messages */
               uart_printf("ysMsUtlAllocSvsrMsg failed for PHY list element \
                     for TXSDU at cellTime(%d,%d)\n",
            cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
            RETVALUE(RFAILED);
         }

         YS_MS_FILL_PHY_LIST_ELEM(pElem,timingInfo.sfn,timingInfo.subframe,
       	    phyTxSdu.txSduReq, sizeof(TXSDUREQ), PHY_TXSDU_REQ);

         /* Add DL SDUs to the linked list of PHY messages */
         ysMsUtlAddToPhyList(pDlData,pElem);

         phyTxSdu.txSduReq = NULLP;
      }
    }
      /*-- Fill the channel descriptor --*/
      ysMsUtlFillDlChanDesc(pduInfo, NULLP, cellCb, &txVect->dlCh[idx],
            PDSCH, chIdx, timingInfo);

      txVect->dlCh[idx].offsetNextCh = 0;
      chIdx++;
      idx++;


      txVect->numberofChannelDescriptors++;
      cmLstEnt = cmLstEnt->next;
   }
#ifdef MSPD
   if (ueCnt >= 2)
   {
      MSPD_DBG("MUE_PER_TTI_DL: Data requests for %d UEs per TTI\n", ueCnt);
   }
#endif

   /*-- Fill the offset for the last channel --*/
   /*--  Check for Buffer overflow --*/
   if (idx >= MAXCHSUBFRAME)
   {
      YS_DBG_ERR((_ysp, "Exceeding Channel configuration\n"));
      MSPD_ERR("Exceeding Channel configuration\n");
      RETVALUE (RFAILED);
   }

   /* TODO:Setting this for the time being as per Vitaliy input.Can be
      reverted at later stage */
   if (idx)
   {
      txVect->dlCh[idx - 1].offsetNextCh = 0;
   }
   else
   {
      MSPD_DBG("Got dummy dat req @ CL (%d,%d)\n",
	  	cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
   }
   txVect->numberofChannelDescriptors = chIdx;
   cellCb->dlEncL1Msgs[sfNum].numChannels = chIdx;

   /* Update the combined list appropriately with DL Data info */

   pUlDlLst = &cellCb->dlEncL1Msgs[sfNum].ulDlLst;
   if (pDlData->count)
   {
      ysMsUtlCatPhyList(pUlDlLst, pDlData);

      ysMsUtlInitPhyList(pDlData);
   }

    /* Check whether the CRC indication has been sent for this subframe or not
     * if yes
     * 1. append the UlCntrlreq elements into the PHY message list at the end.
     * 2. Send the TX and RX vector down.
     * else
    * 2. set the flag isdatReqPres to FALSE as the DatReq is already
    *    processed here.
     * */
    if (TRUE == cellCb->crcSent[cellCb->timingInfo.subframe])
    {

      /* As we always combine the UlCnrl for previous subframe with current
       * subframes
	 * DlCnrl info we are retrieving the UlCntrl from the previous subframe */
        
	    YS_MS_DECR_TIME(timingInfo,ulCntrlTiming, (TFU_DELTA - TFU_ULCNTRL_DELTA));
	  	
        pUlCntrl= &cellCb->dlEncL1Msgs[ulCntrlTiming.subframe].ulCntrl;
	    if (0 < pUlCntrl->count)
	    {
           ysMsUtlCatPhyList(pUlDlLst, pUlCntrl);
           ysMsUtlInitPhyList(pUlCntrl);
	    }
	/* sending the Tx&RX vector down */
        ysMsDlmSndVectSDU(cellCb);
	/* reset the flag as the current subframe is sent out*/
	cellCb->dlEncL1Msgs[sfNum].isdatReqPres = FALSE;
    }
    else
    {
	cellCb->dlEncL1Msgs[sfNum].isdatReqPres = FALSE;
    }

   /* All channels except PCFICH will be filled in TXSDU */
   RETVALUE(ret);
}

/*
*
*       Fun:   ysMsDlmIsDataPres
*
*       Desc:  Checks if Data is expected for the control request 
*
*       Ret:   ROK/RFAILED
*
*       Notes: None
*
*       File:  ys_ms_dl.c
*
*/
#ifdef ANSI
PRIVATE BOOL ysMsDlmIsDataPres
(
TfuCntrlReqInfo *cntrlReq
)
#else
PRIVATE BOOL ysMsDlIsDataPres(cntrlReq)
TfuCntrlReqInfo *cntrlReq;
#endif
{
   CmLList      *lnk = NULLP;
   TfuPdcchInfo *pdcchInfo = NULLP;

   if (!cntrlReq->dlPdcchLst.count)
   {
      if (((cntrlReq->dlTiming.subframe == 0)
            && ((cntrlReq->dlTiming.sfn % 4) == 0)))
      	{
      	   #ifdef CL_DEBUG
           MSPD_DBG("\nReceived CntrlReq for DLPDCCH and also setting the flag"
           " cell Time (%d) (%d) CntrlReqTime (%d) (%d) cout (%u)\n",
           cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
           cntrlReq->dlTiming.sfn , cntrlReq->dlTiming.subframe,
           cntrlReq->dlPdcchLst.count);
           #endif
		   RETVALUE(TRUE);
      	}
	  RETVALUE(FALSE);
   }
   else
   {
      for (lnk = cntrlReq->dlPdcchLst.first; lnk; lnk = lnk->next)
      {
         pdcchInfo = (TfuPdcchInfo*)(lnk->node);
		 if(pdcchInfo->dci.dciFormat == TFU_DCI_FORMAT_3 || pdcchInfo->dci.dciFormat == TFU_DCI_FORMAT_3A)
		 {
		    continue;
		 }
		 else
		 {
		    break;
		 }
      }
	  if (lnk)
	  {
	     #ifdef CL_DEBUG
         MSPD_DBG("\nReceived CntrlReq for DLPDCCH and also setting the flag"
           " cell Time (%d) (%d) CntrlReqTime (%d) (%d) cout (%u)\n",
           cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
           cntrlReq->dlTiming.sfn , cntrlReq->dlTiming.subframe,
           cntrlReq->dlPdcchLst.count);
         #endif
		 RETVALUE(TRUE);
	  }
	  else
	  {
	     RETVALUE(FALSE);
	  }
      
   }

}

/*
*
*       Fun:   ysMsDlmPrcCntrlReq
*
*       Desc:  Cntrl Request Handler
*
*       Ret:   ROK/RFAILED
*
*       Notes: None
*
*       File:  ys_ms_dl.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsDlmPrcCntrlReq
(
YsCellCb        *cellCb,
TfuCntrlReqInfo *cntrlReq
)
#else
PUBLIC S16 ysMsDlmPrcCntrlReq(cellCb, cntrlReq)
YsCellCb        *cellCb;
TfuCntrlReqInfo *cntrlReq;
#endif
{
   PGENMSGDESC  pMsgDesc = NULLP;
   PDLSUBFRDESC txVector;
   /* HARQ: DCI0_PHICH */
   PHIADCIULMSGDESC dci0HiVector;
   U8               sfNum;
   PMAC2PHY_QUEUE_EL pElem;
   YsPhyLstCp *pDlCntrl = NULLP;
   YsPhyLstCp *pUlCntrl = NULLP;
   YsPhyLstCp *pUlDlLst = NULLP;
#ifdef MSPD_MLOG_NEW
   volatile int    t = MacGetTick();
#endif

   TRC2(ysMsDlmPrcCntrlReq)

   if (cntrlReq->ulPdcchLst.count || cntrlReq->phichLst.count)
   {
  
      CmLteTimingInfo timingInfo;
      YS_MS_DECR_TIME(cntrlReq->ulTiming,timingInfo, 2);
      if (!YS_TIMING_INFO_SAME(timingInfo, cellCb->timingInfo))
      {
         MSPD_DBG("Dropping the Invalid UL CNTRLREQ(DCI0&HI) (%d,%d) crnttime(%d,%d)****\n",
         cntrlReq->ulTiming.sfn, cntrlReq->ulTiming.subframe, cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
         RETVALUE(ROK);
      }
			
      sfNum = cntrlReq->ulTiming.subframe;

      /* Allocate a message towards Phy using sysCore for DCI0HI vector */
      pMsgDesc = (PGENMSGDESC)ysMsUtlAllocSvsrMsg(FALSE);
      if (pMsgDesc == NULLP)
      {
         YS_DBG_ERR((_ysp, "Unable to allocate memory for HIDCI0 Vector"));
         uart_printf("Memory for HIDC0 vector \n");
         RETVALUE (RFAILED);
      }

      dci0HiVector = (PHIADCIULMSGDESC)(pMsgDesc);
      ysMsUtlFillAndAddDci0HiInfo(cntrlReq, cellCb, dci0HiVector);

      /* Append the ulCntrl to combined list of next subframe if the DatReq is not
       * expected for this subframe */
      /* As the ULCntrl always comes for the previous subframe timing info
       * move it to the subfame for which DLCntrlReq was recieved */
      sfNum = (sfNum + (TFU_DELTA - TFU_ULCNTRL_DELTA)) % YS_NUM_SUB_FRAMES;
      if (FALSE == cellCb->dlEncL1Msgs[sfNum].isdatReqPres)
      {
         pUlCntrl= &cellCb->dlEncL1Msgs[cntrlReq->ulTiming.subframe].ulCntrl;
         pUlDlLst = &cellCb->dlEncL1Msgs[sfNum].ulDlLst;
#ifdef CL_DEBUG
         MSPD_DBG("\ULCNTRL (%d) (%d) count : %d at sfNum %u\n",
         cntrlReq->ulTiming.sfn, cntrlReq->ulTiming.subframe, pUlDlLst->count, sfNum);
#endif
         ysMsUtlCatPhyList(pUlDlLst, pUlCntrl);
         ysMsUtlInitPhyList(pUlCntrl);
      }
#ifdef MSPD_MLOG_NEW
     MLogTask(PID_CL_CNTRL_REQ_UL, RESOURCE_LARM, t, MacGetTick());
#endif

#ifdef RG_ULSCHED_AT_CRC
      RETVALUE(ROK);
#endif
   }

   pMsgDesc = NULLP;
   /* Detecting the existence of DatReq from the MAC for this fubframe
	   checking for PDSCH or PBCH DatReq */
   sfNum = cntrlReq->dlTiming.subframe;
   cellCb->dlEncL1Msgs[sfNum].isdatReqPres = ysMsDlmIsDataPres(cntrlReq);
   
   pMsgDesc = cellCb->dlEncL1Msgs[sfNum].txVector;
   pDlCntrl = &cellCb->dlEncL1Msgs[sfNum].dlCntrl;

    /* At this point it is possible that Data Req has arrived and
        * we have already allocated the txVector - as it is loosely coupled it
        * is also possible that Data Req hasn't arrived yet - so we should
        * check and allocated
        */
      if (pMsgDesc == NULLP)
      {

         /* Allocate a message towards Phy using sysCore */
         pMsgDesc = (PGENMSGDESC) ysMsUtlAllocSvsrMsg (FALSE);
         if (pMsgDesc == NULLP)
         {
            YS_DBG_ERR((_ysp, "Unable to allocate memory for TxVector"));
            uart_printf("No Memory for Tx Vector in CntrlReq\n");
            RETVALUE (RFAILED);
         }

         pMsgDesc->msgType     = PHY_TXSTART_REQ;
         pMsgDesc->phyEntityId = YS_PHY_INST_ID;
         pMsgDesc->msgSpecific = sizeof(DLSUBFRDESC);
         txVector = (PDLSUBFRDESC)(pMsgDesc + 1);
         txVector->numberofChannelDescriptors = 0;
      /* TODO:Setting this for the time being as per Vitaliy input.
         Can be reverted at later stage */
         txVector->dlCh[0].offsetNextCh = 0;

         /* Buffer the txVector for next transmission */
         cellCb->dlEncL1Msgs[sfNum].txVector = pMsgDesc;

         /* Add message to the linked list of PHY messages */

         pElem = ysMsUtlGetPhyListElem();
         if (!pElem)
         {
            /* HARQ_DBG */
         uart_printf("ysMsUtlAllocSvsrMsg failed for PHY list element \
               for TxVector at time(%d,%d)\n",
            cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
            RETVALUE(RFAILED);
         }
      YS_MS_FILL_PHY_LIST_ELEM(pElem, txVector->frameNumber,
            txVector->subframeNumber,
               pMsgDesc, sizeof(GENMSGDESC) + sizeof(DLSUBFRDESC), PHY_TXSTART_REQ);
         ysMsUtlAddToPhyList(pDlCntrl,pElem);
      }
	  pDlCntrl = &cellCb->dlEncL1Msgs[sfNum].dlCntrl;
      txVector = (PDLSUBFRDESC)(pMsgDesc + 1);

      /* Fill the TxVector with the recieved CntrlReqInfo */
      if (ysMsUtlFillTxVector(cntrlReq, cellCb, txVector) != ROK)
      {
         /* HARQ_TODO: Free the last allocated message and element of PHY linked list */
         uart_printf("ysMsUtlAllocSvsrMsg failed for PHY list element for TxVector\
         SDU\n");
         RETVALUE(RFAILED);
      }

      /* HARQ: Assuming there is a control request always from MAC */
      /* Append the DL control and data to the combined list also*/
      pUlDlLst = &cellCb->dlEncL1Msgs[sfNum].ulDlLst;
      ysMsUtlCatPhyList(pUlDlLst, pDlCntrl);
      ysMsUtlInitPhyList(pDlCntrl);


   pMsgDesc = NULLP;
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_CL_CNTRL_REQ_DL, RESOURCE_LARM, t, MacGetTick());
#endif

   RETVALUE(ROK);

}

PUBLIC U8 memAlrm = 0;

/*
*
*       Fun:   ysMsDlmPrcTtiInd
*
*       Desc:  TTI handler from PHY
*
*       Ret:   ROK/RFAILED
*
*       Notes: None
*
*       File:  ys_ms_dl.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsDlmPrcTtiInd
(
PMSGIND         txStartInd,
YsCellCb        *cellCb,
Bool            isDummyTti
)
#else
PUBLIC S16 ysMsDlmPrcTtiInd(txStartInd, cellCb)
PMSGIND         txStartInd;
YsCellCb        *cellCb;
Bool            isDummyTti;
#endif
{
   S16             ret = ROK;
   CmLteTimingInfo timingInfo;
   /* HARQ */
   CmLteTimingInfo rxSduSf;
   TfuTtiIndInfo   *macTtiInd;
   TfuTtiIndInfo   *schTtiInd;
   volatile int              t;
   YsUlL1Msg *ulRxSduInds = NULLP;
   U8 loopCnt = 0;
#ifdef RG_ULSCHED_AT_CRC
   U32 t1 = 0;
#endif

   ++numeTti;

   TRC2(ysMsDlmPrcTtiInd)

   if (!cellCb->phyState)
   {
      YS_DBG_ERR((_ysp, "PHY is not in running state"));
      uart_printf( "PHY is not in running state");
      RETVALUE(RFAILED);
   }

   /* Set the current time information */
   timingInfo = cellCb->timingInfo;

/*Periodically monitor the available memory in the system so that we dont admit incoming packets at PDCP beyond available memory limits*/
   if((numeTti %100) == 0)
	{		
		SChkResUtl(DFLT_REGION,&memAlrm);
		MSPD_DBG("Memory utilisation ALARM SET:%d\n", memAlrm);
	}
   
#ifndef RG_ULSCHED_AT_CRC
   /* Send UL messages received in previous TTI */
   {
      TfuCrcIndInfo     *crcIndInfo = NULLP;
      CmLteTimingInfo   prevSf;

      YS_MS_DECR_TIME(timingInfo, prevSf, 2);

#ifdef CL_DEBUG
      MSPD_DBG("CRC not expected at (%d, %d) prevSf (%d,%d) rxSdu in prev Sf (%d)\n",
         timingInfo.sfn, timingInfo.subframe, prevSf.sfn, prevSf.subframe,
         cellCb->ulEncL1Msgs[prevSf.subframe].rxSduPres);
#endif

      if (cellCb->ulEncL1Msgs[prevSf.subframe].rxSduPres)
      {
         CmLteTimingInfo rxEndTiming = prevSf;
         /* Previous subframe is ready to be sent */

#ifdef CL_DEBUG
      MSPD_DBG("PrevSf (%d,%d) sending at (%d,%d)\n",
         prevSf.sfn, prevSf.subframe,
         timingInfo.sfn, timingInfo.subframe);
#endif
         ysMsUlmRxEndIndAtRxSdu(rxEndTiming, cellCb);
       }
   }
#endif

   /* Allocate TTI for MAC */
   ret = SGetSBuf(ysCb.macTfuSap->sapPst.region,
                  ysCb.macTfuSap->sapPst.pool,
                  (Data **)&macTtiInd, sizeof(TfuTtiIndInfo));
   if(ret == RFAILED)
   {
      YS_DBG_ERR((_ysp, "SGetSBuf(): Memory allocation failed\n"));
      MSPD_ERR( "SGetSBuf(): Memory allocation failed\n");
      RETVALUE(RFAILED);
   }

   /* Allocate TTI for MAC Scheduler */
   ret = SGetSBuf(cellCb->schTfuSap->sapPst.region,
                  cellCb->schTfuSap->sapPst.pool,
                  (Data **)&schTtiInd, sizeof(TfuTtiIndInfo));
   if(ret == RFAILED)
   {
      uart_printf("SGetSBuf(): Memory allocation failed\n");
      YS_DBG_ERR((_ysp, "SGetSBuf(): Memory allocation failed\n"));
      MSPD_ERR( "SGetSBuf(): Memory allocation failed\n");
      SPutSBuf(ysCb.macTfuSap->sapPst.region,
               ysCb.macTfuSap->sapPst.pool,
               (Data *)macTtiInd, sizeof(TfuTtiIndInfo));
      RETVALUE(RFAILED);
   }

   macTtiInd->cellId              = cellCb->cellId;
   macTtiInd->timingInfo.subframe = timingInfo.subframe;
   macTtiInd->timingInfo.sfn      = timingInfo.sfn;
   macTtiInd->isDummyTti          = isDummyTti;      

   *schTtiInd = *macTtiInd;

   /* changing the order of TTI invocation
	 * Old order - MAC then SCH
	 * New order - SCH then MAC
	 * This is a crude work around to get the TX vector in order
	 * i.e. Data followed by PDCCH information.
	 */
	   /* send TTI indication to MAC */
   YsUiTfuTtiInd(&ysCb.macTfuSap->sapPst, ysCb.macTfuSap->suId, macTtiInd);

   /* send TTI indication to scheduler */
   YsUiTfuSchTtiInd(&cellCb->schTfuSap->sapPst,
                    cellCb->schTfuSap->suId,
                    schTtiInd);
#ifdef SS_DIAG 
   if((ssDiagSndToPack( &ysCb.ysInit.lmPst)) != ROK)
   {
      YS_DBG_ERR((_ysp, "ssPackLogs(): Packing of log failed\n"));
   }
#endif

#ifdef RG_ULSCHED_AT_CRC
   t1 = GetTIMETICK();
   /* Process the UL Data & Cntrl for this subframe */
   ulRxSduInds = &cellCb->ulRxSduInds[timingInfo.subframe];
   if (ulRxSduInds->sduIndCnt)
   {
      for (loopCnt = 0; loopCnt < ulRxSduInds->sduIndCnt; loopCnt++)
      {
	 ret = ysMsUlmSduInd(ulRxSduInds->pRxSduInd[loopCnt], cellCb);
	 SvsrFreeIMem(ulRxSduInds->pRxSduInd[loopCnt]);
         ulRxSduInds->pRxSduInd[loopCnt] = NULLP;
      }
      /* resetting the count */
      ulRxSduInds->sduIndCnt = 0;
   }
   if (NULLP != ulRxSduInds->pRxEndInd)
   {
      ysMsUlmRxEndInd(ulRxSduInds->pRxEndInd, cellCb);
      SvsrFreeIMem(ulRxSduInds->pRxEndInd);
      ulRxSduInds->pRxEndInd = NULLP;
   }
#ifdef MSPD_MLOG_NEW
      MLogTask(PID_CL_RXSDUANDEND_IND,RESOURCE_LARM, t1, GetTIMETICK());
#endif
   /* HARQ_ERR: Last subframe, no CRC indication was sent */
   rxSduSf = timingInfo;
	rxSduSf.subframe = timingInfo.subframe ? (timingInfo.subframe - 1): 9 ;
   if (!cellCb->crcSent[rxSduSf.subframe])
   {
      /* Neither dummy nor actual CRC was sent in last subframe */
      MSPD_DBG("ULCRCIND ERR: Neither dummy nor actual CRC was sent in last subframe"
            " Last TTI(%d,%d): exp(%d) sent(%d) CrntTTI(%d,%d) exp(%d) crcSent(%d)\n",
            rxSduSf.sfn, rxSduSf.subframe,
            cellCb->isCrcExptd[rxSduSf.subframe],
            cellCb->crcSent[rxSduSf.subframe], timingInfo.sfn,
            timingInfo.subframe, cellCb->isCrcExptd[timingInfo.subframe],
            cellCb->crcSent[timingInfo.subframe]);
	  DelyedCrcInd++;
	  #if 0
      stop_printf("ULCRCIND ERR: Neither dummy nor actual CRC was sent in last subframe"
            " Last TTI(%d,%d): exp(%d) sent(%d) CrntTTI(%d,%d) exp(%d) crcSent(%d)\n",
            rxSduSf.sfn, rxSduSf.subframe,
            cellCb->isCrcExptd[rxSduSf.subframe],
            cellCb->crcSent[rxSduSf.subframe], timingInfo.sfn,
            timingInfo.subframe, cellCb->isCrcExptd[timingInfo.subframe],
            cellCb->crcSent[timingInfo.subframe]);
	  #endif
   }

    /* HARQ_UL: If TTI indication is recieved for subframe which is not expected
	   to have CRC indication, send dummy CRC indication here */
   if ((!cellCb->isCrcExptd[timingInfo.subframe]) &&
         (!cellCb->crcSent[timingInfo.subframe]))
   {
         TfuCrcIndInfo     *crcIndInfo = NULLP;
         t = MacGetTick();
         /* Send the CRC Indication to MAC for the corresponding subframe */
         ret = ysUtlAllocEventMem((Ptr *)&crcIndInfo,
               sizeof(TfuCrcIndInfo));
         if(ret == RFAILED)
         {
            YS_DBG_ERR((_ysp, "ysMsUpdCrcInd(): Memory allocation failed for CrcInd\n"));
            MSPD_ERR("Memory allocation failed for CrcInd\n");
            RETVALUE(RFAILED);
         } /* end of if statement */

#ifdef CL_DEBUG
         MSPD_DBG("Fake CRC without RX SDU %x at (%d, %d)\n",
         crcIndInfo, cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
         #endif


         cmLListInit(&(crcIndInfo->crcLst));

         crcIndInfo->cellId = ysCb.cellId;
         crcIndInfo->timingInfo.sfn = timingInfo.sfn;
         crcIndInfo->timingInfo.subframe = timingInfo.subframe;
         ysSendCrcToMac(cellCb, crcIndInfo);
         cellCb->crcSent[timingInfo.subframe] = TRUE;
         #ifdef MSPD_MLOG_NEW
         MLogTask(PID_CRC_IND_DUMMY, RESOURCE_LARM, t, MacGetTick());
         #endif
   }
#endif /* RG_ULSCHED_AT_CRC */

#ifdef CL_DEBUG
   MSPD_DBG("Sent dummy CRC Indication to MAC(%d,%d)\n", timingInfo.sfn,
         timingInfo.subframe);
#endif
   RETVALUE(ROK);

}


#ifdef YS_ALL_PERF

#if YS_PHY_STOP_AUTO  /* Useful when auto stop is enabled */
PRIVATE Void ysAllPerfPrntTimes (YsCellCb		*cellCb)
{
	U32   indx;
	U32   prevIndx;
	float timeTaken;
	float ttiDiff;
	Txt	_prntBuf[256];

   sprintf(_prntBuf, "ysAllPerfPrntTimes Number of entries  (%d)\n",
         cellCb->ttiIndex);
	SPrint(_prntBuf);
	sprintf(_prntBuf, "\nStart EndTime\t\tDiff\t\tInterTtiDiff\tAvailMem \n");
	SPrint(_prntBuf);
	for (indx = 0; indx < YS_MS_MAX_PERF_TTI; indx++)
	{
      timeTaken = (cellCb->perfInfo[indx].ttiEndTick -
            cellCb->perfInfo[indx].ttiStartTick);
		timeTaken = (timeTaken / 150000.0);

		prevIndx = (indx == 0)? 0: (indx - 1);

      ttiDiff = (cellCb->perfInfo[indx].ttiStartTick -
            cellCb->perfInfo[prevIndx].ttiStartTick);
		ttiDiff = (ttiDiff / 150000.0);

      if(cellCb->maxtimeTaken < timeTaken)
         cellCb->maxtimeTaken = timeTaken;

      if(cellCb->maxttiDiff < ttiDiff)
         cellCb->maxttiDiff = ttiDiff;

		sprintf(_prntBuf, "\n\t(%d)\t(%d)\t(%u)\t(%u)\t(%f)\t(%f)\t(%d)",
            cellCb->perfInfo[indx].timingInfo.sfn,
            cellCb->perfInfo[indx].timingInfo.subframe,
            cellCb->perfInfo[indx].ttiStartTick,
            cellCb->perfInfo[indx].ttiEndTick, timeTaken, ttiDiff,
					cellCb->perfInfo[indx].avlMem);
		SPrint(_prntBuf);
	}

   sprintf(_prntBuf, "\nmaxtimeTaken=%f   maxttiDiff=%f\n",
         cellCb->maxtimeTaken, cellCb->maxttiDiff);
	SPrint(_prntBuf);
	RETVOID;
}

#else

PUBLIC Void ysPerfPrntTimes (YsCellCb		*cellCb)
{
	float timeTaken;
	float ttiDiff;
	Txt	_prntBuf[256];

   if(cellCb->ttiIndex == 0)
      cellCb->perfInfo.ttiEndTick = cellCb->perfInfo.ttiStartTick;

   timeTaken = (cellCb->perfInfo.ttiEndTick - cellCb->perfInfo.ttiStartTick);
   timeTaken = (timeTaken / 150000.0);
   sprintf(_prntBuf, "\t%f\n",timeTaken );

   SPrint(_prntBuf);

	RETVOID;
}

#endif
#endif

U32 dlrate_tfu, dlrate_rgu, dlrate_kwu, dlrate_pju;
U32 ulrate_tfu, ulrate_rgu, ulrate_kwu, ulrate_pju, ulrate_grant;
U32 dldrops_kwu_um, dlpkt_um, buffer_occ, ulCqiRptCnt[16], dlCqiRptCnt[16];
U32 totDlCqiRpt, totUlCqiRpt;
U32 ysTotalPkts, ysPktsDropped, ysPktsDrpdMem;
U32 ysMsDlAmTxPdus = 0, ysMsDlAmRetxPdus = 0;
U32 ysMsAmmDlRetxs[10];
U32 ysMsAmmDlPollReTx;
U32 ysMsNumPartRlcNacks, ysMsNumMaxReTx;
U32 ysMsNumCtrlPdus, ysMsNumErrCtrlPdus, ysMsNumRlcNacks;
U32 ysMsAmmSduBranch1, ysMsAmmSduBranch2, ysMsAmmSduBranch3;
U32 ysMsAmmReEstReqRcvd, ysMsAmmDelUeReqRcvd, ysMsAmmErrReEstReqRcvd;
U32 invaldl1Tti;
U32 ysMsAmmNumToBeRlsTxLstCount, ysMsAmmNumToBeRlsReTxLstCount;
U32 ysRlcTimerArr[6], ysPdcpTimerArr[5];
U32 ysMsAmmNumStaPduRbLstCount, ysMsAmmNumReassemblyRbLstCount, ysMsAmmNumPduRlsRbLstCount, ysMsAmmNumReestablishRbLstCount, ysMsAmmNumDeleteRbLstCount, ysMsAmmNumReassemblyLstCount;
U32 ysMsAmmStaPduCounter, ysMsAmmNumSduQCount, ysMsAmmNumRetxLstCnt, ysMsAmmPjNumEntries, ysMsAmmPjDatQ;
U32 ysMsAmmUlNumPduDiscarded, ysMsAmmUlMaxLiCntRcvd, ysMsAmmNumAllBo, ysMsAmmNewDataBo;
U32 ysMsUmmReEstReqRcvd, ysMsUmmErrReEstReqRcvd, ysMsUmBo;
int fmsSud =0;
U32 missedCntr = 0;
U32 noTti = 0;
U32 dummyRxSdu=0;

/*
*
*       Fun:   ysMsDlmSndVectSDU
*
*       Desc:  Send Tx and Rx vectors prepared in a linked list
*
*       Ret:   ROK/RFAILED
*
*       Notes: None
*
*       File:  ys_ms_dl.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsDlmSndVectSDU
(
YsCellCb        *cellCb
)
#else
PUBLIC S16 ysMsDlmSndVectSDU(cellCb)
YsCellCb        *cellCb;
#endif
{
   PGENMSGDESC          pMsgDesc;
   YsPhyLstCp           *pUlDlList = NULLP;
   PMAC2PHY_QUEUE_EL    pElem = NULLP;
   CmLteTimingInfo      timingInfo;
   U8                   sfNum;
#ifdef MSPD_MLOG_NEW
   U32                  startTick = 0;
#endif
   double     diff = 0;
   U32        endTti = 0;
 

#if defined(YS_MIB_WARND) || defined (YS_PHY_STOP_AUTO)
   static unsigned int  tticnt = 0;
#endif

   TRC2(ysMsDlmSndVectSDU)

#ifdef MSPD_MLOG_NEW
   startTick = GetTIMETICK();
#endif

   if (!cellCb->phyState)
   {
      YS_DBG_ERR((_ysp, "PHY is not in running state"));
      uart_printf( "PHY is not in running state");
      RETVALUE(RFAILED);
   }

#ifdef YS_MIB_WARND
   if(++tticnt >= 36)
   {
      acceptPrimsFrmMac = TRUE;
   }
#else
   tticnt++;
#endif
   if (tticnt == 1000 * STAT_PERIOD)
   {
      U32 memAvail=0;
      U32 dlCqiRptCntLt15 = dlCqiRptCnt[14] + dlCqiRptCnt[13] + 
                            dlCqiRptCnt[12] + dlCqiRptCnt[11];
      U32 ulCqiRptCntLt15 = ulCqiRptCnt[14] + ulCqiRptCnt[13] + 
                            ulCqiRptCnt[12] + ulCqiRptCnt[11];
      if (dlrate_tfu >= STAT_THRESH * STAT_PERIOD)
         MSPD_LOG("dl kbps tfu=%u rgu=%u kwu=%u pju=%u kwu_drops_um=%u num_um_pkts=%u bo=%u ysTotalPkts=%u ysPktsDropped=%u ysPktsDrpdMem=%u"
         "(dlCqi=15)=%d%% (dlCqi>10&&<15)=%d%% (dlcqi<10)=%d%% \n",
            to_kb(dlrate_tfu), to_kb(dlrate_rgu), to_kb(dlrate_kwu),
      	    to_kb(dlrate_pju), dldrops_kwu_um, dlpkt_um, buffer_occ,
      	    ysTotalPkts, ysPktsDropped, ysPktsDrpdMem, ((dlCqiRptCnt[15] * 100)/totDlCqiRpt),((dlCqiRptCntLt15 * 100)/totDlCqiRpt),
      	    (((totDlCqiRpt - dlCqiRptCnt[15] - dlCqiRptCntLt15) * 100)/totDlCqiRpt));
      dlrate_tfu = dlrate_rgu = dlrate_kwu = dlrate_pju = dldrops_kwu_um = totDlCqiRpt = 0;	  	    //ysTotalPkts = ysPktsDropped = 0;
      cmMemset((U8 *) dlCqiRptCnt, 0, 16 * sizeof(U32));
	  
      if (ulrate_tfu >= STAT_THRESH * STAT_PERIOD)
      {
         MSPD_LOG("ul kbps tfu=%u rgu=%u kwu=%u pju=%u grantrate=%u"
                  "(ulCqi=15)=%d%% (ulCqi>10&&<15)=%d%% (ulcqi<10)=%d%% \n",
                  to_kb(ulrate_tfu), to_kb(ulrate_rgu), to_kb(ulrate_kwu),
      	          to_kb(ulrate_pju), to_kb(ulrate_grant), 
                  (ulCqiRptCnt[15] * 100)/totUlCqiRpt,
      	          (ulCqiRptCntLt15 * 100)/totUlCqiRpt,
      	          (((totUlCqiRpt - ulCqiRptCnt[15] - ulCqiRptCntLt15) * 100) / 
                    totUlCqiRpt));
      }
      cmMemset((U8 *) ulCqiRptCnt, 0, 16 * sizeof(U32));
      MSPD_LOG("MissdCntr %d noTti %d dummyRxSdu%d  TTI proc time (>1 to 1.25): %d (1.25 to 1.5): %d"
                "(1.5 to 2): %d (>2): %d NumOfDummyTTI:%d NumOfDatReqDrop:%d"
                "Delayed Expected CrcInd:%d Number of Invalid TTIs:%d\n", missedCntr, noTti,dummyRxSdu, 
                robLogs[0], robLogs[1], robLogs[2], robLogs[3], robLogs[4], 
                robLogs[5], invaldl1Tti, DelyedCrcInd);

      MSPD_LOG("Delayed HARQ Count (%d)\n",delayedHarq);
      MSPD_LOG("Memory Alarm  (%d)\n",memAlrm);
      robLogs[0]= robLogs[1] = robLogs[2] = robLogs[3] = robLogs[4] = robLogs[5] = invaldl1Tti = 0;
      ulrate_tfu = ulrate_rgu = ulrate_kwu = ulrate_pju = ulrate_grant = totUlCqiRpt = totDlCqiRpt = 0;
      tticnt = 0;
#ifdef HARQ_STATISTICS
      totl_dlAcks += dlAcks;
      totl_dlNacks += dlNacks;
      totl_ulAcks += ulAcks;
      totl_ulNacks += ulNacks;
      totlDlTb += totl_dlTbs;
      totlUlTb += totl_ulTbs;
      MSPD_LOG("\n=========================================\n");
      MSPD_LOG("DL HARQ STATS for %dth Sec- (ACK/NACK/TOTAL DL TBS) \
			 (%d/%d/%d)\n",Secs, dlAcks, dlNacks, totl_dlTbs);
      MSPD_LOG("DL HARQ STATS so far - (ACK/NACK/TOTAL DL TBS) \
			 (%d/%d/%d)\n", totl_dlAcks, totl_dlNacks, totlDlTb);
      MSPD_LOG("UL HARQ STATS for %dth Sec- (ACK/NACK/TOTAL UL TBS) \
			 (%d/%d/%d)\n", Secs, ulAcks, ulNacks, totl_ulTbs);
      MSPD_LOG("UL HARQ STATS so far - (ACK/NACK/TOTAL UL TBS) \
			 (%d/%d/%d)\n", totl_ulAcks, totl_ulNacks, totlUlTb);
      MSPD_LOG("\n=========================================\n");

      dlAcks = dlNacks = ulAcks = ulNacks = totl_dlTbs = totl_ulTbs = 0;
      Secs++;
#endif
      MSPD_LOG("******************DL RLC Stats(Till now)*****************\n");
      MSPD_LOG("#TxPDUs:%d #PollRetxPDUs:%d #MaxReTx:%d\n",
               ysMsDlAmTxPdus, ysMsAmmDlPollReTx, ysMsNumMaxReTx);
      MSPD_LOG("#UeDelRcvd:%d #ReEstRcvd:%d #ErrReEstRcvd:%d\n", 
                ysMsAmmDelUeReqRcvd, ysMsAmmReEstReqRcvd,
                ysMsAmmErrReEstReqRcvd);
      MSPD_LOG("#CtrlPdus:%d #ErrCtrlPdus:%d #Nacks:%d #PartialNacks:%d\n", 
                ysMsNumCtrlPdus, ysMsNumErrCtrlPdus, ysMsNumRlcNacks,
                ysMsNumPartRlcNacks);
      MSPD_LOG("Retx: 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d 7:%d\n",
               ysMsAmmDlRetxs[0], ysMsAmmDlRetxs[1], ysMsAmmDlRetxs[2],
               ysMsAmmDlRetxs[3], ysMsAmmDlRetxs[4], ysMsAmmDlRetxs[5],
               ysMsAmmDlRetxs[6]);
#if 0
      MSPD_LOG("RLC TMR %d %d %d %d %d %d\n",
               ysRlcTimerArr[0], ysRlcTimerArr[1], ysRlcTimerArr[2],
               ysRlcTimerArr[3], ysRlcTimerArr[4], ysRlcTimerArr[5]);
      MSPD_LOG("PDCP TMR %d %d %d %d %d\n",
               ysPdcpTimerArr[0], ysPdcpTimerArr[1], ysPdcpTimerArr[2],
               ysPdcpTimerArr[3], ysPdcpTimerArr[4]);
#endif
      MSPD_LOG("RB Lists - #PduRls:%d #StaPdu:%d #Reassembly:%d "
               "#ReEst:%d #Delete:%d\n",
                ysMsAmmNumPduRlsRbLstCount, ysMsAmmNumStaPduRbLstCount,
                ysMsAmmNumReassemblyRbLstCount, ysMsAmmNumReestablishRbLstCount,
                ysMsAmmNumDeleteRbLstCount);
#if 0
      /* TODO - Need to have consolidated across multiple UEs */
      MSPD_LOG("PDU Lists - #Tx:%d #ReTx:%d #Reassembly:%d #StatusPdu:%d\n", 
                ysMsAmmNumToBeRlsTxLstCount, ysMsAmmNumToBeRlsReTxLstCount,
                ysMsAmmNumReassemblyLstCount, ysMsAmmStaPduCounter);
#endif
      MSPD_LOG("******************UL RLC Stats(Till now)*****************\n");
      MSPD_LOG("NumUlPduDiscarded %d MaxLiCountRcvd %d \n",
               ysMsAmmUlNumPduDiscarded, ysMsAmmUlMaxLiCntRcvd);
      MSPD_LOG("PDCP: Total Pkts=%u Pkts Dropped=%u\n",
               ysTotalPkts, ysPktsDropped);
      SRegInfoShow(0,&memAvail);

      /* Initialize the counters */
      memAvail = 0; tticnt = 0;
      dlrate_tfu = dlrate_rgu = dlrate_kwu = dlrate_pju = 0;
      dldrops_kwu_um = 0;
      robLogs[0]= robLogs[1] = robLogs[2] = robLogs[3] = 0;
      robLogs[4] = robLogs[5] = invaldl1Tti = 0;
      ulrate_tfu = ulrate_rgu = ulrate_kwu = ulrate_pju = 0;
      ulrate_grant = 0;
   }

   timingInfo.subframe = cellCb->timingInfo.subframe;
   timingInfo.sfn	   = cellCb->timingInfo.sfn;


   /* HARQ: CL Sending 2 subframes in advance */
   sfNum = (timingInfo.subframe + (TFU_DELTA )) % YS_NUM_SUB_FRAMES;

   /* Send the TxVector, Tx SDUs for PDCCH and DL Data, HIDCI0 vector and
      HIDCI0 SDUs */
   pUlDlList = &cellCb->dlEncL1Msgs[sfNum].ulDlLst;

    /* HARQ_DBG */
#ifdef CL_DEBUG
    MSPD_DBG("Tx Vector %x at TTI(%d, %d) for sfNum %d\n",
         pUlDlList->head, timingInfo.sfn, timingInfo.subframe, sfNum);
#endif

   /* Add rxVector to the list */
   {
      PGENMSGDESC   pRxVector;
      PULSUBFRDESC  pUlSf;
      /* HARQ: CL sending 1 subframes in advance */
      sfNum = (timingInfo.subframe + (TFU_RECPREQ_DLDELTA)) % YS_NUM_SUB_FRAMES;
      pRxVector = (cellCb->ulEncL1Msgs[sfNum].rxVector);
      if (pRxVector)
      {
         pUlSf = (PULSUBFRDESC) (pRxVector + 1);

         YS_DBG_INFO((_ysp, "Sending RX Vector to PHY :  RX VECTOR time - \
                  (%d) (%d) Cell Time (%d) (%d)\n",
         pUlSf->frameNumber, pUlSf->subframeNumber,
         cellCb->timingInfo.sfn, cellCb->timingInfo.subframe));

         if ((pUlSf->numberofChannelDescriptors -
                  pUlSf->numberOfCtrlChannelDescriptors) >= 2)
         {
//#ifdef CL_DEBUG
            MSPD_DBG("MUE_PER_TTI_UL: pUlSf->frameNumber %u pUlSf->subframeNumber %u total \
                  channels %u PUSCH %u PUCCH %u\n",
               pUlSf->frameNumber,pUlSf->subframeNumber,
               pUlSf->numberofChannelDescriptors,
                  pUlSf->numberofChannelDescriptors -
                  pUlSf->numberOfCtrlChannelDescriptors,
               pUlSf->numberOfCtrlChannelDescriptors);
//#endif
         }

         pElem = ysMsUtlGetPhyListElem();
         if (!pElem)
         {
            /* HARQ_DBG */
            uart_printf("ysMsUtlAllocSvsrMsg failed for PHY list element"
                        "for rxVector at time(%d,%d)\n",
                        cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
            RETVALUE(RFAILED);
         }


         YS_MS_FILL_PHY_LIST_ELEM(pElem, pUlSf->frameNumber,
                                  pUlSf->subframeNumber, pRxVector,
                                  sizeof(GENMSGDESC) + sizeof(ULSUBFRDESC),
                                  PHY_RXSTART_REQ);
         ysMsUtlAddToPhyList(pUlDlList,pElem);
         cellCb->ulEncL1Msgs[sfNum].rxVector = NULLP;
      }
   }

   /* Send the pointer to the linked list to PHY */

   if (pUlDlList->count < 2)
   {
     if (acceptPrimsFrmMac)
      {
#ifdef CL_DEBUG
         MSPD_DBG("Dropping vectors at cell time(%d,%d):"
                  "ListCp count (%d) Head (%x) Tail (%x)\n",
                  cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
                  pUlDlList->count, pUlDlList->head, pUlDlList->tail);
#endif
        ysMsUtlFreePhyList(pUlDlList);
      }
     RETVALUE(ROK);
   }

   pMsgDesc = (PGENMSGDESC)(pUlDlList->head);
#ifdef CL_DEBUG
   MSPD_DBG("(%u, %u) head(%0x) count (%0x)\n",
            cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
            pUlDlList->head, pUlDlList->count);
#endif
   /* Drop the TX & RX vector we detect that we are late. 
    * This will be flushed in next TTI by Missed Sending Vector 
    * handler */
   endTti = GetTIMETICK();
   diff = YS_MS_TTI_DIFF(endTti, startTti);
   if (diff > YS_TTI)
   {
      MSPD_ERR("DatReq processing stretched though it got"
               " received before %f of TTI. Hence dropping"
               " the Vectors. Please try decrementing the YS_MAX_DAT_REQ_DELAY"
               " value based on DatReq processing time\n",
      YS_MAX_DAT_REQ_DELAY);
      RETVALUE(ROK);
   } 

   prev_sf  = timingInfo.subframe;
   prev_sfn = timingInfo.sfn;

   if(ysMsLimSendToPhy(sizeof(MAC2PHY_QUEUE_EL),pMsgDesc) != ROK)
   {
      /* HARQ_TODO: Free the uldlList before retruning from here */
      YS_DBG_ERR((_ysp, "Failed to send the TxVector"));
      MSPD_ERR("Failed to send the TxVector");
      RETVALUE(RFAILED);
   }

   /* Re-initialize the sent lists */
   sfNum = (timingInfo.subframe + (TFU_DELTA)) % YS_NUM_SUB_FRAMES;
   ysMsUtlInitPhyList(&cellCb->dlEncL1Msgs[sfNum].ulDlLst);
   ysMsUtlInitPhyList(&cellCb->dlEncL1Msgs[sfNum].dlData);
   ysMsUtlInitPhyList(&cellCb->dlEncL1Msgs[sfNum].dlCntrl);
   cellCb->dlEncL1Msgs[sfNum].txVector = NULLP;

   sfNum = (timingInfo.subframe + (TFU_ULCNTRL_DELTA)) % YS_NUM_SUB_FRAMES;
   ysMsUtlInitPhyList(&cellCb->dlEncL1Msgs[sfNum].ulCntrl);
   cellCb->dlEncL1Msgs[sfNum].numChannels = 0;

   sfNum = (timingInfo.subframe + (TFU_RECPREQ_DLDELTA )) % YS_NUM_SUB_FRAMES;      
   cellCb->ulEncL1Msgs[sfNum].rxVector = NULLP;
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_CL_SND_VECT_TO_PHY , RESOURCE_LARM, startTick, GetTIMETICK());
#endif
   RETVALUE(ROK);
}

/*
*
*       Fun:   ysMsDlmPrcTxEndInd
*
*       Desc:  TxEnd.Ind handler from PHY
*
*       Ret:   ROK/RFAILED
*
*       Notes: None
*
*       File:  ys_ms_dl.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsDlmPrcTxEndInd
(
PINITIND        txEndInd,
YsCellCb        *cellCb
)
#else
PUBLIC S16 ysMsDlmPrcTxEndInd(txEndInd, cellCb)
PINITIND        txEndInd;
YsCellCb        *cellCb;
#endif
{
   TRC2(ysMsDlmPrcTxEndInd)

   RETVALUE(ROK);
} /* End of ysMsDlmPrcTxEndInd */








/** Print LTE PHY API message
 *
 * @param level Bitmask specifying current debug level
 * @param vector Pointer to the PHY API message
 * @return Standard result code
 */
PUBLIC S16 DiagDumpPhyApi(PTR vector)
{
    PGENMSGDESC pGen = (PGENMSGDESC) vector;
    PDLSUBFRDESC pDlSf;
    PULSUBFRDESC pUlSf;
    PTXSDUREQ pTxSduReq;
    PRXSDUIND pRxSduInd;
    DLCHANDESC *pDlCh;
    ULCHANDESC *pUlCh;
    DLSUBCHINFO *pSubCh;
    ULSUBCHINFO *pUlSubCh;
#ifdef DEBUGP
    CRCINFO *pCrc;
    SCRAMBLERINFO *pScram;
    DLDEDICATEDPRECODERINFO *pPrecoder;
    DLCMNTXPWRCTL *pPwrCtl;
    PULSUBFRCMNCTRL pUlCmnCtrl;
    RACHCTRL *pRachCtrl;
    PINITPARM pInit;
    PMSGIND pMsgInd;
#endif
    MAPPINGINFO *pMapping;
    DCICHANNELDESC *pDciCh;
    ULCTRLCHDESC *pUlCtrlCh;
    SRSDED *pSrs;
    unsigned int i, j, k;
    unsigned char *pDat;


    YS_DBG_INFO((_ysp, "pGen->msgSpecific=%i\n", pGen->msgSpecific));
    YS_DBG_INFO((_ysp, "pGen->msgType=%i\n", pGen->msgType));
    YS_DBG_INFO((_ysp, "pGen->phyEntityId=%i\n", pGen->phyEntityId));

    switch (pGen->msgType)
    {
    case PHY_TXSTART_REQ:
        pDlSf = (PDLSUBFRDESC) (pGen + 1);

        YS_DBG_INFO((_ysp, "  pDlSf->frameNumber=%i\n", pDlSf->frameNumber));
        YS_DBG_INFO((_ysp, "  pDlSf->subframeNumber=%i\n", pDlSf->subframeNumber));
        YS_DBG_INFO((_ysp, "  pDlSf->subframeType=%i\n", pDlSf->subframeType));
        YS_DBG_INFO((_ysp, "  pDlSf->antennaPortcount=%i\n", pDlSf->antennaPortcount));
         YS_DBG_INFO((_ysp, "  pDlSf->numberofChannelDescriptors=%i\n",
                  pDlSf->numberofChannelDescriptors));
        YS_DBG_INFO((_ysp, "  pDlSf->offsetPowerCtrl=%i\n", pDlSf->offsetPowerCtrl));
         YS_DBG_INFO((_ysp, "  pDlSf->offsetDCIChannels=%i\n",
                  pDlSf->offsetDCIChannels));
         YS_DBG_INFO((_ysp, "  pDlSf->offsetCustomFeatures=%i\n",
                  pDlSf->offsetCustomFeatures));
        YS_DBG_INFO((_ysp, "  pDlSf->numCtrlSymbols=%i\n", pDlSf->numCtrlSymbols));
        YS_DBG_INFO((_ysp, "  pDlSf->phichResource=%i\n", pDlSf->phichResource));
        YS_DBG_INFO((_ysp, "  pDlSf->phichDuration=%i\n", pDlSf->phichDuration));
         YS_DBG_INFO((_ysp, "  pDlSf->numberOfPhichChannels=%i\n",
                  pDlSf->numberOfPhichChannels));
        YS_DBG_INFO((_ysp, "  pDlSf->numCtrlChannels=%i\n", pDlSf->numCtrlChannels));

        pDlCh = &pDlSf->dlCh[0];
        for (i = 0; i < pDlSf->numberofChannelDescriptors - pDlSf->numCtrlChannels; i++)
        {
            YS_DBG_INFO((_ysp, "CHANNEL %i:\n----------\n", i));
            YS_DBG_INFO((_ysp, "    pDlCh->offsetNextCh=%i\n", pDlCh->offsetNextCh));
            YS_DBG_INFO((_ysp, "    pDlCh->channelId=%i\n", pDlCh->channelId));
            YS_DBG_INFO((_ysp, "    pDlCh->txpowerControl=%i\n", pDlCh->txpowerControl));
            YS_DBG_INFO((_ysp, "    pDlCh->persistEnable=%i\n",
                     pDlCh->persistEnable));
            YS_DBG_INFO((_ysp, "    pDlCh->repeatCycle=%i\n", pDlCh->repeatCycle));
            YS_DBG_INFO((_ysp, "    pDlCh->channelType=%i\n", pDlCh->channelType));
            YS_DBG_INFO((_ysp, "    pDlCh->hCid=%i\n", pDlCh->hCid));
            YS_DBG_INFO((_ysp, "    pDlCh->numCodeWords=%i\n", pDlCh->numCodeWords));
            YS_DBG_INFO((_ysp, "    pDlCh->nLayers=%i\n", pDlCh->nLayers));
            YS_DBG_INFO((_ysp, "    pDlCh->transmissionMode=%i\n",
                     pDlCh->transmissionMode));

            pSubCh = &pDlCh->subChInfo[0];
            for (j = 0; j < pDlCh->numCodeWords; j++)
            {
                YS_DBG_INFO((_ysp, "SUBCHANNEL %i:\n----------\n", j));
                YS_DBG_INFO((_ysp, "      pSubCh->codingDescriptor=%i\n", pSubCh->codingDescriptor));
                YS_DBG_INFO((_ysp, "      pSubCh->blockCodeConcatenation=%i\n", pSubCh->blockCodeConcatenation));
                YS_DBG_INFO((_ysp, "      pSubCh->modulationType=%i\n", pSubCh->modulationType));
                YS_DBG_INFO((_ysp, "      pSubCh->mcsType=%i\n", pSubCh->mcsType));
                YS_DBG_INFO((_ysp, "      pSubCh->nDi=%i\n", pSubCh->nDi));
                YS_DBG_INFO((_ysp, "      pSubCh->rV=%i\n", pSubCh->rV));
                YS_DBG_INFO((_ysp, "      pSubCh->flushReq=%i\n", pSubCh->flushReq));

                pSubCh++;
            }

#ifdef DEBUGP
            pCrc = &pDlCh->crcInfo;
            YS_DBG_INFO((_ysp, "    pCrc->crcLength=%i\n", pCrc->crcLength));
            YS_DBG_INFO((_ysp, "    pCrc->crcScrambling=%i\n", pCrc->crcScrambling));

            pScram = &pDlCh->scrInfo;
            YS_DBG_INFO((_ysp, "    pScram->scramblerType=%i\n", pScram->scramblerType));
            YS_DBG_INFO((_ysp, "    pScram->scrinitValueinput=%i\n", pScram->scrinitValueinput));

            pPrecoder = &pDlCh->dlPrecoderInfo;
            YS_DBG_INFO((_ysp, "    pPrecoder->cddType=%i\n", pPrecoder->cddType));
            YS_DBG_INFO((_ysp, "    pPrecoder->codeBookIdx=%i\n", pPrecoder->codeBookIdx));
#endif

            pMapping = &pDlCh->mapInfo;
            YS_DBG_INFO((_ysp, "    pMapping->numberofEntries=%i\n", pMapping->numberofEntries));
            for (k = 0; k < pMapping->numberofEntries; k++)
            {
                YS_DBG_INFO((_ysp, "        pMapping->reselmInfo startRes=[%i]......",
                       pMapping->reselmInfo[k].startRes));
                YS_DBG_INFO((_ysp, "        pMapping->reselmInfo numRes=[%i]\n",
                       pMapping->reselmInfo[k].numRes));
            }

            pDlCh++;
        }

        pDciCh = (DCICHANNELDESC *) ((unsigned long) pDlSf + (unsigned long) pDlSf->offsetDCIChannels);
        for (i = 0; i < pDlSf->numCtrlChannels; i++)
        {
            YS_DBG_INFO((_ysp, "CTRL CHANNEL %i:\n---------------\n", i));
            YS_DBG_INFO((_ysp, "    pDciCh->offsetNextCh=%i\n", pDciCh->offsetNextCh));
            YS_DBG_INFO((_ysp, "    pDciCh->channelId=%i\n", pDciCh->channelId));
            YS_DBG_INFO((_ysp, "    pDciCh->txpowerControl=%i\n", pDciCh->txpowerControl));
            YS_DBG_INFO((_ysp, "    pDciCh->crcLength=%i\n", pDciCh->crcLength));
            YS_DBG_INFO((_ysp, "    pDciCh->crcScrambling=%i\n", pDciCh->crcScrambling));
            YS_DBG_INFO((_ysp, "    pDciCh->channelType=%i\n", pDciCh->channelType));
            YS_DBG_INFO((_ysp, "    pDciCh->numCodeWords=%i\n", pDciCh->numCodeWords));
            YS_DBG_INFO((_ysp, "    pDciCh->nLayers=%i\n", pDciCh->nLayers));
            YS_DBG_INFO((_ysp, "    pDciCh->transmissionMode=%i\n", pDciCh->transmissionMode));
            YS_DBG_INFO((_ysp, "    pDciCh->scrmblerInitValue=%i\n", pDciCh->scrmblerInitValue));
            YS_DBG_INFO((_ysp, "    pDciCh->numberofEntries=%i\n", pDciCh->numberofEntries));
            YS_DBG_INFO((_ysp, "    pDciCh->startRes=%i\n", pDciCh->startRes));
            YS_DBG_INFO((_ysp, "    pDciCh->numRes=%i\n", pDciCh->numRes));

            pDciCh++;
        }
#ifdef DEBUGP
        if (pDlSf->offsetPowerCtrl != 0)
        {
            pPwrCtl = (DLCMNTXPWRCTL*) ((unsigned long)pDlSf + (unsigned long)pDlSf->offsetPowerCtrl);
            YS_DBG_INFO((_ysp, "  pPwrCtl->pilotPower=%i\n", pPwrCtl->pilotPower));
            YS_DBG_INFO((_ysp, "  pPwrCtl->psyncPower=%i\n", pPwrCtl->psyncPower));
            YS_DBG_INFO((_ysp, "  pPwrCtl->ssyncPower=%i\n", pPwrCtl->ssyncPower));
            YS_DBG_INFO((_ysp, "  pPwrCtl->ciPower=%i\n", pPwrCtl->ciPower));
            YS_DBG_INFO((_ysp, "  pPwrCtl->paprControl=%i\n", pPwrCtl->paprControl));
            YS_DBG_INFO((_ysp, "  pPwrCtl->paprThreshold=%i\n", pPwrCtl->paprThreshold));
        }
#endif
        break;

    case PHY_TXSDU_REQ:
        {

           U32          msgIdx = 0;
           U16          numBytes;

           pTxSduReq = (PTXSDUREQ) pGen;
           YS_DBG_INFO((_ysp, "  pTxSdu->nackAck=%i\n", pTxSduReq->nackAck));
           YS_DBG_INFO((_ysp, "  pTxSdu->uciFormat=%i\n", pTxSduReq->uciFormat));
           YS_DBG_INFO((_ysp, "  pTxSdu->channelType=%i\n", pTxSduReq->channelType));
           YS_DBG_INFO((_ysp, "  pTxSdu->phichSeqIndex=%i\n", pTxSduReq->phichSeqIndex));
           YS_DBG_INFO((_ysp, "  pTxSdu->cwId=%i\n", pTxSduReq->cwId));
           YS_DBG_INFO((_ysp, "  pTxSdu->msgLen=%i\n", pTxSduReq->msgLen));
           YS_DBG_INFO((_ysp, "  pTxSdu->phichGrpNumber=%i\n", pTxSduReq->phichGrpNumber));
           YS_DBG_INFO((_ysp, "  pTxSdu->maxBitsperCw=%i\n", pTxSduReq->maxBitsperCw));
           numBytes = (pTxSduReq->maxBitsperCw / 8);
           if ((pTxSduReq->maxBitsperCw % 8) != 0)
           {
              numBytes++;
           }

           pDat = (unsigned char *) (pTxSduReq + 1);
           for (i = 0; i < numBytes;i++)
           {

              msgIdx += 4;
           }

           msgIdx++;
        }
        break;

         /*TODO:Commeting these for the time being, to avoid unnnecessary prints, uncomment it further */
    case PHY_RXSTART_REQ:
        pUlSf = (PULSUBFRDESC) (pGen + 1);

        YS_DBG_INFO((_ysp, "  pUlSf->frameNumber=%i\n", pUlSf->frameNumber));
        YS_DBG_INFO((_ysp, "  pUlSf->subframeNumber=%i\n", pUlSf->subframeNumber));
        YS_DBG_INFO((_ysp, "  pUlSf->subframeType=%i\n", pUlSf->subframeType));
        YS_DBG_INFO((_ysp, "  pUlSf->antennaPortcount=%i\n", pUlSf->antennaPortcount));
        YS_DBG_INFO((_ysp, "  pUlSf->numberofChannelDescriptors=%i\n", pUlSf->numberofChannelDescriptors));
        YS_DBG_INFO((_ysp, "  pUlSf->numberOfCtrlChannelDescriptors=%i\n", pUlSf->numberOfCtrlChannelDescriptors));
        YS_DBG_INFO((_ysp, "  pUlSf->numberSrsinSf=%i\n", pUlSf->numberSrsinSf));
        YS_DBG_INFO((_ysp, "  pUlSf->offsetRachCtrlStruct=%i\n", pUlSf->offsetRachCtrlStruct));
        YS_DBG_INFO((_ysp, "  pUlSf->offsetULCtrlChannels=%i\n", pUlSf->offsetULCtrlChannels));
        YS_DBG_INFO((_ysp, "  pUlSf->offsetsrsInfo=%i\n", pUlSf->offsetsrsInfo));
        YS_DBG_INFO((_ysp, "  pUlSf->offsetCustomFeatures=%i\n", pUlSf->offsetCustomFeatures));

#ifdef DEBUGP
        pUlCmnCtrl = &pUlSf->ulSfrCtrl;
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->deltaPUCCHShift=%i\n", pUlCmnCtrl->deltaPUCCHShift));
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->nRBCQI=%i\n", pUlCmnCtrl->nRBCQI));
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->nCSAn=%i\n", pUlCmnCtrl->nCSAn));
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->n1PucchAN=%i\n", pUlCmnCtrl->n1PucchAN));
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->srsBandwitdhConfig=%i\n", pUlCmnCtrl->srsBandwitdhConfig));
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->srsSubframeConfig=%i\n", pUlCmnCtrl->srsSubframeConfig));
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->ackNackSRSSimultaneousTransmission=%i\n", pUlCmnCtrl->ackNackSRSSimultaneousTransmission));
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->nSB=%i\n", pUlCmnCtrl->nSB));
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->hoppingMode=%i\n", pUlCmnCtrl->hoppingMode));
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->puschhoppingOffset=%i\n", pUlCmnCtrl->puschhoppingOffset));
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->enable64QAM=%i\n", pUlCmnCtrl->enable64QAM));
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->groupHoppingEnabled=%i\n", pUlCmnCtrl->groupHoppingEnabled));
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->groupAssignmentPUSCH=%i\n", pUlCmnCtrl->groupAssignmentPUSCH));
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->sequenceHoppingEnabled=%i\n", pUlCmnCtrl->sequenceHoppingEnabled));
        YS_DBG_INFO((_ysp, "    pUlCmnCtrl->cyclicShift=%i\n", pUlCmnCtrl->cyclicShift));
#endif

        pUlCh = &pUlSf->ulCh[0];
        for (i = 0; (int)i < (pUlSf->numberofChannelDescriptors - pUlSf->numberOfCtrlChannelDescriptors); i++)
        {
            YS_DBG_INFO((_ysp, "CHANNEL %i:\n----------\n", i));
            YS_DBG_INFO((_ysp, "    pUlCh->offsetNextCh=%i\n", pUlCh->offsetNextCh));
            YS_DBG_INFO((_ysp, "    pUlCh->channelId=%i\n", pUlCh->channelId));
            YS_DBG_INFO((_ysp, "    pUlCh->txpowerControl=%i\n", pUlCh->txpowerControl));
            YS_DBG_INFO((_ysp, "    pUlCh->persistEnable=%i\n", pUlCh->persistEnable));
            YS_DBG_INFO((_ysp, "    pUlCh->repeatCycle=%i\n", pUlCh->repeatCycle));
            YS_DBG_INFO((_ysp, "    pUlCh->channelType=%i\n", pUlCh->channelType));
            YS_DBG_INFO((_ysp, "    pUlCh->halfIterations=%i\n", pUlCh->halfIterations));

            pUlSubCh = &pUlCh->ulSubChInfo;
            YS_DBG_INFO((_ysp, "    pUlSubCh->mcinfo.codingDescriptor=%i\n", pUlSubCh->mcinfo.codingDescriptor));
            YS_DBG_INFO((_ysp, "    pUlSubCh->mcinfo.blockCodeConcatenation=%i\n", pUlSubCh->mcinfo.blockCodeConcatenation));
            YS_DBG_INFO((_ysp, "    pUlSubCh->mcinfo.modulationType=%i\n", pUlSubCh->mcinfo.modulationType));
            YS_DBG_INFO((_ysp, "    pUlSubCh->mcinfo.mcsType=%i\n", pUlSubCh->mcinfo.mcsType));
            YS_DBG_INFO((_ysp, "    pUlSubCh->harqInfo.nDi=%i\n", pUlSubCh->harqInfo.nDi));
            YS_DBG_INFO((_ysp, "    pUlSubCh->harqInfo.rV=%i\n", pUlSubCh->harqInfo.rV));
            YS_DBG_INFO((_ysp, "    pUlSubCh->harqInfo.flushReq=%i\n", pUlSubCh->harqInfo.flushReq));
            YS_DBG_INFO((_ysp, "    pUlSubCh->crcInfo.crcLength=%i\n", pUlSubCh->crcInfo.crcLength));
            YS_DBG_INFO((_ysp, "    pUlSubCh->crcInfo.crcScrambling=%i\n", pUlSubCh->crcInfo.crcScrambling));
            YS_DBG_INFO((_ysp, "    pUlSubCh->scrInfo.scramblerType=%i\n", pUlSubCh->scrInfo.scramblerType));
            YS_DBG_INFO((_ysp, "    pUlSubCh->scrInfo.scrinitValueinput=%i\n", pUlSubCh->scrInfo.scrinitValueinput));
            YS_DBG_INFO((_ysp, "    pUlSubCh->puschDed.betaOffsetACKIndex=%i\n", pUlSubCh->puschDed.betaOffsetACKIndex));
            YS_DBG_INFO((_ysp, "    pUlSubCh->puschDed.betaOffsetRIIndex=%i\n", pUlSubCh->puschDed.betaOffsetRIIndex));
            YS_DBG_INFO((_ysp, "    pUlSubCh->puschDed.betaOffsetCQIIndex=%i\n", pUlSubCh->puschDed.betaOffsetCQIIndex));
            YS_DBG_INFO((_ysp, "    pUlSubCh->cqiPmiRiRpt.cqiReportModeAperiodic=%i\n", pUlSubCh->cqiPmiRiRpt.cqiReportModeAperiodic));
            YS_DBG_INFO((_ysp, "    pUlSubCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset=%i\n", pUlSubCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset));
            YS_DBG_INFO((_ysp, "    pUlSubCh->cqiPmiRiRpt.cqiReportPeriodicEnable=%i\n", pUlSubCh->cqiPmiRiRpt.cqiReportPeriodicEnable));
            YS_DBG_INFO((_ysp, "    pUlSubCh->cqiPmiRiRpt.cqiPUCCHResourceIndex=%i\n", pUlSubCh->cqiPmiRiRpt.cqiPUCCHResourceIndex));
            YS_DBG_INFO((_ysp, "    pUlSubCh->cqiPmiRiRpt.cqipmiConfigIndex=%i\n", pUlSubCh->cqiPmiRiRpt.cqipmiConfigIndex));
            YS_DBG_INFO((_ysp, "    pUlSubCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic=%i\n", pUlSubCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic));
            YS_DBG_INFO((_ysp, "    pUlSubCh->cqiPmiRiRpt.K=%i\n", pUlSubCh->cqiPmiRiRpt.K));
            YS_DBG_INFO((_ysp, "    pUlSubCh->cqiPmiRiRpt.riConfigIndex=%i\n", pUlSubCh->cqiPmiRiRpt.riConfigIndex));
            YS_DBG_INFO((_ysp, "    pUlSubCh->cqiPmiRiRpt.simultaneousAckNackAndCQI=%i\n", pUlSubCh->cqiPmiRiRpt.simultaneousAckNackAndCQI));

            YS_DBG_INFO((_ysp, "    pUlSubCh->mapInfo.numberofEntries=%i\n", pUlSubCh->mapInfo.numberofEntries));
            for (j = 0; j < pUlSubCh->mapInfo.numberofEntries; j++)
            {
                YS_DBG_INFO((_ysp, "      pUlSubCh->mapInfo.reselmInfo[%i]=(%i,%i)\n", j,
                       pUlSubCh->mapInfo.reselmInfo[j].startRes,
                       pUlSubCh->mapInfo.reselmInfo[j].numRes));
            }
            pUlCh++;
        }
        pUlCtrlCh = &pUlSf->ulCtlCh[0];
        for (i = 0; i < pUlSf->numberOfCtrlChannelDescriptors; i++)
        {
            YS_DBG_INFO((_ysp, "CTRL CHANNEL %i:\n----------\n", i));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->offsetNextCh=%i\n", pUlCtrlCh->offsetNextCh));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->channelId=%i\n", pUlCtrlCh->channelId));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->txpowerControl=%i\n", pUlCtrlCh->txpowerControl));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->crcLength=%i\n", pUlCtrlCh->crcLength));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->channelType=%i\n", pUlCtrlCh->channelType));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->scrmblerInitValue=%i\n", pUlCtrlCh->scrmblerInitValue));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->codingDescriptor=%i\n", pUlCtrlCh->codingDescriptor));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->blockCodeConcatenation=%i\n", pUlCtrlCh->blockCodeConcatenation));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->modulationType=%i\n", pUlCtrlCh->modulationType));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->mcsType=%i\n", pUlCtrlCh->mcsType));

            YS_DBG_INFO((_ysp, "    pUlCtrlCh->pucchDedCtl.formatType=%i\n", pUlCtrlCh->pucchDedCtl.formatType));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->pucchDedCtl.ackNackRepetition=%i\n", pUlCtrlCh->pucchDedCtl.ackNackRepetition));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->pucchDedCtl.repetitionFactor=%i\n", pUlCtrlCh->pucchDedCtl.repetitionFactor));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->pucchDedCtl.n1PucchANRep=%i\n", pUlCtrlCh->pucchDedCtl.n1PucchANRep));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->pucchDedCtl.cqiPUCCHResourceIndex=%i\n", pUlCtrlCh->pucchDedCtl.cqiPUCCHResourceIndex));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->pucchDedCtl.srPUCCHResourceIndex=%i\n", pUlCtrlCh->pucchDedCtl.srPUCCHResourceIndex));

            YS_DBG_INFO((_ysp, "    pUlCtrlCh->cqiPmiRiRpt.cqiReportModeAperiodic=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqiReportModeAperiodic));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset=%i\n", pUlCtrlCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->cqiPmiRiRpt.cqiReportPeriodicEnable=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqiReportPeriodicEnable));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->cqiPmiRiRpt.cqiPUCCHResourceIndex=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqiPUCCHResourceIndex));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->cqiPmiRiRpt.cqipmiConfigIndex=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqipmiConfigIndex));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->cqiPmiRiRpt.K=%i\n", pUlCtrlCh->cqiPmiRiRpt.K));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->cqiPmiRiRpt.riConfigIndex=%i\n", pUlCtrlCh->cqiPmiRiRpt.riConfigIndex));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->cqiPmiRiRpt.simultaneousAckNackAndCQI=%i\n", pUlCtrlCh->cqiPmiRiRpt.simultaneousAckNackAndCQI));

            YS_DBG_INFO((_ysp, "    pUlCtrlCh->numberofEntries=%i\n", pUlCtrlCh->numberofEntries));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->startRes=%i\n", pUlCtrlCh->startRes));
            YS_DBG_INFO((_ysp, "    pUlCtrlCh->numRes=%i\n", pUlCtrlCh->numRes));

            pUlCtrlCh++;
        }

        pSrs = (SRSDED *)(((unsigned long)pUlSf) + pUlSf->offsetsrsInfo);
        for (i = 0; i < pUlSf->numberSrsinSf; i++)
        {
            YS_DBG_INFO((_ysp, "SRS %i:\n----------\n", i));
            YS_DBG_INFO((_ysp, "      pSrs->enableDisable=%i\n", pSrs->enableDisable));
            YS_DBG_INFO((_ysp, "      pSrs->srsBandwidth=%i\n", pSrs->srsBandwidth));
            YS_DBG_INFO((_ysp, "      pSrs->srsHoppingBandwidth=%i\n", pSrs->srsHoppingBandwidth));
            YS_DBG_INFO((_ysp, "      pSrs->transmissionComb=%i\n", pSrs->transmissionComb));
            YS_DBG_INFO((_ysp, "      pSrs->cyclicShift=%i\n", pSrs->cyclicShift));
            YS_DBG_INFO((_ysp, "      pSrs->freqDomainPosition=%i\n", pSrs->freqDomainPosition));
            YS_DBG_INFO((_ysp, "      pSrs->duration=%i\n", pSrs->duration));
            YS_DBG_INFO((_ysp, "      pSrs->srsConfigIndex=%i\n", pSrs->srsConfigIndex));

            pSrs++;
        }

#ifdef DEBUGP
        pRachCtrl = (RACHCTRL *)(((unsigned long)pUlSf) + pUlSf->offsetRachCtrlStruct);
        YS_DBG_INFO((_ysp, "    pRachCtrl->prachEnable=%i\n", pRachCtrl->prachEnable));
        YS_DBG_INFO((_ysp, "    pRachCtrl->rootSequenceIndex=%i\n", pRachCtrl->rootSequenceIndex));
        YS_DBG_INFO((_ysp, "    pRachCtrl->prachConfigIndex=%i\n", pRachCtrl->prachConfigIndex));
        YS_DBG_INFO((_ysp, "    pRachCtrl->highSpeedFlag=%i\n", pRachCtrl->highSpeedFlag));
        YS_DBG_INFO((_ysp, "    pRachCtrl->zeroCorrelationZoneConfig=%i\n", pRachCtrl->zeroCorrelationZoneConfig));
        YS_DBG_INFO((_ysp, "    pRachCtrl->prachFreqOffset=%i\n", pRachCtrl->prachFreqOffset));
#endif
        break;

    case PHY_RXSTART_IND:
#ifdef DEBUGP
        pMsgInd = (PMSGIND) pGen;
        YS_DBG_INFO((_ysp, "  pRxStartInd->subFrameNum=%i\n", pMsgInd->subFrameNum));
#endif
        break;

    case PHY_RXSDU_IND:
        pRxSduInd = (PRXSDUIND) pGen;
        YS_DBG_INFO((_ysp, "  pRxSdu->channelId=%i\n", pRxSduInd->channelId));
        YS_DBG_INFO((_ysp, "  pRxSdu->msgType=%i\n", pRxSduInd->msgType));
        YS_DBG_INFO((_ysp, "  pRxSdu->phyEntityId=%i\n", pRxSduInd->phyEntityId));
        YS_DBG_INFO((_ysp, "  pRxSdu->numBitsRx=%i\n", pRxSduInd->numBitsRx));
        YS_DBG_INFO((_ysp, "  pRxSdu->status=%i\n", pRxSduInd->status));
        YS_DBG_INFO((_ysp, "  pRxSdu->frameNum=%i\n", pRxSduInd->frameNum));
        YS_DBG_INFO((_ysp, "  pRxSdu->subFrameNum=%i\n", pRxSduInd->subFrameNum));
        YS_DBG_INFO((_ysp, "  pRxSdu->chanType=%i\n", pRxSduInd->chanType));
        YS_DBG_INFO((_ysp, "  pRxSdu->pRxSdu=%p\n", (void*)pRxSduInd->pRxSdu));

        // Currently payload starts at this field
        pDat = (unsigned char *) (&pRxSduInd->pRxSdu);
        for (i = 0; i < pRxSduInd->numBitsRx >> 3;)
        {
            for (j = 0; j < 16; j++)
            {
                YS_DBG_INFO((_ysp, "%02X ", pDat[j]));
                if (i + j + 1 >= (pRxSduInd->numBitsRx >> 3))
                    break;
            }
            for (j = 0; j < 16; j++)
            {
                //if (isprint(pDat[j]))
                if (TRUE)
					 {
                    YS_DBG_INFO((_ysp, "%c", pDat[j]));
					 }
                else
					 {
                    YS_DBG_INFO((_ysp, "."));
					 }
                if (i + j + 1 >= (pRxSduInd->numBitsRx >> 3))
                    break;
            }
            YS_DBG_INFO((_ysp, "\n"));
            i += 16;
            pDat += 16;

            // FIXME: This is debug check and to be removed
            if (i > 64)
                break;
        }
        break;

    case PHY_RXEND_IND:
#ifdef DEBUGP
        pMsgInd = (PMSGIND) pGen;
        YS_DBG_INFO((_ysp, "  pRxEndInd->subFrameNum=%i\n", pMsgInd->subFrameNum));
        YS_DBG_INFO((_ysp, "  pRxEndInd->frameNumber=%i\n", pMsgInd->frameNumber));
#endif
        break;

    case PHY_INIT_REQ:
#ifdef DEBUGP
        pInit = (PINITPARM) (pGen + 1);

        YS_DBG_INFO((_ysp, "  pInit->channelBandwidth=%i\n", pInit->channelBandwidth));
        YS_DBG_INFO((_ysp, "  pInit->frtypeDuplexmode=%i\n", pInit->frtypeDuplexmode));
        YS_DBG_INFO((_ysp, "  pInit->radioAccessMode=%i\n", pInit->radioAccessMode));
        YS_DBG_INFO((_ysp, "  pInit->physicalResourceBandwidth=%i\n", pInit->physicalResourceBandwidth));
        YS_DBG_INFO((_ysp, "  pInit->numberResourceBlocksperSlot=%i\n", pInit->numberResourceBlocksperSlot));
        YS_DBG_INFO((_ysp, "  pInit->phyLayerCellIdGroup=%i\n", pInit->phyLayerCellIdGroup));
        YS_DBG_INFO((_ysp, "  pInit->phyLayerId=%i\n", pInit->phyLayerId));
        YS_DBG_INFO((_ysp, "  pInit->txAntennaPortCount=%i\n", pInit->txAntennaPortCount));
        YS_DBG_INFO((_ysp, "  pInit->fastfwdorPreclockingNumber=%i\n", pInit->fastfwdorPreclockingNumber));
        YS_DBG_INFO((_ysp, "  pInit->fftSize=%i\n", pInit->fftSize));
        YS_DBG_INFO((_ysp, "  pInit->numberUsedSubcarriers=%i\n", pInit->numberUsedSubcarriers));
        YS_DBG_INFO((_ysp, "  pInit->nMaxDlRb=%i\n", pInit->nMaxDlRb));
        YS_DBG_INFO((_ysp, "  pInit->nMaxUlRb=%i\n", pInit->nMaxUlRb));
        YS_DBG_INFO((_ysp, "  pInit->referenceSignalPower=%i\n", pInit->referenceSignalPower));
        YS_DBG_INFO((_ysp, "  pInit->primarySyncSignalPower=%i\n", pInit->primarySyncSignalPower));
        YS_DBG_INFO((_ysp, "  pInit->secondarySyncSignalPower=%i\n", pInit->secondarySyncSignalPower));
        YS_DBG_INFO((_ysp, "  pInit->numDataRePerPRB=%i\n", pInit->numDataRePerPRB));
        YS_DBG_INFO((_ysp, "  pInit->cyclicPrefixType=%i\n", pInit->cyclicPrefixType));
        YS_DBG_INFO((_ysp, "  pInit->rxAntennaPortCount=%i\n", pInit->rxAntennaPortCount));
        YS_DBG_INFO((_ysp, "  pInit->pb=%i\n", pInit->pb));
        YS_DBG_INFO((_ysp, "  pInit->customExtensionReportEn=%i\n", pInit->customExtensionReportEn));
        YS_DBG_INFO((_ysp, "  pInit->rachReportMode=%i\n", pInit->rachReportMode));
        YS_DBG_INFO((_ysp, "  pInit->txSduConfEnable=%i\n", pInit->txSduConfEnable));
        YS_DBG_INFO((_ysp, "  pInit->txStartConfDisable =%i\n", pInit->txStartConfDisable));
        YS_DBG_INFO((_ysp, "  pInit->rxStartConfDisable =%i\n", pInit->rxStartConfDisable));
        YS_DBG_INFO((_ysp, "  pInit->sduConfig=%i\n", pInit->sduConfig));
        YS_DBG_INFO((_ysp, "  pInit->radioFrameNumber=%i\n", pInit->radioFrameNumber));
        YS_DBG_INFO((_ysp, "  pInit->subframeNumber=%i\n", pInit->subframeNumber));
        YS_DBG_INFO((_ysp, "  pInit->slotNumber=%i\n", pInit->slotNumber));
        YS_DBG_INFO((_ysp, "  pInit->srsBandwidthConfig=%i\n", pInit->srsBandwidthConfig));
        YS_DBG_INFO((_ysp, "  pInit->srsSubframeConfig=%i\n", pInit->srsSubframeConfig));
        YS_DBG_INFO((_ysp, "  pInit->srsSimultaneousANandSRS=%i\n", pInit->srsSimultaneousANandSRS));
        YS_DBG_INFO((_ysp, "  pInit->prachConfigurationIndex=%i\n", pInit->prachConfigurationIndex));
        YS_DBG_INFO((_ysp, "  pInit->prachFrequencyOffset=%i\n", pInit->prachFrequencyOffset));
        YS_DBG_INFO((_ysp, "  pInit->prachHighSpeedFlag=%i\n", pInit->prachHighSpeedFlag));
        YS_DBG_INFO((_ysp, "  pInit->prachCyclicShiftConfig=%i\n", pInit->prachCyclicShiftConfig));
        YS_DBG_INFO((_ysp, "  pInit->prachRootSequenceIndex=%i\n", pInit->prachRootSequenceIndex));
#endif
        break;

    default:
        //YS_DBG_INFO((_ysp, "Unknown command:\n----------------\n"));
        // Dump 64 bytes of unknown command
        pDat = (unsigned char *) vector;
        for (i = 0; i < 64; i += 16)
        {
            for (j = 0; j < 16; j++)
            {
                YS_DBG_INFO((_ysp, "%02X ", pDat[j]));
            }
            for (j = 0; j < 16; j++)
            {
                //if (isprint(pDat[j]))
                if (TRUE)
					 {
                    YS_DBG_INFO((_ysp, "%c", pDat[j]));
					 }
                else
					 {
                    YS_DBG_INFO((_ysp, "."));
					 }
            }
            YS_DBG_INFO((_ysp, "\n"));
            pDat += 16;
        }
        break;
    }
    return SUCCESS;
}

#ifdef CCPU_MLOG
#ifdef ANSI
PUBLIC Void L2PrintFunc (U8		*str)
#else
PUBLIC Void L2PrintFunc (str)
	U8		*str;
#endif
{
    YS_DBG_INFO((_ysp, " External Print=%s\n", str));
	 RETVOID;
}


#endif
#ifdef ANSI
PUBLIC Void ysMsCleanupTxRxVectors
(
YsCellCb        *cellCb,
U8               sfNum
)
#else
PUBLIC Void ysMsCleanupTxRxVectors (cellCb, sfNum)
YsCellCb        *cellCb;
U8               sfNum;
#endif
{
   U8           lclSf;

   lclSf = (sfNum + TFU_DELTA) % YS_NUM_SUB_FRAMES;
   ysMsUtlFreePhyList(&cellCb->dlEncL1Msgs[lclSf].ulDlLst);
   ysMsUtlInitPhyList(&cellCb->dlEncL1Msgs[lclSf].ulDlLst);
   ysMsUtlFreePhyList(&cellCb->dlEncL1Msgs[lclSf].dlData);
   ysMsUtlInitPhyList(&cellCb->dlEncL1Msgs[lclSf].dlData);
   ysMsUtlFreePhyList(&cellCb->dlEncL1Msgs[lclSf].dlCntrl);
   ysMsUtlInitPhyList(&cellCb->dlEncL1Msgs[lclSf].dlCntrl);
   cellCb->dlEncL1Msgs[lclSf].isdatReqPres = FALSE;
   cellCb->dlEncL1Msgs[lclSf].txVector = NULLP;
   cellCb->dlEncL1Msgs[lclSf].numChannels = 0;
   
   lclSf = (sfNum + TFU_ULCNTRL_DELTA) % YS_NUM_SUB_FRAMES;
   ysMsUtlFreePhyList(&cellCb->dlEncL1Msgs[lclSf].ulCntrl);
   ysMsUtlInitPhyList(&cellCb->dlEncL1Msgs[lclSf].ulCntrl);
   
   /* Add TFU_RECPREQ_DLDELTA to get RxVector subframe */
   lclSf = (sfNum + TFU_RECPREQ_DLDELTA) % YS_NUM_SUB_FRAMES;
   if (cellCb->ulEncL1Msgs[lclSf].rxVector != NULLP)
   {
      SvsrFreeMsg(cellCb->ulEncL1Msgs[lclSf].rxVector);
      cellCb->ulEncL1Msgs[lclSf].rxVector = NULLP;
   }

   /* Add TFU_RECPREQ_DLDELTA + TFU_CRCIND_ULDELTA to get CRC subframe */
   lclSf = (sfNum + TFU_RECPREQ_DLDELTA + TFU_CRCIND_ULDELTA) % YS_NUM_SUB_FRAMES; 
   cellCb->isCrcExptd[lclSf] = FALSE;
   
   //checkIcorePartitions();
//   MSPD_DBG("Cleaning Up ysMsCleanupTxRxVectors End\n");

   RETVOID;
}

PUBLIC Void ysMsPrcMissedVectors
(
YsCellCb       *cellCb,
U8              missedSf
)
{
   ysMsCleanupTxRxVectors(cellCb, missedSf);
}

/********************************************************************30**

         End of file:     ys_ms_dl.c@@/main/1 - Sun Mar 28 21:19:19 2010

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
/main/1      ---      pk   1. initial release.
/main/1    ys004.102  vr   1. Merged MSPD code with phy 1.7
*********************************************************************91*/

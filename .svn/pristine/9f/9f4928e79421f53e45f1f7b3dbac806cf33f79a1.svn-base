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

/************************************************************************

     Name:     LTE MAC Convergence Layer

     Type:     C source file

     Desc:     C source code for lower interface of Convergence Layer

     File:     ys_ms_li.c

     Sid:

     Prg:      rk

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
#include "ys_err.h"        /* CL error header file */
#include "tfu.h"
#include "ctf.h"
#include "lys.h"
#include "ys_ms.h"

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
//#include "supervisor.h"
#include "resultcodes.h"
#include "basetypes.h"

#include "ys_ms.x"

/*ys004.102 :  Merged MSPD code with phy 1.7 */
#ifdef YS_ALL_PERF
#include "hal.h"
#endif
extern U16 agFN;
extern U8  agSFN;
#ifdef SS_4GMX_LCORE
EXTERN S16 rbTmrHdlr ARGS((void));
#endif
#ifdef TTI_PROC_FIRST
PRIVATE S16 ysMsLimProcessMsg ARGS((U8 msgType, U16 entityId, Ptr msg, Bool isFree));
#else
PRIVATE S16 ysMsLimProcessMsg ARGS((U8 msgType, U16 entityId, Ptr msg));
#endif
PRIVATE Pst mainThreadPst;
PUBLIC RESULTCODE ysReceivePHYMsg ( U32   size, PTR   l1Msg);

EXTERN Void ysPerfPrntTimes (YsCellCb		*cellCb);

EXTERN S16 ysMsPrcRachInd ARGS((
			PRXSTATUSIND       pRxStatusInd,
			YsCellCb        	 *cellCb,
			TfuRaReqIndInfo 	 *rachIndInfo
			));
EXTERN S16 TraceMsMsg ARGS((PTR vector));

char *ysPhyAPIMsgStr[]={
"DUMMY_MSG_SHOULD_NEVER_APPEAR",
"PHY_TXSTART_REQ",
"PHY_TXSTART_CONF",
"PHY_TXSTART_IND",
"PHY_TXSDU_REQ",
"PHY_TXSDU_CONF",
"PHY_TXEND_IND",
"PHY_RXSTART_REQ",
"PHY_RXSTART_CONF",
"PHY_RXSTART_IND",
"PHY_RXSDU_IND",
"PHY_RXEND_IND",
"PHY_INIT_REQ",
"PHY_INIT_IND",
"PHY_RXSTATUS_IND",
"PHY_RECONFIG_REQ",
"PHY_RECONFIG_CNF",
"PHY_START_REQ",
"PHY_START_CONF",
"PHY_STOP_REQ",
"PHY_STOP_CONF"
};

#ifdef YS_MIB_WARND
/*variable introduced for temp testing */
extern Bool  acceptPrimsFrmMac;
#endif

#define IID_LTE_EX   38
#define SVSR_MSG_OPT_DEFAULT                (0 << 0)
char   ys_global_printbuf[256];
U32   dbgprntcnt;
extern U32 numTti;
#if 0
void ysPrintPHYAPIMsg(U8* msg, U32 size)
{
   PMSGHEADER pHdr;
   PGENMSGDESC pMsgDesc;
   int i;
   int dumpHexMsg = 0;

   pHdr = (PMSGHEADER)msg;
   pMsgDesc = (PGENMSGDESC)(msg+16);

   switch(pMsgDesc->msgType)
   {
      case PHY_INIT_IND:
         {
            PINITIND tmp = (PINITIND)(msg+16);
            break;
         }

      case PHY_TXSTART_IND:
         {
            PMSGIND tmp = (PMSGIND)(msg+16);
            break;
         }

      case PHY_TXSDU_REQ:
         {
            PTXSDUREQ tmp = (PTXSDUREQ)(msg+16);
            break;
         }

      case PHY_TXSDU_CONF:
         {
            PTXSDUIND tmp = (PTXSDUIND)(msg+16);
            break;
         }

      case PHY_RXSDU_IND:
         {
            PRXSDUIND tmp = (PRXSDUIND)(msg+16);
            break;
         }

      default:
         break;
   }

   RETVOID;
}
#endif
/*
*
*       Fun:   ysInitMainThreadPst
*
*       Desc:  This primitive is used initialize pst for self posting to YS
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_li.c`
*
*/

#ifdef ANSI
PUBLIC Void ysInitMainThreadPst
(
)
#else
PUBLIC Void ysInitMainThreadPst()
#endif
{
   TRC2(ysInitMainThreadPst)

   cmMemset((U8 *)&mainThreadPst, 0, sizeof(Pst));
   mainThreadPst.selector = 0;
   mainThreadPst.srcEnt = ENTTF;
   mainThreadPst.dstEnt = ENTTF;
   mainThreadPst.dstProcId = 0;
   mainThreadPst.srcProcId = 0;

   RETVOID;
}

/*
*
*       Fun:   MacDone
*
*       Desc:  Stop MAC
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_li.c`
*
*/

#ifdef ANSI
PUBLIC RESULTCODE MacDone
(
Void
)
#else
PUBLIC RESULTCODE MacDone(Void)
#endif
{
   RESULTCODE ret = SUCCESS;

   TRC2(MacDone)
/* This function need not do anything */
/* TODO: Needto call this function
    ret = LteMacDestroy();
  */

   RETVALUE(ret);
} /* end of MacDone */

#if 0
/*
*
*       Fun:   MacDispatch
*
*       Desc:  This primitive is used by PHY to provide uplink messages
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_li.c`
*
*/

#ifdef ANSI
PUBLIC RESULTCODE MacDispatch
(
U32   isList,
PTR   l1Msg
)
#else
PUBLIC RESULTCODE MacDispatch (isList, l1Msg)
U32   isList;
PTR   l1Msg;
#endif
{
	ysReceivePHYMsg(isList, l1Msg);

} /* end of MacDispatch */

/*
*
*       Fun:   ysReceivePHYMsg
*
*       Desc:  This primitive is used to receive msg from the receiver thread
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_li.c`
*
*/

#ifdef ANSI
PUBLIC RESULTCODE ysReceivePHYMsg
(
U32   isList,
PTR   l1Msg
)
#else
PUBLIC RESULTCODE ysReceivePHYMsg(isList,l1Msg)
U32   isList,
PTR   l1Msg
#endif
{
   S16            ret;
   PMSGHEADER     pHdr;
   PGENMSGDESC    pMsgDesc;

   TRC2(ysReceivePHYMsg)
/*
   SRemPreMsgMult((Data*)&size,4,buf);
   SRemPreMsgMult((Data*)&l1Msg,4,buf);
   SPutMsg(buf);
*/
   ret = ROK;
   if(l1Msg == NULLP)
   {
      YS_DBG_ERR((_ysp, "ysReceivePHYMsg(): Empty msg received \n"));
      RETVALUE(FAILURE);
   }
   if (!isList)
   {
       pMsgDesc = (PGENMSGDESC) l1Msg;
      /* sr118146: Using the configured cell id*/
#ifdef TTI_PROC_FIRST
      ret = ysMsLimProcessMsg(pMsgDesc->msgType, ysCb.cellId, pMsgDesc, FALSE);
#else
      ret = ysMsLimProcessMsg(pMsgDesc->msgType, ysCb.cellId, pMsgDesc);
#endif
      if(ret != ROK)
      {
         YS_DBG_ERR((_ysp, "ysReceivePHYMsg(): Failed to process message\n"));
         RETVALUE(FAILURE);
      }
      RETVALUE(SUCCESS);
   }

   /* Process the incoming list - free all nodes/msgs except the first node (the incoming pointer) */
   {
      MAC2PHY_QUEUE_EL *elem = (MAC2PHY_QUEUE_EL *)l1Msg;
      MAC2PHY_QUEUE_EL *next = NULL;

      pMsgDesc = (PGENMSGDESC)elem->MessagePtr;

      while (elem)
      {
         pMsgDesc = (PGENMSGDESC)elem->MessagePtr;

#ifdef TTI_PROC_FIRST
         ret = ysMsLimProcessMsg(pMsgDesc->msgType, ysCb.cellId, pMsgDesc, TRUE);
#else
         ret = ysMsLimProcessMsg(pMsgDesc->msgType, ysCb.cellId, pMsgDesc);
         SvsrFreeIMem(pMsgDesc);
#endif
         next = elem->Next;
         SvsrFreeIMem(elem);
         elem = next;

         if(ret != ROK)
         {
            YS_DBG_ERR((_ysp, "ysReceivePHYMsg(): Failed to process message\n"));
         }
      }


      RETVALUE(SUCCESS);
   }

} /* end of ysReceivePHYMsg */
#endif
int ysInitMSFramework()
{
    /* Initialize BSP */
    {
        /* Cannot do any further processing caller should initiate shutdown
         * of the enodeB
         */
        return FAILURE;
    }

    /* Initialize OSAL */
    {
        /* Cannot do any further processing caller should initiate shutdown
         * of the enodeB
         */
        return FAILURE;
    }


    return SUCCESS;
}


/*  added for, functions need to be filled in */
/** Set MAC config
 *  *
 *   * @param heap
 *    * @return Standard result code
 *     */
RESULTCODE MacSetInitConfig(void *h)
{

    return SUCCESS;
}

RESULTCODE AppInit(void *heap)
{
    return SUCCESS;
}

RESULTCODE MacRxEthPacket(PTR unused, U32 Len)
{
    return SUCCESS;
}

/* dummy functions to compile with MS BSP */
int sctp_send(int s, const void *msg, size_t len,
          const struct sctp_sndrcvinfo *sinfo, int flags)
{
   return 1;
}

U32	kwActvTmrStart;
U32	kwActvTmrEnd;
U32 kwTmrExpiryStart;
U32 kwTmrExpiryEnd;
U32 kwUmmReOrdTmrExpStart;
U32 kwUmmReOrdTmrExpEnd;
U32 kwUmmReassStart;
U32 kwUmmReassEnd;
Txt tmpKwDbg[128];
EXTERN S16 kwActvTmr();

int stopphy;
U32 ysGT;

#if 0
/*
*
*       Fun:   ysMsLimProcessMsg
*
*       Desc:  This primitive is used provide message to UL/DL modules
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_li.c`
*
*/
#ifdef TTI_PROC_FIRST
#ifdef ANSI
PRIVATE S16 ysMsLimProcessMsg
(
U8    msgType,
U16   entityId,
Ptr   msg,
Bool  isfree
)
#else
PRIVATE S16 ysMsLimProcessMsg (msgType, entityId, msg, isfree)
U8    msgType;
U16   entityId;
Ptr   msg;
Bool  isfree;
#endif
#else
#ifdef ANSI
PRIVATE S16 ysMsLimProcessMsg
(
U8    msgType,
U16   entityId,
Ptr   msg
)
#else
PRIVATE S16 ysMsLimProcessMsg (msgType, entityId, msg)
U8    msgType;
U16   entityId;
Ptr   msg;
#endif
#endif
{
   S16         ret;
   YsCellCb    *cellCb;
   U8			rxEndOffset;
   Txt	_prntBuf[256];
   U32  t1 = 0;
   U32  t2 = 0;
#ifdef TTI_PROC_FIRST
   CmLteTimingInfo rxtime;
   YsUlL1Msg *ulRxSduInds = NULLP;
   Bool isFree = isfree;
#endif

   /* crude workaround for now
    * when we receive the first MSGT_RESPONSE from the PHY we treat it like the
    * PHY_STOP_CONF and call ysMsLimProcessMsg (all other MSGT_RESPONSE msgs
    * will not be passed to the LimProcessMsg) to go ahead in the state machine
    * successfully. There is a bug in the PHY, and it sends two MSGT_RESPONSE
    * messages, the first with success and the second with failure on receiving
    * the PHY_START_REQ msg. Fix will be done in the PHY and then this
    * workaround can be removed.
    */
   static int workAroundOnlyOnce = 0;
	static int firstCall = TRUE;

	rxEndOffset = 2;

   TRC2(ysMsLimProcessMsg)

   if(msg == NULLP)
   {
      YS_DBG_ERR((_ysp, "ysMsLimProcessMsg(): Empty msg received \n"));
      RETVALUE(RFAILED);
   }
   ret = ROK;

   /* Get the cellCb */
   cellCb = (YsCellCb*)ysMsCfgGetCellCfg(entityId);
   if(cellCb == NULLP)
   {
      YS_DBG_ERR((_ysp, "ysMsLimProcessMsg(): cellCb is NULL\n"));
      RETVALUE(RFAILED);
   }
   YS_DBG_INFO((_ysp, "ysMsLimProcessMsg(): msgType=%d TTI (%d,%d)\n",
      msgType, cellCb->timingInfo.sfn, cellCb->timingInfo.subframe));

   switch(msgType)
   {
      case PHY_TXSTART_IND:
      {
         extern U32 robLogs[10];
         extern U32  startTti;
         static U32  prevTti = 0;
         U8       prntBuf[200];
         PMSGIND  pMsgInd = (PMSGIND)msg;
         MSGIND   rxStartInd;
         /* HARQ: RxEndInd spoofing removed */
         MSGIND   rxEndInd;
         /* fetch the TTI generated time from the API 
            as of now the last 4bytes in the PHY_TXSTART_IND */
         U32      l1Tti = *((U32 *)(pMsgInd + 1));
         U32      l2TtiStart;
         U8       lsf;
         U32      endTti ;
         U32      dlmTtiStart ;
         U32      dlmTtiEnd ;
         U32      rxEndStart ;
         U32      rxEndEnd ;
         U32      mtTmrStart ;
         U32      mtTmrEnd ;
         U32      rxStartStart ;
         U32      rxStartEnd ;
         U32      diffInMs;
         double   driftDiff = 0;
         double   l2ProcTime = 0;
         Bool     isDummyTti = FALSE;
         CmLteTimingInfo  lastsf;

         startTti = l1Tti;
         l2TtiStart = GetTIMETICK();
         if (stopphy)
            if (--stopphy == 0)
               stop_printf("stop\n");

         /* Time difference between the actual TTI generation at PHY and the 
          *  TTI received time at CL */
         driftDiff = YS_MS_TTI_DIFF(l2TtiStart, l1Tti);

         /* collecting statistics  for L2 Processing time for each TTI*/
         {
            /* Time taken by L2 to process the previous TTI */
            l2ProcTime = YS_MS_TTI_DIFF(l2TtiStart, prevTti);
            if (l2ProcTime > 1)
               if (l2ProcTime < 1.25)
                  robLogs[0]++;
               else if (l2ProcTime < 1.5)
                  robLogs[1]++;
               else if (l2ProcTime < 2)
                  robLogs[2]++;
               else
                  robLogs[3]++;
            prevTti = l2TtiStart;
         }
         /* Cheking whether the TTI received is within the acceptable
                 *  delay to start the processing at L2 so that it won't get stretched 
                 *  into next TTI. And this delay threshold is derived from the 
                 *  previous experiments */

         if (driftDiff > YS_TTI_START_MAX_DELAY)
         {
            MSPD_LOG("Marking as dummy TTI as it is picked late by (%f)ms."
                  " PrevSf cellTime(%d,%d)\n", driftDiff,
                  cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
            isDummyTti = TRUE;
            /* Updating Statistics */
            {
               robLogs[4]++;
            }
         }
         else
         {
            isDummyTti = FALSE;
         }
#ifdef YS_ALL_PERF
         if (cellCb->capPerfData == TRUE)
         {
#if YS_PHY_STOP_AUTO
         cellCb->perfInfo[cellCb->ttiIndex].ttiStartTick = GetTIMETICK();
#else
         cellCb->perfInfo.ttiStartTick = GetTIMETICK();
#endif
         }
		 
         if(cellCb->ttiIndex == 0)
         cellCb->perfInfo.ttiEndTick = cellCb->perfInfo.ttiStartTick ;

         sprintf(_prntBuf, "%d\t%d\t%d\t%f\t%d",
         cellCb->ttiIndex,cellCb->perfInfo.timingInfo.sfn, cellCb->perfInfo.timingInfo.subframe,
         ((cellCb->perfInfo.ttiStartTick - cellCb->ttiPrevTick)/150000.0),
         cellCb->perfInfo.avlMem);
         SPrint(_prntBuf);
#endif

         /* HARQ: RxEndInd spoofing removed */

         rxEndInd.subFrameNum = cellCb->timingInfo.subframe;
         rxEndInd.frameNumber = cellCb->timingInfo.sfn;

         lastsf.sfn = cellCb->timingInfo.sfn;
         lastsf.subframe = cellCb->timingInfo.subframe;
         /* HARQ: Increment the subframe number and sfn to mark new subframe */
         cellCb->timingInfo.subframe = (cellCb->timingInfo.subframe + 1) % YS_NUM_SUB_FRAMES;
         if (0 == cellCb->timingInfo.subframe)
         {
         cellCb->timingInfo.sfn = (cellCb->timingInfo.sfn + 1) % YS_NUM_SFN;
         }
         ysGT = cellCb->timingInfo.sfn * 10 + cellCb->timingInfo.subframe;
         lsf = prev_sf;
         if (lsf != 30 && lsf != lastsf.subframe)
         {
            U8 sf = (lsf + 1) % YS_NUM_SUB_FRAMES;
            MSPD_DBG("Missed sending Vectors, prevSf(%u)\n", lsf);
            cellCb->isCrcExptd[sf] = FALSE;
            ysMsPrcMissedVectors(cellCb, sf);
            prev_sf = lastsf.subframe;
         }

#ifndef YS_MS_NO_TA
         ysMsUtlTmrProc(cellCb);
#endif

         /* work around for no RX_START_IND and RX_END_IND */
         rxStartInd.status = pMsgInd->status;
         rxStartInd.phyEntityId = pMsgInd->phyEntityId;
         rxStartInd.reserved = pMsgInd->reserved;
         rxStartInd.msgType = PHY_RXSTART_IND;

         /* END indication should be 3 subframes behind TXSTART */
         /* HARQ: RxEndInd spoofing removed */

         if (rxEndInd.subFrameNum >= rxEndOffset)
         {
            rxEndInd.subFrameNum -= (rxEndOffset);
         }
         else
         {
            rxEndInd.subFrameNum = (10 + rxEndInd.subFrameNum - (rxEndOffset)) % 10;
         }
         rxEndInd.status = pMsgInd->status;
         rxEndInd.phyEntityId = pMsgInd->phyEntityId;
         rxEndInd.reserved = pMsgInd->reserved;
         rxEndInd.msgType = PHY_RXEND_IND;
         YS_DBG_INFO((_ysp, "Spoofing RxEnd Indication \n"));
         if (firstCall == FALSE)
         {
         rxEndStart = GetTIMETICK();
         ysMsUlmRachRxEndInd(&rxEndInd, cellCb);
         rxEndEnd = GetTIMETICK();

         }
         else
         {
         firstCall = FALSE;
         }


         YS_DBG_INFO((_ysp, "Received TxStart Indication cell Time (%d) (%d) TxStart Time (%d) (%d) \n",
         	cellCb->timingInfo.sfn, cellCb->timingInfo.subframe, pMsgInd->frameNumber, pMsgInd->subFrameNum));
         dlmTtiStart = GetTIMETICK();
         ret = ysMsDlmPrcTtiInd(pMsgInd, cellCb, isDummyTti);
         dlmTtiEnd = GetTIMETICK();

         /* Generate Timer to layers using TTI from for SS_TICK_COUNT*/
            /* SS_TICKS gives the number of TICKS per second towards
         * the cmProcTmr. */
         mtTmrStart = GetTIMETICK();
#ifdef SS_4GMX_LCORE
	 rbTmrHdlr();
#endif
         mtTmrEnd = GetTIMETICK();
         /* kwActvTmr();*/
         /* work around for no RX_START_IND */

         YS_DBG_INFO((_ysp, "Spoofing RxStart Indication \n"));
         rxStartInd.subFrameNum = cellCb->timingInfo.subframe;
         rxStartInd.frameNumber = cellCb->timingInfo.sfn;
         /* START indication should be one subframe behind RXEND */

         if (rxStartInd.subFrameNum >= (rxEndOffset+1))
         {
            rxStartInd.subFrameNum -= (rxEndOffset+1);
         }
         else
         {
            rxStartInd.subFrameNum = (10 + rxStartInd.subFrameNum - (rxEndOffset+1)) % 10;
         }
         rxStartStart = GetTIMETICK();
         ret = ysMsUlmRxStartInd(&rxStartInd, cellCb);
         rxStartEnd = GetTIMETICK();

#ifdef YS_ALL_PERF
         if (cellCb->capPerfData == TRUE)
         {
            U32 mem;
            SRegInfoShow(0,&mem);
#if YS_PHY_STOP_AUTO
            cellCb->perfInfo[cellCb->ttiIndex].ttiEndTick = GetTIMETICK();
            cellCb->perfInfo[cellCb->ttiIndex].timingInfo.sfn = cellCb->timingInfo.sfn;
            cellCb->perfInfo[cellCb->ttiIndex].timingInfo.subframe = cellCb->timingInfo.subframe;
            cellCb->perfInfo[cellCb->ttiIndex].avlMem = mem;
#else
            cellCb->perfInfo.ttiEndTick = GetTIMETICK();
            cellCb->perfInfo.timingInfo.sfn = cellCb->timingInfo.sfn;
            cellCb->perfInfo.timingInfo.subframe = cellCb->timingInfo.subframe;
            cellCb->perfInfo.avlMem = mem;
            ysPerfPrntTimes(cellCb);
            cellCb->ttiPrevTick = cellCb->perfInfo.ttiStartTick;
#endif
            //cellCb->ttiIndex = (cellCb->ttiIndex + 1) % YS_MS_MAX_PERF_TTI;

            cellCb->ttiIndex++;
#if YS_PHY_STOP_AUTO
            if (cellCb->ttiIndex == YS_MS_MAX_PERF_TTI)
            {
            cellCb->capPerfData = FALSE;
            }
#endif

         }
#endif

         endTti = GetTIMETICK();
         diffInMs = ((endTti - prevTti)/ 150000.0);
         if (diffInMs > 0.5)
         {
            sprintf(prntBuf, "Our Processing Time (%d) (%d) timeDiff (%f)\n",
                  cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
                  ((endTti - prevTti)/ 150000.0));
            SPrint(prntBuf);
            sprintf(prntBuf, "RX End (%f) \t TTI (%f) \t mt (%f) \n Rx Start (%f) \t kwActvTmr (%f) \n kwTmrExpiry (%f)\t ummExp (%f) \t kwUmmReAss (%f) \n",

                  ((rxEndEnd - rxEndStart)/ 150000.0),
                  ((dlmTtiEnd - dlmTtiStart)/ 150000.0),
                  ((mtTmrEnd - mtTmrStart)/ 150000.0),
                  ((rxStartEnd - rxStartStart)/ 150000.0),
                  ((kwActvTmrEnd - kwActvTmrStart)/ 150000.0),
                  ((kwTmrExpiryEnd - kwTmrExpiryStart)/ 150000.0),
                  ((kwUmmReOrdTmrExpEnd - kwUmmReOrdTmrExpStart)/ 150000.0),
                  ((kwUmmReassEnd - kwUmmReassStart)/ 150000.0)
						);
            SPrint(prntBuf);
			SPrint(tmpKwDbg);
			if (diffInMs > 1.0)
			{
               stop_printf("L2 Exceeded 1 Ms...Exiting.....\n");
			   exit(-1);
			}
         }
      break;
      }
      case PHY_RXSTART_CONF:
      {
         PINITIND pRxStartCfm = (PINITIND)msg;
         YS_DBG_INFO((_ysp, "Received RxStart Confirm \n"));
         ysMsUlmRxStartCnf(pRxStartCfm, cellCb);
         break;
      }
      case PHY_RXSTART_IND:/* Removed from MSPD*/
      {
         PMSGIND pMsgInd  = (PMSGIND)msg;
         YS_DBG_INFO((_ysp, "Received RxStart Indication \n"));
         ret = ysMsUlmRxStartInd(pMsgInd, cellCb);
         break;
      }
      case PHY_RXSDU_IND:
      {
         PRXSDUIND pRxSduInd = (PRXSDUIND)msg;
         t1 = GetTIMETICK();
         //YS_DBG_PARAM((_ysp, "Received RxSdu Indication at droppping it for now (%d) \n", cellCb->gotRACH));

#ifdef TTI_PROC_FIRST
	 rxtime.sfn = pRxSduInd->frameNum;
         rxtime.subframe = pRxSduInd->subFrameNum;
	     YS_INCR_TIMING_INFO(rxtime, 2);
         if (!isFree)
         {
	        stop_printf("\n RXSDU IND Single pointer RCVD and this should \
               not be freed in CL \n");
         }
         if ((cellCb->timingInfo.sfn == rxtime.sfn ) &&
			 (cellCb->timingInfo.subframe == rxtime.subframe))
         {

            ret = ysMsUlmSduInd(pRxSduInd, cellCb);
#ifdef MSPD_MLOG_NEW
	     MLogTask(PID_CL_RXSDU_IND,RESOURCE_LARM, t1, GetTIMETICK());
#endif
	 }
	 else
	 {
//           MSPD_DBG("\n SDUIND RCVD early at(%d,%d) celltime(%d,%d)",pRxSduInd->frameNum,pRxSduInd->subFrameNum, \
			   cellCb->timingInfo.sfn,cellCb->timingInfo.subframe );
	    ulRxSduInds = &cellCb->ulRxSduInds[rxtime.subframe];
            ulRxSduInds->sduIndCnt++;
            ulRxSduInds->pRxSduInd[(ulRxSduInds->sduIndCnt - 1)] = pRxSduInd;
            isFree = FALSE;
	 }
#else
         ret = ysMsUlmSduInd(pRxSduInd, cellCb);
#ifdef MSPD_MLOG_NEW
	     MLogTask(PID_CL_RXSDU_IND,RESOURCE_LARM, t1, GetTIMETICK());
#endif
#endif

         break;
      }
      case PHY_RXEND_IND:
      {
         PMSGIND pRxEndInd = (PMSGIND)msg;
         t2 = GetTIMETICK();
#ifdef TTI_PROC_FIRST
	 rxtime.sfn = pRxEndInd->frameNumber;
         rxtime.subframe = pRxEndInd->subFrameNum;
	 YS_INCR_TIMING_INFO(rxtime, 2);
         if (!isFree)
         {
		 stop_printf("\n RXEND IND Single pointer RCVD and this should \
                 not be freed in CL \n");
         }

         if ((cellCb->timingInfo.sfn == rxtime.sfn ) &&
			 (cellCb->timingInfo.subframe == rxtime.subframe))
         {		 
         ysMsUlmRxEndInd(pRxEndInd, cellCb);
         {
		    U32 rxEndtime[4];
		    rxEndtime[0] = cellCb->timingInfo.sfn;
		    rxEndtime[1] = cellCb->timingInfo.subframe;
		    rxEndtime[2] = pRxEndInd->frameNumber;
		    rxEndtime[3] = pRxEndInd->subFrameNum;
#ifdef MSPD_MLOG_NEW
		       MLogAddVariables(4, rxEndtime, t2);
		       MLogTask(PID_CL_RXEND_IND,RESOURCE_LARM, t2, GetTIMETICK());
#endif
         }
      }
	 else
	 {
//           MSPD_DBG("\n RXENDIND RCVD early at(%d,%d) celltime(%d,%d)",pRxEndInd->frameNumber,pRxEndInd->subFrameNum, \
			   cellCb->timingInfo.sfn,cellCb->timingInfo.subframe );
	    ulRxSduInds = &cellCb->ulRxSduInds[rxtime.subframe];
	    ulRxSduInds->pRxEndInd	 =  pRxEndInd;
	    isFree = FALSE;
	 }
#else
	      ysMsUlmRxEndInd(pRxEndInd, cellCb);
	      {
		     U32 rxEndtime[4];
		     rxEndtime[0] = cellCb->timingInfo.sfn;
		     rxEndtime[1] = cellCb->timingInfo.subframe;
		     rxEndtime[2] = pRxEndInd->frameNumber;
		     rxEndtime[3] = pRxEndInd->subFrameNum;
#ifdef MSPD_MLOG_NEW
		     MLogAddVariables(4, rxEndtime, t2);
		     MLogTask(PID_CL_RXEND_IND,RESOURCE_LARM, t2, GetTIMETICK());
#endif
	      }
#endif
      break;
      }
      case PHY_INIT_IND:
      {
         PINITIND pInitInd = (PINITIND)msg;
         YS_DBG_INFO((_ysp, "Received Phy Init Indication \n"));
         ret = ysMsCfgSm(cellCb, YS_MS_EVENT_CFG_RSP, pInitInd);
         break;
      }
      case PHY_START_CONF:
      {
         PINITIND pInitInd = (PINITIND)msg;
         YS_DBG_INFO((_ysp, "Received Phy Start Confirm\n"));

         ret = ysMsCfgSm(cellCb, YS_MS_EVENT_START_RSP, pInitInd);
         break;
      }
      case PHY_STOP_CONF:
      {
         PINITIND pInitInd = (PINITIND)msg;
         YS_DBG_INFO((_ysp, "Received Phy Stop Confirm\n"));
            /* fix for stop_rsp - value is 6 however we can accomodate only 5 */
         ret = ysMsCfgSm(cellCb, (YS_MS_EVENT_STOP_RSP - 1), pInitInd);
         break;
      }
      case PHY_RXSTATUS_IND:
      {
         PRXSTATUSIND 	 pRxStatusInd = (PRXSTATUSIND)msg;
		 ysMsPrcRxStatusInd(pRxStatusInd, cellCb);

      }
      break;
      case PHY_RECONFIG_CNF:
      case PHY_TXSDU_CONF:
      case PHY_TXEND_IND:
      YS_DBG_INFO((_ysp, "ysMsLimProcessMsg(): Not handling msg type %d\n", msgType));
      break;

      default:
      ret = RFAILED;
      YS_DBG_ERR((_ysp, "ysMsLimProcessMsg(): Invalid msg type %d\n", msgType));
      break;
   }

   if(ret != ROK)
   {
      YS_DBG_ERR((_ysp, "ysMsLimProcessMsg(): Failed to process message\n"));
      RETVALUE(RFAILED);
   }
#ifdef TTI_PROC_FIRST
   if (isFree)
   {
      SvsrFreeIMem(msg);
   }
#endif

   RETVALUE(ROK);
} /* end of ysMsLimProcessMsg */

#ifdef ANSI
PUBLIC S16 TraceMsMsg
(
PTR vector
)
#else
PUBLIC S16 TraceMsMsg(vector)
(
PTR vector;
)
#endif
{
   PGENMSGDESC pGen = (PGENMSGDESC) vector;
   PDLSUBFRDESC pDlSf;
   PTXSDUREQ pTxSduReq;
   DLCHANDESC *pDlCh;
   DLSUBCHINFO *pSubCh;
   CRCINFO *pCrc;
   SCRAMBLERINFO *pScram;
   DLDEDICATEDPRECODERINFO *pPrecoder;
   MAPPINGINFO *pMapping;
   DCICHANNELDESC *pDciCh;
   DLCMNTXPWRCTL *pPwrCtl;
   unsigned int i, j, k;
   unsigned char *pDat;
    PINITPARM pInit;
    PRXSDUIND pRxSduInd;
    PMSGIND pMsgInd;
    RACHCTRL *pRachCtrl;
    SRSDED *pSrs;
    PULSUBFRCMNCTRL pUlCmnCtrl;
    ULCTRLCHDESC *pUlCtrlCh;
    ULCHANDESC *pUlCh;
    ULSUBCHINFO *pUlSubCh;
    PULSUBFRDESC pUlSf;
    PHIADCIULMSGDESC pHiDci0Vector;
    PHIINFOMSGDESC pHiSdu;
    PDCIULSDUMSG pDci0Sdu;

   uart_printf("pGen->msgSpecific=%i\n", pGen->msgSpecific);
   uart_printf("pGen->msgType=%i\n", pGen->msgType);
   uart_printf("pGen->phyEntityId=%i\n", pGen->phyEntityId);

   switch (pGen->msgType)
   {
      case PHY_TXSTART_REQ:
         pDlSf = (PDLSUBFRDESC) (pGen + 1);

         uart_printf("  pDlSf->frameNumber=%i\n", pDlSf->frameNumber);
         uart_printf("  pDlSf->subframeNumber=%i\n", pDlSf->subframeNumber);
         uart_printf("  pDlSf->subframeType=%i\n", pDlSf->subframeType);
         uart_printf("  pDlSf->antennaPortcount=%i\n", pDlSf->antennaPortcount);
         uart_printf("  pDlSf->numberofChannelDescriptors=%i\n", pDlSf->numberofChannelDescriptors);
         uart_printf("  pDlSf->offsetPowerCtrl=%i\n", pDlSf->offsetPowerCtrl);
         uart_printf("  pDlSf->offsetDCIChannels=%i\n", pDlSf->offsetDCIChannels);
         uart_printf("  pDlSf->offsetCustomFeatures=%i\n", pDlSf->offsetCustomFeatures);
         uart_printf("  pDlSf->numCtrlSymbols=%i\n", pDlSf->numCtrlSymbols);
         uart_printf("  pDlSf->phichResource=%i\n", pDlSf->phichResource);
         uart_printf("  pDlSf->phichDuration=%i\n", pDlSf->phichDuration);
         uart_printf("  pDlSf->numberOfPhichChannels=%i\n", pDlSf->numberOfPhichChannels);
         uart_printf("  pDlSf->numCtrlChannels=%i\n", pDlSf->numCtrlChannels);

         pDlCh = &pDlSf->dlCh[0];
         for (i = 0; i < pDlSf->numberofChannelDescriptors - pDlSf->numCtrlChannels; i++)
         {
            uart_printf("CHANNEL %i:\n----------\n", i);
            uart_printf("    pDlCh->offsetNextCh=%i\n", pDlCh->offsetNextCh);
            uart_printf("    pDlCh->channelId=%i\n", pDlCh->channelId);
            uart_printf("    pDlCh->txpowerControl=%i\n", pDlCh->txpowerControl);
            uart_printf("    pDlCh->persistEnable=%i\n", pDlCh->persistEnable);
            uart_printf("    pDlCh->repeatCycle=%i\n", pDlCh->repeatCycle);
            uart_printf("    pDlCh->channelType=%i\n", pDlCh->channelType);
            uart_printf("    pDlCh->hCid=%i\n", pDlCh->hCid);
            uart_printf("    pDlCh->numCodeWords=%i\n", pDlCh->numCodeWords);
            uart_printf("    pDlCh->nLayers=%i\n", pDlCh->nLayers);
            uart_printf("    pDlCh->transmissionMode=%i\n", pDlCh->transmissionMode);

            pSubCh = &pDlCh->subChInfo[0];
            for (j = 0; j < pDlCh->numCodeWords; j++)
            {
               uart_printf("SUBCHANNEL %i:\n----------\n", j);
               uart_printf("      pSubCh->codingDescriptor=%i\n", pSubCh->codingDescriptor);
               uart_printf("      pSubCh->blockCodeConcatenation=%i\n", pSubCh->blockCodeConcatenation);
               uart_printf("      pSubCh->modulationType=%i\n", pSubCh->modulationType);
               uart_printf("      pSubCh->mcsType=%i\n", pSubCh->mcsType);
               uart_printf("      pSubCh->nDi=%i\n", pSubCh->nDi);
               uart_printf("      pSubCh->rV=%i\n", pSubCh->rV);
               uart_printf("      pSubCh->flushReq=%i\n", pSubCh->flushReq);

               pSubCh++;
            }

            pCrc = &pDlCh->crcInfo;
            uart_printf("    pCrc->crcLength=%i\n", pCrc->crcLength);
            uart_printf("    pCrc->crcScrambling=%i\n", pCrc->crcScrambling);

            pScram = &pDlCh->scrInfo;
            uart_printf("    pScram->scramblerType=%i\n", pScram->scramblerType);
            uart_printf("    pScram->scrinitValueinput=%i\n", pScram->scrinitValueinput);

            pPrecoder = &pDlCh->dlPrecoderInfo;
            uart_printf("    pPrecoder->cddType=%i\n", pPrecoder->cddType);
            uart_printf("    pPrecoder->codeBookIdx=%i\n", pPrecoder->codeBookIdx);
            uart_printf("    pPrecoder->codebookSubsetRestriction[]=[%i %i %i %i %i %i %i %i]\n",
                  pPrecoder->codebookSubsetRestriction[0],
                  pPrecoder->codebookSubsetRestriction[1],
                  pPrecoder->codebookSubsetRestriction[2],
                  pPrecoder->codebookSubsetRestriction[3],
                  pPrecoder->codebookSubsetRestriction[4],
                  pPrecoder->codebookSubsetRestriction[5],
                  pPrecoder->codebookSubsetRestriction[6],
                  pPrecoder->codebookSubsetRestriction[7]);

            pMapping = &pDlCh->mapInfo;
            uart_printf("    pMapping->numberofEntries=%i\n", pMapping->numberofEntries);
            for (k = 0; k < pMapping->numberofEntries; k++)
            {
               uart_printf("        pMapping->reselmInfo=[%i,%i]\n",
                     pMapping->reselmInfo[k].startRes,
                     pMapping->reselmInfo[k].numRes);
            }

            pDlCh++;
         }

         pDciCh = (DCICHANNELDESC *) ((unsigned long) pDlSf + (unsigned long) pDlSf->offsetDCIChannels);
         for (i = 0; i < pDlSf->numCtrlChannels; i++)
         {
            uart_printf("CTRL CHANNEL %i:\n---------------\n", i);
            uart_printf("    pDciCh->offsetNextCh=%i\n", pDciCh->offsetNextCh);
            uart_printf("    pDciCh->channelId=%i\n", pDciCh->channelId);
            uart_printf("    pDciCh->txpowerControl=%i\n", pDciCh->txpowerControl);
            uart_printf("    pDciCh->crcLength=%i\n", pDciCh->crcLength);
            uart_printf("    pDciCh->crcScrambling=%i\n", pDciCh->crcScrambling);
            uart_printf("    pDciCh->channelType=%i\n", pDciCh->channelType);
            uart_printf("    pDciCh->numCodeWords=%i\n", pDciCh->numCodeWords);
            uart_printf("    pDciCh->nLayers=%i\n", pDciCh->nLayers);
            uart_printf("    pDciCh->transmissionMode=%i\n", pDciCh->transmissionMode);
            uart_printf("    pDciCh->scrmblerInitValue=%i\n", pDciCh->scrmblerInitValue);
            uart_printf("    pDciCh->numberofEntries=%i\n", pDciCh->numberofEntries);
            uart_printf("    pDciCh->startRes=%i\n", pDciCh->startRes);
            uart_printf("    pDciCh->numRes=%i\n", pDciCh->numRes);

            pDciCh++;
         }
         if (pDlSf->offsetPowerCtrl != 0)
         {
            pPwrCtl = (DLCMNTXPWRCTL*) ((unsigned long)pDlSf + (unsigned long)pDlSf->offsetPowerCtrl);
            uart_printf("    pPwrCtl->pilotPower=%i\n", pPwrCtl->pilotPower);
            uart_printf("    pPwrCtl->psyncPower=%i\n", pPwrCtl->psyncPower);
            uart_printf("    pPwrCtl->ssyncPower=%i\n", pPwrCtl->ssyncPower);
            uart_printf("    pPwrCtl->ciPower=%i\n", pPwrCtl->ciPower);
//            uart_printf("    pPwrCtl->paprControl=%i\n", pPwrCtl->paprControl);
//            uart_printf("    pPwrCtl->paprThreshold=%i\n", pPwrCtl->paprThreshold);
         }
         break;
      case PHY_TXSDU_REQ:
         pTxSduReq = (PTXSDUREQ) pGen;
         uart_printf("  pTxSdu->nackAck=%i\n", pTxSduReq->nackAck);
         uart_printf("  pTxSdu->uciFormat=%i\n", pTxSduReq->uciFormat);
         uart_printf("  pTxSdu->channelType=%i\n", pTxSduReq->channelType);
         uart_printf("  pTxSdu->phichSeqIndex=%i\n", pTxSduReq->phichSeqIndex);
         uart_printf("  pTxSdu->cwId=%i\n", pTxSduReq->cwId);
         uart_printf("  pTxSdu->msgLen=%i\n", pTxSduReq->msgLen);
         uart_printf("  pTxSdu->phichGrpNumber=%i\n", pTxSduReq->phichGrpNumber);
         uart_printf("  pTxSdu->maxBitsperCw=%i\n", pTxSduReq->maxBitsperCw);

         pDat = (unsigned char *) (pTxSduReq + 1);
         for (i = 0; i < pTxSduReq->msgLen;)
         {
            for (j = 0; j < 16; j++)
            {
               uart_printf("%02X ", pDat[j]);
               if (i + j + 1 >= pTxSduReq->msgLen)
                  break;
            }
            for (j = 0; j < 16; j++)
            {
               if (isprint(pDat[j]))
                  uart_printf("%c", pDat[j]);
               else
                  uart_printf(".");
               if (i + j + 1 >= pTxSduReq->msgLen)
                  break;
            }
            uart_printf("\n");
            i += 16;
            pDat += 16;

            // FIXME: // This // is // debug // check // and // to // be // removed
            if (i > 64)
               break;
         }
         break;
    case PHY_RXSTART_REQ:
        pUlSf = (PULSUBFRDESC) (pGen + 1);

        uart_printf("  pUlSf->frameNumber=%i\n", pUlSf->frameNumber);
        uart_printf("  pUlSf->subframeNumber=%i\n", pUlSf->subframeNumber);
        uart_printf("  pUlSf->subframeType=%i\n", pUlSf->subframeType);
        uart_printf("  pUlSf->antennaPortcount=%i\n", pUlSf->antennaPortcount);
        uart_printf("  pUlSf->numberofChannelDescriptors=%i\n", pUlSf->numberofChannelDescriptors);
        uart_printf("  pUlSf->numberOfCtrlChannelDescriptors=%i\n", pUlSf->numberOfCtrlChannelDescriptors);
        uart_printf("  pUlSf->numberSrsinSf=%i\n", pUlSf->numberSrsinSf);
        uart_printf("  pUlSf->offsetRachCtrlStruct=%i\n", pUlSf->offsetRachCtrlStruct);
        uart_printf("  pUlSf->offsetULCtrlChannels=%i\n", pUlSf->offsetULCtrlChannels);
        uart_printf("  pUlSf->offsetsrsInfo=%i\n", pUlSf->offsetsrsInfo);
        uart_printf("  pUlSf->offsetCustomFeatures=%i\n", pUlSf->offsetCustomFeatures);

        pUlCmnCtrl = &pUlSf->ulSfrCtrl;
        uart_printf("    pUlCmnCtrl->deltaPUCCHShift=%i\n", pUlCmnCtrl->deltaPUCCHShift);
        uart_printf("    pUlCmnCtrl->nRBCQI=%i\n", pUlCmnCtrl->nRBCQI);
        uart_printf("    pUlCmnCtrl->nCSAn=%i\n", pUlCmnCtrl->nCSAn);
        uart_printf("    pUlCmnCtrl->n1PucchAN=%i\n", pUlCmnCtrl->n1PucchAN);
        uart_printf("    pUlCmnCtrl->srsBandwitdhConfig=%i\n", pUlCmnCtrl->srsBandwitdhConfig);
        uart_printf("    pUlCmnCtrl->srsSubframeConfig=%i\n", pUlCmnCtrl->srsSubframeConfig);
        uart_printf("    pUlCmnCtrl->ackNackSRSSimultaneousTransmission=%i\n", pUlCmnCtrl->ackNackSRSSimultaneousTransmission);
        uart_printf("    pUlCmnCtrl->nSB=%i\n", pUlCmnCtrl->nSB);
        uart_printf("    pUlCmnCtrl->hoppingMode=%i\n", pUlCmnCtrl->hoppingMode);
        uart_printf("    pUlCmnCtrl->puschhoppingOffset=%i\n", pUlCmnCtrl->puschhoppingOffset);
        uart_printf("    pUlCmnCtrl->enable64QAM=%i\n", pUlCmnCtrl->enable64QAM);
        uart_printf("    pUlCmnCtrl->groupHoppingEnabled=%i\n", pUlCmnCtrl->groupHoppingEnabled);
        uart_printf("    pUlCmnCtrl->groupAssignmentPUSCH=%i\n", pUlCmnCtrl->groupAssignmentPUSCH);
        uart_printf("    pUlCmnCtrl->sequenceHoppingEnabled=%i\n", pUlCmnCtrl->sequenceHoppingEnabled);
        uart_printf("    pUlCmnCtrl->cyclicShift=%i\n", pUlCmnCtrl->cyclicShift);

        pUlCh = &pUlSf->ulCh[0];
        for (i = 0; (int)i < (pUlSf->numberofChannelDescriptors - pUlSf->numberOfCtrlChannelDescriptors); i++)
        {
            uart_printf("CHANNEL %i:\n----------\n", i);
            uart_printf("    pUlCh->offsetNextCh=%i\n", pUlCh->offsetNextCh);
            uart_printf("    pUlCh->channelId=%i\n", pUlCh->channelId);
            uart_printf("    pUlCh->txpowerControl=%i\n", pUlCh->txpowerControl);
            uart_printf("    pUlCh->persistEnable=%i\n", pUlCh->persistEnable);
            uart_printf("    pUlCh->repeatCycle=%i\n", pUlCh->repeatCycle);
            uart_printf("    pUlCh->channelType=%i\n", pUlCh->channelType);
            uart_printf("    pUlCh->halfIterations=%i\n", pUlCh->halfIterations);

            pUlSubCh = &pUlCh->ulSubChInfo;
            uart_printf("    pUlSubCh->mcinfo.codingDescriptor=%i\n", pUlSubCh->mcinfo.codingDescriptor);
            uart_printf("    pUlSubCh->mcinfo.blockCodeConcatenation=%i\n", pUlSubCh->mcinfo.blockCodeConcatenation);
            uart_printf("    pUlSubCh->mcinfo.modulationType=%i\n", pUlSubCh->mcinfo.modulationType);
            uart_printf("    pUlSubCh->mcinfo.mcsType=%i\n", pUlSubCh->mcinfo.mcsType);
            uart_printf("    pUlSubCh->harqInfo.nDi=%i\n", pUlSubCh->harqInfo.nDi);
            uart_printf("    pUlSubCh->harqInfo.rV=%i\n", pUlSubCh->harqInfo.rV);
            uart_printf("    pUlSubCh->harqInfo.flushReq=%i\n", pUlSubCh->harqInfo.flushReq);
            uart_printf("    pUlSubCh->crcInfo.crcLength=%i\n", pUlSubCh->crcInfo.crcLength);
            uart_printf("    pUlSubCh->crcInfo.crcScrambling=%i\n", pUlSubCh->crcInfo.crcScrambling);
            uart_printf("    pUlSubCh->scrInfo.scramblerType=%i\n", pUlSubCh->scrInfo.scramblerType);
            uart_printf("    pUlSubCh->scrInfo.scrinitValueinput=%i\n", pUlSubCh->scrInfo.scrinitValueinput);
            uart_printf("    pUlSubCh->puschDed.betaOffsetACKIndex=%i\n", pUlSubCh->puschDed.betaOffsetACKIndex);
            uart_printf("    pUlSubCh->puschDed.betaOffsetRIIndex=%i\n", pUlSubCh->puschDed.betaOffsetRIIndex);
            uart_printf("    pUlSubCh->puschDed.betaOffsetCQIIndex=%i\n", pUlSubCh->puschDed.betaOffsetCQIIndex);
            uart_printf("    pUlSubCh->cqiPmiRiRpt.cqiReportModeAperiodic=%i\n", pUlSubCh->cqiPmiRiRpt.cqiReportModeAperiodic);
            uart_printf("    pUlSubCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset=%i\n", pUlSubCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset);
            uart_printf("    pUlSubCh->cqiPmiRiRpt.cqiReportPeriodicEnable=%i\n", pUlSubCh->cqiPmiRiRpt.cqiReportPeriodicEnable);
            uart_printf("    pUlSubCh->cqiPmiRiRpt.cqiPUCCHResourceIndex=%i\n", pUlSubCh->cqiPmiRiRpt.cqiPUCCHResourceIndex);
            uart_printf("    pUlSubCh->cqiPmiRiRpt.cqipmiConfigIndex=%i\n", pUlSubCh->cqiPmiRiRpt.cqipmiConfigIndex);
            uart_printf("    pUlSubCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic=%i\n", pUlSubCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic);
            uart_printf("    pUlSubCh->cqiPmiRiRpt.K=%i\n", pUlSubCh->cqiPmiRiRpt.K);
            uart_printf("    pUlSubCh->cqiPmiRiRpt.riConfigIndex=%i\n", pUlSubCh->cqiPmiRiRpt.riConfigIndex);
            uart_printf("    pUlSubCh->cqiPmiRiRpt.simultaneousAckNackAndCQI=%i\n", pUlSubCh->cqiPmiRiRpt.simultaneousAckNackAndCQI);

            uart_printf("    pUlSubCh->mapInfo.numberofEntries=%i\n", pUlSubCh->mapInfo.numberofEntries);
            for (j = 0; j < pUlSubCh->mapInfo.numberofEntries; j++)
            {
                uart_printf("      pUlSubCh->mapInfo.reselmInfo[%i]=(%i,%i)\n", j,
                       pUlSubCh->mapInfo.reselmInfo[j].startRes,
                       pUlSubCh->mapInfo.reselmInfo[j].numRes);
            }
            pUlCh++;
        }
        pUlCtrlCh = &pUlSf->ulCtlCh[0];
        for (i = 0; i < pUlSf->numberOfCtrlChannelDescriptors; i++)
        {
            uart_printf("CTRL CHANNEL %i:\n----------\n", i);
            uart_printf("    pUlCtrlCh->offsetNextCh=%i\n", pUlCtrlCh->offsetNextCh);
            uart_printf("    pUlCtrlCh->channelId=%i\n", pUlCtrlCh->channelId);
            uart_printf("    pUlCtrlCh->txpowerControl=%i\n", pUlCtrlCh->txpowerControl);
            uart_printf("    pUlCtrlCh->crcLength=%i\n", pUlCtrlCh->crcLength);
            uart_printf("    pUlCtrlCh->channelType=%i\n", pUlCtrlCh->channelType);
            uart_printf("    pUlCtrlCh->scrmblerInitValue=%i\n", pUlCtrlCh->scrmblerInitValue);
            uart_printf("    pUlCtrlCh->codingDescriptor=%i\n", pUlCtrlCh->codingDescriptor);
            uart_printf("    pUlCtrlCh->blockCodeConcatenation=%i\n", pUlCtrlCh->blockCodeConcatenation);
            uart_printf("    pUlCtrlCh->modulationType=%i\n", pUlCtrlCh->modulationType);
            uart_printf("    pUlCtrlCh->mcsType=%i\n", pUlCtrlCh->mcsType);

            uart_printf("    pUlCtrlCh->pucchDedCtl.formatType=%i\n", pUlCtrlCh->pucchDedCtl.formatType);
            uart_printf("    pUlCtrlCh->pucchDedCtl.ackNackRepetition=%i\n", pUlCtrlCh->pucchDedCtl.ackNackRepetition);
            uart_printf("    pUlCtrlCh->pucchDedCtl.repetitionFactor=%i\n", pUlCtrlCh->pucchDedCtl.repetitionFactor);
            uart_printf("    pUlCtrlCh->pucchDedCtl.n1PucchANRep=%i\n", pUlCtrlCh->pucchDedCtl.n1PucchANRep);
            uart_printf("    pUlCtrlCh->pucchDedCtl.cqiPUCCHResourceIndex=%i\n", pUlCtrlCh->pucchDedCtl.cqiPUCCHResourceIndex);
            uart_printf("    pUlCtrlCh->pucchDedCtl.srPUCCHResourceIndex=%i\n", pUlCtrlCh->pucchDedCtl.srPUCCHResourceIndex);

            uart_printf("    pUlCtrlCh->cqiPmiRiRpt.cqiReportModeAperiodic=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqiReportModeAperiodic);
            uart_printf("    pUlCtrlCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset=%i\n", pUlCtrlCh->cqiPmiRiRpt.nomPDSCHRSEPREOffset);
            uart_printf("    pUlCtrlCh->cqiPmiRiRpt.cqiReportPeriodicEnable=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqiReportPeriodicEnable);
            uart_printf("    pUlCtrlCh->cqiPmiRiRpt.cqiPUCCHResourceIndex=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqiPUCCHResourceIndex);
            uart_printf("    pUlCtrlCh->cqiPmiRiRpt.cqipmiConfigIndex=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqipmiConfigIndex);
            uart_printf("    pUlCtrlCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic=%i\n", pUlCtrlCh->cqiPmiRiRpt.cqiFormatIndicatorPeriodic);
            uart_printf("    pUlCtrlCh->cqiPmiRiRpt.K=%i\n", pUlCtrlCh->cqiPmiRiRpt.K);
            uart_printf("    pUlCtrlCh->cqiPmiRiRpt.riConfigIndex=%i\n", pUlCtrlCh->cqiPmiRiRpt.riConfigIndex);
            uart_printf("    pUlCtrlCh->cqiPmiRiRpt.simultaneousAckNackAndCQI=%i\n", pUlCtrlCh->cqiPmiRiRpt.simultaneousAckNackAndCQI);

            uart_printf("    pUlCtrlCh->numberofEntries=%i\n", pUlCtrlCh->numberofEntries);
            uart_printf("    pUlCtrlCh->startRes=%i\n", pUlCtrlCh->startRes);
            uart_printf("    pUlCtrlCh->numRes=%i\n", pUlCtrlCh->numRes);

            pUlCtrlCh++;
        }

        pSrs = (SRSDED *)(((unsigned long)pUlSf) + pUlSf->offsetsrsInfo);
        for (i = 0; i < pUlSf->numberSrsinSf; i++)
        {
            uart_printf("SRS %i:\n----------\n", i);
            uart_printf("      pSrs->enableDisable=%i\n", pSrs->enableDisable);
            uart_printf("      pSrs->srsBandwidth=%i\n", pSrs->srsBandwidth);
            uart_printf("      pSrs->srsHoppingBandwidth=%i\n", pSrs->srsHoppingBandwidth);
            uart_printf("      pSrs->transmissionComb=%i\n", pSrs->transmissionComb);
            uart_printf("      pSrs->cyclicShift=%i\n", pSrs->cyclicShift);
            uart_printf("      pSrs->freqDomainPosition=%i\n", pSrs->freqDomainPosition);
            uart_printf("      pSrs->duration=%i\n", pSrs->duration);
            uart_printf("      pSrs->srsConfigIndex=%i\n", pSrs->srsConfigIndex);

            pSrs++;
        }

        pRachCtrl = (RACHCTRL *)(((unsigned long)pUlSf) + pUlSf->offsetRachCtrlStruct);
        uart_printf("    pRachCtrl->prachEnable=%i\n", pRachCtrl->prachEnable);
        uart_printf("    pRachCtrl->rootSequenceIndex=%i\n", pRachCtrl->rootSequenceIndex);
        uart_printf("    pRachCtrl->prachConfigIndex=%i\n", pRachCtrl->prachConfigIndex);
        uart_printf("    pRachCtrl->highSpeedFlag=%i\n", pRachCtrl->highSpeedFlag);
        uart_printf("    pRachCtrl->zeroCorrelationZoneConfig=%i\n", pRachCtrl->zeroCorrelationZoneConfig);
        uart_printf("    pRachCtrl->prachFreqOffset=%i\n", pRachCtrl->prachFreqOffset);
        break;

    case PHY_RXSTART_IND:
        pMsgInd = (PMSGIND) pGen;
        uart_printf("  pRxStartInd->subFrameNum=%i\n", pMsgInd->subFrameNum);
        break;

    case PHY_RXSDU_IND:
        pRxSduInd = (PRXSDUIND) pGen;
        uart_printf("  pRxSdu->channelId=%i\n", pRxSduInd->channelId);
        uart_printf("  pRxSdu->msgType=%i\n", pRxSduInd->msgType);
        uart_printf("  pRxSdu->phyEntityId=%i\n", pRxSduInd->phyEntityId);
        uart_printf("  pRxSdu->numBitsRx=%i\n", pRxSduInd->numBitsRx);
        uart_printf("  pRxSdu->status=%i\n", pRxSduInd->status);
        uart_printf("  pRxSdu->frameNum=%i\n", pRxSduInd->frameNum);
        uart_printf("  pRxSdu->subFrameNum=%i\n", pRxSduInd->subFrameNum);
        uart_printf("  pRxSdu->chanType=%i\n", pRxSduInd->chanType);
        uart_printf("  pRxSdu->pRxSdu=%p\n", (void*)pRxSduInd->pRxSdu);

        // Currently payload starts at this field
        pDat = (unsigned char *) (&pRxSduInd->pRxSdu);
        for (i = 0; i < pRxSduInd->numBitsRx >> 3;)
        {
            for (j = 0; j < 16; j++)
            {
                uart_printf("%02X ", pDat[j]);
                if (i + j + 1 >= (pRxSduInd->numBitsRx >> 3))
                    break;
            }
            for (j = 0; j < 16; j++)
            {
                if (isprint(pDat[j]))
                    uart_printf("%c", pDat[j]);
                else
                    uart_printf(".");
                if (i + j + 1 >= (pRxSduInd->numBitsRx >> 3))
                    break;
            }
            uart_printf("\n");
            i += 16;
            pDat += 16;

            // FIXME: This is debug check and to be removed
            if (i > 64)
                break;
        }
        break;

    case PHY_RXEND_IND:
        pMsgInd = (PMSGIND) pGen;
        uart_printf("  pRxEndInd->subFrameNum=%i\n", pMsgInd->subFrameNum);
        uart_printf("  pRxEndInd->frameNumber=%i\n", pMsgInd->frameNumber);
        break;

    case PHY_INIT_REQ:
        pInit = (PINITPARM) (pGen + 1);

        uart_printf("  pInit->channelBandwidth=%i\n", pInit->channelBandwidth);
        uart_printf("  pInit->frtypeDuplexmode=%i\n", pInit->frtypeDuplexmode);
        uart_printf("  pInit->radioAccessMode=%i\n", pInit->radioAccessMode);
        uart_printf("  pInit->physicalResourceBandwidth=%i\n", pInit->physicalResourceBandwidth);
        uart_printf("  pInit->numberResourceBlocksperSlot=%i\n", pInit->numberResourceBlocksperSlot);
        uart_printf("  pInit->phyLayerCellIdGroup=%i\n", pInit->phyLayerCellIdGroup);
        uart_printf("  pInit->phyLayerId=%i\n", pInit->phyLayerId);
        uart_printf("  pInit->txAntennaPortCount=%i\n", pInit->txAntennaPortCount);
        uart_printf("  pInit->fastfwdorPreclockingNumber=%i\n", pInit->fastfwdorPreclockingNumber);
        uart_printf("  pInit->fftSize=%i\n", pInit->fftSize);
        uart_printf("  pInit->numberUsedSubcarriers=%i\n", pInit->numberUsedSubcarriers);
        uart_printf("  pInit->nMaxDlRb=%i\n", pInit->nMaxDlRb);
        uart_printf("  pInit->nMaxUlRb=%i\n", pInit->nMaxUlRb);
        uart_printf("  pInit->referenceSignalPower=%i\n", pInit->referenceSignalPower);
        uart_printf("  pInit->primarySyncSignalPower=%i\n", pInit->primarySyncSignalPower);
        uart_printf("  pInit->secondarySyncSignalPower=%i\n", pInit->secondarySyncSignalPower);
        uart_printf("  pInit->numDataRePerPRB=%i\n", pInit->numDataRePerPRB);
        uart_printf("  pInit->cyclicPrefixType=%i\n", pInit->cyclicPrefixType);
        uart_printf("  pInit->rxAntennaPortCount=%i\n", pInit->rxAntennaPortCount);
        uart_printf("  pInit->pb=%i\n", pInit->pb);
        uart_printf("  pInit->customExtensionReportEn=%i\n", pInit->customExtensionReportEn);
        uart_printf("  pInit->rachReportMode=%i\n", pInit->rachReportMode);
        uart_printf("  pInit->txSduConfEnable=%i\n", pInit->txSduConfEnable);
        uart_printf("  pInit->sduConfig=%i\n", pInit->sduConfig);
        uart_printf("  pInit->radioFrameNumber=%i\n", pInit->radioFrameNumber);
        uart_printf("  pInit->subframeNumber=%i\n", pInit->subframeNumber);
        uart_printf("  pInit->slotNumber=%i\n", pInit->slotNumber);
        uart_printf("  pInit->srsBandwidthConfig=%i\n", pInit->srsBandwidthConfig);
        uart_printf("  pInit->srsSubframeConfig=%i\n", pInit->srsSubframeConfig);
        uart_printf("  pInit->srsSimultaneousANandSRS=%i\n", pInit->srsSimultaneousANandSRS);
        uart_printf("  pInit->prachConfigurationIndex=%i\n", pInit->prachConfigurationIndex);
        uart_printf("  pInit->prachFrequencyOffset=%i\n", pInit->prachFrequencyOffset);
        uart_printf("  pInit->prachHighSpeedFlag=%i\n", pInit->prachHighSpeedFlag);
        uart_printf("  pInit->prachCyclicShiftConfig=%i\n", pInit->prachCyclicShiftConfig);
        uart_printf("  pInit->prachRootSequenceIndex=%i\n", pInit->prachRootSequenceIndex);
        break;
        case PHY_TXHIADCIUL_REQ:
         pHiDci0Vector = (PHIADCIULMSGDESC) (pGen);
         uart_printf("pHiDci0Vector->frameNumber %i\n", pHiDci0Vector->frameNumber);
         uart_printf("pHiDci0Vector->subFrameNumber %i\n", pHiDci0Vector->subFrameNumber);
         uart_printf("pHiDci0Vector->msgLength %i\n", pHiDci0Vector->msgLength);
         uart_printf("pHiDci0Vector->msgType %i\n", pHiDci0Vector->msgType);
         uart_printf("pHiDci0Vector->numberDciUlSdus %i\n", pHiDci0Vector->numDciUlSdus);
         uart_printf("pHiDci0Vector->numHiSdus %i\n", pHiDci0Vector->numHiSdus);
         uart_printf("pHiDci0Vector->phyEntityId %i\n", pHiDci0Vector->phyEntityId);
         uart_printf("pHiDci0Vector->reserved %i\n", pHiDci0Vector->reserved);
        break;
        case PHY_TXHISDU_REQ:
         pHiSdu = (PHIINFOMSGDESC) (pGen);
         uart_printf("pHiSdu->channelId %i\n", pHiSdu->channelId);
         uart_printf("pHiSdu->msgLength %i\n", pHiSdu->msgLength);
         uart_printf("pHiSdu->msgType %i\n", pHiSdu->msgType);
         uart_printf("pHiSdu->nackAck %i\n", pHiSdu->nackAck);
         uart_printf("pHiSdu->phichGroupNumber %i\n", pHiSdu->phichGroupNumber);
         uart_printf("pHiSdu->phichSequenceIndex %i\n", pHiSdu->phichSequenceIndex);
         uart_printf("pHiSdu->phyEntityId %i\n", pHiSdu->phyEntityId);
         uart_printf("pHiSdu->transmissionMode %i\n", pHiSdu->transmissionMode);
         uart_printf("pHiSdu->txPowerControl %i\n", pHiSdu->txPowerControl);
         uart_printf("pHiSdu->reserved %i\n", pHiSdu->reserved);
        break;
        case PHY_TXDCIULSDU_REQ:
         pDci0Sdu = (PDCIULSDUMSG) (pGen);
         uart_printf("pDci0Sdu->channelId %i\n", pDci0Sdu->channelId);
         uart_printf("pDci0Sdu->msgLength %i\n", pDci0Sdu->msgLength);
         uart_printf("pDci0Sdu->msgType %i\n", pDci0Sdu->msgType);
         uart_printf("pDci0Sdu->rnti %i\n", pDci0Sdu->rnti);
         uart_printf("pDci0Sdu->numBitsDciUl %i\n", pDci0Sdu->numBitsDciUL);
         uart_printf("pDci0Sdu->startCCE %i\n", pDci0Sdu->startCCE);
         uart_printf("pDci0Sdu->numCCE %i\n", pDci0Sdu->numCCE);
         uart_printf("pDci0Sdu->numPayloadBytes %i\n", pDci0Sdu->numPayloadBytes);
         uart_printf("pDci0Sdu->phyEntityId %i\n", pDci0Sdu->phyEntityId);
         uart_printf("pDci0Sdu->transmissionMode %i\n", pDci0Sdu->transmissionMode);
         uart_printf("pDci0Sdu->txPowerControl %i\n", pDci0Sdu->txPowerControl);

         pDat = (unsigned char *) (&pDci0Sdu->pTxSdu);
        for (i = 0; i < pDci0Sdu->numBitsDciUL >> 3;)
        {
            for (j = 0; j < 16; j++)
            {
                uart_printf("%02X ", pDat[j]);
                if (i + j + 1 >= (pDci0Sdu->numBitsDciUL >> 3))
                    break;
            }
            for (j = 0; j < 16; j++)
            {
                if (isprint(pDat[j]))
                    uart_printf("%c", pDat[j]);
                else
                    uart_printf(".");
                if (i + j + 1 >= (pDci0Sdu->numBitsDciUL >> 3))
                    break;
            }
            uart_printf("\n");
            i += 16;
            pDat += 16;

            // FIXME: This is debug check and to be removed
            if (i > 64)
                break;
        }
        break;

      default:
         uart_printf("Unknown command:\n----------------\n");
         // Dump 64 bytes of unknown command
         pDat = (unsigned char *) vector;

         for (i = 0; i < 64; i += 16)
         {
            for (j = 0; j < 16; j++)
            {
               uart_printf("%02X ", pDat[j]);
            }
            for (j = 0; j < 16; j++)
            {
               if (isprint(pDat[j]))
                 uart_printf("%c", pDat[j]);
               else
                  uart_printf(".");
            }
            uart_printf("\n");
            pDat += 16;
         }
         break;
   }
   /*
      ENODEB
    */
   return
      SUCCESS;
}

/*
*
*       Fun:   ysMsLimSendToPhy
*
*       Desc:  This primitive is used by CL to send message to PHY
*
*       Ret:
*
*       Notes: None
*
*       File:  ys_ms_li.c`
*
*/

#ifdef ANSI
PUBLIC S16 ysMsLimSendToPhy
(
U32          size,
PGENMSGDESC   pMsgDesc
)
#else
PUBLIC S16 ysMsLimSendToPhy(size, pMsgDesc)
U32          size;
PGENMSGDESC   pMsgDesc;
#endif
{
   U32            t;
   RESULTCODE     ret;

   TRC2(ysMsLimSendToPhy)

   YS_DBG_INFO((_ysp, "ysMsLimSendToPhy() \n"));



   if(pMsgDesc == NULLP)
   {
      uart_printf("ysMsLimSendToPhy(): Empty msg received \n");
      YS_DBG_ERR((_ysp, "ysMsLimSendToPhy(): Empty msg received \n"));
      RETVALUE(RFAILED);
   }
   t = GetTIMETICK();
   ret = SvsrSendMsg(0, IID_LTE_EX, pMsgDesc, SVSR_MSG_OPT_DEFAULT);
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_CL_SEND_TO_PHY, RESOURCE_LARM, t, GetTIMETICK());
#endif
   if(ret != SUCCESS)
   {
      extern HANDLE  SvsrMsgPart;
      YS_DBG_ERR((_ysp, "ysMsLimSendToPhy(): SvsrSendMsg returened error=%d len=%d MxGetFreePartBlocks=%d \n",
            ret, size, MxGetFreePartBlocks(SvsrMsgPart) ));
      RETVALUE(ROK);
   }
   else
      YS_DBG_INFO((_ysp, "ysMsLimSendToPhy Success \n"));

   RETVALUE(ROK);
} /* end of ysMsLimSendToPhy */


#endif
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
/main/1      ---      rk   1. initial release.
/main/1    ys004.102  vr   1. Merged MSPD code with phy 1.7
*********************************************************************91*/

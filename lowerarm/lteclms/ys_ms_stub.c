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
  
     Desc:     C source code for Entry point fucntions
  
     File:     ys_ms_stub.c
  
     Sid:      
  
     Prg:      pk
  
**********************************************************************/


/* header include files (.h) */
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
#include "LtePhyL2Api.h"
#include "apidefs.h"
#include "resultcodes.h"
#include "ys_ms.h"            /* defines and macros for CL */
#include "ys_ms_err.h"        /* YS error defines */
#include "osal.h"

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
#include "ys_ms.x"            /* typedefs for CL */

/* Global OSAL Definitions */

/*ys004.102 :  Merged MSPD code with phy 1.7 */

OSAL_SEMAPHORE PhyTickSema;
OSAL_PADDRESS PhyQueueMem = NULL;
OSAL_QUEUE PhyQueue;
OSAL_SEMAPHORE PhyRxSema;
OSAL_MEMARRAY PhyPart;
OSAL_PADDRESS PhyPartMem = NULL;

OSAL_MEMARRAY  MsgArray;

U32 ExpectedTxSdu = 0;
U32 ExpectedRxSdu = 0;
U32 TxFrameNumber = 0;
U32 TxSubFrameNumber = 0;
U32 RxFrameNumber = 0;
U32 RxSubFrameNumber = 0;
U32 TxChanId = 0;
U32 RxChanId = 0;
U32 pFrameNumber = 0;
U32 pSubFrameNumber = 0;

#define           MSG_MAXSIZE  16384

DLSUBFRDESC TxDlSubFrame;
ULSUBFRDESC RxUlSubFrame;

/* Forward Declarations */
PUBLIC RESULTCODE PhySendToMacMsg
(
U8   MsgType,
U16  phyEntityId,
U8  *pData,
U32  len
);

PUBLIC RESULTCODE PhySendToPhyMsg
(
U8   MsgType,
U8  *pData,
U32  size
);

PUBLIC RESULTCODE PhyDispatch
(
U32  size,
PTR  msg
);

PUBLIC Void PhyTxStart
(
PGENMSGDESC msg,
U32 len
);

void PhyTxSdu(PMSGHEADER msg, U32 size);
void PhyRxStart(PGENMSGDESC msg);
void PhyRxSdu(PGENMSGDESC msg);
/* End of Forward Declarations */



PTR MsgGetDataOffset(PTR msg)
{   
       PMSGHEADER pHdr = (PMSGHEADER) msg;
           return &pHdr->param[0];
} 

#ifndef LEAK_CHECK
#ifdef ANSI
PUBLIC Void MsgFreeBuffer
(
PMSGHEADER pMsg
)
#else
PUBLIC Void MsgFreeBuffer(pMsg)
PMSGHEADER pMsg;
#endif
{
#ifndef PHY_TRILL_MEM
    /* NULL check will be performed in OsalFreeMemArray */
    OsalFreeMemArray(&MsgArray, (OSAL_PADDRESS) pMsg);
#else
    free(pMsg);
    pMsg = NULLP;
#endif

}
#else
#ifdef ANSI
PUBLIC Void MsgFreeBuffer1
(
PMSGHEADER pMsg,
char *s,
int l
)
#else
PUBLIC Void MsgFreeBuffer(pMsg)
PMSGHEADER pMsg;
#endif
{
    printf("Free memory %x %s:%d\n", pMsg, s, l);
    /* NULL check will be performed in OsalFreeMemArray */
    OsalFreeMemArray(&MsgArray, (OSAL_PADDRESS) pMsg);
}
#endif


#ifndef LEAK_CHECK
#ifdef ANSI
PUBLIC PMSGHEADER MsgAllocBuffer
(
int flags
)
#else
PUBLIC PMSGHEADER MsgAllocBuffer(int flags)
#endif
{
    OSAL_PADDRESS pBlock;
#ifndef PHY_TRILL_MEM
    if (OsalAllocMemArray(&MsgArray, &pBlock) != SUCCESS)
    {
        return NULL;
    }
#else
    printf("using malloc\n");
    pBlock = (OSAL_PADDRESS) malloc(4000);
#endif
    return (PMSGHEADER) pBlock;
}
#else
#ifdef ANSI
PUBLIC PMSGHEADER MsgAllocBuffer1
(
 char *s,
 int l
)
#else
PUBLIC PMSGHEADER MsgAllocBuffer(int flags)
#endif
{
    OSAL_PADDRESS pBlock;
    if (OsalAllocMemArray(&MsgArray, &pBlock) != SUCCESS)
    {
        return NULL;
    }
    printf("Allocation of point %x %s:%d\n", pBlock, s, l);
    return (PMSGHEADER) pBlock;
}
#endif



#ifdef ANSI
PUBLIC Void MsgInitMessage
(
PMSGHEADER pHdr
)
#else
PUBLIC Void MsgInitMessage(pHdr)
PMSGHEADER pHdr;
#endif
{
   TRC2(MsgInitMessage)

    pHdr->control = CTRL_VERSION;
    // Zero length
    pHdr->length = 0;
    // Invalid type
    pHdr->type = 0;
    // Invalid instance
    pHdr->srcID = 0;
    pHdr->dstID = 0;
    // Invalid message ID
    pHdr->msgID = 0;
}


#ifdef ANSI
PUBLIC RESULTCODE MsgDispatch
(
U32  srcID,
U32  size,
PTR  msg
)
#else
PUBLIC RESULTCODE MsgDispatch(srcID, size, msg)
U32  srcID;
U32  size;
PTR  msg;
#endif
{
   PMSGHEADER pHdr = (PMSGHEADER) msg;
   RESULTCODE ret =  FAILURE;
   U16        route = pHdr->dstID & MSGS_ROUTE;

   TRC2(MsgDispatch)

   if (route == MSGS_LARM)
   {
      switch (srcID)
      {
         case MSGS_MAC:
            ret = PhyDispatch(size, msg);
            break;
         default:
            YS_DBG_ERR((_ysp, "MsgDispatch() : Invalid srcID"));
            RETVALUE(RFAILED);
      }
   }


   RETVALUE (ret);
}

#ifdef ANSI
PUBLIC RESULTCODE PhyDispatch
(
U32  size,
PTR  msg
)
#else
PUBLIC RESULTCODE PhyDispatch(size, msg)
U32  size;
PTR  msg;
#endif
{
   PMSGHEADER pHdr = (PMSGHEADER) msg;
   PGENMSGDESC pMsg;
   U16         phyEntityId;

   pMsg        = (PGENMSGDESC) &pHdr->param[0];
   phyEntityId = pMsg->phyEntityId;
   switch (pMsg->msgType)
   {
      case PHY_TXSTART_REQ:
         PhyTxStart(pMsg, (size - (sizeof(MSGHEADER) - sizeof(pHdr->param[0]))));
         break;
      case PHY_TXSTART_CONF:
         break;
      case PHY_TXSTART_IND:
         break;
      case PHY_TXSDU_REQ:
         PhyTxSdu(pHdr, size);
         break;
      case PHY_TXSDU_CONF:
         break;
      case PHY_TXEND_IND:
         break;
      case PHY_RXSTART_REQ:
         PhyRxStart(pMsg);
         break;
      case PHY_RXSTART_CONF:
         break;
      case PHY_RXSTART_IND:
         break;
      case PHY_RXSDU_IND:
         break;
      case PHY_RXEND_IND:
         break;
      case PHY_INIT_REQ:
         PhySendToMacMsg(PHY_INIT_IND, phyEntityId, NULL, 0);
         break;
      case PHY_START_REQ:
         PhySendToMacMsg(PHY_START_CONF, phyEntityId, NULL, 0);
         break;
      case PHY_STOP_REQ:
         PhySendToMacMsg(PHY_STOP_CONF, phyEntityId, NULL, 0);
         break;
      case PHY_INIT_IND:
         break;
      default:
         return FAILURE;
   }

   MsgFreeBuffer(msg); //free

   return SUCCESS;
}

#ifdef ANSI
PUBLIC RESULTCODE PhySendToMacMsg
(
U8   MsgType,
U16  phyEntityId,
U8  *pData,
U32  len
)
#else
PUBLIC RESULTCODE PhySendToMacMsg(MsgType, pData, len)
U8   MsgType;
U16  phyEntityId;
U8  *pData;
U32  len;
#endif
{
    PMSGHEADER pHdr;
    PGENMSGDESC pToMacMsg;
    PRXSDUIND pRxSduMsg;
    PTXSDUIND pTxSduInd;
    OSAL_SIZE msgSize = 0;

    pHdr = (PMSGHEADER) MsgAllocBuffer(YS_MS_MSG_COMMON);

    if (pHdr == NULL)
    {
       YS_DBG_ERR((_ysp, "Failed to Allocate Memory"));
       RETVALUE (RFAILED);
    }

    pToMacMsg = MsgGetDataOffset(pHdr);

    // TODO: Check PHY ID
    pToMacMsg->phyEntityId = phyEntityId;
    pToMacMsg->msgType = MsgType;
    switch (MsgType)
    {
    case PHY_TXSTART_CONF:
        if (pData)
        {
            ((PMSGIND)pToMacMsg)->subFrameNum = ((PMSGIND)pData)->subFrameNum;
            ((PMSGIND)pToMacMsg)->frameNumber = ((PMSGIND)pData)->frameNumber;
            ((PMSGIND)pToMacMsg)->status = ((PMSGIND)pData)->status;
        }
        else
        {
            ((PMSGIND)pToMacMsg)->subFrameNum = TxSubFrameNumber;
            ((PMSGIND)pToMacMsg)->frameNumber = TxFrameNumber;
            ((PMSGIND)pToMacMsg)->status = SUCCESS;
        }
        msgSize = sizeof(MSGIND);
        pHdr->type = MSGT_DATA;
        break;
    case PHY_TXSTART_IND:
        ((PMSGIND)pToMacMsg)->subFrameNum = TxSubFrameNumber;
        ((PMSGIND)pToMacMsg)->frameNumber = TxFrameNumber;
        ((PMSGIND)pToMacMsg)->status = SUCCESS;
        msgSize = sizeof(MSGIND);
        pHdr->type = MSGT_DATA;
        break;
    case PHY_TXSDU_CONF:
        pTxSduInd = (PTXSDUIND) pToMacMsg;
        if (pData)
        {
            pTxSduInd->status = ((PTXSDUIND)pData)->status;
            pTxSduInd->subFrameNum = TxSubFrameNumber;
            pTxSduInd->frameNumber = TxFrameNumber;
            pTxSduInd->chanId = ((PTXSDUIND)pData)->chanId;
        }
        else
        {
            pTxSduInd->status = SUCCESS;
            pTxSduInd->subFrameNum = TxSubFrameNumber;
            pTxSduInd->frameNumber = TxFrameNumber;
            pTxSduInd->chanId = TxChanId;
        }
        msgSize = sizeof(TXSDUIND);
        break;
    case PHY_TXEND_IND:
        ((PMSGIND)pToMacMsg)->subFrameNum = TxSubFrameNumber;
        ((PMSGIND)pToMacMsg)->status = SUCCESS;
        msgSize = sizeof(MSGIND);
        break;
    case PHY_RXSTART_CONF:
        if (pData)
        {
            ((PMSGIND)pToMacMsg)->subFrameNum = ((PMSGIND)pData)->subFrameNum;
            ((PMSGIND)pToMacMsg)->status = ((PMSGIND)pData)->status;
        }
        else
        {
            ((PMSGIND)pToMacMsg)->subFrameNum = RxSubFrameNumber;
            ((PMSGIND)pToMacMsg)->status = SUCCESS;
        }
        msgSize = sizeof(MSGIND);
        pHdr->type = MSGT_DATA;
        break;
    case PHY_RXSTART_IND:
        msgSize = sizeof(MSGIND);
        break;
    case PHY_RXSDU_IND:
        if (pData)
        {
            pRxSduMsg = (PRXSDUIND) pToMacMsg;
            pRxSduMsg->numBitsRx = len << 3;
            pRxSduMsg->channelId = RxChanId;
            pRxSduMsg->status = SUCCESS;
            pRxSduMsg->subFrameNum = RxSubFrameNumber;
            pRxSduMsg->frameNum = RxFrameNumber;
            memcpy(&pRxSduMsg->pRxSdu, pData, len);
            msgSize = sizeof(RXSDUIND) + len;
        }
        else
        {
            RETVALUE(RFAILED);
        }
        break;
    case PHY_RXEND_IND:
        ((PMSGIND)pToMacMsg)->subFrameNum = RxSubFrameNumber;
        ((PMSGIND)pToMacMsg)->frameNumber = RxFrameNumber;
        msgSize = sizeof(MSGIND);
        break;
    case PHY_INIT_IND:
        ((PINITIND)pToMacMsg)->status = SUCCESS;
        msgSize = sizeof(INITIND);
        pHdr->type = MSGT_INDICATION;
        break;
    case PHY_START_CONF:
        ((PINITIND)pToMacMsg)->status = SUCCESS;
        msgSize = sizeof(INITIND);
        pHdr->type = MSGT_INDICATION;
        break;
    case PHY_STOP_CONF:
        ((PINITIND)pToMacMsg)->status = SUCCESS;
        msgSize = sizeof(INITIND);
        pHdr->type = MSGT_INDICATION;
        break;
    default:
        pToMacMsg->msgType = MsgType;
        msgSize = sizeof(GENMSGDESC);
        break;
    }

    pHdr->control = CTRL_BYPASS | CTRL_VERSION;
    pHdr->length = msgSize;
    pHdr->srcID  = MSGS_LARM;
    pHdr->dstID  = MSGS_MAC;

    /* Post the task */
    ysPalSendSelf((Data*)pHdr, sizeof(MSGHEADER) + pHdr->length);

    MsgFreeBuffer(pHdr);

    if (MsgType == PHY_START_CONF)
    {
#ifdef USE_SSI_TMR
       ysPalInitTti();
#else
       ysPalInitTtiTmrTsk();
#endif
    }

    RETVALUE (ROK);
}

void PhyTxStart(PGENMSGDESC msg, U32 size)
{
    MSGIND msgData;
    PDLSUBFRDESC pSubFrame = (PDLSUBFRDESC) (msg + 1);
    U32 len;
    PMSGHEADER rxStartIndpHdr;
    PGENMSGDESC rxStartIndMacMsg;
    PMSGHEADER rxEndIndpHdr;
    PGENMSGDESC rxEndIndMacMsg;

    len = sizeof(DLSUBFRDESC);
    len -= sizeof(DLCHANDESC) * MAXCHSUBFRAME;
    len += sizeof(DLCHANDESC) * pSubFrame->numberofChannelDescriptors;

    memcpy(&TxDlSubFrame, pSubFrame, len);

    /* - Save TxSubFrame Number -*/
    TxSubFrameNumber = pSubFrame->subframeNumber;
    TxFrameNumber    = pSubFrame->frameNumber;

    if (ExpectedTxSdu > 0)
    {
        // Error, we did not finish previous transaction
        msgData.subFrameNum = pSubFrame->subframeNumber;
        msgData.status = OVERRUN;
        msgData.frameNumber = pSubFrame->frameNumber;
    }
    else
    {
        ExpectedTxSdu       = pSubFrame->numberofChannelDescriptors;
        msgData.subFrameNum = pSubFrame->subframeNumber;
        msgData.frameNumber = pSubFrame->frameNumber;
        msgData.status      = SUCCESS;
        // Inform peer we're started
        rxStartIndpHdr = (PMSGHEADER) MsgAllocBuffer(YS_MS_MSG_COMMON);
        if (rxStartIndpHdr == NULL)
        {
           YS_DBG_ERR((_ysp, "Failed to allocate memory\n"));
           RETVALUE (RFAILED);
        }
        rxStartIndMacMsg = MsgGetDataOffset(rxStartIndpHdr);
        rxStartIndMacMsg->msgType     = PHY_RXSTART_IND;
        rxStartIndMacMsg->phyEntityId = msg->phyEntityId;
        ((PMSGHEADER)rxStartIndpHdr)->control = CTRL_BYPASS | CTRL_VERSION;
        ((PMSGHEADER)rxStartIndpHdr)->length = size ;
        ((PMSGHEADER)rxStartIndpHdr)->srcID  = MSGS_LARM;
        ((PMSGHEADER)rxStartIndpHdr)->dstID  = MSGS_MAC;
        ((PMSGHEADER)rxStartIndpHdr)->type   = MSGT_DATA;

        pSubFrameNumber = pSubFrame->subframeNumber;
        pFrameNumber = pSubFrame->frameNumber; 
        memcpy( (rxStartIndMacMsg + 1), pSubFrame, sizeof(DLSUBFRDESC));
        PhySendToPhyMsg(PHY_RXSTART_IND, (U8 *)rxStartIndpHdr, size + (sizeof(MSGHEADER) - sizeof(rxStartIndpHdr->param[0])));
       
        if( !ExpectedTxSdu)
        { 
           rxEndIndpHdr = (PMSGHEADER) MsgAllocBuffer(YS_MS_MSG_COMMON);
           if (NULLP == rxEndIndpHdr)
           {
              YS_DBG_ERR((_ysp, "Message Alloc Failed\n"));
              RETVALUE (RFAILED);
           }
           rxEndIndMacMsg = MsgGetDataOffset(rxEndIndpHdr);
           ((PMSGIND)rxEndIndMacMsg)->subFrameNum = pSubFrame->subframeNumber;
           ((PMSGIND)rxEndIndMacMsg)->frameNumber = pSubFrame->frameNumber;
           rxEndIndMacMsg->msgType     = PHY_RXEND_IND;
           rxEndIndMacMsg->phyEntityId = msg->phyEntityId;
           ((PMSGHEADER)rxEndIndpHdr)->control = CTRL_BYPASS | CTRL_VERSION;
           ((PMSGHEADER)rxEndIndpHdr)->length = sizeof(MSGIND);
           ((PMSGHEADER)rxEndIndpHdr)->srcID  = MSGS_LARM;
           ((PMSGHEADER)rxEndIndpHdr)->dstID  = MSGS_MAC;
           ((PMSGHEADER)rxEndIndpHdr)->type   = MSGT_DATA;

           PhySendToPhyMsg(PHY_RXEND_IND, (U8 *)rxEndIndpHdr, sizeof(MSGHEADER) + sizeof(MSGIND));

           MsgFreeBuffer(rxEndIndpHdr);
         } /* end of ExpectedTxSdu == 0 */
        MsgFreeBuffer(rxStartIndpHdr);
    }
} /* end of PhyTxStart */

void PhyTxSdu(PMSGHEADER msg, U32 size)
{
    TXSDUIND msgData;
    U32 buffer[MSG_MAXSIZE / sizeof(U32)];
    U32 buffer1[MSG_MAXSIZE / sizeof(U32)];
    PMSGHEADER pHdr = (PMSGHEADER) buffer;
    PRXSDUIND  pDst;
    PTXSDUREQ  pSdu;
    PMSGHEADER rxEndInd = (PMSGHEADER) buffer1; 
    PMSGIND   rxMsgData;


    pHdr->control = CTRL_BYPASS | CTRL_VERSION;
    pHdr->length = 0;
    pHdr->type = MSGT_DATA;
    pHdr->srcID = MSGS_LARM ;
    pHdr->dstID = MSGS_LARM ;
    pHdr->msgID = 0;
    pDst = MsgGetDataOffset(pHdr);

    pSdu = (PTXSDUREQ) &msg->param[0];

    if (ExpectedTxSdu > 0)
    {
        TxChanId = pSdu->chanId;
        PhySendToMacMsg(PHY_TXSDU_CONF, 1, NULL, 0);

        pDst->channelId = pSdu->chanId;
        pDst->msgType = PHY_RXSDU_IND;
        pDst->phyEntityId = pSdu->phyEntityId;
        pDst->numBitsRx = pSdu->msgLen * 8;
        pDst->status = SUCCESS;
        pDst->frameNum = pFrameNumber;     // TODO: Fill in on receiving side
        pDst->subFrameNum = pSubFrameNumber;  // TODO: Fill in on receiving side
        pDst->chanType =  pSdu->channelType;
        printf("chanType %d\n", pDst->chanType);
        memcpy(&pDst->pRxSdu, &pSdu->pTxSdu  , pSdu->msgLen);

        if(pSdu->msgLen > sizeof(pDst->pRxSdu))
        {
           pHdr->length = sizeof(RXSDUIND) + pSdu->msgLen  - sizeof(pDst->pRxSdu); 
        }
        else
        {
           pHdr->length = sizeof(RXSDUIND); 
        }
        // TXSDU.req becomes RXSDU.ind on other side
        PhySendToPhyMsg(PHY_RXSDU_IND, (U8 *)pHdr, (sizeof(MSGHEADER) - sizeof(pHdr->param[0])) + pHdr->length);
        if (--ExpectedTxSdu == 0)
        {
            // It is not requires as we do here through timer 
            /*PhySendToMacMsg(PHY_TXSTART_IND, 1, NULL, 0); */
            PhySendToMacMsg(PHY_TXEND_IND, 1, NULL, 0);
            // Inform peer we're done
            rxMsgData = MsgGetDataOffset(rxEndInd);     
            rxMsgData->msgType     = PHY_RXEND_IND;
            rxMsgData->phyEntityId = pSdu->phyEntityId;
        ((PMSGIND)rxMsgData)->subFrameNum = pSubFrameNumber;
        ((PMSGIND)rxMsgData)->frameNumber = pFrameNumber;
        ((PMSGHEADER)rxEndInd)->control = CTRL_BYPASS | CTRL_VERSION;
        ((PMSGHEADER)rxEndInd)->length = sizeof(MSGIND);
        ((PMSGHEADER)rxEndInd)->srcID  = MSGS_LARM;
        ((PMSGHEADER)rxEndInd)->dstID  = MSGS_MAC;
        ((PMSGHEADER)rxEndInd)->type   = MSGT_DATA;
            PhySendToPhyMsg(PHY_RXEND_IND, (U8*)rxEndInd, sizeof(MSGHEADER) - sizeof(rxEndInd->param[0])+ sizeof(MSGIND));
        }
    }
    else
    {
        // Error, we do not expect SDU at this time
        msgData.status = OVERRUN;
        msgData.chanId = pSdu->chanId;
    }
}

#ifdef ANSI
PUBLIC RESULTCODE PhySendToPhyMsg
(
U8   MsgType,
U8  *pData,
U32  msgLen
)
#else
PUBLIC RESULTCODE PhySendToPhyMsg(MsgType, pData, msgLen)
U8   MsgType;
U8  *pData;
U32  msgLen;
#endif
{
   switch(MsgType)
   {
      case PHY_RXSTART_IND:
      case PHY_RXSDU_IND:
      case PHY_TXSTART_IND:
      case PHY_RXEND_IND:
      {
         ysPalSend2Peer(pData, msgLen);
         break;
      }
   } /* end of switch statement */
   
   return (0);
}

void PhyRxStart(PGENMSGDESC msg)
{
    PULSUBFRDESC pSubFrame = (PULSUBFRDESC) (msg + 1);
    U32 len;
    MSGIND msgData;

    len = sizeof(ULSUBFRDESC);
    len -= sizeof(ULCHANDESC) * MAXCHSUBFRAME;
    len += sizeof(ULCHANDESC) * pSubFrame->numberofChannelDescriptors;

    memcpy(&RxUlSubFrame, pSubFrame, len);

    msgData.subFrameNum = pSubFrame->subframeNumber;
    msgData.frameNumber = pSubFrame->frameNumber;
    msgData.status      = SUCCESS;

    PhySendToMacMsg(PHY_RXSTART_CONF, msg->phyEntityId, (U8*)&msgData, sizeof(msgData));
    // Inform peer we're started
}

void PhyRxSdu(PGENMSGDESC msg)
{

}


#ifdef ANSI
PUBLIC RESULTCODE BspInit 
(
U32  cpuid 
)
#else
PUBLIC RESULTCODE BspInit(cpuid)
U32  cpuid;
#endif
{
   return (0);
}

#ifdef ANSI
PUBLIC RESULTCODE AuxInit 
(
)
#else
PUBLIC RESULTCODE AuxInit()
#endif
{
   return(SUCCESS);
}

#ifdef ANSI
PUBLIC RESULTCODE ProtoInit 
(
U32  protoid
)
#else
PUBLIC RESULTCODE ProtoInit(protoid)
U32  protoid;
#endif
{
   return(SUCCESS);
}

#ifdef ANSI
PUBLIC RESULTCODE SvsrInit 
(
)
#else
PUBLIC RESULTCODE SvsrInit()
#endif
{
   return(SUCCESS);
}

#ifdef ANSI
PUBLIC RESULTCODE IcpuInit 
(
)
#else
PUBLIC RESULTCODE IcpuInit()
#endif
{
   return(SUCCESS);
}

#ifdef ANSI
PUBLIC RESULTCODE MsgDispatcherInit
(
)
#else
PUBLIC RESULTCODE MsgDispatcherInit()
#endif
{
   return(SUCCESS);
}

#ifdef ANSI
PUBLIC RESULTCODE InitSettings
(
int  argc,
char *argv[]
)
#else
PUBLIC RESULTCODE InitSettings(argc, argv[])
int  argc;
char *argv[];
#endif
{
   return(SUCCESS);
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
/main/1      ---     an 1. initial release.
/main/1    ys004.102 vr 1. Merged MSPD code with phy 1.7
*********************************************************************
91*/

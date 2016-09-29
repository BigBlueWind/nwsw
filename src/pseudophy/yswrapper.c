/**
 * @file    yswrapper.c
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @details This file contains the wrapper routines used by pseudophy interface
 *
 * @author Ritesh
 *
 */
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
#include "ctrlmsg.h"
#include "resultcodes.h"
#include "ys_ms.h"            /* defines and macros for CL */
#include "ys_err.h"        /* YS error defines */
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
U32 ExpectedTxSdu1 = 0;
U32 ExpectedRxSdu1 = 0;
U32 TxFrameNumber1 = 0;
U32 TxSubFrameNumber1 = 0;
U32 RxFrameNumber1 = 0;
U32 RxSubFrameNumber1 = 0;
U32 TxChanId1 = 0;
U32 RxChanId1 = 0;
U32 pFrameNumber1 = 0;
U32 pSubFrameNumber1 = 0;

/* Forward Declarations */
PUBLIC RESULTCODE PsuedoToCLMsg
(
U8   MsgType,
U16  phyEntityId,
U8  *pData,
U32  len
);


#ifdef ANSI
PUBLIC RESULTCODE PsuedoToCLMsg
(
U8   MsgType,
U16  phyEntityId,
U8  *pData,
U32  len
)
#else
PUBLIC RESULTCODE PsuedoToCLMsg(MsgType, phyEntityId, pData, len)
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

    pHdr = (PMSGHEADER) MsgAllocBuffer();

    if (pHdr == NULL)
    {
       YS_DBG_ERR((_ysp, "Failed to Allocate Memory"));
       RETVALUE (RFAILED);
    }

    pToMacMsg = (PGENMSGDESC)MsgGetDataOffset((PTR)pHdr);

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
            ((PMSGIND)pToMacMsg)->subFrameNum = TxSubFrameNumber1;
            ((PMSGIND)pToMacMsg)->frameNumber = TxFrameNumber1;

            ((PMSGIND)pToMacMsg)->subFrameNum = ((PMSGIND)pData)->subFrameNum;
            ((PMSGIND)pToMacMsg)->frameNumber = ((PMSGIND)pData)->frameNumber;

            ((PMSGIND)pToMacMsg)->status = SUCCESS;
        }
        msgSize = sizeof(MSGIND);
        pHdr->type = MSGT_DATA;
        break;
    case PHY_TXSTART_IND:
        ((PMSGIND)pToMacMsg)->subFrameNum = TxSubFrameNumber1;
        ((PMSGIND)pToMacMsg)->frameNumber = TxFrameNumber1;
        ((PMSGIND)pToMacMsg)->status = SUCCESS;
        msgSize = sizeof(MSGIND);
        pHdr->type = MSGT_DATA;
        break;
    case PHY_TXSDU_CONF:
        pTxSduInd = (PTXSDUIND) pToMacMsg;
        if (pData)
        {
            pTxSduInd->status = ((PTXSDUIND)pData)->status;
            pTxSduInd->subFrameNum = TxSubFrameNumber1;
            pTxSduInd->frameNumber = TxFrameNumber1;
            pTxSduInd->chanId = ((PTXSDUIND)pData)->chanId;
        }
        else
        {
            pTxSduInd->status = SUCCESS;
            pTxSduInd->subFrameNum = TxSubFrameNumber1;
            pTxSduInd->frameNumber = TxFrameNumber1;
            pTxSduInd->chanId = TxChanId1;
        }
        msgSize = sizeof(TXSDUIND);
        break;
    case PHY_TXEND_IND:
        ((PMSGIND)pToMacMsg)->subFrameNum = TxSubFrameNumber1;
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
            ((PMSGIND)pToMacMsg)->subFrameNum = RxSubFrameNumber1;
            ((PMSGIND)pToMacMsg)->status = SUCCESS;
        }
        msgSize = sizeof(MSGIND);
        pHdr->type = MSGT_DATA;
        break;
    case PHY_RXSTART_IND:

        ((PMSGIND)pToMacMsg)->status = ((PMSGIND)pData)->status;
    	((PMSGIND)pToMacMsg)->subFrameNum = ((PMSGIND)pData)->subFrameNum;

        msgSize = sizeof(MSGIND);
        break;
    case PHY_RXSDU_IND:
        if (pData)
        {
            pRxSduMsg = (PRXSDUIND) pToMacMsg;
            pRxSduMsg->numBitsRx = len << 3;
            pRxSduMsg->channelId = ys_crnti;
            pRxSduMsg->status = SUCCESS;
            pRxSduMsg->subFrameNum = RxSubFrameNumber1;
            pRxSduMsg->frameNum = RxFrameNumber1;
            memcpy(&pRxSduMsg->pRxSdu, pData, len);
            pRxSduMsg->subFrameNum = pData[0];
            pRxSduMsg->chanType = PUSCH;
            if(pRxSduMsg->numBitsRx <= 24)
            {
            	pRxSduMsg->chanType = PRACH;
                pRxSduMsg->channelId = 2;
            	pRxSduMsg->numBitsRx = 8;
            	memcpy(&pRxSduMsg->pRxSdu, pData, len);

            }
            else
            {
            	memcpy(&pRxSduMsg->pRxSdu, pData + 1, len);
            }


            msgSize = sizeof(RXSDUIND) + len;
        }
        else
        {
            RETVALUE(RFAILED);
        }
        break;
    case PHY_RXEND_IND:
        ((PMSGIND)pToMacMsg)->subFrameNum = RxSubFrameNumber1;

        ((PMSGIND)pToMacMsg)->subFrameNum = pData[0];

        ((PMSGIND)pToMacMsg)->frameNumber = RxFrameNumber1;
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

    RETVALUE (ROK);
}

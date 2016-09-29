/** @file rrciface.c
 *
 * @brief RRC interface for eNodeB
 * @author Mindspeed Technologies
 * @version $Revision: 1.36 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef ENODEB

#include "osal.h"
#include "rrciface.h"
#include "apidefs.h"
#include "maciface.h"
#include "msgroute.h"
#include "rrc_types.h"
#include "ltemac.h"

//#include "RrcMacParse.h"

#define RRC_MAC_QUEUE_LEN   4

extern OSAL_HEAP OsalGlobalHeap;

OSAL_TASK RRC_FromMacTaskId;
OSAL_SEMAPHORE ToRRCSema;
OSAL_QUEUE ToRRCQueue;
OSAL_MEMARRAY RRC_MAC_MsgArray;

void RRC_FromMacTask(PTR pParam);
U32 MS_RRC_MsgParse(void* handle, U32* msgID);
extern RESULTCODE SystemCtrlParse(U8* InBuf, U32 InLen);

U32  RrcInit(void* phy_out, void* phy_in)
{
    U32 rc = SUCCESS;
    OSAL_PADDRESS RRC_MACMemBlock;
    OSAL_PADDRESS RRC_MACQueue;

    rc = RRC_Ctx_Init(OsalGlobalHeap);

    if (rc)
    {
        printf("EnodeB Rrc Init Error: heap allocateion error %d\n", rc);
        return rc;
    }
    if(OsalAllocHeap(&OsalGlobalHeap,
         &RRC_MACQueue,
         RRC_MAC_QUEUE_LEN * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        printf("EnodeB Rrc Init Error: cannot allocate the RRc block\n");
        return FAILURE;
    }
    if (OsalCreateQueue(&ToRRCQueue,RRC_MACQueue,RRC_MAC_QUEUE_LEN * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        printf("EnodeB Rrc Init Error: ToRRCQueue\n");
        rc = FAILURE;
    }

    if (OsalCreateSema(&ToRRCSema) != SUCCESS)
    {
        printf("EnodeB Rrc Init Error: ToRRCSema\n");
        rc = FAILURE;
    }

    OsalAllocHeap(&OsalGlobalHeap, &RRC_MACMemBlock, MSG_MAXSIZE * RRC_MAC_QUEUE_LEN * 2);
    OsalCreateMemArray(&RRC_MAC_MsgArray, RRC_MACMemBlock, MSG_MAXSIZE * RRC_MAC_QUEUE_LEN * 2, MSG_MAXSIZE);

    // todo: check is MAC and PHY modules inited
    OsalCreateTask(&RRC_FromMacTaskId,
        (OSAL_PADDRESS) &RRC_FromMacTask,
        NULL, "RRC_FromMacTask",
        OSAL_PRIORITY_NORMAL,
        (OSAL_PADDRESS) 0,
        0);

    OsalActivateTask(&RRC_FromMacTaskId);

    return rc;
}
void RRC_FromMacTask(PTR pParam)
{
    U32 rc = SUCCESS;
    U8* data = NULL;
    rrcInfoData* p_rrc_info;
    U32 msgId;
    PMSGHEADER msg;
    while(1)
    {
        if (OsalWaitForSema(&ToRRCSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            if ((OsalDequeue(&ToRRCQueue, (OSAL_PADDRESS *)&data )) == SUCCESS)
            {
                p_rrc_info = (rrcInfoData*)(data + sizeof(MSGHEADER));
                rc = MS_RRC_MsgParse(p_rrc_info, &msgId);
                msg = (PMSGHEADER)((U32)(p_rrc_info + 1) - sizeof(MSGHEADER));
                msg->length = p_rrc_info->dataLen;
                msg->control = CTRL_VERSION;
                msg->srcID = MSGS_RRC;
                msg->dstID = MSGS_HOST;
                msg->type = MSGT_DATA;
                msg->msgID = msgId;
                if ( rc != SUCCESS)
                {
                    msg->param[0] = rc;
                    msg->length = sizeof(U32);
                }
                MsgDispatch(MSGS_RRC, msg->length + sizeof(MSGHEADER), msg); //goto upper layer
                OsalFreeMemArray(&RRC_MAC_MsgArray,msg);
            }
        }
    }
}
U32  MS_Close(void)
{
    U32 rc = SUCCESS;
    if (RRC_FromMacTaskId)
    {
        rc = OsalDestroyTask(&RRC_FromMacTaskId);
        RRC_FromMacTaskId = 0;
    }
    RRC_Destroy();
    return rc;
}
static void RrcHeader(PMSGHEADER pHdr, U16 MsgId)
{
        pHdr->control = CTRL_VERSION;
        pHdr->length = sizeof(MSGHEADER);
        pHdr->srcID = MSGS_RRC;
        pHdr->dstID = MSGS_MAC;
        pHdr->type = MSGT_CONFIG;
        pHdr->msgID = MsgId;
}

U32 MS_RRC_MsgParse(void* handle, U32* msgID)
{
    U8 TxBuffer[MSG_MAXSIZE] = {0};
    PMSGHEADER conf_msg = (PMSGHEADER)TxBuffer;
    rrcInfoData* p_rrc_info = (rrcInfoData*)handle;//msg->param;
    U32 rrc_msg_id = p_rrc_info->msgID;
    U32 rc = SUCCESS;
    if (p_rrc_info->msgType == CCCH)
    {
        switch(rrc_msg_id)
        {
        case RRC_CONNECT_REQUEST:
            {
         //   RrcHeader(conf_msg,0);
        //    RRC_ConnectRequest(conf_msg, p_rrc_info, p_rrc_info->dataLen + sizeof(rrcInfoData));
          //  rc = MsgDispatch(MSGS_RRC, conf_msg->length, conf_msg); //goto upper layer
            }
            break;
        case RRC_DEBUG_DATA:
            break;
        default:
            printf("EnodeB Rrc Parse Error: RRC receives unknown msg API: %x\n", rrc_msg_id);
            rc = RRC_UNKNOWN_API;
        }
    }
    else
    {
       if (p_rrc_info->msgType == DCCH)
       {
          switch(rrc_msg_id)
            {

            case RRC_CONNECT_COMPLETE:
               break;

            case RRC_CONNECT_RECONFIGURATION_COMPLETE:
                break;

            default:
                rc = RRC_UNKNOWN_API;
           }
       }
       else
       {
           printf("EnodeB Rrc Parse Error: RRC received message has unsupported the LCH type: %x\n", p_rrc_info->msgType);
           rc = RRC_UNKNOWN_LOGIC_CHANNEL;
       }
    }
    printf("EnodeB Rrc Parse Info: the RrcApi %x received from UE\n", rrc_msg_id);
   *msgID = rrc_msg_id;
    return rc;
}

RESULTCODE RrcDispatch(U32 size, PTR handle)
{
    PMSGHEADER msg = (PMSGHEADER) handle;
    void* pParam = (void*)&msg->param;
    U16 ParamLen = msg->length;
    rrcInfoData* p_rrc_info;
    PMSGHEADER pRsp;
    U32 rc = SUCCESS;

    OsalAllocMemArray(&RRC_MAC_MsgArray, (OSAL_PADDRESS *)&p_rrc_info);
    if (p_rrc_info == NULL)
    {
        printf("EnodeB Rrc ApiHandler Error: RRC block allocated failed\n");

        MsgFreeBuffer(msg); //free
        return FAILURE;
    }
    memset(p_rrc_info, 0, sizeof(rrcInfoData));

        p_rrc_info->msgID = msg->msgID;
        p_rrc_info->dataLen = ParamLen;
        p_rrc_info->ueIdx = (U16)0x77;
        p_rrc_info->data_p = p_rrc_info + 1;
        memcpy(p_rrc_info->data_p,pParam, ParamLen);
        switch (msg->msgID)
        {
        case RRC_CONNECT_SETUP:
            p_rrc_info->msgType = (U16)CCCH;
            rc = RRC_ConnectSetup((void*)p_rrc_info, ParamLen + sizeof(rrcInfoData));
            break;
        case RRC_DEBUG_DATA:
            p_rrc_info->msgType = (U16)CCCH;
            rc = RRC_DebugData((void*)p_rrc_info, ParamLen + sizeof(rrcInfoData));
            break;

        case RRC_CONNECT_RELEASE:
        case RRC_CONNECT_RECONFIGURATION:
        case RRC_UL_INFORMATION_TRANSFER:
        case RRC_UECAPABILITY_ENQUIRY:
            p_rrc_info->msgType = (U16)DCCH;
            rc = RRC_DebugData((void*)p_rrc_info, ParamLen + sizeof(rrcInfoData));
            break;
        case SYSTEM_CTRL_REQ:
            rc = SystemCtrlParse(p_rrc_info->data_p, ParamLen);
            OsalFreeMemArray(&RRC_MAC_MsgArray, (void*)p_rrc_info);

            break;
        case DRB_COMMON_REQ:
            p_rrc_info->msgType = (U16)DCCH;
            rc = RRC_DebugData((void*)p_rrc_info, ParamLen + sizeof(rrcInfoData));
            break;
        default:
            rc = ERROR_RRC_UNKNOWN_API;
            OsalFreeMemArray(&RRC_MAC_MsgArray,p_rrc_info);
            break;
    }
    if ((pRsp = MsgInitReply(msg)) != NULL)
    {
        MsgResult(pRsp, rc);
        MsgDispatch(pRsp->srcID, pRsp->length + sizeof(MSGHEADER), pRsp);
    }

	MsgFreeBuffer(msg); //free 
	
    return rc;
}

int QueueRrcToMac(U16* logicChanID, PTR pBuf, OSAL_SIZE blockSize)
{
    OSAL_SIZE msgSize;
    rrcInfoData* ptr = NULL;

    OsalDequeue(&bRrcToMac,(void*)&ptr);
    if (ptr == NULL)
        {
        msgSize = 0;
        return msgSize;
        }

    msgSize = ptr->dataLen + sizeof(rrcInfoData);
    *logicChanID = (U16)ptr->msgType;
    memcpy(pBuf, ptr, msgSize);
    OsalFreeMemArray(&RRC_MAC_MsgArray,ptr);
#if 0
    {
    int i ;
    U8* p = (U8*)pBuf;
    printf("Data QueueRrcToMac:Size%d :", msgSize);
    for (i=0;i<32;i++)
        printf("%x  ",p[i]);
    printf("\n");
    }
#endif
    return msgSize;
}
int QueueMacToRrc(U16* logicChanType, PTR pBuf, OSAL_SIZE blockSize)
{

    rrcInfoData* rrc_info = (rrcInfoData*)pBuf;
    U8* rrc_info_out = NULL;
    U8 MsgOffset = sizeof(MSGHEADER);
    U8 rc = 0;
    if (rrc_info->dataLen > MSG_MAXSIZE)
    {
        printf("WARNING: RRC frame too long [len=%d]\n", rrc_info->dataLen);
        return -1;
    }
    OsalAllocMemArray(&RRC_MAC_MsgArray, (OSAL_PADDRESS *)&rrc_info_out);
    memcpy(rrc_info_out + MsgOffset, rrc_info, rrc_info->dataLen + sizeof(rrcInfoData));

#if 0
    {
    int i ;
    U8* p = (U8*)rrc_info_out;
    printf("Data QueueMacToRrc:Size%d :", blockSize);
    for (i=0;i<32;i++)
        printf("%x  ",p[i]);
    printf("\n");
    }
#endif

    rc = OsalEnqueue(&ToRRCQueue,rrc_info_out);
    OsalRaiseSema(&ToRRCSema);
    return rc;
}
U32 RrcGetBufferLength(PTR buf)
{
    if(buf == NULL)
        return 0;
    return ((rrcInfoData*)buf)->dataLen + sizeof(rrcInfoData);
}
U32 RrcGetBufferlcID(PTR buf)
{
    if(buf == NULL)
        return 0;
    return ((rrcInfoData*)buf)->msgType;
}
#endif /* ENODEB */

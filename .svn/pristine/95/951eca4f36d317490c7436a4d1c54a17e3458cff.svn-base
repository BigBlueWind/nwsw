/** @file LteMacRlcInterface.c
 *
 * @brief Implemenataion of interface to Message based PHY interface
 * @author Mindspeed Technologies
 * @version $Revision: 1.34 $
 *
 * COPYRIGHT(c) 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef STUB

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "osal.h"
#ifdef EHDMAC
#include "lteGlueLayerAtUe.h"
#endif
#include "LteMacRlcInterface.h"
#include "rlciface.h"
#include "apidefs.h"
#include "msgroute.h"
#include "transport.h"
#include "ltemaccore.h"
#include "rrc_types.h"
#ifdef _USRDLL
#include "apidefs.h"
#endif

#define RLC_QUEUE_LEN 1200
#define Udp_Send_Frame_Size 1400
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
void LteRlc2UpperTask(PTR pParam);
RlcContext RlcCtx;
OSAL_TASK LteRlcUpperTaskId;
OSAL_QUEUE RlcAmQueue;
OSAL_MEMARRAY Rlc_MsgArray;
OSAL_MEMARRAY Rlc_SendMsgArray;


RLCDataBuffer RlcBufferCurrent;
extern UdpSendMsgCB UdpSendMsg;
U32 TrashListEl(RLCUplinkDataInfo* pRlcData, U32 i);

RESULTCODE RlcInit()
{
    OSAL_PADDRESS RlcQueueMem;
    OSAL_PADDRESS RlcMemBlock;
    OSAL_PADDRESS RlcSendMemBlock;

    U32 QueueRcvMemSize = sizeof(RLCDataBuffer) * RLC_QUEUE_LEN;
#if defined(ARICENT_MSPD_MWC_2010) && defined(UE)
    createSocketsForVLCPlayer();
#else
    UdpProtoInit();
#endif/*ARICENT_MSPD_MWC_2010 && UE*/
    RlcCtx.Current = &RlcCtx.RlcDataTransport[0];
    RlcCtx.Next = &RlcCtx.RlcDataTransport[1];
    OsalCreateMutex(&RlcCtx.CurrentLock);
    RlcCtx.ToRlcDataBusy = FALSE;
    RlcCtx.UeIdx = 0; // GetUeId();
    if (OsalCreateSema(&RlcCtx.LteRlcTxSema) != SUCCESS)
    {
        printf("Error create LteRlcTxSema\n");
        return FAILURE;
    }
   if(OsalAllocHeap(&OsalGlobalHeap,
         &RlcQueueMem,
         RLC_QUEUE_LEN * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        printf("Rlc Init Error: cannot allocate the RLC block\n");
        return FAILURE;
    }
    if (OsalCreateQueue(&RlcAmQueue, RlcQueueMem, RLC_QUEUE_LEN * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        printf("RLC Init Error: RlcAmQueue can not be created\n");
        return FAILURE;
    }
    OsalAllocHeap(&OsalGlobalHeap, &RlcMemBlock, QueueRcvMemSize);
    OsalCreateMemArray(&Rlc_MsgArray, RlcMemBlock, QueueRcvMemSize,
                       sizeof(RLCDataBuffer));
  
    OsalAllocHeap(&OsalGlobalHeap, &RlcSendMemBlock, Udp_Send_Frame_Size * RLC_QUEUE_LEN);
    OsalCreateMemArray(&Rlc_SendMsgArray, RlcSendMemBlock, Udp_Send_Frame_Size * RLC_QUEUE_LEN,
                       Udp_Send_Frame_Size);

    RlcBufferCurrent.rlcBuffer_p = NULL;
    RlcBufferCurrent.remainingDataSize = 0;
    OsalCreateTask(&LteRlcUpperTaskId, (OSAL_PADDRESS) &LteRlc2UpperTask, NULL, "LteRlc2UpperTask",
                   OSAL_PRIORITY_NORMAL, (OSAL_PADDRESS) 0, 0);
    OsalActivateTask(&LteRlcUpperTaskId);
    RlcCtx.RlcActive = 1;
    printf("RLC init complete\n");
    return SUCCESS;
}

void RlcTxStart(void)
{
    if(RlcCtx.RlcActive == 1)
    {
    if(RlcCtx.Current->numLCInfo || RlcCtx.Next->numLCInfo)
    OsalRaiseSema(&RlcCtx.LteRlcTxSema);
    }
}

RESULTCODE RlcDataRcv(U16 lcid, PTR pBuf, OSAL_SIZE blockSize)
{
    switch(lcid)
    {
    case MAC_CCCH_LCID:
        QueueMacToRrc(&lcid, pBuf, blockSize);
        break;
    case MAC_DCCH_LCID:
        QueueMacToRrc(&lcid, pBuf, blockSize);
        break;
    default:
        if(lcid >=LCID_MIN_LCHAN && lcid <= LCID_MAX_LCHAN)
             RlcDataAdd(lcid, pBuf, blockSize);
    }
    return SUCCESS;
}

RESULTCODE RlcDataAdd(U16 lcid, U8* pMacPdu, U32 BlockLen)
{
    RLCUplinkDataInfo* pDataInfo = NULL;
#if 0
        {
            PMSGHEADER pHdr = NULL;

            if (OsalAllocMemArray(&MsgArray, (OSAL_PADDRESS *) &pHdr) != SUCCESS)
            return -1;

            pHdr->srcID = MSGS_RLC;
            pHdr->dstID = MSGS_HOST;
            pHdr->type = MSGT_DATA;
            pHdr->control = CTRL_VERSION;
            pHdr->length = BlockLen;
            memcpy(&pHdr->param, pMacPdu, BlockLen);
            IcpuSendShm(pHdr->length + sizeof(MSGHEADER), pHdr);
            OsalFreeMemArray(&MsgArray, pHdr);
            return 0;
        }
#endif /* _USRDLL */

    if (RlcCtx.ToRlcDataBusy == FALSE)
        {
             if (OsalCaptureMutex(&RlcCtx.CurrentLock, OSAL_WAIT_INFINITE) == SUCCESS)
           {
                pDataInfo = &RlcCtx.Current->rlcDataArr[RlcCtx.Current->numLCInfo++];
                if (OsalAllocMemArray(&Rlc_SendMsgArray, (OSAL_PADDRESS *) &pDataInfo->rlcdataBuffer) != SUCCESS)
                  return -1;
                memcpy(pDataInfo->rlcdataBuffer ,pMacPdu,BlockLen);
                pDataInfo->dataLength = BlockLen;
                pDataInfo->lcId = (U8)lcid;
                OsalFreeMutex(&RlcCtx.CurrentLock);
                return 0;
           }
        }
    if(pDataInfo == NULL)
    {
            pDataInfo = &RlcCtx.Next->rlcDataArr[RlcCtx.Next->numLCInfo++];
    }
     if (OsalAllocMemArray(&Rlc_SendMsgArray, (OSAL_PADDRESS *) &pDataInfo->rlcdataBuffer) != SUCCESS)
        return -1;

    memcpy(pDataInfo->rlcdataBuffer ,pMacPdu,BlockLen);
    pDataInfo->dataLength = BlockLen;
    pDataInfo->lcId = (U8)lcid;
    return 0;
}

void LteRlc2UpperTask(PTR pParam)
{
    RLCUplinkDataInfo* DataInfo = NULL;
    RLCUplinkData* temp;

    while (1)
    {
        if (OsalWaitForSema(&RlcCtx.LteRlcTxSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            if(OsalCaptureMutex(&RlcCtx.CurrentLock, OSAL_WAIT_INFINITE) == SUCCESS)
            {
                RlcCtx.ToRlcDataBusy = TRUE;
                DataInfo = RlcCtx.Current->rlcDataArr;
                while(RlcCtx.Current->numLCInfo)
                {
#if defined (UE) && defined (ARICENT_MSPD_MWC_2010)
                    macUeDataIndAtUe(RlcCtx.UeIdx, RlcCtx.Current);
                    TrashListEl(DataInfo, RlcCtx.Current->numLCInfo);
                    RlcCtx.Current->numLCInfo = 0;
                    break;
#else
                    UdpSendMsg(DataInfo->dataLength, DataInfo->rlcdataBuffer);
#endif /* UE && ARICENT_MSPD_MWC_2010 */
                    OsalFreeMemArray(&Rlc_SendMsgArray, DataInfo->rlcdataBuffer);
                    DataInfo->rlcdataBuffer = NULL;
                    RlcCtx.Current->numLCInfo--;
                    DataInfo++;
                }
                if (RlcCtx.Next->numLCInfo > 0)
                {
                    temp = RlcCtx.Current;
                    RlcCtx.Current = RlcCtx.Next;
                    RlcCtx.Next = temp;
                    RlcTxStart();
                }
                OsalFreeMutex(&RlcCtx.CurrentLock);
            }
            RlcCtx.ToRlcDataBusy = FALSE;
        }
        else
            break;
    }
}

U32 RlcGetBufferLength(PTR buf)
{
    if(buf == NULL)
        return 0;
    return ((RLCDataBuffer*)buf)->bufferLength;
}
U32 RlcGetBufferlcID(PTR buf)
{
    if(buf == NULL)
        return 0;
    return ((RLCDataBuffer*)buf)->lcID;
}

int QueueRlcToMac(U16* logicChanID, PTR pBuf, OSAL_SIZE blockSize)
{
    OSAL_SIZE msgSize;
    RLCDataBuffer* ptr = NULL;

    OsalDequeue(&RlcAmQueue,(void*)&ptr);
    if (ptr == NULL)
        return 0;
    *logicChanID = ptr->lcID;
    msgSize = RlcGetBufferLength(ptr);
    if(msgSize > blockSize)
    {
        printf("Rlc Error: Packet length is %d. Max buffer length is %d\n",msgSize,blockSize);
        OsalFreeMemArray(&Rlc_MsgArray,ptr);
        return 0;
    }
    memcpy((U8*)pBuf,ptr->rlcBuffer_p,ptr->bufferLength);
    UdpFrameFree(ptr->rlcBuffer_p);
    OsalFreeMemArray(&Rlc_MsgArray,ptr);
    return msgSize;
}

RESULTCODE LteMacRxEthPacket(PTR PacketPtr, U32 Len)
{
    static U32 Seqnum = 0;
    RLCDataBuffer* p_info = NULL;
    U32 NumEl = OsalGetElementCount(&RlcAmQueue);

    if( OsalGetElementCount(&RlcAmQueue) == (OsalGetQueueSize(&RlcAmQueue) - 1))
        return EXPIRED;

    OsalAllocMemArray(&Rlc_MsgArray, (OSAL_PADDRESS *) &p_info);
    if (p_info == NULL)
    {
        printf(C4KFW "RLC: ERROR: No memory [%s]\n", __FUNCTION__);
        return EXPIRED;
    }
    p_info->bufferLength = Len;
    p_info->rlcBuffer_p = (U8*)(p_info + 1);
    // Hardcode to avoid extra includes
    p_info->lcID = 2; //DTCH;
    p_info->remainingDataSize = 0;
    p_info->seqNumber = Seqnum++;
    p_info->rlcBuffer_p = PacketPtr;
    if (OsalEnqueue(&RlcAmQueue, p_info) == SUCCESS)
    {
        return NumEl+4;//FAILURE;
    }
    OsalFreeMemArray(&Rlc_MsgArray, p_info);
    return SUCCESS;
}


/**
 * Function to send packets to Ethernet layer
 * @param pPacket - pointer to the packet descriptor
 * @return SUCCESS/FAILURE - result of operation
 */
U32 TrashListEl(RLCUplinkDataInfo* pRlcData, U32 i)
{
    if (i <= 0)
        return i;
    i--;
    TrashListEl((pRlcData + 1), i);
    OsalFreeMemArray(&Rlc_SendMsgArray, pRlcData->rlcdataBuffer);
    pRlcData->rlcdataBuffer = NULL;
    return i;
}
RESULTCODE RlcDone()
{
    UdpProtoDone();
    if (LteRlcUpperTaskId)
        OsalDestroyTask(&LteRlcUpperTaskId);

    LteRlcUpperTaskId = 0;
    OsalDestroySema(&RlcCtx.LteRlcTxSema);
    OsalFreeHeap(&OsalGlobalHeap, &Rlc_MsgArray);
    OsalDestroyQueue(&RlcAmQueue);
    OsalFreeHeap(&OsalGlobalHeap, &Rlc_SendMsgArray);
    return SUCCESS;
}

#endif // STUB

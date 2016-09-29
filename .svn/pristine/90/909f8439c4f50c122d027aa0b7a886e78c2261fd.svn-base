//-------------------------------------------------------------------------------------------
/** @file Intercpu.c
 *
 * @brief This file contains definition of inter-cpu API used in application
 * @author Mindspeed Technologies
 * @version $Revision: 1.29 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "intercpu.h"
#include "appprintf.h"
#include <string.h>
#include "apimgr.h"
#include "4gmx.h"
#include "heaps.h"
#include "hbidrv.h"
#include "sirqassign.h"
#include "icdrv.h"
#include "systimers.h"
#include "initcpu.h"
#include "icpu2.h"
#include "lcorecfg.h"

//#define I_CPU_CHECK_DELIVERING    

/*********************************************************************
*               The module global data                               *
*********************************************************************/

SICMemMap *MemMap = NULL;
static ICCTX ic_ctx;

/** @brief This function frees allocated resources for opened inter-cpu channel

    @NOTE  The memory for context will be deallocated as well!!!

    @param pCtx [in] - the pointer to allocated channel context

    *  \ingroup group_lte_hal

    */

void ICReleaseChannelCtx (ICChannelCtx * pCtx)
{
    _ASSERT(pCtx != NULL);

    if (HANDLE_NOT_NULL(pCtx->hHandler))
    {
        MxRemoveThread(pCtx->hHandler);
    }

    if (pCtx->ChanID == IC_CHAN_SVSR)
    {
        if (HANDLE_NOT_NULL(pCtx->hRxPart))
        {
            MxRemovePart(pCtx->hRxPart);
        }

        if (HANDLE_NOT_NULL(pCtx->hTxPart))
        {
            MxRemovePart(pCtx->hTxPart);
        }

        if (HANDLE_NOT_NULL(pCtx->hRxQueue))
        {
            MxRemoveQueue(pCtx->hRxQueue);
        }

        if (HANDLE_NOT_NULL(pCtx->hTxQueue))
        {
            MxRemoveQueue(pCtx->hTxQueue);
        }
    }
    else if (pCtx->ChanID == IC_CHAN_SVSR_HBI)
    {
         HBIDRVCTX * pHbiCtx = HBIDrvGetCtx ();

         pHbiCtx->hThr = HNULL;
    }

    MxHeapFree(AppHeap, pCtx);

    memset (&ic_ctx, 0, sizeof (ic_ctx));
    
}

/** @brief  This function is an inter-cpu interrupt handler that
            schedules appropriate thread for processing inter-cpu data

    @param pClear [in] - the pointer to the interrupt confirmation register
    @param pParam [in] - the pointer to the data of "ICIrqData" type

    *  \ingroup group_lte_hal

    */

void InterCpuIntHandle(volatile UINT32 * pClear, UINT32 nIrqID, LPVOID pParam)
{
#ifdef _MX_DEBUG
    MXRC rc = MX_OK;
#endif

    ICChannelCtx * pCtx;

    pCtx = (ICChannelCtx*)pParam;

    _ASSERT(pCtx != NULL);

    _ASSERT(HANDLE_NOT_NULL(pCtx->hHandler));

    // This function should be called to clean
    // inter-cpu irq

    ICDrvCleanInterCpu (pCtx->SrcIrqID);

    /* To schedule the IC handler */

#ifdef _MX_DEBUG
    rc = MxScheduleThread(pCtx->hHandler);
#else
    MxScheduleThread(pCtx->hHandler);
#endif

    _ASSERT_RC(rc);

    /* Ack the interrupt */
    *pClear = nIrqID;
}

/** @brief The function inits inter-cpu manager, this function should be called
           before usage of any API declared in this header.

    @return [MXRC] error code of initialization

    *  \ingroup group_lte_hal

    */

MXRC ICInit(void)
{
    MXRC rc = MX_OK;

    UINT32 BaseAddr = CoreCfg.ICPUBase;
    memset (&ic_ctx, 0, sizeof (ic_ctx));
    ic_ctx.BaseAddr = BaseAddr;
    MemMap = (SICMemMap *)BaseAddr;

//#define LOWERARM_DEBUG_PRINT

#if defined(LOWERARM_DEBUG_PRINT)


    printf(" MemMap->ToUpArmCtrlObj.queue.Storage  =%x\r\n",
           MemMap->ToUpArmCtrlObj.queue.Storage);
    printf(" MemMap->ToUpArmCtrlObj.queue.GetIndex =%x\r\n",
           MemMap->ToUpArmCtrlObj.queue.GetIndex);
    printf(" MemMap->ToUpArmCtrlObj.queue.PutIndex =%x\r\n",
           MemMap->ToUpArmCtrlObj.queue.PutIndex);
    printf(" MemMap->ToUpArmCtrlObj.queue.QueueSize=%x\r\n",
           MemMap->ToUpArmCtrlObj.queue.QueueSize);
    printf(" MemMap->ToUpArmCtrlObj.queue.ElmSize  =%x\r\n",
           MemMap->ToUpArmCtrlObj.queue.ElmSize);
    printf(" MemMap->ToUpArmCtrlObj.queue.Sync     =%x\r\n",
           MemMap->ToUpArmCtrlObj.queue.Sync);

    printf("\r\n");

    printf(" MemMap->ToUpArmCtrlObj.part.Storage  =%x\r\n",
           MemMap->ToUpArmCtrlObj.part.Storage);
    printf(" MemMap->ToUpArmCtrlObj.part.FreeBlock=%x\r\n",
           MemMap->ToUpArmCtrlObj.part.FreeBlock);
    printf(" MemMap->ToUpArmCtrlObj.part.BlkSize  =%x\r\n",
           MemMap->ToUpArmCtrlObj.part.BlkSize);
    printf(" MemMap->ToUpArmCtrlObj.part.BlkCount =%x\r\n",
           MemMap->ToUpArmCtrlObj.part.BlkCount);
    printf(" MemMap->ToUpArmCtrlObj.part.Sync     =%x\r\n",
           MemMap->ToUpArmCtrlObj.part.Sync);
    printf(" MemMap->ToUpArmCtrlObj.part.AllocNum =%x\r\n",
           MemMap->ToUpArmCtrlObj.part.AllocNum);

    printf("\r\n");

    printf(" MemMap->FromUpArmCtrlObj.queue.Storage  =%x\r\n",
           MemMap->FromUpArmCtrlObj.queue.Storage);
    printf(" MemMap->FromUpArmCtrlObj.queue.GetIndex =%x\r\n",
           MemMap->FromUpArmCtrlObj.queue.GetIndex);
    printf(" MemMap->FromUpArmCtrlObj.queue.PutIndex =%x\r\n",
           MemMap->FromUpArmCtrlObj.queue.PutIndex);
    printf(" MemMap->FromUpArmCtrlObj.queue.QueueSize=%x\r\n",
           MemMap->FromUpArmCtrlObj.queue.QueueSize);
    printf(" MemMap->FromUpArmCtrlObj.queue.ElmSize  =%x\r\n",
           MemMap->FromUpArmCtrlObj.queue.ElmSize);
    printf(" MemMap->FromUpArmCtrlObj.queue.Sync     =%x\r\n",
           MemMap->FromUpArmCtrlObj.queue.Sync);

    printf("\r\n");

    printf(" MemMap->FromUpArmCtrlObj.part.Storage  =%x\r\n",
           MemMap->FromUpArmCtrlObj.part.Storage);
    printf(" MemMap->FromUpArmCtrlObj.part.FreeBlock=%x\r\n",
           MemMap->FromUpArmCtrlObj.part.FreeBlock);
    printf(" MemMap->FromUpArmCtrlObj.part.BlkSize  =%x\r\n",
           MemMap->FromUpArmCtrlObj.part.BlkSize);
    printf(" MemMap->FromUpArmCtrlObj.part.BlkCount =%x\r\n",
           MemMap->FromUpArmCtrlObj.part.BlkCount);
    printf(" MemMap->FromUpArmCtrlObj.part.Sync     =%x\r\n",
           MemMap->FromUpArmCtrlObj.part.Sync);
    printf(" MemMap->FromUpArmCtrlObj.part.AllocNum =%x\r\n",
           MemMap->FromUpArmCtrlObj.part.AllocNum);

    printf("\r\n");
#endif

    return rc;
}

/** @brief This function returns the total size of I-CPU objects

    @param BaseAddr [in] - the base address of I-CPU space

    @return [UINT32] the total size of I-CPU objects

    *  \ingroup group_lte_hal

    */

UINT32 ICGetICpuSize (UINT32 BaseAddr)
{
    UINT32 nSize = 0;
    //UINT32 nMaxAddr;

    SICMemMap * p = (SICMemMap *)BaseAddr;

    nSize += p->ToUpArmCtrlObj.queue.QueueSize * p->ToUpArmCtrlObj.queue.ElmSize;
    nSize += p->FromUpArmCtrlObj.queue.QueueSize * p->FromUpArmCtrlObj.queue.ElmSize;

    nSize += p->ToUpArmCtrlObj.part.BlkSize * p->ToUpArmCtrlObj.part.BlkCount;
    nSize += p->FromUpArmCtrlObj.part.BlkSize * p->FromUpArmCtrlObj.part.BlkCount;

    nSize += sizeof (SICMemMap);

    return nSize;
}

UINT32 ICGetRXPartStorageAddr (UINT32 BaseAddr, UINT32 icpu_id)
{
    SICMemMap * p = (SICMemMap *)BaseAddr;

    if (icpu_id == IC_CHAN_SVSR_EX_ICPU)
        return ICpuGetRxStorageBase (BaseAddr);

    return (UINT32)p->FromUpArmCtrlObj.part.Storage;
}

UINT32 ICGetRXPartStorageSize (UINT32 BaseAddr, UINT32 icpu_id)
{
    SICMemMap * p = (SICMemMap *)BaseAddr;

    if (icpu_id == IC_CHAN_SVSR_EX_ICPU)
        return ICpuGetRxStorageSize ();

    return p->FromUpArmCtrlObj.part.BlkCount * p->FromUpArmCtrlObj.part.BlkSize;
}

UINT32 ICGetTXPartStorageAddr (UINT32 BaseAddr, UINT32 icpu_id)
{
    SICMemMap * p = (SICMemMap *)BaseAddr;

    if (icpu_id == IC_CHAN_SVSR_EX_ICPU)
        return ICpuGetTxStorageBase (BaseAddr);

    return (UINT32)p->ToUpArmCtrlObj.part.Storage;
}

UINT32 ICGetTXPartStorageSize (UINT32 BaseAddr, UINT32 icpu_id)
{
    SICMemMap * p = (SICMemMap *)BaseAddr;

    if (icpu_id == IC_CHAN_SVSR_EX_ICPU)
        return ICpuGetTxStorageSize ();

    return p->ToUpArmCtrlObj.part.BlkCount * p->ToUpArmCtrlObj.part.BlkSize;
}

/** @brief The function opens specified by the id inter-cpu channel and
           creates a thread for processing incomming data.

    @param nCommID[in] - the id of inter-cpu channel
    @param hand   [in] - the pointer to function, this function will be
                         scheduled for execution every time when new data
                         be precent in opened channel. It can be NULL.
    @param pParam [in] - the pointer to handler parameter

    @param pChannel[out]-the descriptor of opened channel

    @return [MXRC] error code

    *  \ingroup group_lte_hal

    */

MXRC ICOpenChannel (ICChannelID nCommID, ICHandler hand, LPVOID pParam,
                    ICChannelCtx ** pChannel)

{
    ICChannelCtx * pCtx;
    MXRC rc = MX_OK;
    UINT32 dst_irq = 0, src_irq = 0, cache = 0, part_cache = 1;

    HBIDRVCTX * pHbiCtx = NULL;

    //SICMemMap *MemMap = NULL;

    //MemMap = (SICMemMap *) INTER_CPU_MEM_ADDRESS;

    //uart_printf ("INTER-CPU:Open > Cpu=%d, id=%d, Mem=%x\r\n", MxGetCpuID (), nCommID, MemMap);

    _ASSERT ((MemMap != NULL) || (nCommID != IC_CHAN_SVSR));

    _ASSERT(pChannel != NULL);

    pCtx = (ICChannelCtx *)MxHeapAlloc(AppHeap, sizeof (ICChannelCtx));

    if (pCtx == NULL)
        return IC_RC_NOT_MEMORY;

    memset (pCtx, 0, sizeof (ICChannelCtx));

    pCtx->SrcIrqID = 0xFFFFFFFF;
    pCtx->DstIrqID = 0xFFFFFFFF;

    ic_ctx.nInfID = nCommID;
    ic_ctx.CpuCtx[MxGetCpuID()] = pCtx;

    switch (nCommID)
    {
        case IC_CHAN_SVSR:
            {
                dst_irq = SOFT_ICPU_TO_UARM;
                src_irq = SOFT_ICPU_FROM_UARM;

                pCtx->DstIrqID = dst_irq;
                pCtx->SrcIrqID = src_irq;

                ic_ctx.nDstIrqID = dst_irq;
                ic_ctx.nSrcIrqID  = src_irq;

                // for the UARM+LARM we need to use HW semas

                if (MxGetARMID () != 0 || 1)
                {
                	rc = MxOpenQueueEx(&(MemMap->ToUpArmCtrlObj.queue), NULL,
                				(PUINT32)MemMap->ToUpArmCtrlObj.queue.Sync, cache,
                                  &pCtx->hTxQueue);
                }
                else
                {
                	rc = MxOpenQueue(&(MemMap->ToUpArmCtrlObj.queue), NULL, &pCtx->hTxQueue);
                }

                if (FAILED(rc))
                {
                    ICReleaseChannelCtx (pCtx);
                    return rc;
                }

                MxQueueConfig (pCtx->hTxQueue, MX_QUEUE_CFG_LOCK_TIMEOUT, ICPU_LOCK_TX_TIMEOUT);

                // for the UARM+LARM we need to use HW semas

                if (MxGetARMID () != 0 || 1)
                {
                	rc = MxOpenPartEx(&(MemMap->ToUpArmCtrlObj.part), NULL,
                			(LPVOID)MemMap->ToUpArmCtrlObj.part.Sync, part_cache, &pCtx->hTxPart);
                }
                else
                {
                	rc = MxOpenPart(&(MemMap->ToUpArmCtrlObj.part), NULL,
                                 &pCtx->hTxPart);
                }

                if (FAILED(rc))
                {
                    ICReleaseChannelCtx (pCtx);
                    return rc;
                }

                // for the UARM+LARM we need to use HW semas
                if (MxGetARMID () != 0 || 1)
                {
                	rc = MxOpenQueueEx(&(MemMap->FromUpArmCtrlObj.queue), NULL,
                			(PUINT32)MemMap->FromUpArmCtrlObj.queue.Sync,  cache, &pCtx->hRxQueue);
                }
                else
                {
                	rc = MxOpenQueue(&(MemMap->FromUpArmCtrlObj.queue), NULL,
                                  &pCtx->hRxQueue);
                }

                if (FAILED(rc))
                {
                    ICReleaseChannelCtx (pCtx);
                    return rc;
                }

                MxQueueConfig (pCtx->hRxQueue, MX_QUEUE_CFG_LOCK_TIMEOUT, ICPU_LOCK_RX_TIMEOUT);

                // for the UARM+LARM we need to use HW semas
                if (MxGetARMID () != 0 || 1)
                {
                	rc = MxOpenPartEx(&(MemMap->FromUpArmCtrlObj.part), NULL,
                			(PUINT32)MemMap->FromUpArmCtrlObj.part.Sync, part_cache, &pCtx->hRxPart);

                }
                else
                {
                	rc = MxOpenPart(&(MemMap->FromUpArmCtrlObj.part), NULL,
                	                                 &pCtx->hRxPart);
                }

                if (FAILED(rc))
                {
                    ICReleaseChannelCtx (pCtx);
                    return rc;
                }

                pCtx->ChanID = nCommID;

                break;
            }

        case IC_CHAN_SVSR_HBI:
            {
            	pHbiCtx = HBIDrvGetCtx ();

                src_irq = 0xFFFFFFFF;

                pCtx->SrcIrqID = src_irq;
                pCtx->DstIrqID = src_irq;

                pCtx->hRxQueue = pHbiCtx->RxQ;
                pCtx->hTxQueue = pHbiCtx->TxQ;
                pCtx->hRxPart  = pHbiCtx->RxP;
                pCtx->hTxPart  = pHbiCtx->TxP;

                pCtx->ChanID = nCommID;

                break;
            }

        case IC_CHAN_SVSR_EX_ICPU:
            {
                dst_irq = SOFT_ICPU_TO_UARM;
                src_irq = SOFT_ICPU_FROM_UARM;

                pCtx->DstIrqID = dst_irq;
                pCtx->SrcIrqID = src_irq;

                ic_ctx.nDstIrqID = dst_irq;
                ic_ctx.nSrcIrqID  = src_irq;

                pCtx->ChanID = nCommID;

                if (ic_ctx.bInited == 0)
                {
                    rc = ICpuInit((LPVOID)ic_ctx.BaseAddr);

                    if (FAILED(rc))
                        return rc;
                }

                break;
            }

        default:
            {
                ICReleaseChannelCtx (pCtx);
                return IC_RC_CHAN_ID_ERROR;
            }
    }

    // the interrupt can be generated in any case
    // even if source IRQ is processed on other ARM

    pCtx->DstIrqID  = dst_irq;

    if (hand != NULL)
    {
        ic_ctx.Proc = hand;
    
        pCtx->SrcIrqID  = src_irq;

        rc = MxCreateThread((TENTRY)hand, NULL, pCtx, (TPRIORITY)3,
                             &pCtx->hHandler);

        if (FAILED(rc))
        {
            ICReleaseChannelCtx (pCtx);
            return rc;
        }

        if (pHbiCtx != NULL)
            HBIDrvSetRxQueueHandler(pCtx->hHandler);

        pCtx->HandParam = pParam;

        // to set the inter-cpu interrupt handler
        // and to specify the parameter of the thread that should process
        // incoming API command

        if (src_irq != 0xFFFFFFFF)
        {
            ic_ctx.nMasterCpuID = MxGetCpuID ();
                
            rc = MxSetIntHandler(src_irq, INT_TYPE_IRQ,
                                  PRI_ICPU, InterCpuIntHandle, pCtx);

            if (FAILED(rc))
            {
                ICReleaseChannelCtx (pCtx);
                return rc;
            }

            // to enable the specified inter-cpu interrupt

            rc = MxEnableInt(src_irq);

            if (FAILED(rc))
            {
                ICReleaseChannelCtx (pCtx);
                return rc;
            }
        }

        ic_ctx.bInited = 1;
    }

    *pChannel = pCtx;

    return rc;
}


/** @bried The function removes allocated system resource
           and disables inter-cpu interrupt that was assigned for
           opened channel.

    @param pChannel [in] - the descriptor of opened channel

    @return [MXRC] error code

    *  \ingroup group_lte_hal

    */

MXRC ICCloseChannel (ICChannelCtx * pChannel)
{
    MXRC rc = MX_OK;

    _ASSERT(pChannel != NULL);

    if (pChannel->SrcIrqID != 0xFFFFFFFF)
    {
        rc = MxDisableInt(pChannel->SrcIrqID);
    }

    ICReleaseChannelCtx(pChannel);

    return rc;
}

/** @brief The function returns size of communication channel block used for
           data sending/receiving. In case of non message-oriented comm. chan.,
           the size of block will be 0xFFFFFFFF bytes.

    @param pChannel [in] - the context of communication channel

    @return [UINT32] size of block

    *  \ingroup group_lte_hal

    */

UINT32 ICGetSendBlockSize (ICChannelCtx * pChannel)
{
    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
        return ICpuGetTxBlockSize ();

    return MxGetPartBlockSize(pChannel->hTxPart);
}

/** @brief The function returns number of communication channel blocks used to
           send/receive data. In case of non message-oriented comm. chan.,
           the number is equal to 0.

    @param pChannel [in] - the context of communication channel

    @return [UINT32] size of block

    *  \ingroup group_lte_hal

    */

UINT32 ICGetRecvBlockCount (ICChannelCtx * pChannel)
{
    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
        return ICpuGetRxBlockNum ();

    return MxGetPartBlockCount(pChannel->hRxPart);
}

/** @brief The function returns size of communication channel block used for
           data sending/receiving. In case of non message-oriented comm. chan.,
           the size of block will be 0xFFFFFFFF bytes.

    @param pChannel [in] - the context of communication channel

    @return [UINT32] size of block

    *  \ingroup group_lte_hal

    */

UINT32 ICGetRecvBlockSize (ICChannelCtx * pChannel)
{
    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
        return ICpuGetRxBlockSize ();

    return MxGetPartBlockSize(pChannel->hRxPart);
}

/** @brief This function returns a number of accessible TX blocks in
           communication channel

    @param pChannel [in] - the descriptor of communication channel

    @return [UINT32]  number of block

    *  \ingroup group_lte_hal

    */

UINT32 ICGetTxFreeBlocks (ICChannelCtx * pChannel)
{
    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
        return ICpuGetTxFreeBlock ();

    return MxGetFreePartBlocks(pChannel->hTxPart);
}

/** @brief This function returns a number of accessible RX blocks in
           communication channel

    @param pChannel [in] - the descriptor of communication channel

    @return [UINT32]  number of block

    *  \ingroup group_lte_hal

    */

UINT32 ICGetRecvFreeBlocks (ICChannelCtx * pChannel)
{
    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
        return ICpuGetRxFreeBlock ();

    return MxGetFreePartBlocks(pChannel->hRxPart);
}




/** @brief The function sends API command to the upper ARM by using
           inter-cpu communication objects.

    @param IntID[in] - the ID of inter-cpu communication interface
    @param pData[in] - the pointer to the API command
    @param nSize[in] - the size of data pointer by the pData argument

    @return [MXRC] error code

    *  \ingroup group_lte_hal

    */

MXRC ICWriteData (ICChannelCtx * pChannel, LPVOID pData, UINT32 nSize)
{
    UINT16 nIrqNum;
    MXRC   rc;

    HANDLE hPart;
    HANDLE hQueue;

    UINT32 nCopySize = 0, nBlockSize;
    LPVOID pBlock;

    INTC_CPU_IDS  cpudest;

    _ASSERT(pData != NULL);

    switch (pChannel->ChanID)
    {
        case IC_CHAN_SVSR:
            {
                hPart   = pChannel->hTxPart;
                hQueue  = pChannel->hTxQueue;
                nIrqNum = pChannel->DstIrqID;

                cpudest = 0;

                break;
            }
        default:
            return IC_RC_CHAN_ID_ERROR;
    }

    nBlockSize = MxGetPartBlockSize(hPart);

    nCopySize = MIN(nSize, nBlockSize);

    pBlock = MxAllocBlock(hPart);

    if (pBlock == NULL)
    {
        _ASSERT(pBlock != NULL);

        return IC_RC_NOT_MEMORY;
    }

    memcpy(pBlock, pData, nCopySize);

    rc = MxPutQueueData(hQueue,
                         (LPVOID)MxPartPointerToIndex(hPart, pBlock));

    if (FAILED(rc))
    {
        MxFreeBlock(hPart, pBlock);
        return rc;
    }

    rc = MxRaiseInterCpu(cpudest, nIrqNum);

    _ASSERT_RC(rc);

    return rc;
}

/** @brief The function reads data stored in the inter-cpu channel

    @param pChannel [in]  - the channel descriptor
    @param pData    [out] - the received data
    @param pnBufSize[in/out] - in: the buffer length
                              out: the size of received data

    @return [MXRC] error code

    *  \ingroup group_lte_hal

    */

MXRC ICReadData (ICChannelCtx * pChannel, LPVOID pData, PUINT32 pnBufSize)
{
    HANDLE hPart;
    HANDLE hQueue;

    UINT32 nIndex, nBlockSize, nApiSize = 0, nCopySize;
    MXRC   rc;
    LPVOID pBlock;

    _ASSERT(pChannel != NULL);
    _ASSERT(pData != NULL);
    _ASSERT(pnBufSize != NULL);

    switch (pChannel->ChanID)
    {
        case IC_CHAN_SVSR:
            {
                hPart = pChannel->hRxPart;
                hQueue= pChannel->hRxQueue;

                break;
            }
        default:
            return IC_RC_CHAN_ID_ERROR;
    }

    nBlockSize = MxGetPartBlockSize(hPart);

    // To try receive the API command

    rc = MxGetQueueData(hQueue, &nIndex);

    // The queue can be empty

    if (FAILED(rc))
        return rc;

    pBlock = MxPartIndexToPointer(hPart, nIndex);

    _ASSERT(pBlock != NULL);

    switch (pChannel->ChanID)
    {
        case IC_CHAN_SVSR:
            {
                nApiSize = ((UINT32)((ApiHeader*)pBlock)->Length +
                                     sizeof(ApiHeader));
                break;
            }
        default:
            {
                nApiSize = nBlockSize;
                break;
            }
    }

    _ASSERT(nApiSize <= *pnBufSize);

    nCopySize = MIN(nApiSize, nBlockSize);

    memcpy(pData, pBlock, nCopySize);

    rc = MxFreeBlock(hPart, pBlock);

    _ASSERT_RC(rc);

    return rc;
}

/**@brief The function returns a pointer to the received data, the data
          are stored in communication channel and should be deallocated
          by calling free function.

   @param pChannel [in] - the context of communication channel

   @return [LPVOID] the pointer to the receive data

   *  \ingroup group_lte_hal

   */

LPVOID ICLockRxData (ICChannelCtx * pChannel)
{
    UINT32 nIndex;
#ifdef _MX_DEBUG
    MXRC   rc;
#endif

    _ASSERT(pChannel != NULL);

    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
        return ICpuLockRxData ();

#ifdef _MX_DEBUG
    if (FAILED(rc = MxGetQueueData(pChannel->hRxQueue, &nIndex)))
    {
        _ASSERT(rc == MX_QUEUE_EMPTY);
        return NULL;
    }
#else
    if (FAILED(MxGetQueueData(pChannel->hRxQueue, &nIndex)))
        return NULL;
#endif

    // to return the pointer to the received data
    return MxPartIndexToPointer(pChannel->hRxPart, nIndex);
}

/**@brief The function frees locked received data.

   @param pChannel [in] - the context of communication channel
   @param pData    [in] - the return value of ICLockRxData function

   @return [LPVOID] the pointer to the receive data

   *  \ingroup group_lte_hal

   */

#ifdef SYS_TRACER_ENABLED
    MXRC ICFreeRxData (ICChannelCtx * pChannel, LPVOID pData)
    {
        UINT32 lr = MX_GET_RADDR;
        return ICFreeRxDataLR(pChannel, pData, lr);
    }

    MXRC ICFreeRxDataLR (ICChannelCtx * pChannel, LPVOID pData, UINT32 lr)
#else
    MXRC ICFreeRxData (ICChannelCtx * pChannel, LPVOID pData)
#endif
{
    MXRC rc;

    _ASSERT(pChannel != NULL);
    _ASSERT(pData != NULL);

    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
        return ICpuFreeRxData(pData);

    #ifndef SYS_TRACER_ENABLED
        rc = MxFreeBlock(pChannel->hRxPart, pData);
    #else
        rc = MxFreeBlockLR(pChannel->hRxPart, pData, lr);
    #endif

    _ASSERT_RC(rc);

    return rc;
}

/**@brief The function returns a pointer to the allocated block in send partition.

   @param pChannel [in] - the context of communication channel

   @return [LPVOID] the pointer to the allocated block

   *  \ingroup group_lte_hal

   */

#ifdef SYS_TRACER_ENABLED
    LPVOID ICLockTxData (ICChannelCtx * pChannel)
    {
        UINT32 lr = MX_GET_RADDR;
        return ICLockTxDataLR(pChannel, lr);
    }

    LPVOID ICLockTxDataLR (ICChannelCtx * pChannel, UINT32 lr)
#else
    LPVOID ICLockTxData (ICChannelCtx * pChannel)
#endif
{
    _ASSERT(pChannel != NULL);

    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
        return ICpuLockTxData();

    #ifdef SYS_TRACER_ENABLED
        return MxAllocBlockLR(pChannel->hTxPart, lr);
    #else
        return MxAllocBlock(pChannel->hTxPart);
    #endif
}

/** @brief The function returns a pointer to the allocated data of TX storage
    @param pChannel [in] - the pointer to the communication channel

    @return [LPVOID] the pointer to the allocated block

    *  \ingroup group_lte_hal

    */

#ifdef SYS_TRACER_ENABLED
    LPVOID ICAllocTxData (ICChannelCtx * pChannel)
    {
        UINT32 lr = MX_GET_RADDR;
        return ICAllocTxDataLR(pChannel, lr);
    }

    LPVOID ICAllocTxDataLR (ICChannelCtx * pChannel, UINT32 lr)
#else
    LPVOID ICAllocTxData (ICChannelCtx * pChannel)
#endif
{
    _ASSERT(pChannel != NULL);

    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
        return ICpuLockTxData();

    #ifdef SYS_TRACER_ENABLED
        return MxAllocBlockLR(pChannel->hTxPart, lr);
    #else
        return MxAllocBlock(pChannel->hTxPart);
    #endif
}

/** @brief The function returns a pointer to the allocated data of RX storage
    @param pChannel [in] - the pointer to the communication channel

    @return [LPVOID] the pointer to the allocated block

    *  \ingroup group_lte_hal

    */

LPVOID ICAllocRxData (ICChannelCtx * pChannel)
{
    _ASSERT(pChannel != NULL);

    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
        return ICpuAllocRxData();

    return MxAllocBlock(pChannel->hRxPart);
}

/**@brief The function returns a pointer to the allocated block in send partition.

   @param pChannel [in] - the context of communication channel
   @param pData    [in] - the data allocated by the ICLockTxData function
   @param bNotify  [in] - notify/don't notify  the remote side

   @return [LPVOID] the pointer to the allocated block

   *  \ingroup group_lte_hal

   */

MXRC ICSendHiTxData (ICChannelCtx * pChannel, LPVOID pData, BOOL bNotify)
{
    _ASSERT(pChannel != NULL);

    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
    {
        return ICpuSendHiTxData(pData, bNotify);
    }

    return IC_RC_NON_SUPPORTED;
}

/**@brief The function returns a pointer to the allocated block in send partition.

   @param pChannel [in] - the context of communication channel
   @param pData    [in] - the data allocated by the ICLockTxData function
   @param bNotify  [in] - notify/don't notify  the remote side

   @return [LPVOID] the pointer to the allocated block

   *  \ingroup group_lte_hal

   */

MXRC ICSendTxData (ICChannelCtx * pChannel, LPVOID pData, BOOL bNotify)
{
    MXRC   rc = MX_OK;

#ifdef I_CPU_CHECK_DELIVERING    
    UINT32 t;
#endif

    _ASSERT(pChannel != NULL);

    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
    {
        return ICpuSendTxData(pData, bNotify);
    }
    else if (pChannel->ChanID == IC_CHAN_SVSR)
    {
        rc = MxPutQueueData(pChannel->hTxQueue,
                            (LPVOID)MxPartPointerToIndex(pChannel->hTxPart,
                            pData));

        if (FAILED(rc))
            return rc;

        if (bNotify)
            rc = ICDrvRaiseInterCpu (0, pChannel->DstIrqID);

#ifdef I_CPU_CHECK_DELIVERING
        t = MxGetTicks();

        while (MemMap->ToUpArmCtrlObj.queue.GetIndex != MemMap->ToUpArmCtrlObj.queue.PutIndex)
        {
            _NOP ();
            _NOP ();

            if (MxGetTicks () - t > 150* 1000)
            {
                uart_printf ("U-ARM does hang!\r\n");
                while (1);
            }
        }
#endif
        
    }
    else if (pChannel->ChanID == IC_CHAN_SVSR_HBI)
    {
        // At this point we do not know the size of data,
        // the HBI driver operates with API commands only,
        // so, the real size will be calculated latter,

        if (HBIDrvXmitMsg (pData, 0) != HBI_RC_OK)
        {
            rc = IC_RC_SEND_ERROR;
        }
    }

    return rc;
}

/** @brief This function puts data to the incoming channel and generates event to
            the local system (to own processor) to emulate remote I-CPU sender

    @param pChannel [in] - the pointer to the channel context
    @param pData    [in] - the pointer to the input data
    @param bNotify  [in] - the flag of event generating

    @return [MXRC] an error code
    \ingroup group_lte_hal

    */

MXRC ICAddRxMessage (ICChannelCtx * pChannel, LPVOID pData, BOOL bNotify)
{
    MXRC   rc = MX_OK;

    _ASSERT(pChannel != NULL);

    if (pChannel->ChanID == IC_CHAN_SVSR)
    {
        rc = MxPutQueueData(pChannel->hRxQueue,
                            (LPVOID)MxPartPointerToIndex(pChannel->hRxPart,
                            pData));

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        if (bNotify)
        {
            rc = ICDrvRaiseInterCpu (0, pChannel->SrcIrqID);
            _ASSERT_RC(rc);
        }
    }
    else if (pChannel->ChanID == IC_CHAN_SVSR_HBI)
    {
        rc = IC_RC_NON_SUPPORTED;
        _ASSERT_RC(rc);
    }
    else if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
    {
        rc = IcpuAddToRxQueue (pData, bNotify);
    }

    return rc;
}


/** @brief The function frees allocated TX block of communication channel

    @param pChannel [in] - the context of communication channel
    @param pData    [in] - the allocated by the ICLockTxData function block

    @return [MXRC] error code

    *  \ingroup group_lte_hal

    */

#ifdef SYS_TRACER_ENABLED
    MXRC ICFreeTxData (ICChannelCtx * pChannel, LPVOID pData)
    {
        #ifdef SYS_TRACER_ENABLED
        UINT32 lr = MX_GET_RADDR;
        #endif
        
        return ICFreeTxDataLR(pChannel, pData, lr);
    }

    MXRC ICFreeTxDataLR (ICChannelCtx * pChannel, LPVOID pData, UINT32 lr)
#else
    MXRC ICFreeTxData (ICChannelCtx * pChannel, LPVOID pData)
#endif
{
    _ASSERT_PTR(pChannel);
    _ASSERT_PTR(pData);

    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
        return ICpuFreeTxData(pData);

    #ifndef SYS_TRACER_ENABLED
        return MxFreeBlock(pChannel->hTxPart, pData);
    #else
        return MxFreeBlockLR(pChannel->hTxPart, pData, lr);
    #endif
}

UINT32 ICIsTxMemory (ICChannelCtx * pChannel, LPVOID pData)
{
    _ASSERT_PTR(pChannel);
    _ASSERT_PTR(pData);

    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
        return ICpuIsTxData(pData);

    return MxIsPartMemory(pChannel->hTxPart, pData);
}

UINT32 ICIsRxMemory (ICChannelCtx * pChannel, LPVOID pData)
{
    _ASSERT_PTR(pChannel);
    _ASSERT_PTR(pData);

    if (pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
        return ICpuIsRxData(pData);

    return MxIsPartMemory(pChannel->hRxPart, pData);
}

MXRC GetICMemObjects(SICMemMap * pInfo)
{
    _ASSERT_PTR (pInfo);

    if (MemMap == NULL)
        return IC_RC_NOT_INITED;

    if (ic_ctx.nInfID == IC_CHAN_SVSR_EX_ICPU)
        return IC_RC_NON_SUPPORTED;

    *pInfo = *MemMap;

    return MX_OK;
}

MXRC GetExtICMemObjects(ICPU_OBJ_CTX * pInfo)
{
    _ASSERT_PTR (pInfo);

    if (ic_ctx.nInfID != IC_CHAN_SVSR_EX_ICPU)
        return IC_RC_NON_SUPPORTED;
    
   *pInfo = *(ICPU_OBJ_CTX *)ic_ctx.BaseAddr;
    return MX_OK;
}

UINT32 GetICpuRegionSize(UINT32 BaseAddr, UINT32 icpu_id)
{
    SICMemMap * p = (SICMemMap *)BaseAddr;
    UINT32 nEnd = 0;

    if (icpu_id == IC_CHAN_SVSR_EX_ICPU)
    {
        return ICpuGetTotalStorageSize ();
    }

    nEnd = MAX ((UINT32)p->FromUpArmCtrlObj.queue.Storage + p->FromUpArmCtrlObj.queue.ElmSize * p->FromUpArmCtrlObj.queue.QueueSize, nEnd);
    nEnd = MAX ((UINT32)p->FromUpArmCtrlObj.part.Storage + p->FromUpArmCtrlObj.part.BlkSize * p->FromUpArmCtrlObj.part.BlkCount, nEnd);

    nEnd = MAX ((UINT32)p->ToUpArmCtrlObj.queue.Storage + p->ToUpArmCtrlObj.queue.ElmSize * p->ToUpArmCtrlObj.queue.QueueSize, nEnd);
    nEnd = MAX ((UINT32)p->ToUpArmCtrlObj.part.Storage + p->ToUpArmCtrlObj.part.BlkSize * p->ToUpArmCtrlObj.part.BlkCount, nEnd);

    return ROUND(nEnd, 1024*1024) - ROUND(BaseAddr, 1024*1024);
}

/** @brief This function initializes the I-CPU IRQ handler on the specified core 

    @param nCpuID [in] - the CPU id

    @return [MXRC] an error code */

MXRC ICSelectMasterCpu (UINT32 nCpuID)
{
    UINT32 rc = MX_OK;

    if (nCpuID >= SysGetCpuNum())
        return IC_RC_CPU_ID_ERROR;

    if (ic_ctx.bInited == 0)
        return IC_RC_NOT_INITED;

    if (ic_ctx.nMasterCpuID == nCpuID)
        return MX_OK;

    if (ic_ctx.nInfID != IC_CHAN_SVSR && ic_ctx.nInfID != IC_CHAN_SVSR_EX_ICPU)
        return IC_RC_ICPU_IS_REQUIRED;

    if (ic_ctx.CpuCtx [nCpuID] == NULL)
        return IC_RC_NOT_INITED;

    if (ic_ctx.Proc == NULL)
        return IC_RC_PROC_ERROR;

    rc = MxDisableIntEx(ic_ctx.nMasterCpuID, ic_ctx.nSrcIrqID);

    _ASSERT_RC(rc);

    if (FAILED(rc))
        return rc;

    ic_ctx.nMasterCpuID = nCpuID;

    if (HANDLE_NULL(ic_ctx.CpuCtx[nCpuID]->hHandler))
    {
        rc = MxCreateThreadEx(nCpuID, (TENTRY)ic_ctx.Proc, NULL, ic_ctx.CpuCtx[nCpuID], (TPRIORITY)0,
                             &ic_ctx.CpuCtx[nCpuID]->hHandler);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }
    }

    rc = MxSetIntHandlerEx(nCpuID, ic_ctx.nSrcIrqID, INT_TYPE_IRQ,
                                  PRI_ICPU, InterCpuIntHandle, ic_ctx.CpuCtx[nCpuID]);

    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    rc = MxEnableIntEx(nCpuID, ic_ctx.nSrcIrqID);

    _ASSERT_RC(rc);

    if (FAILED(rc))
        return rc;

    return MX_OK;
}


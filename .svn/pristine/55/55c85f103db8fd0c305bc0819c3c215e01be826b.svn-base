//-------------------------------------------------------------------------------------------
/** @file icpu2.c
 *
 * @brief This file contains declaration of I-CPU externed interface used 
 *        to send/receive API commands between x4 and x2 ARMs
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "4gmx.h"
#include "icpu2.h"
#include "sirqassign.h"
#include "icdrv.h"

ICPU_OBJ_CTX  * p_icpu2_ctx = NULL;
VUINT32  IcpuLocks [ICPU_QUEUE_NUM] = {0};

void ICpuInitQueue (ICPUQ * pQ, PVUINT32 pStor, UINT32 nElmNum)
{
    _ASSERT_PTR(pQ);
    _ASSERT_PTR(pStor);

    pQ->Ptr = pStor;
    pQ->Num = nElmNum;
}

UINT32 ICpuGetQueueSize (ICPUQ * pQ)
{
    UINT32 nSize;

    if ((pQ->Put >= pQ->Get))
    {
        nSize = (pQ->Put - pQ->Get);
    }
    else
    {
        nSize = (pQ->Num - (pQ->Get - pQ->Put));
    }

    return nSize;
}

void ICpuAddFreeBlocks(ICPUQ * pQ, UINT32 nElmNum)
{
    UINT32 i = 0;

    _ASSERT_PTR(pQ);

    for (i = 0; i < nElmNum; i++)
    {
        pQ->Ptr [pQ->Put] = i;
        pQ->Put++;
        pQ->StatPut ++;
        
        if (pQ->Put >= pQ->Num)
            pQ->Put = 0;
    }
}

UINT32 ICpuGetIndex (ICPUQ * pQ, VUINT32 * pLock)
{
    UINT32 index;
    IRQTYPE r;

    _ASSERT_PTR(pQ);

    r = ARM_INT_disable();

    if (pLock != NULL)
        MxGetLock (pLock);

    if (pQ->Get != pQ->Put)
    {
        index = pQ->Ptr [pQ->Get];

        pQ->Get++;

        if (pQ->Get >= pQ->Num)
            pQ->Get = 0;

        pQ->StatGet ++;
    }
    else
    {
        index = (UINT32)-1;
    }

    if (pLock != NULL)
        MxReleaseLock (pLock);

    ARM_INT_restore(r);

    return index;
}

void ICpuPutIndex(ICPUQ * pQ, VUINT32 * pLock, UINT32 nIndex)
{
    IRQTYPE r;

    _ASSERT_PTR(pQ);
    _ASSERT_PTR(pLock);

    r = ARM_INT_disable();

    MxGetLock (pLock);

    pQ->Ptr [pQ->Put] = nIndex;

    // Addeded here to prevent rare problems 
    // when U-ARM reads value (because ++ but indes was not putted yet)
    // actually compiler dependent problem 
    
    if ((pQ->Put + 1) >= pQ->Num)
    {
        pQ->Put = 0;
    }
    else
    {
        pQ->Put++;
    }

    pQ->StatPut ++;

    MxReleaseLock (pLock);

    ARM_INT_restore(r);
}

/** @brief This function initializes I-CPU objects

    @param pStorage [in]  - the pointer to the storage where I-CPU objects will be located

    @return [MXRC] an error code */

MXRC ICpuInit (void * pStorage)
{
    MXRC rc = MX_OK;

    UINT32 nSize;
    
    PVUINT32 pQ;  // queue 
    PUINT8  pP;   // partition

    if (pStorage == NULL)
    {
        _ASSERT_PTR(pStorage);
        return ICPU_RC_PTR_ERROR;
    }

    if (((UINT32)pStorage) & 0xFFFFF)
    {
        _ASSERT ((((UINT32)pStorage) & 0xFFFFF) == 0);
        return ICPU_RC_PTR_ALIGN_ERROR;
    }

    p_icpu2_ctx = (ICPU_OBJ_CTX  *)pStorage;

    memset (p_icpu2_ctx, 0, sizeof (ICPU_OBJ_CTX));

    p_icpu2_ctx->Version = ICPU_VERSION;
    p_icpu2_ctx->RxIrqID = SOFT_ICPU_FROM_UARM;
    p_icpu2_ctx->TxIrqID = SOFT_ICPU_TO_UARM;

    p_icpu2_ctx->TxQNum  = 3;   // FreeQ + REG-Q + HI-Q; it can be increased for example with Hi-hi-Q :)
    p_icpu2_ctx->RxQNum  = 3;   // FreeQ + REG-Q + HI-Q; it can be increased 

    nSize = ROUND(sizeof (ICPU_OBJ_CTX), 32);
    nSize += p_icpu2_ctx->TxQNum * sizeof (UINT32) * (ICPU_BLOCK_TX_NUM + 1);
    nSize += p_icpu2_ctx->RxQNum * sizeof (UINT32) * (ICPU_BLOCK_RX_NUM + 1);

    if (nSize >= 1024*1024)
    {
        rc = ICPU_RC_STORAGE_OVERWRITE;
        _ASSERT_RC(rc);
        return rc;
    }

    // Queue initialization 
    pQ = (PUINT32)((UINT32)p_icpu2_ctx + ROUND(sizeof (ICPU_OBJ_CTX), 32));

    ICpuInitQueue (&p_icpu2_ctx->Queues[ICPU_TX_FREE_QUEUE], pQ, ICPU_BLOCK_TX_NUM+1);
    pQ+= ICPU_BLOCK_TX_NUM+1;
    
    ICpuInitQueue (&p_icpu2_ctx->Queues[ICPU_TX_HI_QUEUE], pQ, ICPU_BLOCK_TX_NUM+1);
    pQ+= ICPU_BLOCK_TX_NUM+1;
    
    ICpuInitQueue (&p_icpu2_ctx->Queues[ICPU_TX_REG_QUEUE], pQ, ICPU_BLOCK_TX_NUM+1);
    pQ+= ICPU_BLOCK_TX_NUM+1;
    
    ICpuInitQueue (&p_icpu2_ctx->Queues[ICPU_RX_FREE_QUEUE], pQ, ICPU_BLOCK_RX_NUM+1);
    pQ+= ICPU_BLOCK_RX_NUM+1;
    
    ICpuInitQueue (&p_icpu2_ctx->Queues[ICPU_RX_HI_QUEUE], pQ, ICPU_BLOCK_RX_NUM+1);
    pQ+= ICPU_BLOCK_RX_NUM+1;
    
    ICpuInitQueue (&p_icpu2_ctx->Queues[ICPU_RX_REG_QUEUE], pQ, ICPU_BLOCK_RX_NUM+1);
    pQ+= ICPU_BLOCK_RX_NUM+1;

    // Partition parameters initialization 
    // The partition storage is 1MB aligned
    //                          ~~~

    pP = (PUINT8)((UINT32)p_icpu2_ctx + (1024*1024));

    p_icpu2_ctx->TxMsgStoragePtr = pP; pP += (ICPU_BLOCK_TX_NUM * ICPU_BLOCK_TX_SIZE);
    p_icpu2_ctx->RxMsgStoragePtr = pP; pP += (ICPU_BLOCK_RX_NUM * ICPU_BLOCK_RX_SIZE);

    p_icpu2_ctx->TxBlocksNum = ICPU_BLOCK_TX_NUM;
    p_icpu2_ctx->TxBlockSize = ICPU_BLOCK_TX_SIZE;

    p_icpu2_ctx->RxBlocksNum = ICPU_BLOCK_RX_NUM;
    p_icpu2_ctx->RxBlockSize = ICPU_BLOCK_RX_SIZE;

    ICpuAddFreeBlocks (&p_icpu2_ctx->Queues[ICPU_TX_FREE_QUEUE], ICPU_BLOCK_TX_NUM);
    ICpuAddFreeBlocks (&p_icpu2_ctx->Queues[ICPU_RX_FREE_QUEUE], ICPU_BLOCK_RX_NUM);

    p_icpu2_ctx->Status = 1;

    return rc;
}

LPVOID ICpuLockTxData (void)
{
    LPVOID p;
    UINT32 index;

    if (p_icpu2_ctx == NULL)
        return NULL;

    index = ICpuGetIndex (&(p_icpu2_ctx->Queues[ICPU_TX_FREE_QUEUE]), &IcpuLocks[ICPU_TX_FREE_QUEUE]);

    if (index == (UINT32)-1)
        return NULL;

    _ASSERT_PTR(p_icpu2_ctx->TxMsgStoragePtr);
    _ASSERT_PTR(index < p_icpu2_ctx->TxBlocksNum);
    
    p = (p_icpu2_ctx->TxMsgStoragePtr + (p_icpu2_ctx->TxBlockSize * index));

    return p;
}

MXRC ICpuFreeTxData (LPVOID Ptr)
{
    UINT32 index;

    if (p_icpu2_ctx == NULL)
        return ICPU_RC_INIT_ERROR;

    _ASSERT_PTR(p_icpu2_ctx->TxMsgStoragePtr);

    index = ((UINT32)Ptr - (UINT32)p_icpu2_ctx->TxMsgStoragePtr) / p_icpu2_ctx->TxBlockSize;
    
    _ASSERT_PTR(index < p_icpu2_ctx->TxBlocksNum);

    ICpuPutIndex (&(p_icpu2_ctx->Queues[ICPU_TX_FREE_QUEUE]), &IcpuLocks[ICPU_TX_FREE_QUEUE], index);

    return MX_OK;
}

MXRC ICpuSendTxData (LPVOID Ptr, UINT32 nSignal)
{
    UINT32 nIndex;

    _ASSERT_PTR(Ptr);

    if (p_icpu2_ctx == NULL)
        return ICPU_RC_INIT_ERROR;

    nIndex = ((UINT32)Ptr - (UINT32)p_icpu2_ctx->TxMsgStoragePtr) / p_icpu2_ctx->TxBlockSize;

    _ASSERT(nIndex < p_icpu2_ctx->TxBlocksNum);

    ICpuPutIndex (&(p_icpu2_ctx->Queues[ICPU_TX_REG_QUEUE]), &IcpuLocks[ICPU_TX_REG_QUEUE], nIndex);

    if (nSignal)
        return ICDrvRaiseInterCpu (0, p_icpu2_ctx->TxIrqID);

    return MX_OK;
}

MXRC ICpuSendHiTxData (LPVOID Ptr, UINT32 nSignal)
{
    UINT32 nIndex;

    _ASSERT_PTR(Ptr);

    if (p_icpu2_ctx == NULL)
        return ICPU_RC_INIT_ERROR;

    nIndex = ((UINT32)Ptr - (UINT32)p_icpu2_ctx->TxMsgStoragePtr) / p_icpu2_ctx->TxBlockSize;

    _ASSERT(nIndex < p_icpu2_ctx->TxBlocksNum);

    ICpuPutIndex (&(p_icpu2_ctx->Queues[ICPU_TX_HI_QUEUE]), &IcpuLocks[ICPU_TX_HI_QUEUE], nIndex);

    if (nSignal)
        return ICDrvRaiseInterCpu (0, p_icpu2_ctx->TxIrqID);

    return MX_OK;
}

LPVOID ICpuLockRxData (void)
{
    UINT32 index;

    if (p_icpu2_ctx == NULL)
        return NULL;

    index = ICpuGetIndex (&(p_icpu2_ctx->Queues[ICPU_RX_HI_QUEUE]), NULL);

    if (index == (UINT32)-1)
        index = ICpuGetIndex (&(p_icpu2_ctx->Queues[ICPU_RX_REG_QUEUE]), NULL);

    if (index != (UINT32)-1)
    {
        _ASSERT(index < p_icpu2_ctx->RxBlocksNum);
        return (p_icpu2_ctx->RxMsgStoragePtr + (index * p_icpu2_ctx->RxBlockSize));
    }

    return NULL;
}

MXRC ICpuFreeRxData (LPVOID Ptr)
{
    UINT32 index;

    if (p_icpu2_ctx == NULL)
        return ICPU_RC_INIT_ERROR;

    _ASSERT_PTR(p_icpu2_ctx->RxMsgStoragePtr);
    _ASSERT_PTR(Ptr);

    index = ((UINT32)Ptr - (UINT32)p_icpu2_ctx->RxMsgStoragePtr) / p_icpu2_ctx->RxBlockSize;
    
    _ASSERT_PTR(index < p_icpu2_ctx->RxBlocksNum);

    ICpuPutIndex (&(p_icpu2_ctx->Queues[ICPU_RX_FREE_QUEUE]), &IcpuLocks[ICPU_RX_FREE_QUEUE], index);

    return MX_OK;
}

/**@brief This function returns the total number of TX blocks that 
          can be allocated in TX partition(array)

   @return [UINT32] number of TX blocks */

UINT32 ICpuGetTxBlockNum (void)
{
    return ICPU_BLOCK_TX_NUM;
}

/**@brief This function returns the size of TX block in bytes 

   @return [UINT32] the size of TX block in bytes*/

UINT32 ICpuGetTxBlockSize(void)
{
    return ICPU_BLOCK_TX_SIZE;
}

/**@brief This function returns the total number of RX blocks that 
          can be allocated in RX partition(array)

   @return [UINT32] number of TX blocks */

UINT32 ICpuGetRxBlockNum (void)
{
    return ICPU_BLOCK_RX_NUM;
}

/**@brief This function returns the size of TX block in bytes 

   @return [UINT32] the size of TX block in bytes*/
   
UINT32 ICpuGetRxBlockSize(void)
{
    return ICPU_BLOCK_RX_SIZE;
}

/** @brief This function returns the number of free TX blocks that 
           can be really allocated at this moment. This function is not
           the same to ICpuGetTxBlockNum()

    @return [UINT32]  accessible number of I-CPU blocks */

UINT32 ICpuGetTxFreeBlock(void)
{
    if (p_icpu2_ctx == NULL)
        return 0;

    return ICpuGetQueueSize(&p_icpu2_ctx->Queues[ICPU_TX_FREE_QUEUE]);
}

/** @brief This function returns the number of free RX blocks that 
           can be really allocated at this moment. This function is not
           the same to ICpuGetRxBlockNum()

    @return [UINT32]  accessible number of I-CPU blocks */

UINT32 ICpuGetRxFreeBlock(void)
{
    if (p_icpu2_ctx == NULL)
        return 0;

    return ICpuGetQueueSize(&p_icpu2_ctx->Queues[ICPU_RX_FREE_QUEUE]);
}

/** @brief Diagnostics function used to allocate memory in RX partiton 

    @return [LPVOID] the pointer to RX storage block */

LPVOID ICpuAllocRxData(void)
{
    UINT32 index;

    if (p_icpu2_ctx == NULL)
        return NULL;

    index = ICpuGetIndex(&p_icpu2_ctx->Queues[ICPU_RX_FREE_QUEUE], &IcpuLocks[ICPU_RX_FREE_QUEUE]);

    if (index == (UINT32)-1)
        return NULL;

    return (p_icpu2_ctx->RxMsgStoragePtr + (p_icpu2_ctx->RxBlockSize * index));
}

/** @brief Diagnostics function used to emulate sending command from remote side 

    @param pData  [in] - the pointer to the data
    @param bIrqSet[in] - to set IRQ signal or not

    @return [MXRC] an error code */

MXRC IcpuAddToRxQueue(LPVOID pData, UINT32 bIrqSet)
{
    UINT32 index;

    if (p_icpu2_ctx == NULL)
        return NULL;

    index = ((UINT32)pData - (UINT32)p_icpu2_ctx->RxMsgStoragePtr) / p_icpu2_ctx->RxBlockSize;

    _ASSERT(index < p_icpu2_ctx->RxBlocksNum);

    ICpuPutIndex (&p_icpu2_ctx->Queues[ICPU_RX_REG_QUEUE], &IcpuLocks [ICPU_RX_REG_QUEUE], index);

    if (bIrqSet)
        return ICDrvRaiseInterCpu (0, p_icpu2_ctx->RxIrqID);

    return MX_OK;
}

/** @brief This function checks is block from TX storage or not

    @param pData [in] - the pointer to the data

    @return [BOOL] result */

BOOL ICpuIsTxData (LPVOID pData)
{
    if (p_icpu2_ctx == NULL)
        return FALSE;

    return (((UINT32)pData >= (UINT32)p_icpu2_ctx->TxMsgStoragePtr) && 
           ((UINT32)pData < ((UINT32)p_icpu2_ctx->TxMsgStoragePtr+p_icpu2_ctx->TxBlocksNum*p_icpu2_ctx->TxBlockSize)));
}

/** @brief This function checks is block from RX storage or not

    @param pData [in] - the pointer to the data

    @return [BOOL] result */

BOOL ICpuIsRxData(LPVOID pData)
{
    return (((UINT32)pData >= (UINT32)p_icpu2_ctx->RxMsgStoragePtr) && 
           ((UINT32)pData < ((UINT32)p_icpu2_ctx->RxMsgStoragePtr+p_icpu2_ctx->RxBlocksNum*p_icpu2_ctx->RxBlockSize)));
}

/** @brief This function returns the total size in bytes of I-CPU storage 
            it includes: 
               - size of context
               - size of queues 
               - size of partitions

    @return [UINT32] the total size of I-CPU in bytes */
               
UINT32 ICpuGetTotalStorageSize(void)
{
    UINT32 nSize = ROUND(sizeof (ICPU_OBJ_CTX), 32);

    nSize +=sizeof (UINT32) * ( (ICPU_BLOCK_TX_NUM + 1) + 
                                (ICPU_BLOCK_TX_NUM + 1) + 
                                (ICPU_BLOCK_TX_NUM + 1) + 
            
                                (ICPU_BLOCK_RX_NUM + 1) + 
                                (ICPU_BLOCK_RX_NUM + 1) + 
                                (ICPU_BLOCK_RX_NUM + 1));

    nSize = ROUND(nSize, 1024*1024);

    nSize += (ICPU_BLOCK_TX_SIZE * ICPU_BLOCK_TX_NUM);
    nSize += (ICPU_BLOCK_RX_SIZE * ICPU_BLOCK_RX_NUM);

    return nSize;
}

/** @brief This function returns a pointer to the TX storage 
    @param base[in] - the base address of I-CPU storage 
    @return [UINT32] the base address */

UINT32 ICpuGetTxStorageBase(UINT32 base)
{
    // base should not be NULL
    if (base == NULL)
    {
        _ASSERT_PTR(base);
        return 0;
    }

    // it should be 1MB aligned
    
    if (base & 0xFFFFF)
    {
        _ASSERT ((base & 0xFFFFF) == 0);
        return 0;
    }

    return (base + 1024*1024);
}

/** @brief This function returns a pointer to the RX storage 
    @param base[in] - the base address of I-CPU storage 
    @return [UINT32] the base address */

UINT32 ICpuGetRxStorageBase(UINT32 base)
{
    return ICpuGetTxStorageBase(base) + ICpuGetTxStorageSize();
}

/** @brief This function returns the size of RX storage in bytes
    @return [UINT32] the base address */

UINT32 ICpuGetRxStorageSize()
{
    return ICPU_BLOCK_RX_SIZE * ICPU_BLOCK_RX_NUM;
}

/** @brief This function returns the size of TX storage in bytes
    @return [UINT32] the base address */

UINT32 ICpuGetTxStorageSize()
{
    return ICPU_BLOCK_TX_SIZE * ICPU_BLOCK_TX_NUM;
}


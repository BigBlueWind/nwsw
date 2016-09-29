//-------------------------------------------------------------------------------------------
/** @file 4gmx_part.c
 *
 * @brief Partition API implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.15 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"

#ifdef MX_PART_ENABLED

#include "4gmx.h"
#include "4gmx_part.h"
#include "4gmx_kernel.h"
#include "4gmx_serv.h"
#include "4gmx_debug.h"

#ifdef MX_INTERCPU_ENABLED
#include "4gmx_intercpu.h"
#endif

#if  defined (TRACE_PART_CREATE)    || \
     defined (TRACE_PART_REMOVE)    || \
     defined (TRACE_PART_OPEN)

/** @brief  This function is called by the event API for tracing PARTITION.

    @param pCtx    [in] - the system context
    @param id      [in] - the trace event ID
    @param hHandle [in] - the object handle (or HNULL)
    @param rc      [in] - error code of operation

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_partition
*/
MXRC AddPartCtrl (PMXCONTEXT pCtx, TraceEventID id, HANDLE hHandle, MXRC rc)
{
    TracePartCtrl trace;

    trace.hPart  = hHandle;
    trace.Rc     = rc;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif

#if  defined (TRACE_PART_ALLOC) || defined (TRACE_PART_FREE)

/** @brief  This function is called by the event API for tracing PARTITION.

    @param pCtx    [in] - the system context
    @param id      [in] - the trace event ID
    @param hHandle [in] - the object handle (or HNULL)
    @param data    [in] - the pointer to the mem block of allocation/deallocation

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_partition
*/
MXRC AddPartDataTrace (PMXCONTEXT pCtx, TraceEventID id,
                       HANDLE hHandle, UINT32 data)
{
    TracePartData trace;

    trace.hPart  = hHandle;
    trace.Data   = data;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif


/** @brief The function creates a partition.

    @param pMem   [in] - the pointer to the memory (part's storage)
    @param nBlkCount [in] - the number of blocks
    @param nBlkSize  [in] - the size of memory block
    @param name   [in] - the partition name, it can be null
    @param hEventAlloc [in] - the allocation event handler, it can be HNULL
    @param hEventDealloc[in] - the deallocation event handle, it can be HNULL
    @param oHand  [out] - the handle of created object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_partition
*/
MXRC MxCreatePart(LPVOID pMem, UINT32 nBlkCount, UINT32 nBlkSize,
                  const char *name, HANDLE hEventAlloc, HANDLE hEventDealloc,
                  HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    PPART pPart;
    IRQTYPE irq;
    HANDLE h = HNULL;
    PUINT32 p;
    DEF_RADDR;

    _ASSERT(pMem != NULL);
    _ASSERT(nBlkSize >= 4);
    _ASSERT(oHand != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pPart = MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_PARTITION]);

    if (pPart == NULL)
    {
#ifdef TRACE_PART_CREATE
        AddPartCtrl (pCtx, TEID_CREATE_PART, HNULL, MX_PART_ALLOC_ERROR);
#endif

        MxiRestInt(irq);
        return MX_PART_ALLOC_ERROR;
    }

    Mximemset (pPart, 0, sizeof (*pPart));

    pPart->StorageOrOpenObj = pMem;
    pPart->FreeBlock = pMem;
    pPart->BlkCount = nBlkCount;
    pPart->BlkSize = nBlkSize;
    pPart->AllocNum = 0;
    pPart->CtrlFlags = 0;
    pPart->Sync = 0;

    pPart->hEventAlloc = hEventAlloc;
    pPart->hEventDeAlloc = hEventDealloc;

    p = (PUINT32) pPart->StorageOrOpenObj;

    while (--nBlkCount)
    {
        *p = (UINT32)(((UINT8 *) p) + nBlkSize);
        p = (UINT32 *) * p;
    }

    *p = (UINT32) NULL;

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_PARTITION,
                ((((UINT32) pPart) -
                  ((UINT32) pCtx->partitions)) / sizeof(PART)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_PARTITION], pPart);

#ifdef TRACE_PART_CREATE
            AddPartCtrl (pCtx, TEID_CREATE_PART,
                         HNULL, MX_PART_ASSIGN_NAME_ERROR);
#endif
            MxiRestInt(irq);

            return MX_PART_ASSIGN_NAME_ERROR;
        }

        MARK_PART_NAMED(pPart);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_PART_UNNAMED(pPart);
    }

    MARK_PART_DESYNC(pPart);
    MARK_PART_ALLOC(pPart);

#ifdef TRACE_PART_CREATE
    AddPartCtrl (pCtx, TEID_CREATE_PART, h, MX_OK);
#endif

    *oHand = h;

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function initializes the shared partiton for simple inter-cpu communication.

    @param pPartDescr [in] - the pointer to the inter-cpu partition descriptor
    @param pMem   [in] - the pointer to the partition buffer
    @param nBlkCount [in] - the total number of partition blocks
    @param nBlkSize  [in] - the size of block in bytes
    @param name   [in] - the name of opened object
    @param hEventAlloc [in] - the allocation event handler, it can be HNULL (used in case of local system allocation only!)
    @param hEventDealloc[in] - the deallocation event handle, it can be HNULL (used in case of local system allocation only!)
    @param oHand  [out] - the handle of opened partition

    @return [MXRC] error code

    \ingroup group_lte_4gmx_partition
*/
MXRC MxCreateSharedPart(ICPART * pPartDescr, LPVOID pMem, UINT32 nBlkCount,
                     UINT32 nBlkSize, const char *name, HANDLE hEventAlloc,
                     HANDLE hEventDealloc, HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    PPART pPart;
    IRQTYPE irq;
    HANDLE h = HNULL;
    PUINT32 p;
    DEF_RADDR;

    _ASSERT(pPartDescr != NULL);
    _ASSERT(oHand != NULL);
    _ASSERT(pMem != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pPart = MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_PARTITION]);

    if (pPart == NULL)
    {
#ifdef TRACE_PART_CREATE
        AddPartCtrl (pCtx, TEID_CREATE_PART, HNULL, MX_PART_ALLOC_ERROR);
#endif

        MxiRestInt(irq);
        return MX_PART_ALLOC_ERROR;
    }

    Mximemset (pPart, 0, sizeof (*pPart));

    pPart->StorageOrOpenObj = pPartDescr;

    /* The local fields are not used for opened(remote) partition */

    pPart->FreeBlock = NULL;
    pPart->BlkCount = 0;
    pPart->BlkSize = 0;
    pPart->AllocNum = 0;
    pPart->CtrlFlags = 0;
    pPart->Sync = (MXSYNC)&(pPartDescr->Sync);
    pPart->hEventAlloc = hEventAlloc;
    pPart->hEventDeAlloc = hEventDealloc;

    pPartDescr->Storage = pMem;
    pPartDescr->FreeBlock = pMem;
    pPartDescr->BlkCount = nBlkCount;
    pPartDescr->BlkSize = nBlkSize;
    pPartDescr->Sync = 0;
    pPartDescr->AllocNum = 0;

    p = (PUINT32) pMem;

    while (--nBlkCount)
    {
        *p = (UINT32)(((UINT8 *) p) + nBlkSize);
        p = (UINT32 *) * p;
    }

    *p = (UINT32) NULL;

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_PARTITION,
                ((((UINT32) pPart) -
                  ((UINT32) pCtx->partitions)) / sizeof(PART)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_PARTITION], pPart);

#ifdef TRACE_PART_CREATE
            AddPartCtrl (pCtx, TEID_CREATE_PART,
                         HNULL, MX_PART_ASSIGN_NAME_ERROR);
#endif

            MxiRestInt(irq);

            return MX_PART_ASSIGN_NAME_ERROR;
        }

        MARK_PART_NAMED(pPart);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_PART_UNNAMED(pPart);
    }

    MARK_PART_OPEN(pPart);
    MARK_PART_SYNC(pPart);
    MARK_PART_ALLOC(pPart);

#ifdef TRACE_PART_CREATE
    AddPartCtrl (pCtx, TEID_CREATE_PART, h, MX_OK);
#endif

    *oHand = h;

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function "opens"/"set connections with" created partition and returns its handle,
   the opened object can be used like a locally created.

    @param pPartDescr [in] - the pointer to the inter-cpu partition descriptor
    @param name   [in] - the name of opened object
    @param oHand  [out] - the handle of opened partition

    @return [MXRC] error code

    \ingroup group_lte_4gmx_partition
*/
MXRC MxOpenPart(ICPART * pPartDescr, const char *name, HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    PPART pPart;
    IRQTYPE irq;
    HANDLE h = HNULL;
    PPART pRemotePart;
    DEF_RADDR;

    _ASSERT(pPartDescr != NULL);
    _ASSERT(oHand != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pRemotePart = (PPART) pPartDescr;

    pPart = MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_PARTITION]);

    if (pPart == NULL)
    {
#ifdef TRACE_PART_OPEN
        AddPartCtrl (pCtx, TEID_OPEN_PART, HNULL, MX_PART_ALLOC_ERROR);
#endif
        MxiRestInt(irq);
        return MX_PART_ALLOC_ERROR;
    }

    Mximemset (pPart, 0, sizeof (*pPart));

    pPart->StorageOrOpenObj = pPartDescr;

    /* The local fields are not used for opened(remote) partition */

    pPart->FreeBlock = NULL;
    pPart->BlkCount = 0;
    pPart->BlkSize = 0;
    pPart->AllocNum = 0;
    pPart->CtrlFlags = 0;
    pPart->Sync = (MXSYNC) & (pRemotePart->Sync);
    pPart->hEventAlloc = HNULL;
    pPart->hEventDeAlloc = HNULL;

    MxiSyncCtrlOn(OBJ_TYPE_PARTITION, pPart);

    _ASSERT(pRemotePart->StorageOrOpenObj != NULL);

    MxiSyncCtrlOff(OBJ_TYPE_PARTITION, pPart);

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_PARTITION,
                ((((UINT32) pPart) -
                  ((UINT32) pCtx->partitions)) / sizeof(PART)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_PARTITION], pPart);

#ifdef TRACE_PART_OPEN
            AddPartCtrl (pCtx, TEID_OPEN_PART, HNULL, MX_PART_ASSIGN_NAME_ERROR);
#endif
            MxiRestInt(irq);

            return MX_PART_ASSIGN_NAME_ERROR;
        }

        MARK_PART_NAMED(pPart);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_PART_UNNAMED(pPart);
    }

    MARK_PART_OPEN(pPart);
    MARK_PART_SYNC(pPart);
    MARK_PART_ALLOC(pPart);

#ifdef TRACE_PART_OPEN
    AddPartCtrl (pCtx, TEID_OPEN_PART, h, MX_OK);
#endif

    *oHand = h;

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function "opens"/"set connections with" created partition
    and returns its handle, the opened object can be used like a locally created.

    @note: this function is designed to be used with HW implemented semaphores
           and due to this implementation is depended on the platform

    @param pPartDescr  [in] - the pointer to the inter-cpu partition descriptor
    @param name        [in] - the name of opened object
    @param pHwSema     [in] - the pointer to the HW sema cell (it cannot be NULL)
    @param bCacheable  [in] - if TRUE, the opened descriptor is located in cacheable memory
    @param oHand       [out] - the handle of opened partition

    @return [MXRC] error code

    \ingroup group_lte_4gmx_partition
*/
MXRC MxOpenPartEx(ICPART * pPartDescr, const char *name, PUINT32 pHwSema, BOOL bCacheable, HANDLE * oHand)

{
    PMXCONTEXT pCtx;
    PPART pPart;
    IRQTYPE irq;
    HANDLE h = HNULL;
    DEF_RADDR;

    _ASSERT(pPartDescr != NULL);
    _ASSERT(oHand != NULL);
    _ASSERT_PTR(pHwSema);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

     pPart = MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_PARTITION]);

    if (pPart == NULL)
    {
#ifdef TRACE_PART_OPEN
        AddPartCtrl (pCtx, TEID_OPEN_PART, HNULL, MX_PART_ALLOC_ERROR);
#endif
        MxiRestInt(irq);
        return MX_PART_ALLOC_ERROR;
    }

    Mximemset (pPart, 0, sizeof (*pPart));

    pPart->StorageOrOpenObj = pPartDescr;

    /* The local fields are not used for opened(remote) partition */

    pPart->FreeBlock = NULL;
    pPart->BlkCount = 0;
    pPart->BlkSize = 0;
    pPart->AllocNum = 0;
    pPart->CtrlFlags = 0;
    pPart->Sync = (MXSYNC) pHwSema;
    pPart->hEventAlloc = HNULL;
    pPart->hEventDeAlloc = HNULL;

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_PARTITION,
                ((((UINT32) pPart) -
                  ((UINT32) pCtx->partitions)) / sizeof(PART)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_PARTITION], pPart);

#ifdef TRACE_PART_OPEN
            AddPartCtrl (pCtx, TEID_OPEN_PART, HNULL, MX_PART_ASSIGN_NAME_ERROR);
#endif
            MxiRestInt(irq);

            return MX_PART_ASSIGN_NAME_ERROR;
        }

        MARK_PART_NAMED(pPart);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_PART_UNNAMED(pPart);
    }

    MARK_PART_OPEN(pPart);
    MARK_PART_SYNC(pPart);
    MARK_PART_ALLOC(pPart);
    MARK_PART_HW_SEMA(pPart);

    if (bCacheable)
    {
        MARK_PART_OPEN_CACHE(pPart);
    }

#ifdef TRACE_PART_OPEN
    AddPartCtrl (pCtx, TEID_OPEN_PART, h, MX_OK);
#endif

    *oHand = h;

    MxiRestInt(irq);

    return MX_OK;
}


/** @brief The function removes created partition.

    @param hPart [in] - the partition handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_partition
*/
MXRC MxRemovePart(HANDLE hPart)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    PPART pPart;

    _ASSERT(HANDLE_NOT_NULL(hPart));
    _ASSERT(HANDLE_GET_TYPE(hPart) == OBJ_TYPE_PARTITION);
    _ASSERT(HANDLE_GET_CPU(hPart) == MxGetCpuID());

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pPart = (PPART) MxiGetPartDescr(pCtx, hPart);

    _ASSERT(pPart != NULL);
    _ASSERT(IS_PART_ALLOC(pPart));

    if (IS_PART_SYNC(pPart) || IS_PART_OPENED(pPart))
    {
        MxiSyncCtrlOn(OBJ_TYPE_PARTITION, pPart);
    }

#ifdef MX_NAMING_ENABLED

    if (IS_PART_NAMED(pPart))
    {
        MxiFreeName(pCtx, hPart);

        MARK_PART_UNNAMED(pPart);
    }

#endif /*MX_NAMING_ENABLED */

    MARK_PART_DEALLOC(pPart);

    MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_PARTITION], pPart);

    if (IS_PART_SYNC(pPart) || IS_PART_OPENED(pPart))
    {
        MxiSyncCtrlOff(OBJ_TYPE_PARTITION, pPart);
    }

#ifdef TRACE_PART_REMOVE
    AddPartCtrl (pCtx, TEID_REMOVE_PART, hPart, MX_OK);
#endif

    MxiRestInt(irq);

    return MX_OK;
}

/** @breif The function allocates block of memory (of specified size) and returns
   pointer to allocated memory or NULL if no any accessible blocks

 @param hPart [in] - the partition handle

 @return [LPVOID] a pointer to allocated partition block or NULL*/

#ifdef MX_TRACER_ENABLED

LPVOID MxAllocBlock(HANDLE hPart)
{
    UINT32 trace_lr = MX_GET_RADDR;
    return MxAllocBlockLR (hPart, trace_lr);
}
LPVOID MxAllocBlockLR(HANDLE hPart, UINT32 trace_lr)
    
#else

LPVOID MxAllocBlock(HANDLE hPart)

#endif

{
    PMXCONTEXT pCtx;
    PPART pPart, pRemPart;
    IRQTYPE irq;
    PUINT32 pBlock;

    _ASSERT(HANDLE_NOT_NULL(hPart));
    _ASSERT(HANDLE_GET_TYPE(hPart) == OBJ_TYPE_PARTITION);

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

#ifndef MX_INTERCPU_ENABLED

    _ASSERT(HANDLE_GET_CPU(hPart) == MxGetCpuID());

    MxiDisInt(&irq);

    pPart = MxiGetPartDescr(pCtx, hPart);

#else

    MxiDisInt(&irq);

    if (HANDLE_GET_CPU(hPart) != MxGetCpuID())
    {
        pPart = (PPART)MxiFindSharedDescr (pCtx, hPart);

#ifndef MX_ARM_SMP_ENABLED
        MxiCacheInvalid(pPart, sizeof (PART));
#endif
    }
    else
    {
        pPart = MxiGetPartDescr(pCtx, hPart);
    }

#endif

    _ASSERT(pPart != NULL);
    _ASSERT(IS_PART_ALLOC(pPart));

    if (IS_PART_OPENED(pPart))
    {
        _ASSERT(pPart->StorageOrOpenObj != NULL);

        pRemPart = (PPART) pPart->StorageOrOpenObj;
    }
    else
    {
        pRemPart = pPart;
    }

    if (IS_PART_SYNC(pPart) || IS_PART_OPENED(pPart))
    {
        MxiSyncCtrlOn(OBJ_TYPE_PARTITION, pPart);
    }

#ifndef MX_ARM_SMP_ENABLED
    if (IS_PART_OPEN_CACHE(pPart))
    {
        MxiCacheInvalid(pRemPart, sizeof (*pPart));
    }
#endif

    pBlock = (PUINT32) pRemPart->FreeBlock;

    if (IS_PART_OPEN_CACHE(pPart) && (pBlock != NULL))
    {
        MxiCacheInvalid(pBlock, 4);
    }

    _ASSERT ((UINT32)pBlock >= (UINT32)pRemPart->StorageOrOpenObj);
    _ASSERT ((UINT32)pBlock < ((UINT32)pRemPart->StorageOrOpenObj + pRemPart->BlkCount * pRemPart->BlkSize));

    if (pBlock)
    {
        pRemPart->FreeBlock = (UINT32 *) * pBlock;
        pRemPart->AllocNum++;
    }

    _ASSERT ((UINT32)pRemPart->FreeBlock >= (UINT32)pRemPart->StorageOrOpenObj || (UINT32)pRemPart->FreeBlock == NULL);
    _ASSERT ((UINT32)pRemPart->FreeBlock < ((UINT32)pRemPart->StorageOrOpenObj + pRemPart->BlkCount * pRemPart->BlkSize));

    #ifdef MX_TRACER_ENABLED
    if (pPart->TrAlloc != NULL && pBlock != NULL)
    {
        pPart->TrLR         = trace_lr;
        pPart->TrFreeBlk    = pRemPart->BlkCount - (pRemPart->AllocNum);
        pPart->TrAlloc (pPart->TrPtr, pBlock);
    }
    #endif

#ifdef TRACE_PART_ALLOC
    AddPartDataTrace (pCtx, TEID_ALLOC_PART, hPart, (UINT32)pBlock);
#endif

#ifndef MX_ARM_SMP_ENABLED
    if (IS_PART_OPEN_CACHE(pPart))
    {
        MxiCacheClean(pRemPart, sizeof (*pPart));
    }
#endif

    if (IS_PART_SYNC(pPart) || IS_PART_OPENED(pPart))
    {
        MxiSyncCtrlOff(OBJ_TYPE_PARTITION, pPart);
    }

#ifdef MX_EVENT_ENABLED
    if (HANDLE_NOT_NULL(pPart->hEventAlloc))
    {
        MxSetEvent(pPart->hEventAlloc);
    }
#endif

    MxiRestInt(irq);

    return pBlock;
}


/** @bried The function frees allocated memory block
 @param hPart [in] - the partition handle
 @param pBlock[in] - the pointer to block of memory
 @return [MXRC] error code */

#ifdef MX_TRACER_ENABLED

MXRC MxFreeBlock(HANDLE hPart, LPVOID pBlock)
{
    UINT32 trace_lr = MX_GET_RADDR;

    return MxFreeBlockLR(hPart, pBlock, trace_lr);
}

MXRC MxFreeBlockLR(HANDLE hPart, LPVOID pBlock, UINT32 trace_lr)

#else

MXRC MxFreeBlock(HANDLE hPart, LPVOID pBlock)

#endif

{
    PMXCONTEXT pCtx;
    PPART pPart, pRemPart;
    IRQTYPE irq;

    _ASSERT(HANDLE_NOT_NULL(hPart));
    _ASSERT(HANDLE_GET_TYPE(hPart) == OBJ_TYPE_PARTITION);
    _ASSERT(pBlock != NULL);

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    MxiDisInt(&irq);

    if (HANDLE_GET_CPU(hPart) != MxGetCpuID())
    {
        pPart = (PPART)MxiFindSharedDescr (pCtx, hPart);

#ifndef MX_ARM_SMP_ENABLED
        MxiCacheInvalid(pPart, sizeof (PART));
#endif
    }
    else
    {
        pPart = MxiGetPartDescr(pCtx, hPart);
    }

#else
    _ASSERT(HANDLE_GET_CPU(hPart) == MxGetCpuID());

    MxiDisInt(&irq);

    pPart = MxiGetPartDescr(pCtx, hPart);

#endif

    _ASSERT(pPart != NULL);
    _ASSERT(IS_PART_ALLOC(pPart));

    if (IS_PART_OPENED(pPart))
    {
        _ASSERT(pPart->StorageOrOpenObj != NULL);

        pRemPart = (PPART) pPart->StorageOrOpenObj;
    }
    else
    {
        pRemPart = pPart;
    }

    if (IS_PART_SYNC(pPart) || IS_PART_OPENED(pPart))
    {
        MxiSyncCtrlOn(OBJ_TYPE_PARTITION, pPart);
    }

#ifndef MX_ARM_SMP_ENABLED
    if (IS_PART_OPEN_CACHE(pPart))
    {
        MxiCacheInvalid(pRemPart, sizeof (*pPart));
    }
#endif

#ifdef TRACE_PART_FREE
    AddPartDataTrace (pCtx, TEID_FREE_PART, hPart, (UINT32)pBlock);
#endif

    _ASSERT ((UINT32)pBlock >= (UINT32)pRemPart->StorageOrOpenObj);
    _ASSERT ((UINT32)pBlock < ((UINT32)pRemPart->StorageOrOpenObj + pRemPart->BlkCount * pRemPart->BlkSize));

    _ASSERT ((UINT32)pRemPart->FreeBlock >= (UINT32)pRemPart->StorageOrOpenObj || pRemPart->FreeBlock == NULL);
    _ASSERT ((UINT32)pRemPart->FreeBlock < ((UINT32)pRemPart->StorageOrOpenObj + pRemPart->BlkCount * pRemPart->BlkSize));

    *(UINT32 *) pBlock = (UINT32) pRemPart->FreeBlock;
    pRemPart->FreeBlock = (UINT32 *) pBlock;

    pRemPart->AllocNum--;

    #ifdef MX_TRACER_ENABLED
    if (pPart->TrFree != NULL)
    {
        pPart->TrFreeBlk = (pRemPart->BlkCount - pRemPart->AllocNum);
        pPart->TrLR = trace_lr;
        pPart->TrFree (pPart->TrPtr, pBlock);
    }
    #endif
    
    if (IS_PART_OPEN_CACHE(pPart))
    {
#ifndef MX_ARM_SMP_ENABLED
        MxiCacheClean(pRemPart, sizeof (*pPart));
#endif
        MxiCacheClean(pBlock, 4);
    }

    if (IS_PART_SYNC(pPart) || IS_PART_OPENED(pPart))
    {
        MxiSyncCtrlOff(OBJ_TYPE_PARTITION, pPart);
    }

#ifdef MX_EVENT_ENABLED
    if (HANDLE_NOT_NULL(pPart->hEventDeAlloc))
    {
        MxSetEvent(pPart->hEventDeAlloc);
    }
#endif

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function returns number of non-used partition's blocks,that can be allocated by app

    @param hPart [in] - the partition handle

    @return [UINT32] a number of non-used partition's blocks

    \ingroup group_lte_4gmx_partition
*/
UINT32 MxGetFreePartBlocks(HANDLE hPart)
{
    PMXCONTEXT pCtx;
    PPART pPart, pRemPart;
    IRQTYPE irq;
    UINT32 nNum;

    _ASSERT(HANDLE_NOT_NULL(hPart));
    _ASSERT(HANDLE_GET_TYPE(hPart) == OBJ_TYPE_PARTITION);

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    MxiDisInt(&irq);

    if (HANDLE_GET_CPU(hPart) != MxGetCpuID())
    {
        pPart = (PPART)MxiFindSharedDescr (pCtx, hPart);

#ifndef MX_ARM_SMP_ENABLED
        MxiCacheInvalid(pPart, sizeof (PART));
#endif
    }
    else
    {
        pPart = MxiGetPartDescr(pCtx, hPart);
    }

#else
    _ASSERT(HANDLE_GET_CPU(hPart) == MxGetCpuID());

    MxiDisInt(&irq);

    pPart = MxiGetPartDescr(pCtx, hPart);

#endif

    _ASSERT(pPart != NULL);
    _ASSERT(IS_PART_ALLOC(pPart));

    if (IS_PART_OPENED(pPart))
    {
        _ASSERT(pPart->StorageOrOpenObj != NULL);
        pRemPart = (PPART) pPart->StorageOrOpenObj;
    }
    else
    {
        pRemPart = pPart;
    }

    if (IS_PART_SYNC(pPart) || IS_PART_OPENED(pPart))
    {
        MxiSyncCtrlOn(OBJ_TYPE_PARTITION, pPart);
    }

#ifndef MX_ARM_SMP_ENABLED
    if (IS_PART_OPEN_CACHE(pPart))
    {
        MxiCacheInvalid(pRemPart, sizeof (*pPart));
    }
#endif

    nNum = (pRemPart->BlkCount - pRemPart->AllocNum);

    if (IS_PART_SYNC(pPart) || IS_PART_OPENED(pPart))
    {
        MxiSyncCtrlOff(OBJ_TYPE_PARTITION, pPart);
    }

    MxiRestInt(irq);

    return nNum;
}

/** @brief This function converts block index to the block pointer

    @param hPart [in] - the partition descriptor
    @param nIndex[in] - the block index

    @return [LPVOID] the pointer to the block or NULL

    \ingroup group_lte_4gmx_partition
*/
LPVOID MxPartIndexToPointer(HANDLE hPart, UINT32 nIndex)
{
    PMXCONTEXT pCtx;
    PPART pPart;
    ICPART * pRemPart;
    IRQTYPE irq;
    LPVOID pData;

    _ASSERT(HANDLE_NOT_NULL(hPart));
    _ASSERT(HANDLE_GET_TYPE(hPart) == OBJ_TYPE_PARTITION);

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    MxiDisInt(&irq);

    if (HANDLE_GET_CPU(hPart) != MxGetCpuID())
    {
        pPart = (PPART)MxiFindSharedDescr (pCtx, hPart);

#ifndef MX_ARM_SMP_ENABLED
        MxiCacheInvalid(pPart, sizeof (PART));
#endif
    }
    else
    {
        pPart = MxiGetPartDescr(pCtx, hPart);
    }
#else
    _ASSERT(HANDLE_GET_CPU(hPart) == MxGetCpuID());

    MxiDisInt(&irq);

    pPart = MxiGetPartDescr(pCtx, hPart);
#endif

    _ASSERT(pPart != NULL);
    _ASSERT(IS_PART_ALLOC(pPart));

    if (IS_PART_OPENED(pPart))
    {
        _ASSERT(pPart->StorageOrOpenObj != NULL);

        pRemPart = (ICPART*)pPart->StorageOrOpenObj;
    }
    else
    {
        pRemPart = (ICPART*)pPart;
    }

    _ASSERT(nIndex < pRemPart->BlkCount);

    pData = (LPVOID)((PUINT8)pRemPart->Storage + nIndex * pRemPart->BlkSize);

    MxiRestInt(irq);

    return pData;
}

/** @brief This function converts block pointer to the block index

    @param hPart [in] - the partition descriptor
    @param pData [in] - the block pointer

    @return [LPVOID] the index of the block or 0xFFFFFFFF

    \ingroup group_lte_4gmx_partition
*/
UINT32 MxPartPointerToIndex(HANDLE hPart, LPVOID pData)
{
    PMXCONTEXT pCtx;
    PPART pPart;
    ICPART * pRemPart;
    IRQTYPE irq;
    UINT32 nIndex;

    _ASSERT(HANDLE_NOT_NULL(hPart));
    _ASSERT(HANDLE_GET_TYPE(hPart) == OBJ_TYPE_PARTITION);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    MxiDisInt(&irq);

    if (HANDLE_GET_CPU(hPart) != MxGetCpuID())
    {
        pPart = (PPART)MxiFindSharedDescr (pCtx, hPart);

#ifndef MX_ARM_SMP_ENABLED
        MxiCacheInvalid(pPart, sizeof (PART));
#endif
    }
    else
    {
        pPart = MxiGetPartDescr(pCtx, hPart);
    }

#else
    _ASSERT(HANDLE_GET_CPU(hPart) == MxGetCpuID());

    MxiDisInt(&irq);

    pPart = MxiGetPartDescr(pCtx, hPart);

#endif

    _ASSERT(pPart != NULL);
    _ASSERT(IS_PART_ALLOC(pPart));

    if (IS_PART_OPENED(pPart))
    {
        _ASSERT(pPart->StorageOrOpenObj != NULL);

        pRemPart = (ICPART*)pPart->StorageOrOpenObj;
    }
    else
    {
        pRemPart = (ICPART*)pPart;
    }

    _ASSERT(pRemPart->BlkSize != 0);

    nIndex = (((UINT32)pData - (UINT32)pRemPart->Storage) / pRemPart->BlkSize);

    _ASSERT(nIndex < pRemPart->BlkCount);

    MxiRestInt(irq);

    return nIndex;
}

/** @brief The function returns size of partition block in bytes

    @param hPart [in] - the partition handler

    @return [UINT32] the size of block in bytes

    \ingroup group_lte_4gmx_partition
*/
UINT32 MxGetPartBlockSize (HANDLE hPart)
{
    PMXCONTEXT pCtx;
    PPART pPart;
    IRQTYPE irq;
    UINT32 nBlockSize = 0;

    _ASSERT(HANDLE_NOT_NULL(hPart));
    _ASSERT(HANDLE_GET_TYPE(hPart) == OBJ_TYPE_PARTITION);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    MxiDisInt(&irq);

    if (HANDLE_GET_CPU(hPart) != MxGetCpuID())
    {
        pPart = (PPART)MxiFindSharedDescr (pCtx, hPart);

#ifndef MX_ARM_SMP_ENABLED
        MxiCacheInvalid(pPart, sizeof (PART));
#endif
    }
    else
    {
        pPart = MxiGetPartDescr(pCtx, hPart);
    }

#else
    _ASSERT(HANDLE_GET_CPU(hPart) == MxGetCpuID());

    MxiDisInt(&irq);

    pPart = MxiGetPartDescr(pCtx, hPart);

#endif

    _ASSERT(pPart != NULL);
    _ASSERT(IS_PART_ALLOC(pPart));

    if (IS_PART_OPENED(pPart))
    {
        _ASSERT(pPart->StorageOrOpenObj != NULL);

        nBlockSize = ((ICPART*)pPart->StorageOrOpenObj)->BlkSize;
    }
    else
    {
        nBlockSize = pPart->BlkSize;
    }

    MxiRestInt(irq);

    return nBlockSize;
}

/** @brief The function returns total number of partition blocks

    @param hPart [in] - the partition handler

    @return [UINT32] the number of blocks

    \ingroup group_lte_4gmx_partition
*/
UINT32 MxGetPartBlockCount (HANDLE hPart)
{
    PMXCONTEXT pCtx;
    PPART pPart;
    IRQTYPE irq;
    UINT32 nBlockCount;

    _ASSERT(HANDLE_NOT_NULL(hPart));
    _ASSERT(HANDLE_GET_TYPE(hPart) == OBJ_TYPE_PARTITION);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    MxiDisInt(&irq);

    if (HANDLE_GET_CPU(hPart) != MxGetCpuID())
    {
        pPart = (PPART)MxiFindSharedDescr (pCtx, hPart);

#ifndef MX_ARM_SMP_ENABLED
        MxiCacheInvalid(pPart, sizeof (PART));
#endif
    }
    else
    {
        pPart = MxiGetPartDescr(pCtx, hPart);
    }

#else
    _ASSERT(HANDLE_GET_CPU(hPart) == MxGetCpuID());

    MxiDisInt(&irq);

    pPart = MxiGetPartDescr(pCtx, hPart);

#endif

    _ASSERT(pPart != NULL);
    _ASSERT(IS_PART_ALLOC(pPart));

    if (IS_PART_OPENED(pPart))
    {
        _ASSERT(pPart->StorageOrOpenObj != NULL);

        nBlockCount = ((ICPART*)pPart->StorageOrOpenObj)->BlkCount;
    }
    else
    {
        nBlockCount = pPart->BlkCount;
    }

    MxiRestInt(irq);

    return nBlockCount;
}

/** @brief This function check if the specified block already allocated or not,
           the pointer should points to the partition storage

    @param hPart        [in] - the partition handle 
    @param pTestBlock   [in] - the pointer to the partition block

    @return [BOOL] 1 / 0*/

UINT32 MxIsBlockAllocated (HANDLE hPart, LPVOID pTestBlock)
{
    PMXCONTEXT pCtx;
    PPART pPart;
    IRQTYPE irq;
    ICPART * pPartObj;
    LPVOID pBlock;

    _ASSERT(HANDLE_NOT_NULL(hPart));
    _ASSERT(HANDLE_GET_TYPE(hPart) == OBJ_TYPE_PARTITION);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    MxiDisInt(&irq);

    if (HANDLE_GET_CPU(hPart) != MxGetCpuID())
    {
        pPart = (PPART)MxiFindSharedDescr (pCtx, hPart);
    }
    else
    {
        pPart = MxiGetPartDescr(pCtx, hPart);
    }

#else
    _ASSERT(HANDLE_GET_CPU(hPart) == MxGetCpuID());

    MxiDisInt(&irq);

    pPart = MxiGetPartDescr(pCtx, hPart);

#endif

    _ASSERT(pPart != NULL);
    _ASSERT(IS_PART_ALLOC(pPart));

    if (IS_PART_OPENED(pPart))
    {
        _ASSERT(pPart->StorageOrOpenObj != NULL);
        pPartObj = (ICPART*)pPart->StorageOrOpenObj;
    }
    else
    {
        pPartObj = (ICPART*)pPart;
    }

    if (IS_PART_SYNC(pPart) || IS_PART_OPENED(pPart))
    {
        MxiSyncCtrlOn(OBJ_TYPE_PARTITION, pPart);
    }

    pBlock = pPartObj->FreeBlock;

    while (pBlock != NULL)
    {
         if ((UINT32)pBlock ==  (UINT32)pTestBlock)
         {
            if (IS_PART_SYNC(pPart) || IS_PART_OPENED(pPart))
            {
                MxiSyncCtrlOff(OBJ_TYPE_PARTITION, pPart);
            }

            MxiRestInt(irq);

            return FALSE;
         }

         pBlock = (LPVOID)(((PUINT32)pBlock)[0]);
    }


    if (IS_PART_SYNC(pPart) || IS_PART_OPENED(pPart))
    {
        MxiSyncCtrlOff(OBJ_TYPE_PARTITION, pPart);
    }

    MxiRestInt(irq);

    return TRUE;
}

/** @brief This function checks partition range of memory
           and returns TRUE if specified data belongs to this range of memory

    @param hPart [in] - the partition handle
    @param pData [in] - the pointer to the data

    @return [UINT32] flag 1 or 0
    \ingroup group_lte_4gmx_partition
    */

UINT32 MxIsPartMemory (HANDLE hPart, LPVOID pData)
{
    PMXCONTEXT pCtx;
    PPART pPart;
    IRQTYPE irq;
    UINT32 nFlag;
    ICPART * pPartObj;

    _ASSERT(HANDLE_NOT_NULL(hPart));
    _ASSERT(HANDLE_GET_TYPE(hPart) == OBJ_TYPE_PARTITION);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    MxiDisInt(&irq);

    if (HANDLE_GET_CPU(hPart) != MxGetCpuID())
    {
        pPart = (PPART)MxiFindSharedDescr (pCtx, hPart);
    }
    else
    {
        pPart = MxiGetPartDescr(pCtx, hPart);
    }

#else
    _ASSERT(HANDLE_GET_CPU(hPart) == MxGetCpuID());

    MxiDisInt(&irq);

    pPart = MxiGetPartDescr(pCtx, hPart);

#endif

    _ASSERT(pPart != NULL);
    _ASSERT(IS_PART_ALLOC(pPart));

    if (IS_PART_OPENED(pPart))
    {
        _ASSERT(pPart->StorageOrOpenObj != NULL);
        pPartObj = (ICPART*)pPart->StorageOrOpenObj;
    }
    else
    {
        pPartObj = (ICPART*)pPart;
    }

    nFlag = ((UINT32)pData >= (UINT32)pPartObj->Storage) &&
            ((UINT32)pData <  ((UINT32)pPartObj->Storage + pPartObj->BlkSize*pPartObj->BlkCount));

    MxiRestInt(irq);

    return nFlag;
}

#ifdef MX_INTERCPU_ENABLED

/** @brief This function shares a partition

    @param hPart  [in] - the partition handler
    @param pLock  [in] - the address of the sync object (in non cached memory)

    @return [MXRC] error code

    \ingroup group_lte_4gmx_partition
*/
MXRC MxSharePart (HANDLE INP hPart, PVUINT32 pLock)
{
    PMXCONTEXT pCtx;
    PART * pPartDescr;
	MXRC rc = MX_OK;

    _ASSERT (HANDLE_NOT_NULL(hPart));

    _ASSERT_PTR(pLock);

    _ASSERT (HANDLE_GET_TYPE(hPart) == OBJ_TYPE_PARTITION);
    _ASSERT (HANDLE_GET_CPU(hPart) == MxGetCpuID ());

    pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);

    pPartDescr = MxiGetPartDescr (pCtx, hPart);

    _ASSERT_PTR(pPartDescr);

	// The opened partition cannot be shared (it's a senseless operation)
	// it can be directly opened by calling MxOpenPart

	if (IS_PART_OPENED(pPartDescr))
	{
		rc = MX_PART_CANNOT_BE_SHARED;

		_ASSERT_RC (rc);

		return rc;
	}

	// The shared partition should use synchronization

    pPartDescr->Sync = pLock;

	MARK_PART_SYNC(pPartDescr);

#ifndef MX_ARM_SMP_ENABLED
    MxiCacheClean(pPartDescr, sizeof (PART));
#endif

    return rc;
}

/** @brief This function unshares a partiton and this partition cannot be accessed
           from the other 4GMX systems

    @param hPart       [in]  - the partition handler

    @return [MXRC] error code

    \ingroup group_lte_4gmx_partition
*/
MXRC MxUnsharePart (HANDLE INP hPart)
{
	PMXCONTEXT pCtx;
    PART * pPartDescr;

    _ASSERT (HANDLE_NOT_NULL(hPart));
    _ASSERT (HANDLE_GET_TYPE(hPart) == OBJ_TYPE_PARTITION);
    _ASSERT (HANDLE_GET_CPU(hPart) == MxGetCpuID ());

    pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);

    pPartDescr = MxiGetPartDescr (pCtx, hPart);

    _ASSERT_PTR(pPartDescr);

    pPartDescr->Sync = NULL;

	if (!IS_PART_OPENED(pPartDescr))
	{
		MARK_PART_DESYNC(pPartDescr);
	}

    return MX_OK;
}

#endif /*MX_INTERCPU_ENABLED*/

#endif /*MX_PART_ENABLED */

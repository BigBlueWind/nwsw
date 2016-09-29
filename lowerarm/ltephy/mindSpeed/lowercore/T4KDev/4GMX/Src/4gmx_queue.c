//-------------------------------------------------------------------------------------------
/** @file 4gmx_queue.c
 *
 * @brief Queue API implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"

#ifdef MX_QUEUE_ENABLED

#include "4gmx.h"
#include "4gmx_queue.h"
#include "4gmx_kernel.h"
#include "4gmx_serv.h"
#include "4gmx_debug.h"

#ifdef MX_INTERCPU_ENABLED
#include "4gmx_intercpu.h"
#endif

#if  defined (TRACE_QUEUE_CREATE)    || \
     defined (TRACE_QUEUE_REMOVE)    || \
     defined (TRACE_QUEUE_OPEN)

/** @brief  This function is called by the event API for tracing Queue.

    @param pCtx    [in] - the system context
    @param id      [in] - the trace event ID
    @param hHandle [in] - the object handle (or HNULL)
    @param rc      [in] - error code of operation

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_queue
*/
MXRC AddQueueCtrl (PMXCONTEXT pCtx, TraceEventID id, HANDLE hHandle, MXRC rc)
{
    TraceQueueCtrl trace;

    trace.hQueue = hHandle;
    trace.Rc     = rc;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif

#if  defined (TRACE_QUEUE_PUT) || defined (TRACE_QUEUE_GET)

/** @brief  This function is called by the event API for tracing Queue.

    @param pCtx    [in] - the system context
    @param id      [in] - the trace event ID
    @param hHandle [in] - the object handle (or HNULL)
    @param rc      [in] - error code of operation

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_queue
*/
MXRC AddQueueDataTrace (PMXCONTEXT pCtx, TraceEventID id,
                       HANDLE hHandle, MXRC rc, UINT32 data)
{
    TraceQueueData trace;

    trace.hQueue = hHandle;
    trace.Data   = data;
    trace.rc     = rc;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif


/** @brief The function creates a queue.

    @param pMem   [in] - the pointer to memory (queue storage)
    @param nElmCount [in] - the max number of elements that can be stored in queue
    @param nElmSize  [in] - the size of element in bytes
    @param name   [in] - the name of queue, it can be null
    @param hEvent  [in] - the event associated with the queue, this event will be set when new one element is placed into queue
    @param oHand  [out] - the handle of created object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/
MXRC MxCreateQueue(LPVOID pMem, UINT32 nElmCount, UINT32 nElmSize,
                   const char *name, HANDLE hEvent, HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    HANDLE h = HNULL;
    PQUEUE pQueue;

    DEF_RADDR;

    _ASSERT(pMem != NULL);
    _ASSERT(oHand != NULL);

#ifdef MX_QUEUE_4B_ELMS

    _ASSERT(nElmSize == 4);

#endif

#ifdef MX_QUEUE_OVERFLOW_CHECK_ENABLED

    _ASSERT(nElmCount > 1);

#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pQueue = (PQUEUE) MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_QUEUE]);

    if (pQueue == NULL)
    {
#ifdef TRACE_QUEUE_CREATE
        AddQueueCtrl(pCtx, TEID_CREATE_QUEUE, HNULL, MX_QUEUE_ALLOC_ERROR);
#endif

        MxiRestInt(irq);
        return MX_QUEUE_ALLOC_ERROR;
    }

    Mximemset(pQueue, 0, sizeof (*pQueue));

    pQueue->StorageOrLinkedObj = pMem;
    pQueue->GetIndex = 0;
    pQueue->PutIndex = 0;
    pQueue->QueueSize = nElmCount;
    pQueue->ElmSize = nElmSize;
    pQueue->Sync = 0;

    pQueue->Event = hEvent;
    pQueue->CtrlFlags = 0;

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_QUEUE,
                ((((UINT32) pQueue) -
                  ((UINT32) pCtx->queues)) / sizeof(QUEUE)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_QUEUE], pQueue);

#ifdef TRACE_QUEUE_CREATE
            AddQueueCtrl(pCtx, TEID_CREATE_QUEUE,
                         HNULL, MX_QUEUE_ASSIGN_NAME_ERROR);
#endif

            MxiRestInt(irq);

            return MX_QUEUE_ASSIGN_NAME_ERROR;
        }

        MARK_QUEUE_NAMED(pQueue);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_QUEUE_UNNAMED(pQueue);
    }

    MARK_QUEUE_ALLOC(pQueue);

#ifdef TRACE_QUEUE_CREATE
    AddQueueCtrl(pCtx, TEID_CREATE_QUEUE, h, MX_OK);
#endif

    *oHand = h;

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function allocates the shared queue for simple inter-cpu communication.

    @param pQDescr  [in] - the pointer to the not cached memory for queue descriptor
    @param pMem  [in] - the pointer to the queue storage
    @param nElmCount[in] - the maximum number of queue elements
    @param nElmSize [in] - the element size in bytes
    @param name  [in] - the name of object
    @param hEvent [in] - the Event HANDLE that was used in case of putting data into the queue
    @param oHand [out] - the queue handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/
MXRC MxCreateSharedQueue(ICQUEUE * pQDescr, LPVOID pMem, UINT32 nElmCount,
                      UINT32 nElmSize, const char *name, HANDLE hEvent,
                      HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    HANDLE h = HNULL;
    PQUEUE pAllocQueue;

    DEF_RADDR;

    _ASSERT(pQDescr != NULL);
    _ASSERT(pMem != NULL);
    _ASSERT(oHand != NULL);

#ifdef MX_QUEUE_4B_ELMS
    _ASSERT(nElmSize == 4);
#endif

#ifdef MX_QUEUE_OVERFLOW_CHECK_ENABLED
    _ASSERT(nElmCount > 1);
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pAllocQueue = (PQUEUE) MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_QUEUE]);

    if (pAllocQueue == NULL)
    {
#ifdef TRACE_QUEUE_CREATE
        AddQueueCtrl(pCtx, TEID_CREATE_QUEUE, HNULL, MX_QUEUE_ALLOC_ERROR);
#endif
        MxiRestInt(irq);
        return MX_QUEUE_ALLOC_ERROR;
    }

    Mximemset(pAllocQueue, 0, sizeof (*pAllocQueue));

    pAllocQueue->StorageOrLinkedObj = pQDescr;

    pAllocQueue->GetIndex = 0;
    pAllocQueue->PutIndex = 0;
    pAllocQueue->QueueSize = 0;
    pAllocQueue->ElmSize = 0;
    pAllocQueue->Sync = &pQDescr->Sync;

    pAllocQueue->Event = hEvent;
    pAllocQueue->CtrlFlags = 0;

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_QUEUE,
                ((((UINT32) pAllocQueue) -
                  ((UINT32) pCtx->queues)) / sizeof(QUEUE)));

    MX_CREATE_OBJECT_LOG(h);

    pQDescr->Storage = pMem;
    pQDescr->GetIndex = 0;
    pQDescr->PutIndex = 0;
    pQDescr->QueueSize = nElmCount;
    pQDescr->ElmSize = nElmSize;
    pQDescr->Sync = 0;

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_QUEUE], pAllocQueue);

#ifdef TRACE_QUEUE_CREATE
            AddQueueCtrl(pCtx, TEID_CREATE_QUEUE,
                         HNULL, MX_QUEUE_ASSIGN_NAME_ERROR);
#endif

            MxiRestInt(irq);

            return MX_QUEUE_ASSIGN_NAME_ERROR;
        }

        MARK_QUEUE_NAMED(pAllocQueue);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_QUEUE_UNNAMED(pAllocQueue);
    }

    MARK_QUEUE_OPENED(pAllocQueue);
    MARK_QUEUE_SYNC(pAllocQueue);
    MARK_QUEUE_ALLOC(pAllocQueue);

#ifdef TRACE_QUEUE_CREATE
    AddQueueCtrl(pCtx, TEID_CREATE_QUEUE, h, MX_OK);
#endif

    *oHand = h;

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function "opens"/"set connections with" created queue and returns
           its handle, the opened object can be used like a locally created.

    @param pQueueDescr [in] - the pointer to the inter-cpu queue descriptor
    @param name   [in] - the name of opened object
    @param oHand  [out] - the handle of opened queue

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/
MXRC MxOpenQueue(ICQUEUE * pQueueDescr, const char *name, HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    PQUEUE pQueue, pRemoteQueue;
    IRQTYPE irq;
    HANDLE h = HNULL;

    DEF_RADDR;

    _ASSERT(pQueueDescr != NULL);
    _ASSERT(oHand != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pRemoteQueue = (PQUEUE) pQueueDescr;

    pQueue = (PQUEUE) MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_QUEUE]);

    if (pQueue == NULL)
    {
#ifdef TRACE_QUEUE_OPEN
        AddQueueCtrl(pCtx, TEID_OPEN_QUEUE, HNULL, MX_QUEUE_ALLOC_ERROR);
#endif
        MxiRestInt(irq);
        return MX_QUEUE_ALLOC_ERROR;
    }

    Mximemset(pQueue, 0, sizeof (*pQueue));

    pQueue->StorageOrLinkedObj = pQueueDescr;

    pQueue->CtrlFlags = 0;
    pQueue->ElmSize = 0;
    pQueue->QueueSize = 0;
    pQueue->Event = HNULL;
    pQueue->GetIndex = 0;
    pQueue->PutIndex = 0;
    pQueue->Sync = (MXSYNC) & (pRemoteQueue->Sync);

    MxiSyncCtrlOn(OBJ_TYPE_QUEUE, pQueue);

#ifdef MX_QUEUE_4B_ELMS

    _ASSERT(((PQUEUE) pQueueDescr)->ElmSize == 4);

#endif

#ifdef MX_QUEUE_OVERFLOW_CHECK_ENABLED

    _ASSERT(((PQUEUE) pQueueDescr)->QueueSize > 1);

#endif
    MxiSyncCtrlOff(OBJ_TYPE_QUEUE, pQueue);

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_QUEUE,
                ((((UINT32) pQueue) -
                  ((UINT32) pCtx->queues)) / sizeof(QUEUE)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_QUEUE], pQueue);

#ifdef TRACE_QUEUE_OPEN
            AddQueueCtrl(pCtx, TEID_OPEN_QUEUE,
                         HNULL, MX_QUEUE_ASSIGN_NAME_ERROR);
#endif

            MxiRestInt(irq);

            return MX_QUEUE_ASSIGN_NAME_ERROR;
        }

        MARK_QUEUE_NAMED(pQueue);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_QUEUE_UNNAMED(pQueue);
    }

    MARK_QUEUE_OPENED(pQueue);
    MARK_QUEUE_SYNC(pQueue);
    MARK_QUEUE_ALLOC(pQueue);

#ifdef TRACE_QUEUE_OPEN
    AddQueueCtrl(pCtx, TEID_OPEN_QUEUE, h, MX_OK);
#endif

    *oHand = h;

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function "opens"/"set connections with" created queue and returns
           its handle, the opened object can be used like a locally created.

    @note: this function is designed to be used with HW implemented semas,
            so usage of this function is platform depended

    @param pQueueDescr [in] - the pointer to the inter-cpu queue descriptor
    @param name        [in] - the name of opened object
    @param pHwSema     [in] - the pointer to the HW SEMA register (it cannot be NULL)
    @param bCacheable  [in] - if TRUE - the opened descriptor is cacheable

    @param oHand       [out] - the handle of opened queue

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/
MXRC MxOpenQueueEx(ICQUEUE * pQueueDescr, const char *name, PUINT32 pHwSema, BOOL bCacheable, HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    PQUEUE pQueue;
    IRQTYPE irq;
    HANDLE h = HNULL;

    DEF_RADDR;

    _ASSERT(pQueueDescr != NULL);
    _ASSERT(oHand != NULL);
    _ASSERT_PTR(pHwSema);

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pQueue = (PQUEUE) MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_QUEUE]);

    if (pQueue == NULL)
    {
#ifdef TRACE_QUEUE_OPEN
        AddQueueCtrl(pCtx, TEID_OPEN_QUEUE, HNULL, MX_QUEUE_ALLOC_ERROR);
#endif
        MxiRestInt(irq);
        return MX_QUEUE_ALLOC_ERROR;
    }

    Mximemset(pQueue, 0, sizeof (*pQueue));

    pQueue->StorageOrLinkedObj = pQueueDescr;

    pQueue->CtrlFlags = 0;
    pQueue->ElmSize = 0;
    pQueue->QueueSize = 0;
    pQueue->Event = HNULL;
    pQueue->GetIndex = 0;
    pQueue->PutIndex = 0;
    pQueue->Sync = (MXSYNC) pHwSema;

#ifdef MX_QUEUE_4B_ELMS

    _ASSERT(((PQUEUE) pQueueDescr)->ElmSize == 4);

#endif

#ifdef MX_QUEUE_OVERFLOW_CHECK_ENABLED

    _ASSERT(((PQUEUE) pQueueDescr)->QueueSize > 1);

#endif

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_QUEUE,
                ((((UINT32) pQueue) -
                  ((UINT32) pCtx->queues)) / sizeof(QUEUE)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_QUEUE], pQueue);

#ifdef TRACE_QUEUE_OPEN
            AddQueueCtrl(pCtx, TEID_OPEN_QUEUE,
                         HNULL, MX_QUEUE_ASSIGN_NAME_ERROR);
#endif

            MxiRestInt(irq);

            return MX_QUEUE_ASSIGN_NAME_ERROR;
        }

        MARK_QUEUE_NAMED(pQueue);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_QUEUE_UNNAMED(pQueue);
    }

    MARK_QUEUE_OPENED(pQueue);
    MARK_QUEUE_SYNC(pQueue);
    MARK_QUEUE_ALLOC(pQueue);
    MARK_QUEUE_HW_SEMA(pQueue);

    if (bCacheable)
    {
        MARK_QUEUE_OPEN_CACHE(pQueue);
    }

#ifdef TRACE_QUEUE_OPEN
    AddQueueCtrl(pCtx, TEID_OPEN_QUEUE, h, MX_OK);
#endif

    *oHand = h;

    MxiRestInt(irq);

    return MX_OK;
}


/** @brief The function removes created queue.

    @param h [in] - the queue handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/
MXRC MxRemoveQueue(HANDLE h)
{
    PMXCONTEXT pCtx;
    PQUEUE pQueue;
    IRQTYPE irq;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_QUEUE);

    /* Only locally created object can be removed */

    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pQueue = (PQUEUE) MxiGetQueueDescr(pCtx, h);

    _ASSERT(pQueue != NULL);
    _ASSERT(IS_QUEUE_ALLOC(pQueue));

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        MxiSyncCtrlOn(OBJ_TYPE_QUEUE, pQueue);
    }

#ifdef MX_NAMING_ENABLED

    if (IS_QUEUE_NAMED(pQueue))
    {
        MxiFreeName(pCtx, h);

        MARK_QUEUE_UNNAMED(pQueue);
    }

#endif /*MX_NAMING_ENABLED */

    pQueue->CtrlFlags = 0;

    MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_QUEUE], pQueue);

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        MxiSyncCtrlOff(OBJ_TYPE_QUEUE, pQueue);
    }

#ifdef TRACE_QUEUE_REMOVE
    AddQueueCtrl(pCtx, TEID_REMOVE_QUEUE, h, MX_OK);
#endif

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function puts data into a queue, if an event object is not HNULL,
           the event object will be set.

    @param h [in] - the queue handle
    @param pData[in] - the data placed into queue

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/

#ifdef MX_TRACER_ENABLED
    MXRC MxPutQueueData(HANDLE h, LPVOID pData)
    {
        UINT32 trace_lr = MX_GET_RADDR;
        return MxPutQueueDataLR (h, pData, trace_lr);
    }

    MXRC MxPutQueueDataLR(HANDLE h, LPVOID pData, UINT32 trace_lr)

#else 
    MXRC MxPutQueueData(HANDLE h, LPVOID pData)
#endif
{
    PMXCONTEXT pCtx;
    PQUEUE pQueue, pRemQueue;
    IRQTYPE irq;
    MXRC rc = MX_OK;
    UINT16 put, put2;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_QUEUE);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifndef MX_INTERCPU_ENABLED

        _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

        MxiDisInt(&irq);

        pQueue = MxiGetQueueDescr(pCtx, h);

#else
        MxiDisInt(&irq);

        if (HANDLE_GET_CPU(h) != MxGetCpuID())
        {
            pQueue = (PQUEUE)MxiFindSharedDescr (pCtx, h);

#ifndef MX_ARM_SMP_ENABLED
            MxiCacheInvalid(pQueue, sizeof (QUEUE));
#endif
        }
        else
        {
            pQueue = MxiGetQueueDescr(pCtx, h);
        }
#endif

    _ASSERT(pQueue != NULL);
    _ASSERT(IS_QUEUE_ALLOC(pQueue));

    if (IS_QUEUE_OPENED(pQueue))
    {
        _ASSERT(pQueue->StorageOrLinkedObj != NULL);

        pRemQueue = (PQUEUE) pQueue->StorageOrLinkedObj;
    }
    else
    {
        pRemQueue = (PQUEUE) pQueue;
    }

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        if (FAILED(MxiSyncCtrlOn(OBJ_TYPE_QUEUE, pQueue)))
        {
            MxiRestInt(irq);
            return MX_QUEUE_SYNC_TIMEOUT;
        }
    }

    if (IS_QUEUE_OPEN_CACHE(pQueue))
    {
        MxiCacheInvalid(pRemQueue, sizeof (*pRemQueue));
    }

    put = pRemQueue->PutIndex;

    put2 = ((put + 1) < pRemQueue->QueueSize) ? (put + 1) : 0;

#ifdef MX_QUEUE_OVERFLOW_CHECK_ENABLED

    if (put2 == pRemQueue->GetIndex)
    {
        rc = MX_QUEUE_OVERFLOW;

#ifdef TRACE_QUEUE_PUT
        AddQueueDataTrace(pCtx, TEID_PUT_QUEUE, h, rc, (UINT32)pData);
#endif

    }
    else
    {

#endif /*MX_QUEUE_OVERFLOW_CHECK_ENABLED */

#ifdef MX_QUEUE_4B_ELMS

        ((UINT32 *) pRemQueue->StorageOrLinkedObj)[put] = (UINT32) pData;

#else /*not MX_QUEUE_4B_ELMS */

        _ASSERT(pData != NULL);

        Mximemcpy((UINT8 *) pRemQueue->StorageOrLinkedObj +
                  put * pRemQueue->ElmSize, pData, pRemQueue->ElmSize);

#endif /*#ifdef MX_QUEUE_4B_ELMS */

#ifndef MX_ARM_SMP_ENABLED
        if (IS_QUEUE_SYNC(pQueue))
        {
            MxiCacheClean((UINT8 *) pRemQueue->StorageOrLinkedObj +
                           put * pRemQueue->ElmSize, pRemQueue->ElmSize);
        }
#endif

        //pRemQueue->PutIndex = ((put + 1) % pRemQueue->QueueSize);

        pRemQueue->PutIndex = put2;

#ifdef TRACE_QUEUE_PUT
        AddQueueDataTrace(pCtx, TEID_PUT_QUEUE, h, MX_OK, (UINT32)pData);
#endif

        #ifdef MX_TRACER_ENABLED
        if (pQueue->TrPut != NULL)
        {
            pQueue->TrLR        = trace_lr;
            pQueue->TrNumFree   = (put2 >= pRemQueue->GetIndex) ? (put2 - pRemQueue->GetIndex) : (pRemQueue->QueueSize -
                                                                (pRemQueue->GetIndex - put2));
            pQueue->TrPut(pQueue->TrPtr, (UINT32)pData);
        }
        #endif

#ifdef MX_QUEUE_OVERFLOW_CHECK_ENABLED
    }
#endif

    if (IS_QUEUE_OPEN_CACHE(pQueue))
    {
        MxiCacheClean (pRemQueue, sizeof (*pRemQueue));
    }

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        MxiSyncCtrlOff(OBJ_TYPE_QUEUE, pQueue);
    }

#ifdef MX_EVENT_ENABLED

    if (HANDLE_NOT_NULL(pQueue->Event) && SUCCESSFUL(rc))
    {
        rc = MxSetEvent(pQueue->Event);
    }

#endif

    MxiRestInt(irq);

    return rc;
}

/** @brief The function puts data to the queue,
    if an event object is not HNULL it will be set.

    @param h [in] - the queue handle
    @param pData[in] - the data placed into queue
    @param pCount[out] - the current number of elements in this queue

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/

#ifdef MX_TRACER_ENABLED
    MXRC MxPutQueueDataEx(HANDLE h, LPVOID pData, PUINT32 pCount)
    {
        UINT32 trace_lr = MX_GET_RADDR;
        return MxPutQueueDataExLR(h, pData, pCount, trace_lr);
    }

    MXRC MxPutQueueDataExLR(HANDLE h, LPVOID pData, PUINT32 pCount, UINT32 trace_lr)

#else
    MXRC MxPutQueueDataEx(HANDLE h, LPVOID pData, PUINT32 pCount)
#endif

{
    PMXCONTEXT pCtx;
    PQUEUE pQueue, pRemQueue;
    IRQTYPE irq;
    MXRC rc = MX_OK;
    UINT16 put, put2;

    #ifdef MX_TRACER_ENABLED
    
    #endif

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_QUEUE);
    _ASSERT_PTR(pCount);

    pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);

#ifndef MX_INTERCPU_ENABLED

        _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

        MxiDisInt(&irq);

        pQueue = MxiGetQueueDescr(pCtx, h);

#else
        MxiDisInt(&irq);

        if (HANDLE_GET_CPU(h) != MxGetCpuID())
        {
            pQueue = (PQUEUE)MxiFindSharedDescr (pCtx, h);

#ifndef MX_ARM_SMP_ENABLED
            MxiCacheInvalid(pQueue, sizeof (QUEUE));
#endif
        }
        else
        {
            pQueue = MxiGetQueueDescr(pCtx, h);
        }
#endif

    _ASSERT(pQueue != NULL);
    _ASSERT(IS_QUEUE_ALLOC(pQueue));

    if (IS_QUEUE_OPENED(pQueue))
    {
        _ASSERT(pQueue->StorageOrLinkedObj != NULL);

        pRemQueue = (PQUEUE) pQueue->StorageOrLinkedObj;
    }
    else
    {
        pRemQueue = (PQUEUE) pQueue;
    }

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        if (FAILED(MxiSyncCtrlOn(OBJ_TYPE_QUEUE, pQueue)))
        {
            MxiRestInt(irq);
            return MX_QUEUE_SYNC_TIMEOUT;
        }
    }

    if (IS_QUEUE_OPEN_CACHE(pQueue))
    {
        MxiCacheInvalid(pRemQueue, sizeof (*pRemQueue));
    }

    put = pRemQueue->PutIndex;

    put2 = put + 1;

    if (put2 >= pRemQueue->QueueSize)
    {
    	put2 = 0;
    }

#ifdef MX_QUEUE_OVERFLOW_CHECK_ENABLED

    if (put2 == pRemQueue->GetIndex)
    {
        rc = MX_QUEUE_OVERFLOW;

#ifdef TRACE_QUEUE_PUT
        AddQueueDataTrace(pCtx, TEID_PUT_QUEUE, h, rc, (UINT32)pData);
#endif

    }
    else
    {

#endif /*MX_QUEUE_OVERFLOW_CHECK_ENABLED */

#ifdef MX_QUEUE_4B_ELMS

        ((UINT32 *) pRemQueue->StorageOrLinkedObj)[put] = (UINT32) pData;

#else /*not MX_QUEUE_4B_ELMS */

        _ASSERT(pData != NULL);

        Mximemcpy((UINT8 *) pRemQueue->StorageOrLinkedObj +
                  put * pRemQueue->ElmSize, pData, pRemQueue->ElmSize);

#endif /*#ifdef MX_QUEUE_4B_ELMS */

#ifndef MX_ARM_SMP_ENABLED
        if (IS_QUEUE_SYNC(pQueue))
        {
            MxiCacheClean((UINT8 *) pRemQueue->StorageOrLinkedObj +
                           put * pRemQueue->ElmSize, pRemQueue->ElmSize);
        }
#endif

        //pRemQueue->PutIndex = ((put + 1) % pRemQueue->QueueSize);

        pRemQueue->PutIndex = put2;

        // to set the number of elements in the queue

        *pCount = (put2 >= pRemQueue->GetIndex) ? (put2 - pRemQueue->GetIndex) : (pRemQueue->QueueSize -
                                                                (pRemQueue->GetIndex - put2));

#ifdef TRACE_QUEUE_PUT
        AddQueueDataTrace(pCtx, TEID_PUT_QUEUE, h, MX_OK, (UINT32)pData);
#endif

        #ifdef MX_TRACER_ENABLED
        if (pQueue->TrPut != NULL)
        {
            pQueue->TrLR = trace_lr;
            pQueue->TrNumFree = *pCount;
            pQueue->TrPut(pQueue->TrPtr, (UINT32)pData);
        }
        #endif

#ifdef MX_QUEUE_OVERFLOW_CHECK_ENABLED
    }
#endif

    if (IS_QUEUE_OPEN_CACHE(pQueue))
    {
        MxiCacheClean (pRemQueue, sizeof (*pRemQueue));
    }

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        MxiSyncCtrlOff(OBJ_TYPE_QUEUE, pQueue);
    }

#ifdef MX_EVENT_ENABLED

    if (HANDLE_NOT_NULL(pQueue->Event) && SUCCESSFUL(rc))
    {
        rc = MxSetEvent(pQueue->Event);
    }

#endif

    MxiRestInt(irq);

    return rc;
}


/** @brief The function returns queue element, the returned element will be deleted from the queue

    @param h [in] - the queue handle
    @param pData[out] - the buffer for data storing

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/

#ifdef MX_TRACER_ENABLED

    MXRC MxGetQueueData(HANDLE h, LPVOID pData) 
    {
        UINT32 trace_lr = MX_GET_RADDR;
        return MxGetQueueDataLR(h, pData, trace_lr);
    }

    MXRC MxGetQueueDataLR(HANDLE h, LPVOID pData, UINT32 trace_lr)
#else
    MXRC MxGetQueueData(HANDLE h, LPVOID pData)
#endif
{
    PMXCONTEXT pCtx;
    PQUEUE pQueue, pRemQueue;
    IRQTYPE irq;
    MXRC rc = MX_OK;
    UINT16 get;

#ifdef TRACE_QUEUE_GET
    UINT32 nTraceVal;
#endif

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_QUEUE);
    _ASSERT(pData != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifndef MX_INTERCPU_ENABLED

        _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

        MxiDisInt(&irq);

        pQueue = MxiGetQueueDescr(pCtx, h);

#else
        MxiDisInt(&irq);

        if (HANDLE_GET_CPU(h) != MxGetCpuID())
        {
            pQueue = (PQUEUE)MxiFindSharedDescr (pCtx, h);

#ifndef MX_ARM_SMP_ENABLED
            MxiCacheInvalid(pQueue, sizeof (QUEUE));
#endif
        }
        else
        {
            pQueue = MxiGetQueueDescr(pCtx, h);
        }
#endif

    _ASSERT(pQueue != NULL);
    _ASSERT(IS_QUEUE_ALLOC(pQueue));

    if (IS_QUEUE_OPENED(pQueue))
    {
        _ASSERT(pQueue->StorageOrLinkedObj != NULL);

        pRemQueue = (PQUEUE) pQueue->StorageOrLinkedObj;
    }
    else
    {
        pRemQueue = (PQUEUE) pQueue;
    }

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        if (FAILED(MxiSyncCtrlOn(OBJ_TYPE_QUEUE, pQueue)))
        {
            MxiRestInt(irq);
            return MX_QUEUE_SYNC_TIMEOUT;
        }
    }

    if (IS_QUEUE_OPEN_CACHE(pQueue))
    {
        MxiCacheInvalid(pRemQueue, sizeof (*pRemQueue));
    }

    get = pRemQueue->GetIndex;

    if (get != pRemQueue->PutIndex)
    {
#ifndef MX_ARM_SMP_ENABLED
        if (IS_QUEUE_SYNC(pQueue))
        {
            MxiCacheInvalid((UINT8*)pRemQueue->StorageOrLinkedObj +
                              get * pRemQueue->ElmSize, pRemQueue->ElmSize);
        }
#endif

#ifdef MX_QUEUE_4B_ELMS

        *(UINT32 *) pData = ((UINT32 *) pRemQueue->StorageOrLinkedObj)[get];

#ifdef TRACE_QUEUE_GET
        nTraceVal = *(UINT32 *) pData;
#endif

#else /*not MX_QUEUE_4B_ELMS */

        if (pRemQueue->ElmSize == sizeof(UINT32))
        {
            *(UINT32 *) pData = ((UINT32 *) pRemQueue->StorageOrLinkedObj)[get];

#ifdef TRACE_QUEUE_GET
            nTraceVal = *(UINT32 *) pData;
#endif

        }
        else if (pRemQueue->ElmSize == sizeof(UINT16))
        {
            *(UINT16 *) pData = ((UINT16 *) pRemQueue->StorageOrLinkedObj)[get];

#ifdef TRACE_QUEUE_GET
            nTraceVal = *(UINT16 *) pData;
#endif
        }
        else if (pRemQueue->ElmSize == sizeof(UINT8))
        {
            *(UINT8 *) pData = ((UINT8 *) pRemQueue->StorageOrLinkedObj)[get];

#ifdef TRACE_QUEUE_GET
            nTraceVal = *(UINT8 *) pData;
#endif
        }
        else
        {

#ifdef TRACE_QUEUE_GET
            nTraceVal = (UINT32) pData;
#endif
            Mximemcpy(pData,
                      (UINT8 *) pRemQueue->StorageOrLinkedObj +
                      get * pRemQueue->ElmSize, pRemQueue->ElmSize);
        }
#endif /*#ifdef MX_QUEUE_4B_ELMS */

#ifdef TRACE_QUEUE_GET
       AddQueueDataTrace(pCtx, TEID_GET_QUEUE, h, MX_OK, nTraceVal);
#endif

        //pRemQueue->GetIndex = ((get + 1) % pRemQueue->QueueSize);

        pRemQueue->GetIndex = ((get + 1) < pRemQueue->QueueSize) ? (get + 1) : 0;

        #ifdef MX_TRACER_ENABLED
        if (pQueue->TrGet != NULL)
        {
            pQueue->TrLR = trace_lr;
            pQueue->TrGet(pQueue->TrPtr, *(UINT32*)pData);
        }
        #endif

    }
    else
    {
        rc = MX_QUEUE_EMPTY;

#ifdef TRACE_QUEUE_GET
        AddQueueDataTrace(pCtx, TEID_GET_QUEUE, h, rc, (UINT32)pData);
#endif

    }

    if (IS_QUEUE_OPEN_CACHE(pQueue))
    {
        MxiCacheClean(pRemQueue, sizeof (*pRemQueue));
    }

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        MxiSyncCtrlOff(OBJ_TYPE_QUEUE, pQueue);
    }

    MxiRestInt(irq);

    return rc;
}

/** @brief The function returns queue element without an erasing operation

    @param h [in] - the queue handle
    @param pData[out] - the buffer for data storing

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/
MXRC MxPeekQueueData(HANDLE h, LPVOID pData)
{
    PMXCONTEXT pCtx;
    PQUEUE pQueue, pRemQueue;
    IRQTYPE irq;
    MXRC rc = MX_OK;
    UINT16 get;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_QUEUE);
    _ASSERT(pData != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifndef MX_INTERCPU_ENABLED

        _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

        MxiDisInt(&irq);

        pQueue = MxiGetQueueDescr(pCtx, h);

#else
        MxiDisInt(&irq);

        if (HANDLE_GET_CPU(h) != MxGetCpuID())
        {
            pQueue = (PQUEUE)MxiFindSharedDescr (pCtx, h);

#ifndef MX_ARM_SMP_ENABLED
            MxiCacheInvalid(pQueue, sizeof (QUEUE));
#endif
        }
        else
        {
            pQueue = MxiGetQueueDescr(pCtx, h);
        }
#endif

    _ASSERT(pQueue != NULL);
    _ASSERT(IS_QUEUE_ALLOC(pQueue));

    if (IS_QUEUE_OPENED(pQueue))
    {
        _ASSERT(pQueue->StorageOrLinkedObj != NULL);

        pRemQueue = (PQUEUE) pQueue->StorageOrLinkedObj;
    }
    else
    {
        pRemQueue = (PQUEUE) pQueue;
    }

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        if (FAILED(MxiSyncCtrlOn(OBJ_TYPE_QUEUE, pQueue)))
        {
            MxiRestInt(irq);
            return MX_QUEUE_SYNC_TIMEOUT;
        }
    }

    if (IS_QUEUE_OPEN_CACHE(pQueue))
    {
        MxiCacheInvalid(pRemQueue, sizeof (*pRemQueue));
    }

    get = pRemQueue->GetIndex;

    if (get != pRemQueue->PutIndex)
    {
#ifndef MX_ARM_SMP_ENABLED
        if (IS_QUEUE_SYNC(pQueue))
        {
            MxiCacheInvalid((PUINT8)pRemQueue->StorageOrLinkedObj +
                              get * pRemQueue->ElmSize, pRemQueue->ElmSize);
        }
#endif

#ifdef MX_QUEUE_4B_ELMS

        *(UINT32 *) pData = ((UINT32 *) pRemQueue->StorageOrLinkedObj)[get];

#else /*#ifdef MX_QUEUE_4B_ELMS */

        if (pRemQueue->ElmSize == sizeof(UINT32))
        {
            *(UINT32 *) pData = ((UINT32 *) pRemQueue->StorageOrLinkedObj)[get];
        }
        else if (pRemQueue->ElmSize == sizeof(UINT16))
        {
            *(UINT16 *) pData = ((UINT16 *) pRemQueue->StorageOrLinkedObj)[get];
        }
        else if (pRemQueue->ElmSize == sizeof(UINT8))
        {
            *(UINT8 *) pData = ((UINT8 *) pRemQueue->StorageOrLinkedObj)[get];
        }
        else
        {
            Mximemcpy(pData,
                      (UINT8 *) pRemQueue->StorageOrLinkedObj +
                      get * pRemQueue->ElmSize, pRemQueue->ElmSize);
        }
#endif /*#ifdef MX_QUEUE_4B_ELMS */

    }
    else
    {
        rc = MX_QUEUE_EMPTY;
    }

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        MxiSyncCtrlOff(OBJ_TYPE_QUEUE, pQueue);
    }

    MxiRestInt(irq);

    return rc;
}

/** @brief The function empties queue.

    @param h [in] - the queue handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/
MXRC MxEmptyQueue(HANDLE h)
{
    PMXCONTEXT pCtx;
    PQUEUE pQueue, pRemQueue;
    IRQTYPE irq;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_QUEUE);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifndef MX_INTERCPU_ENABLED

        _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

        MxiDisInt(&irq);

        pQueue = MxiGetQueueDescr(pCtx, h);

#else
        MxiDisInt(&irq);

        if (HANDLE_GET_CPU(h) != MxGetCpuID())
        {
            pQueue = (PQUEUE)MxiFindSharedDescr (pCtx, h);

#ifndef MX_ARM_SMP_ENABLED
            MxiCacheInvalid(pQueue, sizeof (QUEUE));
#endif
        }
        else
        {
            pQueue = MxiGetQueueDescr(pCtx, h);
        }
#endif

    _ASSERT(pQueue != NULL);
    _ASSERT(IS_QUEUE_ALLOC(pQueue));

    if (IS_QUEUE_OPENED(pQueue))
    {
        _ASSERT(pQueue->StorageOrLinkedObj != NULL);

        pRemQueue = (PQUEUE) pQueue->StorageOrLinkedObj;
    }
    else
    {
        pRemQueue = (PQUEUE) pQueue;
    }

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        if (FAILED(MxiSyncCtrlOn(OBJ_TYPE_QUEUE, pQueue)))
        {
            MxiRestInt(irq);
            return MX_QUEUE_SYNC_TIMEOUT;
        }
    }

    if (IS_QUEUE_OPEN_CACHE(pQueue))
    {
        MxiCacheInvalid(pRemQueue, sizeof (*pRemQueue));
    }

    pRemQueue->PutIndex = pRemQueue->GetIndex;

    if (IS_QUEUE_OPEN_CACHE(pQueue))
    {
        MxiCacheClean(pRemQueue, sizeof (*pRemQueue));
    }

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        MxiSyncCtrlOff(OBJ_TYPE_QUEUE, pQueue);
    }

#ifdef TRACE_QUEUE_EMPTY
    AddQueueCtrl(pCtx, TEID_EMPTY_QUEUE, h, MX_OK);
#endif

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function returns the maximum number of elements that can be putted to the queue

    @param h  [in] - the queue handle
    @param pnCount [out] - the number of queue elements

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/
UINT32 MxGetQueueSize(HANDLE h)
{
    PMXCONTEXT pCtx;
    PQUEUE pQueue, pRemQueue;
    IRQTYPE irq;
    UINT32 nSize = 0;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_QUEUE);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifndef MX_INTERCPU_ENABLED
        _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
        MxiDisInt(&irq);
        pQueue = MxiGetQueueDescr(pCtx, h);

#else
        MxiDisInt(&irq);
        if (HANDLE_GET_CPU(h) != MxGetCpuID())
        {
            pQueue = (PQUEUE)MxiFindSharedDescr (pCtx, h);

#ifndef MX_ARM_SMP_ENABLED
            MxiCacheInvalid(pQueue, sizeof (QUEUE));
#endif
        }
        else
        {
            pQueue = MxiGetQueueDescr(pCtx, h);
        }
#endif

    _ASSERT(pQueue != NULL);
    _ASSERT(IS_QUEUE_ALLOC(pQueue));

    if (IS_QUEUE_OPENED(pQueue))
    {
        _ASSERT(pQueue->StorageOrLinkedObj != NULL);
        pRemQueue = (PQUEUE) pQueue->StorageOrLinkedObj;
    }
    else
    {
        pRemQueue = (PQUEUE) pQueue;
    }

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        if (FAILED(MxiSyncCtrlOn(OBJ_TYPE_QUEUE, pQueue)))
        {
            MxiRestInt(irq);
            return MX_QUEUE_SYNC_TIMEOUT;
        }
    }

    if (IS_QUEUE_OPEN_CACHE(pQueue))
    {
        MxiCacheInvalid(pRemQueue, sizeof (*pRemQueue));
    }

    if (pRemQueue->QueueSize != 0)
    {
        nSize = pRemQueue->QueueSize - 1;
    }

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        MxiSyncCtrlOff(OBJ_TYPE_QUEUE, pQueue);
    }

    MxiRestInt(irq);

    return nSize;
}

/** @brief The function returns number of elements currently present in queue

    @param h  [in] - the queue handle
    @param pnCount [out] - the number of queue elements

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/
MXRC MxGetQueueDataCount(HANDLE h, UINT32 * pnCount)
{
    PMXCONTEXT pCtx;
    PQUEUE pQueue, pRemQueue;
    IRQTYPE irq;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_QUEUE);
    _ASSERT(pnCount != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifndef MX_INTERCPU_ENABLED

        _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

        MxiDisInt(&irq);

        pQueue = MxiGetQueueDescr(pCtx, h);

#else
        MxiDisInt(&irq);

        if (HANDLE_GET_CPU(h) != MxGetCpuID())
        {
            pQueue = (PQUEUE)MxiFindSharedDescr (pCtx, h);

#ifndef MX_ARM_SMP_ENABLED
            MxiCacheInvalid(pQueue, sizeof (QUEUE));
#endif
        }
        else
        {
            pQueue = MxiGetQueueDescr(pCtx, h);
        }
#endif

    _ASSERT(pQueue != NULL);
    _ASSERT(IS_QUEUE_ALLOC(pQueue));

    if (IS_QUEUE_OPENED(pQueue))
    {
        _ASSERT(pQueue->StorageOrLinkedObj != NULL);

        pRemQueue = (PQUEUE) pQueue->StorageOrLinkedObj;
    }
    else
    {
        pRemQueue = (PQUEUE) pQueue;
    }

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        if (FAILED(MxiSyncCtrlOn(OBJ_TYPE_QUEUE, pQueue)))
        {
            MxiRestInt(irq);
            return MX_QUEUE_SYNC_TIMEOUT;
        }
    }

    if (IS_QUEUE_OPEN_CACHE(pQueue))
    {
        MxiCacheInvalid(pRemQueue, sizeof (*pRemQueue));
    }

    *pnCount =
        (pRemQueue->PutIndex >=  pRemQueue->GetIndex) ? (pRemQueue->PutIndex -
                                 pRemQueue->GetIndex) : (pRemQueue->QueueSize -
                                                         (pRemQueue->GetIndex -
                                                          pRemQueue->PutIndex));

    if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
    {
        MxiSyncCtrlOff(OBJ_TYPE_QUEUE, pQueue);
    }

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function configures queue object 

 @param h  [in] - the queue handle
 @param id [in] - the queue config parameter: see MX_QUEUE_CFG_xxx
 @param val[in] - parameter

 @return [MXRC] error code */

MXRC MxQueueConfig (HANDLE h, UINT32 id, UINT32 val)
{
    PMXCONTEXT pCtx;
    PQUEUE pQueue;
    IRQTYPE irq;
    MXRC rc = MX_OK;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_QUEUE);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
        return MX_QUEUE_REMOTE_OBJ_NOT_SUP;

    MxiDisInt(&irq);
    
    pQueue = MxiGetQueueDescr(pCtx, h);
        
    _ASSERT(pQueue != NULL);
    _ASSERT(IS_QUEUE_ALLOC(pQueue));

    switch (id)
    {
        case MX_QUEUE_CFG_LOCK_TIMEOUT:
        {   
            pQueue->SyncTicks = val;
            break;
        }
        
        default:
        {
            rc = MX_QUEUE_UNKNOWN_PARAMETER;
            break;
        }
    }

    MxiRestInt(irq);

    return rc;
}

#ifdef MX_INTERCPU_ENABLED

/** @brief This function shares the queue

    @param hQueue  [in] - the queue handler
    @param pLock   [in] - the address of the sync object (in non cached memory)

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/
MXRC MxShareQueue (HANDLE INP hQueue, PVUINT32 pLock)
{
    PMXCONTEXT pCtx;
    QUEUE * pDescr;
	MXRC rc = MX_OK;

    _ASSERT (HANDLE_NOT_NULL(hQueue));

    _ASSERT_PTR(pLock);

    _ASSERT (HANDLE_GET_TYPE(hQueue) == OBJ_TYPE_QUEUE);
    _ASSERT (HANDLE_GET_CPU(hQueue) == MxGetCpuID ());

    pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);

    pDescr = MxiGetQueueDescr (pCtx, hQueue);

    _ASSERT_PTR(pDescr);

	// The opened object cannot be shared (it's a senseless operation)
	// it can be directly opened by calling MxOpenQueue

	if (IS_PART_OPENED(pDescr))
	{
		rc = MX_QUEUE_CANNOT_BE_SHARED;

		_ASSERT_RC (rc);

		return rc;
	}

	// The shared object should use synchronization

    pDescr->Sync = pLock;

	MARK_QUEUE_SYNC(pDescr);

#ifndef MX_ARM_SMP_ENABLED
    MxiCacheClean(pDescr, sizeof (QUEUE));
#endif

    return rc;
}

/** @brief This function unshares the queue and this queue cannot be accessed
           from the other 4GMX systems

    @param hQueue [in]  - the queue handler

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/
MXRC MxUnshareQueue (HANDLE INP hQueue)
{
    PMXCONTEXT pCtx;
    QUEUE* pDescr;

    _ASSERT (HANDLE_NOT_NULL(hQueue));
    _ASSERT (HANDLE_GET_TYPE(hQueue) == OBJ_TYPE_QUEUE);
    _ASSERT (HANDLE_GET_CPU(hQueue) == MxGetCpuID ());

    pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);

    pDescr = MxiGetQueueDescr (pCtx, hQueue);

    _ASSERT_PTR(pDescr);

    pDescr->Sync = NULL;

	if (!IS_QUEUE_OPENED(pDescr))
	{
		MARK_QUEUE_DESYNC(pDescr);
	}

    return MX_OK;
}

#endif /*MX_INTERCPU_ENABLED*/

#endif /*MX_QUEUE_ENABLED */


//-------------------------------------------------------------------------------------------
/** @file 4gmx_tracer.c
 *
 * @brief The file contains implementation of 4GMX tracer service designed to track partitions and queues objects
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_tracer.h"
#include "4gmx_thread.h"
#include "4gmx_part.h"
#include "4gmx_queue.h"
#include "4gmx_kernel.h"

#ifdef MX_TRACER_ENABLED

#include "4gmx_serv.h"

MX_TRACE_PART_INFO * MxiTracePartAdd (MX_TRACER_CTX * pCtx)
{
    MX_TRACE_PART_INFO * pInfo = &((MX_TRACE_PART_INFO *)pCtx->TracePtr)[pCtx->Put];

    pCtx->Put ++;

    if (pCtx->Put >= pCtx->Num)
        pCtx->Put = 0;

    // just to remove one element from the queue 
    // to prevent situation of queue overflow

    if (pCtx->Put == pCtx->Get)
    {
        pCtx->Get ++;
        
        if (pCtx->Get >= pCtx->Num)
            pCtx->Get = 0;
    }

    return pInfo;
}

MX_TRACE_QUEUE_INFO * MxiTraceQueueAdd (MX_TRACER_CTX * pCtx)
{
    MX_TRACE_QUEUE_INFO * pInfo = &((MX_TRACE_QUEUE_INFO*)pCtx->TracePtr)[pCtx->Put];

    pCtx->Put ++;

    if (pCtx->Put >= pCtx->Num)
        pCtx->Put = 0;

    // just to remove one element from the queue 
    // to prevent situation of queue overflow

    if (pCtx->Put == pCtx->Get)
    {
        pCtx->Get ++;
        
        if (pCtx->Get >= pCtx->Num)
            pCtx->Get = 0;
    }

    return pInfo;
}

void MxiTracePartAlloc(void * pTrCtx, void * pBlock)
{
    MX_TRACE_PART_INFO * pInfo;
    MX_TRACER_CTX * pCtx = (MX_TRACER_CTX *)pTrCtx;
    PPART pPart;

    _ASSERT_PTR(pTrCtx);

    pPart = (PPART)pCtx->ObjCtx;
    _ASSERT_PTR(pPart);

    pInfo = MxiTracePartAdd(pCtx);

    pInfo->OperationID = MX_TRACE_OP_ID_PART_ALLOC;
    pInfo->Index       = pCtx->Index++;

    #ifdef MX_TRACE_PART_LR
    pInfo->LR = pPart->TrLR;
    #endif

    #ifdef MX_TRACE_PART_CPU_ID
    pInfo->CpuID = MxGetCpuID();
    #endif

    #ifdef MX_TRACE_PART_TIME
    pInfo->Time = MxGetTicks();
    #endif

    #ifdef MX_TRACE_PART_FREE_BLOCKS
    pInfo->FreeNum = pPart->TrFreeBlk;
    #endif

    #ifdef MX_TRACE_PART_PTR
    pInfo->Ptr = (UINT32)pBlock;
    #endif

}

void MxiTracePartFree(void * pTrCtx, void * pBlock)
{
    MX_TRACE_PART_INFO * pInfo;
    MX_TRACER_CTX * pCtx = (MX_TRACER_CTX *)pTrCtx;
    PPART pPart;

    _ASSERT_PTR(pTrCtx);

    pPart = (PPART)pCtx->ObjCtx;
    _ASSERT_PTR(pPart);

    pInfo = MxiTracePartAdd(pCtx);

    pInfo->OperationID = MX_TRACE_OP_ID_PART_DEALLOC;
    pInfo->Index       = pCtx->Index++;

    #ifdef MX_TRACE_PART_LR
    pInfo->LR = pPart->TrLR;
    #endif

    #ifdef MX_TRACE_PART_CPU_ID
    pInfo->CpuID = MxGetCpuID();
    #endif

    #ifdef MX_TRACE_PART_TIME
    pInfo->Time = MxGetTicks();
    #endif

    #ifdef MX_TRACE_PART_FREE_BLOCKS
    pInfo->FreeNum = pPart->TrFreeBlk;
    #endif

    #ifdef MX_TRACE_PART_PTR
    pInfo->Ptr = (UINT32)pBlock;
    #endif
}

void MxiTraceQueuePut(void * pTrCtx, UINT32 data)
{
    MX_TRACE_QUEUE_INFO * pInfo;
    MX_TRACER_CTX * pCtx = (MX_TRACER_CTX *)pTrCtx;
    PQUEUE pQueue;

    _ASSERT_PTR(pTrCtx);

    pQueue = (PQUEUE)pCtx->ObjCtx;
    _ASSERT_PTR(pQueue);

    pInfo = MxiTraceQueueAdd(pCtx);

    pInfo->OperationID = MX_TRACE_OP_ID_QUEUE_PUT;
    pInfo->Index       = pCtx->Index++;

    #ifdef MX_TRACE_QUEUE_LR
    pInfo->LR = pQueue->TrLR;
    #endif

    #ifdef MX_TRACE_QUEUE_CPU_ID
    pInfo->CpuID = MxGetCpuID();
    #endif

    #ifdef MX_TRACE_QUEUE_TIME
    pInfo->Time = MxGetTicks();
    #endif

    #ifdef MX_TRACE_QUEUE_FREE_SIZE
    pInfo->FreeNum = pQueue->TrNumFree;
    #endif

    #ifdef MX_TRACE_QUEUE_VAL
    pInfo->Val = (UINT32)data;
    #endif
   
}

void MxiTraceQueueGet(void * pTrCtx, UINT32 data)
{
    MX_TRACE_QUEUE_INFO * pInfo;
    MX_TRACER_CTX * pCtx = (MX_TRACER_CTX *)pTrCtx;
    PQUEUE pQueue;

    _ASSERT_PTR(pTrCtx);

    pQueue = (PQUEUE)pCtx->ObjCtx;
    _ASSERT_PTR(pQueue);

    pInfo = MxiTraceQueueAdd(pCtx);

    pInfo->OperationID = MX_TRACE_OP_ID_QUEUE_GET;
    pInfo->Index       = pCtx->Index++;

    #ifdef MX_TRACE_QUEUE_LR
    pInfo->LR = pQueue->TrLR;
    #endif

    #ifdef MX_TRACE_QUEUE_CPU_ID
    pInfo->CpuID = MxGetCpuID();
    #endif

    #ifdef MX_TRACE_QUEUE_TIME
    pInfo->Time = MxGetTicks();
    #endif

    #ifdef MX_TRACE_QUEUE_FREE_SIZE
    pInfo->FreeNum = pQueue->TrNumFree;
    #endif

    #ifdef MX_TRACE_QUEUE_VAL
    pInfo->Val = (UINT32)data;
    #endif
}

/** @brief This function attaches tracer to the 4GMX object (partition or queue)

    @param nTraceID [in]    - the trace ID (it has to be unique for the 4gmx instance)
    @param hObj     [in]    - the data object handler 
    @param pMem     [in]    - the pointer to the memory, where information will be stored 
    @param nMemSize [in]    - the size of memory buffer in bytes 
    @param oHand    [in]    - the tracer object handle

    \ingroup group_lte_4gmx_tracer

    @return [MXRC] an error code */

MXRC MxTraceAttach (UINT32 nTraceID, HANDLE hObj, void * pMem, UINT32 nMemSize, HANDLE * oHand)
{
    // in this case we need to create new one tracer object 

    HANDLE h;
    PMXCONTEXT pCtx, pObjOSCtx;
    MX_TRACER_CTX * pTracer;
    IRQTYPE irq;
    UINT32 num = 0;
    PPART pPart;
    PQUEUE pQueue;
    DEF_ASSERT_LR();
    DEF_RADDR;

    _ASSERT_LR(pMem != NULL);
    _ASSERT_LR(oHand != NULL);
    _ASSERT_HANDLE_LR(hObj);
    _ASSERT_LR(HANDLE_GET_TYPE(hObj) == OBJ_TYPE_PARTITION || HANDLE_GET_TYPE(hObj) == OBJ_TYPE_QUEUE);

    pCtx = MxInqContext();
    _ASSERT_LR(pCtx != NULL);

    pObjOSCtx = MxInqRemContext(HANDLE_GET_CPU(hObj));
    _ASSERT_LR(pObjOSCtx != NULL);

    MxiDisInt(&irq);

    if ((pTracer = MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_TRACER])) == NULL)
    {
        MxiRestInt(irq);
        return MX_TRACE_RC_CREATE_ERROR;
    }

    Mximemset(pTracer, 0, sizeof (*pTracer));

    if (HANDLE_GET_TYPE(hObj) == OBJ_TYPE_PARTITION)
    {
        _ASSERT (nMemSize >= sizeof (MX_TRACE_PART_INFO));
        num = nMemSize / sizeof (MX_TRACE_PART_INFO);
        pTracer->ObjCtx = (pPart = MxiGetPartDescr (pObjOSCtx, hObj));
        _ASSERT_PTR_LR(pTracer->ObjCtx);

        pPart->TrAlloc = MxiTracePartAlloc;
        pPart->TrFree  = MxiTracePartFree;
        pPart->TrPtr   = pTracer;
    }
    else if (HANDLE_GET_TYPE(hObj) == OBJ_TYPE_QUEUE)
    {
        _ASSERT (nMemSize >= sizeof (MX_TRACE_QUEUE_INFO));
        num = nMemSize / sizeof (MX_TRACE_QUEUE_INFO);
        pTracer->ObjCtx = (pQueue = MxiGetQueueDescr (pObjOSCtx, hObj));
        _ASSERT_PTR_LR(pTracer->ObjCtx);

        pQueue->TrPut = MxiTraceQueuePut;
        pQueue->TrGet = MxiTraceQueueGet;
        pQueue->TrPtr = pTracer;
    }

    pTracer->OwnerID    = MxGetCpuID();
    pTracer->ID         = nTraceID;
    pTracer->Handle     = hObj;
    pTracer->Num        = num;
    pTracer->TracePtr   = (void*)pMem;
    
    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_TRACER,
                ((((UINT32) pTracer) -
                  ((UINT32) pCtx->tracers)) / sizeof(MX_TRACER_CTX)));


    MARK_TR_ALLOC(pTracer);

    *oHand = h;

    MxiRestInt(irq);
    return MX_OK;
}

/** @brief This function detaches the tracer attached to 4GMX object (partition or queue)

    @param hObj [in]    - the data object handler

    \ingroup group_lte_4gmx_tracer

    @return [MXRC] an error code */

MXRC MxTraceDetach(HANDLE hObj)
{
    PMXCONTEXT      pCtx, pObjCtx;
    IRQTYPE         irq;
    PPART           pPart;
    PQUEUE          pQueue;
    MX_TRACER_CTX*  pTracer = NULL;

    DEF_ASSERT_LR();

    _ASSERT(HANDLE_NOT_NULL(hObj));
    _ASSERT(HANDLE_GET_TYPE(hObj) == OBJ_TYPE_PARTITION || HANDLE_GET_TYPE(hObj) == OBJ_TYPE_QUEUE);

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    pObjCtx = MxInqRemContext(HANDLE_GET_CPU(hObj));
    _ASSERT(pObjCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_GET_TYPE(hObj) == OBJ_TYPE_PARTITION)
    {
        pPart = MxiGetPartDescr(pObjCtx, hObj);

        _ASSERT(pPart != NULL);
        _ASSERT(IS_PART_ALLOC(pPart));

        pTracer = (MX_TRACER_CTX*)pPart->TrPtr;
        _ASSERT_PTR_LR(pTracer);

        // if an object is shared, we need to get lock to this object
        // to prevent system crash

        if (IS_PART_SYNC(pPart) || IS_PART_OPENED(pPart))
        {
            MxiSyncCtrlOn(OBJ_TYPE_PARTITION, pPart);
        }

        pPart->TrAlloc = NULL;
        pPart->TrFree  = NULL;
        pPart->TrPtr   = NULL;

        if (IS_PART_SYNC(pPart) || IS_PART_OPENED(pPart))
        {
            MxiSyncCtrlOff(OBJ_TYPE_PARTITION, pPart);
        }
        
    }
    else if (HANDLE_GET_TYPE(hObj) == OBJ_TYPE_QUEUE)
    {
        pQueue = MxiGetQueueDescr(pObjCtx, hObj);

        _ASSERT(pQueue!= NULL);
        _ASSERT(IS_QUEUE_ALLOC(pQueue));

        pTracer = (MX_TRACER_CTX*)pQueue->TrPtr;
        _ASSERT_PTR_LR(pTracer);

        // if an object is shared, we need to get lock to this object
        // to prevent system crash

        if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
        {
            MxiSyncCtrlOn(OBJ_TYPE_QUEUE, pQueue);
        }

        pQueue->TrGet = NULL;
        pQueue->TrPut = NULL;
        pQueue->TrPtr = NULL;

        if (IS_QUEUE_SYNC(pQueue) || IS_QUEUE_OPENED(pQueue))
        {
            MxiSyncCtrlOff(OBJ_TYPE_QUEUE, pQueue);
        }
    }

    pTracer->Handle = HNULL;

    MARK_TR_DEALLOC(pTracer);

    MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_TRACER], pTracer);

    MxiRestInt(irq);
    return MX_OK;
}

/** @brief This function estimates what the size in bytes is needed to keep 
           a trace for specified history (nNum) 

    @param nType[in] - object type, see OBJ_TYPE_xxxx
    @param nNum [in] - the number of elements

    @return [UINT32] the size in bytes */

UINT32 MxTraceCalcStorSize (UINT32 nType, UINT32 nNum)
{
    switch (nType)
    {
        case OBJ_TYPE_PARTITION:
            return sizeof (MX_TRACE_PART_INFO) * nNum;

        case OBJ_TYPE_QUEUE:
            return sizeof (MX_TRACE_QUEUE_INFO) * nNum;

        default:
            return 0;
    }
}

/** @brief This function returns a handle of the tracer object 
           attached to the object specified in the parameter 

    @param hObj [in] - the partition/queue handle

    @return [HANDLE] tracer object or HNULL */

HANDLE MxGetTracerHandle(HANDLE hObj)
{
    PMXCONTEXT      pObjCtx, pCtx;
    IRQTYPE         irq;
    PPART           pPart;
    PQUEUE          pQueue;
    MX_TRACER_CTX*  pTracer = NULL;
    HANDLE h = HNULL;

    DEF_ASSERT_LR();

    _ASSERT_HANDLE(hObj);

    pObjCtx = MxInqRemContext(HANDLE_GET_CPU(hObj));
    _ASSERT(pObjCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_GET_TYPE(hObj) == OBJ_TYPE_PARTITION)
    {
        pPart = MxiGetPartDescr(pObjCtx, hObj);
        _ASSERT_PTR_LR(pPart);
        
        pTracer = (MX_TRACER_CTX*)pPart->TrPtr;
    }
    else if (HANDLE_GET_TYPE(hObj) == OBJ_TYPE_QUEUE)
    {
        pQueue = MxiGetQueueDescr(pObjCtx, hObj);
        _ASSERT_PTR_LR(pQueue);
        
        pTracer = (MX_TRACER_CTX*)pQueue->TrPtr;
    }

    if (pTracer != NULL)
    {
        pCtx = MxInqRemContext(pTracer->OwnerID);
        _ASSERT_PTR_LR(pCtx);

        HANDLE_INIT(h, pTracer->OwnerID, OBJ_TYPE_TRACER,
            ((((UINT32) pTracer) -
              ((UINT32) pCtx->tracers)) / sizeof(MX_TRACER_CTX)));
    }

    MxiRestInt(irq);
    return h;    
}

/** @brief This function scans trace and returns a pointer to the trace element

    @param hTracer  [in] - the trace object handle
    @param pPos     [in/out] - current possition, initially should be (~0) 0xFFFFFFFF

    @return [void *] MX_TRACE_PART_INFO* | MX_TRACE_QUEUE_INFO* */

void * MxTraceGetNext (HANDLE hTracer, PUINT32 pPos) 
{
    PMXCONTEXT      pCtx;
    IRQTYPE         irq;
    MX_TRACER_CTX*  pTracer;
    UINT32 get = 0;
    void * p = NULL;

    DEF_ASSERT_LR();

    _ASSERT_HANDLE(hTracer);
    _ASSERT_HANDLE_TYPE(hTracer, OBJ_TYPE_TRACER);
    _ASSERT_PTR(pPos);

    pCtx = MxInqRemContext(HANDLE_GET_CPU(hTracer));
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pTracer = MxiGetTracerDescr(pCtx, hTracer);
    _ASSERT_PTR(pTracer);

    if (HANDLE_NULL(pTracer->Handle))
    {
        MxiRestInt(irq);
        return NULL;
    }

    // here we need to detect th situation when 
    // we starts from the beginning or just continue 
    // list scanning 
    
    get = (*pPos != ~0) ? *pPos : pTracer->Get;

    // if we reached the end 
    // of the list 
    
    if (get == pTracer->Put)
    {
        MxiRestInt(irq);
        return NULL;
    }

    *pPos = (get + 1);

    if (*pPos >= pTracer->Num)
    {
        *pPos = 0;
    }

    if (HANDLE_GET_TYPE(pTracer->Handle) == OBJ_TYPE_PARTITION)
    {
        p = &((MX_TRACE_PART_INFO*)pTracer->TracePtr)[get];
    }
    else if (HANDLE_GET_TYPE(pTracer->Handle) == OBJ_TYPE_QUEUE)
    {
        p = &((MX_TRACE_QUEUE_INFO*)pTracer->TracePtr)[get];
    }

    MxiRestInt(irq);
    return p;
}

/** @brief This function scans the list of created tracer objects and returns object handle 
           if tracer object is marked with unique ID

    @param pCtx [in] - the pointer to OS context
    @param id   [in] - the tracer ID, see: MxTraceAttach

    @return [HANDLE] handle or HNULL */
    
HANDLE MxTraceFindByID(void *pOSCtx, UINT32 id)
{
    UINT32 i = 0;
    MX_TRACER_CTX * pTracer;
    HANDLE h = HNULL;
    PMXCONTEXT pCtx = (PMXCONTEXT)pOSCtx;

    _ASSERT_PTR(pCtx);

    while (i < MX_MAX_TRACERS)
    {
        pTracer = &pCtx->tracers[i];
        
        if (IS_TR_ALLOC(pTracer) && pTracer->ID == id)
        {
            HANDLE_INIT(h, pTracer->OwnerID, OBJ_TYPE_TRACER,
                            ((((UINT32) pTracer) -
                            ((UINT32) pCtx->tracers)) / sizeof(MX_TRACER_CTX)));

            return h;
        }

        i++;
    }

    return h;
}

#endif // #ifdef MX_TRACER_ENABLED


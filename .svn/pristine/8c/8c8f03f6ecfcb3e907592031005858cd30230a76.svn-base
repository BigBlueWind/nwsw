//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL tracer.h
 *
 * @brief Tracer API definition, it's used to track partitions and queue objects
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "tracer.h"
#include "heaps.h"
#include "initcpu.h"

#ifdef MX_TRACER_ENABLED

/** @brief This function enables trace for the partition or for the queue objects 
           It can be called at any period of time 
           
    @param nTraceID [in] - the tracer ID, see: DataObjID of appids.h
    @param h        [in] - the data object handle (partition or queue)
    @param nDepSize [in] - the history length, if 0 it will be equal to the max number of elements in part/queue
    @param pOut     [out]- the tracer object handle 

    @return [MXRC] an error code */

MXRC SysEnableTrace  (UINT32 nTraceID, HANDLE h, UINT32 nDepSize, HANDLE * pOut)
{
    UINT32 rc;
    UINT32 nNum = 0;
    UINT32 nSize;
    LPVOID pStor;

    DEF_ASSERT_LR();

    _ASSERT_PTR_LR(pOut);

    if (HANDLE_GET_TYPE(h) != OBJ_TYPE_PARTITION && HANDLE_GET_TYPE(h) != OBJ_TYPE_QUEUE)
        return TRACE_RC_OBJ_TYPE_ERROR;

    if (nDepSize != 0)
    {
        nNum = nDepSize;
    }
    else if (HANDLE_GET_TYPE(h) == OBJ_TYPE_PARTITION)
    {
        nNum = MxGetPartBlockCount(h);
    }
    else if (HANDLE_GET_TYPE(h) == OBJ_TYPE_QUEUE)
    {
        nNum = MxGetQueueSize(h);
    }

    nSize = MxTraceCalcStorSize (HANDLE_GET_TYPE(h), nNum);
    _ASSERT(nSize != 0);

    pStor = (LPVOID)MxHeapAlloc (DDR1Heap, nSize);

    _ASSERT_PTR_LR(pStor);
    
    if (pStor == NULL)
        return TRACE_RC_ALLOC_ERROR;

    rc = MxTraceAttach(nTraceID, h, pStor, nSize, pOut);;
    _ASSERT_RC_LR(rc);
    
    return rc;
}

/** @brief This function scans the list of created tracer list and returns 
           the object handle or HNULL

    @param hObj[in] - the object handle (partition/queue), see: SysEnableTrace

    @return [HANDLE] handle or HNULL */

HANDLE SysGetTraceByObj (HANDLE hObj)
{
    return MxGetTracerHandle(hObj);
}

/** @brief This function scans the list of created tracers and returns a handle 
           of tracer object masked with unique ID

    @param nTraceID [in] - the tracer ID, see: SysEnableTrace

    @return [HANDLE] object handle or HNULL
    */
            
HANDLE SysGetTraceByID (UINT32 nTraceID)
{
    UINT32 nCpuNum = SysGetCpuNum();
    UINT32 i = 0;
    HANDLE h;

    PMXCONTEXT pCtx;

    while (i < nCpuNum)
    {
        pCtx = MxInqRemContext(i);
        _ASSERT_PTR(pCtx);

        h = MxTraceFindByID (pCtx, nTraceID);

        if (HANDLE_NOT_NULL(h))
            return h;

        i++;
    }

    return HNULL;
}

/** @brief The function stops tracing the object

    @param hDataObj [in] - the data object handle, see SysEnableTrace(..., h,...)
    @return [MXRC] an error code */

MXRC SysDisableTrace (HANDLE hDataObj)
{
    return MxTraceDetach(hDataObj);
}

/** @brief The function scans the trace and returns a pointer 
           to the control structures 

    @param ID [in] - the trace ID, see: SysEnableTrace
    @param pos[in/out] - the trace possition, at the beginnign it has to be set to (~0)

    @return [void*] NULL or pointer to (MX_TRACE_PART_INFO* or MX_TRACE_QUEUE_INFO*) */
           
void* SysGetNextTraceElm (HANDLE hTracer, PUINT32 pos)
{
    _ASSERT_HANDLE(hTracer);
    _ASSERT_HANDLE_TYPE(hTracer, OBJ_TYPE_TRACER);
    _ASSERT_PTR(pos);

    return MxTraceGetNext (hTracer, pos);
}

#endif // MX_TRACER_ENABLED


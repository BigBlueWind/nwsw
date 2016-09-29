//-------------------------------------------------------------------------------------------
/** @file 4gmx_tracer.h
 *
 * @brief The file contains declaration of 4GMX tracer service designed to track partitions and queue objects
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_TRACER_H_
#define _4GMX_TRACER_H_

#include "4gmx_config.h"
#include "4gmx_types.h"

#ifdef MX_TRACER_ENABLED
#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************/
/*        4GMX Tracer service error codes                               */
/************************************************************************/
#define MX_TRACE_RC_CREATE_ERROR            DEF_MXRC(MXM_TRACER, 1)
#define MX_TRACE_RC_ALLOC_ERROR             DEF_MXRC(MXM_TRACER, 2)


/************************************************************************/
/*    The tracer control flags                                          */
/************************************************************************/
#define TRCF_ALLOCATED                      (1 << 0)

#define MARK_TR_ALLOC(obj)                  ((obj)->CtrlFlags |= TRCF_ALLOCATED)
#define MARK_TR_DEALLOC(obj)                ((obj)->CtrlFlags &= ~TRCF_ALLOCATED)
#define IS_TR_ALLOC(obj)                    ((obj)->CtrlFlags & TRCF_ALLOCATED)

#define MX_TRACE_OP_ID_PART_ALLOC           1
#define MX_TRACE_OP_ID_PART_DEALLOC         2

#define MX_TRACE_OP_ID_QUEUE_PUT            1
#define MX_TRACE_OP_ID_QUEUE_GET            2

/** @brief This function describes tracer information block 
           of partition operation, these blocks will be located 
           in trace storage */

typedef struct _MX_TRACE_PART_INFO_
{
    UINT32      OperationID;                // The operation: 1 - allocation; 2 - deallocation
    UINT32      Index;                      // Automatically incrementable value 

    #ifdef MX_TRACE_PART_LR    
    UINT32      LR;                         // The address of code where function is called 
    #endif

    #ifdef MX_TRACE_PART_CPU_ID    
    UINT32      CpuID;                      // The address of code where function is called 
    #endif

    #ifdef MX_TRACE_PART_TIME
    UINT32      Time;                       // The time of operation: MxGetTicks is used 
    #endif

    #ifdef MX_TRACE_PART_FREE_BLOCKS
    UINT32      FreeNum;                    // The number of free blocks after allocation/deallocation
    #endif

    #ifdef MX_TRACE_PART_PTR
    UINT32      Ptr;                        // The pointer to the allocated/de-allocated block
    #endif

}MX_TRACE_PART_INFO;

/** @brief This function describes tracer information block 
           of partition operation, these blocks will be located 
           in trace storage */

typedef struct _MX_TRACE_QUEUE_INFO_
{
    UINT32      OperationID;                // The operation: 1 - allocation; 2 - deallocation
    UINT32      Index;                      // Automatically incrementable value 

    #ifdef MX_TRACE_QUEUE_LR    
    UINT32      LR;                         // The address of code where function is called 
    #endif

    #ifdef MX_TRACE_QUEUE_CPU_ID    
    UINT32      CpuID;                      // The address of code where function is called 
    #endif

    #ifdef MX_TRACE_QUEUE_TIME
    UINT32      Time;                       // The time of operation: MxGetTicks is used 
    #endif

    #ifdef MX_TRACE_QUEUE_FREE_SIZE
    UINT32      FreeNum;                    // The number of free blocks after allocation/deallocation
    #endif

    #ifdef MX_TRACE_QUEUE_VAL
    UINT32      Val;                        // The queue put/get value
    #endif
}MX_TRACE_QUEUE_INFO;


/** @brief The context of tracer, it contains 
           all needed information to tracer data objects */

typedef struct _MX_TRACER_CTX_
{
    UINT32      ID;                 // Unique tracer object ID
    UINT32      OwnerID;            // The CPU core id where this object is created 
    UINT32      Index;              // Automatically incrementable value 
    void *      TracePtr;           // The pointer to the tracer storage 
    UINT32      Num;                // The maximum number of elements that can be added to the trace storage 
    UINT32      Put;                // The put index to add new one element 
    UINT32      Get;                // The get index to query element 
    HANDLE      Handle;             // The object handle attached to this tracer 
    void *      ObjCtx;             // The object context 
    UINT32      CtrlFlags;

}MX_TRACER_CTX;

/** @brief This function attaches tracer to the 4GMX object (partition or queue)

    @param nTraceID [in]    - the trace ID (it has to be unique for the 4gmx instance)
    @param h        [in]    - the data object handler 
    @param pMem     [in]    - the pointer to the memory, where information will be stored 
    @param nMemSize [in]    - the size of memory buffer in bytes 
    @param oHand    [in]    - the tracer object handle

    \ingroup group_lte_4gmx_tracer

    @return [MXRC] an error code */

MXRC MxTraceAttach (UINT32 nTraceID, HANDLE h, void * pMem, UINT32 nMemSize, HANDLE * oHand);

/** @brief This function detaches the tracer attached to 4GMX object (partition or queue)

    @param h [in]    - the obect handler

    \ingroup group_lte_4gmx_tracer

    @return [MXRC] an error code */

MXRC MxTraceDetach(HANDLE h);

/** @brief This function estimates what the size in bytes is needed to keep 
           trace with specified length 

    @param nType[in] - object type, see OBJ_TYPE_xxxx
    @param nNum [in] - the number of elements

    @return [UINT32] the size in bytes */

UINT32 MxTraceCalcStorSize (UINT32 nType, UINT32 nNum);

/** @brief This function returns handle of the tracer object 
           attached to the object specified in the parameter 

    @param hObj [in] - the partition/queue handle

    @return [HANDLE] tracer object or HNULL */

HANDLE MxGetTracerHandle(HANDLE hObj);

/** @brief This function scans trace and returns a pointer to the trace element

    @param hTracer  [in] - the trace object handle
    @param pPos     [in/out] - current possition, initially should be (~0) 0xFFFFFFFF

    @return [void *] MX_TRACE_PART_INFO* | MX_TRACE_QUEUE_INFO* */

void * MxTraceGetNext (HANDLE hTracer, PUINT32 pPos);

/** @brief This function scans the list of created tracer objects and returns object handle 
           if tracer object is marked with unique ID

    @param pCtx [in] - the pointer to OS context
    @param id   [in] - the tracer ID, see: MxTraceAttach

    @return [HANDLE] handle or HNULL */
    
HANDLE MxTraceFindByID(void *pOSCtx, UINT32 id);

#ifdef __cplusplus
}
#endif
#endif // MX_TRACER_ENABLED
#endif // _4GMX_TRACER_H_

//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL tracer.h
 *
 * @brief Tracer API definition, it's used to track partitions and queue objects
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "config.h"
#include "appids.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _SYS_TRACER_H_
#define _SYS_TRACER_H_

#define TRACE_RC_ALLOC_ERROR                DEF_USRRC(APP_MODULE, TRACER, 1)
#define TRACE_RC_OBJ_TYPE_ERROR             DEF_USRRC(APP_MODULE, TRACER, 2)

#ifdef MX_TRACER_ENABLED

/** @brief This function enables trace for the partition or for the queue objects 
           It can be called at any period of time 
           
    @param nTraceID [in] - the tracer ID, see: DataObjID of appids.h
    @param h        [in] - the data object handle (partition or queue)
    @param nDepSize [in] - the history length, if 0 it will be equal to the max number of elements in part/queue
    @param pOut     [out]- the tracer object handle 

    @return [MXRC] an error code */

MXRC SysEnableTrace  (UINT32 nTraceID, HANDLE h, UINT32 nDepSize, HANDLE * pOut);

/** @brief The function stops tracing the object

    @param hDataObj [in] - the data object handle, see SysEnableTrace(..., h,...)
    @return [MXRC] an error code */

MXRC SysDisableTrace (HANDLE hDataObj);

/** @brief The function scans the trace and returns a pointer 
           to the control structures 

    @param ID [in] - the trace ID, see: SysEnableTrace
    @param pos[in/out] - the trace possition, at the beginnign it has to be set to (~0)

    @return [void*] NULL or pointer to (MX_TRACE_PART_INFO* | MX_TRACE_QUEUE_INFO*) */
           
void* SysGetNextTraceElm (HANDLE hTracer, PUINT32 pos);

/** @brief This function scans the list of created tracer list and returns 
           the object handle or HNULL

    @param hObj[in] - the object handle (partition/queue), see: SysEnableTrace

    @return [HANDLE] handle or HNULL */

HANDLE SysGetTraceByObj (HANDLE hObj);

/** @brief This function scans the list of created tracers and returns a handle 
           of tracer object masked with unique ID

    @param nTraceID [in] - the tracer ID, see: SysEnableTrace

    @return [HANDLE] object handle or HNULL
    */
            
HANDLE SysGetTraceByID (UINT32 nTraceID);

#endif

#endif // _SYS_TRACER_H_

#ifdef __cplusplus
}
#endif



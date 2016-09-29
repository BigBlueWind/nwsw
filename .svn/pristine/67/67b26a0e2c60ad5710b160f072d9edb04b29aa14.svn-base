//-------------------------------------------------------------------------------------------
/** @file 4gmx_thread.c
 *
 * @brief Threads API implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.21 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"
#include "4gmx.h"
#include "4gmx_thread.h"
#include "4gmx_serv.h"
#include "4gmx_debug.h"

#ifdef MX_INTERCPU_ENABLED
#include "4gmx_intercpu.h"
#endif

#ifdef MX_MTHREAD_ENABLED
#include "4gmx_mthread.h"
#endif


#if  defined(TRACE_THR_CREATE) || defined (TRACE_THR_REMOVE) || defined (TRACE_THR_SCHEDULE) || defined (TRACE_THR_UNSCHEDULE)

/** @brief  This function is called by the thread API for tracing
            thread events.

    @param pCtx    [in] - the system context
    @param id      [in] - the trace event ID
    @param hHandle [in] - the thread handle (or HNULL)
    @param rc      [in] - error code of operation

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_thread
*/
MXRC AddThreadTrace (PMXCONTEXT pCtx, TraceEventID id, HANDLE hHandle, MXRC rc)
{
    TraceDataThread trace;

    trace.hThr = hHandle;
    trace.Rc   = rc;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif /*TRACE_THR_CREATE*/

#ifdef TRACE_THR_PRIOR

MXRC AddThreadPriorTrace (PMXCONTEXT pCtx, TraceEventID id,
                          HANDLE hHandle, MXRC rc, TPRIORITY pr)
{
    TraceDataThreadPrior trace;

    trace.Rc   = rc;
    trace.hThr = hHandle;
    trace.Prior= pr;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif /*TRACE_THR_PRIOR*/

#ifdef MX_THREAD_MSG_ENABLED

/** @brief This function is designed to initialize thead message mechanism

    @param pThread [in] - the pointer to the thread context

    \ingroup group_lte_4gmx_thread

    */

void MxThreadInitMsg (PTHREAD pThread)
{
    _ASSERT_PTR(pThread);

    pThread->MsgSync  = 0;
    pThread->MsgFirst = NULL;
    pThread->MsgLast  = NULL;
    MxiDefSysPart (&pThread->MsgPart, (LPVOID)(&pThread->MsgStor[0]), sizeof (pThread->MsgStor[0]), MX_MAX_THREAD_MSG);
}

#endif // MX_THREAD_MSG_ENABLED

/** @brief The function creates thread context and returns thread handle to an application

    @param proc [in] - the pointer to the thread binary code
    @param name [in] - an object's name, it can be null
    @param param[in] - the thread parameter
    @param prior[in] - the thread priority
    @param outHandle[out]- the handle of created object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/
MXRC MxCreateThread(TENTRY proc, const char *name, LPVOID param,
                    TPRIORITY priority, HANDLE * outHandle)
{
    PMXCONTEXT pCtx;
    PTHREAD pThread;
    HANDLE h = HNULL;
    IRQTYPE irq;
    DEF_ASSERT_LR();
    DEF_RADDR;
    
    _ASSERT(proc != NULL);
    _ASSERT(outHandle != NULL);

    pCtx = MxInqContext();

    _ASSERT_LR(pCtx != NULL);

    MxiDisInt(&irq);

    pThread = (PTHREAD) MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_THREAD]);

    if (pThread == NULL)
    {
#ifdef TRACE_THR_CREATE
        AddThreadTrace (pCtx, TEID_CREATE_THR, HNULL, MX_ALLOC_THREAD_ERROR);
#endif
        MxiRestInt(irq);
        return MX_ALLOC_THREAD_ERROR;
    }

    memset (pThread, 0, sizeof (*pThread));

    pThread->CtrlFlags = 0;
    pThread->Proc = proc;
    pThread->Priority = priority;
    pThread->ProcParam = param;
    pThread->ReturnVal = 0;
    pThread->NextScheduled = NULL;
    pThread->nOwnerCpuID  = MxGetCpuID();

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_THREAD,
                ((((UINT32) pThread) -
                  ((UINT32) pCtx->threads)) / sizeof(THREAD)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_THREAD], pThread);

#ifdef TRACE_THR_CREATE
            AddThreadTrace (pCtx, TEID_CREATE_THR, HNULL,
                              MX_ASSIGN_THREAD_NAME_ERROR);
#endif
            MxiRestInt(irq);
            return MX_ASSIGN_THREAD_NAME_ERROR;
        }

        MARK_THR_NAMED(pThread);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_THR_UNNAMED(pThread);
    }

#ifdef MX_THREAD_MSG_ENABLED
    MxThreadInitMsg (pThread);
#endif

    MARK_THR_ALLOC(pThread);

    *outHandle = h;

#ifdef TRACE_THR_CREATE
    AddThreadTrace (pCtx, TEID_CREATE_THR, h, MX_OK);
#endif

    MxiRestInt(irq);
    return MX_OK;
}

#ifdef MX_INTERCPU_ENABLED

/** @brief The function creates thread and returns thread's handle
           to the application, the function allows to specify the target CPU.

    @param cpu  [in] - the thread target CPU
    @param proc [in] - the pointer to the thread binary code
    @param name [in] - object's name, it can be null
    @param param[in] - the thread parameter
    @param prior[in] - the thread priority
    @param outHandle[out]- the handle of created object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/
MXRC MxCreateThreadEx(CPUID cpu, TENTRY proc, LPCSTR name, LPVOID param,
                    UINT8 priority, HANDLE * outHandle)

{
    MxICThrCreate ic;

    if (cpu == MxGetCpuID() || cpu == CUR_CPU)
        return MxCreateThread(proc, NULL, param, priority, outHandle);

    ic.Proc   = proc;
    ic.Param  = param;
    ic.Prior  = priority;
    ic.Name   = name;
    ic.Handle = outHandle;

    return MxICExec(cpu, MXIC_THREAD_CREATE, &ic, sizeof (ic));
}

#endif /*MX_INTERCPU_ENABLED*/


/** @brief The function removes a thread and unshedules it(if it was scheduled).
   If object was associated with ASCIIZ name, the linkage is dropped.

    @param h [in] - the thread descriptor

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/
MXRC MxRemoveThread(HANDLE h)
{
    PMXCONTEXT pCtx;
    PTHREAD pThr;
    IRQTYPE irq;

#ifdef MX_INTERCPU_ENABLED
    MxICThrRemove ic;
#endif
    
    DEF_ASSERT_LR();

    _ASSERT_LR(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD || HANDLE_GET_TYPE(h) == OBJ_TYPE_MTHREAD);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        return MxICExec (HANDLE_GET_CPU(h), MXIC_THREAD_REMOVE,
                          &ic, sizeof (ic));
    }

#else

    /* Only locally created thread can be removed */

    _ASSERT_LR(HANDLE_GET_CPU(h) == MxGetCpuID());

#endif

#ifdef MX_MTHREAD_ENABLED
    if (HANDLE_GET_TYPE(h) == OBJ_TYPE_MTHREAD)
        return MxMRemoveThread (h);
#endif

    pCtx = MxInqContext();

    _ASSERT_LR(pCtx != NULL);

    pThr = MxiGetThreadDescr(pCtx, h);

    _ASSERT_LR(pThr != NULL);
    _ASSERT_LR(IS_THR_ALLOC(pThr));

    MxiDisInt(&irq);

    if (IS_SCHEDULED(pThr))
    {
        MxiCutThreadFromSchList(pCtx, pThr);
    }

#ifdef MX_NAMING_ENABLED

    if (IS_THR_NAMED(pThr))
    {
        MxiFreeName(pCtx, h);

        MARK_THR_UNNAMED(pThr);
    }

#endif /*MX_NAMING_ENABLED */

    MARK_THR_DEALLOC(pThr);

    MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_THREAD], pThr);

#ifdef TRACE_THR_REMOVE
    AddThreadTrace (pCtx, TEID_REMOVE_THR, h, MX_OK);
#endif

    MxiRestInt(irq);
    return MX_OK;
}

/** @brief The function sets/changes thread priority. If thread is already scheduled, the
   thread will be rescheduled according to the new priority.

    @param h [in]   - the thread descriptor
    @param priority [in] - the new thread priority

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/
MXRC MxSetThreadPriority(HANDLE h, TPRIORITY priority)
{
    PMXCONTEXT pCtx;
    PTHREAD pThread, pPos;
    IRQTYPE irq;

#ifdef MX_INTERCPU_ENABLED
    MxICThrSetPrior ic;
#endif

    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        ic.Prior  = priority;

        return MxICExec(HANDLE_GET_CPU(h), MXIC_THREAD_SETPRIOR,
                         &ic, sizeof (ic));
    }

#else

    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    pThread = (PTHREAD) MxiGetThreadDescr(pCtx, h);

    if (pThread == NULL)
    {
#ifdef TRACE_THR_REMOVE
        AddThreadPriorTrace (pCtx, TEID_SET_THR_PRIOR,
                             h, MX_HANDLE_ERROR, priority);
#endif
        return MX_HANDLE_ERROR;
    }

    _ASSERT(IS_THR_ALLOC(pThread));

    MxiDisInt(&irq);

    pThread->Priority = priority;

    /* thread rescheduling */

    if (IS_SCHEDULED(pThread))
    {
        MxiCutThreadFromSchList(pCtx, pThread);

        /* to find the place for scheduling according to the thread priority */

        pPos = MxiFindSchPos(pCtx, pThread);

        MxiAddThreadToSchList(pCtx, pThread, pPos);
    }

    MxiRestInt(irq);

#ifdef TRACE_THR_PRIOR
    AddThreadPriorTrace (pCtx, TEID_SET_THR_PRIOR, h, MX_OK, priority);
#endif

    return MX_OK;
}

/** @brief The function returns thread priority.

    @param h [in]   - the thread descriptor
    @param priority [out] - the thread priority

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/
MXRC MxGetThreadPriority(HANDLE h, TPRIORITY * priority)
{
    PMXCONTEXT pCtx;
    PTHREAD pThread;

#ifdef MX_INTERCPU_ENABLED
    MxICThrGetPrior ic;
#endif

    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD);
    _ASSERT(priority != NULL);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        ic.Prior  = priority;

        return MxICExec(HANDLE_GET_CPU(h), MXIC_THREAD_GETPRIOR,
                         &ic, sizeof (ic));
    }

#else

    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    pThread = (PTHREAD) MxiGetThreadDescr(pCtx, h);

    if (pThread == NULL)
        return MX_HANDLE_ERROR;

    _ASSERT(IS_THR_ALLOC(pThread));

    *priority = pThread->Priority;

    return MX_OK;
}

/** @brief The function sets/changes thread parameter.

    @param h [in]  - the thread descriptor
    @param param [in] - the new thread parameter

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/
MXRC MxSetThreadParam(HANDLE h, LPVOID param)
{
    PMXCONTEXT pCtx;
    PTHREAD pThread;

#ifdef MX_INTERCPU_ENABLED
    MxICThrSetParam ic;
#endif

    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        ic.Param  = param;

        return MxICExec(HANDLE_GET_CPU(h), MXIC_THREAD_SETPARAM,
                         &ic, sizeof (ic));
    }

#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    pThread = (PTHREAD) MxiGetThreadDescr(pCtx, h);

    if (pThread == NULL)
        return MX_HANDLE_ERROR;

    _ASSERT(IS_THR_ALLOC(pThread));

    pThread->ProcParam = param;

    return MX_OK;
}

/** @brief The function returns thread parameter

    @param h [in]  - the thread descriptor
    @param param [in] - the new thread parameter

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/
MXRC MxGetThreadParam(HANDLE h, LPVOID * param)
{
    PMXCONTEXT pCtx;
    PTHREAD pThread;

#ifdef MX_INTERCPU_ENABLED
    MxICThrGetParam ic;
#endif

    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD);
    _ASSERT(param != NULL);

#ifdef MX_INTERCPU_ENABLED
    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        ic.Param  = param;

        return MxICExec(HANDLE_GET_CPU(h), MXIC_THREAD_GETPARAM,
                         &ic, sizeof (ic));
    }
#else

    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    pThread = (PTHREAD) MxiGetThreadDescr(pCtx, h);

    if (pThread == NULL)
        return MX_HANDLE_ERROR;

    _ASSERT(IS_THR_ALLOC(pThread));

    *param = pThread->ProcParam;

    return MX_OK;
}

/** @brief The function schedules thread for execution, if thread was already scheduled,
    the function do nothing.

    @param h [in] - the thread handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/
MXRC MxScheduleThread(HANDLE h)
{
    PMXCONTEXT pCtx;
    PTHREAD pThr, pPos;
    MXRC rc = MX_OK;

    IRQTYPE irq;

#ifdef MX_INTERCPU_ENABLED
    MxICThrSchedule ic;
#endif

    DEF_ASSERT_LR();

    _ASSERT_LR(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD || HANDLE_GET_TYPE(h) == OBJ_TYPE_MTHREAD);
    _ASSERT_HANDLE_LR(h);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        return MxICExec(HANDLE_GET_CPU(h), MXIC_THREAD_SCHEDULE,
                         &ic, sizeof (ic));
    }

#else

    _ASSERT_LR(HANDLE_GET_CPU(h) == MxGetCpuID());

#endif

#ifdef MX_MTHREAD_ENABLED
    if (HANDLE_GET_TYPE(h) == OBJ_TYPE_MTHREAD)
    {
        return MxScheduleMThread (h);
    }
#endif

    pCtx = MxInqContext();

    _ASSERT_LR(pCtx != NULL);

    pThr = MxiGetThreadDescr(pCtx, h);

    _ASSERT_LR(pThr != NULL);
    _ASSERT_LR(IS_THR_ALLOC(pThr));

    MxiDisInt(&irq);

    if (IS_SCHEDULED(pThr))
    {
        MxiRestInt(irq);
        return MX_OK;
    }

    // to prevent scheduling of the thread if this thread is under 
    // SMP processing on other core

    if (IS_UNDER_SMP_PROC(pThr) && (pThr->nSMPExecCpuID != MxGetCpuID ()))
    {
        MxiRestInt(irq);
        rc = MX_THREAD_IS_UNDER_SMP_PROC;
        _ASSERT_RC_LR(rc);
        return rc;
    }

    // if this thread was scheduled like SMP, we need to prevent scheduling of
    // this tread like regular one
    
    if (IS_TO_BE_SMP_SCHED(pThr))
    {
        MxiRestInt(irq);
        //rc = MX_THREAD_IS_UNDER_SMP_PROC;
        //_ASSERT_RC(rc);
        return rc;
    }

    pPos = MxiFindSchPos(pCtx, pThr);

    MxiAddThreadToSchList(pCtx, pThr, pPos);

#ifdef TRACE_THR_SCHEDULE
    AddThreadTrace (pCtx, TEID_SCHEDULE_THR, h, MX_OK);
#endif

#ifdef MX_MTHREAD_ENABLED
    MxScheduleSSThread ();
#endif

    MxiRestInt(irq);
    return MX_OK;
}

/** @brief The function SMP schedules thread for execution, if thread was already scheduled,
    the function do nothing.

    @param h        [in] - the thread handle
    @param PrCond   [in] - the new thread priority (if SCHED_USE_PRIORITY is used)
                         - the thread schedule threshold (if SCHED_BY_CONDITION is used)
    @param SchedCtrl[in] - the control mask, see: TSCHEDCTRL type

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/

MXRC MxSchedSmpThread (HANDLE h, UINT32 PrCond, UINT32 SchedCtrl)
{
    PMXCONTEXT pCtx;
    PTHREAD pThr;
    UINT32 nThrCpuID;

    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD);
    _ASSERT_HANDLE(h);

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    if ((pCtx->ICSMPCtx) == NULL)
    {
        _ASSERT_PTR(pCtx->ICSMPCtx);
        return MX_THREAD_SMP_IS_NOT_DEF;
    }

    nThrCpuID = HANDLE_GET_CPU(h);

    if (nThrCpuID != MxGetCpuID ())
    {
        pCtx = MxInqRemContext(nThrCpuID);
    }

    _ASSERT(pCtx != NULL);

    pThr = (PTHREAD) MxiGetThreadDescr(pCtx, h);

    _ASSERT_PTR(pThr);

    return MxSchedSmpThreadEx(pThr, PrCond, SchedCtrl);
}

/** @brief The function SMP schedules thread for execution, if thread was already scheduled,
    the function do nothing.

    @param pThr     [in] - the thread descriptor
    @param PrCond   [in] - the new thread priority (if SCHED_USE_PRIORITY is used)
                         - the thread schedule threshold (if SCHED_BY_CONDITION is used)
    @param SchedCtrl[in] - the control mask, see: TSCHEDCTRL type

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/

MXRC MxSchedSmpThreadEx (PTHREAD pThr, UINT32 PrCond, UINT32 SchedCtrl)
{
    PMXCONTEXT pCtx;
    PTHREAD pThrPos;
    IRQTYPE irq;
    MXSMPCTX * pSmpCtx;

    _ASSERT_PTR(pThr);

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    if ((pSmpCtx = pCtx->ICSMPCtx) == NULL)
    {
        _ASSERT_PTR(pCtx->ICSMPCtx);
        return MX_THREAD_SMP_IS_NOT_DEF;
    }

    MxiDisInt(&irq);
    MxGetLock (&pSmpCtx->Lock);

    // if conditional scheduling is used 
    // we need to check threshold value 
    // LOCK is needed here to be in sync with other processors 
    // because this thread cannot be a part of this 4GMX instance 
    
    if ((SchedCtrl & SCHED_BY_CONDITION) != 0)
    {
        // in this case priority parameter cannot be used 
        SchedCtrl &= ~SCHED_USE_PRIORITY;

        pThr->SchedCondVal ++;

        if (pThr->SchedCondVal < PrCond)
        {
            MxReleaseLock (&pSmpCtx->Lock);
            MxiRestInt(irq);
            //return MX_THREAD_LIM_IS_NOT_EXCEEDED;
            return MX_OK;
        }

        pThr->SchedCondVal = 0;
    }

    // if this is kernel call and we need to clean SMP scheduled bit
    
    if (SchedCtrl & SCHED_R_SMP_PROC)
    {
        CLEAR_UNDER_SMP_PROC(pThr);
    }

    // if Thread is already under processing, 
    // we cannot schedule this thread again
    // to prevent start of this thread on 
    // other processor, so we mark this thread 
    // to be scheduled and core that executes this thread 
    // will reschedule thread again when thread is completed

    if (IS_UNDER_SMP_PROC(pThr))
    {
        // this parameters will be used later, 
        // when thread will be rescheduled by kernel
    
        pThr->nSMPCtrlFlags = SchedCtrl;
        pThr->nSMPPrior     = PrCond;
    
        MARK_TO_BE_SMP_SCHED(pThr);
    }
    // if the thread is not scheduled, just add
    // this one thread to the list of SMP scheduled threads
    else if (!IS_SCHEDULED(pThr))
    {
        CLEAR_TO_BE_SMP_SCHED(pThr);
        
        pThrPos = MxiFindSmpSchPos(pSmpCtx, (SchedCtrl & SCHED_USE_PRIORITY) ? PrCond : pThr->Priority, (SchedCtrl & SCHED_AT_BEGIN));
        MxiAddThreadToSmpSchList(pSmpCtx, pThr, pThrPos);

        #ifdef TRACE_THR_SCHEDULE
        AddThreadTrace (pCtx, TEID_SCHEDULE_THR, h, MX_OK);
        #endif

        pSmpCtx->SchedNum++;
    }

    MxReleaseLock (&pSmpCtx->Lock);
    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function schedules thread for execution, if thread was already scheduled,
    the function do nothing.

    @param h        [in] - the thread handle
    @param PrCond   [in] - the new thread priority (if SCHED_USE_PRIORITY is used)
                         - the conditional threshold (if SCHED_BY_CONDITION is used)
    @param SchedCtrl[in] - the control mask, see: TSCHEDCTRL type

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/
MXRC MxScheduleThreadEx(HANDLE h, UINT32 PrCond, UINT32 SchedCtrl)
{
    PMXCONTEXT pCtx;
    PTHREAD pThr, pPos;
    MXRC rc = MX_OK;

    IRQTYPE irq;

#ifdef MX_INTERCPU_ENABLED
    MxICThrScheduleEx ic;
#endif

    DEF_ASSERT_LR();

    _ASSERT_LR(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD);
    _ASSERT_HANDLE_LR(h);

    if (SchedCtrl & SCHED_SMP)
    {
        return MxSchedSmpThread (h, PrCond, SchedCtrl);
    }

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle    = h;
        ic.Prior     = PrCond;
        ic.SchedCtrl = SchedCtrl;
        return MxICExec(HANDLE_GET_CPU(h), MXIC_THREAD_SCHEDULE_EX,
                         &ic, sizeof (ic));
    }

#else

    _ASSERT_LR(HANDLE_GET_CPU(h) == MxGetCpuID());

#endif

    pCtx = MxInqContext();

    _ASSERT_LR(pCtx != NULL);

    pThr = MxiGetThreadDescr(pCtx, h);

    _ASSERT_LR(pThr != NULL);
    _ASSERT_LR(IS_THR_ALLOC(pThr));

    MxiDisInt(&irq);

    // if conditional scheduling is used 
    if ((SchedCtrl & SCHED_BY_CONDITION) != 0)
    {
        // in this case priority parameter cannot be used 
        SchedCtrl &= ~SCHED_USE_PRIORITY;

        pThr->SchedCondVal ++;

        if (pThr->SchedCondVal < PrCond)
        {
            MxiRestInt(irq);
            //return MX_THREAD_LIM_IS_NOT_EXCEEDED;
            return MX_OK;
        }

        pThr->SchedCondVal = 0;
    }

    // to prevent scheduling of the thread if this thread was scheduled like SMP
    // thread and is ready to be executed 

    if (IS_TO_BE_SMP_SCHED(pThr))
    {
        MxiRestInt(irq);
        //rc = MX_THREAD_SMP_SCHED_DETECTED;
        //_ASSERT_RC(rc);
        return rc;
    }

    // if this thread is under SMP processing on other core, 
    // we cannot schedule this thread on owner core, to prevent 
    // running of the thread from two places at the same time

    if (IS_UNDER_SMP_PROC(pThr) && (pThr->nSMPExecCpuID != MxGetCpuID()))
    {
        MxiRestInt(irq);
        rc = MX_THREAD_IS_UNDER_SMP_PROC;
        _ASSERT_RC_LR(rc);
        return rc;
    }

    if (IS_SCHEDULED(pThr))
    {
        MxiRestInt(irq);
        return MX_OK;
    }

    pPos = MxiFindSchPosEx(pCtx, (SchedCtrl & SCHED_USE_PRIORITY) ? (PrCond) : (pThr->Priority), (SchedCtrl & SCHED_AT_BEGIN));

    MxiAddThreadToSchList(pCtx, pThr, pPos);

#ifdef TRACE_THR_SCHEDULE
    AddThreadTrace (pCtx, TEID_SCHEDULE_THR, h, MX_OK);
#endif

    MxiRestInt(irq);

    return MX_OK;
}


/** @brief This function "injects" thread code to the stream of remote CPU

    @param cpu[in] - the CPU id, it cannot be equal to the current CPU id
    @param h  [in] - the thread handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/
MXRC MxInjectThread(CPUID id, HANDLE h)
{
#ifdef MX_INTERCPU_ENABLED

    MxICThrInjectHandle ic_handle;
    MxICThrInjectProc   ic_proc;

    PMXCONTEXT pCtx;
    PTHREAD pThr;

    _ASSERT_HANDLE(h);
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD);

    // In this case we need to specify proc entry only
    // the handle belongs to this OS and thread context
    // can be taken and processed

    if (HANDLE_GET_CPU(h) == MxGetCpuID ())
    {
        pCtx = MxInqContext();

        _ASSERT(pCtx != NULL);

        pThr = MxiGetThreadDescr(pCtx, h);

        _ASSERT(pThr != NULL);
        _ASSERT(IS_THR_ALLOC(pThr));

        ic_proc.Entry = pThr->Proc;
        ic_proc.Param = pThr->ProcParam;

        return MxICExec(id, MXIC_THREAD_INJECT_PROC,
                         &ic_proc, sizeof (ic_proc));
    }

    // In this case the thread handle belogs to the
    // remote OS and handle will be processed on remote CPU
    // because we have not access to the remote system context

    else if (HANDLE_GET_CPU(h) == id)
    {
        ic_handle.Handle = h;

        return MxICExec(id, MXIC_THREAD_INJECT_HANDLE,
                         &ic_handle, sizeof (ic_handle));
    }

    // Only locally created or remote thread
    // can be injected to the remote system

    return MX_INJECT_CPU_ERROR;

#else
    return MX_INTERCPU_DISABLED;
#endif
}

/** @brief This function "injects" application code to the stream of remote CPU
            in IRQ mode,  so, the application should take care about time of code execution

    @param cpu     [in] - the CPU id, it cannot be equal to the current CPU id
    @param entry   [in] - the entry point of application code
    @param pParam  [in] - application code parameter (it can be NULL)

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/
MXRC MxInjectCode(CPUID id, TENTRY entry, LPVOID pParam)
{
#ifdef MX_INTERCPU_ENABLED

    MxICThrInjectProc   ic_proc;

    ic_proc.Entry = entry;
    ic_proc.Param = pParam;

    return MxICExec(id, MXIC_THREAD_INJECT_PROC,
                     &ic_proc, sizeof (ic_proc));

#else
    return MX_INTERCPU_DISABLED;
#endif
}



/** @brief The function unschedules thread if thread was scheduled or
    do nothing in other case.

    @param h [in] - the thread handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/
MXRC MxUnscheduleThread(HANDLE h)
{
    PMXCONTEXT pCtx;
    PTHREAD pThr;

    IRQTYPE irq;

#ifdef MX_INTERCPU_ENABLED
    MxICThrSchedule ic;
#endif

    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD || HANDLE_GET_TYPE(h) == OBJ_TYPE_MTHREAD);

#ifdef MX_INTERCPU_ENABLED
    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;

        return MxICExec (HANDLE_GET_CPU(h), MXIC_THREAD_UNSCHEDULE,
                          &ic, sizeof (ic));
    }
#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

#ifdef MX_MTHREAD_ENABLED
    if (HANDLE_GET_TYPE(h) == OBJ_TYPE_MTHREAD)
    {
        return MxUnScheduleMThread (h);
    }
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    pThr = MxiGetThreadDescr(pCtx, h);

    _ASSERT(pThr != NULL);
    _ASSERT(IS_THR_ALLOC(pThr));

    MxiDisInt(&irq);

    if (!IS_SCHEDULED(pThr))
    {
        MxiRestInt(irq);
        return MX_OK;
    }

    MxiCutThreadFromSchList(pCtx, pThr);

#ifdef TRACE_THR_SCHEDULE
    AddThreadTrace (pCtx, TEID_UNSCHEDULE_THR, h, MX_OK);
#endif

    MxiRestInt(irq);

    return MX_OK;
}

#ifndef MX_EVENT_ENABLED

INLINE MXRC MxScheduleThreadByEvent(HANDLE hThr, HANDLE hEvn, UINT32 nMode)
{
    return MX_EVENT_API_DISABLED;
}

INLINE MXRC MxUnscheduleThreadByEvent(HANDLE hThr, HANDLE hEvn)
{
    return MX_EVENT_API_DISABLED;
}

#else /*#ifndef MX_EVENT_ENABLED */

MXRC MxScheduleThreadByEvent(HANDLE hThr, HANDLE hEvn, UINT32 nMode)
{
    PMXCONTEXT pCtx;

    IRQTYPE irq;

    PEVENT pEvent;

#ifdef _MX_DEBUG
    PTHREAD pThread;
#endif

#ifdef MX_INTERCPU_ENABLED
    MxICThrSchByEvent ic;
#endif

#ifdef MX_MTHREAD_ENABLED
    _ASSERT(HANDLE_GET_TYPE(hThr) == OBJ_TYPE_THREAD || HANDLE_GET_TYPE(hThr) == OBJ_TYPE_MTHREAD);
#else
    _ASSERT(HANDLE_GET_TYPE(hThr) == OBJ_TYPE_THREAD);
#endif
    _ASSERT(HANDLE_GET_TYPE(hEvn) == OBJ_TYPE_EVENT);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(hEvn) != MxGetCpuID())
    {
        ic.Mode       = nMode;
        ic.ThrHandle  = hThr;
        ic.EventHandle= hEvn;

        return MxICExec (HANDLE_GET_CPU(hEvn), MXIC_THREAD_SCH_BY_EVENT,
                          &ic, sizeof (ic));
    }

#else

    _ASSERT(HANDLE_GET_CPU(hThr) == MxGetCpuID());
    _ASSERT(HANDLE_GET_CPU(hEvn) == MxGetCpuID());

#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pEvent = MxiGetEventDescr(pCtx, hEvn);

    _ASSERT(pEvent != NULL);

#ifdef _MX_DEBUG
    pThread = (PTHREAD) MxiGetThreadDescr(pCtx, hThr);

    _ASSERT(pThread != NULL);
#endif

    pEvent->thread = hThr;

    MARK_EVN_LINKED(pEvent);

    if (nMode & THREAD_EVENT_AUTO_RESET)
    {
        MARK_EVN_AUTORESET(pEvent);
    }
    else
    {
        MARK_EVN_NOTAUTORESET(pEvent);
    }

    if (nMode & THREAD_EVENT_SMP)
    {
        MARK_EVN_SCHED_SMP(pEvent);
    }
    else
    {
        MARK_EVN_NO_SCHED_SMP(pEvent);
    }

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function drops a connection between the thread and the event objects

    @param hThr [in] - the thread handle
    @param hEvn [in] - the event handle

    @return [MXRC]

    \ingroup group_lte_4gmx_thread
*/
MXRC MxUnscheduleThreadByEvent(HANDLE hThr, HANDLE hEvn)
{
    PMXCONTEXT pCtx;

    IRQTYPE irq;

    PEVENT pEvent;

#ifdef MX_INTERCPU_ENABLED
    MxICThrSchByEvent ic;
#endif

    _ASSERT(HANDLE_GET_TYPE(hThr) == OBJ_TYPE_THREAD);

    _ASSERT(HANDLE_GET_TYPE(hEvn) == OBJ_TYPE_EVENT);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(hEvn) != MxGetCpuID())
    {
        ic.ThrHandle   = hThr;
        ic.EventHandle = hEvn;
        ic.Mode        = 0;

        return MxICExec(HANDLE_GET_CPU(hEvn), MXIC_THREAD_UNSCH_BY_EVENT,
                         &ic, sizeof (ic));
    }

#else
    _ASSERT(HANDLE_GET_CPU(hThr) == MxGetCpuID());
    _ASSERT(HANDLE_GET_CPU(hEvn) == MxGetCpuID());

#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pEvent = (PEVENT) MxiGetEventDescr(pCtx, hEvn);
    _ASSERT(pEvent != NULL);

#ifdef _MX_DEBUG
    _ASSERT(MxiGetThreadDescr(pCtx, hThr) != NULL);
#endif

    pEvent->thread = HNULL;

    MARK_EVN_UNLINKED(pEvent);

    MxiRestInt(irq);

    return MX_OK;
}

#endif /*MX_EVENT_ENABLED */

/** @brief The function returns previous thread's return value or 0 if
    thread was not executed.

    @param h [in]  - the thread descriptor

    @return [UINT32] return value

    \ingroup group_lte_4gmx_thread
*/
UINT32 MxGetThreadRetVal(HANDLE h)
{
    PMXCONTEXT pCtx;
    PTHREAD pThread;

#ifdef MX_INTERCPU_ENABLED
    MxICThrRet ic;
    UINT32 ret = 0;
    MXRC rc;
#endif
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        ic.Ret    = &ret;

        rc = MxICExec(HANDLE_GET_CPU(h), MXIC_THREAD_GETRET,
                       &ic, sizeof (ic));

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);

            return 0;
        }

        return ret;
    }

#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    pThread = (PTHREAD) MxiGetThreadDescr(pCtx, h);

    if (pThread == NULL)
        return 0;

    _ASSERT(IS_THR_ALLOC(pThread));

    return pThread->ReturnVal;
}


/** @brief This function is designed to run thread code
           described with thread handle

    @param h        [in] - the thread handle
    @param pRetVal  [out]- optional, thread return value

    @return [MXRC] an error code

    \ingroup group_lte_4gmx_thread
*/
MXRC MxRunThread (HANDLE h, PUINT32 pRetVal)
{
    PMXCONTEXT pCtx;
    PTHREAD pThread;
    UINT32 ret;

    _ASSERT_HANDLE (h);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
        return MX_THREAD_CPUID_ERROR;

    pCtx = MxInqContext ();

    _ASSERT_PTR (pCtx);

    if ((pThread = (PTHREAD) MxiGetThreadDescr(pCtx, h)) == NULL)
        return MX_THREAD_HANDLE_ERROR;

    _ASSERT(IS_THR_ALLOC(pThread));

    ret = pThread->Proc (pThread->ProcParam);

    if (pRetVal != NULL)
        *pRetVal = ret;

    return MX_OK;
}

/** @brief The function return status of the specified thread:

             TRUE - the thread was scheduled and is waiting to be processed
             FALSE- the thread was not scheduled or under processing that means it's not more scheduled

    @return [BOOL] TRUE/FALSE

    \ingroup group_lte_4gmx_thread
*/
BOOL MxIsThreadScheduled (HANDLE h)
{
    PMXCONTEXT pCtx;
    PTHREAD pThread;

    _ASSERT_HANDLE (h);
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD);

    if (HANDLE_GET_CPU(h) == MxGetCpuID())
    {
        pCtx = MxInqContext ();
    }
    else
    {
        pCtx = MxInqRemContext(HANDLE_GET_CPU(h));
    }

    _ASSERT_PTR (pCtx);

    if ((pThread = (PTHREAD) MxiGetThreadDescr(pCtx, h)) == NULL)
    {
        _ASSERT_PTR (pThread);
        return FALSE;
    }

    return IS_SCHEDULED(pThread);
}

#ifdef MX_THREAD_MSG_ENABLED

/** @brief This function is designed to send a message (to put a message to the thread message queue)
           to the specified by the handle thread. This function can be used for all threads
           (locally and remotely created).

    @NOTE The thread is automatically scheduled by the system

    The function parameters are:

    @param  h      [in] - the thread handle
    @param pMsgPtr [in] - the pointer to the message (it will be delivered to the thread)

    @return [MXRC] an error code

    \ingroup group_lte_4gmx_thread

*/
MXRC MxThreadSendMsg (HANDLE h, LPVOID pMsgPtr)
{
    PMXCONTEXT pCtx;
    PTHREAD pThread;
    IRQTYPE irq;
    THREADMSG *pMsg;
    MXRC rc = MX_THREAD_MSG_OVERFLOW;
    UINT32 bSchedThread = 0;

    _ASSERT_HANDLE (h);
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD);

    pCtx = MxInqRemContext (HANDLE_GET_CPU(h));

    _ASSERT_PTR (pCtx);

    if ((pThread = (PTHREAD) MxiGetThreadDescr(pCtx, h)) == NULL)
        return MX_THREAD_HANDLE_ERROR;

    _ASSERT(IS_THR_ALLOC(pThread));

    MxiDisInt(&irq);
    MxGetLock (&pThread->MsgSync);

    if ((pMsg = (THREADMSG*)MxiAllocSysPart (&pThread->MsgPart)) != NULL)
    {
        rc = MX_OK;

        pMsg->Prior = (UINT32)(-1); // the lowest message priority that is putted to the end of list
        pMsg->Data  = pMsgPtr;
        pMsg->Next  = NULL;

        if (pThread->MsgFirst == NULL)
        {
            pThread->MsgFirst = pMsg;
            pThread->MsgLast  = pMsg;

            bSchedThread = 1;
        }
        else
        {
            pThread->MsgLast->Next = pMsg;
            pThread->MsgLast = pMsg;

            // in this case the thread was scheduled on the previous step
            // and has to process all messages putted to the queue
        }
    }

    MxReleaseLock (&pThread->MsgSync);
    MxiRestInt(irq);

    if (bSchedThread)
        rc = MxScheduleThread (h);

    return rc;
}

/** @brief This function is designed to send a message (to put a message to the thread message queue)
           to the specified by the handle thread. This function can be used for all threads
           (locally and remotely created).

    @NOTE The thread is automatically scheduled by the system

    The function parameters are:

    @param h            [in] - the thread handle
    @param pMsgPtr      [in] - the pointer to the message (it will be delivered to the thread)
    @param MsgPriority  [in] - the message priority, 0 - highest priority
    @param Ctrl         [in] - the message control flags, please see: TSCHEDCTRL

    @return [MXRC] an error code

    \ingroup group_lte_4gmx_thread

*/

MXRC MxThreadSendMsgEx (HANDLE h, LPVOID pMsgPtr, UINT32 MsgPriority, UINT32 Ctrl)
{
    PMXCONTEXT pCtx;
    PTHREAD pThread;
    IRQTYPE irq;
    THREADMSG *pMsg, *SearchMsg, *PrevMsg = NULL;
    MXRC rc = MX_THREAD_MSG_OVERFLOW;
    UINT32 bSchedThread = 0;

    _ASSERT_HANDLE (h);
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD);

    pCtx = MxInqRemContext (HANDLE_GET_CPU(h));

    _ASSERT_PTR (pCtx);

    if ((pThread = (PTHREAD) MxiGetThreadDescr(pCtx, h)) == NULL)
        return MX_THREAD_HANDLE_ERROR;

    _ASSERT(IS_THR_ALLOC(pThread));

    MxiDisInt(&irq);
    MxGetLock (&pThread->MsgSync);

    if ((pMsg = (THREADMSG*)MxiAllocSysPart (&pThread->MsgPart)) != NULL)
    {
        rc = MX_OK;

        pMsg->Prior = MsgPriority;
        pMsg->Data  = pMsgPtr;
        pMsg->Next  = NULL;

        // if a list is empty
        if (pThread->MsgFirst == NULL)
        {
            pThread->MsgFirst = pMsg;
            pThread->MsgLast  = pMsg;

            bSchedThread = 1;
        }
        else
        {
            // to find the proper message place according to the message priority

            SearchMsg = pThread->MsgFirst;

            while (SearchMsg != NULL)
            {
                if (MsgPriority < SearchMsg->Prior)
                    break;

                PrevMsg   = SearchMsg;
                SearchMsg = SearchMsg->Next;
            }

            if (SearchMsg == NULL)
            {
                pThread->MsgLast->Next = pMsg;
                pThread->MsgLast = pMsg;
            }
            else if (PrevMsg != NULL)
            {
                pMsg->Next = SearchMsg;
                PrevMsg->Next = pMsg;
            }
            else
            {
                pMsg->Next = pThread->MsgFirst;
                pThread->MsgFirst = pMsg;
            }

            // in this case the thread was scheduled on the previous step
            // and has to process all messages putted to the queue
        }
    }

    MxReleaseLock (&pThread->MsgSync);
    MxiRestInt(irq);

    if (bSchedThread && (Ctrl & SCHED_ONLY_PUT_MSG) == 0)
        rc = MxScheduleThreadEx (h, 0, (Ctrl & (SCHED_AT_END | SCHED_AT_BEGIN/* | SCHED_SMP*/)));

    return rc;
}

/** @brief This function is designed to receive the message thread inside.

    @NOTE The thread is automatically scheduled by the system

    The function parameters are:

    @param  h     [in] - the thread handle, please see: MxGetCurrentThread()
    @param pMsgPtr[in] - the pointer to the variable to store a pointer to the message

    @return [MXRC] an error code

    \ingroup group_lte_4gmx_thread

*/
MXRC MxThreadRecvMsg (HANDLE h, LPVOID * pMsgPtr)
{
    PMXCONTEXT pCtx;
    PTHREAD pThread;
    IRQTYPE irq;
    THREADMSG *pTmpMsg;
    MXRC rc = MX_THREAD_MSG_EMPTY;

    _ASSERT_PTR(pMsgPtr);

    _ASSERT_HANDLE (h);
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD);

    pCtx = MxInqRemContext (HANDLE_GET_CPU(h));

    _ASSERT_PTR (pCtx);

    if ((pThread = (PTHREAD) MxiGetThreadDescr(pCtx, h)) == NULL)
        return MX_THREAD_HANDLE_ERROR;

    _ASSERT(IS_THR_ALLOC(pThread));

    MxiDisInt(&irq);
    MxGetLock (&pThread->MsgSync);

    if (pThread->MsgFirst != NULL)
    {
        rc = MX_OK;

        pTmpMsg = pThread->MsgFirst;
        *pMsgPtr = pTmpMsg->Data;
        pThread->MsgFirst = pTmpMsg->Next;

        if (pThread->MsgFirst == NULL)
            pThread->MsgLast = NULL;

        // to free the allocated message
        MxiFreeSysPart (&pThread->MsgPart, pTmpMsg);
    }
    else
    {
        *pMsgPtr = NULL;
    }

    MxReleaseLock (&pThread->MsgSync);
    MxiRestInt(irq);

    return rc;
}

#endif


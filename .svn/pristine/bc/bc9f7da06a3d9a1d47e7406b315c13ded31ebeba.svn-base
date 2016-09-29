//-------------------------------------------------------------------------------------------
/** @file 4gmx_event.c
 *
 * @brief An event API implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"

#ifdef MX_EVENT_ENABLED

#include "4gmx.h"
#include "4gmx_event.h"
#include "4gmx_kernel.h"
#include "4gmx_serv.h"
#include "4gmx_debug.h"
#ifdef MX_INTERCPU_ENABLED
#include "4gmx_intercpu.h"
#endif

#if  defined(TRACE_EVENT_CREATE) || \
     defined (TRACE_EVENT_REMOVE) || \
     defined (TRACE_EVENT_SET)   || \
     defined (TRACE_EVENT_RESET)

/** @brief  This function is called by the event API for tracing EVENT's events.

    @param pCtx    [in] - the system context
    @param id      [in] - the trace event ID
    @param hHandle [in] - the object handle (or HNULL)
    @param rc      [in] - error code of operation

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_event
*/
MXRC AddEventTrace (PMXCONTEXT pCtx, TraceEventID id, HANDLE hHandle, MXRC rc)
{
    TraceDataEvent trace;

    trace.hEvent = hHandle;
    trace.Rc     = rc;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif



/** @brief The function creates event object and initializes created descriptor.

    @param nInitState [in] - the initial event state
    @param name   [in] - the object name (it can be NULL)
    @param oHand  [out] - the handle of created object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_event
*/
MXRC MxCreateEvent(UINT32 nInitState, const char *name, HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    PEVENT pEvent;
    HANDLE h = HNULL;
    IRQTYPE irq;
    DEF_RADDR;

    _ASSERT(oHand != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pEvent = (PEVENT) MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_EVENT]);

    if (pEvent == NULL)
    {
#ifdef TRACE_EVENT_CREATE
        AddEventTrace (pCtx, TEID_CREATE_EVENT, HNULL, MX_EVENT_ALLOC_ERROR);
#endif
        MxiRestInt(irq);

        return MX_EVENT_ALLOC_ERROR;
    }

    pEvent->CtrlFlags = 0;
    pEvent->thread = HNULL;

    if (nInitState & EVENT_MODE_SET)
    {
        MARK_EVN_SET(pEvent);
    }
    else
    {
        MARK_EVN_RESET(pEvent);
    }

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_EVENT,
                ((((UINT32) pEvent) -
                  ((UINT32) pCtx->events)) / sizeof(EVENT)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_EVENT], pEvent);


#ifdef TRACE_EVENT_CREATE
            AddEventTrace (pCtx, TEID_CREATE_EVENT, h,
                           MX_EVENT_ASSIGN_NAME_ERROR);
#endif
            MxiRestInt(irq);

            return MX_EVENT_ASSIGN_NAME_ERROR;
        }

        MARK_EVN_NAMED(pEvent);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_EVN_UNNAMED(pEvent);
    }

    MARK_EVN_ALLOC(pEvent);


#ifdef TRACE_EVENT_CREATE
    AddEventTrace (pCtx, TEID_CREATE_EVENT, h, MX_OK);
#endif

    *oHand = h;

    MxiRestInt(irq);

    return MX_OK;
}

#ifdef MX_INTERCPU_ENABLED

/** @brief The function creates event object on remote system.

    @param cpu        [in] - the cpu ID
    @param nInitState [in] - the initial event state
    @param name       [in] - the object name (it can be NULL)
    @param oHand      [out] - the handle of created object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_event
*/
MXRC MxCreateEventEx(CPUID cpu, UINT32 nInitState, LPCSTR name, HANDLE * oHand)
{
    MxICEventCreate ic;

    _ASSERT_PTR(oHand);

    if (cpu == MxGetCpuID() || cpu == CUR_CPU)
        return MxCreateEvent(nInitState, name, oHand);

    ic.InitState = nInitState;
    ic.Handle    = oHand;
    ic.Name      = name;

    return MxICExec (cpu, MXIC_EVENT_CREATE, &ic, sizeof (ic));
}

#endif /*MX_INTERCPU_ENABLED*/

/** @brief The function removes allocated event descriptor and removes a linkage
           with a thread if the last one exists.

    @param h [in] - the event handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_event
*/
MXRC MxRemoveEvent(HANDLE h)
{
    PMXCONTEXT pCtx;
    PEVENT pEvent;
    IRQTYPE irq;

#ifdef MX_INTERCPU_ENABLED
    MxICEventRemove ic;
#endif /*MX_INTERCPU_ENABLED*/

    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_EVENT);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        return MxICExec(HANDLE_GET_CPU(h), MXIC_EVENT_REMOVE,
                         &ic, sizeof (ic));
    }

#else /*MX_INTERCPU_ENABLED*/

    /* Only locally created object can be removed */

    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

#endif /*MX_INTERCPU_ENABLED*/

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pEvent = (PEVENT) MxiGetEventDescr(pCtx, h);

    MARK_EVN_DEALLOC(pEvent);

#ifdef MX_NAMING_ENABLED

    if (IS_EVN_NAMED(pEvent))
    {
        MxiFreeName(pCtx, h);

        MARK_EVN_UNNAMED(pEvent);
    }

#endif /*MX_NAMING_ENABLED */

    pEvent->CtrlFlags = 0;

    MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_EVENT], pEvent);

#ifdef TRACE_EVENT_REMOVE
    AddEventTrace (pCtx, TEID_REMOVE_EVENT, h, MX_OK);
#endif

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function checks status of event object

    @param h [in] - the event handle

    @return [MXRC] error code/event state

    \ingroup group_lte_4gmx_event
*/
MXRC MxIsEventSet(HANDLE h)
{
    PMXCONTEXT pCtx;
    PEVENT pEvent;

#ifdef MX_INTERCPU_ENABLED
    MxICEventIsSet ic;
#endif

    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_EVENT);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        return MxICExec(HANDLE_GET_CPU(h), MXIC_EVENT_IS_SET,
                         &ic, sizeof (ic));
    }

#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    pEvent = (PEVENT) MxiGetEventDescr(pCtx, h);

    _ASSERT(pEvent != NULL);

    return (IS_EVN_SET(pEvent)) ? (MX_EVENT_SET) : MX_EVENT_RESET;
}

/** @brief The function sets event

    @param h [in] - the event handle

    @return [MXRC] error code/event state

    \ingroup group_lte_4gmx_event
*/
MXRC MxSetEvent(HANDLE h)
{
    PMXCONTEXT pCtx;
    PEVENT pEvent;
    IRQTYPE irq;
    MXRC rc;

#ifdef MX_INTERCPU_ENABLED
    MxICEventSet ic;
#endif

    rc = MX_OK;

    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_EVENT);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        return MxICExec(HANDLE_GET_CPU(h), MXIC_EVENT_SET,
                         &ic, sizeof (ic));
    }

#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    pEvent = (PEVENT) MxiGetEventDescr(pCtx, h);

    _ASSERT(pEvent != NULL);

    MxiDisInt(&irq);

    if (IS_EVN_AUTORESET(pEvent))
    {
        MARK_EVN_RESET(pEvent);
    }
    else
    {
        MARK_EVN_SET(pEvent);
    }

#ifdef TRACE_EVENT_SET
    AddEventTrace (pCtx, TEID_SET_EVENT, h, MX_OK);
#endif

    if (IS_EVN_LINKED(pEvent))
    {
        if (IS_EVN_SCHED_SMP(pEvent))
            rc = MxScheduleThreadEx(pEvent->thread, 0, SCHED_SMP);
        else
            rc = MxScheduleThread(pEvent->thread);
    }

    MxiRestInt(irq);

    return rc;
}

/** @brief The function resets event

    @param h [in] - the event handle

    @return [MXRC] error code/event state

    \ingroup group_lte_4gmx_event
*/
MXRC MxResetEvent(HANDLE h)
{
    PMXCONTEXT pCtx;
    PEVENT pEvent;
    IRQTYPE irq;

#ifdef MX_INTERCPU_ENABLED
    MxICEventSet ic;
#endif

    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_EVENT);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        return MxICExec(HANDLE_GET_CPU(h), MXIC_EVENT_RESET,
                         &ic, sizeof(ic));
    }

#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    pEvent = (PEVENT) MxiGetEventDescr(pCtx, h);

    _ASSERT(pEvent != NULL);

    MxiDisInt(&irq);

    MARK_EVN_RESET(pEvent);

#ifdef TRACE_EVENT_RESET
    AddEventTrace (pCtx, TEID_RESET_EVENT, h, MX_OK);
#endif

    MxiRestInt(irq);

    return MX_OK;
}

#endif /*MX_EVENT_ENABLED */

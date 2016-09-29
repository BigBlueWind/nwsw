//-------------------------------------------------------------------------------------------
/** @file 4gmx_list.c
 *
 * @brief Lists API implementation
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

#ifdef MX_TIMER_ENABLED

#include "4gmx.h"
#include "4gmx_timer.h"
#include "4gmx_kernel.h"
#include "4gmx_serv.h"
#include "4gmx_debug.h"

#ifdef MX_INTERCPU_ENABLED
#include "4gmx_intercpu.h"
#endif

#if  defined(TRACE_TIMER_CREATE)    || \
     defined (TRACE_TIMER_REMOVE)    || \
     defined (TRACE_TIMER_START)    || \
     defined (TRACE_TIMER_STOP)     || \
     defined (TRACE_TIMER_PULSE)

/** @brief  This function is called by the event API for tracing TIMER's events.

    @param pCtx    [in] - the system context
    @param id      [in] - the trace event ID
    @param hHandle [in] - the object handle (or HNULL)
    @param rc      [in] - error code of operation

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_timer
*/
MXRC AddTimerTrace (PMXCONTEXT pCtx, TraceEventID id, HANDLE hHandle, MXRC rc)
{
    TraceDataTimer trace;

    trace.hTimer = hHandle;
    trace.Rc     = rc;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif

INLINE void CutTimerFromList(PMXCONTEXT pCtx, PTIMER pTimer)
{
    if (pCtx->FirstTimer == pTimer)
    {
        pCtx->FirstTimer = pTimer->Next;
    }

    if (pCtx->LastTimer == pTimer)
    {
        pCtx->LastTimer = pTimer->Prev;
    }

    if (pTimer->Prev != NULL)
    {
        pTimer->Prev->Next = pTimer->Next;
    }

    if (pTimer->Next != NULL)
    {
        pTimer->Next->Prev = pTimer->Prev;
    }

    pTimer->Next = NULL;
    pTimer->Prev = NULL;
}

INLINE void CutFracTimerFromList(PMXCONTEXT pCtx, PTIMER pTimer)
{
    if (pCtx->FirstFracTimer == pTimer)
    {
        pCtx->FirstFracTimer = pTimer->Next;
    }

    if (pCtx->LastFracTimer == pTimer)
    {
        pCtx->LastFracTimer = pTimer->Prev;
    }

    if (pTimer->Prev != NULL)
    {
        pTimer->Prev->Next = pTimer->Next;
    }

    if (pTimer->Next != NULL)
    {
        pTimer->Next->Prev = pTimer->Prev;
    }

    pTimer->Next = NULL;
    pTimer->Prev = NULL;
}

void CutTimerFromTheList (PMXCONTEXT pCtx, PTIMER pTimer)
{
    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTimer);

    if (IS_FRAC_TMR(pTimer))
    {
        CutFracTimerFromList(pCtx, pTimer);
    }
    else
    {
        CutTimerFromList(pCtx, pTimer);
    }
}

BOOL IsTimerEnabled(void)
{
    MXRC rc;
    PMXCONTEXT pCtx;
    BOOL bEnabled;
    UINT32 len = sizeof (bEnabled);

     pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    if (pCtx->TmrDrvId == 0xFFFFFFFF)
        return FALSE;

    rc = MxReadDrvData(MxGetDrv(pCtx->TmrDrvId), TIMER_CMD_GET_STATUS, &bEnabled, &len);    

    if (FAILED(rc))
    {
        return FALSE;
    }

    return bEnabled;
}

MXRC EnableTimer(void)
{
    MXRC rc;
    PMXCONTEXT pCtx;

     pCtx = MxInqContext();
     _ASSERT(pCtx != NULL);

     if (pCtx->TmrDrvId == 0xFFFFFFFF)
        return MX_TIMER_NOT_ENABLED;
     
    rc = MxConfigDrv(MxGetDrv(pCtx->TmrDrvId), TIMER_CMD_INIT_LOCAL_TIMER, NULL, NULL);

    _ASSERT_RC(rc);

    return rc;
}

/** @brief The function creates timer object and initializes it.

    @param nMode  [in] - the timer's mode
    @param nPeriod  [in] - the timer period in ms
    @param name   [in] - the object name (it can be NULL)
    @param hEvent  [in] - the handle of event (it can be HNULL)
    @param oHand  [out] - the handle of created object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_timer
*/
MXRC MxCreateTimer(UINT32 nMode, UINT32 nPeriod, const char *name,
                   HANDLE hEvent, HANDLE * oHand)
{
#ifdef MX_TIMER_AUTO_LAUNCH
    MXRC rc;
#endif
    PMXCONTEXT pCtx;
    PTIMER pTimer;
    IRQTYPE irq;
    HANDLE h = HNULL;
    DEF_RADDR;

    _ASSERT(oHand != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_TIMER_AUTO_LAUNCH
    if (!IsTimerEnabled())
    {
        rc = EnableTimer();

        if (FAILED(rc))
            return rc;
    }
#endif
    MxiDisInt(&irq);

    pTimer = MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_TIMER]);

    if (pTimer == NULL)
    {
#ifdef TRACE_TIMER_CREATE
        AddTimerTrace (pCtx, TEID_CREATE_TIMER, HNULL, MX_TIMER_ALLOC_ERROR);
#endif

        MxiRestInt(irq);
        return MX_TIMER_ALLOC_ERROR;
    }

    pTimer->Next        = NULL;
    pTimer->Prev        = NULL;
    pTimer->CtrlFlags   = 0;
    pTimer->hEvent      = hEvent;
    pTimer->nPeriod     = nPeriod;
    pTimer->nPulses     = 0;
    pTimer->Proc        = 0;
    pTimer->ProcPtr     = 0;

    if (nMode & TIMER_MODE_PERIOD)
    {
        MARK_TMR_PERIODIC(pTimer);
    }
    else
    {
        MARK_TMR_NOT_PERIODIC(pTimer);
    }

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_TIMER,
                ((((UINT32) pTimer) -
                  ((UINT32) pCtx->timers)) / sizeof(TIMER)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_TIMER], pTimer);

#ifdef TRACE_TIMER_CREATE
            AddTimerTrace (pCtx, TEID_CREATE_TIMER, HNULL,
                           MX_TIMER_ASSIGN_NAME_ERROR);
#endif

            MxiRestInt(irq);

            return MX_TIMER_ASSIGN_NAME_ERROR;
        }

        MARK_TMR_NAMED(pTimer);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_TMR_UNNAMED(pTimer);
    }

    MARK_TMR_ALLOC(pTimer);

#ifdef TRACE_TIMER_CREATE
    AddTimerTrace (pCtx, TEID_CREATE_TIMER, h, MX_OK);
#endif

    *oHand = h;

    MxiRestInt(irq);

    return MX_OK;
}

#ifdef MX_INTERCPU_ENABLED

/** @brief The function creates and initializes timer on remote system.

    @param cpu      [in]  - the ID of remote system
    @param name     [in]  - the name of created object (it can be NULL)
    @param nMode    [in]  - the timer's mode
    @param nPeriod  [in]  - the timer period in ms
    @param hEvent   [in]  - the handle of event (it can be HNULL)
    @param oHand    [out] - the handle of created object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_timer
*/
MXRC MxCreateTimerEx(CPUID cpu, UINT32 nMode, UINT32 nPeriod, LPCSTR name,
                   HANDLE hEvent, HANDLE * oHand)
{
    MxICTimerCreate ic;

    if (cpu == MxGetCpuID() || cpu == CUR_CPU)
        return MxCreateTimer(nMode, nPeriod, NULL, hEvent, oHand);

    ic.Handle = oHand;
    ic.hEvent = hEvent;
    ic.nMode  = nMode;
    ic.nPeriod= nPeriod;
    ic.Name   = name;

    return MxICExec(cpu, MXIC_TIMER_CREATE, &ic, sizeof (ic));
}

#endif /*MX_INTERCPU_ENABLED*/


/** @brief The function removes allocated descriptor.

    @param h [in] - the event handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_timer
*/
MXRC MxRemoveTimer(HANDLE h)
{
    PMXCONTEXT pCtx;
    PTIMER pTimer;
    IRQTYPE irq;

#ifdef MX_INTERCPU_ENABLED
    MxICTimerRemove ic;
#endif

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_TIMER);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;

        return MxICExec(HANDLE_GET_CPU(h), MXIC_TIMER_REMOVE,
                         &ic, sizeof (ic));
    }

#else

     /* Only locally created object can be removed */
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

 #endif /*MX_INTERCPU_ENABLED*/

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pTimer = (PTIMER) MxiGetTimerDescr(pCtx, h);

    _ASSERT(pTimer != NULL);
    _ASSERT(IS_TMR_ALLOC(pTimer));

#ifdef MX_NAMING_ENABLED

    if (IS_TMR_NAMED(pTimer))
    {
        MxiFreeName(pCtx, h);

        MARK_TMR_UNNAMED(pTimer);
    }

#endif /*MX_NAMING_ENABLED */

    if (IS_TMR_ENABLED(pTimer))
    {
        CutTimerFromTheList(pCtx, pTimer);
    }

    pTimer->CtrlFlags = 0;

    MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_TIMER], pTimer);

#ifdef TRACE_TIMER_REMOVE
    AddTimerTrace (pCtx, TEID_REMOVE_TIMER, h, MX_OK);
#endif

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function starts timer (the timer will be processed by TIMER ISR).

    @param h [in] - the timer handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_timer
*/
MXRC MxStartTimer(HANDLE h)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    PTIMER pTimer;

#ifdef MX_INTERCPU_ENABLED
    MxICTimerStart ic;
#endif

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_TIMER);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;

        return MxICExec(HANDLE_GET_CPU(h), MXIC_TIMER_START,
                         &ic, sizeof (ic));
    }

#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pTimer = MxiGetTimerDescr(pCtx, h);

    _ASSERT(pTimer != NULL);

    if (IS_TMR_ENABLED(pTimer))
        return MX_OK;

    /* The timer should be added to the list of processed timers */

    if (pCtx->FirstTimer == NULL)
    {
        pCtx->FirstTimer = pTimer;
        pCtx->LastTimer = pTimer;

        pTimer->Next = NULL;
        pTimer->Prev = NULL;
    }
    else
    {
        pCtx->LastTimer->Next = pTimer;
        pTimer->Prev = pCtx->LastTimer;
        pCtx->LastTimer = pTimer;

        pTimer->Next = NULL;
    }

    MARK_TMR_ENABLED(pTimer);
    MARK_TMR_NON_FRAC(pTimer);

#ifdef TRACE_TIMER_START
    AddTimerTrace (pCtx, TEID_START_TIMER, h, MX_OK);
#endif

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function starts timer (the timer will be processed by TIMER ISR).

    @param h [in] - the timer handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_timer
*/
MXRC MxStartFracTimer(HANDLE h)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    PTIMER pTimer;

#ifdef MX_INTERCPU_ENABLED
    MxICTimerStart ic;
#endif

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_TIMER);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        return MxICExec(HANDLE_GET_CPU(h), MXIC_TIMER_START_FRAC,
                         &ic, sizeof (ic));
    }

#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pTimer = MxiGetTimerDescr(pCtx, h);

    _ASSERT(pTimer != NULL);

    if (IS_TMR_ENABLED(pTimer))
	{
		MxiRestInt(irq);
        return MX_TIMER_ALREADY_STARTED;
	}

    /* The timer should be added to the list of processed timers */

    if (pCtx->FirstFracTimer == NULL)
    {
        pCtx->FirstFracTimer = pTimer;
        pCtx->LastFracTimer = pTimer;

        pTimer->Next = NULL;
        pTimer->Prev = NULL;
    }
    else
    {
        pCtx->LastFracTimer->Next = pTimer;
        pTimer->Prev = pCtx->LastFracTimer;
        pCtx->LastFracTimer = pTimer;

        pTimer->Next = NULL;
    }

    MARK_TMR_ENABLED(pTimer);
    MARK_TMR_FRAC(pTimer);

#ifdef TRACE_TIMER_START
    AddTimerTrace (pCtx, TEID_START_TIMER_FRAC, h, MX_OK);
#endif

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function cuts timer from the list of processed by TIMER ISR timers.

    @param h [in] - the timer handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_timer
*/
MXRC MxStopTimer(HANDLE h)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    PTIMER pTimer;

#ifdef MX_INTERCPU_ENABLED
    MxICTimerStart ic;
#endif

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_TIMER);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;

        return MxICExec(HANDLE_GET_CPU(h), MXIC_TIMER_STOP,
                         &ic, sizeof (ic));
    }

#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pTimer = MxiGetTimerDescr(pCtx, h);

    _ASSERT(pTimer != NULL);

    if (IS_TMR_ENABLED(pTimer))
    {
        CutTimerFromTheList(pCtx, pTimer);

        MARK_TMR_DISABLED(pTimer);
    }

#ifdef TRACE_TIMER_STOP
    AddTimerTrace (pCtx, TEID_STOP_TIMER, h, MX_OK);
#endif
    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function sets timer period.

    @param h  [in] - the timer handle
    @param nPeriod [in] - the new timer period

    @return [MXRC] error code

    \ingroup group_lte_4gmx_timer
*/
MXRC MxSetTimerPeriod(HANDLE h, UINT32 nPeriod)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    PTIMER pTimer;

#ifdef MX_INTERCPU_ENABLED
    MxICTimerSetPeriod ic;
#endif

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_TIMER);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        ic.nPeriod= nPeriod;

        return MxICExec(HANDLE_GET_CPU(h), MXIC_TIMER_SETPERIOD,
                         &ic, sizeof (ic));
    }

#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pTimer = MxiGetTimerDescr(pCtx, h);

    _ASSERT(pTimer != NULL);

    pTimer->nPeriod = nPeriod;
    pTimer->nPulses = 0;

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function sets timer callback.
 @param h       [in] - the timer handle
 @param proc    [in] - the pointer to the application callback
 @param pPtr    [in] - the pointer for applicaton callback
 
 @return [MXRC] error code */

MXRC MxSetTimerProc(HANDLE h, MXTIMERPROC proc, void * pPtr)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    PTIMER pTimer;

#ifdef MX_INTERCPU_ENABLED
    MxICTimerSetProc ic;
#endif

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_TIMER);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        ic.Proc   = proc;
        ic.Ptr    = pPtr;

        return MxICExec(HANDLE_GET_CPU(h), MXIC_TIMER_SETPROC,
                         &ic, sizeof (ic));
    }

#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pTimer = MxiGetTimerDescr(pCtx, h);

    _ASSERT(pTimer != NULL);

    pTimer->Proc    = proc;
    pTimer->ProcPtr = pPtr;

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function returns timer period.

    @param h  [in] - the timer handle
    @param pnPeriod [out] - the timer period

    @return [MXRC] error code

    \ingroup group_lte_4gmx_timer
*/
MXRC MxGetTimerPeriod(HANDLE h, UINT32 * pnPeriod)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    PTIMER pTimer;

#ifdef MX_INTERCPU_ENABLED
    MxICTimerGetPeriod ic;
#endif

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_TIMER);
    _ASSERT(pnPeriod != NULL);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        ic.pnPeriod= pnPeriod;

        return MxICExec(HANDLE_GET_CPU(h), MXIC_TIMER_GETPERIOD,
                         &ic, sizeof (ic));
    }

#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pTimer = MxiGetTimerDescr(pCtx, h);

    _ASSERT(pTimer != NULL);

    *pnPeriod = pTimer->nPeriod;

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function sets/changes event handle.

    @param h  [in] - the timer handle
    @param hEvent [in] - the event handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_timer
*/
MXRC MxSetTimerEvent(HANDLE h, HANDLE hEvent)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    PTIMER pTimer;

#ifdef MX_INTERCPU_ENABLED
    MxICTimerSetEvent ic;
#endif

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_TIMER);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        ic.Event  = hEvent;

        return MxICExec(HANDLE_GET_CPU(h), MXIC_TIMER_SETEVENT,
                         &ic, sizeof (ic));
    }

#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pTimer = MxiGetTimerDescr(pCtx, h);

    _ASSERT(pTimer != NULL);

    pTimer->hEvent = hEvent;

    MxiRestInt(irq);

    return MX_OK;
}

MXRC MxRunTimer(LPVOID pCtx, PTIMER pTimer)
{
    MXRC rc = MX_OK;

#ifdef TRACE_TIMER_PULSE
    HANDLE h;
#endif

    pTimer->nPulses++;

    if (pTimer->nPulses >= pTimer->nPeriod)
    {
        pTimer->nPulses = 0;

#ifdef TRACE_TIMER_PULSE

        HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_TIMER,
                    ((UINT32)pTimer - (UINT32)((PMXCONTEXT)pCtx)->timers)/
                    sizeof (TIMER));

        AddTimerTrace (pCtx, TEID_PULSE_TIMER, h, MX_OK);
#endif

#ifdef MX_EVENT_ENABLED
        if (HANDLE_NOT_NULL(pTimer->hEvent))
        {
            rc = MxSetEvent(pTimer->hEvent);
        }
#endif

        if (IS_TMR_NOT_PERIODIC(pTimer))
        {
            MARK_TMR_DISABLED(pTimer);
            CutTimerFromTheList((PMXCONTEXT) pCtx, pTimer);
        }

        if (pTimer->Proc != NULL)
        {
            pTimer->Proc(pTimer->ProcPtr);
        }
    }

    return rc;
}

#endif /*MX_LIST_ENABLED */

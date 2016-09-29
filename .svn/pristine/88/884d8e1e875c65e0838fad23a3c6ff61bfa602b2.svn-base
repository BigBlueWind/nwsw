//-------------------------------------------------------------------------------------------
/** @file 4gmx_timer.h
 *
 * @brief Timers API declaration
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_TIMER_H_
#define _4GMX_TIMER_H_

#include "4gmx_config.h"

#ifdef MX_TIMER_ENABLED

#ifdef __cplusplus
extern "C"
{
#endif

#include "4gmx_types.h"
#include "4gmx_const.h"

/************************************************************************/
/*       The list of module error codes                                 */
/************************************************************************/
#define MX_TIMER_ALLOC_ERROR       DEF_MXRC(MXM_TIMER,  1)
#define MX_TIMER_ASSIGN_NAME_ERROR DEF_MXRC(MXM_TIMER,  2)
#define MX_TIMER_ALREADY_STARTED   DEF_MXRC(MXM_TIMER,  3)
#define MX_TIMER_NOT_ENABLED       DEF_MXRC(MXM_TIMER,  4)

/************************************************************************/
/*      The timer control flags                                         */
/************************************************************************/
#define TMR_CF_ALLOCATED           (1<<0)
#define TMR_CF_NAMED               (1<<1)
#define TMR_CF_PERIODIC            (1<<3)
#define TMR_CF_ENABLED             (1<<4)
#define TMR_CF_FRAC_TIMER          (1<<5)

#define MARK_TMR_ALLOC(TMR)        ((TMR)->CtrlFlags |= TMR_CF_ALLOCATED)
#define MARK_TMR_DEALLOC(TMR)      ((TMR)->CtrlFlags &= ~TMR_CF_ALLOCATED)
#define IS_TMR_ALLOC(TMR)          ((TMR)->CtrlFlags & TMR_CF_ALLOCATED)

#define MARK_TMR_NAMED(TMR)        ((TMR)->CtrlFlags |= TMR_CF_NAMED)
#define MARK_TMR_UNNAMED(TMR)      ((TMR)->CtrlFlags &= ~TMR_CF_NAMED)
#define IS_TMR_NAMED(TMR)          ((TMR)->CtrlFlags & TMR_CF_NAMED)

#define MARK_TMR_PERIODIC(TMR)     ((TMR)->CtrlFlags |= TMR_CF_PERIODIC)
#define MARK_TMR_NOT_PERIODIC(TMR) ((TMR)->CtrlFlags &= ~TMR_CF_PERIODIC)
#define IS_TMR_PERIODIC(TMR)       ((TMR)->CtrlFlags & TMR_CF_PERIODIC)
#define IS_TMR_NOT_PERIODIC(TMR)   (((TMR)->CtrlFlags & TMR_CF_PERIODIC) == 0)

#define MARK_TMR_ENABLED(TMR)      ((TMR)->CtrlFlags |= TMR_CF_ENABLED)
#define MARK_TMR_DISABLED(TMR)     ((TMR)->CtrlFlags &= ~TMR_CF_ENABLED)
#define IS_TMR_ENABLED(TMR)        ((TMR)->CtrlFlags & TMR_CF_ENABLED)

#define MARK_TMR_FRAC(TMR)         ((TMR)->CtrlFlags |= TMR_CF_FRAC_TIMER)
#define MARK_TMR_NON_FRAC(TMR)     ((TMR)->CtrlFlags &= ~TMR_CF_FRAC_TIMER)
#define IS_FRAC_TMR(TMR)           ((TMR)->CtrlFlags & TMR_CF_FRAC_TIMER)

/***************************************************************************/

/************************************************************************/
/*           The event type descriptor definition                       */
/************************************************************************/

typedef void (*MXTIMERPROC)(void * pPtr);

typedef struct _MX_TIMER_
{
    struct _MX_TIMER_ *Next;
    struct _MX_TIMER_ *Prev;

    HANDLE          hEvent;    /**< The event object connected with the timer */
    UINT32          nPeriod;   /**< The timer's period             */
    UINT32          nPulses;   /**< The current value of timer's ticks */
    UINT32          CtrlFlags;  /**< The object control flags       */

    MXTIMERPROC     Proc;
    void *          ProcPtr;
    
} TIMER, *PTIMER;

typedef enum _TIMER_MODE_
{
    TIMER_MODE_ONE_TIME = (0 << 0),
    TIMER_MODE_PERIOD   = (1 << 0)
} TIMERMODE;

/** @brief The function creates timer object and initializes it.

 @param nMode  [in] - the timer's mode
 @param nPeriod  [in] - the timer period in ms
 @param name   [in] - the object name (it can be NULL)
 @param hEvent  [in] - the handle of event (it can be HNULL)
 @param oHand  [out] - the handle of created object

 @return [MXRC] error code
*/
MXRC MxCreateTimer(UINT32 nMode, UINT32 nPeriod, const char *name,
                   HANDLE hEvent, HANDLE * oHand);

#ifdef MX_INTERCPU_ENABLED

/** @brief The function creates and initializes timer on remote system.

 @param cpu      [in]  - the ID of remote system
 @param name     [in]  - the name of created object (it can be NULL)
 @param nMode    [in]  - the timer's mode
 @param nPeriod  [in]  - the timer period in ms
 @param hEvent   [in]  - the handle of event (it can be HNULL)
 @param oHand    [out] - the handle of created object

 @return [MXRC] error code */

MXRC MxCreateTimerEx(CPUID cpu, UINT32 nMode, UINT32 nPeriod, LPCSTR name,
                   HANDLE hEvent, HANDLE * oHand);

#endif /*MX_INTERCPU_ENABLED*/

/** @brief The function removes allocated descriptor.
 @param h [in] - the event handle
 @return [MXRC] error code */

MXRC MxRemoveTimer(HANDLE h);

/** @brief The function starts timer (the timer will be processed by TIMER ISR).
 @param h [in] - the timer handle
 @return [MXRC] error code */

MXRC MxStartTimer(HANDLE h);

/** @brief The function starts timer (the timer will be processed by TIMER ISR).

    @param h [in] - the timer handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_timer
*/
MXRC MxStartFracTimer(HANDLE h);

/** @brief The function cuts timer from the list of processed by TIMER ISR timers.
 @param h [in] - the timer handle
 @return [MXRC] error code */

MXRC MxStopTimer(HANDLE h);

/** @brief The function sets timer period.
 @param h  [in] - the timer handle
 @param nPeriod [in] - the new timer period
 @return [MXRC] error code */

MXRC MxSetTimerPeriod(HANDLE h, UINT32 nPeriod);

/** @brief The function sets timer callback.
 @param h       [in] - the timer handle
 @param proc    [in] - the pointer to the application callback
 @param pPtr    [in] - the pointer for applicaton callback
 
 @return [MXRC] error code */

MXRC MxSetTimerProc(HANDLE h, MXTIMERPROC proc, void * pPtr);

/** @brief The function returns timer period.
 @param h  [in] - the timer handle
 @param pnPeriod [out] - the timer period
 @return [MXRC] error code */

MXRC MxGetTimerPeriod(HANDLE h, UINT32 * pnPeriod);

/** @brief The function sets/changes event handle.
 @param h  [in] - the timer handle
 @param hEvent [in] - the event handle
 @return [MXRC] error code */

MXRC MxSetTimerEvent(HANDLE h, HANDLE hEvent);

MXRC MxRunTimer(LPVOID pCtx, PTIMER pTimer);

#ifdef __cplusplus
}
#endif

#endif        /*MX_TIMER_ENABLED */
#endif /*_4GMX_TIMER_H_*/

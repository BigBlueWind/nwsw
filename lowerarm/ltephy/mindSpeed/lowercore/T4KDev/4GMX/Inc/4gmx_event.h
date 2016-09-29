//-------------------------------------------------------------------------------------------
/** @file 4gmx_event.h
 *
 * @brief An event API declaration
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_EVENT_H_
#define _4GMX_EVENT_H_

#include "4gmx_config.h"

#ifdef MX_EVENT_ENABLED

#ifdef __cplusplus
extern "C"
{
#endif

#include "4gmx_types.h"
#include "4gmx_const.h"

/************************************************************************/
/*       The list of module error codes                                 */
/************************************************************************/
#define MX_EVENT_ALLOC_ERROR        DEF_MXRC(MXM_EVENT, 1)
#define MX_EVENT_ASSIGN_NAME_ERROR  DEF_MXRC(MXM_EVENT, 2)
#define MX_EVENT_SET                DEF_MXRC(MXM_EVENT, 3)
#define MX_EVENT_RESET              DEF_MXRC(MXM_EVENT, 4)

/************************************************************************/
/*      The event control flags                                         */
/************************************************************************/
#define ECF_EVENT_SET               (1<<0)
#define ECF_NAMED                   (1<<1)
#define ECF_ALLOCATED               (1<<2)
#define ECF_LINKED                  (1<<3)  /**< The event object is linked with a thread   */
#define ECF_AUTO_RESET              (1<<4)  /**< An auto reset is used before thread scheduling  */
#define ECF_SCHED_SMP               (1<<5)  /**< The thread assigned to the event will be scheduled in SMP mode*/

#define MARK_EVN_SET(evn)           ((evn)->CtrlFlags |= ECF_EVENT_SET)
#define MARK_EVN_RESET(evn)         ((evn)->CtrlFlags &= ~ECF_EVENT_SET)
#define IS_EVN_SET(evn)             ((evn)->CtrlFlags & ECF_EVENT_SET)

#define MARK_EVN_ALLOC(evn)         ((evn)->CtrlFlags |= ECF_ALLOCATED)
#define MARK_EVN_DEALLOC(evn)       ((evn)->CtrlFlags &= ~ECF_ALLOCATED)
#define IS_EVN_ALLOC(evn)           ((evn)->CtrlFlags & ECF_ALLOCATED)

#define MARK_EVN_NAMED(evn)         ((evn)->CtrlFlags |= ECF_NAMED)
#define MARK_EVN_UNNAMED(evn)       ((evn)->CtrlFlags &= ~ECF_NAMED)
#define IS_EVN_NAMED(evn)           ((evn)->CtrlFlags & ECF_NAMED)

#define MARK_EVN_LINKED(evn)        ((evn)->CtrlFlags |= ECF_LINKED)
#define MARK_EVN_UNLINKED(evn)      ((evn)->CtrlFlags &= ~ECF_LINKED)
#define IS_EVN_LINKED(evn)          ((evn)->CtrlFlags & ECF_LINKED)

#define MARK_EVN_AUTORESET(evn)     ((evn)->CtrlFlags |= ECF_AUTO_RESET)
#define MARK_EVN_NOTAUTORESET(evn)  ((evn)->CtrlFlags &= ~ECF_AUTO_RESET)
#define IS_EVN_AUTORESET(evn)       ((evn)->CtrlFlags & ECF_AUTO_RESET)

#define MARK_EVN_SCHED_SMP(evn)     ((evn)->CtrlFlags |= ECF_SCHED_SMP)
#define MARK_EVN_NO_SCHED_SMP(evn)  ((evn)->CtrlFlags &= ~ECF_SCHED_SMP)
#define IS_EVN_SCHED_SMP(evn)       ((evn)->CtrlFlags & ECF_SCHED_SMP)

/***************************************************************************/

/************************************************************************/
/*           The event type descriptor definition                       */
/************************************************************************/
typedef struct _MX_EVENT_
{
    HANDLE thread;   /**< The descriptor of linked thread */
    UINT8 CtrlFlags; /**< The control flags of object     */
} EVENT, *PEVENT;

typedef enum _EVENT_MODE_
{
    EVENT_MODE_RESET = (0),
    EVENT_MODE_SET   = (1 << 0)
} EVENTMODE;

/** @brief The function creates event object and initializes creates descriptor.

 @param nInitState [in] - the initial event state
 @param name   [in] - the object name (it can be NULL)
 @param oHand  [out] - the handle of created object

 @return [MXRC] error code
*/
MXRC MxCreateEvent(UINT32 nInitState, const char *name, HANDLE * oHand);


#ifdef MX_INTERCPU_ENABLED

/** @brief The function creates event object on remote system.

 @param cpu        [in] - the cpu ID
 @param nInitState [in] - the initial event state
 @param name       [in] - the object name (it can be NULL)
 @param oHand      [out] - the handle of created object

 @return [MXRC] error code */

MXRC MxCreateEventEx(CPUID cpu, UINT32 nInitState, LPCSTR name, HANDLE * oHand);

#endif /*MX_INTERCPU_ENABLED*/


/** @brief The function removes allocated event descriptor and remove linkage with thread if last one exists.

 @param h [in] - the event handle

 @return [MXRC] error code */

MXRC MxRemoveEvent(HANDLE h);

/** @brief The function checks status of event object

 @param h [in] - the event handle

 @return [MXRC] error code/event state */

MXRC MxIsEventSet(HANDLE h);

/** @brief The function sets event

 @param h [in] - the event handle

 @return [MXRC] error code/event state */

MXRC MxSetEvent(HANDLE h);

/** @brief The function resets event

 @param h [in] - the event handle

 @return [MXRC] error code/event state */

MXRC MxResetEvent(HANDLE h);

#ifdef __cplusplus
}
#endif

#endif        /*MX_EVENT_ENABLED */
#endif /*_4GMX_EVENT_H_*/

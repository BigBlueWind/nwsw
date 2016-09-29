//-------------------------------------------------------------------------------------------
/** @file 4gmx_thread.h
 *
 * @brief The file contains types and threads API definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------


#ifndef _4GMX_THREAD_H_
#define _4GMX_THREAD_H_

#include "4gmx_config.h"
#include "4gmx_types.h"
#include "4gmx_const.h"

#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************/
/*     A module error codes                                             */
/************************************************************************/
#define  MX_ALLOC_THREAD_ERROR          DEF_MXRC(MXM_THREAD,  1)
#define  MX_ASSIGN_THREAD_NAME_ERROR    DEF_MXRC(MXM_THREAD,  2)
#define  MX_HANDLE_ERROR                DEF_MXRC(MXM_THREAD,  3)
#define  MX_EVENT_API_DISABLED          DEF_MXRC(MXM_THREAD,  4)
#define  MX_INJECT_CPU_ERROR            DEF_MXRC(MXM_THREAD,  5)
#define  MX_INTERCPU_DISABLED           DEF_MXRC(MXM_THREAD,  6)
#define  MX_THREAD_CPUID_ERROR          DEF_MXRC(MXM_THREAD,  7)
#define  MX_THREAD_HANDLE_ERROR         DEF_MXRC(MXM_THREAD,  8)
#define  MX_THREAD_SMP_IS_NOT_DEF       DEF_MXRC(MXM_THREAD,  9)
#define  MX_THREAD_MSG_OVERFLOW         DEF_MXRC(MXM_THREAD,  10)
#define  MX_THREAD_MSG_EMPTY            DEF_MXRC(MXM_THREAD,  11)
#define  MX_THREAD_SMP_SCHED_DETECTED   DEF_MXRC(MXM_THREAD,  12)
#define  MX_THREAD_IS_UNDER_SMP_PROC    DEF_MXRC(MXM_THREAD,  14)
#define  MX_THREAD_LIM_IS_NOT_EXCEEDED  DEF_MXRC(MXM_THREAD,  15)

/************************************************************************/
/*    The thread control flags                                          */
/************************************************************************/
#define  TCF_SCHEDULED                (1 << 0)
#define  TCF_ALLOCATED                (1 << 1)
#define  TCF_NAMED                    (1 << 2)
#define  TCF_UNDER_SMP_PROC           (1 << 3)  // the thread is under SMP processing at this moment
#define  TCF_NEW_SMP_SCHED            (1 << 4)  // this flag informs the system that SMP thread is needed to be rescheduled

#define  MARK_SCHEDULED(thr)          ((thr)->CtrlFlags |= TCF_SCHEDULED)
#define  MARK_UNSCHEDULED(thr)        ((thr)->CtrlFlags &= ~TCF_SCHEDULED)
#define  IS_SCHEDULED(thr)            ((thr)->CtrlFlags & TCF_SCHEDULED)

#define  MARK_THR_ALLOC(thr)          ((thr)->CtrlFlags |= TCF_ALLOCATED)
#define  MARK_THR_DEALLOC(thr)        ((thr)->CtrlFlags &= ~TCF_ALLOCATED)
#define  IS_THR_ALLOC(thr)            ((thr)->CtrlFlags & TCF_ALLOCATED)

#define  MARK_THR_NAMED(thr)          ((thr)->CtrlFlags |= TCF_NAMED)
#define  MARK_THR_UNNAMED(thr)        ((thr)->CtrlFlags &= ~TCF_NAMED)
#define  IS_THR_NAMED(thr)            ((thr)->CtrlFlags & TCF_NAMED)

#define  MARK_UNDER_SMP_PROC(thr)       ((thr)->CtrlFlags |= TCF_UNDER_SMP_PROC)
#define  CLEAR_UNDER_SMP_PROC(thr)      ((thr)->CtrlFlags &= ~TCF_UNDER_SMP_PROC)
#define  IS_UNDER_SMP_PROC(thr)         ((thr)->CtrlFlags & TCF_UNDER_SMP_PROC)

#define  MARK_TO_BE_SMP_SCHED(thr)      ((thr)->CtrlFlags |= TCF_NEW_SMP_SCHED)
#define  CLEAR_TO_BE_SMP_SCHED(thr)     ((thr)->CtrlFlags &= ~TCF_NEW_SMP_SCHED)
#define  IS_TO_BE_SMP_SCHED(thr)        ((thr)->CtrlFlags & TCF_NEW_SMP_SCHED)


/************************************************************************/
/*    The thread context definition                                     */
/************************************************************************/

typedef UINT32(*TENTRY)(LPVOID pData);

typedef struct _THREAD_MSG_
{
    struct _THREAD_MSG_ *   Next;  // the pointer to the next msg block
    LPVOID                  Data;  // the pointer to the data 
    UINT32                  Prior; // message priority, 0 - is high priority

}THREADMSG, *PTHREADMSG;

typedef struct _THREAD_
{
    volatile struct _THREAD_ *NextScheduled; /**< The pointer to the next scheduled thread  */
    volatile struct _THREAD_ *PrevScheduled; /**< The pointer to the previously scheduled thread */

    TENTRY Proc;        /**< The thread procedure                  */
    LPVOID ProcParam;   /**< A thread parameter, it can be NULL    */

    UINT32 ReturnVal;   /**< The latest return value of thread     */
    TPRIORITY Priority; /**< The thread priority, 0 - is a higest priority */
    VUINT32 CtrlFlags;  /**< The flags of thread state             */

#ifdef MX_THREAD_MSG_ENABLED
    VUINT32    MsgSync;
    THREADMSG  MsgStor [MX_MAX_THREAD_MSG];
    SYSFPART   MsgPart;
    THREADMSG *MsgFirst;
    THREADMSG *MsgLast;
#endif    
    
    UINT32     nSMPCtrlFlags;   // The parameters of SchedCtrl, see: MxSchedSmpThread 
    UINT32     nSMPPrior;       // the parameter pr, see: MxSchedSmpThread 
    UINT32     nOwnerCpuID;     // The CPU ID where this thread was created 
    UINT32     nSMPExecCpuID;   // SMP executor cpu id, the CPU id where this thread is executed at this moment, VALID if IS_UNDER_SMP_PROC()

    VUINT32    SchedCondVal;    // The current value of conditional threshold value of thread scheduling , if (SchedCondVal >= SchedCondLim) to sched a thread

} THREAD, *PTHREAD;

#ifdef MX_EVENT_ENABLED

typedef enum _THREAD_EVENT_
{
    THREAD_EVENT_MANUAL_RESET = (0),
    THREAD_EVENT_AUTO_RESET   = (1 << 0),
    THREAD_EVENT_SMP          = (1 << 1)
} THREADEVENT;

#endif        /*MX_EVENT_ENABLED */

/** @brief The function creates thread context and returns thread handle to an application

 @param proc [in] - the pointer to the thread binary code
 @param name [in] - an object's name, it can be null
 @param param[in] - the thread parameter
 @param prior[in] - the thread priority
 @param outHandle[out]- the handle of created object

 @return [MXRC] error code */

MXRC MxCreateThread(TENTRY proc, const char *name, LPVOID param,
                    TPRIORITY priority, HANDLE * outHandle);


#ifdef MX_INTERCPU_ENABLED

/** @brief The function creates thread and returns thread's handle
           to the application, the function allows to specify the target CPU.

 @param cpu  [in] - the thread target CPU
 @param proc [in] - the pointer to the thread binary code
 @param name [in] - object's name, it can be null
 @param param[in] - the thread parameter
 @param prior[in] - the thread priority
 @param outHandle[out]- the handle of created object

 @return [MXRC] error code */

MXRC MxCreateThreadEx(CPUID cpu, TENTRY proc, LPCSTR name, LPVOID param,
                    UINT8 priority, HANDLE * outHandle);

#endif /*MX_INTERCPU_ENABLED*/


/** @brief The function removes thread handle and unshedules thread if it was scheduled.
   If object was associated with ASCIIZ name, the linkage is dropped.

 @param h [in] - the thread descriptor

 @return [MXRC] error code */

MXRC MxRemoveThread(HANDLE h);

/** @brief The function sets/changes thread priority. If thread is already scheduled, the
   thread will be rescheduled according to the new priority.

 @param h [in]   - the thread descriptor
 @param priority [in] - the new thread priority

 @return [MXRC] error code */

MXRC MxSetThreadPriority(HANDLE h, TPRIORITY priority);

/** @brief The function returns thread priority.

 @param h [in]   - the thread descriptor
 @param priority [out] - the thread priority

 @return [MXRC] error code */

MXRC MxGetThreadPriority(HANDLE h, TPRIORITY * priority);

/** @brief The function sets/changes thread parameter.

 @param h [in]  - the thread descriptor
 @param param [in] - the new thread parameter

 @return [MXRC] error code */

MXRC MxSetThreadParam(HANDLE h, LPVOID param);

/** @brief The function returns thread parameter

 @param h [in]  - the thread descriptor
 @param param [in] - the new thread parameter

 @return [MXRC] error code */

MXRC MxGetThreadParam(HANDLE h, LPVOID * param);

/** @brief The function schedules thread for execution, if thread was already scheduled,
   the function do nothing.

 @param h [in] - the thread handle

 @return [MXRC] error code */

MXRC MxScheduleThread(HANDLE h);

/** @brief The function schedules thread for execution, if thread was already scheduled,
    the function do nothing.

    @param h        [in] - the thread handle
    @param PrCond   [in] - the new thread priority (if SCHED_USE_PRIORITY is used)
                         - the conditional threshold (if SCHED_BY_CONDITION is used)
    @param SchedCtrl[in] - the control mask, see: TSCHEDCTRL type

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/

MXRC MxScheduleThreadEx(HANDLE h, UINT32 PrCond, UINT32 SchedCtrl);

/** @brief This function "injects" thread code to the stream of remote CPU

 @param cpu[in] - the CPU id, it cannot be equal to the current CPU id
 @param h  [in] - the thread handle

 @return [MXRC] error code */

MXRC MxInjectThread(CPUID id, HANDLE h);

/** @brief This function "injects" application code to the stream of remote CPU
            in IRQ mode,  so, the application should take care about time of code execution

 @param cpu     [in] - the CPU id, it cannot be equal to the current CPU id
 @param entry   [in] - the entry point of application code
 @param pParam  [in] - application code parameter (it can be NULL)

 @return [MXRC] error code */

MXRC MxInjectCode(CPUID id, TENTRY entry, LPVOID pParam);

/** @brief The function unschedules thread, if thread was scheduled or
   do nothing in other case.

 @param h [in] - the thread handle

 @return [MXRC] error code */

MXRC MxUnscheduleThread(HANDLE h);

MXRC MxScheduleThreadByEvent(HANDLE hThr, HANDLE hEvn, UINT32 nMode);

/** @brief The function drops a connection between the thread and the event objects

 @param hThr [in] - the thread handle
 @param hEvn [in] - the event handle

 @return [MXRC] */

MXRC MxUnscheduleThreadByEvent(HANDLE hThr, HANDLE hEvn);

/** @brief The function returns previous thread return value or 0 if
   thread was not executed.

 @param h [in]  - the thread descriptor

 @return [UINT32] return value */

UINT32 MxGetThreadRetVal(HANDLE h);

/** @brief This function is designed to run thread code
           described with thread handle

    @param h        [in] - the thread handle
    @param pRetVal  [out]- optional, thread return value

    @return [MXRC] an error code */

MXRC MxRunThread (HANDLE h, PUINT32 pRetVal);

/** @brief The function return status of the specified thread: 

             TRUE - the thread was scheduled and is waiting to be processed
             FALSE- the thread was not scheduled or under processing that means it's not more scheduled 

    @return [BOOL] TRUE/FALSE */

BOOL MxIsThreadScheduled (HANDLE h);

/** @brief The function SMP schedules thread for execution, if thread was already scheduled,
    the function do nothing.

    @param pThr     [in] - the thread descriptor
    @param PrCond   [in] - the new thread priority (if SCHED_USE_PRIORITY is used)
                         - the thread schedule threshold (if SCHED_BY_CONDITION is used)
    @param SchedCtrl[in] - the control mask, see: TSCHEDCTRL type

    @return [MXRC] error code

    \ingroup group_lte_4gmx_thread
*/

MXRC MxSchedSmpThreadEx (PTHREAD pThr, UINT32 PrCond, UINT32 SchedCtrl);

#ifdef MX_THREAD_MSG_ENABLED

    /** @brief This function is designed to send a message (to put a message to the thread message queue)
               to the specified by the handle thread. This function can be used for all threads 
               (locally and remotely created). 

        @NOTE The thread is automatically scheduled by the system

        The function parameters are: 

        @param  h   [in] - the thread handle  
        @param pMsg [in] - the pointer to the message (it will be delivered to the thread)

        @return [MXRC] an error code */

    MXRC MxThreadSendMsg (HANDLE h, LPVOID pMsg);

    /** @brief This function is designed to send a message (to put a message to the thread message queue)
               to the specified by the handle thread. This function can be used for all threads 
               (locally and remotely created). 

        @NOTE The thread is automatically scheduled by the system

        The function parameters are: 

        @param  h           [in] - the thread handle  
        @param pMsg         [in] - the pointer to the message (it will be delivered to the thread)
        @param MsgPriority  [in] - the message priority, 0 - highest priority 
        @param Ctrl         [in] - the message control flags, please see: TSCHEDCTRL

        @return [MXRC] an error code */
        
    MXRC MxThreadSendMsgEx (HANDLE h, LPVOID pMsg, UINT32 MsgPriority, UINT32 Ctrl);

    /** @brief This function is designed to receive the message thread inside.

        @NOTE The thread is automatically scheduled by the system

        The function parameters are: 

        @param  h   [in] - the thread handle, please see: MxGetCurrentThread()
        @param pMsg [in] - the pointer to the variable to store a pointer to the message

        @return [MXRC] an error code */
        
    MXRC MxThreadRecvMsg (HANDLE h, LPVOID * pMsg);

#endif

#ifdef __cplusplus
}
#endif

#endif /*_4GMX_THREAD_H_*/

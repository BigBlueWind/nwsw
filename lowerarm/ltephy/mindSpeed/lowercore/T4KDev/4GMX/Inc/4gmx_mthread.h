//-------------------------------------------------------------------------------------------
/** @file 4gmx_mthread.h
 *
 * @brief The file contains types and multi-stack threads API definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------


#ifndef _4GMX_MTHREAD_H_
#define _4GMX_MTHREAD_H_

#include "4gmx_config.h"
#include "4gmx_types.h"
#include "4gmx_const.h"

#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************/
/*     Module error codes                                               */
/************************************************************************/
#define MX_ALLOC_MTHREAD_ERROR          DEF_MXRC(MXM_MTHREAD,  1)  // M-Thread allocate context error 
#define MX_ASSIGN_MTHREAD_NAME_ERROR    DEF_MXRC(MXM_MTHREAD,  2)  // Error to allocate the space for object name 
#define MX_MTHREAD_ALLOC_STACK_ERROR    DEF_MXRC(MXM_MTHREAD,  3)  // Can't  allocate the stack for m-thread
#define MX_MTHREAD_SELF_REMOVING        DEF_MXRC(MXM_MTHREAD,  4)  // The system cannot remove self thread
#define MX_MTHREAD_SSTHREAD_REMOVING    DEF_MXRC(MXM_MTHREAD,  5)  // SSThread removing is restricted 
/************************************************************************/
/*    The create thread API flags                                        */
/************************************************************************/
#define MX_MTHREAD_FLAGS_SUSPENDED      (1 << 0)

/************************************************************************/
/*    The thread control flags                                          */
/************************************************************************/
#define MTCF_ALLOCATED                  (1 << 0)
#define MTCF_NAMED                      (1 << 1)
#define MTCF_SCHEDULED                  (1 << 2)
#define MTCF_STACK_ALLOC                (1 << 3)    // The stack was allocated by OS and has to be deallocated by OS
#define MTCF_WAITING                    (1 << 4)    // The thread is waiting for some event

#define MARK_MTHR_ALLOC(thr)            ((thr)->CtrlFlags |= MTCF_ALLOCATED)
#define MARK_MTHR_DEALLOC(thr)          ((thr)->CtrlFlags &= ~MTCF_ALLOCATED)
#define IS_MTHR_ALLOC(thr)              ((thr)->CtrlFlags & MTCF_ALLOCATED)

#define MARK_MTHR_NAMED(thr)            ((thr)->CtrlFlags |= MTCF_NAMED)
#define MARK_MTHR_UNNAMED(thr)          ((thr)->CtrlFlags &= ~MTCF_NAMED)
#define IS_MTHR_NAMED(thr)              ((thr)->CtrlFlags & MTCF_NAMED)

#define MARK_M_SCHEDULED(thr)           ((thr)->CtrlFlags |= MTCF_SCHEDULED)
#define MARK_M_UNSCHEDULED(thr)         ((thr)->CtrlFlags &= ~MTCF_SCHEDULED)
#define IS_M_SCHEDULED(thr)             ((thr)->CtrlFlags & MTCF_SCHEDULED)

#define MARK_MTHR_STACK_ALLOC(thr)      ((thr)->CtrlFlags |= MTCF_STACK_ALLOC)
#define MARK_MTHR_IS_STACK_ALLOC(thr)   ((thr)->CtrlFlags & MTCF_STACK_ALLOC)
#define MARK_MTHR_NOT_STACK_ALLOC(thr)  ((thr)->CtrlFlags &= ~MTCF_STACK_ALLOC)

#define MART_MTHR_WAITING(thr)          ((thr)->CtrlFlags |= MTCF_WAITING)
#define MART_MTHR_NOT_WAITING(thr)      ((thr)->CtrlFlags &= ~MTCF_WAITING)
#define IS_MTHR_WAITING(thr)            ((thr)->CtrlFlags & MTCF_WAITING)

/************************************************************************/
/*    The m-thread context definition                                     */
/************************************************************************/

typedef UINT32(*MTENTRY)(LPVOID pData);

#ifdef MX_ARM_ENABLED

#define MTHREAD_HW_REG_NUM             16       // The number of registers used to switch the contexts
#define MTHREAD_HW_CPSR_DEF            0x113    // The default value of CPSR ARM register for the m-thread: ARM-MODE, enIRQ, enFIQ 

#define MTHREAD_HW_PARAM_REG           0
#define MTHREAD_HW_STACK_REG           13
#define MTHREAD_HW_LR_REG              14
#define MTHREAD_HW_PROC_REG            15
#define MTHREAD_HW_CSPR                16

typedef struct _MTHREAD_HW_CTX_
{
    VUINT32 r[17];

}MTHREAD_HW_CTX, *PMTHREAD_HW_CTX;

#else
#error "m-thread does support ARM processor only"
#endif // MX_ARM_ENABLED

typedef enum _MTHREAD_LIST_ID_
{
    MTHREAD_LIST_CREATED = 0,   // The list of created threads 
    MTHREAD_LIST_SCHED   = 1,   // The list of scheduled threads 
    MTHREAD_LIST_WAIT    = 2,   // The list of waited threads (MxWaitForObject)
    MTHREAD_LIST_SLEEP   = 3,   // The list of waited threads (MxSleep)
    MTHREAD_LIST_SEMA    = 4,   // The semaphore thread waiting for 

    MTHREAD_LIST_NUM
}MTHREAD_LIST_ID;

struct _MTHREAD_;

typedef struct _MTHREAD_LST_ELM_
{
    struct _MTHREAD_ *NextScheduled; /**< The pointer to the next scheduled thread  */
    struct _MTHREAD_ *PrevScheduled; /**< The pointer to the previously scheduled thread */

}MTHREAD_LST_ELM;

typedef struct _MTHREAD_
{
    MTHREAD_LST_ELM list [MTHREAD_LIST_NUM];

    UINT32  ID;                     // The thread ID assigned by the system automatically 
    UINT32  CpuID;                  // Cpu-id where this object is created 
    UINT32  CtrlFlags;              // The object control flags: see MTCF_xxx

    void *  StackPtr;               // The pointer to the stack buffer 
    UINT32  StackSize;              // The size of stack in bytes 

    UINT32  ThreadPrior;            // The priority provided by the application
    UINT32  ThreadSubPrior;         // The internal thread priority to track threads scheduling

    MTENTRY Proc;                   // The pointer to the thread procedure 
    LPVOID  ProcPtr;                // The pointer to the thread parameter
    UINT32  Quantum;                // The time quantum after which the thread has to be switched 
    UINT32  StartThreadTime;        // The time when m-thread was started 
    UINT32  StartSleepTime;         // The time when thread was putted to the list of waited threads (MxSleep)
    UINT32  SleepTime;              // The number of ticks to wait (ticks depends on API, it can be microseconds or part of microseconds)
    UINT32  SwNum;                  // The statistics, number of times thread took processor time 
    HANDLE  hWaitObj;               // The handle of the object this thread waiting for 

    MTHREAD_HW_CTX  hw;             // The HW context to implement context switching 

}MTHREAD, *PMTHREAD;


/** @brief This function creates multi-stack thread and  
           schedules it if flag MX_MTHREAD_STATUS_SUSPENDED is not specified 

    @param proc         [in]    - the pointer to the procedure 
    @param name         [in]    - the ACIIZ name of the thread 
    @param param        [in]    - the thread parameter
    @param priority     [in]    - the thread priority , 0 - is the highest 
    @param flags        [in]    - see: MX_MTHREAD_FLAGS_xxx
    @param pStack       [in]    - the pointer to the stack or NULL, if NULL the system automatically allocate the stack
    @param nStackSize   [in]    - the stack size in bytes 
    @param pOutHandle   [out]   - the thread handle


    */

MXRC MxMCreateThread (MTENTRY proc, 
                         const char * name, 
                         LPVOID param, 
                         TPRIORITY priority, 
                         UINT32 flags,
                         LPVOID pStack, 
                         UINT32 nStackSize, 
                         HANDLE * pOutHandle);

/** @brief This function creates multi-stack thread and  
           schedules it if flag MX_MTHREAD_STATUS_SUSPENDED is not specified 

    @param cpu          [in]    - the destination CPU-id
    @param proc         [in]    - the pointer to the procedure 
    @param name         [in]    - the ACIIZ name of the thread 
    @param param        [in]    - the thread parameter
    @param priority     [in]    - the thread priority , 0 - is the highest 
    @param flags        [in]    - see: MX_MTHREAD_FLAGS_xxx
    @param pStack       [in]    - the pointer to the stack or NULL, if NULL the system automatically allocate the stack
    @param nStackSize   [in]    - the stack size in bytes 
    @param pOutHandle   [out]   - the thread handle


    */

MXRC MxMCreateThreadEx (UINT32 cpu,
                         MTENTRY proc, 
                         const char * name, 
                         LPVOID param, 
                         TPRIORITY priority, 
                         UINT32 flags,
                         LPVOID pStack, 
                         UINT32 nStackSize, 
                         HANDLE * pOutHandle);

/** @brief This function is responsible to remove the thread from the pool of started threads 
           and to free the context of this thread 

    @param h [in] - the thread handler 

    @return [MXRC] an error code */

MXRC MxMRemoveThread (HANDLE h);

#ifdef __cplusplus
}
#endif

#endif


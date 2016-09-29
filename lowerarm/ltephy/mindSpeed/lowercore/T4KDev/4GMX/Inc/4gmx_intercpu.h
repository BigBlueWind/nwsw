//-------------------------------------------------------------------------------------------
/** @file 4gmx_intercpu.h
 *
 * @brief The file contains declaration of 4GMX inter-cpu commands/IDs
 *        and inter-cpu commands parameters
 * @author Mindspeed Technologies
 * @version $Revision: 1.16 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"

#ifdef MX_INTERCPU_ENABLED

#ifndef _4GMX_INTERCPU_H_
#define _4GMX_INTERCPU_H_

#include "4gmx_types.h"
#include "4gmx_const.h"

#include "4gmx_intercpu_thread.h"
#include "4gmx_intercpu_event.h"
#include "4gmx_intercpu_timer.h"
#include "4gmx_intercpu_sema.h"

#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************/
/*      Inter-CPU sub-module error codes                                */
/************************************************************************/
#define MX_IC_OK                    MX_OK
#define MX_IC_SET_IRQ_ERROR         DEF_MXRC(MXM_IC, 1)
#define MX_IC_ALLOC_THR_ERROR       DEF_MXRC(MXM_IC, 2)
#define MX_IC_ALLOC_QUEUE_ERROR     DEF_MXRC(MXM_IC, 3)
#define MX_IC_ENABLE_IRQ_ERROR      DEF_MXRC(MXM_IC, 4)
#define MX_IC_UNKNOWN_CLASS         DEF_MXRC(MXM_IC, 5)
#define MX_IC_UNKNOWN_CMD           DEF_MXRC(MXM_IC, 6)
#define MX_IC_DOESNOT_INIT          DEF_MXRC(MXM_IC, 7)
#define MX_IC_REMSYS_DOESNOT_INIT   DEF_MXRC(MXM_IC, 8)
#define MX_IC_OVERFLOW              DEF_MXRC(MXM_IC, 9)
#define MX_IC_TIMEOUT               DEF_MXRC(MXM_IC, 10)

/*****************************************************************************
*            The control flags of MXICCTX.CF and macros of flags control     *
******************************************************************************/
#define MX_IC_CF_SYS_INIT           (1 << 0)   // The system has been initialized and started

#define IS_IC_INIT(ctx)             (((ctx)->CF & MX_IC_CF_SYS_INIT))
#define IC_SET_INIT(ctx)            (((ctx)->CF |= MX_IC_CF_SYS_INIT))
#define IC_RESET_INIT(ctx)          (((ctx)->CF &= ~MX_IC_CF_SYS_INIT))


/******************************************************************************
*  The control flags of inter-cpu command status and macros of status control *
*******************************************************************************/
#define MXIC_STAT_COMPLETE      (UINT32)(1 << 0)
#define MXIC_STAT_SYNC_CMD      (UINT32)(1 << 1)

#define IS_CMD_COMPLETED(cmd)    ((cmd)->CmdStat & MXIC_STAT_COMPLETE)
#define SET_CMD_COMPLETED(cmd)   ((cmd)->CmdStat |= MXIC_STAT_COMPLETE)

#define IS_SYNC_CMD(cmd)        ((cmd)->CmdStat & MXIC_STAT_SYNC_CMD)
#define SET_SYNC_CMD(cmd)       ((cmd)->CmdStat |= MXIC_STAT_SYNC_CMD)
#define SET_ASYNC_CMD(cmd)      ((cmd)->CmdStat &= ~MXIC_STAT_SYNC_CMD)

/*****************************************************************************
*            The offsets of inter-cpu commands                               *
******************************************************************************/
#define MXIC_KERNEL_CLASS            0x0100
#define MXIC_THREAD_CLASS            0x0200
#define MXIC_EVENT_CLASS             0x0300
#define MXIC_TIMER_CLASS             0x0400
#define MXIC_SEMA_CLASS              0x0500

#define MXIC_DEFCMD(cls, id)         (((cls)) + (id))
#define MXIC_GETCLASS(cmd)           ((cmd) & 0xFF00)
#define MXIC_GETID(cmd)              ((cmd) & 0x00FF)

#ifdef MX_ARM_SMP_ENABLED

#define ICMxiCacheInvalid(x, y)
#define ICMxiCacheClean(x, y)

#else

#define ICMxiCacheInvalid(x, y) MxiCacheInvalid ((x), (y));
#define ICMxiCacheClean(x, y) MxiCacheClean ((x), (y));

#endif

/** @brief The inter-cpu commands IDs definition, it's used by the
            intr-cpu functionality for remote objects control    */

typedef enum _IC_CMD_ID_
{
    MXIC_KERNEL_GETOBJ          = MXIC_DEFCMD(MXIC_KERNEL_CLASS, 0),
    MXIC_KERNEL_SETOBJ_NAME     = MXIC_DEFCMD(MXIC_KERNEL_CLASS, 1),
    MXIC_KERNEL_GETOBJ_NAME     = MXIC_DEFCMD(MXIC_KERNEL_CLASS, 2),
    MXIC_KERNEL_GETCPU_INFO     = MXIC_DEFCMD(MXIC_KERNEL_CLASS, 3),

    /*  The threads inter-cpu commands */

    MXIC_THREAD_CREATE          = MXIC_DEFCMD(MXIC_THREAD_CLASS, 0),
    MXIC_THREAD_REMOVE          = MXIC_DEFCMD(MXIC_THREAD_CLASS, 1),
    MXIC_THREAD_SCHEDULE        = MXIC_DEFCMD(MXIC_THREAD_CLASS, 2),
    MXIC_THREAD_UNSCHEDULE      = MXIC_DEFCMD(MXIC_THREAD_CLASS, 3),
    MXIC_THREAD_SCH_BY_EVENT    = MXIC_DEFCMD(MXIC_THREAD_CLASS, 4),
    MXIC_THREAD_UNSCH_BY_EVENT  = MXIC_DEFCMD(MXIC_THREAD_CLASS, 5),
    MXIC_THREAD_SETPARAM        = MXIC_DEFCMD(MXIC_THREAD_CLASS, 6),
    MXIC_THREAD_GETPARAM        = MXIC_DEFCMD(MXIC_THREAD_CLASS, 7),
    MXIC_THREAD_GETRET          = MXIC_DEFCMD(MXIC_THREAD_CLASS, 8),
    MXIC_THREAD_SETPRIOR        = MXIC_DEFCMD(MXIC_THREAD_CLASS, 9),
    MXIC_THREAD_GETPRIOR        = MXIC_DEFCMD(MXIC_THREAD_CLASS, 10),
    MXIC_THREAD_INJECT_PROC     = MXIC_DEFCMD(MXIC_THREAD_CLASS, 11),
    MXIC_THREAD_INJECT_HANDLE   = MXIC_DEFCMD(MXIC_THREAD_CLASS, 12),
    MXIC_THREAD_SCHEDULE_EX     = MXIC_DEFCMD(MXIC_THREAD_CLASS, 13),

    #ifdef MX_MTHREAD_ENABLED
    MXIC_THREAD_CREATE_M        = MXIC_DEFCMD(MXIC_THREAD_CLASS, 14),
    #endif
    

#ifdef MX_EVENT_ENABLED

    /*  The events inter-cpu commands */

    MXIC_EVENT_CREATE           = MXIC_DEFCMD(MXIC_EVENT_CLASS,  0),
    MXIC_EVENT_REMOVE           = MXIC_DEFCMD(MXIC_EVENT_CLASS,  1),
    MXIC_EVENT_SET              = MXIC_DEFCMD(MXIC_EVENT_CLASS,  2),
    MXIC_EVENT_RESET            = MXIC_DEFCMD(MXIC_EVENT_CLASS,  3),
    MXIC_EVENT_IS_SET           = MXIC_DEFCMD(MXIC_EVENT_CLASS,  4),

#endif

#ifdef MX_TIMER_ENABLED

    /*  The timers inter-cpu commands */

    MXIC_TIMER_CREATE           = MXIC_DEFCMD(MXIC_TIMER_CLASS,  0),
    MXIC_TIMER_REMOVE           = MXIC_DEFCMD(MXIC_TIMER_CLASS,  1),
    MXIC_TIMER_SETPERIOD        = MXIC_DEFCMD(MXIC_TIMER_CLASS,  2),
    MXIC_TIMER_GETPERIOD        = MXIC_DEFCMD(MXIC_TIMER_CLASS,  3),
    MXIC_TIMER_START            = MXIC_DEFCMD(MXIC_TIMER_CLASS,  4),
    MXIC_TIMER_START_FRAC       = MXIC_DEFCMD(MXIC_TIMER_CLASS,  5),
    MXIC_TIMER_STOP             = MXIC_DEFCMD(MXIC_TIMER_CLASS,  6),
    MXIC_TIMER_SETEVENT         = MXIC_DEFCMD(MXIC_TIMER_CLASS,  7),
    MXIC_TIMER_SETPROC          = MXIC_DEFCMD(MXIC_TIMER_CLASS,  8),

#endif

#ifdef MX_MTHREAD_ENABLED
    MXIC_SEMA_CREATE            = MXIC_DEFCMD(MXIC_SEMA_CLASS,   0),
    MXIC_SEMA_REMOVE            = MXIC_DEFCMD(MXIC_SEMA_CLASS,   1),
    MXIC_SEMA_SET               = MXIC_DEFCMD(MXIC_SEMA_CLASS,   2),
#endif

}MXICCMDID;


/** @brief This type is used for sharing 4GMX data objects like
			partitions and queues (an extended inter-cpu functionality) */

typedef struct
{
	LPVOID pObjDescr;	// The pointer to the data object descriptor (it's located in the 4GMX context)
	HANDLE hHand;		// The descriptor of the shared data objects
	VUINT32 Lock;		// It's used for locking objects (not cached memory)

}MXICShrObj;


typedef struct
{
    LPCSTR  Name;   // In : The object name
    HANDLE* Handle; // Out: The object handle

}MxICKrnGetObj;

typedef struct
{
    HANDLE Handle; // In: The object handle
    LPCSTR Name;   // In: The object name

}MxICKrnSetObjName;

typedef struct
{
    HANDLE  Handle; // In : The object handle
    LPCSTR *Name;   // Out: The object name

}MxICKrnGetObjName;

typedef struct
{
    void * pCpuCtx; // The pointer to the cpu context (uniq for processor, ARM: ARMCPUCONTEXT)
}MxICKrnGetCpuCtx;

/** @brief The InterCPU command's parameter definition. Depends on the command ID,
           the command's parameter should be taken from one of defined objects */

typedef union _MX_IC_DATA_
{
    MxICKrnGetObj       KrnGetObj;      // "Get object handle" command
    MxICKrnSetObjName   KrnSetObjName;  // "It sets name of object (for any object)"
    MxICKrnGetObjName   KrnGetObjName;  // "It returns object name (for any object)"
    MxICKrnGetCpuCtx    KrnGetCpuCtx;   // To get remote CPU context

    /* The threads' commands */

    MxICThrCreate       ThrCreate;      // "Create thread" command
    #ifdef MX_MTHREAD_ENABLED
    MxICMThrCreate      MThrCreate;     // "Create m-thread" command
    #endif
    MxICThrRemove       ThrRemove;      // "Remove thread" command
    MxICThrSchedule     ThrSchedule;    // "Schedue/Unschedule thread" command
    MxICThrScheduleEx   ThrScheduleEx;  // "Schedue/Unschedule thread" command
    MxICThrSchByEvent   ThrSchByEvent;  // "Schedue thread by event" command
    MxICThrSetParam     ThrSetParam;    // "Set thread param" command
    MxICThrGetParam     ThrGetParam;    // "Get thread param" command
    MxICThrRet          ThrRet;         // "Get thread ret. value" command
    MxICThrSetPrior     ThrSetPrior;    // "Set thread priority" command
    MxICThrGetPrior     ThrGetPrior;    // "Get thread priority" command
    MxICThrInjectProc   ThrInjectProc;  // "Inject thread" command as proc entry
    MxICThrInjectHandle ThrInjectHandle;// "Inject thread" command as handle

#ifdef MX_EVENT_ENABLED

    MxICEventCreate     EventCreate;    // "Create event" command
    MxICEventRemove     EventRemove;    // "Remove event" command
    MxICEventSet        EventSetReset;  // "Set/Reset" commands
    MxICEventIsSet      EventIsSet;     // "Is event set" request

#endif /*MX_EVENT_ENABLED*/

#ifdef MX_TIMER_ENABLED

    MxICTimerCreate     TimerCreate;    // "Create timer" command
    MxICTimerRemove     TimerRemove;    // "Remove timer" command
    MxICTimerStart      TimerStart;     // "Start/Stop timer"  command
    MxICTimerSetPeriod  TimerSetPeriod; // "Set timer period "  command
    MxICTimerGetPeriod  TimerGetPeriod; // "Get timer period "  command
    MxICTimerSetEvent   TimerSetEvent;  // "Set timer event"  command
    MxICTimerSetProc    TimerSetProc;   // "Set timer proc"  command

#endif /*MX_TIMER_ENABLED*/

#ifdef MX_MTHREAD_ENABLED

    MxICSemaCreate      SemaCreate;     // To create the semaphore object
    MxICSemaRemove      SemaRemove;     // To create the semaphore object
    MxICSemaSet         SemaSet;        // To set semaphore 

#endif /*MX_TIMER_ENABLED*/


}MXICDATA;


/** @brief The 4GMX Inter-CPU command, this type is used
           as a base type of Inter-CPU queues and it's processed by the
           Inter-CPU commands parser */

typedef struct _MX_IC_CMD_
{
    UINT32      SrcCpuID; // The source CPU id
    
    #ifdef _MX_DEBUG
    UINT32      DstCpuID; // The destination CPU id
    #endif
    
    UINT32      CmdID;    // The ID of inter-cpu command (the type of data
    VUINT32     CmdStat;  // The status of command processing; see: MXIC_STAT_xxx
    MXICDATA    Param;    // The Inter-CPU command parameter

}MXICCMD;


/** @brief This context of data is used to suuport SMP threads

*/

typedef struct _MX_SMP_CTX_
{
    VUINT32 Lock;       // It's used to lock/unlock object
    VUINT32 ExcAPILock; // The lock used for exclusive kernel API (set, inc, dec)
    VUINT32 SchedNum;   // The number of scheduled threads
    PTHREAD FST;        // The pointer to the first SMP thread scheduled to be excecuted on some available core
    PTHREAD LST;        // The pointer to the last SMP thread scheduled to be excecuted on some available core

#if MX_SAFE_SECTION_NUM > 0
    UINT32  Res[128];
    VUINT32 SafeSectLocks [MX_SAFE_SECTION_NUM];
#endif

}MXSMPCTX;

/** @brief The 4GMX inter-cpu context, the main element of extended
           inter-cpu functionality*/
           
typedef struct _MXICCTX_
{
    VUINT32     Lock;       /**< IC semaphore                                    */
    VUINT32     CF;         /**< 4GMX inter-cpu control flags, see: MX_IC_CF_xxx */
    VUINT32     Irq;        /**< The inter-cpu interrupt id used for this system-instance */
    LPVOID      lpMxCtx;    /**< The pointer to the 4GMX OS context owns this object */
    VUINT32     CoreCPUID;  /**< This field contains information about current CPU*/

    UINT32      ScanPool;
    MXPOOL      CmdPool[MX_MAX_INTERCPU];
    MXRC        CmdRC  [MX_MAX_INTERCPU];   // This array is used to provide error code of sync commands processing
    
    SYSFPART    ShrPart;    // The partition for the 4GMX objects sharing

    // ========================================================================
    // The next part of the inter-cpu context has floating offset;
    // it depends on MX_MAX_xxx macros definition,
    // the direct access is not used, these objects should be used indirectly
    // by using objects descriptors
    // In this way, the different OSs can define different size of storages
    // ========================================================================

    UINT32      CmdQFree [MX_MAX_INTERCPU][MX_MAX_IC_QUEUE+1]; // The queue contains indexes of accessible IC commands (CmdPStor)
    UINT32      CmdQUsed [MX_MAX_INTERCPU][MX_MAX_IC_QUEUE+1]; // The queue contains indexes of allocated IC commands (CmdPStor)
    MXICCMD     CmdPStor [MX_MAX_INTERCPU][MX_MAX_IC_QUEUE];   // The storages of inter-cpu partiton

#if (MX_MAX_DATA_SHARED_OBJ > 0)
    MXICShrObj  ShrStorage [MX_MAX_DATA_SHARED_OBJ];
#endif
}MXICCTX, *PMXICCTX;

/** @brief The function initializes inter-cpu sub-system and called
           automatically by system kernel.

    @param pSysCtx[in] - the pointer to the system context
    @param nIrq   [in] - the Inter-CPU IRQ id
    @param pTable [in] - the pointer to the array of the 4GMX inter-cpu contexts
    @param pICCtx [in] - the pointer to the storage for the inter-cpu context
    @param pSmpCtx[in] - the pointer to the OSs SMP context

    @return [MXRC] an initialization error code

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxiInterCpuInit (LPVOID pSysCtx, UINT32 nIrq,
                      MXICCTX ** pTable, MXICCTX * pICCtx, MXSMPCTX * pSmpCtx);

/** @brief The function allocates a space for sharing 4GMX data object

    @param pMxCtx [in] - the pointer to the 4GMX context

    @return [MXICShrObj *] the pointer to the allocated memory */

MXICShrObj * MxiAllocSharedSpace(LPVOID pMxCtx);

/** @brief The function frees an allocated data object space

    @param pMxCtx [in] - the pointer to the 4GMX context
    @param pSpace [in] - the pointer to the allocated space (see: MxiAllocSharedSpace)

    @return [MXRC] error code */

MXRC MxiFreeSharedSpace(LPVOID pMxCtx, LPVOID pSpace);

/** @brief This function returns a pointer to the informational block of
           the shared object

    @param pMxCtx [in] - the pointer to the 4GMX context
    @param h      [in] - the handle of the searched object

    @return [MXICShrObj *] the pointer to the informational block or NULL*/

MXICShrObj *MxiFindSharedBlock (LPVOID pMxCtx, HANDLE h);

/** @brief This function returns a pointer to the object descriptor

    @param pMxCtx [in] - the pointer to the 4GMX context
    @param h      [in] - the handle of the searched object

    @return [void *] the pointer to the object descriptor or NULL*/

void * MxiFindSharedDescr (LPVOID pMxCtx, HANDLE h);

/** @brief The function sends an inter-cpu command to the remote system and
           returns result of command execution to the caller.

    @param DstCpu [in] - the destination CPU id
    @param id     [in] - the IC command (see: MXICCMDID type)
    @param pParam [in] - the pointer to command the parameter
    @param nSize  [in] - the size of parameter

    @return [MXRC] error code: MX_IC_xxx | remoute 4GMX error code */

MXRC MxICExec (CPUID DstCpu, MXICCMDID id, LPVOID pParam, UINT32 nSize);

/** @brief The function returns the remote system initialization status.
    @param cpu [in] - the remote system ID
    @return [BOOL] status */

BOOL MxICIsSystemInit (CPUID cpu);

MXRC MxiICWaitSysInit (UINT32 nCF);

//#define MxICSysWaitInit(ic_obj)	MxiICWaitSysInit((UINT32)&((ic_obj).CF))
#define MxICSysWaitInit(ic_obj)	MxiICWaitSysInit((UINT32)(ic_obj))

/** @brief The function waits a status of remote system initialization

    @param cpu     [in] - the remote system id
    @param nNumMs  [in] - the limit of waiting (in milliseconds)

    @return [MXRC] an error code of waiting */

MXRC MxICWaitSystemInit (CPUID cpu, UINT32 nNumMs);

MXRC MxICSetInit (BOOL b);

/** @brief This function returns information about CPU core
            of the remote 4GMX system, it's implemented to
            address remote systems like 0, 1, 2 ...

    @param cpu  [in] - the logical CPU ID
    @param pnVal[out]- the information of remote CPU

    @return [MXRC] */

MXRC MxGetRemoteCoreID (CPUID cpu, PUINT32 pnVal);

#ifdef __cplusplus
}
#endif

#endif /*_4GMX_INTERCPU_H_*/

#endif /*MX_INTERCPU_ENABLED */

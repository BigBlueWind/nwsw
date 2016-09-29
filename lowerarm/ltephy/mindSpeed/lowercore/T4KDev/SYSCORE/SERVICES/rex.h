//-------------------------------------------------------------------------------------------
/** @file rex.h
 *
 * @brief The declaration of the syscore RapidEXecutive (REX)
 * @author Mindspeed Technologies
 * @version $Revision: 1.51 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _REX_H_
#define _REX_H_

#ifndef CEVA_PROJ
#include "config.h"
#include "4gmx.h"
#include "appids.h"
#else
#include "global.h"
#endif

#include "tcb.h"

/*************************************************************************
*                      REX error codes                                   *
*************************************************************************/

#define REX_RC_PARAM_ERROR          DEF_USRRC(APP_MODULE, REX, 1)
#define REX_RC_ALLOC_ERROR          DEF_USRRC(APP_MODULE, REX, 2)
#define REX_RC_IO_INITED            DEF_USRRC(APP_MODULE, REX, 3)
#define REX_RC_IO_NUM_ERROR         DEF_USRRC(APP_MODULE, REX, 4)
#define REX_RC_DEP_OVERLOADED       DEF_USRRC(APP_MODULE, REX, 5)
#define REX_RC_LIST_EMPTY           DEF_USRRC(APP_MODULE, REX, 6)
#define REX_RC_LIST_DONE            DEF_USRRC(APP_MODULE, REX, 7)
#define REX_RC_SKIP                 DEF_USRRC(APP_MODULE, REX, 8)
#define REX_RC_DEP_ERROR            DEF_USRRC(APP_MODULE, REX, 9)
#define REX_RC_LIST_IS_SCHEDULED    DEF_USRRC(APP_MODULE, REX, 10)
#define REX_RC_IO_BUF_ERROR         DEF_USRRC(APP_MODULE, REX, 11)
#define REX_RC_LIST_IS_SCHED        DEF_USRRC(APP_MODULE, REX, 12)
#define REX_RC_UNSUPPORTED_RES      DEF_USRRC(APP_MODULE, REX, 14)
#define REX_RC_REINIT_ERROR         DEF_USRRC(APP_MODULE, REX, 15)
#define REX_RC_DOUBLE_SCHED         DEF_USRRC(APP_MODULE, REX, 16)
#define REX_RC_DOUBLE_ADD_TO_LST    DEF_USRRC(APP_MODULE, REX, 17)
#define REX_RC_SCHED_SKIPPED        DEF_USRRC(APP_MODULE, REX, 18)
#define REX_RC_REG_LIST_SKIPPED     DEF_USRRC(APP_MODULE, REX, 19)
#define REX_RC_LOCK_UNLOCK_DISABLED DEF_USRRC(APP_MODULE, REX, 20)
#define REX_RC_NOT_INITED           DEF_USRRC(APP_MODULE, REX, 21)
#define REX_RC_POOL_MASK_ERROR      DEF_USRRC(APP_MODULE, REX, 22)
#define REX_RC_REX_CS_IS_NOT_USED   DEF_USRRC(APP_MODULE, REX, 23)
#define REX_RC_MULT_LIST_DETECTED   DEF_USRRC(APP_MODULE, REX, 24)
#define REX_RC_DEV_MLOG_ERROR       DEF_USRRC(APP_MODULE, REX, 25)
#define REX_RC_NOT_SUPPORTED        DEF_USRRC(APP_MODULE, REX, 26)

#define REX_RESOURCE_CEVA_POOL_A    (RESOURCE_CEVA + 1)
#define REX_RESOURCE_CEVA_POOL_B    (RESOURCE_CEVA + 2)
#define REX_SUPPORTED_RES           (4 + 2)   // see resources in TCB.h + defined before

#define REX_ARM_DRV_TASK_NUM        512

#define REX_MAX_TCB_NUM             200
#define REX_MAX_HW_CTX_NUM          160
#define REX_MAX_SW_CTX_NUM          (REX_MAX_TCB_NUM - REX_MAX_HW_CTX_NUM)
#define REX_MAX_PRIVATE_TCB_NUM     10              // The number of private TCBs used by REX for communication with CEVAs
#define REX_MAX_LIST_NUM            24
#define REX_MAX_CEVA_PROC_PN        7               // CEVA procedure parameters numver
#define REX_CEVA_BLOCK_PROC_TIME    MS_TO_TICKS(2)  // The quantum of time of completion CEVA procedure calling(for blocking call only)

#define REX_HEAP                    (AppHeap)       // The heap that is used for default configuration
#define REX_SERV_HEAP               (DspHeap)       // REXTCB-D: The heap that is used in case of client/server configuration, see: REXCFG::ctrl {REX_CFG_STAT_SERV_ENABLED}
//#define REX_SERV_HEAP               (DspHeap)       // The heap that is used in case of client/server configuration, see: REXCFG::ctrl {REX_CFG_STAT_SERV_ENABLED}

#define REX_HW_HEAP                 (DspHeap)       // CEVA::TCB

#define REX_SW_HEAP                 (AppHeap)
#define REX_SW_SERV_HEAP            (DspHeap)       // FP::TCB + ARM::TCB

#define REX_LIST_HEAP               (AppHeap)       // REX::TASKLIST objects
#define REX_LIST_SERV_HEAP          (DspHeap)

#ifndef AUTO_ON
#define AUTO_ON                     1
#endif

#ifdef AUTO_OFF
#define AUTO_OFF                    0
#endif

/*************************************************************************
*                      REX configuration                                 *
*************************************************************************/

#define REX_SPLIT_FFT_IFFT              // To prevent scheduling of FFT and IFFT tasks at the same time (FP driver does not support it)
//#define REX_CB_THREAD_BASED           // To call the task-list callbacks in ARM supervisor mode, not in ARM IRQ mode
//#define REX_TCB_PRIORITY_ENABLED        // In this case REX uses TCB priority to find and schedule TCB block with highest priorities
#define REX_TCB_LOCK_UNLOCK_ENABLED     // To enable the Lock/Unlock functionality
//#define REX_PROF_ENABLED
#define REX_REINIT_BAD_HW
#define REX_QUANTUM_ENABLED
#define REX_INJECT_CALL_INSTEAD_OF_IRQ

#define REX_TCB_MAX_IOBUFS          32
#define REX_TCB_MAX_IOBUFS_SIZE     (REX_TCB_MAX_IOBUFS * sizeof (TIOBufDesc))
#define REX_TCB_MAX_CONTEXT_SIZE    (256)
#define REX_TCB_RSR_SPACE           (32)
#define REX_RES_FIFO_DEPTH          (20)

#define REX_TCB_CTX_SIZE            (ROUND((sizeof (TCB) + REX_TCB_MAX_CONTEXT_SIZE + REX_TCB_MAX_IOBUFS_SIZE), 32))
#define REX_TCB_SIZE                (ROUND(sizeof (REXTCBD), 32))

#define REC_TCB_CTRL_IN_LIST        (1 << 0)


#define REX_TCB_STAT_READY          (0 << 0)
#define REX_TCB_STAT_DONE           (1 << 1)

#define REX_LIST_STAT_RUN           (0 << 0)
#define REX_LIST_STAT_DONE          (1 << 0)

#define REX_CFG_STAT_INITED         (1 << 0) // The service is initialized
#define REX_CFG_STAT_SERV_ENABLED   (1 << 1) // The service is splitted to client and server parts
#define REX_CFG_IDFT_ON_CEVA        (1 << 2) // The IDFT(FP control) is going to be done on CEVA (REX-SERVER)

#define REX_CTRL_TCB_IN_LIST        (1 << 0)
#define REX_CTRL_TCB_SCHEDULED      (1 << 1)
#define REX_CTRL_TCB_BYPASSED       (1 << 2)
#define REX_CTRL_TCB_LOCKED         (1 << 3) // if this flag is set, REX does not touch this TCB block, till flag is removed
#define REX_CTRL_TCB_DONE           (1 << 4) // if this flag is set, if TCB block is done
#define REX_CTRL_LIST_IS_SCHEDULED  (1 << 0) // this flag shows that the list was added to the queue and will be processed by kernel
#define REX_CTRL_LIST_IS_DONE       (1 << 1) // this flag shows that list is done
#define REX_CTRL_LIST_STARTED       (1 << 2) // this flag to put once information to the MLog
#define REX_CTRL_LIST_UNDER_PROC    (1 << 3) // this flag shows that the list is under proceccing by REX

#define REX_RES_CTRL_UPDATED        (1 << 0) // The new one element was putted to the list of ready elements (valid for REX_TCB_PRIORITY_ENABLED)

// the parameters for RexMarkTCBControl function
#define REX_BYPASS_TCB	            (1 << 1) //if tcb is marked as such, scheduler will delink this tcb (ie not executed it) and mark it as complete
#define REX_LOCKED_TCB              (1 << 2)// if this flag is set, REX does not touch this TCB block, till flag is removed
#define REX_BLOCKING_CALL           ((UINT32)1 << 31)

#define REX_IS_SERV_CFG(ctx)        ((ctx)->Ctrl & REX_CFG_STAT_SERV_ENABLED)
#define REX_IS_IDFT_ON_CEVA(ctx)    ((ctx)->Ctrl & REX_CFG_IDFT_ON_CEVA)

typedef void (*CevaCallCbDoneType)(void*Ptr, UINT32 ceva_ret);

#define ceva_call(x)                                RexCevaCall(NULL, NULL, x, 0, 0)
#define ceva_call_q(x, q)                           RexCevaCall(NULL, NULL, x, q, 0)
#define ceva_call_cb(cb, ptr, x)                    RexCevaCall(cb,   ptr,  x, 0, 0)
#define ceva_call_cb_q(cb, ptr, x, q)               RexCevaCall(cb,   ptr,  x, q, 0)

#if REX_MAX_CEVA_PROC_PN>=1
#define ceva_call1(x,a)                             RexCevaCall(NULL, NULL, x, 0, 1, a)
#define ceva_call1_cb(cb, ptr, x, a)                RexCevaCall(cb,   ptr,  x, 0, 1, a)
#define ceva_call1_q(x,a,q)                         RexCevaCall(NULL, NULL, x, q, 1, a)
#define ceva_call1_cb_q(cb, ptr, x, q, a)           RexCevaCall(cb,   ptr,  x, q, 1, a)
#endif

#if REX_MAX_CEVA_PROC_PN>=2
#define ceva_call2(x,a,b)                           RexCevaCall(NULL, NULL, x, 0, 2, a, b)
#define ceva_call2_cb(cb, ptr, x,a,b)               RexCevaCall(cb,   ptr,  x, 0, 2, a, b)
#define ceva_call2_q(x,q,a,b)                       RexCevaCall(NULL, NULL, x, q, 2, a, b)
#define ceva_call2_cb_q(cb,ptr,x,q,a,b)             RexCevaCall(cb,   ptr,  x, q, 2, a, b)
#endif

#if REX_MAX_CEVA_PROC_PN>=3
#define ceva_call3(x,a,b,c)                         RexCevaCall(NULL, NULL, x, 0, 3, a, b, c)
#define ceva_call3_cb(cb,ptr,x,a,b,c)               RexCevaCall(cb,   ptr,  x, 0, 3, a, b, c)
#define ceva_call3_q(x,q,a,b,c)                     RexCevaCall(NULL, NULL, x, q, 3, a, b, c)
#define ceva_call3_cb_q(cb,ptr,x,q,a,b,c)           RexCevaCall(cb,   ptr,  x, q, 3, a, b, c)
#endif

#if REX_MAX_CEVA_PROC_PN>=4
#define ceva_call4(x,a,b,c,d)                       RexCevaCall(NULL, NULL, x, 0, 4, a, b, c, d)
#define ceva_call4_cb(cb,ptr,x,a,b,c,d)             RexCevaCall(cb,   ptr,  x, 0, 4, a, b, c, d)
#define ceva_call4_q(x,q,a,b,c,d)                   RexCevaCall(NULL, NULL, x, q, 4, a, b, c, d)
#define ceva_call4_cb_q(cb,ptr,x,q,a,b,c,d)         RexCevaCall(cb,   ptr,  x, q, 4, a, b, c, d)
#endif

#if REX_MAX_CEVA_PROC_PN>=5
#define ceva_call5(x,a,b,c,d,e)                     RexCevaCall(NULL, NULL, x, 0, 5, a, b, c, d, e)
#define ceva_call5_cb(cb, ptr, x,a,b,c,d,e)         RexCevaCall(cb,   ptr,  x, 0, 5, a, b, c, d, e)
#define ceva_call5_q(x,q,a,b,c,d,e)                 RexCevaCall(NULL, NULL, x, q, 5, a, b, c, d, e)
#define ceva_call5_cb_q(cb,ptr,x,q,a,b,c,d,e)       RexCevaCall(cb,   ptr,  x, q, 5, a, b, c, d, e)
#endif

#if REX_MAX_CEVA_PROC_PN>=6
#define ceva_call6(x,a,b,c,d,e,f)                   RexCevaCall(NULL, NULL, x, 0, 6, a, b, c, d, e, f)
#define ceva_call6_cb(cb, ptr, x,a,b,c,d,e,f)       RexCevaCall(cb,   ptr,  x, 0, 6, a, b, c, d, e, f)
#define ceva_call6_q(x,q,a,b,c,d,e,f)               RexCevaCall(NULL, NULL, x, q, 6, a, b, c, d, e, f)
#define ceva_call6_cb_q(cb,ptr,x,q,a,b,c,d,e,f)     RexCevaCall(cb,   ptr,  x, q, 6, a, b, c, d, e, f)
#endif

#if REX_MAX_CEVA_PROC_PN>=7
#define ceva_call7(x,a,b,c,d,e,f,g)                 RexCevaCall(NULL, NULL, x, 0, 7, a, b, c, d, e, f, g)
#define ceva_call7_cb(cb, ptr, x,a,b,c,d,e,f,g)     RexCevaCall(cb,   ptr,  x, 0, 7, a, b, c, d, e, f, g)
#define ceva_call7_q(x,q,a,b,c,d,e,f,g)             RexCevaCall(NULL, NULL, x, q, 7, a, b, c, d, e, f, g)
#define ceva_call7_cb_q(cb,ptr,x,q,a,b,c,d,e,f,g)   RexCevaCall(cb,   ptr,  x, q, 7, a, b, c, d, e, f, g)
#endif

#if REX_MAX_CEVA_PROC_PN>=8
#define ceva_call8(x,a,b,c,d,e,f,g,h)               RexCevaCall(NULL, NULL, x, 0, 8, a, b, c, d, e, f, g, h)
#define ceva_call8_cb(cb,ptr,x,a,b,c,d,e,f,g,h)     RexCevaCall(cb,   ptr,  x, 0, 8, a, b, c, d, e, f, g, h)
#define ceva_call8_q(x,q,a,b,c,d,e,f,g,h)           RexCevaCall(NULL, NULL, x, q, 8, a, b, c, d, e, f, g, h)
#define ceva_call8_cb_q(cb,ptr,x,q,a,b,c,d,e,f,g,h) RexCevaCall(cb,   ptr,  x, q, 8, a, b, c, d, e, f, g, h)
#endif

#if 0
#define _REX_NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
#define ceva_callx(x, ...)  (RexCevaCall(x, _REX_NUMARGS(__VA_ARGS__), __VA_ARGS__))
#endif

#define ceva_wait_complete(handle, time_is_us, ret) RexCevaWaitCallComplete(handle, time_is_us, ret)

typedef struct _REXCFG_
{
    UINT32  Ctrl;       // Configuration and status bits of REX, see: REX_CFG_STAT_xxx
    UINT32  Res [REX_SUPPORTED_RES];    // The bit-mask of the resources [CEVA-0, FP-1, and so on ]
    UINT32  TcbNum;     // The maximum number of TCBs that can be allocated by the application, if 0 the def. value is used
    UINT32  HwNum;      // The maximum number of HW TCBs contextex that can be allocated by the application, if 0 the def. value is used
    UINT32  SwNum;      // The maximum number of SW TCBs contextex that can be allocated by the application, if 0 the def. value is used
    UINT32  ListNum;    // The maximum number of Lists that can be allocated by the application, if 0 - the def. value is used

}REXCFG, *PREXCFG;

typedef struct _REXTCBD_
{
            PTCB                pHwTCB;         // The pointer to the TCB block located in ncnbmemory and used to communicate with CEVA
            unsigned int        TaskID;         // The task ID, this value is equal to TCB::TaskID (cacheable value)
            unsigned int        ResourceID;     // The copy if resource ID (to increase the speed of element processing)
volatile 	unsigned int        Ctrl;			// TCB control flags, see: REX_TCB_CTRL_xxx
	        void *              RexList;		// The pointer to the REX list, if not NULL, the TCB is already in the list
	        unsigned int	    DepNum;			// The number of tasks that required to be done before this element can be started
	        unsigned int	    DepDone;		// The number of processed elements that this element depends on
volatile 	struct _REXTCBD_ *	RexSchedNext;	// The pointer to the head scheduled element
volatile 	struct _REXTCBD_ *	RexSchedPrev;	// The pointer to the last scheduled element
volatile 	struct _REXTCBD_ *	RexDevNext;		// The pointer to the next routed to the device (resource) element
volatile 	struct _REXTCBD_ *	RexRoutedNext;	// The pointer to the next routed to the device (resource) element
	        unsigned int	    ResIndex;		// The index of the resource that has processed this TCB
	        unsigned int	    RefNum;			// The number of elements that refer to this task
	        struct _REXTCBD_ *	RefList [REX_DEP_SIZE]; // The array of references to this task
            struct _REXTCBD_ *	RexTcbNext;		// The pointer to the next element of the list
	        struct _REXTCBD_ *	RexTcbPrev;		// The pointer to the previous element of the list
	        unsigned int        Priority;       // The priority value (valid if REX_TCB_PRIORITY_ENABLED is defined)
	        void                (*CbDone)(TCB* pTcb, LPVOID pPtr, UINT32 nErrCode);
            LPVOID              CbDonePtr;      // The parameter for CbDone

            #ifdef REX_QUANTUM_ENABLED
            UINT32              Quantum;        // The quantum of time when task has to be completed (specified in global timer ticks)
            #endif

}REXTCBD, *PREXTCBD;

typedef struct REC_TASK_LIST
{
    UINT32                  ID;             // The task list ID, see: CreateTaskList function
    VUINT32                 Ctrl;
    VUINT32                 TcbNum;         // The number of TCB tasks located in this list
    VUINT32                 TcbDone;        // The number of completed TCB tasks
    VUINT32                 TcbError;       // The number of TCB tasks processed with error (CEVA timeout or CEVA returned error)
    volatile REXTCBD*       SchedHead;      // The pointer to the first scheduled element
    volatile REXTCBD*       SchedTail;      // The pointer to the last scheduled element
    volatile REXTCBD*       TcbHead;        // The head TCB of the task list
    volatile REXTCBD*       TcbTail;        // The last TCB of the task list

    UINT32                  StartTick;   // The value in ticks when list has been processed and callback is called
    void                    (*CbDone) (struct REC_TASK_LIST * pList, LPVOID lpData);
    void*                   CbDoneParam;
    VUINT32                 Lock;           // It's used to sync list access on different cores (Kernel & LOCK/UNLOCK TCBs)
}TASKLIST, *PTASKLIST;

// the type of TCB callback parameter
typedef void (*REXTCBCBDONE)(TCB* pTcb, LPVOID pPtr, UINT32 nErrCode);
typedef void (*REXCBDONE)(PTASKLIST pList, LPVOID lpData);
typedef MXRC  (*RexRunType)(LPVOID pRexCtx, REXTCBD * pTcbD);
typedef MXRC  (*RexRunClientType)(LPVOID pRexCtx, TCB * pTcb);
typedef UINT32 (*REXCBBADHW)(UINT32 nResID, UINT32 nMask, LPVOID lpData);
/*************************************************
*        REXRESOURCE::RoutedTcbHead index        *
*************************************************/

#define REX_ARM_RES_ID      0
#define REX_IDFT_RES_ID     0
#define REX_IFFT_FFT_RES_ID 1   // The same to IFFT&FFT

typedef struct _REX_RESOURCE_
{
    UINT32      EnableMask; // The bit-mask of the resources [CEVA-0,1,2...] or FP [...]
    UINT32      Ctrl;       // The resource flags, see: REX_RES_CTRL_xxx

    REXTCBD*    Head;       // The first  scheduled element
    REXTCBD*    Tail;       // The latest scheduled element

    RexRunType  Run;

    REXTCBD*    RoutedTcbHead[10];  // The pointer to the first TCB descriptor routed to the specific HW resource
    REXTCBD*    RoutedTcbTail[10];  // The pointer to the last TCB descriptor routed to the specific HW resource

    VUINT32     FFTNum;
    VUINT32     IFFTNum;

    RexRunClientType    RunClient;

    TCB*                tcb_fifo[REX_RES_FIFO_DEPTH];
    UINT32              tcb_fifo_get;
    UINT32              tcb_fifo_put;

}REXRESOURCE;

typedef struct _REX_CTX_
{
    UINT32       Ctrl;
    REXRESOURCE  Res[REX_SUPPORTED_RES];

    VUINT32     SchedListLock;
    VUINT32     SchedListPut;
    VUINT32     SchedListGet;
    PTASKLIST   SchedLists [REX_MAX_LIST_NUM];

    PTASKLIST   CList [REX_MAX_LIST_NUM];
    VUINT32     CListPut;
    VUINT32     CListGet;
    HANDLE      hCompleteThread;

    UINT32      DiscoverDep;    // To discover dependencies when new TCB is added to the list, 0 - disabled, 1 - enabled
    UINT32      MasterCpu;

    IRQTYPE     Irq;
    VUINT32     Lock;

    HANDLE      hListPart;
    HANDLE      hTcbPart;
    HANDLE      hHwCtxPart;
    HANDLE      hSwCtxPart;

    PTASKLIST   FirstList;  // The head of the list that is under processing
    PTASKLIST   LastList;   // The tail of the list that is under processing

    PTASKLIST   SchedListHead;  // The pointer to the first scheduled list
    PTASKLIST   SchedListTail;  // The pointer to the last scheduled list

    LPVOID      pTcbPtr;
    LPVOID      pHwCtxPtr;  // The pointer to the ncnb storage for TCB context/io bufs
    LPVOID      pSwCtxPtr;  // The pointer to the cached storage for TCB context/io bufs
    LPVOID      pListPtr;
    UINT32      TcbStorSize;
    UINT32      HwStorSize;
    UINT32      SwStorSize;

    PTASKLIST   CbDoneList;
    VUINT32     CbDoneBadTcb;   // The number of bad TCBs detected on processing current task list
                                // it's applicable for TaskList callback function only
    REXCBBADHW  CbBadHW;
    void*       CbBadHWPtr;

}REXCTX, *PREXCTX;

typedef enum _REX_ENUM_PROF_ID_
{
    REX_PROF_CEVA_RUN,
    REX_PROF_CEVA_DONE,
    REX_PROF_FP_RUN,
    REX_PROF_FP_DONE,
    REX_PROF_ARM_RUN,
    REX_PROF_ARM_DONE,
    REX_PROF_KERNEL,
    REX_PROF_PROC_LIST,
    REX_PROF_EXEC_LIST,
    REX_PROF_REINIT_LIST,
    REX_PROF_CALL_CB,
    REX_PROF_SCHED_TASKS,
    REX_PROF_RUN_SCHED_TASKS,
    REX_PROF_CEVA_RUN_FROM_CEVA_DONE,
    REX_PROF_FP_RUN_FROM_FP_DONE,

    REX_PROF_NUM
}REX_ENUM_PROF_ID;

typedef struct _REX_PROF_
{
    UINT32  MaxTime;
    UINT32  MinTime;
    UINT32  TotalTime;
    UINT32  TotalNum;

}REXPROF;

#ifdef __cplusplus
extern "C" {
#endif

/** @brief This function is designed to initialize REX service and
           should be called first before usage of any other API of this module

    @param pCfg [in] - configuration parameters

    @return [MXRC] an error code */

MXRC RexInit (REXCFG * pCfg);

/** @brief This function enables/disables discovery flag, if it's enabled
           the system searches for the additional dependency when new one TCB is added to the list

    @param nEnabled [in] - the flag : 0 - to disable; 1 - to enable

    @return [MXRC] an error code */

MXRC RexSetDepDiscovery (UINT32 nEnabled);

/** @brief This function allocates and initializes TCB object.

    @param TaskID           [in] - the task ID
    @param ResourceID       [in] - the resource ID, see: RESOURCE_xxx macros
    @param pContextData     [in] - the pointer to the context, it can be NULL
    @param ContextDataLen   [in] - the size of context in bytes
    @param cbDone           [in] - the pointer to cbDone function (it may be NULL)
    @param pPtr             [in] - the parameter for the cbDone function

    @return [TCB*]  the pointer to the allocated block or NULL

    *  \ingroup group_lte_rexscheduler

    */

TCB * RexCreateTCB   (UINT32 TaskID, UINT32 ResourceID, UINT32* pContextData, UINT32 ContextDataLen, REXTCBCBDONE cbDone, LPVOID pPtr);

/** @brief This function allocates and initializes TCB object.

    @NOTE It's used to define TASK time quantum

    @param TaskID           [in] - the task ID
    @param ResourceID       [in] - the resource ID, see: RESOURCE_xxx macros
    @param pContextData     [in] - the pointer to the context, it can be NULL
    @param ContextDataLen   [in] - the size of context in bytes
    @param cbDone           [in] - the pointer to cbDone function (it may be NULL)
    @param pPtr             [in] - the parameter for the cbDone function
    @param quantum          [in] - the period of time in microseconds, 0 - quantum is not used

    @return [TCB*]  the pointer to the allocated block or NULL

    *  \ingroup group_lte_rexscheduler

    */

TCB * RexCreateTCBQ   (UINT32 TaskID, UINT32 ResourceID, UINT32* pContextData, UINT32 ContextDataLen, REXTCBCBDONE cbDone, LPVOID pPtr, UINT32 quantum);

/** @brief The function deallocates memory reserved for the TCB block

    @param pTCB [in] - the removed TCB block

    @return [MXRC] an error code */

MXRC RexDestroyTCB (TCB * pTCB);

/** @brief The function can be used to mark TCB as BYPASSED
           The bypassed TCB is not going to be processed, this TCB
           just marks as completed

    @param pTCB [in] - the pointer to the TCB block
    @param nCtrl[in] - the control bits

    @return [MXRC] an error code */

MXRC RexMarkTCBControl (TCB * pTCB, UINT32 nCtrl);

/** @brief The function can be used to set TCB as BYPASSED/LOCKED
           The bypassed TCB is not going to be processed, this TCB
           just marks as completed
           The Locked TCB can not be scheduled to be executed and list
           is blocked till TCB is unlocked

    @param pTCB [in] - the pointer to the TCB block
    @param nCtrl[in] - the control bits

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexSetTCBControl (TCB * pTCB, UINT32 nCtrl);

/** @brief The function can be used to set TCB list as BYPASSED/LOCKED
           The bypassed TCB is not going to be processed, this TCB
           just marks as completed
           The Locked TCB can not be scheduled to be executed and list
           is blocked till TCB is unlocked

    @param pTCB [in] - the pointer to the TCB block
    @param nCtrl[in] - the control bits

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexSetTCBControlEx (TCB ** ppTCB, UINT32 nTcbNum, UINT32 nCtrl);

/** @brief The function can be used to set TCB as BYPASSED/LOCKED
           The bypassed TCB is not going to be processed, this TCB
           just marks as completed
           The Locked TCB can not be scheduled to be executed and list
           is blocked till TCB is unlocked

    @param pTCB [in] - the pointer to the TCB block
    @param nCtrl[in] - the control bits

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexResetTCBControl (TCB * pTCB, UINT32 nCtrl);

/** @brief The function can be used to reset list of TCBs as BYPASSED/LOCKED
           The bypassed TCB is not going to be processed, this TCB
           just marks as completed
           The Locked TCB can not be scheduled to be executed and list
           is blocked till TCB is unlocked

    @param pTCB [in] - the pointer to the TCB block
    @param nCtrl[in] - the control bits

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexResetTCBControlEx (TCB ** ppTCB, UINT32 nTcbNum, UINT32 nCtrl);

/** @brief This function creates/initializes TCB::IOBuffer pointer

    @param pTCB     [in] - the pointer to the TCB block, see: CreateTCB function
    @param NumDesc  [in] - the number of descriptors , it's limited by TCB_MAX_IOBUFS

    @return [MXRC] an error code */

MXRC RexCreateIOBuf(PTCB pTCB, UINT32 NumDesc);

/** @brief This function sets IO control buffers

    @param pTCB     [in] - the TCB task
    @param pData    [in] - the pointer to the IO buffer (in/out)
    @param DataLen  [in] - the size of IO buffer
    @param DataType [in] - the type of buffer (INPUT/OUTPUT/BOTH)

    @return [MXRC] an error code */

MXRC RexFillTCBIOBuf(PTCB pTCB, void* pData, UINT32 DataLen, UINT32 DataType);

/** @brief This function re-sets IO specific control buffers

    @param pTCB     [in] - the TCB task
    @param pData    [in] - the pointer to the IO buffer (in/out)
    @param DataLen  [in] - the size of IO buffer
    @param DataType [in] - the type of buffer (INPUT/OUTPUT/BOTH)
    @param IONum    [in] - the buffer index

    @return [MXRC] an error code */

MXRC RexReFillTCBIOBuf(PTCB pTCB, void* pData, UINT32 DataLen, UINT32 DataType, UINT32 IONum);

/** @brief This function returns IO control buffer specified by the index

    @param pTCB         [in] - the pointer to the TCB task
    @param nBufIndex    [in] - the index of the IO buffer

    @return [LPVOID] the pointer to the buffer */

LPVOID RexGetIOBuf(PTCB pTCB, UINT32 nBufIndex);

/** @brief This function is designed to find and return IO buffer specified by the index and by the type

    @param pTCB     [in    ] - the pointer to the TCB task
    @param pLen     [out   ] - the pointer to the buffer length
    @param IOType   [in    ] - the type of data
    @param ioNum    [in/out] - the buffer index

    @return [LPVOID] the pointer to the buffer */

LPVOID RexGetIOBufByType(PTCB pTCB, UINT32* pLen, UINT32 IOType, UINT32 *ioNum);

/** @brief This function creates and initializes a task list, the task list can be scheduled for execution

    @param TaskListID [in] - the task list ID, the ID should be unique

    @return [PTASKLIST] the pointer to the allocated task list or NULL */

PTASKLIST RexCreateTaskList(UINT32 TaskListID);

/** @brief This function destroys a task list object and a list of TCBs of this task list

    @param pList [in] - the pointer to the task list

    @return [MXRC] an error code */

MXRC RexDestroyTaskList (PTASKLIST pList);

/** @brief This function registers task list callback

    @param pList    [in] - the pointer to the task list
    @param cbDone   [in] - the pointer to the callback
    @param lpData   [in] - the callback parameter

    @return [MXRC] an error code */

MXRC RexRegListCBDone(TASKLIST* pList, REXCBDONE cbDone, LPVOID lpData);

/** @brief This function schedules list for execution, when list is completed,
            the REX service will call specified callback function.

    @param pList [in] - the pointer to the allocated list

    @return [MXRC] an error code of operation*/

MXRC RexExecList (PTASKLIST pList);

/** @brief This function schedules list for execution, when list is completed,
            the REX service will call specified callback function.

    @param pListX [in] - the pointer to the allocated list

    @return [MXRC] an error code of operation*/

MXRC RexExecList4 (PTASKLIST pList0, PTASKLIST pList1, PTASKLIST pList2, PTASKLIST pList3);

/** @brief This code routs one TCB block to the appropriate resource

    @param pTcb [in] - the pointer to the TCB task

    @return [MXRC] an error code */

MXRC RexExecTCB(TCB* pTcb);

/** @brief This function informs REX-S side about new TTI, in this way REX-S
           may upload prepared on previous TTI information to the DDR memory (like MLog)

    @return [MXRC] an error code of operation */

MXRC RexNewTTINotification(void);

/** @brief This function uploads MLog to DDR memory,
           this function can be used in Client-Server model only

    @return [MXRC] an error code*/

MXRC RexUploadMLog(void);

/** @brief The function adds TCB dependency, the dependency means that original TCB block depends on
            the specified one and cannot be executed prior dependent.

    @param pTcb         [in] - the pointer to the TCB that will be dependent on specified TCB task
    @param pDepOnThis   [in] - the dependence

    @return [MXRC] an error code*/

MXRC RexAddDep (TCB* pTcb, TCB * pDepOnThis);

/** @brief This function is used to associate TCB with memory cell,
           this is specific dependencies that is used to handle HW accelerators
           when CELL value is 1, REX will treat this dependencies like 'done' and
           TCB can be scheduled if no other dependencies

    @NOTE!
            This functionality is applicable for REX-SERVER mode only (CEVA or ARM-emulator)

    @param  pTcb [in] - the pointer to the TCB block accosiated with memory CELL
    @param  nAddr[in] - the CELL address (it cannot be NULL)
    @param  nInitVal[in] - the initial value for memory cell (if cell value not equal to this value, the cell dep. is done)

    @return [MXRC] an error code of operation */

MXRC RexAddCellDep (TCB* pTcb, UINT32 nAddr, UINT32 nInitVal);

/** @brief This function is used to reinitialize cells assigned to TCB blocks

    @NOTE!
            This functionality is applicable for REX-SERVER mode only (CEVA or ARM-emulator)

    @param  pCellList   [in] - the array of cell addresses
    @param  nNum        [in] - the array element number

    @return [MXRC] an error code of operation */


MXRC RexReinitCells (UINT32 * pCellList, UINT32 nNum);

/** @brief This function is used to reinitialize cells assigned to TCB blocks

    @NOTE!
            This functionality is applicable for REX-SERVER mode only (CEVA or ARM-emulator)

    @param  pCellList       [in] - the array of cell addresses
    @param  pNewCellAddrList[in] - the array of new cell addresses (addresses will be changed for current cells)
    @param  nNum            [in] - the array element number

    @return [MXRC] an error code of operation */


MXRC RexReinitCellsEx (UINT32 * pCellList, UINT32 * pNewCellAddrList, UINT32 nNum);

/** @brief The function adds TCB task to the specified task list according to the dependence weight priority

    @param pTaskList    [in] - the task list
    @param pTcb         [in] - the pointer to the TCB task that should be added to the list
    @param nTcbPrior    [in] - the TCB priority, 0 - highest priority, it's valid only if REX_TCB_PRIORITY_ENABLED is defined

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexAddTCBToList(PTASKLIST pTaskList, PTCB pTCB, UINT32 nTcbPrior);

/** @brief This function is designed to reinitialize the task list and to prepare it
           for the new scheduling

    @NOTE!  The list should not be scheduled for execution

    @param pList [in] - the pointer to the task list

    @return [MXRC] an error code */

MXRC RexReinitList (PTASKLIST pList);

/** @brief This is a central component of the REX service, this code
            schedules TCB tasks to the resources and designed to process
            task lists */

MXRC RexKernelRun(void);

/** @brief This function is designed to set ARM task handler

    @param taskid [in] - the TaskID, see: TCB::TaskID
    @param cb     [in] - the handler

    @return [MXRC] an error code */

MXRC RexRegArmTask (UINT32 taskid, void (*cb)(TCB*p));

/** @brief The system returns CPU core ID, on which REX service was initialized.
    @return [UINT32] core ID */

UINT32 RexGetMasterCpu (void);

/** @brief This function is designed to configure mask of available
           HW accelerators for some resource

    @param nResID   [in] - the resource ID, it can be:
                            REX_RESOURCE_CEVA_POOL_A
                            REX_RESOURCE_CEVA_POOL_B

    @param nResMask [in] - the bit-mask , 0 - to exclude the resource

    @return [MXRC] an error code */

MXRC RexConfigResource (UINT32 nResID, UINT32 nResMask);

/** @brief This function shutdowns REX service and frees allocated resources
    @return [MXRC] an error code */

MXRC RexShutdown (void);

/** @brief This code initializes super-TCB block

    @param GID          [in] - GroupID
    @param GroupHandle  [in] - the pointer to the previous TCB block or NULL
    @param pTaskList    [in] - the pointer to the task list (for MLog)
    @param numTCB       [in] - the number of parameters
    @param ...          [in] - TCB pointers

    @return [LPVOID] the pointer to the last element in the super TCB block */

LPVOID RexAddSuperTcb (UINT32 GID, LPVOID GroupHandle, PTASKLIST pTaskList, UINT32 numTCB, ...);

REXPROF * RexGetProfInfo (REX_ENUM_PROF_ID id);

/** @brief This function is designed to print the information about task list
           number of elements, processed elements, which tasks were scheduled,
           etc ...

    @param nID [in] - the task list ID (if '-1' - for all scheduled task lists)

    @return [MXRC] an error code */

MXRC RexPrintListInformation (UINT32 nID);

/** @brief This function prints main information about TCB block

    @param pTcbD [in] - the pointer to the TCB task

    @return [MXRC] an error code */

MXRC RexPrintTCBInfo (REXTCBD * pTcbD);

MXRC RexGenShedding(void);

UINT32 RexGetListID(PTASKLIST pList);

UINT32 RexGetNumBadTcb(PTASKLIST pList);

MXRC RexSetBadHWDetector(REXCBBADHW cb, void* ptr);

/** @brief This function sends request to CEVA code to call some CEVA code function,
           defined by nProcAddr.

    @param CbDone    [in]       - the pointer to the ARM CB-DONE function
    @param Ptr       [in]       - the pointer to the cbDone argument
    @param nProcAddr [in]       - the address of CEVA function
    @param nQ        [in]       - the time in microseconds the called function has to be completed (0 - the feature is not used)
    @param nParamNum [in]       - the number of parameters
    @param ...       [in/out]   - the function parameters, optional and depends on CEVA function

    @return [UINT32] CEVA call handle*/

UINT32 RexCevaCall (CevaCallCbDoneType CbDone, void * Ptr, UINT32 nProcAddr, UINT32 nQ, UINT32 nParamNum, ...);

/** @brief This function is designed to wait/check status of CEVA call
           started by RexCevaCall function.

    @Note!  ARM code may start CEVA code and than to check it completion

            handle = RexCevaCall(...);

            rc = RexCevaWaitCallComplete(handle, time, &ret);

    @param handle       [in] - the CEVA call handle
    @param time_is_us   [in] - the time in microsecods, 0 - to check and immediately return
    @param pRet         [in] - the pointer to storage for CEVA function return value (or NULL)

    @return [MXRC] an error code */

MXRC RexCevaWaitCallComplete(UINT32 handle, UINT32 time_is_us, UINT32 * pRet);

#ifdef __cplusplus
}
#endif

#endif // _REX_H_

#ifdef __cplusplus
}
#endif


//-------------------------------------------------------------------------------------------
/** @file rex_serv.h
 *
 * @brief The Rapid eXecutive (REX) server declaration, the server part is used
 *        by original REX service to process requests of REX and to free processor MIPS
 *        where original REX is located
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.32 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _REX_SERV_H_
#define _REX_SERV_H_

#include "tcb.h"
#include "rex.h"
//#include "ceva_proc.h"


#ifndef CEVA_PROJ
//#define REX_SERV_ON_ARM
#define REX_SERV_ARM_CORE_ID        2
#endif
//#define REXS_TASKS_MLOG_ENABLED

#define REXS_TRACE_SIZE             128
#define REXS_SUB_TASK_NUM           0//16          // Set to other than 0 value (max num of sub-tasks per CEVA)
#define REXS_CELL_DEP_NUM           16          // The number of TCBs that can be assigned for one memory cell
#define REXS_CELL_NUM               16          // The number of memory cells supported by REX-S
#define REXS_CEVA_RST_TIMEOUT       (5*150000)  // 5 ms
#define REXS_CEVA_PROC_CALL_NUM     16
#define REXS_CEVA_PROC_DONE_NUM     REXS_CEVA_PROC_CALL_NUM

/*************************************************************************
*                      REX-server MLOGs tasks                            *
*************************************************************************/

#define RESOURCE_REXS               RESOURCE_LARM
#define REXS_MLOG_PROC_CMD          100             // [100..149] is reserved for commands
#define REXS_MLOG_COMPLETE_LIST     150
#define REXS_MLOG_PROC_LIST         181
#define REXS_MLOG_PROC_SCHED_LISTS  182
#define REXS_MLOG_PROC_RES          183
#define REXS_MLOG_CLIENT_RUN_TCB    184
#define REXS_MLOG_CLIENT_RUN_TCB_CB 185

/*************************************************************************
*                      REX-server 4gmx error codes                       *
*************************************************************************/
#define REX_SERV_RC_ALLOC_ERROR     DEF_USRRC(APP_MODULE, REX_SERV, 1)
#define REX_SERV_RC_ALLOC_CMD_ERROR DEF_USRRC(APP_MODULE, REX_SERV, 2)
#define REX_SERV_RC_ALLOC_TCB_ERROR DEF_USRRC(APP_MODULE, REX_SERV, 3)
#define REX_SERV_RC_SEND_TCB_ERROR  DEF_USRRC(APP_MODULE, REX_SERV, 4)
#define REX_SERV_RC_MLOG_STOR_NULL  DEF_USRRC(APP_MODULE, REX_SERV, 5)
#define REX_SERV_RC_SHD_CMD_ERR     DEF_USRRC(APP_MODULE, REX_SERV, 6)
#define REX_SERV_RC_SHD_TIMEOUT     DEF_USRRC(APP_MODULE, REX_SERV, 7)
#define REX_SERV_RC_PARAM_ERROR     DEF_USRRC(APP_MODULE, REX_SERV, 8)
#define REX_SERV_RC_ADD_DEP_ERROR   DEF_USRRC(APP_MODULE, REX_SERV, 9)
#define REX_SERV_RC_SCMD_TIMEOUT    DEF_USRRC(APP_MODULE, REX_SERV,10)
#define REX_SERV_RC_PRV_INIT_ERR    DEF_USRRC(APP_MODULE, REX_SERV,11)
#define REX_SERV_RC_HANDLE_ERROR    DEF_USRRC(APP_MODULE, REX_SERV,12)
#define REX_SERV_RC_NOT_DONE        DEF_USRRC(APP_MODULE, REX_SERV,14)
#define REX_SERV_RC_NOT_SUPPORTED   DEF_USRRC(APP_MODULE, REX_SERV,15)

/*************************************************************************
*                      REX-server CEVA error codes                       *
*************************************************************************/
#define REX_SERV_RC_UNKNOWN_CMD         1
#define REX_SERV_RC_SKIP                2
#define REX_SERV_RC_DEP_ERROR           3
#define REX_SERV_RC_DEP_OVERLOADED      4
#define REX_SERV_RC_LIST_IS_SCHED       5
#define REX_SERV_RC_DOUBLE_ADD_TO_LST   6
#define REX_SERV_RC_LIST_IS_SCHEDULED   7
#define REX_SERV_RC_LIST_IS_UNDER_PROC  8
#define REX_SERV_RC_MLOG_OVERFLOW       9
#define REX_SERV_RC_LIST_CREATE_ERROR   10
#define REX_SERV_RC_TCBD_CREATE_ERROR   11
#define REX_SERV_RC_LIST_ID_ERROR       12
#define REX_SERV_RC_NOT_FREE_CELLS      13
#define REX_SERV_RC_NOT_FREE_CELLS_TCB  14
#define REX_SERV_RC_TCBD_ERROR          15
#define REX_SERV_RC_TCBD_LIST_ERROR     16
#define REX_SERV_RC_FEATURE_TURNED_OFF  17
#define REX_SERV_RC_INDEX_ERROR         18
#define REX_SERV_RC_PRV_BLOCK_INIT_ERR  19
#define REX_SERV_RC_CALL_QUEUE_OVERFLOW 20
/*************************************************************************
*                      REX-server parameters                             *
*************************************************************************/
#define REX_SERVER_CMD_NUM          30
#define REX_SERVER_PROC_QUEUE       (REX_MAX_HW_CTX_NUM+REX_MAX_PRIVATE_TCB_NUM) // The number of elements that can be putted to the processing queue
#define REXS_CEVA_HIGH_PROC_QUEUE   20
#define REX_SERVER_LIST_NUM         (REX_MAX_LIST_NUM)                           // The number of lists
#define RES_SERV_HW_CEVA_NUM        10
#define REX_SERV_CLIENT_IRQ_ID      6
#define REX_SERV_TIMEOUT_SYNC_CMD   150000

#define REX_SERV_CELL_ALLOC         (1<<0)
#define REX_SERV_CELL_ENABLED       (1<<1)

/**************************************************************************/
/* REX CC - REX CEVA CALL macros                                          */
/**************************************************************************/
#define REX_MAKE_CC_HANDLE(cpu, idx)       (((UINT32)1 << 31) | (((cpu)&0xF)<<16) | ((idx)&0xFFFF))
#define REX_CC_HANDLE_GET_CPU(x)        (((x) >> 16) & 0xF)
#define REX_CC_HANDLE_GET_IDX(x)        (((x) >> 0 ) & 0xFFFF)
#define REX_CC_HANDLE_INIT_VAL          0
#define REX_CC_HANDLE_DONE_VAL          1

/**************************************************************************/
/*    Control/Configuration parameters                                    */
/**************************************************************************/
#define REX_SERV_CTRL_IDFT_ON_CEVA      (1<<0)

/** @brief The commands ID definition, these commands are used to communicate
           between client->server and server->client

    [Server] part is located on CEVA processor and it controls CEVA pools
    [Client] part is located on ARM and processes FP/ARM tasks and communicate with server part

*/

typedef enum _REX_SERV_CMD_
{
    REX_CMD_INIT                = 0,    // to server: server initialization
    REX_CMD_EXEC_LIST           = 1,    // to server: to run TCB list
    REX_CMD_LIST_IS_DONE        = 2,    // to client: the list is done and callback maybe called
    REX_CMD_EXEC_TCB            = 3,    // to client: the client processed TCB block (mainly for FP or ARM tasks)
    REX_CMD_EXEC_TCB_CB         = 4,    // to client: the client must call TCB callback
    REX_CMD_TCB_IS_DONE         = 5,    // to server: the client completed TCB block(s) and server may process dependencies
    REX_CMD_SET_TCB_CONTROL     = 6,    // to server: to set/reset bypass/locked flags
    REX_CMD_RESET_TCB_CONTROL   = 7,    // to server: to set/reset bypass/locked flags
    REX_CMD_REINIT_LIST         = 8,    // to server: to reinit task list
    REX_CMD_ERROR_CODE          = 9,    // to client: to report about some problem
    REX_CMD_ASSERT              = 10,   // to client: to report about some assert
    REX_CMD_NEW_TTI             = 11,   // to server: to notify about new TTI and server will upload all internal information like MLogs
    REX_CMD_SET_TCB_CONTROL_EX  = 12,   // to server: the same to REX_CMD_SET_TCB_CONTROL cmd, but list of TCBs instead of one TCB
    REX_CMD_RESET_TCB_CONTROL_EX= 13,   // to server: the same to REX_CMD_RESET_TCB_CONTROL cmd, but list of TCBs instead of one TCB
    REX_CMD_SHUTDOWN            = 14,   // to server: to go to infinite loop and to stop any processing
    REX_CMD_UPLOAD_MLOG         = 15,   // to server: to upload MLOG storage from internal CEVA memory to DDR
    REX_CMD_CREATE_LIST         = 16,   // to server: to creare list object in server local memory
    REX_CMD_REG_CB_DONE_LIST    = 17,   // to server: to set ARM call-back pointer to be called on ARM side when list is done
    REX_CMD_DESTROY_LIST        = 18,   // to server: to destroy task list object in server local memory
    REX_CMD_CREATE_TCBD         = 19,   // to server: to creare TCB descriptor (real TCB is located in CRAM )object in server local memory
    REX_CMD_DESTROY_TCBD        = 20,   // to server: to destroy TCB descriptor (real TCB is located in CRAM )object in server local memory
    REX_CMD_ADD_DEP             = 21,   // to server: to add TCB depencencies
    REX_CMD_ADD_TO_LIST         = 22,   // to server: to add TCB block the task list (located in internal memory)
    REX_CMD_GET_LIST_INFO       = 23,   // to server: to questy information about task list
    REX_CMD_ADD_CELL_DEP        = 24,   // to server: to add new memory cell dependencies to the TCB
    REX_CMD_REINIT_CELLS        = 25,   // to server: to reinitialize the list of memory cells used like additional dependencies for TCB
    REX_CMD_DIAGNOSTICS         = 26,   // to server: diagnostics command specified by input/output buffers and sub-command ID
    REX_CMD_BAD_CEVA_DETECTED   = 27,   // to client: REX-SERVER has detected bad CEVAs, client(ARM code) has to decide what to do
    REX_CMD_RESTORED_CEVA       = 28,   // to server: REX-SERVER has to add to free pool CEVA restored by the client
    REX_CMD_CALL_CEVA_PROC      = 29,   // to server: REX-SERVER has to send command to CEVA(s) to call some CEVA procedure defined by address
    REX_CMD_CEVA_PROC_DONE      = 30,   // to client: REX-SERVER informs that CEVA call is completed and client may take the results
    REX_CMD_PRV_TCB             = 31,   // to server: to provide REX-S with new private TCB task, private TCB block does not belong to application
    REX_CMD_EXEC_LIST_SET       = 32,   // to server: to run the set of TCB lists

    REX_CMD_NUM,                        // the number of commands (client->server / server->client)

    REX_CMD_OPT_BLOCKING	= (1 << 28)

}REX_SERV_CMD;


typedef enum _REX_SERV_DIAG_CMD_
{
    REX_DIAG_CMD_CHECK_SYS_MDMA = 0,
    REX_DIAG_CMD_GET_STAT_INFO  = 1,   // to server: to query statistics information about server work
    REX_DIAG_CMD_CHECK_PRINT    = 2

}REX_SERV_DIAG_CMD;

typedef struct _REXSYSFPART_
{
    UINT32 *storage;
    UINT32 *freeblk;
    UINT32  blksize;
    UINT32  blkcnt;
    UINT32  AllocCnt;
} REXSYSFPART, *PREXSYSFPART;


typedef struct _REX_SERV_INIT_
{
    unsigned int        ctrl;           // control/configuration parameters provided by the ARM
    unsigned int        hw_mask;

    #ifdef REXS_POOLS_ENABLED
    unsigned int        hw_mask_pool_a;
    unsigned int        hw_mask_pool_b;
    #endif

    unsigned int        SubTaskNum;
    TCB_SUB_TASK_INFO * SubTaskInfo;

    void *              CommInfPtr;     // The pointer to communication interface, see: REX_SERV_COMM_INF
    void *              MLogPtr;        // The pointer to the MLog storage, added to keep REX-server data (mainly CEVA data)
    void *              TracePtr;
    unsigned int        TraceSize;
}RexCmdServInit;


typedef struct _REX_EXEC_LIST_
{
    void *          ListPtr;        // The pointer to the list object (REX::TASKLIST)
    UINT32          StartTicks;
}RexCmdExecList;

typedef struct _REX_LIST_IS_DONE_
{
    PTASKLIST       ListPtr;        // The pointer to the list object (REX::TASKLIST)
    UINT32          ListID;
    UINT32          CbDone;
    UINT32          CbDonePtr;
    UINT32          StartTicks;
    UINT32          NumBadTcb;

}RexCmdListIsDone;

typedef struct _REX_EXEC_TCB_
{
    TCB*                pTcb;
}RexCmdExecTcb;

typedef struct _REX_EXEC_TCB_CB_
{
    REXTCBCBDONE        CbDone;
    void        *       CbDonePtr;
    TCB*                pTcb;
    UINT32              nErrCode;

}RexCmdExecTcbCb;

typedef struct _REX_TCB_IS_DONE_
{
    TCB *           TcbPtr;         // The pointer to the TCB that has been done on client side
}RexCmdTcbIsDone;

typedef struct _REX_REINIT_LIST_
{
    TASKLIST    *   List;           // The pointer to the list that has to be reinitialized
}RexCmdReinitList;

typedef struct _REX_TCB_CTRL_
{
    TCB *           TcbPtr;         // The pointer to the TCB that has been configured
    unsigned int    TcbCtrl;        // The control bit mask
}RexCmdTcbCtrl;

typedef struct _REX_TCB_CTRL_EX_
{
    TCB **          TcbList;        // The list of pointers to the TCB that has to be configured
    unsigned int    TcbNum;         // The number of TCBs in the list
    unsigned int    TcbCtrl;        // The control bit mask
}RexCmdTcbCtrlEx;

typedef struct _REX_ERROR_
{
    unsigned int    ErrorCode;      // error code
    unsigned int    ErrorLine;      // used in asserts only
    unsigned int    ErrorData;

}RexCmdError;

typedef struct _REX_NEW_TTI_
{
    void *          MLogPtr;        // The pointer to the new MLog storage

}RexCmdNewTTI;

typedef struct _REX_CREATE_LIST_
{
    UINT32      ListID;             // In
    UINT32      ListPtr;            // Out

}RexCmdCreateList;

typedef struct _REX_REG_CBDONE_LIST_
{
    void *      ListPtr;
    void *      ListCbDone;
    void *      ListCbDoneParam;

}RexCmdRegCbDoneList;


typedef struct _REX_DESTROY_LIST_
{
    void *     ListPtr; // In
    TCB**      TcbArray;// the pointer to the array provided by ARM application to the REX SERVER to free TCBs located in the DDR/CRAM memory
    VUINT32    ArrNum;  // the number of this array 

}RexCmdDestroyList;

typedef struct _REX_CREATE_TCBD_
{
    UINT32      TaskID;
    UINT32      ResID;
    void *      pHwTcb;
    UINT32      CbDone;
    UINT32      CbDonePtr;
    UINT32      Quantum;

}RexCmdCreateTcbD;

typedef struct _REX_DESTROY_TCBD_
{
    void *      pTcbD;

}RexCmdDestroyTcbD;

typedef struct _REX_ADD_DEP_
{
    void *      pTcb;
    void *      pDepOnThis;

}RexCmdAddDep;

typedef struct _REX_ADD_TO_LIST_
{
    void *                  pList;
    void *                  pTcb;
    UINT32                  Prior;
    volatile unsigned int   ListID;

}RexCmdAddToList;

typedef struct _REX_GET_LIST_INFO_
{
    PTASKLIST       pList;
    UINT32          ListID;
    UINT32          Ctrl;
    UINT32          TcbNum;
    UINT32          TcbDone;
    UINT32          TcbError;

}RexCmdGetListInfo;

typedef struct _REX_ADD_CELL_DEP_
{
    TCB*        pTcb;
    UINT32      CellAddr;
    UINT32      CellInitVal;

}RexCmdAddCellDep;

typedef struct _REX_CELL_REINIT_
{
    UINT32 *        CellList;       // The array of cell addresses
    UINT32 *        NewCellList;    // The array of new cell addresses (or NULL)
    unsigned int    CellNum;        // The number of TCBs in the list
}RexCmdCellReinit;

typedef struct _REX_DIAG_
{
    UINT32      CmdID;
    void *      InputPtr;
    void *      OutputPtr;
    UINT32      InputSize;
    UINT32      OutputSize;

}RexCmdDiag;

typedef struct _REX_BAD_CEVAS_DET_
{
    UINT32      BadMask;            // REX-SERVER found bad CEVAs

}RexCmdBadCevaDet;

typedef struct _REX_BAD_CEVAS_REST_
{
    UINT32      RestMask;           // The mask of CEVAs that have been restored

}RexCmdBadCevaRest;

typedef struct _REX_CMD_CALL_CEVA_PROC_
{
    UINT32      Handle;
    UINT32      CevaProc;
    UINT32      CevaProcQ;      // The quantum of time (in timer ticks) the called function has to be completed or 0 (0-infinite)
    UINT32      ParamNum;
    UINT32      Param[REX_MAX_CEVA_PROC_PN];
}RexCmdCallCevaProc;

typedef struct _REX_CMD_CEVA_PROC_DONE_
{
    UINT32      CevaProc;
    UINT32      ProcRet;
    UINT32      Handle;

}RexCmdCevaProcDone;

typedef struct _REX_CMD_PRV_TCB_
{
    UINT32      nIndex;     // The index of private TCB [0..REX_MAX_PRIVATE_TCB_NUM)
    TCB*        pTcb;       // The pointer to allocated TCB (allocated on client side)
}RexCmdPrvTcb;

typedef struct _REX_EXEC_SET_LIST_
{
    TASKLIST*       ListPtr[4];        // The array of pointers to the list objects (REX::TASKLIST)
    UINT32          StartTicks;
}RexCmdExecSetList;

typedef union _REX_CMD_PARAM_
{
    RexCmdServInit      Init;
    RexCmdExecList      ExecList;
    RexCmdListIsDone    ListIsDone;
    RexCmdExecTcb       ExecTcb;
    RexCmdExecTcbCb     ExecTcbCb;
    RexCmdTcbIsDone     TcbIsDone;
    RexCmdReinitList    ReinitList;
    RexCmdTcbCtrl       TcbCtrl;
    RexCmdTcbCtrlEx     TcbCtrlEx;
    RexCmdError         TcbError;
    RexCmdNewTTI        NewTTI;
    RexCmdCreateList    CreateList;
    RexCmdRegCbDoneList RegCbDoneList;
    RexCmdDestroyList   DestroyList;
    RexCmdCreateTcbD    CreateTcbD;
    RexCmdDestroyTcbD   DestroyTcbD;
    RexCmdAddDep        AddDep;
    RexCmdAddToList     AddToList;
    RexCmdGetListInfo   ListInfo;
    RexCmdAddCellDep    AddCellDep;
    RexCmdCellReinit    ReinitCells;
    RexCmdDiag          Diag;
    RexCmdBadCevaDet    BadCevaDet;
    RexCmdBadCevaRest   BadCevaRest;
    RexCmdCallCevaProc  CallCevaProc;
    RexCmdCevaProcDone  CevaProcDone;
    RexCmdPrvTcb        PrvTcb;
    RexCmdExecSetList   ExecListSet;

}RexCmdParam;


/** @brief The REX command type definition, this command type is used
           to communicate between server and client parts of REX */

typedef struct _REX_CMD_
{
    unsigned int             CmdID;      // See: REX_SERV_CMD
    RexCmdParam              Param;      // The command parameters
    volatile unsigned short  Status;     // the command status set by server part
    volatile unsigned short  RC;         // the command result (error code, see: REX_SERV_RC_xxx) used in sync commands

}RexCMD;

typedef struct _REX_CMD_POOL_
{
    volatile unsigned int FreePut;       // The queue put index (free cmd blocks)
    volatile unsigned int FreeGet;       // The queue get index (free cmd blocks)

    volatile unsigned int ProcPut;       // The queue put index (already allocated command)
    volatile unsigned int ProcGet;       // The queue get index (already allocated command)

    unsigned int free_pool[REX_SERVER_CMD_NUM + 1];
    unsigned int proc_pool[REX_SERVER_CMD_NUM + 1];

    unsigned char * Storage;    // The pointer to the storage
    unsigned int    BlockSize;  // The block size in bytes (storage block)

}REX_CMD_POOL;


/** @brief The REX server <-> REX client communication interface
           it's used to send / receive REX commands
*/

typedef struct _REX_SERV_COMM_INF_
{
    REX_CMD_POOL  server[2];
    REX_CMD_POOL  client;

    RexCMD  server_pool_0 [REX_SERVER_CMD_NUM];
    RexCMD  server_pool_1 [REX_SERVER_CMD_NUM];
    RexCMD  client_pool   [REX_SERVER_CMD_NUM];

}REX_SERV_COMM_INF;

typedef struct _REX_SERV_CELL_
{
    struct _REX_SERV_CELL_* Next;       // The next allocated CELL or NULL
    struct _REX_SERV_CELL_* Prev;       // The previous allocated CELL or NULL

    UINT32  Status;                     // CELL status (see: REX_SERV_CELL_xxx)
    UINT32  Addr;                       // Cell Address
    UINT32  InitVal;                    // The initial value used to detect changes in CELL

    UINT32  TcbDNum;
    REXTCBD*TcbD[REXS_CELL_DEP_NUM];    // The list of TCBs that depends on this cell

}REX_SERV_CELL;

typedef struct _REX_SERV_STAT_INFO_
{
    UINT32      NumListRun;             // The total number of lists started to be processed
    UINT32      NumBadList;             // The total number of list incorrectly executed due to bad CEVAs
    UINT32      NumCevaRest;            // The number of CEVAs restored in a process of work
    UINT32      CevaBadDevMask;         // The number of bad CEVAs that was not restored

}REX_SERV_STAT_INFO;

/** @brief This type describes context of new type TCB dependencies
           that is based on memory cells set by HW accelerators */

typedef struct _REX_SERV_CELL_DEP_CTX_
{
    UINT32          Num;                    // The number of already used cells
    REX_SERV_CELL*  First;                  // The pointer to the first cell or NULL
    REX_SERV_CELL*  Last;                   // The pointer to the latest cell or NULL
    REX_SERV_CELL   Cells[REXS_CELL_NUM];   // The cell arrays

}REX_SERV_CELL_DEP_CTX;

typedef void (*RexServRunType)(LPVOID pRexServCtx, REXTCBD * pTcbD, TCB * pHwTcb);

/** @brief This is REX server part context, this type of data is used on
           CEVA (or in case of debugging on ARM) processors */

typedef struct _REX_SERV_CTX_
{
    REX_SERV_COMM_INF   *   pInf;               // The context of communication interface (client/server)
    TCB_SUB_TASK_INFO   *   SubTaskInfo;        // The pointer to the sub-task array (for all cevas)
    UINT32                  SubTaskNumPerCeva;  // The number of cells in array (SubTaskInfo) per each CEVA, see REXS_SUB_TASK_NUM

    REX_CMD_POOL*           serv_inf[2];
    REX_CMD_POOL*           client_inf;

    VUINT32                 InfLock;      // The communication lock (used on 4gmx only)
    void *                  MLogPtr;      // The pointer to the MLog storage allocated to keep REX-server data

    volatile unsigned int   hw_mask;            // The mask of accessible HW accelerators
    volatile unsigned int   hw_mask_pool_a;     // The special pool-a, used to process some TCBs specially on some HW accelerator
    volatile unsigned int   hw_mask_pool_b;     // The special pool-b, used to process some TCBs specially on some HW accelerator
    volatile unsigned int   hw_rst_mask;        // The mask of bad CEVA devices that was restarted by ARM, by default it's 0
    volatile unsigned int   hw_bad_mask;        // The bad devices detected by the system
    volatile unsigned int   hw_restored_num;    // The number of times when CEVAs were restored
    volatile unsigned int   hw_rst_dev_time[RES_SERV_HW_CEVA_NUM]; // the time when device was restarted

    volatile unsigned int   tcbd_proc_put;
    volatile unsigned int   tcbd_proc_get;
    REXTCBD*                tcbd_proc_queue [REX_SERVER_PROC_QUEUE];

    volatile unsigned int   tcbd_high_proc_queue_num;
    volatile unsigned int   tcbd_high_proc_put;
    volatile unsigned int   tcbd_high_proc_get;
    REXTCBD*                tcbd_high_proc_queue [REXS_CEVA_HIGH_PROC_QUEUE];

    unsigned int            NumListRun;
    unsigned int            NumBadList;

    RexServRunType          Res [REX_SUPPORTED_RES];

    UINT32                  hw_res_ticks  [RES_SERV_HW_CEVA_NUM];       // The array of ticks used to track CEVA tasks
    TCB*                    hw_res        [RES_SERV_HW_CEVA_NUM];       // HW CEVAs used to process TCB blocks
    REXTCBD*                hw_res_tcbd   [RES_SERV_HW_CEVA_NUM];       // TCB-Descriptoes of the TCBs routed to the CEVAs
    UINT32                  hw_res_num;                                 // The number of started tasks (loop optimization)

    TCB*                    hw_res_pool_a [RES_SERV_HW_CEVA_NUM];       // HW CEVAs used to process TCB blocks
    TCB*                    hw_res_pool_b [RES_SERV_HW_CEVA_NUM];       // HW CEVAs used to process TCB blocks

    volatile unsigned int   list_put;
    volatile unsigned int   list_get;
    TASKLIST*               proc_list  [REX_SERVER_LIST_NUM];

    REXSYSFPART             ListPart;   // The partition of the locally created list objects
    REXSYSFPART             TcbDPart;   // The partition of the locally created TCB descriptors

    PUINT32                 TracePtr;
    UINT32                  TraceSize;
    UINT32                  TraceOff;

    REX_SERV_CELL_DEP_CTX   CellCtx;

    REXTCBD*                IdftTcbdPtr;    // The pointer to the TCB routed to IDFT processing or NULL
    UINT32                  IdftTcbStart;   // The ticks when TCB task was started

    #if (REX_MAX_PRIVATE_TCB_NUM > 0)

    UINT32                  PrvFreeMask;
    TCB*                    PrvTcb[REX_MAX_PRIVATE_TCB_NUM];
    REXTCBD*                PrvTcbD[REX_MAX_PRIVATE_TCB_NUM];
    UINT32                  PrvTimeStart[REX_MAX_PRIVATE_TCB_NUM];

    VUINT32                 CevaCallStatus[4][REXS_CEVA_PROC_CALL_NUM]; // CEVA call handle value (used to check status of CEVA call)
    VUINT32                 CevaCallHandleIdx[4];                       // Free handle index (for each CPU)
    VUINT32                 CevaCallRetVal[4][REXS_CEVA_PROC_DONE_NUM]; // Return value of CEVA code
    CevaCallCbDoneType      CevaCallCbDone[4][REXS_CEVA_PROC_CALL_NUM];
    void *                  CevaCallCbDonePtr[4][REXS_CEVA_PROC_CALL_NUM];

    VUINT32                 CevaCallPut;
    VUINT32                 CevaCallGet;
    RexCmdCallCevaProc      CevaCall[REXS_CEVA_PROC_CALL_NUM];

    #endif

}REX_SERV_CTX;

#ifndef CEVA_PROJ

#define REX_SERV_INF_HEAP   DspHeap

/******************************************************************************
*                   4GMX interface used by 4GMX REX only                      *
******************************************************************************/

    /** @brief To initialize sub-server part, it has to be called on 4GMX
           to initialize communication with CEVA server part

        @param plpCfg [in] - the pointer to the REX configuration parameters

        @return [MXRC] an error code of operation */

    MXRC RexServInit (void * plpCfg);

    /** @brief This function turns off the REX-server part and called automatically from the REX
        @return [MXRC] an error code */

    MXRC RexServShutdown(void);

    /** @brief This function sends command to server part of REX to execute the
               task list defined by function parameter

        @param pList [in] - the pointer to the task list
        @return [MXRC] an error code of operation*/

    MXRC RexServExecListCmd (TASKLIST * pList);

    /** @brief This function sends command to server part of REX to execute the
               task list defined by function parameter

        @param pListX [in] - the pointer to the task list (or NULL)

        @return [MXRC] an error code of operation*/

    MXRC RexServExecList4Cmd (TASKLIST * pList0, TASKLIST * pList1, TASKLIST * pList2, TASKLIST * pList3);

    /** @brief This function sends confirmation to server part that TCB task was
           completed on client side (client is only one side where this TCB can be done)

        @param pTCB [in] - the pointer to the completed TCB

        @return [MXRC] an error code of operation*/

    MXRC RexServTcbIsDoneCmd (TCB * pTCB);


    /** @brief This function sets/resets TCB block control parameters like
                    - BYPASS
                    - LOCKED

        @param nSet [in] - 1 - to set, 0 - to reset
        @param pTCB [in] - the pointer to the completed TCB
        @param mask [in] - the bit mask, see: REX_BYPASS_TCB | REX_LOCKED_TCB

        @return [MXRC] an error code of operation*/

    MXRC RexServTcbCtrlCmd (UINT32 nSet, TCB * pTCB, UINT32 mask);

    /** @brief This function sets/resets the list of TCB blocks control parameters like
                - BYPASS
                - LOCKED

        @param nSet     [in] - 1 - to set, 0 - to reset
        @param ppTCB    [in] - the pointer to the completed TCBs
        @param nTcbNum  [in] - the number of TCBs in the array
        @param mask     [in] - the bit mask, see: REX_BYPASS_TCB | REX_LOCKED_TCB

        @return [MXRC] an error code of operation*/

    MXRC RexServTcbCtrlExCmd (UINT32 nSet, TCB ** ppTCB, UINT32 nTcbNum, UINT32 mask);

    /** @brief This function sends command to server part of REX to reinitialize
               task list, it saves time on client part

        @param pList [in] - the pointer to the task list

        @return [MXRC] an error code */

    MXRC RexServReinitListCmd(TASKLIST * pList);

    MXRC RexServShowTrace(UINT32 u32num);

    /** @brief This function sends to the server message about new TTI with pointers
               to the MLog storages and other information

        @return [MXRC] an error code */

    MXRC RexServNewTTICmd(void);

    /** @brief This function sends message to the REX-S component about uploading
               created Mlog information

        @return [MXRC] an error code */

    MXRC RexServUploadMLogCmd(void);

    /** @brief This function sends the request to the server part
               and returns list 'descriptors' (the pointer to some location)
               this location cannot be accessed directly

        @param ListID [in] - the list ID

        @return [PTASKLIST] the pointer to the list descriptor*/

    PTASKLIST RexServCreateListCmd (UINT32 ListID);

    /** @brief This function sends the request to the server part
           locally created object will be destroyed

        @param pList [in] - The pointer to the list object
        @param pArr  [in] - the pointer to the array of TCBs that should be free on ARM side
        @param pnNum [in] - in: max number of elements in the array, 
                           out: number of elements placed by the REX-SERCER

        @return [MXRC] the error code of operation */

    MXRC RexServDestroyListCmd(PTASKLIST pList, TCB ** pArr, UINT32*pnNum);

    /** @brief This function sets pointer to the callback function that will be
           called on ARM side when task list is done

        @param pList [in] - the pointer to the list object (see: RexServCreateList)
        @param cbDone[in] - the pointer to ARM procedure
        @param lpData[in] - the procedure parameter

        @return [MXRC] an error code */

    MXRC RexServRegCBDoneCmd(PTASKLIST pList, REXCBDONE cbDone, LPVOID lpData);

    MXRC RexServCreateTCBDCmd(UINT32 TaskID, UINT32 ResID, REXTCBCBDONE cbDone, LPVOID pPtr, TCB * pHwTcb, UINT32 quantum);
    MXRC RexServDestroyTCBDCmd(REXTCBD* pTcbD);
    MXRC RexServAddTcbDepCmd(TCB * pTcb, TCB * pDepOnThis);
    MXRC RexServAddToListCmd(TASKLIST * pList, TCB * pTcb, UINT32 nTcbPrior, UINT32 * plist_id);

    MXRC RexServGetListInfoCmd (TASKLIST * pList, RexCmdGetListInfo* pInfo);
    UINT32 RexServGetListIDCmd(TASKLIST * pList);

    /** @brief This function is used to associate TCB with memory cell,
           this is specific dependencies that is used to handle HW accelerators
           when CELL value is 1, REX will treat this dependencies like 'done' and
           TCB can be scheduled if no other dependencies

    @NOTE!
            This functionality is applicable for REX-SERVER mode only (CEVA or ARM-emulator)

    @param  pTcb [in] - the pointer to the TCB block accosiated with memory CELL
    @param  nAddr[in] - the CELL address (it cannot be NULL)
    @param  nInit[in] - the initial value for this memory cell

    @return [MXRC] an error code of operation */

    MXRC RexServAddCellDepCmd(TCB * pTcb, UINT32 nAddr, UINT32 nInit);

    /** @brief This function is used to reinitialize cells assigned to TCB blocks

    @NOTE!
            This functionality is applicable for REX-SERVER mode only (CEVA or ARM-emulator)

    @param  pCellList   [in] - the array of cell addresses
    @param  pNewCellList[in] - the array of new cell addresses (it can be NULL)
    @param  nNum        [in] - the array element number

    @return [MXRC] an error code of operation */

    MXRC RexServReinitCellsCmd(UINT32* pCellList, UINT32* pNewCellList, UINT32 nNum);

    /** @brief This diagnostics API is designed to run some diagnostics code on ARM/CEVA
               REX-SERVER, and used to check some functionality, not designed to be used
               in general application

        @param nCmdID   [in]   - the diagnostics API ID
        @param pInPtr   [in]   - the input buffer for diag API
        @param InSize   [in]   - the size in bytes of input buffer
        @param pOutPtr  [out]  - the output buffer for diag API
        @param OutSize  [in]   - the size in bytes of output buffer

        @return [MXRC] an error code of operation */

    MXRC RexServDiagCmd (UINT32 nCmdID, void*pInPtr, UINT32 nInSize, void*pOutPtr, UINT32 nOutSize);

    char * RexServGetErrorDescr(UINT32 code);

    /** @brief This function sends to REX-SERVER information about restored
               CEVAs that can be used again to process TCBs

        @param nGoodMask[in] - the mask of good processors restored on ARM

        @return [MXRC] an error code */

    MXRC RexServNotifyRestCevaCmd(UINT32 nGoodMask);

    void RexServMmuCfg(UINT32 nCpuID, UINT32 nParam);

    /** @brief This function sends request to CEVA code to call some CEVA code function,
       defined by nProcAddr.

        @param CbDone    [in]       - the pointer to the ARM CB-DONE function
        @param Ptr       [in]       - the pointer to the cbDone argument
        @param nProcAddr [in]       - the address of CEVA function
        @param quantum   [in]       - the quantum of time CEVA function has to be completed (in microseconds) 0 - quantum is turned off
        @param nParamNum [in]       - the number of parameters
        @param pParamArr [in]       - the CEVA function parameters, optional and depends on CEVA function

    @return [UINT32] the handle of CEVA call or NULL */

    UINT32 RexServCallCevaProc(CevaCallCbDoneType CbDone, void * Ptr, UINT32 nProcAddr, UINT32 quantum, UINT32 nParamNum, UINT32 * pParamArr);

    /** @brief This function waits for CEVA call completion

        @param handle   [in]  - the handle of CEVA call returned by RexServCallCevaProc function
        @param nTimeInUs[in]  - the time in microseconds (to wait completion), 0 - to check and return
        @param pRet     [in]  - the pointer to the variable to store CEVA procedure return value

        @return [MXRC] an error code of operation*/

    MXRC RexServWaitCallCevaDone(UINT32 handle, UINT32 nTimeInUs, UINT32 * pRet);

#else  //_4GMX_CONFIG_H_

/******************************************************************************
*                   CEVA specific sinterface used by REX-server only          *
******************************************************************************/

/** @brief This function sets pointer to the communication interface
			the interface is specified by the host

	@param pInf [in] - the pointer to the interface */

void RexServSetInterface (void * pInf);

#endif //_4GMX_CONFIG_H_


/** @brief The REX server kernel, this code processes TCB/TCBD blocks
           and makes possible to complete the list(s), also it communicates with
           client part of REX located on other platform

*/
void RexServKernel (UINT32 nCheckComm);

void RexServPreInit(void);

void RexServSendErrorEx(unsigned int code, unsigned int data);

#endif //_REX_SERV_H_

#ifdef __cplusplus
}
#endif


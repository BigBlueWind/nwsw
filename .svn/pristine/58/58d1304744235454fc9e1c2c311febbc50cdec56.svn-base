//-------------------------------------------------------------------------------------------
/** @file tcbscheduler.h
 *
 * @brief TCB scheduler (TrEx) definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.19 $
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

#ifdef __CC_ARM     // ARM compiler
#include "config.h"
#endif

#ifndef _TCBSCHEDULER_H_
#define _TCBSCHEDULER_H_

//redefine null to get around warnings once we changed the APIs parmater from Void * to UN32
#undef NULL
#define NULL 0

#ifndef CEVA_INTRINSICS
#include "mlog.h"
#endif

#include "typedef.h"
#include <string.h>

#if defined(_WIN32) //only use if windows
#include <stdio.h>

#include <stdlib.h>
#include <time.h>
#include <math.h>
//file to print TCB execution flow
extern FILE* fileTCBEXE;
#else
#if defined (FPGAHW)//FGPA
//for MLOG funcitons if FPGA no support yet... NOP
#define MLogOpen(filename)
#define MLogClose()
#define MLogAdd(  id, param)
#define MLogTCBBefore(ptcb)
#define MLogTCBAfter(ptcb)
#define MLogTCB(x)
#define MLogEXEStart(x,y)
#define MLogEXEFinish(x)
#define MLogEXEFinish2(a,b)
#define MLogDepends(x,y,z)
#else //evm

#if !defined(CEVA_INTRINSICS)  //avoid including this file when building a ceva project, else add it
#include "heaps.h"
#include "4gmx_kernel.h"
#endif

//if defined then TCB structure in CACHABLE memory, else TCBs are in NCNB
#define _TCB_CACHED

#endif // FPGAHW

#endif // _WIN32

#include "tcb.h"
#include <stdarg.h> //used by addDependency

//   Types definitions
typedef short Word16;
typedef long Word32;
typedef double Word40;
#ifndef CEVA_INTRINSICS
typedef int Flag;
#endif
//typedef long			S32;
typedef unsigned long int UN32;

//define values that set autoDependency will take
//global var set to 1 by default so that Trex discover intertask dependecies,
//if =0 the application has this responsibility using AddDependecny Api
//if=2 is equal to 1 plus it will print the intertask dependency on screen.
#define AUTO_ON    1
#define AUTO_OFF   0
#define AUTO_ON_PRINT 2

//defines to be used for TaskControl.. try to encode as ONE HOT
#define BLOCKING_TCB	0x1  //if tcb is marked as such, scheduler will return of list exe
#define BYPASS_TCB		0x2  //if tcb is marked as such, scheduler will delink this tcb (ie not executed it) and mark it as complete

//task list ID definitions
#define TASKLISTID_RSRV_0       0
#define TASKLISTID_RSRV_1       1
#define TASKLISTID_WIMAX_DL     2
#define TASKLISTID_WIMAX_UL     3
#define TASKLISTID_LTE_DL       4
#define TASKLISTID_LTE_UL       5

//superTCB GroupID
#define GID_RSRV                0xFFFF

//error codes
#define TCB_OK                  0
#define TRX_OK                  0
#define TCB_ERR                 1
#define TCB_NULL                2
#define TCB_ERR_NOTDEF          3
#define TCB_COMPLETE            4
#define TRX_PROCESSED           5

#define REGHAL_ERR              5
#define TRX_NODSP               6   //no dsp resource available
#define TRX_ERR                 7   //general error
#define TCB_WARN                9   //warning, not criticlal but noteworthy
#define TCB_IGNORE_ADD          10
#define TRX_ERR_LISTADD         11
#define TCB_ERR_BADIO           12

#define TCBQ_OK                 0
#define TCBQ_ERR                1
#define TCBQ_DONE               2

//scheduler debug levels TWO Bytes
//LSB byte
#define TRX_DBG_NONE            0x1  //no data logging
#define TRX_DBG_NODATA          0x1  //no tcb data logging but will still have dependency and exe flow ( ie profiling)
#define TRX_DBG_NOPROF          0x0  //no profiling what so ever (no tcb data and no dep and exe flow)
#define TRX_DBG_PROFDATA        0x2  //both profiling and data are ON
// MSB byte: TCBDATA bit definition  2 bytes , one bit per type,start with MSB[ARM: CEVA:FEC:MAP]
#define TRX_DBG_TCBDATA_ARM     (0x10|TRX_DBG_PROFDATA) //only arm tasks will be printed
#define TRX_DBG_TCBDATA_CEVA    (0x40|TRX_DBG_PROFDATA) //only ceva tcbs are printed
#define TRX_DBG_TCBDATA_MAP     (0x20|TRX_DBG_PROFDATA) //only MAP or FP
#define TRX_DBG_TCBDATA_FEC     (0x80|TRX_DBG_PROFDATA) //only FEC Tcbs are printed
//all tcbs are printed with profiling
#define TRX_DBG_TCBDATA         (TRX_DBG_PROFDATA|TRX_DBG_TCBDATA_ARM|TRX_DBG_TCBDATA_CEVA|\
                                 TRX_DBG_TCBDATA_MAP|TRX_DBG_TCBDATA_FEC)  //all tcbs

//scheduling policies
#define TRX_POLICY_RSRV         0x0
#define TRX_POLICY_FALI         0x1 // resource allocated as first available lowest index.
#define TRX_POLICY_RR           0x2 // resourcs allocated as round robin, this policy results in
                                    // predictable allocation
#define TRX_POLICY_LB           0x3 // load balance the tasks (mips) on resources, this policy also
                                    // results in predictable allocation

#define ReadyListRunning        2
#define ReadyListDONE           1
#define ReadyListBUSY           0

#define TASKLISTINDEX_DEFAULT   0xff

#define MAX_NUM_DSP             10
#define PU_READY                1
#define PU_BUSY                 0
#define PU_UNUSED               0xFF

//copied from fec.h
#define FEC_HWCONF_WIMAX        0x00000001
#define FEC_HWCONF_LTE          0x00000000

#define CEVA_0	                0
#define CEVA_1	                1
#define CEVA_2	                2
#define CEVA_3	                3
#define CEVA_4	                4
#define CEVA_5	                5
#define CEVA_6	                6
#define CEVA_7	                7
#define CEVA_8	                8
#define CEVA_9	                9

#define FP_0                    0
#define FP_1                    1
#define FP_2                    2
#define FP_3                    3
#define FP_4                    4
#define FP_5                    5
#define FP_6                    6
#define FP_7                    7
#define FP_8                    8
#define FP_9                    9

#define MAX_ACTIVE_LIST         16

//fucntion type to be used with RegARM Task
typedef void functionARM(TCB *);

//map task id to an physical address, currently only for arm tasks
extern functionARM *gTaskID[];

//task list number which maps to predefined list that can be read from an offline
#define	TASKLIST_TEST           0
//max mips for a ceva dsp
#define CEVA_MAX_MIPS           600

typedef struct tSchedInit
{
    U8 FECMode;      //used for initializing FEC: LTE or WIMAX, FEC_HWCONF_WIMAX or FEC_HWCONF_LTE
    U8 CEVAPool;     //number of ceva to use in this job
    U8 FPPool;       //number of fps to use in this job
    U8 SerialExe;    //if set to 1, run the list as a serial in the order of the tasks added to list
    U8 SchedPolicy;  //pertains to scheduling policy
    U8 DebugLevel;   //0x1= print contents of TCBs
    U8 BlockingAPI;  //0=NON-blocking , 1= block: task list exe API call will be blocked until list
                     // finishes execution, turn on for multlist
    U16 CEVAIDS[10]; //10 locations, must be used consequtively, a PU_UNUSED location terminates the search
    //At a minimum: location [0] must be populated with a ceva number, (does not have to be 0)
    //only the populated elements will have tasks scheduled on them,
    //eg if u want to use ceva5, and 8 only then you SHOULD cevaids[0]=5, cevaids[1]=8
    // note that the number of elements occupied must equal the CEVAPool Number
    U16 FPIDS[10];   //same as above with FPPool inidicating total number of occupied cells
    U8 CEVAID_RR;    //remember last ceva id in round robin
} SCHEDSTRC, *PSCHEDSTRC;

typedef struct tTaskList TASKLIST;
typedef struct tTaskList *PTASKLIST;

typedef void fListDoneCB(TASKLIST* thisList, void* param);

struct tTaskList
{
    U16 TaskListID;
    volatile U16 TaskListStatus; //done or inproces
    struct tTCB *TaskListHead; //the first TCB in a linked list of TCBs
    struct tTCB *TaskListTail; //the last TCB in a linked list of TCBs , also the first in reverse linklist
    U8 TaskListIndex; //used for active multi-list mgmnt
    U8 TaskListDestroyFlag; //if set means destroy task list when done
    UN32 checkin;
    UN32 checkout;
    fListDoneCB *TaskListDoneCB; //pointer to callback funciton
    void* TaskListDoneCBParam; //pointer to a paramater used by call back func
    SCHEDSTRC TaskListConfig; // a struct to keep the scheduler configuratin used for this list
};

typedef struct tServiceTaskList
{
    volatile PTASKLIST ActiveTaskList[MAX_ACTIVE_LIST]; //save pointer to the task list created
    volatile UN32 numActiveTaskList; //how many task lists are registered
} ServiceTaskList, *PServiceTaskList;

typedef struct tDepList
{
    struct tTCB *depTCB;
    struct tDepList *Next;
} DEPLIST, *PDEPLIST;

//temp structure to work with phy control code and the ceva
typedef struct tCEVAThread
{
    TCB *pTCB; //current tcb job
    int dspnum;
    void* pCtx; //TODO, may not be required later ....ceva context
} CEVAFN, *PCEVAFN;

//add a priority Que structure for putting tcbs in a priority order
#define PQDAT struct tTCB *
#define PQPRIO(pTCB) (pTCB->TaskPri)

#if defined(_WIN32) //only use if windows
#define PRIOR_QUEUE
#endif

struct pqueue
{
#ifdef PRIOR_QUEUE
    int size; // current size of this que
    int avail; //max  available slots
    PQDAT *d;
#else
    UN32 size; // current size of this que
    UN32 get; //max  available slots
    UN32 put; //max  available slots

    PQDAT *d;
#endif
};

/*macro version, used for dsp code */
#define EXTRACTTCBIOBUF(tcb, ionum)     (*((UN32 *)tcb->IOControlPtr+ (ionum*2)))
#define EXTRACTTCBIOBUFSZ(tcb, ionum)   ((*((UN32 *)tcb->IOControlPtr+ (ionum*2)+1))&(0x00ffffff))
#define TRX_MAX(a, b)           ((a) < (b) ? (b) : (a))
extern UN32 gDepDiscovery;
#define SetDepDiscovery(val)     (gDepDiscovery = val)

//extern PTCB pTask[MAX_NUM_TCB];

#define IsEmpty(Q) ((Q)->Size == 0)
#define IsFull(Q) ((Q)->Size == (Q)->Capacity)

extern PTASKLIST CreateTaskList(U16 TaskListID, UN32* Rsrv);
extern UN32 AddTCBToList(PTCB pTCB, PTASKLIST pTaskList, UN32 taskPri);
extern UN32 ExeTSchedList(TASKLIST* thisList);
extern UN32 ExeTSchedSingle(TCB* thisTCB);
extern UN32 MarkTCBControl(PTCB myTCB, UN32 val, void * RSRV);

extern PTCB CreateTCB(UN32 TaskID, UN32 ResourceID, UN32* pContextData, UN32 ContextDataLen,
                      void* CallBkFn, UN32 taskmips);
extern UN32 CreateTCBIOBuf(PTCB pTCB, UN32 NumDesc);
extern UN32 FillTCBIOBuf(PTCB pTCB, void* pData, UN32 DataLen, UN32 DataType, void* Rsrv);
extern UN32 DestroyTaskList(PTASKLIST thisList);
extern UN32 InitTSched(SCHEDSTRC *);
extern UN32 ShutdownTSched(void);
extern UN32 TSchedGetMasterCpu (void);
extern UN32 extractTCBIOBuf(PTCB pTCB, UN32 ioNum);
extern UN32 DestroyTCB(PTCB pTCB);
extern UN32 AddDependency(PTCB pTargetTCB, U16 numargs, ...);
extern UN32 RemoveDependency(PTCB pTargetTCB, U16 numargs, ...);
extern UN32 RegARMTask(UN32 taskid, functionARM* fnptr);
extern UN32 RegHALFunc(UN32 resourceid, void* fnptr);
extern UN32 FillTCBSingleInOut(PTCB pTCB, UN32* pInData, UN32 InDataLen, UN32* pOutData,
                               UN32 OutDataLen);
extern UN32 IsDepConflict(PTCB pTargetTCB, PTCB pPreTCB);
extern void * AddSuperTCB(U16 GID, void* GroupHandle, void * Rsrv, U16 numTCB, ...);
extern UN32 InitTaskList(PTASKLIST thisList, SCHEDSTRC* config);
extern UN32 ReinitTaskList(PTASKLIST thisList, void * RSRV);
extern UN32 ReFillTCBIOBuf(PTCB pTCB, void* pData, UN32 DataLen, UN32 DataType, UN32 IONum);
extern UN32 RegListDoneCB(TASKLIST* thisList, fListDoneCB* fnptr, void* param);

typedef struct _CTEX_PU_STRUC
{
    UN32 ready; //ready=0, busy=1;
    TCB *pTCB; //current tcb job
    U16 dspnum;
    int freeMIPS; //mips currently free , can go negative a little
} PUCTX;



#endif // _TCBSCHEDULER_H_


#ifdef __cplusplus
}
#endif


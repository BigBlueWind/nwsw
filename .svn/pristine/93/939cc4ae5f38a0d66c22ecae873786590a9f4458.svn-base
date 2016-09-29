//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Rexcomp.h
 *
 * @brief REX and TrEX compatibility
 * @author Mindspeed Technologies
 * @version $Revision: 1.16 $
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

#include "config.h"

#ifndef _REX_COMP_H_
#define _REX_COMP_H_

#include <string.h>
#include "tcb.h"
#include "rex.h"
#include "appprintf.h"
#include "devinfo.h"

#define _TCB_CACHED
#define MAX_NUM_DSP  10


#define TRX_DBG_NONE            0x1  //no data logging
#define TRX_DBG_NODATA          0x1  //no tcb data logging but will still have dependency and exe flow ( ie profiling)
#define TRX_DBG_NOPROF          0x0  //no profiling what so ever (no tcb data and no dep and exe flow)
#define TRX_DBG_PROFDATA        0x2  //both profiling and data are ON

//scheduling policies
#define TRX_POLICY_RSRV         0x0
#define TRX_POLICY_FALI         0x1 // resource allocated as first available lowest index.
#define TRX_POLICY_RR           0x2 // resourcs allocated as round robin, this policy results in
                                    // predictable allocation
#define TRX_POLICY_LB           0x3 // load balance the tasks (mips) on resources, this policy also
                                    // results in predictable allocation

#define CEVA_0                  0
#define CEVA_1                  1
#define CEVA_2                  2
#define CEVA_3                  3
#define CEVA_4                  4
#define CEVA_5                  5
#define CEVA_6                  6
#define CEVA_7                  7
#define CEVA_8                  8
#define CEVA_9                  9

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

#define TCB_OK                  0
#define TRX_OK                  0

#define AUTO_ON    1
#define AUTO_OFF   0

//defines to be used for TaskControl.. try to encode as ONE HOT
#define BLOCKING_TCB    0x1  //if tcb is marked as such, scheduler will return of list exe
#define BYPASS_TCB      0x2  //if tcb is marked as such, scheduler will delink this tcb (ie not executed it) and mark it as complete

//   Types definitions
typedef short Word16;
typedef long Word32;
typedef double Word40;
#ifndef CEVA_INTRINSICS
typedef int Flag;
typedef unsigned char U8;
typedef unsigned short U16;
typedef short S16;
#endif

typedef void functionARM(TCB *);
typedef void fListDoneCB(TASKLIST* thisList, void* param);

typedef struct tSchedInit
{
    UINT8 FECMode;      //used for initializing FEC: LTE or WIMAX, FEC_HWCONF_WIMAX or FEC_HWCONF_LTE
    UINT8 CEVAPool;     //number of ceva to use in this job
    UINT8 FPPool;       //number of fps to use in this job
    UINT8 SerialExe;    //if set to 1, run the list as a serial in the order of the tasks added to list
    UINT8 SchedPolicy;  //pertains to scheduling policy
    UINT8 DebugLevel;   //0x1= print contents of TCBs
    UINT8 BlockingAPI;  //0=NON-blocking , 1= block: task list exe API call will be blocked until list
                     // finishes execution, turn on for multlist
    UINT16 CEVAIDS[10]; //10 locations, must be used consequtively, a PU_UNUSED location terminates the search
    //At a minimum: location [0] must be populated with a ceva number, (does not have to be 0)
    //only the populated elements will have tasks scheduled on them,
    //eg if u want to use ceva5, and 8 only then you SHOULD cevaids[0]=5, cevaids[1]=8
    // note that the number of elements occupied must equal the CEVAPool Number
    UINT16 FPIDS[10];   //same as above with FPPool inidicating total number of occupied cells
    UINT8 CEVAID_RR;    //remember last ceva id in round robin
} SCHEDSTRC, *PSCHEDSTRC;


///////////////////////////////////////////////////////////////////////////////
///                         TrEX API definition                             ///
///////////////////////////////////////////////////////////////////////////////

#define SetDepDiscovery(val) RexSetDepDiscovery (val)

#define EXTRACTTCBIOBUF(tcb, ionum)     (*((UINT32 *)tcb->IOControlPtr+ (ionum*2)))
#define EXTRACTTCBIOBUFSZ(tcb, ionum)   ((*((UINT32 *)tcb->IOControlPtr+ (ionum*2)+1))&(0x00ffffff))

UINT32 INLINE MarkTCBControl(PTCB myTCB, UINT32 val, void * RSRV)
{
    return RexMarkTCBControl (myTCB, val);
}

UINT32 InitTSched(SCHEDSTRC * p);

PTASKLIST INLINE CreateTaskList(U16 TaskListID, UINT32* Rsrv)
{
    return RexCreateTaskList (TaskListID);
}

UINT32 INLINE AddTCBToList(PTCB pTCB, PTASKLIST pTaskList, UINT32 taskPri)
{
#ifndef REX_TCB_PRIORITY_ENABLED
    _ASSERT (taskPri == 0);
#endif
    return RexAddTCBToList (pTaskList, pTCB, taskPri);
}

UINT32 ExeTSchedList(TASKLIST* thisList);

UINT32 ExeTSchedSingle(TCB* thisTCB);

PTCB INLINE CreateTCB(UINT32 TaskID, UINT32 ResourceID, UINT32* pContextData, UINT32 ContextDataLen,
                      void* CallBkFn, UINT32 cbDonePtr)
{
    //_ASSERT (taskmips == 0);
    //_ASSERT (CallBkFn == NULL);

    return RexCreateTCB (TaskID, ResourceID, (UINT32*)pContextData, ContextDataLen, (REXTCBCBDONE)CallBkFn, (LPVOID)cbDonePtr);
}

UINT32 INLINE CreateTCBIOBuf(PTCB pTCB, UINT32 NumDesc)
{
    return RexCreateIOBuf (pTCB, NumDesc);
}
extern BOOL SysCoreIsDCached (UINT32 nAddr);

UINT32 INLINE FillTCBIOBuf(PTCB pTCB, void* pData, UINT32 DataLen, UINT32 DataType, void* Rsrv)
{

#ifdef DEBUG_CEVA_CACHEABLE_IO
    if (pTCB->ResourceID == RESOURCE_CEVA || pTCB->ResourceID == RESOURCE_MAP)
    {
        if (SysCoreIsDCached((UINT32)pData))
        {
            uart_printf("IOBuf[%d] for Task[%d] is allocated in Cacheable DDR\r\n", (TCB_CONTROL_LEN(pTCB) / sizeof (TIOBufDesc))+1, pTCB->TaskID);
        }
    }
#endif

#if 0
    UINT32 lr = __return_address () - 4;

    if (pData == NULL && DataLen!=0)
    {
        uart_printf ("FILL_TCB_IO ERROR FOUND!!!, lr=%x,  type=%d, len=%d\r\n", lr, DataType, DataLen);
        while (1);
    }
#endif

    _ASSERT (Rsrv == 0);

    return RexFillTCBIOBuf (pTCB, pData, DataLen, DataType);
}

UINT32 INLINE DestroyTaskList(PTASKLIST thisList)
{
    return RexDestroyTaskList (thisList);
}

UINT32 INLINE ShutdownTSched(void)
{
    return RexShutdown();
}

UINT32 INLINE TSchedGetMasterCpu (void)
{
    return RexGetMasterCpu ();
}

LPVOID INLINE extractTCBIOBuf(PTCB pTCB, UINT32 ioNum)
{
    return RexGetIOBuf(pTCB, ioNum);
}

UINT32 INLINE DestroyTCB(PTCB pTCB)
{
    return RexDestroyTCB (pTCB);
}

UINT32 INLINE AddDependency(PTCB pTargetTCB, U16 numargs, TCB * pTcb)
{
    return RexAddDep (pTargetTCB, pTcb);
}

UINT32 INLINE RegARMTask(UINT32 taskid, functionARM* fnptr)
{
    return RexRegArmTask (taskid, fnptr);
}

#define AddSuperTCB(...) RexAddSuperTcb (__VA_ARGS__)

#define InitTaskList(a, b) 0

UINT32 INLINE ReinitTaskList(PTASKLIST thisList, void * RSRV)
{
    _ASSERT (RSRV == NULL);
    return RexReinitList (thisList);
}

UINT32 INLINE ReFillTCBIOBuf(PTCB pTCB, void* pData, UINT32 DataLen, UINT32 DataType, UINT32 IONum)
{
    return RexReFillTCBIOBuf (pTCB, pData, DataLen, DataType, IONum);
}

UINT32 INLINE RegListDoneCB(TASKLIST* thisList, fListDoneCB* fnptr, void* param)
{
    return RexRegListCBDone (thisList, fnptr, param);
}

#endif //_REX_COMP_H_

#ifdef __cplusplus
}
#endif


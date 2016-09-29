//-------------------------------------------------------------------------------------------
/** @file rex.c
 *
 * @brief The implementation of the syscore RapidEXecutive (REX)
 * @author Mindspeed Technologies
 * @version $Revision: 1.81 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "rex.h"
#include "fpdrv.h"
#include "DSPdrv.h"
#include "icdrv.h"
#include "schtaskdrv.h"
#include "appprintf.h"
#include "mlog.h"
#include "clkrst.h"
#include "hal.h"
#include "rex_serv.h"

#define REX_SHOWLIST                (1 << 0)
#define REX_ADDSCHEDULING           (1 << 1)
#define REX_ADD_DEP                 (1 << 2)
#define REX_PROC_DEP                (1 << 3)
#define REX_CEVA_RUN                (1 << 4)
#define REX_CEVA_DONE               (1 << 5)
#define REX_FP_RUN                  (1 << 6)
#define REX_FP_DONE                 (1 << 7)
#define REX_ARM_RUN                 (1 << 8)
#define REX_ARM_DONE                (1 << 9)
#define REX_KERNEL_RUN              (1 << 10)
#define REX_IC_RUN                  (1 << 11)
#define REX_CREATE_TCB              (1 << 12)
#define REX_DESTROY_TCB             (1 << 13)
#define REX_TCB_CTRL                (1 << 14)
#define REX_TCB_CREATE_TASK_LIST    (1 << 15)
#define REX_DESTROY_TASK_LIST       (1 << 16)
#define REX_RUN_TASK_LIST           (1 << 17)
#define REX_ADD_TO_LIST             (1 << 18)
#define REX_REINIT_LIST             (1 << 19)
#define REX_ADD_SUPER_TCB           (1 << 20)
#define REX_PROC_LIST               (1 << 21)
#define REX_DISCOVER_DEP            (1 << 22)
#define REX_PROC_LIST_DONE          (1 << 23)
#define REX_CALL_CBDONE             (1 << 24)
#define REX_BYPASS_PROCESSING       (1 << 25)
#define REX_EMPTY_LIST_DETECTED     (1 << 26)
#define REX_CEVA_DONE_REGLIST       (1 << 27)
#define REX_FP_DONE_REGLIST         (1 << 28)
#define REX_ARM_DONE_REGLIST        (1 << 29)
#define REX_LIST_COMPLETED          (1 << 30)
#define REX_LIST_RESCHED_IS_DET     ((UINT32)1 << 31)

const UINT32 rex_dbg_msk =  0                           |
                            //REX_RUN_TASK_LIST           |
                            //REX_CREATE_TCB              |
                            //REX_DESTROY_TCB             |
                            //REX_TCB_CREATE_TASK_LIST    |
                            //REX_DESTROY_TASK_LIST       |
                            REX_SHOWLIST                |
                            //REX_PROC_LIST               |
                            //REX_CEVA_RUN                |
                            //REX_CEVA_DONE               |
                            //REX_FP_RUN                  |
                            //REX_FP_DONE                 |
                            //REX_ARM_RUN                 |
                            //REX_ARM_DONE                |
                            //REX_DISCOVER_DEP            |
                            //REX_PROC_LIST_DONE          |
                            //REX_REINIT_LIST             |
                            //REX_IC_RUN                  |
                            REX_CALL_CBDONE             |
                            //REX_CEVA_DONE_REGLIST       |
                            //REX_FP_DONE_REGLIST         |
                            //REX_ARM_DONE_REGLIST        |
                            REX_LIST_COMPLETED          |
                            //REX_BYPASS_PROCESSING       |
                            //REX_LIST_RESCHED_IS_DET     |
                            REX_EMPTY_LIST_DETECTED     |
                            0;

//#define dbg_printf(x, ...) if (rex_dbg_msk & (x)) {uart_printf (__VA_ARGS__); MxDelayTicks(TICKS_TO_MS(20));}
#define dbg_printf(...)
#define rex_printf(...) uart_printf (__VA_ARGS__); MxDelayTicks(TICKS_TO_MS(20));

#ifdef REX_PROF_ENABLED

#define REX_PROF_DEF_TIME() UINT32 RexProfTime = GetTIMETICK();
#define REX_PROF_DEF_TIME_EX() UINT32 RexProfTimeEx=0, RexProfTime = GetTIMETICK();
#define REX_PROF_ADD_TIME(id) RexAddProf (id, GetTIMETICK()-RexProfTime);
#define REX_PROF_ADD_TIME_EX(id) RexAddProf (id, (GetTIMETICK()-RexProfTimeEx));
#define REX_PROF_START_TIME_EX() RexProfTimeEx = GetTIMETICK();
#define REX_PROF_END_TIME_EX() RexProfTimeEx = GetTIMETICK() - RexProfTimeEx;


#else

#define REX_PROF_DEF_TIME()
#define REX_PROF_DEF_TIME_EX()
#define REX_PROF_ADD_TIME(id)
#define REX_PROF_ADD_TIME_EX(id)
#define REX_PROF_START_TIME_EX()
#define REX_PROF_END_TIME_EX()

#endif


// INLINE directive should be turned off
// to correctly track LR in functions

#ifdef _MX_DEBUG
#define REX_INLINE
#else
#define REX_INLINE INLINE
#endif

REXCTX  __align(32) rex_ctx;
REXPROF __align(32) rex_prof [REX_PROF_NUM];

void RexResCevaDone (TCB * pTCB);
MXRC RexResCevaRun (REXCTX * pCtx, REXTCBD * pTCB);

void RexResFpDone (TCB * pTCB);
MXRC RexResFpRun (REXCTX * pCtx, REXTCBD * pTCB);

MXRC RexClientFpRun(REXCTX * pCtx, TCB * pTcb);
void RexClientFpDone(TCB * pTcb);

void RexResArmDone (LPVOID pDrvCtx, LPVOID p);
MXRC RexResArmRun (REXCTX * pCtx, REXTCBD * pTCB);

UINT32 DspDrvGetCevaIRQStatus (void);

void RexAddProf (REX_ENUM_PROF_ID id, UINT32 t)
{
    REXPROF * pProf = &rex_prof [id];

    pProf->MaxTime   = MAX(pProf->MaxTime, t);
    pProf->TotalTime += t;
    pProf->MinTime   = (pProf->MinTime == 0) ? t : MIN(pProf->MinTime, t);
    pProf->TotalNum  ++;
}

REXPROF * RexGetProfInfo (REX_ENUM_PROF_ID id)
{
    _ASSERT (id < REX_PROF_NUM);
    return &rex_prof [id];
}

char * RexGetHeapName (HANDLE h)
{
    if (IS_HANDLE_EQUAL(h,AppHeap))
        return "AppHeap";

    if (IS_HANDLE_EQUAL(h,DspHeap))
        return "DspHeap";

    if (IS_HANDLE_EQUAL(h,IramHeap))
        return "IramHeap";

    if (IS_HANDLE_EQUAL(h,NcnbHeap))
        return "NcnbHeap";

    if (IS_HANDLE_EQUAL(h,DDR1Heap))
        return "DDR1Heap";

    if (IS_HANDLE_EQUAL(h, DDR1NcnbHeap))
        return "DDR1NcnbHeap";

     return "general";
}

void RexShowTaskList (PTASKLIST pList)
{
    UINT32 i;
    volatile REXTCBD * pTcbD = pList->TcbHead;

    dbg_printf (REX_SHOWLIST, "TASK LIST (0x%x), id=%d, num=%d, done=%d\r\n", pList, pList->ID, pList->TcbNum, pList->TcbDone);
    dbg_printf (REX_SHOWLIST, "=======================================\r\n");

    while (pTcbD)
    {
        dbg_printf (REX_SHOWLIST, " - TCB(0x%x), res=%d, taskid=%d, DepNum=%d, RefNum=%d ctrl=0x%x\r\n",
            pTcbD->pHwTCB, pTcbD->ResourceID, pTcbD->pHwTCB->TaskID, pTcbD->DepNum, pTcbD->RefNum, pTcbD->Ctrl
        );

        i = 0;

        while (i < pTcbD->RefNum)
        {
            REXTCBD * pRefTcb = pTcbD->RefList[i];

            dbg_printf (REX_SHOWLIST, "     *REF(0x%x), resID=%d, resIdx=%d, taskid=%d, DepNum=%d, RefNum=%d, list_id=%d ctrl=0x%x\r\n",
              pRefTcb->pHwTCB, pRefTcb->ResourceID, pRefTcb->pHwTCB->ResourceIndex, pRefTcb->pHwTCB->TaskID, pRefTcb->DepNum, pRefTcb->RefNum,
              (pRefTcb->RexList != NULL) ? (((PTASKLIST)pRefTcb->RexList)->ID) : ((UINT32)~0), pRefTcb->Ctrl);

            i++;
        }

        pTcbD = pTcbD->RexTcbNext;
    }

    dbg_printf (REX_SHOWLIST, "===================================\r\n");
}

REX_INLINE REXCTX * RexGetCtx (void)
{
    return &rex_ctx;
}

ASMPROC void RexPreloadData (LPVOID pAddr)
{
    PLD [r0, #0 ]
    PLD [r0, #32]
    PLD [r0, #64]
    PLD [r0, #96]
    PLD [r0, #128]
    PLD [r0, #160]
    PLD [r0, #192]
    PLD [r0, #224]

    BX LR
}

HANDLE REX_INLINE RexGetCtxPart(PREXCTX pCtx, TCB *pTCB)
{
    UINT32 Data;

    Data = (UINT32)pTCB;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(Data);

    if ((UINT32)pCtx->pHwCtxPtr <= Data && Data <= (UINT32)pCtx->pHwCtxPtr + pCtx->HwStorSize)
        return pCtx->hHwCtxPart;

    return pCtx->hSwCtxPart;
}

/** @brief This function calls tasklist callbacks

    @param pCtx [in] - the pointer to the REX service context

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexCompleteThread(PREXCTX pCtx)
{
    PTASKLIST pList;

    while (pCtx->CListGet != pCtx->CListPut)
    {
        pList = pCtx->CList [pCtx->CListGet++];

        _ASSERT_PTR(pList);
        _ASSERT_PTR(pList->CbDone);

        if (pCtx->CListGet >= REX_MAX_LIST_NUM)
            pCtx->CListGet = 0;

        dbg_printf(REX_CALL_CBDONE, "REX-List(%x)(id=%d) is done, cbDone:(%x), param:(%x)\r\n", pList, pList->ID, pList->CbDone, pList->CbDoneParam);

        pList->CbDone (pList, pList->CbDoneParam);
    }

    return MX_OK;
}

/** @brief This function removes the complete thread and frees system resources

    @param pCtx [in] - the pointer to the context
    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexRemoveCompleteThread(PREXCTX pCtx)
{
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);

    if (HANDLE_NOT_NULL(pCtx->hCompleteThread))
        rc = MxRemoveThread (pCtx->hCompleteThread);

    pCtx->hCompleteThread = HNULL;

    return rc;
}

/** @brief This function creates a thread the is used to call tasklist complete
           callback to notify the application about task list completeness

    @param pCtx [in] - the pointer to the REX contex
    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexCreateCompleteThread (PREXCTX pCtx)
{
    MXRC rc;

    _ASSERT_PTR(pCtx);

    rc = MxCreateThread ((TENTRY)RexCompleteThread, NULL, pCtx, 0, &pCtx->hCompleteThread);

    _ASSERT_RC(rc);

    return rc;
}

BOOL REX_INLINE RexIsTcbInList (PTASKLIST pList, REXTCBD * pTcbD)
{
    volatile REXTCBD * pBlock;
    _ASSERT_PTR(pTcbD);

    if (pList == NULL)
        return FALSE;

    pBlock = pList->TcbHead;

    while (pBlock)
    {
        if (pBlock == pTcbD)
            return TRUE;

        pBlock = pBlock->RexTcbNext;
    }

    return FALSE;
}

/** @brief This function removes the specified TCB block from the list of references

    @param pTcb     [in] - the pointer to the TCB to be processed
    @param pRefTcb  [in] - the reference

    *  \ingroup group_lte_rexscheduler

    */

void REX_INLINE RexRemoveRef (REXTCBD * pTcbD, REXTCBD * pRefTcbD)
{
    UINT32 i;

    _ASSERT_PTR(pTcbD);
    _ASSERT_PTR(pRefTcbD);

    i = 0;

    while (i < pTcbD->RefNum)
    {
        if (pTcbD->RefList [i] == pRefTcbD)
        {
            if ((i + 1) < pTcbD->RefNum)
            {
                //memcpy (&(pTcb->RefList [i]), &(pTcb->RefList [i+1]), (pTcb->RefNum - i - 1) * sizeof (pTcb->RefList[0]));
                pTcbD->RefList [i] = pTcbD->RefList [pTcbD->RefNum - 1];
            }

            pTcbD->RefList [--pTcbD->RefNum] = NULL;

            // to decrease the number of elements this element depends on
            pRefTcbD->DepNum --;
            continue;
        }

        i++;
    }
}

/** @brief This code scans the list of the TCB tasks and removes references
           to the bypassd TCBs.

    @param pList [in] - the task list
    @param pTcb  [in] - the TCB task

    @return [NONE]

    *  \ingroup group_lte_rexscheduler

    */

void REX_INLINE RexRemoveRefInList (PTASKLIST pList, REXTCBD * pTcbD)
{
    REXTCBD * pBlock;

    _ASSERT_PTR(pTcbD);

    if (pList == NULL)
        return;

    pBlock = (REXTCBD*)pList->TcbHead;

    while (pBlock != NULL)
    {
        if (pBlock != pTcbD)
        {
            RexRemoveRef (pBlock, pTcbD);
        }

        pBlock = pBlock->RexTcbNext;
    }
}

/** @brief The function is designed to cut the TCB task from the task list
            mainly it's used to reorganize the list according to the new TCB task d-weight

    @param pTcb [in] - the pointer to the TCB task (this task contain pointer to
                       the list in which this TCB is included)

                       *  \ingroup group_lte_rexscheduler

                       */

void REX_INLINE RexCutFromList (REXTCBD * pTcbD)
{
    TASKLIST * pList;

    _ASSERT_PTR (pTcbD);

    pList = (TASKLIST*)pTcbD->RexList;

    // if TCB block is not in the list

    if (pList == NULL)
        return;

    if (pTcbD->RexTcbPrev != NULL)
    {
        pTcbD->RexTcbPrev->RexTcbNext = pTcbD->RexTcbNext;
    }

    if (pTcbD->RexTcbNext != NULL)
    {
        pTcbD->RexTcbNext->RexTcbPrev = pTcbD->RexTcbPrev;
    }

    if (pList->TcbHead == pTcbD)
    {
        pList->TcbHead = pTcbD->RexTcbNext;
    }

    if (pList->TcbTail == pTcbD)
    {
        pList->TcbTail = pTcbD->RexTcbPrev;
    }

    pTcbD->RexTcbNext = NULL;
    pTcbD->RexTcbPrev = NULL;
    pList->TcbNum --;

    // to process the list and removes links to this block
    RexRemoveRefInList (pList, pTcbD);

    pTcbD->RexList = NULL;
    pTcbD->Ctrl &= ~REX_CTRL_TCB_IN_LIST;
}

/** @brief This function removes TCB task from the list of the scheduled task.
           The TCB block remains to be in the task list but the element is not scheduled

    @param pList[in] - the pointer to the task list
    @param pTcb [in] - the pointer to the TCB block

    *  \ingroup group_lte_rexscheduler

    */

void RexCutFromSchedList (PTASKLIST pList, REXTCBD * pTcbD, BOOL bSafe)
{
    IRQTYPE r = 0;
    REXCTX * pCtx = RexGetCtx ();

    _ASSERT_PTR(pTcbD);
    _ASSERT_PTR(pList);
    _ASSERT_PTR(pCtx);

    if (pList == NULL)
    {
        _ASSERT_PTR(pList);
        return;
    }

    if (bSafe)
    {
        r = ARM_INT_disable();

        if (!REX_IS_SERV_CFG(pCtx))
        MxGetLock (&pList->Lock);
    }

    if (pTcbD->Ctrl & REX_CTRL_TCB_SCHEDULED)
    {
        if (pTcbD->RexSchedPrev != NULL)
        {
            pTcbD->RexSchedPrev->RexSchedNext = pTcbD->RexSchedNext;
        }

        if (pTcbD->RexSchedNext != NULL)
        {
            pTcbD->RexSchedNext->RexSchedPrev = pTcbD->RexSchedPrev;
        }

        if (pList->SchedHead == pTcbD)
        {
            pList->SchedHead = pTcbD->RexSchedNext;
        }

        if (pList->SchedTail == pTcbD)
        {
            pList->SchedTail = pTcbD->RexSchedPrev;
        }

        pTcbD->RexSchedNext = NULL;
        pTcbD->RexSchedPrev = NULL;

        pTcbD->Ctrl &= ~REX_CTRL_TCB_SCHEDULED;
    }

    if (bSafe)
    {
        if (!REX_IS_SERV_CFG(pCtx))
        MxReleaseLock (&pList->Lock);

        ARM_INT_restore(r);
    }
}

/** @brief This function adds new one element to the list of scheduled elements,
            the scheduled elements are ready to be routed to the device (CEVA, ARM, FP, etc)

    @param pList [in] - the pointer to the list
    @param pTCB  [in] - the pointer to the scheduled task

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC REX_INLINE RexAddToScheduled (PTASKLIST pList, REXTCBD * pTcbD, BOOL bSync)
{
    IRQTYPE r = 0;
    MXRC rc = MX_OK;
    REXCTX * pCtx = RexGetCtx();

    _ASSERT_PTR(pList);
    _ASSERT_PTR(pTcbD);
    _ASSERT_PTR(pCtx);

    dbg_printf (REX_ADDSCHEDULING, "+sched, list:(%x)(head=%x, tail=%x), tcb-d:(%x) tcb:(%x)\r\n", pList, pList->SchedHead, pList->SchedTail, pTcbD, pTcbD->pHwTCB);

    if (bSync)
    {
        r = ARM_INT_disable();

        if (!REX_IS_SERV_CFG(pCtx))
           MxGetLock (&pList->Lock);
    }

    // just to prevent scheduling of this element twice
    // the case of cross-connected lists (one list is scheduled, the next one is not)

    if (0 == (pTcbD->Ctrl & (REX_CTRL_TCB_SCHEDULED | REX_CTRL_TCB_DONE | REX_CTRL_TCB_LOCKED)))
    {
        if (pList->SchedHead == NULL)
        {
            pList->SchedHead = pTcbD;
            pList->SchedTail = pTcbD;

            pTcbD->RexSchedNext = NULL;
            pTcbD->RexSchedPrev = NULL;
        }
        else
        {
            pList->SchedTail->RexSchedNext = pTcbD;
            pTcbD->RexSchedPrev = (REXTCBD*)pList->SchedTail;

            pList->SchedTail = (volatile REXTCBD*)pTcbD;
            pTcbD->RexSchedNext = NULL;
        }

        pTcbD->Ctrl |= REX_CTRL_TCB_SCHEDULED;
    }
    else
    {
        rc = REX_RC_SCHED_SKIPPED;
    }

    dbg_printf (REX_ADDSCHEDULING, " >> REX:Scheduling, tcb-d:(%x) tcb(%x), Id=%d, ResID=%d\r\n", pTcbD, pTcbD->pHwTCB, pTcbD->pHwTCB->TaskID, pTcbD->ResourceID);

    if (bSync)
    {
        if (!REX_IS_SERV_CFG(pCtx))
            MxReleaseLock (&pList->Lock);

        ARM_INT_restore(r);
    }

    return rc;
}

/** @brief This function checks the possibility to add new dependency

    @param pTcb         [in] - the TCB task that depends on pDepOnThis
    @param pDepOnThis   [in] - the dependency

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexIsDepConflict (REXTCBD * pTcbD, REXTCBD * pDepOnThisD)
{
    UINT32 i;

    _ASSERT_PTR(pTcbD);
    _ASSERT_PTR(pDepOnThisD);

    if (pTcbD == pDepOnThisD)
        return REX_RC_DEP_ERROR;

    // to check if dependency is already added

    for (i = 0; i < pDepOnThisD->RefNum; i++)
    {
        if (pDepOnThisD->RefList [i] == pTcbD)
            return REX_RC_SKIP;
    }

    // to check the cyclic dependency

    for (i = 0; i < pTcbD->RefNum; i++)
    {
        if (pTcbD->RefList [i] == pDepOnThisD)
            return REX_RC_DEP_ERROR;
    }

    // if this TCB block is not in task list,
    // this dependencies is meaningless

    if (pDepOnThisD->RexList != NULL)
    {
        if (!RexIsTcbInList ((PTASKLIST)pDepOnThisD->RexList, pDepOnThisD))
        {
            dbg_printf (REX_ADD_DEP, "ADD-DEP#1, TCB-D:(%x) TCB:(%x) is not in the list\r\n", pDepOnThisD, pDepOnThisD->pHwTCB);
            return REX_RC_SKIP;
        }
    }
    else
    {
        dbg_printf (REX_ADD_DEP, "ADD-DEP#2, TCB-D:(%x) TCB:(%x) is not in the list\r\n", pDepOnThisD, pDepOnThisD->pHwTCB);
        return REX_RC_SKIP;
    }

    // the dependency can de added

    return MX_OK;
}

BOOL RexIsBuffersIntersects (UINT32 p1, UINT32 p2, UINT32 len1, UINT32 len2)
{
    if (p1 != NULL && p1 != NULL)
    {
        if (p2 >= p1 && p2 <= (p1 + len1) )
            return TRUE;

        if (p1 >= p2 && p1 <= (p2 + len2) )
            return TRUE;
    }

    return FALSE;
}

BOOL RexIsIntersected(TCB * pTcb, TCB * pElm)
{
    UINT32 inp_scan, out_scan;
    UINT32 p1, p2, len1 = 0, len2 = 0;

    _ASSERT_PTR(pTcb);
    _ASSERT_PTR(pElm);

    // to check the <INPUT> and <OUTPUT> buffers

    if (RexIsBuffersIntersects ((UINT32)pTcb->InputDataPtr, (UINT32)pElm->OutputDataPtr, pTcb->InputDataLen, pElm->OutputDataLen))
        return TRUE;

    // to scan the IO buffers

    inp_scan = 0;

    while ((p1 = (UINT32)RexGetIOBufByType(pTcb, &len1, IN_BUF, &inp_scan)) != NULL)
    {
        if (RexIsBuffersIntersects (p1, (UINT32)pElm->OutputDataPtr, len1, pElm->OutputDataLen))
            return TRUE;

        out_scan = 0;

        while ((p2 = (UINT32)RexGetIOBufByType(pElm, &len2, OUT_BUF, &out_scan)) != NULL)
        {
            if (RexIsBuffersIntersects (p1, p2, len1, len2))
                return TRUE;
        }
    }

    return FALSE;
}

/** @brief The function discovers the additional dependencies by scanning the list,
           the dependencies are detected like input/output buffers intersection

    @param pList [in] - the list of tasks
    @param pTcb  [in] - the new one added task

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexDiscoverDep (PTASKLIST pList, REXTCBD * pTcbD)
{
    REXTCBD * pElmD;
    MXRC rc = MX_OK;
    TCB* pElm, *pTcb;

    _ASSERT_PTR(pList);

    pElmD = (REXTCBD*)pList->TcbHead;

    pTcb = pTcbD->pHwTCB;
    _ASSERT_PTR(pTcb);

    while (pElmD)
    {
        pElm = pElmD->pHwTCB;
        _ASSERT_PTR(pElm);

        if (pTcb->ContextPtr == pElm->ContextPtr)
        {
            dbg_printf (REX_DISCOVER_DEP, "REX-DISCDEP#1, add new dep tcb-d:(%x) tcb(%x)=>tcb-d:(%x) tcb(%x) by context\r\n", pTcbD, pTcb, pElmD, pElm);
            rc = RexAddDep (pTcb, pElm);
        }
        else if (RexIsIntersected(pTcb, pElm))
        {
            dbg_printf (REX_DISCOVER_DEP, "REX-DISCDEP#2, add new dep tcb-d:(%x) tcb(%x)=>tcb-d:(%x) tcb(%x) by context\r\n", pTcbD, pTcb, pElmD, pElm);
            rc = RexAddDep (pTcb, pElm);
        }
        else if (RexIsIntersected(pElm, pTcb))
        {
            dbg_printf (REX_DISCOVER_DEP, "REX-DISCDEP#3, add new dep tcb-d:(%x) tcb(%x)=>tcb-d:(%x) tcb(%x) by context\r\n", pElmD, pElm, pTcbD, pTcb);
            rc = RexAddDep (pElm, pTcb);
        }

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        pElmD = pElmD->RexTcbNext;
    }

    return MX_OK;
}

/** @brief This function is designed to mark the list as completed,
           set completion time and call list callback function

    @param pList [in] - the pointer to the completed list

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexCompleteListHandler(PREXCTX pCtx, PTASKLIST pList, UINT32 ListID, UINT32 StartTicks, UINT32 cbDone, UINT32 cbDonePtr, UINT32 NumBadTcb)
{
    REXCBDONE cb_done = (REXCBDONE)cbDone;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pList);

    pCtx->CbDoneList   = pList;
    pCtx->CbDoneBadTcb = NumBadTcb;

    if (cb_done != NULL)
        cb_done(pList, (LPVOID*)cbDonePtr);

    pCtx->CbDoneList   = NULL;
    pCtx->CbDoneBadTcb = 0;

    return MX_OK;
}

/** @brief This function is designed to mark the list as completed,
           set completion time and call list callback function

    @param pList [in] - the pointer to the completed list

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexCompleteList(PREXCTX pCtx, PTASKLIST pList)
{
#ifdef REX_CB_THREAD_BASED
    UINT32 bNotRun;
#endif

    UINT32 ctrl;

#if 1
    UINT32 lr = MX_GET_RADDR;
#endif

    REX_PROF_DEF_TIME();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pList);

    ctrl = pList->Ctrl;

    if (ctrl & REX_CTRL_LIST_IS_DONE)
    {
        dbg_printf(REX_LIST_COMPLETED, "REX:TaskList:(%x)(id=%d) is completed, DOUBLE CALLING!!!, tcb_num=%d, tcb_proc=%d\r\n", pList, pList->ID, pList->TcbNum, pList->TcbDone);
        return MX_OK;
    }

    //uart_printf ("(ARM-%d/%x)REX(id=%02d): cbDone 0x%x(0x%x), ctrl=0x%x\r\n", MxGetCpuID(), MxGetCpuMode(), pList->ID, pList, pList->CbDoneParam, pList->Ctrl);

#if 1
    if (ctrl & REX_CTRL_LIST_IS_SCHEDULED)
    {
        //rex_printf ("REX:CompleteList: List(id=%d, ctrl=%x) is done but is scheduled for some reason!!!",
        //    pList->ID, pList->Ctrl);

        //_ASSERT (!(pList->Ctrl & REX_CTRL_LIST_IS_SCHEDULED));

        // this situation is possible when some TCB block(s) was unlocked and marked like
        // bypassed at the same time. Also at this time the same list was gotten by REX:Kernel
        // and kernel is going to process it. The unlock procedure (RexResetTCBControl)
        // detected that list is not already in the queue and putted it to the queue again.
        // RexProcList () called by Kernel may process all list elements and call RexCompleteList()
        // but because the list was putted to the queue by RexResetTCBControl function we may detect
        // this situation here

        // In this case we need to return, this code will be called right now again from the kernel

        return MX_OK;
    }
#endif

    pList->StartTick = GetTIMETICK () - pList->StartTick;
    ctrl &= ~REX_CTRL_LIST_UNDER_PROC;
    ctrl |= REX_CTRL_LIST_IS_DONE;
    pList->Ctrl = ctrl;

    if (pList->CbDone == NULL)
    {
        REX_PROF_ADD_TIME(REX_PROF_CALL_CB);
        return MX_OK;
    }

#ifdef REX_CB_THREAD_BASED

    bNotRun = (pCtx->CListPut == pCtx->CListGet);

    pCtx->CList [pCtx->CListPut++] = pList;

    if (pCtx->CListPut >= REX_MAX_LIST_NUM)
        pCtx->CListPut = 0;

    dbg_printf(REX_LIST_COMPLETED, "REX:TaskList:(%x)(id=%d) is completed, tcb_num=%d, tcb_proc=%d\r\n", pList, pList->ID, pList->TcbNum, pList->TcbDone);

    if (bNotRun)
        MxScheduleThread(pCtx->hCompleteThread);

#else

     dbg_printf(REX_LIST_COMPLETED, "REX:TaskList:(%x)(id=%d) is completed, tcb_num=%d, tcb_proc=%d\r\n", pList, pList->ID, pList->TcbNum, pList->TcbDone);

     pList->CbDone(pList, pList->CbDoneParam);

#endif

    REX_PROF_ADD_TIME(REX_PROF_CALL_CB);

    return MX_OK;
}

/** @brief This function is called in IRQ mode to register new one list for execution

    @param pCtx [in] - the pointer to the REX context
    @param pList[in] - the pointer to the list

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

REX_INLINE MXRC RexRegList (REXCTX * pCtx, PTASKLIST pList)
{
    UINT32 rc = MX_OK;

    _ASSERT_PTR (pList);
    _ASSERT_PTR (pCtx);

    // we need to schedule this list to be processed only in case
    // if this list was started by RexExecList function and prevent scheduling
    // of this list by cross-connected lists

    if (pList->Ctrl & REX_CTRL_LIST_UNDER_PROC)
    {
        MxGetLock (&pCtx->SchedListLock);

        if (!(pList->Ctrl & (REX_CTRL_LIST_IS_SCHEDULED | REX_CTRL_LIST_IS_DONE)))
        {
            pList->Ctrl |= REX_CTRL_LIST_IS_SCHEDULED;
            pCtx->SchedLists [pCtx->SchedListPut++] = pList;

            if (pCtx->SchedListPut >= REX_MAX_LIST_NUM)
                pCtx->SchedListPut = 0;
        }
        else
        {
            rc = REX_RC_REG_LIST_SKIPPED;
        }

        MxReleaseLock (&pCtx->SchedListLock);
    }
    else
    {
        rc = REX_RC_REG_LIST_SKIPPED;

        dbg_printf(REX_LIST_RESCHED_IS_DET, "REX:SchedList - resched is detected!!!, list=%d, id=%d, tcb_num=%d\r\n", pList, pList->ID, pList->TcbNum);

        _ASSERT_RC(rc);
    }

    return rc;
}

/** @brief This function is called in ARM supervisor/IRQ modes to register new one list for execution
           if new TCB is ready (was unlocked)

    @param pCtx [in] - the pointer to the REX context
    @param pList[in] - the pointer to the list

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

REX_INLINE MXRC RexRegListWithLock (REXCTX * pCtx, PTASKLIST pList)
{
    UINT32 rc = MX_OK;

    _ASSERT_PTR (pList);
    _ASSERT_PTR (pCtx);

    // we need to schedule this list to be processed only in case
    // if this list was started by RexExecList function and prevent scheduling
    // of this list by cross-connected lists

    if (pList->Ctrl & REX_CTRL_LIST_UNDER_PROC)
    {
        MxGetLock (&pCtx->SchedListLock);

        if (!(pList->Ctrl & (REX_CTRL_LIST_IS_SCHEDULED | REX_CTRL_LIST_IS_DONE)))
        {
            pList->Ctrl |= REX_CTRL_LIST_IS_SCHEDULED;
            pCtx->SchedLists [pCtx->SchedListPut++] = pList;

            if (pCtx->SchedListPut >= REX_MAX_LIST_NUM)
                pCtx->SchedListPut = 0;
        }
        else
        {
            rc = REX_RC_REG_LIST_SKIPPED;
        }
        MxReleaseLock (&pCtx->SchedListLock);
    }
    else
    {
        rc = REX_RC_REG_LIST_SKIPPED;

        //dbg_printf(REX_LIST_RESCHED_IS_DET, "REX:RexRegListWithLock - resched is detected!!!, list=%d, id=%d, tcb_num=%d\r\n", pList, pList->ID, pList->TcbNum);

        //_ASSERT_RC(rc);
    }

    return rc;
}

REX_INLINE BOOL RexRegNewList (REXCTX * pCtx, PTASKLIST pList)
{
    BOOL bEmpty = 0;

    _ASSERT_PTR (pList);
    _ASSERT_PTR (pCtx);

    if ((pList->Ctrl & (REX_CTRL_LIST_IS_SCHEDULED | REX_CTRL_LIST_IS_DONE | REX_CTRL_LIST_UNDER_PROC)) == 0)
    {
        IRQTYPE irq = ARM_INT_disable();
        MxGetLock (&pCtx->SchedListLock);

        bEmpty = (pCtx->SchedListPut == pCtx->SchedListGet);
        if (!(pList->Ctrl & (REX_CTRL_LIST_IS_SCHEDULED | REX_CTRL_LIST_UNDER_PROC | REX_CTRL_LIST_IS_DONE)))
        {
            pList->Ctrl |= REX_CTRL_LIST_IS_SCHEDULED | REX_CTRL_LIST_STARTED | REX_CTRL_LIST_UNDER_PROC;
            pCtx->SchedLists [pCtx->SchedListPut++] = pList;

            if (pCtx->SchedListPut >= REX_MAX_LIST_NUM)
                pCtx->SchedListPut = 0;
        }
        else
        {
            _ASSERT(!(pList->Ctrl & (REX_CTRL_LIST_IS_SCHEDULED | REX_CTRL_LIST_UNDER_PROC | REX_CTRL_LIST_IS_DONE)));
        }

        MxReleaseLock (&pCtx->SchedListLock);
        ARM_INT_restore(irq);
    }
    else
    {
        rex_printf ("\r\n\r\nREX:NewList, list ctrl error, ctrl=%x, list-id=%d\r\n", pList->Ctrl, pList->ID);
        dbg_printf(REX_LIST_RESCHED_IS_DET, "REX:SchedList - resched is detected!!!, list=%d, id=%d, tcb_num=%d\r\n", pList, pList->ID, pList->TcbNum);

        _ASSERT((pList->Ctrl & (REX_CTRL_LIST_IS_SCHEDULED | REX_CTRL_LIST_IS_DONE | REX_CTRL_LIST_UNDER_PROC)) == 0);
    }

    return bEmpty;
}

ASMPROC UINT32 RexGetFirstIndex (UINT32 nVal)
{
    MOV r1, #31
    CLZ r0, r0
    SUB r0, r1, r0
    BX LR
}

/** @brief This function processes element dependency

    @param pCtx [in] - the REX context
    @param pTcbD[in] - the pointer to the TCB task
    @param bSync[in] - to enable/disable synchronization

    *  \ingroup group_lte_rexscheduler

    */

void REX_INLINE ProcDependency (REXCTX * pCtx, REXTCBD * pTcbD, UINT32 bSync)
{
    REXTCBD * pProcTcbD;
    UINT32 i = 0;
    MXRC ret_code = 0;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTcbD);

    dbg_printf (REX_PROC_DEP, "proc-dep, tcb-d:(%x) tcb:(%x)\r\n", pTcbD, pTcbD->pHwTCB);

    while (i < pTcbD->RefNum)
    {
        pProcTcbD = pTcbD->RefList [i];

        _ASSERT_PTR(pProcTcbD);

        pProcTcbD->DepDone ++;

        _ASSERT (pProcTcbD->DepDone <= pProcTcbD->DepNum);

        // if dependency of the list TCB is completed and this TCB is not locked
        // and TCB was not processed, we need to add TCB to the list of ready elements
        // and to schedule this list to be processed by kernel

        if ((pProcTcbD->DepDone == pProcTcbD->DepNum) && !(pProcTcbD->Ctrl & (REX_CTRL_TCB_LOCKED | REX_CTRL_TCB_DONE)))
        {
            dbg_printf (REX_PROC_DEP, "  *TCB-d:(%x) TCB:(%x) => to list:(%x)\r\n", pProcTcbD, pProcTcbD->pHwTCB, pTcbD->RexList);

            ret_code = RexAddToScheduled ((PTASKLIST)pProcTcbD->RexList, pProcTcbD, bSync ? 1 : ((UINT32)pTcbD->RexList != (UINT32)pProcTcbD->RexList));

            // The task list has to be scheduled only if this list is not processed by
            // RexProcList function, and this list was started by ExecList
            // (we need to prevent auto scheduling of cross-connected lists if the second one was not schedulued by Application)

            if ((bSync || ((UINT32)pTcbD->RexList != (UINT32)pProcTcbD->RexList))&& SUCCESSFUL(ret_code) && (((PTASKLIST)pProcTcbD->RexList)->Ctrl & REX_CTRL_LIST_UNDER_PROC))
            {
                RexRegList(pCtx, (PTASKLIST)pProcTcbD->RexList);
            }
        }

        i++;
    }
}

MXRC RexResCevaRun (REXCTX * pCtx, REXTCBD * pTcbD)
{
    REXRESOURCE * pRes;
    UINT32 nIndex;
    MXRC rc = MX_OK;

    REX_PROF_DEF_TIME();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTcbD);
    _ASSERT_PTR(pTcbD->pHwTCB);

    pTcbD->RexDevNext = NULL;
    pTcbD->RexRoutedNext = NULL;

    dbg_printf (REX_CEVA_RUN, "Ceva-RUN:list-id:(%d) tcb-d:(%x) tcb:(%x) taskid=%d, ridx=%d\r\n", ((PTASKLIST)pTcbD->RexList)->ID,  pTcbD, pTcbD->pHwTCB, pTcbD->pHwTCB->TaskID, pTcbD->ResIndex);

    pRes = &pCtx->Res [pTcbD->ResourceID];

    // if all cevas are busy
    // just add this block to the list of task
    // scheduled for CEVA

    if (pRes->EnableMask == 0)
    {
        dbg_printf (REX_CEVA_RUN, "Ceva#2\r\n");

        if (pRes->Head == NULL)
        {
            pRes->Head = pTcbD;
            pRes->Tail = pTcbD;
        }
        else
        {
            pRes->Tail->RexDevNext = pTcbD;
            pRes->Tail = pTcbD;
        }
    }
    else
    {
        // in this case we can route the task directly to the CEVA
        nIndex = RexGetFirstIndex (pRes->EnableMask);

        dbg_printf (REX_CEVA_RUN, "Ceva#3 (index=%d)\r\n", nIndex);

        // to mark a resource like busy
        pRes->EnableMask ^= (1 << nIndex);
        pTcbD->ResIndex = nIndex;

        dbg_printf (REX_CEVA_RUN, "Ceva#4 (index=%d)\r\n", nIndex);

        _ASSERT (nIndex < sizeof (pRes->RoutedTcbHead)/sizeof (pRes->RoutedTcbHead[0]));

        pRes->RoutedTcbHead[nIndex] = pTcbD;

        // to run the task
        rc = DspDrvRun(nIndex, pTcbD->pHwTCB);

        dbg_printf (REX_CEVA_RUN, "Ceva#5 (index=%d)\r\n", nIndex);

        _ASSERT_RC(rc);
    }

    dbg_printf (REX_CEVA_RUN, "Ceva#6\r\n");

    REX_PROF_ADD_TIME(REX_PROF_CEVA_RUN);

    return rc;
}

void RexResCevaDone (TCB * pTcb)
{
    REXRESOURCE * pRes;
    REXCTX * pCtx = RexGetCtx ();
    PTASKLIST pList;
    REXTCBD * pSchedTcbD, *pTcbD;

    REX_PROF_DEF_TIME_EX();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTcb);

    _ASSERT(pCtx->MasterCpu == MxGetCpuID());

    pTcbD = (REXTCBD*)pTcb->RexDescr;
    _ASSERT_PTR(pTcbD);

    dbg_printf (REX_CEVA_DONE, "Rex:Ceva-Done, tcb-d:(%x), tcb:(%x), taskid:(%d), ridx:(%d)\r\n", pTcbD, pTcb, pTcb->TaskID, pTcbD->ResIndex);

    pList = (PTASKLIST)pTcbD->RexList;
    _ASSERT_PTR(pList);

    _ASSERT((pTcbD->Ctrl & REX_CTRL_TCB_DONE) == 0);

    if ((pTcbD->Ctrl & REX_CTRL_TCB_DONE) != 0)
    {
        uart_printf ("REX:CEVA-Done warning! TCB(0x%x), task-id=%d, list-id=%d is processed twice\r\n", pTcb, pTcb->TaskID, pList->ID);
        while (1);
    }

    pTcbD->Ctrl |= REX_CTRL_TCB_DONE;

    if (pTcbD->CbDone != NULL)
    {
        // to call the application callback
        // if this callback is specified
        pTcbD->CbDone (pTcb, pTcbD->CbDonePtr, 0);
    }

    // to increase the number of processed TCBs
    pList->TcbDone ++;

    // to get resource context
    pRes = &pCtx->Res [pTcbD->ResourceID];

    // To mark resource like accessible to be used
    pRes->EnableMask |= (1 << pTcbD->ResIndex);

    _ASSERT (pTcbD->ResIndex < sizeof (pRes->RoutedTcbHead)/sizeof (pRes->RoutedTcbHead[0]));
    _ASSERT_PTR (pRes->RoutedTcbHead [pTcbD->ResIndex]);

    pRes->RoutedTcbHead [pTcbD->ResIndex] = NULL;

    REX_PROF_START_TIME_EX();

    while (pRes->EnableMask != 0 && pRes->Head != NULL)
    {
        // to get the first element
        // from the list of scheduled TCBs
        pSchedTcbD = pRes->Head;

        // To remove this element from the list
        // the removed element will be scheduled
        pRes->Head = (REXTCBD*)pSchedTcbD->RexDevNext;

        dbg_printf (REX_CEVA_DONE, "REX:Ceva-Done#2 sheduling of new element, tcb-d:(%x), tcb:(%x)\r\n", pSchedTcbD,pSchedTcbD->pHwTCB);

        // to schedule the removed element
        RexResCevaRun (pCtx, pSchedTcbD);
    }

    REX_PROF_ADD_TIME_EX(REX_PROF_CEVA_RUN_FROM_CEVA_DONE);

    dbg_printf (REX_CEVA_DONE, "REX:Ceva-Done#3, processing dependencies for tcb-d:(%x), tcb:(%x)\r\n", pTcbD, pTcb);

    if (pList->TcbDone > pList->TcbNum)
    {
        rex_printf ("REX: DONE:%d, NUM:%d\r\n", pList->TcbDone, pList->TcbNum);
    }

    _ASSERT (pList->TcbDone <= pList->TcbNum);

    // if all elements of the list are processed
    // we need to put this list to the queue
    // to call the list callback

    if (pList->TcbDone == pList->TcbNum)
    {
        RexRegList (pCtx, pList);
    }

    ProcDependency (pCtx, pTcbD, 1);

    REX_PROF_ADD_TIME(REX_PROF_CEVA_DONE);

    // if no scheduler IRQ, just
    // run kernel to schedule new elements

    RexKernelRun ();

    dbg_printf (REX_CEVA_DONE, "Ceva-Done#6\r\n");
}

REX_INLINE MXRC RexRunIDFT (REXCTX * pCtx, REXRESOURCE * pRes, TCB * pHwTcb)
{
    MapIdftDrvSetCallback(REX_IS_SERV_CFG(pCtx) ? (MAPDONECB)RexClientFpDone : (MAPDONECB)RexResFpDone, NULL);
    MapIdftDrvRun(pHwTcb);

    return MX_OK;
}

REX_INLINE MXRC RexRunIFFT (REXCTX * pCtx, REXRESOURCE * pRes, TCB * pHwTcb)
{
    MapIfftDrvSetCallback(REX_IS_SERV_CFG(pCtx) ? (MAPDONECB)RexClientFpDone : (MAPDONECB)RexResFpDone, pHwTcb);
    MapIfftDrvRun(pHwTcb);

#ifdef REX_SPLIT_FFT_IFFT
    // to set FP is busy with IFFT
    pRes->IFFTNum ++;
#endif

    return MX_OK;
}

REX_INLINE MXRC RexRunFFT (REXCTX * pCtx, REXRESOURCE * pRes, TCB * pHwTcb)
{
    MapFftDrvSetCallback(REX_IS_SERV_CFG(pCtx) ? (MAPDONECB)RexClientFpDone : (MAPDONECB)RexResFpDone, pHwTcb);
    MapFftDrvRun(pHwTcb);

#ifdef REX_SPLIT_FFT_IFFT
    // to set FP is busy with FFT
    pRes->FFTNum ++;
#endif

    return MX_OK;
}

MXRC RexPush_IFFT_FFT(REXRESOURCE * pRes, REXTCBD * pTcbD)
{
    _ASSERT_PTR (pRes);
    _ASSERT_PTR (pTcbD);

    // pTcbD->RexDevNext is equal to NULL, see RexResFpRun

    if (pRes->Head == NULL)
    {
        pRes->Head = pTcbD;
        pRes->Tail = pTcbD;
    }
    else
    {
        pRes->Tail->RexDevNext = pTcbD;
        pRes->Tail = pTcbD;
    }

    return MX_OK;
}

REXTCBD * RexPop_IFFT_FFT(REXRESOURCE * pRes)
{
    REXTCBD * pTcbD;
    _ASSERT_PTR (pRes);

    // pTcbD->RexDevNext is equal to NULL, see RexResFpRun

    if (pRes->Head == NULL)
        return NULL;

    pTcbD = pRes->Head;

    pRes->Head = (REXTCBD*)pTcbD->RexDevNext;

    if (pRes->Head == NULL)
        pRes->Tail = NULL;

    return pTcbD;
}

void RexClientPush_IFFT_FFT(REXRESOURCE * pRes, TCB * pHwTcb)
{
    _ASSERT_PTR(pRes);
    _ASSERT_PTR(pHwTcb);

    pRes->tcb_fifo [pRes->tcb_fifo_put++] = pHwTcb;

    if (pRes->tcb_fifo_put >= REX_RES_FIFO_DEPTH)
        pRes->tcb_fifo_put = 0;

    _ASSERT(pRes->tcb_fifo_put != pRes->tcb_fifo_get);
}

TCB* RexClientPop_IFFT_FFT(REXRESOURCE * pRes)
{
    TCB * pTcb;

    _ASSERT_PTR(pRes);

    if (pRes->tcb_fifo_put == pRes->tcb_fifo_get)
        return NULL;

    pTcb = pRes->tcb_fifo [pRes->tcb_fifo_get++];

    if (pRes->tcb_fifo_get >= REX_RES_FIFO_DEPTH)
        pRes->tcb_fifo_get = 0;

    return pTcb;
}

TCB* RexClientPick_IFFT_FFT(REXRESOURCE * pRes)
{
    TCB * pTcb;

    _ASSERT_PTR(pRes);

    if (pRes->tcb_fifo_put == pRes->tcb_fifo_get)
        return NULL;

    pTcb = pRes->tcb_fifo [pRes->tcb_fifo_get];

    return pTcb;
}

MXRC RexClientFpRun(REXCTX * pCtx, TCB * pHwTcb)
{
    MXRC rc = MX_OK;
    UINT32 task_id;
    REXRESOURCE * pRes;

    REX_PROF_DEF_TIME();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pHwTcb);

    dbg_printf (REX_FP_RUN, "  REX:FP-run list:(0x%x) list-id:(%d) tcb-d:(0x%x), tcb:(0x%x) (task-%d)\r\n", pTcbD->RexList, ((PTASKLIST)pTcbD->RexList)->ID, pTcbD, pTcbD->pHwTCB, pTcbD->pHwTCB->TaskID);

    pRes = &pCtx->Res [RESOURCE_MAP];
    task_id = pHwTcb->TaskID;

    if (task_id == TASKID_RX_IDFT)
    {
        rc = RexRunIDFT(pCtx, pRes, pHwTcb);
    }
    else if (task_id == TASKID_TX_IFFT)
    {
        // if FP is not busy with FFT, we can run IFFT
        if (pRes->FFTNum == 0)
        {
            rc = RexRunIFFT(pCtx, pRes, pHwTcb);
        }
        else
        {
            RexClientPush_IFFT_FFT(pRes, pHwTcb);
        }
    }
    else if (task_id == TASKID_RX_FFT)
    {
        // if FP is not busy with FFT, we can run IFFT
        if (pRes->IFFTNum == 0)
        {
            rc = RexRunFFT(pCtx, pRes, pHwTcb);
        }
        else
        {
            RexClientPush_IFFT_FFT(pRes, pHwTcb);
        }
    }
    else
    {
        _ASSERT (task_id == TASKID_RX_FFT);
    }

    _ASSERT_RC (rc);
    REX_PROF_ADD_TIME(REX_PROF_FP_RUN);

    return rc;
}

void RexClientFpDone(TCB * pTcb)
{
    REXCTX * pCtx = RexGetCtx ();
#if defined(REX_SPLIT_FFT_IFFT) || defined(REX_TCB_PRIORITY_ENABLED)
    TCB * pFifoTcb;
#endif
    REXRESOURCE * pRes;
    UINT32 task_id;
    REX_PROF_DEF_TIME_EX();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTcb);

    _ASSERT(pCtx->MasterCpu == MxGetCpuID());

    pRes = &pCtx->Res [RESOURCE_MAP];
    task_id = pTcb->TaskID;

    if (task_id == TASKID_TX_IFFT)
    {
        #if defined(REX_SPLIT_FFT_IFFT)
        pRes->IFFTNum --;
        #endif
    }
    else if (task_id == TASKID_RX_FFT)
    {
        #if defined(REX_SPLIT_FFT_IFFT)
        pRes->FFTNum --;
        #endif
    }
    else
    {
        // to generate the assert, and prevent compiler warnings
        // pTCB->TaskID cannot be equal to TASKID_RX_IDFT here!

        _ASSERT (task_id == TASKID_RX_IDFT);
    }

#if defined(REX_SPLIT_FFT_IFFT) || defined(REX_TCB_PRIORITY_ENABLED)
    // To process the list of ready elements, pushed to the list
    // inside FP-RUN function

    REX_PROF_START_TIME_EX();

    while ((pFifoTcb = RexClientPick_IFFT_FFT(pRes)) != NULL)
    {
        task_id = pFifoTcb->TaskID;

        if (task_id == TASKID_TX_IFFT && pRes->FFTNum == 0)
        {
            RexClientPop_IFFT_FFT(pRes);
            RexClientFpRun(pCtx, pFifoTcb);
        }
        else if (task_id == TASKID_RX_FFT && pRes->IFFTNum == 0)
        {
            RexClientPop_IFFT_FFT(pRes);
            RexClientFpRun(pCtx, pFifoTcb);
        }
        else
        {
            break;
        }
    }

    REX_PROF_ADD_TIME_EX(REX_PROF_FP_RUN_FROM_FP_DONE);

#endif

    RexServTcbIsDoneCmd(pTcb);
}

MXRC RexResFpRun (REXCTX * pCtx, REXTCBD * pTcbD)
{
    MXRC rc = MX_OK;
    TCB * pHwTcb;
    REXRESOURCE * pRes;
    UINT32 res_id = 0xFF;

    REX_PROF_DEF_TIME();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTcbD);

    pRes = &pCtx->Res [RESOURCE_MAP];

    pTcbD->RexDevNext    = NULL;
    pTcbD->RexRoutedNext = NULL;

    pHwTcb = pTcbD->pHwTCB;
    _ASSERT_PTR(pHwTcb);

    dbg_printf (REX_FP_RUN, "  REX:FP-run list:(0x%x) list-id:(%d) tcb-d:(0x%x), tcb:(0x%x) (task-%d)\r\n", pTcbD->RexList, ((PTASKLIST)pTcbD->RexList)->ID, pTcbD, pTcbD->pHwTCB, pTcbD->pHwTCB->TaskID);

    if (pTcbD->TaskID == TASKID_RX_IDFT)
    {
        rc = RexRunIDFT(pCtx, pRes, pHwTcb);

        res_id = REX_IDFT_RES_ID;
    }
    else if (pTcbD->TaskID == TASKID_TX_IFFT)
    {
        // if FP is not busy with FFT, we can run IFFT
        if (pRes->FFTNum == 0)
        {
            rc = RexRunIFFT(pCtx, pRes, pHwTcb);
            res_id = REX_IFFT_FFT_RES_ID;
        }
        else
        {
            RexPush_IFFT_FFT(pRes, pTcbD);
        }
    }
    else if (pTcbD->TaskID == TASKID_RX_FFT)
    {
        // if FP is not busy with FFT, we can run IFFT
        if (pRes->IFFTNum == 0)
        {
            rc = RexRunFFT(pCtx, pRes, pHwTcb);
            res_id = REX_IFFT_FFT_RES_ID;
        }
        else
        {
            RexPush_IFFT_FFT(pRes, pTcbD);
        }
    }
    else
    {
        _ASSERT (pTcbD->TaskID == TASKID_RX_FFT);
    }

    // this list contains only the elements
    // routed to the device and not pushed to the
    // list of ready elements

    if (res_id != 0xFF)
    {
        if (pRes->RoutedTcbHead[res_id] == NULL)
        {
            pRes->RoutedTcbHead[res_id] = pTcbD;
            pRes->RoutedTcbTail[res_id] = pTcbD;
        }
        else
        {
            pRes->RoutedTcbTail[res_id]->RexRoutedNext = pTcbD;
            pRes->RoutedTcbTail[res_id] = pTcbD;
        }
    }

    _ASSERT_RC (rc);

    REX_PROF_ADD_TIME(REX_PROF_FP_RUN);

    return rc;
}

void RexResFpDone (TCB * pTCB)
{
    REXCTX * pCtx = RexGetCtx ();
    PTASKLIST pList;
    REXTCBD * pTcbD;
#if defined(REX_SPLIT_FFT_IFFT) || defined(REX_TCB_PRIORITY_ENABLED)
    REXTCBD *pProcTcbD, *pPrevTcbD, *pTcbDToRun;
    MXRC rc = MX_OK;
#endif
    REXRESOURCE * pRes;
    UINT32 res_id = 0;
    REX_PROF_DEF_TIME_EX();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTCB);

    _ASSERT(pCtx->MasterCpu == MxGetCpuID());

    pTcbD = pTCB->RexDescr;
    _ASSERT_PTR(pTcbD);

    pList = (PTASKLIST)pTcbD->RexList;
    _ASSERT_PTR(pList);

    dbg_printf (REX_FP_DONE, "REX:FP-done#1, list:(0x%x), list-id:(%d),tcb-d:(0x%x), tcb:(0x%x) TaskID:(%d)\r\n", pTcbD->RexList, ((PTASKLIST)pTcbD->RexList)->ID, pTcbD, pTCB, pTCB->TaskID);

    pRes = &pCtx->Res [RESOURCE_MAP];

    _ASSERT((pTcbD->Ctrl & REX_CTRL_TCB_DONE) == 0);

    if ((pTcbD->Ctrl & REX_CTRL_TCB_DONE) != 0)
    {
        uart_printf ("REX:FpDone warning! TCB(0x%x), task-id=%d, list-id=%d is processed twice\r\n", pTCB, pTCB->TaskID, ((PTASKLIST)pTcbD->RexList)->ID);
        while (1);
    }

    if (pTcbD->CbDone != NULL)
    {
        // to call the application callback
        // if this callback is specified
        pTcbD->CbDone (pTCB, pTcbD->CbDonePtr, 0);
    }

    if (pTcbD->TaskID == TASKID_RX_IDFT)
    {
        res_id = REX_IDFT_RES_ID;
    }
    else if (pTcbD->TaskID == TASKID_TX_IFFT)
    {
        #if defined(REX_SPLIT_FFT_IFFT)
        pRes->IFFTNum --;
        #endif
        res_id = REX_IFFT_FFT_RES_ID;
    }
    else if (pTcbD->TaskID == TASKID_RX_FFT)
    {
        #if defined(REX_SPLIT_FFT_IFFT)
        pRes->FFTNum --;
        #endif
        res_id = REX_IFFT_FFT_RES_ID;
    }
    else
    {
        // to generate the assert, and prevent compiler warnings
        // pTCB->TaskID cannot be equal to TASKID_RX_IDFT here!

        _ASSERT (pTCB->TaskID == TASKID_RX_IDFT);
    }

    _ASSERT_PTR(pRes->RoutedTcbHead[res_id]);

    // to remove the element from the list of routed elements
    pRes->RoutedTcbHead[res_id] = (REXTCBD*)pRes->RoutedTcbHead[res_id]->RexRoutedNext;

    if (pRes->RoutedTcbHead[res_id] == NULL)
    {
        pRes->RoutedTcbTail[res_id] = NULL;
    }

#if defined(REX_SPLIT_FFT_IFFT) || defined(REX_TCB_PRIORITY_ENABLED)
    // To process the list of ready elements, pushed to the list
    // inside FP-RUN function

    REX_PROF_START_TIME_EX();

    pPrevTcbD = NULL;
    pProcTcbD = pRes->Head;

    while (pProcTcbD != NULL)
    {
        if ((pProcTcbD->TaskID == TASKID_TX_IFFT && pRes->FFTNum == 0) ||
            (pProcTcbD->TaskID == TASKID_RX_FFT && pRes->IFFTNum == 0) ||
            (pProcTcbD->TaskID == TASKID_RX_IDFT))
        {
            pTcbDToRun = pProcTcbD;

            if (pRes->Head == pProcTcbD)
            {
                pRes->Head = (REXTCBD *)pProcTcbD->RexDevNext;
            }
            else
            {
                // we need to cut this element from this list

                _ASSERT_PTR(pPrevTcbD);
                pPrevTcbD->RexDevNext = pProcTcbD->RexDevNext;

                if (pRes->Tail == pProcTcbD)
                {
                    pRes->Tail = pPrevTcbD;
                }
            }

            pProcTcbD = (REXTCBD *)pProcTcbD->RexDevNext;

            if(FAILED(rc = RexResFpRun (pCtx, pTcbDToRun)))
            {
                _ASSERT_RC(rc);
                rex_printf("\r\nREX:RexResFpDone-error, rc=0x%08x\r\n", rc);
                return;
            }

        }
        else
        {
            pPrevTcbD = pProcTcbD;
            pProcTcbD = (REXTCBD *)pProcTcbD->RexDevNext;
        }

    }

    if (pRes->Head == NULL)
        pRes->Tail = NULL;

    REX_PROF_ADD_TIME_EX(REX_PROF_FP_RUN_FROM_FP_DONE);

#endif

    if ((pCtx->Ctrl & REX_CFG_STAT_SERV_ENABLED) == 0)
    {
        pTcbD->Ctrl |= REX_CTRL_TCB_DONE;

        // to increase the number of processed TCBs
        pList->TcbDone ++;

        _ASSERT (pList->TcbDone <= pList->TcbNum);

        // if all elements of the list are processed
        // we need to put this list to the queue
        // to call the list callback

        if (pList->TcbDone == pList->TcbNum)
        {
            RexRegList (pCtx, pList);
        }

        ProcDependency (pCtx, pTcbD, 1);

        REX_PROF_ADD_TIME(REX_PROF_FP_DONE);

        // if no scheduler IRQ, just
        // run kernel to schedule new elements

        RexKernelRun ();
    }
    else
    {
        RexServTcbIsDoneCmd(pTCB);
    }
}

MXRC RexClientArmRun(REXCTX * pCtx, TCB * pTcb)
{
    MXRC rc = MX_OK;

    REX_PROF_DEF_TIME();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTcb);

    MLogEXEStart(pTcb, 0, 0);
    pTcb->ExecTicks = GetTIMETICK ();

    rc = STDrvRunTask(1, pTcb);
    _ASSERT_RC(rc);

    REX_PROF_ADD_TIME(REX_PROF_ARM_RUN);

    return rc;

}

void RexClientArmDone (LPVOID pDrvCtx, LPVOID p)
{
    PTCB pTCB;
    REXCTX * pCtx = RexGetCtx ();

    REX_PROF_DEF_TIME();

    _ASSERT_PTR (pCtx);
    _ASSERT(pCtx->MasterCpu == MxGetCpuID());

    while ((pTCB = STDGetReadyTcb(pDrvCtx)) != NULL)
    {
        pTCB->ResourceIndex = 1;
        MLogEXEFinish(pTCB);

        RexServTcbIsDoneCmd(pTCB);
    }

    REX_PROF_ADD_TIME(REX_PROF_ARM_DONE);
}

MXRC RexResArmRun (REXCTX * pCtx, REXTCBD * pTcbD)
{
    MXRC rc = MX_OK;
    TCB * pTcb;
    REXRESOURCE * pRes;

    REX_PROF_DEF_TIME();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTcbD);
    _ASSERT_PTR(pTcbD->pHwTCB);

    pTcb = pTcbD->pHwTCB;

    dbg_printf (REX_ARM_RUN, "Rex-ARMRun:(tcb-d=%x)(tcb=%x), task=%d, arm_cpu=%d\r\n", pTcbD, pTcbD->pHwTCB, pTcbD->pHwTCB->TaskID, pTcbD->pHwTCB->ResourceIndex);

    pTcbD->RexDevNext = NULL;
    pTcbD->RexRoutedNext = NULL;

    MLogEXEStart(pTcb, 0, 0);
    pTcb->ExecTicks = GetTIMETICK ();

    pRes = &pCtx->Res [RESOURCE_LARM];

    if (pRes->RoutedTcbHead[REX_ARM_RES_ID] == NULL)
    {
        pRes->RoutedTcbHead[REX_ARM_RES_ID] = pTcbD;
        pRes->RoutedTcbTail[REX_ARM_RES_ID] = pTcbD;
    }
    else
    {
        pRes->RoutedTcbTail[REX_ARM_RES_ID]->RexRoutedNext = pTcbD;
        pRes->RoutedTcbTail[REX_ARM_RES_ID] = pTcbD;
    }

    rc = STDrvRunTask(1, pTcb);

    _ASSERT_RC(rc);

    REX_PROF_ADD_TIME(REX_PROF_ARM_RUN);

    return rc;
}


void RexResArmDone (LPVOID pDrvCtx, LPVOID p)
{
    PTCB pTCB;
    REXTCBD * pTcbD;
    PTASKLIST pList = NULL;
    REXRESOURCE * pRes;
    REXCTX * pCtx = RexGetCtx ();

    REX_PROF_DEF_TIME();

    _ASSERT_PTR (pCtx);

    _ASSERT(pCtx->MasterCpu == MxGetCpuID());

    pRes = &pCtx->Res [RESOURCE_LARM];

    while ((pTCB = STDGetReadyTcb(pDrvCtx)) != NULL)
    {
        pTcbD = (REXTCBD *)pTCB->RexDescr;
        _ASSERT_PTR(pTcbD);

        dbg_printf (REX_ARM_DONE, "REX:ARM-DONE, tcb-d:(%x), tcb:(%x)\r\n", pTcbD, pTCB);

        pTCB->ResourceIndex = 1;
        MLogEXEFinish(pTCB);

        pList = (PTASKLIST)pTcbD->RexList;
        _ASSERT_PTR(pList);

        if (pRes->RoutedTcbHead[REX_ARM_RES_ID] == NULL)
        {
            uart_printf ("REX:ARM-Done warning! TCB(0x%x), task-id=%d, list-id=%d is processed twice\r\n", pTCB, pTCB->TaskID, pList->ID);
            while (1);
        }

        pRes->RoutedTcbHead[REX_ARM_RES_ID] = (REXTCBD*)pRes->RoutedTcbHead[REX_ARM_RES_ID]->RexRoutedNext;

        _ASSERT((pTcbD->Ctrl & REX_CTRL_TCB_DONE) == 0);

        if ((pTcbD->Ctrl & REX_CTRL_TCB_DONE) != 0)
        {
            uart_printf ("REX:ARM-Done warning! TCB(0x%x), task-id=%d, list-id=%d is processed twice\r\n", pTCB, pTCB->TaskID, pList->ID);
            while (1);
        }

        if (pTcbD->CbDone != NULL)
        {
            // to call the application callback
            // if this callback is specified
            pTcbD->CbDone (pTCB, pTcbD->CbDonePtr, 0);
        }

        _ASSERT (pList->TcbDone <= pList->TcbNum);

        if ((pCtx->Ctrl & REX_CFG_STAT_SERV_ENABLED) == 0)
        {
            pTcbD->Ctrl |= REX_CTRL_TCB_DONE;
            pList->TcbDone ++;

        // if all elements of the list are processed
        // we need to put this list to the queue
        // to call the list callback

        if (pList->TcbDone == pList->TcbNum)
        {
            RexRegList (pCtx, pList);
        }

        ProcDependency (pCtx, pTcbD, 1);
    }
        else
        {
            RexServTcbIsDoneCmd(pTcbD->pHwTCB);
        }
    }

    _ASSERT_PTR(pList);

    REX_PROF_ADD_TIME(REX_PROF_ARM_DONE);

    // The kernel has to be called only for non-client/server
    // configuration

    if ((pCtx->Ctrl & REX_CFG_STAT_SERV_ENABLED) == 0)
    {
        RexKernelRun ();
    }
}


/** @brief This function puts the task to the resource queue,
           the main purpose of this function is to put TCB task
           according to the task priorities.

    @param pRes [in] - the pointer to the resource
    @param pTcbD[in] - the pointer to the TCB descriptor

    *  \ingroup group_lte_rexscheduler

    @return [MXRC] an error code */

MXRC RexResSchedTask (REXRESOURCE * pRes, REXTCBD * pTcbD)
{
    // To find the appropriate place for
    // the TCB according to the TCB priority

    REXTCBD * pPrevTCB = NULL, * pCurElm;

    REX_PROF_DEF_TIME();

    _ASSERT_PTR(pRes);
    _ASSERT_PTR(pTcbD);

    // The TCB block should be done, locked and bypassed

    if (pTcbD->Ctrl & (REX_CTRL_TCB_DONE | REX_CTRL_TCB_LOCKED | REX_CTRL_TCB_BYPASSED))
    {
        rex_printf ("\r\nREX:TCBD(%x), TCB(%x), id=%d, list-id=%d, tcb-ctrl=%x, list-ctrl=%x cannot be scheduled\r\n\r\n",

                pTcbD,
                pTcbD->pHwTCB,
                pTcbD->TaskID,
                ((PTASKLIST)pTcbD->RexList)->ID,
                pTcbD->Ctrl,
                ((PTASKLIST)pTcbD->RexList)->Ctrl
        );

        _ASSERT (!(pTcbD->Ctrl & (REX_CTRL_TCB_DONE | REX_CTRL_TCB_LOCKED | REX_CTRL_TCB_BYPASSED)));
        while (1);
    }

    pRes->Ctrl |= REX_RES_CTRL_UPDATED;

    pTcbD->RexDevNext = NULL;

    if (pRes->Head == NULL)
    {
        pRes->Head = pTcbD;
        pRes->Tail = pTcbD;
    }
    else
    {
        pCurElm = pRes->Head;

        while (pCurElm != NULL)
        {
            if (pTcbD->Priority < pCurElm->Priority)
            {
                if (pRes->Head == pCurElm)
                {
                    pTcbD->RexDevNext = pCurElm;
                    pRes->Head = pTcbD;
                }
                else
                {
                    _ASSERT_PTR(pPrevTCB);

                    pTcbD->RexDevNext = pPrevTCB->RexDevNext;
                    pPrevTCB->RexDevNext = pTcbD;
                }

                return MX_OK;
            }

            pPrevTCB = pCurElm;
            pCurElm = (REXTCBD *)pCurElm->RexDevNext;
        }

        // To put the element to the list tail

        pRes->Tail->RexDevNext = pTcbD;
        pRes->Tail = pTcbD;
    }

    REX_PROF_ADD_TIME(REX_PROF_SCHED_TASKS);

    return MX_OK;
}

/** @brief This function runs the scheduled(according to the priority) TCB tasks
           on specified HW accelerators

    @param pCtx[in] - the REX contex

    @return [MXRC] an error code */

MXRC RexResRunSchedTasks (REXCTX * pCtx)
{
    REXRESOURCE * pRes;
    REXTCBD * pTcbD, *pPrevTcbD, *pTcbDToRun;
    MXRC rc = MX_OK;

    REX_PROF_DEF_TIME();

    _ASSERT_PTR(pCtx);

    // ************************************************
    //                 CEVA driver                    *
    // ************************************************

    pRes = &pCtx->Res [RESOURCE_CEVA];

    if (pRes->Ctrl & REX_RES_CTRL_UPDATED)
    {
        pRes->Ctrl &= ~REX_RES_CTRL_UPDATED;

        while ((pRes->EnableMask != 0) && (pRes->Head != NULL))
        {
            pTcbD = pRes->Head;
            pRes->Head = (REXTCBD *)pTcbD->RexDevNext;

            if(FAILED(rc = pRes->Run (pCtx, pTcbD)))
            {
                _ASSERT_RC(rc);
                return rc;
            }
        }

        if (pRes->Head == NULL)
            pRes->Tail = NULL;
    }

    pRes = &pCtx->Res [REX_RESOURCE_CEVA_POOL_A];

    if (pRes->Ctrl & REX_RES_CTRL_UPDATED)
    {
        pRes->Ctrl &= ~REX_RES_CTRL_UPDATED;

        while ((pRes->EnableMask != 0) && (pRes->Head != NULL))
        {
            pTcbD = pRes->Head;
            pRes->Head = (REXTCBD *)pTcbD->RexDevNext;

            if(FAILED(rc = pRes->Run (pCtx, pTcbD)))
            {
                _ASSERT_RC(rc);
                return rc;
            }
        }

        if (pRes->Head == NULL)
            pRes->Tail = NULL;
    }

    pRes = &pCtx->Res [REX_RESOURCE_CEVA_POOL_B];

    if (pRes->Ctrl & REX_RES_CTRL_UPDATED)
    {
        pRes->Ctrl &= ~REX_RES_CTRL_UPDATED;

        while ((pRes->EnableMask != 0) && (pRes->Head != NULL))
        {
            pTcbD = pRes->Head;
            pRes->Head = (REXTCBD *)pTcbD->RexDevNext;

            if(FAILED(rc = pRes->Run (pCtx, pTcbD)))
            {
                _ASSERT_RC(rc);
                return rc;
            }
        }

        if (pRes->Head == NULL)
            pRes->Tail = NULL;
    }

    // ************************************************
    //                 L-ARM task driver              *
    // ************************************************

    pRes = &pCtx->Res [RESOURCE_LARM];

    if (pRes->Ctrl & REX_RES_CTRL_UPDATED)
    {
        pRes->Ctrl &= ~REX_RES_CTRL_UPDATED;

        while (pRes->Head != NULL)
        {
            pTcbD = pRes->Head;
            pRes->Head = (REXTCBD *)pTcbD->RexDevNext;

            if(FAILED(rc = pRes->Run (pCtx, pTcbD)))
            {
                _ASSERT_RC(rc);
                return rc;
            }
        }

        if (pRes->Head == NULL)
            pRes->Tail = NULL;
    }

    // ************************************************
    //                 FP driver                      *
    // ************************************************

    pRes = &pCtx->Res [RESOURCE_MAP];

    if (pRes->Ctrl & REX_RES_CTRL_UPDATED)
    {
        pRes->Ctrl &= ~REX_RES_CTRL_UPDATED;

        pPrevTcbD = NULL;
        pTcbD = pRes->Head;

        while (pTcbD != NULL)
        {
            if ((pTcbD->TaskID == TASKID_TX_IFFT && pRes->FFTNum == 0) ||
                (pTcbD->TaskID == TASKID_RX_FFT && pRes->IFFTNum == 0) ||
                (pTcbD->TaskID == TASKID_RX_IDFT))
            {
                pTcbDToRun = pTcbD;

                if (pRes->Head == pTcbD)
                {
                    pRes->Head = (REXTCBD *)pTcbD->RexDevNext;
                }
                else
                {
                    // we need to cut this element from this list

                    _ASSERT_PTR(pPrevTcbD);
                    pPrevTcbD->RexDevNext = pTcbD->RexDevNext;

                    if (pRes->Tail == pTcbD)
                    {
                        pRes->Tail = pPrevTcbD;
                    }
                }

                pTcbD = (REXTCBD *)pTcbD->RexDevNext;

                if(FAILED(rc = pRes->Run (pCtx, pTcbDToRun)))
                {
                    _ASSERT_RC(rc);
                    return rc;
                }

            }
            else
            {
                pPrevTcbD = pTcbD;
                pTcbD = (REXTCBD *)pTcbD->RexDevNext;
            }

        }

        if (pRes->Head == NULL)
            pRes->Tail = NULL;
    }

    REX_PROF_ADD_TIME(REX_PROF_RUN_SCHED_TASKS);

    return rc;
}

/** @brief To schedule tasks of the list

    @param pList [in] - the pointer to the task list

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexProcList (REXCTX * pCtx, PTASKLIST pList)
{
    REXTCBD *pProcTcbD;
    MXRC rc = MX_OK;

    REX_PROF_DEF_TIME();

    _ASSERT_PTR(pList);

    dbg_printf (REX_PROC_LIST, "REX:ProcList-begin, list(%x), list-id=%d, tcb_num=%d, tcb_done=%d\r\n", pList, pList->ID, pList->TcbNum, pList->TcbDone);

    // to lock the access to the
    // list under processing

#ifdef REX_TCB_LOCK_UNLOCK_ENABLED
    MxGetLock (&pList->Lock);
#endif

    // the main loop of REX scheduling
    // this code finds the appropriate TCB and routs it
    // to the resource handler

    while (pList->SchedHead)
    {
        // if the TCB is ready to be scheduled

        _ASSERT (pList->SchedHead->ResourceID < REX_SUPPORTED_RES);
        _ASSERT_PTR(pCtx->Res[pList->SchedHead->ResourceID].Run);

        pProcTcbD = (REXTCBD*)pList->SchedHead;
        pList->SchedHead = (volatile REXTCBD*)pProcTcbD->RexSchedNext;

        if (pProcTcbD->Ctrl & REX_CTRL_TCB_DONE)
        {
            rex_printf ("\r\n\r\nREX:ProcList: Detected double processing of TCB:\r\n");
            RexPrintTCBInfo(pProcTcbD);
            rex_printf ("\r\n\r\n");

            _ASSERT ((pProcTcbD->Ctrl & REX_CTRL_TCB_DONE) == 0);
        }

        pProcTcbD->Ctrl &= ~REX_CTRL_TCB_SCHEDULED;

        if (pProcTcbD->Ctrl & REX_CTRL_TCB_BYPASSED)
        {
            pProcTcbD->Ctrl |= REX_CTRL_TCB_DONE;

            dbg_printf (REX_BYPASS_PROCESSING, "REX-PROCLIST:Bypass is detected, tcb-d:(%x), tcb:(%x), depnum-%d, refnum-%d, list=%x, list-id=%d\r\n", pProcTcbD, pProcTcbD->pHwTCB, pProcTcbD->DepNum, pProcTcbD->RefNum, pProcTcbD->RexList, ((PTASKLIST)pProcTcbD->RexList)->ID);

            pList->TcbDone ++;

            ProcDependency (pCtx, pProcTcbD, 0);
        }
        else
        {
            #ifdef REX_TCB_PRIORITY_ENABLED
                if (FAILED(rc = RexResSchedTask(&pCtx->Res[pProcTcbD->ResourceID], pProcTcbD)))
                {
                    _ASSERT_RC(rc);
                    return rc;
                }
            #else
                if (FAILED(rc = pCtx->Res[pProcTcbD->ResourceID].Run(pCtx, pProcTcbD)))
                {
                    _ASSERT_RC(rc);
                    return rc;
                }
            #endif
        }
    }

    pList->SchedTail = NULL;

    _ASSERT (pList->TcbDone <= pList->TcbNum);

#ifdef REX_TCB_LOCK_UNLOCK_ENABLED
    // to unlock the processed list
    MxReleaseLock (&pList->Lock);
#endif

    // if the list is done
    if (pList->TcbNum == pList->TcbDone)
    {
        dbg_printf (REX_PROC_LIST_DONE, "CPU-%d, REX:ProcList-DONE, list(%x), list-id=%d, tcb_num=%d, tcb_done=%d\r\n", MxGetCpuID (), pList, pList->ID, pList->TcbNum, pList->TcbDone);
        RexCompleteList (pCtx, pList);
    }

    dbg_printf (REX_PROC_LIST, "REX:ProcList-end, list(%x), list-id=%d, tcb_num=%d, tcb_done=%d\r\n", pList, pList->ID, pList->TcbNum, pList->TcbDone);
    REX_PROF_ADD_TIME(REX_PROF_PROC_LIST);

    return rc;
}

PTASKLIST GetList (REXCTX * pCtx)
{
    PTASKLIST pList;

    MxGetLock (&pCtx->SchedListLock);

    if (pCtx->SchedListGet != pCtx->SchedListPut)
    {
        pList = pCtx->SchedLists[pCtx->SchedListGet++];

        if (pCtx->SchedListGet >= REX_MAX_LIST_NUM)
            pCtx->SchedListGet = 0;

        pList->Ctrl &= ~(REX_CTRL_LIST_IS_SCHEDULED|REX_CTRL_LIST_STARTED);

    }
    else
    {
        pList = NULL;
    }

    MxReleaseLock (&pCtx->SchedListLock);

    return pList;
}

/** @brief This function is added to process messages(commands) of REX server part
           and called automatically from rex_serv.c file

    @param pCmd [in] - the REX server/client command

    @return [MXRC] an error code of operation */

MXRC RexProcServCmd (RexCMD * pCmd)
{
    REXCTX * pCtx = RexGetCtx();
    UINT32 nBadCeva;

    _ASSERT_PTR(pCmd);
    _ASSERT_PTR(pCtx);

    switch (pCmd->CmdID)
    {
        case REX_CMD_LIST_IS_DONE:
        {
            RexCompleteListHandler(pCtx,
                                   pCmd->Param.ListIsDone.ListPtr,
                                   pCmd->Param.ListIsDone.ListID,
                                   pCmd->Param.ListIsDone.StartTicks,
                                   pCmd->Param.ListIsDone.CbDone,
                                   pCmd->Param.ListIsDone.CbDonePtr,
                                   pCmd->Param.ListIsDone.NumBadTcb);
            break;
        }

        case REX_CMD_EXEC_TCB:
        {
            TCB * pTcb = pCmd->Param.ExecTcb.pTcb;

            // At this point we need to route TCB to the driver
            // located on ARM processor

            _ASSERT_PTR(pTcb);
            _ASSERT(pTcb->ResourceID < REX_SUPPORTED_RES);
            _ASSERT_PTR(pCtx->Res[pTcb->ResourceID].RunClient);

            pCtx->Res[pTcb->ResourceID].RunClient(pCtx, pTcb);

            break;
        }

        case REX_CMD_EXEC_TCB_CB:
        {
            REXTCBCBDONE CbDone = pCmd->Param.ExecTcbCb.CbDone;

            if (CbDone != NULL)
            {
                CbDone( pCmd->Param.ExecTcbCb.pTcb,
                        pCmd->Param.ExecTcbCb.CbDonePtr,
                        pCmd->Param.ExecTcbCb.nErrCode);
            }

            break;
        }

        case REX_CMD_ERROR_CODE:
        {
            rex_printf("\r\n\r\n--------REX-SERVER ERROR----------");
            rex_printf("\r\n        Error: rc:[0x%x], data:[%d]",
                pCmd->Param.TcbError.ErrorCode,
                pCmd->Param.TcbError.ErrorData);
            rex_printf("\r\n        Descr: {%s}",
                RexServGetErrorDescr (pCmd->Param.TcbError.ErrorCode));
            rex_printf("\r\n----------------------------------\r\n");

            _ASSERT(pCmd->Param.TcbError.ErrorCode == 0);

            stop_printf("REX-SERVER returned error code(0x%x) {data:0x%x}",
                pCmd->Param.TcbError.ErrorCode,
                pCmd->Param.TcbError.ErrorData);

            break;
        }

        case REX_CMD_ASSERT:
        {
            rex_printf("\r\nREX-SERVER: ASSERT: rc=0x%x, line=%d\r\n",
                    pCmd->Param.TcbError.ErrorCode,
                    pCmd->Param.TcbError.ErrorLine);

            // --------------------------------------------------------
            // just to generate assert and to prevent compiler warnings
            // --------------------------------------------------------
            _ASSERT(pCmd->CmdID != REX_CMD_ASSERT);

            stop_printf("REX-SERVER assert rc=0x%x line=0x%x {data:0x%x}",
                pCmd->Param.TcbError.ErrorCode,
                pCmd->Param.TcbError.ErrorLine,
                pCmd->Param.TcbError.ErrorData);

            break;
        }

        case REX_CMD_BAD_CEVA_DETECTED:
        {
            nBadCeva = pCmd->Param.BadCevaDet.BadMask;

            if (pCtx->CbBadHW != NULL)
            {
                nBadCeva = pCtx->CbBadHW(RESOURCE_CEVA,
                                nBadCeva,
                                pCtx->CbBadHWPtr
                                );
            }
            else
            {
                #ifndef REX_REINIT_BAD_HW
                nBadCeva = 0;
                #endif
            }

            if (nBadCeva != 0)
            {
                nBadCeva = DspDrvReinitCeva(nBadCeva);

                if (nBadCeva != 0)
                {
                    RexServNotifyRestCevaCmd(nBadCeva);
                }
            }

            break;
        }

    }

    return MX_OK;
}

/** @brief This is a central component of the REX service, this code
            schedules TCB tasks to the resources and designed to process
            task lists

            *  \ingroup group_lte_rexscheduler

            */

MXRC RexKernelRun(void)
{
    REXCTX * pCtx = RexGetCtx ();
    PTASKLIST pList;
    MXRC rc = MX_OK;

    REX_PROF_DEF_TIME();

    _ASSERT_PTR(pCtx);

    dbg_printf (REX_KERNEL_RUN, "REX:Kernel, cpu=%d\r\n", MxGetCpuID());

    while ((pList = GetList(pCtx)) != NULL)
    {
        if ((pList->Ctrl & REX_CTRL_LIST_UNDER_PROC) == 0)
        {
            rex_printf ("\r\n\r\nREX:Kernel: List(id=%d, ctrl=%x) is not under processing!!!\r\n",
                pList->ID, pList->Ctrl);

            _ASSERT (!(pList->Ctrl & REX_CTRL_LIST_UNDER_PROC));
            continue;
        }

        rc = RexProcList (pCtx, pList);

        _ASSERT_RC(rc);
    }

#ifdef REX_TCB_PRIORITY_ENABLED
    rc = RexResRunSchedTasks(pCtx);
#endif

    REX_PROF_ADD_TIME(REX_PROF_KERNEL);
    return rc;
}


/** @brief This is IRQ (event) handler that is designed to start processing of the task list(s)

*  \ingroup group_lte_rexscheduler

*/

#ifndef REX_INJECT_CALL_INSTEAD_OF_IRQ

void RexEventHandler(volatile UINT32 * pClear, UINT32 nIrqID, LPVOID pParam)
{
    ICDrvCleanInterCpu(SOFT_ICPU_SCHEDULER);

    dbg_printf (REX_IC_RUN, "REX-IC#1\r\n");

    RexKernelRun ();

    dbg_printf (REX_IC_RUN, "REX-IC#2\r\n");

    *pClear = nIrqID;
}

#else

UINT32 RexEventHandler(LPVOID pParam)
{
    MXRC rc;
    
    dbg_printf (REX_IC_RUN, "REX-IC#1\r\n");
    rc = RexKernelRun ();
    dbg_printf (REX_IC_RUN, "REX-IC#2\r\n");
    
    return rc;
}

#endif

/** @brief This function generates interrupt to the destination processor
            where REX kernel in started to notify that new list is added

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC REX_INLINE RexRunExec (REXCTX * pCtx)
{
    _ASSERT_PTR(pCtx);

    #ifndef REX_INJECT_CALL_INSTEAD_OF_IRQ
        ICDrvRaiseInterCpu(pCtx->MasterCpu, SOFT_ICPU_SCHEDULER);
        return MX_OK;
    #else
        return MxInjectCode (pCtx->MasterCpu, RexEventHandler, pCtx);
    #endif
}

HANDLE RexGetHeap(UINT32 ctrl)
{
    if (ctrl & REX_CFG_STAT_SERV_ENABLED)
        return REX_SERV_HEAP;

    return REX_HEAP;
}

HANDLE RexGetSwHeap(UINT32 ctrl)
{
    if (ctrl & REX_CFG_STAT_SERV_ENABLED)
        return REX_SW_SERV_HEAP;

    return REX_SW_HEAP;
}

HANDLE RexGetListHeap (UINT32 ctrl)
{
    if (ctrl & REX_CFG_STAT_SERV_ENABLED)
        return REX_LIST_SERV_HEAP;

    return REX_LIST_HEAP;
}

/** @brief This function is designed to initialize REX service and
           should be called first before usage of any other API of this module

    @param pCfg [in] - configuration parameters

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexInit (REXCFG * pCfg)
{
    REXCTX * pCtx = RexGetCtx ();
    MXRC rc;
    UINT32 i;

    if (pCfg == NULL)
    {
        _ASSERT_PTR(pCfg);
        return REX_RC_PARAM_ERROR;
    }

    memset (&(rex_prof[0]), 0, sizeof (rex_prof));

    memset (pCtx, 0, sizeof (*pCtx));

    pCtx->Ctrl = pCfg->Ctrl;

    for (i = 0; i < REX_SUPPORTED_RES; i++)
        pCtx->Res[i].EnableMask = pCfg->Res[i];

    if (pCfg->TcbNum == 0)
        pCfg->TcbNum = (REX_MAX_TCB_NUM + REX_MAX_PRIVATE_TCB_NUM);

    if (pCfg->HwNum == 0)
        pCfg->HwNum = (REX_MAX_HW_CTX_NUM + REX_MAX_PRIVATE_TCB_NUM);

    if (pCfg->SwNum == 0)
        pCfg->SwNum = REX_MAX_SW_CTX_NUM;

    if (pCfg->ListNum == 0)
        pCfg->ListNum = REX_MAX_LIST_NUM;

    uart_printf ("\r\nREX service is started on core-%d\r\n", MxGetCpuID ());

    if (pCfg->Ctrl & REX_CFG_STAT_SERV_ENABLED)
        uart_printf ("Server/Client mode is selected\r\n");

    uart_printf (" *   CEVA mask = 0x%04x\r\n", pCfg->Res [RESOURCE_CEVA]);
    uart_printf (" * CEVA-A mask = 0x%04x\r\n", pCfg->Res [REX_RESOURCE_CEVA_POOL_A]);
    uart_printf (" * CEVA-B mask = 0x%04x\r\n", pCfg->Res [REX_RESOURCE_CEVA_POOL_B]);
    uart_printf (" *     TCB NUM = %d (tcbd-size=%d)\r\n", pCfg->TcbNum, REX_TCB_SIZE);
    uart_printf (" *      HW NUM = %d (heap:%s, Size: %d)\r\n", pCfg->HwNum, RexGetHeapName(REX_HW_HEAP), REX_TCB_CTX_SIZE * pCfg->HwNum);
    uart_printf (" *      SW NUM = %d\r\n", pCfg->SwNum);
    uart_printf (" *    LIST NUM = %d\r\n", pCfg->ListNum);

    // to create the completeness thread
    if (FAILED(rc = RexCreateCompleteThread (pCtx)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    if ((pCfg->Ctrl & REX_CFG_STAT_SERV_ENABLED) == 0)
    {
        // to create the TCB partition
        pCtx->pTcbPtr = MxHeapAlloc (RexGetHeap(pCfg->Ctrl), REX_TCB_SIZE * pCfg->TcbNum);
        pCtx->TcbStorSize = REX_TCB_SIZE * pCfg->TcbNum;

        if (pCtx->pTcbPtr == NULL)
        {
            _ASSERT_PTR(pCtx->pTcbPtr);
            return REX_RC_ALLOC_ERROR;
        }

        rc = MxCreatePart(pCtx->pTcbPtr, pCfg->TcbNum, REX_TCB_SIZE,
                      NULL, HNULL, HNULL, &pCtx->hTcbPart);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }
    }

    // to create the HW-TCB context partition
    if ((pCtx->pHwCtxPtr = MxHeapAlloc (REX_HW_HEAP, REX_TCB_CTX_SIZE * pCfg->HwNum))== NULL)
    {
        _ASSERT_PTR(pCtx->pHwCtxPtr);
        return REX_RC_ALLOC_ERROR;
    }

    pCtx->HwStorSize = REX_TCB_CTX_SIZE * pCfg->HwNum;

    rc = MxCreatePart(pCtx->pHwCtxPtr, pCfg->HwNum, REX_TCB_CTX_SIZE,
                  NULL, HNULL, HNULL, &pCtx->hHwCtxPart);

    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    // to create the SW-TCB context partition
    if ((pCtx->pSwCtxPtr = MxHeapAlloc (RexGetSwHeap(pCfg->Ctrl), REX_TCB_CTX_SIZE * pCfg->SwNum))== NULL)
    {
        _ASSERT_PTR(pCtx->pSwCtxPtr);
        return REX_RC_ALLOC_ERROR;
    }

    pCtx->SwStorSize = REX_TCB_CTX_SIZE * pCfg->SwNum;

    rc = MxCreatePart(pCtx->pSwCtxPtr, pCfg->SwNum, REX_TCB_CTX_SIZE,
                  NULL, HNULL, HNULL, &pCtx->hSwCtxPart);

    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    if ((pCfg->Ctrl & REX_CFG_STAT_SERV_ENABLED) == 0)
    {
        // to create the list partition
        pCtx->pListPtr = MxHeapAlloc (RexGetListHeap(pCfg->Ctrl), ROUND(sizeof (TASKLIST),32) * REX_MAX_LIST_NUM);

        if (pCtx->pListPtr == NULL)
        {
            _ASSERT_PTR(pCtx->pListPtr);
            return REX_RC_ALLOC_ERROR;
        }

        rc = MxCreatePart(pCtx->pListPtr, pCfg->ListNum, ROUND(sizeof (TASKLIST),32),
                      NULL, HNULL, HNULL, &pCtx->hListPart);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }
    }

    if ((pCfg->Ctrl & REX_CFG_STAT_SERV_ENABLED) == 0)
    {
        if (FAILED(rc = MxShareObject(pCtx->hTcbPart, OBJ_SHARE_ON)))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        if (FAILED(rc = MxShareObject(pCtx->hListPart, OBJ_SHARE_ON)))
        {
            _ASSERT_RC(rc);
            return rc;
        }
    }

    if (FAILED(rc = MxShareObject(pCtx->hHwCtxPart, OBJ_SHARE_ON)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    if (FAILED(rc = MxShareObject(pCtx->hSwCtxPart, OBJ_SHARE_ON)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    #ifndef REX_INJECT_CALL_INSTEAD_OF_IRQ
    // to register IRQ handler

    if (FAILED(rc = MxSetIntHandler(SOFT_ICPU_SCHEDULER, INT_TYPE_IRQ, PRI_REX, RexEventHandler, (LPVOID) NULL)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    if (FAILED(rc = MxEnableInt(SOFT_ICPU_SCHEDULER)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    #endif // REX_INJECT_CALL_INSTEAD_OF_IRQ

    // -----------------------------
    //     Table initialization
    // -----------------------------

    pCtx->Res [RESOURCE_CEVA].Run            = (RexRunType)RexResCevaRun;
    pCtx->Res [REX_RESOURCE_CEVA_POOL_A].Run = (RexRunType)RexResCevaRun;
    pCtx->Res [REX_RESOURCE_CEVA_POOL_B].Run = (RexRunType)RexResCevaRun;
    pCtx->Res [RESOURCE_MAP].Run             = (RexRunType)RexResFpRun;
    pCtx->Res [RESOURCE_MAP].RunClient       = (RexRunClientType)RexClientFpRun;
    pCtx->Res [RESOURCE_LARM].Run            = (RexRunType)RexResArmRun;
    pCtx->Res [RESOURCE_LARM].RunClient      = (RexRunClientType)RexClientArmRun;

    rc = STDrvAllocTable(REX_ARM_DRV_TASK_NUM, FALSE); //one time allocation of table to register the functions

    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    rc = STDrvRegNotifier((pCfg->Ctrl & REX_CFG_STAT_SERV_ENABLED) ? RexClientArmDone : RexResArmDone, NULL);

    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    /* In this case REX service is configured to use client and server parts
       the server part can be allocated on CEVA or on ARM for debug purpose*/

    if (pCfg->Ctrl & REX_CFG_STAT_SERV_ENABLED)
    {
        rc = RexServInit (pCfg);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            uart_printf ("REX: Server initialization is failed, cpu=%d, mode=0x%x, rc=0x%x\r\n", MxGetCpuID(), MxGetCpuMode(), rc);
        }
    }

    uart_printf ("\r\n");

    if (SUCCESSFUL(rc))
    {
        // to set master cpu
        pCtx->MasterCpu = MxGetCpuID ();
        pCtx->Ctrl |= REX_CFG_STAT_INITED;
    }

    MLogOpen();
    MLogSetMask(0xFFFFFFFF);
    MLogDevInfo(DevInfoGetHWID());

    MLogFREQ(RESOURCE_UARM0, ClkRstGetFreq(ARMx4_CORE0));
    MLogFREQ(RESOURCE_UARM1, ClkRstGetFreq(ARMx4_CORE1));
    MLogFREQ(RESOURCE_UARM2, ClkRstGetFreq(ARMx4_CORE2));
    MLogFREQ(RESOURCE_UARM3, ClkRstGetFreq(ARMx4_CORE3));


    MLogFREQ(RESOURCE_LARM0, ClkRstGetFreq(ARMx2_CORE0));
    MLogFREQ(RESOURCE_LARM1, ClkRstGetFreq(ARMx2_CORE1));

    MLogFREQ(RESOURCE_CEVA, ClkRstGetFreq(CR_DEV_CEVA_0));
    MLogFREQ(RESOURCE_FLTP, ClkRstGetFreq(CR_DEV_FP_0));

    MLogFREQ(RESOURCE_FECUL, ClkRstGetFreq(CR_DEV_FEC_UL));
    MLogFREQ(RESOURCE_FECDL, ClkRstGetFreq(CR_DEV_FEC_DL));

    MLogFREQ(RESOURCE_XP_AXI, ClkRstGetFreq(XP_AXI));
    MLogFREQ(RESOURCE_SYS_AXI, ClkRstGetFreq(SYS_AXI));
    MLogFREQ(RESOURCE_SPU_AXI, ClkRstGetFreq(SPU_AXI));
    MLogFREQ(RESOURCE_RAD_AXI, ClkRstGetFreq(RAD_AXI));

    MLogEnableRC(1);
    MLogResourceCondition (MLOG_MODE_SUPERVISOR, MxGetCpuID () + RESOURCE_UARM0, 0);

    MxDMB();
    MxDSB();

    return rc;
}

/** @brief This function enables/disables discovery flag, if it's enabled
           the system searches for the additional dependency when new one TCB is added to the list

    @param nEnabled [in] - the flag : 0 - to disable; 1 - to enable

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexSetDepDiscovery (UINT32 nEnabled)
{
    REXCTX * pCtx = RexGetCtx ();

    _ASSERT_PTR(pCtx);

    pCtx->DiscoverDep = nEnabled;

    return MX_OK;
}

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

TCB * RexCreateTCB (UINT32 TaskID, UINT32 ResourceID, UINT32* pContextData, UINT32 ContextDataLen, REXTCBCBDONE cbDone, LPVOID pPtr)
{
    return RexCreateTCBQ(TaskID, ResourceID, pContextData, ContextDataLen, cbDone, pPtr, 0);
}

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

TCB * RexCreateTCBQ (UINT32 TaskID, UINT32 ResourceID, UINT32* pContextData, UINT32 ContextDataLen, REXTCBCBDONE cbDone, LPVOID pPtr, UINT32 quantum)
{
    REXCTX * pCtx = RexGetCtx ();
    REXTCBD * pTcbD = NULL;
    TCB * pHwBlock;
    HANDLE hHeadPart;
    UINT32 HwResourceID;
    UINT32 rc;

    _ASSERT_PTR(pCtx);

    quantum = quantum * 150;

    if (!REX_IS_SERV_CFG(pCtx) && quantum != 0)
    {
        // just to generate assert, because
        // we do not support RexCreateTCBQ API
        // for non-server mode

        _ASSERT(quantum == 0);
        return NULL;
    }

    _ASSERT(ResourceID < REX_SUPPORTED_RES);

    switch (ResourceID)
    {
        case RESOURCE_CEVA:
        case REX_RESOURCE_CEVA_POOL_A:
        case REX_RESOURCE_CEVA_POOL_B:
            hHeadPart = pCtx->hHwCtxPart;
            HwResourceID = RESOURCE_CEVA;
            break;

        case RESOURCE_MAP:

            if (REX_IS_IDFT_ON_CEVA(pCtx))
                hHeadPart = pCtx->hHwCtxPart;
            else
                hHeadPart = pCtx->hSwCtxPart;

            HwResourceID = ResourceID;

            break;

        case RESOURCE_LARM:
            hHeadPart = pCtx->hSwCtxPart;
            HwResourceID = ResourceID;
            break;

        default:
            return NULL;
    }

    if ((pHwBlock = MxAllocBlock (hHeadPart)) == NULL)
    {
        _ASSERT_PTR(pHwBlock);
        return NULL;
    }

    memset (pHwBlock, 0, sizeof (*pHwBlock));

    pHwBlock->TaskID        = TaskID;
    pHwBlock->ResourceID    = HwResourceID;

    if (REX_IS_SERV_CFG(pCtx))
    {
        rc = RexServCreateTCBDCmd(TaskID, ResourceID, cbDone, pPtr, pHwBlock, quantum);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            MxFreeBlock (hHeadPart, pHwBlock);

            return NULL;
        }
    }
    else
    {
        pTcbD = (REXTCBD *)MxAllocBlock (pCtx->hTcbPart);

        if (pTcbD == NULL)
        {
            MxFreeBlock (hHeadPart, pHwBlock);
            _ASSERT_PTR(pTcbD);

            return NULL;
        }

        memset (pTcbD, 0, sizeof (*pTcbD));

        pTcbD->pHwTCB        = pHwBlock;
        pTcbD->ResourceID    = ResourceID;
        pTcbD->TaskID        = TaskID;
        pTcbD->CbDone        = cbDone;
        pTcbD->CbDonePtr     = pPtr;

        #ifdef REX_QUANTUM_ENABLED
        pTcbD->Quantum       = quantum;
        #endif

        pHwBlock->RexDescr   = pTcbD;
    }

    if (ContextDataLen != 0)
    {
        // in this way the pointer points directly to the
        // context specified by the application

        if (pContextData != NULL)
        {
            pHwBlock->ContextPtr = (LPVOID)pContextData;
        }
        else
        {
            // in this case the context pointer is initialized to point to the free storage
            pHwBlock->ContextPtr = (LPVOID)((UINT32)pHwBlock + sizeof (TCB));
        }

        pHwBlock->ContextLen = ContextDataLen;
    }

    switch (ResourceID)
    {
        case RESOURCE_CEVA:
        case REX_RESOURCE_CEVA_POOL_A:
        case REX_RESOURCE_CEVA_POOL_B:
            pHwBlock->cbDone = (pCtx-> Ctrl & REX_CFG_STAT_SERV_ENABLED) ? NULL : RexResCevaDone;
            break;
        case RESOURCE_MAP:
            pHwBlock->cbDone = (pCtx-> Ctrl & REX_CFG_STAT_SERV_ENABLED) ? RexClientFpDone : RexResFpDone;
            break;

        default:
            break;
    }

    dbg_printf (REX_CREATE_TCB, "REX:CreateTCB tcb-d:[%x], tcb:[%x], taskId=%d, resId=%d\r\n", pTcbD, pHwBlock, TCB_TASK_ID(pHwBlock), TCB_RES_ID(pHwBlock));

    MLogTCB(pHwBlock);
    return pHwBlock;
}

/** @brief The function deallocates memory reserved for the TCB block

    @param pTCB [in] - the removed TCB block

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexDestroyTCB (TCB * pTCB)
{
    REXCTX * pCtx = RexGetCtx ();
    MXRC rc = MX_OK;
    UINT32 i;
    TCB * pSup, *tmp;
    REXTCBD * pTcbD;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTCB);

    if (REX_IS_SERV_CFG(pCtx))
    {
        dbg_printf (REX_DESTROY_TCB, "REX-DestroyTCB: tcb-d(0x%x), tcb(0x%x), TaskID=%d, ResId=%d\r\n", pTCB->RexDescr, pTCB, pTCB->TaskID, pTCB->ResourceID);

        if (pTCB->RexDescr != NULL)
        {
            rc = RexServDestroyTCBDCmd((REXTCBD *)pTCB->RexDescr);

            if (FAILED(rc))
            {
                _ASSERT_RC(rc);
                return rc;
            }
        }
    }
    else
    {
        pTcbD = (REXTCBD *)pTCB->RexDescr;
        _ASSERT_PTR(pTcbD);

        dbg_printf (REX_DESTROY_TCB, "REX-DestroyTCB: List(0x%x), tcb(0x%x), TaskID=%d, ResId=%d\r\n", pTcbD->RexList, pTCB, pTCB->TaskID, pTCB->ResourceID);

        if (pTcbD->RexList != NULL)
        {
            RexCutFromSchedList ((PTASKLIST)pTcbD->RexList, pTcbD, 1);
            RexCutFromList (pTcbD);
        }

        for (i = 0; i < pTcbD->RefNum; i++)
        {
            _ASSERT_PTR (pTcbD->RefList [i]);

            pTcbD->RefList [i]->DepNum --;
        }

        // to remove the list of TCBs if this is the super-tcb block
        pSup = TCB_SUP_TCB(pTCB);

        while (pSup)
        {
            tmp = pSup;
            pSup = TCB_SUP_TCB(pSup);

            // to remove REX TCB descriptor
            if (tmp->RexDescr != NULL)
                MxFreeBlock (pCtx->hTcbPart, tmp->RexDescr);

            // to remove HW TCB block
            MxFreeBlock (RexGetCtxPart(pCtx, pTCB), tmp);
        }

        // to remove REX TCB descriptor
        if (pTcbD != NULL)
            MxFreeBlock (pCtx->hTcbPart, pTcbD);
    }

    // to remove HW TCB block
    MxFreeBlock (RexGetCtxPart(pCtx, pTCB), pTCB);

    return rc;
}

/** @brief The function can be used to mark TCB as BYPASSED
           The bypassed TCB is not going to be processed, this TCB
           just marks as completed

    @param pTCB [in] - the pointer to the TCB block
    @param nCtrl[in] - the control bits

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexMarkTCBControl (TCB * pTCB, UINT32 nCtrl)
{
    REXTCBD * pTcbD;
    MXRC rc = MX_OK;
    REXCTX * pCtx = RexGetCtx ();
    IRQTYPE irq = 0;
    PTASKLIST pList;

    _ASSERT_PTR(pTCB);
    _ASSERT_PTR(pCtx);

    // In this case we need to send command to the server part only if
    // client/server model is selected, and list is under processing
    // in other cases we can do it on ARM side immediately

    if (REX_IS_SERV_CFG(pCtx))
    {
        return RexServTcbCtrlCmd(1, pTCB, nCtrl);
    }

    pTcbD = pTCB->RexDescr;
    _ASSERT_PTR(pTcbD);

    pList = (PTASKLIST)pTcbD->RexList;

    nCtrl &= ~REX_BLOCKING_CALL;

    if (pList != NULL)
    {
        irq = ARM_INT_disable();
        MxGetLock (&pList->Lock);
    }

    //MLogMarkTcbControl(pTCB, nCtrl);

    if (pTcbD->RexList != NULL)
    {
        dbg_printf (REX_TCB_CTRL, "TCB-CTRL:(tcb-d:%x) (tcb:%x), ctrl:(%x), num:(%d)", pTcbD, pTCB, nCtrl, ((PTASKLIST)pTcbD->RexList)->TcbNum);
    }
    else
    {
        dbg_printf (REX_TCB_CTRL, "TCB-CTRL:(tcb-d:%x) (tcb:%x), ctrl:(%x)", pTcbD, pTCB, nCtrl);
    }

    if ((nCtrl & REX_BYPASS_TCB) != 0)
    {
        pTcbD->Ctrl |= REX_CTRL_TCB_BYPASSED;
    }
    else
    {
        pTcbD->Ctrl &= ~REX_CTRL_TCB_BYPASSED;
    }

    if (pList != NULL)
    {
        MxReleaseLock (&pList->Lock);
        ARM_INT_restore(irq);
    }

    return rc;
}

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

MXRC RexSetTCBControl (TCB * pTCB, UINT32 nCtrl)
{
    REXTCBD * pTcbD;
    MXRC rc = MX_OK;
    REXCTX * pCtx = RexGetCtx ();
    IRQTYPE irq = 0;
    PTASKLIST pList;

    _ASSERT_PTR(pTCB);
    _ASSERT_PTR(pCtx);

    // In this case we need to send command to the server part only if
    // client/server model is selected, and list is under processing
    // in other cases we can do it on ARM side immediately

    if (REX_IS_SERV_CFG(pCtx))
    {
        return RexServTcbCtrlCmd(1, pTCB, nCtrl);
    }

    _ASSERT_PTR(pList);

    pTcbD = pTCB->RexDescr;
    _ASSERT_PTR(pTcbD);

    pList = (PTASKLIST)pTcbD->RexList;

    //MLogMarkTcbControl(pTCB, nCtrl);

    if (pList != NULL)
    {
        dbg_printf (REX_TCB_CTRL, "TCB-SET-CTRL:(tcb-d:%x) (tcb:%x), ctrl:(%x), num:(%d)", pTcbD, pTCB, nCtrl, ((PTASKLIST)pTcbD->RexList)->TcbNum);

        irq = ARM_INT_disable();
        MxGetLock (&pList->Lock);
    }
    else
    {
        dbg_printf (REX_TCB_CTRL, "TCB-SET-CTRL:(tcb-d:%x) (tcb:%x), ctrl:(%x)", pTcbD, pTCB, nCtrl);
    }

    if ((nCtrl & REX_BYPASS_TCB) != 0)
    {
        pTcbD->Ctrl |= REX_CTRL_TCB_BYPASSED;
    }

    if ((nCtrl & REX_LOCKED_TCB) != 0)
    {
#ifdef REX_TCB_LOCK_UNLOCK_ENABLED

        pTcbD->Ctrl |= REX_CTRL_TCB_LOCKED;

        if (pList != NULL)
        {
            // Safe == 0 because the list is locked in this function
            RexCutFromSchedList (pList, pTcbD, 0);
        }
#else
        rc = REX_RC_LOCK_UNLOCK_DISABLED;
#endif
    }

    if (pList != NULL)
    {
        MxReleaseLock (&pList->Lock);
        ARM_INT_restore(irq);
    }

    return rc;
}

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

MXRC RexSetTCBControlEx (TCB ** ppTCB, UINT32 nTcbNum, UINT32 nCtrl)
{
    REXTCBD * pTcbD;
    MXRC rc = MX_OK;
    REXCTX * pCtx = RexGetCtx ();
    IRQTYPE irq = 0;
    UINT32 i = 0;
    PTASKLIST pLockList = NULL, pList;

    _ASSERT_PTR(ppTCB);
    _ASSERT_PTR(pCtx);

    // In this case we need to send command to the server part only if
    // client/server model is selected, and list is under processing
    // in other cases we can do it on ARM side immediately

    if (REX_IS_SERV_CFG(pCtx))
    {
        if (nTcbNum == 0)
        {
            return 0;
        }

        return RexServTcbCtrlExCmd(1, ppTCB, nTcbNum, nCtrl);
    }

    while (i < nTcbNum)
    {
        _ASSERT_PTR(ppTCB[i]);

        pTcbD = (REXTCBD *)ppTCB[i]->RexDescr;
        _ASSERT_PTR(pTcbD);

        pList = (PTASKLIST)pTcbD->RexList;
        _ASSERT_PTR(pList);

        if (pLockList == NULL)
        {
            pLockList = pList;

            irq = ARM_INT_disable();
            MxGetLock(&pLockList->Lock);
        }
        else if (pLockList != pList)
        {
            // in this way we need to report about error,
            // because TCB belongs to other List
            // and do not belong the aleady locked lick

            MxReleaseLock (&pLockList->Lock);
            ARM_INT_restore(irq);

            rex_printf("REX-SET-CTRLEX:Multiple list is detected\r\n");

            rc = REX_RC_MULT_LIST_DETECTED;
            _ASSERT_RC(rc);

            return rc;
        }

        dbg_printf (REX_TCB_CTRL, "TCB-SET-CTRLEX:(tcb-d:%x) (tcb:%x), ctrl:(%x), num:(%d)", pTcbD, ppTCB[i], nCtrl, ((PTASKLIST)pTcbD->RexList)->TcbNum);

        if ((nCtrl & REX_BYPASS_TCB) != 0)
        {
            pTcbD->Ctrl |= REX_CTRL_TCB_BYPASSED;
        }

        if ((nCtrl & REX_LOCKED_TCB) != 0)
        {
            #ifdef REX_TCB_LOCK_UNLOCK_ENABLED
                pTcbD->Ctrl |= REX_CTRL_TCB_LOCKED;
                if (pList != NULL)
                {
                    // Safe == 0 because the list is locked in this function
                    RexCutFromSchedList (pList, pTcbD, 0);
                }
            #else
                rc = REX_RC_LOCK_UNLOCK_DISABLED;
                break;
            #endif
        }

        i++;
    }

    if (pLockList != NULL)
    {
        MxReleaseLock (&pLockList->Lock);
        ARM_INT_restore(irq);
    }

    return rc;
}

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

MXRC RexResetTCBControl (TCB * pTCB, UINT32 nCtrl)
{
    REXTCBD * pTcbD;
    MXRC rc = MX_OK;
    REXCTX * pCtx = RexGetCtx ();
    IRQTYPE irq = 0;
    PTASKLIST pList;
    BOOL bRun = 0;

    _ASSERT_PTR(pTCB);
    _ASSERT_PTR(pCtx);

    // In this case we need to send command to the server part only if
    // client/server model is selected, and list is under processing
    // in other cases we can do it on ARM side immediately

    if (REX_IS_SERV_CFG(pCtx))
    {
        return RexServTcbCtrlCmd(0, pTCB, nCtrl);
    }

    pTcbD = pTCB->RexDescr;
    _ASSERT_PTR(pTcbD);

    pList = (PTASKLIST)pTcbD->RexList;

    _ASSERT_PTR(pList);

    //MLogMarkTcbControl(pTCB, nCtrl);

    // to protect multi-core access
    if (pList != NULL)
    {
        irq = ARM_INT_disable();
        MxGetLock (&pList->Lock);

        dbg_printf (REX_TCB_CTRL, "TCB-RESET-CTRL:(tcb-d:%x) (tcb:%x), ctrl:(%x), task-id:(%d), list-id:(%d) list-tcb-num:(%d), list-tcb-done:(%d)\r\n",
            pTcbD,
            pTCB,
            nCtrl,
            pTcbD->TaskID,
            ((PTASKLIST)pTcbD->RexList)->ID,
            ((PTASKLIST)pTcbD->RexList)->TcbNum,
            ((PTASKLIST)pTcbD->RexList)->TcbDone);
    }
    else
    {
        dbg_printf (REX_TCB_CTRL, "TCB-RESET-CTRL:(tcb-d:%x) (tcb:%x), ctrl:(%x), task-id:(%d), no-list\r\n",
            pTcbD,
            pTCB,
            nCtrl,
            pTcbD->TaskID);
    }

    if ((nCtrl & REX_BYPASS_TCB) != 0)
    {
        pTcbD->Ctrl &= ~REX_CTRL_TCB_BYPASSED;
    }

    // TO UNLOCK TCB processing if TCB was really locked

    if ((nCtrl & REX_LOCKED_TCB) != 0 && (pTcbD->Ctrl & REX_CTRL_TCB_LOCKED))
    {

#ifdef REX_TCB_LOCK_UNLOCK_ENABLED

        pTcbD->Ctrl &= ~REX_CTRL_TCB_LOCKED;

        if ((pList != NULL) && (pTcbD->DepDone == pTcbD->DepNum) && !(pTcbD->Ctrl & (REX_CTRL_TCB_DONE | REX_CTRL_TCB_SCHEDULED)))
        {
            if (SUCCESSFUL(rc = RexAddToScheduled(pList, pTcbD, 0)))
            {
                if (SUCCESSFUL(RexRegListWithLock (pCtx, pList)))
                {
                    bRun = 1;
                }
            }
        }
#else
        rc = REX_RC_LOCK_UNLOCK_DISABLED;
#endif

    }

    if (pList != NULL)
    {
        MxReleaseLock (&pList->Lock);
        ARM_INT_restore(irq);
    }

    if (bRun)
        RexRunExec(pCtx);

    return rc;
}

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

MXRC RexResetTCBControlEx (TCB ** ppTCB, UINT32 nTcbNum, UINT32 nCtrl)
{
    REXTCBD * pTcbD;
    MXRC rc = MX_OK;
    REXCTX * pCtx = RexGetCtx ();
    IRQTYPE irq = 0;
    PTASKLIST pLockList = NULL, pList;
    BOOL bRun = 0;
    UINT32 i = 0;

    _ASSERT_PTR(ppTCB);
    _ASSERT_PTR(pCtx);

    // In this case we need to send command to the server part only if
    // client/server model is selected, and list is under processing
    // in other cases we can do it on ARM side immediately

    if (REX_IS_SERV_CFG(pCtx))
    {
       if (nTcbNum == 0)
       {
           return 0;
       }

       return RexServTcbCtrlExCmd(0, ppTCB, nTcbNum, nCtrl);
    }

    while (i < nTcbNum)
    {
        _ASSERT_PTR(ppTCB[i]);

        pTcbD = ppTCB[i]->RexDescr;
        _ASSERT_PTR(pTcbD);

        pList = (PTASKLIST)pTcbD->RexList;
        _ASSERT_PTR(pList);

        if (pLockList == NULL)
        {
            pLockList = pList;

            irq = ARM_INT_disable();
            MxGetLock(&pLockList->Lock);
        }
        else if (pLockList != pList)
        {
            // in this way we need to report about error,
            // because TCB belongs to other List
            // and do not belong the aleady locked lick

            MxReleaseLock (&pLockList->Lock);
            ARM_INT_restore(irq);

            rex_printf("REX-RESET-CTRLEX:Multiple list is detected\r\n");

            rc = REX_RC_MULT_LIST_DETECTED;
            _ASSERT_RC(rc);

            return rc;
        }

        dbg_printf (REX_TCB_CTRL, "TCB-RESET-CTRLEX:(tcb-d:%x) (tcb:%x), ctrl:(%x), task-id:(%d), list-id:(%d) list-tcb-num:(%d), list-tcb-done:(%d)\r\n",
            pTcbD,
            ppTCB[i],
            nCtrl,
            pTcbD->TaskID,
            ((PTASKLIST)pTcbD->RexList)->ID,
            ((PTASKLIST)pTcbD->RexList)->TcbNum,
            ((PTASKLIST)pTcbD->RexList)->TcbDone);

        if ((nCtrl & REX_BYPASS_TCB) != 0)
        {
            pTcbD->Ctrl &= ~REX_CTRL_TCB_BYPASSED;
        }

        // TO UNLOCK TCB processing if TCB was really locked

        if ((nCtrl & REX_LOCKED_TCB) != 0 && (pTcbD->Ctrl & REX_CTRL_TCB_LOCKED))
        {
            #ifdef REX_TCB_LOCK_UNLOCK_ENABLED

                pTcbD->Ctrl &= ~REX_CTRL_TCB_LOCKED;
                if ((pList != NULL) && (pTcbD->DepDone == pTcbD->DepNum) && !(pTcbD->Ctrl & (REX_CTRL_TCB_DONE | REX_CTRL_TCB_SCHEDULED)))
                {
                    if (SUCCESSFUL(rc = RexAddToScheduled(pList, pTcbD, 0)))
                    {
                        if (SUCCESSFUL(RexRegListWithLock (pCtx, pList)))
                        {
                            bRun = 1;
                        }
                    }
                }

            #else
                rc = REX_RC_LOCK_UNLOCK_DISABLED;
                break;
            #endif

        }

        i++;
    }

    if (pLockList != NULL)
    {
        MxReleaseLock (&pLockList->Lock);
        ARM_INT_restore(irq);
    }

    if (bRun)
        RexRunExec(pCtx);

    return rc;
}

/** @brief This function creates/initializes TCB::IOControlPtr pointer

    @param pTCB     [in] - the pointer to the TCB block, see: CreateTCB function
    @param NumDesc  [in] - the number of descriptors, it's limited by TCB_MAX_IOBUFS

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexCreateIOBuf(PTCB pTCB, UINT32 NumDesc)
{
    MXRC rc = MX_OK;

    _ASSERT_PTR(pTCB);

    if (NumDesc > REX_TCB_MAX_IOBUFS)
    {
        rc = REX_RC_IO_NUM_ERROR;
        _ASSERT_RC(rc);
        return rc;
    }

    TCB_CONTROL(pTCB) = (LPVOID)((UINT32)pTCB + sizeof (TCB) + pTCB->ContextLen);
    TCB_CONTROL_LEN(pTCB) = 0;

    return MX_OK;
}

/** @brief This function sets IO control buffers

    @param pTCB     [in] - the TCB task
    @param pData    [in] - the pointer to the IO buffer (in/out)
    @param DataLen  [in] - the size of IO buffer
    @param DataType [in] - the type of buffer (INPUT/OUTPUT/BOTH)

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexFillTCBIOBuf(PTCB pTCB, void* pData, UINT32 DataLen, UINT32 DataType)
{
    TIOBufDesc * pIO;
    MXRC rc = MX_OK;

    _ASSERT_PTR(pTCB);

    // The pData cannot be checked due to the logics of the buffers usage
    _ASSERT_PTR (TCB_CONTROL(pTCB));

    switch (DataType & IO_MASK)
    {
        case IN_BUF:

            if (TCB_INPUT(pTCB) != NULL)
            {
                rc = REX_RC_IO_BUF_ERROR;
                _ASSERT_RC(rc);
                return rc;
            }

            break;

        case OUT_BUF:

            if (TCB_OUTPUT(pTCB) != NULL)
            {
                rc = REX_RC_IO_BUF_ERROR;
                _ASSERT_RC(rc);
                return rc;
            }

            break;

        default:
            break;
    }


    if ((TCB_CONTROL_LEN(pTCB) / sizeof (TIOBufDesc)) >= REX_TCB_MAX_IOBUFS)
    {
        rc = REX_RC_IO_NUM_ERROR;
        _ASSERT_RC (rc);

        return rc;
    }

    pIO = (TIOBufDesc *)((UINT32)TCB_CONTROL(pTCB) + TCB_CONTROL_LEN(pTCB));

    _ASSERT_PTR(pIO);

    pIO->IOBufPtr     = pData;
    pIO->IOBufControl = (DataType & IO_MASK) | (DataLen & BUF_SIZE_MASK);

    TCB_CONTROL_LEN(pTCB) += sizeof (TIOBufDesc);

    return rc;
}

/** @brief This function re-sets IO specific control buffers

    @param pTCB     [in] - the TCB task
    @param pData    [in] - the pointer to the IO buffer (in/out)
    @param DataLen  [in] - the size of IO buffer
    @param DataType [in] - the type of buffer (INPUT/OUTPUT/BOTH)
    @param IONum    [in] - the buffer index

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexReFillTCBIOBuf(PTCB pTCB, void* pData, UINT32 DataLen, UINT32 DataType, UINT32 IONum)
{
    TIOBufDesc * pIO;

    if (pTCB == NULL)
    {
        _ASSERT_PTR(pTCB);
        return REX_RC_PARAM_ERROR;
    }

    _ASSERT_PTR(pTCB);

    if(TCB_CONTROL(pTCB) == NULL)
    {
        _ASSERT_PTR(TCB_CONTROL(pTCB));
        return REX_RC_PARAM_ERROR;
    }

    if (IONum >= TCB_CONTROL_LEN(pTCB) / sizeof (TIOBufDesc))
    {
        _ASSERT (IONum < (TCB_CONTROL_LEN(pTCB) / sizeof (TIOBufDesc)));
        return REX_RC_PARAM_ERROR;
    }

    pIO = (TIOBufDesc *)((UINT32)TCB_CONTROL(pTCB) + IONum * sizeof (TIOBufDesc));

    pIO->IOBufControl = (DataType & IO_MASK) | (DataLen & BUF_SIZE_MASK);
    pIO->IOBufPtr     = pData;

    return MX_OK;
}

/** @brief This function returns IO control buffer specified by the index

    @param pTCB         [in] - the pointer to the TCB task
    @param nBufIndex    [in] - the index of the IO buffer

    @return [LPVOID] the pointer to the buffer

    *  \ingroup group_lte_rexscheduler

    */

LPVOID RexGetIOBuf(PTCB pTCB, UINT32 nBufIndex)
{
    TIOBufDesc * pDesc;

    if (pTCB == NULL)
    {
        _ASSERT_PTR(pTCB);
        return NULL;
    }

    if ((nBufIndex * sizeof(TIOBufDesc)) >= TCB_CONTROL_LEN(pTCB))
    {
        _ASSERT((nBufIndex * sizeof(TIOBufDesc)) < TCB_CONTROL_LEN(pTCB));
        return NULL;
    }

    pDesc = (TIOBufDesc *)((UINT32)TCB_CONTROL(pTCB) + (nBufIndex * sizeof(TIOBufDesc)));

    return pDesc->IOBufPtr;
}

/** @brief This function is designed to find and return IO buffer specified by the index and by the type

    @param pTCB     [in    ] - the pointer to the TCB task
    @param pLen     [out   ] - the pointer to the buffer length
    @param IOType   [in    ] - the type of data
    @param ioNum    [in/out] - the buffer index

    @return [LPVOID] the pointer to the buffer

    *  \ingroup group_lte_rexscheduler

    */

LPVOID RexGetIOBufByType(PTCB pTCB, UINT32* pLen, UINT32 IOType, UINT32 *ioNum)
{
    TIOBufDesc * pDesc;
    UINT32 nNum, i;

    _ASSERT_PTR(pLen);
    _ASSERT_PTR(ioNum);

    if (pTCB == NULL)
    {
        _ASSERT_PTR(pTCB);
        return NULL;
    }

    nNum = TCB_CONTROL_LEN(pTCB) / sizeof (TIOBufDesc);
    pDesc = (TIOBufDesc *)((UINT32)TCB_CONTROL(pTCB) + ((*ioNum) * sizeof(TIOBufDesc)));

    i = *ioNum;

    while (i < nNum)
    {
        if ((pDesc->IOBufControl & IOType) != 0)
        {
            *ioNum = i + 1;
            *pLen  = pDesc->IOBufControl & BUF_SIZE_MASK;

            return pDesc->IOBufPtr;
        }

        i++;
        pDesc++;
    }

    *pLen = 0;
    *ioNum= 0;

    return NULL;
}

/** @brief This function creates and initializes a task list, the task list can be scheduled for execution

    @param TaskListID [in] - the task list ID, the ID should be unique

    @return [PTASKLIST] the pointer to the allocated task list or NULL

    *  \ingroup group_lte_rexscheduler

    */

PTASKLIST RexCreateTaskList(UINT32 TaskListID)
{
    TASKLIST * pList;
    REXCTX * pCtx = RexGetCtx ();

    _ASSERT_PTR(pCtx);

    _ASSERT(TaskListID < REX_MAX_LIST_NUM);

    // to create the list on Server side if
    // Server mode is enabled

    if (REX_IS_SERV_CFG(pCtx))
    {
        pList = RexServCreateListCmd(TaskListID);
    }
    else
    {
        pList = (TASKLIST *)MxAllocBlock (pCtx->hListPart);

        if (pList == NULL)
        {
            _ASSERT_PTR(pList);
            return NULL;
        }

        memset (pList, 0, sizeof (*pList));
        pList->ID = TaskListID;
    }

    MLogTCBList(TaskListID);
    dbg_printf (REX_TCB_CREATE_TASK_LIST, "REX:CreateTaskList:%x, id=%d\r\n", pList, TaskListID);

    return pList;
}

/** @brief This function destroys a task list object and a list of TCBs of this task list

    @param pList [in] - the pointer to the task list

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexDestroyTaskList (PTASKLIST pList)
{
    MXRC rc = MX_OK;
    REXCTX * pCtx = RexGetCtx ();
    UINT32 nNum = REX_MAX_TCB_NUM, i;
    TCB ** pCramMem;

    _ASSERT_PTR (pList);
    _ASSERT_PTR (pCtx);

    if (REX_IS_SERV_CFG(pCtx))
    {
        pCramMem = (TCB**)MxHeapAlloc (DspHeap, nNum * sizeof (TCB*));

        if (pCramMem == NULL)
            return REX_RC_ALLOC_ERROR;
    
        dbg_printf (REX_DESTROY_TASK_LIST, "REX-DestroyTaskList: List(%x)\r\n", pList);
        rc = RexServDestroyListCmd(pList, pCramMem, &nNum);

        if (SUCCESSFUL(rc))
        {
            for (i = 0; i < nNum; i++)
            {
                MxFreeBlock (RexGetCtxPart(pCtx, pCramMem[i]), pCramMem[i]);
            }
        }

        MxHeapFree (DspHeap, pCramMem);
    }
    else
    {
        dbg_printf (REX_DESTROY_TASK_LIST, "REX-DestroyTaskList: List(%x), list_id=%d, elm_num=%d\r\n", pList, pList->ID, pList->TcbNum);

        if (pList->Ctrl & REX_CTRL_LIST_IS_SCHEDULED)
        {
            rc = REX_RC_LIST_IS_SCHEDULED;
            _ASSERT_RC(rc);

            return rc;
        }

        while (pList->TcbHead)
        {
            rc = RexDestroyTCB(pList->TcbHead->pHwTCB);

            if (FAILED(rc))
            {
                _ASSERT_RC(rc);
                return rc;
            }
        }

        MxFreeBlock (pCtx->hListPart, pList);
    }

    return rc;
}

/** @brief This function registers task list callback

    @param pList    [in] - the pointer to the task list
    @param cbDone   [in] - the pointer to the callback
    @param lpData   [in] - the callback parameter

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexRegListCBDone(TASKLIST* pList, REXCBDONE cbDone, LPVOID lpData)
{
    REXCTX * pCtx = RexGetCtx ();
    MXRC rc;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pList);

    if (pList == NULL)
        return REX_RC_PARAM_ERROR;

    if (REX_IS_SERV_CFG(pCtx))
    {
        rc = RexServRegCBDoneCmd(pList, cbDone, lpData);
    }
    else
    {
        pList->CbDone       = cbDone;
        pList->CbDoneParam  = lpData;

        rc = MX_OK;
    }

    return rc;
}

/** @brief This function schedules list for execution, when list is completed,
            the REX service will call specified callback function.

    @param pList [in] - the pointer to the allocated list

    @return [MXRC] an error code of operation

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexExecList (PTASKLIST pList)
{
    REXCTX * pCtx = RexGetCtx ();
    MXRC rc = MX_OK;

    DEF_ASSERT_LR();

    REX_PROF_DEF_TIME();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pList);

    if (REX_IS_SERV_CFG(pCtx))
    {
        // In this case REX is splitted to Client/Server parts
        // and we need to send commmand to server part to run the list

        rc = RexServExecListCmd(pList);

        if (FAILED(rc))
        {
            dbg_printf (REX_RUN_TASK_LIST, "***REX-CLIENT Exec-error, rc=0x%x list:(0x%x)\r\n", rc, pList);
            _ASSERT_RC_LR(rc);
            return rc;
        }
    }
    else
    {
        pList->StartTick = GetTIMETICK ();

        dbg_printf (REX_RUN_TASK_LIST, "***REX-RUN_TASK_LIST:(0x%x)(list-id=%d) (num-%d)\r\n", pList, pList->ID, pList->TcbNum);

        //if (pList->ID == 0)
        //    RexShowTaskList(pList);

        if (pList->Ctrl & REX_CTRL_LIST_UNDER_PROC)
        {
            stop_printf ("REX:ExecList: list(%x), list-id=%d, list-ctrl=%x is under processing\r\n", pList, pList->ID, pList->Ctrl);
            _ASSERT(!(pList->Ctrl & REX_CTRL_LIST_UNDER_PROC));
            return REX_RC_DOUBLE_SCHED;
        }

        pList->TcbDone  = 0;

        if (RexRegNewList (pCtx, pList))
        {
           // to generate the interrupt and run execution
           RexRunExec(pCtx);
        }

        // if the list was started in blocking mode,
        // the REX waits for the list completeness

        if (pList->CbDone == NULL)
        {
            while (1)
            {
                if (pList->Ctrl & REX_CTRL_LIST_IS_DONE)
                {
                    //RexCompleteList (pCtx, pList);
                    break;
                }
            }

            dbg_printf (REX_RUN_TASK_LIST, "***REX-Task list is DONE:(list=0x%x) (list-id=%d) (num-%d), ticks=%d\r\n", pList, pList->ID, pList->TcbNum, pList->StartTick);
        }
    }

    REX_PROF_ADD_TIME(REX_PROF_EXEC_LIST);

    return rc;
}


/** @brief This function schedules list for execution, when list is completed,
            the REX service will call specified callback function.

    @param pListX [in] - the pointer to the allocated list

    @return [MXRC] an error code of operation*/

MXRC RexExecList4 (PTASKLIST pList0, PTASKLIST pList1, PTASKLIST pList2, PTASKLIST pList3)
{
    REXCTX * pCtx = RexGetCtx ();
    MXRC rc = MX_OK;

    DEF_ASSERT_LR();

    REX_PROF_DEF_TIME();

    _ASSERT_PTR(pCtx);

    if (REX_IS_SERV_CFG(pCtx))
    {
        // In this case REX is splitted to Client/Server parts
        // and we need to send commmand to server part to run the list

        rc = RexServExecList4Cmd(pList0,pList1,pList2,pList3);

        if (FAILED(rc))
        {
            dbg_printf (REX_RUN_TASK_LIST, "***REX-CLIENT ExecLISTS-error, rc=0x%x list0:(0x%x) list1:(0x%x) list2:(0x%x) list3:(0x%x)\r\n", rc, pList0, pList1, pList2, pList3);
            _ASSERT_RC_LR(rc);
            return rc;
        }
    }
    else
    {
        if (pList0 != NULL)
        {
            if (FAILED(rc = RexExecList(pList0)))
                return rc;
        }

        if (pList1 != NULL)
        {
            if (FAILED(rc = RexExecList(pList1)))
                return rc;
        }

        if (pList2 != NULL)
        {
            if (FAILED(rc = RexExecList(pList2)))
                return rc;
        }

        if (pList3 != NULL)
        {
            if (FAILED(rc = RexExecList(pList3)))
                return rc;
        }
    }

    REX_PROF_ADD_TIME(REX_PROF_EXEC_LIST);

    return rc;

}

/** @brief This code routs one TCB block to the appropriate resource

    @param pTcb [in] - the pointer to the TCB task

    @return [MXRC] an error code


    \ingroup group_lte_rexscheduler

    */

MXRC RexExecTCB(TCB* pTcb)
{
    PTASKLIST pList;
    MXRC rc;

    _ASSERT_PTR (pTcb);

    pList = RexCreateTaskList (REX_MAX_LIST_NUM - 1);

    if (pList == NULL)
    {
        _ASSERT_PTR(pList);
        return REX_RC_ALLOC_ERROR;
    }

    rc = RexAddTCBToList (pList, pTcb, 0);

    if (FAILED(rc))
    {
        _ASSERT_RC (rc);

        RexDestroyTaskList (pList);
        return rc;
    }

    rc = RexExecList (pList);

    RexDestroyTaskList (pList);

    return rc;
}

/** @brief This function informs REX-S side about new TTI, in this way REX-S
           may upload prepared on previous TTI information to the DDR memory (like MLog)

    @return [MXRC] an error code of operation */

MXRC RexNewTTINotification(void)
{
    REXCTX * pCtx = RexGetCtx ();
    _ASSERT_PTR(pCtx);

    if (!REX_IS_SERV_CFG(pCtx))
        return REX_RC_REX_CS_IS_NOT_USED;

    return RexServNewTTICmd();
}

/** @brief This function uploads MLog to DDR memory,
           this function can be used in Client-Server model only

    @return [MXRC] an error code*/

MXRC RexUploadMLog(void)
{
    REXCTX * pCtx = RexGetCtx ();
    _ASSERT_PTR(pCtx);

    if (!REX_IS_SERV_CFG(pCtx))
        return REX_RC_REX_CS_IS_NOT_USED;

    return RexServUploadMLogCmd();
}

/** @brief The function adds TCB dependency, the dependency means that original TCB block depends on
            the specified one and cannot be executed prior dependent.

    @param pTcb         [in] - the pointer to the TCB that will be dependent on specified TCB task
    @param pDepOnThis   [in] - the dependence

    @return [MXRC] an error code

    \ingroup group_lte_rexscheduler

    */

MXRC RexAddDep (TCB* pTcb, TCB * pDepOnThis)
{
    MXRC rc = MX_OK;
    REXCTX * pCtx = RexGetCtx ();
    REXTCBD * pTcbD, *pDepOnThisD;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTcb);
    _ASSERT_PTR(pDepOnThis);

    if (REX_IS_SERV_CFG(pCtx))
    {
        if (FAILED(rc = RexServAddTcbDepCmd(pTcb, pDepOnThis)))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        MLogDepends(MLOG_DEPENDS_TCB_TCB, (UINT32)pTcb, (UINT32)pDepOnThis);

        return rc;
    }

    pTcbD = (REXTCBD *)pTcb->RexDescr;
    pDepOnThisD = (REXTCBD *)pDepOnThis->RexDescr;

    _ASSERT_PTR(pTcbD);
    _ASSERT_PTR(pDepOnThisD);

    dbg_printf (REX_ADD_DEP, "REX-AddDep, tcb-d:[%x] tcb:[%x] dep on tcb-d:[%x] tcb:[%x]\r\n", pTcbD, pTcb, pDepOnThisD, pDepOnThis);

    if ((pDepOnThisD->RefNum) >= REX_DEP_SIZE)
    {
        rc = REX_RC_DEP_OVERLOADED;
        _ASSERT_RC(rc);
        return rc;
    }

    // to check the dependency conflicts
    rc = RexIsDepConflict (pTcbD, pDepOnThisD);

    if (rc == REX_RC_SKIP)
    {
        dbg_printf (REX_ADD_DEP, "REX-ADDDEP is skipped, tcb-d:(%x), tcb:(%x) on tcb-d:(%x), tcb:(%x)\r\n", pTcbD, pTcb, pDepOnThisD, pDepOnThis);
        return MX_OK;
    }

    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    MLogDepends(MLOG_DEPENDS_TCB_TCB, (UINT32)pTcb, (UINT32)pDepOnThis);

    // to add a reference
    pDepOnThisD->RefList [pDepOnThisD->RefNum++] = pTcbD;

    // to increase the number of dependencies
    pTcbD->DepNum ++;

    dbg_printf(REX_ADD_DEP, " REX-ADDDEP:([%x].DepNum=%d dep on [%x].RefNum=%d)\r\n",
        pTcbD, pTcbD->DepNum, pDepOnThisD, pDepOnThisD->RefNum);

    // the sched list should contain only elements with 0 dependencies

    if (pTcbD->Ctrl & REX_CTRL_TCB_SCHEDULED)
    {
        // the element is already in the list
        // we need to reorganize the list according to the new priorities

        RexCutFromSchedList (pTcbD->RexList, pTcbD, 1);
    }

    return rc;
}


/** @brief This function is used to associate TCB with memory cell,
           this is specific dependencies that is used to handle HW accelerators
           when CELL value is 1, REX will treat this dependencies like 'done' and
           TCB can be scheduled if no other dependencies

    @NOTE!
            This functionality is applicable for REX-SERVER mode only (CEVA or ARM-emulator)

    @param  pTcb    [in] - the pointer to the TCB block accosiated with memory CELL
    @param  nAddr   [in] - the CELL address (it cannot be NULL)
    @param  nInitVal[in] - the initial value for memory cell (if cell value not equal to this value, the cell dep. is done)

    @return [MXRC] an error code of operation */

MXRC RexAddCellDep (TCB* pTcb, UINT32 nAddr, UINT32 nInitVal)
{
    REXCTX * pCtx = RexGetCtx ();

    if (!REX_IS_SERV_CFG(pCtx))
        return REX_RC_NOT_SUPPORTED;

    return RexServAddCellDepCmd(pTcb, nAddr, nInitVal);
}

/** @brief This function is used to reinitialize cells assigned to TCB blocks

    @NOTE!
            This functionality is applicable for REX-SERVER mode only (CEVA or ARM-emulator)

    @param  pCellList   [in] - the array of cell addresses
    @param  nNum        [in] - the array element number

    @return [MXRC] an error code of operation */


MXRC RexReinitCells (UINT32 * pCellList, UINT32 nNum)
{
    REXCTX * pCtx = RexGetCtx ();

    if (!REX_IS_SERV_CFG(pCtx))
        return REX_RC_NOT_SUPPORTED;

    return RexServReinitCellsCmd(pCellList, NULL, nNum);
}

/** @brief This function is used to reinitialize cells assigned to TCB blocks

    @NOTE!
            This functionality is applicable for REX-SERVER mode only (CEVA or ARM-emulator)

    @param  pCellList       [in] - the array of cell addresses
    @param  pNewCellAddrList[in] - the array of new cell addresses (addresses will be changed for current cells)
    @param  nNum            [in] - the array element number

    @return [MXRC] an error code of operation */


MXRC RexReinitCellsEx (UINT32 * pCellList, UINT32 * pNewCellAddrList, UINT32 nNum)
{
    REXCTX * pCtx = RexGetCtx ();

    if (!REX_IS_SERV_CFG(pCtx))
        return REX_RC_NOT_SUPPORTED;

    return RexServReinitCellsCmd(pCellList, NULL, nNum);
}

/** @brief The function adds TCB task to the specified task list according to the dependence weight priority

    @param pTaskList    [in] - the task list
    @param pTcb         [in] - the pointer to the TCB task that should be added to the list
    @param nTcbPrior    [in] - the TCB priority, 0 - highest priority, it's valid only if REX_TCB_PRIORITY_ENABLED is defined

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexAddTCBToList(PTASKLIST pTaskList, PTCB pTCB, UINT32 nTcbPrior)
{
    MXRC rc = MX_OK;
    REXCTX * pCtx = RexGetCtx ();
    REXTCBD * pTcbD;
    IRQTYPE r;
    UINT32 list_id = 0;

    _ASSERT_PTR (pCtx);
    _ASSERT_PTR (pTCB);
    _ASSERT_PTR (pTaskList);

    if (REX_IS_SERV_CFG(pCtx))
    {
        rc = RexServAddToListCmd(pTaskList, pTCB, nTcbPrior, &list_id);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        MLogTCBAddToList(pTCB, list_id);

        return rc;
    }

    pTcbD = (REXTCBD *)pTCB->RexDescr;
    _ASSERT_PTR(pTcbD);

    dbg_printf (REX_ADD_TO_LIST, "AddToList(list:%x, id=%d), tcb-d:(%x), tcb:(%x), TaskId:(%d), disvdep=%d\r\n", pTaskList,pTaskList->ID, pTcbD, pTCB, pTCB->TaskID, pCtx->DiscoverDep);

    if (pTaskList->Ctrl & (REX_CTRL_LIST_IS_SCHEDULED | REX_CTRL_LIST_UNDER_PROC | REX_CTRL_LIST_STARTED))
    {
        rex_printf ("REX:AddTCBToList: The list(id=%d) is under processing!!!\r\n", pTaskList->ID);

        rc = REX_RC_LIST_IS_SCHED;
        _ASSERT_RC (rc);
        return rc;
    }

    if ((pTcbD->RexList != NULL) || (pTcbD->Ctrl & REX_CTRL_TCB_IN_LIST))
    {
        rex_printf ("REX(1):TCBD(0x%x), TCB(0x%x), TaskID=%d, TCB-CTRL=0x%x is already in the list(%x), list-id=%d\r\n",
            pTcbD,
            pTcbD->pHwTCB,
            pTcbD->TaskID,
            pTcbD->Ctrl,
            pTcbD->RexList,
            (pTcbD->RexList!=NULL) ? ((PTASKLIST)pTcbD->RexList)->ID : (UINT32)0xFFFF
        );

        rc = REX_RC_DOUBLE_ADD_TO_LST;
        _ASSERT_RC(rc);
        return rc;
    }

    if (RexIsTcbInList (pTaskList, pTcbD))
    {
        rex_printf ("REX(2):TCBD(0x%x), TCB(0x%x), TaskID=%d, TCB-CTRL=0x%x is already in the list(%x), list-id=%d\r\n",
            pTcbD,
            pTcbD->pHwTCB,
            pTcbD->TaskID,
            pTcbD->Ctrl,
            pTcbD->RexList,
            (pTcbD->RexList!=NULL) ? ((PTASKLIST)pTcbD->RexList)->ID : (UINT32)0xFFFF
        );

        rc = REX_RC_DOUBLE_ADD_TO_LST;
        _ASSERT_RC(rc);
        return rc;
    }

    MLogTCBAddToList(pTCB, pTaskList->ID);

    if (pCtx->DiscoverDep)
    {
        dbg_printf (REX_ADD_TO_LIST, "Rex-AddTCBToList:DiscoverDep(LIST:(%x) TCB-d:(%x), TCB:(%x)\r\n", pTaskList, pTcbD, pTCB);

        rc = RexDiscoverDep (pTaskList, pTcbD);

        if (FAILED(rc))
        {
            rex_printf ("REX:AddTCBToList: list(id=%d) dependencies problem is detected \r\n", pTaskList->ID);

            _ASSERT_RC(rc);
            return rc;
        }
    }

    r = ARM_INT_disable();

    if (!REX_IS_SERV_CFG(pCtx))
        MxGetLock (&pTaskList->Lock);

    // ListTcb points to the element prior what new TCB should be added
    // to add the new one block to the list

    if (pTaskList->TcbHead == NULL)
    {
        pTaskList->TcbHead = pTcbD;
        pTaskList->TcbTail = pTcbD;

        pTcbD->RexTcbNext = NULL;
        pTcbD->RexTcbPrev = NULL;
    }
    else
    {
        pTaskList->TcbTail->RexTcbNext = pTcbD;
        pTcbD->RexTcbPrev = (REXTCBD*)pTaskList->TcbTail;

        pTaskList->TcbTail = pTcbD;
        pTcbD->RexTcbNext = NULL;
    }

    pTaskList->TcbNum ++;

    pTcbD->Priority = nTcbPrior;
    pTcbD->RexList  = pTaskList;
    pTcbD->Ctrl    |= REX_CTRL_TCB_IN_LIST;

    // If the number of dependency is 0 and this element is allowed to be scheduled
    // this element can be added to the list of already scheduled elements

    if ((pTcbD->DepNum == 0) && !(pTcbD->Ctrl & REX_CTRL_TCB_LOCKED))
    {
        RexAddToScheduled (pTaskList, pTcbD, 0);
    }

    if (!REX_IS_SERV_CFG(pCtx))
        MxReleaseLock (&pTaskList->Lock);

    ARM_INT_restore(r);

    dbg_printf (REX_ADD_TO_LIST, " element is added, list_tcb_num=%d\r\n", pTaskList->TcbNum);

    return rc;
}

/** @brief This function is designed to reinitialize the task list and to prepare it
           for the new scheduling

    @NOTE!  The list should not be scheduled for execution

    @param pList [in] - the pointer to the task list

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexReinitList (PTASKLIST pList)
{
    REXTCBD * pTcbD;
    MXRC rc = MX_OK;
    REXCTX * pCtx = RexGetCtx ();
    IRQTYPE r;
    UINT32 i;
    PTASKLIST pRemList;

    REX_PROF_DEF_TIME();

    _ASSERT_PTR(pList);

    // for the client/server configuration reinitialization is done
    // on the server part to save client ARM MIPS

    if (pCtx->Ctrl & REX_CFG_STAT_SERV_ENABLED)
    {
        return RexServReinitListCmd(pList);
    }

    dbg_printf (REX_REINIT_LIST, "ReInitList (%x), list_id=%d, elm_num=%d\r\n", pList, pList->ID, pList->TcbNum);

    if (pList->Ctrl & (REX_CTRL_LIST_IS_SCHEDULED | REX_CTRL_LIST_STARTED | REX_CTRL_LIST_UNDER_PROC))
    {
        rex_printf ("\r\n\r\nREX:List(id=%d) reinit cannot be done due to list status, ctrl=%x\r\n\r\n", pList->ID, pList->Ctrl);
        rc = REX_RC_REINIT_ERROR;
        _ASSERT_RC(rc);
        return rc;
    }

    r = ARM_INT_disable();
    MxGetLock(&pList->Lock);

    pList->TcbDone = 0;
    pList->Ctrl = 0;
    pList->SchedHead = NULL;
    pList->SchedTail = NULL;

    pTcbD = (REXTCBD *)pList->TcbHead;

    while (pTcbD)
    {
        // Clean up MLog
        //if (pTcbD->Ctrl & REX_CTRL_TCB_BYPASSED)
        //    MLogMarkTcbControl(pTcbD->pHwTCB, 0);

        pTcbD->DepDone      = 0;
        pTcbD->Ctrl         &= (REX_CTRL_TCB_IN_LIST);

        pTcbD->RexSchedNext = NULL;
        pTcbD->RexSchedPrev = NULL;

        pTcbD->RexDevNext   = NULL;
        pTcbD->RexRoutedNext= NULL;

        _ASSERT_PTR(pTcbD->pHwTCB);
        pTcbD->pHwTCB->Status = 0;

        // Check the cross-connections

        for (i = 0; i < pTcbD->RefNum; i++)
        {
            _ASSERT_PTR(pTcbD->RefList [i]);

            pRemList = (PTASKLIST)pTcbD->RefList [i]->RexList;

            // if TCB element that belongs to other list depends on this task
            // we need to decrease the DoneNum to show that this element
            // is not ready (because this list is reinitialized)

            if ((UINT32)pRemList != (UINT32)pTcbD->RexList && !(pRemList->Ctrl & REX_CTRL_LIST_UNDER_PROC))
            {
                MxGetLock(&pRemList->Lock);

                if (pTcbD->RefList [i]->DepDone != 0)
                    pTcbD->RefList [i]->DepDone --;

                MxReleaseLock(&pRemList->Lock);
            }
        }

        if ((pTcbD->DepNum == 0) && !(pTcbD->Ctrl & REX_CTRL_TCB_LOCKED))
        {
            rc = RexAddToScheduled (pList, pTcbD, 0);

            if (FAILED(rc))
            {
                _ASSERT_RC(rc);
                return rc;
            }
        }

        pTcbD = pTcbD->RexTcbNext;
    }

    MxReleaseLock(&pList->Lock);
    ARM_INT_restore(r);

    REX_PROF_ADD_TIME(REX_PROF_REINIT_LIST);

    return rc;
}

/** @brief This function is designed to set ARM task handler

    @param taskid [in] - the TaskID, see: TCB::TaskID
    @param cb     [in] - the handler

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexRegArmTask (UINT32 taskid, void (*cb)(TCB*p))
{
    return STDrvRegARMTask(taskid, cb);
}

/** @brief The system returns CPU core ID, on which REX service was initialized.
    @return [UINT32] core ID

    *  \ingroup group_lte_rexscheduler

    */

UINT32 RexGetMasterCpu (void)
{
    REXCTX * pCtx = RexGetCtx ();
    _ASSERT_PTR (pCtx);

    if (pCtx->Ctrl == 0)
        return 0xFFFF;

    return pCtx->MasterCpu;
}

/** @brief This function is designed to configure mask of available
           HW accelerators for some resource

    @param nResID   [in] - the resource ID, it can be:
                            REX_RESOURCE_CEVA_POOL_A
                            REX_RESOURCE_CEVA_POOL_B
    @param nResMask [in] - the bit-mask , 0 - to exclude the resource

    @return [MXRC] an error code */

MXRC RexConfigResource (UINT32 nResID, UINT32 nResMask)
{
    REXCTX * pCtx = RexGetCtx ();
    REXRESOURCE *pResCeva, *pCevaPool;
    UINT32 nMaskRes = 0, nMaskPool = 0;
    IRQTYPE irq;

    _ASSERT_PTR (pCtx);

    if (pCtx == NULL)
        return REX_RC_NOT_INITED;

    if ((nResID != REX_RESOURCE_CEVA_POOL_A) && (nResID != REX_RESOURCE_CEVA_POOL_B))
        return REX_RC_PARAM_ERROR;

    pResCeva = &pCtx->Res[RESOURCE_CEVA];
    pCevaPool= &pCtx->Res[nResID];

    irq = ARM_INT_disable();
    MxGetLock (&pCtx->SchedListLock);

    nMaskRes = pResCeva->EnableMask;
    nMaskPool= pCevaPool->EnableMask;

    // we need to reconfigure pool, so the first step is to
    // move assigned cevas to the general pool

    pResCeva->EnableMask |= pCevaPool->EnableMask;
    pCevaPool->EnableMask = 0;

    // if mask was set wrong, for example CEVA-0 failed BISR
    // and now is not in the general pool, we cannot assign it to
    // special pool, because the system will hang

    if ((pResCeva->EnableMask & nResMask) != nResMask)
    {
        // to restore the changed values

        pResCeva->EnableMask  = nMaskRes;
        pCevaPool->EnableMask = nMaskPool;

        ARM_INT_restore(irq);
        MxReleaseLock (&pCtx->SchedListLock);

        return REX_RC_POOL_MASK_ERROR;
    }

    // to exclude from general CEVA pool specified HW devices
    // also we take into account that nResMask cannot include
    // CEVAs that are not in general pool!

    pCevaPool->EnableMask = (pResCeva->EnableMask & nResMask);

    // to exclude from general pool CEVAs assigned for specific pool
    pResCeva->EnableMask ^= nResMask;

    ARM_INT_restore(irq);
    MxReleaseLock (&pCtx->SchedListLock);

    return MX_OK;
}

/** @brief This function shutdowns REX service and frees allocated resources
    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexShutdown (void)
{
    REXCTX * pCtx = RexGetCtx ();
    MXRC rc;

    _ASSERT_PTR (pCtx);

    if (pCtx->Ctrl == 0 || pCtx->MasterCpu != MxGetCpuID ())
        return MX_OK;

    if (REX_IS_SERV_CFG(pCtx))
    {
        RexServShutdown();
    }

    if (!REX_IS_SERV_CFG(pCtx))
    {
        if (FAILED(rc = MxShareObject(pCtx->hTcbPart, OBJ_SHARE_OFF)))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        if (FAILED(rc = MxRemovePart(pCtx->hTcbPart)))
        {
            _ASSERT_RC(rc);
            return rc;
        }
    }

    if (FAILED(rc = MxShareObject(pCtx->hHwCtxPart, OBJ_SHARE_OFF)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    if (FAILED(rc = MxRemovePart(pCtx->hHwCtxPart)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    if (FAILED(rc = MxShareObject(pCtx->hSwCtxPart, OBJ_SHARE_OFF)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    if (FAILED(rc = MxRemovePart(pCtx->hSwCtxPart)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    if (!REX_IS_SERV_CFG(pCtx))
    {
        if (FAILED(rc = MxShareObject(pCtx->hListPart, OBJ_SHARE_OFF)))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        if (FAILED(rc = MxRemovePart(pCtx->hListPart)))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        MxHeapFree (RexGetListHeap(pCtx->Ctrl), pCtx->pListPtr);
        MxHeapFree (RexGetHeap(pCtx->Ctrl),     pCtx->pTcbPtr);
    }

    MxHeapFree (REX_HW_HEAP, pCtx->pHwCtxPtr);
    MxHeapFree (RexGetSwHeap(pCtx->Ctrl),   pCtx->pSwCtxPtr);

    #ifndef REX_INJECT_CALL_INSTEAD_OF_IRQ
    if (FAILED(rc = MxDisableInt(SOFT_ICPU_SCHEDULER)))
    {
        _ASSERT_RC(rc);
        return rc;
    }
    #endif

    RexRemoveCompleteThread (pCtx);

    memset (pCtx, 0, sizeof (*pCtx));

    MLogClose();

    return MX_OK;
}

MXRC RexGenShedding(void)
{
    REXCTX * pCtx = RexGetCtx ();
    MXRC rc = MX_OK;

    _ASSERT_PTR (pCtx);

    if (pCtx->Ctrl == 0 || pCtx->MasterCpu != MxGetCpuID ())
        return rc;

    if (REX_IS_SERV_CFG(pCtx))
    {
        //uart_printf ("Stop REX-S\r\n");
        RexServShutdown();
    }

    #ifndef REX_INJECT_CALL_INSTEAD_OF_IRQ
    if (FAILED(rc = MxDisableInt(SOFT_ICPU_SCHEDULER)))
    {
        _ASSERT_RC(rc);
        return rc;
    }
    #endif

    return rc;
}

UINT32 RexGetListID(PTASKLIST pList)
{
    REXCTX * pCtx = RexGetCtx ();

    if (pList == NULL)
        return ~0;

    if (REX_IS_SERV_CFG(pCtx))
    {
        return RexServGetListIDCmd (pList);
    }

    return pList->ID;
}

UINT32 RexGetNumBadTcb(PTASKLIST pList)
{
    REXCTX * pCtx = RexGetCtx ();
    MXRC rc;
    RexCmdGetListInfo info;

    _ASSERT_PTR(pCtx);

    if (pList == NULL)
        return 0;

    if (!REX_IS_SERV_CFG(pCtx))
        return pList->TcbError;

    if (pCtx->CbDoneList == pList)
        return pCtx->CbDoneBadTcb;

    rc = RexServGetListInfoCmd(pList, &info);

    if (FAILED(rc))
        return ~0;

    return info.TcbError;
}


MXRC RexSetBadHWDetector(REXCBBADHW cb, void* ptr)
{
    REXCTX * pCtx = RexGetCtx ();
    _ASSERT_PTR(pCtx);

    pCtx->CbBadHWPtr = ptr;
    pCtx->CbBadHW = cb;

    return MX_OK;
}

/** @brief This function sends request to CEVA code to call some CEVA code function,
           defined by nProcAddr.

    @param CbDone    [in]       - the pointer to the ARM CB-DONE function
    @param Ptr       [in]       - the pointer to the cbDone argument
    @param nProcAddr [in]       - the address of CEVA function
    @param nQ        [in]       - the time in microseconds the called function has to be completed (0 - the feature is not used)
    @param nParamNum [in]       - the number of parameters
    @param ...       [in/out]   - the function parameters, optional and depends on CEVA function

    @return [UINT32] CEVA call handle*/

UINT32 RexCevaCall (CevaCallCbDoneType CbDone, void * Ptr, UINT32 nProcAddr, UINT32 nQ, UINT32 nParamNum, ...)
{
    REXCTX * pCtx = RexGetCtx ();
    va_list args;
    UINT32 params [16], i;

    _ASSERT_PTR(pCtx);

    if (!REX_IS_SERV_CFG(pCtx))
        return NULL;

    i = 0;

    va_start(args, nParamNum);
    while (i < nParamNum && i < sizeof (params)/sizeof(params[0]))
    {
        params [i] = va_arg(args, UINT32);
        i++;
    }
    va_end(args);

    return RexServCallCevaProc(CbDone, Ptr, nProcAddr, nQ, i, params);
}

/** @brief This function is designed to wait/check status of CEVA call
           started by RexCevaCall function.

    @Note!  ARM code may start CEVA code and than to check it completion

            handle = RexCevaCall(...);

            rc = RexCevaWaitCallComplete(handle, time, &ret);

    @param handle       [in] - the CEVA call handle
    @param time_is_us   [in] - the time in microsecods, 0 - to check and immediately return
    @param pRet         [in] - the pointer to storage for CEVA function return value (or NULL)

    @return [MXRC] an error code */

MXRC RexCevaWaitCallComplete(UINT32 handle, UINT32 time_is_us, UINT32 * pRet)
{
    REXCTX * pCtx = RexGetCtx ();

    _ASSERT_PTR(pCtx);

    if (!REX_IS_SERV_CFG(pCtx))
        return REX_RC_NOT_SUPPORTED;

    return RexServWaitCallCevaDone(handle, time_is_us, pRet);
}

/** @brief This code initializes super-TCB block

    @param GID          [in] - GroupID
    @param GroupHandle  [in] - the pointer to the previous TCB block or NULL
    @param pTaskList    [in] - the pointer to the task list (for MLog)
    @param numTCB       [in] - the number of parameters
    @param ...          [in] - TCB pointers

    @return [LPVOID] the pointer to the last element in the super TCB block

    *  \ingroup group_lte_rexscheduler

    */

LPVOID RexAddSuperTcb (UINT32 GID, LPVOID GroupHandle, PTASKLIST pTaskList, UINT32 numTCB, ...)
{
    va_list arguments;
    PTCB pTCB = NULL;
    PTCB pPrevTCB = (PTCB) GroupHandle;
    UINT32 i = 0;

    va_start(arguments, numTCB);

    dbg_printf (REX_ADD_SUPER_TCB, "TaskList(%x), num=%d, GID=%d {...}\r\n", pTaskList, numTCB, GID);

    while (numTCB--)
    {
        pTCB = va_arg(arguments, PTCB);
        pTCB->GroupID = GID;

        _ASSERT_PTR(pTaskList);

        // the first one TCB is already added
        if (i++ > 0)
            MLogTCBAddToList(pTCB, ((PTASKLIST)pTaskList)->ID); //part of new scheme

        if (pPrevTCB)
            pPrevTCB->NextSupTcb = pTCB;//forward link from prev TCB to this TCB

        pPrevTCB = pTCB; //last step
    }
    va_end(arguments);

    return (void *) pTCB; //pointer to last tcb added
}

/** @brief This function prints main information about TCB block

    @param pTcbD [in] - the pointer to the TCB task

    @return [MXRC] an error code */

MXRC RexPrintTCBInfo (REXTCBD * pTcbD)
{
    TCB * pTcb;

    _ASSERT_PTR (pTcbD);

    pTcb= (TCB *)pTcbD->pHwTCB;

    rex_printf ("TCBD:0x%x, TCB:0x%x, list-id=%d, res=%d, task=%d, d-ctrl=0x%x, IOCtrl=0x%x, IOCtx=0x%x, Inp=0x%x, Out=0x%x",
        pTcbD,
        pTcb,
        (pTcbD->RexList != NULL) ? (((PTASKLIST)pTcbD->RexList)->ID) : 1111111,
        pTcb->ResourceID,
        pTcb->TaskID,
        pTcbD->Ctrl,
        pTcb->IOControlPtr,
        pTcb->ContextPtr,
        pTcb->InputDataPtr,
        pTcb->OutputDataPtr);

    return MX_OK;
}

MXRC RexPrintCevaResInfo(REXRESOURCE * pRes)
{
    UINT32 i;
    REXTCBD* pBlock;

    _ASSERT_PTR(pRes);

    for (i = 0; i < sizeof (pRes->RoutedTcbHead)/sizeof (pRes->RoutedTcbHead[0]); i++)
    {
        pBlock = pRes->RoutedTcbHead[i];

        if (pBlock == NULL)
        {
            rex_printf("   *CEVA-%02d:NONE\r\n", i);
        }
        else
        {
            rex_printf("   *CEVA-%02d:", i);
            RexPrintTCBInfo(pBlock);
            rex_printf ("\r\n");
        }
    }

    rex_printf ("\r\n");

    return MX_OK;
}

MXRC RexPrintMapResInfo(REXRESOURCE * pRes)
{
    REXTCBD * pTcbD;
    _ASSERT_PTR(pRes);

    if (pRes->RoutedTcbHead[REX_IDFT_RES_ID] == NULL)
    {
        rex_printf("   *IDFT:NONE\r\n");
    }
    else
    {
        pTcbD = pRes->RoutedTcbHead[REX_IDFT_RES_ID];

        while (pTcbD != NULL)
        {
            rex_printf("   *IDFT:");
            RexPrintTCBInfo(pTcbD);
            rex_printf ("\r\n");

            pTcbD = (REXTCBD *)pTcbD->RexRoutedNext;
        }
    }

    if (pRes->RoutedTcbHead[REX_IFFT_FFT_RES_ID] == NULL)
    {
        rex_printf("   *IFFT:NONE\r\n");
        rex_printf("   * FFT:NONE\r\n");
    }
    else
    {
        pTcbD = pRes->RoutedTcbHead[REX_IFFT_FFT_RES_ID];

        while (pTcbD != NULL)
        {
            if (pTcbD->pHwTCB->TaskID == TASKID_TX_IFFT)
            {
                rex_printf("   *IFFT:");
            }
            else if (pTcbD->pHwTCB->TaskID == TASKID_RX_FFT)
            {
                rex_printf("   * FFT:");
            }

            RexPrintTCBInfo(pTcbD);
            rex_printf ("\r\n");

            pTcbD = (REXTCBD *)pTcbD->RexRoutedNext;
        }
    }

    rex_printf ("\r\n");

    return MX_OK;
}

MXRC RexPrintLARMResInfo(REXRESOURCE * pRes)
{
    REXTCBD * pTcbD;
    _ASSERT_PTR(pRes);

    if (pRes->RoutedTcbHead[0] == NULL)
    {
        rex_printf("   *ARM-TASK-DRV:NONE\r\n");
    }
    else
    {
        pTcbD = pRes->RoutedTcbHead[REX_ARM_RES_ID];

        while (pTcbD != NULL)
        {
            rex_printf("   *ARM-TASK-DRV:");
            RexPrintTCBInfo(pTcbD);
            rex_printf ("\r\n");

            pTcbD = (REXTCBD *)pTcbD->RexRoutedNext;
        }
    }

    return MX_OK;
}

MXRC RexPrintListInfo(TASKLIST *pList)
{
    REXCTX * pCtx = RexGetCtx ();

    _ASSERT_PTR(pList);
    _ASSERT_PTR(pCtx);

    rex_printf ("-----------LIST-----------\r\n");
    rex_printf ("REX-List ID:               %d\r\n", pList->ID);
    rex_printf ("REX-List tcb-num/tcb-done: %d/%d\r\n", pList->TcbNum, pList->TcbDone);
    rex_printf ("REX-List is:               [%s]\r\n", (pList->Ctrl & REX_CTRL_LIST_UNDER_PROC) ? "under processing" : "not scheduled");

    if (pList->Ctrl & REX_CTRL_LIST_IS_SCHEDULED)
        rex_printf ("REX-List status is         [scheduled for kernel]\r\n");

    if (pList->Ctrl & REX_CTRL_LIST_IS_DONE)
        rex_printf ("REX-List status is         [completed]\r\n");

    if (pList->Ctrl & REX_CTRL_LIST_STARTED)
        rex_printf ("REX-List status is         [scheduled and not processed yet]\r\n");

    if (pList->Ctrl & REX_CTRL_LIST_UNDER_PROC)
    {
        if (pList->SchedHead != NULL)
        {
            rex_printf ("  TCBD(0x%08x, task-id=%d, res=%d) is waiting to be scheduled\r\n   *",
                pList->SchedHead,
                pList->SchedHead->TaskID,
                pList->SchedHead->ResourceID);

            RexPrintTCBInfo((REXTCBD*)pList->SchedHead);
        }
        else
        {
            //rex_printf ("  No any TCB is waiting to be scheduled\r\n");
        }
     }

    rex_printf ("--------------------------\r\n");

    return MX_OK;
}

/** @brief This function is designed to print the information about task list
           number of elements, processed elements, which tasks were scheduled,
           etc ...

    @param nID [in] - the task list ID (if '-1' - for all scheduled task lists)

    @return [MXRC] an error code */

MXRC RexPrintListInformation (UINT32 nID)
{
    IRQTYPE r;
    UINT32 i;
    REXCTX * pCtx = RexGetCtx ();
    TASKLIST * pList;
    REXRESOURCE * pRes;

    _ASSERT_PTR (pCtx);

    r = ARM_INT_disable();
    MxGetLock (&pCtx->SchedListLock);

    rex_printf("\r\n  REX-scheduler resources:\r\n");
    rex_printf("  ~~~~~~~~~~~~~~~~~~~~\r\n");

    for (i = 0; i < REX_SUPPORTED_RES; i++)
    {
        pRes = &(pCtx->Res [i]);

        if ((i == RESOURCE_CEVA) || (i == REX_RESOURCE_CEVA_POOL_A) || (i == REX_RESOURCE_CEVA_POOL_B))
            RexPrintCevaResInfo (pRes);

        if (i == RESOURCE_MAP)
            RexPrintMapResInfo (pRes);

        if (i == RESOURCE_LARM)
            RexPrintLARMResInfo (pRes);
    }

    rex_printf("  ~~~~~~~~~~~~~~~~~~~~\r\n");

    for (i = 0; i < REX_MAX_LIST_NUM; i++)
    {
        pList = (TASKLIST *)((UINT32)pCtx->pListPtr + ROUND(sizeof (TASKLIST),32) * i);

        // ------------------------------------------------
        // if the block was allocated to contain the TCB list
        // ------------------------------------------------
        if (MxIsBlockAllocated(pCtx->hListPart, pList))
        {
            if ((pList->ID == nID) || (nID == ((UINT32)-1)))
            {
                RexPrintListInfo(pList);
            }
        }
    }

    MxReleaseLock (&pCtx->SchedListLock);
    ARM_INT_restore (r);

    return MX_OK;
}


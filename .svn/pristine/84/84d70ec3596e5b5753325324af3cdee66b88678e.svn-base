//-------------------------------------------------------------------------------------------
/** @file rexdiag.c
 *
 * @brief The declaration of the syscore RapidEXecutive (REX)
 * @author Mindspeed Technologies
 * @version $Revision: 1.16 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "rexdiag.h"
#include "appprintf.h"
#include "mlog.h"
#include "clkrst.h"
#include "heaps.h"
#include "svsrlogger.h"
#include "syscorecfg.h"
#include "fpdrv.h"
#include "rex_serv.h"
#include "ceva_proc.h"

#define REX_DIAG_TCB_HIGH_PRIOR     1
#define REX_DIAG_TCB_REGULAR_PRIOR  0

static NCNBVAR VUINT32 done = 0;
static NCNBVAR VUINT32 done_prior = 0;
static         VUINT32 start_time = 0;
static         VUINT32 end_time   = 0;

static NCNBVAR VUINT32 cell_dep[2];
static NCNBVAR UINT32  cell_addr[2];

void RexDiagCheckPriorCB (PTASKLIST pList, LPVOID lpData)
{
    _ASSERT_PTR(lpData);

    //uart_printf ("RexDiagCheckPriorCB, cpu-%d\r\n", MxGetCpuID ());

    (*(VUINT32*)lpData) = 1;
}

PTCB RexDiagCreateCopyTcb (UINT32 nInpSize)
{
    PTCB pTcb;

    pTcb = RexCreateTCBQ(TASKID_COPY_DATA, RESOURCE_CEVA, NULL, 0, 0, 0, 0);
    //pTcb = RexCreateTCBQ(TASKID_COPY_DATA, RESOURCE_CEVA, NULL, 0, 0, 0, 800);

    if (pTcb == NULL)
        return NULL;

    pTcb->InputDataPtr  = MxHeapAlloc (NcnbHeap, nInpSize);
    pTcb->OutputDataPtr = MxHeapAlloc (NcnbHeap, nInpSize);
    pTcb->InputDataLen  = nInpSize;
    pTcb->OutputDataLen = nInpSize;

    if (pTcb->InputDataPtr == NULL || pTcb->OutputDataPtr == NULL)
    {
        if (pTcb->InputDataPtr != NULL)
            MxHeapFree (NcnbHeap, pTcb->InputDataPtr);

        if (pTcb->OutputDataPtr != NULL)
            MxHeapFree (NcnbHeap, pTcb->OutputDataPtr);

        DestroyTCB (pTcb);
        return NULL;
    }

    memset (pTcb->InputDataPtr,  0x12, nInpSize);
    memset (pTcb->OutputDataPtr, 0x00, nInpSize);

    return pTcb;
}

BOOL RexDiagCheckCopyTcb (PTCB pTcb)
{
    UINT32 size, i;

    _ASSERT_PTR(pTcb);

    if (pTcb->InputDataPtr == NULL || pTcb->OutputDataPtr == NULL)
        return FALSE;

    size = MIN(pTcb->InputDataLen, pTcb->OutputDataLen);

    for (i = 0; i < size; i++)
    {
        if (((PUINT8)pTcb->InputDataPtr) [i] != ((PUINT8)pTcb->OutputDataPtr) [i])
            return FALSE;
    }

    return TRUE;
}

void RexDiagDestroyCopyTcb(PTCB pTcb)
{
    if (pTcb == NULL)
        return;

    if (pTcb->InputDataPtr != NULL)
        MxHeapFree (NcnbHeap, pTcb->InputDataPtr);

    if (pTcb->OutputDataPtr != NULL)
        MxHeapFree (NcnbHeap, pTcb->OutputDataPtr);
}


/** @brief This function is designed to check TCB priorities supported by REX

    @return [MXRC] an error code */

MXRC RexDiagCheckPriors (void)
{
#ifndef REX_TCB_PRIORITY_ENABLED
    uart_printf ("TCB priorities is not enabled, please see: REX_DIAG_RC_NOT_SUPPORTED macro\r\n\r\n");
    return REX_DIAG_RC_NOT_SUPPORTED;

#else 

    // To create the task list

    PTASKLIST pList, pListPrior;
    PTCB pTcb [20], pPriorTcb;
    MXRC rc = MX_OK, i;

    SYSCORECFG cfg;
    FEC_CONFIG_REGS FecConfigRegs;
    UINT32 nTime;
    REXPROF * pProf;

    memset (&FecConfigRegs, 0, sizeof (FecConfigRegs));
    memset (&cfg, 0, sizeof (cfg));

    FecConfigRegs.wimax_lte = FEC_HWCONF_LTE;
    FecConfigRegs.bp_rate_match = 0;
    FecConfigRegs.bp_channel_interleaver = 0;
    FecConfigRegs.ci_symbol_number = 0; // 12

    cfg.CpuDrivers [0] =    SYSCORE_FEC_DL_DRV      |
                            SYSCORE_FEC_UL_DRV      |
                            SYSCORE_SYS_MDMA_DRV    |
                            SYSCORE_SPU_MDMA_DRV    |
                            SYSCORE_RAD_MDMA_DRV;

    cfg.CpuDrivers [1] =    SYSCORE_CEVA_DRV        |
                            SYSCORE_FP_DRV          |
                            SYSCORE_SCHEDULER;

    cfg.FpMode = FPMODE_FFT1024;

    if (FAILED(rc = SysCoreConfig(&cfg)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    done = 0;
    done_prior = 0;

    // **********************************************************************
    //                   LIST-1 (one hight priority TCB)
    // **********************************************************************

    pListPrior = CreateTaskList (1, 0);
    _ASSERT_PTR(pList);

    if (pListPrior == NULL)
        return REX_DIAG_RC_LIST_CREATE_ERROR;

    RegListDoneCB(pListPrior, RexDiagCheckPriorCB, (LPVOID)&done_prior);    

    pPriorTcb = CreateTCB (TASKID_CLEARBUFF, RESOURCE_CEVA, 0, 0, 0, 0);
    _ASSERT_PTR(pPriorTcb);

    if (pPriorTcb == NULL)
        return REX_DIAG_RC_TCB_CREATE_ERROR;


    AddTCBToList (pPriorTcb, pListPrior, REX_DIAG_TCB_HIGH_PRIOR);


    // **********************************************************************
    //                   LIST-0 (just a list of copy data)
    // **********************************************************************
    
    pList = CreateTaskList (0, 0);
    _ASSERT_PTR(pList);

    if (pList == NULL)
        return REX_DIAG_RC_LIST_CREATE_ERROR;

    RegListDoneCB(pList, RexDiagCheckPriorCB, (LPVOID)&done);

    memset (pTcb, 0, sizeof (pTcb));

    // to create the task list with COPY_TASK, the task priority is <1> 
    // just for example

    for (i = 0; i < sizeof (pTcb)/sizeof (pTcb[0]); i++)
    {
        uart_printf ("Create TCB[%d] ... ", i);
        MxDelayTicks (150*1000);
    
        pTcb[i] = RexDiagCreateCopyTcb(1024*5);
        _ASSERT_PTR(pTcb[i]);

        if (pTcb[i] == NULL)
        {
            i --;
        
            while (i != 0)
            {
                RexDiagDestroyCopyTcb (pTcb[i]);
                i--;
            }
        
            uart_printf ("REX: Create TCB error, i = %d\r\n", i);
            MxDelayTicks (150*1000);
            return REX_DIAG_RC_TCB_CREATE_ERROR;
        }
        else
        {
            uart_printf ("OK\r\n");
            MxDelayTicks (150*1000);
        }

        if (i < 12)
            AddTCBToList (pTcb[i], pList, REX_DIAG_TCB_REGULAR_PRIOR);
        else
            AddTCBToList (pTcb[i], pList, 100);
    }

    uart_printf ("Start task lists[%d/%d] ... ", pList->ID, pListPrior->ID);
    MxDelayTicks (150*1000);

    nTime = MxGetTicks();

    rc = ExeTSchedList(pList);
    _ASSERT_RC(rc);

    rc = ExeTSchedList(pListPrior);
    _ASSERT_RC(rc);

    while (done == 0 || done_prior == 0 )
    {
        _NOP();
    }

    nTime = MxGetTicks() - nTime;

    uart_printf ("DONE\r\nTotal time is %d\r\n", nTime);

    for (i = 0; i < sizeof (pTcb)/sizeof (pTcb[0]); i++)
    {
        if (!RexDiagCheckCopyTcb (pTcb[i]))
        {
            uart_printf ("TCB [%d] - error\r\n", i);
            MxDelayTicks (150*1000);
        }
        else
        {
            //uart_printf ("TCB [%d] - OK\r\n", i);
            //MxDelayTicks (150*1000);
        }
    }

    {
        UINT32 nID [] = {REX_PROF_CEVA_RUN, 
                         REX_PROF_CEVA_DONE,
                         REX_PROF_KERNEL,
                         REX_PROF_PROC_LIST,
                         REX_PROF_EXEC_LIST,
                         REX_PROF_SCHED_TASKS,
                         REX_PROF_RUN_SCHED_TASKS,
                         REX_PROF_CEVA_RUN_FROM_CEVA_DONE,
                         REX_PROF_FP_RUN_FROM_FP_DONE,
                         REX_PROF_CALL_CB,
                         (UINT32)-1};

        const char * pIDName [] = { "REX_PROF_CEVA_RUN",
                                    "REX_PROF_CEVA_DONE",
                                    "REX_PROF_KERNEL",
                                    "REX_PROF_PROC_LIST",
                                    "REX_PROF_EXEC_LIST",
                                    "REX_PROF_SCHED_TASKS",
                                    "REX_PROF_RUN_SCHED_TASKS",
                                    "REX_PROF_CEVA_RUN_FROM_CEVA_DONE",
                                    "REX_PROF_FP_RUN_FROM_FP_DONE",
                                    "REX_PROF_CALL_CB",
                                    NULL};
        i = 0;

        while (pIDName [i]!=NULL)
        {
            pProf = RexGetProfInfo ((REX_ENUM_PROF_ID)nID[i]);
            _ASSERT_PTR(pProf);

            if (pProf->TotalNum != 0)
                uart_printf (" - %s: ticks:%d, num:%d, avg:%d, max:%d, min:%d\r\n", pIDName[i], pProf->TotalTime, pProf->TotalNum, pProf->TotalTime / pProf->TotalNum, pProf->MaxTime, pProf->MinTime);
            
            MxDelayTicks (150*1000);
            i++;
        }
    }

    i = RexGetProfInfo(REX_PROF_KERNEL)->TotalTime + RexGetProfInfo(REX_PROF_CEVA_DONE)->TotalTime;
    uart_printf ("KERNEL+CBDone: %d ticks (%d.%d us)\r\n", i, i / 150, ((i) / 15)%10);

    uart_printf ("The test is done, please use Mlog to check\r\n");

    DestroyTaskList (pListPrior);
    DestroyTaskList (pList);

    SvsrLoggerFlush();
    SvsrLoggerSetMask(0);

    MLogPrint();
    MLogSetMask(0);
    MLogClose();

    return rc;

#endif    
}

void MyCevaProcCbDone(void*p, UINT32 res)
{
    end_time = MxGetTicks ();
    //uart_printf ("CEVA cbDone: ptr:0x%x, ret:0x%x, time=%d\r\n", (UINT32)p,res, end_time - start_time);
    done = 1;
}

MXRC RexDiagCheckSimpleList (void)
{
    // To create the task list

    PTASKLIST pList;
    PTCB pTcb [100];
    MXRC rc = MX_OK, i, cell_dep_enabled = 0;

    SYSCORECFG cfg;
    FEC_CONFIG_REGS FecConfigRegs;
    UINT32 nTime;
    REXPROF * pProf;

    memset (&FecConfigRegs, 0, sizeof (FecConfigRegs));
    memset (&cfg, 0, sizeof (cfg));

    FecConfigRegs.wimax_lte = FEC_HWCONF_LTE;
    FecConfigRegs.bp_rate_match = 0;
    FecConfigRegs.bp_channel_interleaver = 0;
    FecConfigRegs.ci_symbol_number = 0; // 12

    cfg.CpuDrivers [0] =    SYSCORE_FEC_DL_DRV      |
                            SYSCORE_FEC_UL_DRV      |
                            SYSCORE_SYS_MDMA_DRV    |
                            SYSCORE_SPU_MDMA_DRV    |
                            SYSCORE_RAD_MDMA_DRV;

    cfg.CpuDrivers [0] |=   SYSCORE_CEVA_DRV        |
                            SYSCORE_FP_DRV          |
                            SYSCORE_SCHEDULER;

    cfg.FpMode = FPMODE_FFT1024;

    if (FAILED(rc = SysCoreConfig(&cfg)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    RexNewTTINotification();
    
    done = 0;

    cell_dep [0] = 0;
    cell_dep [1] = 0;

    // **********************************************************************
    //                   LIST-0 (just a list of copy data)
    // **********************************************************************
    
    pList = CreateTaskList (0, 0);
    _ASSERT_PTR(pList);

    if (pList == NULL)
        return REX_DIAG_RC_LIST_CREATE_ERROR;

    RegListDoneCB(pList, RexDiagCheckPriorCB, (LPVOID)&done);

    memset (pTcb, 0, sizeof (pTcb));

    // to create the task list with COPY_TASK, the task priority is <1> 
    // just for example

    for (i = 0; i < sizeof (pTcb)/sizeof (pTcb[0]); i++)
    {
        pTcb[i] = RexDiagCreateCopyTcb(1024*5);
        _ASSERT_PTR(pTcb[i]);

        if (pTcb[i] == NULL)
        {
            uart_printf ("Create TCB[%d] - error!\r\n", i);
            MxDelayTicks (150*1000);
        
            i --;
        
            while (i != 0)
            {
                RexDiagDestroyCopyTcb (pTcb[i]);
                i--;
            }
        
            uart_printf ("REX: Create TCB error, i = %d\r\n", i);
            MxDelayTicks (150*1000);
            return REX_DIAG_RC_TCB_CREATE_ERROR;
        }

        AddTCBToList (pTcb[i], pList, REX_DIAG_TCB_REGULAR_PRIOR);

        #ifdef REX_S_FORCED_ENABLED
        if (i < (sizeof (cell_dep)/sizeof(cell_dep[0])) )
        {
            if (RexAddCellDep (pTcb[i], (UINT32)&(cell_dep[i]), 0xFF) == MX_OK)
            {
                uart_printf ("Added cell dep for TCB[%x] -> [%x]\r\n", pTcb[i], (UINT32)&(cell_dep[i]));
                cell_dep_enabled = 1;
            }
        }
        #endif
        
    }

    //uart_printf ("Start task lists[%d] ... ", RexGetListID(pList));
    //MxDelayTicks (150*1000);

    nTime = MxGetTicks();

    rc = RexExecList4(pList, 0, 0, 0);
    _ASSERT_RC(rc);

    while (done == 0)
    {
        _NOP();

        if (MxGetTicks() - nTime >= 150000000 * 2)
            break;

        if (cell_dep_enabled)
        {
            if (MxGetTicks() - nTime >= 1500)
            {
                cell_dep[0] = 1;
                cell_dep[1] = 1;

                //uart_printf ("[Cell-Dep set] ");

                cell_dep_enabled = 0;
            }
        }
    }

    nTime = MxGetTicks() - nTime;

    uart_printf ("List is done with [%s]\r\n", done ? "OK" : "TIMEOUT");

    #ifdef REX_S_FORCED_ENABLED
    if (1)
    {
        UINT32 h;

        done = 0;
        start_time = MxGetTicks ();
        h = ceva_call2_cb_q(MyCevaProcCbDone, (void*)0x123, ceva_dummy_func, 800, 1, 2);
        if (h == 0)
            uart_printf ("CEVA CALL is error!\r\n");
        else
            while (done == 0);

        MLogTask(0x1000, RESOURCE_LARM, start_time, end_time);
    }
    #endif

    //uart_printf ("DONE\r\nTotal time is %d\r\n", nTime);

    cell_addr[0] = (UINT32)(&cell_dep[0]);
    cell_addr[1] = (UINT32)(&cell_dep[1]);

    #ifdef REX_S_FORCED_ENABLED
    RexReinitCells(cell_addr, 2);
    #endif

    for (i = 0; i < sizeof (pTcb)/sizeof (pTcb[0]); i++)
    {
        if (!RexDiagCheckCopyTcb (pTcb[i]))
        {
            uart_printf ("TCB [%d] - error\r\n", i);
            MxDelayTicks (150*1000);
        }
        else
        {
            //uart_printf ("TCB [%d] - OK\r\n", i);
            //MxDelayTicks (150*1000);
        }
    }

    uart_printf ("CELL[0]=%d, CELL[1]=%d\r\n", cell_dep[0], cell_dep[1]);

    {
        UINT32 nID [] = {REX_PROF_CEVA_RUN, 
                         REX_PROF_CEVA_DONE,
                         REX_PROF_KERNEL,
                         REX_PROF_PROC_LIST,
                         REX_PROF_EXEC_LIST,
                         REX_PROF_SCHED_TASKS,
                         REX_PROF_RUN_SCHED_TASKS,
                         REX_PROF_CEVA_RUN_FROM_CEVA_DONE,
                         REX_PROF_FP_RUN_FROM_FP_DONE,
                         REX_PROF_CALL_CB,
                         (UINT32)-1};

        const char * pIDName [] = { "REX_PROF_CEVA_RUN",
                                    "REX_PROF_CEVA_DONE",
                                    "REX_PROF_KERNEL",
                                    "REX_PROF_PROC_LIST",
                                    "REX_PROF_EXEC_LIST",
                                    "REX_PROF_SCHED_TASKS",
                                    "REX_PROF_RUN_SCHED_TASKS",
                                    "REX_PROF_CEVA_RUN_FROM_CEVA_DONE",
                                    "REX_PROF_FP_RUN_FROM_FP_DONE",
                                    "REX_PROF_CALL_CB",
                                    NULL};
        i = 0;

        while (pIDName [i]!=NULL)
        {
            pProf = RexGetProfInfo ((REX_ENUM_PROF_ID)nID[i]);
            _ASSERT_PTR(pProf);

            if (pProf->TotalNum != 0)
                uart_printf (" - %s: ticks:%d, num:%d, avg:%d, max:%d, min:%d\r\n", pIDName[i], pProf->TotalTime, pProf->TotalNum, pProf->TotalTime / pProf->TotalNum, pProf->MaxTime, pProf->MinTime);
            
            MxDelayTicks (150*1000);
            i++;
        }
    }

    i = RexGetProfInfo(REX_PROF_KERNEL)->TotalTime + RexGetProfInfo(REX_PROF_CEVA_DONE)->TotalTime;
    uart_printf ("KERNEL+CBDone: %d ticks (%d.%d us)\r\n", i, i / 150, ((i) / 15)%10);

    // to free ncnb memory used for TCB buffers

    i = 0;
    while (i < sizeof (pTcb)/sizeof (pTcb[0]))
    {
        if (pTcb[i]!=NULL)
            RexDiagDestroyCopyTcb(pTcb[i]);

        pTcb[i] = NULL;

        i++;
    }

    uart_printf ("Free TCBs buffers\r\n");

    if (done)
    {
        uart_printf ("The test is done, please use Mlog to check\r\n");
        DestroyTaskList (pList);

        SvsrLoggerFlush();
        SvsrLoggerSetMask(0);

        RexUploadMLog();
        MxDelayTicks (150*1000);

        MLogPrint();
        MLogSetMask(0);
        //MLogClose();
    }
    else
    {
        uart_printf ("The test is NOT done, please use Mlog to check\r\n");
    }

    RexServShowTrace (16);
    return rc;
}

MXRC RexDiagCheckSuperTCB (void)
{
    // To create the task list

    PTASKLIST pList;
    PTCB pTcb [60];
    MXRC rc = MX_OK, i = 0;

    SYSCORECFG cfg;
    FEC_CONFIG_REGS FecConfigRegs;
    UINT32 nTime;
    REXPROF * pProf;

    memset (&FecConfigRegs, 0, sizeof (FecConfigRegs));
    memset (&cfg, 0, sizeof (cfg));

    FecConfigRegs.wimax_lte = FEC_HWCONF_LTE;
    FecConfigRegs.bp_rate_match = 0;
    FecConfigRegs.bp_channel_interleaver = 0;
    FecConfigRegs.ci_symbol_number = 0; // 12

    cfg.CpuDrivers [0] =    SYSCORE_FEC_DL_DRV      |
                            SYSCORE_FEC_UL_DRV      |
                            SYSCORE_SYS_MDMA_DRV    |
                            SYSCORE_SPU_MDMA_DRV    |
                            SYSCORE_RAD_MDMA_DRV;

    cfg.CpuDrivers [1] =    SYSCORE_CEVA_DRV        |
                            SYSCORE_FP_DRV          |
                            SYSCORE_SCHEDULER;

    cfg.FpMode = FPMODE_FFT1024;

    if (FAILED(rc = SysCoreConfig(&cfg)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    done = 0;

    // **********************************************************************
    //                   LIST-0 (just a list of copy data)
    // **********************************************************************
    
    pList = CreateTaskList (0, 0);
    _ASSERT_PTR(pList);

    if (pList == NULL)
        return REX_DIAG_RC_LIST_CREATE_ERROR;

    RegListDoneCB(pList, RexDiagCheckPriorCB, (LPVOID)&done);

    memset (pTcb, 0, sizeof (pTcb));

    // to create the task list with COPY_TASK, the task priority is <1> 
    // just for example

    pTcb[0] = RexDiagCreateCopyTcb(1024*5);
        _ASSERT_PTR(pTcb[0]);

    if (pTcb[0] == NULL)
    {
        uart_printf ("Create TCB[%d] - error!\r\n", 0);
        MxDelayTicks (150*1000);
    
        uart_printf ("REX: Create TCB error, i = %d\r\n", i);
        MxDelayTicks (150*1000);

        DestroyTaskList(pList);
        
        return REX_DIAG_RC_TCB_CREATE_ERROR;
    }

    AddTCBToList (pTcb[0], pList, REX_DIAG_TCB_REGULAR_PRIOR);

    for (i = 1; i < sizeof (pTcb)/sizeof (pTcb[0]); i++)
    {
        pTcb[i] = RexDiagCreateCopyTcb(1024*5);
        _ASSERT_PTR(pTcb[i]);

        if (pTcb[i] == NULL)
        {
            uart_printf ("Create TCB[%d] - error!\r\n", i);
            MxDelayTicks (150*1000);
        
            i --;
        
            while (i != 0)
            {
                RexDiagDestroyCopyTcb (pTcb[i]);
                i--;
            }
        
            uart_printf ("REX: Create TCB error, i = %d\r\n", i);
            MxDelayTicks (150*1000);
            return REX_DIAG_RC_TCB_CREATE_ERROR;
        }


        AddSuperTCB(0, pTcb[i - 1], pList, 1, pTcb[i]);
    }

    uart_printf ("Start task lists[%d] ... ", pList->ID);
    MxDelayTicks (150*1000);

    nTime = MxGetTicks();

    rc = ExeTSchedList(pList);
    _ASSERT_RC(rc);

    while (done == 0)
    {
        _NOP();
    }

    nTime = MxGetTicks() - nTime;

    uart_printf ("DONE\r\nTotal time is %d\r\n", nTime);

    for (i = 0; i < sizeof (pTcb)/sizeof (pTcb[0]); i++)
    {
        if (!RexDiagCheckCopyTcb (pTcb[i]))
        {
            uart_printf ("TCB [%d] - error\r\n", i);
            MxDelayTicks (150*1000);
        }
        else
        {
            //uart_printf ("TCB [%d] - OK\r\n", i);
            //MxDelayTicks (150*1000);
        }
    }

    {
        UINT32 nID [] = {REX_PROF_CEVA_RUN, 
                         REX_PROF_CEVA_DONE,
                         REX_PROF_KERNEL,
                         REX_PROF_PROC_LIST,
                         REX_PROF_EXEC_LIST,
                         REX_PROF_SCHED_TASKS,
                         REX_PROF_RUN_SCHED_TASKS,
                         REX_PROF_CEVA_RUN_FROM_CEVA_DONE,
                         REX_PROF_FP_RUN_FROM_FP_DONE,
                         REX_PROF_CALL_CB,
                         (UINT32)-1};

        const char * pIDName [] = { "REX_PROF_CEVA_RUN",
                                    "REX_PROF_CEVA_DONE",
                                    "REX_PROF_KERNEL",
                                    "REX_PROF_PROC_LIST",
                                    "REX_PROF_EXEC_LIST",
                                    "REX_PROF_SCHED_TASKS",
                                    "REX_PROF_RUN_SCHED_TASKS",
                                    "REX_PROF_CEVA_RUN_FROM_CEVA_DONE",
                                    "REX_PROF_FP_RUN_FROM_FP_DONE",
                                    "REX_PROF_CALL_CB",
                                    NULL};
        i = 0;

        while (pIDName [i]!=NULL)
        {
            pProf = RexGetProfInfo ((REX_ENUM_PROF_ID)nID[i]);
            _ASSERT_PTR(pProf);

            if (pProf->TotalNum != 0)
                uart_printf (" - %s: ticks:%d, num:%d, avg:%d, max:%d, min:%d\r\n", pIDName[i], pProf->TotalTime, pProf->TotalNum, pProf->TotalTime / pProf->TotalNum, pProf->MaxTime, pProf->MinTime);
            
            MxDelayTicks (150*1000);
            i++;
        }
    }

    i = RexGetProfInfo(REX_PROF_KERNEL)->TotalTime + RexGetProfInfo(REX_PROF_CEVA_DONE)->TotalTime;
    uart_printf ("KERNEL+CBDone: %d ticks (%d.%d us)\r\n", i, i / 150, ((i) / 15)%10);

    uart_printf ("The test is done, please use Mlog to check\r\n");

    DestroyTaskList (pList);

    SvsrLoggerFlush();
    SvsrLoggerSetMask(0);

    MLogPrint();
    MLogSetMask(0);
    MLogClose();

    return rc;
}


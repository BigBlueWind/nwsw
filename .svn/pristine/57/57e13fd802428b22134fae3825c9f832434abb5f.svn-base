//-------------------------------------------------------------------------------------------
/** @file schtaskdrv.c
 *
 * @brief This file is an implementation of the scheduler task driver designed
*       to be used by the scheduler to run DSP tasks on ARM processor(s)
 * @author Mindspeed Technologies
 * @version $Revision: 1.11 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "heaps.h"
#include "schtaskdrv.h"
#include "icdrv.h"
#include "systimers.h"
#include <string.h>
#include "initcpu.h"
//#define STDrv_DEBUG

#ifdef STDrv_DEBUG
#include "appprintf.h"
#endif

STDCTX  stdctx;
VUINT32 stdlock_proc [STD_CPUS] = {0};
VUINT32 stdlock_ready = 0;

BOOL STDSchedTcb (STDCTX * pCtx, UINT32 nDstCpu, TCB * pTcb)
{
    BOOL b;
    IRQTYPE irq = ARM_INT_disable ();
    MxGetLock (&stdlock_proc[nDstCpu]);

    pTcb->NextTcb = NULL;

    if (pCtx->PTaskBegin[nDstCpu] == NULL)
    {
        b = TRUE;

        pCtx->PTaskBegin[nDstCpu] = pTcb;
        pCtx->PTaskEnd[nDstCpu]   = pTcb;
    }
    else
    {
        pCtx->PTaskEnd[nDstCpu]->NextTcb = pTcb;
        pCtx->PTaskEnd[nDstCpu] = pTcb;

        b = FALSE;
    }

    MxReleaseLock (&stdlock_proc[nDstCpu]);
    ARM_INT_restore (irq);

    return b;
}


BOOL STDAddReadyTcb (STDCTX * pCtx, TCB * pTcb)
{
    BOOL b;
    IRQTYPE irq = ARM_INT_disable ();
    MxGetLock (&stdlock_ready);

    pTcb->NextTcb = NULL;

    if (pCtx->RTaskBegin == NULL)
    {
        pCtx->RTaskBegin = pTcb;
        pCtx->RTaskEnd   = pTcb;
        b = TRUE;
    }
    else
    {
        pCtx->RTaskEnd->NextTcb = pTcb;
        pCtx->RTaskEnd = pTcb;
        b = FALSE;
    }

    MxReleaseLock (&stdlock_ready);
    ARM_INT_restore (irq);

    return b;
}


BOOL STDIsScheduled(STDCTX * pCtx)
{
    //return (pCtx->RTaskBegin != NULL);
    return 0;
}

TCB * STDGetTcb (STDCTX * pCtx, BOOL bReadyList)
{
    TCB * p = NULL;
    UINT32 nCpuID = MxGetCpuID ();
    IRQTYPE irq = ARM_INT_disable ();

    if (bReadyList)
    {
        MxGetLock (&stdlock_ready);

        if (pCtx->RTaskBegin != NULL)
        {
            p = pCtx->RTaskBegin;

            pCtx->RTaskBegin = p->NextTcb;

            if (pCtx->RTaskBegin == NULL)
                pCtx->RTaskEnd = NULL;

            p->NextTcb = NULL;
        }

        MxReleaseLock (&stdlock_ready);
    }
    else
    {
        MxGetLock (&stdlock_proc [nCpuID]);

        if (pCtx->PTaskBegin[nCpuID] != NULL)
        {
            p = pCtx->PTaskBegin[nCpuID];

            pCtx->PTaskBegin[nCpuID] = p->NextTcb;

            if (pCtx->PTaskBegin[nCpuID] == NULL)
                pCtx->PTaskEnd[nCpuID] = NULL;

            p->NextTcb = NULL;
        }

        MxReleaseLock (&stdlock_proc [nCpuID]);
    }

    ARM_INT_restore (irq);
    return p;
}

UINT32 STDEventHandler(void * pPtr)
{
    STDCTX * pCtx = (STDCTX *) pPtr;
    UINT32 nCpuID = MxGetCpuID ();

    // for the debug mode only
    _ASSERT_PTR (pCtx);

    // on the 'master' core, the function processes complete request

    if (nCpuID == pCtx->MasterCpuID)
    {

#ifdef STDrv_DEBUG
    uart_printf ("STDIrqHandler: To proc ready list\r\n");
#endif
    	if (pCtx->Notifier != NULL)
    	{
#ifdef STDrv_DEBUG
            uart_printf ("STDIrqHandler: Call notifier:%x, with param:%x\r\n", pCtx->Notifier, pCtx->NotifierPtr);
#endif
    		pCtx->Notifier (pCtx, pCtx->NotifierPtr);

#ifdef STDrv_DEBUG
            uart_printf ("STDIrqHandler: Notifier[%x] - OK\r\n", pCtx->Notifier);
#endif
    	}
    	else
    	{

#ifdef STDrv_DEBUG
            uart_printf ("STDIrqHandler: Force clear ready queue\r\n", pCtx->Notifier, pCtx->NotifierPtr);
#endif
    		// just to clean the readiness queue
	        while (STDGetTcb (pCtx, 1) != NULL);
    	}
    }

    return 0;
}

UINT32 STDThread (LPVOID pPtr)
{
    //UINT32 nCpuID;
    STDCTX * pCtx = (STDCTX *) pPtr;
    PTCB pTcb;
    UINT32 nTicks;

    // for the debug mode only
    _ASSERT_PTR (pCtx);

    //nCpuID = MxGetCpuID ();

#ifdef STDrv_DEBUG
    uart_printf ("STDThread:check the list (cpu=%d)\n", MxGetCpuID ());
#endif

    while ((pTcb = STDGetTcb (pCtx, 0)) != NULL)
    {
        // to prevent to generate the 'empty' I-IRQ

#ifdef STDrv_DEBUG
    	uart_printf ("STDThread:got TCB (tcb=0x%08x, id=%d)\n", pTcb, pTcb->TaskID);
#endif

        _ASSERT_PTR (pTcb);

        _ASSERT (pTcb->TaskID < pCtx->TableSize);

        _ASSERT_PTR (pCtx->LookupTable [pTcb->TaskID].TaskHandler);

        // to call the TCB handler

        nTicks = SysTimerGetTick ();

#ifdef STDrv_DEBUG
        uart_printf ("STDThread: Task handler addr [%08x]\r\n", pCtx->LookupTable [pTcb->TaskID].TaskHandler);
#endif
        pCtx->LookupTable [pTcb->TaskID].TaskHandler (pTcb);

#ifdef STDrv_DEBUG
        uart_printf ("STDThread: Completed Task handler [%08x]\r\n", pCtx->LookupTable [pTcb->TaskID].TaskHandler);
#endif

        // To mark the task as complited,
        // it's needed to follow TCB drivers logic
        pTcb->Status = STATUS_COMPLETE;

        // to put the task pointer to the processed tasks queue
        // with the number of exec ticks
        pTcb->ExecTicks = (SysTimerGetTick () - nTicks);

#ifdef STDrv_DEBUG
        uart_printf ("STDThread: Add to ready list\r\n");
#endif
        // Only master CPU should process the processed tasks
        if (STDAddReadyTcb (pCtx, pTcb))
        {
#ifdef STDrv_DEBUG
            uart_printf ("STDThread: gen irq[%d] to cpu=%d\r\n", pCtx->IrqIDs [pCtx->MasterCpuID], pCtx->MasterCpuID);
#endif
        	//ICDrvRaiseInterCpu (pCtx->MasterCpuID, pCtx->IrqIDs [pCtx->MasterCpuID]);
        	MxInjectThread(pCtx->MasterCpuID, pCtx->MasterEventHandler);
        }
    }

#ifdef STDrv_DEBUG
    uart_printf ("STDThread: out of loop\r\n");
#endif

    return 0;
}

/** @brief This function should be called by the application/scheduler
           to allocate the tasks table in run-time mode

    @param nElmNum   [in] - the number of elements in the task table
    @param bSaveData [in] - if TRUE - the data from the previous table
                            will be copied to the new table

    @return [MXRC] an error code
            \ingroup group_lte_services
            */

MXRC STDrvAllocTable (UINT32 nElmNum, BOOL bSaveData)
{
    STDCTX * pCtx = &stdctx;
    LPVOID pData;

    if (bSaveData == FALSE && pCtx->LookupTable != NULL)
    {
        MxHeapFree (AppHeap, pCtx->LookupTable);

        pCtx->LookupTable = NULL;
        pCtx->TableSize   = 0;
    }

    pData = MxHeapAlloc (AppHeap, sizeof (STDHANDLER) * nElmNum);

    if (pData == NULL)
        return STD_RC_NOT_MEMORY;

    memset (pData, 0, sizeof (STDHANDLER) * nElmNum);

    if (bSaveData != FALSE && pCtx->LookupTable != NULL)
    {
        memcpy (pData,  pCtx->LookupTable, MIN(pCtx->TableSize, nElmNum));
    }

    if (pCtx->LookupTable != NULL)
    {
        MxHeapFree (AppHeap, pCtx->LookupTable);

        pCtx->LookupTable = NULL;
        pCtx->TableSize   = 0;
    }

    pCtx->TableSize = nElmNum;
    pCtx->LookupTable = (STDHANDLER  *)pData;

    return STD_RC_OK;
}

/** @brief This function registers new task in the task table, the name of function
           is compatible with scheduler definition

    @param nTaskID [in] - the task ID, see: tcb.h file
    @param fnptr   [in] - the pointer to the task handler

    @return [MXRC] an error code of operation
            \ingroup group_lte_services
            */

UINT32 STDrvRegARMTask (UINT32 nTaskID,  void (*fnptr)(TCB*))
{
    STDCTX * pCtx = (STDCTX *)&stdctx;

    if (pCtx->LookupTable == NULL)
        return STD_RC_LOOKUP_TABLE_ERR;

    if (nTaskID >= pCtx->TableSize)
        return STD_RC_PARAM_ERROR;

    pCtx->LookupTable [nTaskID].TaskHandler = fnptr;

#ifdef STDrv_DEBUG
    uart_printf ("RegARMTask (task=%d)(cpu=%d)(proc=0x%08x)\n", nTaskID, MxGetCpuID (), fnptr);
#endif

    return STD_RC_OK;
}
/** @brief The function runs task in ARM supervisor mode, this function is
           designed to be called in ARM IRQ mode

	@param nDstCpu [in] - the destination CPU (0 or 1)
    @param pTcb    [in] - the task descriptor

    @return [MXRC] an error code of operation
            \ingroup group_lte_services
            */

MXRC STDrvRunTask (UINT32 nDstCpu, TCB * pTcb)
{
    STDCTX * pCtx = (STDCTX *)&stdctx;
    UINT32 nCpuID = MxGetCpuID ();
    MXRC rc;

    _ASSERT(nDstCpu < STD_CPUS);

    #ifdef STDrv_DEBUG
    uart_printf ("STDRunTask (cpu=%d), cbDone=%08x\n", nCpuID, pTcb->cbDone);
    #endif

    // to rise I-CPU only for the first
    // one element putted to the queue

    if (STDSchedTcb (pCtx, nDstCpu, pTcb))
    {
        #ifdef STDrv_DEBUG
        uart_printf ("STDRunTask: sched thread(on CPU-%d) to locally proc TCB\r\n", nDstCpu);
        #endif
        
        rc = MxScheduleThread (pCtx->hThread [nDstCpu]);
    }
    else
    {
        rc = STD_RC_OK;
    }

    return rc;
}

/** @brief This function registers callback handler, the callback handler will
           be called in ARM IRQ mode

    @param proc [in] - the pointer to the function, it can be NULL

    @return [MXRC] an error code
            \ingroup group_lte_services
            */

MXRC STDrvRegNotifier (STDNotifier proc, LPVOID pParam)
{
    IRQTYPE irq;

    irq = ARM_INT_disable();

    stdctx.Notifier    = proc;
    stdctx.NotifierPtr = pParam;

    STDrvSetMasterCpu (MxGetCpuID ());

    ARM_INT_restore(irq);

#ifdef STDrv_DEBUG
    uart_printf ("Reg notifier (cpu=%d)\n", MxGetCpuID ());
#endif

    return STD_RC_OK;
}

/** @brief This function sets master cpu id, master id is an id
           where completed tasks are processed

    @param nMasterCpu [in] - master cpu id [0 ... N]

    @return [MXRC] an error code
            \ingroup group_lte_services
            */

MXRC STDrvSetMasterCpu (UINT32 nMasterCpu)
{
    STDCTX * pCtx = &stdctx;

    if (nMasterCpu >= SysGetCpuNum ())
        return STD_RC_UNSUPPORTED_CPU;

    if ((pCtx->SupportedCpus & (1 << nMasterCpu)) == 0)
        return STD_RC_UNSUPPORTED_CPU;

    pCtx->MasterCpuID = nMasterCpu;

    return STD_RC_OK;
}

MXRC STDrv_init(UINT32 nInit, LPVOID * pStorageDrvCtx)
{
    MXRC rc = STD_RC_OK;
    UINT32 nCpu;

    STDCTX * pCtx = &stdctx;

    nCpu = MxGetCpuID ();

    if (nCpu >= STD_CPUS)
    	return STD_RC_UNSUPPORTED_CPU;

    if (nCpu == 0)
    {
        memset (&stdctx, 0, sizeof (stdctx));

#ifdef STDrv_DEBUG
        uart_printf ("STDrv_init, cpu=%d, irq=%d\r\n", nCpu, pCtx->IrqIDs [nCpu]);
#endif
        if (FAILED(rc = MxCreateThread (STDEventHandler, NULL, pCtx, 0, &pCtx->MasterEventHandler)))
            return rc;
    }

    //
    // the common context/driver initialization
    //

    if (FAILED(rc = MxCreateThread (STDThread, NULL, pCtx, 0, &pCtx->hThread [nCpu])))
        return rc;

    pCtx->SupportedCpus |= (1 << MxGetCpuID ());

    *pStorageDrvCtx = pCtx;

    return STD_RC_OK;
}



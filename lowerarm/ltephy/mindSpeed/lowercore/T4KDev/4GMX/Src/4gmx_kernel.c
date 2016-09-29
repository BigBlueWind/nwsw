//-------------------------------------------------------------------------------------------
/** @file 4gmx_kernel.c
 *
 * @brief The file contains implementation of 4GMX kernel API
 * @author Mindspeed Technologies
 * @version $Revision: 1.37 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_kernel.h"
#include "4gmx_gvars.h"
#include "4gmx_arm.h"
#include "4gmx_serv.h"
#include "4gmx_debug.h"

#ifdef MX_INTERCPU_ENABLED
#include "4gmx_intercpu.h"
#endif

static MXRC InitThreads(MXCONTEXT * pCtx)
{
    Mximemset(pCtx->threads, 0, GETARRSIZE(pCtx->threads));

    MxiDefSysPart(&pCtx->parts[OBJ_TYPE_THREAD], pCtx->threads,
                  GETARRELMSIZE(pCtx->threads), GETARRCOUNT(pCtx->threads));

    return MX_OK;
}

#ifdef MX_MTHREAD_ENABLED
static MXRC InitMThreads(MXCONTEXT * pCtx)
{
    Mximemset(pCtx->mthreads, 0, GETARRSIZE(pCtx->mthreads));

    MxiDefSysPart(&pCtx->parts[OBJ_TYPE_MTHREAD], pCtx->mthreads,
                  GETARRELMSIZE(pCtx->mthreads), GETARRCOUNT(pCtx->mthreads));

    return MX_OK;
}

static MXRC InitSemas(MXCONTEXT * pCtx)
{
    Mximemset(pCtx->semas, 0, GETARRSIZE(pCtx->semas));

    MxiDefSysPart(&pCtx->parts[OBJ_TYPE_SEMA], pCtx->semas,
                  GETARRELMSIZE(pCtx->semas), GETARRCOUNT(pCtx->semas));

    return MX_OK;
}

#endif /* MX_MTHREAD_ENABLED */

#ifdef MX_EVENT_ENABLED
static MXRC InitEvents(MXCONTEXT * pCtx)
{
    Mximemset(pCtx->events, 0, GETARRSIZE(pCtx->events));

    MxiDefSysPart(&pCtx->parts[OBJ_TYPE_EVENT], pCtx->events,
                  GETARRELMSIZE(pCtx->events), GETARRCOUNT(pCtx->events));

    return MX_OK;
}
#endif /*MX_EVENT_ENABLED */

#ifdef MX_LIST_ENABLED
static MXRC InitLists(MXCONTEXT * pCtx)
{
    Mximemset(pCtx->lists, 0, GETARRSIZE(pCtx->lists));

    MxiDefSysPart(&pCtx->parts[OBJ_TYPE_LIST], pCtx->lists,
                  GETARRELMSIZE(pCtx->lists), GETARRCOUNT(pCtx->lists));

    return MX_OK;
}
#endif /*MX_LIST_ENABLED */

#ifdef MX_TIMER_ENABLED
static MXRC InitTimers(MXCONTEXT * pCtx)
{
    pCtx->FirstTimer = NULL;
    pCtx->LastTimer = NULL;

    Mximemset(pCtx->timers, 0, GETARRSIZE(pCtx->timers));

    MxiDefSysPart(&pCtx->parts[OBJ_TYPE_TIMER], pCtx->timers,
                  GETARRELMSIZE(pCtx->timers), GETARRCOUNT(pCtx->timers));

    return MX_OK;
}
#endif /*MX_TIMER_ENABLED */

#ifdef MX_HEAP_ENABLED
static MXRC InitHeaps(MXCONTEXT * pCtx)
{
    Mximemset(pCtx->heaps, 0, GETARRSIZE(pCtx->heaps));

    MxiDefSysPart(&pCtx->parts[OBJ_TYPE_HEAP], pCtx->heaps,
                  GETARRELMSIZE(pCtx->heaps), GETARRCOUNT(pCtx->heaps));

    return MX_OK;
}
#endif /*MX_HEAP_ENABLED */

#ifdef MX_RHEAP_ENABLED
static MXRC InitRHeaps(MXCONTEXT * pCtx)
{
    Mximemset(pCtx->rheaps, 0, GETARRSIZE(pCtx->rheaps));

    MxiDefSysPart(&pCtx->parts[OBJ_TYPE_RHEAP], pCtx->rheaps,
                  GETARRELMSIZE(pCtx->rheaps), GETARRCOUNT(pCtx->rheaps));

    return MX_OK;
}
#endif /*MX_RHEAP_ENABLED */


#ifdef MX_QUEUE_ENABLED
static MXRC InitQueues(MXCONTEXT * pCtx)
{
    MXRC rc = MX_OK;

    Mximemset(pCtx->queues, 0, GETARRSIZE(pCtx->queues));

    MxiDefSysPart(&pCtx->parts[OBJ_TYPE_QUEUE], pCtx->queues,
                  GETARRELMSIZE(pCtx->queues), GETARRCOUNT(pCtx->queues));

    return rc;
}

static MXRC InitDefHeap(PMXCONTEXT pCtx)
{
    if (pCtx->SysHeap != NULL)
    {
        return MxCreateHeap(pCtx->SysHeap, pCtx->SysHeapSize, sizeof(UINT32),
                            NULL, &pCtx->hDefHeap);
    }

    return MX_OK;
}

#endif /*MX_QUEUE_ENABLED */

#ifdef MX_DRV_FRAMEWORK_ENABLED

static MXRC DrvFrameworkInit(PMXCONTEXT pCtx)
{
    MXRC rc = MX_OK;
    UINT32 i = 0;

    _ASSERT(pCtx != NULL);

    if (pCtx->Drivers == NULL)
        return MX_OK;

    /* To detect the number of drivers */

    pCtx->DrvCount = 0;

    while (pCtx->Drivers[pCtx->DrvCount].Init != NULL)
    {
        pCtx->DrvCount++;

        _ASSERT(pCtx->DrvCount < MX_MAX_DRIVERS);
    }

    /* The interrupts controller should be initialized first */

    if (pCtx->ICDrvId != 0xFFFFFFFF)
    {
        rc = pCtx->Drivers[pCtx->ICDrvId].Init(pCtx->Drivers[pCtx->ICDrvId].
                                               InitParam,
                                               &(pCtx->DrvCtx[pCtx->ICDrvId]));

        if (FAILED(rc))
            return rc;

#ifdef MX_NAMING_ENABLED

        if (pCtx->Drivers[pCtx->ICDrvId].DrvName != NULL)
        {
            rc = MxSetObjName(MxGetDrv(pCtx->ICDrvId),
                              pCtx->Drivers[pCtx->ICDrvId].DrvName);

            if (FAILED(rc))
                return rc;
        }
#endif
    }

    while (pCtx->Drivers[i].Init != NULL)
    {
        if (i == pCtx->ICDrvId)
        {
            i++;
            continue;
        }

        rc = pCtx->Drivers[i].Init(pCtx->Drivers[i].InitParam,
                                   &(pCtx->DrvCtx[i]));

        if (FAILED(rc))
            return rc;

#ifdef MX_NAMING_ENABLED

        if (pCtx->Drivers[i].DrvName != NULL)
        {
            rc = MxSetObjName(MxGetDrv(i), pCtx->Drivers[i].DrvName);

            if (FAILED(rc))
                return rc;
        }
#endif
        i++;
    }

    return MX_OK;
}

#endif /*MX_DRV_FRAMEWORK_ENABLED */

#ifdef MX_PART_ENABLED
static MXRC InitPartitions(MXCONTEXT * pCtx)
{
    Mximemset(pCtx->partitions, 0, GETARRSIZE(pCtx->partitions));

    MxiDefSysPart(&pCtx->parts[OBJ_TYPE_PARTITION], pCtx->partitions,
                  GETARRELMSIZE(pCtx->partitions),
                  GETARRCOUNT(pCtx->partitions));

    return MX_OK;
}
#endif /*MX_PART_ENABLED */

#ifdef MX_TRACER_ENABLED
static MXRC InitTracers(MXCONTEXT * pCtx)
{
    Mximemset(pCtx->tracers, 0, GETARRSIZE(pCtx->tracers));

    MxiDefSysPart(&pCtx->parts[OBJ_TYPE_TRACER], pCtx->tracers,
                  GETARRELMSIZE(pCtx->tracers),
                  GETARRCOUNT(pCtx->tracers));

    return MX_OK;
}
#endif

/** @brief The function initializes 4GMX system and should be called before MX API usage

    @param pInit[in] - the configuration parameters of system
    @param pCtx [out] - the context is used like a system storage where 4GMX contains flags, objects, and so on ..

    @return [MXRC] an initialization error code

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxInit(PMXINIT pInit, MXCONTEXT * pCtx)
{
    UINT32 cpuid;
    MXRC rc;

    cpuid = MxGetCpuID();

    _ASSERT(pInit != NULL);
    _ASSERT(cpuid < MX_MAX_INTERCPU);
    _ASSERT(pCtx != NULL);

    Mximemset(pCtx, 0, sizeof(MXCONTEXT));

    MXCtx[cpuid] = pCtx;

    pCtx->SysCfg = pInit->ConfigSet;

#ifdef MX_HWSEMA_ENABLED
    pCtx->HwSemaBase = pInit->HwSemaBase;
#endif

#ifdef MX_ARM_ENABLED
    pCtx->lvecaddr = pInit->lvecaddr;

    pCtx->mmu_l1_table_ptr = pInit->mmu_l1_table_ptr;
    pCtx->mmu_l2_table_ptr = pInit->mmu_l2_table_ptr;
    pCtx->mmu_l2_table_size= pInit->mmu_l2_table_size;

    if (FAILED(rc = MxARMSubSystemInit(pCtx, pInit->ConfigSet)))
        return rc;
#endif

#ifdef MX_DRV_FRAMEWORK_ENABLED
    pCtx->Drivers = pInit->Drivers;
    pCtx->ICDrvId = pInit->ICDrvId;
    pCtx->TmrDrvId = pInit->TmrDrvId;
#endif

#ifdef MX_HEAP_ENABLED
    pCtx->SysHeap = pInit->SysHeap;
    pCtx->SysHeapSize = pInit->SysHeapSize;
#endif

#ifdef _MX_DEBUG
    if (pInit->TraceBufSize == 0 || pInit->TraceRingBuf == NULL)
    {
        pCtx->TraceBufBegin = NULL;
        pCtx->TraceBufEnd   = NULL;
        pCtx->TraceGet      = NULL;
        pCtx->TracePut      = NULL;
        pCtx->TraceBlkCount = 0;
    }
    else
    {
        pCtx->TraceBufBegin = (PUINT8)pInit->TraceRingBuf;
        pCtx->TraceBufEnd   = (PUINT8)pInit->TraceRingBuf + pInit->TraceBufSize;
        pCtx->TraceGet      = (PUINT8)pInit->TraceRingBuf;
        pCtx->TracePut      = (PUINT8)pInit->TraceRingBuf;
        pCtx->TraceBlkCount = 0;
    }
#endif

#ifdef MX_EXT_THREAD_TRACER_ENABLED
    pCtx->ExtThrTracer = pInit->ExtThrTracer;
#endif

#ifdef MX_HWSEMA_ENABLED
    pCtx->HwSemaBase = pInit->HwSemaBase;
#endif

    MXCtx[cpuid] = pCtx;

    if (FAILED(rc = InitThreads(pCtx)))
        return rc;

#ifdef MX_MTHREAD_ENABLED
    if (FAILED(rc = InitMThreads(pCtx)))
        return rc;

    if (FAILED(rc = InitSemas(pCtx)))
        return rc;
#endif

#ifdef MX_EVENT_ENABLED
    if (FAILED(rc = InitEvents(pCtx)))
        return rc;
#endif

#ifdef MX_LIST_ENABLED
    if (FAILED(rc = InitLists(pCtx)))
        return rc;
#endif

#ifdef MX_TIMER_ENABLED
    if (FAILED(rc = InitTimers(pCtx)))
        return rc;
#endif

#ifdef MX_HEAP_ENABLED
    if (FAILED(rc = InitHeaps(pCtx)))
        return rc;
    if (FAILED(rc = InitDefHeap(pCtx)))
        return rc;
#endif

#ifdef MX_RHEAP_ENABLED
    if (FAILED(rc = InitRHeaps(pCtx)))
        return rc;
#endif

#ifdef MX_PART_ENABLED
    if (FAILED(rc = InitPartitions(pCtx)))
        return rc;
#endif

#ifdef MX_QUEUE_ENABLED
    if (FAILED(rc = InitQueues(pCtx)))
        return rc;
#endif

#ifdef MX_TRACER_ENABLED
    if (FAILED(rc = InitTracers(pCtx)))
        return rc;
#endif

#ifdef MX_DRV_FRAMEWORK_ENABLED
    if (FAILED(rc = DrvFrameworkInit(pCtx)))
        return rc;
#endif /*MX_DRV_FRAMEWORK_ENABLED */

#ifdef MX_MTHREAD_ENABLED
    if (FAILED(rc = MxMKernelInit()))
        return rc;
#endif

#ifdef MX_ARM_ENABLED
    if (FAILED(rc = MxARMConfigIntc(pCtx, pInit->ConfigSet)))
        return rc;
#endif

#ifdef MX_INTERCPU_ENABLED
    if (FAILED(rc = MxiInterCpuInit(pCtx, pInit->ICIrq, pInit->ICTable, pInit->ICCtx, pInit->ICSMPCtx)))
        return rc;
#endif

    return MX_OK;
}

/** @brief The function starts the 4GMX scheduler

    @param idle [in] - the pointer to the idle function  (it can be null)
    @param param[in] - the parameter of idle function

    @return [MXRC] error code

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxStart(TENTRY idle, LPVOID pParam)
{
    MXCONTEXT *pCtx;
    IRQTYPE irq;
    PTHREAD pThread;
    UINT32 nThisCpuID = MxGetCpuID ();

#ifdef MX_INTERCPU_ENABLED
    MXSMPCTX * pSmpCtx;
#endif

#if defined(TRACE_THR_START) || defined (TRACE_THR_FINISH)
    TraceDataThread trace;
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED
    MxICSetInit(TRUE);
#endif

    pCtx->StopScheduler = FALSE;

#ifdef MX_INTERCPU_ENABLED
    pSmpCtx = pCtx->ICSMPCtx;
#endif

    while (!pCtx->StopScheduler)
    {
        if (pCtx->FST != NULL)
        {
#ifdef MX_EXT_THREAD_TRACER_ENABLED
            if (pCtx->ExtThrTracer != NULL)
            {
                MxiDisInt(&irq);
                pCtx->ExtThrTracer (EXT_TRACER_START_THREADS);
                MxiRestInt(irq);
            }
#endif

            while (pCtx->FST != NULL && !pCtx->StopScheduler)
            {
                MxiDisInt(&irq);

#ifdef TRACE_THR_START

                HANDLE_INIT(trace.hThr, MxGetCpuID(), OBJ_TYPE_THREAD,
                              ((UINT32)pCtx->FST - (UINT32)pCtx->threads)/
                              sizeof (THREAD));

                trace.Rc = MX_OK;
                MxWriteDebugTrace(pCtx, TEID_START_THR, &trace, sizeof (trace));
#endif

                pCtx->CET = (pThread = (THREAD *) pCtx->FST);

                MxiCutThreadFromSchList (pCtx, pThread);

                MxiRestInt(irq);

#ifdef MX_EXT_THREAD_TRACER_ENABLED
                if (pCtx->ExtThrTracer != NULL)
                    pCtx->ExtThrTracer (EXT_TRACER_START_THREAD);
#endif
                pThread->ReturnVal = (pThread->Proc)(pThread->ProcParam);

#ifdef MX_EXT_THREAD_TRACER_ENABLED
                if (pCtx->ExtThrTracer != NULL)
                    pCtx->ExtThrTracer (EXT_TRACER_STOP_THREAD);
#endif

#ifdef TRACE_THR_FINISH
                HANDLE_INIT(trace.hThr, MxGetCpuID(), OBJ_TYPE_THREAD,
                              ((UINT32)pCtx->CET - (UINT32)pCtx->threads)/
                              sizeof (THREAD));

                trace.Rc = MX_OK;
                MxWriteDebugTrace(pCtx, TEID_FINISH_THR, &trace, sizeof (trace));
#endif

                pCtx->CET = NULL;

            }

        }
        else
        {
            #ifdef MX_INTERCPU_ENABLED

            if (pSmpCtx != NULL && !(pCtx->SysCfg & SYS_INIT_NO_PROC_SMP_THREAD))
            {
                if (pSmpCtx->SchedNum != 0)
                {
                    MxiDisInt(&irq);
                    MxGetLock(&pSmpCtx->Lock);

                    pCtx->CESMPT = pThread = pSmpCtx->FST;

                    if (pThread != NULL)
                    {
                        pSmpCtx->FST = (PTHREAD)pSmpCtx->FST->NextScheduled;

                        #ifdef MX_EXT_THREAD_TRACER_ENABLED
                        if (pCtx->ExtThrTracer != NULL)
                        {
                            pCtx->ExtThrTracer (EXT_TRACER_START_THREADS);
                        }
                        #endif

                        if (pSmpCtx->FST == NULL)
                            pSmpCtx->LST = NULL;

                        // to set executor CPU id where this thread is started
                        pThread->nSMPExecCpuID = nThisCpuID;

                        MARK_UNDER_SMP_PROC(pThread);
                        MARK_UNSCHEDULED(pThread);

                        pSmpCtx->SchedNum--;
                    }

                    MxReleaseLock(&pSmpCtx->Lock);
                    MxiRestInt(irq);

                    if (pThread != NULL)
                    {
                        pThread->ReturnVal = pThread->Proc(pThread->ProcParam);

                        pCtx->CESMPT = NULL;

                        // if thread was scheduled again like SMP thread
                        // inside this thread
                        if (IS_TO_BE_SMP_SCHED(pThread))
                        {
                            MxSchedSmpThreadEx(pThread, pThread->nSMPPrior, pThread->nSMPCtrlFlags | SCHED_R_SMP_PROC);
                        }
                        else
                        {
                            MxiDisInt(&irq);
                            MxGetLock(&pSmpCtx->Lock);

                            CLEAR_UNDER_SMP_PROC(pThread);

                            MxReleaseLock(&pSmpCtx->Lock);
                            MxiRestInt(irq);
                        }

                        continue;
                    }

                }
            }

            #endif

            /**********************************************************************/
            /* This code processes situation when SS m-thread has nothing to do   */
            /* In this case we need to switch SS m-thread or to call idle handle  */
            /* To notify the application about idle mode                          */
            /*********************************************************************/

            #ifdef MX_MTHREAD_ENABLED
                MxSwitchSSThread (idle, pParam);
            #else
                if (idle != NULL)
                {
                    #ifdef MX_EXT_THREAD_TRACER_ENABLED
                        if (pCtx->ExtThrTracer != NULL)
                        {
                            MxiDisInt(&irq);
                            pCtx->ExtThrTracer (EXT_TRACER_START_IDLE);
                            MxiRestInt(irq);
                        }
                    #endif

                    idle(pParam);
                }
           #endif // MX_MTHREAD_ENABLED
        }
    }

    return MX_OK;
}

/** @brief The function should be called for stopping the 4GMX scheduler,
   it can be called from ISR/thread contexts of execution.

   @return [MXRC] error code

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxStop(void)
{
    MXCONTEXT *pCtx;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    pCtx->StopScheduler = TRUE;

    return MX_OK;
}

/** @brief The function returns a number of ticks spent from the system startup

    @return [UINT32] the ticks number

    \ingroup group_lte_4gmx_kernel
*/
UINT32 MxGetTime(void)
{
    PMXCONTEXT pCtx;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    return pCtx->Ticks;
}

/** @brief The function returns a number of ticks spent from the system startup

    @return [UINT32] the ticks number

    \ingroup group_lte_4gmx_kernel
*/
UINT32 MxGetFracTime(void)
{
    PMXCONTEXT pCtx;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    return pCtx->FracTicks;
}


/** @brief This function returns number of ticks, 150 ticks is equal to 1us
           it depends on the system initialization, and usage of this function is
           related to usage of device HW, it's strongly recomented to check device before usage of this function

    @return [UINT32] ticks in unit,  150 unit = 1us

    \ingroup group_lte_4gmx_kernel
    */

UINT32 MxGetTicks(void)
{
#ifdef MX_ONBOARD_TIMER_ENABLED
    return MXREG32(MX_ONBOARD_TIMER_REG);
#else
#error "MxGetTicks is not implemented"
    return (UINT32)-1;
#endif
}

/** @brief The function sets/changes the number of ticks spent from the system startup

    @param nTime [in] - new value of ticks

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxSetTime(UINT32 nTime)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pCtx->Ticks = 0;

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function waits specified number of ms

    @NOTE: The timer driver should be activated

    @param nTime [in] - the number of ms

    @return [MXRC] error code of operation
    \ingroup group_lte_4gmx_kernel
*/
MXRC MxDelay (UINT32 nTimeout)
{
    PMXCONTEXT pCtx;
    UINT32 nTime;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    nTime = pCtx->Ticks;

    while (pCtx->Ticks - nTime < nTimeout)
    {

    }

    return MX_OK;
}

/** @brief This function uses on-board timer to wait some number of ticks

    @param nTickCount [in] - the ticks number to wait

    @return [MXRC] an error code
    \ingroup group_lte_4gmx_kernel
    */

MXRC MxDelayTicks (UINT32 nTickCount)
{
#ifdef MX_ONBOARD_TIMER_ENABLED

    VUINT32 * pReg = (VUINT32 *)MX_ONBOARD_TIMER_REG;
    UINT32 nTick = *pReg;

    while (*pReg - nTick < nTickCount)
    {
        _NOP();
    }

    return MX_OK;
#else
    return MX_KERNEL_UNSUPPORTED_OBJ;
#endif
}

/** @brief The function returns currently executed thread function or NULL

    @return [UINT32] the function of current thread or NULL

    \ingroup group_lte_4gmx_kernel
*/
UINT32 MxGetCurrentThreadProc(void)
{
    PMXCONTEXT pCtx;//, pRemCtx;

    pCtx = MxInqContext();

    if (pCtx->CET != NULL)
    {
       return  (UINT32)pCtx->CET->Proc;
    }
    else if (pCtx->CESMPT != NULL)
    {
        //pRemCtx = MxInqRemContext(pCtx->CESMPT->nOwnerCpuID);
        //_ASSERT_PTR (pRemCtx);
       return  (UINT32)pCtx->CESMPT->Proc;
    }

    return 0;
}


/** @brief The function returns currently executed thread handle or HNULL

    @return [HANDLE] the handle of current thread or HNULL

    \ingroup group_lte_4gmx_kernel
*/
HANDLE MxGetCurrentThread(void)
{
    PMXCONTEXT pCtx, pRemCtx;
    HANDLE h = HNULL;

    pCtx = MxInqContext();

    if (pCtx->CET != NULL)
    {
        HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_THREAD,
                ((UINT32) pCtx->CET - (UINT32) pCtx->threads) / sizeof(THREAD));
    }
    else if (pCtx->CESMPT != NULL)
    {
        pRemCtx = MxInqRemContext(pCtx->CESMPT->nOwnerCpuID);
        _ASSERT_PTR (pRemCtx);
        HANDLE_INIT(h, pCtx->CESMPT->nOwnerCpuID, OBJ_TYPE_THREAD,
                ((UINT32) pCtx->CESMPT - (UINT32) pRemCtx->threads) / sizeof(THREAD));
    }

    return h;
}

/** @brief The function returns a total number of created objects of "type" type

    @param type [in] - the type of objects

    @return [UINT32] number of created objects

    \ingroup group_lte_4gmx_kernel
*/
UINT32 MxGetObjCount(OBJTYPE type)
{
    PMXCONTEXT pCtx;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_DRV_FRAMEWORK_ENABLED
    if (type == OBJ_TYPE_DRIVER)
        return pCtx->DrvCount;
#endif

    _ASSERT(type < OBJ_TYPE_NUM);

    return pCtx->parts[type].AllocCnt;
}

/** @brief The function returns a total number of created objects of "type" type

    @param nCpuID[in] - the CPU index
    @param type  [in] - the type of objects

    @return [UINT32] number of created objects

    \ingroup group_lte_4gmx_kernel
*/
UINT32 MxGetObjCountEx(UINT32 nCpuID, OBJTYPE type)
{
    PMXCONTEXT pCtx;

    pCtx = MxInqRemContext(nCpuID);

    _ASSERT(pCtx != NULL);

    if (pCtx == NULL)
        return 0;

#ifdef MX_DRV_FRAMEWORK_ENABLED
    if (type == OBJ_TYPE_DRIVER)
        return pCtx->DrvCount;
#endif

    _ASSERT(type < OBJ_TYPE_NUM);

    return pCtx->parts[type].AllocCnt;
}

/** @brief The function returns object's handle, specified by the object
           type and object index.

    @param type   [in] - the type of objects
    @param nIndex [in] - the object index

    @return [UINT32] the object handle or HNULL

    \ingroup group_lte_4gmx_kernel
*/
HANDLE MxGetObjByIndex(OBJTYPE type, UINT32 nIndex)
{
    IRQTYPE irq;
    HANDLE h = HNULL;

#ifdef _MX_DEBUG

    PMXCONTEXT pCtx;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#endif

#ifdef MX_DRV_FRAMEWORK_ENABLED
    if (type == OBJ_TYPE_DRIVER)
    {
        HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_DRIVER, nIndex);
        return h;
    }
#endif

    MxiDisInt(&irq);

    _ASSERT(type < OBJ_TYPE_NUM);
    _ASSERT(pCtx->parts[type].AllocCnt > nIndex);

    HANDLE_INIT(h, MxGetCpuID(), type, nIndex);

    MxiRestInt(irq);

    return h;
}

/** @brief The function returns object's handle, specified by the object
           type and object index.

    @param nCpuID [in] - the CPU id of remoute system
    @param type   [in] - the type of objects
    @param nIndex [in] - the object index

    @return [UINT32] the object handle or HNULL*/

HANDLE MxGetObjByIndexEx(UINT32 nCpuID, OBJTYPE type, UINT32 nIndex)
{
    IRQTYPE irq;
    HANDLE h = HNULL;

#ifdef _MX_DEBUG

    PMXCONTEXT pCtx;

    pCtx = MxInqRemContext(nCpuID);

    _ASSERT(pCtx != NULL);

    if (pCtx == NULL)
        return HNULL;
#endif

#ifdef MX_DRV_FRAMEWORK_ENABLED
    if (type == OBJ_TYPE_DRIVER)
    {
        HANDLE_INIT(h, nCpuID, OBJ_TYPE_DRIVER, nIndex);
        return h;
    }
#endif

    MxiDisInt(&irq);

    _ASSERT(type < OBJ_TYPE_NUM);
    _ASSERT(pCtx->parts[type].AllocCnt > nIndex);

    HANDLE_INIT(h, nCpuID, type, nIndex);

    MxiRestInt(irq);

    return h;

}

#ifdef _MX_DEBUG
/** @brief The function gets exclusive access to the object

    @param pObj [in] - the address of the object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxGetLock (VUINT32 * pObj)
{
#ifdef MX_ARM_ENABLED
    extern void MxiGetLock (VUINT32 * p);
     _ASSERT_PTR(pObj);
    MxiGetLock (pObj);
#else
#error "The MxLockObj should be implemented for the CEVA processor"
#endif
    return MX_OK;
}

/** @brief The function gets exclusive access and disables IRQ/FIQ

    @param pObj [in] - the address of the object

    @return [IRQTYPE] the IRQ/FIQ status

    \ingroup group_lte_4gmx_kernel
*/
IRQTYPE MxGetLockEx (VUINT32 * pObj)
{
#ifdef MX_ARM_ENABLED
    extern IRQTYPE MxiGetLockEx (VUINT32 * p);

    _ASSERT_PTR(pObj);
    return MxiGetLockEx (pObj);

#else
#error "The MxLockObj should be implemented for the CEVA processor"
#endif
}

#endif

#if defined(MX_INTERCPU_ENABLED)

/** @brief This functions shares data object (partition/queue) for
           inter-cpu functionality

    @param h     [in] - The object handler (it should be partition or queue)
    @param nMode [in] - enable/disable sharing

    @return [MXRC]

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxShareObject(HANDLE h, SHAREMODE nMode)
{
    MXRC rc = MX_OK;
    PMXCONTEXT pCtx;
    ShareProc share;
    UnshareProc unshare;
    MXICShrObj * pSharedSpace;
    LPVOID pObjDescr;
    IRQTYPE irq;

    pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    MxiDisInt(&irq);

    switch (HANDLE_GET_TYPE(h))
    {
#ifdef MX_PART_ENABLED
        case OBJ_TYPE_PARTITION:
            {
                share    = MxSharePart;
                unshare  = MxUnsharePart;
                pObjDescr= MxiGetPartDescr(pCtx, h);

                break;
            }
#endif

#ifdef MX_QUEUE_ENABLED
        case OBJ_TYPE_QUEUE:
            {
                share    = MxShareQueue;
                unshare  = MxUnshareQueue;
                pObjDescr= MxiGetQueueDescr(pCtx, h);

                break;
            }
#endif

        default:
            {
                MxiRestInt(irq);

                rc = MX_KERNEL_UNSUPPORTED_OBJ;

                _ASSERT_RC(rc);

                return rc;
            }

    }

    _ASSERT_PTR(share);
    _ASSERT_PTR(unshare);
    _ASSERT_PTR(pObjDescr);

    if (nMode == OBJ_SHARE_ON)
    {
        if (MxiFindSharedBlock(pCtx, h) != NULL)
        {
            MxiRestInt(irq);

            return MX_OK;
        }

        pSharedSpace = MxiAllocSharedSpace (pCtx);

        if (pSharedSpace == NULL)
        {
            MxiRestInt(irq);

            _ASSERT_PTR(pSharedSpace);

            return MX_KERNEL_NOT_SHARED_SPACE;
        }

        pSharedSpace->hHand     = h;
        pSharedSpace->pObjDescr = pObjDescr;
        pSharedSpace->Lock      = 0;

        rc = share (h, &pSharedSpace->Lock);

        if (FAILED(rc))
        {
            MxiRestInt(irq);

            _ASSERT_RC(rc);

            MxiFreeSharedSpace(pCtx, pSharedSpace);

            return rc;
        }
    }
    else
    {
        pSharedSpace = MxiFindSharedBlock (pCtx, h);

        if (pSharedSpace == NULL)
        {
            MxiRestInt(irq);

            _ASSERT_PTR(pSharedSpace);

            return MX_KERNEL_NOT_SHARED_SPACE;
        }

        rc = unshare (h);

        if (FAILED(rc))
        {
            MxiRestInt(irq);

            _ASSERT_RC(rc);

            return rc;
        }

        rc = MxiFreeSharedSpace(pCtx, pSharedSpace);

        _ASSERT_RC(rc);
    }

    MxiRestInt(irq);

    return rc;
}

/** @brief  The function schedules inter-cpu handler, it can be called
            from thread/ISR execution context

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxScheduleInterCpu(void)
{
    PMXCONTEXT pCtx;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    return MxScheduleThread(pCtx->hInterCpuThr);
}

/** @brief This function changes default priority of inter-cpu thread

    @param pr [in] - the new thread priority

    @return [pr] error code

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxSetInterCpuPriority(TPRIORITY pr)
{
    PMXCONTEXT pCtx;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    if (HANDLE_NOT_NULL(pCtx->hInterCpuThr) == FALSE)
        return MX_KERNEL_IC_NOT_INIT;

    return MxSetThreadPriority(pCtx->hInterCpuThr, pr);
}

#endif /*#ifdef MX_INTERCPU_ENABLED */


/** @brief The function returns the version of 4GMX system

    @param pHVer [out] - the hight version number
    @param pLVer [out] - the low version number

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxGetVersion(UINT8 * pHVer, UINT8 * pLVer)
{
    _ASSERT(pHVer != NULL);
    _ASSERT(pLVer != NULL);

    *pHVer = MX_HIGH_VERSION;
    *pLVer = MX_LOW_VERSION;

    return MX_OK;
}

/***************************************************************/
/*     Naming APIs implementation                              */
/***************************************************************/

#ifdef MX_NAMING_ENABLED

/** @brief The function returns an object handle, specified by the name

    @param cpu_id [in] - the CPU id where object will be searched
    @param name   [in] - the ASSCIZ name of object
    @param outHand[out]- the object handle

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxGetObjByName(CPUID cpu_id, const char *name, HANDLE * outHand)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    SINT32 nPos;

#ifdef MX_INTERCPU_ENABLED
    MxICKrnGetObj ic;
#endif

    _ASSERT(name != NULL);
    _ASSERT(outHand != NULL);

#ifdef MX_INTERCPU_ENABLED

    if (cpu_id != MxGetCpuID() && (cpu_id != CUR_CPU))
    {
        ic.Handle = outHand;
        ic.Name   = name;

        return MxICExec(cpu_id, MXIC_KERNEL_GETOBJ, &ic, sizeof (ic));
    }

#else
    _ASSERT((cpu_id == CUR_CPU) || cpu_id == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    nPos = MxiFindName(pCtx, name, NULL);

    if (nPos == -1)
    {
        *outHand = HNULL;

        MxiRestInt(irq);

        return MX_KERNEL_OBJ_NOT_FOUND;
    }

    *outHand = pCtx->NameTable[nPos].hObj;

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function associates an object with the ASCIIZ name

    @Note: Only locally created objects are supported

    @param h   [in] - the object handle
    @param name[in] - the ASSCIZ name

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxSetObjName(HANDLE h, LPCSTR name)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;

#ifdef MX_INTERCPU_ENABLED
     MxICKrnSetObjName ic;
#endif

    _ASSERT_HANDLE(h);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        ic.Name   = name;

        return MxICExec(HANDLE_GET_CPU(h), MXIC_KERNEL_SETOBJ_NAME, &ic, sizeof(ic));
    }

#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (name != NULL)
    {
        if (MxiIsObjNamed(pCtx, h))
        {
            MxiFreeName(pCtx, h);
        }

        if (!MxiAllocName(pCtx, h, name))
        {
            MxiRestInt(irq);

            MxiMarkObjUnnamed(pCtx, h);

            return MX_KERNEL_ALLOC_NAME_ERROR;
        }

        MxiMarkObjNamed(pCtx, h);
    }
    else if (MxiIsObjNamed(pCtx, h))
    {
        MxiFreeName(pCtx, h);
        MxiMarkObjUnnamed(pCtx, h);
    }

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function returns a pointer to the object's name or NULL

    @Note:  It can be used for locally created objects only.

    @param h   [in] - the object handle

    @return [const char*] the pointer to the object name or NULL

    \ingroup group_lte_4gmx_kernel
*/
LPCSTR MxGetObjName(HANDLE h)
{
    PMXCONTEXT pCtx;
    UINT32 i;

#ifdef MX_INTERCPU_ENABLED
    MxICKrnGetObjName ic;
    MXRC rc;
    LPCSTR pName = NULL;
#endif

    _ASSERT_HANDLE(h);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        ic.Name   = &pName;

        rc = MxICExec(HANDLE_GET_CPU(h), MXIC_KERNEL_GETOBJ_NAME, &ic, sizeof(ic));

        if (FAILED(rc))
        {
            _ASSERT_RC (rc);

            return NULL;
        }

        return pName;
    }

#else
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    if (MxiIsObjNamed(pCtx, h) == FALSE)
        return NULL;

    /*  The table is sorted by name, so, the search should be done
       in simple enumeration method */

    for (i = 0; i < pCtx->NameCount; i++)
    {
        if (*(UINT32 *) & pCtx->NameTable[i].hObj == *(UINT32 *) & h)
            return pCtx->NameTable[i].name;
    }

    return NULL;
}

#endif /*MX_NAMING_ENABLED */

/************************************************************************/
/*                                                                      */
/*        Timers internal API                                           */
/*                                                                      */
/************************************************************************/

/** @brief This function processes 1ms timers
    @param pCtx [in] - OS context
    @return [MXRC] an error code */

MXRC MxProcTimers(PMXCONTEXT pCtx)
{
#ifdef MX_TIMER_ENABLED
    PTIMER pTimer, pNext;
#endif

    MXRC rc = MX_OK;

    _ASSERT(pCtx != NULL);

    pCtx->Ticks++;

#ifdef MX_TIMER_ENABLED

    pTimer = pCtx->FirstTimer;

    while (pTimer != NULL)
    {
        pNext = pTimer->Next;

        rc = MxRunTimer(pCtx, pTimer);

        pTimer = pNext;
    }

#endif /*MX_TIMER_ENABLED */

    return rc;
}

/** @brief This function is responsible to process fraction timers
           fraction means timers with less than 1ms resolution (1ms/n),
           where n is the number of fractions

    @param pCtx [in] - the OS context

    @return [MXRC] an error code
    */

MXRC MxProcFractionTimers(PMXCONTEXT pCtx)
{
#ifdef MX_TIMER_ENABLED
    PTIMER pTimer, pNext;
#endif

    MXRC rc = MX_OK;

    _ASSERT(pCtx != NULL);

    pCtx->FracTicks++;

#ifdef MX_TIMER_ENABLED

    pTimer = pCtx->FirstFracTimer;

    while (pTimer != NULL)
    {
        pNext = pTimer->Next;

        rc = MxRunTimer(pCtx, pTimer);

        pTimer = pNext;
    }

#endif /*MX_TIMER_ENABLED */

#ifdef MX_MTHREAD_ENABLED
    MxMProcSleepList();
#endif

    return rc;
}

/** @brief This function sets a value of the variable in exclusive way (with usage of synhronization)

    @param pVar [in] - the pointer to the variable
    @param nVal [in] - the value of the variable

    @return [UINT32] the set value
    \ingroup group_lte_4gmx_kernel

    */

UINT32  MxExcSet (UINT32 * pVar, UINT32 nVal)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    MXSMPCTX* ICSMPCtx;

    _ASSERT_PTR (pVar);

    pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);

    ICSMPCtx = pCtx->ICSMPCtx;
    _ASSERT_PTR (ICSMPCtx);

    irq = ARM_INT_disable ();
    MxGetLock (&ICSMPCtx->ExcAPILock);

    *pVar = nVal;

    MxReleaseLock (&ICSMPCtx->ExcAPILock);
    ARM_INT_restore(irq);

    return nVal;
}

/** @brief This function increments the variable in exclusive way (with usage of synhronization)

    @param pVar [in] - the pointer to the variable

    @return [UINT32] the new value of variable

    \ingroup group_lte_4gmx_kernel
    */

UINT32  MxExcInc (VUINT32 * pVar)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    MXSMPCTX* ICSMPCtx;
    UINT32 nVal;

    _ASSERT_PTR (pVar);

    pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);

    ICSMPCtx = pCtx->ICSMPCtx;
    _ASSERT_PTR (ICSMPCtx);

    irq = ARM_INT_disable ();
    MxGetLock (&ICSMPCtx->ExcAPILock);

    nVal = ++(*pVar);

    MxReleaseLock (&ICSMPCtx->ExcAPILock);
    ARM_INT_restore(irq);

    return nVal;
}

/** @brief This function decrements the variable in exclusive way (with usage of synhronization)

    @param pVar [in] - the pointer to the variable

    @return [UINT32] the new value of variable
    \ingroup group_lte_4gmx_kernel

    */

UINT32  MxExcDec (VUINT32 * pVar)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    MXSMPCTX* ICSMPCtx;
    UINT32 nVal;

    _ASSERT_PTR (pVar);

    pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);

    ICSMPCtx = pCtx->ICSMPCtx;
    _ASSERT_PTR (ICSMPCtx);

    irq = ARM_INT_disable ();
    MxGetLock (&ICSMPCtx->ExcAPILock);

    nVal = --(*pVar);

    MxReleaseLock (&ICSMPCtx->ExcAPILock);
    ARM_INT_restore(irq);

    return nVal;
}

#if MX_SAFE_SECTION_NUM > 0

    /** @brief This function starts 'safe' section of code, IRQs are disabled
               the lock is established

        @param nSectID [in] - the section ID, ID < MX_SAFE_SECTION_NUM
        @return [MXRC] an error code

        \ingroup group_lte_4gmx_kernel

        */
    MXRC MxOpenSafeSection (UINT32 nSectID)
    {
        PMXCONTEXT pCtx;
        MXSMPCTX* ICSMPCtx;
        IRQTYPE irq;

        pCtx = MxInqContext();

        _ASSERT_PTR(pCtx);
        _ASSERT (nSectID < MX_SAFE_SECTION_NUM);

        if (nSectID >= MX_SAFE_SECTION_NUM)
            return MX_KERNEL_SECT_ID_ERROR;

        irq = ARM_INT_disable();

        _ASSERT (pCtx->IRQStatPut  < (MX_INT_STATUS_ARRAY + MX_INT_STATUS_ARRAY + 1));
        _ASSERT (pCtx->SafeSectPut < (MX_SAFE_SECTION_NUM + 1));

        // to push IRQ status
        pCtx->IRQStatStack [pCtx->IRQStatPut] = irq;
        pCtx->IRQStatPut ++;

        // to push section ID
        pCtx->SafeSectStack [pCtx->SafeSectPut] = nSectID;
        pCtx->SafeSectPut ++;

        ICSMPCtx = pCtx->ICSMPCtx;

        if (ICSMPCtx != NULL)
        {
            MxGetLock ((VUINT32*) &(ICSMPCtx->SafeSectLocks [nSectID]));
        }

        return MX_OK;
    }

    /** @brief This function closes 'safe' section of code, IRQs are restored
               the lock is released

        @return [MXRC] an error code

        \ingroup group_lte_4gmx_kernel

        */
    MXRC MxCloseSafeSection (void)
    {
        PMXCONTEXT pCtx;
        MXSMPCTX* ICSMPCtx;
        IRQTYPE irq;
        UINT32 nSectID;

        pCtx = MxInqContext();

        _ASSERT_PTR(pCtx);

        if (pCtx->IRQStatPut == 0 || pCtx->SafeSectPut == 0)
        {
            _ASSERT(!(pCtx->IRQStatPut == 0 || pCtx->SafeSectPut == 0));
            return MX_KERNEL_SECT_UNSYNC;
        }

        pCtx->IRQStatPut --;
        irq = pCtx->IRQStatStack [pCtx->IRQStatPut];

        pCtx->SafeSectPut--;
        nSectID = pCtx->SafeSectStack [pCtx->SafeSectPut];

        ICSMPCtx = pCtx->ICSMPCtx;

        if (ICSMPCtx != NULL)
        {
            _ASSERT (nSectID < MX_SAFE_SECTION_NUM);
            MxReleaseLock ((VUINT32*) &(ICSMPCtx->SafeSectLocks [nSectID]));
        }

        ARM_INT_restore (irq);
        return MX_OK;
    }

    /** @brief This function disables IRQ signals and pushes status to the stack

        @NOTE: Please use MxIntRestore() to restore the system

        @return [MXRC] an error code
        \ingroup group_lte_4gmx_kernel
        */

    MXRC MxIntDisable (void)
    {
        PMXCONTEXT pCtx = MxInqContext();

        _ASSERT_PTR(pCtx);
        _ASSERT ((pCtx->IRQStatPut + 1) < (MX_INT_STATUS_ARRAY + MX_INT_STATUS_ARRAY + 1));
        pCtx->IRQStatStack [pCtx->IRQStatPut ++ ] = ARM_INT_disable();

        return MX_OK;
    }

    /** @brief This function disables IRQ signals and pushes status to the stack

        @NOTE: Please use MxIntRestore() to restore the system

        @return [MXRC] an error code
        \ingroup group_lte_4gmx_kernel
    */

    MXRC MxIntRestore (void)
    {
        PMXCONTEXT pCtx = MxInqContext();

        _ASSERT_PTR (pCtx);
        _ASSERT (pCtx->IRQStatPut > 0);

        if (pCtx->IRQStatPut > 0)
            ARM_INT_restore(pCtx->IRQStatStack [--pCtx->IRQStatPut]);

        return MX_OK;
    }

#endif

/************************************************************************/
/*                                                                      */
/*        Drivers API implementation                                    */
/*                                                                      */
/************************************************************************/

#ifdef MX_DRV_FRAMEWORK_ENABLED

/** @brief The function returns driver handle based on the index of driver in drivers table.

    @param nDrvIndex [in] - the drivers index

    @return [HANDLE] driver handle (it is used in driver API)

    \ingroup group_lte_4gmx_kernel
*/
HANDLE MxGetDrv(UINT32 nDrvIndex)
{
    HANDLE h = HNULL;

#ifdef _MX_DEBUG

    PMXCONTEXT pCtx;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    _ASSERT(nDrvIndex < pCtx->DrvCount);

#endif

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_DRIVER, nDrvIndex);

    return h;
}

/** @brief The function configures driver's parameter.

    @param hDrv   [in] - the driver handle (see MxGetDrv function)
    @param nCfgParam [in] - the id of driver parameter
    @param pParamData [in] - the pointer to data
    @param nDataSize [in] - the data size (in bytes)

    @return [MXRC] error code of configuration

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxConfigDrv(HANDLE hDrv, UINT32 nCfgParam, LPVOID pParamData,
                 UINT32 nDataSize)
{
    PMXCONTEXT pCtx;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    _ASSERT(HANDLE_NOT_NULL(hDrv));
    _ASSERT(HANDLE_GET_INDEX(hDrv) < pCtx->DrvCount);

    _ASSERT(pCtx->Drivers[HANDLE_GET_INDEX(hDrv)].Config != NULL);

    return pCtx->Drivers[HANDLE_GET_INDEX(hDrv)].Config(
            pCtx->DrvCtx[HANDLE_GET_INDEX(hDrv)],
            nCfgParam, pParamData,
            nDataSize);
}

/** @brief The function writes data to driver

    @param hDrv   [in] - the driver handle (see MxGetDrv function)
    @param nCmdID  [in] - the id of command
    @param pCmdData  [in] - the pointer to data
    @param nDataSize [in] - the data size (in bytes)

    @return [MXRC] error code of writing

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxWriteDrvData(HANDLE hDrv, UINT32 nCmdID, LPVOID pCmdData,
                    UINT32 nDataSize)
{
    PMXCONTEXT pCtx;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);
    _ASSERT(HANDLE_NOT_NULL(hDrv));
    _ASSERT(HANDLE_GET_INDEX(hDrv) < pCtx->DrvCount);
    _ASSERT(pCtx->Drivers[HANDLE_GET_INDEX(hDrv)].Write != NULL);

    return pCtx->Drivers[HANDLE_GET_INDEX(hDrv)].Write(
            pCtx->DrvCtx[HANDLE_GET_INDEX(hDrv)], nCmdID,
            pCmdData, nDataSize);
}

/** @brief The function reads driver data

    @param hDrv   [in] - the driver handle (see MxGetDrv function)
    @param nCmdID  [in] - the id of command
    @param pCmdData  [out]- the pointer to data
    @param nDataSize [in/out] - in: the data size of buffer (in bytes)
           out:the size of placed data (in bytes)

    @return [MXRC] error code of reading

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxReadDrvData(HANDLE hDrv, UINT32 nCmdID, LPVOID pCmdData,
                   UINT32 * pnDataSize)
{
    PMXCONTEXT pCtx;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);
    _ASSERT(HANDLE_NOT_NULL(hDrv));
    _ASSERT(HANDLE_GET_INDEX(hDrv) < pCtx->DrvCount);
    _ASSERT(pCtx->Drivers[HANDLE_GET_INDEX(hDrv)].Read != NULL);

    return pCtx->Drivers[HANDLE_GET_INDEX(hDrv)].Read(
            pCtx->DrvCtx[HANDLE_GET_INDEX(hDrv)], nCmdID,
            pCmdData, pnDataSize);
}

/** @brief The function returns pointer to driver context.

    @param hDrv [in] - the driver handle

    @return [LPVOID] a pointer to driver context, it can be NULL

    \ingroup group_lte_4gmx_kernel
*/
LPCVOID MxGetDrvCtx(HANDLE hDrv)
{
    PMXCONTEXT pCtx;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);
    _ASSERT(HANDLE_NOT_NULL(hDrv));
    _ASSERT(HANDLE_GET_INDEX(hDrv) < pCtx->DrvCount);

    return pCtx->DrvCtx[HANDLE_GET_INDEX(hDrv)];
}

#endif /*MX_DRV_FRAMEWORK_ENABLED */

/** @brief The function raises inter-cpu interrupt by using interrupt controller driver

    @param nCpuList [in] - the List of CPUs (bit mask)
    @param nIntId   [in] - the ID of interrupt :  0 - 15

    @return [MXRC] error code

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxRaiseInterCpu(INTC_CPU_IDS nCpuList, UINT16 nIntId)
{
#ifndef MX_DRV_FRAMEWORK_ENABLED
    return MX_KERNEL_DRV_DONT_USED;
#else

    HANDLE hICDrv;
    PMXCONTEXT pCtx;
    INTC_RAISE_SOFTINT data;

#ifdef MX_DIRECT_ICPU_IRQ_GENERATING_ENABLED

    if (nIntId >= 32)
    {
        MXREG32(MX_ICPU_GEN_REG) = 1 << (nIntId - 32);

        return MX_OK;
    }

#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    if (pCtx->ICDrvId == 0xFFFFFFFF)
        return MX_KERNEL_NO_ICDRV;

    hICDrv = MxGetDrv(pCtx->ICDrvId);

    _ASSERT(HANDLE_NOT_NULL(hICDrv));

    data.CpuList = (UINT16) nCpuList;
    data.IntId = nIntId;

    return MxWriteDrvData(hICDrv, INTC_RAISE_SOFT_INT, &data, sizeof(data));

#endif /*MX_DRV_FRAMEWORK_ENABLED */
}


/** @brief The function cleares inter-cpu interrupt by using interrupt controller driver

     @param nCpuList [in] - the List of CPUs (bit mask)
     @param nIntId   [in] - the ID of interrupt :  0 - 15

     @return [MXRC] error code

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxClearInterCpu(INTC_CPU_IDS nCpuList, UINT16 nIntId)
{
#ifndef MX_DRV_FRAMEWORK_ENABLED
    return MX_KERNEL_DRV_DONT_USED;
#else

    HANDLE hICDrv;
    PMXCONTEXT pCtx;
    INTC_RAISE_SOFTINT data;

#ifdef MX_DIRECT_ICPU_IRQ_GENERATING_ENABLED

    if (nIntId >= 32)
    {
        MXREG32(MX_ICPU_CLR_REG) = 1 << (nIntId - 32);
        return MX_OK;
    }

#endif

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    if (pCtx->ICDrvId == 0xFFFFFFFF)
        return MX_KERNEL_NO_ICDRV;

    hICDrv = MxGetDrv(pCtx->ICDrvId);

    _ASSERT(HANDLE_NOT_NULL(hICDrv));

    data.CpuList = (UINT16) nCpuList;
    data.IntId = nIntId;

    return MxWriteDrvData(hICDrv, INTC_CLEAR_SOFT_INT, &data, sizeof(data));

#endif /*MX_DRV_FRAMEWORK_ENABLED */
}


/** @brief This function is designed to read 32bits data
           in safety context (data exception is handled)

    @param pAddr [in]   -   an address
    @param pData [out]  -   the read data (if return code is SUCCESS)

    @return [MXRC] an error code of operation

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxSafeRead32 (PUINT32 pAddr, UINT32 * pData)
{
#ifndef MX_DRV_FRAMEWORK_ENABLED
    return MX_KERNEL_DRV_DONT_USED;
#else

    HANDLE hICDrv;
    PMXCONTEXT pCtx;
    INTC_SAFE_RW data;
    MXRC rc = MX_OK;
    UINT32 nDataSize;

    _ASSERT_PTR(pData);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    if (pCtx->ICDrvId == 0xFFFFFFFF)
        return MX_KERNEL_NO_ICDRV;

    hICDrv = MxGetDrv(pCtx->ICDrvId);

    _ASSERT(HANDLE_NOT_NULL(hICDrv));

    data.Address = (UINT32)pAddr;
    data.Value   = 0;

    nDataSize = sizeof (data);

    rc = MxReadDrvData(hICDrv, INTC_SAFE_READ_32BITS, &data, &nDataSize);

    if (FAILED(rc))
    {
        return rc;
    }

    *pData = data.Value;

    return rc;

#endif /*MX_DRV_FRAMEWORK_ENABLED */

}

/** @brief This function is designed to write 32bits data
           in safety context (data exception is handled)

    @param pAddr [in]  -   an address
    @param nData [in]  -   write data

    @return [MXRC] an error code of operation

    \ingroup group_lte_4gmx_kernel
*/
MXRC MxSafeWrite32 (PUINT32 pAddr, UINT32 nData)
{
#ifndef MX_DRV_FRAMEWORK_ENABLED
    return MX_KERNEL_DRV_DONT_USED;
#else

    HANDLE hICDrv;
    PMXCONTEXT pCtx;
    INTC_SAFE_RW data;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    if (pCtx->ICDrvId == 0xFFFFFFFF)
        return MX_KERNEL_NO_ICDRV;

    hICDrv = MxGetDrv(pCtx->ICDrvId);

    _ASSERT(HANDLE_NOT_NULL(hICDrv));

    data.Address = (UINT32)pAddr;
    data.Value   = nData;

    return MxWriteDrvData(hICDrv, INTC_SAFE_WRITE_32BITS, &data, sizeof(data));

#endif /*MX_DRV_FRAMEWORK_ENABLED */

}

/**@brief This function is designed to load CPU context
          the data is specific for the processor,
          ARM processor is described with <ARMCPUCONTEXT> type

    @param nCpuID   [in] - the CPU id
    @param pCpuCtx  [out]- the pointer to the structure to where context will be loaded

    @return [MXRC] an error code of operation*/

MXRC MxLoadCpuContext (UINT32 nCpuID, void * pCpuCtx)
{
    MXRC rc;
    MxICKrnGetCpuCtx cmd;

    if (pCpuCtx == NULL)
        return MX_KERNEL_PARAM_ERROR;

    cmd.pCpuCtx = pCpuCtx;

    rc = MxICExec (nCpuID, MXIC_KERNEL_GETCPU_INFO, &cmd, sizeof (cmd));

    return rc;
}


//-------------------------------------------------------------------------------------------
/** @file 4gmx_mthread.c
 *
 * @brief The file contains types and multi-stack threads API declaration
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_mthread.h"
#include "4gmx_serv.h"
#include "4gmx_kernel.h"
#include "4gmx_const.h"
#include "4gmx_heap.h"

#ifdef MX_MTHREAD_ENABLED

#ifdef MX_ARM_ENABLED

/** @brief This function initializes ARM HW context 

    @param pThread [in] - the pointer to M-Thread context

    */

void MxiMThreadHWInit (MTHREAD * pThread)
{
    pThread->hw.r[MTHREAD_HW_PARAM_REG] = (UINT32)pThread->ProcPtr;
    pThread->hw.r[MTHREAD_HW_STACK_REG] = (UINT32)pThread->StackPtr + pThread->StackSize;
    pThread->hw.r[MTHREAD_HW_LR_REG]    = (UINT32)pThread->Proc;
    pThread->hw.r[MTHREAD_HW_PROC_REG]  = (UINT32)pThread->Proc;
    pThread->hw.r[MTHREAD_HW_CSPR]      = MTHREAD_HW_CPSR_DEF;
}

#endif // MX_ARM_ENABLED

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
                         HANDLE * pOutHandle)
{
    MTHREAD * pThr;
    IRQTYPE r;
    PMXCONTEXT pCtx = MxInqContext ();
    HANDLE h;
    UINT32 nAllocated = 0;
    DEF_RADDR;

    _ASSERT_PTR (pCtx);
    _ASSERT_PTR (proc);
    _ASSERT_PTR (pOutHandle);

    if (nStackSize == 0)
        nStackSize = MX_MTHREAD_STACK_SIZE;

    if (pStack == NULL)
    {
        pStack = MxHeapAlloc (HNULL, nStackSize);
        nAllocated = 1;
    }

    if (pStack == NULL)
        return MX_MTHREAD_ALLOC_STACK_ERROR;

    MxiDisInt (&r);

    pThr = (PMTHREAD) MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_MTHREAD]);

    if (pThr == NULL)
    {
        MxiRestInt(r);
        return MX_ALLOC_MTHREAD_ERROR;
    }

    Mximemset (pThr, 0, sizeof (*pThr));

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_MTHREAD,
                ((((UINT32) pThr) -
                  ((UINT32) pCtx->mthreads)) / sizeof(MTHREAD)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_MTHREAD], pThr);

            MxiRestInt(r);
            return MX_ASSIGN_MTHREAD_NAME_ERROR;
        }

        MARK_MTHR_NAMED(pThr);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_MTHR_UNNAMED(pThr);
    }

    MARK_MTHR_ALLOC(pThr);

    if (priority > MX_MAX_MTHREAD_PRIOR)
        priority = MX_MAX_MTHREAD_PRIOR;

    priority = MX_MAX_MTHREAD_PRIOR - priority;

    pThr->StackPtr      = pStack;
    pThr->StackSize     = nStackSize;
    pThr->Proc          = proc;
    pThr->ProcPtr       = param;
    pThr->ThreadPrior   = (1 << priority);
    pThr->ThreadSubPrior= 0;
    pThr->Quantum       = MX_MTHREAD_QUANTUM;
    pThr->ID            = pCtx->MThreadID++;
    pThr->CpuID         = MxGetCpuID ();

    if (nAllocated)
    {
        MARK_MTHR_STACK_ALLOC(pThr);
    }
    else
    {
        MARK_MTHR_NOT_STACK_ALLOC(pThr);
    }

    MxiMThreadHWInit (pThr);

    *pOutHandle = h;

    MxiRestInt(r);

    if ((flags & MX_MTHREAD_FLAGS_SUSPENDED) != 0)
        return MX_OK;

    return MxScheduleMThread (h);
}

#ifdef MX_INTERCPU_ENABLED
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
                         HANDLE * pOutHandle)
{
    MxICMThrCreate ic;

    if (cpu == MxGetCpuID() || cpu == CUR_CPU)
        return MxMCreateThread(proc, NULL, param, priority, flags, pStack, nStackSize, pOutHandle);

    ic.Proc         = proc;
    ic.Param        = param;
    ic.Prior        = priority;
    ic.Name         = name;
    ic.flag         = flags;
    ic.pStack       = pStack;
    ic.nStackSize   = nStackSize;
    ic.Handle       = pOutHandle;

    return MxICExec(cpu, MXIC_THREAD_CREATE_M, &ic, sizeof (ic));
}

#endif // #ifdef MX_INTERCPU_ENABLED
/** @brief This function is responsible to remove the thread from the pool of started threads 
           and to free the context of this thread 

    @param h [in] - the thread handler 

    @return [MXRC] an error code */

MXRC MxMRemoveThread (HANDLE h)
{
    PMXCONTEXT pCtx;
    PMTHREAD pThr;
    IRQTYPE irq;

#ifdef MX_INTERCPU_ENABLED
    MxICThrRemove ic;
#endif

    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        ic.Handle = h;
        return MxICExec (HANDLE_GET_CPU(h), MXIC_THREAD_REMOVE,
                          &ic, sizeof (ic));
    }

#else
    /* Only locally created thread can be removed */
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
#endif

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    pThr = MxiGetMThreadDescr(pCtx, h);

    _ASSERT(pThr != NULL);
    _ASSERT(IS_MTHR_ALLOC(pThr));

    MxiDisInt(&irq);

    if (pCtx->MCET == pThr)
    {
        MxiRestInt(irq);
        return MX_MTHREAD_SELF_REMOVING;
    }

    if (pThr == MxMGetSSThread())
    {
        MxiRestInt(irq);
        return MX_MTHREAD_SSTHREAD_REMOVING;
    }

#ifdef MX_NAMING_ENABLED

    if (IS_MTHR_NAMED(pThr))
    {
        MxiFreeName(pCtx, h);
        MARK_MTHR_UNNAMED(pThr);
    }

#endif /*MX_NAMING_ENABLED */

    if (MARK_MTHR_IS_STACK_ALLOC(pThr))
    {
        MxHeapFree (HNULL, pThr->StackPtr);
        MARK_MTHR_NOT_STACK_ALLOC(pThr);
    }

    MxiRemoveThreadSwStat (pThr);

    // if this thread is in the list of m-threads under execution
    // we need to remove this thread
    if (IS_M_SCHEDULED(pThr))
    {
        MxiMCutThreadFromSchList(pCtx, pThr);
    }

    MARK_MTHR_DEALLOC(pThr);
    MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_MTHREAD], pThr);

    // if removed thread is currently executed, we need 
    // to switch context 

    MxSwitchContext ();
    
    MxiRestInt(irq);
    return MX_OK;
}

#endif //MX_MTHREAD_ENABLED


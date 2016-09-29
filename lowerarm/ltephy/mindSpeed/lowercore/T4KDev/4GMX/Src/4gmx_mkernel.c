//-------------------------------------------------------------------------------------------
/** @file 4gmx_mkernel.c
 *
 * @brief The file contains types and multi-stack threads API implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_mthread.h"
#include "4gmx_kernel.h"
#include "4gmx_serv.h"

int uart_printf(const char *fmt, ...);

#ifdef MX_MTHREAD_ENABLED

/*****************************************************************************/
/*                         Internal functions                                */
/*****************************************************************************/

void MxiPushToSleepList (PMXCONTEXT pCtx, PMTHREAD pThr, UINT32 cur_ticks)
{
    UINT32 ttw; // time to wait 
    PMTHREAD tmp, prev = NULL;

   _ASSERT_PTR(pCtx);
   _ASSERT_PTR(pThr);

   tmp = pCtx->SleepList;

   while (tmp != NULL)
   {
        ttw = (tmp->StartSleepTime + tmp->SleepTime) - cur_ticks;

        if (pThr->SleepTime < ttw)
            break;

        prev = tmp;
        tmp = tmp->list[MTHREAD_LIST_SLEEP].NextScheduled;
   }

   if (tmp == NULL)
   {
        if (prev == NULL)
        {
            // just to add the element to the list HEAD
        
            pCtx->SleepList = pThr;
            
            pThr->list[MTHREAD_LIST_SLEEP].NextScheduled = NULL;
            pThr->list[MTHREAD_LIST_SLEEP].PrevScheduled = NULL;
        }
        else
        {
            // actually to add new element to the end of list
        
            pThr->list[MTHREAD_LIST_SLEEP].NextScheduled = NULL;
            prev->list[MTHREAD_LIST_SLEEP].NextScheduled = pThr;
            pThr->list[MTHREAD_LIST_SLEEP].PrevScheduled = prev;
        }
   }
   else
   {
        // just to insert the element somewhere in the middle of the list
        pThr->list[MTHREAD_LIST_SLEEP].NextScheduled = tmp;
        tmp->list[MTHREAD_LIST_SLEEP].PrevScheduled = pThr;

        if (prev != NULL)
        {
            prev->list[MTHREAD_LIST_SLEEP].NextScheduled = pThr;
        }

        pThr->list[MTHREAD_LIST_SLEEP].PrevScheduled = prev;
        
        if (pCtx->SleepList == tmp)
        {
            pCtx->SleepList = pThr;
        }
   }

    MART_MTHR_WAITING(pThr);
}

PMTHREAD MxiPopFromSleepList (PMXCONTEXT pCtx)
{
    PMTHREAD pThr;

    _ASSERT_PTR(pCtx);

    pThr = pCtx->SleepList;

    if (pThr == NULL)
       return NULL;

    pCtx->SleepList = pThr->list[MTHREAD_LIST_SLEEP].NextScheduled;

    if (pCtx->SleepList != NULL)
    {
       pCtx->SleepList->list[MTHREAD_LIST_SLEEP].PrevScheduled = NULL;
    }

    pThr->list[MTHREAD_LIST_SLEEP].NextScheduled = NULL;
    pThr->list[MTHREAD_LIST_SLEEP].PrevScheduled = NULL;

    MART_MTHR_NOT_WAITING(pThr);

    return pThr;
}

void INLINE MxiIncContextSwitching(PMXCONTEXT pCtx)
{
    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCtx->MCET);

    pCtx->ThreadSwNum ++;

    if (pCtx->MCET != NULL)
        pCtx->MCET->SwNum ++;
}

void MxiRemoveThreadSwStat(PMTHREAD pThr)
{
    PMXCONTEXT pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pThr);

    pCtx->ThreadSwNum -= MIN(pThr->SwNum, pCtx->ThreadSwNum);
}

/** @brief This function returns pointer to the m-thread designed to handle S-threads

    @param pCtx [in] - the OS context

    @return [PMTHREAD] the pointer to the m-thread context*/

PMTHREAD INLINE MxiMGetSSThread (PMXCONTEXT pCtx)
{
    _ASSERT_PTR(pCtx);
    return MxiGetMThreadDescr(pCtx, pCtx->hSSThread);
}

/*****************************************************************************/
/*                         Interfac functions                                */
/*****************************************************************************/


/** @brief This function initializes M-KERNEL 
           and automatically called by the OS kernel, the 
           main purpose of this thread is to create SS thread 

    @return [MXRC] an error code */

MXRC MxMKernelInit (void)
{
    PMXCONTEXT pCtx = MxInqContext();
    UINT32 nLr = MX_GET_RADDR;
    MXRC rc;
    IRQTYPE r;
    PMTHREAD pThr;
    
    _ASSERT_PTR(pCtx);

    MxiDisInt(&r);
    
    rc = MxMCreateThread((MTENTRY)nLr, 
                         NULL, 
                         pCtx, 
                         MX_SS_MTHREAD_PRIOR, 
                         MX_MTHREAD_FLAGS_SUSPENDED,
                         &rc,                           // it's OK, later it will be overwritten
                         sizeof (rc),                   // it's OK, later it will be overwritten
                         &pCtx->hSSThread);

    if (FAILED(rc))
    {
        MxiRestInt(r);
        return rc;
    }

    pThr = MxiGetMThreadDescr (pCtx, pCtx->hSSThread);
    _ASSERT_PTR(pThr);

    MxiMAddThreadToSchList (pCtx, pThr, NULL);

    pThr->StartThreadTime   = MxGetTicks(); // The begin time of SS-Thread start-up
    pCtx->MCET              = pThr;         // The SS-Thread handler 
    pCtx->ThreadSwNum       = 1;            // The base initialization, the system started from SS-Thread       

    pThr->SwNum = 1;

    MxiRestInt(r);

    return rc;
}


/** @brief This function sets a pointer to the application handler 
           that will be called to process interrupts 

    @return [MXRC] an error code*/

MXRC MxSetAppIrqHandler (void (*AppIrqHandler)(void))
{
    PMXCONTEXT pCtx = MxInqContext();
    _ASSERT_PTR(pCtx);

    pCtx->AppIrqHandler = AppIrqHandler;

    return MX_OK;
}

#ifdef MX_ARM_ENABLED
/** @brief This function is designed to switch thread context 
           and start executing another thread

    @param p1 [in] - the pointer to the thread that will be stopped 
    @param p2 [in] - the pointer to the thread that will be activated

    */

ASMPROC void MxiSwitch (MTHREAD_HW_CTX * p1, MTHREAD_HW_CTX * p2)
{
    // **********************************************
    //      to save the context of thread-p1
    //         [r0-r12][sp][lr][pc][cpsr]
    // **********************************************

    STMIA r0!, {r0-r12}

    mov r2, sp
    mov r3, lr
    mov r4, lr
    mrs r5, cpsr
   
    STMIA r0, {r2, r3, r4, r5}  

    // **********************************************
    //      to restore the context of thread-p2
    //         [r0-r12][sp][lr][pc][cpsr]
    // **********************************************

    LDR   r6, [r1]              // to save R0
    ADD   r0, r1, #(13*4)
    LDMIA r0, {r2, r3, r4, r5}  // sp, lr, pc, cpsr
    STR   r6, [r1, #(14*4)]     // to put R0 instead of read LR

    MOV   sp, r2
    MOV   lr, r3
    MSR   CPSR_cxsf, r5

    ADD   r0, r1, #4            // just skip R0
    LDMIA r0!, {r1-r12}         // and load main m-thread stack-frame, [sp][lr][cpsr] are already loaded
    ADD   r0, r0, #4            // to skip SP value 
    LDMIA r0, {r0, pc}          // to load R0 and PC(and immediatelly jump to the caller)
    
}


/** @brief 4GMX ARM IRQ handler, it has to be called to process threads switching 
           from IRQ mode */

ASMPROC void MxIrqHandler (void)
{
    PRESERVE8

    IMPORT   MxProcAppIrqHandler
    IMPORT   MxProcThreadSwitching
    IMPORT   MxHandleMThreadQuantums
    IMPORT   MxSetIrqTime

    /**********************************************/
    /*       To save active m-thread              */
    /**********************************************/
    PUSH    {r0-r4}                 // To save terminated R0-R4
    MOV     r4, lr
    
    MOV     r0, #0
    BL      MxProcThreadSwitching
    
    // R0 points to the m-thread stack frame 
    // (address of PMTHREAD::HW in R0)

    ADD     r3, r0, #(5*4)          // to skip the [r0-r4] (5 registers of 4 bytes each) range 
    STMIA   r3!, {r5-r12}

	MRS	    r1, CPSR                // save current CPSR to r1 for mode switching
	MRS	    r2, SPSR                // spsr_irq = CPSR of irq mode: save to r2 for mode switching
	MRS	    r7, SPSR                // R7 - CPSR of the terminated m-thread
    SUB     r6, r4, #4              // Address of the terminated thread instruction

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // to switch to the terminated mode
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ORR	    r2, r2, #(0x40:OR:0x80)    // no FIQs/no IRQ
	MSR	    CPSR_c, r2

    MOV     r4, sp                  // Thread stack pointer 
    MOV     r5, lr                  // Terminated LR

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // to switch back to the IRQ mode
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    MSR	    CPSR_c, r1

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // To save the rest 4 registers to M-thread context
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    STMIA   r3!, {r4,r5,r6,r7}  // SP, LR, PC, CPSR
    MOV     r5,  r0
        
    POP     {r0-r4}             // to restore [r0-r4] stored at the beginning 
    STMIA   r5, {r0-r4}         // to save these registers to the m-thread stack frame

    /***********************************************/
    /*  [r0-r12][sp][lr][pc][cpsr] are saved       */
    /***********************************************/


    /**********************************************/
    /* The m-thread context is saved and we       */
    /* can work on IRQ processing                 */
    /**********************************************/

    BL       MxSetIrqTime
    
    BL       MxProcAppIrqHandler

#ifndef MX_MTHREAD_MANUAL_QUANTUM_CONTROL
    BL       MxHandleMThreadQuantums
#endif // MX_MTHREAD_MANUAL_QUANTUM_CONTROL

    /**********************************************/
    /*       To return to active m-thread        **/
    /**********************************************/
    MOV     r0, #1
    BL      MxProcThreadSwitching

    MRS	    r1, CPSR                // save current CPSR to r1 for mode switching

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // to switch to the terminated supervisor mode
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    MRS	    r2, SPSR                // spsr_irq = CPSR of terminated supervisor mode
	ORR	    r2, r2, #(0x40:OR:0x80)    // no FIQs/no IRQ
	MSR	    CPSR_c, r2

    LDR     sp, [r0, #(4*13)]       // restore supervisor SP
    LDR     lr, [r0, #(4*14)]       // restore supervisor LR

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // to switch back to the IRQ mode 
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    MSR	    CPSR_c, r1              // to go back to IRQ mode 
    NOP
    LDR     lr, [r0, #(4*15)]       // To prepare supervisor PC register 
    LDR     r4, [r0, #(4*16)]       // To prepare supervisor CPSR register 
    MSR     SPSR_cxsf, r4

    LDMIA   r0, {r0-r12}
    MOVS    pc, lr
}


#endif // MX_ARM_ENABLED

void MxProcAppIrqHandler(void)
{
    PMXCONTEXT pCtx = MxInqContext();
    _ASSERT_PTR (pCtx);

    if (pCtx->AppIrqHandler)
        pCtx->AppIrqHandler();
}

void MxSetIrqTime (void)
{
    MXCONTEXT * p = MxInqContext();
    _ASSERT_PTR(p);

    p->IrqStartTime = MxGetTicks();
}

/** @brief This function is designed to process m-threads of sleep list
           and to put these threads to exec list if sleep time is over

*/           
void MxMProcSleepList (void)
{
    PMXCONTEXT pCtx = MxInqContext();
    UINT32 nTicks, num = 0;
    PMTHREAD pThr;

    _ASSERT_PTR (pCtx);

    nTicks = MxGetTicks();

    while ((pThr = pCtx->SleepList) != NULL)
    {
        if ((nTicks - pThr->StartSleepTime) >= pThr->SleepTime)
        {   
            pThr = MxiPopFromSleepList(pCtx);
            MxiMAddThreadToSchList (pCtx, pThr, MxiMFindSchPos(pCtx, pThr));

            num ++;

            continue;
        }

        break;
    }

    if (num != 0)
    {
        MxSwitchContext ();
    }
}

MTHREAD_HW_CTX* MxProcThreadSwitching (UINT32 nChangeSub)
{
    PMXCONTEXT pCtx = MxInqContext();
    PMTHREAD pThr;
    
    _ASSERT_PTR (pCtx);
    _ASSERT_PTR (pCtx->MCET);

    pThr = (PMTHREAD)pCtx->MCET;

#ifdef MX_PREVENT_INT_THREAD_SWITCHING
    return &pThr->hw;
#else
    // in this case the thread is going to be scheduled 

    if (nChangeSub)
    {
        _ASSERT_PTR(pCtx->CETTBI);
        
        if (pThr != pCtx->CETTBI)
        {
            pCtx->CETTBI->ThreadSubPrior = 0;
            pThr->ThreadSubPrior = 0;
            pThr->StartThreadTime = MxGetTicks();

            //keep tracking statictics 
            MxiIncContextSwitching(pCtx);
        }
    }
    else
    {
        pCtx->CETTBI = pCtx->MCET;
    }
    
    return &pThr->hw;
#endif    
}

/** @brief This function scans the list of scheduled threads 
           and returns the pointer to the highest priority thread.
           Also this function increases the internal priorities of the 
           scanned threads.

    @NOTE!  The interrupts have to be disabled 

    @param pCtx         [in] - the pointer to the context
    @param bChangeSub   [in] - to change sub priorities or not

    @return [PMTHREAD] the pointer to the thread context or NULL */

PMTHREAD MxiMFindHighestThread (PMXCONTEXT pCtx, BOOL bChangeSub)
{
    PMTHREAD pThr, phThr;
    UINT32 prior = 0;

    _ASSERT_PTR(pCtx);

    phThr = pThr = (PMTHREAD)pCtx->MFST;

    if (phThr != NULL)
    {
        prior = phThr->ThreadPrior + phThr->ThreadSubPrior;
        pThr = (PMTHREAD)pThr->list[MTHREAD_LIST_SCHED].NextScheduled;
    }

    while (pThr != NULL)
    {
        if (prior < (pThr->ThreadPrior + pThr->ThreadSubPrior))
        {
            prior = pThr->ThreadPrior + pThr->ThreadSubPrior;

            if (phThr != NULL)
            {
                // to increase the internal priority 
                // of the skipped thread

                if (bChangeSub)
                    phThr->ThreadSubPrior += phThr->ThreadPrior;
            }

            phThr = pThr;
        }
        else
        {
            if (bChangeSub)
                pThr->ThreadSubPrior += pThr->ThreadPrior;
        }

        pThr = (PMTHREAD)pThr->list[MTHREAD_LIST_SCHED].NextScheduled;
    }

    if (phThr != NULL)
    {
        if (bChangeSub)
            phThr->ThreadSubPrior = 0;
    }

    return phThr;
}

/** @brief This function scans the list of scheduled threads 
           and returns the pointer to the highest priority thread
           the excluded thread is skipped. 
           Also this function increases the internal priorities of the 
           scanned threads.

    @NOTE!  The interrupts have to be disabled 

    @param pCtx      [in] - the pointer to the context
    @param pExcThr   [in] - the pointer to the excluded thread

    @return [PMTHREAD] the pointer to the thread context or NULL */

PMTHREAD MxiMFindHighestThreadExclude (PMXCONTEXT pCtx, PMTHREAD pExcThr)
{
    PMTHREAD pThr, phThr;
    UINT32 prior = 0;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pExcThr);

    phThr = pThr = (PMTHREAD)pCtx->MFST;

    while (phThr != NULL)
    {
        if (phThr == pExcThr)
        {
            pThr = phThr = (PMTHREAD)phThr->list[MTHREAD_LIST_SCHED].NextScheduled;
            continue;
        }
    
        prior = phThr->ThreadPrior + phThr->ThreadSubPrior;
        pThr = (PMTHREAD)pThr->list[MTHREAD_LIST_SCHED].NextScheduled;
        break;
    }

    while (pThr != NULL)
    {
        if (pThr == pExcThr)
        {
            pThr = (PMTHREAD)pThr->list[MTHREAD_LIST_SCHED].NextScheduled;
            continue;
        }
    
        if (prior < (pThr->ThreadPrior + pThr->ThreadSubPrior))
        {
            prior = pThr->ThreadPrior + pThr->ThreadSubPrior;

            if (phThr != NULL)
            {
                // to increase the internal priority 
                // of the skipped thread
                
                phThr->ThreadSubPrior += phThr->ThreadPrior;
            }

            phThr = pThr;
        }
        else
        {
            pThr->ThreadSubPrior += pThr->ThreadPrior;
        }

        pThr = (PMTHREAD)pThr->list[MTHREAD_LIST_SCHED].NextScheduled;
    }

    if (phThr != NULL)
    {
        phThr->ThreadSubPrior = 0;
    }

    return phThr;
}

MXRC MxSwitchContextEx (PMTHREAD pThr2)
{
    PMTHREAD pThr1;
    IRQTYPE irq;
    PMXCONTEXT pCtx;
    UINT32 nMode;

    pCtx = MxInqContext ();
    _ASSERT_PTR(pCtx);

    MxiDisInt (&irq);
    pThr1 = (PMTHREAD)pCtx->MCET;           // the pointer to this m-thread

    _ASSERT_PTR(pThr1);

    if ((pThr1 != pThr2) && (pThr2 != NULL))
    {
        nMode = MxGetCpuMode ();
    
        if (nMode == CPU_MODE_IRQ)
        {
            pCtx->MCET = pThr2;
            pThr2->StartThreadTime = pCtx->IrqStartTime;
        }
        else if (nMode == CPU_MODE_SUPERVISOR)
        {
            pCtx->MCET = pThr2;
            pThr2->StartThreadTime = MxGetTicks();

            // keep tracking statistics of threads 
            // switching
            
            MxiIncContextSwitching (pCtx);
            MxiSwitch (&pThr1->hw, &pThr2->hw);
        }
    }

    MxiRestInt(irq);
    return MX_OK;
}

/** @brief This function switches context of the current thread by finding 
           another highest priority thread

    @return [MXRC] an error code */

MXRC MxSwitchContext (void)
{
    PMTHREAD pThr2;
    IRQTYPE irq;
    PMXCONTEXT pCtx;

    pCtx = MxInqContext ();
    _ASSERT_PTR(pCtx);

    MxiDisInt (&irq);

    pThr2 = MxiMFindHighestThread(pCtx, (MxGetCpuMode () == CPU_MODE_SUPERVISOR));    // The pointer to the next thread 

    if (pThr2 != NULL)
        MxSwitchContextEx (pThr2);
    
    MxiRestInt(irq);

    return MX_OK;
}

/** @brief This function removes current thread and switches context of the current thread by finding 
           another highest priority thread

    @return [MXRC] an error code */

MXRC MxCutAndSwitchCurThread (void)
{
    PMTHREAD pThr2, pThr1;
    IRQTYPE irq;
    PMXCONTEXT pCtx;
    UINT32 nMode;

    pCtx = MxInqContext ();
    _ASSERT_PTR(pCtx);

    MxiDisInt (&irq);

    pThr1 = (PMTHREAD)pCtx->MCET;
    pCtx->MCET = NULL;

    _ASSERT_PTR(pThr1);

    if (IS_M_SCHEDULED(pThr1))
        MxiMCutThreadFromSchList (pCtx, pThr1);

    pThr2 = MxiMFindHighestThread(pCtx, (MxGetCpuMode () == CPU_MODE_SUPERVISOR));    // The pointer to the next thread 

    if (pThr2 != NULL)
    {
        nMode = MxGetCpuMode ();
    
        if (nMode == CPU_MODE_IRQ)
        {
            pCtx->MCET = pThr2;
            pThr2->StartThreadTime = pCtx->IrqStartTime;
        }
        else if (nMode == CPU_MODE_SUPERVISOR)
        {
            pCtx->MCET = pThr2;
            pThr2->StartThreadTime = MxGetTicks();

            // keep tracking statistics of threads 
            // switching
            
            MxiIncContextSwitching (pCtx);
            
            MxiSwitch (&pThr1->hw, &pThr2->hw);
        }
    }
    
    MxiRestInt(irq);

    return MX_OK;
}


/** @brief This function has to be called by system IRQ handler to provide the possibility 
           to switch threads context from IRQ handlers, if this function is not called 
           the threads context remains the same and switching can be initiated by m-threads 
*/           

void MxHandleMThreadQuantums(void)
{
    PMTHREAD pThr, pThr2;

    // here we need to find the thread that has to be 
    // started and if currently executed thread has to be switched  

    PMXCONTEXT pCtx = MxInqContext();
    _ASSERT_PTR (pCtx);

    pThr = (PMTHREAD)pCtx->MCET;
    _ASSERT_PTR(pThr);

    if (pThr->Quantum != 0 && (pCtx->IrqStartTime - pThr->StartThreadTime) >= pThr->Quantum)
    {
        pThr2 = MxiMFindHighestThreadExclude(pCtx, pThr);    // The pointer to the next thread 
        MxSwitchContextEx (pThr2);
    }
}

/** @brief This function processes situation when SS m-thread has nothing to do,
           ans OS needs to switch to another thread or to call application idle handler 

    @param idle     [in] - the application IDLE handler 
    @param pParam   [in] - the idle parameter 

    @return [MXRC] an error code */

MXRC MxSwitchSSThread (TENTRY idle, LPVOID pParam)
{
    PMXCONTEXT pCtx = MxInqContext();
    IRQTYPE r;
    PMTHREAD pThr, pSSThr;

    _ASSERT_PTR(pCtx);

    MxiDisInt(&r);

    pSSThr = MxiMGetSSThread(pCtx);
    _ASSERT_PTR (pSSThr);

    pThr = MxiMFindHighestThreadExclude (pCtx, pSSThr);

    if (pThr != NULL)
    {
        MxSwitchContextEx(pThr);
        MxiRestInt(r);
    }
    else
    {
        if (idle != NULL)
        {
            #ifdef MX_EXT_THREAD_TRACER_ENABLED
                if (pCtx->ExtThrTracer != NULL)
                    pCtx->ExtThrTracer (EXT_TRACER_START_IDLE);
            #endif

            MxiRestInt(r);
            idle(pParam);
        }
        else
        {
            MxiRestInt(r);
        }
    }

    return MX_OK;
}

/** @brief This function schedules SS m-thread to process S-threads

    @return [MXRC] an error code */

MXRC MxScheduleSSThread(void)
{
    IRQTYPE r;
    PMXCONTEXT pCtx;
    PMTHREAD pThr;

    pCtx = MxInqContext();
    _ASSERT_PTR(pCtx);

    MxiDisInt(&r);

    pThr = MxiMGetSSThread(pCtx);
    _ASSERT_PTR(pThr);

    if (!IS_M_SCHEDULED (pThr) && !IS_MTHR_WAITING(pThr))
    {
        MxiMAddThreadToSchList (pCtx, pThr, MxiMFindSchPos(pCtx, pThr));
    }

    // Switch to the highest priority m-thread
    // if SS thread has highest priority
    // it will be provided by CPU control
    
    MxSwitchContext();

    MxiRestInt(r);

    return MX_OK;
}

/** @brief This function is designed to schedule m-thread and internally 
           called by MxScheduleThread. The thread is putted to the list of 
           executed threads and context switching is called. The next 
           high priority thread takes processor control

    @param h [in] - the m-thread handle 

    @return [MXRC] an error code */

MXRC MxScheduleMThread (HANDLE h)
{
    IRQTYPE r;
    PMTHREAD pThr;
    PMXCONTEXT pCtx;

    _ASSERT_HANDLE(h);
    _ASSERT_HANDLE_TYPE(h, OBJ_TYPE_MTHREAD);

    MxiDisInt(&r);

    pCtx = MxInqContext ();
    _ASSERT_PTR(pCtx);

    pThr = MxiGetMThreadDescr (pCtx, h);
    _ASSERT_PTR(pThr);

    if (!IS_MTHR_ALLOC(pThr))
    {
        MxiRestInt(r);
        return MX_MKERNEL_THREAD_ERROR;
    }

    // to schedule the thread 
    // if this thread is not under processing
    // and thread does not wait any system events like SEMAs
    // if the thread will wait for some event(s), that event object(s)
    // automatically schedule the thread in case of event
    
    if (!IS_M_SCHEDULED (pThr) && !IS_MTHR_WAITING (pThr))
    {
        MxiMAddThreadToSchList (pCtx, pThr, MxiMFindSchPos(pCtx, pThr));
    }

    MxSwitchContext();

    MxiRestInt(r);

    return MX_OK;
}

/** @brief This function is designed to un-schedule m-thread and internally 
           called from MxUnScheduleThread. The thread is removed from the list of 
           executed threads and context switching is called. The next 
           high priority thread takes processor control

    @param h [in] - the m-thread handle 

    @return [MXRC] an error code */

MXRC MxUnScheduleMThread (HANDLE h)
{
    PMXCONTEXT pCtx;
    IRQTYPE r;
    PMTHREAD pThr;

    _ASSERT_HANDLE(h);
    _ASSERT_HANDLE_TYPE(h, OBJ_TYPE_MTHREAD);

    pCtx = MxInqContext();
    _ASSERT_PTR(pCtx);

    MxiDisInt (&r);

    pThr = MxiGetMThreadDescr(pCtx, h);
    _ASSERT_PTR(pThr);

    // We need to remove this thread from the 
    // scheduled list 

    MxiMCutThreadFromSchList (pCtx, pThr);

    if (pThr == (PMTHREAD)pCtx->MCET)
    {
        MxCutAndSwitchCurThread();
    }
    else
    {
        MxSwitchContext();
    }

    MxiRestInt (r);

    return MX_OK;
}

/** @brief This function removes the thread from the list of executed threads 
           and prevents thread execution till Resume event

    @param h [in] - the handle of the m-thread 

    @return [MXRC] an error code */

MXRC MxSuspendMThread (HANDLE h)
{
    IRQTYPE r;
    PMTHREAD pThr;
    PMXCONTEXT pCtx;

    _ASSERT_HANDLE(h);
    _ASSERT_HANDLE_TYPE(h, OBJ_TYPE_MTHREAD);

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        return MX_OK;
    }

    MxiDisInt(&r);

    pCtx = MxInqContext ();
    _ASSERT_PTR(pCtx);

    pThr = MxiGetMThreadDescr (pCtx, h);
    _ASSERT_PTR(pThr);

    if (IS_M_SCHEDULED(pThr))
    {
        MxiMCutThreadFromSchList (pCtx, pThr);

        // we need to switch the context of the threads 
        MxSwitchContext();
    }

    MxiRestInt(r);

    return MX_OK;
}

/** @brief This function adds the thread to the list of executed threads 
           and allows thread execution

    @param h [in] - the handle of the m-thread 

    @return [MXRC] an error code */

MXRC MxResumeMThread (HANDLE h)
{
    IRQTYPE r;
    PMTHREAD pThr;
    PMXCONTEXT pCtx;

    _ASSERT_HANDLE(h);
    _ASSERT_HANDLE_TYPE(h, OBJ_TYPE_MTHREAD);

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        return MX_OK;
    }

    MxiDisInt(&r);

    pCtx = MxInqContext ();
    _ASSERT_PTR(pCtx);

    pThr = MxiGetMThreadDescr (pCtx, h);
    _ASSERT_PTR(pThr);

    // if the thread is not under execution 
    // to put the thread to the active list and 
    // iterate threads switching 
    
    if (!IS_M_SCHEDULED(pThr) && !IS_MTHR_WAITING(pThr))
    {
        MxiMAddThreadToSchList (pCtx, pThr, MxiMFindSchPos(pCtx, pThr));

        // we need to switch the context of the threads 
        MxSwitchContext();
    }

    MxiRestInt(r);

    return MX_OK;
}

/** @brief This function returns handle of current M-thread 
           executed on the processor

    @return [HANDLE]*/

HANDLE MxGetCurrentMThread (void)
{
    PMXCONTEXT pCtx;
    IRQTYPE r;
    HANDLE h = HNULL;

    pCtx = MxInqContext ();
    _ASSERT_PTR(pCtx);

    MxiDisInt(&r);

    if (pCtx->MCET == NULL)
        return h;

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_MTHREAD,
                ((UINT32) pCtx->MCET - (UINT32) pCtx->mthreads) / sizeof(MTHREAD));

    MxiRestInt(r);

    return h;
}

/** @brief This function returns a pointer to the m-thread designed to handle S-threads
    @return [PMTHREAD] the pointer to the m-thread context*/

PMTHREAD MxMGetSSThread (void)
{
    PMXCONTEXT pCtx = MxInqContext();
    _ASSERT_PTR(pCtx);
    return MxiGetMThreadDescr(pCtx, pCtx->hSSThread);
}

/** @brief This function waits for the object (event) specified by the handler
           if timeout is 0 - the function checks status of the object and returns immediately
           if timeout is INFINITE - the thread is removed from the execution and will be resumed when event is occured

    @param h [in] - the handle of SEMA, see SEMA API
    @param us[in] - the time of waiting in microseconds 

    @return [UINT32] the status , see: MX_WAIT_xxxx */
           
UINT32 MxWaitForSingleObject (HANDLE h, UINT32 us)
{
    UINT32 rc;
    _ASSERT_HANDLE(h);

    switch (HANDLE_GET_TYPE(h))
    {
        case OBJ_TYPE_SEMA:
            rc = MxLockSema(h);
            break;
    
        default:
            rc = WAIT_TIMEOUT;
            break; 
    }

    if (FAILED(rc))
       return WAIT_TIMEOUT;

    return WAIT_OBJECT_0;
}

/** @brief This function puts the thread to sleep list and thread 
           will be putted to the executed list again when waiting time is over 

    @param ticks [in] - the time in ticks 

    @return [MXRC] an error code */

MXRC MxSleep (UINT32 ticks)
{
    PMXCONTEXT pCtx = MxInqContext();
    IRQTYPE r;
    PMTHREAD pThr;
    
    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCtx->MCET);

    MxiDisInt (&r);

    if (pCtx->SchedNum == 1)
    {
        UINT32 t = MxGetTicks();

        MxiRestInt (r);
        while (MxGetTicks() - t < ticks);
        
        return MX_OK;
    }

    pThr = pCtx->MCET;

    MxiMCutThreadFromSchList (pCtx, pThr);

    pThr->StartSleepTime = MxGetTicks();
    pThr->SleepTime      = ticks;
    
    MxiPushToSleepList(pCtx, pThr, pThr->StartSleepTime);
    
    MxCutAndSwitchCurThread();

    MxiRestInt (r);

    return MX_OK;
}

#endif //MX_MTHREAD_ENABLED


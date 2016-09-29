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
#include "4gmx_sema.h"
#include "4gmx_serv.h"
#include "4gmx_kernel.h"
#include "4gmx_mkernel.h"
#include "4gmx_const.h"

#ifdef MX_MTHREAD_ENABLED


void MxiSemaPushThread (PMXSEMA pSema, PMTHREAD pThr)
{
    PMTHREAD tmp, prev = NULL;
    UINT32 nPrior;

    _ASSERT_PTR(pSema);
    _ASSERT_PTR(pThr);

    MART_MTHR_WAITING(pThr);

    // we need to find the proper place and to 
    // put new element to the list

    if (pSema->ThrNum == 0)
    {
        pSema->ThrList = pThr;

        pThr->list[MTHREAD_LIST_SEMA].NextScheduled = 0;
        pThr->list[MTHREAD_LIST_SEMA].PrevScheduled = 0;
    }
    else
    {
        tmp = pSema->ThrList;

        nPrior = pThr->ThreadPrior + pThr->ThreadSubPrior;

        while (tmp != NULL)
        {
            if (nPrior >= (tmp->ThreadPrior + tmp->ThreadSubPrior))
                break;

            prev= tmp;
            tmp = (PMTHREAD)tmp->list[MTHREAD_LIST_SEMA].NextScheduled;
        }

        // to put to the end of the list 
        
        if (tmp == NULL)
        {
            _ASSERT_PTR(prev);

            prev->list[MTHREAD_LIST_SEMA].NextScheduled = pThr;
            pThr->list[MTHREAD_LIST_SEMA].PrevScheduled = prev;
            pThr->list[MTHREAD_LIST_SEMA].NextScheduled = 0;
        }
        else
        {
            // we need to put the element before the <tmp> element

           pThr->list[MTHREAD_LIST_SEMA].NextScheduled = tmp;

           if (tmp->list[MTHREAD_LIST_SEMA].PrevScheduled != NULL)
           {
               tmp->list[MTHREAD_LIST_SEMA].PrevScheduled->list[MTHREAD_LIST_SEMA].NextScheduled = pThr;
           }

           pThr->list[MTHREAD_LIST_SEMA].PrevScheduled = tmp->list[MTHREAD_LIST_SEMA].PrevScheduled;
           tmp->list[MTHREAD_LIST_SEMA].PrevScheduled  = pThr;

           if (pSema->ThrList == tmp)
           {
               pSema->ThrList = pThr;
           }
        }
    }

    pSema->ThrNum++;
}

PMTHREAD MxiSemaPopThread (PMXSEMA pSema)
{
    PMTHREAD p;

    _ASSERT_PTR(pSema);

    if (pSema->ThrNum == 0)
        return NULL;

    pSema->ThrNum --;

    p = pSema->ThrList;
    
    pSema->ThrList = (PMTHREAD)pSema->ThrList->list[MTHREAD_LIST_SEMA].NextScheduled;

    if (pSema->ThrList != NULL)
        pSema->ThrList->list[MTHREAD_LIST_SEMA].PrevScheduled = NULL;

    p->list[MTHREAD_LIST_SEMA].NextScheduled = NULL;
    p->list[MTHREAD_LIST_SEMA].PrevScheduled = NULL;
    p->hWaitObj      = HNULL;

    MART_MTHR_NOT_WAITING(p);

    return p;
}

void MxiSemaProcThreadPrior(PMXSEMA pSema)
{
    PMTHREAD p;

    _ASSERT_PTR(pSema);

    p = pSema->ThrList;
    
    while (p != NULL)
    {
        p->ThreadSubPrior += p->ThreadPrior;
        p = (PMTHREAD)p->list[MTHREAD_LIST_SEMA].NextScheduled;
    }
}

UINT32 INLINE MxiSemaGetHighPrior(PMXSEMA pSema)
{
    _ASSERT_PTR(pSema);

    if (pSema->ThrList == 0)
        return 0;
    
    return (pSema->ThrList->ThreadPrior + pSema->ThrList->ThreadSubPrior);
}

/** @brief This function creates semaphore 

    @param nInitVal     [in]    - the initial value for semaphore (0 - means locked by default) 
    @param pName        [in]    - object name (it can be NULL)
    @param pOutHandle   [out]   - the thread handle

    */

MXRC MxCreateSema (UINT32 nInitVal, const char * pName, HANDLE * pOutHandle)
{
    PMXCONTEXT pCtx;
    PMXSEMA pSema;
    HANDLE h = HNULL;
    IRQTYPE irq;
    DEF_RADDR;

    _ASSERT(pOutHandle != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pSema = (PMXSEMA) MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_SEMA]);

    if (pSema == NULL)
    {
        MxiRestInt(irq);
        return MX_SEMA_ALLOC_ERROR;
    }

    Mximemset (pSema, 0, sizeof (*pSema));

    pSema->Value = nInitVal;

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_SEMA,
                ((((UINT32) pSema) -
                  ((UINT32) pCtx->semas)) / sizeof(MXSEMA)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (pName != NULL)
    {
        if (MxiAllocName(pCtx, h, pName) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_SEMA], pSema);
            MxiRestInt(irq);

            return MX_SEMA_ASSIGN_NAME_ERROR;
        }

        MARK_SEMA_NAMED(pSema);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_SEMA_UNNAMED(pSema);
    }

    MARK_SEMA_ALLOC(pSema);

    *pOutHandle = h;

    MxiRestInt(irq);

    return MX_OK;
}


#ifdef MX_INTERCPU_ENABLED

/** @brief The function creates semaphore object on remote system.

    @param cpu        [in] - the cpu ID
    @param nInitVal   [in]    - the initial value for semaphore (0 - means locked by default) 
    @param name       [in] - the object name (it can be NULL)
    @param pOutHandle [out] - the handle of created object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_event
*/
MXRC MxCreateSemaEx(CPUID cpu, UINT32 nInitVal, const char * pName, HANDLE * pOutHandle)
{
    MxICSemaCreate ic;

    _ASSERT_PTR(pOutHandle);

    if (cpu == MxGetCpuID() || cpu == CUR_CPU)
        return MxCreateSema(nInitVal, pName, pOutHandle);

    ic.InitState = nInitVal;
    ic.Handle    = pOutHandle;
    ic.Name      = pName;

    return MxICExec (cpu, MXIC_SEMA_CREATE, &ic, sizeof (ic));
}

#endif /*MX_INTERCPU_ENABLED*/

/** @brief This function removes semaphore and releases all waited threads 

    @param hSema [in] - the semaphore handle 

    @return [MXRC] an error code */

MXRC MxRemoveSema (HANDLE hSema)
{
    PMXCONTEXT pCtx;
    IRQTYPE r;
    PMXSEMA pSema;
    PMTHREAD pThr;
    UINT32 i = 0;

    _ASSERT_HANDLE(hSema);
    _ASSERT_HANDLE_TYPE(hSema, OBJ_TYPE_SEMA);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(hSema) != MxGetCpuID())
    {
        MxICSemaRemove ic;
        ic.Handle = hSema;
        return MxICExec(HANDLE_GET_CPU(hSema), MXIC_SEMA_REMOVE,
                         &ic, sizeof (ic));
    }

#else /*MX_INTERCPU_ENABLED*/

    /* Only locally created object can be removed */
    _ASSERT(HANDLE_GET_CPU(hSema) == MxGetCpuID());

#endif /*MX_INTERCPU_ENABLED*/

    pCtx = MxInqContext();
    _ASSERT_PTR(pCtx);

    MxiDisInt (&r);

    pSema = MxiGetSemaDescr(pCtx, hSema);
    _ASSERT_PTR(pSema);

    while ((pThr = MxiSemaPopThread (pSema)) != NULL)
    {
        i ++;
        MxiMAddThreadToSchList (pCtx, pThr, MxiMFindSchPos(pCtx, pThr));
    }

    pSema->CtrlFlags = 0;

    MxiFreeSysPart (&pCtx->parts[OBJ_TYPE_SEMA], pSema);

    if (i != 0)
        MxSwitchContext();

    MxiRestInt (r);

    return MX_OK;
}

/** @brief This function sets sema to some value and sema can be used 
           by the threads that are waiting for this sema
           
    @param h        [in] - the thread handler 
    @param nVal     [in] - new value for the sema

    @return [MXRC] an error code */

MXRC MxSetSema (HANDLE h, UINT32 nVal)
{
    PMXCONTEXT pCtx;
    IRQTYPE r;
    PMXSEMA pSema;
    PMTHREAD pThr;

    _ASSERT_HANDLE(h);
    _ASSERT_HANDLE_TYPE(h, OBJ_TYPE_SEMA);

#ifdef MX_INTERCPU_ENABLED

    if (HANDLE_GET_CPU(h) != MxGetCpuID())
    {
        MxICSemaSet ic;
        ic.Handle = h;
        ic.Value  = nVal;
        return MxICExec(HANDLE_GET_CPU(h), MXIC_SEMA_SET,
                         &ic, sizeof (ic));
    }

#else /*MX_INTERCPU_ENABLED*/

    /* Only locally created object can be removed */
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

#endif /*MX_INTERCPU_ENABLED*/

    pCtx = MxInqContext();
    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCtx->MCET);

    MxiDisInt (&r);

    pSema = MxiGetSemaDescr(pCtx, h);
    _ASSERT_PTR(pSema);

    pSema->Value = nVal;

    while (pSema->Value != 0 && pSema->ThrList != NULL)
    {
        pThr = MxiSemaPopThread (pSema);
        MxiMAddThreadToSchList (pCtx, pThr, MxiMFindSchPos(pCtx, pThr));
        
        pSema->Value --;
    }

    if (pSema->Value == 0 && pSema->ThrList != NULL)
    {
        MxiSemaProcThreadPrior (pSema);
    }

    MxSwitchContext();

    MxiRestInt (r);

    return MX_OK;
}

/** @brief This function locks sema and tries to get access to the resource 
           if no access the thread will be waiting 
           
    @param h        [in] - the thread handler 

    @return [MXRC] an error code */

MXRC MxLockSema (HANDLE h)
{
    PMXCONTEXT pCtx;
    IRQTYPE r;
    PMXSEMA pSema;
    PMTHREAD pThr;

    _ASSERT_HANDLE(h);
    _ASSERT_HANDLE_TYPE(h, OBJ_TYPE_SEMA);

    pCtx = MxInqContext();
    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCtx->MCET);

    MxiDisInt (&r);

    pSema = MxiGetSemaDescr(pCtx, h);
    _ASSERT_PTR(pSema);

    pThr = (PMTHREAD)pCtx->MCET;

    if (pSema->Value == 0)
    {
        // We need to remove this thread from the 
        // scheduled list and put to the list of waited 
        // threads 

        MxiMCutThreadFromSchList (pCtx, pThr);
        MxiSemaPushThread(pSema, pThr);

        pThr->hWaitObj = h;
        
        MxCutAndSwitchCurThread();
    }
    else
    {
        pSema->Value --;
        
        if ((pThr->ThreadPrior + pThr->ThreadSubPrior) < MxiSemaGetHighPrior(pSema))
        {
            // In this case the priority of waited threads are higher
            // than this thread, in this case current thread is putted to the 
            // queue queue

            pThr = (PMTHREAD)pCtx->MCET;
            pThr->hWaitObj = h;
            MxiMCutThreadFromSchList (pCtx, pThr);
            MxiSemaPushThread(pSema, pThr);

            pThr = MxiSemaPopThread (pSema);
            _ASSERT_PTR(pThr);
            MxiMAddThreadToSchList (pCtx, pThr, MxiMFindSchPos(pCtx, pThr));
            
            MxSwitchContextEx(pThr);
        }
    }

    MxiRestInt (r);

    return MX_OK;
}

/** @brief This function locks sema and tries to get access to the resource 
           if no access the thread will be waiting 
           
    @param h        [in] - the thread handler 
    @param us       [in] - the time in microsecords 

    @return [MXRC] an error code */
    
MXRC MxLockSemaEx(HANDLE h, UINT32 us)
{
    PMXCONTEXT pCtx;
    IRQTYPE r;
    PMXSEMA pSema;
    PMTHREAD pThr;

    _ASSERT_HANDLE(h);
    _ASSERT_HANDLE_TYPE(h, OBJ_TYPE_SEMA);

    pCtx = MxInqContext();
    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCtx->MCET);

    MxiDisInt (&r);

    pSema = MxiGetSemaDescr(pCtx, h);
    _ASSERT_PTR(pSema);

    if (pSema->Value == 0)
    {
        // We need to remove this thread from the 
        // scheduled list and put to the list of waited 
        // threads 

        pThr = (PMTHREAD)pCtx->MCET;

        MxiMCutThreadFromSchList (pCtx, pThr);
        MxiSemaPushThread(pSema, pThr);
        MxCutAndSwitchCurThread();
    }
    else
    {
        pSema->Value --;
    }

    MxiRestInt (r);

    return MX_OK;
}

/** @brief This function unlocks sema and provides access to the resource 

    @param h        [in] - the thread handler 

    @return [MXRC] an error code */

MXRC MxUnlockSema (HANDLE h)
{
    PMXCONTEXT pCtx;
    IRQTYPE r;
    PMXSEMA pSema;
    PMTHREAD pThr;

    _ASSERT_HANDLE(h);
    _ASSERT_HANDLE_TYPE(h, OBJ_TYPE_SEMA);

    pCtx = MxInqContext();
    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCtx->MCET);

    MxiDisInt (&r);

    pSema = MxiGetSemaDescr(pCtx, h);
    _ASSERT_PTR(pSema);

    pSema->Value++;

    while (pSema->Value != 0 && pSema->ThrList != NULL)
    {
        pThr = MxiSemaPopThread (pSema);
        MxiMAddThreadToSchList (pCtx, pThr, MxiMFindSchPos(pCtx, pThr));
        
        pSema->Value --;
    }

    if (pSema->Value == 0 && pSema->ThrList != NULL)
    {
        MxiSemaProcThreadPrior (pSema);
    }

    MxSwitchContext();

    MxiRestInt (r);

    return MX_OK;
}

#endif //MX_MTHREAD_ENABLED


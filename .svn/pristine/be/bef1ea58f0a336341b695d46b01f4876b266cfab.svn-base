//-------------------------------------------------------------------------------------------
/** @file 4gmx_serv.c
 *
 * @brief The file contains implementation of 4GMX  internal service functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.23 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_serv.h"
#include "4gmx_const.h"
#include <string.h>

#ifdef MX_INTERCPU_ENABLED
#include "4gmx_intercpu.h"
#endif

/************************************************************************/
/*      The pool API                                                    */
/************************************************************************/

/** @bridf The function initializes pool and has to be called first before other pool API are used

    @param pPool    [out]   - the pool descriptor 
    @param pStorage [in]    - the pointer to the storage (data blocks storage)
    @param nBlockNum[in]    - the number of blocks in the storage 
    @param nBlockSize[in]   - the size of data block
    @param pFreePtr [in]    - the pointer to the pool storage with free indexes 
    @param pUsedPtr [in]    - the pointer to the pool storage with indexes of used objects 

    Warning!
    ~~~~~~~~
    the size of queues (pFreePtr & pUsedPtr) should be (nBlockNum + 1)
*/
void MxiPoolInit(PMXPOOL pPool, void * pStorage, UINT32 nBlockNum, UINT32 nBlockSize, PUINT32 pFreePtr, PUINT32 pUsedPtr)
{
    UINT32 i;

    _ASSERT_PTR(pPool);
    _ASSERT_PTR(pFreePtr);
    _ASSERT_PTR(pUsedPtr);
    _ASSERT_PTR(pStorage);
    _ASSERT(nBlockSize > 0);

    memset (pPool, 0, sizeof (*pPool));

    pPool->StoragePtr = (PUINT8)pStorage;
    pPool->BlockSize  = nBlockSize;
    pPool->BlockNum   = nBlockNum;

    pPool->FreePtr = pFreePtr;
    pPool->UsedPtr = pUsedPtr;

    // to put the indexes to the free storage 

    i = 0;

    while (i < nBlockNum)
    {
        MxiPoolFree (pPool, pPool->StoragePtr + (pPool->BlockSize * i));
        i++;
    }
}

/** @brief This function allocates (returns) the index of free object 

    @param pPool [in] - the pool descriptor 

    @return [void*] the pointer to the allocated block*/

void* MxiPoolAlloc(PMXPOOL pPool)
{
    UINT32 nIndex;
    _ASSERT_PTR(pPool);

    if (pPool->FreeGet == pPool->FreePut)
        return NULL;

    nIndex = pPool->FreePtr[pPool->FreeGet++];

    if (pPool->FreeGet >= (pPool->BlockNum+1))
        pPool->FreeGet = 0;

    return pPool->StoragePtr + (pPool->BlockSize * nIndex);
}

/** @brief This function sends data block (puts a block index to the 'used storage')

    @param pPool [in] - the pool descriptor 
    @param pBlock[in] - the pointer to the block, allocated by MxiPoolAlloc(...)

    @return [BOOL] TRUE - sending is done successfully */

BOOL MxiPoolSend(PMXPOOL pPool, void * pBlock)
{
    UINT32 index;

    _ASSERT_PTR(pPool);
    _ASSERT_PTR(pBlock);

    _ASSERT(((UINT32)pBlock >= (UINT32)pPool->StoragePtr));
    _ASSERT(((UINT32)pBlock < (UINT32)pPool->StoragePtr+pPool->BlockSize*pPool->BlockNum));

    index = ((UINT32)pBlock - (UINT32)pPool->StoragePtr) / pPool->BlockSize;
    pPool->UsedPtr [pPool->UsedPut] = index;

    // ++ is here to prevent any problems when data is read on other core 
    // before it is really set on this core 
    
    if ((pPool->UsedPut + 1) >= (pPool->BlockNum+1))
    {
        pPool->UsedPut = 0;
    }
    else
    {
        pPool->UsedPut ++;
    }

    return TRUE;
}

/** @brief This function returns the pointer to the sent block 
           (reads index from the 'used storage' and converts it to the pointer)

    @param pPool [in] - the pool descriptor

    @return [void*] the pointer to the received block or NULL*/

void * MxiPoolRecv(PMXPOOL pPool)
{
    UINT32 index;

    _ASSERT_PTR(pPool);

    if (pPool->UsedGet == pPool->UsedPut)
        return NULL;

    index = pPool->UsedPtr[pPool->UsedGet++];

    if (pPool->UsedGet >= (pPool->BlockNum+1))
        pPool->UsedGet = 0;

    return pPool->StoragePtr + (index * pPool->BlockSize);
}

/** @brief This function frees block (puts block index to the 'free storage')

    @param pPool [in]  - the pool descriptor 
    @param pBlock[in]  - the pointer to the block allocated by MxiPoolAlloc(...)

    @return [BOOL] TRUE - free is done successfully*/

BOOL MxiPoolFree(PMXPOOL pPool, void * pBlock)
{
    UINT32 index;

    _ASSERT_PTR(pPool);
    _ASSERT_PTR(pBlock);

    _ASSERT(((UINT32)pBlock >= (UINT32)pPool->StoragePtr));
    _ASSERT(((UINT32)pBlock < (UINT32)pPool->StoragePtr+pPool->BlockSize*pPool->BlockNum));

    index = ((UINT32)pBlock - (UINT32)pPool->StoragePtr) / pPool->BlockSize;

    pPool->FreePtr [pPool->FreePut ++] = index;

    if (pPool->FreePut >= (pPool->BlockNum+1))
        pPool->FreePut = 0;

    return TRUE;
}

/************************************************************************/
/*      The partition API                                               */
/************************************************************************/
void MxiDefSysPart(SYSFPART * pPart, LPVOID pStorage, UINT32 nBlkSize,
                   UINT32 nBlkCnt)
{
    UINT32 *p;

    _ASSERT(pPart != NULL);
    _ASSERT(pStorage != NULL);

    pPart->storage = (UINT32 *) pStorage;
    pPart->freeblk = (UINT32 *) pStorage;
    pPart->blkcnt = nBlkCnt;
    pPart->blksize = nBlkSize;

    p = pPart->storage;

    while (--nBlkCnt)
    {
        *p = (UINT32)(((UINT8 *) p) + nBlkSize);
        p = (UINT32 *) * p;
    }

    *p = (UINT32) NULL;
}

/************************************************************************/
/*      The internal queue API                                          */
/************************************************************************/

void MxiDefSysQueue (SYSQUEUE * pQueue, LPVOID pStorage, UINT16 nCount, UINT16 nSize)
{
	_ASSERT_PTR (pQueue);
	_ASSERT_PTR (pStorage);
	_ASSERT (nSize == 4);
	_ASSERT (nCount != 0);

	pQueue->storage = (UINT32*)pStorage;
	pQueue->get     = 0;
	pQueue->put     = 0;
	pQueue->size    = nSize;
	pQueue->count   = nCount;
}


/************************************************************************/
/*      The names' table handling                                       */
/************************************************************************/

#ifdef MX_NAMING_ENABLED

BOOL MxiAllocName(PMXCONTEXT pCtx, HANDLE h, const char *name)
{
    SINT32 pos, inspos = 0, nLen;

    _ASSERT(pCtx != NULL);
    _ASSERT(name != NULL);

    if ((pCtx->NameCount + 1) >= MX_MAX_NAMETABLE_SIZE)
        return FALSE;

    pos = MxiFindName(pCtx, name, &inspos);

    if (pos < 0)
    {
        Mximemmove(&pCtx->NameTable[inspos + 1], &pCtx->NameTable[inspos],
                   sizeof(OBJNAME) * (pCtx->NameCount - inspos));

        pCtx->NameTable[inspos].hObj = h;

        nLen = MIN(strlen(name), MX_MAX_OBJ_NAME);

        strncpy(pCtx->NameTable[inspos].name, name, nLen);

        pCtx->NameTable[inspos].name[nLen] = '\0';

        /*strcpy (pCtx->NameTable [inspos].name, name); */

        pCtx->NameCount++;
    }
    else
    {
        /* the name is already used */

        return FALSE;
    }

    return TRUE;
}

HANDLE MxiGetObjByName(PMXCONTEXT pCtx, const char *name)
{
    SINT32 pos, inspos;

    _ASSERT(pCtx != NULL);
    _ASSERT(name != NULL);

    pos = MxiFindName(pCtx, name, &inspos);

    if (pos > -1)
    {
        return pCtx->NameTable[pos].hObj;
    }

    return HNULL;
}

void MxiFreeName(PMXCONTEXT pCtx, HANDLE h)
{
    SINT32 pos = -1, i;

    _ASSERT(pCtx != NULL);

    for (i = 0; i < (SINT32) pCtx->NameCount; i++)
    {
        if (*(UINT32 *) & pCtx->NameTable[i].hObj == *(UINT32 *) & h)
        {
            pos = i;
        }
    }

    if (pos >= 0)
    {
        Mximemmove(&pCtx->NameTable[pos], &pCtx->NameTable[pos + 1],
                   sizeof(OBJNAME) * (pCtx->NameCount - pos + 1));

        pCtx->NameCount--;
    }
}

SINT32 MxiFindName(PMXCONTEXT pCtx, const char *name, SINT32 * pnInsPos)
{
    SINT32 nBegin, nEnd, nCenter, nCount;
    int res;

    _ASSERT(pCtx != NULL);
    _ASSERT(name != NULL);

    nCount = (SINT32) pCtx->NameCount;

    nBegin = 0;
    nEnd = nCount;

    while ((nBegin <= nEnd) && (nBegin < nCount))
    {
        nCenter = ((nBegin + nEnd) >> 1);

        res = strcmp(name, pCtx->NameTable[nCenter].name);

        if (res < 0)
        {
            nEnd = nCenter - 1;
        }
        else if (res > 0)
        {
            nBegin = nCenter + 1;
        }
        else
        {
            return nCenter;
        }
    }

    if (pnInsPos != NULL)
    {
        if (nEnd < 0)
        {
            *pnInsPos = nBegin;
        }
        else if (nBegin >= (SINT32) pCtx->NameCount)
        {
            *pnInsPos = nCount;
        }
        else
        {
            *pnInsPos = nBegin;
        }
    }
    return -1;
}

BOOL MxiIsObjNamed(PMXCONTEXT pCtx, HANDLE h)
{
    switch (HANDLE_GET_TYPE(h))
    {
        case OBJ_TYPE_THREAD:
            return IS_THR_NAMED(MxiGetThreadDescr(pCtx, h));

#ifdef MX_EVENT_ENABLED
        case OBJ_TYPE_EVENT:
            return IS_EVN_NAMED(MxiGetEventDescr(pCtx, h));
#endif /*MX_EVENT_ENABLED */

#ifdef MX_LIST_ENABLED
        case OBJ_TYPE_LIST:
            return IS_LST_NAMED(MxiGetListDescr(pCtx, h));
#endif /*MX_LIST_ENABLED */

#ifdef MX_TIMER_ENABLED
        case OBJ_TYPE_TIMER:
            return IS_TMR_NAMED(MxiGetTimerDescr(pCtx, h));
#endif /*MX_TIMER_ENABLED */

#ifdef MX_PART_ENABLED
        case OBJ_TYPE_PARTITION:
            return IS_PART_NAMED(MxiGetPartDescr(pCtx, h));
#endif /*MX_PART_ENABLED */

#ifdef MX_QUEUE_ENABLED
        case OBJ_TYPE_QUEUE:
            return IS_QUEUE_NAMED(MxiGetQueueDescr(pCtx, h));
#endif /*MX_QUEUE_ENABLED */

#ifdef MX_DRV_FRAMEWORK_ENABLED
        case OBJ_TYPE_DRIVER:
            return (pCtx->
                    DrvNames[HANDLE_GET_INDEX(h) / 32] >> (HANDLE_GET_INDEX(h) %
                                                           32)) & 1;
#endif /*MX_DRV_FRAMEWORK_ENABLED */

        default:
            return FALSE;
    }
}

void MxiMarkObjUnnamed(PMXCONTEXT pCtx, HANDLE h)
{
    switch (HANDLE_GET_TYPE(h))
    {
        case OBJ_TYPE_THREAD:
            {
                MARK_THR_UNNAMED(MxiGetThreadDescr(pCtx, h));
                break;
            }

#ifdef MX_EVENT_ENABLED
        case OBJ_TYPE_EVENT:
            {
                MARK_EVN_UNNAMED(MxiGetEventDescr(pCtx, h));
                break;
            }
#endif /*MX_EVENT_ENABLED */

#ifdef MX_LIST_ENABLED
        case OBJ_TYPE_LIST:
            {
                MARK_LST_UNNAMED(MxiGetListDescr(pCtx, h));
                break;
            }
#endif /*MX_LIST_ENABLED */

#ifdef MX_TIMER_ENABLED
        case OBJ_TYPE_TIMER:
            {
                MARK_TMR_UNNAMED(MxiGetTimerDescr(pCtx, h));
                break;
            }
#endif /*MX_TIMER_ENABLED */

#ifdef MX_PART_ENABLED
        case OBJ_TYPE_PARTITION:
            {
                MARK_PART_UNNAMED(MxiGetPartDescr(pCtx, h));
                break;
            }
#endif /*MX_PART_ENABLED */

#ifdef MX_QUEUE_ENABLED
        case OBJ_TYPE_QUEUE:
            {
                MARK_QUEUE_UNNAMED(MxiGetQueueDescr(pCtx, h));
                break;
            }
#endif /*MX_QUEUE_ENABLED */

#ifdef MX_DRV_FRAMEWORK_ENABLED
        case OBJ_TYPE_DRIVER:
            {
                pCtx->DrvNames[HANDLE_GET_INDEX(h) / 32] &=
                    ~(1 << (HANDLE_GET_INDEX(h) % 32));
                break;
            }
#endif /*MX_DRV_FRAMEWORK_ENABLED */

        default:
            break;
    }
}

void MxiMarkObjNamed(PMXCONTEXT pCtx, HANDLE h)
{
    switch (HANDLE_GET_TYPE(h))
    {
        case OBJ_TYPE_THREAD:
            {
                MARK_THR_NAMED(MxiGetThreadDescr(pCtx, h));
                break;
            }

#ifdef MX_EVENT_ENABLED
        case OBJ_TYPE_EVENT:
            {
                MARK_EVN_NAMED(MxiGetEventDescr(pCtx, h));
                break;
            }
#endif /*MX_EVENT_ENABLED */

#ifdef MX_LIST_ENABLED
        case OBJ_TYPE_LIST:
            {
                MARK_LST_NAMED(MxiGetListDescr(pCtx, h));
                break;
            }
#endif /*MX_LIST_ENABLED */

#ifdef MX_TIMER_ENABLED
        case OBJ_TYPE_TIMER:
            {
                MARK_TMR_NAMED(MxiGetTimerDescr(pCtx, h));
                break;
            }
#endif /*MX_TIMER_ENABLED */

#ifdef MX_PART_ENABLED
        case OBJ_TYPE_PARTITION:
            {
                MARK_PART_NAMED(MxiGetPartDescr(pCtx, h));
                break;
            }
#endif /*MX_PART_ENABLED */

#ifdef MX_QUEUE_ENABLED
        case OBJ_TYPE_QUEUE:
            {
                MARK_QUEUE_NAMED(MxiGetQueueDescr(pCtx, h));
                break;
            }
#endif /*MX_QUEUE_ENABLED */

#ifdef MX_DRV_FRAMEWORK_ENABLED
        case OBJ_TYPE_DRIVER:
            {
                pCtx->DrvNames[HANDLE_GET_INDEX(h) / 32] |=
                    (1 << (HANDLE_GET_INDEX(h) % 32));
                break;
            }
#endif /*MX_DRV_FRAMEWORK_ENABLED */

        default:
            break;
    }
}

#endif /*MX_NAMING_ENABLED */

/************************************************************************/
/*          Objects control                                             */
/************************************************************************/

/** @breif The function returns a pointer to the thread place or NULL
   if the list is empty or thread should be added to the end of the list.

 @param pCtx  [in] - the pointer to 4GMX context
 @param pThread [in] - the pointer to the thread

 @return [PTHREAD] the thread plase or NULL if list was empty

 \ingroup group_lte_4gmx_service
 */

#ifdef MX_SCH_SEARCH_FROM_BEGIN

PTHREAD MxiFindSchPos(PMXCONTEXT pCtx, PTHREAD pThread)
{
    PTHREAD p;
    TPRIORITY pr;

    _ASSERT(pCtx != NULL);
    _ASSERT(pThread != NULL);

    p = (PTHREAD) pCtx->FST;

    pr = pThread->Priority;

    while (p)
    {
        if (pr < p->Priority)
            return p;

        p = (PTHREAD) p->NextScheduled;
    }

    return p;
}

void MxiAddThreadToSchList(PMXCONTEXT pCtx, PTHREAD pThr, PTHREAD pBeforeThis)
{
    _ASSERT(pCtx != NULL);
    _ASSERT(pThr != NULL);

    if (pBeforeThis == NULL)
    {
        if (pCtx->FST == NULL)
        {
            pCtx->FST = pThr;
            pCtx->LST = pThr;

            pThr->NextScheduled = NULL;
            pThr->PrevScheduled = NULL;
        }
        else
        {
            pCtx->LST->NextScheduled = pThr;
            pThr->PrevScheduled = pCtx->LST;
            pCtx->LST = pThr;

            pThr->NextScheduled = NULL;
        }
    }
    else
    {
        if (pBeforeThis->PrevScheduled != NULL)
        {
            pBeforeThis->PrevScheduled->NextScheduled = pThr;
        }

        pThr->PrevScheduled = pBeforeThis->PrevScheduled;

        pThr->NextScheduled = pBeforeThis;
        pBeforeThis->PrevScheduled = pThr;

        if (pCtx->FST == pBeforeThis)
        {
            pCtx->FST = pThr;
        }
    }

    MARK_SCHEDULED(pThr);
}

#else /*if not MX_SCH_SEARCH_FROM_BEGIN */

PTHREAD MxiFindSchPos(PMXCONTEXT pCtx, PTHREAD pThread)
{
    PTHREAD p;
    TPRIORITY pr;

    _ASSERT(pCtx != NULL);
    _ASSERT(pThread != NULL);

    p = (PTHREAD) pCtx->LST;

    pr = pThread->Priority;

    while (p)
    {
        if (p->Priority <= pr)
            return p;

        p = (PTHREAD) p->PrevScheduled;
    }

    return p;
}

void MxiAddThreadToSchList(PMXCONTEXT pCtx, PTHREAD pThr, PTHREAD pAfterThis)
{
    _ASSERT(pCtx != NULL);
    _ASSERT(pThr != NULL);

    if (pAfterThis == NULL)
    {
        if (pCtx->FST == NULL)
        {
            pCtx->FST = pThr;
            pCtx->LST = pThr;

            pThr->NextScheduled = NULL;
            pThr->PrevScheduled = NULL;
        }
        else
        {
            pThr->NextScheduled = pCtx->FST;
            pThr->PrevScheduled = NULL;

            pCtx->FST->PrevScheduled = pThr;
            pCtx->FST = pThr;
        }
    }
    else
    {
        pThr->NextScheduled = pAfterThis->NextScheduled;

        if (pAfterThis->NextScheduled != NULL)
        {
            pAfterThis->NextScheduled->PrevScheduled = pThr;
        }

        pAfterThis->NextScheduled = pThr;
        pThr->PrevScheduled = pAfterThis;

        if (pCtx->LST == pAfterThis)
        {
            pCtx->LST = pThr;
        }
    }

    MARK_SCHEDULED(pThr);
}

#endif /*MX_SCH_SEARCH_FROM_BEGIN */

#ifdef MX_MTHREAD_ENABLED
PMTHREAD MxiMFindSchPos(PMXCONTEXT pCtx, PMTHREAD pThread)
{
    PMTHREAD p;
    TPRIORITY pr;

    _ASSERT(pCtx != NULL);
    _ASSERT(pThread != NULL);

    p = (PMTHREAD) pCtx->MLST;

    pr = pThread->ThreadPrior;

    while (p)
    {
        if (p->ThreadPrior >= pr)
            return (PMTHREAD)p;

        p = (PMTHREAD)p->list[MTHREAD_LIST_SCHED].PrevScheduled;
    }

    return p;
}

void MxiMAddThreadToSchList(PMXCONTEXT pCtx, PMTHREAD pThr, PMTHREAD pAfterThis)
{
    _ASSERT(pCtx != NULL);
    _ASSERT(pThr != NULL);

    if (IS_M_SCHEDULED(pThr))
        return;

    if (pAfterThis == NULL)
    {
        if (pCtx->MFST == NULL)
        {
            pCtx->MFST = pThr;
            pCtx->MLST = pThr;

            pThr->list[MTHREAD_LIST_SCHED].NextScheduled = NULL;
            pThr->list[MTHREAD_LIST_SCHED].PrevScheduled = NULL;
        }
        else
        {
            pCtx->MLST->list[MTHREAD_LIST_SCHED].NextScheduled = pThr;
            pThr->list[MTHREAD_LIST_SCHED].PrevScheduled = pCtx->MLST;
            pThr->list[MTHREAD_LIST_SCHED].NextScheduled = NULL;
            pCtx->MLST = pThr;
        }
    }
    else
    {
        pThr->list[MTHREAD_LIST_SCHED].NextScheduled = pAfterThis->list[MTHREAD_LIST_SCHED].NextScheduled;

        if (pAfterThis->list[MTHREAD_LIST_SCHED].NextScheduled != NULL)
        {
            pAfterThis->list[MTHREAD_LIST_SCHED].NextScheduled->list[MTHREAD_LIST_SCHED].PrevScheduled = pThr;
        }

        pThr->list[MTHREAD_LIST_SCHED].PrevScheduled = pAfterThis;
        pAfterThis->list[MTHREAD_LIST_SCHED].NextScheduled = pThr;

        if (pCtx->MLST == pAfterThis)
        {
            pCtx->MLST = pThr;
        }
    }

    pCtx->SchedNum++;
    
    pThr->ThreadSubPrior = 0;
    MARK_M_SCHEDULED(pThr);
}

void MxiMAddThreadToList(PMXCONTEXT pCtx, MTHREAD_LIST_ID id, PMTHREAD pAfterThis, PMTHREAD pThr)
{
    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pThr);
    _ASSERT_PTR(pAfterThis);

    pThr->list[id].NextScheduled = pAfterThis->list[id].NextScheduled;

    if (pAfterThis->list[id].NextScheduled != NULL)
    {
        pAfterThis->list[id].NextScheduled->list[id].PrevScheduled = pThr;
    }

    pThr->list[id].PrevScheduled = pAfterThis;
    pAfterThis->list[id].NextScheduled = pThr;
}

#endif // MX_MTHREAD_ENABLED

/** @breif The function returns a pointer to the thread place or NULL
   if the list is empty or a thread should be added to the end of the list.

 @param pCtx     [in] - the pointer to 4GMX context
 @param pr       [in] - the priority of the scheduled thread
 @param nAtBegin [in] - flag: 1 - the new thread will be added like the first element (to the same priority list)
                              0 - the new theread will be added like the last element (to the same priority list)

 @return [PTHREAD] the thread plase or NULL if list was empty

 \ingroup group_lte_4gmx_service
 */

PTHREAD MxiFindSchPosEx(PMXCONTEXT pCtx, TPRIORITY pr, UINT32 nAtBegin)
{
    PTHREAD p;

    _ASSERT(pCtx != NULL);

    p = (PTHREAD) pCtx->FST;

    if (nAtBegin != 0)
    {
        while (p)
        {
            if (pr <= p->Priority)
                return p;

            p = (PTHREAD) p->NextScheduled;
        }
    }
    else
    {
        while (p)
        {
            if (pr < p->Priority)
                return p;

            p = (PTHREAD) p->NextScheduled;
        }
    }

    return p;
}


#ifdef MX_INTERCPU_ENABLED

/** @breif The function returns a pointer to the thread place or NULL
   if the list is empty or a thread should be added to the end of the list.

 @param pCtx     [in] - the pointer to 4GMX context
 @param pr       [in] - the priority of the scheduled thread
 @param nAtBegin [in] - flag: 1 - the new thread will be added like the first element (to the same priority list)
                              0 - the new theread will be added like the last element (to the same priority list)

 @return [PTHREAD] the thread plase or NULL if list was empty

 \ingroup group_lte_4gmx_service
 */

PTHREAD MxiFindSmpSchPos(MXSMPCTX * pSmpCtx, TPRIORITY pr, UINT32 nAtBegin)
{
    PTHREAD p;

    _ASSERT(pSmpCtx != NULL);

    p = (PTHREAD) pSmpCtx->FST;

    if (nAtBegin != 0)
    {
        while (p)
        {
            if (pr <= p->Priority)
                return p;

            p = (PTHREAD) p->NextScheduled;
        }
    }
    else
    {
        while (p)
        {
            if (pr < p->Priority)
                return p;

            p = (PTHREAD) p->NextScheduled;
        }
    }

    return p;
}

/** @brief This function adds new one thread to the list of SMP threads,
           the scheduled thread will be processed by the first available core

    @param pSmpCtx      [in] - the pointer to the SMP context
    @param pThr         [in] - the pointer to the thread handle
    @param pAfterThis   [in] - the pointer to the place

    \ingroup group_lte_4gmx_service
    */

void MxiAddThreadToSmpSchList(MXSMPCTX *pSmpCtx, PTHREAD pThr, PTHREAD pAfterThis)
{
    _ASSERT(pSmpCtx != NULL);
    _ASSERT(pThr != NULL);

    if (pAfterThis == NULL)
    {
        if (pSmpCtx->FST== NULL)
        {
            pSmpCtx->FST = pThr;
            pSmpCtx->LST = pThr;

            pThr->NextScheduled = NULL;
            pThr->PrevScheduled = NULL;
        }
        else
        {
            pThr->NextScheduled = pSmpCtx->FST;
            pThr->PrevScheduled = NULL;

            pSmpCtx->FST->PrevScheduled = pThr;
            pSmpCtx->FST = pThr;
        }
    }
    else
    {
        pThr->NextScheduled = pAfterThis->NextScheduled;

        if (pAfterThis->NextScheduled != NULL)
        {
            pAfterThis->NextScheduled->PrevScheduled = pThr;
        }

        pAfterThis->NextScheduled = pThr;
        pThr->PrevScheduled = pAfterThis;

        if (pSmpCtx->LST == pAfterThis)
        {
            pSmpCtx->LST = pThr;
        }
    }

    MARK_SCHEDULED(pThr);
}


#endif // MX_INTERCPU_ENABLED

/** @brief The function cuts thread context from the double linked list (the list of scheduled threads )

    @param pCtx [in] -the pointer to the 4GMX context
    @param pThr [in] -the pointer to the thread context

    \ingroup group_lte_4gmx_service
*/
void MxiCutThreadFromSchList(PMXCONTEXT pCtx, PTHREAD pThr)
{
    _ASSERT(pCtx != NULL);
    _ASSERT(pThr != NULL);

    if (pThr->PrevScheduled != NULL)
    {
        pThr->PrevScheduled->NextScheduled = pThr->NextScheduled;
    }

    if (pThr->NextScheduled != NULL)
    {
        pThr->NextScheduled->PrevScheduled = pThr->PrevScheduled;
    }

    if (pCtx->FST == pThr)
    {
        pCtx->FST = pThr->NextScheduled;
    }

    if (pCtx->LST == pThr)
    {
        pCtx->LST = pThr->PrevScheduled;
    }

    pThr->NextScheduled = NULL;
    pThr->PrevScheduled = NULL;

    MARK_UNSCHEDULED(pThr);
}

#ifdef MX_MTHREAD_ENABLED
void MxiMCutThreadFromSchList(PMXCONTEXT pCtx, PMTHREAD pThr)
{
    _ASSERT(pCtx != NULL);
    _ASSERT(pThr != NULL);

    if (!IS_M_SCHEDULED(pThr))
        return;

    if (pThr->list[MTHREAD_LIST_SCHED].PrevScheduled != NULL)
    {
        pThr->list[MTHREAD_LIST_SCHED].PrevScheduled->list[MTHREAD_LIST_SCHED].NextScheduled = pThr->list[MTHREAD_LIST_SCHED].NextScheduled;
    }

    if (pThr->list[MTHREAD_LIST_SCHED].NextScheduled != NULL)
    {
        pThr->list[MTHREAD_LIST_SCHED].NextScheduled->list[MTHREAD_LIST_SCHED].PrevScheduled = pThr->list[MTHREAD_LIST_SCHED].PrevScheduled;
    }

    if (pCtx->MFST == pThr)
    {
        pCtx->MFST = pThr->list[MTHREAD_LIST_SCHED].NextScheduled;
    }

    if (pCtx->MLST == pThr)
    {
        pCtx->MLST = pThr->list[MTHREAD_LIST_SCHED].PrevScheduled;
    }

    pThr->list[MTHREAD_LIST_SCHED].NextScheduled = NULL;
    pThr->list[MTHREAD_LIST_SCHED].PrevScheduled = NULL;
    pThr->ThreadSubPrior = 0;

    pCtx->SchedNum--;

    MARK_M_UNSCHEDULED(pThr);
}

void MxiMCutThreadFromList(PMXCONTEXT pCtx, PMTHREAD pThr, MTHREAD_LIST_ID id)
{
    _ASSERT(pCtx != NULL);
    _ASSERT(pThr != NULL);

    if (pThr->list[id].PrevScheduled != NULL)
    {
        pThr->list[id].PrevScheduled->list[id].NextScheduled = pThr->list[id].NextScheduled;
    }

    if (pThr->list[id].NextScheduled != NULL)
    {
        pThr->list[id].NextScheduled->list[id].PrevScheduled = pThr->list[id].PrevScheduled;
    }

    pThr->list[id].NextScheduled = NULL;
    pThr->list[id].PrevScheduled = NULL;
}

#endif

/************************************************************************/
/*       Interrupts control (platform independent)                      */
/************************************************************************/
MXRC MxiSyncCtrlOn(OBJTYPE type, LPVOID pObj)
{
#ifdef MX_ARM_ENABLED

    volatile MXSYNC sync;
    UINT32 nHwSema = 0;
    UINT32 nTimeout= 0;
    UINT32 time;

#ifndef MX_ARM_SMP_ENABLED
    UINT32   nSize = 0;
#endif

    switch (type)
    {
#ifdef MX_QUEUE_ENABLED
        case OBJ_TYPE_QUEUE:
            {
                sync = ((PQUEUE) pObj)->Sync;

                nHwSema = IS_QUEUE_HW_SEMA((PQUEUE) pObj);
                nTimeout= ((PQUEUE) pObj)->SyncTicks;

#ifndef MX_ARM_SMP_ENABLED
                nSize = sizeof (QUEUE);
#endif
                break;
            }
#endif

#ifdef MX_PART_ENABLED
        case OBJ_TYPE_PARTITION:
            {
                sync = ((PPART) pObj)->Sync;

                nHwSema = IS_PART_HW_SEMA((PPART) pObj);

#ifndef MX_ARM_SMP_ENABLED
                nSize = sizeof (PART);
#endif
                break;
            }
#endif

#ifdef MX_LIST_ENABLED
        case OBJ_TYPE_LIST:
            {
                sync = ((PLIST) pObj)->Sync;

#ifndef MX_ARM_SMP_ENABLED
                nSize = sizeof (LIST);
#endif
                break;
            }
#endif

#ifdef MX_HEAP_ENABLED
        case OBJ_TYPE_HEAP:
            {
                sync = ((PHEAP) pObj)->Sync;

#ifndef MX_ARM_SMP_ENABLED
                nSize = sizeof (HEAP);
#endif
                break;
            }
#endif

#ifdef MX_RHEAP_ENABLED
        case OBJ_TYPE_RHEAP:
            {
                sync = ((PRHEAP) pObj)->Sync;

#ifndef MX_ARM_SMP_ENABLED
                nSize = sizeof (RHEAP);
#endif
                break;
            }
#endif

        default:
            _ASSERT(FALSE);
#ifndef  _MX_DEBUG
            return MX_SERV_RC_UNSUPPORTED_OBJ;
#endif
    }

    if (nHwSema != 0)
    {
        time = MxGetTicks ();
    
        // the logic of HW sema is implemented here
        while (TRUE)
        {
            if (nTimeout != 0)
            {
                if ((MxGetTicks () - time) > nTimeout)
                    return MX_SERV_RC_LOCK_TIMEOUT;
            }
        
            if (*sync == 0)
                break;
        }
    }
    else
    {
        MxGetLock(sync);
    }

#ifndef MX_ARM_SMP_ENABLED
    MxiCacheInvalid(pObj, nSize);
#endif

#elif defined (MX_CEVA_ENABLED)

#endif /*MX_ARM_ENABLED */

    return MX_OK;
}

void MxiSyncCtrlOff(OBJTYPE type, LPVOID pObj)
{
    volatile MXSYNC sync;

#ifndef MX_ARM_SMP_ENABLED
    UINT32 nSize = 0;
#endif

    UINT32 nHwSema = 0;

#ifdef MX_ARM_ENABLED

    switch (type)
    {
#ifdef MX_QUEUE_ENABLED
        case OBJ_TYPE_QUEUE:
            {
                sync = ((PQUEUE) pObj)->Sync;

                nHwSema = IS_QUEUE_HW_SEMA((PQUEUE) pObj);

#ifndef MX_ARM_SMP_ENABLED
                nSize = sizeof (QUEUE);
#endif
                break;
            }
#endif

#ifdef MX_PART_ENABLED
        case OBJ_TYPE_PARTITION:
            {
                sync = ((PPART) pObj)->Sync;

                nHwSema = IS_PART_HW_SEMA((PPART) pObj);

#ifndef MX_ARM_SMP_ENABLED
                nSize = sizeof (PART);
#endif
                break;
            }
#endif

#ifdef MX_LIST_ENABLED
        case OBJ_TYPE_LIST:
            {
                sync = ((PLIST) pObj)->Sync;

#ifndef MX_ARM_SMP_ENABLED
                nSize = sizeof (LIST);
#endif
                break;
            }
#endif

#ifdef MX_HEAP_ENABLED
        case OBJ_TYPE_HEAP:
            {
                sync = ((PHEAP) pObj)->Sync;

#ifndef MX_ARM_SMP_ENABLED
                nSize = sizeof (HEAP);
#endif
                break;
            }
#endif

#ifdef MX_RHEAP_ENABLED
        case OBJ_TYPE_RHEAP:
            {
                sync = ((PRHEAP) pObj)->Sync;

#ifndef MX_ARM_SMP_ENABLED
                nSize = sizeof (RHEAP);
#endif
                break;
            }
#endif
        default:
            _ASSERT(FALSE);
#ifndef  _MX_DEBUG
            return;
#endif
    }

#ifndef MX_ARM_SMP_ENABLED
    MxiCacheClean(pObj, nSize);
#endif

    if (nHwSema != 0)
    {
        *sync = 0; // to release the HW semaphore
    }
    else
    {
        MxReleaseLock(sync);
    }

#elif defined (MX_CEVA_ENABLED)

#endif /*MX_ARM_ENABLED */
}


#ifdef MX_HWSEMA_ENABLED

    /** @brief This function initializes HW semahore
        @param semaID [in] - the semaphore ID , please see MX_HW_SEMA_xxx of config file
        \ingroup group_lte_4gmx_service

        */

    void MxiInitHwSema (PMXCONTEXT pCtx, UINT32 semaID)
    {
        _ASSERT_PTR(pCtx);
        _ASSERT_PTR(pCtx->HwSemaBase);
    
        MXREG32(pCtx->HwSemaBase + (semaID << 2)) = 0;
    }

    /** @brief This function is specially designed to support T4K platform HW semas
        @param semaID [in] - the sema ID (please see MX_HW_SEMA_xxx of config file)
        \ingroup group_lte_4gmx_service

        */

    void MxiLockHwSema (UINT32 semaID)
    {
        PMXCONTEXT pCtx = MxInqContext();

        _ASSERT_PTR(pCtx);
        _ASSERT_PTR(pCtx->HwSemaBase);
    
        while (MXREG32(pCtx->HwSemaBase + (semaID << 2)) != 0)
        {
            _NOP();
        }
    }

    /** @brief This function is specially designed to support T4K platform HW semas
        @param semaID [in] - the sema ID (please see MX_HW_SEMA_xxx of config file)
        \ingroup group_lte_4gmx_service

        */

    void MxiUnlockHwSema (UINT32 semaID)
    {
        PMXCONTEXT pCtx = MxInqContext();
        _ASSERT_PTR(pCtx);
        _ASSERT_PTR(pCtx->HwSemaBase);
        
        MXREG32(pCtx->HwSemaBase + (semaID << 2)) = 0;
    }

#endif // MX_HWSEMA_ENABLED

/************************************************************************/
/*          Inter-CPU communication                                     */
/************************************************************************/

#ifdef MX_INTERCPU_ENABLED

#endif /*MX_INTERCPU_ENABLED */

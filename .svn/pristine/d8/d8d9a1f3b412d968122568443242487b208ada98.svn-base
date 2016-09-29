//-------------------------------------------------------------------------------------------
/** @file 4gmx_serv.h
 *
 * @brief The file contains declaration of 4GMX  internal service functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.14 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_SERV_H_
#define _4GMX_SERV_H_

#include <stdlib.h>
#include <string.h>
#include "4gmx_config.h"
#include "4gmx_types.h"
#include "4gmx_kernel.h"

#ifdef MX_ARM_ENABLED
#include "4gmx_arm.h"
#endif

/************************************************************************/
/*       The list of module error codes                                 */
/************************************************************************/

#define MX_SERV_RC_LOCK_TIMEOUT     DEF_MXRC(MXM_SERV,  1)  // It was impossible to get the lock for specified period of time
#define MX_SERV_RC_UNSUPPORTED_OBJ  DEF_MXRC(MXM_SERV,  2)  // This type of object is not supported

/************************************************************************/
/*      The pool API                                                    */
/************************************************************************/
/** @bridf The function initializes pool and has to be called first before other pool API are used

    @param pPool    [out]   - the pool descriptor 
    @param pStorage [in]    - the pointer to the storage (data blocks storage )
    @param nBlockNum[in]    - the number of blocks in the storage 
    @param nBlockSize[in]   - the size of data block
    @param pFreePtr [in]    - the pointer to the pool storage with free indexes 
    @param pUsedPtr [in]    - the pointer to the pool storage with indexes of used objects 

    Warning!
    ~~~~~~~~
    the size of queues (pFreePtr & pUsedPtr) should be (nBlockNum + 1)
    
*/
void MxiPoolInit(PMXPOOL pPool, void * pStorage, UINT32 nBlockNum, UINT32 nBlockSize, PUINT32 pFreePtr, PUINT32 pUsedPtr);

/** @brief This function allocates (returns) the index of free object 

    @param pPool [in] - the pool descriptor 

    @return [void*] the pointer to the allocated block*/

void* MxiPoolAlloc(PMXPOOL pPool);

/** @brief This function sends data block (puts a block index to the 'used storage')

    @param pPool [in] - the pool descriptor 
    @param pBlock[in] - the pointer to the block, allocated by MxiPoolAlloc(...)

    @return [BOOL] TRUE - sending is done successfully */

BOOL MxiPoolSend(PMXPOOL pPool, void * pBlock);

/** @brief This function returns the pointer to the sent block 
           (reads index from the 'used storage' and converts it to the pointer)

    @param pPool [in] - the pool descriptor

    @return [void*] the pointer to the received block or NULL*/

void * MxiPoolRecv(PMXPOOL pPool);

/** @brief This function frees block (puts block index to the 'free storage')

    @param pPool [in]  - the pool descriptor 
    @param pBlock[in]  - the pointer to the block allocated by MxiPoolAlloc(...)

    @return [BOOL] TRUE - free is done successfully*/

BOOL MxiPoolFree(PMXPOOL pPool, void * pBlock);

/************************************************************************/
/*      The partition API                                               */
/************************************************************************/

void MxiDefSysPart(SYSFPART * pPart, LPVOID pStorage, UINT32 nBlkSize,
                   UINT32 nBlkCnt);

INLINE LPVOID MxiAllocSysPart(SYSFPART * pPart)
{
    UINT32 *p;

    _ASSERT(pPart != NULL);

    p = pPart->freeblk;

    if (p)
    {
        pPart->freeblk = (UINT32 *) * p;
        pPart->AllocCnt++;
    }

    return p;
}

INLINE UINT32 MxiGetBlockIndex (SYSFPART * pPart, LPVOID pBlock)
{
    _ASSERT_PTR(pPart);
    _ASSERT_PTR(pBlock);

    return ((UINT32)pBlock - (UINT32)pPart->storage) / pPart->blksize;
}

INLINE void MxiFreeSysPart(SYSFPART * pPart, LPVOID pBlk)
{
    _ASSERT(pPart != NULL);
    _ASSERT(pBlk != NULL);

    *(UINT32 *) pBlk = (UINT32) pPart->freeblk;
    pPart->freeblk = (UINT32 *) pBlk;

    pPart->AllocCnt--;
}


/************************************************************************/
/*      The internal queue API                                          */
/************************************************************************/

void MxiDefSysQueue (SYSQUEUE * pQueue, LPVOID pStorage,
					 UINT16 nCount, UINT16 nSize);

INLINE BOOL MxiPutSysQueue (SYSQUEUE * pQueue, UINT32 nData)
{
	_ASSERT_PTR (pQueue);

	pQueue->storage [pQueue->put] = nData;

#if 0

	pQueue->put = (pQueue->put + 1) % pQueue->count;

#else
    pQueue->put++;

    if (pQueue->put >= pQueue->count)
        pQueue->put = 0;

#endif

	return TRUE;
}

INLINE BOOL MxiGetSysQueue (SYSQUEUE * pQueue, UINT32 * pnData)
{
	_ASSERT_PTR (pQueue);
	_ASSERT_PTR (pnData);

	if (pQueue->put == pQueue->get)
		return FALSE;

	*pnData = pQueue->storage [pQueue->get];

#if 0
	pQueue->get = (pQueue->get + 1) % pQueue->count;
#else
	pQueue->get++;

    if (pQueue->get >= pQueue->count)
        pQueue->get = 0;
#endif

	return TRUE;
}

INLINE UINT32 MxiGetSysQueueElms (SYSQUEUE * pQueue)
{
	_ASSERT_PTR (pQueue);

	if (pQueue->put >= pQueue->get)
		return (pQueue->put - pQueue->get);

	return pQueue->count - (pQueue->get - pQueue->put);
}

INLINE BOOL MxiIsSysQueueEmpty (SYSQUEUE * pQueue)
{
	_ASSERT_PTR (pQueue);

	return pQueue->put == pQueue->get;
}

/************************************************************************/
/*      The memory API                                                  */
/************************************************************************/

/*INLINE LPVOID Mximemset(LPVOID pMem, int c, UINT32 nSize)
{
    return (void *)memset(pMem, c, nSize);
}

INLINE LPVOID Mximemmove(void *dest, const void *src, UINT32 count)
{
    return memmove(dest, src, count);
}

INLINE LPVOID Mximemcpy(void *dest, const void *src, UINT32 count)
{
    return memcpy(dest, src, count);
}*/

/*INLINE LPVOID Mximemset(LPVOID pMem, int c, UINT32 nSize)
{
    PUINT8 p = (PUINT8)pMem;

    while (nSize --)
    {
        *p++ = c;
    }

    return pMem;
}

INLINE LPVOID Mximemcpy(void *dest, const void *src, UINT32 count)
{
    PUINT8 p1 = (PUINT8)dest;
    PUINT8 p2 = (PUINT8)src;

    while (count --)
    {
        *p1++ = *p2++;
    }

    return dest;
}

INLINE LPVOID Mximemmove(void *dest, const void *src, UINT32 count)
{
    PUINT8 p1 = (PUINT8)dest;
    PUINT8 p2 = (PUINT8)src;

    while (count --)
    {
        *p1++ = *p2++;
}

    return dest;
} */



#define Mximemset(pMem, c, nSize) memset ((pMem), (c), (nSize))
#define Mximemmove(dest,src,count)memmove ((dest), (src), (count))
#define Mximemcpy(dest,src,count)memcpy ((dest), (src), (count))
#define Mxistrlen(str)strlen ((str))

/************************************************************************/
/*      The names' table handling                                       */
/************************************************************************/

#ifdef MX_NAMING_ENABLED

BOOL MxiAllocName(PMXCONTEXT pCtx, HANDLE h, const char *name);
HANDLE MxiGetObjByName(PMXCONTEXT pCtx, const char *name);
void MxiFreeName(PMXCONTEXT pCtx, HANDLE h);
SINT32 MxiFindName(PMXCONTEXT pCtx, const char *name, SINT32 * pnInsPos);
BOOL MxiIsObjNamed(PMXCONTEXT pCtx, HANDLE h);
void MxiMarkObjUnnamed(PMXCONTEXT pCtx, HANDLE h);
void MxiMarkObjNamed(PMXCONTEXT pCtx, HANDLE h);

#endif /*MX_NAMING_ENABLED */

/************************************************************************/
/*          Objects control                                             */
/************************************************************************/

INLINE PTHREAD MxiGetThreadDescr(PMXCONTEXT pCtx, HANDLE h)
{
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_THREAD);
    _ASSERT(HANDLE_GET_INDEX(h) < MX_MAX_THREADS);

    return &pCtx->threads[HANDLE_GET_INDEX(h)];
}

#ifdef MX_MTHREAD_ENABLED

INLINE PMTHREAD MxiGetMThreadDescr(PMXCONTEXT pCtx, HANDLE h)
{
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_MTHREAD);
    _ASSERT(HANDLE_GET_INDEX(h) < MX_MAX_MTHREADS);

    return &pCtx->mthreads[HANDLE_GET_INDEX(h)];
}

INLINE PMXSEMA MxiGetSemaDescr(PMXCONTEXT pCtx, HANDLE h)
{
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_SEMA);
    _ASSERT(HANDLE_GET_INDEX(h) < MX_MAX_SEMA);

    return &pCtx->semas[HANDLE_GET_INDEX(h)];
}

#endif

#ifdef MX_EVENT_ENABLED

INLINE PEVENT MxiGetEventDescr(PMXCONTEXT pCtx, HANDLE h)
{
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_EVENT);
    _ASSERT(HANDLE_GET_INDEX(h) < MX_MAX_EVENTS);

    return &pCtx->events[HANDLE_GET_INDEX(h)];
}

#endif

#ifdef MX_TIMER_ENABLED

INLINE PTIMER MxiGetTimerDescr(PMXCONTEXT pCtx, HANDLE h)
{
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_TIMER);
    _ASSERT(HANDLE_GET_INDEX(h) < MX_MAX_TIMERS);

    return &pCtx->timers[HANDLE_GET_INDEX(h)];
}

#endif

#ifdef MX_PART_ENABLED

INLINE PPART MxiGetPartDescr(PMXCONTEXT pCtx, HANDLE h)
{
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_PARTITION);
    _ASSERT(HANDLE_GET_INDEX(h) < MX_MAX_PARTITIONS);

    return &pCtx->partitions[HANDLE_GET_INDEX(h)];
}

#endif

#ifdef MX_HEAP_ENABLED

INLINE PHEAP MxiGetHeapDescr(PMXCONTEXT pCtx, HANDLE h)
{
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_INDEX(h) < MX_MAX_HEAPS);

    return &pCtx->heaps[HANDLE_GET_INDEX(h)];
}

#endif

#ifdef MX_RHEAP_ENABLED

INLINE PRHEAP MxiGetRHeapDescr(PMXCONTEXT pCtx, HANDLE h)
{
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_RHEAP);
    _ASSERT(HANDLE_GET_INDEX(h) < MX_MAX_RHEAPS);

    return &pCtx->rheaps[HANDLE_GET_INDEX(h)];
}

#endif

#ifdef MX_QUEUE_ENABLED

INLINE PQUEUE MxiGetQueueDescr(PMXCONTEXT pCtx, HANDLE h)
{
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_QUEUE);
    _ASSERT(HANDLE_GET_INDEX(h) < MX_MAX_QUEUES);

    return &pCtx->queues[HANDLE_GET_INDEX(h)];
}

#endif

#ifdef MX_LIST_ENABLED

INLINE PLIST MxiGetListDescr(PMXCONTEXT pCtx, HANDLE h)
{
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_LIST);
    _ASSERT(HANDLE_GET_INDEX(h) < MX_MAX_LISTS);

    return &pCtx->lists[HANDLE_GET_INDEX(h)];
}

#endif

#ifdef MX_TRACER_ENABLED
INLINE  MX_TRACER_CTX * MxiGetTracerDescr(PMXCONTEXT pCtx, HANDLE h)
{
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_TRACER);
    _ASSERT(HANDLE_GET_INDEX(h) < MX_MAX_TRACERS);

    return &pCtx->tracers[HANDLE_GET_INDEX(h)];
}
#endif


PTHREAD MxiFindSchPos(PMXCONTEXT pCtx, PTHREAD pThread);

#ifdef MX_MTHREAD_ENABLED
PMTHREAD MxiMFindSchPos(PMXCONTEXT pCtx, PMTHREAD pThread);
#endif

/** @breif The function returns a pointer to the thread place or NULL
   if the list is empty or a thread should be added to the end of the list.

 @param pCtx     [in] - the pointer to 4GMX context
 @param pr       [in] - the priority of the scheduled thread
 @param nAtBegin [in] - flag: 1 - the new thread will be added like the first element (to the same priority list)
                              0 - the new theread will be added like the last element (to the same priority list)

 @return [PTHREAD] the thread plase or NULL if list was empty */

PTHREAD MxiFindSchPosEx(PMXCONTEXT pCtx, TPRIORITY pr, UINT32 nAtBegin);

#ifdef MX_INTERCPU_ENABLED

/** @breif The function returns a pointer to the thread place or NULL
   if the list is empty or a thread should be added to the end of the list.

 @param pCtx     [in] - the pointer to 4GMX context
 @param pr       [in] - the priority of the scheduled thread
 @param nAtBegin [in] - flag: 1 - the new thread will be added like the first element (to the same priority list)
                              0 - the new theread will be added like the last element (to the same priority list)

 @return [PTHREAD] the thread plase or NULL if list was empty */

PTHREAD MxiFindSmpSchPos(MXSMPCTX *pSmpCtx, TPRIORITY pr, UINT32 nAtBegin);

/** @brief This function adds new one thread to the list of SMP threads,
           the scheduled thread will be processed by the first available core

    @param pSmpCtx      [in] - the pointer to the SMP context
    @param pThr         [in] - the pointer to the thread handle
    @param pAfterThis   [in] - the pointer to the place   */

void MxiAddThreadToSmpSchList(MXSMPCTX *pSmpCtx, PTHREAD pThr, PTHREAD pAfterThis);

#endif //MX_INTERCPU_ENABLED

void MxiAddThreadToSchList(PMXCONTEXT pCtx, PTHREAD pThr, PTHREAD pAfterThis);
void MxiCutThreadFromSchList(PMXCONTEXT pCtx, PTHREAD pThr);

#ifdef MX_MTHREAD_ENABLED
void MxiMAddThreadToSchList(PMXCONTEXT pCtx, PMTHREAD pThr, PMTHREAD pBeforeThis);
void MxiMAddThreadToList(PMXCONTEXT pCtx, MTHREAD_LIST_ID id, PMTHREAD pAfterThis, PMTHREAD pThr);
void MxiMCutThreadFromSchList(PMXCONTEXT pCtx, PMTHREAD pThr);
void MxiMCutThreadFromList(PMXCONTEXT pCtx, PMTHREAD pThr, MTHREAD_LIST_ID id);
#endif

/************************************************************************/
/*       Interrupts control (platform independent)                      */
/************************************************************************/

INLINE void MxiRestInt(IRQTYPE irq)
{
#ifdef MX_ARM_ENABLED

    ARM_INT_restore(irq);

#elif defined (MX_CEVA_ENABLED)

#endif /*MX_ARM_ENABLED */
}

INLINE void MxiDisInt(IRQTYPE * irq)
{
#ifdef MX_ARM_ENABLED

    *irq = ARM_INT_disable();

#elif defined (MX_CEVA_ENABLED)

#endif /*MX_ARM_ENABLED */
}

/************************************************************************/
/*       Inter-CPU communication                                        */
/************************************************************************/

MXRC MxiSyncCtrlOn(OBJTYPE type, LPVOID pObj);
void MxiSyncCtrlOff(OBJTYPE type, LPVOID pObj);

#ifdef MX_ARM_MMU_ENABLED

#define MxiCacheInvalid(pData, nSize) MxCacheInvalidate((LPVOID)(pData), (UINT32)(nSize))
#define MxiCacheClean(pData, nSize)   MxCacheClean((LPVOID)(pData), (UINT32)(nSize))
#define MxiCacheFlush(pData, nSize)   MxCacheFlush((LPVOID)(pData), (UINT32)(nSize))

#else

// The cache is not used, it does not needed to invalidate/clean caches

#define MxiCacheInvalid(pData, nSize)
#define MxiCacheClean(pData, nSize)
#define MxiCacheFlush(pData, nSize)

#endif //MX_ARM_ENABLED

/** @brief This function initializes HW semahore 
    @param semaID [in] - the semaphore ID , please see MX_HW_SEMA_xxx of config file */

void MxiInitHwSema (PMXCONTEXT pCtx, UINT32 semaID);

/** @brief This function is specially designed to support T4K platform HW semas
    @param semaID [in] - the sema ID (please see MX_HW_SEMA_xxx of config file) */
    
void MxiLockHwSema (UINT32 semaID);

/** @brief This function is specially designed to support T4K platform HW semas
    @param semaID [in] - the sema ID (please see MX_HW_SEMA_xxx of config file) */
    
void MxiUnlockHwSema (UINT32 semaID);

#endif /*_4GMX_SERV_H_*/

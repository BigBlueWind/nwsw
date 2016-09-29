/** @file osal.c
 *
 * @brief Implementation of OSAL for Linux
 * @author Mindspeed Technologies
 * @version $Revision: 1.53 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_LINUX

#include "osal.h"
#include <sched.h>
#include <sys/times.h>

#undef DEBUG_MEMPART

//
// Globals
//
OSAL_HEAP OsalGlobalHeap;

//
// External dependencies
//
//extern void CtrlcHandler(int);

#ifdef OSAL_LINUX_PTHREAD_SEMA
// For all threads
pthread_mutex_t ThreadMutex = PTHREAD_MUTEX_INITIALIZER;
#endif /* OSAL_LINUX_PTHREAD_SEMA */

/*! \brief Initialize all OS-dependent structures
 *
 *  \return Standard result code
*/
RESULTCODE OsalInit(U32 cpuid)
{
    return SUCCESS;
}


/*! \brief Create task
 *
 *  \param pTask [OUT] Pointer to task structure
 *  \param pEntryPoint Message source/destination
 *  \param pTaskParam Parameter for task startup routine
 *  \param taskName (Optional) Task name - can be NULL
 *  \param taskPriority Task priority
 *  \param pStackBase Not used in Win32
 *  \param stackSize Size of stack in bytes
 *  \return Standard result code
*/
RESULTCODE OsalCreateTask(OSAL_TASK *pTask, OSAL_PADDRESS pEntryPoint,
                          PTR pTaskParam, const char *taskName,
                          OSAL_PRIORITY taskPriority,
                          OSAL_PADDRESS pStackBase, OSAL_SIZE stackSize)
{
    RESULTCODE ret = FAILURE;
    pthread_attr_t attr;

    if (pEntryPoint == NULL)
    {
        *pTask = 0;
        return ret;
    }

    pthread_attr_init(&attr);
    // FIXME: use stack or not ?
//  pthread_attr_setstacksize(&attr, stackSize);
    if (pthread_create((pthread_t*)pTask, &attr, pEntryPoint, pTaskParam) == 0)
    {
        // FIXME: use scheduler or not?
//      pthread_setschedprio(*pTask, taskPriority);
        ret = SUCCESS;
    }
    else
    {
        *pTask = 0;
    }
//  pthread_attr_destroy(&attr);
    return ret;
}

/*! \brief Destroy task
 *
 *  \param pTask Pointer to task structure
 *  \return Standard result code
*/
RESULTCODE OsalDestroyTask(OSAL_TASK *pTask)
{
    pthread_cancel((pthread_t)*pTask);
    *pTask = 0;
    return SUCCESS;
}

/*! \brief Activate (resume) task
 *
 *  \param pTask Pointer to task structure
 *  \return Standard result code
*/
RESULTCODE OsalActivateTask(OSAL_TASK *pTask)
{
    // No support in Linux ?
    if (*pTask != 0)
    {
        return SUCCESS;
    }
    return FAILURE;
}

/*! \brief Stop task execution (pause)
 *
 *  \param pTask Pointer to task structure
 *  \return Standard result code
*/
RESULTCODE OsalDeactivateTask(OSAL_TASK *pTask)
{
    // No support in Linux ?
    return SUCCESS;
}

/*! \brief Change task priority
 *
 *  \param pTask Pointer to task structure
 *  \param TaskPriority New task's priority
 *  \return Standard result code
*/
RESULTCODE OsalSetTaskPriority(OSAL_TASK *pTask, OSAL_PRIORITY taskPriority)
{
    struct sched_param param;
    int policy;

    // Get current scheduling policy
    if (pthread_getschedparam(*pTask, &policy, &param) != 0)
    {
        return FAILURE;
    }
    param.sched_priority = taskPriority;
    // Set new priority
    if (pthread_setschedparam(*pTask, policy, &param) != 0)
    {
        return FAILURE;
    }
    return SUCCESS;
}

/*! \brief Query task priority
 *
 *  \param pTask Pointer to task structure
 *  \param TaskPriority [OUT] Return value
 *  \return Standard result code
*/
RESULTCODE OsalGetTaskPriority(OSAL_TASK *pTask, OSAL_PRIORITY *pTaskPriority)
{
    struct sched_param param;
    int policy;

    // Get current scheduling policy
    if (pthread_getschedparam(*pTask, &policy, &param) != 0)
    {
        return FAILURE;
    }
    *pTaskPriority = (OSAL_PRIORITY)param.sched_priority;

    return SUCCESS;
}

RESULTCODE OsalWaitForTaskClose(OSAL_TASK *pTask, OSAL_DELAYU timeout, BOOLEAN forceClose)
{
    if (pTask != NULL && *pTask != 0)
    {
        pthread_join((pthread_t) *pTask, NULL);
    }
    return SUCCESS;
}

/*! \brief Delay current task execution for period of time
 *
 *  \param delayMicroSeconds Time in microseconds
 *  \return Standard result code
*/
RESULTCODE OsalWaitDelay(OSAL_DELAYU delayMicroSeconds)
{
    if (delayMicroSeconds < 1000000)
    {
        if (usleep((useconds_t)delayMicroSeconds) == 0)
        {
            return SUCCESS;
        }
    }
    else
    {
        struct timespec req;

        req.tv_sec = delayMicroSeconds / 1000000;
        req.tv_nsec = (delayMicroSeconds % 1000000) * 1000;
        if (nanosleep(&req, NULL) == 0)
        {
            return SUCCESS;
        }
    }
    return FAILURE;
}

/*! \brief Places current task to the end of ready queue
 *
 *  \return Standard result code
*/
RESULTCODE OsalYield(void)
{
    pthread_yield();
    return SUCCESS;
}

/*! \brief Create semaphore (event) object
 *
 *  \param pSemaphore [OUT] Pointer to newly created semaphore
 *  \return Standard result code
*/
RESULTCODE OsalCreateSema(OSAL_SEMAPHORE *pSemaphore)
{
#ifdef OSAL_LINUX_PTHREAD_SEMA
    if (pthread_cond_init(pSemaphore, NULL) == 0)
    {
        return SUCCESS;
    }
#else
    // Not shared
    if (sem_init((sem_t*)pSemaphore, 0, 0) == 0)
    {
        return SUCCESS;
    }
#endif
    return FAILURE;
}

/*! \brief Destroy semaphore (event) object
 *
 *  \param pSemaphore Pointer to semaphore object
 *  \return Standard result code
*/
RESULTCODE OsalDestroySema(OSAL_SEMAPHORE *pSemaphore)
{
#ifdef OSAL_LINUX_PTHREAD_SEMA
    if (pthread_cond_destroy(pSemaphore) == 0)
    {
        bzero(pSemaphore, sizeof(OSAL_SEMAPHORE));
        return SUCCESS;
    }
#else
    if (sem_destroy(pSemaphore) == 0)
    {
        bzero(pSemaphore, sizeof(OSAL_SEMAPHORE));
        return SUCCESS;
    }

#endif /* OSAL_LINUX_PTHREAD_SEMA */
    return FAILURE;
}

/*! \brief Put semaphore (event) object into signaling state
 *
 *  \param pSemaphore Pointer to semaphore object
 *  \return Standard result code
*/
RESULTCODE OsalRaiseSema(OSAL_SEMAPHORE *pSemaphore)
{
#ifdef OSAL_LINUX_PTHREAD_SEMA
    if (pthread_cond_signal(pSemaphore) == 0)
    {
        return SUCCESS;
    }
#else
    if (sem_post(pSemaphore) == 0)
    {
        return SUCCESS;
    }
#endif
    return FAILURE;
}

/*! \brief Reset semaphore (event) object into inactive state
 *
 *  \param pSemaphore Pointer to semaphore object
 *  \return Standard result code
*/
RESULTCODE OsalClearSema(OSAL_SEMAPHORE *pSemaphore)
{
    // ???
    return FAILURE;
}

/*! \brief Wait for single semaphore (event) object to become active
 *
 *  \param pSemaphore Pointer to semaphore object
 *  \param timeoutMicroSeconds Wait timeout in microseconds, 0 - infinite
 *  \return Standard result code
*/
RESULTCODE OsalWaitForSema(OSAL_SEMAPHORE *pSemaphore,
                           OSAL_DELAYU timeoutMicroSeconds)
{
#ifdef OSAL_LINUX_PTHREAD_SEMA
    if (timeoutMicroSeconds == OSAL_WAIT_INFINITE)
    {
        pthread_mutex_lock(&ThreadMutex);
        pthread_cond_wait(pSemaphore, &ThreadMutex);
        pthread_mutex_unlock(&ThreadMutex);
    }
    else
    {
        struct timespec currTime, timeout;

        timeout.tv_sec = timeoutMicroSeconds / 1000000;
        timeout.tv_nsec = (timeoutMicroSeconds % 1000000) * 1000;

        clock_gettime(CLOCK_REALTIME, &currTime);

        timeout.tv_sec += currTime.tv_sec;
        timeout.tv_nsec += currTime.tv_nsec;
        if (timeout.tv_nsec >= 1000000000)
        {
            timeout.tv_sec++;
            timeout.tv_nsec -= 1000000000;
        }

        pthread_mutex_lock(&ThreadMutex);
        result = pthread_cond_timedwait(pSemaphore, &ThreadMutex, &timeout);
        pthread_mutex_unlock(&ThreadMutex);
        if (result == ETIMEDOUT)
            return EXPIRED;
    }
#else /* OSAL_LINUX_PTHREAD_SEMA */
    if (timeoutMicroSeconds == OSAL_WAIT_INFINITE)
    {
        if (sem_wait(pSemaphore) == 0)
        {
            return SUCCESS;
        }
    }
    else
    {
        struct timespec currTime, timeout;

        timeout.tv_sec = timeoutMicroSeconds / 1000000;
        timeout.tv_nsec = (timeoutMicroSeconds % 1000000) * 1000;

        clock_gettime(CLOCK_REALTIME, &currTime);

        timeout.tv_sec += currTime.tv_sec;
        timeout.tv_nsec += currTime.tv_nsec;
        if (timeout.tv_nsec >= 1000000000)
        {
            timeout.tv_sec++;
            timeout.tv_nsec -= 1000000000;
        }

        if (sem_timedwait(pSemaphore, &timeout ) != -1)
        {
             return SUCCESS;
        }
        else
        {
            if (errno == ETIMEDOUT)
            {
                return EXPIRED;
            }
        }
    }
#endif
    return SUCCESS;
}

/*! \brief Check if semaphore (event) objects is in signaling state
 *
 *  \param pSemaphoreList Pointer to NULL-terminated array of semaphore objects
 *  \return Standard result code
*/
RESULTCODE OsalCheckSema(OSAL_SEMAPHORE *pSemaphore)
{
#ifdef OSAL_LINUX_PTHREAD_SEMA
    return FAILURE;
#else
    int sval;

    if (sem_getvalue((sem_t*)pSemaphore, &sval) < 0)
    {
        return FAILURE;
    }
    if (sval > 0)
    {
        return SUCCESS;
    }
    return FAILURE;
#endif /* OSAL_LINUX_PTHREAD_SEMA */
}

/*! \brief Create mutex object
 *
 *  \param pMutex [OUT] Pointer to variable which receive new mutex handle
 *  \return Standard result code
*/
RESULTCODE OsalCreateMutex(OSAL_MUTEX *pMutex)
{
    RESULTCODE ret = FAILURE;
    pthread_mutexattr_t attr;

    if (pthread_mutexattr_init(&attr) == 0)
    {
        if (pthread_mutex_init((pthread_mutex_t*)pMutex, &attr) == 0)
        {
            ret = SUCCESS;
        }
    }
    pthread_mutexattr_destroy(&attr);
    return ret;
}

/*! \brief Create mutex object
 *
 *  \param pMutex Pointer to mutex object to destroy
 *  \return Standard result code
*/
RESULTCODE OsalDestroyMutex(OSAL_MUTEX *pMutex)
{
    if (pthread_mutex_destroy((pthread_mutex_t*)pMutex) < 0)
    {
        return FAILURE;
    }
    bzero(pMutex, sizeof(OSAL_MUTEX));
    return SUCCESS;
}

/*! \brief Take ownership on mutex object
 *
 *  \param pMutex Pointer to mutex object
 *  \param timeoutMicroSeconds Timeout in microseconds, 0 - infinite
 *  \return Standard result code
*/
RESULTCODE OsalCaptureMutex(OSAL_MUTEX *pMutex, OSAL_DELAYU timeoutMicroSeconds)
{
    if (timeoutMicroSeconds == OSAL_WAIT_INFINITE)
    {
        // Fast mutex - blocking
        if (pthread_mutex_lock((pthread_mutex_t*)pMutex) == 0)
        {
            return SUCCESS;
        }
    }
    else
    {
        struct timespec currTime, timeout;
        int result;

        timeout.tv_sec = timeoutMicroSeconds / 1000000;
        timeout.tv_nsec = (timeoutMicroSeconds % 1000000) * 1000;

        clock_gettime(CLOCK_REALTIME, &currTime);

        timeout.tv_sec += currTime.tv_sec;
        timeout.tv_nsec += currTime.tv_nsec;
        if (timeout.tv_nsec >= 1000000000)
        {
            timeout.tv_sec++;
            timeout.tv_nsec -= 1000000000;
        }
        if ((result = pthread_mutex_timedlock((pthread_mutex_t*) pMutex, &timeout)) == 0)
        {
            return SUCCESS;
        }
        else if (result == ETIMEDOUT)
        {
            return EXPIRED;
        }
    }
    return FAILURE;
}

/*! \brief Release (free) mutex object
 *
 *  \param pMutex Pointer to mutex object
 *  \return Standard result code
*/
RESULTCODE OsalFreeMutex(OSAL_MUTEX *pMutex)
{
    if (pthread_mutex_unlock((pthread_mutex_t*)pMutex) == 0)
    {
        return SUCCESS;
    }
    return FAILURE;
}

/*! \brief Create heap structure in selected memory region
 *
 *  \param pHeap [OUT] Pointer to variable which receive new heap object
 *  \param pHeapMemory Pointer to memory address when heap starts
 *  \param HeapSize Size of heap memory in bytes
 *  \param CacheMgmtNeeded Do we need to align blocks on cache line size?
 *  \return Standard result code
*/
RESULTCODE OsalCreateHeap(OSAL_HEAP *pHeap, OSAL_PADDRESS pHeapMemory,
                          OSAL_SIZE HeapSize, BOOLEAN CacheMgmtNeeded)
{
    return SUCCESS;
}

/*! \brief Destroy heap structure in memory
 *
 *  \param pHeap Pointer to heap structure
 *  \return Standard result code
*/
RESULTCODE OsalDestroyHeap(OSAL_HEAP *pHeap)
{
    return SUCCESS;
}

/*! \brief Allocate block from heap
 *
 *  \param pHeap Pointer to heap structure
 *  \param ppBlock [OUT] Pointer to variable which receives allocated block address
 *  \param BlockSize Requested block size to allocate
 *  \return Standard result code
*/
RESULTCODE OsalAllocHeap(OSAL_HEAP *pHeap, OSAL_PADDRESS *ppBlock,
                         OSAL_SIZE BlockSize)
{
    *ppBlock = malloc((size_t)BlockSize);
    if (*ppBlock == NULL)
    {
        fprintf(stderr, "ERROR: Out of heap memory\n");
        return FAILURE;
    }
    return SUCCESS;
}

/*! \brief Release (free) heap memory block
 *
 *  \param pHeap Pointer to heap structure
 *  \param pBlock Pointer to starting memory address to release
 *  \return Standard result code
*/
RESULTCODE OsalFreeHeap(OSAL_HEAP *pHeap, OSAL_PADDRESS pBlock)
{
    if (pBlock)
    {
        free(pBlock);
    }
    return SUCCESS;
}

/*! \brief Create memory array (fast partition) structure
 *
 *  \param pMemArray [OUT] Pointer to memory array structure
 *  \param pMemArrayMemory Pointer to starting memory address of structure
 *  \param totalSize Total size, must be multiply of BlockSize
 *  \param blockSize Size of array element
 *  \return Standard result code
*/
RESULTCODE OsalCreateMemArray(OSAL_MEMARRAY *pMemArray,
                              OSAL_PADDRESS pMemArrayMemory,
                              OSAL_SIZE totalSize, OSAL_SIZE blockSize)
{
    int numBlocks = totalSize / blockSize;
    OSAL_PADDRESS *ptr;
    U32 i;

    // Can't be less than pointer size
    if (blockSize < sizeof(OSAL_PADDRESS))
    {
        return FAILURE;
    }

    // Can't be less than one block
    if (totalSize < sizeof(OSAL_PADDRESS))
    {
        return FAILURE;
    }

    if (pMemArrayMemory == NULL)
    {
        if (OsalAllocHeap(&OsalGlobalHeap, &pMemArrayMemory,
                               totalSize) != SUCCESS)
        {
            return FAILURE;
        }
        pMemArray->flags = MA_AUTOALLOC;
    }
    else
        pMemArray->flags = 0;

    pthread_mutex_init(&pMemArray->lock, NULL);
    pMemArray->ppFreeBlock = (OSAL_PADDRESS*)pMemArrayMemory;
    pMemArray->storage = pMemArrayMemory;
    pMemArray->endOfStorage = ((U32*)pMemArrayMemory) + numBlocks * blockSize / sizeof(U32);
    pMemArray->blockSize = blockSize;
    pMemArray->blockCount = totalSize / blockSize;

    // Initialize single-linked list of free blocks;
    ptr = (OSAL_PADDRESS*)pMemArrayMemory;
    for (i = 0; i < pMemArray->blockCount; i++)
    {
        if (i == pMemArray->blockCount - 1)
        {
            *ptr = NULL;      // End of list
        }
        else
        {   // Points to the next block
            *ptr = (OSAL_PADDRESS*)(((U8*)ptr) + blockSize);
            ptr += blockSize / sizeof(U32);
        }
    }
    return SUCCESS;
}

/** Frees up resources used by MemArray
 *
 * @param pMemArray
 * @return Standard result code
 */
RESULTCODE OsalDestroyMemArray(OSAL_MEMARRAY *pMemArray)
{
    pthread_mutex_destroy(&pMemArray->lock);
    if (pMemArray->flags & MA_AUTOALLOC)
    {
        OsalFreeHeap(&OsalGlobalHeap, pMemArray->storage);
        pMemArray->storage = NULL;
    }
    return SUCCESS;
}

/*! \brief Allocate element within memory array
 *
 *  \param pMemArray Pointer to memory array structure
 *  \param ppBlock [OUT] Pointer to variable which receives address of allocated block
 *  \return Standard result code
*/
RESULTCODE OsalAllocMemArray(OSAL_MEMARRAY *pMemArray, OSAL_PADDRESS *ppBlock)
{
    pthread_mutex_lock(&pMemArray->lock);
    if (pMemArray->ppFreeBlock == NULL)
    {
        pthread_mutex_unlock(&pMemArray->lock);
        return FAILURE;
    }

    // FIXME: Remove after debugging
    if (((OSAL_PADDRESS) pMemArray->ppFreeBlock < pMemArray->storage) ||
        ((OSAL_PADDRESS) pMemArray->ppFreeBlock >= pMemArray->endOfStorage))
    {
#ifdef DEBUG_MEMPART
        fprintf(stderr,
                C4KFW "ERROR: Corrupted MemArray;Arr=%p,Stor=%p,Free=%p\n",
                pMemArray, pMemArray->storage, pMemArray->ppFreeBlock);
#endif
        pthread_mutex_unlock(&pMemArray->lock);
        return FAILURE;
    }

    // FIXME: Remove after debugging
    if ((*pMemArray->ppFreeBlock != NULL) &&
        (((*pMemArray->ppFreeBlock) < pMemArray->storage) ||
        ((*pMemArray->ppFreeBlock) >= pMemArray->endOfStorage)))
    {
#ifdef DEBUG_MEMPART
        fprintf(stderr,
                C4KFW "ERROR: Corrupted MemArray;Arr=%p,Stor=%p,Free=%p,Curr=%p\n",
                pMemArray, pMemArray->storage, pMemArray->ppFreeBlock,
                *pMemArray->ppFreeBlock);
#endif
	// Why return FAILURE now ?
//        pthread_mutex_unlock(&pMemArray->lock);
//        return FAILURE;
    }

    *ppBlock = (OSAL_PADDRESS) pMemArray->ppFreeBlock;
    pMemArray->ppFreeBlock = (OSAL_PADDRESS*) (*pMemArray->ppFreeBlock);

#ifdef DEBUG_MEMPART
    // FIXME: Debug
    printf(C4KFW "Block allocd [%p,%p]\n", pMemArray, *ppBlock);
#endif

     pthread_mutex_unlock(&pMemArray->lock);

    return SUCCESS;
}

/*! \brief Return element of memory array to the pool of free blocks
 *
 *  \param pMemArray Pointer to memory array structure
 *  \param pBlock Pointer to allocated block
 *  \return Standard result code
*/
RESULTCODE OsalFreeMemArray(OSAL_MEMARRAY *pMemArray, OSAL_PADDRESS pBlock)
{
    if ((pBlock < pMemArray->storage) || (pBlock >= pMemArray->endOfStorage))
    {
        // Trying to free foreign block or local variable
        // Local variable - OK
        // Foreign block - bad
        // TODO: Distinguish

        // FIXME: Remove after debugging
#ifdef DEBUG_MEMPART
        printf(C4KFW "WARNING: Trying to free foreign block;Arr=%p,Blk=%p\n",
               pMemArray, pBlock);
#endif

        return FAILURE;
    }

    pthread_mutex_lock(&pMemArray->lock);
    if (((OSAL_PADDRESS) pMemArray->ppFreeBlock) == pBlock)
    {
        // Simple protection against freeing of already freed block
        pthread_mutex_unlock(&pMemArray->lock);
        return SUCCESS;
    }

    // FIXME: Remove after debugging
    if ((pMemArray->ppFreeBlock != NULL)
        && (((OSAL_PADDRESS) pMemArray->ppFreeBlock < pMemArray->storage)
        || ((OSAL_PADDRESS) pMemArray->ppFreeBlock >= pMemArray->endOfStorage)))
    {
#ifdef DEBUG_MEMPART
        fprintf(stderr,
                C4KFW "ERROR: Corrupted MemArray;Arr=%p,Stor=%p,Free=%p\n",
                pMemArray, pMemArray->storage, pMemArray->ppFreeBlock);
#endif
        pthread_mutex_unlock(&pMemArray->lock);
        return FAILURE;
    }

    // FIXME: Remove after debugging
    if ((pBlock < pMemArray->storage) ||
        (pBlock >= pMemArray->endOfStorage))
    {
#if DEBUG_MEMPART
        fprintf(stderr,
                C4KFW "ERROR: Invalid block;Arr=%p,Blk=%p\n",
                pMemArray, pBlock);
#endif
        pthread_mutex_unlock(&pMemArray->lock);
        return FAILURE;
    }

    *((OSAL_PADDRESS*)pBlock) = pMemArray->ppFreeBlock;
    pMemArray->ppFreeBlock = (OSAL_PADDRESS*) pBlock;

#ifdef DEBUG_MEMPART
    // FIXME: Debug
    printf(C4KFW "Block freed [%p,%p]\n", pMemArray, pBlock);
#endif

    pthread_mutex_unlock(&pMemArray->lock);
    return SUCCESS;
}

#ifdef OSAL_LINUX_SIMPLEQ
/*! \brief Create queue control structure in memory
 *
 *  \param pQueue [OUT] Pointer to queue structure
 *  \param pQueueMemory Where to allocate
 *  \param queueSize Size of structure in bytes. Will be aligned up to sizeof(void*)
 *  \return Standard result code
*/
RESULTCODE OsalCreateQueue(OSAL_QUEUE *pQueue, OSAL_PADDRESS pQueueMemory,
                           OSAL_SIZE queueSize)
{
    if (pQueueMemory == NULL)
    {
        pQueueMemory = malloc(queueSize);
        if (pQueueMemory == NULL)
        {
            return FAILURE;
        }
        pQueue->pQueueStorage = pQueueMemory;
    }
    else
        pQueue->pQueueStorage = NULL;
    SIMPLEQ_INIT(&pQueue->head);
    return SUCCESS;
}

/*! \brief Destroy queue control structure in memory
 *
 *  \param pQueue Pointer to queue structure to destroy
 *  \return Standard result code
*/
RESULTCODE OsalDestroyQueue(OSAL_QUEUE *pQueue)
{
    OsalPurgeQueue(pQueue);
    if (pQueue->pQueueStorage)
    {
        free(pQueue->pQueueStorage);
        pQueue->pQueueStorage = NULL;
    }
    return SUCCESS;
}

/*! \brief Enqueue element
 *
 *  \param pQueue Pointer to queue structure
 *  \param pQueueElement Element address to enqueue
 *  \return Standard result code
*/
RESULTCODE OsalEnqueue(OSAL_QUEUE *pQueue, OSAL_PADDRESS pQueueElement)
{
    SIMPLEQ_INSERT_TAIL(&pQueue->head, pQueueElement, entry);
    return SUCCESS;
}

/*! \brief Dequeue next available element
 *
 *  \param pQueue Pointer to queue structure
 *  \param [OUT] pQueueElement Pointer to variable receiving address (void*),
 *         NULL if no elements
 *  \return Standard result code
*/
RESULTCODE OsalDequeue(OSAL_QUEUE *pQueue, OSAL_PADDRESS *ppQueueElement)
{
    if (SIMPLEQ_FIRST(&pQueue->head) != NULL)
    {
        SIMPLEQ_REMOVE_HEAD(&pQueue->head, entry);
        return SUCCESS;
    }
    return FAILURE;
}

/*! \brief Remove all waiting elements from queue
 *
 *  \param pQueue Pointer to queue structure
 *  \return Standard result code
*/
RESULTCODE OsalPurgeQueue(OSAL_QUEUE *pQueue)
{
    while (!SIMPLEQ_EMPTY(&pQueue->head))
    {
        SIMPLEQ_REMOVE_HEAD(&pQueue->head, entry);
    }
    return SUCCESS;
}

#else /* OSAL_LINUX_SIMPLEQ */

/*! \brief Create queue control structure in memory
 *
 *  \param pQueue [OUT] Pointer to queue structure
 *  \param pQueueMemory Where to allocate
 *  \param QueueSize Size of structure in bytes. Will be aligned up to sizeof(void*)
 *  \return Standard result code
*/
RESULTCODE OsalCreateQueue(OSAL_QUEUE *pQueue, OSAL_PADDRESS pQueueMemory,
                           OSAL_SIZE QueueSize)
{
    pQueue->storage = pQueueMemory;
    pQueue->get = 0;
    pQueue->put = 0;
    pQueue->size = QueueSize / sizeof(OSAL_PADDRESS);
    pthread_mutex_init(&pQueue->lock, NULL);
    return SUCCESS;
}

/*! \brief Destroy queue control structure in memory
 *
 *  \param pQueue Pointer to queue structure to destroy
 *  \return Standard result code
*/
RESULTCODE OsalDestroyQueue(OSAL_QUEUE *pQueue)
{
    pthread_mutex_destroy(&pQueue->lock);
    return SUCCESS;
}

/*! \brief Enqueue element
 *
 *  \param pQueue Pointer to queue structure
 *  \param pQueueElement Element address to enqueue
 *  \return Standard result code
*/
RESULTCODE OsalEnqueue(OSAL_QUEUE *pQueue, OSAL_PADDRESS pQueueElement)
{
    U32 put;
    U32 rc = FAILURE;

    // Not yet initialized
    if (pQueue->storage == NULL)
        return rc;

    // Waits for ownership of the specified critical section object.
    // The function returns when the calling thread is granted ownership
    // Hangs when was not initialized
    pthread_mutex_lock(&pQueue->lock);
    put = pQueue->put;

    if (++put >= pQueue->size)
    {
        put = 0;
    }

    if (put != pQueue->get)
    {
        rc = SUCCESS;
        pQueue->storage[pQueue->put] = pQueueElement;
        pQueue->put = put;
    }
    pthread_mutex_unlock(&pQueue->lock);
    return rc;
}

/*! \brief Dequeue next available element
 *
 *  \param pQueue Pointer to queue structure
 *  \param pQueueElement [OUT] Pointer to variable receiving address (void*),
 *         NULL if no elements
 *  \return Standard result code
*/
RESULTCODE OsalDequeue(OSAL_QUEUE *pQueue, OSAL_PADDRESS *ppQueueElement)
{
    U32 retVal = SUCCESS;
    U32 getIdx;

    // Wait for ownership of the specified critical section object.
    // The function returns when the calling thread is granted ownership
    // Hangs when was not initialized
    pthread_mutex_lock(&pQueue->lock);
    getIdx = pQueue->get;
    if (pQueue->put != getIdx)
    {
        *ppQueueElement = pQueue->storage[getIdx];
        if (++getIdx == pQueue->size)
        {
            getIdx = 0;
        }
        pQueue->get = getIdx;
    }
    else
    {
        retVal = FAILURE;
    }
    pthread_mutex_unlock(&pQueue->lock);
    return retVal;
}

/*! \brief Remove all waiting elements from queue
 *
 *  \param pQueue Pointer to queue structure
 *  \return Standard result code
*/
RESULTCODE OsalPurgeQueue(OSAL_QUEUE *pQueue)
{
    pQueue->get = pQueue->put;
    return SUCCESS;
}

/*! \brief Get number of elements waiting to be dequeued
 *
 *  \param pQueue Pointer to queue structure
 *  \return Number of elements waiting
*/
RETURNVALUE OsalGetElementCount(OSAL_QUEUE *pQueue)
{
    U32 res;
    if (pQueue->get <= pQueue->put)
    {
        res = pQueue->put - pQueue->get;
        return res;
    }
    res = pQueue->size - pQueue->get + pQueue->put;
    return res;
}

/*! \brief Get maximum number of elements can be enqueued
 *
 *  \param pQueue Pointer to queue structure
 *  \return Number of elements waiting
*/
RETURNVALUE OsalGetQueueSize(OSAL_QUEUE *pQueue)
{
    return pQueue->size;
}

#endif /* OSAL_LINUX_SIMPLEQ */

/*! \brief Create ring buffer control structure in memory
 *
 *  \param pRingBuf Pointer to queue structure
 *  \param pQueueMemory Where to allocate
 *  \param queueSize Size of structure in bytes. Will be aligned up to sizeof(void*)
 *  \return Standard result code
*/
RESULTCODE OsalCreateRing(OSAL_RINGBUF *pRingBuf, OSAL_PADDRESS pRingBufMemory,
                          OSAL_SIZE ringBufSize)
{
    ringBufSize = RoundUp(ringBufSize, sizeof(PTR));
    // TODO: Alloc memory if NULL
    if (pRingBufMemory == NULL)
    {
        return FAILURE;
    }

    pRingBuf->get = 0;
    pRingBuf->put = 0;
    pRingBuf->size = ringBufSize / sizeof(PTR);    // In elements
    pRingBuf->storage = pRingBufMemory;

    return SUCCESS;
}

/*! \brief Destroy ring buffer control structure in memory
 *
 *  \param pRingBuf Pointer to queue structure to destroy
 *  \return Standard result code
*/
RESULTCODE OsalDestroyRing(OSAL_RINGBUF *pRingBuf)
{
    if (pRingBuf == NULL)
    {
        return FAILURE;
    }

    pRingBuf->get = 0;
    pRingBuf->put = 0;

    return SUCCESS;
}

/*! \brief Put element into ring buffer
 *
 *  \param pRingBuf Pointer to queue structure
 *  \param pQueueElement Element address to enqueue
 *  \return Standard result code
*/
RESULTCODE OsalPutRing(OSAL_RINGBUF *pRingBuf, OSAL_PADDRESS pRingBufElement)
{
    pRingBuf->storage[pRingBuf->put++] = pRingBufElement;
    if (pRingBuf->put >= pRingBuf->size)
    {
        pRingBuf->put = 0;
    }

    return SUCCESS;
}

/*! \brief Get next available element
 *
 *  \param pRingBuf Pointer to queue structure
 *  \param pQueueElement [OUT] Pointer to variable receiving address (void*),
 *         NULL if no elements
 *  \return Standard result code
*/
RESULTCODE OsalGetRing(OSAL_RINGBUF *pRingBuf, OSAL_PADDRESS *ppRingBufElement)
{
    if (pRingBuf->put == pRingBuf->get)
    {
        return FAILURE;
    }

    *ppRingBufElement = pRingBuf->storage[pRingBuf->get++];
    if (pRingBuf->get >= pRingBuf->size)
    {
        pRingBuf->get = 0;
    }

    return SUCCESS;
}

/*! \brief Remove all waiting elements from ring buffer
 *
 *  \param pRingBuf Pointer to queue structure
 *  \return Standard result code
*/
RESULTCODE OsalFlushRing(OSAL_RINGBUF *pRingBuf)
{
    pRingBuf->put = pRingBuf->get;

    return SUCCESS;
}

static void TimerHandler(sigval_t val)
{
    if (val.sival_ptr)
    {
        OsalRaiseSema((OSAL_SEMAPHORE *) val.sival_ptr);
    }
}

/*! \brief Create timer
 *
 *  \param pTimer [OUT] Pointer to allocated timer
 *  \return Standard result code
*/
RESULTCODE OsalCreateTimer(OSAL_TIMER *pTimer)
{
    // Actual create is in OsalStartTimer

    // Set flag indicating timer has "created"
    *pTimer = (void*)0xDEADC0DE;
    return SUCCESS;
}

/*! \brief Destroy timer
 *
 *  \param pTimer Pointer to timer object
 *  \return Standard result code
*/
RESULTCODE OsalDestroyTimer(OSAL_TIMER *pTimer)
{
    if (timer_delete((timer_t)*pTimer) != 0)
        return FAILURE;
    *pTimer = 0;
    return SUCCESS;
}

/*! @brief Create timer
 *
 *  @param pTimer Pointer to allocated timer
 *  @param DelayMicroSeconds Timeout after which timer signals
 *  @param pSemaphore Pointer to semaphore object to set in signalling state
 *  @param autoRestart
 *  @return Standard result code
*/
RESULTCODE OsalStartTimer(OSAL_TIMER *pTimer, OSAL_DELAYU delayMicroSeconds,
                          OSAL_SEMAPHORE *pSemaphore, BOOLEAN autoRestart)
{
    struct sigevent evp;
    struct itimerspec value;

    // Check if timer already "created"
    if (*pTimer == (void*)0xDEADC0DE)
    {
        bzero(&evp, sizeof(evp));
        evp.sigev_notify = SIGEV_THREAD;
        evp.sigev_notify_function = TimerHandler;
        evp.sigev_value.sival_ptr = (void*)pSemaphore;
        // TODO: Is local variable allowed?
        if (timer_create(CLOCK_REALTIME, &evp, (timer_t*)pTimer) != 0)
        {
            return FAILURE;
        }
    }

    // Periodic timer
    value.it_interval.tv_sec = delayMicroSeconds / 1000000;
    value.it_interval.tv_nsec = (delayMicroSeconds % 1000000) * 1000;
    value.it_value = value.it_interval;

    // Relative time
    if (timer_settime(*pTimer, 0, &value, NULL) != 0)
    {
        OsalStopTimer(pTimer);
        return FAILURE;
    }
    return SUCCESS;
}

/*! \brief Stop timer
 *
 *  \param pTimer Pointer to timer object
 *  \return Standard result code
*/
RESULTCODE OsalStopTimer(OSAL_TIMER *pTimer)
{
    return OsalStartTimer(pTimer, 0, NULL, FALSE);
}

/*! \brief Create (open) message basket
 *         If msgBasketTotalSize & msgSize both specified then new
 *         message basket will be created, otherwise existing one will
 *         open
 *
 *  \param pMsgBasket Pointer to message basket (mail box)
 *  \param pMsgBasketName Pointer to NULL-terminated string
 *  \param msgBasketTotalSize Not used
 *  \param msgSize Not used
 *  \param pTaskSemaphore Not used
 *  \return Standard result code
*/
RESULTCODE OsalCreateMailBox(OSAL_MAILBOX *pMsgBasket,
                             OSAL_PADDRESS pMsgBasketName,
                             OSAL_SIZE msgBasketTotalSize,
                             OSAL_SIZE msgSize,
                             OSAL_SEMAPHORE *pTaskSemaphore,
                             BOOLEAN CacheManagementNeeded)
{
    mqd_t msgQueue;
    struct mq_attr msgAttr;

    bzero(&msgAttr, sizeof(&msgAttr));

    if (msgBasketTotalSize && msgSize)
    {
        //
        // Sizes specified - create new message queue
        //
        msgAttr.mq_flags = 0;
        msgAttr.mq_maxmsg = msgBasketTotalSize / msgSize;
        msgAttr.mq_msgsize = msgSize;
        msgAttr.mq_curmsgs = 0;

        msgQueue = mq_open((const char *)pMsgBasketName, O_RDWR | O_CREAT, 00666, &msgAttr);
    }
    else
    {
        //
        // Sizes missed - open existing message queue
        //

        // 1st step - check if queue really exist
        msgQueue = mq_open((const char *)pMsgBasketName, O_RDWR | O_CREAT | O_EXCL, 0666, NULL);
        if (msgQueue == (mqd_t)-1)
        {
            // Error indicated - queue already exist
            // Open existing queue
            msgQueue = mq_open((const char *)pMsgBasketName, O_RDWR);
        }
        else
        {
            // Queue does not exist - bailing out
            mq_close(msgQueue);
            mq_unlink((const char *) pMsgBasketName);
            msgQueue = (mqd_t) -1;          // Indicate failure
        }
    }

    if (msgQueue == (mqd_t) -1)
    {
        *pMsgBasket = 0;
        return FAILURE;
    }

    *pMsgBasket = msgQueue;
    return SUCCESS;
}

/*! \brief Destroy message basket
 *
 *  \param pMsgBasket Pointer to Message basket handle
 *  \param pMsgBasketMemory [Optional] Remove queue if not NULL
 *  \return Standard result code
*/
RESULTCODE OsalDestroyMailBox(OSAL_MAILBOX *pMsgBasket,
                              OSAL_PADDRESS pMsgBasketName)
{
    if (*pMsgBasket && (mq_close(*pMsgBasket) == 0))
    {
        // Just try to unlink in any case regardless of error
        if (pMsgBasketName)
        {
            mq_unlink((const char *) pMsgBasket);
        }
        return SUCCESS;
    }

    return FAILURE;
}

/*! \brief Put message into basket
 *
 *  \param pMsgBasket
 *  \param pMsg
 *  \param msgSize
 *  \return Standard result code
*/
RESULTCODE OsalSendMsg(OSAL_MAILBOX *pMsgBasket, PTR pMsg, OSAL_SIZE msgSize)
{
    // Priority is always 0
    return (mq_send(*pMsgBasket, pMsg, msgSize, 0) == 0) ? SUCCESS : FAILURE;
}

/*! \brief Get next message from basket
 *
 *  \param pMsgBasket Pointer to message basket (mail box) handler
 *  \param pMsg [OUT] Data buffer which receives available message
 *  \param pMsgSize [IN/OUT] Size of receiving buffer / Number of bytes copied
 *  \return Standard result code
*/
RESULTCODE OsalGetMsg(OSAL_MAILBOX *pMsgBasket, PTR pMsg, OSAL_SIZE *pMsgSize)
{
    ssize_t msgSize;

    // Ignore priority
    // Blocking call
    if ((msgSize = mq_receive(*pMsgBasket, (char *) pMsg, *pMsgSize, NULL)) != -1)
    {
        *pMsgSize = msgSize;
        return SUCCESS;
    }
    *pMsgSize = 0;

    return FAILURE;
}

/*! \brief Get current time
 *
 *  \param pCurrentTime [OUT] In microseconds
 *  \return Standard result code
*/
RESULTCODE OsalGetTime(OSAL_DELAYU *pCurrentTime)
{
    struct timespec tp;

    if (clock_gettime(CLOCK_REALTIME, &tp) < 0)
    {
        return FAILURE;
    }
    *pCurrentTime = (OSAL_DELAYU) (tp.tv_sec * 1000000 + tp.tv_nsec / 1000);
    return SUCCESS;
}

/*! \brief Get current time in milliseconds
 *
 *  \param pCurrentTime [OUT] In milliseconds
 *  \return Standard result code
*/
RESULTCODE OsalGetTimeMs(OSAL_DELAYM *pCurrentTime)
{
    struct timespec tp;

    if (clock_gettime(CLOCK_REALTIME, &tp) < 0)
    {
        return FAILURE;
    }
    *pCurrentTime = (OSAL_DELAYM) (tp.tv_sec * 1000 + tp.tv_nsec / 1000000);
    return SUCCESS;
}

/*! \brief Get CPU ID
 *
 *  \return CPU ID
*/
U32 OsalGetCpuID(void)
{
#if __GLIBC__ >= 2 && __GLIBC_MINOR__ >= 8
    int cpuID = sched_getcpu();
#else
    int cpuID = 0;
#endif /* __GLIBC__ */

    // Error has been occured, return default
    if (cpuID < 0)
    {
        cpuID = 0;
    }

    return (U32) cpuID;
}

#endif /*USE_LINUX*/

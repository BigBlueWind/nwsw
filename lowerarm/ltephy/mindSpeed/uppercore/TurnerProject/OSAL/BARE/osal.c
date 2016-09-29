/** @file osal.c
 *
 * @brief Implementation of OSAL for bare metal environment
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_BARE

#include "osal.h"
#include "sysctrl.h"
#include "intc.h"
#include "timers.h"
#include <stdlib.h>

#ifdef __CC_ARM
extern void* Image$$ARM_LIB_HEAP$$Base;
extern unsigned int Image$$ARM_LIB_HEAP$$Length;
#endif /* __CC_ARM */

// heap size for OSAL and applications on top of it
#define OSAL_HEAP_SIZE          (16*1024*1024)

// Semaphores
#define OSAL_SIGNALLED          0x00000001

// Mutexes
#define OSAL_LOCKED             0x00000001

OSAL_HEAP OsalGlobalHeap;

/*! \brief Initialize all OS-dependent structures
 *
 *  \return Standard result code
*/
RESULTCODE OsalInit(U32 cpuid)
{
    if (cpuid == UARM)
    {
#ifdef __CC_ARM
        OsalCreateHeap(&OsalGlobalHeap, Image$$ARM_LIB_HEAP$$Base,
                       Image$$ARM_LIB_HEAP$$Length, FALSE);
#endif /* __CC_ARM */

        IntcEnableLowArm();
    }
    // This should be done at the end of initialization
    __enable_irq();

    return SUCCESS;
}

/*! \brief Create task
 *
 *  \param pTask [OUT] Pointer to task structure
 *  \param pEntryPoint Message source/destination
 *  \param pTaskParam Parameter for task startup routine
 *  \param TaskName (Optional) Task name - can be NULL
 *  \param TaskPriority Task priority
 *  \param pStackBase Not used in Win32
 *  \param StackSize Size of stack in bytes
 *  \return Standard result code
*/
RESULTCODE OsalCreateTask(OSAL_TASK *pTask, OSAL_PADDRESS pEntryPoint,
                          PTR pTaskParam, const char *TaskName,
                          OSAL_PRIORITY TaskPriority,
                          OSAL_PADDRESS pStackBase, OSAL_SIZE StackSize)
{
    return SUCCESS;
}

/*! \brief Destroy task
 *
 *  \param pTask Pointer to task structure
 *  \return Standard result code
*/
RESULTCODE OsalDestroyTask(OSAL_TASK *pTask)
{
    return SUCCESS;
}

/*! \brief Activate (resume) task
 *
 *  \param pTask Pointer to task structure
 *  \return Standard result code
*/
RESULTCODE OsalActivateTask(OSAL_TASK *pTask)
{
    return SUCCESS;
}

/*! \brief Stop task execution (pause)
 *
 *  \param pTask Pointer to task structure
 *  \return Standard result code
*/
RESULTCODE OsalDeactivateTask(OSAL_TASK *pTask)
{
    return SUCCESS;
}

/*! \brief Change task priority
 *
 *  \param pTask Pointer to task structure
 *  \param TaskPriority New task's priority
 *  \return Standard result code
*/
RESULTCODE OsalSetTaskPriority(OSAL_TASK *pTask, OSAL_PRIORITY TaskPriority)
{
    return SUCCESS;
}

/*! \brief Query task priority
 *
 *  \param pTask Pointer to task structure
 *  \param [OUT] TaskPriority Return value
 *  \return Standard result code
*/
RESULTCODE OsalGetTaskPriority(OSAL_TASK *pTask, OSAL_PRIORITY *pTaskPriority)
{
    return SUCCESS;
}

/*! \brief Delay current task execution for period of time
 *
 *  \param DelayMicroSeconds Time in microseconds
 *  \return Standard result code
*/
RESULTCODE OsalWaitDelay(OSAL_DELAYU delayMicroSeconds)
{
    OSAL_DELAYU expected = CurrentTimeUs() + delayMicroSeconds;

    if (expected > CurrentTimeUs())
    {
        while (CurrentTimeUs() < expected)
        {
            __nop();
        }
    }
    else if (expected < CurrentTimeUs())
    {
        while (expected < CurrentTimeUs())
        {
            __nop();
        }
        while (CurrentTimeUs() < expected)
        {
            __nop();
        }
    }
    return SUCCESS;
}

/*! \brief Places current task to the end of ready queue
 *
 *  \return Standard result code
*/
RESULTCODE OsalYield(void)
{
    return SUCCESS;
}

/*! \brief Create semaphore (event) object
 *
 *  \param pSemaphore [OUT] Pointer to newly created semaphore
 *  \return Standard result code
*/
RESULTCODE OsalCreateSema(OSAL_SEMAPHORE *pSemaphore)
{
    *pSemaphore = 0;
    return SUCCESS;
}

/*! \brief Destroy semaphore (event) object
 *
 *  \param pSemaphore Pointer to semaphore object
 *  \return Standard result code
*/
RESULTCODE OsalDestroySema(OSAL_SEMAPHORE *pSemaphore)
{
    // Do nothing for now
    return SUCCESS;
}

/*! \brief Put semaphore (event) object into signaling state
 *
 *  \param pSemaphore Pointer to semaphore object
 *  \return Standard result code
*/
RESULTCODE OsalRaiseSema(OSAL_SEMAPHORE *pSemaphore)
{
    *pSemaphore |= OSAL_SIGNALLED;
    return SUCCESS;
}

/*! \brief Put semaphore (event) object into signaling state
 *
 *  \param pSemaphore Pointer to semaphore object
 *  \return Standard result code
*/
RESULTCODE OsalRaiseSemaIsr(OSAL_SEMAPHORE *pSemaphore)
{
    *pSemaphore |= OSAL_SIGNALLED;
    return SUCCESS;
}

/*! \brief Reset semaphore (event) object into inactive state
 *
 *  \param pSemaphore Pointer to semaphore object
 *  \return Standard result code
*/
RESULTCODE OsalClearSema(OSAL_SEMAPHORE *pSemaphore)
{
    *pSemaphore &= ~OSAL_SIGNALLED;
    return SUCCESS;
}

/*! \brief Wait for single semaphore (event) object to become active
 *
 *  \param pSemaphore Pointer to semaphore object
 *  \param TimeoutMicroSeconds Wait timeout in microseconds, 0 - infinite
 *  \return Standard result code
*/
RETURNVALUE OsalWaitForSema(OSAL_SEMAPHORE *pSemaphore,
                            OSAL_DELAYU TimeoutMicroSeconds)
{
    if (*pSemaphore & OSAL_SIGNALLED)
    {
        OsalClearSema(pSemaphore);
        return SUCCESS;
    }
    return EXPIRED;
}

/*! \brief Check if semaphore (event) objects is in signalling state
 *
 *  \param pSemaphoreList Pointer to NULL-terminated array of semaphore objects
 *  \return Standard result code
*/
RETURNVALUE OsalCheckSema(OSAL_SEMAPHORE *pSemaphore)
{
    if (*pSemaphore & OSAL_SIGNALLED)
    {
        return SUCCESS;
    }
    return FAILURE;
}

/*! \brief Create mutex object
 *
 *  \param pMutex [OUT] Pointer to variable which receive new mutex handle
 *  \return Standard result code
*/
RESULTCODE OsalCreateMutex(OSAL_MUTEX *pMutex)
{
    *pMutex = 0;
    return SUCCESS;
}

/*! \brief Create mutex object
 *
 *  \param pMutex Pointer to mutex object to destroy
 *  \return Standard result code
*/
RESULTCODE OsalDestroyMutex(OSAL_MUTEX *pMutex)
{
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
    while (*pMutex & OSAL_LOCKED)
    {
        __nop();
        // TODO: Read time here
        //       Now it is a dead loop
    }
    *pMutex |= OSAL_LOCKED;
    return EXPIRED;
}

/*! \brief Release (free) mutex object
 *
 *  \param pMutex Pointer to mutex object
 *  \return Standard result code
*/
RESULTCODE OsalFreeMutex(OSAL_MUTEX *pMutex)
{
    *pMutex &= ~OSAL_LOCKED;
    return SUCCESS;
}

/*! \brief Create heap structure in selected memory region
 *
 *  \param pHeap [OUT] Pointer to variable which receive new heap object
 *  \param pHeapMemory Pointer to memory address when heap starts
 *  \param HeapSize Size of heap memory in bytes
 *  \param CacheManagementNeeded Do we need to align blocks on cache line size?
 *         Not used in Win32
 *  \return Standard result code
*/
RESULTCODE OsalCreateHeap(OSAL_HEAP *pHeap, OSAL_PADDRESS pHeapMemory,
                          OSAL_SIZE heapSize, BOOLEAN cacheManagementNeeded)
{
    // Single heap managed by ARM lib
    pHeap->base = pHeapMemory;
    pHeap->size = heapSize;

    return SUCCESS;
}

/*! \brief Destroy heap structure in memory
 *
 *  \param pHeap Pointer to heap structure
 *  \return Standard result code
*/
RESULTCODE OsalDestroyHeap(OSAL_HEAP *pHeap)
{
    // Single heap managed by ARM lib - nothing to do
    pHeap->base = NULL;
    pHeap->size = 0;

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
    // Ignore pHeap

    *ppBlock = malloc((size_t) BlockSize);
    if (*ppBlock == NULL)
    {
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
    // Ignore pHeap
    free(pBlock);
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
    }
    pMemArray->freeBlock = (OSAL_PADDRESS*)pMemArrayMemory;
    pMemArray->storage = pMemArrayMemory;
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
        else                  // Points to the next block
        {
            *ptr = (OSAL_PADDRESS*)(((U8*)ptr) + blockSize);
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
    if (pMemArray->freeBlock == NULL)
    {
        return FAILURE;
    }

    *ppBlock = (OSAL_PADDRESS)pMemArray->freeBlock;
    pMemArray->freeBlock = (OSAL_PADDRESS*) *(pMemArray->freeBlock);

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
    // TODO: Verify if this is valid block
    *((OSAL_PADDRESS*)pBlock) = *(pMemArray->freeBlock);
    pMemArray->freeBlock = (OSAL_PADDRESS*) pBlock;

    return SUCCESS;
}

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
        // Allocate memory block in default heap
        if (OsalAllocHeap(NULL, &pQueueMemory, queueSize) != SUCCESS)
        {
            return FAILURE;
        }
        pQueue->heap = &OsalGlobalHeap;
    }
    else
    {
        pQueue->heap = NULL;
    }
    pQueue->get = 0;
    pQueue->put = 0;
    pQueue->size = queueSize / sizeof(PTR);    // In elements
    pQueue->elemSize = sizeof(PTR);
    pQueue->storage = pQueueMemory;

    return SUCCESS;
}

/*! \brief Create queue control structure in memory
 *
 *  \param pQueue [OUT] Pointer to queue structure
 *  \param pQueueMemory Where to allocate
 *  \param queueSize Size of structure in bytes. Will be aligned up to sizeof(void*)
 *  \return Standard result code
*/
RESULTCODE OsalCreateQueueEx(OSAL_QUEUE *pQueue, OSAL_PADDRESS pQueueMemory,
                             OSAL_SIZE queueSize, OSAL_SIZE elemSize)
{
    return OsalCreateQueue(pQueue, pQueueMemory,
                           queueSize / elemSize * sizeof(PTR));
}

/*! \brief Destroy queue control structure in memory
 *
 *  \param pQueue Pointer to queue structure to destroy
 *  \return Standard result code
*/
RESULTCODE OsalDestroyQueue(OSAL_QUEUE *pQueue)
{
    if (pQueue == NULL)
    {
        return FAILURE;
    }
    if (pQueue->storage && pQueue->heap)
    {
        OsalFreeHeap(pQueue->heap, pQueue->storage);
        pQueue->storage = NULL;
        pQueue->heap = NULL;
    }
    pQueue->get = 0;
    pQueue->put = 0;
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
    pQueue->storage[pQueue->put++] = pQueueElement;
    if (pQueue->put >= pQueue->size)
    {
        pQueue->put = 0;
    }

    return SUCCESS;
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
    if (pQueue->put == pQueue->get)
    {
        return FAILURE;
    }

    *ppQueueElement = pQueue->storage[pQueue->get++];
    if (pQueue->get >= pQueue->size)
    {
        pQueue->get = 0;
    }

	return SUCCESS;
}

/*! \brief Remove all waiting elements from queue
 *
 *  \param pQueue Pointer to queue structure
 *  \return Standard result code
*/
RESULTCODE OsalPurgeQueue(OSAL_QUEUE *pQueue)
{
    pQueue->put = pQueue->get;

    return SUCCESS;
}

/*! \brief Get number of elements waiting to be dequeued
 *
 *  \param pQueue Pointer to queue structure
 *  \return Number of elements waiting
*/
RETURNVALUE OsalGetElementCount(OSAL_QUEUE *pQueue)
{
    U32 get = pQueue->get;
    U32 put = pQueue->put;

    if (get < put)
    {
        return put - get;
    }
    else if (get > put)
    {
        return pQueue->size - get + put;
    }
    return 0;
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

/*! \brief Create timer
 *
 *  \param pTimer [OUT] Pointer to allocated timer
 *  \return Standard result code
*/
RESULTCODE OsalCreateTimer(OSAL_TIMER *pTimer)
{
    return SUCCESS;
}

/*! \brief Destroy timer
 *
 *  \param pTimer Pointer to timer object
 *  \return Standard result code
*/
RESULTCODE OsalDestroyTimer(OSAL_TIMER *pTimer)
{
    return SUCCESS;
}

/*! \brief Create timer
 *
 *  \param pTimer Pointer to allocated timer
 *  \param DelayMicroSeconds Timeout after which timer signals
 *  \param pSemaphore Pointer to semaphore object to set in signalling state
 *  \return Standard result code
*/
RESULTCODE OsalStartTimer(OSAL_TIMER *pTimer, OSAL_DELAYU delayMicroSeconds,
                          OSAL_SEMAPHORE *pSemaphore, BOOLEAN autoRestart)
{
    return SUCCESS;
}

/*! \brief Stop timer
 *
 *  \param pTimer Pointer to timer object
 *  \return Standard result code
*/
RESULTCODE OsalStopTimer(OSAL_TIMER *pTimer)
{
    return SUCCESS;
}

/*! \brief Get current time
 *
 *  \param pCurrentTime [OUT] In microseconds
 *  \return Standard result code
*/
RESULTCODE OsalGetCurrentTime(OSAL_DELAYU *pCurrentTime)
{
    *pCurrentTime = (OSAL_DELAYU) CurrentTimeUs();
    return SUCCESS;
}

/*! \brief Get current time in ms
 *
 *  \param pCurrentTime [OUT] In milliseconds
 *  \return Standard result code
*/
RESULTCODE OsalGetCurrentTimeMs(OSAL_DELAYM *pCurrentTime)
{
    *pCurrentTime = (OSAL_DELAYM) CurrentTimeMs;
    return SUCCESS;
}

/*! \brief Get CPU ID
 *
 *  \return CPU ID
*/
U32 OsalGetCpuID(void)
{
    // 0 == UARM, 1 == LARM
    return (U32) SysReadCpuID();
}

#endif /* USE_BARE */

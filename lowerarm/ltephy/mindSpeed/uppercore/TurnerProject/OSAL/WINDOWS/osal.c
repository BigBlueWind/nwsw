/** @file osal.c
 *
 * @brief Implementation of OSAL for Microsoft Windows
 * @author Mindspeed Technologies
 * @version $Revision: 1.36 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#if defined (USE_WINDOWS) || defined (WIN32)

#include "osal.h"
#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>

#define OSAL_SEMACOUNT          64

OSAL_HEAP OsalGlobalHeap;

/*! \brief Initialize all OS-dependent structures
 *
 *  \return Standard result code
*/
RESULTCODE OsalInit(U32 cpuid)
{
	OsalCreateHeap(&OsalGlobalHeap, NULL, 0, FALSE);
	return SUCCESS;
}

/*! \brief Create task
 *
 *  \param pTask [OUT] Pointer to task structure
 *  \param pEntryPoint Message source/destination
 *  \param pTaskParam Parameter for task startup routine
 *  \param TaskName [IN] Task name - not used
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
    HANDLE task;

    task = CreateThread(NULL, StackSize, (LPTHREAD_START_ROUTINE)pEntryPoint,
                        (LPVOID)pTaskParam, CREATE_SUSPENDED, NULL);
    if (task == NULL)
    {
        // TODO: Handle OS error codes
        return FAILURE;
    }

    SetThreadPriority((HANDLE)task, TaskPriority);

    *pTask = (OSAL_TASK) task;
    return SUCCESS;
}

/*! \brief Destroy task
 *
 *  \param pTask Pointer to task structure
 *  \return Standard result code
*/
RESULTCODE OsalDestroyTask(OSAL_TASK *pTask)
{
    TerminateThread((HANDLE)*pTask, SUCCESS);
    CloseHandle((HANDLE)*pTask);
    return SUCCESS;
}

RESULTCODE OsalWaitForTaskClose(OSAL_TASK *pTask, OSAL_DELAYU timeout, BOOLEAN forceClose)
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
    ResumeThread((HANDLE)*pTask);
    return SUCCESS;
}

/*! \brief Stop task execution (pause)
 *
 *  \param pTask Pointer to task structure
 *  \return Standard result code
*/
RESULTCODE OsalDeactivateTask(OSAL_TASK *pTask)
{
    SuspendThread((HANDLE)*pTask);
    return SUCCESS;
}

/*! \brief Change task priority
 *
 *  \param pTask Pointer to task structure
 *  \param taskPriority New task's priority
 *  \return Standard result code
*/
RESULTCODE OsalSetTaskPriority(OSAL_TASK *pTask, OSAL_PRIORITY taskPriority)
{
    SetThreadPriority((HANDLE)*pTask, taskPriority);
    return SUCCESS;
}

/*! \brief Query task priority
 *
 *  \param pTask Pointer to task structure
 *  \param pTaskPriority [OUT] Return value
 *  \return Standard result code
*/
RESULTCODE OsalGetTaskPriority(OSAL_TASK *pTask, OSAL_PRIORITY *pTaskPriority)
{
    *pTaskPriority = (OSAL_PRIORITY) GetThreadPriority((HANDLE)*pTask);
    return SUCCESS;
}

/*! \brief Delay current task execution for period of time
 *
 *  \param delayMicroSeconds Time in microseconds
 *  \return Standard result code
*/
RESULTCODE OsalWaitDelay(OSAL_DELAYU delayMicroSeconds)
{
    // Not possible to delay less than 1 ms
    if (delayMicroSeconds < 1000)
        delayMicroSeconds = 1000;
	if (SleepEx((U32)(delayMicroSeconds / 1000),TRUE) == 0)
	    return SUCCESS;
	return FAILURE;
}

/*! \brief Places current task to the end of ready queue
 *
 *  \return Standard result code
*/
RESULTCODE OsalYield(void)
{
    SleepEx(0, 0);
    return SUCCESS;
}

/*! \brief Create semaphore (event) object
 *
 *  \param [OUT] pSemaphore Pointer to newly created semaphore
 *  \return Standard result code
*/
RESULTCODE OsalCreateSema(OSAL_SEMAPHORE *pSemaphore)
{
    HANDLE evnt = CreateSemaphore(NULL, 0, OSAL_SEMACOUNT, NULL);
    if (evnt == NULL)
    {
        // TODO: Handle OS return code
        *pSemaphore = NULL;
        return FAILURE;
    }
    *pSemaphore = (OSAL_SEMAPHORE)evnt;
    return SUCCESS;
}

/*! \brief Destroy semaphore object
 *
 *  \param pSemaphore Pointer to semaphore object
 *  \return Standard result code
*/
RESULTCODE OsalDestroySema(OSAL_SEMAPHORE *pSemaphore)
{
    CloseHandle((HANDLE) *pSemaphore);
    return SUCCESS;
}

/*! \brief Put semaphore object into signaling state
 *
 *  \param pSemaphore Pointer to semaphore object
 *  \return Standard result code
*/
RESULTCODE OsalRaiseSema(OSAL_SEMAPHORE *pSemaphore)
{
    ReleaseSemaphore((HANDLE) *pSemaphore, 1, NULL);
    return SUCCESS;
}

/*! \brief Reset semaphore object into inactive state
 *
 *  \param pSemaphore Pointer to semaphore object
 *  \return Standard result code
*/
RESULTCODE OsalClearSema(OSAL_SEMAPHORE *pSemaphore)
{
    return FAILURE;
}

/*! \brief Wait for a semaphore object to become active
 *
 *  \param pSemaphore Pointer to semaphore object
 *  \param TimeoutMicroSeconds Wait timeout in microseconds, 0 - infinite
 *  \return Standard result code
*/
RESULTCODE OsalWaitForSema(OSAL_SEMAPHORE *pSemaphore,
                           OSAL_DELAYU timeoutMicroSeconds)
{
    DWORD timeout;
    DWORD result = WAIT_IO_COMPLETION;
    RESULTCODE ret = FAILURE;

    // Win32 uses milliseconds
    if (timeoutMicroSeconds == OSAL_WAIT_INFINITE)
    {
        timeout = INFINITE;
    }
    else
    {
        timeout = (U32)(timeoutMicroSeconds / 1000);
    }

    while (result == WAIT_IO_COMPLETION)
    {
        result = WaitForSingleObjectEx((HANDLE) *pSemaphore, timeout, TRUE);

        if (result == WAIT_OBJECT_0)
        {
            ret = SUCCESS;
            break;
        }
        else if (result == WAIT_ABANDONED)
        {
            break;
        }
        else if (result == WAIT_TIMEOUT)
        {
            ret = EXPIRED;
            break;
        }
    }
    return ret;
}

/*! \brief Check if semaphore (event) objects is in signalling state
 *
 *  \param pSemaphoreList Pointer to NULL-terminated array of semaphore objects
 *  \return Standard result code
*/
RETURNVALUE OsalCheckSema(OSAL_SEMAPHORE *pSemaphore)
{
    // TODO: Is this exist in Win32 ???
    return FAILURE;
}

/*! \brief Create mutex object
 *
 *  \param [OUT] pMutex Pointer to variable which receive new mutex handle
 *  \return Standard result code
*/
RESULTCODE OsalCreateMutex(OSAL_MUTEX *pMutex)
{
    // Caller thread is NOT an owner
    HANDLE mutex = CreateMutex(NULL, FALSE, NULL);
    if (mutex == NULL)
    {
        // TODO: Handle error code
        return FAILURE;
    }
    *pMutex = (OSAL_MUTEX) mutex;
    return SUCCESS;
}

/*! \brief Create mutex object
 *
 *  \param pMutex Pointer to mutex object to destroy
 *  \return Standard result code
*/
RESULTCODE OsalDestroyMutex(OSAL_MUTEX *pMutex)
{
    CloseHandle((HANDLE) *pMutex);
    return SUCCESS;
}

/*! \brief Take ownership on mutex object
 *
 *  \param pMutex Pointer to mutex object
 *  \param timeoutMicroSeconds Timeout in microseconds
 *  \return Standard result code
*/
RESULTCODE OsalCaptureMutex(OSAL_MUTEX *pMutex, OSAL_DELAYU timeoutMicroSeconds)
{
    DWORD result;
    DWORD timeout;

    // Win32 timers in milliseconds
    if (timeoutMicroSeconds == OSAL_WAIT_INFINITE)
        timeout = INFINITE;
    else
        timeout = (U32)(timeoutMicroSeconds / 1000);

    result = WaitForSingleObject((HANDLE) *pMutex, timeout);
    if (result == WAIT_OBJECT_0)
    {
        return SUCCESS;
    }
    else if (result == WAIT_ABANDONED)
    {
        return FAILURE;
    }
    return EXPIRED;
}

/*! \brief Release (free) mutex object
 *
 *  \param pMutex Pointer to mutex object
 *  \return Standard result code
*/
RESULTCODE OsalFreeMutex(OSAL_MUTEX *pMutex)
{
    ReleaseMutex((HANDLE) *pMutex);
    return SUCCESS;
}

/*! \brief Create heap structure in selected memory region
 *
 *  \param [OUT] pHeap Pointer to variable which receive new heap object
 *  \param pHeapMemory Pointer to memory address when heap starts
 *  \param HeapSize Size of heap memory in bytes
 *  \param CacheManagementNeeded Do we need to align blocks on cache line size?
 *         Not used in Win32
 *  \return Standard result code
*/
RESULTCODE OsalCreateHeap(OSAL_HEAP *pHeap, OSAL_PADDRESS pHeapMemory,
                          OSAL_SIZE HeapSize, BOOLEAN CacheManagementNeeded)
{
    *pHeap = NULL;
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
    void *address = malloc(BlockSize);
    if (address == NULL)
    {
        *ppBlock = NULL;
        return FAILURE;
    }
    *ppBlock = address;
    return SUCCESS;
}

/*! \brief Release (free) heap memory block
 *
 *  \param pHeap Pointer to heap structure
 *  \param pBlock Pointer to starting memory address to release
 *  \return Standard result code
*/
RESULTCODE OsalFreeHeap(OSAL_HEAP *pHeap, PTR pBlock)
{
    free(pBlock);
    return SUCCESS;
}

/*! \brief Create memory array (fast partition) structure
 *
 *  \param pMemArray [OUT] Pointer to memory array structure
 *  \param pMemArrayMemory Pointer to starting memory address of structure
 *  \param TotalSize Total size, must be multiple of BlockSize
 *  \param BlockSize Size of array element in bytes
 *  \return Standard result code
*/
RESULTCODE OsalCreateMemArray(OSAL_MEMARRAY *pMemArray, OSAL_PADDRESS pMemArrayMemory,
                              OSAL_SIZE TotalSize, OSAL_SIZE BlockSize)
{
    int numBlocks = (int)(TotalSize / BlockSize);
    int i;
    OSAL_PADDRESS *ptr;

    InitializeCriticalSection(&pMemArray->lock);

    EnterCriticalSection(&pMemArray->lock);
    pMemArray->freeBlock = pMemArrayMemory;
    pMemArray->storage = pMemArrayMemory;
    pMemArray->endOfStorage = ((U32*)pMemArrayMemory) + numBlocks * BlockSize / sizeof(U32);
    pMemArray->blockSize = BlockSize;
    pMemArray->blockCount = numBlocks;

    ptr = pMemArrayMemory;
    for (i = 0; i < numBlocks - 1; i++)
    {
        // 1st element used as pointer to the next block (chain of blocks)
		// We need to save a pointer, not 32 bits.
        *ptr = (OSAL_PADDRESS)(ptr + BlockSize / sizeof(OSAL_PADDRESS));
        ptr += BlockSize / sizeof(OSAL_PADDRESS);
    }
    // This is last block
    *ptr = NULL;

    LeaveCriticalSection(&pMemArray->lock);
    return SUCCESS;
}

RESULTCODE OsalDestroyMemArray(OSAL_MEMARRAY *pMemArray)
{
    DeleteCriticalSection(&pMemArray->lock);
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
    OSAL_PADDRESS retVal = NULL;

    EnterCriticalSection(&pMemArray->lock);
    retVal = pMemArray->freeBlock;
    if (retVal)
    {
        if((retVal < pMemArray->storage) || (retVal >= pMemArray->endOfStorage))
        {
            // TODO: DebugPrintf
            LeaveCriticalSection(&pMemArray->lock);
            *ppBlock = NULL;
            return FAILURE;
        }

        pMemArray->freeBlock = *((OSAL_PADDRESS*)retVal);
    }
    LeaveCriticalSection(&pMemArray->lock);

    *ppBlock = retVal;

    if (retVal)
        return SUCCESS;

    return FAILURE;
}

/*! \brief Return element of memory array to the pool of free blocks
 *
 *  \param pMemArray Pointer to memory array structure
 *  \param pBlock Pointer to allocated block
 *  \return Standard result code
*/
RESULTCODE OsalFreeMemArray(OSAL_MEMARRAY *pMemArray, OSAL_PADDRESS pBlock)
{
    if((pBlock < pMemArray->storage) || (pBlock >= pMemArray->endOfStorage))
    {
 //           printf("OsalFreeMem pBlock=0x%x is not inside MsgArray range: 0x%x ~ 0x%x \n", pBlock, pMemArray->storage, pMemArray->endOfStorage );
        // TODO: DebugPrintf
        return FAILURE;
    }

    if (pMemArray->freeBlock == pBlock)
    {
        // Simple protections against freeing of already freed block
        // TODO: DebugPrintf
        return SUCCESS;
    }

    EnterCriticalSection(&pMemArray->lock);
    *((OSAL_PADDRESS*)pBlock) = pMemArray->freeBlock;
    pMemArray->freeBlock = pBlock;
    LeaveCriticalSection(&pMemArray->lock);

    return SUCCESS;
}

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
    InitializeCriticalSection(&pQueue->lock);
    return SUCCESS;
}

/*! \brief Destroy queue control structure in memory
 *
 *  \param pQueue Pointer to queue structure to destroy
 *  \return Standard result code
*/
RESULTCODE OsalDestroyQueue(OSAL_QUEUE *pQueue)
{
    DeleteCriticalSection(&pQueue->lock);
    return FAILURE;
}

/*! \brief Enqueue element
 *
 *  \param pQueue Pointer to queue structure
 *  \param pQueueElement Element address to enqueue
 *  \return Standard result code
*/
RESULTCODE OsalEnqueue_spinlock(OSAL_QUEUE *pQueue, OSAL_PADDRESS pQueueElement)
{
    return OsalEnqueue(pQueue, pQueueElement);
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

    //Waits for ownership of the specified critical section object.
    //The function returns when the calling thread is granted ownership
    EnterCriticalSection(&pQueue->lock);
    put = pQueue->put;

    if (++put >= pQueue->size)
        put = 0;

    if (put != pQueue->get)
    {
        rc = SUCCESS;
        pQueue->storage[pQueue->put] = pQueueElement;
        pQueue->put = put;
    }
    LeaveCriticalSection(&pQueue->lock);
    return rc;
}

/*! \brief Dequeue next available element
 *
 *  \param pQueue Pointer to queue structure
 *  \param pQueueElement [OUT] Pointer to variable receiving address (void*),
 *         NULL if no elements
 *  \return Standard result code
*/
RESULTCODE OsalDequeue_spinlock(OSAL_QUEUE *pQueue, OSAL_PADDRESS *ppQueueElement)
{
    return OsalDequeue(pQueue, ppQueueElement);
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
    EnterCriticalSection(&pQueue->lock);
    getIdx = pQueue->get;
    if (pQueue->put != getIdx)
    {
        *ppQueueElement = pQueue->storage[getIdx];
        if (++getIdx == pQueue->size)
            getIdx = 0;
        pQueue->get = getIdx;
    }
    else
        retVal = FAILURE;
    LeaveCriticalSection(&pQueue->lock);
    return retVal;
}

/*! \brief Remove all waiting elements from queue
 *
 *  \param pQueue Pointer to queue structure
 *  \return Standard result code
*/
RESULTCODE OsalPurgeQueue(OSAL_QUEUE *pQueue)
{
    EnterCriticalSection(&pQueue->lock);
    pQueue->get = pQueue->put;
    LeaveCriticalSection(&pQueue->lock);
    return SUCCESS;
}

/*! \brief Get number of elements waiting to be dequeued
 *
 *  \param pQueue Pointer to queue structure
 *  \return Number of elements waiting
*/
RETURNVALUE OsalGetElementCount(OSAL_QUEUE *pQueue)
{
    if (pQueue->get <= pQueue->put)
        return pQueue->put - pQueue->get;
    return (RETURNVALUE)(pQueue->size - pQueue->get + pQueue->put);
}

/*! \brief Get maximum number of elements can be enqueued
 *
 *  \param pQueue Pointer to queue structure
 *  \return Number of elements waiting
*/
RETURNVALUE OsalGetQueueSize(OSAL_QUEUE *pQueue)
{
    return (RETURNVALUE)(pQueue->size);
}

/*! \brief Create timer
 *
 *  \param pTimer [OUT] Pointer to allocated timer
 *  \return Standard result code
*/
RESULTCODE OsalCreateTimer(OSAL_TIMER *pTimer)
{
    // Synchronization timer
    HANDLE timer = CreateWaitableTimer(NULL, FALSE, NULL);
    if (timer == NULL)
    {
        // TODO: Handle error
        *pTimer = NULL;
        return FAILURE;
    }
    *pTimer = (OSAL_TIMER) timer;
    return SUCCESS;
}

/*! \brief Destroy timer
 *
 *  \param pTimer Pointer to timer object
 *  \return Standard result code
*/
RESULTCODE OsalDestroyTimer(OSAL_TIMER *pTimer)
{
    CloseHandle((HANDLE) *pTimer);
    return SUCCESS;
}

/*! \brief Callback function used to signal that timer has expired
 *
 *  \param Arg Argument passed through SetWaitableTimer
 *  \param TimerLow Low-order portion of the UTC-based time at which the timer was signaled
 *  \param TimerHigh high-order portion
 *  \return Standard result code
*/
VOID CALLBACK OsalWin32TimerCB(LPVOID Arg, DWORD TimerLow, DWORD TimerHigh)
{
    OsalRaiseSema((OSAL_SEMAPHORE*) Arg);
    // OsalYield();
}

/*! \brief Create timer
 *
 *  \param pTimer Pointer to allocated timer
 *  \param delayMicroSeconds Timeout after which timer signals
 *  \param pSemaphore Pointer to semaphore object to set in signalling state
 *  \return Standard result code
*/
RESULTCODE OsalStartTimer(OSAL_TIMER *pTimer, OSAL_DELAYU delayMicroSeconds,
                          OSAL_SEMAPHORE *pSemaphore, BOOLEAN autoRestart)
{
	LARGE_INTEGER dueTime;
	LONG period;

    if (delayMicroSeconds > 1000)
    {
        // restart period in milliseconds
        period = delayMicroSeconds / 1000;
        // dueTime in 100-nanoseconds
        // negative number means relative period
        dueTime.QuadPart = -((LONG)(delayMicroSeconds * 10L));
    }
    else
    {
        // Win32 restart period cannot be less than 1 ms
        period = 1;
        dueTime.QuadPart = -10000;
    }

    // Periodic timer
    if (SetWaitableTimer((HANDLE) *pTimer, &dueTime, period, OsalWin32TimerCB,
                         (LPVOID) pSemaphore, FALSE) == 0)
    {
        // TODO: Handle error
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
    CancelWaitableTimer((HANDLE) *pTimer);
    return SUCCESS;
}

/*! \brief Put message into message basket (mail box)
 *
 *  \param pMsgBasket Pointer to message basket (mail box) handler
 *  \param pMsgBasketMemory
 *  \param msgBasketTotalSize
 *  \param msgSize
 *  \param pTaskSemaphore
 *  \param cacheManagementNeeded
 *  \return Standard result code
*/
RESULTCODE OsalCreateMsgBasket(OSAL_MAILBOX *pMsgBasket,
                               OSAL_PADDRESS pMsgBasketMemory,
                               OSAL_SIZE MsgBasketTotalSize,  OSAL_SIZE MsgSize,
                               OSAL_SEMAPHORE *pTaskSemaphore,
                               BOOLEAN CacheManagementNeeded)
{
    if (MsgSize == 0 || MsgBasketTotalSize == 0)
    {
        return FAILURE;
    }

    pMsgBasket->pSema = pTaskSemaphore;
    OsalCreateQueue(&pMsgBasket->queue, pMsgBasketMemory, MsgBasketTotalSize);
    pMsgBasketMemory = (OSAL_PADDRESS)(((U8*)pMsgBasketMemory) +
        (MsgBasketTotalSize / MsgSize + 1) * sizeof(OSAL_PADDRESS));
    OsalCreateMemArray(&pMsgBasket->data, pMsgBasketMemory,
        MsgBasketTotalSize, MsgSize);

    OsalCreateSema(pMsgBasket->pSema);

    return SUCCESS;
}

/** Send message to the message basket (mail box)
 *
 *  @param pMsgBasket [IN] Pointer to message basket (mail box) handler
 *  @param pMsg [IN] Pointer to the message data
 *  @param msgSize [IN] Data size
 */
RESULTCODE OsalSendMsg(OSAL_MAILBOX *pMsgBasket, PTR pMsg, OSAL_SIZE msgSize)
{
    OSAL_PADDRESS ppBlock;

    // check size of Msg
    if ((msgSize + sizeof(PTR)) > pMsgBasket->data.blockSize)
        return FAILURE;

    // coppy to Msg Array
    if (OsalAllocMemArray(&pMsgBasket->data, &ppBlock) != SUCCESS)
        return FAILURE;

    // set size of msg
    *((OSAL_SIZE*)ppBlock) = msgSize;
    memcpy(((U32*)ppBlock) + 1, pMsg, msgSize);

    // put to the queue
    OsalEnqueue(&pMsgBasket->queue, ppBlock);

    // rise semaphore
    OsalRaiseSema(pMsgBasket->pSema);

    return SUCCESS;
}

/*! \brief Get next message from basket
 *
 *  \param pMsgBasket [IN] Pointer to message basket (mail box) handler
 *  \param pMsg [IN/OUT] Pointer to the memory area which will receive data
 *  \param pMsgSize [OUT] Data size
 *  \return Standard result code
*/
RESULTCODE OsalGetMsg(OSAL_MAILBOX *pMsgBasket, PTR pMsg, OSAL_SIZE *pMsgSize)
{
    OSAL_PADDRESS ppBlock;
    OSAL_SIZE Size = 0;

    // wait for semaphore
    if (OsalWaitForSema(pMsgBasket->pSema, OSAL_WAIT_INFINITE) == SUCCESS)
    {
        // Dequeue
        if (OsalDequeue(&pMsgBasket->queue, &ppBlock) == SUCCESS)
        {
            // get size of msg
            Size = *((U32*)ppBlock);
            if((Size + 4) > pMsgBasket->data.blockSize)
            {
                OsalFreeMemArray(&pMsgBasket->data, ppBlock);
                return FAILURE;
            }
            // copy msg out
            memcpy(pMsg, ((U32*)ppBlock) + 1, Size);

            // free partition element
            OsalFreeMemArray(&pMsgBasket->data, ppBlock);

            if (OsalGetElementCount(&pMsgBasket->queue))
            {
                // raise sema for next MSG since the OsalWaitForSema clear semaphore
                OsalRaiseSema(pMsgBasket->pSema);
            }
        }
    }

    *pMsgSize = Size;
    if (Size)
        return SUCCESS;
    else
        return FAILURE;
}

/*! \brief Get current time
 *
 *  \param pCurrentTime [OUT] In microseconds
 *  \return Standard result code
*/
RESULTCODE OsalGetTime(OSAL_DELAYU *pCurrentTime)
{
    DWORD clk = GetTickCount();
    *pCurrentTime = (OSAL_DELAYU) clk * 1000UL;
    return SUCCESS;
}

/*! \brief Get current time in ms
 *
 *  \param pCurrentTime [OUT] In milliseconds
 *  \return Standard result code
*/
RESULTCODE OsalGetTimeMs(OSAL_DELAYM *pCurrentTime)
{
    DWORD clk = GetTickCount();
    *pCurrentTime = (OSAL_DELAYM) clk;
    return SUCCESS;
}

/*! \brief Get CPU ID
 *
 *  \return CPU ID
*/
U32 OsalGetCpuID(void)
{
#if _WIN32_WINNT >= 0x0600
    // Only present in Windows Server 2003+
    // although there is no restriction in <winbase.h>
    return (U32) GetCurrentProcessorNumber();
#else /* _WIN32_WINT */
    _asm {mov eax, 1}
    _asm {cpuid}
    _asm {shr ebx, 24}
    _asm {mov eax, ebx}
#endif /* _WIN32_WINNT */
}

#endif /* USE_WINDOWS || WIN32 */

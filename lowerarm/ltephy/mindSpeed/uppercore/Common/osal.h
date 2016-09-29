/** @file osal.h
 *
 * @brief OS Abstraction Layer interfaces and definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef OSAL_H_
#define OSAL_H_

// Support for R**-MAC
#ifdef SS_LINUX
#define USE_LINUX
#endif

#include "resultcodes.h"

//
// Common definitions
//

// TODO: do we need CPU mask?
#ifndef UARM
#define UARM                    0
#define LARM                    1
#endif /*UARM*/

#define OSAL_WAIT_INFINITE      0
#define OSAL_SECONDS            1000000
#define OSAL_MILLISECONDS       1000

// Pointer to memory area (void*)
typedef PTR OSAL_PADDRESS;

//
// Memory arrays
//
#define MA_AUTOALLOC            1       // Automatically allocated on global heap

//
// OS specific definitions
//

//
// Definitions for Linux
//
#if defined (USE_LINUX)

// For RVDS 4.0
#if __ARMCC_VERSION >= 400000
#include <linux_rvct.h>
#endif /* __ARMCC_VERSION */

//
// Compile-time settings
//
#undef OSAL_LINUX_PTHREAD_SEMA
#undef OSAL_LINUX_SIMPLEQ

// For pthread_yield
#define _GNU_SOURCE

#include <features.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <mqueue.h>
#include <sys/queue.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <syscall.h>
#include <linux/reboot.h>
#include <linux/limits.h>
#include <sys/reboot.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>
#include <ctype.h>

#ifdef OSAL_LINUX_PTHREAD_SEMA
typedef pthread_cond_t OSAL_SEMAPHORE;
#else /* OSAL_LINUX_PTHREAD_SEMA */
typedef sem_t OSAL_SEMAPHORE;
#endif /* OSAL_LINUX_PTHREAD_SEMA */

typedef pthread_t OSAL_TASK;
typedef pthread_mutex_t OSAL_MUTEX;
typedef void* OSAL_HEAP;
typedef int OSAL_PRIORITY;
typedef size_t OSAL_SIZE;
typedef timer_t OSAL_TIMER;

typedef useconds_t OSAL_DELAYU;     // In microseconds (default)
typedef U32 OSAL_DELAYM;            // In milliseconds

// Thread priority mapping
#define OSAL_PRIORITY_IDLE          0
#define OSAL_PRIORITY_REALTIME      99
#define OSAL_PRIORITY_NORMAL        ((OSAL_PRIORITY_IDLE+OSAL_PRIORITY_REALTIME)/2)

typedef mqd_t OSAL_MAILBOX;

#ifdef OSAL_LINUX_SIMPLEQ
typedef struct OsalQueueEntry
{
    SIMPLEQ_ENTRY(OsalQueueEntry) entry;
    OSAL_PADDRESS data;
} OSAL_QUEUE_ENTRY;
#endif /* OSAL_LINUX_SIMPLEQ */

// Queues (can store pointer in queue)
typedef struct OsalQueue
{
    U32 get;                        // Index
    U32 put;                        // Index
    OSAL_PADDRESS *storage;
    OSAL_SIZE size;
    pthread_mutex_t lock;
    pthread_spinlock_t spin;
} OSAL_QUEUE;

// For fast memory allocations of fixed size
typedef struct OsalMemArray
{
    OSAL_PADDRESS *ppFreeBlock;
    OSAL_PADDRESS storage;
    OSAL_PADDRESS endOfStorage;
    U32 blockSize;
    U32 blockCount;
    U32 flags;
    pthread_mutex_t lock;
} OSAL_MEMARRAY;

// One-way pipe (ring buffer)
// TODO: Make similar to MsgBasket but internal to process
typedef struct OsalRingBuf
{
    U32 get;
    U32 put;
    U32 size;
    OSAL_PADDRESS *storage;
    OSAL_PADDRESS heap;
    pthread_mutex_t lock;
} OSAL_RINGBUF;

// CPU cache management
#define OSAL_CACHELINE_SIZE         32

//
// Definitions for bare metal access
//
#elif defined (USE_BARE)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Time measurement
typedef U32 OSAL_DELAYU;            // In microseconds (default)
typedef U32 OSAL_DELAYM;            // In milliseconds

typedef V32 OSAL_SEMAPHORE;
typedef U32 OSAL_TIMER;
typedef U32 OSAL_TASK;
typedef V32 OSAL_MUTEX;
typedef U32 OSAL_MAILBOX;
typedef U32 OSAL_SOCKET;

typedef U32 OSAL_SIZE;
typedef U32 OSAL_PRIORITY;

typedef struct OsalHeap
{
    OSAL_PADDRESS base;
    OSAL_SIZE size;
} OSAL_HEAP;

typedef struct OsalQueue
{
    U32 get;                        // Index
    U32 put;                        // Index
    OSAL_PADDRESS *storage;
    OSAL_SIZE size;
    OSAL_SIZE elemSize;
    OSAL_HEAP *heap;
} OSAL_QUEUE;

typedef struct OsalMemArray
{
    OSAL_PADDRESS *freeBlock;
    OSAL_PADDRESS storage;
    U32 blockSize;
    U32 blockCount;
} OSAL_MEMARRAY;

typedef OSAL_QUEUE OSAL_RINGBUF;

// Thread priority mapping
#define OSAL_PRIORITY_IDLE          1
#define OSAL_PRIORITY_NORMAL        50
#define OSAL_PRIORITY_REALTIME      100

// CPU cache management
#define OSAL_CACHELINE_SIZE         32

//
// Definitions for Windows
//
#elif defined (WIN32) || defined (USE_WINDOWS)

// Targeting Windows XP
#ifndef _WIN32_WINNT
#define _WIN32_WINNT                0x0501
#endif

#if _MSC_VER >= 1400
#ifndef WINVER
#define WINVER                      0x0501
#endif
#endif

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

// Time measurement
typedef U32 OSAL_DELAYU;            // In microseconds (default)
typedef U32 OSAL_DELAYM;            // In milliseconds

typedef HANDLE OSAL_SEMAPHORE;
typedef HANDLE OSAL_TIMER;
typedef HANDLE OSAL_TASK;
typedef HANDLE OSAL_MUTEX;
typedef HANDLE OSAL_HEAP;
typedef SIZE_T OSAL_SIZE;

typedef struct OsalQueue
{
    OSAL_PADDRESS *storage;
    U32 get;                        // Index
    U32 put;                        // Index
    OSAL_SIZE size;
    CRITICAL_SECTION lock;
} OSAL_QUEUE;

typedef OSAL_QUEUE OSAL_RINGBUF;

typedef struct OsalMemArray
{
    OSAL_PADDRESS *freeBlock;
    OSAL_PADDRESS storage;
    OSAL_PADDRESS endOfStorage;
    OSAL_SIZE blockSize;
    U32 blockCount;
    CRITICAL_SECTION lock;
} OSAL_MEMARRAY, *POSAL_MEMARRAY;

typedef struct OsalMgBasket
{
    // FIFO
    OSAL_QUEUE queue;
    // array for data of msg basket
    OSAL_MEMARRAY data;
    // semaphore for control
    OSAL_SEMAPHORE *pSema;
} OSAL_MAILBOX;

typedef HANDLE OSAL_SOCKET;

// Task priorities
typedef U32 OSAL_PRIORITY;

// Thread priority mapping
#define OSAL_PRIORITY_IDLE          THREAD_PRIORITY_IDLE
#define OSAL_PRIORITY_NORMAL        THREAD_PRIORITY_NORMAL
#define OSAL_PRIORITY_REALTIME      THREAD_PRIORITY_TIME_CRITICAL

// CPU cache management
#define OSAL_CACHELINE_SIZE         1

//
// Definitions for RTXC
//
#elif defined(USE_RTXC)

#include <stdlib.h>
#include <string.h>
#include <rtxcopts.h>
#include <typedef.h>
#include <utilprot.h>
#include <rtxcstdio.h>
#include <rtxcapi.h>

#define printf                      rtxc_printf
#define sprintf                     rtxc_sprintf
#define getchar                     rtxc_getchar
#define gets                        rtxc_gets
#define putchar                     rtxc_putchar
#define puts                        rtxc_puts

// Time measurement
typedef U32 OSAL_DELAYU;            // In microseconds (default)
typedef U32 OSAL_DELAYM;            // In milliseconds

typedef SEMA OSAL_SEMAPHORE;
typedef ALARM OSAL_TIMER;
typedef TASK OSAL_TASK;
typedef MUTX OSAL_MUTEX;
typedef KOBJECT OSAL_HEAP;
typedef PART OSAL_MEMARRAY;
typedef QUEUE OSAL_QUEUE;
typedef KOBJECT OSAL_MAILBOX;
typedef KOBJECT OSAL_SOCKET;

typedef struct OsalRingBuf
{
    U32 get;                        // Index
    U32 put;                        // Index
    U32 size;
    OSAL_PADDRESS *storage;
} OSAL_RINGBUF;

typedef unsigned OSAL_SIZE;
typedef PRIORITY OSAL_PRIORITY;

// Thread priority mapping
#define OSAL_PRIORITY_IDLE          NULLTASK_PRIORITY
#define OSAL_PRIORITY_NORMAL        ((LOWEST_PRIORITY - HIGHEST_PRIORITY)/2)
#define OSAL_PRIORITY_REALTIME      HIGHEST_PRIORITY

// CPU cache management
#define OSAL_CACHELINE_SIZE          32

//
// Definitions for vxWorks
//
#elif defined(USE_VXWORKS)

#include <vxWorks.h>

#endif

//
// Function prototypes
//

// Main entry point for OS initialization
RESULTCODE OsalInit(U32 cpuid);

//
// Tasks
//
RESULTCODE OsalCreateTask(OSAL_TASK *pTask, OSAL_PADDRESS pEntryPoint,
                          PTR pTaskParam, const char *TaskName,
                          OSAL_PRIORITY TaskPriority, OSAL_PADDRESS pStackBase,
                          OSAL_SIZE StackSize);
RESULTCODE OsalDestroyTask(OSAL_TASK *pTask);
RESULTCODE OsalActivateTask(OSAL_TASK *pTask);
RESULTCODE OsalDeactivateTask(OSAL_TASK *pTask);
RESULTCODE OsalSetTaskPriority(OSAL_TASK *pTask, OSAL_PRIORITY TaskPriority);
RESULTCODE OsalGetTaskPriority(OSAL_TASK *pTask, OSAL_PRIORITY *pTaskPriority);
RESULTCODE OsalWaitForTaskClose(OSAL_TASK *pTask, OSAL_DELAYU timeout, BOOLEAN forceClose);
RESULTCODE OsalWaitDelay(OSAL_DELAYU delayMicroSeconds);
RESULTCODE OsalYield(void);

//
// Semaphores
//
RESULTCODE OsalCreateSema(OSAL_SEMAPHORE *pSemaphore);
RESULTCODE OsalDestroySema(OSAL_SEMAPHORE *pSemaphore);
RESULTCODE OsalRaiseSema(OSAL_SEMAPHORE *pSemaphore);
RESULTCODE OsalRaiseSemaIsr(OSAL_SEMAPHORE *pSemaphore);
RESULTCODE OsalClearSema(OSAL_SEMAPHORE *pSemaphore);
RESULTCODE OsalWaitForSema(OSAL_SEMAPHORE *pSemaphore,
                           OSAL_DELAYU timeoutMicroSeconds);
RESULTCODE OsalCheckSema(OSAL_SEMAPHORE *pSemaphore);

//
// Mutexes
//
RESULTCODE OsalCreateMutex(OSAL_MUTEX *pMutex);
RESULTCODE OsalDestroyMutex(OSAL_MUTEX *pMutex);
RESULTCODE OsalCaptureMutex(OSAL_MUTEX *pMutex, OSAL_DELAYU timeoutMicroSeconds);
RESULTCODE OsalFreeMutex(OSAL_MUTEX *pMutex);

//
// Heap
//
RESULTCODE OsalCreateHeap(OSAL_HEAP *pHeap, OSAL_PADDRESS pHeapMemory,
                          OSAL_SIZE HeapSize, BOOLEAN CacheManagementNeeded);
RESULTCODE OsalDestroyHeap(OSAL_HEAP *pHeap);
RESULTCODE OsalAllocHeap(OSAL_HEAP *pHeap, OSAL_PADDRESS *ppBlock,
                              OSAL_SIZE BlockSize);
RESULTCODE OsalFreeHeap(OSAL_HEAP *pHeap, OSAL_PADDRESS pBlock);

//
// Memory arrays (fast partitions)
//
RESULTCODE OsalCreateMemArray(OSAL_MEMARRAY *pMemArray, OSAL_PADDRESS pMemArrayMemory,
                              OSAL_SIZE TotalSize, OSAL_SIZE BlockSize);
RESULTCODE OsalDestroyMemArray(OSAL_MEMARRAY *pMemArray);
RESULTCODE OsalAllocMemArray(OSAL_MEMARRAY *pMemArray, OSAL_PADDRESS *ppBlock);
RESULTCODE OsalFreeMemArray(OSAL_MEMARRAY *pMemArray, OSAL_PADDRESS pBlock);
#if defined (USE_RTXC)
RESULTCODE OsalAllocMemArrayIsr(OSAL_MEMARRAY *pMemArray, OSAL_PADDRESS *ppBlock);
#else /*USE_RTXC*/
#define OsalAllocMemArrayIsr   OsalAllocMemArray
#endif /*USE_RTXC*/

//
// Queue functions
//
RESULTCODE OsalCreateQueue(OSAL_QUEUE *pQueue, OSAL_PADDRESS pQueueMemory,
                           OSAL_SIZE QueueSize);
RESULTCODE OsalDestroyQueue(OSAL_QUEUE *pQueue);
RESULTCODE OsalEnqueue(OSAL_QUEUE *pQueue, OSAL_PADDRESS pQueueElement);
RESULTCODE OsalEnqueue_spinlock(OSAL_QUEUE *pQueue, OSAL_PADDRESS pQueueElement);
RESULTCODE OsalDequeue(OSAL_QUEUE *pQueue, OSAL_PADDRESS *ppQueueElement);
RESULTCODE OsalDequeue_spinlock(OSAL_QUEUE *pQueue, OSAL_PADDRESS *ppQueueElement);
RESULTCODE OsalPurgeQueue(OSAL_QUEUE *pQueue);
RETURNVALUE OsalGetElementCount(OSAL_QUEUE *pQueue);
RETURNVALUE OsalGetQueueSize(OSAL_QUEUE *pQueue);

#if defined (USE_RTXC) || defined (USE_LINUX)
// Ring Buffer functions (ISR-compatible)
RESULTCODE OsalCreateRing(OSAL_RINGBUF *pRingBuf, OSAL_PADDRESS pRingBufMemory,
                           OSAL_SIZE ringBufSize);
RESULTCODE OsalDestroyRing(OSAL_RINGBUF *pRingBuf);
RESULTCODE OsalPutRing(OSAL_RINGBUF *pRingBuf, OSAL_PADDRESS pRingBufElement);
RESULTCODE OsalGetRing(OSAL_RINGBUF *pRingBuf, OSAL_PADDRESS *ppRingBufElement);
RESULTCODE OsalFlushRing(OSAL_RINGBUF *pRingBuf);
#else /* USE_RTXC || USE_LINUX */
#define OsalCreateRing           OsalCreateQueue
#define OsalDestroyRing          OsalDestroyQueue
#define OsalPutRing              OsalEnqueue
#define OsalGetRing              OsalDequeue
#define OsalFlushRing            OsalPurgeQueue
#endif /* USE_RTXC || USE_LINUX */

//
// Timers
//
RESULTCODE OsalCreateTimer(OSAL_TIMER *pTimer);
RESULTCODE OsalDestroyTimer(OSAL_TIMER *pTimer);
RESULTCODE OsalStartTimer(OSAL_TIMER *pTimer, OSAL_DELAYU delayMicroSeconds,
                          OSAL_SEMAPHORE *pSemaphore, BOOLEAN autoRestart);
RESULTCODE OsalStopTimer(OSAL_TIMER *pTimer);

//
// Message baskets
//
RESULTCODE OsalCreateMailBox(OSAL_MAILBOX *pMsgBasket,
                             OSAL_PADDRESS pMsgBasketMemory,
                             OSAL_SIZE MsgBasketTotalSize,  OSAL_SIZE MsgSize,
                             OSAL_SEMAPHORE *pTaskSemaphore,
                             BOOLEAN CacheManagementNeeded);
RESULTCODE OsalDestroyMailBox(OSAL_MAILBOX *pMsgBasket,
                              OSAL_PADDRESS pMsgBasketName);
RESULTCODE OsalSendMsg(OSAL_MAILBOX *pMsgBasket, PTR pMsg,
                       OSAL_SIZE msgSize);
RESULTCODE OsalGetMsg(OSAL_MAILBOX *pMsgBasket,
                      PTR pMsg,
                      OSAL_SIZE *pMsgSize);

//
// Time
//

// In microseconds
RESULTCODE OsalGetTime(OSAL_DELAYU *pCurrentTime);
// In milliseconds
RESULTCODE OsalGetTimeMs(OSAL_DELAYM *pCurrentTime);

//
// Miscellaneous
//
U32 OsalGetCpuID(void);

//
// Global variables
//
extern OSAL_HEAP OsalGlobalHeap;

#endif /* OSAL_H_ */

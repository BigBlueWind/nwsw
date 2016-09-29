//-------------------------------------------------------------------------------------------
/** @file 4gmx_queue.h
 *
 * @brief Queues API declaration
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_QUEUE_H_
#define _4GMX_QUEUE_H_

#include "4gmx_config.h"

#ifdef MX_QUEUE_ENABLED

#ifdef __cplusplus
extern "C"
{
#endif

#include "4gmx_types.h"
#include "4gmx_const.h"

/************************************************************************/
/*       The list of module error codes                                 */
/************************************************************************/
#define MX_QUEUE_ALLOC_ERROR        DEF_MXRC(MXM_QUEUE,  1)
#define MX_QUEUE_ASSIGN_NAME_ERROR  DEF_MXRC(MXM_QUEUE,  2)
#define MX_QUEUE_EMPTY              DEF_MXRC(MXM_QUEUE,  3)
#define MX_QUEUE_OVERFLOW           DEF_MXRC(MXM_QUEUE,  4)
#define MX_QUEUE_CANNOT_BE_SHARED   DEF_MXRC(MXM_QUEUE,  5)
#define MX_QUEUE_REMOTE_OBJ_NOT_SUP DEF_MXRC(MXM_QUEUE,  6) // Only localy created object can be used for object configuration
#define MX_QUEUE_UNKNOWN_PARAMETER  DEF_MXRC(MXM_QUEUE,  7) // This parameter id is not supported by queue API
#define MX_QUEUE_SYNC_TIMEOUT       DEF_MXRC(MXM_QUEUE,  8) // It was not possible to get the lock for specified period of time 

/************************************************************************/
/*      The queue control flags                                          */
/************************************************************************/
#define QUEUE_CF_ALLOCATED          (1<<0)
#define QUEUE_CF_NAMED              (1<<1)
#define QUEUE_CF_OPENED             (1<<3)
#define QUEUE_CF_SYNC               (1<<4)
#define QUEUE_CF_EMPTY              (1<<5)
#define QUEUE_CF_HW_SEMA            (1<<6)
#define QUEUE_CF_CACHE_ON           (1<<7)

#define MARK_QUEUE_ALLOC(QUEUE)     ((QUEUE)->CtrlFlags |= QUEUE_CF_ALLOCATED)
#define MARK_QUEUE_DEALLOC(QUEUE)   ((QUEUE)->CtrlFlags &= ~QUEUE_CF_ALLOCATED)
#define IS_QUEUE_ALLOC(QUEUE)       ((QUEUE)->CtrlFlags & QUEUE_CF_ALLOCATED)

#define MARK_QUEUE_NAMED(QUEUE)     ((QUEUE)->CtrlFlags |= QUEUE_CF_NAMED)
#define MARK_QUEUE_UNNAMED(QUEUE)   ((QUEUE)->CtrlFlags &= ~QUEUE_CF_NAMED)
#define IS_QUEUE_NAMED(QUEUE)       ((QUEUE)->CtrlFlags & QUEUE_CF_NAMED)

#define MARK_QUEUE_OPENED(QUEUE)    ((QUEUE)->CtrlFlags |= QUEUE_CF_OPENED)
#define MARK_QUEUE_CLOSED(QUEUE)    ((QUEUE)->CtrlFlags &= ~QUEUE_CF_OPENED)
#define IS_QUEUE_OPENED(QUEUE)      ((QUEUE)->CtrlFlags & QUEUE_CF_OPENED)

#define MARK_QUEUE_SYNC(QUEUE)      ((QUEUE)->CtrlFlags |= QUEUE_CF_SYNC)
#define MARK_QUEUE_DESYNC(QUEUE)    ((QUEUE)->CtrlFlags &= ~QUEUE_CF_SYNC)
#define IS_QUEUE_SYNC(QUEUE)        ((QUEUE)->CtrlFlags & QUEUE_CF_SYNC)

#define MARK_QUEUE_HW_SEMA(QUEUE)   ((QUEUE)->CtrlFlags |= QUEUE_CF_HW_SEMA)
#define MARK_QUEUE_NOT_HW_SEMA(QUEUE)((QUEUE)->CtrlFlags &= ~QUEUE_CF_HW_SEMA)
#define IS_QUEUE_HW_SEMA(QUEUE)     ((QUEUE)->CtrlFlags & QUEUE_CF_HW_SEMA)

#define MARK_QUEUE_OPEN_CACHE(QUEUE)((QUEUE)->CtrlFlags |= QUEUE_CF_CACHE_ON)
#define MARK_QUEUE_OPEN_NOT_CACHE   (QUEUE)((QUEUE)->CtrlFlags &= ~QUEUE_CF_CACHE_ON)
#define IS_QUEUE_OPEN_CACHE(QUEUE)  ((QUEUE)->CtrlFlags & QUEUE_CF_CACHE_ON)

/***************************************************************************/
/**            QUEUE configuration parameters                              */
/***************************************************************************/
#define MX_QUEUE_CFG_LOCK_TIMEOUT   1   // The timeout to get lock for the resource 

/***************************************************************************/

/************************************************************************/
/*           The event type descriptor definition                       */
/************************************************************************/
typedef struct _MX_QUEUE_
{
    LPVOID  StorageOrLinkedObj;
    /**< The pointer to the queue storage or pointer to descriptor of opened queue */
    VUINT16 GetIndex;  /**< The index of getting              */
    VUINT16 PutIndex;  /**< The index of putting new one element          */
    UINT16  QueueSize; /**< The maximum number of elements that can be placed into queue    */
    UINT16  ElmSize;   /**< The size of element in bytes            */
    MXSYNC  Sync;      /**< Synchronization               */
    UINT32  SyncTicks; /**< The maximum ticks to get the lock for resource in timers ticks, 0 - not used*/

    HANDLE  Event;     /**< The Event associated with the queue          */
    UINT32  CtrlFlags;  /**< The object control flags              */

    #ifdef MX_TRACER_ENABLED
    void (*TrPut)(void*pCtx, UINT32 data);   /**< The pointer to the tracer put function                             */
    void (*TrGet)(void*pCtx, UINT32 data);   /**< The pointer to the tracer get function                             */
    void * TrPtr;                            /**< The pointer to some abstract tracer context, it's set by a tracer  */
    UINT32 TrLR;                             /**< The caller address                                                 */
    UINT32 TrNumFree;
    #endif

} QUEUE, *PQUEUE;

typedef struct _MX_IC_QUEUE_
{
    LPVOID  Storage;   /**< The pointer to the queue storage or pointer to descriptor of opened queue */
    VUINT16 GetIndex;  /**< The index of getting              */
    VUINT16 PutIndex;  /**< The index of putting new one element          */
    UINT16  QueueSize; /**< The maximum number of elements that can be placed into queue    */
    UINT16  ElmSize;   /**< The size of element in bytes            */
    UINT32  Sync;      /**< Synchronization               */

} ICQUEUE, PICQUEUE;

/** @brief The function creates a queue.

 @param pMem   [in] - the pointer to memory (queue storage)
 @param nElmCount [in] - the max number of elements that can be stored in queue
 @param nElmSize  [in] - the size of element in bytes
 @param name   [in] - the name of queue, it can be null
 @param hEvent  [in] - the event associated with the queue, this event will be set when new one element is placed into queue
 @param oHand  [out] - the handle of created object

 @return [MXRC] error code*/

MXRC MxCreateQueue(LPVOID pMem, UINT32 nElmCount, UINT32 nElmSize,
                   const char *name, HANDLE hEvent, HANDLE * oHand);

/** @brief The function allocates the shared queue for simple inter-cpu communication.

 @param pQDescr  [in] - the pointer to the not cached memory for queue descriptor
 @param pMem  [in] - the pointer to the queue storage
 @param nElmCount[in] - the maximum number of queue elements
 @param nElmSize [in] - the element size in bytes
 @param name  [in] - the name of object
 @param hEvent [in] - the Event HANDLE that was used in case of putting data into the queue
 @param oHand [out] - the queue handle

 @return [MXRC] error code */

MXRC MxCreateSharedQueue(ICQUEUE * pQDescr, LPVOID pMem, UINT32 nElmCount,
                      UINT32 nElmSize, const char *name, HANDLE hEvent,
                      HANDLE * oHand);

/** @brief The function "opens"/"set connections with" created queue and returns its handle,
   the opened object can be used like a locally created.

 @param pQueueDescr [in] - the pointer to the inter-cpu queue descriptor
 @param name   [in] - the name of opened object
 @param oHand  [out] - the handle of opened queueition

 @return [MXRC] error code */
MXRC MxOpenQueue(ICQUEUE * pQueueDescr, const char *name, HANDLE * oHand);

/** @brief The function "opens"/"set connections with" created queue and returns
           its handle, the opened object can be used like a locally created.

    @note: this function is designed to be used with HW implemented semas,
            so usage of this function is platform depended

 @param pQueueDescr [in] - the pointer to the inter-cpu queue descriptor
 @param name        [in] - the name of opened object
 @param pHwSema     [in] - the pointer to the HW SEMA register (it cannot be NULL)
 @param bCacheable  [in] - if TRUE - the opened descriptor is cacheable

 @param oHand       [out] - the handle of opened queue

 @return [MXRC] error code */

MXRC MxOpenQueueEx(ICQUEUE * pQueueDescr, const char *name, PUINT32 pHwSema, BOOL bCacheable, HANDLE * oHand);

/** @brief The function removes created queue.
 @param h [in] - the queue handle
 @return [MXRC] error code */

MXRC MxRemoveQueue(HANDLE h);

/** @brief The function puts data into queue, if event object is not HNULL,
   the event object will be set.

 @param h [in] - the queue handle
 @param pData[in] - the data placed into queue

 @return [MXRC] error code */

#ifdef MX_TRACER_ENABLED
    MXRC MxPutQueueData(HANDLE h, LPVOID pData);
    MXRC MxPutQueueDataLR(HANDLE h, LPVOID pData, UINT32 trace_lr);

#else 
    MXRC MxPutQueueData(HANDLE h, LPVOID pData);
#endif

/** @brief The function puts data to the queue,
 *  if an event object that is associated with this queue is not HNULL,
 *  it will be set.

 @param h [in] - the queue handle
 @param pData[in] - the data placed into queue
 @param pCount[out] - the current number of elements in this queue

 @return [MXRC] error code */

#ifdef MX_TRACER_ENABLED
    MXRC MxPutQueueDataEx(HANDLE h, LPVOID pData, PUINT32 pCount);
    MXRC MxPutQueueDataExLR(HANDLE h, LPVOID pData, PUINT32 pCount, UINT32 trace_lr);

#else 
    MXRC MxPutQueueDataEx(HANDLE h, LPVOID pData, PUINT32 pCount);
#endif

/** @brief The function returns queue element, the returned element will be deleted from the queue
    @param h [in] - the queue handle
    @param pData[out] - the buffer for data storing
    @return [MXRC] error code */

#ifdef MX_TRACER_ENABLED
    MXRC MxGetQueueData(HANDLE h, LPVOID pData);
    MXRC MxGetQueueDataLR(HANDLE h, LPVOID pData, UINT32 trace_lr);
#else
    MXRC MxGetQueueData(HANDLE h, LPVOID pData);
#endif

/** @brief The function returns queue element without an erasing operation
 @param h [in] - the queue handle
 @param pData[out] - the buffer for data storing
 @return [MXRC] error code */

MXRC MxPeekQueueData(HANDLE h, LPVOID pData);

/** @brief The function empties queue.
 @param h [in] - the queue handle
 @return [MXRC] error code */

MXRC MxEmptyQueue(HANDLE h);

/** @brief The function returns the maximum number of elements that can be putted to the queue

    @param h  [in] - the queue handle
    @param pnCount [out] - the number of queue elements

    @return [MXRC] error code

    \ingroup group_lte_4gmx_queue
*/
UINT32 MxGetQueueSize(HANDLE h);

/** @brief The function returns number of elements currently present in queue

 @param h  [in] - the queue handle
 @param pnCount [out] - the number of queue elements

 @return [MXRC] error code */

MXRC MxGetQueueDataCount(HANDLE h, UINT32 * pnCount);

/** @brief The function configures queue object 

 @param h  [in] - the queue handle
 @param id [in] - the queue config parameter: see MX_QUEUE_CFG_xxx
 @param val[in] - parameter

 @return [MXRC] error code */

MXRC MxQueueConfig (HANDLE h, UINT32 id, UINT32 val);

#ifdef MX_INTERCPU_ENABLED

/** @brief This function shares the queue
    @param hQueue  [in] - the queue handler
    @param pLock   [in] - the address of the sync object (in non cached memory)
    @return [MXRC] error code */

MXRC MxShareQueue (HANDLE INP hQueue, PVUINT32 pLock);

/** @brief This function unshares the queue and this queue cannot be accessed
           from the other 4GMX systems

    @param hQueue [in]  - the queue handler
    @return [MXRC] error code */

MXRC MxUnshareQueue (HANDLE INP hQueue);

#endif /*MX_INTERCPU_ENABLED*/

#ifdef __cplusplus
}
#endif

#endif        /*MX_QUEUE_ENABLED */
#endif /*_4GMX_QUEUE_H_*/

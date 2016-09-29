//-------------------------------------------------------------------------------------------
/** @file 4gmx_debug.h
 *
 * @brief The declaration of internal 4GMX API for tracing system work
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_DEBUG_H_
#define _4GMX_DEBUG_H_

#include "4gmx_config.h"

#ifdef _MX_DEBUG

#ifdef __cplusplus
extern "C"
{
#endif

#include "4gmx_types.h"
#include "4gmx_const.h"
#include "4gmx_kernel.h"

/************************************************************/
/*         The list of module error codes                   */
/************************************************************/
#define MX_TRACE_OK                     MX_OK
#define MX_TRACE_EMPTY                  DEF_MXRC(MXM_DEBUG_TRACE, 1)
#define MX_TRACE_SMALL_BUF              DEF_MXRC(MXM_DEBUG_TRACE, 2)


#define THREAD_TRACE_BASE   0
#define EVENT_TRACE_BASE    10
#define TIMER_TRACE_BASE    20
#define PART_TRACE_BASE     30
#define QUEUE_TRACE_BASE    40
#define HEAP_TRACE_BASE     50
#define RHEAP_TRACE_BASE    60
#define LIST_TRACE_BASE     70
#define USR_APP_TRACE_BASE  100

#define MAKE_TRACE_ID(base, x)      ((base) + (x))

/** @brief The IDs of system events maintained by the trace */

typedef enum _TRACE_EVENT_ID_
{

/**************************************/
/* The "Thread" objects' trace events */
/**************************************/

#ifdef TRACE_THR_CREATE
    TEID_CREATE_THR     = MAKE_TRACE_ID(THREAD_TRACE_BASE,  1), // The thread object has been created
#endif

#ifdef TRACE_THR_REMOVE
    TEID_REMOVE_THR     = MAKE_TRACE_ID(THREAD_TRACE_BASE,  2), // The thread object has been removed
#endif

#ifdef TRACE_THR_SCHEDULE
    TEID_SCHEDULE_THR   = MAKE_TRACE_ID(THREAD_TRACE_BASE,  3), // The thread object has been scheduled for exec.
#endif

#ifdef TRACE_THR_UNSCHEDULE
    TEID_UNSCHEDULE_THR = MAKE_TRACE_ID(THREAD_TRACE_BASE,  4), // The thread object has been unscheduled for exec.
#endif

#ifdef TRACE_THR_START
    TEID_START_THR      = MAKE_TRACE_ID(THREAD_TRACE_BASE,  5), // The thread object has been starting
#endif

#ifdef TRACE_THR_FINISH
    TEID_FINISH_THR     = MAKE_TRACE_ID(THREAD_TRACE_BASE,  6), // The thread object has been executed
#endif

#ifdef TRACE_THR_PRIOR
    TEID_SET_THR_PRIOR  = MAKE_TRACE_ID(THREAD_TRACE_BASE,  7), // The thread object priority has been changed
#endif

/**************************************/
/* The "Event" objects' trace events  */
/**************************************/

#ifdef MX_EVENT_ENABLED

#ifdef TRACE_EVENT_CREATE
    TEID_CREATE_EVENT   = MAKE_TRACE_ID(EVENT_TRACE_BASE,  1),    // The event object creation
#endif

#ifdef TRACE_EVENT_REMOVE
    TEID_REMOVE_EVENT   = MAKE_TRACE_ID(EVENT_TRACE_BASE,  2),    // The event object removing
#endif

#ifdef TRACE_EVENT_SET
    TEID_SET_EVENT      = MAKE_TRACE_ID(EVENT_TRACE_BASE,  3),   // The event object removing
#endif

#ifdef TRACE_EVENT_RESET
    TEID_RESET_EVENT    = MAKE_TRACE_ID(EVENT_TRACE_BASE,  4),   // The event object removing
#endif

#endif /*MX_EVENT_ENABLED*/


/**************************************/
/* The "Timer" objects' trace events  */
/**************************************/

#ifdef MX_TIMER_ENABLED

#ifdef TRACE_TIMER_CREATE
    TEID_CREATE_TIMER   = MAKE_TRACE_ID(TIMER_TRACE_BASE,  1),   // The timer object creation
#endif

#ifdef TRACE_TIMER_REMOVE
    TEID_REMOVE_TIMER   = MAKE_TRACE_ID(TIMER_TRACE_BASE,  2),   // The timer object removing
#endif

#ifdef TRACE_TIMER_PULSE
    TEID_PULSE_TIMER    = MAKE_TRACE_ID(TIMER_TRACE_BASE,  3),   // The timer object has set event
#endif

#ifdef TRACE_TIMER_START
    TEID_START_TIMER    = MAKE_TRACE_ID(TIMER_TRACE_BASE,  4),   // The timer object has been started
#endif

#ifdef TRACE_TIMER_START
    TEID_START_TIMER_FRAC= MAKE_TRACE_ID(TIMER_TRACE_BASE, 5),   // The timer object has been started
#endif

#ifdef TRACE_TIMER_STOP
    TEID_STOP_TIMER     = MAKE_TRACE_ID(TIMER_TRACE_BASE,  6),   // The timer object has been stopped
#endif

#endif /*MX_TIMER_ENABLED*/

/*******************************************/
/* The "Partition" objects' trace events   */
/*******************************************/

#ifdef MX_PART_ENABLED

#ifdef TRACE_PART_CREATE
    TEID_CREATE_PART    = MAKE_TRACE_ID(PART_TRACE_BASE,  1),   // The object creation
#endif

#ifdef TRACE_PART_REMOVE
    TEID_REMOVE_PART    = MAKE_TRACE_ID(PART_TRACE_BASE,  2),   // The object removing
#endif

#ifdef TRACE_PART_OPEN
    TEID_OPEN_PART      = MAKE_TRACE_ID(PART_TRACE_BASE,  3),   // The object opening (inter-cpu object)
#endif

#ifdef TRACE_PART_ALLOC
    TEID_ALLOC_PART     = MAKE_TRACE_ID(PART_TRACE_BASE,  4),   // Memory allocation
#endif

#ifdef TRACE_PART_FREE
    TEID_FREE_PART      = MAKE_TRACE_ID(PART_TRACE_BASE,  5),   // Memory free
#endif

#endif /*MX_PART_ENABLED*/


/*******************************************/
/* The "Queue" objects' trace events       */
/*******************************************/

#ifdef MX_QUEUE_ENABLED

#ifdef TRACE_QUEUE_CREATE
    TEID_CREATE_QUEUE   = MAKE_TRACE_ID(QUEUE_TRACE_BASE,  1),   // An queue creation
#endif

#ifdef TRACE_QUEUE_REMOVE
    TEID_REMOVE_QUEUE   = MAKE_TRACE_ID(QUEUE_TRACE_BASE,  2),   // An queue removing
#endif

#ifdef TRACE_QUEUE_OPEN
    TEID_OPEN_QUEUE     = MAKE_TRACE_ID(QUEUE_TRACE_BASE,  3),   // An queue opening
#endif

#ifdef TRACE_QUEUE_PUT
    TEID_PUT_QUEUE      = MAKE_TRACE_ID(QUEUE_TRACE_BASE,  4),   // Data putting
#endif

#ifdef TRACE_QUEUE_GET
    TEID_GET_QUEUE      = MAKE_TRACE_ID(QUEUE_TRACE_BASE,  5),   // Data getting
#endif

#ifdef TRACE_QUEUE_EMPTY
    TEID_EMPTY_QUEUE    = MAKE_TRACE_ID(QUEUE_TRACE_BASE,  6),   // Erasing of all queue elements
#endif

#endif /*MX_QUEUE_ENABLED*/

/*******************************************/
/* The "HEAP" objects' trace events        */
/*******************************************/

#ifdef MX_HEAP_ENABLED

#ifdef TRACE_HEAP_CREATE
    TEID_CREATE_HEAP    = MAKE_TRACE_ID(HEAP_TRACE_BASE,  1),   // The creation of heap
#endif

#ifdef TRACE_HEAP_REMOVE
    TEID_REMOVE_HEAP    = MAKE_TRACE_ID(HEAP_TRACE_BASE,  2),   // The removing of heap
#endif

#ifdef TRACE_HEAP_ALLOC
    TEID_ALLOC_HEAP     = MAKE_TRACE_ID(HEAP_TRACE_BASE,  3),   // heap data allocation
#endif

#ifdef TRACE_HEAP_FREE
    TEID_FREE_HEAP      = MAKE_TRACE_ID(HEAP_TRACE_BASE,  4),   // heap data free
#endif

#endif /*MX_HEAP_ENABLED*/

/*******************************************/
/* The "RHEAP" objects' trace events        */
/*******************************************/

#ifdef MX_RHEAP_ENABLED

#ifdef TRACE_RHEAP_CREATE
    TEID_CREATE_RHEAP    = MAKE_TRACE_ID(RHEAP_TRACE_BASE,  1),   // The creation of heap
#endif

#ifdef TRACE_RHEAP_REMOVE
    TEID_REMOVE_RHEAP    = MAKE_TRACE_ID(RHEAP_TRACE_BASE,  2),   // The removing of heap
#endif

#ifdef TRACE_RHEAP_ALLOC
    TEID_ALLOC_RHEAP     = MAKE_TRACE_ID(RHEAP_TRACE_BASE,  3),   // heap data allocation
#endif

#ifdef TRACE_RHEAP_FREE
    TEID_FREE_RHEAP      = MAKE_TRACE_ID(RHEAP_TRACE_BASE,  4),   // heap data free
#endif

#endif /*MX_RHEAP_ENABLED*/


/*******************************************/
/* The "LIST" objects' trace events        */
/*******************************************/

#ifdef MX_LIST_ENABLED

#ifdef TRACE_LIST_CREATE
    TEID_CREATE_LIST    = MAKE_TRACE_ID(LIST_TRACE_BASE,  1),   // The creation of the list object
#endif

#ifdef TRACE_LIST_REMOVE
    TEID_REMOVE_LIST    = MAKE_TRACE_ID(LIST_TRACE_BASE,  2),   // Removing of the list object
#endif

#ifdef TRACE_LIST_ADD_ELM
    TEID_ADD_ELM_LIST   = MAKE_TRACE_ID(LIST_TRACE_BASE,  3),   // New one element was add to the end of list
#endif

#ifdef TRACE_LIST_DEL_ELM
    TEID_DEL_ELM_LIST   = MAKE_TRACE_ID(LIST_TRACE_BASE,  4),   // New one element was removed from the list
#endif

#ifdef TRACE_LIST_EMPTY
    TEID_EMPTY_LIST     = MAKE_TRACE_ID(LIST_TRACE_BASE,  5),   // The list was forced empty
#endif

#ifdef TRACE_LIST_INSERT
    TEID_INSERT_LIST    = MAKE_TRACE_ID(LIST_TRACE_BASE,  6),   // The new one elm was inserted into the list
#endif

#endif /*MX_LIST_ENABLED*/


    TEID_USER_APP       = USR_APP_TRACE_BASE   // The lat element of enum type
}TraceEventID;


/************************************************************/
/**       @brief The type trace element header              */
/************************************************************/

typedef UINT8 TTRACEID;
typedef UINT8 TTRACESIZE;

typedef struct _TRACE_HEADER_
{
    TTRACEID   ID;         // The trace event ID (see: TraceEventID type )
    TTRACESIZE Size;       // The data size, the range is [0 ... )

    // The trace event's  data are placed after header

}TraceHeader;


/************************************************************/
/**       @brief The common data of thread trace events     */
/************************************************************/
typedef struct _TRACE_DATA_THR_
{
    MXRC    Rc;         // The error code of operation
    HANDLE  hThr;       // The thread descriptor
}TraceDataThread;

/************************************************************/
/**       @brief The thread trace priority event            */
/************************************************************/
typedef struct _TRACE_DATA_THR_PRIOR_
{
    MXRC        Rc;         // The error code of operation
    HANDLE      hThr;       // The thread descriptor
    TPRIORITY   Prior;      // New thread priority
}TraceDataThreadPrior;


/************************************************************/
/**       @brief The common data of event trace events      */
/************************************************************/
typedef struct _TRACE_DATA_EVENT_
{
    MXRC    Rc;         // The error code of operation
    HANDLE  hEvent;     // The event descriptor
}TraceDataEvent;


/************************************************************/
/**       @brief The common data of event trace events      */
/************************************************************/
typedef struct _TRACE_DATA_TIMER_
{
    MXRC    Rc;         // The error code of operation
    HANDLE  hTimer;     // The obj descriptor
}TraceDataTimer;


/************************************************************/
/**       @brief The common data of part trace events      */
/************************************************************/
typedef struct _TRACE_PART_CTRL_
{
    MXRC    Rc;         // The error code of operation
    HANDLE  hPart;      // The obj descriptor
}TracePartCtrl;

typedef struct _TRACE_PART_DATA_
{
    HANDLE  hPart;      // The obj descriptor
    UINT32  Data;       // The pointer (for alloc and free operations)
}TracePartData;

/************************************************************/
/**       @brief The common data of queue trace events      */
/************************************************************/
typedef struct _TRACE_QUEUE_CTRL_
{
    MXRC    Rc;         // The error code of operation
    HANDLE  hQueue;     // The obj descriptor
}TraceQueueCtrl;

typedef struct _TRACE_QUEUE_DATA_
{
    HANDLE  hQueue;     // The obj descriptor
    UINT32  Data;       // The pointer to data that placed into the queue
    MXRC    rc;         // The error code of operation
}TraceQueueData;

/************************************************************/
/**       @brief The common data of heap trace events       */
/************************************************************/
typedef struct _TRACE_HEAP_CTRL_
{
    MXRC    Rc;         // The error code of operation
    HANDLE  hHeap;      // The obj descriptor
}TraceHeapCtrl;

typedef struct _TRACE_HEAP_DATA_
{
    HANDLE  hHeap;      // The obj descriptor
    UINT32  Data;       // The pointer to the alloc/dealloc memory
    UINT32  Size;       // The size of mem block in bytes
}TraceHeapData;

/************************************************************/
/**       @brief The common data of heap trace events       */
/************************************************************/
typedef struct _TRACE_RHEAP_CTRL_
{
    MXRC    Rc;         // The error code of operation
    HANDLE  hHeap;      // The obj descriptor
}TraceRHeapCtrl;

typedef struct _TRACE_RHEAP_DATA_
{
    HANDLE  hHeap;      // The obj descriptor
    UINT32  Data;       // The pointer to the alloc/dealloc memory
    UINT32  Size;       // The size of mem block in bytes
}TraceRHeapData;

/************************************************************/
/**       @brief The common data of the list trace events   */
/************************************************************/
typedef struct _TRACE_LIST_CTRL_
{
    MXRC    Rc;         // The error code of operation
    HANDLE  hList;      // The obj descriptor
    UINT32  Type;       // Single/Double linked list
}TraceListCtrl;

typedef struct _TRACE_LIST_AR_ELM_
{
    HANDLE  hList;     // The obj descriptor
    UINT32  ElmAddr;  // The address of the list element
}TraceListAddRemElm;

typedef struct _TRACE_LIST_INS_ELM_
{
    HANDLE  hList;      // The obj descriptor
    UINT32  Ctrl;       // The ID of operation: begin/end insertion
    UINT32  Base;       // The address of the reference element
    UINT32  NewElm;     // The address of the list element
}TraceListInsElm;




/** @brief The function adds information to the system debug trace
    @param pCtx  [in] - the system context
    @param id    [in] - the elenent ID
    @param pData [in] - the pointer to the element data (it can be NULL)
    @param nSize [in] - the data size (0 if pData is NULL)

    @return [MXRC] error code */

MXRC MxWriteDebugTrace (PMXCONTEXT pCtx, TraceEventID id, LPVOID pData, UINT8 nSize);

/** @brief The function reads information of the system debug trace
    @param pCtx  [in]     - the system context
    @param pID   [out]    - the element ID
    @param pData [out]    - the pointer to the element data
    @param pnSize[in/out] - the data size

    @return [MXRC] error code */

MXRC MxReadDebugTrace  (PMXCONTEXT pCtx, TraceEventID * pID, LPVOID pData, PUINT8 pnSize);

#ifdef __cplusplus
}
#endif

#endif /*_MX_DEBUG*/

#endif /*_4GMX_DEBUG_H_*/

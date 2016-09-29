/** @file hostiface.c
 *
 * @brief Host Interface message exchange
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_RTXC

#include <string.h>
#include "hostiface.h"
#include "osal.h"
#include "ctrlmsg.h"

//
// Pointers to actual functions
//
RESULTCODE (*HostSendMsg)(U32 msgSize, PUNKNOWN data) = NULL;
void (*HostRxTask)(PUNKNOWN data) = NULL;

//
// Task IDs
//
OSAL_SEMAPHORE HostRxSema;
OSAL_SEMAPHORE HostTxSema;

//
// Events
//
OSAL_TASK HostRxTaskID;
OSAL_TASK HostTxTaskID;

//
// Message Queues
//
OSAL_QUEUE HostRxQueue;
OSAL_QUEUE HostTxQueue;

//
// Storage area
//
OSAL_MEMARRAY HostTx;
OSAL_MEMARRAY HostRx;
U8 HostTxMem[MSG_MAXSIZE*HIF_MAXMSGS] __attribute__((aligned(32)));
U8 HostRxMem[MSG_MAXSIZE*HIF_MAXMSGS] __attribute__((aligned(32)));

//
// Forward declarations
//
RESULTCODE HostSendMsgQueue(U32 msgSize, PUNKNOWN data);
void HostRxTaskQueue(PUNKNOWN data);

/** Initialize Host Interface
 *
 * @return Standard result code
 */
RESULTCODE HostInit(void)
{
    if (OsalCreateSema(&HostRxSema) != SUCCESS)
    {
        return FAILURE;
    }
    if (OsalCreateSema(&HostTxSema) != SUCCESS)
    {
        return FAILURE;
    }

    if (OsalCreateMemArray(&HostTx, HostTxMem, MSG_MAXSIZE*HIF_MAXMSGS,
                           MSG_MAXSIZE) != SUCCESS)
    {
        return FAILURE;
    }

    if (OsalCreateMemArray(&HostRx, HostRxMem, MSG_MAXSIZE*HIF_MAXMSGS,
                           MSG_MAXSIZE) != SUCCESS)
    {
        return FAILURE;
    }

    if (OsalCreateTask(&HostTxTaskID, HostTxTask, NULL, "HostTxTask",
                       OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
    {
        return FAILURE;
    }
    OsalActivateTask(&HostTxTaskID);

    HostRxTask = HostRxTaskQueue;
    if (OsalCreateTask(&HostRxTaskID, HostRxTask, NULL, "HostRxTask",
                       OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
    {
        return FAILURE;
    }
    OsalActivateTask(&HostRxTaskID);

    HostSendMsg = HostSendMsgQueue;

    return SUCCESS;
}

/** Stop Host Interface
 *
 * @return Standard result code
 */
RESULTCODE HostDone(void)
{
    if (HostRxTaskID)
    {
        OsalDestroyTask(&HostRxTaskID);
        OsalDestroySema(&HostRxSema);
//      OsalDestroyMemArray(&HostRx);
    }
    if (HostTxTaskID)
    {
        OsalDestroyTask(&HostTxTaskID);
        OsalDestroySema(&HostTxSema);
//      OsalDestroyMemArray(&HostTx);
    }

    return SUCCESS;
}

/** This task receives messages from host (from transport interface)
 *
 * @param data Task default parameter
 */
void HostRxTaskQueue(PUNKNOWN data)
{
    U32 rxBuffer[MSG_MAXSIZE/sizeof(U32)];

    while (1)
    {
        if (OsalWaitForSema(&HostRxSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            if (MsgDispatch(MSGS_HOST, 0, rxBuffer) != SUCCESS)
            {
                // TODO: handle dispatch failure
            }
        }
    }
}

/** This task sends control messages to host (to transport interface)
 *
 * @param data Task default parameter
 */
void HostTxTask(PUNKNOWN data)
{
    OSAL_PADDRESS pQueueElem;

    while (1)
    {
        if (OsalWaitForSema(&HostTxSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            while (OsalDequeue(&HostTxQueue, &pQueueElem) == SUCCESS)
            {
                // TODO: send to lower level
                OsalFreeMemArrayBlock(&HostTx, pQueueElem);
            }
        }
    }
}

/** Send message to the Host
 *
 * @param msgSize Size of message in bytes
 * @param data Pointer to the data buffer
 * @return Standard result code
 */
RESULTCODE HostSendMsgQueue(U32 msgSize, PUNKNOWN data)
{
    OSAL_PADDRESS pBlock;

    if (msgSize > MSG_MAXSIZE)
        return FAILURE;

    // TODO: Use memory copy or enqueue data pointer?
    if (OsalAllocMemArrayBlock(&HostTx, &pBlock) != SUCCESS)
    {
        return FAILURE;
    }
    memcpy(pBlock, data, msgSize);

    if (OsalEnqueue(&HostTxQueue, pBlock) != SUCCESS)
    {
        return FAILURE;
    }
    OsalRaiseSema(&HostTxSema);

    return SUCCESS;
}

#endif /* USE_RTXC */

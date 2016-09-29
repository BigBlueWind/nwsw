/** @file hostiface.c
 *
 * @brief Host Interface message exchange
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_WINDOWS

#include "hostiface.h"
#include "osal.h"
#include "msgroute.h"
#include "icpu.h"

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
// Pointers to actual functions
//
RESULTCODE (*HostSendMsg)(U32 msgSize, PTR data) = NULL;
void (*HostRxTask)(PTR data) = NULL;

//
// Storage area
//
OSAL_MEMARRAY HostTx;
OSAL_MEMARRAY HostRx;
U8 HostTxMem[MSG_MAXSIZE*HIF_MAXMSGS];
U8 HostRxMem[MSG_MAXSIZE*HIF_MAXMSGS];

//
// External dependencies
//
extern int IcpuMode;
extern RESULTCODE IcpuSendShm(U32 size, PTR data);

//
// Forward declarations
//
RESULTCODE HostSendMsgQueue(U32 msgSize, PTR data);
void HostRxTaskQueue(PTR data);

/** Initialize Host Interface
 *
 * @return Standard result code
 */
RESULTCODE HostInit(void)
{
    if (IcpuMode == IMODE_SHM)
    {
        if (HostSendMsg == NULL)
        {
            printf(C4KFW "HIF in shared memory mode\n");
            HostSendMsg = IcpuSendShm;
        }
    }
    else if (IcpuMode == IMODE_DIRECT)
    {
#if defined (_USRDLL) && defined (UE)
        if (HostSendMsg == NULL)
        {
            HostSendMsg = IcpuTxPut;
        }
#endif        
    }
    else if (IcpuMode == IMODE_HBI)
    {
#if defined (UE) && !defined(_USRDLL)
        // TODO
#endif
    }
    return SUCCESS;
}

/** Stop Host Interface
 *
 * @return Standard result code
 */
RESULTCODE HostDone(void)
{
    printf(C4KFW "Stopping Host interface... ");
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
    printf("Done\n");
    return SUCCESS;
}

/** This task receives messages from host (from transport interface)
 *
 * @param data Task default parameter
 */
void HostRxTaskQueue(PTR data)
{
    U32 rxBuffer[MSG_MAXSIZE/sizeof(U32)];

    while (1)
    {
        if (OsalWaitForSema(&HostRxSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            // TODO: read message
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
void HostTxTask(PTR data)
{
    OSAL_PADDRESS pQueueElem;

    while (1)
    {
        if (OsalWaitForSema(&HostTxSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            while (OsalDequeue(&HostTxQueue, &pQueueElem) == SUCCESS)
            {
                // TODO: send to lower level
                OsalFreeMemArray(&HostTx, pQueueElem);
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
RESULTCODE HostSendMsgQueue(U32 msgSize, PTR data)
{
    OSAL_PADDRESS pBlock;

    if (msgSize > MSG_MAXSIZE)
    {
        return FAILURE;
    }

    // TODO: Use memory copy or enqueue data pointer?
    if (OsalAllocMemArray(&HostTx, &pBlock) != SUCCESS)
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

#endif /* USE_WINDOWS */

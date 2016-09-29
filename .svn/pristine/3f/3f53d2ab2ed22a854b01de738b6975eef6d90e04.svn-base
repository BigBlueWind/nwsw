/** @file hostiface.c
 *
 * @brief Host Interface message exchange
 * @author Mindspeed Technologies
 * @version $Revision: 1.16 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_LINUX

#include "hostiface.h"
#include "osal.h"
#include "ctrlmsg.h"
#include "diags.h"
#include "settings.h"
#include "msgroute.h"
#include <string.h>

//
// Constants
//
// Used in RELEASE mode
#define HIF_RXQUEUE                 "/fromhost"
#define HIF_TXQUEUE                 "/tohost"

// Used in DEMO mode
#define HIF_RXQUEUED                "/toupper"
#define HIF_TXQUEUED                "/fromupper"

//
// Task IDs
//
OSAL_TASK HostRxTaskID = 0;

//
// Message Queues
//
OSAL_MAILBOX HostRxMbox = 0;
OSAL_MAILBOX HostTxMbox = 0;
extern OSAL_MEMARRAY MsgArray;

//
// Pointers to actual functions
//
RESULTCODE (*HostSendMsg)(U32 msgSize, PTR data) = NULL;
void (*HostRxTask)(PTR data) = NULL;

//
// Global mode of operation
// 0 - RELEASE, 1 - DEMO
//
int HostMode = HMODE_RELEASE;

//
// Forward declarations
//
RESULTCODE HostSendMsgMailBox(U32 msgSize, PTR data);
void HostRxTaskMailBox(PTR data);

/** Initialize Host Interface
 *
 * @return Standard result code
 */
RESULTCODE HostInit(void)
{
    char const *hostIP = "127.0.0.1";

    //
    // First check if we explicitly forced to use network connection
    //
    if (ReadSettingStr("hostip", &hostIP, hostIP) == SUCCESS)
    {
        // String was specified in settings
        HostMode = HMODE_FORCE_NET;
    }

    // To prevent usage of mailbox queues
    if (HostMode == HMODE_FORCE_NET)
    {
        if (HostSendMsg == NULL)
        {
            // Hopefully ProtoInit() will take care of this
            return SUCCESS;
        }
        else
        {
            DiagPrint(DIAG_LEVEL1, "HOST", "Mode already set\n");
            // Fall back to default mode
            HostMode = HMODE_RELEASE;
        }
    }
    
    //
    // Check if we are running in DEMO mode
    //
    if (HostSendMsg == NULL)
    {
        if (OsalCreateMailBox(&HostRxMbox, HIF_RXQUEUED, 0, 0, NULL, FALSE) == SUCCESS)
        {
            if (OsalCreateMailBox(&HostTxMbox, HIF_TXQUEUED, 0, 0, NULL, FALSE) == SUCCESS)
            {
                HostMode = HMODE_DEMO;
                HostRxTask = HostRxTaskMailBox;
                HostSendMsg = HostSendMsgMailBox;
                DiagPrint(DIAG_LEVEL2, "HOST", "Using MBOX DEMO mode\n");
            }
            else
            {
                DiagPrint(DIAG_LEVEL1, "HOST", "Unable to create TX mailbox\n");
                OsalDestroyMailBox(&HostRxMbox, HIF_RXQUEUED);
            }
        }
    }

    //
    // Next, check if we are running in release mode
    //
    if (HostSendMsg == NULL)
    {
        if (HostMode == HMODE_RELEASE)
        {
            if (OsalCreateMailBox(&HostRxMbox, HIF_RXQUEUE, 0, 0, NULL, FALSE)
                != SUCCESS)
            {
                DiagPrint(DIAG_LEVEL0, "HOST", "Unable to open RX queue [%i]\n", errno);
                return FAILURE;
            }
            if (OsalCreateMailBox(&HostTxMbox, HIF_TXQUEUE, 0, 0, NULL, FALSE)
                != SUCCESS)
            {
                OsalDestroyMailBox(&HostRxMbox, HIF_RXQUEUE);
                DiagPrint(DIAG_LEVEL0, "HOST", "Unable to open TX queue [%i]\n", errno);
                return FAILURE;
            }
            HostRxTask = HostRxTaskMailBox;
            HostSendMsg = HostSendMsgMailBox;
            DiagPrint(DIAG_LEVEL2, "HOST", "Using MBOX mode\n");
        }
    }

    if (HostRxTask)
    {
    	OsalCreateTask(&HostRxTaskID, HostRxTask, NULL, "HostRxTask", OSAL_PRIORITY_NORMAL, 0, 0);
		OsalActivateTask(&HostRxTaskID);
    }

    return SUCCESS;
}

/** Stop Host interface
 *
 * @return Standard result code
 */
RESULTCODE HostDone(void)
{
    if (HostRxTaskID)
    {
        OsalDestroyTask(&HostRxTaskID);
        HostRxTaskID = 0;
    }
    if (HostRxMbox)
    {
        OsalDestroyMailBox(&HostRxMbox, HIF_RXQUEUED);
        HostRxMbox = 0;
    }
    if (HostTxMbox)
    {
        OsalDestroyMailBox(&HostTxMbox, HIF_TXQUEUED);
        HostTxMbox = 0;
    }
    return SUCCESS;
}

/** This task receives messages from host (from transport interface)
 *
 * @param data Task default parameter
 */
void HostRxTaskMailBox(PTR data)
{
    PTR rxBuffer;
    OSAL_SIZE msgSize;

    while (1)
    {
        if (OsalAllocMemArray(&MsgArray, &rxBuffer) == SUCCESS)
        {
            msgSize = MSG_MAXSIZE;
            if (OsalGetMsg(&HostRxMbox, rxBuffer, &msgSize) == SUCCESS)
            {
                MsgDispatch(MSGS_HOST, msgSize, rxBuffer);
            }
            else
            {
                DiagPrint(DIAG_LEVEL4, "HOST", "MBOX RX failure [%i]\n", errno);
                OsalFreeMemArray(&MsgArray, rxBuffer);
                break;
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
RESULTCODE HostSendMsgMailBox(U32 msgSize, PTR data)
{
    RESULTCODE ret = FAILURE;

    if ((HostTxMbox != 0) && (msgSize <= MSG_MAXSIZE))
    {
        ret = OsalSendMsg(&HostTxMbox, data, msgSize);
    }
    OsalFreeMemArray(&MsgArray, data);

    return ret;
}

#endif /* USE_LINUX */

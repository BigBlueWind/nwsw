/** @file auxiface.c
 *
 * @brief AUX interface message exchange
 * @author Mindspeed Technologies
 * @version $Revision: 1.20 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_LINUX

#include "osal.h"
#include "auxiface.h"
#include "msgroute.h"
#include "settings.h"
#include "icpu.h"
#include "maciface.h"
#include "transport.h"
#include "diags.h"

//
// Constants
//
#define AUX_RXQUEUE                 "/auxout"
#define AUX_TXQUEUE                 "/auxin"

//
// Tasks
//
OSAL_TASK AuxRxTaskID;
OSAL_TASK AuxTxTaskID;

//
// Message Queues
//
OSAL_MAILBOX AuxRxMbox;
OSAL_MAILBOX AuxTxMbox;
extern OSAL_MEMARRAY MsgArray;

//
// Pointers to actual send function
//
extern RESULTCODE (*HostSendMsg)(U32 msgSize, PTR data);
RESULTCODE (*AuxSendMsg)(U32 msgSize, PTR data) = NULL;

void AuxRxTask(PTR data);

//
// Globals
//
int AuxMode = AUX_NOTUSED;

/** Initialize Host Interface
 *
 * @return Standard result code
 */
RESULTCODE AuxInit(void)
{
    int temp;

    // "useaux" is an old spelling
    ReadSettingInt("useaux", &AuxMode, AuxMode);
    // "aux" is a new one
    ReadSettingInt("aux", &temp, AuxMode);
    // New setting takes precedence over old
    if (temp != AuxMode)
        AuxMode = temp;

    if (AuxMode == AUX_CHAINED)
    {
        //
        // This app chained to another app
        //
        if (OsalCreateMailBox(&AuxRxMbox, AUX_TXQUEUE, 0, 0, NULL, FALSE) != SUCCESS)
        {
            fprintf(stderr, C4KFW "ERROR: Unable to open AUX Rx queue [%i]\n", errno);
            return FAILURE;
        }
        if (OsalCreateMailBox(&AuxTxMbox, AUX_RXQUEUE, 0, 0, NULL, FALSE) != SUCCESS)
        {
            fprintf(stderr, C4KFW "ERROR: Unable to open AUX Tx queue [%i]\n", errno);
            return FAILURE;
        }

        // Unconditionally change ICPU
        IcpuSendMsg = AuxSendMsgQueue;
        if (HostSendMsg == NULL)
        {
            HostSendMsg = AuxSendMsgQueue;
        }
        if (AuxSendMsg == NULL)
        {
            AuxSendMsg = MacDispatch;
        }
        printf(C4KFW "INFO: Using AUX interface\n");
    }
    else if (AuxMode == AUX_ORIGIN)
    {
        //
        // This app acts like chain origin
        //
        if (OsalCreateMailBox(&AuxRxMbox, AUX_RXQUEUE, MSG_MAXSIZE * AUX_MAXMSGS,
                              MSG_MAXSIZE, NULL, FALSE) != SUCCESS)
        {
            fprintf(stderr, C4KFW "ERROR: Unable to create AUX Rx queue [%i]\n", errno);
            return FAILURE;
        }
        if (OsalCreateMailBox(&AuxTxMbox, AUX_TXQUEUE, MSG_MAXSIZE * AUX_MAXMSGS,
                              MSG_MAXSIZE, NULL, FALSE) != SUCCESS)
        {
            fprintf(stderr, C4KFW "ERROR: Unable to create AUX Tx queue [%i]\n", errno);
            return FAILURE;
        }

        if (AuxSendMsg == NULL)
        {
            AuxSendMsg = AuxSendMsgQueue;
        }
    }
    else if (AuxMode == AUX_REMOTE)
    {
        int protocol;

        if (AuxSendMsg == NULL)
        {
            ReadSettingInt("proto", &protocol, PROTO_SCTP);
            if (protocol == PROTO_SCTP)
                AuxSendMsg = SctpSend;
            else if (protocol == PROTO_TCP)
                AuxSendMsg = TcpSend;
            else if (protocol == PROTO_UDP)
                AuxSendMsg = UdpSend;
        }
        if (IcpuSendMsg == NULL)
        {
            IcpuSendMsg = AuxSendMsg;
        }
        printf(C4KFW "INFO: Using AUX interface\n");
    }

    if (AuxSendMsg != NULL)
    {
        OsalCreateTask(&AuxRxTaskID, AuxRxTask, NULL, "AuxRxTask", OSAL_PRIORITY_NORMAL, 0, 0);
        OsalActivateTask(&AuxRxTaskID);
    }

    return SUCCESS;
}

/** This task receives messages from AUX interface
 *
 * @param data Task default parameter
 */
void AuxRxTask(PTR data)
{
    PTR rxBuffer;
    OSAL_SIZE msgSize;

    while (1)
    {
        if (OsalAllocMemArray(&MsgArray, &rxBuffer) == SUCCESS)
        {
            msgSize = MSG_MAXSIZE;
            if (OsalGetMsg(&AuxRxMbox, rxBuffer, &msgSize) == SUCCESS)
            {
            	DiagPrintApi(DIAG_LEVEL30, "AUX", 0, rxBuffer);
                MsgDispatch(MSGS_AUX, msgSize, rxBuffer);
            }
            else
            {
                DiagPrint(DIAG_LEVEL4, "AUX", "Message receive failure\n");
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
RESULTCODE AuxSendMsgQueue(U32 msgSize, PTR data)
{
    RESULTCODE ret = FAILURE;
    if (msgSize <= MSG_MAXSIZE)
    {
        ret = OsalSendMsg(&AuxTxMbox, data, msgSize);
    }
    OsalFreeMemArray(&MsgArray, data);
    return ret;
}

RESULTCODE AuxDone(void)
{
    if (AuxMode == AUX_CHAINED)
    {
        OsalDestroyMailBox(&AuxRxMbox, AUX_TXQUEUE);
        OsalDestroyMailBox(&AuxRxMbox, AUX_RXQUEUE);
    }
    else if (AuxMode == AUX_ORIGIN)
    {
        OsalDestroyMailBox(&AuxRxMbox, AUX_RXQUEUE);
        OsalDestroyMailBox(&AuxRxMbox, AUX_TXQUEUE);
    }

    return SUCCESS;
}

#endif /* USE_LINUX */


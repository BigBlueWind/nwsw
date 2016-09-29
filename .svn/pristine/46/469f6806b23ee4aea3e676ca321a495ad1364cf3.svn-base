/** @file auxiface.c
 *
 * @brief AUX interface message exchange (dummy for Windows)
 * @author Mindspeed Technologies
 * @version $Revision: 1.14 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_WINDOWS

#include "osal.h"
#include "auxiface.h"
#include "settings.h"
#include "apidefs.h"
#include "msgroute.h"
#include "maciface.h"
#include "transport.h"
#include "diags.h"

//
// Constants
//
#if defined (MSMAC) || defined (UE)
#define AUX_NAMERX                  "\\\\.\\pipe\\C4KAUXTX"
#define AUX_NAMETX                  "\\\\.\\pipe\\C4KAUXRX"
#else
#define AUX_NAMERX                  "\\\\.\\pipe\\C4KAUXRX"
#define AUX_NAMETX                  "\\\\.\\pipe\\C4KAUXTX"
#endif

//
// Tasks
//
OSAL_TASK AuxRxTaskID = 0;
OSAL_TASK AuxTxTaskID = 0;

//
// Message Queues
//
HANDLE AuxQueueRx = NULL;
HANDLE AuxQueueTx = NULL;

//
// Pointers to actual send function
//
extern RESULTCODE (*IcpuSendMsg)(U32 msgSize, PTR data);
extern RESULTCODE (*HostSendMsg)(U32 msgSize, PTR data);
RESULTCODE (*AuxSendMsg)(U32 msgSize, PTR data) = NULL;

void AuxRxTask(PTR data);
void AuxTxTask(PTR data);

//
// Globals
//
int AuxMode = AUX_NOTUSED;
int AuxRxConnected = 0;
int AuxTxConnected = 0;
extern OSAL_MEMARRAY MsgArray;

/** Initialize Host Interface
 *
 * @return Standard result code
 */
RESULTCODE AuxInit(void)
{
    int temp;
    DWORD mode = PIPE_READMODE_MESSAGE | PIPE_WAIT;

    // "useaux" is an old spelling
    ReadSettingInt("useaux", &AuxMode, AuxMode);
    // "aux" is a new one
    ReadSettingInt("aux", &temp, AuxMode);
    // New setting takes precedence over old
    if (temp != AuxMode)
        AuxMode = temp;

    if (AuxMode == AUX_REMOTE)
    {
        if (AuxSendMsg == NULL)
        {
            int proto;
            // TODO: Add protocol selection when available
            ReadSettingInt("proto", &proto, PROTO_SCTP);
            switch (proto)
            {
            case PROTO_TCP:
                AuxSendMsg = TcpSend;
                break;
            case PROTO_UDP:
                AuxSendMsg = UdpSend;
                break;
            case PROTO_SCTP:
                AuxSendMsg = SctpSend;
                break;
            default:
                AuxSendMsg = NULL;
                break;
            }
        }
        if (IcpuSendMsg == NULL)
        {
            IcpuSendMsg = AuxSendMsg;
        }
        DiagPrint(DIAG_LEVEL2, "AUX", "Enabled\n");
    }
    else if (AuxMode != AUX_NOTUSED)
    {
#if defined (ENODEB) || defined (BSMAC)
        //
        // RX pipe (server)
        //
        AuxQueueRx = CreateNamedPipe(AUX_NAMERX,
                                     PIPE_ACCESS_INBOUND,
                                     PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                     PIPE_UNLIMITED_INSTANCES,
                                     MSG_MAXSIZE * 6,
                                     MSG_MAXSIZE * 6,
                                     NMPWAIT_WAIT_FOREVER,
                                     NULL);
        if (AuxQueueRx == INVALID_HANDLE_VALUE)
        {
            DiagPrint(DIAG_LEVEL0, "ERROR", "Unable to create AUX IN interface [%u]\n", GetLastError());
            AuxQueueRx = NULL;
            return FAILURE;
        }

        //
        // TX pipe (server)
        //
        AuxQueueTx = CreateNamedPipe(AUX_NAMETX,
                                     PIPE_ACCESS_OUTBOUND,
                                     PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                     PIPE_UNLIMITED_INSTANCES,
                                     MSG_MAXSIZE * 6,
                                     MSG_MAXSIZE * 6,
                                     NMPWAIT_WAIT_FOREVER,
                                     NULL);
        if (AuxQueueTx == INVALID_HANDLE_VALUE)
        {
            DiagPrint(DIAG_LEVEL0, "ERROR", "Unable to create AUX OUT interface [%u]\n", GetLastError());
            AuxQueueTx = NULL;
            return FAILURE;
        }
#endif /* ENODEB || BSMAC */

#if defined (UE) || defined (MSMAC)
        //
        // This app chained to another app
        //
        DiagPrint(DIAG_LEVEL2, "AUX", "Connecting...\n");
        if (WaitNamedPipe(AUX_NAMETX, 5000) == 0)
        {
            DiagPrint(DIAG_LEVEL0, "ERROR", "AUX OUT interface not available [%i]\n", GetLastError());
            return FAILURE;
        }
        AuxQueueTx = CreateFile(AUX_NAMETX,
                                GENERIC_WRITE,
                                0,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL, NULL);
        if (AuxQueueTx == INVALID_HANDLE_VALUE)
        {
            DiagPrint(DIAG_LEVEL0, "ERROR", "Unable to open AUX OUT interface [%i]\n", GetLastError());
            return FAILURE;
        }
        SetNamedPipeHandleState(AuxQueueTx, &mode, NULL, NULL);
        AuxTxConnected = 1;

        if (WaitNamedPipe(AUX_NAMERX, 5000) == 0)
        {
            DiagPrint(DIAG_LEVEL0, "ERROR", "AUX IN interface not available [%i]\n", GetLastError());
            return FAILURE;
        }
        AuxQueueRx = CreateFile(AUX_NAMERX,
                                GENERIC_READ,
                                0,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL, NULL);
        if (AuxQueueRx == INVALID_HANDLE_VALUE)
        {
            DiagPrint(DIAG_LEVEL0, "ERROR", "Unable to open AUX IN interface [%i]\n", GetLastError());
            return FAILURE;
        }
        SetNamedPipeHandleState(AuxQueueRx, &mode, NULL, NULL);
        AuxRxConnected = 1;
#endif /* UE || MSMAC */
        if (AuxSendMsg == NULL)
        {
            AuxSendMsg = AuxSendMsgQueue;
            OsalCreateTask(&AuxRxTaskID, AuxRxTask, NULL, "AuxRxTask", OSAL_PRIORITY_NORMAL, 0, 0);
            OsalCreateTask(&AuxTxTaskID, AuxTxTask, NULL, "AuxTxTask", OSAL_PRIORITY_NORMAL, 0, 0);
            DiagPrint(DIAG_LEVEL2, "AUX", "Enabled\n");
        }
    }

    if (AuxRxTaskID)
    {
        OsalActivateTask(&AuxRxTaskID);
    }
    if (AuxTxTaskID)
    {
        OsalActivateTask(&AuxTxTaskID);
    }

    return SUCCESS;
}

/**
 *
 * @param data Task default parameter
 */
void AuxRxTask(PTR data)
{
    PTR rxBuffer;
    DWORD bytesRead;

    // Wait forever for client connection
#if defined (ENODEB) || defined (BSMAC)
    if (ConnectNamedPipe(AuxQueueRx, NULL) != 0)
#endif
    {
        // Connected
        AuxRxConnected = 1;
        DiagPrint(DIAG_LEVEL6, "AUX", "RX Client connected\n");

        while (1)
        {
            if (OsalAllocMemArray(&MsgArray, &rxBuffer) == SUCCESS)
            {
                if (ReadFile(AuxQueueRx, rxBuffer, MSG_MAXSIZE, &bytesRead, NULL) != 0)
                {
                    MsgDispatch(MSGS_AUX, bytesRead, rxBuffer);
                }
                else
                {
                    // Error occured, exit task
                    OsalFreeMemArray(&MsgArray, rxBuffer);
                    break;
                }
            }
        }
    }
}

void AuxTxTask(PTR data)
{
    // Wait forever for client connection
    if (ConnectNamedPipe(AuxQueueTx, NULL) != 0)
    {
        // Connected
        AuxTxConnected = 1;
        DiagPrint(DIAG_LEVEL6, "AUX", "TX Client connected\n");
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
    DWORD written;

    if (AuxQueueTx && AuxTxConnected)
    {
        if (msgSize <= MSG_MAXSIZE)
        {
            if (WriteFile(AuxQueueTx, data, msgSize, &written, NULL) != 0)
            {
                ret = SUCCESS;
            }
        }
    }
    OsalFreeMemArray(&MsgArray, data);
    return ret;
}

RESULTCODE AuxDone(void)
{
    DiagPrint(DIAG_LEVEL6, "AUX", "Stopping interface...\n");
    if (AuxQueueRx)
    {
        CancelIo(AuxQueueRx);
#if defined (ENODEB) || defined (BSMAC)
        if (AuxRxConnected)
        {
            DisconnectNamedPipe(AuxQueueRx);
        }
#else
        // FIXME: This caused app to hang
//      CloseHandle(AuxQueueRx);
#endif
        AuxQueueRx = NULL;
    }
    if (AuxQueueTx)
    {
        CancelIo(AuxQueueTx);
#if defined (ENODEB) || defined (BSMAC)
        if (AuxTxConnected)
        {
            DisconnectNamedPipe(AuxQueueTx);
        }
#else
        // FIXME: This caused app to hang
//      CloseHandle(AuxQueueTx);
#endif
        AuxQueueTx = NULL;
    }
    if (AuxRxTaskID)
    {
        OsalDeactivateTask(&AuxRxTaskID);
        OsalDestroyTask(&AuxRxTaskID);
        AuxRxTaskID = 0;
    }
    if (AuxTxTaskID)
    {
        OsalDestroyTask(&AuxTxTaskID);
        AuxTxTaskID = 0;
    }
    DiagPrint(DIAG_LEVEL6, "AUX", "Stopped\n");
    return SUCCESS;
}

#endif /* USE_WINDOWS */

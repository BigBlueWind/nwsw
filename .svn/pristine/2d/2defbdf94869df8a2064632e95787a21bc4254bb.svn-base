/** @file protocol.c
 *
 * @brief SCTP/TCP/UDP transport for Windows
 * @author Mindspeed Technologies
 * @version $Revision: 1.45 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_WINDOWS

#include "osal.h"
#include "bsp.h"
#include "sctplib/sctp.h"
#include "sctplib/globals.h"
#include "settings.h"
#include "hostiface.h"
#include "msgroute.h"
#include "transport.h"
#include "maciface.h"
#include "diags.h"
#include "svsr.h"

#define PROTO                       IPPROTO_SCTP
#define C4K_PORT                    32
#define UDP_MAX_FRAME               1400

//
// Host address
//
unsigned char HostIPAddr[SCTP_MAX_IP_LEN];
unsigned int HostPort;
struct sockaddr_in hostAddr;
struct sockaddr_in UdpHostAddr;
OSAL_SEMAPHORE SEMA_UDP_RX;
OSAL_MEMARRAY UdpMsgArray;

//
// Local address
//
unsigned char LocalIPAddr[1][SCTP_MAX_IP_LEN];

//
// Globals
//
int ProtoSocket = -1;
int ProtoSocketUdp = -1;
OSAL_TASK ProtoRxTaskID = NULL;
OSAL_TASK UdpRxTaskID = NULL;
SCTP_ulpCallbacks SctpCB;
unsigned short SctpInstanceID;
CRITICAL_SECTION ProtoMutex;
int Protocol = 0;
extern OSAL_MEMARRAY MsgArray;
HANDLE ProtoReady = NULL;

UdpSendMsgCB UdpSendMsg = NULL;

//
// Forward declarations
//
RESULTCODE (*ProtoSend)(U32 size, PTR data) = NULL;
RESULTCODE SctpSend(U32 size, PTR data);
RESULTCODE TcpSend(U32 size, PTR data);
RESULTCODE UdpSend(U32 size, PTR data);
void UdpFrameFree(PTR Packet);
void SctpRxTask(PTR taskParm);
void TcpRxTask(PTR taskParm);
void UdpRxTask(PTR taskParm);

void SctpLock(void *data);
void SctpUnlock(void *data);

//
// Callbacks
//
void SctpDataArriveCB(unsigned int assocID, unsigned short streamID, unsigned int len,
                      unsigned short streamSN, unsigned int TSN, unsigned int protoID,
                      unsigned int unordered, void *ulpData)
{
    unsigned int buffer[MSG_MAXSIZE/sizeof(int)];
    PTR rxBuffer;
    PTR ptr;

    if (OsalAllocMemArray(&MsgArray, &rxBuffer) == SUCCESS)
    {
        ptr = rxBuffer;
    }
    else
    {
        // Receive in any case but don't dispatch
        ptr = (PTR) buffer;
        rxBuffer = NULL;
    }
    if (sctp_receive(assocID, streamID, ptr, &len, &streamSN, &TSN,
                     SCTP_MSG_DEFAULT) == SCTP_SUCCESS)
    {
        if (rxBuffer)
        {
            MsgDispatch(MSGS_HOST, len, rxBuffer);
        }
        else
        {
            printf("SCTP: Unable to allocate message buffer\n");
        }
    }
}

void SctpSendFailureCB(unsigned int assocID, unsigned char *notSent,
                       unsigned int length, unsigned int *context, void *ulpData)
{
    printf(C4KFW "SCTP: Send failure\n");
}

void SctpNetworkStatusChangeCB(unsigned int assocID, short index, unsigned short newPath,
                               void *ulpData)
{
    printf(C4KFW "SCTP: Network status changed [%i]\n", newPath);
    switch (newPath)
    {
        case SCTP_PATH_OK:
            if (ProtoReady != NULL)
            {
                ReleaseSemaphore(ProtoReady, 1, NULL);
            }
            break;
        case SCTP_PATH_UNREACHABLE:
            break;
        case SCTP_PATH_ADDED:
            break;
        case SCTP_PATH_REMOVED:
            break;
        case SCTP_PATH_CONFIRMED:
            break;
        case SCTP_PATH_UNCONFIRMED:
            break;
    }
}

void* SctpCommunicationUpCB(unsigned int assocID, int status, unsigned int destAddr,
                            unsigned short inStreams, unsigned short outStreams,
                            int supportPRSCTP, void *ulpData)
{
    printf(C4KFW "SCTP: Communication established [%i]\n", status);
    ProtoSocket = assocID;
    return NULL;
}

void SctpCommunicationLostCB(unsigned int assocID, unsigned short status, void *ulpData)
{
    printf(C4KFW "SCTP: Communication lost [%i]\n", status);
    if (assocID == (unsigned int) ProtoSocket)
    {
        ProtoSocket = -1;
    }
    sctp_deleteAssociation(assocID);
}

void SctpCommunicationErrorCB(unsigned int assocID, unsigned short status, void *ulpData)
{
    printf(C4KFW "SCTP: Communication error [%i]\n", status);
}

void SctpRestartCB(unsigned int assocID, void *ulpData)
{
    printf(C4KFW "SCTP: Restart requested\n");
}

void SctpPeerShutdownReceivedCB(unsigned int assocID, void *ulpData)
{
    printf(C4KFW "SCTP: Peer shutdown received\n");
    if (assocID == (unsigned int) ProtoSocket)
    {
        ProtoSocket = -1;
    }
    sctp_deleteAssociation(assocID);
}

void SctpShutdownCompleteCB(unsigned int assocID, void *ulpData)
{
    printf(C4KFW "SCTP: Shutdown completed\n");
    if (assocID == (unsigned int) ProtoSocket)
    {
        ProtoSocket = -1;
    }
    sctp_deleteAssociation(assocID);
}

void SctpQueueStatusChangeCB(unsigned int assocID, int queueType, int queueID,
                             int queueLength, void *ulpData)
{
    printf(C4KFW "SCTP: Queue status changed [%i,%i,%i]\n", queueType, queueID, queueLength);
}

void SctpAsconfStatusCB(unsigned int assocID, unsigned int correlationID,
                        int result, void *tempStruct, void *ulpData)
{
    printf(C4KFW "SCTP: ASCONF status [%i,%i]\n", correlationID, result);
}

/** Initialize CTRL interface
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
RESULTCODE ProtoInit(U32 protoid)
{
    int ret;
    char *tmpPtr;
    SCTP_LibraryParameters params;
    WSADATA wsaData;

    ProtoReady = CreateSemaphore(NULL, 0, 3, NULL);

    ReadSettingInt("proto", &Protocol, protoid);
    switch (Protocol)
    {
    case IPPROTO_TCP:
        WSAStartup(MAKEWORD(2,2), &wsaData);
        ProtoSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        //
        // Host IP address
        //
        ReadSettingStr("hostip", &tmpPtr, "0.0.0.0");
        strncpy(HostIPAddr, tmpPtr, SCTP_MAX_IP_LEN);

        //
        // Host port
        //
        ReadSettingInt("hostport", &HostPort, C4K_PORT);

        hostAddr.sin_family = AF_INET;
        hostAddr.sin_addr.s_addr = inet_addr(HostIPAddr);
        hostAddr.sin_port = htons((unsigned short)HostPort);

        if (HostSendMsg == NULL)
        {
            // Connect to the host
            if (connect(ProtoSocket, (SOCKADDR*) &hostAddr, sizeof(hostAddr)) == SOCKET_ERROR)
            {
                DiagPrint(DIAG_LEVEL0, "TCP", "Connection error [%i]\n", WSAGetLastError());
                closesocket(ProtoSocket);
                ProtoSocket = -1;
                return FAILURE;
            }

            DiagPrint(DIAG_LEVEL2, "TCP", "Enabled\n");
            HostSendMsg = TcpSend;
            ProtoSend = TcpSend;

            OsalCreateTask(&ProtoRxTaskID, TcpRxTask, NULL, "ProtoRxTask", OSAL_PRIORITY_NORMAL, NULL, 0);
            OsalActivateTask(&ProtoRxTaskID);
        }
        else
        {
            DiagPrint(DIAG_LEVEL1, "TCP", "Inactive\n");
            return FAILURE;
        }
        break;
    case IPPROTO_SCTP:
        InitializeCriticalSection(&ProtoMutex);
        if ((ret = sctp_initLibrary()) != 0)
        {
            DiagPrint(DIAG_LEVEL0, "SCTP", "Library init error [%i]\n", ret);
            return FAILURE;
        }

        sctp_getLibraryParameters(&params);
        params.sendOotbAborts = FALSE;
        params.checksumAlgorithm = SCTP_CHECKSUM_ALGORITHM_CRC32C;
        sctp_setLibraryParameters(&params);

        SctpCB.dataArriveNotif          = &SctpDataArriveCB;
        SctpCB.sendFailureNotif         = &SctpSendFailureCB;
        SctpCB.networkStatusChangeNotif = &SctpNetworkStatusChangeCB;
        SctpCB.communicationUpNotif     = &SctpCommunicationUpCB;
        SctpCB.communicationLostNotif   = &SctpCommunicationLostCB;
        SctpCB.communicationErrorNotif  = &SctpCommunicationErrorCB;
        SctpCB.restartNotif             = &SctpRestartCB;
        SctpCB.peerShutdownReceivedNotif= &SctpPeerShutdownReceivedCB;
        SctpCB.shutdownCompleteNotif    = &SctpShutdownCompleteCB;
        SctpCB.queueStatusChangeNotif   = &SctpQueueStatusChangeCB;
        SctpCB.asconfStatusNotif        = &SctpAsconfStatusCB;

        //
        // Host IP address
        //
        ReadSettingStr("hostip", &tmpPtr, "0.0.0.0");
        strncpy(HostIPAddr, tmpPtr, SCTP_MAX_IP_LEN);

        //
        // Host port
        //
        ReadSettingInt("hostport", &HostPort, C4K_PORT);

        //
        // Local IP address
        //
        ReadSettingStr("localip", &tmpPtr, "0.0.0.0");  // INADDR_ANY
        strncpy(LocalIPAddr[0], tmpPtr, SCTP_MAX_IP_LEN);

        SctpInstanceID = sctp_registerInstance(0, 1, 1, 1, LocalIPAddr, SctpCB);

        if (SctpInstanceID <= 0)
        {
            DiagPrint(DIAG_LEVEL0, "SCTP", "Instance init error [%i]\n", SctpInstanceID);
            return FAILURE;
        }

        if (HostSendMsg == NULL)
        {
            // Connect to the host
            if (sctp_associate(SctpInstanceID, 1, HostIPAddr, HostPort, NULL) == 0)
            {
                DiagPrint(DIAG_LEVEL0, "SCTP", "ERROR Connecting to Host\n");
                return FAILURE;
            }

            DiagPrint(DIAG_LEVEL2, "SCTP", "Connecting...\n");
            HostSendMsg = SctpSend;
            ProtoSend = SctpSend;

            OsalCreateTask(&ProtoRxTaskID, SctpRxTask, NULL, "ProtoRxTask", OSAL_PRIORITY_NORMAL, NULL, 0);
            OsalActivateTask(&ProtoRxTaskID);

            // Wait 15 seconds for connection to become active
            if (WaitForSingleObject(ProtoReady, 15000) != WAIT_OBJECT_0)
            {
                DiagPrint(DIAG_LEVEL1, "SCTP", "No connection to Host\n");
                ProtoDone();
                ProtoSend = NULL;
                HostSendMsg = NULL;
            }
            else
            {
                DiagPrint(DIAG_LEVEL2, "SCTP", "Connected\n");
            }
        }
        else
        {
            DiagPrint(DIAG_LEVEL1, "SCTP", "Inactive\n");
            return FAILURE;
        }
        break;
    case IPPROTO_UDP:
        break;

    default:
        Protocol = 0;
        break;
    }

    return SUCCESS;
}

/** Send data block to transport layer
 *
 * @param size Size of block in bytes
 * @param data Pointer to data
 * @return Standart result code
 */
RESULTCODE SctpSend(U32 size, PTR data)
{
    RESULTCODE res = FAILURE;
    int ret;

    SvsrApiMsgStats.SctpTxMsgCount++;

    if (ProtoSocket > 0)
    {
        SctpLock(&ProtoMutex);
        ret = sctp_send(ProtoSocket, 0,
            (unsigned char *) data, size,
            SCTP_GENERIC_PAYLOAD_PROTOCOL_ID,
            SCTP_USE_PRIMARY,
            SCTP_NO_CONTEXT,
            SCTP_INFINITE_LIFETIME,
            SCTP_ORDERED_DELIVERY,
            SCTP_BUNDLING_DISABLED);
        SctpUnlock(&ProtoMutex);
        if (ret == SCTP_SUCCESS)
        {
            SvsrApiMsgStats.SctpTxMsgCount1++;
            res = SUCCESS;
        }
        else
        {
             SvsrApiMsgStats.SctpTxMsgCountErr2++;
        }
    }
    else
    {
        SvsrApiMsgStats.SctpTxMsgCountErr1++;
        DiagPrint(DIAG_LEVEL4, "SCTP", "No socket\n");
    }
    OsalFreeMemArray(&MsgArray, data);

    return res;
}

RESULTCODE TcpSend(U32 size, PTR data)
{
    RESULTCODE ret = SUCCESS;
    int res = 0;
    U32 total = 0;

    if (ProtoSocket > 0)
    {
        while (total < size)
        {
            res = send(ProtoSocket, (unsigned char *) data + total, size - total, 0);
            if (res == SOCKET_ERROR)
            {
                DiagPrint(DIAG_LEVEL4, "TCP", "Send failure [%i]\n", res);
                ret = FAILURE;
                break;
            }
            total += res;
        }
    }
    OsalFreeMemArray(&MsgArray, data);
    return SUCCESS;
}

/** This task receives packets from transport layer
 *
 * @param taskParm Not used
 */
void SctpRxTask(PTR taskParm)
{
    // Event loop
    while (sctp_extendedEventLoop(SctpLock, SctpUnlock, &ProtoMutex) >= 0)
    {
        // Break when negative
    }
    if (ProtoSocket > 0)
    {
        sctp_shutdown(ProtoSocket);
    }
}

void TcpRxTask(PTR taskParm)
{
    PMSGHEADER pHdr;
    int bytesRecvd;
    int total;
    int runTask = 1;

    while (runTask)
    {
        if (OsalAllocMemArray(&MsgArray, &pHdr) == SUCCESS)
        {
#if _WIN32_WINNT >= 0x0502
            bytesRecvd = recv(ProtoSocket, (char*) buffer, sizeof(MSGHEADER), MSG_WAITALL);
#else
            total = 0;
            while (total < sizeof(MSGHEADER))
            {
                bytesRecvd = recv(ProtoSocket, (char *) pHdr, sizeof(MSGHEADER), 0);
                if (bytesRecvd == SOCKET_ERROR)
                {
                    runTask = 0;
                    OsalFreeMemArray(&MsgArray, pHdr);
                    break;
                }
                if (bytesRecvd == 0)
                {
                    // Peer closed connection
                    runTask = 0;
                    OsalFreeMemArray(&MsgArray, pHdr);
                    break;
                }
                total += bytesRecvd;
            }
#endif /* _WIN32_WINNT */
            if ((pHdr->control & CTRL_VERSMASK) == CTRL_VERSION)
            {
                if ((pHdr->length == 0) && runTask)
                {
                    MsgDispatch(MSGS_HOST, sizeof(MSGHEADER), pHdr);
                }
                else if (pHdr->length <= MSG_MAXSIZE - sizeof(MSGHEADER))
                {
#if _WIN32_WINNT >= 0x0502
                    bytesRecvd = recv(ProtoSocket, MsgGetDataOffset(buffer), pHdr->length, MSG_WAITALL);
#else
                    total = 0;
                    while ((unsigned) total < pHdr->length)
                    {
                        bytesRecvd = recv(ProtoSocket,
                                          (char *) ((uintptr_t)MsgGetDataOffset(pHdr) + total),
                                          pHdr->length - total, 0);
                        if (bytesRecvd == SOCKET_ERROR)
                        {
                            runTask = 0;
                            OsalFreeMemArray(&MsgArray, pHdr);
                            break;
                        }
                        if (bytesRecvd == 0)
                        {
                            runTask = 0;
                            OsalFreeMemArray(&MsgArray, pHdr);
                            break;
                        }
                        total += bytesRecvd;
                    }
#endif
                    if (runTask)
                    {
                        MsgDispatch(MSGS_HOST, sizeof(MSGHEADER) + pHdr->length, pHdr);
                    }
                    else
                    {
                        MsgFreeBuffer(pHdr);
                    }
                }
            }
            else
            {
                MsgFreeBuffer(pHdr);
            }
            // TODO: Process part of possible next chunk?
        }
        else
        {
            DiagPrint(DIAG_LEVEL5, "TCP", "No memory [%s], retrying\n", __FUNCTION__);
            OsalWaitDelay(MEM_RECOVERYTIME);
        }
    }
}
RESULTCODE ProtoDone(void)
{
    printf(C4KFW "Stopping transport layer... ");
    switch (Protocol)
    {
    case IPPROTO_TCP:
        if (ProtoSocket > 0)
        {
            closesocket(ProtoSocket);
        }
        if (ProtoRxTaskID != NULL)
        {
            WaitForSingleObject((HANDLE) ProtoRxTaskID, INFINITE);
            CloseHandle(ProtoRxTaskID);
        }
        break;
    case IPPROTO_UDP:
        break;
    case IPPROTO_SCTP:
        if (ProtoSocket > 0)
        {
            SctpLock(&ProtoMutex);
            sctp_abort(ProtoSocket);
            SctpUnlock(&ProtoMutex);
        }
        if (ProtoRxTaskID != NULL)
        {
            SctpLock(&ProtoMutex);
            sctp_stop();
            SctpUnlock(&ProtoMutex);
            WaitForSingleObject((HANDLE) ProtoRxTaskID, INFINITE);
            CloseHandle(ProtoRxTaskID);
        }
        ProtoSocket = -1;
        break;
    }
    ProtoRxTaskID = NULL;
    DeleteCriticalSection(&ProtoMutex);
    if (ProtoReady != NULL)
    {
        CloseHandle(ProtoReady);
        ProtoReady = NULL;
    }
    printf("Done\n");
    return SUCCESS;
}

void SctpLock(void *data)
{
    EnterCriticalSection((LPCRITICAL_SECTION) data);
}

void SctpUnlock(void *data)
{
    LeaveCriticalSection((LPCRITICAL_SECTION) data);
}


// Udp protocol:
// need to add:
// - AM/UM direction support instead of #ifdef ENODEB/UE
// - the UdpProtoInit has to affort each DTCH

RESULTCODE UdpProtoInit(void)
{
    WSADATA wsaData;

    int iOptLen = sizeof(U32);
    int iOptVal = 500 * 1024;
    int NumTimeOut = 1000; // 1 sec

    WSAStartup(MAKEWORD(2,2), &wsaData);
    ProtoSocketUdp = socket(AF_INET, SOCK_DGRAM, 0);

#ifdef ENODEB
    {
    char *tmpPtr;
    OSAL_PADDRESS UdpMemBlock;

    HostPort = 1234;
    ReadSettingInt("vlc_rcv_port", &HostPort, C4K_PORT);

    ReadSettingStr("vlc_rcv_ip", &tmpPtr, "0.0.0.0");
    strncpy(HostIPAddr, tmpPtr, SCTP_MAX_IP_LEN);

    OsalAllocHeap(&OsalGlobalHeap, &UdpMemBlock, UDP_MAX_FRAME * 1000);
    OsalCreateMemArray(&UdpMsgArray, UdpMemBlock, UDP_MAX_FRAME * 1000,
                   UDP_MAX_FRAME);
    UdpHostAddr.sin_family = AF_INET;
    UdpHostAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    UdpHostAddr.sin_port = htons((unsigned short)HostPort);
    if(bind(ProtoSocketUdp, (SOCKADDR*) &UdpHostAddr,sizeof(UdpHostAddr)) != 0)
    {
         printf("%d: cannot bind udp port\n",WSAGetLastError());
         closesocket(ProtoSocketUdp);
         return FAILURE;
   }

    if(setsockopt(ProtoSocketUdp,SOL_SOCKET, SO_RCVBUF, (char*)&iOptVal, iOptLen) != 0)
    {
        printf("%d: cannot set socket options\n",WSAGetLastError());
        closesocket(ProtoSocketUdp);
        return FAILURE;
    }
    if(getsockopt(ProtoSocketUdp,SOL_SOCKET, SO_RCVBUF, (char*)&iOptVal, &iOptLen) != 0)
    {
        printf("%d: cannot get socket options\n",WSAGetLastError());
        closesocket(ProtoSocketUdp);
        return FAILURE;
    }
//       ret = setsockopt(ProtoSocketUdp,SOL_SOCKET, SO_RCVTIMEO, (char *) &NumTimeOut, sizeof(NumTimeOut));

    OsalCreateTask(&UdpRxTaskID, UdpRxTask, NULL, "UdpRxTask", OSAL_PRIORITY_NORMAL, NULL, 0);
    OsalCreateSema(&SEMA_UDP_RX);
    OsalActivateTask(&UdpRxTaskID);
    }
#elif defined (UE)
    {
    char *tmpPtr;

    HostPort = 1235;
    ReadSettingInt("vlc_tran_port", &HostPort, C4K_PORT);
    ReadSettingStr("vlc_tran_ip", &tmpPtr, "127.0.0.1");
    strncpy(HostIPAddr, tmpPtr, SCTP_MAX_IP_LEN);

    UdpHostAddr.sin_family = AF_INET;
    UdpHostAddr.sin_addr.s_addr = inet_addr(HostIPAddr);
    UdpHostAddr.sin_port = htons((unsigned short)HostPort);
    if (connect(ProtoSocketUdp, (SOCKADDR*) &UdpHostAddr, sizeof(UdpHostAddr)) == SOCKET_ERROR)
        {
            printf(C4KFW "UDP: Connection error\n");
            closesocket(ProtoSocketUdp);
            ProtoSocketUdp = -1;
            return FAILURE;
        }

    if (UdpSendMsg == NULL)
        UdpSendMsg = UdpSend;
    }
#endif
    WSACleanup();
    return SUCCESS;
}
RESULTCODE UdpProtoDone(void)
{
    printf(C4KFW "Stopping UDP layer... ");
    if (UdpRxTaskID)
    {
        OsalDeactivateTask(&UdpRxTaskID);
        OsalDestroyTask(&UdpRxTaskID);
        UdpRxTaskID = 0;
    }
    OsalDestroySema(&SEMA_UDP_RX);

    if (ProtoSocketUdp > 0)
    {
        if( SOCKET_ERROR == closesocket(ProtoSocketUdp))
        {
            printf("Udp Socket error: %d\n", WSAGetLastError());
            return FAILURE;
        }
        WSACleanup();
    }


    OsalFreeHeap(&OsalGlobalHeap, &UdpMsgArray);

    printf("Done\n");
    return SUCCESS;
}
RESULTCODE UdpSend(U32 size, PTR data)
{
    U32 res = 0;

    if (ProtoSocketUdp > 0)
    {
           res= send(ProtoSocketUdp, (unsigned char *) data, size,0);
            if (res == SOCKET_ERROR)
            {
                printf("%d Send failure [%i]\n", WSAGetLastError());
                return FAILURE;
        }
    }
    return SUCCESS;

}

PTR UdpGetFrame(U32* BufLen)
{ 
    PTR rxBuffer;
    U32 bytesRecvd = 0;
    U32 total = 0;
    U32 len = sizeof(UdpHostAddr);
    U32 err;

    *BufLen = 0;
    if(ProtoSocketUdp == 0)
    {
        return NULL;
    }
    if (OsalAllocMemArray(&UdpMsgArray, &rxBuffer) == SUCCESS)
    {

        bytesRecvd = recvfrom(ProtoSocketUdp, ((char*) rxBuffer),
            UdpMsgArray.blockSize, 0,
            (struct sockaddr *) &UdpHostAddr,
            &len);
        if(bytesRecvd == SOCKET_ERROR)
        {
            err= WSAGetLastError();
            //    if(WSAETIMEDOUT == err)
            //   {
            //   }
            *BufLen = 0;
            UdpFrameFree(rxBuffer);
            return NULL;
        }
        if(bytesRecvd == WSAEMSGSIZE)
        {
            *BufLen = 0;
            UdpFrameFree(rxBuffer);
            return NULL;
        }
    }
    *BufLen = bytesRecvd;// + 4;
    return rxBuffer;
}

void UdpRxTask(PTR taskParm)
{
    PTR rxBuffer = NULL;
    U32 Len;
    U32 res = 0;
    while (TRUE)
    {
        {
            if (rxBuffer != NULL)
            {
                res = MacRxEthPacket(rxBuffer, Len);
                if( (res != EXPIRED) && (res != SUCCESS))
                    rxBuffer = NULL;
            }
            if (rxBuffer == NULL)
            {
                 while ((rxBuffer = UdpGetFrame(&Len)) != NULL)
                {
                    res = MacRxEthPacket(rxBuffer, Len);
                    if( res == EXPIRED || res == SUCCESS)
                    {
                        break;
                    }
                    rxBuffer = NULL;
                 }
            }
        }
    }

}
void  UdpFrameFree(PTR Packet)
{
	OsalFreeMemArray(&UdpMsgArray, Packet);
}

void UdpRxRaiseSema(void)
{
    if(SEMA_UDP_RX)
    OsalRaiseSema(&SEMA_UDP_RX);
}
/** Stop transport module
 *
 * @return Standard result code
 */


#endif /* USE_WINDOWS */

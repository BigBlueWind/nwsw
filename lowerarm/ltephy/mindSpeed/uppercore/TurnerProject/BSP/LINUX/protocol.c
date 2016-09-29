/** @file protocol.c
 *
 * @brief SCTP/TCP/UDP transport for Linux
 * @author Mindspeed Technologies
 * @version $Revision: 1.36 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_LINUX

#include "osal.h"
#include "bsp.h"
#include "transport.h"
#include "settings.h"
#include "apidefs.h"
#include "hostiface.h"
#include "msgroute.h"
#include "maciface.h"
#include "diags.h"
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <linux/tcp.h>
#include "svsr.h"

//
// Constants
//
#define PROTO_HOSTADDR              "127.0.0.1"
#define PROTO_HOSTPORT              32
#define SOCKET_ERROR                (-1)

#define UDP_MAX_FRAME               2000

//
// Host address
//
struct sockaddr_in HostAddr;
struct sockaddr_in UdpHostAddr;

char HostIPAddr[256];
unsigned int HostIPPort;
OSAL_SEMAPHORE SEMA_UDP_RX;

//
// Globals
//
int Protocol = 0;
int ProtoSocket = -1;
int ProtoSocketUdp = -1;

OSAL_TASK ProtoRxTaskID = (OSAL_TASK) 0;
OSAL_TASK UdpRxTaskID = (OSAL_TASK) 0;
OSAL_MEMARRAY UdpMsgArray;
extern OSAL_MEMARRAY MsgArray;
UdpSendMsgCB UdpSendMsg = NULL;

//
// Pointers to actual send functions
//
extern RESULTCODE (*HostSendMsg)(U32 msgSize, PTR data);
RESULTCODE (*ProtoSend)(U32 size, PTR data) = NULL;
RESULTCODE UdpSend(U32 size, PTR data);
void UdpFrameFree(PTR Packet);
void UdpRxTask(PTR taskParm);

//
// Forward declarations
//
void ProtoRxTaskSctp(PTR taskParm);
void ProtoRxTaskTcp(PTR taskParm);
void ProtoRxTaskUdp(PTR taskParm);

/** Initialize transport module
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
RESULTCODE ProtoInit(U32 protoid)
{
    const char *hostAddr;
    int ret;
    int ndelay = 1;

    bzero((void *)&HostAddr, sizeof(HostAddr));

    ReadSettingStr("hostip", &hostAddr, PROTO_HOSTADDR);
    strcpy(HostIPAddr, hostAddr);
    ReadSettingInt("hostport", (int *) &HostIPPort, PROTO_HOSTPORT);

    HostAddr.sin_family = AF_INET;
    HostAddr.sin_port = htons((uint16_t)HostIPPort);
    HostAddr.sin_addr.s_addr = inet_addr(HostIPAddr);

    ReadSettingInt("proto", &Protocol, protoid);
    ProtoSocket = socket(AF_INET, SOCK_STREAM, Protocol);

    if (HostSendMsg == NULL)
    {
        switch (Protocol)
        {
            case PROTO_TCP:
                DiagPrint(DIAG_LEVEL2, "TCP", "Connecting...\n");
                setsockopt(ProtoSocket, IPPROTO_TCP, TCP_NODELAY, &ndelay, sizeof(int));
                ret = connect(ProtoSocket, (struct sockaddr *)&HostAddr, sizeof(HostAddr));
                if (ret < 0)
                {
                    DiagPrint(DIAG_LEVEL1, "TCP", "No connection to Host [%i]\n", errno);
                }
                else
                {
                    DiagPrint(DIAG_LEVEL2, "TCP", "Connected\n");
                    OsalCreateTask(&ProtoRxTaskID, ProtoRxTaskTcp, NULL, "ProtoRxTask",
                                   OSAL_PRIORITY_NORMAL, 0, 0);
                    OsalActivateTask(&ProtoRxTaskID);

                    ProtoSend = TcpSend;
                    HostSendMsg = TcpSend;
                }
                break;
            case PROTO_UDP:
                ProtoSend = UdpSend;
                HostSendMsg = UdpSend;
                break;
            case PROTO_SCTP:
            {
                struct sctp_initmsg initmsg;
                struct sctp_event_subscribe events;

                DiagPrint(DIAG_LEVEL2, "SCTP", "Connecting...\n");

                // Initialize parameters
                bzero(&initmsg, sizeof(initmsg));
                initmsg.sinit_num_ostreams = 1;
                initmsg.sinit_max_instreams = 1;
                initmsg.sinit_max_attempts = 1;
                setsockopt(ProtoSocket, SOL_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg));

                // TODO: Check performance
                setsockopt(ProtoSocket, SOL_SCTP, SCTP_NODELAY, &ndelay, sizeof(ndelay));

                // Enable SCTP send/recv events
                memset(&events, 0, sizeof(events));
                events.sctp_data_io_event = 1;
                setsockopt(ProtoSocket, SOL_SCTP, SCTP_EVENTS, &events, sizeof(events));

                // Connect to the server
                ret = connect(ProtoSocket, (struct sockaddr *)&HostAddr, sizeof(HostAddr));
                if (ret < 0)
                {
                    if (errno == EALREADY || errno == EISCONN)
                    {
                        // Not a real errors, just show that operation is in progress
                        DiagPrint(DIAG_LEVEL1, "SCTP", "Connection in progress [%i]\n", errno);
                        ret = 0;
                    }
                    else
                    {
                        DiagPrint(DIAG_LEVEL1, "SCTP", "No connection to Host [%i]\n", errno);
                    }
                }
                if (ret >= 0)
                {
                    DiagPrint(DIAG_LEVEL2, "SCTP", "Connected\n");

                    ProtoSend = SctpSend;
                    HostSendMsg = SctpSend;

                    OsalCreateTask(&ProtoRxTaskID, ProtoRxTaskSctp, NULL, "ProtoRxTask",
                                   OSAL_PRIORITY_NORMAL, 0, 0);
                    OsalActivateTask(&ProtoRxTaskID);
                }
                break;
            }
            default:
                return FAILURE;
        }
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
    RESULTCODE ret = SUCCESS;
    int res;

    DiagPrintApi(DIAG_LEVEL30, "SCTPTX", 0, data);

    SvsrApiMsgStats.SctpTxMsgCount++;

    if (ProtoSocket >= 0)
    {
        res = sctp_send(ProtoSocket, data, size, NULL, 0);
        if (res == -1)
        {
            ret = FAILURE;
            SvsrApiMsgStats.SctpTxMsgCountErr2++;
        }
        else
        {
            SvsrApiMsgStats.SctpTxMsgCount1++;
        }
    }
    else
    {
        SvsrApiMsgStats.SctpTxMsgCountErr1++;
    }

    MsgFreeBuffer(data);

    return ret;
}

/** Send data block to transport layer
 *
 * @param size Size of block in bytes
 * @param data Pointer to data
 * @return Standart result code
 */
RESULTCODE TcpSend(U32 size, PTR data)
{
    RESULTCODE ret = SUCCESS;
    int res = 0;

    if (ProtoSocket >= 0)
    {
        while (size)
        {
            res = send(ProtoSocket, data + res, size, 0);
            if (res == -1)
            {
                ret = FAILURE;
                break;
            }
            size -= res;
        }
    }
    MsgFreeBuffer(data);

    return ret;
}

/** This task receives packets from transport layer
 *
 * @param taskParm Not used
 */
void ProtoRxTaskSctp(PTR taskParm)
{
    PMSGHEADER buffer;
    int flags, ret;
    struct sctp_sndrcvinfo sndRcvInfo;

    while (1)
    {
        if ((buffer = MsgAllocBuffer(MSG_COMMON)) != NULL)
        {
            ret = sctp_recvmsg(ProtoSocket, buffer, MSG_MAXSIZE, NULL, NULL,
                               &sndRcvInfo, &flags);
//          recv(ProtoSocket, buffer, sizeof(buffer), 0);
            if (ret == -1)
            {
                DiagPrint(DIAG_LEVEL4, "SCTP", "Error receiving data [%i]\n", errno);
                MsgFreeBuffer(buffer);
                break;
            }
            DiagPrintApi(DIAG_LEVEL30, "SCTPRX", 0, buffer);
            MsgDispatch(MSGS_HOST, (U32) ret, buffer);
        }
        else
        {
            DiagPrint(DIAG_LEVEL5, "SCTP", "No memory [%s,%i]\n", __FUNCTION__, __LINE__);
            OsalWaitDelay(MEM_RECOVERYTIME);
        }
    }
    close(ProtoSocket);
    ProtoSocket = -1;
}

/** This task receives packets from transport layer
 *
 * @param taskParm Not used
 */
void ProtoRxTaskTcp(PTR taskParm)
{
    PMSGHEADER buffer;
    int ret;

    while (1)
    {
        if ((buffer = MsgAllocBuffer(MSG_COMMON)) != NULL)
        {
            ret = recv(ProtoSocket, buffer, sizeof(MSGHEADER), MSG_WAITALL);
            if (ret == -1)
            {
                DiagPrint(DIAG_LEVEL4, "TCP", "Error receiving data [%i]\n", errno);
                MsgFreeBuffer(buffer);
                break;
            }
            if (buffer->length > 0)
            {
                ret = recv(ProtoSocket, buffer + 1, buffer->length, MSG_WAITALL);
                if (ret == -1)
                {
                    DiagPrint(DIAG_LEVEL4, "TCP", "Error receiving data [%i]\n", errno);
                    MsgFreeBuffer(buffer);
                    break;
                }
            }
            DiagPrintApi(DIAG_LEVEL30, "TCP", 0, buffer);
            MsgDispatch(MSGS_HOST, sizeof(MSGHEADER) + buffer->length, buffer);
        }
        else
        {
            DiagPrint(DIAG_LEVEL5, "TCP", "No memory [%s,%i]\n", __FUNCTION__, __LINE__);
            OsalWaitDelay(MEM_RECOVERYTIME);
        }
    }
    close(ProtoSocket);
    ProtoSocket = -1;
}

/** Stop transport module
 *
 * @return Standard result code
 */
RESULTCODE ProtoDone(void)
{
    if (ProtoSocket >= 0)
    {
        shutdown(ProtoSocket, SHUT_RDWR);
        close(ProtoSocket);
    }
    if (ProtoRxTaskID != (OSAL_TASK) 0)
    {
        OsalDestroyTask(&ProtoRxTaskID);
        ProtoRxTaskID = (OSAL_TASK) 0;
    }
    if (HostSendMsg == ProtoSend)
    {
        HostSendMsg = NULL;
    }
    ProtoSend = NULL;
    ProtoSocket = -1;
    return SUCCESS;
}


//UDP init
RESULTCODE UdpProtoInit(void)
{
#if defined (ENODEB) || defined (UE)
    int ret;
    OSAL_PADDRESS UdpMemBlock;
    const char *hostAddr;
    //struct sctp_event_subscribe events;
    int HostPort;
#endif


       ProtoSocketUdp = socket(AF_INET, SOCK_DGRAM, 0);
        //
        // Host port
        //
#if defined (ENODEB)
        HostPort = 1234;
        ReadSettingInt("vlc_rcv_port", &HostPort, PROTO_HOSTPORT);

        ReadSettingStr("vlc_rcv_ip", &hostAddr, "0.0.0.0");
        strcpy(HostIPAddr, hostAddr);

        OsalAllocHeap(&OsalGlobalHeap, &UdpMemBlock, UDP_MAX_FRAME * 600);
        OsalCreateMemArray(&UdpMsgArray, UdpMemBlock, UDP_MAX_FRAME * 600,
                       UDP_MAX_FRAME);
        UdpHostAddr.sin_family = AF_INET;
        UdpHostAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        UdpHostAddr.sin_port = htons((unsigned short)HostPort);
        if((ret = bind(ProtoSocketUdp, &UdpHostAddr,sizeof(UdpHostAddr))) != 0)
           {
             printf("cannot bind udp port\n");
             //exit(1);
           }
        OsalCreateTask(&UdpRxTaskID, UdpRxTask, NULL, "UdpRxTask", OSAL_PRIORITY_NORMAL, NULL, 0);
        OsalCreateSema(&SEMA_UDP_RX);
        OsalActivateTask(&UdpRxTaskID);

#elif defined (UE)
        HostPort = 1235;

        ReadSettingInt("vlc_tran_port", &HostPort, HostPort);
        // ?????
        ReadSettingStr("vlc_tran_ip", &hostAddr, "10.1.184.67");
        strcpy(HostIPAddr, hostAddr);

        UdpHostAddr.sin_family = AF_INET;
        UdpHostAddr.sin_addr.s_addr = inet_addr(HostIPAddr);
        UdpHostAddr.sin_port = htons((unsigned short)HostPort);

        if (UdpSendMsg == NULL)
            UdpSendMsg = UdpSend;
#endif
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
        close(ProtoSocketUdp);
    }

    OsalFreeHeap(&OsalGlobalHeap, &UdpMsgArray);

    printf("Done\n");
    return SUCCESS;
}

PTR UdpGetFrame(U32* BufLen)
{ 
    PTR rxBuffer;
    U32 bytesRecvd = 0;
    U32 len = sizeof(UdpHostAddr);

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
                if (bytesRecvd > MsgArray.blockSize || bytesRecvd == 0)
                {
                    UdpFrameFree(rxBuffer);
                    return NULL;
                }
        }
    *BufLen = bytesRecvd;
    return rxBuffer;
}
void UdpRxTask(PTR taskParm)
{
    PTR rxBuffer;
    U32 Len;
    U32 FrameNum = 0;
    U32 MaxFrameNum = 20;
    while (TRUE)
    {
        if (OsalWaitForSema(&SEMA_UDP_RX, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            while ((FrameNum < (MaxFrameNum - 1)) && (rxBuffer = UdpGetFrame(&Len)) != NULL)
            {
                U32 res ;
                res = MacRxEthPacket(rxBuffer, Len);
                if( res == EXPIRED || res == SUCCESS)
                {
                    break;
                }
                FrameNum = (FrameNum + 1)%MaxFrameNum;
             }
            FrameNum = 0;
        }
    }

}
void  UdpFrameFree(PTR Packet)
{
	OsalFreeMemArray(&UdpMsgArray, Packet);
}

void UdpRxRaiseSema(void)
{
    OsalRaiseSema(&SEMA_UDP_RX);
}
RESULTCODE UdpSend(U32 size, PTR data)
{
    RESULTCODE ret = SUCCESS;
    U32 res;

    if (ProtoSocketUdp > 0)
    {

            res= sendto(ProtoSocketUdp, (unsigned char *) data, size, 0,
                         (struct sockaddr *) &UdpHostAddr,
                         sizeof(UdpHostAddr));
            if (res == SOCKET_ERROR)
            {
                printf("UDP Send failure\n");
                ret = FAILURE;
            }
    }
    return ret;

}

#endif /* USE_LINUX */

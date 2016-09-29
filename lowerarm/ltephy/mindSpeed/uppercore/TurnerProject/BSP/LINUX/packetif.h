/** @file packetif.h
 *
 * @brief Interface to external network interface
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _PACKETIF_H_
#define _PACKETIF_H_

#include "basetypes.h"
#include "osal.h"
#include "pcap.h"

//#define USED_ETH_ADAPTER  1

#define ETH_MAXFRAMESIZE 1536

#define   ETH_HDR_SIZE 14  /* Ethernet header size     */
#define ETHER_ADDR_LEN 6
#define SIZE_ETHER_TYPE 4

// TODO: move diagnostics to self module
#define DIAG_ENABLED 0

typedef struct _PACKET_DESC
{
    struct _PACKET_DESC *pNext;
    void *pRefData;
    OSAL_MEMARRAY *pPart;
    void *pArq;
    U16 Counter;
    U16 Length;
    U16 Offset;
    U16 Cid;
    U8 *Payload;
} PACKET_DESC;

/* 4 bytes IP address */
typedef struct ip_address
{
    u_char byte1;   // (highest) Transferred first
    u_char byte2;
    u_char byte3;
    u_char byte4;
} ip_address;

/* IPv4 header */
typedef struct ip_header
{
    u_char ver_ihl;     // Version (4 bits) + Internet header length (4 bits)
    u_char tos;      // Type of service
    u_short tlen;     // Total length
    u_short id;   // Identification
    u_short flags_fo;    // Flags (3 bits) + Fragment offset (13 bits)
    u_char ttl;      // Time to live
    u_char proto;     // Protocol
    u_short crc;     // Header checksum
    ip_address saddr;    // Source address
    ip_address daddr;    // Destination address
    u_int op_pad;     // Option + Padding
} ip_header;

/* UDP header*/
typedef struct udp_header
{
    u_short sport;     // Source port
    u_short dport;     // Destination port
    u_short len;     // Datagram length
    u_short crc;     // Checksum
} udp_header;

typedef struct
{
    u_char ver_ihl;     // Version (4 bits) + Internet header length (4 bits)
    u_char tos;      // Type of service
    u_short tlen;     // Total length
    u_short id;   // Identification
    u_short flags_fo;    // Flags (3 bits) + Fragment offset (13 bits)
    u_char ttl;      // Time to live
    u_char proto;     // Protocol
    u_short crc;     // Header checksum
    ip_address saddr;    // Source address
    ip_address daddr;    // Destination address
    u_short udp_sport;     // UDP source port
    u_short udp_dport;     // UDP destination port
    u_short udp_len;     // Datagram Length
    u_short udp_xsum;    // Checksum
} ip_udp_hdr;

typedef RESULTCODE(*MacCB)(PTR, U32);

#define UDP_HDR_SIZE 8
#define IPV4_HDR_SIZE (sizeof (ip_udp_hdr) - UDP_HDR_SIZE)
#define IPV4_UDP_HDR_SIZE  (sizeof (ip_udp_hdr))

RESULTCODE NetAllocPacket(PACKET_DESC ** ppPacket);
RESULTCODE NetFreePacket(PACKET_DESC * pPacket);
RESULTCODE NetEthInit(BOOLEAN bOpen, OSAL_HEAP *pHeap, MacCB cb);
RESULTCODE NetEthDone(void);
RESULTCODE NetEthSendPacket(PACKET_DESC * pPacket);
void NetRaiseSema(void);

#endif /* _PACKETIF_H_ */


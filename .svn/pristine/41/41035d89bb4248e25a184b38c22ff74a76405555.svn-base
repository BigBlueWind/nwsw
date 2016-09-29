
#ifndef _ETHERNET_H_
#define _ETHERNET_H_

#include "net_types.h"
#include "network.h"

#define NET_ETHERNET_PKT_IP 	0x0800
#define NET_ETHERNET_PKT_ARP 	0x0806

extern byte net_bcast_mac[NET_MAC_SIZE];
extern UINT8 eth_buf[];

typedef struct
{
	connection_t	*conn;
	byte			*frame;
	size_t			size;
}net_arm_resolve_queue_el_t;

#pragma pack(1)

typedef struct _ethhdr_t 
{
    byte   dest_mac[6];   // destination eth addr
    byte   source_mac[6]; // source ether addr
    UINT16  pkt_type;      // packet type ID field
}ethhdr_t;

#define ethhdr_size sizeof(ethhdr_t)

MXRC net_ethernet_send(connection_t *conn, UINT16 pkt_type, byte *hbuff, size_t hsize, const  byte *pbuff, const size_t psize);
MXRC net_ethernet_send_to(byte *mac, UINT16 pkt_type, byte *hbuff, size_t hsize, const byte *pbuff, const size_t psize);


#endif

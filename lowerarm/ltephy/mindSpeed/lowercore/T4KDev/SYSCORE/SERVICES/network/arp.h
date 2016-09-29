
#ifndef _ARP_H_
#define _ARP_H_

#include "net_types.h"
#include "network.h"

#define NET_ARP_TABLE_SIZE	128
#define NET_ARP_OPCODE_REQUEST	0x0001
#define NET_ARP_OPCODE_RESP		0x0002
#define NET_ETHERNET_PKT_ETHTP	0x0001

#define NET_ARP_WAIT_TO	500000

typedef struct 
{
	ipaddr_t 	ip;
	byte 		mac[NET_MAC_SIZE];
}net_arp_entry_t;

#pragma pack(1)

typedef struct {
    UINT16 		arp_htype;   // format of hardware address	
    UINT16		arp_ptype;   // format of protocol address	
    byte		arp_hln;   // length of hardware address	
    byte		arp_pln;   // length of protocol address	
    UINT16		arp_oper;    // ARP opcode (command)		
    // Ethernet looks like this, but the following can be variable sized elsewhere.
    byte		arp_sha[NET_MAC_SIZE]; // sender hardware address
    ipaddr_t   	arp_sip;        // sender IP address		
    byte		arp_tha[NET_MAC_SIZE]; // target hardware address
    ipaddr_t   	arp_tip;        // target IP address		
}arphdr_eth_t;

#define arphdr_eth_size sizeof(arphdr_eth_t)

MXRC net_arp_init(void);
MXRC net_arp_upd_entry(ipaddr_t ip, byte *mac);
ipaddr_t net_arp_get_ip(byte *mac);
MXRC net_arp_get_mac(ipaddr_t ip, byte *mac);
MXRC net_arp_request(ipaddr_t ip);
MXRC net_arp_parce_pkt(byte *pkt);
MXRC net_arp_queue_put(connection_t *conn);

#endif

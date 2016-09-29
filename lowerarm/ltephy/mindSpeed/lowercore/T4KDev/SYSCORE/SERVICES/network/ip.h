#ifndef _IP_H_
#define _IP_H_

#include "net_types.h"
#include "network.h"

#define NET_IP_TTL 128
#define NET_IP_MAX_ID	0xFFFF
#define NET_IP_DEFAULT_TOS 0
#define NET_IP_BROADCAST 0xFFFFFFFF

#pragma pack(1)

typedef struct _iphdr_t // 20 or more bytes long
{
    
	byte    ihl:4,      // IP header length
        	version:4;  // version = 4            
    byte    tos;        // type of service (not used)
    UINT16  tot_len;    // size of datagram (header + data)
    UINT16  id;
    UINT16  frag_off;
    byte    ttl;        // number of network hops
    byte    protocol;   // type of transport protocol
    UINT16  check;      // header checksum
    UINT32  saddr;      // source IP address
    UINT32  daddr;      // destination IP address
    // The options start here.
}iphdr_t;

#define iphdr_size sizeof(iphdr_t)
#define iphdr iphdr_t


MXRC net_ip_send_to(connection_t *conn, net_sockaddr *to, byte *hbuff, size_t hsize, const byte *pbuff, const size_t psize);
MXRC net_ip_send(connection_t *conn, byte *hbuff, size_t hsize, const byte *pbuff,  const size_t psize);
UINT16	net_ip_get_id(connection_t *conn);
UINT16 net_ip_calc_cs(byte *buff, size_t size);
MXRC net_ip_parce_pkt(byte *pkt);

#endif


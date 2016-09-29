
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <string.h>
#include "config.h"
#include "4gmx.h"
#include "appids.h"
#include "hal.h"
#include "appprintf.h"
#include "net_types.h"
#include "net_buff.h"

#define USE_IPERF 1

#define NET_MAX_CONN_NUMBER	32
#define NET_NO_SOCKETS 0
#define NET_START_PORT_NUMBER	45000
#define NET_MAX_PORT_NUMBER	0xFFF0
#define NET_ARP_QUEUE_LEN	NET_MAX_CONN_NUMBER
#define NET_QUEUE_LEN	NET_MAX_CONN_NUMBER

#define NET_BIG_ENDIAN	1
#define NET_LIT_ENDIAN	2

#define NET_BYTE_ORDER  NET_LIT_ENDIAN 

#define NET_API_SETIP	1
#define NET_API_SETMAC	2

enum 
{
	net_prof_net_in = 0,
	net_prof_net_out,
	net_prof_udp_in,
	net_prof_udp_out,
	net_prof_ip_in,
	net_prof_ip_out,
	net_prof_eth_in,
	net_prof_eth_out,
	net_prof_hw_in,
	net_prof_hw_out,
	net_prof_gem_1,
	net_prof_gem_2,
	net_prof_gem_3,
	net_prof_gem_4,
	net_prof_gem_5,
};

#define NET_PROF_SIZE  15

extern UINT32 net_prof[];

//#define NET_PROFILE(a)  net_prof[a] = MxGetTicks()
#define NET_PROFILE(a)


#ifndef HTONS
	#if NET_BYTE_ORDER == UIP_BIG_ENDIAN
		#define HTONS(n) (n)
	#else 
		#define HTONS(n) (UINT16)((((UINT16) (n)) << 8) | (((UINT16) (n)) >> 8))
	#endif
#else
	#error "HTONS already defined!"
#endif 

#ifndef HTON
	#if NET_BYTE_ORDER == UIP_BIG_ENDIAN
		#define HTON(n) (n)
	#else 
		#define HTON(n) (UINT32)( (((UINT32) (n)) << 24) | ( (((UINT32) (n)) << 8) & 0xFF0000 ) | ( (((UINT32) (n)) >> 8) & 0xFF00 ) | ( ((UINT32) (n) >> 24) ))
	#endif
#else
	#error "HTON already defined!"
#endif 

#ifndef htons
UINT16 htons(UINT16 val);
#endif 

#ifndef ntohs
#define ntohs htons
#endif

#ifndef hton
UINT32 hton(UINT32 val);
#endif 

#ifndef ntoh
#define ntoh hton
#endif

typedef void (*net_soc_cb) (void);

typedef struct _connection_t 
{
	proto_t 	proto;
	soctype_t	type;
	UINT32		flags;
	ipaddr_t 	daddr;
	UINT16    	dport;
	UINT16    	sport;
	byte	 	dmac[NET_MAC_SIZE];
	UINT16		ip_id;
	UINT32		tos;
	UINT32		rcv_buff_size;
	volatile net_cond_t  wait_arp;
	volatile net_cond_t  wait_recv_from;
	net_soc_cb cb;
	net_buff_t *rx_queue;
	net_buff_t *tx_queue;
	PUINT8		header_buffer;
	//cyclic_buffer_t rx_queue_cb;
	//cyclic_buffer_t tx_queue_cb;
}connection_t;

typedef struct _resource_ctx
{
	VUINT32     	lock;
	VUINT32     	irq[CPU_NUM];
	void 			*ptr;
	SYSFPART		headers_part;
	SYSFPART		conn_part;
}resource_ctx;

extern UINT16 net_port_number;
extern connection_t *net_arp_queue[NET_ARP_QUEUE_LEN];
extern connection_t *net_udp_rx_queue[NET_QUEUE_LEN];
extern connection_t *net_raw_rx_queue[NET_QUEUE_LEN];

extern resource_ctx connections_ctx;

MXRC net_init(void);
net_socket net_connection(soctype_t type, proto_t proto);
MXRC net_connect(net_socket soc, net_sockaddr *to);
MXRC net_close(net_socket soc);
MXRC net_bind(net_socket soc, net_sockaddr *addr);
MXRC net_sendto(net_socket soc, net_sockaddr *to, const byte *data, size_t size, int flags);
int net_recv_from(net_socket soc, byte *data, size_t size, net_sockaddr *from, int flags);
MXRC net_send(net_socket soc, const byte *data, size_t size, int flags);
int net_recv(net_socket soc, byte *buff, size_t size, int flags);
MXRC net_set_flags(net_socket soc, int flag, bool enable);
bool net_get_flags(net_socket soc, int flag);
MXRC net_set_opt(net_socket soc, int name, const void *opt, size_t optlen);
MXRC net_get_opt(net_socket soc, int name, const void *opt, size_t *optlen);
int net_select (net_socket *readsoc, size_t size, unsigned long int *timeout);

int net_reg_cb(net_socket soc, net_soc_cb cb);
ipaddr_t net_get_own_addr(void);
void net_get_own_mac(byte *mac);
void net_set_own_addr(ipaddr_t addr);
void net_set_own_mac(byte *mac);


#endif

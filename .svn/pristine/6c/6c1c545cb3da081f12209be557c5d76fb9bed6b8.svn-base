#ifndef _NET_TYPES_H_
#define _NET_TYPES_H_

#include "4gmx.h"

//#define _NET_DEBUG_

typedef UINT32 ipaddr_t;
typedef UINT8  proto_t;
typedef UINT8  soctype_t;
typedef UINT8  byte;
typedef UINT32 net_socket;
typedef UINT32 net_cond_t;

#ifndef  __cplusplus
typedef UINT32 bool;
#endif

#define RC_NETWORK_ALLOC_ERROR         	DEF_USRRC(APP_MODULE, NETWORK_LAYER, 1)
#define RC_NETWORK_SIZE_ERROR         	DEF_USRRC(APP_MODULE, NETWORK_LAYER, 2)
#define RC_NETWORK_NOSOC_ERROR         	DEF_USRRC(APP_MODULE, NETWORK_LAYER, 3)
#define RC_NETWORK_ARP_ERROR         	DEF_USRRC(APP_MODULE, NETWORK_LAYER, 4)
#define RC_NETWORK_NO_ARP	         	DEF_USRRC(APP_MODULE, NETWORK_LAYER, 5)
#define RC_NETWORK_WAIT_ARP	         	DEF_USRRC(APP_MODULE, NETWORK_LAYER, 6)
#define RC_NETWORK_ARP_QUEUE_NSPACE    	DEF_USRRC(APP_MODULE, NETWORK_LAYER, 7)
#define RC_NETWORK_QNS		    		DEF_USRRC(APP_MODULE, NETWORK_LAYER, 8)
#define RC_NETWORK_AGAIN		    	DEF_USRRC(APP_MODULE, NETWORK_LAYER, 9)
#define RC_NETWORK_USED_ERROR	    	DEF_USRRC(APP_MODULE, NETWORK_LAYER, 10)
#define RC_NETWORK_TIMEOUT	    		DEF_USRRC(APP_MODULE, NETWORK_LAYER, 11)
#define RC_NETWORK_QUEUE_ERR    		DEF_USRRC(APP_MODULE, NETWORK_LAYER, 12)
#define RC_NETWORK_INIT_ERR    			DEF_USRRC(APP_MODULE, NETWORK_LAYER, 13)
#define RC_NETWORK_ARP_TO	         	DEF_USRRC(APP_MODULE, NETWORK_LAYER, 14)
#define RC_NETWORK_GEM_INIT_ERR        	DEF_USRRC(APP_MODULE, NETWORK_LAYER, 15)
#define RC_NETWORK_UNKNOWN_API        	DEF_USRRC(APP_MODULE, NETWORK_LAYER, 16)


#define RC_NETWORK_UNKNOWN_ERROR        DEF_USRRC(APP_MODULE, NETWORK_LAYER, 100)

#define true	1
#define false   0
#define NET_DEF_MAC 	0x00, 0x11, 0x22, 0x33, 0x44, 0x55
#define NET_DEF_IP		0x0500A8C0
#define NET_DEF_MASK	0x00FFFFFF
#define NET_DEF_GW		0x0
#define NET_MAC_SIZE	6
#define NET_IP_SIZE		4
#define NET_ETHERNET_MAX_PAYLOAD_SIZE	1500

#define NET_FLAG_NOBLOCK	0x00000001
#define NET_FLAG_BINDED		0x00000002
#define NET_FLAG_CONNECTED	0x00000004

#define NET_CMDFLAG_PEEK	0x00000010

#define NET_OPT_IP_TOS	4
#define NET_OPT_RCVBUF  0x1002

#define NET_DEFAULT_RCV_BUFF_SIZE  0x4000

#define NET_PROTO_TCP	6
#define NET_PROTO_UDP	17

#define  NET_SOCKTYPE_RAW       3
#define  NET_SOCKTYPE_DGRAM     2
#define  NET_SOCKTYPE_STREAM    1

//#define NET_NON_BLOCKING_MODE

#ifdef _NET_DEBUG_
	#define net_uart_printf(...) uart_printf (__VA_ARGS__)
#else
	#define net_uart_printf(...)
#endif

typedef struct _net_conf_t
{
	byte		mac[6];
	ipaddr_t	addr;
	ipaddr_t	mask;
	ipaddr_t	gw;
}net_conf_t;

extern net_conf_t net_conf;

typedef struct _ipv4hostaddr_t
{
	ipaddr_t 	addr;
	UINT16		port;
}ipv4hostaddr_t;



typedef struct _net_sockaddr 
{
	byte	addr_family;
	union{
		byte	data[14];
		ipv4hostaddr_t ipv4addr;
	}addr;
}net_sockaddr;



#endif


#include "arp.h"
#include "ethernet.h"
#include "ip.h"
#include <string.h>
#include "utils.h"

net_arp_entry_t net_arp_tab[NET_ARP_TABLE_SIZE];
connection_t *net_arp_queue[NET_ARP_QUEUE_LEN];

resource_ctx arp_queue_ctx;

static MXRC net_arp_process_wait_queue(ipaddr_t ip);
static net_arp_entry_t *net_arp_find_by_ip(ipaddr_t ip);

MXRC net_arp_init(void)
{
	arp_queue_ctx.lock = 0;
	arp_queue_ctx.ptr = net_arp_queue;
	
	memset(net_arp_tab, 0, sizeof(net_arp_tab));
	//add broadcast entry
	net_arp_tab[0].ip = NET_IP_BROADCAST;
	memset(&net_arp_tab[0].mac, 0xFF, NET_MAC_SIZE);

	return MX_OK;
}

MXRC net_arp_queue_put(connection_t *conn)
{
	net_lock(&arp_queue_ctx);
	
	if(net_queue_put((connection_t **)arp_queue_ctx.ptr, NET_ARP_QUEUE_LEN, conn) == MX_OK)
	{
		net_unlock(&arp_queue_ctx);
		return MX_OK;
	}
			
	net_unlock(&arp_queue_ctx);
	return RC_NETWORK_ARP_QUEUE_NSPACE;
}

connection_t * net_arp_queue_get(void)
{	
	connection_t *res;
	
	net_lock(&arp_queue_ctx);
	
	res = net_queue_get((connection_t **)arp_queue_ctx.ptr, NET_ARP_QUEUE_LEN);
	
	net_unlock(&arp_queue_ctx);
	
	
	return res;
}

MXRC net_arp_upd_entry(ipaddr_t ip, byte *mac)
{
	net_arp_entry_t *res;
	UINT32				i;
	
	res = net_arp_find_by_ip(ip);
		
	net_uart_printf("net_arp_tab addr = %x\r\n", net_arp_tab);
	
	if(!res)
	{				
		for(i = 0; i < NET_ARP_TABLE_SIZE; i++)
		{
			if(net_arp_tab[i].ip == 0)
			{
				res = &net_arp_tab[i];
				break;				
			}
		}				
	}
	
	net_uart_printf("net_arp_tab addr = %x\r\n", res);
	
	net_uart_printf("add ip %x\r\n", ip);
	
	res->ip = ip;

	_memcpy(res->mac, mac, NET_MAC_SIZE);
	
	net_uart_printf("ok\r\n");
	
	return MX_OK;	
}

static net_arp_entry_t *net_arp_find_by_ip(ipaddr_t ip)
{
	UINT32				i;

	for(i = 0; i < NET_ARP_TABLE_SIZE; i++)
	{
		if(net_arp_tab[i].ip == ip)
			return &net_arp_tab[i];
	}
		
	return NULL;	
}

MXRC net_arp_get_mac(ipaddr_t ip, byte *mac)
{
	net_arp_entry_t *res;
	
	net_uart_printf("search mac for ip %x\r\n", ip);
	
	res = net_arp_find_by_ip(ip);
	
	if(!res)
		return RC_NETWORK_NO_ARP;
	
	_memcpy(mac, res->mac, NET_MAC_SIZE);
	
	return MX_OK;
}

MXRC net_arp_request(ipaddr_t ip)
{
	byte packet[ethhdr_size + arphdr_eth_size];
	arphdr_eth_t *arp_header;
	UINT32 size;
	MXRC rc;
	
	size = arphdr_eth_size + ethhdr_size;
	
	arp_header = (arphdr_eth_t *)(packet + ethhdr_size);
	
	arp_header->arp_htype = htons(NET_ETHERNET_PKT_ETHTP);
	arp_header->arp_ptype = htons(NET_ETHERNET_PKT_IP);
	arp_header->arp_hln = NET_MAC_SIZE;
	arp_header->arp_pln = NET_IP_SIZE;
	arp_header->arp_oper = htons(NET_ARP_OPCODE_REQUEST);
	net_get_own_mac(arp_header->arp_sha);
	arp_header->arp_sip = net_get_own_addr();
	memset(arp_header->arp_tha, 0, NET_MAC_SIZE);
	arp_header->arp_tip = ip;
	
	rc = net_ethernet_send_to(net_bcast_mac, NET_ETHERNET_PKT_ARP, packet, size, NULL, 0);
	
	return rc;
}

MXRC net_arp_responce(ipaddr_t ip, byte *mac)
{
	byte packet[ethhdr_size + arphdr_eth_size];
	arphdr_eth_t *arp_header;
	UINT32 size;
	MXRC rc;
	
	size = arphdr_eth_size + ethhdr_size;
			
	arp_header = (arphdr_eth_t *)(packet + ethhdr_size);
	
	arp_header->arp_htype = htons(NET_ETHERNET_PKT_ETHTP);
	arp_header->arp_ptype = htons(NET_ETHERNET_PKT_IP);
	arp_header->arp_hln = NET_MAC_SIZE;
	arp_header->arp_pln = NET_IP_SIZE;
	arp_header->arp_oper = htons(NET_ARP_OPCODE_RESP);
	net_get_own_mac(arp_header->arp_sha);
	arp_header->arp_sip = net_get_own_addr();
	_memcpy(arp_header->arp_tha, mac, NET_MAC_SIZE);
	arp_header->arp_tip = ip;
	
	rc = net_ethernet_send_to(mac, NET_ETHERNET_PKT_ARP, packet, size, NULL, 0);
	
	return rc;
}

MXRC net_arp_parce_pkt(byte *pkt)
{	
	arphdr_eth_t *arp_header;
	
	
	arp_header = (arphdr_eth_t *)pkt;
	
	switch(htons(arp_header->arp_oper))
	{
		case NET_ARP_OPCODE_REQUEST:
			net_uart_printf ("Get arp request\r\n");
			net_uart_printf("my ip %x, req ip %x\r\n", net_get_own_addr(), arp_header->arp_tip);
			if(arp_header->arp_tip == net_get_own_addr())
				net_arp_responce(arp_header->arp_sip, arp_header->arp_sha);			
			break;
		case NET_ARP_OPCODE_RESP:
			net_uart_printf ("Get arp response\r\n");
			net_arp_upd_entry(arp_header->arp_sip, arp_header->arp_sha);
			net_arp_process_wait_queue(arp_header->arp_sip);
			break;
		default:
			break;
	}
	
	return MX_OK;
}

MXRC net_arp_process_wait_queue(ipaddr_t ip)
{
	connection_t 	*conn;
	byte 			*frame;
	UINT32			size;
	iphdr_t			*ip_header;
	
	net_uart_printf ("Proccess wait queue for %x\r\n", ip);
	
	conn = net_arp_queue_get();
		
	while(conn)
	{					
		net_uart_printf ("conn %x\r\n", conn);
		
		if(net_buff_peek(conn->tx_queue, &frame, &size) == MX_OK)
		{
			net_uart_printf ("have frame\r\n");
			
			ip_header = (iphdr_t *)( frame + ethhdr_size);
			
			net_uart_printf ("packet to %x\r\n", ip_header->daddr);
			net_uart_printf("size to send %d \r\n", size);
			
			if(ip_header->daddr == ip)
			{
				net_uart_printf ("sending packet\r\n");
				
				net_cond_send(&conn->wait_arp);									
			}
		}
		conn = net_arp_queue_get();
	}
	
	net_uart_printf ("ok \r\n");
	
	return MX_OK;
}



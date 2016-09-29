#include "ip.h"
#include "ethernet.h"
#include "udp.h"
#include "utils.h"
#include "raw.h"

MXRC net_ip_send_to(connection_t *conn, net_sockaddr *to, byte *hbuff, size_t hsize, const byte *pbuff, const size_t psize)
{
	iphdr_t *ip_header;
	
	net_uart_printf("ip_send_to\r\n");
	
	ip_header = (iphdr_t *)( hbuff + ethhdr_size); 
		
	net_uart_printf("daddr %x\r\n", to->addr.ipv4addr.addr);

	
	ip_header->daddr 	= to->addr.ipv4addr.addr;
	conn->daddr = to->addr.ipv4addr.addr;
	ip_header->saddr 	= net_get_own_addr();
	ip_header->tos		= 0;
	ip_header->version  = 4;
	ip_header->ihl		= 5;
	ip_header->ttl		= NET_IP_TTL;
	ip_header->protocol = conn->proto;		
	ip_header->check	= 0; //net_ip_calc_cs((byte *) ip_header, iphdr_size);
	ip_header->tot_len  = htons(hsize + iphdr_size + psize);
	ip_header->id		= htons(net_ip_get_id(conn));
	
	return net_ethernet_send(conn, NET_ETHERNET_PKT_IP, hbuff, hsize + iphdr_size, pbuff, psize);	
}

MXRC net_ip_send(connection_t *conn, byte *hbuff, size_t hsize, const byte *pbuff, const size_t psize)
{
	iphdr_t *ip_header;
	MXRC rc;

	NET_PROFILE(net_prof_ip_in);
	
	net_uart_printf("ip_send\r\n");
	
	ip_header = (iphdr_t *)( hbuff + ethhdr_size); 
		
	net_uart_printf("daddr %x\r\n", conn->daddr);
	
	if(!ip_header->daddr)
	{
		net_uart_printf("ip first time \r\n");
		ip_header->daddr 	= conn->daddr;
		ip_header->saddr 	= net_get_own_addr();
		ip_header->tos		= 0;
		ip_header->version  = 4;
		ip_header->ihl		= 5;
		ip_header->ttl		= NET_IP_TTL;
		ip_header->protocol = conn->proto;		
		ip_header->check	= 0; //net_ip_calc_cs((byte *) ip_header, iphdr_size);
	}
	ip_header->tot_len  = htons(hsize + iphdr_size + psize);
	ip_header->id		= htons(net_ip_get_id(conn));
	
	rc = net_ethernet_send(conn, NET_ETHERNET_PKT_IP, hbuff, hsize + iphdr_size, pbuff, psize);	

	NET_PROFILE(net_prof_ip_out);

	return rc;
}


UINT16	net_ip_get_id(connection_t *conn)
{
	if(conn->ip_id < NET_MAX_PORT_NUMBER)
			return conn->ip_id++;
	else
	{
		conn->ip_id = 0;
		return conn->ip_id;
	}
}

UINT16 net_ip_calc_cs(byte *buff, size_t size)
{
	return 0;
}

MXRC net_ip_parce_pkt(byte *pkt)
{
	iphdr_t *ip_header;
	
	net_uart_printf("net_ip_parce_pkt\r\n");
	
	ip_header = (iphdr_t *)pkt;
	
	if(ip_header->daddr != net_get_own_addr() && ip_header->daddr != NET_IP_BROADCAST)
		return MX_OK;
	
	if(!net_raw_parce_pkt(pkt, htons(ip_header->tot_len)))
	{
		switch(ip_header->protocol)
		{
		case NET_PROTO_UDP:
			net_uart_printf("net_ip_parce_pkt: UDP\r\n");		
			net_udp_parce_pkt(pkt, htons(ip_header->tot_len));		
			break;
		case NET_PROTO_TCP:
			break;
		default:
			break;
		}
	}
	
	return MX_OK;
}

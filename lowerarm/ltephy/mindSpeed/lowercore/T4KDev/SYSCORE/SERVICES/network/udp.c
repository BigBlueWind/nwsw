
#include "udp.h"
#include "ip.h"
#include "ethernet.h"
#include "utils.h"

connection_t *net_udp_rx_queue[NET_QUEUE_LEN];

resource_ctx udp_queue_ctx;

MXRC net_udp_init(void)
{
	udp_queue_ctx.lock = 0;
	udp_queue_ctx.ptr = net_udp_rx_queue;
	
	return MX_OK;
}

MXRC net_udp_rx_queue_put(connection_t *conn)
{
	net_lock(&udp_queue_ctx);
	
	if(net_queue_put((connection_t **)udp_queue_ctx.ptr, NET_QUEUE_LEN, conn) == MX_OK)
	{
		net_unlock(&udp_queue_ctx);
		return MX_OK;
	}
				
	net_unlock(&udp_queue_ctx);
	return RC_NETWORK_ARP_QUEUE_NSPACE;
}

connection_t * net_udp_rx_queue_get(void)
{
	connection_t *res;
	net_lock(&udp_queue_ctx);
	res = net_queue_get((connection_t **)udp_queue_ctx.ptr, NET_QUEUE_LEN);
	net_unlock(&udp_queue_ctx);
	
	return res;
}

connection_t ** net_udp_rx_queue_get_first(void)
{
	connection_t **res;
	net_lock(&udp_queue_ctx);
	res = net_queue_get_first((connection_t **)udp_queue_ctx.ptr, NET_QUEUE_LEN);
	net_unlock(&udp_queue_ctx);
	
	return res;
}

connection_t ** net_udp_rx_queue_get_next(connection_t **prev)
{
	connection_t **res;
	net_lock(&udp_queue_ctx);
	res = net_queue_get_next((connection_t **)udp_queue_ctx.ptr, prev, NET_QUEUE_LEN);
	net_unlock(&udp_queue_ctx);
	
	return res;
}

MXRC net_udp_rx_queue_remove(connection_t *conn)
{
	MXRC res;
	net_lock(&udp_queue_ctx);	
	res = net_queue_remove((connection_t **)udp_queue_ctx.ptr, conn, NET_QUEUE_LEN);
	net_unlock(&udp_queue_ctx);
	
	return res;
}

MXRC net_udp_send(connection_t *conn, const byte *data, size_t size, int flags)
{
	UINT32 buff_size;
	UINT32 udp_packet_size;
	UINT32 buff_hdr_size;
	byte *buff;
	udphdr_t  *udp_packet_hdr;

	NET_PROFILE(net_prof_udp_in);
	
	net_uart_printf("udp_sendex function \r\n");
	
	buff_hdr_size = udphdr_size + iphdr_size + ethhdr_size;	
	buff_size = buff_hdr_size + size;
	udp_packet_size = udphdr_size + size;
	
	if(buff_size > NET_ETHERNET_MAX_PAYLOAD_SIZE)
	{
		net_uart_printf("payload too big %d \r\n", buff_size);
		return RC_NETWORK_SIZE_ERROR;
	}		
	
	buff = conn->header_buffer;

	udp_packet_hdr = (udphdr_t *)(buff + (buff_hdr_size - udphdr_size));

	if(!udp_packet_hdr->source)
	{	
		net_uart_printf("udp first time \r\n");
		udp_packet_hdr->source	= conn->sport;
		udp_packet_hdr->dest 	= conn->dport;
		udp_packet_hdr->check	= 0;
	}
	udp_packet_hdr->len 	= htons(udp_packet_size);
		
	net_ip_send(conn, buff, udphdr_size, data, size);

	NET_PROFILE(net_prof_udp_out);
	
	return MX_OK;
}


MXRC net_udp_send_to(connection_t *conn, net_sockaddr *to, const byte *data, size_t size, int flags)
{
	UINT32 buff_size;
	UINT32 udp_packet_size;
	UINT32 buff_hdr_size;
	byte *buff;
	udphdr_t  *udp_packet_hdr;

	net_uart_printf("udp_send_toex function \r\n");
	
	buff_hdr_size = udphdr_size + iphdr_size + ethhdr_size;	
	buff_size = buff_hdr_size + size;
	udp_packet_size = udphdr_size + size;
	
	if(buff_size > NET_ETHERNET_MAX_PAYLOAD_SIZE)
	{
		net_uart_printf("payload too big %d \r\n", buff_size);
		return RC_NETWORK_SIZE_ERROR;
	}		
	
	buff = conn->header_buffer;

	udp_packet_hdr = (udphdr_t *)(buff + (buff_hdr_size - udphdr_size));

	udp_packet_hdr->source	= conn->sport;
	udp_packet_hdr->dest 	= to->addr.ipv4addr.port;
	udp_packet_hdr->check	= 0;
	udp_packet_hdr->len 	= htons(udp_packet_size);	
		
	net_ip_send_to(conn, to,  buff, udphdr_size, data, size);
	
	return MX_OK;
}


int net_udp_recv_from(connection_t *conn, byte *buff, size_t size, net_sockaddr *addr, int flags)
{
	udphdr_t *udp_header;
	iphdr_t  *ip_header;
	byte *frame = NULL;
	size_t frame_size;
	size_t packet_size;
	size_t headers_size;
	size_t size_to_copy;

	if(net_buff_empty(conn->rx_queue))
	{
        if(conn->flags & NET_FLAG_NOBLOCK)
            return 0;        

		net_uart_printf("net_udp_recv_from: wait\r\n");
		net_uart_printf("net_udp_recv_from: conn = %x\r\n", conn);
		net_cond_wait(&(conn->wait_recv_from));			
	}

	net_uart_printf("net_udp_recv_from: get data\r\n");

	if(flags & NET_CMDFLAG_PEEK)
		net_buff_peek(conn->rx_queue, &frame, &frame_size);
	else
		net_buff_get(conn->rx_queue, &frame, &frame_size);
	if(!frame)
	{
		net_uart_printf("net_udp_recv_from: NO DATA\r\n");
		return -1;
	}
	net_cond_init(&(conn->wait_recv_from));
	headers_size = udphdr_size + iphdr_size;
	packet_size = frame_size - headers_size;
	ip_header = (iphdr_t *)frame;
	udp_header = (udphdr_t *)(frame + iphdr_size);
	
	size_to_copy = (size >= packet_size) ? packet_size : size;
	
	net_uart_printf("net_udp_recv_from: packet_size %d, size_to_copy %d\r\n", packet_size, size_to_copy);
	
	_memcpy(buff, frame + headers_size, size_to_copy);
	
	if(addr)
	{
		addr->addr.ipv4addr.port = udp_header->source;
		addr->addr.ipv4addr.addr = ip_header->saddr;
		net_uart_printf("net_udp_recv_from: from %x:%d\r\n", addr->addr.ipv4addr.addr, addr->addr.ipv4addr.port);
	}
	
	net_free(frame);
				
	net_uart_printf("net_recv_from: DATA\r\n");
				
	net_dump_data(buff, size_to_copy);
	
	return size_to_copy;
}

MXRC net_udp_parce_pkt(byte *pkt, size_t size)
{
	byte * frame;
	connection_t *conn;
	connection_t **conn_pointer;
	udphdr_t *udp_header;
	iphdr_t *ip_header;
	
	net_uart_printf("net_udp_parce_pkt\r\n");
	
	if(!pkt || !size)
	{
		return RC_NETWORK_UNKNOWN_ERROR;		
	}
		
	udp_header = (udphdr_t *)(pkt + iphdr_size);
	ip_header = (iphdr_t *)pkt;
	
	net_uart_printf("net_udp_parce_pkt: inspect bind sockets\r\n");
		
	conn_pointer = net_udp_rx_queue_get_first();
		 		
	while(conn_pointer )
	{
		net_uart_printf("net_udp_parce_pkt: conn_pointer %x\r\n", conn_pointer);
		
		if( (*conn_pointer) && (*conn_pointer)->sport == udp_header->dest)
		{
			if((*conn_pointer)->flags & NET_FLAG_CONNECTED)
			{
				if((*conn_pointer)->dport == udp_header->source && (*conn_pointer)->daddr == ip_header->saddr )
					break;
			}
			else
				break;			
		}	
		
		conn_pointer = net_udp_rx_queue_get_next(conn_pointer);
	};
		
	if(!conn_pointer)
	{
		// nobady wait packet
		net_uart_printf("net_udp_parce_pkt: no nobady wait\r\n");
		return MX_OK;		
	}
	
	conn = *conn_pointer;

	net_uart_printf("net_udp_parce_pkt: size %d\r\n", size);
	
	frame = (byte *)net_alloc(size);

	if (!frame)
	{
		return RC_NETWORK_ALLOC_ERROR;		
	}	

	_memcpy(frame, pkt, size);
		
	net_buff_put(conn->rx_queue, frame, size);
	net_uart_printf("net_udp_parce_pkt: send signal\r\n");
	net_cond_send(&(conn->wait_recv_from));
	net_uart_printf("net_udp_parce_pkt: conn = %x (%d)\r\n", conn, conn->wait_recv_from);
	
	if(conn->cb)
	{		
		conn->cb();	
	}

	return MX_OK;
}


MXRC net_udp_bind(connection_t *conn, net_sockaddr *addr)
{
	connection_t **conn_pointer;
	
	
	if(!conn)
		return RC_NETWORK_UNKNOWN_ERROR;
	
	
	conn_pointer = net_udp_rx_queue_get_first();
			 		
	while(conn_pointer )
	{
		//net_uart_printf("net_udp_bind: conn_pointer %x\r\n", conn_pointer);
			
		if( (*conn_pointer) && (*conn_pointer)->sport == addr->addr.ipv4addr.port)
			return RC_NETWORK_USED_ERROR;	
			
		conn_pointer = net_udp_rx_queue_get_next(conn_pointer);
	};
		
	conn->flags |= NET_FLAG_BINDED; 
	conn->sport = addr->addr.ipv4addr.port;
	
	return MX_OK;
}

MXRC net_udp_close(connection_t *conn)
{
	return net_udp_rx_queue_remove(conn);
}


MXRC net_udp_connect(connection_t *conn, net_sockaddr *to)
{
	if(!conn)
		return RC_NETWORK_UNKNOWN_ERROR;
	
	if(conn->flags & NET_FLAG_CONNECTED)
		return RC_NETWORK_UNKNOWN_ERROR;
	
	conn->dport = to->addr.ipv4addr.port;
	conn->daddr = to->addr.ipv4addr.addr;
	
	conn->flags |= NET_FLAG_CONNECTED;
	
	return MX_OK;
}

int net_udp_recv(connection_t *conn, byte *buff, size_t size, int flags)
{
	byte *frame = NULL;
	size_t frame_size = 0;
	size_t packet_size;
	size_t headers_size;
	size_t size_to_copy;
	
	if(net_buff_empty(conn->rx_queue))
	{			
		if(conn->flags & NET_FLAG_NOBLOCK)
			return 0;
        
		net_uart_printf("net_recv: wait\r\n");
		net_cond_wait(&(conn->wait_recv_from));			
	}
	net_uart_printf("net_recv: get data\r\n");
	if(flags & NET_CMDFLAG_PEEK)
		net_buff_peek(conn->rx_queue, &frame, &frame_size);
	else
		net_buff_get(conn->rx_queue, &frame, &frame_size);
	if(!frame)
	{
		net_uart_printf("net_recv: NO DATA\r\n");
		return -1;
	}
	net_cond_init(&(conn->wait_recv_from));

	headers_size = udphdr_size + iphdr_size;
	packet_size = frame_size - headers_size;
					
	size_to_copy = (size >= packet_size) ? packet_size : size;
				
	_memcpy(buff, frame + headers_size, size_to_copy);
	
	net_free(frame);
				
	net_uart_printf("net_recv: DATA\r\n");
				
	net_dump_data(buff, size_to_copy);
	
	return size_to_copy;
}

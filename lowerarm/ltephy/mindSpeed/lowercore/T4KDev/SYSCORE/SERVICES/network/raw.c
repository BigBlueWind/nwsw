#include "raw.h"
#include "ip.h"
#include "ethernet.h"
#include "utils.h"

connection_t *net_raw_rx_queue[NET_QUEUE_LEN];

resource_ctx net_raw_queue_ctx;


MXRC net_raw_init(void)
{
	net_raw_queue_ctx.lock = 0;
	net_raw_queue_ctx.ptr = net_raw_rx_queue;
	
	return MX_OK;
}

MXRC net_raw_rx_queue_put(connection_t *conn)
{
	net_lock(&net_raw_queue_ctx);
	
	if(net_queue_put((connection_t **)net_raw_queue_ctx.ptr, NET_QUEUE_LEN, conn) == MX_OK)
	{
		net_unlock(&net_raw_queue_ctx);
		return MX_OK;
	}
				
	net_unlock(&net_raw_queue_ctx);
	
	return RC_NETWORK_ARP_QUEUE_NSPACE;
}

connection_t * net_raw_rx_queue_get(void)
{
	connection_t *res;
	net_lock(&net_raw_queue_ctx);
	
	res =  net_queue_get((connection_t **)net_raw_queue_ctx.ptr, NET_QUEUE_LEN);
	
	net_unlock(&net_raw_queue_ctx);
	
	return res;
}

connection_t ** net_raw_rx_queue_get_first(void)
{
	connection_t **res;
	
	net_lock(&net_raw_queue_ctx);
	
	res = net_queue_get_first((connection_t **)net_raw_queue_ctx.ptr, NET_QUEUE_LEN);
	
	net_unlock(&net_raw_queue_ctx);
	
	return res;
}

connection_t ** net_raw_rx_queue_get_next(connection_t **prev)
{
	connection_t **res;
	
	net_lock(&net_raw_queue_ctx);
	
	res = net_queue_get_next((connection_t **)net_raw_queue_ctx.ptr, prev, NET_QUEUE_LEN);
	
	net_unlock(&net_raw_queue_ctx);
	
	return res;
}

MXRC net_raw_rx_queue_remove(connection_t *conn)
{
	MXRC res;
	
	net_lock(&net_raw_queue_ctx);
	
	res = net_queue_remove((connection_t **)net_raw_queue_ctx.ptr, conn, NET_QUEUE_LEN);
	
	net_unlock(&net_raw_queue_ctx);
	
	return res;
}

bool net_raw_parce_pkt(byte *pkt, size_t size)
{

	byte * frame;
	connection_t *conn;
	connection_t **conn_pointer;
	iphdr_t *ip_header;
	
	net_uart_printf("net_raw_parce_pkt\r\n");
	
	if(!pkt || !size)
	{
		return false;		
	}

	ip_header = (iphdr_t *)pkt;
	
	net_uart_printf("net_raw_parce_pkt: inspect bind sockets\r\n");
		
	conn_pointer = net_raw_rx_queue_get_first();
		 		
	while(conn_pointer )
	{
		net_uart_printf("net_raw_parce_pkt: conn_pointer %x\r\n", conn_pointer);
		
		if( (*conn_pointer) && (*conn_pointer)->proto == ip_header->protocol)
		{
			break;
		}	
		
		conn_pointer = net_raw_rx_queue_get_next(conn_pointer);
	};
	
	
	if(!conn_pointer)
	{
		// nobady wait packet
		net_uart_printf("net_raw_parce_pkt: no nobady wait\r\n");
		return false;		
	}
	
	conn = *conn_pointer;

	net_uart_printf("net_raw_parce_pkt: size %d\r\n", size);
	
	frame = (byte *)net_alloc(size);
		
	if (!frame)
	{
		return RC_NETWORK_ALLOC_ERROR;		
	}
		
	_memcpy(frame, pkt, size);
		
	net_buff_put(conn->rx_queue, frame, size);
	net_uart_printf("net_raw_parce_pkt: send signal\r\n");
	net_uart_printf("net_raw_parce_pkt: conn = %x\r\n", conn);
	net_cond_send(&(conn->wait_recv_from));	
	
	if(conn->cb)
		conn->cb();
	
	return true;
}

MXRC net_raw_bind(connection_t *conn, net_sockaddr *addr)
{
	return MX_OK;
}

MXRC net_raw_close(connection_t *conn)
{
	return MX_OK;
}

MXRC net_raw_connect(connection_t *conn, net_sockaddr *to)
{
	return MX_OK;
}

MXRC net_raw_send_to(connection_t *conn, net_sockaddr *to, const byte *data, size_t size, int flags)
{	
	net_uart_printf("raw_send_to function \r\n");
	
	if(size > NET_ETHERNET_MAX_PAYLOAD_SIZE)
	{
		net_uart_printf("payload too big %d \r\n", size);
		return RC_NETWORK_SIZE_ERROR;
	}		

	return net_ip_send_to(conn, to, conn->header_buffer, 0, data, size);	
}

int net_raw_recv_from(connection_t *conn, byte *buff, size_t size, net_sockaddr *addr, int flags)
{
	iphdr_t  *ip_header;
	byte *frame = NULL;
	size_t frame_size = 0;
	size_t packet_size;
	size_t size_to_copy;
	
	if(net_buff_empty(conn->rx_queue))
	{
        if(conn->flags & NET_FLAG_NOBLOCK)
            return 0;        

		net_uart_printf("net_raw_recv_from: wait\r\n");
		net_uart_printf("net_raw_recv_from: conn = %x\r\n", conn);
		net_cond_wait(&(conn->wait_recv_from));			
	}
	net_uart_printf("net_raw_recv_from: get data\r\n");
	if(flags & NET_CMDFLAG_PEEK)
		net_buff_peek(conn->rx_queue, &frame, &frame_size);
	else
		net_buff_get(conn->rx_queue, &frame, &frame_size);
	if(!frame)
	{
		net_uart_printf("net_raw_recv_from: NO DATA\r\n");
		return -1;
	}
	packet_size = frame_size;
	ip_header = (iphdr_t *)frame;
		
	size_to_copy = (size >= packet_size) ? packet_size : size;
	
	net_uart_printf("net_recv_from: packet_size %d, size_to_copy %d\r\n", packet_size, size_to_copy);
	
	_memcpy(buff, frame, size_to_copy);

	if(addr)
	{
		addr->addr.ipv4addr.addr = ip_header->saddr;
		net_uart_printf("net_raw_recv_from: from %x:%d\r\n", addr->addr.ipv4addr.addr, addr->addr.ipv4addr.port);
	}
	
	net_free(frame);
				
	net_uart_printf("net_raw_recv_from: DATA\r\n");
				
	//net_dump_data(buff, size_to_copy);
	
	return size_to_copy;
}

MXRC net_raw_send(connection_t *conn, const byte *data, size_t size, int flags)
{
	return RC_NETWORK_UNKNOWN_ERROR;
}

int net_raw_recv(connection_t *conn, byte *buff, size_t size, int flags)
{
	return net_raw_recv_from(conn, buff, size, NULL, flags);
}

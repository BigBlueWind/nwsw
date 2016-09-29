#include "network.h"
#include "lcorecfg.h"
#include "hw.h"
#include "heaps.h"
#include "udp.h"
#include "arp.h"
#include "ethernet.h"
#include "ip.h"
#include "raw.h"
#include "utils.h"
#include "dhcp/dhcp.h"
#include "supervisor.h"

//UINT32 net_prof[NET_PROF_SIZE];

connection_t *connections[NET_MAX_CONN_NUMBER];
UINT16 net_port_number = NET_START_PORT_NUMBER;

resource_ctx connections_ctx;

int net_inited = 0;

APIHEC net_api_handler (LPVOID pCtx, ApiHeader * pApi, ApiHeader * pResp);

__align (128) UINT8 NCNBVAR headers_part [NET_MAX_CONN_NUMBER * 128];
UINT8 NCNBVAR conn_part [NET_MAX_CONN_NUMBER * sizeof(connection_t)];

net_conf_t net_conf = 
{
	0,
	0,
	0,
	0
};

UINT16 net_assign_port(void)
{
	if(net_port_number < NET_MAX_PORT_NUMBER)
		return net_port_number++;
	else
	{
		net_port_number = NET_START_PORT_NUMBER;
		return net_port_number;
	}
}

/*
 * This function is helper, it is not treadsafe, dont call it directly
 * 
 */ 
UINT32 net_find_free_socket(void)
{
	UINT32 id = 0;
	connection_t  **conn_array = (connection_t **)(connections_ctx.ptr); 
	
	for(; id < NET_MAX_CONN_NUMBER; id++)
		if(conn_array[id] == NULL)
			return ++id;
	
	return NET_NO_SOCKETS;
}

connection_t *net_get_conn(net_socket soc)
{
	connection_t  **conn_array = (connection_t **)(connections_ctx.ptr); 
	connection_t  *conn = NULL;
	
	if(soc > NET_MAX_CONN_NUMBER)
		return NULL;
	
	net_lock(&connections_ctx);
	
	conn = conn_array[--soc]; 
	                  
	net_unlock(&connections_ctx);
	
	return conn;
}

ipaddr_t net_get_own_addr(void)
{
	return net_conf.addr;
}

void net_get_own_mac(byte *mac)
{
	_memcpy(mac, net_conf.mac, NET_MAC_SIZE);
}

void net_set_own_addr(ipaddr_t addr)
{
	net_conf.addr = addr;
}

void net_set_own_mac(byte *mac)
{
	_memcpy(net_conf.mac, mac, NET_MAC_SIZE);
}

MXRC net_init(void)
{	
	MXRC rc = MX_OK;

	memset(connections, NULL, sizeof(connection_t *) * NET_MAX_CONN_NUMBER);
	memset(net_arp_queue, NULL, sizeof(connection_t *) * NET_ARP_QUEUE_LEN);
	memset(net_udp_rx_queue, NULL, sizeof(connection_t *) * NET_QUEUE_LEN);
	
	connections_ctx.lock = 0;
	connections_ctx.ptr = connections;
	
	MxiDefSysPart(&(connections_ctx.headers_part), headers_part, 128, NET_MAX_CONN_NUMBER);
	MxiDefSysPart(&(connections_ctx.conn_part), conn_part, sizeof(connection_t), NET_MAX_CONN_NUMBER);
	
	_memcpy(&net_conf.mac, &CoreCfg.MacAddr, NET_MAC_SIZE);

	net_uart_printf("MAC:");
	net_dump_data(net_conf.mac, NET_MAC_SIZE);

	if((rc = net_hw_init()) != MX_OK)
		return rc;
	net_arp_init();
	net_udp_init();
	net_raw_init();
	
	if(CoreCfg.IPAddr)
		net_conf.addr = (ipaddr_t)CoreCfg.IPAddr;
	else
	{
		int attempts = 3;
		rc = 1;

		while (rc != MX_OK && attempts--)					
			rc = dhcp_request(&net_conf);		
	}

	if(!SUCCESSFUL(rc))
		return RC_NETWORK_INIT_ERR;

    rc = SvsrRegClient (IID_NETWORK, net_api_handler, NULL);

	net_inited = 1;

	return MX_OK;	
}


net_socket net_connection(soctype_t type, proto_t proto)
{
	connection_t *new_connection;
	
	connection_t  **conn_array = (connection_t **)(connections_ctx.ptr); 
	net_socket new_soc;
	
	net_uart_printf("net_connection function \r\n");
	
	net_lock(&connections_ctx);
	
	new_soc = net_find_free_socket();
	
	if(new_soc  == NET_NO_SOCKETS)
	{
		net_uart_printf("cannot find free sockets \r\n");
		net_unlock(&connections_ctx);
		return NET_NO_SOCKETS;
	}
	
	net_uart_printf("socket %d \r\n", new_soc);
	
	//conn_array[new_soc - 1] = net_alloc(sizeof(connection_t));
	conn_array[new_soc - 1] = (connection_t *)MxiAllocSysPart(&(connections_ctx.conn_part));
	new_connection = conn_array[new_soc - 1];
		
	net_unlock(&connections_ctx);
	
	if(!new_connection)
	{
		net_uart_printf("cannot allocate \r\n");
		return NET_NO_SOCKETS;
	}
	
	//memset(new_connection, 0, sizeof(connection_t));
	net_cond_init(&(new_connection->wait_arp));
	net_cond_init(&(new_connection->wait_recv_from));
	new_connection->header_buffer = (PUINT8)MxiAllocSysPart(&(connections_ctx.headers_part));
	memset(new_connection->header_buffer, 0, ethhdr_size + iphdr_size + udphdr_size);

	if(!new_connection->header_buffer)
	{
		net_uart_printf("cannot allocate header buffer \r\n");
		return NET_NO_SOCKETS;
	}

	new_connection->header_buffer[0] = 0;
	new_connection->header_buffer[5] = 0;
	
	new_connection->rx_queue = net_buff_create();
	if(!new_connection->rx_queue)
	{
		net_uart_printf("cannot create rx queue \r\n");
		return NET_NO_SOCKETS;
	}
	new_connection->tx_queue = net_buff_create();
	if(!new_connection->tx_queue)
	{
		net_uart_printf("cannot create tx queue \r\n");
		return NET_NO_SOCKETS;
	}

	new_connection->type = type;
	new_connection->proto = proto;
	new_connection->tos = NET_IP_DEFAULT_TOS;
	new_connection->rcv_buff_size = NET_DEFAULT_RCV_BUFF_SIZE;
	new_connection->ip_id = 0;
		
	switch(type)
	{	
	case NET_SOCKTYPE_DGRAM:
		new_connection->sport = htons(net_assign_port());
		net_udp_rx_queue_put(new_connection);							
		break;
	case NET_SOCKTYPE_RAW:
		net_raw_rx_queue_put(new_connection);
		break;
	}

	net_buff_init(new_connection->rx_queue);
	net_buff_init(new_connection->tx_queue);
	
	return new_soc;
}

MXRC net_close(net_socket soc)
{
	connection_t  **conn_array = (connection_t **)(connections_ctx.ptr); 
	connection_t *conn;
	
	conn = net_get_conn(soc);
	
	if(!conn)
		return RC_NETWORK_NOSOC_ERROR;
	
	//TO DO need remove from hw level
	
	switch(conn->type)
	{
		case NET_SOCKTYPE_DGRAM:			
			net_udp_close(conn);
			break;
		case NET_SOCKTYPE_RAW:	
			net_raw_rx_queue_remove(conn);
			break;
		default:
			break;	
	}
	
	net_buff_destroy(conn->rx_queue);
	net_buff_destroy(conn->tx_queue);

	MxiFreeSysPart(&(connections_ctx.headers_part), conn->header_buffer);
	MxiFreeSysPart(&(connections_ctx.conn_part), conn);
	
	net_lock(&connections_ctx);
	conn_array[--soc] = NULL;
	net_unlock(&connections_ctx);
	
	return MX_OK;
}

MXRC net_sendto(net_socket soc, net_sockaddr *to, const byte *data, size_t size, int flags)
{
	connection_t *conn;

	net_uart_printf("net_sendtoex function \r\n");
	
	conn = net_get_conn(soc);
	
	if(!conn)
	{
		net_uart_printf("no socket \r\n");
		return RC_NETWORK_NOSOC_ERROR;
	}
	
	net_uart_printf("soc %d \r\n", soc);
	net_uart_printf("proto %d \r\n", conn->proto);
	
	
	switch(conn->type)
	{
	case NET_SOCKTYPE_RAW:
		{
			net_uart_printf("to raw sendto \r\n");
			return net_raw_send_to(conn, to, data, size, flags);
		}		
	case NET_SOCKTYPE_DGRAM:
		{
			net_uart_printf("to udp sendto ex \r\n");
			return net_udp_send_to(conn, to,  data, size, flags);
		}						
	
	default:
		 net_uart_printf("unknown socktype \r\n");
		 return RC_NETWORK_UNKNOWN_ERROR;
	}
		
}


int net_recv_from(net_socket soc, byte *data, size_t size, net_sockaddr *from, int flags)
{	
	connection_t *conn;
	
	//net_uart_printf("net_recv_from\r\n");
	
	conn = net_get_conn(soc);
		
	if(!conn)
	{
		net_uart_printf("no socket \r\n");
		return -1;
	}
		
	//net_uart_printf("soc %d \r\n", soc);
	//net_uart_printf("proto %d \r\n", conn->proto);
	
	switch(conn->type)
	{
	case NET_SOCKTYPE_DGRAM:				
			return net_udp_recv_from(conn, data, size, from, flags);								
	case NET_SOCKTYPE_RAW:
			return net_raw_recv_from(conn, data, size, from, flags);
	default:
		break;	
	}
	
	return 0;
}

MXRC net_bind(net_socket soc, net_sockaddr *addr)
{
	connection_t *conn;
		
	net_uart_printf("net_bind\r\n");
			
	conn = net_get_conn(soc);
			
	if(!conn)
	{
		net_uart_printf("no socket \r\n");
		return RC_NETWORK_NOSOC_ERROR;
	}
	
	switch(conn->type)
		{
		case NET_SOCKTYPE_DGRAM:				
				return net_udp_bind(conn, addr);								
		case NET_SOCKTYPE_RAW:
			break;
		default:
			break;	
		}
	
	return MX_OK;
}


MXRC net_connect(net_socket soc, net_sockaddr *to)
{
	connection_t *conn;
			
	net_uart_printf("net_connect\r\n");
				
	conn = net_get_conn(soc);
				
	if(!conn)
	{
		net_uart_printf("no socket \r\n");
		return RC_NETWORK_NOSOC_ERROR;
	}
		
	switch(conn->type)
	{
		case NET_SOCKTYPE_DGRAM:				
			return net_udp_connect(conn, to);								
		case NET_SOCKTYPE_RAW:
			break;
		default:
			break;	
	}
		
	return MX_OK;
}

MXRC net_send(net_socket soc, const byte *data, size_t size, int flags)
{
	connection_t *conn;

	NET_PROFILE(net_prof_net_in);
	
	net_uart_printf("net_send ex function \r\n");
	
	conn = net_get_conn(soc);
	
	if(!conn && !conn->flags & NET_FLAG_CONNECTED)
	{
		net_uart_printf("no socket \r\n");
		return RC_NETWORK_NOSOC_ERROR;
	}
	
	net_uart_printf("soc %d \r\n", soc);
	net_uart_printf("proto %d \r\n", conn->proto);
	
	
	switch(conn->type)
	{
	case NET_SOCKTYPE_DGRAM:
		{
			net_uart_printf("to udp send ex \r\n");
			net_udp_send(conn, data, size, flags);
		}					
		break;
	case NET_SOCKTYPE_RAW:
		{
			net_uart_printf("to raw \r\n");			
		}					
		break;
	
	default:
		 return RC_NETWORK_UNKNOWN_ERROR;
	}

	NET_PROFILE(net_prof_net_out);
	return MX_OK;
}



int net_recv(net_socket soc, byte *buff, size_t size, int flags)
{	
	connection_t *conn;
	
	net_uart_printf("net_recv_from\r\n");
	
	conn = net_get_conn(soc);
		
	if(!conn && !conn->flags & NET_FLAG_CONNECTED)
	{
		net_uart_printf("no socket \r\n");
		return -1;
	}
		
	net_uart_printf("soc %d \r\n", soc);
	net_uart_printf("proto %d \r\n", conn->proto);
	
	switch(conn->type)
	{
	case NET_SOCKTYPE_DGRAM:				
			return net_udp_recv(conn, buff, size, flags);								
	case NET_SOCKTYPE_RAW:
			return net_raw_recv(conn, buff, size, flags);	
	default:
		break;	
	}
	
	return 0;
}

MXRC net_set_flags(net_socket soc, int flag, bool enable)
{
	connection_t *conn;

	net_uart_printf("net_set_flags function \r\n");
	
	conn = net_get_conn(soc);
	
	if(!conn)
	{
		net_uart_printf("no socket \r\n");
		return RC_NETWORK_NOSOC_ERROR;
	}	
			
	if(enable)	
		conn->flags |= flag;
	else
		conn->flags &= !flag;
	
	return MX_OK;
}

bool net_get_flags(net_socket soc, int flag)
{
	connection_t *conn;

	net_uart_printf("net_get_flags function \r\n");
	
	conn = net_get_conn(soc);
	
	if(!conn)
	{
		net_uart_printf("no socket \r\n");
		return false;
	}	
	
	return conn->flags & flag;	
}

MXRC net_set_opt(net_socket soc, int name, const void *opt, size_t optlen)
{
	connection_t *conn;

	net_uart_printf("net_set_flags function \r\n");
		
	conn = net_get_conn(soc);
		
	if(!conn)
	{
		net_uart_printf("no socket \r\n");
		return RC_NETWORK_NOSOC_ERROR;
	}	
				
	switch(name)
	{
	case NET_OPT_IP_TOS:
		conn->tos = *(UINT32 *)opt;
		break;
	case NET_OPT_RCVBUF:
		conn->rcv_buff_size = *(UINT32 *)opt;
		break;
	default:
		return RC_NETWORK_UNKNOWN_ERROR;	
	}	
	
	return MX_OK;
}

MXRC net_get_opt(net_socket soc, int name, const void *opt, size_t *optlen)
{
	connection_t *conn;

	net_uart_printf("net_set_flags function \r\n");
			
	conn = net_get_conn(soc);
			
	if(!conn)
	{
		net_uart_printf("no socket \r\n");
		return RC_NETWORK_NOSOC_ERROR;
	}	
					
	switch(name)
	{
	case NET_OPT_IP_TOS:
		*(UINT32 *)opt = conn->tos;
		*optlen = sizeof(UINT32);
		break;
	case NET_OPT_RCVBUF:
		*(UINT32 *)opt = conn->rcv_buff_size;
		*optlen = sizeof(UINT32);
		break;
	default:
		return RC_NETWORK_UNKNOWN_ERROR;	
	}	
		
	return MX_OK;
}

int net_select (net_socket *readsoc, size_t size, unsigned long int *timeout)
{
	int i;
	int readycon = 0;
	int need_break = 0;
	int start_time = 0;
	net_socket first_sock = 0;
	connection_t *conn;
	net_socket given_conn[NET_MAX_CONN_NUMBER];
	
	if(size >= NET_MAX_CONN_NUMBER)
		return -1;
	
	_memcpy(given_conn, readsoc, size * sizeof(net_socket));
		
	memset(readsoc, 0, sizeof(net_socket) * size);
		
	if(timeout)
		start_time = MxGetTime();
	
	while(1)
	{
		for(i = 0; i < size; i++)
		{			
			if(first_sock == given_conn[i])
			{
				need_break++;
				break;
			}
			
			conn = net_get_conn(given_conn[i]);
			
			if(!conn)
			{
				net_uart_printf("no socket \r\n");
				return -1;
			}						
			
			if(!net_buff_empty(conn->rx_queue))
			{				
				readsoc[readycon] = given_conn[i];
				first_sock = given_conn[i]; 
				readycon++;				
			}						
		}
		
		if(timeout)
		{
			if(MxGetTime() - start_time > *timeout);
			{
				*timeout = 0;
				break;
			}				
		}
		
		if(need_break)
		{
			if(timeout)
				*timeout -= MxGetTime() - start_time;
			break;
		}
	}
	
	return readycon;
}

int net_reg_cb(net_socket soc, net_soc_cb cb)
{
	connection_t *conn;

	net_uart_printf("net_set_flags function \r\n");
				
	conn = net_get_conn(soc);
				
	if(!conn)
	{
		net_uart_printf("no socket \r\n");
		return RC_NETWORK_NOSOC_ERROR;
	}	
	
	conn->cb = cb;
	
	return MX_OK;
}


APIHEC net_api_handler (LPVOID pCtx, ApiHeader * pApi, ApiHeader * pResp)
{
    MXRC rc = RC_NETWORK_UNKNOWN_API;

    // Control interface initialization
    if (IS_CFG_API(pApi, NET_API_SETIP))
    {
        UINT32 ip = ApiGetParamU32(pApi, 1, 0);    
		net_conf.addr = (ipaddr_t)ip;
		rc = MX_OK;
    }
    else if(IS_CFG_API(pApi, NET_API_SETMAC))
    {
        ApiParam *mac = ApiGetParam(pApi, 1);   
		_memcpy(&net_conf.mac, mac->ParamData, NET_MAC_SIZE);		
		rc = MX_OK;
    }

    ApiAddErrorCode(pResp, rc);

    return APIHEC_OK;
}


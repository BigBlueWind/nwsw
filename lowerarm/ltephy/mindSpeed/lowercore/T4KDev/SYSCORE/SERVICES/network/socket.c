#include "socket.h"
#include "network.h"

//*****************************************************************************

int socket_errno = 0;

int getpeername(int fd, sockaddr *addr, socklen_t *len)
{
	return 0;
}

int getsockname(int fd, sockaddr *address, socklen_t *address_len)
{
	return 0;
}

int socket(int domain, int type, int protocol)
{
	if(domain != AF_INET)
		return -1;
	
	switch(type)
	{
	case SOCK_DGRAM:
		if(!protocol)
			protocol = IPPROTO_UDP;		
		break;
	case SOCK_RAW:
		break;
	default:
		return -1;
	
	}

	return net_connection(type, protocol);
}

int close(int sd)
{
	if(net_close(sd) == MX_OK)	
		return 0;
	else 
		return -1;
}

int bind(int sd, const sockaddr *addr, int addrlen)
{
	net_sockaddr tmp;
	tmp.addr.ipv4addr.addr = ((sockaddr_in *)addr)->sin_addr.s_addr;
	tmp.addr.ipv4addr.port = ((sockaddr_in *)addr)->sin_port;
	
	if(net_bind(sd, &tmp) == MX_OK)
		return 0;
	else
		return -1; 
}

int connect(int sd, const sockaddr *addr, int addrlen)
{
	net_sockaddr tmp;
	tmp.addr.ipv4addr.addr = ((sockaddr_in *)addr)->sin_addr.s_addr;
	tmp.addr.ipv4addr.port = ((sockaddr_in *)addr)->sin_port;
	
	if(net_connect(sd, &tmp) == MX_OK)
		return 0;
	else
		return -1;	
}

int setsockopt(int sd, int level, int optname, const void *optval,
                      socklen_t optlen)
{
	
	if( net_set_opt(sd, optname, optval, optlen) == MX_OK)
		return 0;
	else
		return -1;
}

int getsockopt(int sd, int level, int optname, void *optval,
                      socklen_t *optlen)
{
		
	if(net_get_opt(sd, optname, optval, optlen) != MX_OK)
		return -1;
	
	return 0;
}

int read(int sd, void *buf, int len)
{
	return recv(sd, buf, len, 0);
}

int recv(int sd, void *buf, int len, int flags)
{
	int ret;
	
	ret = net_recv(sd, buf, len, flags);
	
	if(ret == 0)
	{
		socket_errno = EAGAIN;
		return -1;		
	}
	
	if(ret < 0)
		return -1;
	
	return ret;
}

int recvfrom(int sd, void *buf, int len, int flags, sockaddr *from, 
                    socklen_t *fromlen)
{
	net_sockaddr tmp;
	int ret;
	
	ret = net_recv_from(sd, buf, len, &tmp, flags);
	
	if(ret == 0)
	{
		socket_errno = EAGAIN;
		return -1;		
	}
	
	if(ret < 0)
		return -1;
			
	((sockaddr_in *)from)->sin_addr.s_addr = tmp.addr.ipv4addr.addr;
	((sockaddr_in *)from)->sin_port = tmp.addr.ipv4addr.port;
	*fromlen = sizeof(sockaddr_in );
	
	return ret;
}

int write(int sd, const void *buf, int len)
{
	return send(sd, buf, len, 0);
}

int send(int sd, const void *buf, int len, int flags)
{
	int ret;
	ret = net_send(sd, buf, len, flags);

	if(ret != MX_OK && ret != RC_NETWORK_AGAIN)
		return -1;
	
	if(ret == RC_NETWORK_AGAIN)
	{
		socket_errno = EAGAIN;
		return -1;		
	}
	
	return len;
}

int sendto(int sd, const void *buf, int len, int flags, 
                  const sockaddr *to, socklen_t tolen)
{
	int ret;
	
	net_sockaddr tmp;
	tmp.addr.ipv4addr.addr = ((sockaddr_in *)to)->sin_addr.s_addr;
	tmp.addr.ipv4addr.port = ((sockaddr_in *)to)->sin_port;
	
	ret = net_sendto(sd, &tmp, buf, len, flags);
	
	if(ret != MX_OK && ret != RC_NETWORK_AGAIN)
	{
		net_uart_printf("sendto: some error: %x\r\n", ret);
		return -1;
	}
		
	if(ret == RC_NETWORK_AGAIN)
	{
		socket_errno = EAGAIN;
		net_uart_printf("sendto: again\r\n");
		return -1;		
	}
	
	net_uart_printf("sendto: send %i bytes\r\n", len);
	
	return len;
}

int accept (int sd, sockaddr *addr, socklen_t *addrlen)
{
//	#warning "no accrpt"
	return 0;
}

int listen (int sd, int backlog)
{
//	#warning "no listen"
	return 0;
}


int select (int nfds, fd_set *readsds, fd_set *writesds, fd_set *exceptsds,
            timeval *timeout)
{
	int i;
	int sock_num = 0;
	unsigned long int timeout_ms = 0;
	MXRC ret;
	net_socket socs[NET_MAX_CONN_NUMBER];
	
	for(i = 0; i < nfds; i++)
	{
		if(FD_ISSET(i, readsds))
		{
			socs[sock_num++] = i;
		}
	}
	
	if(timeout)
	{
		timeout_ms = timeout->tv_sec * 1000 + timeout->tv_usec/1000;
	}
	
	ret = net_select(socs, sock_num, (timeout_ms) ? &timeout_ms : NULL);
	
	if(timeout)
	{
		int sec = timeout_ms / 1000;
		int usec = (timeout_ms % 1000) * 1000;
		
		timeout->tv_sec -= sec;
		timeout->tv_usec -= usec;			
	}
	
	if(ret > 0)
	{
		FD_ZERO(readsds);
		FD_ZERO(writesds);
		FD_ZERO(exceptsds);
		
		//net_dump_data(socs, sock_num * sizeof(net_socket));
		
		for(i = 0; i < sock_num; i++)
			if(socs[i])
				FD_SET(socs[i], readsds);
				
	}
	
	return ret;
}


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "4gmx.h"
#include "appids.h"
#include "hal.h"
#include "appprintf.h"
#include "network.h"
#include "inet_aton.h"
#include "socket.h"
#include "mem.h"
#include "dhcp.h"

MXRC dhcp_request(net_conf_t *conf)
{
	int sockfd;
	sockaddr_in servaddr, cliaddr, rservaddr;
	dhcpmessage_t dhcpmsg;
	byte recvbuff[NET_ETHERNET_MAX_PAYLOAD_SIZE];
	UINT32 start_time;
	UINT32 cur_time;
	int rc;
	dhcp_opt_header_t *dhcp_opt = NULL;
	int recv_attempt = DHCP_RECV_ATTEMPT;
	socklen_t rservlen;
	dhcpmessage_t *recvdhcpmsg = (dhcpmessage_t *)recvbuff;


	if((sockfd=socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		net_uart_printf("dhcp_request: create socket error \r\n");
		return RC_DHCP_ERR;
	}

	net_set_flags(sockfd, NET_FLAG_NOBLOCK, TRUE);

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	cliaddr.sin_port = htons(DHCP_CLIENT_PORT);
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sockfd,(sockaddr*)&cliaddr,sizeof(cliaddr)) < 0)
	{
		net_uart_printf("dhcp_request: bind error \r\n");
		closesocket(sockfd);
		return RC_DHCP_ERR;
	}

	servaddr.sin_port = htons(DHCP_SERVER_PORT);
	servaddr.sin_family = AF_INET;
	inet_aton("255.255.255.255", (in_addr *)&servaddr.sin_addr.s_addr);

	memset(&dhcpmsg, 0, sizeof(dhcpmsg));

	dhcpmsg.op = 1;
	dhcpmsg.htype = 1;
	dhcpmsg.hlen = 6;
	dhcpmsg.hops = 0;
	dhcpmsg.xid = htonl(1000);
	dhcpmsg.secs = htons(0);
	dhcpmsg.flags = htons(0x8000);
	net_get_own_mac(dhcpmsg.chaddr);
	dhcpmsg.magic[0] = 99;
	dhcpmsg.magic[1] = 130;
	dhcpmsg.magic[2] = 83;
	dhcpmsg.magic[3] = 99;
	dhcpmsg.opt[0] = 53;
	dhcpmsg.opt[1] = 1;
	dhcpmsg.opt[2] = 1;
	dhcpmsg.end = 0xFF;

	if(sendto(sockfd,&dhcpmsg,sizeof(dhcpmsg),0,(sockaddr*)&servaddr,sizeof(servaddr)) < 0)
	{
		net_uart_printf("dhcp_request: sendto error \r\n");
		closesocket(sockfd);
		return RC_DHCP_ERR;
	}

	while(recv_attempt)
	{
		start_time = MxGetTime();
		cur_time = start_time;
		rservlen = sizeof(sockaddr_in);

		while(cur_time - start_time < DHCP_REPLAY_TIMEOUT)
		{
			socket_errno = 0;
			rc = recvfrom(sockfd, &recvbuff, sizeof(recvbuff), 0, (sockaddr*)&rservaddr, &rservlen);
			if(rc < 0)
			{
				if(socket_errno == EAGAIN)
				{
					MxDelay(10);
				}
				else
				{
					net_uart_printf("dhcp_request: recvfrom error: %i \r\n", socket_errno);
					closesocket(sockfd);
					return RC_DHCP_ERR;
				}
			}
			else
				break;
			cur_time = MxGetTime();
		}

		if(rc <= 0 && socket_errno != EAGAIN)
		{
			net_uart_printf("dhcp_request: recvfrom error: %i, attempt %i \r\n", socket_errno, recv_attempt);
			closesocket(sockfd);
			return RC_DHCP_ERR;
		}

		if(memcmp(conf->mac, recvdhcpmsg->chaddr, NET_MAC_SIZE) == 0)
			break;

		recv_attempt--;
	}

	if(recv_attempt == 0)
	{
		net_uart_printf("dhcp_request: recieve attepmt exeed!\r\n");
		closesocket(sockfd);
		return RC_DHCP_ERR;
	}

	conf->addr = (ipaddr_t)recvdhcpmsg->yiaddr;

	net_uart_printf("DHCP set ip %X\r\n", conf->addr);

	dhcp_opt = (dhcp_opt_header_t *)recvdhcpmsg->opt;

	while(dhcp_opt && dhcp_opt->opt_type != DHCP_OPT_END)
	{
		switch(dhcp_opt->opt_type)
		{
			case DHCP_OPT_MSG_TYPE:
				net_uart_printf("DHCP msg type is %d\r\n", dhcp_opt->ptr);
			break;
			case DHCP_OPT_NET_MASK:
				net_uart_printf("DHCP net mask is %x\r\n", *(ipaddr_t*)&dhcp_opt->ptr);
				conf->mask = (ipaddr_t)dhcp_opt->ptr;
			break;
			default:
				net_uart_printf("get dhcp option type %d, size %d\r\n", dhcp_opt->opt_type, dhcp_opt->opt_len);
				net_dump_data(&dhcp_opt->ptr, dhcp_opt->opt_len);
			break;
		}
		dhcp_opt = (dhcp_opt_header_t *)(&dhcp_opt->ptr + dhcp_opt->opt_len);
	}

	closesocket(sockfd);

	return MX_OK;
}

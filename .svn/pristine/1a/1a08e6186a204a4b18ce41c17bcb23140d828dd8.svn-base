#include "ethernet.h"
#include "hw.h"
#include "arp.h"
#include "utils.h"

byte net_zero_mac[NET_MAC_SIZE] = {0,0,0,0,0,0};
byte net_bcast_mac[NET_MAC_SIZE] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

MXRC net_ethernet_send(connection_t *conn, UINT16 pkt_type, byte *hbuff, size_t hsize, const byte *pbuff, const size_t psize)
{
	ethhdr_t *eth_header;

	NET_PROFILE(net_prof_eth_in);
	
	net_uart_printf("ethernet_sendex \r\n");
	
	if(!hbuff)
	{
		net_uart_printf("no buff \r\n");
		return RC_NETWORK_UNKNOWN_ERROR;
	}
	
	eth_header = (ethhdr_t *)hbuff;
	//net_uart_printf("%X: %x:%x:%x:%x:%x:%x\r\n",  eth_header, eth_header->dest_mac[0], eth_header->dest_mac[1], eth_header->dest_mac[2], eth_header->dest_mac[3], eth_header->dest_mac[4], eth_header->dest_mac[5]);
	
	net_uart_printf("ethernet first time \r\n");
	net_get_own_mac(eth_header->source_mac); 
	eth_header->pkt_type = htons(pkt_type);
	
	net_uart_printf("size to send %d \r\n", hsize + psize);
	
	if(memcmp(eth_header->dest_mac, net_zero_mac, NET_MAC_SIZE) == 0)
	{		
		net_uart_printf("no mac \r\n");
		while(1)
		{
		if(net_arp_get_mac(conn->daddr, eth_header->dest_mac) == MX_OK )
		{
			net_uart_printf("get mac from arp \r\n");
			_memcpy(conn->dmac, eth_header->dest_mac, NET_MAC_SIZE);
			break;
		}
		else
		{		
			net_uart_printf("arp cannot resolve\r\n");
			if(net_arp_queue_put(conn) == MX_OK)
			{				
				if(net_buff_put(conn->tx_queue, hbuff, hsize) != MX_OK)
				{					
					net_uart_printf("some problems to put in tx queue\r\n");
				}
				
				net_arp_request(conn->daddr);
				//if(conn->flags & NET_FLAG_NOBLOCK)
				//	return RC_NETWORK_WAIT_ARP;
				//else
				if(net_cond_wait_timeout(&conn->wait_arp, NET_ARP_WAIT_TO))
				{					
					net_buff_remove_last(conn->tx_queue);
					return RC_NETWORK_ARP_TO;	
				}

				net_buff_remove_last(conn->tx_queue);
				continue;
				
			}
			else
				return RC_NETWORK_ARP_ERROR;
		}
		}
	}
	//else
	//{
	//	net_uart_printf("have mac\r\n");
	//	_memcpy(eth_header->dest_mac, conn->dmac, NET_MAC_SIZE);		
	//}	
	
	net_hw_send(hbuff, hsize + ethhdr_size, pbuff, psize);

	NET_PROFILE(net_prof_eth_out);
	
	return MX_OK;
}


MXRC net_ethernet_send_to(byte *mac, UINT16 pkt_type, byte *hbuff, size_t hsize, const byte *pbuff, const size_t psize)
{
	ethhdr_t *eth_header;
	
	net_uart_printf("ethernet_send to\r\n");
	
	if(!hbuff)
	{
		net_uart_printf("no buff \r\n");
		return RC_NETWORK_UNKNOWN_ERROR;
	}
	
	eth_header = (ethhdr_t *)hbuff;
	net_get_own_mac(eth_header->source_mac); 
	eth_header->pkt_type = htons(pkt_type);
	_memcpy(eth_header->dest_mac, mac, NET_MAC_SIZE);
		
	net_hw_send(hbuff, hsize + ethhdr_size, pbuff, psize);
	
	return MX_OK;
}


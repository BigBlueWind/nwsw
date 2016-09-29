#include "config.h"
#include "hw.h"
#include "network.h"
#include "gemdrv.h"
#include <string.h>
#include <stdio.h>
#include "4gmx_types.h" 
#include "appprintf.h"
#include "ncnbdata.h"
#include "ethernet.h"
#include "arp.h"
#include "ip.h"
#include "utils.h"
#include "heaps.h"

void NetworkCb (LPVOID pCBCtx, LPVOID pGemDrvCtx, UINT32 nIrqStatus);

MXRC net_hw_init(void)
{	
	GemCfg    cfg;
	GemDMACfg dmacfg;
	LPVOID    pGemDrv;
	MXRC rc = MX_OK;
	int i	=	10;
	PUINT8 tx_addr;

	GemDrvShutdown (1);

	rc = GemDrvInit (GEMAC1_BASEADDR);

	if (FAILED(rc))
	{
		net_uart_printf ("GEM init failed, rc=%x\r\n", rc);
	    return RC_NETWORK_GEM_INIT_ERR;
	}

	net_uart_printf ("GEM init OK\r\n");

	pGemDrv = GemDrvGetCtx (1);

	_ASSERT_PTR (pGemDrv);

	cfg.Phy     = RGMII;
	cfg.Speed   = MAC_SPEED_1000Mbps;

	// 00-11-22-33-44-55 - temporary MAC address

	net_get_own_mac(cfg.MacAddr);

	if (FAILED(rc = GemDrvCfg(pGemDrv, &cfg)))
	{
		uart_printf ("GEM config failed, rc=%x\r\n", rc);
	    return RC_NETWORK_GEM_INIT_ERR;
	}

	net_uart_printf ("GEM config OK\r\n");

	// GEM DMA configuration

	dmacfg.RxDescrCount = ETH_NUM_RX_BUFS;
	dmacfg.TxDescrCount = ETH_NUM_TX_BUFS;

	dmacfg.RxDescr   = (HGRxFrm)gem_rx_frm;
	tx_addr = (PUINT8)MxHeapAlloc(IramHeap, sizeof(HGTxFrm) * ETH_NUM_TX_BUFS + 128);
	if(!tx_addr)
		return RC_NETWORK_GEM_INIT_ERR;
	
	tx_addr -= ((UINT32)tx_addr%128);
	dmacfg.TxDescr = (HGTxFrm)tx_addr;	
	dmacfg.RxBuffers = gem_rx_buffers;

	dmacfg.DMAConfig = GEM_DMA_DEF_RX_SIZE(2048) | GEM_DMA_BURST_INCR4 /*| GEM_DMA_CHK_SUM_ENB*/;
	dmacfg.RxBufOpt    = GEM_DMA_RX_IRQ_ENB;

    if (FAILED(rc = GemDrvConfigDMA(pGemDrv, &dmacfg)))
	{
		uart_printf ("GEM config DMA failed, rc=%x\r\n", rc);
	    return RC_NETWORK_GEM_INIT_ERR;
	}

	GemDrvDisableRX(pGemDrv);
	GemDrvEnableTX(pGemDrv);

	net_uart_printf ("GEM config DMA OK\r\n");

	if (FAILED(rc = GemDrvCopyAll (pGemDrv, 1)))
	{
	    uart_printf ("GEM set copyall failed, rc=%x\r\n", rc);
	    return RC_NETWORK_GEM_INIT_ERR;
	}

	net_uart_printf ("GEM set copyall OK\r\n");

	if (FAILED(rc = GemDrvEnBroadcast (pGemDrv, 1)))
	{
	    uart_printf ("GEM set broadcast failed, rc=%x\r\n", rc);
	    return RC_NETWORK_GEM_INIT_ERR;
	}

	net_uart_printf ("GEM set broadcast OK\r\n");  

	GemDrvSetIrqProc (pGemDrv, NetworkCb, NULL, GEM_INT_RX_COMPLETE|GEM_INT_TX_COMPLETE);
	GemDrvEnableRX(pGemDrv);

	while (i)
	{
		if(GemDrvIsLinkUp(pGemDrv))
			return MX_OK;
				
		MxDelayTicks(150*1000*1000);

		i --;
	}

	uart_printf ("Link is down\r\n");
	
	return RC_NETWORK_GEM_INIT_ERR;		
}


void NetworkCb (LPVOID pCBCtx, LPVOID pGemDrvCtx, UINT32 nIrqStatus)
{
    HGRxFrm rx_frm;
    UINT32 BufNum = 0;
    UINT32 FrmSize = 0;
    PUINT8 pRxBuf = 0;
    ethhdr_t *eth_header;

    net_uart_printf ("GEM IRQ\r\n");

    while ((GemDrvRxFrm (pGemDrvCtx, &rx_frm, &BufNum, &FrmSize)) == MX_OK)
    {
        pRxBuf = GemDrvGetRxFrmBuf (pGemDrvCtx, rx_frm, 0);

        eth_header = (ethhdr_t *)pRxBuf;
        net_uart_printf("SMAC = %x \r\n dMAC = %x \r\n ethtype = %x \r\n", eth_header->source_mac, eth_header->dest_mac, eth_header->pkt_type);

        switch(htons(eth_header->pkt_type))
        {
        	case NET_ETHERNET_PKT_ARP:
        		{
        			net_uart_printf ("Get arp packet\r\n");
        			net_arp_parce_pkt(pRxBuf + ethhdr_size);
        		}
        	break;
        	case NET_ETHERNET_PKT_IP:
        	    {
        	   		net_uart_printf ("Get ip packet\r\n");
        	   		net_ip_parce_pkt(pRxBuf + ethhdr_size);        	        			
        	   	}
        	break;
        	default:
        		break;
        }
        
        GemDrvFreeRxFrm (pGemDrvCtx, rx_frm);                
        BufNum = 0;
        FrmSize= 0;
    }

    net_uart_printf ("GEM IRQ IS COMPLETED\r\n");
}

MXRC net_hw_send(byte *hframe, size_t hsize, const byte *pframe, const size_t psize)
{
	 LPVOID pGemDrv;
	 MXRC   rc     = MX_OK;

	 NET_PROFILE(net_prof_hw_in);

	 net_uart_printf("hw_sendex \r\n");
	 
	 pGemDrv = GemDrvGetCtx (1);

	 if (pGemDrv == NULL)
	 {
	     net_uart_printf ("GEM is not inited\r\n");
	     return 0;
	 }
	
     net_uart_printf ("Send packet (size=%d) ... ", hsize + psize);
     net_dump_data(pframe, psize);
	 
     rc = GemDrvTxFrm(pGemDrv, GEM_DMA_TX_CRC | GEM_DMA_TX_L3_L4_ADDED, hframe, hsize, (LPVOID)pframe, psize);

     if (FAILED(rc))
    	 net_uart_printf("error, rc=%x\r\n", rc);
	 else
	     net_uart_printf("OK\r\n");

	 NET_PROFILE(net_prof_hw_out);
	 
     return MX_OK; 
}


/** @file packetif.c
 *
 * @brief Network interface using pcap library
 * @author Mindspeed Technologies
 * @version $Revision: 1.26 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "osal.h"
#include "packetif.h"
#include "pcap.h"
#include "tinyarp.h"
#include "settings.h"
#ifdef BSMAC
#include "mac_constants.h"
#endif

#ifndef DBG_EVENT
#define DBG_EVENT
#endif

#define MAX_CONCURRENT_CONNECTIONS 600

static pcap_if_t Eth_Adapters[10];
static pcap_t *Eth_Interfaces[10];
static U8 UsedEthAdapt = 0;
static OSAL_SEMAPHORE SEMA_ETH_RX;
static OSAL_TASK NET_ETH_RX_task;
OSAL_MEMARRAY PacketPartition;
static MacCB NetToMac;
static U32 PacketPartFreeCnt = MAX_CONCURRENT_CONNECTIONS*7;
static OSAL_HEAP* NetHeap = NULL;
static pcap_if_t *AllDevs = NULL;

RESULTCODE NetAllocPacket(PACKET_DESC ** ppPacket)
{
    OSAL_MEMARRAY *pArray  = &PacketPartition;
    PACKET_DESC *pPacket;
    RESULTCODE retVal = FAILURE;

    OsalAllocMemArray(pArray, (OSAL_PADDRESS*)&pPacket);
    if(pPacket)
    {
        pPacket->Payload = (U8 *) ((U8 *) pPacket + sizeof(PACKET_DESC));
        pPacket->Offset = 0;
        pPacket->Counter = 0;
        pPacket->Length = 0;
        pPacket->Cid = 0xFFFF;
        pPacket->pNext = NULL;
        pPacket->pRefData = NULL;
        pPacket->pArq = NULL;

        pPacket->pPart = pArray;
        if(PacketPartFreeCnt == 0)
            DBG_EVENT;
        --PacketPartFreeCnt;
        retVal = SUCCESS;
    }
    else
    {
        printf("NetAllocPacket: zero packet desc\n");
    }

    *ppPacket = pPacket;

    return retVal;
}

RESULTCODE NetFreePacket(PACKET_DESC * pPacket)
{
    if(pPacket && (pPacket->Counter == 0))
    {
        PacketPartFreeCnt++;
        return OsalFreeMemArray(pPacket->pPart, pPacket);
    }
    return FAILURE;
}

static inline void NetWriteU32Swap(U8* pDest, U32* pDataU32, U16 NumU32)
{
    while(NumU32--)
    {
        U32 Value = *pDataU32++;
        *pDest++ = (U8)(Value >> 24);
        *pDest++ = (U8)(Value >> 16);
        *pDest++ = (U8)(Value >> 8);
        *pDest++ = (U8)Value;
    }
}
#if 0
static unsigned NetIpCrc(u_char * ptr, int len)
{
    u_long xsum;
    u_short *p = (u_short *) ptr;

    xsum = 0;
    while (len-- > 0)
        xsum += *p++;
    xsum = (xsum & 0xffff) + (xsum >> 16);
    xsum = (xsum & 0xffff) + (xsum >> 16);
    return (xsum & 0xffff);
}
#endif
#if 0
static int NetIpCrcOk(u_char * ptr, int len)
{
    return !((NetIpCrc(ptr, len) + 1) & 0xfffe);
}

static void NetWriteEthHdr(u_char* peth, u_char* dst, u_char* src, u_short type)
{
    memcpy(peth, dst, ETHER_ADDR_LEN);
    peth += ETHER_ADDR_LEN;
    memcpy(peth, src, ETHER_ADDR_LEN);
    peth += ETHER_ADDR_LEN;
    *(u_short*)peth = type;
}

static void NetWriteIpUdp(u_char * xip, ip_address dst, ip_address src, int dport, int sport,
              int len)
{
    static U32 NetIPID;
    ip_udp_hdr *ip = (ip_udp_hdr *) xip;

    /*
     *  If the data is an odd number of bytes, zero the
     *  byte after the last byte so that the checksum
     *  will work.
     */
    if (len & 1)
        xip[IPV4_UDP_HDR_SIZE + len] = 0;

    /*
     *  Construct an IP and UDP header.
     *  (need to set no fragment bit - XXX)
     */
    ip->ver_ihl = 0x45; /* IP_HDR_SIZE / 4 (not including UDP) */
    ip->tos = 0;
    ip->tlen = htons((U16)(IPV4_UDP_HDR_SIZE + len));
    ip->id = htons((U16)(NetIPID++));
    ip->flags_fo = htons(0x4000); /* No fragmentation */
    ip->ttl = 255;
    ip->proto = 17; /* UDP */
    ip->crc = 0;
    memcpy(&ip->saddr, &src, sizeof(ip_address));/* already in network byte order */
    memcpy(&ip->daddr, &dst, sizeof(ip_address));/* - "" - */
    ip->udp_sport = htons((U16)sport);
    ip->udp_dport = htons((U16)dport);
    ip->udp_len = htons((U16)(UDP_HDR_SIZE + len));
    ip->udp_xsum = 0;
    ip->crc = ~ NetIpCrc((u_char *) ip, IPV4_HDR_SIZE / 2);
}
#endif

static RESULTCODE NetEthGetPacket(PACKET_DESC * pPacket)
{
    struct pcap_pkthdr *header;
    const u_char *pkt_data;
    int res;

    /* Retrieve the packets */
    if ((res = pcap_next_ex(Eth_Interfaces[UsedEthAdapt], &header, &pkt_data))
        > 0)
    {
        struct tm *ltime;
        char timestr[16];
        time_t local_tv_sec;
        ip_header *ih;
        udp_header *uh;
        u_int ip_len;
        u_short sport, dport;

        /* convert the timestamp to readable format */
        local_tv_sec = header->ts.tv_sec;
        ltime = localtime(&local_tv_sec);
        strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

        /* retireve the position of the ip header */
        ih = (ip_header *) (pkt_data + 14); //length of ethernet header

        /* retireve the position of the udp header */
        ip_len = (ih->ver_ihl & 0xf) * 4;
        uh = (udp_header *) ((u_char *) ih + ip_len);

        /* convert from network byte order to host byte order */
        sport = ntohs(uh->sport);
        dport = ntohs(uh->dport);

        pPacket->Length = (U16)header->len;
        pPacket->Payload = (U8 *)pkt_data;

        return SUCCESS;
    }
    return 1;
}

static void NetEthTask(void)
{
    PACKET_DESC PDesc; // used only to transfer values
    PACKET_DESC *pPacket;

    while (TRUE)
    {
        if (OsalWaitForSema(&SEMA_ETH_RX, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            while (NetEthGetPacket(&PDesc) == SUCCESS)
            {
                NetAllocPacket(&pPacket);
                if(pPacket)
                {
                    memcpy(pPacket->Payload, PDesc.Payload, PDesc.Length);
                    pPacket->Length = PDesc.Length;
                    if(pPacket->Length >= 1536)
                    {
                        printf("NetEthTask: wrong packet length %d\n", pPacket->Length);
                        NetFreePacket(pPacket);
                    }
                    else 
                     {
#ifdef ENODEB
                       U32 res ;
                        res = NetToMac(pPacket,pPacket->Length);
                        if( res == EXPIRED || res == SUCCESS)
                            break;
#else
                    if (NetToMac(pPacket, 0) != SUCCESS)
                        NetFreePacket(pPacket);
#endif
                     }

                }
                else
                {
                    printf("NetEthTask: can't alloc packet desc\n");
                    DBG_EVENT;
                }
            }
        }
    }
}

RESULTCODE NetEthInit(BOOLEAN bOpen, OSAL_HEAP *pHeap, MacCB cb)
{
    OSAL_PADDRESS pQueueMemory;
    pcap_if_t *d;
    int i = 0;
    u_int netmask;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fcode;
#ifdef BSMAC
    char packetFilter[] = "ether dst ff:ff:ff:ff:ff:ff or ether dst ac:ac:ac:ac:ac:ac or ether dst bc:bc:bc:bc:bc:bc or ether dst dc:dc:dc:dc:dc:dc or ether dst cc:cc:cc:cc:cc:cc";    
#else
    char packetFilter[256];
    char *packetFilterTemplate = "udp port %s";
    int capturePort = 1234;
    char udpport[5];
#endif

    printf("NetEthInit: entry point\n");

#ifndef BSMAC
    ReadSettingInt("udpport", &capturePort, capturePort);
    sprintf(packetFilter, packetFilterTemplate, _itoa(capturePort,udpport,10));
#endif
    if(NetHeap == NULL)
    {
        if(pHeap == NULL)
            return FAILURE;

        /* Retrieve the device list from the local machine */
        if (pcap_findalldevs(&AllDevs, errbuf) == -1)
        {
            fprintf(stderr, "PCAP: Device list error: %s\n", errbuf);
            return 1;
        }

        /* Print the list */
        for (d = AllDevs; d != NULL; d = d->next)
        {
            memcpy(&Eth_Adapters[i], d, sizeof(*d));
            printf("%d. %s", ++i, d->name);
            if (d->description)
                printf(" (%s)\n", d->description);
            else
                printf(" (No description available)\n");
        }

        if (i == 0)
        {
            printf("PCAP: No interfaces found.\n");
            return 2;
        }
        // set incoming packets handler
        NetToMac = cb;
            // TODO: is it enough - per SF?
        OsalAllocHeap(pHeap, &pQueueMemory,
            (OSAL_SIZE) ((sizeof(PACKET_DESC) + ETH_MAXFRAMESIZE) * MAX_CONCURRENT_CONNECTIONS * 7));

        OsalCreateMemArray(&PacketPartition, pQueueMemory,
            (OSAL_SIZE) ((sizeof(PACKET_DESC) + ETH_MAXFRAMESIZE) * MAX_CONCURRENT_CONNECTIONS * 7),
            (OSAL_SIZE) (sizeof(PACKET_DESC) + ETH_MAXFRAMESIZE));

        OsalCreateSema(&SEMA_ETH_RX);
        if (OsalCreateTask(&NET_ETH_RX_task, (OSAL_PADDRESS) &NetEthTask,
                 &NET_ETH_RX_task, NULL, OSAL_PRIORITY_NORMAL,
                 NULL, 0) != SUCCESS)
            return 3;

        // Open USED_ETH_ADAPTER
        if (bOpen)
        {
            pcap_t *fp;

            // Find "Microsoft'" in the adapter description
            {
                char ref[11] = "Microsoft's";

                for (d = AllDevs, i = 0; d != NULL; d = d->next, i++)
                {
                    int length = 0;
                    if (d->description)
                    {
                        char *uut = d->description;
                        do
                        {
                            if (uut[length++] == '(')
                            {
                                if (memcmp(ref, &uut[length], sizeof(ref)) == 0)
                                    break;
                                length += sizeof(ref);
                            }
                        } while (uut[length] != 0);

                        if (uut[length] != 0)
                            break;
                    }
                }
                if (d)
                {
                    UsedEthAdapt = i;
                }
            }

            /* Jump to the selected adapter */
            //      for(d=AllDevs, i=0; i< UsedEthAdapt ;d=d->next, i++);
            printf("Opened Ethernet Adapter %d.\n", UsedEthAdapt+1);
#ifndef PCAP_OPENFLAG_PROMISCUOUS
#define PCAP_OPENFLAG_PROMISCUOUS		1
#endif
            if ((fp = pcap_open_live(Eth_Adapters[UsedEthAdapt].name, // name of the device
                                     65536, // portion of the packet to capture. It doesn't matter in this case
                                     PCAP_OPENFLAG_PROMISCUOUS, // promiscuous mode (nonzero means promiscuous)
                                     1000, // read timeout
                                     errbuf // error buffer
                )) == NULL)
            {
                fprintf(stderr, "PCAP: Unable to open adapter '%s'\n",
                        Eth_Adapters[UsedEthAdapt].name);
                /* Free the device list */
                pcap_freealldevs(AllDevs);
                return 4;
            }
            Eth_Interfaces[UsedEthAdapt] = fp;

            /* Check the link layer. We support only Ethernet for simplicity. */
            if (pcap_datalink(fp) != DLT_EN10MB)
            {
                fprintf(stderr, "PCAP: Invalid network type.\n");
                /* Free the device list */
                pcap_freealldevs(AllDevs);
                return 5;
            }

            netmask = 0xffffffff;

            //compile the filter
            if (pcap_compile(fp, &fcode, packetFilter, 1, netmask) < 0)
            {
                fprintf(stderr, "PCAP: Unable to compile the packet filter. Check the syntax.\n");
                /* Free the device list */
                pcap_freealldevs(AllDevs);
                return 6;
            }

            //set the filter
            if (pcap_setfilter(fp, &fcode) < 0)
            {
                fprintf(stderr, "PCAP: Error setting the filter.\n");
                /* Free the device list */
                pcap_freealldevs(AllDevs);
                return 7;
            }

            /* start the capture */
            //      pcap_loop(fp, 0, NetEthRxPacket_cb, NULL);
        }

        arp_init();

        OsalActivateTask(&NET_ETH_RX_task);

        NetHeap = pHeap;
    }
    return SUCCESS;
}

RESULTCODE NetEthDone(void)
{
    if(NetHeap)
    {
        if(OsalDeactivateTask(&NET_ETH_RX_task) != SUCCESS)
            return FAILURE;

        pcap_close(Eth_Interfaces[UsedEthAdapt]);
        Eth_Interfaces[UsedEthAdapt] = NULL;
        pcap_freealldevs(AllDevs);
        AllDevs = NULL;

        if(OsalDestroyTask(&NET_ETH_RX_task) != SUCCESS)
            return FAILURE;

        if(OsalDestroySema(&SEMA_ETH_RX) != SUCCESS)
            return FAILURE;

        if(OsalDestroyMemArray(&PacketPartition) != SUCCESS)
            return FAILURE;
        if(PacketPartition.storage)
        {
            if(OsalFreeHeap(NetHeap, PacketPartition.storage) != SUCCESS)
                return FAILURE;
            PacketPartition.storage = NULL;
        }
        NetToMac = NULL;
        NetHeap = NULL;
    }
    return SUCCESS;
}

//int NetEthSendPacket(int argc, char **argv)
RESULTCODE NetEthSendPacket( PACKET_DESC * pPacket)
{
    pcap_t * fp = Eth_Interfaces[UsedEthAdapt];
    /* Send down the packet */
    if (fp)
    {
        if (pcap_sendpacket(fp,                                 // Adapter
                            pPacket->Payload + pPacket->Offset, // buffer with the packet
                            pPacket->Length                     // size
        ) != 0)
        {
            fprintf(stderr, "PCAP: Error %s sending the packet: Len_%d\n",
                    pcap_geterr(fp), (U32)pPacket->Length);
            return 3;
        }
        return SUCCESS;
    }
    return FAILURE;
}

void NetRaiseSema(void)
{
    OsalRaiseSema(&SEMA_ETH_RX);
}

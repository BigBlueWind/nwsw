/** @file tinyarp.c
 *
 * @brief The simpliest ARP protocol implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

//#include <netinet/in.h>
#include "osal.h"
#include "tinyarp.h"

#if defined(WIN32)
static ethaddr_t bcastaddr = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
#elif defined(USE_LINUX)
static ethaddr_t bcastaddr = {
    .addr = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
}; 
#endif

arptab_t arptab[ARP_NENTRIES];
U8 bIsArpTabActive = 0;

void arp_init(void)
{
    memset(&arptab[0], 0, sizeof(arptab));
}

/*
 * Broadcast an ARP packet (i.e. ask who has address "addr")
 */
ushort arp_whohas_prepare(IP_ADDR_T addr, char *msg, char *eth_my,
                          IP_ADDR_T ip_my)
{
    arphdr_t *ap;
    ethhdr_t *ether;
    ushort length = 0;

    // write Ethernet header
    ether = (ethhdr_t *) msg;
    memcpy(ether->eth_dst.addr, &bcastaddr, ETH_ADDRSIZE);
    memcpy(ether->eth_src.addr, eth_my, ETH_ADDRSIZE);

    ether->eth_proto = ntohs(ETHTYPE_ARP);
    msg += ETH_HDR_SIZE;
    length += ETH_HDR_SIZE;

    // write ARP header
    ap = (arphdr_t *) msg;
    ap->arp_hrd = ntohs(ARPHRD_ETHER);
    ap->arp_pro = ntohs(ETHTYPE_IP);
    ap->arp_hln = ETH_ADDRSIZE;
    ap->arp_pln = sizeof(IP_ADDR_T);
    ap->arp_op = ntohs(ARPOP_REQUEST);
    memcpy(ap->arp_sha, eth_my, ETH_ADDRSIZE);
    memcpy(ap->arp_spa, &ip_my, sizeof(ap->arp_spa));
    memset(ap->arp_tha, 0, sizeof(ap->arp_tha));
    memcpy(ap->arp_tpa, &addr, sizeof(ap->arp_spa));
    msg += ARP_HDR_SIZE;
    length += ARP_HDR_SIZE;

    //write trailer to reach ETH_MIN_PACK size
    memset(msg, 0, (ETH_MIN_PACK - length));
    msg += ETH_MIN_PACK - length;
    length += ETH_MIN_PACK - length;

    return length;
}

uchar *arp_check(IP_ADDR_T addr)
{
    int i;

    for (i = 0; i < ARP_NENTRIES; i++)
    {
        if (arptab[i].at_ipaddr == addr && arptab[i].at_reftime == 0)
        {
            return arptab[i].at_eaddr;
        }
    }
    return NULL;
}

char arp_we_may_send_request(IP_ADDR_T addr)
{
    int i;
    OSAL_DELAYM CurTime;

    OsalGetTimeMs(&CurTime);
    if(CurTime == 0)
        CurTime++;

    for (i = 0; i < ARP_NENTRIES; i++)
    {
        if (arptab[i].at_ipaddr == addr)
        {
            if(arptab[i].at_reftime != 0)
            {
                if((CurTime - arptab[i].at_reftime) <= ARPREQ_TIMEOUT)
                {// still wait for answer for previous request
                    return 0;
                }
                else
                {// clear slot, time to send our request again
                    arptab[i].at_ipaddr = 0;
                    bIsArpTabActive--;
                }
            }
            else
            {// it may be used right now, arp_check should be called before
                return 0;
            }

        }

        if (arptab[i].at_ipaddr == 0)
        { // found free table slot
            arptab[i].at_ipaddr = addr;
            arptab[i].at_reftime = CurTime;
            bIsArpTabActive++;
            return 1;
        }
    }
    return 0;
}

ushort arp_check_header(char *data, ushort len, ushort proto)
{
    arphdr_t *ap;
    ushort op;

    //  printf("\t****arp checks process ");

    if (len < sizeof(arphdr_t))
        return 3;
    //  printf("1 ");
    if (proto != ETHTYPE_ARP && proto != ETHTYPE_RARP)
        return 4;
    //  printf("2 ");
    ap = (arphdr_t *) data;
    if (ntohs(ap->arp_hrd) != ARPHRD_ETHER)
        return 5;
    //  printf("3 ");
    if (ntohs(ap->arp_pro) != ETHTYPE_IP)
        return 6;
    //  printf("4 ");
    if (ap->arp_hln != ETH_ADDRSIZE)
        return 7;
    //  printf("5 ");
    if (ap->arp_pln != sizeof(IP_ADDR_T))
        return 8;
    //  printf("6 ");

    op = ntohs(ap->arp_op);
    if (op > ARPOP_REPLY || op == 0)
        return 9;

    return op;
}

ushort arp_process_packet(char *data, char *eth_my)
{
    arphdr_t *ap;
    IP_ADDR_T senderIP;
    ushort op;
    int i;

    ap = (arphdr_t *) data;
    op = ntohs(ap->arp_op);

    if (op == ARPOP_REPLY && bIsArpTabActive)
    {
        printf("REPLY !!!");
        if (memcmp(ap->arp_tha, eth_my, ETH_ADDRSIZE) != 0)
            return 0;

        printf("\n****Event: Got ARP respond\n");

        memcpy(&senderIP, ap->arp_spa, sizeof(IP_ADDR_T));

        for (i = 0; i < ARP_NENTRIES; i++)
        {
            if (arptab[i].at_ipaddr == senderIP)
            {
                memcpy(arptab[i].at_eaddr, ap->arp_sha, ETH_ADDRSIZE);
                arptab[i].at_reftime = 0; // mark as allowed for usage
                printf(
                       "\tARPTable Idx_%d - Write EthAddr %X %X %X %X %X %X for IP %d.%d.%d.%d\n",
                       i, ap->arp_sha[0], ap->arp_sha[1], ap->arp_sha[2],
                       ap->arp_sha[3], ap->arp_sha[4], ap->arp_sha[5],
                       (U8) senderIP, (U8)(senderIP >> 8),
                       (U8)(senderIP >> 16), (U8)(senderIP >> 24));
                bIsArpTabActive--;
            }
        }
    }

    return op;
}

uchar arp_check_tpa(char *msg, IP_ADDR_T my_ipaddr)
{
    arphdr_t *ap;
    ap = (arphdr_t *) msg;

    if (*(IP_ADDR_T *) ap->arp_tpa == my_ipaddr)
        return 1;
    return 0;
}

ushort arp_prepare_reply(char *src, char *dst, IP_ADDR_T my_ipaddr,
                         char *eth_my)
{
    arphdr_t *apDst;
    arphdr_t *apSrc;
    ethhdr_t *etherDst;
    ethhdr_t *etherSrc;
    ushort length = 0;

    etherSrc = (ethhdr_t *) src;
    apSrc = (arphdr_t *) (src + ETH_HDR_SIZE);

    // write Ethernet header
    etherDst = (ethhdr_t *) dst;
    memcpy(etherDst->eth_dst.addr, etherSrc->eth_src.addr, ETH_ADDRSIZE);
    memcpy(etherDst->eth_src.addr, eth_my, ETH_ADDRSIZE);
    etherDst->eth_proto = ntohs(ETHTYPE_ARP);
    dst += ETH_HDR_SIZE;
    length += ETH_HDR_SIZE;

    // write ARP header
    apDst = (arphdr_t *) dst;
    apDst->arp_hrd = ntohs(ARPHRD_ETHER);
    apDst->arp_pro = ntohs(ETHTYPE_IP);
    apDst->arp_hln = ETH_ADDRSIZE;
    apDst->arp_pln = sizeof(IP_ADDR_T);
    apDst->arp_op = htons(ARPOP_REPLY);
    memcpy(apDst->arp_sha, eth_my, ETH_ADDRSIZE);
    memcpy(apDst->arp_spa, &my_ipaddr, sizeof(IP_ADDR_T));
    memcpy(apDst->arp_tha, apSrc->arp_sha, ETH_ADDRSIZE);
    memcpy(apDst->arp_tpa, apSrc->arp_spa, sizeof(IP_ADDR_T));
    dst += ARP_HDR_SIZE;
    length += ARP_HDR_SIZE;

    //write trailer to reach ETH_MIN_PACK size
    memset(dst, 0, (ETH_MIN_PACK - length));
    dst += ETH_MIN_PACK - length;
    length += ETH_MIN_PACK - length;

    return length;
}

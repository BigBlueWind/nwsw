/** @file arp.h
 *
 * @brief Ethernet Address Resolution Protocol (see RFC 826)
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _TINYARP_H_
#define _TINYARP_H_

#include "basetypes.h"
#include "osal.h"
#include "net.h"

#define ARP_HDR_SIZE 28
#define ARP_NRETRIES 3  /* # of ARP & RARP retries */
#define ARP_NENTRIES 20  /* # of ARP table entries */

#define ARPHRD_ETHER 1  /* ethernet hardware addr */

#define ARPOP_REQUEST 1  /* request to resolve addr */
#define ARPOP_REPLY 2   /* response to prev request */
#define REVARP_REQUEST 3  /* reverse ARP request */
#define REVARP_REPLY 4  /* reverse ARP reply */

#define ARPREQ_TIMEOUT 1000 /* ms */
/*
 * ARP packets are variable in size; the arphdr_t type defines the
 * 10Mb Ethernet variant.  Field names used correspond to RFC 826.
 */
typedef struct
{
    ushort arp_hrd;     /* format of hardware addr */
    ushort arp_pro;     /* format of proto. addr */
    uchar arp_hln;     /* length of hardware addr */
    uchar arp_pln;     /* length of protocol addr */
    ushort arp_op;
    uchar arp_sha[ETH_ADDRSIZE]; /* sender hardware addr */
    uchar arp_spa[4];    /* sender protocol addr */
    uchar arp_tha[ETH_ADDRSIZE]; /* target hardware addr */
    uchar arp_tpa[4];    /* target protocol addr */
} arphdr_t;

/*
 * Internet to hardware address resolution table
 */
typedef struct
{
    IP_ADDR_T at_ipaddr;   /* internet address */
    uchar at_eaddr[ETH_ADDRSIZE]; /* ethernet address */
    ulong at_reftime;    /* time when referenced */
} arptab_t;

ushort arp_whohas_prepare(IP_ADDR_T addr, char *msg, char *eth_my,
                          IP_ADDR_T ip_my);
uchar *arp_check(IP_ADDR_T addr);
char arp_we_may_send_request(IP_ADDR_T addr);
ushort arp_check_header(char *data, ushort len, ushort proto);
ushort arp_process_packet(char *data, char *eth_my);
uchar arp_check_tpa(char *msg, IP_ADDR_T my_ipaddr);
ushort arp_prepare_reply(char *src, char *dst, IP_ADDR_T my_ipaddr,
                         char *eth_my);
void arp_init(void);

#endif /* _TINYARP_H_ */

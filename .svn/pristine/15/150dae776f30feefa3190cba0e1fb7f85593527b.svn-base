/** @file net.h
 *
 * @brief Structures and constants for IP networking
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef __NET_H__
#define __NET_H__


typedef unsigned char uchar;
typedef unsigned long ulong;
typedef unsigned short ushort;

#define   ETH_HDR_SIZE 14  /* Ethernet header size     */
#define E802_HDR_SIZE 22  /* 802 ethernet header size */
#define E8022_LLC_HDR_SIZE 3 /* 802.2 LLC header size: DSAP byte, SSAP byte, Control byte    */
#define E8022_SNAP_HDR_SIZE  5 /* 802.2 SNAP header size: OUI 3bytes, Type 2bytes */
#define VLAN_TAG_SIZE 2

#define   ETH_MIN_PACK 60
#define ETH_MINTU (ETH_MIN_PACK-ETH_HDR_SIZE) /* minimal tranmission unit */
#define ETH_MAXTU 1500  /* maximum transmission unit */
#define ETH_ADDRSIZE 6  /* size of ethernet addresses */

#define ETHTYPE_LOWEST 0x0600 /* = 1536, values below are lengthes not types */
#define ETHTYPE_IP 0x0800  /* IP protocol */
#define ETHTYPE_ARP 0x0806  /* ARP protocol */
#define ETHTYPE_RARP 0x8035 /* Reverse ARP protocol */
#define ETHTYPE_VLAN 0x8100 /* VLAN - 802.1D 802.1Q */

/*
 * Ethernet address
 */
typedef struct
{
    uchar addr[ETH_ADDRSIZE];  /* 48 bit address */
} ethaddr_t;

/*
 * Ethernet packet header
 */
typedef struct
{
    ethaddr_t eth_dst;    /* destination address */
    ethaddr_t eth_src;    /* source address */
    ushort eth_proto;    /* protocol type */
} ethhdr_t;
/*
 * Ethernet header
 */
typedef struct
{
    uchar et_dest[6];    /* Destination node     */
    uchar et_src[6];    /* Source node          */
    ushort et_protlen;    /* Protocol or length       */
    uchar et_dsap;     /* 802 DSAP         */
    uchar et_ssap;     /* 802 SSAP         */
    uchar et_ctl;     /* 802 control          */
    uchar et_snap1;     /* SNAP             */
    uchar et_snap2;
    uchar et_snap3;
    ushort et_prot;     /* 802 protocol         */
} Ethernet_t;

typedef struct
{
    ushort et_protlen;    /* Protocol or length       */
    uchar et_dsap;     /* 802 DSAP         */
    uchar et_ssap;     /* 802 SSAP         */
    uchar et_ctl;     /* 802 control          */
    uchar et_snap1;     /* SNAP             */
    uchar et_snap2;
    uchar et_snap3;
    ushort et_prot;     /* 802 protocol         */
} EthernetNoMACs_t;

typedef ulong IP_ADDR_T;   /* IP address type */

#define IP_PROTO_UDP 17  /* UDP protocol number */
#define IP_PROTO_ICMP 1
#define IP_PROTO_TCP 6
/*
 * Structure of an internet header (without options)
 */
typedef struct
{
    uchar ip_vhl;     /* version and header length */
#define IP_VERSION 4    /* current version number */
    uchar ip_tos;     /* type of service */
    short ip_len;     /* total length */
    ushort ip_id;     /* identification */
    short ip_off;     /* fragment offset field */
#define IP_DF  0x4000   /* don't fragment flag */
#define IP_MF  0x2000   /* more fragments flag */
#define IP_OFFMASK 0x1FFF   /* fragment offset mask */
    uchar ip_ttl;     /* time to live */
#define IP_FRAGTTL 60    /* time to live for frags */
    uchar ip_p;      /* protocol */
    ushort ip_sum;     /* checksum */
    IP_ADDR_T ip_src;    /* source address */
    IP_ADDR_T ip_dst;    /* destination address */
} iphdr_t;

/*
 * Internet Protocol (IP) header.
 */
typedef struct
{
    uchar ip_hl_v;     /* header length and version    */
    uchar ip_tos;     /* type of service      */
    ushort ip_len;     /* total length         */
    ushort ip_id;     /* identification       */
    ushort ip_off;     /* fragment offset field    */
    uchar ip_ttl;     /* time to live         */
    uchar ip_p;      /* protocol         */
    ushort ip_sum;     /* checksum         */
    IP_ADDR_T ip_src;    /* Source IP address        */
    IP_ADDR_T ip_dst;    /* Destination IP address   */
    ushort udp_src;     /* UDP source port      */
    ushort udp_dst;     /* UDP destination port     */
    ushort udp_len;     /* Length of UDP packet     */
    ushort udp_xsum;    /* Checksum         */
} IP_UDP_HDR_T;

#define IP_HDR_SIZE_NO_UDP (sizeof (IP_UDP_HDR_T) - 8)
#define IP_HDR_SIZE  (sizeof (IP_UDP_HDR_T))

/*
 * The following functions are a bit ugly, but necessary to deal with
 * alignment restrictions on ARM.
 *
 * We're using inline functions, which had the smallest memory
 * footprint in our tests.
 */
/* return IP *in network byteorder* */
static INLINE IP_ADDR_T NetReadIP(volatile void *from)
{
    IP_ADDR_T ip;
    memcpy((void *)&ip, (void *)from, sizeof(ip));
    return ip;
}

/* return ulong *in network byteorder* */
static INLINE ulong NetReadLong(ulong * from)
{
    ulong l;
    memcpy((void *)&l, (void *)from, sizeof(l));
    return l;
}

/* write IP *in network byteorder* */
static INLINE void NetWriteIP(void *to, IP_ADDR_T ip)
{
    memcpy(to, (void *)&ip, sizeof(ip));
}

/* copy IP */
static INLINE void NetCopyIP(volatile void *to, void *from)
{
    memcpy((void *)to, from, sizeof(IP_ADDR_T));
}

/* copy ulong */
static INLINE void NetCopyLong(ulong * to, ulong * from)
{
    memcpy((void *)to, (void *)from, sizeof(ulong));
}

int NetCksumOk(uchar * ptr, int len);
void NetSetIP(uchar * xip, IP_ADDR_T dest, IP_ADDR_T src, int dport, int sport,
              int len);
unsigned NetCksum(uchar * ptr, int len);

#endif

#ifndef __DHCP_H__
#define __DHCP_H__

#define RC_DHCP_ERR    		DEF_USRRC(APP_MODULE, NETWORK_LAYER, 101)

#pragma pack(1)

typedef struct _dhcpmessage
{
    byte op;
    byte htype;
    byte hlen;
    byte hops;
    UINT32 xid;
    UINT16 secs;
    UINT16 flags;
    UINT32 ciaddr;
    UINT32 yiaddr;
    UINT32 siaddr;
    UINT32 giaddr;
    byte chaddr[16];
    byte sname[64];
    byte file[128];
    byte magic[4];
    byte opt[3];
    byte end;
}dhcpmessage_t;

#pragma pack(1)
typedef struct _dhcp_opt_header
{
	byte opt_type;
	byte opt_len;
	byte ptr;
}
dhcp_opt_header_t;


#define DHCP_CLIENT_PORT 68
#define DHCP_SERVER_PORT 67

#define DHCP_OPT_MSG_TYPE	53
#define DHCP_OPT_NET_MASK	1
#define DHCP_OPT_ADDR_LEASE_TIME	51
#define DHCP_OPT_DNS	6
#define DHCP_OPT_END	0xFF
#define DHCP_REPLAY_TIMEOUT 1000
#define DHCP_RECV_ATTEMPT 5

MXRC dhcp_request(net_conf_t *conf);

#endif

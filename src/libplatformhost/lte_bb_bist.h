#ifndef BIST_LTE_H
#define BIST_LTE_H

#define BIST_ERR_LOG(format...)            syslog(LOG_ERR,  format)
#define BIST_CRIT_LOG(format...)           syslog(LOG_CRIT, format)

#define DEBUG_BIST

#ifdef DEBUG_BIST
#define BIST_DBG_LOG(x...)          printf(x)
#else
#define BIST_DBG_LOG(x...)
#endif

#define BIST_ERROR   -1
#define BIST_SUCCESS  0

/* defines */
#define PING_TIMEOUT        5
#define PING_SND_BUF_SZ     64
#define PING_RCV_BUF_SZ     128
#define PROTO_ICMP          1
#define TST(bit)            (A(bit) & B(bit))
#define SINADDROF(x)        (((struct sockaddr_in *)x)->sin_addr)
#define INA                 struct in_addr

#define BACKHUAL_ETHERNET_PORT "eth2"
#define DEBUG_ETHERNET_PORT    "eth1"


typedef enum e_BistReturnCode
{
    BISTRETURNCODE_SUCCESS = 150,
    BISTRETURNCODE_FAILURE,
    BISTRETURNCODE_INVALID_PARAMETERS,
}BistReturnCode;


typedef enum e_BistTestType
{
    BISTTESTTYPE_BACKHAUL_ETHERNET_PORT=100,
    BISTTESTTYPE_DEBUG_ETHERNET_PORT,
    BISTTESTTYPE_PHY,
    BISTTESTTYPE_LOWERARM,
    BISTTESTTYPE_ALL
}BistTestType;

typedef enum e_EtherTestType
{
    ETHERTESTYPE_INTERFACE_FLAGS =110,
    ETHERTESTYPE_PING_PACKET_TEST,
    ETHERTESTYPE_ALL
}EtherTestType;

extern BistReturnCode bistTestStart(BistTestType bistTestType , EtherTestType etherTestType);
#endif /*BIST_LTE_H*/

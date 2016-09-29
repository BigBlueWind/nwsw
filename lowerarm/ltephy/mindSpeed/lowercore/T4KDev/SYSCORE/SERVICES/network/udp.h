#ifndef _UDP_H_
#define _UDP_H_

#include "net_types.h"
#include "network.h"

#pragma pack(1)

typedef struct _udphdr_t { // 8 bytes long
    UINT16 source;     // source port number
    UINT16 dest;       // destination port number
    UINT16 len;        // length of the data including 8 bytes for the header
    UINT16 check;      // packet checksum
}udphdr_t;

#define udphdr_size sizeof(udphdr_t)

MXRC net_udp_init(void);
MXRC net_udp_parce_pkt(byte *pkt, size_t size);
MXRC net_udp_bind(connection_t *conn, net_sockaddr *addr);
MXRC net_udp_close(connection_t *conn);
MXRC net_udp_connect(connection_t *conn, net_sockaddr *to);
MXRC net_udp_send(connection_t *conn, const byte *data, size_t size, int flags);
MXRC net_udp_send_to(connection_t *conn, net_sockaddr *to, const byte *data, size_t size, int flags);
int net_udp_recv_from(connection_t *conn, byte *buff, size_t size, net_sockaddr *addr, int flags);
int net_udp_recv(connection_t *conn, byte *buff, size_t size, int flags);
MXRC net_udp_rx_queue_put(connection_t *conn);

#endif

#ifndef __RAW_H__
#define __RAW_H__

#include "net_types.h"
#include "network.h"

MXRC net_raw_init(void);

bool net_raw_parce_pkt(byte *pkt, size_t size);
MXRC net_raw_bind(connection_t *conn, net_sockaddr *addr);
MXRC net_raw_close(connection_t *conn);
MXRC net_raw_connect(connection_t *conn, net_sockaddr *to);
MXRC net_raw_send_to(connection_t *conn, net_sockaddr *to, const byte *data, size_t size, int flags);
int net_raw_recv_from(connection_t *conn, byte *buff, size_t size, net_sockaddr *addr, int flags);
MXRC net_raw_send(connection_t *conn, const byte *data, size_t size, int flags);
int net_raw_recv(connection_t *conn, byte *buff, size_t size, int flags);

MXRC net_raw_rx_queue_put(connection_t *conn);
connection_t * net_raw_rx_queue_get(void);
connection_t ** net_raw_rx_queue_get_first(void);
connection_t ** net_raw_rx_queue_get_next(connection_t **prev);
MXRC net_raw_rx_queue_remove(connection_t *conn);

#endif


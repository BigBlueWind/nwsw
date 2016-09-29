#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include "net_types.h"
#include "network.h"
#include "mem.h"

//syncronizations primitive functions
void net_cond_init(volatile net_cond_t *cond);
void net_cond_wait(volatile net_cond_t *cond);
unsigned int net_cond_wait_timeout(volatile net_cond_t *cond, unsigned int to);
void net_cond_send(volatile net_cond_t *cond);
bool net_cond_check(volatile net_cond_t *cond);

void net_lock (resource_ctx *ctx);
void net_unlock (resource_ctx *ctx);


MXRC net_queue_put(connection_t **queue, UINT32 queue_size, connection_t *conn);
connection_t * net_queue_get(connection_t **queue, UINT32 queue_size);
connection_t ** net_queue_get_first(connection_t **queue, UINT32 queue_size);
connection_t ** net_queue_get_next(connection_t **queue, connection_t **prev, UINT32 queue_size);
MXRC net_queue_remove(connection_t **queue, connection_t * conn, UINT32 queue_size);

#endif

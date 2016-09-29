#ifndef __net_buff_H__
#define __net_buff_H__

#include "net_types.h"
#include "net_list.h"
#include "cycle_buffer.h"

#define NET_USE_CB 1

#ifdef NET_USE_LIST
	typedef net_list_t net_buff_t;
#elif defined( NET_USE_CB )
	typedef cyclic_buffer_t net_buff_t ;
#define NET_CB_SIZE  10240

	typedef struct _cb_el_t
	{
		UINT32 size;
		void  *ptr;
	}cb_el_t;
#endif

net_buff_t  *net_buff_create(void);
MXRC net_buff_init(net_buff_t *queue);
MXRC net_buff_put(net_buff_t *queue, byte *data, UINT32 size);
MXRC net_buff_get(net_buff_t *queue, byte **data, UINT32 *size);
MXRC net_buff_peek(net_buff_t *queue, byte **data, UINT32 *size);
MXRC net_buff_destroy(net_buff_t *queue);
bool net_buff_empty(net_buff_t *queue);
MXRC net_buff_remove_last(net_buff_t *queue);

#endif

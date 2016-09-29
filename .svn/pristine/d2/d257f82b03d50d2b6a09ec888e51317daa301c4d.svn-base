#ifndef _NET_LIST_H_
#define _NET_LIST_H_

#include "net_types.h"

typedef struct _net_list_el_t net_list_el_t;

struct _net_list_el_t
{
	byte 	*data;
	UINT32	size;
	net_list_el_t *next;
};

typedef struct _net_list_t
{
	net_list_el_t *head;
	net_list_el_t *tail;
	UINT32		   count;	
}net_list_t;

net_list_t  *net_list_create(void);
MXRC net_list_init(net_list_t *list);
MXRC net_list_put(net_list_t *list, byte *data, UINT32 size);
MXRC net_list_get(net_list_t *list, byte **data, UINT32 *size);
MXRC net_list_peek(net_list_t *list, byte **data, UINT32 *size);
MXRC net_list_destroy(net_list_t *list);
bool net_list_empty(net_list_t *list);
MXRC net_list_remove_last(net_list_t *list);

#endif

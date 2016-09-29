#include "net_types.h" 
#include "net_list.h"
#include "network.h"
#include <string.h>
#include "utils.h"

net_list_t  *net_list_create(void)
{
	net_list_t *new_list;
	
	new_list = (net_list_t *)net_alloc(sizeof(net_list_t));
	
	return new_list;
}

MXRC net_list_init(net_list_t *list)
{
	memset(list, 0, sizeof(net_list_t));
	
	return MX_OK;
}


MXRC net_list_put(net_list_t *list, byte *data, UINT32 size)
{
	net_list_el_t *new;
	
	if(!data || !size)
		return RC_NETWORK_UNKNOWN_ERROR;
	
	//allocation of memory	
	new = net_alloc(sizeof(net_list_el_t));
	
	if(!new)
		return RC_NETWORK_UNKNOWN_ERROR;
	
	net_uart_printf ("net_list_put:new=%x\r\n", new);

	new->data = data;
	new->size = size;
	new->next = NULL;
	if(!list->head)
	{
		list->head = new;
		list->tail = list->head;
	}	
	else
	{
		list->tail->next = new;
		list->tail = new;
	}
	
	net_uart_printf("net_list_put: list->head = %x,  %x\r\n", list->head, list->tail);
	
	return MX_OK;
}

MXRC net_list_get(net_list_t *list, byte **data, UINT32 *size)
{	
	if(!list->head)
		return RC_NETWORK_UNKNOWN_ERROR;
	
	*data = list->head->data;
	*size = list->head->size;

	net_free(list->head);
	
	list->head = list->head->next;
	
	if(!list->head)
		list->tail = list->head;
	
	return MX_OK;
}

MXRC net_list_remove_last(net_list_t *list)
{
	if(!list->head)
		return RC_NETWORK_UNKNOWN_ERROR;

	net_free(list->head);
	
	list->head = list->head->next;
	
	if(!list->head)
		list->tail = list->head;
	
	return MX_OK;

}

MXRC net_list_peek(net_list_t *list, byte **data, UINT32 *size)
{	
	net_uart_printf("net_list_peek: list->head = %x\r\n", list->head);
	
	if(!list->head)
		return RC_NETWORK_UNKNOWN_ERROR;
	
	*data = list->head->data;
	*size = list->head->size;
	
	return MX_OK;
}


MXRC net_list_destroy(net_list_t *list)
{
	net_list_el_t *el, *next_el;
	
	el = list->head;
	
	while(el)
	{		
		net_free(el->data);
		next_el = el->next;
		net_free(el);
		el = next_el;
	}
	
	net_free(list);
	
	return MX_OK;
}

bool net_list_empty(net_list_t *list)
{
	if(!list)
		return true;
	
	if(list->head == NULL)
		return true;
	
	return false;
}


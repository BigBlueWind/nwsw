#include "net_buff.h"
#include "utils.h"

net_buff_t  *net_buff_create(void)
{
#ifdef NET_USE_LIST
	return net_list_create();
#elif defined( NET_USE_CB )
	void *ptr;
	ptr = net_alloc(NET_CB_SIZE);
	return cyclic_buffer_init(ptr, NET_CB_SIZE);

#endif
}

MXRC net_buff_init(net_buff_t *queue)
{
#ifdef NET_USE_LIST
	return net_list_init(queue);
#elif defined( NET_USE_CB )
	return MX_OK;
#endif
}

MXRC net_buff_put(net_buff_t *queue, byte *data, UINT32 size)
{
#ifdef NET_USE_LIST
	return net_list_put(queue, data, size);
#elif defined( NET_USE_CB )
	int rc;

	cb_el_t tmp;
	tmp.size = size;
	tmp.ptr = data;
	rc = cyclic_buffer_put(queue, (unsigned char *)&tmp, sizeof(tmp));

	if(rc != 0)
		return RC_NETWORK_QUEUE_ERR;

	return MX_OK;

#endif
}

MXRC net_buff_get(net_buff_t *queue, byte **data, UINT32 *size)
{
#ifdef NET_USE_LIST
	return net_list_get(queue, data, size);
#elif defined( NET_USE_CB )
	int rc;
	cb_el_t tmp;
	size_t s;

	rc = cyclic_buffer_get(queue, (unsigned char *)&tmp, &s);

	if(rc != 0)
		return RC_NETWORK_QUEUE_ERR;

	*data = tmp.ptr;
	*size = tmp.size;

	return MX_OK;
#endif
}

MXRC net_buff_peek(net_buff_t *queue, byte **data, UINT32 *size)
{
#ifdef NET_USE_LIST
	return net_list_peek(queue, data, size);
#elif defined( NET_USE_CB )
	int rc;
	cb_el_t tmp;
	size_t s;

	rc = cyclic_buffer_peek(queue, (unsigned char *)&tmp, &s);

	if(rc != 0)
		return RC_NETWORK_QUEUE_ERR;


	*data = tmp.ptr;
	*size = tmp.size;

	return MX_OK;
#endif
}

MXRC net_buff_destroy(net_buff_t *queue)
{
#ifdef NET_USE_LIST
	return net_list_destroy(queue);
#elif defined( NET_USE_CB )
	net_free(queue->buffer_start);
	cyclic_buffer_free(queue);
	return MX_OK;
#endif
}

bool net_buff_empty(net_buff_t *queue)
{
#ifdef NET_USE_LIST
	return net_list_empty(queue);
#elif defined( NET_USE_CB )
	return !queue->item_count;
#endif
}

MXRC net_buff_remove_last(net_buff_t *queue)
{
#ifdef NET_USE_LIST
	return net_list_remove_last(queue);
#elif defined( NET_USE_CB )
	int rc;
	cb_el_t tmp;
	size_t s;

	rc = cyclic_buffer_get(queue, (unsigned char *)&tmp, &s);

	if(rc != 0)
		return RC_NETWORK_QUEUE_ERR;

	return MX_OK;
#endif
}

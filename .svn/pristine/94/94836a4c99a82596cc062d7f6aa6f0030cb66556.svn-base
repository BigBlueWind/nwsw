#include "utils.h"
#include "network.h"
#include "heaps.h"

typedef struct _timeval_t
{
     long tv_sec;     /* seconds */
     long tv_usec;    /* and microseconds */
} timeval;


bool net_cond_check(volatile net_cond_t *cond)
{
	return *cond; 
}

void net_cond_wait(volatile net_cond_t *cond)
{

	while (!*cond) {_NOP();};

	*cond = 0;
}

unsigned int net_cond_wait_timeout(volatile net_cond_t *cond, unsigned int to)
{
	unsigned int start_time;
	unsigned int cur_time;

	start_time = cur_time = MxGetTicks();

	while (!*cond && (cur_time - start_time) < (to * 150)) 
	{
		cur_time = MxGetTicks();
		_NOP();
	};

	if(cur_time - start_time < (to * 150))
	{
		*cond = 0;		
		return 0;
	}

	return 1;
}


void net_cond_send(volatile net_cond_t *cond)
{
	*cond = 1;
}

void net_cond_init(volatile net_cond_t *cond)
{
	*cond = 0;
}

void * net_alloc(size_t size)
{
	return MxHeapAlloc(AppHeap/*NcnbHeap*/ , size);
}

void net_free(void *ptr)
{
	MxHeapFree(AppHeap/*NcnbHeap*/, ptr);
}

UINT16 htons(UINT16 val)
{
  return HTONS(val);
}

UINT32 hton(UINT32 val)
{
  return HTON(val);
}

#ifdef NET_DEBUG

void net_dump_data(const byte *data, const int  size)
{

		int _s = 0;

		uart_printf("\n");

		for ( ;_s < size; _s++ )
		{
			uart_printf("%02X ", (unsigned char)data[_s]);
			if ( ((_s + 1) % 16) == 0 )
				uart_printf("\n");

		}

		uart_printf("\n");   

}
#endif

MXRC net_queue_put(connection_t **queue, UINT32 queue_size, connection_t *conn)
{
	UINT32 i;
	
	for(i = 0; i < queue_size; i++)
		if(queue[i] == NULL)
		{
			queue[i] = conn;
			return MX_OK;			
		}
	
	return RC_NETWORK_QNS;
}


connection_t * net_queue_get(connection_t **queue, UINT32 queue_size)
{
	UINT32 i;
	connection_t *res;
	
	for(i = 0; i < queue_size; i++)
		if( queue[i] )
		{
			res = queue[i];
			queue[i] = NULL;
			return res;					
		}
	
	return NULL;
}

connection_t ** net_queue_get_first(connection_t **queue, UINT32 queue_size)
{
	UINT32 i;
	
	for(i = 0; i < queue_size; i++)
		if( queue[i] )
		{	
			return &queue[i];								
		}
	
	return NULL;
}

connection_t ** net_queue_get_next(connection_t **queue, connection_t **prev, UINT32 queue_size)
{
	if(prev && prev != queue + (queue_size - 1))
		return prev + 1;
	
	return NULL;
}


MXRC net_queue_remove(connection_t **queue, connection_t * conn, UINT32 queue_size)
{
	UINT32 i;
		
	for(i = 0; i < queue_size; i++)
		if( queue[i] && queue[i] == conn)
		{	
			queue[i] = NULL;
			return MX_OK;
		}
		
	return RC_NETWORK_NOSOC_ERROR;
}

void net_lock (resource_ctx *ctx)
{
    ctx->irq[MxGetCpuID ()] = ARM_INT_disable ();
    MxGetLock (&ctx->lock);
}

void net_unlock (resource_ctx *ctx)
{
    MxReleaseLock (&ctx->lock);
    ARM_INT_restore (ctx->irq[MxGetCpuID ()]);
}

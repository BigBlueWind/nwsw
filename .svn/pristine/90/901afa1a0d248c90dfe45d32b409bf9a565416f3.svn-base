#include <stdarg.h>
#include "intercpu.h"
#include "appprintf.h"
#include <string.h>
#include "apimgr.h"
#include "4gmx.h"
#include "heaps.h"
#include "hbidrv.h"
#include "sirqassign.h"
#include "icdrv.h"
#include "systimers.h"
#include "initcpu.h"
#include "icpu2.h"
#include "lcorecfg.h"
#include "conicpu.h"


static CharICPUCtx conicpu_ctx = {NULL, NULL, 0, 0, 0, 0};

//static	char tmp_buff[1024];

#define CINICPU_DEBUG
#define GET_QUEUE_SIZE(q) ((q->qgetp <= q->qputp) ? (q->qputp - q->qgetp) : (q->qsize - q->qputp + q->qgetp))
#define GET_QUEUE_FREE_SPACE(q) ((q->qgetp <= q->qputp) ? ((q->qsize - q->qputp) + q->qgetp) : (q->qgetp - q->qputp))


MXRC CharICPUCtxInit(CharICPUMailbox *mbox,  CharICPUCtx *ctx, UINT32 buf_size);
MXRC CharICPUCtxRelease(CharICPUCtx *ctx);
MXRC CharICPUQueueInit(CharICPUQueue *queue, LPVOID storage, UINT32 charnum);
MXRC CharICPUQueuePutCh(CharICPUQueue *queue, UINT8 ch);
MXRC CharICPUQueueGetCh(CharICPUQueue *queue, PUINT8 ch);
MXRC CharICPUQueuePutData(CharICPUQueue *queue, PUINT8 data, UINT32 size);
MXRC CharICPUQueueGetData(CharICPUQueue *queue, PUINT8 data, PUINT32 size);
UINT32 CharICPUQueueGetFreeSpace(CharICPUQueue *queue);




void ConICPUIntHandle(volatile UINT32 * pClear, UINT32 nIrqID, LPVOID pParam)
{
#ifdef _MX_DEBUG
    MXRC rc = MX_OK;
#endif

    CharICPUCtx * pCtx;

    pCtx = (CharICPUCtx *)pParam;

    _ASSERT_PTR(pCtx);

    _ASSERT(HANDLE_NOT_NULL(pCtx->hHandler));

    // This function should be called to clean
    // inter-cpu irq

    ICDrvCleanInterCpu (pCtx->fromUARMIRQ);

    if(HANDLE_NOT_NULL(pCtx->hHandler))
    {
#ifdef _MX_DEBUG
	    rc = MxScheduleThread(pCtx->hHandler);
#else
	    MxScheduleThread(pCtx->hHandler);
#endif
	    _ASSERT_RC(rc);
    }
    *pClear = nIrqID;
}


UINT32 ConICPUIsInit(void)
{
	return conicpu_ctx.inited;
}

MXRC ConICPUInit(CIPIHandler hand)
{
	CharICPUMailbox* icpu_mbox;
	MXRC rc;

	if(conicpu_ctx.inited)
	{
		uart_printf("CONICPU: Init : seems to be inited\r\n");
		return MX_OK;
	}
		

	memset(&conicpu_ctx, 0, sizeof(ICCTX));

	icpu_mbox = (CharICPUMailbox*) CoreCfg.ConICPUBase;

	uart_printf("CONICPU: Init : baseaddr = 0x%X\r\n", icpu_mbox);

	CharICPUCtxInit(icpu_mbox, &conicpu_ctx, ICPU_QUEUE_BUFSIZE);
	conicpu_ctx.toUARMIRQ = CICPU_TO_UARM_IRQ;
	conicpu_ctx.fromUARMIRQ = CICPU_FROM_UARM_IRQ;

	uart_printf("CONICPU: Init : icpu mailbox = 0x%X\r\n", conicpu_ctx.IcpuMailbox);
	uart_printf("CONICPU: Init : icpu to uarm queue = 0x%X\r\n", conicpu_ctx.IcpuMailbox->IcpuQueueToUarm);
	uart_printf("CONICPU: Init : icpu from uarm queue = 0x%X\r\n", conicpu_ctx.IcpuMailbox->IcpuQueueFromUarm);
	uart_printf("CONICPU: Init : buff queue = 0x%X\r\n", conicpu_ctx.BufferQueue);	

	if(hand)
		rc = MxCreateThread((TENTRY)hand, NULL, &conicpu_ctx, (TPRIORITY)3,
								 &conicpu_ctx.hHandler);
	
	rc = MxSetIntHandler(conicpu_ctx.fromUARMIRQ, INT_TYPE_IRQ,
                                  PRI_ICPU, ConICPUIntHandle, &conicpu_ctx);
    rc = MxEnableInt(conicpu_ctx.fromUARMIRQ);

	conicpu_ctx.inited = 1;

	return rc;

}

MXRC ConICPURegHandler(CIPIHandler hand)
{
	MXRC rc = MX_OK;

	if(hand)
		rc = MxCreateThread((TENTRY)hand, NULL, &conicpu_ctx, (TPRIORITY)3,
								 &conicpu_ctx.hHandler);	
	return rc;
}


void ConICPUExit(void)
{
	if(HANDLE_NOT_NULL(conicpu_ctx.hHandler))
	{
		MxRemoveThread(conicpu_ctx.hHandler);
		conicpu_ctx.hHandler = HNULL;
	}

	if(HANDLE_NOT_NULL(conicpu_ctx.hTimer))
	{
		MxStopTimer(conicpu_ctx.hTimer);
		MxRemoveTimer(conicpu_ctx.hTimer);
		conicpu_ctx.hTimer= HNULL;
	}
	
	if(HANDLE_NOT_NULL(conicpu_ctx.hEvent))
	{		
		MxRemoveEvent(conicpu_ctx.hEvent);
		conicpu_ctx.hEvent= HNULL;
	}
			

	if(HANDLE_NOT_NULL(conicpu_ctx.hRefresh))
	{
		MxRemoveThread(conicpu_ctx.hRefresh);
		conicpu_ctx.hRefresh= HNULL;
	}

	MxSetIntHandler(conicpu_ctx.fromUARMIRQ, INT_TYPE_IRQ,
                                  PRI_ICPU, NULL, NULL);

	MxDisableInt(conicpu_ctx.fromUARMIRQ);

	CharICPUCtxRelease(&conicpu_ctx);
	
}

MXRC CharICPUCtxInit(CharICPUMailbox *mbox,  CharICPUCtx *ctx, UINT32 buf_size)
{
	CharICPUQueue *buf_queue;
	PUINT8 buf_storage;

	ctx->IcpuMailbox= mbox;

	buf_queue   = (CharICPUQueue *)MxHeapAlloc(AppHeap, sizeof(CharICPUQueue));
	buf_storage = (PUINT8)MxHeapAlloc(AppHeap, buf_size);

	CharICPUQueueInit(buf_queue, buf_storage, buf_size);

	buf_queue->qstorage = buf_storage;

	ctx->BufferQueue = buf_queue;

	return ICPU_QUEUE_OK;
}

MXRC CharICPUCtxRelease(CharICPUCtx *ctx)
{
	_ASSERT_PTR(ctx);

	MxHeapFree(AppHeap, ctx->BufferQueue->qstorage);
	MxHeapFree(AppHeap, ctx->BufferQueue);

	return MX_OK;
}

MXRC CharICPUQueueInit(CharICPUQueue *queue, LPVOID storage, UINT32 charnum)
{
	memset(queue, 0, sizeof(CharICPUQueue));

	queue->qstorage = (UINT8 *) storage;
	queue->qsize = charnum;
	
	return ICPU_QUEUE_OK;
}

UINT32 CharICPUQueueGetFreeSpace(CharICPUQueue *queue)
{
	UINT32 freespace = 0;

	_ASSERT_PTR(queue);
	MxGetLock(&queue->sync);

	freespace = GET_QUEUE_FREE_SPACE(queue);
	
	MxReleaseLock(&queue->sync);

	return freespace;
}

unsigned int CharICPUQueueGetSize(CharICPUQueue *q)
{
	unsigned int  size = 0;
	
	MxGetLock(&q->sync);

	size = GET_QUEUE_SIZE(q);	
	MxReleaseLock(&q->sync);

	return size;
}

MXRC CharICPUQueuePutCh(CharICPUQueue *queue, UINT8 ch)
{
	_ASSERT_PTR(queue);

	MxGetLock(&queue->sync);
	
	if((queue->qputp + 1 == queue->qgetp) || ( (queue->qputp == queue->qsize) && (queue->qgetp == 0)))		
	{
		MxReleaseLock(&queue->sync);
		return ICPU_QUEUE_FULL;
	}
	
	queue->qstorage[queue->qputp++] = ch;

	if(queue->qputp >= queue->qsize)		
		queue->qputp = 0;

	MxReleaseLock(&queue->sync);

	return ICPU_QUEUE_OK;
}

MXRC CharICPUQueueGetCh(CharICPUQueue *queue, PUINT8 ch)
{
	_ASSERT_PTR(queue);
	
	MxGetLock(&queue->sync);

	if(!GET_QUEUE_SIZE(queue))
	{
		MxReleaseLock(&queue->sync);
		return ICPU_QUEUE_EMPTY;
	}

	*ch = queue->qstorage[queue->qgetp++];

	if(queue->qgetp >= queue->qsize)		
		queue->qgetp = 0;

	MxReleaseLock(&queue->sync);

	return ICPU_QUEUE_OK;
}

MXRC CharICPUQueuePutData(CharICPUQueue *queue, PUINT8 data, UINT32 size)
{
	_ASSERT_PTR(queue);
	_ASSERT_PTR(data);
	
	MxGetLock(&queue->sync);

	if((queue->qputp + 1 == queue->qgetp) || ( (queue->qputp == queue->qsize) && (queue->qgetp == 0)))
	{
		MxReleaseLock(&queue->sync);
		return ICPU_QUEUE_FULL;
	}

	if(queue->qputp + size <= queue->qsize)			
	{
		memcpy(queue->qstorage + queue->qputp, data, size);
		queue->qputp += size;
	}
	else
	{
		int tail;
		int front = queue->qsize - queue->qputp;		
		tail = size - front;
		memcpy(queue->qstorage + queue->qputp, data, front);
		memcpy(queue->qstorage, data + front, tail);
		queue->qputp = tail;
	}

	MxReleaseLock(&queue->sync);

	return ICPU_QUEUE_OK;
}


MXRC CharICPUQueueGetData(CharICPUQueue *queue, PUINT8 data, PUINT32 size)
{
	_ASSERT_PTR(queue);
	_ASSERT_PTR(data);

	MxGetLock(&queue->sync);

	if(!GET_QUEUE_SIZE(queue))
	{
		MxReleaseLock(&queue->sync);
		return ICPU_QUEUE_EMPTY;
	}

	*size = MIN(*size, GET_QUEUE_SIZE(queue));

	if(*size <= queue->qsize - queue->qgetp)
	{
		memcpy(data, queue->qstorage + queue->qgetp, *size);
		queue->qgetp += *size;
	}
	else
	{
		int tail;
		int front = queue->qsize - queue->qgetp;		
		tail = *size - front;
		memcpy(data, queue->qstorage + queue->qgetp, front);
		memcpy(data + front, queue->qstorage, tail);
		queue->qgetp= tail;
	}

	MxReleaseLock(&queue->sync);

	return ICPU_QUEUE_OK;
}

MXRC ConICPUPutCh(UINT8 ch, BOOL notify)
{
	MXRC rc;
	
	CharICPUCtx *ctx = &conicpu_ctx;

	if(!conicpu_ctx.inited)
	{
		uart_force_printf("not inited\r\n");
		return 0;
	}

	_ASSERT_PTR(ctx);
	_ASSERT_PTR(ctx->IcpuMailbox);
	_ASSERT_PTR(ctx->BufferQueue);	

	MxGetLock(&ctx->sync);

	if((rc = CharICPUQueuePutCh(&ctx->IcpuMailbox->IcpuQueueToUarm, ch)) == ICPU_QUEUE_FULL)
	{
		MxReleaseLock(&ctx->sync);
		return rc;
	}

	 if(notify)	 	
		 rc = ICDrvRaiseInterCpu (0, ctx->toUARMIRQ);

    MxReleaseLock(&ctx->sync);

	return rc;
}

MXRC ConICPUGetCh(PUINT8 ch)
{
	CharICPUCtx *ctx = &conicpu_ctx;
	MXRC rc;

	if(!conicpu_ctx.inited)
		return 0;

	_ASSERT_PTR(ctx);
	_ASSERT_PTR(ctx->IcpuMailbox);

	MxGetLock(&ctx->sync);

	rc = CharICPUQueueGetCh(&ctx->IcpuMailbox->IcpuQueueFromUarm, ch);	

	MxReleaseLock(&ctx->sync);

	return rc;
}

MXRC ConICPUPutData(PUINT8 data, UINT32 size, BOOL notify)
{
	MXRC rc;

	CharICPUCtx *ctx = &conicpu_ctx;

	if(!conicpu_ctx.inited)
		return 0;

	_ASSERT_PTR(ctx);	
	_ASSERT_PTR(ctx->IcpuMailbox);

	MxGetLock(&ctx->sync);

	if((rc = CharICPUQueuePutData(&ctx->IcpuMailbox->IcpuQueueToUarm, data, size)) == ICPU_QUEUE_FULL)
	{
		MxReleaseLock(&ctx->sync);
		return rc;
	}

	if(notify)
		rc = ICDrvRaiseInterCpu (0, ctx->toUARMIRQ);

	MxReleaseLock(&ctx->sync);
	return rc;
}


MXRC ConICPUNotify(void)
{
	CharICPUCtx *ctx = &conicpu_ctx;

	_ASSERT_PTR(ctx);	
	
	return ICDrvRaiseInterCpu (0, ctx->toUARMIRQ);
}


static VUINT32 cicpu_prn_lock = 0;
static char cicpu_buffer [2*1024];


int cicpu_printf(const char *fmt, ...)
{
    va_list args;
    IRQTYPE irq;
    int length = 0;

    // synchronization is needed here because we use
    // the same UART device, so in this case we can use one global buffer
    // instead of local one located on the stack, also the size of local buffer
    // can be much more bigger related to the local one

	if(!conicpu_ctx.inited)
		return 0;
	

    irq = ARM_INT_disable();
    MxGetLock (&cicpu_prn_lock);

    va_start(args, fmt);     /* Initialize variable arguments. */

    // ------------------------------------------------------------------
    // Safe function is used to prevent buffer overwriting
    // ------------------------------------------------------------------
    length = _vsnprintf(cicpu_buffer, sizeof (cicpu_buffer) - 1, fmt, args);

	ConICPUPutData((PUINT8)cicpu_buffer, length, TRUE);

    MxReleaseLock (&cicpu_prn_lock);
    ARM_INT_restore (irq);

    va_end(args);

    return length;
}

//unused code

/*
	if(!ctx->BufferQueue->qcharnum)
	if(!CharICPUQueueGetSize(ctx->BufferQueue))
	{
		uart_force_printf("ConICPUPutCh: try put to icpu queue\r\n");
		if((rc = CharICPUQueuePutCh(&ctx->IcpuMailbox->IcpuQueueToUarm, ch)) == ICPU_QUEUE_FULL)
		{
			uart_force_printf("ConICPUPutCh : unsuccess, try put to internal queue\r\n");
			if((rc = CharICPUQueuePutCh(ctx->BufferQueue, ch)) != ICPU_QUEUE_OK)
			{
				MxReleaseLock(&ctx->sync);
				return rc;
			}
		}
	}
	else 
	{
		UINT32 tsize;

//		uart_force_printf("ConICPUPutCh: try put to internal queue\r\n");
		if((rc = CharICPUQueuePutCh(ctx->BufferQueue, ch)) != ICPU_QUEUE_OK)
		{
			MxReleaseLock(&ctx->sync);
			return rc;
		}

		tsize = CharICPUQueueGetFreeSpace(&ctx->IcpuMailbox->IcpuQueueToUarm);

//		uart_force_printf("ConICPUPutCh : free space in icpu queue is %d\r\n", tsize);
		
		if(tsize)
		{
			//uart_force_printf("ConICPUPutCh : try to copy from internal queue to icpu queue\r\n");		
			//MxDelayTicks (US_TO_TICKS(900));
			if((rc = CharICPUQueueGetData(ctx->BufferQueue, (PUINT8)tmp_buff, &tsize)) == ICPU_QUEUE_OK)
			{
				if((rc = CharICPUQueuePutData(&ctx->IcpuMailbox->IcpuQueueToUarm, (PUINT8)tmp_buff, tsize)) != ICPU_QUEUE_OK)
				{
					uart_force_printf("ConICPUPutCh: internal queue overflow\r\n");
					MxReleaseLock(&ctx->sync);
					return ICPU_QUEUE_ERROR;
				}
			}
			//uart_force_printf("ConICPUPutCh : success\r\n");
		}

	}*/

	/*
	if(!CharICPUQueueGetSize(ctx->BufferQueue))
	{
		//uart_force_printf("ConICPUPutData: try put to icpu queue\r\n");
		if((rc = CharICPUQueuePutData(&ctx->IcpuMailbox->IcpuQueueToUarm, data, size)) == ICPU_QUEUE_FULL)
		{
			//uart_force_printf("ConICPUPutData : unsuccess, try put to internal queue\r\n");
			if((rc = CharICPUQueuePutData(ctx->BufferQueue, data, size)) != ICPU_QUEUE_OK)
			{
				//uart_force_printf("CharICPUPutData: out put in tmp buff 1 error %d\r\n", rc);
				MxReleaseLock(&ctx->sync);
				return rc;
			}
		}
	}
	else 
	{		
		UINT32 tsize;

		//uart_force_printf("ConICPUPutData: try put to internal queue\r\n");
		if((rc = CharICPUQueuePutData(ctx->BufferQueue, data, size)) != ICPU_QUEUE_OK)
		{		
			//uart_force_printf("CharICPUPutData: out put in tmp buff 2 error  %d\r\n", rc);			
			MxReleaseLock(&ctx->sync);
			return rc;
		}
		
		tsize = CharICPUQueueGetFreeSpace(&ctx->IcpuMailbox->IcpuQueueToUarm);

		//uart_force_printf("ConICPUPutData : free space in icpu queue is %d\r\n", tsize);		

		if(tsize)
		{
			if((rc = CharICPUQueueGetData(ctx->BufferQueue, (PUINT8)tmp_buff, &tsize)) == ICPU_QUEUE_OK)
				if((rc = CharICPUQueuePutData(&ctx->IcpuMailbox->IcpuQueueToUarm, (PUINT8)tmp_buff, tsize)) != ICPU_QUEUE_OK)
				{
					uart_force_printf("CharICPUPutData: internal queue overflow\r\n");
					MxReleaseLock(&ctx->sync);
					return ICPU_QUEUE_ERROR;
				}
			//uart_force_printf("ConICPUPutData : success\r\n");
		}

	}*/


	/*
					
		rc = MxCreateThread((TENTRY)ConICPURefresh, NULL, NULL, 0, &conicpu_ctx.hRefresh);	
		if(FAILED(rc))
			uart_force_printf("CreateThread error %d\r\n", rc);
	
		MxScheduleThread(conicpu_ctx.hRefresh);
	
	   */
		/*
		rc = MxCreateEvent(0, NULL, &conicpu_ctx.hEvent);
		if(FAILED(rc))
			uart_force_printf("CreateEvent error %d\r\n", rc);
		
		rc = MxCreateTimer(TIMER_MODE_PERIOD , 100, NULL, conicpu_ctx.hEvent, &conicpu_ctx.hTimer);
		
		if(FAILED(rc))
		   uart_force_printf("MxCreateTimer error %d\r\n", rc);
	
		rc = MxScheduleThreadByEvent(conicpu_ctx.hRefresh, conicpu_ctx.hEvent, THREAD_EVENT_AUTO_RESET | THREAD_EVENT_SMP);
		if(FAILED(rc))
			uart_force_printf("ScheduleThreadByEvent %d\r\n", rc);
		
		rc = MxStartTimer(conicpu_ctx.hTimer);
		if(FAILED(rc))
			uart_force_printf("MxStartTimer %d\r\n", rc);
	*/

	/*
	UINT32 ConICPURefresh(void *p)
	{
		UINT32 tsize;
		MXRC rc;
		char tmp[ICPU_QUEUE_BUFSIZE];
		CharICPUCtx *ctx = &conicpu_ctx;
	
		//uart_force_printf(".");
	
		if(ctx->sync)
		{
			MxScheduleThread(ctx->hRefresh);
			return 0; 
		}
	
		MxGetLock(&ctx->sync);
	
		tsize = CharICPUQueueGetFreeSpace(&ctx->IcpuMailbox->IcpuQueueToUarm);
	
		if(tsize)
		{
			uart_force_printf("ConICPURefresh : try to copy from internal queue to icpu queue\r\n");		
			if((rc = CharICPUQueueGetData(ctx->BufferQueue, (PUINT8)tmp, &tsize)) == ICPU_QUEUE_OK)
				if((rc = CharICPUQueuePutData(&ctx->IcpuMailbox->IcpuQueueToUarm, (PUINT8)tmp, tsize)) != ICPU_QUEUE_OK)
				{
					uart_force_printf("CharICPUPutData: internal queue overflow!!!!!!\r\n");
					MxReleaseLock(&ctx->sync);
					MxScheduleThread(ctx->hRefresh);
					return ICPU_QUEUE_ERROR;
				}		
		}
	
		MxReleaseLock(&ctx->sync);
	
		MxScheduleThread(ctx->hRefresh);
		
		return rc; 
	}*/


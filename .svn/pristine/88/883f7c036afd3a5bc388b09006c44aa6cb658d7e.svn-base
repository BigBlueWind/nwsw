

#ifndef __CONICPU_H__
#define __CONICPU_H__

#ifdef CINICPU_DEBUG
//	#define conicpu_uart_printf(...) uart_printf (__VA_ARGS__)
	#define conicpu_uart_printf uart_printf
#else
	#define conicpu_uart_printf(...)
#endif

#define ICPU_QUEUE_OK		0
#define ICPU_QUEUE_FULL		1
#define ICPU_QUEUE_EMPTY		2
#define ICPU_QUEUE_TOO_LARGE 3
#define ICPU_QUEUE_ERROR		0xFF

#define ICPU_QUEUE_BUFSIZE 10240

#define CICPU_TO_UARM_IRQ       (HAL_IRQ_INTER_ARM_2)
#define CICPU_FROM_UARM_IRQ     (HAL_IRQ_INTER_ARM_3)


typedef struct tagCharICPUQueue
{
	UINT8 *qstorage;
	UINT32  qsize;
	UINT32 qcharnum;
	UINT32 qputp;
	UINT32 qgetp;
	UINT32 sync;
}CharICPUQueue;

typedef struct tagCharICPUMailbox
{
	CharICPUQueue IcpuQueueToUarm;
	CharICPUQueue IcpuQueueFromUarm;
}CharICPUMailbox;

typedef struct tagCharICPUCtx
{
	CharICPUMailbox *IcpuMailbox;
	CharICPUQueue *BufferQueue;
	UINT32 		   toUARMIRQ;
	UINT32 		   fromUARMIRQ;
	HANDLE  	   hHandler;  
	UINT32		   inited;
	UINT32		   sync;
	HANDLE  	   hRefresh;
	HANDLE		   hEvent;
	HANDLE		   hTimer;
}CharICPUCtx;

typedef UINT32 (*CIPIHandler) (CharICPUCtx * ctx);

MXRC ConICPUInit(CIPIHandler hand);
UINT32 ConICPUIsInit(void);
MXRC ConICPURegHandler(CIPIHandler hand);
void ConICPUExit(void);
MXRC ConICPUPutCh(UINT8 ch, BOOL notify);
MXRC ConICPUGetCh(PUINT8 ch);
MXRC ConICPUPutData(PUINT8 data, UINT32 size, BOOL notify);
MXRC ConICPUNotify(void);
int cicpu_printf(const char *fmt, ...);



#endif


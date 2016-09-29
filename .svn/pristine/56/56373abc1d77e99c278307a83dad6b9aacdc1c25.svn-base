#include <string.h>
#include "config.h"
#include "4gmx.h"
#include "appids.h"
#include "apimgr.h"
#include "appprintf.h"
#include "heaps.h"
#include "network/network.h"
#include "network/sctp/sctp.h"
#include "sctp_client.h"
#include "supervisor.h"

typedef struct _sctp_client_ctx_t
{
	unsigned int sctp_associationID;
	unsigned int sctp_instanceID;
	HANDLE sctp_client_tread;
	HANDLE  SctpMsgPart;
	LPVOID  SctpMsgStorage;
	SCTP_RX_Proc_t sctp_rx_proc;
}sctp_client_ctx_t;

static sctp_client_ctx_t SCTP_client_ctx;

static sctp_client_ctx_t * SCTP_get_ctx(void)
{
	return &SCTP_client_ctx;
}

UINT32 SCTP_RX_Thread(LPVOID param)
{
	UINT32 length = MSG_MAXSIZE;	
	char *buffer;
	unsigned short streamSN;
	unsigned int tsn;
	sctp_client_ctx_t *pCtx;

	pCtx = SCTP_get_ctx();

	buffer = SCTP_Alloc_part();
	if(!buffer)
	{
		uart_printf("SCTP_RX_Thread: No memory!!!");
		return 0;
	}

	if (sctp_receive(pCtx->sctp_associationID, 0, (unsigned char *)buffer, &length, &streamSN, &tsn, SCTP_MSG_DEFAULT) != SCTP_SUCCESS)
	{
		uart_printf("SCTP_RX_Thread: Massage too big!!!");
		SCTP_Free_part(buffer);
		return 0;
	}

	if(pCtx->sctp_rx_proc)
		pCtx->sctp_rx_proc(buffer);

	SCTP_Free_part(buffer);

	return 0;
}


void SCTP_data_notif_cb(unsigned int assocID, unsigned short streamID, unsigned int len,
								unsigned short streamSN, unsigned int TSN, unsigned int protoID,
								unsigned int unordered, void* ulpDataPtr)
{
	sctp_client_ctx_t *pCtx;

	pCtx = SCTP_get_ctx();

	MxScheduleThread(pCtx->sctp_client_tread);
}

void * SCTP_Alloc_part(void)
{
	sctp_client_ctx_t *pCtx;

	pCtx = SCTP_get_ctx();

	return MxAllocBlock (pCtx->SctpMsgPart);
}


MXRC SCTP_Free_part(void *ptr)
{
	sctp_client_ctx_t *pCtx;

	pCtx = SCTP_get_ctx();

	return MxFreeBlock(pCtx->SctpMsgPart, ptr);
}

MXRC SCTP_client_init(char *hostaddr, int hostport, SCTP_RX_Proc_t proc)
{
	MXRC rc;
	SCTP_ulpCallbacks Ulp;
	unsigned char	localAddressList[1][SCTP_MAX_IP_LEN];
	unsigned char myaddr[4];
	char myaddr_str[20];
	sctp_client_ctx_t *pCtx;
	HANDLE h = AppHeap;

	//============MEMORY PART=========================

	int nElmNum = 10;
	int nBlockSize = 16024;

	pCtx = SCTP_get_ctx();
	memset(pCtx, 0, sizeof(SCTP_client_ctx));

	pCtx->sctp_rx_proc = proc;

	if (HANDLE_NOT_NULL(pCtx->SctpMsgPart))
	{
		MxShareObject (pCtx->SctpMsgPart, OBJ_SHARE_OFF);
	    MxRemovePart (pCtx->SctpMsgPart);
	    pCtx->SctpMsgPart = HNULL;
	}

	if (pCtx->SctpMsgStorage != NULL)
	{
		MxHeapFree (h, pCtx->SctpMsgStorage);
		pCtx->SctpMsgStorage = NULL;
	}

	pCtx->SctpMsgStorage = MxHeapAlloc (h, nElmNum * nBlockSize);

	if (pCtx->SctpMsgStorage == NULL)
	    return RC_SVSR_ALLOC_ERROR;

	rc = MxCreatePart (pCtx->SctpMsgStorage, nElmNum, nBlockSize, NULL, HNULL, HNULL, &(pCtx->SctpMsgPart));

	if (FAILED(rc))
	{
		pCtx->SctpMsgPart = HNULL;
	    MxHeapFree (h, pCtx->SctpMsgStorage);
	    pCtx->SctpMsgStorage = NULL;

	    return rc;
	}

	rc = MxShareObject (pCtx->SctpMsgPart, OBJ_SHARE_ON);

	if (FAILED(rc))
	{
	    MxRemovePart (pCtx->SctpMsgPart);
	    pCtx->SctpMsgPart = HNULL;

	    MxHeapFree (h, pCtx->SctpMsgStorage);
	    pCtx->SctpMsgStorage = NULL;

	    return rc;
	}

	//============NETWORK PART========================

	rc = net_init();
	rc = sctp_initLibrary();
				
	*(unsigned int*)&myaddr = (unsigned int)net_get_own_addr();
	
	sprintf(myaddr_str, "%u.%u.%u.%u", myaddr[0], myaddr[1], myaddr[2], myaddr[3]);
	memset(&Ulp, 0, sizeof(Ulp));
	Ulp.dataArriveNotif = SCTP_data_notif_cb;
	strcpy((char *)localAddressList[0], myaddr_str);
			
	rc = MxCreateThread(SCTP_RX_Thread, NULL, NULL, 0, &(pCtx->sctp_client_tread));
	if(FAILED(rc))
	   	uart_printf("SCTP_init_client: CreateThread error %d\r\n", rc);

	pCtx->sctp_instanceID = sctp_registerInstance(0,	MAXIMUM_NUMBER_OF_IN_STREAMS,
											MAXIMUM_NUMBER_OF_OUT_STREAMS,
											1, localAddressList, Ulp);
	
	pCtx->sctp_associationID = sctp_associate( pCtx->sctp_instanceID, MAXIMUM_NUMBER_OF_OUT_STREAMS,
														(unsigned char*)hostaddr, hostport, NULL );
	
	if(pCtx->sctp_associationID <= 0)
		return RC_NETWORK_UNKNOWN_ERROR;  //TODO make own error code
	


	return MX_OK;
}

MXRC SCTP_client_send(unsigned char* data, UINT32 size)
{
	int res;
	sctp_client_ctx_t *pCtx;

	pCtx = SCTP_get_ctx();
	
	res = sctp_send(pCtx->sctp_associationID, 0, data, size,
					SCTP_GENERIC_PAYLOAD_PROTOCOL_ID,									
					SCTP_USE_PRIMARY,													
					SCTP_NO_CONTEXT,													
					SCTP_INFINITE_LIFETIME,												
					SCTP_ORDERED_DELIVERY,												
					SCTP_BUNDLING_DISABLED );
	
	if(res != SCTP_SUCCESS)
		return RC_NETWORK_UNKNOWN_ERROR;  //TODO make own error code
	
	return MX_OK;
}


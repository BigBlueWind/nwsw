#ifndef __SCTP_CLIENT__
#define __SCTP_CLIENT__

#define MAXIMUM_NUMBER_OF_IN_STREAMS          1
#define MAXIMUM_NUMBER_OF_OUT_STREAMS         1

#define MSGS_HOST               0x0002      // Host
#define MSG_MAXSIZE             16384

typedef  UINT32 (*SCTP_RX_Proc_t)(void * msg);

void * SCTP_Alloc_part(void);
MXRC SCTP_Free_part(void *ptr);
MXRC SCTP_client_init(char *hostaddr, int hostport, SCTP_RX_Proc_t proc);
MXRC SCTP_client_send(unsigned char* data, UINT32 size);


#endif


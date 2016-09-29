
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef WIN32
#include "4gmx.h"
#include "network/network.h"
#include "network/sctp/sctp.h"
#else 
#include "sctp.h"


#endif
#include "fs.h"
#include "fsctp.h"

#ifdef WIN32
#include <windows.h>

HANDLE WorkThread;
DWORD dwThreadID;
CRITICAL_SECTION cs;

static int cs_inited = 0;

DWORD WINAPI ThreadProc(LPVOID lpParam) 
{
	while(1)
	{
		EnterCriticalSection(&cs);
		sctp_getEvents();
		LeaveCriticalSection(&cs);
	}
}

#endif


static int sctp_instanceID = -1;
static int sctp_assoc_id = -1;

unsigned short streamSN = 0; 
unsigned int tsn = 0; 


typedef struct fsctp_sync_t
{
	#ifndef WIN32
	VUINT32     	lock;
	VUINT32     	irq[CPU_NUM];
	#else
	int pad;
	#endif
}fsctp_sync_t;

static fsctp_sync_t fsctp_sync_ctx;

void fsctp_lock(fsctp_sync_t *ctx)
{
#ifndef WIN32
//	while(ctx->lock){MxDelay(10); 	
//	ctx->irq[MxGetCpuID ()] = ARM_INT_disable ();	
	MxGetLock (&ctx->lock);
//	ARM_INT_restore (ctx->irq[MxGetCpuID ()]);
#else
	
#endif
}

void fsctp_unlock (fsctp_sync_t *ctx)
{	
#ifndef WIN32
//	ctx->irq[MxGetCpuID ()] = ARM_INT_disable ();	
	MxReleaseLock (&ctx->lock);
//  ARM_INT_restore (ctx->irq[MxGetCpuID ()]);
#else
	
#endif
}

#define _sctpsend(ptr, size) sctp_send((int)sctp_assoc_id, 0, (unsigned char *)ptr, size, \
					SCTP_GENERIC_PAYLOAD_PROTOCOL_ID, \
					SCTP_USE_PRIMARY, \
					SCTP_NO_CONTEXT, \
					SCTP_INFINITE_LIFETIME, \
					SCTP_ORDERED_DELIVERY, \
					SCTP_BUNDLING_DISABLED )

#define _sctpreceive(ptr, size) 	sctp_receive((int) sctp_assoc_id, 0, (unsigned char *)ptr, (unsigned int *)size, &streamSN, &tsn, SCTP_MSG_DEFAULT)

#ifdef WIN32

int sctpsend(byte * ptr, size_t size)
{
	int res;
	//_sleep(1000);
	EnterCriticalSection(&cs);
	res = _sctpsend(ptr, size);
	LeaveCriticalSection(&cs);
	_sleep(1000);
	return res;
}

int sctpreceive(byte * ptr, size_t *size)
{
	int res;
	_sleep(1000);
	EnterCriticalSection(&cs);
	res = _sctpreceive(ptr, size);
	LeaveCriticalSection(&cs);
	//_sleep(1000);
	return res;
}

#else
#define sctpreceive _sctpreceive
#define sctpsend _sctpsend
#endif




file_deskt *sctp_open (const char * filename, int mode)
{
	const char *ptr = filename;
	const char *ip_start;
	const char *port_start;
	const char *fname_start;
	char ipstr[20];
	char portstr[20];
	int port;
	char fnamestr[256];
	file_deskt *fdesk;
	unsigned int rsize = 256;
	byte cmd[256];
	fsctp_command_ack *open_cmd_ask = (fsctp_command_ack *) cmd;
	fsctp_command_open *open_cmd = (fsctp_command_open *) cmd;

#ifdef WIN32

	if(!cs_inited)
	{
		InitializeCriticalSection(&cs);
		cs_inited = 1;
	}
#endif

	fsctp_lock(&fsctp_sync_ctx);

	ptr += FSCTP_TOC_LEN;

	ip_start = ptr;

	while(*ptr != ':' && *ptr != 0)
		ptr++;

	if(*ptr == 0)
	{
		fsctp_unlock(&fsctp_sync_ctx);
		uart_printf("No address\r\n");
		return NULL;
	}

	memset(ipstr, 0, sizeof(ipstr));
	_memcpy(ipstr, ip_start, ptr - ip_start);

	port_start = ++ptr;

	while(*ptr != '/' && *ptr != '\\')
		ptr++;

	memset(portstr, 0, sizeof(portstr));
	_memcpy(portstr, port_start, ptr - port_start);
	
	port = atoi(portstr);

	fname_start= ++ptr;
	
	while(*ptr != 0)
		ptr++;

	if(sctp_instanceID == -1)
	{
		byte myaddr[4];
		char myaddr_str[20];
		SCTP_ulpCallbacks Ulp;
		unsigned char	localAddressList[1][SCTP_MAX_IP_LEN];
#ifndef WIN32
		net_init();
#endif
		sctp_initLibrary();
#ifndef WIN32
		*(unsigned int*)&myaddr = (unsigned int)net_get_own_addr();
		sprintf(myaddr_str, "%u.%u.%u.%u", myaddr[0], myaddr[1], myaddr[2], myaddr[3]);
#else		
		sprintf(myaddr_str, "0.0.0.0");		 
#endif
		memset(&Ulp, 0, sizeof(Ulp));
		strcpy((char *)localAddressList[0], myaddr_str);

		sctp_instanceID = sctp_registerInstance(0, 1, 1, 1, localAddressList, Ulp);

		sctp_assoc_id = sctp_associate( sctp_instanceID, 1,	(unsigned char*)ipstr, port, NULL );

		#ifdef WIN32
		WorkThread = CreateThread(
            NULL,              // default security
            0,                 // default stack size
            ThreadProc,        // name of the thread function
            NULL,      // thread parameters
            0,                 // default startup flags
            &dwThreadID); 

			if (WorkThread == NULL) 
			{
				printf("CreateThread failed (%d)\n", GetLastError());
				return NULL;
			}
			_sleep(100);
		#endif

		if(sctp_assoc_id <= 0)
		{
			uart_printf("No connection\r\n");
			return NULL;		
		}
	}


	open_cmd->hdr.command_id = FSCTP_CMD_OPEN;
	open_cmd->fname_len = strlen(fnamestr);
	strcpy(open_cmd->fname, fname_start);	
	open_cmd->mode = mode;
	open_cmd->hdr.command_size = sizeof(fsctp_command_open) + open_cmd->fname_len;
	
	if(sctpsend(cmd, open_cmd->hdr.command_size) != SCTP_SUCCESS)
	{
		fsctp_unlock(&fsctp_sync_ctx);
		uart_printf("Error send command\r\n");
		return NULL;	
	}
	
	if(sctpreceive(cmd, &rsize) != SCTP_SUCCESS)	
	{
		fsctp_unlock(&fsctp_sync_ctx);
		uart_printf("Error receive command\r\n");
		return NULL;	
	}

	if(open_cmd_ask->ack_id != FSCTP_AKC_OK)
	{
		fsctp_unlock(&fsctp_sync_ctx);
		uart_printf("Command unsuccesfull\r\n");
		return NULL;
	}

	fdesk = malloc(sizeof(file_deskt));
	if(!fdesk)
	{
		fsctp_unlock(&fsctp_sync_ctx);
		uart_printf("Error allocate file descriptor\r\n");
		return NULL;
	}
	
	memset(fdesk, 0, sizeof(file_deskt));

	fdesk->flags |= FSCTP;
	fdesk->clusters = (size_t) open_cmd_ask->payload;

	fsctp_unlock(&fsctp_sync_ctx);

	return fdesk;
}

int sctp_close(file_deskt *fd)
{
	int rsize = 256;
	int ret = 0;
	byte cmd[256];
	fsctp_command_ack *close_cmd_ask = (fsctp_command_ack *) cmd;
	fsctp_command_close *close_cmd = (fsctp_command_close *) cmd;
	close_cmd->hdr.command_size = sizeof(fsctp_command_close); 

	if(!fd)
		return -1;

	fsctp_lock(&fsctp_sync_ctx);

	close_cmd->hdr.command_id = FSCTP_CMD_CLOSE;
	close_cmd->hdr.fd = (UINT16)fd->clusters;
	
	if(sctpsend(cmd, close_cmd->hdr.command_size) != SCTP_SUCCESS)
		ret = -1;	

	if(sctpreceive(cmd, &rsize) != SCTP_SUCCESS)	
		ret = -1;	
	
	if(close_cmd_ask->ack_id != FSCTP_AKC_OK)
		ret = -1;

	free(fd);

	fsctp_unlock(&fsctp_sync_ctx);

	return ret;
}

size_t sctp_read (void * ptr, size_t size,  file_deskt *fd)
{	
	int rsize = 256;
	byte cmd[256];
	fsctp_command_ack *read_cmd_ask = (fsctp_command_ack *) cmd;
	fsctp_command_read *read_cmd = (fsctp_command_read *) cmd;

	if(!ptr || !fd)
		return 0;

	fsctp_lock(&fsctp_sync_ctx);

	read_cmd->hdr.command_id = FSCTP_CMD_READ;
	read_cmd->hdr.fd = (UINT16)fd->clusters;
	read_cmd->hdr.command_size = sizeof(fsctp_command_read); 
	read_cmd->size = size;
	
	if(sctpsend(cmd, read_cmd->hdr.command_size) != SCTP_SUCCESS)
	{
		fsctp_unlock(&fsctp_sync_ctx);
		return 0;
	}

	if(sctpreceive(cmd, &rsize) != SCTP_SUCCESS)	
	{
		fsctp_unlock(&fsctp_sync_ctx);
		return 0;
	}

	if(read_cmd_ask->ack_id != FSCTP_AKC_OK)
	{
		fsctp_unlock(&fsctp_sync_ctx);
		return 0;
	}	

	if (sctpreceive(ptr, &size) != SCTP_SUCCESS)	
	{
		fsctp_unlock(&fsctp_sync_ctx);
		return 0;	
	}

	fsctp_unlock(&fsctp_sync_ctx);

	return size;
}

size_t sctp_write (const void * ptr, size_t size,  file_deskt *fd)
{
	
	int rsize = 256;
	byte cmd[256];
	fsctp_command_ack *write_cmd_ask = (fsctp_command_ack *) cmd;
	fsctp_command_write* write_cmd = (fsctp_command_write*) cmd;

	if(!ptr || !fd)
		return 0;

	fsctp_lock(&fsctp_sync_ctx);
	
	write_cmd->hdr.command_id = FSCTP_CMD_WRITE;
	write_cmd->hdr.fd = (UINT16)fd->clusters;
	write_cmd->hdr.command_size = sizeof(fsctp_command_write); 
	write_cmd->size = size;
	
	if(sctpsend(cmd, write_cmd->hdr.command_size) != SCTP_SUCCESS)
	{
		fsctp_unlock(&fsctp_sync_ctx);
		return 0;
	}

	if(sctpreceive(cmd, &rsize) != SCTP_SUCCESS)	
	{
		fsctp_unlock(&fsctp_sync_ctx);
		return 0;
	}
	
	if(write_cmd_ask->ack_id != FSCTP_AKC_OK)
	{
		fsctp_unlock(&fsctp_sync_ctx);
		return 0;
	}

	if(sctpsend(ptr, size) != SCTP_SUCCESS)
	{
		fsctp_unlock(&fsctp_sync_ctx);
		return 0;  //TODO make own error code
	}

	fsctp_unlock(&fsctp_sync_ctx);

	return size;
}

int sctp_seek (file_deskt *fd, long int off, int whence)
{
	int rsize = 256;
	byte cmd[256];
	fsctp_command_ack *seek_cmd_ask = (fsctp_command_ack *) cmd;
	fsctp_command_seek* seek_cmd = (fsctp_command_seek*) cmd;

	if(!fd)
		return -1;

	fsctp_lock(&fsctp_sync_ctx);
	
	seek_cmd->hdr.command_id = FSCTP_CMD_SEEK;
	seek_cmd->hdr.fd = (UINT16)fd->clusters;
	seek_cmd->offset = off;
	seek_cmd->whence = whence;
	seek_cmd->hdr.command_size = sizeof(fsctp_command_seek); 
	
	if(sctpsend(cmd, seek_cmd->hdr.command_size) != SCTP_SUCCESS)
	{
		fsctp_unlock(&fsctp_sync_ctx);
		return -1;
	}

	if(sctpreceive(cmd, &rsize) != SCTP_SUCCESS)	
	{
		fsctp_unlock(&fsctp_sync_ctx);
		return -1;
	}
	
	if(seek_cmd_ask->ack_id != FSCTP_AKC_OK)
	{
		fsctp_unlock(&fsctp_sync_ctx);
		return -1;
	}	

	fsctp_unlock(&fsctp_sync_ctx);	

	return 0;
}

long int sctp_tell (file_deskt *fd)
{
	int rsize = 256;
	byte cmd[256];
	fsctp_command_ack *tell_cmd_ask = (fsctp_command_ack *) cmd;
	fsctp_command_tell* tell_cmd = (fsctp_command_tell*) cmd;

	if(!fd)
		return -1;

	fsctp_lock(&fsctp_sync_ctx);
	
	tell_cmd->hdr.command_id = FSCTP_CMD_TELL;
	tell_cmd->hdr.fd = (UINT16)fd->clusters;
	tell_cmd->hdr.command_size = sizeof(fsctp_command_tell); 
	
	if(sctpsend(cmd, tell_cmd->hdr.command_size) != SCTP_SUCCESS)
	{
		fsctp_unlock(&fsctp_sync_ctx);
		return -1;
	}

	if(sctpreceive(cmd, &rsize) != SCTP_SUCCESS)	
	{
		fsctp_unlock(&fsctp_sync_ctx);
		return -1;
	}
	
	if(tell_cmd_ask->ack_id != FSCTP_AKC_OK)
	{
		fsctp_unlock(&fsctp_sync_ctx);
		return -1;
	}	

	fsctp_unlock(&fsctp_sync_ctx);	
	return (long int)tell_cmd_ask->payload;
}

int sctp_delete(file_deskt * fd)
{
	return 0;
}

void sctp_close_conn(void)
{

	if(sctp_instanceID != -1)
	{
		sctp_shutdown((int)sctp_assoc_id);
		sctp_deleteAssociation((int)sctp_assoc_id);		
		sctp_unregisterInstance((unsigned short) sctp_instanceID);
		//sctp_freeLibrary();
	}
}


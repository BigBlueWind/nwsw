#ifndef __FSCTP_H__
#define __FSCTP_H__

#ifdef __cplusplus
extern "C" {
#endif

#define FSCTP_CMD_OPEN		0
#define FSCTP_CMD_CLOSE		1
#define FSCTP_CMD_READ		2
#define FSCTP_CMD_WRITE		3
#define FSCTP_CMD_SEEK		4
#define FSCTP_CMD_TELL		5
#define FSCTP_CMD_DELETE	6

#define FSCTP_AKC_OK		0
#define FSCTP_AKC_ERR		1

#pragma pack(push)

#pragma pack(1)

typedef struct fsctp_command_header
{
	UINT8 	command_id;
	UINT16 	fd;
	UINT8 	command_size;			
}fsctp_command_header;

typedef struct fsctp_command_ack
{
	UINT8 	ack_id;
	UINT64 	payload;				
}fsctp_command_ack;

typedef struct fsctp_command_open
{
	fsctp_command_header hdr;
	UINT8  mode;
	UINT8  fname_len;
	char   fname[];
}fsctp_command_open;

typedef struct fsctp_command_close
{
	fsctp_command_header hdr;
}fsctp_command_close;


typedef struct fsctp_command_read
{
	struct fsctp_command_header hdr;
	UINT32 size;
}fsctp_command_read;

typedef struct fsctp_command_write
{
	struct fsctp_command_header hdr;
	UINT32 size;	
}fsctp_command_write;

typedef struct fsctp_command_seek
{
	struct fsctp_command_header hdr;
	UINT8  whence;
	UINT32 offset;
}fsctp_command_seek;

typedef struct fsctp_command_tell
{
	struct fsctp_command_header hdr;
}fsctp_command_tell;

typedef struct fsctp_command_delete
{
	struct fsctp_command_header hdr;
	UINT8  fname_len;
	char   fname[];
}fsctp_command_delete;


#pragma pack(pop)

file_deskt *sctp_open (const char * filename, int mode);
int sctp_close(file_deskt *fd);
size_t sctp_read (void * ptr, size_t size,  file_deskt *fd);
size_t sctp_write (const void * ptr, size_t size,  file_deskt *fd);
int sctp_seek (file_deskt *fd, long int off, int whence);
long int sctp_tell (file_deskt *fd);
int sctp_delete(file_deskt * fd);
void sctp_close_conn(void);


#ifdef __cplusplus
}
#endif

#endif


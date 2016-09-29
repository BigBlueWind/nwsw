#ifndef __FS_H__
#define __FS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define FFILES 32
#define FCLUSTER_NUM 4096
#define FCLUSTER_SIZE 4096
#define FILE_ID_START 100

#define FREAD	0x1
#define FWRITE	0x2
#define FAPPEND	0x4
#define FCREATE	0x8
#define FSCTP	0x10
#define FCONT	0x20

#define O_RDONLY	FREAD
#define O_WRONLY	FWRITE
#define O_RDWR		FREAD | FWRITE
#define O_APPEND	FAPPEND
#define O_CREAT		FCREATE

#define FCLUSTER_FREE	0
#define FCLUSTER_EOF	0xFFFFFFE5

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

#define FSCTP_TOC "sctp://"
#define FSCTP_TOC_LEN 7
#define FCONT_TOC "cont://"
#define FCONT_TOC_LEN 7

typedef long off_t;
typedef struct fclustert fclustert;

#pragma pack(push)

#pragma pack(1)

struct fclustert
{
	unsigned int next;
};

typedef struct _file_deskt
{
	char path[256];
	size_t size;
	size_t clusters;
	size_t cur_pos;
	int flags;
	fclustert *first_cluster;
	fclustert *last_cluster;
	fclustert *cur_cluster;
}file_deskt;

typedef struct _fst
{
	size_t fs_size;
	size_t fs_free_space;
	size_t fs_clusters;
	size_t fs_free_clusters;
	fclustert	  *free_cluster;
	file_deskt files[FFILES];	
	fclustert  clusters[];	
}fst;

#pragma pack(pop)

typedef struct _FILE
{	
	file_deskt *fdesk;
}__FILE;

#ifndef WIN32
#define finit 	_finit
#define ffree 	_ffree
#define fopen 	_fopen
#define fclose 	_fclose
#define fread 	_fread
#define fwrite	_fwrite
#define fseek	_fseek
#define ftell	_ftell
#define fdelete	_fdelete
#define rewind	_rewind
#define open	_open
#define close	_close
#define read	_read
#define write	_write
#define lseek	_lseek
#define FILE	__FILE
#else
typedef unsigned char UINT8;
typedef unsigned short int UINT16;
typedef unsigned int UINT32;
typedef unsigned __int64 UINT64;
typedef UINT8 byte;

#define _memcpy memcpy
#endif


int _finit(void *ptr, size_t size, int init_fs);

extern __FILE *_fopen (const char * __filename, const char * __modes);

extern int _fclose(__FILE * __stream);

extern size_t _fread (void * __ptr, size_t __size,  size_t __n, __FILE * __stream);

extern size_t _fwrite (const void * __ptr, size_t __size, size_t __n, __FILE * __s);

extern int _fseek (__FILE *__stream, long int __off, int __whence);

extern long int _ftell (__FILE *__stream);

extern void _rewind (__FILE *__stream);

extern int _fdelete(const char * __filename);

extern void * _fget_ptr(__FILE *__stream);

extern int _open (const char *__file, int __oflag, ...);

extern int _close (int __fd);

extern size_t _read (int __fd, void *__buf, size_t __nbytes);

extern size_t _write (int __fd, const void *__buf, size_t __n);

extern off_t _lseek (int __fd, off_t __offset, int __whence);

extern void * _get_ptr(int __fd);

extern void _ffree(void);

extern fst  *fs;


#ifdef __cplusplus
}
#endif

#endif

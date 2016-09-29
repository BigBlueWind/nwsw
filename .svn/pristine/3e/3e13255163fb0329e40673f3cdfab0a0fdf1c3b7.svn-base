#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#ifndef WIN32
#include "4gmx.h"
#endif
#include "fs.h"
#include "ffat.h"
#include "fcont.h"
#include "fsctp.h"

typedef unsigned char byte;

fst  *fs = NULL;

int _finit(void *ptr, size_t size, int init_fs)
{
	unsigned int clusters;
	unsigned int space_for_clasters;
	unsigned int unused_space;

	if(size <= sizeof(fst))
		return -1;

	fs = (fst *)ptr;

	if(init_fs)
	{
		memset(fs, 0, sizeof(fst));

		size -= sizeof(fst);

		clusters = size / FCLUSTER_SIZE;
		unused_space = size - clusters * FCLUSTER_SIZE;
		space_for_clasters = clusters * sizeof(fclustert);

		memset((byte *)fs + sizeof(fst), 0, space_for_clasters);

		if(space_for_clasters <= unused_space)
		{
			fs->fs_clusters = fs->fs_free_clusters = clusters;		 		
			fs->fs_free_space = fs->fs_size = clusters * FCLUSTER_SIZE;				
		}
		else
		{
			int needed_clusters = (space_for_clasters - unused_space) / FCLUSTER_SIZE;
			if((space_for_clasters - unused_space) % FCLUSTER_SIZE)
				needed_clusters++;
			
			clusters -= needed_clusters;
			space_for_clasters = clusters * sizeof(fclustert);
			fs->fs_clusters = fs->fs_free_clusters = clusters;		 		
			fs->fs_free_space = fs->fs_size = clusters * FCLUSTER_SIZE;				
		}
				
		fs->free_cluster = &fs->clusters[0];
	}

	return 0;
}



__FILE *_fopen (const char * __filename,	const char *__modes)
{
	file_deskt *fdesk = NULL;
	__FILE *file = NULL;
	int flags = 0;

	if(!fs)
	{
		uart_printf("finit() dose not called\r\n");
		return NULL; //finit() dose not called
	}
	if(strchr(__modes, 'w'))
		flags |= FWRITE;
	if(strchr(__modes, 'r'))
		flags |= FREAD;
	if(strchr(__modes, 'a'))
		flags |= FAPPEND;
	if(strchr(__modes, '+'))
		flags |= FCREATE;
	
	fdesk = fd_open(__filename, flags);
	if(fdesk)
	{
		file = malloc(sizeof(__FILE));	
		file->fdesk = fdesk;
	}
	return file;
}

int _fclose(__FILE * __stream)
{
	file_deskt *fdesk = NULL;

	if(!__stream)
		return -1;

	fdesk = __stream->fdesk;
	free(__stream);

	if(fd_close(fdesk) != 0)
		return -1;
	
	return 0;
}

size_t _fwrite (const void * __ptr, size_t __size, size_t __n, __FILE * __s)
{
	if(!__s)  // No FILE
		return 0;
	
	return (size_t)fd_write(__ptr, __size * __n, __s->fdesk);
}

size_t _fread (void *__ptr, size_t __size, size_t __n, __FILE * __stream)
{
	if(!__stream)  // No FILE
		return 0;
	
	return (size_t)fd_read(__ptr, __size * __n, __stream->fdesk);
}

int _fseek (__FILE *__stream, long int __off, int __whence)
{
	if(!__stream)
		return -1;

	return fd_seek(__stream->fdesk, __off, __whence);
}

long int _ftell (__FILE *__stream)
{
	if(!__stream)
		return -1;

	return fd_tell(__stream->fdesk);
}

int _fdelete(const char * __filename)
{
	if(!__filename)
		return -1;

	return fd_delete(__filename);
}

void _rewind (__FILE *__stream)
{
	_fseek(__stream, 0L, SEEK_SET);
}

int _open (const char *__file, int __oflag, ...)
{	
	file_deskt *fdesk;

	fdesk = fd_open(__file, __oflag);

	if(!fdesk)
		return -1;

	return FS_GET_FDESK_ID(fdesk) + FILE_ID_START;
}

int _close (int __fd)
{
	file_deskt *fdesk;

	fdesk = FS_GET_FDESK(__fd);

	if(!fdesk)
		return -1;

	return fd_close(fdesk);
}

size_t _read (int __fd, void *__buf, size_t __nbytes)
{
	file_deskt *fdesk;

	fdesk = FS_GET_FDESK(__fd);

	if(!fdesk)
		return 0;

	return fd_read(__buf, __nbytes, fdesk);
}

size_t _write (int __fd, const void *__buf, size_t __n)
{
	file_deskt *fdesk;

	fdesk = FS_GET_FDESK(__fd);

	if(!fdesk)
		return 0;

	return fd_write(__buf, __n, fdesk);
}

off_t _lseek (int __fd, off_t __offset, int __whence)
{
	file_deskt *fdesk;

	fdesk = FS_GET_FDESK(__fd);

	if(!fdesk)
		return -1;

	if(fd_seek(fdesk, __offset, __whence) != 0)
		return -1;

	return fd_tell(fdesk);
}

void _ffree(void)
{
	sctp_close_conn();
}


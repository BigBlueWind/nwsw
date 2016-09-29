#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#ifndef WIN32
#include "4gmx.h"
#include "heaps.h"
#include "config.h"
#endif
#include "fs.h"
#include "fcont.h"
#include "ffat.h"

file_deskt *cont_open (const char * filename, int mode, file_deskt * fd)
{
	const char *ptr = filename;
	const char *psize;
	const char *pname;
	char sizestr[20];
	char namestr[100];
	int size;
	file_deskt *fdesk;
	unsigned char *pdata = NULL;


	ptr += FCONT_TOC_LEN;
	
	pname= ptr;

	while(*ptr != ':' && *ptr != 0)
		ptr++;

	if(*ptr == 0)
		return NULL;

	memset(namestr, 0, sizeof(namestr));
	_memcpy(namestr, pname, ptr - pname);

	psize = ++ptr;

	while(*ptr != 0)
		ptr++;

	memset(sizestr, 0, sizeof(sizestr));
	_memcpy(sizestr, psize, ptr - psize);

	size = atoi(sizestr);
#ifndef WIN32
	pdata = MxHeapAlloc(FS_HEAP , size);
#else
	pdata = malloc(size);
#endif

	if(!fd)
		fdesk = malloc(sizeof(file_deskt));
	else
		fdesk = fd;

	memset(fdesk, 0, sizeof(file_deskt));

	fdesk->flags |= FCONT;

	fdesk->first_cluster = (fclustert *)pdata;
	fdesk->clusters = size;
	fdesk->cur_pos = fdesk->size = 0;
	strcpy(fdesk->path, filename);

	return fdesk;
}

int cont_close(file_deskt *fd)
{

	if(fd)
	{
		if(fd < &fs->files[0] || fd > &fs->files[FFILES])
			free(fd);
	}
	else
		return -1;

	return 0;
}

size_t cont_read (void * ptr, size_t size,  file_deskt *fd)
{
	size_t size_to_read = 0;

	if(!ptr || !fd || !fd->first_cluster)
	{
		return 0;
	}

	size_to_read = (fd->cur_pos + size <= fd->size) ? size : fd->size - fd->cur_pos;

	_memcpy(ptr, ((unsigned char *)(fd->first_cluster)) + fd->cur_pos, size_to_read);

	fd->cur_pos += size_to_read;

	return size_to_read;
}

size_t cont_write (const void * ptr, size_t size,  file_deskt *fd)
{
	size_t size_to_whrite = 0;

	if(!ptr || !fd || !fd->first_cluster)
	{
		return 0;
	}

	size_to_whrite = (fd->cur_pos + size <= fd->clusters) ? size : fd->clusters - fd->cur_pos;

	_memcpy(((unsigned char *)(fd->first_cluster)) + fd->cur_pos, ptr, size_to_whrite);

	fd->cur_pos += size_to_whrite;

	if(fd->cur_pos > fd->size)
		fd->size = fd->cur_pos;

	return size_to_whrite;
}

int cont_seek (file_deskt *fd, long int off, int whence)
{
	switch(whence)
	{
		case SEEK_SET:		
			if((size_t)off <= fd->clusters)
				fd->cur_pos = off;			
			else
				return -1;
		break;
		case SEEK_CUR:		
			if(fd->cur_pos + off <= fd->clusters)			
				fd->cur_pos += off;			
			else
				return -1;
		break;
		case SEEK_END:			
			if(fd->size + off <= fd->clusters)
				fd->cur_pos = fd->size + off;						 
			else
				return -1;			
		break;
		default:
			return -1;
	}	

	if(fd->cur_pos > fd->size)
		fd->size = fd->cur_pos;

	return 0;
}

long int cont_tell (file_deskt *fd)
{
	return fd->cur_pos;

}

int cont_delete(file_deskt * fd)
{
	
	if(fd && fd->first_cluster)
	{
#ifndef WIN32
		MxHeapFree(FS_HEAP, fd->first_cluster);
#else
		free(fd->first_cluster);
#endif
		free(fd);		
	}
	else
		return -1;

	return 0;
}

void * _fget_ptr(__FILE *__stream)
{
	if(!__stream|| !__stream->fdesk)  // No FILE
		return NULL;

	return __stream->fdesk->first_cluster;

}

void * _get_ptr(int __fd)
{

	file_deskt *fdesk = FS_GET_FDESK(__fd);

	if(fdesk)
		return fdesk->first_cluster;		

	return NULL;
}


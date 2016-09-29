#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#ifndef WIN32
#include "4gmx.h"
#include "config.h"
#endif
#include "fs.h"
#include "fcont.h"
#include "fsctp.h"
#include "ffat.h"


typedef unsigned char byte;

int _ffind_free_cluster(void)
{
	int free_custer_id = -1; 
	int cur_cluster = -1;
	int scaned_clusters = 0;
	

	if(!fs->fs_free_clusters)
		return -1;

	cur_cluster = FS_GET_CLUSTER_ID(fs->free_cluster);			
		
	while(free_custer_id == -1 && ((size_t)scaned_clusters < fs->fs_clusters))
	{
		if(fs->clusters[cur_cluster].next == FCLUSTER_FREE)
		{
			free_custer_id = cur_cluster;			
			fs->fs_free_clusters--;
			fs->clusters[cur_cluster].next = FCLUSTER_EOF;
		}

		scaned_clusters++;

		if((size_t)cur_cluster < fs->fs_clusters)
			cur_cluster++;
		else
			cur_cluster = 0;

	}

	if(free_custer_id == -1)
		return -1;

	fs->free_cluster = &fs->clusters[cur_cluster]; //hope that next cluster is free

	return free_custer_id;
}

int _ffree_cluster_chain(fclustert *first_cluster)
{
	fclustert *cluster;
	unsigned int next_cluster_id;

	cluster = first_cluster;

	while (cluster)
	{				
		next_cluster_id = cluster->next;
		cluster->next = FCLUSTER_FREE;

		if(next_cluster_id == FCLUSTER_EOF)
			cluster = NULL;
		else if(next_cluster_id == FCLUSTER_FREE)
			return -1;
		else
			cluster = &fs->clusters[next_cluster_id];	

		fs->fs_free_clusters++;
	}

	return 0;
}

file_deskt *fd_open (const char * filename, int mode)
{

	file_deskt *fdesk = NULL;
	int new_file = 1;
	int i;

	if(memcmp(filename, FSCTP_TOC, FSCTP_TOC_LEN) == 0)	
		return sctp_open(filename, mode);
	
	//find existing file or allocate new one

	for(i = 0; i < FFILES; i++)
	{
		if(strcmp(fs->files[i].path, filename) == 0)
		{
			fdesk = &fs->files[i];
			new_file = 0;
			break;
		}

		if((strcmp(fs->files[i].path, "") == 0) && !fdesk)
		{
			fdesk = &fs->files[i];
		}
	}

	if(!fdesk)
		return NULL; //no free files record (temporary limitation)

	if(new_file && !(mode & FCREATE)) //caller not set to create file
		return NULL;

	if(!new_file)
	{
		if(fdesk->flags & FCONT)
			mode |= FCONT;

		if(mode & FAPPEND)
		{			
			if(!(fdesk->flags & FCONT))
				fdesk->cur_cluster = fdesk->last_cluster;
			fdesk->cur_pos = fdesk->size;			
		}
		else if(mode & FWRITE)
		{
			if(!(fdesk->flags & FCONT))
			{
				_ffree_cluster_chain(fdesk->first_cluster);
				fs->fs_free_clusters--;
				fdesk->cur_cluster = fdesk->last_cluster = fdesk->first_cluster;
				fdesk->cur_cluster->next = FCLUSTER_EOF;			
				fdesk->clusters = 1;
			}
			fdesk->size = 0;	
			fdesk->cur_pos = 0;
		}
		else if(mode & FREAD)
		{
			if(!(fdesk->flags & FCONT))
			{
				fdesk->cur_cluster = fdesk->first_cluster;
			}
			fdesk->cur_pos = 0;
		}
	}
	else
	{

		if(memcmp(filename, FCONT_TOC, FCONT_TOC_LEN) == 0)	
		{
			fdesk = cont_open(filename, mode, fdesk);	
			return fdesk;
		}
		else
		{
			fdesk->cur_pos = 0;
			fdesk->size = 0;
			fdesk->clusters = 0;
			fdesk->cur_cluster = fdesk->first_cluster = fdesk->last_cluster = NULL;
		}
		strcpy(fdesk->path, filename);		
	}

	fdesk->flags = mode;

	return fdesk;
}

int fd_close(file_deskt *fd)
{
	if(!fd)
		return -1;

	if(fd->flags & FSCTP)
		return sctp_close(fd);
	else if(fd->flags & FCONT)
		return cont_close(fd);


	return 0;
}

size_t fd_write (const void * ptr, size_t size,  file_deskt *fd)
{
	file_deskt *fdesk = fd;
	size_t writing_bytes = 0;
	size_t left_to_copy = size;
	size_t to_copy = 0;
	int clusters = 0;	
	int new_cluster = -1;
	byte *copy_ptr = (byte *)ptr;

	if(!fdesk || !ptr)
		return 0;

	if(fd->flags & FSCTP)
		return sctp_write(ptr, size, fd);	
	else if(fd->flags & FCONT)
		return cont_write(ptr, size, fd);

	if(size > fs->fs_free_space)  // No space
		return 0;

	if(!fdesk->flags & FWRITE && !fdesk->flags & FAPPEND && !fdesk->flags & FCREATE)
		return 0;

	if(!fdesk->cur_cluster)
	{		
		clusters = size / FCLUSTER_SIZE;
		if(size % FCLUSTER_SIZE)
			clusters++;

		//first time write
		if((new_cluster = _ffind_free_cluster()) != -1)
		{
			fdesk->cur_cluster = fdesk->first_cluster = fdesk->last_cluster = FS_GET_CLUSTER(new_cluster);	
			fdesk->clusters++;
		}
		else
			return 0;

		clusters--;
				
		if(left_to_copy >= FCLUSTER_SIZE)
			to_copy = FCLUSTER_SIZE;
		else
			to_copy = left_to_copy;

		_memcpy(FS_GET_CLUSTER_PTR(fdesk->cur_cluster), copy_ptr, (unsigned int)to_copy);

		fs->fs_free_space -= to_copy;
		writing_bytes += to_copy;
		left_to_copy -= to_copy;

		fdesk->cur_pos += to_copy;	

		if(fdesk->cur_pos > fdesk->size)
			fdesk->size = fdesk->cur_pos;

		copy_ptr += to_copy;

		while(clusters--)
		{
			if((new_cluster = _ffind_free_cluster()) != -1)
			{
				fdesk->last_cluster = FS_GET_CLUSTER(new_cluster);
				fdesk->cur_cluster->next = new_cluster;
				fdesk->cur_cluster = fdesk->last_cluster;				
				fdesk->clusters++;

				if(left_to_copy >= FCLUSTER_SIZE)
					to_copy = FCLUSTER_SIZE;
				else
					to_copy = left_to_copy;

				_memcpy(FS_GET_CLUSTER_PTR(fdesk->cur_cluster), copy_ptr, (unsigned int)to_copy);

				fs->fs_free_space -= to_copy;
				writing_bytes += to_copy;
				left_to_copy -= to_copy;

				fdesk->cur_pos += to_copy;
				if(fdesk->cur_pos > fdesk->size)
					fdesk->size = fdesk->cur_pos;
				
				copy_ptr += to_copy;
			}
			else
			{
				return (size_t)writing_bytes;
			}
		}
	}
	else
	{
		unsigned int free_in_cur_cluster = 0;
		unsigned int cur_cluster_pos = 0;

		cur_cluster_pos = (fdesk->cur_pos % FCLUSTER_SIZE);
		free_in_cur_cluster = FCLUSTER_SIZE - cur_cluster_pos;

		if(free_in_cur_cluster >= left_to_copy)
		{
			_memcpy(FS_GET_CLUSTER_PTR(fdesk->cur_cluster) + cur_cluster_pos, copy_ptr, (unsigned int)left_to_copy);
			writing_bytes += left_to_copy;

			fdesk->cur_pos += left_to_copy;
			
			if(fdesk->cur_pos > fdesk->size)
				fdesk->size = fdesk->cur_pos;

			return (size_t)writing_bytes;
		}

		clusters = (size - free_in_cur_cluster) / FCLUSTER_SIZE;
		if((size - free_in_cur_cluster) % FCLUSTER_SIZE)
			clusters++;

		if(free_in_cur_cluster)
		{
			_memcpy(FS_GET_CLUSTER_PTR(fdesk->cur_cluster) + cur_cluster_pos, copy_ptr, free_in_cur_cluster);
			writing_bytes += free_in_cur_cluster;
			left_to_copy -= free_in_cur_cluster;

			fdesk->cur_pos += free_in_cur_cluster;
			if(fdesk->cur_pos > fdesk->size)
				fdesk->size = fdesk->cur_pos;

			copy_ptr += free_in_cur_cluster;
		}

		while(clusters--)
		{
			if(fdesk->cur_cluster->next != FCLUSTER_EOF)
			{
				fdesk->cur_cluster = FS_GET_CLUSTER(fdesk->cur_cluster->next);

				if(left_to_copy >= FCLUSTER_SIZE)
					to_copy = FCLUSTER_SIZE;
				else
					to_copy = left_to_copy;

				_memcpy(FS_GET_CLUSTER_PTR(fdesk->cur_cluster), copy_ptr, (unsigned int)to_copy);
				writing_bytes += to_copy;
				left_to_copy -= to_copy;				

				fdesk->cur_pos += to_copy;

				if(fdesk->cur_pos > fdesk->size)
					fdesk->size = fdesk->cur_pos;

				copy_ptr += to_copy;
			}
			else if((new_cluster = _ffind_free_cluster()) != -1)
			{				
				fdesk->last_cluster = FS_GET_CLUSTER(new_cluster);

				fdesk->cur_cluster->next = new_cluster;
				fdesk->cur_cluster = fdesk->last_cluster;				
				fdesk->clusters++;								

				if(left_to_copy >= FCLUSTER_SIZE)
					to_copy = FCLUSTER_SIZE;
				else
					to_copy = left_to_copy;
			
				_memcpy(FS_GET_CLUSTER_PTR(fdesk->cur_cluster), copy_ptr, (unsigned int)to_copy);
			
				fs->fs_free_space -= to_copy;
				writing_bytes += to_copy;
				left_to_copy -= to_copy;				

				fdesk->cur_pos += to_copy;
				
				if(fdesk->cur_pos > fdesk->size)
					fdesk->size = fdesk->cur_pos;

				copy_ptr += to_copy;
			}
			else
			{
				return (size_t)writing_bytes;
			}
		}
	}

	return (size_t)writing_bytes;
}

size_t fd_read (void * ptr, size_t size,  file_deskt *fd)
{
	file_deskt *fdesk = fd;
	size_t reading_bytes = 0;
	size_t left_to_copy = size;
	size_t to_copy = 0;
	int clusters = 0;	
	byte *copy_ptr = (byte *)ptr;
	byte *cur_data_ptr = NULL;
	size_t left_in_cur_cluster;
	size_t left_unread_data;	

	if(!fdesk || !ptr)
		return 0;

	if(!fdesk->flags & FREAD && !fdesk->flags & FCREATE)
		return 0;

	if(fd->flags & FSCTP)
		return sctp_read(ptr, size, fd);
	else if(fd->flags & FCONT)
		return cont_read(ptr, size, fd);

	left_in_cur_cluster = FCLUSTER_SIZE - fdesk->cur_pos % FCLUSTER_SIZE;
	left_unread_data = fdesk->size - fdesk->cur_pos;

	if(left_to_copy > left_unread_data)
		left_to_copy = left_unread_data;
	
	clusters = (size) / FCLUSTER_SIZE;
	if(size % FCLUSTER_SIZE)
		clusters++;

	if(left_to_copy > left_in_cur_cluster)	
	{
		clusters++;
		to_copy = left_in_cur_cluster;
	}
	else
		to_copy = left_to_copy;
	
	cur_data_ptr = FS_GET_CLUSTER_PTR(fdesk->cur_cluster) + (fdesk->cur_pos % FCLUSTER_SIZE);

	while(clusters--)
	{
		_memcpy(copy_ptr + reading_bytes, cur_data_ptr, (unsigned int)to_copy);

		reading_bytes += to_copy;
		left_to_copy -= to_copy;
		fdesk->cur_pos += to_copy;

		if(clusters)
		{
			if(fdesk->cur_cluster->next == FCLUSTER_EOF || !left_to_copy)
				return (size_t) reading_bytes;

			if(left_to_copy >= FCLUSTER_SIZE)
				to_copy = FCLUSTER_SIZE;
			else
				to_copy = left_to_copy;

			fdesk->cur_cluster = FS_GET_CLUSTER(fdesk->cur_cluster->next);
			cur_data_ptr = FS_GET_CLUSTER_PTR(fdesk->cur_cluster);

		}
	}

	return (size_t)reading_bytes;
}

int fd_seek (file_deskt *fd, long int off, int whence)
{
	file_deskt *fdesk = fd;
	file_deskt old_fdesk;
	int clusters;

	if(!fdesk)
		return -1;

	if(fd->flags & FSCTP)
		return sctp_seek(fd, off, whence);	
	else if(fd->flags & FCONT)
		return cont_seek(fd, off, whence);

	memcpy(&old_fdesk, fdesk, sizeof(file_deskt));

	switch(whence)
	{
		case SEEK_SET:		
			fdesk->cur_pos = off;			
		break;
		case SEEK_CUR:		
			fdesk->cur_pos += off;		
		break;
		case SEEK_END:
			fdesk->cur_pos = fdesk->size + off;		
		break;
		default:
			return -1;
	}	

	if(fdesk->cur_pos > fdesk->size)
	{
		if(fdesk->cur_pos > fdesk->clusters * FCLUSTER_SIZE)
		{
			int need_space = fdesk->cur_pos - fdesk->size - (fdesk->clusters * FCLUSTER_SIZE - fdesk->size); 
			int need_clusters = need_space / FCLUSTER_SIZE;
			if(need_space % FCLUSTER_SIZE)
				need_clusters++;

			while(need_clusters--)
			{
				int new_cluster;

				if((new_cluster = _ffind_free_cluster()) != -1)
				{
					fdesk->last_cluster->next = new_cluster;
					fdesk->last_cluster = FS_GET_CLUSTER(new_cluster);
				}
				else 
				{
					memcpy(fdesk, &old_fdesk, sizeof(file_deskt));
					_ffree_cluster_chain(fdesk->last_cluster);
					fs->free_cluster--;
					fdesk->last_cluster->next = FCLUSTER_EOF;

					return -1;
				}
			}
		}

		fdesk->size = fdesk->cur_pos;
	}

	fdesk->cur_cluster = fdesk->first_cluster;

	clusters = fdesk->cur_pos / FCLUSTER_SIZE;

	while(clusters--)
		fdesk->cur_cluster = FS_GET_CLUSTER(fdesk->cur_cluster->next);	

	return 0;
}

long int fd_tell (file_deskt *fd)
{
	if(!fd)
		return -1;

	if(fd->flags & FSCTP)
		return sctp_tell(fd);	
	else if(fd->flags & FCONT)
		return cont_tell(fd);

	return fd->cur_pos;
}

int fd_delete(const char * __filename)
{
	file_deskt *fdesk = NULL;

	int i;


	//find existing file or allocate new one

	for(i = 0; i < FFILES; i++)
	{
		if(strcmp(fs->files[i].path, __filename) == 0)
		{
			fdesk = &fs->files[i];
			break;
		}
	}

	if(!fdesk)
		return -1;

	
	if(fdesk->flags & FSCTP)
		return 0;	
	else if(fdesk->flags & FCONT)
		return cont_delete(fdesk);

	if(_ffree_cluster_chain(fdesk->first_cluster) == -1)
		return -1;

	fs->fs_free_space += fdesk->size;
	fs->free_cluster = fdesk->first_cluster;
	fdesk->clusters = 0;
	fdesk->size = 0;
	strcpy(fdesk->path,"");

	return 0;
}


#ifndef __FCONT_H__
#define __FCONT_H__

file_deskt *cont_open (const char * filename, int mode, file_deskt * fd);
int cont_close(file_deskt *fd);
size_t cont_read (void * ptr, size_t size,  file_deskt *fd);
size_t cont_write (const void * ptr, size_t size,  file_deskt *fd);
int cont_seek (file_deskt *fd, long int off, int whence);
long int cont_tell (file_deskt *fd);
int cont_delete(file_deskt * fd);

#define FS_HEAP 	DDR1Heap

#endif


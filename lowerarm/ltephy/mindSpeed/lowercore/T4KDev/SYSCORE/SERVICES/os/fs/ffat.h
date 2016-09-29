#ifndef __FFAT_H__
#define __FFAT_H__

#define FS_GET_STORAGE_PTR ((byte *)fs + sizeof(fst) + fs->fs_clusters * sizeof(fclustert))
#define FS_GET_CLUSTER_ID(cluster) (((byte *)cluster - (byte *)&fs->clusters[0]) / sizeof(fclustert) )
#define FS_GET_CLUSTER(id) (&(fs->clusters[id]))
#define FS_GET_CLUSTER_PTR(cluster) (FS_GET_STORAGE_PTR + ((FS_GET_CLUSTER_ID(cluster) /*+ 1*/) * FCLUSTER_SIZE))
#define FS_GET_CUR_CLUSTER_POS(fdesk) (fdesk->size % (fdesk->clysters * FCLUSTER_SIZE))
#define FS_GET_FDESK_CUR_PTR(fdesk) (fdesk->clusters * FCLUSTER_SIZE + FS_GET_CUR_CLUSTER_POS(fdesk))
#define FS_GET_FDESK_ID(fdesk) (((byte *)fdesk - (byte *)&fs->files[0]) / sizeof(file_deskt) )
#define FS_GET_FDESK(fd) &(fs->files[fd - FILE_ID_START])

int _ffree_cluster_chain(fclustert *first_cluster);
int _ffind_free_cluster(void);

file_deskt *fd_open (const char * filename, int mode);
int fd_close(file_deskt *fd);
size_t fd_read (void * ptr, size_t size,  file_deskt *fd);
size_t fd_write (const void * ptr, size_t size,  file_deskt *fd);
int fd_seek (file_deskt *fd, long int off, int whence);
long int fd_tell (file_deskt *fd);
int fd_delete(const char * __filename);

#endif


#ifndef __NET_MEM_H__
#define __NET_MEM_H__

//os-primitive functuins
#ifdef NET_DEBUG
void net_dump_data(const byte *data, const int  size);
#else

#define net_dump_data(data, size)

#endif
void * net_alloc(size_t size);
void net_free(void *ptr);

#endif

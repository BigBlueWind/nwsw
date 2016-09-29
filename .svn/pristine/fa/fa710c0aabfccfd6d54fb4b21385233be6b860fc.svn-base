/* 
 */

#ifndef CYCLE_BUFFER_H
#define	CYCLE_BUFFER_H

#ifdef	__cplusplus
extern "C" {
#endif

#define byte unsigned char

typedef struct _cyclic_buffer_t
{
    void            *buffer_start;  // data buffer
    void            *buffer_end;    // end of data buffer
    unsigned short  free_buff_size; // maximum number of items in the buffer
    unsigned short  item_count;     // number of items in the buffer
    unsigned short  item_size;      // size of each item in the buffer
    void            *head;          // pointer to head
    void            *tail;          // pointer to tail
    unsigned short  array_size;		// size of array
} cyclic_buffer_t;

cyclic_buffer_t *cyclic_buffer_init(void *buf, size_t size);
void cyclic_buffer_free(cyclic_buffer_t *CB);
void cyclic_buffer_clear(cyclic_buffer_t *CB);
int cyclic_buffer_put(cyclic_buffer_t *CB, unsigned char *data, size_t size);
int cyclic_buffer_get(cyclic_buffer_t *CB, unsigned char *data, size_t *size);
int cyclic_buffer_peek(cyclic_buffer_t *CB, unsigned char *data, size_t *size);
int  cyclic_buffer_test(void);

#ifdef	__cplusplus
}
#endif

#endif	/* CYCLE_BUFFER_H */


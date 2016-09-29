//*****************************************************************************

//*****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "4gmx.h"
#include "appids.h"
#include "apimgr.h"
#include "appprintf.h"
#include "heaps.h"
#include "cycle_buffer.h"

/**
 *  Code for working with cyclic buffer
 *
 *  @param  string  pointer to the string contains params for Iperf
 *
 *  @return none    count of arguments in stirng
 *
 *  @brief
 */
cyclic_buffer_t *cyclic_buffer_init(void *buf, size_t size)
{
	cyclic_buffer_t *CB;

	CB = (cyclic_buffer_t *) malloc(sizeof(cyclic_buffer_t));

	if(!CB)
		return NULL;
	
    CB->buffer_start = buf;
    CB->buffer_end = (char*)buf + size;
    CB->free_buff_size = size;
    CB->item_count = 0;
    CB->head = CB->buffer_start;
    CB->tail = CB->buffer_start;
    CB->array_size = size;

	memset(CB->buffer_start, 0x0, CB->array_size);

    return CB;
}

void cyclic_buffer_free(cyclic_buffer_t *buf)
{
	free(buf);
}
/**
 *  ...
 *
 *  @param  buff_len     ...
 *  
 *  @return         ...
 *
 *  @brief          ...
 *
 */ 
void cyclic_buffer_clear(cyclic_buffer_t *CB)
{
    memset(CB->buffer_start, 0x0, CB->array_size);
	CB->head = CB->buffer_start;
    CB->tail = CB->buffer_start;
	CB->item_count = 0;
	CB->free_buff_size = CB->array_size;
}

/**
 *  ...
 *
 *  @param  buff_len     ...
 *  
 *  @return         ...
 *
 *  @brief          ...
 *
 */ 
int cyclic_buffer_put(cyclic_buffer_t *CB, byte *data, size_t size)
{
	//unsigned int  tempSize = 0;
	unsigned int needToCopy = 0;	

    if(CB->free_buff_size < (size + sizeof(UINT32)))
        // free space is less than we need
    {
      // erase previous message for example
      return -1;
    }
    
    switch ((unsigned int)((char*)CB->buffer_end - (char*)CB->head))
    {
    	case 1:
    		*((byte*)CB->head) = *((byte *)&size);
    		CB->head = CB->buffer_start;
    		*((byte*)CB->head) = *((byte *)&size + 1);
    		*((byte*)CB->head + 1) = *((byte *)&size + 2);
    		*((byte*)CB->head + 2) = *((byte *)&size + 3);
    		CB->head = (byte*)CB->head + 3;
    		break;
    	case 2:
    		*((byte*)CB->head) = *((byte *)&size);
    		*((byte*)CB->head + 1) = *((byte *)&size + 1);
    		CB->head = CB->buffer_start;
    		*((byte*)CB->head) = *((byte *)&size + 2);
    		*((byte*)CB->head + 1) = *((byte *)&size + 3);
    		CB->head = (byte*)CB->head + 2;
    		break;
    	case 3:    	
    		*((byte*)CB->head) = *((byte *)&size );
    		*((byte*)CB->head + 1) = *((byte *)&size + 1);
    		*((byte*)CB->head + 2) = *((byte *)&size + 2);
    		CB->head = CB->buffer_start;
    		*((byte*)CB->head) = *((byte *)&size + 3);
    		CB->head = (byte*)CB->head + 1;
    		break;
    	default:
    		*((UINT32 *)CB->head) = size;
    		CB->head = (byte*)CB->head + sizeof(UINT32);
    		break;
    }     
    
    
    if ((unsigned int)((char*)CB->buffer_end - (char*)CB->head) < size)
    {
    	// now we should find payload len correctly
    	_memcpy((char*)CB->head, data,  (unsigned int)((char*)CB->buffer_end - (char*)CB->head));
    	needToCopy = (size) - (unsigned int)((char*)CB->buffer_end - (char*)CB->head);
    	_memcpy((char*)CB->buffer_start,  data + (size - needToCopy), needToCopy);
    	CB->head = (char*)CB->buffer_start + needToCopy;
    }
    else
    {
    	_memcpy((char*)CB->head, data, size);
    	CB->head = (char*)CB->head + size;
    }
    /*
	while (tempSize < size)
	{
	    if(CB->head == CB->buffer_end)
		    CB->head = CB->buffer_start;

	    *((byte*)CB->head) = data[tempSize];
		//((byte*)CB->head)[0] = data[tempSize];

		tempSize++;
		CB->head = (byte*)CB->head + 1;
	}
    */
	CB->free_buff_size -= (size + sizeof(UINT32));

    // verigy head pointer in case we achieve end of buffer
    if(CB->head == CB->buffer_end)
        CB->head = CB->buffer_start;

    // increment count of events inside buffer
    CB->item_count++;

    return 0;
}

/**
 *  ...
 *
 *  @param  buff_len     ...
 *  
 *  @return         ...
 *
 *  @brief          ...
 *
 */ 
int cyclic_buffer_get(cyclic_buffer_t *CB, byte *data, size_t *size)
{	
	unsigned int needToRead = 0;

    if(CB->item_count == 0)
    {
          return -1;
    }

	// now we should find payload size correctly
	switch ((unsigned int)((char*)CB->buffer_end - (char*)CB->tail))
	{
		case 1:
			    *((byte *)size) = *((byte*)CB->tail);
	    		CB->tail = CB->buffer_start;
	    		*((byte *)size + 1) = *((byte*)CB->tail);
	    		*((byte *)size + 2) = *((byte*)CB->tail + 1);
	    		*((byte *)size + 3) = *((byte*)CB->tail + 2);
	    		CB->tail = (byte*)CB->tail + 3;
	    		break;
	    case 2:
	    		*((byte *)size) = *((byte*)CB->tail);
	    		*((byte *)size + 1) = *((byte*)CB->tail + 1);
	    		CB->head = CB->buffer_start;
	    		*((byte *)size + 2) = *((byte*)CB->tail);
	    		*((byte *)size + 2) = *((byte*)CB->tail + 1);
	    		CB->tail = (byte*)CB->tail + 2;
	    		break;
	    case 3:    	
	    		*((byte *)size) = *((byte*)CB->tail);
	    		*((byte *)size + 1) = *((byte*)CB->tail + 1);
	    		*((byte *)size + 2) = *((byte*)CB->tail + 2);
	    		CB->head = CB->buffer_start;
	    		*((byte *)size + 3) = *((byte*)CB->tail);
	    		CB->tail = (byte*)CB->tail + 1;
	    		break;
	    default:
	    		*size = *((UINT32 *)CB->tail);
	    		CB->tail = (byte*)CB->tail + sizeof(UINT32);
	    		break;			
	}

	// check if the event payload fits till end of buffer
	// or already written partialy to the beginning also
	if ((unsigned int)((char*)CB->buffer_end - (char*)CB->tail) < *size)
	{
		// now we should find payload len correctly
		_memcpy(data, (char*)CB->tail, (unsigned int)((char*)CB->buffer_end - (char*)CB->tail));
		needToRead = (*size) - (unsigned int)((char*)CB->buffer_end - (char*)CB->tail);
		_memcpy(data + (*size - needToRead), (char*)CB->buffer_start, needToRead);
		CB->tail = (char*)CB->buffer_start + needToRead;
	}
	else
	{
		_memcpy(data, (char*)CB->tail, *size);
		CB->tail = (char*)CB->tail + *size;
	}
	
	// add event payload len itself
	CB->free_buff_size += *size + sizeof(UINT32);

    if(CB->tail == CB->buffer_end)
        CB->tail = CB->buffer_start;

	CB->item_count--;

	return 0;
}

/**
 *  ...
 *
 *  @param  buff_len     ...
 *
 *  @return         ...
 *
 *  @brief          ...
 *
 */
int cyclic_buffer_peek(cyclic_buffer_t *CB, byte *data, size_t *size)
{
	unsigned int needToRead = 0;
	void *old_tail = CB->tail;


    if(CB->item_count == 0)
    {
          return -1;
    }

	// now we should find payload size correctly
	switch ((unsigned int)((char*)CB->buffer_end - (char*)CB->tail))
	{
		case 1:
			    *((byte *)size) = *((byte*)CB->tail);
	    		CB->tail = CB->buffer_start;
	    		*((byte *)size + 1) = *((byte*)CB->tail);
	    		*((byte *)size + 2) = *((byte*)CB->tail + 1);
	    		*((byte *)size + 3) = *((byte*)CB->tail + 2);
	    		CB->tail = (byte*)CB->tail + 3;
	    		break;
	    case 2:
	    		*((byte *)size) = *((byte*)CB->tail);
	    		*((byte *)size + 1) = *((byte*)CB->tail + 1);
	    		CB->head = CB->buffer_start;
	    		*((byte *)size + 2) = *((byte*)CB->tail);
	    		*((byte *)size + 2) = *((byte*)CB->tail + 1);
	    		CB->tail = (byte*)CB->tail + 2;
	    		break;
	    case 3:
	    		*((byte *)size) = *((byte*)CB->tail);
	    		*((byte *)size + 1) = *((byte*)CB->tail + 1);
	    		*((byte *)size + 2) = *((byte*)CB->tail + 2);
	    		CB->head = CB->buffer_start;
	    		*((byte *)size + 3) = *((byte*)CB->tail);
	    		CB->tail = (byte*)CB->tail + 1;
	    		break;
	    default:
	    		*size = *((UINT32 *)CB->tail);
	    		CB->tail = (byte*)CB->tail + sizeof(UINT32);
	    		break;
	}

	// check if the event payload fits till end of buffer
	// or already written partialy to the beginning also
	if ((unsigned int)((char*)CB->buffer_end - (char*)CB->tail) < *size)
	{
		// now we should find payload len correctly
		_memcpy(data, (char*)CB->tail, (unsigned int)((char*)CB->buffer_end - (char*)CB->tail));
		needToRead = (*size) - (unsigned int)((char*)CB->buffer_end - (char*)CB->tail);
		_memcpy(data + (*size - needToRead), (char*)CB->buffer_start, needToRead);
		CB->tail = (char*)CB->buffer_start + needToRead;
	}
	else
	{
		_memcpy(data, (char*)CB->tail, *size);
		CB->tail = (char*)CB->tail + *size;
	}

	CB->tail = old_tail;

	return 0;
}



int  cyclic_buffer_test(void)
{
    void * ptr;
	cyclic_buffer_t *CB;

	unsigned char tstb1[] = {1,2,3,4};
	unsigned char tstb2[] = {5,6,7,8,9};
	unsigned char tstb3[] = {10,11,12,13,14,15};

	unsigned char tstb[20];

	size_t tstb1_size = sizeof(tstb1);
	size_t tstb2_size = sizeof(tstb2);
	size_t tstb3_size = sizeof(tstb3);

	size_t tstb_size = 0;

	int i;

	ptr = malloc(1024);

	if(!ptr)
		return -1;

	CB = cyclic_buffer_init(ptr, 1024);

	if(!CB)
			return -1;

	uart_printf("test1 started ... ");

	cyclic_buffer_put(CB, tstb1, tstb1_size);

	cyclic_buffer_get(CB, tstb, &tstb_size);

	if(memcmp(tstb, tstb1, tstb_size) != 0)
	{

		uart_printf("test1 failed \r\n");
		return -1;
	}
	uart_printf("OK\r\n");

	cyclic_buffer_clear(CB);

	uart_printf("test2 started ... ");

	cyclic_buffer_put(CB, tstb1, tstb1_size);
	cyclic_buffer_put(CB, tstb2, tstb2_size);
	cyclic_buffer_put(CB, tstb3, tstb3_size);

	cyclic_buffer_get(CB, tstb, &tstb_size);

	if(memcmp(tstb, tstb1, tstb_size) != 0)
	{

		uart_printf("test2.1 failed \r\n");
		return -1;
	}

	cyclic_buffer_get(CB, tstb, &tstb_size);

	if(memcmp(tstb, tstb2, tstb_size) != 0)
	{

		uart_printf("test2.2 failed \r\n");
		return -1;
	}

	cyclic_buffer_get(CB, tstb, &tstb_size);

	if(memcmp(tstb, tstb3, tstb_size) != 0)
	{

		uart_printf("test2.3 failed \r\n");
		return -1;
	}

	uart_printf("OK\r\n");

	cyclic_buffer_clear(CB);

	uart_printf("test3 started ... ");

	for(i = 0; i < 102; i++)
	{
		if(cyclic_buffer_put(CB, tstb3, tstb3_size) != 0)
		{
			uart_printf("test3.1 add put failed \r\n");
			return -1;
		}
	}

	for(i = 0; i < 10; i++)
	{
		cyclic_buffer_get(CB, tstb, &tstb_size);

		if(memcmp(tstb, tstb3, tstb_size) != 0)
		{
			uart_printf("test3.2 get failed \r\n");
			return -1;
		}
	}

	for(i = 0; i < 5; i++)
	{
		if(cyclic_buffer_put(CB, tstb3, tstb3_size) != 0)
		{
			uart_printf("test3.3 add put failed \r\n");
			return -1;
		}
	}

	for(i = 0; i < (102 - 10 + 5); i++)
	{
		cyclic_buffer_get(CB, tstb, &tstb_size);

		if(memcmp(tstb, tstb3, tstb_size) != 0)
		{
			uart_printf("test3.4 get failed \r\n");
			return -1;
		}
	}

	uart_printf("OK\r\n");

	cyclic_buffer_clear(CB);

	cyclic_buffer_free(CB);
	free(ptr);
	return 0;
}


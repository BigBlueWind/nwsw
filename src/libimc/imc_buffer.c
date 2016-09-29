/**
 * @file    imc_buffer.c
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief   framework for buffer manager
 * @details This file contains the functions definitions for
 *          buffer management and using the buffer.
 *
 * @author Lakshmi Narayan Madala
 *
 */

#include "imc_buffer.h"

/**
* @details  This function initializes the buffer tracker
*           and return pointer to the tracker to caller
*/
BufferTracker *imcInitBufferTracker(void)
{
  BufferTracker *bufferTracker = NULL;

  bufferTracker = malloc(sizeof(BufferTracker));
  bufferTracker->head = NULL;
  bufferTracker->tail = NULL;

  return bufferTracker;
}

/**
 * @details This function allocates memory of size fixed block
 *          and return the pointer to node to the caller.
 */
Node *imcCreateNode(void)
{
  Node *node = NULL;
  node = malloc(sizeof(Node));

  if(node)
  {
    node->data = NULL;
    node->len = 0;
    node->prev = NULL;
    node->next = NULL;

    /* If memory cannot be allocated for data in a node,
     * There is no point of having that node
     **/
    node->data = malloc(FIXED_BLOCK_SIZE);
    if( NULL EQ node->data )
    {
      free(node);
      node = NULL;
    }
  }
  return node;
}

/**
* @param   newNode is the container of the memory block
* @param   bufferTracker locates the header and tail of the list
* @details This function attaches node to the header of the list
*/
void imcAddNodeToHead(
  IO Node *newNode,
  IO BufferTracker *bufferTracker
  )
{
  Node *temp = NULL;

  /* If list is empty assign first node address to both and tail in tracker */
  if(bufferTracker->head EQ NULL)
  {
    bufferTracker->head = newNode;
    bufferTracker->tail = newNode;
  }
  /* Else attach node to the header node and update the new header node */
  else
  {
    temp = bufferTracker->head;
    bufferTracker->head = newNode;
    temp->prev = newNode;
    newNode->next = temp;
    newNode->prev = NULL;
  }
}

/**
* @param   newNode is the container of the memory block
* @param   bufferTracker locates the header and tail of the list
* @details This function attaches node to the tail of the list
*/
void imcAddNodeToTail(
  IO Node *newNode,
  IO BufferTracker *bufferTracker
  )
{
  Node *temp = NULL;

  /* If list is empty assign first node address to both and tail in tracker */
  if(bufferTracker->tail EQ NULL)
  {
    bufferTracker->head = newNode;
    bufferTracker->tail = newNode;
  }
  /* Else attach node to the tail node and update the new tail node */
  else
  {
    temp = bufferTracker->tail;
    bufferTracker->tail = newNode;
    temp->next = newNode;
    newNode->prev = temp;
    newNode->next = NULL;
  }
}

/**
* @param   msg is the pointer to the user data
* @param   len is the length of user data
* @param   bufferTracker locates the header and tail of the list
* @details This function stores the user data in the buffer
*/
TBOOL imcAddBytesToBuffer(
  IN UINT8 *msg,
  IO UINT8 len,
  IO BufferTracker *bufferTracker
  )
{
  TBOOL retVal = TRUE;

  Node *node = NULL;
  /*
  * If length of user data falls under or equal to fixed block size,
  * write the data in the first node
  */
  if(len <= FIXED_BLOCK_SIZE)
  {
    node = imcCreateNode();
    if(node)
    {
      memcpy(node->data, msg, len);
      node->len = len;
      imcAddNodeToTail(node, bufferTracker);
    }
    else
    {
      logError("%s","Could not allocate memory block for buffer");
      retVal = FALSE;
    }
  }
  /*
  * If length of user data is more than fixed block size,
  * write the data into multiple nodes
  */
  else
  {
    while(len)
    {
      node = imcCreateNode();
      if(node)
      {
        if(len <= FIXED_BLOCK_SIZE)
        {
          memcpy(node->data , msg, len);
          node->len = len;
          len = 0;
        }
        else
        {
          memcpy(node->data, msg, FIXED_BLOCK_SIZE);
          node->len = FIXED_BLOCK_SIZE;
          msg += FIXED_BLOCK_SIZE;
          len -= FIXED_BLOCK_SIZE;
        }
        imcAddNodeToTail(node, bufferTracker);
        node = NULL;
      }
      else
      {
        logError("%s","Could not allocate memory block for buffer");
        retVal = FALSE;
      }
    }
  }
  return retVal;
}

/**
* @param   msg is the pointer to the user data
* @param   len is the length of user data
* @param   bufferTracker locates the header and tail of the list
* @details This function stores the user data in the buffer at the top
*/
TBOOL imcAddAtTop(
  IN UINT8 *msg,
  IO UINT8 len,
  IO BufferTracker *bufferTracker
  )
{
  TBOOL retVal = TRUE;

  Node *node = bufferTracker->head;
  /*
  * If list is empty, call function to write
  * the data to top of buffer
  */
  if(node EQ NULL)
  {
    retVal = imcAddBytesToBuffer(msg, len, bufferTracker);
  }
  /*
  * Else write the data to top of buffer
  */
  else
  {
    /*
     * Check if space is available in the header node of the list
     */
    if(bufferTracker->head NOTEQ bufferTracker->tail)
    {
      if(node->len NOTEQ FIXED_BLOCK_SIZE)
      {
        if(len <= (FIXED_BLOCK_SIZE - node->len))
        {
          memcpy(node->data + FIXED_BLOCK_SIZE - (node->len + len), msg, len);
          node->len += len;
          len = 0;
        }
        else
        {
          memcpy(node->data, msg + (len - FIXED_BLOCK_SIZE + node->len), FIXED_BLOCK_SIZE - node->len);
          len -= FIXED_BLOCK_SIZE - node->len;
          node->len = FIXED_BLOCK_SIZE;
        }
      }
    }
    while(len)
    {
      node = NULL;
      node = imcCreateNode();
      if(node)
      {
        if(len <= FIXED_BLOCK_SIZE)
        {
          memcpy(node->data + FIXED_BLOCK_SIZE - len, msg, len);
          node->len = len;
          imcAddNodeToHead(node, bufferTracker);
          break;
        }
        else
        {
          memcpy(node->data, msg + (len - FIXED_BLOCK_SIZE), FIXED_BLOCK_SIZE);
          node->len = FIXED_BLOCK_SIZE;
          imcAddNodeToHead(node, bufferTracker);
        }
        len -= FIXED_BLOCK_SIZE;
      }
      else
      {
        logError("%s","Could not allocate memory block for buffer");
        retVal = FALSE;
      }
    }
   }
  return retVal;
}

/**
* @param   msg is the pointer to the user data
* @param   len is the length of user data
* @param   bufferTracker locates the header and tail of the list
* @details This function stores the user data in the buffer at the bottom
*/
TBOOL imcAddAtBottom(
  IN UINT8 *msg,
  IO UINT8 len,
  IO BufferTracker *bufferTracker
  )
{
  TBOOL retVal = TRUE;
  Node *node = NULL;

  node = bufferTracker->tail;
  /*
  * If list is empty, call function to write
  * the data to top of buffer
  */
  if(node EQ NULL)
  {
    retVal = imcAddBytesToBuffer(msg, len, bufferTracker);
  }
  /*
  * Else write the data to bottom of buffer
  */
  else
  {
    /*
     * Check if space is available in the tail node of the list
     */
    if(node->len NOTEQ FIXED_BLOCK_SIZE)
    {
      if(len <= (FIXED_BLOCK_SIZE - node->len))
      {
        memcpy(node->data + node->len, msg, len);
        node->len += len;
        len = 0;
      }
      else
      {
        memcpy(node->data + node->len, msg, FIXED_BLOCK_SIZE - node->len);
        msg += FIXED_BLOCK_SIZE - node->len;
        len -= FIXED_BLOCK_SIZE - node->len;
        node->len = FIXED_BLOCK_SIZE;
      }
    }
    while(len)
    {
      node = NULL;
      node = imcCreateNode();
      if(node)
      {
        if(len < FIXED_BLOCK_SIZE)
        {
          memcpy(node->data, msg, len);
          node->len = len;
          len = 0;
        }
        else
        {
          memcpy(node->data, msg, FIXED_BLOCK_SIZE);
          node->len = FIXED_BLOCK_SIZE;
          msg += FIXED_BLOCK_SIZE;
          len -= FIXED_BLOCK_SIZE;
        }
        imcAddNodeToTail(node, bufferTracker);
      }
      else
      {
        logError("%s","Could not allocate memory block for buffer");
        retVal = FALSE;
      }
    }
  }
  return retVal;
}

/**
* @param   noBytes is no of bytes to read
* @param   bufferTracker locates the header and tail of the list
* @param   dstBuf is the pointer where to write the data
* @details This function reads data from the top of buffer
*/
TBOOL imcGetBytesFromTop(
  IO UINT8 noBytes,
  IO BufferTracker *bufferTracker,
  IO UINT8 *dstBuf
  )
{
  Node *node = bufferTracker->head;
  UINT8 totalLen = 0;
  TBOOL retVal = TRUE;

  /*
   * Call function to find the total length of data present in the buffer
   */
  totalLen = imcGetTotalLen(bufferTracker);

  if(totalLen < noBytes)
  {
    retVal = FALSE;
  }
  else
  {
    node = bufferTracker->head;
    while(noBytes)
    {
      if(noBytes <= node->len)
      {
        if(bufferTracker->head EQ bufferTracker->tail)
        {
          memcpy(dstBuf, node->data, noBytes);
        }
        else
        {
          if(bufferTracker->head EQ node)
          {
            memcpy(dstBuf, node->data + (FIXED_BLOCK_SIZE - node->len), noBytes);
          }
          else
          {
            memcpy(dstBuf, node->data, noBytes);
          }
        }
        break;
      }
      else
      {
        memcpy(dstBuf, node->data + (FIXED_BLOCK_SIZE - node->len), node->len);
        dstBuf += node->len;
        noBytes -= node->len;
      }
      node = node->next;
    }
  }
  return retVal;
}

/**
* @param   bufferTracker locates the header and tail of the list
* @details This function returns the total length of user data stored in buffer
*/
UINT16 imcGetTotalLen(
  IO BufferTracker *bufferTracker
  )
{
  Node *node = bufferTracker->head;
  UINT16 totalLen = 0;

  while(node)
  {
    totalLen += node->len;
    node = node->next;
  }
  return totalLen;
}

/**
* @param   noBytes is no of bytes to read
* @param   bufferTracker locates the header and tail of the list
* @param   dstBuf is the pointer where to write the data
* @details This function reads data from the bottom of buffer
*/
TBOOL imcGetBytesFromBottom(
  IO UINT16          noBytes,
  IO BufferTracker *bufferTracker,
  IO UINT8         *dstBuf
  )
{
  Node *node = bufferTracker->head;
  UINT16 totalLen = 0;
  TBOOL retVal = TRUE;

  /*
   * Call function to find the total length of data present in the buffer
   */
  totalLen = imcGetTotalLen(bufferTracker);
  if(totalLen < noBytes)
  {
    retVal = FALSE;
  }
  else
  {
    node = bufferTracker->tail;
    while(noBytes)
    {
      if(noBytes <= node->len)
      {
        memcpy(dstBuf, node->data + (node->len - noBytes), noBytes);
        dstBuf += noBytes;
        break;
      }
      noBytes -= node->len;
      node = node->prev;
    }
    while(node->next NOTEQ NULL)
   {
      node = node->next;
      memcpy(dstBuf, node->data, node->len);
      dstBuf += node->len;
    }
  }
  return retVal;
}

/**
* @param   bufferTracker locates the header and tail of the list
* @details This function will freeze all the nodes in the linked list and
*          also the tracker itself
*/
void imcFreeBuffer(
  IO BufferTracker *bufferTracker
  )
{
  Node *node = bufferTracker->head;
  Node *temp = NULL;

  while(node)
  {
    temp = node;
    node = node->next;
    free(temp->data);
    free(temp);
  }

  free(bufferTracker);
}

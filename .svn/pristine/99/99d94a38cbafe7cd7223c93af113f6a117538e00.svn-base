/**
 * @file    imc_buffer.h
 * @brief Includes data structures and all function prototypes
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author : Lakshmi Narayan Madala
 * Date   :
 * Description :
 */

#ifndef IMC_BUFFER_H
#define IMC_BUFFER_H

#include "log.h"

/* Buffer to store user data */
typedef struct s_Node
{
  UINT8 *data;
  UINT8 len;
  struct s_Node *prev;
  struct s_Node *next;
} Node;

/* Buffer tracker to locate the head and tail of the linked list */
typedef struct s_BufferTracker
{
  Node *head;
  Node *tail;
} BufferTracker;

/* Macro to define size of memory block */
#define FIXED_BLOCK_SIZE 50

/* Function prototypes */
BufferTracker *imcInitBufferTracker( void );

Node *imcCreateNode( void );

void imcAddNodeToHead(
  IO Node *newNode,
  IO BufferTracker *bufferTracker
  );
void imcAddNodeToTail(
  IO Node *newNode,
  IO BufferTracker *bufferTracker
  );
TBOOL imcAddAtTop(
  IN UINT8 *msg,
  IO UINT8 len,
  IO BufferTracker *bufferTracker
  );
TBOOL imcAddAtBottom(
  IN UINT8 *msg,
  IO UINT8 len,
  IO BufferTracker *bufferTracker
  );
TBOOL imcAddBytesToBuffer(
  IN UINT8 *msg,
  IO UINT8 len,
  IO BufferTracker *bufferTracker
  );
UINT16 imcGetTotalLen(
  IO BufferTracker *bufferTracker
  );
TBOOL imcGetBytesFromBottom(
  IO UINT16          noBytes,
  IO BufferTracker *bufferTracker,
  IO UINT8         *dstBuf
  );
TBOOL imcGetBytesFromTop(
  IO UINT8 noBytes,
  IO BufferTracker *bufferTracker,
  IO UINT8 *dstBuf
  );
void imcPrintBuffer(
  IN BufferTracker *bufferTracker
  );
void imcFreeBuffer(
  IO BufferTracker *bufferTracker
  );

#endif

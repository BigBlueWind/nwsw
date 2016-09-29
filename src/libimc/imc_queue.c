/**
 * @file    imc_queue.c
 * @brief This file contains queue functionality.
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author Manu Sharma
 * @author Ramprasad Chinta
 *
 * @todo complete implementation of queue is to be done at a latter date.
 *       Presently we use libgdsl for these functions.
 */


#include "imc_queue.h"

/* Implementing as a circular buffer
   (A)
   msgs 0 1 2 3 4 5 6 7 ......... N
            |       | 
            + first |
                    + last  
   first < last < N  [  N Elements in queue = last - first + 1 ]
   add new msg to last+1

   (B)
   msgs 0 1 2 3 4 5 6 7 ......... N
              |              | 
              |              + first
              + last  
   first > last
   But last - first > 1
   add new msg to last+1
   nElements = N - first + last + 1
 
   (C)
   (last+1) = first => Queue full
*/

/* Add elem to end of que */
void Enqueue(
  Queue *queue,
  void  *elem
)
{
  if(queue->nElem < MAX_MSG_Q_LEN)
  {
    queue->nElem++;
    queue->elem[queue->last] = elem;
     addModulo(queue->last, 1, MAX_MSG_Q_LEN, &queue->last);
  }
  else
  {
    BS_ERROR_QUEUE_FULL();
  }
  logInfo("Q: first=%d, last=%d, nElem=%d", queue->first, queue->last, queue->nElem);
}

void *Dequeue(
  Queue *queue
)
{
  void *elem = queue->elem[queue->first];
  if(elem)
  {
    addModulo(queue->first, 1, MAX_MSG_Q_LEN, &queue->first);
    queue->nElem--;
    logInfo("Q: first=%d, last=%d, nElem=%d", queue->first, queue->last, queue->nElem);
  }
  return elem;
}

void QInit(Queue *q)
{
  UINT32 i;
  for (i=0; i< MAX_MSG_Q_LEN; i++)
  {
    q->elem[i] = NULL;
  }
  q->first = 0;
  q->last = 0;
  q->nElem = 0;
}

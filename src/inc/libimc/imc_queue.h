/**
 * @file    imc_queue.h
 * @brief Description of all messages in the system
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author : Manu Sharma
 *          Ramprasad Chinta
 * Date   :
 * Description :
 */
#ifndef IMC_QUEUE_H
#define IMC_QUEUE_H

#include "all_cmn.h"

typedef struct s_Queue
{
  void   *elem[MAX_MSG_Q_LEN];
  UINT16  first;
  UINT16  last;
  UINT16  nElem;
} Queue;

void Enqueue(
  Queue *queue,
  void  *elem
);

void *Dequeue(
  Queue *queue
);

void QInit(Queue *q);
/* API */
#define qHasElements(q) q->nElem != 0
#define qEnqueue(q, elem) Enqueue(q, elem), elem = NULL
#define qDequeue Dequeue
#define qInit QInit


#endif /* IMC_QUEUE_H */

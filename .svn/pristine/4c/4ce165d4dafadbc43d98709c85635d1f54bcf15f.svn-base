/* memory related statistics */

#ifdef PWAV_MEM2
#include "pwav_mem.h"
#include <string.h>

/** GLOBALS **/
PmStats g_pwavMemStats = {0};

/** MACROS **/

/** FUNCTIONS **/

/** TODO **/
void pmPrintStats(const char *tag, char *fillMe);

/** 
 * Copies all statistics to fillMe 
 */

void pmFillStats(PmStats *fillMe)
{
  memcpy(fillMe, getPwavMemStats(), sizeof(PmStats));
}

/** 
 * Sets all statistics to 0
 */

void pmResetStats(void) 
{ 
  memset(getPwavMemStats(), 0, sizeof(PmStats)); 
}

/** 
 * Returns the pointer to the statistics of tid
 */

PmThreadStats *getPwavMemStatsByTid(Tid tid)
{
  int i;
  PmStats       *st = getPwavMemStats();

  for (i=0; i < st->n; i++) {
    if (tid == st->currentThreadStats[i].tid) 
      return &(st->currentThreadStats[i]);
   }
   /* thread is not yet found */
   if (i < PM_MAX_THREADS)
   {
     st->n++;
     st->currentThreadStats[i].tid = tid; 
     PM_PRINTF("Starting stats collection for tid=%lu, n=%ld\n", tid, st->n);
     return &(st->currentThreadStats[i]); 
   }
   else 
   {
     PM_PRINTF("ERROR: could not find stats for tid=%ld in n=%ld (max=%d)\n", tid, st->n, PM_MAX_THREADS);
     return NULL;
   }
}

/** 
 *  Updates alloc count.
 *  Called when memory alloc request is received. 
 *  thread id is taken from pthread_self()
 */

void pwavUpdateAllocStats(
  unsigned    reqSize,          /* bytes requested */
  int         ok,               /* 0 means failed */
  int         src,              /* 1 means from bucket, 2 means heap */
  unsigned   actuallyAllocated
)
{
  Tid tid             = pthread_self(); /* whose statistics are to be updated */
  PmThreadStats   *st = getPwavMemStatsByTid(tid);
  int               i = 0;
  int         fitsBkt = 0;

  if (!st) return;
  /* else case */
  
  if (st->isLocked > 0) 
  {
    PM_PRINTF("ERROR: stats of tid=%lu locked by tid=%lu. Alloc not updated. Req=%u, ok=%d, src=%d, allocated=%u\n",
               tid, st->isLocked, reqSize, ok, src, actuallyAllocated);
    return;
  }

  /* else  */
  st->isLocked = tid;

  if (reqSize > st->maxReqSize)
  {
    PM_PRINTF("tid=%lu maxReqSize=%u (old %ld)\n", tid, reqSize, st->maxReqSize);
    st->maxReqSize = reqSize;
  }

  /* find the bucket where this fits */
  while (i< PM_MAX_BKTS)
  {
    if((reqSize >> i) <= PM_BKT_QTM) { fitsBkt = 1; st->bktStats[i].numReq++; break; }
    i++;
  }
  if(!fitsBkt) 
  {
   PM_PRINTF("Did not fit any bucket: %d bytes\n", reqSize);
   st->numNonBktReq++;
  }

  if (ok) 
  { 
    if (src == 1) /* bucket */ st->bytesAllocFromBkts  += reqSize;
    else                       st->bytesAllocFromHeap  += reqSize;
  }
  else /* was not allocated */
  {
    st->numAllocFail++;
    if(src == 1)
      PM_PRINTF("ERR: memoy alloc failed from bucket (%u)\n", reqSize);
    else if (src == 2)
      PM_PRINTF("ERR: memoy alloc failed from heap (%u)\n", reqSize);
    else
      PM_PRINTF("ERR: FATAL: memoy alloc (%u) failed from bucket and heap\n", reqSize);
  }
 
  st->isLocked = 0;
#if PM_ENABLE_PERIODIC_PRINT
  if(st->numAllocReq % PM_PRINT_PERIOD == 0) /* TODO */
  printPwavMemStats("periodic", st);
#endif
}

/** 
 * Updates memory free count. 
 * Called when a thread frees memory. 
 * thread id is taken from pthread_self()
 */

void pwavUpdateFreeStats(
  long  size, 
  int   ok, 
  int   src /* free to buckets or heap */
)
{

  Tid tid             = pthread_self(); /* whose statistics are to be updated */
  PmThreadStats   *st = getPwavMemStatsByTid(tid);
  int i               = 0;
  int fitsBkt         = 0;

  if(st->isLocked > 0)
  {
    PM_PRINTF("ERROR: stats of tid=%lu locked by tid=%lu: Free not updated. bytes freed=%ld, ok=%d, src=%d\n", 
       tid, st->isLocked, size, ok, src);
    return;
  }
  /* else */

  st->isLocked = tid;

  while (i< PM_MAX_BKTS)
  {
    if      ((size >> i) <= PM_BKT_QTM) { fitsBkt = 1; st->bktStats[i].numFree++; break; }
    i++;
  }

  if(!fitsBkt) st->numNonBktFree++;

  if(ok)
  {
    if (src == 1) st->bytesAllocFromBkts  -= size;
    else          st->bytesAllocFromHeap  -= size;
  }
  else
  {
    st->numFreeFail++;
    if      (src == 1) PM_PRINTF("ERR: FATAL: memoy free failed - bucket for tid=%ld\n", tid);
    else if (src == 2) PM_PRINTF("ERR: FATAL: memoy free failed - heap for tid=%ld\n",   tid);
  }

  st->isLocked = 0;
}


/**
 * print the collected memory statistics
 * prints the statistics for all threads
 */

void printPwavMemStats(const char *tag, PmStats *totalStats)
{
  int i, j;
  PmThreadStats *st;

  PM_PRINTF("-- %s -- \nTid        |bkt|  numReq   | numFree   | stillAlloc\n", tag);
  PM_PRINTF("-----------------------------------------------------------------\n");
  for (i=0; i < totalStats->n; i++)
  {
    st = &(totalStats->currentThreadStats[i]);
    for (j = 0; j < PM_MAX_BKTS; j++) 
    {
      PM_PRINTF("%lu | %d | %9ld | %9ld | %3ld\n",
                 st->tid,
                 j, 
                 st->bktStats[j].numReq,
                 st->bktStats[j].numFree, 
                 st->bktStats[j].numReq - st->bktStats[j].numFree);
   }

  PM_PRINTF ("\nnumNonBktReq %ld | numNonBktFree %ld | numAllocFail %ld | numFreeFail %ld | bytesAllocFromBkts %ld | bytesAllocFromHeap %ld\n",
      st->numNonBktReq,
      st->numNonBktFree,
      st->numAllocFail,
      st->numFreeFail, 
      st->bytesAllocFromBkts,
      st->bytesAllocFromHeap);
  PM_PRINTF("-----------------------------------------------------------------\n");
  }
}

#if TEST

#include <stdio.h>

#define checkval(fld, val )  printf("Test %s=%ld: %s\n", #fld, fld, (fld == val ? "OK" : "FAIL"))

void *run_test(void *dummy)
{
  PmStats       *gst = getPwavMemStats();
  PmThreadStats *st  = getPwavMemStatsByTid(pthread_self());
  /* add elem 1 to bkt 0 */
  pwavUpdateAllocStats(10, 1, 1, 10);
  printPwavMemStats(NULL, gst);
  checkval(st->numAllocFail, 0);
  checkval(st->bytesAllocFromBkts, 10);
  checkval(st->bktStats[0].numReq, 1);
  
  /* add elem 2 bkt 0 */
  pwavUpdateAllocStats(20, 1, 1, 20);
  printPwavMemStats(NULL, gst);
  checkval(st->numAllocFail, 0);
  checkval(st->bytesAllocFromBkts, 30);
  checkval(st->bktStats[0].numReq, 2);

  /* add elem 1 to bkt 1 */
  pwavUpdateAllocStats(33, 1, 1, 33);
  printPwavMemStats(NULL, gst);
  checkval(st->bktStats[1].numReq, 1);

  /* add  elem 2 to bkt 1 */
  pwavUpdateAllocStats(64, 1, 1, 64);
  printPwavMemStats(NULL, gst);
  checkval(st->bktStats[1].numReq, 2);

  /* add elem 1 to bkt 3 */
  pwavUpdateAllocStats(257, 1, 1, 512);
  printPwavMemStats(NULL, gst);
  checkval(st->bktStats[3].numReq, 1);

  /* remove 1 elem from bkt 1 */
  pwavUpdateFreeStats(64, 1, 1);
  printPwavMemStats("abc", gst);
  checkval(st->bktStats[1].numFree, 1);

  return NULL;
}

int main (int argc, char **argv)
{

  int i, j=3;
  Tid tid[3];

  while (j-- > 0) {
    i = pthread_create (&tid[j], NULL, run_test, NULL);
    if (i != 0) perror("pthread_create");
    else printf("created tid=%lu\n", tid[j]);
  }
   
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);
  
  return 0;

}

#endif
#endif

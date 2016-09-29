#include "pwav_ipcmemleakfinder.h"

MBufStatsTable g_mbufStatsTable;
unsigned cntr = 0;

void insertMbufStat(unsigned char src, unsigned char dst, unsigned char event, const void *mBuf)
{
  unsigned i = 0;
  if (g_mbufStatsTable.n >= MAX_MBUF_STATS)
  {
    printf("ERR | %s |  insert failed, increase MAX_MBUF_STATS (%d\n)", __func__, MAX_MBUF_STATS);
    return;
  }
  while(i <  MAX_MBUF_STATS)
  {
    if(g_mbufStatsTable.stats[i].ptr == NULL)
    {
      g_mbufStatsTable.stats[i].ptr    = mBuf;
      g_mbufStatsTable.stats[i].srcEnt = src;
      g_mbufStatsTable.stats[i].dstEnt = dst;
      g_mbufStatsTable.stats[i].event  = event;
      g_mbufStatsTable.n++;
      break;
    }
    i++;
  }
}

void markMbufFreed(const void *mBuf)
{
  unsigned i;
  for (i=0; i < MAX_MBUF_STATS;i++)
  {
    if(g_mbufStatsTable.stats[i].ptr == mBuf)
    {
      g_mbufStatsTable.stats[i].ptr = NULL; /* mark it as freed */
      g_mbufStatsTable.n--;
      return;
    }
  }
  printf("PTR not found in the list: %x\n",mBuf);
}
void printMbufStats(void)
{
  unsigned i;
  printf("Current Print Entry: %d\n",cntr++);
  printf("\ni   | ptr | src | dst | event \n ----------------------------------------- \n");
  for (i=0; i< MAX_MBUF_STATS;i++)
  {
    if(g_mbufStatsTable.stats[i].ptr != NULL)
    {
     printf("SNo - %d   | Alloctd Ptr - %p | Src Ent - %d | Dst Ent - %d | Event - %d\n",
        i,
        g_mbufStatsTable.stats[i].ptr,
        g_mbufStatsTable.stats[i].srcEnt,
        g_mbufStatsTable.stats[i].dstEnt,
        g_mbufStatsTable.stats[i].event
      ); 
     /*logError("SNo - %d   | Alloctd Ptr - %p | Src Ent - %d | Dst Ent - %d | Event - %d\n",
        i,
        g_mbufStatsTable.stats[i].ptr,
        g_mbufStatsTable.stats[i].srcEnt,
        g_mbufStatsTable.stats[i].dstEnt,
        g_mbufStatsTable.stats[i].event
      );*/
    }
  }
  printf("Total Allocatd PTRs: %d\n",g_mbufStatsTable.n);
}

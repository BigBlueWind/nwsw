
#ifndef PWAV_IPC_MEM_LEAK_DET_
#define PWAV_IPC_MEM_LEAK_DET_

/*#include "pst_events.h"
#include "ccpu_cmn.h"
#include "imc_unix.h"
#include "imc_udp.h"
#include "sfi.h"
#include "errno.h"
#include "sys/socket.h"
#include "sys/types.h"
#include <sys/ioctl.h>
#include "mt_4gmx.x"

#include "cm_lib.x"
#include "ssi.x"           
#include "gen.x"           
#include "log.h"
#include "cmn_types.h"     */

#include <stdio.h>
#include "log.h"

typedef struct s_mbufStats
{
  void          *ptr; /* mBuf */
  unsigned char srcEnt;
  unsigned char dstEnt;
  unsigned char event;
} MBufStats;

#define MAX_MBUF_STATS 10000
typedef struct s_mbufStatsTable
{
  unsigned n; /* how many are present in in the array below */
  MBufStats stats[MAX_MBUF_STATS];
} MBufStatsTable;


void insertMbufStat(unsigned char src, unsigned char dst, unsigned char event, const void *ptr);
void markMbufFreed(const void *ptr);
void clearMbufStats(void);
void printMbufStats(void);
#endif

/*****************************************************************************80
 * @file        pwav_mem.h
 * @details     Copyright Powerwave Technologies, Inc., 2009
 * @brief       Determines memory statistics for SW using SSI (cmAlloc, cmFree).
 *              Determines statistics based on cmAlloc and cmFree only.
 *
 * Change History:
 *  @author    Manu Sharma
 *  @date      17 July 2012
 *  @details   Initial Version
 *    - collects number of alloc requests, frees, bytes requested, allocated.
 *    - Uses a concept of buckets to determine the packet sizes requested.
 *    - These bucket sizes are its own and have nothing to do with actual bucket sizes in SSI.
 *    - Also these buckets have nothing to do with {region, pool} concept of SSI.
 *    - Statistics are printed using printf().
 *  
 **/

/** Feature Name      : Powerwave Memory Statistics Utility
 *  Compilation Flag  : PWAV_MEM2
 *  Prefix            : PM for macros, Pm for structures and functions
 */

#ifdef PWAV_MEM2 /* Feature enabler */
#include <stdio.h> /* for printf */
#include <pthread.h>

/** Bucket Definition.
 *  BucketNumber        Size        Meaning
 * -------------------------------------------------------------------
 *  0       BKT_QTM =  32          requests between 1  and   32 bytes 
 *  1        32 * 2 =  64          requests between 33 and   64 bytes 
 *  2                 128          requests between 65 and  128 bytes
 *  3                 256          ...
 *  4                 512
 *  5                1024
 *  6                2048
 *  7                4096
 */
 
/** USER CONFIGURABLE PARAMETERS **/

#define PM_MAX_BKTS         12     /* means there are 8 buckets, numbered from 0 to 7 as shown above */ 
#define PM_BKT_QTM          32    /* See Bucket Definition */

/** Memory stats can be printed periodically or at specific points by the user.
 *  The parameters below are for periodic printing.
 *  Periodic here refers to number-of-allocation requests, not time.
 *  Reason: This utility does not use timers. However, it can be extended.
 *
 *  PM_ENABLE_PEIODIC_PRINT enables or disables periodic printing.
 *  PM_PRINT_PERIOD  x means statistics will be printed after every x alloc requests.
 */

#define PM_ENABLE_PERIODIC_PRINT      0
#define PM_PRINT_PERIOD            2000    /* in terms of allocs */

#define PM_MAX_THREADS               10    /* number of threads that this app is expected to encounter */
/* function to print the statistics. Default is printf.
 * users can customize to other functions like logInfo(), logCounters() etc 
 * but that should have the same parameters as printf.
 */
#define PM_PRINTF                  printf
/** END OF CONFIGURABLE PARAMETER **/

/** Requirements and Design:
 *  1  List the number of alloc calls per bucket 
 *     - can be used to determine which buckets are being used most frequently
 *
 *  2  List the number of free  calls per bucket
 *     -  (alloc-calls - free-calls) are the potential leaks
 *
 *  3  Should work in a multi-threaded environment.
 *     - We dont want this utility to change run time behaviour of threads (to the extent possible).
 *     - So no semaphores are used; We collect data per thread.
 *     - While printing data is printed for each thread and total.
 *     - Note that thread-1 may allocate and send the buffer to thread-2, which then frees it.
 *       So leak should not be judged from thread data. 
 */
 
typedef struct s_pmBktStats
{
  long   numReq;                  /* number of alloc calls */
  long   numFree;                 /* number of free calls  */ 
} PmBktStats;

typedef pthread_t Tid; /* Thread Id: OS Specific */

typedef struct s_pmThreadStats {

  Tid           tid;                   /* threadid for which these are being collected */
  Tid           isLocked;              /* non zero means tid is currently updating it. Set to 0 when function returns */
  long          numAllocFail;          /* number of alloc fails */
  long          numFreeFail;           /* number of free fails  */
  long          bytesAllocFromBkts;    /* total bytes that are still allocated from buckets */
  long          bytesAllocFromHeap;    /* bytes bytes that are still allocated from heap */
  long          numNonBktReq;          /* allocs from heap i.e. those that did not fit any bucket */
  long          numNonBktFree;         /* free to heap */
  long          maxReqSize;            /* max size requested so far */
  PmBktStats    bktStats[PM_MAX_BKTS]; /* bucket wise alloc, frees of this thread */
} PmThreadStats;

/** All statistics are stored here */
typedef struct s_pmStats
{
  Tid           locked; /* flag to check if stats are being updated by someone else.
                         * Contains pid that has locked it */

  /* number of valid entries in the array below. If value is > 0, then (0 .. val-1) are the valid entires. */
  long          n;
  PmThreadStats currentThreadStats[PM_MAX_THREADS];
} PmStats;

extern PmStats g_pwavMemStats;
#define getPwavMemStats() &(g_pwavMemStats)
/** APIS *******************************************************************78*/

/** @brief    prints the collected memory statistics. 
 *            2 Modes are supported a) printing using STATSP and b) to a buffer.
 *  
 *   tag      - a string that will be printed along with the stats.
 *            - if buffer is passed, this string will be printed at start of buffer, followed by stats. 
 *            - not used by the utility as such
 *            - can be passed as NULL
 *
 *   fillMe   - a buffer into which the stats should be printed
 *            - Can be NULL
 *            - if NULL, stats will be printed using printf
 *            - else,    stats will be printed into buffer
 *            - buffer should be large enough: TODO: Add size
 *
 *   examples - printPmStats(NULL, NULL); - will print statistics using printf
 *            - printPmStats("before-unlock", NULL); - will print ,'before-unlock', followed by statistics 
 *            - printPmStats("at rrcConnRecd", fillMe); - will print the stats to fillMe
 *
 */

void pmPrintStats(const char *tag, char *fillMe);
void pmFillStats(PmStats *fillMe);                /* returns the current stats */
void pmResetStats(void);                          /* resets all stats to 0 and restarts collection */

#endif  /* PWAV_MEM2 */

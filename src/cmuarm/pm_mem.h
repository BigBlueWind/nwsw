/** File        : pm_mem.h
 *              :
 *  Purpose     : Memory leak detection for software using SSI's memory management.
 *              :
 *  Description : SSI offers 4 memory alloc APIs to applications cmAllocEvnt, SGetSBuf, SGetMsg, cmGetMem.
 *              : This utility tries to find allocations made using these APIs.
 *              : 
 *  Method      : Allocation and Deallocation of each of the above is recorded.
 *              : The recorded information includes the pointer, file and line.
 *              :
 *  Detection   : This tool lists memory elements that are 'still' allocated.
 *              : An engineer then has to analyze the information to determine 
 *              : a) if it is a leak OR b) regular storage in a buffer.
 *              :
 *  Output      : User can print all allocations at a given time by sending MOD_STATE_REQ to UEH (bs.sh status).
 *              : The allocations are printed using printf and thus go to where stdout is redirected.
 *              : Allocations of all modules in that executable are printed.
 *              : For example, in cp.exe allocations of UEH, RRC, S1AP and ipc are printed.
 *              : The output consists of 3 tables:
 *              :   'block' tab contains information about currently cmAllocEvnt allocated blocks
 *              :   'msg'   tab contains information about currently sGetMsg     allocated blocks
 *              :   'sbuf'  tab contains information about currently sGetSbuf    allocated blocks
 * Sample       :
[--- block tab=0x4ec8d4 max=500 nAlloc=0 nAllocReq=5 nAllocFail=0 nFreeReq=5 nFreeFail=0 idx=0 ---]
idx | ptr | isAlloc | size | alloc.file | alloc.line
--  -- end tab=0x4ec8d4  --  --

[--- msg tab=0x4ec95c max=6000 nAlloc=2 nAllocReq=355 nAllocFail=0 nFreeReq=353 nFreeFail=0 idx=103 ---]
idx | ptr | isAlloc | size | alloc.file | alloc.line
 92 | 0x402a8188 | 1 | 0 | src/mtuarm/ss_msg.c | 3090
 93 | 0x402a8508 | 1 | 0 | src/mtuarm/ss_msg.c | 3090
--  -- end tab=0x4ec95c  --  --

[--- sbuf tab=0x4ec9e4 max=1000 nAlloc=80 nAllocReq=245 nAllocFail=0 nFreeReq=165 nFreeFail=0 idx=84 ---]
idx | ptr | isAlloc | size | alloc.file | alloc.line
 0 | 0x40a5a880 | 1 | 5000 | ./src/cprm/ueh/ueh_mi.c | 128
 1 | 0x40a594f8 | 1 | 5000 | ./src/cprm/ueh/ueh_mi.c | 128
 2 | 0x40293508 | 1 | 40 | src/sctp/sb_bdy4.c | 551
 3 | 0x40293308 | 1 | 20 | src/sctp/sb_bdy4.c | 566
 4 | 0x40293608 | 1 | 40 | src/sctp/sb_bdy4.c | 581
 5 | 0x40293488 | 1 | 20 | src/sctp/sb_bdy4.c | 596

 * Notes        : Do not interpret idx field - it has no significance.
 *
 * Unit Testing : This utility can be tested on host also.
 *              : Build it as: gcc -g -DTEST_PM pm_mem.c
 *
 */

/* User configurable parameters - none */

#include <pthread.h> /* mutex */

/** Types */

/* Location from where memory was allocated or freed. */
typedef struct s_PmSrcLoc {
  char *file;
  int   line;
} PmSrcLoc;

typedef struct s_PmMemTableEntry {

  int       index;
  void     *ptr;
  int       isAllocated; /* 1 means allocated */
  int       size;
  PmSrcLoc  alloc;
  PmSrcLoc  free;

  struct s_PmMemTableEntry *next;
  struct s_PmMemTableEntry *prev;

} PmTableEntry;

/**
 *      A doubly linked circular list
 *
 *      --------------<--------------
 *     |                             |
 *    Node  <--     .prev           .next
 *   .next   -->     Node   <--     .prev
 *   .prev          .next   -->     Node
 *     |                             |
 *      -------------->--------------
 */

typedef struct s_PmTable {
  char         name[80];    /* Table name; for convenience of user while printing */
  int          isEnabled;   /* A way of enabling and disabling at run-time. 
                               1 means this table is being used/enabled, 0 means not used/disabled. */
  int          index;       /* index of available buffer */
  int          maxElem;     /* number of rows; used to malloc elem[] */
  int          nAlloc;      /* number of elements currently filled */
  int          nAllocReq;   /* number of alloc requests */
  int          nFreeReq;    /* number of free requests */
  int          nAllocFail;  /* number of alloc requests that could not be recorded */
  int          nFreeFail;   /* number of free requests that had no record in table */
  pthread_mutex_t  mutex;   /* Restrict alloc and free entries to one thread at a time */

  PmTableEntry *elem;       /* first element is elem[0]. range 0 .. (maxElem -1) */
} PmTable;

/** Globals **/
extern PmTable g_blockBufTable;
extern PmTable g_staticBufTable;
extern PmTable g_msgBufTable;

/** Prototypes **/

/* Record in 'tab' that 'ptr' has been allocated, of 'size' bytes, from 'file', 'line' */
void pmRecordBufAlloc(
  PmTable   *tab,
  void      *ptr,
  int        size,
  char      *file,
  int        line);

/* Record in 'tab' that 'ptr' has been deallocated/freed, of 'size' bytes, from 'file', 'line' */
void pmRecordBufFree(
  PmTable         *tab,
  const void      *ptr,
  int              size,
  char            *file,
  int              line);

/* Create a PmTable of 'maxElem'
 * This function must be called first, say in main/SInit, 
 * before memory allocation/free functions are called.
 */
void pmInitCircularList(PmTable *tab, char *name, int maxElem);

/* Print the contents of PmTable 
 * printAll = 0 means print allocated buffer information.
 * printAll = 1 means print the whole table (all entries).
 * For most purposes, printAll = 0 is adequate. 
 */
void printPmTable(PmTable *tab, int printAll);

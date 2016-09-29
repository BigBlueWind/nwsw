//-------------------------------------------------------------------------------------------
/** @file mlog.h
 *
 * @brief Mindspeed Transaction Executive (TrEx) Profiler and System Debug Trace Logger
 * @author Mindspeed Technologies
 * @version $Revision: 1.79 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef _MLOG_H_
#define _MLOG_H_

#ifdef __cplusplus
extern "C"
{
#endif

//#define MLOG_SUBTASK_ENABLE    //allows for ceva sub task profiling for some tasks.
//#define MLOG_ADD_DEPENDENCY    // allows to add dependencies in MLOG
//#define MLOG_IRQ_SUP_ENABLE    //allows  for monitiring time spent by arm in irq or supervisor more ( heavy logging)

#if defined(_WIN32) || defined(__linux__) || defined(__KERNEL__)
#define MX_OK 0
#endif

#ifndef _WIN32
#define MLOG_ENABLED
#endif

#ifdef __KERNEL__
#define MLOG_INLINE __always_inline
#else
#define MLOG_INLINE __inline
#endif

#define MLOG_TRUE             1
#define MLOG_FALSE            0

#define MLOG_DWORD unsigned int
#define MLOG_UINTSIZE       4
#define MLOG_BUFFERS_COUNT  4


#define MLOG_NOP              0x00000000
#define MLOG_NEWLOG           0x01000000
#define MLOG_INFO             0x08000000
#define MLOG_ERROR            0x09000000
#define MLOG_END              0x0A000000
#define MLOG_CONTEXT          0x14000000
#define MLOG_INPUT            0x15000000
#define MLOG_OUTPUT           0x16000000
#define MLOG_BUFFERIN         0x17000000
#define MLOG_BUFFEROUT        0x18000000
#define MLOG_BUFFER           0x19000000



#define MLOG_SIZE_MASK        0x00FFFFFF
#define MLOG_ID_MASK          0xFF000000

#define MLOG_SIZE_EXESTART      12
#define MLOG_EXESTART           0x0C000000
#define MLOG_ID_EXESTART        0x0C00000C // MLOG_EXESTART & MLOG_SIZE_EXESTART

#define MLOG_SIZE_EXEFINISH     20
//#define MLOG_SIZE_EXEFINISH     16
//#define MLOG_EXEFINISH          0x0D000000
#define MLOG_ID_EXEFINISH       0x0D000014 // MLOG_EXEFINISH & MLOG_SIZE_EXEFINISH
//#define MLOG_ID_EXEFINISH       0x0D000010 // MLOG_EXEFINISH & MLOG_SIZE_EXEFINISH

#define MLOG_SIZE_NEWTASK       12
#define MLOG_NEWTASK            0x0F000000
#define MLOG_ID_NEWTASK         0x0F00000C // MLOG_NEWTASK  & MLOG_SIZE_NEWTASK

#define MLOG_SIZE_SUBTASK       12
#define MLOG_SUBTASK            0x20000000
#define MLOG_ID_SUBTASK         0x2000000C // MLOG_SUBTASK & MLOG_SIZE_SUBTASK

#define MLOG_SIZE_FREQ          12
#define MLOG_FREQ               0x21000000
#define MLOG_ID_FREQ            0x2100000C // MLOG_FREQ   & MLOG_SIZE_FREQ

#define MLOG_SIZE_TASK          20
#define MLOG_TASK               0x22000000
#define MLOG_ID_TASK            0x22000014 // MLOG_TASK   & MLOG_SIZE_TASK

#define MLOG_SIZE_MARK          8
#define MLOG_MARK               0x23000000
#define MLOG_ID_MARK            0x23000008 // MLOG_FREQ   & MLOG_SIZE_FREQ

#define MLOG_SIZE_LIST          4
#define MLOG_LIST               0x25000000
#define MLOG_ID_LIST            0x25000004

#define MLOG_LISTSTART          0x26000000
#define MLOG_ID_LISTSTART       0x26000008

#define MLOG_LISTSTOP           0x27000000
#define MLOG_ID_LISTSTOP        0x27000008

#define MLOG_SIZE_ADDTOLIST     8
#define MLOG_ADDTOLIST          0x28000000
#define MLOG_ID_ADDTOLIST       0x28000008

#define MLOG_SIZE_RESCONDITION  12
#define MLOG_RESCONDITION       0x33000000
#define MLOG_ID_RESCONDITION    0x3300000C

#define MLOG_SIZE_MARK_TCB_CTRL 8
#define MLOG_MARK_TCB_CTRL      0x34000000
#define MLOG_ID_MARK_TCB_CTRL   0x34000008


#define MLOG_SIZE_DEPENDS       8
#define MLOG_DEPENDS            0x02000000
#define MLOG_ID_DEPENDS         0x02000008 // MLOG_DEPENDS  & MLOG_SIZE_DEPENDS
#define MLOG_DEPENDS_TCB_TCB    MLOG_ID_DEPENDS

#define MLOG_TASKDEPENDS        0x24000000
#define MLOG_ID_TASKDEPENDS     0x24000008 // MLOG_TASKDEPENDS  & MLOG_SIZE_DEPENDS
#define MLOG_DEPENDS_KEY_KEY    MLOG_ID_TASKDEPENDS

#define MLOG_LTDEPENDS          0x29000000
#define MLOG_ID_LTDEPENDS       0x29000008
#define MLOG_DEPENDS_LISTID_TCB MLOG_ID_LTDEPENDS

#define MLOG_TLDEPENDS          0x30000000
#define MLOG_ID_TLDEPENDS       0x30000008
#define MLOG_DEPENDS_TCB_LISTID MLOG_ID_TLDEPENDS

#define MLOG_LKDEPENDS          0x31000000
#define MLOG_ID_LKDEPENDS       0x31000008
#define MLOG_DEPENDS_LISTID_KEY MLOG_ID_LKDEPENDS

#define MLOG_KLDEPENDS          0x32000000
#define MLOG_ID_KLDEPENDS       0x32000008
#define MLOG_DEPENDS_KEY_LISTID MLOG_ID_KLDEPENDS

#define MLOG_SIZE_FR_SUBFR_NUM  8
#define MLOG_FR_SUBFR_NUM       0x36000000
#define MLOG_ID_FR_SUBFR        0x36000008

#define MLOG_ID_ADD_VARIABLES   0x37000000

#define MLOG_SIZE_DEVICE_INFO   4
#define MLOG_DEVICE_INFO        0x38000000
#define MLOG_ID_DEVICE_INFO     0x38000004

#define MLOG_SIZE_CACHE_MIPS_STATS         ((CPU_NUM*7)+2)*4  // 2 = CPU_NUM and GetTicks
#define MLOG_SIZE_CACHE_MIPS_STATS_SINGLE  ((1*8)+2)*4                   // 2 = CPU_NUM and GetTicks
#define MLOG_CACHE_MIPS_STATS               0x39000000
#define MLOG_ID_CACHE_MIPS_STATS            MLOG_CACHE_MIPS_STATS | MLOG_SIZE_CACHE_MIPS_STATS
#define MLOG_ID_CACHE_MIPS_STATS_SINGLE     MLOG_CACHE_MIPS_STATS | MLOG_SIZE_CACHE_MIPS_STATS_SINGLE



#define MLOG_MAGIC                 0x474f4c4d  // "MLOG"
#define MLOG_VERSION               29

#define MLOG_FILENAME "mlog.bin"


#define MLOG_MARK_NOTHING        0
#define MLOG_MARK_FRAMEBORDER    1

#define MLOG_MAX_LOGGING_SPACE   1000000 // Ticks (Some arbitrary large Tick Counter)
#define MLOG_DONT_LOCK           0
#define MLOG_PUT_LOCK            1



#define MLOG_MODE_UNKNOWN        0
#define MLOG_MODE_IRQ            1
#define MLOG_MODE_SUPERVISOR     2
#define MLOG_MODE_IDLE           3


#define MLOG_FRAME_LIMIT 200
#define MLOG_FRAME_SIZE  (7*1024)//3584  //4*1024

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
#define RC_MLOG_ALLOC_ERROR         DEF_USRRC(APP_MODULE, MLOG_SVSR,  1)
#define RC_MLOG_CTX_ERROR           DEF_USRRC(APP_MODULE, MLOG_SVSR,  2)
#define MLOG_DEVICE_COUNT           1

#else
#define RC_MLOG_ALLOC_ERROR         1
#define RC_MLOG_CTX_ERROR           2
#define RC_MLOG_NO_SPACE            3
#if defined(__KERNEL__)
#define CPU_NUM                     2
#else
#define CPU_NUM                     1
#endif
#define MLOG_DEVICE_COUNT           0


#define _memcpy memcpy
#ifndef INLINE
#define INLINE
#endif

typedef struct _SYSFPART_
{
    MLOG_DWORD *storage;
    MLOG_DWORD *freeblk;
    MLOG_DWORD blksize;
    MLOG_DWORD blkcnt;
    MLOG_DWORD AllocCnt;
} SYSFPART, *PSYSFPART;

#endif


typedef struct tagMLogFHPart
{
    MLOG_DWORD Index;
    MLOG_DWORD Size;
    MLOG_DWORD StaticData;
    MLOG_DWORD DynamicData;
}MLOGFHPART;


typedef struct tagMLogFileHeader
{
    MLOG_DWORD Magic;
    MLOG_DWORD Version;
    MLOG_DWORD BasePointer;
    MLOG_DWORD PartNum;
} MLOGFILEHEADER;


typedef struct MLOG_FRAME_BLOCK_HEADER MLOG_FRAME_BLOCK_HEADER;

struct MLOG_FRAME_BLOCK_HEADER
{
    MLOG_FRAME_BLOCK_HEADER *next;
    MLOG_DWORD payload;
    MLOG_DWORD size;
    unsigned char *cur_data;
};

typedef struct _MLOG_STORAGE_HEADER
{
    SYSFPART mem_part;
    MLOG_DWORD mempart_size;
    unsigned char *    ptr;
    MLOG_FRAME_BLOCK_HEADER * static_block;
    MLOG_FRAME_BLOCK_HEADER * first_block;
    MLOG_FRAME_BLOCK_HEADER * current_block;
    MLOG_DWORD block_num;
    MLOG_DWORD tti_counter;
    MLOG_DWORD payload;
    MLOGFHPART *hdr;
}MLOG_STORAGE_HEADER;

typedef unsigned int (*write_fn) (unsigned int cpu, unsigned char *rec, unsigned int size);

typedef struct _MLOG_STORAGE_CTX
{
    MLOG_STORAGE_HEADER *storage[CPU_NUM + MLOG_DEVICE_COUNT];
    MLOG_DWORD tti_counter;
    MLOG_DWORD buff_num;
    unsigned char *        ptr;
    MLOGFILEHEADER *hdr;
    write_fn   wr_dyn[CPU_NUM];
    write_fn   wr_stat[CPU_NUM];
}MLOG_STORAGE_CTX;

typedef struct _MLOG_CTX_
{
            MLOG_DWORD      Opened;
            MLOG_DWORD      Finalized;
            MLOG_DWORD      Started;
            MLOG_DWORD      EnabledMaskProg;
            MLOG_DWORD      EnabledMask;
            MLOG_STORAGE_CTX *StoragePtr;
volatile    MLOG_DWORD      StorageSize;
volatile    MLOG_DWORD      Mlogtaskkeycounter;
            MLOG_DWORD      Mlogenablerc;
            unsigned char*  DevStoragePtr;
            unsigned int    MLogMarkTicks;
            unsigned int    MLogMarkId;
            unsigned int    MLogMarkFrameNumber;
            unsigned int    MLogMarkSubframeFrameNumber;
} MLOGCTX;

typedef struct tagMLogListRec
{
    MLOG_DWORD code;
    MLOG_DWORD listid;
    MLOG_DWORD ticks;
}MLOGLISTREC;

typedef struct tagMLogTCBRec
{
    MLOG_DWORD code;
    MLOG_DWORD ptcb;
    MLOG_DWORD taskID;
    MLOG_DWORD resID;
}MLOGTCBREC;

typedef struct tagMLogEXERec
{
    MLOG_DWORD code;
    MLOG_DWORD ptcb;
    MLOG_DWORD ticks;
    MLOG_DWORD resID;
    MLOG_DWORD execticks;
    MLOG_DWORD errcode;
}MLOGEXEREC;

typedef struct tagMLogSubTaskRec
{
    MLOG_DWORD code;
    MLOG_DWORD ptcb;
    MLOG_DWORD subID;
    MLOG_DWORD ticks;
}MLOGSUBTASKREC;

typedef struct tagMLogTaskRec
{
    MLOG_DWORD code;
    MLOG_DWORD taskID;
    MLOG_DWORD resID;
    MLOG_DWORD tickstart;
    MLOG_DWORD tickstop;
    MLOG_DWORD key;
}MLOGTASKREC;

typedef struct tagMLogFreqRec
{
    MLOG_DWORD code;
    MLOG_DWORD resID;
    MLOG_DWORD freqID;
    MLOG_DWORD freqval;
}MLOGFREQREC;

typedef struct tagMLogMarkRec
{
    MLOG_DWORD code;
    MLOG_DWORD markID;
    MLOG_DWORD ticks;
}MLOGMARKREC;


typedef struct tagMLogDepRec
{
    MLOG_DWORD mode;
    MLOG_DWORD param1;
    MLOG_DWORD param2;
}MLOGDEPREC;

typedef struct tagMLogTCBCntrRec
{
    MLOG_DWORD code;
    MLOG_DWORD ptcb;
    MLOG_DWORD markid;
}MLOGTCBCNTRREC;

typedef struct tagMLogResCondRec
{
    MLOG_DWORD id;
    MLOG_DWORD code;
    MLOG_DWORD resid_index;
    MLOG_DWORD ticks;
}MLOGRESCONDREC;

typedef struct tagMLogAddListRec
{
    MLOG_DWORD code;
    MLOG_DWORD ptcb;
    MLOG_DWORD listid;
}MLOGADDLISTREC;

typedef struct tagMLogDevInfoRec
{
    MLOG_DWORD code;
    MLOG_DWORD devinfo;
}MOGDEVINFOREC;

typedef struct tagMLogFrameSFRec
{
    MLOG_DWORD code;
    MLOG_DWORD framenum;
    MLOG_DWORD subframenum;
}MLOGFRAMESFREC;

typedef struct tagCacheMipsHeadRec
{
    MLOG_DWORD code;
    MLOG_DWORD ticks;
    MLOG_DWORD buffcount;
}MLOGCACHEMIPSHEADREC;

typedef struct tagCacheMipsRec
{
    MLOG_DWORD buffnum;
    MLOG_DWORD cachemiss;
    MLOG_DWORD cachehit;
    MLOG_DWORD irqcycles;
    MLOG_DWORD idlecycles;
    MLOG_DWORD supervisiorcycles;
    MLOG_DWORD irqcount;
}MLOGCACHEMIPSREC;

typedef struct tagCacheMipsSRec
{
    MLOG_DWORD buffnum;
    MLOG_DWORD cachemiss;
    MLOG_DWORD cachehit;
    MLOG_DWORD cachemissinst;
    MLOG_DWORD irqcycles;
    MLOG_DWORD idlecycles;
    MLOG_DWORD supervisiorcycles;
    MLOG_DWORD irqcount;
}MLOGCACHEMIPSSREC;

#ifdef MLOG_ENABLED

MLOGCTX * MLogGetCtx (void);
unsigned int MLogSetMask (MLOG_DWORD nMask);
unsigned int MLogSetMaskProg (MLOG_DWORD nMask);
unsigned int MLogActivateMaskProg (void);
MLOG_DWORD MLogGetMask (void);
unsigned int * MLogGetFileLocation(void);
unsigned int MLogGetFileSize(void);

int  MLogOpen(void);
int MLogClose(void);

void MLogTCB(void* ptcb);
void MLogEXEStart(void* ptcb,int resourcenum,unsigned int thisTicks);
void MLogEXEFinish(void* ptcb);
void MLogEXEFinish2(void* ptcb,int resourcenum);

void MLogEnableRC(unsigned int bVal);

void MLogFREQ(unsigned int resourceid, unsigned long freqvalue);
void MLogDevInfo(unsigned devInfo);
void MLogSubTask(void* ptcb_parent, unsigned int subtaskid, unsigned int ticks);
unsigned int MLogTask(unsigned int taskid, unsigned int resourceid , unsigned int ticksstart,unsigned int ticksstop);
unsigned int MLogTaskREX(unsigned int taskid, unsigned int resourceid , unsigned int ticksstart,unsigned int ticksstop);

void MLogMark(unsigned int markid,  unsigned int ticks);

#ifdef MLOG_ADD_DEPENDENCY
void MLogDepends(int mode,unsigned int param1,unsigned int param2);
#else
#define MLogDepends(a, b, c)
#endif


void MLogMarkTcbControl(void* ptcb, unsigned int markid);


void MLogTCBList(unsigned int listid);
void MLogTCBListStart(unsigned int listid);
void MLogTCBListStartEx(unsigned int listid, unsigned int cpu);
void MLogTCBListStop(unsigned int listid);
void MLogTCBAddToList(void* ptcb,unsigned int listid);

unsigned int MLogPrint(void);
unsigned int MLogFlush(void);

#ifdef MLOG_IRQ_SUP_ENABLE
void MLogResourceCondition(unsigned int code,unsigned int resid,unsigned int resindex);
#else
#define MLogResourceCondition(a, b, c)
#endif

void MLogRegisterFrameSubframe(unsigned int frameNum, unsigned int subFrameNum);
void MLogMarkAndRegisterFrameSubframe(unsigned int markid, unsigned int ticks, unsigned int frameNum, unsigned int subFrameNum);
void MLogAddVariables(unsigned int numVar, unsigned int *variables, unsigned int ticks);
void MLogCacheMipsStats(unsigned int ticks);
void MLogCacheMipsStatsSingle(unsigned int ticks);
void MLogCacheMipsStatsSingleEx(unsigned int ticks);

MLOG_FRAME_BLOCK_HEADER * MLogDevGetCurBlock(unsigned int devid);

#else
#define MLogGetCtx() 0
#define MLogSetMask(a)
#define MLogSetMaskProg(a)
#define MLogActivateMaskProg()
#define MLogGetMask() 0
#define MLogGetFileLocation() 0
#define MLogGetFileSize() 0

#define MLogOpen()
#define MLogClose()

#define MLogTCB(a)
#define MLogEXEStart(a, b, c)
#define MLogEXEFinish(a)
#define MLogEXEFinish2(a,b)

#define MLogEnableRC(bVal)

#define MLogFREQ(a, b)
#define MLogDevInfo(a)
#define MLogSubTask(a, b, c)
#define MLogTask(a, b, c, d) (0)
#define MLogTaskREX(a, b, c, d) (0)
#define MLogMark(a,b)
#define MLogDepends(a, b, c)


#define MLogTCBList(a)
#define MLogTCBListStart(a)
#define MLogTCBListStartEx(a,cpu)
#define MLogTCBListStop(a)
#define MLogTCBAddToList(a,b)

#define MLogPrint()
#define MLogFlush ()
//#define MLogReset(a)

#define MLogSetMarkers()
#define MLogSetResetCounter(a)
#define MLogIncrementCounter()


#define MLogResourceCondition(a, b, c)

#define MLogRegisterFrameSubframe(a,b)
#define MLogMarkAndRegisterFrameSubframe(markid, ticks, frameNum, subFrameNum)
#define MLogAddVariables(a, b, c)
#define MLogMarkTcbControl(ptcb, markid)
#define MLogCacheMipsStats(a)
#define MLogCacheMipsStatsSingle(a)

#define MLogGetCurBlock(a) NULL

#endif  // MLOG_ENABLED


#ifdef __cplusplus
}
#endif



#endif



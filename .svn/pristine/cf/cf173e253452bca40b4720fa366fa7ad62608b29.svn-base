#ifndef __MLOGCEVA_H__
#define __MLOGCEVA_H__

#define MLOG_DWORD unsigned int

#define MLOG_TRUE             1
#define MLOG_FALSE            0

#define MLOG_UINTSIZE       4

#define MLOG_GETTICK()   0
#define MLOG_CEVA_DEVICE 0

////////////////////////////////////////////
// define TCB Resource TYPE
////////////////////////////////////////////
#define RESOURCE_CEVA       3

#define MLOG_SIZE_LIST          4
#define MLOG_LIST               0x25000000
#define MLOG_ID_LIST            0x25000004

#define MLOG_LISTSTART          0x26000000
#define MLOG_ID_LISTSTART       0x26000004

#define MLOG_LISTSTOP           0x27000000
#define MLOG_ID_LISTSTOP        0x27000004

#define MLOG_SIZE_TASK          20
#define MLOG_TASK               0x22000000
#define MLOG_ID_TASK            0x22000014 // MLOG_TASK   & MLOG_SIZE_TASK

#define MLOG_SIZE_EXESTART      12
#define MLOG_EXESTART           0x0C000000
#define MLOG_ID_EXESTART        0x0C00000C // MLOG_EXESTART & MLOG_SIZE_EXESTART

#define MLOG_SIZE_EXEFINISH     16
#define MLOG_EXEFINISH          0x0D000000
#define MLOG_ID_EXEFINISH       0x0D000010 // MLOG_EXEFINISH & MLOG_SIZE_EXEFINISH

typedef struct MLOG_FRAME_BLOCK_HEADER MLOG_FRAME_BLOCK_HEADER;

struct MLOG_FRAME_BLOCK_HEADER
{
	MLOG_FRAME_BLOCK_HEADER *next;
	MLOG_DWORD payload;
	MLOG_DWORD size;
	unsigned char *cur_data;
};

typedef struct tagMLogTaskRec
{
	MLOG_DWORD code;
	MLOG_DWORD taskID;
	MLOG_DWORD resID;
	MLOG_DWORD tickstart;
	MLOG_DWORD tickstop;
	MLOG_DWORD key;
}MLOGTASKREC;

typedef struct tagMLogEXERec
{
	MLOG_DWORD code;
	MLOG_DWORD ptcb;
	MLOG_DWORD ticks;
	MLOG_DWORD resID;
	MLOG_DWORD execticks;
}MLOGEXEREC;

typedef struct tagMLogListRec
{
	MLOG_DWORD code;
	MLOG_DWORD listid;
	MLOG_DWORD ticks;
}MLOGLISTREC;

typedef struct tTCB
{
    //*****************<  HW BLOCK BEGIN  >*************************
    // CACHELINE-1
    struct tTCB *NextTcb;
    unsigned int TaskID;
    unsigned int ResourceID;
    volatile unsigned int Status;
    void (* cbDone)(struct tTCB *Tcb);
    void *ContextPtr;
    unsigned int ContextLen;
    void *IOControlPtr;

    // CACHELINE-2
    unsigned int IOControlLen;
    void *InputDataPtr;
    unsigned int InputDataLen;
    void *OutputDataPtr;
    unsigned int OutputDataLen;
    unsigned short ResourceIndex; //RIX is used to index which resource (ceva:0-9, MAP:0-9) are allocated to this tcb
    unsigned short GroupID; //GID super Tcb group ID , FFFF is reserved,
    void *DependencyList;

    // CACHELINE-3
    unsigned int ExecTicks;
    struct tTCB *NextSupTcb; //THIS IS THE LAST FIELD OF TCB//pointer to linked list of tasks belonging to same superTCB group
    //*****************<  HW BLOCK END  >*************************

    //new.. below are needed for scheduler
    unsigned int TaskPri; //task priority
    unsigned int TaskMIPS; //task mips used in calculating a PU load factor.
    struct tTCB *PrevTcb; //backward link for the task list
    void *SeedList;
    unsigned short isym; // symbol num to be used by new IFFT driver
    unsigned short SubTaskTimer; //to be set by PHY and used by new FFT driver
    unsigned int TaskControl; //bit field (TBD) that defines special handling for a tcb

} TCB, *PTCB;

extern MLOG_FRAME_BLOCK_HEADER * MLogDevGetCurBlock(unsigned int devid);

unsigned int MLogDevTask(MLOG_FRAME_BLOCK_HEADER * current_block, unsigned int taskid, unsigned int resourceid , unsigned int ticksstart,unsigned int ticksstop);
void MLogDevEXE(MLOG_FRAME_BLOCK_HEADER * current_block, void* ptcb,int resourcenum,int isstart,unsigned int ticks);
void MLogDevEXEStart(MLOG_FRAME_BLOCK_HEADER * current_block, void* ptcb,int resourcenum,unsigned int thisTicks);
void MLogDevEXEFinish(MLOG_FRAME_BLOCK_HEADER * current_block, void* ptcb);
void MLogDevEXEFinish2(MLOG_FRAME_BLOCK_HEADER * current_block, void* ptcb,int resourcenum);
void MLogDevTCBListPut(MLOG_FRAME_BLOCK_HEADER * current_block, unsigned int code,unsigned int listid);
void MLogDevTCBListPutEx(MLOG_FRAME_BLOCK_HEADER * current_block, unsigned int code,unsigned int listid, unsigned int cpu);
void MLogDevTCBList(MLOG_FRAME_BLOCK_HEADER * current_block, unsigned int listid);
void MLogDevTCBListStart(MLOG_FRAME_BLOCK_HEADER * current_block, unsigned int listid);
void MLogDevTCBListStartEx(MLOG_FRAME_BLOCK_HEADER * current_block, unsigned int listid, unsigned int cpu);

#endif


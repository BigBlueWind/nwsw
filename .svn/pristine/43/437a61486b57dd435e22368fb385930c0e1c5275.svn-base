#include "mlogceva.h"

#ifdef CEVA_PROJ
#include "global.h"
#endif

#include "tcb.h"
#include "rex_serv.h"

#define MLOG_DWORD unsigned int

#define MLOG_TRUE               1
#define MLOG_FALSE              0

#define MLOG_UINTSIZE           4

#ifdef CEVA_PROJ
#define MLOG_GETTICK()          GetBusTick()
#else
#define MLOG_GETTICK()          MxGetTicks()
#endif
#define MLOG_CEVA_DEVICE        0

////////////////////////////////////////////
// define TCB Resource TYPE
////////////////////////////////////////////
#define RESOURCE_CEVA           3

#define MLOG_SIZE_LIST          4
#define MLOG_LIST               0x25000000
#define MLOG_ID_LIST            0x25000004

#define MLOG_LISTSTART          0x26000000
#define MLOG_ID_LISTSTART       0x26000008

#define MLOG_LISTSTOP           0x27000000
#define MLOG_ID_LISTSTOP        0x27000008

#define MLOG_SIZE_SUBTASK       12
#define MLOG_SUBTASK            0x20000000
#define MLOG_ID_SUBTASK         0x2000000C // MLOG_SUBTASK & MLOG_SIZE_SUBTASK

#define MLOG_SIZE_TASK          20
#define MLOG_TASK               0x22000000
#define MLOG_ID_TASK            0x22000014 // MLOG_TASK   & MLOG_SIZE_TASK

#define MLOG_SIZE_EXESTART      12
#define MLOG_EXESTART           0x0C000000
#define MLOG_ID_EXESTART        0x0C00000C // MLOG_EXESTART & MLOG_SIZE_EXESTART

#define MLOG_SIZE_EXEFINISH     20
#define MLOG_EXEFINISH          0x0D000000
#define MLOG_ID_EXEFINISH       0x0D000014 // MLOG_EXEFINISH & MLOG_SIZE_EXEFINISH

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
    MLOG_DWORD errcode;
}MLOGEXEREC;

typedef struct tagMLogListRec
{
	MLOG_DWORD code;
	MLOG_DWORD listid;
	MLOG_DWORD ticks;
}MLOGLISTREC;

typedef struct tagMLogSubTaskRec
{
    MLOG_DWORD code;
    MLOG_DWORD ptcb;
    MLOG_DWORD subID;
    MLOG_DWORD ticks;
}MLOGSUBTASKREC;

char ceva_mlog_storage [7*1024] = {0};

unsigned int Mlogtaskkeycounter = 0x100000;
MLOG_FRAME_BLOCK_HEADER * MLogFramePtr = NULL;

int uart_printf(const char *fmt, ...);

void MLogDevInit (void)
{
	MLOG_FRAME_BLOCK_HEADER * pCur = (MLOG_FRAME_BLOCK_HEADER *)ceva_mlog_storage;

	MLogFramePtr = NULL;
	memset (ceva_mlog_storage, 0, sizeof (ceva_mlog_storage));

	// base initialization
	pCur->cur_data = ((unsigned char*)pCur + sizeof (MLOG_FRAME_BLOCK_HEADER));
	pCur->payload  = 0;
}

void MLogDevSetFrame(void * pFrame)
{
    MLOG_FRAME_BLOCK_HEADER * pCur = (MLOG_FRAME_BLOCK_HEADER *)ceva_mlog_storage;

    //uart_printf ("MLOG-SET: %x\r\n", pFrame);

    MlogDevUpload();

    MLogFramePtr = (MLOG_FRAME_BLOCK_HEADER *)pFrame;

    if (MLogFramePtr != NULL)
    {
        pCur->next = MLogFramePtr->next;
		pCur->size = MLogFramePtr->size;
    
        // base initialization
	    pCur->cur_data = ((unsigned char*)pCur + sizeof (MLOG_FRAME_BLOCK_HEADER));
    	pCur->payload  = 0;
    }
}

void MlogDevUpload (void)
{
    MLOG_FRAME_BLOCK_HEADER * pCur = (MLOG_FRAME_BLOCK_HEADER *)ceva_mlog_storage;

    if (MLogFramePtr != NULL && pCur->payload != 0)
    {
        // just to load header prepared
        // by ARM code
    
        pCur->next = MLogFramePtr->next;
		pCur->size = MLogFramePtr->size;
    
        // to correct pointer to the cur_data
        // local CEVA mem -> global DDR mem

        pCur->cur_data = ((unsigned char*)MLogFramePtr + sizeof (MLOG_FRAME_BLOCK_HEADER));
        pCur->cur_data += pCur->payload;

        // In this case we need to upload already prepared data
        // and to set a pointer to the new mlog device frame

        //uart_printf ("MLOG-UPLOAD: %x => %x (size=%d)\r\n", pCur, MLogFramePtr, pCur->payload + sizeof (MLOG_FRAME_BLOCK_HEADER));

        #ifdef CEVA_PROJ
            MemCpyToExt(MLogFramePtr, pCur, pCur->payload + sizeof (MLOG_FRAME_BLOCK_HEADER));
        #else
		    memcpy(MLogFramePtr, pCur, pCur->payload + sizeof (MLOG_FRAME_BLOCK_HEADER));
        #endif

		pCur->payload  = 0;
		pCur->cur_data = ((unsigned char*)pCur + sizeof (MLOG_FRAME_BLOCK_HEADER));
    }
}

unsigned int MLogDevTask(unsigned int taskid, unsigned int resourceid , unsigned int ticksstart,unsigned int ticksstop)
{
    unsigned int key=0;
    MLOGTASKREC taskrec;

    taskrec.code        = MLOG_ID_TASK;
    taskrec.taskID      = taskid;
    taskrec.resID       = resourceid;
    taskrec.tickstart   = ticksstart;
    taskrec.tickstop    = ticksstop;
    taskrec.key         = Mlogtaskkeycounter++;

    MLogDevWriteRecord((unsigned char *)&taskrec, sizeof(MLOGTASKREC));

    return key;
}

void MLogDevTaskEx(unsigned int taskid, unsigned int resId, unsigned int resIdx, unsigned int start,unsigned int stop, unsigned int key)
{
    MLOGTASKREC taskrec;

    taskrec.code        = MLOG_ID_TASK;
    taskrec.taskID      = taskid;
    taskrec.resID       = (resId & 0xFFFF)|(resIdx<<16);
    taskrec.tickstart   = start;
    taskrec.tickstop    = stop;
    taskrec.key         = key;

    MLogDevWriteRecord((unsigned char *)&taskrec, sizeof(MLOGTASKREC));
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL DOXYGEN_TO_DO
 *
 *
 *  @param  ptcb
 *  @param  resourcenum
 *  @param  isstart
 *  @param  ticks
 *
 *  @return none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogDevEXE(void* ptcb,int resourcenum,int isstart,unsigned int ticks)
{
    MLOGEXEREC exerec;

    exerec.code     = isstart ? MLOG_ID_EXESTART : MLOG_ID_EXEFINISH;
    exerec.ptcb     = (MLOG_DWORD)ptcb;
    exerec.ticks    = MLOG_GETTICK();
    exerec.resID    = resourcenum;
    exerec.errcode  = 0;
    
    if (isstart == MLOG_FALSE)
    {
        exerec.execticks = ((PTCB)ptcb)->ExecTicks;
        MLogDevWriteRecord((unsigned char *)&exerec, sizeof(MLOGEXEREC));
    }
    else
    {
        MLogDevWriteRecord((unsigned char *)&exerec, sizeof(MLOGEXEREC) - (2*MLOG_UINTSIZE));
    }
	
	ticks = ticks;
}

//-------------------------------------------------------------------------------------------
/** @brief This function logs the start of execution of the TCB. It is automatically called by
 *         the TrEx when it dispatches a TCB
 *
 *
 *  @param  ptcb          Pointer to the TCB that needs to be logged
 *  @param  resourcenum   Resource Index of the Resource being used. If Ceva 5 is used, then 5
 *  @param  thisTicks     Clock State at this point of time. This is found by reading the internal
 *                        clock register
 *
 *  @return none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogDevEXEStart(void* ptcb,int resourcenum,unsigned int thisTicks)
{
    MLogDevEXE(ptcb,resourcenum,MLOG_TRUE,thisTicks);
}

//-------------------------------------------------------------------------------------------
/** @brief This function logs the end of execution of the TCB. It is automatically called by
 *         the TrEx when it get interrupted by the resource when TCB completed
 *
 *
 *  @param   ptcb Pointer to the TCB that needs to be logged
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogDevEXEFinish(void* ptcb)
{
    MLogDevEXE(ptcb,(((PTCB) ptcb)->ResourceIndex),MLOG_FALSE,0); //pass 0 for ticks
}

//-------------------------------------------------------------------------------------------
/** @brief This function logs the end of execution of the TCB. It is automatically called by
 *         the TrEx when it get interrupted by the resource when TCB completed
 *
 *
 *  @param   ptcb Pointer to the TCB that needs to be logged
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogDevEXEFinishError(void* ptcb, unsigned int res, unsigned int err)
{
    MLOGEXEREC exerec;

    exerec.code     = MLOG_ID_EXEFINISH;
    exerec.ptcb     = (MLOG_DWORD)ptcb;
    exerec.ticks    = MLOG_GETTICK();
    exerec.resID    = res;
    exerec.execticks= ((PTCB)ptcb)->ExecTicks;
    exerec.errcode  = err;
    
    MLogDevWriteRecord((unsigned char *)&exerec, sizeof(MLOGEXEREC));
	
	err = err;
}

//-------------------------------------------------------------------------------------------
/** @brief This function logs the end of execution of the TCB. It is automatically called by
 *         the scheduler when it dispatches a TCB. This is used to log in the end of a TCB that
 *         is part of a SuperTCB. This is a special kind of TCB. It is automitically called by the
 *         TrEx when it get interrupted by the resource when TCB completed
 *
 *  @param   ptcb          Pointer to the TCB that needs to be logged
 *  @param   resourcenum   Resource Index of the Resource being used. If Ceva 5 is used, then 5
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogDevEXEFinish2(void* ptcb,int resourcenum)
{
    MLogDevEXE(ptcb,resourcenum,MLOG_FALSE,0); //pass 0 for ticks
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL DOXYGEN_TO_DO
 *
 *
 *  @param  code
 *  @param  listid
 *
 *  @return none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogDevTCBListPut(unsigned int code,unsigned int listid)
{
    MLOGLISTREC listrec;

    listrec.code = code;
    listrec.listid = listid;
	listrec.ticks = MLOG_GETTICK();

    if(code != MLOG_ID_LIST)
        MLogDevWriteRecord((unsigned char *)&listrec, sizeof(MLOGLISTREC));
    else
        MLogDevWriteRecord((unsigned char *)&listrec, sizeof(MLOGLISTREC));

}

void MLogDevTCBListPutEx(unsigned int code,unsigned int listid, unsigned int cpu)
{
    MLOGLISTREC listrec;

    listrec.code = code;
    listrec.listid = listid;
	listrec.ticks = MLOG_GETTICK();

    MLogDevWriteRecord((unsigned char *)&listrec, sizeof(MLOGLISTREC));
	
	cpu =  cpu;
}

//-------------------------------------------------------------------------------------------
/** @brief This function is used to register the creation of a TaskList. It is automatically
 *         by the TrEx when a new list is created
 *
 *
 *  @param   listid  ID of the List being created
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogDevTCBList(unsigned int listid)
{
    MLogDevTCBListPut(MLOG_ID_LIST, listid);
}

//-------------------------------------------------------------------------------------------
/** @brief This function is used to log in the time when a task list was started to execute.
 *         It is automatically called by the TrEx when a List start execution
 *
 *
 *  @param   listid  ID of the List being Executed
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogDevTCBListStart(unsigned int listid)
{
    MLogDevTCBListPut(MLOG_ID_LISTSTART, listid);
}

void MLogDevTCBListStartEx(unsigned int listid, unsigned int cpu)
{
    MLogDevTCBListPutEx(MLOG_ID_LISTSTART, listid, cpu);
}

//-------------------------------------------------------------------------------------------
/** @brief This functions helps in logging tasks that are within a TCB inside a resource.
 *         One can profile subtasks within a TCB using this approach to see how many cycles that
 *         a smaller task within a TCB took and optimize this function if needed. It is automatically
 *         called by the TrEx if the TCB has subtasks
 *
 *
 *  @param   ptcb_parent    Pointer to the TCB that needs to be logged
 *  @param   subtaskid      ID for the Subtask that needs to be profiled
 *  @param   ticks          Time of the task logged within the resource
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogDevSubTask(void* ptcb_parent, unsigned int subtaskid, unsigned int ticks)
{
    MLOGSUBTASKREC strec;

    strec.code = MLOG_ID_SUBTASK;
    strec.ptcb = (MLOG_DWORD)ptcb_parent;
    strec.subID = subtaskid;
    strec.ticks = ticks;
    MLogDevWriteRecord((unsigned char *)&strec, sizeof(MLOGSUBTASKREC));
}

unsigned int MLogDevWriteRecord(unsigned char *rec, unsigned int size)
{
    MLOG_FRAME_BLOCK_HEADER * current_block = (MLOG_FRAME_BLOCK_HEADER *)ceva_mlog_storage;

	if(!current_block)
		return (unsigned int)-1;

    if(current_block->payload + size <= current_block->size)
    {
        memcpy(current_block->cur_data, rec, size);
        current_block->cur_data += size;
        current_block->payload += size;
    }
    else
    {
        RexServSendErrorEx(REX_SERV_RC_MLOG_OVERFLOW, current_block->payload + size);
        return (unsigned int)-1;
    }

    return 0;
}

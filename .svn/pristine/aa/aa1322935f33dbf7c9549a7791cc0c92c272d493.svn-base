#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "mlogceva.h"

unsigned int Mlogtaskkeycounter = 0x100000;

unsigned int MLogDevWriteRecord(MLOG_FRAME_BLOCK_HEADER * current_block, unsigned char *rec, unsigned int size);

unsigned int MLogDevTask(MLOG_FRAME_BLOCK_HEADER * current_block, unsigned int taskid, unsigned int resourceid , unsigned int ticksstart,unsigned int ticksstop)
{
    unsigned int key=0;
    MLOGTASKREC taskrec;

    taskrec.code = MLOG_ID_TASK;
    taskrec.taskID = taskid;
    taskrec.resID = resourceid;
    taskrec.tickstart = ticksstart;
    taskrec.tickstop = ticksstop;
    taskrec.key= Mlogtaskkeycounter++;

    MLogDevWriteRecord(current_block, (unsigned char *)&taskrec, sizeof(MLOGTASKREC));

    return key;
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
void MLogDevEXE(MLOG_FRAME_BLOCK_HEADER * current_block, void* ptcb,int resourcenum,int isstart,unsigned int ticks)
{
    MLOGEXEREC exerec;

    exerec.code = isstart ? MLOG_ID_EXESTART : MLOG_ID_EXEFINISH;
    exerec.ptcb = (MLOG_DWORD)ptcb;
    exerec.ticks = MLOG_GETTICK();
    exerec.resID = resourcenum;
    if (isstart == MLOG_FALSE)
    {
        exerec.execticks = ((PTCB)ptcb)->ExecTicks;
        MLogDevWriteRecord(current_block, (unsigned char *)&exerec, sizeof(MLOGEXEREC));
    }
    else
    {
        MLogDevWriteRecord(current_block, (unsigned char *)&exerec, sizeof(MLOGEXEREC) - MLOG_UINTSIZE);
    }
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
void MLogDevEXEStart(MLOG_FRAME_BLOCK_HEADER * current_block, void* ptcb,int resourcenum,unsigned int thisTicks)
{
    MLogDevEXE(current_block, ptcb,resourcenum,MLOG_TRUE,thisTicks);
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
void MLogDevEXEFinish(MLOG_FRAME_BLOCK_HEADER * current_block, void* ptcb)
{
    MLogDevEXE(current_block, ptcb,(((PTCB) ptcb)->ResourceIndex),MLOG_FALSE,0); //pass 0 for ticks
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
void MLogDevEXEFinish2(MLOG_FRAME_BLOCK_HEADER * current_block, void* ptcb,int resourcenum)
{
    MLogDevEXE(current_block, ptcb,resourcenum,MLOG_FALSE,0); //pass 0 for ticks
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
void MLogDevTCBListPut(MLOG_FRAME_BLOCK_HEADER * current_block, unsigned int code,unsigned int listid)
{
    MLOGLISTREC listrec;

    listrec.code = code;
    listrec.listid = listid;
	listrec.ticks = MLOG_GETTICK();

    if(code != MLOG_ID_LIST)
        MLogDevWriteRecord(current_block, (unsigned char *)&listrec, sizeof(MLOGLISTREC));
    else
        MLogDevWriteRecord(current_block, (unsigned char *)&listrec, sizeof(MLOGLISTREC));

}

void MLogDevTCBListPutEx(MLOG_FRAME_BLOCK_HEADER * current_block, unsigned int code,unsigned int listid, unsigned int cpu)
{
    MLOGLISTREC listrec;

    listrec.code = code;
    listrec.listid = listid;
	listrec.ticks = MLOG_GETTICK();

    MLogDevWriteRecord(current_block, (unsigned char *)&listrec, sizeof(MLOGLISTREC));
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
void MLogDevTCBList(MLOG_FRAME_BLOCK_HEADER * current_block, unsigned int listid)
{
    MLogDevTCBListPut(current_block, MLOG_ID_LIST, listid);
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
void MLogDevTCBListStart(MLOG_FRAME_BLOCK_HEADER * current_block, unsigned int listid)
{
    MLogDevTCBListPut(current_block, MLOG_ID_LISTSTART, listid);
}

void MLogDevTCBListStartEx(MLOG_FRAME_BLOCK_HEADER * current_block, unsigned int listid, unsigned int cpu)
{
    MLogDevTCBListPutEx(current_block, MLOG_ID_LISTSTART, listid, cpu);
}


unsigned int MLogDevWriteRecord(MLOG_FRAME_BLOCK_HEADER * current_block, unsigned char *rec, unsigned int size)
{
	if(!current_block)
		return (unsigned int)-1;

    if(current_block->payload + size <= current_block->size)
    {
        memcpy(current_block->cur_data, rec, size);
        current_block->cur_data += size;
        current_block->payload += size;
    }
    else
        return (unsigned int)-1;

    return 0;
}
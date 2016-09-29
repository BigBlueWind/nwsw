#ifndef __MLOGCEVA_H__
#define __MLOGCEVA_H__

void MLogDevInit (void);

unsigned int MLogDevTask(unsigned int taskid, unsigned int resourceid , unsigned int ticksstart,unsigned int ticksstop);
void MLogDevTaskEx(unsigned int taskid, unsigned int resId, unsigned int resIdx, unsigned int start,unsigned int stop, unsigned int key);
void MLogDevEXE(void* ptcb,int resourcenum,int isstart,unsigned int ticks);
void MLogDevEXEStart(void* ptcb,int resourcenum,unsigned int thisTicks);
void MLogDevEXEFinish(void* ptcb);
void MLogDevEXEFinishError(void* ptcb, unsigned int res, unsigned int err);
void MLogDevEXEFinish2(void* ptcb,int resourcenum);
void MLogDevTCBListPut(unsigned int code,unsigned int listid);
void MLogDevTCBListPutEx(unsigned int code,unsigned int listid, unsigned int cpu);
void MLogDevTCBList(unsigned int listid);
void MLogDevTCBListStart(unsigned int listid);
void MLogDevTCBListStartEx(unsigned int listid, unsigned int cpu);
void MLogDevSubTask(void* ptcb_parent, unsigned int subtaskid, unsigned int ticks);
unsigned int MLogDevWriteRecord(unsigned char *rec, unsigned int size);

void MLogDevSetFrame(void * pFrame);
void MlogDevUpload (void);

#endif


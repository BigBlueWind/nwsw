
#ifndef _PWAV_MEM_LEAK_DET_
#define _PWAV_MEM_LEAK_DET_

#include <stdio.h>
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "log.h"
/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common library function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_mblk.x"       /* Common memory allocation        */


#define MEM_INFO_LIST_ENTRY_FREE 0
#define MEM_INFO_LIST_ENTRY_USED 1
#define MEM_INFO_LIST_SIZE 5000
#define MAX_FILE_NAME_SIZE 100
#define NUM_MEM_LEAK_DETECT_BINS 32
#define GET_MEMLEAKHASHCP() &memLkDtcHashListCp

#ifndef CM_MT_HASH_BIN
   CmListEnt ghl[NUM_MEM_LEAK_DETECT_BINS];                  /* pointer to hash list bins */
#else
   CmListBinEnt ghl[NUM_MEM_LEAK_DETECT_BINS];               /* pointer to hash list bins */
#endif

typedef struct s_MemLeakInfo
{
  U32         alloctdPtr;  
  U32         memReqSize;
  U32         memAllocSize;
  U32         lineNumber;
  unsigned char  fileName[MAX_FILE_NAME_SIZE];
  unsigned char  entryStatus;  
  U32            ageCounter;
} MemAllocInfo;

extern CmHashListCp memLkDtcHashListCp;
extern MemAllocInfo   memInfoList[MEM_INFO_LIST_SIZE];

void  cmMemLeakDet_initMemInfoList();

MemAllocInfo* cmMemLeakDet_getFreeMemInfoListEntry();

void cmMemLeakDet_fillMemInfoStruct(MemAllocInfo* memAllocInfo,
                       U32 key,
                       U32 memReqSize,
                       U32 memAllocSize,
                       U32 lineNumber,
                       unsigned char* fileName);

void cmMemLeakDet_deleteMemInfoListEntry(MemAllocInfo* memAllocInfo);
void cmMemLeakDet_deleteMemInfoArrayEntry(U32 key, U32 memAllocSize, U32 lineNumber, U8 *fileName);
void cmMemLeakDet_printMemAllocInfo();
void cmMemLeakDet_incrementAgeCounterForUsedEntries();
S16 cmMemLeakDet_initHashlist(CmHashListCp *cp, U16 nmbBins);

S16 cmMemLeakDet_insHashEle(U32 key,
                                   U32 memReqSize,
                                   U32 memAllocSize,
                                   U32 lineNumber,
                                   U8  *fileName);

PUBLIC S16 cmMemLeakDet_delHashEle(U32 key, U32 memAllocSize, U32 lineNumber, U8 *fileName);


#endif

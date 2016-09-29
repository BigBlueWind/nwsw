#include "pwav_memlkdtec.h"

MemAllocInfo   memInfoList[MEM_INFO_LIST_SIZE];
CmHashListCp memLkDtcHashListCp;
U32 NumberOfAllocations;
U32 NumberOfDeletions;
U8  listEntriesExhausted;
U8 ageCounter;

void cmMemLeakDet_initMemInfoList()
{
   listEntriesExhausted = 0;
   ageCounter = 0;
   memset(memInfoList,0,(size_t)(sizeof(MemAllocInfo)*MEM_INFO_LIST_SIZE));
   return ;
}


MemAllocInfo* cmMemLeakDet_getFreeMemInfoListEntry()
{
    U32 listCntr;

    for(listCntr=0;listCntr<MEM_INFO_LIST_SIZE;listCntr++)
    {
        if(MEM_INFO_LIST_ENTRY_FREE == memInfoList[listCntr].entryStatus)
        {
            memInfoList[listCntr].entryStatus = MEM_INFO_LIST_ENTRY_USED;
            return (&memInfoList[listCntr]);
        }
    }

    if(!listEntriesExhausted)
    logError("No Free entry is available in the FreeList %d", MEM_INFO_LIST_SIZE);
    listEntriesExhausted = 1;
    return NULL;
}

void cmMemLeakDet_printMemAllocInfo()
{
    U32 listCntr;
    U32 totalUsedEntries = 0;

    ageCounter++;

    logError("%s", "----------START: Current Memory Alloc Info ------------");
    logError("%s" , "Alloctd PTR |  RSize | ASize | File | Line | AgeCounter \n");

    for(listCntr=0;listCntr<MEM_INFO_LIST_SIZE;listCntr++)
    {
        if(MEM_INFO_LIST_ENTRY_USED == memInfoList[listCntr].entryStatus)
        {
            logError(" %p  |  %d  |  %d |  %s | %d | %d", \
             memInfoList[listCntr].alloctdPtr, memInfoList[listCntr].memReqSize, \
             memInfoList[listCntr].memAllocSize, memInfoList[listCntr].fileName, \
             memInfoList[listCntr].lineNumber, memInfoList[listCntr].ageCounter);

             totalUsedEntries++;
             //memInfoList[listCntr].ageCounter++;
        }
    }
    logError("----------END: Current Memory Alloc Info (%d entries) ------------", totalUsedEntries);
}
void cmMemLeakDet_fillMemInfoStruct(MemAllocInfo* memAllocInfo,
                       U32 key,
                       U32 memReqSize,
                       U32 memAllocSize,
                       U32 lineNumber,
                       unsigned char* fileName)
{
    memAllocInfo->alloctdPtr = key;
    memAllocInfo->memReqSize = memReqSize;
    memAllocInfo->memAllocSize = memAllocSize;
    memAllocInfo->lineNumber = lineNumber;
    strcpy((char*)&memAllocInfo->fileName, (const char*)fileName);
    memAllocInfo->ageCounter = ageCounter;

    return ;
}


void cmMemLeakDet_deleteMemInfoListEntry(MemAllocInfo* memAllocInfo)
{
    memset(memAllocInfo, 0, sizeof(MemAllocInfo));

    return ;
}


void cmMemLeakDet_deleteMemInfoArrayEntry(U32 key, U32 memAllocSize, U32 lineNumber, U8 *fileName)
{
    U32 listCntr;

    for(listCntr=0;listCntr<MEM_INFO_LIST_SIZE;listCntr++)
    {
        if(key == memInfoList[listCntr].alloctdPtr)
        {
            memset(&memInfoList[listCntr], 0, sizeof(MemAllocInfo));
            /*if(memInfoList[listCntr].memAllocSize != memAllocSize)
            {
                logError("Size mismatches for the memory: Allocatd:%d - Freed:%d" \
                ,memInfoList[listCntr].memAllocSize, memAllocSize);
            }*/
            return;
        }
    }
    if(!listEntriesExhausted)
      logError("Entry not found: Key %x | %s:%d ",key, fileName, lineNumber);
    return ;
}


void cmMemLeakDet_incrementAgeCounterForUsedEntries()
{
    U32 listCntr;

    for(listCntr=0;listCntr<MEM_INFO_LIST_SIZE;listCntr++)
    {
       if(memInfoList[listCntr].entryStatus == MEM_INFO_LIST_ENTRY_USED)
       {
           memInfoList[listCntr].ageCounter++;
       }
    }
    return ;
}
PUBLIC S16 cmMemLeakDet_initHashlist(CmHashListCp *cp, U16 nmbBins)
{
#ifndef CM_MT_HASH_BIN
   CmListEnt *hl;
#else
   CmListBinEnt *hl;
#endif
   U32 i = 0;

   cmHashListInit(cp, 0, 0, FALSE, CM_HASH_KEYTYPE_U32MOD, 0, 0); 

   /* initialize bin pointers */
   hl = cp->hl = &ghl;
   
   for(i = 0; i < nmbBins; i++)
   {

#ifndef CM_MT_HASH_BIN
      hl[i].next = hl[i].prev = &hl[i];
#else
      /* This initialization is being done as a part of checking 
       * the presence of empty/non-empty bins. 
       */
      hl[i].next = hl[i].prev = (CmListEnt*)&hl[i];

      hl[i].nmbEnt = 0;
#endif
   }

   /* initialize bin size */
   cp->nmbBins = nmbBins;

   /* initialize bin bit mask */
   if (((nmbBins) & (nmbBins - 1)) == 0)
   {
      U16   binBitMask;

      /* number of bins is a power of 2 */
      cp->binBitMask = nmbBins - 1;

      /* compute number of bits in the bit mask */
      for (binBitMask = cp->binBitMask; binBitMask; binBitMask >>= 1)
         cp->nmbBinBits++;

   }

   return ;

}

PUBLIC S16 cmMemLeakDet_insHashEle(U32 key,
                                   U32 memReqSize,
                                   U32 memAllocSize,
                                   U32 lineNumber,
                                   U8  *fileName)
{
  MemAllocInfo* memAllocInfo = NULL;
  //CmHashListCp *hashListCp = GET_MEMLEAKHASHCP();
  memAllocInfo = cmMemLeakDet_getFreeMemInfoListEntry();
  if ( memAllocInfo == NULL)
  {
    return RFAILED;
  }

 // cmMemLeakDet_incrementAgeCounterForUsedEntries();
  cmMemLeakDet_fillMemInfoStruct(memAllocInfo,
                                 key,
                                 memReqSize,
                                 memAllocSize,
                                 lineNumber,
                                 fileName);
  //cmHashListInsert(hashListCp, memAllocInfo, &key, sizeof(U32));
 // logError("\n Number of Allocations: %d", NumberOfAllocations++);
   
  return ROK;
}

PUBLIC S16 cmMemLeakDet_delHashEle(U32 key, U32 memAllocSize, U32 lineNumber, U8 *fileName)
{
//  MemAllocInfo *memAllocInfo = NULL;
  /*CmHashListCp *hashListCp = GET_MEMLEAKHASHCP();

  if (ROK == cmHashListFind(hashListCp, &key, sizeof(U32), 0, (PTR *)&memAllocInfo))
  {
     cmHashListDelete(hashListCp, memAllocInfo);
     if(memAllocInfo->memAllocSize != memAllocSize)
     {
       logError("Size mismatches for the memory: Allocatd:%d - Freed:%d",memAllocInfo->memAllocSize, memAllocSize);
     }
     cmMemLeakDet_deleteMemInfoListEntry(memAllocInfo);
     
  logError("\n Number of Deletions: %d", NumberOfDeletions++);
  }*/
  cmMemLeakDet_deleteMemInfoArrayEntry(key,memAllocSize, lineNumber, fileName);
  //logError("\n Number of Deletions: %d", NumberOfDeletions++);
  return ROK;
}

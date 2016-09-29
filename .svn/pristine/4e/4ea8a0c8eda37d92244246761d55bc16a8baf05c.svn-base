/* Memory allocation and free recording */
#include <stdio.h>    /* printf */
#include <stdlib.h>   /* calloc */
#include <string.h>   /* memset */
#include "pm_mem.h"

/** MACROS **/
/* For logging errors */
#define PM_ERROR(format, ...) printf("PM | ER |"format"\n", __VA_ARGS__)

#define PM_INFO(format, ...)  printf("PM | IF |"format"\n", __VA_ARGS__)

#define pmIsTableFull(tab) (tab->nAlloc < tab->maxElem ? 0 : 1)

#define PM_ALLOC(tab, entry) /* \
  PM_INFO(" AL | tab=%p | tabIdx=%d | entryIdx=%d | ptr=%p | size=%d | file=%s | line=%d", tab, tab->index, entry->index, entry->ptr, entry->size, entry->alloc.file, entry->alloc.line)
*/
#define PM_FREE(tab, ptr, size, file, line) /* \
  PM_INFO(" FR | tab=%p | idx=%d | ptr=%p | size=%d | file=%s | line=%d", tab, tab->index, ptr, size, file, line) 
*/
#define PM_ERROR_TAB(errStr, tab)\
  PM_ERROR(" tab=%p | max=%d | nAlloc=%d | idx=%d | nAllocReq=%d | nAllocFail=%d | nFreeFail=%d | err=%s", tab, tab->maxElem, tab->nAlloc, tab->index, tab->nAllocReq, tab->nAllocFail, tab->nFreeFail, errStr)

/** Globals **/
PmTable g_blockBufTable;
PmTable g_staticBufTable;
PmTable g_msgBufTable;

/** Functions **/

/* Record that a ptr has been allocated */
void pmRecordBufAlloc(
  PmTable   *tab, 
  void      *ptr, 
  int        size, 
  char      *file, 
  int        line)
{
  
  pthread_mutex_lock(&tab->mutex);

  int           found = 0;
  int           i     = 0;
  PmTableEntry *entry = &(tab->elem[tab->index]);

  tab->nAllocReq++;
  if(pmIsTableFull(tab))
  {
    tab->nAllocFail++;
    goto end;
  }
  while( i < tab->maxElem ) /* search all starting from current */
  {
    if(1 == entry->isAllocated) 
    {
      i++;
      entry = entry->next;
    }
    else
    {
      tab->nAlloc++;
      found              = 1;
      entry->isAllocated = 1;
      entry->ptr         = ptr;
      entry->size        = size;
      entry->alloc.file  = file;
      entry->alloc.line  = line;
      PM_ALLOC(tab, entry);
      break;
    }
  }
  if(!found) PM_ERROR_TAB("ProgError|NoFreeElementFound", tab); 
  if(pmIsTableFull(tab)) PM_ERROR_TAB("TabFull", tab);

  end:
  pthread_mutex_unlock(&tab->mutex);
}


void pmRecordBufFree(
  PmTable         *tab, 
  const void      *ptr, 
  int              size, /* not used */ 
  char            *file, 
  int              line)
{
  pthread_mutex_lock(&tab->mutex);
  tab->nFreeReq++;

  int           i        = 0;
  int           found    = 0;
  PmTableEntry *entry    = &(tab->elem[tab->index]);

  while(i < tab->maxElem )
  {
    if(entry->ptr != ptr) 
    {
      i++;
      entry = entry->prev; /* search backward */
    }
    else
    {
      found              = 1;
      entry->isAllocated = 0;
      entry->ptr         = NULL;
      entry->size        = 0;
      entry->free.file   = file;
      entry->free.line   = line;
      tab->nAlloc--;
      tab->index = entry->index; /* this one is available/free */
      PM_FREE(tab, ptr, size, file, line);
      break;
    }
  }
  if(!found)
  {
    tab->nFreeFail++;
    PM_ERROR("Free | NotFound | tab=%p | ptr=%p | size=%d | file=%s | line=%d", tab, ptr, size, file, line);
  }
  pthread_mutex_unlock(&tab->mutex);
}

/* Prints all allocated buffers in a table 
 * printAll !=0 => print all
 * printAll ==0 => print allocated
 */
void printPmTable(PmTable *tab, int printAll)
{
  int i;
  if(tab->isEnabled == 1)
  {
    printf("\n[--- %s tab=%p max=%d nAlloc=%d nAllocReq=%d nAllocFail=%d nFreeReq=%d nFreeFail=%d idx=%d ---]\n", 
          tab->name, 
          tab, 
          tab->maxElem, 
          tab->nAlloc, 
          tab->nAllocReq,
          tab->nAllocFail,
          tab->nFreeReq,
          tab->nFreeFail,
          tab->index
        );
   }
   else
   {
     printf("\nmemLeak detection flag is OFF for %s\n",tab->name);
   }

  if (printAll)
    printf("idx | ptr | isAlloc | size | alloc.file | alloc.line | free.file | free.line\n");
  else
    printf("idx | ptr | isAlloc | size | alloc.file | alloc.line\n");

  for ( i=0; i < tab->maxElem ; i++ )
  {
    if (printAll)
     printf(" %d | %p | %d | %d | %s | %d | %s | %d\n",
       i,
       tab->elem[i].ptr,
       tab->elem[i].isAllocated,
       tab->elem[i].size,
       tab->elem[i].alloc.file,
       tab->elem[i].alloc.line,
       tab->elem[i].free.file,
       tab->elem[i].free.line);
    else if (tab->elem[i].isAllocated) {
     printf(" %d | %p | %d | %d | %s | %d\n",
       i,
       tab->elem[i].ptr,
       tab->elem[i].isAllocated,
       tab->elem[i].size,
       tab->elem[i].alloc.file,
       tab->elem[i].alloc.line);
    }
  }
  printf("--  -- end tab=%p  --  --\n", tab);
}

/** initialises the table by setting the next and prev pointers */

void pmInitCircularList(PmTable *tab, char *name, int maxElem)
{
  int i;
  memset(tab, 0, sizeof(PmTable));
 
  memset(&tab->mutex, 0, sizeof(pthread_mutex_t));

  pthread_mutex_lock(&tab->mutex);

  snprintf(&(tab->name[0]), sizeof(tab->name)-1, "%s", name);

  tab->maxElem = maxElem;
  tab->elem = calloc(maxElem, sizeof(PmTableEntry));
  if(!tab->elem) {
    printf("FATAL | could not allocate memory for %s | maxElem=% * size=%d", name, maxElem, sizeof(PmTableEntry));
    exit(0);
  }
  /* Set it for all.
   * The prev of first and next of last will be incorrect; corrected after for loop.
   */
  for(i=0; i < tab->maxElem; i++)
  {
    tab->elem[i].index = i;
    tab->elem[i].next  = &(tab->elem[i+1]);
    tab->elem[i].prev  = &(tab->elem[i-1]);
  }
  /* Prev of first if last.
   * Next of last is first.
   */
  tab->elem[0].prev                 = &(tab->elem[tab->maxElem - 1]);
  tab->elem[tab->maxElem - 1].next  = &(tab->elem[0]);

  pthread_mutex_unlock(&tab->mutex);

}

/** For unit testing this utility */
#if TEST_PM
#define PRINT_INT(int1) printf("%s=%d\n", #int1, int1)
#define START_TEST(num, purpose) printf("Test %d |%s\n", num, purpose)
#define CHECK_INT(cond) printf("  %s|%s\n", (cond ? "OK" : "FAIL"), #cond);
#define FAIL_UNLESS_EQUAL(int1, int2) printf("%s|%s(=%d) == %s(=%d)", (int1 == int2 ? "OK" : "FAIL"), #int1, int1, #int2, int2);
int main (void)
{

  char c, d, e;
  PmTable *tab = &g_blockBufTable;

  { /* test 1 */
    START_TEST(1, "circ list is initialised correctly");
    pmInitCircularList(tab, "block", 3);
    CHECK_INT(tab->maxElem == 3);
    CHECK_INT(tab->nAlloc == 0);
    CHECK_INT(tab->index  == 0);
    CHECK_INT(tab->nAllocReq  == 0);
    CHECK_INT(tab->nAllocFail  == 0);
    CHECK_INT(tab->nFreeReq  == 0);
    CHECK_INT(tab->nFreeFail  == 0);
    CHECK_INT(tab->maxElem  == 3);
  }

  { /* test 2 */
    START_TEST(1, "Add first elem to circ list");
    pmRecordBufAlloc(tab, &c, 256, __FILE__, 100);
    CHECK_INT(tab->nAlloc == 1);
    CHECK_INT(tab->index  == 0); /* unchanged */
    CHECK_INT(tab->nAllocReq  == 1);
    CHECK_INT(tab->nAllocFail  == 0);
    CHECK_INT(tab->nFreeReq  == 0);
    CHECK_INT(tab->nFreeFail  == 0);
    CHECK_INT(tab->elem[0].isAllocated == 1);
    CHECK_INT(tab->elem[0].ptr == &c);
    CHECK_INT(tab->elem[0].size == 256);
    CHECK_INT(strcmp(tab->elem[0].alloc.file, __FILE__) == 0);
    CHECK_INT(tab->elem[0].alloc.line == 100);
  }
  { /* test 3 */
    START_TEST(3, "Remove first elem from circ list");
    pmRecordBufFree(tab, &c, 256, __FILE__, 120);
    CHECK_INT(tab->nAlloc == 0);
    CHECK_INT(tab->index  == 0); /* should be set to 0 as free happended here */
    CHECK_INT(tab->nAllocReq  == 1);
    CHECK_INT(tab->nAllocFail  == 0);
    CHECK_INT(tab->nFreeReq  == 1);
    CHECK_INT(tab->nFreeFail  == 0);
    CHECK_INT(tab->elem[0].isAllocated == 0);
    CHECK_INT(tab->elem[0].ptr == NULL);
    CHECK_INT(tab->elem[0].size == 0);
    CHECK_INT(strcmp(tab->elem[0].alloc.file, __FILE__) == 0);
    CHECK_INT(tab->elem[0].alloc.line == 100);
    CHECK_INT(strcmp(tab->elem[0].free.file, __FILE__) == 0);
    CHECK_INT(tab->elem[0].free.line == 120);
  }
  { /* test 4 */
    START_TEST(4, "Add 3 elems, should get added to index 0, 1, 2");
    pmRecordBufAlloc(tab, &c, 256,  "a.c", 120);
    pmRecordBufAlloc(tab, &d, 123,  "b.c", 112233);
    pmRecordBufAlloc(tab, &e, 2345, "a.c", 1210);
    CHECK_INT(tab->nAlloc == 3);
    CHECK_INT(tab->index  == 0); /*  unchanged  */
    CHECK_INT(tab->nAllocReq  == 4);
    CHECK_INT(tab->nAllocFail  == 0);
    CHECK_INT(tab->nFreeReq  == 1);
    CHECK_INT(tab->nFreeFail  == 0);
    CHECK_INT(tab->elem[2].isAllocated == 1);
    CHECK_INT(tab->elem[2].ptr == &e);
    CHECK_INT(tab->elem[2].size == 2345);
    CHECK_INT(strcmp(tab->elem[2].alloc.file, "a.c") == 0);
    CHECK_INT(tab->elem[2].alloc.line == 1210);
    //CHECK_INT(strcmp(tab->elem[2].free.file, NULL) == 0);
    //CHECK_INT(tab->elem[2].free.line == 0);
  }
  { /* test 5 */
    START_TEST(5, "Add elem to full table should fail but be counted");
    pmRecordBufAlloc(tab, &c, 256,  "a.c", 120);
    CHECK_INT(tab->nAlloc == 3);
    CHECK_INT(tab->index  == 0); /*  should stay unchanged */
    CHECK_INT(tab->nAllocReq  == 5);
    CHECK_INT(tab->nAllocFail  == 1);
    CHECK_INT(tab->nFreeReq  == 1); /* unchanged */
    CHECK_INT(tab->nFreeFail  == 0); /* unchanged */
  }
  { /* test 6 */
    START_TEST(6, "Delete middle elem");
    CHECK_INT(tab->elem[1].ptr == &d);
    pmRecordBufFree(tab, &d, 123,  "x.c", 11);
    CHECK_INT(tab->nAlloc == 2);
    CHECK_INT(tab->index  == 1); /*  change to this index */
    CHECK_INT(tab->nAllocReq  == 5); /* unchanged */
    CHECK_INT(tab->nAllocFail  == 1); /* unchanged */
    CHECK_INT(tab->nFreeReq  == 2); /* incremented */
    CHECK_INT(tab->nFreeFail  == 0); /* unchanged */
    CHECK_INT(tab->elem[1].isAllocated == 0); 
    CHECK_INT(tab->elem[1].ptr == NULL);
    CHECK_INT(tab->elem[1].size == 0);
    CHECK_INT(strcmp(tab->elem[1].alloc.file, "b.c") == 0);
    CHECK_INT(tab->elem[1].alloc.line == 112233);
    CHECK_INT(strcmp(tab->elem[1].free.file, "x.c") == 0);
    CHECK_INT(tab->elem[1].free.line == 11);
  }
  { /* test 7 */
    void *p = 0x2345678;
    START_TEST(7, "New elem should get added in middle");
    CHECK_INT(tab->elem[1].ptr == NULL);
    pmRecordBufAlloc(tab, p, 123,  "y.c", 101);
    CHECK_INT(tab->nAlloc == 3); /* increment */
    CHECK_INT(tab->index  == 1); /*  unchanged */
    CHECK_INT(tab->nAllocReq  == 6); /* incremented */
    CHECK_INT(tab->nAllocFail  == 1); /* unchanged */
    CHECK_INT(tab->nFreeReq  == 2); /* unchanged */
    CHECK_INT(tab->nFreeFail  == 0); /* unchanged */
    CHECK_INT(tab->elem[1].isAllocated == 1); 
    CHECK_INT(tab->elem[1].ptr == p);
    CHECK_INT(tab->elem[1].size == 123);
    CHECK_INT(strcmp(tab->elem[1].alloc.file, "y.c") == 0);
    CHECK_INT(tab->elem[1].alloc.line == 101);
    CHECK_INT(strcmp(tab->elem[1].free.file, "x.c") == 0); /* unchanged */
    CHECK_INT(tab->elem[1].free.line == 11); /* unchanged */
  }
  { /* test 8 */
    void *p = 0x2345678;
    START_TEST(8, "Del an elem twice");
    pmRecordBufFree(tab, p, 123,  "y.c", 101);
    pmRecordBufFree(tab, p, 123,  "y.c", 101);
    CHECK_INT(tab->nAlloc == 2); /* increment */
    CHECK_INT(tab->index  == 1); /*  unchanged */
    CHECK_INT(tab->nAllocReq  == 6); /* unchanged */
    CHECK_INT(tab->nAllocFail  == 1); /* unchanged */
    CHECK_INT(tab->nFreeReq  == 4); /* incremented by 2 */
    CHECK_INT(tab->nFreeFail  == 1); /* since 1 was not found */
    CHECK_INT(tab->elem[1].isAllocated == 0); 
  }
  /* Reset */
  pmInitCircularList(tab, "block", 3);
  { /* test 9 */
    START_TEST(9, "Add same elem twice");
    pmRecordBufAlloc(tab, &c, 123,  "y.c", 101);
    pmRecordBufAlloc(tab, &c, 123,  "y.c", 101);
    CHECK_INT(tab->nAlloc == 2); /* increment */
    CHECK_INT(tab->index  == 0); /*  unchanged */
    CHECK_INT(tab->nAllocReq  == 2); /* unchanged */
    CHECK_INT(tab->nAllocFail  == 0); /* unchanged */
    CHECK_INT(tab->nFreeReq  == 0); /* unchanged */
    CHECK_INT(tab->nFreeFail  == 0); /* unchanged */
    CHECK_INT(tab->elem[0].isAllocated == 1); 
    CHECK_INT(tab->elem[1].isAllocated == 1); 
    CHECK_INT(tab->elem[2].isAllocated == 0); 
  }
  { /* test 10 */
    START_TEST(10, "Remove one instance of 2 identical ptrs");
    pmRecordBufFree(tab, &c, 123,  "y.c", 101);
    CHECK_INT(tab->nAlloc == 1); /* increment */
    CHECK_INT(tab->index  == 0); /*  should be 0 */
    CHECK_INT(tab->nAllocReq  == 2); /* unchanged */
    CHECK_INT(tab->nAllocFail  == 0); /* unchanged */
    CHECK_INT(tab->nFreeReq  == 1); /* increment */
    CHECK_INT(tab->nFreeFail  == 0); /* unchanged */
    CHECK_INT(tab->elem[0].isAllocated == 0); 
    CHECK_INT(tab->elem[1].isAllocated == 1); 
    CHECK_INT(tab->elem[2].isAllocated == 0); 
  }
  return 1;
}

#endif

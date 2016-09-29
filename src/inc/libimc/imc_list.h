/**
 * @file    imc_list.h
 * @brief List Operations
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author : Manu Sharma / Shweta Polepally
 * Date:
 * Description:   Initial version.
 *
 */

#ifndef IMC_LIST_H
#define IMC_LIST_H

#include "all_cmn.h"

/* List Macros */

#define listHasSpace(list) ((list)->numElemPresent < list->maxElem)
#define listGetNumElem(list) (list)->numElemPresent
#define listGetMaxLen(list) list->maxElem
#define listInsert listInsertLast

/* Structures and enum */
typedef struct s_ListNode
{
  struct s_ListNode *next;
  void              *data;
  TBOOL             isUsed;
} ListNode;

typedef struct s_List
{
  UINT32     maxElem;
  UINT32     numElemPresent;          /* number of elements in the list */
  ListNode  *nodeTab;  /* Previously defined as array. Changed to pointer - 6April2011 */
  ListNode  *first;
} List;

/* Function Prototypes */
void listInit(
  IN UINT32  maxElem,
  IN UINT32  sizofEachElem,
  IO List   *list
  );
TBOOL listInsertLast(
  IO List     *list,
  IO void     *data,
  IN UINT32    lenOfData
  );
void listDeleteByNode(
  IO List        *list,
  IO ListNode    *node,
  IO ListNode    *prev,
  IN UINT32       lenofdata
  );
TBOOL listDeleteByData(
  IO List     *list,
  IO void     *data,
  IN UINT32   lenofdata
  );

#endif

/**
 * @file    imc_list.c
 * @brief  Implementation of a linked list without malloc's.
 *         Maximum number of elements that can go in a list.
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author  Manu Sharma
 * @author  Shweta Polepally
 *
 * @details  Initial version.
 */

#include "imc_list.h"

/**
 * @param    list List on which operation is to be performed.
 * @details  This function search for an un-used element, if found returns the node.
 **/
ListNode *allocNode(
  IO List *list
  )
{
  UINT32 i;
  for( i=0; i<listGetMaxLen(list); i++ )
  {
    if(list->nodeTab[i].isUsed EQ FALSE)
    {
      list->nodeTab[i].isUsed = TRUE;
      list->numElemPresent++;
      return (&list->nodeTab[i]);
    }
  }
  return NULL;
}

/**
 * @param    list List on which operation is to be performed.
 * @param    node Address of list->nodeTab
 * @details  This function de-allocates the node.
 **/
void deallocNode(
  IO List     *list,
  IO ListNode *node,
  IN UINT32   lenOfData
  )
{
  /* node is an address of list->nodeTab only */
  node->isUsed = FALSE;
  node->next= NULL;
  memset( node->data, 0, lenOfData );
  list->numElemPresent--;
}

/**
 * @param    list List on which operation is to be performed.
 * @param    data Entry to add in list
 * @details  This function inserts node at the end of the list
 **/
TBOOL listInsertLast(
  IO List     *list,
  IO void     *data,
  IN UINT32    lenOfData
  )
{
  ListNode *newNode;
  if(listHasSpace(list) EQ FALSE)
  {
    logInfo("List=%x of size=%d is full; data=%x not inserted", list, listGetMaxLen(list), data);
    return FALSE;
  }

  newNode = allocNode(list);
  /* newNode->data = data; Changed on 6thApril2011*/
  copyBytes( data, lenOfData, newNode->data );

  if( list->first EQ NULL )
  {
    list->first = newNode;
  }
  else
  {
    ListNode *n = list->first;
    while(n->next NOTEQ NULL)
      n = n->next;

    n->next = newNode;
  }
  logInfo("Inserted data=%x in list=%x", data, list);
  return TRUE;
}

/**
 * @param    list List on which operation is to be performed.
 * @details  This function deletes the first node of the list.
 **/
TBOOL listDeleteFirst(
  IO List      *list,
  IN UINT32     lenofdata
  )
{
  ListNode  *node2Del;

  if(!list->first)
  {
    logInfo("%s called but list=%x is empty", __FUNCTION__, list);
    return FALSE;
  }
  node2Del     = list->first;
  list->first  = node2Del->next;
  deallocNode(list, node2Del, lenofdata);
  return TRUE;
}

/**
 * @param    list  List on which operation is to be performed.
 * @param    data to be deleted
 *           This pointer is not freed;
 *           listDeleteByNode() just removes the node that contained this data.
 * @details  This function deletes the element 'data' from the list
 **/
void listDeleteByNode(
  IO List        *list,
  IO ListNode    *node,
  IO ListNode    *prev,
  IN UINT32       lenofdata
  )
{
  if ( prev EQ NULL )
  {
    listDeleteFirst( list, lenofdata );
  }
  else
  {
    prev->next = node->next;
    deallocNode(list, node, lenofdata);
  }
  logInfo("Deleted node=%x from list=%x", node, list);
}

TBOOL listDeleteByData(
  IO List     *list,
  IO void     *data,
  IN UINT32   lenofdata
  )
{
        /* Element data is assumed to be present in the list */
  ListNode *node = list->first;
  ListNode *prev = NULL;
  while(node)
  {
    if(node->data EQ data)
    {
      if(!prev)
        listDeleteFirst( list, lenofdata );
      else
      {
        prev->next = node->next;
        deallocNode(list, node, lenofdata);
      }
      logInfo("Deleted data=%x from list=%x", data, list);
      return TRUE;
    }
    else
    {
      prev = node;
      node = node->next;
    }
  }
  logInfo("could not find data=%x in list=%x", data, list);
  return FALSE;
}

/**
 * @param    list List on which operation is to be performed.
 * @details  This function initializes the list.
 **/
void listInit(
  IN UINT32  maxElem,
  IN UINT32  sizofEachElem,
  IO List   *list
  )
{
  UINT32 i;

  /* memset(list, 0, sizeof(List));*/
  list->nodeTab = malloc( sizeof(ListNode) * maxElem );

  list->maxElem = maxElem;
  list->first   = NULL;

  for(i = 0; i < maxElem; i++)
  {
    list->nodeTab[i].data   = malloc( sizofEachElem );
    list->nodeTab[i].next   = NULL;
    list->nodeTab[i].isUsed = FALSE;
  }
}

/**
 * @param    list List on which operation is to be performed.
 * @details  This function prints the list
 **/
void listPrint(
  IN List *list
  )
{
  ListNode *node;
  logInfo("%s: list=%x has %d elements", __FUNCTION__, list, listGetNumElem(list));
  node = list->first;
  while(node)
  {
    logInfo("node=%x data=%x, next=%x", node, (node->data ? node->data : NULL), (node->next ? node->next : NULL));
    node = node->next;
  }
}

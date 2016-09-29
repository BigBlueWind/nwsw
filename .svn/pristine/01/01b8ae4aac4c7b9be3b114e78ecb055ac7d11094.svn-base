//-------------------------------------------------------------------------------------------
/** @file 4gmx_list.h
 *
 * @brief Lists API declaration
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_LIST_H_
#define _4GMX_LIST_H_

#include "4gmx_config.h"

#ifdef MX_LIST_ENABLED

#ifdef __cplusplus
extern "C"
{
#endif

#include "4gmx_types.h"
#include "4gmx_const.h"

/************************************************************************/
/*       The list of module error codes                                 */
/************************************************************************/
#define MX_LIST_ALLOC_ERROR       DEF_MXRC(MXM_EVENT, 1)
#define MX_LIST_ASSIGN_NAME_ERROR DEF_MXRC(MXM_EVENT, 2)

/************************************************************************/
/*      The list control flags                                          */
/************************************************************************/
#define LCF_ALLOCATED              (1<<0)
#define LCF_NAMED                  (1<<1)
#define LCF_DOUBLELINKED           (1<<2)
#define LCF_SYNC                   (1<<3)

#define MARK_LST_ALLOC(lst)        ((lst)->CtrlFlags |= LCF_ALLOCATED)
#define MARK_LST_DEALLOC(lst)      ((lst)->CtrlFlags &= ~LCF_ALLOCATED)
#define IS_LST_ALLOC(lst)          ((lst)->CtrlFlags & LCF_ALLOCATED)

#define MARK_LST_NAMED(lst)        ((lst)->CtrlFlags |= LCF_NAMED)
#define MARK_LST_UNNAMED(lst)      ((lst)->CtrlFlags &= ~LCF_NAMED)
#define IS_LST_NAMED(lst)          ((lst)->CtrlFlags & LCF_NAMED)

#define MARK_LST_DOUBLELINKED(lst) ((lst)->CtrlFlags |= LCF_DOUBLELINKED)
#define IS_LST_DOUBLELINKED(lst)   ((lst)->CtrlFlags & LCF_DOUBLELINKED)

#define MARK_LST_SINGLELINKED(lst) ((lst)->CtrlFlags &= ~LCF_DOUBLELINKED)
#define IS_LST_SINGLELINKED(lst)   (!IS_LST_DOUBLELINKED(lst))

#define MARK_LST_SYNC(lst)         ((lst)->CtrlFlags |= LCF_SYNC)
#define MARK_LST_DESYNC(lst)       ((lst)->CtrlFlags &= ~LCF_SYNC)
#define IS_LST_SYNC(lst)           ((lst)->CtrlFlags & LCF_SYNC)

/***************************************************************************/

/************************************************************************/
/*           The event type descriptor definition                       */
/************************************************************************/
typedef struct _MX_LIST_
{
    LPVOID First;      /**< The pointer to the first element of list */
    LPVOID Last;       /**< The pointer to the second element of list */

    HANDLE hEvent;     /**< The event object connected with the list */

    UINT16 Count;      /**< The number of list elements    */
    UINT16 CtrlFlags;  /**< The object control flags      */
    MXSYNC Sync;       /**< It used for synchronization    */
} LIST, *PLIST;

typedef enum _LIST_TYPE_
{
    SINGLE_LINKED_LIST = (0 << 0),
    DOUBLE_LINKED_LIST = (1 << 0)
} ListType;

typedef struct _DOUBLE_LIST_
{
    LPVOID Next;   /**< The pointer to the next element of list  */
    LPVOID Prev;   /**< The pointer to the previous element of list */
} DOUBLELIST, *PDOUBLELIST;

typedef struct _SINGLE_LIST_
{
    LPVOID Next;   /**< The pointer to the next element of list */
} SINGLELIST, *PSINGLELIST;

#define DLIST(x)  ((PDOUBLELIST)(x))
#define SLIST(x)  ((PSINGLELIST)(x))

typedef enum _LIST_INSERT_TYPE_
{
    LIST_INSERT_BEFORE = (0 << 0),
    LIST_INSERT_AFTER  = (1 << 0)
} LISTINSTYPE;

/** @brief The function creates event object and initializes it.

 @param type   [in] - the list type
 @param name   [in] - the object name (it can be NULL)
 @param hEvent  [in] - the handle of event (it can be HNULL)
 @param oHand  [out] - the handle of created object

 @return [MXRC] error code
*/
MXRC MxCreateList(ListType type, const char *name, HANDLE hEvent,
                  HANDLE * oHand);

/** @brief The function removes allocated descriptor.

 @param h [in] - the event handle

 @return [MXRC] error code */

MXRC MxRemoveList(HANDLE h);

/** @brief The function adds new one element to the end of list.

 @param h [in] - the list handle
 @param pElm [in] - the pointer to new list's element

 @return [MXRC] error code */

MXRC MxAddListElm(HANDLE h, LPVOID pElm);

/** @brief The function cuts element from the list. This function no deallocates memory!
 @param h [in] - the list handle
 @param pElm [in] - the list's element (Be sure that element belong to the list)

 @return [MXRC] error code */

MXRC MxDelListElm(HANDLE h, LPVOID pElm);

/** @brief The function resets list object.
 @param h [in] - the list handle

 @return [MXRC] error code */

MXRC MxEmptyList(HANDLE h);

MXRC MxInsertListElm(HANDLE h, LISTINSTYPE nMode, LPVOID pBase,
                     LPVOID pElm);

UINT32 MxGetListElmCount(HANDLE h);

LPVOID MxGetListNextElm(HANDLE h, LPVOID pBase);
LPVOID MxGetListPrevElm(HANDLE h, LPVOID pBase);
LPVOID MxGetListFirstElm(HANDLE h);
LPVOID MxGetListLastElm(HANDLE h);

#ifdef __cplusplus
}
#endif

#endif        /*MX_LIST_ENABLED */
#endif /*_4GMX_LIST_H_*/

//-------------------------------------------------------------------------------------------
/** @file 4gmx_list.c
 *
 * @brief Lists API implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"

#ifdef MX_LIST_ENABLED

#include "4gmx.h"
#include "4gmx_list.h"
#include "4gmx_kernel.h"
#include "4gmx_serv.h"
#include "4gmx_debug.h"

#if defined (TRACE_LIST_CREATE)|| \
    defined(TRACE_LIST_REMOVE)  || \
    defined(TRACE_LIST_EMPTY)

/** @brief  This function is called by the list API for tracing create/remove
            operations

    @param pCtx    [in] - the system context
    @param id      [in] - the trace event ID
    @param hHandle [in] - the object handle (or HNULL)
    @param rc      [in] - error code of operation

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_list
*/
MXRC AddListCtrl (PMXCONTEXT pCtx, TraceEventID id, HANDLE hHandle, MXRC rc,
UINT32 Type) { TraceListCtrl trace;

    trace.hList = hHandle;
    trace.Rc    = rc;
    trace.Type  = Type;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif

#if defined (TRACE_LIST_ADD_ELM) || defined(TRACE_LIST_DEL_ELM)

/** @brief  This function is called by the list API for tracing add/remove
            list operations

    @param pCtx    [in] - the system context
    @param id      [in] - the trace event ID
    @param hList   [in] - the object handle (or HNULL)
    @param Elm     [in] - the address of element

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_list
*/
MXRC AddListAddRemElm (PMXCONTEXT pCtx, TraceEventID id, HANDLE hList, LPVOID Elm)
{
    TraceListAddRemElm trace;

    trace.hList   = hList;
    trace.ElmAddr = (UINT32)Elm;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif

#if defined (TRACE_LIST_INSERT)

/** @brief  This function is called by the list API for tracing insert
            list operations

    @param pCtx    [in] - the system context
    @param id      [in] - the trace event ID
    @param hList   [in] - the object handle (or HNULL)
    @param nCtrl   [in] - the ID of before/end place of insertion
    @param Base    [in] - the address of reference
    @param Elm     [in] - the address of inserted element

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_list
*/
MXRC AddListInsElm (PMXCONTEXT pCtx, TraceEventID id, HANDLE hList,
                    UINT32 nCtrl, LPVOID Base, LPVOID Elm)
{
    TraceListInsElm trace;

    trace.hList  = hList;
    trace.Base   = (UINT32)Base;
    trace.NewElm = (UINT32)Elm;
    trace.Ctrl   = nCtrl;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif



/** @brief The function creates list object and initializes it.

    @param type   [in] - the list type
    @param name   [in] - the object name (it can be NULL)
    @param hEvent  [in] - the handle of event (it can be HNULL)
    @param oHand  [out] - the handle of created object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_list
*/
MXRC MxCreateList(ListType type, const char *name, HANDLE hEvent,
                  HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    PLIST pList;
    IRQTYPE irq;
    HANDLE h = HNULL;
    DEF_RADDR;

    _ASSERT(oHand != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pList = MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_LIST]);

    if (pList == NULL)
    {
#ifdef TRACE_LIST_CREATE
        AddListCtrl (pCtx, TEID_CREATE_LIST, HNULL, MX_LIST_ALLOC_ERROR, type);
#endif

        MxiRestInt(irq);
        return MX_LIST_ALLOC_ERROR;
    }

    pList->CtrlFlags = 0;
    pList->Count = 0;
    pList->First = NULL;
    pList->Last = NULL;
    pList->hEvent = hEvent;
    pList->Sync = 0;

    if (type == DOUBLE_LINKED_LIST)
    {
        MARK_LST_DOUBLELINKED(pList);
    }
    else
    {
        MARK_LST_SINGLELINKED(pList);
    }

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_LIST,
                ((((UINT32) pList) - ((UINT32) pCtx->lists)) / sizeof(LIST)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_LIST], pList);

#ifdef TRACE_LIST_CREATE
            AddListCtrl (pCtx, TEID_CREATE_LIST, HNULL,
                         MX_LIST_ASSIGN_NAME_ERROR, type);
#endif
            MxiRestInt(irq);

            return MX_LIST_ASSIGN_NAME_ERROR;
        }

        MARK_LST_NAMED(pList);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_LST_UNNAMED(pList);
    }

    MARK_LST_DESYNC(pList);
    MARK_LST_ALLOC(pList);

    *oHand = h;

#ifdef TRACE_LIST_CREATE
    AddListCtrl (pCtx, TEID_CREATE_LIST, h, MX_OK, type);
#endif

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function removes allocated descriptor.

    @param h [in] - the list handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_list
*/
MXRC MxRemoveList(HANDLE h)
{
    PMXCONTEXT pCtx;
    PLIST pList;
    IRQTYPE irq;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_LIST);

    /* Only locally created object can be removed */

    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pList = (PLIST) MxiGetListDescr(pCtx, h);

    _ASSERT(pList != NULL);
    _ASSERT(IS_LST_ALLOC(pList));

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOn(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

#ifdef MX_NAMING_ENABLED

    if (IS_LST_NAMED(pList))
    {
        MxiFreeName(pCtx, h);

        MARK_LST_UNNAMED(pList);
    }

#endif /*MX_NAMING_ENABLED */

    MARK_LST_DEALLOC(pList);

#ifdef TRACE_LIST_REMOVE
    AddListCtrl (pCtx, TEID_REMOVE_LIST, h, MX_OK,
                 IS_LST_DOUBLELINKED(pList) ?
                 DOUBLE_LINKED_LIST : SINGLE_LINKED_LIST);
#endif

    MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_LIST], pList);

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOff(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function adds new one element to the end of list.

    @param h [in] - the list handle
    @param pElm [in] - the pointer to new list's element

    @return [MXRC] error code

    \ingroup group_lte_4gmx_list
*/
MXRC MxAddListElm(HANDLE h, LPVOID pElm)
{
    PMXCONTEXT pCtx;
    PLIST pList;
    IRQTYPE irq;
    MXRC rc;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_LIST);

    _ASSERT(pElm != NULL);

    rc = MX_OK;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    _ASSERT(h.CPU == MxGetCpuID());

#else

    _ASSERT(h.CPU == MxGetCpuID());

#endif

    pList = MxiGetListDescr(pCtx, h);

    _ASSERT(pList != NULL);

    MxiDisInt(&irq);

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOn(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

    if (IS_LST_DOUBLELINKED(pList))
    {
        if (pList->First == NULL)
        {
            pList->First = pElm;
            pList->Last = pElm;

            ((PDOUBLELIST) pElm)->Next = NULL;
            ((PDOUBLELIST) pElm)->Prev = NULL;
        }
        else
        {
            DLIST(pList->Last)->Next = pElm;

            DLIST(pElm)->Prev = pList->Last;
            DLIST(pElm)->Next = NULL;

            pList->Last = pElm;
        }
    }
    else
    {
        if (pList->First == NULL)
        {
            pList->First = pElm;
            pList->Last = pElm;

            SLIST(pElm)->Next = NULL;
        }
        else
        {
            SLIST(pElm)->Next = NULL;

            SLIST(pList->Last)->Next = pElm;

            pList->Last = pElm;
        }

    }

    pList->Count++;

#ifdef TRACE_LIST_ADD_ELM
    AddListAddRemElm (pCtx, TEID_ADD_ELM_LIST, h, pElm);
#endif

#ifdef MX_EVENT_ENABLED

    if (HANDLE_NOT_NULL(pList->hEvent))
    {
        rc = MxSetEvent(pList->hEvent);
    }

#endif

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOff(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

    MxiRestInt(irq);

    return rc;
}

/** @brief The function cuts element from the list. This function no deallocates memory!

    @param h [in] - the list handle
    @param pElm [in] - the list's element (Be sure that element belong to the list)

    @return [MXRC] error code

    \ingroup group_lte_4gmx_list
*/
MXRC MxDelListElm(HANDLE h, LPVOID pElm)
{
    PMXCONTEXT pCtx;
    PLIST pList;
    IRQTYPE irq;
    PSINGLELIST pItem;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_LIST);
    _ASSERT(pElm != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    _ASSERT(h.CPU == MxGetCpuID());

#else

    _ASSERT(h.CPU == MxGetCpuID());

#endif

    pList = MxiGetListDescr(pCtx, h);

    _ASSERT(pList != NULL);

    MxiDisInt(&irq);

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOn(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

    if (IS_LST_DOUBLELINKED(pList))
    {
        if (((PDOUBLELIST) pElm)->Prev != NULL)
        {
            ((PDOUBLELIST)((PDOUBLELIST) pElm)->Prev)->Next =
                ((PDOUBLELIST) pElm)->Next;
        }

        if (((PDOUBLELIST) pElm)->Next != NULL)
        {
            ((PDOUBLELIST)((PDOUBLELIST) pElm)->Next)->Prev =
                ((PDOUBLELIST) pElm)->Prev;
        }

        if (pList->First == pElm)
        {
            pList->First = ((PDOUBLELIST) pElm)->Next;
        }

        if (pList->Last == pElm)
        {
            pList->Last = ((PDOUBLELIST) pElm)->Prev;
        }

        ((PDOUBLELIST) pElm)->Next = NULL;
        ((PDOUBLELIST) pElm)->Prev = NULL;
    }
    else
    {
        if (pList->First == pElm)
        {
            pList->First = ((PSINGLELIST) pElm)->Next;

            if (pList->Last == pElm)
            {
                pList->Last = NULL;
            }
        }
        else
        {
            pItem = (PSINGLELIST) pList->First;

            while (pItem != NULL)
            {
                if (pItem->Next == pElm)
                    break;

                pItem = pItem->Next;
            }

            _ASSERT(pItem != NULL);

            pItem->Next = ((PSINGLELIST) pElm)->Next;

            if (pList->Last == pElm)
            {
                pList->Last = pItem;
            }
        }

        ((PSINGLELIST) pElm)->Next = NULL;
    }

    pList->Count--;

#ifdef TRACE_LIST_DEL_ELM
    AddListAddRemElm (pCtx, TEID_DEL_ELM_LIST, h, pElm);
#endif

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOff(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function resets list object.

    @param h [in] - the list handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_list
*/
MXRC MxEmptyList(HANDLE h)
{
    PMXCONTEXT pCtx;
    PLIST pList;
    IRQTYPE irq;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_LIST);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    _ASSERT(h.CPU == MxGetCpuID());

#else

    _ASSERT(h.CPU == MxGetCpuID());

#endif

    pList = MxiGetListDescr(pCtx, h);

    _ASSERT(pList != NULL);

    MxiDisInt(&irq);

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOn(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

    pList->Count = 0;
    pList->First = NULL;
    pList->Last = NULL;

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOff(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

#ifdef TRACE_LIST_EMPTY
    AddListCtrl (pCtx, TEID_EMPTY_LIST, h, MX_OK,
                 IS_LST_DOUBLELINKED(pList) ?
                 DOUBLE_LINKED_LIST : SINGLE_LINKED_LIST);
#endif

    MxiRestInt(irq);

    return MX_OK;
}

MXRC MxInsertListElm(HANDLE h, LISTINSTYPE nMode, LPVOID pBase, LPVOID pElm)
{
    PMXCONTEXT pCtx;
    PLIST pList;
    IRQTYPE irq;
    MXRC rc = MX_OK;
    PSINGLELIST pItem;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_LIST);

    _ASSERT(pBase != NULL);
    _ASSERT(pElm != NULL);

    _ASSERT(nMode == LIST_INSERT_BEFORE || nMode == LIST_INSERT_AFTER);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    _ASSERT(h.CPU == MxGetCpuID());

#else

    _ASSERT(h.CPU == MxGetCpuID());

#endif

    pList = MxiGetListDescr(pCtx, h);

    _ASSERT(pList != NULL);

    MxiDisInt(&irq);

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOn(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

    if (IS_LST_DOUBLELINKED(pList))
    {
        if (nMode == LIST_INSERT_BEFORE)
        {
            DLIST(pElm)->Prev = DLIST(pBase)->Prev;

            if (DLIST(pBase)->Prev != NULL)
            {
                DLIST(DLIST(pBase)->Prev)->Next = pElm;
            }

            DLIST(pBase)->Prev = pElm;

            DLIST(pElm)->Next = pBase;

            if (pList->First == pBase)
            {
                pList->First = pElm;
            }
        }
        else
        {
            DLIST(pElm)->Next = DLIST(pBase)->Next;

            if (DLIST(pBase)->Next != NULL)
            {
                DLIST(DLIST(pBase)->Next)->Prev = pElm;
            }

            DLIST(pBase)->Next = pElm;
            DLIST(pElm)->Prev = pBase;

            if (pList->Last == pBase)
            {
                pList->Last = pElm;
            }
        }
    }
    else
    {
        if (nMode == LIST_INSERT_BEFORE)
        {
            if (pList->First == pBase)
            {
                pList->First = pElm;
                SLIST(pElm)->Next = pBase;
            }
            else
            {
                /* The previous element should be find out */

                pItem = (PSINGLELIST) pList->First;

                while (pItem != NULL)
                {
                    if (pItem->Next == pBase)
                        break;

                    pItem = pItem->Next;
                }

                _ASSERT(pItem != NULL);

                pItem->Next = pElm;
                SLIST(pElm)->Next = pBase;
            }
        }
        else
        {
            SLIST(pElm)->Next = SLIST(pBase)->Next;
            SLIST(pBase)->Next = pElm;

            if (pList->Last == pBase)
            {
                pList->Last = pElm;
            }
        }
    }

    pList->Count++;


#ifdef TRACE_LIST_INSERT
    AddListInsElm(pCtx, TEID_INSERT_LIST, h,
                   (UINT32)nMode, pBase, pElm);
#endif

#ifdef MX_EVENT_ENABLED
    if (HANDLE_NOT_NULL(pList->hEvent))
    {
        rc = MxSetEvent(pList->hEvent);
    }
#endif

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOff(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

    MxiRestInt(irq);

    return rc;
}

UINT32 MxGetListElmCount(HANDLE h)
{
    PMXCONTEXT pCtx;
    PLIST pList;
    UINT32 count;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_LIST);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    _ASSERT(h.CPU == MxGetCpuID());

#else

    _ASSERT(h.CPU == MxGetCpuID());

#endif

    pList = MxiGetListDescr(pCtx, h);

    _ASSERT(pList != NULL);

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOn(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

    count = pList->Count;

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOff(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

    return count;
}

LPVOID MxGetListNextElm(HANDLE h, LPVOID pBase)
{
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_LIST);
    _ASSERT(pBase != NULL);

    /* The single and double linked lists have the same offset for the <next> field */

    return DLIST(pBase)->Next;
}

LPVOID MxGetListPrevElm(HANDLE h, LPVOID pBase)
{
    PMXCONTEXT pCtx;
    PLIST pList;
    PSINGLELIST pPrev;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_LIST);
    _ASSERT(pBase != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    _ASSERT(h.CPU == MxGetCpuID());

#else

    _ASSERT(h.CPU == MxGetCpuID());

#endif

    pList = MxiGetListDescr(pCtx, h);

    _ASSERT(pList != NULL);

    if (IS_LST_DOUBLELINKED(pList))
    {
        return DLIST(pBase)->Prev;
    }
    else
    {

#ifdef MX_INTERCPU_ENABLED

        if (IS_LST_SYNC(pList))
        {
            MxiSyncCtrlOn(OBJ_TYPE_LIST, pList);
        }

#endif /*MX_INTERCPU_ENABLED */

        pPrev = pList->First;

        while (pPrev != NULL)
        {
            if (pPrev->Next == pBase)
                break;

            pPrev = pPrev->Next;
        }

#ifdef MX_INTERCPU_ENABLED

        if (IS_LST_SYNC(pList))
        {
            MxiSyncCtrlOff(OBJ_TYPE_LIST, pList);
        }

#endif /*MX_INTERCPU_ENABLED */

        return pPrev;
    }
}

LPVOID MxGetListFirstElm(HANDLE h)
{
    PMXCONTEXT pCtx;
    PLIST pList;
    LPVOID pItem;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_LIST);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    _ASSERT(h.CPU == MxGetCpuID());

#else

    _ASSERT(h.CPU == MxGetCpuID());

#endif

    pList = MxiGetListDescr(pCtx, h);

    _ASSERT(pList != NULL);

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOn(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

    pItem = pList->First;

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOff(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

    return pItem;
}

LPVOID MxGetListLastElm(HANDLE h)
{
    PMXCONTEXT pCtx;
    PLIST pList;
    LPVOID pItem;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_LIST);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

#ifdef MX_INTERCPU_ENABLED

    _ASSERT(h.CPU == MxGetCpuID());

#else

    _ASSERT(h.CPU == MxGetCpuID());

#endif

    pList = MxiGetListDescr(pCtx, h);

    _ASSERT(pList != NULL);

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOn(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

    pItem = pList->Last;

#ifdef MX_INTERCPU_ENABLED

    if (IS_LST_SYNC(pList))
    {
        MxiSyncCtrlOff(OBJ_TYPE_LIST, pList);
    }

#endif /*MX_INTERCPU_ENABLED */

    return pItem;
}

#endif /*MX_LIST_ENABLED */

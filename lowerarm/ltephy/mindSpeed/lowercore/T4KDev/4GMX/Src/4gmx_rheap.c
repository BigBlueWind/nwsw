//-------------------------------------------------------------------------------------------
/** @file 4gmx_rheap.c
 *
 * @brief Rapid Heaps API implementation
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

#ifdef MX_RHEAP_ENABLED

#include "4gmx.h"
#include "4gmx_rheap.h"
#include "4gmx_kernel.h"
#include "4gmx_serv.h"
#include "4gmx_gvars.h"
#include "4gmx_debug.h"

#if  defined (TRACE_RHEAP_CREATE)    || \
     defined (TRACE_RHEAP_REMOVE)    || \
     defined (TRACE_RHEAP_OPEN)

/** @brief  This function is called by the event API for tracing RHEAP.

    @param pCtx    [in] - the system context
    @param id      [in] - the trace event ID
    @param hHandle [in] - the object handle (or HNULL)
    @param rc      [in] - error code of operation

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_rapid_heap
*/
MXRC AddRHeapCtrl (PMXCONTEXT pCtx, TraceEventID id, HANDLE hHandle, MXRC rc)
{
    TraceRHeapCtrl trace;

    trace.hHeap  = hHandle;
    trace.Rc     = rc;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif

#if  defined (TRACE_RHEAP_ALLOC) || defined (TRACE_RHEAP_FREE)

/** @brief  This function is called by the event API for tracing RHEAP.

    @param pCtx    [in] - the system context
    @param id      [in] - the trace event ID
    @param hHandle [in] - the object handle (or HNULL)
    @param data    [in] - the pointer to the mem block of allocation/deallocation
    @param nMemSize[in] - the size of heap mem block in bytes

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_rapid_heap
*/
MXRC AddRHeapDataTrace (PMXCONTEXT pCtx, TraceEventID id,
                       HANDLE hHandle, UINT32 data, UINT32 nMemSize)
{
    TraceRHeapData trace;

    trace.hHeap  = hHandle;
    trace.Data   = data;
    trace.Size   = nMemSize;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif


/** @brief The function creates a heap.

    @param pMem   [in] - the pointer to memory (heap storage)
    @param nSize  [in] - the storage size in bytes
    @param nAlign  [in] - the alignment in bytes
    @param oHand  [out] - the handle of created object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_rapid_heap
*/
MXRC MxCreateRHeap(LPVOID pMem, UINT32 nSize, UINT32 nAlign, const char *name,
                  HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    HANDLE h = HNULL;
    PRHEAP pHeap;
    UINT32 RealSize, HeapAddr;
    DEF_RADDR;

    _ASSERT(pMem != NULL);
    _ASSERT(oHand != NULL);
    _ASSERT(nSize >= sizeof(UINT32));
    _ASSERT(nAlign >= 4);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pHeap = MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_RHEAP]);

    if (pHeap == NULL)
    {
#ifdef TRACE_RHEAP_CREATE
        AddRHeapCtrl (pCtx, TEID_CREATE_RHEAP, HNULL, MX_RHEAP_ALLOC_ERROR);
#endif
        MxiRestInt(irq);
        return MX_RHEAP_ALLOC_ERROR;
    }

    Mximemset(pHeap, 0, sizeof (*pHeap));

    // The heap alignment should be divisible by 4 bytes
    // 0, 4, 8, 12, 16 ...
    nAlign = ROUND(nAlign, 4);

    // The address of heap buffer should be alignment on
    // nAlign size
    HeapAddr = ROUND((UINT32)pMem, nAlign);

    // The real size of heap buffer can be less than
    // nSize because of address alignment
    RealSize = nSize - (HeapAddr - (UINT32)pMem);

#ifdef _MX_DEBUG
    pHeap->TotalSize = RealSize;
#endif

    // The heap size should be divisible by
    // heap alignment
    //RealSize = RealSize & (~(nAlign - 1));

    pHeap->HeapStorage = (LPVOID)HeapAddr;
    pHeap->FreeBlock   = (LPVOID)HeapAddr;
    pHeap->FreeSize    = RealSize;
    pHeap->Align       = nAlign;
    pHeap->CtrlFlags   = 0;

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_RHEAP,
                ((((UINT32) pHeap) - ((UINT32) pCtx->rheaps)) / sizeof(RHEAP)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_RHEAP], pHeap);

#ifdef TRACE_RHEAP_CREATE
            AddRHeapCtrl (pCtx, TEID_CREATE_RHEAP,
                         HNULL, MX_RHEAP_ASSIGN_NAME_ERROR);
#endif

            MxiRestInt(irq);

            return MX_RHEAP_ASSIGN_NAME_ERROR;
        }

        MARK_RHEAP_NAMED(pHeap);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_RHEAP_UNNAMED(pHeap);
    }

    MARK_RHEAP_ALLOC(pHeap);

#ifdef TRACE_RHEAP_CREATE
    AddRHeapCtrl (pCtx, TEID_CREATE_RHEAP, h, MX_OK);
#endif

    *oHand = h;

    MxiRestInt(irq);

    return MX_OK;
}


/** @brief The function creates a shared heap. The heap descriptor is not located
           in the 4GMX context, this descriptor can be used to open
           a heap by other 4GMX system(s)

    @param pHeapDescr  [in] - the pointer to the heap descriptor that will be initialized by this function
    @param pMem        [in] - the pointer to memory (heap storage)
    @param nSize       [in] - the storage size in bytes
    @param nAlign      [in] - the alignment in bytes
    @param oHand       [out] - the handle of created object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_rapid_heap
*/
MXRC MxCreateSharedRHeap(ICRHEAP * pHeapDescr, LPVOID pMem, UINT32 nSize, UINT32 nAlign,
                  const char *name, HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    HANDLE h = HNULL;
    PRHEAP pHeap;
    UINT32 RealSize, HeapAddr;
    DEF_RADDR;

    _ASSERT(pHeapDescr != NULL);
    _ASSERT(pMem != NULL);
    _ASSERT(oHand != NULL);
    _ASSERT(nSize >= sizeof(UINT32));
    _ASSERT(nAlign >= 4);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pHeap = MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_RHEAP]);

    if (pHeap == NULL)
    {
#ifdef TRACE_RHEAP_CREATE
        AddHeapCtrl (pCtx, TEID_CREATE_RHEAP, HNULL, MX_RHEAP_ALLOC_ERROR);
#endif
        MxiRestInt(irq);
        return MX_RHEAP_ALLOC_ERROR;
    }

    Mximemset(pHeap, 0, sizeof (*pHeap));

    // To point to the real heap descriptor,
    // the real heap descriptor is not located in the
    // 4GMX system context

    pHeap->HeapStorage = (LPVOID)pHeapDescr;

    MARK_RHEAP_OPENED(pHeap);
    MARK_RHEAP_SYNC(pHeap);

    pHeap->Sync = &pHeapDescr->Sync;

    pHeapDescr->Sync = 0;

    // The heap alignment should be divisible by 4 bytes
    // 0, 4, 8, 12, 16 ...
    nAlign = ROUND(nAlign, 4);

    // The address of heap buffer should be alignment on
    // nAlign size
    HeapAddr = ROUND((UINT32)pMem, nAlign);

    // The real size of heap buffer can be less than
    // nSize because of address alignment
    RealSize = nSize - (HeapAddr - (UINT32)pMem);

    pHeapDescr->HeapStorage = (LPVOID)HeapAddr;
    pHeapDescr->FreeBlock   = (LPVOID)HeapAddr;
    pHeapDescr->FreeSize    = RealSize;
    pHeapDescr->TotalSize   = RealSize;
    pHeapDescr->Align       = nAlign;

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_RHEAP,
                ((((UINT32) pHeap) - ((UINT32) pCtx->rheaps)) / sizeof(RHEAP)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_RHEAP], pHeap);

#ifdef TRACE_RHEAP_CREATE
            AddHeapCtrl (pCtx, TEID_CREATE_RHEAP,
                         HNULL, MX_RHEAP_ASSIGN_NAME_ERROR);
#endif

            MxiRestInt(irq);

            return MX_RHEAP_ASSIGN_NAME_ERROR;
        }

        MARK_RHEAP_NAMED(pHeap);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_RHEAP_UNNAMED(pHeap);
    }

    MARK_RHEAP_ALLOC(pHeap);

#ifdef TRACE_RHEAP_CREATE
    AddRHeapCtrl (pCtx, TEID_CREATE_RHEAP, h, MX_OK);
#endif

    *oHand = h;

    MxiRestInt(irq);

    return MX_OK;
}


/** @brief The function "opens"/"set connections with" a created heap and returns its handle,
   the opened object can be used like a locally created.

    @param pHeapDescr  [in] - the pointer to the inter-cpu heap descriptor
    @param name        [in] - the associated name of opened object
    @param oHand       [out]- the handle of opened partition

    @return [MXRC] error code

    \ingroup group_lte_4gmx_rapid_heap
*/
MXRC MxOpenRHeap(ICRHEAP * pHeapDescr, const char *name, HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    HANDLE h = HNULL;
    PRHEAP pHeap;
    DEF_RADDR;

    _ASSERT(pHeapDescr != NULL);
    _ASSERT(oHand != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pHeap = MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_RHEAP]);

    if (pHeap == NULL)
    {
#ifdef TRACE_RHEAP_CREATE
        AddRHeapCtrl (pCtx, TEID_CREATE_RHEAP, HNULL, MX_RHEAP_ALLOC_ERROR);
#endif
        MxiRestInt(irq);
        return MX_RHEAP_ALLOC_ERROR;
    }

    Mximemset(pHeap, 0, sizeof (*pHeap));

    // To point to the real heap descriptor,
    // the real heap descriptor is not located in the
    // 4GMX system context

    pHeap->HeapStorage = (LPVOID)pHeapDescr;

    MARK_RHEAP_OPENED(pHeap);
    MARK_RHEAP_SYNC(pHeap);

    pHeap->Sync = &pHeapDescr->Sync;

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_RHEAP,
                ((((UINT32) pHeap) - ((UINT32) pCtx->rheaps)) / sizeof(RHEAP)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_RHEAP], pHeap);

#ifdef TRACE_RHEAP_CREATE
            AddHeapCtrl (pCtx, TEID_CREATE_RHEAP,
                         HNULL, MX_RHEAP_ASSIGN_NAME_ERROR);
#endif

            MxiRestInt(irq);

            return MX_RHEAP_ASSIGN_NAME_ERROR;
        }

        MARK_RHEAP_NAMED(pHeap);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_RHEAP_UNNAMED(pHeap);
    }

    MARK_RHEAP_ALLOC(pHeap);

#ifdef TRACE_RHEAP_CREATE
    AddRHeapCtrl (pCtx, TEID_CREATE_RHEAP, h, MX_OK);
#endif

    *oHand = h;

    MxiRestInt(irq);

    return MX_OK;
}


/** @brief The function removes created heap.

    @param h [in] - the heap handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_rapid_heap
*/
MXRC MxRemoveRHeap(HANDLE h)
{
    PMXCONTEXT pCtx;
    PRHEAP pHeap;
    IRQTYPE irq;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_RHEAP);

    /* Only locally created object can be removed */

    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pHeap = (PRHEAP) MxiGetRHeapDescr(pCtx, h);

    _ASSERT(pHeap != NULL);
    _ASSERT(IS_RHEAP_ALLOC(pHeap));

#ifdef MX_NAMING_ENABLED

    if (IS_RHEAP_NAMED(pHeap))
    {
        MxiFreeName(pCtx, h);

        MARK_RHEAP_UNNAMED(pHeap);
    }

#endif /*MX_NAMING_ENABLED */

    pHeap->CtrlFlags = 0;

    MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_RHEAP], pHeap);

#ifdef TRACE_RHEAP_REMOVE
    AddRHeapCtrl (pCtx, TEID_REMOVE_RHEAP, h, MX_OK);
#endif

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function allocates a block of memory of reqired size and returns a pointer to
    the allocated memory or NULL. The size is alignment and the memory block can be bigger that required.

    @param h [in] - the heap handle
    @param nSize[in] - the size of block in bytes

    @return [LPVOID] a pointer to allocated block or NULL.

    \ingroup group_lte_4gmx_rapid_heap
*/
LPVOID MxRHeapAlloc(HANDLE h, UINT32 nSize)
{
    PMXCONTEXT pCtx;
    PRHEAP pHeapObj;
    ICHEAP * pHeap;
    IRQTYPE irq;
    LPVOID pAllocBlock;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_RHEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
    _ASSERT(nSize > 0);

    pHeapObj = MxiGetRHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_RHEAP_ALLOC(pHeapObj));

    if (IS_RHEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;

        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_RHEAP_OPENED(pHeapObj) || IS_RHEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_RHEAP, pHeapObj);
    }

    // The allocated block has to be aligned on heap alignment

    nSize = ROUND(nSize, pHeap->Align);

    if (pHeap->FreeSize < nSize)
    {

#ifdef TRACE_RHEAP_ALLOC
        AddRHeapDataTrace (pCtx, TEID_ALLOC_RHEAP, h, (UINT32)NULL, nSize);
#endif

        if (IS_RHEAP_OPENED(pHeapObj) || IS_RHEAP_SYNC(pHeapObj))
        {
            MxiSyncCtrlOff(OBJ_TYPE_RHEAP, pHeapObj);
        }

        MxiRestInt(irq);

	// this code is added specially to catch all potentially dangerous
	// situations when MxRHeapAlloc returns NULL and application does not check it
	// (nSize == 0) is used to prevent compiler warnings, nSize != 0, and assert will take place

        _ASSERT (nSize == 0);

        return NULL;
    }

    pAllocBlock = pHeap->FreeBlock;
    pHeap->FreeBlock = (LPVOID)((PUINT8)pHeap->FreeBlock + nSize);
    pHeap->FreeSize -= nSize;

#ifdef TRACE_RHEAP_ALLOC
    AddRHeapDataTrace (pCtx, TEID_ALLOC_RHEAP, h,
                               (UINT32)&(pAllocBlock), nSize);
#endif

    if (IS_RHEAP_OPENED(pHeapObj) || IS_RHEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_RHEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return pAllocBlock;
}

/** @brief The function frees a block of heap memory

    @param h        [in] - the heap handler
    @param pBlock   [in] - the pointer to the heap memory block

    @return [MXRC] an error code of operation

    \ingroup group_lte_4gmx_rapid_heap
*/
MXRC MxRHeapFree(HANDLE h)
{
    PMXCONTEXT pCtx;
    PRHEAP pHeapObj;
    ICRHEAP * pHeap;
    IRQTYPE irq;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_RHEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pHeapObj = MxiGetRHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_RHEAP_ALLOC(pHeapObj));

    if (IS_RHEAP_OPENED(pHeapObj))
    {
        pHeap = (ICRHEAP*)pHeapObj->HeapStorage;

        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICRHEAP*)pHeapObj;
    }

    if (IS_RHEAP_OPENED(pHeapObj) || IS_RHEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_RHEAP, pHeapObj);
    }

    pHeap->FreeBlock = pHeap->HeapStorage;
    pHeap->FreeSize  = pHeap->TotalSize;

    if (IS_RHEAP_OPENED(pHeapObj) || IS_RHEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_RHEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function returns total free size of the heap in bytes

    @NOTE: because of possible fragmentation, the heap may return NULL
            for allocation of block with this size

    @param h [in] - the heap descriptor

    @return [UINT32] the size of heap

    \ingroup group_lte_4gmx_rapid_heap
*/
UINT32 MxRHeapGetFreeSpace (HANDLE h)
{
    PMXCONTEXT pCtx;
    PRHEAP pHeapObj;
    ICRHEAP * pHeap;
    IRQTYPE irq;
    UINT32 nSize = 0;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_RHEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pHeapObj = MxiGetRHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_RHEAP_ALLOC(pHeapObj));

    if (IS_RHEAP_OPENED(pHeapObj))
    {
        pHeap = (ICRHEAP*)pHeapObj->HeapStorage;

        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICRHEAP*)pHeapObj;
    }

    if (IS_RHEAP_OPENED(pHeapObj) || IS_RHEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_RHEAP, pHeapObj);
    }

    nSize = pHeap->FreeSize;

    if (IS_RHEAP_OPENED(pHeapObj) || IS_RHEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_RHEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return nSize;
}

#endif /*MX_RHEAP_ENABLED */

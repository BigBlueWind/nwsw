//-------------------------------------------------------------------------------------------
/** @file 4gmx_heap.c
 *
 * @brief Heaps API implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.16 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"

#ifdef MX_HEAP_ENABLED

#include "4gmx.h"
#include "4gmx_heap.h"
#include "4gmx_kernel.h"
#include "4gmx_serv.h"
#include "4gmx_gvars.h"
#include "4gmx_debug.h"

#if  defined (TRACE_HEAP_CREATE)    || \
     defined (TRACE_HEAP_REMOVE)    || \
     defined (TRACE_HEAP_OPEN)

/** @brief  This function is called by the event API for tracing HEAP.

    @param pCtx    [in] - the system context
    @param id      [in] - the trace event ID
    @param hHandle [in] - the object handle (or HNULL)
    @param rc      [in] - error code of operation

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_heap
*/
MXRC AddHeapCtrl (PMXCONTEXT pCtx, TraceEventID id, HANDLE hHandle, MXRC rc)
{
    TraceHeapCtrl trace;

    trace.hHeap  = hHandle;
    trace.Rc     = rc;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif

#if  defined (TRACE_HEAP_ALLOC) || defined (TRACE_HEAP_FREE)

/** @brief  This function is called by the event API for tracing HEAP.

    @param pCtx    [in] - the system context
    @param id      [in] - the trace event ID
    @param hHandle [in] - the object handle (or HNULL)
    @param data    [in] - the pointer to the mem block of allocation/deallocation
    @param nMemSize[in] - the size of heap mem block in bytes

    @return [MXRC] error code of operation

    \ingroup group_lte_4gmx_heap
*/
MXRC AddHeapDataTrace (PMXCONTEXT pCtx, TraceEventID id,
                       HANDLE hHandle, UINT32 data, UINT32 nMemSize)
{
    TraceHeapData trace;

    trace.hHeap  = hHandle;
    trace.Data   = data;
    trace.Size   = nMemSize;

    return MxWriteDebugTrace(pCtx, id, &trace, sizeof (trace));
}

#endif

#ifdef MX_HEAP_LEAKAGE_CONTROL

/** @brief This function inserts new already allocated block to the chain (doble linked list)
           of allocated blocks. By using 4GMX heap API it will be possible to scan the allocated blocks
           and to get all needed information about application that allocated these blocks

    @param pHeap [in] - the heap descriptor
    @param nLR   [in] - the link register
    @param pBlock[in] - the pointer to the header of allocated block

    \ingroup group_lte_4gmx_heap

    */

INLINE void MxiHeapAddBlock (ICHEAP* pHeap, UINT32 nLR, MEM_ALLOC_BLOCK*pBlock)
{
    _ASSERT_PTR(pHeap);
    _ASSERT_PTR(pBlock);

    if (pHeap->AllocBlock != NULL)
    {
        pHeap->AllocBlock->PrevAllocated = pBlock;
    }

    pBlock->NextAllocated = pHeap->AllocBlock;
    pBlock->PrevAllocated = NULL;
    pBlock->AllocAddr     = nLR;
    pBlock->AllocCpuID    = MxGetCpuID ();
    pBlock->MarkID        = pHeap->MarkID;
    
    pHeap->AllocBlock = pBlock;
}

/** @brief This function removes the block from the chain (doble linked list)
           of allocated blocks. By using 4GMX heap API it will be possible to scan the allocated blocks
           and to get all needed information about application that allocated these blocks.

    @param pHeap [in] - the heap descriptor
    @param pBlock[in] - the pointer to the header of allocated block

    \ingroup group_lte_4gmx_heap

    */

INLINE void MxiHeapRemoveBlock (ICHEAP* pHeap, MEM_ALLOC_BLOCK*pBlock)
{
    _ASSERT_PTR(pHeap);
    _ASSERT_PTR(pBlock);

    if (pBlock->PrevAllocated != NULL)
    {
        pBlock->PrevAllocated->NextAllocated = pBlock->NextAllocated;
    }

    if (pBlock->NextAllocated != NULL)
    {
        pBlock->NextAllocated->PrevAllocated = pBlock->PrevAllocated;
    }

    if (pHeap->AllocBlock == pBlock)
    {
        pHeap->AllocBlock = pBlock->NextAllocated;
    }
}

#endif

/** @brief The function creates a heap.

    @param pMem   [in] - the pointer to memory (heap storage)
    @param nSize  [in] - the storage size in bytes
    @param nAlign  [in] - the alignment in bytes
    @param oHand  [out] - the handle of created object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_heap
*/
MXRC MxCreateHeap(LPVOID pMem, UINT32 nSize, UINT32 nAlign, const char *name,
                  HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    HANDLE h = HNULL;
    PHEAP pHeap;
    UINT32 RealSize, HeapAddr;
    DEF_RADDR;

    _ASSERT(pMem != NULL);
    _ASSERT(oHand != NULL);
    _ASSERT(nSize >= sizeof(MEMBLOCK));
    _ASSERT(nAlign >= 4);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pHeap = MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_HEAP]);

    if (pHeap == NULL)
    {
#ifdef TRACE_HEAP_CREATE
        AddHeapCtrl (pCtx, TEID_CREATE_HEAP, HNULL, MX_HEAP_ALLOC_ERROR);
#endif
        MxiRestInt(irq);
        return MX_HEAP_ALLOC_ERROR;
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

    pHeap->TotalSize = RealSize;

    // The heap size should be divisible by
    // heap alignment
    //RealSize = RealSize & (~(nAlign - 1));

    pHeap->HeapStorage = (LPVOID)HeapAddr;
    pHeap->FreeBlock   = (PMEMBLOCK)HeapAddr;
    pHeap->FreeSize    = RealSize;
    pHeap->Align = nAlign;
    pHeap->CtrlFlags = 0;

    pHeap->FreeBlock->NextFree = NULL;
    pHeap->FreeBlock->Size     = RealSize;

#ifdef MX_HEAP_INTEGRITY
    pHeap->FreeBlock->BlockCheckSum = CREATE_INTEGRITY(pHeap->FreeBlock);
#endif

#ifdef MX_HEAP_LEAKAGE_CONTROL
    pHeap->AllocBlock = NULL;
#endif

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_HEAP,
                ((((UINT32) pHeap) - ((UINT32) pCtx->heaps)) / sizeof(HEAP)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_HEAP], pHeap);

#ifdef TRACE_HEAP_CREATE
            AddHeapCtrl (pCtx, TEID_CREATE_HEAP,
                         HNULL, MX_HEAP_ASSIGN_NAME_ERROR);
#endif

            MxiRestInt(irq);

            return MX_HEAP_ASSIGN_NAME_ERROR;
        }

        MARK_HEAP_NAMED(pHeap);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_HEAP_UNNAMED(pHeap);
    }

    MARK_HEAP_ALLOC(pHeap);

#ifdef TRACE_HEAP_CREATE
    AddHeapCtrl (pCtx, TEID_CREATE_HEAP, h, MX_OK);
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

    \ingroup group_lte_4gmx_heap
*/
MXRC MxCreateSharedHeap(ICHEAP * pHeapDescr, LPVOID pMem, UINT32 nSize, UINT32 nAlign,
                  const char *name, HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    HANDLE h = HNULL;
    PHEAP pHeap;
    UINT32 RealSize, HeapAddr;
    DEF_RADDR;

    _ASSERT(pHeapDescr != NULL);
    _ASSERT(pMem != NULL);
    _ASSERT(oHand != NULL);
    _ASSERT(nSize >= sizeof(MEMBLOCK));
    _ASSERT(nAlign >= 4);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pHeap = MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_HEAP]);

    if (pHeap == NULL)
    {
#ifdef TRACE_HEAP_CREATE
        AddHeapCtrl (pCtx, TEID_CREATE_HEAP, HNULL, MX_HEAP_ALLOC_ERROR);
#endif
        MxiRestInt(irq);
        return MX_HEAP_ALLOC_ERROR;
    }

    Mximemset(pHeap, 0, sizeof (*pHeap));

    // To point to the real heap descriptor,
    // the real heap descriptor is not located in the
    // 4GMX system context

    pHeap->HeapStorage = (LPVOID)pHeapDescr;

    MARK_HEAP_OPENED(pHeap);
    MARK_HEAP_SYNC(pHeap);

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

    pHeapDescr->TotalSize = RealSize;

    // The heap size should be divisible by
    // heap alignment
    //RealSize = RealSize & (~(nAlign - 1));

    pHeapDescr->HeapStorage = (LPVOID)HeapAddr;
    pHeapDescr->FreeBlock   = (PMEMBLOCK)HeapAddr;
    pHeapDescr->FreeSize    = RealSize;
    pHeapDescr->Align       = nAlign;

    pHeapDescr->FreeBlock->NextFree = NULL;
    pHeapDescr->FreeBlock->Size     = RealSize;

#ifdef MX_HEAP_INTEGRITY
    pHeapDescr->FreeBlock->BlockCheckSum = CREATE_INTEGRITY(pHeapDescr->FreeBlock);
#endif

#ifdef MX_HEAP_LEAKAGE_CONTROL
    pHeapDescr->AllocBlock = NULL;
    pHeapDescr->MarkID     = 0;
#endif
    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_HEAP,
                ((((UINT32) pHeap) - ((UINT32) pCtx->heaps)) / sizeof(HEAP)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_HEAP], pHeap);

#ifdef TRACE_HEAP_CREATE
            AddHeapCtrl (pCtx, TEID_CREATE_HEAP,
                         HNULL, MX_HEAP_ASSIGN_NAME_ERROR);
#endif

            MxiRestInt(irq);

            return MX_HEAP_ASSIGN_NAME_ERROR;
        }

        MARK_HEAP_NAMED(pHeap);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_HEAP_UNNAMED(pHeap);
    }

    MARK_HEAP_ALLOC(pHeap);

#ifdef TRACE_HEAP_CREATE
    AddHeapCtrl (pCtx, TEID_CREATE_HEAP, h, MX_OK);
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

    \ingroup group_lte_4gmx_heap
*/
MXRC MxOpenHeap(ICHEAP * pHeapDescr, const char *name, HANDLE * oHand)
{
    PMXCONTEXT pCtx;
    IRQTYPE irq;
    HANDLE h = HNULL;
    PHEAP pHeap;
    DEF_RADDR;

    _ASSERT(pHeapDescr != NULL);
    _ASSERT(oHand != NULL);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pHeap = MxiAllocSysPart(&pCtx->parts[OBJ_TYPE_HEAP]);

    if (pHeap == NULL)
    {
#ifdef TRACE_HEAP_CREATE
        AddHeapCtrl (pCtx, TEID_CREATE_HEAP, HNULL, MX_HEAP_ALLOC_ERROR);
#endif
        MxiRestInt(irq);
        return MX_HEAP_ALLOC_ERROR;
    }

    Mximemset(pHeap, 0, sizeof (*pHeap));

    // To point to the real heap descriptor,
    // the real heap descriptor is not located in the
    // 4GMX system context

    pHeap->HeapStorage = (LPVOID)pHeapDescr;

    MARK_HEAP_OPENED(pHeap);
    MARK_HEAP_SYNC(pHeap);

    pHeap->Sync = &pHeapDescr->Sync;

    HANDLE_INIT(h, MxGetCpuID(), OBJ_TYPE_HEAP,
                ((((UINT32) pHeap) - ((UINT32) pCtx->heaps)) / sizeof(HEAP)));

    MX_CREATE_OBJECT_LOG(h);

#ifdef MX_NAMING_ENABLED
    if (name != NULL)
    {
        if (MxiAllocName(pCtx, h, name) == FALSE)
        {
            MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_HEAP], pHeap);

#ifdef TRACE_HEAP_CREATE
            AddHeapCtrl (pCtx, TEID_CREATE_HEAP,
                         HNULL, MX_HEAP_ASSIGN_NAME_ERROR);
#endif

            MxiRestInt(irq);

            return MX_HEAP_ASSIGN_NAME_ERROR;
        }

        MARK_HEAP_NAMED(pHeap);
    }
    else
#endif /*MX_NAMING_ENABLED */
    {
        MARK_HEAP_UNNAMED(pHeap);
    }

    MARK_HEAP_ALLOC(pHeap);

#ifdef TRACE_HEAP_CREATE
    AddHeapCtrl (pCtx, TEID_CREATE_HEAP, h, MX_OK);
#endif

    *oHand = h;

    MxiRestInt(irq);

    return MX_OK;
}


/** @brief The function removes created heap.

    @param h [in] - the heap handle

    @return [MXRC] error code

    \ingroup group_lte_4gmx_heap
*/
MXRC MxRemoveHeap(HANDLE h)
{
    PMXCONTEXT pCtx;
    PHEAP pHeap;
    IRQTYPE irq;

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);

    /* Only locally created object can be removed */

    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pHeap = (PHEAP) MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeap != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeap));

#ifdef MX_NAMING_ENABLED

    if (IS_HEAP_NAMED(pHeap))
    {
        MxiFreeName(pCtx, h);

        MARK_HEAP_UNNAMED(pHeap);
    }

#endif /*MX_NAMING_ENABLED */

    pHeap->CtrlFlags = 0;

    MxiFreeSysPart(&pCtx->parts[OBJ_TYPE_HEAP], pHeap);

#ifdef TRACE_HEAP_REMOVE
    AddHeapCtrl (pCtx, TEID_REMOVE_HEAP, h, MX_OK);
#endif

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function allocates a block of memory of reqired size and returns a pointer to
   the allocated memory or NULL. The size is alignment and the memory block can be bigger that required.

    @param h    [in] - the heap handle
    @param nSize[in] - the size of block in bytes
    @param nLR  [in] - the link register address (to handle intermediate calls)

    @return [LPVOID] a pointer to allocated block or NULL.

    \ingroup group_lte_4gmx_heap
*/
#ifdef MX_HEAP_LEAKAGE_CONTROL

LPVOID MxHeapAlloc(HANDLE h, UINT32 nSize)
{
    UINT32 nLR = MX_GET_RADDR;
    return MxHeapAllocEx (h, nSize, nLR);
}

LPVOID MxHeapAllocEx(HANDLE h, UINT32 nSize, UINT32 nLR)
    
#else

LPVOID MxHeapAlloc(HANDLE h, UINT32 nSize)

#endif
{
    PMXCONTEXT pCtx;
    UINT32 dx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    PMEMBLOCK pBlock, pPrev, pNewFreeBlock;
    IRQTYPE irq;

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
    _ASSERT(nSize > 0);

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;

        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    // The allocated block has a header (an allocated size)
    // the pointer to allocated memory will be immediately after
    // block header with specified the heap alignment

    nSize += ROUND(ALLOC_HEADER_SIZE, pHeap->Align);

    // The total size should be alignment as well
    nSize = ROUND(nSize, pHeap->Align);

    if (pHeap->FreeSize < nSize)
    {

#ifdef TRACE_HEAP_ALLOC
        AddHeapDataTrace (pCtx, TEID_ALLOC_HEAP, h, (UINT32)NULL, nSize);
#endif

        if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
        {
            MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
        }

        MxiRestInt(irq);

        // this code is added specially to catch all potentially dangerous
        // situations when MxHeapAlloc returns NULL and application does not check it
        // (nSize == 0) is used to prevent compiler warnings, nSize != 0, and assert will take place
        _ASSERT (nSize == 0);

        return NULL;
    }

    pBlock = pHeap->FreeBlock;
    pPrev = NULL;

    while (pBlock != NULL)
    {
        if (pBlock->Size >= nSize)
        {
            dx = (pBlock->Size - nSize);

            if (dx < ROUND(sizeof(MEMBLOCK), pHeap->Align))
            {
                pNewFreeBlock = pBlock->NextFree;
                pHeap->FreeSize -= pBlock->Size;
                _ASSERT(pHeap->FreeSize <= pHeap->TotalSize);
                nSize = pBlock->Size;
            }
            else
            {
                // The size is aligned with heap's alingment
                // so this operation moves a pointer to the next aligned block

                pNewFreeBlock = (PMEMBLOCK)((UINT8 *) pBlock + nSize);
                pNewFreeBlock->NextFree = pBlock->NextFree;
                pNewFreeBlock->Size = pBlock->Size - nSize;

#ifdef MX_HEAP_INTEGRITY
                // to add integrity information for the new one block
                pNewFreeBlock->BlockCheckSum = CREATE_INTEGRITY(pNewFreeBlock);
#endif

                pHeap->FreeSize -= nSize;
                _ASSERT(pHeap->FreeSize <= pHeap->TotalSize);
            }

            if (pHeap->FreeBlock == pBlock)
            {
                pHeap->FreeBlock = pNewFreeBlock;
            }
            else if (pPrev != NULL)
            {
                pPrev->NextFree = pNewFreeBlock;

#ifdef MX_HEAP_INTEGRITY
                // to change integrity information for the previous one block
                pPrev->BlockCheckSum = CREATE_INTEGRITY(pPrev);
#endif
            }

            // To store allocated size (user size + size of allocated
            // block header (4 bytes))
            pBlock->Size = nSize;

#ifdef TRACE_HEAP_ALLOC
           AddHeapDataTrace (pCtx, TEID_ALLOC_HEAP, h,
                               (UINT32)&(pBlock->NextFree), nSize);
#endif

#ifdef MX_HEAP_INTEGRITY

            // to set checksum to the allocated block
            // it will help to detect any potential problems

            ((PMEM_ALLOC_BLOCK)pBlock)->BlockCheckSum = ~nSize;
#endif

#ifdef MX_HEAP_LEAKAGE_CONTROL
            MxiHeapAddBlock (pHeap, nLR, (PMEM_ALLOC_BLOCK)pBlock);
#endif

            if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
            {
                MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
            }

            MxiRestInt(irq);

            // to return the pointer to allocated memory
            return (PUINT8)pBlock + ROUND(ALLOC_HEADER_SIZE, pHeap->Align);
        }

        pPrev = pBlock;
        pBlock = pBlock->NextFree;
    }

#ifdef TRACE_HEAP_ALLOC
    AddHeapDataTrace (pCtx, TEID_ALLOC_HEAP, h, (UINT32)NULL, nSize);
#endif

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

   // this code is added specially to catch all potentially dangerous
   // situations when MxHeapAlloc returns NULL and application does not check it
   // (nSize == 0) is used to prevent compiler warnings, nSize != 0, and assert will take place
    _ASSERT (nSize == 0);

    return NULL;

}

/** @brief This function is designed to allocate memory at the specified address
            this is needed to allocate buffer at specific range to use different
            controllers and make system faster

    @param h        [in] - the heap handle
    @param nAddr    [in] - the global address that is a part of heap storage
    @param nSize    [in] - the size of block in bytes

    @return [LPVOID] the pointer to the allocated block or NULL if block cannot be allocated

    \ingroup group_lte_4gmx_heap
*/
#ifdef MX_HEAP_LEAKAGE_CONTROL

LPVOID  MxHeapAllocAt (HANDLE h, UINT32 nAddr, UINT32 nSize)
{
    UINT32 nLR = MX_GET_RADDR;
    return MxHeapAllocAtEx (h, nAddr, nSize, nLR);
}

LPVOID  MxHeapAllocAtEx (HANDLE h, UINT32 nAddr, UINT32 nSize, UINT32 nLR)
#else
LPVOID  MxHeapAllocAt (HANDLE h, UINT32 nAddr, UINT32 nSize)
#endif
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    PMEMBLOCK pBlock, pPrev, pNewFreeBlock;
    MEM_ALLOC_BLOCK * pAllocHeader;
    IRQTYPE irq;

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT (nAddr > 0);
    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
    _ASSERT(nSize > 0);

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;

        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    // the block should be aligned
    if ((nAddr & (pHeap->Align - 1)) != 0)
    {
        _ASSERT ((nAddr & (pHeap->Align - 1)) == 0);
        return NULL;
    }

    // to get the exclusive access

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    // The allocated block has a header (see:MEM_ALLOC_BLOCK)
    // the pointer to allocated memory will be immediately after
    // block header with specified the heap alignment

    nSize += ROUND(ALLOC_HEADER_SIZE, pHeap->Align);

    // The total size should be aligned as well
    nSize = ROUND(nSize, pHeap->Align);

    // the address should be prepared to locate the header
    // of allocated block
    nAddr -= ROUND(ALLOC_HEADER_SIZE, pHeap->Align);

    if (pHeap->FreeSize < nSize)
    {

#ifdef TRACE_HEAP_ALLOC
        AddHeapDataTrace (pCtx, TEID_ALLOC_HEAP, h, (UINT32)NULL, nSize);
#endif

        if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
        {
            MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
        }

        MxiRestInt(irq);

        // this code is added specially to catch all potentially dangerous
        // situations when MxHeapAllocAt returns NULL and application does not check it
        // (nSize == 0) is used to prevent compiler warnings, nSize != 0, and assert will take place
        _ASSERT (nSize == 0);

        return NULL;
    }

    pBlock = pHeap->FreeBlock;
    pPrev = NULL;

    while (pBlock != NULL)
    {
        UINT32 nBSize,  // size before allocated block in the free block
               nASize;  // size after allocated block in the free block

        // to find the free block that can be used to allocate memory at specified address
        if (((UINT32)pBlock <= nAddr) && ((nAddr + nSize) <= ((UINT32)pBlock+pBlock->Size)))
        {
            nBSize = nAddr - (UINT32)pBlock;
            nASize = (pBlock->Size - nSize) - nBSize;

            // if "after size" less than some threshold just
            // increase the size of required block
            if (nASize <= ROUND(sizeof (MEMBLOCK), pHeap->Align))
            {
                nSize += nASize;
                nASize = 0;
            }

            // to point to the next free block
            if (nBSize != 0)
            {
                _ASSERT(nBSize >= ROUND(sizeof (MEMBLOCK), pHeap->Align));

                pBlock->Size -= (nSize + nASize);

                if (nASize != 0)
                {
                    pNewFreeBlock = (MEMBLOCK*) ((UINT32)pBlock + nSize + nBSize);
                    pNewFreeBlock->Size     = nASize;
                    pNewFreeBlock->NextFree = pBlock->NextFree;

                    #ifdef MX_HEAP_INTEGRITY
                    pNewFreeBlock->BlockCheckSum = CREATE_INTEGRITY(pNewFreeBlock);
                    #endif

                    pBlock->NextFree = pNewFreeBlock;
                }

                #ifdef MX_HEAP_INTEGRITY
                pBlock->BlockCheckSum = CREATE_INTEGRITY(pBlock);
                #endif

                // create a header for allocated block
                pAllocHeader = (PMEM_ALLOC_BLOCK)((UINT32)pBlock + nBSize);
            }
            else // nBSize == 0
            {
                if (nASize != 0)
                {
                    pNewFreeBlock = (MEMBLOCK*) ((UINT32)pBlock + nSize);
                    pNewFreeBlock->Size     = nASize;
                    pNewFreeBlock->NextFree = pBlock->NextFree;

                    #ifdef MX_HEAP_INTEGRITY
                    pNewFreeBlock->BlockCheckSum = CREATE_INTEGRITY(pNewFreeBlock);
                    #endif
                }
                else
                {
                    pNewFreeBlock = pBlock->NextFree;
                }

                if ((UINT32)pHeap->FreeBlock == (UINT32)pBlock)
                {
                    pHeap->FreeBlock = pNewFreeBlock;
                }
                else if (pPrev != NULL)
                {
                    pPrev->NextFree = pNewFreeBlock;

                    #ifdef MX_HEAP_INTEGRITY
                    pPrev->BlockCheckSum = CREATE_INTEGRITY(pPrev);
                    #endif
                }

                // create a header for allocated block
                pAllocHeader = (PMEM_ALLOC_BLOCK)pBlock;
            }

            pAllocHeader->Size = nSize;
            #ifdef MX_HEAP_INTEGRITY
            pAllocHeader->BlockCheckSum = ~nSize;
            #endif

            #ifdef MX_HEAP_LEAKAGE_CONTROL
            MxiHeapAddBlock (pHeap, nLR, pAllocHeader);
            #endif

            #ifdef TRACE_HEAP_ALLOC
            AddHeapDataTrace (pCtx, TEID_ALLOC_HEAP, h,
                               (UINT32)(pAllocHeader), nSize);
            #endif

            pHeap->FreeSize -= nSize;

            if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
            {
                MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
            }

            MxiRestInt(irq);

            return (PUINT8)pAllocHeader + ROUND(ALLOC_HEADER_SIZE, pHeap->Align);
        }

        pPrev = pBlock;
        pBlock = pBlock->NextFree;
    }

#ifdef TRACE_HEAP_ALLOC
    AddHeapDataTrace (pCtx, TEID_ALLOC_HEAP, h, (UINT32)NULL, nSize);
#endif

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

   // this code is added specially to catch all potentially dangerous
   // situations when MxHeapAlloc returns NULL and application does not check it
   // (nSize == 0) is used to prevent compiler warnings, nSize != 0, and assert will take place
    _ASSERT (nSize == 0);

    return NULL;
}


/** @brief This function checks is block located in the specified range
            and if this block is located in this range, this functions returns
            a beginning of the address where new block can be allocated

    @param nBlockAddr [in] - the address of heap free block
    @param nBlockSize [in] - the size of free block in bytes
    @param nSize      [in] - the required size of allocated block
    @param nBegin     [in] - the begin address of the range
    @param nEnd       [in] - the end address of the range

    @return [UINT32] 0 or begin address of the block

    \ingroup group_lte_4gmx_heap
    */

UINT32 MxiIsBlockInRange (UINT32 nBlockAddr, UINT32 nBlockSize, UINT32 nSize, UINT32 nBegin, UINT32 nEnd)
{
    UINT32 nRangeSize = nBlockSize;

    // if out of size
    if (nBlockSize < nSize)
        return 0;

    // if out of range
    if ((nBlockAddr >= nEnd) || (nBlockAddr + nBlockSize) <= nBegin)
        return 0;

    if (nBegin > nBlockAddr)
    {
        nRangeSize -= (nBegin - nBlockAddr);
    }

    if (nEnd < (nBlockAddr + nBlockSize))
    {
        nRangeSize -= (nBlockAddr + nBlockSize) - nEnd;
    }

    if (nRangeSize < nSize)
        return 0;

    return (nBegin > nBlockAddr) ? nBegin : nBlockAddr;
}

/** @brief This function is designed to allocate memory in the specified range
            this is needed to allocate buffer at specific range to use different
            controllers and make system more faster

    @param h        [in] - the heap handle
    @param nBegin   [in] - the global address, begin point of range
    @param nEnd     [in] - the global address, end point of range
    @param nSize    [in] - the size of block in bytes

    @return [LPVOID] the pointer to the allocated block or NULL if block cannot be allocated

    \ingroup group_lte_4gmx_heap
*/
#ifdef MX_HEAP_LEAKAGE_CONTROL

LPVOID  MxHeapAllocInRange (HANDLE h, UINT32 nBegin, UINT32 nEnd, UINT32 nSize)
{
    UINT32 nLR = MX_GET_RADDR;
    return MxHeapAllocInRangeEx (h, nBegin, nEnd, nSize, nLR);
}

LPVOID  MxHeapAllocInRangeEx (HANDLE h, UINT32 nBegin, UINT32 nEnd, UINT32 nSize, UINT32 nLR)
#else
LPVOID  MxHeapAllocInRange (HANDLE h, UINT32 nBegin, UINT32 nEnd, UINT32 nSize)
#endif
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    PMEMBLOCK pBlock, pPrev, pNewFreeBlock;
    MEM_ALLOC_BLOCK * pAllocHeader;
    IRQTYPE irq;
    UINT32 nAddr;

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT (nAddr > 0);
    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
    _ASSERT(nSize > 0);

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;

        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    // the begin address of block should be aligned
    if ((nBegin & (pHeap->Align - 1)) != 0)
    {
        _ASSERT ((nBegin & (pHeap->Align - 1)) == 0);
        return NULL;
    }

    // the end address of block should be aligned
    if ((nEnd & (pHeap->Align - 1)) != 0)
    {
        _ASSERT ((nEnd & (pHeap->Align - 1)) == 0);
        return NULL;
    }

    // to get the exclusive access

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    // The allocated block has a header (see:MEM_ALLOC_BLOCK)
    // the pointer to allocated memory will be immediately after
    // block header with specified the heap alignment

    nSize += ROUND(ALLOC_HEADER_SIZE, pHeap->Align);

    // The total size should be aligned as well
    nSize = ROUND(nSize, pHeap->Align);

    if (pHeap->FreeSize < nSize)
    {

#ifdef TRACE_HEAP_ALLOC
        AddHeapDataTrace (pCtx, TEID_ALLOC_HEAP, h, (UINT32)NULL, nSize);
#endif

        if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
        {
            MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
        }

        MxiRestInt(irq);

        // this code is added specially to catch all potentially dangerous
        // situations when MxHeapAllocAt returns NULL and application does not check it
        // (nSize == 0) is used to prevent compiler warnings, nSize != 0, and assert will take place
        _ASSERT (nSize == 0);

        return NULL;
    }

    pBlock = pHeap->FreeBlock;
    pPrev = NULL;

    while (pBlock != NULL)
    {
        UINT32 nBSize,  // size before allocated block in the free block
               nASize;  // size after allocated block in the free block

        // to find the free block that can be used to allocate memory at specified address
        if ( (nAddr = MxiIsBlockInRange ((UINT32)pBlock, pBlock->Size, nSize, nBegin, nEnd)) != 0 )
        {
            nBSize = nAddr - (UINT32)pBlock;
            nASize = (pBlock->Size - nSize) - nBSize;

            // if "after size" less than some threshold just
            // increase the size of required block
            if (nASize <= ROUND(sizeof (MEMBLOCK), pHeap->Align))
            {
                nSize += nASize;
                nASize = 0;
            }

            // to point to the next free block
            if (nBSize != 0)
            {
                _ASSERT(nBSize >= ROUND(sizeof (MEMBLOCK), pHeap->Align));

                pBlock->Size -= (nSize + nASize);

                if (nASize != 0)
                {
                    pNewFreeBlock = (MEMBLOCK*) ((UINT32)pBlock + nSize + nBSize);
                    pNewFreeBlock->Size     = nASize;
                    pNewFreeBlock->NextFree = pBlock->NextFree;

                    #ifdef MX_HEAP_INTEGRITY
                    pNewFreeBlock->BlockCheckSum = CREATE_INTEGRITY(pNewFreeBlock);
                    #endif

                    pBlock->NextFree = pNewFreeBlock;
                }

                #ifdef MX_HEAP_INTEGRITY
                pBlock->BlockCheckSum = CREATE_INTEGRITY(pBlock);
                #endif

                // create a header for allocated block
                pAllocHeader = (PMEM_ALLOC_BLOCK)((UINT32)pBlock + nBSize);
            }
            else // nBSize == 0
            {
                if (nASize != 0)
                {
                    pNewFreeBlock = (MEMBLOCK*) ((UINT32)pBlock + nSize);
                    pNewFreeBlock->Size     = nASize;
                    pNewFreeBlock->NextFree = pBlock->NextFree;

                    #ifdef MX_HEAP_INTEGRITY
                    pNewFreeBlock->BlockCheckSum = CREATE_INTEGRITY(pNewFreeBlock);
                    #endif
                }
                else
                {
                    pNewFreeBlock = pBlock->NextFree;
                }

                if ((UINT32)pHeap->FreeBlock == (UINT32)pBlock)
                {
                    pHeap->FreeBlock = pNewFreeBlock;
                }
                else if (pPrev != NULL)
                {
                    pPrev->NextFree = pNewFreeBlock;

                    #ifdef MX_HEAP_INTEGRITY
                    pPrev->BlockCheckSum = CREATE_INTEGRITY(pPrev);
                    #endif
                }

                // create a header for allocated block
                pAllocHeader = (PMEM_ALLOC_BLOCK)pBlock;
            }

            pAllocHeader->Size = nSize;
            #ifdef MX_HEAP_INTEGRITY
            pAllocHeader->BlockCheckSum = ~nSize;
            #endif

            #ifdef MX_HEAP_LEAKAGE_CONTROL
            MxiHeapAddBlock (pHeap, nLR, pAllocHeader);
            #endif

            #ifdef TRACE_HEAP_ALLOC
            AddHeapDataTrace (pCtx, TEID_ALLOC_HEAP, h,
                               (UINT32)(pAllocHeader), nSize);
            #endif

            pHeap->FreeSize -= nSize;

            if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
            {
                MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
            }

            MxiRestInt(irq);

            return (PUINT8)pAllocHeader + ROUND(ALLOC_HEADER_SIZE, pHeap->Align);
        }

        pPrev = pBlock;
        pBlock = pBlock->NextFree;
    }

#ifdef TRACE_HEAP_ALLOC
    AddHeapDataTrace (pCtx, TEID_ALLOC_HEAP, h, (UINT32)NULL, nSize);
#endif

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

   // this code is added specially to catch all potentially dangerous
   // situations when MxHeapAlloc returns NULL and application does not check it
   // (nSize == 0) is used to prevent compiler warnings, nSize != 0, and assert will take place
    _ASSERT (nSize == 0);

    return NULL;
}



/** @brief The function frees a block of heap memory

    @param h        [in] - the heap handler
    @param pBlock   [in] - the pointer to the heap memory block

    @return [MXRC] an error code of operation

    \ingroup group_lte_4gmx_heap
*/
MXRC MxHeapFree(HANDLE h, LPVOID pData)
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    IRQTYPE irq;
    PMEMBLOCK pBlock, pTmp, pBeginPack = NULL;
    UINT32 Num;

    DEF_ASSERT_LR();

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT_LR(HANDLE_NOT_NULL(h));
    _ASSERT_LR(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT_LR(HANDLE_GET_CPU(h) == MxGetCpuID());
    _ASSERT_LR(pData != NULL);

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT_LR(pHeapObj != NULL);
    _ASSERT_LR(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;
        _ASSERT_LR(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    pBlock = (PMEMBLOCK)((UINT8 *)pData - ROUND(ALLOC_HEADER_SIZE, pHeap->Align));
    _ASSERT_LR((UINT32)pBlock >= (UINT32)pHeap->HeapStorage);

#ifdef MX_HEAP_INTEGRITY
    if (((PMEM_ALLOC_BLOCK)pBlock)->Size != ~((PMEM_ALLOC_BLOCK)pBlock)->BlockCheckSum)
    {
        _ASSERT_LR (((PMEM_ALLOC_BLOCK)pBlock)->Size == ~((PMEM_ALLOC_BLOCK)pBlock)->BlockCheckSum);
        return MX_HEAP_INTEGRITY_ERROR;
    }
#endif

#ifdef MX_HEAP_LEAKAGE_CONTROL
    MxiHeapRemoveBlock (pHeap, (MEM_ALLOC_BLOCK*)pBlock);
#endif

    // The asserts are defined for _MX_DEBUG mode but this condition is implemented
    // to take attention to the pHeap->TotalSize variable that is defined for _MX_DEBUG mode only
    _ASSERT_LR( (UINT32)pBlock < ((UINT32)pHeap->HeapStorage + pHeap->TotalSize));
    _ASSERT_LR( (UINT32)pBlock >=((UINT32)pHeap->HeapStorage));

    if ((UINT32)pBlock >= ((UINT32)pHeap->HeapStorage + pHeap->TotalSize) || (UINT32)pBlock < (UINT32)pHeap->HeapStorage)
    {
        if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
        {
            MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
        }

        MxiRestInt(irq);
        
        return MX_HEAP_OUT_OF_STORAGE;
    }

#ifdef TRACE_HEAP_FREE
       AddHeapDataTrace (pCtx, TEID_FREE_HEAP, h, (UINT32)pData, pBlock->Size);
#endif

    pHeap->FreeSize += pBlock->Size;
    _ASSERT_LR(pHeap->FreeSize <= pHeap->TotalSize);
    if ((UINT32) pBlock < (UINT32) pHeap->FreeBlock || pHeap->FreeBlock == NULL)
    {
        pBlock->NextFree = pHeap->FreeBlock;
        pHeap->FreeBlock = pBlock;
        pBeginPack = pHeap->FreeBlock;

#ifdef MX_HEAP_INTEGRITY
        pBlock->BlockCheckSum = CREATE_INTEGRITY(pBlock);
#endif

    }
    else
    {
        // Look for the best place in the chain of free blocks
        pTmp = pHeap->FreeBlock;
        while (pTmp)
        {
            if ((UINT32) pTmp < (UINT32) pBlock
                    && ((UINT32) pTmp->NextFree > (UINT32) pBlock
                        || pTmp->NextFree == NULL))
            {
                pBeginPack = pTmp;
                _ASSERT_LR((UINT32)pTmp >= (UINT32)pHeap->HeapStorage);

                // The asserts are defined for _MX_DEBUG mode but this condition is implemented
                // to take attention to the pHeap->TotalSize variable that is defined for _MX_DEBUG mode only
                _ASSERT_LR( (UINT32)pTmp < ((UINT32)pHeap->HeapStorage + pHeap->TotalSize));

                // This condition is done to check NextFree pointer, this pointer should be in
                // the heap space and if it points to outside, it means, the heap is corrupted
                _ASSERT_LR( (UINT32)pTmp->NextFree < ((UINT32)pHeap->HeapStorage + pHeap->TotalSize));
                pBlock->NextFree = pTmp->NextFree;

#ifdef MX_HEAP_INTEGRITY
                pBlock->BlockCheckSum = CREATE_INTEGRITY(pBlock);
#endif
                // This is a sign of corrupted heap (will go to infinite loop then)
                _ASSERT_LR(pTmp->NextFree != pBlock);

                pTmp->NextFree = pBlock;

#ifdef MX_HEAP_INTEGRITY
                pTmp->BlockCheckSum = CREATE_INTEGRITY(pTmp);
#endif

                break;
            }

            pTmp = pTmp->NextFree;

            // This condition is done to check NextFree pointer, this pointer should be in
            // the heap space and if it points to outside, it means, the heap is corrupted
            _ASSERT_LR( (UINT32)pTmp < ((UINT32)pHeap->HeapStorage + pHeap->TotalSize));
        }
    }

    Num = 0;

    // Pack adjacent blocks
    // Number of iterations is less than 3!
    while (pBeginPack)
    {
        while (((UINT32) pBeginPack + pBeginPack->Size) ==
                (UINT32) pBeginPack->NextFree)
        {
            pBeginPack->Size += pBeginPack->NextFree->Size;
            pBeginPack->NextFree = pBeginPack->NextFree->NextFree;

#ifdef MX_HEAP_INTEGRITY
            pBeginPack->BlockCheckSum = CREATE_INTEGRITY(pBeginPack);
#endif
        }

        pBeginPack = (++Num < 2) ? pBeginPack->NextFree : NULL;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return MX_OK;
}

/** @brief The function returns total free size of the heap in bytes

    @NOTE: because of possible fragmentation, the heap may return NULL
            for allocation of block with this size

    @param h [in] - the heap descriptor

    @return [UINT32] the size of heap

    \ingroup group_lte_4gmx_heap
*/
UINT32 MxHeapGetFreeSpace (HANDLE h)
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    IRQTYPE irq;
    UINT32 nSize = 0;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;

        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    nSize = pHeap->FreeSize;

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return nSize;
}

/** @brief The function returns total storage size of the heap in bytes

    @param h [in] - the heap descriptor

    @return [UINT32] the total size of heap in bytes

    \ingroup group_lte_4gmx_heap
*/
UINT32 MxHeapGetTotalSize (HANDLE h)
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    IRQTYPE irq;
    UINT32 nSize = 0;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;

        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    nSize = pHeap->TotalSize;

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return nSize;
}


/** @brief The function returns total free size of the heap in bytes

    @NOTE: because of possible fragmentation, the heap may return NULL
            for allocation of block with this size

    @param h [in] - the heap descriptor

    @return [UINT32] the size of heap

    \ingroup group_lte_4gmx_heap
*/
UINT32 MxHeapGetMaxFreeBlockSize (HANDLE h)
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    IRQTYPE irq;
    UINT32 nSize = 0;

    PMEMBLOCK pBlock;

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;
        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    pBlock = pHeap->FreeBlock;

    while (pBlock != NULL)
    {
         nSize = MAX(nSize, pBlock->Size);
         pBlock = pBlock->NextFree;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return nSize;
}

/** @brief This function returns size of allocated block

    @param h [in] the heap handler
    @param pData [in] the pointer to the allacted memory

    @return [UINT32] the size of block

    \ingroup group_lte_4gmx_heap
*/
UINT32 MxHeapGetBlockSize (HANDLE h, LPVOID pData)
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    PMEMBLOCK pBlock;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
    _ASSERT(pData != NULL);

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;
        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    pBlock = (PMEMBLOCK)((UINT8 *)pData - ROUND(ALLOC_HEADER_SIZE, pHeap->Align));

    return pBlock->Size;

}

#ifdef MX_HEAP_INTEGRITY

/** @brief This function checks heap integrity
           by analyzing free heap free blocks

    @param h [in] - the heap descriptor
    @param pBlockAddr [out] - the address of error block, it can be null

    @return [MXRC] an error code

    \ingroup group_lte_4gmx_heap
*/
MXRC MxHeapCheckIntegrity(HANDLE h, LPVOID * pBlockAddr)
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    IRQTYPE irq;
    MXRC rc = MX_OK;

    PMEMBLOCK pBlock;

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;
        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    pBlock = pHeap->FreeBlock;

    while (pBlock != NULL)
    {
        if (GET_INTEGRITY(pBlock) != CREATE_INTEGRITY(pBlock))
        {
            rc = MX_HEAP_INTEGRITY_ERROR;

            if (pBlockAddr != NULL)
            {
                *pBlockAddr = pBlock;
            }

            break;
        }

        pBlock = pBlock->NextFree;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return rc;
}

/** @brief This function is designed to check heap block header CRC
           and to return the status of this CRC

    @param hHeap [in] - the heap handle 
    @param pPtr  [in] - the pointer to the allocated by MxHeapAlloc function block in this heap

    @return [UINT32] see MX_HEAP_INTEGRITY_xxx */

UINT32 MxHeapBlockIntegrity (HANDLE h, void * pPtr)
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    IRQTYPE irq;
    MXRC rc = MX_HEAP_INTEGRITY_OK;
    MEM_ALLOC_BLOCK *pBlock;

    DEF_ASSERT_LR();

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;
        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    pBlock = (MEM_ALLOC_BLOCK*)((UINT8 *)pPtr - ROUND(ALLOC_HEADER_SIZE, pHeap->Align));
    _ASSERT_LR((UINT32)pBlock >= (UINT32)pHeap->HeapStorage);

    if (pBlock->BlockCheckSum != ~pBlock->Size)
    {
        rc = MX_HEAP_INTEGRITY_BAD;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return rc;
}

#else 

UINT32 MxHeapBlockIntegrity (HANDLE hHeap, void * pPtr)
{
    return MX_HEAP_INTEGRITY_OFF;
}

#endif

#ifdef MX_HEAP_LEAKAGE_CONTROL

/** @brief This function returns number of allocated block for this moment

    @return [UINT32] the number of allocated blocks

    \ingroup group_lte_4gmx_heap
*/
UINT32 MxHeapGetAllocNum (HANDLE h)
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    IRQTYPE irq;
    UINT32 nNum = 0;
    MEM_ALLOC_BLOCK *pBlock;

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;
        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    pBlock = pHeap->AllocBlock;

    while (pBlock != NULL)
    {
        nNum++;
        pBlock = pBlock->NextAllocated;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return nNum;
}

/** @brief This function returns the address of code that allocated some
           block of memory.

    @param h       [in]        - the heap descriptor
    @return [MEM_ALLOC_BLOCK] the allocated block information

    \ingroup group_lte_4gmx_heap
*/
MEM_ALLOC_BLOCK* MxHeapGetAllocChain (HANDLE h)
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    IRQTYPE irq;
    MEM_ALLOC_BLOCK * pBlock = NULL;

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;
        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    pBlock = pHeap->AllocBlock;

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return pBlock;
}

/** @brief This function returns the address of the heap memory returned to application.

    @param h    [in]    - the heap descriptor
    @param p    [in]    - the pointer to the information block 
    
    @return [LPVOID] the pointer to the allocated block

    \ingroup group_lte_4gmx_heap
*/
LPVOID MxHeapGetAppBlockPtr (HANDLE h, MEM_ALLOC_BLOCK* p)
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    IRQTYPE irq;
    UINT32 ptr = 0;

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;
        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    ptr = ((UINT32)p) + ROUND(sizeof (MEM_ALLOC_BLOCK), pHeap->Align);

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return (LPVOID)ptr;
}

/** @brief This function returns the size of allocated by application block.

    @param h    [in]    - the heap descriptor
    @param p    [in]    - the pointer to the allocated information block
    
    @return [UINT32] the size of allocated block

    \ingroup group_lte_4gmx_heap
*/
UINT32 MxHeapGetAppBlockSize (HANDLE h, MEM_ALLOC_BLOCK* p)
{
    _ASSERT_PTR(p);

    return p->Size;
}

/** @brief This function returns the size of storage in bytes, needed to store 
               information that represents current heap state

    @param h    [in]    - the heap descriptor
    
    @return [UINT32] the size in bytes

    \ingroup group_lte_4gmx_heap
*/

UINT32 MxHeapGetPacketSize (HANDLE h)
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    IRQTYPE irq;
    UINT32 size = sizeof (PACKEDHEAPHEADER);
    MEM_ALLOC_BLOCK * pBlock = NULL;

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;
        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }


    pBlock = pHeap->AllocBlock;

    while (pBlock != NULL)
    {
        size += sizeof (PACKEDALLOCHEADER);

        pBlock = pBlock->NextAllocated;
    }
    
    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return size;
}

/** @brief This function packs heap to be analyzed on the remote side

    @param h            [in]    - the heap descriptor
    @param nHeapID      [in]    - the heap ID, provided by the application
    @param pPackStor    [out]   - the pointer to the storage 
    @param nStorSize    [in]    - the size of storage in bytes
    
    @return [MXRC] an error code

    \ingroup group_lte_4gmx_heap
*/

MXRC MxHeapPack (HANDLE h, UINT32 nHeapID, LPVOID pPackStor, UINT32 nStorSize)
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    IRQTYPE irq;
    MEM_ALLOC_BLOCK * pBlock = NULL;
    PUINT8 pStor;
    MXRC rc = MX_OK;

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
    
    _ASSERT_PTR(pPackStor);
    _ASSERT (nStorSize >= sizeof (PACKEDHEAPHEADER));

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;
        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    pStor = (PUINT8)pPackStor;

    memset (pStor, 0, sizeof (PACKEDHEAPHEADER));

    ((PPACKEDHEAPHEADER)pStor)->Version     = 0x0101;
    ((PPACKEDHEAPHEADER)pStor)->HeapID      = nHeapID;
    ((PPACKEDHEAPHEADER)pStor)->BaseAddr    = (UINT32)pHeap->HeapStorage;
    ((PPACKEDHEAPHEADER)pStor)->HeapSize    = (UINT32)pHeap->TotalSize;
    ((PPACKEDHEAPHEADER)pStor)->HeapAlign   = pHeap->Align;
    ((PPACKEDHEAPHEADER)pStor)->AllocNum    = 0;

    pStor += sizeof (PACKEDHEAPHEADER);

    pBlock = pHeap->AllocBlock;

    while (pBlock != NULL)
    {
        ((PPACKEDALLOCHEADER)pStor)->BaseAddr = (UINT32)pBlock + ROUND(sizeof (MEM_ALLOC_BLOCK), pHeap->Align);
        ((PPACKEDALLOCHEADER)pStor)->Size     = pBlock->Size;
        ((PPACKEDALLOCHEADER)pStor)->CpuID    = pBlock->AllocCpuID;
        ((PPACKEDALLOCHEADER)pStor)->CpuAddr  = pBlock->AllocAddr;

        ((PPACKEDHEAPHEADER)pPackStor)->AllocNum++;

        pStor += sizeof (PACKEDALLOCHEADER);
        pBlock = pBlock->NextAllocated;

        if (((UINT32)pStor - (UINT32)pPackStor) > nStorSize)
        {
            rc = MX_HEAP_OUT_OF_STORAGE;
            break;
        }
    }
    
    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return rc;
}


/** @brief This function sets unique ID for the new allocated blocks, 
               these blocks can be easily found and deallocated, in this way we may 
               store heap state 

        @param h        [in] - the heap handle
        @param blockid  [in] - the unique id

        @return [MXRC] an error code

        \ingroup group_lte_4gmx_heap

        */

MXRC MxHeapMarkBlocks (HANDLE h, UINT32 blockid)
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    IRQTYPE irq;
    MXRC rc = MX_OK;

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());
    
    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;
        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    pHeap->MarkID = blockid;
    
    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return rc;
    
}

/** @brief This function is designed to find the allocated blocks
           marked with unique ID and to free these blocks

    @note Please see MxHeapMarkBlocks function.

    @param h        [in] - the heap handle
    @param blockid  [in] - the unique id

    @return [MXRC] an error code 

    \ingroup group_lte_4gmx_heap

    */

MXRC MxHeapFreeBlocks (HANDLE h, UINT32 blockid)
{
    MEM_ALLOC_BLOCK* pBlock;
    UINT32 bLoop = 1;

    while (bLoop)
    {
        bLoop = 0;
    
        pBlock = MxHeapGetAllocChain (h);

        while (pBlock != NULL)
        {
            if (pBlock->MarkID == blockid)
            {
                MxHeapFree (h, MxHeapGetAppBlockPtr(h, pBlock));
                
                bLoop = 1;
                break;
            }
            
            pBlock = pBlock->NextAllocated;
        }
    }

    return MX_OK;
}

#endif


/** @brief This function is designed to scan heap free blocks

    @param h        [in]     - the heap handle
    @param bFirst   [in]     - TRUE - to get the first free block, FALSE - to continue scanning
    @param pBlock   [in/out] - in: the address of previous block (if bFirst is FALSE)
                              out: the address of the next block (first block if bFirst is TRUE)

    @return [MXRC] an error code

    \ingroup group_lte_4gmx_heap

    */

MXRC MxHeapScan (HANDLE h, UINT32 bFirst, PMEMBLOCK * pBlock)
{
    PMXCONTEXT pCtx;
    PHEAP pHeapObj;
    ICHEAP * pHeap;
    IRQTYPE irq;

    _ASSERT_PTR(pBlock);

    pCtx = MxInqContext();
    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    if (HANDLE_NOT_NULL(h) == FALSE)
    {
        h = pCtx->hDefHeap;
    }

    _ASSERT(HANDLE_NOT_NULL(h));
    _ASSERT(HANDLE_GET_TYPE(h) == OBJ_TYPE_HEAP);
    _ASSERT(HANDLE_GET_CPU(h) == MxGetCpuID());

    pHeapObj = MxiGetHeapDescr(pCtx, h);

    _ASSERT(pHeapObj != NULL);
    _ASSERT(IS_HEAP_ALLOC(pHeapObj));

    if (IS_HEAP_OPENED(pHeapObj))
    {
        pHeap = (ICHEAP*)pHeapObj->HeapStorage;
        _ASSERT(pHeap != NULL);
    }
    else
    {
        pHeap = (ICHEAP*)pHeapObj;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOn(OBJ_TYPE_HEAP, pHeapObj);
    }

    if (bFirst != 0)
    {
        *pBlock = pHeap->FreeBlock;
    }
    else
    {
        if ((*pBlock) != NULL)
            (*pBlock) = (*pBlock)->NextFree;
    }

    if (IS_HEAP_OPENED(pHeapObj) || IS_HEAP_SYNC(pHeapObj))
    {
        MxiSyncCtrlOff(OBJ_TYPE_HEAP, pHeapObj);
    }

    MxiRestInt(irq);

    return (*pBlock) ? MX_OK : MX_HEAP_NO_MORE_ITEMS;
}

#ifdef MX_INTERCPU_ENABLED

/** @brief This function shares a heap

    @param hPart  [in] - the heap handler
    @param pLock  [in] - the address of the sync object (in non cached memory)
    @return [MXRC] error code

    \ingroup group_lte_4gmx_heap
*/
MXRC MxShareHeap (HANDLE INP hHepa, PVUINT32 pLock)
{
    return MX_OK;
}

/** @brief This function unshares a heap and this heap cannot be accessed
           from the other 4GMX systems

    @param hHeap [in]  - the heap handler

    @return [MXRC] error code

    \ingroup group_lte_4gmx_heap
*/
MXRC MxUnshareHeap (HANDLE INP hHeap)
{
    return MX_OK;
}

#endif /*MX_INTERCPU_ENABLED*/


#endif /*MX_HEAP_ENABLED */

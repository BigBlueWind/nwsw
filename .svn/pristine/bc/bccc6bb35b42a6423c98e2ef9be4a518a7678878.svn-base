//-------------------------------------------------------------------------------------------
/** @file 4gmx_heap.h
 *
 * @brief Heaps API declaration
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_HEAP_H_
#define _4GMX_HEAP_H_

#include "4gmx_config.h"

#ifdef MX_HEAP_ENABLED

#ifdef __cplusplus
extern "C"
{
#endif

#include "4gmx_types.h"
#include "4gmx_const.h"

/************************************************************************/
/*       The list of module error codes                                 */
/************************************************************************/
#define MX_HEAP_ALLOC_ERROR          DEF_MXRC(MXM_HEAP,  1)
#define MX_HEAP_ASSIGN_NAME_ERROR    DEF_MXRC(MXM_HEAP,  2)
#define MX_HEAP_INTEGRITY_ERROR      DEF_MXRC(MXM_HEAP,  3)
#define MX_HEAP_NO_MORE_ITEMS        DEF_MXRC(MXM_HEAP,  4)
#define MX_HEAP_OUT_OF_STORAGE       DEF_MXRC(MXM_HEAP,  5)

/************************************************************************/
/*      The heap control flags                                          */
/************************************************************************/
#define HEAP_CF_ALLOCATED           (1<<0)
#define HEAP_CF_NAMED               (1<<1)
#define HEAP_CF_SYNC                (1<<2)
#define HEAP_CF_OPEN                (1<<3)

#define MARK_HEAP_ALLOC(HEAP)       ((HEAP)->CtrlFlags |= HEAP_CF_ALLOCATED)
#define MARK_HEAP_DEALLOC(HEAP)     ((HEAP)->CtrlFlags &= ~HEAP_CF_ALLOCATED)
#define IS_HEAP_ALLOC(HEAP)         ((HEAP)->CtrlFlags & HEAP_CF_ALLOCATED)

#define MARK_HEAP_NAMED(HEAP)       ((HEAP)->CtrlFlags |= HEAP_CF_NAMED)
#define MARK_HEAP_UNNAMED(HEAP)     ((HEAP)->CtrlFlags &= ~HEAP_CF_NAMED)
#define IS_HEAP_NAMED(HEAP)         ((HEAP)->CtrlFlags & HEAP_CF_NAMED)

#define MARK_HEAP_SYNC(HEAP)        ((HEAP)->CtrlFlags |= HEAP_CF_SYNC)
#define MARK_HEAP_UNSYNC(HEAP)      ((HEAP)->CtrlFlags &= ~HEAP_CF_SYNC)
#define IS_HEAP_SYNC(HEAP)          ((HEAP)->CtrlFlags & HEAP_CF_SYNC)

#define MARK_HEAP_OPENED(HEAP)      ((HEAP)->CtrlFlags |= HEAP_CF_OPEN)
#define MARK_HEAP_UNOPENED(HEAP)    ((HEAP)->CtrlFlags &= ~HEAP_CF_OPEN)
#define IS_HEAP_OPENED(HEAP)        ((HEAP)->CtrlFlags & HEAP_CF_OPEN)

/***************************************************************************/

#define ALLOC_HEADER_SIZE (sizeof(MEM_ALLOC_BLOCK))
#define CREATE_INTEGRITY(block)  (~(((UINT32)(block)->Size) ^ ((UINT32)(block)->NextFree)))
#define GET_INTEGRITY(block)     ((block)->BlockCheckSum)

#define MX_HEAP_INTEGRITY_OK        0   // Block header CRC is OK
#define MX_HEAP_INTEGRITY_BAD       1   // Block header CRC is bad
#define MX_HEAP_INTEGRITY_OFF       2   // The integrity feature is turned off

/************************************************************************/
/*           The heap type descriptors definition                       */
/************************************************************************/

typedef struct _MEM_BLOCK_
{
    UINT32 Size;                  /**< The size of memory block in bytes   */
    struct _MEM_BLOCK_ *NextFree; /**< The pointer to the next free memory block */

#ifdef MX_HEAP_INTEGRITY
    UINT32 BlockCheckSum;         /**< This value is a checksum of Size and NextFree to catch data overwriting*/
#endif

} MEMBLOCK, *PMEMBLOCK;

/** @brief This block defines header of allocate block */

typedef struct _MEM_ALLOC_BLOCK_
{
    UINT32 Size;                  /**< The size of memory block in bytes   */

#ifdef MX_HEAP_INTEGRITY
    UINT32 BlockCheckSum;         /**< This value is a checksum of Size and NextFree to catch data overwriting*/
#endif

#ifdef MX_HEAP_LEAKAGE_CONTROL

    struct _MEM_ALLOC_BLOCK_ * NextAllocated;
    struct _MEM_ALLOC_BLOCK_ * PrevAllocated;

    UINT32 AllocAddr;            /**< The caller address, it's used to point to the code allocated this block*/
    UINT32 AllocCpuID;           /**< The CPU ID from where an allocator called MxHeapAlloc*/
    UINT32 MarkID;               /**< The unique value used to mark the allocated block*/
#endif

}MEM_ALLOC_BLOCK, *PMEM_ALLOC_BLOCK;

typedef struct _MX_HEAP_
{
    LPVOID HeapStorage;   /**< The pointer to the heap storage          */
    PMEMBLOCK FreeBlock;  /**< The pointer to the first free memory block of heap (it can be null) */
    UINT32 FreeSize;      /**< The total size of free space in bytes         */
    UINT32 Align;         /**< The data alignment in bytes           */
    MXSYNC Sync;          /**< The synchronization object, used for shared objects  */
    UINT32 TotalSize;     /** Moved here for pointer compatibility between PHEAP and PICHEAP */

#ifdef MX_HEAP_LEAKAGE_CONTROL
    PMEM_ALLOC_BLOCK AllocBlock; /**< The pointer to the first allocated block*/
    UINT32           MarkID;     /**< The unique value used to mark the allocated blocks*/
#endif

    UINT32 CtrlFlags;     /**< The object control flags      */
} HEAP, *PHEAP;

typedef struct _MX_ICHEAP_
{
    LPVOID HeapStorage;   /**< The pointer to the heap storage                      */
    PMEMBLOCK FreeBlock;  /**< The pointer to the first free memory block of heap (it can be null) */
    UINT32 FreeSize;      /**< The total size of free space in bytes                */
    UINT32 Align;         /**< The data alignment in bytes                          */
    UINT32 Sync;          /**< The synchronization object, used for shared objects  */
    UINT32 TotalSize;

#ifdef MX_HEAP_LEAKAGE_CONTROL
    PMEM_ALLOC_BLOCK AllocBlock; /**< The pointer to the first allocated block*/
    UINT32           MarkID;     /**< The unique value used to mark the allocated blocks*/
#endif

}ICHEAP, *PICHEAP;

/** @brief The header of the packet information, used by the system to 
           provide the base information about some heap */
           
typedef struct _PACKED_HEAP_HEADER_
{
    UINT32  Version;        /**< The format version                     */
    UINT32  HeapID;         /**< The heap ID provided by the application*/
    UINT32  BaseAddr;       /**< The base address of the storage        */
    UINT32  HeapSize;       /**< The heap storage size in bytes         */
    UINT32  HeapAlign;      /**< The data alignment                     */
    UINT32  AllocNum;       /**< The number of allocated bloks          */
    UINT32  Res [2];        /**< The reserved fields                    */

}PACKEDHEAPHEADER, *PPACKEDHEAPHEADER;

/** @brief The header of allocated block, this is descriptor used */
           
typedef struct _PACKED_ALLOC_HEADER_
{
    UINT32  BaseAddr;       /**< The address of allocated block                     */
    UINT32  Size;           /**< The size of allocated block in bytes               */
    UINT32  CpuID;          /**< The CPU ID where allocator was called              */
    UINT32  CpuAddr;        /**< The ARM code address where MxHeapAllocXXX called   */
    UINT32  Res [2];        /**< Reserved                                           */

}PACKEDALLOCHEADER, *PPACKEDALLOCHEADER;


/** @brief The function creates a heap.

 @param pMem   [in] - the pointer to memory (heap storage)
 @param nSize  [in] - the storage size in bytes
 @param nAlign  [in] - the alignment in bytes
 @param oHand  [out] - the handle of created object

 @return [MXRC] error code
*/
MXRC MxCreateHeap(LPVOID pMem, UINT32 nSize, UINT32 nAlign,
                  const char *name, HANDLE * oHand);

#ifdef MX_HEAP_LEAKAGE_CONTROL

/** @brief The function creates a heap with a leakage control.

 @param pMem        [in] - the pointer to memory (heap storage)
 @param nSize       [in] - the storage size in bytes
 @param nAlign      [in] - the alignment in bytes
 @param recStorage  [in] - the pointer to the storage of leakage control
 @param nStorSize   [in] - the storage size in bytes
 @param oHand       [out] - the handle of created object

 @return [MXRC] error code
*/
MXRC MxCreateHeapLC(LPVOID pMem, UINT32 nSize, UINT32 nAlign, const char *name,
                  PUINT32 recStorage, UINT32 nStorSize, HANDLE * oHand);

#endif //MX_HEAP_LEAKAGE_CONTROL

/** @brief The function creates a shared heap. The heap descriptor is not located
           in the 4GMX context, this descriptor can be used to open
           a heap by other 4GMX system(s)

 @param pHeapDescr  [in] - the pointer to the heap descriptor that will be initialized by this function
 @param pMem        [in] - the pointer to memory (heap storage)
 @param nSize       [in] - the storage size in bytes
 @param nAlign      [in] - the alignment in bytes
 @param oHand       [out] - the handle of created object

 @return [MXRC] error code
*/
MXRC MxCreateSharedHeap(ICHEAP * pHeapDescr, LPVOID pMem, UINT32 nSize, UINT32 nAlign,
                  const char *name, HANDLE * oHand);

#ifdef MX_HEAP_LEAKAGE_CONTROL

/** @brief The function creates a shared heap with leakage control. The heap descriptor is not located
           in the 4GMX context, this descriptor can be used to open
           a heap by other 4GMX system(s)

 @param pHeapDescr  [in] - the pointer to the heap descriptor that will be initialized by this function
 @param pMem        [in] - the pointer to memory (heap storage)
 @param nSize       [in] - the storage size in bytes
 @param nAlign      [in] - the alignment in bytes
 @param recStorage  [in] - the pointer to the storage of leakage control
 @param nStorSize   [in] - the storage size in bytes
 @param oHand       [out]- the handle of created object

 @return [MXRC] error code
*/
MXRC MxCreateSharedHeapLC(ICHEAP * pHeapDescr, LPVOID pMem, UINT32 nSize, UINT32 nAlign,
                  const char *name, PUINT32 recStorage, UINT32 nStorSize, HANDLE * oHand);

#endif //MX_HEAP_LEAKAGE_CONTROL

/** @brief The function "opens"/"set connections with" a created heap and returns its handle,
   the opened object can be used like a locally created.

 @param pHeapDescr  [in] - the pointer to the inter-cpu heap descriptor
 @param name        [in] - the associated name of opened object
 @param oHand       [out]- the handle of opened partition

 @return [MXRC] error code */

MXRC MxOpenHeap(ICHEAP * pHeapDescr, const char *name, HANDLE * oHand);

/** @brief The function removes created heap.
 @param h [in] - the heap handle
 @return [MXRC] error code */

MXRC MxRemoveHeap(HANDLE h);

#ifdef MX_HEAP_LEAKAGE_CONTROL

    /** @brief The function allocates a block of memory of reqired size and returns a pointer to
       the allocated memory or NULL. The size is alignment and the memory block can be bigger that required.

     @param h    [in] - the heap handle
     @param nSize[in] - the size of block in bytes
     @param nLR  [in] - the link register address (to handle intermediate calls)

     @return [LPVOID] a pointer to allocated block or NULL. */

    LPVOID MxHeapAllocEx(HANDLE h, UINT32 nSize, UINT32 nLR);


    /** @brief The function allocates a block of memory of reqired size and returns a pointer to
       the allocated memory or NULL. The size is alignment and the memory block can be bigger that required.

     @param h [in] - the heap handle
     @param nSize[in] - the size of block in bytes

     @return [LPVOID] a pointer to allocated block or NULL. */

    LPVOID MxHeapAlloc(HANDLE h, UINT32 nSize);

    /** @brief This function is designed to allocate memory at the specified address
                this is needed to allocate buffer at specific range to use different
                controllers and make system faster

        @param h        [in] - the heap handle
        @param nAddr    [in] - the global address that is a part of heap storage
        @param nSize    [in] - the size of block in bytes
        @param nLR      [in] - the link register address (the address of caller)

        @return [LPVOID] the pointer to the allocated block or NULL if block cannot be allocated*/

    LPVOID  MxHeapAllocAtEx (HANDLE h, UINT32 nAddr, UINT32 nSize, UINT32 nLR);

    /** @brief This function is designed to allocate memory at the specified address
                this is needed to allocate buffer at specific range to use different
                controllers and to make the system more faster

        @param h        [in] - the heap handle
        @param nAddr    [in] - the global address that is a part of heap storage
        @param nSize    [in] - the size of block in bytes

        @return [LPVOID] the pointer to the allocated block or NULL if block cannot be allocated*/

    LPVOID  MxHeapAllocAt (HANDLE h, UINT32 nAddr, UINT32 nSize);

    /** @brief This function is designed to allocate memory in the specified range
            this is needed to allocate buffer at specific range to use different
            controllers and make system more faster

        @param h        [in] - the heap handle
        @param nBegin   [in] - the global address, begin point of range
        @param nEnd     [in] - the global address, end point of range
        @param nSize    [in] - the size of block in bytes
        @param nLR      [in] - the link register provided by the application (to remove intermediate chains)

        @return [LPVOID] the pointer to the allocated block or NULL if block cannot be allocated

        \ingroup group_lte_4gmx_heap
    */

    LPVOID  MxHeapAllocInRangeEx (HANDLE h, UINT32 nBegin, UINT32 nEnd, UINT32 nSize, UINT32 nLR);

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
    LPVOID  MxHeapAllocInRange (HANDLE h, UINT32 nBegin, UINT32 nEnd, UINT32 nSize);

#else // NO MX_HEAP_LEAKAGE_CONTROL

    /** @brief The function allocates a block of memory of reqired size and returns a pointer to
       the allocated memory or NULL. The size is alignment and the memory block can be bigger that required.

     @param h [in] - the heap handle
     @param nSize[in] - the size of block in bytes

     @return [LPVOID] a pointer to allocated block or NULL. */
    LPVOID MxHeapAlloc(HANDLE h, UINT32 nSize);

    /** @brief This function is designed to allocate memory at the specified address
                this is needed to allocate buffer at specific range to use different
                controllers and make system faster

        @param h        [in] - the heap handle
        @param nAddr    [in] - the global address that is a part of heap storage
        @param nSize    [in] - the size of block in bytes

        @return [LPVOID] the pointer to the allocated block or NULL if block cannot be allocated*/

    LPVOID  MxHeapAllocAt (HANDLE h, UINT32 nAddr, UINT32 nSize);

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
    LPVOID  MxHeapAllocInRange (HANDLE h, UINT32 nBegin, UINT32 nEnd, UINT32 nSize);

#endif //MX_HEAP_LEAKAGE_CONTROL

/** @brief The function frees a block of heap memory

    @param h        [in] - the heap handler
    @param pBlock   [in] - the pointer to the heap memory block

    @return [MXRC] an error code of operation */

MXRC MxHeapFree(HANDLE h, LPVOID pBlock);

/** @brief The function returns total free size of the heap in bytes

    @NOTE: because of possible fragmentation, the heap may return NULL
           for allocation of block with this size

    @param h [in] - the heap descriptor

    @return [UINT32] the size of heap */

UINT32 MxHeapGetFreeSpace (HANDLE h);

/** @brief The function returns total storage size of the heap in bytes

    @param h [in] - the heap descriptor

    @return [UINT32] the total size of heap in bytes

    \ingroup group_lte_4gmx_heap
*/
UINT32 MxHeapGetTotalSize (HANDLE h);

/** @brief The function returns total free size of the heap in bytes

    @NOTE: because of possible fragmentation, the heap may return NULL
            for allocation of block with this size

    @param h [in] - the heap descriptor

    @return [UINT32] the size of heap */

UINT32 MxHeapGetMaxFreeBlockSize (HANDLE h);

/** @brief This function returns size of allocated block

    @param h [in] the heap handler
    @param pData [in] the pointer to the allacted memory

    @return [UINT32] the size of block */

UINT32 MxHeapGetBlockSize (HANDLE h, LPVOID pData);

#ifdef MX_HEAP_INTEGRITY
    /** @brief This function checks heap integrity
           by analyzing free heap free blocks

    @param h [in] - the heap descriptor
    @param pBlockAddr [out] - the address of error block, it can be null

    @return [MXRC] an error code */

    MXRC MxHeapCheckIntegrity(HANDLE h, LPVOID * pBlockAddr);
#endif

/** @brief This function is designed to check heap block header CRC
       and to return the status of this CRC

@param hHeap [in] - the heap handle 
@param pPtr  [in] - the pointer to the allocated by MxHeapAlloc function block in this heap

@return [UINT32] see MX_HEAP_INTEGRITY_xxx */

UINT32 MxHeapBlockIntegrity (HANDLE h, void * pPtr);

#ifdef MX_HEAP_LEAKAGE_CONTROL

    /** @brief This function returns number of allocated block for this moment

        @return [UINT32] the number of allocated blocks */

    UINT32 MxHeapGetAllocNum (HANDLE h);

    /** @brief This function returns the address of code that allocated some
           block of memory.

        @param h       [in]        - the heap descriptor
        @return [MEM_ALLOC_BLOCK] the allocated block information

        \ingroup group_lte_4gmx_heap
    */
    MEM_ALLOC_BLOCK* MxHeapGetAllocChain (HANDLE h);

    /** @brief This function returns the address of the heap memory returned to application.

        @param h    [in]    - the heap descriptor
        @param p    [in]    - the pointer to the information block 
        
        @return [LPVOID] the pointer to the allocated block

        \ingroup group_lte_4gmx_heap
    */
    LPVOID MxHeapGetAppBlockPtr (HANDLE h, MEM_ALLOC_BLOCK* p);

    /** @brief This function returns the size of allocated by application block.

        @param h    [in]    - the heap descriptor
        @param p    [in]    - the pointer to the allocated information block
        
        @return [UINT32] the size of allocated block

        \ingroup group_lte_4gmx_heap
    */
    UINT32 MxHeapGetAppBlockSize (HANDLE h, MEM_ALLOC_BLOCK* p);

    /** @brief This function returns the size of storage in bytes, needed to store 
               information that represents current heap state

        @param h    [in]    - the heap descriptor
        
        @return [UINT32] the size in bytes

        \ingroup group_lte_4gmx_heap
    */

    UINT32 MxHeapGetPacketSize (HANDLE h);

    /** @brief This function packs heap

        @param h            [in]    - the heap descriptor
        @param nHeapID      [in]    - the heap ID, provided by the application
        @param pPackStor    [out]   - the pointer to the storage 
        @param nStorSize    [in]    - the size of storage in bytes
        
        @return [MXRC] an error code

        \ingroup group_lte_4gmx_heap
    */

    MXRC MxHeapPack (HANDLE h, UINT32 nHeapID, LPVOID pPackStor, UINT32 nStorSize);

    /** @brief This function sets unique ID for the new allocated blocks, 
               these blocks can be easily found and deallocated, in this way we may 
               store heap state 

        @param h        [in] - the heap handle
        @param blockid  [in] - the unique id

        @return [MXRC] an error code*/

    MXRC MxHeapMarkBlocks (HANDLE h, UINT32 blockid);

    /** @brief This function is designed to find the allocated blocks
               marked with unique ID and to free these blocks

        @note Please see MxHeapMarkBlocks function.

        @param h        [in] - the heap handle
        @param blockid  [in] - the unique id

        @return [MXRC] an error code */

    MXRC MxHeapFreeBlocks (HANDLE h, UINT32 blockid);

#endif

/** @brief This function is designed to scan heap free blocks

    @param h        [in]     - the heap handle
    @param bFirst   [in]     - TRUE - to get the first free block, FALSE - to continue scanning
    @param pBlock   [in/out] - in: the address of previous block (if bFirst is FALSE)
                              out: the address of the next block (first block if bFirst is TRUE)

    @return [MXRC] an error code

    \ingroup group_lte_4gmx_heap

    */

MXRC MxHeapScan (HANDLE h, UINT32 bFirst, PMEMBLOCK * pBlock);

#ifdef MX_INTERCPU_ENABLED

/** @brief This function shares a heap
    @param hPart  [in] - the heap handler
    @param pLock  [in] - the address of the sync object (in non cached memory)
    @return [MXRC] error code */

MXRC MxShareHeap (HANDLE INP hHepa, PVUINT32 pLock);

/** @brief This function unshares a heap and this heap cannot be accessed
           from the other 4GMX systems

    @param hHeap [in]  - the heap handler

    @return [MXRC] error code */

MXRC MxUnshareHeap (HANDLE INP hHeap);

#endif /*MX_INTERCPU_ENABLED*/

#ifdef __cplusplus
}
#endif

#endif        /*MX_HEAP_ENABLED */
#endif /*_4GMX_HEAP_H_*/

//-------------------------------------------------------------------------------------------
/** @file 4gmx_part.h
 *
 * @brief Partitions API declaration
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_PART_H_
#define _4GMX_PART_H_

#include "4gmx_config.h"

#ifdef MX_PART_ENABLED

#ifdef __cplusplus
extern "C"
{
#endif

#include "4gmx_types.h"
#include "4gmx_const.h"

#ifdef MX_INTERCPU_ENABLED
#include "4gmx_intercpu.h"
#endif //MX_INTERCPU_ENABLED

/************************************************************************/
/*       The module's error codes                                       */
/************************************************************************/
#define MX_PART_ALLOC_ERROR        DEF_MXRC(MXM_PART,  1)
#define MX_PART_ASSIGN_NAME_ERROR  DEF_MXRC(MXM_PART,  2)
#define MX_PART_CANNOT_BE_SHARED   DEF_MXRC(MXM_PART,  3)

/************************************************************************/
/*      The part control flags                                          */
/************************************************************************/
#define PART_CF_ALLOCATED          (1<<0)
#define PART_CF_NAMED              (1<<1)
#define PART_CF_SYNC               (1<<3)
#define PART_CF_OPEN               (1<<4)
#define PART_CF_HW_SEMA            (1<<5)
#define PART_CF_OPEN_CACHE         (1<<6)

#define MARK_PART_ALLOC(PART)      ((PART)->CtrlFlags |= PART_CF_ALLOCATED)
#define MARK_PART_DEALLOC(PART)    ((PART)->CtrlFlags &= ~PART_CF_ALLOCATED)
#define IS_PART_ALLOC(PART)        ((PART)->CtrlFlags & PART_CF_ALLOCATED)

#define MARK_PART_NAMED(PART)      ((PART)->CtrlFlags |= PART_CF_NAMED)
#define MARK_PART_UNNAMED(PART)    ((PART)->CtrlFlags &= ~PART_CF_NAMED)
#define IS_PART_NAMED(PART)        ((PART)->CtrlFlags & PART_CF_NAMED)

#define MARK_PART_SYNC(PART)       ((PART)->CtrlFlags |= PART_CF_SYNC)
#define MARK_PART_DESYNC(PART)     ((PART)->CtrlFlags &= ~PART_CF_SYNC)
#define IS_PART_SYNC(PART)         ((PART)->CtrlFlags & PART_CF_SYNC)

#define MARK_PART_OPEN(PART)       ((PART)->CtrlFlags |= PART_CF_OPEN)
#define MARK_PART_CLOSED(PART)     ((PART)->CtrlFlags &= ~PART_CF_OPEN)
#define IS_PART_OPENED(PART)       ((PART)->CtrlFlags & PART_CF_OPEN)

#define MARK_PART_HW_SEMA(PART)    ((PART)->CtrlFlags |= PART_CF_HW_SEMA)
#define MARK_PART_NOT_HW_SEMA(PART)((PART)->CtrlFlags &= ~PART_CF_HW_SEMA)
#define IS_PART_HW_SEMA(PART)      ((PART)->CtrlFlags & PART_CF_HW_SEMA)

#define MARK_PART_OPEN_CACHE(PART) ((PART)->CtrlFlags |= PART_CF_OPEN_CACHE)
#define MARK_PART_NOT_OPEN_CACHE(PART)((PART)->CtrlFlags &= ~PART_CF_OPEN_CACHE)
#define IS_PART_OPEN_CACHE(PART)   ((PART)->CtrlFlags & PART_CF_OPEN_CACHE)

/***************************************************************************/

/************************************************************************/
/*           The partition type descriptor definition                   */
/************************************************************************/
typedef struct _MX_PART_
{
    /* The list of general partition's properties */

    LPVOID StorageOrOpenObj;/**< The pointer to the partition storage or to the shared PARTITION descriptor */
    LPVOID FreeBlock;       /**< The pointer to the partition free block */
    UINT32 BlkSize;         /**< The size of block in bytes     */
    UINT32 BlkCount;        /**< The total number of blocks     */
    MXSYNC Sync;            /**< The object of synchronization     */
    UINT32 AllocNum;        /**< The number of allocated blocks    */

    /* The 4GMX additional partition's properties */

    HANDLE hEventAlloc;     /**< The event that is associated with the allocation  */
    HANDLE hEventDeAlloc;   /**< The event that is associated with the deallocation */

    UINT32 CtrlFlags;        /**< The object control flags                                          */

    #ifdef MX_TRACER_ENABLED
    void (*TrAlloc)(void*, void*);  /**< The pointer to the tracer alloc function                          */
    void (*TrFree)(void*, void*);   /**< The pointer to the tracer free function                           */
    void * TrPtr;                   /**< The pointer to some abstract tracer context, it's set by a tracer */
    UINT32 TrLR;                    /**< The caller address                                                */
    UINT32 TrFreeBlk;
    #endif

} PART, *PPART;

typedef struct _MX_ICPART_
{
    LPVOID Storage;         /**< The pointer to the partition storage   */
    LPVOID FreeBlock;       /**< The pointer to the partition free block */
    UINT32 BlkSize;         /**< The size of block in bytes     */
    UINT32 BlkCount;        /**< The total number of blocks     */
    UINT32 Sync;            /**< The object of synchronization     */
    UINT32 AllocNum;        /**< The number of allocated blocks    */

} ICPART, *PICPART;

/** @brief The function creates a partition.

 @param pMem         [in] - the pointer to the memory (part's storage)
 @param nBlkCount    [in] - the number of blocks
 @param nBlkSize     [in] - the size of memory block
 @param name         [in] - the partition name, it can be null
 @param hEventAlloc  [in] - the allocation event handler, it can be HNULL
 @param hEventDealloc[in] - the deallocation event handle, it can be HNULL
 @param oHand        [out] - the handle of created object

 @return [MXRC] error code*/

MXRC MxCreatePart(LPVOID pMem, UINT32 nBlkCount, UINT32 nBlkSize,
                  const char *name, HANDLE hEventAlloc,
                  HANDLE hEventDealloc, HANDLE * oHand);

/** @brief The function initializes the shared partiton for simple inter-cpu communication.

 @param pPartDescr   [in] - the pointer to the inter-cpu partition descriptor
 @param pMem         [in] - the pointer to the partition buffer
 @param nBlkCount    [in] - the total number of partition blocks
 @param nBlkSize     [in] - the size of block in bytes
 @param name         [in] - the name of opened object
 @param hEventAlloc  [in] - the allocation event handler, it can be HNULL (used in case of local system allocation only!)
 @param hEventDealloc[in] - the deallocation event handle, it can be HNULL (used in case of local system allocation only!)
 @param oHand        [out] - the handle of opened partition

 @return [MXRC] error code */

MXRC MxCreateSharedPart(ICPART * pPartDescr, LPVOID pMem, UINT32 nBlkCount,
                     UINT32 nBlkSize, const char *name, HANDLE hEventAlloc,
                     HANDLE hEventDealloc, HANDLE * oHand);

/** @brief The function "opens"/"set connections with" created partition and returns its handle,
   the opened object can be used like a locally created.

 @param pPartDescr [in] - the pointer to the inter-cpu partition descriptor
 @param name   [in] - the name of opened object
 @param oHand  [out] - the handle of opened partition

 @return [MXRC] error code */

MXRC MxOpenPart(ICPART * pPartDescr, const char *name, HANDLE * oHand);

/** @brief The function "opens"/"set connections with" created partition
    and returns its handle, the opened object can be used like a locally created.

    @note: this function is designed to be used with HW implemented semaphores
           and due to this implementation is depended on the platform

 @param pPartDescr  [in] - the pointer to the inter-cpu partition descriptor
 @param name        [in] - the name of opened object
 @param pHwSema     [in] - the pointer to the HW sema cell (it cannot be NULL)
 @param bCacheable  [in] - if TRUE, the opened descriptor is located in cacheable memory
 @param oHand       [out] - the handle of opened partition

 @return [MXRC] error code */

MXRC MxOpenPartEx(ICPART * pPartDescr, const char *name, PUINT32 pHwSema, BOOL bCacheable, HANDLE * oHand);


/** @brief The function removes created partition.
 @param h [in] - the partition handle
 @return [MXRC] error code */

MXRC MxRemovePart(HANDLE hPart);

/** @breif The function allocates a block of memory (of specified size) and returns
           a pointer to the allocated memory or NULL if no any accessible blocks

    @param hPart [in] - the partition handle

    @return [LPVOID] a pointer to allocated partition block or NULL*/

#ifdef MX_TRACER_ENABLED
    LPVOID MxAllocBlock(HANDLE hPart);
    LPVOID MxAllocBlockLR(HANDLE hPart, UINT32 trace_lr);
#else
    LPVOID MxAllocBlock(HANDLE hPart);
#endif

/** @bried The function frees allocated memory block

    @param hPart [in] - the partition handle
    @param pBlock[in] - the pointer to block of memory
    @return [MXRC] error code */

#ifdef MX_TRACER_ENABLED

    MXRC MxFreeBlock(HANDLE hPart, LPVOID pBlock);
    MXRC MxFreeBlockLR(HANDLE hPart, LPVOID pBlock, UINT32 trace_lr);

#else

    MXRC MxFreeBlock(HANDLE hPart, LPVOID pBlock);

#endif

/** @brief The function returns number of non-used partition's blocks,that can be allocated by app
 @param hPart [in] - the partition handle
 @return [UINT32] a number of non-used partition's blocks */

UINT32 MxGetFreePartBlocks(HANDLE hPart);

/** @brief This function converts block index to the block pointer
    @param hPart [in] - the partition descriptor
    @param nIndex[in] - the block index

    @return [LPVOID] the pointer to the block or NULL */

LPVOID MxPartIndexToPointer(HANDLE hPart, UINT32 nIndex);

/** @brief This function converts block pointer to the block index
    @param hPart [in] - the partition descriptor
    @param pData [in] - the block pointer

    @return [LPVOID] the index of the block or 0xFFFFFFFF */

UINT32 MxPartPointerToIndex(HANDLE hPart, LPVOID pData);

/** @brief The function returns size of partition block in bytes
    @param hPart [in] - the partition handler
    @return [UINT32] the size of block in bytes */

UINT32 MxGetPartBlockSize (HANDLE hPart);

/** @brief The function returns total number of partition blocks
    @param hPart [in] - the partition handler
    @return [UINT32] the number of blocks*/

UINT32 MxGetPartBlockCount (HANDLE hPart);

/** @brief This function checks partition range of memory 
           and returns TRUE if specified data belongs to this range of memory 

    @param hPart [in] - the partition handle 
    @param pData [in] - the pointer to the data 

    @return [UINT32] flag 1 or 0 */

UINT32 MxIsPartMemory (HANDLE hPart, LPVOID pData);

/** @brief This function check if the specified block already allocated or not,
           the pointer should points to the partition storage

    @param hPart        [in] - the partition handle 
    @param pTestBlock   [in] - the pointer to the partition block

    @return [BOOL] 1 / 0*/

UINT32 MxIsBlockAllocated (HANDLE hPart, LPVOID pTestBlock);

#ifdef MX_INTERCPU_ENABLED

/** @brief This function shares a partition
    @param hPart  [in] - the partition handler
    @param pLock  [in] - the address of the sync object (in non cached memory)
    @return [MXRC] error code */

MXRC MxSharePart (HANDLE INP hPart, PVUINT32 pLock);

/** @brief This function unshares a partiton and this partition cannot be accessed
           from the other 4GMX systems

    @param hPart [in]  - the partition handler

    @return [MXRC] error code */

MXRC MxUnsharePart (HANDLE INP hPart);

#endif /*MX_INTERCPU_ENABLED*/

#ifdef __cplusplus
}
#endif

#endif        /*MX_PART_ENABLED */
#endif /*_4GMX_PART_H_*/

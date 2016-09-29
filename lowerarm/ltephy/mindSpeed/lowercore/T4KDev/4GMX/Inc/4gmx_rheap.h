//-------------------------------------------------------------------------------------------
/** @file 4gmx_rheap.h
 *
 * @brief Rapid Heaps API declaration
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------


#ifndef _4GMX_RHEAP_H_
#define _4GMX_RHEAP_H_

#include "4gmx_config.h"

#ifdef MX_RHEAP_ENABLED

#ifdef __cplusplus
extern "C"
{
#endif

#include "4gmx_types.h"
#include "4gmx_const.h"

/************************************************************************/
/*       The list of module error codes                                 */
/************************************************************************/
#define MX_RHEAP_ALLOC_ERROR          DEF_MXRC(MXM_RHEAP,  1)
#define MX_RHEAP_ASSIGN_NAME_ERROR    DEF_MXRC(MXM_RHEAP,  2)

/************************************************************************/
/*      The heap control flags                                          */
/************************************************************************/
#define RHEAP_CF_ALLOCATED           (1<<0)
#define RHEAP_CF_NAMED               (1<<1)
#define RHEAP_CF_SYNC                (1<<2)
#define RHEAP_CF_OPEN                (1<<3)

#define MARK_RHEAP_ALLOC(HEAP)       ((HEAP)->CtrlFlags |= RHEAP_CF_ALLOCATED)
#define MARK_RHEAP_DEALLOC(HEAP)     ((HEAP)->CtrlFlags &= ~RHEAP_CF_ALLOCATED)
#define IS_RHEAP_ALLOC(HEAP)         ((HEAP)->CtrlFlags & RHEAP_CF_ALLOCATED)

#define MARK_RHEAP_NAMED(HEAP)       ((HEAP)->CtrlFlags |= RHEAP_CF_NAMED)
#define MARK_RHEAP_UNNAMED(HEAP)     ((HEAP)->CtrlFlags &= ~RHEAP_CF_NAMED)
#define IS_RHEAP_NAMED(HEAP)         ((HEAP)->CtrlFlags & RHEAP_CF_NAMED)

#define MARK_RHEAP_SYNC(HEAP)        ((HEAP)->CtrlFlags |= RHEAP_CF_SYNC)
#define MARK_RHEAP_UNSYNC(HEAP)      ((HEAP)->CtrlFlags &= ~RHEAP_CF_SYNC)
#define IS_RHEAP_SYNC(HEAP)          ((HEAP)->CtrlFlags & RHEAP_CF_SYNC)

#define MARK_RHEAP_OPENED(HEAP)      ((HEAP)->CtrlFlags |= RHEAP_CF_OPEN)
#define MARK_RHEAP_UNOPENED(HEAP)    ((HEAP)->CtrlFlags &= ~RHEAP_CF_OPEN)
#define IS_RHEAP_OPENED(HEAP)        ((HEAP)->CtrlFlags & RHEAP_CF_OPEN)

/***************************************************************************/

/************************************************************************/
/*           The heap type descriptors definition                       */
/************************************************************************/

typedef struct _MX_RHEAP_
{
    LPVOID HeapStorage;   /**< The pointer to the heap storage          */
    LPVOID FreeBlock;     /**< The pointer to the first free memory block of heap (it can be null) */
    UINT32 FreeSize;      /**< The total size of free space in bytes         */
    UINT32 Align;         /**< The data alignment in bytes           */
    MXSYNC Sync;          /**< The synchronization object, used for shared objects  */
    UINT32 TotalSize;     /**< The real size of the heap memory                     */

    UINT32 CtrlFlags;     /**< The object control flags      */

} RHEAP, *PRHEAP;

typedef struct _MX_ICRHEAP_
{
    LPVOID HeapStorage;   /**< The pointer to the heap storage                      */
    LPVOID FreeBlock;     /**< The pointer to the first free memory block of heap (it can be null) */
    UINT32 FreeSize;      /**< The total size of free space in bytes                */
    UINT32 Align;         /**< The data alignment in bytes                          */
    UINT32 Sync;          /**< The synchronization object, used for shared objects  */
    UINT32 TotalSize;     /**< The real size of the heap memory                     */

}ICRHEAP, *PICRHEAP;

/** @brief The function creates a rapid-heap.

 @param pMem   [in] - the pointer to memory (heap storage)
 @param nSize  [in] - the storage size in bytes
 @param nAlign  [in] - the alignment in bytes
 @param oHand  [out] - the handle of created object

 @return [MXRC] error code
*/
MXRC MxCreateRHeap(LPVOID pMem, UINT32 nSize, UINT32 nAlign,
                  const char *name, HANDLE * oHand);

/** @brief The function creates a shared rapid heap. The rapid heap descriptor is not located
           in the 4GMX context, this descriptor can be used to open
           a heap by other 4GMX system(s)

 @param pHeapDescr  [in] - the pointer to the heap descriptor that will be initialized by this function
 @param pMem        [in] - the pointer to memory (heap storage)
 @param nSize       [in] - the storage size in bytes
 @param nAlign      [in] - the alignment in bytes
 @param oHand       [out] - the handle of created object

 @return [MXRC] error code
*/
MXRC MxCreateSharedRHeap(ICRHEAP * pHeapDescr, LPVOID pMem, UINT32 nSize, UINT32 nAlign,
                  const char *name, HANDLE * oHand);


/** @brief The function "opens"/"set connections with" a created rapid heap and returns its handle,
   the opened object can be used like a locally created.

 @param pHeapDescr  [in] - the pointer to the inter-cpu heap descriptor
 @param name        [in] - the associated name of opened object
 @param oHand       [out]- the handle of opened partition

 @return [MXRC] error code */

MXRC MxOpenRHeap(ICRHEAP * pHeapDescr, const char *name, HANDLE * oHand);

/** @brief The function removes created heap.
 @param h [in] - the heap handle
 @return [MXRC] error code */

MXRC MxRemoveRHeap(HANDLE h);

/** @brief The function allocates a reqired size block memory and returns a pointer to
   the allocated memory or NULL. The size is alignment and the memory block can be bigger that required.

 @param h [in] - the heap handle
 @param nSize[in] - the size of block in bytes

 @return [LPVOID] a pointer to allocated block or NULL. */

LPVOID MxRHeapAlloc(HANDLE h, UINT32 nSize);

/** @brief The function reinits rapid heap and frees all allocated memory

 @param h [in] - the heap handle
 @param nSize[in] - the size of block in bytes

 @return [LPVOID] a pointer to allocated block or NULL. */

MXRC MxRHeapFree(HANDLE h);

/** @brief The function returns total free size of the heap in bytes

    @NOTE: because of possible fragmentation, the heap may return NULL
           for allocation of block with this size

    @param h [in] - the heap descriptor

    @return [UINT32] the size of heap */

UINT32 MxRHeapGetFreeSpace (HANDLE h);

#ifdef __cplusplus
}
#endif

#endif        /*MX_RHEAP_ENABLED */
#endif /*_4GMX_RHEAP_H_*/

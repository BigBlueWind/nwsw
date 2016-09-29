//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL heaps.h
 *
 * @brief The handles of system heaps
 * @author Mindspeed Technologies
 * @version $Revision: 1.14 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _HEAPS_H_
#define _HEAPS_H_

#include "4gmx.h"

#define ID_HEAP_APP         1
#define ID_HEAP_DSP         2
#define ID_HEAP_NCNB        3
#define ID_HEAP_IRAM        4
#define ID_HEAP_DDR1        5
#define ID_HEAP_DDR1_NCNB   6
#define ID_HEAP_PRV_CORE0   7

typedef struct _HEAP_INFO_
{
    BOOL    bAllowed;
    BOOL    bInited;
    UINT32  BaseAddr;
    UINT32  Size;

}HEAPINFO;


extern HANDLE hAppHeap [];
extern HANDLE hDspHeap [];
extern HANDLE hPhyHeap [];
extern HANDLE hNcNbHeap [];
extern HANDLE hIramHeap [];
extern HANDLE hDDR1Heap [];
extern HANDLE hDDR1NcnbHeap [];
extern HANDLE hPrvHeap [];

#define AppHeap (hAppHeap[MxGetCpuID()])
#define DspHeap (hDspHeap[MxGetCpuID()])
#define PhyHeap (hPhyHeap[MxGetCpuID()])
#define NcnbHeap (hNcNbHeap[MxGetCpuID()])
#define IramHeap (hIramHeap[MxGetCpuID()])
#define DDR1Heap (hDDR1Heap[MxGetCpuID()])
#define DDR1NcnbHeap (hDDR1NcnbHeap[MxGetCpuID()])
#define PRV_CORE_HEAP (hPrvHeap[MxGetCpuID()])

/** @brief This function returns the base address of DDR0 memory

    @return [UINT32] the base address of DDR-0 */
    
UINT32 GetDDR0Base (void);

/** @brief This function returns the size of DDR0 memory in bytes

    @return [UINT32] the size of DDR-0 storage in bytes*/

UINT32 GetDDR0Size (void);

/** @brief This function returns the base address of DDR1 memory

    @return [UINT32] the base address of DDR-1 */
    
UINT32 GetDDR1Base (void);

/** @brief This function returns the size of DDR1 memory in bytes

    @return [UINT32] the size of DDR-1 storage in bytes*/

UINT32 GetDDR1Size (void);

/** @brief This function initializes system heaps that are used by the application

    @param nCpuID [in] - the ID of current CPU

    @return [MXRC] error code of operation */

MXRC CreateSystemHeaps (UINT32 nCpuID);

MXRC SystemHeapsInit(void);

/** @brief This function is designed to create PHY rapid heap
 * 			located in DspHeap and have be called after all drivers / modules initialiozation
 *
 *  @param nCpuID [in] - the CPU id
 *
 */
MXRC CreatePHYHeap (UINT32 nCpuID);

/** @brief This function returns information about heap

    @NOTE  This function cannot be called before CreateSystemHeaps()
    
    @param nHeapID[in] - the heap ID, see: ID_HEAP_xxx
    @param pInfo [out] - the heap information

    @return [MXRC] an error code */
    
MXRC GetHeapInfo (UINT32 nHeapID, HEAPINFO * pInfo);

/** @brief This function returns the configuration size of DDR1 Heap
           it can be called any time and does not require some additional 
           initialization

    @param pBase [in] - the base address of the heap, it can be NULL

    @return [UINT32] the size of heap in bytes */

UINT32 GetDDR1HeapBaseSize(UINT32 * pBase);

/** @brief This function returns the configuration size of DDR1 NCNB Heap
           it can be called at any time and does not require some additional 
           initialization. Also this function checks location of I-CPU 
           provided by Linux and takes it into account 

    @param pBase [in] - the base address of the heap, it can be NULL

    @return [UINT32] the size of heap in bytes */

UINT32 GetDDR1NcNbHeapBaseSize(UINT32 * pBase);

/** @brief This function returns the configuration size of DDR1 NCNB Heap
           it can be called at any time and does not require some additional 
           initialization. Also this function checks location of I-CPU 
           provided by Linux and takes it into account 

    @return [UINT32] the size of heap in bytes */

UINT32 GetDDR1NcNbHeapSize(void);

/** @brief This function returns HANDLE of a heap which has passed address
 * 
 *  @param pPtr [in] - current address
 * 
 *  @return [HANDLE] - a handle of a heap 
 */
HANDLE GetHeapHandle(LPVOID pPtr);

/** @brief This function releases a dynamically allocated memory using only the pointer
 * 
 *  @param pPtr [in] - current address
 * 
 *  @return [MXRC] - a code of error
 */
MXRC HeapFreeByPtr(LPVOID pPtr);

#endif /*_HEAPS_H_*/

#ifdef __cplusplus
}
#endif


//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL heaps.c
 *
 * @brief The handles of system heaps
 * @author Mindspeed Technologies
 * @version $Revision: 1.29 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include "4gmx.h"
#include "config.h"
#include "heaps.h"
#include "DSPdrv.h"
#include "sysddrctrl.h"
#include "appprintf.h"
#include "devinfo.h"
#include "lcorecfg.h"
#include "initcpu.h"

extern char appheapbuf [];
extern char ncnbheapbuf[];
extern char core_0_heap_buf [];

ICHEAP AppHeapDescr;     // The global descriptor of appliation heap
ICHEAP DspHeapDescr;     // The global descriptor of DSP heap
ICRHEAP PhyHeapDescr;    // The global descriptor of PHY heap
ICHEAP NcNbHeapDescr;    // The global descriptor of NonCachable-NonBuffered heap
ICHEAP IramHeapDescr;    // The global descriptor of NonCachable-NonBuffered heap
ICHEAP DDR1HeapDescr;    // The global descriptor of Cachable-Buffered heap mapped to DDR1 controller
ICHEAP DDR1NcnbHeapDescr;// The global descriptor of NonCachable-NonBuffered heap mapped to DDR1 controller

HANDLE hAppHeap [CPU_NUM];
HANDLE hDspHeap [CPU_NUM];
HANDLE hPhyHeap [CPU_NUM];
HANDLE hNcNbHeap[CPU_NUM];
HANDLE hIramHeap[CPU_NUM];
HANDLE hDDR1Heap[CPU_NUM];
HANDLE hDDR1NcnbHeap[CPU_NUM];
HANDLE hPrvHeap[CPU_NUM];

//#define CORE0_HEAP_SECTION

static MXRC CreatePrvHeap(void)
{
    MXRC rc;
    SECTINFO info;

    #ifdef CORE0_HEAP_SECTION
        rc = SysCoreGetSectInfo(SECTION_PRV_CORE0, &info);

        if(FAILED(rc))
        {
            uart_printf ("PRV_CORE0 section error, rc=0x%x\r\n", rc);
            return rc;
        }
    #else
        info.Base = (UINT32)core_0_heap_buf;
        info.Size = CORE0_HEAP_SIZE;
    #endif

    rc = MxCreateHeap((LPVOID)info.Base, info.Size, 32, NULL, &PRV_CORE_HEAP);

    if (FAILED(rc))
    {
        uart_printf ("PRIVATE heap create error, rc=0x%x\r\n", rc);
        return rc;
    }
    
    return rc;
}

UINT32 HeapIsPrvCoreHeap(UINT32 p)
{
    SECTINFO info;

    #ifdef CORE0_HEAP_SECTION
    MXRC rc = SysCoreGetSectInfo(SECTION_PRV_CORE0, &info);

    if(FAILED(rc))
        return 0;

    #else

    info.Base = (UINT32)core_0_heap_buf;
    info.Size = CORE0_HEAP_SIZE;
    
    #endif

    return (p >= info.Base) && (p < (info.Base+info.Size));
}

static MXRC CreateAppHeap(void)
{
    MXRC rc = MX_OK;
    UINT32 AppHeapSize = DevInfoGetSize(MEMID_DDR0);

    if(AppHeapSize > 1)
    {
        // We need to have 32 bytes alignment for the cached data due to the size of cache line
        if (FAILED(rc = MxCreateSharedHeap(&AppHeapDescr, appheapbuf, AppHeapSize, 32, NULL, &(AppHeap))))
            return rc;
    }

    #ifdef HEAPS_PRINT_INFO_ENABLED
    uart_printf("AppHeap (0x%X) size is %d bytes\n", appheapbuf, AppHeapSize);
    #endif

    return rc;
}

static MXRC CreateDspHeap(void)
{
    MXRC rc = MX_OK;
    UINT32 DspHeapAddr = DspDrvGetDspHeapAddr();
    UINT32 DspHeapSize = DspDrvGetDspHeapSize(DspHeapAddr);

    if(DspHeapSize > 1)
    {
        if (FAILED(rc = MxCreateSharedHeap(&DspHeapDescr, (LPVOID)DspHeapAddr, DspHeapSize, 32, NULL, &(DspHeap))))
            return rc;
    }

    #ifdef HEAPS_PRINT_INFO_ENABLED
    uart_printf("DspHeap (0x%X) size is %d bytes\n", DspHeapAddr, DspHeapSize);
    #endif

    return rc;
}

static MXRC CreateNcNbHeap(void)
{
    MXRC rc = MX_OK;
    UINT32 NcNbHeapSize = DevInfoGetSize(MEMID_DDR0_NCNB);

    if(NcNbHeapSize > 1)
    {
        if (FAILED(rc = MxCreateSharedHeap(&NcNbHeapDescr, (LPVOID)ncnbheapbuf, NcNbHeapSize, 32, NULL, &(NcnbHeap))))
            return rc;
    }

    #ifdef HEAPS_PRINT_INFO_ENABLED
    uart_printf("NcNbHeap (0x%X) size is %d bytes\n", ncnbheapbuf, NcNbHeapSize);
    #endif

    return rc;
}

static MXRC CreateIramHeap(void)
{
    MXRC rc = MX_OK;

#ifdef IRAM_HEAP_INIT
    UINT32 IramHeapSize = DevInfoGetSize(MEMID_IRAM);

    if(IramHeapSize > 1)
    {
        if (FAILED(rc = MxCreateSharedHeap(&IramHeapDescr, (LPVOID)IRAM_HEAP_ADDR, IramHeapSize, 32, NULL, &(IramHeap))))
            return rc;
    }

    #ifdef HEAPS_PRINT_INFO_ENABLED
    uart_printf("IramHeap (0x%X) size is %d bytes\n", IRAM_HEAP_ADDR, IramHeapSize);
    #endif

#endif

    return rc;
}

BOOL IsHwDDR1Enabled (void)
{
#ifdef NO_USE_DDR1

    // In this case virtual DDR-1 is used 
    // virtual DDR-1 is mapped to DDR-0 just to prevent 
    // application changes 
    
    return TRUE;

#else
    // if DDR-1 controller under reset

    if (REG32(SYS_DDR_RESET) & 0x10)
        return FALSE;

    return TRUE;
#endif    
}

/** @brief This function returns the base address of DDR0 memory

    @return [UINT32] the base address of DDR-0 */
    
UINT32 GetDDR0Base (void)
{
    return (REG32(CFG_SYS_DDR_LL0) << 20);
}

/** @brief This function returns the size of DDR0 memory in bytes

    @return [UINT32] the size of DDR-0 storage in bytes*/

UINT32 GetDDR0Size (void)
{
    return (REG32(CFG_SYS_DDR_HL0) << 20) - (REG32(CFG_SYS_DDR_LL0) << 20);
}

/** @brief This function returns the base address of DDR1 memory

    @return [UINT32] the base address of DDR-1 */
    
UINT32 GetDDR1Base (void)
{
    extern UINT8 Image$$EMUL_DDR1_SPACE$$Base [];
    UINT32 base;
    
#ifdef NO_USE_DDR1

    // The compilation options prevent usage of DDR-1 memory
    // in this case we use virtual DDR-1 (that is mapped to DDR-0)
    // to prevent changes in application code 
    // the base address of virtual DDR-1 is taken by using scatter section
    // the size by using DDR-0 controller 

    base = (UINT32)Image$$EMUL_DDR1_SPACE$$Base;
    return base;
    
#else

    extern UINT8 Image$$ROOT_RO$$Base [];

    base = (UINT32)Image$$ROOT_RO$$Base;

    if (!IsHwDDR1Enabled ())
        return 0;

    // if code was started from DDR-1 memory 
    if (base >= (REG32(CFG_SYS_DDR_LL1) << 20))
    {
        // the code was started from DDR-1 (DDR-0:512MB / DDR-1:512MB)
        // and the code was build with 512M scatter 
        // in this case we need to prevent overlap (ddr-1:code & ddr-1:data)
    
        return (UINT32)Image$$EMUL_DDR1_SPACE$$Base;
    }
    
    return (REG32(CFG_SYS_DDR_LL1) << 20);

#endif 
    
}

/** @brief This function returns the size of DDR1 memory in bytes

    @return [UINT32] the size of DDR-1 storage in bytes*/

UINT32 GetDDR1Size (void)
{
    UINT32 nSize; 

#ifdef NO_USE_DDR1

    // The compilation options prevent usage of DDR-1 memory
    // in this case we use virtual DDR-1 (that is mapped to DDR-0)
    // to prevent changes in application code 
    // the base address of virtual DDR-1 is taken by using scatter section
    // the size by using DDR-0 controller 

    extern UINT8 Image$$EMUL_DDR1_SPACE$$Base [];
    UINT32 base;

    base = (UINT32)Image$$EMUL_DDR1_SPACE$$Base;

    // this code is added here to process situation when 
    // code is compiled with NO_DDR1 option 
    // but for some reason it's started from DDR1 memory
    
    if (REG32(CFG_SYS_DDR_HL0) << 20) >= base)
    {
        nSize = (REG32(CFG_SYS_DDR_HL0) << 20) - base;
    }
    else
    {
        nSize = (REG32(CFG_SYS_DDR_HL1) << 20) - base;
    }

    return nSize;

#else
    
    if (!IsHwDDR1Enabled ())
        return 0;

    nSize = (REG32(CFG_SYS_DDR_HL1) << 20) - GetDDR1Base ();

    return nSize;

#endif 
    
}


/** @brief This function returns the configuration size of DDR1 Heap
           it can be called any time and does not require some additional 
           initialization

    @param pBase [in] - the base address of the heap, it can be NULL

    @return [UINT32] the size of heap in bytes */

UINT32 GetDDR1HeapBaseSize(UINT32 * pBase)
{
    if (pBase != NULL)
        *pBase = GetDDR1Base ();
    
    return MIN (GetDDR1Size(), DevInfoGetSize(MEMID_DDR1));
}

/** @brief This function returns the configuration size of DDR1 NCNB Heap
           it can be called at any time and does not require some additional 
           initialization. Also this function checks location of I-CPU 
           provided by Linux and takes it into account 

    @param pBase [in] - the base address of the heap, it can be NULL

    @return [UINT32] the size of heap in bytes */

UINT32 GetDDR1NcNbHeapBaseSize(UINT32 * pBase)
{
    if (!IsHwDDR1Enabled ())
        return 0;

    if (pBase != NULL)
    {
        *pBase = GetDDR1Base () + GetDDR1HeapBaseSize(NULL);
    }

    return GetDDR1NcNbHeapSize();
}

/** @brief This function returns the configuration size of DDR1 NCNB Heap
           it can be called at any time and does not require some additional 
           initialization. Also this function checks location of I-CPU 
           provided by Linux and takes it into account 

    @return [UINT32] the size of heap in bytes */

UINT32 GetDDR1NcNbHeapSize(void)
{
    UINT32 nSize, nAddr; 

    if (!IsHwDDR1Enabled ())
        return 0;

    nSize = GetDDR1Size ();

    if (nSize != 0) 
    {
        // the logics is to subtract the size of DDR-1 cacheable memory 
        // but with some protection , DDR-1 cacheable size should be less than DDR-1 total size
        // to prevent overflow case
        
        nSize -= MIN(DevInfoGetSize(MEMID_DDR1), nSize);
    }

    
    if (nSize == 0)
    {   
        // if something wrong with DDR-1 configuration
        // just return the size 0,
        // it means we do not have DDR-1 ncnb!
        
        return 0;
    }

    nAddr = GetDDR1Base() + GetDDR1HeapBaseSize(NULL);

    // We need to check if I-CPU is mapped by Linux to DDR-1 
    // and to prevent the possible problem of data overwriting
    // we also assume that I-CPU storage can be located 
    // at the end of the DDR-1 range where DDR1-NCNB is located

    if (!IS_HBI_MODE())
    {
        if (CoreCfg.ICPUBase >= nAddr && CoreCfg.ICPUBase <= (nAddr + nSize))
        {
            nSize = CoreCfg.ICPUBase - nAddr;
        }
    }

    return nSize;
}

MXRC CreateDDR1Heaps (void)
{
    UINT32 nAddr;
    UINT32 nSize, nSizeNcnb, nSizeCache;
    MXRC rc = MX_OK;

#ifndef NO_USE_DDR1

    PDENALIDDRC pDDR = (PDENALIDDRC)DDR1_CONTROLLER_BASE;

    if (!IsHwDDR1Enabled ())
        return MX_OK;

    if (pDDR->pll_reset_n == 0)
    {
        uart_printf ("DDR-1 is not initialized\r\n");
        return MX_OK;
    }

#endif

    nAddr = GetDDR1Base ();
    nSize = GetDDR1Size ();

    if (nSize != 0)
    {
        nSizeCache = DevInfoGetSize(MEMID_DDR1);
        nSizeNcnb = DevInfoGetSize(MEMID_DDR1_NCNB);
    }
    else
    {
        return MX_OK;
    }

    // We need to check if I-CPU is mapped by Linux to DDR-1 
    // and to prevent the possible problem of data overwriting
    // we also assume that I-CPU storage can be located 
    // at the end of the DDR-1 range where DDR1-NCNB is located

    if (!IS_HBI_MODE())
    {
        if (CoreCfg.ICPUBase >= nAddr && CoreCfg.ICPUBase <= (nAddr + nSize))
        {
            UINT32 dS = (nAddr + nSize) - CoreCfg.ICPUBase;
            nSizeNcnb -= dS;

            uart_printf ("\r\nI-CPU storage [0x%08x] is mapped to DDR-1 [0x%x...0x%x]\r\n", 
                        CoreCfg.ICPUBase, 
                        nAddr, 
                        (nAddr + nSize)
                    );
            uart_printf ("DDR-1:(NCNB heap) size is reduced on %dMB and equal to %dMB\r\n", 
                        dS / (1024*1024),
                        nSizeNcnb / (1024*1024)
                    );
        }
    }

    if (nSizeCache != 0)
    {
        #ifdef HEAPS_PRINT_INFO_ENABLED
        uart_printf ("DDR-1 cacheable heap (0x%X) size is %dMBytes\r\n", nAddr, nSizeCache / (1024 * 1024));
        #endif
        if (FAILED(rc = MxCreateSharedHeap(&DDR1HeapDescr, (LPVOID)nAddr, nSizeCache, 32, NULL, &(DDR1Heap))))
            return rc;
    }

    if (nSizeNcnb != 0)
    {
        #ifdef HEAPS_PRINT_INFO_ENABLED
        uart_printf ("DDR-1 ncnb heap (0x%X) size is %dMBytes\r\n", nAddr + nSizeCache, nSizeNcnb / (1024 * 1024));
        #endif
        if (FAILED(rc = MxCreateSharedHeap(&DDR1NcnbHeapDescr, (LPVOID)(nAddr + nSizeCache), nSizeNcnb, 32, NULL, &(DDR1NcnbHeap))))
            return rc;
    }

    return MX_OK;
}



/** @brief This function initializes system heaps that are used by the application

    @param nCpuID [in] - the ID of current CPU

    @return [MXRC] error code of operation */

MXRC CreateSystemHeaps (UINT32 nCpuID)
{
    MXRC rc;

    if (nCpuID == 0)
    {
       CreatePrvHeap();
       CreateAppHeap();
       CreateDspHeap();
       CreateNcNbHeap();
       CreateIramHeap();

        if (FAILED(rc = CreateDDR1Heaps()))
        {
            uart_printf ("Failed to create DDR-1 heap, rc=%x\r\n", rc);
            return rc;
        }
    }
    else
    {
        if (FAILED(rc = MxOpenHeap(&AppHeapDescr, NULL, &(AppHeap))))
            return rc;

        if (FAILED(rc = MxOpenHeap(&DspHeapDescr, NULL, &(DspHeap))))
            return rc;

        if (FAILED(rc = MxOpenHeap(&NcNbHeapDescr, NULL, &(NcnbHeap))))
            return rc;

#ifdef IRAM_HEAP_INIT
        if (FAILED(rc = MxOpenHeap(&IramHeapDescr, NULL, &(IramHeap))))
            return rc;
#endif

        if (HANDLE_NOT_NULL(hDDR1Heap[0]))
        {
            if (FAILED(rc = MxOpenHeap(&DDR1HeapDescr, NULL, &(DDR1Heap))))
                return rc;
        }

        if (HANDLE_NOT_NULL(hDDR1NcnbHeap[0]))
        {
            if (FAILED(rc = MxOpenHeap(&DDR1NcnbHeapDescr, NULL, &(DDR1NcnbHeap))))
                return rc;
        }
    }

    return MX_OK;
}

MXRC SystemHeapsInit(void)
{
    return CreateSystemHeaps(MxGetCpuID());
}

/** @brief This function is designed to create PHY rapid heap
 * 			located in DspHeap and have be called after all drivers / modules initialiozation
 * 			by PHY code
 *  @param nCpuID [in] - the CPU id
 *
 */
MXRC CreatePHYHeap (UINT32 nCpuID)
{
	MXRC rc = MX_OK;

	if (nCpuID == 0)
    {
		LPVOID * pMem = NULL;

		UINT32 nSize;

		printf ("Init PHY R-HEAP ...\r\n");

		nSize = MxHeapGetMaxFreeBlockSize (DspHeap);

		// header size alignment
		nSize -= 32;

		_ASSERT (nSize != 0);

		pMem = MxHeapAlloc (DspHeap, nSize);

		_ASSERT_PTR (pMem);

		printf ("PHY R-HEAP SIZE about %d KB\r\n", nSize / 1024);

        if (FAILED(rc = MxCreateSharedRHeap(&PhyHeapDescr, (LPVOID)pMem, nSize, 32, NULL, &(PhyHeap))))
            return rc;
    }
    else
    {
        if (FAILED(rc = MxOpenRHeap(&PhyHeapDescr, NULL, &(PhyHeap))))
            return rc;
    }

	return MX_OK;
}

/** @brief This function returns information about heap

    @NOTE  This function cannot be called before CreateSystemHeaps()
    
    @param nHeapID[in] - the heap ID, see: ID_HEAP_xxx
    @param pInfo [out] - the heap information

    @return [MXRC] an error code */

MXRC GetHeapInfo (UINT32 nHeapID, HEAPINFO * pInfo)
{
    _ASSERT_PTR (pInfo);

    if (pInfo == NULL)
        return MX_OK;

    memset (pInfo, 0, sizeof (*pInfo));

    switch (nHeapID)
    {
        case ID_HEAP_APP:
            pInfo->bAllowed = 1;
            pInfo->bInited  = HANDLE_NOT_NULL(AppHeap);
            pInfo->BaseAddr = (UINT32)appheapbuf;
            //pInfo->Size     = DevInfoGetSize(MEMID_DDR0);
            
            if (pInfo->bInited)
                pInfo->Size = MxHeapGetTotalSize (AppHeap);
            
            break;

        case ID_HEAP_DSP:
            pInfo->bAllowed = 1;
            pInfo->bInited  = HANDLE_NOT_NULL(DspHeap);
            pInfo->BaseAddr = (UINT32)DspDrvGetDspHeapAddr();
            //pInfo->Size     = DspDrvGetDspHeapSize(pInfo->BaseAddr);

            if (pInfo->bInited)
                pInfo->Size = MxHeapGetTotalSize (DspHeap);
            
            break;

        case ID_HEAP_IRAM:
            pInfo->bAllowed = 1;
            pInfo->bInited  = HANDLE_NOT_NULL(IramHeap);
            pInfo->BaseAddr = IRAM_HEAP_ADDR;//(UINT32)DspDrvGetDspHeapAddr();
            //pInfo->Size     = DspDrvGetDspHeapSize(pInfo->BaseAddr);

            if (pInfo->bInited)
                pInfo->Size = MxHeapGetTotalSize (IramHeap);
            
            break;

        case ID_HEAP_NCNB:
            pInfo->bAllowed = 1;
            pInfo->bInited  = HANDLE_NOT_NULL(NcnbHeap);
            pInfo->BaseAddr = (UINT32)ncnbheapbuf;
            //pInfo->Size     = DevInfoGetSize(MEMID_DDR0_NCNB);

            if (pInfo->bInited)
                pInfo->Size = MxHeapGetTotalSize (NcnbHeap);
            
            break;

        case ID_HEAP_DDR1:

            if (IsHwDDR1Enabled ())
            {
                pInfo->bAllowed = 1;
                pInfo->bInited  = HANDLE_NOT_NULL(DDR1Heap);
                
                if (pInfo->bInited)
                    pInfo->Size = GetDDR1HeapBaseSize(&pInfo->BaseAddr);
            }
            
            break;

        case ID_HEAP_DDR1_NCNB:

            if (IsHwDDR1Enabled ())
            {
                pInfo->bAllowed = 1;
                pInfo->bInited  = HANDLE_NOT_NULL(DDR1NcnbHeap);

                if (pInfo->bInited)
                    pInfo->Size = GetDDR1NcNbHeapBaseSize(&pInfo->BaseAddr);
            }
            
            break;

        case ID_HEAP_PRV_CORE0:
        {
             #ifdef CORE0_HEAP_SECTION
                SECTINFO info;
                if (!FAILED(SysCoreGetSectInfo(SECTION_PRV_CORE0, &info)))
                {
                    pInfo->bAllowed = 1;
                    pInfo->bInited  = HANDLE_NOT_NULL(PRV_CORE_HEAP);
                    pInfo->BaseAddr = (UINT32)info.Base;
                    pInfo->Size     = (UINT32)info.Size;
                }

            #else

                pInfo->bAllowed = 1;
                pInfo->bInited  = HANDLE_NOT_NULL(PRV_CORE_HEAP);
                pInfo->BaseAddr = (UINT32)core_0_heap_buf;
                pInfo->Size     = (UINT32)CORE0_HEAP_SIZE;

            #endif
            
            break;
        }

        default:
            break;
    }

    return MX_OK;
}

/** @brief This function returns HANDLE of a heap which has passed address
 * 
 *  @param pPtr [in] - current address
 * 
 *  @return [HANDLE] - a handle of a heap 
 */
HANDLE GetHeapHandle(LPVOID pPtr)
{
	PMEMBLOCK pBlock;
	HANDLE hStorage = {0};
		
	_ASSERT(pPtr != NULL);

	// looking for corresponding heap
	
	// AppHeap
	pBlock = (PMEMBLOCK)((UINT8 *)pPtr - ROUND(ALLOC_HEADER_SIZE, AppHeapDescr.Align));
	// checking the passed address in current heap boundaries
	if (((UINT32)pBlock >= (UINT32)AppHeapDescr.HeapStorage) && ((UINT32)pBlock < ((UINT32)AppHeapDescr.HeapStorage + (UINT32)AppHeapDescr.TotalSize)))
	{
		return hAppHeap[MxGetCpuID()];
	}
	
	// DspHeap
	pBlock = (PMEMBLOCK)((UINT8 *)pPtr - ROUND(ALLOC_HEADER_SIZE, DspHeapDescr.Align));
	// checking the passed address in current heap boundaries
	if (((UINT32)pBlock >= (UINT32)DspHeapDescr.HeapStorage) && ((UINT32)pBlock < ((UINT32)DspHeapDescr.HeapStorage + (UINT32)DspHeapDescr.TotalSize)))
	{
		return hDspHeap[MxGetCpuID()];
	}
	
	// NcnbHeap
	pBlock = (PMEMBLOCK)((UINT8 *)pPtr - ROUND(ALLOC_HEADER_SIZE, NcNbHeapDescr.Align));
	// checking the passed address in current heap boundaries
	if (((UINT32)pBlock >= (UINT32)NcNbHeapDescr.HeapStorage) && ((UINT32)pBlock < ((UINT32)NcNbHeapDescr.HeapStorage + (UINT32)NcNbHeapDescr.TotalSize)))
	{
		return hNcNbHeap[MxGetCpuID()];
	}
	
	// IramHeap
	pBlock = (PMEMBLOCK)((UINT8 *)pPtr - ROUND(ALLOC_HEADER_SIZE, IramHeapDescr.Align));
	// checking the passed address in current heap boundaries
	if (((UINT32)pBlock >= (UINT32)IramHeapDescr.HeapStorage) && ((UINT32)pBlock < ((UINT32)IramHeapDescr.HeapStorage + (UINT32)IramHeapDescr.TotalSize)))
	{
		return hIramHeap[MxGetCpuID()];
	}
	
	// DDR1Heap
	pBlock = (PMEMBLOCK)((UINT8 *)pPtr - ROUND(ALLOC_HEADER_SIZE, DDR1HeapDescr.Align));
	// checking the passed address in current heap boundaries
	if (((UINT32)pBlock >= (UINT32)DDR1HeapDescr.HeapStorage) && ((UINT32)pBlock < ((UINT32)DDR1HeapDescr.HeapStorage + (UINT32)DDR1HeapDescr.TotalSize)))
	{
		return hDDR1Heap[MxGetCpuID()];
	}
	
	// DDR1NcnbHeap
	pBlock = (PMEMBLOCK)((UINT8 *)pPtr - ROUND(ALLOC_HEADER_SIZE, DDR1NcnbHeapDescr.Align));
	// checking the passed address in current heap boundaries
	if (((UINT32)pBlock >= (UINT32)DDR1NcnbHeapDescr.HeapStorage) && ((UINT32)pBlock < ((UINT32)DDR1NcnbHeapDescr.HeapStorage + (UINT32)DDR1NcnbHeapDescr.TotalSize)))
	{
		return hDDR1NcnbHeap[MxGetCpuID()];
	}
	
	// PhyHeap
	pBlock = (PMEMBLOCK)((UINT8 *)pPtr - ROUND(ALLOC_HEADER_SIZE, PhyHeapDescr.Align));
	// checking the passed address in current heap boundaries
	if (((UINT32)pBlock >= (UINT32)PhyHeapDescr.HeapStorage) && ((UINT32)pBlock < ((UINT32)PhyHeapDescr.HeapStorage + (UINT32)PhyHeapDescr.TotalSize)))
	{
		return hPhyHeap[MxGetCpuID()];
	}
	
	return hStorage;
}
#if 0
#define NUM_HEAPS 7 // number of heaps
HANDLE GetHeapHandle(LPVOID pPtr)
{
	UINT32 i;
	PMXCONTEXT pCtx;
	PHEAP pHeapObj;
	ICHEAP * pHeap;
	PMEMBLOCK pBlock;
	
	HANDLE hStorage = {0};
	
	HANDLE Heaps[NUM_HEAPS];
	Heaps[0] = hAppHeap[MxGetCpuID()];
	Heaps[1] = hDspHeap[MxGetCpuID()];	
	Heaps[2] = hNcNbHeap[MxGetCpuID()];
	Heaps[3] = hIramHeap[MxGetCpuID()];
	Heaps[4] = hDDR1Heap[MxGetCpuID()];
	Heaps[5] = hDDR1NcnbHeap[MxGetCpuID()];
	Heaps[6] = hPhyHeap[MxGetCpuID()];
		
	pCtx = MxInqContext();

	_ASSERT(pCtx != NULL);
	
	_ASSERT(pPtr != NULL);

	// looking for corresponding heap
	for (i = 0; i < NUM_HEAPS; i++)
	{
//	    uart_printf ("\nHeap type %d core# %d\n", (UINT32)Heaps[i].Type, (UINT32)MxGetCpuID()); // ab_debugging
		pHeapObj = MxiGetHeapDescr(pCtx, Heaps[i]); // getting next Heap Object
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
		pBlock = (PMEMBLOCK)((UINT8 *)pPtr - ROUND(ALLOC_HEADER_SIZE, pHeap->Align));
		// checking the passed address in current heap boundaries
		if (((UINT32)pBlock >= (UINT32)pHeap->HeapStorage) && ((UINT32)pBlock < ((UINT32)pHeap->HeapStorage + (UINT32)pHeap->TotalSize)))
		{
//	        uart_printf ("\n\n\n%d\n\n\n", i);
			return Heaps[i];
		}
	}

//    uart_printf ("\nno storage\n");	
	return hStorage;
}
#endif

MXRC HeapFreeByPtr(LPVOID pPtr)
{	
	// looking for corresponding heap
		
	// AppHeap
	// checking the passed address in current heap boundaries
	if (((UINT32)pPtr >= (UINT32)AppHeapDescr.HeapStorage) && ((UINT32)pPtr < ((UINT32)AppHeapDescr.HeapStorage + (UINT32)AppHeapDescr.TotalSize)))
	{
		return MxHeapFree(AppHeap, pPtr);
	}
		
	// DspHeap
	// checking the passed address in current heap boundaries
	else if (((UINT32)pPtr >= (UINT32)DspHeapDescr.HeapStorage) && ((UINT32)pPtr < ((UINT32)DspHeapDescr.HeapStorage + (UINT32)DspHeapDescr.TotalSize)))
	{
		return MxHeapFree(DspHeap, pPtr);
	}
		
	// NcnbHeap
	// checking the passed address in current heap boundaries
	else if (((UINT32)pPtr >= (UINT32)NcNbHeapDescr.HeapStorage) && ((UINT32)pPtr < ((UINT32)NcNbHeapDescr.HeapStorage + (UINT32)NcNbHeapDescr.TotalSize)))
	{
		return MxHeapFree(NcnbHeap, pPtr);
	}
		
	// IramHeap
	// checking the passed address in current heap boundaries
	else if (((UINT32)pPtr >= (UINT32)IramHeapDescr.HeapStorage) && ((UINT32)pPtr < ((UINT32)IramHeapDescr.HeapStorage + (UINT32)IramHeapDescr.TotalSize)))
	{
		return MxHeapFree(IramHeap, pPtr);
	}
		
	// DDR1Heap
	// checking the passed address in current heap boundaries
	else if (((UINT32)pPtr >= (UINT32)DDR1HeapDescr.HeapStorage) && ((UINT32)pPtr < ((UINT32)DDR1HeapDescr.HeapStorage + (UINT32)DDR1HeapDescr.TotalSize)))
	{
		return MxHeapFree(DDR1Heap, pPtr);
	}
		
	// DDR1NcnbHeap
	// checking the passed address in current heap boundaries
	else if (((UINT32)pPtr >= (UINT32)DDR1NcnbHeapDescr.HeapStorage) && ((UINT32)pPtr < ((UINT32)DDR1NcnbHeapDescr.HeapStorage + (UINT32)DDR1NcnbHeapDescr.TotalSize)))
	{
		return MxHeapFree(DDR1NcnbHeap, pPtr);
	}
    else if (HeapIsPrvCoreHeap ((UINT32)pPtr))
    {
        return MxHeapFree (PRV_CORE_HEAP, pPtr);
    }
		
	return MX_HEAP_NO_MORE_ITEMS;
}


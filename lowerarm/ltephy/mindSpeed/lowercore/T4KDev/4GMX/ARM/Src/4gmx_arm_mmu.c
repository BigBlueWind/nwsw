//-------------------------------------------------------------------------------------------
/** @file 4gmx_arm_mmu.c
 *
 * @brief ARM MMU API implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.15 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"

#ifdef MX_ARM_MMU_ENABLED

#include "4gmx_arm_mmu.h"
#include "4gmx_serv.h"
#include "4gmx_l2cache.h"

void MxiInvalidICache (void);
void MxiInvalidDCache (void);
void MxiInvalidIDCache(void);

void MxiEnableMmuMaster(UINT32 nCtrl);
void MxiEnableMmuSlave(UINT32 nCtrl);
void MxiDisableMMU (void);

void MxiEnableICache (void);
void MxiEnableDCache (void);
void MxiDisableICache (void);
void MxiDisableDCache (void);

void MxiInvalidDCache (void);
void MxiFlushDCache (void);
void MxiFlushIDCache (void);
void MxiInvalidateDataTLB(void);

void MxSycnProc(void);

/** @brief  The internal function of 4GMX system that used for base initialization of
   sub-ARM MMU system. This function is called automatically by the 4GMX kernel.

 @param pCtx [in] - the pointer to the 4GMX context
 @param nOpt [in] - the initialization options (see MXINIT.ConfigSet)

 @return [MXRC] error code

 \ingroup group_lte_4gmx_kernel_mmu

 */

MXRC MxMmuSubSysInit(PMXCONTEXT pCtx, UINT32 nOpt)
{
    UINT32 nBlockCnt;

    IRQTYPE irq = ARM_INT_disable ();

    if (pCtx->mmu_l2_table_ptr != NULL)
    {
        nBlockCnt = pCtx->mmu_l2_table_size / (MMU_L2_TABLE_COARSE_INDEX_NUM * sizeof (UINT32));

        if (nBlockCnt != NULL)
        {
            MxiDefSysPart (&pCtx->mmu_l2_part,
                            pCtx->mmu_l2_table_ptr,
                            MMU_L2_TABLE_COARSE_INDEX_NUM * sizeof (UINT32),
                            nBlockCnt);
        }
    }

    MxiInvalidIDCache ();

    ARM_INT_restore (irq);

    return MX_OK;
}

/** @brief  The function inits (creates ) a master table with default
   parameters specified by <ctrl> flags.

 @param pStorage [out] - the pointer to memory where master table will be allocated
 @param PageID [in] - the ID of PTE, see MMU_PAGE_ID type
 @param ctrl  [in] - the default parameters of PTE (page table entry)

 Warning! The <pStorage> should be alignment on 16 KB

 @return [MASTERTABLE] the descriptor of master table

 \ingroup group_lte_4gmx_kernel_mmu

 */

MASTERTABLE MxMmuCreateMasterTable(LPVOID pStorage, MMU_PAGE_ID PageID,
                                   UINT32 ctrl)
{
    UINT32 *p, nPattern = 0, i;

    _ASSERT(pStorage != NULL);
    _ASSERT((((UINT32) pStorage) & (0x3FFF)) == 0);
    _ASSERT(PageID == MMU_1M_PAGE_ID || PageID == MMU_FAULT_PAGE_ID);

    p = (UINT32 *) pStorage;

    if (PageID == MMU_1M_PAGE_ID)
    {
        ctrl &= ~(MMU_PAGE_ID_MASK);
        ctrl &= ~(0x1F << 15); /* 19..15 bits should be set to 0   */
        ctrl &= ~(1 << 9);     /* 9 Bit should be set to 0         */

        nPattern = (ctrl);

        nPattern |= MMU_1M_PAGE_ID;

        /* 4096 - entries in master page = 4GB memory space */

        for (i = 0; i < 4096; i++)
        {
            *p++ = MMU_CREATE_MASTER_ADDR(i) | nPattern;
        }
    }
    else if (PageID == MMU_FAULT_PAGE_ID)
    {
        ctrl = 0x0;

        /* 4096 - entries in master page = 4GB memory space */

        for (i = 0; i < 4096; i++)
        {
            *p++ = nPattern;
        }
    }

    return (MASTERTABLE) pStorage;
}

/** @brief The function changes/sets parameter of sections (1MBytes region )

 @NOTE  The function should be called before MxSetTTB function

 @param pMTable   [in] - the descriptor of master table
 @param BaseMem   [in] - the address of virtual page
 @param MapTo     [in] - the physical address, the virtual address is mapped to
 @param nSectCount[in] - the number of section (1MBytes size blocks)
 @param ctrl      [in] - the section's attributes

 @return [MXRC] error code

 \ingroup group_lte_4gmx_kernel_mmu

 */

MXRC MxMmuSetSectParam(MASTERTABLE pMTable, UINT32 BaseMem, UINT32 MapTo,
                       UINT32 nSectCount, UINT32 ctrl)
{
    UINT32 id, i, nPattern = 0;
    IRQTYPE irq;

    _ASSERT(pMTable != NULL);
    _ASSERT(((UINT32) pMTable & 0x3FFF) == 0);

    ctrl &= ~MMU_PAGE_ID_MASK;
    //ctrl &= ~(0xF << 12);  /* 19..12 bits should be set to 0   */
    //ctrl &= ~(1 << 9);     /* 9 Bit should be set to 0         */
    //ctrl &= ~(1 << 4);     /* 4 bit should be set to 0 (ARMv6) */

    nPattern = (ctrl);     /* 4 bit should be set to 0 (ARMv6) */

    nPattern |= MMU_1M_PAGE_ID;

    irq = ARM_INT_disable ();

    for (i = 0; i < nSectCount; i++)
    {
        id = MMU_GET_MASTER_ADDR(BaseMem);

        pMTable[id] = nPattern | ((UINT32) MapTo & ~0xFFFFF);
        BaseMem += 0x100000;
        MapTo += 0x100000;
    }

    if (MxMmuIsEnabled())
        MxiInvalidateDataTLB();

    ARM_INT_restore (irq);

    return MX_OK;
}

MXRC MxMmuSetTTB(MASTERTABLE pMTable, UINT32 nTTBOpt)
{
    IRQTYPE irq;

    _ASSERT(pMTable != NULL);
    _ASSERT(((UINT32) pMTable & (0x3FFF)) == 0);

    if (pMTable == NULL)
        return MX_OK;

    irq = ARM_INT_disable ();

    mmu_write_ttb0((((UINT32) pMTable) | nTTBOpt));

    ARM_INT_restore (irq);

    return MX_OK;
}

/** @brief The function activates/deactivates MMU
 @param nCtrl [in] - enable/disable an MMU, see: MMU_CTRL_xxx macros in this file
 @return [MXRC]

 \ingroup group_lte_4gmx_kernel_mmu

 */

MXRC MxMmuCtrl(UINT32 nCtrl)
{
    IRQTYPE irq = ARM_INT_disable ();

    PMXCONTEXT pCtx = MxInqContext ();

    _ASSERT_PTR(pCtx);

    if ((pCtx->SysCfg & SYS_INIT_AMP_MODE) == 0)
    {
        nCtrl |= ((UINT32)1 << 31);
    }

    if (nCtrl & MMU_CTRL_L1_ENABLED)
    {
        if ((pCtx->SysCfg & SYS_SMP_MASTER) != 0)
        {
            MxiEnableMmuMaster(nCtrl);
        }
        else
        {
            MxiEnableMmuSlave (nCtrl);
        }

#ifdef MX_ARM_L2_CACHE_ENABLED

        if (nCtrl & MMU_CTRL_L2_ENABLED)
            MxL2_init (MX_ARM_L2_AUX_CFG, 0xFFFFFFFF);
#endif
    }
    else
    {
#ifdef MX_ARM_L2_CACHE_ENABLED

        MxL2_flush_all ();
        MxL2_disable();

#endif
        MxiDisableMMU ();
    }

    ARM_INT_restore (irq);

    return MX_OK;
}

/** @brief The function sets domain access flags
 @param nDomainID [in] - the domain ID [0..15]
 @param nAccess   [in] - the access flags: see MMU_DOMAIN enum
 @return [MXRC] error code

 \ingroup group_lte_4gmx_kernel_mmu

 */

MXRC MxMmuSetDomain(UINT32 nDomainID, UINT32 nAccess)
{
    UINT32 nVal;
    IRQTYPE irq;

    _ASSERT(nDomainID < 16);

    nAccess &= 3;

    irq = ARM_INT_disable ();

    nVal = mmu_read_domain_access();
    nVal &= ~(3 << (nDomainID * 2));
    nVal |= (nAccess << (nDomainID * 2));

    mmu_write_domain_access(nVal);

    ARM_INT_restore (irq);

    return MX_OK;
}

/** @brief This function cleans and invalidates ARM D-CACHE
 *

 \ingroup group_lte_4gmx_kernel_mmu

 */

void MxFlushDCache (void)
{
	IRQTYPE irq;

    irq = ARM_INT_disable ();

	MxiFlushDCache ();

#ifdef MX_ARM_L2_CACHE_ENABLED
    MxL2_flush_all ();
#endif

	ARM_INT_restore (irq);
}

void MxFlushIDCache (void)
{
	IRQTYPE irq = ARM_INT_disable ();

	MxiFlushIDCache ();

#ifdef MX_ARM_L2_CACHE_ENABLED
    MxL2_flush_all ();
#endif

	ARM_INT_restore (irq);
}

void MxInvalidDCache (void)
{
	IRQTYPE irq = ARM_INT_disable ();

	MxiInvalidDCache ();

#ifdef MX_ARM_L2_CACHE_ENABLED
    MxL2_inv_all ();
#endif

	ARM_INT_restore (irq);
}

void MxDisableDCache(void)
{
	IRQTYPE irq = ARM_INT_disable ();

	MxiFlushIDCache();

#ifdef MX_ARM_L2_CACHE_ENABLED
    MxL2_inv_all ();
#endif

	ARM_INT_restore (irq);

}

/** @brief This function returns handler of MMU table for the current processor
    @return [MASTERTABLE] MMU table handler

    \ingroup group_lte_4gmx_kernel_mmu

    */

MASTERTABLE MxMmuGetMasterTable (void)
{
    PMXCONTEXT pCtx;
    pCtx = MxInqContext ();
    _ASSERT_PTR(pCtx);

    return (MASTERTABLE)pCtx->mmu_l1_table_ptr;
}

/** @brief This function returns handle of MMU section (1MB descriptor)
    @param hTable[in] - the MMU master table handle
    @param nAddr [in] - the address of memory where section is located, it sgould be aligned on 1MB
    @return [MMUSECTION] section handle

    \ingroup group_lte_4gmx_kernel_mmu

    */

MMUSECTION MxMmuGetSection (MASTERTABLE hTable, UINT32 nAddr)
{
    _ASSERT_PTR (hTable);
    _ASSERT (((UINT32)hTable & 0x3FFF) == 0);

    // The section is 1MB size
    return (MMUSECTION)&(hTable[nAddr >> 20]);
}

/** @brief This function returns handle of MMU page (64/4 KB descriptor)
    @param hTable[in] - the MMU master table handle
    @param nAddr [in] - the address of memory where page is located
    @return [MMUPAGE] page handle

    \ingroup group_lte_4gmx_kernel_mmu

    */

MMUPAGE MxMmuGetPage (MASTERTABLE hTable, UINT32 nAddr)
{
    MMUSECTION hSect = MxMmuGetSection (hTable, nAddr);
    PUINT32 nL2Addr;

    if (hSect == NULL)
    {
        _ASSERT_PTR(hSect);
        return NULL;
    }

    // chech if 1MB section is divided to pages

    if (((*hSect) & MMU_PAGE_ID_MASK) != MMU_COARSE_PAGE_ID)
    {
        _ASSERT (((*hSect) & MMU_PAGE_ID_MASK) == MMU_COARSE_PAGE_ID);
        return NULL;
    }

    nL2Addr = (PUINT32)((*hSect) & ~0x3FF);

    // to proc 64 and 4 KB pages

    if ((nL2Addr[0] & MMU_PAGE_ID_MASK) == MMU_COARSE_PAGE_64)
    {
        //return &(nL2Addr [(nAddr >> 12) & 0xFF]);
        // Just to point to the first page in the list
        return &(nL2Addr [(nAddr >> 12) & 0xF0]);
    }


    return &(nL2Addr [(nAddr >> 12) & 0xFF]);
}

/** @brief This function initializes MMU L2 table for page64 mode
    @param pL2 [in] - the pointer to the L2 table
    @param nOpt[in] - the page64 options

    \ingroup group_lte_4gmx_kernel_mmu

    */

void MxiMmuInitPage64 (PUINT32 pL2, UINT32 nAddr, UINT32 nOpt)
{
    UINT32 i;

    nAddr &= 0xFFFF0000;

    // bit mask for:
    //==============
    // 15    - XN
    // 14-12 - TEX
    // 11    - nG
    // 10    - S
    // 9     - APX
    // 5-4   - AP
    // 3     - C
    // 2     - B
    // 1     - 0 always for 64KB page
    // 0     - 1 always for 64KB page

    nOpt &= (1 << 15) | (7 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (3 << 4) | (1 << 3) | (1 << 2);

    /*  Second-level translation tables are 1KB in size, and must be aligned on a 1KB boundary. Each 32-bit entry
        in a table provides translation information for 4KB of memory. VMSAv7 supports two page sizes:
           • Large pages are 64KByte in size
           • Small pages are 4KByte in size.
        The required replication of Large page descriptors preserves this 4KB per entry relationship:
        (4KBytes per entry) x (16 replicated entries) = 64KBytes = Large page size
    */

    for (i = 0; i < 16 * 16; i++)
    {
        if ( (i != 0) && ((i % 16) == 0))
            nAddr += (64 * 1024);

        pL2 [i] = nAddr | nOpt | MMU_COARSE_PAGE_64;
    }

    if (MxMmuIsEnabled())
        MxiInvalidateDataTLB ();
}

/** @brief This function initializes MMU L2 table for page64 mode
    @param pL2 [in] - the pointer to the L2 table
    @param nOpt[in] - the page64 options

    \ingroup group_lte_4gmx_kernel_mmu

    */

void MxiMmuInitPage4 (PUINT32 pL2, UINT32 nAddr, UINT32 nOpt)
{
    UINT32 i;

    nAddr &= 0xFFFFF000;

    // bit mask for:
    //==============
    // 11    - nG
    // 10    - S
    // 9     - APX
    // 8-6   - TEX
    // 5-4   - AP
    // 3     - C
    // 2     - B
    // 1     - 1 always for 4KB page
    // 0     - XN

    nOpt &= (1 << 11) | (1 << 10) | (1 << 9) | (0x1F << 4) | (1 << 3) | (1 << 2) | (1);

    for (i = 0; i < 256; i++)
    {
        pL2 [i] = nAddr | nOpt | MMU_COARSE_PAGE_4;

        nAddr += (4*1024);
    }

    if (MxMmuIsEnabled())
        MxiInvalidateDataTLB ();
}


/** @brief This function sets type of 1MB section

    @param hSect    [in] - the section handle , see: MxMmuGetSection(...)
    @param nType    [in] - the type of section, see MMU_SECTION_TYPE_xxx
    @param nSOptions[in] - additional section options, the data depends on the section type
    @param nPOptions[in] - additional page options, it's used in case of page64 and page4

    @return [MXRC] an error code

    \ingroup group_lte_4gmx_kernel_mmu

    */

MXRC MxMmuSetSectionType (MMUSECTION hSect, UINT32 nType, UINT32 nSOptions, UINT32 nPOptions)
{
    PMXCONTEXT pCtx;
    MXRC rc = MX_OK;
    LPVOID pL2SubPage, pL2PrevPage = NULL;
    IRQTYPE irq;
    UINT32 nAddr=0;

    _ASSERT_PTR (hSect);

    pCtx = MxInqContext ();

    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable();

    // to deallocate the page space
    // if pages space was allocated before

    if (((*hSect) & MMU_PAGE_ID_MASK) == MMU_COARSE_PAGE_ID)
    {
        pL2PrevPage = (LPVOID)((*hSect) & (~0x3FF));

        // if it is in the range of page storage

        if ((UINT32)pL2PrevPage >= (UINT32)pCtx->mmu_l2_table_ptr &&
            (UINT32)pL2PrevPage < ((UINT32)pCtx->mmu_l2_table_ptr + pCtx->mmu_l2_table_size))
        {
            MxiFreeSysPart (&pCtx->mmu_l2_part, pL2PrevPage);
            pL2PrevPage = NULL;
        }
    }

    // this address will be used to config pages
    // if 1/16 MB section is devided to 64/4 KB pages

    if (((*hSect) & MMU_PAGE_ID_MASK) == MMU_1M_PAGE_ID)
    {
        if ((*hSect) & (1 << 18))
            nAddr = (*hSect) & 0xFF000000;
        else
            nAddr = (*hSect) & 0xFFF00000;
    }

    // to change the section type and set section parameters

    switch (nType)
    {
        case MMU_SECTION_TYPE_FAULT:
            (*hSect) = MMU_FAULT_PAGE_ID;
            break;

        case MMU_SECTION_TYPE_PAGED_64:

            pL2SubPage = MxiAllocSysPart (&pCtx->mmu_l2_part);

            if (pL2SubPage == NULL)
            {
                rc = MX_ARM_MMU_NO_FREE_PAGES;
                _ASSERT_RC (rc);
                break;
            }

            MxiMmuInitPage64 (pL2SubPage, nAddr, nPOptions);

            nSOptions &= (0xF << 5) | (1 << 3); // To save domains only and NS bit
            (*hSect) = ((UINT32)pL2SubPage & (~0x3FF)) | nSOptions | (MMU_COARSE_PAGE_ID);
            break;

        case MMU_SECTION_TYPE_PAGED_4:

            pL2SubPage = MxiAllocSysPart (&pCtx->mmu_l2_part);

            if (pL2SubPage == NULL)
            {
                rc = MX_ARM_MMU_NO_FREE_PAGES;
                _ASSERT_RC (rc);
                break;
            }

            MxiMmuInitPage4 (pL2SubPage, nAddr, nPOptions);

            nSOptions &= (0xF << 5); // To save domains only and clear all other bits
            (*hSect) = ((UINT32)pL2SubPage & (~0x3FF)) | nSOptions | (MMU_COARSE_PAGE_ID);

            break;

        case MMU_SECTION_TYPE_1M_SECTION:

            // To save: DCACHE  [3-2]  bits
            // To save: domains [8-5]  bits
            // To save: AP      [11-10]bits
            // To save: TEX     [14-12]bits
            // To save: MAP ADDR[31-20]bits

            nSOptions &= ((0xF << 5) | (1 << 2) | (1 << 3) | (3 << 10) | (7 << 12) | ((UINT32)0xFFF << 20));
            (*hSect) = nSOptions | (MMU_1M_PAGE_ID);
            break;

        case MMU_SECTION_TYPE_16M_SECTION:

            // To save: DCACHE  [3-2]  bits
            // To save: AP      [11-10]bits
            // To save: TEX     [14-12]bits
            // To save: MAP ADDR[31-24]bits

            nSOptions &= ((1 << 2) | (1 << 3) | (3 << 10) | (7 << 12)| ((UINT32)0xFF << 24));
            (*hSect) =  (1 << 18) | nSOptions | (MMU_1M_PAGE_ID);
            break;

         default:
            rc = MX_ARM_MMU_UNSUPPORTED;
            break;
    }

    if (MxMmuIsEnabled())
        MxiInvalidateDataTLB ();
    
    ARM_INT_restore(irq);

    _ASSERT_RC(rc);
    return rc;
}

/** @brief This function sets page parameter (64/4KB page)

    @param hPage [in] - the page handler (see: MxMmuGetPage)
    @param MapTo [in] - mapping to physical address
    @param nOpt  [in] - the page options, see: page specific options

    @return [MXRC] an error code

    \ingroup group_lte_4gmx_kernel_mmu

    */

MXRC MxMmuSetPageParam (MMUPAGE hPage, UINT32 MapTo, UINT32 nOpt)
{
    UINT32 nPageType, i;
    IRQTYPE irq;

    _ASSERT_PTR(hPage);

    irq = ARM_INT_disable();

    nPageType = (*hPage & MMU_PAGE_ID_MASK);

    if (nPageType == MMU_COARSE_PAGE_64)
    {
        nOpt &= 0x7FFC;
        MapTo&= 0xFFFF0000;

        for (i = 0; i < 16; i++)
            hPage[i] = (MapTo | nOpt | nPageType);
    }
    else
    {
        nOpt &= 0xFFC;
        MapTo&= 0xFFFFF000;

        hPage[0] = (MapTo | nOpt | nPageType);
    }

    if (MxMmuIsEnabled())
        MxiInvalidateDataTLB ();
    
    ARM_INT_restore(irq);

    return MX_OK;
}

/** @brief This function returns mapping address for the section,
            if a section is devided for pages, this function returns address of
            the secondary translation table

    @NOTE it's mainly designed to be used with MxMmuSetSectionType function

    @param hSect [in] - the section handle

    @return [UINT32] physical address

    \ingroup group_lte_4gmx_kernel_mmu

    */

UINT32 MxMmuGetSectionMapAddress (MMUSECTION hSect)
{
    _ASSERT_PTR(hSect);

    switch ((*hSect) & MMU_PAGE_ID_MASK)
    {
        case MMU_1M_PAGE_ID:
            {
                // if 16 MB section
                if ((*hSect) & (1 << 18))
                    return ((*hSect) >> 24);
                else
                    return ((*hSect) >> 20);
            }

        case MMU_COARSE_PAGE_ID:
            return (*hSect) >> 10;

        default:
            return 0;
    }
}


UINT32 MxiMmuGetPageMapping (PUINT32 pL2, UINT32 va)
{
    UINT32 nVal;

    if (pL2 == NULL)
    {
        _ASSERT_PTR(pL2);
        return NULL;
    }

    // to proc 64 and 4 KB pages

    if ((pL2[0] & MMU_PAGE_ID_MASK) == MMU_COARSE_PAGE_64)
    {
        nVal = pL2[(va >> 12) & 0xFF];

        return (nVal & 0xFFFF0000) | (va & 0xFFFF);
    }

    nVal = pL2[(va >> 12) & 0xFF];

    return (nVal & 0xFFFFF000) | (va & 0xFFF);
}

/** @brief This function is designed to convert virtual to physical address
            according to the mapping table

    @param nAddr [in] - the virtual address

    @return [UINT32] physical address

    \ingroup group_lte_4gmx_kernel_mmu

    */

UINT32 MxMmuVToP (UINT32 nAddr)
{
    UINT32 nSectVal;
    MASTERTABLE hTable;

    hTable = MxMmuGetMasterTable ();

    _ASSERT_PTR(hTable);

    nSectVal = hTable [nAddr >> 20];

    switch (nSectVal & MMU_PAGE_ID_MASK)
    {
        case MMU_1M_PAGE_ID:
        {
            // to proc 16MB section
            if (nSectVal & (1 << 18))
                return (nSectVal & (0xFF000000)) | (nAddr & 0xFFFFFF);

            return (nSectVal & 0xFFF00000) |  (nAddr & 0x000FFFFF);
        }

        case MMU_COARSE_PAGE_ID:
            return MxiMmuGetPageMapping ((PUINT32)(nSectVal & ~(0x3FF)), nAddr);

        default:
            return (UINT32)-1;
    }
}

ASMPROC UINT32 MxMmuVaToPA(UINT32 addr)
{
    MRS 	r3, CPSR				// Save IRQ Status
	ORR 	r1, r3, #(0x80:OR:0x40)	// Disable IRQ/FIQ
	MSR 	CPSR_c, r1				// Disable IRQ/FIQ

    BIC r0, #0x1F
    MCR p15,0,r0,c7,c8,0
    MRC p15,0,r0,c7,c4,0

    MSR 	CPSR_c, r3    // restore IRQ/FIQ
    
    BX LR
}

/** @brief This function checks DCache flag for the specific memory

    @param nAddr [in] - the address

    @return [BOOL] 1 or 0

    \ingroup group_lte_4gmx_kernel_mmu

    */

BOOL MxMmuIsDCached (UINT32 nAddr)
{
    UINT32 val;

    if ((sys_read_control() & (CR_C | CR_M)) != (CR_C | CR_M))
        return 0;

    val = MxMmuVaToPA(nAddr);

    // if translation is fault
    if (val & 1)
        return 0;

    /*
    Inner[2:0], bits [6:4]
        Inner memory attributes from the translation table entry. Permitted values are:
        0b111 Write-Back, no Write-Allocate
        0b110 Write-Through
        0b101 Write-Back, Write-Allocate
        0b011 Device
        0b001 Strongly-ordered
        0b000 Non-cacheable.
    Other encodings for Inner[2:0] are reserved.
        Outer[1:0], bits [3:2]
        Outer memory attributes from the translation table. Possible values are:
        0b11 Write-Back, no Write-Allocate.
        0b10 Write-Through, no Write-Allocate.
        0b01 Write-Back, Write-Allocate.
        0b00 Non-cacheable.
    */

    return (val & (1 << 6)) | ((val >> 2) & 3);
}

/** @brief This function retuns DCache parameters for the specific page
    @param nAddr [in] - the address
    @return [UINT32] bit[3:2] - outer cache
                     bit[6:4] - inner cache 

    \ingroup group_lte_4gmx_kernel_mmu

    */

BOOL MxMmuGetDCacheParam (UINT32 nAddr)
{
    UINT32 val;

    if ((sys_read_control() & (CR_C | CR_M)) != (CR_C | CR_M))
        return 0;

    val = MxMmuVaToPA(nAddr);

    // if translation is fault
    if (val & 1)
        return 0;

    /***************************************************************************************
    PA, bits [31:12]
        Physical Address. The physical address corresponding to the supplied virtual address.
        Address bits [31:12] are returned.

    Bits [10:1] Return information from the translation table entry used for the translation:
         NOS, bit [10]
                Not Outer Shareable attribute. Indicates whether the physical memory is Outer
                Shareable:
                    0 Memory is Outer Shareable
                    1 Memory is not Outer Shareable.
                On an implementation that do not support Outer Shareable, this bit is
                UNK/SBZP.
         NS, bit [9] Non-secure. The NS bit from the translation table entry

    SH, bit [7]
        Shareable attribute. Indicates whether the physical memory is Shareable:
        0 Memory is Non-shareable
        1 Memory is Shareable.

    Inner[2:0], bits [6:4]
        Inner memory attributes from the translation table entry. Permitted values are:
        0b111 Write-Back, no Write-Allocate
        0b110 Write-Through
        0b101 Write-Back, Write-Allocate
        0b011 Device
        0b001 Strongly-ordered
        0b000 Non-cacheable.
        Other encodings for Inner[2:0] are reserved.

    Outer[1:0], bits [3:2]
        Outer memory attributes from the translation table. Possible values are:
        0b11 Write-Back, no Write-Allocate.
        0b10 Write-Through, no Write-Allocate.
        0b01 Write-Back, Write-Allocate.
        0b00 Non-cacheable.

    SS, bit [1] SuperSection

    F,  bit [0] F bit is 0 if the conversion completed successfully.
   
    ***************************************************************************/

    return val;
}

/** @brief This function is designed to set access permission (AP)
           to the selected memory region (1M/64K/4K)

    @param nAddr [in] - the address of memory region
    @param nAP   [in] - the access permission, see: enum MMU_AP

    @return [MXRC] an error code

    \ingroup group_lte_4gmx_kernel_mmu

    */

MXRC MxMmuSetMemAP (UINT32 nAddr, UINT32 nAP)
{
    MASTERTABLE hTable;
    MMUSECTION hSect;
    MMUPAGE hPage;
    UINT32 tmp, i;
    MXRC rc = MX_OK;
    IRQTYPE irq;

    hTable = MxMmuGetMasterTable ();

    if (hTable == NULL)
    {
        rc = MX_ARM_MMU_NO_MASTER_TABLE;
        _ASSERT_RC (rc);
        return rc;
    }

    irq = ARM_INT_disable ();

    nAddr &= ~0x3FF;    // 4K aligment

    hSect = MxMmuGetSection (hTable, nAddr);

    _ASSERT_PTR (hSect);

    if ((*hSect & MMU_PAGE_ID_MASK) == MMU_1M_PAGE_ID)
    {
        tmp = (*hSect) & ~((1 << 15) | (3 << 10));
        (*hSect) = tmp | MMU_MASTER_AP(nAP);
    }
    else
    {
        // it returns the first page in the list
        hPage = MxMmuGetPage (hTable, nAddr);

        switch ((*hPage & MMU_PAGE_ID_MASK))
        {
            case MMU_COARSE_PAGE_64:
            {
                tmp = (*hPage) & ~((1 << 9) | (3 << 4));
                //(*hPage) = tmp | MMU_PAGE_AP(nAP);

                // in a loop to duplicate it !

                for (i = 0; i < 16; i++)
                {
                    hPage[i] = tmp | MMU_PAGE_AP(nAP);
                }

                break;
            }

            case (MMU_COARSE_PAGE_4 | 1): // PAGE4 + XN flag
            case MMU_COARSE_PAGE_4:
            {
                tmp = (*hPage) & ~((1 << 9) | (3 << 4));
                (*hPage) = tmp | MMU_PAGE_AP(nAP);
                break;
            }

            default:
                ARM_INT_restore (irq);
                rc = MX_ARM_MMU_UNSUPPORTED_PAGE;
                _ASSERT_RC(rc);
                return rc;
        }
    }

    if (MxMmuIsEnabled())
        MxiInvalidateDataTLB ();
    
    ARM_INT_restore (irq);
    return rc;
}

/** @brief This function is designed to set non-exec bit (XN)
           to the selected memory region (1M/64K/4K)

    @param nAddr [in] - the address of memory region
    @param nXN   [in] - the flag of XN bit

    @return [MXRC] an error code

    \ingroup group_lte_4gmx_kernel_mmu

    */

MXRC MxMmuSetMemXN (UINT32 nAddr, UINT32 nXN)
{
    MASTERTABLE hTable;
    MMUSECTION hSect;
    MMUPAGE hPage;
    UINT32 i;
    MXRC rc = MX_OK;
    IRQTYPE irq;

    hTable = MxMmuGetMasterTable ();

    if (hTable == NULL)
    {
        rc = MX_ARM_MMU_NO_MASTER_TABLE;
        _ASSERT_RC (rc);
        return rc;
    }

    irq = ARM_INT_disable ();

    nAddr &= ~0x3FF;    // 4K aligment

    hSect = MxMmuGetSection (hTable, nAddr);

    _ASSERT_PTR (hSect);

    if ((*hSect & MMU_PAGE_ID_MASK) == MMU_1M_PAGE_ID)
    {
        if (nXN)
            (*hSect) |= MMU_MASTER_NX;
        else
            (*hSect) &= ~MMU_MASTER_NX;
    }
    else
    {
        // it returns the first page in the list
        hPage = MxMmuGetPage (hTable, nAddr);

        switch ((*hPage & MMU_PAGE_ID_MASK))
        {
            case MMU_COARSE_PAGE_64:
            {
                // in a loop to duplicate it !

                for (i = 0; i < 16; i++)
                {
                    if (nXN)
                        hPage[i] |= MMU_PAGE64_NX;
                    else
                        hPage[i] &= ~MMU_PAGE64_NX;
                }

                break;
            }

            case (MMU_COARSE_PAGE_4 | 1): // PAGE4 + XN flag
            case MMU_COARSE_PAGE_4:
            {
                if (nXN)
                    (*hPage) |= MMU_PAGE4_NX;
                else
                    (*hPage) &= ~MMU_PAGE4_NX;
                
                break;
            }

            default:
                ARM_INT_restore (irq);
                rc = MX_ARM_MMU_UNSUPPORTED_PAGE;
                _ASSERT_RC(rc);
                return rc;
        }
    }

    if (MxMmuIsEnabled())
        MxiInvalidateDataTLB ();
    
    ARM_INT_restore (irq);
    return rc;
}

/** @brief This function is designed to set/change cache parameters for the 
           specified pages

    @param nAddr     [in] - the address of memory region
    @param nSize     [in] - the size of memory in bytes 
    @param Cache     [in] - the cache parameters, see: type MXMMUTEX and TEX description

    @return [MXRC] an error code

    \ingroup group_lte_4gmx_kernel_mmu

    */

MXRC MxMmuSetMemCacheCtrl (UINT32 nAddr, UINT32 nSize, MXMMUTEX Cache)
{
    MASTERTABLE hTable;
    MMUSECTION hSect;
    MMUPAGE hPage;
    UINT32 tmp, i;
    MXRC rc = MX_OK;
    IRQTYPE irq;
    UINT32 tex, procSize = 0;

    hTable = MxMmuGetMasterTable ();

    if (hTable == NULL)
    {
        rc = MX_ARM_MMU_NO_MASTER_TABLE;
        _ASSERT_RC (rc);
        return rc;
    }

    irq = ARM_INT_disable ();

    nAddr &= ~0x3FF;    // 4K aligment

    while (procSize < nSize)
    {
        hSect = MxMmuGetSection (hTable, nAddr);

        _ASSERT_PTR (hSect);
        
        // ----------------------------------------------
        // 1 MB and 16 MB pages processing 
        // ----------------------------------------------
        if ((*hSect & MMU_PAGE_ID_MASK) == MMU_1M_PAGE_ID)
        {
            // TEX[2:0] - bit[14:12]
            // C        - bit[3]
            // B        - bit[2]
            
            tmp = (*hSect) & ~((7 << 12)|(1<<3)|(1<<2));
            tex = ((UINT32)Cache.tex << 12) | ((UINT32)Cache.c << 3) | ((UINT32)Cache.b << 2);
            (*hSect) = tmp | tex;

            procSize += 1024*1024;
        }
        else
        {
            // it returns the first page in the list
            hPage = MxMmuGetPage (hTable, nAddr);

            switch ((*hPage & MMU_PAGE_ID_MASK))
            {
                case MMU_COARSE_PAGE_64:
                {
                    // TEX[2:0] - bit[14:12]
                    // C        - bit[3]
                    // B        - bit[2]
                    
                    tmp = (*hPage) & ~((7 << 12)|(1<<3)|(1<<2));
                    tex = ((UINT32)Cache.tex << 12) | ((UINT32)Cache.c << 3) | ((UINT32)Cache.b << 2);

                    // in a loop to duplicate it !

                    for (i = 0; i < 16; i++)
                    {
                        hPage[i] = tmp | tex;
                    }

                    procSize += (64*1024);

                    break;
                }

                case (MMU_COARSE_PAGE_4 | 1): // PAGE4 + XN flag
                case MMU_COARSE_PAGE_4:
                {
                    // TEX[2:0] - bit[8:6]
                    // C        - bit[3]
                    // B        - bit[2]
                    
                    tmp = (*hPage) & ~((7 << 6)|(1<<3)|(1<<2));
                    tex = ((UINT32)Cache.tex << 6) | ((UINT32)Cache.c << 3) | ((UINT32)Cache.b << 2);
                    
                    (*hPage) = tmp | tex;

                    procSize += (4*1024);
                    
                    break;
                }

                default:
                    ARM_INT_restore (irq);
                    rc = MX_ARM_MMU_UNSUPPORTED_PAGE;
                    _ASSERT_RC(rc);
                    return rc;
            }
        }
        
    }

    if (MxMmuIsEnabled())
        MxiInvalidateDataTLB ();
    
    ARM_INT_restore (irq);
    return rc;
}

/** @brief This function returns type of the region 1M/64KB/4KB

    @param nAddr [in] - the address of the region

    @return [UINT32] ID: MMU_SECTION_TYPE_xxx

    \ingroup group_lte_4gmx_kernel_mmu

    */

UINT32 MxMmuGetRegionMapType (UINT32 nAddr)
{
    MASTERTABLE hTable;
    MMUSECTION hSect;
    MMUPAGE hPage;

    hTable = MxMmuGetMasterTable ();

    if (hTable == NULL)
    {
        _ASSERT_PTR(hTable);
        return MMU_SECTION_TYPE_FAULT;
    }

    hSect = MxMmuGetSection (hTable, nAddr);

    _ASSERT_PTR(hSect);

    if ((*hSect & MMU_PAGE_ID_MASK) == MMU_1M_PAGE_ID)
    {
        if (*hSect & (1 << 18))
            return MMU_SECTION_TYPE_16M_SECTION;
        return MMU_SECTION_TYPE_1M_SECTION;
    }

    hPage = MxMmuGetPage (hTable, nAddr);

    _ASSERT_PTR(hPage);

    if ((*hPage & MMU_PAGE_ID_MASK) == MMU_COARSE_PAGE_64)
        return MMU_SECTION_TYPE_PAGED_64;

    return MMU_SECTION_TYPE_PAGED_4;
}

/** @brief This function checks is MMU enabled or not 

    @return [BOOL] flag */

BOOL MxMmuIsEnabled(void)
{
    return (sys_read_control() & CR_M);
}

/** @brief This function checks is D-Cache enabled or not 

    @return [BOOL] flag */

BOOL MxMmuIsDCache(void)
{
    return (sys_read_control() & CR_C);
}

/** @brief This function checks is D-Cache enabled or not 

    @return [BOOL] flag */

BOOL MxMmuIsICache(void)
{
    return (sys_read_control() & CR_I);
}

/** @brief This function checks is FlowPrediction enabled or not 

    @return [BOOL] flag */

BOOL MxMmuIsIFlowPred(void)
{
    return (sys_read_control() & CR_Z);
}

UINT32 MmuIsMemInfoEq (MX_MMU_MEM_INFO *p1, MX_MMU_MEM_INFO *p2)
{
    _ASSERT_PTR(p1);
    _ASSERT_PTR(p2);

    if (p1->AP != p2->AP)
        return 0;

    if (p1->Tex != p2->Tex)
        return 0;

    if (p1->XN != p2->XN)
        return 0;

    if (p1->Shared != p2->Shared)
        return 0;

    if (p1->Domain != p2->Domain)
        return 0;

    return 1;
}

MX_MMU_MEM_INFO MmuLoadPageMemInfo(MASTERTABLE hMt, UINT32 nAddr)
{
    UINT32 nSect, nPageInfo;
    MMUPAGE hPage;
    MX_MMU_MEM_INFO info;
    
    memset (&info, 0, sizeof (info));

    nSect = hMt[nAddr>>20];

    info.Addr   = nAddr;
    info.Domain = (nSect >> 5) & 0xF;
    
    hPage = (MMUPAGE)(nSect & (~0x3FF));

    if ((*hPage & MMU_PAGE_ID_MASK) == MMU_COARSE_PAGE_64)
    {
        nPageInfo = hPage[(nAddr >> 12) & 0xF0];
    
        info.Size   = 64*1024;
        info.AP     = ((nPageInfo >> 4) & 3) | (((nPageInfo >> 9) & 1) << 2);
        info.Shared = (nPageInfo>>10) & 1;
        info.Tex    = (((nPageInfo>>12) & 7) << 2) | (((nPageInfo>>3) & 1) << 1) | (((nPageInfo>>2) & 1) << 0);
        info.XN     = (nPageInfo>>15) & 1;
    }
    else
    {
        nPageInfo = hPage[(nAddr >> 12) & 0xFF];

        info.Size   = 4*1024;
        info.AP     = ((nPageInfo >> 4) & 3) | (((nPageInfo >> 9) & 1) << 2);
        info.Shared = (nPageInfo>>10) & 1;
        info.Tex    = (((nPageInfo>>6) & 7) << 2) | (((nPageInfo>>3) & 1) << 1) | (((nPageInfo>>2) & 1) << 0);
        info.XN     = (nPageInfo>>0) & 1;
    }
    
    return info;
}

/** @brief This function is designed to scan memory defined with MMU table and 
           to return information to the application about memory regions 

    @param hMt      [in]        - the handle of MMU master (1MB) table
    @param pHandle  [in/out]    - the handle used by the system

    @return [MX_MMU_MEM_INFO] information about memory region */
           
MX_MMU_MEM_INFO  MxMmuScanMem (MASTERTABLE hMt, MX_MMU_MEM_SCAN_HANDLE * pHandle)
{
    MX_MMU_MEM_INFO info, tmp;

    UINT32  nType;
    UINT32  hSect;
    UINT32  Base;

    _ASSERT_PTR(hMt);
    _ASSERT_PTR(pHandle);
    
    memset (&info, 0, sizeof (info));

    if (pHandle->Addr == 0xFFFFFFFF)
        return info;

    while (1)
    {
        Base = pHandle->Addr;
    
        hSect = hMt[pHandle->Addr >> 20];

        nType = (hSect & MMU_PAGE_ID_MASK);

        if (nType == MMU_FAULT_PAGE_ID)
        {
            if (info.Size != 0)
                break;
        
            pHandle->Addr += (1024*1024);
        }
        else if (nType == MMU_1M_PAGE_ID)
        {
            tmp.Addr       = pHandle->Addr;
            tmp.AP         = ((hSect >> 10) & 0x3) | (((hSect >> 15) & 1) << 2);
            tmp.Domain     = (hSect >> 5) & 0xF;
            tmp.Shared     = (hSect >> 16) & 1;
            tmp.XN         = (hSect >> 4) & 1;
            tmp.Size       = (nType == MMU_1M_PAGE_ID) ? 1024*1024 : 16*1024*1024;
            tmp.Tex        = (((hSect>>12)&7) << 2) | (((hSect>>3)&1) << 1) | (((hSect>>2)&1) << 0);

            if (info.Size == 0)
            {
                info = tmp;
            }
            else if (MmuIsMemInfoEq(&info, &tmp))
            {
                info.Size += tmp.Size;
            }
            else
            {
                break;
            }

            pHandle->Addr += tmp.Size;
        }
        else if (nType == MMU_COARSE_PAGE_ID)
        {
            tmp = MmuLoadPageMemInfo(hMt, pHandle->Addr);

            if (info.Size == 0)
            {
                info = tmp;
            }
            else if (MmuIsMemInfoEq(&info, &tmp))
            {
                info.Size += tmp.Size;
            }
            else
            {
                break;
            }

            pHandle->Addr += tmp.Size;
        }

        // just to check overflow condition
        //---------------------------------
        if (Base >= pHandle->Addr)
        {
            pHandle->Addr = 0xFFFFFFFF;
            break;
        }
    }

    return info;
}

#endif /*MX_ARM_MMU_ENABLED */


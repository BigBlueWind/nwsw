/** @file mmu.c
 *
 * @brief Memory Management Unit
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#if defined (USE_BARE) || defined (USE_RTXC)

#include "mmu.h"
#include "sysctrl.h"
#include "memlimits.h"
#include "apb.h"
#include "resultcodes.h"
#include <string.h>

/*
 * Definitions
 */
#define NUM_PTE_BUF 4

#define DOMAIN	0

//
// Translation Table
//
#pragma bssseg ("mmubss")
static U32 TtbFirstLevel[PGE_ENTRIES];
static U32 TtbSecondLevel[NUM_PTE_BUF][PTE_ENTRIES];
static U32 PteAllocBitmask;

static inline U32 FindFirstZero(U32 word)
{
    U32 k;

    word = ~word;
    k = 31;
    if (word & 0x0000ffff)
    {
        k -= 16;
        word <<= 16;
    }
    if (word & 0x00ff0000)
    {
        k -= 8;
        word <<= 8;
    }
    if (word & 0x0f000000)
    {
        k -= 4;
        word <<= 4;
    }
    if (word & 0x30000000)
    {
        k -= 2;
        word <<= 2;
    }
    if (word & 0x40000000)
    {
        k -= 1;
    }
    return k;
}

/*! \brief Lock translation table entry in I-TLB or D-TLB
 *         in order to avoid time consuming page table walk
 *
 *  \param addr
 *  \param tlb
 *  \param victim
 *  \return Standard result code
*/
RESULTCODE MmuLockDownTlb(U32 addr, U32 tlb, U32 victim)
{
    U32 type;
    V32 temp;
    U32 base = 0;

    type = MmuReadTlbType();
    DrainWriteBuffer();

    // Invalidate TLB entry and put it in preserve state
    if (type & TLBTYPE_U)
    {
        if (tlb & MMU_ITLB)
        {
            if (victim >= TLBTYPE_ILSIZE(type))
                return FAILURE;

            victim <<= (32 - FindFirstZero(TLBTYPE_ILSIZE(type)));

            MmuTlbICInvalidateEntry(addr);
            MmuTlbICLockdown(base | victim | TLBLOCK_P);
        }

        if (tlb & MMU_DTLB)
        {
            if (victim >= TLBTYPE_DLSIZE(type))
                return FAILURE;

            victim <<= (32 - FindFirstZero(TLBTYPE_DLSIZE(type)));

            MmuTlbDCInvalidateEntry(addr);
            MmuTlbDCLockdown(base | victim | TLBLOCK_P);
        }
    }
    else
    {
        if (victim >= TLBTYPE_DLSIZE(type))
            return FAILURE;

		victim <<= (32 - FindFirstZero(TLBTYPE_DLSIZE(type)));

		MmuTlbInvalidateEntry(addr);
		MmuTlbLockdown(base | victim | TLBLOCK_P);
    }

    // Do translation fault to fill TLB entry
    temp = *(V32*) addr;

    // Now when entry is put in TLB, clear preserve state bit
    if (type & TLBTYPE_U)
    {
        MmuTlbICLockdown(0);
        MmuTlbDCLockdown(0);
    }
    else
        MmuTlbLockdown(0);

    return SUCCESS;
}

/*! \brief
 *
 *  \param pgeOffset
 *  \param domain
 *  \param accessType
 *  \param cacheMode
 *  \return Standard result code
*/
RESULTCODE CreatePte(U32 pgeOffset, U32 domain, U32 accessType, U32 cacheMode)
{
    int i;
    U32 *pte;
    U32 *pge = &TtbFirstLevel[pgeOffset];
    U32 pteID;

    if ((PteAllocBitmask + 1) == (1 << NUM_PTE_BUF))
    {
        // NUM_PTE_BUF has to be increased
        return FAILURE;
    }
    pteID = FindFirstZero(PteAllocBitmask);

    pte = &TtbSecondLevel[pteID][0];
    if ((U32)pte & PGE_COARSE_MASK)
    {
        // pte has to be aligned to 1K boundary
        return FAILURE;
    }

    // Mark allocated table
    PteAllocBitmask |= 1 << pteID;

    // Setup coarse table entry
    *pge = (U32) pte | PGE_TYPE_COARSE | PGE_DOMAIN(domain);

    // Setup coarse page table
    for (i = 0; i < PTE_ENTRIES; i++, pte++)
    {
        *pte = PTE_TYPE_SMALL_PAGE;
        *pte |= (pgeOffset << PGE_ENTRY_SHIFT) | (i << PAGE_SHIFT_4K);
        *pte |= cacheMode  << PTE_CACHE_SHIFT;
        *pte |= accessType << PTE_AP0_SHIFT;
        *pte |= accessType << PTE_AP1_SHIFT;
        *pte |= accessType << PTE_AP2_SHIFT;
        *pte |= accessType << PTE_AP3_SHIFT;
    }

    return SUCCESS;
}

/*! \brief Destroy coarse table
 *
 *  \param pgeOffset
 *  \return Standard result code
*/
RESULTCODE DestroyPte(U32 pgeOffset)
{
    int pteID;
    U32 baseAddr;
    U32 *pge = &TtbFirstLevel[pgeOffset];

    baseAddr = *pge & PGE_COARSE_BASE_MASK;
    for (pteID = 0; pteID < NUM_PTE_BUF; pteID++)
    {
        // TODO: Looks like this is incorrect
        if ((U32)&TtbSecondLevel[pteID][0] == baseAddr)
        {
            PteAllocBitmask &= ~(1 << pteID);
            return SUCCESS;
        }
    }
    return FAILURE;
}

/*! \brief Setup translation table entries
 *
 *  \param startAddr
 *  \param size
 *  \param accessType
 *  \param cacheMode
 *  \return No return value
*/
RESULTCODE MmuMap(U64 startAddr, U64 size, U32 accessType, U32 cacheMode)
{
    U32 i;
    U32 pgeType;
    U32 cache;
    U32 access;
    U32 domain;
    U64 lastAddr;
    U32 pgeOffset;
    U32 lastPge;
    U32 *pge;
    U32 *pte;
    U32 *pteTemp;
    S32 offset;
    U32 subpages;
    U32 shift;

    // Setup and round up last address to a page size
    lastAddr = ((startAddr) + size - 1);
    lastAddr = ((lastAddr) + PAGE_SIZE_1K - 1) & ~PAGE_MASK_1K;

    // Round to a page size
    startAddr &= ~PAGE_MASK_1K;

    // Check boundaries
    if ((size == 0) || startAddr > lastAddr)
        return FAILURE;

    lastPge = (U32)(lastAddr >> PGE_ENTRY_SHIFT);
    while (startAddr < lastAddr)
    {
        pgeOffset = (U32)startAddr >> PGE_ENTRY_SHIFT;

        // Size now in a number of pge entries
        size = (lastPge - pgeOffset);

        pge = &TtbFirstLevel[pgeOffset];
        pgeType = (*pge & PGE_TYPE_MASK);

        if (pgeType == PGE_TYPE_SECTION)
        {
            // Check that startAddr and size aligned to 1M
            if ((startAddr & PGE_SECTION_MASK) || (((lastAddr + 1)
                & PGE_SECTION_MASK) && (size == 0)))
            {
                // Extract pge attributes
                cache = (*pge & PGE_CACHE_MASK) >> PGE_CACHE_SHIFT;
                access = (*pge & PGE_AP_MASK) >> PGE_AP_SHIFT;
                domain = (*pge & PGE_DOMAIN_MASK) >> PGE_DOMAIN_SHIFT;

                // Create coarse table with pge attributes
                if (CreatePte(pgeOffset, domain, access, cache) != SUCCESS)
                    return FAILURE;

                continue;
            }

            // Set section attributes
            *pge = PGE_TYPE_SECTION | PGE_DOMAIN(DOMAIN);
            *pge |= startAddr | (cacheMode << 2) | (accessType << 10);

            // Adjust startAddr to a section size
            startAddr += PGE_ENTRY_SIZE;
        }
        else if (pgeType == PGE_TYPE_SUPERSECTION)
        {
            // Check that startAddr and size aligned to 16M
            if ((startAddr & PGE_SUPER_MASK) || (((lastAddr + 1)
                & PGE_SUPER_MASK) && (!(size & 0x1FF0))))
            {
                // Break supersection to 16 section entries
                pgeOffset &= ~0xF;
                pge = &TtbFirstLevel[pgeOffset];
                for (i = 0; i < 16; i++, pgeOffset++)
                {
                    // TODO: Looks like this is a bug
                    *pge &= ~0x00040000;
                    *pge |= (*pge & PGE_SECTION_MASK) | (pgeOffset << PGE_ENTRY_SHIFT);
                    pge++;
                }
                continue;
            }

            // Process supersection as PGE_TYPE_FAULT
            *pge = PGE_TYPE_FAULT;
            continue;
        }
        else if (pgeType == PGE_TYPE_COARSE)
        {
            offset = (U32)(startAddr & 0xFFC00);
            pte = (U32*) ((int)TtbFirstLevel[pgeOffset] & PGE_COARSE_BASE_MASK);
            subpages = 0;

            while (offset)
            {
                if ( ((*pte) & PTE_TYPE_MASK) == PTE_TYPE_LARGE_PAGE)
                {
                    if (offset < PAGE_SIZE_64K)
                    {
                        pteTemp = pte;
                        cache = (*pteTemp & PTE_CACHE_MASK) >> PTE_CACHE_SHIFT;
                        access = (*pteTemp & PTE_AP0_MASK) >> PTE_AP0_SHIFT;

                        for (i = 0; i < 16; i++)
                        {
                            *pteTemp = PTE_TYPE_SMALL_PAGE;
                            *pteTemp |= startAddr & ~PAGE_MASK_4K;
                            *pteTemp |= cache << PTE_CACHE_SHIFT;
                            *pteTemp |= access << PTE_AP0_SHIFT;
                            *pteTemp |= access << PTE_AP1_SHIFT;
                            *pteTemp |= access << PTE_AP2_SHIFT;
                            *pteTemp |= access << PTE_AP3_SHIFT;
                            pteTemp++;
                        }
                        continue;
                    }
                    offset -= PAGE_SIZE_64K;
                    pte += 16;
                }
                else if (((*pte) & PTE_TYPE_MASK) == PTE_TYPE_SMALL_PAGE)
                {
                    if (offset < PAGE_SIZE_4K)
                    {
                        subpages++;
                        offset -= PAGE_SIZE_1K;
                    }
                    else
                    {
                        offset -= PAGE_SIZE_4K;
                        pte ++;
                    }
                }
                else
                    return FAILURE;
            }

            offset = (pgeOffset < lastPge) ? (U32)(PGE_ENTRY_SIZE
                - (U32)(startAddr & 0xFFC00)) : (U32)(lastAddr - startAddr);

            if (subpages)
            {
                //Overwrite cache mode
                *pte &= ~PTE_CACHE_MASK;
                *pte |= cacheMode << PTE_CACHE_SHIFT;

                for (; offset && subpages != 4; subpages++)
                {
                    shift = subpages << 1;
                    *pte &= ~(PTE_AP0_MASK << shift);
                    *pte |= accessType << (shift + PTE_AP0_SHIFT);
                    offset -= PAGE_SIZE_1K;
                    startAddr += PAGE_SIZE_1K;
                }
                pte++;
            }

            while (offset > 0)
            {
                if (offset >= PAGE_SIZE_64K && !(startAddr & PAGE_MASK_64K))
                {
                    for (i = 0; i < 16; i++)
                    {
                        *pte = PTE_TYPE_LARGE_PAGE;
                        *pte |= startAddr & ~PAGE_MASK_64K;
                        *pte |= cacheMode << PTE_CACHE_SHIFT;
                        *pte |= accessType << PTE_AP0_SHIFT;
                        *pte |= accessType << PTE_AP1_SHIFT;
                        *pte |= accessType << PTE_AP2_SHIFT;
                        *pte |= accessType << PTE_AP3_SHIFT;
                        pte++;
                    }
                    offset -= PAGE_SIZE_64K;
                    startAddr += PAGE_SIZE_64K;
                }
                else if (offset >= PAGE_SIZE_1K && !(startAddr & PAGE_MASK_4K))
                {
                    *pte = PTE_TYPE_SMALL_PAGE;
                    *pte |= startAddr & ~PAGE_MASK_4K;
                    *pte |= cacheMode << PTE_CACHE_SHIFT;
                    *pte |= accessType << PTE_AP0_SHIFT;
                    *pte |= accessType << PTE_AP1_SHIFT;
                    *pte |= accessType << PTE_AP2_SHIFT;
                    *pte |= accessType << PTE_AP3_SHIFT;
                    pte++;

                    offset -= PAGE_SIZE_4K;
                    startAddr += PAGE_SIZE_4K;
                }
                else
                    return FAILURE;
            }

            // startAddr got aligned to section bounds
            if (((pgeOffset + 1) << PGE_ENTRY_SHIFT) >= startAddr)
                startAddr = (pgeOffset + 1) << PGE_ENTRY_SHIFT;
            else
                startAddr = lastAddr;
        }
        else if (pgeType == PGE_TYPE_FAULT)
        {
            if ((startAddr & PGE_SECTION_MASK) || (((lastAddr + 1)
                & PGE_SECTION_MASK) && (size == 0)))
            {
                // Create generic coarse table with no access attribute
                if (CreatePte(pgeOffset, DOMAIN, MMU_NO_ACCESS,
                                MMU_CACHE_OFF) != SUCCESS)
                    return FAILURE;

                /* setup coarse table */
                continue;
            }

            /* check supersection support */
            if (!(startAddr & PGE_SUPER_MASK) && (ID_ARCH(SysReadCpuid()) >= ID_ARCH_6))
            {
                // Adjust to supersection size
                for (i = 0; i < (size & 0x1FF0); i++, pgeOffset++, pge++)
                {
					// Destory any coarse entries
                    pgeType = *pge & PGE_TYPE_MASK;
                    if (pgeType == PGE_TYPE_COARSE)
                        DestroyPte(pgeOffset);

					// Set supersection attributes
                    *pge = (U32)(startAddr & PGE_SUPER_BASE_MASK)
                        | (cacheMode << PGE_CACHE_SHIFT);
                    *pge |= PGE_TYPE_SUPERSECTION | (accessType
                        << PGE_AP_SHIFT);

                    startAddr += PGE_ENTRY_SIZE;
                }

                /* cut size to 15Meg */
                size &= 0x0F;
            }

            /* setup on a section base */
            for (i = 0; i < size; i++, pgeOffset++, pge++)
            {
                /* destory any coarse entries */
                pgeType = *pge & PGE_TYPE_MASK;
                if (pgeType == PGE_TYPE_COARSE)
                    DestroyPte(pgeOffset);

                // Set section attributes
                *pge = (U32)(startAddr & PGE_SECTION_BASE_MASK)
                    | (cacheMode << PGE_CACHE_SHIFT);
                *pge |= PGE_TYPE_SECTION | (accessType
                    << PGE_AP_SHIFT) | PGE_DOMAIN(DOMAIN);

                startAddr += PGE_ENTRY_SIZE;
            }
        }
        else if (pgeType == PGE_TYPE_RESERVED)
        {
            // Oops... that's something never expected
            return FAILURE;
        }
    }

    return SUCCESS;
}

/*! \brief Initialise MMU
 *
 *  \param cpuid CPU identifier (Upper ARM, Lower ARM)
 *  \return Standard result code
*/
RESULTCODE MmuInit(U32 cpuid)
{
	// TTB must be 16K aligned
	if ((U32)TtbFirstLevel & 0x3FFF)
		return FAILURE;

	// Disable MMU, D-Cache and Extended Page Tables
	SysWriteControl(SysReadControl() & ~(CR_M | CR_A | CR_C | CR_XP | CR_S | CR_R));

	// Invalidate and clean I-Cache and D-Cache
	CacheInvalidate();
	CacheDCClean();
	DrainWriteBuffer();

	// Create translation table
	if (cpuid == UARM)
	{
        int i, j;

        for (i = 0; i < PGE_ENTRIES; i++)
            TtbFirstLevel[i] = 0;

        for (i = 0; i < NUM_PTE_BUF; i++)
        {
            for (j = 0; j < PTE_ENTRIES; j++)
                TtbSecondLevel[i][j] = 0;
        }

        MmuMap(SDRAM_BASE, SDRAM_SIZE, MMU_ALL_ACCESS, MMU_CACHE_CB);
        // TODO: How about high interrupt vectors?
        MmuMap(SDRAM_SIZE, 0x100000000ULL-SDRAM_SIZE, MMU_ALL_ACCESS, MMU_CACHE_OFF);
#ifdef M823XX
	    MmuMap(IRAM_BASE, IRAM_SIZE, MMU_ALL_ACCESS, MMU_CACHE_OFF);
	    MmuMap(ARAM_BASE, ARAM_SIZE, MMU_ALL_ACCESS, MMU_CACHE_CB);
#endif /*M823XX*/
	}

	// Invalidate TLB. Handle unified TLB (ARM1136)
    if (MmuReadTlbType() & TLBTYPE_U)
    {
	    MmuTlbICInvalidate();
	    MmuTlbDCInvalidate();
    }
    else
        MmuTlbInvalidate();

	// Write Translation Table Base and Domain Access Control
	MmuWriteTTControl(TTBCR_TTB0_16K);
	MmuWriteTtb0((U32)TtbFirstLevel);

	// Domain #0 used on ARM11 (to support supersections)
	MmuWriteDomainAccess(DACR_MANAGER << DACR_Dx_SHIFT(0));

	//  Enable MMU
	SysWriteControl(SysReadControl() | CR_M);

	// Process TLB lockdown regions.
	// Note that ARM1136 capable to lockdown 8 unified entries
    {
        U32 index = 0;
        MmuLockDownTlb(SDRAM_BASE, MMU_ITLB | MMU_DTLB, index++);// SDRAM (16M)
        MmuLockDownTlb(APB_BASE, MMU_DTLB, index++);             // APB
#ifdef M823XX
        MmuLockDownTlb(ERAM_BASE, MMU_DTLB, index++);            // ERAM
        MmuLockDownTlb(IRAM_BASE, MMU_DTLB, index++);            // ARAM
#endif /*M823XX*/
    }

	return SUCCESS;
}

#endif /* USE_BARE || USE_RTXC */

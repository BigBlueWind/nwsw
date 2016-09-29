/** @file mmu.h
 *
 * @brief Memory Management Unit interfaces and definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef MMU_H_
#define MMU_H_

#include "basetypes.h"
#include "resultcodes.h"

//
// First Level Descriptor Attributes
//
#define PGE_TYPE_FAULT          0x00000000
#define PGE_TYPE_COARSE         0x00000001
#define PGE_TYPE_SECTION        0x00000002
#define PGE_TYPE_SUPERSECTION   0x00040002      // ARM11 only
#define PGE_TYPE_RESERVED       0x00000003      // ARM11 only
#define PGE_TYPE_MASK           0x00040003
#define PGE_DOMAIN_MASK         0x000001E0
#define PGE_DOMAIN_SHIFT        5
#define PGE_DOMAIN(x)           (((x) << 5) & PGE_DOMAIN_MASK)
#define PGE_AP_MASK             0x00000C00
#define PGE_AP_SHIFT            10
#define PGE_CACHE_MASK          0x0000000C
#define PGE_CACHE_SHIFT         2

#define PGE_SECTION_BASE_MASK   0xFFF00000
#define PGE_SECTION_MASK        0x000FFFFF
#define PGE_SUPER_BASE_MASK     0xFF000000
#define PGE_SUPER_MASK          0x00FFFFFF
#define PGE_COARSE_BASE_MASK    0xFFFFFC00
#define PGE_COARSE_MASK         0x000003FF

//
// First Level Descriptor Base Address
//
#define PGE_BASE_MASK(type)     (~(1 << PG_BASE_SHIFT(type)) + 1)

#define PGE_BASE_SHIFT(type) \
    ((((type) & PG_TYPE_MASK) == PG_TYPE_SECTION) ? 20 : \
    (((type) & PG_TYPE_MASK) == PG_TYPE_COARSE)  ? 10 : \
    (((type) & PG_TYPE_MASK) == PG_TYPE_FINE)    ? 12 : \
    (((type) & PG_TYPE_MASK) == PG_TYPE_SUPERSECTION) ? 24 : 31)

//
// Number of entries in First Level Table
//
#define PGE_ENTRIES             4096
#define PGE_PAGE_SIZE           (4*PGE_ENTRIES)
#define PGE_ENTRY_SHIFT         20
#define PGE_ENTRY_SIZE          (1 << PGE_ENTRY_SHIFT)

//
// Second Level Descriptor Attributes
//
#define PTE_TYPE_FAULT          0x00000000
#define PTE_TYPE_LARGE_PAGE     0x00000001
#define PTE_TYPE_SMALL_PAGE     0x00000002
#define PTE_TYPE_TINY_PAGE      0x00000003      // ARM920T
#define PTE_TYPE_MASK           0x00000003
#define PTE_CACHE_SHIFT         2
#define PTE_AP0_SHIFT           4
#define PTE_AP1_SHIFT           6
#define PTE_AP2_SHIFT           8
#define PTE_AP3_SHIFT           10

#define PTE_CACHE_MASK          0x0000000C
#define PTE_AP0_MASK            0x00000030
#define PTE_AP1_MASK            0x000000C0
#define PTE_AP2_MASK            0x00000300
#define PTE_AP3_MASK            0x00000C00

//
// Second Level Descriptor Base Address
//
#define PTE_BASE_MASK(type)     (~(1 << PG_BASE_SHIFT(type)) + 1)

#define PTE_BASE_SHIFT(type) \
    ((((type) & PG_TYPE_MASK) == PTE_TYPE_LARGE_PAGE) ? 16 : \
    (((type) & PG_TYPE_MASK) == PTE_TYPE_SMALL_PAGE) ? 12 : \
    (((type) & PG_TYPE_MASK) == PTE_TYPE_TINY_PAGE)  ? 10 : 31)

//
// Number of entries in Second Level Table
//
#define PTE_ENTRIES             256
#define PTE_ENTRY_SHIFT         12
#define PTE_ENTRY_SIZE          (1 << PTE_ENTRY_SHIFT)
#define PTE_PAGE_SIZE           (4*PTE_ENTRIES)

//
// Minimal MMU unit size (based on small page)
//
#define PAGE_SIZE_4K            4096
#define PAGE_SHIFT_4K           12
#define PAGE_MASK_4K            0xFFF

#define PAGE_SIZE_64K           (1024 * 64)
#define PAGE_MASK_64K           0xFFFF
#define PAGE_SIZE_1K            1024
#define PAGE_MASK_1K            0x3FF

// MmuMap access type values
#define MMU_NO_ACCESS           0x00
#define MMU_SVC_RW              0x01
#define MMU_USR_RO              0x02
#define MMU_ALL_ACCESS          0x03

// MmuMap cache mode values
#define MMU_CACHE_OFF           0x00
#define MMU_CACHE_B             0x01
#define MMU_CACHE_C             0x02
#define MMU_CACHE_CB            0x03

// MmuLockDownTlb tlb values
#define MMU_DTLB                0x01
#define MMU_ITLB                0x02

//
// MMU Interface functions
//
RESULTCODE MmuInit(U32 cpuid);

#endif /*MMU_H_ */

//-------------------------------------------------------------------------------------------
/** @file 4gmx_arm_mmu.h
 *
 * @brief MMU API definition
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_ARM_MMU_H_
#define _4GMX_ARM_MMU_H_

#include "4gmx_config.h"
#include "4gmx_types.h"
#include "4gmx_const.h"
#include "4gmx_arm_types.h"
#include "4gmx_kernel.h"
#include "4gmx_arm_cp15.h"

#ifdef MX_ARM_L2_CACHE_ENABLED
#include "4gmx_l2cache.h"
#endif

#ifdef MX_ARM_MMU_ENABLED

#ifdef __cplusplus
extern "C"
{
#endif

#define MX_ARM_MMU_OK               MX_OK
#define MX_ARM_ISR_ERROR            DEF_MXRC(MXM_ARM, 1)
#define MX_ARM_MMU_ISIZE            DEF_MXRC(MXM_ARM, 2)
#define MX_ARM_MMU_DSIZE            DEF_MXRC(MXM_ARM, 3)
#define MX_ARM_MMU_UNSUPPORTED      DEF_MXRC(MXM_ARM, 4)
#define MX_ARM_MMU_NO_FREE_PAGES    DEF_MXRC(MXM_ARM, 5)
#define MX_ARM_MMU_NO_MASTER_TABLE  DEF_MXRC(MXM_ARM, 6)
#define MX_ARM_MMU_UNSUPPORTED_PAGE DEF_MXRC(MXM_ARM, 7)

#define MMU_CACHE_ENABLED          (1 << 3)
#define MMU_WRITEBUFF_ENABLED      (1 << 2)

// =======================================================
//       MMU section configuration parameters (L1 table)
// =======================================================
#define MMU_MASTER_DOMAIN(x)       ((x) << 5 )
#define MMU_MASTER_AP(x)           ((x) << 10)
#define MMU_MASTER_TEX(x)          ((x) << 12)
#define MMU_MASTER_SHARED           (1 << 16)
#define MMU_MASTER_NGLOBAL          (1 << 17)
#define MMU_MASTER_NS               (1 << 19)
#define MMU_MASTER_NX               (1 <<  4)
#define MMU_CREATE_MASTER_ADDR(x)  ((UINT32)(x) << 20 )
#define MMU_GET_MASTER_ADDR(x)     (((UINT32)(x) >> 20) & 0xFFF) /**< 12 bits for address 31:20*/

// =======================================================
//       MMU 64/4 page configuration parameters (L2 table)
// =======================================================
#define MMU_PAGE_AP(x)              ((x) << 4)
#define MMU_PAGE64_TEX(x)           ((x) << 12)
#define MMU_PAGE4_TEX(x)            ((x) << 6)
#define MMU_PAGE_SHARED             (1 << 10)
#define MMU_PAGE_NGLOBAL            (1 << 11)
#define MMU_PAGE64_NX               (1 << 15)
#define MMU_PAGE4_NX                (1 << 0)


/** @brief MxMmuActivate(...) constants */

#define MMU_CTRL_L1_ENABLED         (1 << 0)
#define MMU_CTRL_L2_ENABLED         (1 << 1)
#define MMU_CTRL_ICACHE_ENABLED     (1 << 2)
#define MMU_CTRL_DCACHE_ENABLED     (1 << 3)

/**************************************************************
*                 The MMU section types                       *
**************************************************************/
#define MMU_SECTION_TYPE_FAULT          0   // Translation fault, the 1M range cannot be accessed
#define MMU_SECTION_TYPE_PAGED_64       1   // The 1MB section is devived to 64 KB pages 
#define MMU_SECTION_TYPE_PAGED_4        2   // The 1MB section is devived to 4 KB pages 
#define MMU_SECTION_TYPE_1M_SECTION     3   // This is 1MB section
#define MMU_SECTION_TYPE_16M_SECTION    4   // This is 16MB section

#define MMU_L2_TABLE_COARSE_INDEX_NUM   256

typedef enum _MMU_AP_
{
    AP_NO_ACCESS                = (0x0),        // MGR/USER: All accesses generate Permission faults
    AP_MGR_READ_WRITE           = (0x1),        // MGR: read/write,  user: not access
    AP_MGR_FULL_USER_READ_ONLY  = (0x2),        // MGR: read/write,  user: read only
    AP_FULL_ACCESS              = (0x3),        // MGR: read/write,  user: read/write
    AP_MGR_READ_ONLY            = ((1 << 5) | (1 << 0)),	// MGR: read,  user: not access
    AP_MGR_USER_READ_ONLY       = ((1 << 5) | (3 << 0)),	// MGR: read,  user: read
    
} MMU_AP;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TEX[2:0]    C   B       Description         Memory type         Page Shareable
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    000     0   0       Strongly-ordered    Strongly-ordered    Shareable
    000     0   1       Shareable Device    Device              Shareable
    000     1   0       Outer and Inner 
                        Write-Through, no   Normal              S bita
                        Write-Allocate
    000     1   1       Outer and Inner 
                        Write-Back, no 
                        Write-Allocate      Normal              S bita
    001     0   0       Outer and Inner 
                        Non-cacheable       Normal              S bita
    001     0   1       Reserved            -                   -
    001     1   0       IMPLEMENTATION      DEFINED             IMPLEMENTATION
    001     1   1       Outer and Inner     Normal              S bita
                        Write-Back, 
                        Write-Allocate  
    010     0   0       Non-shareable       Device              Non-shareable
    010     0   1       Reserved            -                   -
    010     1   X       Reserved            -                   -
    011     X   X       Reserved            -                   -
    1BB     A   A       Cacheable memory:   Normal              S bita
                        AA = Inner attributeb
                        BB = Outer attribute
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef enum _MMU_TEX_C_B_
{
    TEXCB_STRONGLY_ORDERED = (0),
    TEXCB_SHARED_DEV       = (MMU_WRITEBUFF_ENABLED),

    TEXCB_MASTER_OI_WB_WA   = (MMU_MASTER_TEX(1) | MMU_CACHE_ENABLED | MMU_WRITEBUFF_ENABLED),				// Outer and Inner Write-Back, Write-Allocate Normal S-NON-SHARED
    TEXCB_MASTER_OI_WB_WA_S = (MMU_MASTER_TEX(1) | MMU_CACHE_ENABLED | MMU_WRITEBUFF_ENABLED | MMU_MASTER_SHARED),	// Outer and Inner Write-Back, Write-Allocate Normal S-SHARED

    TEXCB_PAGE64_OI_WB_WA_S = (MMU_PAGE64_TEX(1) | MMU_CACHE_ENABLED | MMU_WRITEBUFF_ENABLED | MMU_PAGE_SHARED), // Outer and Inner Write-Back, Write-Allocate Normal S-SHARED
    TEXCB_PAGE64_OI_WB_WA   = (MMU_PAGE64_TEX(1) | MMU_CACHE_ENABLED | MMU_WRITEBUFF_ENABLED),                   // Outer and Inner Write-Back, Write-Allocate Normal 
    TEXCB_PAGE4_OI_WB_WA_S  = (MMU_PAGE4_TEX(1)  | MMU_CACHE_ENABLED | MMU_WRITEBUFF_ENABLED | MMU_PAGE_SHARED), // Outer and Inner Write-Back, Write-Allocate Normal S-SHARED
    TEXCB_PAGE4_OI_WB_WA    = (MMU_PAGE4_TEX(1)  | MMU_CACHE_ENABLED | MMU_WRITEBUFF_ENABLED),                   // Outer and Inner Write-Back, Write-Allocate Normal 

} MMU_TEX_C_B;

typedef enum _MMU_DOMAIN_
{
    MMU_DOMAIN_MANAGER    = (0x3),
    MMU_DOMAIN_RES        = (0x2),
    MMU_DOMAIN_CLIENT     = (0x1),
    MMU_DOMAIN_NOACCESS   = (0x0)
} MMU_DOMAIN;

typedef enum _MMU_PAGE_ID_
{
    MMU_COARSE_PAGE_ID   = 0x1,
    MMU_1M_PAGE_ID       = 0x2,
    MMU_16M_PAGE_ID      = (2 | 1 << 18),
    MMU_FAULT_PAGE_ID    = 0x0,

    MMU_COARSE_PAGE_64   = 0x1,
    MMU_COARSE_PAGE_4    = 0x2,
    
    MMU_PAGE_ID_MASK     = 0x3
} MMU_PAGE_ID;

typedef struct _MMUSECTION_BITMASK_
{
    unsigned int id       :2;
    unsigned int wrbuff   :1;
    unsigned int cache    :1;
    unsigned int alw_0    :1;
    unsigned int domain   :4;
    unsigned int p        :1;
    unsigned int ap       :2;
    unsigned int tex      :3;     /**< extension field encoding  */
    unsigned int sbz_1    :3;
    unsigned int supersect:1;   /**< 1 - 16MB sections    */
    unsigned int sbz_2    :1;
    unsigned int baseaddr :12;
} MMUSECTIONBITMASK, *PMMUSECTIONBITMASK;

typedef struct _MXMMUTEX_
{
    UINT32  b   : 1;
    UINT32  c   : 1;
    UINT32  tex : 3;
    UINT32  res : 27;

}MXMMUTEX, *PMXMMUTEX;

typedef PUINT32 MASTERTABLE;
typedef PUINT32 MMUSECTION;
typedef PUINT32 MMUPAGE;

typedef struct _MX_MMU_MEM_INFO_
{
    UINT32      Addr;   // The address of block
    UINT32      Size;   // The size in bytes
    UINT32      AP;     // Access permission
    UINT32      Domain; // The domain ID
    UINT32      Shared; // Shared bit
    UINT32      XN;     // Non-execute bit
    UINT32      Tex;    // Cache type

}MX_MMU_MEM_INFO, *PMX_MMU_MEM_INFO;

typedef struct _MX_MMU_MEM_SCAN_HANDLE_
{
    UINT32  Addr;
}MX_MMU_MEM_SCAN_HANDLE;

/************************************************************************/
/*     API definition                                                   */
/************************************************************************/

/** @brief  The internal function of 4GMX system that used for base initialization of
   sub-ARM MMU system. This function is called automatically by the 4GMX kernel.

 @param pCtx [in] - the pointer to the 4GMX context
 @param nOpt [in] - the initialization options (see MXINIT.ConfigSet)

 @return [MXRC] error code */

MXRC MxMmuSubSysInit(PMXCONTEXT pCtx, UINT32 nOpt);

/** @brief  The function inits (creates ) a master table with default
   parameters specified by <ctrl> flags.

 @param pStorage [out] - the pointer to memory where master table will be allocated
 @param PageID [in] - the ID of PTE, see MMU_PAGE_ID type
 @param ctrl  [in] - the default parameters of PTE (page table entry)

 Warning! The <pStorage> should be alignment on 16 KB

 @return [MASTERTABLE] the descriptor of master table */

MASTERTABLE MxMmuCreateMasterTable(LPVOID pStorage, MMU_PAGE_ID PageID,
                                   UINT32 ctrl);

/** @brief The function changes/sets parameter of sections (1MBytes region )

 @NOTE  The function should be called before MxSetTTB function

 @param pMTable  [in] - the descriptor of master table
 @param BaseMem [in]  - the address of virtual page
 @param MapTo    [in]  -  the physical address, the virtual address is mapped to
 @param nSectCount[in] - the number of section (1MBytes size blocks)
 @param ctrl  [in] - the section's attributes

 @return [MXRC] error code */

MXRC MxMmuSetSectParam(MASTERTABLE pMTable, UINT32 BaseMem, UINT32 MapTo,
                       UINT32 nSectCount, UINT32 ctrl);

/** @brief The function sets domain access flags
 @param nDomainID [in] - the domain ID [0..15]
 @param nAccess   [in] - the access flags: see MMU_DOMAIN enum
 @return [MXRC] error code */

MXRC MxMmuSetDomain(UINT32 nDomainID, UINT32 nAccess);

MXRC MxMmuSetTTB(MASTERTABLE pMTable, UINT32 nTTBOpt);

/** @brief The function activates/deactivates MMU
 @param nCtrl [in] - enable/disable an MMU, see: MMU_CTRL_xxx macros in this file
 @return [MXRC] */

MXRC MxMmuCtrl(UINT32 nCtrl);

/** @brief This function cleans and invalidates ARM D-CACHE
 * */
void MxFlushDCache (void);
void MxInvalidDCache (void);
void MxFlushIDCache (void);
void MxDisableDCache(void);

/** @brief This function returns handler of MMU table for the current processor 
    @return [MASTERTABLE] MMU table handler */

MASTERTABLE MxMmuGetMasterTable (void);

/** @brief This function returns handle of MMU section (1MB descriptor)
    @param hTable[in] - the MMU master table handle
    @param nAddr [in] - the address of memory where section is located 
    @return [MMUSECTION] section handle */
    
MMUSECTION MxMmuGetSection (MASTERTABLE hTable, UINT32 nAddr);

/** @brief This function returns handle of MMU page (64/4 KB descriptor)
    @param hTable[in] - the MMU master table handle
    @param nAddr [in] - the address of memory where page is located 
    @return [MMUPAGE] page handle */
    
MMUPAGE MxMmuGetPage (MASTERTABLE hTable, UINT32 nAddr);

/** @brief This function sets type of 1MB section

    @param hSect    [in] - the section handle , see: MxMmuGetSection(...)
    @param nType    [in] - the type of section, see MMU_SECTION_TYPE_xxx
    @param nSOptions[in] - additional section options, the data depends on the section type
    @param nPOptions[in] - additional page options, it's used in case of page64 and page4

    @return [MXRC] an error code */

MXRC MxMmuSetSectionType (MMUSECTION hSect, UINT32 nType, UINT32 nSOptions, UINT32 nPOptions);

/** @brief This function sets page parameter (64/4KB page)

    @param hPage [in] - the page handler (see: MxMmuGetPage)
    @param MapTo [in] - mapping to physical address
    @param nOpt  [in] - the page options, see: page specific options

    @return [MXRC] an error code */

MXRC MxMmuSetPageParam (MMUPAGE hPage, UINT32 MapTo, UINT32 nOpt);

/** @brief This function is designed to convert virtual to physical address 
            according to the mapping table 

    @param nAddr [in] - the virtual address 
    
    @return [UINT32] physical address */

UINT32 MxMmuVToP (UINT32 nAddr);

/** @brief This function checks DCache flag for the specific memory 

    @param nAddr [in] - the address 

    @return [BOOL] 1 or 0 */

BOOL MxMmuIsDCached (UINT32 nAddr);

/** @brief This function retuns DCache parameters for the specific page
    @param nAddr [in] - the address
    @return [UINT32] bit[3:2] - outer cache
                     bit[6:4] - inner cache 

    \ingroup group_lte_4gmx_kernel_mmu

    */

BOOL MxMmuGetDCacheParam (UINT32 nAddr);

/** @brief This function returns type of the region 1M/64KB/4KB

    @param nAddr [in] - the address of the region

    @return [UINT32] ID: MMU_SECTION_TYPE_xxx */

UINT32 MxMmuGetRegionMapType (UINT32 nAddr);

/** @brief This function is designed to set access permission (AP)
           to the selected memory region (1M/64K/4K)

    @param nAddr [in] - the address of memory region
    @param nAP   [in] - the access permission, see: enum MMU_AP

    @return [MXRC] an error code */

MXRC MxMmuSetMemAP (UINT32 nAddr, UINT32 nAP);

/** @brief This function is designed to set non-exec bit (XN)
           to the selected memory region (1M/64K/4K)

    @param nAddr [in] - the address of memory region
    @param nXN   [in] - the flag of XN bit

    @return [MXRC] an error code

    \ingroup group_lte_4gmx_kernel_mmu

    */

MXRC MxMmuSetMemXN (UINT32 nAddr, UINT32 nXN);

/** @brief This function is designed to set/change cache parameters for the 
           specified memory region 

    @param nAddr     [in] - the address of memory region
    @param nSize     [in] - the size of memory in bytes 
    @param Cache     [in] - the cache parameters, see: type MXMMUTEX and TEX description

    @return [MXRC] an error code

    \ingroup group_lte_4gmx_kernel_mmu

    */

MXRC MxMmuSetMemCacheCtrl (UINT32 nAddr, UINT32 nSize, MXMMUTEX Cache);

/** @brief This function checks is MMU enabled or not 

    @return [BOOL] flag */

BOOL MxMmuIsEnabled(void);

/** @brief This function checks is D-Cache enabled or not 

    @return [BOOL] flag */

BOOL MxMmuIsDCache(void);

/** @brief This function checks is D-Cache enabled or not 

    @return [BOOL] flag */

BOOL MxMmuIsICache(void);

/** @brief This function checks is FlowPrediction enabled or not 

    @return [BOOL] flag */

BOOL MxMmuIsIFlowPred(void);

/** @brief This function is designed to scan memory defined with MMU table and 
           to return information to the application about memory regions 

    @param hMt      [in]        - the handle of MMU master (1MB) table
    @param pHandle  [in/out]    - the handle used by the system

    @return [MX_MMU_MEM_INFO] information about memory region */
           
MX_MMU_MEM_INFO  MxMmuScanMem (MASTERTABLE hMt, MX_MMU_MEM_SCAN_HANDLE * pHandle);

#ifdef __cplusplus
}
#endif

#endif /*MX_ARM_ENABLED */
#endif /*_4GMX_ARM_H_*/

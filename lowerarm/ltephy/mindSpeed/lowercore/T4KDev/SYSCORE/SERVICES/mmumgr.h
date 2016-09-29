//------------------------------------------------------------------------------
/** @file DOX_INTERNAL mmumgr.h
 *
 * @brief  MMU manager: MMU configuration, setting/resetting access rights
 * @author Mindspeed Technologies
 * @version $Revision: 1.0 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//------------------------------------------------------------------------------

#ifndef _MMU_MGR_H_
#define _MMU_MGR_H_

#include "4gmx.h"
#include "config.h"
#include "appids.h"

typedef MXRC (*MmuCfg)(MASTERTABLE hTable);

MXRC MMUInit (UINT32 nCpuID, MmuCfg mmu_cfg);
UINT32 IsMMUMsgEnabled (void);

/** @brief This function is designed to be called at initialization step of system start-up
           to configure MMU table and mainly used by the system drivers to allow accesing 
           HW registers memory range

    @param Base     [in] - the base address of the region
    @param nSize    [in] - the size of the region in bytes
    @param nAP      [in] - the access permission for the list of pages specified by [BASE-SIZE] see: 4GMX enum <MMU_AP>
    @param nDefAP   [in] - the AP for other pages but only if 1MB section was not divided to pages, if was divided, it's ignored

    @return [UINT32] an error code */
    
MXRC MmuMgrCfgMemAP(UINT32 Base, UINT32 nSize, UINT32 nAP, UINT32 nDefAP);

/** @brief This function is designed to be called at initialization step of system start-up
           to configure MMU table and mainly used by the system drivers to allow accesing 
           HW registers memory range

    @param Base     [in] - the base address of the region
    @param nSize    [in] - the size of the region in bytes
    @param nCode    [in] - to mark region like executable
    @param nCache   [in] - the flag of cacheable memory
    @param nShare   [in] - the flag of sharable memory (only if memory is cached)
    @param nAP      [in] - the access permission for the list of pages specified by [BASE-SIZE] see: 4GMX enum <MMU_AP>
    @param nDefAP   [in] - the AP for other pages but only if 1MB section was not divided to pages, if was divided, it's ignored

    @return [UINT32] an error code */
    
MXRC MmuMgrCfgMemCacheShareAP(UINT32 Base, UINT32 nSize, UINT32 nCode, UINT32 nCache, UINT32 nShare, UINT32 nAP, UINT32 nDefAP);

#endif // _MMU_MGR_H_


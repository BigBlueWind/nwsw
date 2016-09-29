//------------------------------------------------------------------------------
/** @file DOX_INTERNAL mmumgr.c
 *
 * @brief  MMU manager: MMU configuration, setting/resetting access rights
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//------------------------------------------------------------------------------

#include "mmumgr.h"
#include "initcpu.h"
#include "intercpu.h"
#include "lcorecfg.h"
#include "mdmadrv.h"
#include "clkrst.h"
#include "heaps.h"
#include "uartdrv.h"
#include "hbidrv.h"
#include "systimers.h"
#include "spaccdrv.h"
#include "DSPdrv.h"
#include "fpdrv.h"
#include "cpridrv.h"
#include "pciedrv.h"
#include "evmfpga.h"
#include "srdsdrv.h"
#include "rex_serv.h"
#include "gemdrv.h"
#include "spidrv.h"

#define MMU_CORE_0              (1<<0)
#define MMU_CORE_1              (1<<1)
#define MMU_CORE_2              (1<<2)
#define MMU_CORE_3              (1<<3)
#define MMU_CORE_ALL            (MMU_CORE_0|MMU_CORE_1|MMU_CORE_2|MMU_CORE_3)
#define MMU_CORE_STOP           0xFFFF

#define MMU_SHARED              1
#define MMU_N_SHARED            0
#define MMU_CACHED              1
#define MMU_N_CACHED            0

#define MMU_L_VECTORS           0
#define MMU_RO                  1
#define MMU_RW                  2
#define MMU_ICPU                3
#define MMU_PRV_CORE0           4
#define MMU_STACK_HEAP          5
#define MMU_NCNB_HEAP           6
#define MMU_DDR1_HEAP           7
#define MMU_DDR1_NCNB_HEAP      8
#define MMU_H_VECTORS           9
#define MMU_IRAM                10
#define MMU_CRAM                11
#define MMU_ICPU_RX             14
#define MMU_ICPU_TX             15
#define MMU_ARM_SCU             16
#define MMU_HW_SEMA             17
#define MMU_APB_REGS            18
#define MMU_DDR0_CONTROLLER     19
#define MMU_DDR1_CONTROLLER     20
#define MMU_HW_ICPU_INT         21
#define MMU_HBI_RST_CLK         22
#define MMU_ROOT_INFO           23

extern UINT8 Image$$ROOT_RO$$Base[];
extern UINT8 Image$$ROOT_INFO$$Base[];
extern UINT8 Image$$ROOT_RWZI$$Base[];
extern UINT8 Image$$EXT_ICPU$$Base[];
extern UINT8 Image$$PRV_MEM_CORE0$$Base[];
extern UINT8 Image$$STACK_HEAP$$Base[];
extern UINT8 Image$$NCNB_DATA$$Base[];
extern UINT8 Image$$EMUL_DDR1_SPACE$$Base[];

#define MMU_INFO_RES_OK             0       // MMU is configured (1MB or PAGE 4KB is selected by the system)
#define MMU_INFO_RES_OK_PAGE4       1       // MMU is configured (PAGE 4KB is only used)
#define MMU_INFO_RES_SKIP           2       // This memory region is not allowed for the system

typedef struct _mmu_info_proc_res_
{
    UINT32      res;    // see: MMU_INFO_RES_xxx
    UINT32      base;   // base address
    UINT32      size;   // in bytes 
    UINT32      code;   // If this is executable region

}mmu_info_proc_res;

typedef mmu_info_proc_res (*mmu_info_proc)(UINT32 nID);
typedef void (*mmu_drv_proc)(UINT32 nCpuID, UINT32 nParam);

typedef struct _mmu_cfg_
{
    UINT32          CpuList;
    UINT32          Access;
    UINT32          Shared;
    UINT32          Cache;
    UINT32          SectID;
    mmu_info_proc   SectInfo;
    
}mmu_cfg;

typedef struct _mmu_drv_
{
    UINT32          DrvParam;
    mmu_drv_proc    DrvProc;

}mmu_drv;

mmu_info_proc_res MmuGetInfo(UINT32 id);

/******************************************************************************/
/*   THIS TABLE IS USED TO CONFIGURE MEMORY FOR ALL CORES POWERED BY SYSCORE  */
/*   It's possible to add/remove/modify memory sections and access rights     */
/*   If memory region is not defined in this table and drivers did not add    */
/*   such region to MMU table dynamically, ARM cores do not have access to    */
/*   these regions                                                            */
/******************************************************************************/

mmu_cfg mmu_table_cfg [] = 
{
        // ***********************************************************************************************
        //   CPU             ACCESS           SHARED          CACHE          SECT ID            INFORMER
        // ***********************************************************************************************

        {MMU_CORE_ALL,  AP_MGR_READ_ONLY,   MMU_N_SHARED,   MMU_N_CACHED,   MMU_APB_REGS,       MmuGetInfo},
        {MMU_CORE_ALL,  AP_MGR_READ_ONLY,   MMU_N_SHARED,   MMU_N_CACHED,   MMU_DDR0_CONTROLLER,MmuGetInfo},
        {MMU_CORE_ALL,  AP_MGR_READ_ONLY,   MMU_N_SHARED,   MMU_N_CACHED,   MMU_DDR1_CONTROLLER,MmuGetInfo},
        {MMU_CORE_ALL,  AP_MGR_READ_WRITE,  MMU_N_SHARED,   MMU_N_CACHED,   MMU_ARM_SCU,        MmuGetInfo},
        {MMU_CORE_ALL,  AP_MGR_READ_WRITE,  MMU_N_SHARED,   MMU_N_CACHED,   MMU_HW_SEMA,        MmuGetInfo},
        {MMU_CORE_ALL,  AP_MGR_READ_WRITE,  MMU_N_SHARED,   MMU_N_CACHED,   MMU_HW_ICPU_INT,    MmuGetInfo},
        {MMU_CORE_0,    AP_MGR_READ_WRITE,  MMU_N_SHARED,   MMU_N_CACHED,   MMU_HBI_RST_CLK,    MmuGetInfo},
        
        {MMU_CORE_ALL,  AP_MGR_READ_ONLY,   MMU_SHARED,     MMU_CACHED,     MMU_L_VECTORS,      MmuGetInfo},
        {MMU_CORE_ALL,  AP_MGR_READ_ONLY,   MMU_SHARED,     MMU_CACHED,     MMU_RO,             MmuGetInfo},
        {MMU_CORE_ALL,  AP_MGR_READ_ONLY,   MMU_SHARED,     MMU_CACHED,     MMU_ROOT_INFO,      MmuGetInfo},
        
        {MMU_CORE_ALL,  AP_MGR_READ_WRITE,  MMU_SHARED,     MMU_CACHED,     MMU_RW,             MmuGetInfo},
        {MMU_CORE_ALL,  AP_MGR_READ_WRITE,  MMU_SHARED,     MMU_N_CACHED,   MMU_ICPU,           MmuGetInfo},
        {MMU_CORE_0,    AP_MGR_READ_WRITE,  MMU_N_SHARED,   MMU_CACHED,     MMU_PRV_CORE0,      MmuGetInfo},
        {MMU_CORE_ALL,  AP_MGR_READ_WRITE,  MMU_SHARED,     MMU_CACHED,     MMU_STACK_HEAP,     MmuGetInfo},
        {MMU_CORE_ALL,  AP_MGR_READ_WRITE,  MMU_SHARED,     MMU_N_CACHED,   MMU_NCNB_HEAP,      MmuGetInfo},
        {MMU_CORE_ALL,  AP_MGR_READ_WRITE,  MMU_SHARED,     MMU_CACHED,     MMU_DDR1_HEAP,      MmuGetInfo},
        {MMU_CORE_ALL,  AP_MGR_READ_WRITE,  MMU_SHARED,     MMU_N_CACHED,   MMU_DDR1_NCNB_HEAP, MmuGetInfo},
        {MMU_CORE_ALL,  AP_MGR_READ_ONLY,   MMU_SHARED,     MMU_CACHED,     MMU_H_VECTORS,      MmuGetInfo},
        {MMU_CORE_0,    AP_MGR_READ_WRITE,  MMU_N_SHARED,   MMU_N_CACHED,   MMU_IRAM,           MmuGetInfo},
        {MMU_CORE_ALL,  AP_MGR_READ_WRITE,  MMU_N_SHARED,   MMU_N_CACHED,   MMU_CRAM,           MmuGetInfo},

        #ifdef I_CPU_RX_CACHED
        {MMU_CORE_ALL,  AP_MGR_READ_WRITE,  MMU_SHARED,     MMU_CACHED,     MMU_ICPU_RX,        MmuGetInfo},
        #else
        {MMU_CORE_ALL,  AP_MGR_READ_WRITE,  MMU_N_SHARED,   MMU_N_CACHED,   MMU_ICPU_RX,        MmuGetInfo},
        #endif
        
        #ifdef I_CPU_TX_CACHED
        {MMU_CORE_ALL,  AP_MGR_READ_WRITE,  MMU_SHARED,     MMU_CACHED,     MMU_ICPU_TX,        MmuGetInfo},
        #else
        {MMU_CORE_ALL,  AP_MGR_READ_WRITE,  MMU_N_SHARED,   MMU_N_CACHED,   MMU_ICPU_TX,        MmuGetInfo},
        #endif

        // ***********************************************************************************************
        {MMU_CORE_STOP, 0,                  0,              0,              0,                  NULL},
};


/******************************************************************************/
/*   THIS TABLE IS USED TO ALLOW DRIVERS TO CONFIGURE MMU TABLE OF ALL CORES  */
/*   Drivers API defined in this table will be called by the system           */
/*   to configure MMU dynamically (in RUN-TIME mode)                          */
/******************************************************************************/

mmu_drv mmu_drv_table [] = 
{
// *****************************************************************************
//     Parameter    Driver API
// *****************************************************************************
        {0,         MdmaSysMmuCfg},
        {0,         MdmaSpuMmuCfg},
        {0,         MdmaRadMmuCfg},
        {0,         FecDlMmuCfg},
        {0,         FecUlMmuCfg},
        {0,         SysClkMmuCfg},
        {0,         UartMmuCfg},
        {0,         HBIMmuCfg},
        {0,         SysTimerMmuCfg},
        {0,         DspMmuCfg},
        {0,         FpMmuCfg},
        {0,         CpriMmuCfg},
        {0,         PcieMmuCfg},
        {0,         EvmFpgaMmuCfg},
        {0,         SrdsMmuCfg},
        {0,         RexServMmuCfg},
        {0,         GemDrvMmuCfg},
        {0,         SpiDrvMmuCfg},
        #ifdef SPACC_DRV_ENABLED
        {0,         SPAccMmuCfg},
        #endif
// *****************************************************************************
        {0,         NULL}
};

UINT32 IsMMUMsgEnabled (void)
{
    return (1 << MxGetCpuID()) & MX_MMU_INIT_MSG_MASK;
}

mmu_info_proc_res MmuGetInfo(UINT32 id)
{
    mmu_info_proc_res res;

    res.base = 0;
    res.size = 0;
    res.code = 0;
    res.res  = MMU_INFO_RES_SKIP;

    switch (id)
    {
        case MMU_L_VECTORS:
        {
            if (MxGetARMID () != 0)
            {
                res.res = MMU_INFO_RES_SKIP;
                break;
            }

            if (!IS_HBI_MODE())
            {
                res.base = 0x0;
                res.size = 4*1024; 
                res.res  = MMU_INFO_RES_OK;
                res.code = 1;
            }

            break;
        }

        case MMU_RO:
        {
            res.base = (UINT32)Image$$ROOT_RO$$Base;
            res.size = ROOT_RO_SIZE;
            res.res  = MMU_INFO_RES_OK;
            res.code = 1;

            break;
        }

        case MMU_RW:
        {
            res.base = (UINT32)Image$$ROOT_RWZI$$Base;
            res.size = ROOT_RW_SIZE;
            res.res  = MMU_INFO_RES_OK;

            break;
        }

        case MMU_ICPU:
        {
            if (CoreCfg.HostIntID == IC_CHAN_SVSR_EX_ICPU)
            {
                res.base = (UINT32)Image$$EXT_ICPU$$Base;
                res.size = ROOT_ICPUV2_SIZE;
                res.res  = MMU_INFO_RES_OK;
            }
            else if (CoreCfg.HostIntID == IC_CHAN_SVSR)
            {
                res.base = CoreCfg.ICPUBase;
                res.size = GetICpuRegionSize (CoreCfg.ICPUBase, CoreCfg.HostIntID);
                res.res  = MMU_INFO_RES_OK;
            }

            break;
        }

        case MMU_PRV_CORE0:
        {
            res.base = (UINT32)Image$$PRV_MEM_CORE0$$Base;
            res.size = ROOT_PRV_CORE0_MEM_SIZE;
            res.res  = MMU_INFO_RES_OK;

            break;
        }

        case MMU_STACK_HEAP:
        {
            res.base = (UINT32)Image$$STACK_HEAP$$Base;
            res.size = ROOT_HEAP_SIZE;
            res.res  = MMU_INFO_RES_OK;

            break;
        }

        case MMU_NCNB_HEAP:
        {
            res.base = (UINT32)Image$$NCNB_DATA$$Base;
            res.size = ROOT_NCNB_SIZE;
            res.res  = MMU_INFO_RES_OK;

            break;
        }

        case MMU_DDR1_HEAP:
        {
            res.size = GetDDR1HeapBaseSize(&res.base);

            // if DDR1 is enabled, the base address is not 0
            // ---------------------------------------------
            res.res  = (res.base == 0) ? MMU_INFO_RES_SKIP : MMU_INFO_RES_OK;

            break;
        }

        case MMU_DDR1_NCNB_HEAP:
        {
            res.size = GetDDR1NcNbHeapBaseSize(&res.base);

            // if DDR1 is enabled, the base address is not 0
            // ---------------------------------------------
            res.res  = (res.base == 0) ? MMU_INFO_RES_SKIP : MMU_INFO_RES_OK;

            break;
        }

        case MMU_H_VECTORS:
        {
            if (MxGetARMID() != 0 || 1)
            {
                res.base = 0xFFFF0000;
                res.size = 4*1024;
                res.res  = MMU_INFO_RES_OK;
                res.code = 1;
            }

            break;
        }

        case MMU_IRAM:
        {
            if (!IS_HBI_MODE())
            {
                res.base = IRAM_HEAP_ADDR;
                res.size = IRAM_HEAP_SIZE;
                res.res  = MMU_INFO_RES_OK_PAGE4;
            }
            else
            {
                res.base = IRAM_BASEADDR;
                res.size = 0x40000; // 256KB
                res.res  = MMU_INFO_RES_OK_PAGE4;
                res.code = 1;
            }
           
            break;
        }

        case MMU_CRAM:
        {
            res.base = CRAM_BASEADDR;
            res.size = CRAM_SIZE;
            res.res  = MMU_INFO_RES_SKIP;

            if (MxGetCpuID () == 0)
            {
                res.res = MMU_INFO_RES_OK;
            }
            else
            {
                if (MdmaIsMemUsed(res.base, res.size) ||
                    FecIsMemUsed(res.base, res.size))
                {
                    res.res = MMU_INFO_RES_OK;
                }
            }
            
            break;
        }

        case MMU_ICPU_RX:
        {
            if (!IS_HBI_MODE())
            {
                res.base = ICGetRXPartStorageAddr (CoreCfg.ICPUBase, CoreCfg.HostIntID);
                res.size = ROUND(ICGetRXPartStorageSize (CoreCfg.ICPUBase, CoreCfg.HostIntID), (1024*1024));
                res.res  = MMU_INFO_RES_OK;
            }
            break;
        }

        case MMU_ICPU_TX:
        {
            if (!IS_HBI_MODE())
            {
                res.base = ICGetTXPartStorageAddr (CoreCfg.ICPUBase, CoreCfg.HostIntID);
                res.size = ROUND(ICGetTXPartStorageSize (CoreCfg.ICPUBase, CoreCfg.HostIntID), (1024*1024));
                res.res  = MMU_INFO_RES_OK;
            }
            break;
        }

        case MMU_ARM_SCU:
        {
            // to allow access to ARM:SCU{SCU,GIC,L2}
            res.base = MxGetArmScuBaseAddr();
            res.size = 1*1024*1024;
            res.res  = MMU_INFO_RES_OK_PAGE4;
            break;
        }

        case MMU_HW_SEMA:
        {
            res.base = SEMAPHORES_BASE;
            res.size = 4*1024;
            res.res  = MMU_INFO_RES_OK;
            break;
        }

        case MMU_APB_REGS:
        {
            res.base = APB_BASEADDR;
            res.size = 9*1024*1024;
            res.res  = MMU_INFO_RES_OK_PAGE4;
            break;
        }

        case MMU_DDR0_CONTROLLER:
        {
            res.base = DDR0_CONTROLLER_BASE;
            res.size = 0x80000;
            res.res  = MMU_INFO_RES_OK_PAGE4;
            break;
        }
        case MMU_DDR1_CONTROLLER:
        {
            res.base = DDR1_CONTROLLER_BASE;
            res.size = 0x80000;
            res.res  = MMU_INFO_RES_OK_PAGE4;
            break;
        }
        case MMU_HW_ICPU_INT:
        {
            res.base = CFG_SYS_CFG_0;
            res.size = 4*1024;
            res.res  = MMU_INFO_RES_OK_PAGE4;
            break;
        }
        case MMU_HBI_RST_CLK:
        {
            break;
        }

        case MMU_ROOT_INFO:
        {
            res.base = (UINT32)Image$$ROOT_INFO$$Base;
            res.size = ROOT_INFO_SIZE;
            res.code = 0;              
            res.res  = MMU_INFO_RES_OK;

            break;
        }
    }

    return res;
}

MXRC MMUInit (UINT32 nCpuID, MmuCfg mmu_cfg)
{
    LPVOID pStor = (LPVOID)(mmutable[nCpuID]);
    MASTERTABLE hMT;
    UINT32 i, base, size, type;
    mmu_info_proc_res info;
    MXRC rc;

    if (nCpuID == 0 || 1)
    {
        hMT = MxMmuCreateMasterTable(pStor, MMU_1M_PAGE_ID, MMU_MASTER_AP(AP_NO_ACCESS) | MMU_MASTER_NX);
    }
    else
    {
        hMT = (MASTERTABLE) pStor;
    }

    i = 0;
    rc = MX_OK;

    while ((mmu_table_cfg[i].CpuList != MMU_CORE_STOP) && (rc == MX_OK))
    {
        if ((mmu_table_cfg[i].CpuList & (1 << nCpuID)) == 0)
        {
            i++;
            continue;
        }

        if (mmu_table_cfg[i].SectInfo == NULL)
        {
            i++;
            continue;
        }

        info = mmu_table_cfg[i].SectInfo (mmu_table_cfg[i].SectID);

        if (info.res == MMU_INFO_RES_SKIP)
        {
            i++;
            continue;
        }

        base = info.base & ~0xFFFFF;
        size = (info.size >> 20) ? (info.size >> 20) : 1;

        // if section is 1MB aligned (base address & size)
        //------------------------------------------------
        if ((info.base == base)                     && 
            ((size<<20) == info.size)               && 
            (info.res != MMU_INFO_RES_OK_PAGE4)     && 
            (MxMmuGetRegionMapType (info.base) == MMU_SECTION_TYPE_1M_SECTION))
        {
            if (mmu_table_cfg[i].Cache)
            {
                type = mmu_table_cfg[i].Shared ? (TEXCB_MASTER_OI_WB_WA_S) : (TEXCB_MASTER_OI_WB_WA);
            }
            else
            {
                type = TEXCB_STRONGLY_ORDERED;
            }

            type |= MMU_MASTER_AP(mmu_table_cfg[i].Access);
            type |= MMU_MASTER_DOMAIN(0);

            if (!info.code)
                type |= MMU_MASTER_NX;

            //uart_printf ("MMU: %x %d (ap:%x)\r\n", base, size, type);

    	    rc = MxMmuSetSectParam(hMT, base, base, size, type);

            //uart_printf ("MMU: 0x%08x / %d, num:%d\r\n", info.base, info.size, size);
        }
        // The memory region is divided by 4KB pages
        //------------------------------------------------
        else 
        {
            //uart_printf (" *MMU-P: %x %d\r\n", info.base, info.size);
        
            rc = MmuMgrCfgMemCacheShareAP(info.base, 
                                          info.size, 
                                          info.code,
                                          mmu_table_cfg[i].Cache, 
                                          mmu_table_cfg[i].Shared, 
                                          mmu_table_cfg[i].Access, 
                                          AP_NO_ACCESS);

            if (FAILED(rc))
            {
                uart_printf ("MMU-ERROR, rc=0x%x, base:0x%x, size:%d\r\n", rc, info.base, info.size);
                break;
            }
        }

        i ++;
    }

    if (FAILED(rc))
    {
        uart_printf ("CPU[%d], mmu initialization failed, rc=0x%x\r\n", MxGetCpuID(), rc);
        return rc;
    }

    if (mmu_cfg != NULL)
    {
        mmu_cfg (hMT);
    }

    i = 0;
    while (mmu_drv_table[i].DrvProc != NULL)
    {
        mmu_drv_table[i].DrvProc(nCpuID, mmu_drv_table[i].DrvParam);
        i++;
    }

    MxMmuSetDomain(0, MMU_DOMAIN_CLIENT);
    MxMmuSetDomain(1, MMU_DOMAIN_NOACCESS);
    MxMmuSetTTB(hMT, 0);

    #ifdef L2_ALWAYS_ENABLED
        MxMmuCtrl(MMU_CTRL_L1_ENABLED | MMU_CTRL_L2_ENABLED | MMU_CTRL_ICACHE_ENABLED | MMU_CTRL_DCACHE_ENABLED);
    #else
        if (CoreCfg.Options & LCFG_OPT_L2_ENABLED)
       	    MxMmuCtrl(MMU_CTRL_L1_ENABLED | MMU_CTRL_L2_ENABLED | MMU_CTRL_ICACHE_ENABLED | MMU_CTRL_DCACHE_ENABLED);
        else
            MxMmuCtrl(MMU_CTRL_L1_ENABLED | MMU_CTRL_ICACHE_ENABLED | MMU_CTRL_DCACHE_ENABLED);
    #endif

    if (MxMmuIsEnabled ())
    {
        if (IsMMUMsgEnabled())
            uart_printf ("[CPU:%d] MMU is enabled [D-CACHE(%s), I-CACHE(%s)]\r\n",
                    MxGetCpuID (),
                    MxMmuIsDCache() ? "ON" : "OFF",
                    MxMmuIsICache() ? "ON" : "OFF");

        #ifdef MX_ARM_L2_CACHE_ENABLED
        if (MxL2IsEnabled ())
        {
            if (IsMMUMsgEnabled())
                uart_printf ("[CPU:%d] MMU L2 cache is [ON] mode:[%s]\r\n", MxGetCpuID (), MxL2IsWB() ? "write-back" : "write-through");
        }
        else
        {
            if (IsMMUMsgEnabled())
                uart_printf ("[CPU:%d] MMU L2 cache is [OFF]\r\n", MxGetCpuID ());
        }
        #endif
    }
    else
    {
        if (IsMMUMsgEnabled())
            uart_printf ("[CPU:%d] MMU is disabled\r\n", MxGetCpuID ());
    }

    return MX_OK;
}

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
    
MXRC MmuMgrCfgMemCacheShareAP(UINT32 Base, UINT32 nSize, UINT32 nCode, UINT32 nCache, UINT32 nShare, UINT32 nAP, UINT32 nDefAP)
{
    MASTERTABLE hMT = MxMmuGetMasterTable();
    MMUSECTION hSect;
    MXRC rc = MX_OK;
    UINT32 Base_1M_a_b, Base_1M_a_e;
    UINT32 nType, OneShot;

    //uart_printf ("  MMU-P(c:%d s:%d): 0x%08x / %d\r\n", nCache, nShare, Base, nSize);

    if (nCache)
    {
        nType = nShare ? (TEXCB_PAGE4_OI_WB_WA_S) : (TEXCB_PAGE4_OI_WB_WA);
    }
    else
    {
        nType = TEXCB_STRONGLY_ORDERED;
    }

    if (!nCode)
        nType |= MMU_PAGE4_NX;

    Base_1M_a_b = (Base >> 20) << 20;
    Base_1M_a_e = ((Base+nSize) >> 20) << 20;

    OneShot = (Base_1M_a_b == 0xFFF00000);

    while (Base_1M_a_b <= Base_1M_a_e || OneShot)
    {
        if (MxMmuGetRegionMapType (Base_1M_a_b) != MMU_SECTION_TYPE_PAGED_4)
        {
            //uart_printf ("   MMU 1MB to PAGE#4: %x\r\n", ((Base_1M_a_b) >> 20) << 20);
        
            hSect = MxMmuGetSection(hMT, Base_1M_a_b);
            
            _ASSERT_PTR(hSect);
            rc = MxMmuSetSectionType (hSect, MMU_SECTION_TYPE_PAGED_4, MMU_MASTER_DOMAIN(0), MMU_PAGE_AP(nDefAP) | nType);

            _ASSERT_RC(rc);
            
            if (FAILED(rc))
                return rc;
        }
        
        if (OneShot)
            break;
        
        Base_1M_a_b += 0x100000;
    }

    nSize = ROUND(nSize, 4*1024);

    while (nSize != 0)
    {
        rc = MxMmuSetMemAP(Base, nAP);

        _ASSERT_RC(rc);

        if (FAILED(rc))
            return rc;

        rc = MxMmuSetMemXN(Base, !nCode);

        _ASSERT_RC(rc);

        if (FAILED(rc))
            return rc;

        nSize -= 4*1024;
        Base += 4*1024;
    }

    return MX_OK;
}

/** @brief This function is designed to be called at initialization step of system start-up
           to configure MMU table and mainly used by the system drivers to allow accesing 
           HW registers memory range

    @param Base     [in] - the base address of the region
    @param nSize    [in] - the size of the region in bytes
    @param nAP      [in] - the access permission for the list of pages specified by [BASE-SIZE] see: 4GMX enum <MMU_AP>
    @param nDefAP   [in] - the AP for other pages but only if 1MB section was not divided to pages, if was divided, it's ignored

    @return [UINT32] an error code */
    
MXRC MmuMgrCfgMemAP(UINT32 Base, UINT32 nSize, UINT32 nAP, UINT32 nDefAP)
{
    UINT32 lr = __return_address () - 4;
    MXRC rc = MmuMgrCfgMemCacheShareAP(Base, nSize, 0, 0, 0, nAP, nDefAP);
    
    _ASSERT_RC(rc);

    if (FAILED(rc))
        uart_printf ("MmuMgrCfgMemAP, rc=0x%x, lr=0x%x\r\n", rc, lr);
    
    return rc;
}


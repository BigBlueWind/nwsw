//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Config.h
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.96 $
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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "4gmx.h"
#include "hal.h"

#define CPU_NUM  4

#define CPU0_ENABLED

#if CPU_NUM >= 2
#define CPU1_ENABLED
#endif

#if CPU_NUM >= 3
#define CPU2_ENABLED
#endif

#if CPU_NUM >= 4
#define CPU3_ENABLED
#endif

// It will be defined with section name to locate all variables
// with this marker in NCNB section
#define NCNBVAR	__attribute__((section(".NCNBSECT.")))
#define EXT_ICPU_VAR	__attribute__((section(".EXT_ICPU_SECT.")))
#define EMUL_DDR1	__attribute__((section(".EMUL_DDR1.")))
#define VARADDR(x) __attribute__((at(x)))
#define PRV_CORE0           __attribute__((section(".PRV_CORE0.")))

#define MX_MMU_L2_1M_NUMS               100
#define MX_TIMER_FRACTION               10                      // 1ms / MX_TIMER_FRACTION = timer period
#define MX_TIMER_CORES                  (0<<3|0<<2|0<<1|1<<0)   // the bit mask where timer is initialized 
#define MX_WD_CORES                  	(0<<3|0<<2|0<<1|0<<0)   // the bit mask where watchdog is initialized 
#define MX_MMU_INIT_MSG_MASK            (0<<3|0<<2|0<<1|1<<0)
#define MX_APP_INIT_MSG_MASK            (0<<3|0<<2|0<<1|1<<0)

/* The MAP of ARAM-J (stacks and system heap ), the total size is 4 KB */
/* The size of ARAM-J is 8KB, so 4KB for every ARM processor           */
#define SPRV_STACK_SIZE                 (1024 * 100)
#define ABORT_STACK_SIZE                (1024)
#define UNDEF_STACK_SIZE                (1024)
#define FIQ_STACK_SIZE                  (1024 * 10)
#define IRQ_STACK_SIZE                  (1024 * 10)
#define DEF_HEAP_SIZE                   (25 * 1024)
#define RVDS_SYSTEM_HEAP                (50 * 1024)
#define IRAM_HEAP_ADDR                  (IRAM_BASEADDR + 0x10000)
#define IRAM_HEAP_SIZE                  ((256 * 1024) - 0x10000)
#define APP_HEAP_SIZE                   (50 * 1024 * 1024)
#define NCNB_HEAP_SIZE                  (8 * 1024 * 1024)

//******************* [DDR1 parameters] *******************************
//                    -----------------
//   These parameters define DDR-1 heaps, there are two configurations:
//       - DDR-1 is enabled to be used (real DDR-1 HW is used)
//       - DDR-1 is disabled to be used (special compilation of SYSCORE, see: NO_USE_DDR1)
//         in this case DDR-0 is used only and DDR-1 heaps are mapped to DDR-0 space
//*********************************************************************

#ifndef NO_USE_DDR1
    #define DDR1_CACHEABLE_HEAP_SIZE    (128 * 1024 * 1024)  // set to 0 to remove DDR1 cacheable heap
#else
    #define DDR1_CACHEABLE_HEAP_SIZE    (128 * 1024 * 1024)  // set to 0 to remove DDR1 cacheable heap
#endif
//*********************************************************************

#define ETH_NUM_TX_BUFS                 (32)
#define ETH_NUM_RX_BUFS                 (32)

#ifndef NO_USE_DDR1
    #define SVSR_API_LOGGER_BUF_SIZE   (22 * 1024 * 1024)  // set to 0 to remove DDR1 cacheable heap
#else
    #define SVSR_API_LOGGER_BUF_SIZE   (22 * 1024 * 1024)  // set to 0 to remove DDR1 cacheable heap
#endif

//#define UNIDIAG_ENABLED
/*
#define PHY_TXSTART_REQ     1
#define PHY_TXSTART_CONF    2
#define PHY_TXSTART_IND     3
#define PHY_TXSDU_REQ       4
#define PHY_TXSDU_CONF      5
#define PHY_TXEND_IND       6
#define PHY_RXSTART_REQ     7
#define PHY_RXSTART_CONF    8
#define PHY_RXSTART_IND     9
#define PHY_RXSDU_IND       10
#define PHY_RXEND_IND       11
#define PHY_INIT_REQ        12
#define PHY_INIT_IND        13
#define PHY_RXSTATUS_IND    14
#define PHY_RECONFIG_REQ    15
#define PHY_RECONFIG_CNF    16
#define PHY_START_REQ       17
#define PHY_START_CONF      18
#define PHY_STOP_REQ        19
#define PHY_STOP_CONF       20
#define PHY_STOP_IND        21
#define PHY_TXHIADCIUL_REQ  22
#define PHY_TXHISDU_REQ     23
#define PHY_TXDCIULSDU_REQ  24
*/
#define SVSR_API_LOGGER_DEF_MASK        (0)
#define SVSR_API_LOGGER_DEF_MASK_ENA        ((1<<1)|(1<<3)|(1<<4)|(1<<7)|(1<<10)|(1<<12)|(1<<14)|(1<<17)|(1<<19)|(1<<22)|(1<<23)|(1<<24))

#define MLOG_BUF_SIZE                   (MLOG_FRAME_LIMIT * MLOG_FRAME_SIZE * (CPU_NUM + MLOG_DEVICE_COUNT) + 1024*1024)
#define GEN_LOG_BUF_SIZE                (1024)
#define GEN_LOG_OPTIONS                 (LOG_CTRL_RAD_MDMA | LOG_CTRL_SYNCPU)
#define GEN_LOG_HEAP                    (DDR1NcnbHeap)
/** @brief The patterns for stacks filling */

#define SVSR_STACK_PATTER               0xCCCCCCCC
#define ABORT_STACK_PATTER              0xCC
#define UNDEF_STACK_PATTER              0xCC
#define FIQ_STACK_PATTER                0xCC
#define IRQ_STACK_PATTER                0xCC

/** @brief The high and lower application version */

#define APP_H_VER                       0x04
#define APP_L_VER                       0x00

/**********************************************************************
*     The flags of the conditional compilation                        *
***********************************************************************/
//#define ARM_LVECTORS_PROTECTION
//#define SYS_INFO_ENABLED
//#define APP_4GMX_TRACER_ENABLED
#define GIC_INIT
#define LOCAL_TIMER_INIT
#define SYS_TIMER_INIT
#define PRINTF_INIT0
#define PRINTF_INIT1
#define PRINTF_INIT2
//#define DSP_PRINT_INIT
#define CMD_LINE_INIT
//#define TIMER_TEST
#define WIMAX_INIT
#define LTE_INIT
//#define IRAM_FEC_MDMA_ALLOC_ENABLED
#define GIC_IRQ_PROTECTION_ENABLED
#define SVSR_API_LOGGER_ENABLED
#define SPI_DRV_INIT
#define RDMEM_CACHE_CONTROL_ENABLED
#define GENLOG_ENABLED
#define L2_ALWAYS_ENABLED
//#define MMU_CFG_APP_HEAP_AS_64K_PAGES
#define SPACC_DRV_ENABLED
//#define SRIO_DRV_ENABLED
#define REX_ENABLED
#define IRAM_HEAP_INIT
//#define PROFILE_T3K
//#define X7_DEV_ENABLED
//#define PM_ENABLED
//#define DL_IQ_PATTERN_ENABLED
#define REX_S_FORCED_ENABLED
#define UNIDIAG_ENABLED
//#define IQPHONE_ENABLED
#define SWDIAG_EN // enables Turner Software Diagnostics module
//#define HEAPS_PRINT_INFO_ENABLED
//#define MMU_PRINT_INFO_ENABLED

#ifdef MX_TRACER_ENABLED
    #define SYS_TRACER_ENABLED      // To enable tracing partitions/queues
#endif

#ifdef IRAM_FEC_MDMA_ALLOC_ENABLED

#define IRAM_HEAP_INIT
#define MDMA_FEC_HEAP   IramHeap

#else

#define MDMA_FEC_HEAP   DspHeap

#endif

/* uncomment this to compile code for IPHASE board */
//#define IPHASE_36701_BOARD

#define SPI_HEAP            HNULL      // HNULL = the default core heap is used
#define SPI_DEF_DEV_NUM     10

// these macro variables enable/disable corresponding startup tests
#define DIAG_FEC_ENABLED
#define DIAG_FP_ENABLED
//#define DIAG_FP_PRINT_ALWAYS  // to print messages even if FP diagnostics is ok
#define DIAG_DSP_ENABLED
//#define DIAG_DSP_PRINT_ALWAYS  // to print messages even if CEVA diagnostics is ok
#define DIAG_ARM_ENABLED
#define DIAG_SCH_ENABLED
//#define DIAG_I2C_ENABLED
//#define DIAG_SPI_ENABLED
//#define DIAG_PCIE_ENABLED
#ifdef SPACC_DRV_ENABLED
#define DIAG_SPACC_ENABLED
#endif

#define CLINE_INIT
#define PHY_GEM_ENABLED         // To allow linux to enable this device
//#define PHY_GEM_FORCED_CORE0    // GEMAC-1 location, core-0
//#define PHY_GEM_FORCED_CORE1    // GEMAC-1 location, core-1
//#define PHY_GEM_FORCED_CORE2    // GEMAC-1 location, core-2
//#define PHY_GEM_FORCED_CORE3    // GEMAC-1 location, core-3
//#define PHY_GEM_IGNORE_LINUX_ENABLE_GEM
#define SYS_SHUTDOWN_DRV_ENABLED
#define I_CPU_RX_CACHED
#define I_CPU_TX_CACHED
#define SVSR_ON_ARM_1_ENABLED
#define SVSR_ON_ARM_2_ENABLED
#define SVSR_ON_ARM_3_ENABLED
//#define SVSR_INIT_MSG_ENABLED   // to enable initialization messages (just message not error messages)
#define PROC_ABORTS_ENABLED
#define SYSCORE_DEFAULT_UART_ID  1 // Console manager works with: 0:UART-0(not supported in current configuration), 1:UART-1, 2:UART-2, 3:disabled
//#define UART_DISABLE_FORCED // disables UART and console using
#define CMGR_TURNED_OFF 3

#define MAX_NUM_ANT_SUPPORTED           2
#define CPRI_DMA_BUF_ADDR               0xFC200000
#define CPRI_DMA_BUF_SIZE               (2*4*61440)
#define CPRI_RXDMA_BUF_ADDR             (CPRI_DMA_BUF_ADDR + CPRI_DMA_BUF_SIZE)
#define CPRI_RXDMA_BUF_SIZE              CPRI_DMA_BUF_SIZE

/**********************************************************************
*     Use this flag to enable/disable HBI interface and               *
*     enable/disable API routing to/from the host                     *
***********************************************************************/

#if defined(TIMER_TEST)&&!defined(GIC_INIT)
#error "Please define <GIC_INIT> macro"
#endif

typedef struct _STACKS_HEAP_
{
    char spvs[SPRV_STACK_SIZE];
    char abort[ABORT_STACK_SIZE];
    char undef[UNDEF_STACK_SIZE];
    char fiq[FIQ_STACK_SIZE];
    char irq[IRQ_STACK_SIZE];

} STACKSHEAP;

extern STACKSHEAP stacks_cpu0;
extern STACKSHEAP stacks_cpu1;
extern STACKSHEAP stacks_cpu2;
extern STACKSHEAP stacks_cpu3;

extern const UINT32 mmutable[][4096];
extern const UINT32 mmu_l2_table[CPU_NUM][MX_MMU_L2_1M_NUMS * 256];

extern char defheapbuf[][DEF_HEAP_SIZE];
extern MXCONTEXT MxCtx[];
extern MXSMPCTX  MxSmpCtx;

#define STACK_BASE_ADDR(cpu)  ( &stacks_cpu0 + (cpu))

#define DEFHEAP_BASE_ADDR(cpu)  (defheapbuf [(cpu)])

#define STACK_BASE_ADDR_CPU0  (stacks_cpu0)
#define STACK_BASE_ADDR_CPU1  (stacks_cpu1)
#define STACK_BASE_ADDR_CPU2  (stacks_cpu2)
#define STACK_BASE_ADDR_CPU3  (stacks_cpu3)

#define PRI_DDR_MON         0x1
#define PRI_DSP             0x1
#define PRI_EVMFPGA         0x0     // Highest
#define PRI_GEM             0x1
#define PRI_HBI             0x1
#define PRI_PCIE            0x1
#define PRI_RDMEM           0x1
#define PRI_SCH_TASK        0x1
#define PRI_SPI             0x1
#define PRI_SRIO            0x1
#define PRI_SYS_SHTDWN      0x1
#define PRI_SYS_TIMERS      0x1
#define PRI_TIMER           0x0     // Highest
#define PRI_CPRI            0x0     // Highest
#define PRI_DL_FEC          0x1
#define PRI_UL_FEC          0x1
#define PRI_MDMA_SYS        0x1
#define PRI_MDMA_RAD        0x1
#define PRI_MDMA_SPU        0x1
#define PRI_MAP_ARM         0x1
#define PRI_MAP_DMA         0x1
#define PRI_UART            0x1
#define PRI_ICPU            0x1
#define PRI_PMON            0x1
#define PRI_REX             0x1
#define PRI_REX_SERV        0x1
#define PRI_SPAC            0x1


extern void * _memcpy (void *dest, const void *src, unsigned int count);

#ifdef __CC_ARM
#define CPU_IDLE() __nop ()
#else
#define CPU_IDLE()
#endif

#endif /*_CONFIG_H_*/

#ifdef __cplusplus
}
#endif


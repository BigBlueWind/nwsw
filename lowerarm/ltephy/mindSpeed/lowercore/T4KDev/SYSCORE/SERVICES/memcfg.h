//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL memcfg.h
 *
 * @brief  Memory regions definition
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------
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
#define CORE0_HEAP_SIZE                 ((4 * 1024 * 1024) + (1024*1024/2)) // 4.5 MB

#define CRAM_VAR_SIZE                   0x2000
#define CRAM_VAR_BASE                   0xFC300000-CRAM_VAR_SIZE

//******************* [SCATTER FILES PARAMETERS] **********************
#define ROOT_RO_SIZE                    0x0700000       // 7 MB
#define ROOT_RW_SIZE                    0x0500000       // 5 MB
#define ROOT_ICPUV2_SIZE                0x0A00000       // 10 MB
#define ROOT_HEAP_SIZE                  0x3C00000       // 60 MB
#define ROOT_PRV_CORE0_MEM_SIZE         0x0500000       // 5 MB
#define ROOT_INFO_SIZE                  0x0200000       // 2 MB
#define ROOT_NCNB_SIZE                  0x0A00000       // 10 MB

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


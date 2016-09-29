//-------------------------------------------------------------------------------------------
/** @file devinfo.h
 *
 * @brief Device Information functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
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

#ifndef _DEVINFO_H_
#define _DEVINFO_H_

#include "config.h"

#define HWID_T3K    3000
#define HWID_T4K    4000
#define HWID_T4K_X4 4000
#define HWID_T4K_X6 4006
#define HWID_T4K_X7 4007

//TBD
#define	HWID_T2K	5000

#define OBJECTID_START  100

typedef enum{
    DEVID_START = OBJECTID_START,

    DEVID_ARM_CORE = DEVID_START,             /* to carry the number of ARM cores in our cluster if any */
    DEVID_DSP_CORE,                 /* to carry the number of DSP cores in DSP cluster */
    DEVID_L1_CACHE,                 /* to carry the L1 cache capacity */
    DEVID_L2_CACHE,                 /* to carry the L2 cache capacity */
    DEVID_FEC,                              /* to carry the number of FEC processors */
    DEVID_FP,                               /* to carry the number of FP/MAP processors */
    DEVID_MDMA,                         /* to carry the number of MDMA engines */
    DEVID_GEMAC,                        /* to carry the number of GEMACs */
    DEVID_DDR,                              /* to carry the number of DDR interfaces */
    DEVID_CPRI,                             /* to carry the number of CPRI interfcaes */
    DEVID_SRIO,                         /* to carry the number of SRIO ports */
    DEVID_PCIE,
    DEVID_SPA,
    DEVID_TIMER,
    DEVID_I2C,
    DEVID_SPI,

    DEVID_END
}DevIDs;

typedef enum{
    MEMID_START = (OBJECTID_START + 100),

    MEMID_DDR0 = MEMID_START,
    MEMID_DDR0_NCNB,
    MEMID_DDR1,
    MEMID_DDR1_NCNB,
    MEMID_CRAM,
    MEMID_IRAM,
    MEMID_PRV_CORE0,

    MEMID_END
}MemIDs;

typedef enum{
    FEATID_START = (OBJECTID_START + 200),

    FEATID_L1_CACHE = FEATID_START,
    FEATID_L2_CACHE,

    FEATID_L2_ALWAYS,
    FEATID_ARM_LVECTORS_PROTECTION,
    FEATID_GIC_IRQ_PROTECTION,
    FEATID_IRAM_FEC_MDMA_ALLOC,
    FEATID_MMU_CFG_APP_HEAP_AS_64K_PAGES,
    FEATID_I_CPU_RX_CACHED,
    FEATID_I_CPU_TX_CACHED,
    FEATID_PROC_ABORTS,     /* depends on FEATID_SYS_SHUTDOWN_DRV */
    FEATID_UART_DISABLE_FORCED,

    FEATID_GIC,
    FEATID_LOCAL_TIMER,
    FEATID_SYS_SHUTDOWN_DRV,
    FEATID_PRINTF_INIT0,
    FEATID_PRINTF_INIT1,
    FEATID_PRINTF_INIT2,
    FEATID_PHY_GEM,
    FEATID_SYS_TIMER,
    FEATID_SPI_DRV,
    FEATID_RDMEM_CACHE_CONTROL,

    FEATID_SRIO_DRV,
    FEATID_SPACC_DRV,

    FEATID_DIAG_FEC,
    FEATID_DIAG_FP,
    FEATID_DIAG_PCIE,
    FEATID_DIAG_DSP,
    FEATID_DIAG_ARM,
    FEATID_DIAG_SCH,
    FEATID_DIAG_I2C,
    FEATID_DIAG_SPACC,      /* depends on FEATID_SPACC_DRV */


    FEATID_CLINE_INIT,
    FEATID_CMD_LINE_INIT,

    FEATID_SYS_INFO,

    FEATID_GENLOG,
    FEATID_SVSR_API_LOGGER,

    FEATID_END
}FeatureIDs;

typedef enum{
    CONSTID_START = (OBJECTID_START + 300),

    CONSTID_DSP_STARTID = CONSTID_START,
    CONSTID_PRACH_DSP_NUM,
    CONSTID_PRACH_DSP_ID_START,
    CONSTID_IDFT_FP_ID_START,
    CONSTID_IDFT_FP_ID_STOP,
    CONSTID_IFFT_FP_ID_START,
    CONSTID_IFFT_FP_ID_STOP,
    CONSTID_FFT_FP_ID_START,
    CONSTID_FFT_FP_ID_STOP,

    CONSTID_END
}ConstIDs;

#define OBJECTID_END  CONSTID_END

enum{
    DEVID_IDX = 0,
    MEMID_IDX,
    FEATID_IDX,
    CONSTID_IDX
};

typedef struct{
   UINT32 ID;
    UINT32 Value;
    UINT32 (* pHandler) (void);
}ObjectInfo;

extern const ObjectInfo* T4KInfo[];
extern const ObjectInfo* T3KInfo[];

#define MEMID_SIZE_DYNAMIC  1
#define DEVID_MAX_DSP_CORE       10

#define DEVINFO_USE(val) (val)

#define DEVINFO_GET_HWID (HWID_T4K)

// return ObjectInfo**
#define DEVINFO_GET_TABLE ((DEVINFO_GET_HWID == HWID_T4K)? \
        T4KInfo : ((DEVINFO_GET_HWID == HWID_T3K)? T3KInfo : T4KInfo))

// return ObjectInfo*
#define DEVINFO_GET_TABLE_ROW(Type) ((DEVINFO_GET_HWID == HWID_T4K)? \
        T4KInfo[Type] : ((DEVINFO_GET_HWID == HWID_T3K)? T3KInfo[Type] : T4KInfo[Type]))

// return ObjectInfo
// be careful to list all the described fields at the table if this macro is used !
#define DEVINFO_GET_TABLE_CELL(Type, Idx) ((DEVINFO_GET_HWID == HWID_T4K)? \
        T4KInfo[Type][(Idx) - 100*(Type) - OBJECTID_START] : \
            ((DEVINFO_GET_HWID == HWID_T3K)? T3KInfo[Type][(Idx) - 100*(Type) - OBJECTID_START] : \
                T4KInfo[Type][(Idx) - 100*(Type) - OBJECTID_START]))

// return ObjectInfo.Value
// be careful to list all the described fields at the table if this macro is used !
#define DEVINFO_GET_TABLE_VAL(Type, Idx) ((DEVINFO_GET_HWID == HWID_T4K)? \
        T4KInfo[Type][(Idx) - 100*(Type) - OBJECTID_START].Value : \
            ((DEVINFO_GET_HWID == HWID_T3K)? T3KInfo[Type][(Idx) - 100*(Type) - OBJECTID_START].Value : \
                T4KInfo[Type][(Idx) - 100*(Type) - OBJECTID_START].Value))

MXRC   DevInfoInit (void);
UINT32 DevInfoGetHWID(void);
UINT32 DevInfoIsEnabled (UINT32 ObjectID);
UINT32 DevInfoGetSize (MemIDs ObjectID);
UINT32 DevInfoGetNumber (DevIDs ObjectID);
UINT32 DevInfoGetValue (ConstIDs ObjectID);
UINT32 DevInfoIsT4Kx7 (void);
UINT32 DevInfoIsT4Kx6 (void);
UINT32 DevInfoIsT4Kx4 (void);
UINT32 DevInfoIsT2K (void);
UINT32 DevInfoGetDevRef (void);
UINT32 DevInfoReadID (void);

#endif

#ifdef __cplusplus
}
#endif


//-------------------------------------------------------------------------------------------
/** @file syscorecfg.h
 *
 * @brief Syscore configuration types and API
 * @author Mindspeed Technologies
 * @version $Revision: 1.15 $
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

#ifndef _SYS_CORE_CFG_H_
#define _SYS_CORE_CFG_H_

#include "4gmx.h"
#include "config.h"
#include "appids.h"
#include "FECdrv.h"
#include "rexcomp.h"

#define RC_SYSCORE_CFG_CPU_N_INACTIVE           DEF_USRRC (APP_MODULE, CORE_CFG, 1)
#define RC_SYSCORE_CFG_CEVA_POOL_A_ERROR        DEF_USRRC (APP_MODULE, CORE_CFG, 2)
#define RC_SYSCORE_CFG_CEVA_POOL_B_ERROR        DEF_USRRC (APP_MODULE, CORE_CFG, 3)
#define RC_SYSCORE_CFG_TIMEOUT                  DEF_USRRC (APP_MODULE, CORE_CFG, 4)

#define SYSCORE_CFG_TIMEOUT     (6 * 1000 * 150000) // 6 seconds

#define SYSCORE_SYS_MDMA_DRV            (1 << 0)
#define SYSCORE_SPU_MDMA_DRV            (1 << 1)
#define SYSCORE_FEC_DL_DRV              (1 << 2)
#define SYSCORE_FEC_UL_DRV              (1 << 3)
#define SYSCORE_FEC_ROBUST_CTRL         (1 << 4)
#define SYSCORE_CEVA_DRV                (1 << 5)
#define SYSCORE_FP_DRV                  (1 << 6)
#define SYSCORE_SCHEDULER               (1 << 7)
#define SYSCORE_RAD_MDMA_DRV            (1 << 8)
#define SYSCORE_SYS_MDMA_ROBUST_CTRL    (1 << 9)
#define SYSCORE_SPU_MDMA_ROBUST_CTRL    (1 << 10)
#define SYSCORE_RAD_MDMA_ROBUST_CTRL    (1 << 11)

typedef struct _SysCoreCfg_
{
    UINT32           RexCfg;                // REX configuration flags, see: REX_CFG_STAT_xxx
    UINT32           CpuDrivers [CPU_NUM];  // the mask of drivers allocation
    PSCHEDSTRC       PtrSchedCfg;
    UINT32           FpMode;
    PFEC_CONFIG_REGS PtrFecCfg;
    UINT32           CevaSpecPool [2];

}SYSCORECFG, *PSYSCORECFG;


/** @brief This function is designed to provide flexible syscore configuration
           and can be optionally called by application to
           make some reconfiguration needed for the application

    @param pCfg [in] - the pointer to the configuration

    @return [MXRC] an error code */

MXRC SysCoreConfig (SYSCORECFG * pCfg);

/** @brief This function shuts down the drivers

    @param pCfg [in] - the pointer to the configuration

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SysCoreShutdown (SYSCORECFG * pCfg);

#endif //_SYS_CORE_CFG_H_


#ifdef __cplusplus
}
#endif


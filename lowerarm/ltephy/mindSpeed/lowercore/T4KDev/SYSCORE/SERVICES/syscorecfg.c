//-------------------------------------------------------------------------------------------
/** @file syscorecfg.c
 *
 * @brief Syscore configuration types and API
 * @author Mindspeed Technologies
 * @version $Revision: 1.28 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "syscorecfg.h"
#include "mdmadrv.h"
#include "FECdrv.h"
#include "fpdrv.h"
#include "DSPdrv.h"
#include "initcpu.h"

typedef struct _DRV_RECONFIG_
{
    UINT32  SysCoreDrvID;       // See masks: SYSCORE_xxx of syscorecfg.h file
    MXRC   (*InitEntry) (void);
    MXRC   (*StopEntry) (void);
    UINT32 (*MasterCpu) (void);

}DrvReconfig;

MXRC  SchedInit(void);
MXRC  FpInit(void);
MXRC  FecInitDL(void);
MXRC  FecInitUL(void);

static DrvReconfig ReconfigTable [] =
{
        {SYSCORE_FP_DRV,                FpInit,             MapDrvShutdown,         MapDrvGetMasterCpu              },

        {SYSCORE_CEVA_DRV,              DspDrvInit,         DspDrvShutdown,         DspDrvGetMasterCpu              },
        {SYSCORE_SCHEDULER,             SchedInit,          RexShutdown,            RexGetMasterCpu                 },

        {SYSCORE_FEC_DL_DRV,            FecInitDL,          FecDlShutdown,          FecDLGetMasterCpu               },
        {SYSCORE_FEC_UL_DRV,            FecInitUL,          FecUlShutdown,          FecULGetMasterCpu               },
        {SYSCORE_FEC_ROBUST_CTRL,       FecRobustInit,      FecRobustShutdown,      FecRobustGetMasterCpu           },

        {SYSCORE_SYS_MDMA_DRV,          MdmaSysInit,        MdmaSysShutdown,        MdmaSysGetMasterCpu             },
        {SYSCORE_SPU_MDMA_DRV,          MdmaSpuInit,        MdmaSpuShutdown,        MdmaSpuGetMasterCpu             },
        {SYSCORE_RAD_MDMA_DRV,          MdmaRadInit,        MdmaRadShutdown,        MdmaRadGetMasterCpu             },

        {SYSCORE_SYS_MDMA_ROBUST_CTRL,  MdmaSysRobustInit,  MdmaSysRobustShutdown,  MdmaSysRobustGetMasterCpu       },
        {SYSCORE_SPU_MDMA_ROBUST_CTRL,  MdmaSpuRobustInit,  MdmaSpuRobustShutdown,  MdmaSpuRobustGetMasterCpu       },
        {SYSCORE_RAD_MDMA_ROBUST_CTRL,  MdmaRadRobustInit,  MdmaRadRobustShutdown,  MdmaRadRobustGetMasterCpu       },

        {0,                             NULL,               NULL,                   NULL                            }
};

VUINT32      NCNBVAR CfgComplete = 0;
VUINT32      NCNBVAR CfgError    = 0;
SYSCORECFG * NCNBVAR CfgParams   = NULL;

MXRC  SchedInit(void)
{
    REXCFG cfg;
    UINT32 cevas;

    memset (&cfg, 0, sizeof (cfg));

    cevas = DspDrvGetDevMask ();

    if ((cevas & CfgParams->CevaSpecPool[0]) != CfgParams->CevaSpecPool[0])
    {
        uart_printf ("CEVA pool-A is configured wrong. Required mask is [%x], accessible [%x]\r\n",
            CfgParams->CevaSpecPool[0],
            cevas
        );
        return RC_SYSCORE_CFG_CEVA_POOL_A_ERROR;
    }

    if ((cevas & CfgParams->CevaSpecPool[1]) != CfgParams->CevaSpecPool[1])
    {
        uart_printf ("CEVA pool-B is configured wrong. Required mask is [%x], accessible [%x]\r\n",
            CfgParams->CevaSpecPool[1],
            cevas
        );
        return RC_SYSCORE_CFG_CEVA_POOL_B_ERROR;
    }

    cevas &= ~CfgParams->CevaSpecPool[0];
    cevas &= ~CfgParams->CevaSpecPool[1];

    cfg.Res [RESOURCE_CEVA]            = cevas;
    cfg.Res [REX_RESOURCE_CEVA_POOL_A] = CfgParams->CevaSpecPool[0];
    cfg.Res [REX_RESOURCE_CEVA_POOL_B] = CfgParams->CevaSpecPool[1];

	cfg.Res [RESOURCE_MAP]  = 1;	// 1 MAP

    cfg.Ctrl = CfgParams->RexCfg;   // REX configuration flags

#ifdef REX_S_FORCED_ENABLED
    cfg.Ctrl |= REX_CFG_STAT_SERV_ENABLED;
#endif
    return RexInit(&cfg);
}

MXRC  FpInit(void)
{
    MXRC rc;
    UINT32 nCfg = 0;

    if (CfgParams->RexCfg & REX_CFG_IDFT_ON_CEVA)
    {
        nCfg |= FP_DRV_CFG_IDFT_ON_CEVA;
    }

    if (FAILED(rc = MapDrvInit (CfgParams->FpMode, nCfg)))
    {
        return rc;
    }

    return MapDrvRestoreMapMask();
}

MXRC  FecInitUL(void)
{
    return FecUlInit (CfgParams->PtrFecCfg);
}

MXRC  FecInitDL(void)
{
    return FecDlInit (CfgParams->PtrFecCfg);
}

MXRC CfgStopDrivers (SYSCORECFG * pCfg)
{
    UINT32 nCpuID = MxGetCpuID ();
    UINT32 i = 0;
    MXRC rc = MX_OK;

    MxDMB();
    MxDSB();

    while (ReconfigTable [i].SysCoreDrvID != 0)
    {
        _ASSERT_PTR(ReconfigTable [i].MasterCpu);

        // if driver was started on this core

        if (ReconfigTable [i].MasterCpu () == nCpuID)
        {
            // and driver is reallocated to other core
            // to run driver shutdown routine

            if ((pCfg->CpuDrivers [nCpuID] & ReconfigTable [i].SysCoreDrvID) != 0)
            {
                _ASSERT_PTR(ReconfigTable [i].StopEntry);

                rc = ReconfigTable [i].StopEntry ();

                _ASSERT_RC(rc);

                if (FAILED(rc))
                    return rc;
            }
        }

        i++;
    }

    MxDMB();
    MxDSB();

    return rc;
}

MXRC CfgStartDrivers (SYSCORECFG * pCfg)
{
    UINT32 nCpuID = MxGetCpuID ();
    UINT32 i = 0;
    MXRC rc = MX_OK, total_rc = 0;

    MxDMB();
    MxDSB();

    while (ReconfigTable [i].SysCoreDrvID != 0)
    {
        if ((pCfg->CpuDrivers [nCpuID] & ReconfigTable [i].SysCoreDrvID) != 0)
        {
            _ASSERT_PTR(ReconfigTable [i].InitEntry);

            rc = ReconfigTable [i].InitEntry ();

            _ASSERT_RC(rc);

            if (FAILED(rc) && !FAILED(total_rc))
            {
                total_rc = rc;
            }
        }

        i++;
    }

    MxDMB();
    MxDSB();

    return total_rc;
}

UINT32 CfgStartThread (LPVOID pCfg)
{
    CfgError = CfgStartDrivers ((SYSCORECFG *)pCfg);

    CfgComplete = TRUE;

    return 0;
}

UINT32 CfgStopThread (LPVOID pCfg)
{
    CfgError = CfgStopDrivers ((SYSCORECFG *)pCfg);

    CfgComplete = TRUE;

    return 0;
}

BOOL CfgWaitComplete(void)
{
    UINT32 t = MxGetTicks();

    while (CfgComplete == FALSE)
    {
        if (MxGetTicks() - t > SYSCORE_CFG_TIMEOUT)
            return FALSE;
    }

    MxDMB();
    MxDSB();

    return TRUE;
}

/** @brief This function is designed to provide flexible syscore configuration
           and can be optionally called by application to
           make some reconfiguration needed for the application

    @param pCfg [in] - the pointer to the configuration

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SysCoreConfig (SYSCORECFG * pCfg)
{
    UINT32 i;
    volatile HANDLE hThr;
    MXRC rc;
    UINT32 nCpuID = MxGetCpuID ();
    UINT32 nCpuNum;

    _ASSERT_PTR (pCfg);

    CfgParams = pCfg;

    nCpuNum = SysGetCpuNum ();

    for (i = 0; i < nCpuNum; i++)
    {
        if (i != nCpuID)
        {
            if (!MxICIsSystemInit (i))
                return RC_SYSCORE_CFG_CPU_N_INACTIVE;

            if (FAILED(rc = MxCreateThreadEx (i, CfgStopThread, NULL, pCfg, 0, (HANDLE*)&hThr)))
            {
                _ASSERT_RC (rc);

                return rc;
            }

            CfgComplete = FALSE;
            CfgError = MX_OK;

            if(FAILED(rc = MxScheduleThread (hThr)))
            {
                _ASSERT_RC (rc);
                return rc;
            }

            if (!CfgWaitComplete())
            {
                uart_printf ("SysCfg: Stop Drv timeout, dst-cpu=%d\r\n", i);

                rc = RC_SYSCORE_CFG_TIMEOUT;
                _ASSERT_RC(rc);
                return rc;
            }

            if(FAILED(rc = MxRemoveThread (hThr)))
            {
                _ASSERT_RC(rc);
                return rc;
            }

            if (FAILED(CfgError))
            {
               _ASSERT_RC(CfgError);
               return CfgError;
            }

        }
        else
        {
            if (FAILED(rc = CfgStopThread (pCfg)))
            {
                _ASSERT_RC(rc);
                return rc;
            }
        }
    }


    // to run drivers initialization

    for (i = 0; i < nCpuNum; i++)
    {
        if ((pCfg->CpuDrivers[i] != 0))
        {
            if (i != nCpuID)
            {
                if (!MxICIsSystemInit (i))
                    return RC_SYSCORE_CFG_CPU_N_INACTIVE;

                if (FAILED(rc = MxCreateThreadEx (i, CfgStartThread, NULL, pCfg, 0, (HANDLE*)&hThr)))
                {
                    _ASSERT_RC (rc);

                    return rc;
                }

                CfgComplete = FALSE;
                CfgError = MX_OK;

                if(FAILED(rc = MxScheduleThread (hThr)))
                {
                    _ASSERT_RC (rc);
                    return rc;
                }

                if (!CfgWaitComplete())
                {
                    uart_printf ("SysCfg: Run Drv timeout, dst-cpu=%d\r\n", i);
                    rc = RC_SYSCORE_CFG_TIMEOUT;
                    _ASSERT_RC(rc);
                    return rc;
                }

                if(FAILED(rc = MxRemoveThread (hThr)))
                {
                    _ASSERT_RC(rc);
                    return rc;
                }

                if (FAILED(CfgError))
                {
                   _ASSERT_RC(CfgError);
                   return CfgError;
                }

            }
            else
            {
                if (FAILED(rc = CfgStartThread (pCfg)))
                {
                    _ASSERT_RC(rc);
                    return rc;
                }
            }
        }
    }

    return MX_OK;
}


/** @brief This function shuts down the drivers

    @param pCfg [in] - the pointer to the configuration

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SysCoreShutdown (SYSCORECFG * pCfg)
{
    UINT32 i;
    volatile HANDLE hThr;
    MXRC rc;
    UINT32 nCpuID = MxGetCpuID ();
    UINT32 nCpuNum;

    _ASSERT_PTR (pCfg);

    CfgParams = pCfg;

    nCpuNum = SysGetCpuNum ();

    for (i = 0; i < nCpuNum; i++)
    {
        if (i != nCpuID)
        {
            if (!MxICIsSystemInit (i))
                return RC_SYSCORE_CFG_CPU_N_INACTIVE;

            if (FAILED(rc = MxCreateThreadEx (i, CfgStopThread, NULL, pCfg, 0, (HANDLE*)&hThr)))
            {
                _ASSERT_RC (rc);

                return rc;
            }

            CfgComplete = FALSE;
            CfgError = MX_OK;

            if(FAILED(rc = MxScheduleThread (hThr)))
            {
                _ASSERT_RC (rc);
                return rc;
            }

            if (!CfgWaitComplete())
            {
                uart_printf ("SysCfg: Stop Drv timeout, dst-cpu=%d\r\n", i);

                rc = RC_SYSCORE_CFG_TIMEOUT;
                _ASSERT_RC(rc);
                return rc;
            }

            if(FAILED(rc = MxRemoveThread (hThr)))
            {
                _ASSERT_RC(rc);
                return rc;
            }

            if (FAILED(CfgError))
            {
               _ASSERT_RC(CfgError);
               return CfgError;
            }

        }
        else
        {
            if (FAILED(rc = CfgStopThread (pCfg)))
            {
                _ASSERT_RC(rc);
                return rc;
            }
        }
    }
    return MX_OK;
}




//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL dataint.c
 *
 * @brief Communication data interface, it's developed to unify
*       different interfaces like CPRI/PCIe/sRIO
 * @author Mindspeed Technologies
 * @version $Revision: 1.33 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "dataint.h"
#include "appprintf.h"
#include <stdlib.h>
#include "globalstoragearrays.h"
#include "mdmadrv.h"
#include "supervisor.h"
#include "ad9361radio.h"
#include "dejitter.h"

static DiCtx  di_ctx;

INLINE PDiCtx DiGetCtx (void)
{
    return &di_ctx;
}

UINT32 DiGetCPRIDevNum (PCPRIDRVCTX pCpriDrvCtx)
{
    UINT32 DevID = 0, EnabledDev = 0;

    while (DevID < CPRI_DEV_NUM)
    {
        if (pCpriDrvCtx->DevEnaMask & (1<<DevID))
        {
            EnabledDev ++;
        }

        DevID ++;
    }

    return EnabledDev;
}

UINT32 DiCPRIInit(PCPRIDRVCTX pCpriDrvCtx)
{
    PDiCtx pCtx = DiGetCtx();
    CPRICTX * pCpriCtx;
    UINT32 DevID = 0;
    PUINT8 *pTxLog, *pRxLog;
    UINT32 TxLogSize, RxLogSize, i;

    _ASSERT_PTR (pCtx);
    _ASSERT_PTR (pCpriDrvCtx);

    pTxLog = pCtx->TxLog;
    pRxLog = pCtx->RxLog;

    TxLogSize = pCtx->TxLogSize;// / pCtx->nTxAntenas;
    RxLogSize = pCtx->RxLogSize;// / pCtx->nRxAntenas;

    DevID = 0;

    while(DevID < CPRI_DEV_NUM)
    {
        if (pCpriDrvCtx->DevEnaMask & (1<<DevID))
        {
            pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];

            if (pRxLog != NULL)
            {
                pCpriCtx->nRXAntenas = pCtx->nRxAntenas;
                for(i = 0; i < pCtx->nRxAntenas; i++)
                {
                    pCpriCtx->pRxLogBuf[i] = pRxLog[i];
                    pCpriCtx->RxLogBufPos[i] = 0;
                }

                pCpriCtx->RxLogBufSize = RxLogSize;
                pCpriCtx->RxLogBufRoudupPos = pCtx->RxLogRoundupPos;
                pCpriCtx->RxLogFreezeNum = pCtx->RxLogFreezeNum;

            }

            if (pTxLog != NULL)
            {
                pCpriCtx->nTXAntenas = pCtx->nTxAntenas;
                for(i = 0; i < pCtx->nTxAntenas; i++)
                {
                    pCpriCtx->pTxLogBuf[i] = pTxLog[i];
                    pCpriCtx->TxLogBufPos[i] = 0;
                }
                pCpriCtx->TxLogBufSize = TxLogSize;
                pCpriCtx->TxLogFreezeNum = pCtx->TxLogFreezeNum;

            }

            pCpriCtx->LogCtrl = (pCtx->LogCtrl & CPRI_LOG_CTRL_MASK);

            if ((pCpriCtx->LogCtrl & DI_LOG_TX0_LOG_ENABLED) || (pCpriCtx->LogCtrl & DI_LOG_TX1_LOG_ENABLED))
                CPRI_TX_LOG_ENABLE(pCpriCtx);
            else
                CPRI_TX_LOG_DISABLE(pCpriCtx);

            if ((pCpriCtx->LogCtrl & DI_LOG_RX0_LOG_ENABLED) || (pCpriCtx->LogCtrl & DI_LOG_RX1_LOG_ENABLED))
                CPRI_RX_LOG_ENABLE(pCpriCtx);
            else
                CPRI_RX_LOG_DISABLE(pCpriCtx);

            // Installing CPRI callbacks

            pCpriCtx->pRxDMACallBack = pCtx->RxCb;
            pCpriCtx->pTxDMACallBack = pCtx->TxCb;

#if 0
            pCpriCtx->RxPtr = pCtx->RxCbPtr;
            pCpriCtx->TxPtr = pCtx->TxCbPtr;
#else
            pCpriCtx->RxPtr = pCpriCtx;
            pCpriCtx->TxPtr = pCpriCtx;
#endif

        }

        DevID ++;
    }

    return 0;
}

MXRC DiOpenCpri (PDiCtx pCtx, PDiConfig pCfg, BOOL bEmul, DIHANDLE * pOutHandle)
{
    MXRC rc = MX_OK;
    CPRICONFIG * pCpri;

    pCpri = &pCfg->DrvCfg.CpriCfg;

    if (bCpriConfigByHost)
    {
        pCpri->NumCpriDev       = CpriConfigByHost.NumCpriDev;
        pCpri->SamplingRate     = CpriConfigByHost.SamplingRate;
        pCpri->SampleWidth      = CpriConfigByHost.SampleWidth;
        pCpri->RecMacAddr[0]    = pCpri->ReMacAddr[0] = CpriConfigByHost.RecMacAddr[0];
        pCpri->RecMacAddr[1]    = pCpri->ReMacAddr[1] = CpriConfigByHost.RecMacAddr[1];
        pCpri->RecMacAddr[2]    = pCpri->ReMacAddr[2] = CpriConfigByHost.RecMacAddr[2];
        pCpri->RecMacAddr[3]    = pCpri->ReMacAddr[3] = CpriConfigByHost.RecMacAddr[3];
        pCpri->RecMacAddr[4]    = pCpri->ReMacAddr[4] = CpriConfigByHost.RecMacAddr[4];
        pCpri->RecMacAddr[5]    = CpriConfigByHost.RecMacAddr[5];
        pCpri->ReMacAddr[5]     = CpriConfigByHost.ReMacAddr[5];
        pCpri->RecIpAddr        = CpriConfigByHost.RecIpAddr;
        pCpri->ReIpAddr         = CpriConfigByHost.ReIpAddr;
        pCpri->EthEnable        = CpriConfigByHost.EthEnable;
        pCpri->EthLittleEndian  = CpriConfigByHost.EthLittleEndian;
        pCpri->EthFcsEnable     = CpriConfigByHost.EthFcsEnable;
        pCpri->EthRate          = CpriConfigByHost.EthRate;
        pCpri->HdlcEnable       = CpriConfigByHost.HdlcEnable;
        pCpri->MaxAcNum         = CpriConfigByHost.MaxAcNum;
        pCpri->CpriRate         = CpriConfigByHost.CpriRate;
    }
    else
    {
        pCpri->NumCpriDev    = 1;
        pCpri->SamplingRate  = CPRI_15_36_MSPS;
        pCpri->SampleWidth   = 15;
        pCpri->RecMacAddr[0] = pCpri->ReMacAddr[0] = 0x00;
        pCpri->RecMacAddr[1] = pCpri->ReMacAddr[1] = 0x11;
        pCpri->RecMacAddr[2] = pCpri->ReMacAddr[2] = 0x22;
        pCpri->RecMacAddr[3] = pCpri->ReMacAddr[3] = 0x33;
        pCpri->RecMacAddr[4] = pCpri->ReMacAddr[4] = 0x44;
        pCpri->RecMacAddr[5] = 0x55;
        pCpri->ReMacAddr[5]  = 0x66;
        pCpri->RecIpAddr     = 0xc0a80100;
        pCpri->ReIpAddr      = 0xc0a80101;
        pCpri->EthEnable     = 1;
        pCpri->EthLittleEndian = 1;
        pCpri->EthFcsEnable  = 1;
        pCpri->EthRate       = 0x3f;
        pCpri->HdlcEnable    = 0;
        pCpri->MaxAcNum      = 1;
    }

    if ((pCfg->DrvCtrl & DI_DRV_CTRL_CPRI_OPTICAL_LOOPBACK) && (pCfg->DrvCtrl & DI_DRV_CTRL_CPRI_RADIOCOMP))
    {
        uart_printf("Mode: DI_DRV_CTRL_CPRI_OPTICAL_LOOPBACK && DI_DRV_CTRL_CPRI_RADIOCOMP\r\n");
        pCpri->MaxAcNum     = (bCpriConfigByHost) ? CpriConfigByHost.MaxAcNum : 2;
        pCpri->SampleWidth  = (bCpriConfigByHost) ? CpriConfigByHost.SampleWidth : 16;
        pCpri->CpriConfType = CPRI_RADIOCOMP;
        pCpri->CpriRate = (bCpriConfigByHost) ? CpriConfigByHost.CpriRate : CPRIRATE_3072; // CPRIRATE_1228

        //CpriConf.pCPRICreateCallBack = cbCPRIInitRingBuffer;
    }
    else if (pCfg->DrvCtrl & DI_DRV_CTRL_CPRI_RADIOCOMP)
    {
        uart_printf("Mode: DI_DRV_CTRL_CPRI_RADIOCOMP \r\n");
        pCpri->SampleWidth  = (bCpriConfigByHost) ? CpriConfigByHost.SampleWidth : 16;
        pCpri->MaxAcNum     = (bCpriConfigByHost) ? CpriConfigByHost.MaxAcNum : 2;
        pCpri->CpriConfType = CPRI_RADIOCOMP;
        pCpri->CpriRate = (bCpriConfigByHost) ? CpriConfigByHost.CpriRate : CPRIRATE_3072; /*CPRIRATE_2457*/
        pCpri->RxIrqGpioEna =1;
        pCpri->RxIrqGpioPort = 12;

        //CpriConf.pCPRICreateCallBack = cbCPRIInitPhyInterface;
    }
    else if (pCfg->DrvCtrl & DI_DRV_CTRL_CPRI_10MHZ_MIMO_2AC)
    {
        uart_printf("Mode: DI_DRV_CTRL_CPRI_10MHZ_MIMO_2AC \r\n");
        pCpri->SampleWidth  = (bCpriConfigByHost) ? CpriConfigByHost.SampleWidth : 15;
        pCpri->MaxAcNum     = 2;
        pCpri->CpriConfType = CPRI_MIMO_2AC;
        pCpri->CpriRate = (bCpriConfigByHost) ? CpriConfigByHost.CpriRate : CPRIRATE_2457; // CPRIRATE_1228

        //CpriConf.pCPRICreateCallBack = cbCPRIInitPhyInterface;

    }
    else if (pCfg->DrvCtrl & DI_DRV_CTRL_CPRI_20MHZ_1AC)
    {
        uart_printf("Mode: DI_DRV_CTRL_CPRI_20MHZ_1AC \r\n");
        pCpri->SamplingRate = (bCpriConfigByHost) ? CpriConfigByHost.SamplingRate : CPRI_30_72_MSPS;
        pCpri->SampleWidth  = (bCpriConfigByHost) ? CpriConfigByHost.SampleWidth : 16;
        pCpri->CpriConfType = CPRI_IXIA;
        pCpri->CpriRate = (bCpriConfigByHost) ? CpriConfigByHost.CpriRate : CPRIRATE_3072; // CPRIRATE_1228

        //CpriConf.pCPRICreateCallBack = cbCPRIInitPhyInterface;
    }
    else
    {
        if (!bEmul)
        {
            uart_printf("Mode: %s \r\n", (pCfg->DrvCtrl & DI_DRV_CTRL_CPRI_OPTICAL_LOOPBACK) ? "DI_DRV_CTRL_CPRI_OPTICAL_LOOPBACK" : "DI_DRV_CTRL_CPRI_IXIA");
            pCpri->CpriRate = (bCpriConfigByHost) ? CpriConfigByHost.CpriRate : CPRIRATE_2457; // CPRIRATE_1228
            pCpri->CpriConfType = CPRI_IXIA;
            pCpri->SampleWidth = (bCpriConfigByHost) ? CpriConfigByHost.SampleWidth : 15;
        }
    }


    pCpri->pCPRICreateCallBack = DiCPRIInit;
    pCpri->UseRxAntenna = pCfg->UseRxAntenna;
    pCpri->UseTxAntenna = pCfg->UseTxAntenna;

    pCpri->UpDelay = pCtx->UpDelay;
    pCpri->RxStartDelayCounter = pCtx->RxStartDelayCounter;

    if (pCtx->DrvCtrl & DI_DRV_CTRL_CPRI_SFP_ENABLED)
    {
        uart_printf ("SFP initialization ... ");

        if (FAILED(rc = SrdsSfpCtrl(1, 3)))
        {
            uart_printf ("FAILED, rc=0x%x\r\n", rc);
            return rc;
        }

        uart_printf ("OK\r\n");
    }

    if (!bEmul)
    {
        if (pCfg->DrvCtrl & DI_DRV_CTRL_CPRI_OPTICAL_LOOPBACK)
            CpriSrds1Init(pCpri->CpriRate, 0, 1);
        else
            CpriSrds1Init(pCpri->CpriRate, 0, 0);
    }

    if (bEmul)
    {
        pCpri->CpriConfType = CPRI_EMULATOR;
        pCpri->EmulTickTime = pCfg->EmulTickTime;
    }

    pCpri->DiUsed = 1;

    if (FAILED(rc = CPRIDrvInit(pCpri)))
    {
        uart_printf ("CPRI driver initialization faild,rc=0x%08x\r\n", rc);
        return rc;
    }

    if (pCfg->DrvCtrl & DI_DRV_CTRL_HW_LB)
    {
        CPRIDrvSetLB (0, 1);
    }

    (*pOutHandle) = (LPVOID)CPRIDrvGetCtx();

    CPRIDrvEnableDma(0);

    return rc;
}

MXRC DiOpenPCIe (PDiCtx pCtx, PDiConfig pCfg, DIHANDLE * pOutHandle)
{
    EvmCfg cfg;
    MXRC rc = MX_OK;
    UINT32 i = 0;

    memset (&cfg, 0, sizeof (cfg));

    cfg.AntennaNum      = pCfg->nTxAntenas;
    cfg.IntProfID       = pCtx->EvmFpgaIntProfID;

    cfg.CtrlMask        |= EVM_FPGA_CTRL_AUTO_RUN;
    cfg.CtrlMask        |= (pCfg->LogCtrl & DI_LOG_OPT_MASK);

    //cfg.CtrlMask      |= EVM_FPGA_CTRL_CHECK;
    //cfg.CtrlMask      |= EVM_FPGA_CTRL_TX_LOG;
    //cfg.CtrlMask      |= EVM_FPGA_CTRL_RX_LOG;
    //cfg.CtrlMask      |= EVM_FPGA_CTRL_RX_LOG_RR;
    //cfg.CtrlMask      |= EVM_FPGA_CTRL_TX_LOG_RR;

    if (pCfg->DrvCtrl & DI_DRV_CTRL_ADI_PCIE_L4)
    {
        cfg.PcieLines = 4;
    }
    else if (pCfg->DrvCtrl & DI_DRV_CTRL_ADI_PCIE_L2)
    {
        cfg.PcieLines = 2;
    }
    else if (pCfg->DrvCtrl & DI_DRV_CTRL_ADI_PCIE_L1)
    {
        cfg.PcieLines = 1;
    }
    else
    {
        cfg.PcieLines = 0; //1;
    }

    cfg.pRxCbCtx        = pCfg->RxCbPtr;
    cfg.pRxDMACallBack  = pCfg->RxCb;

    cfg.pTxCbCtx        = pCfg->TxCbPtr;
    cfg.pTxDMACallBack  = pCfg->TxCb;

    for (i = 0; i < 4; i++)
    {
        cfg.pTxLog[i] = pCfg->TxLog[i];
        cfg.pRxLog[i] = pCfg->RxLog[i];
    }

    cfg.TxLogSize   = pCfg->TxLogSize;
    cfg.RxLogSize   = pCfg->RxLogSize;

    rc = EvmFpgaInit(&cfg);

    if (FAILED(rc))
    {
        printf ("PCIe:RC/FPGA-PCIe:EP/Radio is failed, rc=0x%x\r\n", rc);
        return rc;
    }

    return rc;
}

APIHEC DiApiHandler (LPVOID pCtx, ApiHeader * pApi, ApiHeader * pResp)
{
    MXRC rc = RC_DI_UNKNOWN_CMD;
    UINT32 DejitterOn;

    // Control interface initialization
    if (IS_CFG_API(pApi, DI_AD9361_CTRL))
    {
        UINT32 profID = ApiGetParamU32(pApi, 1, 0);
        UINT32 tx_num = ApiGetParamU32(pApi, 3, 1);
        UINT32 rx_num = ApiGetParamU32(pApi, 4, 1);
        UINT32 lb     = ApiGetParamU32(pApi, 5, 0);
    	UINT32 rxfreq = ApiGetParamU32(pApi, 6, 0);
		UINT32 txfreq = ApiGetParamU32(pApi, 7, 0);

        uart_printf ("AD9361 initialization, band=%d, tx-annt=%d, rx-annt=%d, lb=%d, rxfreq=%d, txfreq=%d...\r\n", profID, tx_num, rx_num, lb, rxfreq, txfreq);

        if (FAILED(rc = Ad9361RadioInit (profID, tx_num, rx_num, lb, rxfreq, txfreq)))
        {
            uart_printf ("error,rc=%x\r\n", rc);
            ApiAddErrorCode(pResp, rc);
            return APIHEC_OK;
        }

        uart_printf ("OK\r\n");
        DejitterOn = ApiGetParamU32(pApi, 2, 0);

        if(DejitterOn == 1)
        {
            uart_printf ("DeJitter initialization ... ");

            if (FAILED(rc = DejitterInit(DEJITTER_INIT_RADIO, AD9548_CLK_SRC_REFD)))
            {
                uart_printf ("error,rc=%x\r\n", rc);
                ApiAddErrorCode(pResp, rc);
                return APIHEC_OK;
            }
            uart_printf ("OK\r\n");
        }

    }
    else if(IS_CFG_API(pApi, DI_EVMFPGA_GPIO_CTRL))
    {
        rc = EvmFpgaCfgApi(pApi, pResp);
    }
    else if (IS_CFG_API(pApi, DI_DEALY_CTRL))
    {
        if(pCtx)
        {
            ((PDiCtx)pCtx)->UpDelay = ApiGetParamU32(pApi, 1, 0);
            ((PDiCtx)pCtx)->RxStartDelayCounter = ApiGetParamU32(pApi, 2, 0);
            uart_printf("up delay %d, rx start delay %d\r\n", ((PDiCtx)pCtx)->UpDelay, ((PDiCtx)pCtx)->RxStartDelayCounter);
        }

        rc = MX_OK;
    }
    else if (IS_CFG_API(pApi, DI_LOG_MASK))
    {
        ApiParam * pParam;
        PDiCtx pCtx = DiGetCtx ();
        _ASSERT_PTR (pCtx);

        pParam = ApiGetParamByID (pApi, 2);

        // to disable the host provided mask

        if (pParam == NULL)
        {
            pCtx->HostLogMaskEnabled = 0;
            pCtx->HostLogMaskValue   = 0;

            uart_printf ("DI: Forced log mask provided by the host is disabled\r\n");
        }
        else
        {
            pCtx->HostLogMaskEnabled = 1;
            pCtx->HostLogMaskValue   = pParam->ParamData[0] & DI_LOG_OPT_MASK;

            uart_printf ("DI: Forced log mask provided by the host is enabled, mask=[0x%x]\r\n", pCtx->HostLogMaskValue);
        }

        rc = MX_OK;

    }
    else if (IS_QRY_API(pApi, DI_LOG_MASK))
    {
        PDiCtx pCtx = DiGetCtx ();
        _ASSERT_PTR (pCtx);

        if (pCtx->HostLogMaskEnabled)
        {
            ApiAddParam (pResp, NULL, 2, &pCtx->HostLogMaskValue, sizeof (pCtx->HostLogMaskValue));
            rc = MX_OK;
        }
        else
        {
            rc = RC_DI_MASK_IS_DISABLED;
        }
    }
    else if (IS_CFG_API(pApi, DI_SET_EVMFPGA_INT_PROF))
    {
        PDiCtx pCtx = DiGetCtx ();
        _ASSERT_PTR (pCtx);

        pCtx->EvmFpgaIntProfID = ApiGetParamU32(pApi, 2, EVM_FPGA_INT_PROF_DEF);
        rc = MX_OK;
    }
    else if (IS_QRY_API(pApi, DI_GET_CPRI_REGS))
    {
        CPRICONFIG  cfg;

        if(!((PDiCtx)pCtx)->Opened)
            rc = RC_DI_NOT_OPENED;
        else if(((PDiCtx)pCtx)->IntID == DI_CPRI_EMULATOR)
            cfg.CpriConfType = CPRI_EMULATOR;
        else if (((PDiCtx)pCtx)->IntID == DI_CPRI)
            cfg.CpriConfType = CPRI_IXIA;
        else
            rc = RC_DI_NOT_SUPPORTED;

        if(rc != RC_DI_NOT_SUPPORTED && rc != RC_DI_NOT_OPENED)
        {
            ApiAddParam (pResp, NULL, 0, CpriGetRegs(&cfg, ApiGetParamU32(pApi, 1, 0)), sizeof (CPRIREGS));
            rc = MX_OK;
        }
    }

    ApiAddErrorCode(pResp, rc);

    return APIHEC_OK;
}

/** @brief This funtion initaizes DataInterface module and it's called by
           the service manager

    @return [MXRC] an error code */

MXRC DiInit (void)
{
    MXRC rc;

    rc = SvsrRegClient (IID_DI, DiApiHandler, DiGetCtx());

    return rc;
}

/** @brief This function opens communication interface and returns interface handle

    @NOTE This function should be called first before usage of any other API of this service

    @param nDiID        [in] - the interface ID, see: DI_xxx of this file
    @param pCfg         [in] - the common configuration parameters
    @param pOutHandle   [out]- the interface handle

    @return [MXRC] an error code */

MXRC DiOpen (UINT32 nDiID, PDiConfig pCfg, DIHANDLE * pOutHandle)
{
    MXRC rc = RC_DI_UNKNOWN_INT;
    PDiCtx pCtx = DiGetCtx ();
    IRQTYPE irq;
    UINT32 i, enabled, val, profint;

    _ASSERT_PTR (pCfg);
    _ASSERT_PTR (pOutHandle);
    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable();

    enabled = pCtx->HostLogMaskEnabled;
    val     = pCtx->HostLogMaskValue;
    profint = pCtx->EvmFpgaIntProfID;

    memset (pCtx, 0, sizeof (pCtx));

    pCtx->HostLogMaskEnabled = enabled;
    pCtx->HostLogMaskValue = val;
    pCtx->EvmFpgaIntProfID = profint;

    if (pCtx->HostLogMaskEnabled)
    {
        pCfg->LogCtrl &= ~DI_LOG_OPT_MASK;
        pCfg->LogCtrl |= pCtx->HostLogMaskValue;
    }

    pCtx->LogCtrl   = pCfg->LogCtrl;
    pCtx->DrvCtrl   = pCfg->DrvCtrl;
    pCtx->TxCb      = pCfg->TxCb;
    pCtx->RxCb      = pCfg->RxCb;
    pCtx->TxCbPtr   = pCfg->TxCbPtr;
    pCtx->RxCbPtr   = pCfg->RxCbPtr;

    for (i = 0; i < 4; i++)
    {
        pCtx->TxLog[i] = pCfg->TxLog[i];
    }
    for (i = 0; i < 4; i++)
    {
        pCtx->RxLog[i] = pCfg->RxLog[i];
    }


    pCtx->TxLogSize = pCfg->TxLogSize;
    pCtx->RxLogSize = pCfg->RxLogSize;
    pCtx->RxLogRoundupPos = pCfg->RxLogRoudupPos;
    pCtx->TxLogFreezeNum = pCfg->TxLogFreezeNum;
    pCtx->RxLogFreezeNum = pCfg->RxLogFreezeNum;
    pCtx->nRxAntenas = pCfg->nRxAntenas;
    pCtx->nTxAntenas = pCfg->nTxAntenas;

    pCtx->IntID     = nDiID;

    switch (nDiID)
    {
        case DI_CPRI:
            rc = DiOpenCpri (pCtx, pCfg, 0, pOutHandle);
            break;

        case DI_CPRI_EMULATOR:
            rc = DiOpenCpri (pCtx, pCfg, 1, pOutHandle);
            break;

        case DI_PCIE:
            rc = DiOpenPCIe (pCtx, pCfg, pOutHandle);
            break;

        default:
            break;
    }

    if (SUCCESSFUL(rc))
        pCtx->Opened = 1;

    ARM_INT_restore(irq);

    return rc;
}

/** @brief This function shutdowns communication interface

    @NOTE This function should be called first before usage of any other API of this service

    @return [MXRC] an error code */

MXRC DiShutdown (void)
{
    MXRC rc = RC_DI_UNKNOWN_INT;
    PDiCtx pCtx = DiGetCtx ();
    IRQTYPE irq;

    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable();

    switch (pCtx->IntID)
    {
        case DI_CPRI:
            rc = CPRIDrvShutdown ();
            break;

        case DI_CPRI_EMULATOR:
            rc = CPRIDrvShutdown ();
            break;

        case DI_PCIE:
            rc = EvmFpgaShutdown ();
            break;

        default:
            break;
    }

    memset (pCtx, 0, sizeof (pCtx));

    ARM_INT_restore(irq);

    return rc;
}


/** @brief This function closes communication interface

    @param h [in]- the interface handle (see: DiOpen)

    @return [MXRC] an error code */

MXRC DiClose (DIHANDLE h)
{
    PDiCtx pCtx = DiGetCtx ();
    UINT32 nNumDev, i;
    IRQTYPE irq;
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);

    if (pCtx->Opened == 0)
        return RC_DI_NOT_OPENED;

    irq = ARM_INT_disable();

    //uart_printf ("DI: close channel (%d)\r\n", pCtx->IntID);

    switch (pCtx->IntID)
    {
        case DI_CPRI:
        case DI_CPRI_EMULATOR:

            nNumDev = DiGetCPRIDevNum ((PCPRIDRVCTX)h);

            //uart_printf ("Num=%d (from CTX=0x%08x)\r\n", nNumDev, (PCPRIDRVCTX)h);

            for (i = 0; i < nNumDev; i++)
            {
                if (pCtx->IntID != DI_CPRI_EMULATOR)
                {
                    MxDisableInt(HAL_IRQ_CPRI_0 + i);
                    MxDisableInt(HAL_IRQ_CPDMA_TX_0 + i);
                    MxDisableInt(HAL_IRQ_CPDMA_RX_0 + i);
                }

                CPRIDrvDisableDma(i);
            }

            uart_printf ("\r\nDI:CPRI is closed (devnum=%d)\r\n", nNumDev);
            if (pCtx->LogCtrl)
            {
                uart_printf("Log Status: Tx[%d]. Rx[%d]\r\n", CPRIDrvGetTxLogStatus(0), CPRIDrvGetRxLogStatus(0));
                uart_printf("Log Location:\r\n");
                uart_printf("TxLogLocation: 0x%08x. Stopped at SubFrame: %d\r\n",
                    pCtx->TxLog[0], CPRIDrvGetTxLogSize(0, 0)/61440);
                uart_printf("RxLogLocation: 0x%08x. Stopped at SubFrame: %d\r\n",
                    pCtx->RxLog[0], CPRIDrvGetRxLogSize(0, 0)/61440);
            }

            break;

        case DI_PCIE:
            EvmFpgaStopRadio();
            uart_printf ("\r\nDI:PCIe is closed\r\n");
            if (pCtx->LogCtrl)
            {
                uart_printf("Log Location:\r\n");
                uart_printf("TxLogLocation: 0x%08x. Stopped at SubFrame: %d\r\n",
                    pCtx->TxLog[0], EvmFpgaGetTxLogSize()/61440);
                uart_printf("RxLogLocation: 0x%08x. Stopped at SubFrame: %d\r\n",
                    pCtx->RxLog[0], EvmFpgaGetRxLogSize()/61440);
            }
            break;

        default:
            rc = RC_DI_UNKNOWN_INT;
            break;
    }

    if (SUCCESSFUL(rc))
        pCtx->Opened = 0;

    ARM_INT_restore (irq);

    return MX_OK;
}

MXRC DiPause(DIHANDLE h)
{
    PDiCtx pCtx = DiGetCtx ();

    if (pCtx->IntID == DI_CPRI_EMULATOR)
        CPRIDrvPauseDma(0);

    return MX_OK;
}

MXRC DiResume(DIHANDLE h)
{
    PDiCtx pCtx = DiGetCtx ();

    if (pCtx->IntID == DI_CPRI_EMULATOR)
        CPRIDrvResumeDma(0);

    return MX_OK;
}
/** @brief This function enables/disables LOGs for selected device

    @param h        [in] - the DI handle (please see: DiOpen(...))
    @param nChID    [in] - the channel ID (actually antenna ID)
    @param nLogCtrl [in] - the LOG bit mask: see DI_LOG_xxx

    @return [MXRC] an error code */

MXRC DiLogControl (DIHANDLE h, UINT32 nChID, UINT32 nLogCtrl)
{
    IRQTYPE irq;
    PDiCtx pCtx = DiGetCtx ();
    MXRC rc = MX_OK;
    UINT32 nCtrl;
    PCPRICTX pCpriCtx;

    _ASSERT_PTR(pCtx);

    if (pCtx->Opened == 0)
        return RC_DI_NOT_OPENED;

    irq = ARM_INT_disable();

    switch (pCtx->IntID)
    {
        case DI_CPRI:
        case DI_CPRI_EMULATOR:

            pCpriCtx = GetCPRICtx (nChID);

            _ASSERT_PTR(pCpriCtx);

            pCpriCtx->LogCtrl = (nLogCtrl & DI_LOG_OPT_MASK);

            if (nLogCtrl & DI_LOG_TX_LOG_RESET)
            {
                pCpriCtx->TxLogBufPos[nChID] = 0;
            }

            if (nLogCtrl & DI_LOG_RX_LOG_RESET)
            {
                pCpriCtx->RxLogBufPos[nChID] = 0;
            }


            break;

        case DI_PCIE:

            nCtrl = EvmFpgaGetCtrl ();

            nCtrl &= ~DI_LOG_OPT_MASK;

            rc = EvmFpgaSetCtrl (nLogCtrl | nCtrl);

            if (nLogCtrl & DI_LOG_TX_LOG_RESET)
            {
                EvmFpgaResetTxLog ();
            }

            if (nLogCtrl & DI_LOG_RX_LOG_RESET)
            {
                EvmFpgaResetRxLog ();
            }

            break;

        default:
            rc = RC_DI_UNKNOWN_INT;
            break;
    }

    ARM_INT_restore (irq);

    return rc;
}

UINT32 DiGetCPRIInfo (PCPRIDRVCTX pCpriDrv, UINT32 nChID, UINT32 DiInfoID)
{
    CPRICTX * pCtx;

    _ASSERT_PTR (pCpriDrv);
    _ASSERT_PTR (nChID < CPRI_DEV_NUM);

    pCtx = &pCpriDrv->CpriCtx [nChID];

    switch (DiInfoID)
    {
        case DI_INFO_TX_IRQ_NUM:
            return pCtx->TxIrqNum;

        case DI_INFO_RX_IRQ_NUM:
            return pCtx->RxIrqNum;

        case DI_INFO_TX_LOG_SIZE:
            return pCtx->TxLogBufPos[nChID];

        case DI_INFO_RX_LOG_SIZE:
            return pCtx->RxLogBufPos[nChID];

        case DI_INFO_TX_MAX_IRQ_TIME:
            return pCtx->MaxTxIrqPeriod;

        case DI_INFO_RX_MAX_IRQ_TIME:
            return pCtx->MaxRxIrqPeriod;

        default:
            break;
    }

    return 0;
}

UINT32 DiGetPCIeInfo (UINT32 nChID, UINT32 DiInfoID)
{
    switch (DiInfoID)
    {
        case DI_INFO_TX_IRQ_NUM:
            return EvmFpgaGetTxIrqNum();

        case DI_INFO_RX_IRQ_NUM:
            return EvmFpgaGetRxIrqNum();

        case DI_INFO_TX_LOG_SIZE:
            return EvmFpgaGetTxLogSize();

        case DI_INFO_RX_LOG_SIZE:
            return EvmFpgaGetRxLogSize();

        case DI_INFO_TX_MAX_IRQ_TIME:
            return EvmFpgaGetMaxTxIrqPeriod ();

        case DI_INFO_RX_MAX_IRQ_TIME:
            return EvmFpgaGetMaxRxIrqPeriod ();

        default:
            break;
    }

    return 0;
}

/** @brief This function returns information from data interface driver

    @param h        [in] - the channel handle, see: DiOpen (...)
    @param nChID    [in] - the channel ID (can be treated like antenna)
    @param DiInfoID [in] - the parameter ID (see: DI_INFO_xxx)

    @return [UINT32] value */

UINT32 DiGetInfo (DIHANDLE h, UINT32 nChID, UINT32 DiInfoID)
{
    IRQTYPE irq;
    PDiCtx pCtx = DiGetCtx ();
    MXRC rc = MX_OK;
    //PCPRICTX pCpriCtx;
    UINT32 val=0;

    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable();

    switch (pCtx->IntID)
    {
        case DI_CPRI:
        case DI_CPRI_EMULATOR:
            val = DiGetCPRIInfo ((PCPRIDRVCTX)h, nChID, DiInfoID);
            break;

        case DI_PCIE:
            val = DiGetPCIeInfo (nChID, DiInfoID);
            break;

        default:
            rc = RC_DI_UNKNOWN_INT;
            if (FAILED(rc))
            {
                val = NULL;
                _ASSERT_RC (rc);
            }
            break;
    }

    ARM_INT_restore (irq);

    return val;
}

/** @brief This function returns pointer to the current TX buffer, where data can be putted

    @param h        [in] - the DI handle
    @param nChID    [in] - the channel ID

    @return [LPVOID] pointer to the TX buffer */

LPVOID DiGetTxBuf (DIHANDLE h, UINT32 nChID)
{
    IRQTYPE irq;
    PDiCtx pCtx = DiGetCtx ();
    MXRC rc = MX_OK;
    //PCPRICTX pCpriCtx;
    LPVOID val = 0;

    _ASSERT_PTR(pCtx);

    if (pCtx->Opened == 0)
        return NULL;

    irq = ARM_INT_disable();

    switch (pCtx->IntID)
    {
        case DI_CPRI:
        case DI_CPRI_EMULATOR:
            val = CPRIDrvGetTxBuffer (nChID);
            break;

        case DI_PCIE:
            val = EvmGetTxBuffer (nChID);
            break;

        default:
            rc = RC_DI_UNKNOWN_INT;
            if (FAILED(rc))
            {
                val = NULL;
               _ASSERT_RC (rc);
            }
            break;
    }

    ARM_INT_restore (irq);

    return val;
}

/** @brief This function returns pointer to the current RX buffer, from where data can be read

    @param h        [in] - the DI handle
    @param nChID    [in] - the channel ID

    @return [LPVOID] pointer to the RX buffer */

LPVOID DiGetRxBuf (DIHANDLE h, UINT32 nChID)
{
    IRQTYPE irq;
    PDiCtx pCtx = DiGetCtx ();
    MXRC rc = MX_OK;
    //PCPRICTX pCpriCtx;
    LPVOID val = 0;

    _ASSERT_PTR(pCtx);

    if (pCtx->Opened == 0)
        return NULL;

    irq = ARM_INT_disable();

    switch (pCtx->IntID)
    {
        case DI_CPRI:
        case DI_CPRI_EMULATOR:
            val = CPRIDrvGetRxBuffer (nChID);
            break;

        case DI_PCIE:
            val = EvmGetRxBuffer (nChID);
            break;

        default:
            rc = RC_DI_UNKNOWN_INT;
            if (FAILED(rc))
            {
                val = NULL;
               _ASSERT_RC (rc);
            }
            break;
    }

    ARM_INT_restore (irq);

    return val;
}

/** @brief This function is designed to set ADI/CPRI HW loopback (TX->RX)

    @param h    [in] - the DI handle (see: DiOpen(...))
    @param bLB  [in] - LB mode: 1 - set LB, 0 - reset LB

    @return [MXRC] an error code */

MXRC DiSetLB (DIHANDLE h, BOOL bLB)
{
    IRQTYPE irq;
    PDiCtx pCtx = DiGetCtx ();
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);

    if (pCtx->Opened == 0)
        return RC_DI_NOT_OPENED;

    irq = ARM_INT_disable();

    switch (pCtx->IntID)
    {
        case DI_CPRI:
            rc = CPRIDrvSetLB(0, bLB);
            break;

        default:
            rc = RC_DI_NOT_SUPPORTED;
            break;
    }

    ARM_INT_restore (irq);

    return rc;
}

/** @brief This function returns log mask forced set by the host
    @return [UINT32] log mask*/

UINT32 DiGetForcedLogMask (void)
{
    PDiCtx pCtx = DiGetCtx ();

    _ASSERT_PTR(pCtx);

    if (pCtx->HostLogMaskEnabled)
        return pCtx->HostLogMaskValue;

    return 0;
}

/** @brief This function returns status is forced log mask enabled or not
    @return [UINT32] status*/

UINT32 DiGetIsForcedLogMask (void)
{
    PDiCtx pCtx = DiGetCtx ();

    _ASSERT_PTR(pCtx);

    return pCtx->HostLogMaskValue;
}

/** @brief This function sets log mask that will be applied to data interface
    @param Enabled [in] - enable/disable log mask
    @param nMask   [in] - the log mask value

    @return [MXRC] an error code */

MXRC DiSetForcedLogMask (UINT32 Enabled, UINT32 nMask)
{
    PDiCtx pCtx = DiGetCtx ();
    _ASSERT_PTR(pCtx);

    pCtx->HostLogMaskEnabled = Enabled;
    pCtx->HostLogMaskValue   = nMask;

    return MX_OK;
}

//=============================================================================//
//                             Self test code                                  //
//=============================================================================//

#define DI_SELF_TEST_NUM 100

static VUINT32 DiTestNum;
static DIHANDLE DiChannel;
static PUINT8 txPtr[4], rxPtr[4];
//static UINT32 RxTracePtr [30];
//static UINT32 RxTraceID  [30];
//static UINT32 RxTraceIdx = 0;

void RxCb (LPVOID pPtr, UINT32 nChID, PUINT8 pRxData, UINT32 nSize)
{
    //uart_printf ("rx(%x, %d, %x, %d)\r\n", pPtr, nChID, pRxData, nSize);

#if 0
    if (RxTraceIdx >= sizeof (RxTracePtr)/sizeof (RxTracePtr[0]))
        return;

    RxTracePtr [RxTraceIdx] = (UINT32)pRxData;
    RxTraceID  [RxTraceIdx] = nChID;

    RxTraceIdx++;
#endif
}

void TxCb (LPVOID pPtr, UINT32 nChID, PUINT8 pTxData, UINT32 nSize)
{
    //uart_printf ("tx(%x, %d, %x, %d)\r\n", pPtr, nChID, pTxData, nSize);

    //memset (pTxData, DiPattern, 128);
    //memset (txPtr[index], DiPattern, 128);

    if (DiTestNum != 0)
    {
        DiTestNum --;
    }
}

typedef struct
{
    PUINT8 LogPtr;
    UINT32 Offs;
    UINT32 LogSize;

    UINT32 LastAddr;

}DiCheckCtx;

UINT16 DiGetLogData (DiCheckCtx * pCtx, UINT32 shift)
{
    UINT16 data;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCtx->LogPtr);

    data = *(PUINT16)(pCtx->LogPtr + pCtx->Offs);
    pCtx->LastAddr = (UINT32)(pCtx->LogPtr + pCtx->Offs);

    if (shift)
    {
        pCtx->Offs += sizeof (UINT16);

        if (pCtx->Offs >= pCtx->LogSize)
            pCtx->Offs = 0;
    }

    return data & (~1);
}

void DiCheckUL(PUINT8 pRxLog0, PUINT8 pRxLog1)
{
    DiCheckCtx log1, log2;
    DiCheckCtx tx1, tx2;
    UINT16 ref, tst;
    UINT32 nSkipped = 0, shift = 0, num_to_check;
    UINT32 test_num = (61440 / sizeof (UINT16)) * (DI_SELF_TEST_NUM - 10);
    UINT32 offs = 0;

    _ASSERT_PTR(pRxLog0);
    _ASSERT_PTR(pRxLog1);

    log1.LogPtr = pRxLog0;
    log1.Offs   = 0;
    log1.LogSize= 61440 * DI_SELF_TEST_NUM;

    log2.LogPtr = pRxLog1 + 0;
    log2.Offs   = 0;
    log2.LogSize= 61440 * (DI_SELF_TEST_NUM - 1);

    tx1.LogPtr = txPtr[0];
    tx1.LogSize= 61440 * 2;
    tx1.Offs   = 0;

    tx2.LogPtr = txPtr[2];
    tx2.LogSize= 61440 * 2;
    tx2.Offs   = 0;

    // to skip some bytes
    nSkipped = 0;
    shift    = 0;

    //num_to_check = (DI_SELF_TEST_NUM-1) * (61440 / sizeof (UINT16));
    num_to_check = test_num;

    while (num_to_check != 0)
    {
        ref = DiGetLogData(&tx1,  shift);
        tst = DiGetLogData(&log1, 1);

        if (ref != tst)
        {
            if (shift == 0)
            {
                nSkipped++;

                if (nSkipped < 500)
                    continue;

                uart_printf ("Cannot find pattern for antenna#0\r\n");
                return;
            }
            else
            {
                uart_printf ("Found error! RxLog#0(ref_addr=%x(%d), tst_addr=%x(%d)), ref:[%x]!= tst:[%x]\r\n",
                    tx1.LastAddr,
                    tx1.LastAddr - (UINT32)tx1.LogPtr,
                    log1.LastAddr,
                    log1.LastAddr - (UINT32)log1.LogPtr,
                    ref, tst);
                return;
            }
        }

        if (shift == 0)
        {
            offs = log1.Offs;
            shift = 1;
            ref = DiGetLogData(&tx1,  shift);
        }

        num_to_check --;
    }

    uart_printf ("\r\nAntenna#0 is OK! (offs=%d)\r\n", offs);


    // to skip some bytes
    nSkipped = 0;
    shift    = 0;

    //num_to_check = (DI_SELF_TEST_NUM-1) * (61440 / sizeof (UINT16));
    num_to_check = test_num;

    while (num_to_check != 0)
    {
        ref = DiGetLogData(&tx2,  shift);
        tst = DiGetLogData(&log2, 1);

        if (ref != tst)
        {
            if (shift == 0)
            {
                nSkipped++;

                if (nSkipped < 500)
                    continue;

                uart_printf ("Cannot find pattern for antenna#1\r\n");
                return;
            }
            else
            {
                uart_printf ("Found error! RxLog#0(ref_addr=%x(%d), tst_addr=%x(%d)), ref:[%x]!= tst:[%x]\r\n",
                    tx2.LastAddr,
                    tx2.LastAddr - (UINT32)tx2.LogPtr,
                    log2.LastAddr,
                    log2.LastAddr - (UINT32)log2.LogPtr,
                    ref, tst);

                return;
            }
        }

        if (shift == 0)
        {
            offs = log2.Offs;
            shift = 1;
            ref = DiGetLogData(&tx2,  shift);
        }

        num_to_check --;
    }

    uart_printf ("Antenna#1 is OK! (offs=%d)\r\n", offs);

}

MXRC DiTest (UINT32 nMode)
{
    DiConfig cfg;
    MXRC rc;
    static int init_iq_buf = 0;
    UINT32 i;
    UINT32 tx_num = 2, rx_num = 2;

    rc = MdmaRadInit ();

    if (FAILED(rc))
    {
        uart_printf ("RAD-MDMA init error \r\n");
        return rc;
    }

    if (!init_iq_buf)
    {
        InitializeCramIqBufs (tx_num, rx_num, 61440);
        init_iq_buf = 1;
    }

    txPtr[0] = QueryCramIqBuf(0, 0, 0);
    txPtr[1] = QueryCramIqBuf(0, 1, 0);

    txPtr[2] = QueryCramIqBuf(0, 0, 1);
    txPtr[3] = QueryCramIqBuf(0, 1, 1);

    rxPtr[0] = QueryCramIqBuf(1, 0, 0);
    rxPtr[1] = QueryCramIqBuf(1, 1, 0);

    rxPtr[2] = QueryCramIqBuf(1, 0, 1);
    rxPtr[3] = QueryCramIqBuf(1, 1, 1);

    uart_printf ("ANT#%d DL-0(0x%x), DL-1(0x%x) ... ", 0, txPtr[0], txPtr[1]);
    memset (txPtr[0], 0xAA, 61440);
    memset (txPtr[1], 0xBB, 61440);
    uart_printf ("OK\r\n");

    uart_printf ("ANT#%d UL-0(0x%x), UL-1(0x%x) ... ", 0, rxPtr[0], rxPtr[1]);
    memset (rxPtr[0], 0, 61440);
    memset (rxPtr[1], 0, 61440);
    uart_printf ("OK\r\n");

    uart_printf ("ANT#%d DL-0(0x%x), DL-1(0x%x) ... ", 1, txPtr[2], txPtr[3]);
    memset (txPtr[2], 0xCC, 61440);
    memset (txPtr[3], 0xDD, 61440);
    uart_printf ("OK\r\n");

    uart_printf ("ANT#%d UL-0(0x%x), UL-1(0x%x) ... ", 1, rxPtr[2], rxPtr[3]);
    memset (rxPtr[2], 0, 61440);
    memset (rxPtr[3], 0, 61440);
    uart_printf ("OK\r\n");

    DiTestNum = DI_SELF_TEST_NUM;

    memset (&cfg, 0, sizeof (cfg));

    if (nMode == DI_CPRI)
    {
        cfg.DrvCtrl   = DI_DRV_CTRL_CPRI_IXIA;

        cfg.DrvCtrl = DI_DRV_CTRL_CPRI_10MHZ_MIMO_2AC;

        uart_printf ("DI-CPRI(MIMO 2x2)\r\n");
    }
    else if (nMode == DI_CPRI_EMULATOR)
    {
        cfg.DrvCtrl      = DI_DRV_CTRL_CPRI_IXIA;
        cfg.EmulTickTime = 1000;
        DiTestNum = 10;
        uart_printf ("DI-CPRI_EMULATOR\r\n");
    }
    else if (nMode == DI_PCIE)
    {
        cfg.DrvCtrl   = DI_DRV_CTRL_ADI_PCIE_L1;
        uart_printf ("DI-PCIe\r\n");
    }

    cfg.DrvCtrl |= DI_DRV_CTRL_HW_LB;

    cfg.nTxAntenas = tx_num;
    cfg.nRxAntenas = rx_num;

    cfg.LogCtrl   = DI_LOG_TX0_LOG_ENABLED | DI_LOG_TX1_LOG_ENABLED | DI_LOG_RX0_LOG_ENABLED | DI_LOG_RX1_LOG_ENABLED;
    //cfg.LogCtrl   = 0;

    cfg.TxLog[0]     = (LPVOID)gDlIqBuf;
    cfg.RxLog[0]     = (LPVOID)gUlIqBuf;

    cfg.TxLog[1]     = (LPVOID)((UINT32)gDlIqBuf + 61440 * 500);
    cfg.RxLog[1]     = (LPVOID)((UINT32)gUlIqBuf + 61440 * 500);

    cfg.TxLogSize = DL_IQ_SAMPLES_BUF_SIZE * sizeof (UINT16) / tx_num;
    cfg.RxLogSize = UL_IQ_SAMPLES_BUF_SIZE * sizeof (UINT16) / rx_num;

#if 1

    uart_printf ("Erasing log buffers DL ... ");

    for (i = 0; i < tx_num; i++)
    {
        memset (cfg.TxLog[i], 0, cfg.TxLogSize);
    }

    uart_printf ("OK; UL ... ");

    for (i = 0; i < rx_num; i++)
    {
        memset (cfg.RxLog[i], 0, cfg.RxLogSize);
    }

    uart_printf ("OK\r\n");
#endif

    cfg.TxCb      = TxCb;
    cfg.TxCbPtr   = (LPVOID)(0x12345678);
    cfg.RxCb      = RxCb;
    cfg.RxCbPtr   = (LPVOID)(0x87654321);

    rc = DiOpen (nMode, &cfg, &DiChannel);

    if (FAILED(rc))
    {
        uart_printf ("DiOpen-failed, rc=0x%x\r\n", rc);
        return rc;
    }

    while (DiTestNum != 0)
    {
        _NOP();
    }

    uart_printf ("Test is completed\r\n");

    DiClose (DiChannel);

    uart_printf ("Checking data by using UL logs ...");

    // Here we check the DL and UL logs by

    DiCheckUL (cfg.RxLog[0], cfg.RxLog[1]);

    return 0;
}


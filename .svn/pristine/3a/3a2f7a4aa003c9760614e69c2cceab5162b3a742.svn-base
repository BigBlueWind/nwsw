//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Iqphone.c
 *
 * @brief IQ player/recorder service, it's developed to be used with CPRI driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "iqphone.h"
#include "supervisor.h"
#include "mdmadrv.h"
#include "appprintf.h"
#include "heaps.h"

static IQPhoneCtx iq_phone_ctx;

#define IS_CPRI_MODE(mode)  (((mode) & 0xF) == 0 || ((mode) & 0xF) == 2)

INLINE IQPhoneCtx * IQPhoneGetCtx (void)
{
    return &iq_phone_ctx;
}

/** @brief This function is developed to process CPRI TX interrupt
           and prepare TX data, the data is taken from TX LOG
           TX log can be pre-created by the host

    @param pCpriCtx [in] - the CPRI driver context
    @return [NULL] */

void IQPhoneTxCb (LPVOID pTxPtr, UINT32 nChannID, PUINT8 pTxData, UINT32 nSize)
{
    IQPhoneCtx * pCtx = IQPhoneGetCtx();
    MXRC rc;

    _ASSERT_PTR(pCtx);

    // if TX logging is turned off
    if ((pCtx->TxLogPtr == NULL) || (pCtx->Mode & IQPHONE_CTRL_TX_ENABLE) == 0)
        return;

    if (pCtx->TxSkipNum != 0)
    {
        pCtx->TxSkipNum --;
        return;
    }

    if (pCtx->TxSendNum >= pCtx->TxLogLim)
        return;

    pCtx->TxSendNum ++;

    rc = MdmaCopy (MDMA_HW_RAD, pTxData, pCtx->TxLogPtr + pCtx->TxLogOffs, nSize);

    if (FAILED(rc))
    {
        pCtx->Status |= IQPHONE_STAT_MDMA_TX;
        return;
    }

    pCtx->TxLogOffs += nSize;

    if ((pCtx->TxLogOffs + nSize) > pCtx->TxLogSize)
    {
        if (pCtx->Mode & IQPHONE_CTRL_TX_RR)
            pCtx->TxLogOffs = 0;
        else
            pCtx->Mode &= ~IQPHONE_CTRL_TX_ENABLE;
    }

    return;
}

/** @brief This function is developed to process CPRI RX interrupt
           and log RX data, RX log can be downloaded by the host for further investigation

    @param pCpriCtx [in] - the CPRI driver context
    @return [NULL] */

void IQPhoneRxCb (LPVOID pRxPtr, UINT32 nChannID, PUINT8 pRxData, UINT32 nSize)
{
    IQPhoneCtx * pCtx = IQPhoneGetCtx();
    //PCPDMAREGS pCpdmaRegs = pCpriCtx->pCpdmaRegs;
    //UINT32 BufOffset = pCpriCtx->RxBufOffset;
    MXRC rc;

    // if TX logging is turned off
    if ((pCtx->RxLogPtr == NULL) || (pCtx->Mode & IQPHONE_CTRL_RX_ENABLE) == 0)
        return;

    if (pCtx->RxSkipNum != 0)
    {
        pCtx->RxSkipNum --;
        return;
    }

    if (pCtx->RxRecvNum >= pCtx->RxLogLim)
        return;

    pCtx->RxRecvNum ++;

    rc = MdmaCopy (MDMA_HW_RAD, pCtx->RxLogPtr + pCtx->RxLogOffs, pRxData, nSize);

    if (FAILED(rc))
    {
        pCtx->Status |= IQPHONE_STAT_MDMA_RX;
        return;
    }

    pCtx->RxLogOffs += nSize;

    if ((pCtx->RxLogOffs + nSize) > pCtx->RxLogSize)
    {
        if (pCtx->Mode & IQPHONE_CTRL_RX_RR)
            pCtx->RxLogOffs = 0;
        else
            pCtx->Mode &= ~IQPHONE_CTRL_RX_ENABLE;
    }

    return;
}

UINT32 IQPhoneCPRISubInit(PCPRIDRVCTX pCpriDrvCtx)
{
    pCpriDrvCtx->CpriCtx->pTxDMACallBack = IQPhoneTxCb;
    pCpriDrvCtx->CpriCtx->pRxDMACallBack = IQPhoneRxCb;

    return 0;
}

MXRC IQPhoneDealloc (IQPhoneCtx * pCtx)
{
    IRQTYPE irq = ARM_INT_disable();

    if (pCtx->TxLogPtr != NULL)
    {
        MxHeapFree (DDR1NcnbHeap, pCtx->TxLogPtr);
        pCtx->TxLogPtr = NULL;
    }

    if (pCtx->RxLogPtr != NULL)
    {
        MxHeapFree (DDR1NcnbHeap, pCtx->RxLogPtr);
        pCtx->RxLogPtr = NULL;
    }

    pCtx->TxLogSize = 0;
    pCtx->RxLogSize = 0;

    pCtx->TxLogOffs = 0;
    pCtx->RxLogOffs = 0;

    ARM_INT_restore(irq);

    return MX_OK;
}

MXRC IQPhoneAlloc (IQPhoneCtx * pCtx, IQPhoneInitData * pCfgData)
{
    MXRC rc;
    IRQTYPE irq;

    rc = IQPhoneDealloc (pCtx);

    if (FAILED(rc))
    {
        uart_printf ("IQPhone: dealloc resource error, rc=%x\r\n", rc);
        return rc;
    }

    irq = ARM_INT_disable();

    pCtx->Mode = pCfgData->nMode;

    pCtx->TxLogPtr = (PUINT8)MxHeapAlloc(DDR1NcnbHeap, pCfgData->TxLogSize);
    pCtx->RxLogPtr = (PUINT8)MxHeapAlloc(DDR1NcnbHeap, pCfgData->RxLogSize);

    if (pCtx->TxLogPtr == NULL || pCtx->RxLogPtr == NULL)
    {
        uart_printf ("IQPhone: Allocation error, not memory\r\n");
        IQPhoneDealloc (pCtx);
        ARM_INT_restore(irq);
        return RC_IQPHONE_ALLOC_ERROR;
    }

    pCtx->TxLogOffs = 0;
    pCtx->RxLogOffs = 0;
    pCtx->TxLogSize = pCfgData->TxLogSize;
    pCtx->RxLogSize = pCfgData->RxLogSize;

    ARM_INT_restore(irq);

    return MX_OK;
}

/** @brief This function is designed to initialize CPRI interface
           to play and record data

    @param pCfgData [in] - the configuration data

    @return [MXRC] an error code */

MXRC IQPhoneInitCpri (IQPhoneInitData * pCfgData)
{
    MXRC rc;
    CPRICONFIG CpriConf;
    UINT32 nMode;
    IQPhoneCtx * pCtx = IQPhoneGetCtx ();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCfgData);

    rc = IQPhoneAlloc(pCtx, pCfgData);

    if (FAILED(rc))
        return rc;

    pCtx->Mode      = pCfgData->nMode;
    pCtx->TxSkipNum = pCfgData->TxSkipNum;
    pCtx->RxSkipNum = pCfgData->RxSkipNum;
    pCtx->TxLogLim  = pCfgData->TxLogLim;
    pCtx->RxLogLim  = pCfgData->RxLogLim;

    memset (&CpriConf, 0, sizeof (CpriConf));

    nMode = pCfgData->nMode & 0xF;

    CpriConf.NumCpriDev = 1;
    CpriConf.SamplingRate = CPRI_15_36_MSPS;
    CpriConf.SampleWidth = 15;
    CpriConf.RecMacAddr[0] = CpriConf.ReMacAddr[0] = 0x00;
    CpriConf.RecMacAddr[1] = CpriConf.ReMacAddr[1] = 0x11;
    CpriConf.RecMacAddr[2] = CpriConf.ReMacAddr[2] = 0x22;
    CpriConf.RecMacAddr[3] = CpriConf.ReMacAddr[3] = 0x33;
    CpriConf.RecMacAddr[4] = CpriConf.ReMacAddr[4] = 0x44;
    CpriConf.RecMacAddr[5] = 0x55;
    CpriConf.ReMacAddr[5] = 0x66;
    CpriConf.RecIpAddr = 0xc0a80100;
    CpriConf.ReIpAddr = 0xc0a80101;
    CpriConf.EthEnable = 1;
    CpriConf.EthLittleEndian = 1;
    CpriConf.EthFcsEnable = 1;
    CpriConf.EthRate = 0x3f;
    CpriConf.HdlcEnable = 0;
    CpriConf.MaxAcNum = 1;

    if (nMode == 0)         // IXIA MODE
    {
        //CPRI init
        CpriConf.CpriRate = CPRIRATE_2457; // CPRIRATE_1228
		CpriConf.CpriConfType = CPRI_IXIA;
		CpriSrds1Init(CPRIRATE_2457, 0, 1);
		CpriConf.SampleWidth = 15;
        CpriConf.pCPRICreateCallBack = IQPhoneCPRISubInit;
		uart_printf("CPRI-Mode: Default (IXIA)\n");
	}
    else if (nMode == 1)    // RADIOCOMP MODE
	{
	    CpriConf.SampleWidth = 16;
		CpriConf.MaxAcNum = 2;
		CpriConf.CpriConfType = CPRI_RADIOCOMP;
        CpriConf.pCPRICreateCallBack = IQPhoneCPRISubInit;
		CpriConf.CpriRate = CPRIRATE_3072; /*CPRIRATE_2457*/; //CPRIRATE_3072; // CPRIRATE_1228

		//CPRI init
		CpriSrds1Init(/*CPRIRATE_2457*/ CPRIRATE_3072, 0, 1);
		uart_printf("CPRI-Mode: RADIOCOMP\n");
	}
    else
	{
	    IQPhoneDealloc(pCtx);
	    uart_printf ("IQPhone: non-supported CPRI mode\r\n");
		return RC_IQPHONE_CPRI_MODE_ERROR;
	}

    if (FAILED(rc = CPRIDrvInit(&CpriConf)))
    {
        IQPhoneDealloc(pCtx);
        uart_printf ("CPRI driver init error, rc=%x\r\n", rc);
        return rc;
    }

    pCtx->bModuleInit = 1;

    uart_printf ("IQPhone: CPRI player/recorder is initialized\r\n");

    return MX_OK;
}

MXRC IQPhoneLoadCfgData(ApiHeader * pApi, IQPhoneInitData * pCfg)
{
    _ASSERT_PTR(pApi);
    _ASSERT_PTR(pCfg);

    pCfg->nMode     = ApiGetParamU32 (pApi, 1, 0);
    pCfg->TxLogSize = ApiGetParamU32 (pApi, 2, 0);
    pCfg->RxLogSize = ApiGetParamU32 (pApi, 3, 0);
    pCfg->TxSkipNum = ApiGetParamU32 (pApi, 4, 0);
    pCfg->RxSkipNum = ApiGetParamU32 (pApi, 5, 0);
    pCfg->TxLogLim  = ApiGetParamU32 (pApi, 6, 0xffffffff);
    pCfg->RxLogLim  = ApiGetParamU32 (pApi, 7, 0xffffffff);

    if (pCfg->TxLogSize == 0 && (pCfg->nMode&IQPHONE_CTRL_TX_ENABLE))
        return RC_IQPHONE_TX_SIZE_ERROR;

    if (pCfg->RxLogSize == 0 && (pCfg->nMode&IQPHONE_CTRL_RX_ENABLE))
        return RC_IQPHONE_RX_SIZE_ERROR;

    return MX_OK;
}

APIHEC IQPhoneApiHandler (LPVOID lpCtx, ApiHeader * pApi, ApiHeader * pResp)
{
    IQPhoneCtx * pCtx = IQPhoneGetCtx();
    IQPhoneInitData cfg;
    MXRC rc = 0;

    _ASSERT_PTR(pCtx);

    if (IS_CFG_API(pApi, IQPHONE_INIT))
    {
        if (SUCCESSFUL(rc = IQPhoneLoadCfgData (pApi, &cfg)))
        {
            if (IS_CPRI_MODE(cfg.nMode))
            {
                rc = IQPhoneInitCpri (&cfg);

                if (SUCCESSFUL(rc))
                {
                    pCtx->bModuleInit = 1;

                    ApiAddParam(pResp, NULL, 2, &pCtx->TxLogPtr, sizeof (pCtx->TxLogPtr));
                    ApiAddParam(pResp, NULL, 3, &pCtx->RxLogPtr, sizeof (pCtx->RxLogPtr));
                }
            }
            else
            {
                rc = RC_IQPHONE_UNSUP_MODE;
            }
        }
    }
    else if (IS_CFG_API(pApi, IQPHONE_START))
    {
        if (SUCCESSFUL(rc = IQPhoneLoadCfgData (pApi, &cfg)))
        {
            if (IS_CPRI_MODE(cfg.nMode))
            {
                uart_printf ("IQPhone: start player/recorder\r\n");
                rc = IQPhoneStart(pCtx->Mode, &cfg);
            }
            else
            {
                rc = RC_IQPHONE_UNSUP_MODE;
            }
        }
    }
    else if (IS_CFG_API(pApi, IQPHONE_STOP))
    {
        UINT32 nCtrl = ApiGetParamU32(pApi, 2, IQPHONE_CTRL_TX_ENABLE | IQPHONE_CTRL_RX_ENABLE);
        rc = IQPhoneStop(nCtrl);

        uart_printf ("IQPhone: stop API, rc=0x%04x, ctrl=%x\r\n", rc, nCtrl);
    }
    else if (IS_QRY_API(pApi, IQPHONE_GET_INFO))
    {
        ApiAddParam(pResp, NULL, 2, &pCtx->TxLogPtr, sizeof (pCtx->TxLogPtr));
        ApiAddParam(pResp, NULL, 3, &pCtx->RxLogPtr, sizeof (pCtx->RxLogPtr));
        ApiAddParam(pResp, NULL, 4, &pCtx->Status, sizeof (pCtx->Status));
        ApiAddParam(pResp, NULL, 5, &pCtx->RxLogOffs, sizeof (pCtx->RxLogOffs));
        ApiAddParam(pResp, NULL, 6, &pCtx->TxLogOffs, sizeof (pCtx->TxLogOffs));
        ApiAddParam(pResp, NULL, 7, &pCtx->TxSendNum, sizeof (pCtx->TxSendNum));
        ApiAddParam(pResp, NULL, 8, &pCtx->RxRecvNum, sizeof (pCtx->RxRecvNum));
    }

    ApiAddErrorCode(pResp, rc);

    return APIHEC_OK;
}

MXRC IQPhoneInit (void)
{
    IQPhoneCtx * pCtx = IQPhoneGetCtx();
    MXRC rc;
    _ASSERT_PTR(pCtx);

    memset (pCtx, 0, sizeof (*pCtx));

    if (FAILED(rc = SvsrRegClient (IID_IQ_PHONE, IQPhoneApiHandler, pCtx)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    pCtx->bCtxInit = 1;

    return MX_OK;
}

/** @brief This function starts player/logger

    @param nInterface [in] - the player/recorder communication interface, see: IQPHONE_COM_xxx
    @param pCfgData   [in] - the pointer to the player/recorder configuration parameters

    @return [MXRC] an error code */

MXRC IQPhoneStart (UINT32 nInterface, IQPhoneInitData * pCfgData)
{
    IQPhoneCtx * pCtx = IQPhoneGetCtx();
    IRQTYPE irq;
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);

    if (pCtx->bModuleInit == 0)
        return RC_IQPHONE_NOT_INITED;

    irq = ARM_INT_disable();

    pCtx->Mode &= ~(IQPHONE_CTRL_TX_ENABLE|IQPHONE_CTRL_RX_ENABLE|IQPHONE_CTRL_TX_RR|IQPHONE_CTRL_RX_RR);

    if (pCfgData->nMode & IQPHONE_CTRL_TX_ENABLE)
    {
        pCtx->TxLogLim  = pCfgData->TxLogLim;
        pCtx->TxSkipNum = pCfgData->TxSkipNum;

        pCtx->TxLogOffs = 0;
        pCtx->Mode |= IQPHONE_CTRL_TX_ENABLE;
    }

    if (pCfgData->nMode & IQPHONE_CTRL_RX_ENABLE)
    {
        pCtx->RxLogLim  = pCfgData->RxLogLim;
        pCtx->RxSkipNum = pCfgData->RxSkipNum;

        pCtx->RxLogOffs = 0;
        pCtx->Mode |= IQPHONE_CTRL_RX_ENABLE;
    }

    pCtx->Mode |= (pCfgData->nMode & (IQPHONE_CTRL_TX_RR|IQPHONE_CTRL_RX_RR));

    if (IS_CPRI_MODE(pCtx->Mode))
        CPRIDrvEnableDma(pCtx->DevID);

    ARM_INT_restore(irq);

    return rc;
}

/** @brief This function stops player/logger

    @param nMode    [in] - the player/recorder id,
                           see: IQPHONE_CTRL_TX_ENABLE/IQPHONE_CTRL_RX_ENABLE

    @return [MXRC] an error code */

MXRC IQPhoneStop (UINT32 nCtrl)
{
    IQPhoneCtx * pCtx = IQPhoneGetCtx();
    IRQTYPE irq;
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);

    if (pCtx->bModuleInit == 0)
        return RC_IQPHONE_NOT_INITED;

    irq = ARM_INT_disable();

    pCtx->Mode &= ~(nCtrl & (IQPHONE_CTRL_TX_ENABLE|IQPHONE_CTRL_RX_ENABLE));

    if (IS_CPRI_MODE(pCtx->Mode))
        CPRIDrvDisableDma(pCtx->DevID);

    ARM_INT_restore(irq);

    return rc;
}


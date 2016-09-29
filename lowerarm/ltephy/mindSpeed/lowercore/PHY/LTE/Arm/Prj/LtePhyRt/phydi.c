//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL phydi.c
 *
 * @brief PHY data interface
 *       CPRI/PCIe/sRIO
 * @author Mindspeed Technologies
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------


#include "ltephy.h"
#include "ltephydef.h"
#include "phydi.h"
#include "globalstoragearrays.h"
#include "ltephydef.h"
#include "LtePhyL2Api.h"
#include "lte_entry.h"
#include "Debug.h"
#include "prototypes.h"
#include "ltertconfig.h"
#include "mlog.h"
#include "appprintf.h"
#include "LteBsRxThread.h"
#include "LteBsPhyStats.h"

#include "mdmadrv.h"
#include "timerdrv.h"
#include "prototypes.h"
#include "ltephydef.h"
#include "ltephy.h"
#include "ext_phy.h"
#include "svsrlogger.h"
#include "supervisor.h"
#include "LteBsApiThread.h"

#include "cd_data_objects.h"
#include "UMTSNwlMsgPro.h"

DIHANDLE PhyDiHandle;
PHYDICTX PhyDiCtx;

extern LTEAppCtx *pLteCtx;
extern U32 nmm_subframe_count;
extern  U8 gUmtsnmmMode;
extern 	U32  UMTSNwl_subframe_count;
INLINE PPHYDICTX PhyDiGetCtx()
{
    return &PhyDiCtx;
}

MXRC PhyDiInit(PLTEPHYDESC pPhy)
{
    MXRC rc = MX_OK;
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    UINT32 numBytes;
    UINT16 N_tx_ante,N_rx_ante;

    if(gMode == UMTS_NWL_MODE)
    {
        numBytes = 4*15360;     //3GNWL_TEMP
        N_tx_ante = 1;
        N_rx_ante = 1;
    }
    else
    {
    _ASSERT_PTR(pPhy);
        numBytes = 4*ComputeUpperBound(pPhy->pBs->Nfft);    // 4 because IQ samples and 2 bytes per S16        
        N_tx_ante = pPhy->pBsTx->Ntx_antennas;
        N_rx_ante = pPhy->pBsRx->Nrx_antennas;        
    }



    pPhyDiCtx->dlIqSizePerSfn = numBytes;
    pPhyDiCtx->dlIqBuf = (PUINT8)gDlIqBuf;
    pPhyDiCtx->dlIqBufSize = iq_samples_size_dl * 2;

    // Needed because for DL the DDR Buffer looks like: |iq0,0, iq0,1, iq0,2,..., iq0,n|iq1,0, iq1,1, iq1,2,..., iq1,n| .. |iqm,0, iqm,1, iqm,2,..., iqm,n|
    // m = number of antenna and n = number of subframes
    pPhyDiCtx->dlIqJumpSize = pPhyDiCtx->dlIqBufSize / N_tx_ante;

    pPhyDiCtx->ulIqSizePerSfn = numBytes * N_rx_ante;
    pPhyDiCtx->ulIqBuf = (PUINT8)gUlIqBuf;
    pPhyDiCtx->ulIqBufSize = iq_samples_size_ul * 2;

    pPhyDiCtx->NtxAntenna = N_tx_ante;
    pPhyDiCtx->NrxAntenna = N_rx_ante;

    memset(pPhyDiCtx->dlIqBuf, 0xAB, pPhyDiCtx->dlIqBufSize);

    uart_printf("\r\nPHYDI-Init\r\n");
    uart_printf("DL IQ Samples Buf: 0x%08x (size: %d bytes). Num Tx ant: %d. Num Bytes per SFN: %d.\r\n",
        pPhyDiCtx->dlIqBuf, pPhyDiCtx->dlIqBufSize, N_tx_ante, pPhyDiCtx->dlIqSizePerSfn);
    uart_printf("UL IQ Samples Buf: 0x%08x (size: %d bytes). Num Rx ant: %d. Num Bytes per SFN: %d.\r\n",
        pPhyDiCtx->ulIqBuf, pPhyDiCtx->ulIqBufSize, N_rx_ante, pPhyDiCtx->ulIqSizePerSfn);

    pPhyDiCtx->PhyState = PHY_STATE_INIT;

    // Allocate Memory for DL and UL IQ samples in CRAM at location CPRI_DMA_BUF_ADDR
    InitializeCramIqBufs(pPhyDiCtx->NtxAntenna, pPhyDiCtx->NrxAntenna, numBytes);

    pPhyDiCtx->PhyPauseResume = 0;
    pPhyDiCtx->PhyPauseResumeSkipped = 0;

    return rc;
}


MXRC PhyDiDestroy(void)
{
    MXRC rc = MX_OK;
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();

    _ASSERT_PTR(pPhyDiCtx);

    DestroyCramIqBufs();

    //memset(pPhyDiCtx, 0, sizeof(PHYDICTX));

    return rc;
}

UINT32 PhyDiQueryTimerPeriod(void)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    return (pPhyDiCtx->PhyQueryTTIPeriod);
}

U32 PhyDiStart(U32 phyInstance, PSTARTREQ pStartReq, U32 logIqEnable)
{
    DiConfig cfg;
    MXRC rc;
    int i;
    int extraTTI;
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    PTRTPauseCtx pRtPauseCtx = &pPhyDiCtx->rt_pause_ctx;


    U32 ret = MX_OK;

    if (pPhyDiCtx->PhyState == PHY_STATE_SHUTDOWN)
    {
        uart_printf("PHYDI State: PHY_STATE_SHUTDOWN. Need ReInit of PHY\r\n");
        return ret;
    }

    pPhyDiCtx->PhyStartMode = pStartReq->mode;
    pPhyDiCtx->PhyStartCount = (pStartReq->count ? (SINT32)pStartReq->count+2 : (SINT32)-1);
    pPhyDiCtx->PhyStartPeriod = CPRI_GET_RATE(pStartReq->period);

    if (pRtPauseCtx->PauseTTIThr)
    {
        extraTTI = (pStartReq->count)/pRtPauseCtx->PauseTTIThr;
        uart_printf("PHYDI-EXTRA CYCLES: %d\r\n", extraTTI);
        pPhyDiCtx->PhyStartCount += extraTTI;
    }

    uart_printf("PHYDI-START: Mode: %d, Count: %d, Period: %d\r\n",
        pPhyDiCtx->PhyStartMode, pPhyDiCtx->PhyStartCount, pPhyDiCtx->PhyStartPeriod);

    uart_printf("SvsrLogger current mask: 0x%08X\r\n", SvsrLoggerGetMask());
    uart_printf("MLog current mask: 0x%08X\r\n\n", MLogGetMask());

    rc = MdmaRadInit();

    if (FAILED(rc))
    {
        uart_printf("RAD-MDMA init error \r\n");
        return rc;
    }

    memset(&cfg, 0, sizeof(cfg));

    cfg.LogCtrl = logIqEnable;

    pPhyDiCtx->PhyQueryTTIPeriod = 1000;            // usecs

    if (pPhyDiCtx->PhyStartMode == DI_CPRI)
    {
        cfg.UseRxAntenna = CPRI_GET_RX_USAGE(pStartReq->period);
        cfg.UseTxAntenna = CPRI_GET_TX_USAGE(pStartReq->period);

        if (pPhyDiCtx->PhyStartPeriod == RADIOCOMP)
        {
           cfg.DrvCtrl = DI_DRV_CTRL_CPRI_RADIOCOMP;
           uart_printf("PHYDI-CPRI(RADIOCOMP)\r\n");
        }
        else if (pPhyDiCtx->PhyStartPeriod == OPTICAL_LOOPBACK)
        {
           cfg.DrvCtrl = DI_DRV_CTRL_CPRI_OPTICAL_LOOPBACK;
           uart_printf("PHYDI-CPRI(OPTICAL LOOPBACK)\r\n");
        }
        else if (pPhyDiCtx->PhyStartPeriod == OPTICAL_LOOPBACK_RADIOCOMP)
        {
           cfg.DrvCtrl = DI_DRV_CTRL_CPRI_OPTICAL_LOOPBACK | DI_DRV_CTRL_CPRI_RADIOCOMP;
           uart_printf("PHYDI-CPRI(OPTICAL LOOPBACK RADIOCOMP)\r\n");
        }
        else if (pPhyDiCtx->PhyStartPeriod == CPRI_20MHZ_1AC)
        {
           cfg.DrvCtrl = DI_DRV_CTRL_CPRI_20MHZ_1AC;
           uart_printf("PHYDI-CPRI(20MHZ_1AC)\r\n");
        }
        else if (pPhyDiCtx->PhyStartPeriod == CPRI_10MHZ_MIMO_2AC)
        {
           cfg.DrvCtrl = DI_DRV_CTRL_CPRI_10MHZ_MIMO_2AC;
           uart_printf("PHYDI-CPRI(10MHZ_MIMO_2AC)\r\n");
        }
        else
        {
           cfg.DrvCtrl = DI_DRV_CTRL_CPRI_IXIA;
           uart_printf("PHYDI-CPRI(IXIA)\r\n");
        }

    }
    else if (pPhyDiCtx->PhyStartMode == DI_CPRI_EMULATOR)
    {
        pPhyDiCtx->PhyQueryTTIPeriod = pPhyDiCtx->PhyStartPeriod * 1000;    // usec
        cfg.DrvCtrl = DI_DRV_CTRL_CPRI_IXIA;
        cfg.EmulTickTime = pPhyDiCtx->PhyStartPeriod;
        uart_printf("PHYDI-CPRI(EMULATOR:%d)\r\n", pPhyDiCtx->PhyStartPeriod);
    }
    else if (pPhyDiCtx->PhyStartMode == DI_PCIE)
    {
        if (pPhyDiCtx->PhyStartPeriod == 2)
            cfg.DrvCtrl = DI_DRV_CTRL_ADI_PCIE_L2;
        else if (pPhyDiCtx->PhyStartPeriod == 4)
            cfg.DrvCtrl = DI_DRV_CTRL_ADI_PCIE_L4;
        else
            cfg.DrvCtrl = DI_DRV_CTRL_ADI_PCIE_L1;

        uart_printf("PHY-DI interface: [PCIe]\r\n");
    }

    for(i = 0; i < pPhyDiCtx->NtxAntenna; i++)
        cfg.TxLog[i] = (LPVOID)((PUINT8)gDlIqBuf + i * pPhyDiCtx->dlIqJumpSize);

    cfg.nTxAntenas = pPhyDiCtx->NtxAntenna;

    for(i = 0; i < pPhyDiCtx->NrxAntenna; i++)
        cfg.RxLog[i] = (LPVOID)((PUINT8)gUlIqBuf + i * pPhyDiCtx->dlIqJumpSize);

    cfg.nRxAntenas = pPhyDiCtx->NrxAntenna;

    cfg.TxLogSize = pPhyDiCtx->dlIqBufSize / 2;
    cfg.RxLogSize = pPhyDiCtx->ulIqBufSize / 2;

    // Warning!
    cfg.RxLogRoudupPos = 0;
    cfg.TxLogFreezeNum = 1; // to skip logging in the emulator mode irrelevant SFs at the beginning
    cfg.RxLogFreezeNum = 2; // to skip logging in the emulator mode irrelevant SFs at the beginning

    cfg.TxCb = NULL;            // No need to call at this point
    cfg.RxCb = PHYDiRxHandler;  // Should be called at this point

    pPhyDiCtx->PhyState = PHY_STATE_START;

    rc = DiOpen(pPhyDiCtx->PhyStartMode, &cfg, &PhyDiHandle);

    if (FAILED(rc))
    {
        uart_printf("DiOpen-failed, rc=0x%x\r\n", rc);
        return rc;
    }
    pPhyDiCtx->PhyStartCurrCount = 0;

    if (FAILED(ret))
    {
        uart_printf ("PhyDiStart is failed, rc=%x\r\n", ret);
        _ASSERT_RC(ret);
    }

#ifdef PM_ENABLED
    // to enable the profiling and to convert number of
    // cpu cores to cpu_mask
    PMEnableProf ((1 << SysGetCpuNum ()) - 1);
#endif

    return ret;
}

void PHYDiTxHandler(LPVOID pTxPtr, UINT32 nChID, PUINT8 pTxData, UINT32 nSize)
{
    UINT32 t = GET_TICKS;

    MLogTask(PID_PHYDI_TX_CALLBACK, RESOURCE_LARM, t, t+100);
}

void PHYDiRxHandler(LPVOID pRxPtr, UINT32 nChID, PUINT8 pRxData, UINT32 nSize)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    UINT32 t = GET_TICKS;

    if(gMode == UMTS_NWL_MODE)
    {
        UMTSNwl_thread_SaveData(pRxData);
        return;	
    }
    if (gMode == LTE_MODE)
    {
        if (pPhyDiCtx->PhyState == PHY_STATE_START)
            LtePhyApiThreadStartTimer();
    }

    MLogTask(PID_PHYDI_RX_CALLBACK, RESOURCE_LARM, t, t+100);

    if (pPhyDiCtx->PhyPauseResume == 1)
    {
        return;
    }

    if (pPhyDiCtx->PhyPauseResumeSkipped)
    {
        pPhyDiCtx->PhyPauseResumeSkipped--;
        return;
    }


    if (nChID > 0)
        return;

    if (stopPhy)
        stopPhy++;

    if (stopPhy >= 10)
    {
        stop_printf("Stopped at some trap\r\n");
        return;
    }

    // Sepcial case where the PHY is shutdown from console
    if (pPhyDiCtx->PhyState == PHY_STATE_SEND_STOP)
    {
        pPhyDiCtx->PhyState = PHY_STATE_START;
        PhyDiStop(NULL);
        return;
    }

    // Sepcial case where the PHY is shutdown from console
    if (pPhyDiCtx->PhyState == PHY_STATE_SEND_SHUTDOWN)
    {
        pPhyDiCtx->PhyState = PHY_STATE_START;
        PhyDiShutdown(NULL);
        return;
    }

    if ((pPhyDiCtx->PhyStartCount != -1) && (pPhyDiCtx->PhyStartCurrCount == pPhyDiCtx->PhyStartCount))
    {
        PhyDiStop(NULL);
    }
    else
    {
        pPhyDiCtx->PhyStartCurrCount++;
        LtePhySchedulePhyThread(NULL);
    }
}

UINT32 PhyDiPauseSetRunCtrl (UINT32 nTTIThr, UINT32 nSkipThr, UINT32 nDstInstID)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    PTRTPauseCtx pRtPauseCtx = &pPhyDiCtx->rt_pause_ctx;

    memset (&pPhyDiCtx->rt_pause_ctx, 0, sizeof (pPhyDiCtx->rt_pause_ctx));

    pRtPauseCtx->PauseTTIThr = nTTIThr - 1; // "#0 Frame should be taken into account"
    pRtPauseCtx->SkipTTIThr  = nSkipThr;
    pRtPauseCtx->DstInstID   = (nDstInstID == 0)? UARM_INSTID_HOST : nDstInstID;

    if (pRtPauseCtx->PauseTTIThr)
        pRtPauseCtx->Mode = RT_PAUSE_MODE_ENABLED;
    else
        pRtPauseCtx->Mode = RT_PAUSE_MODE_IDLE;

    pRtPauseCtx->PauseTTIVal = 0;

    return 0;
}



UINT32 PhyDiResume (void)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();

    uart_printf("PHYDI-RESUME\r\n");

    if (pPhyDiCtx->PhyStartMode == AUTO_TIMER)
    {
        DiResume(PhyDiHandle);
    }

    return 0;
}


UINT32 PhyDiPauseGenInd (void)
{
    ApiHeader * pApi;
    MXRC rc;
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    PTRTPauseCtx pRtPauseCtx = &pPhyDiCtx->rt_pause_ctx;
    LTEAppCtx * pCtx = pLteCtx;

    _ASSERT_PTR(pCtx);

    pApi = SvsrAllocICApi ();

    if (pApi == NULL)
    {
        uart_printf ("\r\nPHYDI-SvsrAllocICApi==NULL\r\n");
        while (1);
    }

    pApi = ApiCreate (pApi, API_DEF_FLAGS, pCtx->nInstanceID, pRtPauseCtx->DstInstID, API_TYPE_IND, PHY_LTE_TIMER_PAUSED);

    ApiAddParam(pApi, NULL, 2, &(pLteBsPhyStats->timerStats.numUlListMissed),sizeof (pLteBsPhyStats->timerStats.numUlListMissed));
    ApiAddParam(pApi, NULL, 3, &(pLteBsPhyStats->timerStats.numDlListMissed),sizeof (pLteBsPhyStats->timerStats.numDlListMissed));

    if (FAILED(rc = SvsrSendICApi(pApi)))
    {
        uart_printf ("PHYDI-SvsrSendAPI error 0x%02x\r\n", rc);
        while (1);
    }

    return 0;
}


UINT32 PhyDiPauseProc (void)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    PTRTPauseCtx pRtPauseCtx = &pPhyDiCtx->rt_pause_ctx;

    if (pPhyDiCtx->PhyStartMode == AUTO_TIMER)
    {
        switch (pRtPauseCtx->Mode)
        {
            case RT_PAUSE_MODE_IDLE:
            {
                // just do nothing, this mechanism was not enabled
                break;
            }
            case RT_PAUSE_MODE_ENABLED:
            {
                // if this functionality is disabled
                if (pRtPauseCtx->PauseTTIThr == 0)
                    return 0;

                if (pRtPauseCtx->PauseTTIThr == pRtPauseCtx->PauseTTIVal)
                {
                    pRtPauseCtx->Mode = RT_PAUSE_MODE_SKIP;
                    pRtPauseCtx->PauseTTIVal = 0;

                    if (pPhyDiCtx->PhyStartMode == AUTO_TIMER)
                    {
                        DiPause(PhyDiHandle);
                    }
                    uart_printf("PHYDI-PAUSED\r\n");

                    PhyDiPauseGenInd ();

                    return 1;
                }

                pRtPauseCtx->PauseTTIVal ++;

                break;
            }
            case RT_PAUSE_MODE_SKIP:
            {
                if (pRtPauseCtx->SkipTTIThr == 0)
                {
                    // the parameter of skipping was not enabled,
                    // in this case just resume PHY code

                    pRtPauseCtx->Mode = RT_PAUSE_MODE_ENABLED;
                    break;
                }

                if (pRtPauseCtx->SkipTTIThr == pRtPauseCtx->SkipTTIVal)
                {
                    pRtPauseCtx->SkipTTIVal = 0;
                    pRtPauseCtx->Mode = RT_PAUSE_MODE_ENABLED;

                    return 0;
                }

                uart_printf ("PHYDI-PHY paused (%d)\r\n", pRtPauseCtx->SkipTTIVal);

                pRtPauseCtx->SkipTTIVal++;
                return 1;
            }

            default:
                break;
        }
    }

    return 0;
}

MXRC PhyDiSetStop(void)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    MXRC rc = MX_OK;

    if(pPhyDiCtx->PhyState == PHY_STATE_START)
    {// change state, call shutdown from PHYDiRxHandler
        pPhyDiCtx->PhyState = PHY_STATE_SEND_STOP;
    }

    return (rc);
}

MXRC PhyDiStop(LPVOID pCtx)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    MXRC rc = MX_OK;
    U32 outMsgSize, t;

    if (pPhyDiCtx->PhyState == PHY_STATE_START)
    {
        LtePhyApiThreadStopTimer();

        pPhyDiCtx->PhyState = PHY_STATE_STOP;

        rc |= DiClose(PhyDiHandle);
        uart_printf("\r\nPHYDI-STOP\r\n");

        t = GET_TICKS;

        LteGenerateAPIMsg(0, NULL, &outMsgSize, PHY_STOP_IND, 0);

        LtePhyStopApiFree();

        LteBsPhyStop();

        MLogTask(PID_PHYDI_DISTOP, RESOURCE_LARM, t, GET_TICKS);
    }

    return rc;
}

MXRC PhyDiStopNmm(LPVOID pCtx)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    MXRC rc = MX_OK;

    if (pPhyDiCtx->PhyState == PHY_STATE_START)
    {
        LtePhyApiThreadStopTimer();

        pPhyDiCtx->PhyState = PHY_STATE_STOP;

        rc |= DiClose(PhyDiHandle);
        uart_printf("\r\nPHYDI-STOP\r\n");

        gFirstMessageArrived = 0;
    }

    return rc;
}

UINT32 PhyDiQueryState(void)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();

    return pPhyDiCtx->PhyState;
}


MXRC PhyDiSetShutdown(void)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    MXRC rc = MX_OK;

    if(pPhyDiCtx->PhyState == PHY_STATE_START)
    {// change state, call shutdown from PHYDiRxHandler
        if (PhyDiQueryMode() == AUTO_PCIE)
        {
             PhyDiSetPauseResume(0, 0);
        }
        pPhyDiCtx->PhyState = PHY_STATE_SEND_SHUTDOWN;
    }
    else
    {// already stopped, shutdown right now
        PhyDiShutdown(NULL);
    }

    return (rc);
}

void (*L2StopHndlr)();

MXRC PhyDiShutdown(LPVOID pCtx)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    MXRC rc = MX_OK;
    U32 i;
    U8* pCoreDump;
    U8* pStartCoreDump;
    U32* pCoreDump32;

    if(gMode == UMTS_NWL_MODE)
	{
        if (pPhyDiCtx->PhyState == PHY_STATE_START)
	    {                
            pPhyDiCtx->PhyState = PHY_STATE_STOP;        
            DiClose(PhyDiHandle);
            uart_printf("\r\nPHYDI-STOP\r\n"); 
        }
		if (pPhyDiCtx->PhyState == PHY_STATE_STOP)
		{
		    pPhyDiCtx->PhyState = PHY_STATE_SHUTDOWN;
			MxDelayTicks(15000000);
            uart_printf("\r\nPHYDI-SHUTDOWN\r\n");
			DiShutdown();
		    SvsrLoggerFlush();
            MLogPrint();
		}
		return rc;
    }

    LtePhyApiThreadStopTimer();

    if (pPhyDiCtx->PhyState == PHY_STATE_START)
    {
        PhyDiStop(NULL);
    }

    if (pPhyDiCtx->PhyState == PHY_STATE_STOP)
    {
        pPhyDiCtx->PhyState = PHY_STATE_SHUTDOWN;

        MxDelayTicks(15000000);

        uart_printf("\r\nPHYDI-SHUTDOWN\r\n");

        LtePhyStartApiFree();

        DiShutdown();

        if (L2StopHndlr)
            L2StopHndlr();

        SvsrLoggerFlush();
        MLogPrint();

        LteBsPhyStatsUpdateRexStats();

        if (pCtx == NULL)           // If not null, then it is reconfig message
            LteBsPhyStatsPrint(NULL);

        //  Create coredump array
        pCoreDump = MemAlloc(glbTotalSize+glbNumObj*2*sizeof(U32), DDR1_NCNB_MEM, 0);
        pStartCoreDump = pCoreDump;

        for(i=0; i<glbNumObj; i++)
        {
            pCoreDump32 = (U32*) pCoreDump;
            *pCoreDump32++ = glbObjNum[i];
            *pCoreDump32++ = glbObjSize[i];
            pCoreDump += 2*sizeof(U32);
            memcpy(pCoreDump, (void*) glbObjStart[i], glbObjSize[i]);
            pCoreDump += glbObjSize[i];
        }

        uart_printf("\n\n\rMemory CoreDump of %d Data Objects at 0x%x, size %d\r\n\n\n",
            glbNumObj, pStartCoreDump, glbTotalSize+glbNumObj*2*sizeof(U32));

        MemFree(pStartCoreDump, DDR1_NCNB_MEM);

        glbNumObj = 0;
        glbTotalSize = 0;

        LteBsPhyDestroy(0);
    }

    return rc;
}


static UINT32 NCNBVAR haltCores[4];
static UINT32 NCNBVAR isStopPrintf = 0;

void PhyDiCheckCond(UINT32 cpuId, UINT32 cond)
{
    while(haltCores[cpuId] == cond)
    {
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        MxDelayTicks(150);
    }
}

MXRC PhyDiCoreHalt(LPVOID pCtx)
{
    UINT32 cpuId = MxGetCpuID();

    haltCores[cpuId] = 1;
    PhyDiCheckCond(cpuId, 1);

    return MX_OK;
}

UINT32 PhyDiIsStopPrintf(void)
{
    return isStopPrintf;
}


MXRC PhyDiShutDownHandlerCore0(LPVOID pCtx)
{
    // This Thread was injected from a core as part of stop_printf. So lets kill all other cores before shutdown so that nothing else runs in the ARMs
    // This Thread is only run on core 0

    UINT32 cpuIdFrom = (UINT32)pCtx;        // cpuId from which Shutdown was called
    HANDLE hPhyDiCloseCores[4];
    UINT32 i, numCores;

    isStopPrintf = 1;

    MLogSetMask(0);

    if (MxGetARMID() == 0)
    { /* Quad core */
        numCores = 4;
    }
    else
    { /* Dual Core */
        numCores = 2;
    }

    for (i = 1; i < numCores; i++)     // No need of Master Core
    {
        if (i == cpuIdFrom)     // Already Halted
            continue;

        haltCores[i] = 0;

        MxCreateThread(PhyDiCoreHalt, NULL, NULL, 0, &hPhyDiCloseCores[i]);
        MxInjectThread(i, hPhyDiCloseCores[i]);
    }

    // Wait for the core that is in stop_printf state to update so we get the print on screen first. Then proceed with rest of shutdown
    if (cpuIdFrom != 0)
    {
        PhyDiCheckCond(cpuIdFrom, 0);
    }

    uart_printf("\r\n!!!!SYSTEM HALT PROCEDURE STARTED FROM CORE %d!!!!\r\n\r\n", cpuIdFrom);

    i = 1;
    while(i != numCores)
    {
        if (i != cpuIdFrom)             // Already Halted
        {
            PhyDiCheckCond(i, 0);
        }
        i++;
    }

    // All Other Cores halted. Now we can start to Shutdown everything.
    if (cpuIdFrom == 0)
        return MX_OK;
    else
    {
        PhyDiShutdown(NULL);
        while(1);
    }


}


MXRC PhyDiStopHandler(char* fileName, UINT32 lineNum, char *stopString)
{
    UINT32 cpuID = MxGetCpuID();
    HANDLE hPhyDiStop;
    unsigned int gTxFr = (gLastTxSfn / 10), gTxSfn = (gLastTxSfn % 10);
    unsigned int gRxFr = (gLastRxSfn / 10), gRxSfn = (gLastRxSfn % 10);
    UINT32 t = GET_TICKS;

    haltCores[cpuID] = 0;

    if(cpuID == 0)
    {
        // Disable IRQ
        ARM_INT_disable();

        PhyDiShutDownHandlerCore0((LPVOID)0);

        uart_printf("\r\n[Stopping system and PHY] CPU(%d) at %5d usec (%d ticks) Tx: (%d, %d), Rx: (%d, %d)\r\n",
            cpuID, t/150000,  t, gTxFr, gTxSfn, gRxFr, gRxSfn);
        uart_printf("Called from File: %s at Line: %u.\r\nComment: %s", fileName, lineNum, stopString);

        PhyDiShutdown(NULL);
    }
    else
    {
        MxCreateThread(PhyDiShutDownHandlerCore0, NULL, (LPVOID)cpuID, 0, &hPhyDiStop);
        MxInjectThread(0, hPhyDiStop);

        uart_printf("\r\n[Stopping system and PHY] CPU(%d) at %5d usec (%d ticks) Tx: (%d, %d), Rx: (%d, %d)\r\n",
            cpuID, t/150000,  t, gTxFr, gTxSfn, gRxFr, gRxSfn);
        uart_printf("Called from File: %s at Line: %u.\r\nComment: %s", fileName, lineNum, stopString);
    }

    haltCores[cpuID] = 1;
    PhyDiCheckCond(cpuID, 1);

    return MX_OK;
}

UINT32 PhyDiGetCurrentTTI(void)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();

    return pPhyDiCtx->PhyStartCurrCount;
}

UINT32 PhyDiQueryMode(void)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    return (pPhyDiCtx->PhyStartMode);
}

MXRC PhyDiSetPauseResume(UINT32 PhyPauseResume, UINT32 PhyPauseResumeSkipped)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();

    pPhyDiCtx->PhyPauseResume = PhyPauseResume;
    pPhyDiCtx->PhyPauseResumeSkipped = PhyPauseResumeSkipped;

    return MX_OK;
}

PSINT16 PhyDiGetTxBufPos(UINT32 phyInstance, UINT8 antenna)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    UINT32 loc;
    PSINT16 ret;

    loc = pPhyDiCtx->dlIqJumpSize*antenna;
    ret = (PSINT16)&pPhyDiCtx->dlIqBuf[loc];

    return (ret);
}

UINT32 PhyDiGetTxBufSize(UINT32 phyInstance, UINT8 antenna)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    return (pPhyDiCtx->dlIqBufSize);
}

PSINT16 PhyDiGetRxBufPos(UINT32 phyInstance, UINT8 antenna)
{
    //PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    //PSINT16 ret = (PSINT16)&pPhyDiCtx->ulIqBuf[0];

    //return (ret);
    return (gUlIqBuf);
}

UINT32 PhyDiGetRxBufSize(UINT32 phyInstance, UINT8 antenna)
{
    //PPHYDICTX pPhyDiCtx = PhyDiGetCtx();

    //return (pPhyDiCtx->ulIqBufSize);
    return (iq_samples_size_ul * 2);
}



PUINT8 PhyDiIqBufLoc(PLTEPHYDESC pPhy, U32 direction, U32 nAnt)
{
    PUINT8 pRet = NULL;
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    UINT32 loc;

    if (direction == DL)
    {
        loc = pPhy->txFrameNumber * 10;
        loc += pPhy->pBsTx->subframeNumber;
        loc *= pPhyDiCtx->dlIqSizePerSfn;

        // Circle around
        while (loc >= pPhyDiCtx->dlIqJumpSize)
        {
            loc -= pPhyDiCtx->dlIqJumpSize;
        }
        loc += (pPhyDiCtx->dlIqJumpSize*nAnt);

        pRet = &pPhyDiCtx->dlIqBuf[loc];
    }
    else if (direction == UL)
    {
        loc = pPhy->rxFrameNumber * 10;
        loc += pPhy->pBsRx->subframeNumber;
        loc *= pPhyDiCtx->ulIqSizePerSfn;

        // Circle around
        while (loc >= pPhyDiCtx->ulIqBufSize)
        {
            loc -= pPhyDiCtx->ulIqBufSize;
        }

        pRet = &pPhyDiCtx->ulIqBuf[loc];
    }

    return pRet;
}


void PhyDiCopyDone(LPVOID pCtx)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    PUINT32 pCtxLocal = (PUINT32) pCtx;

    if (pCtx == (LPVOID)&pPhyDiCtx->dlIqCopyTime)
    {
        MLogTask(PID_PHYDI_DLIQ_MDMA_COPY, RESOURCE_RAD_AXI, *pCtxLocal, GET_TICKS);
    }
    else
    {
        ScheduleFftSlot0(FFT_SLOT0_IQ_COPY_DONE);
        MLogTask(PID_PHYDI_ULIQ_MDMA_COPY, RESOURCE_RAD_AXI, *pCtxLocal, GET_TICKS);
    }
    return;
}



MXRC PhyDiCopyIq(PLTEPHYDESC pPhy, UINT32 direction)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    MXRC rc = MX_OK;
    PBS pBs;
    PBS_RX pBsRx;
    PBS_TX pBsTx;

    U32 numBytes, remBytes;
    U32 nBuf = 0;
    U32 i, nAnt;
    MDMA_FDESC * pMdma, *pMdmaCurrent;
    PUINT8 pSrc = NULL, pDst = NULL;
    UINT32 tt = GET_TICKS;

    // We need to do MDMA copy only for Timer mode
    if (pPhyDiCtx->PhyStartMode == AUTO_TIMER ||
        (pPhyDiCtx->PhyStartMode == AUTO_CPRI && pPhyDiCtx->PhyStartPeriod == OPTICAL_LOOPBACK))
    {
        _ASSERT_PTR(pPhy);

        pBs = pPhy->pBs;
        _ASSERT_PTR(pPhy->pBs);

        pBsRx = pPhy->pBsRx;
        _ASSERT_PTR(pPhy->pBsRx);

        pBsTx = pPhy->pBsTx;
        _ASSERT_PTR(pPhy->pBsTx);

        if ((direction != DL) && (direction != UL))
            return 0;

        numBytes = 2 * 2 * 2 * ((pBs->Nfft + pBs->Ncp0) + 6 * (pBs->Nfft + pBs->Ncp));  // Ex: 2 * 2 * 2 for slot, IQ and S16

        //MDMA has a size limit of 64K bytes
        if (numBytes < MAX_MDMA_COPY_SIZE_BYTES)
        {
            remBytes = 0;
            nBuf = 1; //For FFTSizes 1024 and below
        }
        else if (numBytes < MAX_MDMA_COPY_SIZE_BYTES*2)
        {
            remBytes = numBytes - MAX_MDMA_COPY_SIZE_BYTES;
            nBuf = 2; //For FFTSize 1536 and 2048
        }
        else
        {
            remBytes = numBytes - (MAX_MDMA_COPY_SIZE_BYTES*2);
            nBuf = 3; //For FFTSize greater then 2048
        }


        if (direction == DL)
        {
            _ASSERT_PTR(pBsTx->ifftOut);
            nAnt = pBsTx->Ntx_antennas;
        }
        else
        {
            _ASSERT_PTR(pBsRx->adcbuf);
            nAnt = pBsRx->Nrx_antennas;
            pSrc = (PUINT8)PhyDiIqBufLoc(pPhy, UL, 0);               // To be copied from DDR to CRAM
        }


        pMdma = MdmaAllocDesc (MDMA_HW_RAD, nBuf * nAnt);
        _ASSERT_PTR(pMdma);
        pMdmaCurrent = pMdma;

        for (i = 0; i < nAnt; i++)
        {

            if (direction == DL)
            {
                pSrc = (PUINT8)pBsTx->ifftOut[i];                   // In CRAM populated by MAP
                pDst = (PUINT8)PhyDiIqBufLoc(pPhy, DL, i);          // To be written to DDR from CRAM
            }
            else
            {
                pDst = (PUINT8)pBsRx->adcbuf[i];            // TO be copied to CRAM will be read by MAP
            }

            _ASSERT_PTR(pSrc);
            _ASSERT_PTR(pDst);

            //For FFTSizes 1024 and below
            if (nBuf == 1)
            {
                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, numBytes, (U32*) pDst, numBytes);
                pSrc += numBytes;
                pDst += numBytes;
            }

            //For FFTSizes 1536 and 2048
            if (nBuf == 2)
            {
                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, MAX_MDMA_COPY_SIZE_BYTES, (U32*) pDst, MAX_MDMA_COPY_SIZE_BYTES);

                pMdmaCurrent = pMdmaCurrent->Next;
                pSrc += MAX_MDMA_COPY_SIZE_BYTES;
                pDst += MAX_MDMA_COPY_SIZE_BYTES;

                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, remBytes, (U32*) pDst, remBytes);
                pSrc += remBytes;
                pDst += remBytes;
            }

            //For FFTSizes greater then 2048
            if (nBuf == 3)
            {
                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, MAX_MDMA_COPY_SIZE_BYTES, (U32*) pDst, MAX_MDMA_COPY_SIZE_BYTES);

                pMdmaCurrent = pMdmaCurrent->Next;
                pSrc += MAX_MDMA_COPY_SIZE_BYTES;
                pDst += MAX_MDMA_COPY_SIZE_BYTES;

                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, MAX_MDMA_COPY_SIZE_BYTES, (U32*) pDst, MAX_MDMA_COPY_SIZE_BYTES);


                pMdmaCurrent = pMdmaCurrent->Next;
                pSrc += MAX_MDMA_COPY_SIZE_BYTES;
                pDst += MAX_MDMA_COPY_SIZE_BYTES;

                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, remBytes, (U32*) pDst, remBytes);
                pSrc += remBytes;
                pDst += remBytes;
            }

            pMdmaCurrent = pMdmaCurrent->Next;
        }

        if (direction == DL)
        {
            pPhyDiCtx->dlIqCopyTime = GET_TICKS;
            // non blocking copy
            MdmaRunEx (MDMA_HW_RAD, pMdma, PhyDiCopyDone, &pPhyDiCtx->dlIqCopyTime);
        }
        else
        {
            pPhyDiCtx->ulIqCopyTime = GET_TICKS;
            // non blocking copy
            MdmaRunEx (MDMA_HW_RAD, pMdma, PhyDiCopyDone, &pPhyDiCtx->ulIqCopyTime);
        }

        MLogTask(PID_PHYDI_COPYIQ_TIMER_MODE, RESOURCE_LARM, tt, GET_TICKS);

    }
    else
    {
        if (direction == UL)
            ScheduleFftSlot0(FFT_SLOT0_IQ_COPY_DONE);
    }
    return rc;
}

volatile NCNBVAR U16 nmm_iq_copy_done;

void PhyDiCopyNmmDone(LPVOID pCtx)
{
    nmm_iq_copy_done = 1;
}

void PhyDiCopyIqNMM(void)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    PLTEPHYDESC pPhyList = getPhyCtxListProc(0);
    PLTEPHYDESC pPhyApi = getPhyCtxApiProc(0);
    PBS pBs;
    PBS_RX pBsRx;
    U32 loc,ulIqBuf_end;

    U32 numBytes, remBytes;
    U32 nBuf = 0;
    U32 i, nAnt;
    MDMA_FDESC * pMdma, *pMdmaCurrent;
    PUINT8 pSrc = NULL, pDst = NULL;
    UINT32 tt = GET_TICKS;
    UINT32 ulIqSizePerSfn ;
#define IQ_BUF_DEBUG 1

#if IQ_BUF_DEBUG
        nAnt = 1;
#else        
        nAnt = pBsRx->Nrx_antennas;        
#endif
    // We need to do MDMA copy only for Timer mode
    if (pPhyDiCtx->PhyStartMode == AUTO_TIMER ||
        (pPhyDiCtx->PhyStartMode == AUTO_CPRI && pPhyDiCtx->PhyStartPeriod == OPTICAL_LOOPBACK))
    {
        _ASSERT_PTR(pPhyList);

        pBs = pPhyList->pBs;
        _ASSERT_PTR(pPhyList->pBs);

        pBsRx = pPhyList->pBsRx;
        _ASSERT_PTR(pPhyList->pBsRx);

        numBytes = 2 * 2 * 2 * ((pBs->Nfft + pBs->Ncp0) + 6 * (pBs->Nfft + pBs->Ncp));  // Ex: 2 * 2 * 2 for slot, IQ and S16
        ulIqSizePerSfn = numBytes * nAnt;

        //MDMA has a size limit of 64K bytes
        if (numBytes < MAX_MDMA_COPY_SIZE_BYTES)
        {
            remBytes = 0;
            nBuf = 1; //For FFTSizes 1024 and below
        }
        else if (numBytes < MAX_MDMA_COPY_SIZE_BYTES*2)
        {
            remBytes = numBytes - MAX_MDMA_COPY_SIZE_BYTES;
            nBuf = 2; //For FFTSize 1536 and 2048
        }
        else
        {
            remBytes = numBytes - (MAX_MDMA_COPY_SIZE_BYTES*2);
            nBuf = 3; //For FFTSize greater then 2048
        }

        _ASSERT_PTR(pBsRx->adcbuf);

        /*determine the location in UL IQ buffer*/
        // To be copied from DDR to CRAM

        // Circle around
        ulIqBuf_end = 10240*ulIqSizePerSfn;
        while(ulIqBuf_end > pPhyDiCtx->ulIqBufSize)
        {
            ulIqBuf_end >>= 1;
        }


#if IQ_BUF_DEBUG
        ulIqBuf_end = 320*ulIqSizePerSfn;
#endif
		loc = nmm_subframe_count * ulIqSizePerSfn;

        while (loc >= ulIqBuf_end)
        {
            loc -= ulIqBuf_end;
        }

        pSrc = &pPhyDiCtx->ulIqBuf[loc];


        pMdma = MdmaAllocDesc (MDMA_HW_RAD, nBuf * nAnt);
        _ASSERT_PTR(pMdma);
        pMdmaCurrent = pMdma;

        for (i = 0; i < nAnt; i++)
        {
            pDst = (PUINT8)pBsRx->adcbuf[i];            // TO be copied to CRAM will be read by MAP

            _ASSERT_PTR(pSrc);
            _ASSERT_PTR(pDst);

            //For FFTSizes 1024 and below
            if (nBuf == 1)
            {
                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, numBytes, (U32*) pDst, numBytes);
                pSrc += numBytes;
                pDst += numBytes;
            }

            //For FFTSizes 1536 and 2048
            if (nBuf == 2)
            {
                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, MAX_MDMA_COPY_SIZE_BYTES, (U32*) pDst, MAX_MDMA_COPY_SIZE_BYTES);

                pMdmaCurrent = pMdmaCurrent->Next;
                pSrc += MAX_MDMA_COPY_SIZE_BYTES;
                pDst += MAX_MDMA_COPY_SIZE_BYTES;

                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, remBytes, (U32*) pDst, remBytes);
                pSrc += remBytes;
                pDst += remBytes;
            }

            //For FFTSizes greater then 2048
            if (nBuf == 3)
            {
                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, MAX_MDMA_COPY_SIZE_BYTES, (U32*) pDst, MAX_MDMA_COPY_SIZE_BYTES);

                pMdmaCurrent = pMdmaCurrent->Next;
                pSrc += MAX_MDMA_COPY_SIZE_BYTES;
                pDst += MAX_MDMA_COPY_SIZE_BYTES;

                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, MAX_MDMA_COPY_SIZE_BYTES, (U32*) pDst, MAX_MDMA_COPY_SIZE_BYTES);


                pMdmaCurrent = pMdmaCurrent->Next;
                pSrc += MAX_MDMA_COPY_SIZE_BYTES;
                pDst += MAX_MDMA_COPY_SIZE_BYTES;

                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, remBytes, (U32*) pDst, remBytes);
                pSrc += remBytes;
                pDst += remBytes;
            }

            pMdmaCurrent = pMdmaCurrent->Next;
        }

        pPhyDiCtx->ulIqCopyTime = GET_TICKS;
        // blocking copy
        nmm_iq_copy_done = 0;
        MdmaRunEx (MDMA_HW_RAD, pMdma, PhyDiCopyNmmDone, NULL);
        while(nmm_iq_copy_done == 0);

        MLogTask(PID_PHYDI_COPYIQ_TIMER_MODE, RESOURCE_LARM, tt, GET_TICKS);

        /* update sfn because rx thread not run in nmm case */
        pPhyList->pBsRx->subframeNumber++;
        pPhyApi->pBsRx->subframeNumber = pPhyList->pBsRx->subframeNumber;
        if((pPhyList->pBsRx->subframeNumber % 10) == 0) {
            pPhyList->pBsRx->subframeNumber = 0;
            pPhyApi->pBsRx->subframeNumber = pPhyList->pBsRx->subframeNumber;
            pPhyList->rxFrameNumber++;
            pPhyApi->rxFrameNumber = pPhyList->rxFrameNumber;
        }
    }
}



/*UMTSNMM*/

volatile NCNBVAR U16 UMTSnmm_iq_copy_done;

void PhyDiCopyUmtsNmmDone(LPVOID pCtx)
{
    UMTSnmm_iq_copy_done = 1;
}



void PhyDiCopyIqUMTSNMM(PUINT8 sigIn)
{
    PPHYDICTX pPhyDiCtx = PhyDiGetCtx();
    //PBS pBs;

    U32 numBytes, remBytes;
    U32 nBuf = 0;
    U32 i, nAnt;
    MDMA_FDESC * pMdma, *pMdmaCurrent;
    PUINT8 pSrc = NULL, pDst = NULL;
    PUINT8 pinitSrc = NULL;
    UINT32 tt = GET_TICKS;
    UINT32 loc,ulIqBuf_end;

    // We need to do MDMA copy only for Timer mode
    if (DI_CPRI_EMULATOR ==gUmtsnmmMode)
    {

        numBytes = 3840*4*2*sizeof(S16);  //3840:1ms ,4: sample rate, 2: IQ

        //MDMA has a size limit of 64K bytes
        if (numBytes < MAX_MDMA_COPY_SIZE_BYTES)
        {
            remBytes = 0;
            nBuf = 1; //For FFTSizes 1024 and below
        }
        else if (numBytes < MAX_MDMA_COPY_SIZE_BYTES*2)
        {
            remBytes = numBytes - MAX_MDMA_COPY_SIZE_BYTES;
            nBuf = 2; //For FFTSize 1536 and 2048
        }
        else
        {
            remBytes = numBytes - (MAX_MDMA_COPY_SIZE_BYTES*2);
            nBuf = 3; //For FFTSize greater then 2048
        }

        
        nAnt = 1;
        pinitSrc = (PUINT8)gUlIqBuf;               // To be copied from DDR to CRAM

        // Circle around
        ulIqBuf_end = 1300*pPhyDiCtx->ulIqSizePerSfn; // 1300: 130 frames
        while(ulIqBuf_end > pPhyDiCtx->ulIqBufSize)
        {
            ulIqBuf_end >>= 1;
        }

        loc = UMTSNwl_subframe_count * pPhyDiCtx->ulIqSizePerSfn;
		

        while (loc >= ulIqBuf_end)
        {
            loc -= ulIqBuf_end;
        }
        pSrc = &pPhyDiCtx->ulIqBuf[loc];       
        
        pMdma = MdmaAllocDesc (MDMA_HW_RAD, nBuf * nAnt);
        _ASSERT_PTR(pMdma);
        pMdmaCurrent = pMdma;

        for (i = 0; i < nAnt; i++)
        {
            pDst = sigIn;            // TO be copied to CRAM will be read by MAP

            _ASSERT_PTR(pSrc);
            _ASSERT_PTR(pDst);

            //For FFTSizes 1024 and below
            if (nBuf == 1)
            {
                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, numBytes, (U32*) pDst, numBytes);
                pSrc += numBytes;
                pDst += numBytes;
            }

            //For FFTSizes 1536 and 2048
            if (nBuf == 2)
            {
                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, MAX_MDMA_COPY_SIZE_BYTES, (U32*) pDst, MAX_MDMA_COPY_SIZE_BYTES);

                pMdmaCurrent = pMdmaCurrent->Next;
                pSrc += MAX_MDMA_COPY_SIZE_BYTES;
                pDst += MAX_MDMA_COPY_SIZE_BYTES;

                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, remBytes, (U32*) pDst, remBytes);
                pSrc += remBytes;
                pDst += remBytes;
            }

            //For FFTSizes greater then 2048
            if (nBuf == 3)
            {
                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, MAX_MDMA_COPY_SIZE_BYTES, (U32*) pDst, MAX_MDMA_COPY_SIZE_BYTES);

                pMdmaCurrent = pMdmaCurrent->Next;
                pSrc += MAX_MDMA_COPY_SIZE_BYTES;
                pDst += MAX_MDMA_COPY_SIZE_BYTES;

                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, MAX_MDMA_COPY_SIZE_BYTES, (U32*) pDst, MAX_MDMA_COPY_SIZE_BYTES);


                pMdmaCurrent = pMdmaCurrent->Next;
                pSrc += MAX_MDMA_COPY_SIZE_BYTES;
                pDst += MAX_MDMA_COPY_SIZE_BYTES;

                MdmaSetCopy(pMdmaCurrent);
                MdmaSetData(pMdmaCurrent, (U32*) pSrc, remBytes, (U32*) pDst, remBytes);
                pSrc += remBytes;
                pDst += remBytes;
            }

            pMdmaCurrent = pMdmaCurrent->Next;
        }

        // non blocking copy
        UMTSnmm_iq_copy_done = 0;
        MdmaRunEx (MDMA_HW_RAD, pMdma, PhyDiCopyUmtsNmmDone, NULL);
        while(UMTSnmm_iq_copy_done == 0);
    }
}

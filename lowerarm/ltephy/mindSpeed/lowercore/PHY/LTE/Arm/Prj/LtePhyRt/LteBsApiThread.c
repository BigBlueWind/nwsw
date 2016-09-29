//-------------------------------------------------------------------------------------------
/** @file LteBsApiThread.c
 *
 * @brief Implementation of API Parsing and Dispatching for the LTE BS
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.283 $
 *
 * COPYRIGHT&copy; 2009-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#include "ltephy.h"
#include "ltephydef.h"
#include "Debug.h"
#include "prototypes.h"
#include "supervisor.h"
#include "heaps.h"
#include "apimgr.h"
#include "larmmsgid.h"
#include "LteBsApiThread.h"
#include "LteBsRxThread.h"
#include "LteBsTxThread.h"
#include "LteBsRxDataSdu.h"
#include "LteBsTxDataSdu.h"
#include "LteBsRxConfig.h"
#include "LteBsPhyStats.h"
#include "LteBsApiErrorCheck.h"
#include "ltertconfig.h"
#include "ltemac.h"
#include "ext_phy.h"
#include "LteBsTxThread.h"
#include "timerdrv.h"
#include "basic_op_cc.h"
#include "lte_entry.h"
#include "fpdrv.h"
#include "globalstoragearrays.h"
#include "dataint.h"
#include "ceva_proc.h"
#include "mlog.h"

static LTE_PHY_API_THREAD_CTX LtePhyApiCtx;

static FIFO_STACK MacToPhyApiStack;
static FIFO_STACK PhyApiLogAndCleanUp;
static UINT32 tMapSetupDesc;

extern volatile UINT32 LteBsApiErrorCheckDispatchRunning;

volatile UINT32 TTI_start;

void MapSetupDescriptorsTask_done(void* ptr, UINT32 ceva_ret);

U32 LteBsApiThreadSetupMapVars(void)
{
    PMAP_DRV_CTX          pMapDrvCtx = MapGetDrvCtx();
    PLTEPHYDESC           pPhyList, pPhyApi, pPhy1, pPhy2;
    PMAP_SETUP_DESCRIPTOR pMapSetupDescList, pMapSetupDescApi;
    PBS                   pBs1;
    PBS_TX                pBsTx1;
    PBS_RX                pBsRx1;
    MAP_DMA_TR_CTRL       trCtrl;
    PUINT8                pAllocPtr;
    U32                   i, j, offset, K, step_reDet, isym;
    U32                   step_aDet, mo, L, MO;
    U32                   Idx2, stepsize_a, mi, MI;

    pPhyList = getPhyCtxListProc(0);
    _ASSERT_PTR(pPhyList);
    pPhyApi  = getPhyCtxApiProc(0);
    _ASSERT_PTR(pPhyApi);

    pMapSetupDescList = pPhyList->pBsRx->pMapSetupDesc;
    _ASSERT_PTR(pMapSetupDescList);
    pMapSetupDescApi  = pPhyApi->pBsRx->pMapSetupDesc;
    _ASSERT_PTR(pMapSetupDescApi);

    MapFttIfftDrvFlushQueue();

    // Initialize the IQ Sample location in CRAM for FFT Input and IFFT Output
    //--------------------------------------------------------------
    for(i = 0; i < pPhyApi->pBsTx->Ntx_antennas; i++)
    {
        if (gLowLatencyFlag)
        {
            pPhyList->pBsTx->ifftOut[i] = (S16*)QueryCramIqBuf(DL, 0, i);
            pPhyApi->pBsTx->ifftOut[i]  = (S16*)QueryCramIqBuf(DL, 1, i);
        }
        else
        {
            pPhyList->pBsTx->ifftOut[i] = (S16*)QueryCramIqBuf(DL, 1, i);
            pPhyApi->pBsTx->ifftOut[i]  = (S16*)QueryCramIqBuf(DL, 0, i);
        }
    }

    for(i = 0; i < pPhyApi->pBsRx->Nrx_antennas; i++)
    {
        pPhyList->pBsRx->adcbuf[i] = (S16*)QueryCramIqBuf(UL, 1, i);
        pPhyApi->pBsRx->adcbuf[i]  = (S16*)QueryCramIqBuf(UL, 0, i);
    }

    // Set IDFT Driver Input / Output Buffers
    //----------------------------------------------
    K          = pPhyList->pBsRx->Nul_rb * pPhyList->pBs->Nrb_sc; //total number of frequency indices in resource grid
    step_reDet = 2 * RUP8B(K + 4*MAXCHSUBFRAME);

    for (isym = 0; isym < (U32)(2*pPhyList->pBsRx->isym_eff[0]); isym++)
    {
        MapIdftDrvSetInBuf((PUINT8)&pPhyList->pBsRx->pBsRxPointers->pReDet0[isym * step_reDet], isym, step_reDet*2);
        MapIdftDrvSetOutBuf((PUINT8)&pPhyList->pBsRx->pBsRxPointers->pReDet0[isym * step_reDet], isym, step_reDet*2);
    }

    // Setup Map Descriptor Lists
    //---------------------------------

    // FFT / IFFT Stuff
    pMapSetupDescList->numRxAnt                        = pPhyList->pBsRx->Nrx_antennas;
    pMapSetupDescList->numTxAnt                        = pPhyList->pBsTx->Ntx_antennas;
    pMapSetupDescList->fftIfftSize                     = pPhyList->pBs->Nfft;
    memcpy(&pMapSetupDescList->FftInTrCtrl,    &pMapDrvCtx->FftInTrCtrl, sizeof(MAP_DMA_TR_CTRL_CEVA));
    memcpy(&pMapSetupDescList->FftInTrCtrl128, &pMapDrvCtx->FftInTrCtrl128, sizeof(MAP_DMA_TR_CTRL_CEVA));
    memcpy(&pMapSetupDescList->IfftInTrCtrl0,  &pMapDrvCtx->IfftInTrCtrl[0], sizeof(MAP_DMA_TR_CTRL_CEVA));
    memcpy(&pMapSetupDescList->IfftInTrCtrl1,  &pMapDrvCtx->IfftInTrCtrl[1], sizeof(MAP_DMA_TR_CTRL_CEVA));
    for (i = 0; i < 4; i++)
        pMapSetupDescList->fftIfftCtxNum[i]   = 0;
    pMapSetupDescList->pFftFDescHead0  = pMapDrvCtx->pFftDmaDesc + (pMapSetupDescList->fftIfftCtxNum[FFT_SLOT0]*FFT_DRV_QSIZE*2);
    pMapSetupDescList->pFftFDescHead1  = pMapDrvCtx->pFftDmaDesc + (pMapSetupDescList->fftIfftCtxNum[FFT_SLOT1]*FFT_DRV_QSIZE*2);
    pMapSetupDescList->pIfftFDescHead0 = pMapDrvCtx->pIfftDmaDesc + (pMapSetupDescList->fftIfftCtxNum[IFFT_SLOT0]*IFFT_DRV_QSIZE*2);

    // Tables
    pMapSetupDescList->fp_idft_big_config_start        = (U16*)fp_idft_big_config_start;
    pMapSetupDescList->fp_idft_mult_table_start        = (U16*)fp_idft_mult_table_start;
    pMapSetupDescList->fp_idft_size_index_map          = (U8*) fp_idft_size_index_map;
    pMapSetupDescList->fp_idft_table_sizes_with_offset = (U16*)fp_idft_table_sizes_with_offset;
    for (i = 0; i < 35; i++)
        pMapSetupDescList->fp_idft_tables[i] = (U16*)fp_idft_tables[i];

    // Driver
    pMapSetupDescList->IdftFpStart    = pMapDrvCtx->IdftFpStart;
    pMapSetupDescList->IdftFpStop     = pMapDrvCtx->IdftFpStop;
    pMapSetupDescList->IdftOutputSize = pMapDrvCtx->IdftOutputSize;
    for (j = 0; j < FPDRV_NUM_FP; j++)
    {
        for (i = 0; i < 3; i++)
        {
            trCtrl      = pMapDrvCtx->IdftInTrCtrl[i];
            trCtrl.FpId = j;
            memcpy(&pMapSetupDescList->IdftInTrCtrl[j][i], &trCtrl, sizeof(MAP_DMA_TR_CTRL_CEVA));
        }
        trCtrl      = pMapDrvCtx->IdftOutTrCtrl;
        trCtrl.FpId = j;
        memcpy(&pMapSetupDescList->IdftOutTrCtrl[j], &trCtrl, sizeof(MAP_DMA_TR_CTRL_CEVA));
    }
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < FPDRV_NUM_FP; j++)
        {
            pMapSetupDescList->pReadyListHead[j] = NULL;
            pMapSetupDescList->pReadyListTail[j] = NULL;
        }
    }

    // MAP Tables
    pAllocPtr = (PUINT8)pPhyList->pBsRx->pIdftDriverBuf;
    offset = 0;
    for (j = 0; j < MAX_IDFT_TRANSACTIONS_PER_SYMBOL; j++)
    {
        pMapSetupDescList->pConfigTables[j] = (U8*)MemAllocFrom(pAllocPtr, &offset, IDFT_CONFIG_TABLE_SIZE); // 8 for 2 Loop counters
        pMapSetupDescList->pMultTables[j]   = (U8*)MemAllocFrom(pAllocPtr, &offset, IDFT_MULT_TABLE_SIZE);
    }


    // Other Context
    // FFT / IFFT Stuff
    pMapSetupDescApi->numRxAnt                        = pPhyList->pBsRx->Nrx_antennas;
    pMapSetupDescApi->numTxAnt                        = pPhyList->pBsTx->Ntx_antennas;
    pMapSetupDescApi->fftIfftSize                     = pPhyList->pBs->Nfft;
    memcpy(&pMapSetupDescApi->FftInTrCtrl,    &pMapDrvCtx->FftInTrCtrl, sizeof(MAP_DMA_TR_CTRL_CEVA));
    memcpy(&pMapSetupDescApi->FftInTrCtrl128, &pMapDrvCtx->FftInTrCtrl128, sizeof(MAP_DMA_TR_CTRL_CEVA));
    memcpy(&pMapSetupDescApi->IfftInTrCtrl0,  &pMapDrvCtx->IfftInTrCtrl[0], sizeof(MAP_DMA_TR_CTRL_CEVA));
    memcpy(&pMapSetupDescApi->IfftInTrCtrl1,  &pMapDrvCtx->IfftInTrCtrl[1], sizeof(MAP_DMA_TR_CTRL_CEVA));
    for (i = 0; i < 4; i++)
        pMapSetupDescApi->fftIfftCtxNum[i]   = 1;
    pMapSetupDescApi->pFftFDescHead0  = pMapDrvCtx->pFftDmaDesc + (pMapSetupDescApi->fftIfftCtxNum[FFT_SLOT0]*FFT_DRV_QSIZE*2);
    pMapSetupDescApi->pFftFDescHead1  = pMapDrvCtx->pFftDmaDesc + (pMapSetupDescApi->fftIfftCtxNum[FFT_SLOT1]*FFT_DRV_QSIZE*2);
    pMapSetupDescApi->pIfftFDescHead0 = pMapDrvCtx->pIfftDmaDesc + (pMapSetupDescApi->fftIfftCtxNum[IFFT_SLOT0]*IFFT_DRV_QSIZE*2);

    // Tables
    pMapSetupDescApi->fp_idft_big_config_start        = (U16*)fp_idft_big_config_start;
    pMapSetupDescApi->fp_idft_mult_table_start        = (U16*)fp_idft_mult_table_start;
    pMapSetupDescApi->fp_idft_size_index_map          = (U8*) fp_idft_size_index_map;
    pMapSetupDescApi->fp_idft_table_sizes_with_offset = (U16*)fp_idft_table_sizes_with_offset;
    for (i = 0; i < 35; i++)
        pMapSetupDescApi->fp_idft_tables[i] = (U16*)fp_idft_tables[i];

    // Driver
    pMapSetupDescApi->IdftFpStart    = pMapDrvCtx->IdftFpStart;
    pMapSetupDescApi->IdftFpStop     = pMapDrvCtx->IdftFpStop;
    pMapSetupDescApi->IdftOutputSize = pMapDrvCtx->IdftOutputSize;
    for (j = 0; j < FPDRV_NUM_FP; j++)
    {
        for (i = 0; i < 3; i++)
        {
            trCtrl      = pMapDrvCtx->IdftInTrCtrl[i];
            trCtrl.FpId = j;
            memcpy(&pMapSetupDescApi->IdftInTrCtrl[j][i], &trCtrl, sizeof(MAP_DMA_TR_CTRL_CEVA));
        }
        trCtrl      = pMapDrvCtx->IdftOutTrCtrl;
        trCtrl.FpId = j;
        memcpy(&pMapSetupDescApi->IdftOutTrCtrl[j], &trCtrl, sizeof(MAP_DMA_TR_CTRL_CEVA));
    }
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < FPDRV_NUM_FP; j++)
        {
            pMapSetupDescApi->pReadyListHead[j] = NULL;
            pMapSetupDescApi->pReadyListTail[j] = NULL;
        }
    }

    // MAP Tables
    pAllocPtr = (PUINT8)pPhyApi->pBsRx->pIdftDriverBuf;
    offset    = 0;
    for (j = 0; j < MAX_IDFT_TRANSACTIONS_PER_SYMBOL; j++)
    {
        pMapSetupDescApi->pConfigTables[j] = (U8*)MemAllocFrom(pAllocPtr, &offset, IDFT_CONFIG_TABLE_SIZE); // 8 for 2 Loop counters
        pMapSetupDescApi->pMultTables[j]   = (U8*)MemAllocFrom(pAllocPtr, &offset, IDFT_MULT_TABLE_SIZE);
    }

    // Set IFFT /FFT Driver Input / Output Buffers
    // Context 1
    if (gLowLatencyFlag)
    {
        pPhy1        = pPhyApi;
        pPhy2        = pPhyList;
    }
    else
    {
        pPhy1        = pPhyList;
        pPhy2        = pPhyList;
    }

    pBs1         = pPhy1->pBs;
    pBsTx1       = pPhy1->pBsTx;
    pBsRx1       = pPhy2->pBsRx;

    MI           = pBsTx1->Ntx_antennas;
    MO           = pBsRx1->Nrx_antennas;
    stepsize_a   = RUP32B(pBsTx1->Ndl_rb * pBs1->Nrb_sc);
    Idx2         = stepsize_a * (pBsTx1->Ndl_symb << 1) * 2;
    L            = 2 * pBsRx1->Nul_symb; //total number of time indices
    K            = pBsRx1->Nul_rb * pBs1->Nrb_sc; //total number of frequency indices in resource grid
    step_aDet    = 2*RUP8B(K+2);

    for (mi = 0; mi < MI; mi++)
    {
        MapIfftDrvSetInpBuf((PUINT32)(pBsTx1->a+(Idx2*mi)), 14, mi*14);
        MapIfftDrvSetOutBuf((PUINT32)(pBsTx1->ifftOut[mi]), 14, mi*14);
    }
    for(mo = 0; mo < MO; mo++)
    {
        MapFftDrvSetInpBuf((PUINT32)pBsRx1->adcbuf[mo], 14, mo*14);
        MapFftDrvSetOutBuf((PUINT32)&pBsRx1->aDet[mo*step_aDet*L], 14, mo*14);
    }
    MapFftIfftDrvContextSwap((SWAP_FFT | SWAP_IFFT));

    // Context 2
    if (gLowLatencyFlag)
    {
        pPhy1       = pPhyList;
        pPhy2       = pPhyApi;
    }
    else
    {
        pPhy1       = pPhyApi;
        pPhy2       = pPhyApi;
    }

    pBs1        = pPhy1->pBs;
    pBsTx1      = pPhy1->pBsTx;
    pBsRx1      = pPhy2->pBsRx;

    MI          = pBsTx1->Ntx_antennas;
    MO          = pBsRx1->Nrx_antennas;
    stepsize_a  = RUP32B(pBsTx1->Ndl_rb * pBs1->Nrb_sc);
    Idx2        = stepsize_a * (pBsTx1->Ndl_symb << 1) * 2;
    L           = 2 * pBsRx1->Nul_symb; //total number of time indices
    K           = pBsRx1->Nul_rb * pBs1->Nrb_sc; //total number of frequency indices in resource grid
    step_aDet   = 2*RUP8B(K+2);

    for (mi = 0; mi < MI; mi++)
    {
        MapIfftDrvSetInpBuf((PUINT32)(pBsTx1->a+(Idx2*mi)), 14, mi*14);
        MapIfftDrvSetOutBuf((PUINT32)(pBsTx1->ifftOut[mi]), 14, mi*14);
    }
    for(mo = 0; mo < MO; mo++)
    {
        MapFftDrvSetInpBuf((PUINT32)pBsRx1->adcbuf[mo], 14, mo*14);
        MapFftDrvSetOutBuf((PUINT32)&pBsRx1->aDet[mo*step_aDet*L], 14, mo*14);
    }

    if (gLowLatencyFlag)
        MapFftIfftDrvContextSwap(SWAP_FFT);
    else
        MapFftIfftDrvContextSwap((SWAP_FFT | SWAP_IFFT));

    // Setup Map Driver Variables to run with PHY
    MapIfftDrvSetNumSym(14);
    MapIfftDrvSetNumAntennas(MI);
    MapFftDrvSetNumAntennas(MO);

    MapIfftDrvSetCallback((MAPDONECB)LteTxIfftDoneCb, NULL);

    MapIdftUseQueueFromTcb(1);

    pPhyApi->pBsRx->pMapSetupDesc->ArrSize = 0;
    MapSetupDescriptorsTask((U32)pPhyApi->pBsRx->pMapSetupDesc);
    tMapSetupDesc = GET_TICKS;
    MapSetupDescriptorsTask_done(pPhyApi->pBsRx->pMapSetupDesc, 0);

    pPhyList->pBsRx->pMapSetupDesc->ArrSize = 0;
    MapSetupDescriptorsTask((U32)pPhyList->pBsRx->pMapSetupDesc);
    tMapSetupDesc = GET_TICKS;
    MapSetupDescriptorsTask_done(pPhyList->pBsRx->pMapSetupDesc, 0);

    return 0;
}

#ifdef SLOT_BASED_FFT
void ScheduleFftSlot0(UINT32 mask)
{
    PLTEPHYDESC pPhy;

    gRunFftSlot0 |= mask;
    if ((gRunFftSlot0 & SCHEDULE_FFT) == SCHEDULE_FFT)
    {
        pPhy = getPhyCtxApiProc(0);
        MapFftDrvRun(pPhy->pBsRx->pFftTcb[0]);
        gRunFftSlot0 = 0;
    }
}
#else
void ScheduleFftSlot0(UINT32 mask)
{
    return;
}
#endif

void MapSetupDescriptorsTask_done(void* ptr, UINT32 ceva_ret)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    PMAP_SETUP_DESCRIPTOR pMapSetupDesc = (PMAP_SETUP_DESCRIPTOR)ptr;
    UINT32 i;

    _ASSERT_PTR(pMapDrvCtx);
    _ASSERT_PTR(pMapSetupDesc);

    if (ceva_ret != 0)
    {
        stop_printf("Something is wrong with Ceva function MapSetupDescriptorsTask retCode: 0x%x\r\n", ceva_ret);
        return;
    }

    // FFT
    for (i = FFT_SLOT0; i < IFFT_SLOT0; i++)
    {
        (pMapDrvCtx->pReadyListFftIfftTail[pMapSetupDesc->fftIfftCtxNum[i]][i] + 1)->FControl = 0x1;    // Set Interrupt
        pMapDrvCtx->UpdateQueueFftIfft[pMapSetupDesc->fftIfftCtxNum[i]][i] = 1;
    }

    // IFFT
    (pMapDrvCtx->pReadyListFftIfftTail[pMapSetupDesc->fftIfftCtxNum[IFFT_SLOT0]][IFFT_SLOT0] + 1)->FControl = 0x1;    // Set Interrupt
    pMapDrvCtx->UpdateQueueFftIfft[pMapSetupDesc->fftIfftCtxNum[IFFT_SLOT0]][IFFT_SLOT0] = 1;

    ScheduleFftSlot0(FFT_SLOT0_DESCRIPTOR_SETUP_DONE);

    MLogTask(PID_RXLIST_IDFT_FFT_UPDATE_DESC_IN_CEVA, RESOURCE_LARM, tMapSetupDesc, GET_TICKS);
}

U32 UpdateMapDriverDescriptors(PBS_RX pBsRx)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    PMAP_SETUP_DESCRIPTOR pMapSetupDesc;
    U32 hSetupBufs;
    PULRXINPUTPARAMS pUlRxInputParams;

    tMapSetupDesc = GET_TICKS;
    pUlRxInputParams = pBsRx->pUlRxInputParams;

    pMapSetupDesc = pBsRx->pMapSetupDesc;
    pMapSetupDesc->pIdftSizeArr    = (U16*)pUlRxInputParams->Mpusch_sc;
    pMapSetupDesc->ArrSize         = pUlRxInputParams->numPUSCH;
    pMapSetupDesc->SymMask         = 0xFFF;
    pMapSetupDesc->pIdftFDescHead  = pMapDrvCtx->pIdftDmaDesc + (pMapDrvCtx->IdftQueueToRun * IDFT_DRV_QSIZE * MAX_NUM_IDFT_DMA_DESC);
    pMapDrvCtx->IdftQueueToRun     = 1 - pMapDrvCtx->IdftQueueToRun;
#if 0
    MapSetupDescriptorsTask((U32)pMapSetupDesc);
    MapSetupDescriptorsTask_done(pMapSetupDesc, 0);
#else
    hSetupBufs = ceva_call1_cb(MapSetupDescriptorsTask_done, (void*)pBsRx->pMapSetupDesc, ceva_MapSetupDescriptorsTask, (UINT32) pBsRx->pMapSetupDesc);
    if(!hSetupBufs)
    {
        stop_printf("Failed to start ceva_MapSetupDescriptorsTask function on Ceva\r\n");
        return 1;
    }
#endif

    return 0;
}


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Get Pointer to LTE PHY API Context
 *
 *
 *  @param   none
 *
 *  @return    Pointer to LTE PHY API Context
 *
 *  \ingroup group_lte_phy_control_rt
 *
**/
//-------------------------------------------------------------------------------------------
LTE_PHY_API_THREAD_CTX *LtePhyApiGetCtx (void)
{
     return &LtePhyApiCtx;
}

//-------------------------------------------------------------------------------------------
/** @brief This is the 500 usec timer
 *
 *  @param none
 *
 *  @return MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
void LtePhyTimerApiIrqCb(LPVOID pParam)
{
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    UINT32 t = GET_TICKS;
    U32 outMsgSize;

    gNumSlotIrq++;
    if (gStartSubfrTimeMark)
        LteBsPhyStatsTimer(pLteBsPhyStats->timerStats.SlotTtiIntervals, gStartSubfrTimeMark, t);

    // Send PHY_INTRA_TTI_IND to MAC if enabled
    if (gSendSecondTTIIndication)
        LteGenerateAPIMsg(0, NULL, &outMsgSize, PHY_INTRA_TTI_IND, gNumSlotIrq);

    ScheduleFftSlot0(FFT_SLOT0_IRQ_DONE);

    MLogMark(2, t);
    MLogTask(PID_APITHREAD_FRACTIONAL_TIMER, RESOURCE_LARM, t, GET_TICKS);

    return;
}


UINT32 LtePhyApiThreadStartTimer(void)
{
    PLTE_PHY_API_THREAD_CTX pThCtx = LtePhyApiGetCtx();
    MXRC rc = MX_OK;

    rc = MxStartFracTimer(pThCtx->hFracTimer);
    if (rc)
    {
        UINT32 tt = GET_TICKS;
        rc = MxRemoveTimer(pThCtx->hFracTimer);
        _ASSERT_RC(rc);
        if(rc) stop_printf("Could not remove Timer: %x\r\n", rc);
        pThCtx->hFracTimer = HNULL;
         TimerDrvRestart();
        // Secondary Timer
        rc = MxCreateTimer(TIMER_MODE_ONE_TIME, 5, NULL, HNULL, &pThCtx->hFracTimer);
        _ASSERT_RC(rc);
        if(rc) stop_printf("Could not create Timer: %x\r\n", rc);
        rc = MxSetTimerProc(pThCtx->hFracTimer, LtePhyTimerApiIrqCb, pThCtx);
        _ASSERT_RC(rc);
        if(rc) stop_printf("Could not set Timer: %x\r\n", rc);
        rc = MxStartFracTimer(pThCtx->hFracTimer);
        return rc;
    }

    return rc;
}

UINT32 LtePhyApiThreadStopTimer(void)
{
    PLTE_PHY_API_THREAD_CTX pThCtx = LtePhyApiGetCtx();
    MXRC rc = MX_OK;

    // Stop Timer
    if (HANDLE_NOT_NULL(pThCtx->hFracTimer))
    {
        rc = MxStopTimer(pThCtx->hFracTimer);
        if (rc != MX_OK)
        {
            stop_printf("Could not stop timer\r\n");
            return rc;
        }
    }

    return rc;
}


//-------------------------------------------------------------------------------------------
/** @brief This function initializes the variables needed for processing the Phy APIs (from MAC)
 *
 *  @param none
 *
 *  @return MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
MXRC LtePhyInitApiThread()
{
    MXRC rc = MX_OK;
    PLTE_PHY_API_THREAD_CTX pThCtx = LtePhyApiGetCtx();
    UINT32 i;

    _ASSERT_PTR(pThCtx);

    if (HANDLE_NOT_NULL(pThCtx->hLtePhyThread))
        return MX_OK;

    memset(pThCtx, 0, sizeof(LTE_PHY_API_THREAD_CTX));

    rc |= MxCreateThreadEx(API_DISPATCH_THREAD_ARM, LtePhyApiThread,
                           NULL,
                           pThCtx,
                           0,
                           &pThCtx->hLtePhyThread);

    if (FAILED(rc))
    {
        MxRemoveThread(pThCtx->hLtePhyThread);
        pThCtx->hLtePhyThread = HNULL;
    }

    rc |= MxCreateThreadEx(API_DISPATCH_THREAD_ARM, LteBsRxPucchSduCleanUp,
                           NULL,
                           pThCtx,
                           0,
                           &pThCtx->hLtePhyCleanUpPucch);

    if (FAILED(rc))
    {
        MxRemoveThread(pThCtx->hLtePhyCleanUpPucch);
        pThCtx->hLtePhyCleanUpPucch = HNULL;
    }


    rc |= MxCreateThreadEx(API_DISPATCH_THREAD_ARM, LteBsRxPuschSduCleanUp,
                           NULL,
                           pThCtx,
                           0,
                           &pThCtx->hLtePhyCleanUpPusch);

    if (FAILED(rc))
    {
        MxRemoveThread(pThCtx->hLtePhyCleanUpPusch);
        pThCtx->hLtePhyCleanUpPusch = HNULL;
    }

    rc |= MxCreateThreadEx(API_DISPATCH_THREAD_ARM, LteBsRxMuxPuschSduCleanUp,
                           NULL,
                           pThCtx,
                           0,
                           &pThCtx->hLtePhyCleanUpMuxPusch);

    if (FAILED(rc))
    {
        MxRemoveThread(pThCtx->hLtePhyCleanUpMuxPusch);
        pThCtx->hLtePhyCleanUpMuxPusch = HNULL;
    }

    for (i = 0; i < NUM_UL_SDU_CONTEXT; i++)
        gRxSduListState[i] = RXSDU_LIST_STATE__CLEAN;

    LtePhyInitMacToPhyMessageStack();

    // Secondary Timer
    rc = MxCreateTimer(TIMER_MODE_ONE_TIME, 5, NULL, HNULL, &pThCtx->hFracTimer);
    _ASSERT_RC(rc);
    rc = MxSetTimerProc(pThCtx->hFracTimer, LtePhyTimerApiIrqCb, pThCtx);
    _ASSERT_RC(rc);

#if API_COPY_THREAD_ARM == 0
    LtePhyInitApiLogThread();
#endif

    pThCtx->apiThreadPush = 0;
    pThCtx->apiThreadPull = 0;

    gDefaultApiLoggingCore          = API_COPY_THREAD_ARM;

    return rc;
}

//-------------------------------------------------------------------------------------------
/** @brief This function destroys the variables needed for processing the Phy APIs (from MAC)
 *
 *  @param none
 *
 *  @return MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
MXRC LtePhyDestroyApiThread()
{
    MXRC rc = MX_OK;
    PLTE_PHY_API_THREAD_CTX pThCtx = LtePhyApiGetCtx();

    _ASSERT_PTR(pThCtx);

    if (HANDLE_NOT_NULL(pThCtx->hLtePhyThread))
    {
        MxRemoveThread(pThCtx->hLtePhyThread);
        pThCtx->hLtePhyThread = HNULL;
    }

    if (HANDLE_NOT_NULL(pThCtx->hLtePhyCleanUpPucch))
    {
        MxRemoveThread(pThCtx->hLtePhyCleanUpPucch);
        pThCtx->hLtePhyCleanUpPucch = HNULL;
    }

    if (HANDLE_NOT_NULL(pThCtx->hLtePhyCleanUpPusch))
    {
        MxRemoveThread(pThCtx->hLtePhyCleanUpPusch);
        pThCtx->hLtePhyCleanUpPusch = HNULL;
    }

    if (HANDLE_NOT_NULL(pThCtx->hLtePhyCleanUpMuxPusch))
    {
        MxRemoveThread(pThCtx->hLtePhyCleanUpMuxPusch);
        pThCtx->hLtePhyCleanUpMuxPusch = HNULL;
    }

    if (HANDLE_NOT_NULL(pThCtx->hLtePhyApiLogThread))
    {
        MxRemoveThread(pThCtx->hLtePhyApiLogThread);
        pThCtx->hLtePhyApiLogThread = HNULL;
    }

    LtePhyDestroyMacToPhyMessageStack();

    // Secondary Timer
    if (HANDLE_NOT_NULL(pThCtx->hFracTimer))
    {
        MxRemoveTimer(pThCtx->hFracTimer);
        pThCtx->hFracTimer = HNULL;
    }

    memset(pThCtx, 0, sizeof(LTE_PHY_API_THREAD_CTX));

    return rc;
}

//-------------------------------------------------------------------------------------------
/** @brief Schedule API Thread to ARM core
 *
 *
 *  @param   none
 *
 *  @return   none
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
void LtePhyScheduleApiThread(PMAC2PHY_QUEUE_EL pMsg, UINT32 isDummy, void* pLteBsApiErrorCheck)
{
    PLTE_PHY_API_THREAD_CTX pThCtx = LtePhyApiGetCtx();
    U32 push;

    _ASSERT((pThCtx != NULL));

    push                       = pThCtx->apiThreadPush;
    pThCtx->apiThreadPush      = IncrementApiThreadFifo(pThCtx->apiThreadPush);

    pThCtx->pMsgQueue[push]           = pMsg;
    pThCtx->isDummyQueue[push]        = isDummy;
    pThCtx->pLteBsApiErrorCheck[push] = pLteBsApiErrorCheck;

    MxScheduleThreadEx(pThCtx->hLtePhyThread, 0, PHY_SMP_THREAD);

    return;
}

UINT32 LteBsApiThreadEnableLogs(void)
{
    UINT32 PhyDiState = PhyDiQueryState();

    //====================================================================
    // Enable Logs
    //====================================================================

    //====================================================================
    // MLOG
    //====================================================================
    if (gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_MLOG)
    {
        MLogEnableRC (1);
        MLogSetMask(0xFFFFFFFF);
    }
    else
    {
        MLogSetMask(0);
    }

    //====================================================================
    // API Logs
    //====================================================================
    if (gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_APILOG)
    {
        SvsrLoggerSetMask(gDefaultApiLoggingMask);
        SvsrLoggerSetTTICycleLimit(gDefaultApiLoggingResetCounter);
    }
    else
    {
        SvsrLoggerSetMask(0);
    }

    //====================================================================
    // IQ Samples
    //====================================================================
    if ((gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_IQLOGS) == 0)
    {
        gDefaultIqEnableMask = 0;
    }

    //====================================================================
    // API Error Check
    //====================================================================
    if (gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_APIERRORCHECK)
    {
        LteBsApiErrorCheckEnable();
    }
    else
    {
        LteBsApiErrorCheckDisable();
    }

    //====================================================================
    // PhyStats
    //====================================================================
    if (gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_PHYSTATS)
    {
        LteBsPhyStatsSetEnableFlag(gDefaultPhyStatsEnableMask);
    }
    else
    {
        LteBsPhyStatsSetEnableFlag(0);
    }

    //====================================================================

    if (PhyDiState != PHY_STATE_START)
    {
        uart_printf("PHY Logs Enabled:\r\n");
        uart_printf("-----------------\r\n");
        uart_printf("Enabled Flags (Mask: 0x%08x):\r\n", gPhyLogsEnableMask);
        uart_printf("LTE_BS_PHY_ENABLE_MLOG:                %s\r\n", ((gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_MLOG)?"YES":"NO"));
        uart_printf("LTE_BS_PHY_ENABLE_APILOG:              %s\r\n", ((gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_APILOG)?"YES":"NO"));
        uart_printf("LTE_BS_PHY_ENABLE_IQLOGS:              %s\r\n", ((gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_IQLOGS)?"YES":"NO"));
        uart_printf("LTE_BS_PHY_ENABLE_PHYSTATS:            %s\r\n", ((gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_PHYSTATS)?"YES":"NO"));
        uart_printf("LTE_BS_PHY_ENABLE_APIERRORCHECK:       %s\r\n", ((gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_APIERRORCHECK)?"YES":"NO"));
        uart_printf("\r\n");
    }

    return 0;
}

UINT32 LtePhyApiThreadSchedLogging(PMAC2PHY_QUEUE_EL pMsg, UINT32 loc, UINT32 isDummy)
{
    UINT32 run = 0;
    if (loc == 0)
    {
        if (pMsg && (gDefaultApiLoggingCore == 0))
            run = 1;
    }
    else if (loc == 1)
    {
        if (pMsg && (gDefaultApiLoggingCore != 0))
            run = 1;
    }
    else
    {
        run = 1;
    }

    if (run)
    {
        LtePhyMessagePush(pMsg, FIFO_PHY_LOG_AND_CLEAN_API,
             (isDummy ? PHY_API_CLEANUP__MAC_TO_PHY_DUMMY_LIST : PHY_API_CLEANUP__MAC_TO_PHY_LIST),
              0);

        LtePhyScheduleApiLogThread();
    }

    return 0;
}


//-------------------------------------------------------------------------------------------
/** @brief This function processes the Phy APIs (from MAC)
 *
 *
 *  @param   pParam  Pointer to LTE PHY API Context
 *
 *  @return   MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 LtePhyApiThread(LPVOID pParam)
{
    PLTE_PHY_API_THREAD_CTX pThCtx = LtePhyApiGetCtx();
    PLTEPHYDESC pPhy, pPhyList;
    PBS_TX pBsTx, pBsTxList;
    PBS_RX pBsRx;
    PMAC2PHY_QUEUE_EL pMsg;
    U32 phyInstance = 0;

    PTXSDUREQ pTxSduReq;
    PDCICHANNELDESC pDlDciChanDesc, pDlDciChanDescBase;

    PDLTXINPUTPARAMS    pTxInputParams;
    PDCIULSDUMSG        pDci;
    PULRXINPUTPARAMS    pUlRxInputParams;
    PULRXFECPARAMS      pFecParams;
    PULRXSDUPARAMS      pUlRxSduParams;
    PIOParams_DlControl pDlCntrl_Pars;
    PHARQS              pharq;
    PLTE_PHY_UL_SDU_DESC pLtePhyUlSduDesc;
    U32 CurrCtx;
    U32 iter[MAXCHSUBFRAME];
    U32 numRb, pdcchType, cqiPmiHarqRiSrPt;
    U32 currTti;
    U8* pPtr;

    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    PLTE_BS_PHY_STATS_USER_STAT pPhyStatsUserStat;
    PLTE_BS_PHY_STATS_SUBFRAME_CTX pLtePhyStatsSubFrameCtx = NULL;
    PLTE_BS_PHY_STATS_UE_CTX pLtePhyStatsUeCtx;
    UINT32 phyStatsEnabledMaskPucch = LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_USRSTS_PUCCH);
    UINT32 phyStatsEnabledMaskPusch = LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_USRSTS_PUSCH);
    UINT32 phyStatsEnabledMaskMuxPusch = LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_USRSTS_MUX_PUSCH);

    PLTE_BS_API_ERROR_CHECK pLteBsApiErrorCheck;

    U32 numBytes, i, j;
    U32 status = 0, chanId = 0;
    U16 cwId, size[2];

    UINT32 isDummyRunning = 0;
    UINT32 mlogVariables[30], mlogVariablesCnt = 0;
    U32 pull;
    U32 tTotal = GET_TICKS;
    U32 t = tTotal;

//	IRQTYPE irqLock = 0;
	
    _my_sfn++;

    _ASSERT((pThCtx != NULL));

    pull                       = pThCtx->apiThreadPull;
    if (pThCtx->apiThreadPull != pThCtx->apiThreadPush)
        pThCtx->apiThreadPull      = IncrementApiThreadFifo(pThCtx->apiThreadPull);

    pMsg                = pThCtx->pMsgQueue[pull];
    isDummyRunning      = pThCtx->isDummyQueue[pull];
    pLteBsApiErrorCheck = (PLTE_BS_API_ERROR_CHECK)pThCtx->pLteBsApiErrorCheck[pull];


    pPhy = getPhyCtxListProc(0);
    MLogMarkAndRegisterFrameSubframe(MLOG_MARK_FRAMEBORDER, gStartSubfrTimeMark, pPhy->txFrameNumber, pPhy->pBsTx->subframeNumber);
    RexNewTTINotification ();

    if (!gFirstMessageArrived)
    {
        // If first message not arrived yet and PHY has been started, then unlock DL TCBs since DL list runs all the time
        if (gLowLatencyFlag)
        {
            t = GET_TICKS;
            pPhy = getPhyCtxApiProc(phyInstance);
            _ASSERT_PTR(pPhy);
            RexResetTCBControl(pPhy->pBsTx->pDlControlTCB[0], REX_LOCKED_TCB);
            RexResetTCBControl(pPhy->pBsTx->pSetupBufsTCB, REX_LOCKED_TCB);
            RexResetTCBControlEx(pPhy->pBsTx->pModTCB, 7, REX_LOCKED_TCB);

            if (UpdateMapDriverDescriptors(pPhy->pBsRx) != 0)
                return 1;

            // Since Swapping is done in FP driver now once things are dispatched. We need to keep swapping the context till API arrives as pPhy is swapping every TTI
            MapFftIfftDrvContextSwap((SWAP_FFT));

            MLogTask(PID_TXLIST_UNLOCK_SETUP_BUFS, RESOURCE_LARM, t, GET_TICKS);
        }
        else
        {
            MapFftIfftDrvContextSwap((SWAP_FFT | SWAP_IFFT));
        }

        // If first message not arrived yet or if some error, then get out
        // This means MAC message did not arrive yet or we are using internal PHY empty message because of some error
        LtePhyApiThreadSchedLogging(pMsg, 2, isDummyRunning);

        MLogTask(PID_APITHREAD_CASE, RESOURCE_LARM, tTotal, GET_TICKS);
        return status;
    }

    LtePhyApiThreadSchedLogging(pMsg, 0, isDummyRunning);

    MLogTask(PID_APITHREAD_CASE, RESOURCE_LARM, t, GET_TICKS);

    // Copy IQ samples from DDR to CRAM for Timer Mode
    pPhy = getPhyCtxApiProc(phyInstance);
    _ASSERT_PTR(pPhy);
    pBsTx = pPhy->pBsTx;
    _ASSERT_PTR(pBsTx);
    pBsRx = pPhy->pBsRx;
    _ASSERT_PTR(pBsRx);
    pPhyList = getPhyCtxListProc(phyInstance);
    _ASSERT_PTR(pPhyList);
    pBsTxList = pPhyList->pBsTx;
    _ASSERT_PTR(pBsTxList);

    pTxInputParams =    pBsTx->pTxInputParams;
    _ASSERT_PTR(pTxInputParams);

    while(LteBsApiErrorCheckDispatchRunning)
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
    }

    pPhy->txFrameNumber             = pLteBsApiErrorCheck->pTxVector->frameNumber;
    pPhy->pBsTx->subframeNumber     = pLteBsApiErrorCheck->pTxVector->subframeNumber;

    pPhy->rxFrameNumber             = pLteBsApiErrorCheck->pRxVector->frameNumber;
    pPhy->pBsRx->subframeNumber     = pLteBsApiErrorCheck->pRxVector->subframeNumber;

    // Unlock Control TCB
    if (pTxInputParams->numCtrlSymbols == 0)
    {
        RexSetTCBControl(pBsTx->pDlControlTCB[0], BYPASS_TCB);
    }

    if (gLowLatencyFlag)
    {
        if (pBsTx->isListRunning)
            RexResetTCBControl(pBsTx->pDlControlTCB[0], REX_LOCKED_TCB);
    }
    else
    {
        if (pBsTxList->isListRunning)
            RexResetTCBControl(pBsTxList->pDlControlTCB[0], REX_LOCKED_TCB);
    }

    PhyDiCopyIq(pPhy, UL);

    t = GET_TICKS;
//	irqLock = ARM_INT_disable();
    while(pPhy->pBsRx->isExeTaskListReady == TRUE)
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
		if( (GET_TICKS - t) > 150*600 )
		{	
//			ARM_INT_restore(irqLock);	
	        stop_printf("RxList execution time exceeded limit; TTI start %d start waiting %d current time %d \r\n", TTI_start, t, GET_TICKS);
		}
    }
//	ARM_INT_restore(irqLock);

    rxVectorProcDone                        = 0;
    pUlRxInputParams                        = pPhy->pBsRx->pUlRxInputParams;
    pUlRxInputParams->pMdmaDescr            = NULL;
    pUlRxInputParams->isDummyApiRunning     = isDummyRunning;

    t = GET_TICKS;
    status = LteBsPhyRxVectorProcessing(0,  pLteBsApiErrorCheck->pRxVector,  pLteBsApiErrorCheck->lenRxVector);

    if (status != 0)
        stop_printf("RXVECTOR FAILURE: %d\r\n", status);
    MLogTask(PID_RXVECTORPROC, RESOURCE_LARM, t, GET_TICKS);

#ifdef IDFT_CELL_DEPENDENCY
    {
        PLTEPHYDESC pPhyList;
        pPhyList = getPhyCtxListProc(phyInstance);
        _ASSERT_PTR(pPhyList);

        if (pPhyList->pBsRx->pUlRxInputParams->numPUSCH == 0)
        {
            for (i = 0; i < NUM_IDFT_CELL_DEPENDENCY; i++)
                *pPhyList->pBsRx->pIdftCell[i] = ~RX_DEP_IDFT_INIT_VAL;
        }
    }
#endif

    if (UpdateMapDriverDescriptors(pBsRx) != 0)
        return 1;

    t = GET_TICKS;
    status = LteBsRxSduPreProcessingRT(pBsRx->rxSduCtx, pPhy);
    MLogTask(PID_RXLIST_PREPROC, RESOURCE_LARM, t, GET_TICKS);


    //---------------------------------------------------------------------------------------
    // Generate all Phy Stats from MAC to PHY APIs

    t = GET_TICKS;

    pUlRxInputParams =  pBsRx->pUlRxInputParams;
    pFecParams =        pBsRx->pUlRxFecParams[1-pBsRx->RxFecParam_Context];  // 1- because it has been swapped by PreProc
    pDlCntrl_Pars =     pBsTx->pDlCntrl_TxInPars;
    pUlRxSduParams =    pBsRx->pUlRxSduParams;
    pharq =             pPhy->pharq;
    CurrCtx =           pBsRx->rxSduCtx;

    // Time Histogram from TxVector and RxVector
    if (LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_TIME_HISTOGRAM))
    {
        // Down Link
        pLtePhyStatsSubFrameCtx = LteBsPhyStatsTimeHistogramIncrement(DL,
                              (pTxInputParams->numChan - pTxInputParams->numCtrlChan),
                               pTxInputParams->numCtrlChan + pLteBsApiErrorCheck->numDciSdu,
                              (pTxInputParams->txFrameNumber*10 + pTxInputParams->subframeNumber));

        // Time Histogram for PDCCH
        for (i = 0; i < pTxInputParams->numCtrlChan; i++)
        {
            pLtePhyStatsUeCtx = LteBsPhyStatsTimerHistogramGetDlUeCtx(pLtePhyStatsSubFrameCtx);
            LteBsPhyStatsTimeHistogramUeCtxRNTI(pLtePhyStatsUeCtx, pDlCntrl_Pars->pdcchRNTI[i]);
            LteBsPhyStatsTimeHistogramUeCtxType(pLtePhyStatsUeCtx, PDCCH);
            LteBsPhyStatsTimeHistogramUeCtxNumRb(pLtePhyStatsUeCtx, (U8)pDlCntrl_Pars->NumCCECtl[i]);
            LteBsPhyStatsTimeHistogramDLUeCtxIncrement(pLtePhyStatsSubFrameCtx);
        }

        // Time Histogram for PDCCH DCI0
        for(i=0; i<pLteBsApiErrorCheck->numDciSdu; i++)
        {
            pDci = pLteBsApiErrorCheck->pDciSdu[i];
            pLtePhyStatsUeCtx = LteBsPhyStatsTimerHistogramGetDlUeCtx(pLtePhyStatsSubFrameCtx);
            LteBsPhyStatsTimeHistogramUeCtxRNTI(pLtePhyStatsUeCtx, pDci->rnti);
            LteBsPhyStatsTimeHistogramUeCtxType(pLtePhyStatsUeCtx, (U8)PDCCH_DCI0);
            LteBsPhyStatsTimeHistogramUeCtxNumRb(pLtePhyStatsUeCtx, (U8)pDci->numCCE);
            LteBsPhyStatsTimeHistogramDLUeCtxIncrement(pLtePhyStatsSubFrameCtx);
        }


        // Time Histogram for PDSCH / PBCH
        for (i = pTxInputParams->numCtrlChan; i < (U32) pTxInputParams->numChan; i++)
        {
            numRb = 0;
            if (pTxInputParams->channelType[i] == PDSCH)
            {
                for(j = 0; j < pTxInputParams->numberofEntries[i]; j++)
                    numRb += pTxInputParams->reselmInfo[i][j].numRes;
            }

            pLtePhyStatsUeCtx = LteBsPhyStatsTimerHistogramGetDlUeCtx(pLtePhyStatsSubFrameCtx);
            LteBsPhyStatsTimeHistogramUeCtxRNTI(pLtePhyStatsUeCtx, pTxInputParams->nRNTI[i]);
            LteBsPhyStatsTimeHistogramUeCtxType(pLtePhyStatsUeCtx, pTxInputParams->channelType[i]);
            LteBsPhyStatsTimeHistogramUeCtxNumRb(pLtePhyStatsUeCtx, numRb);
            LteBsPhyStatsTimeHistogramUeCtxNumCw(pLtePhyStatsUeCtx, pTxInputParams->numCodeWords[i]);
            LteBsPhyStatsTimeHistogramUeCtxRv0(pLtePhyStatsUeCtx, pTxInputParams->rV0[i]);
            LteBsPhyStatsTimeHistogramUeCtxMcs0(pLtePhyStatsUeCtx, pTxInputParams->mcsType0[i]);
            LteBsPhyStatsTimeHistogramUeCtxQm0(pLtePhyStatsUeCtx, pTxInputParams->Qm0[i]);
            if (pTxInputParams->numCodeWords[i] > 1)
            {
                LteBsPhyStatsTimeHistogramUeCtxRv1(pLtePhyStatsUeCtx, pTxInputParams->rV1[i]);
                LteBsPhyStatsTimeHistogramUeCtxMcs1(pLtePhyStatsUeCtx, pTxInputParams->mcsType1[i]);
                LteBsPhyStatsTimeHistogramUeCtxQm1(pLtePhyStatsUeCtx, pTxInputParams->Qm1[i]);
            }
            LteBsPhyStatsTimeHistogramDLUeCtxIncrement(pLtePhyStatsSubFrameCtx);
         }



        // Up Link
        pLtePhyStatsSubFrameCtx = LteBsPhyStatsTimeHistogramIncrement(UL, pUlRxInputParams->numPUSCH,
                                                    pUlRxInputParams->numPUCCH,
                                                    (pPhy->rxFrameNumber * 10 + pBsRx->subframeNumber));

        for (i = 0; i < pUlRxInputParams->numChan; i++)
        {
            if (pUlRxInputParams->physChanType[i] == PUSCH)
            {
                iter[i] = pharq->harq_iter[pharq->harq_prc][pFecParams->harq_user[i]];

                pLtePhyStatsUeCtx = LteBsPhyStatsTimerHistogramGetUlUeCtx(pLtePhyStatsSubFrameCtx);
                LteBsPhyStatsTimeHistogramUeCtxRNTI(pLtePhyStatsUeCtx, pFecParams->nRNTI[i]);
                LteBsPhyStatsTimeHistogramUeCtxType(pLtePhyStatsUeCtx, (PUSCH<<4));
                LteBsPhyStatsTimeHistogramUeCtxNumRb(pLtePhyStatsUeCtx, pUlRxInputParams->NumRBs[i]);
                LteBsPhyStatsTimeHistogramUeCtxRv0(pLtePhyStatsUeCtx, iter[i]);
                LteBsPhyStatsTimeHistogramUeCtxMcs0(pLtePhyStatsUeCtx, CalcMcs(pFecParams->Itbs[i], pFecParams->Qm[i]));
                LteBsPhyStatsTimeHistogramUeCtxQm0(pLtePhyStatsUeCtx, pFecParams->Qm[i]);
                cqiPmiHarqRiSrPt = ((pFecParams->nr1CQI[i] + pFecParams->nrg1CQI[i]) ? PHYSTATS_CQI_PMI_PT : 0);
                cqiPmiHarqRiSrPt |= (pFecParams->nRI[i] ? PHYSTATS_RI_PT : 0);
                cqiPmiHarqRiSrPt |= (pFecParams->nACK[i] ? PHYSTATS_HARQ_PT : 0);
                LteBsPhyStatsTimeHistogramUeCtxCqiPmiHarqRiSrPt(pLtePhyStatsUeCtx, cqiPmiHarqRiSrPt);
                LteBsPhyStatsTimeHistogramULUeCtxIncrement(pLtePhyStatsSubFrameCtx);
            }
            else
            {
                pLtePhyStatsUeCtx = LteBsPhyStatsTimerHistogramGetUlUeCtx(pLtePhyStatsSubFrameCtx);
                LteBsPhyStatsTimeHistogramUeCtxRNTI(pLtePhyStatsUeCtx, pFecParams->nRNTI[i]);
                LteBsPhyStatsTimeHistogramUeCtxType(pLtePhyStatsUeCtx, (PUCCH<<4 | pFecParams->pucchFormat[i]));
                LteBsPhyStatsTimeHistogramUeCtxNumRb(pLtePhyStatsUeCtx, pUlRxInputParams->n1[i]);
                LteBsPhyStatsTimeHistogramUeCtxRv0(pLtePhyStatsUeCtx, pUlRxInputParams->srPUCCHResourceIndex[i]);
                LteBsPhyStatsTimeHistogramUeCtxMcs0(pLtePhyStatsUeCtx, pUlRxInputParams->n2[i]);
                cqiPmiHarqRiSrPt = (pUlRxInputParams->simSRHarq[i] ? PHYSTATS_SR_PT : 0);
                LteBsPhyStatsTimeHistogramUeCtxCqiPmiHarqRiSrPt(pLtePhyStatsUeCtx, cqiPmiHarqRiSrPt);
                LteBsPhyStatsTimeHistogramULUeCtxIncrement(pLtePhyStatsSubFrameCtx);
            }

            pLtePhyUlSduDesc = (PLTE_PHY_UL_SDU_DESC) pRxSduDesc[CurrCtx][i];
            pLtePhyUlSduDesc->pPhyStatsUlUeCtx = pLtePhyStatsUeCtx;
        }

    }

    // PDCCH SDU
    if (LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_USRSTS_PDCCH))
    {
        pDlDciChanDescBase = (PDCICHANNELDESC) ((U32) pLteBsApiErrorCheck->pTxVector +
                                                (U32) pLteBsApiErrorCheck->pTxVector->offsetDCIChannels);
        for(i = 0; i < pLteBsApiErrorCheck->numPdcchSdu; i++)
        {
            pTxSduReq = pLteBsApiErrorCheck->pPdcchSdu[i];
            chanId    = pTxSduReq->chanId;

            if (pTxInputParams->txChanIdMappingTable[chanId] != -1)
            {
                numBytes = pTxSduReq->msgLen;

                pdcchType = 10;             //Some Dummy Location
                if (numBytes == 4)
                {
                    if(gUseTxSduPointer)
                        pPtr = (U8*)pTxSduReq->pTxSdu;
                    else
                        pPtr = (U8*)&(pTxSduReq->pTxSdu);
                    if (*pPtr & 0x80)
                        pdcchType = 1;
                    else
                        pdcchType = 0;
                }
                else
                {
                    pdcchType = 2;
                }

                pDlDciChanDesc = &pDlDciChanDescBase[pTxInputParams->txChanIdMappingTable[chanId]];

                pPhyStatsUserStat = LteBsPhyStatsGetRntiId(pDlDciChanDesc->scrmblerInitValue);
                LteBsPhyStatsUserStatNumPdcchSdu(pPhyStatsUserStat);
                LteBsPhyStatsUserStatNumPdcchTypeSdu(pPhyStatsUserStat, pdcchType);
            }
        }
    }

    // PDSCH / PBCH
    if(LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_USRSTS_PDSCH))
    {
        size[0] = 0;
        size[1] = 0;
        for(i=0; i<pLteBsApiErrorCheck->numPdschSdu; i++)
        {
            pTxSduReq = pLteBsApiErrorCheck->pPdschSdu[i];
            chanId = pTxSduReq->chanId;

            if (pTxInputParams->txChanIdMappingTable[chanId] != -1)
            {
                numBytes = pTxSduReq->msgLen;
                cwId = pTxSduReq->cwId;
                size[cwId] += numBytes;

                chanId = pTxInputParams->txChanIdMappingTable[chanId];

                pPhyStatsUserStat = LteBsPhyStatsGetRntiId(pTxInputParams->nRNTI[chanId]);
                LteBsPhyStatsThroughput(pPhyStatsUserStat, DL, numBytes, cwId);
                if (cwId)
                {
                    LteBsPhyStatsUserStatDlHarqTransmissions(pPhyStatsUserStat, cwId, pTxInputParams->rV1[chanId]);
                    LteBsPhyStatsMcs(pPhyStatsUserStat, pTxInputParams->mcsType1[chanId], cwId);
                }
                else
                {
                    LteBsPhyStatsUserStatDlHarqTransmissions(pPhyStatsUserStat, cwId, pTxInputParams->rV0[chanId]);
                    LteBsPhyStatsMcs(pPhyStatsUserStat, pTxInputParams->mcsType0[chanId], cwId);
                }
                LteBsPhyStatsUserStatNumPdschSdu(pPhyStatsUserStat, cwId);
            }
        }

        if (size[0])
            LteBsPhyStatsThroughput((PLTE_BS_PHY_STATS_USER_STAT )pLteBsPhyStats->timerStats.systemThroughput[0], SYSTEM, size[0], 0);

        if (size[1])
            LteBsPhyStatsThroughput((PLTE_BS_PHY_STATS_USER_STAT )pLteBsPhyStats->timerStats.systemThroughput[1], SYSTEM, size[1], 1);

    }

    // Up Link
    if (phyStatsEnabledMaskPucch | phyStatsEnabledMaskPusch | phyStatsEnabledMaskMuxPusch)
    {
        currTti = (pPhy->rxFrameNumber * 10) + pBsRx->subframeNumber;
        size[0] = 0;
        for (i = 0; i < pUlRxInputParams->numChan; i++)
        {
            size[0] += pFecParams->numBytes[i];

            pPhyStatsUserStat = LteBsPhyStatsGetRntiId(pFecParams->nRNTI[i]);
            if (pUlRxInputParams->physChanType[i] == PUSCH)
            {
                if (phyStatsEnabledMaskPusch)
                {
                    LteBsPhyStatsUserStatNumPuschSdu(pPhyStatsUserStat);
                    LteBsPhyStatsThroughput(pPhyStatsUserStat, UL, pFecParams->numBytes[i], 0);
                    LteBsPhyStatsMcs(pPhyStatsUserStat, CalcMcs(pFecParams->Itbs[i], pFecParams->Qm[i]), 2);
                    LteBsPhyStatsUserStatUlHarqTransmissions(pPhyStatsUserStat, iter[i]);
                }
                if (phyStatsEnabledMaskMuxPusch)
                {
                    if (pFecParams->numbitsACK[i])
                        LteBsPhyStatsUserStatNumMuxPuschAckNack(pPhyStatsUserStat);
                    if (pFecParams->numbitsCQI[i])
                    {
                        LteBsPhyStatsUserStatNumMuxPuschCqi(pPhyStatsUserStat);
                        LteBsPhyStatsUlUciPeriodicity(pPhyStatsUserStat, PHY_STATS_DL_CQI_PERIODICITY, currTti);
                    }
                    if (pFecParams->numbitsRI[i])
                    {
                        LteBsPhyStatsUserStatNumMuxPuschRi(pPhyStatsUserStat);
                        LteBsPhyStatsUlUciPeriodicity(pPhyStatsUserStat, PHY_STATS_DL_RI_PERIODICITY, currTti);
                    }
                }
            }
            else
            {
                if (phyStatsEnabledMaskPucch)
                {
                    LteBsPhyStatsUserStatNumPucch(pPhyStatsUserStat, pFecParams->pucchFormat[i]);
                    LteBsPhyStatsUserStatSimultaneousSr(pPhyStatsUserStat, pFecParams->pucchFormat[i], pUlRxInputParams->simSRHarq[i]);
                    if (pFecParams->pucchFormat[i] == FORMAT1)
                        LteBsPhyStatsUlUciPeriodicity(pPhyStatsUserStat, PHY_STATS_SR_PERIODICITY, currTti);
                    else if (pFecParams->pucchFormat[i] < FORMAT2)
                    {
                        if (pUlRxInputParams->simSRHarq[i])
                            LteBsPhyStatsUlUciPeriodicity(pPhyStatsUserStat, PHY_STATS_SR_PERIODICITY, currTti);
                    }
                    else if (pFecParams->pucchFormat[i] > FORMAT1B)
                    {
                        if (LteBsIsCqiPmi(pUlRxSduParams->dlCqiPmiSizeBits[i]))
                        {
                            LteBsPhyStatsUlUciPeriodicity(pPhyStatsUserStat, PHY_STATS_DL_CQI_PERIODICITY, currTti);
                            LteBsPhyStatsUserStatDlNumDlCqi(pPhyStatsUserStat, (pFecParams->pucchFormat[i] - FORMAT2));
                        }
                        else
                        {
                            LteBsPhyStatsUlUciPeriodicity(pPhyStatsUserStat, PHY_STATS_DL_RI_PERIODICITY, currTti);
                            LteBsPhyStatsUserStatDlNumDlRi(pPhyStatsUserStat, (pFecParams->pucchFormat[i] - FORMAT2));
                        }
                    }
                }
            }
            pLtePhyUlSduDesc = (PLTE_PHY_UL_SDU_DESC) pRxSduDesc[CurrCtx][i];
            pLtePhyUlSduDesc->pUserStat = pPhyStatsUserStat;
        }

        if (size[0])
            LteBsPhyStatsThroughput((PLTE_BS_PHY_STATS_USER_STAT )pLteBsPhyStats->timerStats.systemThroughput[2], SYSTEM, size[0], 0);

    }

    if (gLastTxSfn == 1)
        LteBsPhyStatsUpdateRexStats();

    mlogVariables[mlogVariablesCnt++] = 0xABABABAB;
    mlogVariables[mlogVariablesCnt++] = LteBsConstructSfn (pPhy->txFrameNumber, pPhy->pBsTx->subframeNumber);
    mlogVariables[mlogVariablesCnt++] = LteBsConstructSfn (pPhy->rxFrameNumber, pPhy->pBsRx->subframeNumber);
    mlogVariables[mlogVariablesCnt++] = pTxInputParams->numCtrlChan;
    mlogVariables[mlogVariablesCnt++] = (pTxInputParams->numChan - pTxInputParams->numCtrlChan);
    mlogVariables[mlogVariablesCnt++] = pLteBsApiErrorCheck->numDciSdu;
    mlogVariables[mlogVariablesCnt++] = pLteBsApiErrorCheck->numHARQVector;
    mlogVariables[mlogVariablesCnt++] = pLteBsApiErrorCheck->numPhichSdu;
    mlogVariables[mlogVariablesCnt++] = pUlRxInputParams->numPUCCH;
    mlogVariables[mlogVariablesCnt++] = pUlRxInputParams->numPUSCH;

    if (mlogVariablesCnt)
        MLogAddVariables (mlogVariablesCnt, mlogVariables, t);

    MLogTask(PID_APITHREAD_PHYSTATS_CTX0+CurrCtx, RESOURCE_LARM, t, GET_TICKS);

    //---------------------------------------------------------------------------------------

    LtePhyApiThreadSchedLogging(pMsg, 1, isDummyRunning);

    MLogTask(PID_APITHREAD_COMPLETE, RESOURCE_LARM, tTotal, GET_TICKS);

    return status;
}



//-------------------------------------------------------------------------------------------
/** @brief This function initializes the FIFO Stack used for I-Core communication between MAC and PHY
 *
 *  @param none
 *
 *  @return none
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
void LtePhyInitMacToPhyMessageStack(void)
{
    memset(&MacToPhyApiStack, 0, sizeof(FIFO_STACK));
    memset(&PhyApiLogAndCleanUp, 0, sizeof(FIFO_STACK));
}

//-------------------------------------------------------------------------------------------
/** @brief This function destroys the FIFO Stack used for I-Core communication between MAC and PHY
 *
 *  @param none
 *
 *  @return none
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
void LtePhyDestroyMacToPhyMessageStack(void)
{
    memset(&MacToPhyApiStack, 0, sizeof(FIFO_STACK));
    memset(&PhyApiLogAndCleanUp, 0, sizeof(FIFO_STACK));
}


//-------------------------------------------------------------------------------------------
/** @brief This function is used by MAC to populate the FIFO Stack with Api List Header
 *
 *  @param ApiHeader    Pointer to the Linked List Header
 *
 *  @return MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
MXRC LtePhyMessagePush(PMAC2PHY_QUEUE_EL ListHeader, UINT32 fifoBufferType, UINT32 fifoCleanType, UINT32 fifoMsgLen)
{
    U32 push, retVal;
    IRQTYPE irqLock = 0;
    U32 tt, diffTime, loc;
    PFIFO_STACK pFifoStack;
    PDLSUBFRDESC pTxVector;
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    UINT32 cleanApi = 0, numApi, numTxSdu;

    if (fifoBufferType == FIFO_MAC_TO_PHY_API)
        pFifoStack = &MacToPhyApiStack;
    else if (fifoBufferType == FIFO_PHY_LOG_AND_CLEAN_API)
        pFifoStack = &PhyApiLogAndCleanUp;
    else
        return FIFO_INVALID_ERR;


    if (fifoBufferType == FIFO_MAC_TO_PHY_API)
    {
        pTxVector   = NULL;
        numApi      = 0;
        numTxSdu    = 0;
        if (LteBsApiErrorCheckScanApi(ListHeader, &numApi, &numTxSdu, &pTxVector) != 0)            // Means that there are still some APIs left in the queue after scnaning
        {
            // Disable IRQ
            irqLock = ARM_INT_disable();

            retVal = LteBsApiErrorCheckPushApiState(numApi, numTxSdu, pTxVector);

            tt = GET_TICKS;
            if (retVal != LTE_BS_API_ERROR_CHECK_SUCCESS)
            {
                cleanApi = 1;
                LtePhyApiCleanUpThread(ListHeader, TIMEOUT_TXSTART_REQ, TIMEOUT_RXSTART_REQ);
            }

            if (cleanApi == 0)
            {
                diffTime = LteBsPhyStatsTimer(pLteBsPhyStats->timerStats.ValidApiDeliveryIntervals, gStartSubfrTimeMark, tt);
                loc = diffTime / 100;     // diffTime is in usecs
                LteBsPhyStatsApiArrivalHistorgram(pLteBsPhyStats, loc);

                if (!gLowLatencyFlag)
                {
                    // Check for error Conditions
                    push = pFifoStack->ListPushLoc + 1;
                    if (push >= FIFO_DEPTH)
                        push = 0;

                    if (push == pFifoStack->ListPopLoc)
                    {
                        ARM_INT_restore(irqLock);
                        stop_printf("Queue Overflow\r\n");
                        return FIFO_FULL_ERR;
                    }

                    // Put to Queue
                    pFifoStack->ListHeader[pFifoStack->ListPushLoc] = ListHeader;
                    pFifoStack->FifoCleanType[pFifoStack->ListPushLoc] = fifoCleanType;
                    pFifoStack->FifoCleanMsgLen[pFifoStack->ListPushLoc] = fifoMsgLen;
                    pFifoStack->ListPushLoc = push;

                    // This is needed for ARM Sync
                    MxDMB();

                }
                else
                {
                    if (fifoBufferType == FIFO_MAC_TO_PHY_API)
                        LteBsApiErrorCheckDispatch(ListHeader, 0, 1);
                }
            }

            // Enable IRQ
            ARM_INT_restore(irqLock);

            MLogTask(PID_APITHREAD_PUSH_BASE+fifoBufferType, RESOURCE_LARM, tt, GET_TICKS);
        }
    }
    else
    {
        tt = GET_TICKS;

        // Disable IRQ
        irqLock = ARM_INT_disable();


        // Check for error Conditions
        push = pFifoStack->ListPushLoc + 1;
        if (push >= FIFO_DEPTH)
            push = 0;

        if (push == pFifoStack->ListPopLoc)
        {
            ARM_INT_restore(irqLock);
            stop_printf("Queue Overflow\r\n");
            return FIFO_FULL_ERR;
        }

        // Put to Queue
        pFifoStack->ListHeader[pFifoStack->ListPushLoc] = ListHeader;
        pFifoStack->FifoCleanType[pFifoStack->ListPushLoc] = fifoCleanType;
        pFifoStack->FifoCleanMsgLen[pFifoStack->ListPushLoc] = fifoMsgLen;
        pFifoStack->ListPushLoc = push;

        // This is needed for ARM Sync
        MxDMB();

        // Enable IRQ
        ARM_INT_restore(irqLock);

        MLogTask(PID_APITHREAD_PUSH_BASE+fifoBufferType, RESOURCE_LARM, tt, GET_TICKS);
    }

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief This function is used by PHY to read the FIFO Stack to parse the APIs
 *
 *  @param none
 *
 *  @return ApiHeader    Pointer to the Linked List Header
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
LPVOID LtePhyMessagePop(UINT32 fifoBufferType, PUINT32 pFifoCleanType, PUINT32 pFifoMsgLen)
{
    LPVOID ListHeader;
    IRQTYPE irqLock;
    U32 tt = GET_TICKS, pop;
    PFIFO_STACK pFifoStack;

    if (fifoBufferType == FIFO_MAC_TO_PHY_API)
        pFifoStack = &MacToPhyApiStack;
    else if (fifoBufferType == FIFO_PHY_LOG_AND_CLEAN_API)
        pFifoStack = &PhyApiLogAndCleanUp;
    else
        return NULL;

    // Disable IRQ
    irqLock = ARM_INT_disable();

    // This is needed for ARM Sync
    MxDMB();

    pop = pFifoStack->ListPopLoc;

    // Check for error Conditions
    if (pFifoStack->ListPushLoc == pFifoStack->ListPopLoc)
    {
        MLogTask(PID_APITHREAD_POP_NO_FIFO_BASE+fifoBufferType, RESOURCE_LARM, tt, GET_TICKS);

        ARM_INT_restore(irqLock);
        return NULL;
    }

    // Pop from Queue
    pFifoStack->ListPopLoc++;
    if (pFifoStack->ListPopLoc >= FIFO_DEPTH)
        pFifoStack->ListPopLoc = 0;
    ListHeader = pFifoStack->ListHeader[pop];
    *pFifoCleanType = pFifoStack->FifoCleanType[pop];
    *pFifoMsgLen = pFifoStack->FifoCleanMsgLen[pop];

    // Enable IRQ
    ARM_INT_restore(irqLock);

    MLogTask(PID_APITHREAD_POP_BASE+fifoBufferType, RESOURCE_LARM, tt, GET_TICKS);

    return ListHeader;
}


//-------------------------------------------------------------------------------------------
/** @brief This function is used by PHY to read the FIFO Stack silently
 *
 *  @param none
 *
 *  @return ApiHeader    Pointer to the Linked List Header
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
LPVOID LtePhyMessagePopSilent(UINT32 fifoBufferType, PUINT32 pFifoCleanType, PUINT32 pFifoMsgLen)
{
    LPVOID ListHeader = NULL;
    IRQTYPE irqLock;
    U32 tt = GET_TICKS, pop;
    PFIFO_STACK pFifoStack;

    if (fifoBufferType == FIFO_MAC_TO_PHY_API)
        pFifoStack = &MacToPhyApiStack;
    else if (fifoBufferType == FIFO_PHY_LOG_AND_CLEAN_API)
        pFifoStack = &PhyApiLogAndCleanUp;
    else
        return NULL;

    // Disable IRQ
    irqLock = ARM_INT_disable();

    // This is needed for ARM Sync
    MxDMB();

    pop = pFifoStack->ListPopLoc;

    // Check for error Conditions
    if (pFifoStack->ListPushLoc != pop)
    {
        // Pop from Queue
        pFifoStack->ListPopLoc++;
        if (pFifoStack->ListPopLoc >= FIFO_DEPTH)
            pFifoStack->ListPopLoc = 0;
        ListHeader = pFifoStack->ListHeader[pop];
        *pFifoCleanType = pFifoStack->FifoCleanType[pop];
        *pFifoMsgLen = pFifoStack->FifoCleanMsgLen[pop];
    }

    // Enable IRQ
    ARM_INT_restore(irqLock);

    MLogTask(PID_APITHREAD_POP_SILENT_BASE+fifoBufferType, RESOURCE_LARM, tt, GET_TICKS);

    return ListHeader;
}

//-------------------------------------------------------------------------------------------
/** @brief This function frees the API messages that Arrives from the MAC pretty late
 *
 *
 *  @return   MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 LtePhyStopApiFree(void)
{
    PLTE_PHY_API_THREAD_CTX pThCtx = LtePhyApiGetCtx();

    LtePhyScheduleApiLogThread();

    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief This function frees the API messages that Arrives from the MAC pretty late
 *
 *
 *  @return   MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 LtePhyStartApiFree(void)
{
    PLTE_PHY_API_THREAD_CTX pThCtx = LtePhyApiGetCtx();
    LPVOID pMsg;
    PMAC2PHY_QUEUE_EL curr, tmp;
    PGENMSGDESC pMsgHeader;
    UINT32 fifoCleanType, fifoMsgLen;

    pMsg = LtePhyMessagePopSilent(FIFO_MAC_TO_PHY_API, &fifoCleanType, &fifoMsgLen);

    while(pMsg)
    {// have list
        curr = (PMAC2PHY_QUEUE_EL)pMsg;
        while (curr)
        {//have message
            tmp = curr;

            curr = curr->Next;
            tmp->Next = NULL;

            pMsgHeader = (PGENMSGDESC) tmp->MessagePtr;
            if (pMsgHeader->msgType == PHY_TXSDU_REQ)
            {
                PTXSDUREQ pTxSduReq = (PTXSDUREQ) pMsgHeader;
                if(gUseTxSduPointer)
                    SvsrFreeIMem(pTxSduReq->pTxSdu);
            }
            SvsrFreeIMem(tmp);
            SvsrFreeIMem(pMsgHeader);
        }

        pMsg = LtePhyMessagePopSilent(FIFO_MAC_TO_PHY_API, &fifoCleanType, &fifoMsgLen);
    }

    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief This function cleans up the API message that Arrives from the MAC pretty late
 *
 *  @param   pParam  Pointer to LTE PHY API Context
 *
 *  @return   MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 LtePhyApiCleanUpThread(PMAC2PHY_QUEUE_EL pMsg, UINT32 statusTx, UINT32 statusRx)
{
    PMAC2PHY_QUEUE_EL curr;
    PGENMSGDESC pMsgHeader;
    PLTEPHYDESC pPhy = getPhyCtxApiProc(0);
    U8* listRxEnd = NULL;
    UINT32 t = GET_TICKS;
    PMAC2PHY_QUEUE_EL listElement;
    U32 numPUSCHChan = 0;
    U32 pucchListPresent = 0;
    IRQTYPE irqLock;

    U32 rxSduCtx = gRxSduCtx;
    gRxSduCtx = IncrementSduCtx(gRxSduCtx);

    curr = pMsg;

    // Parse messages and free
    while (curr)
    {
        pMsgHeader = (PGENMSGDESC) curr->MessagePtr;

        switch (pMsgHeader->msgType)
        {
            case PHY_TXSTART_REQ:
            {
                PDLSUBFRDESC pTxVector;
                PMSGIND pMsgInd;

                pTxVector = (PDLSUBFRDESC) (pMsgHeader + 1);

                // PHY_ERROR_IND for Delayed / Discarded API
                pMsgInd = (PMSGIND) Phy2MacIfAllocBuffer (sizeof(MSGIND) + 32);
                pMsgInd->phyEntityId = (U8) 0;
                pMsgInd->msgType     = (U8) PHY_ERROR_IND;
                pMsgInd->status      = (U8) statusTx;
                pMsgInd->subFrameNum = pTxVector->subframeNumber;
                pMsgInd->frameNumber = pTxVector->frameNumber;

                // Allocate List Elements head
                listElement = (PMAC2PHY_QUEUE_EL) Phy2MacIfAllocBuffer(sizeof(MAC2PHY_QUEUE_EL));
                listElement->Next           = NULL;
                listElement->frameNumber    = pTxVector->frameNumber;
                listElement->subframeNumber = pTxVector->subframeNumber;
                listElement->MessageType    = PHY_ERROR_IND;
                listElement->MessagePtr     = (U8*)pMsgInd;
                listElement->MessageLen     = sizeof(MSGIND) + 4;

                LteDelayedApiIndAdd2List(PUSCH_LIST_TYPE, rxSduCtx, listElement);
            }
            break;

            // Need to look only at RxVector as we need to send out dummy RxSdus
            case PHY_RXSTART_REQ:
            {
                LTE_PHY_UL_SDU_DESC rxSduDesc;
                PRXSDUIND           pRxSduMsg;
                PRXSTATUSIND        pRxStatusInd;
                PRX_CQIRIHI_STATUS_EVENT pRxStatusEvent;
                PULSUBFRDESC        pRxVector;
                PULCHANDESC         pPuschChan;
                PULCTRLCHDESC       pPucchChan;
                PULSUBCHINFO        pSubChInfo;
                PPUSCHDED           pUlChDedInfo;
                ULRXFECPARAMS       FecPars;
                U32 i, numChan = 0, muxPuschPresent = 0;
                U32 allocSize = sizeof(RXSDUIND) + 32;  // Add 32 bytes for safety. Not needed
                U32 allocSize1 = sizeof(RXSTATUSIND) + sizeof(RX_CQIRIHI_STATUS_EVENT) + 32;  // Add 32 bytes for safety. Not needed

                // Parse RxVector
                pRxVector = (PULSUBFRDESC) (pMsgHeader + 1);
                numChan = pRxVector->numberofChannelDescriptors;
                numPUSCHChan = (U32) (numChan - pRxVector->numberOfCtrlChannelDescriptors);

                // Build a dummy RxSdu Desc for the LteRxSduAdd2List and LteRxStatusCqiRiHiAdd2List functions
                rxSduDesc.rxFrameNumber     = pRxVector->frameNumber;
                rxSduDesc.subframeNumber    = pRxVector->subframeNumber;
                rxSduDesc.numBitsRx         = 0;
                rxSduDesc.srdetected        = 0;
                rxSduDesc.TimingAdvance     = 0;
                rxSduDesc.mErrAvg[0]        = 0;
                rxSduDesc.mErrAvg[1]        = 0;
                rxSduDesc.mErrExpo          = 0;
                rxSduDesc.ul_CQI            = 0;
                rxSduDesc.cqiPmiConf        = 0;
                rxSduDesc.chanIndex         = 0;
                FecPars.nr1CQI[0]           = 0;
                FecPars.nrg1CQI[0]          = 0;
                FecPars.nRI[0]              = 0;
                FecPars.nACK[0]             = 0;

                rxSduDesc.pUlRxFecParams = &FecPars;

                // Process PUSCH
                for (i = 0; i < numPUSCHChan; i++)
                {
                    pPuschChan = &pRxVector->ulCh[i];
                    FecPars.channelType[i] = PUSCH;
                    if (pPuschChan->channelType == PUSCH)
                    {
                        pRxSduMsg = (PRXSDUIND) Phy2MacIfAllocNcNbBuffer (allocSize);     // Allocate SDU for PUSCH
                        pRxSduMsg->phyEntityId      = 0;
                        pRxSduMsg->msgType          = PHY_RXSDU_IND;
                        pRxSduMsg->numBitsRx        = 0;
                        pRxSduMsg->channelId        = i;
                        pRxSduMsg->frameNum         = pRxVector->frameNumber;
                        pRxSduMsg->subFrameNum      = pRxVector->subframeNumber;
                        pRxSduMsg->chanType         = PUSCH;
                        pRxSduMsg->listType         = UL_LIST_TYPE_PUSCH;
                        pRxSduMsg->srdetected       = 0;
                        pRxSduMsg->pucchDetected    = 0;

                        // Allocate List Elements head
                        listElement = (PMAC2PHY_QUEUE_EL) Phy2MacIfAllocBuffer(sizeof(MAC2PHY_QUEUE_EL));
                        listElement->Next           = NULL;
                        listElement->frameNumber    = pRxVector->frameNumber;
                        listElement->subframeNumber = pRxVector->subframeNumber;
                        listElement->MessageType    = PHY_RXSDU_IND;
                        listElement->MessagePtr     = (U8*)pRxSduMsg;
                        listElement->MessageLen     = sizeof(RXSDUIND);

                        LteRxSduAdd2List(&rxSduDesc, statusRx, PUSCH_LIST_TYPE, rxSduCtx, listElement);
                    }

                    // Check for Multiplex ACK / NACK
                    pSubChInfo = &pPuschChan->ulSubChInfo;
                    pUlChDedInfo = &pSubChInfo->puschDed;
                    muxPuschPresent = pUlChDedInfo->nACK + pUlChDedInfo->nRI + pUlChDedInfo->nr1CQI + pUlChDedInfo->nrg1CQI;
                    if (muxPuschPresent)
                    {
                        pRxStatusInd = (PRXSTATUSIND) Phy2MacIfAllocBuffer (allocSize1);     // Allocate SDU for PUSCH
                        pRxStatusInd->phyEntityId   = (U8) 0;
                        pRxStatusInd->msgType       = PHY_RXSTATUS_IND;
                        pRxStatusInd->frameNum      = pRxVector->frameNumber;
                        pRxStatusInd->subFrameNum   = pRxVector->subframeNumber;
                        pRxStatusInd->statusType    = CQIRIHI_RESULT;
                        pRxStatusInd->status        = statusRx;
                        pRxStatusInd->eventNumber   = 0;
                        pRxStatusInd->listType      = UL_LIST_TYPE_PUCCH;

                        // Now fill the status info based on the channel info
                        pRxStatusEvent              = (PRX_CQIRIHI_STATUS_EVENT)&pRxStatusInd->pEventList;
                        pRxStatusEvent->status_len  = 0;
                        pRxStatusEvent->chanId      = i;
                        pRxStatusEvent->cqi_pmi_pt  = 0;
                        pRxStatusEvent->ri_pt       = 0;
                        pRxStatusEvent->harq_pt     = 0;

                        // Allocate List Elements head
                        listElement = (PMAC2PHY_QUEUE_EL) Phy2MacIfAllocBuffer(sizeof(MAC2PHY_QUEUE_EL));
                        listElement->Next           = NULL;
                        listElement->frameNumber    = pRxVector->frameNumber;
                        listElement->subframeNumber = pRxVector->subframeNumber;
                        listElement->MessageType    = PHY_RXSTATUS_IND;
                        listElement->MessagePtr     = (U8*)pRxStatusInd;
                        listElement->MessageLen     = sizeof(RX_CQIRIHI_STATUS_EVENT) + sizeof(RXSTATUSIND);

                        LteRxStatusCqiRiHiAdd2List(&rxSduDesc, MUX_PUSCH_CQI_RI, PUCCH_LIST_TYPE, rxSduCtx, listElement);
                        pucchListPresent = 1;
                    }
                }

                // Process PUCCH
                pPucchChan = (PULCTRLCHDESC) ((U32) pRxVector + pRxVector->offsetULCtrlChannels);
                for (i = numPUSCHChan; i < numChan; i++)
                {
                    FecPars.channelType[i] = PUCCH;
                    if (pPucchChan->channelType == PUCCH)
                    {
                        pRxSduMsg = (PRXSDUIND) Phy2MacIfAllocBuffer (allocSize);     // Allocate SDU for PUCCH
                        pRxSduMsg->phyEntityId      = 0;
                        pRxSduMsg->msgType          = PHY_RXSDU_IND;
                        pRxSduMsg->numBitsRx        = 0;
                        pRxSduMsg->channelId        = i;
                        pRxSduMsg->frameNum         = pRxVector->frameNumber;
                        pRxSduMsg->subFrameNum      = pRxVector->subframeNumber;
                        pRxSduMsg->chanType         = PUCCH;
                        pRxSduMsg->listType         = UL_LIST_TYPE_PUCCH;
                        pRxSduMsg->pucchType        = pPucchChan->pucchDedCtl.formatType;
                        pRxSduMsg->srdetected       = 0;
                        pRxSduMsg->pucchDetected    = 0;

                        // Allocate List Elements head
                        listElement = (PMAC2PHY_QUEUE_EL) Phy2MacIfAllocBuffer(sizeof(MAC2PHY_QUEUE_EL));
                        listElement->Next = NULL;
                        listElement->frameNumber    = pRxVector->frameNumber;
                        listElement->subframeNumber = pRxVector->subframeNumber;
                        listElement->MessageType    = PHY_RXSDU_IND;
                        listElement->MessagePtr     = (U8*)pRxSduMsg;
                        listElement->MessageLen     = sizeof(RXSDUIND);

                        LteRxSduAdd2List(&rxSduDesc, statusRx, PUCCH_LIST_TYPE, rxSduCtx, listElement);
                        pucchListPresent = 1;
                    }
                    pPucchChan = (pPucchChan + 1);
                }

                listRxEnd = LteRxSduBuildRxEndMessage(pRxVector->frameNumber, pRxVector->subframeNumber);
            }
            break;

            default:
            break;
        }
        curr = curr->Next;
    }

    // Disable IRQ
    irqLock = ARM_INT_disable();

    if (pucchListPresent)
    {
        gRxSduListState[rxSduCtx] |= RXSDU_LIST_STATE__PUCCH_PRESENT;
        SendRxSduList(PUCCH_LIST_TYPE, 0, rxSduCtx, listRxEnd);       // If PUSCH Exists then this is not last list
    }

    gRxSduListState[rxSduCtx] |= RXSDU_LIST_STATE__PUSCH_PRESENT;
    SendRxSduList(PUSCH_LIST_TYPE, 1, rxSduCtx, listRxEnd);           // Send it always since we have TxStartInd for Delayed / Bad API

    // Enable IRQ
    ARM_INT_restore(irqLock);

    // If Api is delayed then it is caught early before Api Thread. If it is invalid then it is already sent for cleaning.
    if (statusTx != INVALID_TX_VECTOR)
    {
        LtePhyMessagePush(pMsg, FIFO_PHY_LOG_AND_CLEAN_API, PHY_API_CLEANUP__PHY_TO_MAC_CLEANUP_THREAD, 0);
        LtePhyScheduleApiLogThread();
    }

    MLogTask(PID_APITHREAD_CLEANUP_CTX0+rxSduCtx, RESOURCE_LARM, t, GET_TICKS);

    return 0;
}


//-------------------------------------------------------------------------------------------
/** @brief Initialize API Copy Thread to ARM core
 *
 *
 *  @param   none
 *
 *  @return   none
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
MXRC LtePhyInitApiLogThread(void)
{
    MXRC rc = MX_OK;
    PLTE_PHY_API_THREAD_CTX pThCtx = LtePhyApiGetCtx();

    _ASSERT_PTR(pThCtx);

    rc |= MxCreateThreadEx(API_COPY_THREAD_ARM, LtePhyApiLogThread,
                           NULL,
                           pThCtx,
                           0,
                           &pThCtx->hLtePhyApiLogThread);

    if (FAILED(rc))
    {
        MxRemoveThread(pThCtx->hLtePhyApiLogThread);
        pThCtx->hLtePhyApiLogThread = HNULL;
    }

    pThCtx->hLtePhyApiLogThreadNew = HNULL;

    return rc;
}

MXRC LtePhyInitApiLogThreadSecond(void)
{
    MXRC rc = MX_OK;
    PLTE_PHY_API_THREAD_CTX pThCtx = LtePhyApiGetCtx();

    rc = MxCreateThreadEx(API_COPY_THREAD_ARM, LtePhyApiLogThread,
                           NULL,
                           pThCtx,
                           0,
                           &pThCtx->hLtePhyApiLogThreadNew);

    if (FAILED(rc))
    {
        MxRemoveThread(pThCtx->hLtePhyApiLogThreadNew);
        pThCtx->hLtePhyApiLogThreadNew = HNULL;
    }

    pThCtx->hLtePhyApiLogThreadNew = HNULL;

    return rc;
}

//-------------------------------------------------------------------------------------------
/** @brief Destroy API Copy Thread to ARM core
 *
 *
 *  @param   none
 *
 *  @return   none
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
void LtePhyDestroyApiLogThread(void)
{
    PLTE_PHY_API_THREAD_CTX pThCtx = LtePhyApiGetCtx();

    _ASSERT_PTR(pThCtx);

    if (HANDLE_NOT_NULL(pThCtx->hLtePhyApiLogThread))
    {
        MxRemoveThread(pThCtx->hLtePhyApiLogThread);
        pThCtx->hLtePhyApiLogThread = HNULL;
    }

    if (HANDLE_NOT_NULL(pThCtx->hLtePhyApiLogThreadNew))
    {
        MxRemoveThread(pThCtx->hLtePhyApiLogThreadNew);
        pThCtx->hLtePhyApiLogThreadNew = HNULL;
    }

    return;
}


//-------------------------------------------------------------------------------------------
/** @brief Schedule API Copy Thread to ARM core
 *
 *
 *  @param   none
 *
 *  @return   none
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
void LtePhyScheduleApiLogThread(void)
{
    PLTE_PHY_API_THREAD_CTX pThCtx = LtePhyApiGetCtx();
    _ASSERT((pThCtx != NULL));

    if (HANDLE_NOT_NULL(pThCtx->hLtePhyApiLogThreadNew))
        MxScheduleThreadEx(pThCtx->hLtePhyApiLogThreadNew, 0, PHY_SMP_THREAD);
    else
        MxScheduleThreadEx(pThCtx->hLtePhyApiLogThread, 0, PHY_SMP_THREAD);

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief This function processes the Copy of APIs for debugging
 *
 *  @param   pParam  Pointer to LTE PHY API Context
 *
 *  @return   MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 LtePhyApiLogThread(LPVOID pParam)
{
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    PMAC2PHY_QUEUE_EL curr, tmp, free;
    PGENMSGDESC pMsgHeader;
    PHIADCIULMSGDESC pHiDciMsg;
    PTXSDUREQ pTxSduReq;
    PDCIULSDUMSG pDciUlSduMsg;
    UINT32 len, numBytes = 0;
    U32 t = GET_TICKS;
    UINT32 fifoCleanType, fifoMsgLen, txSfn, rxSfn, notFifoTypeLateMessage;
    UINT32 numApiFree, numIcpuBlockFree, numTxSdu;

    curr = LtePhyMessagePopSilent(FIFO_PHY_LOG_AND_CLEAN_API, &fifoCleanType, &fifoMsgLen);
    notFifoTypeLateMessage = !(fifoCleanType & PHY_API_CLEANUP__PHY_TO_MAC_CLEANUP_THREAD);

    while(curr)
    {
        if (fifoCleanType & (PHY_API_CLEANUP__MAC_TO_PHY_LIST | PHY_API_CLEANUP__MAC_TO_PHY_DUMMY_LIST | PHY_API_CLEANUP__PHY_TO_MAC_CLEANUP_THREAD))
        {
            free = curr;

            // Log the API
            txSfn = LteBsGetSubFrameNum(gLastTxSfn);
            rxSfn = LteBsGetSubFrameNum(gLastRxSfn);

            while (curr)
            {
                pMsgHeader = (PGENMSGDESC) curr->MessagePtr;

                switch (pMsgHeader->msgType)
                {
                    case PHY_TXSTART_REQ:
                        SvsrLoggerAddMsg (PHY_TXSTART_REQ, LOGGER_MSG_INFO(0, txSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                    break;

                    case PHY_RXSTART_REQ:
                        SvsrLoggerAddMsg (PHY_RXSTART_REQ, LOGGER_MSG_INFO(0, rxSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                    break;

                    case PHY_TXHIADCIUL_REQ:
                        if (notFifoTypeLateMessage)
                        {
                            pHiDciMsg = (PHIADCIULMSGDESC)pMsgHeader;
                            len = pHiDciMsg->msgLength;
                            SvsrLoggerAddMsg (PHY_TXHIADCIUL_REQ, LOGGER_MSG_INFO(0, pHiDciMsg->subFrameNumber, IID_LTE_MAC, IID_LTE), pMsgHeader, (len + sizeof (HIADCIULMSGDESC)));
                        }
                    break;

                    case PHY_TXHISDU_REQ:
                        if (notFifoTypeLateMessage)
                            SvsrLoggerAddMsg (PHY_TXHISDU_REQ, LOGGER_MSG_INFO(0, txSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, sizeof (HIINFOMSGDESC));
                    break;

                    case PHY_TXSDU_REQ:
                        if (notFifoTypeLateMessage)
                        {
                            pTxSduReq = (PTXSDUREQ) pMsgHeader;
                            numBytes = pTxSduReq->msgLen;
                            SvsrLoggerAddMsg (PHY_TXSDU_REQ, LOGGER_MSG_INFO(0, txSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, (10 + sizeof (TXSDUREQ)));
                        }
                    break;

                    case PHY_TXDCIULSDU_REQ:
                        if (notFifoTypeLateMessage)
                        {
                            pDciUlSduMsg = (PDCIULSDUMSG) pMsgHeader;
                            numBytes = pDciUlSduMsg->msgLength;
                            SvsrLoggerAddMsg (PHY_TXDCIULSDU_REQ, LOGGER_MSG_INFO(0, txSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, (numBytes + sizeof (DCIULSDUMSG)));
                        }
                    break;

                    case PHY_INIT_REQ:
                        if (notFifoTypeLateMessage)
                            SvsrLoggerAddInit (PHY_INIT_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof (GENMSGDESC)));
                    break;

                    case PHY_START_REQ:
                        if (notFifoTypeLateMessage)
                        {
                            SvsrLoggerAddMsgForce (PHY_START_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, sizeof(STARTREQ));
                            SvsrLoggerSetResetMarker();
                        }
                        break;

                    case PHY_STOP_REQ:
                        if (notFifoTypeLateMessage)
                            SvsrLoggerAddMsg (PHY_STOP_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                    break;

                    case PHY_PAUSE_REQ:
                        if (notFifoTypeLateMessage)
                            SvsrLoggerAddMsg (PHY_PAUSE_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                    break;

                    case PHY_RESUME_REQ:
                        if (notFifoTypeLateMessage)
                            SvsrLoggerAddMsg (PHY_RESUME_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                    break;

                    case PHY_SHUTDOWN_REQ:
                        if (notFifoTypeLateMessage)
                            SvsrLoggerAddMsg (PHY_SHUTDOWN_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                    break;

                    case PHY_RECONFIG_REQ:
                        if (notFifoTypeLateMessage)
                            SvsrLoggerAddMsgForce (PHY_RECONFIG_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                    break;

                    case PHY_LOGS_ENABLE_REQ:
                        if (notFifoTypeLateMessage)
                            SvsrLoggerAddMsg (PHY_LOGS_ENABLE_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                    break;

                    default:
                    break;
                }

                curr = curr->Next;
            }

            // Clean up the previous TTIs APIs
            if (fifoCleanType & (PHY_API_CLEANUP__MAC_TO_PHY_LIST | PHY_API_CLEANUP__PHY_TO_MAC_CLEANUP_THREAD))
            {
                numApiFree = 0;
                numTxSdu = 0;
                while (free)
                {
                    tmp = free;

                    free = free->Next;
                    tmp->Next = NULL;

                    pMsgHeader = (PGENMSGDESC) tmp->MessagePtr;
                    if (pMsgHeader->msgType == PHY_TXSDU_REQ)
                    {
                        pTxSduReq = (PTXSDUREQ) pMsgHeader;
                        if(gUseTxSduPointer)
                        {
                            numTxSdu++;
                            SvsrFreeIMem(pTxSduReq->pTxSdu);
                        }
                    }
                    SvsrFreeIMem(tmp);
                    SvsrFreeIMem(pMsgHeader);

                    numApiFree++;
                }

                if (LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_APISTATS))
                {
                    numIcpuBlockFree = (numApiFree << 1);     // Multiply by 2 for the pListHeader
                    numIcpuBlockFree += 1;                    // Add 1 for the general header as part of each subframe
                    if (gUseTxSduPointer)
                        numIcpuBlockFree += numTxSdu;         // Because each of these would be another Icpu Block

                    LteBsPhyStatsNumApisFreed(pLteBsPhyStats, numApiFree);
                    LteBsPhyStatsNumMac2PhyBlocksFreed(pLteBsPhyStats, numIcpuBlockFree);

                }

            }
        }

        if (fifoCleanType & (PHY_API_CLEANUP__PHY_TO_MAC_SRS_MSG | PHY_API_CLEANUP__PHY_TO_MAC_RACH_MSG))
        {
            SvsrLoggerAddMsg(PHY_RXSTATUS_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC),
                              curr, fifoMsgLen);
        }

        if (fifoCleanType & PHY_API_CLEANUP__PHY_TO_MAC_OTHER_MSG)
        {
            PMSGIND pOtherMsg = (PMSGIND)curr;
            SvsrLoggerAddMsg(pOtherMsg->msgType, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC),
                              curr, fifoMsgLen);
        }


        if (fifoCleanType & PHY_API_CLEANUP__PHY_TO_MAC_RX_SDU_LIST)
        {
            while (curr)
            {
                pMsgHeader = (PGENMSGDESC) curr->MessagePtr;
                numBytes = curr->MessageLen;

                switch (pMsgHeader->msgType)
                {
                    case PHY_RXSDU_IND:
                        SvsrLoggerAddMsg(PHY_RXSDU_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), pMsgHeader, 10 + sizeof(RXSDUIND));
                    break;

                    case PHY_RXSTATUS_IND:
                        SvsrLoggerAddMsg (PHY_RXSTATUS_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), pMsgHeader, numBytes);
                    break;

                    case PHY_RXEND_IND:
                        SvsrLoggerAddMsg(PHY_RXEND_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), pMsgHeader, sizeof(MSGIND));
                    break;

                    default:
                    break;
                }

                curr = curr->Next;
            }
        }

        curr = LtePhyMessagePopSilent(FIFO_PHY_LOG_AND_CLEAN_API, &fifoCleanType, &fifoMsgLen);

    }

    MLogTask(PID_APITHREAD_LOG_AND_FREE, RESOURCE_LARM, t, GET_TICKS);

    return 0;
}


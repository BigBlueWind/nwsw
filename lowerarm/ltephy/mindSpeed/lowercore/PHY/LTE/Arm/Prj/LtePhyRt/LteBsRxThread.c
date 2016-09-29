//-------------------------------------------------------------------------------------------
/** @file LteBsRxThread.c
 *
 * @brief Implementation of UL De-Modulator thread for the LTE BS
 * @author Mindspeed Technologies
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------


#include "prototypes.h"
#include "lte_bs_phy_nmm_fdd.h"
#include "ext_phy.h"
#include "Debug.h"

#include "ltertconfig.h"
#include "LteBsRxThread.h"
#include "LteBsTxThread.h"
#include "LteBsApiThread.h"
#include "LteBsApiErrorCheck.h"
#include "LteBsRxPrachThread.h"
#include "LteBsRxSrsThread.h"
#include "LteBsRxSduPreproc.h"
#include "LteBsPhyStats.h"
#include "lte_bs_phy_nmm_thread.h"

#include "appprintf.h"
#include "fpdrv.h"
#include "mlog.h"
#include "svsrlogger.h"
#include "globalstoragearrays.h"

#ifdef PM_ENABLED
#include "pm.h"
#endif

extern U32          nmm_subframe_count;

static LTE_RX_PROCESS_CTX RxProcessCtx[2];
static LTE_BS_RX_THREAD ltebsRxThread;

static U32 RxProcessCtxNum = 0;
static U32 gTtiTick;

static PLTE_RX_PROCESS_CTX rxListProcessCtx[PROCESS_CTX_BUF_LEN];
static U32 rxListProcessCtxPushCnt = 0;
static U32 rxListProcessCtxPullCnt = 0;

extern volatile UINT32 TTI_start;

PLTE_BS_RX_THREAD LteBsRxThreadGetCtx(void)
{
    return &ltebsRxThread;
}


UINT32 LteBsRxThreadLowLatencyPush(PMAC2PHY_QUEUE_EL pMsg, UINT32 isDummy, void* pLteBsApiErrorCheck)
{
    PLTE_BS_RX_THREAD pLteBsRxThreadCtx = LteBsRxThreadGetCtx();
    UINT32 push = pLteBsRxThreadCtx->rxThreadPush;

    pLteBsRxThreadCtx->rxThreadPush                 = IncrementProcessCtx(pLteBsRxThreadCtx->rxThreadPush);
    pLteBsRxThreadCtx->pMsgQueue[push]              = pMsg;
    pLteBsRxThreadCtx->pLteBsApiErrorCheck[push]    = pLteBsApiErrorCheck;
    pLteBsRxThreadCtx->isDummyQueue[push]           = isDummy;

    return 0;
}


//-------------------------------------------------------------------------------------------
/** @brief Schedule RX Thread to ARM core from thread context
 *
 *  @param   none
 *
 *  @return   none
 *
 *  \ingroup group_lte_phy_control_rt_rx_listrun
 *
**/
//-------------------------------------------------------------------------------------------
MXRC LtePhySchedulePhyThread(LPVOID pParam)
{
    MXRC rc = MX_OK;
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    PLTEPHYDESC pPhy = getPhyCtxListProc(0);
    PMAC2PHY_QUEUE_EL pMsg;
    PLISTENINGDESC pListening;
    PLTE_BS_RX_THREAD pLteBsRxThreadCtx = LteBsRxThreadGetCtx();
    UINT32 pull;

    U32 outMsgSize;
    UINT32 isDummyRunning = 0;
    UINT32 t = GET_TICKS;

    gStartSubfrTimeMark = t;
    gNumSlotIrq         = 0;

	TTI_start = t;
	
    // to process pause parameters
    // if this function returns non-zero value
    // we need to stop data processing
    if (PhyDiPauseProc())
    {
        if(gLowLatencyFlag == 0)
        {
            // Pause has kicked in. We need to bypass the DL Control so that IFFT can run and complete. Special case.
            pPhy = getPhyCtxApiProc(0);
            RexResetTCBControl(pPhy->pBsTx->pDlControlTCB[0], REX_BYPASS_TCB | REX_LOCKED_TCB);
        }
        return MX_OK;
    }

    if (gFirstMessageArrived)
    {
        LteBsPhyStatsTimer(pLteBsPhyStats->timerStats.SubframeTtiIntervals, gTtiTick, t);
        gLastTxSfn = LteBsIncrementSfn(gLastTxSfn);
        gLastRxSfn = LteBsIncrementSfn(gLastRxSfn);
    }
    else    // Make sure FFT is not going to run in this subframe. It can run only from TTI after API has arrived
    {
        gLastTxSfn = 0;
        gLastRxSfn = 0;
    }

    gRunFftSlot0 = 0;
    gTtiTick = t;

    if (gMode == NMM_MODE)
    {
        LteBsPhyStatsNumNmmSubframe(pLteBsPhyStats->timerStats.numNmmSubframes);

        MLogMarkAndRegisterFrameSubframe(MLOG_MARK_FRAMEBORDER, t, pPhy->rxFrameNumber, pPhy->pBsRx->subframeNumber);

        RexNewTTINotification ();

        pListening = LteBsGetNmmCtx();

        if (pListening)
        {
            nmm_subframe_count ++;
            if(nmm_subframe_count == MAX_NMM_SUBFRAME_COUNT)
                nmm_subframe_count = 0;

            if(pListening->received_nmm_stop_flag==1)
            {
                lte_bs_phy_nmm_stop(0, NULL, 0);
                return MX_OK;
            }

            if (pListening->state >= NMM_RSSI_MEASURING) {
                swapPhyCtx(0);
                pPhy = getPhyCtxListProc(0);
                lte_bs_phy_nmm_schedule_thread();
            }
        }
    }
    else if (gMode == LTE_MODE)
    {
        // Send TXSTART IND to MAC
        LteGenerateAPIMsg(0, NULL, &outMsgSize, PHY_TXSTART_IND, 0);

        // Swap PHY Context
        swapPhyCtx(0);

        pPhy = getPhyCtxListProc(0);
        LteBsPhyStatsNumSubframe(pLteBsPhyStats->timerStats.numSubframes);

        // Schedule Rx Thread and Run FFTs
        if (pPhy->pBsRx->isExeTaskListReady == TRUE)
        {
#ifdef IDFT_CELL_DEPENDENCY
            RexReinitCells((UINT32*)pPhy->pBsRx->pIdftCell, NUM_IDFT_CELL_DEPENDENCY);
#endif

            LtePhyThreadRx(pPhy);

#ifndef SLOT_BASED_FFT
            MapFftDrvRun(pPhy->pBsRx->pFftTcb[0]);
#endif
            MapFftDrvRun(pPhy->pBsRx->pFftTcb[1]);
        }
        else
        {
            LteBsPhyStatsULMissed(pLteBsPhyStats->timerStats.numUlListMissed);
            MLogTask(PID_RXLIST_MISSED_RUN, RESOURCE_LARM, t, GET_TICKS);
        }

        // Schedule Tx Thread
        if (pPhy->pBsTx->isExeTaskListReady == TRUE)
        {
            if (gLowLatencyFlag)
                LtePhyThreadTx(getPhyCtxApiProc(0));
            else
                LtePhyThreadTx(getPhyCtxListProc(0));
        }
        else
        {
            LteBsPhyStatsDLMissed(pLteBsPhyStats->timerStats.numDlListMissed);
            MLogTask(PID_TXLIST_MISSED_RUN, RESOURCE_LARM, t, GET_TICKS);
        }

        // Schedule PRACH Thread
        if(pPhy->pBsRx->isExePrachTaskListReady[pPhy->pBsRx->RxPrach_Context] == TRUE)
        {
            LtePhyThreadPrach(pPhy);
        }
        else
        {
            LteBsPhyStatsPrachMissed(pLteBsPhyStats->timerStats.numPrachListMissed);
            MLogTask(PID_PRACHLIST_MISSED_RUN, RESOURCE_LARM, t, GET_TICKS);
        }

        // Schedule SRS Thread
        if (pPhy->pBsRx->isExeSrsTaskListReady == TRUE)
        {
            LtePhyThreadSrs(pPhy);
        }
        else
        {
            LteBsPhyStatsSrsMissed(pLteBsPhyStats->timerStats.numSrsListMissed);
            MLogTask(PID_SRSLIST_MISSED_RUN, RESOURCE_LARM, t, GET_TICKS);
        }

        // Schedule API Thread
        if (!gLowLatencyFlag)
        {
            // Get New API
            pMsg = LteBsPhyStatsGetApi(&isDummyRunning);

            if (pMsg)
            {
                LteBsApiErrorCheckDispatch(pMsg, isDummyRunning, 0);
            }
            else
            {
                gCtxDlFec = 1 - gCtxDlFec;
                LtePhyScheduleApiThread(NULL, 0, NULL);
            }
        }
        else
        {
            pull = pLteBsRxThreadCtx->rxThreadPull;

            if (pLteBsRxThreadCtx->rxThreadPull != pLteBsRxThreadCtx->rxThreadPush)
                pLteBsRxThreadCtx->rxThreadPull = IncrementProcessCtx(pLteBsRxThreadCtx->rxThreadPull);

            if (pLteBsRxThreadCtx->pLteBsApiErrorCheck[pull])
            {
                LtePhyScheduleApiThread(pLteBsRxThreadCtx->pMsgQueue[pull], pLteBsRxThreadCtx->isDummyQueue[pull], pLteBsRxThreadCtx->pLteBsApiErrorCheck[pull]);
                pLteBsRxThreadCtx->pLteBsApiErrorCheck[pull] = NULL;
            }
            else
            {
                // Get New API
                pMsg = LteBsPhyStatsGetApi(&isDummyRunning);

                if (pMsg)
                {
                    LteBsApiErrorCheckDispatch(pMsg, isDummyRunning, 0);
                }
                else
                {
                    gCtxDlFec = 1 - gCtxDlFec;
                    LtePhyScheduleApiThread(NULL, 0, NULL);
                }
            }
        }

        // Stats
#ifdef PM_ENABLED
        PMGenSyncSignal (0);
#endif
    }

    MLogTask(PID_SCHEDULE_PHY_THREAD, RESOURCE_LARM, t, GET_TICKS);

    return rc;
}

//-------------------------------------------------------------------------------------------
/** @brief This is the call back function to the RX Demodulator process.
 *  Once RX Demodulator list is completed, this function is called by REX
 *
 *  @param   thisList Pointer to RX Demodulator TaskList
 *  @param   param Pointer to RX Demodulator Thread Context
 *
 *  @return none
 *
 *  \ingroup group_lte_phy_control_rt_rx_listrun
 *
**/
//-------------------------------------------------------------------------------------------
void LteRxListDoneCb(TASKLIST* thisList, void* param)
{
    PLTE_RX_PROCESS_CTX pRxProcessCtx;
    PLTEPHYDESC pPhy;
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    UINT32 t = GET_TICKS, ID;

    pRxProcessCtx = rxListProcessCtx[rxListProcessCtxPullCnt];
    rxListProcessCtxPullCnt = IncrementProcessCtx(rxListProcessCtxPullCnt);

    _ASSERT_PTR(pRxProcessCtx);
    _ASSERT(pRxProcessCtx->listStateRx == EXEC_LIST_PROCESSING);

    pPhy = pRxProcessCtx->pPhy;
    _ASSERT_PTR(pPhy);

    ID = pPhy->pBsRx->rxListId;
    MLogTCBListStop(ID);
    MLogTask(PID_TASKLIST_EXEC_00 + ID, RESOURCE_LARM, pRxProcessCtx->t, t);

    pPhy->pBsRx->isExeTaskListReady = FALSE;

    LteBsPhyStatsTimer(pLteBsPhyStats->timerStats.ULListProcessingTime, pRxProcessCtx->t, t);

    // Put RX SDUs to the decoding
    if (pRxProcessCtx->numChan)
        LteBsRxSduProcessingRT(pRxProcessCtx->rxSduCtx, pRxProcessCtx->t);

    ReinitTaskList(pPhy->pBsRx->pExeTaskList[0], NULL);
    pPhy->pBsRx->isExeTaskListReady = FALSE;

    pRxProcessCtx->listStateRx = EXEC_LIST_IDLE;

    MLogTask(PID_RXLIST_COMPLETE_LIST, RESOURCE_LARM, t, GET_TICKS);

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief This function disptaches the Rx Demodulator taskList to REX
 *
 *  @param   pParam  Pointer to RX Demodulator Thread Context
 *
 *  @return   MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_rx_listrun
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 LtePhyThreadRx(PLTEPHYDESC pPhy)
{
    LTE_RX_PROCESS_CTX* pRxProcessCtx;
    PBS_RX pBsRx = pPhy->pBsRx;
    U32 *pContext, t = GET_TICKS;

    RxProcessCtxNum = 1 - RxProcessCtxNum;
    pRxProcessCtx   = &RxProcessCtx[RxProcessCtxNum];

    if (pRxProcessCtx->listStateRx != EXEC_LIST_IDLE)
    {
        stop_printf("Previous Rx List Not Completed\r\n");
        return 0;
    }

    rxListProcessCtx[rxListProcessCtxPushCnt] = pRxProcessCtx;
    rxListProcessCtxPushCnt = IncrementProcessCtx(rxListProcessCtxPushCnt);

    pRxProcessCtx->t           = gStartSubfrTimeMark;
    pRxProcessCtx->listStateRx = EXEC_LIST_PROCESSING;
    pRxProcessCtx->pPhy        = pPhy;
    pRxProcessCtx->numChan          = pBsRx->pUlRxInputParams->numChan;
    pRxProcessCtx->rxSduCtx         = pBsRx->rxSduCtx;

    // Update SNR TCBs Context
    pContext    = pBsRx->pSnrSubframeTask->ContextPtr;
    pContext[0] = pRxProcessCtx->t;
    pContext[1] = pBsRx->rxSduCtx;

    pContext    = pBsRx->pRipRssiSubframeTask->ContextPtr;
    pContext[3] = pBsRx->rxSduCtx;

    // Run List
    MLogTCBListStart(pBsRx->rxListId);
    ExeTSchedList(pBsRx->pExeTaskList[0]);

    MLogTask(PID_RXLIST_RUN_LIST, RESOURCE_LARM, t, GET_TICKS);

    return 0;
}

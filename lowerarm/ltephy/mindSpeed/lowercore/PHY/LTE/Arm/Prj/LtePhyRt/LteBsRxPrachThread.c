//-------------------------------------------------------------------------------------------
/** @file LteBsRxPrachThread.c
 *
 * @brief Implementation of PRACH processing thread for the LTE BS
 * @author Mindspeed Technologies
 * @version $Revision: 1.43 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#include "ltephy.h"
#include "ltephydef.h"
#include "Debug.h"
#include "prototypes.h"
#include "LteBsRxPrachThread.h"
#include "LteBsPhyStats.h"
#include "ltertconfig.h"
#include "bs_rx_prach.h"
#include "mlog.h"

static LTE_PHY_PRACH_PROCESS_CTX prachListProcessCtx[PRACH_PROCESS_CTX_LEN];
static U32 prachListProcessCtxCnt = 0;

U32 firstPrach = 0;

//-------------------------------------------------------------------------------------------
/** @brief This is the call back function to the PRACH process. Once PRACH list is completed, this function is called by REX
 *
 *  @param   thisList Pointer to PRACH TaskList
 *  @param   param Pointer to RX PRACH Thread Context
 *
 *  @return none
 *
 *  \ingroup group_lte_phy_control_rt_rx_prach
 *
**/
//-------------------------------------------------------------------------------------------
void LtePhyPrachListDoneCb(TASKLIST* pThisList, void* param)
{
    PLTE_PHY_PRACH_PROCESS_CTX pPrachCtx = NULL;
    PBS_RX_PRACHDet pBsRxPrachDet;
    PMS_DET pDetEvent;

    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    PLTE_BS_PHY_STATS_UE_CTX pUlCurrCtx;
    UINT32 phyStatsEnabledMaskTimeHist = LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_TIME_HISTOGRAM);

    PLTEPHYDESC pPhy;
    PBS_RX pBsRx;
    U32 prachProcIndex, procInd;
    U32 outMsgSize;

    UINT32 t1 = GET_TICKS;
    UINT32 tTotal = t1;

    procInd                                 = *((U32*)param);

    pPrachCtx                               = &prachListProcessCtx[procInd];
    pPhy                                    = pPrachCtx->pPhy;
    pBsRx                                   = pPrachCtx->pPhy->pBsRx;
    prachProcIndex                          = pPrachCtx->prachProcInd;

    _ASSERT(pPrachCtx->listState == EXEC_LIST_PROCESSING);

    MLogTCBListStop(pPrachCtx->listId);
    MLogTask(PID_TASKLIST_EXEC_00 + pPrachCtx->listId, RESOURCE_LARM, pPrachCtx->t, t1);
    LteBsPhyStatsTimer(pLteBsPhyStats->timerStats.PrachListProcessingTime, pPrachCtx->t, t1);


    // Send the SDU
    pBsRxPrachDet                           = pPhy->pBs->pBsRxPrachDet[prachProcIndex];
    _ASSERT_PTR (pBsRxPrachDet);

    pDetEvent                               = &(pBsRxPrachDet->bs_prachDet_msDet);
    LteBsPhyStatsNumPrachDet(pLteBsPhyStats->rachStats.numPrachDetected, pDetEvent->num);

    if (pDetEvent->num && phyStatsEnabledMaskTimeHist)
    {
        LteBsPhyStatsTimeHistogramSfnCtxIncrementUlChs(pPrachCtx->pLtePhyStatsSubFrameCtx, 1);
        pUlCurrCtx = LteBsPhyStatsTimerHistogramGetUlUeCtx(pPrachCtx->pLtePhyStatsSubFrameCtx);
        LteBsPhyStatsTimeHistogramUeCtxType(pUlCurrCtx, (PRACH<<4));
        LteBsPhyStatsTimeHistogramUeCtxNumRach(pUlCurrCtx, pDetEvent->num);
        LteBsPhyStatsTimeHistogramULUeCtxIncrement(pPrachCtx->pLtePhyStatsSubFrameCtx);
    }

#if 0
    if ((pDetEvent->num) && (firstPrach == 0))
    {
        firstPrach = 1;
        DiLogControl (NULL, 0, DI_LOG_TX0_LOG_ENABLED | \
                      DI_LOG_TX1_LOG_ENABLED | \
                      DI_LOG_RX0_LOG_ENABLED | \
                      DI_LOG_TX_LOG_RR | \
                      DI_LOG_RX_LOG_RR);
        stopPhy = 1;
    }
#endif

    // send PRACH SDU
    if (!(gMuteRxSduForSrRach && (pDetEvent->num == 0)))
        LteGenerateRxPrachStatusIndMsg(pPhy, NULL, &outMsgSize,
                                       pDetEvent, pPrachCtx->startedSf, pPrachCtx->startedSfn);

    MLogTask(PID_PRACHLIST_CALC_AND_SEND, RESOURCE_LARM, t1, GET_TICKS);



    // Re-Init the task list
    ReinitTaskList(pBsRx->pExePrachTaskList[pPrachCtx->prachProcInd], NULL);
    pBsRx->isExePrachTaskListReady[pPrachCtx->prachProcInd] = FALSE;

    pPrachCtx->pPhy = NULL;
    pPrachCtx->listState = EXEC_LIST_IDLE;

    MLogTask(PID_PRACHLIST_COMPLETE_LIST, RESOURCE_LARM, tTotal, GET_TICKS);

    return;
}


UINT32 LtePhyInitThreadPrach(void)
{
    PLTE_PHY_PRACH_PROCESS_CTX pPrachCtx = NULL;
    UINT32 i;

    for (i = 0 ;i < PRACH_PROCESS_CTX_LEN; i++)
    {
        pPrachCtx = &prachListProcessCtx[i];
        memset(pPrachCtx, 0, sizeof(LTE_PHY_PRACH_PROCESS_CTX));
        pPrachCtx->listState = EXEC_LIST_IDLE;
    }

    prachListProcessCtxCnt = 0;

    return 0;
}



//-------------------------------------------------------------------------------------------
/** @brief This function disptaches the PRACH taskList to REX
 *
 *  @param   pParam  Pointer to RX PRACH Thread Context
 *
 *  @return   MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_rx_prach
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 LtePhyThreadPrach(PLTEPHYDESC pPhy)
{
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    PLTE_PHY_PRACH_PROCESS_CTX pPrachCtx = NULL;
    PBS_RX pBsRx;
    U32 prachProcIndex, procInd;

    UINT32 t = GET_TICKS;

    procInd = prachListProcessCtxCnt;
    prachListProcessCtxCnt = IncrementPrachProcessCtx(prachListProcessCtxCnt);

    pPrachCtx = &prachListProcessCtx[procInd];

    if (pPrachCtx->listState != EXEC_LIST_IDLE)
    {
        stop_printf("Previous PRACH List not Completed\r\n");
        return 1;
    }

    pBsRx                                   = pPhy->pBsRx;
    prachProcIndex                          = pBsRx->RxPrach_Context;
    pBsRx->RxPrach_Context                  = 1 - pBsRx->RxPrach_Context;
    pBsRx->prachReturnCtx[prachProcIndex]   = procInd;                          // in List Call Back function, we will see this variable. Store PRACH context to take it there

    pPrachCtx->pPhy                         = pPhy;
    pPrachCtx->startedSf                    = pPhy->pBsRx->subframeNumber;
    pPrachCtx->startedSfn                   = pPhy->rxFrameNumber;
    pPrachCtx->prachProcInd                 = prachProcIndex;
    pPrachCtx->pLtePhyStatsSubFrameCtx      = LteBsPhyStatsTimerHistogramGetUlSubFrameCtx(pLteBsPhyStats);
    pPrachCtx->t                            = t;
    pPrachCtx->listId                       = pBsRx->prachListId[prachProcIndex];
    pPrachCtx->listState                    = EXEC_LIST_PROCESSING;

    MLogTCBListStart(pBsRx->prachListId[prachProcIndex]);
    ExeTSchedList(pBsRx->pExePrachTaskList[prachProcIndex]);

    MLogTask(PID_PRACHLIST_RUN_LIST, RESOURCE_LARM, t, GET_TICKS);

    return 0;
}



//-------------------------------------------------------------------------------------------
/** @file LteBsTxThread.c
 *
 * @brief Implementation of DL processing thread for the LTE BS
 * @author Mindspeed Technologies
 * @version $Revision: 1.93 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------


#include "ltephy.h"
#include "ltephydef.h"
#include "ext_phy.h"
#include "Debug.h"
#include "prototypes.h"

#include "ltertconfig.h"
#include "LteBsTxThread.h"
#include "LteBsTxDataSdu.h"
#include "LteBsRxThread.h"
#include "LteBsPhyStats.h"

#include "appprintf.h"
#include "fpdrv.h"
#include "mlog.h"

static LTE_TX_PROCESS_CTX TxProcessCtx[2];
static U32 TxProcessCtxNum = 0;

static PLTE_TX_PROCESS_CTX txListProcessCtx[PROCESS_CTX_BUF_LEN];
static U32 txListProcessCtxPushCnt = 0;
static U32 txListProcessCtxPullCnt = 0;

//-------------------------------------------------------------------------------------------
/** @brief This is the call back function to the TX Modulator process.
 *  Once TX Modulator list is completed, this function is called by REX
 *
 *  @param   thisList Pointer to TX Modulator TaskList
 *  @param   param Pointer to TX Modulator Thread Context
 *
 *  @return none
 *
 *  \ingroup group_lte_phy_control_rt_tx_listrun
 *
**/
//-------------------------------------------------------------------------------------------
void LtePhyTxListDoneCb(TASKLIST* thisList, void* param)
{
    PLTE_TX_PROCESS_CTX pTxProcessCtx;
    PLTEPHYDESC pPhy;
    U32 *pContext;
    U32 t = GET_TICKS, ID;

    pTxProcessCtx = txListProcessCtx[txListProcessCtxPullCnt];
    txListProcessCtxPullCnt = IncrementProcessCtx(txListProcessCtxPullCnt);

    _ASSERT_PTR(pTxProcessCtx);
    _ASSERT(pTxProcessCtx->listStateTx == EXEC_LIST_PROCESSING);

    pPhy = pTxProcessCtx->pPhyList;
    _ASSERT_PTR(pPhy);

    ID = pPhy->pBsTx->txListId;
    MLogTCBListStop(ID);
    MLogTask(PID_TASKLIST_EXEC_00 + ID, RESOURCE_LARM, pTxProcessCtx->t, t);


    // Start IFFT
    pContext = (U32*) pPhy->pBsTx->pIfftTCB[0]->ContextPtr;
    pContext[1] = pTxProcessCtx->t;
    MapIfftDrvRun(pPhy->pBsTx->pIfftTCB[0]);

    pPhy->pBsTx->isListRunning = 0;
    ReinitTaskList(thisList, NULL);

    /* clean up */
    pTxProcessCtx->listStateTx = EXEC_LIST_IDLE;

    if (gLowLatencyFlag)
    {
        RexSetTCBControl(pPhy->pBsTx->pSetupBufsTCB, REX_LOCKED_TCB);
        RexSetTCBControlEx(pPhy->pBsTx->pModTCB, 7, REX_LOCKED_TCB);
    }
    RexSetTCBControl(pPhy->pBsTx->pDlControlTCB[0], REX_LOCKED_TCB);

    MLogTask(PID_TXLIST_COMPLETE_LIST, RESOURCE_LARM, t, GET_TICKS);

    return;
}


//-------------------------------------------------------------------------------------------
/** @brief This is the call back function to the IFFT driver for MAP
 *
 *  @param   param that gives the slot number
 *
 *  @return none
 *
 *  \ingroup group_lte_phy_control_rt_rx_listrun
 *
**/
//-------------------------------------------------------------------------------------------
void LteTxIfftDoneCb(void* param)
{
    PTCB pTcb = (PTCB)param;
    U32 *pContext, i, diffTime;
    PLTEPHYDESC pPhy;
    UINT32 tt = GET_TICKS;
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    PDLTXINPUTPARAMS    pTxInputParams;
    PLTE_BS_PHY_STATS_USER_STAT pPhyStatsUserStat;

    pContext = (U32*) pTcb->ContextPtr;
    pPhy = (PLTEPHYDESC)pContext[0];
    pTxInputParams = pPhy->pBsTx->pTxInputParams;

    // Copy IQ samples to DDR from CRAM for Timer Mode
    PhyDiCopyIq(pPhy, DL);

    diffTime = LteBsPhyStatsTimer(pLteBsPhyStats->timerStats.DLListProcessingTime, pContext[1], tt);

    if(LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_USRSTS_PDSCH) && (diffTime > LTE_BS_HY_STATS_MAX_DL_LIST_PROCESSING_TIME))
    {
        for (i = pTxInputParams->numCtrlChan; i < (U32) pTxInputParams->numChan; i++)
        {
            pPhyStatsUserStat = LteBsPhyStatsGetRntiId(pTxInputParams->nRNTI[i]);
            LteBsPhyStatsUserStatDlNumTtiListProcessingHigh(pPhyStatsUserStat);
        }
    }

    MLogTask(PID_TXLIST_IFFT_CALLBACK, RESOURCE_LARM, tt, GET_TICKS);

    return;
}



//-------------------------------------------------------------------------------------------
/** @brief This function disptaches the Tx Modulator taskList to REX
 *
 *  @param   pParam  Pointer to TX Modulator Thread Context
 *
 *  @return   MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_tx_listrun
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 LtePhyThreadTx(PLTEPHYDESC pPhy)
{
    PLTE_TX_PROCESS_CTX pTxProcessCtx;
    PBS_TX pBsTx = pPhy->pBsTx;
    U32 t = GET_TICKS;

    TxProcessCtxNum = 1 - TxProcessCtxNum;
    pTxProcessCtx = &TxProcessCtx[TxProcessCtxNum];

    if (pTxProcessCtx->listStateTx != EXEC_LIST_IDLE)
    {
        stop_printf("Previous Tx List Not Completed\r\n");
        return 0;
    }

    txListProcessCtx[txListProcessCtxPushCnt] = pTxProcessCtx;
    txListProcessCtxPushCnt = IncrementProcessCtx(txListProcessCtxPushCnt);

    pTxProcessCtx->listStateTx = EXEC_LIST_PROCESSING;
    pTxProcessCtx->t = gStartSubfrTimeMark;
    pTxProcessCtx->pPhyList = pPhy;

    pBsTx->isListRunning = 1;

    MLogTCBListStart(pBsTx->txListId);
    ExeTSchedList(pBsTx->pExeTaskList);

    MLogTask(PID_TXLIST_RUN_LIST, RESOURCE_LARM, t, GET_TICKS);

    return 0;
}



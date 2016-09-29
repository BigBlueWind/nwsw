//-------------------------------------------------------------------------------------------
/** @file LteBsRxSrsThread.c
 *
 * @brief Implementation of SRS processing thread for the LTE BS
 * @author Mindspeed Technologies
 * @version $Revision: 1.19 $
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
#include "LteBsRxSrsThread.h"
#include "ltertconfig.h"
#include "mlog.h"


static LTE_PHY_SRS_THREAD_CTX srsListProcessCtx[SRS_PROCESS_CTX_LEN];
static U32 srsListProcessCtxCntPush = 0;
static U32 srsListProcessCtxCntPop = 0;


//-------------------------------------------------------------------------------------------
/** @brief This function initializes the variables needed for Rx SRS Thread Processing
 *
 *  @param none
 *
 *  @return MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_rx_srs
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 LtePhyInitThreadSrs(void)
{
    PLTE_PHY_SRS_THREAD_CTX pSrsCtx = NULL;
    UINT32 i;

    for (i = 0 ;i < SRS_PROCESS_CTX_LEN; i++)
    {
        pSrsCtx = &srsListProcessCtx[i];
        memset(pSrsCtx, 0, sizeof(LTE_PHY_SRS_THREAD_CTX));
        pSrsCtx->listState = EXEC_LIST_IDLE;
    }

    srsListProcessCtxCntPush    = 0;
    srsListProcessCtxCntPop     = 0;

    return 0;
}



//-------------------------------------------------------------------------------------------
/** @brief This is the call back function to the SRS process. Once SRS list is completed, this function is called by REX
 *
 *  @param   thisList Pointer to SRS TaskList
 *  @param   param Pointer to RX SRS Thread Context
 *
 *  @return none
 *
 *  \ingroup group_lte_phy_control_rt_rx_srs
 *
**/
//-------------------------------------------------------------------------------------------
void LtePhySrsListDoneCb(TASKLIST* pThisList, void* param)
{
    PLTE_PHY_SRS_THREAD_CTX pSrsCtx = NULL;
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    PLTEPHYDESC pPhy;
    PBS_RX pBsRx;
    U32 procInd;
    U32 size;

    UINT32 t = GET_TICKS;

    procInd = srsListProcessCtxCntPop;
    srsListProcessCtxCntPop = IncrementSrsProcessCtx(srsListProcessCtxCntPop);

    pSrsCtx = &srsListProcessCtx[procInd];

    _ASSERT(pSrsCtx->listState == EXEC_LIST_PROCESSING);

    pPhy                        = pSrsCtx->pPhy;
    pBsRx                       = pPhy->pBsRx;

    MLogTCBListStop(pBsRx->srsListId);
    MLogTask(PID_TASKLIST_EXEC_00 + pBsRx->srsListId, RESOURCE_LARM, pSrsCtx->t, t);
    LteBsPhyStatsTimer(pLteBsPhyStats->timerStats.SrsListProcessingTime, pSrsCtx->t, t);

    LteGenerateRxSrsStatusIndMsg(pPhy, NULL, &size, pSrsCtx->startedSf, pSrsCtx->startedSfn);
    MLogTask(PID_SRSLIST_CALC_AND_SEND, RESOURCE_LARM, t, GET_TICKS);

    // Re-Init list for next time
    ReinitTaskList(pBsRx->pExeSrsTaskList[0], NULL);
    pBsRx->isExeSrsTaskListReady = FALSE;

    pSrsCtx->listState          = EXEC_LIST_DONE;
    pSrsCtx->pPhy               = NULL;

    MLogTask(PID_SRSLIST_COMPLETE_LIST, RESOURCE_LARM, t, GET_TICKS);

    return;
}


//-------------------------------------------------------------------------------------------
/** @brief This function disptaches the SRS taskList to REX
 *
 *  @param   pParam  Pointer to RX SRS Thread Context
 *
 *  @return   MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_rx_srs
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 LtePhyThreadSrs(PLTEPHYDESC pPhy)
{
    PLTE_PHY_SRS_THREAD_CTX pSrsCtx = NULL;
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    PBS_RX pBsRx;
    U32 procInd;

    UINT32 t = GET_TICKS;

    procInd = srsListProcessCtxCntPush;
    srsListProcessCtxCntPush = IncrementSrsProcessCtx(srsListProcessCtxCntPush);

    pSrsCtx = &srsListProcessCtx[procInd];

    if (pSrsCtx->listState != EXEC_LIST_IDLE)
    {
        stop_printf("Previous SRS List not Completed\r\n");
        return 1;
    }

    pBsRx                           = pPhy->pBsRx;

    pSrsCtx->t                      = t;
    pSrsCtx->listState              = EXEC_LIST_PROCESSING;
    pSrsCtx->pPhy                   = pPhy;
    pSrsCtx->startedSf              = pBsRx->subframeNumber;
    pSrsCtx->startedSfn             = pPhy->rxFrameNumber;

    MLogTCBListStart(pBsRx->srsListId);
    ExeTSchedList(pBsRx->pExeSrsTaskList[0]);

    MLogTask(PID_SRSLIST_RUN_LIST, RESOURCE_LARM, t, GET_TICKS);

    return 0;
}




//-------------------------------------------------------------------------------------------
/** @file LteBsRxDataSduThread.c
 *
 * @brief This file implements the Rx SDU (Decoder) processing for the LTE BS
 * @author Mindspeed Technologies
 * @version $Revision: 1.66 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#include "ltephy.h"
#include "ltephydef.h"
#include "lte_entry.h"
#include "ext_phy.h"
#include "4gmx_serv.h"
#include "Debug.h"
#include "prototypes.h"
#include "mdmadrv.h"
#include "supervisor.h"
#include "LteBsApiThread.h"
#include "LteBsRxConfig.h"
#include "LteBsRxDataSdu.h"
#include "LteBsRxThread.h"
#include "LteBsRxSduPreproc.h"
#include "FECdrv.h"
#include "gputil_fx.h"
#include "initcpu.h"
#include "supervisor.h"
#include "config.h"
#include "ltertconfig.h"
#include "globalstoragearrays.h"
#include "mlog.h"
#include "ceva_proc.h"



//#define PUSCH_CRC_STOP
//#define MUX_NACK_STOP
//#define MUX_DTX_STOP
//#define PUCCH_NACK_STOP
//#define PUCCH_DTX_STOP
//#define DTX_WORKAROUND
//#define SR_WORK_AROUND
//#define RI_ONE_WORKAROUND
//#define KOREA_BMT_WORKAROUND


void SendList(U8* pListRxEnd, U32 listType, U32 RxSduProcCtx)
{
    U32 PuschPresent, PucchPresent, MuxPuschCqiRiPresent, MuxPuschAckNackPresent;
    U32 lastFlag = 1, sendListFlag = 0, checkLast = 0;

    PuschPresent           = (pRxSduFirstPusch[RxSduProcCtx] ? 1 : 0);
    PucchPresent           = (pRxSduFirstPucch[RxSduProcCtx] ? 1 : 0);
    MuxPuschCqiRiPresent   = (pRxSduFirstMuxCtrlCqiRi[RxSduProcCtx] ? 1 : 0);
    MuxPuschAckNackPresent = (pRxSduFirstMuxCtrlAckNack[RxSduProcCtx] ? 1 : 0);

    // Set the Last Flag
    if (listType == PUSCH)
    {
        if (PucchPresent)
            checkLast = 1;

        if(gTwoRxMuxControlLists)                           // Then ACK NACK is added to the pRxSduFirstMuxCtrlAckNack List
        {
            if (MuxPuschAckNackPresent)
                checkLast = 1;
        }
        else                                                // Then ACK NACK is added to the pRxSduFirstMuxCtrlCqiRi List
        {
            if (MuxPuschCqiRiPresent)
                checkLast = 1;
        }

        if (checkLast)
            lastFlag = ((RxControlSduSyncCtrl[RxSduProcCtx] & PUCCH_LIST_SENT) ? 1 : 0);
    }
    else
    {
        if (PuschPresent)
            lastFlag = ((RxControlSduSyncCtrl[RxSduProcCtx] & PUSCH_LIST_SENT) ? 1 : 0);
    }


    if (listType == PUSCH)                                      // PUSCH
    {
        SendRxSduList(PUSCH_LIST_TYPE, lastFlag, RxSduProcCtx, pListRxEnd);
        RxControlSduSyncCtrl[RxSduProcCtx] |= PUSCH_LIST_SENT;
    }
    else if (listType == PUCCH)                                 // PUCCH
    {
        if(gTwoRxMuxControlLists)                           // Then ACK NACK is added to the pRxSduFirstMuxCtrlAckNack List
        {
            if (MuxPuschAckNackPresent)
            {
                if (RxControlSduSyncCtrl[RxSduProcCtx] & MUX_PUSCH_SDU_ADDED)
                    sendListFlag = 1;
            }
            else
            {
                sendListFlag = 1;
            }
        }
        else                                                // Then ACK NACK is added to the pRxSduFirstMuxCtrlCqiRi List
        {
            if (MuxPuschCqiRiPresent)
            {
                if (RxControlSduSyncCtrl[RxSduProcCtx] & MUX_PUSCH_SDU_ADDED)
                    sendListFlag = 1;
            }
            else
            {
                sendListFlag = 1;
            }
        }

        if (sendListFlag)
        {
            SendRxSduList(PUCCH_LIST_TYPE, lastFlag, RxSduProcCtx, pListRxEnd);
            RxControlSduSyncCtrl[RxSduProcCtx] |= PUCCH_LIST_SENT;
        }
    }
    else if (listType == MUX_PUSCH_ACK_NACK)                    // MUX_PUSCH_ACK_NACK. Called only in case of 2 Lists
    {
        if (PucchPresent)
        {
            if (RxControlSduSyncCtrl[RxSduProcCtx] & PUCCH_SDU_ADDED)
                sendListFlag = 1;
        }
        else
        {
            sendListFlag = 1;
        }

        if (sendListFlag)
        {
            SendRxSduList(PUCCH_LIST_TYPE, lastFlag, RxSduProcCtx, pListRxEnd);
            RxControlSduSyncCtrl[RxSduProcCtx] |= PUCCH_LIST_SENT;
        }
    }
    else if (listType == MUX_PUSCH_CQI_RI)                      // MUX_PUSCH_RI_CQI
    {
        if(gTwoRxMuxControlLists)                               // Then this is a seperate list of its own. Just Send it
        {
            SendRxSduList(OTHER_LIST_TYPE, 0, RxSduProcCtx, NULL); // No RxEnd. This is special list. No need to update Sync Control Flag.
        }
        else                                                    // This is coupled along with PUCCH List
        {
            if (PucchPresent)
            {
                if (RxControlSduSyncCtrl[RxSduProcCtx] & PUCCH_SDU_ADDED)
                    sendListFlag = 1;
            }
            else
            {
                sendListFlag = 1;
            }

            if (sendListFlag)
            {
                SendRxSduList(PUCCH_LIST_TYPE, lastFlag, RxSduProcCtx, pListRxEnd);
                RxControlSduSyncCtrl[RxSduProcCtx] |= PUCCH_LIST_SENT;
            }
        }
    }
}

void LteBsRxSchedulePuschSduCleanUp(UINT32 RxSduProcCtx)
{
    PLTE_PHY_API_THREAD_CTX pApiThreadCtx = LtePhyApiGetCtx();

    pApiThreadCtx->puschCleanUpRxSduProcCtx[pApiThreadCtx->puschCleanUpPush] = RxSduProcCtx;
    pApiThreadCtx->puschCleanUpPush = IncrementSduCtx(pApiThreadCtx->puschCleanUpPush);
    MxScheduleThreadEx(pApiThreadCtx->hLtePhyCleanUpPusch, 0, PHY_SMP_THREAD);
}

void LteBsRxSchedulePucchSduCleanUp(UINT32 RxSduProcCtx)
{
    PLTE_PHY_API_THREAD_CTX pApiThreadCtx = LtePhyApiGetCtx();

    pApiThreadCtx->pucchCleanUpRxSduProcCtx[pApiThreadCtx->pucchCleanUpPush] = RxSduProcCtx;
    pApiThreadCtx->pucchCleanUpPush = IncrementSduCtx(pApiThreadCtx->pucchCleanUpPush);
    MxScheduleThreadEx(pApiThreadCtx->hLtePhyCleanUpPucch, 0, PHY_SMP_THREAD);
}

void LteBsRxScheduleMuxPuschSduCleanUp(UINT32 RxSduProcCtx)
{
    PLTE_PHY_API_THREAD_CTX pApiThreadCtx = LtePhyApiGetCtx();

    pApiThreadCtx->muxPuschCleanUpRxSduProcCtx[pApiThreadCtx->muxPuschCleanUpPush] = RxSduProcCtx;
    pApiThreadCtx->muxPuschCleanUpPush = IncrementSduCtx(pApiThreadCtx->muxPuschCleanUpPush);
    MxScheduleThreadEx(pApiThreadCtx->hLtePhyCleanUpMuxPusch, 0, PHY_SMP_THREAD);
}


UINT32 LteBsRxPuschSduCleanUp(LPVOID pParam)
{
    PLTE_PHY_API_THREAD_CTX pApiThreadCtx = LtePhyApiGetCtx();
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    UINT32 phyStatsEnabledMaskTimeHist = LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_TIME_HISTOGRAM);
    UINT32 phyStatsEnabledMaskPusch = LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_USRSTS_PUSCH);
    PLTE_PHY_UL_SDU_DESC FirstPusch = NULL, CurrPusch = NULL;
    U32 tTotal;
    UINT32 RxSduCtx;

    while(pApiThreadCtx->puschCleanUpPush != pApiThreadCtx->puschCleanUpPop)
    {
        tTotal = GET_TICKS;
        RxSduCtx = pApiThreadCtx->puschCleanUpRxSduProcCtx[pApiThreadCtx->puschCleanUpPop];
        pApiThreadCtx->puschCleanUpPop = IncrementSduCtx(pApiThreadCtx->puschCleanUpPop);

        FirstPusch          = pRxSduFirstPusch[RxSduCtx];
        pRxSduFirstPusch[RxSduCtx] = NULL;

        CurrPusch = FirstPusch;

        while(CurrPusch)
        {
            if (CurrPusch->stat != STAT_SUCCESS)
            {
                if (phyStatsEnabledMaskPusch)
                    LteBsPhyStatsUserStatPuschError(CurrPusch->pUserStat);
                if (phyStatsEnabledMaskTimeHist)
                    LteBsPhyStatsTimeHistogramUeCtxResult(CurrPusch->pPhyStatsUlUeCtx, RX_CRC_ERROR);
#ifdef PUSCH_CRC_STOP
                if (stopPhy == 0)
                    stopPhy = 1;
#endif
            }
            else
            {
                if (phyStatsEnabledMaskTimeHist)
                    LteBsPhyStatsTimeHistogramUeCtxResult(CurrPusch->pPhyStatsUlUeCtx, SUCCESS);
            }

            // Clean (Free memory)
            MemFree(CurrPusch->out, IRAM_MEM);
            CurrPusch->out = NULL;

            CurrPusch = CurrPusch->Next;
        }
        MLogTask((PID_RXSDU_CTX0_PUSCH_SDU_CLEANUP + (RxSduCtx*30)), RESOURCE_LARM, tTotal, GET_TICKS);
    }

    return 0;
}



UINT32 LteBsRxPucchSduCleanUp(LPVOID pParam)
{
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    PLTE_PHY_API_THREAD_CTX pApiThreadCtx = LtePhyApiGetCtx();
    UINT32 phyStatsEnabledMaskTimeHist = LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_TIME_HISTOGRAM);
    UINT32 phyStatsEnabledMaskPucch = LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_USRSTS_PUCCH);
    UINT32 phyStatsEnabledMaskPusch = LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_USRSTS_PUSCH);
    PLTE_PHY_UL_SDU_DESC CurrPusch = NULL, CurrPucch = NULL;
    PLTE_PHY_UL_SDU_DESC FirstPusch = NULL, FirstPucch = NULL;
    U32 tTotal, nc = 0, pucch_format, isCqi = 0, cqiRi = 0, cqiConf;
    UINT32 RxSduProcCtx;

    while(pApiThreadCtx->pucchCleanUpPush != pApiThreadCtx->pucchCleanUpPop)
    {
        tTotal = GET_TICKS;

        RxSduProcCtx = pApiThreadCtx->pucchCleanUpRxSduProcCtx[pApiThreadCtx->pucchCleanUpPop];
        pApiThreadCtx->pucchCleanUpPop = IncrementSduCtx(pApiThreadCtx->pucchCleanUpPop);

        FirstPusch          = pRxSduFirstPusch[RxSduProcCtx];
        FirstPucch          = pRxSduFirstPucch[RxSduProcCtx];
        pRxSduFirstPucch[RxSduProcCtx] = NULL;

#if defined (PUCCH_NACK_STOP) || defined (PUCCH_DTX_STOP)
        if(FirstPucch)
        {
            // PUCCH send message
            CurrPucch           = FirstPucch;

            while (CurrPucch)
            {
                nc                  = CurrPucch->chanIndex;
                pucch_format        = CurrPucch->pUlRxFecParams->pucchFormat[nc];

#ifdef PUCCH_NACK_STOP
                if (CurrPucch->stat != STAT_PUCCH_DEADZONE)
                {
                    // PUCCH Format 1a / Format 1b Ant 1 Nack
                    if (pucch_format == FORMAT1A || pucch_format == FORMAT1B)
                        if ((CurrPucch->out[1] & 0x04) == 0)
                            if (stopPhy == 0)
                                stopPhy = 1;

                    // PUCCH Format 1b Ant 2 Nack
                    if (pucch_format == FORMAT1B)
                        if ((CurrPucch->out[1] & 0x02) == 0)
                            if (stopPhy == 0)
                                stopPhy = 1;
                }
#endif

#ifdef PUCCH_DTX_STOP
                if (CurrPucch->stat == STAT_PUCCH_DEADZONE)
                {
                    // PUCCH Format 1a / Format 1b DTX
                    if (pucch_format == FORMAT1A || pucch_format == FORMAT1B)
                        if (stopPhy == 0)
                            stopPhy = 1;
                }
#endif
                CurrPucch = CurrPucch->Next;
            }

        }
#endif

        // Muting sending of SR PUCCH if we did not detect it. We need to clean these allocated buffers
        if(gMuteRxSduForSrRach && FirstPucch)
        {
            CurrPucch = FirstPucch;
            while (CurrPucch)
            {
                if (CurrPucch->pListElement)
                {
                    PushPUCCHSdu((U32)CurrPucch->pListElement);
                    CurrPucch->pListElement = NULL;
                }

                CurrPucch = CurrPucch->Next;
            }
        }

        // PHY Stats
        if (phyStatsEnabledMaskTimeHist | phyStatsEnabledMaskPucch | phyStatsEnabledMaskPusch)
        {
            // PUSCH
            if(FirstPusch)
            {
                CurrPusch = FirstPusch;
                while (CurrPusch)
                {
                    nc = CurrPusch->chanIndex;

                    if (phyStatsEnabledMaskPusch)
                    {
                        LteBsPhyStatsCalcSinrTmAdv(CurrPusch->pUserStat, CurrPusch->ul_CQI,
                            CurrPusch->TimingAdvance, PUSCH, 0);
                        LteBsPhyStatsAveragePuschCqi(CurrPusch->pUserStat, CurrPusch->ul_CQI);
                        LteBsPhyStatsAveragePuschCqiIncrementCtr(CurrPusch->pUserStat);
                    }
                    if (phyStatsEnabledMaskTimeHist)
                    {
                        LteBsPhyStatsTimeHistogramUeCtxTmgAdv(CurrPusch->pPhyStatsUlUeCtx, CurrPusch->TimingAdvance);
                        LteBsPhyStatsTimeHistogramUeCtxSinr(CurrPusch->pPhyStatsUlUeCtx, CurrPusch->ul_CQI);
                    }

                    CurrPusch = CurrPusch->Next;
                }
            }

            //PUCCH
            if(FirstPucch)
            {
                CurrPucch = FirstPucch;
                while (CurrPucch)
                {
                    nc                  = CurrPucch->chanIndex;
                    pucch_format        = CurrPucch->pUlRxFecParams->pucchFormat[nc];

                    if (phyStatsEnabledMaskPucch)
                    {
                        LteBsPhyStatsCalcSinrTmAdv(CurrPucch->pUserStat, CurrPucch->ul_CQI,
                            CurrPucch->TimingAdvance, PUCCH, pucch_format);

                        LteBsPhyStatsAveragePucchCqi(CurrPucch->pUserStat, CurrPucch->ul_CQI);
                        LteBsPhyStatsAveragePucchCqiIncrementCtr(CurrPucch->pUserStat);

                        if (CurrPucch->stat != STAT_PUCCH_DEADZONE)
                        {
                            // Phy Stats for DL CQI AND RI on PUCCH
                            if (pucch_format > FORMAT1B)
                            {
                                isCqi = LteBsIsCqiPmi(CurrPucch->pUlRxSduParams->dlCqiPmiSizeBits[nc]);
                                if (isCqi)
                                {
                                    cqiRi = ((CurrPucch->out[0] >> 4) & 0x0F);
                                }
                                else
                                {
                                    cqiRi = ((CurrPucch->out[0] >> (8 - CurrPucch->pUlRxSduParams->dlCqiPmiSizeBits[nc])) & 0x0F);
                                }
                            }

                            if (pucch_format >= FORMAT2 && isCqi)
                            {
                                cqiConf = (((CurrPucch->cqiPmiConf>>14) & 0x3) == 0x3) ? 1 : 0;
                                LteBsPhyStatsUserStatCqiPmiConfMetric(CurrPucch->pUserStat, (pucch_format-FORMAT2), cqiConf);
                            }

                            if (pucch_format == FORMAT1)
                                LteBsPhyStatsUserStatPucchFormat1Det(CurrPucch->pUserStat);

                            else if (pucch_format == FORMAT1A)
                                LteBsPhyStatsUserStatPucchFormat1ANack(CurrPucch->pUserStat, ((CurrPucch->out[1] & 0x04) ? 0:1));

                            else if (pucch_format == FORMAT1B)
                            {
                                LteBsPhyStatsUserStatPucchFormat1BaNack(CurrPucch->pUserStat, ((CurrPucch->out[1] & 0x04) ? 0:1));
                                LteBsPhyStatsUserStatPucchFormat1BbNack(CurrPucch->pUserStat, ((CurrPucch->out[1] & 0x02) ? 0:1));
                            }

                            else if (pucch_format == FORMAT2)
                            {
                                LteBsPhyStatsPdschCqiRi(CurrPucch->pUserStat, cqiRi, PUCCH, FORMAT2, isCqi);
                            }

                            else if (pucch_format == FORMAT2A)
                            {
                                LteBsPhyStatsUserStatPucchFormat2ANack(CurrPucch->pUserStat, ((CurrPucch->out[1] & 0x04) ? 0:1));
                                LteBsPhyStatsPdschCqiRi(CurrPucch->pUserStat, cqiRi, PUCCH, FORMAT2A, isCqi);
                            }

                            else if (pucch_format == FORMAT2B)
                            {
                                LteBsPhyStatsUserStatPucchFormat2BaNack(CurrPucch->pUserStat, ((CurrPucch->out[1] & 0x04) ? 0:1));
                                LteBsPhyStatsUserStatPucchFormat2BbNack(CurrPucch->pUserStat, ((CurrPucch->out[1] & 0x02) ? 0:1));
                                LteBsPhyStatsPdschCqiRi(CurrPucch->pUserStat, cqiRi, PUCCH, FORMAT2B, isCqi);
                            }
                        }
                        else
                        {
                            if (pucch_format == FORMAT1A)
                                LteBsPhyStatsUserStatPucchFormat1ADtx(CurrPucch->pUserStat);

                            else if (pucch_format == FORMAT1B)
                                LteBsPhyStatsUserStatPucchFormat1BDtx(CurrPucch->pUserStat);

                            else if (pucch_format == FORMAT2A)
                                LteBsPhyStatsUserStatPucchFormat2ADtx(CurrPucch->pUserStat);

                            else if (pucch_format == FORMAT2B)
                                LteBsPhyStatsUserStatPucchFormat2BDtx(CurrPucch->pUserStat);
                        }

                        LteBsPhyStatsUserStatSimultaneousSrDet(CurrPucch->pUserStat, pucch_format, CurrPucch->srdetected);
                    }

                    if (phyStatsEnabledMaskTimeHist)
                    {
                        LteBsPhyStatsTimeHistogramUeCtxTmgAdv(CurrPucch->pPhyStatsUlUeCtx, CurrPucch->TimingAdvance);
                        LteBsPhyStatsTimeHistogramUeCtxSinr(CurrPucch->pPhyStatsUlUeCtx, CurrPucch->ul_CQI);
                        if (CurrPucch->stat != STAT_PUCCH_DEADZONE)
                        {
                            LteBsPhyStatsTimeHistogramUeCtxResult(CurrPucch->pPhyStatsUlUeCtx, CurrPucch->out[1]);
                        }
                        else
                        {
                            LteBsPhyStatsTimeHistogramUeCtxResult(CurrPucch->pPhyStatsUlUeCtx, 0xFF);
                        }
                    }

                    CurrPucch = CurrPucch->Next;
                }
            }
        }
        MLogTask((PID_RXSDU_CTX0_PUCCH_SDU_CLEANUP + (RxSduProcCtx*30)), RESOURCE_LARM, tTotal, GET_TICKS);
    }
    return 0;

}



UINT32 LteBsRxMuxPuschSduCleanUp(LPVOID pParam)
{
    PLTE_PHY_API_THREAD_CTX pApiThreadCtx = LtePhyApiGetCtx();
    PLTE_PHY_UL_SDU_DESC FirstPuschCqiRi, NextMuxPuschCqiRi;
    ULRXFECPARAMS *pFecPars;
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    UINT32 phyStatsEnabledMask = LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_USRSTS_MUX_PUSCH);
    U32 chIdx, cqiConf;
    U32 t;
    UINT32 RxSduCtx;

    while(pApiThreadCtx->muxPuschCleanUpPush != pApiThreadCtx->muxPuschCleanUpPop)
    {
        t = GET_TICKS;
        RxSduCtx = pApiThreadCtx->muxPuschCleanUpRxSduProcCtx[pApiThreadCtx->muxPuschCleanUpPop];
        pApiThreadCtx->muxPuschCleanUpPop = IncrementSduCtx(pApiThreadCtx->muxPuschCleanUpPop);

        FirstPuschCqiRi          = pRxSduFirstMuxCtrlCqiRi[RxSduCtx];
        pRxSduFirstMuxCtrlCqiRi[RxSduCtx] = NULL;

        NextMuxPuschCqiRi        = FirstPuschCqiRi;
        pFecPars                 = FirstPuschCqiRi->pUlRxFecParams;

#ifdef MUX_NACK_STOP
        while(NextMuxPuschCqiRi)
        {
            chIdx    = NextMuxPuschCqiRi->chanIndex;

            if (pFecPars->numbitsACK[chIdx])
            {
                // Mux Ant1 Nack
                if ((pFecPars->ackStore[chIdx]) & 0x80 == 0)
                    if (stopPhy == 0)
                        stopPhy = 1;

                // Mux Ant2 Nack
                if (pFecPars->nACK[chIdx] > 1)
                    if ((pFecPars->ackStore[chIdx]) & 0x40 == 0)
                        if (stopPhy == 0)
                            stopPhy = 1;

                // Mux DTX
                if ((pFecPars->ackStore[chIdx]) & 0x20 == 0)
                    if (stopPhy == 0)
                        stopPhy = 1;
            }

            NextMuxPuschCqiRi = NextMuxPuschCqiRi->NextCqiRiMuxCtrl;
        }
#endif

        // Phy Stats
        if (phyStatsEnabledMask)
        {
            NextMuxPuschCqiRi        = FirstPuschCqiRi;

            while(NextMuxPuschCqiRi)
            {
                chIdx    = NextMuxPuschCqiRi->chanIndex;

                if (pFecPars->numbitsACK[chIdx])
                {
                    LteBsPhyStatsUserStatNumMuxPuschNack1(NextMuxPuschCqiRi->pUserStat, (((pFecPars->ackStore[chIdx]) & 0x80) ? 0:1));
                    if (pFecPars->nACK[chIdx] > 1)
                        LteBsPhyStatsUserStatNumMuxPuschNack2(NextMuxPuschCqiRi->pUserStat, (((pFecPars->ackStore[chIdx]) & 0x40) ? 0:1));
                    LteBsPhyStatsUserStatNumMuxPuschAckNackDtx(NextMuxPuschCqiRi->pUserStat, (((pFecPars->ackStore[chIdx]) & 0x20) ? 0:1));
                }

                if (pFecPars->nr1CQI[chIdx] + pFecPars->nrg1CQI[chIdx])
                {
                    LteBsPhyStatsPdschCqiRi(NextMuxPuschCqiRi->pUserStat, ((pFecPars->cqiStore[chIdx][0] >> 4) & 0xF), PUSCH, 0, 1);

                    if ((pFecPars->nr1CQI[chIdx] + pFecPars->nrg1CQI[chIdx]) <= 11)
                    {
                        cqiConf = (((NextMuxPuschCqiRi->cqiPmiConf>>14) & 0x3) == 0x3) ? 1 : 0;
                        LteBsPhyStatsUserStatCqiPmiConfMetric(NextMuxPuschCqiRi->pUserStat, 3, cqiConf);
                    }
                }

                if (pFecPars->nRI[chIdx])
                {
                    LteBsPhyStatsPdschCqiRi(NextMuxPuschCqiRi->pUserStat, ((pFecPars->riStore[chIdx] >> (8-pFecPars->nRI[chIdx])) & 0xF), PUSCH, 0, 0);
                }

                NextMuxPuschCqiRi = NextMuxPuschCqiRi->NextCqiRiMuxCtrl;
            }
        }

        MLogTask((PID_RXSDU_CTX0_MUX_PUSCH_SDU_CLEANUP + (RxSduCtx*30)), RESOURCE_LARM, t, GET_TICKS);
    }

    return 0;
}



//-------------------------------------------------------------------------------------------
/** @brief Rx MDMA / FEC Callback function. This is called by 4GMX once the hardware resource interrupts the ARM after
 *              finishing a task
 *
 *  @param   pCtx  Pointer to RX SDU Decoder Thread Context
 *
 *  @return   none
 *
 *  \ingroup group_lte_phy_control_rt_rx_sdu
 *
**/
//-------------------------------------------------------------------------------------------
void LtePhySduDecMdmaFecCbDone(LPVOID pCtx)
{
    PLTE_PHY_UL_PUSCH_SDU_DESC pSduPuschProc;
    PLTE_PHY_UL_SDU_DESC FirstPusch, CurrPusch;
    U32 B, CrcCalc;
    U32 tt, tt2, mlogPid;

    _ASSERT_PTR(pCtx);
    pSduPuschProc = (PLTE_PHY_UL_PUSCH_SDU_DESC) pCtx;
    FirstPusch          = (PLTE_PHY_UL_SDU_DESC) pSduPuschProc->pFirst;
    mlogPid = FirstPusch->RxSduProcCtx * 30;

    if (pSduPuschProc->sduState == SDU_FEC_UL_DONE)
    {
        tt = GET_TICKS;
        CurrPusch = (PLTE_PHY_UL_SDU_DESC) pSduPuschProc->pFirst;

        MLogTask((PID_RXSDU_CTX0_DATA_FEC + mlogPid), RESOURCE_FECUL, pSduPuschProc->t_fec, tt);

        pSduPuschProc->t_mdma = tt;
        pSduPuschProc->sduState = SDU_CRC_UL_DONE;
        MdmaRunExQ (MDMA_HW_SYS, pSduPuschProc->pMdmaStart, LtePhySduDecMdmaFecCbDone, pSduPuschProc, 200);
        MLogTask((PID_RXSDU_CTX0_DATA_DISPATCH_CRC + mlogPid), RESOURCE_LARM, tt-20, tt);
    }
    else if (pSduPuschProc->sduState == SDU_CRC_UL_DONE)
    {
        PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
        tt = GET_TICKS;

        MLogTask((PID_RXSDU_CTX0_DATA_CRC + mlogPid), RESOURCE_SYS_AXI, pSduPuschProc->t_mdma, tt);

        FirstPusch->t_arm   = pSduPuschProc->t_total;       // For PhyStats
        CurrPusch           = FirstPusch;

        while(CurrPusch)
        {
            B  = (CurrPusch->out[CurrPusch->outp] << 16);
            B |= (CurrPusch->out[CurrPusch->outp + 1] << 8);
            B |= (CurrPusch->out[CurrPusch->outp + 2]);

            _ASSERT_PTR(CurrPusch->CrcOut);
            CrcCalc  = (CurrPusch->CrcOut[CurrPusch->outp + 2] << 16);
            CrcCalc |= (CurrPusch->CrcOut[CurrPusch->outp + 1] << 8);
            CrcCalc |= (CurrPusch->CrcOut[CurrPusch->outp]);

            CurrPusch->stat = STAT_SUCCESS;

            // If received parity not same as computed parity, then
            if (B != CrcCalc)
            {
                // Clear status bit0.
                CurrPusch->stat = CurrPusch->stat & 2;
            }

            // Send RXSDUs to MAC
            if (CurrPusch->out)
            {
                LteRxSduAdd2List(CurrPusch, ((CurrPusch->stat == STAT_SUCCESS) ? SUCCESS : RX_CRC_ERROR),
                    PUSCH_LIST_TYPE, CurrPusch->RxSduProcCtx, CurrPusch->pListElement);
                CurrPusch->pListElement = NULL;
            }

            CurrPusch = CurrPusch->Next;
        }

        SendList(FirstPusch->pListRxEnd, PUSCH, FirstPusch->RxSduProcCtx);

        LteBsRxSchedulePuschSduCleanUp(FirstPusch->RxSduProcCtx);

        tt2 = GET_TICKS;
        LteBsPhyStatsTimer(pLteBsPhyStats->timerStats.ULSDUPuschProcessing, FirstPusch->t_arm, tt2);
        MLogTask((PID_RXSDU_CTX0_DATA_SEND_LIST + mlogPid), RESOURCE_LARM, tt, tt2);
    }
    return;
}


//-------------------------------------------------------------------------------------------
/** @brief Callback function to send MUX Control messages
 *
 *  @return  none
 *
 *  \ingroup group_lte_phy_control_rt_rx_sdu
 *
 **/
//-------------------------------------------------------------------------------------------
static void MuxControlCqiRiDone(void* param, UINT32 ceva_ret)
{
    PLTE_PHY_UL_SDU_DESC FirstPuschCqiRi, NextMuxPuschCqiRi;
    ULRXFECPARAMS *pFecPars;
    U32 chIdx, listType;
    U32 t, t2;
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();

    t = GET_TICKS;

    FirstPuschCqiRi     = (PLTE_PHY_UL_SDU_DESC)param;
    NextMuxPuschCqiRi   = FirstPuschCqiRi;

    MLogTask(TASKID_RX_FEC_MUX_RI_CQI_CNTL_WRAP, RESOURCE_LARM, FirstPuschCqiRi->t_arm, t);

    pFecPars            = FirstPuschCqiRi->pUlRxFecParams;

    if (gTwoRxMuxControlLists)
        listType = OTHER_LIST_TYPE;         // Special case of sending MUX CQI and RI as a seperate list
    else
        listType = PUCCH_LIST_TYPE;

    while(NextMuxPuschCqiRi)
    {
        chIdx    = NextMuxPuschCqiRi->chanIndex;

#ifdef DTX_WORKAROUND
        if (pFecPars->numbitsACK[chIdx])
        {
            pFecPars->ackStore[chIdx] = 0xE0;
        }
#endif

#ifdef RI_ONE_WORKAROUND
        if (pFecPars->numbitsRI[chIdx])
        {
            pFecPars->riStore[chIdx] &= 0x3F;
        }
#endif

        NextMuxPuschCqiRi->cqiPmiConf = pFecPars->rmconf[chIdx];

        LteRxStatusCqiRiHiAdd2List(NextMuxPuschCqiRi, MUX_PUSCH_CQI_RI, listType, NextMuxPuschCqiRi->RxSduProcCtx, NextMuxPuschCqiRi->pListElementCqiRi);

        NextMuxPuschCqiRi->pListElementCqiRi = NULL;
        NextMuxPuschCqiRi                    = NextMuxPuschCqiRi->NextCqiRiMuxCtrl;
    }

    if (gTwoRxMuxControlLists == 0)                 // Need to Update this only if 2 control lists are not present
        RxControlSduSyncCtrl[FirstPuschCqiRi->RxSduProcCtx] |= MUX_PUSCH_SDU_ADDED;
    SendList(FirstPuschCqiRi->pListRxEnd, MUX_PUSCH_CQI_RI, FirstPuschCqiRi->RxSduProcCtx);

    t2 = GET_TICKS;
    LteBsPhyStatsTimer(pLteBsPhyStats->timerStats.ULSDUPuschMuxCtrlCqiRiProcessing, FirstPuschCqiRi->t_total, t2);

    LteBsRxScheduleMuxPuschSduCleanUp(FirstPuschCqiRi->RxSduProcCtx);

    MLogTask((PID_RXSDU_CTX0_MUX_PUSCH_CQI_SEND_LIST + (FirstPuschCqiRi->RxSduProcCtx*30)), RESOURCE_LARM, t, t2);

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief Callback function to send MUX Control messages
 *
 *  @return  none
 *
 *  \ingroup group_lte_phy_control_rt_rx_sdu
 *
 **/
//-------------------------------------------------------------------------------------------
static void MuxControlAckNackReport(PLTE_PHY_UL_SDU_DESC pFirstAckNackMuxContorl)
{
    ULRXFECPARAMS *pFecPars;
    U32 t, t2, chIdx;
    PLTE_PHY_UL_SDU_DESC FirstMuxPuschAckNack, NextMuxPuschAckNack;
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    UINT32 phyStatsEnabledMask = LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_USRSTS_MUX_PUSCH);

    t = GET_TICKS;

    FirstMuxPuschAckNack = (PLTE_PHY_UL_SDU_DESC)pFirstAckNackMuxContorl;

    pFecPars             = FirstMuxPuschAckNack->pUlRxFecParams;

    NextMuxPuschAckNack  = FirstMuxPuschAckNack;
    while(NextMuxPuschAckNack)
    {
        chIdx            = NextMuxPuschAckNack->chanIndex;

        if (pFecPars->numbitsACK[chIdx] && phyStatsEnabledMask)
        {
            LteBsPhyStatsUserStatNumMuxPuschNack1(NextMuxPuschAckNack->pUserStat, (((pFecPars->ackStore[chIdx]) & 0x80) ? 0:1));
            if (pFecPars->nACK[chIdx] > 1)
                LteBsPhyStatsUserStatNumMuxPuschNack2(NextMuxPuschAckNack->pUserStat, (((pFecPars->ackStore[chIdx]) & 0x40) ? 0:1));
            LteBsPhyStatsUserStatNumMuxPuschAckNackDtx(NextMuxPuschAckNack->pUserStat, (((pFecPars->ackStore[chIdx]) & 0x20) ? 0:1));
        }

#ifdef MUX_NACK_STOP
        if (pFecPars->numbitsACK[chIdx])
        {
            // Mux Ant1 Nack
            if ((pFecPars->ackStore[chIdx]) & 0x80 == 0)
                if (stopPhy == 0)
                    stopPhy = 1;

            // Mux Ant2 Nack
            if (pFecPars->nACK[chIdx] > 1)
                if ((pFecPars->ackStore[chIdx]) & 0x40 == 0)
                    if (stopPhy == 0)
                        stopPhy = 1;
            // Mux DTX
            if ((pFecPars->ackStore[chIdx]) & 0x20 == 0)
                if (stopPhy == 0)
                    stopPhy = 1;
       }
#endif

#ifdef DTX_WORKAROUND
        if (pFecPars->numbitsACK[chIdx])
        {
            pFecPars->ackStore[chIdx] = 0xE0;
        }
#endif

        LteRxStatusCqiRiHiAdd2List(NextMuxPuschAckNack, MUX_PUSCH_ACK_NACK, PUCCH_LIST_TYPE, NextMuxPuschAckNack->RxSduProcCtx, NextMuxPuschAckNack->pListElementHi);

        NextMuxPuschAckNack->pListElementHi = NULL;
        NextMuxPuschAckNack                 = NextMuxPuschAckNack->NextHiMuxCtrl;
    }

    RxControlSduSyncCtrl[FirstMuxPuschAckNack->RxSduProcCtx] |= MUX_PUSCH_SDU_ADDED;
    SendList(FirstMuxPuschAckNack->pListRxEnd, MUX_PUSCH_ACK_NACK, FirstMuxPuschAckNack->RxSduProcCtx);

    if (gTwoRxMuxControlLists)
        pRxSduFirstMuxCtrlAckNack[FirstMuxPuschAckNack->RxSduProcCtx] = NULL;
    else
        pRxSduFirstMuxCtrlCqiRi[FirstMuxPuschAckNack->RxSduProcCtx] = NULL;

    t2 = GET_TICKS;
    if (gTwoRxMuxControlLists)
        LteBsPhyStatsTimer(pLteBsPhyStats->timerStats.ULSDUPuschMuxCtrlHarqProcessing, pFirstAckNackMuxContorl->t_total, t2);
    else
        LteBsPhyStatsTimer(pLteBsPhyStats->timerStats.ULSDUPuschMuxCtrlCqiRiProcessing, pFirstAckNackMuxContorl->t_total, t2);

    MLogTask((PID_RXSDU_CTX0_MUX_PUSCH_ACKNACK_SEND_LIST + (FirstMuxPuschAckNack->RxSduProcCtx*30)), RESOURCE_LARM, t, t2);

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief Callback function to send PUCCH messages
 *
 *  @return  none
 *
 *  \ingroup group_lte_phy_control_rt_rx_sdu
 *
 **/
//-------------------------------------------------------------------------------------------
void SNRDone(TCB* pTCB, void* param)
{
    PLTE_PHY_API_THREAD_CTX pApiThreadCtx = LtePhyApiGetCtx();
    U32 tt, tt2, tTotal, nc, *pContext;
    U32 chanId;
    PULRXINPUTPARAMS pUlRxInputParams;
    PLTE_PHY_UL_SDU_DESC CurrPusch = NULL, CurrPucch = NULL;
    PLTE_PHY_UL_SDU_DESC FirstPusch = NULL, FirstPucch = NULL;
    UINT32 RxSduProcCtx, mlogPid;
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();

    tTotal = GET_TICKS;

    pContext = pTCB->ContextPtr;
    RxSduProcCtx = pContext[1];

    FirstPusch          = pRxSduFirstPusch[RxSduProcCtx];
    FirstPucch          = pRxSduFirstPucch[RxSduProcCtx];
    mlogPid             = RxSduProcCtx*30;

    if(FirstPusch)
    {
        CurrPusch = FirstPusch;
        pUlRxInputParams = CurrPusch->pUlRxInputParams;
        tt = GET_TICKS;
        while (CurrPusch)
        {
            // Setup these variables for LteMsgMgr (used to send to MAC)
            nc = CurrPusch->chanIndex;
            chanId = pUlRxInputParams->ChanId[nc];
            CurrPusch->srdetected    = 0;
            CurrPusch->TimingAdvance = pUlRxInputParams->BsRxChMeas[chanId].TimingAdvance;
            CurrPusch->mErrAvg[0]    = pUlRxInputParams->mErrAvg[2*chanId];
            CurrPusch->mErrAvg[1]    = pUlRxInputParams->mErrAvg[2*chanId + 1];
            CurrPusch->mErrExpo      = pUlRxInputParams->mErrExpo[chanId];
            CurrPusch->ul_CQI        = pUlRxInputParams->ul_CQI[chanId];

            CurrPusch = CurrPusch->Next;
        }
        MLogTask((PID_RXSDU_CTX0_DATA_UPDATE_TA_CQI + mlogPid), RESOURCE_LARM, tt, GET_TICKS);
    }

    if(FirstPucch)
    {
        // PUCCH send message
        CurrPucch = FirstPucch;
        pUlRxInputParams = CurrPucch->pUlRxInputParams;
        FirstPucch->t_arm = pContext[0];

        tt = GET_TICKS;
        while (CurrPucch)
        {
            nc = CurrPucch->chanIndex;
            chanId = pUlRxInputParams->ChanId[nc];

            CurrPucch->srdetected    = pUlRxInputParams->srdetected[chanId];
            CurrPucch->TimingAdvance = pUlRxInputParams->BsRxChMeas[chanId].TimingAdvance;
            CurrPucch->mErrAvg[0]    = pUlRxInputParams->mErrAvg[2*chanId];
            CurrPucch->mErrAvg[1]    = pUlRxInputParams->mErrAvg[2*chanId + 1];
            CurrPucch->mErrExpo      = pUlRxInputParams->mErrExpo[chanId];
            CurrPucch->cqiPmiConf    = pUlRxInputParams->rmconf[chanId];
            CurrPucch->ul_CQI        = pUlRxInputParams->ul_CQI[chanId];
            CurrPucch->out[0]        = CurrPucch->pUlRxSduParams->pucch_mac_sdu[nc][0];
            CurrPucch->out[1]        = CurrPucch->pUlRxSduParams->pucch_mac_sdu[nc][1];
            CurrPucch->stat          = CurrPucch->pUlRxSduParams->stat[nc];
            CurrPucch->Rssi_dB       = DEFAULT_RSSI;

#ifdef SR_WORK_AROUND
            if (CurrPucch->pUlRxFecParams->pucchFormat[nc] == FORMAT1)
            {
                CurrPucch->srdetected = 1;
            }
            else
            {
                if (CurrPucch->simSRHarq)
                    CurrPucch->srdetected = 1;
            }
#endif

#ifdef DTX_WORKAROUND
            if (CurrPucch->pUlRxFecParams->pucchFormat[nc] == FORMAT1A || CurrPucch->pUlRxFecParams->pucchFormat[nc] == FORMAT1B
                || CurrPucch->pUlRxFecParams->pucchFormat[nc] == FORMAT2A || CurrPucch->pUlRxFecParams->pucchFormat[nc] == FORMAT2B)
            {
                CurrPucch->out[1] = 0x06;
                CurrPucch->numBitsRx = 16;
                CurrPucch->stat = SUCCESS;
            }
#endif

#ifdef KOREA_BMT_WORKAROUND
            if (CurrPucch->pUlRxFecParams->pucchFormat[nc] == FORMAT1B && CurrPucch->pUlRxInputParams->simSRHarq[nc])
            {
                if (CurrPucch->out[1] & 0x06)
                {
                    CurrPucch->out[1] = 0x06;
                    CurrPucch->numBitsRx = 16;
                    CurrPucch->stat = SUCCESS;
                }
            }
#endif


#ifdef RI_ONE_WORKAROUND
            if (CurrPucch->pUlRxFecParams->pucchFormat[nc] > FORMAT1B)
            {
                if(CurrPucch->pUlRxSduParams->dlCqiPmiSizeBits[nc] != 4)
                {
                    CurrPucch->out[0] = 0;
                }
            }
#endif

            if (CurrPucch->stat == STAT_PUCCH_DEADZONE)
            {
                CurrPucch->numBitsRx = 0;
                CurrPucch->srdetected = 0;
            }

            if (!(gMuteRxSduForSrRach && (CurrPucch->pUlRxFecParams->pucchFormat[nc] == FORMAT1) && (CurrPucch->srdetected == 0)))
            {
                LteRxSduAdd2List(CurrPucch, SUCCESS, PUCCH_LIST_TYPE, CurrPucch->RxSduProcCtx, CurrPucch->pListElement);
                CurrPucch->pListElement = NULL;
            }

            CurrPucch = CurrPucch->Next;
        }
        MLogTask((PID_RXSDU_CTX0_CONTROL_UPDATE_TA_CQI + mlogPid), RESOURCE_LARM, tt, GET_TICKS);

        tt = GET_TICKS;
        RxControlSduSyncCtrl[RxSduProcCtx] |= PUCCH_SDU_ADDED;
        SendList(FirstPucch->pListRxEnd, PUCCH, RxSduProcCtx);

        tt2 = GET_TICKS;
        LteBsPhyStatsTimer(pLteBsPhyStats->timerStats.ULSDUPucchHarqProcessing, FirstPucch->t_arm, tt2);

        MLogTask((PID_RXSDU_CTX0_CONTROL_SEND_LIST + mlogPid), RESOURCE_LARM, tt, tt2);
    }

    LteBsRxSchedulePucchSduCleanUp(RxSduProcCtx);

    MLogTask((PID_RXSDU_CTX0_UPDATE_TA_CQI + mlogPid), RESOURCE_LARM, tTotal, GET_TICKS);
    return;
}

void RipRssiMeasDone(TCB* pTCB, void* param)
{
    U32 tTotal, tt, *pContext;
    PULRXINPUTPARAMS pUlRxInputParams;
    U32 *pRxRipMeasBuf, *pRxThermalMeasBuf;
    U32 ripEnabled, thermalNoiseEnabled;//, rssiEnabled;
    UINT32 RxSduProcCtx, mlogPid;
    U8  sf;
    U16 sfn;

    U32 outMsgSize;

    tTotal = GET_TICKS;

    pContext            = pTCB->ContextPtr;
    pUlRxInputParams    = (PULRXINPUTPARAMS)pContext[2];
    RxSduProcCtx        = pContext[3];
    sf                  = (U8)pContext[4];
    sfn                 = (U8)pContext[5];

    ripEnabled          = pUlRxInputParams->rxIntMeasFlag;
    //rssiEnabled         = pUlRxInputParams->rssiMeasFlag;
    thermalNoiseEnabled = pUlRxInputParams->rxTherNoiseFlag;

    pRxRipMeasBuf       = (U32*)pRxRipMeas[RxSduProcCtx];
    pRxThermalMeasBuf   = (U32*)pRxThermalMeas[RxSduProcCtx];
    mlogPid             = RxSduProcCtx*10;

    if (ripEnabled && pRxRipMeasBuf)
    {
        tt = GET_TICKS;
        LteBsRxGenerateRipIndMsg(pUlRxInputParams->RipPerRB_dB, pUlRxInputParams->Nul_rb, sf, sfn, pRxRipMeasBuf, &outMsgSize);
        pRxRipMeas[RxSduProcCtx] = NULL;
        MLogTask(PID_RXMEAS_CTX0_RIP+mlogPid, RESOURCE_LARM, tt, GET_TICKS);
    }

    if (thermalNoiseEnabled && pRxThermalMeasBuf)
    {
        tt = GET_TICKS;
        LteBsRxGenerateThermalNoiseIndMsg(pUlRxInputParams->ThermalNoisePwr, pUlRxInputParams->unusedRB, sf, sfn, pRxThermalMeasBuf, &outMsgSize);
        pRxThermalMeas[RxSduProcCtx] = NULL;
        MLogTask(PID_RXMEAS_CTX0_THERMAL_NOISE+mlogPid, RESOURCE_LARM, tt, GET_TICKS);
    }

    MLogTask((PID_RXMEAS_CTX0_MEASUREMENTS + mlogPid), RESOURCE_LARM, tTotal, GET_TICKS);
    return;
}

U32 LteBsRxSduProcessingRT(U32 RxSduCtx, U32 startTime)
{
    PTCB                        pTcbMuxCtrl;
    PULRXFECPARAMS              pFecParams;
    PLTE_PHY_UL_SDU_DESC        FirstPusch = NULL, FirstMuxCtrlCqiRi = NULL, FirstMuxCtrlAckNack = NULL;
    PLTE_PHY_UL_SDU_DESC        NextPusch;
    PLTE_PHY_UL_PUSCH_SDU_DESC  pSduPuschProc;
    PULRXINPUTPARAMS            pUlRxInputParams;
    U32 tt = GET_TICKS, mlogPid = RxSduCtx*30;

    FirstPusch          = pRxSduFirstPusch[RxSduCtx];
    FirstMuxCtrlCqiRi   = pRxSduFirstMuxCtrlCqiRi[RxSduCtx];
    FirstMuxCtrlAckNack = pRxSduFirstMuxCtrlAckNack[RxSduCtx];

    if(FirstPusch == NULL)
    {
        MLogTask((PID_RXSDU_CTX0_DATA_MISSED_DISPATCH_FEC + mlogPid), RESOURCE_LARM, tt, GET_TICKS);
        return 0;
    }

    pFecParams = FirstPusch->pUlRxFecParams;
    pTcbMuxCtrl = FirstPusch->pTcbMuxCtrl;

    if (pTcbMuxCtrl && pFecParams->muxPUSCHCqiRi)
    {
        FirstMuxCtrlCqiRi->t_total = startTime;
        FirstMuxCtrlCqiRi->t_arm = tt;
        if(!ceva_call1_cb(MuxControlCqiRiDone, FirstMuxCtrlCqiRi, ceva_muxCntlRI_CQI_Dec_task_wrap, (UINT32)pTcbMuxCtrl))
        {
            stop_printf("Failed to start muxCntlRI_CQI_Dec_task_wrap function on Ceva\r\n");
            return 0;
        }
    }

    if(gTwoRxMuxControlLists)
    {
        if(pFecParams->muxPUSCHAckNack)
        {
            FirstMuxCtrlAckNack->t_total = startTime;
            MuxControlAckNackReport(FirstMuxCtrlAckNack);
        }
    }
    else
    {
        if(pFecParams->muxPUSCHCqiRi == 0 && pFecParams->muxPUSCHAckNack)
        {
            FirstMuxCtrlCqiRi->t_total = startTime;
            MuxControlAckNackReport(FirstMuxCtrlCqiRi);
        }
    }

    // Dispatch the first PUSCH channel
    pSduPuschProc = FirstPusch->pSduPuschProc;
    pSduPuschProc->t_fec = tt;
    pSduPuschProc->sduState = SDU_FEC_UL_DONE;

    if (pSduPuschProc->pFecUlStart == NULL)
    {
        MLogTask((PID_RXSDU_CTX0_DATA_DISPATCH_FEC + mlogPid), RESOURCE_LARM, tt, GET_TICKS);
        stop_printf("\r\nCRAZY\r\n FEC = NULL. Ctx: %d\r\n", RxSduCtx);
        return 0;
    }

    FecUlRunQ(pSduPuschProc->pFecUlStart, TIME_RX_FEC);

    // Measurements RSSI
    pUlRxInputParams = FirstPusch->pUlRxInputParams;
    NextPusch = FirstPusch;
    while(NextPusch)
    {
        if (pUlRxInputParams->rssiMeasFlag)
            NextPusch->Rssi_dB = pUlRxInputParams->Rssi_dB[NextPusch->chanIndex];
        else
            NextPusch->Rssi_dB = DEFAULT_RSSI;

        NextPusch = NextPusch->Next;
    }

    pSduPuschProc->t_total = startTime;

    MLogTask((PID_RXSDU_CTX0_DATA_DISPATCH_FEC + mlogPid), RESOURCE_LARM, tt, GET_TICKS);

    return 0;
}

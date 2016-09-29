//-------------------------------------------------------------------------------------------
/** @file LteBsRxSduPreprocThread.c
 *
 * @brief Implementation of Rx FEC decoding tasks preparation thread for the LTE BS
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.65 $
 *
 * COPYRIGHT&copy; 2009-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#include "ltephydef.h"
#include "Debug.h"
#include "prototypes.h"
#include "supervisor.h"
#include "heaps.h"
#include "apimgr.h"
#include "larmmsgid.h"
#include "LteBsRxSduPreproc.h"
#include "LteBsRxConfig.h"
#include "LteBsRxDataSdu.h"
#include "ltertconfig.h"
#include "ltemac.h"
#include "ext_phy.h"
#include "lte_entry.h"
#include "mlog.h"
#include "rex.h"
#include "ceva_proc.h"

#define CEVA_PRE_PROC 1

U32 LteBsRxSduArmPreProcessingRT(U32 RxSduCtx, PLTEPHYDESC pPhy)
{
    U32                         RxSduCtxLocal = RxSduCtx;
    PBS_RX                      pBsRx;
    PULRXINPUTPARAMS            pUlRxInputParams;
    PULRXSDUPARAMS              pUlRxSduParams;
    PULRXFECPARAMS              pFecParams;
    PLTE_PHY_UL_SHARED_PARAMS   pUlShareParams = (PLTE_PHY_UL_SHARED_PARAMS)&UlSharedParams[RxSduCtxLocal];
    PLTE_PHY_UL_PUSCH_SDU_DESC  pSduPuschProc = (PLTE_PHY_UL_PUSCH_SDU_DESC) &SduPuschDesc[RxSduCtxLocal];
    PLTE_PHY_UL_SDU_DESC        Curr = NULL, TailPusch = NULL, TailPucch = NULL, TailAckNackMuxCtrl = NULL, TailCqiRiMuxCtrl = NULL;
    PMAC2PHY_QUEUE_EL           pListElement;
    U8*                         pListRxEnd = NULL;
    PTCB                        pTcbMuxCtrl;

    PRXSDUIND pRxSduInd;
    PRXSTATUSIND pRxStatusInd;
    PRX_CQIRIHI_STATUS_EVENT pRxCqiRiHiStatusEvent;

    U32* pOutMsgRxStatusCqiRi;
    U32  OutMsgSizeRxStatusCqiRi;
    U32* pOutMsgRxStatusHi;
    U32  OutMsgSizeRxStatusHi;
    U32  rxMeasSize;

    U16 numChan, nc;
    U32 nctmp, currTti;
    U32* numBytes;
    UINT32 puschListPresent = 0, pucchListPresent = 0;

    pBsRx = pPhy->pBsRx;

    pUlRxInputParams            = pBsRx->pUlRxInputParams;
    pUlRxSduParams              = pBsRx->pUlRxSduParams;

    pFecParams                  = pBsRx->pUlRxFecParams[pBsRx->RxFecParam_Context];
    pBsRx->RxFecParam_Context   = 1 - pBsRx->RxFecParam_Context;

    if (pBsRx->MuxControlTcbCtx == 0)
        pTcbMuxCtrl             = pBsRx->pMuxCtrlTcbCtx1;
    else
        pTcbMuxCtrl             = pBsRx->pMuxCtrlTcbCtx2;

    pBsRx->MuxControlTcbCtx     = 1 - pBsRx->MuxControlTcbCtx;

    _ASSERT_PTR(pUlRxInputParams);
    _ASSERT_PTR(pFecParams);

    numChan                     = pFecParams->numChan;
    currTti                     = (pPhy->rxFrameNumber * 10) + pBsRx->subframeNumber;

    if (pRxSduFirstPusch[RxSduCtxLocal] ||
        pRxSduFirstPucch[RxSduCtxLocal] ||
        pRxSduFirstMuxCtrlAckNack[RxSduCtxLocal] ||
        pRxSduFirstMuxCtrlCqiRi[RxSduCtxLocal])
    {
        stop_printf("Previous List Not Completed. Ctx: %d. 0:[%x %x %x %x] 1:[%x %x %x %x] 2:[%x %x %x %x] 3:[%x %x %x %x]\r\n",
            RxSduCtxLocal,
            pRxSduFirstPusch[0], pRxSduFirstPucch[0], pRxSduFirstMuxCtrlAckNack[0], pRxSduFirstMuxCtrlCqiRi[0],
            pRxSduFirstPusch[1], pRxSduFirstPucch[1], pRxSduFirstMuxCtrlAckNack[1], pRxSduFirstMuxCtrlCqiRi[1],
            pRxSduFirstPusch[2], pRxSduFirstPucch[2], pRxSduFirstMuxCtrlAckNack[2], pRxSduFirstMuxCtrlCqiRi[2],
            pRxSduFirstPusch[3], pRxSduFirstPucch[3], pRxSduFirstMuxCtrlAckNack[3], pRxSduFirstMuxCtrlCqiRi[3]);
        return 0;
    }

    RxControlSduSyncCtrl[RxSduCtxLocal] = 0;

    if (numChan)
    {
        pListRxEnd = LteRxSduBuildRxEndMessage(pPhy->rxFrameNumber, pBsRx->subframeNumber);
        pRxSduRxEnd[RxSduCtxLocal] = (PMAC2PHY_QUEUE_EL)pListRxEnd;
    }

    numBytes = pFecParams->numBytes;

    for (nctmp = 0; nctmp < numChan; nctmp++)
    {
        nc = pUlShareParams->IndexMap[nctmp];
        if (pUlRxInputParams->physChanType[nc] == PUSCH)
        {
            puschListPresent                    = 1;

            // Alloc SDU desc from the pool
            Curr                                = (PLTE_PHY_UL_SDU_DESC) pRxSduDesc[RxSduCtxLocal][nc];
            _ASSERT_PTR(Curr);
            memset(Curr, 0, sizeof(LTE_PHY_UL_SDU_DESC));
            Curr->sduType                       = PUSCH;
            Curr->currTti                       = currTti;
            Curr->pTcbMuxCtrl                   = pTcbMuxCtrl;
            Curr->RxSduProcCtx                  = RxSduCtxLocal;

            // Allocate Memory for RXSDUs (List Element)
            pRxSduInd                           = (PRXSDUIND)pUlShareParams->mdmaOut[nc];
            pRxSduInd->phyEntityId              = 0;
            pRxSduInd->msgType                  = PHY_RXSDU_IND;
            pRxSduInd->numBitsRx                = numBytes[nc] * 8;
            pRxSduInd->channelId                = nc;
            pRxSduInd->frameNum                 = pPhy->rxFrameNumber;
            pRxSduInd->subFrameNum              = pBsRx->subframeNumber;
            pRxSduInd->chanType                 = PUSCH;
            pRxSduInd->listType                 = UL_LIST_TYPE_PUSCH;

            // Allocate List Elements head
            pListElement = (PMAC2PHY_QUEUE_EL) Phy2MacIfAllocBuffer(sizeof(MAC2PHY_QUEUE_EL));
            pListElement->Next = NULL;
            pListElement->frameNumber           = pPhy->rxFrameNumber;
            pListElement->subframeNumber        = pBsRx->subframeNumber;
            pListElement->MessageType           = PHY_RXSDU_IND;
            pListElement->MessagePtr            = (U8*)pRxSduInd;
            pListElement->MessageLen            = numBytes[nc] + sizeof(RXSDUIND) - 4;        // remove 4 since output is directly written to pRxSdu

            if(pRxSduFirstPusch[RxSduCtxLocal] == NULL)
            {
                pSduPuschProc->sduState         = SDU_NEW_UL;
                Curr->pSduPuschProc             = pSduPuschProc;
                pSduPuschProc->pFirst           = (U32*) Curr;
                pRxSduFirstPusch[RxSduCtxLocal] = Curr;
                TailPusch                       = Curr;
            }
            else
            {
                TailPusch->Next                 = Curr;
                TailPusch                       = Curr;
            }

            Curr->pUlRxFecParams                = pFecParams;
            Curr->pUlRxSduParams                = pUlRxSduParams;
            Curr->pUlRxInputParams              = pUlRxInputParams;
            Curr->chanIndex                     = nc;

            // Fill in SDU Descrriptor
            //-----------------
            Curr->subframeNumber                = pBsRx->subframeNumber;
            Curr->rxFrameNumber                 = pPhy->rxFrameNumber;
            Curr->numBitsRx                     = numBytes[nc] * 8;
            Curr->cqiPmiConf                    = 1;                      // Good Confidence (default)

            Curr->out                           = pUlShareParams->fecOut[nc];
            Curr->outp                          = numBytes[nc];
            Curr->CrcOut                        = (U8*) &pRxSduInd->pRxSdu;
            Curr->pListElement                  = pListElement;
            Curr->pListRxEnd                    = pListRxEnd;

            if (gTwoRxMuxControlLists)
            {
                // Message for ACK / NACK Multiplexed in PUSCH
                if(pFecParams->numbitsACK[nc])
                {
                    U32 NumBytesStatus                  = (pFecParams->nACK[nc] + 7) >> 3;
                    pucchListPresent                    = 1;

                    OutMsgSizeRxStatusHi                = sizeof(RX_CQIRIHI_STATUS_EVENT) + sizeof(RXSTATUSIND) + NumBytesStatus;
                    pOutMsgRxStatusHi                   = (U32*) Phy2MacIfAllocBuffer (OutMsgSizeRxStatusHi);

                    pRxStatusInd                        = (PRXSTATUSIND) pOutMsgRxStatusHi;
                    pRxStatusInd->phyEntityId           = (U8) 0;
                    pRxStatusInd->msgType               = PHY_RXSTATUS_IND;
                    pRxStatusInd->frameNum              = pPhy->rxFrameNumber;
                    pRxStatusInd->subFrameNum           = pBsRx->subframeNumber;
                    pRxStatusInd->statusType            = CQIRIHI_RESULT;
                    pRxStatusInd->status                = SUCCESS;
                    pRxStatusInd->eventNumber           = 1;
                    pRxStatusInd->listType              = UL_LIST_TYPE_PUCCH;

                    // Now fill the status info based on the channel info
                    pRxCqiRiHiStatusEvent               = (PRX_CQIRIHI_STATUS_EVENT)&pRxStatusInd->pEventList;
                    pRxCqiRiHiStatusEvent->status_len   = 0;
                    pRxCqiRiHiStatusEvent->chanId       = nc;
                    pRxCqiRiHiStatusEvent->cqi_pmi_pt   = 0;
                    pRxCqiRiHiStatusEvent->ri_pt        = 0;
                    pRxCqiRiHiStatusEvent->harq_pt      = pFecParams->nACK[nc];

                    // Allocate List Elements head
                    pListElement                        = (PMAC2PHY_QUEUE_EL) Phy2MacIfAllocBuffer(sizeof(MAC2PHY_QUEUE_EL));
                    pListElement->Next                  = NULL;
                    pListElement->frameNumber           = pPhy->rxFrameNumber;
                    pListElement->subframeNumber        = pBsRx->subframeNumber;
                    pListElement->MessageType           = PHY_RXSTATUS_IND;
                    pListElement->MessagePtr            = (U8*)pOutMsgRxStatusHi;
                    pListElement->MessageLen            = OutMsgSizeRxStatusHi;
                    Curr->pListElementHi                = pListElement;

                    if(pRxSduFirstMuxCtrlAckNack[RxSduCtxLocal] == NULL)
                    {
                        pRxSduFirstMuxCtrlAckNack[RxSduCtxLocal]    = Curr;
                        TailAckNackMuxCtrl                          = Curr;
                        TailAckNackMuxCtrl->NextHiMuxCtrl           = NULL;
                    }
                    else
                    {
                        TailAckNackMuxCtrl->NextHiMuxCtrl           = Curr;
                        TailAckNackMuxCtrl                          = Curr;
                        TailAckNackMuxCtrl->NextHiMuxCtrl           = NULL;
                    }

                }

                // Message for ACK / NACK Multiplexed in PUSCH
                if(pFecParams->numbitsCQI[nc] || pFecParams->numbitsRI[nc])
                {
                    U32 NumBytesStatus                  = 0;
                    NumBytesStatus                     += (pFecParams->nr1CQI[nc] + pFecParams->nrg1CQI[nc] + 7) >> 3;
                    NumBytesStatus                     += (pFecParams->nRI[nc] + 7) >> 3;

                    OutMsgSizeRxStatusCqiRi             = sizeof(RX_CQIRIHI_STATUS_EVENT) + sizeof(RXSTATUSIND) + NumBytesStatus;
                    pOutMsgRxStatusCqiRi                = (U32*) Phy2MacIfAllocBuffer (OutMsgSizeRxStatusCqiRi);

                    pRxStatusInd                        = (PRXSTATUSIND) pOutMsgRxStatusCqiRi;
                    pRxStatusInd->phyEntityId           = (U8) 0;
                    pRxStatusInd->msgType               = PHY_RXSTATUS_IND;
                    pRxStatusInd->frameNum              = pPhy->rxFrameNumber;
                    pRxStatusInd->subFrameNum           = pBsRx->subframeNumber;
                    pRxStatusInd->statusType            = CQIRIHI_RESULT;
                    pRxStatusInd->status                = SUCCESS;
                    pRxStatusInd->eventNumber           = 1;
                    pRxStatusInd->listType              = UL_LIST_TYPE_OTHER;

                    // Now fill the status info based on the channel info
                    pRxCqiRiHiStatusEvent               = (PRX_CQIRIHI_STATUS_EVENT)&pRxStatusInd->pEventList;
                    pRxCqiRiHiStatusEvent->status_len   = 0;
                    pRxCqiRiHiStatusEvent->chanId       = nc;
                    pRxCqiRiHiStatusEvent->cqi_pmi_pt   = pFecParams->nr1CQI[nc] + pFecParams->nrg1CQI[nc];
                    pRxCqiRiHiStatusEvent->ri_pt        = pFecParams->nRI[nc];
                    pRxCqiRiHiStatusEvent->harq_pt      = 0;

                    // Allocate List Elements head
                    pListElement                        = (PMAC2PHY_QUEUE_EL) Phy2MacIfAllocBuffer(sizeof(MAC2PHY_QUEUE_EL));
                    pListElement->Next                  = NULL;
                    pListElement->frameNumber           = pPhy->rxFrameNumber;
                    pListElement->subframeNumber        = pBsRx->subframeNumber;
                    pListElement->MessageType           = PHY_RXSTATUS_IND;
                    pListElement->MessagePtr            = (U8*)pOutMsgRxStatusCqiRi;
                    pListElement->MessageLen            = OutMsgSizeRxStatusCqiRi;
                    Curr->pListElementCqiRi             = pListElement;

                    if(pRxSduFirstMuxCtrlCqiRi[RxSduCtxLocal] == NULL)
                    {
                        pRxSduFirstMuxCtrlCqiRi[RxSduCtxLocal]  = Curr;
                        TailCqiRiMuxCtrl                        = Curr;
                        TailCqiRiMuxCtrl->NextCqiRiMuxCtrl      = NULL;
                    }
                    else
                    {
                        TailCqiRiMuxCtrl->NextCqiRiMuxCtrl      = Curr;
                        TailCqiRiMuxCtrl                        = Curr;
                        TailCqiRiMuxCtrl->NextCqiRiMuxCtrl      = NULL;
                    }
                }
            }
            else
            {
                // Message for CQI, RI and ACK / NACK Multiplexed in PUSCH. All bunched into 1 message
                if(pFecParams->numbitsCQI[nc] || pFecParams->numbitsACK[nc] || pFecParams->numbitsRI[nc])
                {
                    U32 NumBytesStatus                  = 0;
                    pucchListPresent                    = 1;
                    NumBytesStatus                     += (pFecParams->nr1CQI[nc] + pFecParams->nrg1CQI[nc] + 7) >> 3;
                    NumBytesStatus                     += (pFecParams->nRI[nc] + 7) >> 3;
                    NumBytesStatus                     += (pFecParams->nACK[nc] + 7) >> 3;

                    OutMsgSizeRxStatusCqiRi             = sizeof(RX_CQIRIHI_STATUS_EVENT) + sizeof(RXSTATUSIND) + NumBytesStatus;
                    pOutMsgRxStatusCqiRi                = (U32*) Phy2MacIfAllocBuffer (OutMsgSizeRxStatusCqiRi);

                    pRxStatusInd                        = (PRXSTATUSIND) pOutMsgRxStatusCqiRi;
                    pRxStatusInd->phyEntityId           = (U8) 0;
                    pRxStatusInd->msgType               = PHY_RXSTATUS_IND;
                    pRxStatusInd->frameNum              = pPhy->rxFrameNumber;
                    pRxStatusInd->subFrameNum           = pBsRx->subframeNumber;
                    pRxStatusInd->statusType            = CQIRIHI_RESULT;
                    pRxStatusInd->status                = SUCCESS;
                    pRxStatusInd->eventNumber           = 1;
                    pRxStatusInd->listType              = UL_LIST_TYPE_PUCCH;

                    // Now fill the status info based on the channel info
                    pRxCqiRiHiStatusEvent               = (PRX_CQIRIHI_STATUS_EVENT)&pRxStatusInd->pEventList;
                    pRxCqiRiHiStatusEvent->status_len   = 0;
                    pRxCqiRiHiStatusEvent->chanId       = nc;
                    pRxCqiRiHiStatusEvent->cqi_pmi_pt   = pFecParams->nr1CQI[nc] + pFecParams->nrg1CQI[nc];
                    pRxCqiRiHiStatusEvent->ri_pt        = pFecParams->nRI[nc];
                    pRxCqiRiHiStatusEvent->harq_pt      = pFecParams->nACK[nc];

                    // Allocate List Elements head
                    pListElement                        = (PMAC2PHY_QUEUE_EL) Phy2MacIfAllocBuffer(sizeof(MAC2PHY_QUEUE_EL));
                    pListElement->Next                  = NULL;
                    pListElement->frameNumber           = pPhy->rxFrameNumber;
                    pListElement->subframeNumber        = pBsRx->subframeNumber;
                    pListElement->MessageType           = PHY_RXSTATUS_IND;
                    pListElement->MessagePtr            = (U8*)pOutMsgRxStatusCqiRi;
                    pListElement->MessageLen            = OutMsgSizeRxStatusCqiRi;
                    Curr->pListElementCqiRi             = pListElement;
                    Curr->pListElementHi                = pListElement;

                    if(pRxSduFirstMuxCtrlCqiRi[RxSduCtxLocal] == NULL)
                    {
                        pRxSduFirstMuxCtrlCqiRi[RxSduCtxLocal]  = Curr;
                        TailCqiRiMuxCtrl                        = Curr;
                        TailCqiRiMuxCtrl->NextCqiRiMuxCtrl      = NULL;
                        TailCqiRiMuxCtrl->NextHiMuxCtrl         = NULL;
                    }
                    else
                    {
                        TailCqiRiMuxCtrl->NextCqiRiMuxCtrl      = Curr;
                        TailCqiRiMuxCtrl->NextHiMuxCtrl         = Curr;
                        TailCqiRiMuxCtrl                        = Curr;
                        TailCqiRiMuxCtrl->NextCqiRiMuxCtrl      = NULL;
                        TailCqiRiMuxCtrl->NextHiMuxCtrl         = NULL;
                    }
                }
            }
        }
        else
        {
            // Alloc SDU desc from the pool
            pucchListPresent                     = 1;
            Curr                                 = (PLTE_PHY_UL_SDU_DESC) pRxSduDesc[RxSduCtxLocal][nc];
            _ASSERT_PTR(Curr);
            memset(Curr, 0, sizeof(LTE_PHY_UL_SDU_DESC));
            Curr->sduType                        = PUCCH;
            Curr->currTti                        = currTti;
            Curr->cqiPmiConf                     = 1;                                // Good Confidence (default)
            Curr->RxSduProcCtx                   = RxSduCtxLocal;

            // Allocate List Elements head
            pListElement                         = (PMAC2PHY_QUEUE_EL) AllocPUCCHSdu();
            pListElement->Next                   = NULL;
            pListElement->frameNumber            = pPhy->rxFrameNumber;
            pListElement->subframeNumber         = pBsRx->subframeNumber;
            pListElement->MessageType            = PHY_RXSDU_IND;
            pListElement->MessageLen             = (2 * 8) + sizeof(RXSDUIND) - 4;        // remove 4 since output is directly written to pRxSdu

            // Allocate Memory for RXSDUs
            pRxSduInd                            = (PRXSDUIND) pListElement->MessagePtr;
            pRxSduInd->phyEntityId               = 0;
            pRxSduInd->msgType                   = PHY_RXSDU_IND;
            pRxSduInd->channelId                 = nc;
            pRxSduInd->numBitsRx                 = 2 * 8;
            pRxSduInd->frameNum                  = pPhy->rxFrameNumber;
            pRxSduInd->subFrameNum               = pBsRx->subframeNumber;
            pRxSduInd->chanType                  = PUCCH;
            pRxSduInd->pucchType                 = pFecParams->pucchFormat[nc];
            pRxSduInd->pucchDetected             = 0;
            pRxSduInd->listType                  = UL_LIST_TYPE_PUCCH;

            if(pRxSduFirstPucch[RxSduCtxLocal] == NULL)
            {
                pRxSduFirstPucch[RxSduCtxLocal] = Curr;
                TailPucch                       = Curr;
            }
            else
            {
                TailPucch->Next                 = Curr;
                TailPucch                       = Curr;
            }
            Curr->chanIndex                     = nc;

            Curr->pUlRxFecParams                = pFecParams;
            Curr->pUlRxSduParams                = pUlRxSduParams;
            Curr->pUlRxInputParams              = pUlRxInputParams;
            Curr->simSRHarq                     = pUlRxInputParams->simSRHarq[nc];

            Curr->out                           = (U8*) &pRxSduInd->pRxSdu;

            Curr->subframeNumber                = pBsRx->subframeNumber;
            Curr->rxFrameNumber                 = pPhy->rxFrameNumber;

            Curr->numBitsRx                     = 2 * 8; // 13 bits for Format 2
            Curr->pListElement                  = pListElement;
            Curr->pListRxEnd                    = pListRxEnd;

        } // else PUCCH
    }

    // Measurements
    //-------------

    // RSSI
    if(pUlRxInputParams->rssiMeasFlag)
    {
        rxMeasSize = sizeof(RXSTATUSIND) + sizeof(RX_RIP_MEAS_STATUS_EVENT) + ((pUlRxInputParams->Nul_rb>>1)*sizeof(RX_RIP_MEAS_PER_BLOCK));
        pRxRipMeas[RxSduCtx] = (U32*)Phy2MacIfAllocBuffer(rxMeasSize);
    }

    // Thermal Noise
    if(pUlRxInputParams->rxTherNoiseFlag)
    {
        rxMeasSize = sizeof(RXSTATUSIND) + sizeof(RX_THERMAL_NOISE_MEAS_STATUS_EVENT);
        pRxThermalMeas[RxSduCtx] = (U32*)Phy2MacIfAllocBuffer(rxMeasSize);
    }

    if (puschListPresent)
        gRxSduListState[RxSduCtx] |= RXSDU_LIST_STATE__PUSCH_PRESENT;
    if (pucchListPresent)
        gRxSduListState[RxSduCtx] |= RXSDU_LIST_STATE__PUCCH_PRESENT;

    return 0;
}

void LteBsRxSduCevaPreProcessingRT_Done(void* ptr, UINT32 ceva_ret)
{
    U32 t1 = GET_TICKS;
    U32 RxSduCtx = (U32)ptr;
    PLTE_PHY_UL_SHARED_PARAMS pUlShareParams = (PLTE_PHY_UL_SHARED_PARAMS)&UlSharedParams[RxSduCtx];
    PLTE_PHY_UL_PUSCH_SDU_DESC pSduPuschProc = (PLTE_PHY_UL_PUSCH_SDU_DESC) &SduPuschDesc[RxSduCtx];

    if (ceva_ret != 0)
    {
        stop_printf("Something is wrong with Ceva function LteBsRxSduCevaPreProcessingRT retCode: 0x%x\r\n", ceva_ret);
        return;
    }

    if (pUlShareParams->pFecUlStart)
    {
        FecUlSetLastDesc((PFEC_UL_FDESC)pUlShareParams->pFecUlStart, LtePhySduDecMdmaFecCbDone, pSduPuschProc);
    }

    pSduPuschProc->pFecUlStart = (PFEC_UL_FDESC)pUlShareParams->pFecUlStart;
    pSduPuschProc->pMdmaStart  = (PMDMA_FDESC)pUlShareParams->pMdmaStart;

    MLogTask((PID_RXSDU_CTX0_PREPROC_PART2 + (RxSduCtx*30)), RESOURCE_LARM, t1, GET_TICKS);

    return;
}

U32 LteBsRxSduPreProcessingRT(U32 RxSduCtx, PLTEPHYDESC pPhy)
{
    PLTE_PHY_UL_SHARED_PARAMS pUlShareParams = (PLTE_PHY_UL_SHARED_PARAMS)&UlSharedParams[RxSduCtx];
    PLTE_PHY_UL_PUSCH_SDU_DESC  pSduPuschProc = (PLTE_PHY_UL_PUSCH_SDU_DESC) &SduPuschDesc[RxSduCtx];
    PULRXFECPARAMS pFecParams;
    PULRXINPUTPARAMS pUlRxInputParams;
    U16 numPUSCH;
    U16 numChan;
    U32 nctmp, tmp, nc;
    U32* numBytes;
    U32 numswitch;
    UINT32 t1, t2, mlogPid = RxSduCtx*30;

#ifdef CEVA_PRE_PROC
    U32 hPreProc;
#endif

    t1 = GET_TICKS;

    memset(pSduPuschProc, 0, sizeof(LTE_PHY_UL_PUSCH_SDU_DESC));

    pFecParams       = pPhy->pBsRx->pUlRxFecParams[pPhy->pBsRx->RxFecParam_Context];
    pUlRxInputParams = pPhy->pBsRx->pUlRxInputParams;

    numBytes = pFecParams->numBytes;
    numChan  = pFecParams->numChan;
    numPUSCH = pUlRxInputParams->numPUSCH;

    // FEC UL workaround, change order of FEC UL loading (numbytes increasing)
    for(nctmp = 0; nctmp < numChan; nctmp++)
    {
        pUlShareParams->IndexMap[nctmp] = nctmp;
    }

    if(numPUSCH > 1)
    {
        do
        {
            numswitch = 0;
            for(nctmp = 1; nctmp < numPUSCH; nctmp++)
            {
                if(numBytes[pUlShareParams->IndexMap[nctmp-1]] > numBytes[pUlShareParams->IndexMap[nctmp]])
                {
                    tmp = pUlShareParams->IndexMap[nctmp];
                    pUlShareParams->IndexMap[nctmp] = pUlShareParams->IndexMap[nctmp-1];
                    pUlShareParams->IndexMap[nctmp-1] = tmp;
                    numswitch++;
                }
            }
        }while(numswitch != 0);
    }

    // Allocate buffers for PUSCH
    for (nctmp = 0; nctmp < numChan; nctmp++)
    {
        nc = pUlShareParams->IndexMap[nctmp];
        if (pUlRxInputParams->physChanType[nc] == PUSCH)
        {
            // Allocate Memory for RXSDUs (List Element)
            pUlShareParams->size[nc]    = numBytes[nc] + 32 + 8 + sizeof(RXSDUIND);
            pUlShareParams->fecOut[nc]  = (U8*) MemAlloc(numBytes[nc] + 8, IRAM_MEM, 0);
            _ASSERT_PTR(pUlShareParams->fecOut[nc]);
            pUlShareParams->mdmaOut[nc] = (U8*) Phy2MacIfAllocNcNbBuffer (pUlShareParams->size[nc]);
            _ASSERT_PTR(pUlShareParams->mdmaOut[nc]);
        }
    }

    t2 = GET_TICKS;
    MLogTask((PID_RXSDU_CTX0_PREPROC_PART1 + mlogPid), RESOURCE_LARM, t1, t2);

    while(rxVectorProcDone == 0)
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
    MLogTask(PID_WAIT_RXVECTORPROC_COMPLETE, RESOURCE_LARM, t2, GET_TICKS);

    // Store the Api State for this Subframe
    gMissedApiTable[pPhy->pharq->harq_prc] = pUlRxInputParams->isDummyApiRunning;

    if (numChan)
    {
#ifndef CEVA_PRE_PROC
        LteBsRxSduCevaPreProcessingRT((U32)pUlRxInputParams, (U32)pFecParams, (U32)pUlShareParams, (U32)pPhy->pharq,
            (U32) Table_FEC_CodeBlock_C, (U32) Table_FEC_CodeBlock_K, (U32) Table_FEC_RateMatching_k0);
        t1 = GET_TICKS;
        MLogTask((PID_RXSDU_CTX0_PREPROC_PART2 + mlogPid), RESOURCE_LARM, t2, t1);
        LteBsRxSduCevaPreProcessingRT_Done((void*)RxSduCtx, 0);

#else   // CEVA_PRE_PROC
        hPreProc = ceva_call7_cb(LteBsRxSduCevaPreProcessingRT_Done, (void*)RxSduCtx, ceva_LteBsRxSduCevaPreProcessingRT,
                            (U32)pUlRxInputParams, (U32)pFecParams, (U32)pUlShareParams, (U32)pPhy->pharq,
                            (U32) Table_FEC_CodeBlock_C, (U32) Table_FEC_CodeBlock_K, (U32) Table_FEC_RateMatching_k0);
        if(!hPreProc)
        {
            stop_printf("Failed to start UL PreProc function on Ceva\r\n");
        }

#endif  //CEVA_PRE_PROC

        t1 = GET_TICKS;
        LteBsRxSduArmPreProcessingRT(RxSduCtx, pPhy);
        t2 = GET_TICKS;
        MLogTask((PID_RXSDU_CTX0_PREPROC_PART3 + mlogPid), RESOURCE_LARM, t1, t2);
    }
    else
    {
        pPhy->pBsRx->RxFecParam_Context   = 1 - pPhy->pBsRx->RxFecParam_Context;
        pPhy->pBsRx->MuxControlTcbCtx     = 1 - pPhy->pBsRx->MuxControlTcbCtx;
    }

    return 0;
}


static U32 allocPucchSduPush = 0, allocPucchSduPull = 0;
static U32 allocPucchSduBuf[ALLOC_PUCCH_SDU_DEPTH] = {NULL};

U32 AllocPUCCHSdu(void)
{
    U32 retSdu = NULL;
    PMAC2PHY_QUEUE_EL pListElement;
    PRXSDUIND pRxSduInd;

    if (allocPucchSduPush != allocPucchSduPull)
    {
        retSdu                      = allocPucchSduBuf[allocPucchSduPull];
        allocPucchSduBuf[allocPucchSduPull] = NULL;
        allocPucchSduPull           = IncrementAllocPucchSduCtx(allocPucchSduPull);
    }
    else
    {
        pRxSduInd                   = (PRXSDUIND) Phy2MacIfAllocBuffer (4 + sizeof(RXSDUIND));
        pListElement                = (PMAC2PHY_QUEUE_EL) Phy2MacIfAllocBuffer(sizeof(MAC2PHY_QUEUE_EL));
        pListElement->MessagePtr    = (U8*)pRxSduInd;
        retSdu                      = (U32) pListElement;
    }

    return retSdu;
}

void PushPUCCHSdu(U32 sduLoc)
{
    allocPucchSduBuf[allocPucchSduPush] = sduLoc;
    allocPucchSduPush = IncrementAllocPucchSduCtx(allocPucchSduPush);

    return;
}


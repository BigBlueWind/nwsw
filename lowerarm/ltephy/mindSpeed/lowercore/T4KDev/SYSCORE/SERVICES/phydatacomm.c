//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Phydatacomm.c
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
//#include "lte_entry.h"
//#include "wimax_entry.h"
#include "appprintf.h"
#include "supervisor.h"
#include "larmmsgid.h"
#include "phydatacomm.h"

#if 0

extern const char LTmrName[];

UINT16 PhyCommIFTx = 0;
UINT16 PhyCommIFRx = 0;

// LTE
//extern LTEAppCtx *pLteCtx;
//extern short* pDacSamples;
extern short* pInSmp;
// WiMAX
//extern PWiMaxAppCtx pWiMaxAppCtxBs;
extern short *saved_iq;

static ReTxCtx ReTxCtxArr[RETX_QUEUE_SIZE] = {0};

static LteCommSmpCtx gLteSendOutSmp[PHYCOMMIF_MAX] = {0};
static LteCommSmpCtx gLteRcvInSmp[PHYCOMMIF_MAX] = {0};
static PUINT8 pEthTxBufAlloc = NULL;
static PUINT8 pEthTxBuf = NULL;

static short* pOutIQSmp = NULL;
static short* pInIQSmp = NULL;

static HANDLE PhyDataTimer = {0};

static LPVOID pGemDrv = NULL;
static UINT8 gProcessGemacRx = 0;

static UINT32 LteGetSamplesSizeByte(PLteCommSmpCtx pCtx)
{
    UINT32 NumSamples;

    switch(pCtx->ChanBW)
    {// to set correct values for all BW modes
        case 0: // 1.4 MHz
            NumSamples = 2304;
            break;
        case 1: // 3.0 MHz
            NumSamples = 4608;
            break;
        case 2: // 5.0 MHz
            NumSamples = 7680;
            break;
        case 3: // 10.0 MHz
            NumSamples = 15360;
            break;
        case 4: // 15.0 MHz
            NumSamples = 23040;
            break;
        case 5: // 20.0 MHz
            NumSamples = 30720;
            break;
        default:
            NumSamples = 15360;
            break;
    }

    return (NumSamples * 2 * sizeof(UINT16) * pCtx->NumAntennas);
}

static void ReTxCtxClear(PReTxCtx pReTxCtx)
{
    if(HANDLE_NOT_NULL(pReTxCtx->ReTxTimeoutThr))
    {
        MxRemoveThread(pReTxCtx->ReTxTimeoutThr);
        pReTxCtx->ReTxTimeoutThr = HNULL;
    }
    if(HANDLE_NOT_NULL(pReTxCtx->ReTxTimerEvent))
    {
        MxRemoveEvent(pReTxCtx->ReTxTimerEvent);
        pReTxCtx->ReTxTimerEvent = HNULL;
    }
    if(HANDLE_NOT_NULL(pReTxCtx->ReTxTimer))
    {
        MxRemoveTimer(pReTxCtx->ReTxTimer);
        pReTxCtx->ReTxTimer = HNULL;
    }
}

static void ReTxCtxInit(PReTxCtx pReTxCtx, TENTRY pThr)
{
    ReTxCtxClear(pReTxCtx);

    MxCreateThread(pThr, NULL, pReTxCtx, 1, &pReTxCtx->ReTxTimeoutThr);
    MxCreateEvent(EVENT_MODE_RESET, NULL, &pReTxCtx->ReTxTimerEvent);
    MxCreateTimer(TIMER_MODE_ONE_TIME, pReTxCtx->ReTxTimeout, NULL,
            pReTxCtx->ReTxTimerEvent, &pReTxCtx->ReTxTimer);
}

static void ClearACtx(PAsyncProcCtx pCtx)
{
    if(HANDLE_NOT_NULL(pCtx->AEvent))
    {
        MxRemoveEvent (pCtx->AEvent);
        pCtx->AEvent = HNULL;
    }
    if(HANDLE_NOT_NULL(pCtx->ATimer))
    {
        MxRemoveTimer (pCtx->ATimer);
        pCtx->ATimer = HNULL;
    }
    if(HANDLE_NOT_NULL(pCtx->AThrComplete))
    {
        MxRemoveThread (pCtx->AThrComplete);
        pCtx->AThrComplete = HNULL;
    }
    if(HANDLE_NOT_NULL(pCtx->AThrTimeout))
    {
        MxRemoveThread (pCtx->AThrTimeout);
        pCtx->AThrTimeout = HNULL;
    }
    if(HANDLE_NOT_NULL(pCtx->AThrMainTx))
    {
        MxRemoveThread (pCtx->AThrMainTx);
        pCtx->AThrMainTx = HNULL;
    }
    if(HANDLE_NOT_NULL(pCtx->AThrMainRx))
    {
        MxRemoveThread (pCtx->AThrMainRx);
        pCtx->AThrMainRx = HNULL;
    }
    if(pCtx->RxCbCreated)
    {
        // restore Rx Cb
        GemDrvPopIrqProc (GemDrvGetCtx (1));
        pCtx->RxCbCreated = 0;
    }
    if(pCtx->ReTxNum)
    {
        UINT32 i=0;
        for(i=0; i<RETX_QUEUE_SIZE; i++)
        {
            ReTxCtxClear(&ReTxCtxArr[i]);
        }
        pCtx->ReTxNum = 0;
    }
}

static void PhyCommResetGemacDMA(MXRC rc)
{
    UINT32 Count = 100000;
    GemCtx * pCtx = (GemCtx *)pGemDrv;

    printf("GEMAC error: %X\n", rc);

    GemDrvDisableTX (pGemDrv);
    GemDrvResetDMA(pGemDrv);
    pCtx->IPRegs->TxStatus = 0xFFFFFFFF;
    while (--Count)
        __nop();
    GemDrvEnableTX (pGemDrv);
    pCtx->IPRegs->NetCtrl |= GEM_NETCTRL_START_TX;
}

static MXRC TxEthPacketProcess(PUINT8 pHdrBuf, UINT8 HdrSize, PUINT8 pBuf, UINT32 Size)
{
    MXRC rc = MX_OK;

    // call GEMAC driver
    if(pHdrBuf)
    {
        HGTxFrm frm = GemDrvAllocTxFrm (pGemDrv, GEM_DMA_TX_CRC, pHdrBuf, HdrSize);
        if (frm == NULL)
        {
            return GEM_RC_TX_ERROR;
        }
        rc = GemDrvAddTxBuf (pGemDrv, frm, pBuf, Size);

        //printf("TxEthPack1: rc_%X\n", rc);

        //_ASSERT_RC(rc);
        if (rc != MX_OK)
            return rc;
    }
    else
    {
        if (GemDrvAllocTxFrm (pGemDrv, GEM_DMA_TX_CRC, pBuf, Size) == NULL)
            return GEM_RC_TX_ERROR;
        //printf("TxEthPack2: pBuf_%X Size_%d\n", pBuf, Size);

    }

    // send just enqueud packet
    rc = GemDrvTxFrm (pGemDrv, 0, 0, 0);
    if(rc ==10)
     {
        printf("To reset DMA\n");
        PhyCommResetGemacDMA(rc);
        GemDrvAllocTxFrm (pGemDrv, GEM_DMA_TX_CRC, pBuf, Size);
        rc = GemDrvTxFrm (pGemDrv, 0, 0, 0);
     }

    //_ASSERT_RC(rc);
    return rc;
}

static MXRC LteSendData2Eth(PUINT8 pBuf, UINT32 Offset, UINT32 nDataSize, UINT8 Tid)
{// sends one portion of data
    // do not perform excess copy, provide lower level with Header and with Data
    PLteCommSmpCtx pCtx = &gLteSendOutSmp[PHYCOMMIF_ETH];
    MXRC rc = MX_OK;
    PUINT8 pHdrBuf = NULL;
    UINT8 HdrSize = 0;

    if(pCtx->TotalHdrSize == 0)
    {// gather 2 layers header first
        pCtx->pTotalHdr = (PUINT8)ROUND((UINT32)(pCtx->TotalHdr), 32);
        pHdrBuf = pCtx->pTotalHdr;

        if(pCtx->L1HdrSize)
        {
            memcpy(pHdrBuf, pCtx->L1Hdr, pCtx->L1HdrSize);
            HdrSize = pCtx->L1HdrSize;
            pCtx->TotalHdrSize += pCtx->L1HdrSize;
        }

        if(pCtx->L2HdrSize)
        {
            if(pCtx->L2HdrMode != L2HDRMODE_NO)
            {
                PApiHeader pApiConf = (PApiHeader) pCtx->L2Hdr;
                PApiHeader pApi;
                PApiParam pParam;
                UINT32 L2HdrSize = 0;

                pApi = ApiCreate(pHdrBuf + HdrSize, pApiConf->Control,
                              pApiConf->SrcInstID,
                              pApiConf->DstInstID,
                              pApiConf->Type,
                              pApiConf->MessageID);

                if(pCtx->L2HdrMode >= L2HDRMODE_MSPD_SN)
                {// overwrite TID
                    pApi->Control &= ~API_TID_MASK;
                    pApi->Control |= ((Tid) << API_TID_SHIFT) & API_TID_MASK;
                    pApi->Control |= API_USE_TID;
                }

                pParam = (ApiParam*) (pApi + 1);
                pParam->ParamID = 4; // PHY_BUF_OFFS_PARAM_ID;
                pParam->ParamLength = sizeof(UINT32);
                pParam->ParamData[0] = Offset;

                ++pParam;
                pParam->ParamID = 5; // PHY_DATA_PARAM_ID;
                pParam->ParamLength = nDataSize + sizeof(UINT32);
                pParam->ParamData[0] = nDataSize;

                L2HdrSize = API_HDR_SIZE + 2*sizeof (ApiParam);
                pApi->Length = L2HdrSize - API_HDR_SIZE + nDataSize;

                HdrSize += L2HdrSize;

                memcpy(pHdrBuf + pCtx->TotalHdrSize, pApi, L2HdrSize);
                pCtx->TotalHdrSize += L2HdrSize;

            }
        }

            printf("LteSendData2Eth: have hdr (size=%d)\n", pCtx->TotalHdrSize);
    }
    else
    {// use collected header
        pHdrBuf = pCtx->pTotalHdr;
        HdrSize = pCtx->TotalHdrSize;

        if(pCtx->L2HdrMode != L2HDRMODE_NO)
        {
             ApiHeader* pApi;
             ApiParam* pParam;

            pApi = (ApiHeader*) (pHdrBuf + pCtx->L1HdrSize);
            pApi->Length = nDataSize + 2*sizeof (ApiParam);

            if(pCtx->L2HdrMode >= L2HDRMODE_MSPD_SN)
            {// overwrite TID
                pApi->Control &= ~API_TID_MASK;
                pApi->Control |= ((Tid) << API_TID_SHIFT) & API_TID_MASK;
            }
            // overwrite Offset
            pParam = (ApiParam*) ((PUINT8)pApi + API_HDR_SIZE);
            pParam->ParamData[0] = Offset;
            // overwrite parameter Length
            ++pParam;
            pParam->ParamLength = (UINT16)nDataSize + sizeof(UINT32);
            pParam->ParamData[0] = nDataSize;
        }
        //printf("LteSendData2Eth: use hdr (Off=%d Size=%d)\n", Offset, nDataSize);
    }

    {
#if 1
        PUINT8 pEthBuf = pEthTxBuf;

        memcpy(pEthBuf, pHdrBuf, HdrSize);
        memcpy(pEthBuf + HdrSize, pBuf + Offset, nDataSize);
        rc = TxEthPacketProcess(0, 0, pEthBuf, HdrSize + nDataSize);

//        printf("SendPack: buf=%X %X %X %X %X %X %X\n", pHdrBuf,
//            pHdrBuf[10], pHdrBuf[11], pHdrBuf[12], pHdrBuf[13], pHdrBuf[14], pHdrBuf[15]);
//        printf("SendPack: pEthBuf=%X p2=%X Size=%d\n", pEthBuf, (PUINT8)((UINT32)pEthBuf + HdrSize), HdrSize + nDataSize + 4);
//                printf("SendPack: buf=%X %X %X %X %X %X %X\n", pHdrBuf,
//                    pEthBuf[10], pEthBuf[11], pEthBuf[12], pEthBuf[13], pEthBuf[14], pEthBuf[15]);
#else
            rc = TxEthPacketProcess(pHdrBuf, HdrSize, pBuf + Offset, nDataSize);
#endif
    }

    return rc;
}

static void LteSendAck2Eth(PLteCommSmpCtx pApiCtx, PUINT8 pRxBuf)
{
    PApiHeader pRxApi;
    UINT8 RxTid;
    PUINT8 pHdrBuf = NULL;
    UINT8 HdrSize = 0;

    pRxApi = (PApiHeader)(pRxBuf + pApiCtx->L1HdrSize);
    RxTid = (pRxApi->Control & API_TID_MASK) >> API_TID_SHIFT;

    if(pApiCtx->TotalHdrSize == 0)
    {// gather 2 layers header first

        pApiCtx->pTotalHdr = (PUINT8)ROUND((UINT32)(pApiCtx->TotalHdr), 32);
        pHdrBuf = pApiCtx->pTotalHdr;

        if(pApiCtx->L1HdrSize)
        {
            memcpy(pHdrBuf, pApiCtx->L1Hdr, pApiCtx->L1HdrSize);
            HdrSize = pApiCtx->L1HdrSize;
            pApiCtx->TotalHdrSize += pApiCtx->L1HdrSize;
        }

        if(pApiCtx->L2HdrMode != L2HDRMODE_NO)
        {
            ApiHeader* pApi;

            pApi = ApiCreate(pHdrBuf + HdrSize, pRxApi->Control,
                          pRxApi->DstInstID,
                          pRxApi->SrcInstID,
                          pRxApi->Type,
                          pRxApi->MessageID);

            // overwrite TID
            pApi->Control &= ~API_TID_MASK;
            pApi->Control |= ((RxTid) << API_TID_SHIFT) & API_TID_MASK;
            pApi->Control |= API_USE_TID;

            HdrSize += API_HDR_SIZE;

            memcpy(pHdrBuf + pApiCtx->TotalHdrSize, pApi, API_HDR_SIZE);
            pApiCtx->TotalHdrSize += API_HDR_SIZE;

        }
    }
    else
    {// use collected header
        pHdrBuf = pApiCtx->pTotalHdr;
        HdrSize = pApiCtx->TotalHdrSize;

        if(pApiCtx->L2HdrMode != L2HDRMODE_NO)
        {
             ApiHeader* pApi;

            pApi = (ApiHeader*) (pHdrBuf + pApiCtx->L1HdrSize);

            // overwrite TID
            pApi->Control &= ~API_TID_MASK;
            pApi->Control |= ((RxTid) << API_TID_SHIFT) & API_TID_MASK;
        }
    }

    TxEthPacketProcess(0, 0, pHdrBuf, HdrSize);

}

#if 0
static void LteSendData2Cpri(PUINT8 pBuf, UINT16 nDataSize)
{// sends one portion of data
    // do not perform excess copy, provide lower level with Header and with Data
    PLteCommSmpCtx pCtx = &gLteSendOutSmp[PHYCOMMIF_CPRI];

//    CpriDrvSend(HdrBuf, HdrOffset, pBuf, nDataSize);
}
#endif

static MXRC LteTxSmpReTxTimeoutThr (LPVOID pParam)
{
    MXRC rc = MX_OK;
    PReTxCtx pReTxCtx = (PReTxCtx)pParam;

    MxStopTimer(pReTxCtx->ReTxTimer);

    if(pReTxCtx->ReTxNumAttempts)
    {// resending
        LteSendData2Eth((PUINT8)pOutIQSmp,
            pReTxCtx->ReTxOffset, pReTxCtx->ReTxDataSize, pReTxCtx->ReTxTid);

        // Schedule timeout thread
        MxScheduleThreadByEvent(pReTxCtx->ReTxTimeoutThr,
            pReTxCtx->ReTxTimerEvent, THREAD_EVENT_AUTO_RESET);
        MxStartTimer(pReTxCtx->ReTxTimer);
        pReTxCtx->ReTxNumAttempts--;
    }
    else
    {// all is bad, not delivered
        PAsyncProcCtx pCtx = (PAsyncProcCtx)pReTxCtx->RefCtx;
        printf("Data is not delivered for Tid=%d\n", pReTxCtx->ReTxTid);

        // free entry
        if(pCtx->ReTxNumCurr)
        {
            if(pReTxCtx->Idx < (pCtx->ReTxNumCurr - 1))
            {// there are at least two entries, our isn't last, swap our with last
                PReTxCtx pReTxCtxLast;

                pReTxCtxLast = pCtx->pReTxCtxArr[pCtx->ReTxNumCurr - 1];

                pReTxCtxLast->Idx = pReTxCtx->Idx;
                pReTxCtx->Idx = pCtx->ReTxNumCurr - 1;

                pCtx->pReTxCtxArr[pReTxCtx->Idx] = pReTxCtx;
                pCtx->pReTxCtxArr[pReTxCtxLast->Idx] = pReTxCtxLast;
            }
            pCtx->ReTxNumCurr--;
        }
    }

    return rc;
}

static MXRC LteTxSmp2EthCheckAck (LPVOID pRxBuf, UINT32 frmsize)
{
    PLteCommSmpCtx pApiCtx = &gLteSendOutSmp[PHYCOMMIF_ETH];
    PAsyncProcCtx pCtx = &pApiCtx->AProcCtx;
    PUINT8 pL2Hdr = NULL;
    MXRC rc = MX_OK;

    // Assumed symmetrical packets headers
    if(pCtx->ReTxNumCurr)
    {// we're waiting something
        // Jump to L2Hdr
        if(pApiCtx->L1HdrSize)
        {
            ApiHeader* pApi;

            pL2Hdr = (PUINT8)pRxBuf + pApiCtx->L1HdrSize;
            pApi = (ApiHeader*)pL2Hdr;
            if(pApi->Control & API_USE_TID)
            {// used TID
                PReTxCtx     pReTxCtx;
                UINT8 RxTid = (pApi->Control & API_TID_MASK) >> API_TID_SHIFT;
                UINT8 i;
                for(i=0; i<pCtx->ReTxNumCurr; i++)
                {
                    pReTxCtx = pCtx->pReTxCtxArr[i];
                    if(pReTxCtx->ReTxTid == RxTid)
                    {// Ack on our packet
                        pReTxCtx->bAcked = TRUE;

                        if(HANDLE_NOT_NULL(pReTxCtx->ReTxTimer))
                        {
                            MxStopTimer(pReTxCtx->ReTxTimer);
                        }

                        // free entry
                        if(pReTxCtx->Idx < (pCtx->ReTxNumCurr - 1))
                        {// there are at least two entries, our isn't last, swap our with last
                            PReTxCtx pReTxCtxLast;

                            pReTxCtxLast = pCtx->pReTxCtxArr[pCtx->ReTxNumCurr - 1];

                            pReTxCtxLast->Idx = pReTxCtx->Idx;
                            pReTxCtx->Idx = pCtx->ReTxNumCurr - 1;

                            pCtx->pReTxCtxArr[pReTxCtx->Idx] = pReTxCtx;
                            pCtx->pReTxCtxArr[pReTxCtxLast->Idx] = pReTxCtxLast;
                        }
                        pCtx->ReTxNumCurr--;
                        break;
                    }
                }
            }
        }
    }

    // may be we've finished
    if(pCtx->Offset >= pCtx->ActualDataSize)
    {
        if(pCtx->ReTxNumCurr == 0)
        {
            rc = !MX_OK;
        }
    }

    return rc;
}

#if 0
static UINT32 LteSendOutSmp2EthRx (void * p)
{
    volatile LPVOID    pGemDrv;
    HGRxFrm rxfrm;
    MXRC rc;
    LPVOID pRxBuf = NULL;
    UINT32 frmsize;
    UINT32 buf_num;

    pGemDrv = GemDrvGetCtx (1);

    while(TRUE)
    {
        rc = GemDrvRxFrm (pGemDrv, &rxfrm, &buf_num, &frmsize);
        if (rc == MX_OK)
        {
            pRxBuf = GemDrvGetRxBuf (rxfrm);
            rc = LteTxSmp2EthCheckAck (pRxBuf, frmsize);
            GemDrvFreeRxFrm (pGemDrv, rxfrm);
            if(rc != MX_OK)
                break;
        }
    }

    return 0;
}
#endif

static void LteTxSmp2EthRxCb (LPVOID pCBCtx, LPVOID pGemDrvCtx, UINT32 nIrqStatus)
{
    HGRxFrm rxfrm;
    MXRC rc;
    LPVOID pRxBuf = NULL;
    UINT32 frmsize;
    UINT32 buf_num;

    rc = GemDrvRxFrm (pGemDrvCtx, &rxfrm, &buf_num, &frmsize);
    if (rc == MX_OK)
    {
        pRxBuf = GemDrvGetRxBuf (rxfrm);
        rc = LteTxSmp2EthCheckAck (pRxBuf, frmsize);
        GemDrvFreeRxFrm (pGemDrvCtx, rxfrm);
    }
}

static UINT32 LteTxSmp2EthTxThrAcks (void * p)
{
    PAsyncProcCtx pCtx = (PAsyncProcCtx)p;
    MXRC rc = MX_OK;

    while (TRUE)
    {
        // send pOutSmp to Ethernet here
        // use ACKs and retransmition
        printf("use Ack ReTx: Curr=%d\n", pCtx->ReTxNumCurr);
        if(pCtx->ReTxNumCurr < pCtx->ReTxNum)
        {// allowed next Tx or finish
            if(pCtx->Offset >= pCtx->ActualDataSize && (pCtx->ReTxNumCurr == 0))
            {// no more new, exit this thread, retransmitting is done
                if(HANDLE_NOT_NULL(pCtx->AThrComplete))
                {
                    MxScheduleThread(pCtx->AThrComplete);
                }
                break;
            }
            else if(pCtx->Offset < pCtx->ActualDataSize)
            {
                if ((pCtx->ActualDataSize - pCtx->Offset) < pCtx->DataChunkSize)
                {
                    pCtx->DataSize = pCtx->ActualDataSize - pCtx->Offset;
                }

                {// will wait ACK before next portion of data sending
                    UINT8 Idx = pCtx->ReTxNumCurr;
                    PReTxCtx pReTxCtx = pCtx->pReTxCtxArr[Idx];

                    pReTxCtx->ReTxOffset = pCtx->Offset;
                    pReTxCtx->ReTxDataSize = pCtx->DataSize;
                    pReTxCtx->ReTxTid = pCtx->Tid;
                    pReTxCtx->RefCtx = (LPVOID)pCtx;
                    pReTxCtx->Idx = Idx;

                    rc = LteSendData2Eth((PUINT8)pOutIQSmp, pReTxCtx->ReTxOffset,
                        pReTxCtx->ReTxDataSize, pReTxCtx->ReTxTid);

                    if(rc == MX_OK)
                    {
                        // Schedule timeout thread
                        if(HANDLE_NOT_NULL(pReTxCtx->ReTxTimeoutThr))
                        {
                            MxScheduleThreadByEvent(pReTxCtx->ReTxTimeoutThr,
                                pReTxCtx->ReTxTimerEvent, THREAD_EVENT_AUTO_RESET);
                        }
                        if(HANDLE_NOT_NULL(pReTxCtx->ReTxTimer))
                        {
                            MxStartTimer(pReTxCtx->ReTxTimer);
                        }

                        pReTxCtx->ReTxNumAttempts = RETX_ATTEMPTS;

                        pCtx->ReTxNumCurr++;
                        pCtx->ChunkCounter++;
                        pCtx->Tid++;
                        pCtx->Offset += pCtx->DataSize;
                    }
                    else
                    {
                        PhyCommResetGemacDMA(rc);
                    }
                }
            }
        }
    }
    return 0;
}

static UINT32 LteTxSmp2EthTxThrSimple (void * p)
{
    PAsyncProcCtx pCtx = (PAsyncProcCtx)p;
    MXRC rc = MX_OK;

    while (TRUE)
    {
        // send pOutSmp to Ethernet here
        // don't use ACKs and retransmition
        if(pCtx->Offset >= pCtx->ActualDataSize)
        {
            if(HANDLE_NOT_NULL(pCtx->AThrComplete))
            {
                MxScheduleThread(pCtx->AThrComplete);
            }
            break;
        }
        else if ((pCtx->ActualDataSize - pCtx->Offset) < pCtx->DataChunkSize)
        {
            pCtx->DataSize = pCtx->ActualDataSize - pCtx->Offset;
        }

        rc = LteSendData2Eth((PUINT8)pOutIQSmp, pCtx->Offset, pCtx->DataSize, pCtx->Tid);

        if(rc == MX_OK)
        {
            pCtx->Tid++;
            pCtx->Offset += pCtx->DataSize;
            pCtx->ChunkCounter++;
        }
        else
        {
            PhyCommResetGemacDMA(rc);
        }
    }

    return 0;
}

static UINT32 LteTxSmp2CpriTxThr (void * p)
{
    PAsyncProcCtx pCtx = (PAsyncProcCtx)p;
    UINT32 nSec;
    UINT32 nMin;
    UINT32 nHour;
    VUINT32 nTicks   = 0xFFFFFFFF, tmp;

//    VUINT32 Offset = 0;
//    VUINT32 DataSize = pCtx->DataChunkSize;

    printf ("Begin LteSendOutSmp2Cpri:\r\n");
    printf ("Period is %d\r\n", LOCAL_TIMER_PERIOD);

    printf ("\r\n\r\n");

    while (TRUE)
    {
        // send pOutSmp to CPRI here
#if 0
        if(Offset < pApiCtx->ActualDataSize)
        {
            LteSendData2Cpri((PUINT8)pOutIQSmp + Offset, DataSize);
            Offset += DataSize;
        }
        else
        {
            if(HANDLE_NOT_NULL(pCtx->AThrComplete))
            {
                MxScheduleThread(pCtx->AThrComplete);
            }
            break;
        }
#else
        if ((MxGetTime () - nTicks) >= 1000)
        {
            nTicks = MxGetTime ();

            tmp = nTicks / 1000;

            nHour = (tmp / (60*60)) % 24;
            nMin  = (tmp / (60   )) % 60;
            nSec  = (tmp          ) % 60;

            printf ("CPRI %02d:%02d:%02d\r\n", nHour, nMin, nSec);
            if(nSec == 3)
            {
                if(HANDLE_NOT_NULL(pCtx->AThrComplete))
                {
                    MxScheduleThread(pCtx->AThrComplete);
                }
                break;
            }
        }
#endif
    }
    return 0;
}

static UINT32 LteTxSmp2CpriRxThr (void * p)
{
    return 0;
}

static MXRC LteTxSmpCompleteThr (LPVOID pParam)
{
    MXRC rc = MX_OK;
    PAsyncProcCtx pCtx = (PAsyncProcCtx)pParam;
    ApiHeader * pApi;
    int buf [(API_HDR_SIZE + (sizeof (ApiParam) + sizeof (UINT32)) * 2)];

    printf("IQs are sent to air (%X) (%d)!\n", pCtx->Mode, pCtx->ChunkCounter);

    pCtx->ChunkCounter = 0; // To Count Num Packets

    MxStopTimer(pCtx->ATimer);

    pApi = ApiCreate(buf, API_DEF_FLAGS,
                  pCtx->SrcInstID,
                  pCtx->DstInstID,
                  API_TYPE_IND,
                  pCtx->IndMsgID);

    ApiAddErrorCode(pApi, MX_OK);
    ApiAddParam(pApi, NULL, (UINT16) (pCtx->Mode << 8), NULL, 0);

    rc = SvsrSendAPI(pApi);

    return rc;
}

static MXRC LteTxSmpTimeoutThr (LPVOID pParam)
{
    MXRC rc = MX_OK;
    PAsyncProcCtx pCtx = (PAsyncProcCtx)pParam;
    ApiHeader * pApi;
    char buf [API_HDR_SIZE + (sizeof (ApiParam) + sizeof (UINT32)) * 2];

    printf("IQs sending to air is timed out (%X)!\n", pCtx->Mode);

    MxStopTimer(pCtx->ATimer);

    pApi = ApiCreate(buf, API_DEF_FLAGS,
                  pCtx->SrcInstID,
                  pCtx->DstInstID,
                  API_TYPE_IND,
                  pCtx->IndMsgID);

    ApiAddErrorCode(pApi, 0x2310B /*RC_LTE_TIMEOUT*/);
    ApiAddParam(pApi, NULL, pCtx->Mode << 8, 0, 0);

    rc = SvsrSendAPI(pApi);

    return rc;
}

static MXRC LteTxSmpInit(PAsyncProcCtx pCtx, UINT16 L2HdrMode)
{
    MXRC rc = MX_OK;

    if(pGemDrv == NULL)
    {
        pGemDrv = GemDrvGetCtx (1);
    }

    if(HANDLE_NULL(pCtx->AThrComplete))
    {
        rc = MxCreateThread(LteTxSmpCompleteThr, NULL, pCtx, 1, &pCtx->AThrComplete);
        if (FAILED (rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }
    }

    if(HANDLE_NULL(pCtx->AThrTimeout))
    {
        rc = MxCreateThread(LteTxSmpTimeoutThr, NULL, pCtx, 1, &pCtx->AThrTimeout);
        if (FAILED (rc))
        {
            _ASSERT_RC(rc);
            ClearACtx(pCtx);
            return rc;
        }
    }

    if(HANDLE_NULL(pCtx->AEvent))
    {
        rc = MxCreateEvent(EVENT_MODE_RESET, NULL, &pCtx->AEvent);
        if (FAILED (rc))
        {
            _ASSERT_RC(rc);
            ClearACtx(pCtx);
            return rc;
        }
    }

    if(HANDLE_NULL(pCtx->ATimer))
    {
        rc = MxCreateTimer(TIMER_MODE_ONE_TIME, pCtx->ATimeout, NULL, pCtx->AEvent, &pCtx->ATimer);
        if (FAILED (rc))
        {
            _ASSERT_RC(rc);
            ClearACtx(pCtx);
            return rc;
        }
    }

    if(HANDLE_NULL(pCtx->AThrMainTx))
    {
        if(pCtx->Mode == (1 << PHYCOMMIF_ETH))
        {
            if(L2HdrMode == L2HDRMODE_MSPD_SN_ACK)
            {
                rc = MxCreateThread (LteTxSmp2EthTxThrAcks, NULL, pCtx, 0, &pCtx->AThrMainTx);
            }
            else
            {
                rc = MxCreateThread (LteTxSmp2EthTxThrSimple, NULL, pCtx, 0, &pCtx->AThrMainTx);

                if(pCtx->ReTxNum)
                {
                    {// init pointers
                        UINT32 i=0;
                        for(i=0; i<RETX_QUEUE_SIZE; i++)
                        {
                            ReTxCtxClear(&ReTxCtxArr[i]);
                        }
                    }
                    pCtx->ReTxNum = 0;
                }
            }
        }
        else if(pCtx->Mode == (1 << PHYCOMMIF_CPRI))
        {
            rc = MxCreateThread (LteTxSmp2CpriTxThr, NULL, pCtx, 0, &pCtx->AThrMainTx);
        }
        if (FAILED (rc))
        {
            _ASSERT_RC(rc);
            ClearACtx(pCtx);
            return rc;
        }
    }

    if(HANDLE_NULL(pCtx->AThrMainRx))
    {
        if(L2HdrMode == L2HDRMODE_MSPD_SN_ACK)
        {
            if(pCtx->Mode == (1 << PHYCOMMIF_ETH))
            {
                //rc = MxCreateThread (LteSendOutSmp2EthRx, NULL, pCtx, 0, &pCtx->AThrMainRx);
                // Register new Rx callback, Store old one if any
                GemDrvDisableRX(pGemDrv);

                GemDrvPushIrqProc(pGemDrv);
                GemDrvSetIrqProc(pGemDrv, LteTxSmp2EthRxCb, NULL, GEM_INT_RX_COMPLETE);

                GemDrvEnableRX(pGemDrv);

                printf("LteTxSmpInit: set Rx CB\n");

                pCtx->RxCbCreated = 1;
            }
            else if(pCtx->Mode == (1 << PHYCOMMIF_CPRI))
            {
                rc = MxCreateThread (LteTxSmp2CpriRxThr, NULL, pCtx, 0, &pCtx->AThrMainRx);
            }
            if (FAILED (rc))
            {
                _ASSERT_RC(rc);
                ClearACtx(pCtx);
                return rc;
            }
            pCtx->ReTxNum = RETX_QUEUE_SIZE;
            pCtx->ReTxTimeout = RETX_TIMEOUT;
            {// init pointers
                UINT32 i=0;
                for(i=0; i<RETX_QUEUE_SIZE; i++)
                {
                    PReTxCtx pReTxCtx = &ReTxCtxArr[i];

                    pCtx->pReTxCtxArr[i] = pReTxCtx;
                    pReTxCtx->ReTxTimeout = pCtx->ReTxTimeout;
                    pReTxCtx->RefCtx = (LPVOID)pCtx;
                    pReTxCtx->Idx = i;

                    ReTxCtxInit(pReTxCtx, LteTxSmpReTxTimeoutThr);
                }
            }
        }
    }

    pCtx->ReTxNumCurr = 0;

    return rc;
}

MXRC LteSendOutSmp(unsigned int mode)
{
    MXRC rc = MX_OK;

    if(pOutIQSmp == NULL)
        return rc;
    if (HANDLE_NULL(PhyDataTimer))
    {
        printf ("Warning: LTmr is not found\r\n");
    }

    if(mode & (1 << PHYCOMMIF_ETH))
    {
        // run in async mode with timer for timeout
        // report 'complete' or 'timeout' to Host by indication
        PAsyncProcCtx pCtx = &gLteSendOutSmp[PHYCOMMIF_ETH].AProcCtx;

        // Schedule timeout thread
        rc = MxScheduleThreadByEvent(pCtx->AThrTimeout, pCtx->AEvent, THREAD_EVENT_AUTO_RESET);
        if (FAILED (rc))
        {
            printf("Can't schedule timeout thread (%d)\n", PHYCOMMIF_ETH);
            return rc;
        }

#if 0
        {
            UINT32 i = 0;
            PUINT32 pDst = (PUINT32)pOutIQSmp;
            do
            {
                *pDst++ = 0xAABBCCDD;
            }while(++i < pCtx->ActualDataSize / sizeof(PUINT32));
        }
#endif
        printf ("IQs sending to Ethernet is started...\n");
        pCtx->ChunkCounter = 0; // To Count Num Packets

        // Start timer
        rc = MxStartTimer(pCtx->ATimer);
        if (FAILED(rc))
        {
            printf ("Start timer error: %x (%d)\r\n", rc, PHYCOMMIF_ETH);
            _ASSERT_RC(rc);
            ClearACtx(pCtx);
            return rc;
        }

        pCtx->Offset = 0;
        pCtx->DataSize = pCtx->DataChunkSize;

        // Start data sending thread
        MxScheduleThread(pCtx->AThrMainTx);
    }

    if(mode & (1 << PHYCOMMIF_CPRI))
    {
        // run in async mode with timer for timeout
        // report 'complete' or 'timeout' to Host by indication
        PAsyncProcCtx pCtx = &gLteSendOutSmp[PHYCOMMIF_CPRI].AProcCtx;

        // Schedule timeout thread
        rc = MxScheduleThreadByEvent(pCtx->AThrTimeout, pCtx->AEvent, THREAD_EVENT_AUTO_RESET);
        if (FAILED (rc))
        {
            printf("Can't schedule timeout thread (%d)\n", PHYCOMMIF_CPRI);
            _ASSERT_RC(rc);
            return rc;
        }

        printf ("IQs sending to CPRI is started...\n");

        // Start timer
        rc = MxStartTimer(pCtx->ATimer);
        if (FAILED(rc))
        {
            printf ("Start timer error: %x (%d)\r\n", rc, PHYCOMMIF_CPRI);
            _ASSERT_RC(rc);
            ClearACtx(pCtx);
            return rc;
        }

        pCtx->Offset = 0;
        pCtx->DataSize = pCtx->DataChunkSize;

        // Start data sending thread
        MxScheduleThread(pCtx->AThrMainTx);

        // Schedule receive thread (to be removed if IRQ CallBack used)
        if(gLteSendOutSmp[PHYCOMMIF_CPRI].L2HdrMode == L2HDRMODE_MSPD_SN_ACK)
        {
            MxScheduleThread(pCtx->AThrMainRx);
        }
    }

    return rc;
}

static MXRC LteRxSmpCompleteThr (LPVOID pParam)
{
    MXRC rc = MX_OK;
    PAsyncProcCtx pCtx = (PAsyncProcCtx)pParam;
    ApiHeader * pApi;
    char buf [API_HDR_SIZE + (sizeof (ApiParam) + sizeof (UINT32)) * 2];

    printf("IQs are got from air (%X)!\n", pCtx->Mode);

    MxStopTimer(pCtx->ATimer);
    pCtx->ChunkCounter = 0; // To Count Num Packets
    pCtx->ReTxNum = 0;
    pCtx->ReTxNumCurr = 0;

    pApi = ApiCreate(buf, API_DEF_FLAGS,
                  pCtx->SrcInstID,
                  pCtx->DstInstID,
                  API_TYPE_IND,
                  pCtx->IndMsgID);

    ApiAddErrorCode(pApi, MX_OK);
    ApiAddParam(pApi, NULL, pCtx->Mode << 8, 0, 0);

    rc = SvsrSendAPI(pApi);

    // And start new data portion receiving
    // Schedule timeout thread
    rc = MxScheduleThreadByEvent(pCtx->AThrTimeout, pCtx->AEvent, THREAD_EVENT_AUTO_RESET);
    if (FAILED (rc))
    {
        printf("Can't schedule timeout thread (%d)\n", PHYCOMMIF_ETH);
        _ASSERT_RC(rc);
        return rc;
    }

    printf("waiting for next IQs from air is started...\n");

    // Start timer
    rc = MxStartTimer(pCtx->ATimer);
    if (FAILED(rc))
    {
        printf ("Start timer error: %x (%d)\r\n", rc, PHYCOMMIF_ETH);
        _ASSERT_RC(rc);
        ClearACtx(pCtx);
        return rc;
    }

    return rc;
}

static MXRC LteRxSmpTimeoutThr (LPVOID pParam)
{
    MXRC rc = MX_OK;
    PAsyncProcCtx pCtx = (PAsyncProcCtx)pParam;
    ApiHeader * pApi;
    char buf [API_HDR_SIZE + (sizeof (ApiParam) + sizeof (UINT32)) * 2];

    printf("IQs getting from air is timed out (%X)!\n", pCtx->Mode);

    MxStopTimer(pCtx->ATimer);

    pApi = ApiCreate(buf, API_DEF_FLAGS,
                  pCtx->SrcInstID,
                  pCtx->DstInstID,
                  API_TYPE_IND,
                  pCtx->IndMsgID);

    ApiAddErrorCode(pApi, 0x2310B /*RC_LTE_TIMEOUT*/);
    ApiAddParam(pApi, NULL, pCtx->Mode << 8, 0, 0);

    rc = SvsrSendAPI(pApi);

    // And start new data portion receiving
    // Schedule timeout thread
    rc = MxScheduleThreadByEvent(pCtx->AThrTimeout, pCtx->AEvent, THREAD_EVENT_AUTO_RESET);
    if (FAILED (rc))
    {
        printf("Can't schedule timeout thread (%d)\n", PHYCOMMIF_ETH);
        _ASSERT_RC(rc);
        return rc;
    }

    printf("waiting for next IQs from air is started...\n");

    // Start timer
    rc = MxStartTimer(pCtx->ATimer);
    if (FAILED(rc))
    {
        printf ("Start timer error: %x (%d)\r\n", rc, PHYCOMMIF_ETH);
        _ASSERT_RC(rc);
        ClearACtx(pCtx);
        return rc;
    }

    return rc;
}

static MXRC LteRxSmpNextDataTimeoutThr (LPVOID pParam)
{
    MXRC rc = MX_OK;
    PReTxCtx pReTxCtx = (PReTxCtx)pParam;
    PAsyncProcCtx pCtx = (PAsyncProcCtx)pReTxCtx->RefCtx;
    ApiHeader * pApi;
    char buf [API_HDR_SIZE + (sizeof (ApiParam) + sizeof (UINT32)) * 2];

    MxStopTimer(pReTxCtx->ReTxTimer);

    printf ("Waiting for next data from air (%X) is timed out!\n", pCtx->Mode);

    pApi = ApiCreate(buf, API_DEF_FLAGS,
                  pCtx->SrcInstID,
                  pCtx->DstInstID,
                  API_TYPE_IND,
                  pCtx->IndMsgID);

    ApiAddErrorCode(pApi, 0x2310B /*RC_LTE_TIMEOUT*/);
    ApiAddParam(pApi, NULL, pCtx->Mode << 8, 0, 0);

    rc = SvsrSendAPI(pApi);

    return rc;
}

static MXRC LteRxSmpEthRxDataThr (LPVOID pParam)
{
    PLteCommSmpCtx pApiCtx;
    PAsyncProcCtx pCtx;
    LPVOID pGemacDrv = pGemDrv;
    //PReTxCtx pReTxCtx;

    UINT32 frmsize;
    UINT32 buf_num;
    HGRxFrm rxfrm;

    MXRC rc = MX_OK;

    pApiCtx = &gLteRcvInSmp[PHYCOMMIF_ETH];
    pCtx = &pApiCtx->AProcCtx;

    //pReTxCtx = pCtx->pReTxCtxArr[0];

    while((rc = GemDrvRxFrm (pGemacDrv, &rxfrm, &buf_num, &frmsize)) == MX_OK)
    {
        LPVOID pRxBuf = GemDrvGetRxBuf (rxfrm);

//        printf("rxFrm=%p,bufNum=%i,frmSize=%i,buf=%p,Descr=%X\n",
//               rxfrm, buf_num, frmsize, pRxBuf, rxfrm->Descr);

        //printf("Rx Pack: Proto_%X\n", *(PUINT16)((UINT32)pRxBuf + 12));

        if(*(PUINT16)((UINT32)pRxBuf + 12) != ETH_PROTOID_T4K)
        {
            GemDrvFreeRxFrm (pGemacDrv, rxfrm);
            continue;
        }
        // we're waiting something

        // Jump to L2Hdr
        if(pApiCtx->L1HdrSize)
        {
            PApiHeader pApi;
            PApiParam pParam;
            UINT32 offset;
            UINT32 dataSize;
//            UINT8 Stack[512 + 16];
//            PUINT8 pStack;

            pApi = (ApiHeader*)((PUINT8)pRxBuf + pApiCtx->L1HdrSize);

//            printf("Ctrl=%X,Len=%p,Typ=%X,Msg=%X\n", pApi->Control, pApi->Length, pApi->Type, pApi->MessageID);

            ++pCtx->ChunkCounter;

            // Buffer offset
            pParam = (ApiParam*) (pApi + 1);
            if (pParam->ParamID == 4 /*PHY_BUF_OFFS_PARAM_ID*/)
                offset = pParam->ParamData[0];
            else
            {
                offset = 0;
                ++pCtx->ReTxNum;
                printf("Unexp. paramID: %4.4X (#%d)\n", pParam->ParamID, pCtx->ChunkCounter);
                GemDrvFreeRxFrm (pGemacDrv, rxfrm);
                continue;
            }

            // Inbound I/Q samples
            ++pParam;
            if (pParam->ParamID == 5 /*PHY_DATA_PARAM_ID*/)
            {
                dataSize = pParam->ParamData[0];
                if(dataSize > ETH_DATA_CHUNK)
                {
                    printf("Unexp. data size: %d (#%d)\n", dataSize, pCtx->ChunkCounter);
                    GemDrvFreeRxFrm (pGemacDrv, rxfrm);
                    continue;
                }
            }
            else
            {
                dataSize = 0;
                ++pCtx->ReTxNumCurr;
                printf("T4K Holiday!!!\n");
                GemDrvFreeRxFrm (pGemacDrv, rxfrm);
                continue;
            }

 /*           if((pCtx->DataSize > ((pCtx->ActualDataSize / ETH_DATA_CHUNK) - 8))
                && ((pCtx->DataSize % 2) != 0))
                    printf("Rx# %d Off=%X Size=%d\n", pCtx->DataSize, offset, dataSize);*/

/*            pStack = (PUINT8)ROUND((UINT32)Stack, 16);
            if(dataSize < sizeof(Stack))
            {
                memcpy(pStack, pParam->ParamData + 1, dataSize);
            }*/

            if((offset + dataSize) <= pCtx->ActualDataSize)
            {
                memcpy(((PUINT8)pInIQSmp + offset), pParam->ParamData + 1 /*pStack*/, dataSize);
            }
            else
            {
                printf("Oh Men! Par=%p,Offs=%i,Size=%i\n", pParam, offset, dataSize);
            }

            GemDrvFreeRxFrm (pGemacDrv, rxfrm);

            if(pApi->Control & API_USE_TID)
            {// Send Ack
                LteSendAck2Eth(pApiCtx, pRxBuf);
            }

            if((offset + dataSize) >= pCtx->ActualDataSize)
            {// finish
                printf("RX IQ done (%d) (%d %d)!!!!!\n", pCtx->ChunkCounter, pCtx->ReTxNum, pCtx->ReTxNumCurr);
                if(HANDLE_NOT_NULL(pCtx->AThrComplete))
                {
                    MxScheduleThread(pCtx->AThrComplete);
                }
            }
            else
            {
                // Schedule next packet timeout thread
/*                        if(HANDLE_NOT_NULL(pReTxCtx->ReTxTimeoutThr) &&
                    HANDLE_NOT_NULL(pReTxCtx->ReTxTimerEvent) &&
                    HANDLE_NOT_NULL(pReTxCtx->ReTxTimer))
                {
                    MxScheduleThreadByEvent(pReTxCtx->ReTxTimeoutThr,
                        pReTxCtx->ReTxTimerEvent, THREAD_EVENT_AUTO_RESET);
                    MxStartTimer(pReTxCtx->ReTxTimer);
                }*/
            }
        }
        else
        {
            GemDrvFreeRxFrm (pGemacDrv, rxfrm);
        }
    }
    return rc;
}


static void LteRxSmpEthRxCb (LPVOID pCBCtx, LPVOID pGemDrvCtx, UINT32 nIrqStatus)
{// to process incoming packet here
#if 0
    PLteCommSmpCtx pApiCtx = &gLteRcvInSmp[PHYCOMMIF_ETH];
    PAsyncProcCtx pCtx = &pApiCtx->AProcCtx;

    if(HANDLE_NOT_NULL(pCtx->AThrMainRx))
    {
        MxScheduleThread(pCtx->AThrMainRx);
    }
#else
//    printf("Rx IRQ\n");
    if(gProcessGemacRx == 0)
        return;
    LteRxSmpEthRxDataThr (NULL);
#endif
}

static UINT32 LteRxSmpCpriRxThr (void * p)
{
    MXRC rc = MX_OK;
    return rc;
}

static MXRC LteRxSmpInit(PAsyncProcCtx pCtx, UINT16 L2HdrMode)
{
    MXRC rc = MX_OK;

    if(pGemDrv == NULL)
    {
        pGemDrv = GemDrvGetCtx (1);
    }

    if(HANDLE_NULL(pCtx->AThrComplete))
    {
        rc = MxCreateThread(LteRxSmpCompleteThr, NULL, pCtx, 1, &pCtx->AThrComplete);
        if (FAILED (rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }
    }

    if(HANDLE_NULL(pCtx->AThrTimeout))
    {
        rc = MxCreateThread(LteRxSmpTimeoutThr, NULL, pCtx, 1, &pCtx->AThrTimeout);
        if (FAILED (rc))
        {
            _ASSERT_RC(rc);
            ClearACtx(pCtx);
            return rc;
        }
    }

    if(HANDLE_NULL(pCtx->AEvent))
    {
        rc = MxCreateEvent(EVENT_MODE_RESET, NULL, &pCtx->AEvent);
        if (FAILED (rc))
        {
            _ASSERT_RC(rc);
            ClearACtx(pCtx);
            return rc;
        }
    }

    if(HANDLE_NULL(pCtx->ATimer))
    {
        rc = MxCreateTimer(TIMER_MODE_ONE_TIME, pCtx->ATimeout, NULL, pCtx->AEvent, &pCtx->ATimer);
        if (FAILED (rc))
        {
            _ASSERT_RC(rc);
            ClearACtx(pCtx);
            return rc;
        }
    }

    if(HANDLE_NULL(pCtx->AThrMainRx))
    {
        if(pCtx->Mode == (1 << PHYCOMMIF_ETH))
        {
            PReTxCtx pReTxCtx;

            rc = MxCreateThread (LteRxSmpEthRxDataThr, NULL, NULL, 0, &pCtx->AThrMainRx);

            // Register new Rx callback, Store old one if any
            GemDrvDisableRX(pGemDrv);

            GemDrvPushIrqProc(pGemDrv);
            GemDrvSetIrqProc(pGemDrv, LteRxSmpEthRxCb, NULL, GEM_INT_RX_COMPLETE);

            GemDrvEnableRX(pGemDrv);

            printf("LteRxSmpInit: set Rx CB\n");

            pCtx->RxCbCreated = 1;
            pCtx->ReTxTimeout = RETX_TIMEOUT * 2;   // will use to wait for next packet

            {// init pointers
                UINT32 i=0;
                for(i=0; i<RETX_QUEUE_SIZE; i++)
                {
                    pCtx->pReTxCtxArr[i] = &ReTxCtxArr[i];
                }
            }

            pReTxCtx = pCtx->pReTxCtxArr[0];

            pReTxCtx->ReTxTimeout = pCtx->ReTxTimeout;
            pReTxCtx->RefCtx = (LPVOID)pCtx;
            pReTxCtx->Idx = 0;

            ReTxCtxInit(pReTxCtx, LteRxSmpNextDataTimeoutThr);

        }
        else if(pCtx->Mode == (1 << PHYCOMMIF_CPRI))
        {
            rc = MxCreateThread (LteRxSmpCpriRxThr, NULL, pCtx, 0, &pCtx->AThrMainRx);
        }
        if (FAILED (rc))
        {
            _ASSERT_RC(rc);
            ClearACtx(pCtx);
            return rc;
        }
    }

    return rc;
}

MXRC LteRcvInSmp(unsigned int mode)
{
    MXRC rc = MX_OK;

    if(pInIQSmp == NULL)
        return rc;
    if (HANDLE_NULL(PhyDataTimer))
    {
        printf ("Warning: LTmr is not found\r\n");
    }

    if(mode & (1 << PHYCOMMIF_ETH))
    {
        PAsyncProcCtx pCtx = &gLteRcvInSmp[PHYCOMMIF_ETH].AProcCtx;

        // Schedule timeout thread
        rc = MxScheduleThreadByEvent(pCtx->AThrTimeout, pCtx->AEvent, THREAD_EVENT_AUTO_RESET);
        if (FAILED (rc))
        {
            printf("Can't schedule timeout thread (%d)\n", PHYCOMMIF_ETH);
            _ASSERT_RC(rc);
            return rc;
        }

        printf ("IQs getting from Ethernet is started...\n");
        pCtx->ChunkCounter = 0; // To Count Num Packets
        pCtx->ReTxNum = 0; // To Count error packets - 1
        pCtx->ReTxNumCurr = 0; // To Count error packets - 2

        // Start timer
        rc = MxStartTimer(pCtx->ATimer);
        if (FAILED(rc))
        {
            printf ("Start timer error: %x (%d)\r\n", rc, PHYCOMMIF_ETH);
            _ASSERT_RC(rc);
            ClearACtx(pCtx);
            return rc;
        }

            GemDrvDisableRX(pGemDrv);
            gProcessGemacRx = 1;
            GemDrvEnableRX(pGemDrv);

    }

    if(mode & (1 << PHYCOMMIF_CPRI))
    {
        PAsyncProcCtx pCtx = &gLteRcvInSmp[PHYCOMMIF_CPRI].AProcCtx;

        // Schedule timeout thread
        rc = MxScheduleThreadByEvent(pCtx->AThrTimeout, pCtx->AEvent, THREAD_EVENT_AUTO_RESET);
        if (FAILED (rc))
        {
            printf("Can't schedule timeout thread (%d)\n", PHYCOMMIF_CPRI);
            _ASSERT_RC(rc);
            return rc;
        }

        printf ("IQs getting from CPRI is started...\n");

        // Start timer
        rc = MxStartTimer(pCtx->ATimer);
        if (FAILED(rc))
        {
            printf ("Start timer error: %x (%d)\r\n", rc, PHYCOMMIF_CPRI);
            _ASSERT_RC(rc);
            ClearACtx(pCtx);
            return rc;
        }

    }

    return rc;
}

MXRC PhyDataCommApiProc(ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * pParam;
    MXRC rc = MX_OK;

    if(pApi->Type == API_TYPE_CFG_PARAM)
    {
        switch(pApi->MessageID)
        {
            case PHY_LTE_SEND_IQ_ENA:
                // LTE
                uart_printf("PhyDataCommApiProc: Not Supported any More\r\n");

                // to prevent warnings 
                while(rc == MX_OK);

#if 0
                pOutIQSmp = pDacSamples;
                if(pOutIQSmp == NULL)
                {
                    rc = 0x2310A; //RC_PHY_HEAP_ALLOC_ERROR;
                }
                else if(pApi->Length)
                {
                    ApiParam * pParamGroup = NULL;
                    while ((pParamGroup = ApiGetNextParam (pApi, pParamGroup) ) != NULL)
                    {
                        if(pParamGroup->ParamID == PAR_COMMIQ_PARAM_GROUP)
                        {
                            PLteCommSmpCtx pCtx = NULL;

                            pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_MODE);
                            if(pParam)
                            {
                                if(pParam->ParamData[0] == (1 << PHYCOMMIF_ETH))
                                {// 2Eth
                                    PhyCommIFTx |= (1 << PHYCOMMIF_ETH);
                                    pCtx = &gLteSendOutSmp[PHYCOMMIF_ETH];
                                    pCtx->PhyComm = 1 << PHYCOMMIF_ETH;
                                }
                                else if(pParam->ParamData[0] == (1 << PHYCOMMIF_CPRI))
                                {// 2Cpri
                                    PhyCommIFTx |= (1 << PHYCOMMIF_CPRI);
                                    pCtx = &gLteSendOutSmp[PHYCOMMIF_CPRI];
                                    pCtx->PhyComm = 1 << PHYCOMMIF_CPRI;
                                }
                            }
                            if(pCtx)
                            {
                                // 1 anttenna by default
                                pCtx->NumAntennas = 1;
                                pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_NUMANT);
                                if(pParam)
                                {
                                    pCtx->NumAntennas = (UINT8)pParam->ParamData[0];
                                }

                                // 10 MHz by default
                                pCtx->ChanBW = 3;
                                pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_CHANBW);
                                if(pParam)
                                {
                                    pCtx->ChanBW = (UINT8)pParam->ParamData[0];
                                }

                                pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_DATACHUNKSIZE);
                                pCtx->AProcCtx.DataChunkSize = 0;
                                if(pParam && pParam->ParamLength)
                                {
                                    pCtx->AProcCtx.DataChunkSize = (UINT16)pParam->ParamData[0];
                                }
                                pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_L1HDR);
                                if(pParam && pParam->ParamLength)
                                {
                                    if(pParam->ParamLength <= sizeof(pCtx->L1Hdr))
                                    {
                                        pCtx->L1HdrSize = pParam->ParamLength;
                                        memcpy(pCtx->L1Hdr, (PUINT8)pParam->ParamData, pCtx->L1HdrSize);
                                    }
                                    else
                                    {
                                        rc = 0x23101; //RC_LTE_PARAM_ERROR;
                                        continue;
                                    }
                                }
                                pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_L2MODE);
                                if(pParam && pParam->ParamLength)
                                {
                                    pCtx->L2HdrMode = (UINT16)pParam->ParamData[0];
                                }
                                pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_L2HDR);
                                if(pParam && pParam->ParamLength)
                                {
                                    if(pParam->ParamLength <= sizeof(pCtx->L2Hdr))
                                    {
                                        pCtx->L2HdrSize = pParam->ParamLength;
                                        memcpy(pCtx->L2Hdr, (PUINT8)pParam->ParamData, pCtx->L2HdrSize);
                                    }
                                    else
                                    {
                                        rc = 0x23101; //RC_LTE_PARAM_ERROR;
                                        continue;
                                    }
                                }
                                if(pCtx->PhyComm == (1 << PHYCOMMIF_ETH))
                                {// set rest of params, perform some init
                                    pCtx->TotalHdrSize = 0;

                                    if(pCtx->AProcCtx.DataChunkSize == 0)
                                        pCtx->AProcCtx.DataChunkSize = ETH_DATA_CHUNK;

                                    pCtx->AProcCtx.Mode = 1 << PHYCOMMIF_ETH;
                                    pCtx->AProcCtx.ATimeout = GEN_TIMEOUT;
                                    pCtx->AProcCtx.SrcInstID = 0x19; //pLteCtx->nInstanceID;
                                    pCtx->AProcCtx.DstInstID = UARM_INSTID_HOST;
                                    pCtx->AProcCtx.IndMsgID = pApi->MessageID;//PHY_LTE_PHYTICK_MSG;

                                    pCtx->AProcCtx.ActualDataSize = LteGetSamplesSizeByte(pCtx);

                                    pEthTxBufAlloc = MxHeapAlloc (ETH_TXBUF_HEAP, ETH_DATA_CHUNK + 88);
                                    if(pEthTxBufAlloc == NULL)
                                    {
                                        rc = 0x23102; //RC_LTE_ALLOC_ERROR;
                                        break;
                                    }
                                    pEthTxBuf = (PUINT8)ROUND((UINT32)pEthTxBufAlloc, 32);

                                    rc = LteTxSmpInit(&pCtx->AProcCtx, pCtx->L2HdrMode);
                                    if (FAILED (rc))
                                    {
                                        _ASSERT_RC(rc);
                                    }
                                    printf("IQs sending to Ethernet: Chunk=%d L1HdrSize=%d L2HdrSize=%d\n",
                                        pCtx->AProcCtx.DataChunkSize,
                                        pCtx->L1HdrSize,
                                        pCtx->L2HdrSize);
                                }
                                else if(pCtx->PhyComm == (1 << PHYCOMMIF_CPRI))
                                {// set rest of params, perform some init
                                    pCtx->TotalHdrSize = 0;

                                    if(pCtx->AProcCtx.DataChunkSize == 0)
                                        pCtx->AProcCtx.DataChunkSize = CPRI_DATA_CHUNK;

                                    pCtx->AProcCtx.Mode = 1 << PHYCOMMIF_CPRI;
                                    pCtx->AProcCtx.ATimeout = GEN_TIMEOUT;
                                    pCtx->AProcCtx.SrcInstID = 0x19; //pLteCtx->nInstanceID;
                                    pCtx->AProcCtx.DstInstID = UARM_INSTID_HOST;
                                    pCtx->AProcCtx.IndMsgID = PHY_LTE_PHYTICK_MSG;

                                    pCtx->AProcCtx.ActualDataSize = LteGetSamplesSizeByte(pCtx);

                                    rc = LteTxSmpInit(&pCtx->AProcCtx, pCtx->L2HdrMode);
                                    if (FAILED (rc))
                                    {
                                        _ASSERT_RC(rc);
                                    }
                                    printf("IQs sending to CPRI: Chunk=%d L1HdrSize=%d L2HdrSize=%d\n",
                                        pCtx->AProcCtx.DataChunkSize,
                                        pCtx->L1HdrSize,
                                        pCtx->L2HdrSize);
                                }

                                // Get timer object from system
                                if (HANDLE_NULL(PhyDataTimer))
                                {
                                    rc = MxGetObjByName(CUR_CPU, LTmrName, &PhyDataTimer);
                                    if (FAILED(rc))
                                    {
                                        PhyDataTimer = HNULL;
                                        printf ("Warning: LTmr is not found - 1\r\n");
                                    }
                                }
                            }
                            else
                            {
                                rc = 0x23108; //RC_PHY_INCORRECT_PARAM;
                                continue;
                            }
                        }
                    }
                }
                else
                {
                    rc = 0x23101; //RC_LTE_PARAM_ERROR;
                }
#endif
                break;

            case PHY_LTE_SEND_IQ_DIS:
                if(pApi->Length == 0)
                {// disable all by default
                    PLteCommSmpCtx pCtx;

                    pCtx = &gLteSendOutSmp[PHYCOMMIF_ETH];
                    ClearACtx(&pCtx->AProcCtx);
                    pCtx->L2HdrSize = 0;
                    pCtx->L1HdrSize = 0;
                    pCtx->TotalHdrSize = 0;
                    pCtx->AProcCtx.DataChunkSize = 0;
                    pCtx->L2HdrMode = L2HDRMODE_NO;
                    pCtx->PhyComm = 0;
                    if(pEthTxBufAlloc)
                    {
                        MxHeapFree(ETH_TXBUF_HEAP, pEthTxBufAlloc);
                        pEthTxBufAlloc = NULL;
                        pEthTxBuf = NULL;
                    }
                    pCtx = &gLteSendOutSmp[PHYCOMMIF_CPRI];
                    ClearACtx(&pCtx->AProcCtx);
                    pCtx->L2HdrSize = 0;
                    pCtx->L1HdrSize = 0;
                    pCtx->TotalHdrSize = 0;
                    pCtx->AProcCtx.DataChunkSize = 0;
                    pCtx->L2HdrMode = L2HDRMODE_NO;
                    pCtx->PhyComm = 0;
                    PhyCommIFTx = 0;
                    printf("\t-disabled all\n");
                }
                else
                {
                    ApiParam * pParamGroup = NULL;
                    while ((pParamGroup = ApiGetNextParam (pApi, pParamGroup) ) != NULL)
                    {
                        if(pParamGroup->ParamID == PAR_COMMIQ_PARAM_GROUP)
                        {
                            PLteCommSmpCtx pCtx = NULL;

                            pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_MODE);
                            if(pParam)
                            {
                                if(pParam->ParamData[0] == (1 << PHYCOMMIF_ETH))
                                {// 2Eth
                                    PhyCommIFTx &= ~(1 << PHYCOMMIF_ETH);
                                    pCtx = &gLteSendOutSmp[PHYCOMMIF_ETH];
                                }
                                else if(pParam->ParamData[0] == (1 << PHYCOMMIF_CPRI))
                                {// 2Cpri
                                    PhyCommIFTx &= ~(1 << PHYCOMMIF_CPRI);
                                    pCtx = &gLteSendOutSmp[PHYCOMMIF_CPRI];
                                }
                            }
                            if(pCtx)
                            {
                                ClearACtx(&pCtx->AProcCtx);
                                pCtx->L1HdrSize = 0;
                                pCtx->L2HdrSize = 0;
                                pCtx->TotalHdrSize = 0;
                                pCtx->AProcCtx.DataChunkSize = 0;
                                pCtx->L2HdrMode = L2HDRMODE_NO;
                                pCtx->PhyComm = 0;
                                if(pEthTxBufAlloc)
                                {
                                    MxHeapFree(ETH_TXBUF_HEAP, pEthTxBufAlloc);
                                    pEthTxBufAlloc = NULL;
                                    pEthTxBuf = NULL;
                                }
                            }
                            else
                            {
                                rc = 0x23108; //RC_PHY_INCORRECT_PARAM;
                                continue;
                            }
                        }
                    }
                }
                break;

            case PHY_LTE_RCV_IQ_ENA:
                // LTE
                pInIQSmp = pInSmp;

                if(pInIQSmp == NULL)
                {
                    rc = 0x2310A; //RC_PHY_HEAP_ALLOC_ERROR;
                }
                else if(pApi->Length)
                {
                    ApiParam * pParamGroup = NULL;
                    while ((pParamGroup = ApiGetNextParam (pApi, pParamGroup) ) != NULL)
                    {
                        if(pParamGroup->ParamID == PAR_COMMIQ_PARAM_GROUP)
                        {
                            PLteCommSmpCtx pCtx = NULL;

                            pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_MODE);
                            if(pParam)
                            {
                                if(pParam->ParamData[0] == (1 << PHYCOMMIF_ETH))
                                {// 2Eth
                                    PhyCommIFRx |= (1 << PHYCOMMIF_ETH);
                                    pCtx = &gLteRcvInSmp[PHYCOMMIF_ETH];
                                    pCtx->PhyComm = 1 << PHYCOMMIF_ETH;
                                    printf("IQs receiving from Eth will be used\n");
                                }
                                else if(pParam->ParamData[0] == (1 << PHYCOMMIF_CPRI))
                                {// 2Cpri
                                    PhyCommIFRx |= (1 << PHYCOMMIF_CPRI);
                                    pCtx = &gLteRcvInSmp[PHYCOMMIF_CPRI];
                                    pCtx->PhyComm = 1 << PHYCOMMIF_CPRI;
                                    printf("IQs receiving from CPRI will be used\n");
                                }
                            }
                            if(pCtx)
                            {
                                // 1 anttenna by default
                                pCtx->NumAntennas = 1;
                                pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_NUMANT);
                                if(pParam)
                                {
                                    pCtx->NumAntennas = (UINT8)pParam->ParamData[0];
                                }

                                // 10 MHz by default
                                pCtx->ChanBW = 3;
                                pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_CHANBW);
                                if(pParam)
                                {
                                    pCtx->ChanBW = (UINT8)pParam->ParamData[0];
                                }

                                pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_DATACHUNKSIZE);
                                pCtx->AProcCtx.DataChunkSize = 0;
                                if(pParam && pParam->ParamLength)
                                {
                                    pCtx->AProcCtx.DataChunkSize = (UINT16)pParam->ParamData[0];
                                }
                                pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_L1HDR);
                                if(pParam && pParam->ParamLength)
                                {
                                    if(pParam->ParamLength <= sizeof(pCtx->L1Hdr))
                                    {
                                        pCtx->L1HdrSize = pParam->ParamLength;
                                        memcpy(pCtx->L1Hdr, (PUINT8)pParam->ParamData, pCtx->L1HdrSize);
                                    }
                                    else
                                    {
                                        rc = 0x23101; //RC_LTE_PARAM_ERROR;
                                        continue;
                                    }
                                }
                                pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_L2MODE);
                                if(pParam && pParam->ParamLength)
                                {
                                    pCtx->L2HdrMode = (UINT16)pParam->ParamData[0];
                                }
                                pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_L2HDR);
                                if(pParam && pParam->ParamLength)
                                {
                                    if(pParam->ParamLength <= sizeof(pCtx->L2Hdr))
                                    {
                                        pCtx->L2HdrSize = pParam->ParamLength;
                                        memcpy(pCtx->L2Hdr, (PUINT8)pParam->ParamData, pCtx->L2HdrSize);
                                    }
                                    else
                                    {
                                        rc = 0x23101; //RC_LTE_PARAM_ERROR;
                                        continue;
                                    }
                                }
                                if(pCtx->PhyComm == (1 << PHYCOMMIF_ETH))
                                {// set rest of params, perform some init
                                    pCtx->TotalHdrSize = 0;

                                    if(pCtx->AProcCtx.DataChunkSize == 0)
                                        pCtx->AProcCtx.DataChunkSize = ETH_DATA_CHUNK;

                                    pCtx->AProcCtx.Mode = 1 << PHYCOMMIF_ETH;
                                    pCtx->AProcCtx.ATimeout = GEN_TIMEOUT * 3;
                                    pCtx->AProcCtx.SrcInstID = 0x19; //pLteCtx->nInstanceID;
                                    pCtx->AProcCtx.DstInstID = UARM_INSTID_HOST;
                                    pCtx->AProcCtx.IndMsgID = PHY_LTE_PHYTICK_MSG;

                                    pCtx->AProcCtx.ActualDataSize = LteGetSamplesSizeByte(pCtx);

                                    rc = LteRxSmpInit(&pCtx->AProcCtx, pCtx->L2HdrMode);
                                    if (FAILED (rc))
                                    {
                                        _ASSERT_RC(rc);
                                    }
                                    printf("IQs receiving from Ethernet: Chunk=%d L1HdrSize=%d L2HdrSize=%d\n",
                                        pCtx->AProcCtx.DataChunkSize,
                                        pCtx->L1HdrSize,
                                        pCtx->L2HdrSize);
                                }
                                else if(pCtx->PhyComm == (1 << PHYCOMMIF_CPRI))
                                {// set rest of params, perform some init
                                    pCtx->TotalHdrSize = 0;

                                    if(pCtx->AProcCtx.DataChunkSize == 0)
                                        pCtx->AProcCtx.DataChunkSize = CPRI_DATA_CHUNK;

                                    pCtx->AProcCtx.Mode = 1 << PHYCOMMIF_CPRI;
                                    pCtx->AProcCtx.ATimeout = GEN_TIMEOUT * 3;
                                    pCtx->AProcCtx.SrcInstID = 0x19; //pLteCtx->nInstanceID;
                                    pCtx->AProcCtx.DstInstID = UARM_INSTID_HOST;
                                    pCtx->AProcCtx.IndMsgID = PHY_LTE_PHYTICK_MSG;

                                    pCtx->AProcCtx.ActualDataSize = LteGetSamplesSizeByte(pCtx);

                                    rc = LteRxSmpInit(&pCtx->AProcCtx, pCtx->L2HdrMode);
                                    if (FAILED (rc))
                                    {
                                        _ASSERT_RC(rc);
                                    }
                                    printf("IQs receiving from CPRI: Chunk=%d L1HdrSize=%d L2HdrSize=%d\n",
                                        pCtx->AProcCtx.DataChunkSize,
                                        pCtx->L1HdrSize,
                                        pCtx->L2HdrSize);
                                }

                                // Get timer object from system
                                if (HANDLE_NULL(PhyDataTimer))
                                {
                                    rc = MxGetObjByName(CUR_CPU, LTmrName, &PhyDataTimer);
                                    if (FAILED(rc))
                                    {
                                        PhyDataTimer = HNULL;
                                        printf ("Warning: LTmr is not found - 1\r\n");
                                    }
                                }

                                if(PhyCommIFRx)
                                {// start to Rx IQ samples right now
                                    LteRcvInSmp(PhyCommIFRx);
                                }
                            }
                            else
                            {
                                rc = 0x23108; //RC_PHY_INCORRECT_PARAM;
                                continue;
                            }
                        }
                    }
                }
                else
                {
                    rc = 0x23101; //RC_LTE_PARAM_ERROR;
                }
                break;

                case PHY_LTE_RCV_IQ_DIS:
                    if(pApi->Length == 0)
                    {// disable all by default
                        PLteCommSmpCtx pCtx;

                        pCtx = &gLteRcvInSmp[PHYCOMMIF_ETH];
                        ClearACtx(&pCtx->AProcCtx);
                        pCtx->L2HdrSize = 0;
                        pCtx->L1HdrSize = 0;
                        pCtx->TotalHdrSize = 0;
                        pCtx->AProcCtx.DataChunkSize = 0;
                        pCtx->L2HdrMode = L2HDRMODE_NO;
                        pCtx->PhyComm = 0;
                        pCtx = &gLteRcvInSmp[PHYCOMMIF_CPRI];
                        ClearACtx(&pCtx->AProcCtx);
                        pCtx->L2HdrSize = 0;
                        pCtx->L1HdrSize = 0;
                        pCtx->TotalHdrSize = 0;
                        pCtx->AProcCtx.DataChunkSize = 0;
                        pCtx->L2HdrMode = L2HDRMODE_NO;
                        pCtx->PhyComm = 0;
                        PhyCommIFRx = 0;
                        printf("\t-disabled all\n");
                    }
                    else
                    {
                        ApiParam * pParamGroup = NULL;
                        while ((pParamGroup = ApiGetNextParam (pApi, pParamGroup) ) != NULL)
                        {
                            if(pParamGroup->ParamID == PAR_COMMIQ_PARAM_GROUP)
                            {
                                PLteCommSmpCtx pCtx = NULL;

                                pParam =  ApiGetParamByID(pApi, PAR_COMMIQ_MODE);
                                if(pParam)
                                {
                                    if(pParam->ParamData[0] == (1 << PHYCOMMIF_ETH))
                                    {// 2Eth
                                        PhyCommIFRx &= ~(1 << PHYCOMMIF_ETH);
                                        pCtx = &gLteRcvInSmp[PHYCOMMIF_ETH];
                                    }
                                    else if(pParam->ParamData[0] == (1 << PHYCOMMIF_CPRI))
                                    {// 2Cpri
                                        PhyCommIFRx &= ~(1 << PHYCOMMIF_CPRI);
                                        pCtx = &gLteRcvInSmp[PHYCOMMIF_CPRI];
                                    }
                                }
                                if(pCtx)
                                {
                                    ClearACtx(&pCtx->AProcCtx);
                                    pCtx->L1HdrSize = 0;
                                    pCtx->L2HdrSize = 0;
                                    pCtx->TotalHdrSize = 0;
                                    pCtx->AProcCtx.DataChunkSize = 0;
                                    pCtx->L2HdrMode = L2HDRMODE_NO;
                                    pCtx->PhyComm = 0;
                                }
                                else
                                {
                                    rc = 0x23108; //RC_PHY_INCORRECT_PARAM;
                                    continue;
                                }
                            }
                        }
                    }
                    break;

            default:
                break;
        }
    }
    else if(pApi->Type == API_TYPE_QRY_PARAM)
    {
        switch(pApi->MessageID)
        {
            default:
                break;
        }
    }
    return rc;

}

#endif



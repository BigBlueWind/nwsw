//-------------------------------------------------------------------------------------------
/** @file LteBsApiErrorCheck.c
 *
 * @brief Api Error Check for robustness
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT&copy; 2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#include "LteBsApiErrorCheck.h"
#include "LteBsApiThread.h"
#include "LteBsRxThread.h"
#include "LteBsPhyStats.h"
#include "Debug.h"
#include "ext_phy.h"
#include "prototypes.h"
#include "ltertconfig.h"
#include "lte_entry.h"
#include "icdrv.h"
#include "mlog.h"
#include "ltertconfig.h"
#include "ceva_proc.h"

#define CEVA_API_ERROR_CHECK 1
#define CEVA_DL_FEC          1
#define CEVA_DL_BCH          1

LTE_BS_API_ERROR_CHECK_CTX CRAMVAR LteBsApiErrorCheckCtx;
LTE_BS_API_ERROR_CHECK CRAMVAR lteBsApiErrorCheck[2];

PLTE_BS_API_ERROR_CHECK_CTX LteBsApiErrorCheckGetCtx(void)
{
    return &LteBsApiErrorCheckCtx;
}

UINT32 LteBsApiErrorCheckEnable(void)
{
    PLTE_BS_API_ERROR_CHECK_CTX pLteBsApiErrorCheckCtx = LteBsApiErrorCheckGetCtx();

    _ASSERT_PTR(pLteBsApiErrorCheckCtx);
    pLteBsApiErrorCheckCtx->errorCheckEnabled = 1;

    return 0;
}

UINT32 LteBsApiErrorCheckDisable(void)
{
    PLTE_BS_API_ERROR_CHECK_CTX pLteBsApiErrorCheckCtx = LteBsApiErrorCheckGetCtx();

    _ASSERT_PTR(pLteBsApiErrorCheckCtx);
    pLteBsApiErrorCheckCtx->errorCheckEnabled = 0;

    return 0;
}


//-------------------------------------------------------------------------------------------
/** @brief This function is used to initialize Dummy vectors that will be used to continue with processing if MAC messages are not delivered on time.
 *
 *  @param pPhy  Pointer to the PHY structure
 *
 *  @return status    0 if SUCCESS
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 LtePhyApiErrorCheckInitEmptyChannels(PLTEPHYDESC pPhy)
{
    PLTE_BS_API_ERROR_CHECK_CTX pLteBsApiErrorCheckCtx = LteBsApiErrorCheckGetCtx();

    PDLCHANDESC pDlCh;
    UINT32 allocSize;

    // ----------------------------------------------------------------

    _ASSERT_PTR(pLteBsApiErrorCheckCtx);

    // Queue Element for Tx Vector
    pLteBsApiErrorCheckCtx->pTxMsg = MemAlloc(sizeof(MAC2PHY_QUEUE_EL), CRAM_MEM, 1);
    _ASSERT_PTR(pLteBsApiErrorCheckCtx->pTxMsg);

    // Empty TxVector
    allocSize =   sizeof(DLSUBFRDESC)
                - sizeof(pLteBsApiErrorCheckCtx->pEmptyTxVector->dciCh)
                - sizeof(pLteBsApiErrorCheckCtx->pEmptyTxVector->dlCh)
                + sizeof(pLteBsApiErrorCheckCtx->pEmptyTxVector->dlCh[0]);
    pLteBsApiErrorCheckCtx->pTxHeader = MemAlloc((sizeof(GENMSGDESC) + allocSize), CRAM_MEM, 1);
    _ASSERT_PTR(pLteBsApiErrorCheckCtx->pTxHeader);

    // Initialize this TxVector
    pLteBsApiErrorCheckCtx->pTxHeader->msgType      = PHY_TXSTART_REQ;
    pLteBsApiErrorCheckCtx->pTxHeader->msgSpecific  = allocSize;

    pLteBsApiErrorCheckCtx->pEmptyTxVector = (PDLSUBFRDESC)(pLteBsApiErrorCheckCtx->pTxHeader + 1);
    pLteBsApiErrorCheckCtx->pEmptyTxVector->antennaPortcount = (U8)pPhy->pBsTx->Ntx_antennas;
    pLteBsApiErrorCheckCtx->pEmptyTxVector->subframeType     = DL;
    pLteBsApiErrorCheckCtx->pEmptyTxVector->numCtrlSymbols   = 1;
    pLteBsApiErrorCheckCtx->pEmptyTxVector->reserved0        = 0xAA;


    // TODO: provide default power values if offsetPowerCtrl is not zero, the previous ones in context will be used if zero
    pLteBsApiErrorCheckCtx->pEmptyTxVector->offsetPowerCtrl = 0;//(U32)(((PUINT32) &pLteBsApiErrorCheckCtx->pEmptyTxVector->dlCmnPwrCtl) - ((PUINT32)pLteBsApiErrorCheckCtx->pEmptyTxVector));

    // Add a Dummy PBCH Channel
    pDlCh = &pLteBsApiErrorCheckCtx->pEmptyTxVector->dlCh[0];
    pDlCh->persistEnable                = 1;
    pDlCh->repeatCycle                  = 1;
    pDlCh->channelId                    = 0;
    pDlCh->channelType                  = PBCH;
    pDlCh->hCid                         = 0;
    pDlCh->numCodeWords                 = 1;
    pDlCh->nLayers                      = ((pPhy->pBsTx->Ntx_antennas == 1) ? SINGLEANT : TXDIVERSITY);
    pDlCh->transmissionMode             = ((pPhy->pBsTx->Ntx_antennas == 1) ? SINGLEANT : TXDIVERSITY);
    pDlCh->dlPrecoderInfo.cddType       = 0;
    pDlCh->subChInfo[0].modulationType  = 1;
    pDlCh->subChInfo[1].modulationType  = 0;

    // ----------------------------------------------------------------


    // ----------------------------------------------------------------


    // Queue Element for Rx Vector
    pLteBsApiErrorCheckCtx->pRxMsg = MemAlloc(sizeof(MAC2PHY_QUEUE_EL), CRAM_MEM, 1);
    _ASSERT_PTR(pLteBsApiErrorCheckCtx->pRxMsg);

    // Empty RxVector
    allocSize =   sizeof(ULSUBFRDESC)
                - sizeof(pLteBsApiErrorCheckCtx->pEmptyRxVector->ulCh)
                - sizeof(pLteBsApiErrorCheckCtx->pEmptyRxVector->ulCtlCh)
                + sizeof(pLteBsApiErrorCheckCtx->pEmptyRxVector->srsInfo);
    pLteBsApiErrorCheckCtx->pRxHeader = MemAlloc((sizeof(GENMSGDESC) + allocSize), CRAM_MEM, 1);
    _ASSERT_PTR(pLteBsApiErrorCheckCtx->pRxHeader);

    pLteBsApiErrorCheckCtx->pRxHeader->msgType          = PHY_RXSTART_REQ;
    pLteBsApiErrorCheckCtx->pRxHeader->msgSpecific      = allocSize;

    pLteBsApiErrorCheckCtx->pEmptyRxVector = (PULSUBFRDESC)(pLteBsApiErrorCheckCtx->pRxHeader + 1);
    pLteBsApiErrorCheckCtx->pEmptyRxVector->antennaPortcount     = (U8)pPhy->pBsRx->Nrx_antennas;
    pLteBsApiErrorCheckCtx->pEmptyRxVector->subframeType         = UL;

    // TODO: provide default rach values if offsetRachCtrlStruct is not zero, rach detection is disabled if zero
    pLteBsApiErrorCheckCtx->pEmptyRxVector->offsetRachCtrlStruct = 0;
    pLteBsApiErrorCheckCtx->pEmptyRxVector->ulSfrCtrl.pad        = 0x155;
    pLteBsApiErrorCheckCtx->pEmptyRxVector->ulSfrCtrl.padding    = 0x555;

    // ----------------------------------------------------------------


    // Set Pointers
    pLteBsApiErrorCheckCtx->pTxMsg->MessagePtr = (PUINT8)pLteBsApiErrorCheckCtx->pTxHeader;                 // Queue Item = Empty TxVector
    pLteBsApiErrorCheckCtx->pRxMsg->MessagePtr = (PUINT8)pLteBsApiErrorCheckCtx->pRxHeader;                 // Queue Item = Empty RxVector

    pLteBsApiErrorCheckCtx->pTxMsg->Next = pLteBsApiErrorCheckCtx->pRxMsg;                                  // Next Queue element is RxVector
    pLteBsApiErrorCheckCtx->pRxMsg->Next = NULL;                                                            // There is no other queue element


    pLteBsApiErrorCheckCtx->pgFirstMessageArrived = (PUINT32)&gFirstMessageArrived;
    pLteBsApiErrorCheckCtx->pgLastTxSfn           = (PUINT32)&gLastTxSfn;
    pLteBsApiErrorCheckCtx->pgLastRxSfn           = (PUINT32)&gLastRxSfn;
    pLteBsApiErrorCheckCtx->pgTTIPauseResumeCount = (PUINT32)&gTTIPauseResumeCount;

    return 0;
}


//-------------------------------------------------------------------------------------------
/** @brief This function is used to destroy Dummy vectors that were malloced at init
 *
 *  @param none
 *
 *  @return status    0 if SUCCESS
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 LtePhyApiErrorCheckDestroyEmptyChannels(void)
{
    PLTE_BS_API_ERROR_CHECK_CTX pLteBsApiErrorCheckCtx = LteBsApiErrorCheckGetCtx();

    _ASSERT_PTR(pLteBsApiErrorCheckCtx);

    if (pLteBsApiErrorCheckCtx->pTxMsg)
    {
        MemFree(pLteBsApiErrorCheckCtx->pTxMsg, CRAM_MEM);
        pLteBsApiErrorCheckCtx->pTxMsg = NULL;
    }
    if (pLteBsApiErrorCheckCtx->pTxHeader)
    {
        MemFree(pLteBsApiErrorCheckCtx->pTxHeader, CRAM_MEM);
        pLteBsApiErrorCheckCtx->pTxHeader = NULL;
    }
    if (pLteBsApiErrorCheckCtx->pRxMsg)
    {
        MemFree(pLteBsApiErrorCheckCtx->pRxMsg, CRAM_MEM);
        pLteBsApiErrorCheckCtx->pRxMsg = NULL;
    }
    if (pLteBsApiErrorCheckCtx->pRxHeader)
    {
        MemFree(pLteBsApiErrorCheckCtx->pRxHeader, CRAM_MEM);
        pLteBsApiErrorCheckCtx->pRxHeader = NULL;
    }

    return 0;
}




static UINT32 tSetBuf;
void SetupSymbolsDone(void* Ptr, UINT32 ceva_ret)
{
    PLTE_PHY_SDU_DESC pSduDesc = (PLTE_PHY_SDU_DESC)Ptr;

    if (ceva_ret != 0)
    {
        stop_printf("Something is wrong with Ceva function SetupSymbols retCode: 0x%x\r\n", ceva_ret);
        return;
    }

    *pSduDesc->pIsExeTaskListReady = TRUE;

    UnlockModTcb(UNLOCK_DLSCH_TCB_SETUPSYM_DONE, pSduDesc);

    MLogTask(TASKID_TX_SETUP_SYMB_BUFS_WRAP, RESOURCE_LARM, tSetBuf, GET_TICKS);
}


void LteBsTxSduProcessingRt_cb(void* ptr, UINT32 ceva_ret)
{
    PLTE_PHY_SDU_DESC pSduDesc = (PLTE_PHY_SDU_DESC)ptr;
    if (ceva_ret != 0 || ptr == NULL)
    {
        stop_printf("Something is wrong with Ceva function LteBsTxSduProcessingRt retCode: 0x%x, ptr: %x\r\n", ceva_ret, ptr);
        return;
    }

    pSduDesc->dlFecDescriptorDone = 1;
    DispatchDlFec(pSduDesc);
}

void BCH_cb(void* ptr, UINT32 ceva_ret)
{
    PLTE_PHY_SDU_DESC pSduDesc = (PLTE_PHY_SDU_DESC)ptr;
    if (ceva_ret != 0 || ptr == NULL)
    {
        stop_printf("Something is wrong with Ceva BCH function retCode: 0x%x, ptr: %x\r\n", ceva_ret, ptr);
        return;
    }

    if (((pSduDesc->pTxInputParams->txFrameNumber & 0x3) != 0) || pSduDesc->gPhyInternalBch)
        UnlockModTcb(UNLOCK_DLSCH_TCB_BCH_COMPLETE, pSduDesc);

    MLogTask(PID_APITHREAD_START_PBCH_CEVA, RESOURCE_LARM, pSduDesc->t_arm, GET_TICKS);
}









volatile UINT32 LteBsApiErrorCheckDispatchRunning = 0;


UINT32 LteBsApiErrorCheckDispatch(PMAC2PHY_QUEUE_EL pMsg, UINT32 isDummyRunning, UINT32 flag)
{
    PLTE_BS_API_ERROR_CHECK_CTX pLteBsApiErrorCheckCtx  = LteBsApiErrorCheckGetCtx();
    PLTE_BS_PHY_STATS pLteBsPhyStats                    = LteBsPhyStatsGetCtx();
    PLTE_BS_API_ERROR_CHECK pLteBsApiErrorCheck;
    PLTE_PHY_SDU_DESC pSduDesc;
    UINT32 t = GET_TICKS;

#ifdef CEVA_API_ERROR_CHECK
    U32 hApiErrorCheck;
#endif

    gCtxDlFec = 1 - gCtxDlFec;

    pLteBsApiErrorCheck                         = &lteBsApiErrorCheck[gCtxDlFec];
    pSduDesc                                    = gpSduDesc[gCtxDlFec];
    pSduDesc->sduState                          = SDU_NEW;
    pSduDesc->dlFecDescriptorDone               = 0;
    pSduDesc->dlCrcDone                         = 0;

    pLteBsApiErrorCheckCtx->pApiList            = pMsg;
    pLteBsApiErrorCheckCtx->pLteBsApiErrorCheck = pLteBsApiErrorCheck;
    pLteBsApiErrorCheckCtx->isDummy             = isDummyRunning;
    pLteBsApiErrorCheckCtx->isInternalBch       = gPhyInternalBch;
    pLteBsApiErrorCheckCtx->pSduDesc            = pSduDesc;
    pSduDesc->numSDU                            = 0;

    pLteBsApiErrorCheckCtx->lastTxSfn = LteBsIncrementSfn(pLteBsApiErrorCheckCtx->lastTxSfn);

#ifndef LTE_BS_API_ERROR_CHECK_ROBUSTNESS_ENABLED
    pLteBsApiErrorCheckCtx->robustControlEnabled = 0;
#else
    pLteBsApiErrorCheckCtx->robustControlEnabled = 1;
#endif

    if (LteBsApiErrorCheckDispatchRunning)
    {
        stop_printf("LteBsApiErrorCheckDispatchRunning Already Running\r\n");
        return 0;
    }

    LteBsApiErrorCheckDispatchRunning = 1;

#ifndef CEVA_API_ERROR_CHECK
    LteBsStartTtiProcessing((UINT32)pLteBsApiErrorCheckCtx, (UINT32)pSduDesc);
    LteBsApiErrorCheck_CB(NULL, 0);
#else
    hApiErrorCheck = ceva_call2_cb(LteBsApiErrorCheck_CB, NULL, ceva_LteBsStartTtiProcessing, (UINT32)pLteBsApiErrorCheckCtx, (UINT32)pSduDesc);
    if(!hApiErrorCheck)
    {
        stop_printf("Failed to start Api Error Check function on Ceva\r\n");
    }
#endif

    if (flag)
        LteBsRxThreadLowLatencyPush(pMsg, isDummyRunning, pLteBsApiErrorCheck);

    if (gLowLatencyFlag && !isDummyRunning)
    {
        if (pLteBsApiErrorCheckCtx->successiveNoFifo)
        {
            LteBsPhyStatsNumConsecutiveDummyApis(pLteBsPhyStats, (pLteBsApiErrorCheckCtx->successiveNoFifo > 10 ? 10 : pLteBsApiErrorCheckCtx->successiveNoFifo-1));
            pLteBsApiErrorCheckCtx->successiveNoFifo = 0;
        }
    }

    MLogTask(PID_API_ERRORCHECK_COMPLETE, RESOURCE_LARM, t, GET_TICKS);

    return 0;
}


void LteBsApiErrorCheck_CB(void* ptr, UINT32 ceva_ret)
{
    PLTE_BS_API_ERROR_CHECK_CTX pLteBsApiErrorCheckCtx  = LteBsApiErrorCheckGetCtx();
    PLTE_BS_PHY_STATS pLteBsPhyStats                    = LteBsPhyStatsGetCtx();
    PMAC2PHY_QUEUE_EL pApiList                          = pLteBsApiErrorCheckCtx->pApiList;
    PLTE_PHY_SDU_DESC pSduDesc                          = pLteBsApiErrorCheckCtx->pSduDesc;
    PDLTXINPUTPARAMS pTxInputParams                     = pSduDesc->pTxInputParams;
    PLTE_BS_API_ERROR_CHECK pLteBsApiErrorCheck         = pLteBsApiErrorCheckCtx->pLteBsApiErrorCheck;

    PMAC2PHY_QUEUE_EL pCurrMsg;
    PGENMSGDESC pMsgHeader;
    PTXSDUREQ pTxSduReq;

    PHIADCIULMSGDESC pHiDciMsg = NULL;
    PDCIULSDUMSG pDciUlSduMsg = NULL;
    UINT32 numBytes = 0, txSfn, rxSfn;
    UINT32 chanId = 0;
    UINT32 hDLFEC_Setup;

    UINT32 tTotal = GET_TICKS;
    UINT32 t = tTotal;

    if (*pLteBsApiErrorCheckCtx->pgFirstMessageArrived && pLteBsApiErrorCheckCtx->errorCheckEnabled)
    {
        if (ceva_ret && LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_TMRSTS))
        {
            LteBsPhyStatsBadApiMessages(pLteBsPhyStats);
            LteBsPhyStatsBadApiMask(pLteBsPhyStats, ceva_ret);
        }

        if (!pLteBsApiErrorCheckCtx->robustControlEnabled)
        {
            // IF robustness is not enabled, then we just Force Log all the APIs and stop PHY
            if (ceva_ret)
            {
                pCurrMsg = pApiList;

                txSfn = LteBsGetSubFrameNum(pLteBsApiErrorCheckCtx->lastTxSfn);
                rxSfn = LteBsGetSubFrameNum(pLteBsApiErrorCheckCtx->lastRxSfn);

                while (pCurrMsg)
                {
                    pMsgHeader = (PGENMSGDESC) pCurrMsg->MessagePtr;

                    switch (pMsgHeader->msgType)
                    {
                        case PHY_TXSTART_REQ:
                            SvsrLoggerAddMsgForce (PHY_TXSTART_REQ, LOGGER_MSG_INFO(0, txSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                        break;

                        case PHY_RXSTART_REQ:
                            SvsrLoggerAddMsgForce (PHY_RXSTART_REQ, LOGGER_MSG_INFO(0, rxSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                        break;

                        case PHY_TXHIADCIUL_REQ:
                            pHiDciMsg = (PHIADCIULMSGDESC)pMsgHeader;
                            numBytes = pHiDciMsg->msgLength;
                            SvsrLoggerAddMsgForce (PHY_TXHIADCIUL_REQ, LOGGER_MSG_INFO(0, pHiDciMsg->subFrameNumber, IID_LTE_MAC, IID_LTE), pMsgHeader, (numBytes + sizeof (HIADCIULMSGDESC)));
                        break;

                        case PHY_TXHISDU_REQ:
                            SvsrLoggerAddMsgForce (PHY_TXHISDU_REQ, LOGGER_MSG_INFO(0, txSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, sizeof (HIINFOMSGDESC));
                        break;

                        case PHY_TXSDU_REQ:
                            pTxSduReq = (PTXSDUREQ) pMsgHeader;
                            numBytes = pTxSduReq->msgLen;
                            SvsrLoggerAddMsgForce (PHY_TXSDU_REQ, LOGGER_MSG_INFO(0, txSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, (10 + sizeof (TXSDUREQ)));
                        break;

                        case PHY_TXDCIULSDU_REQ:
                            pDciUlSduMsg = (PDCIULSDUMSG) pMsgHeader;
                            numBytes = pDciUlSduMsg->msgLength;
                            SvsrLoggerAddMsgForce (PHY_TXDCIULSDU_REQ, LOGGER_MSG_INFO(0, txSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, (numBytes + sizeof (DCIULSDUMSG)));
                        break;

                        default:
                        break;
                    }

                    pCurrMsg = pCurrMsg->Next;
                }

                stop_printf("Something wrong in API: 0x%08x\r\n", ceva_ret);
                return;
            }
        }
    }

    // Dispatch DL Stuff right now
    //-------------------------

    // Setup Symbols
    t = GET_TICKS;
    tSetBuf = t;
    if(!ceva_call1_cb(SetupSymbolsDone, pSduDesc, ceva_SetupSymbDataInpBufs_task_wrap, (UINT32) pSduDesc->pSetupBufsTCB))
    {
        stop_printf("Failed to start SetupSymbDataInpBufs function on Ceva\r\n");
        return;
    }
    MLogTask(PID_TXLIST_UNLOCK_SETUP_BUFS, RESOURCE_LARM, t, GET_TICKS);

    // BCH
    t = GET_TICKS;
    pSduDesc->scrInit = (pTxInputParams->subframeNumber << 9) + pTxInputParams->Ncell_id;
    if(!pTxInputParams->subframeNumber)
    {
        chanId                              = pTxInputParams->numChan - 1;

        if (gPhyInternalBch == 0)
        {
            pTxSduReq                       = (PTXSDUREQ) pLteBsApiErrorCheck->pBchSdu;
            pSduDesc->pBchSdu               = (gUseTxSduPointer)? (U8*)pTxSduReq->pTxSdu : ((U8*) pTxSduReq + sizeof(TXSDUREQ));
            pSduDesc->bchNumBytes           = pTxSduReq->msgLen;
        }

        pSduDesc->pBchScrOut                = pSduDesc->ScrOutBufs[0];
        pSduDesc->ScrIndex[0][chanId]       = pSduDesc->scrsize[0];
        pSduDesc->totalsizescr[0]          += RUP32B(PBCHDATASIZE/8);
        pSduDesc->scrsize[0]               += RUP32B(PBCHDATASIZE/8);

#ifdef CEVA_DL_BCH
        pSduDesc->t_arm = GET_TICKS;
        hDLFEC_Setup = ceva_call1_cb(BCH_cb, pSduDesc, ceva_InsertPBCH_RT, (U32)pSduDesc);
        if(!hDLFEC_Setup)
        {
            stop_printf("Failed to start BCH function on Ceva\r\n");
            return;
        }
#else
        InsertPBCH_RT((U32)pSduDesc);
        BCH_cb(pSduDesc, 0);
#endif

    }
    else
    {
        UnlockModTcb(UNLOCK_DLSCH_TCB_BCH_COMPLETE, pSduDesc);
    }
    MLogTask(PID_TXSDU_BCH, RESOURCE_LARM, t, GET_TICKS);

    // Tx PDSCH SDU Processing
    t = GET_TICKS;
    if(pSduDesc->numSDU != 0)
    {
        pSduDesc->startTime = tTotal;

#ifdef CEVA_DL_FEC

        hDLFEC_Setup = ceva_call1_cb(LteBsTxSduProcessingRt_cb, pSduDesc, ceva_LteBsTxSduProcessingRT, (U32)pSduDesc);
        if(!hDLFEC_Setup)
        {
            stop_printf("Failed to start DL FEC setup function on Ceva\r\n");
            return;
        }
#else
        LteBsTxSduProcessingRT((U32)pSduDesc);
#endif
    }
    else
    {
        UnlockModTcb(UNLOCK_DLSCH_TCB_PDSCH_DONE, pSduDesc);
    }
    MLogTask(PID_TXSDU_DISPATCH_DATASDU, RESOURCE_LARM, t, GET_TICKS);

    if (!gLowLatencyFlag)
        LtePhyScheduleApiThread(pLteBsApiErrorCheckCtx->pApiList, pLteBsApiErrorCheckCtx->isDummy, pLteBsApiErrorCheckCtx->pLteBsApiErrorCheck);
    else
    {
        if (pLteBsApiErrorCheckCtx->isDummy)
            LtePhyScheduleApiThread(pLteBsApiErrorCheckCtx->pApiList, pLteBsApiErrorCheckCtx->isDummy, pLteBsApiErrorCheckCtx->pLteBsApiErrorCheck);
    }

    LteBsApiErrorCheckDispatchRunning = 0;

    MLogTask(PID_API_ERRORCHECK_CB, RESOURCE_LARM, tTotal, GET_TICKS);

    return;
}




UINT32 LteBsApiErrorCheckSfn(PLTE_BS_API_ERROR_CHECK_CTX pLteBsApiErrorCheckCtx, UINT32 currSfn, UINT32 direction )
{
    UINT32 retVal = LTE_BS_API_ERROR_CHECK_DEFAULT_RET;
    UINT32 prevSfn = 0;

    // Set context
    if (direction == DL)
    {
        prevSfn = pLteBsApiErrorCheckCtx->lastTxSfn;
    }
    else if (direction == UL)
    {
        prevSfn = pLteBsApiErrorCheckCtx->lastRxSfn;
    }
    else
    {
        return retVal;
    }

    // Compare
    if (currSfn >= prevSfn)
    {
        retVal = ((currSfn - prevSfn == 1) ? LTE_BS_API_ERROR_CHECK_SUCCESS : LTE_BS_API_ERROR_CHECK_SFN_CHECK_FAIL);
    }
    else
    {
        retVal = ((currSfn + 10240 - prevSfn == 1) ? LTE_BS_API_ERROR_CHECK_SUCCESS : LTE_BS_API_ERROR_CHECK_SFN_CHECK_FAIL);
    }

    return retVal;
}

U32 LteBsApiErrorCheckEnableInterfaces(U32 phyCfg)
{
    gPhyRobustControl        =  ((phyCfg & PHYINIT_ROBUST_CONTROL) ? 1 : 0);
    gPhyInternalBch          =  ((phyCfg & PHYINIT_PHY_MAINTAINS_PBCH) ? 1 : 0);
    gUseTxSduPointer         =  ((phyCfg & PHYINIT_USE_TXSDU_POINTER) ? 1 : 0);
    gTwoRxMuxControlLists    =  ((phyCfg & PHYINIT_TWO_MUXCONTROL_LISTS) ? 1 : 0);
    gSendSecondTTIIndication =  ((phyCfg & PHYINIT_PHY_SEND_SECOND_TTI_IND) ? 1 : 0);
    gMuteRxSduForSrRach      =  ((phyCfg & PHYINIT_MUTE_RXSDU_FOR_SR_RACH) ? 1 : 0);
    gLowLatencyFlag          =  ((phyCfg & PHYINIT_LOW_LATENCY_PATH) ? 1 : 0);

    uart_printf("Interfaces Enabled:\r\n");
    uart_printf("--------------------\r\n\n");
    uart_printf("phyCfg                                    0x%08x:\r\n", phyCfg);
    uart_printf("PHYINIT_ROBUST_CONTROL:                   %s\r\n", (gPhyRobustControl ? "YES":"NO"));
    uart_printf("PHYINIT_PHY_MAINTAINS_PBCH:               %s\r\n", (gPhyInternalBch ? "YES":"NO"));
    uart_printf("PHYINIT_USE_TXSDU_POINTER:                %s\r\n", (gUseTxSduPointer ? "YES":"NO"));
    uart_printf("PHYINIT_TWO_MUXCONTROL_LISTS:             %s\r\n", (gTwoRxMuxControlLists ? "YES":"NO"));
    uart_printf("PHYINIT_PHY_SEND_SECOND_TTI_IND:          %s\r\n", (gSendSecondTTIIndication ? "YES":"NO"));
    uart_printf("PHYINIT_MUTE_RXSDU_FOR_SR_RACH:           %s\r\n", (gMuteRxSduForSrRach ? "YES":"NO"));
    uart_printf("PHYINIT_LOW_LATENCY_PATH:                 %s\r\n", (gLowLatencyFlag ? "YES":"NO"));
    uart_printf("\r\n");

    return 0;
}


UINT32 LteBsApiErrorCheckScanApi(PMAC2PHY_QUEUE_EL pListHeader, UINT32* pNumApi, UINT32* pNumTxSdu, PDLSUBFRDESC* ppTxVector)
{
    PMAC2PHY_QUEUE_EL curr;
    PGENMSGDESC pMsgHeader;
    PINITPARM pInit;
    PPHY_LOGGS_ENABLE pPhyLogsEnable;

    U32 numApis, numApisParsed;
    U32 phyInstance = 0;
    U32 OutSize = 0, status = 0;
    U32 i;
    U32 t, killPhy = 0, msgTypes = 0;
    UINT32 phyDiState = PhyDiQueryState();

#if 0
    PTXSDUREQ pTxSduReq;

    PHIADCIULMSGDESC pHiDciMsg = NULL;
    PDCIULSDUMSG pDciUlSduMsg = NULL;
    UINT32 numBytes = 0, txSfn, rxSfn;
#endif

    //  Parse messages
    numApis = numApisParsed = 0;

    curr = pListHeader;
    while (curr)
    {
        pMsgHeader = (PGENMSGDESC) curr->MessagePtr;

        msgTypes |= 1<<((pMsgHeader->msgType > 31) ? 31 : pMsgHeader->msgType);

        switch (pMsgHeader->msgType)
        {
            case PHY_INIT_REQ:
                numApisParsed++;

                if (phyDiState == PHY_STATE_START)
                {
                    killPhy = 1;
                    break;
                }

                gTTIPauseResumeCount = 0;
                gStartSubfrTimeMark = 0;
                gFirstMessageArrived = 0;

                LtePhyStartApiFree();

                // Process INIT_REQ

                ICDrvEnableIrqTrace (1);

                pInit = (PINITPARM) (pMsgHeader+1);

                LteBsApiErrorCheckEnableInterfaces(pInit->phyCfg);

                memcpy((void*)&gStoredInitParm, (PINITPARM) (pMsgHeader+1), sizeof(INITPARM));

                status = LteBsPhyInit(phyInstance, (PINITPARM) (pMsgHeader+1));

#if API_COPY_THREAD_ARM != 0
                LtePhyInitApiLogThread();
#endif
                gMode = INIT_MODE;

                // Once the initialization has completed issue the PHY_INIT.indication
                t = GET_TICKS;
                LteGenerateAPIMsg(phyInstance, NULL, &OutSize, PHY_INIT_IND, status);
                MLogTask(PID_APITHREAD_PHY_INIT, RESOURCE_LARM, t, GET_TICKS);
            break;

            case PHY_START_REQ:
                numApisParsed++;

                if (phyDiState == PHY_STATE_START)
                {
                    killPhy = 1;
                    break;
                }

                LtePhyStartApiFree();

                if (gMode == NMM_MODE)
                {// hope that NMM operation is stopped, reconfigure PHY before PHY start
                    U32 OutSize = 0;
                
                    LtePhyApiThreadStopTimer();

                    // Clean up for next start
                    gStartSubfrTimeMark = 0;
                    gFirstMessageArrived = 0;
                    LtePhyStartApiFree();
                
                    // Shutdown PHY
                    PhyDiShutdown(&OutSize);
                
                    // Re-initialize the Phy
                    gTTIPauseResumeCount = 0;
                
                    // Process INIT_REQ
                    uart_printf("\r\nRe-Configuring the PHY\r\n");
                    uart_printf(    "----------------------\r\n\r\n");
                
                    LteBsApiErrorCheckEnableInterfaces(gStoredInitParm.phyCfg);
                
                    LteBsPhyInit(0, (PINITPARM)&gStoredInitParm);
                }

                for (i = 0; i < NUM_UL_SDU_CONTEXT; i++)
                    gRxSduListState[i] = RXSDU_LIST_STATE__CLEAN;

                // Initialize variables
                _my_sfn = 0;
                gFirstMessageArrived = 0;

                LteBsApiThreadEnableLogs();

                LteBsApiThreadSetupMapVars();

                gMode = LTE_MODE;
                LteStart(phyInstance, (PSTARTREQ)pMsgHeader, gDefaultIqEnableMask);

                t = GET_TICKS;
                LteGenerateAPIMsg(phyInstance, NULL, &OutSize, PHY_START_CONF, 0);
                MLogTask(PID_APITHREAD_PHY_START, RESOURCE_LARM, t, GET_TICKS);
            break;

            case PHY_STOP_REQ:
                numApisParsed++;

                LtePhyApiThreadStopTimer();

                /* do pause fist to ignore irqs before stop  */
                if (PhyDiQueryMode() == AUTO_TIMER)
                {
                    DiPause(NULL);
                }
                else if (PhyDiQueryMode() == AUTO_PCIE)
                {
                    PhyDiSetPauseResume(1, getPhyCtxListIndx(0));
                }
                else
                {
                    uart_printf("PHYDI: This mode doesn't support PAUSE/RESUME\n");
                }
                LtePhyStop(NULL, NULL);

                t = GET_TICKS;
                LteGenerateAPIMsg(phyInstance, NULL, &OutSize, PHY_STOP_CONF, 0);
                MLogTask(PID_APITHREAD_PHY_STOP, RESOURCE_LARM, t, GET_TICKS);
            break;

            case PHY_PAUSE_REQ:
                numApisParsed++;

                if (phyDiState == PHY_STATE_START)
                {
                    killPhy = 1;
                    break;
                }

                t = GET_TICKS;
                LtePhyApiThreadStopTimer();

                if (PhyDiQueryMode() == AUTO_TIMER)
                {
                    DiPause(NULL);
                }
                else if (PhyDiQueryMode() == AUTO_PCIE)
                {
                    PhyDiSetPauseResume(1, 0);
                }
                else
                {
                    uart_printf("PHYDI: This mode doesn't syupport PAUSE/RESUME\n");
                }
                MLogTask(PID_APITHREAD_PHY_PAUSE, RESOURCE_LARM, t, GET_TICKS);

                gStartSubfrTimeMark = 0;
                uart_printf("PHYDI-PAUSE: Last TTI was %d\n", gTTIPauseResumeCount);
            break;

            case PHY_RESUME_REQ:
                numApisParsed++;

                if (phyDiState == PHY_STATE_START)
                {
                    killPhy = 1;
                    break;
                }

                uart_printf("PHYDI-RESUME: Current TTI is %d\n", gTTIPauseResumeCount);

                t = GET_TICKS;
                if (PhyDiQueryMode() == AUTO_TIMER)
                {
                    DiResume(NULL);
                }
                else if (PhyDiQueryMode() == AUTO_PCIE)
                {
                    PhyDiSetPauseResume(0, 5);
                }
                else
                {
                    uart_printf("PHYDI: This mode doesn't syupport PAUSE/RESUME\n");
                }
                MLogTask(PID_APITHREAD_PHY_RESUME, RESOURCE_LARM, t, GET_TICKS);
                gStartSubfrTimeMark = 0;
            break;


            case PHY_SHUTDOWN_REQ:
                numApisParsed++;

                LtePhyApiThreadStopTimer();
                gStartSubfrTimeMark = 0;

                LtePhyShutdown(NULL, NULL);

                t = GET_TICKS;
                LteGenerateAPIMsg(phyInstance, NULL, &OutSize, PHY_SHUTDOWN_CONF, 0);
                MLogTask(PID_APITHREAD_PHY_SHUTDOWN, RESOURCE_LARM, t, GET_TICKS);
            break;

            case PHY_RECONFIG_REQ:
                numApisParsed++;

                if (phyDiState == PHY_STATE_START)
                {
                    killPhy = 1;
                    break;
                }

                LtePhyApiThreadStopTimer();

                // Clean up for next start
                gStartSubfrTimeMark = 0;
                gFirstMessageArrived = 0;
                LtePhyStartApiFree();

                // Shutdown PHY
                PhyDiShutdown(&OutSize);


                // Re-initialize the Phy
                gTTIPauseResumeCount = 0;

                gMode = INIT_MODE;

                // Process INIT_REQ
                uart_printf("\r\nRe-Configuring the PHY\r\n");
                uart_printf(    "----------------------\r\n\r\n");

                pInit = (PINITPARM) (pMsgHeader+1);

                memcpy((void*)&gStoredInitParm, pInit, sizeof(INITPARM));

                LteBsApiErrorCheckEnableInterfaces(pInit->phyCfg);

                status = LteBsPhyInit(phyInstance, pInit);

                // Send Re-config Confirmation message to MAC
                t = GET_TICKS;
                LteGenerateAPIMsg(phyInstance, NULL, &OutSize, PHY_RECONFIG_CNF, status);
                MLogTask(PID_APITHREAD_PHY_RECONFIG, RESOURCE_LARM, t, GET_TICKS);
            break;

            case PHY_LOGS_ENABLE_REQ:
                numApisParsed++;

                if (phyDiState != PHY_STATE_START)
                {

                    pPhyLogsEnable = (PPHY_LOGGS_ENABLE)(pMsgHeader+1);

                    if (pPhyLogsEnable->mlogMask)
                    {
                        LtePhyLogsAddMask(LTE_BS_PHY_ENABLE_MLOG);
                    }
                    else
                    {
                        LtePhyLogsRemoveMask(LTE_BS_PHY_ENABLE_MLOG);
                    }

                    if (pPhyLogsEnable->svsrLoggerMask)
                    {
                        LtePhyLogsAddMask(LTE_BS_PHY_ENABLE_APILOG);
                        gDefaultApiLoggingMask          = pPhyLogsEnable->svsrLoggerMask;
                        gDefaultApiLoggingResetCounter  = pPhyLogsEnable->svsrLoggerResetCounter;
                        gDefaultApiLoggingTxSduMaxSize  = pPhyLogsEnable->svsrLoggerMaxTxSduLoggingSize;
                        gDefaultApiLoggingRxSduMaxSize  = pPhyLogsEnable->svsrLoggerMaxRxSduLoggingSize;
                        if (pPhyLogsEnable->svsrLoggerCoreId != API_COPY_THREAD_ARM)
                        {
                            if (LtePhyInitApiLogThreadSecond() == MX_OK)
                                gDefaultApiLoggingCore      = pPhyLogsEnable->svsrLoggerCoreId;
                        }
                    }
                    else
                    {
                        LtePhyLogsRemoveMask(LTE_BS_PHY_ENABLE_APILOG);
                    }

                    if (pPhyLogsEnable->iqSamplesEnableMask)
                    {
                        LtePhyLogsAddMask(LTE_BS_PHY_ENABLE_IQLOGS);
                        gDefaultIqEnableMask = pPhyLogsEnable->iqSamplesEnableMask;
                    }
                    else
                    {
                        LtePhyLogsRemoveMask(LTE_BS_PHY_ENABLE_IQLOGS);
                    }


                    if (pPhyLogsEnable->apiErrorCheckEnable)
                    {
                        LtePhyLogsAddMask(LTE_BS_PHY_ENABLE_APIERRORCHECK);
                    }
                    else
                    {
                        LtePhyLogsRemoveMask(LTE_BS_PHY_ENABLE_APIERRORCHECK);
                    }


                    if (pPhyLogsEnable->phyStatsMask)
                    {
                        LtePhyLogsAddMask(LTE_BS_PHY_ENABLE_PHYSTATS);
                        gDefaultPhyStatsEnableMask = pPhyLogsEnable->phyStatsMask;
                    }
                    else
                    {
                        LtePhyLogsRemoveMask(LTE_BS_PHY_ENABLE_PHYSTATS);
                    }

                    LteBsApiThreadEnableLogs();
                }

                // Send Phy Logs Enable Confirmation message to MAC
                t = GET_TICKS;
                LteGenerateAPIMsg(phyInstance, NULL, &OutSize, PHY_LOGS_ENABLE_IND, 0);
                MLogTask(PID_APITHREAD_PHY_LOGS_ENABLE, RESOURCE_LARM, t, GET_TICKS);

            break;

            case PHY_TXSTART_REQ:
                *ppTxVector = (PDLSUBFRDESC) (pMsgHeader + 1);
            break;

            case PHY_TXSDU_REQ:
                *pNumTxSdu++;
            break;

            default:
            break;
        }

        curr = curr->Next;
        numApis++;
    }

    if (killPhy)
    {
        stop_printf("Some BadApis are present in List after PhyStart. Types: %x. numApis: %d numApisParsed: %d\r\n", msgTypes, numApis, numApisParsed);

#if 0
        curr = pListHeader;

        txSfn = LteBsGetSubFrameNum(gLastTxSfn);
        rxSfn = LteBsGetSubFrameNum(gLastRxSfn);

        while (curr)
        {
            pMsgHeader = (PGENMSGDESC) curr->MessagePtr;

            switch (pMsgHeader->msgType)
            {
                case PHY_TXSTART_REQ:
                    SvsrLoggerAddMsgForce (PHY_TXSTART_REQ, LOGGER_MSG_INFO(0, txSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                break;

                case PHY_RXSTART_REQ:
                    SvsrLoggerAddMsgForce (PHY_RXSTART_REQ, LOGGER_MSG_INFO(0, rxSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                break;

                case PHY_TXHIADCIUL_REQ:
                    pHiDciMsg = (PHIADCIULMSGDESC)pMsgHeader;
                    numBytes = pHiDciMsg->msgLength;
                    SvsrLoggerAddMsgForce (PHY_TXHIADCIUL_REQ, LOGGER_MSG_INFO(0, pHiDciMsg->subFrameNumber, IID_LTE_MAC, IID_LTE), pMsgHeader, (numBytes + sizeof (HIADCIULMSGDESC)));
                break;

                case PHY_TXHISDU_REQ:
                    SvsrLoggerAddMsgForce (PHY_TXHISDU_REQ, LOGGER_MSG_INFO(0, txSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, sizeof (HIINFOMSGDESC));
                break;

                case PHY_TXSDU_REQ:
                    pTxSduReq = (PTXSDUREQ) pMsgHeader;
                    numBytes = pTxSduReq->msgLen;
                    SvsrLoggerAddMsgForce (PHY_TXSDU_REQ, LOGGER_MSG_INFO(0, txSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, (10 + sizeof (TXSDUREQ)));
                break;

                case PHY_TXDCIULSDU_REQ:
                    pDciUlSduMsg = (PDCIULSDUMSG) pMsgHeader;
                    numBytes = pDciUlSduMsg->msgLength;
                    SvsrLoggerAddMsgForce (PHY_TXDCIULSDU_REQ, LOGGER_MSG_INFO(0, txSfn, IID_LTE_MAC, IID_LTE), pMsgHeader, (numBytes + sizeof (DCIULSDUMSG)));
                break;

                case PHY_INIT_REQ:
                    SvsrLoggerAddMsgForce (PHY_INIT_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof (GENMSGDESC)));
                break;

                case PHY_START_REQ:
                    SvsrLoggerAddMsgForce (PHY_START_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, sizeof(STARTREQ));
                    break;

                case PHY_STOP_REQ:
                    SvsrLoggerAddMsgForce (PHY_STOP_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                break;

                case PHY_PAUSE_REQ:
                    SvsrLoggerAddMsgForce (PHY_PAUSE_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                break;

                case PHY_RESUME_REQ:
                    SvsrLoggerAddMsgForce (PHY_RESUME_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                break;

                case PHY_SHUTDOWN_REQ:
                    SvsrLoggerAddMsgForce (PHY_SHUTDOWN_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                break;

                case PHY_RECONFIG_REQ:
                    SvsrLoggerAddMsgForce (PHY_RECONFIG_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                break;

                case PHY_LOGS_ENABLE_REQ:
                    SvsrLoggerAddMsgForce (PHY_LOGS_ENABLE_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsgHeader, (pMsgHeader->msgSpecific + sizeof(GENMSGDESC)));
                break;

                default:
                break;
            }

            curr = curr->Next;
        }
#endif
    }

    *pNumApi = numApis;

    if ((numApis - numApisParsed) == 0)                     // Nothing else left in queue and everything has been parsed.
        LtePhyApiThreadSchedLogging(pListHeader, 3, 0);

    return (numApis - numApisParsed);
}


UINT32 LteBsApiErrorCheckPushApiState(UINT32 numApi, UINT32 numTxSdu, PDLSUBFRDESC pTxVector)
{
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    PLTE_BS_API_ERROR_CHECK_CTX pLteBsApiErrorCheckCtx = LteBsApiErrorCheckGetCtx();
    UINT32 sfn, retVal = LTE_BS_API_ERROR_CHECK_SUCCESS, numIcpuBlocks;
    UINT32 mlogVars[10], mlogVarsCnt = 0;

    mlogVars[mlogVarsCnt++] = 0xCDCDCDCD;

    if (gFirstMessageArrived != 0)
    {
        if (pTxVector)
        {
            mlogVars[mlogVarsCnt++] = 0x11111111;
            sfn = LteBsConstructSfn(pTxVector->frameNumber, pTxVector->subframeNumber);
            retVal = LteBsApiErrorCheckSfn(pLteBsApiErrorCheckCtx, sfn, DL);

            mlogVars[mlogVarsCnt++] = sfn;
            mlogVars[mlogVarsCnt++] = pLteBsApiErrorCheckCtx->lastTxSfn;

            // Check to see if this subframe is already has been pushed by MAC. If we already got an API with this subframe num.
            // Already in Queue (because it is a bug in MAC side). Report error
            if (pLteBsApiErrorCheckCtx->apiSfn[pTxVector->subframeNumber] != NULL)
                retVal = LTE_BS_API_ERROR_CHECK_DUPLICATE_SFN;
        }
        else
        {
            mlogVars[mlogVarsCnt++] = 0x22222222;
            retVal = LTE_BS_API_ERROR_CHECK_SFN_CHECK_FAIL;
        }
    }

    mlogVars[mlogVarsCnt++] = retVal;
    MLogAddVariables(mlogVarsCnt, mlogVars, GET_TICKS);

    if (LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_APISTATS))
    {
        numIcpuBlocks = (numApi << 1);     // Multiply by 2 for the pListHeader
        numIcpuBlocks += 1;                // Add 1 for the general header as part of each subframe
        if (gUseTxSduPointer)
            numIcpuBlocks += numTxSdu;     // Because each of these would be another Icpu Block

        LteBsPhyStatsNumApisArrived(pLteBsPhyStats, numApi);
        LteBsPhyStatsNumMac2PhyBlocksArrived(pLteBsPhyStats, numIcpuBlocks);

        if (retVal == LTE_BS_API_ERROR_CHECK_DUPLICATE_SFN)
        {
            LteBsPhyStatsMacToPhyDuplicateSfn(pLteBsPhyStats);
            LteBsPhyStatsMacToPhyDuplicateSfnApis(pLteBsPhyStats, numApi);
        }
        if (retVal & (LTE_BS_API_ERROR_CHECK_SFN_CHECK_FAIL | LTE_BS_API_ERROR_CHECK_DUPLICATE_SFN))
        {
            LteBsPhyStatsMacToPhyMessageCleanSfn(pLteBsPhyStats);
            LteBsPhyStatsMacToPhyMessageCleanSfnApis(pLteBsPhyStats, numApi);
        }
    }

    return retVal;
}


PMAC2PHY_QUEUE_EL LteBsPhyStatsGetApi(UINT32 *pIsDummy)
{
    PLTE_BS_API_ERROR_CHECK_CTX pLteBsApiErrorCheckCtx = LteBsApiErrorCheckGetCtx();
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    PMAC2PHY_QUEUE_EL pMsg;
    UINT32 fifoCleanType, fifoMsgLen, t = GET_TICKS;

    *pIsDummy = 0;

    pMsg = LtePhyMessagePop(FIFO_MAC_TO_PHY_API, &fifoCleanType, &fifoMsgLen);

    if (pMsg == NULL)
    {
        if (gFirstMessageArrived)
        {
            if (gPhyRobustControl)
            {
                LteBsPhyStatsNumDummyApis(pLteBsPhyStats);
                pMsg = LtePhyApiErrorCheckUseEmptyChannels(NULL);
                *pIsDummy = 1;
                pLteBsApiErrorCheckCtx->successiveNoFifo++;

                if (pLteBsApiErrorCheckCtx->successiveNoFifo >= MAX_SUCCESSIVE_NO_API_TTIS_BEFORE_STOP)
                {
                    LteBsPhyStatsNumConsecutiveDummyApis(pLteBsPhyStats, 10);
                    MLogTask(PID_API_ERRORCHECK_SUCCESSIVE_API_MISSING, RESOURCE_LARM, t, GET_TICKS);
                    stop_printf("Phy Inserted %d Successive Dummy Apis\r\n", MAX_SUCCESSIVE_NO_API_TTIS_BEFORE_STOP);
                    return (PMAC2PHY_QUEUE_EL)FIFO_EMPTY_ERR;
                }
            }
            else
            {
                stop_printf("FIFO Stack is empty\r\n");
            }
        }
    }
    else
    {
        if (pLteBsApiErrorCheckCtx->successiveNoFifo)
            LteBsPhyStatsNumConsecutiveDummyApis(pLteBsPhyStats, (pLteBsApiErrorCheckCtx->successiveNoFifo > 10 ? 10 : pLteBsApiErrorCheckCtx->successiveNoFifo-1));
        pLteBsApiErrorCheckCtx->successiveNoFifo = 0;
    }

    // Stop if no API and First API has arrived
    if (pMsg == NULL && gFirstMessageArrived)
    {
        MLogTask(PID_APITHREAD_NO_API, RESOURCE_LARM, t, GET_TICKS);
        stop_printf("No APIs preseet\r\n");
        return 0;
    }

    return pMsg;
}


//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL apiplayer.c
 *
 * @brief DOXYGEN_TO_DO
 * @author Mindspeed Technologies
 * @version $Revision: 1.71 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "ltephy.h"
#include "apiplayer.h"
#include "appprintf.h"
#include "LtePhyL2Api.h"
#include <string.h>
#include "globalstoragearrays.h"
#include "mdmadrv.h"
#include "Debug.h"
#include "Debug.h"
#include "prototypes.h"
#include "lte_entry.h"
#include "mlog.h"
#include "LteBsPhyStats.h"

APIPLAYERCTX player_ctx;

apiplayer_mode_t apiplayer_mode = APIPLAY_MODE_DEF;
pSTORAGE_CTX apiplayer_storage = NULL;
PUINT8 apiplayer_own_buff = NULL;
UINT32 apiplayer_own_buff_size = 0;

extern void SetMacInstanceId(UINT32 MacId);
extern UINT32 GetMacInstanceId(void);
UINT32 gDdr1RxSduBufWriteEna;

static UINT32 ApiPlayerWatchDog(LPVOID p)
{
    UINT32 curtick;
    static int count = 50;

    APIPLAYERCTX* pCtx = ApiPlayerGetCtx();

    curtick = MxGetTicks();

    if (pCtx->StopInd == 1)
        return 0;

	if((pCtx->delay_ticks) && (curtick - pCtx->lasttick) / 150000 >= (pCtx->delay_ticks))
	{
		uart_printf("\r\n\r\nAPI-PLAYER:whaiting too long... %d, TTI-recv=%d\r\n", (curtick - pCtx->lasttick) / 150000, pCtx->TTINum);
		count--;
	}
	else
		MxScheduleThread(pCtx->hWdg);

    return 0;
}


/** @brief This function stops player
    @param pApiPlayerParam [in] - the pointer to API player configuration parameter

    \ingroup group_lte_services*/

static void ApiPlayerStop(ApiPlayerParam * pApiPlayerParam)
{
    APIPLAYERCTX* pCtx = ApiPlayerGetCtx();
    UINT32 i, num, tti = pCtx->TTINum;

    uart_printf ("API-PLAYER received PHY_STOP_IND message\n");

    if (pCtx->PlayStarted == 0)
        return;

    pCtx->PlayStarted = 0;

    // to wait 1 second
    num = 5000;
    i   = 0;

    while ((i < num) && (pCtx->StopInd == 0))
    {
        SvsrProcICoreMsg();

        // to wait 1 ms
        MxDelayTicks(1000);
        i++;
    }

    // Send PhyShutdown
    {
        PMAC2PHY_QUEUE_EL pMsgBlock = (PMAC2PHY_QUEUE_EL)SvsrAllocMsg();
        U32 *pOutMsg = (U32 *)Phy2MacIfApiBuffer(sizeof(MSGIND) + 32);
        PINITIND ptr;

        _ASSERT_PTR (pMsgBlock);
        _ASSERT_PTR (pOutMsg);

        ptr = (PINITIND) pOutMsg;
        ptr->phyEntityId = (U8) 0;
        ptr->msgType = (U8) PHY_SHUTDOWN_REQ;
        ptr->status = (U8) 0;

        memset (pMsgBlock, 0, sizeof (*pMsgBlock));

        pMsgBlock->MessagePtr = (PUINT8)pOutMsg;
        pMsgBlock->MessageLen = sizeof(MSGIND) + 32;
        pMsgBlock->MessageType= PHY_SHUTDOWN_REQ;

        LteBsPhyStatsDisableMask(PHYSTATS_ENABLE_PHYSTATS_PRINT);
        SvsrSendMsg(0, IID_LTE_EX, pMsgBlock, SVSR_MSG_OPT_DEFAULT);
    }

    SvsrSetMsgHandler(IID_LTE_MAC, &pApiPlayerParam->hdlr_ctx);
    SetMacInstanceId(pApiPlayerParam->MacInstID);

    SvsrSetMsgHandler(IID_LTE_MAC_EX, &pApiPlayerParam->hdlr_ctx_ex);

    if(apiplayer_mode == APIPLAY_MODE_DEF)
        SvsrLoggerSetMask(pApiPlayerParam->nMask);
    MLogSetMask(0);

    if (pCtx->bSendStopInd)
    {
        ApiHeader* pInd = SvsrAllocICApi();
        if (pInd)
        {
            pCtx->bSendStopInd = 0;

            pInd = ApiCreate(pInd, API_DEF_FLAGS, IID_API_PLAYER, UARM_INSTID_HOST, API_TYPE_IND,
                             APIPLAYER_STOP);
            ApiAddParam(pInd, NULL, APIPLAY_PAR_FINISH_SFN, &pApiPlayerParam->CurrentSFN - 1, sizeof(UINT32));

            if (pCtx->bLogSdu)
            {
                ApiAddParam(pInd, NULL, APIPLAY_PAR_SDU_LOG_PTR, &pCtx->LogPtr, sizeof(UINT32));
                ApiAddParam(pInd, NULL, APIPLAY_PAR_SDU_LOG_SIZE, &pCtx->LogPos, sizeof(UINT32));

                uart_printf ("--------------------------------------------------------------\r\n");
                uart_printf ("API-PLAYER: logptr=%x, logsize=%d, numsdu=%d, statnumsdu=%d\r\n", pCtx->LogPtr, pCtx->LogPos, pCtx->LogSduNum, pCtx->LogSduStatusNum);
                uart_printf ("--------------------------------------------------------------\r\n");
            }

            ApiAddErrorCode(pInd, MX_OK);
            uart_printf("API-PLAYER: send Stop indication to Host\r\n");
            SvsrSendICApi(pInd);
            //SvsrSendAPI(pInd);
        }
        else
        {
            uart_printf("API-PLAYER: can't allocate ICPU for Stop ind\r\n");
        }
    }

    uart_printf("API-PLAYER: Done, last subframe #%d  is played\r\n",
        (pApiPlayerParam->CurrentSFN != 0xFFFFFFFF) ?
            pApiPlayerParam->CurrentSFN - 1 :
            pApiPlayerParam->FinishSFN);

    MxRemoveThread(pCtx->hWdg);

    memset(pApiPlayerParam, 0, sizeof(*pApiPlayerParam));
}

/** @brief This function returns a pointer to th API plaer storage,
           because of two storage can be used : API LOGGER / API PLAYER
           this function checks parameters and returns a pointer to the proper storage


    @return [PUINT8] the pointer to the storage

    \ingroup group_lte_services*/

static pSTORAGE_CTX ApiPlayerStorage(void)
{
    if(apiplayer_mode == APIPLAY_MODE_DEF)
        return  SvsrLoggerGetStorageCtx();
    else if (apiplayer_mode == APIPLAY_MODE_OWN_BUFF)
        return apiplayer_storage;
    else
        return NULL;
}

/** @brief This function returns the size of API player storage in bytes

    @return [UINT32] the size of storage in bytes

    \ingroup group_lte_services*/

static UINT32 ApiPlayerStorageSize(void)
{
    if(apiplayer_mode == APIPLAY_MODE_DEF)
        return SvsrLoggerGetStorageSize();
    else if (apiplayer_mode == APIPLAY_MODE_OWN_BUFF)
        return apiplayer_own_buff_size;
    else
        return 0;
}

/** @brief This function returns pointer to the end of storage

    @return [PUINT8] the pointer to the end of storage

    \ingroup group_lte_services*/

static PUINT8 ApiPlayerStorageLimit(void)
{
    return ((PUINT8)(ApiPlayerStorage()->hdr) + ApiPlayerStorageSize());
}

/** @brief The API messages handler, this function handles PHY requests
            and cofigures/starts API Player for new TTI, it's designed for I-CORE

    @param pClientCtx [in] - the pointer to the context
    @param pMsg       [in] - the pointer to the message

    @return [UINT32] see error codes of I-CORE communication

    \ingroup group_lte_services*/

static MXRC ApiPlayerMsgHandler(LPVOID pClientCtx, LPVOID pMsg) // The response for API command
{
    PGENMSGDESC pMsgHdr = (PGENMSGDESC) pMsg;
    HANDLE hThr = *(HANDLE*) pClientCtx;
    APIPLAYERCTX* pCtx = ApiPlayerGetCtx();
    UINT32 t, t1, LogPos;

    _ASSERT_PTR(pCtx);

    if (pMsgHdr->msgType == PHY_TXSTART_IND)
    {
        // to disable PHY logging if any

        pCtx->lasttick = MxGetTicks();

        if (!pCtx->TTINum)
            MxScheduleThread(pCtx->hWdg);

        pCtx->TTINum++;

        if (pCtx->bLogSdu)
            gDdr1RxSduBufWriteEna = 0;

        if (HANDLE_NOT_NULL(hThr))
        {
            MxScheduleThread(hThr);
        }
    }
    else if (pMsgHdr->msgType == PHY_RXSDU_IND)
    {
        PRXSDUIND   pSduInd   = (PRXSDUIND)pMsg;
        UINT32 nHdrSize, nSduSize;

        _ASSERT_PTR(pSduInd);

        if (pCtx->bLogSdu)
        {
            t = GET_TICKS;
            nHdrSize = sizeof(RXSDUIND);
            nSduSize = nHdrSize + (pSduInd->numBitsRx >> 3);

            // to log the SDU if it's enabled on the API player side

            if ((pCtx->LogPos + nSduSize) < pCtx->LogSize)
            {
                LogPos = pCtx->LogPos;
                pCtx->LogPos += (nSduSize + 3) & ~3;
                memcpy(pCtx->LogPtr + LogPos, pMsg, nSduSize);

                pCtx->LogSduNum ++;
            }
            MLogTask(PID_API_PLAYER_RXSDU_IND, RESOURCE_LARM, t, GET_TICKS);
        }
    }
    else if (pMsgHdr->msgType == PHY_RXSTATUS_IND)
    {
        PRXSTATUSIND pSduInd   = (PRXSTATUSIND)pMsg;
        PRX_CQIRIHI_STATUS_EVENT pMsg1 = (PRX_CQIRIHI_STATUS_EVENT)&pSduInd->pEventList;
        PRX_RIP_MEAS_STATUS_EVENT pRipMeasStatusEvent;
        UINT32 nHdrSize = 0, nSduSize = 0, nPayloadLen = 0;

        _ASSERT_PTR(pSduInd);

        if (pCtx->bLogSdu)
        {
            t1 = GET_TICKS;
            nHdrSize = sizeof(RXSTATUSIND);
            if (pSduInd->statusType == SRS_RESULT)
            {
                nSduSize = nHdrSize + pSduInd->eventNumber * (sizeof(RX_SRS_STATUS_EVENT) - sizeof(U8*))
                                                            + pSduInd->msgSpecific - sizeof(U32);
            }
            else if (pSduInd->statusType == CQIRIHI_RESULT)
            {
                nPayloadLen = pMsg1->status_len;
                nSduSize = nHdrSize + (pSduInd->eventNumber * sizeof(RX_CQIRIHI_STATUS_EVENT)) + nPayloadLen;
            }
            else if (pSduInd->statusType == PRACH_RESULT)
            {
                nSduSize = nHdrSize + (pSduInd->eventNumber * sizeof(RX_PRACH_STATUS_EVENT));
            }
            else if (pSduInd->statusType == RECEIVE_INTERFERENCE_POWER_RESULT)
            {
                pRipMeasStatusEvent = (PRX_RIP_MEAS_STATUS_EVENT) &pSduInd->pEventList;
                nSduSize = nHdrSize + sizeof(RX_RIP_MEAS_STATUS_EVENT) + (((pRipMeasStatusEvent->numResBlocks+1)>>1)*sizeof(RX_RIP_MEAS_PER_BLOCK));
            }
            else if (pSduInd->statusType == THREMAL_NOISE_RESULT)
            {
                nSduSize = nHdrSize + sizeof(RX_THERMAL_NOISE_MEAS_STATUS_EVENT);
            }

            // Round Up
            nSduSize = (nSduSize + 3) & ~3;

            // to log the SDU if it's enabled on the API player side

            if ((pCtx->LogPos + nSduSize) < pCtx->LogSize)
            {
                LogPos = pCtx->LogPos;
                pCtx->LogPos += nSduSize;
                memcpy(pCtx->LogPtr + LogPos, pMsg, nSduSize);

                pCtx->LogSduStatusNum ++;
            }
            MLogTask(PID_API_PLAYER_RXSTATUS_IND, RESOURCE_LARM, t1, GET_TICKS);
        }
    }
    else if (pMsgHdr->msgType == PHY_STOP_IND)
    {
        pCtx->StopInd = 1;
        ApiPlayerStop(&pCtx->param);
    }

    return 0;
}

static MXRC ApiPlayerRxSduMsgHandler(LPVOID pClientCtx, LPVOID pMsg) // The response for API command
{
    PMAC2PHY_QUEUE_EL Curr, Tail;
    APIPLAYERCTX* pCtx = ApiPlayerGetCtx();
    UINT32 t, LogPos, rc;
    PRXSDUIND   pSduInd;
    UINT32 nHdrSize, nSduSize;

    _ASSERT_PTR(pCtx);

    if (pCtx->bLogSdu)
    {
        t = GET_TICKS;
        Curr = (PMAC2PHY_QUEUE_EL)pMsg;
        while(Curr)
        {
            pSduInd = (PRXSDUIND)Curr->MessagePtr;
            switch(pSduInd->msgType)
            {
                case PHY_RXSDU_IND:
                {
                    nHdrSize = sizeof(RXSDUIND);
                    nSduSize = nHdrSize + (pSduInd->numBitsRx >> 3);
                    if ((pCtx->LogPos + nSduSize) < pCtx->LogSize)
                    {
                        LogPos = pCtx->LogPos;
                        pCtx->LogPos += (nSduSize + 3) & ~3;
                        memcpy(pCtx->LogPtr + LogPos, pSduInd, nSduSize);
                        if ((rc = SvsrFreeIMem(pSduInd)) != MX_OK)
                        {
                            uart_printf("API-PLAYER ERROR: cann't free the RxSdu [%x]\n", rc);
                            return rc;
                        }
                        pCtx->LogSduNum ++;
                    }
                }
                break;

                case PHY_RXSTATUS_IND:
                {
                    PRXSTATUSIND pSduStatus   = (PRXSTATUSIND)pSduInd;
                    PRX_CQIRIHI_STATUS_EVENT pMsg1 = (PRX_CQIRIHI_STATUS_EVENT)&pSduStatus->pEventList;
                    UINT32 nSduSize = 0, nPayloadLen = 0;

                    _ASSERT_PTR(pSduInd);

                    nPayloadLen = pMsg1->status_len;
                    nSduSize = sizeof(RXSTATUSIND) + (pSduStatus->eventNumber * sizeof(RX_CQIRIHI_STATUS_EVENT))
                                + nPayloadLen;
                    // Round Up
                    nSduSize = (nSduSize + 3) & ~3;

                    // to log the SDU if it's enabled on the API player side
                    if ((pCtx->LogPos + nSduSize) < pCtx->LogSize)
                    {
                        LogPos = pCtx->LogPos;
                        pCtx->LogPos += nSduSize;
                        memcpy(pCtx->LogPtr + LogPos, pSduStatus, nSduSize);
                        if ((rc = SvsrFreeIMem(pSduStatus)) != MX_OK)
                        {
                            uart_printf("API-PLAYER ERROR: cann't free the RxStatus [%x] %x\n", rc, pSduStatus);
                            return rc;
                        }
                        pCtx->LogSduStatusNum ++;
                    }
                }
                break;

                case PHY_RXEND_IND:
                {
                    if ((rc = SvsrFreeIMem(pSduInd)) != MX_OK)
                    {
                        uart_printf("API-PLAYER ERROR: cann't free the RxEnd Indication [%x]\n", rc);
                        return rc;
                    }
                }
                break;

                default:
                    uart_printf("API-PLAYER Error: unknown RxSdu.Ind type[%d]",pSduInd->msgType);
                    break;
            }

            Tail = Curr->Next;
            SvsrFreeIMem(Curr);
            Curr = Tail;
        }

        if(pCtx->LogSduNum)
            MLogTask(PID_API_PLAYER_RXSDU_IND, RESOURCE_LARM, t, GET_TICKS);

        if(pCtx->LogSduStatusNum)
            MLogTask(PID_API_PLAYER_RXSTATUS_IND, RESOURCE_LARM, t, GET_TICKS);

    }


    return 0;
}

/** @brief This function sends list of messages to the PHY
    @param pApiPlayerParam [in] - the pointer to the API player configuration parameters

    \ingroup group_lte_services*/

void ApiPlayerPlaySubframe (ApiPlayerParam * pApiPlayerParam)
{
    UINT32 rc;

    _ASSERT_PTR(pApiPlayerParam);

    // if we have nothing to send
    if (pApiPlayerParam->MsgHead == NULL)
        return;

    _ASSERT_PTR(pApiPlayerParam->MsgHead);

    if (pApiPlayerParam->MsgHead->MessageType >=  NMM_START && pApiPlayerParam->MsgHead->MessageType < NMM_final_to_be_updated)
        rc = SvsrSendMsg(0, IID_LTE_NMM, pApiPlayerParam->MsgHead, SVSR_MSG_OPT_DEFAULT);
    else
        rc = SvsrSendMsg(0, IID_LTE_EX, pApiPlayerParam->MsgHead, SVSR_MSG_OPT_DEFAULT);

    _ASSERT_RC(rc);

    if (FAILED(rc))
    {
        uart_printf ("API-PLAYER: failed to send message, rc=%x\r\n", rc);
        while (1);
    }

    pApiPlayerParam->MsgHead = NULL;
    pApiPlayerParam->MsgTail = NULL;
}

/** @brief The function puts message to the list

    @param pHdr [in] - the pointer to the LOG message

    @return [MXRC] an error code of operation, MX_OK - OK

    \ingroup group_lte_services*/

static MXRC ApiPlayerPutMsgToList(API_LOG_INFO * pHdr)
{
    PGENMSGDESC pPhyMsg = (PGENMSGDESC) SvsrGetApiPayload(pHdr);
    PGENMSGDESC pBuffer = (PGENMSGDESC) SvsrAllocMsg();
    ApiPlayerParam * pApiPlayerParam = ApiPlayerGetParam();

    PMAC2PHY_QUEUE_EL pMsgBlock;

    if (!pBuffer)
    {
        uart_printf("API-PLAYER: Can't allocate ICPU, no memory, subframes played=%d\r\n", pApiPlayerParam->CurrentSFN);
        ApiPlayerStop(pApiPlayerParam);
        return 1;
    }

    memcpy(pBuffer, pPhyMsg, pApiPlayerParam->pHdr->PayloadLen);

    pMsgBlock = (PMAC2PHY_QUEUE_EL)SvsrAllocMsg();
    _ASSERT_PTR (pMsgBlock);

    if (pMsgBlock == NULL)
    {
        SvsrFreeMsg(pBuffer);

        uart_printf("API-PLAYER: Can't allocate ICPU (for msg block), no memory, subframes played=%d\r\n", pApiPlayerParam->CurrentSFN);
        ApiPlayerStop(pApiPlayerParam);
        return 1;
    }

    memset (pMsgBlock, 0, sizeof (*pMsgBlock));

    pMsgBlock->MessagePtr = (PUINT8)pBuffer;
    pMsgBlock->MessageLen = pApiPlayerParam->pHdr->PayloadLen;
    pMsgBlock->MessageType= pBuffer->msgType;

    if (pApiPlayerParam->MsgHead == NULL)
    {
        pApiPlayerParam->MsgHead = pMsgBlock;
        pApiPlayerParam->MsgTail = pMsgBlock;

        //uart_printf ("Add msg to the head: ID=%d, Len=%d, ptr=%x\r\n", pBuffer->msgType, pParam->pHdr->PayloadLen, pBuffer);
    }
    else
    {
        pApiPlayerParam->MsgTail->Next = pMsgBlock;
        pApiPlayerParam->MsgTail = pMsgBlock;

        //uart_printf ("Add msg to the tail: ID=%d, Len=%d, ptr=%x\r\n", pBuffer->msgType, pParam->pHdr->PayloadLen, pBuffer);
    }

    return MX_OK;
}

/** @brief The function sends INIT message to the PHY code
    @param pApiPlayerParam [in] - the pointer to the player parameters

    @return [MXRC] an error code of operation

    \ingroup group_lte_services*/

static MXRC ApiPlayerPrepareInitMsg(ApiPlayerParam * pApiPlayerParam)
{
    MXRC rc;

    // to get the first message
    if ((pApiPlayerParam->pHdr = LoggerStorageGetNextMsg(apiplayer_storage, &pApiPlayerParam->NextMsgPos)) == NULL)
    {
        uart_printf("API-PLAYER: Init API command index error\r\n");
        ApiPlayerStop(pApiPlayerParam);
        return 1;
    }

    if (pApiPlayerParam->pHdr->Message != PHY_INIT_REQ)
    {
        uart_printf("API-PLAYER: The first API is not PHY_INI\r\n");
        ApiPlayerStop(pApiPlayerParam);
        return 2;
    }

    rc = ApiPlayerPutMsgToList(pApiPlayerParam->pHdr);

    return rc;
}

/** @brief The function sends START message (and sets TTI if specified) to the PHY
    @param pApiPlayerParam [in] - the pointer to the player parameters

    @return [MXRC] an error code of operation

    \ingroup group_lte_services*/

static MXRC ApiPlayerPrepareStartMsg(ApiPlayerParam * pApiPlayerParam)
{
    MXRC rc;

    // to get the first message
    if ((pApiPlayerParam->pHdr = LoggerStorageGetNextMsg(apiplayer_storage, &pApiPlayerParam->NextMsgPos)) == NULL)
    {
        uart_printf("API-PLAYER: PHY_Start API command index error\r\n");
        ApiPlayerStop(pApiPlayerParam);
        return 3;
    }

    if (pApiPlayerParam->pHdr->Message != PHY_START_REQ)
    {
        uart_printf("API-PLAYER: The second API is not PHY_START\r\n");
        ApiPlayerStop(pApiPlayerParam);
        return 4;
    }

    if (pApiPlayerParam->nPhyTTI != 0)
    {
        PGENMSGDESC pPhyMsg = (PGENMSGDESC) SvsrGetApiPayload(pApiPlayerParam->pHdr);
        PSTARTREQ pStartMsg = (PSTARTREQ)pPhyMsg;

        _ASSERT_PTR(pStartMsg);

        pStartMsg->period = pApiPlayerParam->nPhyTTI;
    }

    if (pApiPlayerParam->nPhyDI != 0xFF)
    {
        PGENMSGDESC pPhyMsg = (PGENMSGDESC) SvsrGetApiPayload(pApiPlayerParam->pHdr);
        PSTARTREQ pStartMsg = (PSTARTREQ)pPhyMsg;

        _ASSERT_PTR(pStartMsg);

        pStartMsg->mode  = pApiPlayerParam->nPhyDI; // data interface specifyed by the command parameter
    }

    rc = ApiPlayerPutMsgToList(pApiPlayerParam->pHdr);

    return rc;
}

/** @brief The function extract subframe number from current message
    @param pHdr [in] - the pointer to the LOG message (PHY_TXSTART_REQ)

    @return [UINT32] a subframe number

    \ingroup group_lte_services*/

UINT32 ApiPlayerGetCurrentSFN(API_LOG_INFO * pHdr)
{
    UINT32 SubframeShift;
    PDLSUBFRDESC pDlSubFrame;
    UINT32 SFN = 0xFFFFFFFF;

    SubframeShift = (UINT32)pHdr;

    pDlSubFrame = (PDLSUBFRDESC) (SubframeShift+ sizeof(API_LOG_INFO) + sizeof(GENMSGDESC));
    SFN = pDlSubFrame->frameNumber * 10 + pDlSubFrame->subframeNumber;

    return SFN;
}

/** @brief The function search first subframe number at the LOG

    @return [UINT32] a first subframe number

    \ingroup group_lte_services*/

UINT32 ApiPlayerGetFirstSubframeNumber(void)
{
    UINT32 NextPosition = 0;
    API_LOG_INFO * pHdr;
    API_LOG_INFO * First_TxStartReq_Adr = NULL;
    UINT32 PHY_TxStartReq_First_SFN = 0xFFFFFFFF;

	if(!apiplayer_storage)
	{
		uart_printf("API-PLAYER: apiplayer_storage is NULL \r\n");
		return 0;
	}

    // Get first Tx vector
    while ((pHdr = LoggerStorageGetNextMsg(apiplayer_storage, &NextPosition)) != NULL)
        if (pHdr->Message == PHY_TXSTART_REQ)
        {
            First_TxStartReq_Adr = pHdr;
            break;
        }

    // Check that we've found Tx vector
    _ASSERT_PTR(First_TxStartReq_Adr);

    // Calculate subframe number
    PHY_TxStartReq_First_SFN = ApiPlayerGetCurrentSFN(First_TxStartReq_Adr);

    // return first subframe number
    return PHY_TxStartReq_First_SFN;
}


/** @brief The function search last subframe number at the LOG

    @return [UINT32] a last subframe number

    \ingroup group_lte_services*/

UINT32 ApiPlayerGetLastSubframeNumber(void)
{
    UINT32 NextPosition = 0;
    API_LOG_INFO * pHdr;
    API_LOG_INFO * Last_TxStartReq_Adr = NULL;
    UINT32 PHY_TxStartReq_Last_SFN = 0;

	if(!apiplayer_storage)
	{
		uart_printf("API-PLAYER: apiplayer_storage is NULL \r\n");
		return 0;
	}

    // Get last Tx vector
    while ((pHdr = LoggerStorageGetNextMsg(apiplayer_storage, &NextPosition)) != NULL)
        if (pHdr->Message == PHY_TXSTART_REQ) Last_TxStartReq_Adr = pHdr;

    // Check that we've found Tx vector
    _ASSERT_PTR(Last_TxStartReq_Adr);

    // Calculate subframe number
    PHY_TxStartReq_Last_SFN = ApiPlayerGetCurrentSFN(Last_TxStartReq_Adr);

    // return last subframe number
    return PHY_TxStartReq_Last_SFN;
}

/** @brief This function is designed to prepare the trace of API commands
           that has to be sent to the PHY

    @return [UINT32] Current subframe number

    \ingroup group_lte_services */

UINT32 ApiPlayerPrepareSubframe (APIPLAYERCTX * pCtx)
    {
    ApiPlayerParam * pApiPlayerParam;
    UINT32 CurrentSFN = 0xFFFFFFFF;
    UINT32 CurrentTTI = 0xFFFFFFFF;

    _ASSERT_PTR (pCtx);

    pApiPlayerParam = &pCtx->param;

    // if we reached last message - do nothing
    if (pApiPlayerParam->pHdr == NULL)
    {
        return CurrentSFN;
    }

    // Previous subframe should be played or drained
    if (pApiPlayerParam->MsgHead != NULL)
    {
        uart_printf ("API-PLAYER: For some reason MsgHead is not equal to NULL\r\n");
        _ASSERT(0);
    }

    // to prepare the trace , this trace will be sent to the PHY
    while (pApiPlayerParam->pHdr != NULL)
    {
        // Save next message pointer
        UINT32 MsgPosSave = pApiPlayerParam->NextMsgPos;

        // Get message to process
        pApiPlayerParam->pHdr = LoggerStorageGetNextMsg(apiplayer_storage, &pApiPlayerParam->NextMsgPos);

        // we reached the end of trace?
        if (pApiPlayerParam->pHdr == NULL)
        {
            break;
        }

        // This message shouldn't be processed
        if (((pApiPlayerParam->pHdr->Destination != IID_LTE) && (pApiPlayerParam->pHdr->Destination != IID_LTE_NMM)) || (pApiPlayerParam->pHdr->Message == 0))
        {
            continue;
        }

        // Get TTI number from first message of subframe
        if (CurrentTTI == 0xFFFFFFFF)
        {
            CurrentTTI = pApiPlayerParam->pHdr->CurrentTTI;
        }

        // Add message from current TTI to the list
        if (CurrentTTI == pApiPlayerParam->pHdr->CurrentTTI)
        {
            if (ApiPlayerPutMsgToList(pApiPlayerParam->pHdr))
            {
                uart_printf("API-PLAYER: send message error\r\n");
                _ASSERT(0);
            }
        }
        else
        // New TTI message has been found
            {
            // Save new TTI
            CurrentTTI = pApiPlayerParam->pHdr->CurrentTTI;

            // Save current message for future processing
                pApiPlayerParam->NextMsgPos = MsgPosSave;
                break;
            }

        // Extract current subframe number
        if (pApiPlayerParam->pHdr->Message == PHY_TXSTART_REQ)
        {
             CurrentSFN = ApiPlayerGetCurrentSFN(pApiPlayerParam->pHdr);
        }
    }

    return CurrentSFN;
}

/** @brief The function seek LOG to desired subframe number

    @param DesiredSFN [in] - desired subframe number

    @return [BOOL] operation result

    \ingroup group_lte_services*/

BOOL ApiPlayerSeekToSFN(UINT32 DesiredSFN)
{
    ApiPlayerParam * pApiPlayerParam = ApiPlayerGetParam();
    API_LOG_INFO * pHdr;
    UINT32 NextPosition = pApiPlayerParam->NextMsgPos;
    UINT32 SFBoundaryPosition = pApiPlayerParam->NextMsgPos;
    UINT32 SavedPosition = pApiPlayerParam->NextMsgPos;
    UINT32 CurrentSFTTI = 0xFFFFFFFF;
    BOOL Result = FALSE;

    // Search desired SF boundary
    while ((pHdr = LoggerStorageGetNextMsg(apiplayer_storage, &NextPosition)) != NULL) // Get Next!
    {
        // Check for new SF boundary
        if (CurrentSFTTI != pHdr->CurrentTTI)
        {
            //
            // New subframe detected!
            //

            // Save its TTI
            CurrentSFTTI = pHdr->CurrentTTI;
            // Save its start message position
            SFBoundaryPosition = SavedPosition;
        }

        // Save next message position
        SavedPosition = NextPosition;

        // Check Subframe number
        if (pHdr->Message == PHY_TXSTART_REQ)
        {
            // Yes! We found it!
            if (ApiPlayerGetCurrentSFN(pHdr) == DesiredSFN)
            {
                // Restore frame boundary position
                pApiPlayerParam->NextMsgPos = SFBoundaryPosition;

                // Return from a function
                Result = TRUE;
                break;
            }
        }
    }

    return Result;
}

static BOOL ApiPlayerIsNMMLog(void)
{
    UINT32 NextPosition = 0;
    API_LOG_INFO * pHdr;
    BOOL rc = FALSE;

    while ((pHdr = LoggerStorageGetNextMsg(apiplayer_storage, &NextPosition)) != NULL)
        if (pHdr->Message >= NMM_START && pHdr->Message < NMM_final_to_be_updated)
            rc = TRUE;

    return rc;
}


static MXRC ApiPlayerPrepareNMMStartMsg(ApiPlayerParam * pApiPlayerParam)
{
    MXRC rc;

    // to get the first message
    if ((pApiPlayerParam->pHdr = LoggerStorageGetNextMsg(apiplayer_storage, &pApiPlayerParam->NextMsgPos)) == NULL)
    {
        uart_printf("API-PLAYER: PHY_Start API command index error\r\n");
        ApiPlayerStop(pApiPlayerParam);
        return 3;
    }

    if (pApiPlayerParam->pHdr->Message != NMM_START)
    {
        uart_printf("API-PLAYER: The second API is not PHY_START\r\n");
        ApiPlayerStop(pApiPlayerParam);
        return 4;
    }

    uart_printf("[%s] params tti %d, di %d\n", __FUNCTION__, pApiPlayerParam->nPhyTTI, pApiPlayerParam->nPhyDI);

    rc = ApiPlayerPutMsgToList(pApiPlayerParam->pHdr);

    return rc;
}


/** @brief The API player thread, it forms messages and iterates messages sending
    @param p [in] - the pointer to the API player parameters

    @return [UINT32] 0

    \ingroup group_lte_services*/

static UINT32 ApiPlayerThread(LPVOID p)
{
    ApiPlayerParam * pApiPlayerParam = (ApiPlayerParam *) p;
    APIPLAYERCTX * pCtx = ApiPlayerGetCtx();

    UINT32 t = GET_TICKS;


    if (ApiPlayerIsNMMLog())
    {
        if (ApiPlayerPrepareInitMsg(pApiPlayerParam))
            return 1;

        /* play phy_init message */
        ApiPlayerPlaySubframe (pApiPlayerParam);

        /* get nmm_start message */
        if (ApiPlayerPrepareNMMStartMsg(pApiPlayerParam))
            return 2;

        ApiPlayerPrepareSubframe(pCtx);
        ApiPlayerPrepareSubframe(pCtx);

        ApiPlayerPlaySubframe (pApiPlayerParam);

        return MX_OK;
    }

    if (!pApiPlayerParam->InitAndStartDone)
    {
        pApiPlayerParam->InitAndStartDone = TRUE;

        pApiPlayerParam->MacInstID = GetMacInstanceId();
        SetMacInstanceId(IID_LTE_MAC);

        SvsrGetMsgHandler(IID_LTE_MAC, &pApiPlayerParam->hdlr_ctx);
        SvsrRegMsgHandler(IID_LTE_MAC, ApiPlayerMsgHandler, &pCtx->hThr);

        SvsrGetMsgHandler(IID_LTE_MAC_EX, &pApiPlayerParam->hdlr_ctx_ex);
        SvsrRegMsgHandler(IID_LTE_MAC_EX, ApiPlayerRxSduMsgHandler, &pCtx->hThr);

        if (ApiPlayerPrepareInitMsg(pApiPlayerParam))
            return 1;

        if (ApiPlayerPrepareStartMsg(pApiPlayerParam))
            return 2;

        // Play Init + Start messages
        ApiPlayerPlaySubframe (pApiPlayerParam);

        // Search and prepare to play start subframe
        if (ApiPlayerSeekToSFN(pApiPlayerParam->StartSFN))
        {
            pApiPlayerParam->CurrentSFN = ApiPlayerPrepareSubframe(pCtx);
        }
        else
        {
            uart_printf("API-PLAYER: Start SFN %d couldn't be found!\nAPI-PLAYER stoped!\n\n", pApiPlayerParam->StartSFN);
            ApiPlayerStop(pApiPlayerParam);
            return 3;
        }

        return MX_OK;
    }

    // to send the list of prepared commands
    ApiPlayerPlaySubframe (pApiPlayerParam);

    // Move to the next subframe processing
    // Prepare the trace for the next TTI
    pApiPlayerParam->CurrentSFN = ApiPlayerPrepareSubframe(pCtx);

    // Stop thread generation if nothing to send
    if (pApiPlayerParam->CurrentSFN == 0xFFFFFFFF)
        pCtx->hThr = HNULL;

    MLogTask(PID_API_PLAYER_SEND_MESSAGE_TO_PHY, RESOURCE_LARM, t, GET_TICKS);

    return MX_OK;
}

/** @brief This function starts playing the preloaded API trace to the PHY code

    @param DataPtr   [in] the pointer to the API trace buffer
    @param StartSFN  [in] the start index , 0 - based, the first SFN to play
    @param FinishSFN [in] the last SFN to play
    @param tti       [in] the TTI value, if 0 - TTI provided in START message will be used
    @param di        [in] the Data interface ID (if 0xFF the specified in START command will be used)
    @param bLogRxSdu [in] if 1 - the API player will log received SDUs

    @return [MXRC] an error code of operation

    \ingroup group_lte_services*/

MXRC ApiPlayerStart(PUINT8 DataPtr, UINT32 StartSFN, UINT32 FinishSFN, UINT32 tti, UINT32 di, UINT32 bLogRxSdu)
{
    MXRC rc = MX_OK;
    APIPLAYERCTX* pCtx = ApiPlayerGetCtx();
    ApiPlayerParam* pApiPlayerParam = ApiPlayerGetParam();
    UINT32 PlayerThreadCpu;
    char * pPhyDI;
    UINT32 LogVersion;
    UINT32 SFNtoPlay;

    // Check Log Version
    LogVersion = LoggerStorageGetLogVersion(apiplayer_storage);
    if (LogVersion < LOGGER_VERSION)
    {
        uart_printf("API-PLAYER: Current ApiLog version 0x%x isn't supported anymore!\n", LogVersion);
        return RC_APIPLAY_LOGVER_ERROR;
    }

    // Fill ApiPlayer parameters
    memset(pApiPlayerParam, 0, sizeof(*pApiPlayerParam));

    // Wrap FinishSFN
    if (FinishSFN > 1023*10 + 9) FinishSFN >>= 10;

    pApiPlayerParam->FirstSFN = ApiPlayerGetFirstSubframeNumber();
    pApiPlayerParam->LastSFN = ApiPlayerGetLastSubframeNumber();
    pApiPlayerParam->StartSFN  = StartSFN;
    pApiPlayerParam->FinishSFN = MIN(FinishSFN, pApiPlayerParam->LastSFN);
    pApiPlayerParam->InitAndStartDone = FALSE;
    pApiPlayerParam->nPhyTTI = tti;
    pApiPlayerParam->nPhyDI  = di;
    pApiPlayerParam->NextMsgPos = 0;
    pApiPlayerParam->nMask = SvsrLoggerGetMask();

    // SFN wrap occured?
    if (pApiPlayerParam->FinishSFN < pApiPlayerParam->StartSFN)
        SFNtoPlay = 10240 - pApiPlayerParam->StartSFN + pApiPlayerParam->FinishSFN + 1;
    else
        SFNtoPlay = pApiPlayerParam->FinishSFN - pApiPlayerParam->StartSFN + 1;

    if(apiplayer_mode == APIPLAY_MODE_DEF)
    {
        SvsrLoggerSetMask(0);
        SvsrLoggerDisable();
    }

    pCtx->TTINum            = 0;
    pCtx->StopInd           = 0;
    pCtx->LogPos            = 0;
    pCtx->LogStatus         = 0;
    pCtx->LogSduNum         = 0;
    pCtx->LogSduStatusNum   = 0;
    pCtx->PlayStarted       = 1;
    pCtx->bLogSdu           = bLogRxSdu;
    pCtx->delay_ticks       = tti * 2;

    PlayerThreadCpu = (MxGetARMID() == 0) ? 3 : 1;

    switch (pApiPlayerParam->nPhyDI)
    {
        case 0:
            pPhyDI = "CPRI";
            break;

        case 1:
            pPhyDI = "TIMER";
            break;

        case 4:
            pPhyDI = "ADI";
            break;

        default:
            pPhyDI = "default";
            break;
    }

    uart_printf ("\r\n");
    uart_printf ("API-PLAYER: Start playing\r\n");
    uart_printf (" * start SF:      %d\r\n", pApiPlayerParam->StartSFN);
    uart_printf (" * finish SF:     %d\r\n", pApiPlayerParam->FinishSFN);
    uart_printf (" * SFN to play:   %d\r\n", SFNtoPlay);
    uart_printf (" * first SFN:     %d\r\n", pApiPlayerParam->FirstSFN);
    uart_printf (" * last SFN:      %d\r\n", pApiPlayerParam->LastSFN);
    uart_printf (" * PHY TTI:       %d\r\n", pApiPlayerParam->nPhyTTI);
    uart_printf (" * PHY DI:        %s\r\n", pPhyDI);
    uart_printf (" * ApiLog ver:    0x%x\r\n", LogVersion);
    uart_printf (" * cpu-%d \r\n", MxGetCpuID());
    uart_printf ("\r\n");

    if (HANDLE_NULL(pCtx->hThr))
    {
        rc = MxCreateThreadEx(PlayerThreadCpu, ApiPlayerThread, 0, pApiPlayerParam, 0, &pCtx->hThr);
        _ASSERT_RC(rc);
    }


    if (HANDLE_NULL(pCtx->hWdg))
    {
        rc = MxCreateThreadEx(PlayerThreadCpu, ApiPlayerWatchDog, 0, NULL, 0, &pCtx->hWdg);
        _ASSERT_RC(rc);
    }

    pCtx->lasttick = MxGetTicks();

    // start right now
    rc = MxScheduleThread(pCtx->hThr);

    return rc;
}

/** @brief The function is designed to fill API player buffer
    @param pApi [in] - the pointer to the MSPD API command

    @return [MXRC] an error code

    \ingroup group_lte_services*/

MXRC ApiPlayerLoadBuffer(ApiHeader * pApi)
{
    ApiParam *pApiParam;
    PUINT8 pBuffer;
    UINT32 offset = 0;
    UINT32 dataSize = 0;

    if (ApiPlayerStorage() == NULL)
    {
        return RC_APIPLAY_ALLOC_ERROR;
    }

    pBuffer = (PUINT8) ApiPlayerStorage()->hdr;

    // Buffer offset
    pApiParam = ApiGetParamByID(pApi, APIPLAY_PAR_BUF_OFFS_PARAM_ID);
    if (pApiParam != NULL)
        offset = pApiParam->ParamData[0];

    // Data
    pApiParam = ApiGetParamByID(pApi, APIPLAY_PAR_DATA_PARAM_ID);
    if (pApiParam == NULL)
        return RC_APIPLAY_PARAM_ERROR;
    dataSize = ApiGetParamDataSize(pApiParam);

    if ((pBuffer + offset + dataSize) > ApiPlayerStorageLimit())
    {
        uart_printf("API-PLAYER: buffer overflow: %X > %X\r\n", (pBuffer + offset + dataSize),
                    ApiPlayerStorageLimit());
        return RC_APIPLAY_BUFFER_OUT;
    }

    memcpy((pBuffer + offset), pApiParam->ParamData, dataSize);

    return MX_OK;
}

/** @brief This function allocates API player own buffers, in this case API logger can be used
    @param pApi [in] - the pointer to the API command with parameters

    @return [MXRC] an error code

    \ingroup group_lte_services*/

static MXRC ApiPlayerCreateOwnBuff(ApiHeader * pApi)
{
    ApiParam *pApiParam;
    int mode = APIPLAY_MODE_DEF;

    pApiParam = ApiGetParamByID(pApi, APIPLAY_INIT_MODE);
    if (pApiParam)
        mode = pApiParam->ParamData[0];

    apiplayer_mode = (apiplayer_mode_t)mode;

    apiplayer_storage = LoggerStorageCreate(&apiplayer_own_buff, API_PLAYER_OWN_BUFF_SIZE);

    if(apiplayer_storage == NULL)
        return RC_APIPLAY_ALLOC_ERROR;

    apiplayer_own_buff_size = API_PLAYER_OWN_BUFF_SIZE;

    return RC_APIPLAY_OK;
}

/** @brief This function handles host requests and configures/queries API player
    @param lpCtx [in] - NULL
    @param pApi  [in] - the API command
    @param pResp [out]- the response

    @return [APIHEC]

    \ingroup group_lte_services*/

APIHEC ApiPlayerApiHandler(LPVOID lpCtx, ApiHeader * pApi, ApiHeader * pResp)
{
    MXRC rc = APIHEC_OK;
	ApiParam *pApiParam;

    if (IS_CFG_API(pApi, APIPLAYER_INIT))
    {
        if(ApiGetParamNum(pApi))
        {
            rc = ApiPlayerCreateOwnBuff(pApi);
        }
    }

    if (IS_CFG_API(pApi, APIPLAYER_LOAD))
    {
        rc = ApiPlayerLoadBuffer(pApi);

        if (rc == MX_OK)
        {
        	pApiParam = ApiGetParamByID(pApi, APIPLAY_PAR_FINISH_PART);
    		if (pApiParam != NULL)
    		{
    		    UINT32 FirstSFN, LastSFN;

	            FirstSFN = ApiPlayerGetFirstSubframeNumber();
	            LastSFN  = ApiPlayerGetLastSubframeNumber();
    	        ApiAddParam(pResp, NULL, APIPLAY_PAR_FIRST_SFN, &FirstSFN, sizeof(UINT32));
    	        ApiAddParam(pResp, NULL, APIPLAY_PAR_LAST_SFN,  &LastSFN,  sizeof(UINT32));
    		}
        }
		else
		{
			uart_printf("\r\nAPI-PLAYER: Load Buffer error %d \r\n", rc);
		}
    }
    else if (IS_CFG_API(pApi, APIPLAYER_START))
    {
        APIPLAYERCTX * pCtx = ApiPlayerGetCtx();
        ApiParam *pApiParam;

        UINT32 StartSFN = 0;
        UINT32 FinishSFN = 0xFFFFFFFF;
        UINT32 PhyTTI = 0; // to use default in START message
        UINT32 bLogSdu = APIPLAYER_DEF_LOG_SDU_CTRL;
        UINT32 nPhyDI  = 0xFF;

        _ASSERT_PTR(pCtx);
        pCtx->bSendStopInd = 1;

        pApiParam = ApiGetParamByID(pApi, APIPLAY_PAR_START_SFN);
        if (pApiParam)
            StartSFN = pApiParam->ParamData[0];

        pApiParam = ApiGetParamByID(pApi, APIPLAY_PAR_SFN_TO_PLAY);
        if (pApiParam)
        {
            FinishSFN = StartSFN + pApiParam->ParamData[0] - 1;
        }

        if ((pApiParam = ApiGetParamByID(pApi, APIPLAY_PAR_SDU_LOG_CTRL)) != NULL)
            bLogSdu = pApiParam->ParamData[0];

        pApiParam = ApiGetParamByID(pApi, APIPLAY_PAR_PHY_TTI);
        if (pApiParam)
            PhyTTI = pApiParam->ParamData[0];

        pApiParam = ApiGetParamByID(pApi, APIPLAY_PAR_PHY_DI);
        if (pApiParam)
            nPhyDI = pApiParam->ParamData[0];

        rc = ApiPlayerStart((PUINT8)ApiPlayerStorage(), StartSFN, FinishSFN, PhyTTI, nPhyDI, bLogSdu);
    }
    else if (IS_CFG_API(pApi, APIPLAYER_STOP))
    {
    }
    else if (IS_QRY_API(pApi, APIPLAYER_GET_INFO))
    {
        UINT32 StorageSize = ApiPlayerStorageSize();
        ApiAddParam(pResp, NULL, APIPLAY_PAR_BUF_SIZE_PARAM_ID, &StorageSize, sizeof(UINT32));
    }

    ApiAddErrorCode(pResp, rc);

    return APIHEC_OK;
}

/** @brief Thi function initializes the API player and has to be calle before usage of any API player API
    @return [MXRC] an error code of operation , MX_OK - OK

    \ingroup group_lte_services */

MXRC ApiPlayerInit(void)
{
    APIPLAYERCTX * pCtx = ApiPlayerGetCtx();
    MXRC rc;
    _ASSERT_PTR(pCtx);

    memset(pCtx, 0, sizeof(*pCtx));

    apiplayer_storage = ApiPlayerStorage();

    if (FAILED(rc = SvsrRegClient (IID_API_PLAYER, ApiPlayerApiHandler, pCtx)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    pCtx->bCtxInit = 1;

    pCtx->hThr = HNULL;

    pCtx->bLogSdu   = APIPLAYER_DEF_LOG_SDU_CTRL;
    pCtx->LogPos    = 0;
    pCtx->LogSize   = UL_SDU_BUF_SIZE;
    pCtx->LogPtr    = (PUINT8)gUlSduBuf;
    pCtx->LogStatus = 0;

    LtePhyLogsAddMask(LTE_BS_PHY_ENABLE_PHYSTATS);

    return MX_OK;
}


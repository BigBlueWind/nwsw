/** @file phystub.c
 *
 * @brief PHY stub for LTE eNodeB
 * @author Mindspeed Technologies
 * @version $Revision: 1.67 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef ENODEB

#include "osal.h"
#include "apidefs.h"
#include "msgroute.h"
#include "ltemac.h"
#include "ltephydef.h"
#include "phyiface.h"
#include "LtePhyL2Api.h"
#include "LteMac2PhyInterface.h"
#include "settings.h"
#include "icpu.h"
#include "diags.h"
#include "auxiface.h"

//
// This ID is present in all MAC types
//
const int Domain4G = PAR_LTE;
const int Mac4G = PAR_BSMAC;

//
// PHY Mode used in PHY_START_REQ
//
int PhyMode = AUTO_TIMER;
int PhyModePeriod = 100;
int PhyModeCount = 0;

extern RESULTCODE (*IcpuSendMsg)(U32 size, PTR data);
extern int IcpuMode;
extern int AuxMode;

#define PHY_QUEUESIZE               16

//
// Task IDs
//
OSAL_TASK PhyToMacTaskID = 0;

//
// Synchronization
//
OSAL_SEMAPHORE PhyTickSema = {0};
OSAL_PADDRESS PhyQueueMem = NULL;
OSAL_QUEUE PhyQueue = {0};
OSAL_SEMAPHORE PhyRxSema = {0};
OSAL_MEMARRAY PhyPart = {0};
OSAL_PADDRESS PhyPartMem = NULL;

//
// PHY context
//
extern DLSUBFRDESC DlSubFrame;
U32 PhyInstanceID = 0;

#if defined (SIMUPC) && defined (USE_WINDOWS)
typedef int (__cdecl* PHYPROC)();

HMODULE PhyLibrary = NULL;
PHYPROC  PhyLteMain = NULL;
// Input I-Q Samples pointer
S16* pPhyInSmp;
// Output I-Q Samples pointer
S16* pPhyOutSmp;
S16* pBufferPhySmp;
#endif


//
// Externals defined in LTE MAC modules
//
extern OSAL_SEMAPHORE LteMacDlSema;
extern OSAL_HEAP OsalGlobalHeap;

#ifdef MAC_PHY_TRACE
static FILE *binfile = NULL;
#endif


//
// PHY Stub state machine
//
U32 ExpectedTxSdu = 0;
U32 ExpectedRxSdu = 0;
U32 TxFrameNumber = 0;
U32 TxSubFrameNumber = 0;
U32 RxFrameNumber = 0;
U32 RxSubFrameNumber = 0;
U32 TxChanId = 0;
U32 RxChanId = 0;

DLSUBFRDESC TxDlSubFrame;

PTR pIMSG_TO_PHY = NULL;
PTR pIMSG_FROM_PHY = NULL;

static U32 tickCounter = 0;

/** MAC sends message to the PHY
 *  Must guarantee data was copied
 *
 *  @param h [IN] Unused
 *  @param pMsg [IN] Pointer to the message data
 *  @param msgSize [IN] Data size
 */
RESULTCODE PhySendMsg(HANDLE h, PTR pMsg, OSAL_SIZE msgSize)
{
#ifndef MAC_RT
    PMSGHEADER pResp = (PMSGHEADER) MsgAllocBuffer(MSG_COMMON);

    if (pResp == NULL)
    {
        MsgFreeBuffer(pMsg);
        return FAILURE;
    }
#else
    PMSGHEADER pResp = (PMSGHEADER) pMsg;
#endif

#if defined (SIMUPC) && defined (USE_WINDOWS)
    if (PhyLibrary && PhyLteMain)
    {
        RESULTCODE ret = FAILURE;
        U32 payloadLen = 0;

        DiagDumpPhyApi(DIAG_LEVEL29, MsgGetDataOffset(pMsg));

        // This is blocking call (LteBsPhyDllMain)
        if (PhyLteMain(PhyInstanceID,
                       NULL,
                       NULL,
                       MsgGetDataOffset(pMsg),
                       msgSize - sizeof(MSGHEADER),
                       MsgGetDataOffset(pResp),
                       &payloadLen)
            == 0)
        {
            ret = SUCCESS;
        }
        // Wrap returned response
        if (payloadLen)
        {
            DiagDumpPhyApi(DIAG_LEVEL29, MsgGetDataOffset(pResp));

            pResp->control = CTRL_BYPASS | CTRL_VERSION; // Encapsulated protocol
			pResp->length = payloadLen;
			pResp->type = MSGT_DATA;
			pResp->srcID = MSGS_LARM | PhyInstanceID;
			pResp->dstID = MSGS_MAC;
			pResp->msgID = PHY_LTE_MSG;
			OsalEnqueue(&PhyQueue, pResp);
			OsalRaiseSema(&PhyRxSema);
        }
        else
        {
            MsgFreeBuffer(pResp);
        }
        return ret;
    }
    else
    {
        pResp->control = CTRL_BYPASS | CTRL_VERSION; // Encapsulated protocol
        // TODO: Is this calculation correct?
        pResp->length = msgSize - sizeof(MSGHEADER);
        pResp->type = MSGT_DATA;
        pResp->srcID = MSGS_MAC;
        pResp->dstID = MSGS_LARM | PhyInstanceID;
        pResp->msgID = PHY_LTE_MSG;
#ifndef MAC_RT
        memcpy(MsgGetDataOffset(pResp), MsgGetDataOffset(pMsg), msgSize - sizeof(MSGHEADER));
        return PhyDispatch(/*MSGS_MAC,*/ msgSize, pResp);
#else
        return PhyDispatch(msgSize - sizeof(MSGHEADER), pResp);
#endif
    }
#else  /* SIMUPC && USE_WINDOWS */
    pResp->control = CTRL_BYPASS | CTRL_VERSION; // Encapsulated protocol
    pResp->length = msgSize - sizeof(MSGHEADER);
    pResp->type = MSGT_DATA;
    pResp->srcID = MSGS_MAC;
    pResp->dstID = MSGS_LARM | PhyInstanceID;
    pResp->msgID = PHY_LTE_MSG;

#ifndef MAC_RT
    memcpy(MsgGetDataOffset(pResp),
           MsgGetDataOffset(pMsg),
           msgSize - sizeof(MSGHEADER));     // Encapsulate foreign payload
#endif
    return MsgDispatch(MSGS_MAC, msgSize, pResp);
#endif /* SIMUPC && USE_WINDOWS */
}

RESULTCODE PhySendMsgEx(HANDLE h, PTR pMsg, OSAL_SIZE msgSize, S16* sigOutput, S16* sigInput)
{
    PMSGHEADER pResp = MsgAllocBuffer(MSG_COMMON);

    if (pResp == NULL)
    {
        return FAILURE;
    }

#if defined (SIMUPC) && defined (USE_WINDOWS)

    if (PhyLibrary && PhyLteMain)
    {
        RESULTCODE ret = FAILURE;
        U32 payloadLen = 0;
        PGENMSGDESC pGenMsg = (pMsg == NULL) ? NULL : MsgGetDataOffset(pMsg);

        if (pGenMsg)
        {
            DiagDumpPhyApi(DIAG_LEVEL29, MsgGetDataOffset(pMsg));
        }

        // This is blocking call and return response immediately
        // LteBsPhyDllMain()
        if (PhyLteMain(PhyInstanceID,
                       sigOutput,
                       sigInput,
                       pGenMsg,
                       msgSize - sizeof(MSGHEADER),
                       MsgGetDataOffset(pResp),
                       &payloadLen)
            == 0)
        {
            ret = SUCCESS;
        }

        // Wrap returned response
        if (payloadLen)
        {
            DiagDumpPhyApi(DIAG_LEVEL29, MsgGetDataOffset(pResp));

            pResp->control = CTRL_BYPASS | CTRL_VERSION; // Encapsulated protocol
            pResp->length = payloadLen;
            pResp->type = MSGT_DATA;
            pResp->srcID = MSGS_LARM | PhyInstanceID;
            pResp->dstID = MSGS_MAC;
            pResp->msgID = PHY_LTE_MSG;

            OsalEnqueue(&PhyQueue, pResp);
            OsalRaiseSema(&PhyRxSema);
        }

        return ret;
    }
    else
    {
        pResp->control = CTRL_BYPASS | CTRL_VERSION; // Encapsulated protocol
        pResp->length = msgSize - sizeof(MSGHEADER);

#ifdef MAC_PHY_TRACE
        fwrite(&msgSize, 1, sizeof(U32), binfile);
        fwrite(pMsg, 1, msgSize, binfile);
        fflush(binfile);
#endif
        // TODO: memcpy is evil
        memcpy(MsgGetDataOffset(pResp), MsgGetDataOffset(pMsg), msgSize - sizeof(MSGHEADER));
        return PhyDispatch(/*MSGS_MAC,*/ msgSize, pResp);
    }

#else  /* SIMUPC && USE_WINDOWS */
    pResp->control = CTRL_BYPASS | CTRL_VERSION; // Encapsulated protocol
    pResp->length = msgSize - sizeof(MSGHEADER);
    pResp->type = MSGT_DATA;
    pResp->srcID = MSGS_MAC;
    pResp->dstID = MSGS_LARM | PhyInstanceID;
    pResp->msgID = PHY_LTE_MSG;

    // FIXME: Remove data copy
    memcpy(MsgGetDataOffset(pResp), MsgGetDataOffset(pMsg), msgSize - sizeof(MSGHEADER));     // Encapsulate foreign payload
    return MsgDispatch(MSGS_MAC, msgSize, pResp);
#endif /* SIMUPC && USE_WINDOWS */
 }

RESULTCODE PhySetRxBufIq( PMSGHEADER pApi, PMSGHEADER pResp)
{
#define PARAM_ID_INVALID          0x0000

    U32 rc = SUCCESS;

#if defined (SIMUPC) && defined (USE_WINDOWS)

    PPARAMHEADER pApiParam;
    UINT8 *pInputSignal;
    UINT32 offset, dataSize;
    UINT32 phySectorId, phyAntId;

    if (pPhyInSmp == NULL )
    {
        return RC_LTE_ALLOC_ERROR;
    }
    // fill inSamp buffer !!
    pInputSignal = (UINT8*)pPhyInSmp;

    // Sector number
    pApiParam = MsgGetParam(pApi, PAR_PHY_SECTOR);
    if (pApiParam != NULL)
    {
        phySectorId = pApiParam->data[0];
    }
    else
    {
        phySectorId = 0;
    }

    // Antenna number
    pApiParam = MsgGetParam(pApi, PAR_PHY_ANT_NUM);
    if (pApiParam != NULL)
    {
        phyAntId = pApiParam->data[0];
    }
    else
    {
        phyAntId = 0;
    }

    // Buffer offset
    pApiParam = MsgGetParam(pApi, PAR_PHY_BUF_OFFSET);
    if (pApiParam != NULL)
    {
        offset = pApiParam->data[0];
    }
    else
    {
        offset = 0;
    }

    // Inbound I/Q samples
    pApiParam = MsgGetParam(pApi, PAR_PHY_DATA);
    if (pApiParam != NULL)
    {
        if (pApiParam->paramID == PARAM_ID_INVALID)
            dataSize = 0;
        else
            dataSize = pApiParam->length;

        //dataSize = ApiGetParamDataSize(pApiParam);;
    }
    else
    {
        return RC_LTE_PARAM_ERROR;
    }

    //Fill InPut I Q Samp  !!
    memcpy((pInputSignal + offset), pApiParam->data, dataSize);

#endif

    return rc;
}

RESULTCODE PhyGetTxBufIq(PMSGHEADER pApi, PMSGHEADER pResp)
{
    U32 rc = SUCCESS;

#if defined (SIMUPC) && defined (USE_WINDOWS)

    PPARAMHEADER pApiParam;
    U32 nSize, nOffs;
    U32 phySectorId = 0;
    U32 antennId = 0;
    U8 *pOutputSignalAx = NULL;

    //
    if (pPhyOutSmp == NULL)
        return RC_LTE_ALLOC_ERROR;

    pOutputSignalAx = (UINT8*)pPhyOutSmp;

    // get sector #
    pApiParam = MsgGetParam(pApi, PAR_PHY_SECTOR);
    if (pApiParam == NULL)
        return RC_LTE_PARAM_ERROR;

    phySectorId = pApiParam->data[0];
    if (phySectorId != 0) // only one sector is supported
        return RC_PHY_INCORRECT_PARAM;

    // get  antenna
    pApiParam = MsgGetParam(pApi, PAR_PHY_ANT_NUM);
    if (pApiParam == NULL)
        return RC_LTE_PARAM_ERROR;

    antennId = pApiParam->data[0];

    // Get Size of requested data
    pApiParam = MsgGetParam(pApi, PAR_PHY_BUF_SIZE);
    if (pApiParam == NULL)
        return RC_LTE_PARAM_ERROR;

    nSize = pApiParam->data[0];

    // Get Offset
    pApiParam = MsgGetParam(pApi, PAR_PHY_BUF_OFFSET);
    if (pApiParam == NULL)
        return RC_LTE_PARAM_ERROR;

    nOffs = pApiParam->data[0];

    if (nSize + nOffs > MAX_LTEIQ)
    {
        printf ("OFFS:%d,  SIZE:%d, TOTAL:%d\r\n", nOffs, nSize, MAX_LTEIQ);

        if(nOffs >= MAX_LTEIQ)
            return RC_LTE_PARAM_ERROR;

        nSize = MAX_LTEIQ - nOffs;
    }

    // put chunk of TX IQ sambles buffer to response

    //MsgAppendParam(pInd, PAR_BIN_DATA, 580, &pRxSduInd->pRxSdu);
    MsgAppendParam(pResp,
            PAR_PHY_DATA,
            nSize,
            pOutputSignalAx + nOffs
            );

    // put sector number to Response
    MsgAppendParam(pResp,
            PAR_PHY_SECTOR,
            sizeof(phySectorId),
            &phySectorId
            );

    // put Antenna number to Response
    MsgAppendParam(pResp,
            PAR_PHY_ANT_NUM,
            sizeof(phySectorId),
            &antennId
            );
#endif

    return rc;
}

RESULTCODE PhyGetDiags(PMSGHEADER pApi, PMSGHEADER pResp)
{
#if defined (SIMUPC) && defined (USE_WINDOWS)

    PPARAMHEADER pApiParam;
    U32 size, offs;
    U32 diagType;
    U32 dataBufSize;

    pApiParam = MsgGetParam(pApi, DIAG_BUF_SIZE_PARAM_ID);
    if (pApiParam == NULL)
        return RC_LTE_PARAM_ERROR;
    size = pApiParam->data[0];

    pApiParam = MsgGetParam(pApi, DIAG_BUF_OFFS_PARAM_ID);
    if (pApiParam == NULL)
        return RC_LTE_PARAM_ERROR;
    offs = pApiParam->data[0];

    pApiParam = MsgGetParam(pApi, DIAG_DATA_TYPE);
    if (pApiParam == NULL)
        return RC_LTE_PARAM_ERROR;
    diagType = pApiParam->data[0];
    dataBufSize = pApiParam->data[1];

    switch (diagType)
    {
        case PROFILE_INFO:
            // Add some fake data at this moment
            MsgAppendParam(pResp, DIAG_DATA_PARAM_ID, size, pPhyOutSmp);
            break;

        case IQ_SAMPLES:
            MsgAppendParam(pResp, DIAG_DATA_PARAM_ID, size, ((U8*)pPhyOutSmp) + offs);
            break;
    }
#endif // SIMU_PC && USE_WINDOWS

    return SUCCESS;
}

RESULTCODE PhySendData(HANDLE h, PTR pMsg, OSAL_SIZE msgSize)
{
    PMSGHEADER pHdr = (PMSGHEADER) MsgAllocBuffer(MSG_COMMON);

    pHdr->control = CTRL_VERSION;
    pHdr->length = msgSize;
    pHdr->type = MSGT_CONFIG;
    pHdr->srcID = MSGS_MAC;
    pHdr->dstID = MSGS_LARM | PhyInstanceID;
    pHdr->msgID = PHY_LTE_DEBUG_MSG;

    // FIXME: Remove data copy
    memcpy(&pHdr->param[0], pMsg, msgSize);
    // TODO: Free pMsg ?
    return MsgDispatch(MSGS_MAC, sizeof(MSGHEADER) + msgSize, pHdr);
}

/*! \brief Get next message from the PHY
 *         Must be blocking call
 *
 *  \param h [IN] Not used
 *  \param pMsg [IN/OUT] Pointer to the memory area which receive data
 *  \param pMsgSize [OUT] Data size
 *  \return Standard result code
*/
RESULTCODE PhyGetMsg(HANDLE h, PTR pMsg, OSAL_SIZE *pMsgSize)
{
    PMSGHEADER pHdr;

    if (OsalWaitForSema(&PhyRxSema, OSAL_WAIT_INFINITE) == SUCCESS)
    {
        if (OsalDequeue(&PhyQueue, (OSAL_PADDRESS *) &pHdr) == SUCCESS)
        {
            if (pHdr->control & CTRL_BYPASS)
            {
                // TODO: Dispatch

                // Copy payload
                memcpy(pMsg, MsgGetDataOffset(pHdr), pHdr->length);
                *pMsgSize = pHdr->length;

                if( (OsalFreeMemArray(&PhyPart, pHdr)) != SUCCESS)
                {
                    // maybe allocate from MsgArray in "IcpuRxTaskShm(PTR)"
                    OsalFreeMemArray(&MsgArray, pHdr);
                }
                return SUCCESS;
            }
            else
            {
                // TODO: What else?
            }
            OsalFreeMemArray(&PhyPart, pHdr);
        }
    }
    return FAILURE;
}

/** Not needed when real PHY is used
 */
void PhySendToMacMsg(U8 MsgType, U8* pData, U32 len)
{
    PMSGHEADER pHdr;
    PGENMSGDESC pToMacMsg;
    PRXSDUIND pRxSduMsg;
    PTXSDUIND pTxSduInd;
    OSAL_SIZE msgSize = 0;

    if (OsalAllocMemArray((OSAL_MEMARRAY *)&PhyPart,
                          (OSAL_PADDRESS *)&pHdr) != SUCCESS)
    {
        fprintf(stderr, C4KFW "ERROR: No memory [%s]\n", __FUNCTION__);
        return;
    }
    pToMacMsg = MsgGetDataOffset(pHdr);

    pToMacMsg->phyEntityId = 0;
    pToMacMsg->msgType = MsgType;
    switch (MsgType)
    {
    case PHY_TXSTART_CONF:
        if (pData)
        {
            ((PMSGIND)pToMacMsg)->subFrameNum = ((PMSGIND)pData)->subFrameNum;
            ((PMSGIND)pToMacMsg)->frameNumber = ((PMSGIND)pData)->frameNumber;
            ((PMSGIND)pToMacMsg)->status = ((PMSGIND)pData)->status;
        }
        else
        {
            ((PMSGIND)pToMacMsg)->subFrameNum = TxSubFrameNumber;
            ((PMSGIND)pToMacMsg)->frameNumber = TxFrameNumber;
            ((PMSGIND)pToMacMsg)->status = SUCCESS;
        }
        msgSize = sizeof(MSGIND);
        break;
    case PHY_TXSTART_IND:
        ((PMSGIND)pToMacMsg)->subFrameNum = TxSubFrameNumber;
        ((PMSGIND)pToMacMsg)->frameNumber = TxFrameNumber;
        ((PMSGIND)pToMacMsg)->status = SUCCESS;
        msgSize = sizeof(MSGIND);
        break;
    case PHY_TXSDU_CONF:
        pTxSduInd = (PTXSDUIND) pToMacMsg;
        if (pData)
        {
            pTxSduInd->status = ((PTXSDUIND)pData)->status;
            pTxSduInd->subFrameNum = TxSubFrameNumber;
            pTxSduInd->frameNumber = TxFrameNumber;
            pTxSduInd->chanId = ((PTXSDUIND)pData)->chanId;
        }
        else
        {
            pTxSduInd->status = SUCCESS;
            pTxSduInd->subFrameNum = TxSubFrameNumber;
            pTxSduInd->frameNumber = TxFrameNumber;
            pTxSduInd->chanId = TxChanId;
        }
        msgSize = sizeof(TXSDUIND);
        break;
    case PHY_TXEND_IND:
        ((PMSGIND)pToMacMsg)->subFrameNum = TxSubFrameNumber;
        ((PMSGIND)pToMacMsg)->status = SUCCESS;
        msgSize = sizeof(MSGIND);
        break;
    case PHY_RXSTART_CONF:
        if (pData)
        {
            ((PMSGIND)pToMacMsg)->subFrameNum = ((PMSGIND)pData)->subFrameNum;
            ((PMSGIND)pToMacMsg)->status = ((PMSGIND)pData)->status;
        }
        else
        {
            ((PMSGIND)pToMacMsg)->subFrameNum = RxSubFrameNumber;
            ((PMSGIND)pToMacMsg)->status = SUCCESS;
        }
        msgSize = sizeof(MSGIND);
        break;
    case PHY_RXSTART_IND:
        msgSize = sizeof(MSGIND);
        break;
    case PHY_RXSDU_IND:
        if (pData)
        {
            pRxSduMsg = (PRXSDUIND) pToMacMsg;
            pRxSduMsg->numBitsRx = len << 3;
            pRxSduMsg->channelId = RxChanId;
            pRxSduMsg->status = SUCCESS;
            pRxSduMsg->subFrameNum = RxSubFrameNumber;
            pRxSduMsg->frameNum = RxFrameNumber;
            memcpy(&pRxSduMsg->pRxSdu, pData, len);
            msgSize = sizeof(RXSDUIND) + len;
        }
        else
        {
            return;
        }
        break;
    case PHY_RXEND_IND:
        ((PMSGIND)pToMacMsg)->subFrameNum = RxSubFrameNumber;
        ((PMSGIND)pToMacMsg)->frameNumber = RxFrameNumber;
        msgSize = sizeof(MSGIND);
        break;
    case PHY_INIT_IND:
        ((PINITIND)pToMacMsg)->status = SUCCESS;
        msgSize = sizeof(INITIND);
        break;
    default:
        pToMacMsg->msgType = MsgType;
        msgSize = sizeof(GENMSGDESC);
        break;
    }

    pHdr->control = CTRL_BYPASS | CTRL_VERSION;
    pHdr->length = msgSize;
    if (OsalEnqueue(&PhyQueue, pHdr) != SUCCESS)
    {
        printf(C4KFW "ERROR: Queue full [%s]\n", __FUNCTION__);
        OsalFreeMemArray(&PhyPart, pHdr);
    }
    else
    {
        OsalRaiseSema(&PhyRxSema);
    }
}

RESULTCODE PhySendToPhyMsg(U8 msgType, PTR data)
{
    PMSGHEADER pHdr = (PMSGHEADER) MsgAllocBuffer(MSG_COMMON);

    if (pHdr == NULL)
        return NULL;

    pHdr->control = CTRL_BYPASS | CTRL_VERSION;
    pHdr->length = 0;
    pHdr->type = MSGT_DATA;
    pHdr->srcID = MSGS_LARM | PhyInstanceID;    // ???
    pHdr->dstID = MSGS_LARM | PhyInstanceID;
    pHdr->msgID = PHY_LTE_DEBUG_MSG;

    switch (msgType)
    {
    case PHY_RXSTART_IND:
        {
            PMSGIND pDst = MsgGetDataOffset(pHdr);

            pDst->phyEntityId = 0;
            pDst->msgType = PHY_RXSTART_IND;
            pDst->status = SUCCESS;
            pDst->subFrameNum = TxSubFrameNumber;   // ???

            pHdr->length = sizeof(MSGIND);
        }
        break;
    case PHY_RXSDU_IND:
        {
            PRXSDUIND pDst = MsgGetDataOffset(pHdr);
            PTXSDUREQ pSrc = (PTXSDUREQ) data;

            pDst->channelId = pSrc->chanId;
            pDst->msgType = PHY_RXSDU_IND;
            pDst->phyEntityId = pSrc->phyEntityId;
            pDst->numBitsRx = pSrc->msgLen * 8;
            pDst->status = SUCCESS;
            pDst->frameNum = 0;     // TODO: Fill in on receiving side
            pDst->subFrameNum = 0;  // TODO: Fill in on receiving side
            pDst->chanType = TxDlSubFrame.dlCh[pSrc->chanId].channelType;
            printf("chanType %d\n", pDst->chanType);
            memcpy(&pDst->pRxSdu, &pSrc->pTxSdu + 1 , pSrc->msgLen);
       //     memcpy(&pDst->pRxSdu, ((U8*)pSrc + sizeof(TXSDUREQ)), pSrc->msgLen);

            pHdr->length = sizeof(RXSDUIND) + pSrc->msgLen;
        }
        break;
    case PHY_RXEND_IND:
        {
            PMSGIND pDst = MsgGetDataOffset(pHdr);

            pDst->phyEntityId = 0;
            pDst->msgType = PHY_RXEND_IND;
            pDst->status = SUCCESS;
            pDst->subFrameNum = TxSubFrameNumber;   // ???
            pDst->frameNumber = TxFrameNumber;      // ???

            pHdr->length = sizeof(MSGIND);
        }
        break;
    case PHY_START_REQ:
        {
            PGENMSGDESC pDst = MsgGetDataOffset(pHdr);
            pDst->msgType = PHY_START_REQ;
            pDst->msgSpecific = 0;
            pDst->phyEntityId = 0;

            pHdr->length = sizeof(GENMSGDESC);
        }
        break;
    case PHY_STOP_REQ:
        {
            PGENMSGDESC pDst = MsgGetDataOffset(pHdr);
            pDst->msgType = PHY_STOP_REQ;
            pDst->msgSpecific = 0;
            pDst->phyEntityId = 0;

            pHdr->length = sizeof(GENMSGDESC);
        }
        break;
    default:
        return FAILURE;
    }
    return MsgDispatch(MSGS_UARM, sizeof(MSGHEADER) + pHdr->length, pHdr);
}

/** This is stub task which will trigger PHY -> MAC process by timer
 *
 * @param data Not used
 */
void PhyToMacTask(PTR data)
{
    while (1)
    {
        if (OsalWaitForSema(&PhyTickSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            OsalRaiseSema(&LteMacDlSema);
        }
        else
        {
            // Error, stop task
            break;
        }
    }
    PhyToMacTaskID = 0;
}

RESULTCODE PhyInit(void)
{
#if defined (SIMUPC) && defined (USE_WINDOWS)
	int phylib = 0;

    // Check if PHY DLL is present
	ReadSettingInt("phylib", &phylib, phylib);
	if (phylib)
		PhyLibrary = LoadLibrary("LteBsPhy.dll");
    if (PhyLibrary)
    {
		// Allocate memory for Input Samples
		if (pPhyInSmp == NULL)
		{
			pPhyInSmp = (S16*) malloc(MAX_LTEIQ);
			memset((U8 *)pPhyInSmp, 0, MAX_LTEIQ);
		}

		// Allocate memory for Output Samples
		if (pPhyOutSmp == NULL)
		{
			pPhyOutSmp = (S16*) malloc(MAX_LTEIQ);
			memset((U8 *)pPhyOutSmp, 0, MAX_LTEIQ);
		}

		// Allocate memory for Sample Reading/Writing msg.
		if (pBufferPhySmp == NULL)
			pBufferPhySmp = (S16*) malloc(PHY_IQ_CHUNK + sizeof(MSGHEADER) );

		PhyLteMain = (PHYPROC)GetProcAddress(PhyLibrary, "LteBsPhyDllMain");
    }
#endif

#ifdef MAC_PHY_TRACE
    binfile= fopen("tophy.bin","wb");
    if (binfile == NULL)
        fprintf(stderr, C4KFW "ERROR: Open MAC PHY Api trace file\n");
#endif

    PhyInstanceID = MSGS_LTEPHY;

    OsalCreateSema(&PhyTickSema);

    OsalAllocHeap(&OsalGlobalHeap, &PhyQueueMem, PHY_QUEUESIZE * sizeof(OSAL_PADDRESS));
    OsalCreateQueue(&PhyQueue, PhyQueueMem, PHY_QUEUESIZE * sizeof(OSAL_PADDRESS));

    OsalAllocHeap(&OsalGlobalHeap, &PhyPartMem, MSG_MAXSIZE * PHY_QUEUESIZE);
    OsalCreateMemArray(&PhyPart, PhyPartMem, MSG_MAXSIZE * PHY_QUEUESIZE, MSG_MAXSIZE);

    OsalCreateTask(&PhyToMacTaskID, PhyToMacTask, NULL, "PhyToMacTask", OSAL_PRIORITY_NORMAL, 0, 0);
    OsalActivateTask(&PhyToMacTaskID);

    return SUCCESS;
}

RESULTCODE PhyEnable(U16 *instID)
{
    if (PhyInstanceID == 0)
    {
        PhyInit();
    }
    if (instID != NULL)
    {
        *instID = (U16) PhyInstanceID;
    }
    return LteMacSendToPhyMessage(PHY_INIT_REQ, NULL);
}

RESULTCODE PhyDone(void)
{
#ifdef MAC_PHY_TRACE
    fclose(binfile);
    binfile = NULL;
#endif

    if (PhyToMacTaskID)
    {
        OsalDestroyTask(&PhyToMacTaskID);
        PhyToMacTaskID = 0;
    }
    OsalDestroySema(&PhyTickSema);
    OsalDestroySema(&PhyRxSema);
    if (PhyQueueMem)
    {
        OsalDestroyQueue(&PhyQueue);
        OsalFreeHeap(&OsalGlobalHeap, PhyQueueMem);
        PhyQueueMem = NULL;
    }
    if (PhyPartMem)
    {
        OsalDestroyMemArray(&PhyPart);
        OsalFreeHeap(&OsalGlobalHeap, PhyPartMem);
        PhyPartMem = NULL;
    }
#if defined (SIMUPC) && defined (USE_WINDOWS)
    if (PhyLibrary)
    {
        FreeLibrary(PhyLibrary);
        PhyLibrary = NULL;
        PhyLteMain = NULL;
    }
#endif
    PhyInstanceID = 0;

    return SUCCESS;
}

void PhyTxStart(PGENMSGDESC msg)
{
    MSGIND msgData;
    PDLSUBFRDESC pSubFrame = (PDLSUBFRDESC) (msg + 1);
    U32 len;

    len = sizeof(DLSUBFRDESC);
    len -= sizeof(DLCHANDESC) * MAXCHSUBFRAME;
    len += sizeof(DLCHANDESC) * pSubFrame->numberofChannelDescriptors;

    memcpy(&TxDlSubFrame, pSubFrame, len);

    if (ExpectedTxSdu > 0)
    {
        // Error, we did not finish previous transaction
        msgData.subFrameNum = pSubFrame->subframeNumber;
        msgData.status = OVERRUN;
        msgData.frameNumber = pSubFrame->frameNumber;
        PhySendToMacMsg(PHY_TXSTART_CONF, (U8*) &msgData, sizeof(msgData));
    }
    else
    {
        ExpectedTxSdu = pSubFrame->numberofChannelDescriptors;
        TxFrameNumber = pSubFrame->frameNumber;
        TxSubFrameNumber = pSubFrame->subframeNumber;
        PhySendToMacMsg(PHY_TXSTART_CONF, NULL, 0);
        // Inform peer we're started
        PhySendToPhyMsg(PHY_RXSTART_IND, NULL);
    }
}

void PhyTxSdu(PGENMSGDESC msg)
{
    TXSDUIND msgData;
    PTXSDUREQ pSdu = (PTXSDUREQ) msg;

    if (ExpectedTxSdu > 0)
    {
        TxChanId = pSdu->chanId;
        PhySendToMacMsg(PHY_TXSDU_CONF, NULL, 0);
        // TXSDU.req becomes RXSDU.ind on other side
        PhySendToPhyMsg(PHY_RXSDU_IND, msg);
        if (--ExpectedTxSdu == 0)
        {
            PhySendToMacMsg(PHY_TXSTART_IND, NULL, 0);
            PhySendToMacMsg(PHY_TXEND_IND, NULL, 0);
            // Inform peer we're done
            PhySendToPhyMsg(PHY_RXEND_IND, NULL);
        }
    }
    else
    {
        // Error, we do not expect SDU at this time
        msgData.status = OVERRUN;
        msgData.chanId = pSdu->chanId;
        PhySendToMacMsg(PHY_TXSDU_CONF, (U8*) &msgData, sizeof(msgData));
    }
}

void PhyBypass(PGENMSGDESC msg)
{
    MSGIND msgData;

    switch (msg->msgType)
    {
    case PHY_RXSTART_IND:
        msgData.subFrameNum = ((PMSGIND) msg)->subFrameNum;
        PhySendToMacMsg(PHY_RXSTART_IND, (U8*) &msgData, sizeof(msgData));
    break;
    }
}

void PhyRxStart(PGENMSGDESC msg)
{
    PULSUBFRDESC pSubFrame = (PULSUBFRDESC) (msg + 1);

    ExpectedRxSdu = pSubFrame->numberofChannelDescriptors;
    RxFrameNumber = pSubFrame->frameNumber;
    RxSubFrameNumber = pSubFrame->subframeNumber;
    PhySendToMacMsg(PHY_RXSTART_CONF, NULL, 0);
}

void PhyRxSdu(PMSGHEADER hdr, PGENMSGDESC msg, OSAL_SIZE len)
{
    PMSGHEADER pHdr;

    if (OsalAllocMemArray(&PhyPart, (OSAL_PADDRESS *) &pHdr) != SUCCESS)
    {
        fprintf(stderr, C4KFW "ERROR: No memory [%s]\n", __FUNCTION__);
        return;
    }
    // TODO: memcpy is evil
    memcpy(MsgGetDataOffset(pHdr), msg, len);

    pHdr->control = CTRL_BYPASS | CTRL_VERSION;
    pHdr->length = len;
    if (OsalEnqueue(&PhyQueue, pHdr) != SUCCESS)
    {
        fprintf(stderr, C4KFW "ERROR: Queue full [%s]\n", __FUNCTION__);
        OsalFreeMemArray(&PhyPart, pHdr);
    }
    else
    {
        OsalRaiseSema(&PhyRxSema);
    }
}

RESULTCODE PhyDispatch(U32 size, PTR msg)
{
    PMSGHEADER pHdr = (PMSGHEADER) msg;
    PGENMSGDESC pMsg;

    if (IcpuMode == IMODE_HBI || AuxMode == AUX_REMOTE)
    {
        return IcpuSendMsg(size, msg);
    }

    if (pHdr->control & CTRL_BYPASS)
    {
        pMsg = (PGENMSGDESC) &pHdr->param[0];
        switch (pMsg->msgType)
        {
        case PHY_TXSTART_REQ:
            PhyTxStart(pMsg);
            break;
        case PHY_TXSTART_CONF:
            break;
        case PHY_TXSTART_IND:
            break;
        case PHY_TXSDU_REQ:
            PhyTxSdu(pMsg);
            break;
        case PHY_TXSDU_CONF:
            break;
        case PHY_TXEND_IND:
            break;
        case PHY_RXSTART_REQ:
            PhyRxStart(pMsg);
            break;
        case PHY_RXSTART_CONF:
            break;
        case PHY_RXSTART_IND:
            // Bypass message to MAC
            PhyBypass(pMsg);
            break;
        case PHY_RXSDU_IND:
            PhyRxSdu(pHdr, pMsg, size);
            break;
        case PHY_RXEND_IND:
            break;
        case PHY_INIT_REQ:
            PhySendToMacMsg(PHY_INIT_IND, NULL, 0);
            break;
        case PHY_INIT_IND:
            break;
        default:
            return FAILURE;
            }
        }

    MsgFreeBuffer(msg);

    return SUCCESS;
}


#if defined (SIMUPC) && defined (USE_WINDOWS)

RESULTCODE PhyTransfer(U32 size, PTR msg)
{
    U32 rc = SUCCESS;
    PMSGHEADER pHdr = (PMSGHEADER) msg;
    PMSGHEADER pRsp = NULL;

    pRsp = MsgInitReply(pHdr);
    if (!pRsp)
    {
        MsgFreeBuffer(msg);
        return FAILURE;
    }

    if (pHdr->control & CTRL_BYPASS)
    {
#if defined (SIMUPC) && defined (USE_WINDOWS)
        if (PhyInstanceID == 0)
            PhyInit();
#endif
        PhySendMsg(NULL, msg, size);
    }
    else if (pHdr->type == MSGT_CONFIG)
    {
        switch (pHdr->msgID)
        {
            // Old bit-exact test
            case PHY_LTE_DEBUG_MSG:
                // This message triggers the start of the PHY TX Processing
                rc = PhySendMsgEx(NULL, NULL, sizeof(MSGHEADER), pPhyOutSmp, pPhyInSmp);
                break;

            case PHY_LTE_PHYTICK_MSG:
                // emulation of Antenna interface event for FDX test
                rc = PhySendMsgEx(NULL, NULL, sizeof(MSGHEADER), pPhyOutSmp, pPhyInSmp);

                // This message triggers the start of the Phy Tx and RX Processing
                //rc = LteBsTx(0, pOutSmp, OutMsgArea, &OutSize);
                //rc = LteBsRx(0, pInSmp, OutMsgArea, &OutSize);

                tickCounter++;
                DiagPrint(DIAG_LEVEL28, "TTI", "Frame #%d processed\n", tickCounter);
                break;

            // Old bit-exact test
            case PHY_LTE_CPTIBILITY_MSG:
                break;

            case PHY_LTE_RX_IQ:
                rc = PhySetRxBufIq(pHdr, pRsp);
                break;

            default:
                break;
        }
    }
    else if (pHdr->type == MSGT_QUERY)
    {
        switch (pHdr->msgID)
        {
            case PHY_LTE_TX_IQ:
                rc = PhyGetTxBufIq(pHdr, pRsp);
                break;

            case DIAG_OUTPUT:
                rc = PhyGetDiags(pHdr, pRsp);
                break;

            default:
                break;
        }
    }

    if (pRsp)
    {
        MsgResult(pRsp, rc);
        MsgDispatch(pRsp->srcID, pRsp->length + sizeof(MSGHEADER), pRsp);
    }

    MsgFreeBuffer(msg);

    return rc;
}
#endif


#endif /* ENODEB */

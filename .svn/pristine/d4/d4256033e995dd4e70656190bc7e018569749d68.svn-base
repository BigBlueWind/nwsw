//-------------------------------------------------------------------------------------------
/** @file LteMsgMgr.c
 *
 * @brief This file implements the Lte Message Manager for the L2-PHY API
 * @author Mindspeed Technologies
 * @version $Revision: 1.154 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"
#include "mlog.h"

#ifdef _WIN32
#include "Appids.h"
#include "svsrlogger_c_code.h"
#else
#include "supervisor.h"
#include "lte_entry.h"
#include "LteBsRxDataSdu.h"
#include "LteBsApiThread.h"
#include "LteBsApiErrorCheck.h"
#include "ltertconfig.h"
#include "LteBsRxSduPreproc.h"
#endif

#define PRACH_WORKAROUND 0
#define PRACH_THRESHOLD_REPORTING 1

#if defined(_LTEAPIDUMP_) && !defined(_WIN32)

//#define printing printf
#define printing uart_printf

#define TXEN 0x00000004

extern UINT32 SvsrDumpMask;

static void LteApiDumpOut(U32* outMsg, U32 outMsgSize)
{
    PINITIND ptr = (PINITIND) outMsg;
    PMSGIND pMsg = (PMSGIND)outMsg;

    switch (ptr->msgType)
    {
        case PHY_TXSTART_CONF:
        printing("\n*[PHY_TXSTART_CONF]:\n");
        break;
        case PHY_TXSTART_IND:
        printing("\n*[PHY_TXSTART_IND]:\n");
        break;
        case PHY_TXEND_IND:
        printing("\n*[PHY_TXEND_IND]:\n");
        break;
        case PHY_RXSTART_CONF:
        printing("\n*[PHY_RXSTART_CONF]:\n");
        break;
        case PHY_RXSTART_IND:
        printing("\n*[PHY_RXSTART_IND]:\n");
        break;
        case PHY_RXEND_IND:
        printing("\n*[PHY_RXEND_IND]:\n");
        break;
        case PHY_INIT_IND:
        printing("\n*[PHY_INIT_IND]:\n");
        break;
        case PHY_START_CONF:
        printing("\n*[PHY_START_CONF]:\n");
        break;
        case PHY_STOP_CONF:
        printing("\n*[PHY_STOP_CONF]:\n");
        break;
        case PHY_STOP_IND:
        printing("\n*[PHY_STOP_IND]:\n");
        break;
        case PHY_SHUTDOWN_CONF:
        printing("\n*[PHY_SHUTDOWN_CONF]:\n");
        break;
        default:
        return;
    }

    printing("frameNumber: %d, msgType: %d, phyEntityId: %d, status: %d, subFrameNum: %d\n", (UINT32)(pMsg->frameNumber), (UINT32)(pMsg->msgType), (UINT32)(pMsg->phyEntityId), (UINT32)(pMsg->status), (UINT32)(pMsg->subFrameNum));
}

#endif

//-------------------------------------------------------------------------------------------
/** @brief Generates messages according to the L2-PHY API
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Physical Layer Descriptor
 *
 *  @param outMsg Pointer to the Storage memory for the output message
 *
 *  @param outMsgSize Pointer to the storage for Message size
 *
 *  @param msgType Specific API message to be generated
 *
 *  @param status Status to be reported back
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |              |        |                   |            |               |         |
 *  -------------------------------------------------------------------------------------------------------
 *  |phyInstance |         |Input         |1       |unsigned int       |(1:31:0)    |[0,2]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |outMsg      |         |pInput/pOutput|1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |outMsgSize  |         |pInput/pOutput|1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |msgType     |         |Input         |1       |unsigned int       |(1:31:0)    |[1,LASTAPIMSG]|      |
 *  -------------------------------------------------------------------------------------------------------
 *  |status      |         |Output        |1       |unsigned int       |(1:31:0)    | [0,1]        |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteGenerateAPIMsg           |
 *  ---------------------------------------------------
 *  |Input Streams       |msgType,                    |
 *  ---------------------------------------------------
 *  |Output Streams      |outMsg,OutMsgSize           |
 *  ---------------------------------------------------
 *  |Config Parameters   |phyInstance                 |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function generates an LteL2PhyAPI message as stated by its fourth
 *  parameter for the phyInstance contained in the first parameter
 *  The message complies with the format specified in document MSA-E81613
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * MSA-81613
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void LteGenerateAPIMsg(U32 phyInstance, U32* outMsg, U32* outMsgSize, U32 msgType, U32 status)
{
    PINITIND pInitMsgInd;
    PMSGIND pMsg;
    U32 *pOutMsg, *pTxStartIndTime;

#ifdef _WIN32
    pOutMsg = outMsg;
#else
    U32 tt = GET_TICKS;

    pOutMsg = (U32 *)Phy2MacIfApiBuffer(sizeof(MSGIND) + 32);
#endif

    pInitMsgInd                 = (PINITIND) pOutMsg;
    pInitMsgInd->phyEntityId    = (U8) phyInstance;
    pInitMsgInd->msgType        = (U8) msgType;
    pInitMsgInd->status         = (U8) status;
    pMsg                        = (PMSGIND) pOutMsg;

    // Switch on msgType
    switch (msgType)
    {
        case PHY_TXSTART_CONF:
            pMsg->frameNumber   = LteBsGetFrameNum(gLastTxSfn);
            pMsg->subFrameNum   = LteBsGetSubFrameNum(gLastTxSfn);
            *outMsgSize         = sizeof(INITIND);
        break;
        case PHY_TXSTART_IND:
            pMsg->frameNumber   = LteBsGetFrameNum(gLastTxSfn);
            pMsg->subFrameNum   = LteBsGetSubFrameNum(gLastTxSfn);
            *outMsgSize         = (sizeof(MSGIND)+4);
            pTxStartIndTime     = (U32*)(pMsg+1);
            *pTxStartIndTime    = GET_TICKS;
        break;
        case PHY_TXEND_IND:
            pMsg->frameNumber   = LteBsGetFrameNum(gLastTxSfn);
            pMsg->subFrameNum   = LteBsGetSubFrameNum(gLastTxSfn);
            *outMsgSize         = sizeof(INITIND);
        break;
        case PHY_RXSTART_CONF:
            pMsg->frameNumber   = LteBsGetFrameNum(gLastRxSfn);
            pMsg->subFrameNum   = LteBsGetSubFrameNum(gLastRxSfn);
            *outMsgSize         = sizeof(INITIND);
        break;
        case PHY_RXSTART_IND:
            pMsg->frameNumber   = LteBsGetFrameNum(gLastRxSfn);
            pMsg->subFrameNum   = LteBsGetSubFrameNum(gLastRxSfn);
            *outMsgSize         = sizeof(INITIND);
        break;
        case PHY_RXEND_IND:
            pMsg->frameNumber   = LteBsGetFrameNum(gLastRxSfn);
            pMsg->subFrameNum   = LteBsGetSubFrameNum(gLastRxSfn);
            *outMsgSize         = sizeof(MSGIND);
        break;
        case PHY_INIT_IND:
            // In this case the common processing sets whole indication
            // message so no additional work required here
            pMsg->frameNumber   = LteBsGetFrameNum(gLastTxSfn);
            pMsg->subFrameNum   = LteBsGetSubFrameNum(gLastTxSfn);
            *outMsgSize         = sizeof(INITIND);
        break;
        case PHY_START_CONF:
            pMsg->frameNumber   = LteBsGetFrameNum(gLastTxSfn);
            pMsg->subFrameNum   = LteBsGetSubFrameNum(gLastTxSfn);
            *outMsgSize         = sizeof(INITIND);
        break;
        case PHY_STOP_CONF:
            pMsg->frameNumber   = LteBsGetFrameNum(gLastTxSfn);
            pMsg->subFrameNum   = LteBsGetSubFrameNum(gLastTxSfn);
            *outMsgSize         = sizeof(INITIND);
        break;
        case PHY_STOP_IND:
            pMsg->frameNumber   = LteBsGetFrameNum(gLastTxSfn);
            pMsg->subFrameNum   = LteBsGetSubFrameNum(gLastTxSfn);
            *outMsgSize         = sizeof(MSGIND);
        break;
        case PHY_SHUTDOWN_CONF:
            pMsg->frameNumber   = LteBsGetFrameNum(gLastTxSfn);
            pMsg->subFrameNum   = LteBsGetSubFrameNum(gLastTxSfn);
            *outMsgSize         = sizeof(MSGIND);
        break;
        case PHY_RECONFIG_CNF:
            pMsg->frameNumber   = LteBsGetFrameNum(gLastTxSfn);
            pMsg->subFrameNum   = LteBsGetSubFrameNum(gLastTxSfn);
            *outMsgSize         = sizeof(MSGIND);
        break;
        case PHY_INTRA_TTI_IND:
            pMsg->frameNumber   = LteBsGetFrameNum(gLastTxSfn);
            pMsg->subFrameNum   = LteBsGetSubFrameNum(gLastTxSfn);
            *outMsgSize         = sizeof(MSGIND);
        break;
        case PHY_LOGS_ENABLE_IND:
            pMsg->frameNumber   = LteBsGetFrameNum(gLastTxSfn);
            pMsg->subFrameNum   = LteBsGetSubFrameNum(gLastTxSfn);
            *outMsgSize         = sizeof(MSGIND);
        break;
        default:
            // Return Failure status
            pInitMsgInd->status = FAILURE;
        break;
    }

    // Now message is ready to be send
    if (*outMsgSize)
    {
#ifndef _WIN32
        if (SvsrLoggerGetMask() & (1<<msgType))
        {
#if 1
            SvsrLoggerAddMsg(msgType, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC),
                        pOutMsg, *outMsgSize);
#else
            LtePhyMessagePush(pOutMsg, FIFO_PHY_LOG_AND_CLEAN_API,
                 PHY_API_CLEANUP__PHY_TO_MAC_OTHER_MSG, *outMsgSize);

            LtePhyScheduleApiLogThread();
#endif
        }
        MLogTask(PID_MSGMGR_OTHER_BUILD_MSG, RESOURCE_LARM, tt, GET_TICKS);

        tt = GET_TICKS;
        LteSendMsgToMac(pOutMsg, *outMsgSize);
        MLogTask(PID_MSGMGR_OTHER_SEND_MSG, RESOURCE_LARM, tt, GET_TICKS);
#else
        SvsrLoggerAddMsg(msgType, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC),
                                        pOutMsg, *outMsgSize);
#endif
    }

#if defined(_LTEAPIDUMP_) && !defined(_WIN32)
    if ((SvsrDumpMask & TXEN) && (*outMsgSize != 0))
    LteApiDumpOut(pOutMsg, *outMsgSize);
#endif

}

//-------------------------------------------------------------------------------------------
/** @brief Generates TxSdu confirmation messages according to
 *         the L2-PHY API defined in MSA-E81613
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Physical Layer Descriptor
 *
 *  @param outMsg Pointer to the Storage memory for the output message
 *
 *  @param outMsgSize Pointer to the storage for Message size
 *
 *  @param status Status to be reported back
 *
 *  @param chanId Channel identifier for the TxSdu confirmation message
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |              |        |                   |            |               |         |
 *  -------------------------------------------------------------------------------------------------------
 *  |phyInstance |         |Input         |1       |unsigned int       |(1:31:0)    |[0,2]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |outMsg      |         |pInput/pOutput|1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |outMsgSize  |         |pInput/pOutput|1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |chanId      |         |Input         |1       |unsigned int       |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |status      |         |Output        |1       |unsigned int       |(1:31:0)    | [0,1]        |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteGenerateTxSduConfMsg     |
 *  ---------------------------------------------------
 *  |Input Streams       |chanId                      |
 *  ---------------------------------------------------
 *  |Output Streams      |outMsg,outMsgSize           |
 *  ---------------------------------------------------
 *  |Config Parameters   |phyInstance                 |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function generates an LteL2PhyAPI TxSdu confirmation message with the
 *  status contained in its fourth parameter
 *  The message complies with the format specified in document MSA-E81613
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * MSA-81613
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void LteGenerateTxSduConfMsg(U32 phyInstance, U32* outMsg, U32* outMsgSize, U32 status, U32 chanId)
{
    PLTEPHYDESC pPhy = NULL;
    PBS_TX pBsTx;
    PTXSDUIND pMsg;

    // First build the common part of the reply message and setup PhyInstance
    pPhy = getPhyCtxApiProc(phyInstance);
    pBsTx = pPhy->pBsTx;
    pMsg = (PTXSDUIND) outMsg;
    pMsg->phyEntityId = (U8) phyInstance;
    pMsg->msgType = PHY_TXSDU_CONF;
    pMsg->status = (U8) status;
    pMsg->subFrameNum = pBsTx->subframeNumber;
    pMsg->frameNumber = pPhy->txFrameNumber;
    pMsg->chanId = chanId;
    // Need to write outMsgSize
    *outMsgSize = sizeof(TXSDUIND);

    // Now message is ready to be send
}



//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Generates RxSdu indication messages according to
 *         the L2-PHY API defined in MSA-E81613
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Physical Layer Descriptor
 *
 *  @param outMsg Pointer to the Storage memory for the output message
 *
 *  @param outMsgSize Pointer to the storage for Message size
 *
 *  @param status Status to be reported back
 *
 *  @param chanId Channel identifier for the TxSdu confirmation message
 *
 *  @param i Allows this function to be called for each received codeword
 *         in support of spatial multiplexing
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |              |        |                   |            |               |         |
 *  -------------------------------------------------------------------------------------------------------
 *  |phyInstance |         |Input         |1       |unsigned int       |(1:31:0)    |[0,2]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |pPhy        |         |pInput        |1       |pStruct(LTEPHYDESC)|(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |outMsg      |         |pInput/pOutput|1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |outMsgSize  |         |pInput/pOutput|1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |chanId      |         |Input         |1       |unsigned int       |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |status      |         |Output        |1       |unsigned int       |(1:31:0)    | [0,1]        |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |i           |         |Input         |1       |unsigned int       |(1:31:0)    | [0,1]        |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteGenerateRxPrachStatusIndMsg     |
 *  ---------------------------------------------------
 *  |Input Streams       |pPhy,i,chanId               |
 *  ---------------------------------------------------
 *  |Output Streams      |outMsg,outMsgSize           |
 *  ---------------------------------------------------
 *  |Config Parameters   |phyInstance                 |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function generates an LteL2PhyAPI RxPrach indication message
 *
 *  The message complies with the format specified in document MSA-E81613
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * MSA-81613
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void LteGenerateRxPrachStatusIndMsg(PLTEPHYDESC pPhy, U32* outMsg, U32* outMsgSize, MS_DET* pDetEvent, U8 sf, U16 sfn)
{
    PRXSTATUSIND pMsg;
    PRX_PRACH_STATUS_EVENT pPrachEvent;
    U32 i, actualNum = 0;
    U32 *pOutMsg;

#ifndef _WIN32
    U32 tt = GET_TICKS;
#endif

    // First build the common part of the reply message and setup PhyInstance
    if (pPhy)
    {

#ifdef _WIN32
        pOutMsg = outMsg;
#else
        i = sizeof(RXSTATUSIND) + (pDetEvent->num * sizeof(RX_PRACH_STATUS_EVENT)) + 32;
        pOutMsg = (U32 *)Phy2MacIfApiBuffer(i);
#endif

        pMsg = (PRXSTATUSIND) pOutMsg;
        pMsg->phyEntityId = (U8) 0;
        pMsg->msgType = PHY_RXSTATUS_IND;
        pMsg->subFrameNum = sf;
        pMsg->frameNum = sfn;
        pMsg->statusType = PRACH_RESULT;
        pMsg->listType  = UL_SINGLE_ENTITY;

        pPrachEvent = (PRX_PRACH_STATUS_EVENT) &pMsg->pEventList;

        for (i = 0; i < (U32) (pDetEvent->num); i++)
        {

#ifndef _WIN32
            if (LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_RACH_STATS))
                LteBsPhyStatsRachStats(pDetEvent->tDelMicrosecEst[i], pDetEvent->peakToAvgPowerRatio[i]);
#endif

#if PRACH_THRESHOLD_REPORTING
            if (pDetEvent->peakToAvgPowerRatio[i] <= PRACH_DETECTION_THRESHOLD_GOOD)
                continue;
#endif

#if PRACH_WORKAROUND
            pPrachEvent->timingOffset = 9;
#else
            pPrachEvent->timingOffset = pDetEvent->tDelMicrosecEst[i];
#endif
            pPrachEvent->prachPreambleId = pDetEvent->cyclicShift[i];

            actualNum++;
            pPrachEvent++;
        }

        pMsg->eventNumber = actualNum;

        // Need to write outMsgSize
        *outMsgSize = sizeof(RXSTATUSIND) + sizeof(RX_PRACH_STATUS_EVENT) * pMsg->eventNumber;
        *outMsgSize = (*outMsgSize + 3) & ~3;

        // Now message is ready to be send
#ifndef _WIN32
        if (*outMsgSize)
        {
            if (SvsrLoggerGetMask() & (1<<PHY_RXSTATUS_IND))
            {
#if 1
                SvsrLoggerAddMsg(PHY_RXSTATUS_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC),
                            pOutMsg, *outMsgSize);
#else
                LtePhyMessagePush(pOutMsg, FIFO_PHY_LOG_AND_CLEAN_API,
                     PHY_API_CLEANUP__PHY_TO_MAC_RACH_MSG, *outMsgSize);

                LtePhyScheduleApiLogThread();
#endif
            }
            MLogTask(PID_MSGMGR_PRACH_BUILD_MSG, RESOURCE_LARM, tt, GET_TICKS);

            tt = GET_TICKS;
            LteSendMsgToMac(pOutMsg, *outMsgSize);
            MLogTask(PID_MSGMGR_PRACH_SEND_MSG, RESOURCE_LARM, tt, GET_TICKS);
        }
#else
        SvsrLoggerAddMsg(PHY_RXSTATUS_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), pOutMsg,
                         *outMsgSize);
#endif
    }
}

//-------------------------------------------------------------------------------------------
/** @brief Base Station Receiver RIP(Receive Interference Power) Status Report
 *  Message generation
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   RipPerRB_dB  - Pointer to RIP measured per RB
 *  @param   numRbs       - Number of Resource blocks in this RIP report
 *  @param   outMsg       - Pointer to the memory associated with the output message
 *  @param   outMsgSize   - Pointer to the memory that contains the size of the message
 *                          being generated
 *  @param   sf           - Subframe Number
 *  @param   sfn          - System Frame Number
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------------------------------
 *  |Name                       |Name in |Direction   |Length                  |Type          |Data Format |Valid Range      |Units |
 *  |                           |Standard|            |                        |              |            |                 |      |
 *  ---------------------------------------------------------------------------------------------------------------------------------
 *  |RipPerRB_dB                |        |input       |numRbs                  |Real          |(16:0:0)    |[0,2^16]         |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |numRbs                     |        |input       |1                       |Real          |(32:0:0)    |[0:100           |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |outMsg                     |        |pOutput     |outMsgSize              |pU32          |(1:31:0)    |[0:2^31-1]       |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |outMsgSize                 |        |input       |    1                   |pU32          |(1:31:0)    |[0:2^31-1        |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |sf                         |        |input       |    1                   |Int           |(1:8:0)     |[0:9]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |sfn                        |        |input       |    1                   |Int           |(1:15:0)    |[0:1023]         |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |LteBsRxGenerateRipIndMsg          |
 *  ---------------------------------------------------------
 *  |Input Stream        |RipPerRB_dB                       |
 *  --------------------------------------------------------
 *  |Output Stream       |outMsg                            |
 *  ---------------------------------------------------------
 *  |Config Parameters   |numRbs, sf, sfn, *outMsgSize      |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                              |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 *  Base Station Receiver RIP(Receive Interference Power) Status Report 
 *  generation to MAC.  Will construct RxStatus message with RIP per RB
 *  for current UE.
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void LteBsRxGenerateRipIndMsg(U16 *RipPerRB_dB, U32 numRbs, U8 sf, U16 sfn, U32 *outMsg, U32 *outMsgSize)
{
    PRXSTATUSIND pMsg;
    PRX_RIP_MEAS_STATUS_EVENT pRipMeasStatusEvent;
    PRX_RIP_MEAS_PER_BLOCK pRipMeasPerBlock;
    U32 *pOutMsg, i;

    pOutMsg                 = outMsg;

    pMsg                    = (PRXSTATUSIND) pOutMsg;
    pMsg->phyEntityId       = (U8) 0;
    pMsg->msgType           = PHY_RXSTATUS_IND;
    pMsg->subFrameNum       = sf;
    pMsg->frameNum          = sfn;
    pMsg->statusType        = RECEIVE_INTERFERENCE_POWER_RESULT;
    pMsg->eventNumber       = 1;
    pMsg->listType          = UL_SINGLE_ENTITY;

    pRipMeasStatusEvent = (PRX_RIP_MEAS_STATUS_EVENT) &pMsg->pEventList;
    pRipMeasStatusEvent->numResBlocks = numRbs;
    pRipMeasPerBlock = (PRX_RIP_MEAS_PER_BLOCK)(pRipMeasStatusEvent+1);

    for (i = 0; i < numRbs; i++)
    {
        pRipMeasPerBlock->rxRIP_dB0 = RipPerRB_dB[i];
        i++;

        pRipMeasPerBlock->rxRIP_dB1 = RipPerRB_dB[i];

        pRipMeasPerBlock++;
    }

    // Need to write outMsgSize
    *outMsgSize = sizeof(RXSTATUSIND) + sizeof(RX_RIP_MEAS_STATUS_EVENT) + (((numRbs+1)>>1)*sizeof(RX_RIP_MEAS_PER_BLOCK));
    *outMsgSize = (*outMsgSize + 3) & ~3;       // Round

    // Now message is ready to be send
#ifndef _WIN32
    if (*outMsgSize)
    {
        if (SvsrLoggerGetMask() & (1<<PHY_RXSTATUS_IND))
        {
            SvsrLoggerAddMsg(PHY_RXSTATUS_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC),
                        pOutMsg, *outMsgSize);
        }

        LteSendMsgToMac(pOutMsg, *outMsgSize);
    }
#else
    SvsrLoggerAddMsg(PHY_RXSTATUS_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), pOutMsg,
                     *outMsgSize);
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief Base Station Thermal noise Status Report Message generation
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   ThermalNoisePwr - Total Thermal noise measured by the PHY for specific UE 
 *                             resource block allocation
 *  @param   numRbs          - Number of Resource blocks used to compute thermal noise
 *  @param   outMsg          - Pointer to the memory associated with the output message
 *  @param   outMsgSize      - Pointer to the memory that contains the size of the message
 *                             being generated
 *  @param   sf              - Subframe Number
 *  @param   sfn             - System Frame Number
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------------------------------
 *  |Name                       |Name in |Direction   |Length                  |Type          |Data Format |Valid Range      |Units |
 *  |                           |Standard|            |                        |              |            |                 |      |
 *  ---------------------------------------------------------------------------------------------------------------------------------
 *  |ThermalNoisePwr            |        |input       |1                       |Real          |(1:15:0)    |[-2^15,2^15-1]   |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |numRbs                     |        |input       |1                       |Real          |(0:32:0)    |[0:100           |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |outMsg                     |        |pOutput     |outMsgSize              |pU32          |(1:31:0)    |[0:2^31-1]       |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |outMsgSize                 |        |input       |    1                   |pU32          |(1:31:0)    |[0:2^31-1        |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |sf                         |        |input       |    1                   |Int           |(1:8:0)     |[0:9]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |sfn                        |        |input       |    1                   |Int           |(1:15:0)    |[0:1023]         |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |LteBsRxGenerateThermalNoiseIndMsg |
 *  ---------------------------------------------------------
 *  |Input Stream        |ThermalNoisePwr                   |
 *  --------------------------------------------------------
 *  |Output Stream       |outMsg                            |
 *  ---------------------------------------------------------
 *  |Config Parameters   |numRbs, sf, sfn, *outMsgSize      |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                              |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 *  Base Station Receiver Thermal noise Status Report 
 *  generation to MAC.  Will construct RxStatus message with Thermal Noise measured 
 *  by the PHY for current UE.
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void LteBsRxGenerateThermalNoiseIndMsg(S16 ThermalNoisePwr, U16 numRbs, U8 sf, U16 sfn, U32 *outMsg, U32 *outMsgSize)
{
    PRXSTATUSIND pMsg;
    PRX_THERMAL_NOISE_MEAS_STATUS_EVENT pThermalNoiseMeasStatEvent;
    U32 *pOutMsg;

    pOutMsg                 = outMsg;

    pMsg                    = (PRXSTATUSIND) pOutMsg;
    pMsg->phyEntityId       = (U8) 0;
    pMsg->msgType           = PHY_RXSTATUS_IND;
    pMsg->subFrameNum       = sf;
    pMsg->frameNum          = sfn;
    pMsg->statusType        = THREMAL_NOISE_RESULT;
    pMsg->eventNumber       = 1;
    pMsg->listType          = UL_SINGLE_ENTITY;


    pThermalNoiseMeasStatEvent = (PRX_THERMAL_NOISE_MEAS_STATUS_EVENT) &pMsg->pEventList;
    pThermalNoiseMeasStatEvent->ThermalNoisePwr = ThermalNoisePwr;
    pThermalNoiseMeasStatEvent->TotalRb         = numRbs;

    // Need to write outMsgSize
    *outMsgSize = sizeof(RXSTATUSIND) + sizeof(RX_THERMAL_NOISE_MEAS_STATUS_EVENT);
    *outMsgSize = (*outMsgSize + 3) & ~3;       // Round

    // Now message is ready to be send
#ifndef _WIN32
    if (*outMsgSize)
    {
        if (SvsrLoggerGetMask() & (1<<PHY_RXSTATUS_IND))
        {
            SvsrLoggerAddMsg(PHY_RXSTATUS_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC),
                        pOutMsg, *outMsgSize);
        }

        LteSendMsgToMac(pOutMsg, *outMsgSize);
    }
#else
    SvsrLoggerAddMsg(PHY_RXSTATUS_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), pOutMsg,
                     *outMsgSize);
#endif

}

//-------------------------------------------------------------------------------------------
/** @brief Base Station Receiver Sounding Reference Signal Physical Receive Status Report
 *  Message generation
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param   pPhy                          Pointer to LTE PHY descriptor structure
 *
 *  @param   phyInstance                   Identifies the phy for which the report is being
 *                                         prepared
 *
 *  @param   outMsg                        Pointer to the memory associated with the output message
 *
 *  @param   outMsgSize                    Pointer to the memory that contains the size of the message
 *                                         being generated
 *
 *  @param   sf                            Subframe Number
 *
 *  @param   sfn                           System Frame Number
 *
 *  @return  none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------------------------------
 *  |Name                       |Name in |Direction   |Length                  |Type          |Data Format |Valid Range      |Units |
 *  |                           |Standard|            |                        |              |            |                 |      |
 *  ---------------------------------------------------------------------------------------------------------------------------------
 *  |pPhy                       |        |input       |    1                   |pStruct       |(1:31:0)    |[0,2^31-1]       |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |phyInstance                |        |input       |    1                   |Real          |(1:15:0)    |[0:4]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |outMsg                     |        |input       |    1                   |pU32          |(1:31:0)    |[0:2^31-1]       |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |outMsgSize                 |        |input       |    1                   |pU32          |(1:31:0)    |[0:2^31-1        |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |sf                         |        |input       |    1                   |Int           |(1:8:0)     |[0:9]            |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  |sfn                        |        |input       |    1                   |Int           |(1:15:0)    |[0:1023]         |      |
 *  --------------------------------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------------
 *  |Block Name          |LteGenerateRxSrsStatusIndMsg                 |
 *  ---------------------------------------------------------
 *  |Input Stream        |pPhy,phyInstance,outMsg,outMsgSize|
 *  |                    |sf,sfn                            |
 *  --------------------------------------------------------
 *  |Output Stream       |                                  |
 *  ---------------------------------------------------------
 *  |Config Parameters   |                                  |
 *  ---------------------------------------------------------
 *  |Status Parameters   |none                              |
 *  ---------------------------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *
 *  Base Station Receiver Sounding Reference Signal Physical Receive Status Report
 *  Message generator, assembles the data from the different SRS's signals and
 *  generates an snr per RB and a timing offset estimate on a per SRS signal
 *
 *
 *
 *  <5> Cycle Count Formula:
 *  TBD
 *
 *
 *  <6> References:
 *   [1] 3GPP TS 36.211 Rel.8, Sec.5.5.3, "Sounding Reference Signal"<BR>
 *
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void LteGenerateRxSrsStatusIndMsg(PLTEPHYDESC pPhy, U32* outMsg, U32* outMsgSize, U8 sf, U16 sfn)
{
    U32 i,j;
    U8  *ptr, rbEnd;
    S16 Mrb, numRb;
    PRXSTATUSIND pMsg;
    U32 *pOutMsg;

    PRX_SRS_STATUS_EVENT pSrsEvent;

    PBS_RX_CHAN_SRS pSrsCh;
    PBS_RX_POINTERS pRxPointers;
    PBSRXCHMeas pBsRxChMeas;
    U32 numSrs;

#ifndef _WIN32
    U32 tt = GET_TICKS;
#endif

    if (pPhy)
    {
        // Determine how many srs's signals were received based on the number received in the
        // RXVECTOR and then fill the template for the PHYRXSTATUS message
        pSrsCh = &pPhy->pBsRx->pBsRxChanSrs[0];
        pRxPointers = &pPhy->pBsRx->pBsRxPointers[0];
        pBsRxChMeas = &pRxPointers->pSrsChMeas[0];
        numSrs = pPhy->pBsRx->numSrsChan;
        Mrb = pPhy->pBsRx->Nul_rb;

#ifdef _WIN32
        pOutMsg = outMsg;
#else
        numRb = 0;
        for (i=0; i < numSrs; i++)
        {
            rbEnd = (U8)pSrsCh->EndRb[i];
            numRb += (rbEnd - pSrsCh->StartRb[i]);
        }

        i = sizeof(RXSTATUSIND) + numSrs * (sizeof(RX_SRS_STATUS_EVENT)+numRb) + 32;
        pOutMsg = (U32 *)Phy2MacIfApiBuffer(i);
#endif

        // First build the common part of the reply message and setup PhyInstance
        pMsg = (PRXSTATUSIND) pOutMsg;
        pMsg->phyEntityId = (U8) 0;
        pMsg->msgType = PHY_RXSTATUS_IND;
        pMsg->subFrameNum = sf;
        pMsg->frameNum = sfn;
        pMsg->statusType = SRS_RESULT;
        pMsg->eventNumber = numSrs;
        pMsg->listType  = UL_SINGLE_ENTITY;

        pSrsEvent = (PRX_SRS_STATUS_EVENT) &pMsg->pEventList;
        ptr = (U8*)&pSrsEvent->psnr;

        numRb = 0;
        for (i=0; i < numSrs; i++)
        {
            pSrsEvent->srsIndex = (U16)i;
            pSrsEvent->reserved = 0;
            pSrsEvent->timingAdvance = pBsRxChMeas[i].TimingAdvance;
            pSrsEvent->rbStart = (U8)pSrsCh->StartRb[i];
            rbEnd = (U8)pSrsCh->EndRb[i];
            pSrsEvent->numberRb = rbEnd - pSrsEvent->rbStart;
            numRb += pSrsEvent->numberRb;
            //pSrsEvent->psnr = ptr;
            for (j= pSrsEvent->rbStart; j< rbEnd; j++)
            {
                *ptr++ = pRxPointers->pSrsAvgSnrPerRbdB[i*Mrb+j];
            }

            pSrsEvent = (PRX_SRS_STATUS_EVENT)ptr; // Always in multiples of 4 RB's
            ptr = (U8*)&pSrsEvent->psnr;
           // ptr = ptr + sizeof(RX_SRS_STATUS_EVENT) - sizeof(U8*);
        }

        pMsg->msgSpecific = numRb;
        *outMsgSize = sizeof(RXSTATUSIND) -sizeof(U32) + pMsg->eventNumber * (sizeof(RX_SRS_STATUS_EVENT)-sizeof(U8*)) + pMsg->msgSpecific;
        *outMsgSize = (*outMsgSize + 3) & ~3;

#ifndef _WIN32
        if (*outMsgSize)
        {
            if (SvsrLoggerGetMask() & (1<<PHY_RXSTATUS_IND))
            {
                tt = GET_TICKS;
#if 1
                SvsrLoggerAddMsg(PHY_RXSTATUS_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC),
                            pOutMsg, *outMsgSize);

#else
                LtePhyMessagePush(pOutMsg, FIFO_PHY_LOG_AND_CLEAN_API,
                     PHY_API_CLEANUP__PHY_TO_MAC_SRS_MSG, *outMsgSize);

                LtePhyScheduleApiLogThread();
#endif
            }
            MLogTask(PID_MSGMGR_SRS_BUILD_MSG, RESOURCE_LARM, tt, GET_TICKS);

            tt = GET_TICKS;
            LteSendMsgToMac(pOutMsg, *outMsgSize);
            MLogTask(PID_MSGMGR_SRS_SEND_MSG, RESOURCE_LARM, tt, GET_TICKS);
        }
#else
        SvsrLoggerAddMsg(PHY_RXSTATUS_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), pOutMsg,
                         *outMsgSize);

#endif

    }
}



#ifdef _WIN32
//-------------------------------------------------------------------------------------------
/** @brief Generates RxSdu indication messages according to
 *         the L2-PHY API defined in MSA-E81613
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Physical Layer Descriptor
 *
 *  @param outMsg Pointer to the Storage memory for the output message
 *
 *  @param outMsgSize Pointer to the storage for Message size
 *
 *  @param status Status to be reported back
 *
 *  @param chanId Channel identifier for the TxSdu confirmation message
 *
 *  @param i Allows this function to be called for each received codeword
 *         in support of spatial multiplexing
 *
 *  @param pErrAvg Pointer to timing advance error info
 *
 *  @param errExpo Timing Advance error exponent
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |              |        |                   |            |               |         |
 *  -------------------------------------------------------------------------------------------------------
 *  |phyInstance |         |Input         |1       |unsigned int       |(1:31:0)    |[0,2]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |outMsg      |         |pInput/pOutput|1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |outMsgSize  |         |pInput/pOutput|1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |chanId      |         |Input         |1       |unsigned int       |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |status      |         |Output        |1       |unsigned int       |(1:31:0)    | [0,1]        |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |pUlRxInputParams |         |Input         |1       |PULRXINPUTPARAMS       |(struct)    |        |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteGenerateRxSduIndMsg      |
 *  ---------------------------------------------------
 *  |Input Streams       |i,chanId, pErrAvg ,errExp       |
 *  ---------------------------------------------------
 *  |Output Streams      |outMsg,outMsgSize           |
 *  ---------------------------------------------------
 *  |Config Parameters   |phyInstance                 |
 *  ---------------------------------------------------
 *  |Status Parameters   |status                      |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function generates an LteL2PhyAPI RxSdu indication message with the
 *  status contained in its fourth parameter
 *  The message complies with the format specified in document MSA-E81613
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * MSA-81613
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void LteGenerateRxSduIndMsg(void *Context, U32* outMsg, U32* outMsgSize, U32 status, U32 chanId,
                                      PULRXINPUTPARAMS pUlRxInputParams, PULRXFECPARAMS pRxFecParams,
                                      U32 ctxState)
{
    U32 sendMsg = 0;
    PRXSDUIND pMsg;

    PLTEPHYDESC pPhy = NULL;

    PBS_RX pBsRx;

    U32* pDst;
    U32 NumBytes;
    U8 *pSrc;

    // First build the common part of the reply message and setup PhyInstance
    pMsg = (PRXSDUIND) outMsg;
    pMsg->phyEntityId = 0;//(U8) phyInstance;
    pMsg->msgType = PHY_RXSDU_IND;
    pMsg->status = (U8) status;
    pMsg->channelId = chanId;

    if (ctxState == CONTEXT_P_PHY)
    {
        pPhy = (PLTEPHYDESC)Context;

        if (pPhy)
        {
            sendMsg = 1;
            pBsRx = pPhy->pBsRx;

            pMsg->frameNum    = pPhy->rxFrameNumber;
            pMsg->subFrameNum = pBsRx->subframeNumber;
            pMsg->chanType    = pUlRxInputParams->physChanType[chanId];
            pMsg->numBitsRx   = pRxFecParams->numBitsRx[chanId];

            pMsg->srdetected   = pUlRxInputParams->srdetected[chanId];
            pMsg->timingAdv    = pUlRxInputParams->BsRxChMeas[chanId].TimingAdvance;
            pMsg->mErrAvgCh[0] = pUlRxInputParams->mErrAvg[2*chanId];
            pMsg->mErrAvgCh[1] = pUlRxInputParams->mErrAvg[2*chanId + 1];
            pMsg->mErrExpo = pUlRxInputParams->mErrExpo[chanId];
            pMsg->ul_CQI = pUlRxInputParams->ul_CQI[chanId];
            pMsg->cqiPmiConf = ((pUlRxInputParams->rmconf[chanId]>>14) & 0x0003);
            pMsg->rssiMeasurement = pUlRxInputParams->Rssi_dB[chanId];

            if (pRxFecParams->channelType[chanId] == PUCCH)
            {
                pMsg->timingAdv     = 31;
                pMsg->pucchDetected = 1;
                pMsg->pucchType = pRxFecParams->pucchFormat[chanId];
                if (pMsg->numBitsRx == 0)
                    pMsg->pucchDetected = 0;
            }
        }
    }

    if (sendMsg)
    {
        //For PUSCH
        NumBytes = (pMsg->numBitsRx + 7) >> 3;

        // Need to write outMsgSize
        *outMsgSize = NumBytes;

        pDst = &pMsg->pRxSdu;
        *outMsgSize += sizeof(RXSDUIND);

        // Need to adjust byte count to 4 byte multiple
        *outMsgSize = (*outMsgSize + 3) & ~3;

        pSrc = pRxFecParams->mac_sdu[chanId];

        //For PUCCH, it is always 2 bytes.  Update NumBytes here
        //so we don't disturb *outMsgSize.  Host application reads
        //size of message from numBitsRx which now has PUCCH format
        //type appended to upper 8 bits.
        if (pUlRxInputParams->physChanType[chanId] == PUCCH)
        {
           NumBytes = 2;
           pSrc = pRxFecParams->pucch_mac_sdu[chanId];
        }

        memcpy(pDst, pSrc, NumBytes);
        SvsrLoggerAddMsg(PHY_RXSDU_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), outMsg,
                         *outMsgSize);
    }
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Generates RxSdu indication messages according to
 *         the L2-PHY API defined in MSA-E81613
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Physical Layer Descriptor
 *
 *  @param outMsg Pointer to the Storage memory for the output message
 *
 *  @param outMsgSize Pointer to the storage for Message size
 *
 *  @param chanId Channel identifier for the RxStatus indication message
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |              |        |                   |            |               |         |
 *  -------------------------------------------------------------------------------------------------------
 *  |pPhy        |         |pInput        |1       |pStruct(LTEPHYDESC)|(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |phyInstance |         |Input         |1       |unsigned int       |(1:31:0)    |[0,2]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |outMsg      |         |pInput/pOutput|1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |outMsgSize  |         |pInput/pOutput|1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |pChan       |        |pInput        |1       |punsigned int     |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |chanId      |         |Input         |1       |unsigned int       |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteGenerateRxStatusCqiRiHi      |
 *  ---------------------------------------------------
 *  |Input Streams       |pPhy,pCh,chanId               |
 *  ---------------------------------------------------
 *  |Output Streams      |outMsg,outMsgSize           |
 *  ---------------------------------------------------
 *  |Config Parameters   |phyInstance                 |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function generates an LteL2PhyAPI RxSdu indication message
 *  The message complies with the format specified in document MSA-E81613
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * MSA-81613
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void LteGenerateRxStatusCqiRiHi(void *Context, U32 phyInstance, U32* outMsg,
                                          U32* outMsgSize, PULRXFECPARAMS pCh, U32 chanId, U8 sf, U32 ctxState)
{
    U32 sendMsg = 0;
    PRXSTATUSIND pMsg;
    PRX_CQIRIHI_STATUS_EVENT pMsg1;

    PLTEPHYDESC pPhy = NULL;

    U32 tt;
    U8 *ptr;
    U8 *cqiOut, *riOut, *ackOut;
    U16 numBytesCQI, numBytesRI, numBytesACK;

    // First build the common part of the reply message and setup PhyInstance
    tt = GET_TICKS;
    pMsg = (PRXSTATUSIND) outMsg;
    pMsg->phyEntityId = (U8) 0;
    pMsg->msgType = PHY_RXSTATUS_IND;
    pMsg->subFrameNum = sf;
    pMsg->statusType = CQIRIHI_RESULT;
    pMsg->eventNumber = 1;
    pMsg1= (PRX_CQIRIHI_STATUS_EVENT)&pMsg->pEventList;

    // Now fill the status info based on the channel info
    pMsg1->status_len = 0;
    ptr = (U8*)pMsg1;
    ptr = ptr + sizeof(RX_CQIRIHI_STATUS_EVENT);
    pMsg1->chanId = chanId;
    pMsg1->cqi_pmi_pt = 0;
    pMsg1->ri_pt = 0;
    pMsg1->harq_pt = 0;
    pMsg1->cqiPmiConf = ((pCh->rmconf[chanId]>>14) & 0x0003);

    if (ctxState == CONTEXT_P_PHY)
    {
       pPhy = (PLTEPHYDESC)Context;
       if (pPhy)
       {
           sendMsg = 1;

           pMsg->frameNum = pPhy->rxFrameNumber;

           numBytesCQI = (U16)((pCh->nr1CQI[chanId] + pCh->nrg1CQI[chanId] + 7)/8);
           numBytesRI  = (U16)((pCh->nRI[chanId]  + 7)/8);
           numBytesACK = (U16)((pCh->nACK[chanId] + 7)/8);

           pMsg1->cqi_pmi_pt = pCh->nr1CQI[chanId] + pCh->nrg1CQI[chanId];
           pMsg1->ri_pt = pCh->nRI[chanId];
           pMsg1->harq_pt = pCh->nACK[chanId];

           cqiOut = &pCh->cqiStore[chanId][0];
           riOut  = &pCh->riStore[chanId];
           ackOut = &pCh->ackStore[chanId];
       }
    }

    if (sendMsg)
    {
        if (numBytesCQI)
        {
            memcpy(ptr, cqiOut, numBytesCQI);
            pMsg1->status_len = numBytesCQI;
        }
        else
        {
            pMsg1->cqi_pmi_pt = 0;
        }

        if (numBytesRI)
        {
            // Output byte. Bit7 is 1st decoded bit and bit6 is 2nd decoded bit.
            // Bit5 is used to indicate valid reception (1=yes, 0=no).
            if (*riOut & 0x20)  // If Valid Reception
            {
                *(ptr+pMsg1->status_len)= *riOut & 0xC0;
                pMsg1->status_len++;
            }
            else
            {
                pMsg1->ri_pt = 0;
            }
        }
        else
        {
            pMsg1->ri_pt = 0;
        }

        if (numBytesACK)
        {
            // Output byte. Bit7 is 1st decoded bit and bit6 is 2nd decoded bit.
            // Bit5 is used to indicate valid reception (1=yes, 0=no).
            if (*ackOut & 0x20)  // If Valid Reception
            {
                *(ptr+pMsg1->status_len)= *ackOut & 0xC0;
                pMsg1->status_len++;
            }
            else
            {
                pMsg1->harq_pt = 0;
            }
        }
        else
        {
            pMsg1->harq_pt = 0;
        }
    }

    *outMsgSize = sizeof(RXSTATUSIND) + (pMsg->eventNumber * sizeof(RX_CQIRIHI_STATUS_EVENT)) + pMsg1->status_len;
    *outMsgSize = (*outMsgSize + 3) & ~3;

    SvsrLoggerAddMsg(PHY_RXSTATUS_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), outMsg,
                     *outMsgSize);
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Generates RxSdu indication messages according to
 *         the L2-PHY API defined in MSA-E81613
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Physical Layer Descriptor
 *
 *  @param outMsg Pointer to the Storage memory for the output message
 *
 *  @param outMsgSize Pointer to the storage for Message size
 *
 *  @param chanId Channel identifier for the RxStatus indication message
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |              |        |                   |            |               |         |
 *  -------------------------------------------------------------------------------------------------------
 *  |pPhy        |         |pInput        |1       |pStruct(LTEPHYDESC)|(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |phyInstance |         |Input         |1       |unsigned int       |(1:31:0)    |[0,2]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |outMsg      |         |pInput/pOutput|1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |outMsgSize  |         |pInput/pOutput|1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |pChan       |        |pInput        |1       |punsigned int     |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |chanId      |         |Input         |1       |unsigned int       |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteGenerateRxStatusHi      |
 *  ---------------------------------------------------
 *  |Input Streams       |pPhy,pCh,chanId               |
 *  ---------------------------------------------------
 *  |Output Streams      |outMsg,outMsgSize           |
 *  ---------------------------------------------------
 *  |Config Parameters   |phyInstance                 |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function generates an LteL2PhyAPI RxSdu indication message
 *  The message complies with the format specified in document MSA-E81613
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * MSA-81613
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void LteGenerateRxStatusHi(void *Context, U32 phyInstance, U32* outMsg,
                           U32* outMsgSize, PULRXFECPARAMS pCh, U32 chanId, U8 sf, U32 ctxState)
{
    U32 sendMsg = 0;
    PRXSTATUSIND pMsg;
    PRX_CQIRIHI_STATUS_EVENT pMsg1;

    PLTEPHYDESC pPhy = NULL;

    U32 tt;
    U8 *ptr;
    U8 *ackOut;
    U16 numBytesACK;

    // First build the common part of the reply message and setup PhyInstance
    tt = GET_TICKS;
    pMsg = (PRXSTATUSIND) outMsg;
    pMsg->phyEntityId = (U8) 0;
    pMsg->msgType = PHY_RXSTATUS_IND;
    pMsg->subFrameNum = sf;
    pMsg->statusType = CQIRIHI_RESULT;
    pMsg->eventNumber = 1;
    pMsg1= (PRX_CQIRIHI_STATUS_EVENT)&pMsg->pEventList;

    // Now fill the status info based on the channel info
    pMsg1->status_len = 0;
    ptr = (U8*)pMsg1;
    ptr = ptr + sizeof(RX_CQIRIHI_STATUS_EVENT);
    pMsg1->chanId = chanId;
    pMsg1->cqi_pmi_pt = 0;
    pMsg1->ri_pt = 0;
    pMsg1->harq_pt = 0;
    pMsg1->cqiPmiConf = ((pCh->rmconf[chanId]>>14) & 0x0003);

    if (ctxState == CONTEXT_P_PHY)
    {
       pPhy = (PLTEPHYDESC)Context;
       if (pPhy)
       {
           sendMsg = 1;

           pMsg->frameNum = pPhy->rxFrameNumber;

           numBytesACK    = (U16)((pCh->nACK[chanId] + 7)/8);
           pMsg1->harq_pt = pCh->nACK[chanId];
           ackOut         = &pCh->ackStore[chanId];
       }
    }

    if (sendMsg)
    {
        if (numBytesACK)
        {
            // Output byte. Bit7 is 1st decoded bit and bit6 is 2nd decoded bit.
            // Bit5 is used to indicate valid reception (1=yes, 0=no).
            if (*ackOut & 0x20)  // If Valid Reception
            {
                *(ptr+pMsg1->status_len)= *ackOut & 0xC0;
                pMsg1->status_len++;
            }
            else
            {
                pMsg1->harq_pt = 0;
            }
        }
        else
        {
            pMsg1->harq_pt = 0;
        }
    }

    *outMsgSize = sizeof(RXSTATUSIND) + (pMsg->eventNumber * sizeof(RX_CQIRIHI_STATUS_EVENT)) + pMsg1->status_len;
    *outMsgSize = (*outMsgSize + 3) & ~3;

    SvsrLoggerAddMsg(PHY_RXSTATUS_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), outMsg,
                     *outMsgSize);
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL Generates RxSdu indication messages according to
 *         the L2-PHY API defined in MSA-E81613
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Physical Layer Descriptor
 *
 *  @param outMsg Pointer to the Storage memory for the output message
 *
 *  @param outMsgSize Pointer to the storage for Message size
 *
 *  @param chanId Channel identifier for the RxStatus indication message
 *
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |              |        |                   |            |               |         |
 *  -------------------------------------------------------------------------------------------------------
 *  |pPhy        |         |pInput        |1       |pStruct(LTEPHYDESC)|(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |phyInstance |         |Input         |1       |unsigned int       |(1:31:0)    |[0,2]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |outMsg      |         |pInput/pOutput|1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |outMsgSize  |         |pInput/pOutput|1       |punsigned int      |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |pChan       |        |pInput        |1       |punsigned int     |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |chanId      |         |Input         |1       |unsigned int       |(1:31:0)    |(0,2^32)      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteGenerateRxStatusCqiRi      |
 *  ---------------------------------------------------
 *  |Input Streams       |pPhy,pCh,chanId               |
 *  ---------------------------------------------------
 *  |Output Streams      |outMsg,outMsgSize           |
 *  ---------------------------------------------------
 *  |Config Parameters   |phyInstance                 |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function generates an LteL2PhyAPI RxSdu indication message
 *  The message complies with the format specified in document MSA-E81613
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * MSA-81613
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void LteGenerateRxStatusCqiRi(void *Context, U32 phyInstance, U32* outMsg,
                              U32* outMsgSize, PULRXFECPARAMS pCh, U32 chanId, U8 sf, U32 ctxState)
{
    U32 sendMsg = 0;
    PRXSTATUSIND pMsg;
    PRX_CQIRIHI_STATUS_EVENT pMsg1;

    PLTEPHYDESC pPhy = NULL;

    U32 tt;
    U8 *ptr;
    U8 *cqiOut, *riOut;
    U16 numBytesCQI, numBytesRI;

    // First build the common part of the reply message and setup PhyInstance
    tt = GET_TICKS;
    pMsg = (PRXSTATUSIND) outMsg;
    pMsg->phyEntityId = (U8) 0;
    pMsg->msgType = PHY_RXSTATUS_IND;
    pMsg->subFrameNum = sf;
    pMsg->statusType = CQIRIHI_RESULT;
    pMsg->eventNumber = 1;
    pMsg1= (PRX_CQIRIHI_STATUS_EVENT)&pMsg->pEventList;

    // Now fill the status info based on the channel info
    pMsg1->status_len = 0;
    ptr = (U8*)pMsg1;
    ptr = ptr + sizeof(RX_CQIRIHI_STATUS_EVENT);
    pMsg1->chanId = chanId;
    pMsg1->cqi_pmi_pt = 0;
    pMsg1->ri_pt = 0;
    pMsg1->harq_pt = 0;
    pMsg1->cqiPmiConf = ((pCh->rmconf[chanId]>>14) & 0x0003);

    if (ctxState == CONTEXT_P_PHY)
    {
       pPhy = (PLTEPHYDESC)Context;
       if (pPhy)
       {
           sendMsg = 1;

           pMsg->frameNum = pPhy->rxFrameNumber;

           numBytesCQI = (U16)((pCh->nr1CQI[chanId] + pCh->nrg1CQI[chanId] + 7)/8);
           numBytesRI  = (U16)((pCh->nRI[chanId]  + 7)/8);

           pMsg1->cqi_pmi_pt = pCh->nr1CQI[chanId] + pCh->nrg1CQI[chanId];
           pMsg1->ri_pt = pCh->nRI[chanId];

           cqiOut = &pCh->cqiStore[chanId][0];
           riOut  = &pCh->riStore[chanId];
       }
    }

    if (sendMsg)
    {
        if (numBytesCQI)
        {
            memcpy(ptr, cqiOut, numBytesCQI);
            pMsg1->status_len = numBytesCQI;
        }
        else
        {
            pMsg1->cqi_pmi_pt = 0;
        }

        if (numBytesRI)
        {
            // Output byte. Bit7 is 1st decoded bit and bit6 is 2nd decoded bit.
            // Bit5 is used to indicate valid reception (1=yes, 0=no).
            if (*riOut & 0x20)  // If Valid Reception
            {
                *(ptr+pMsg1->status_len)= *riOut & 0xC0;
                pMsg1->status_len++;
            }
            else
            {
                pMsg1->ri_pt = 0;
            }
        }
        else
        {
            pMsg1->ri_pt = 0;
        }
    }

    *outMsgSize = sizeof(RXSTATUSIND) + (pMsg->eventNumber * sizeof(RX_CQIRIHI_STATUS_EVENT)) + pMsg1->status_len;
    *outMsgSize = (*outMsgSize + 3) & ~3;

    SvsrLoggerAddMsg(PHY_RXSTATUS_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), outMsg,
                     *outMsgSize);
}

#else // ifdef _WIN32


// RT Project Specific Rx MAC Operations
//--------------------------------------

volatile PMAC2PHY_QUEUE_EL Head[3][NUM_UL_SDU_CONTEXT] = {NULL, NULL, NULL};
volatile PMAC2PHY_QUEUE_EL Tail[3][NUM_UL_SDU_CONTEXT] = {NULL, NULL, NULL};

#define GET_LIST_HEAD(listType, rxSduCtx) (&Head[listType][rxSduCtx])
#define GET_LIST_TAIL(listType, rxSduCtx) (&Tail[listType][rxSduCtx])

#define MAC_PROCESSING_THREAD_ARM_ID ((MxGetARMID () == 0) ? 3 : 1)

//-------------------------------------------------------------------------------------------
/** @brief Generates RxEnd message to append to list of messages signalling the end of the list
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param subframeNum - Current subframe being processed
 *
 *  @param frameNum    - Current frame being processed
 *
 *  @return    pointer to RxEnd message
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |              |        |                   |            |              |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |subframeNum |         |Input         |1       |unsigned int       |(1:31:0)    |[0:9]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |frameNum    |         |Input         |1       |unsigned int       |(1:31:0)    |[0:1023]      |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteRxSduBuildRxEndMessage   |
 *  ---------------------------------------------------
 *  |Input Streams       |                            |
 *  ---------------------------------------------------
 *  |Output Streams      |                            |
 *  ---------------------------------------------------
 *  |Config Parameters   |frameNum, subframeNum       |
 *  ---------------------------------------------------
 *  |Status Parameters   |                            |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function generates the RxEnd message to be appended at the end of
 *  the current list of messages.  It will indicate to the MAC that it has
 *  received all PUCCH, PUSCH and Mux ACK PUSCH messages from the PHY for
 *  current subframe.
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 * MSA-81613
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
U8* LteRxSduBuildRxEndMessage(U32 frameNum, U32 subframeNum)
{
    PMSGIND pMsg1;
    PMAC2PHY_QUEUE_EL RxEnd;
    UINT32 tt = GET_TICKS;

    // Build the RxEnd Message
    pMsg1 = (PMSGIND) Phy2MacIfAllocBuffer(sizeof(MSGIND));

    pMsg1->subFrameNum = subframeNum;
    pMsg1->status = (U8) 0;
    pMsg1->msgType = (U8) PHY_RXEND_IND;
    pMsg1->phyEntityId = 0;
    pMsg1->frameNumber = frameNum;


    // Build the List Element
    RxEnd = (PMAC2PHY_QUEUE_EL) Phy2MacIfAllocBuffer(sizeof(MAC2PHY_QUEUE_EL));
    if(RxEnd == NULL)
    {
        stop_printf("RXEND: Can't allocate the PMAC2PHY_QUEUE_EL FN#%d SFN#%d\n ", frameNum, subframeNum);
        return 0;
    }
    RxEnd->Next = NULL;
    RxEnd->frameNumber = frameNum;
    RxEnd->subframeNumber = subframeNum;
    RxEnd->MessageType = PHY_RXEND_IND;
    RxEnd->MessagePtr = (U8*)pMsg1;
    RxEnd->MessageLen = sizeof(MSGIND);

    SvsrLoggerAddMsg(PHY_RXEND_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), pMsg1, sizeof(MSGIND));

    MLogTask(PID_MSGMGR_RXEND_BUILD_MSG, RESOURCE_LARM, tt, GET_TICKS);

    return (U8*)RxEnd;
}

//-------------------------------------------------------------------------------------------
/** @brief Send List of Uplink messages to the MAC
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param listType - Type of list being send to MAC
 *
 *  @param rxSduCtx - Current RxSDU context
 *
 *  @return    none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |              |        |                   |            |              |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |listType    |         |Input         |1       |unsigned int       |(1:31:0)    |[0:2]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |rxSduCtx    |         |Input         |1       |unsigned int       |(1:31:0)    |[0:3]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |SendToMac                   |
 *  ---------------------------------------------------
 *  |Input Streams       |                            |
 *  ---------------------------------------------------
 *  |Output Streams      |                            |
 *  ---------------------------------------------------
 *  |Config Parameters   |listType, rxSduCtx          |
 *  ---------------------------------------------------
 *  |Status Parameters   |                            |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function will send the list of messages generated by the 
 *  Uplink Rx to the MAC
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void SendToMac(U32 listType, U32 rxSduCtx)
{
    volatile U32 tt = GET_TICKS;
    volatile PMAC2PHY_QUEUE_EL *localHead = (PMAC2PHY_QUEUE_EL *)GET_LIST_HEAD(listType, rxSduCtx);
    volatile PMAC2PHY_QUEUE_EL *localTail = (PMAC2PHY_QUEUE_EL *)GET_LIST_TAIL(listType, rxSduCtx);
    U32 mlogPid = 30*rxSduCtx;

    if ((*localHead))
    {
        if (GetMacInstanceId() == UARM_INSTID_DEFMAC)
        {
            // if remote peer is real MAC use real ICPU
            LteSendListToRmtMac((UINT32)*localHead);
            MLogTask(PID_APILIST_SEND_TO_LINUX_MAC, RESOURCE_LARM, tt, GET_TICKS);
        }
        else
        {
            SvsrSendMsg(MAC_PROCESSING_THREAD_ARM_ID, IID_LTE_MAC_EX, *localHead, SVSR_MSG_OPT_DEFAULT);
            MLogTask(PID_APILIST_SEND_TO_4GMX_MAC, RESOURCE_LARM, tt, GET_TICKS);
        }
    }

    (*localHead) = NULL;
    (*localTail) = NULL;

    if (listType == PUSCH_LIST_TYPE)
    {
        gRxSduListState[rxSduCtx] &= ~(RXSDU_LIST_STATE__PUSCH_PRESENT | RXSDU_LIST_STATE__PUSCH_SENT);
        MLogTask(PID_RXSDU_CTX0_SEND_TO_MAC_PUSCH+mlogPid, RESOURCE_LARM, tt, GET_TICKS);
    }
    else if (listType == PUCCH_LIST_TYPE)
    {
        gRxSduListState[rxSduCtx] &= ~(RXSDU_LIST_STATE__PUCCH_PRESENT | RXSDU_LIST_STATE__PUCCH_SENT);
        MLogTask(PID_RXSDU_CTX0_SEND_TO_MAC_PUCCH+mlogPid, RESOURCE_LARM, tt, GET_TICKS);
    }
    else if (listType == OTHER_LIST_TYPE)
    {
        MLogTask(PID_RXSDU_CTX0_SEND_TO_MAC_OTHER+mlogPid, RESOURCE_LARM, tt, GET_TICKS);
    }
}


//-------------------------------------------------------------------------------------------
/** @brief Send RxSDU list generated by Uplink Rx to MAC
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param listType - Type of list being send to MAC
 *  @param Last     - Check if it is time to append RxEnd message
 *  @param rxSduCtx - Current RxSDU context
 *  @param pRxEnd   - Pointer to the RxEnd message
 *
 *  @return    none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |              |        |                   |            |              |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |listType    |         |Input         |1       |unsigned int       |(1:31:0)    |[0:2]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |Last        |         |Input         |1       |unsigned int       |(1:31:0)    |[0:1]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |rxSduCtx    |         |Input         |1       |unsigned int       |(1:31:0)    |[0:3]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |pRxEnd      |         |pInput        |1       |pStructure         |            |              |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |SendRxSduList               |
 *  ---------------------------------------------------
 *  |Input Streams       |pRxEnd                      |
 *  ---------------------------------------------------
 *  |Output Streams      |                            |
 *  ---------------------------------------------------
 *  |Config Parameters   |listType, Last, rxSduCtx    |
 *  ---------------------------------------------------
 *  |Status Parameters   |                            |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 *  This function will send check if there are still RxSDU lists to
 *  send to the MAC from previous subframes and current subframes
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void SendRxSduList(U32 listType, U32 Last, U32 rxSduCtx, U8* pRxEnd)
{
    volatile PMAC2PHY_QUEUE_EL *localHead = (PMAC2PHY_QUEUE_EL *)GET_LIST_HEAD(listType, rxSduCtx);
    volatile PMAC2PHY_QUEUE_EL *localTail = (PMAC2PHY_QUEUE_EL *)GET_LIST_TAIL(listType, rxSduCtx);
    U32 tt = GET_TICKS;
    UINT32 prevCtx = GetPrevSduCtx(rxSduCtx), nextCtx, i;

    if ((*localHead))
    {
        // Build Rx End Message if Last Message is done
        if (Last)
        {
            (*localTail)->Next = (PMAC2PHY_QUEUE_EL)pRxEnd;
            pRxSduRxEnd[rxSduCtx] = NULL;
            MLogTask(PID_MSGMGR_RXEND_ADD_MSG, RESOURCE_LARM, tt, GET_TICKS);
            tt = GET_TICKS;
        }
    }
    else
    {
        // Push RxEnd to Queue because head is NULL only when Mute is enabled and this will be a memory leak if not sent to MAC
        if (Last)
        {
            PushPUCCHSdu((U32)pRxEnd);
            pRxSduRxEnd[rxSduCtx] = NULL;
        }
    }

    if (listType != OTHER_LIST_TYPE)  // Not Special Subframe
    {
        if (listType == PUSCH_LIST_TYPE)
            gRxSduListState[rxSduCtx] |= RXSDU_LIST_STATE__PUSCH_SENT;
        else if (listType == PUCCH_LIST_TYPE)
            gRxSduListState[rxSduCtx] |= RXSDU_LIST_STATE__PUCCH_SENT;

        // If there is nothing to send in the previous list then we can dispatch this list. Otherwise it is queued up
        if (gRxSduListState[prevCtx] == RXSDU_LIST_STATE__CLEAN)
        {
            SendToMac(listType, rxSduCtx);

            if (gRxSduListState[rxSduCtx] == RXSDU_LIST_STATE__CLEAN)
            {
                // This list is done. Check if there is something else to send in the next list
                nextCtx = GetNextSduCtx(rxSduCtx);
                for (i = 0; i < NUM_UL_SDU_CONTEXT; i++)
                {
                    if (gRxSduListState[nextCtx] != RXSDU_LIST_STATE__CLEAN)
                    {
                        if ((gRxSduListState[nextCtx] & (RXSDU_LIST_STATE__PUCCH_PRESENT | RXSDU_LIST_STATE__PUCCH_SENT))
                            == (RXSDU_LIST_STATE__PUCCH_PRESENT | RXSDU_LIST_STATE__PUCCH_SENT))
                            SendToMac(PUCCH_LIST_TYPE, nextCtx);
                        if ((gRxSduListState[nextCtx] & (RXSDU_LIST_STATE__PUSCH_PRESENT | RXSDU_LIST_STATE__PUSCH_SENT))
                            == (RXSDU_LIST_STATE__PUSCH_PRESENT | RXSDU_LIST_STATE__PUSCH_SENT))
                            SendToMac(PUSCH_LIST_TYPE, nextCtx);
                    }

                    // There is more stuff in this list to be sent. No point continuing.
                    if (gRxSduListState[nextCtx] != RXSDU_LIST_STATE__CLEAN)
                        break;

                    nextCtx = GetNextSduCtx(nextCtx);
                }
            }
        }
    }
    else
    {
        SendToMac(listType, rxSduCtx);
    }

    MLogTask(PID_MSGMGR_SEND_LIST, RESOURCE_LARM, tt, GET_TICKS);

    return;
}

//-------------------------------------------------------------------------------------------
/** @brief Adds Uplink RxSdu Indication message to the list of messages 
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param Context     - Pointer to the message to be added to the list
 *  @param listType    - Type of list being send to MAC
 *  @param status      - CRC Status from FEC
 *  @param rxSduCtx    - Current RxSDU context
 *  @param listElement - Pointer to the list of messages
 *
 *  @return    none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |              |        |                   |            |              |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |Context     |         |pInput        |1       |pStructure         |            |              |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |status      |         |Input         |1       |unsigned int       |(1:31:0)    |[0:1]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |listType    |         |Input         |1       |unsigned int       |(1:31:0)    |[0:2]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |rxSduCtx    |         |Input         |1       |unsigned int       |(1:31:0)    |[0:3]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |listElement |         |pOutput       |1       |pStructure         |            |              |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteRxSduAdd2List            |
 *  ---------------------------------------------------
 *  |Input Streams       |Context                     |
 *  ---------------------------------------------------
 *  |Output Streams      |listElement                 |
 *  ---------------------------------------------------
 *  |Config Parameters   |status, listType, rxSduCtx  |
 *  ---------------------------------------------------
 *  |Status Parameters   |                            |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * Function will add Uplink RxSdu Indication Message to the list of 
 * messages to be sent to the MAC.   Message types include PUCCH and 
 * PUSCH RxSDU indications.
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void LteRxSduAdd2List(void *Context, U32 status, U32 listType, U32 rxSduCtx, void *listElement)
{
    PLTE_PHY_UL_SDU_DESC pRxSduDesc = (PLTE_PHY_UL_SDU_DESC)Context;
    PMAC2PHY_QUEUE_EL pListElementHeader = (PMAC2PHY_QUEUE_EL)listElement;
    PRXSDUIND pRxSduInd;
    volatile U32 tt;
    volatile PMAC2PHY_QUEUE_EL *localHead = (PMAC2PHY_QUEUE_EL *)GET_LIST_HEAD(listType, rxSduCtx);
    volatile PMAC2PHY_QUEUE_EL *localTail = (PMAC2PHY_QUEUE_EL *)GET_LIST_TAIL(listType, rxSduCtx);

    if(pRxSduDesc == NULL)
        return;

    if(pListElementHeader == NULL)
        return;

    tt = GET_TICKS;

    // First build the common part of the reply message and setup PhyInstance
    pRxSduInd                           = (PRXSDUIND) pListElementHeader->MessagePtr;
    pRxSduInd->status                   = (U8) status;
    pRxSduInd->timingAdv                = pRxSduDesc->TimingAdvance;
    pRxSduInd->mErrAvgCh[0]             = pRxSduDesc->mErrAvg[0];
    pRxSduInd->mErrAvgCh[1]             = pRxSduDesc->mErrAvg[1];
    pRxSduInd->mErrExpo                 = pRxSduDesc->mErrExpo;
    pRxSduInd->ul_CQI                   = pRxSduDesc->ul_CQI;
    pRxSduInd->rssiMeasurement          = (U8)pRxSduDesc->Rssi_dB;

    if (pRxSduInd->chanType == PUCCH)
    {
        pRxSduInd->srdetected           = pRxSduDesc->srdetected;
        pRxSduInd->timingAdv            = 31;
        pRxSduInd->cqiPmiConf           = ((pRxSduDesc->cqiPmiConf>>14) & 0x0003);
        if (pRxSduDesc->numBitsRx)              // This means No DTX
            pRxSduInd->pucchDetected    = 1;
        else                                    // This means DTX
            pRxSduInd->numBitsRx        = 0;
    }

    if((*localHead) == NULL)
    {
        (*localHead)                    = pListElementHeader;
        (*localTail)                    = pListElementHeader;
    }
    else
    {
        (*localTail)->Next              = pListElementHeader;
        (*localTail)                    = pListElementHeader;
    }

    SvsrLoggerAddMsg(PHY_RXSDU_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), pRxSduInd, 5 + sizeof(RXSDUIND));

    MLogTask(PID_MSGMGR_RXSDU_BUILD_MSG, RESOURCE_LARM, tt, GET_TICKS);
}

//-------------------------------------------------------------------------------------------
/** @brief Adds Uplink RxStatus messages from Mux PUSCH to list of messages
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param Context     - Pointer to the message to be added to the list
 *  @param listType    - Type of list being send to MAC
 *  @param typeCqiRiHi - Type of Mux PUSCH message
 *  @param rxSduCtx    - Current RxSDU context
 *  @param listElement - Pointer to the list of messages
 *
 *  @return    none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |              |        |                   |            |              |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |Context     |         |pInput        |1       |pStructure         |            |              |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |typeCqiRiHi |         |Input         |1       |unsigned int       |(1:31:0)    |[0:1]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |listType    |         |Input         |1       |unsigned int       |(1:31:0)    |[0:2]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |rxSduCtx    |         |Input         |1       |unsigned int       |(1:31:0)    |[0:3]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |listElement |         |pOutput       |1       |pStructure         |            |              |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteRxStatusCqiRiHiAdd2List  |
 *  ---------------------------------------------------
 *  |Input Streams       |Context                     |
 *  ---------------------------------------------------
 *  |Output Streams      |listElement                 |
 *  ---------------------------------------------------
 *  |Config Parameters   |typeCqiRiHi, listType,      |
 *  |                    |rxSduCtx                    |
 *  ---------------------------------------------------
 *  |Status Parameters   |                            |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * Function will add Uplink RxStatus Message to the list of messages to
 * be sent to the MAC.   Message type included Mux PUSCH
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void LteRxStatusCqiRiHiAdd2List(void *Context, U32 typeCqiRiHi, U32 listType, U32 rxSduCtx, void *listElement)
{
    PLTE_PHY_UL_SDU_DESC pRxSduDesc  = (PLTE_PHY_UL_SDU_DESC)Context;
    PMAC2PHY_QUEUE_EL pListElementHeader = (PMAC2PHY_QUEUE_EL)listElement;
    PRXSTATUSIND pRxStatusInd;
    PRX_CQIRIHI_STATUS_EVENT pRxCqiRiHiStatusEvent;
    volatile PMAC2PHY_QUEUE_EL *localHead = (PMAC2PHY_QUEUE_EL *)GET_LIST_HEAD(listType, rxSduCtx);
    volatile PMAC2PHY_QUEUE_EL *localTail = (PMAC2PHY_QUEUE_EL *)GET_LIST_TAIL(listType, rxSduCtx);

    U32 tt;
    U8 *ptr;
    U8 *cqiOut, *riOut, *ackOut;
    U16 numBytesCQI, numBytesRI, numBytesACK;
    U32 chIdx;
    ULRXFECPARAMS   *pFecPars;

    if (pRxSduDesc == NULL)
        return;

    if (pListElementHeader == NULL)
        return;

    tt                                  = GET_TICKS;

    pFecPars                            = pRxSduDesc->pUlRxFecParams;
    chIdx                               = pRxSduDesc->chanIndex;

    // First build the common part of the reply message and setup PhyInstance
    pRxStatusInd                        = (PRXSTATUSIND) pListElementHeader->MessagePtr;
    pRxCqiRiHiStatusEvent               = (PRX_CQIRIHI_STATUS_EVENT)&pRxStatusInd->pEventList;
    pRxCqiRiHiStatusEvent->cqiPmiConf   = ((pRxSduDesc->cqiPmiConf>>14) & 0x0003);
    ptr                                 = (U8*)(pRxCqiRiHiStatusEvent + 1);

    cqiOut                              = &pFecPars->cqiStore[chIdx][0];
    riOut                               = &pFecPars->riStore[chIdx];
    ackOut                              = &pFecPars->ackStore[chIdx];

    numBytesCQI                         = (U16)((pFecPars->nr1CQI[chIdx] + pFecPars->nrg1CQI[chIdx] + 7) >> 3);
    numBytesRI                          = (U16)((pFecPars->nRI[chIdx]  + 7) >> 3);
    numBytesACK                         = (U16)((pFecPars->nACK[chIdx] + 7) >> 3);

    if (gTwoRxMuxControlLists)              // Should send only correct stuff out
    {
        if (typeCqiRiHi == MUX_PUSCH_ACK_NACK)
        {
            numBytesCQI                 = 0;
            numBytesRI                  = 0;
        }
        else if (typeCqiRiHi == MUX_PUSCH_CQI_RI)
        {
            numBytesACK                 = 0;
        }
    }

    if (numBytesCQI)
    {
        memcpy(ptr, cqiOut, numBytesCQI);
        pRxCqiRiHiStatusEvent->status_len   = numBytesCQI;
        ptr                                += numBytesCQI;
    }

    if (numBytesRI)
    {
        // Output byte. Bit7 is 1st decoded bit and bit6 is 2nd decoded bit.
        // Bit5 is used to indicate valid reception (1=yes, 0=no).
        *ptr                                = *riOut & 0xE0;
        ptr++;
        pRxCqiRiHiStatusEvent->status_len++;
    }

    if (numBytesACK)
    {
        // Output byte. Bit7 is 1st decoded bit and bit6 is 2nd decoded bit.
        // Bit5 is used to indicate valid reception (1=yes, 0=no).
        *ptr                                = *ackOut & 0xE0;
        ptr++;
        pRxCqiRiHiStatusEvent->status_len++;
    }

    if((*localHead) == NULL)
    {
        (*localHead)        = pListElementHeader;
        (*localTail)        = pListElementHeader;
    }
    else
    {
        (*localTail)->Next  = pListElementHeader;
        (*localTail)        = pListElementHeader;
    }

    SvsrLoggerAddMsg (PHY_RXSTATUS_IND, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), pRxStatusInd, pListElementHeader->MessageLen);

    MLogTask(PID_MSGMGR_RXSTATUS_BUILD_MSG, RESOURCE_LARM, tt, GET_TICKS);
}


//-------------------------------------------------------------------------------------------
/** @brief Adds Uplink messages from Delayed API to list
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param typeCqiRiHi - Type of Mux PUSCH message
 *  @param rxSduCtx    - Current RxSDU context
 *  @param listElement - Pointer to the list of messages
 *
 *  @return    none
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  -------------------------------------------------------------------------------------------------------
 *  |Name        |Name in  |Direction     |Length  |Type               |Data Format |Valid Range   |Units |
 *  |            |standard |              |        |                   |            |              |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |listType    |         |Input         |1       |unsigned int       |(1:31:0)    |[0:2]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |rxSduCtx    |         |Input         |1       |unsigned int       |(1:31:0)    |[0:3]         |      |
 *  -------------------------------------------------------------------------------------------------------
 *  |listElement |         |pOutput       |1       |pStructure         |            |              |      |
 *  -------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteDelayedApiIndAdd2List    |
 *  ---------------------------------------------------
 *  |Input Streams       |                            |
 *  ---------------------------------------------------
 *  |Output Streams      |listElement                 |
 *  ---------------------------------------------------
 *  |Config Parameters   |listType, rxSduCtx          |
 *  ---------------------------------------------------
 *  |Status Parameters   |                            |
 *  ---------------------------------------------------
 *  BLOCK_END
 *
 *
 *
 * <4> Detailed Description:
 * Function will add messages from Delayed API to the list of messages to
 * be sent to the MAC
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 * <6> References:
 *
 *  \ingroup group_lte_api
 *
 **/
//-------------------------------------------------------------------------------------------
void LteDelayedApiIndAdd2List(U32 listType, U32 rxSduCtx, void *listElement)
{
    PMAC2PHY_QUEUE_EL pListElementHeader = (PMAC2PHY_QUEUE_EL)listElement;
    volatile PMAC2PHY_QUEUE_EL *localHead = (PMAC2PHY_QUEUE_EL *)GET_LIST_HEAD(listType, rxSduCtx);
    volatile PMAC2PHY_QUEUE_EL *localTail = (PMAC2PHY_QUEUE_EL *)GET_LIST_TAIL(listType, rxSduCtx);

    U32 tt = GET_TICKS;

    if (listElement == NULL)
        return;
    else
    {
        if((*localHead) == NULL)
        {
            (*localHead)        = pListElementHeader;
            (*localTail)        = pListElementHeader;
        }
        else
        {
            (*localTail)->Next  = pListElementHeader;
            (*localTail)        = pListElementHeader;
        }
    }

    SvsrLoggerAddMsg (pListElementHeader->MessageType, LOGGER_MSG_INFO(0, 0, IID_LTE, IID_LTE_MAC), pListElementHeader->MessagePtr, pListElementHeader->MessageLen);

    MLogTask(PID_MSGMGR_DELAYED_API_RESPONSE_BUILD_MSG, RESOURCE_LARM, tt, GET_TICKS);
}



#endif /*#ifdef _WIN32*/



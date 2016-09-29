//-------------------------------------------------------------------------------------------
/** @file lteparse.c
 *
 * @brief This file implements the parser of the LTE L2-PHY API
 *        defined in Mindspeed Document MSA-E81613
 * @author Mindspeed Technologies
 * @version $Revision: 1.90 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef _WIN32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "Appids.h"
#include "svsrlogger_c_code.h"

#include "typedef.h"
#include "ltephydef.h"
#include "prototypes.h"
#include "ext_phy.h"

#include "ltemac.h"
#include "basic_op_cc.h"
#include "lte_bs_phy_nmm_fdd.h"

//Global variable needed for SVSR logger for C-code
//to simulate pPhyDiCtx->PhyStartCurrCount in Real-Time Project
//defined in lte_test_config.c
extern U32 PhyStartCurrCount;


//-------------------------------------------------------------------------------------------
/** @brief Parses the messages for the API between L2 and PHY
 *         layers for an LTE Base Station
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *  @param phyInstance Defines the particular PHY that is going to perform the
 *                     processing of the messages and input samples
 *  @param inMsg
 *                     Pointer to the input LTE L2-PHY API Message
 *  @param inMsgSize
 *                     Input LTE L2-PHY message size in long words
 *  @param outMsg
 *                     Pointer to the output LTE L2-PHY API Message
 *  @param outMsgSize
 *                     Pointer to Output LTE L2-PHY message size in long words
 *
 *  @return status     A value of zero indicate success a value of 1 indicates
 *                     failure (We can expand to indicate the specific failure
 *                     reason)
 *  @return    None.
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  ---------------------------------------------------------------------------------------------------------
 *  |Name       |Name in   |Direction      |Length  |Type                 |Data Format |Valid Range |Units  |
 *  |           |Standard  |               |        |                     |            |            |       |
 *  ---------------------------------------------------------------------------------------------------------
 *  |phyInstance|          |Input          |1       |unsigned int         |(1:31:0)    |[0:2]       |       |
 *  ---------------------------------------------------------------------------------------------------------
 *  |inMsg      |          |pInput         |1       |punsigned int        |(1:31:0)    |(0,2^32)    |       |
 *  ---------------------------------------------------------------------------------------------------------
 *  |inMsgSize  |          |Input          |1       |unsigned int         |(1:31:0)    |(0,TBD]     |       |
 *  ---------------------------------------------------------------------------------------------------------
 *  |outMsg     |          |pOutput        |1       |punsigned int        |(1:31:0)    |(0,2^32)    |       |
 *  ---------------------------------------------------------------------------------------------------------
 *  |outMsgSize |          |pOutput        |1       |unsigned int         |(1:31:0)    |(0,2^32)    |       |
 *  ---------------------------------------------------------------------------------------------------------
 *  |status     |          |Output         |1       |unsigned int         |(1:31:0)    |[0,1]       |       |
 *  ---------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ---------------------------------------------------
 *  |Block Name          |LteParseMsg                 |
 *  ---------------------------------------------------
 *  |Input Streams       |inMsg,inMsgSize             |
 *  ---------------------------------------------------
 *  |Output Streams      |outMsg                      |
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
 *  This function takes an LTE L2-PHY API command and parses it and if the
 *  parsing is yields a valid command it executes the command and provides an
 *  indication to the client
 *  The LTE L2-LTE APY messages are carried using the Mindspeed generic message
 *  format described in MSA-81479 using MessageType 0x0002 (Message for PHY)
 *  and MessageID 0x7000 Foreign API Message
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
U32 LteParseMsg(U32 phyInstance, U32* inMsg, U32 inMsgSize, U32* outMsg, U32* outMsgSize)
{
    U16 len = 0, chanId, numBytes, cwId;
    U32* ptr;
    PGENMSGDESC pMsg = (PGENMSGDESC) inMsg;
    PTXSDUREQ pTxSduReq = (PTXSDUREQ) inMsg;
    PDLSUBFRDESC pDlSubFrDesc;
    PHIINFOMSGDESC pTxHiSduReq = (PHIINFOMSGDESC) inMsg;
    PDCIULSDUMSG pTxDciSduReq = (PDCIULSDUMSG) inMsg;
    PHIADCIULMSGDESC pHiDciMsg = (PHIADCIULMSGDESC) inMsg;
    PIOParams_DlControl pDlCntrl_Pars;
    PINITPARM pInit;
    U32 status = 0;
    PLTEPHYDESC pPhy;

    // init respon msg size
    *outMsgSize = 0;

    uart_printf("@@@: LteParseMsg: msgType=%d\n", pMsg->msgType);

    if(pMsg->msgType >=  NMM_START && pMsg->msgType < NMM_final_to_be_updated)
    {
        lte_bs_phy_nmm_parse_msg(phyInstance, inMsg, inMsgSize, outMsg, outMsgSize);
    }
    else
    {
        switch (pMsg->msgType)
        {
            case PHY_TXSTART_REQ:
                // Process TXSTART_REQ
                // First extract the len
                len = (U16) pMsg->msgSpecific;
                // The len is used in the validation of the TXVECTOR info
                ptr = ++inMsg;
                gCtxDlFec = 0;

                SvsrLoggerAddMsg(PHY_TXSTART_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsg,
                                 len + sizeof(GENMSGDESC));

                pPhy                          = getPhyCtxApiProc(phyInstance);
                pDlSubFrDesc                  = (PDLSUBFRDESC)ptr;
                pPhy->txFrameNumber           = pDlSubFrDesc->frameNumber;
                pPhy->pBsTx->subframeNumber   = pDlSubFrDesc->subframeNumber;

                status      = LteBsPhyTxVectorProcData(pDlSubFrDesc, pPhy->pBsTx->pTxInputParams, 0);
                status      = LteBsPhyTxVectorProcControl(pDlSubFrDesc, pPhy->pBsTx->pDlCntrl_TxInPars, pPhy->pBsTx->pTxInputParams);

                //////////////////////////////////////////////////////////////////////////
                //Check if we need to insert PBCH
                //////////////////////////////////////////////////////////////////////////
                if (pPhy->pBsTx->subframeNumber == 0)
                    InsertPBCH(pPhy);

                gLastTxSfn  = LteBsConstructSfn(pPhy->txFrameNumber, pPhy->pBsTx->subframeNumber);

                // Send TXSTART.confirmation if enabled
                if (pPhy->confEnableMask & CONF_TXSTART)
                {
                    LteGenerateAPIMsg(phyInstance, outMsg, outMsgSize, PHY_TXSTART_CONF, status);
                }

                break;
            case PHY_TXSDU_REQ:

                // Process TXSDU_REQ
                chanId = (U16) pMsg->msgSpecific;
                numBytes = pTxSduReq->msgLen;

                // In "MacBsSendCtl() & MacBsSendData() ", we copy SDUREQ databuf to msgbody, thus here fetch data from msgbody.
                ptr = (U32*) ((U8*) pMsg + sizeof(TXSDUREQ));
                cwId = pTxSduReq->cwId;

                SvsrLoggerAddMsg(PHY_TXSDU_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pTxSduReq,
                                 (U32) numBytes + sizeof(TXSDUREQ));

                pPhy                          = getPhyCtxApiProc(phyInstance);
                uart_printf("    @@@: (%d, %d) ChanType: =%d, chanId: %d, numBytes: %d, cwId: %d\n", pPhy->txFrameNumber, pPhy->pBsTx->subframeNumber, pTxSduReq->channelType, chanId, numBytes, cwId);

                // Check Channel Type and invoke appropriate TxSduProcessing Function
                if (pTxSduReq->channelType == PHICH)
                {
                    // Process TXSDU
                    status = LteBsTxHiSduProcessing(phyInstance, pTxSduReq, chanId, numBytes, cwId);
                }
                else
                {
                    // Process TXSDU
                    if (pTxSduReq->channelType == PBCH)
                    {
                        if (pPhy->pBsTx->subframeNumber == 0)
                        {
                            status = LteBsTxSduProcessing(phyInstance, ptr, chanId, pTxSduReq, cwId);
                        }
                    }
                    else
                    {
                        status = LteBsTxSduProcessing(phyInstance, ptr, chanId, pTxSduReq, cwId);
                    }
                }
                pPhy        = getPhyCtxApiProc(phyInstance);

                // Send TXSDU.confirmation if enabled
                if (pPhy->confEnableMask & CONF_TXSDU)
                {
                    LteGenerateTxSduConfMsg(phyInstance, outMsg, outMsgSize, status, (U32) chanId);
                }

                break;
            case PHY_TXHIADCIUL_REQ:
                pPhy     = getPhyCtxApiProc(phyInstance);
                numBytes = pHiDciMsg->msgLength;
                SvsrLoggerAddMsg (PHY_TXDCIULSDU_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pHiDciMsg, (numBytes + sizeof (HIADCIULMSGDESC)));

                pDlCntrl_Pars = pPhy->pBsTx->pDlCntrl_TxInPars;
                pDlCntrl_Pars->numDCI0 = pHiDciMsg->numDciUlSdus;
                pDlCntrl_Pars->number_of_phich_channels = pHiDciMsg->numHiSdus;

                break;
            case PHY_TXHISDU_REQ:
                // Process PHY_TXHISDU_REQ
                chanId = (U16) pTxHiSduReq->channelId;

                SvsrLoggerAddMsg(PHY_TXHISDU_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pTxHiSduReq,
                                 (U32) sizeof(HIINFOMSGDESC));

                status = LteBsTxHiSduReqProcessing(phyInstance, pTxHiSduReq, chanId);

                break;
            case PHY_TXDCIULSDU_REQ:
                // Process PHY_TXHISDU_REQ
                chanId = (U16) pTxDciSduReq->channelId;
                numBytes = pTxDciSduReq->msgLength;

                SvsrLoggerAddMsg(PHY_TXDCIULSDU_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pTxDciSduReq,
                                 (U32) numBytes + sizeof(DCIULSDUMSG));

                status = LteBsTxDciSduReqProcessing(phyInstance, pTxDciSduReq, chanId, numBytes);

                break;
            case PHY_RXSTART_REQ:
                // Process RXSTART_REQ
                // First extract the len

                len = (U16) pMsg->msgSpecific;
                // The len is used in the validation of the RXVECTOR info
                ptr = ++inMsg;

                SvsrLoggerAddMsg(PHY_RXSTART_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsg,
                                 len + sizeof(GENMSGDESC));

                status      = LteBsPhyRxVectorProcessing(phyInstance, (PULSUBFRDESC) ptr, (U32) len);
                pPhy        = getPhyCtxApiProc(phyInstance);
                gLastRxSfn  = LteBsConstructSfn(pPhy->rxFrameNumber, pPhy->pBsRx->subframeNumber);

                // Send RXSTART.confirmation if enabled
                if (pPhy->confEnableMask & CONF_RXSTART)
                {
                    LteGenerateAPIMsg(phyInstance, outMsg, outMsgSize, PHY_RXSTART_CONF, status);
                }

                break;
            case PHY_INIT_REQ:
                // Process INIT_REQ
                // First extract the len
                len = (U16) pMsg->msgSpecific;
                // Check that len matches the structure size
                if (len != sizeof(INITPARM))
                {
                    status = FAILURE;
                }
                // Now invoke initialization function
                ptr = ++inMsg;

                SvsrLoggerInit(0xFFFFFFFF);  // Enable all masks
                SvsrLoggerAddInit(PHY_INIT_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsg,
                                  len + sizeof(GENMSGDESC));

                pInit = (PINITPARM) (ptr);

                gPhyRobustControl        =  (pInit->phyCfg & PHYINIT_ROBUST_CONTROL);
                gPhyInternalBch          =  (pInit->phyCfg & PHYINIT_PHY_MAINTAINS_PBCH);
                gUseTxSduPointer         =  (pInit->phyCfg & PHYINIT_USE_TXSDU_POINTER);
                gTwoRxMuxControlLists    =  (pInit->phyCfg & PHYINIT_TWO_MUXCONTROL_LISTS);
                gSendSecondTTIIndication =  (pInit->phyCfg & PHYINIT_PHY_SEND_SECOND_TTI_IND);
                gMuteRxSduForSrRach      =  (pInit->phyCfg & PHYINIT_MUTE_RXSDU_FOR_SR_RACH);
                gLowLatencyFlag          =  (pInit->phyCfg & PHYINIT_LOW_LATENCY_PATH);

                status = LteBsPhyInit(phyInstance, (PINITPARM) ptr);

                uart_printf("Interfaces Enabled:\r\n");
                uart_printf("--------------------\r\n\n");
                uart_printf("phyCfg = %04x:\r\n", pInit->phyCfg);
                uart_printf("PHYINIT_ROBUST_CONTROL:                   %s\r\n", (gPhyRobustControl ? "YES":"NO"));
                uart_printf("PHYINIT_PHY_MAINTAINS_PBCH:               %s\r\n", (gPhyInternalBch ? "YES":"NO"));
                uart_printf("PHYINIT_USE_TXSDU_POINTER:                %s\r\n", (gUseTxSduPointer ? "YES":"NO"));
                uart_printf("PHYINIT_TWO_MUXCONTROL_LISTS:             %s\r\n", (gTwoRxMuxControlLists ? "YES":"NO"));
                uart_printf("PHYINIT_PHY_SEND_SECOND_TTI_IND:          %s\r\n", (gSendSecondTTIIndication ? "YES":"NO"));
                uart_printf("PHYINIT_MUTE_RXSDU_FOR_SR_RACH:           %s\r\n", (gMuteRxSduForSrRach ? "YES":"NO"));
                uart_printf("PHYINIT_LOW_LATENCY_PATH:                 %s\r\n", (gLowLatencyFlag ? "YES":"NO"));
                uart_printf("\r\n");

                // Once the initialization has completed issue the PHY_INIT.indication
                LteGenerateAPIMsg(phyInstance, outMsg, outMsgSize, PHY_INIT_IND, status);
                break;
            case PHY_START_REQ:

                SvsrLoggerAddMsg(PHY_START_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsg,
                                 sizeof(STARTREQ));

                PhyStartCurrCount++;
                LteGenerateAPIMsg(phyInstance, outMsg, outMsgSize, PHY_START_CONF, 0);

                gMode = LTE_MODE;
                break;
            case PHY_STOP_REQ:
                // Stop before sending confirmation
                LteGenerateAPIMsg(phyInstance, outMsg, outMsgSize, PHY_STOP_CONF, 0);

                SvsrLoggerAddMsg(PHY_STOP_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsg,
                                 sizeof(GENMSGDESC));
                SvsrLoggerCloseStorage();
                break;

            case PHY_SHUTDOWN_REQ:
                // Stop before sending confirmation
                LteGenerateAPIMsg(phyInstance, outMsg, outMsgSize, PHY_SHUTDOWN_CONF, 0);
                SvsrLoggerAddMsg(PHY_SHUTDOWN_REQ, LOGGER_MSG_INFO(0, 0, IID_LTE_MAC, IID_LTE), pMsg,
                                 sizeof(GENMSGDESC));
                SvsrLoggerCloseStorage();
                LteBsPhyDestroy(0);
                break;
            default:
                // Not a valid command return failure
                status = FAILURE;
                break;
        }
   }

    return (status);
}

#endif

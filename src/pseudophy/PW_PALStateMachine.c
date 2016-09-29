/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#include <string.h>
#ifdef LINUX
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include "MemoryDebug.h"
#include "PW_PALStateMachine.h"
#include "PW_PALInterface.h"
#include "PW_PALDefinition.h"
#include "PW_PALSocket.h"
#include "PW_PALMessageParser.h"
#include "PW_PALMessageBuilder.h"
#include "PW_PALMessageQueue.h"
#include "PW_PALTimer.h"

#define __sModuleName__ "FSM"
static int s_arriSockFd[MAX_UE_COUNT] = {-1};
static unsigned short s_usSFN = 0;
static unsigned char s_ucSubframe = 0;
static unsigned char s_txBuffer[PW_MAX_MSG_LENGTH];
static int s_txBufLen = -1;
static SSockFdToRntiMap s_Map;
extern int iUeCount;
extern int iServicePort;

static void _InitializeMap()
{
    int count;
    for(count = 0; count < MAX_UE_COUNT; count++)
    {
        s_Map.iSockFd[count] = -1;
        s_Map.usRnti[count] = 0;
        s_Map.bIsRntiValid[count] = PN_FALSE;
        s_Map.bTxDataPresent[count] = PN_FALSE;
    }
}

static int _UpdateTTIParams()
{
    if(s_ucSubframe == MAX_SUBFRAME_COUNT - 1)
    {
        s_ucSubframe = 0;
        if(s_usSFN == MAX_SFN_COUNT)
        {
            s_usSFN = 0;
        }
        else
        {
            s_usSFN++;
        }
    }
    else
    {
        s_ucSubframe++;
    }
    return PN_SUCCESS;
}

static int _FSMProcessPHYPrimitive(PW_EMspdPhyPrimitive ePhyPrimitive)
{
    struct timeval dlapiTimeout;
    int iDlApiMsgQueueFd = 0;
    fd_set dlapiReadfdSet;
    int dlapiReadyfds = 0;
    unsigned char dlapiBuffer[PW_MAX_MSG_LENGTH];
    int iMsgLen = 0;
    int count;

    dlapiTimeout.tv_sec = 0;
    dlapiTimeout.tv_usec = 5000;
    FD_ZERO(&dlapiReadfdSet);
    iDlApiMsgQueueFd = MQGetQueueId(EPW_MSGQ_TYPE_DLAPI);
    if(iDlApiMsgQueueFd == -1)
    {
        LOG_ERROR(ELOG_MESSAGE, "Unable to get DL API Message QueueRead Fd");
    }
    FD_ZERO(&dlapiReadfdSet);
    FD_SET(iDlApiMsgQueueFd, &dlapiReadfdSet);
    dlapiReadyfds = select(iDlApiMsgQueueFd + 1, &dlapiReadfdSet,
        NULL, NULL, &dlapiTimeout);
    while (dlapiReadyfds > 0)
    {
        if (FD_ISSET(iDlApiMsgQueueFd, &dlapiReadfdSet))
        {
            iMsgLen = UTILRetrieveMessageFromMessageQueue(iDlApiMsgQueueFd,
                dlapiBuffer, PW_MAX_MSG_LENGTH, 0);
            if (iMsgLen > 0)
            {
                TDLAPIEventInfo *psDLAPIEventInfo = 
                    *((TDLAPIEventInfo**)dlapiBuffer);
                switch(psDLAPIEventInfo->m_eMspdPhyPrimitive)
                {
                case EMSPD_PHY_TXSTART_request:
                    LOG_INFO(ELOG_MESSAGE, "Received PHY_TXSTART_request");
                    break;
                case EMSPD_PHY_TXSDU_request:
                    LOG_INFO(ELOG_MESSAGE, "Received PHY_TXSDU_request");
                    STfuDatReqInfo* psTfuDatReqInfo = NULL;
                    psTfuDatReqInfo = (STfuDatReqInfo*)UTILMalloc(sizeof(STfuDatReqInfo));
                    if(psTfuDatReqInfo != NULL)
                    {
                        psTfuDatReqInfo->m_usCellId = 0;
                        psTfuDatReqInfo->m_sTimingInfo.m_usSFN = s_usSFN;
                        psTfuDatReqInfo->m_sTimingInfo.m_ucSubframe = s_ucSubframe;
                        /*If transport channel is BCH*/
                        if(psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_ucChannelType == CH_PBCH)
                        {
                            psTfuDatReqInfo->m_usLengthOfBCHBuffer =
                                psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usPhySduSizeInBytes;
                            psTfuDatReqInfo->m_pucBCHBuffer = 
                                (unsigned char*)UTILMalloc(psTfuDatReqInfo->m_usLengthOfBCHBuffer);
                            UTILMemcpy(psTfuDatReqInfo->m_pucBCHBuffer,
                                psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_pucPhySdu,
                                psTfuDatReqInfo->m_usLengthOfBCHBuffer);
                            s_txBufLen =
                                BuildDLDataRequest(s_txBuffer, psTfuDatReqInfo);
                            /*for(count = 0; count < iUeCount; count++)
                            {
                                if(s_Map.iSockFd[count] != -1)
                                {
                                    s_Map.bTxDataPresent[count] = PN_TRUE;
                                }
                            }*/
                            s_Map.bIsBroadcast = PN_TRUE;

                        }
                        else if(psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_ucChannelType == CH_PDSCH)
                        {
                            char logMsg[128];
                            psTfuDatReqInfo->m_usNumberOfTFUDATREQPDUINFO = 1;
                            psTfuDatReqInfo->m_ppsTfuDatReqPduInfo =
                                (STfuDatReqPduInfo**)UTILMalloc(sizeof(STfuDatReqPduInfo*));
                            psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[0] =
                                (STfuDatReqPduInfo*)UTILMalloc(sizeof(STfuDatReqPduInfo));

                            sprintf(logMsg, "Data received to transmit on PDSCH for RNTI %d", 
                                psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usRnti);
                            LOG_DEBUG(ELOG_MESSAGE, logMsg);
                            if(psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usRnti == SI_RNTI ||
                                psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usRnti == P_RNTI ||
                                psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usRnti >= TEMP_C_RNTI_START)
                            {
                                /*Will handle SI-RNTI and C-RNTI*/
                                psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[0]->m_usRNTI =
                                    psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usRnti;

                                for(count = 0; count < iUeCount; count++)
                                {
                                    if(s_Map.iSockFd[count] != -1 && s_Map.bIsRntiValid[count] == PN_FALSE)
                                    {
                                        if(psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usRnti != SI_RNTI)
                                        {
                                            s_Map.usRnti[count] = 
                                                psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usRnti;
                                            s_Map.bIsRntiValid[count] = PN_TRUE;
                                            s_Map.bTxDataPresent[count] = PN_TRUE;
                                            break; 
                                        }
                                        else
                                        {
                                            /*s_Map.bTxDataPresent[count] = PN_TRUE;*/
                                            s_Map.bIsBroadcast = PN_TRUE;
                                            break;
                                        }
                                    }

                                    if(s_Map.iSockFd[count] != -1 && s_Map.bIsRntiValid[count] == PN_TRUE)
                                    {
                                        if(s_Map.usRnti[count] == psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usRnti)
                                        {
                                            s_Map.bTxDataPresent[count] = PN_TRUE;
                                            break;
                                        }
                                        
                                        if(psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usRnti == SI_RNTI)
                                        {
                                            s_Map.bIsBroadcast = PN_TRUE;
                                            break;
                                        }

                                        if(psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usRnti == P_RNTI)
                                        {
                                            s_Map.bTxDataPresent[count] = PN_TRUE;
                                            LOG_DEBUG(ELOG_MESSAGE, "Flag set for Paging");
                                            break;
                                        }
                                    }
                                } /*end for*/
                            } /*end if SI-RNTI, P-RNTI, C-RNTI*/
                            else
                            {
                                /*Since UE-MAC expects 1 as RA-RNTI, it is set to 1*/
                                psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[0]->m_usRNTI = 1;
                                for(count = 0; count < iUeCount; count++)
                                {
                                    if(s_Map.iSockFd[count] != -1 &&
                                        s_Map.bIsRntiValid[count] == PN_FALSE)
                                    {
                                        s_Map.bTxDataPresent[count] = PN_TRUE;
                                        break;
                                    }
                                }
                            }

                            psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[0]->m_ucNumberOfTB = 1;

                            psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[0]->m_puiMacPdubufferLen =
                                (unsigned int*)UTILMalloc(sizeof(unsigned int));

                            psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[0]->m_puiMacPdubufferLen[0] =
                                psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usPhySduSizeInBytes;

                            psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[0]->m_ppucMacPduBuffer =
                                (unsigned char**)UTILMalloc(sizeof(unsigned char*));

                            psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[0]->m_ppucMacPduBuffer[0] =
                                (unsigned char*)UTILMalloc(psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[0]->m_puiMacPdubufferLen[0]);

                            UTILMemcpy(psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[0]->m_ppucMacPduBuffer[0],
                                psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_pucPhySdu,
                                psTfuDatReqInfo->m_ppsTfuDatReqPduInfo[0]->m_puiMacPdubufferLen[0]);

                            s_txBufLen = BuildDLDataRequest(s_txBuffer, psTfuDatReqInfo);
                        } /*end PDSCH*/
                    }
                    break;
                case EMSPD_PHY_RXSTART_request:
                    break;
                case EMSPD_PHY_INIT_request:
                    break;
                case EMSPD_PHY_RECONFIG_request:
                    break;
                case EMSPD_PHY_START_request:
                    break;
                case EMSPD_PHY_STOP_request:
                    break;
                default:
                    break;
                }
            }
        }
        dlapiReadyfds--;
    }
    return PN_SUCCESS;
}

int _ProcessUEPhyMessage(int bytesRead, unsigned char* rxBuffer, int iSockFdToFlush)
{
    unsigned short usMsgType;
    usMsgType = GetMessageType(rxBuffer);

    switch(usMsgType)
    {
    case RANDACSIND:
        {
            SRandomAccessIndication sRandomAccessIndication = {0};
            STfuRaReqIndInfo* psTfuRaReqIndInfo = NULL;
            if(DecodeRandomAccessIndication(rxBuffer, &sRandomAccessIndication) > 0)
            {
                int count;
                LOG_DEBUG(ELOG_MESSAGE, "Successfully decoded Random Access Indication");
                psTfuRaReqIndInfo = &(sRandomAccessIndication.m_sTfuRaReqIndInfo);
                PW_PALSendPhyRxStartIndication(s_ucSubframe);
                    PW_PALSendPhyRxSduIndicationForRACH(s_ucSubframe,
                    psTfuRaReqIndInfo->m_ppsTfuRachInfo[0]->m_ppsTfuRaReqInfo[0]->m_ucPreambleId);
                for(count = 0; count < iUeCount; count++)
                {
                    if(s_Map.iSockFd[count] == iSockFdToFlush)
                    {
                        s_Map.usRnti[count] = 0;
                        s_Map.bIsRntiValid[count] = PN_FALSE;
                        s_Map.bTxDataPresent[count] = PN_FALSE;
                        break;
                    } 
                }
            }
        }
        break;
    case ULDATAIND:
        {
            SULDataIndication sULDataIndication = {0};
            STfuDatIndInfo* psTfuDatIndInfo = NULL;
            if(DecodeULDataIndication(rxBuffer, &sULDataIndication) > 0)
            {
                LOG_DEBUG(ELOG_MESSAGE, "Successfully decoded UL Data Indication");
                psTfuDatIndInfo = &(sULDataIndication.m_sTfuDatIndInfo);
                PW_PALSendPhyRxStartIndication(s_ucSubframe);
                PW_PALSendPhyRxSduIndicationForULSCH(s_usSFN, s_ucSubframe,
                    psTfuDatIndInfo->m_ppsTfuDatInfo[0]->m_usRNTI,
                    psTfuDatIndInfo->m_ppsTfuDatInfo[0]->m_uiDataPduLength,
                    psTfuDatIndInfo->m_ppsTfuDatInfo[0]->m_pucDataPduBuffer);
            }
        }
        break;
    case ULSRIND:
        {
            SULSRIndication sULSRIndication = {0};
            STfuSrIndInfo* psTfuSrIndInfo = NULL;
            if(DecodeULSRIndication(rxBuffer, &sULSRIndication) > 0)
            {
                LOG_DEBUG(ELOG_MESSAGE, "Successfully decoded UL SR Indication");
                psTfuSrIndInfo = &(sULSRIndication.m_sTfuSrIndInfo);
            }
        }
        break;
    case ULCRCIND:
        {
        }
        break;
    case ULCQIIND:
        {
        }
        break;
    default:
        break;
    }
    return PN_SUCCESS;
}

int FSMInitialize()
{
    int iRetVal = PN_FAILURE;
    unsigned char ucTimeToWait = 1;
    unsigned int uiTimerId = 1;
    int count;

    iRetVal = MQInitialize();
    if(iRetVal == PN_FAILURE)
    {
        LOG_ERROR(ELOG_MESSAGE, "Failed to initialize Message Queues");
        return PN_FAILURE;
    }

    if(SKInitializeSocketModule(s_arriSockFd) == PN_FAILURE)
    {
        LOG_ERROR(ELOG_MESSAGE, "Unable to initialize Socket Module");
        return PN_FAILURE;
    }

    _InitializeMap();

    for(count = 0; count < iUeCount; count++)
    {
        s_Map.iSockFd[count] = s_arriSockFd[count];
        s_Map.uiServicePort[count] = iServicePort + count;
    }

    iRetVal = TMInitTimer(ucTimeToWait, uiTimerId);
    if(iRetVal != PN_SUCCESS)
    {
        LOG_ERROR(ELOG_MESSAGE, "Failed to initialize Timer Module");
        return PN_FAILURE;
    }
    return iRetVal;
}

int FSMClose()
{
    SKCloseSocketModule(s_arriSockFd);
    TMCloseTimer();

    return PN_SUCCESS;
}

/* Handles DL, UL messages and Timer Tick */ 
void FSMExecute()
{
    struct timeval      timerTimeout, rxTimeout;
    fd_set              timerReadfdSet, rxReadfdSet;
    int                 timerReadyfds = 0, rxReadyfds = 0;
    int                 bytesRead = 0;
    unsigned char       rxBuffer[PW_MAX_MSG_LENGTH], dummyBuffer[16];
    PW_EMspdPhyPrimitive eMspdPhyPrimitive;
    int count;

    int iTimerMsgQueueFd = 0;    int iMsgLen = 0;
    timerTimeout.tv_sec = 0;
    timerTimeout.tv_usec = 10000;
    FD_ZERO(&timerReadfdSet);
    iTimerMsgQueueFd = MQGetQueueId(EPW_MSGQ_TYPE_TIMER);
    if(iTimerMsgQueueFd == -1)
    {
        LOG_ERROR(ELOG_MESSAGE, "Unable to get Timer Message QueueRead Fd");
    }
    FD_ZERO(&timerReadfdSet);
    FD_SET(iTimerMsgQueueFd, &timerReadfdSet);
    timerReadyfds = select(iTimerMsgQueueFd + 1, &timerReadfdSet,
        NULL, NULL, &timerTimeout);
    while (timerReadyfds > 0)
    {
        if (FD_ISSET(iTimerMsgQueueFd, &timerReadfdSet))
        {
            iMsgLen = UTILRetrieveMessageFromMessageQueue(iTimerMsgQueueFd,
                dummyBuffer, PW_MAX_MSG_LENGTH, 0);
            if (iMsgLen > 0)
            {
                /*Send TX Start Indication Primitive and buffer to UE, if any*/
                LOG_DEVEL(ELOG_MESSAGE, "Received Timer Tick");
                PW_PALSendPhyTxStartIndication(s_usSFN, s_ucSubframe);

                if(s_txBufLen > 0)
                {
                    for(count = 0; count < iUeCount; count++)
                    {
                        if(s_Map.iSockFd[count] != -1 &&
                            s_Map.bTxDataPresent[count] == PN_TRUE)
                        {
                            s_Map.bTxDataPresent[count] = PN_FALSE;
                            if(SKWriteOverSocket(s_txBuffer, s_txBufLen,
                                s_Map.iSockFd[count], s_Map.uiServicePort[count]) == PN_SUCCESS)
                            {
                                LOG_DEVEL(ELOG_MESSAGE, "Sent message to UE");
                            }
                            s_txBufLen = -1;
                            break;
                        }

                        if(s_Map.iSockFd[count] != -1 && s_Map.bIsBroadcast == PN_TRUE)
                        {
                            if(SKWriteOverSocket(s_txBuffer, s_txBufLen,
                                s_Map.iSockFd[count], s_Map.uiServicePort[count]) == PN_SUCCESS)
                            {
                                LOG_DEVEL(ELOG_MESSAGE, "Sent message to UE");
                            }
                            if(count == iUeCount - 1)
                            {
                                s_txBufLen = -1;
                                s_Map.bIsBroadcast = PN_FALSE;
                            }
                        }
                    }
                }
                /*if(s_txBufLen > 0)
                {
                    SKWriteOverSocket(s_txBuffer, s_txBufLen,
                                s_Map.iSockFd[0], s_Map.uiServicePort[0]);
                    LOG_DEBUG(ELOG_MESSAGE, "Sent message to UE");
                    s_txBufLen = -1;
                }*/
                /*Read Mindspeed primitive received from queue and receive primitive type*/
                _FSMProcessPHYPrimitive(eMspdPhyPrimitive);

                /*Read PHY messages from UEs*/
                rxTimeout.tv_sec = 0;
                rxTimeout.tv_usec = 1000; /*wait 1 ms for UE packet*/
                FD_ZERO(&rxReadfdSet);
                for(count = 0; count < iUeCount; count++)
                {
                    FD_SET(s_arriSockFd[count], &rxReadfdSet);
                }

                rxReadyfds = select(s_arriSockFd[iUeCount - 1] + 1,
                    &rxReadfdSet, NULL, NULL, &rxTimeout);

                while (rxReadyfds > 0)
                {
                    for(count = 0; count < iUeCount; count++)
                    {
                        if (FD_ISSET(s_arriSockFd[count], &rxReadfdSet))
                        {
                            if(bytesRead = SKReadFromSocket(((char*)rxBuffer), s_arriSockFd[count]) > 0)
                            {
                                _ProcessUEPhyMessage(bytesRead, rxBuffer, s_arriSockFd[count]);
                            }
                        }

                        rxReadyfds--;
                    }
                }

                _UpdateTTIParams();
            }
        }

        timerReadyfds--;
    }

    return;
}

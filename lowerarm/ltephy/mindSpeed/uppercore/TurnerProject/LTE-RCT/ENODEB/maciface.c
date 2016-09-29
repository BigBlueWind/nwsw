/** @file maciface.c
 *
 * @brief Interface for LTE eNodeB
 * @author Mindspeed Technologies
 * @version $Revision: 1.46 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef ENODEB

#include "maciface.h"
#include "apidefs.h"
#include "apihdlr.h"
#include "bsmac.h"
#include "msgroute.h"
#include "phyiface.h"
#include "diags.h"

extern RESULTCODE LteMacProcessFromPhyMessage(PGENMSGDESC pMsgHdr,
                                              PDLSUBFRDESC pDlSubFrame,
                                              PULSUBFRDESC pUlSubFrame,
                                              PTR pUe);
extern RESULTCODE LteMacRxEthPacket(PTR PacketPtr, U32 Len);
extern OSAL_QUEUE PhyQueue;
extern OSAL_SEMAPHORE PhyRxSema;
extern OSAL_HEAP OsalGlobalHeap;

static MAC_INIT_CONFIG macInitConfig = { 0 };

/** Set MAC Config
 *
 * @param heap
 * @return Standard result code
 */
RESULTCODE MacSetInitConfig(HANDLE h)
{
    INITPARM *pPhyParm = &macInitConfig.phyInitParam;
    RESULTCODE ret = SUCCESS;
    PMSGHEADER pHdr = (PMSGHEADER) h;
    PPARAMHEADER pParam = MsgGetNextParam(pHdr, NULL);

    while (pParam)
    {
        switch (pParam->paramID)
        {
            case 0:                 // End of list
                pParam = NULL;      // Stop
                continue;
            case PAR_ASSOCID:
                macInitConfig.phyInstanceId = pParam->data[0];
                break;
            case PAR_CREATE_CONFIG:
                macInitConfig.macMode = pParam->data[0];
                break;
            case PAR_CHANNELBANDWIDTH:
                pPhyParm->channelBandwidth = (U8)pParam->data[0];
                break;
            case PAR_FRTYPEDUPLEXMODE:
                pPhyParm->frtypeDuplexmode = (U8)pParam->data[0];
                break;
            case PAR_RADIOACCESSMODE:
                pPhyParm->radioAccessMode = (U8)pParam->data[0];
                break;
            case PAR_PHYSICALRESOURCEBANDWIDTH:
                pPhyParm->physicalResourceBandwidth = (U8)pParam->data[0];
                break;
            case PAR_NUMBERRESOURCEBLOCKSPERSLOT:
                pPhyParm->numberResourceBlocksperSlot = (U8)pParam->data[0];
                break;
            case PAR_PHYLAYERCELLIDGROUP:
                pPhyParm->phyLayerCellIdGroup = (U8)pParam->data[0];
                break;
            case PAR_PHYLAYERID:
                pPhyParm->phyLayerId = (U8)pParam->data[0];
                break;
            case PAR_TXANTENNAPORTCOUNT:
                pPhyParm->txAntennaPortCount = (U8)pParam->data[0];
                break;
            case PAR_FASTFWDORPRECLOCKINGNUMBER:
                pPhyParm->fastfwdorPreclockingNumber = (U16)pParam->data[0];
                break;
            case PAR_FFTSIZE:
                pPhyParm->fftSize = (U16)pParam->data[0];
                break;
            case PAR_NUMBERUSEDSUBCARRIERS:
                pPhyParm->numberUsedSubcarriers = (U16)pParam->data[0];
                break;
            case PAR_NMAXDLRB:
                pPhyParm->nMaxDlRb = (U8)pParam->data[0];
                break;
            case PAR_NMAXULRB:
                pPhyParm->nMaxUlRb = (U8)pParam->data[0];
                break;
            case PAR_REFERENCESIGNALPOWER:
                pPhyParm->referenceSignalPower = (S16)pParam->data[0];
                break;
            case PAR_PRIMARYSYNCSIGNALPOWER:
                pPhyParm->primarySyncSignalPower = (S16)pParam->data[0];
                break;
            case PAR_SECONDARYSYNCSIGNALPOWER:
                pPhyParm->secondarySyncSignalPower = (S16)pParam->data[0];
                break;
            case PAR_NUMDATAREPERPRB:
                pPhyParm->numDataRePerPRB = (U8)pParam->data[0];
                break;
            case PAR_CYCLICPREFIXTYPE:
                pPhyParm->cyclicPrefixType = (U8)pParam->data[0];
                break;
            case PAR_RXANTENNAPORTCOUNT:
                pPhyParm->rxAntennaPortCount = (U8)pParam->data[0];
                break;
            case PAR_TXSDUCONFENABLE:
                pPhyParm->txSduConfEnable = (U8)pParam->data[0];
                break;
            case PAR_RADIOFRAMENUMBER:
                pPhyParm->radioFrameNumber = (U16)pParam->data[0];
                break;
            case PAR_SUBFRAMENUMBER:
                pPhyParm->subframeNumber = (U8)pParam->data[0];
                break;
            case PAR_SLOTNUMBER:
                pPhyParm->slotNumber = (U8)pParam->data[0];
                break;
            case PAR_SRSBANDWIDTHCONFIG:
                pPhyParm->srsBandwidthConfig = (S16)pParam->data[0];
                break;
            case PAR_SRSSUBFRAMECONFIG:
                pPhyParm->srsSubframeConfig = (S16)pParam->data[0];
                break;
            case PAR_SRSSIMULTANEOUSANANDSRS:
                pPhyParm->srsSimultaneousANandSRS = (S16)pParam->data[0];
                break;
            case PAR_PRACHCONFIGURATIONINDEX:
                pPhyParm->prachConfigurationIndex = (S16)pParam->data[0];
                break;
            case PAR_PRACHFREQUENCYOFFSET:
                pPhyParm->prachFrequencyOffset = (S16)pParam->data[0];
                break;
            case PAR_PRACHHIGHSPEEDFLAG:
                pPhyParm->prachHighSpeedFlag = (S16)pParam->data[0];
                break;
            case PAR_PRACHCYCLICSHIFTCONFIG:
                pPhyParm->prachCyclicShiftConfig = (S16)pParam->data[0];
                break;
            case PAR_PRACHROOTSEQUENCEINDEX:
                pPhyParm->prachRootSequenceIndex = (S16)pParam->data[0];
                break;
            default:                // Unknown parameter received
                pParam = NULL;      // Stop
                ret = RC_LTEMAC_UNKNOWN_PARAM;
                continue;
        }
        pParam = MsgGetNextParam(pHdr, pParam);
    }

    return ret;
}

/** Initialize MAC
 *
 * @param heap
 * @return Standard result code
 */
RESULTCODE MacInit(PhyTxCb txCb, PhyRxCb rxCb, HANDLE toPhy, HANDLE fromPhy)
{
    OsalCreateSema(&PhyRxSema);
    return LteMacInit(OsalGlobalHeap, MSGS_MAC, &macInitConfig, txCb, rxCb, toPhy, fromPhy);
}

/** Stop MAC
 *
 * @return
 */
RESULTCODE MacDone(void)
{
    RESULTCODE ret = SUCCESS;

    ret = LteMacDestroy();
    return ret;
}

RESULTCODE AppInit(OSAL_HEAP *heap)
{
    // FIXME: This is dirty hack
    LteMacCreateLogicChannel(MacCreateUE(), DTCH, LCID_MIN_LCHAN);
    return SUCCESS;
}

/** Dispatch MAC messages
 *
 * @param size Message size
 * @param msg Pointer to the message
 * @return Standard result code
 */
RESULTCODE MacDispatch(U32 size, PTR msg)
{
    PMSGHEADER pMsg = (PMSGHEADER) msg;
    PMSGHEADER pRsp = NULL;
    U32 ret = SUCCESS;

    if (pMsg->control & CTRL_BYPASS)
    {
        // This is PHY API message
#ifndef MAC_RT
        if (OsalEnqueue(&PhyQueue, pMsg) == SUCCESS)
        {
            OsalRaiseSema(&PhyRxSema);
            return SUCCESS;
        }
        else
        {
            MsgFreeBuffer(pMsg);
            return FAILURE;
        }
#else
        PGENMSGDESC pPhyMsg;
        pPhyMsg = MsgGetDataOffset(msg);
        // Only support 1 UE currently
        LteMacProcessFromPhyMessage(pPhyMsg, &DlSubFrame, &UlSubFrame, pBsMacDesc->pUe[0]);
        MsgFreeBuffer(pMsg);
        return SUCCESS;
#endif
    }
    else if (pMsg->type == MSGT_CONFIG)
    {
        pRsp = MsgInitReply(pMsg);
        if (pRsp)
        {
            switch (pMsg->msgID)
            {
                case MAC_TEST_CONFIG:
                    ret = MacApiConfigTestConfig(pMsg, pRsp);
                    break;
                case MAC_CONFIG_CELL:
                    ret = MacApiConfigRrc(pMsg, pRsp);
                    break;
                case MAC_RECONFIG_CELL:
                    ret = MacApiConfigRrc(pMsg, pRsp);
                    break;
                case MAC_CREATE_UE_ENTITY:
                    ret = MacApiConfigRrc(pMsg, pRsp);
                    break;
                case MAC_RECONFIGURE_UE_ENTITY:
                    ret = MacApiConfigRrc(pMsg, pRsp);
                    break;
                case PHY_LTE_START:
                case PHY_LTE_STOP:
                    ret = MacApiConfig(pMsg, pRsp);
                    break; 
                default:
                    ret = RC_LTEMAC_UNKNOWN_COMMAND;
                    break;
            }
        }
    }
    else if (pMsg->type == MSGT_QUERY)
    {
        if ((pRsp = MsgInitReply(pMsg)) != NULL)
        {
            switch (pMsg->msgID)
            {
                case MAC_TEST_CONFIG:
                    ret = MacApiQueryTestConfig(pMsg, pRsp);
                    break;
                case MAC_CONFIG_CELL:
                    ret = MacApiQueryRrc(pMsg, pRsp);
                    break;
                case MAC_RECONFIG_CELL:
                    ret = MacApiQueryRrc(pMsg, pRsp);
                    break;
                case MAC_CREATE_UE_ENTITY:
                    ret = MacApiQueryRrc(pMsg, pRsp);
                    break;
                case MAC_RECONFIGURE_UE_ENTITY:
                    ret = MacApiQueryRrc(pMsg, pRsp);
                    break;
                default:
                    ret = RC_LTEMAC_UNKNOWN_COMMAND;
                    break;
            }
        }
    }
    else
    {
        ret = RC_LTEMAC_UNKNOWN_COMMAND;
    }

    // if no ACK suppression
    if (pRsp && ((pMsg->control & CTRL_ACKSUPPRESS) == 0))
    {
        MsgResult(pRsp, ret);
        ret = MsgDispatch(MSGS_MAC, pRsp->length + sizeof(MSGHEADER), pRsp);
    }
    else
    {
        // TODO: Replace with top-level function
        OsalFreeMemArray(&MsgArray, pRsp);
    }

    MsgFreeBuffer(pMsg);

    return SUCCESS;
}

RESULTCODE MacRxEthPacket(PTR pPacket, U32 Len)
{
    return LteMacRxEthPacket(pPacket, Len);
}

#endif /* ENODEB */


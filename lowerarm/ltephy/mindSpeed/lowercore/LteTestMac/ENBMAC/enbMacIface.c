/** @file enbMacIface.c
 *
 * @brief Interface for LTE eNodeB MAC integrated in 4GMX OS (EmbeddedMAC)
 * @author Mindspeed Technologies
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "enbMacIface.h"
#include "apimgr.h"
#include "enbApiHdlr.h"
#include "bsmac.h"
#include "ltemaccore.h"
#include "enbMac2PhyIface.h"
#include "enbApiHdlr.h"
#include "supervisor.h"

/*
 * Local variables
 */
static MAC_INIT_CONFIG macInitConfig = { 0 };

int get_mac_console_vals_num(void)
{
    return macInitConfig.ConsoleValsNum;
}

int get_mac_console_vals(U32 idx)
{
    return macInitConfig.ConsoleVals[idx];
}


/** @brief Check DrainRxSduBuf mode on/off
 *
 * @return TRUE for ON, FALSE otherwise
 */
int MacDrainRxSduModeOn(void)
{
    if (macInitConfig.DrainRxSduModeOn)
        return 1;
    return 0;
}

/** Checks if MAC was configured for RCT tests
 *
 * @return TRUE if configured for RCT tests, FALSE otherwise
 */
int MacExtTestMode(void)
{
    if (macInitConfig.macConfMode == MACCONF_RCT)
        return 1;
    return 0;
}

/** Returns ID of the PHY associated with this MAC instance
 *
 * @return PHY ID
 */
int MacGetPhyInstanceId(void)
{
    return macInitConfig.phyInstanceId;
}

/** Returns FFT size configured for current session
 *
 * @return FFT size (as defined in enum FftSizeOptions)
 */
int MacGetNfft(void)
{
    return macInitConfig.phyInitParam.fftSize;
}

/** Returns receive antenna port count as configured for current session
 *
 * @return Receive antenna port count (as defined in enum RxAntPortCntOptions)
 */
int MacGetNantRx(void)
{
    return macInitConfig.phyInitParam.rxAntennaPortCount;
}

/** Returns transmit antenna port count as configured for current session
 *
 * @return Transmit antenna port count (as defined in enum TxAntPortCntOptions)
 */
int MacGetNantTx(void)
{
    return macInitConfig.phyInitParam.txAntennaPortCount;
}

/** Sends PHY_INIT.req message to the PHY instance with the given mode of operation.
 *
 * @param mode Defines API compatibility support:
 *             local phyCfg is used here
 * @return Standard result code
 */
RESULTCODE MacCreatePhy(U32* pMode)
{
    return LteMacSendToPhyMessage(PHY_INIT_REQ, pMode, 1);
}

/** Sends PHY_RECONFIG.req message to the PHY instance with the given mode of operation.
 *
 * @param mode Defines API compatibility support:
 *             local phyCfg is used here
 * @return Standard result code
 */
RESULTCODE MacReconfigPhy(U32* pMode)
{
    return LteMacSendToPhyMessage(PHY_RECONFIG_REQ, pMode, 1);
}

/** Sends NMM_START, NMM_RSSI_MEAS_REQ, NMM_CELL_SEARCH_REQ, NMM_PBCH_CONFIG_REQ, 
 * NMM_SIB1_CONFIG_REQ messages to the NMM instance with the given mode of operation.
 *
 * @param mode Defines API compatibility support:
 * @return Standard result code
 */
RESULTCODE MacNMMStart(U32 mode)
{
    RESULTCODE rc = MX_OK;

    rc |= LteMacSendToPhyNmmMessageTest(NMM_START, &mode, 1);

    return rc;
}

/** Sends NMM_STOP message to the NMM instance.
 *
 * @param mode Defines API compatibility support:
 * @return Standard result code
 */
RESULTCODE MacNMMStop(U32 mode)
{
    RESULTCODE rc = MX_OK;

    rc |= LteMacSendToPhyNmmMessageTest(NMM_STOP, &mode, 1);

    return rc;
}

/** Sends different NMM messages to the NMM instance depending on given mode.
 *
 * @param mode Defines API compatibility support:
 * @return Standard result code
 */
RESULTCODE MacNMMCmd(U32 mode)
{
	RESULTCODE rc = MX_OK;

	rc |= LteMacSendToPhyNmmMessageTest(NMM_CMD, &mode, 1);

    return rc;
}



/** Processes MAC configuration parameters specified in 'createmac' console command
 *
 * @param pVals pointer to console paramaters values
 * @param num number of console paramaters values
 * @return Standard result code
 */
RESULTCODE MacProcConsoleParams(U32* pVals, U32 num)
{
    U32 Idx = 0;

    macInitConfig.configId = pVals[Idx];
    // To make clear what's actually used in this case
    macInitConfig.macConfMode = MACCONF_FDX0;

    macInitConfig.ConsoleValsNum = num;

    while(num--)
    {
        macInitConfig.ConsoleVals[Idx] = pVals[Idx];
        Idx++;
    }

    return SUCCESS;
}

/** Handles configuration part of SVSR_CREATEMAC command
 *  Fills internal INITPARM structure
 *
 * @param h Pointer to the MSPD API header (MSA-E-81479, defined as struct _API_HEADER_ or
 *          struct MsgHeader)
 * @return Standard result code
 */
RESULTCODE MacSetInitConfig(PTR h)
{
    INITPARM *pPhyParm = &macInitConfig.phyInitParam;
    RESULTCODE ret = SUCCESS;
    ApiHeader *pHdr = (ApiHeader *) h;
    ApiParam *pParam = ApiGetNextParam(pHdr, NULL);

    memset(&macInitConfig, 0, sizeof(macInitConfig));

    while (pParam)
    {
        switch (pParam->ParamID)
        {
            case 0:                     // End of list
                pParam = NULL;          // Stop processing
                continue;
            case PAR_ASSOCID:
                macInitConfig.phyInstanceId = pParam->ParamData[0] & 0xFFFF;
                break;
            case PAR_CONFIG_ID:
                macInitConfig.configId = pParam->ParamData[0];
                break;
            case PAR_CREATE_CONFIG:
                macInitConfig.macConfMode = pParam->ParamData[0];
                break;
            case PAR_DRAIN_RX_SDU:
                macInitConfig.DrainRxSduModeOn = pParam->ParamData[0];
                break;
            case PAR_CHANNELBANDWIDTH:
                pPhyParm->channelBandwidth = (U8) pParam->ParamData[0];
                break;
            case PAR_FRTYPEDUPLEXMODE:
                pPhyParm->frtypeDuplexmode = (U8) pParam->ParamData[0];
                break;
            case PAR_RADIOACCESSMODE:
                pPhyParm->radioAccessMode = (U8) pParam->ParamData[0];
                break;
            case PAR_PHYSICALRESOURCEBANDWIDTH:
                pPhyParm->physicalResourceBandwidth = (U8) pParam->ParamData[0];
                break;
            case PAR_NUMBERRESOURCEBLOCKSPERSLOT:
                pPhyParm->numberResourceBlocksperSlot = (U8) pParam->ParamData[0];
                break;
            case PAR_PHYLAYERCELLIDGROUP:
                pPhyParm->phyLayerCellIdGroup = (U8) pParam->ParamData[0];
                break;
            case PAR_PHYLAYERID:
                pPhyParm->phyLayerId = (U8) pParam->ParamData[0];
                break;
            case PAR_TXANTENNAPORTCOUNT:
                pPhyParm->txAntennaPortCount = (U8) pParam->ParamData[0];
                break;
            case PAR_FASTFWDORPRECLOCKINGNUMBER:
                pPhyParm->fastfwdorPreclockingNumber = (U16) pParam->ParamData[0];
                break;
            case PAR_FFTSIZE:
                pPhyParm->fftSize = (U16) pParam->ParamData[0];
                break;
            case PAR_NUMBERUSEDSUBCARRIERS:
                pPhyParm->numberUsedSubcarriers = (U16) pParam->ParamData[0];
                break;
            case PAR_NMAXDLRB:
                pPhyParm->nMaxDlRb = (U8) pParam->ParamData[0];
                break;
            case PAR_NMAXULRB:
                pPhyParm->nMaxUlRb = (U8) pParam->ParamData[0];
                break;
            case PAR_REFERENCESIGNALPOWER:
                pPhyParm->referenceSignalPower = (S16) pParam->ParamData[0];
                break;
            case PAR_PRIMARYSYNCSIGNALPOWER:
                pPhyParm->primarySyncSignalPower = (S16) pParam->ParamData[0];
                break;
            case PAR_SECONDARYSYNCSIGNALPOWER:
                pPhyParm->secondarySyncSignalPower = (S16) pParam->ParamData[0];
                break;
            case PAR_NUMDATAREPERPRB:
                pPhyParm->numDataRePerPRB = (U8) pParam->ParamData[0];
                break;
            case PAR_CYCLICPREFIXTYPE:
                pPhyParm->cyclicPrefixType = (U8) pParam->ParamData[0];
                break;
            case PAR_RXANTENNAPORTCOUNT:
                pPhyParm->rxAntennaPortCount = (U8) pParam->ParamData[0];
                break;
            case PAR_TXSDUCONFENABLE:
                pPhyParm->txSduConfEnable = (U8) pParam->ParamData[0];
                break;
            case PAR_RADIOFRAMENUMBER:
                pPhyParm->radioFrameNumber = (U16) pParam->ParamData[0];
                break;
            case PAR_SUBFRAMENUMBER:
                pPhyParm->subframeNumber = (U8) pParam->ParamData[0];
                break;
            case PAR_SLOTNUMBER:
                pPhyParm->slotNumber = (U8) pParam->ParamData[0];
                break;
            case PAR_SRSBANDWIDTHCONFIG:
                pPhyParm->srsBandwidthConfig = (S16) pParam->ParamData[0];
                break;
            case PAR_SRSSUBFRAMECONFIG:
                pPhyParm->srsSubframeConfig = (S16) pParam->ParamData[0];
                break;
            case PAR_SRSSIMULTANEOUSANANDSRS:
                pPhyParm->srsSimultaneousANandSRS = (S16) pParam->ParamData[0];
                break;
            case PAR_PRACHCONFIGURATIONINDEX:
                pPhyParm->prachConfigurationIndex = (S16) pParam->ParamData[0];
                break;
            case PAR_PRACHFREQUENCYOFFSET:
                pPhyParm->prachFrequencyOffset = (S16) pParam->ParamData[0];
                break;
            case PAR_PRACHHIGHSPEEDFLAG:
                pPhyParm->prachHighSpeedFlag = (S16) pParam->ParamData[0];
                break;
            case PAR_PRACHCYCLICSHIFTCONFIG:
                pPhyParm->prachCyclicShiftConfig = (S16) pParam->ParamData[0];
                break;
            case PAR_PRACHROOTSEQUENCEINDEX:
                pPhyParm->prachRootSequenceIndex = (S16) pParam->ParamData[0];
                break;
            case PAR_RXSDULINEMONINFOENA:
                pPhyParm->rxSduLineMonitorInfoenable = (U8) pParam->ParamData[0];
                break;
#ifndef AC_MSPD_TDD
            case PAR_PHYCFGBITS:
                pPhyParm->phyCfg = pParam->ParamData[0] & 0x3FF;
                break;
#endif

            case PAR_P_B:
                pPhyParm->pb = pParam->ParamData[0] & 0x3;
                break;

            case PAR_PHICH_RESOURCE:
                pPhyParm->phichResource= pParam->ParamData[0] & 0xFFFF;
                break;

            case PAR_PHICH_DURATION:
                pPhyParm->phichDuration= pParam->ParamData[0] & 0xFFFF;
                break;

            default:                    // Unknown parameter received
                uart_printf("MacSetInitConfig: unknown ParamID=0x%X ParamLength=%d\r\n", pParam->ParamID, pParam->ParamLength);
                ret = RC_LTEMAC_UNKNOWN_PARAM;
                break;
        }
        pParam = ApiGetNextParam(pHdr, pParam);
    }
    return ret;
}

/** Top-level MAC initialization function which should be called from application module
 *
 * @param txCb Pointer to the callback function which will send API messages to PHY
 * @param rxCb Pointer to the callback function which will receive API messages from PHY
 * @param toPhy Application-specific handle associated with the txCb callback
 * @param fromPhy Application-specific handle associated with the rxCb callback
 * @return Standard result code
 */
RESULTCODE MacInit(PhyTxCb txCb, PhyRxCb rxCb, LPVOID toPhy, LPVOID fromPhy)
{
    return eNbMacInit(IID_LTE_MAC, &macInitConfig, txCb, rxCb, toPhy, fromPhy);
}

/** Stop MAC operation and destroy MAC instance

 * @return Standard result code
 */
RESULTCODE MacDone(void)
{
    RESULTCODE ret = SUCCESS;

    ret = eNbMacDestroy();
    return ret;
}

/** Dispatch MAC messages
 *  This dispatcher should be called from Supervisor level when Destination ID set to 'MAC'
 *
 * @param size Message size in bytes
 * @param msg Pointer to the message header (MSA-E-81479)
 * @return Standard result code
 */
RESULTCODE MacDispatch(U32 size, PTR msg)
{
    U32 ret = SUCCESS;

    return ret;
}


/*UMTSNMM*/
RESULTCODE MacUmtsNMMCmd(U32 mode)
{
	RESULTCODE rc = MX_OK;

	rc |= LteMacSendToPhyMessage(UMTSNMM_CMD, &mode, 1);

    return rc;
}

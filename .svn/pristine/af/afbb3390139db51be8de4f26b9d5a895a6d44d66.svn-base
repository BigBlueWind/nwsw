/** @file apihdr.c
 *
 * @brief API Handler for LTE-RCT (common for eNodeB and UE)
 * @author Mindspeed Technologies
 * @version $Revision: 1.108.12.1 $
 *
 * COPYRIGHT(c) 2008-2010 Mindspeed Technologies
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
#if defined (BSMAC) || defined (ENODEB)
#include <math.h>
#include "osal.h"
#include "apidefs.h"
#include "ctrlmsg.h"
#include "ltemaccore.h"
#include "rrc_types.h"
#include "bsmac.h"
#include "LteMacTestCfgMgr.h"

#include "lte_mac_state.h"

extern BS_CONTEXT *pBsMacDesc;

static U32* GetParHeader(U16* msg, U16* LenParam, U32* InBuf, U32* Len)
{
    if (*Len)
        *Len -= sizeof(U32);
    else
        return  NULL;
    *msg = *(U16*)InBuf;
    *LenParam = *((U16*)InBuf+1) - sizeof(U32);
    InBuf++;
    return InBuf;
}

static U32* GetData(U32* Data, U32* InBuf, U32* Len)
{
    *Data = *InBuf;
    if(*Len)
        *Len -= sizeof(U32);
    else
        return NULL;
    InBuf++;
    return InBuf;
}

/**
 *
 * @param pMsg
 * @param pResp
 * @return
 */
RESULTCODE MacApiQueryRrc(PMSGHEADER pMsg, PMSGHEADER pResp)
{
    return SUCCESS;
}

/**
 *
 * @param pMsg
 * @param pResp
 * @return
 */
RESULTCODE MacApiConfigRrc(PMSGHEADER pMsg, PMSGHEADER pResp)
{
    U32* data_param = &pMsg->param[0];
    U32 Len = pMsg->length - sizeof(MSGHEADER);
    U32 UeId = 0;
    U32 SrbId;
    U16 msgID = pMsg->msgID;
    U16 LenParam = pMsg->length;
    while(Len)
    {
        switch(msgID)
        {
            case MAC_CREATE_UE_ENTITY:
                break;
            case MAC_RECONFIGURE_UE_ENTITY:
            {
                U32 UeRnti = 0;
                data_param = GetData(&UeRnti, data_param, &Len);
                UeId = 0;//GetUeId(UeRnti);

            }
            break;
            case PAR_RECONFIGURE_LC:
                data_param = GetData(&SrbId, data_param, &Len);
                break;
            case PAR_UL_LC_RECONFIG:
                break;
            case PAR_DL_LC_RECONFIG:
                break;
            case PAR_DL_LC_RECONFIG_TEST:
            {
                U32 SrbIdNew;
                U32 ChType;
                U32 TestNone;
                if (LenParam <= sizeof(U32))
                {
                    data_param = GetData(&TestNone, data_param, &Len);
                    if(TestNone == TRUE)
                    {
                            SetTestLogicalChannelId_Type(UeId,SrbId,SrbId);
                    }
                }
                else
                {
                    data_param = GetData(&SrbIdNew, data_param, &Len);
                    data_param = GetData(&ChType, data_param, &Len);
                    SetTestLogicalChannelId_Type(UeId, SrbId, SrbIdNew);
                }
            }
            break;
        }
        data_param = GetParHeader(&msgID,&LenParam, data_param, &Len);
        if (data_param == NULL)
            break;
    }

    return SUCCESS;
}

/** Handle MAC_TEST_CONFIG message
 *
 * @param pMsg Pointer to the message
 * @param pResp Pointer to the message
 * @return Standard result code
 */
RESULTCODE MacApiQueryTestConfig(PMSGHEADER pMsg, PMSGHEADER pResp)
{
    return SUCCESS;
}
static U8 SetDCIData(BS_CONFOR_TEST_CONFIG *pTestConfConfig, U8 *data_in, U32 size_in_bit, U32 idx)
{
    U16 PdcchRnti = (U16)(idx >> 16);
    U16 dci_idx = (U16)(idx & 0xFFFF);
    U32 size_in_byte = (size_in_bit + 7 ) >> 3;

    memcpy(pTestConfConfig->payloadPdcch[dci_idx], data_in, size_in_byte);
    pTestConfConfig->PdcchDataLen[dci_idx] = size_in_bit;
    pTestConfConfig->PdcchRnti[dci_idx] = PdcchRnti;
    pTestConfConfig->PdcchRntiEna = 1;
    return SUCCESS;
}

/** Handle MAC_TEST_CONFIG message
 *
 * @param pMsg Pointer to the message
 * @param pResp Pointer to the message
 * @return Standard result code
 */
RESULTCODE MacApiConfigTestConfig(PMSGHEADER pMsg, PMSGHEADER pResp)
{
    PBS_STATIC_CONFIG pStaticConfig = get_static_config();

    RESULTCODE ret = SUCCESS;
    PPARAMHEADER pParam = MsgGetNextParam(pMsg, NULL);
    BS_CONFOR_TEST_CONFIG testConfConfig = { 0 };
    U32 Idx = 0;
     //init params by default
    testConfConfig.prachConfigIndex = 0xFF;

    testConfConfig.SwpTst_MapIdx_1[0] = 0xFF;
    testConfConfig.SwpTst_MapIdx_2[0] = 0xFF;
    testConfConfig.phichDuration = PHICHDUR_NORMAL;
    testConfConfig.phichResource = PHICHRES_ONE_SIXTH;
    testConfConfig.phichIdx = 0xFF;
    testConfConfig.nsymi = 12;

    memset(testConfConfig.RctMcsUl, 0xFF, sizeof(testConfConfig.RctMcsUl));
    memset(testConfConfig.RctMcsDl, 0xFF, sizeof(testConfConfig.RctMcsDl));

    while (pParam)
    {
        switch (pParam->paramID)
        {
            case 0:                 // End of list
                pParam = NULL;      // Stop
                continue;
            case PAR_TEST_MODEL:
                testConfConfig.TestModel = (U8)pParam->data[0];
                break;
            case PAR_N_CW:
                testConfConfig.nCw = (U8)pParam->data[0];
                break;
            case PAR_N_LAYERS:
                testConfConfig.nLayers = (U8)pParam->data[0];
                break;
            case PAR_PRECODING_MODE:
                testConfConfig.precodingMode = (U8)pParam->data[0];
                break;
            case PAR_DURATION:
                testConfConfig.duration = (U32)pParam->data[0];
                break;
            case PAR_RS_BOOSTING:
                testConfConfig.rsBoosting = (U32)pParam->data[0];
                break;
            case PAR_EPRE_SINCHRO:
                testConfConfig.epreSinchro = (S32)pParam->data[0];
                break;
            case PAR_RESERVED_EPRE:
                testConfConfig.reservedEpre = (S32)pParam->data[0];
                break;
            case PAR_PBCH_EPRE:
                testConfConfig.pbchEpre = (U32)pParam->data[0];
                break;
            case PAR_PBCH_RESERVED_EPRE:
                testConfConfig.pbchReservedEpre = (S32)pParam->data[0];
                break;

// --------------------------------------------------------------------------------------------
            case PAR_RS_EPRE:
                pStaticConfig->dl_cmn_pwr_ctl.pilotPower = (S16)pParam->data[0];
                break;

            case PAR_PSS_RA:
                pStaticConfig->dl_cmn_pwr_ctl.psyncPower = (S16)pParam->data[0];
                break;

            case PAR_SSS_RA:
                pStaticConfig->dl_cmn_pwr_ctl.ssyncPower = (S16)pParam->data[0];
                break;

            case PAR_PCFICH_EPRE:
                pStaticConfig->dl_cmn_pwr_ctl.ciPower = (S16)pParam->data[0];
//                testConfConfig.pcfichEpre = (S32)pParam->data[0];
                break;

// --------------------------------------------------------------------------------------------
            case PAR_VRB_DISTRIBUTED:
                pStaticConfig->distributed_flag = pParam->data[0] & 1;
                pStaticConfig->n_gap_flag = (pParam->data[0]>>16) & 1;
                break;

// --------------------------------------------------------------------------------------------
            case PAR_PRS_SUPPORT:
                pStaticConfig->prs_num_rb = (pParam->data[0]) & 0xff;       // Bandwidth available for prs generation (Always smaller than system bandwidth (1,..100)    
                pStaticConfig->i_prs      = (pParam->data[0]>>8) & 0xffff; // PRS configuration Index for Table 6.10.4.3-1 (36.211)
                pStaticConfig->prs_enable = (pParam->data[0]>>31) & 0x1;    // 0 no positioning reference signals are generated in this subframe, 1 prs is generated in this subframe

                pStaticConfig->prs_ra     = (pParam->data[1]) & 0xffff;    // prs Signal Power ratio relative to Cell Specific reference signals power (db)
                break;

            case PAR_HARQ_SUPPORT:
                {
                    PLTE_PDSCH_HARQ_STATE       plte_pdsch_harq_state = get_lte_pdsch_harq_state();

                    plte_pdsch_harq_state->ena_harq = (pParam->data[0]) & 1;
                    // flag that it's programmed externally
                    testConfConfig.ena_harq = plte_pdsch_harq_state->ena_harq;
                }
                break;

            case PAR_CQI_SUPPORT:
                {
                    PLTE_PDSCH_CHAN_STATE       plte_pdsch_chan_state = get_lte_pdsch_chan_state();

                    plte_pdsch_chan_state->ena_dynamic_cqi_pmi_ri = pParam->data[0] & 1;
                    plte_pdsch_chan_state->sched_num_sfn_ri       = pParam->data[1];
                    plte_pdsch_chan_state->sched_num_sfn_cqi      = pParam->data[2];
                    // flag that it's programmed externally
                    testConfConfig.ena_dynamic_cqi_pmi_ri = plte_pdsch_chan_state->ena_dynamic_cqi_pmi_ri;
                }
                break;

            case PAR_BCH_DATA_SET:
                memcpy(&testConfConfig.payloadPbch, pParam->data, pParam->length);
                pBsMacDesc->isMIBconfigured = TRUE;
                break;
            case PAR_N_CTRL_SYMBOLS:
                testConfConfig.nCtrlSymbols = (S32)pParam->data[0];
                break;

            /* PHICH conf*/
            case PAR_N_PHICH_GROUP:
                testConfConfig.nPhichGroup = (U8)pParam->data[0];
                testConfConfig.PhichGroupSfn = testConfConfig.nPhichGroup;
                break;

            case PAR_N_PHICH_PER_GROUP:
                testConfConfig.nPhichPerGroup = (U8)pParam->data[0];
                break;
            case PAR_PHICH_RESOURCE:
                printf("!!! duplicating PAR_PHICH_RESOURCE\n");
                testConfConfig.phichResource = (U8)pParam->data[0];
                break;
            case PAR_PHICH_DURATION:
                printf("!!! duplicating PAR_PHICH_DURATION\n");
                testConfConfig.phichDuration = (U8)pParam->data[0];
                break;
             case PAR_PHICH_IDX:
                testConfConfig.phichIdx = (U8)pParam->data[0];
                break;

            case PAR_PHICH_SYM_POWER:
                testConfConfig.phichSymPower = (S32)pParam->data[0];
                break;
            case PAR_PHICH_GROUP_EPRE:
                testConfConfig.phichGroupEpre = (S32)pParam->data[0];
                break;
            case PAR_N_AVALABLE_REGS:
                testConfConfig.nAvalableRegs = (U8)pParam->data[0];
                break;
            case PAR_N_PDCCH:
                testConfConfig.nPdcch = (U8)pParam->data[0];
                break;
            case PAR_N_CCE_PER_PDCCH:
                testConfConfig.nCcePerPdcch = (U8)pParam->data[0];
                break;
            case PAR_N_REG_PER_CCE:
                testConfConfig.nRegPerCce = (U8)pParam->data[0];
                break;
            case PAR_N_REG_ALL_TO_PDCCH:
                testConfConfig.nRegAllToPdcch = (U8)pParam->data[0];
                break;
            case PAR_N_NIL_REG_PADDING:
                testConfConfig.nNilRegPadding = (U8)pParam->data[0];
                break;
            case PAR_PDCCH_REG_EPRE:
                testConfConfig.pdcchRegEpre = (S32)pParam->data[0];
                break;
            case PAR_NIL_REG_EPRE:
                testConfConfig.nilRegEpre = (S32)pParam->data[0];
                break;
    /*DCI*/				
            case PAR_DCI_SET:
                SetDCIData(&testConfConfig, (U8*)&pParam->data[2], (S32)pParam->data[1], (S32)pParam->data[0]);
                break;
				
    /* PDSCH */
            case PAR_N_PDSCH_BOOSTED_PRB:
                testConfConfig.nPdschBoostedPrb = (U8)pParam->data[0]; // NRF, to check if TDD using it
                testConfConfig.nPdschPrb[0][0]  = (U8)pParam->data[0]; //
                testConfConfig.nPdschPrb[1][0]  = (U8)pParam->data[0]; //

                break;
            case PAR_PDSCH_BOOSTED_PRB_PA:
                testConfConfig.pdschPrbPa[0][0]  = (S32)pParam->data[0]; //
                testConfConfig.pdschPrbPa[1][0]  = (S32)pParam->data[0]; //

                break;
            case PAR_PDSCH_BOOSTED_MODULATION:
                testConfConfig.pdschMod[0][0]  = (U8)pParam->data[0]; //
                testConfConfig.pdschMod[1][0]  = (U8)pParam->data[0]; //

                break;
            case PAR_N_PDSCH_DEBOOSTED_PRB:
                testConfConfig.nPdschDeBoostedPrb = (U8)pParam->data[0]; // NRF, to check if TDD using it
                testConfConfig.nPdschPrb[0][1]    = (U8)pParam->data[0]; //
                testConfConfig.nPdschPrb[1][1]    = (U8)pParam->data[0]; //
                break;
            case PAR_PDSCH_DEBOOSTED_PRB_PA:
                testConfConfig.pdschPrbPa[0][1]    = (S32)pParam->data[0]; //
                testConfConfig.pdschPrbPa[1][1]    = (S32)pParam->data[0]; //
                break;
            case PAR_PDSCH_DEBOOSTED_MODULATION:
                testConfConfig.pdschMod[0][1]    = (U8)pParam->data[0]; //
                testConfConfig.pdschMod[1][1]    = (U8)pParam->data[0]; //
                break;

    /* PUSCH */
            case PAR_PUSCH_BOOSTED_PRB:
                testConfConfig.nPuschBoostedPrb = (U8)pParam->data[0];
                break;
            case PAR_PUSCH_BOOSTED_PRB_PA:
                testConfConfig.puschBoostedPrbPa = (S32)pParam->data[0];
                break;
            case PAR_PUSCH_BOOSTED_MODULATION:
                testConfConfig.puschBoostedMod = (U8)pParam->data[0];
                break;
            case PAR_PUSCH_DEBOOSTED_PRB_PB:
                break;
            case PAR_PUSCH_DEBOOSTED_PRB_PA:
                break;
            case PAR_PUSCH_DEBOOSTED_MODULATION:
                break;
            case PAR_PUSCH_SCRINITVAL:
                memcpy(testConfConfig.rntiPusch, pParam->data, pParam->length);
                break;
            case PAR_PUSCH_DED_N_ACK:
                testConfConfig.puschDed.nACK = (U8)pParam->data[0] & 0xF;
                break;
            case PAR_PUSCH_DED_ACKIDX:
                testConfConfig.puschDed.betaOffsetACKIndex = (U8)pParam->data[0] & 0xF;
                break;
            
            case PAR_PDSCH_MCS_ARRAY_CW1:
                memcpy(testConfConfig.pdschMcs[0], pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.pdschMcs[0])));
                testConfConfig.pdschMcs_used[0] = 1;
                break;
            case PAR_PDSCH_MCS_ARRAY_CW2:
                memcpy(testConfConfig.pdschMcs[1], pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.pdschMcs[1])));
                testConfConfig.pdschMcs_used[1] = 1;
                break;

            case PAR_PUSCH_MCS_ARRAY:
                memcpy(testConfConfig.puschMcs, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.puschMcs)));
                testConfConfig.puschMcs_used = 1;
                break;

            case PAR_RNTI_BOOSTED:
                memcpy(testConfConfig.rntiBoosted, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.rntiBoosted)));
                break;
            case PAR_RNTI_DEBOOSTED:
                memcpy(testConfConfig.rntiDeBoosted, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.rntiDeBoosted)));
                break;

            case PAR_RXNUMBYTES:
                if(Idx < sizeof(testConfConfig.RxNumBytes) / sizeof(testConfConfig.RxNumBytes[0]))
                    testConfConfig.RxNumBytes[Idx++] =(U16)pParam->data[0];
                break;
            case PAR_DIRMODE:
                testConfConfig.DirMode =(U16)pParam->data[0];
                break;

            case PAR_CDDTYPE:
                testConfConfig.cddType = (U8)pParam->data[0];
                break;
            case PAR_CODEBOOKIDX:
                testConfConfig.codeBookIdx = (U8)pParam->data[0];
                break;
            case PAR_RV_DL:
                memcpy(testConfConfig.arRctRvDl, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.arRctRvDl)));
                break;

            case PAR_MCS_DL:
            	memcpy(testConfConfig.RctMcsDl, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.RctMcsDl)));
            	break;
            case PAR_RV:
            	memcpy(testConfConfig.arRctRv, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.arRctRv)));
            	break;
            case PAR_MCS_UL:
                memcpy(testConfConfig.RctMcsUl, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.RctMcsUl)));
                break;
            case PAR_TXMODE:
                testConfConfig.transmissionMode = (U8)pParam->data[0];
                break;

/*PUCCH*/				
            case PAR_PUCCH_DELTA_SHIFT:
                testConfConfig.ulSfrCtrl.deltaPUCCHShift = (U8)pParam->data[0];
                break;				
            case PAR_PUCCH_nRBCQI:
                testConfConfig.ulSfrCtrl.nRBCQI = (U8)pParam->data[0];
                break;				
            case PAR_PUCCH_nCSAN:
                testConfConfig.ulSfrCtrl.nCSAn = (U8)pParam->data[0];
                break;				
            case PAR_PUCCH_n1AN:
                testConfConfig.ulSfrCtrl.n1PucchAN = (U16)pParam->data[0];
                break;				
            case PAR_PUCCH_CYCLIC_SHIFT:
                testConfConfig.ulSfrCtrl.cyclicShift  = (U8)pParam->data[0];
                break;				
            case PAR_PUCCH_FORMAT:
                testConfConfig.ulSfrCtrl.uciformat = (U8)pParam->data[0];
                break;
            case PAR_UL_QAM64_ENA:
                testConfConfig.ulSfrCtrl.enable64QAM = (U8)pParam->data[0];
                break;
            case PAR_PUCCH_DLCQI_SIZE_BITS:
                testConfConfig.ulSfrCtrl.dlCqiPmiSizeBits = (U8)pParam->data[0];
	    		break;
				
	    case PAR_HOPPING_MODE:
		testConfConfig.ulSfrCtrl.hoppingMode = (U8) pParam->data[0];
                break;
            case PAR_PUSCH_HOPPING_OFFSET:
		testConfConfig.ulSfrCtrl.puschhoppingOffset = (U8) pParam->data[0];
		break;
            case PAR_GR_HOPPING_ENA:
                testConfConfig.ulSfrCtrl.groupHoppingEnabled = (U8) pParam->data[0];
                break;
            case PAR_GR_ASSIG_PUSCH:
                testConfConfig.ulSfrCtrl.groupAssignmentPUSCH = (U8) pParam->data[0];
                break;
            case PAR_SEQ_HOPPING_ENA:
                testConfConfig.ulSfrCtrl.sequenceHoppingEnabled = (U8) pParam->data[0];
                break;
            case PAR_nSB:
                testConfConfig.ulSfrCtrl.nSB= (U8) pParam->data[0];
                break;

            case PAR_PUSCH_NUM:
                memcpy(testConfConfig.numPusch, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.numPusch)));
                break;
            case PAR_PUCCH_NUM:
                memcpy(testConfConfig.numPucch, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.numPucch)));
                break;

            case PAR_SWEEP_DL_MAP_PDSCH1:
                memcpy(testConfConfig.SwpTst_MapIdx_1, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.SwpTst_MapIdx_1)));
                break;

            case PAR_SWEEP_DL_MAP_PDSCH2:
                memcpy(testConfConfig.SwpTst_MapIdx_2, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.SwpTst_MapIdx_2)));
                break;

/*PRACH*/
            case PAR_PRACH_ENA:
                break;
            case PAR_PRACH_HIGH_SPEED_FL:
                testConfConfig.highSpeedFlag = (U8)pParam->data[0];
                break;
            case PAR_PRACH_ZERO_CORR_ZONE_CFG:
                testConfConfig.zeroCorrelationZoneConfig = (U8)pParam->data[0];
                break;
            case PAR_PRACH_ROOT_SEQ_IDX:
                testConfConfig.rootSequenceIndex = (U32)pParam->data[0];
                break;
            case PAR_PRACH_CFG_IDX:
                testConfConfig.prachConfigIndex = (U8)pParam->data[0];
                break;
            case PAR_PRACH_FREQ_OFFSET:
                testConfConfig.prachFreqOffset = (U8)pParam->data[0];
                break;
            case PAR_PDSCH_PAYLOAD:
                testConfConfig.pdschPayload = (U8)pParam->data[0];
                testConfConfig.pdschPayloadEna = 1;
                break;

            case PAR_PDSCH:
                {
                    U32 pdsch_idx = (U8)pParam->data[0];
                
                    memset(&testConfConfig.PdschCfg[pdsch_idx], 0x00, sizeof(PDSCH_CFG));
                    memcpy(&testConfConfig.PdschCfg[pdsch_idx], ((U8*)pParam->data+1), pParam->length );
                
                    testConfConfig.PdschCfgEna = 1;
                    testConfConfig.nPdsch[0]++;  
                }
                break;
            case PAR_NMM_MODE:
                testConfConfig.nmmMode = (U8)pParam->data[0];
                break;

            default:                // Unknown parameter received
                printf("%s: received unknown parameter. pParam->paramID = 0x%x\n", __FUNCTION__, pParam->paramID);
                break;
        }
        pParam = MsgGetNextParam(pMsg, pParam);
    }

    /* Set 36.141 conformance test options */
    if (ret == SUCCESS)
    {
        if (TestMgrReconfigure(&testConfConfig) != SUCCESS)
        {
            ret = RC_LTEMAC_PARAM_ERROR;
        }
    }

    return ret;
}

RESULTCODE MacApiConfigTestRxVecCfg(PMSGHEADER pMsg, PMSGHEADER pResp)
{
    RESULTCODE ret = SUCCESS;
    PPARAMHEADER pParam = MsgGetNextParam(pMsg, NULL);
    BS_CONFOR_TEST_CONFIG testConfConfig = { 0 };
    U32 i;

    while (pParam)
    {
        switch (pParam->paramID)
        {
            case 0:                 // End of list
                pParam = NULL;      // Stop
                continue;
            case PAR_TIMADVERR_DFLT_PUCCH:
                memcpy(testConfConfig.timAdvErrDflt_pucch, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.timAdvErrDflt_pucch)));
                testConfConfig.timAdvErrDflt_pucch_len = 
                    MIN(pParam->length, sizeof(testConfConfig.timAdvErrDflt_pucch));
                break;
            case PAR_TIMADVERR_DFLT_PUSCH:
                memcpy(testConfConfig.timAdvErrDflt_pusch, pParam->data,
                    MIN(pParam->length, sizeof(testConfConfig.timAdvErrDflt_pusch)));
                testConfConfig.timAdvErrDflt_pusch_len = 
                    MIN(pParam->length, sizeof(testConfConfig.timAdvErrDflt_pusch));
                break;
            /* SRS */
            case PAR_SRS_BW_CFG:
                testConfConfig.srsBandwitdhConfig = (U8)pParam->data[0];
                break;
            case PAR_SRS_SF_CFG:
                testConfConfig.srsSubframeConfig = (U8)pParam->data[0];
                break;
            case PAR_SRSINFO_NUM:
                testConfConfig.srsNumber = (U8)pParam->data[0];
	        break;
            case PAR_SRSINFO_BW:
                for(i = 0; i < pParam->length; i++)
                    testConfConfig.srsInfo[i].srsBandwidth = *((U8*)pParam->data + i);
                break;
            case PAR_SRSINFO_HOPBW:
                for(i = 0; i < pParam->length; i++)
                    testConfConfig.srsInfo[i].srsHoppingBandwidth = *((U8*)pParam->data + i) & 3;
                break;
            case PAR_SRSINFO_FREQPOS:
                for(i = 0; i < pParam->length; i++)
                    testConfConfig.srsInfo[i].freqDomainPosition = *((U8*)pParam->data + i) & 0x1F;
                break;
            case PAR_SRSINFO_CFGIDX:
                for(i = 0; i < (U32)(pParam->length >> 1); i++)
                    testConfConfig.srsInfo[i].srsConfigIndex = *((U16*)pParam->data + i) & 0x3FF;
                break;
            case PAR_SRSINFO_CYCLSHFT:
                for(i = 0; i < pParam->length; i++)
                    testConfConfig.srsInfo[i].cyclicShift = *((U8*)pParam->data + i) & 0x7;
                break;
            case PAR_SRSINFO_DUR:
                for(i = 0; i < pParam->length; i++)
                    testConfConfig.srsInfo[i].duration = *((U8*)pParam->data + i) & 1;
                break;
            case PAR_SRSINFO_TXCOMB:
                for(i=0; i<pParam->length; i++)
                    testConfConfig.srsInfo[i].transmissionComb = *((U8*)pParam->data + i) & 1;
                break;

            case PAR_PUSCH_MCS_ARRAY:
                memcpy(testConfConfig.puschMcs, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.puschMcs)));
                testConfConfig.puschMcs_used = 1;
                break;
            case PAR_PUSCH_SCRINITVAL:
                memcpy(testConfConfig.rntiPusch, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.rntiPusch)));
                testConfConfig.rntiPusch_used = 1;
                break;
            case PAR_PUSCH_MAP:
                memcpy(testConfConfig.pusch_map, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.pusch_map)));
                testConfConfig.pusch_map_used = 1;
                break;

            case PAR_PUCCH_SCRINITVAL:
                memcpy(testConfConfig.rntiPucch, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.rntiPucch)));
                testConfConfig.rntiPucch_used = 1;
                break;

            case PAR_PUCCH_FORMAT:
                memcpy(testConfConfig.pucchFormat, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.pucchFormat)));
                testConfConfig.pucchFormat_used = 1;
                break;

            case PAR_PUCCH_MAP:
                memcpy(testConfConfig.pucch_map, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.pucch_map)));
                testConfConfig.pucch_map_used = 1;
                break;

          case PAR_PUCCH_DLCQI_SIZE_BITS:
              memcpy(testConfConfig.pucch_dlCqiPmiSizeBits, pParam->data, 
                  MIN(pParam->length, sizeof(testConfConfig.pucch_dlCqiPmiSizeBits)));
              testConfConfig.pucch_dlCqiPmiSizeBits_used = 1;
              break;

          case PAR_PUCCH_HARQ_SIZE_BITS:
              memcpy(testConfConfig.pucch_harqSizebits, pParam->data, 
                  MIN(pParam->length, sizeof(testConfConfig.pucch_harqSizebits)));
              testConfConfig.pucch_harqSizebits_used = 1;
              break;
#ifndef AC_MSPD_TDD
          case PAR_PUCCH_HARQ_IDX:
              memcpy(testConfConfig.pucch_harqPucchIndex, pParam->data, 
                  MIN(pParam->length, sizeof(testConfConfig.pucch_harqPucchIndex)));
              testConfConfig.pucch_harqPucchIndex_used= 1;
              break;
#else
          case PAR_PUCCH_IDX0:
              memcpy(testConfConfig.ackPUCCHResourceIndex0, pParam->data, 
                  MIN(pParam->length, sizeof(testConfConfig.ackPUCCHResourceIndex0)));
              testConfConfig.ackPUCCHResourceIndex[0]= 1;
              break;
          case PAR_PUCCH_IDX1:
              memcpy(testConfConfig.ackPUCCHResourceIndex1, pParam->data, 
                  MIN(pParam->length, sizeof(testConfConfig.ackPUCCHResourceIndex1)));
              testConfConfig.ackPUCCHResourceIndex[1]= 1;
              break;
          case PAR_PUCCH_IDX2:
              memcpy(testConfConfig.ackPUCCHResourceIndex2, pParam->data, 
                  MIN(pParam->length, sizeof(testConfConfig.ackPUCCHResourceIndex2)));
              testConfConfig.ackPUCCHResourceIndex[2]= 1;
              break;
          case PAR_PUCCH_IDX3:
              memcpy(testConfConfig.ackPUCCHResourceIndex3, pParam->data, 
                  MIN(pParam->length, sizeof(testConfConfig.ackPUCCHResourceIndex3)));
              testConfConfig.ackPUCCHResourceIndex[3]= 1;
              break;

          case PAR_ACK_NACK_REPORT_MODE:
                testConfConfig.ackNackReportMode = (U8)pParam->data[0];
                break;

          case PAR_NBUNDLED:
                testConfConfig.Nbundled = (U8)pParam->data[0];
                break;

#endif
          case PAR_PUCCH_CQI_RSRC_IDX:
              memcpy(testConfConfig.pucch_cqirsrcidx, pParam->data, 
                  MIN(pParam->length, sizeof(testConfConfig.pucch_cqirsrcidx)));
              testConfConfig.pucch_cqirsrcidx_used= 1;
              break;
	  case PAR_SRSSIMULTANEOUSANANDSRS:
	      testConfConfig.ackNackSRSSimultaneousTransmission = (U8)pParam->data[0];
	      break;
        /*
        * 	  PUSCH dedicated info configured
        */			
        case PAR_PUSCH_DED_RIIDX:
            memcpy(testConfConfig.PuschRiIdx, pParam->data, 
              MIN(pParam->length, sizeof(testConfConfig.PuschRiIdx)));
            testConfConfig.PuschRiIdxUsed = 1;
            break;
        
        case PAR_PUSCH_DED_CQIIDX:
            memcpy(testConfConfig.PuschCqiIdx, pParam->data, 
                MIN(pParam->length, sizeof(testConfConfig.PuschCqiIdx)));
            testConfConfig.PuschCqiIdxUsed = 1;
            break;
        
        case PAR_PUSCH_DED_N_nRI:
            testConfConfig.Pusch_nRiUsed = MIN(pParam->length, sizeof(testConfConfig.Pusch_nRi));
            memcpy(testConfConfig.Pusch_nRi, pParam->data, testConfConfig.Pusch_nRiUsed);
            break;
        
        case PAR_PUSCH_DED_N_nr1CQI:
            testConfConfig.Pusch_nrlCqiUsed = MIN(pParam->length, sizeof(testConfConfig.Pusch_nrlCqi));
            memcpy(testConfConfig.Pusch_nrlCqi, pParam->data, testConfConfig.Pusch_nrlCqiUsed);
            break;
        
        case PAR_PUSCH_DED_N_nrg1CQI:
            testConfConfig.Pusch_nrglCqiUsed = MIN(pParam->length, sizeof(testConfConfig.Pusch_nrglCqi));
            memcpy(testConfConfig.Pusch_nrglCqi, pParam->data, testConfConfig.Pusch_nrglCqiUsed);
            break;
        case PAR_PUSCH_DED_N_ACK:
            testConfConfig.Pusch_nAckUsed = MIN(pParam->length, sizeof(testConfConfig.Pusch_nAck));
            memcpy(testConfConfig.Pusch_nAck, pParam->data, testConfConfig.Pusch_nAckUsed);
            break;
        case PAR_PUSCH_DED_ACKIDX:
            memcpy(testConfConfig.PuschAckIdx, pParam->data, 
                MIN(pParam->length, sizeof(testConfConfig.PuschAckIdx)));
            testConfConfig.PuschAckUsed = 1;
            break;

        default:                // Unknown parameter received
            printf("%s: received unknown parameter. pParam->paramID = 0x%x\n", __FUNCTION__, pParam->paramID);
            break;
            }
            pParam = MsgGetNextParam(pMsg, pParam);
        }
        
        /* Set 36.141 conformance test options */
        if (ret == SUCCESS)
        {
            if (TestMgrReconfigureRxVec(&testConfConfig) != SUCCESS)
            {
                ret = RC_LTEMAC_PARAM_ERROR;
            }
        }
        
        return ret;
}

RESULTCODE MacApiConfigTestTxVecCfg(PMSGHEADER pMsg, PMSGHEADER pResp)
{
    RESULTCODE ret = SUCCESS;
    PPARAMHEADER pParam = MsgGetNextParam(pMsg, NULL);
    BS_CONFOR_TEST_CONFIG testConfConfig = { 0 };
    U32 idx;

    while (pParam)
    {
        switch (pParam->paramID)
        {
            case 0:                 // End of list
                pParam = NULL;      // Stop
                continue;

            case PAR_PDSCH_MCS_ARRAY_CW1:
                memcpy(testConfConfig.pdschMcs[0], pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.pdschMcs[0])));
                testConfConfig.pdschMcs_used[0] = 1;
                break;
            case PAR_PDSCH_MCS_ARRAY_CW2:
                memcpy(testConfConfig.pdschMcs[1], pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.pdschMcs[1])));
                testConfConfig.pdschMcs_used[1] = 1;
                break;
            case PAR_PDSCH_SCRINITVAL:
                memcpy(testConfConfig.pdschRnti, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.pdschRnti)));
                testConfConfig.pdschRnti_used = 
                    MIN(pParam->length, sizeof(testConfConfig.pdschRnti)) / sizeof(testConfConfig.pdschRnti[0]);
                break;
            case PAR_PDSCH_MAP:
                memcpy(testConfConfig.pdsch_map, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.pdsch_map)));
                testConfConfig.pdsch_map_used = 1;
                break;
            case PAR_PDCCH_MAP:
                memcpy(testConfConfig.pdcch_map, pParam->data, 
                    MIN(pParam->length, sizeof(testConfConfig.pdcch_map)));
                testConfConfig.pdcch_map_used = 1;
                break;
            case PAR_PDSCH:
                idx = (U8)pParam->data[0];
                memcpy(&testConfConfig.PdschCfg[idx], ((U8*)pParam->data+1), 
                    MIN((pParam->length - 1), sizeof(testConfConfig.PdschCfg[0])));
                testConfConfig.PdschCfgEna = 1;
                testConfConfig.nPdsch[0]++;
                break;
            default:                // Unknown parameter received
                printf("%s: received unknown parameter. pParam->paramID = 0x%x\n", __FUNCTION__, pParam->paramID);
                break;
            }
            pParam = MsgGetNextParam(pMsg, pParam);
        }
        
        /* Set 36.141 conformance test options */
        if (ret == SUCCESS)
        {
            if (TestMgrReconfigureTxVec(&testConfConfig) != SUCCESS)
            {
                ret = RC_LTEMAC_PARAM_ERROR;
            }
        }
        
        return ret;
}


RESULTCODE MacApiSetAllocationMap(PMSGHEADER pMsg, PMSGHEADER pResp)
{
    RESULTCODE ret = SUCCESS;
    PPARAMHEADER pParam = MsgGetNextParam(pMsg, NULL);
	U16 sfn;
	
//    printf("%s\n",__FUNCTION__);
    while (pParam)
    {
        switch (pParam->paramID)
        {
            case 0:                
                pParam = NULL;     
                continue;
            case PAR_SFN:     
                sfn = (U32)pParam->data[0];
                pParam = MsgGetNextParam(pMsg, pParam);
                if(pParam->paramID == PAR_PHY_UL_MAP)
                	ret = RctMacAllocationULMap(sfn , (U8*)pParam->data, pParam->length);
                break;
           case PAR_RV:
                ret = SetRv((U8*)pParam->data, pParam->length);
                break;
				
            default:
                pParam = NULL;
                ret = RC_LTEMAC_UNKNOWN_PARAM;
                continue;
        }
        pParam = MsgGetNextParam(pMsg, pParam);
    }
    return ret;
}

/** Handle MAC_TEST_CONFIG message
 *
 * @param pMsg Pointer to the message
 * @param pResp Pointer to the message
 * @return Standard result code
 */
RESULTCODE MacApiConfig(PMSGHEADER pMsg, PMSGHEADER pResp)
{
    RESULTCODE ret = SUCCESS;
    PPARAMHEADER pParam = MsgGetNextParam(pMsg, NULL);
    PHY_AUTORUN_CTX* StartAutoRun = &pBsMacDesc->PhyAutoCtx;
    U32 PhyInstanceId = 0;

    while (pParam)
    {
        switch (pParam->paramID)
        {
            case 0:                 // End of list
                pParam = NULL;      // Stop
                break;
            case PAR_PHY_INSTANCE_ID:
                PhyInstanceId = pParam->data[0];
                break;
            case PAR_PHY_SFCOUNT:
                StartAutoRun->SFStartCount = StartAutoRun->SFCount = pParam->data[0];
                break;
            case PAR_PHY_TIME_RESOLUTION:
                StartAutoRun->TimerResolution = pParam->data[0];
                break;
            case PAR_PHY_DUPLEX_MASK:
                StartAutoRun->Direction = pParam->data[0];
                break;
            case PAR_PHY_TICK_MODE:
                StartAutoRun->AutoMode = pParam->data[0];
                break;
            case PAR_GET_RX_SDU:
                StartAutoRun->RxSduEna = pParam->data[0];
                break;
            default:                // Unknown parameter received
                pParam = NULL;      // Stop
                ret = RC_LTEMAC_UNKNOWN_PARAM;
                break;
        }

        if (((U32)pParam))
        {
        pParam = MsgGetNextParam(pMsg, pParam);
    }
    }

    switch (pMsg->msgID)
    {
        case PHY_LTE_START:
            ret = LteMacAutoRun(PhyAutoStart, PhyInstanceId);
            break;
        case PHY_LTE_STOP:
            ret = LteMacAutoRun(PhyAutoStop, PhyInstanceId);
            break;
        case PHY_LTE_SHUTDOWN:
            ret = LteMacAutoRun(PhyAutoShutdown, PhyInstanceId);
            break;
        default:
            printf("%s: received unknown msgID. pMsg->msgID = 0x%x\n", __FUNCTION__, pMsg->msgID);
            break;
    }

    return ret;
}

#endif /* BSMAC || ENODEB */


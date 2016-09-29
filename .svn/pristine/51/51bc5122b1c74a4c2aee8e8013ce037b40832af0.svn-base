/** @file enbApiHdlr.c
 *
 * @brief API Handler for eNodeB MAC integrated with 4GMX OS (EmbeddedMAC)
 * @author Mindspeed Technologies
 * @version $Revision: 1.75 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include <string.h>
#include <math.h>
#include "ltemaccore.h"
#include "bsmac.h"
#include "LteMacTestCfgMgr.h"
#include "enbApiHdlr.h"
#include "bs_static_config.h"

#include "lte_mac_state.h"
/*
 * Private functions
 */

/** Sets DCI channel payload (PAR_DCI_SET parameter in configuration message)
 *
 * @param pTestConfConfig Pointer to the internal structure holding configuration parameters
 * @param data_in DCI payload
 * @param size_in_bit Size of DCI payload in bytes
 * @param dci_idx Index of PDCCH channel to update
 * @return Standard result code
 */
static RESULTCODE SetDCIData(BS_CONFOR_TEST_CONFIG *pTestConfConfig, U8 *data_in, U32 size_in_bit,
                             U32 idx)
{
    U16 PdcchRnti = (U16)(idx >> 16);
    U16 dci_idx = (U16)(idx & 0xFFFF);
    U32 size_in_byte = (size_in_bit + 7 ) >> 3;

    memcpy(pTestConfConfig->payloadPdcch[dci_idx], data_in, size_in_byte);
    pTestConfConfig->PdcchDataLen[dci_idx] = size_in_bit;
    pTestConfConfig->PdcchRnti[dci_idx] = PdcchRnti;
    pTestConfConfig->PdcchRntiEna = 1;
    return MAC_SUCCESS;
}

/*
 * Public functions
 */

/** Handles query requests sent to RRC layer
 *
 * @param pMsg Pointer to the message header of the query (MSA-E-81479)
 * @param pResp Pointer to the preallocated message buffer used to generate response
 * @return Standard result code
 */
RESULTCODE MacApiQueryRrc(ApiHeader *pMsg, ApiHeader *pResp)
{
    return MAC_SUCCESS;
}

/** Handles configuration commands sent to RRC layer
 *
 * @param pMsg Pointer to the message header of the configuration command (MSA-E-81479)
 * @param pResp Pointer to the preallocated message buffer used to generate response
 * @return Standard result code
 */
RESULTCODE MacApiConfigRrc(ApiHeader *pMsg, ApiHeader *pResp)
{
    return MAC_SUCCESS;
}

/** Responds to MAC_TEST_CONFIG query sent from upper layer (Host)
 *
 * @param pMsg Pointer to the message header of the query (MSA-E-81479)
 * @param pResp Pointer to the preallocated message buffer used to generate response
 * @return Standard result code
 */
RESULTCODE MacApiQueryTestConfig(ApiHeader *pMsg, ApiHeader *pResp)
{
    return MAC_SUCCESS;
}

/** Handle MAC_TEST_CONFIG message
 *
 * @param pMsg Pointer to the message
 * @param pResp Pointer to the message
 * @return Standard result code
 */
RESULTCODE MacApiConfigTestConfig(ApiHeader *pMsg, ApiHeader *pResp)
{
    PBS_STATIC_CONFIG pStaticConfig = get_static_config();

    RESULTCODE ret = MAC_SUCCESS;

    ApiParam* pParam = ApiGetNextParam(pMsg, NULL);
    BS_CONFOR_TEST_CONFIG testConfConfig = { 0 };

    // Set to default configuration if not transmitted
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
        switch (pParam->ParamID)
        {
            case 0:                     // End of list
                pParam = NULL;          // Stop the processing
                continue;
            case PAR_TEST_MODEL:
                testConfConfig.TestModel = (U8) pParam->ParamData[0];
                break;
            case PAR_N_CW:
                testConfConfig.nCw = (U8) pParam->ParamData[0];
                break;
            case PAR_N_LAYERS:
                testConfConfig.nLayers = (U8) pParam->ParamData[0];
                break;
            case PAR_PRECODING_MODE:
                testConfConfig.precodingMode = (U8) pParam->ParamData[0];
                break;
            case PAR_DURATION:
                testConfConfig.duration = (U32) pParam->ParamData[0];
                break;

            case PAR_RNTI_BOOSTED:
                memcpy(testConfConfig.rntiBoosted, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.rntiBoosted)));
                break;
            case PAR_RNTI_DEBOOSTED:
                memcpy(testConfConfig.rntiDeBoosted, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.rntiDeBoosted)));
                break;


            case PAR_RS_BOOSTING:
                testConfConfig.rsBoosting = (U32) pParam->ParamData[0];
                break;
            case PAR_EPRE_SINCHRO:
                testConfConfig.epreSinchro = (S32) pParam->ParamData[0];
                break;
            case PAR_RESERVED_EPRE:
                testConfConfig.reservedEpre = (S32) pParam->ParamData[0];
                break;
            case PAR_PBCH_EPRE:
                testConfConfig.pbchEpre = (U32) pParam->ParamData[0];
                break;
            case PAR_PBCH_RESERVED_EPRE:
                testConfConfig.pbchReservedEpre = (S32) pParam->ParamData[0];
                break;

// --------------------------------------------------------------------------------------------
            case PAR_RS_EPRE:
                pStaticConfig->dl_cmn_pwr_ctl.pilotPower = (S16)pParam->ParamData[0];
                break;

            case PAR_PSS_RA:
                pStaticConfig->dl_cmn_pwr_ctl.psyncPower = (S16)pParam->ParamData[0];
                break;

            case PAR_SSS_RA:
                pStaticConfig->dl_cmn_pwr_ctl.ssyncPower = (S16)pParam->ParamData[0];
                break;

            case PAR_PCFICH_EPRE:
                pStaticConfig->dl_cmn_pwr_ctl.ciPower = (S16)pParam->ParamData[0];
                break;

// --------------------------------------------------------------------------------------------
            case PAR_VRB_DISTRIBUTED:
                pStaticConfig->distributed_flag = (pParam->ParamData[0]) & 1;
                pStaticConfig->n_gap_flag = (pParam->ParamData[0]>>16) & 1;                
                break;

// --------------------------------------------------------------------------------------------
            case PAR_PRS_SUPPORT:
                pStaticConfig->prs_num_rb = (pParam->ParamData[0]) & 0xff;       // Bandwidth available for prs generation (Always smaller than system bandwidth (1,..100)    
                pStaticConfig->prs_ra     = (pParam->ParamData[0]>>8) & 0xff;    // prs Signal Power ratio relative to Cell Specific reference signals power (db)
                pStaticConfig->i_prs      = (pParam->ParamData[0]>>16) & 0x7fff; // PRS configuration Index for Table 6.10.4.3-1 (36.211)
                pStaticConfig->prs_enable = (pParam->ParamData[0]>>31) & 0x1;    // 0 no positioning reference signals are generated in this subframe, 1 prs is generated in this subframe
                break;

            case PAR_HARQ_SUPPORT:
                {
                    PLTE_PDSCH_HARQ_STATE       plte_pdsch_harq_state = get_lte_pdsch_harq_state();

                    plte_pdsch_harq_state->ena_harq = (pParam->ParamData[0]) & 1;
                    testConfConfig.ena_harq = plte_pdsch_harq_state->ena_harq;
                }
                break;

            case PAR_CQI_SUPPORT:
                {
                    PLTE_PDSCH_CHAN_STATE       plte_pdsch_chan_state = get_lte_pdsch_chan_state();

                    plte_pdsch_chan_state->ena_dynamic_cqi_pmi_ri = pParam->ParamData[0] & 1;
                    plte_pdsch_chan_state->sched_num_sfn_ri       = pParam->ParamData[1];
                    plte_pdsch_chan_state->sched_num_sfn_cqi      = pParam->ParamData[2];
                    testConfConfig.ena_dynamic_cqi_pmi_ri = plte_pdsch_chan_state->ena_dynamic_cqi_pmi_ri;
                }
                break;

            case PAR_BCH_DATA_SET:
                memcpy(&testConfConfig.payloadPbch, pParam->ParamData, pParam->ParamLength);
                pBsMacDesc->isMIBconfigured = TRUE;
                break;
            case PAR_N_CTRL_SYMBOLS:
                testConfConfig.nCtrlSymbols = (S32) pParam->ParamData[0];
                break;
            case PAR_N_PHICH_GROUP:
                testConfConfig.nPhichGroup = (U8) pParam->ParamData[0];
                break;
            case PAR_N_PHICH_PER_GROUP:
                testConfConfig.nPhichPerGroup = (U8) pParam->ParamData[0];
                break;
            case PAR_PHICH_RESOURCE:
                uart_printf("!!! duplicating PAR_PHICH_RESOURCE\n");
                testConfConfig.phichResource = (U8)pParam->ParamData[0];
                break;
            case PAR_PHICH_DURATION:
                uart_printf("!!! duplicating PAR_PHICH_DURATION\n");
                testConfConfig.phichDuration = (U8)pParam->ParamData[0];
                break;
             case PAR_PHICH_IDX:
                testConfConfig.phichIdx = (U8)pParam->ParamData[0];
                break;

            case PAR_PHICH_SYM_POWER:
                testConfConfig.phichSymPower = (S32) pParam->ParamData[0];
                break;
            case PAR_PHICH_GROUP_EPRE:
                testConfConfig.phichGroupEpre = (S32) pParam->ParamData[0];
                break;
            case PAR_N_AVALABLE_REGS:
                testConfConfig.nAvalableRegs = (U8) pParam->ParamData[0];
                break;
            case PAR_N_PDCCH:
                testConfConfig.nPdcch = (U8) pParam->ParamData[0];
                break;
            case PAR_N_CCE_PER_PDCCH:
                testConfConfig.nCcePerPdcch = (U8) pParam->ParamData[0];
                break;
            case PAR_N_REG_PER_CCE:
                testConfConfig.nRegPerCce = (U8) pParam->ParamData[0];
                break;
            case PAR_N_REG_ALL_TO_PDCCH:
                testConfConfig.nRegAllToPdcch = (U8) pParam->ParamData[0];
                break;
            case PAR_N_NIL_REG_PADDING:
                testConfConfig.nNilRegPadding = (U8) pParam->ParamData[0];
                break;
            case PAR_PDCCH_REG_EPRE:
                testConfConfig.pdcchRegEpre = (S32) pParam->ParamData[0];
                break;
            case PAR_NIL_REG_EPRE:
                testConfConfig.nilRegEpre = (S32) pParam->ParamData[0];
                break;


                /*
                 * PDSCH parameters
                 */

            case PAR_N_PDSCH_BOOSTED_PRB:
                testConfConfig.nPdschPrb[0][0] = (U8) pParam->ParamData[0];
                testConfConfig.nPdschPrb[1][0] = (U8) pParam->ParamData[0];
                break;
            case PAR_PDSCH_BOOSTED_PRB_PA:
                testConfConfig.pdschPrbPa[0][0] = (S32) pParam->ParamData[0];
                testConfConfig.pdschPrbPa[1][0] = (S32) pParam->ParamData[0];
                break;
            case PAR_PDSCH_BOOSTED_MODULATION:
                testConfConfig.pdschMod[0][0] = (U8) pParam->ParamData[0];
                testConfConfig.pdschMod[1][0] = (U8) pParam->ParamData[0];
                break;
            case PAR_N_PDSCH_DEBOOSTED_PRB:
                testConfConfig.nPdschPrb[0][1] = (U8) pParam->ParamData[0];
                testConfConfig.nPdschPrb[1][1] = (U8) pParam->ParamData[0];
                break;
            case PAR_PDSCH_DEBOOSTED_PRB_PA:
                testConfConfig.pdschPrbPa[0][1] = (S32) pParam->ParamData[0];
                testConfConfig.pdschPrbPa[1][1] = (S32) pParam->ParamData[0];
                break;
            case PAR_PDSCH_DEBOOSTED_MODULATION:
                testConfConfig.pdschMod[0][1] = (U8) pParam->ParamData[0];
                testConfConfig.pdschMod[1][1] = (U8) pParam->ParamData[0];
                break;


                /*
                 * DCI parameters
                 */
            
            case PAR_DCI_SET:
                SetDCIData(&testConfConfig, (U8*) &pParam->ParamData[2],
                           (S32) pParam->ParamData[1], (S32) pParam->ParamData[0]);
                break;

            /*
             * PUSCH parameters
             */

            case PAR_PUSCH_BOOSTED_PRB:
                testConfConfig.nPuschBoostedPrb = (U8) pParam->ParamData[0];
                break;
            case PAR_PUSCH_BOOSTED_PRB_PA:
                testConfConfig.puschBoostedPrbPa = (S32) pParam->ParamData[0];
                break;
            case PAR_PUSCH_BOOSTED_MODULATION:
                testConfConfig.puschBoostedMod = (U8) pParam->ParamData[0];
                break;
            case PAR_PUSCH_DEBOOSTED_PRB_PB:
                break;
            case PAR_PUSCH_DEBOOSTED_PRB_PA:
                break;
            case PAR_PUSCH_DEBOOSTED_MODULATION:
                break;
            case PAR_PUSCH_SCRINITVAL:
                memcpy(testConfConfig.rntiPusch, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.rntiPusch)));
                break;
            case PAR_PUSCH_DED_N_ACK:
                testConfConfig.puschDed.nACK = (U8) pParam->ParamData[0] & 0xF;
                break;
            case PAR_PUSCH_DED_ACKIDX:
                testConfConfig.puschDed.betaOffsetACKIndex = (U8) pParam->ParamData[0] & 0xF;
                break;

            case PAR_PDSCH_MCS_ARRAY_CW1:
                memcpy(testConfConfig.pdschMcs[0], pParam->ParamData, 
                    MIN(pParam->ParamLength,sizeof(testConfConfig.pdschMcs[0])));
                testConfConfig.pdschMcs_used[0] = 1;
                break;
            case PAR_PDSCH_MCS_ARRAY_CW2:
                memcpy(testConfConfig.pdschMcs[1], pParam->ParamData, 
                    MIN(pParam->ParamLength,sizeof(testConfConfig.pdschMcs[1])));
                testConfConfig.pdschMcs_used[1] = 1;
                break;

            case PAR_PUSCH_MCS_ARRAY:
                memcpy(testConfConfig.puschMcs, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.puschMcs)));
                testConfConfig.puschMcs_used = 1;
                break;

            case PAR_CDDTYPE:
                testConfConfig.cddType = (U8) pParam->ParamData[0];
                break;
            case PAR_CODEBOOKIDX:
                testConfConfig.codeBookIdx = (U8) pParam->ParamData[0];
                break;
            case PAR_RV_DL:
                memcpy(testConfConfig.arRctRvDl, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.arRctRvDl)));
                break;


            case PAR_MCS_DL:
                memcpy(testConfConfig.RctMcsDl, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.RctMcsDl)));
                break;
            case PAR_RV:
                memcpy(testConfConfig.arRctRv, pParam->ParamData,
                    MIN(pParam->ParamLength, sizeof(testConfConfig.arRctRv)));
                break;
            case PAR_MCS_UL:
                memcpy(testConfConfig.RctMcsUl, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.RctMcsUl)));
                break;
            case PAR_TXMODE:
                testConfConfig.transmissionMode = (U8) pParam->ParamData[0];
                break;
            case PAR_PHY_SFCOUNT:
                // We need to know it to reallocate this buffer if required
                testConfConfig.SFcount = pParam->ParamData[0];
                break;

            case PAR_PUCCH_DELTA_SHIFT: // PUCCH
                testConfConfig.ulSfrCtrl.deltaPUCCHShift = (U8) pParam->ParamData[0];
                break;
            case PAR_PUCCH_nRBCQI:
                testConfConfig.ulSfrCtrl.nRBCQI = (U8) pParam->ParamData[0];
                break;
            case PAR_PUCCH_nCSAN:
                testConfConfig.ulSfrCtrl.nCSAn = (U8) pParam->ParamData[0];
                break;
            case PAR_PUCCH_n1AN:
                testConfConfig.ulSfrCtrl.n1PucchAN = (U16) pParam->ParamData[0];
                break;
            case PAR_PUCCH_CYCLIC_SHIFT:
                testConfConfig.ulSfrCtrl.cyclicShift = (U8) pParam->ParamData[0];
                break;
            case PAR_PUCCH_FORMAT:
                testConfConfig.ulSfrCtrl.uciformat = (U8) pParam->ParamData[0];
                break;
            case PAR_UL_QAM64_ENA:
                testConfConfig.ulSfrCtrl.enable64QAM = (U8) pParam->ParamData[0];
                break;
            case PAR_PUCCH_DLCQI_SIZE_BITS:
                testConfConfig.ulSfrCtrl.dlCqiPmiSizeBits = (U8) pParam->ParamData[0];
                break;
				
	    case PAR_HOPPING_MODE:
		testConfConfig.ulSfrCtrl.hoppingMode = (U8) pParam->ParamData[0];
		break;
	    case PAR_PUSCH_HOPPING_OFFSET:
		testConfConfig.ulSfrCtrl.puschhoppingOffset = (U8) pParam->ParamData[0];
		break;
	    case PAR_GR_HOPPING_ENA:
		testConfConfig.ulSfrCtrl.groupHoppingEnabled = (U8) pParam->ParamData[0];
		break;
	    case PAR_GR_ASSIG_PUSCH:
		testConfConfig.ulSfrCtrl.groupAssignmentPUSCH = (U8) pParam->ParamData[0];
		break;
	    case PAR_SEQ_HOPPING_ENA:
		testConfConfig.ulSfrCtrl.sequenceHoppingEnabled = (U8) pParam->ParamData[0];
		break;
	    case PAR_nSB:
		testConfConfig.ulSfrCtrl.nSB= (U8) pParam->ParamData[0];
		break;

            case PAR_PUSCH_NUM:
                memcpy(testConfConfig.numPusch, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.numPusch)));
                break;
            case PAR_PUCCH_NUM:
                memcpy(testConfConfig.numPucch, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.numPucch)));
                break;

            case PAR_SWEEP_DL_MAP_PDSCH1:
                memcpy(testConfConfig.SwpTst_MapIdx_1, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.SwpTst_MapIdx_1)));
                break;

            case PAR_SWEEP_DL_MAP_PDSCH2:
                memcpy(testConfConfig.SwpTst_MapIdx_2, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.SwpTst_MapIdx_2)));
                break;
                /*
                 * PRACH parameters
                 */
            
            case PAR_PRACH_ENA:
                break;
            case PAR_PRACH_HIGH_SPEED_FL:
                testConfConfig.highSpeedFlag = (U8) pParam->ParamData[0];
                break;
            case PAR_PRACH_ZERO_CORR_ZONE_CFG:
                testConfConfig.zeroCorrelationZoneConfig = (U8) pParam->ParamData[0];
                break;
            case PAR_PRACH_ROOT_SEQ_IDX:
                testConfConfig.rootSequenceIndex = (U32) pParam->ParamData[0];
                break;
            case PAR_PRACH_CFG_IDX:
                testConfConfig.prachConfigIndex = (U8) pParam->ParamData[0];
                break;
            case PAR_PRACH_FREQ_OFFSET:
                testConfConfig.prachFreqOffset = (U8) pParam->ParamData[0];
                break;
            case PAR_PDSCH_PAYLOAD:
                testConfConfig.pdschPayload = (U8)pParam->ParamData[0];
                testConfConfig.pdschPayloadEna = 1;

            default:                    // Unknown parameter received
                break;
        }
        pParam = ApiGetNextParam(pMsg, pParam);
    }
    // Set 36.141 conformance test options
    if (ret == MAC_SUCCESS)
    {
        if (TestMgrReconfigure(&testConfConfig) != MAC_SUCCESS)
        {
            ret = RC_LTEMAC_PARAM_ERROR;
        }
    }
    return ret;
}


RESULTCODE MacApiConfigTestRxVecCfg(ApiHeader *pMsg, ApiHeader *pResp)
{
    RESULTCODE ret = MAC_SUCCESS;
    U32 i;
    ApiParam* pParam = ApiGetNextParam(pMsg, NULL);
    BS_CONFOR_TEST_CONFIG testConfConfig = { 0 };

    while (pParam)
    {
        switch (pParam->ParamID)
        {
            case 0:                 // End of list
                pParam = NULL;      // Stop
                continue;
            case PAR_TIMADVERR_DFLT_PUCCH:
                memcpy(testConfConfig.timAdvErrDflt_pucch, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.timAdvErrDflt_pucch)));
                testConfConfig.timAdvErrDflt_pucch_len = 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.timAdvErrDflt_pucch));
                break;
            case PAR_TIMADVERR_DFLT_PUSCH:
                memcpy(testConfConfig.timAdvErrDflt_pusch, pParam->ParamData,
                    MIN(pParam->ParamLength, sizeof(testConfConfig.timAdvErrDflt_pusch)));
                testConfConfig.timAdvErrDflt_pusch_len = 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.timAdvErrDflt_pusch));
                break;
            case PAR_SRS_BW_CFG:
                testConfConfig.srsBandwitdhConfig = (U8)pParam->ParamData[0];
                break;
            case PAR_SRS_SF_CFG:
                testConfConfig.srsSubframeConfig = (U8)pParam->ParamData[0];
                break;
            case PAR_SRSINFO_NUM:
                testConfConfig.srsNumber = (U32)pParam->ParamData[0];
                break;
            case PAR_SRSINFO_BW:
                for(i = 0; i < pParam->ParamLength; i++)
                    testConfConfig.srsInfo[i].srsBandwidth = *((U8*)pParam->ParamData + i);
                break;
            case PAR_SRSINFO_HOPBW:
                for(i = 0; i < pParam->ParamLength; i++)
                    testConfConfig.srsInfo[i].srsHoppingBandwidth = *((U8*)pParam->ParamData + i) & 3;
                break;
            case PAR_SRSINFO_FREQPOS:
                for(i = 0; i < pParam->ParamLength; i++)
                    testConfConfig.srsInfo[i].freqDomainPosition = *((U8*)pParam->ParamData + i) & 0x1F;
                break;
            case PAR_SRSINFO_CFGIDX:
                for(i = 0; i < pParam->ParamLength >> 1; i++)
                    testConfConfig.srsInfo[i].srsConfigIndex = *((U16*)pParam->ParamData + i) & 0x3FF;
                break;
            case PAR_SRSINFO_CYCLSHFT:
                for(i = 0; i < pParam->ParamLength; i++)
                    testConfConfig.srsInfo[i].cyclicShift = *((U8*)pParam->ParamData + i) & 0x7;
                break;
            case PAR_SRSINFO_DUR:
                for(i = 0; i < pParam->ParamLength; i++)
                    testConfConfig.srsInfo[i].duration = *((U8*)pParam->ParamData + i) & 1;
                break;
            case PAR_SRSINFO_TXCOMB:
                for(i = 0; i < pParam->ParamLength; i++)
                    testConfConfig.srsInfo[i].transmissionComb = *((U8*)pParam->ParamData + i) & 1;
                break;

            case PAR_PUSCH_MCS_ARRAY:
                memcpy(testConfConfig.puschMcs, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.puschMcs)));
                testConfConfig.puschMcs_used = 1;
                break;
            case PAR_PUSCH_SCRINITVAL:
                memcpy(testConfConfig.rntiPusch, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.rntiPusch)));
                testConfConfig.rntiPusch_used = 1;
                break;

            case PAR_PUSCH_MAP:
                memcpy(testConfConfig.pusch_map, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.pusch_map)));
                testConfConfig.pusch_map_used = 1;
                break;

          case PAR_PUCCH_SCRINITVAL:
              memcpy(testConfConfig.rntiPucch, pParam->ParamData, 
                  MIN(pParam->ParamLength, sizeof(testConfConfig.rntiPucch)));
              testConfConfig.rntiPucch_used = 1;
              break;

          case PAR_PUCCH_FORMAT:
              memcpy(testConfConfig.pucchFormat, pParam->ParamData, 
                  MIN(pParam->ParamLength, sizeof(testConfConfig.pucchFormat)));
              testConfConfig.pucchFormat_used = 1;
              break;

          case PAR_PUCCH_MAP:
              memcpy(testConfConfig.pucch_map, pParam->ParamData, 
                  MIN(pParam->ParamLength, sizeof(testConfConfig.pucch_map)));
              testConfConfig.pucch_map_used = 1;
              break;

          case PAR_PUCCH_DLCQI_SIZE_BITS:
              memcpy(testConfConfig.pucch_dlCqiPmiSizeBits, pParam->ParamData, 
                  MIN(pParam->ParamLength, sizeof(testConfConfig.pucch_dlCqiPmiSizeBits)));
              testConfConfig.pucch_dlCqiPmiSizeBits_used = 1;
              break;

          case PAR_PUCCH_HARQ_SIZE_BITS:
              memcpy(testConfConfig.pucch_harqSizebits, pParam->ParamData, 
                  MIN(pParam->ParamLength, sizeof(testConfConfig.pucch_harqSizebits)));
              testConfConfig.pucch_harqSizebits_used = 1;
              break;

          case PAR_PUCCH_HARQ_IDX:
              memcpy(testConfConfig.pucch_harqPucchIndex, pParam->ParamData, 
                  MIN(pParam->ParamLength, sizeof(testConfConfig.pucch_harqPucchIndex)));
              testConfConfig.pucch_harqPucchIndex_used= 1;
              break;

          case PAR_PUCCH_CQI_RSRC_IDX:
              memcpy(testConfConfig.pucch_cqirsrcidx, pParam->ParamData, 
                  MIN(pParam->ParamLength, sizeof(testConfConfig.pucch_cqirsrcidx)));
              testConfConfig.pucch_cqirsrcidx_used= 1;
              break;

//       PUSCH dedicated info configured
            case PAR_PUSCH_DED_RIIDX:
                memcpy(testConfConfig.PuschRiIdx, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.PuschRiIdx)));
                testConfConfig.PuschRiIdxUsed = 1;
                break;
            
            case PAR_PUSCH_DED_CQIIDX:
                memcpy(testConfConfig.PuschCqiIdx, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.PuschCqiIdx)));
                testConfConfig.PuschCqiIdxUsed = 1;
                break;
            
            case PAR_PUSCH_DED_N_nRI:
                testConfConfig.Pusch_nRiUsed = MIN(pParam->ParamLength, sizeof(testConfConfig.Pusch_nRi));
                memcpy(testConfConfig.Pusch_nRi, pParam->ParamData, testConfConfig.Pusch_nRiUsed);
                break;
            
            case PAR_PUSCH_DED_N_nr1CQI:
                testConfConfig.Pusch_nrlCqiUsed = MIN(pParam->ParamLength, sizeof(testConfConfig.Pusch_nrlCqi));
                memcpy(testConfConfig.Pusch_nrlCqi, pParam->ParamData, testConfConfig.Pusch_nrlCqiUsed);
                break;
            
            case PAR_PUSCH_DED_N_nrg1CQI:
                testConfConfig.Pusch_nrglCqiUsed = MIN(pParam->ParamLength, sizeof(testConfConfig.Pusch_nrglCqi));
                memcpy(testConfConfig.Pusch_nrglCqi, pParam->ParamData, testConfConfig.Pusch_nrglCqiUsed);
                break;

            case PAR_PUSCH_DED_N_ACK:
                testConfConfig.Pusch_nAckUsed = MIN(pParam->ParamLength, sizeof(testConfConfig.Pusch_nAck));
                memcpy(testConfConfig.Pusch_nAck, pParam->ParamData, testConfConfig.Pusch_nAckUsed);
                break;

            case PAR_PUSCH_DED_ACKIDX:
                memcpy(testConfConfig.PuschAckIdx, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.PuschAckIdx)));
                testConfConfig.PuschAckUsed = 1;
                break;
            
              default:                // Unknown parameter received
                    break;
            }
            pParam = ApiGetNextParam(pMsg, pParam);
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

RESULTCODE MacApiConfigTestTxVecCfg(ApiHeader *pMsg, ApiHeader *pResp)
{
    RESULTCODE  ret = MAC_SUCCESS;
    U32         idx;
    ApiParam*   pParam = ApiGetNextParam(pMsg, NULL);
    BS_CONFOR_TEST_CONFIG testConfConfig = { 0 };

    while (pParam)
    {
        switch (pParam->ParamID)
        {
            case 0:                 // End of list
                pParam = NULL;      // Stop
                continue;

            case PAR_PDSCH_MCS_ARRAY_CW1:
                memcpy(testConfConfig.pdschMcs[0], pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.pdschMcs[0])));
                testConfConfig.pdschMcs_used[0] = 1;
                break;
            case PAR_PDSCH_MCS_ARRAY_CW2:
                memcpy(testConfConfig.pdschMcs[1], pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.pdschMcs[1])));
                testConfConfig.pdschMcs_used[1] = 1;
                break;
            case PAR_PDSCH_SCRINITVAL:
                memcpy(testConfConfig.pdschRnti, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.pdschRnti)));
                testConfConfig.pdschRnti_used = 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.pdschRnti)) / sizeof(testConfConfig.pdschRnti[0]);
                break;
            case PAR_PDSCH_MAP:
                memcpy(testConfConfig.pdsch_map, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.pdsch_map)));
                testConfConfig.pdsch_map_used = 1;
                break;
            case PAR_PDCCH_MAP:
                memcpy(testConfConfig.pdcch_map, pParam->ParamData, 
                    MIN(pParam->ParamLength, sizeof(testConfConfig.pdcch_map)));
                testConfConfig.pdcch_map_used = 1;
                break;
            case PAR_PDSCH:
                idx = (U8)pParam->ParamData[0];
                memcpy(&testConfConfig.PdschCfg[idx], ((U8*)pParam->ParamData + 1), 
                    MIN((pParam->ParamLength - 1), sizeof(testConfConfig.PdschCfg[0])));
                testConfConfig.PdschCfgEna = 1;
                testConfConfig.nPdsch[0]++;
                break;

              default:                // Unknown parameter received
                    break;
            }
            pParam = ApiGetNextParam(pMsg, pParam);
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

/** Handles PHY_LTE_START and PHY_LTE_STOP messages
 *
 * @param pHdr Pointer to the message configuration message's header (MSA-E-81479)
 * @param pResp Pointer to the preallocated buffer to store response
 * @return Standard result code
 */
RESULTCODE MacApiConfig(ApiHeader *pHdr, ApiHeader *pResp)
{
    RESULTCODE ret = MAC_SUCCESS;
    ApiParam *pParam = ApiGetNextParam(pHdr, NULL);
    PHY_AUTORUN_CTX *StartAutoRun = &pBsMacDesc->PhyAutoCtx;
    U32 PhyInstanceId = 0;

    if(pBsMacDesc == NULL)
    {
        uart_printf("pBsMacDesc is NULL, please do CreateMac first\r\n");
        return FAILURE;
    }

    while (pParam)
    {
        switch (pParam->ParamID)
        {
            case 0:                     // End of list
                pParam = NULL;          // Stop processing
                continue;
            case PAR_PHY_INSTANCE_ID:
                PhyInstanceId = pParam->ParamData[0];
                break;
            case PAR_PHY_SFCOUNT:
                StartAutoRun->SFStartCount = StartAutoRun->SFCount = pParam->ParamData[0];
                break;
            case PAR_PHY_TIME_RESOLUTION:
                StartAutoRun->TimerResolution = pParam->ParamData[0];
                break;
            case PAR_PHY_DUPLEX_MASK:
                StartAutoRun->Direction = pParam->ParamData[0];
                break;
            case PAR_PHY_TICK_MODE:
                StartAutoRun->AutoMode = pParam->ParamData[0];
                break;
            case PAR_GET_RX_SDU:
                StartAutoRun->RxSduEna = pParam->ParamData[0];
                break;
            default:                    // Unknown parameter received
                pParam = NULL;          // Stop processing
                ret = RC_LTEMAC_UNKNOWN_PARAM;
                continue;
        }
        pParam = ApiGetNextParam(pHdr, pParam);
    }

    switch (pHdr->MessageID)
    {
        case PHY_LTE_START:
            ret = eNbMacAutoRun(PhyAutoStart, PhyInstanceId);
            break;
        case PHY_LTE_STOP:
            ret = eNbMacAutoRun(PhyAutoStop, PhyInstanceId);
            break;
        case PHY_LTE_SHUTDOWN:
        case SVSR_DESTROYPHY:
            // both APIs do the same thing
            ret = eNbMacAutoRun(PhyAutoShutdown, PhyInstanceId);
            break;

    }
    return ret;
}

/** Handles MAC_TEST_MAP_CONF configuration message
 *
 * @param pMsg Pointer to the message header (MSA-E-81479)
 * @param pResp Pointer to the preallocated buffer to store message response
 * @return Standard result code
 */
RESULTCODE MacApiSetAllocationMap(ApiHeader *pMsg, ApiHeader *pResp)
{
    RESULTCODE ret = SUCCESS;
    ApiParam *pParam = ApiGetNextParam(pMsg, NULL);
    U16 sfn;

    while (pParam)
    {
        switch (pParam->ParamID)
        {
            case 0:
                pParam = NULL;
                continue;
            case PAR_SFN:
                sfn = (U32) pParam->ParamData[0];
                pParam = ApiGetNextParam(pMsg, pParam);
                if (pParam && (pParam->ParamID == PAR_PHY_UL_MAP))
                {
                    ret = RctMacAllocationULMap(sfn, (U8*) pParam->ParamData, pParam->ParamLength);
                }
                break;
            default:
                pParam = NULL;
                ret = RC_LTEMAC_UNKNOWN_PARAM;
                continue;
        }
        pParam = ApiGetNextParam(pMsg, pParam);
    }
    return ret;
}

RESULTCODE MacApiInvalidDataTest(ApiHeader *pMsg, ApiHeader *pResp)
{
    RESULTCODE ret = MAC_SUCCESS;
    ApiParam *pParam = ApiGetNextParam(pMsg, NULL);
    PHY_AUTORUN_CTX *StartAutoRun = &pBsMacDesc->PhyAutoCtx;
    U32 PhyInstanceId = 0;

    if(pBsMacDesc == NULL)
    {
        uart_printf("pBsMacDesc is NULL, please do CreateMac first\r\n");
        return FAILURE;
    }
    while (pParam)
    {
        switch (pParam->ParamID)
        {
            case 0:
                pParam = NULL;
                continue;
            case PAR_INVALID_DELAY:
		pBsMacDesc->PhyTest.TrDelay = (U32) pParam->ParamData[0];
                break;
	    case PAR_INVALID_API_CORRUPTION:
		pBsMacDesc->PhyTest.CorruptionApi = (U32) pParam->ParamData[0];
		break;
            default:
                pParam = NULL;
                ret = RC_LTEMAC_UNKNOWN_PARAM;
                continue;
        }
        pParam = ApiGetNextParam(pMsg, pParam);
    }
    return ret;
}

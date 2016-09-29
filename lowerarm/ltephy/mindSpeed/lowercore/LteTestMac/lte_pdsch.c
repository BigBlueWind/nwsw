/** @file lte_pdsch.c
 *
 * @brief Implemenataion for creating PDSCH message
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "LtePhyL2Api.h"
#include "resultcodes.h"
#include "LteMacTestCfgMgr.h"
#include "bsmac.h"

#include "basetypes.h"

#include "lte_mac_typedef.h"
#include "lte_etm.h"
#include "lte_mac_state.h"
#ifndef MX_ARM_ENABLED
#include "msgroute.h"
#include "osal.h" // dlControlTxSduQueue
#include "LteMac2PhyInterface.h" // MsgAllocBuffer(MSG_FAST);

#include "udp_stream.h"
#include "timers.h"
#include "tl.h"
#else
#include "enbMac2PhyIface.h" //Mac2PhyIfAllocBuffer();
#endif

#include "lte_pdsch.h"
#include "lte_rnti.h"
#include "lte_modulation.h"
#include "LteTestTables.h"

extern LTE_MAC_STATE    lte_mac_state;
extern BS_CONTEXT       *pBsMacDesc;

extern tTestMgrGetPdschMap        pTestMgrGetBoostedPdsch;
extern tTestMgrGetPdschMap        pTestMgrGetDeBoostedPdsch;
extern lte_pdsch_set_payload_cb   plte_pdsch_set_payload;



#ifndef MX_ARM_ENABLED
extern OSAL_QUEUE dlControlTxSduQueue2;
#endif


/* Get pointer to lte_mac_state.lte_pdsch_chan_state-structure
 *
 * @return pointer to lte_mac_state.lte_pdsch_chan_state-structure
 */
PLTE_PDSCH_CHAN_STATE      get_lte_pdsch_chan_state(void)
{
    
    return (PLTE_PDSCH_CHAN_STATE)&lte_mac_state.lte_pdsch_chan_state;
}


/* Get pointer to lte_mac_state.lte_pdsch_chan_state.lte_pdsch_chan_desc[chan_id]
 *
 * @param chan_id channel index
 * @return pointer to lte_mac_state.lte_pdsch_chan_state.lte_pdsch_chan_desc[chan_id]-structure
 */
PLTE_PDSCH_CHAN_DESC       get_lte_pdsch_chan_desc(U32 chan_id)
{
    return (PLTE_PDSCH_CHAN_DESC)&lte_mac_state.lte_pdsch_chan_state.lte_pdsch_chan_desc[chan_id];
}


// Compatible with SQV-testing
// Need to update this function for real-MAC
/* Get msc via cqi
 *
 * @param cqi cqi value
 * @return mcs index
 */
U8 lte_pdsch_calc_mcs_via_cqi(U8 cqi)
{
    return cqi;
}


/* Get modulation type via msc
 *
 * @param mcs index
 * @return modulation type
 */
static U8 lte_pdsch_conv_mcs_to_modulation(U8 Mcs)
{
    if(Mcs <= 9 || Mcs == 29)
        return QPSK;
    else if(Mcs <= 16 || Mcs == 30)
        return QAM16;
    else if(Mcs <= 28 || Mcs == 31)
        return QAM64;
    else
        return 0xFF;
}


/** Get modulation value for PDSCH channel
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @param  Pdsch index
 * @param  layer index
 * @return Modulation value
 */
U8 lte_pdsch_get_modulation(BS_CONFOR_TEST_CONFIG *pTestConfConfig, U8 pdsch_idx, U8 layer_idx)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return pTestConfConfig->pdschMod[layer_idx][pdsch_idx];
}


/** Set mapinfo for PDSCH (Analize testMgrCtx.boostingMap)
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @param  pointer to mapinfo
 * @param  Pdsch index
 * @return Standard result code
 */
RESULTCODE lte_pdsch_set_mapinfo_boosted(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                         MAPPINGINFO *pMapInfo, 
                                         U16 ChanId, 
                                         U8 LayerIdx)
{
    U8 **pBoostingMap;
    U8 subFramNum;
    const U8 *pMap;
    U32 cur_num;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    memset(pMapInfo, 0, sizeof(MAPPINGINFO));

    subFramNum = getSubFrameNum();
    if(pTestConfConfig->nPdschPrb[LayerIdx][ChanId] == 0)
        return FAILURE;

    if(testMgrCtx.boostingMap[LayerIdx][ChanId] == NULL)
    {
        printf("ERROR: boostingMap has bad ptr [%s,%s:%d]\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }

    pBoostingMap = testMgrCtx.boostingMap[LayerIdx][ChanId];
    pMap = pBoostingMap[subFramNum];
    cur_num = pTestConfConfig->nPdschPrb[LayerIdx][ChanId];

    if(*pMap == 0xFF)
        return FAILURE;

    while(*pMap != 0xFF)
    {
        pMapInfo->reselmInfo[pMapInfo->numberofEntries].startRes = *pMap;
        pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes =
            TestMgrGetNumConsecutiveRBs((U8**)&pMap, &cur_num);
        pMapInfo->numberofEntries++;
        if(cur_num == 0)
            break;
    }

    return SUCCESS;
}

/** Set mapinfo for PDSCH (Analize testMgrCtx.boostingMap)
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @param  pointer to mapinfo
 * @param  Pdsch index
 * @return Standard result code
 */
RESULTCODE lte_pdsch_set_mapinfo_boosted_ex(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                            MAPPINGINFO *pMapInfo, 
                                            U16 ChanId, 
                                            U8 LayerIdx)
{
    U8 **pBoostingMap;
    U8 *pMap;
    U32 cur_num;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    memset(pMapInfo, 0, sizeof(MAPPINGINFO));

    if(pTestConfConfig->nPdschPrb[LayerIdx][ChanId] == 0)
        return FAILURE;

    if(testMgrCtx.boostingMap[LayerIdx][ChanId] == NULL)
    {
        printf("ERROR: boostingMap has bad ptr [%s,%s:%d]\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }

    pBoostingMap = testMgrCtx.boostingMap[LayerIdx][ChanId];
    pMap = (U8*)pBoostingMap;
    cur_num = pTestConfConfig->nPdschPrb[LayerIdx][ChanId];

    if(*pMap == 0xFF)
        return FAILURE;

    while(*pMap != 0xFF)
    {
        pMapInfo->reselmInfo[pMapInfo->numberofEntries].startRes = *pMap;
        pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes =
                                            TestMgrGetNumConsecutiveRBs(&pMap, &cur_num);
        pMapInfo->numberofEntries++;
        if(cur_num == 0)
            break;
    }

    return SUCCESS;
}

/** Set mapinfo for PDSCH (Copy from pdsch_map)
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @param  pointer to mapinfo
 * @param  Pdsch index
 * @return Standard result code
 */
RESULTCODE lte_pdsch_set_mapinfo_boosted_direct(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                                MAPPINGINFO *pMapInfo, 
                                                U16 ChanId, 
                                                U8 LayerIdx)
{

    PRINT_FUNCTION_NAME(__FUNCTION__);

    memset(pMapInfo, 0, sizeof(MAPPINGINFO));

    pMapInfo->reselmInfo[pMapInfo->numberofEntries].startRes = pTestConfConfig->pdsch_map[ChanId].startRes;
    pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes = pTestConfConfig->pdsch_map[ChanId].numRes;
    pMapInfo->numberofEntries++;

    return SUCCESS;
}

/** Set mapinfo for PDSCH (Copy from PdschCfg)
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @param  pointer to mapinfo
 * @param  Pdsch index
 * @return Standard result code
 */
RESULTCODE lte_pdsch_setmapinfo_direct(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                       MAPPINGINFO *pMapInfo, 
                                       U16 pdsch_idx)
{
    //U32 i = 0, num = 0;
    MAPPINGINFO* in_map = &pTestConfConfig->PdschCfg[pdsch_idx].Map;
    PRINT_FUNCTION_NAME(__FUNCTION__);

//    memset(pMapInfo, 0, sizeof(MAPPINGINFO));
    memcpy(pMapInfo, in_map, sizeof(MAPPINGINFO));

/*
    while(i < in_map->numberofEntries)
    {
        if(in_map->reselmInfo[num].numRes != 0)
        {
            pMapInfo->reselmInfo[num].startRes = in_map->reselmInfo[num].startRes;
            pMapInfo->reselmInfo[num].numRes = in_map->reselmInfo[num].numRes;
            i++;
        }
        num++;
    }
    pMapInfo->numberofEntries = in_map->numberofEntries;*/

    return SUCCESS;
}

/** Set mapinfo for PDSCH (Analize testMgrCtx.deBoostingMap)
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @param  pointer to mapinfo
 * @param  Pdsch index
 * @return Standard result code
 */
RESULTCODE lte_pdsch_set_mapinfo_deboosted(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                           MAPPINGINFO *pMapInfo, 
                                           U16 Pdsch_idx, 
                                           U8 LayerIdx)
{
    MAPPINGINFO mapBoosted;
    U32 i = 0;
    U8 boostedStarRb;
    U8 boostedNumRb;
    U8 nextStarRb;
    //U8 nextNumRb;
    U8 subFramNum;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    /* copy */
    mapBoosted = *pMapInfo;

    /* clean */
    memset(pMapInfo, 0, sizeof(MAPPINGINFO));

    boostedStarRb = mapBoosted.reselmInfo[i].startRes;
    boostedNumRb = mapBoosted.reselmInfo[i].numRes;

    // start
    if (boostedStarRb != 0)
    {
        pMapInfo->reselmInfo[pMapInfo->numberofEntries].startRes = 0;
        pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes = boostedStarRb;

        if (pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes)
            pMapInfo->numberofEntries++;
    }

    if (mapBoosted.numberofEntries > 1)
    {/* multiple RBs for boosted */
        i = 0;
        while (i < mapBoosted.numberofEntries)
        {
            boostedStarRb = mapBoosted.reselmInfo[i].startRes;
            boostedNumRb = mapBoosted.reselmInfo[i].numRes;
            if (i + 1 < mapBoosted.numberofEntries)
            {
                nextStarRb = mapBoosted.reselmInfo[i+1].startRes;
                //nextNumRb = mapBoosted.reselmInfo[i+1].numRes;

                pMapInfo->reselmInfo[pMapInfo->numberofEntries].startRes =
                    boostedStarRb + boostedNumRb;

                pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes =
                    nextStarRb - (boostedStarRb + boostedNumRb);
            }
            else
            {
                pMapInfo->reselmInfo[pMapInfo->numberofEntries].startRes =
                    boostedStarRb + boostedNumRb;

                pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes =
                    (pTestConfConfig->nPdschPrb[LayerIdx][Pdsch_idx] +
                pTestConfConfig->nPdschPrb[LayerIdx][Pdsch_idx - 1]) - (boostedStarRb + boostedNumRb);
            }

            if (pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes)
                pMapInfo->numberofEntries++;

            i++;
        }
    }
    else if (mapBoosted.numberofEntries == 0)
    {// fill all the map by DeBoosted
        U8 *pMap;
        U32 cur_num = 1000;

        /* check if we've got  deBoosting in this model */
        if (testMgrCtx.deBoostingMap == NULL)
            return FAILURE;
        
        subFramNum = getSubFrameNum();
        
        if(*testMgrCtx.deBoostingMap[subFramNum] == 0xFF)
            return FAILURE;

        pMap = testMgrCtx.deBoostingMap[subFramNum];

        while(*pMap != 0xFF)
        {
            pMapInfo->reselmInfo[pMapInfo->numberofEntries].startRes = *pMap;
            pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes =
                                                TestMgrGetNumConsecutiveRBs(&pMap, &cur_num);
            pMapInfo->numberofEntries++;
            if(cur_num == 0)
                break;
        }
    }
    else
    {/* one RB for boosted */
        boostedStarRb = mapBoosted.reselmInfo[0].startRes;
        boostedNumRb = mapBoosted.reselmInfo[0].numRes;

        pMapInfo->reselmInfo[pMapInfo->numberofEntries].startRes =
            boostedStarRb + boostedNumRb;
        pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes =
            pTestConfConfig->nPdschPrb[LayerIdx][Pdsch_idx] - boostedStarRb;

        if (pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes)
            pMapInfo->numberofEntries++;
    }

    if(pMapInfo->numberofEntries == 0)
        return FAILURE;

    return SUCCESS;
}


/** Get power value for PDSCH channel
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @param  Pdsch index
 * @param  layer index
 * @return power in 2Q8 format
 */
static S16 lte_pdsch_get_power(BS_CONFOR_TEST_CONFIG *pTestConfConfig, U8 pdsch_idx, U8 layer_idx)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return (S16)(pTestConfConfig->pdschPrbPa[layer_idx][pdsch_idx]);
}


/* Function addes sub PDSCH channel to TXVECTOR
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @param pCh Pointer to DlChanDesc into TxVector
 * @param Pdsch index
 * @param Harq Id
 * @param Modulation
 * @param Mcs Type
 * @param Code word index
 * @return Standard result code
 */
static RESULTCODE lte_pdsch_fill_sub_desc(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                         PDLSUBCHINFO pSubCh,
                                         U8  pdsch_idx,
                                         U8  harq_id,
                                         U8  modulation,
                                         U8  mcs_type,
                                         U32 cw_id)
{

    PRINT_FUNCTION_NAME(__FUNCTION__);

    pSubCh->codingDescriptor = TURBOCDR;
    pSubCh->blockCodeConcatenation = 1;
    pSubCh->modulationType = modulation;

    pSubCh->mcsType = mcs_type;

    pSubCh->reserved = NOT_SPECIFIED_VALUE;

    lte_harq_fill_pdsch_ndi_rv(pSubCh, cw_id, pdsch_idx, harq_id&7);

    if(pTestConfConfig->PdschCfgEna)
    {
        pSubCh->rV = pTestConfConfig->PdschCfg[pdsch_idx].CwCfg[cw_id].Rvi;
    }

    pSubCh->flushReq = 0;

    pSubCh->pad = 0;

    return SUCCESS;
}


/* Copy payload from udp-queue to buf
 *
 * @param buf pointer to destination
 * @param num number of bytes for copy
 * @return coppied number of bits
 */
U32 lte_pdsch_copy_payload_from_udp(U8 *buf, U32 num)
{
#ifndef MX_ARM_ENABLED
    OSAL_QUEUE  *pqueue = get_osal_udp_rx_queue();

    OsalCyclicBuffGetData_spinlock((OSAL_QUEUE *)pqueue, (U8*)buf, num);
#endif
    return num<<3;
}


/* Set payload to 0xff to buf
 *
 * @param buf pointer to destination
 * @param num number of bytes for copy
 * @return coppied number of bits
 */
U32 lte_pdsch_set_payload(U8 *buf, U32 num)
{
    memset((U8*)buf, 0xff, num);
    return num<<3;
}


/* Function addes PDSCH channel to Subframe with Map According to configuration
 *
 * @param pointer to TestConfConfig-structure
 * @param pointer to DLCHANDESC-structure
 * @param total index of channel
 * @param current index of pdsch channel
 * @param pointer to MapInfo
 * @param rnti value 
 * @param power value
 * @return Standard result code
 */
RESULTCODE lte_pdsch_add(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                         PDLCHANDESC pCh,
                         U32 id,
                         U32 pdsch_idx,
                         MAPPINGINFO *pMapInfo,
                         U16 rnti,
                         S16 power)
{
    U32 i;
    PLTE_PDSCH_CHAN_DESC    plte_pdsch_chan_desc = get_lte_pdsch_chan_desc(pdsch_idx);
    PLTE_PDSCH_CHAN_STATE   plte_pdsch_chan_state = get_lte_pdsch_chan_state();
    BS_CONFOR_TEST_CONFIG*  ptest_cfg = get_bs_conf_test_config();
    PBS_STATIC_CONFIG       pstatic_config = get_static_config();

    PRINT_FUNCTION_NAME(__FUNCTION__);

    /* Fill Chann desc */
    pCh->offsetNextCh = NOT_SPECIFIED_VALUE;

    if (id>100)
        printf("@");

    pCh->channelId = (U16)id;

// TODO: 
#ifndef MX_ARM_ENABLED
    if(pTestConfConfig->TestModel == E_TM_1_2
        && pBsMacDesc->initParm.channelBandwidth == BW_1_4_MHZ
        && (getSubFrameNum() == 0 || getSubFrameNum() == 5))
        power = 0;
#endif

    pCh->txpowerControl = power;

    pCh->persistEnable = 0x01;
    pCh->repeatCycle =  0x01;

    pCh->channelType = PDSCH;
    pCh->hCid = id;

    pCh->distributedFlag = pstatic_config->distributed_flag;
    pCh->nGapFlag        = pstatic_config->n_gap_flag;

    pCh->nLayers = pTestConfConfig->nLayers;

    pCh->transmissionMode = pTestConfConfig->transmissionMode;

    pCh->crcInfo.crcLength = TWTYFOUR;
    pCh->crcInfo.crcScrambling = 0;
    pCh->scrInfo.scramblerType = DLSCR;
    pCh->scrInfo.scrinitValueinput =  rnti;

    pCh->dlPrecoderInfo.cddType = pTestConfConfig->cddType;


// check if enable update cqi/pmi/ri
    if (plte_pdsch_chan_state->ena_dynamic_cqi_pmi_ri == 1)
    {

// ------------ cqi/pmi ------------
        if (plte_pdsch_chan_desc->ena_update_cqi_pmi == 1)
        {
            if (get_globalTTITickCount() == plte_pdsch_chan_desc->lte_pdsch_frame_num)
            {
// update cbi
                plte_pdsch_chan_desc->dl_precoder_info.codeBookIdx = plte_pdsch_chan_desc->dl_precoder_info_updating.codeBookIdx;

// update mcs_index
//                plte_pdsch_chan_desc->sub_ch_info[0].mcsType = plte_pdsch_chan_desc->sub_ch_info_updating[0].mcsType;
//                plte_pdsch_chan_desc->sub_ch_info[1].mcsType = plte_pdsch_chan_desc->sub_ch_info_updating[1].mcsType;

// copy mcs_parameters to testConfConfig
                ptest_cfg->pdschMcs[0][pdsch_idx] = plte_pdsch_chan_desc->sub_ch_info_updating[0].mcsType;
                ptest_cfg->pdschMcs[1][pdsch_idx] = plte_pdsch_chan_desc->sub_ch_info_updating[1].mcsType;

            }
            pCh->dlPrecoderInfo.codeBookIdx = plte_pdsch_chan_desc->dl_precoder_info.codeBookIdx;

// The mcs will update in the LtePhyFillPdschSubDesc-function (It's next call)
//            pCh->subChInfo[0].mcsType = plte_pdsch_chan_desc->sub_ch_info[0].mcsType;
//            pCh->subChInfo[1].mcsType = plte_pdsch_chan_desc->sub_ch_info[1].mcsType;
        }
        else
        {
            pCh->dlPrecoderInfo.codeBookIdx = pTestConfConfig->codeBookIdx;
        }

// ------------ ri ------------
        if (plte_pdsch_chan_desc->ena_update_ri == 1)
        {
            if (get_globalTTITickCount() == plte_pdsch_chan_desc->lte_pdsch_frame_num)
            {
                plte_pdsch_chan_desc->numCodeWords = plte_pdsch_chan_desc->numCodeWords_updating;
            }
            pCh->numCodeWords = plte_pdsch_chan_desc->numCodeWords;
        }
        else
        {
            pCh->numCodeWords = pTestConfConfig->nCw;
        }
    }
    else
    {
        pCh->numCodeWords = pTestConfConfig->nCw;
        pCh->dlPrecoderInfo.codeBookIdx = pTestConfConfig->codeBookIdx;
    }

    pCh->dlPrecoderInfo.reserved = NOT_SPECIFIED_VALUE;
    pCh->ueCategory = pTestConfConfig->PdschCfgEna ? pTestConfConfig->PdschCfg[pdsch_idx].ueCategory : 1;

#ifdef MX_ARM_ENABLED
    for (i = 0; i < sizeof(pCh->dlPrecoderInfo.codebookSubsetRestriction) /
        sizeof(pCh->dlPrecoderInfo.codebookSubsetRestriction[0]); i++)
#else
    for (i = 0; i < 8; i++)
#endif
    {
        pCh->dlPrecoderInfo.codebookSubsetRestriction[i] = 0;
    }

    memcpy(&pCh->mapInfo, pMapInfo, sizeof(MAPPINGINFO));

    return SUCCESS;
}

/** Return required size of transmit block
 *
 * @param mcsIndex MCS index
 * @param nbRB Number of resource blocks
 * @return Transmit block size
 */
U32 lte_pdsch_get_tb_size(U32 mcsIndex, U32 nbRB, U8 TxMode)
{
    U8 ibsIndex = 0;
    U32 tbSizeInBits = 0;

    ibsIndex = TBSArr[mcsIndex];

    if (!nbRB)
    {
        printf("%s:[err]: nbRB = 0\n", __FUNCTION__);
        return 0;
    }    
    tbSizeInBits = TBSizeArr[ibsIndex][nbRB - 1];

    if(TxMode == OPNLPSPMUX)
    {
        if(nbRB > 55)
        {
            U32 Idx;
            for(Idx = 0; Idx < 28*4; Idx++)
            {
                if(TBSizeArrSpaMux[Idx][0] == tbSizeInBits)
                {
                    tbSizeInBits = TBSizeArrSpaMux[Idx][1] / 2;
                    break;
                }
            }
        }
    }
    return tbSizeInBits;
}


/** Get sdu-size for PDSCH channel in bits
 *
 * @param pSubCh Pointer to PDLSUBCHINFO
 * @param pMapInfo pointer to MapInfo
 * @param TxMode type of TxMode (SINGLEANT = 1, TXDIVERSITY, OPNLPSPMUX, CLSLPSPMUX, MUMIMO, CLSLPR1PC, SINGLEANTP5)
 * @return Sdu size in bits
 */
U32 lte_pdsch_get_sdu_size(PDLSUBCHINFO pSubCh, MAPPINGINFO *pMapInfo, U8 TxMode)
{
    U32 nResElm = 0;
    U32 i;
    PRINT_FUNCTION_NAME(__FUNCTION__);

    for (i = 0; i < pMapInfo->numberofEntries; i++)
        nResElm += pMapInfo->reselmInfo[i].numRes;

    return lte_pdsch_get_tb_size(pSubCh->mcsType, nResElm, TxMode);
}


/** Inserts PDSCH channel containing all zeroes into scheduler queue
 *
 * @param numBits Number of bits in SDU payload
 * @param chId PHY channel ID
 * @param channelType (PDSCH, see enum ChTypeOptions)
 * @param Code word index
 * @return Standard result code
 */
RESULTCODE lte_pdsch_create_sdu(U8* payload, U32 num_bits, U8 ch_id, U8 channel_type, U32 cw_id, U32 ones)
{
    U8 *pSdu = NULL;
#ifdef MX_ARM_ENABLED
    PTXSDUREQ  pHdr = (PTXSDUREQ) Mac2PhyIfAllocBuffer();
    PTXSDUREQ  pSduReq = pHdr;
#else
    PMSGHEADER pHdr = MsgAllocBuffer(MSG_FAST);
    PTXSDUREQ  pSduReq = (PTXSDUREQ)LteMac2PhyGetDataOffset((PTR)pHdr);
#endif

    if (pSduReq == NULL)
    {
        printf("Failed to allocate PTXSDUREQ, LtePhyPrepareTestZeroSdu function\r\n");
        return FAILURE;
    }

    pSduReq->phyEntityId = 0;
    pSduReq->msgType = PHY_TXSDU_REQ;
    pSduReq->chanId = ch_id;
    pSduReq->cwId = cw_id;
    pSduReq->uciFormat = 0;
    pSduReq->channelType = channel_type;
    pSduReq->phichGrpNumber = 0;
    pSduReq->maxBitsperCw = num_bits;
    pSduReq->msgLen = num_bits >> 3;
    pSduReq->pTxSdu = 0;

    if (pSdu == NULL)
    {
        if (ones == 2)
        {
            memcpy((U8*) ((U8*)pSduReq + sizeof(TXSDUREQ)), payload, (num_bits+7) >> 3);            
        }
        else
        {
            U8 Pattern = (payload)? *payload: ((ones)? 0xFF: 0);

            memset((U8*) ((U8*) pSduReq + sizeof(TXSDUREQ)), Pattern, (num_bits+7) >> 3);
        }

        if (pBsMacDesc->initParm.rxSduLineMonitorInfoenable)
        {
            U8 TimAdvByte = TestMgrIsTimAdv(&testConfConfig, getSubFrameNum());
            if (TimAdvByte)
            {
                MAC_HEADER_CTRL *pMacHdr;
                U8* pPayload;
                pMacHdr = (MAC_HEADER_CTRL*) ((U8*) pSduReq + sizeof(TXSDUREQ));
                pMacHdr->E = 0;
                pMacHdr->R = 0;
                pMacHdr->LCID = LCID_ADVANCE;
                pPayload = (U8*) pMacHdr + sizeof(MAC_HEADER_CTRL);
                *pPayload = TimAdvByte;
            }
        }
    }
    else
    {
        // Loop back first RxSdu
        memcpy((U8*) ((U8*) pSduReq + sizeof(TXSDUREQ)), pSdu, num_bits >> 3);
    }

#ifdef MX_ARM_ENABLED
// 4gmx
    return SduSend(PHY_TXSDU_REQ | (channel_type << 4), (U8*) pSduReq, pSduReq->msgLen + sizeof(TXSDUREQ));
#else
    #ifdef WIN32
// ccode
        OsalEnqueue(&dlControlTxSduQueue2, (OSAL_PADDRESS)pHdr);
        return SUCCESS;
    #else
// linux
        return SduSend(PHY_TXSDU_REQ, (U8*) pSduReq, pSduReq->msgLen + sizeof(TXSDUREQ));
    #endif
#endif
}


/** Create PDSCH channel
 *
 * @param pointer to TestConfConfig-structure
 * @param pDlSubFrame pointer to PDLSUBFRDESC
 * @param pointer to mapinfo
 * @param pointer to channel index
 * @param Pdsch index
 * @param boosting type
 * @return Standard result code
 */
RESULTCODE lte_pdsch_create(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                            PDLSUBFRDESC pDlSubFrame,
                            PMAPPINGINFO pmapInfo,
                            U8 *chanIdx,
                            U8 Pdsch_idx,
                            U8 boosted)
{
    U32         ones;
    U32         i;
    PDLCHANDESC pdlCh;
    U8          Modulation;
    U8          mcsType;
    U16         rnti;
    RESULTCODE  res;
    U8          chIdx = *chanIdx;
    S16         Power;
    U8          harq_id=0;

    PBS_STATIC_CONFIG pStaticConfig = get_static_config();
    pdlCh = &pDlSubFrame->dlCh[chIdx - pDlSubFrame->numCtrlChannels];
    if (pTestConfConfig->PdschCfgEna)
    {
        lte_pdsch_setmapinfo_direct(pTestConfConfig, pmapInfo, Pdsch_idx);
        rnti = pTestConfConfig->PdschCfg[Pdsch_idx].Rnti;
        Power = (S16)pTestConfConfig->PdschCfg[Pdsch_idx].Pa;
    }
// save old capability for TestConfigurations
    else
    {    
        res = boosted ?
            pTestMgrGetBoostedPdsch(pTestConfConfig, pmapInfo, Pdsch_idx, 0 ) :
            pTestMgrGetDeBoostedPdsch(pTestConfConfig, pmapInfo, Pdsch_idx, 0 );

        if ( res != SUCCESS )
            return res;
        
        if ( pTestConfConfig->pdschRnti_used )
            rnti = pTestConfConfig->pdschRnti[Pdsch_idx];
        else
            rnti = boosted ? lte_rnti_get_boosted_rnti  (pTestConfConfig, 0) :
                             lte_rnti_get_deboosted_rnti(pTestConfConfig, 0);

        Power = lte_pdsch_get_power(pTestConfConfig, Pdsch_idx, 0);
    }
 
    lte_pdsch_add(pTestConfConfig,
                  pdlCh,
                  chIdx&0xff,
                  Pdsch_idx, 
                  pmapInfo,
                  rnti,
                  Power);

    if ((harq_id = lte_harq_get_harq_id(Pdsch_idx, globalTTITickCount)) == 0xff)
    {
        if (globalTTITickCount>6)
        {
//            uart_printf("wrong harq_id for tti: %d, PDSCH-id: %d\n", globalTTITickCount, Pdsch_idx);
            return FAILURE;
        }
    }

    // Fill Subchan Desc
    for(i = 0; i < pTestConfConfig->nCw; i++)
    {
        if (pTestConfConfig->PdschCfgEna)
        {
            Modulation = pTestConfConfig->PdschCfg[Pdsch_idx].CwCfg[i].Mod;
            mcsType = pTestConfConfig->PdschCfg[Pdsch_idx].CwCfg[i].Mcs;
        }
// save old capability for TestConfigurations
        else
        {        
            if(pTestConfConfig->pdschMcs_used[i])
            {
                mcsType = pTestConfConfig->pdschMcs[i][Pdsch_idx];
                Modulation = lte_pdsch_conv_mcs_to_modulation(mcsType);
            }
            else
            {
                U8 layer_idx = boosted ? ((U8)i) : 0;
                Modulation = lte_pdsch_get_modulation(pTestConfConfig, Pdsch_idx, layer_idx);
                mcsType = lte_modulation_get_mcs_index_dl(Modulation, (U16)i);
            }
        }

        lte_pdsch_fill_sub_desc(pTestConfConfig,
                                &pdlCh->subChInfo[i],
                                Pdsch_idx,
                                harq_id,
                                Modulation,
                                mcsType,
                                i);

        if(pTestConfConfig->pdschPayloadMode[i][Pdsch_idx])
            ones = 1;
        else
            ones = 0;

        /* Generate PHY TXSDU.req for this channel */
#ifdef MX_ARM_ENABLED
        lte_pdsch_create_sdu((pTestConfConfig->pdschPayloadEna == 1 )? &pTestConfConfig->pdschPayload:NULL,
                             lte_pdsch_get_sdu_size(&pdlCh->subChInfo[i], pmapInfo, (U8)pdlCh->transmissionMode),
                             chIdx, 
                             PDSCH, 
                             i,
                             ones);
#else
        if(MacRtMode == MACMODE_NRT)
        {
/*            LtePhyPrepareTestSdu((pTestConfConfig->pdschPayloadEna == 1 )? &pTestConfConfig->pdschPayload:NULL, 
                                 TestMgrCalculateDataSduSize(&pdlCh->subChInfo[i], pmapInfo, (U8)pdlCh->transmissionMode),
                                 chIdx,
                                 PDSCH, 
                                 i);*/
            lte_pdsch_create_sdu((pTestConfConfig->pdschPayloadEna == 1 )? &pTestConfConfig->pdschPayload:NULL,
                                 lte_pdsch_get_sdu_size(&pdlCh->subChInfo[i], pmapInfo, (U8)pdlCh->transmissionMode),
                                 chIdx, 
                                 PDSCH, 
                                 i,
                                 ones);
        }
        else
        {
            PLTE_PDSCH_HARQ_STATE plte_pdsch_harq_state = get_lte_pdsch_harq_state();

            if (plte_pdsch_harq_state->ena_udp_stream)
            {
                BS_CONFOR_TEST_CONFIG       *pconf_test_config = get_bs_conf_test_config();
                U32                         num_bits = lte_pdsch_get_sdu_size(&pdlCh->subChInfo[i], pmapInfo, 0);
                PLTE_MAC_STATE              plte_mac_state = get_lte_mac_state();
                U32                         current_harq_tick = (plte_mac_state->lte_pdsch_harq_state.harq_count_cnt)&0x07;
                PLTE_PDSCH_SDU_CTRL_RESEND  ppdsch_sdu_ctrl_resend = get_lte_pdsch_sdu_ctrl_resend(i, Pdsch_idx, current_harq_tick);
                PLTE_PDSCH_SDU_PAYLOAD      plte_pdsch_sdu_payload = get_lte_pdsch_sdu_payload(i, Pdsch_idx, current_harq_tick);


    // if not ack -> copping udp_data to sdu_payload
                if (ppdsch_sdu_ctrl_resend->need_resend == NACK_HARQ_OK)
                {
                    plte_pdsch_sdu_payload->len = num_bits;
                    plte_pdsch_set_payload((U8*)&plte_pdsch_sdu_payload->payload[0], num_bits>>3);
                }

                pdlCh->subChInfo[i].rV  = ppdsch_sdu_ctrl_resend->rv;
                pdlCh->subChInfo[i].nDi = ppdsch_sdu_ctrl_resend->need_resend;

//                if (pdsch_sdu_payload[i][Pdsch_idx][current_tti_tick].len)
                {
//    LtePhyPrepareTestSduRt
                    lte_pdsch_create_sdu( (U8*)&plte_pdsch_sdu_payload->payload[0], 
                                           num_bits, //pdsch_sdu_payload[i][Pdsch_idx][current_tti_tick].len,
                                           chIdx,
                                           PDSCH, 
                                           (U8)i,
                                           2);
                }
            }
            else
            {
//    LtePhyPrepareTestSduRt
                lte_pdsch_create_sdu( (pTestConfConfig->pdschPayloadEna == 1 )? &pTestConfConfig->pdschPayload:NULL,
                                       lte_pdsch_get_sdu_size(&pdlCh->subChInfo[i], pmapInfo, (U8)pdlCh->transmissionMode),
                                       chIdx,
                                       PDSCH, 
                                       (U8)i,
                                       0);
            }
        }
#endif

    }
    chIdx++;
    *chanIdx = chIdx;

    return SUCCESS;
}


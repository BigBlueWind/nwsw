/** @file lte_pusch.c
 *
 * @brief Implemenataion for processing PUSCH message & some service functions for mapinfo
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "LtePhyL2Api.h"
#include "resultcodes.h"
#include "LteMacTestCfgMgr.h"

#ifdef MX_ARM_ENABLED
#include "enbMac2PhyIface.h"
#else
#include "LteMac2PhyInterface.h"
#endif

#include "lte_mac_typedef.h"
#include "lte_mac_state.h"
#include "lte_pdsch.h"
#include "lte_pusch.h"

#include "lte_rnti.h"

extern tGetRvi pGetRvi;

/* update cqi/pmi/ri/harq into plte_pdsch_chan_desc-structure
 *
 * @param pmsg Pointer to RxStatus-message
 * @param prx_status_ind Pointer to RxStatusInd
 * @return Standard result code
 */
RESULTCODE lte_pusch_update_cqi_pmi_ri_harq(PRX_CQIRIHI_STATUS_EVENT pmsg, PRXSTATUSIND prx_status_ind)
{
    PLTE_PDSCH_CHAN_DESC     plte_pdsch_chan_desc = get_lte_pdsch_chan_desc(pmsg->chanId);

    U8  *ptr;
    U8  cqi_pmi[64]={0};
    U8  ri=0, harq=0;
    S8  len;

// The format for the status message is cqi/pmi bytes first if available, 
// followed by single byte of ri if available followed by single byte of harq if available. 
// The information is left justified and starts at the Most significant bit which corresponds to the last in time

// set pointer to last status byte: harq
    ptr = (U8*)((U32)pmsg + pmsg->status_len + sizeof(PRX_CQIRIHI_STATUS_EVENT));
    len = (S8)pmsg->status_len;
    if (pmsg->harq_pt)
    {
        memcpy(&harq, ptr, 1);
        ptr--;
        len--;
    }
    
    if (pmsg->ri_pt)
    {
        plte_pdsch_chan_desc->ena_update_ri = 1;
        memcpy(&ri, ptr, 1);
        ptr--;
        len--;
    }


    if (pmsg->cqi_pmi_pt)
    {
        BS_CONFOR_TEST_CONFIG*  ptest_cfg = get_bs_conf_test_config();

        plte_pdsch_chan_desc->ena_update_cqi_pmi = 1;
        ptr = ptr - len + 1;

        if (len > sizeof(cqi_pmi))
            printf("%s: incoming data size %d > local buffer size %d\n", __FUNCTION__, len, sizeof(cqi_pmi));

        if(MIN(len, sizeof(cqi_pmi)))
            memcpy(&cqi_pmi, ptr, MIN(len, sizeof(cqi_pmi)));

// rank=1
        if (ri==0)
        {
            plte_pdsch_chan_desc->sub_ch_info_updating[0].mcsType = lte_pdsch_calc_mcs_via_cqi(cqi_pmi[0] & 0x0F);
            plte_pdsch_chan_desc->dl_precoder_info_updating.codeBookIdx = (cqi_pmi[0] >> 4) & 0x03;
            plte_pdsch_chan_desc->numCodeWords_updating = 1;
        }

// rank=2
        if (ri==1)
        {
            plte_pdsch_chan_desc->sub_ch_info_updating[0].mcsType = lte_pdsch_calc_mcs_via_cqi( cqi_pmi[0]     & 0x0F);
            plte_pdsch_chan_desc->sub_ch_info_updating[1].mcsType = lte_pdsch_calc_mcs_via_cqi((cqi_pmi[0]>>4) & 0x0F);
            plte_pdsch_chan_desc->dl_precoder_info_updating.codeBookIdx = (cqi_pmi[1] & 0x01) + 1;
            plte_pdsch_chan_desc->numCodeWords_updating = 2;
        }
        plte_pdsch_chan_desc->lte_pdsch_frame_num = (prx_status_ind->frameNum*10 + prx_status_ind->subFrameNum) + 4;
    }

    if (harq & 0x20)
// The harq was correct reveived and decoded
    {
        PLTE_PDSCH_SDU_CTRL_RESEND ppdsch_sdu_ctrl_resend;
        U32 current_harq_id = (prx_status_ind->frameNum*10 + prx_status_ind->subFrameNum)&0x07;
// codeword0 -- need to check
        ppdsch_sdu_ctrl_resend = get_lte_pdsch_sdu_ctrl_resend(0, pmsg->chanId, current_harq_id);
        ppdsch_sdu_ctrl_resend->nack = (harq >> 7) & 1;

// codeword1 -- need to check
        ppdsch_sdu_ctrl_resend = get_lte_pdsch_sdu_ctrl_resend(1, pmsg->chanId, current_harq_id);
        ppdsch_sdu_ctrl_resend->nack = (harq >> 6) & 1;
    }

    return SUCCESS;
}


/** get power value for PUSCH channel
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @return power in 2Q8 format
 */
S16 lte_pusch_get_power(BS_CONFOR_TEST_CONFIG *pTestConfConfig)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return ((S16)pTestConfConfig->puschBoostedPrbPa);
}

/** Convert Mcs to modulation index for PUSCH channel
 *
 * @param  Mcs Type
 * @return Modulation value
 */
U8 lte_pusch_conv_mcs_to_modulation(U8 mcs)
{
    if(mcs <= 10)
        return QPSK;
    else if(mcs <= 20)
        return QAM16;
    else if(mcs <= 28)
        return QAM64;
    else
        return 0xFF;
}

/** Get modulation value for PUSCH channel
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @return Modulation value
 */
U8 lte_pusch_get_modulation(BS_CONFOR_TEST_CONFIG *pTestConfConfig)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return pTestConfConfig->puschBoostedMod;
}


/** Get rv index
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @return rv index
 */
U8 lte_pusch_rvi(BS_CONFOR_TEST_CONFIG *pTestConfConfig)
{
#ifdef MX_ARM_ENABLED
    U8 rvi = pTestConfConfig->arRctRv[pTestConfConfig->curRctRv];
    pTestConfConfig->curRctRv = (++pTestConfConfig->curRctRv) % pTestConfConfig->nRctRv;
#else
    U8 rvi = pTestConfConfig->arRctRv[0];
#endif
    return rvi;
}

/** Get default rv index (0)
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @return rv index
 */
U8 lte_pusch_rvi_default(BS_CONFOR_TEST_CONFIG *pTestConfConfig)
{
    U8 rvi = 0;
    return rvi;
}


/* update mapinfo for PUSCH
 *
 * @param pTestConfConfig Pointer to TestConfConfig
 * @param pMapInfo        Pointer to mapinfo
 * @param pMapInfoStore   Pointer to MapInfoStore
 * @return Standard result code
 */
RESULTCODE lte_pusch_set_mapinfo(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                MAPPINGINFO *pMapInfo, 
                                MAPPINGINFO *pMapInfoStore)
{
    const U8 **pBoostingMap;
    U8 subFramNum;
    const U8 *pMap;
    U32 cur_num;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    if (pTestConfConfig->nPuschBoostedPrb == 0)
        return FAILURE;

    memset(pMapInfo, 0, sizeof(MAPPINGINFO));

    subFramNum = getSubFrameNum();

    if (testMgrCtx.boostingUlMap == NULL)
    {
        printf("ERROR: boostingMap has bad ptr [%s,%s:%d]\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }

    pBoostingMap = testMgrCtx.boostingUlMap;
    pMap = pBoostingMap[subFramNum];
    cur_num = pTestConfConfig->nPuschBoostedPrb;

    if(pMapInfoStore->numberofEntries != 0)
    {
       pMap += pMapInfoStore->reselmInfo[pMapInfoStore->numberofEntries - 1].startRes;
    }

    while (*pMap != 0xFF)
    {
        pMapInfo->reselmInfo[pMapInfo->numberofEntries].startRes = *pMap;
        pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes = TestMgrGetNumConsecutiveRBs((U8**)&pMap, &cur_num);
        pMapInfo->numberofEntries++;
        if(cur_num == 0)
            break;
    }

    if (pMapInfo->numberofEntries)
    {
        // store last mapInfo offset for next PUSCH
        pMapInfoStore->reselmInfo[pMapInfoStore->numberofEntries].startRes = (U8)(pMap - pBoostingMap[subFramNum]);
        pMapInfoStore->numberofEntries++;
        return SUCCESS;
    }
    else
        return FAILURE;

}


/* update mapinfo for PUSCH (Direct from TestConfConfig)
 *
 * @param pTestConfConfig Pointer to TestConfConfig
 * @param pMapInfo        Pointer to mapinfo
 * @param pMapInfoStore   Pointer to MapInfoStore
 * @return Standard result code
 */
RESULTCODE lte_pusch_set_mapinfo_direct(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                        MAPPINGINFO *pMapInfo, 
                                        MAPPINGINFO *pMapInfoStore)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    memset(pMapInfo, 0, sizeof(MAPPINGINFO));

    pMapInfo->reselmInfo[0].startRes = pTestConfConfig->pusch_map[pMapInfoStore->numberofEntries].startRes;
    pMapInfo->reselmInfo[0].numRes = pTestConfConfig->pusch_map[pMapInfoStore->numberofEntries].numRes;
    pMapInfo->numberofEntries++;
    pMapInfoStore->numberofEntries++; // track PUSCH index

    return SUCCESS;
}


/* Function addes sub PUSCH channel to TXVECTOR
 *
 * @param pTestConfConfig Poiter to TestConfConfig
 * @param pCh Pointer to UlChanDesc into RxVector
 * @param Modulation
 * @param Mcs Type
 * @param Pointer to mapinfo
 * @param uplink index
 * @return Standard result code
 */
RESULTCODE lte_pusch_fill_sub_channel(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                      PULSUBCHINFO pSubCh,
                                      U8 modulation,
                                      U8 mcsType,
                                      MAPPINGINFO *pMapInfo,
                                      U8 idx)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    pSubCh->mcinfo.modulationType = modulation;
    pSubCh->mcinfo.codingDescriptor = TURBOCDR;
    pSubCh->mcinfo.blockCodeConcatenation = 1;
    pSubCh->mcinfo.mcsType = mcsType;
    pSubCh->harqInfo.rV = pGetRvi(pTestConfConfig);
    pSubCh->harqInfo.nDi = (pSubCh->harqInfo.rV != 0)? 0 : 1;
    pSubCh->harqInfo.flushReq = 0;
    pSubCh->crcInfo.crcLength = TWTYFOUR;
    pSubCh->crcInfo.crcScrambling = 0;
    pSubCh->scrInfo.scramblerType = ULSCR;
    pSubCh->scrInfo.scrinitValueinput = lte_rnti_get_pusch_rnti(pTestConfConfig, idx);

    pSubCh->puschDed.nDMRS2 = (pTestConfConfig->pusch_nDMRS2_used)?
                               pTestConfConfig->pusch_nDMRS2[idx] : pTestConfConfig->puschDed.nDMRS2;

    pSubCh->puschDed.betaOffsetCQIIndex = pTestConfConfig->betaOffsetCQIIndex;
    pSubCh->puschDed.nsymi = pTestConfConfig->nsymi;

    if(pTestConfConfig->PuschRiIdxUsed)
        pSubCh->puschDed.betaOffsetRIIndex = pTestConfConfig->PuschRiIdx[idx];

    if(pTestConfConfig->PuschCqiIdxUsed)
        pSubCh->puschDed.betaOffsetCQIIndex = pTestConfConfig->PuschCqiIdx[idx];

    if(pTestConfConfig->Pusch_nRiUsed)
    {
        if(pTestConfConfig->ena_harq || pTestConfConfig->ena_dynamic_cqi_pmi_ri)
            pSubCh->puschDed.nRI = pTestConfConfig->Pusch_nRi[idx][getSubFrameNum()];
        else // backward compatibility with one-dimension array
           pSubCh->puschDed.nRI = *((U8*)pTestConfConfig->Pusch_nRi + idx);
    }

    if(pTestConfConfig->Pusch_nrlCqiUsed)
    {
        if(pTestConfConfig->ena_harq || pTestConfConfig->ena_dynamic_cqi_pmi_ri)
            pSubCh->puschDed.nr1CQI = pTestConfConfig->Pusch_nrlCqi[idx][getSubFrameNum()];
        else // backward compatibility with one-dimension array
            pSubCh->puschDed.nr1CQI = *((U8*)pTestConfConfig->Pusch_nrlCqi + idx);
    }

    if( pTestConfConfig->Pusch_nrglCqiUsed )
        pSubCh->puschDed.nrg1CQI = pTestConfConfig->Pusch_nrglCqi[idx];


    if(pTestConfConfig->Pusch_nAckUsed)
    {
        if(pTestConfConfig->ena_harq || pTestConfConfig->ena_dynamic_cqi_pmi_ri)
            pSubCh->puschDed.nACK = pTestConfConfig->Pusch_nAck[idx][getSubFrameNum()];
        else // backward compatibility with one-dimension array
            pSubCh->puschDed.nACK = *((U8*)pTestConfConfig->Pusch_nAck + idx);
    }
    else
        pSubCh->puschDed.nACK = pTestConfConfig->puschDed.nACK;

    if( pTestConfConfig->PuschAckUsed)
        pSubCh->puschDed.betaOffsetACKIndex = pTestConfConfig->PuschAckIdx[idx];
    else
        pSubCh->puschDed.betaOffsetACKIndex = pTestConfConfig->puschDed.betaOffsetACKIndex;


//    if(pSubCh->puschDed.nACK)
    {
        U32 i, nRB = 0;

        for(i = 0; i < pMapInfo->numberofEntries; i++)
        {
            nRB += pMapInfo->reselmInfo[i].numRes;
        }
        
        pSubCh->puschDed.nrbi = nRB;
    }

    pSubCh->reserved = NOT_SPECIFIED_VALUE;

    memcpy(&pSubCh->mapInfo, pMapInfo, sizeof(MAPPINGINFO));

    return SUCCESS;
}


/* Function addes PUSCH channel to Subframe with Map According to configuration
 *
 * @param pointer to TestConfConfig-structure
 * @param pointer to DLCHANDESC-structure
 * @param total index of channel
 * @param power value
 * @return Standard result code
 */
RESULTCODE lte_pusch_add(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                         PULCHANDESC pCh,
                         U32 id,
                         S16 power)

{
    PTIMADVERRINF timAdvErrInfo = &pCh->timAdvErrInfo;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    pCh->offsetNextCh = NOT_SPECIFIED_VALUE;
    pCh->channelId = (U16)id;
    pCh->txpowerControl = power;

    pCh->persistEnable = 1;
    pCh->repeatCycle =  1;

    pCh->channelType = PUSCH;
    pCh->halfIterations = 16;

    pCh->ulSubChInfo.puschDed.nRI = pTestConfConfig->pusch_ri[id];
    pCh->ulSubChInfo.puschDed.nACK  = pTestConfConfig->pusch_nack[id];

    if (pCh->ulSubChInfo.puschDed.nRI == 1)
    {
        pCh->ulSubChInfo.puschDed.nr1CQI = pTestConfConfig->pusch_nCQI[id];
    }
    else
    {
        pCh->ulSubChInfo.puschDed.nrg1CQI = pTestConfConfig->pusch_nCQI[id];
    }

    timAdvErrInfo->mErrAvgCh[0] = pTestConfConfig->timAdvErrInfo[PUSCH - PUSCH].mErrAvgCh[0];
    timAdvErrInfo->mErrAvgCh[1] = pTestConfConfig->timAdvErrInfo[PUSCH - PUSCH].mErrAvgCh[1];
    timAdvErrInfo->mErrExpo = pTestConfConfig->timAdvErrInfo[PUSCH - PUSCH].mErrExpo;
    timAdvErrInfo->mSetDefaults = pTestConfConfig->timAdvErrInfo[PUSCH - PUSCH].mSetDefaults;

    if(pTestConfConfig->timAdvErrInfoArr_get[id] == pTestConfConfig->timAdvErrInfoArr_put[id])
    {// buffer is empty, set default
        timAdvErrInfo->mSetDefaults = 1;
    }
    else
    {// buffer is not empty

// TODO:
#ifdef MX_ARM_ENABLED
        U32 ReadIdx;
        PTIMADVERRINF_FSF pBuf;
        U32 RxVecFSF = getFrameNum() * 10 + getSubFrameNum();
        U32 RxSduFSF;
        U32 DeltaSF = 0;

        while(pTestConfConfig->timAdvErrInfoArr_put[id] != pTestConfConfig->timAdvErrInfoArr_get[id])
        {// look the buffer
            ReadIdx = pTestConfConfig->timAdvErrInfoArr_get[id];
            pBuf = &pTestConfConfig->timAdvErrInfoArrFSF[ReadIdx][id];
            RxSduFSF = pBuf->Fnumber * 10 + pBuf->SFnumber;

            if(++ReadIdx >= TIMERRBUFDEPTH)
                ReadIdx = 0;
            pTestConfConfig->timAdvErrInfoArr_get[id] = ReadIdx;

            DeltaSF = (RxVecFSF >= RxSduFSF)? (RxVecFSF - RxSduFSF) : ((U32)((U32)0xffff * 10 + 9) - RxSduFSF + RxVecFSF + 1);
            if(DeltaSF == TIMERRINFO_DELAY_SF)
            {// found, apply
                timAdvErrInfo->mErrAvgCh[0] = pBuf->mErrAvgCh[0];
                timAdvErrInfo->mErrAvgCh[1] = pBuf->mErrAvgCh[1];
                timAdvErrInfo->mErrExpo = pBuf->mErrExpo;
                timAdvErrInfo->mSetDefaults = 0;
                break;
            }
        }

        if(DeltaSF != TIMERRINFO_DELAY_SF)
        {// not found, set default
            timAdvErrInfo->mSetDefaults = 1;
        }
#else
        U32 BufferLevel;
        if(pTestConfConfig->timAdvErrInfoArr_put[id] > pTestConfConfig->timAdvErrInfoArr_get[id])
            BufferLevel = pTestConfConfig->timAdvErrInfoArr_put[id] - pTestConfConfig->timAdvErrInfoArr_get[id];
        else
            BufferLevel = TIMERRBUFDEPTH - pTestConfConfig->timAdvErrInfoArr_get[id] + pTestConfConfig->timAdvErrInfoArr_put[id];

        if(BufferLevel >= TIMERRINFO_DELAY_SF)
        {
            U32 ReadIdx = pTestConfConfig->timAdvErrInfoArr_get[id];
            timAdvErrInfo->mErrAvgCh[0] = pTestConfConfig->timAdvErrInfoArrFSF[ReadIdx][id].mErrAvgCh[0];
            timAdvErrInfo->mErrAvgCh[1] = pTestConfConfig->timAdvErrInfoArrFSF[ReadIdx][id].mErrAvgCh[1];
            timAdvErrInfo->mErrExpo = pTestConfConfig->timAdvErrInfoArrFSF[ReadIdx][id].mErrExpo;
            timAdvErrInfo->mSetDefaults = 0;
            
            if(++ReadIdx>= TIMERRBUFDEPTH)
                ReadIdx = 0;
            pTestConfConfig->timAdvErrInfoArr_get[id] = (U8)ReadIdx;
        }
        else
        {
            timAdvErrInfo->mSetDefaults = 1;
        }
#endif

    }

    if(pTestConfConfig->timAdvErrDflt_pusch_len == 1)
    {// applied only to current subframe, is not supported for RT
        //timAdvErrInfo->mSetDefaults = pTestConfConfig->timAdvErrDflt_pusch[0];
        pTestConfConfig->timAdvErrDflt_pusch_len = 0;
    }
    else if(pTestConfConfig->timAdvErrDflt_pusch_len)
    {
        timAdvErrInfo->mSetDefaults = 
            pTestConfConfig->timAdvErrDflt_pusch[(getFrameNum() * 10 + getSubFrameNum()) 
                % sizeof(pTestConfConfig->timAdvErrDflt_pusch)];
    }

    return SUCCESS;
}



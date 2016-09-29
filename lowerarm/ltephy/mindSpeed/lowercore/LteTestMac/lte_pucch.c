/** @file lte_pucch.c
 *
 * @brief Implemenataion for creating PUCCH message
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "LtePhyL2Api.h"
#include "resultcodes.h"
#include "MacUtils.h"
#include "LteMacTestCfgMgr.h"

#include "lte_mac_typedef.h"

#include "lte_mac_state.h"
#include "lte_pdsch.h"
#include "lte_pucch.h"

#include "lte_rnti.h"

extern tTestMgrGetPucch pTestMgrGetPucch;

/** Set mapinfo for PUCCH channel
 *
 * @param pTestConfConfig Pointer to TestConfConfig
 * @param pmapinfo Pointer for results storage
 * @param id picch index
 * @return Standard result code
 */
RESULTCODE lte_pucch_set_mapinfo(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                 MAPPINGINFO *pMapInfo, 
                                 U32 id)
{
    const U8 **pBoostingMap;
    U8 subFramNum;
    const U8 *pMap;
    U32 cur_num, idx;
    
    PRINT_FUNCTION_NAME(__FUNCTION__);
    
    memset(pMapInfo, 0, sizeof(MAPPINGINFO));
    
    subFramNum = getSubFrameNum();
    
    if (testMgrCtx.boostingUlMap == NULL)
    {
        printf( "ERROR: boostingMap has bad ptr [%s,%s:%d]\n",
                __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }
    
    pBoostingMap = testMgrCtx.boostingUlMap;
    pMap = pBoostingMap[subFramNum];
        cur_num = 1;//pTestConfConfig->ulSfrCtrl.n1PucchAN;
    idx = 0;

    // TODO: all PUCCH are in one RB for now
    id = 0;

    // make sure we're adding PUCCH before PUSCH
    while (pMap[idx + cur_num*(id+1)]  != 0xFF)
    {
         idx++;
    }

    pMapInfo->reselmInfo[pMapInfo->numberofEntries].startRes = pMap[idx];
    pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes = (U8)cur_num;
    pMapInfo->numberofEntries++;
    
    return SUCCESS;
}


/** Set mapinfo for PUCCH channel(Direct)
 *
 * @param pTestConfConfig Pointer to TestConfConfig
 * @param pmapinfo Pointer for results storage
 * @param id picch index
 * @return Standard result code
 */
RESULTCODE lte_pucch_set_mapinfo_direct(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                        MAPPINGINFO *pMapInfo, 
                                        U32 id)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    memset(pMapInfo, 0, sizeof(MAPPINGINFO));

    pMapInfo->reselmInfo[pMapInfo->numberofEntries].startRes = pTestConfConfig->pucch_map[id].startRes;
    pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes = pTestConfConfig->pucch_map[id].numRes;
    pMapInfo->numberofEntries++;

    return SUCCESS;
}


/* update cqi/harq into plte_pdsch_chan_desc-structure
 *
 * @param prx_status_ind Pointer to RxStatusInd
 * @return Standard result code
 */
RESULTCODE lte_pucch_update_cqi_harq(PRXSDUIND prx_sdu_ind)
{
    PLTE_PDSCH_HARQ_STATE       plte_pdsch_harq_state = get_lte_pdsch_harq_state();
    PLTE_PDSCH_CHAN_STATE       plte_pdsch_chan_state = get_lte_pdsch_chan_state();

    if ((plte_pdsch_harq_state->ena_harq == 0) || (plte_pdsch_chan_state->ena_dynamic_cqi_pmi_ri == 0))
        return SUCCESS;

    {
        PLTE_PDSCH_SDU_CTRL_RESEND  plte_pdsch_sdu_ctrl_resend;
        // the RxSDU may get in the current tti (stall at 1 tti)
        U32 tti_tick = prx_sdu_ind->frameNum*10 + prx_sdu_ind->subFrameNum;
        U32 sdu_harq_id = tti_tick&0x07;

        if ((prx_sdu_ind->pucchType == FORMAT1A) || (prx_sdu_ind->pucchType == FORMAT1B) || (prx_sdu_ind->pucchType == FORMAT2A) || (prx_sdu_ind->pucchType == FORMAT2B))
        {
    // codeword0 -- need to check
            plte_pdsch_sdu_ctrl_resend = get_lte_pdsch_sdu_ctrl_resend(0, prx_sdu_ind->channelId, sdu_harq_id);
            plte_pdsch_sdu_ctrl_resend->nack = (prx_sdu_ind->pRxSdu >> (2+8)) & 1; // bit_swapping - see comments for bs_rx__pucch_chan_coding-function
            plte_pdsch_sdu_ctrl_resend->tti_tick = tti_tick;

    // codeword1 -- need to check
            plte_pdsch_sdu_ctrl_resend = get_lte_pdsch_sdu_ctrl_resend(1, prx_sdu_ind->channelId, sdu_harq_id);
            plte_pdsch_sdu_ctrl_resend->nack = (prx_sdu_ind->pRxSdu >> (1+8)) & 1; // bit_swapping - see comments for bs_rx__pucch_chan_coding-function
            plte_pdsch_sdu_ctrl_resend->tti_tick = tti_tick;
        }

    // cqi/pmi/ri info can transmit just into FORMAT2, FORMAT2A or FORMAT2B
        if ((prx_sdu_ind->pucchType == FORMAT2) || (prx_sdu_ind->pucchType == FORMAT2A) || (prx_sdu_ind->pucchType == FORMAT2B))
        {
            BS_CONFOR_TEST_CONFIG*  ptest_cfg = get_bs_conf_test_config();
            PLTE_PDSCH_CHAN_DESC    plte_pdsch_chan_desc = get_lte_pdsch_chan_desc(prx_sdu_ind->channelId);
            U16 cqi_pmi16;

            cqi_pmi16 = (mac_utils_bit_swp((U8)(prx_sdu_ind->pRxSdu>>8))<<8) | mac_utils_bit_swp((U8)prx_sdu_ind->pRxSdu);

// check for schedule ri
            if (plte_pdsch_chan_state->sched_num_sfn_ri == prx_sdu_ind->subFrameNum)
            {
                plte_pdsch_chan_desc->numCodeWords_updating = 1;
                if (cqi_pmi16 & 1)
                {
                    plte_pdsch_chan_desc->numCodeWords_updating = 2;
                }
            }

// check for schedule cqi            
            if (plte_pdsch_chan_state->sched_num_sfn_cqi == prx_sdu_ind->subFrameNum)
            {
                plte_pdsch_chan_desc->ena_update_cqi_pmi = 1;
// rank=1
                if (plte_pdsch_chan_desc->numCodeWords_updating == 1)
                {
                    plte_pdsch_chan_desc->sub_ch_info_updating[0].mcsType = lte_pdsch_calc_mcs_via_cqi(cqi_pmi16 & 0x0F);
                    plte_pdsch_chan_desc->dl_precoder_info_updating.codeBookIdx = (cqi_pmi16 >> 4) & 0x03;
                }

// rank=2
                if (plte_pdsch_chan_desc->numCodeWords_updating == 2)
                {
                    plte_pdsch_chan_desc->sub_ch_info_updating[0].mcsType = lte_pdsch_calc_mcs_via_cqi( cqi_pmi16 & 0x0F);
                    plte_pdsch_chan_desc->sub_ch_info_updating[1].mcsType = lte_pdsch_calc_mcs_via_cqi((cqi_pmi16 & 0x0F) + ((cqi_pmi16>>4)&0x07));
                    plte_pdsch_chan_desc->dl_precoder_info_updating.codeBookIdx = ((cqi_pmi16 >> 7) & 0x01) + 1;
                }
                plte_pdsch_chan_desc->lte_pdsch_frame_num = tti_tick + 4;
            }
        }
    }

    return SUCCESS;
}

/* Function addes PUSCH channel to RXVECTOR
 *
 * @param pointer to TestConfConfig-structure
 * @param pCh Pointer to DlChanDesc into RxVector
 * @param id PHY channel ID
 * @param pMapInfo PHY channel ID
 * @param Idx PHY channel index
 * @return Standard result code
 */
RESULTCODE lte_pucch_add(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                         PULCTRLCHDESC pCh,
                         U32 id,
                         MAPPINGINFO *pMapInfo,
                         U32 Idx)
{
    PPUCCHDEDCTL                pucchDedCtl = &pCh->pucchDedCtl;
    PCQIPMIRIRPT                cqiPmiRiRpt = &pCh->cqiPmiRiRpt;
    PTIMADVERRINF               timAdvErrInfo = &pCh->timAdvErrInfo;
    PLTE_PDSCH_CHAN_STATE       plte_pdsch_chan_state = get_lte_pdsch_chan_state();

    PRINT_FUNCTION_NAME(__FUNCTION__);
    
    /* Fill Chann desc */
    pCh->offsetNextCh = NOT_SPECIFIED_VALUE;
    
    pCh->channelId = (U16)id;
    pCh->txpowerControl = 0;
    
    pCh->crcLength = NONE;
    pCh->channelType = PUCCH;
    pCh->codingDescriptor = 0;
    pCh->blockCodeConcatenation = 0;
#ifdef MX_ARM_ENABLED
    pCh->modulationType = 0;
#else
    pCh->modulationType = BPSK;
#endif
    pCh->mcsType = 0;
    pCh->reserved = NOT_SPECIFIED_VALUE;
    
    pCh->scrmblerInitValue = (pTestConfConfig->rntiPucch_used)? 
        pTestConfConfig->rntiPucch[Idx] : lte_rnti_get_deboosted_rnti(pTestConfConfig, 0);

    pTestMgrGetPucch(pTestConfConfig, pMapInfo, Idx);

    if(pMapInfo->numberofEntries)
    {
        pCh->startRes = pMapInfo->reselmInfo[pMapInfo->numberofEntries - 1].startRes; // 24
        pCh->numRes = pMapInfo->reselmInfo[pMapInfo->numberofEntries - 1].numRes; // 1
        pCh->numberofEntries = pMapInfo->numberofEntries;
    }

    pucchDedCtl->formatType = (pTestConfConfig->pucchFormat_used)? 
        pTestConfConfig->pucchFormat[Idx] : pTestConfConfig->ulSfrCtrl.uciformat;

    pucchDedCtl->ackNackRepetition = 0;
    pucchDedCtl->repetitionFactor = 0;
    pucchDedCtl->n1PucchANRep = 0;
    pucchDedCtl->cqiPUCCHResourceIndex = (pTestConfConfig->pucch_cqirsrcidx_used)?
        pTestConfConfig->pucch_cqirsrcidx[Idx] : pTestConfConfig->ulSfrCtrl.n1PucchAN;
    pucchDedCtl->srPUCCHResourceIndex = 0;

    if (plte_pdsch_chan_state->ena_dynamic_cqi_pmi_ri == 0)
    {
        pucchDedCtl->dlCqiPmiSizeBits = (pTestConfConfig->pucch_dlCqiPmiSizeBits_used)?
            pTestConfConfig->pucch_dlCqiPmiSizeBits[Idx] : pTestConfConfig->ulSfrCtrl.dlCqiPmiSizeBits;
    }
    else
    {
// the ri was scheduled by api
        if (plte_pdsch_chan_state->sched_num_sfn_ri == getSubFrameNum())
        {
            pucchDedCtl->dlCqiPmiSizeBits = 1;
        }
// the cqi was scheduled by api
        else if (plte_pdsch_chan_state->sched_num_sfn_cqi == getSubFrameNum())
        {
            PLTE_PDSCH_CHAN_DESC    plte_pdsch_chan_desc = get_lte_pdsch_chan_desc(Idx);
// need to send correct number of bits for cqi. The value of bits depend on Rank (ri). The ri must received few ttis ago 
            if (plte_pdsch_chan_desc->numCodeWords_updating == 1)
            {
                pucchDedCtl->dlCqiPmiSizeBits = 6;
            }
            else if (plte_pdsch_chan_desc->numCodeWords_updating == 2)
            {
                pucchDedCtl->dlCqiPmiSizeBits = 8;
            }
        }
// The ri/cqi isn't schedules
        else
        {
            pucchDedCtl->dlCqiPmiSizeBits = 13;
        }
    }
#ifndef AC_MSPD_TDD
    pucchDedCtl->harqPucchIndex = pTestConfConfig->pucch_harqPucchIndex[Idx];

    if(pucchDedCtl->harqPucchIndex)
    {
        if(pucchDedCtl->formatType == FORMAT1A)
            pucchDedCtl->harqSizebits = 1;
        else if(pucchDedCtl->formatType == FORMAT1B)
            pucchDedCtl->harqSizebits = 2;
    }
#else
    if(pucchDedCtl->formatType < FORMAT2)
    {
        pucchDedCtl->ackPUCCHResourceIndex0 = pTestConfConfig->ackPUCCHResourceIndex0[Idx];
        pucchDedCtl->ackPUCCHResourceIndex1 = pTestConfConfig->ackPUCCHResourceIndex1[Idx];
        pucchDedCtl->ackPUCCHResourceIndex2 = pTestConfConfig->ackPUCCHResourceIndex2[Idx];
        pucchDedCtl->ackPUCCHResourceIndex3 = pTestConfConfig->ackPUCCHResourceIndex3[Idx];
    }

#endif
    if(pTestConfConfig->pucch_harqSizebits_used)
    {// it has a higher priority than above setting
        pucchDedCtl->harqSizebits = pTestConfConfig->pucch_harqSizebits[Idx];
    }

    cqiPmiRiRpt->cqiReportModeAperiodic = 0;
    cqiPmiRiRpt->nomPDSCHRSEPREOffset = 0;
    cqiPmiRiRpt->cqiReportPeriodicEnable = 0;
#ifdef MX_ARM_ENABLED
    cqiPmiRiRpt->cqiPUCCHResourceIndex = 0;
#else
    cqiPmiRiRpt->cqiPUCCHResourceIndex = pucchDedCtl->cqiPUCCHResourceIndex;
#endif
    cqiPmiRiRpt->cqipmiConfigIndex = 0;
    cqiPmiRiRpt->cqiFormatIndicatorPeriodic = 0;
    cqiPmiRiRpt->K = 0;
    cqiPmiRiRpt->riConfigIndex = 0;
    cqiPmiRiRpt->simultaneousAckNackAndCQI = 0;
    cqiPmiRiRpt->padding = 0;

    timAdvErrInfo->mErrAvgCh[0] = pTestConfConfig->timAdvErrInfo[PUCCH - PUSCH].mErrAvgCh[0];
    timAdvErrInfo->mErrAvgCh[1] = pTestConfConfig->timAdvErrInfo[PUCCH - PUSCH].mErrAvgCh[1];
    timAdvErrInfo->mErrExpo = pTestConfConfig->timAdvErrInfo[PUCCH - PUSCH].mErrExpo;
    timAdvErrInfo->mSetDefaults = pTestConfConfig->timAdvErrInfo[PUCCH - PUSCH].mSetDefaults;

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
        {// not found, set default
            timAdvErrInfo->mSetDefaults = 1;
        }
#endif

    }

    if(pTestConfConfig->timAdvErrDflt_pucch_len == 1)
    {// applied only to current subframe, is not supported for RT
        //timAdvErrInfo->mSetDefaults = pTestConfConfig->timAdvErrDflt_pucch[0];
        pTestConfConfig->timAdvErrDflt_pucch_len = 0;
    }
    else if(pTestConfConfig->timAdvErrDflt_pucch_len)
    {
        timAdvErrInfo->mSetDefaults = 
            pTestConfConfig->timAdvErrDflt_pucch[(getFrameNum() * 10 + getSubFrameNum()) 
                % sizeof(pTestConfConfig->timAdvErrDflt_pucch)];
    }

    return SUCCESS;
}



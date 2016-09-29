#ifdef AC_MSPD_TDD
#include "MgrCfgTdd.h"
#include "TableTdd.h"
#include "LteMacTestCfgMgr.h"
#include "LteMac2PhyInterface.h"
#include "LteBsMacDlScheduler.h"


#define NOT_SPECIFIED_VALUE (0)
#define E_RS                (0)
#define MINUS_INF_Q8        (-128 * 256)
#define FLOAT2Q8(x)         (S32)(x * 256)
#define Q82FLOAT(x)         (float)(x / 256)

#define PRACH_ON   1
#define PRACH_OFF  0


static U32 IsPrachInSFrame ( TRACHSFQuadruple quadruple, U8 TDDconfig, U32 FNum, U32 SFNum);

static RESULTCODE PdschDirectMapping(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                         MAPPINGINFO *pMapInfo, U16 ChanId)
{
    U32 i = 0, num = 0;
    MAPPINGINFO* in_map = &pTestConfConfig->PdschCfg[ChanId].Map;
    PRINT_FUNCTION_NAME(__FUNCTION__);

    memset(pMapInfo, 0, sizeof(MAPPINGINFO));
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
    pMapInfo->numberofEntries = in_map->numberofEntries;

    return SUCCESS;
}
static RESULTCODE TestMgrGetDeBoostedPdschEx(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                             MAPPINGINFO *pMapInfo, U16 ChanId, U8 LayerIdx)
{
    U8 **pBoostingMap;
    U8 *pMap;
    U32 cur_num;
    U32 subFramNum;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    /* check if we've got  deBoosting in this model */
    if (testMgrCtx.deBoostingMap == NULL)
        return FAILURE;
    subFramNum = getSubFrameNum() + getFrameNum() * 10;
    memset(pMapInfo, 0, sizeof(MAPPINGINFO));

    pBoostingMap = testMgrCtx.deBoostingMap;
    pMap = (U8*)pBoostingMap[subFramNum];

    cur_num = pTestConfConfig->nPdschDeBoostedPrb;

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

static RESULTCODE lte_pusch_setmapinfo_direct(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
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
static RESULTCODE TxVectorCommonTdd(
                                         BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                         PDLSUBFRDESC pSubFrame)
{
    PBS_STATIC_CONFIG pStaticConfig = get_static_config();

    PRINT_FUNCTION_NAME(__FUNCTION__);
    memset(pSubFrame, 0, sizeof(DLSUBFRDESC));

    pSubFrame->frameNumber = getFrameNum();
    pSubFrame->subframeNumber = getSubFrameNum();
    pSubFrame->subframeType = DLTX;

    pSubFrame->antennaPortcount = pBsMacDesc->initParm.txAntennaPortCount;
    pSubFrame->numberofChannelDescriptors = 0x00;

    pSubFrame->reserved0 = NOT_SPECIFIED_VALUE;

    // offsets to be updated later after channels adding
    pSubFrame->offsetCustomFeatures = NOT_SPECIFIED_VALUE;

    pSubFrame->numCtrlSymbols = pTestConfConfig->nCtrlSymbols;

    pSubFrame->phichResource = pTestConfConfig->phichResource;       
    pSubFrame->phichDuration = pTestConfConfig->phichDuration;    

    pSubFrame->numberOfPhichChannels  = pTestConfConfig->phichIdx != 0xFF ? pTestConfConfig->nPhichGroup : TestMgrGetNumPhich(pTestConfConfig);

    pSubFrame->numCtrlChannels = 0;
    
    memcpy(&pSubFrame->dlCmnPwrCtl, &pStaticConfig->dl_cmn_pwr_ctl, sizeof(DLCMNTXPWRCTL));

/*
    pSubFrame->dlCmnPwrCtl.pilotPower = TestMgrGetPilotPower(pTestConfConfig);

    pSubFrame->dlCmnPwrCtl.psyncPower = TestMgrGetPsyncPower(pTestConfConfig);
    pSubFrame->dlCmnPwrCtl.ssyncPower = TestMgrGetSsyncPower(pTestConfConfig);

    // 0x8000;
    pSubFrame->dlCmnPwrCtl.ciPower    = TestMgrGetPcfichPower(pTestConfConfig);

    pSubFrame->dlCmnPwrCtl.paprControl   =  0x0000;
    pSubFrame->dlCmnPwrCtl.paprThreshold =  0x0000; */

    return SUCCESS;
}
static RESULTCODE TxVectorCommonSpTdd(
                                         BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                         PDLSUBFRDESC pSubFrame)
{
    PBS_STATIC_CONFIG pStaticConfig = get_static_config();

    PRINT_FUNCTION_NAME(__FUNCTION__);
    memset(pSubFrame, 0, sizeof(DLSUBFRDESC));

    pSubFrame->frameNumber = getFrameNum();
    pSubFrame->subframeNumber = getSubFrameNum();
    pSubFrame->subframeType = DLTX;

    pSubFrame->antennaPortcount = pBsMacDesc->initParm.txAntennaPortCount;
    pSubFrame->numberofChannelDescriptors = 0x00;

    pSubFrame->reserved0 = NOT_SPECIFIED_VALUE;

    // offsets to be updated later after channels adding
    pSubFrame->offsetCustomFeatures = NOT_SPECIFIED_VALUE;

    pSubFrame->numCtrlSymbols = (pTestConfConfig->nCtrlSymbols > 2 )? 2 : pTestConfConfig->nCtrlSymbols;

    pSubFrame->phichResource = pTestConfConfig->phichResource;       
    pSubFrame->phichDuration = pTestConfConfig->phichDuration;    

    pSubFrame->numberOfPhichChannels  = pTestConfConfig->phichIdx != 0xFF ? pTestConfConfig->nPhichGroup : TestMgrGetNumPhich(pTestConfConfig);

    pSubFrame->numCtrlChannels = 0;

    memcpy(&pSubFrame->dlCmnPwrCtl, &pStaticConfig->dl_cmn_pwr_ctl, sizeof(DLCMNTXPWRCTL));

/*    pSubFrame->dlCmnPwrCtl.pilotPower = TestMgrGetPilotPower(pTestConfConfig);

    pSubFrame->dlCmnPwrCtl.psyncPower = TestMgrGetPsyncPower(pTestConfConfig);
    pSubFrame->dlCmnPwrCtl.ssyncPower = TestMgrGetSsyncPower(pTestConfConfig);

    // 0x8000;
    pSubFrame->dlCmnPwrCtl.ciPower    = TestMgrGetPcfichPower(pTestConfConfig);

    pSubFrame->dlCmnPwrCtl.paprControl   =  0x0000;
    pSubFrame->dlCmnPwrCtl.paprThreshold =  0x0000;*/

    return SUCCESS;
}
U32 TestMgrCalculateDataSduSizeTdd(PDLSUBCHINFO pSubCh,
                                       MAPPINGINFO *pMapInfo)
{
    U32 nResElm = 0;
    U32 i;
    PRINT_FUNCTION_NAME(__FUNCTION__);

    for (i = 0; i < pMapInfo->numberofEntries; i++)
        nResElm += pMapInfo->reselmInfo[i].numRes;
    nResElm = max(nResElm*0.75, 1);
    return lte_pdsch_get_tb_size(pSubCh->mcsType, nResElm, 1);
}
static RESULTCODE TestMgrGetPdcchDirect(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                         U8 pdcchIdx,
                                         MAPPINGINFO *pMapInfo )
{
    U8 SFIndex;

    PRINT_FUNCTION_NAME(__FUNCTION__);
    memset(pMapInfo, 0, sizeof(MAPPINGINFO));

    if(pTestConfConfig->pdcch_map_unified[pdcchIdx])
        SFIndex = 0;
    else
        SFIndex = getSubFrameNum();

    pMapInfo->reselmInfo[pMapInfo->numberofEntries].startRes =
                                    pTestConfConfig->pdcch_map[pdcchIdx][SFIndex].startRes;
    pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes =
                                    pTestConfConfig->pdcch_map[pdcchIdx][SFIndex].numRes;
    pMapInfo->numberofEntries++;

    return SUCCESS;
}
static RESULTCODE TestMgrGetPdcch(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                         U8 pdcchIdx,
                                         MAPPINGINFO *pMapInfo )
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    memset(pMapInfo, 0, sizeof(MAPPINGINFO));

    pMapInfo->reselmInfo[pMapInfo->numberofEntries].startRes =
                                    pdcchIdx * pTestConfConfig->nCcePerPdcch;
    pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes =
                                    pTestConfConfig->nCcePerPdcch;
    pMapInfo->numberofEntries++;

    return SUCCESS;
}
static RESULTCODE SetAutoPdsch(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                         MAPPINGINFO *pMapInfo, U16 ChanId, U8 LayerIdx)
{
    U8 **pBoostingMap;
    U8 subFramNum;
    U8 *pMap;
    U32 cur_num;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    memset(pMapInfo, 0, sizeof(MAPPINGINFO));

    subFramNum = getSubFrameNum() + getFrameNum() * 10;
    if(testMgrCtx.boostingMap == NULL)
    {
        printf(C4KFW "ERROR: boostingMap has bad ptr [%s,%s:%d]\n",
               __FUNCTION__, __FILE__, __LINE__);
        return -1;
    }

    pBoostingMap = testMgrCtx.boostingMap[LayerIdx][ChanId];
    pMap = pBoostingMap[subFramNum];

    cur_num = pTestConfConfig->nPdschBoostedPrb;

    if(*pMap == 0xFF)
        return SUCCESS;

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

RESULTCODE LtePhyPdschSubDesc(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                 PDLSUBCHINFO pSubCh,
                                 U8 modulation,
                                 U8 mcsType,
                                 U8 rV,
                                 U32 cwID)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    pSubCh->codingDescriptor = TURBOCDR;
    pSubCh->blockCodeConcatenation = 1;
    pSubCh->modulationType = modulation;

    pSubCh->mcsType = mcsType;

    pSubCh->reserved = NOT_SPECIFIED_VALUE;
    pSubCh->nDi =  1;
    pSubCh->rV = rV;
    pSubCh->flushReq = 0;

    pSubCh->pad = 0;

    return SUCCESS;
}
static RESULTCODE LtePhyTxVectorTdd(
                                   BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                   PDLSUBFRDESC pSubFrame,
                                   U8 channelType)
{
    U8 chanIdx = pSubFrame->numberofChannelDescriptors;
    U8 Idx = 0;
    MAPPINGINFO mapInfo;
    U8 Pdsch_idx;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    switch(channelType)
    {
        case PHICH:
            if(pTestConfConfig->phichIdx != 0xFF)
            {
                for (Idx = 0;
                     Idx < pTestConfConfig->nPhichGroup;
                     Idx++)
                {
                    lte_add_phich(&pSubFrame->dciCh[chanIdx],
                                  chanIdx,
                                  Idx,  /* group ID */
                                  pTestConfConfig->phichIdx); /* sequence Index */
                    pSubFrame->numCtrlChannels++;
                    chanIdx++;
                }
            }
            else
            {
                for (Idx = 0;
                     Idx < pTestConfConfig->nPhichGroup;
                     Idx++)
                {
                    U32 i;
                    for (i = 0; i < pTestConfConfig->nPhichPerGroup; i++)
                    {
                        lte_add_phich(&pSubFrame->dciCh[chanIdx],
                                       chanIdx,
                                       Idx,  /* group ID */
                                       i*4); /* sequence Index [0,4,0,4,0,4] */
                        pSubFrame->numCtrlChannels++;
                        chanIdx++;
                    }
                }
            }
            break;
        case PDCCH:
            for (Idx = 0;
                 Idx < pTestConfConfig->nPdcch;
                 Idx++)
            {
                 if(pTestConfConfig->pdcch_map_used)
                    lte_pdcch_get_mapinfo_direct(pTestConfConfig, Idx, &mapInfo);
                 else
                    lte_pdcch_get_mapinfo_from_cce(pTestConfConfig, Idx, &mapInfo);
                   
                 lte_add_pdcch(pSubFrame->dciCh + chanIdx,
                               chanIdx,
                               &mapInfo,
                               Idx);

                pSubFrame->numCtrlChannels++;
                chanIdx++;
            }
            break;
        case PDSCH:
            {
                PBS_STATIC_CONFIG pStaticConfig = get_static_config();
                DLCHANDESC* dlCh;
                U32 i;
                U16 rnti;
                for(Pdsch_idx = 0; Pdsch_idx < pTestConfConfig->nPdsch[0]; Pdsch_idx++)
                {
                    if(pTestConfConfig->PdschCfg[Pdsch_idx].Map.numberofEntries)
                        lte_pdsch_setmapinfo_direct(pTestConfConfig,&mapInfo, Pdsch_idx);
                    else
                    {
                        if(Pdsch_idx == 0)
                            SetAutoPdsch(pTestConfConfig,&mapInfo, Pdsch_idx, 0);
                        else
                            TestMgrGetDeBoostedPdschEx(pTestConfConfig,&mapInfo, Pdsch_idx, 0);
                    }
                        
                    dlCh = &pSubFrame->dlCh[chanIdx - pSubFrame->numCtrlChannels];
                    rnti = pTestConfConfig->PdschCfg[Pdsch_idx].Rnti;
                    lte_pdsch_add(pTestConfConfig,
                                  dlCh,
                                  chanIdx,
                                  &mapInfo,
                                  rnti,
                                  pTestConfConfig->PdschCfg[Pdsch_idx].Pa + pStaticConfig->dl_cmn_pwr_ctl.pilotPower);
                    for (i = 0; i < pTestConfConfig->nCw; i++)
                    {
                        LtePhyPdschSubDesc(pTestConfConfig,
                                   &dlCh->subChInfo[i],
                                   pTestConfConfig->PdschCfg[Pdsch_idx].CwCfg[i].Mod,
                                   pTestConfConfig->PdschCfg[Pdsch_idx].CwCfg[i].Mcs,
                                   pTestConfConfig->PdschCfg[Pdsch_idx].CwCfg[i].Rvi, 
                                   i);

                        /* Generate PHY TXSDU.req for this channel */
                        LtePhyPrepareTestSdu((pTestConfConfig->pdschPayloadEna == 1 )? &pTestConfConfig->pdschPayload:NULL,
                                    lte_pdsch_get_sdu_size(&dlCh->subChInfo[i], &mapInfo, 1),
                                    chanIdx,
                                    PDSCH, i);

                    }
                    
                    chanIdx++;

                }
            }
 
            break;
        case PBCH:
            if((MacRtMode != MACMODE_NRT && (pTestConfConfig->nPdschBoostedPrb != 0))
                || MacRtMode == MACMODE_NRT)
            {
                if(pBsMacDesc->isMIBconfigured == FALSE)
                {// fill MIB self for not configured by Host bytes
                    MIB_INFO* pMib = (MIB_INFO*)pTestConfConfig->payloadPbch;

                    // update only SFN for now
                    pMib->systemFrameNumberHi = (getFrameNum() >> (2+6)) & 0x3;
                    pMib->systemFrameNumberLo = (getFrameNum() >> 2) & 0x3F;

                    pMib->phich_resource = pSubFrame->phichResource;
                    pMib->phich_duration = (pSubFrame->phichDuration == PHICHDUR_NORMAL)? 
                        PHICHDUR_MIB_NORMAL : PHICHDUR_MIB_EXTEND;
                }
                lte_add_pbch(pTestConfConfig,
                             &pSubFrame->dlCh[chanIdx - pSubFrame->numCtrlChannels],
                             chanIdx);
            }
            chanIdx++;
            break;
        case PCFICH:
            /* PHY addes automatically this channel */
            break;
        default:
            break;
    }

    pSubFrame->numberofChannelDescriptors = chanIdx;
    return SUCCESS;
}

static RESULTCODE LtePhyTxVectorSPTdd(
                                   BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                   PDLSUBFRDESC pSubFrame)
{
    PBS_STATIC_CONFIG pStaticConfig = get_static_config();
    U8 chanIdx = pSubFrame->numberofChannelDescriptors;
    U8 Idx = 0;
    MAPPINGINFO mapInfo;
    U8 Pdsch_idx = 0;
    DLCHANDESC* dlCh;
    U32 i;
    U16 rnti;

    for(Pdsch_idx = 0; Pdsch_idx < pTestConfConfig->nPdsch[0]; Pdsch_idx++)
    {
        if(pTestConfConfig->PdschCfg[Pdsch_idx].Map.numberofEntries)
            lte_pdsch_setmapinfo_direct(pTestConfConfig,&mapInfo, Pdsch_idx);
        else
        {
            if(Pdsch_idx == 0)
                SetAutoPdsch(pTestConfConfig,&mapInfo, Pdsch_idx, 0);
            else
                TestMgrGetDeBoostedPdschEx(pTestConfConfig,&mapInfo, Pdsch_idx, 0);
        }

        dlCh = &pSubFrame->dlCh[chanIdx - pSubFrame->numCtrlChannels];
        rnti = pTestConfConfig->PdschCfg[Pdsch_idx].Rnti;
        lte_pdsch_add(pTestConfConfig,
                      dlCh,
                      chanIdx,
                      &mapInfo,
                      rnti,
                      pTestConfConfig->PdschCfg[Pdsch_idx].Pa + pStaticConfig->dl_cmn_pwr_ctl.pilotPower);
        for (i = 0; i < pTestConfConfig->nCw; i++)
        {
            LtePhyPdschSubDesc(pTestConfConfig,
                       &dlCh->subChInfo[i],
                       pTestConfConfig->PdschCfg[Pdsch_idx].CwCfg[i].Mod,
                       pTestConfConfig->PdschCfg[Pdsch_idx].CwCfg[i].Mcs,
                       pTestConfConfig->PdschCfg[Pdsch_idx].CwCfg[i].Rvi,
                       i);

            /* Generate PHY TXSDU.req for this channel */
            LtePhyPrepareTestSdu((pTestConfConfig->pdschPayloadEna == 1 )? &pTestConfConfig->pdschPayload:NULL,
                        TestMgrCalculateDataSduSizeTdd(&dlCh->subChInfo[i], &mapInfo),
                        chanIdx,
                        PDSCH, i);

        }
        
        chanIdx++;

    }
    pSubFrame->numberofChannelDescriptors = chanIdx;
    return SUCCESS;
}
void setPhichTdd(U8 PhichFactorM)
{
    testConfConfig.nPhichGroup = testConfConfig.PhichGroupSfn * PhichFactorM;

}
RESULTCODE TestMgrProcessConfigSpDlTdd(PDLSUBFRDESC pSubFrame, U8 SpEna, U8 PdschEna)
{
    U32 res = SUCCESS;
    PRINT_FUNCTION_NAME(__FUNCTION__);

    /* Common stuff */
    if(SpEna == 1)
    {
        TxVectorCommonSpTdd(&testConfConfig, pSubFrame);
    }
    else
    {
        TxVectorCommonTdd(&testConfConfig, pSubFrame);
    }
    /* PCFICH channel is put automaticaly by PHY */
    LtePhyTxVectorTdd(&testConfConfig, pSubFrame, PHICH);


    if(SpEna == 1)
    {
        if(PdschEna)
        {
             LtePhyTxVectorTdd(&testConfConfig, pSubFrame, PDCCH);
             LtePhyTxVectorSPTdd(&testConfConfig, pSubFrame);
        }
    }
    else
    {
        LtePhyTxVectorTdd(&testConfConfig, pSubFrame, PDCCH);
        LtePhyTxVectorTdd(&testConfConfig, pSubFrame, PDSCH);
    }


    if ((getSubFrameNum() == 0) && (getFrameNum() % 4 == 0))
        LtePhyTxVectorTdd(&testConfConfig, pSubFrame, PBCH);

    return SUCCESS;
}
static U8 lte_pusch_conv_mcs_to_modulation(U8 Mcs)
{
    if(Mcs <= 10)
        return QPSK;
    else if(Mcs <= 20)
        return QAM16;
    else if(Mcs <= 28)
        return QAM64;
    else
        return 0xFF;
}
static RESULTCODE lte_pusch_fill_sub_channel(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                 PULSUBCHINFO pSubCh,
                                 U8 modulation,
                                 U8 mcsType,
                                 MAPPINGINFO *pMapInfo,
                                 U16 idx)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    pSubCh->mcinfo.modulationType = modulation;
    pSubCh->mcinfo.codingDescriptor = TURBOCDR;
    pSubCh->mcinfo.blockCodeConcatenation = 1;
    pSubCh->mcinfo.mcsType = mcsType;
    pSubCh->harqInfo.rV = pTestConfConfig->arRctRv[ (getFrameNum() + getSubFrameNum() * 10 ) % 1000];
    pSubCh->harqInfo.nDi = (pSubCh->harqInfo.rV == 0)? 1 : 0;
    pSubCh->harqInfo.flushReq = 0;
    pSubCh->crcInfo.crcLength = TWTYFOUR;
    pSubCh->crcInfo.crcScrambling = 0;
    pSubCh->scrInfo.scramblerType = ULSCR;
    pSubCh->scrInfo.scrinitValueinput = pTestConfConfig->rntiPusch[idx];

    pSubCh->puschDed.nACK = pTestConfConfig->puschDed.nACK;
    pSubCh->puschDed.betaOffsetACKIndex = pTestConfConfig->puschDed.betaOffsetACKIndex;
    pSubCh->puschDed.nDMRS2 = (pTestConfConfig->pusch_nDMRS2_used)?
                               pTestConfConfig->pusch_nDMRS2[idx] : pTestConfConfig->puschDed.nDMRS2;

    pSubCh->puschDed.betaOffsetCQIIndex = pTestConfConfig->betaOffsetCQIIndex;
    pSubCh->puschDed.nsymi = pTestConfConfig->nsymi;
    pSubCh->puschDed.ackNackReportMode = pTestConfConfig->ackNackReportMode;
    pSubCh->puschDed.Nbundled = pTestConfConfig->Nbundled;

    if(pSubCh->puschDed.nACK)
    {
        U32 i, nRB = 0;

        for(i = 0; i < pMapInfo->numberofEntries; i++)
        {
            nRB += pMapInfo->reselmInfo[i].numRes;
        }
        
        pSubCh->puschDed.nrbi = nRB;
        pSubCh->puschDed.nsymi = 12;
    }

    pSubCh->reserved = NOT_SPECIFIED_VALUE;

    memcpy(&pSubCh->mapInfo, pMapInfo, sizeof(MAPPINGINFO));

    return SUCCESS;
}
static RESULTCODE LtePhyUpdateTestRxVectorTdd(
                                   BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                   PULSUBFRDESC pSubFrame,
                                   U8 channelType,
                                   PMAPPINGINFO pmapInfo)
{
    U8 chanIdx = pSubFrame->numberofChannelDescriptors;
    MAPPINGINFO mapInfoPusch;
    U32 Res = SUCCESS;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    switch(channelType)
    {
        case PRACH:
            {
                pSubFrame->offsetRachCtrlStruct =
                    (U32)&pSubFrame->rachCtrl - (U32)pSubFrame;
                pSubFrame->rachCtrl.rootSequenceIndex  = pTestConfConfig->rootSequenceIndex; //22;
                pSubFrame->rachCtrl.prachConfigIndex = pTestConfConfig->prachConfigIndex;// 12; 
                pSubFrame->rachCtrl.highSpeedFlag = pTestConfConfig->highSpeedFlag;// 0
                pSubFrame->rachCtrl.zeroCorrelationZoneConfig = pTestConfConfig->zeroCorrelationZoneConfig;//1;
                pSubFrame->rachCtrl.prachFreqOffset = pTestConfConfig->prachFreqOffset; //0
            }
            break;
        case PUCCH:
            {
                U32 NumCtrl = pSubFrame->numberOfCtrlChannelDescriptors;

                lte_pucch_add(pTestConfConfig,
                              &pSubFrame->ulCtlCh[NumCtrl],
                              chanIdx,
                              pmapInfo,
                              NumCtrl);

                pSubFrame->numberOfCtrlChannelDescriptors++;
                chanIdx++;
            }
            break;
        case PUSCH:
            /* PDSCH channels with boosted PRBs */

            if((chanIdx - pSubFrame->numberOfCtrlChannelDescriptors) == 0)
            {// we're adding the first PUSCH
                pmapInfo->numberofEntries = 0;
            }
            if(pTestConfConfig->pusch_map_used)
                Res = lte_pusch_set_mapinfo_direct(pTestConfConfig, &mapInfoPusch, pmapInfo);
            else
                Res = lte_pusch_set_mapinfo(pTestConfConfig, &mapInfoPusch, pmapInfo);
            if (Res == SUCCESS)
            {
                U8 NumCtrl = pSubFrame->numberOfCtrlChannelDescriptors;
                ULCHANDESC* ulCh = &pSubFrame->ulCh[chanIdx - NumCtrl];
                U8 Modulation;
                U8 mcsType;

                lte_pusch_add(pTestConfConfig,
                              ulCh,
                              chanIdx,
                              lte_pusch_get_power(pTestConfConfig));

                if(pTestConfConfig->puschMcs_used)
                {
                    mcsType = pTestConfConfig->puschMcs[chanIdx - NumCtrl];
                    Modulation = lte_pusch_conv_mcs_to_modulation(mcsType);
                }
                else
                {
                    Modulation = pTestConfConfig->puschBoostedMod;
                    mcsType = GetMcsIndexUl(Modulation);
                }

                lte_pusch_fill_sub_channel( pTestConfConfig,
                                        &ulCh->ulSubChInfo,
                                        Modulation,
                                        mcsType,
                                        &mapInfoPusch,
                                        chanIdx - NumCtrl
                                        );
                
                chanIdx++;
            }
            else
                return FAILURE;

            break;
        default:
            break;
    }

    pSubFrame->numberofChannelDescriptors = chanIdx;
    return SUCCESS;
}
static RESULTCODE LtePhyFillTestRxVectorCommonTdd(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                               PULSUBFRDESC pSubFrame)
{
    TRACHSFQuadruple quadruple;
    U32 i;
    PULSUBFRCMNCTRL ulSfrCtrl = &pSubFrame->ulSfrCtrl;
    PRINT_FUNCTION_NAME(__FUNCTION__);

    memset(pSubFrame, 0, sizeof(ULSUBFRDESC));

    pSubFrame->frameNumber = getFrameNum();
    pSubFrame->subframeNumber = getSubFrameNum();
    pSubFrame->subframeType = ULRX;

    pSubFrame->antennaPortcount = pBsMacDesc->initParm.rxAntennaPortCount;
    pSubFrame->numberofChannelDescriptors = 0x00;

    pSubFrame->numberOfCtrlChannelDescriptors = 0x00;

    // offsets to be updated later after channels adding
    pSubFrame->offsetCustomFeatures = NOT_SPECIFIED_VALUE;

    pSubFrame->numberSrsinSf = NOT_SPECIFIED_VALUE;

    /* disable RACH channel */
    for(i = 0; i < 6; i++)
    {
        quadruple = RACHMapping[pTestConfConfig->prachConfigIndex][pBsMacDesc->initParm.ulDlConfig][i];
        pSubFrame->rachCtrl.prachEnable = IsPrachInSFrame (quadruple, 
                                                        pBsMacDesc->initParm.ulDlConfig, 
                                                        pSubFrame->frameNumber,
                                                        pSubFrame->subframeNumber);
        if (pSubFrame->rachCtrl.prachEnable)
            break;
    }

   /* if(pTestConfConfig->prachEnableAllSfs)
        pSubFrame->rachCtrl.prachEnable = PRACH_ON;
    else
        pSubFrame->rachCtrl.prachEnable = PrachConfig(pTestConfConfig->prachConfigIndex,
                                                  pSubFrame->frameNumber,
                                                  pSubFrame->subframeNumber);
*/
	///SubFrame Control Info
    ulSfrCtrl->deltaPUCCHShift = pTestConfConfig->ulSfrCtrl.deltaPUCCHShift; //2;
    ulSfrCtrl->nRBCQI = pTestConfConfig->ulSfrCtrl.nRBCQI; //1;
    ulSfrCtrl->nCSAn = pTestConfConfig->ulSfrCtrl.nCSAn; //0;
    ulSfrCtrl->n1PucchAN = pTestConfConfig->ulSfrCtrl.n1PucchAN; //11;
    ulSfrCtrl->srsBandwitdhConfig = 0;
    ulSfrCtrl->srsSubframeConfig = 0;
    ulSfrCtrl->ackNackSRSSimultaneousTransmission = pTestConfConfig->ackNackSRSSimultaneousTransmission;
    ulSfrCtrl->nSB = pTestConfConfig->ulSfrCtrl.nSB; //0;
    ulSfrCtrl->hoppingMode = pTestConfConfig->ulSfrCtrl.hoppingMode;
    ulSfrCtrl->puschhoppingOffset = pTestConfConfig->ulSfrCtrl.puschhoppingOffset;
    ulSfrCtrl->enable64QAM = pTestConfConfig->ulSfrCtrl.enable64QAM; //1;
    ulSfrCtrl->groupHoppingEnabled = pTestConfConfig->ulSfrCtrl.groupHoppingEnabled;
    ulSfrCtrl->groupAssignmentPUSCH = pTestConfConfig->ulSfrCtrl.groupAssignmentPUSCH;
    ulSfrCtrl->sequenceHoppingEnabled = pTestConfConfig->ulSfrCtrl.sequenceHoppingEnabled;
    ulSfrCtrl->cyclicShift = pTestConfConfig->ulSfrCtrl.cyclicShift; //0;
    ulSfrCtrl->padding = 0;

    return SUCCESS;
}
RESULTCODE TestMgrProcessConfigUlTdd(PULSUBFRDESC pSubFrame)
{
    U32 i;
    MAPPINGINFO mapInfo;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    memset(&mapInfo, 0, sizeof(MAPPINGINFO));

    /* common stuff */
    LtePhyFillTestRxVectorCommonTdd(&testConfConfig, pSubFrame);

    for(i=0; i<testConfConfig.numPusch[pSubFrame->subframeNumber]; i++)
        LtePhyUpdateTestRxVectorTdd(&testConfConfig, pSubFrame, PUSCH, &mapInfo);

    for(i=0; i<testConfConfig.numPucch[pSubFrame->subframeNumber]; i++)
        LtePhyUpdateTestRxVectorTdd(&testConfConfig, pSubFrame, PUCCH, &mapInfo);

    if(pSubFrame->rachCtrl.prachEnable)
    	LtePhyUpdateTestRxVectorTdd(&testConfConfig, pSubFrame, PRACH, &mapInfo);

    lte_vectors_update_rx_srs_tdd(&testConfConfig, pSubFrame);

    return SUCCESS;
}

static U32 IsPrachInSFrame ( TRACHSFQuadruple quadruple, U8 TDDconfig, U32 FNum, U32 SFNum)
{
	U32 res = 0;

	if ( (quadruple.t0RA == 0) || (quadruple.t0RA == 1 && (FNum%2) == 0) || (quadruple.t0RA == 2 && (FNum%2) != 0))
	{
		unsigned int pSFNum;
		switch (TDDconfig)
		{
		case 0:
			if (quadruple.t1RA == 0)
			{
				if (quadruple.t2RA == 0)
					pSFNum = 2;
				else if (quadruple.t2RA == 1)
					pSFNum = 3;
				else if (quadruple.t2RA == 2)
					pSFNum = 4;
				else if (quadruple.t2RA == 8)
					pSFNum = 1;
			}
			else
			{
				if (quadruple.t2RA == 0)
					pSFNum = 7;
				else if (quadruple.t2RA == 1)
					pSFNum = 8;
				else if (quadruple.t2RA == 2)
					pSFNum = 9;
				else if (quadruple.t2RA == 8)
					pSFNum = 6;
			};
			break;
		case 1:
			if (quadruple.t1RA == 0)
			{
				if (quadruple.t2RA == 0)
					pSFNum = 2;
				else if (quadruple.t2RA == 1)
					pSFNum = 3;
				else if (quadruple.t2RA == 8)
					pSFNum = 1;
			}
			else
			{
				if (quadruple.t2RA == 0)
					pSFNum = 7;
				else if (quadruple.t2RA == 1)
					pSFNum = 8;
				else if (quadruple.t2RA == 8)
					pSFNum = 6;
			};
			break;
		case 2:
			if (quadruple.t2RA == 0)
			{	
				if (quadruple.t2RA == 8)
					pSFNum = 1;
				else
					pSFNum = 2;
			}
			else
			{
				if (quadruple.t2RA == 8)
					pSFNum = 6;
				else 
					pSFNum = 7;
			}
			break;
		case 3:
			if (quadruple.t2RA == 0)
				pSFNum = 2;
			else if (quadruple.t2RA == 1)
				pSFNum = 3;
			else if (quadruple.t2RA == 2)
				pSFNum = 4;
			else if (quadruple.t2RA == 8)
				pSFNum = 1;
			break;
		case 4:
			if (quadruple.t2RA == 0)
				pSFNum = 2;
			else if (quadruple.t2RA == 1)
				pSFNum = 3;
			else if (quadruple.t2RA == 8)
				pSFNum = 1;
			break;
		case 5:
			if (quadruple.t2RA == 8)
				pSFNum = 1;
			else
				pSFNum = 2;
			break;
		case 6:
			if (quadruple.t1RA == 0)
			{
				if (quadruple.t2RA == 0)
					pSFNum = 2;
				else if (quadruple.t2RA == 1)
					pSFNum = 3;
				else if (quadruple.t2RA == 2)
					pSFNum = 4;
				else if (quadruple.t2RA == 8)
					pSFNum = 1;
			}
			else
			{
				if (quadruple.t2RA == 0)
					pSFNum = 7;
				else if (quadruple.t2RA == 1)
					pSFNum = 8;
				else if (quadruple.t2RA == 8)
					pSFNum = 6;
			};
			break;
		}
		if (SFNum == pSFNum)
			res = 1;
	}

	return res;
}
#endif /*AC_MSPD_TDD*/

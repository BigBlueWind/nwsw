/** @file lte_pdcch.c
 *
 * @brief Implemenataion for creating PDCCH message
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "LtePhyL2Api.h"
#include "resultcodes.h"
#include "LteMacTestCfgMgr.h"
#ifndef MX_ARM_ENABLED
#include "msgroute.h"
#endif

#ifdef MX_ARM_ENABLED
#include "enbMac2PhyIface.h"
#include "supervisor.h"            // SvsrAllocMsg
#else
#include "LteMac2PhyInterface.h"
#endif

#include "lte_mac_typedef.h"

#include "lte_mac_state.h"
#include "lte_etm.h"
#include "lte_phich.h"
#include "lte_pusch.h"
#include "lte_pdcch.h"
#include "lte_rnti.h"
#include "lte_modulation.h"

extern LTE_MAC_STATE    lte_mac_state;
extern tTestMgrGetPdcch pTestMgrGetPdcch;

extern U32              global_chanIdx;

/** Get pointer to lte_mac_state.lte_pdcch_chan_state-structure
 *
 * @return pointer to lte_mac_state.lte_pdcch_chan_state-structure
 */
PLTE_PDCCH_CHAN_STATE      get_lte_pdcch_chan_state(void)
{
    return (PLTE_PDCCH_CHAN_STATE)&lte_mac_state.lte_pdcch_chan_state;
}

/** Get Nregs
 *
 * @param n_prb max number of PRBs
 * @param ctrl currently = 1
 * @param num_ant number of antenn
 * @param cp cycle prefix
 * @return value of Nregs
 */
int lte_pdcch_get_N_regs(int n_prb,     // max RB (50)
                         int ctrl,      // 1
                         int num_ant,   // numAnt
                         int cp)        // cycle-prefix, 0-normal, 1-extended
{
    int i, res = 0;
    
    for( i = 0; i < ctrl; i++ )
    {
        if( i == 0 ) 
        {
            res += 2 * n_prb;  
        }
        else if( i == 1)
        {
            res += num_ant == 4 ? 2 * n_prb : 3 * n_prb;
        }
        else if ( i == 2 )
        {
            res += 3 * n_prb;
        }
        else if ( i == 3 )
        {
            res += cp == 1 ? 2 * n_prb : 3 * n_prb;
        }
    }
    return res;
}

/** Set Dci allocation.
 *
 * @param rnti RNTI
 * @param ctrl currently = 1
 * @param agreg_level aggregation level
 * @param nprb max number of PRBs
 * @param phich_ng ???
 * @param nRs number antenn
 * @param cp cycle prefix
 * @param dci_id DCI id
 * @param scce Pointer for results storage
 * @return no
 */
void lte_pdcch_set_dci_alloc(U16 rnti, 
                             U8 ctrl, 
                             U8 agreg_level, 
                             U8 nprb, 
                             int phich_ng, 
                             U8 nRs, 
                             U8 cp, 
                             U8 dci_id, 
                             U32 *scce)
{
    U32 i, y = rnti;
    U32 tmp32 = lte_pdcch_get_N_regs( nprb /* 50 max RB for 10Mhz*/, ctrl, nRs, cp) - lte_phich_get_pcfich_regs();

    printf("CCE: ");
    for (i = 0; i < 10; i++) // calc scce for all sf
    {
        int dci_regs =  tmp32 -  lte_phich_get_regs(phich_ng, 0, 0, i);
        int n_cce = dci_regs/9;
        y = (39827 * y) % 65537;
        scce[i] = agreg_level * (( y + dci_id ) % (n_cce/agreg_level));
        printf("sf%d:%d ", (int)i, (int)scce[i]);
    }
    printf("\n");

    return;
}

/** Set mapinfo
 *
 * @param pTestConfConfig Pointer to TestConfConfig
 * @param pdcch_idx pdcch index
 * @param pmapinfo Pointer for results storage
 * @return Standard result code
 */
RESULTCODE lte_pdcch_set_mapinfo(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                 U8 pdcch_idx,
                                 MAPPINGINFO *pmapinfo)

{
    memset(pmapinfo, 0, sizeof(MAPPINGINFO));

    pmapinfo->reselmInfo[0].startRes = (U8)lte_mac_state.lte_pdcch_chan_state.cce_idx[pdcch_idx];
    lte_mac_state.lte_pdcch_chan_state.cce_idx[pdcch_idx] += pTestConfConfig->nCcePerPdcch;
    pmapinfo->reselmInfo[0].numRes = pTestConfConfig->nCcePerPdcch;
    pmapinfo->numberofEntries++;

    return SUCCESS;
}

/** Get mapinfo
 *
 * @param pTestConfConfig Pointer to TestConfConfig
 * @param pdcch_idx pdcch index
 * @param pmapinfo Pointer for results storage
 * @return Standard result code
 */
RESULTCODE lte_pdcch_get_mapinfo(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                 PDCICHANNELDESC pch,
                                 U32 ch_id,
                                 MAPPINGINFO *pmapinfo)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    pch->numberofEntries = pmapinfo->numberofEntries;
    pch->startRes = pmapinfo->reselmInfo[0].startRes;
    pch->numRes = pmapinfo->reselmInfo[0].numRes;

    return SUCCESS;
}


/** Get mapinfo
 *
 * @param pTestConfConfig Pointer to TestConfConfig
 * @param pdcch_idx pdcch index
 * @param pmapinfo Pointer for results storage
 * @return Standard result code
 */
RESULTCODE lte_pdcch_get_mapinfo_from_cce(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                          U8 pdcchIdx,
                                          MAPPINGINFO *pMapInfo)
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


/** Get mapinfo
 *
 * @param pTestConfConfig Pointer to TestConfConfig
 * @param pdcch_idx pdcch index
 * @param pmapinfo Pointer for results storage
 * @return Standard result code
 */
RESULTCODE lte_pdcch_get_mapinfo_direct(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                        U8 pdcchIdx,
                                        MAPPINGINFO *pMapInfo)
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



/** Get dci length
 *
 * @param pTestConfConfig Pointer to TestConfConfig
 * @param pdcch_idx pdcch index
 * @return length for current dci
 */
static U8 lte_pdcch_get_dci_length(BS_CONFOR_TEST_CONFIG *pTestConfConfig,  U32 dci_idx)
{
    return (U8)pTestConfConfig->PdcchDataLen[dci_idx];
}

/** Get dci power
 *
 * @param pTestConfConfig Pointer to TestConfConfig
 * @return power for dci
 */
static S16 lte_pdcch_get_power(BS_CONFOR_TEST_CONFIG *pTestConfConfig)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return (S16)(pTestConfConfig->pdcchRegEpre);
}

/** Create sdu for pdcch
 *
 * @param pDlSubFrame Pointer to DlSubFrame
 * @param pUlSubFrame Pointer to UlSubFrame
 * @param pRxSdu Pointer to RxSduInd
 * @return 1
 */
U32 DCI0DescSet(PDLSUBFRDESC pDlSubFrame,
                PULSUBFRDESC pUlSubFrame,
                PRXSDUIND    pRxSdu)
{
    PDCIULSDUMSG                DciDesc = NULL;
    DCI_DATA_INFO               DciInfo;
    DCI_INFO                    *pDciInfo = get_dci_info(1);
    BS_STATIC_CONFIG            *pBsStaticConfig = get_static_config();
    PLTE_PDCCH_CHAN_STATE       plte_pdcch_chan_state = get_lte_pdcch_chan_state();
    U32 num_bits = 0;
    U32 num_dci0 = 0;
    U8  mcs_type;

    if(testConfConfig.nPdcchUl == 0)
        return 0;
    if(pDlSubFrame == NULL)
        return 0;

// TODO:
#ifdef MX_ARM_ENABLED
    DciDesc = (PDCIULSDUMSG)SvsrAllocMsg();
#else
    DciDesc = (PDCIULSDUMSG)MsgAllocBuffer(MSG_FAST);
#endif
    if(DciDesc == NULL)
    {
        printf("allocation failure\n");
        return 0;
    }
    memset(DciDesc, 0, sizeof(*DciDesc));
    memset(&DciInfo, 0, sizeof(DciInfo));

    //DciDesc->startCCE = chanIdx * testConfConfig.nCcePerPdcch;
    DciDesc->startCCE = plte_pdcch_chan_state->cce_idx_prev[0];//pDlSubFrame->numCtrlChannels * testConfConfig.nCcePerPdcch;
    DciDesc->numCCE = testConfConfig.nCcePerPdcch;

    num_dci0++;

    DciDesc->pTxSdu = (U32*)((U8*)DciDesc + sizeof(DCIULSDUMSG));

//    if (DciDLPrepareFunctionPtr[DciInfo.dciFormat])
//        DciDLPrepareFunctionPtr[DciInfo.dciFormat]((U8*)((U8*)DciDesc + sizeof(DCIULSDUMSG)), &DciInfo, &numBits);
//    else
//        memcpy((U8*)(DciDesc + 1), testConfConfig.payloadPdcchUl, (numBits + 7) >> 3);

// TODO: warning - just for single user
    if(testConfConfig.puschMcs_used)
    {
        mcs_type = testConfConfig.puschMcs[0];
    }
    else
    {
        mcs_type = lte_modulation_get_mcs_index_ul(lte_pusch_get_modulation(&testConfConfig));
    }    

    pDciInfo->mcs_rv = mcs_type;

// toggling ndi
    pDciInfo->ndi = (globalTTITickCount >> 3) & 1;
    dci_encode_0((U32*)(DciDesc + 1), pDciInfo, pBsStaticConfig, &num_bits); 


    DciDesc->msgType = PHY_TXDCIULSDU_REQ;
    DciDesc->rnti = testConfConfig.rntiPusch[0];
    //DciDesc->channelId = chanIdx;
    DciDesc->channelId = global_chanIdx++;
    DciDesc->transmissionMode = testConfConfig.transmissionMode;
    DciDesc->txPowerControl = lte_pdcch_get_power(&testConfConfig);
    DciDesc->numBitsDciUL = num_bits;
    DciDesc->numPayloadBytes = (num_bits + 7) >> 3;
    DciDesc->msgLength = DciDesc->numPayloadBytes;
    //printf("DCI size%d \n", DciDesc->numPayloadBytes);
    //chanIdx++;
    //pDlSubFrame->numberofChannelDescriptors =     chanIdx;

    SduSend(PHY_TXDCIULSDU_REQ, (U8*)DciDesc, sizeof(DCIULSDUMSG) + DciDesc->numPayloadBytes);

    return num_dci0;
}

/** Recalculate dci
 *
 * @param max_cw max codewords
 * @param max_ch_idx max channel indexes
 * @param current_harq_tick current harq tick(id) = 0..7
 * @return Standard result code
 */
RESULTCODE lte_pdcch_update_dci(U32 max_cw, U32 max_ch_idx, U32 current_harq_tick)
{
    PLTE_PDSCH_HARQ_STATE pdsch_harq_state = get_lte_pdsch_harq_state();

    if (pdsch_harq_state->ena_harq == 0)
    {
// the case doesn't support nack and we don't need to update dci.
        return SUCCESS;
    }

    {
        U32 i, ch_idx, loc_index; //nack, redundancy_ver
        U8  modulation_type=0;
        
        BS_STATIC_CONFIG        *pstatic_config    = get_static_config();
        BS_CONFOR_TEST_CONFIG   *pconf_test_config = get_bs_conf_test_config();

        for (i=0; i<max_cw; i++)
        {
            for (ch_idx=0; ch_idx<max_ch_idx; ch_idx++)
            {
                PLTE_PDSCH_SDU_CTRL_RESEND  ppdsch_sdu_ctrl_resend = get_lte_pdsch_sdu_ctrl_resend(i, ch_idx, current_harq_tick);
                PDCI_INFO                   pdci_info = get_dci_info(ch_idx);

/*
                nack = ppdsch_sdu_ctrl_resend->nack;
                redundancy_ver  = ppdsch_sdu_ctrl_resend->rv;

                if ((nack == 0) && (redundancy_ver<3))
                {
                    redundancy_ver++;
                    pdsch_sdu_ctrl_resend[i][ch_idx][current_tti_proc].redundancy_ver = redundancy_ver;
                    pdsch_sdu_ctrl_resend[i][ch_idx][current_tti_proc].need_resend = NACK_HARQ_REPEAT;               
     
    //                dci_info[ch_idx].tb_info[i].ndi = 0;
                    pdci_info[ch_idx]->tb_info[i].ndi = pdsch_sdu_ctrl_resend[i][ch_idx][current_tti_proc].ndi;
                    pdci_info[ch_idx]->tb_info[i].redundancy_ver = redundancy_ver_table[redundancy_ver];
                }*/
// if ack -> copping udp_data to sdu_payload
//                else
                {
                    ppdsch_sdu_ctrl_resend->rv = 0;
                    ppdsch_sdu_ctrl_resend->need_resend = NACK_HARQ_OK;
// toggle ndi
                    ppdsch_sdu_ctrl_resend->ndi = (ppdsch_sdu_ctrl_resend->ndi ^ 1)&1;
                    pdci_info->tb_info[i].ndi = ppdsch_sdu_ctrl_resend->ndi;
                    pdci_info->tb_info[i].redundancy_ver = 0;
                }
                pdci_info->harq_process_id = (U8)current_harq_tick;

// correct MCS-index
                if(pconf_test_config->pdschMcs_used[ch_idx])
                {
                    modulation_type = pconf_test_config->pdschMcs[i][ch_idx];
                }
                else
                {;
//                    modulation_type = GetMcsIndexDl(pconf_test_config->pdschMod[i][ch_idx], i);
                }
                pdci_info->tb_info[i].modulation_type = modulation_type;

// recalculate dci_1a
                if (pdci_info->dci_format == DCI_FORMAT_1A)
                    dci_encode_1a((U32*)&pconf_test_config->payloadPdcch[ch_idx][0], (DCI_INFO *)pdci_info, (BS_STATIC_CONFIG *)pstatic_config, &loc_index);

// if need repeat old packet -> RXSDU clr nack
                ppdsch_sdu_ctrl_resend->nack = NACK_HARQ_OK;

            }
        }
    }
    return SUCCESS;
}

/** Prepares PDCCH channel and inserts it into scheduler queue
 *
 * @param pDciInfo TODO:
 * @param numBits Number of bits in DCI payload
 * @param chId PHY channel ID
 * @param channelType (PDCCH, see enum ChTypeOptions)
 * @param pDci Pointer to the DCI payload
 * @return Standard result code
 */
RESULTCODE lte_pdcch_create_sdu(DCI_DATA_INFO* pDciInfo, U32 num_bits, U8 ch_id, U8 channel_type, U8* pDci)
{
#ifdef MX_ARM_ENABLED
    PTXSDUREQ  pHdr = (PTXSDUREQ) Mac2PhyIfAllocBuffer();
    PTXSDUREQ  pSduReq = pHdr;
#else
    PMSGHEADER pHdr = MsgAllocBuffer(MSG_FAST);
    PTXSDUREQ  pSduReq = (PTXSDUREQ)LteMac2PhyGetDataOffset((PTR)pHdr);;
#endif

    if (pHdr == NULL)
    {
        printf("%s: Failed to allocate memory\r\n", __FUNCTION__);
        return FAILURE;
    }

    if (DciDLPrepareFunctionPtr[pDciInfo->dciFormat])
    {
        DciDLPrepareFunctionPtr[pDciInfo->dciFormat]((U8*) ((U8*) pSduReq + sizeof(TXSDUREQ)),
                                                     pDciInfo, &num_bits);
    }
    else
    {
        memcpy((U8*) ((U8*) pSduReq + sizeof(TXSDUREQ)), pDci, (num_bits + 7) >> 3);
    }

    pSduReq->phyEntityId = 0;
    pSduReq->msgType = PHY_TXSDU_REQ;
    pSduReq->chanId = ch_id;
    pSduReq->cwId = 0;
    pSduReq->uciFormat = 0;
    pSduReq->channelType = channel_type;
    pSduReq->phichGrpNumber = 0;
    pSduReq->maxBitsperCw = num_bits;
    pSduReq->msgLen = (num_bits + 7) >> 3;
    pSduReq->pTxSdu = 0;

#ifdef MX_ARM_ENABLED
// 4gmx
    return SduSend(PHY_TXSDU_REQ | (channel_type << 4), (U8*) pSduReq, pSduReq->msgLen + sizeof(TXSDUREQ));
#else
    #ifdef WIN32
// ccode
        OsalEnqueue(&dlControlTxSduQueue, (OSAL_PADDRESS)pHdr);
        return SUCCESS;
    #else
// linux
        return SduSend(PHY_TXSDU_REQ, (U8*) pSduReq, pSduReq->msgLen + sizeof(TXSDUREQ));
    #endif
#endif
}

/* Function addes PDCCH channel to TXVECTOR with DCI format
 *
 * @param pCh Pointer to DlChanDesc into TxVector
 * @param id PHY channel ID
 * @param pMapInfo pointer to mapinfo
 * @param dci_idx current dci index
 * @return Standard result code
 */
RESULTCODE lte_add_pdcch(PDCICHANNELDESC pCh,
                         U32 id,
                         MAPPINGINFO *pMapInfo,
                         U32 dci_idx)
{

    DCI_DATA_INFO DciInfo;
    BS_CONFOR_TEST_CONFIG * pTestConfConfig= get_bs_conf_test_config();

    PRINT_FUNCTION_NAME(__FUNCTION__);

    /* Fill Chann desc */
    pCh->offsetNextCh = NOT_SPECIFIED_VALUE;

    pCh->channelId = (U16)id;
    pCh->txpowerControl = lte_pdcch_get_power(pTestConfConfig);

    pCh->crcLength = SIXTEEN;
    pCh->crcScrambling = 1;

    pCh->channelType = PDCCH;

    pCh->reserved = NOT_SPECIFIED_VALUE;

    pCh->numCodeWords = 1;
    pCh->nLayers = pBsMacDesc->initParm.txAntennaPortCount;
    if (pBsMacDesc->initParm.txAntennaPortCount <= TXANT_TWO)
    {
        pCh->transmissionMode = pBsMacDesc->initParm.txAntennaPortCount;
    }
    else
    {
        pCh->transmissionMode = TXDIVERSITY;
    }

    pCh->reserved1 = NOT_SPECIFIED_VALUE;
#ifndef AC_MSPD_TDD
    pCh->scrmblerInitValue = (pTestConfConfig->PdcchRntiEna)? pTestConfConfig->PdcchRnti[dci_idx] :
                             ( (pTestConfConfig->TestModel == E_TM_3_2 ||  pTestConfConfig->TestModel == E_TM_3_3 ) ?  
                             lte_rnti_get_deboosted_rnti(pTestConfConfig, 0) :  lte_rnti_get_boosted_rnti(pTestConfConfig, 0) );
#else
    pCh->scrmblerInitValue =  (pTestConfConfig->PdcchRntiEna)? pTestConfConfig->PdcchRnti[dci_idx] : TestMgrGetBoostedRnti(pTestConfConfig, 0);

#endif
    /* Fill Subchan Desc */
    lte_pdcch_get_mapinfo(pTestConfConfig,
                          pCh,
                          id,
                          pMapInfo);

    SetDLDciCtx(pTestConfConfig, &DciInfo, id);

#ifdef MX_ARM_ENABLED
    if(pTestConfConfig->payloadPdcch_unified)
        dci_idx = 0;
#endif
    
    /* Generate PHY TXSDU.req for this channel */
    lte_pdcch_create_sdu(&DciInfo,
                         lte_pdcch_get_dci_length(pTestConfConfig, dci_idx), 
                         (U8)id,
                         PDCCH,
                         &pTestConfConfig->payloadPdcch[dci_idx][0]);
    
    return SUCCESS;
}



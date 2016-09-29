/** @file lte_phich.c
 *
 * @brief Implemenataion for creating PHICH message
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "LtePhyL2Api.h"
#include "resultcodes.h"
#include "ltemaccore.h"
#include "bs_static_config.h"
#ifndef MX_ARM_ENABLED
#include "msgroute.h"
#endif

#ifdef MX_ARM_ENABLED
#include "enbMac2PhyIface.h"
#include "supervisor.h"            // SvsrAllocMsg
#else
#include "LteMac2PhyInterface.h"
#endif

#include "LteMacTestCfgMgr.h"

#include "lte_mac_typedef.h"
#include "lte_mac_state.h"
#include "lte_rnti.h"
#include "lte_phich.h"



U32 global_chanIdx = 0;


/** get n Group
 *
 * @param pbs_static_config Pointer to bs_static_config
 * @return no
 */
void lte_phich_calc_n_group(BS_STATIC_CONFIG *pbs_static_config)
{
    if(pbs_static_config->cp == CP_NORMAL){
       pbs_static_config->n_phich_group = ((pbs_static_config->six_ng * ((pbs_static_config->n_dl_rb + 7)/ 8) + 5)/6);
       pbs_static_config->two_nsf_phich = 8;
    }
    else{
       pbs_static_config->n_phich_group = 2 * ((pbs_static_config->six_ng * ((pbs_static_config->n_dl_rb + 7)/ 8) + 5)/6);
       pbs_static_config->two_nsf_phich = 4;
    }
    return;
}


/** get PCFICH register
 *
 * @return 4 always
 */
int lte_phich_get_pcfich_regs(void)
{
    return 4;
}


/** get PHICH register
 *
 * @param ng          ???
 * @param frame_mode  FDD (0) or TDD(1)
 * @param uldlcfg     ???    
 * @param sfn         Sub frame number
 * @return minimal RB-value 
 */
int lte_phich_get_regs(int ng, 
                       int frame_mode, // 0 - fdd, 1 - tdd
                       int uldlcfg, 
                       int sfn)
{
    static int mphich[][10] = 
    {
        {2,1,0,0,0,2,1,0,0,0},
        {0,1,0,0,1,0,1,0,0,1},
        {0,0,0,1,0,0,0,0,1,0},
        {1,0,0,0,0,0,0,0,1,1},
        {0,0,0,0,0,0,0,0,1,1},
        {0,0,0,0,0,0,0,0,1,0},
        {1,1,0,0,0,1,1,0,0,1}
    };

    if( frame_mode == 0 )
        return ng * 3;
    else
        return 3 * ng * mphich[ uldlcfg ] [ sfn % 10];
}


/** get minimal num rb into mapinfo
 *
 * @param  pmapinfo Pointer to pmapinfo
 * @return minimal RB-value 
 */
U16 lte_phich_get_lower_rb(PMAPPINGINFO pmapinfo)
{
    U16 i, i_lower_rb = 0x7fff;

    if (pmapinfo->numberofEntries)
    {
        for (i=0; i<pmapinfo->numberofEntries; i++)
        {
            if (i_lower_rb > pmapinfo->reselmInfo[i].startRes)
            {
                i_lower_rb = pmapinfo->reselmInfo[i].startRes;
            }
        }
    }
    return i_lower_rb;
}


/** get I ( it needs for calculate Phich Group Number)
 *
 * @return I
 */
U16 lte_phich_get_i(void)
{

// 0 for FDD
    return 0;
}


/** get power value for PHICH channel
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @return power in 2Q8 format
 */
S16 lte_phich_get_power(void)
{
    BS_CONFOR_TEST_CONFIG*  pTestConfConfig = get_bs_conf_test_config();

    PRINT_FUNCTION_NAME(__FUNCTION__);
    return (S16)(pTestConfConfig->phichSymPower);
}

U16 TestMgrGetNumPhich(BS_CONFOR_TEST_CONFIG *pTestConfConfig)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return(U16)(pTestConfConfig->nPhichGroup * pTestConfConfig->nPhichPerGroup);
}


/** Enable PHICH channel usage in current session and inserts PHICH into scheduler queue
 *
 * @param chId PHY channel ID
 * @param groupId TODO:
 * @param seqIdx TODO:
 * @param channelType PHY channel type (PHICH, see enum ChTypeOptions)
 * @param nackAck TODO:
 * @return Standard result code
 */
RESULTCODE lte_phich_create_sdu(U8 ch_id, U8 group_id, U8 seq_idx, U8 channel_type, U8 nack_ack)
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

    memset(pSduReq, 0x00, sizeof(TXSDUREQ));
    pSduReq->phyEntityId = 0;
    pSduReq->msgType = PHY_TXSDU_REQ;
    pSduReq->chanId = ch_id;
    pSduReq->nackAck = nack_ack;
    pSduReq->cwId = 0;
    pSduReq->uciFormat = 0;
    pSduReq->phichSeqIndex = seq_idx;
    pSduReq->channelType = channel_type;
    pSduReq->phichGrpNumber = group_id;
    pSduReq->maxBitsperCw = 0;
    pSduReq->msgLen = 0;
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


/* Function addes PHICH channel to TXVECTOR with DCI format
 *
 * @param pCh Pointer to DlChanDesc into TxVector
 * @param id PHY channel ID
 * @param group_id number of group
 * @return Standard result code
 */
RESULTCODE lte_add_phich(PDCICHANNELDESC pCh,
                         U32 id,
                         U8 group_id,
                         U8 seq_idx)
{
    BS_CONFOR_TEST_CONFIG * pTestConfConfig= get_bs_conf_test_config();
    U8 nack_ack = 0;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    /* Fill Chann desc */
    pCh->offsetNextCh = NOT_SPECIFIED_VALUE;

    pCh->channelId = (U16)id;
    pCh->txpowerControl = lte_phich_get_power();

    pCh->crcLength = SIXTEEN;
    pCh->crcScrambling = 1;

    pCh->channelType = PHICH;

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

    pCh->scrmblerInitValue = lte_rnti_get_boosted_rnti(pTestConfConfig, 0);

    // No mapping
    pCh->numberofEntries = 0;
    pCh->startRes = 0;
    pCh->numRes = 0;

    // Generate PHY TXSDU.req for this channel
    lte_phich_create_sdu((U8)id, group_id, seq_idx, PHICH,  nack_ack);

    return SUCCESS;
}


/* Function addes PHICH channel to TXVECTOR with DCI format
 *
 * @param pDlSubFrame Pointer to DlSubFrame
 * @param pUlSubFrame Pointer to UlSubFrame
 * @return number of Phich
 */
U32 lte_phich_calc(PDLSUBFRDESC pDlSubFrame,
                   PULSUBFRDESC pUlSubFrame,
                   PRXSDUIND    pRxSdu)
{
    PHIINFOMSGDESC          HiDesc;
    BS_STATIC_CONFIG        *pbs_static_config = get_static_config();
    PLTE_PDSCH_HARQ_STATE   plte_pdsch_harq_state = get_lte_pdsch_harq_state();

    U8 Idx = 0, i;
    //U32 chanIdx = pDlSubFrame->numCtrlChannels;
    U32 HiNum = 0;
    U16 n_drms;
    U16 i_lower_rb;
    U32 cur_tti = get_globalTTITickCount()&7;

//    printf ("#### HiDescSet, nPhichGroup=%d, nPhichPerGroup=%d, chanIdx=%d, pRxSdu->channelId=%d\r\n", testConfConfig.nPhichGroup, testConfConfig.nPhichPerGroup, global_chanIdx, 0);
    if(testConfConfig.phichIdx != 0xFF)
    {
        for (Idx = 0; Idx < testConfConfig.nPhichGroup; Idx++)
        {

// TODO:
#ifdef MX_ARM_ENABLED
            HiDesc = (PHIINFOMSGDESC)SvsrAllocMsg();
#else
            HiDesc = (PHIINFOMSGDESC)MsgAllocBuffer(MSG_FAST);
#endif
            if(HiDesc == NULL)
            {
                printf("HiDesc allocation failure\n");
                return 0;
            }
            memset(HiDesc, 0, sizeof(*HiDesc));
            HiDesc->msgType = PHY_TXHISDU_REQ;
            HiDesc->phyEntityId = 0;


            if (plte_pdsch_harq_state->ena_udp_stream)
            {
                HiDesc->nackAck = 1;

                if (pUlSubFrame)
                {
                    i_lower_rb = lte_phich_get_lower_rb((PMAPPINGINFO)(&pUlSubFrame->ulCh[pRxSdu->channelId].ulSubChInfo.mapInfo));
                }
                else
                {
                    i_lower_rb = 0;
                }
                
                n_drms = 0;

                HiDesc->phichGroupNumber = ((i_lower_rb + n_drms)%pbs_static_config->n_phich_group) + (pbs_static_config->n_phich_group * lte_phich_get_i());
                HiDesc->phichSequenceIndex = ((i_lower_rb/pbs_static_config->n_phich_group)+n_drms)%pbs_static_config->two_nsf_phich;
            }
            else
            {
                if(pUlSubFrame && pUlSubFrame->ulCh[pRxSdu->channelId].ulSubChInfo.harqInfo.nDi != 1)
                    HiDesc->nackAck = 1;
                else
                    HiDesc->nackAck = 0;

                HiDesc->phichSequenceIndex = testConfConfig.phichIdx;
                HiDesc->phichGroupNumber = Idx;
            }

//            HiDesc->nLayers = pBsMacDesc->initParm.txAntennaPortCount;
            if (pBsMacDesc->initParm.txAntennaPortCount <= TXANT_TWO)
            {
                HiDesc->transmissionMode = pBsMacDesc->initParm.txAntennaPortCount;
            }
            else
            {
                HiDesc->transmissionMode = TXDIVERSITY;
            } 
            HiDesc->channelId = global_chanIdx ++;
            HiDesc->txPowerControl = lte_phich_get_power();
            
            SduSend(PHY_TXHISDU_REQ, (U8*)HiDesc, HiDesc->msgLength + sizeof(HIINFOMSGDESC));
            HiNum++;
        }
    }
    else
    {
        for (Idx = 0;
            Idx < testConfConfig.nPhichGroup;
            Idx++)
        {
            for (i = 0; i < testConfConfig.nPhichPerGroup; i++)
            {
    // TODO:
#ifdef MX_ARM_ENABLED
                HiDesc = (PHIINFOMSGDESC)SvsrAllocMsg();
#else
                HiDesc = (PHIINFOMSGDESC)MsgAllocBuffer(MSG_FAST);
#endif
                if(HiDesc == NULL)
                {
                    printf("HiDesc allocation failure\n");
                    return 0;
                }
                memset(HiDesc, 0, sizeof(*HiDesc));
                HiDesc->msgType = PHY_TXHISDU_REQ;
                HiDesc->phyEntityId = 0;

                if (plte_pdsch_harq_state->ena_udp_stream)
                {
                    HiDesc->nackAck = 1;

                    if (pUlSubFrame)
                    {
                        i_lower_rb = lte_phich_get_lower_rb((PMAPPINGINFO)(&pUlSubFrame->ulCh[pRxSdu->channelId].ulSubChInfo.mapInfo));
                    }
                    else
                    {
                        i_lower_rb = 0;
                    }

                    n_drms = 0;

                    HiDesc->phichGroupNumber = ((i_lower_rb + n_drms)%pbs_static_config->n_phich_group) + (pbs_static_config->n_phich_group * lte_phich_get_i());
                    HiDesc->phichSequenceIndex = ((i_lower_rb/pbs_static_config->n_phich_group)+n_drms)%pbs_static_config->two_nsf_phich;
                }
                else
                {
                    if(pUlSubFrame && pUlSubFrame->ulCh[pRxSdu->channelId].ulSubChInfo.harqInfo.nDi != 1)
                        HiDesc->nackAck = 1;
                    else
                        HiDesc->nackAck = 0;
                    
                    HiDesc->phichSequenceIndex = ( testConfConfig.nPhichPerGroup < 3 )? i * 4 : i;
                    HiDesc->phichGroupNumber = Idx;
                }

                HiDesc->transmissionMode = testConfConfig.transmissionMode;
                //HiDesc->channelId = chanIdx;
                HiDesc->channelId = global_chanIdx ++;
                HiDesc->txPowerControl = lte_phich_get_power();

                SduSend(PHY_TXHISDU_REQ, (U8*)HiDesc, HiDesc->msgLength + sizeof(HIINFOMSGDESC));
                HiNum++;
            }
        }
    }
    return HiNum;
}


/** Creates fake HI DCI payload
 *
 * @param pDlSubFrame Pointer to the current Downlink vector
 * @param pUlSubFrame Pointer to the current Uplink vector
 * @param pRxSdu TODO:
 * @return Standard result code
 */
RESULTCODE lte_phich_dci_fake(PDLSUBFRDESC pDlSubFrame, 
                              PULSUBFRDESC pUlSubFrame, 
                              PRXSDUIND pRxSdu)
{
    return SUCCESS;
}


/** Creates and sends HI DCI payload
 *
 * @param pDlSubFramePointer to the current Downlink vector
 * @param pUlSubFramePointer to the current Uplink vector
 * @param pRxSdu Pointer to the HI DCI SDU payload
 * @return Standard result code
 */
RESULTCODE lte_phich_dci_set(PDLSUBFRDESC pDlSubFrame, PULSUBFRDESC pUlSubFrame, PRXSDUIND pRxSdu)
{
    PHIADCIULMSGDESC phi_dci_desc = NULL;
    U32 hi_num = 0;
    U32 dci0_num = 0;

    hi_num = lte_phich_calc(pDlSubFrame, pUlSubFrame, pRxSdu);
    dci0_num = DCI0DescSet(pDlSubFrame, pUlSubFrame, pRxSdu);

    if (hi_num == 0 && dci0_num == 0)
    {
        return SUCCESS;
    }

#ifdef MX_ARM_ENABLED
    phi_dci_desc = (PHIADCIULMSGDESC) SvsrAllocMsg();
#else
    phi_dci_desc = (PHIADCIULMSGDESC) MsgAllocBuffer(MSG_FAST);
#endif

    if (phi_dci_desc == NULL)
    {
        printf("HiDci Allocation failed\n");
        return FAILURE;
    }
    memset(phi_dci_desc, 0, sizeof(HIADCIULMSGDESC));

    phi_dci_desc->phyEntityId = 0;
    phi_dci_desc->msgLength = 0;
    phi_dci_desc->msgType = PHY_TXHIADCIUL_REQ;

    phi_dci_desc->subFrameNumber = getSubFrameNum();
    phi_dci_desc->frameNumber = getFrameNum();

    phi_dci_desc->numDciUlSdus = dci0_num;
    phi_dci_desc->numHiSdus = hi_num;

#ifdef MX_ARM_ENABLED
    SduSend(PHY_TXHIADCIUL_REQ, (U8*) phi_dci_desc, sizeof(HIADCIULMSGDESC));
#else
    LteMac2PhyAddToTail(PHY_TXHIADCIUL_REQ, sizeof(HIADCIULMSGDESC), (U8*) phi_dci_desc);
#endif
    return SUCCESS;
}


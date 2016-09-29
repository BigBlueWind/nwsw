/** @file lte_pbch.c
 *
 * @brief Implemenataion for creating PBCH message
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
#include "LteMacTestCfgMgr.h"
#ifndef MX_ARM_ENABLED
#include "LteMac2PhyInterface.h"
#include "msgroute.h"
#include "osal.h" // dlControlTxSduQueue
#endif


#ifdef MX_ARM_ENABLED
#include "enbMac2PhyIface.h"        // SduSend
#endif

#include "bsmac.h" // BS_CONTEXT
#include "lte_mac_typedef.h"

#ifndef MX_ARM_ENABLED
extern OSAL_QUEUE dlControlTxSduQueue;
#endif
extern BS_CONTEXT *pBsMacDesc;


/** get cell id (need for PBCH channel)
 *
 * @return cell id
 */
U16 lte_pbch_get_cell_id(void)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    /* PHY suports only 0 now */
    return pBsMacDesc->initParm.phyLayerCellIdGroup;
}


/** get power value for PBCH channel
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @return power in 2Q8 format
 */
S16 lte_pbch_get_power(BS_CONFOR_TEST_CONFIG *pTestConfConfig)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    return (S16)(pTestConfConfig->pbchEpre);
}


/** Inserts PBCH channel into scheduler queue
 *
 * @param num_bits Number of bits in SDU payload
 * @param ch_id PHY channel ID
 * @param channel_type (PBCH, see enum ChTypeOptions)
 * @param pbch_payload Pointer to PBCH channel payload
 * @return Standard result code
 */
RESULTCODE lte_pbch_create_sdu(U32 num_bits, U8 ch_id, U8 channel_type, U8* pbch_payload)
{

#ifdef MX_ARM_ENABLED
    PTXSDUREQ  pHdr = (PTXSDUREQ) Mac2PhyIfAllocBuffer();
    PTXSDUREQ  pSduReq = pHdr;
#else
    PMSGHEADER pHdr = MsgAllocBuffer(MSG_FAST);
    PTXSDUREQ  pSduReq = (PTXSDUREQ)LteMac2PhyGetDataOffset((PTR)pHdr);
#endif

    if (pHdr == NULL)
    {
        return FAILURE;
    }

    memset(pSduReq, 0x00, sizeof(TXSDUREQ));
    pSduReq->phyEntityId = 0;
    pSduReq->msgType = PHY_TXSDU_REQ;
    pSduReq->chanId = ch_id;
    pSduReq->cwId = 0;
    pSduReq->uciFormat = 0;
    pSduReq->nackAck = 0;
    pSduReq->channelType = channel_type;
    pSduReq->phichGrpNumber = 0;
    pSduReq->maxBitsperCw = num_bits;
    pSduReq->msgLen = (num_bits+7) >> 3;
    pSduReq->pTxSdu = 0;

    memcpy((U8*)((U8*)pSduReq + sizeof(TXSDUREQ)), pbch_payload, (num_bits+7) >> 3);

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


/* Function addes sub PBCH channel to TXVECTOR
 *
 * @param  pTestConfConfig Poiter to TestConfConfig
 * @param pCh Pointer to DlChanDesc into TxVector
 * @param ch_id PHY channel ID
 * @return Standard result code
 */
static RESULTCODE lte_pbch_fill_sub_desc(BS_CONFOR_TEST_CONFIG *pTestConfConfig,
                                         PDLSUBCHINFO pSubCh,
                                         U32 ch_id)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    pSubCh->codingDescriptor = TBVITCC;
    pSubCh->blockCodeConcatenation = 0;
    pSubCh->modulationType = QPSK;
    pSubCh->mcsType = 4;

    pSubCh->reserved = NOT_SPECIFIED_VALUE;
    pSubCh->nDi =  0;
#ifdef MX_ARM_ENABLED
    pSubCh->rV = pTestConfConfig->pbchRv;
#else
    pSubCh->rV = 0;
#endif
    pSubCh->flushReq = 0;

    return SUCCESS;
}


/* Function addes PBCH channel to TXVECTOR with DCI format
 *
 * @param pCh Pointer to DlChanDesc into TxVector
 * @param id PHY channel ID
 * @return Standard result code
 */
RESULTCODE lte_add_pbch(PDLCHANDESC pCh,
                        U32 id)
{
    U32 i;
    BS_CONFOR_TEST_CONFIG * pTestConfConfig = get_bs_conf_test_config();

    PRINT_FUNCTION_NAME(__FUNCTION__);
    /* Fill Chann desc */
    pCh->offsetNextCh = NOT_SPECIFIED_VALUE;

    pCh->channelId = (U16)id;

    pCh->txpowerControl = lte_pbch_get_power(pTestConfConfig);

    pCh->persistEnable = 1;
    pCh->repeatCycle =  1;

    pCh->channelType = PBCH;
    pCh->hCid = 0;
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

    lte_pbch_fill_sub_desc(pTestConfConfig,
                           &pCh->subChInfo[0],
                           id);

    pCh->crcInfo.crcLength = SIXTEEN;
    pCh->crcInfo.crcScrambling = 1;
    pCh->scrInfo.scramblerType = DLSCR;
    pCh->scrInfo.scrinitValueinput = lte_pbch_get_cell_id();

    pCh->dlPrecoderInfo.cddType = 0;
    pCh->dlPrecoderInfo.codeBookIdx = 0;

    pCh->dlPrecoderInfo.reserved = NOT_SPECIFIED_VALUE;

    for (i = 0; i < 8; i++)
    {
        pCh->dlPrecoderInfo.codebookSubsetRestriction[i] = 0;
    }

    memset(&pCh->mapInfo, 0, sizeof(MAPPINGINFO));

    /* Generate PHY TXSDU.req for this channel */
    lte_pbch_create_sdu(PBCH_SIZE_ONE_SUBFRAME_BITS, (U8)id, PBCH,
                                 &pTestConfConfig->payloadPbch[0]);

    return SUCCESS;
}


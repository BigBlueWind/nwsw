/** @file table.c
 *
 * @brief Tables
 * @author Mindspeed Technologies
 * @version $Revision: 1.4.8.1 $
 *
 * COPYRIGHT(c) 2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#if defined (__linux__) || defined (WIN32)
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#endif

#include "e_tm.h"
#include "e_tm_defines.h"
#include "e_tm_tables.h"
#include "bs_static_config.h"

#ifdef WIN32
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#else 
    #ifdef MX_ARM_ENABLED
      #include "supervisor.h"
      #include "appprintf.h"
    #else

    #endif
#endif

/****************************************************************************
 Private Types
 ****************************************************************************/

typedef struct _stCW_CFG
{
    U8 Mcs;
    U8 Mod;
    U8 Rvi; //3 In Lte used U16
}CW_CFG, *PCW_CFG;

typedef struct _stPDSCH_CFG
{
    CW_CFG CwCfg[2];
    U16 Rnti;
    U16 ueCategory;
    U32 Pa;
    MAPPINGINFO Map; //has to be a last parameter
}PDSCH_CFG, *PPDSCH_CFG;

typedef struct etm_test_mgr_ctx
{
    U8 **boostingMap[MAX_LAYERS][MAX_SDU_CHANNELS];
    const U8 **boostingUlMap;
}TEST_MGR_CTX;

typedef struct MibInfoT
{
    unsigned int systemFrameNumberHi:2;
    unsigned int phich_resource:2;  // enum {oneSixth, half, one, two}
    unsigned int phich_duration:1;  // enum {normal, extended}
    unsigned int dl_Bandwidth:3;    // enum {n6, n15, n25, n50, n75, n100} - firstly transmitted byte
    unsigned int spareHi:2;
    unsigned int systemFrameNumberLo:6; // 8 bits
    unsigned int spareLo:8; // 10 bits
}MIB_INFO;

// 36.211 - Table 6.9.3-1
enum PHICH_DURATION
{
    PHICHDUR_NORMAL = 1,
    PHICHDUR_EXTEND2,
    PHICHDUR_EXTEND3,
};

enum PHICH_MIB_DURATION
{
    PHICHDUR_MIB_NORMAL = 0,
    PHICHDUR_MIB_EXTEND,
};

typedef struct E_TM_CONFORMANCE_CONFIG_STRUCT
{
    /* Select model */
    U8 TestModel;

    /* The following general parameters are used by all E-UTRA test models: */
    U8 nAntennaPort;    /* using p = 0 */
    U8 nCw;             /* 1 code word */
    U8 nLayers;         /* 1 layer */
    U8 precodingMode;   /* not used */
    U32 duration;       /* 10 subframes */
    U8 cp;
    U8 transmissionMode;
    U8 cddType;
    U8 codeBookIdx;
    U8 arRctRvDl[2][10];

    /* Virtual resource blocks of localized type, no intra-subframe hopping for PDSCH */

    /* UE-specific reference signals are not used */
    U32 RctMcsDl[4];

    /* Referance, Syncronisation Signals */
    U32 rsBoosting;     /* RS boosting, PB = EB/EA */
    S16 epreSinchro;    /* Synchronisation signal EPRE / ERS [dB] */

    S32 reservedEpre;   /* Reserved EPRE / ERS [dB] */

    /* PBCH */
    U32 pbchEna;
    S16 pbchEpre;         /* PBCH EPRE / ERS [dB] */
    S32 pbchReservedEpre; /* ReservedEPRE / ERS [dB]/ ERS [dB] */
    U8 payloadPbch[BCH_MAX_SIZE];

    /* PCFICH */
    U8 nCtrlSymbols;      /* # of symbols used for control channels */
    S16 pcfichEpre;         /* PCFICH EPRE / ERS [dB] */

    /* PHICH */
    U8 nPhichGroup;      /* # of PHICH groups */
    U8 nPhichPerGroup;   /* # of PHICH per group */
    U8 PhichGroupSfn;    //for Tdd
    S16 phichSymPower;   /* PHICH BPSK symbol power / ERS [dB] */
    S32 phichGroupEpre;  /* PHICH group EPRE / ERS */

    /* PDCCH */
    U8 nAvalableRegs;   /* # of available REGs */
    U8 nPdcch;          /* # of PDCCH */
    U8 nCcePerPdcch;    /* # of CCEs per PDCCH */
    U8 nRegPerCce;      /* # of REGs per CCE */
    U8 nRegAllToPdcch;  /* # of REGs allocated to PDCCH */
    U8 nNilRegPadding;  /* # of <NIL> REGs added for padding */
    S16 pdcchRegEpre;   /* PDCCH REG EPRE / ERS [dB] */
    S32 nilRegEpre;     /* <NIL> REG EPRE / ERS [dB] */

    U8 pdcch_map_used;
    RESELEMINFO pdcch_map[MAX_PDCCH_CHANNELS][10];
    U8 pdcch_map_unified[MAX_PDCCH_CHANNELS];

    /* DCI  */
    U8 payloadPdcch[MAX_PDCCH_CHANNELS][DCI_MAX_SIZE];
    U32 PdcchDataLen[MAX_PDCCH_CHANNELS];
    U16 PdcchRnti[MAX_PDCCH_CHANNELS];
    U16 PdcchRntiEna;

    /* PDSCH */
    U16 nPdsch[MAX_LAYERS];
    U32 pdschDataLen[MAX_PDSCH_CHANNELS];
    U8  nPdschPrb[MAX_LAYERS][MAX_SDU_CHANNELS];   /* # of QPSK PDSCH PRBs which are boosted */
    S16 pdschPrbPa[MAX_LAYERS][MAX_SDU_CHANNELS];         /* PRB PA = EA/ERS [dB] */
    U8  pdschMod[MAX_LAYERS][MAX_SDU_CHANNELS];

    U8 pdschPayload;
    U8 pdschPayloadEna;

    U8 pdschMcs_used[2/*per CW*/];
    U8 pdschMcs[2/*per CW*/][MAX_PDSCH_CHANNELS/*per Ch*/];

    U8 pdschPayloadMode[2/*per CW*/][MAX_PDSCH_CHANNELS/*per Ch*/];

    RESELEMINFO pdsch_map[MAX_PDSCH_CHANNELS/*per Ch*/];
    U8 pdsch_map_used;

    U8 pdschRnti_used;
    U16 pdschRnti[MAX_PDSCH_CHANNELS/*per Ch*/];
  
    PDSCH_CFG PdschCfg[16/* per CH */];
    U32 PdschCfgEna;
    
    /* PHICH */
    U8 phichResource;
    U8 phichDuration;
    U8 phichIdx;
} E_TM_CONFOR_CONFIG;

typedef U32 (*tAddToTail)(U8 MessageType, U32 MessageLen, U8* tempo, U16 fn, U8 sn);
typedef struct stMac2PhyQueue
{
    PMAC2PHY_QUEUE_EL StartQueueCurr;
    PMAC2PHY_QUEUE_EL TailQueueCurr;
    tAddToTail pAddToTail;
} MAC2PHY_QUEUE, *PMAC2PHY_QUEUE;

/****************************************************************************
 * Global Variables
 ****************************************************************************/

static TEST_MGR_CTX     etm_test_mgr_ctx;
static E_TM_CONFOR_CONFIG    etm_test_config;
static BS_STATIC_CONFIG    e_tm_static_config;

static U8 etm_mcsi_index_table_dl[4][2] = 
    {{0, 0},
     {0, 0},
     {0, 0},
     {0, 0}};

static DLSUBFRDESC SubFrame;    
static PDLSUBFRDESC pSubFrame = &SubFrame;    
static ULSUBFRDESC UlSubFrame;    
static PULSUBFRDESC pUlSubFrame = &UlSubFrame;    

static U32 global_chanIdx = 0;

static MAC2PHY_QUEUE Mac2PhyQueue;

/****************************************************************************
 * Privat Definitions
 ****************************************************************************/

#define NOT_SPECIFIED_VALUE   (0)
#define BLOCK_SIZE 16384 //One block size
#define PBCH_SIZE_ONE_SUBFRAME_BITS (24)

/****************************************************************************
 * Privat Functions (Down level)
 ****************************************************************************/

/** Allocates memory block for PHY API message
 *
 * @return Pointer to a new message (84xxx-SWG-009-A) or NULL on failure
 */
static PGENMSGDESC Mac2PhyIfAllocBuffer(void)
{
    //UINT32 lr = __return_address();
#ifdef WIN32
    PGENMSGDESC pMsg = (PGENMSGDESC) malloc (BLOCK_SIZE);
#else
    #ifdef MX_ARM_ENABLED
    PGENMSGDESC pMsg = (PGENMSGDESC) SvsrAllocMsg();
    #else
        PGENMSGDESC pMsg = NULL;
    #endif
#endif

    if (pMsg == NULL)
    {
        //printf("Mac2PhyIfAllocBuffer - failed to allocate memory, cpu-%d, lr=%x\r\n", MxGetCpuID(), lr);
        printf("Mac2PhyIfAllocBuffer - failed to allocate memory");
    }
    return (PGENMSGDESC) pMsg;
}

/** Allocates memory block for PHY API message
 *
 * @return Pointer to a new message (84xxx-SWG-009-A) or NULL on failure
 */
static PGENMSGDESC Mac2PhyIfAllocBufferEx(U32 nSize)
{
    //U32 lr = __return_address();
#ifdef WIN32
    PGENMSGDESC pMsg = (PGENMSGDESC) malloc (nSize);
#else
    #ifdef MX_ARM_ENABLED
    PGENMSGDESC pMsg = (PGENMSGDESC) SvsrAllocMsgEx(nSize);
    #else
    PGENMSGDESC pMsg = NULL;
    #endif
#endif
    if (NULL == pMsg)
    {
        //printf("Mac2PhyIfAllocBufferEx - failed to allocate memory, cpu-%d, lr=%x\r\n", MxGetCpuID(), lr);
        printf("Mac2PhyIfAllocBufferEx - failed to allocate memory");
    }
    return (PGENMSGDESC) pMsg;
}

/** Inserts PBCH channel into scheduler queue
 *
 * @param numBits Number of bits in SDU payload
 * @param chId PHY channel ID
 * @param channelType (PBCH, see enum ChTypeOptions)
 * @param testPbch Pointer to PBCH channel payload
 * @return Standard result code
 */
static RESULTCODE LtePhyPrepareTestZeroSduPbch(U32 numBits, U16 chId, U8 channelType, U8 *testPbch, U16 fn, U8 sn)
{
    PTXSDUREQ pSduReq = (PTXSDUREQ) Mac2PhyIfAllocBuffer();

    if (pSduReq == NULL)
    {
        printf("Failed to allocate PTXSDUREQ, LtePhyPrepareTestZeroSduPbch function\r\n");
        return FAILURE;
    }

    pSduReq->phyEntityId = 0;
    pSduReq->msgType = PHY_TXSDU_REQ;
    pSduReq->chanId = chId;
    pSduReq->cwId = 0;
    pSduReq->channelType = channelType;
    pSduReq->phichGrpNumber = 0;
    pSduReq->maxBitsperCw = numBits;
    pSduReq->msgLen = numBits >> 3;
    pSduReq->pTxSdu = 0;
    memset((U8*) ((U8*) pSduReq + sizeof(TXSDUREQ)), 0, numBits >> 3);

    return Mac2PhyQueue.pAddToTail(PHY_TXSDU_REQ | (PBCH << 4), pSduReq->msgLen
            + sizeof(TXSDUREQ), (U8*) pSduReq, fn, sn);
}

static U8 TestMgrGetNumConsecutiveRBs(U8 **pMap, U32* num)
{
    U8 startRb = **pMap;
    U8 *pNextRb = *pMap + 1;
    U8 count = 0;

    while (startRb + (++count) == *pNextRb++)
    {
        if(--(*num) == 0)
            break;
    }

    *pMap = pNextRb - 1;
    return count;
}

/** Compresses TXVECTOR structure by removing gaps and adjusting offsets
 *
 * @param pDest Destination buffer
 * @param pVec Original Downlink vector
 * @return New Downlink vector size
 */
static U32 lte_if_copy_tx_vector_etm(U8* pDest, PDLSUBFRDESC pVec)
{
    U32 len = 0;
    U32 num_ctrl_chan = pVec->numCtrlChannels;
    U32 num_data_chan = pVec->numberofChannelDescriptors - num_ctrl_chan;

    if (pDest == NULL)
    {
        return 0;
    }
    
    // Copy header
    memcpy(pDest, pVec, (U32)&pVec->dlCh[0] - (U32)pVec);
    len += (U32)&pVec->dlCh - (U32)pVec;
    // Copy dlCh
    memcpy(pDest + len, pVec->dlCh, num_data_chan * sizeof(pVec->dlCh[0]));
    len += num_data_chan * sizeof(pVec->dlCh[0]);
    // Copy dciCh
    memcpy(pDest + len, pVec->dciCh,  num_ctrl_chan* sizeof(pVec->dciCh[0]));
    len +=  num_ctrl_chan * sizeof(pVec->dciCh[0]);
    // Copy dlCmnPwrCtl
    memcpy(pDest + len, &pVec->dlCmnPwrCtl, sizeof(pVec->dlCmnPwrCtl));
    len += sizeof(pVec->dlCmnPwrCtl);

    // Overwrite offsets
    {
        PDLSUBFRDESC pDestVect = (PDLSUBFRDESC)pDest;

        pDestVect->offsetDCIChannels =
            (U32)&pDestVect->dlCh + num_data_chan * sizeof(pVec->dlCh[0]) - (U32)pDestVect;
        pDestVect->offsetPowerCtrl =
            pDestVect->offsetDCIChannels + num_ctrl_chan * sizeof(pVec->dciCh[0]);
    }

    return len;
}


/** Compresses RXVECTOR structure by removing gaps and adjusting offsets
 *
 * @param pDest Destination buffer
 * @param pVec Original Uplink vector
 * @return New Uplink vector size
 */
static U32 lte_if_copy_rx_vector_etm(U8* pDest, PULSUBFRDESC pVec)
{
    U32 len = 0;
    U32 num_ctrl_chan = pVec->numberOfCtrlChannelDescriptors;
    U32 num_data_chan = pVec->numberofChannelDescriptors - num_ctrl_chan;
    U32 num_srs_chan = pVec->numberSrsinSf;

    // Copy header
    memcpy(pDest, pVec, (U32) &pVec->ulCh[0] - (U32) pVec);
    len += (U32) &pVec->ulCh[0] - (U32) pVec;
    // Copy ulCh
    memcpy(pDest + len, pVec->ulCh, num_data_chan * sizeof(pVec->ulCh[0]));
    len += num_data_chan * sizeof(pVec->ulCh[0]);
    // Copy ulCtlCh
    memcpy(pDest + len, pVec->ulCtlCh, num_ctrl_chan * sizeof(pVec->ulCtlCh[0]));
    len += num_ctrl_chan * sizeof(pVec->ulCtlCh[0]);
    // Copy srsInfo
    memcpy(pDest + len, pVec->srsInfo, num_srs_chan * sizeof(pVec->srsInfo[0]));
    len += num_srs_chan * sizeof(pVec->srsInfo[0]);
    // Copy rachCtrl
    memcpy(pDest + len, &pVec->rachCtrl, sizeof(pVec->rachCtrl));
    len += sizeof(pVec->rachCtrl);

    // Overwrite offsets
    {
        PULSUBFRDESC pDestVect = (PULSUBFRDESC) pDest;

    pDestVect->offsetULCtrlChannels = 
        (U32) &pDestVect->ulCh[0] + num_data_chan * sizeof(pVec->ulCh[0]) - (U32) pDestVect;

    pDestVect->offsetsrsInfo = 
        pDestVect->offsetULCtrlChannels + num_ctrl_chan * sizeof(pVec->ulCtlCh[0]);

    pDestVect->offsetRachCtrlStruct = 
        pDestVect->offsetsrsInfo + num_srs_chan * sizeof(pVec->srsInfo[0]);
    }

    return len;
}



/* Function addes PDSCH channel to Subframe with Map According to configuration
 */
static RESULTCODE LtePhyAddPdsch(PDLCHANDESC pCh,
                                 U16 id,
                                 MAPPINGINFO *pMapInfo,
                                 U16 rnti,
                                 S16 power)

{
    U32 i;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    /* Fill Chann desc */
    pCh->offsetNextCh = NOT_SPECIFIED_VALUE;

    pCh->channelId = id;
    pCh->txpowerControl = power;
    pCh->persistEnable = 0x01;
    pCh->repeatCycle =  0x01;
    pCh->channelType = PDSCH;
    pCh->hCid = id;
    pCh->numCodeWords = etm_test_config.nCw;
    pCh->nLayers = etm_test_config.nLayers;
    pCh->transmissionMode = etm_test_config.transmissionMode;
    pCh->crcInfo.crcLength = TWTYFOUR;
    pCh->crcInfo.crcScrambling = 0;
    pCh->scrInfo.scramblerType = DLSCR;
    pCh->scrInfo.scrinitValueinput =  rnti;
    pCh->dlPrecoderInfo.cddType = etm_test_config.cddType;
    pCh->dlPrecoderInfo.codeBookIdx = etm_test_config.codeBookIdx;
    pCh->dlPrecoderInfo.reserved = NOT_SPECIFIED_VALUE;

    pCh->ueCategory = 1;

//#ifdef MX_ARM_ENABLED
    for (i = 0; i < sizeof(pCh->dlPrecoderInfo.codebookSubsetRestriction) /
        sizeof(pCh->dlPrecoderInfo.codebookSubsetRestriction[0]); i++)
//#else
//    for (i = 0; i < 8; i++)
//#endif
    {
        pCh->dlPrecoderInfo.codebookSubsetRestriction[i] = 0;
    }

    memcpy(&pCh->mapInfo, pMapInfo, sizeof(MAPPINGINFO));

    return SUCCESS;
}

static RESULTCODE LtePhyFillPdschSubDesc(
                                 PDLSUBCHINFO pSubCh,
                                 U8 modulation,
                                 U8 mcsType,
                                 U32 cwID,
                                 U16 fn)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    pSubCh->codingDescriptor = TURBOCDR;
    pSubCh->blockCodeConcatenation = 1;
    pSubCh->modulationType = modulation;

    pSubCh->mcsType = mcsType;

    pSubCh->reserved = NOT_SPECIFIED_VALUE;
    pSubCh->nDi =  1;
    pSubCh->rV = etm_test_config.arRctRvDl[cwID][fn];
    pSubCh->flushReq = 0;

    pSubCh->pad = 0;

    return SUCCESS;
}

static RESULTCODE LtePhyFillPbchSubDesc(PDLSUBCHINFO pSubCh, U32 chId)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    pSubCh->codingDescriptor = TBVITCC;
    pSubCh->blockCodeConcatenation = 0;
    pSubCh->modulationType = QPSK;
    pSubCh->mcsType = 4;


    pSubCh->reserved = NOT_SPECIFIED_VALUE;
    pSubCh->nDi =  0;
    pSubCh->rV = 0;
    pSubCh->flushReq = 0;

    return SUCCESS;
}

/* Function addes PDCCH channel to TXVECTOR with DCI format
 */
static RESULTCODE LtePhyAddPbch(PDLCHANDESC pCh, U16 id, U16 fn, U8 sn)
{
    U32 i;

    PRINT_FUNCTION_NAME(__FUNCTION__);
    /* Fill Chann desc */
    pCh->offsetNextCh = NOT_SPECIFIED_VALUE;

    pCh->channelId = id;

    pCh->txpowerControl = etm_test_config.pbchEpre;

    pCh->persistEnable = 1;
    pCh->repeatCycle =  1;

    pCh->channelType = PBCH;
    pCh->hCid = 0;
    pCh->numCodeWords = 1;
    pCh->nLayers = etm_test_config.nLayers;

    pCh->transmissionMode = (pCh->nLayers > 1)? TXDIVERSITY : SINGLEANT;

    LtePhyFillPbchSubDesc(&pCh->subChInfo[0], id);

    pCh->crcInfo.crcLength = SIXTEEN;
    pCh->crcInfo.crcScrambling = 1;
    pCh->scrInfo.scramblerType = DLSCR;
    
    pCh->dlPrecoderInfo.cddType = 0;
    pCh->dlPrecoderInfo.codeBookIdx = 0;

    pCh->dlPrecoderInfo.reserved = NOT_SPECIFIED_VALUE;

    for (i = 0; i < 8; i++)
    {
        pCh->dlPrecoderInfo.codebookSubsetRestriction[i] = 0;
    }

    memset(&pCh->mapInfo, 0, sizeof(MAPPINGINFO));

    /* Generate PHY TXSDU.req for this channel */
//#ifdef MX_ARM_ENABLED
    LtePhyPrepareTestZeroSduPbch(PBCH_SIZE_ONE_SUBFRAME_BITS, id, PBCH, 
        etm_test_config.payloadPbch, fn, sn);
//#else
//    LtePhyPreparePbchSdu(PBCH_SIZE_ONE_SUBFRAME_BITS, id, PBCH,
//                                 &pTestConfConfig->payloadPbch[0]);
//#endif

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
static RESULTCODE LtePhyPrepareTestPdcchSdu(U32 numBits, U16 chId, U8* pDci, U16 fn, U8 sn)
{
    PTXSDUREQ pSduReq = (PTXSDUREQ) Mac2PhyIfAllocBuffer();

    if (pSduReq == NULL)
    {
        printf("Failed to allocate memory, LtePhyPrepareTestPdcchSdu function\r\n");
        return FAILURE;
    }

    memcpy((U8*) ((U8*) pSduReq + sizeof(TXSDUREQ)), pDci, (numBits + 7) >> 3);

    pSduReq->phyEntityId = 0;
    pSduReq->msgType = PHY_TXSDU_REQ;
    pSduReq->chanId = chId;
    pSduReq->cwId = 0;
    pSduReq->channelType = PDCCH;
    pSduReq->phichGrpNumber = 0;
    pSduReq->maxBitsperCw = numBits;
    pSduReq->msgLen = (numBits + 7) >> 3;
    pSduReq->pTxSdu = 0;
    pSduReq->uciFormat = 0;

    return Mac2PhyQueue.pAddToTail(PHY_TXSDU_REQ | (PDCCH << 4), pSduReq->msgLen
            + sizeof(TXSDUREQ), (U8*) pSduReq, fn, sn);

}

/** Inserts PDSCH channel containing all zeroes into scheduler queue
 *
 * @param numBits Number of bits in SDU payload
 * @param chId PHY channel ID
 * @param channelType (PDSCH, see enum ChTypeOptions)
 * @param cwId
 * @return Standard result code
 */
static RESULTCODE LtePhyPrepareTestZeroSdu(U32 numBits, U8 chId, U8 channelType, U32 cwId, U16 fn, U8 sn)
{
    PTXSDUREQ pSduReq = (PTXSDUREQ) Mac2PhyIfAllocBuffer();

    if (pSduReq == NULL)
    {
        printf("Failed to allocate PTXSDUREQ, LtePhyPrepareTestZeroSdu function\r\n");
        return FAILURE;
    }

    pSduReq->phyEntityId = 0;
    pSduReq->msgType = PHY_TXSDU_REQ;
    pSduReq->chanId = chId;
    pSduReq->cwId = cwId;
    pSduReq->channelType = channelType;
    pSduReq->phichGrpNumber = 0;
    pSduReq->maxBitsperCw = numBits;
    pSduReq->msgLen = numBits >> 3;
    pSduReq->pTxSdu = 0;

    memset((U8*) ((U8*) pSduReq + sizeof(TXSDUREQ)), 0, numBits >> 3);

    Mac2PhyQueue.pAddToTail(PHY_TXSDU_REQ | (channelType << 4), pSduReq->msgLen
            + sizeof(TXSDUREQ), (U8*) pSduReq, fn, sn);
    return SUCCESS;
}

/****************************************************************************
 * Privat Functions (Midle level)
 ****************************************************************************/

static RESULTCODE TestMgrGetBoostedPdsch(MAPPINGINFO *pMapInfo, U16 ChanId, U8 LayerIdx, U8 sn)
{
    U8 **pBoostingMap;
    const U8 *pMap;
    U32 cur_num;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    memset(pMapInfo, 0, sizeof(MAPPINGINFO));

    if(etm_test_config.nPdschPrb[LayerIdx][ChanId] == 0)
        return FAILURE;

    if(etm_test_mgr_ctx.boostingMap[LayerIdx][ChanId] == NULL)
    {
        printf("ERROR: boostingMap has bad ptr [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }

    pBoostingMap = etm_test_mgr_ctx.boostingMap[LayerIdx][ChanId];
    pMap = pBoostingMap[sn];
    cur_num = etm_test_config.nPdschPrb[LayerIdx][ChanId];

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

static RESULTCODE TestMgrGetDeBoostedPdsch(MAPPINGINFO *pMapInfo, U16 Pdsch_idx, U8 LayerIdx, U8 sn)
{
    MAPPINGINFO mapBoosted;
    U32 i = 0;
    U8 boostedStarRb;
    U8 boostedNumRb;
    U8 nextStarRb;
    U8 subFramNum;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    /* check if we've got  deBoosting in this model */
    if (etm_test_mgr_ctx.boostingMap[0][1] == NULL)
        return FAILURE;

    subFramNum = sn;

    if(*(etm_test_mgr_ctx.boostingMap[0][1])[subFramNum] == 0xFF)
        return FAILURE;

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
                    (etm_test_config.nPdschPrb[LayerIdx][Pdsch_idx] +
                etm_test_config.nPdschPrb[LayerIdx][Pdsch_idx - 1]) - (boostedStarRb + boostedNumRb);
            }

            if (pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes)
                pMapInfo->numberofEntries++;

            i++;
        }
    }
    else if (mapBoosted.numberofEntries == 0)
    {// fill all the map by DeBoosted regardless of etm_test_config.nPdschDeBoostedPrb
        U8 *pMap = (etm_test_mgr_ctx.boostingMap[0][1])[subFramNum];
        U32 cur_num = 1000;

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
            etm_test_config.nPdschPrb[LayerIdx][Pdsch_idx] - boostedStarRb;

        if (pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes)
            pMapInfo->numberofEntries++;
    }

    if(pMapInfo->numberofEntries == 0)
        return FAILURE;

    return SUCCESS;
}



static U8 GetMcsIndexDl(U16 modulation, U16 cwId)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    if(modulation>=QPSK && modulation<=QAM64)
        return etm_mcsi_index_table_dl[modulation][cwId];

    printf("Error in funtion %s (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    return 0xFF;
}

static RESULTCODE LtePhyFillPdcchMapping(    PDCICHANNELDESC pCh,
                                            U32 chId,
                                            MAPPINGINFO *pMapInfo)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    pCh->numberofEntries = pMapInfo->numberofEntries;
    pCh->startRes = pMapInfo->reselmInfo[0].startRes;
    pCh->numRes = pMapInfo->reselmInfo[0].numRes;

    return SUCCESS;
}

static RESULTCODE TestMgrGetPdcch(U8 pdcchIdx,
                                  MAPPINGINFO *pMapInfo )
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    memset(pMapInfo, 0, sizeof(MAPPINGINFO));

    pMapInfo->reselmInfo[pMapInfo->numberofEntries].startRes =
                                    pdcchIdx * etm_test_config.nCcePerPdcch;
    pMapInfo->reselmInfo[pMapInfo->numberofEntries].numRes =
                                    etm_test_config.nCcePerPdcch;
    pMapInfo->numberofEntries++;

    return SUCCESS;
}

/* Function addes PDCCH channel to TXVECTOR with DCI format
 */
static RESULTCODE LtePhyAddPdcch(PDCICHANNELDESC pCh,
                                 U16 id,
                                 MAPPINGINFO *pMapInfo,
                                 U32 dci_idx,
                                 U16 fn,
                                 U8 sn)
{

//    DCI_DATA_INFO DciInfo;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    /* Fill Chann desc */
    pCh->offsetNextCh = NOT_SPECIFIED_VALUE;

    pCh->channelId = id;
    pCh->txpowerControl = etm_test_config.pdcchRegEpre;

    pCh->crcLength = SIXTEEN;
    pCh->crcScrambling = 1;

    pCh->channelType = PDCCH;

    pCh->reserved = NOT_SPECIFIED_VALUE;

    pCh->numCodeWords = 1;
    pCh->nLayers = etm_test_config.nLayers;

    pCh->transmissionMode = (pCh->nLayers > 1)? TXDIVERSITY : SINGLEANT;

    pCh->reserved1 = NOT_SPECIFIED_VALUE;
    pCh->scrmblerInitValue =  (etm_test_config.TestModel == E_TM_INT_3_2 ||  etm_test_config.TestModel == E_TM_INT_3_3 ) ?  
        etm_test_config.pdschRnti[1] :  etm_test_config.pdschRnti[0];

    /* Fill Subchan Desc */
    LtePhyFillPdcchMapping(pCh, id, pMapInfo);
    LtePhyPrepareTestPdcchSdu(etm_test_config.PdcchDataLen[dci_idx], 
                             id,
                             &etm_test_config.payloadPdcch[dci_idx][0],
                             fn, sn);

    return SUCCESS;
}

/* Function addes PDCCH channel to TXVECTOR with DCI format
 */
static RESULTCODE LtePhyAddPhich(PDCICHANNELDESC pCh,
                                 U16 id,
                                 U8 groupId,
                                 U8 seqIdx)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    /* Fill Chann desc */
    pCh->offsetNextCh = NOT_SPECIFIED_VALUE;

    pCh->channelId = id;
    pCh->txpowerControl = etm_test_config.phichSymPower;

    pCh->crcLength = SIXTEEN;
    pCh->crcScrambling = 1;

    pCh->channelType = PHICH;

    pCh->reserved = NOT_SPECIFIED_VALUE;

    pCh->numCodeWords = 1;
    pCh->nLayers = etm_test_config.nLayers;

    pCh->transmissionMode = (etm_test_config.transmissionMode == SINGLEANT)? SINGLEANT : TXDIVERSITY;

    pCh->reserved1 = NOT_SPECIFIED_VALUE;

    /* No mapping */
    pCh->numberofEntries = 0;
    pCh->startRes = 0;
    pCh->numRes = 0;

    return SUCCESS;
}

static U32 HiDescSet(PDLSUBFRDESC    pDlSubFrame, PULSUBFRDESC pUlSubFrame, PRXSDUIND pRxSdu, U16 fn, U8    sn)
{
    PHIINFOMSGDESC HiDesc;
    U8 Idx = 0, i;
    U32 HiNum = 0;

    if(etm_test_config.phichIdx != 0xFF)
    {
        for (Idx = 0; Idx < etm_test_config.nPhichGroup; Idx++)
        {
// TODO:
#ifdef WIN32
            HiDesc = (PHIINFOMSGDESC) malloc (BLOCK_SIZE);
#else
            #ifdef MX_ARM_ENABLED
            HiDesc = (PHIINFOMSGDESC)SvsrAllocMsg();
            #else
            HiDesc = NULL;
            #endif
#endif
            if(HiDesc == NULL)
            {
                printf("HiDesc allocation failure\n");
                return 0;
            }
            memset(HiDesc, 0, sizeof(*HiDesc));
            HiDesc->msgType = PHY_TXHISDU_REQ;
            HiDesc->phyEntityId = 0;

            if(pUlSubFrame && pUlSubFrame->ulCh[pRxSdu->channelId].ulSubChInfo.harqInfo.nDi != 1)
                HiDesc->nackAck = 1;
            else
                HiDesc->nackAck = 0;
// Two antenna support
//            if (pBsMacDesc->initParm.txAntennaPortCount <= TXANT_TWO)
//            {
//                HiDesc->transmissionMode = pBsMacDesc->initParm.txAntennaPortCount;
//            }
//            else
//            {
//                HiDesc->transmissionMode = TXDIVERSITY;
//            } 



            HiDesc->transmissionMode = etm_test_config.transmissionMode;
            HiDesc->phichSequenceIndex = etm_test_config.phichIdx;
            HiDesc->phichGroupNumber = Idx;
            HiDesc->channelId = global_chanIdx ++;
            HiDesc->txPowerControl = etm_test_config.phichSymPower;
            
            Mac2PhyQueue.pAddToTail(PHY_TXHISDU_REQ, sizeof(HIINFOMSGDESC), (U8*)HiDesc, fn, sn);
            HiNum++;
        }
   }
   else
   {

    for (Idx = 0;
        Idx < etm_test_config.nPhichGroup;
        Idx++)
    {
        for (i = 0; i < etm_test_config.nPhichPerGroup; i++)
        {
// TODO:
#ifdef WIN32
            HiDesc = (PHIINFOMSGDESC)malloc(BLOCK_SIZE);
#else
            #ifdef MX_ARM_ENABLED
            HiDesc = (PHIINFOMSGDESC)SvsrAllocMsg();
            #else
            HiDesc = NULL;
            #endif
#endif
            if(HiDesc == NULL)
            {
                printf("HiDesc allocation failure\n");
                return 0;
            }
            memset(HiDesc, 0, sizeof(*HiDesc));

            HiDesc->msgType = PHY_TXHISDU_REQ;
            HiDesc->phyEntityId = 0;

            if(pUlSubFrame && pUlSubFrame->ulCh[pRxSdu->channelId].ulSubChInfo.harqInfo.nDi != 1)
                HiDesc->nackAck = 1;
            else
                HiDesc->nackAck = 0;

            HiDesc->transmissionMode = etm_test_config.transmissionMode;
            HiDesc->phichSequenceIndex = ( etm_test_config.nPhichPerGroup < 3 )? i * 4 : i;
            HiDesc->phichGroupNumber = Idx;
            HiDesc->channelId = global_chanIdx ++;
            HiDesc->txPowerControl = etm_test_config.phichSymPower;

            Mac2PhyQueue.pAddToTail(PHY_TXHISDU_REQ, sizeof(HIINFOMSGDESC), (U8*)HiDesc, fn, sn);
            HiNum++;
        }
    }
    }
    return HiNum;
}

/****************************************************************************
 * Privat Functions (Up level)
 ****************************************************************************/

/** Add API message to the head of a MAC-PHY message queue
 *
 * @param MessageType PHY API message ID (LtePhyL2Api.h)
 * @param MessageLen Message size
 * @param MessagePtr Pointer to the message buffer
 * @return Standard result code
 */
static RESULTCODE AddToHead(U8 MessageType, U32 MessageLen, U8* MessagePtr, U16 fn, U8 sn)
{

#ifdef WIN32
    PMAC2PHY_QUEUE_EL tempo = (PMAC2PHY_QUEUE_EL) malloc (sizeof(MAC2PHY_QUEUE_EL));
#else
    #ifdef MX_ARM_ENABLED
    PMAC2PHY_QUEUE_EL tempo = (PMAC2PHY_QUEUE_EL) SvsrAllocMsgEx(sizeof(MAC2PHY_QUEUE_EL));
    #else
    PMAC2PHY_QUEUE_EL tempo = NULL;
    #endif
#endif

    if (tempo == NULL)
    {
        printf("TestMAC, cannot allocate I-CORE memory (AddToHead function)\r\n");
        return FAILURE;
    }

    tempo->frameNumber = fn;
    tempo->subframeNumber = sn;
    tempo->MessageType = MessageType;
    tempo->MessageLen = MessageLen;
    tempo->MessagePtr = MessagePtr;
    tempo->Next = NULL;
    if (Mac2PhyQueue.StartQueueCurr == NULL)
    {
        Mac2PhyQueue.StartQueueCurr = tempo;
        Mac2PhyQueue.TailQueueCurr = tempo;
    }
    else
    {
        tempo->Next = Mac2PhyQueue.StartQueueCurr;
        Mac2PhyQueue.StartQueueCurr = tempo;
    }
    return SUCCESS;
}

/** Add API message to the tail of a MAC-PHY message queue
 *
 * @param MessageType PHY API message ID (LtePhyL2Api.h)
 * @param MessageLen Message size
 * @param MessagePtr Pointer to the message buffer
 * @return Standard result code
 */
static RESULTCODE AddToTail(U8 MessageType, U32 MessageLen, U8* MessagePtr, U16 fn, U8 sn)
{
#ifdef WIN32
    PMAC2PHY_QUEUE_EL tempo = (PMAC2PHY_QUEUE_EL) malloc (sizeof(MAC2PHY_QUEUE_EL));
#else
    #ifdef MX_ARM_ENABLED
    PMAC2PHY_QUEUE_EL tempo = (PMAC2PHY_QUEUE_EL) SvsrAllocMsgEx(sizeof(MAC2PHY_QUEUE_EL));
    #else
    PMAC2PHY_QUEUE_EL tempo = NULL;
    #endif
#endif

    if (tempo == NULL)
    {
        printf("TestMAC, cannot allocate I-CORE memory (AddToTail function)\r\n");
        return FAILURE;
    }

    tempo->frameNumber = fn;
    tempo->subframeNumber = sn;
    tempo->MessageType = MessageType;
    tempo->MessageLen = MessageLen;
    tempo->MessagePtr = MessagePtr;
    tempo->Next = NULL;
    if (Mac2PhyQueue.StartQueueCurr == NULL)
    {
        Mac2PhyQueue.StartQueueCurr = tempo;
        Mac2PhyQueue.TailQueueCurr = tempo;
    }
    else
    {
        Mac2PhyQueue.TailQueueCurr->Next = tempo;
        Mac2PhyQueue.TailQueueCurr = tempo;
    }

    return SUCCESS;
}

static RESULTCODE LtePhyFillTestTxVectorCommon(unsigned int fn, unsigned int sn)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);
    memset(pSubFrame, 0, sizeof(DLSUBFRDESC));

    pSubFrame->frameNumber = fn;
    pSubFrame->subframeNumber = sn;
    pSubFrame->subframeType = DLTX;
    pSubFrame->antennaPortcount = etm_test_config.nAntennaPort;
    pSubFrame->numberofChannelDescriptors = 0x00;
    pSubFrame->reserved0 = NOT_SPECIFIED_VALUE;
    pSubFrame->offsetCustomFeatures = NOT_SPECIFIED_VALUE;
    pSubFrame->numCtrlSymbols = etm_test_config.nCtrlSymbols;
    pSubFrame->phichResource = etm_test_config.phichResource;       // 0 - Ng =1/6
    pSubFrame->phichDuration = etm_test_config.phichDuration;
    pSubFrame->numberOfPhichChannels = (U16)(etm_test_config.nPhichGroup * etm_test_config.nPhichPerGroup);
    pSubFrame->numCtrlChannels = 0;
    pSubFrame->dlCmnPwrCtl.pilotPower = 0;
    pSubFrame->dlCmnPwrCtl.psyncPower = etm_test_config.epreSinchro;
    pSubFrame->dlCmnPwrCtl.ssyncPower = etm_test_config.epreSinchro;
    /* 0x8000; */
    pSubFrame->dlCmnPwrCtl.ciPower = etm_test_config.pcfichEpre;

    return SUCCESS;
}

static RESULTCODE LtePhyUpdatePDSCH(    PDLSUBFRDESC pDlSubFrame,
                                        PMAPPINGINFO pmapInfo,
                                        U8 *chanIdx,
                                        U8 Pdsch_idx,
                                        U8 boosted,
                                        U16 fn,
                                        U8 sn)
{
    U8            i;
    PDLCHANDESC pdlCh;
    U8          Modulation;
    U8          mcsType;
    U16         rnti;
    RESULTCODE  res;
    U8          chIdx = *chanIdx;
    S16         Power = 0;
    
     res = boosted ?
         TestMgrGetBoostedPdsch(pmapInfo, Pdsch_idx, 0, sn ) :    /* for 1 layer only*/
         TestMgrGetDeBoostedPdsch(pmapInfo, Pdsch_idx, 0, sn );    /* for 1 layer only*/
 
    if ( res != SUCCESS )
        return res;
    
    pdlCh = &pDlSubFrame->dlCh[chIdx - pDlSubFrame->numCtrlChannels];

    rnti = etm_test_config.pdschRnti[Pdsch_idx];
    
    Power = boosted ? 
        etm_test_config.pdschPrbPa[0][0]: /* for 1 layer only*/
        etm_test_config.pdschPrbPa[0][1]; /* for 1 layer only*/
 
    LtePhyAddPdsch( pdlCh, chIdx, pmapInfo, rnti, Power);

    // Fill Subchan Desc
    for(i = 0; i < etm_test_config.nCw; i++)
    {
        if(etm_test_config.pdschMcs_used[i])
        {
            mcsType = etm_test_config.pdschMcs[i][Pdsch_idx];

            Modulation = boosted ? 
                etm_test_config.pdschMod[0][0]:
                etm_test_config.pdschMod[0][1];
        }
        else
        {
            //U8 layer_idx = boosted ? i : 0;
            Modulation = etm_test_config.pdschMod[0][Pdsch_idx];
            mcsType = GetMcsIndexDl(Modulation, i);
        }

        LtePhyFillPdschSubDesc(&pdlCh->subChInfo[i], Modulation, mcsType, i, fn);
        
         /* Generate PHY TXSDU.req for this channel */
        LtePhyPrepareTestZeroSdu(etm_test_config.pdschDataLen[Pdsch_idx], chIdx, PDSCH, i, fn, sn);
        
    }
    chIdx++;
    *chanIdx = chIdx;

    return SUCCESS;
}

static RESULTCODE LtePhyUpdateTestTxVector(U8 channelType, U16 fn, U8 sn)
{
    U8 chanIdx = pSubFrame->numberofChannelDescriptors;
    U8 Idx = 0;
    MAPPINGINFO mapInfo;
    U8 Pdsch_idx = 0;

    PRINT_FUNCTION_NAME(__FUNCTION__);

    switch(channelType)
    {
        case PHICH:
            for (Idx = 0; Idx < etm_test_config.nPhichGroup; Idx++)
            {
                U32 i;
                for (i = 0; i < etm_test_config.nPhichPerGroup; i++)
                {
                    LtePhyAddPhich(&pSubFrame->dciCh[chanIdx],
                                    chanIdx,
                                    Idx,  /* group ID */

                                    etm_test_config.phichIdx); /* sequence Index */

                    pSubFrame->numCtrlChannels++;
                    chanIdx++;
                }
            }
            break;

        case PDCCH:
            for (Idx = 0; Idx < etm_test_config.nPdcch; Idx++)
            {
                TestMgrGetPdcch(Idx, &mapInfo);
                LtePhyAddPdcch(&pSubFrame->dciCh[chanIdx], chanIdx, &mapInfo, Idx, fn, sn);
                pSubFrame->numCtrlChannels++;
                chanIdx++;
            }
            break;
            
        case PDSCH:
            {
            Pdsch_idx = 0;
                /* PDSCH channels with boosted PRBs */
                if ( SUCCESS == LtePhyUpdatePDSCH(pSubFrame, &mapInfo, &chanIdx, Pdsch_idx, 1, fn, sn))
                    Pdsch_idx++;

                while(Pdsch_idx && (Pdsch_idx < etm_test_config.nPdsch[0]))
                {
                    /* check if we've got  deBoosting in this model */
                    if (etm_test_mgr_ctx.boostingMap[0][Pdsch_idx] == NULL)
                       return FAILURE;

                    LtePhyUpdatePDSCH(pSubFrame, &mapInfo, &chanIdx, Pdsch_idx, 0, fn, sn);
                    Pdsch_idx++;
                }
            }
            break;
            
        case PBCH:               
            LtePhyAddPbch(&pSubFrame->dlCh[chanIdx - pSubFrame->numCtrlChannels], chanIdx, fn, sn);
            
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

static RESULTCODE LtePhyFillTestRxVectorCommon(U16 fn, U8 sn)
{
    PRINT_FUNCTION_NAME(__FUNCTION__);

    memset(pUlSubFrame, 0, sizeof(ULSUBFRDESC));

    pUlSubFrame->frameNumber = fn;
    pUlSubFrame->subframeNumber = sn;
    pUlSubFrame->subframeType = ULRX;
    pUlSubFrame->antennaPortcount = e_tm_static_config.n_antenna_port;
    pUlSubFrame->numberofChannelDescriptors = 0x00;

    pUlSubFrame->numberOfCtrlChannelDescriptors = 0x00;

    // offsets to be updated later after channels adding
    pUlSubFrame->offsetCustomFeatures = NOT_SPECIFIED_VALUE;

    pUlSubFrame->numberSrsinSf = NOT_SPECIFIED_VALUE;

    /* disable RACH channel */
    pUlSubFrame->rachCtrl.prachEnable = PRACH_OFF;

    return SUCCESS;
}

/** Creates and sends HI DCI payload
 *
 * @param pDlSubFramePointer to the current Downlink vector
 * @param pUlSubFramePointer to the current Uplink vector
 * @param pRxSdu Pointer to the HI DCI SDU payload
 * @return Standard result code
 */
static RESULTCODE LtePhyHiDciSet(PDLSUBFRDESC pDlSubFrame, PULSUBFRDESC pUlSubFrame, PRXSDUIND pRxSdu, U16 fn, U8 sn)
{
    PHIADCIULMSGDESC hiDciDesc = NULL;
    U32 HiNum = 0;
    U32 Dci0Num = 0;
    
    HiNum = HiDescSet(pDlSubFrame, pUlSubFrame, pRxSdu, fn, sn);

    if (HiNum == 0 && Dci0Num == 0)
    {
        return SUCCESS;
    }

#ifdef WIN32
    hiDciDesc = (PHIADCIULMSGDESC)  malloc (BLOCK_SIZE);
#else
    #ifdef MX_ARM_ENABLED
    hiDciDesc = (PHIADCIULMSGDESC) SvsrAllocMsg();
    #else
    hiDciDesc = NULL;
    #endif
#endif

    if (hiDciDesc == NULL)
    {
        printf("HiDci Allocation failed\n");
        return FAILURE;
    }
    memset(hiDciDesc, 0, sizeof(HIADCIULMSGDESC));

    hiDciDesc->phyEntityId = 0;
    hiDciDesc->msgLength = 0;
    hiDciDesc->msgType = PHY_TXHIADCIUL_REQ;

    hiDciDesc->subFrameNumber = sn;
    hiDciDesc->frameNumber = fn;

    hiDciDesc->numDciUlSdus = Dci0Num;
    hiDciDesc->numHiSdus = HiNum;

    Mac2PhyQueue.pAddToTail(PHY_TXHIADCIUL_REQ,  sizeof(HIADCIULMSGDESC), (U8*) hiDciDesc, fn, sn);

    return SUCCESS;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/*
 * init()   : init E-TM model testing
 * cfg [in] : cfg - configuration for E-TM model
 * return   : 0 - success, >0 - failure
 */
unsigned int etm_init(E_TM_CONFIG cfg)
{
    S32 i_tmp;
    PRINT_FUNCTION_NAME(__FUNCTION__);
    Mac2PhyQueue.pAddToTail = (tAddToTail)AddToTail;

    memset(&etm_test_config, 0, sizeof(etm_test_config));
    memset(&etm_test_mgr_ctx, 0, sizeof(etm_test_mgr_ctx));

    etm_test_config.payloadPbch[0] = 0x60;
    etm_test_config.payloadPbch[1] = 0xC0;
    etm_test_config.payloadPbch[2] = 0x00;   

    e_tm_static_config.tdd = cfg.tdd;
    e_tm_static_config.bw_mode = cfg.bw;

    // TODO: port choice replaced with port count
    if (cfg.ant0 && cfg.ant1)
    {// 2 ports
        e_tm_static_config.n_antenna_port = 2;
        etm_test_config.nAntennaPort = 2;
        // TODO: Init next fields for 2 sntena ports
        //etm_test_config.transmissionMode = ;
        //etm_test_config.cddType = ;
        //etm_test_config.codeBookIdx = ;
    }
    else if (cfg.ant0 || cfg.ant1)
    {// 1 port
        e_tm_static_config.n_antenna_port = 1;
        etm_test_config.nAntennaPort = 1;
        etm_test_config.transmissionMode = 1;
    }
    else 
    {// 0 port
        printf("ERROR: Incorrect Antenas config [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }

    etm_test_config.TestModel = cfg.model;
    etm_test_config.nCw = cfg.num_cw;
    etm_test_config.nLayers = cfg.num_layers;
    //etm_test_config.precodingMode = 0; - already set to zero
    etm_test_config.duration = SF_NUM;
    //etm_test_config.cp = 0; - already set to zero
    //etm_test_config.arRctRvDl[][] = 0; - already set to zero
    etm_test_config.rsBoosting = rs_boosting[cfg.model][cfg.bw];
    etm_test_config.epreSinchro = cfg.e_rs + epre_sinchro[cfg.model][cfg.bw];
    etm_test_config.reservedEpre = cfg.e_rs + reserved_epre[cfg.model][cfg.bw];

    /* PBCH */
    etm_test_config.pbchEna = 1;
    etm_test_config.pbchEpre = cfg.e_rs + pbch_epre[cfg.model][cfg.bw];
    etm_test_config.pbchReservedEpre = cfg.e_rs + pbch_reserved_epre[cfg.model][cfg.bw];
    //etm_test_config.payloadPbch = 0; - already set to zero
    
    /* PCFICH */
    etm_test_config.nCtrlSymbols = n_ctrl_symbols[cfg.model][cfg.bw];
    etm_test_config.pcfichEpre = cfg.e_rs + pcfich_epre[cfg.model][cfg.bw];
    
    /* PHICH */
    etm_test_config.nPhichGroup =  n_phich_group[cfg.model][cfg.bw];
    etm_test_config.nPhichPerGroup = n_phich_per_group[cfg.model][cfg.bw];
    etm_test_config.phichSymPower = cfg.e_rs + phich_sym_power[cfg.model][cfg.bw];
    etm_test_config.phichGroupEpre = cfg.e_rs + phich_group_epre[cfg.model][cfg.bw];

    /* PDCCH */
    etm_test_config.nAvalableRegs = n_avalable_regs[cfg.model][cfg.bw];
    etm_test_config.nPdcch = n_pdcch[cfg.model][cfg.bw];
    etm_test_config.nCcePerPdcch = n_cce_per_pdcch[cfg.model][cfg.bw];
    etm_test_config.nRegPerCce = n_reg_per_cce[cfg.model][cfg.bw];
    etm_test_config.nRegAllToPdcch = n_reg_all_to_pdcch[cfg.model][cfg.bw];
    etm_test_config.nNilRegPadding = n_nil_reg_padding[cfg.model][cfg.bw];
    etm_test_config.pdcchRegEpre = cfg.e_rs + pdcch_reg_epre[cfg.model][cfg.bw];    
    etm_test_config.nilRegEpre = cfg.e_rs + nil_reg_epre[cfg.model][cfg.bw];
    //etm_test_config.pdcch_map_used = 0; - already set to zero

    /* DCI */
    for (i_tmp = 0; i_tmp < etm_test_config.nPdcch; i_tmp++)
        etm_test_config.PdcchDataLen[i_tmp] = 32;
    //etm_test_config.payloadPdcch[][] = 0 - already set to zero
    //etm_test_config.PdcchRnti[] = 0; - already set to zero
    etm_test_config.PdcchRntiEna = 0xFFFF; //or -1

    /* PDSCH */
    etm_test_config.pdschMcs_used[0] = 1;
    etm_test_config.pdschMcs_used[1] = 0;
    etm_test_config.pdschRnti_used = 1;
    etm_test_config.pdschPayload = 0;
    etm_test_config.pdschPayloadEna = 1;

    /* Boosting map */
    switch(cfg.model)
    {
        case E_TM_INT_1_1:
            etm_test_config.nPdsch[0] = 1; 
            etm_test_config.nPdschPrb[0][0] = n_pdsch_boosted_prb[E_TM_INT_1_1][cfg.bw];
            etm_test_config.pdschPrbPa[0][0] = cfg.e_rs + pdsch_boosted_prb_pa[E_TM_INT_1_1][cfg.bw];
            etm_test_config.pdschMod[0][0] = QPSK;
            etm_test_config.pdschMcs[0][0] = 9; /* QPSK */
            etm_test_config.pdschRnti[0] = 0;
            etm_test_config.pdschDataLen[0] = 999*8; //7992bit
            etm_test_mgr_ctx.boostingMap[0][0] = (U8**)E_TM_1_1MapIdx[cfg.bw];
            break;
        case E_TM_INT_1_2:
            etm_test_config.nPdsch[0] = 2;
            etm_test_config.nPdschPrb[0][0] = n_pdsch_boosted_prb[E_TM_INT_1_2][cfg.bw];
            etm_test_config.nPdschPrb[0][1] = n_pdsch_de_boosted_prb[E_TM_INT_1_2][cfg.bw];
            etm_test_config.pdschPrbPa[0][0] = cfg.e_rs + pdsch_boosted_prb_pa[E_TM_INT_1_2][cfg.bw];
            etm_test_config.pdschPrbPa[0][1] = cfg.e_rs + pdsch_de_boosted_prb_pa[E_TM_INT_1_2][cfg.bw];
            etm_test_config.pdschMod[0][0] = QPSK;
            etm_test_config.pdschMod[0][1] = QPSK;
            etm_test_config.pdschMcs[0][0] = 9; /* QPSK */
            etm_test_config.pdschMcs[0][1] = 9; /* QPSK */
            etm_test_config.pdschRnti[0] = 0;
            etm_test_config.pdschRnti[1] = 1;
            etm_test_config.pdschDataLen[0] = 389; //3112bit
            etm_test_config.pdschDataLen[1] = 597; //4776bit
            etm_test_mgr_ctx.boostingMap[0][0] = (U8**)E_TM_1_2MapIdx[cfg.bw];
            etm_test_mgr_ctx.boostingMap[0][1] = (U8**)E_TM_1_2MapIdxDeBoosted[cfg.bw];
            break;
        case E_TM_INT_2:
            etm_test_config.nPdsch[0] = 1; 
            etm_test_config.nPdschPrb[0][0] = n_pdsch_boosted_prb[E_TM_INT_2][cfg.bw];
            etm_test_config.pdschPrbPa[0][0] = cfg.e_rs + pdsch_boosted_prb_pa[E_TM_INT_2][cfg.bw];
            etm_test_config.pdschMod[0][0] = QAM64;
            etm_test_config.pdschMcs[0][0] = 24; /* 64QAM */
            etm_test_config.pdschRnti[0] = 0;
            etm_test_config.pdschDataLen[0] = 520; //47; //376bit
            etm_test_mgr_ctx.boostingMap[0][0] = (U8**)E_TM_2MapIdx[cfg.bw];
            break;
        case E_TM_INT_3_1:
            etm_test_config.nPdsch[0] = 1;
            etm_test_config.nPdschPrb[0][0] = n_pdsch_boosted_prb[E_TM_INT_3_1][cfg.bw];
            etm_test_config.pdschPrbPa[0][0] = cfg.e_rs + pdsch_boosted_prb_pa[E_TM_INT_3_1][cfg.bw];
            etm_test_config.pdschMod[0][0] = QAM64;
            etm_test_config.pdschMcs[0][0] = 24; /* 64QAM */
            etm_test_config.pdschRnti[0] = 0;
            etm_test_config.pdschDataLen[0] = 2481; //19848bit
            etm_test_mgr_ctx.boostingMap[0][0] = (U8**)E_TM_3_1MapIdx[cfg.bw];
            break;
        case E_TM_INT_3_2:
            etm_test_config.nPdsch[0] = 2; 
            etm_test_config.nPdschPrb[0][0] = n_pdsch_boosted_prb[E_TM_INT_3_2][cfg.bw];
            etm_test_config.nPdschPrb[0][1] = n_pdsch_de_boosted_prb[E_TM_INT_3_2][cfg.bw];
            etm_test_config.pdschPrbPa[0][0] = cfg.e_rs + pdsch_boosted_prb_pa[E_TM_INT_3_2][cfg.bw];
            etm_test_config.pdschPrbPa[0][1] = cfg.e_rs + pdsch_de_boosted_prb_pa[E_TM_INT_3_2][cfg.bw];
            etm_test_config.pdschMod[0][0] = QAM16;
            etm_test_config.pdschMod[0][1] = QPSK;
            etm_test_config.pdschMcs[0][0] = 16; /* 16QAM */
            etm_test_config.pdschMcs[0][1] = 9;  /* QPSK */
            etm_test_config.pdschRnti[0] = 1;
            etm_test_config.pdschRnti[1] = 0;
            etm_test_config.pdschDataLen[0] = 1143; //9144bit
            etm_test_config.pdschDataLen[1] = 389; //3112bit
            etm_test_mgr_ctx.boostingMap[0][0] = (U8**)E_TM_3_2MapIdx[cfg.bw];
            etm_test_mgr_ctx.boostingMap[0][1] = (U8**)E_TM_3_2MapIdx[cfg.bw];
            break;
        case E_TM_INT_3_3:
            etm_test_config.nPdsch[0] = 2; 
            etm_test_config.nPdschPrb[0][0] = n_pdsch_boosted_prb[E_TM_INT_3_3][cfg.bw];
            etm_test_config.nPdschPrb[0][1] = n_pdsch_de_boosted_prb[E_TM_INT_3_3][cfg.bw];
            etm_test_config.pdschPrbPa[0][0] = cfg.e_rs + pdsch_boosted_prb_pa[E_TM_INT_3_3][cfg.bw];
            etm_test_config.pdschPrbPa[0][1] = cfg.e_rs + pdsch_de_boosted_prb_pa[E_TM_INT_3_3][cfg.bw];
            etm_test_config.pdschMod[0][0] = QPSK;
            etm_test_config.pdschMod[0][1] = QAM16;
            etm_test_config.pdschMcs[0][0] = 9;  /* QPSK */
            etm_test_config.pdschMcs[0][1] = 16; /* 16QAM */
            etm_test_config.pdschRnti[0] = 1;
            etm_test_config.pdschRnti[1] = 0;
            etm_test_config.pdschDataLen[0] = 389; //3112bit
            etm_test_config.pdschDataLen[1] = 1143; //9144bit
            etm_test_mgr_ctx.boostingMap[0][0] = (U8**)E_TM_3_3MapIdx[cfg.bw];
            etm_test_mgr_ctx.boostingMap[0][1] = (U8**)E_TM_3_3MapIdx[cfg.bw];
            break;
        case E_TM_INT_UNDEFINED:
        default:
            printf("ERROR: Incorrect config Id [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
            return FAILURE;
    }

    /* PHICH */
    etm_test_config.phichDuration = 0; //from Ref Apilog
    etm_test_config.phichIdx = 0xFF;

    return SUCCESS;
}

/*
 * get_vectors()     : get list of vectors to be sent to PHY
 * phy_vectors [out] : address of pointer to all PHY vectors
 * return             : 0 - success, >0 - failure
*/
unsigned int etm_get_vectors(MAC2PHY_QUEUE_EL **phy_vectors, unsigned int fn, unsigned int sn)
{
    PGENMSGDESC buffer = NULL;
    PGENMSGDESC buffer_ul = NULL;
    PGENMSGDESC pToPhyMsg = NULL;
    U32 msgSize = 0;
    PTR payload = NULL;
    U32 len;

    /* MacProcTxSduQueue */
    PRINT_FUNCTION_NAME(__FUNCTION__);

    /* Common stuff */
    LtePhyFillTestTxVectorCommon(fn, sn);
    
    LtePhyUpdateTestTxVector(PDCCH, fn, sn);

    LtePhyUpdateTestTxVector(PDSCH, fn, sn);

    if ((sn == 0) && (fn % 4 == 0) && etm_test_config.pbchEna)
        LtePhyUpdateTestTxVector(PBCH, fn, sn);

    buffer = Mac2PhyIfAllocBufferEx(sizeof (DLSUBFRDESC));
    if (buffer == NULL)
    {
        printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }
    pToPhyMsg = (PGENMSGDESC) buffer;
    // This is index in PhyInst[] array
    pToPhyMsg->phyEntityId = 0;
    pToPhyMsg->msgType = PHY_TXSTART_REQ;
    payload = (PTR) (pToPhyMsg + 1);
    len = lte_if_copy_tx_vector_etm(payload, pSubFrame);
    if (pToPhyMsg)
    {
        pToPhyMsg->msgSpecific = len;
    }

    global_chanIdx = pSubFrame->numberofChannelDescriptors;
    if (0 != fn || 0 != sn)
    {
        LtePhyHiDciSet(NULL, NULL, NULL, fn, sn);
    }

    LtePhyFillTestRxVectorCommon(fn, sn);
    buffer_ul = Mac2PhyIfAllocBufferEx(sizeof (ULSUBFRDESC));
    if (buffer_ul == NULL)
    {
        printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }
    pToPhyMsg = (PGENMSGDESC) buffer_ul;
    pToPhyMsg->phyEntityId = 0;
    pToPhyMsg->msgType = PHY_RXSTART_REQ;
    payload = (PTR) (pToPhyMsg + 1);
    len = lte_if_copy_rx_vector_etm(payload, pUlSubFrame);

    if (pToPhyMsg)
    {
        pToPhyMsg->msgSpecific = len;
    }

    //Add RxVerctor
    AddToHead(PHY_RXSTART_REQ, msgSize, (U8*) buffer_ul, fn, sn);
    //Add TxVerctor
    AddToHead(PHY_TXSTART_REQ, msgSize, (U8*) buffer, fn, sn);

    *phy_vectors = Mac2PhyQueue.StartQueueCurr;
    Mac2PhyQueue.StartQueueCurr = NULL;
    Mac2PhyQueue.TailQueueCurr = NULL;
    return SUCCESS;
}


/** @file LteMac2PhyInterface.c
 *
 * @brief Implemenataion of interface to Message based PHY interface
 * @author Mindspeed Technologies
 * @version $Revision: 1.205 $
 *
 * COPYRIGHT(c) 2008-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "basetypes.h"
#include "maciface.h"
#include "resultcodes.h"
#include "LteMac2PhyInterface.h"

#include "LteMac2RrcInterface.h"
#include "bsmac.h"
#include "fdxmac.h"
#include "LTE_test_config.h"
#include "LteMacTestCfgMgr.h"
#include "LteBsMacDlScheduler.h"
#include "LteBsMacUlScheduler.h"

#include "ltemaccore.h"
#include "msgroute.h"
#include "LteMacTestCfgMgr.h"
#include "LteMacUeContext.h"
#include "diags.h"
#include "macMemUtil.h"
#include "timers.h"
#include "icpu.h"
#include "icpu2_lnx.h"
#include "mlog_uarm.h"
#include "apidefs.h"

#include "tl.h"

#include "lte_mac_state.h"
#include "lte_pusch.h"
#include "lte_phich.h"

#include "lte_if.h"

/****************************************************************************
 Private Definitions
 ****************************************************************************/

#define DLTXSDU_QUEUE_SIZE     40
#define PhySduQ_SIZE           40
// Follow 3GPP TS 36.213 Table 7.1.7.2.1-1
// maximum for Itbs=26 Nprb=100 (20MHz QAM64) is 75376 bits = 9422 bytes
// plus overhead for RxSdu headers
#define RXSDUSZ                 10000

/****************************************************************************
 Private Types
 ****************************************************************************/

// Used to fill HI DCI channel
typedef U32 (*tAddToTail)(U8 MessageType, U32 MessageLen, U8* tempo);
typedef struct stMac2PhyQueue
{
    PMAC2PHY_QUEUE_EL StartQueueCurr;
    PMAC2PHY_QUEUE_EL TailQueueCurr;
    tAddToTail pAddToTail;
} MAC2PHY_QUEUE, *PMAC2PHY_QUEUE;


typedef struct
{
    U32 start;
    U32 diff;
    U32 physaddr;
    U32 frame;
    U32 type;
} SvsrApiMsgStats_t;

#define SIZEOF_SVSRAPIMSGSTAT                                   1024
SvsrApiMsgStats_t SvsrApiMsgStats[SIZEOF_SVSRAPIMSGSTAT];
U32 numSvsrApiMsgStats=0;
V32 IcpuFlagErr = 0;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static RESULTCODE DciPrepareFormat1(U8 *pSdu,
                                     DCI_DATA_INFO* pDciInfo,
                                     U32 *index);

static RESULTCODE DciPrepareFormat1a(U8 *pSdu,
                                     DCI_DATA_INFO* pDciInfo,
                                     U32 *index);

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

static int errorCount = 0;
static int sduCount = 0;

static U32 prachErrors = 0;
static U32 prachSdus = 0;

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/* Tx Ue */
OSAL_QUEUE dlControlTxSduQueue;
OSAL_QUEUE dlControlTxSduQueue2;

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

extern OSAL_SEMAPHORE LteMacDlSema;
extern U8 testMacSduData[1419];
extern U8 testBigSduData[4587];
extern U8 InputBytesTest4[1753];

extern U32 PdcchMgrGetCfiValue(void);

extern U8 testPDCCH[128];
extern BS_CONTEXT *pBsMacDesc;

extern CHCFG  chancfg[100], chancfgms[100] ;
extern PCHCFG pchan[100], pchanms[100] ;
extern SRSCHCFG Srschancfg[100];

//
// PHY Mode
//
extern int PhyMode;
extern int PhyModePeriod;
extern int PhyModeCount;

U8* pRxSduBuf = NULL;
U32 RxSduSet = 0;
U32 RxBufSize = 32768; // increase size for pass 7.2-test //2385 * 11;
U32 RxSizeRest;
U32 MacPhyInfID = LISTTYPEDIS;

volatile U32 gEnaStopFlag = 0;
volatile U32 gMac2PhyDirectFIFOUsed = 0;

// this structures uses just in the C4KEVM-configuration
static LISTPOINTER      ListPointer;
MAC2PHY_QUEUE           Mac2PhyQueue;

pLtePhyHiDciAPISet pHiDciApiSet = NULL;

RESULTCODE (*DciDLPrepareFunctionPtr[MAX_DCI_FORMAT])(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index) =
{
    NULL,
    DciPrepareFormat0,           /* For DCI Foramt 0 */
    DciPrepareFormat1,           /* For DCI Foramt 1 */
    DciPrepareFormat1a,          /* For DCI Foramt 1A */
    NULL,                        /* For DCI Foramt 1B */
    NULL,                        /* For DCI Foramt 1C */
    NULL,                        /* For DCI Foramt 1D */
    NULL,                        /* For DCI Foramt 2 */
    NULL,                        /* For DCI Foramt 2A */
    NULL,                        /* For DCI Fromat 3 */
    NULL                         /* For DCI Format 3A */
};

#define NMM_CMD_NUM 11
U8 nmm_message_types[NMM_CMD_NUM] = {NMM_START, NMM_STOP, NMM_RSSI_MEAS_REQ, NMM_STOP_RSSI_MEAS_REQ, NMM_CELL_SEARCH_REQ, NMM_STOP_CELL_SEARCH_REQ,
    NMM_PBCH_CONFIG_REQ, NMM_STOP_PBCH_REQ, NMM_SIB1_CONFIG_REQ, NMM_BCCH_CONFIG_REQ, NMM_STOP_BCCH_REQ};

int PhySduQ[PhySduQ_SIZE];
volatile unsigned int PhySduQPut = 0;
volatile unsigned int PhySduQGet = 0;

extern U32 global_chanIdx;

static RESULTCODE LteMac2PhySendList(Mac2PhyInterface *pPhy);
static RESULTCODE LteMac2PhyNmmSendList(Mac2PhyInterface *pPhy);
static RESULTCODE LteMac2PhyInitList(void);

static U8* LteMac2PhyGetPhys(U8* pAddr)
{
#ifdef C4KEVM
    return (U8*)IcpuGetPhys((PTR)pAddr);
#else
    return pAddr;
#endif
}

U8* LtePhy2MacGetVirt(U8* pAddr)
{
#ifdef C4KEVM
    return (U8*)IcpuGetVirt((PTR)pAddr);
#else

    return pAddr;
#endif
}


PTR LteMac2PhyGetDataOffset(PTR pMsg)
{
    if (MacPhyInfID == LISTTYPEENA)
    {
        return MsgGetDataOffsetList(pMsg);
    }
    else
    {
        return MsgGetDataOffset(pMsg);
    }
}

static RESULTCODE LteMac2PhyInitList(void)
{
    PMAC2PHY_QUEUE pMac2PhyQueue = &Mac2PhyQueue;
    PLISTPOINTER pListPointer    = &ListPointer;

    pMac2PhyQueue->StartQueueCurr = NULL;
    pMac2PhyQueue->TailQueueCurr = NULL;
    pListPointer->cnt = 0;

    return SUCCESS;
}

/** Free API list of a MAC-PHY message queue
 *
 * @return Standard result code
 */
static void LteMac2PhyApiListFree(void)
{
    PMAC2PHY_QUEUE pMac2PhyQueue = &Mac2PhyQueue;
    PMAC2PHY_QUEUE_EL Curr, Prev;
    PLISTPOINTER pListPointer    = &ListPointer;

    Curr = pMac2PhyQueue->StartQueueCurr;

    while(Curr)
    {
        if(Curr->MessagePtr)
        {
            MsgFreeBuffer((PMSGHEADER)LtePhy2MacGetVirt((PTR)Curr->MessagePtr));
            Curr->MessagePtr = NULL;
        }
        Prev = Curr;
        Curr = Curr->Next;
        Prev->Next = NULL;
        MsgFreeBuffer((PMSGHEADER)Prev);
    }

    pMac2PhyQueue->StartQueueCurr = NULL;
    pMac2PhyQueue->TailQueueCurr = NULL;
    pListPointer->cnt = 0;
}

/** Fill SDU with DCI Format 0 payload
 *
 * @param pSdu Pointer to the target SDU payload block
 * @param pDciInfo TODO:
 * @param index TODO:
 * @return Standard result code
 */
RESULTCODE DciPrepareFormat0(U8 *pSdu, DCI_DATA_INFO *pDciInfo, U32 *index)
{
    return SUCCESS;
}

/** Sends SDU to PHY
 *  This function uses new API interface (as opposite to PhySendMsg) and enqueues SDU messages
 *  into system queue (non-blocking call)
 *  Should replace all PhySendMsg usage
 *
 * @param ApiId PHY API message ID as defined in LtePhyL2Api.h
 * @param Data Pointer to the message block
 * @param Len Size of the message block
 * @return Standard result code
 */
RESULTCODE SduSend(U8 ApiId, U8 *Data, U32 Len)
{
// TODDO: Replace LteMac2PhyAddToTail by AddToTail
#ifdef MX_ARM_ENABLED
    if (Mac2PhyQueue.pAddToTail)
    {
        Mac2PhyQueue.pAddToTail(ApiId, Len, Data);
        return SUCCESS;
    }
    MxPutQueueData(dlControlTxSduQueue, Data);
#else
//    if (ApiId == PHY_TXHISDU_REQ)
//        printf("7\n");

    LteMac2PhyAddToTail(ApiId, Len, Data);
#endif
    return SUCCESS;
}

/** Add API message to the tail of a MAC-PHY message queue
 *
 * @param MessageType PHY API message ID (LtePhyL2Api.h)
 * @param MessageLen Message size
 * @param MessagePtr Pointer to the message buffer
 * @return Standard result code
 */
RESULTCODE LteMac2PhyAddToTail(U8 MessageType, U32 MessageLen, U8 *MessagePtr)
{
    if (MacPhyInfID == LISTTYPEENA)
    {
        PLISTPOINTER pListPointer = &ListPointer;
        PMAC2PHY_QUEUE_EL tempo  = (PMAC2PHY_QUEUE_EL) MsgAllocBuffer(MSG_FAST);//SvsrAllocMsgEx(sizeof(MAC2PHY_QUEUE_EL));

        if (tempo == NULL)
        {
            DiagPrint(DIAG_LEVEL5, "ERROR", "No memory [%s,%i]\n", __FUNCTION__, __LINE__);
            return FAILURE;
        }

        tempo->frameNumber = getFrameNum();
        tempo->subframeNumber = getSubFrameNum();
        tempo->MessageType = MessageType;
        tempo->MessageLen = MessageLen;

        // clean the cache
        IcpuCacheClean((U32*)MessagePtr, MessageLen);

        tempo->MessagePtr = (U8*)LteMac2PhyGetPhys((PTR)MessagePtr);

        tempo->Next = NULL;

        pListPointer->listSavedPointer[pListPointer->cnt].pMac2Phy_Queue_El = (PTR) tempo;
        pListPointer->listSavedPointer[pListPointer->cnt].pMessage          = (PTR) MessagePtr;
        pListPointer->cnt++;

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
    }

    return SUCCESS;
}

static RESULTCODE LteMac2PhySendList(Mac2PhyInterface *pPhy)
{
    PMAC2PHY_QUEUE_EL   pCurrVirt, pNextVirt;
    PMSGHEADER  pMsgHeader = (PMSGHEADER)MsgAllocBuffer(MSG_FAST);
    
    if (pMsgHeader == NULL)
    {
        DiagPrint(DIAG_LEVEL5, "ERROR", "No memory [%s,%i]\n", __FUNCTION__, __LINE__);
        return FAILURE;
    }

    LteMacCheckMsgIntegrity();

    pCurrVirt = Mac2PhyQueue.StartQueueCurr;               // pointer to first structure
    if (pCurrVirt == NULL)
    {
        DiagPrint(DIAG_LEVEL5, "ERROR", "Zero list head [%s,%i]\n", __FUNCTION__, __LINE__);
        return FAILURE;
    }

    pMsgHeader->length = 0;
    MsgAppendU32(pMsgHeader, PHY_QUE_HDR_PARAM_ID, (U32)LteMac2PhyGetPhys((PTR)pCurrVirt));

    // clean the cache
    if(pCurrVirt->Next == NULL)
        IcpuCacheClean((U32*)pCurrVirt, sizeof(MAC2PHY_QUEUE_EL));
    else
        while(pCurrVirt->Next)
        {
            pNextVirt = pCurrVirt->Next;
            pCurrVirt->Next = (PMAC2PHY_QUEUE_EL)LteMac2PhyGetPhys((PTR)pNextVirt);

            // clean the cache
            IcpuCacheClean((U32*)pCurrVirt, sizeof(MAC2PHY_QUEUE_EL));
            pCurrVirt = pNextVirt;
        }
    pPhy->txCb(pPhy->toPhy, pMsgHeader, pMsgHeader->length + sizeof(MSGHEADER));
//    MsgFreeBuffer(pMsgHeader);
    return SUCCESS;
}

static RESULTCODE LteMac2PhyNmmSendList(Mac2PhyInterface *pPhy)
{
    PMAC2PHY_QUEUE_EL   pCurrVirt, pNextVirt;
    PMSGHEADER  pMsgHeader = (PMSGHEADER)MsgAllocBuffer(MSG_FAST);
    
    if (pMsgHeader == NULL)
    {
        DiagPrint(DIAG_LEVEL5, "ERROR", "No memory [%s,%i]\n", __FUNCTION__, __LINE__);
        return FAILURE;
    }

    pCurrVirt = Mac2PhyQueue.StartQueueCurr;               // pointer to first structure
    if (pCurrVirt == NULL)
    {
        DiagPrint(DIAG_LEVEL5, "ERROR", "Zero list head [%s,%i]\n", __FUNCTION__, __LINE__);
        return FAILURE;
    }

    pMsgHeader->length = 0;
    MsgAppendU32(pMsgHeader, PHY_QUE_HDR_PARAM_ID, (U32)LteMac2PhyGetPhys((PTR)pCurrVirt));

    // clean the cache
    if(pCurrVirt->Next == NULL)
        IcpuCacheClean((U32*)pCurrVirt, sizeof(MAC2PHY_QUEUE_EL));
    else
        while(pCurrVirt->Next)
        {
            pNextVirt = pCurrVirt->Next;
            pCurrVirt->Next = (PMAC2PHY_QUEUE_EL)LteMac2PhyGetPhys((PTR)pNextVirt);

            // clean the cache
            IcpuCacheClean((U32*)pCurrVirt, sizeof(MAC2PHY_QUEUE_EL));
            pCurrVirt = pNextVirt;
        }

    pMsgHeader->control = CTRL_BYPASS | CTRL_VERSION; // Encapsulated protocol
    pMsgHeader->type    = MSGT_DATA;
    pMsgHeader->srcID   = MSGS_MAC;
    pMsgHeader->dstID   = MSGS_LARM | MSGS_NMM; // 0x8031;

    pMsgHeader->msgID   = PHY_LTE_NMM_MSG;

    MsgDispatch(MSGS_MAC, pMsgHeader->length + sizeof(MSGHEADER), pMsgHeader);

    return SUCCESS;
}

void SetConfigDl4(TSTPDESC *tstpbs)
{
    tstpbs->number_of_channels = 0x0001;
    tstpbs->number_of_phich_channels = 0x0000;
    tstpbs->Ng = 0x0000;
    tstpbs->pilotPower = 0x0000;
    tstpbs->psyncPower = 0x0000;
    tstpbs->ssyncPower = 0x0000;
    tstpbs->ciPower = -32768;
    tstpbs->paprControl = 0x0000;
    tstpbs->paprThreshold = 0x0000;

    tstpbs->pchan[0] = &chancfg[0];
    tstpbs->pchan[0]->ChanType = 0x0001;
    tstpbs->pchan[0]->chPower = 0x0000;
    tstpbs->pchan[0]->Qm = 0x0006;

    return;
}


#ifndef AC_MSPD_TDD
void SetConfigUl2(TSTPDESC *tstpbs)
{
    // gez S32 tmp1 = 0, tmp2 = 0, tmp3 = 0;

    tstpbs->pchan[0] = &chancfgms[0];
    tstpbs->number_of_channels = 0x01;
    tstpbs->pchan[0]->ChanType = PUSCH;
    tstpbs->pchan[0]->N2_RB = 0x00;
    tstpbs->pchan[0]->N1_CS = 0x00;
    tstpbs->pchan[0]->delta_shift_PUCCH = 0x00;
    tstpbs->pchan[0]->n1_pucch = 0x0000;
    tstpbs->pchan[0]->n2_pucch = 0x0000;
//  tstpbs->pchan[0]->beta_pucch = 0x0000;
    tstpbs->pchan[0]->format_type = 0x0000;

    return;
}
#else
void SetConfigUl2(TSTPDESC *tstpbs)
{

    return;
}
#endif


RESULTCODE LtePhyParamInit(PINITPARM pParm,
                           PDLSUBFRDESC pDlVector,
                           PULSUBFRDESC pUlVector,
                           U32 config)
{
    int tstType;
    int mcs;
    U32 numDataCh, qm, numCtlCh, numCtlSymbols, numSubFrames;

    DLSUBFRDESC dummy;
    
    ConfigureTestBsDlDefault(pParm,
                             pParm,
                             pDlVector,
                             &dummy,
                             &pBsMacDesc->tstpbsDl,
                             &pBsMacDesc->tstpbsUl,
                             4);
    /////// DL dirrection

    ///////////////////////////////////////
    //testCase Parameters
    //10Mhz, QPSK, 0 PDSCH, 1 PDCCH, Correct PBCH
    //Single TxAnt
    ///////////////////////////////////////

    // Initialize Test parameter structure for Base Station
    // PDCCH
    switch (config)
    {
        case MACCONF_FDX0: //PDSCH
        {
             qm = 6;
             numCtlSymbols= 3;
             tstType = 0; // DL
             numDataCh =1;
             numCtlCh = 0;
             mcs = 12;
             numSubFrames=1;
             SetConfigDl4(&pBsMacDesc->tstpbsDl);
        }
            break;
        case MACCONF_FDX1:
        {
            tstType = 0; // DL
            numCtlSymbols= (U8)3;
            numCtlCh= (U8)1;
            numDataCh= (U8)0;
            qm= (U8)2;
            mcs= (U8)12;
            numSubFrames=1;

        }
        break;
        default:
            return FAILURE;
            break;
    }


    LtePhyInitDefault(pParm);

    // Initialize Test parameter structure for Base Station
    pBsMacDesc->tstpbsDl.numCtlSymbols= (U8)numCtlSymbols;
    pBsMacDesc->tstpbsDl.numCtlCh= (U8)numCtlCh;
    pBsMacDesc->tstpbsDl.numDataCh= (U8)numDataCh;
    pBsMacDesc->tstpbsDl.qm= (U8)qm;
    pBsMacDesc->tstpbsDl.mcs= (U8)mcs;
    pBsMacDesc->tstpbsDl.rvidx=3;
    pBsMacDesc->tstpbsDl.nTxCodewords=1;
    pBsMacDesc->tstpbsDl.nTxLayers=1;
    pBsMacDesc->tstpbsDl.cddType=NOCDD;
    pBsMacDesc->tstpbsDl.codeBookIndex=0;
    pBsMacDesc->tstpbsDl.cellID=0;
    pBsMacDesc->tstpbsDl.transmissionMode=SINGLEANT;

    // Setup the number of Channel Descriptors in the pDlVector
    pDlVector->numberofChannelDescriptors= (U8)(numCtlCh + numDataCh);
    // Setup the number of control symbols in the pDlVector
    pDlVector->numCtrlSymbols= (U8)numCtlSymbols;

    switch (config)
    {
        case MACCONF_FDX0:
            pBsMacDesc->tstpbsDl.test_case = 4;
            ConfigureTestBS_DL(pParm, pDlVector, &pBsMacDesc->tstpbsDl, DLTX);
        break;
        case MACCONF_FDX1:
            pBsMacDesc->tstpbsDl.test_case = 7;
            ConfigureTestBS_DL(pParm, pDlVector, &pBsMacDesc->tstpbsDl, DLTX);
        break;
        default:
            return FAILURE;
    }

    switch(config)
    {
        case MACCONF_FDX0:
            qm = 6;
            numCtlSymbols = 0;
            tstType = 1; // UL
            numDataCh = 1;
            numCtlCh = 0;
            mcs = 12;
            numSubFrames = 1;
            SetConfigUl2(&pBsMacDesc->tstpbsUl);
        break;
        case MACCONF_FDX1:
            qm = 6;
            numCtlSymbols = 0;
            tstType = 1; // UL
            numDataCh = 0;
            numCtlCh = 1;
            mcs = 12;
            numSubFrames = 1;
            break;
        default:
            return FAILURE;
        break;
    }

    pBsMacDesc->tstpbsUl.numCtlSymbols= (U8)numCtlSymbols;
    pBsMacDesc->tstpbsUl.numCtlCh= (U8)numCtlCh;
    pBsMacDesc->tstpbsUl.numDataCh= (U8)numDataCh;
    pBsMacDesc->tstpbsUl.qm= (U8)qm;
    pBsMacDesc->tstpbsUl.mcs= (U8)mcs;
    pBsMacDesc->tstpbsUl.rvidx=3;
    pBsMacDesc->tstpbsUl.nTxCodewords=1;
    pBsMacDesc->tstpbsUl.nTxLayers=1;
    pBsMacDesc->tstpbsUl.cddType=NOCDD;
    pBsMacDesc->tstpbsUl.codeBookIndex=0;
    pBsMacDesc->tstpbsUl.cellID=0;
    pBsMacDesc->tstpbsUl.transmissionMode=SINGLEANT;
    pBsMacDesc->tstpbsUl.ul_dl_test_indicator = 1;

    // Setup the number of Channel Descriptors in the pSubFrame
    pUlVector->numberofChannelDescriptors= (U8)(numCtlCh + numDataCh);
    // Setup the number of control symbols in the pUlVector
//    pUlVector->numCtrlSymbols= (U8)numCtlSymbols;
    switch (config)
    {
        case MACCONF_FDX0:
            pBsMacDesc->tstpbsUl.test_case = 2;
            ConfigureTest2_UL(pParm, pUlVector, &pBsMacDesc->tstpbsUl, ULRX);
            if(MacRtMode != MACMODE_NRT)
            {
                pParm->rxAntennaPortCount = 1;
            }
        break;
        case MACCONF_FDX1:
            pBsMacDesc->tstpbsUl.test_case = 4;
            ConfigureTest4_UL(pParm, pUlVector, &pBsMacDesc->tstpbsUl, ULRX);
            if(MacRtMode != MACMODE_NRT)
            {
                pParm->rxAntennaPortCount = 1;
            }
        break;
        default:
            return FAILURE;
    }

    return SUCCESS;
}

/** Fill SDU with DCI format 1 payload
 *
 * @param pSdu Pointer to the target SDU payload block
 * @param pDciInfo TODO:
 * @param index TODO:
 * @return Standard result code
 */
static RESULTCODE DciPrepareFormat1(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index)
{
    return SUCCESS;
}

/** Fill SDU with DCI format 1A payload
 *
 * @param pSdu Pointer to the target SDU payload block
 * @param pDciInfo TODO:
 * @param index TODO:
 * @return Standard result code
 */
static RESULTCODE DciPrepareFormat1a(U8 *pSdu, DCI_DATA_INFO* pDciInfo, U32 *index)
{
    return SUCCESS;
}

/** Inserts PDSCH channel containing all zeroes into scheduler queue
 *
 * @param numBits Number of bits in SDU payload
 * @param chId PHY channel ID
 * @param channelType (PDSCH, see enum ChTypeOptions)
 * @param cwId
 * @return Standard result code
 */
RESULTCODE LtePhyPrepareTestSdu(U8* payload, U32 numBits, U8 chId, U8 channelType, U8 cwId)
{
    PMSGHEADER pHdr = MsgAllocBuffer(MSG_COMMON);
    U32 *pMsg;
    PTXSDUREQ pSduReq;
    U8 Pattern = (payload)? *payload : 0;

    if (pHdr == NULL)
    {
        return FAILURE;
    }

    pMsg = (U32*)LteMac2PhyGetDataOffset((PTR)pHdr);
    pSduReq = (PTXSDUREQ)pMsg;

    memset(pSduReq, 0x00, sizeof(TXSDUREQ));
    pSduReq->phyEntityId = 0;
    pSduReq->msgType = PHY_TXSDU_REQ;
    pSduReq->chanId = chId;
    pSduReq->cwId = cwId;
    pSduReq->uciFormat = 0;
    pSduReq->nackAck = 0;
    pSduReq->channelType = channelType;
    pSduReq->phichGrpNumber = 0;
    pSduReq->maxBitsperCw = numBits;
    pSduReq->msgLen = (numBits+7) >>3;
    pSduReq->pTxSdu = 0;

    memset((U8*)((U8*)pMsg + sizeof(TXSDUREQ)), Pattern, (numBits+7) >> 3);

    if(channelType == PDSCH)
    {
        if(pBsMacDesc->initParm.rxSduLineMonitorInfoenable)
        {
            U8 TimAdvByte = TestMgrIsTimAdv(&testConfConfig, getSubFrameNum());
            if(TimAdvByte)
            {
                MAC_HEADER_CTRL *pMacHdr;
                U8* pPayload;
                pMacHdr = (MAC_HEADER_CTRL*)((U8*)pSduReq + sizeof(TXSDUREQ));
                pMacHdr->E = 0;
                pMacHdr->R = 0;
                pMacHdr->LCID = LCID_ADVANCE;
                pPayload = (U8*)pMacHdr + sizeof(MAC_HEADER_CTRL);
                *pPayload = TimAdvByte;
            }
        }
    }

    //printf("Prepared SDU [%d] bits %d\n", pSduReq->msgLen + sizeof(TXSDUREQ), numBits );
    OsalEnqueue(&dlControlTxSduQueue, (OSAL_PADDRESS)pHdr);

    return SUCCESS;
}

/** Reconfigure PHY initialization values to match 20 MHz bandwidth
 *
 * @param pParm Pointer to preallocated block holding PHY initialization values
 * @return Standard result code
 */
/*RESULTCODE LtePhyIfUpdate20MhzBw(PINITPARM pParm)
{
    pParm->channelBandwidth = BW_20_0_MHZ;
    pParm->numberResourceBlocksperSlot = RB_SLOT_100;
    pParm->fftSize = FFTSZ_2048;
    pParm->numberUsedSubcarriers = NUSC_1200;
    pParm->nMaxDlRb = NMAXRB_100;
    pParm->nMaxUlRb = NMAXRB_100;

    return SUCCESS;
}*/

/** Reconfigure PHY initialization values to match 10 MHz bandwidth
 *
 * @param pParm Pointer to preallocated block holding PHY initialization values
 * @return Standard result code
 */
/*RESULTCODE LtePhyIfUpdate10MhzBw(PINITPARM pParm)
{
    pParm->channelBandwidth = BW_10_0_MHZ;
    pParm->numberResourceBlocksperSlot = RB_SLOT_50;
    pParm->fftSize = FFTSZ_1024;
    pParm->numberUsedSubcarriers = NUSC_600;
    pParm->nMaxDlRb = NMAXRB_50;
    pParm->nMaxUlRb = NMAXRB_50;

    return SUCCESS;
}*/

/** Inserts PBCH channel into scheduler queue
 *
 * @param numBits Number of bits in SDU payload
 * @param chId PHY channel ID
 * @param channelType (PBCH, see enum ChTypeOptions)
 * @param testPbch Pointer to PBCH channel payload
 * @return Standard result code
 */
/*
RESULTCODE LtePhyPrepareTestSduRt(U8* payload, U32 numBits, U8 chId, U8 channelType, U8 cwId, U32 flag)
{
    PMSGHEADER pHdr;
    // gez U8  count = 0;
    U32 *pMsg;
    PTXSDUREQ pSduReq;
    
    pHdr  = MsgAllocBuffer(MSG_FAST);

    if (pHdr == NULL)
    {
        return FAILURE;
    }

    pMsg = (U32*)LteMac2PhyGetDataOffset((PTR)pHdr);
    pSduReq = (PTXSDUREQ)pMsg;

    memset(pSduReq, 0x00, sizeof(TXSDUREQ));
    pSduReq->phyEntityId = 0;
    pSduReq->msgType = PHY_TXSDU_REQ;
    pSduReq->chanId = chId;
    pSduReq->cwId = cwId;
    pSduReq->uciFormat = 0;
    pSduReq->nackAck = 0;
    pSduReq->channelType = channelType;
    pSduReq->phichGrpNumber = 0;
    pSduReq->maxBitsperCw = numBits;
    pSduReq->msgLen = (numBits+7) >> 3;
    pSduReq->pTxSdu = 0;


    if (flag==2)
    {
        memcpy((U8*)((U8*)pMsg + sizeof(TXSDUREQ)), payload, (numBits+7) >> 3);
    }
// for merging with enbMac
    else
    {
        U8 Pattern = (payload)? *payload : 0;
        memset((U8*)((U8*)pMsg + sizeof(TXSDUREQ)), Pattern, (numBits+7) >> 3);
    }

    if(channelType == PDSCH)
    {
        if(pBsMacDesc->initParm.rxSduLineMonitorInfoenable)
        {
            U8 TimAdvByte = TestMgrIsTimAdv(&testConfConfig, getSubFrameNum());
            if(TimAdvByte)
            {
                MAC_HEADER_CTRL *pMacHdr;
                U8* pPayload;
                pMacHdr = (MAC_HEADER_CTRL*)((U8*)pSduReq + sizeof(TXSDUREQ));
                pMacHdr->E = 0;
                pMacHdr->R = 0;
                pMacHdr->LCID = LCID_ADVANCE;
                pPayload = (U8*)pMacHdr + sizeof(MAC_HEADER_CTRL);
                *pPayload = TimAdvByte;
            }
        }
    }

    //printf("Prepared SDU [%d] bits %d\n", pSduReq->msgLen + sizeof(TXSDUREQ), numBits );
//    OsalEnqueue(&dlControlTxSduQueue2, (OSAL_PADDRESS)pHdr);

    #ifdef WIN32
    // ccode
            OsalEnqueue(&dlControlTxSduQueue2, (OSAL_PADDRESS)pHdr);
            return SUCCESS;
    #else
    // linux
            return SduSend(PHY_TXSDU_REQ, (U8*) pSduReq, pSduReq->msgLen + sizeof(TXSDUREQ));
    #endif

    return SUCCESS;
}*/

/** Reroutes received PHY_TXSDU.req message (84xxx-SWG-009-A)
 *  to the associated PHY instance
 *
 * @param pDlSubFrame Pointer to current Downlink subframe descriptor
 * @return Standard result code
 */
RESULTCODE LteMacPhyProcessTxSduReq(PDLSUBFRDESC pDlSubFrame)
{
#ifdef WIN32
    PTSTPDESC pTstpbsDl = &pBsMacDesc->tstpbsDl;
    // gez PTSTPDESC pTstpbsUl = &pBsMacDesc->tstpbsUl;
    Mac2PhyInterface  * pPhy = &pBsMacDesc->Phy;
    MAC_PDU_DESC *pMacPduDesc = &MacPduDesc[0];
    U8* pInDat = &testBigSduData[0];//&testMacSduData[0];
    // gez U32  inDatSz = 3422;//1419*3;//4587;//1419;
    U8* pInCtl = &testPDCCH[0]; //&testMacSduData[0];
    U32  inCtlSz = 128;//1419;

    if (/*pBsMacDesc->schedulingSchema == MAIN_SCHEMA ||*/ pBsMacDesc->schedulingSchema == RCT_SCHEMA)
    {
        PMSGHEADER pCtrlHdr;
        PTXSDUREQ pSduReq;
        while (OsalDequeue(&dlControlTxSduQueue2, (OSAL_PADDRESS *) &pCtrlHdr) == SUCCESS)
        {
            pSduReq = (PTXSDUREQ)LteMac2PhyGetDataOffset((PTR)pCtrlHdr);
            //printf("PDSCH Send SDU [%d]\n",pSduReq->msgLen + sizeof(TXSDUREQ));
            if (MacPhyInfID == LISTTYPEENA)
            {
                LteMac2PhyAddToTail(PHY_TXSDU_REQ, pSduReq->msgLen + sizeof(TXSDUREQ), (U8*)pSduReq);
            }
            else
            {
                if(testConfConfig.nmmMode == 0)
                    pPhy->txCb(pPhy->toPhy, pCtrlHdr, pSduReq->msgLen + sizeof(TXSDUREQ) + sizeof(MSGHEADER));
            }
            if(MacRtMode == MACMODE_NRT)
            {
                MsgFreeBuffer(pCtrlHdr);
            }
        }

        while (OsalDequeue(&dlControlTxSduQueue, (OSAL_PADDRESS *) &pCtrlHdr) == SUCCESS)
        {
            pSduReq = (PTXSDUREQ)LteMac2PhyGetDataOffset((PTR)pCtrlHdr);
            //printf("Control Send SDU [%d]\n",pSduReq->msgLen + sizeof(TXSDUREQ));
            if (MacPhyInfID == LISTTYPEENA)
            {
                LteMac2PhyAddToTail(PHY_TXSDU_REQ, pSduReq->msgLen + sizeof(TXSDUREQ), (U8*)pSduReq);
            }
            else
            {
                if(testConfConfig.nmmMode == 0)
                    pPhy->txCb(pPhy->toPhy, pCtrlHdr, pSduReq->msgLen + sizeof(TXSDUREQ) + sizeof(MSGHEADER));
            }
            if(MacRtMode == MACMODE_NRT)
            {
                MsgFreeBuffer(pCtrlHdr);
            }
        }
    }
    else
    {
        printf("Error in function [%s,%d]\n", __FUNCTION__, __LINE__);
    }
#endif

    return SUCCESS;
}

static void LteMacPhyCfgProgramInfo(U32 ConsoleVal, U32 Mode)
{
    if (ConsoleVal & PHYINIT_NEW_MAC_TO_PHY_INTERFACE)
    {
        printf("MAC 2 PHY FIFO %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_PHY_MAINTAINS_PBCH)
    {
        printf("PBCH generation %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_ROBUST_CONTROL)
    {
        printf("robust-control %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_USE_TXSDU_POINTER)
    {
        printf("use TxSdu pointer %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_TWO_MUXCONTROL_LISTS)
    {
        printf("two mux control lists %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_DONT_FREE_NEXT_MISSED_VECTOR)
    {
        printf("don't free next missed vector %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_PHY_SEND_SECOND_TTI_IND)
    {
        printf("phy send second tti ind %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_MUTE_RXSDU_FOR_SR_RACH)
    {
        printf("mute rxsdu for sr rach %s\n", Mode? "at phy enabled": "received from console");
    }
    if (ConsoleVal & PHYINIT_LOW_LATENCY_PATH)
    {
        printf("low latency path %s\n", Mode? "at phy enabled": "received from console");
    }
}

/** Initialization of RX SDU ring buffer
 *
 * @param Pointer to STARTREQ
 */
static void InitRxSduBuf(void* param)
{
    if(pBsMacDesc->PhyAutoCtx.RxSduEna)
    {
        PSTARTREQ pStartReq = (PSTARTREQ) param;
    
        if (pRxSduBuf)
        {
            OsalFreeHeap(&OsalGlobalHeap, (OSAL_PADDRESS)pRxSduBuf);
            pRxSduBuf = NULL;
        }
    
        if (pStartReq->count == 0 || pStartReq->count > 120)
            RxBufSize = RXSDUSZ * 120;
        else
            RxBufSize = RXSDUSZ * pStartReq->count;                
    
        OsalAllocHeap(&OsalGlobalHeap, (OSAL_PADDRESS*)&pRxSduBuf, RxBufSize);
        if(pRxSduBuf == NULL)
        {
            pBsMacDesc->PhyAutoCtx.RxSduEna = FALSE;
        }
        RxSduSet = 0;
        RxSizeRest = RxBufSize;
    }
}

/** Copies new RX SDU to the ring buffer
 *
 * @param Buf Pointer to the TX SDU
 * @param Size Size of this SDU
 */
static void AddToBuf(U8* Buf, U32 Size)
{
    if(Size <= RxSizeRest)
    {
        memcpy(pRxSduBuf + RxSduSet,Buf,  Size);
        RxSduSet = (RxSduSet + Size);
        RxSizeRest -= Size;
        return;
    }
}

/** Send contents of the RX SDU ring buffer to Host
 *  FIXME: Merge with the SendBufToHostThr or create a thread for latter
 *
 */
static void SendBufToHost(void)
{
    U32 idx = 0;
    PRXSDUIND pRxSduInd = NULL;
    U32 RxSduLen = 0;
    PTR pMacPdu;
    PMSGHEADER pInd = NULL;
    U32 Len = 0;
    U32 frameNum, subFrameNum, channelId;
    U32 ChType;    

    if(!RxSduSet)
    {
        pInd = MsgAllocBuffer(MSG_COMMON);
        pInd->control = CTRL_VERSION;
        pInd->length  = 0;
        pInd->type    = MSGT_INDICATION;
        pInd->srcID   = MSGS_MAC;
        pInd->dstID   = MSGS_HOST;
        pInd->msgID   = MAC_RXSDU_IND;
        MsgAppendU32(pInd, PAR_RESULT, FAILURE);
        MsgDispatch(pInd->srcID, pInd->length + sizeof(MSGHEADER), pInd);

        return;
    }  

    while(idx < RxSduSet)
    {
        pRxSduInd = (PRXSDUIND)((U8*)pRxSduBuf + idx);

        pInd = MsgAllocBuffer(MSG_COMMON);
        pInd->control = CTRL_VERSION;
        pInd->length  = 0;
        pInd->type    = MSGT_INDICATION;
        pInd->srcID   = MSGS_MAC;
        pInd->dstID   = MSGS_HOST;


        if (pRxSduInd->msgType == PHY_RXSTATUS_IND)
        {
            PRXSTATUSIND pRxStatusInd = NULL;

            RxSduLen = sizeof(RXSTATUSIND);

            printf("convert values from RxSduInd to RxStatusInd\n");
            pRxStatusInd = (PRXSTATUSIND) pRxSduInd;
            if (pRxStatusInd->statusType == PRACH_RESULT)
            {
                ChType = PRACH;
                Len    = pRxStatusInd->eventNumber * sizeof(RX_PRACH_STATUS_EVENT);
            }
            else if(pRxStatusInd->statusType == CQIRIHI_RESULT)
            {
                PRX_CQIRIHI_STATUS_EVENT pMsg1 = (PRX_CQIRIHI_STATUS_EVENT)&pRxStatusInd->pEventList;
                ChType = CQIRIHI_RESULT;
                Len    = pRxStatusInd->eventNumber * sizeof(RX_CQIRIHI_STATUS_EVENT) + pMsg1->status_len;
            }
            else if(pRxStatusInd->statusType == SRS_RESULT)
            {
                ChType = SRS_RESULT;
                Len    = pRxStatusInd->eventNumber * (sizeof(RX_SRS_STATUS_EVENT) - sizeof(U8*)) + pRxStatusInd->msgSpecific /*always multiple of 4*/;
            }
            else
            {
                print_err("unsupported statusType");
                return;
            }

            frameNum    = pRxStatusInd->frameNum;
            subFrameNum = pRxStatusInd->subFrameNum;
            channelId   = 0;
            pMacPdu     = &pRxStatusInd->pEventList;

            pInd->msgID = MAC_RXSTATUS_IND;

            MsgAppendU32(pInd, PAR_PHY_SFN, frameNum);

            MsgAppendU32(pInd, PAR_PHY_SFCOUNT, subFrameNum);

            MsgAppendU32(pInd, PAR_PHY_CH_TYPE, ChType);

            MsgAppendParam(pInd, PAR_BIN_DATA, Len, pMacPdu);


        }
        else
        {
            RxSduLen = sizeof(RXSDUIND);

            ChType      = pRxSduInd->chanType;
            Len         = pRxSduInd->numBitsRx >>3;
            frameNum    = pRxSduInd->frameNum;
            subFrameNum = pRxSduInd->subFrameNum;
            channelId   = pRxSduInd->channelId;
            pMacPdu     = &pRxSduInd->pRxSdu;
            pInd->msgID = MAC_RXSDU_IND;            

            MsgAppendU32(pInd, PAR_PHY_SFN, frameNum);

            MsgAppendU32(pInd, PAR_PHY_SFCOUNT, subFrameNum);

            MsgAppendU32(pInd, PAR_PHY_CH_TYPE, ChType);

            MsgAppendU32(pInd, PAR_PHY_CH_IDX, channelId);        

            MsgAppendParam(pInd, PAR_BIN_DATA, Len, pMacPdu);
        }

        MsgDispatch(pInd->srcID, pInd->length + sizeof(MSGHEADER), pInd);
        idx += (Len + RxSduLen);

    }

// old behaviour of SendBufToHost
/*    U32 idx = 0;
    PRXSDUIND pRxSduInd = NULL;
    U32 RxSduLen = 0;
    PTR pMacPdu;
    PMSGHEADER pInd = NULL;
    U32 Len = 0;
    U32 Data;
  //  U32 Count = 0;
    if(!RxSduSet)
    {
        pInd = MsgAllocBuffer(MSG_COMMON);
        pInd->control = CTRL_VERSION;
        pInd->length  = 0;
        pInd->type    = MSGT_INDICATION;
        pInd->srcID   = MSGS_MAC;
        pInd->dstID   = MSGS_HOST;
        pInd->msgID   = MAC_RXSDU_IND;
        MsgAppendU32(pInd, PAR_RESULT, FAILURE);
        MsgDispatch(pInd->srcID, pInd->length + sizeof(MSGHEADER), pInd);

        return;
    }

    RxSduLen = sizeof(RXSDUIND);

    while(idx < RxSduSet)
    {
        pRxSduInd = (PRXSDUIND)((U8*)pRxSduBuf + idx);
        Len = pRxSduInd->numBitsRx >>3;
    //    printf("Num%d idx %x Len %d Type %x SF %x SFN %x\n",Count++, idx,
    //    pRxSduInd->numBitsRx, pRxSduInd->chanType, pRxSduInd->frameNum, pRxSduInd->subFrameNum);
        pInd = MsgAllocBuffer(MSG_COMMON);
        pInd->control = CTRL_VERSION;
        pInd->length  = 0;
        pInd->type    = MSGT_INDICATION;
        pInd->srcID   = MSGS_MAC;
        pInd->dstID   = MSGS_HOST;
        pInd->msgID   = MAC_RXSDU_IND;

        Data = (U32)pRxSduInd->frameNum;
        MsgAppendU32(pInd, PAR_PHY_SFN, Data);

        Data = (U32)pRxSduInd->subFrameNum;
        MsgAppendU32(pInd, PAR_PHY_SFCOUNT, Data);

        Data = (U32)pRxSduInd->chanType;
        MsgAppendU32(pInd, PAR_PHY_CH_TYPE, Data);

        Data = (U32)pRxSduInd->channelId;
        MsgAppendU32(pInd, PAR_PHY_CH_IDX, Data);

        pMacPdu = &pRxSduInd->pRxSdu;

        Len = (pRxSduInd->numBitsRx + 7) >>3;
        MsgAppendParam(pInd, PAR_BIN_DATA, Len, pMacPdu);

        MsgDispatch(pInd->srcID, pInd->length + sizeof(MSGHEADER), pInd);
        idx += (Len + RxSduLen);
        
    }*/
}

static void DrainRxSduBufToHost(void)
{
    PMSGHEADER pInd = NULL;
    if(!RxSduSet)
        return;
    // Alloc I-CPU message
    pInd = (PMSGHEADER)MsgAllocBuffer(MSG_COMMON);
    if (pInd == NULL)
    {
        printf("No memory in MSG_COMMON block\n");
        return;
    }

    pInd->control = CTRL_VERSION;
    pInd->length  = 0;
    pInd->type    = MSGT_INDICATION;
    pInd->srcID   = MSGS_MAC;
    pInd->dstID   = MSGS_HOST;
    pInd->msgID   = MAC_RXBUFDRAIN_IND;

    MsgAppendParam(pInd, PAR_BIN_DATA, RxSduSet, pRxSduBuf);
    MsgDispatch(pInd->srcID, pInd->length + sizeof(MSGHEADER), pInd);
    RxSduSet = 0;
    RxSizeRest = RxBufSize;

    return;
}

static void LteMacSendPhyReadyInd(void)
{
    PMSGHEADER pInd = MsgAllocBuffer(MSG_COMMON);
    if (pInd)
    {// send indication to Host, it will know when to send next DLStart
        pInd->control = CTRL_VERSION;
        // Zero length
        pInd->length = 0;
        pInd->type =  MSGT_INDICATION;
        pInd->srcID = MSGS_MAC;
        pInd->dstID = MSGS_HOST;
        pInd->msgID = MAC_PHYREADY_IND;
        MsgDispatch(pInd->srcID, pInd->length + sizeof(MSGHEADER), pInd);
    }
}

extern volatile U32 gPhyStopReq;
/** Send API message (84xxx-SWG-009-A) to the PHY using legacy interface
 *
 * @param MessageType PHY API message ID (LtePhyL2Api.h)
 * @param param Message-dependent parameter
 * @return Standard result code
 */
RESULTCODE LteMacSendToPhyMessage(U32 MessageType, PTR param, U32 flag)
{
    PGENMSGDESC pToPhyMsg = NULL;
    PULSUBFRDESC pUlSubFrame;
    PDLSUBFRDESC pDlSubFrame;
    OSAL_SIZE msgSize = 0;
    PTR payload = NULL;
    U16 len;
    Mac2PhyInterface *pPhy = &pBsMacDesc->Phy;
    PMSGHEADER buffer = NULL;

    switch(MessageType)
    {
        case PHY_TXSTART_REQ:
        case PHY_RXSTART_REQ:
        case PHY_INIT_REQ:
        case PHY_START_REQ:
        case PHY_STOP_REQ:
        case PHY_SHUTDOWN_REQ:
        case PHY_RECONFIG_REQ:
            buffer = MsgAllocBuffer(MSG_FAST);
            if (buffer == NULL)
            {
                //DiagPrint(DIAG_LEVEL5, "ERROR", "No memory [%s,%i]\n", __FUNCTION__, __LINE__);
                printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }
            pToPhyMsg = (PGENMSGDESC)LteMac2PhyGetDataOffset((PTR)buffer);
            // This is index in PhyInst[] array
            pToPhyMsg->phyEntityId = 0;
            pToPhyMsg->msgType = MessageType;
            payload = (PTR) (pToPhyMsg + 1);
            break;
        default:
            break;
    }

    switch (MessageType)
    {
        case PHY_TXSTART_REQ:
            pDlSubFrame = (PDLSUBFRDESC) param;
            if(testConfConfig.nmmMode)
            {
                pToPhyMsg->msgType = PHY_TXSTART_CONF;
                LteMacProcessFromPhyMessage(pToPhyMsg, pDlSubFrame, NULL, NULL);
                if (buffer)
                    MsgFreeBuffer(buffer);
                return SUCCESS;
            }

            if (pDlSubFrame->numberofChannelDescriptors > MacMaxChanDescs)
            {
                DiagPrint(DIAG_LEVEL28, "PHY_TXSTART_REQ", "SubFrame->numChannelDescriptors=%d "
                          "overflow.\n", pDlSubFrame->numberofChannelDescriptors);
                break;
            }

            len = lte_if_copy_tx_vector(payload, pDlSubFrame);
            if (pToPhyMsg)
            {
                pToPhyMsg->msgSpecific = len;
            }

            pBsMacDesc->TxState.NPhyChanLeft = pDlSubFrame->numberofChannelDescriptors;
            pBsMacDesc->TxState.NextChanID = 0;

            msgSize = len + sizeof(GENMSGDESC);
            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            DiagPrint(DIAG_LEVEL28, "PHY_TXSTART_REQ", "Size=%d\n", msgSize);
            break;

        case PHY_TXSDU_REQ:
            pDlSubFrame = (PDLSUBFRDESC) param;
            if ((LteMacPhyProcessTxSduReq(pDlSubFrame)) == SUCCESS)
                return SUCCESS;
            break;

        case PHY_RXSTART_REQ:
            if(testConfConfig.nmmMode)
            {
                if (buffer)
                    MsgFreeBuffer(buffer);
                return SUCCESS;
            }
            pUlSubFrame = (PULSUBFRDESC) param;
            if (pUlSubFrame->numberofChannelDescriptors > MacMaxChanDescs)
            {
                DiagPrint(DIAG_LEVEL28, "PHY_RXSTART_REQ", "SubFrame->numberofChannelDescriptors="
                          "%d overflow.\n", pUlSubFrame->numberofChannelDescriptors);
                break;
            }
            len = lte_if_copy_rx_vector(payload, pUlSubFrame);

            if (pToPhyMsg)
            {
                pToPhyMsg->phyEntityId = 0;
                pToPhyMsg->msgSpecific = len;
            }

            msgSize = len + sizeof(GENMSGDESC);

            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            break;

        case PHY_INIT_REQ:
        case PHY_RECONFIG_REQ:

            if(pBsMacDesc == NULL)
            {
                MsgFreeBuffer(buffer);
                printf("The operation is not allowed at this time, please do createmac first\n");
                return FAILURE;
            }

            gPhyStopReq = 0;
            if (param)
            {// console case - highest priority
                LteMacPhyCfgProgramInfo(*(U32*)param, 0);
                pBsMacDesc->initParm.phyCfg |= ((*(U32*)param) & 0x3ff);
            }

            gMac2PhyDirectFIFOUsed = (pBsMacDesc->initParm.phyCfg & PHYINIT_NEW_MAC_TO_PHY_INTERFACE)? 1 : 0;

            LteMacPhyCfgProgramInfo(pBsMacDesc->initParm.phyCfg, 1);

            // copy config to the message
            memcpy(payload, &pBsMacDesc->initParm, sizeof(INITPARM));
            // This is index in PhyInst[] array
            if (pToPhyMsg)
            {
                pToPhyMsg->phyEntityId = 0;
                pToPhyMsg->msgSpecific = sizeof(INITPARM);
            }
            msgSize = sizeof(GENMSGDESC) + sizeof(INITPARM);
            pHiDciApiSet = lte_phich_dci_fake;

            if (MacPhyInfID == LISTTYPEENA)
            {
                printf("New HI_DCI0 API supported mode\n");
                pHiDciApiSet = lte_phich_dci_set;
                LteBsMacDlSchedulerReconfigByPhyMode();
            }

            LteMac2PhyApiListFree();

            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            flag = 1;

            DiagDumpPhyApi(DIAG_LEVEL29, pToPhyMsg);
        break;

        case PHY_START_REQ:
            globalTTITickCount = pBsMacDesc->initParm.radioFrameNumber * 10 + pBsMacDesc->initParm.subframeNumber;
            setFrameSubFrameNum();
            setTxFrameSubFrameNum();

            LteMac2PhyApiListFree();

            InitRxSduBuf(param);

            if (pToPhyMsg)
            {
                memcpy(pToPhyMsg, param, sizeof(STARTREQ));
            }
            msgSize = sizeof(STARTREQ);

            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            break;

        case PHY_STOP_REQ:            
      
        case PHY_SHUTDOWN_REQ:
            msgSize = sizeof(GENMSGDESC);
            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            flag = 1;
            printf("\nprepare for send PHY_STOP_REQ or PHY_SHUTDOWN_REQ\n");
            break;

        case PHY_PAUSE_REQ:

        case PHY_RESUME_REQ:
            msgSize = sizeof(GENMSGDESC);
            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            if (MessageType == PHY_PAUSE_REQ)
            {
                printf("\nSending PHY_PAUSE_REQ\n");
            }
            else
            {
                printf("\nSending PHY_RESUME_REQ\n");
            }

            break;

        default:
            DiagPrint(DIAG_LEVEL28, "ERROR", "Invalid PHY message\n");
            break;
    }

    if (MacPhyInfID != LISTTYPEENA)
    {

        pPhy->txCb(pPhy->toPhy, buffer, msgSize + sizeof(MSGHEADER));
        if (buffer)
            MsgFreeBuffer(buffer);
    }
    else
    {
        if (flag)
        {
            LteMac2PhySendList(pPhy);
            LteMac2PhyInitList();
        }
    }

    return SUCCESS;
}

static U32 LteMacSendToPhyNMM(U8 *MessageTypes, U32 **pDatas, U32 *nMsgSizes, U32 nMessages)
{
    PMSGHEADER pResp;
    PMAC2PHY_QUEUE_EL pCtrlBlock, pNextVirt;
    PMAC2PHY_QUEUE_EL pCtrlPreviousBlock = NULL;
    PMAC2PHY_QUEUE_EL pCurrVirt = NULL;
    U8 MessageType;
    U32* pData;
    U32 nMsgSize;
    U32 i;

    for (i = 0; i < nMessages; i++) {
        MessageType = MessageTypes[i];
        pData = pDatas[i];
        nMsgSize = nMsgSizes[i];

        pCtrlBlock = (PMAC2PHY_QUEUE_EL) MsgAllocBuffer(MSG_FAST);
        if (pCtrlBlock == NULL) {
            printf("No memory to send ICPU data, pData=%x, Size=%d\n", (U32)pData, nMsgSize);
            return 1;
        }

        if (i == 0)
            pCurrVirt = pCtrlBlock;
        
        memset(pCtrlBlock, 0, sizeof(*pCtrlBlock));
            IcpuCacheClean((U32*)pData, nMsgSize);
        pCtrlBlock->MessagePtr = LteMac2PhyGetPhys((U8*) pData);
        pCtrlBlock->MessageLen = nMsgSize;
        pCtrlBlock->MessageType = MessageType;
        pCtrlBlock->Next = NULL;
        if (pCtrlPreviousBlock)
            pCtrlPreviousBlock->Next = pCtrlBlock;  // use virtual for now
        pCtrlPreviousBlock = pCtrlBlock;
    }

    //rc = SvsrSendMsg(0, IID_LTE_NMM, pHead, SVSR_MSG_OPT_DEFAULT);

    pResp = (PMSGHEADER)MsgAllocBuffer(MSG_FAST);
    if (pResp == NULL) {
        printf("No memory for ICPU API\n");
        return 2;
    }

    pResp->length = 0;
    MsgAppendU32(pResp, PHY_QUE_HDR_PARAM_ID, (U32)LteMac2PhyGetPhys((PTR)pCurrVirt));

    // clean the cache
    if(pCurrVirt->Next == NULL)
        IcpuCacheClean((U32*)pCurrVirt, sizeof(MAC2PHY_QUEUE_EL));
    else
        while(pCurrVirt->Next)
        {
            pNextVirt = pCurrVirt->Next;
            pCurrVirt->Next = (PMAC2PHY_QUEUE_EL)LteMac2PhyGetPhys((PTR)pNextVirt);

            // clean the cache
            IcpuCacheClean((U32*)pCurrVirt, sizeof(MAC2PHY_QUEUE_EL));
            pCurrVirt = pNextVirt;
        }


    pResp->control = CTRL_BYPASS | CTRL_VERSION; // Encapsulated protocol
    pResp->type    = MSGT_DATA;
    pResp->srcID   = MSGS_MAC;
    pResp->dstID   = MSGS_LARM | MSGS_NMM; // 0x8031;

    pResp->msgID   = PHY_LTE_NMM_MSG;

    MsgDispatch(MSGS_MAC, pResp->length + sizeof(MSGHEADER), pResp);

    return SUCCESS;
}

RESULTCODE LteMacSendToPhyNmmMessage(U32 MessageType, PTR param, U32 flag)
{
    PGENMSGDESC pToPhyMsg = NULL;
    OSAL_SIZE msgSize = 0;
    PTR payload = NULL;
    Mac2PhyInterface *pPhy = &pBsMacDesc->Phy;
    PMSGHEADER buffer = NULL;

    switch(MessageType)
    {
        case NMM_START:
        case NMM_STOP:

        case NMM_RSSI_MEAS_REQ:
        case NMM_STOP_RSSI_MEAS_REQ:

        case NMM_CELL_SEARCH_REQ:
        case NMM_STOP_CELL_SEARCH_REQ:

        case NMM_PBCH_CONFIG_REQ:
        case NMM_STOP_PBCH_REQ:

        case NMM_SIB1_CONFIG_REQ:

        case NMM_BCCH_CONFIG_REQ:
        case NMM_STOP_BCCH_REQ:

            buffer = MsgAllocBuffer(MSG_FAST);
            if (buffer == NULL)
            {
                //DiagPrint(DIAG_LEVEL5, "ERROR", "No memory [%s,%i]\n", __FUNCTION__, __LINE__);
                printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }
            pToPhyMsg = (PGENMSGDESC)LteMac2PhyGetDataOffset((PTR)buffer);
            // This is index in PhyInst[] array
            pToPhyMsg->phyEntityId = 0;
            pToPhyMsg->msgType = MessageType;
            payload = (PTR) (pToPhyMsg + 1);
            break;

        default:
            break;
    }

    switch (MessageType)
    {
        case NMM_START:
        {
            STARTREQ* pParam = (STARTREQ*)param;
            fapi_nmm_start_desc_t *nmm_start_desc = (fapi_nmm_start_desc_t *)payload;
            STARTREQ start_params;

            nmm_start_desc->supported_rat = 1; /* lte */
            nmm_start_desc->duplex = 0; /* fdd */
            
            start_params.mode = pParam->mode; /* 0:cpri;1:cpri_simulator */
            start_params.msgType = MessageType;
            start_params.phyEntityId = 0;
            start_params.count = pParam->count; 
            start_params.period = pParam->period;
            memcpy(nmm_start_desc->vendor_specific_list, &start_params, sizeof(STARTREQ));

            msgSize = sizeof(fapi_nmm_start_desc_t) + sizeof(GENMSGDESC);
            pToPhyMsg->msgSpecific = msgSize;
            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
         }
        break;        

        case NMM_STOP:
            msgSize = sizeof(GENMSGDESC);
            pToPhyMsg->msgSpecific = msgSize;
            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            break;

        case NMM_RSSI_MEAS_REQ:
            msgSize = sizeof(fapi_nmm_rssi_meas_req_desc_t);
            memcpy(payload, param, msgSize);
            msgSize += sizeof(GENMSGDESC);
            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            break;

        case NMM_STOP_RSSI_MEAS_REQ:
            msgSize = sizeof(GENMSGDESC);
            pToPhyMsg->msgSpecific = msgSize;
            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            break;

        case NMM_CELL_SEARCH_REQ:
            msgSize = sizeof(fapi_nmm_cell_search_req_desc_t);
            memcpy(payload, param, msgSize);
            msgSize += sizeof(GENMSGDESC);
            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            break;
        case NMM_STOP_CELL_SEARCH_REQ:
            msgSize = sizeof(GENMSGDESC);
            pToPhyMsg->msgSpecific = msgSize;
            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            break;

        case NMM_PBCH_CONFIG_REQ:
            msgSize = sizeof(fapi_nmm_pbch_config_req_desc_t);
            memcpy(payload, param, msgSize);
            msgSize += sizeof(GENMSGDESC);
            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            break;
        case NMM_STOP_PBCH_REQ:
            msgSize = sizeof(GENMSGDESC);
            pToPhyMsg->msgSpecific = msgSize;
            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            break;

        case NMM_SIB1_CONFIG_REQ:
            msgSize = sizeof(fapi_nmm_sib1_config_req_desc_t);
            memcpy(payload, param, msgSize);
            msgSize += sizeof(GENMSGDESC);
            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            break;

        case NMM_BCCH_CONFIG_REQ:
            msgSize = sizeof(fapi_nmm_bcch_config_req_desc_t);
            memcpy(payload, param, msgSize);
            msgSize += sizeof(GENMSGDESC);
            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            break;

        case NMM_STOP_BCCH_REQ:
            msgSize = sizeof(GENMSGDESC);
            pToPhyMsg->msgSpecific = msgSize;
            LteMac2PhyAddToTail(MessageType, msgSize, (U8*)buffer);
            break;

        default:
            DiagPrint(DIAG_LEVEL28, "ERROR", "Invalid PHY message\n");
            break;
    }

    if (MacPhyInfID != LISTTYPEENA)
    {// this is not supported by peer side actually
        pPhy->txCb(pPhy->toPhy, buffer, msgSize + sizeof(MSGHEADER));
        if (buffer)
            MsgFreeBuffer(buffer);
    }
    else
    {
        if (flag)
        {
            LteMac2PhyNmmSendList(pPhy);
            LteMac2PhyInitList();
        }
    }

    return SUCCESS;
}

RESULTCODE LteMacSendToPhyNmmMessageTest(U32 MessageType, PTR param, U32 flag)
{
    PGENMSGDESC pToPhyMsg = NULL;
    PTR payload = NULL;
    PMSGHEADER buffer = NULL;
    U16 nmmCmdType;
    PGENMSGDESC pToPhyMsg_rssi_req = NULL;
    PGENMSGDESC pToPhyMsg2 = NULL;
    PGENMSGDESC pToPhyMsg3 = NULL;
    PGENMSGDESC pToPhyMsg4 = NULL;
    PTR payload_rssi_req = NULL;
    PTR payload2 = NULL;
    PTR payload3 = NULL;
    PTR payload4 = NULL;
    PGENMSGDESC buffer_rssi_req = NULL;
    PGENMSGDESC buffer2 = NULL;
    PGENMSGDESC buffer3 = NULL;
    PGENMSGDESC buffer4 = NULL;
    U16 rssi_earfcn_list[NMM_LTE_MAX_CARRIER_LIST]={3100,3101};
    U16 siPeriodicity_list[MAX_SI_LIST] = {80};
    U16 tmp_i;

    nmmCmdType = 0;    // default is nmm start
    if (param)
        nmmCmdType = *((U16 * ) param);

    switch(MessageType)
    {
        case NMM_STOP:
            buffer = MsgAllocBuffer(MSG_FAST);
            if (buffer == NULL)
            {
                //DiagPrint(DIAG_LEVEL5, "ERROR", "No memory [%s,%i]\n", __FUNCTION__, __LINE__);
                printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }
            pToPhyMsg = (PGENMSGDESC)LteMac2PhyGetDataOffset((PTR)buffer);
            // This is index in PhyInst[] array
            pToPhyMsg->phyEntityId = 0;
            pToPhyMsg->msgType = MessageType;
            payload = (PTR) (pToPhyMsg + 1);
            break;
        /* FIXME (OS) - tmp - hard-coded 4 messages for nmm test: start, cell search, pbch and sib1 */
        case NMM_START:
            buffer = MsgAllocBuffer(MSG_FAST);
            if (buffer == NULL) {
                printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }

            buffer_rssi_req = (PGENMSGDESC)MsgAllocBuffer(MSG_FAST);
            if (buffer_rssi_req == NULL) {
                printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }

            buffer2 = (PGENMSGDESC)MsgAllocBuffer(MSG_FAST);
            if (buffer2 == NULL) {
                printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }
            
            buffer3 = (PGENMSGDESC)MsgAllocBuffer(MSG_FAST);
            if (buffer3 == NULL) {
                printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }
            
            buffer4 = (PGENMSGDESC)MsgAllocBuffer(MSG_FAST);
            if (buffer4 == NULL) {
                printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }
            
            pToPhyMsg = (PGENMSGDESC) buffer;
            pToPhyMsg->phyEntityId = 0;
            pToPhyMsg->msgSpecific = sizeof(fapi_nmm_start_desc_t) + sizeof(GENMSGDESC);
            pToPhyMsg->msgType = MessageType;
            payload = (PTR) (pToPhyMsg + 1);

            pToPhyMsg_rssi_req = (PGENMSGDESC) buffer_rssi_req;
            pToPhyMsg_rssi_req->phyEntityId = 0;
            pToPhyMsg_rssi_req->msgSpecific = sizeof(fapi_nmm_rssi_meas_req_desc_t) + sizeof(GENMSGDESC);
            pToPhyMsg_rssi_req->msgType = NMM_RSSI_MEAS_REQ;
            payload_rssi_req = (PTR) (pToPhyMsg_rssi_req + 1);
            
            pToPhyMsg2 = (PGENMSGDESC) buffer2;
            pToPhyMsg2->phyEntityId = 0;
            pToPhyMsg2->msgSpecific = sizeof(fapi_nmm_cell_search_req_desc_t) + sizeof(GENMSGDESC);
            pToPhyMsg2->msgType = NMM_CELL_SEARCH_REQ;
            payload2 = (PTR) (pToPhyMsg2 + 1);
            
            pToPhyMsg3 = (PGENMSGDESC) buffer3;
            pToPhyMsg3->phyEntityId = 0;
            pToPhyMsg3->msgSpecific = sizeof(fapi_nmm_pbch_config_req_desc_t) + sizeof(GENMSGDESC);
            pToPhyMsg3->msgType = NMM_PBCH_CONFIG_REQ;
            payload3 = (PTR) (pToPhyMsg3 + 1);
            
            pToPhyMsg4 = (PGENMSGDESC) buffer4;
            pToPhyMsg4->phyEntityId = 0;
            pToPhyMsg4->msgSpecific = sizeof(fapi_nmm_sib1_config_req_desc_t) + sizeof(GENMSGDESC);
            pToPhyMsg4->msgType = NMM_SIB1_CONFIG_REQ;
            payload4 = (PTR) (pToPhyMsg4 + 1);
            
            break;

        case NMM_CMD:
            if (nmmCmdType >= NMM_CMD_NUM)
            {
                printf("ERROR: unsupported NMM command %d [%s,%s:%d]\n", nmmCmdType, __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }
        
            buffer = MsgAllocBuffer(MSG_FAST);
            if (buffer == NULL)
            {
                printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
                return FAILURE;
            }
        
            pToPhyMsg = (PGENMSGDESC) buffer;
            pToPhyMsg->msgSpecific = sizeof(GENMSGDESC);    //for request without parameters
            pToPhyMsg->phyEntityId = 0;
            pToPhyMsg->msgType = nmm_message_types[nmmCmdType];
            payload = (PTR) (pToPhyMsg + 1);
            break;

        default:
            break;
    }

    switch (MessageType)
    {
        case NMM_START:
        {
            #define NMM_START_MSG_NUM 5
            U8 message_types[NMM_START_MSG_NUM] = {NMM_START, NMM_RSSI_MEAS_REQ, NMM_CELL_SEARCH_REQ, NMM_PBCH_CONFIG_REQ, NMM_SIB1_CONFIG_REQ};
            U32* datas[NMM_START_MSG_NUM];
            U32 message_sizes[NMM_START_MSG_NUM] = {sizeof(fapi_nmm_start_desc_t),
            sizeof(fapi_nmm_rssi_meas_req_desc_t),sizeof(fapi_nmm_cell_search_req_desc_t), sizeof(fapi_nmm_pbch_config_req_desc_t), sizeof(fapi_nmm_sib1_config_req_desc_t)};
            U8 *p8;
            STARTREQ* pParam = (STARTREQ*)param;

            fapi_nmm_start_desc_t *nmm_start_desc = (fapi_nmm_start_desc_t *)payload;
            STARTREQ start_params;
            fapi_nmm_rssi_meas_req_desc_t *rssi_req_desc = (fapi_nmm_rssi_meas_req_desc_t *)payload_rssi_req;
            fapi_nmm_cell_search_req_desc_t *cell_search_desc = (fapi_nmm_cell_search_req_desc_t *)payload2;
            fapi_nmm_pbch_config_req_desc_t *pbch_config_desc = (fapi_nmm_pbch_config_req_desc_t *)payload3;
            fapi_nmm_sib1_config_req_desc_t *sib1_config_desc = (fapi_nmm_sib1_config_req_desc_t *)payload4;

            nmm_start_desc->supported_rat = 1; /* lte */
            nmm_start_desc->duplex = 0; /* fdd */
            
            start_params.mode = pParam->mode; /* 0:cpri;1:cpri_simulator */
            start_params.msgType = NMM_START;
            start_params.phyEntityId = 0;
            start_params.count = pParam->count;//50;
            start_params.period = pParam->period;//4:for 20MHz 1AC; 5;
            memcpy(nmm_start_desc->vendor_specific_list, &start_params, sizeof(STARTREQ));
            
            /*RSSI meas req*/
            rssi_req_desc->freqBandIndicator = 7;
            rssi_req_desc->measurementPeriod = 10;
            rssi_req_desc->Nr_earfcnList = 2;
            
            p8 = (U8*)(&(rssi_req_desc->Nr_earfcnList))+1;
            memcpy(p8,rssi_earfcn_list,2*sizeof(U16));
            p8 = (U8*)(&(rssi_req_desc->Nr_earfcnList))+ 2*sizeof(U16) + 1;
            p8[0] = 0; /*nr_vendorSpecificList*/            
            message_sizes[1] = p8 + 1 - (U8 *)rssi_req_desc;
            pToPhyMsg_rssi_req->msgSpecific = message_sizes[1] + sizeof(GENMSGDESC);      

            cell_search_desc->earfcn = 1;
            cell_search_desc->exhaustive_search = FALSE;
            cell_search_desc->nr_pci_list = 1;
            cell_search_desc->pci_list[0] = 1;
            p8 = (U8 *)&cell_search_desc->pci_list[1];
            *p8++ = RB_SLOT_50;
            *p8++ = 1;
            p8[0] = 0; /*nr_vendorSpecificList*/            
            message_sizes[2] = p8 + 1 - (U8 *)cell_search_desc;
            pToPhyMsg2->msgSpecific = message_sizes[2] + sizeof(GENMSGDESC);
            
            pbch_config_desc->pci = 1;
            pbch_config_desc->earfcn = 1;
            pbch_config_desc->nr_vendor_specific_list = 0;
            message_sizes[3] = 5;
            pToPhyMsg3->msgSpecific = message_sizes[3] + sizeof(GENMSGDESC);

            sib1_config_desc->retry_count = 1;
            sib1_config_desc->nr_vendor_specific_list = 0;
            message_sizes[4] = 2;
            pToPhyMsg4->msgSpecific = message_sizes[4] + sizeof(GENMSGDESC);

            datas[0] = (U32*)buffer;
            datas[1] = (U32*)buffer_rssi_req;
            datas[2] = (U32*)buffer2;
            datas[3] = (U32*)buffer3;
            datas[4] = (U32*)buffer4;

            //MacSendToPhyNMM(message_types, datas, message_sizes, NMM_START_MSG_NUM);
            if(0) //for test RSSI meas
            {
                LteMacSendToPhyNMM(message_types, datas, message_sizes, 2);
            }
            else //for test cell search etc.
            {
                for(tmp_i=1;tmp_i<=3;tmp_i++)
                {
                    message_types[tmp_i] = message_types[tmp_i+1];
                    datas[tmp_i]=datas[tmp_i+1];
                    message_sizes[tmp_i]=message_sizes[tmp_i+1];
                }
                LteMacSendToPhyNMM(message_types, datas, message_sizes, 4);

            }

            return MAC_SUCCESS;
        }
        break;        

        case NMM_STOP:
            {
                U8 message_types[1] = {NMM_STOP};
                U32* datas[1];
                U32 message_sizes[1];

                datas[0] = (U32*)buffer;
                message_sizes[0] = sizeof(GENMSGDESC);
                LteMacSendToPhyNMM(message_types, datas, message_sizes, 1);
            }
            break;

        case NMM_CMD:
            {
                U8 *p8;
                STARTREQ start_params;
                fapi_nmm_start_desc_t *nmm_start_desc;
                fapi_nmm_rssi_meas_req_desc_t *rssi_req_desc;
                fapi_nmm_cell_search_req_desc_t *cell_search_desc;
                fapi_nmm_pbch_config_req_desc_t *pbch_config_desc;
                fapi_nmm_sib1_config_req_desc_t *sib1_config_desc;
                fapi_nmm_bcch_config_req_desc_t *bcch_config_desc;
                U8 message_types[1];
                U32* datas[1];
                U32 message_sizes[1];

                switch (nmmCmdType)
                {
                    case 0: // nmm start
                        nmm_start_desc = (fapi_nmm_start_desc_t *)payload;
                        nmm_start_desc->supported_rat = 1; /* lte */
                        nmm_start_desc->duplex = 0; /* fdd */

                        start_params.mode = 4; /* 0:cpri;1:cpri_simulator */
                        start_params.msgType = NMM_START;
                        start_params.phyEntityId = 0;
                        start_params.count = 0;//50;
                        start_params.period = 0;//4:for 20MHz 1AC; 5;
                        memcpy(nmm_start_desc->vendor_specific_list, &start_params, sizeof(STARTREQ));

                        pToPhyMsg->msgSpecific = sizeof(fapi_nmm_start_desc_t) + sizeof(GENMSGDESC);
                        break;

                    case 1: // nmm stop
                        pToPhyMsg->msgSpecific = sizeof(GENMSGDESC);
                        break;
                    
                    case 2: // rssi meas req
                        message_types[0] = NMM_STOP;
                        rssi_req_desc = (fapi_nmm_rssi_meas_req_desc_t *)payload;

                        rssi_req_desc->freqBandIndicator = 7;
                        rssi_req_desc->measurementPeriod = 10;
                        rssi_req_desc->Nr_earfcnList = 2;

                        p8 = (U8*)(&(rssi_req_desc->Nr_earfcnList))+1;
                        memcpy(p8,rssi_earfcn_list,2*sizeof(U16));
                        //p8 = (U8*)(&(rssi_req_desc->Nr_earfcnList))+ 2*sizeof(U16);
                        p8 += 2*sizeof(U16);
                        *p8++ = 0; /*nr_vendorSpecificList*/      
                        pToPhyMsg->msgSpecific = (p8 - (U8 *)rssi_req_desc) + sizeof(GENMSGDESC);
                        break;

                    case 4: // cell search req
                        cell_search_desc = (fapi_nmm_cell_search_req_desc_t *)payload;
                        cell_search_desc->earfcn = 1;
                        cell_search_desc->exhaustive_search = FALSE;
                        cell_search_desc->nr_pci_list = 1;
                        cell_search_desc->pci_list[0] = 1;
                        p8 = (U8 *)&cell_search_desc->pci_list[1];
                        *p8++ = RB_SLOT_50;
                        *p8++ = 1;
                        *p8++ = 0; /*nr_vendorSpecificList*/   
                        pToPhyMsg->msgSpecific = (p8 - (U8 *)cell_search_desc) + sizeof(GENMSGDESC);
                        break;

                    case 6: // pbch config req
                        pbch_config_desc = (fapi_nmm_pbch_config_req_desc_t *)payload;
                        pbch_config_desc->pci = 1;
                        pbch_config_desc->earfcn = 1;
                        pbch_config_desc->nr_vendor_specific_list = 0;
                        pToPhyMsg->msgSpecific = 5 + sizeof(GENMSGDESC);

                        break;

                    case 8: // sib1 config req
                        sib1_config_desc = (fapi_nmm_sib1_config_req_desc_t *)payload;
                        sib1_config_desc->retry_count = 1;
                        sib1_config_desc->nr_vendor_specific_list = 0;
                        pToPhyMsg->msgSpecific = 2 + sizeof(GENMSGDESC);
                        break;

                    case 9: // bcch config req
                        bcch_config_desc = (fapi_nmm_bcch_config_req_desc_t *)payload;
                        bcch_config_desc->nr_siPeriodicityList = 1;                    
                        p8 = (U8 *)(&bcch_config_desc->nr_siPeriodicityList) + 1;
                        memcpy(p8,siPeriodicity_list,sizeof(U16));  //siPeriodicityList
                        p8 += sizeof(U16);
                        *p8++ = 2;   //siWindow
                        *p8++ = 1;   //retry_count
                        *p8++ = 0;   //nr_vendor_specific_list
                        pToPhyMsg->msgSpecific = (p8 - (U8 *)bcch_config_desc) + sizeof(GENMSGDESC);
                        break;
                    
                    case 3: // stop rssi meas req
                    case 5: // stop cell search req
                    case 7: // stop pbch req
                    case 10: // stop bcch req
                        p8 = (U8 *)payload;
                        *p8 = 0;    // nr_vendor_specific_list
                        pToPhyMsg->msgSpecific = sizeof(GENMSGDESC) + 1;
                        break;

            }

                    message_types[0] = pToPhyMsg->msgType;
                    datas[0] = (U32*)buffer;
                    message_sizes[0] = pToPhyMsg->msgSpecific;
                    LteMacSendToPhyNMM(message_types, datas, message_sizes, 1);
        }
        break;

        default:
            DiagPrint(DIAG_LEVEL28, "ERROR", "Invalid NMM PHY message [%d]\n", MessageType);
            break;
    }

    return SUCCESS;
}


static RESULTCODE LteMacSendRxSduIndToHost(PRXSDUIND pRxSduInd)
{// send indication to Host, it will know when to send next PhyTick
    PTR pMacPdu = &pRxSduInd->pRxSdu;
    PMSGHEADER pInd = MsgAllocBuffer(MSG_COMMON);
    U32 Data;
    U32 Len;

    if (pInd)
    {
        pInd->control = CTRL_VERSION;
        // Zero length
        pInd->length = 0;
        pInd->type =  MSGT_INDICATION;
        pInd->srcID = MSGS_MAC;
        pInd->dstID = MSGS_HOST;
        pInd->msgID = MAC_RXSDU_IND;

        Data = (U32)pRxSduInd->frameNum;
        MsgAppendU32(pInd, PAR_PHY_SFN, Data);
        Data = (U32)pRxSduInd->subFrameNum;
        MsgAppendU32(pInd,PAR_PHY_SFCOUNT, Data);
        Data = (U32)pRxSduInd->chanType;
        MsgAppendU32(pInd,PAR_PHY_CH_TYPE, Data);
        Data = (U32)pRxSduInd->channelId;
        MsgAppendU32(pInd,PAR_PHY_CH_IDX, Data);
        Data = (U32)pRxSduInd->mErrAvgCh[0];
        MsgAppendU32(pInd, PAR_M_ERR_AVG_CH_0, Data);
        Data = (U32)pRxSduInd->mErrAvgCh[1];
        MsgAppendU32(pInd, PAR_M_ERR_AVG_CH_1, Data);
        Data = (U32)pRxSduInd->mErrExpo;
        MsgAppendU32(pInd, PAR_M_ERR_EXPO, Data);
        Data = (U32)pRxSduInd->timingAdv;
        MsgAppendU32(pInd, PAR_TIMING_ADV, Data);

        if(pRxSduInd->chanType == PUCCH)
            Len = ((pRxSduInd->numBitsRx & 0x00FF) + 7) >> 3;
        else
            Len = (pRxSduInd->numBitsRx + 7) >> 3;
        MsgAppendParam(pInd, PAR_BIN_DATA,  Len, pMacPdu);
        MsgDispatch(pInd->srcID, pInd->length + sizeof(MSGHEADER), pInd);
    }
    return SUCCESS;
}

static RESULTCODE LteMacProcessRxSdu(PRXSDUIND pRxSduInd, PTR ptrCtx)
{
    PTR pMacPdu = NULL;
    UE_DESC* ue_ctx = (UE_DESC*)ptrCtx;
    U32 Put , ChanId;
    PTIMADVERRINF pBuf;
    U32 SF = getSubFrameNum();
    U32 Idx;

    lte_harq_proc_rxsdu(pRxSduInd);

    testConfConfig.TimAdvCmdMap[SF] = (U8)pRxSduInd->timingAdv;

    Idx = pRxSduInd->chanType - PUSCH;
    testConfConfig.timAdvErrInfo[Idx].mErrAvgCh[0] = pRxSduInd->mErrAvgCh[0];
    testConfConfig.timAdvErrInfo[Idx].mErrAvgCh[1] = pRxSduInd->mErrAvgCh[1];
    testConfConfig.timAdvErrInfo[Idx].mErrExpo = pRxSduInd->mErrExpo;
    testConfConfig.timAdvErrInfo[Idx].mSetDefaults = 0;

    ChanId = pRxSduInd->channelId;
    Put = testConfConfig.timAdvErrInfoArr_put[ChanId];
    pBuf = &testConfConfig.timAdvErrInfoArrFSF[Put][ChanId];

    pBuf->mErrAvgCh[0] = pRxSduInd->mErrAvgCh[0];
    pBuf->mErrAvgCh[1] = pRxSduInd->mErrAvgCh[1];
    pBuf->mErrExpo = pRxSduInd->mErrExpo;
    pBuf->mSetDefaults = 0;
    if(++Put >= TIMERRBUFDEPTH)
        Put = 0;
    testConfConfig.timAdvErrInfoArr_put[ChanId] = Put;

#ifndef AC_MSPD_TDD

    if((pRxSduInd->chanType == PUCCH) && (pRxSduInd->pucchDetected == 0))
    {// it's DEAD_ZONE pucch, trash it for TestMAC
        return SUCCESS;
    }
#else
#endif
    pMacPdu = &pRxSduInd->pRxSdu;

    LteMacSendRxSduIndToHost(pRxSduInd);

    // FIXME: should be use PHY status (not implemented yet from PHY)

    if (pBsMacDesc->schedulingSchema != RCT_SCHEMA)
    {
        if (pRxSduInd->chanType == PUSCH ||
            pRxSduInd->chanType == PDSCH)
        {
            LteMacPduDecoder(pMacPdu, (pRxSduInd->numBitsRx+7) >> 3, ue_ctx);
        }
    }

    return SUCCESS;
}

/** Process queue containing RX SDUs
 *
 * @param pDlSubFrame Pointer to the current Downlink vector
 * @param pUlSubFrame Pointer to the current Uplink vector
 */
static void MacProcRxSduQueue(PDLSUBFRDESC pDlSubFrame, PULSUBFRDESC pUlSubFrame)
{
    PGENMSGDESC pMsgHdr;
    PRXSDUIND pRxSduInd;
    U32 FrameNum;
    U32 SubFrameNum;

    if (PhySduQGet == PhySduQPut)
        return;

    pMsgHdr = (PGENMSGDESC) PhySduQ[PhySduQGet];

    if (++PhySduQGet >= PhySduQ_SIZE)
        PhySduQGet = 0;

    pRxSduInd = (PRXSDUIND) pMsgHdr;

    FrameNum = pRxSduInd->frameNum;
    SubFrameNum = pRxSduInd->subFrameNum;

#if defined (USE_LINUX)
    if(testConfConfig.ULEnabled)   
        pHiDciApiSet(pDlSubFrame, pUlSubFrame, pRxSduInd);
#endif

    MsgFreeBuffer((PMSGHEADER)pMsgHdr);


    // TODO: just drain rest of RxSdu-s for now
    while (PhySduQGet != PhySduQPut)
    {
        pMsgHdr = (PGENMSGDESC) PhySduQ[PhySduQGet];

        if(pMsgHdr)
        {
            pRxSduInd = (PRXSDUIND) pMsgHdr;

            if((pRxSduInd->frameNum != FrameNum) || (pRxSduInd->subFrameNum != SubFrameNum))
                break;

            MsgFreeBuffer((PMSGHEADER)pMsgHdr);
        }

        if (++PhySduQGet >= PhySduQ_SIZE)
            PhySduQGet = 0;
    }
}

/** Process API message received from PHY
 *
 * @param pMsgHdr Pointer to the message buffer
 * @param pDlSubFrame Pointer to the current Downlink vector
 * @param pUlSubFrame Pointer to the current Uplink vector
 * @param ptrCtx Not used
 * @return Standard result code
 */
RESULTCODE LteMacProcessFromPhyMessage(PGENMSGDESC pMsgHdr,
                                       PDLSUBFRDESC pDlSubFrame,
                                       PULSUBFRDESC pUlSubFrame,
                                       PTR ptrCtx)
{
    switch (pMsgHdr->msgType)
    {
        case PHY_TXSTART_CONF:
            // This is from PHY confirming the receipt of TxVector
            // Need to follow up with PHY_TXSDU_REQ
            // Assume this always received as a responce to TXSTART_REQ
            // TODO: Update state machine
            LteMacSendToPhyMessage(PHY_TXSDU_REQ, pDlSubFrame, 1);
            LteMacSendPhyReadyInd();
            break;

        case PHY_TXSTART_IND:
            // This is from PHY indicating beginning of transmission
            // TODO: Update state machine, nothing to send
            break;

        case PHY_TXSDU_CONF:
            // This is from PHY indicating receipt of last SDU
            // Need to send next SDU if there's any left
            // TODO: Update state machine
            break;

        case PHY_TXEND_IND:
            // This is from PHY indicating ending of transmission
            // TODO: Update state machine, nothing to send
            break;

        case PHY_RXSTATUS_IND:
        {
            PRXSTATUSIND pRxStatusInd = (PRXSTATUSIND)pMsgHdr;
            PMSGHEADER pInd = MsgAllocBuffer(MSG_COMMON);
            U32 Data;
            U32 DataLen;

            if (pInd)
            {
                pInd->control = CTRL_VERSION;
                // Zero length
                pInd->length = 0;
                pInd->type =  MSGT_INDICATION;
                pInd->srcID = MSGS_MAC;
                pInd->dstID = MSGS_HOST;
                pInd->msgID = MAC_RXSTATUS_IND;

                Data = (U32)pRxStatusInd->frameNum;
                MsgAppendU32(pInd, PAR_PHY_SFN, Data);
                Data = (U32)pRxStatusInd->subFrameNum;
                MsgAppendU32(pInd, PAR_PHY_SFCOUNT, Data);

                if(pRxStatusInd->statusType == PRACH_RESULT)
                {
                    Data = PRACH;
                    DataLen = pRxStatusInd->eventNumber * sizeof(RX_PRACH_STATUS_EVENT);
                }
                else if(pRxStatusInd->statusType == CQIRIHI_RESULT)
                {
                    PRX_CQIRIHI_STATUS_EVENT pMsg1= (PRX_CQIRIHI_STATUS_EVENT)&pRxStatusInd->pEventList;


                    Data = CQIRIHI_RESULT;
                    DataLen = pRxStatusInd->eventNumber * sizeof(RX_CQIRIHI_STATUS_EVENT) + pMsg1->status_len;

                    lte_pusch_update_cqi_pmi_ri_harq(pMsg1, pRxStatusInd);
                }
                else if(pRxStatusInd->statusType == SRS_RESULT)
                {
                    Data = SRS_RESULT;
                    DataLen = pRxStatusInd->eventNumber * (sizeof(RX_SRS_STATUS_EVENT) - sizeof(U8*)) + pRxStatusInd->msgSpecific;
                }

                MsgAppendU32(pInd, PAR_PHY_CH_TYPE, Data);
                MsgAppendParam(pInd, PAR_BIN_DATA, DataLen, &pRxStatusInd->pEventList);

                MsgDispatch(pInd->srcID, pInd->length + sizeof(MSGHEADER), pInd);
            }
        }
    break;

        case PHY_RXSTART_CONF:
            // This is from PHY indicating receipt of RxVector
            // TODO: Update state machine, nothing to send
#ifdef AC_MSPD_TDD
            // TODO: if last SDU
            LteMacSendPhyReadyInd();
#endif
            break;

        case PHY_RXSTART_IND:
            //printf("PHY_RXSTART_IND\n");
            // This is from PHY indicating start of reception
            // TODO: Update state machine, nothing to send
            break;

        case PHY_RXSDU_IND:
            LteMacProcessRxSdu((PRXSDUIND)pMsgHdr, ptrCtx);
            break;
        case PHY_RXEND_IND:
            // This is from PHY indicating end of reception
            // TODO: Update state machine, nothing to send
            break;

        case PHY_INIT_IND:
            // TODO: Update state machine, we are ready to work
            break;

        case PHY_START_CONF:
            break;

        case PHY_STOP_CONF:
            break;

        case NMM_RSSI_MEAS_IND:
        case NMM_RSSI_MEAS_CNF:
        case NMM_CELL_SEARCH_IND:
        case NMM_CELL_SEARCH_CNF:
        case NMM_PBCH_CONFIG_CNF:
        case NMM_PBCH_DATA_IND:
        case NMM_SIB1_CONFIG_CNF:
        case NMM_BCCH_CONFIG_CNF:
        case NMM_BCCH_DATA_IND:
            if(MacRtMode == MACMODE_NRT)
            {// send indication to Host, it will know when to send next DLStart
                PMSGHEADER pInd = MsgAllocBuffer(MSG_COMMON);
                if (pInd)
                {
                    PMSGIND pPhyInd = (PMSGIND) pMsgHdr;
                    pInd->control = CTRL_VERSION;
                    // Zero length
                    pInd->length = 0;
                    pInd->type =  MSGT_ANSWER;
                    pInd->srcID = MSGS_LARM | MSGS_NMM;
                    pInd->dstID = MSGS_HOST;
                    pInd->msgID = pMsgHdr->msgType;
                    MsgAppendParam(pInd, PAR_BIN_DATA, pPhyInd->reserved, pPhyInd);
                    MsgDispatch(pInd->srcID, pInd->length + sizeof(MSGHEADER), pInd);
                }
            }
            break;

        default:
            printf("ups... %x\n",pMsgHdr->msgType );
            DiagPrint(DIAG_LEVEL6, "WARNING", "Unknown message [%s,%d]\n",
                           __FUNCTION__, __LINE__);
            break;
    }

    return SUCCESS;
}

static RESULTCODE LteMacProcessRxSduRt(PRXSDUIND pRxSduInd, PTR ptrCtx)
{
    PTR pMacPdu = NULL;
    UE_DESC* ue_ctx = (UE_DESC*)ptrCtx;
    U32 NextSF = getSubFrameNum();
    U32 Idx, ChanId, SduLen;
    U32 Put;
    PTIMADVERRINF pBuf;

    lte_harq_proc_rxsdu(pRxSduInd);

    if(++NextSF >= 10)
         NextSF = 0;

    testConfConfig.TimAdvCmdMap[NextSF] = (U8)pRxSduInd->timingAdv;

    Idx = pRxSduInd->chanType - PUSCH;
    testConfConfig.timAdvErrInfo[Idx].mErrAvgCh[0] = pRxSduInd->mErrAvgCh[0];
    testConfConfig.timAdvErrInfo[Idx].mErrAvgCh[1] = pRxSduInd->mErrAvgCh[1];
    testConfConfig.timAdvErrInfo[Idx].mErrExpo = pRxSduInd->mErrExpo;
    testConfConfig.timAdvErrInfo[Idx].mSetDefaults = 0;

    ChanId = pRxSduInd->channelId;
    Put = testConfConfig.timAdvErrInfoArr_put[ChanId];
    pBuf = &testConfConfig.timAdvErrInfoArrFSF[Put][ChanId];

    pBuf->mErrAvgCh[0] = pRxSduInd->mErrAvgCh[0];
    pBuf->mErrAvgCh[1] = pRxSduInd->mErrAvgCh[1];
    pBuf->mErrExpo = pRxSduInd->mErrExpo;
    pBuf->mSetDefaults = 0;
    if(++Put >= TIMERRBUFDEPTH)
        Put = 0;
     testConfConfig.timAdvErrInfoArr_put[ChanId] = Put;

        //uart_printf("Got TimAdv: NextSF=%d val=0x%X\r\n", NextSF, pRxSduInd->timingAdv);
#ifndef AC_MSPD_TDD

    if((pRxSduInd->chanType == PUCCH) && (pRxSduInd->pucchDetected == 0))
    {// it's DEAD_ZONE pucch, trash it for TestMAC
        return SUCCESS;
    }
#else
#endif
    pMacPdu = &pRxSduInd->pRxSdu;
    
    {
        U8 *ptr = pMacPdu;
        U32 numBytes = (pRxSduInd->numBitsRx+7)>>3;

      if (pRxSduInd->subFrameNum != 0 )// skip SF# 0 with PRACH
          if (ptr[0] != 172 ||
              ptr[1] != 188 ||
              ptr[2] != 210 ||
              ptr[3] != 17 ||
              ptr[4] != 77 ||
              ptr[numBytes - 5] != 26 ||
              ptr[numBytes - 4] != 213 ||
              ptr[numBytes - 3] != 196 ||
              ptr[numBytes - 2] != 71 ||
              ptr[numBytes - 1] != 93)
                  errorCount++;

      sduCount++;
      }
    if (pBsMacDesc->PhyAutoCtx.RxSduEna == TRUE)
    {
        SduLen = ((pRxSduInd->numBitsRx + 7 ) >> 3) + sizeof(RXSDUIND);
        if( MacDrainRxSduModeOn() && RxSduSet + SduLen >= 12 * 1024 )
        {
            DrainRxSduBufToHost();
        }
        AddToBuf((U8*)pRxSduInd, ((pRxSduInd->numBitsRx + 7 ) >> 3) + sizeof(RXSDUIND));
    }
    if (pBsMacDesc->schedulingSchema != RCT_SCHEMA)
    {
        if (pRxSduInd->chanType == PUSCH ||
            pRxSduInd->chanType == PDSCH)
            LteMacPduDecoder(pMacPdu, (pRxSduInd->numBitsRx+7) >> 3, ue_ctx);
    }

    if(!testConfConfig.DontUsePuschRxSduForHiAndDci0)
    {
        if(pRxSduInd->chanType == PUSCH)
        {
            PMSGHEADER  pCpyRxSduInd= (PMSGHEADER) MsgAllocBuffer(MSG_COMMON);
            if (pCpyRxSduInd == NULL)
            {
                return FAILURE;
            }

            memcpy(pCpyRxSduInd, pRxSduInd, sizeof(RXSDUIND) + ((pRxSduInd->numBitsRx+7) >> 3));
            PhySduQ[PhySduQPut] = (U32)pCpyRxSduInd;

            if (++PhySduQPut >= PhySduQ_SIZE)
                PhySduQPut = 0;
            return SUCCESS;
        }
    }

    return SUCCESS;

}

RESULTCODE LteMacProcessFromPhyMessageRt(PGENMSGDESC pMsgHdr,
                                       PDLSUBFRDESC pDlSubFrame,
                                       PULSUBFRDESC pUlSubFrame,
                                       PTR ptrCtx)
{
    PRXSDUIND pRxSduInd = (PRXSDUIND)pMsgHdr;

    switch (pMsgHdr->msgType)
    {
        case PHY_TXSTART_CONF:

            // This is from PHY confirming the receipt of TxVector
            // Need to follow up with PHY_TXSDU_REQ
            // Assume this always received as a responce to TXSTART_REQ
            // TODO: Update state machine
            // LteMacSendToPhyMessage(PHY_TXSDU_REQ, pDlSubFrame);
            break;
        case PHY_TXSTART_IND:
            {
                if (!gMac2PhyDirectFIFOUsed)
                {
                    if (gPhyStopReq == PhyAutoStop)
                    {
                        LteMac2PhyApiListFree();
                        gPhyStopReq = 0;
                        LteMacSendToPhyMessage(PHY_STOP_REQ, NULL, 1);
                    }
                    else if (gPhyStopReq == PhyAutoShutdown)
                    {
                        LteMac2PhyApiListFree();
                        gPhyStopReq = 0;
                        printf("send PHY_SHUTDOWN_REQ after set flag\n");
                        LteMacSendToPhyMessage(PHY_SHUTDOWN_REQ, NULL, 1);

                        printf("repeat PHY_SHUTDOWN_REQ\n");
                        LteMacSendToPhyMessage(PHY_SHUTDOWN_REQ, NULL, 1);
                    }
                    else
                    {
                        LteMacSendToPhyMessage(PHY_RXSTART_REQ, pUlSubFrame, 1);
                    }
                }

                globalTTITickCount++;
                setFrameSubFrameNum();
                setTxFrameSubFrameNum();

                if (globalTTITickCount % 5000 == 0)
                    printf("sf# %d\n",globalTTITickCount);
                LteBsMacDlScheduler(pBsMacDesc, pDlSubFrame);

                global_chanIdx = pDlSubFrame->numberofChannelDescriptors;
                LteBsMacUlScheduler(pBsMacDesc, pUlSubFrame);

                MacProcRxSduQueue(pDlSubFrame, pUlSubFrame);

                LteMacSendToPhyMessage(PHY_TXSTART_REQ, pDlSubFrame, 0);
                LteMacSendToPhyMessage(PHY_TXSDU_REQ, pDlSubFrame, 0);

                if (gMac2PhyDirectFIFOUsed)
                {
                    if (gPhyStopReq == PhyAutoStop)
                    {
                        LteMac2PhyApiListFree();
                        gPhyStopReq = 0;
                        LteMacSendToPhyMessage(PHY_STOP_REQ, NULL, 1);
                    }
                    else if (gPhyStopReq == PhyAutoShutdown)
                    {
                        LteMac2PhyApiListFree();
                        gPhyStopReq = 0;
                        printf("send PHY_SHUTDOWN_REQ after set flag\n");
                        LteMacSendToPhyMessage(PHY_SHUTDOWN_REQ, NULL, 1);
                    }
                    else
                    {
                        LteMacSendToPhyMessage(PHY_RXSTART_REQ, pUlSubFrame, 1);
                    }
                }

//                MLogTask(PID_LINUX_PHY_TXSTART_IND, RESOURCE_LINUX0, tt, Timer0GetCntr());
            }
            break;
        case PHY_TXSDU_CONF:
            // This is from PHY indicating receipt of last SDU
            // Need to send next SDU if there's any left
            // TODO: Update state machine
            break;

        case PHY_TXEND_IND:
            // This is from PHY indicating ending of transmission
            // TODO: Update state machine, nothing to send
            break;

        case PHY_RXSTART_CONF:
            // This is from PHY indicating receipt of RxVector
            // TODO: Update state machine, nothing to send
            break;


        case PHY_RXSTATUS_IND:
            {
                PRXSTATUSIND pRxStatusInd = (PRXSTATUSIND)pMsgHdr;

                if (pRxSduInd->subFrameNum == 0 )// skip SF# 0 with PRACH
                {
                    PRX_PRACH_STATUS_EVENT pPrachEvents =
                        (PRX_PRACH_STATUS_EVENT)&pRxStatusInd->pEventList;
                    if (pRxStatusInd->eventNumber != 1 ||
                        pPrachEvents->prachPreambleId != 17 ||
                        pPrachEvents->timingOffset != 0)
                    {
                        prachErrors++;
                    }

                    prachSdus++;
                }

                if (pBsMacDesc->PhyAutoCtx.RxSduEna == TRUE)
                {
                    U32 RxSduSize   = sizeof(RXSTATUSIND);
                    //U32 HeadersSize = sizeof (MSGHEADER) + 1 * sizeof (PARAMHEADER);
    
                    if(pRxStatusInd->statusType == PRACH_RESULT)
                    {
                        print_dbg("received prach");
                        RxSduSize += pRxStatusInd->eventNumber * sizeof(RX_PRACH_STATUS_EVENT);
                    }
                    else if(pRxStatusInd->statusType == CQIRIHI_RESULT)
                    {
                        PRX_CQIRIHI_STATUS_EVENT pMsg1= (PRX_CQIRIHI_STATUS_EVENT)&pRxStatusInd->pEventList;

                        print_dbg("received cqi");
                        RxSduSize += pRxStatusInd->eventNumber * sizeof(RX_CQIRIHI_STATUS_EVENT) + pMsg1->status_len;
                    }
                    else
                    {
                        RxSduSize += pRxStatusInd->eventNumber * (sizeof(RX_SRS_STATUS_EVENT) + (pRxStatusInd->msgSpecific - sizeof(U8*)));
                    }
                   
                    AddToBuf((U8*)pRxSduInd, RxSduSize );
                }                
            }
            break;
        case PHY_RXSTART_IND:
            // This is from PHY indicating start of reception
            // TODO: Update state machine, nothing to send
            break;

        case PHY_RXSDU_IND:
            LteMacProcessRxSduRt((PRXSDUIND)pMsgHdr, ptrCtx);
            break;
        case PHY_RXEND_IND:
            // This is from PHY indicating end of reception
            // TODO: Update state machine, nothing to send
            break;

        case PHY_INIT_IND:
            {
                PBS_STATIC_CONFIG pStaticConfig = get_static_config();
                gEnaStopFlag = 1;

                
                if (pStaticConfig->phy_on_linux_rt)
                {
//                    if (pStaticConfig->flag_phy_ena)
                    {
                        PMSGHEADER msg = MsgAllocBuffer(MSG_COMMON);

//                        pStaticConfig->flag_phy_ena = 0;
                        
                        if (msg != NULL)
                        {
                            msg->control = CTRL_VERSION;
                            msg->length  = 0;
                            msg->type    = MSGT_ANSWER;
                            msg->srcID   = MSGS_SVSR;
                            msg->dstID   = MSGS_HOST;
                            msg->msgID   = SVSR_CREATEPHY;
                        
                            MsgResult(msg, SUCCESS);
                            MsgAppendU32(msg, PAR_INSTID, MSGS_LARM | (pStaticConfig->phy_instance_id));
                            print_log("send ack to host from PHY_INIT_IND");
                            MsgDispatch(MSGS_SVSR, msg->length + sizeof(MSGHEADER), msg);
                        }
                    }            
                }           
            }
            break;

        case PHY_START_CONF:
            LteBsMacDlScheduler(pBsMacDesc, pDlSubFrame);
            LteBsMacUlScheduler(pBsMacDesc, pUlSubFrame);

            if (gMac2PhyDirectFIFOUsed)// && globalTTITickCount)
            {
                LteMacSendToPhyMessage(PHY_TXSTART_REQ, pDlSubFrame, 0);
                LteMacSendToPhyMessage(PHY_TXSDU_REQ, pDlSubFrame, 0);
                LteMacSendToPhyMessage(PHY_RXSTART_REQ, pUlSubFrame, 1);
            }

/*
//#ifdef LOWLATENCY
//            if(gMac2PhyDirectFIFOUsed && globalTTITickCount)
            {
// PHY_TXSTART_REQ was created into PHY_START_REQ-case
                LteMacSendToPhyMessage(PHY_RXSTART_REQ, pUlSubFrame, 1);
            }
//#endif*/
            break;

        case PHY_STOP_CONF:
            break;
        case PHY_STOP_IND:
            pBsMacDesc->PhyAutoCtx.RunEna = FALSE;
            LteMac2PhyApiListFree();

            if(pBsMacDesc->PhyAutoCtx.RxSduEna == TRUE)
            {
                if( MacDrainRxSduModeOn() )
                {
                    print_log("send drain to host. sdu size is RxSduSet = %d", RxSduSet);
                    DrainRxSduBufToHost();
                }
                else
                {
                    print_log("send buf to host. sdu size is RxSduSet = %d", RxSduSet);                
                    SendBufToHost();
                }

                OsalFreeHeap(&OsalGlobalHeap, pRxSduBuf);
                RxSduSet = 0;
                RxSizeRest = RxBufSize;
            }
       /*     if (gEnaStopFlag)
            {
                gEnaStopFlag = 0;
                LteMacSendToPhyMessage(PHY_STOP_REQ, pUlSubFrame, 1); // try to stop Phy
            }*/
            MsgIndication(MSGS_MAC,
                MSGS_HOST,
                PHY_LTE_STOP,
                0);
            break;
        case PHY_ERROR_IND:
            LteMac2PhyApiListFree();
            gPhyStopReq = 0;
            printf("send PHY_SHUTDOWN_REQ after received PHY_ERROR_IND\n");
            LteMacSendToPhyMessage(PHY_SHUTDOWN_REQ, NULL, 1);
            break;
            
        default:
            if(pMsgHdr->msgType >= NMM_START &&
                pMsgHdr->msgType < NMM_final_to_be_updated)
            {
                printf("ProcRt: have NMM msg=%d\n", pMsgHdr->msgType);
            }
            else
            {
                DiagPrint(DIAG_LEVEL6, "WARNING", "Unknown message type %d [%s,%d]\n",
                      __FUNCTION__, __LINE__, pMsgHdr->msgType);
            }
            break;
    }
    return SUCCESS;
}

/** Frees internal structures related to the MAC-PHY communication
 *
 * @param pMac2Phy Pointer to the communication context
 * @return Standard result code
 */
RESULTCODE LtePhyInterfaceClean(Mac2PhyInterface *pMac2Phy)
{
    pMac2Phy->txCb = NULL;
    pMac2Phy->rxCb = NULL;
    pMac2Phy->toPhy = NULL;
    pMac2Phy->fromPhy = NULL;

    // TODO: clean SDU in the dlControlTxSduQueue
    OsalDestroyQueue(&dlControlTxSduQueue);
    OsalFreeHeap(&OsalGlobalHeap, dlControlTxSduQueue.storage);

    return SUCCESS;
}

/** Initializes internal structures related to the MAC-PHY communication
 *
 * @param pMac2Phy Pointer to the communication context
 * @param txCb Callback function used to transmit API messages to PHY
 * @param rxCb Callback function used to receive messages from PHY
 * @param toPhy Application-defined parameter associated with the txCb
 * @param fromPhy Application-defined parameter associated with the rxCb
 * @return Standard result code
 */
RESULTCODE LtePhyInterfaceInit(Mac2PhyInterface *pMac2Phy,
                               PhyTxCb txCb,
                               PhyRxCb rxCb,
                               PTR toPhy,
                               PTR fromPhy)
{
    OSAL_PADDRESS pMem = NULL;

    /* Tx Ue Queue  */
    if (OsalAllocHeap(&OsalGlobalHeap,
                      &pMem,
                      DLTXSDU_QUEUE_SIZE * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }

    if (OsalCreateQueue(&dlControlTxSduQueue,
                        pMem,
                        DLTXSDU_QUEUE_SIZE * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }


    if (OsalAllocHeap(&OsalGlobalHeap,
                      &pMem,
                      DLTXSDU_QUEUE_SIZE * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
         printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
         return FAILURE;
    }

    if (OsalCreateQueue(&dlControlTxSduQueue2,
                        pMem,
                        DLTXSDU_QUEUE_SIZE * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
         printf("ERROR: No memory [%s,%s:%d]\n", __FUNCTION__, __FILE__, __LINE__);
         return FAILURE;
    }

    // Init PHY interface
    pMac2Phy->txCb = txCb;
    pMac2Phy->rxCb = rxCb;
    pMac2Phy->toPhy = toPhy;
    pMac2Phy->fromPhy = fromPhy;

    return SUCCESS;
}

/** Initialize PHY parameters to default values
 *  Will be used in PHY_INIT.req message
 *
 * @param pParm Pointer to preallocated structure holding PHY initialization values
 * @return Standard result code
 */
/*
RESULTCODE LtePhyIfInitDefault(PINITPARM pParm)
{
    pParm->channelBandwidth = BW_10_0_MHZ;
    pParm->frtypeDuplexmode = FDD;
    pParm->radioAccessMode = OFDMA;
    pParm->physicalResourceBandwidth = PRB_12;
    pParm->numberResourceBlocksperSlot = RB_SLOT_50;
    pParm->phyLayerCellIdGroup = 0;
    pParm->phyLayerId = 0;
    pParm->txAntennaPortCount = TXANT_ONE;
    pParm->rxAntennaPortCount = RXANT_ONE;
    pParm->txStartConfDisable = 1;
    pParm->rxStartConfDisable = 1;
    pParm->fastfwdorPreclockingNumber = 1600;
    pParm->fftSize = FFTSZ_1024;
    pParm->numberUsedSubcarriers = NUSC_600;
    pParm->nMaxDlRb = NMAXRB_50;
    pParm->nMaxUlRb = NMAXRB_50;
    pParm->referenceSignalPower = 250;
    pParm->primarySyncSignalPower = 65;
    pParm->secondarySyncSignalPower = 65;
    pParm->numDataRePerPRB = 80;
    pParm->cyclicPrefixType = NORMAL;
    pParm->txSduConfEnable = 0;
    pParm->radioFrameNumber = 0;
    pParm->subframeNumber = 0;
    pParm->slotNumber = 0;
    pParm->srsBandwidthConfig = 0;
    pParm->srsSubframeConfig = 15;      // SRS disabled
    pParm->srsSimultaneousANandSRS = 0;

    pParm->prachConfigurationIndex = 0;
    pParm->prachFrequencyOffset = 0;
    pParm->prachHighSpeedFlag = 0;
    pParm->prachCyclicShiftConfig = 0;
    pParm->prachRootSequenceIndex = 0;

    pParm->rxAntennaPortCount = 1;
    pParm->txAntennaPortCount = 1;

    return SUCCESS;
}*/


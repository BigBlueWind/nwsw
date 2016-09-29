//-------------------------------------------------------------------------------------------
/** @file LteBsApiErrorCheck.h
 *
 * @brief Api Error Check for robustness
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT&copy; 2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#ifndef LTEBSAPIERRORCHECK_H
#define LTEBSAPIERRORCHECK_H

#include "ltephydef.h"
#include "ltephy.h"
#include "LteBsTxDataSdu.h"

//#define LTE_BS_API_ERROR_CHECK_ROBUSTNESS_ENABLED       // If Disabled, we will stop the Phy

#ifdef CEVA_INTRINSICS
#define CEVAEXTERN1          __attribute__ ((section (".CSECT cevaboot")))
#else
#define CEVAEXTERN1
#endif

#define LTE_BS_API_ERROR_CHECK_TX_SFN_ERROR             1<<0
#define LTE_BS_API_ERROR_CHECK_RX_SFN_ERROR             1<<1
#define LTE_BS_API_ERROR_CHECK_INVALID_NUM_API          1<<2
#define LTE_BS_API_ERROR_CHECK_TXVEC_DCI_OFFSET_ERR     1<<3
#define LTE_BS_API_ERROR_CHECK_TXVEC_INVALID_CH_ID      1<<4
#define LTE_BS_API_ERROR_CHECK_TXSDU_INVALID_CH_ID      1<<5
#define LTE_BS_API_ERROR_CHECK_TXSDU_INVALID_CH_TYPE    1<<6
#define LTE_BS_API_ERROR_CHECK_TXVEC_INVALID_QM         1<<7
#define LTE_BS_API_ERROR_CHECK_TXVEC_INVALID_NUM_ENTR   1<<8
#define LTE_BS_API_ERROR_CHECK_TXVEC_ZERO_RBS           1<<9
#define LTE_BS_API_ERROR_CHECK_HISDU_NUM_ERR            1<<10
#define LTE_BS_API_ERROR_CHECK_DCISDU_NUM_ERR           1<<11
#define LTE_BS_API_ERROR_CHECK_INTERNAL_BCH_ERR         1<<12


#define LTE_BS_API_ERROR_CHECK_SUCCESS                  (0<<0)
#define LTE_BS_API_ERROR_CHECK_DEFAULT_RET              (1<<0)
#define LTE_BS_API_ERROR_CHECK_DUPLICATE_SFN            (1<<1)
#define LTE_BS_API_ERROR_CHECK_SFN_CHECK_FAIL           (1<<2)
#define LTE_BS_API_ERROR_CHECK_EMPTY_FIFO               (1<<3)



typedef struct tLteBsApiErrorCheck
{
    UINT32 numTxVector;
    UINT32 numRxVector;
    UINT32 numHARQVector;

    UINT32 numPdschSdu;
    UINT32 numPdcchSdu;
    UINT32 numPbchSdu;
    UINT32 numPhichSdu;
    UINT32 numDciSdu;

    UINT32 lenTxVector;
    UINT32 lenRxVector;

    PDLSUBFRDESC pTxVector;
    PULSUBFRDESC pRxVector;

    PHIADCIULMSGDESC pHiDciMsg;
    PHIINFOMSGDESC   pHichSdu[32];
    PDCIULSDUMSG     pDciSdu[32];

    PTXSDUREQ pPdschSdu[32];
    PTXSDUREQ pPdcchSdu[32];
    PTXSDUREQ pBchSdu;
}LTE_BS_API_ERROR_CHECK, *PLTE_BS_API_ERROR_CHECK;

typedef struct tLteBsApiErrorCheckContext
{
    UINT32 errorCheckEnabled;
    UINT32 lastTxSfn;
    UINT32 lastRxSfn;
    UINT32 apiSfn[10];

    PLTE_BS_API_ERROR_CHECK pLteBsApiErrorCheck;
    PMAC2PHY_QUEUE_EL pApiList;
    UINT32 robustControlEnabled;
    UINT16 isDummy;
    UINT16 isInternalBch;
    UINT32 successiveNoFifo;

    PUINT32 pgFirstMessageArrived;
    PUINT32 pgLastTxSfn, pgLastRxSfn;
    PUINT32 pgTTIPauseResumeCount;

    PLTE_PHY_SDU_DESC pSduDesc;

    PMAC2PHY_QUEUE_EL pTxMsg;
    PGENMSGDESC       pTxHeader;
    PDLSUBFRDESC      pEmptyTxVector;
    PMAC2PHY_QUEUE_EL pRxMsg;
    PGENMSGDESC       pRxHeader;
    PULSUBFRDESC      pEmptyRxVector;
} LTE_BS_API_ERROR_CHECK_CTX, *PLTE_BS_API_ERROR_CHECK_CTX;

PLTE_BS_API_ERROR_CHECK_CTX LteBsApiErrorCheckGetCtx(void);

UINT32 LteBsApiErrorCheckEnable(void);
UINT32 LteBsApiErrorCheckDisable(void);

PMAC2PHY_QUEUE_EL LteBsPhyStatsGetApi(UINT32 *pIsDummy);
UINT32 LtePhyApiErrorCheckInitEmptyChannels(PLTEPHYDESC pPhy);
UINT32 LtePhyApiErrorCheckDestroyEmptyChannels(void);
UINT32 LteBsApiErrorCheckDispatch(PMAC2PHY_QUEUE_EL pMsg, UINT32 isDummyRunning, UINT32 flag);
UINT32 LteBsApiErrorCheckSfn(PLTE_BS_API_ERROR_CHECK_CTX pLteBsApiErrorCheckCtx, UINT32 currSfn, UINT32 direction);
void LteBsApiErrorCheck_CB(void* ptr, UINT32 ceva_ret);


PMAC2PHY_QUEUE_EL LtePhyApiErrorCheckUseEmptyChannels(PLTE_BS_API_ERROR_CHECK_CTX pLteBsApiErrorCheckCtx) CEVAEXTERN1;
UINT32 LteBsApiErrorCheckSortTxSdu(PLTE_BS_API_ERROR_CHECK pLteBsApiErrorCheck, PTXSDUREQ *pTxSduReqStore, UINT32 numTxSdu) CEVAEXTERN1;

UINT32 LteBsApiErrorCheckPushApiState(UINT32 numApi, UINT32 numTxSdu, PDLSUBFRDESC pTxVector);
UINT32 LteBsApiErrorCheckScanApi(PMAC2PHY_QUEUE_EL pListHeader, UINT32* pNumApi, UINT32* pNumTxSdu, PDLSUBFRDESC* ppTxVector);

#endif  // LTEBSAPIERRORCHECK_H


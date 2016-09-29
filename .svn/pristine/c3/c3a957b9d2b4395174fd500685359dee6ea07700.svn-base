//-------------------------------------------------------------------------------------------
/** @file LteBsRxDataSduThread.h
 *
 * @brief This file implements the Rx SDU (Decoder) processing for the LTE BS
 * @author Mindspeed Technologies
 * @version $Revision: 1.22 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#ifndef LTEBSRXDATASDU_H
#define LTEBSRXDATASDU_H

#include "ltephy.h"
#include "ltephydef.h"
#include "phydi.h"
#include "FECdrv.h"
#include "mdmadrv.h"
#include "LteBsPhyStats.h"


#define MUX_PUSCH_ACK_NACK   100
#define MUX_PUSCH_CQI_RI     200


#define PUCCH_SDU_ADDED      1<<0
#define MUX_PUSCH_SDU_ADDED  1<<2

#define PUSCH_LIST_SENT      1<<4
#define PUCCH_LIST_SENT      1<<5


typedef enum tLtePhyRxSduProcessingState
{
    SDU_NEW_UL = 0,
    SDU_DESCRAMBLER_DONE,
    SDU_FEC_UL_DONE,
    SDU_CRC_UL_DONE,
    SDU_SR_DONE,
    SDU_RX_PROCESSING_STATE_MAX,
    SDU_RX_PUCCH
} LTE_PHY_UL_SDU_PROCESSING_STATE;

typedef struct tUlPuschSduProcDesc
{
    LTE_PHY_UL_SDU_PROCESSING_STATE sduState;
    U32* pFirst;

    PFEC_UL_FDESC pFecUlStart;
    PMDMA_FDESC   pMdmaStart;

    U32 t_arm;
    U32 t_mdma;
    U32 t_fec;
    U32 t_total;

    U32 numPUCCH;
} LTE_PHY_UL_PUSCH_SDU_DESC, *PLTE_PHY_UL_PUSCH_SDU_DESC;

typedef struct tUlSduDesc
{
    struct tUlSduDesc* Next;
    struct tUlSduDesc* NextCqiRiMuxCtrl;
    struct tUlSduDesc* NextHiMuxCtrl;

    PLTE_PHY_UL_PUSCH_SDU_DESC pSduPuschProc;
    U32 t_arm;
    U32 t_total;

    PMAC2PHY_QUEUE_EL pListElement;
    PMAC2PHY_QUEUE_EL pListElementCqiRi;
    PMAC2PHY_QUEUE_EL pListElementHi;
    U8*               pListRxEnd;

    //FEC Parameters
    U8 *out;        //Pointer to Output of FEC
    S32 outp;
    U8 *CrcOut;

    U32 sduType;
    U16 chanIndex;
    S16 subframeNumber;
    U16 rxFrameNumber;
    U32 numBitsRx;
    U32 simSRHarq;
    S16 srdetected;
    S16 TimingAdvance;
    U16 cqiPmiConf;
    S32 mErrAvg[2];
    S16 mErrExpo;
    U8  ul_CQI;
    U16 Rssi_dB;
    U32 stat;
    U32 currTti;
    U32 RxSduProcCtx;

    PULRXFECPARAMS    pUlRxFecParams;
    PULRXSDUPARAMS    pUlRxSduParams;
    PULRXINPUTPARAMS  pUlRxInputParams;
    PTCB              pTcbMuxCtrl;

    PLTE_BS_PHY_STATS_USER_STAT pUserStat;
    PLTE_BS_PHY_STATS_UE_CTX    pPhyStatsUlUeCtx;
} LTE_PHY_UL_SDU_DESC, *PLTE_PHY_UL_SDU_DESC;


UINT32 LteBsRxPuschSduCleanUp(LPVOID pParam);
UINT32 LteBsRxPucchSduCleanUp(LPVOID pParam);
UINT32 LteBsRxMuxPuschSduCleanUp(LPVOID pParam);

#endif

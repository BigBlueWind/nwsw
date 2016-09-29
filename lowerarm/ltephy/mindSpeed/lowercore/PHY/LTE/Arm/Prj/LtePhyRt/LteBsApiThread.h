//-------------------------------------------------------------------------------------------
/** @file LteBsApiThread.h
 *
 * @brief API processing and Task Lists management thread
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.37 $
 *
 * COPYRIGHT&copy; 2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#ifndef LTEBSAPI_H
#define LTEBSAPI_H

#include "4gmx_types.h"
#include "ltephydef.h"
#include "phydi.h"

#define APITHREAD_FIFO_LEN                  10
#define IncrementApiThreadFifo(a)           ((a == APITHREAD_FIFO_LEN-1) ? 0 : a+1)

typedef enum tLtePhyApiProcessingState
{
    PDCCH_NEW = 0,
    PDCCH_SCRAMBLER_DONE,
    API_PROCESSING_STATE_MAX

} API_LIST_STATE;

#define FFT_SLOT0_DESCRIPTOR_SETUP_DONE            (1<<0)
#define FFT_SLOT0_IRQ_DONE                         (1<<1)
#define FFT_SLOT0_IQ_COPY_DONE                     (1<<2)
#define SCHEDULE_FFT                               (FFT_SLOT0_DESCRIPTOR_SETUP_DONE | FFT_SLOT0_IRQ_DONE | FFT_SLOT0_IQ_COPY_DONE)

#define PHY_API_CLEANUP__MAC_TO_PHY_LIST            (1<<0)
#define PHY_API_CLEANUP__MAC_TO_PHY_DUMMY_LIST      (1<<1)
#define PHY_API_CLEANUP__PHY_TO_MAC_SRS_MSG         (1<<2)
#define PHY_API_CLEANUP__PHY_TO_MAC_RACH_MSG        (1<<3)
#define PHY_API_CLEANUP__PHY_TO_MAC_OTHER_MSG       (1<<4)
#define PHY_API_CLEANUP__PHY_TO_MAC_RX_SDU_LIST     (1<<5)
#define PHY_API_CLEANUP__PHY_TO_MAC_CLEANUP_THREAD  (1<<6)

typedef struct _tagLTE_PHY_API_THREAD_CTX
{
    HANDLE  hLtePhyThread;
    HANDLE  hLtePhyApiLogThreadNew;
    HANDLE  hLtePhyApiLogThread;
    HANDLE  hLtePhyCleanUpPucch;
    HANDLE  hLtePhyCleanUpPusch;
    HANDLE  hLtePhyCleanUpMuxPusch;

    HANDLE  hFracTimer;

    PMAC2PHY_QUEUE_EL pMsg;
    PLTEPHYDESC pPhy;

    UINT32 pucchCleanUpRxSduProcCtx[NUM_UL_SDU_CONTEXT];
    UINT32 puschCleanUpRxSduProcCtx[NUM_UL_SDU_CONTEXT];
    UINT32 muxPuschCleanUpRxSduProcCtx[NUM_UL_SDU_CONTEXT];
    UINT32 pucchCleanUpPush, pucchCleanUpPop;
    UINT32 puschCleanUpPush, puschCleanUpPop;
    UINT32 muxPuschCleanUpPush, muxPuschCleanUpPop;

    PMAC2PHY_QUEUE_EL pMsgQueue[APITHREAD_FIFO_LEN];
    void *pLteBsApiErrorCheck[APITHREAD_FIFO_LEN];
    UINT32 isDummyQueue[APITHREAD_FIFO_LEN];
    UINT32 apiThreadPush;
    UINT32 apiThreadPull;

    volatile U32 t;
    volatile U32 t_mdma;
} LTE_PHY_API_THREAD_CTX, *PLTE_PHY_API_THREAD_CTX;

#define MAX_SUCCESSIVE_NO_API_TTIS_BEFORE_STOP   15
#define FIFO_DEPTH                               30
#define FIFO_FULL_ERR                            1
#define FIFO_EMPTY_ERR                           2
#define FIFO_INVALID_ERR                         3

typedef struct Fifo_Stack{
    LPVOID ListHeader[FIFO_DEPTH];
    UINT32 FifoCleanType[FIFO_DEPTH];
    UINT32 FifoCleanMsgLen[FIFO_DEPTH];
    UINT32 ListPushLoc;
    UINT32 ListPopLoc;
}FIFO_STACK, *PFIFO_STACK;

LTE_PHY_API_THREAD_CTX *LtePhyApiGetCtx(void);
MXRC LtePhyInitApiThread(void);
MXRC LtePhyDestroyApiThread(void);
void LtePhyScheduleApiThread(PMAC2PHY_QUEUE_EL pMsg, UINT32 isDummy, void* pLteBsApiErrorCheck);
UINT32 LtePhyApiThread(LPVOID pParam);

void LtePhyInitMacToPhyMessageStack(void);
void LtePhyDestroyMacToPhyMessageStack(void);

UINT32 LtePhyApiCleanUpThread(PMAC2PHY_QUEUE_EL pMsg,UINT32 statusTx, UINT32 statusRx);
UINT32 LtePhyStopApiFree(void);
UINT32 LtePhyStartApiFree(void);

MXRC LtePhyInitApiLogThread(void);
MXRC LtePhyInitApiLogThreadSecond(void);
void LtePhyDestroyApiLogThread(void);
void LtePhyScheduleApiLogThread(void);
UINT32 LtePhyApiLogThread(LPVOID pParam);

UINT32 LtePhyApiThreadStartTimer(void);
UINT32 LtePhyApiThreadStopTimer(void);
void LtePhyTimerApiIrqCb(LPVOID pParam);
UINT32 LteBsApiThreadEnableLogs(void);
U32 LteBsApiThreadSetupMapVars(void);

void ScheduleFftSlot0(UINT32 mask);
UINT32 LtePhyApiThreadSchedLogging(PMAC2PHY_QUEUE_EL pMsg, UINT32 loc, UINT32 isDummy);



#endif

//-------------------------------------------------------------------------------------------
/** @file LteBsRxThread.h
 *
 * @brief Implementation of UL De-Modulator thread for the LTE BS
 * @author Mindspeed Technologies
 * @version $Revision: 1.28 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#ifndef LTEBSRXTHREAD_H
#define LTEBSRXTHREAD_H

#include "ltephydef.h"
#include "LtePhyL2Api.h"
#include "lte_entry.h"
#include "phydi.h"

#define PROCESS_CTX_BUF_LEN             10
#define IncrementProcessCtx(a)          ((a == PROCESS_CTX_BUF_LEN-1) ? 0 : a+1)

typedef struct tLteBsRxThread
{
    PMAC2PHY_QUEUE_EL pMsgQueue[PROCESS_CTX_BUF_LEN];
    void *pLteBsApiErrorCheck[PROCESS_CTX_BUF_LEN];
    UINT32 isDummyQueue[PROCESS_CTX_BUF_LEN];
    UINT32 rxThreadPush;
    UINT32 rxThreadPull;
} LTE_BS_RX_THREAD, *PLTE_BS_RX_THREAD;

typedef struct tLte_Rx_Process_Ctx
{
    volatile EXE_LIST_STATE listStateRx;
    PLTEPHYDESC pPhy;
    U32 numChan, rxSduCtx;
    U32 t;
} LTE_RX_PROCESS_CTX, *PLTE_RX_PROCESS_CTX;


MXRC LtePhySchedulePhyThread(LPVOID pParam);
UINT32 LtePhyThreadRx(PLTEPHYDESC pPhy);
void LteRxListDoneCb(TASKLIST* thisList, void* param);
UINT32 LteBsRxThreadLowLatencyPush(PMAC2PHY_QUEUE_EL pMsg, UINT32 isDummy, void* pLteBsApiErrorCheck);

#endif /* LTEBSRXTHREAD_H */


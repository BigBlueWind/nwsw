//-------------------------------------------------------------------------------------------
/** @file LteBsTxThread.h
 *
 * @brief Implementation of DL processing thread for the LTE BS
 * @author Mindspeed Technologies
 * @version $Revision: 1.19 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#ifndef LTEBSTXTHREAD_H
#define LTEBSTXTHREAD_H

#include "ltephydef.h"
#include "phydi.h"

#define PROCESS_CTX_BUF_LEN             10
#define IncrementProcessCtx(a)          ((a == PROCESS_CTX_BUF_LEN-1) ? 0 : a+1)

typedef struct tLte_Tx_Process_Ctx
{
    volatile EXE_LIST_STATE listStateTx;
    PLTEPHYDESC pPhyList, pPhyApi;
    U32 t;

} LTE_TX_PROCESS_CTX, *PLTE_TX_PROCESS_CTX;

void LteTxIfftDoneCb(void* param);
UINT32 LtePhyThreadTx(PLTEPHYDESC pPhy);
void LtePhyTxListDoneCb(TASKLIST* thisList, void* param);

#endif /* LTEBSTXTHREAD_H */


//-------------------------------------------------------------------------------------------
/** @file LteBsRxSduPreProcThread.h
 *
 * @brief Implementation of Rx FEC decoding tasks preparation thread for the LTE BS
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT&copy; 2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#ifndef LTEBSRXSDUPREPROC_H
#define LTEBSRXSDUPREPROC_H

#include "4gmx_types.h"
#include "ltephydef.h"
#include "phydi.h"

#define ALLOC_PUCCH_SDU_DEPTH    30
#define IncrementAllocPucchSduCtx(ctx) ((ctx == ALLOC_PUCCH_SDU_DEPTH-1) ? 0 : ctx+1)

typedef struct _tagLTE_PHY_RXSDUPREPROC_THREAD_CTX
{
    HANDLE  hLtePhyThread;
//    fListDoneCB *ListDoneCb;
    EXE_LIST_STATE listState;
    PLTEPHYDESC pPhy;
    U32 t;

} LTE_PHY_RXSDUPREPROC_THREAD_CTX, *PLTE_PHY_RXSDUPREPROC_THREAD_CTX;

MXRC LtePhyInitRxSduPreprocThread(void);
void LtePhyScheduleRxSduPreprocThread(void);
LTE_PHY_RXSDUPREPROC_THREAD_CTX *LtePhyRxSduPreprocGetCtx(void);

extern void LtePhySduDecMdmaFecCbDone(LPVOID pCtx);

U32 AllocPUCCHSdu(void);
void PushPUCCHSdu(U32 sduLoc);

#endif

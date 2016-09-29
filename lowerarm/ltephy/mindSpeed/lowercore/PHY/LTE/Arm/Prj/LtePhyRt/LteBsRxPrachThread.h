//-------------------------------------------------------------------------------------------
/** @file LteBsRxPrachThread.h
 *
 * @brief Implementation of PRACH processing thread for the LTE BS
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#ifndef LTEBSRXPRACH_H
#define LTEBSRXPRACH_H

#include "4gmx_types.h"
#include "ltephydef.h"
#include "phydi.h"
#include "LteBsPhyStats.h"

#define PRACH_PROCESS_CTX_LEN 6

#define IncrementPrachProcessCtx(a)   ((a == PRACH_PROCESS_CTX_LEN-1) ? 0 : a+1)

typedef struct _tagLTE_PHY_PRACH_PROCESS_CTX
{
    EXE_LIST_STATE listState;

    PLTEPHYDESC pPhy;
    U16 startedSfn;
    U8 startedSf;

    U32 prachProcInd;
    U32 listId;

    U32 t;
    PLTE_BS_PHY_STATS_SUBFRAME_CTX pLtePhyStatsSubFrameCtx;
} LTE_PHY_PRACH_PROCESS_CTX, *PLTE_PHY_PRACH_PROCESS_CTX;

UINT32 LtePhyInitThreadPrach(void);
UINT32 LtePhyThreadPrach(PLTEPHYDESC pPhy);
void LtePhyPrachListDoneCb(TASKLIST* pThisList, void* param);

#endif

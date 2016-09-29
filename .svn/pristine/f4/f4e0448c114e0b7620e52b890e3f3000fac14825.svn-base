//-------------------------------------------------------------------------------------------
/** @file LteBsRxSrsThread.h
 *
 * @brief Implementation of SRS processing thread for the LTE BS
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#ifndef LTEBSRXSRS_H
#define LTEBSRXSRS_H

#include "4gmx_types.h"
#include "ltephydef.h"
#include "phydi.h"
#include "LteBsPhyStats.h"

#define SRS_PROCESS_CTX_LEN  4

#define IncrementSrsProcessCtx(a)   ((a == SRS_PROCESS_CTX_LEN-1) ? 0 : a+1)

typedef struct _tagLTE_PHY_SRS_THREAD_CTX
{
    EXE_LIST_STATE listState;

    PLTEPHYDESC pPhy;
    U16 startedSfn;
    U8 startedSf;
    U32 t;

    PLTE_BS_PHY_STATS_SUBFRAME_CTX pLtePhyStatsSubFrameCtx;
} LTE_PHY_SRS_THREAD_CTX, *PLTE_PHY_SRS_THREAD_CTX;

UINT32 LtePhyInitThreadSrs(void);
UINT32 LtePhyThreadSrs(PLTEPHYDESC pPhy);
void LtePhySrsListDoneCb(TASKLIST* pThisList, void* param);

#endif

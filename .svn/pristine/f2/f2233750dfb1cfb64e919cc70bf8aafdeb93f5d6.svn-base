/** @file LteBsMacDlSchduler.c
 *
 * @brief Implemenataion of Dl MAC Schduler
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include <string.h>

#include "LteBsMacDlScheduler.h"
#include "LteTestTables.h"
#include "enbMac2PhyIface.h"
#include "LteMacTestCfgMgr.h"
#include "ltemaccore.h"

#include "lte_vectors.h"

/*
 * Private Types
 */

/*
 * Scheduler implementation for various modes
 */
typedef RESULTCODE (*SchedulerCb)(BS_CONTEXT *pBsCtx, PDLSUBFRDESC pSubFrame);

typedef struct SchedulerData
{
    U32 AvailableRb;
    SchedulerCb DlScheduler;
} DL_SCHEDULER_CONTEXT;

/*
 * Private Variables (Must be declared static)
 */

static DL_SCHEDULER_CONTEXT schedulerDlCtx;

/*
 * Implementation
 */

/** Downlink scheduler implementation
 *  (MAIN_SCHEMA)
 * @param pBsCtx Pointer to eNodeB context
 * @param pSubFrame Pointer to current downlink vector
 * @return Standard result code
 */
RESULTCODE LteBsMacListBaseDlScheduler(BS_CONTEXT *pBsCtx, PDLSUBFRDESC pSubFrame)
{
    return MAC_SUCCESS;
}

/** Return number of resource blocks needs to be allocated for specific UE
 *
 * @param mcsIndex MCS index
 * @param dataSize_p Pointer to UE load queue
 * @return Number of resource blocks
 */
U8 determineRBs(U32 mcsIndex, U32 *dataSize_p)
{
    return 0;
}

/** Downlink scheduler implementation for RCT tests
 *  (RCT_SCHEMA)
 *
 * @param pBsCtx Pointer to eNodeB context
 * @param pSubFrame Pointer to current downlink vector
 * @return Standard result code
 */
RESULTCODE LteBsMacDlTestScheduler(BS_CONTEXT *pBsCtx, PDLSUBFRDESC pSubFrame)
{
    /* Call Test Config proccessing */
    return lte_vectors_processing_dl(pSubFrame);
}

/** Downlink scheduler implementation which uses pointer-based PHY API
 *  (new API)
 *
 * @param pBsCtx Pointer to eNodeB context
 * @param pSubFrame Pointer to current downlink vector
 * @return Standard result code
 */
RESULTCODE LteBsMacDlTest_Ex(BS_CONTEXT *pBsCtx, PDLSUBFRDESC pSubFrame)
{
    /* Call Test Config proccessing */
    return lte_vectors_processing_dl(pSubFrame);
}

/** Start downlink scheduling process
 *
 * @param pBsCtx Pointer to eNodeB context
 * @param pSubFrame Pointer to current downlink vector
 * @return Standard result code
 */
RESULTCODE LteBsMacDlScheduler(BS_CONTEXT *pBsCtx, PDLSUBFRDESC pSubFrame)
{
    return schedulerDlCtx.DlScheduler(pBsCtx, pSubFrame);
}

/** Change downlink scheduler strategy
 *
 * @param pBsCtx Pointer to eNodeB context
 *               schedulingSchema member can define different strategies:
 *               MAIN_SCHEMA - TODO:
 *               RCT_SCHEMA - TODO:
 * @return Standard result code
 */
RESULTCODE LteBsMacDlSchedulerReconfig(BS_CONTEXT *pBsCtx)
{
    switch (pBsCtx->schedulingSchema)
    {
        case MAIN_SCHEMA:
//            schedulerDlCtx.DlScheduler = LteBsMacListBaseDlScheduler;
            break;
        case RCT_SCHEMA:
            schedulerDlCtx.DlScheduler = LteBsMacDlTestScheduler;
            break;
        default:
            return FAILURE;
    }
    return MAC_SUCCESS;
}

/** Change downlink scheduler settings depending on PHY API mode
 *  (compatibility mode or new mode)
 *
 * @return Standard result code
 */
RESULTCODE LteBsMacDlSchedulerReconfigByPhyMode(void)
{
    schedulerDlCtx.DlScheduler = LteBsMacDlTest_Ex;

    return MAC_SUCCESS;
}

/** Initialize downlink scheduler
 *
 * @param pBsCtx eNodeB context
 * @return Standard result code
 */
RESULTCODE LteBsMacDlSchedulerInit(BS_CONTEXT *pBsCtx)
{
    RESULTCODE ret = MAC_SUCCESS;
    /* Init Sheduler context */
    memset(&schedulerDlCtx, 0, sizeof(DL_SCHEDULER_CONTEXT));

    /* Set callback to scheduler function */
    ret = LteBsMacDlSchedulerReconfig(pBsCtx);

    return ret;
}

/** Stop downlink scheduler and clean its internal structures and state
 *
 * @param pBsCtx eNodeB context
 * @return Standard result code
 */
RESULTCODE LteBsMacDlSchedulerClean(BS_CONTEXT *pBsCtx)
{
    return MAC_SUCCESS;
}

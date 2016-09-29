/** @file LteBsMacDlSchduler.c
 *
 * @brief Implemenataion of Dl MAC Schduler
 * @author Mindspeed Technologies
 * @version $Revision: 1.47 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "basetypes.h"
#include "resultcodes.h"
#include "LteTestTables.h"
#include "LteBsMacDlScheduler.h"
#include "LteMac2RrcInterface.h"
#include "LteBsMacPdschMgr.h"
#include "LteMac2PhyInterface.h"
#include "LteMacUeContext.h"
#include "LteBsMacPdcchMgr.h"
#include "LteMacTestCfgMgr.h"
#include "ltemaccore.h"
#include "fdxmac.h"

#include "lte_vectors.h"

#ifdef AC_MSPD_TDD
#include "MgrCfgTdd.h"
#endif /*AC_MSPD_TDD*/
//#include "queue.h"

/****************************************************************************
 Private Definitions
 ****************************************************************************/

/****************************************************************************
 Private Types
 ****************************************************************************/
extern U8 testBigSduData[4587];

typedef RESULTCODE (*SchedulerCb)(BS_CONTEXT *pBsCtx, PDLSUBFRDESC pSubFrame);

typedef struct SchedulerData
{
    U32 AvailableRb;
    PDCCH_DL_CTX pdcchDlCtx;

    SchedulerCb DlScheduler;
}DL_SCHEDULER_CONTEXT;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

static DL_SCHEDULER_CONTEXT schedulerDlCtx;

extern U8 testMacSduData[1419];

U8 determineRBs(U32  mcsIndex, U32  *dataSize_p)
{
   S8  ibs_idx = 0;
   /* Binary lookup */
   U8 low, mid, high;
   //U32  tempDataSize = (*dataSize_p << 3 );
   low = 0;
   high = cellConfig.dlAvailableRBs - 1;

   ibs_idx = TBSArr[mcsIndex];
   *dataSize_p = *dataSize_p << 3;

    //we cannot allocate less than 1 RB
    if(TBSizeArr[ibs_idx][low] >= *dataSize_p)
    {
      *dataSize_p = TBSizeArr[ibs_idx][low] >> 3;
      return (low + 1);
    }

    while( low <= high )
    {
        mid = (low + high) / 2;
        if(TBSizeArr[ibs_idx][mid] < *dataSize_p)
        {
            low = mid + 1;
        }
        else if(TBSizeArr[ibs_idx][mid] > *dataSize_p)
        {
            if(TBSizeArr[ibs_idx][mid - 1] < *dataSize_p)
            {
                *dataSize_p = TBSizeArr[ibs_idx][mid] >> 3;
                return (mid + 1);
            }
            else if(TBSizeArr[ibs_idx][mid - 1] == *dataSize_p)
            {
                /* since each sub-table has multiple enteries with
                 * the same TB Size
                 * so find the first entry with this TB Size*/
                *dataSize_p = TBSizeArr[ibs_idx][mid - 1];
                --mid;
                while((TBSizeArr[ibs_idx][mid - 1] == *dataSize_p)
                    && (mid > low))
                {
                    --mid;
                }

                *dataSize_p = *dataSize_p >> 3;
                return (mid + 1);
            }
            else
            {
                high = mid - 1;
            }

        }
        else if(TBSizeArr[ibs_idx][mid] == *dataSize_p)
        {

            /* Take care of the case where THIS infact is THE size
              we are looking for */
            while((TBSizeArr[ibs_idx][mid - 1] == *dataSize_p) && (mid > low))
            {
                --mid;
            }

            *dataSize_p = *dataSize_p >> 3;
            return (mid + 1);
        }
    }

    return 0;
}

/* Schduler for test 36.141 */
RESULTCODE LteBsMacDlTestScheduler(BS_CONTEXT *pBsCtx, PDLSUBFRDESC pSubFrame)
{
    /* Call Test Config proccessing */
    return lte_vectors_processing_dl(pSubFrame);
        
    //printDlVector(pSubFrame);
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
    return lte_vectors_processing_dl_rt(pSubFrame);
}


RESULTCODE LteBsMacDlScheduler(BS_CONTEXT *pBsCtx, PDLSUBFRDESC pSubFrame)
{
    return schedulerDlCtx.DlScheduler(pBsCtx, pSubFrame);
}
#ifdef AC_MSPD_TDD
RESULTCODE LteBsMacDlSPSchedulerTdd(BS_CONTEXT *pBsCtx, PDLSUBFRDESC pSubFrame)
{
    U32 PdschEna = 1;
    if(pBsCtx->SpTdd == 1)
    {
    if(pBsCtx->initParm.cyclicPrefixType == 0 &&
        (pBsCtx->initParm.specialSubframeConfig == 0 || pBsCtx->initParm.specialSubframeConfig == 5))
        PdschEna = 0;
    if(pBsCtx->initParm.cyclicPrefixType == 1 &&
        (pBsCtx->initParm.specialSubframeConfig == 0 || pBsCtx->initParm.specialSubframeConfig == 4))
        PdschEna = 0;
    }

    return TestMgrProcessConfigSpDlTdd(pSubFrame, pBsCtx->SpTdd, PdschEna);
}
void setPhich(U8 PhichFactorM)
{
    setPhichTdd(PhichFactorM);
}
#endif

RESULTCODE LteBsMacDlSchedulerReconfig(BS_CONTEXT *pBsCtx)
{

    schedulerDlCtx.DlScheduler = LteBsMacDlTestScheduler; // c-code (windows)

    return SUCCESS;
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

RESULTCODE LteBsMacDlSchedulerInit(BS_CONTEXT *pBsCtx)
{
    RESULTCODE ret = SUCCESS;
    /* Init Sheduler context */
    memset(&schedulerDlCtx, 0, sizeof(DL_SCHEDULER_CONTEXT));

    /* Set callback to scheduler function */
    ret = LteBsMacDlSchedulerReconfig(pBsCtx);

    return ret;
}

RESULTCODE LteBsMacDlSchedulerClean(BS_CONTEXT *pBsCtx)
{
    return SUCCESS;
}


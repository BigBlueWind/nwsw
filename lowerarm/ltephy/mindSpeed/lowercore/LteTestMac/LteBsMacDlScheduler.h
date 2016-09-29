/** @file LteBsMacDlSchduler.h
 *
 * @brief Interface file to the DL Schduler
 * @author Mindspeed Technologies
 * @version $Revision: 1.14 $
 *
 * COPYRIGHT(c) 2008-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _LTE_MAC_DL_SCHEDULER_H
#define _LTE_MAC_DL_SCHEDULER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "resultcodes.h"
#include "bsmac.h"

#ifndef MX_ARM_ENABLED
#include "basetypes.h"
#endif

#ifndef AC_MSPD_TDD
#include "LtePhyL2Api.h"
#else
#include "LtePhyL2Api_TDD.h"
#endif

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

RESULTCODE LteBsMacDlScheduler(BS_CONTEXT *pBsCtx, PDLSUBFRDESC pSubFrame);
RESULTCODE LteBsMacDlSchedulerInit(BS_CONTEXT *pBsCtx);
RESULTCODE LteBsMacDlSchedulerClean(BS_CONTEXT *pBsCtx);
RESULTCODE LteBsMacDlSchedulerReconfig(BS_CONTEXT *pBsCtx);
RESULTCODE LteBsMacDlSchedulerReconfigByPhyMode(void);

U8 determineRBs(U32 mcsIndex, U32 *dataSize_p);
#ifdef MX_ARM_ENABLED
U32 determineTBSize(U32 mcsIndex, U32 nbRB, U8 TxMode);
#else
U32 determineTBSize(U32 mcsIndex, U32 nbRB, U8 TxMode);
void setPhich(U8 PhichFactorM);
#endif

#ifdef AC_MSPD_TDD
RESULTCODE LteBsMacDlSPSchedulerTdd(BS_CONTEXT *pBsCtx, PDLSUBFRDESC pSubFrame);
#endif


#endif /* _LTE_MAC_DL_SCHEDULER_H */


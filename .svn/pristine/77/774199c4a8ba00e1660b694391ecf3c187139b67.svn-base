/** @file LteBsMacUlSchduler.h
 *
 * @brief Interface file to the Ul Schduler
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT(c) 2008-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
#ifndef _LTE_MAC_UL_SCHEDULER_H
#define _LTE_MAC_UL_SCHEDULER_H

#include "bsmac.h"

#ifdef MX_ARM_ENABLED
#include "resultcodes.h"
#else
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

RESULTCODE LteBsMacUlScheduler(BS_CONTEXT *pBsCtx, PULSUBFRDESC pSubFrame);
RESULTCODE LteBsMacUlSchedulerReconfig(BS_CONTEXT *pBsCtx);
RESULTCODE LteBsMacUlSchedulerInit(BS_CONTEXT *pBsCtx);
RESULTCODE LteBsMacUlSchedulerClean(BS_CONTEXT *pBsCtx);

#ifdef MX_ARM_ENABLED
PULSUBFRDESC LteBsMacUlVectorGet(void);
RESULTCODE LteBsMacUlSduStatus(PRXSDUIND pRxSduInd);
PULSUBFRDESC LteBsMacPrevUlCtxGet(U32 fn, U32 sfn);
#endif

PULSUBFRDESC UlList_ReadTail(void);
PULSUBFRDESC LteBsMacUlVectorGet(void);

#endif /* _LTE_MAC_UL_SCHEDULER_H */


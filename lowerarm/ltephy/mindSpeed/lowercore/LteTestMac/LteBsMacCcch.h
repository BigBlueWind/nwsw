/** @file LteBsMacDlSchduler.h
 *
 * @brief Interface file to the DL Schduler
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
#ifndef _LTE_MAC_CCCH_H
#define _LTE_MAC_CCCH_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "bsmac.h"
#include "resultcodes.h"
#include "basetypes.h"
#include "LtePhyL2Api.h"
#include "LteBsMacPdcchMgr.h"
//#include "queue.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct CCCHQueueNodeT
{
    LIST_ENTRY(CCCHQueueNodeT) ccchNodeAnchor;
    void   *pMsg; /* zero copy buffer */
    U16 rnti;     /* Represents CRNTI/TCRNTI */
    U32  msgLen;
    U8  rejFlag;  /* Connection Rejection message flag */
}CCCH_QUEUE_NODE;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

RESULTCODE CcchMgrRecive(U8 *pData, U32 dataLen);
RESULTCODE CcchMgrSchedule(PDCCH_DL_CTX *pPdcchDlCtx,
                           U32 *pAvailableCCE,
                           DCI_CCE_CONTAINER *pDciCceContainer,
                           U32 *pAvailableDlRbCount);
RESULTCODE CcchMgrInit(void);
RESULTCODE CcchMgrClean(void);


#endif /* _LTE_MAC_CCCH_H */


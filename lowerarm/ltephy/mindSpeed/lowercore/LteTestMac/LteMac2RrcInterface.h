/** @file LteMac2RrcInterface.h
 *
 * @brief Header file to
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
#ifndef _LTE_MAC_2_RRC_INTERFACE_H
#define _LTE_MAC_2_RRC_INTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "resultcodes.h"
#include "basetypes.h"
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
typedef struct ConfigCellReqT
{
    U8 dlResBlocks;
    U8 ulResBlocks;
    U8 maxHarqRetrans;
    U8 numOfTxAntennas;
    U8 startRARntiRange;
    U8 endRARntiRange;
    U8 phichDuration;
    U8 phichResource;
    U8 ulCyclicLenthPrefix;
} CONFIG_CELL_REQ;


typedef struct CellConfigInfoT
{
    U8   dlAvailableRBs;
    U8   ulAvailableRBs;

    /* Number of HARQ Process per UE*/
    U8   numHARQProcess;

    /* Maximum number of HARQ re-transmission */
    U8   maxHarqRetransmission;

    U8   phichDuration;
    U8   phichResource;
    U8   cyclicPrefix;

    U16  startRARNTI;
    U16  endRARNTI;

    U8   numOfTxAnteenas;
    /* Total active UE's in the system */
    U16  totalActiveUE;
}CELL_CONFIG_INFO;

typedef struct ueContextDlConfT
{
    U16 ueIndex;
    U16 cRnti;
    U8  uePriority;
    U8  cqiIndication;
    U8  cqiMode;
    U8  dlModScheme;
    U32 dlCodingRate;
    U8  dlMaxRB;
    U8  hScheme;
    U8  srPeriodicity;
    U8 transmissionMode;
    U8 rankIndicator;
    U8 codeBookIndex;
    U8 simultaneousAckNackAndCQI;
    //srConfig;
    //cqiInfo;
}ueContextDlConf;

typedef struct ueContextUlConfT
{
    U16 ueIndex;
    U16 cRnti;
    U8  uePriority;
    U8  cqiIndication;
    U8  cqiMode;
    U8  hScheme;
    U8  ulMaxRB;
    U8  ulModScheme;
    U32 ulCodingRate;
    U8  srPeriodicity;
    U8  transmissionMode;
    U8  rankIndicator;
    U8  codeBookIndex;
    U8  simultaneousAckNackAndCQI;
    //srConfig;
    //cqiInfo;
}ueContextUlConf;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern CELL_CONFIG_INFO cellConfig;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
RESULTCODE RrcInterfaceInit(void);
RESULTCODE RrcInterfaceClean(void);
RESULTCODE RrcProcessInterface(void);

#endif /* _LTE_RRC_2_PHY_INTERFACE_H */








/** @file LteMac2RrcInterface.h
 *
 * @brief Header file to
 * @author Mindspeed Technologies
 * @version $Revision: 1.14 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
#ifndef _LTE_MAC_UE_CONTEXT_H
#define _LTE_MAC_UE_CONTEXT_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "osal.h"
#include "resultcodes.h"
#include "basetypes.h"
#include "ltemaccore.h"
#ifndef AC_MSPD_TDD
#include "LtePhyL2Api.h"
#else
#include "LtePhyL2Api_TDD.h"
#endif
#ifdef _WIN32
#include "queue.h"
#endif

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_NUMBER_OF_LOGICAL_CHANNEL      11

/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef struct DlUeContextT
{
    /* used to store entries of type RetransmitQueueNode containing the
      HARQ Process Ids that indicate the HARQ processes which require
      retransmission of data*/
    OSAL_QUEUE  retransmissionQueue;

    /* An array of LogicalChannelInfo where each enty corresponds to details
    * regarding the UEs logical channel*/
    LOGIC_CHAN_DESC* logicalChannel[MAX_NUMBER_OF_LOGICAL_CHANNEL];

    /*A pointer to nodeAnchor of the UE in ueNonZeroQueueLoadList_g*/
    U32  * listUeNonZeroQueueLoadNode_p;
    /*A pointer to nodeAnchor of the UE in ueHarqNonZeroQueueLoadList_g*/
    U32  * listUeHarqNonZeroQueueLoadNode_p;
    /*global tick count indicating when this UE was scheduled*/
    U32              lastScheduledTick;

    /*for optimising on modscheme factor calc*/
    float            modulationSchemeFactor;

    /*for optimising on modscheme factor calc*/
    float            uePriorityFactor;

//  U32               assignedRBQueueLoad[MAX_TEMP_QUEUELOAD_ARR_SIZE];
    /*indicates the current bit rate of the UE*/
    U32              effectiveBitRate;

    /*indication of individual logical channels and UEs existence for
    further TTIs*/
    U32              pendingDeleteBitmask;

    /*indicates the composite queue load of all logical channels of the UE*/
    U32              ueQueueLoad;
   // CQIMap              cqiMap;
    U32              cqiMode;
    U32              codingRate;

    /*indicates the C-RNTI assigned to this UE*/
    U16              ueId;
    /* index into ueDLContextArr_g */
    U16              ueIndex;

    /* multiplexer will wait on this semaphore before processing this UE*/
    OSAL_SEMAPHORE   ueInUseMuxSem;
    /* before entry ueIndex into any of non zero
     * Queueload list we wait on this semaphore*/
    OSAL_SEMAPHORE   nonZeroQueueLoadListInsertSem;

    /*indicates the current state of UE*/
    U8               ueState;

    /*indicates the priority of this UE*/
    U8               uePriority;

    /*indicates the number of logical channels available for this UE*/
    U8               numLogicalChannels;

    /*signifies that this max number of resources can be used in one TTI
    for this UE*/
    U8               maxNbOfRBs;

    /*indicates the modulation scheme to be used for particular UE*/
    U8               modulationScheme;

    /*indicate that Scheduler/Dispatcher/worker thread/Timer thread
    is processing the UEs logical channels so RRC can not delete them*/
    U8               ueContextInUse;

    /*indication that worker thread should process this UE or not*/
    U8               deletionInProgress;
    U8               mcsIndex;
    U8               timingAdvanceValue;
    U8               rbsReservedForUE;
    U8               cqiTypeIndicator;
    U8               assignedRBQueueLoadSchIndex;
    U8               assignedRBQueueLoadRLCIndex;
}DlUeContext;

typedef struct UlUeContextT
{
    /*indicates the C-RNTI assigned to this UE*/
    U16             ueId;

    LOGIC_CHAN_DESC* logicalChannel[11];

    U8               numLogicalChannels;

}UlUeContext;

typedef struct UEIndexListNodeT
{
    /*data type of anchor, stored in every List Node*/
    LIST_ENTRY(UEIndexListNodeT) nodeAnchor;
    U16        ueIndex;
    U16        transactionId;
    U16        ueId;
}UE_INDEX_LIST_NODE;

typedef LIST_HEAD(ueLoadListT, UE_INDEX_LIST_NODE) UE_LOAD_LIST;
typedef enum RNTITypeT
{
    RNTI_NONE = 0,
    CRNTI,
    RA_RNTI,
}RNTI_TYPE;
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/* Collection of UEs handled by this eNode */
extern  DlUeContext * ueDlContextArr[MAX_UE_COUNT];
extern  UlUeContext * ueUlContextArr[MAX_UE_COUNT];

/* TTI tick counter (1ms) */
//extern U32 globalTTITickCount;

/* List of UE with data for transmition */
extern OSAL_MUTEX ueQueueLoadListLock;
extern UE_LOAD_LIST ueLoadList;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
RESULTCODE   UeContextInit(void);
DlUeContext* UeContextDlAlloc(void);
RESULTCODE   UeContextDlFree(DlUeContext* pDlUeContext);
UlUeContext* UeContextUlAlloc(void);
RESULTCODE   UeContextUlFree(UlUeContext* pUlUeContext);

UE_INDEX_LIST_NODE* UeLoadListAlloc(void);
RESULTCODE UeLoadListFree(UE_INDEX_LIST_NODE* pListBlock);

U8 determineRBs(U32  mcsIndex, U32  *dataSize_p);

#endif /* _LTE_MAC_UE_CONTEXT_H */


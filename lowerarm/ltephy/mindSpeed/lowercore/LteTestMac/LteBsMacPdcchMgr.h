/** @file LteBsMacPdcchMgr.c
 *
 * @brief PDCCH handling module interface
 * @author Mindspeed Technologies
 * @version $Revision: 1.11 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
#ifndef _LTE_BS_MAC_PDCCH_MGR_H
#define _LTE_BS_MAC_PDCCH_MGR_H

#include "bsmac.h"
#include "basetypes.h"
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

#define CFI_VALUE 3
#define CCE_AVAILABLE_TEST 5


/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct DLTxNodeT
{
    /*data type of anchor*/
    LIST_ENTRY(DLTxNodeT) dlTxNodeAnchor;
    U16 ueIndex;
    /*This structure stores the information needed by Resource Allocator for
      allocating RBs to a UE */
    //ResourceAllocatorInput resourceAllocInput;
    /*This field store the message type for which the PDCCH region need to be
      allocated .For xample :  Broad Cast Message ,Paging Control Message,Data
      Message ,Contention Resolution Message ,Data Message ,Data with TA ,
      Data with DRX etx */
    //MsgType msgType;
    /*This field stores the DCI Format for which PDCCH will first try to
      allocate the CCE to it */
    //DCIFormat primaryDCIFormat;
    /*This field stores the DCI Format for which PDCCH will try to allocate
      the CCE to it ,if CCE allocation of primaryDCIFormat failed */
  //  DCIFormat secondaryDCIFormat;
    /*This field stores the Transmission Type of Data like if this is New
      Transmission or Adaptive Re-Transmission or Non Adaptive Re-Transmission*/
//    TransmissionType transmissionType;
    U32  sumLCPriority;
    U32  weightedQueueLoad;

    U8 harqProcessId;
    /*This field when set to 1 indicates to Uplink Statergy that CQI request
      need to be sent to UE in the UpLink Grant Message */
    U8 isCQIRequestedULGrantMsg;
    U8 numPushedInAdvanceQueue;

}DL_TX_NODE;

typedef struct ResourceAllocation
{
   U8 requiredRB;
   U8 mcsIndex;
}RESOURCE_ALLOCATION;

typedef struct DLDataMsgInfoT
{
    DL_TX_NODE *pDlTxNode;

    U16 entryType;

    RESOURCE_ALLOCATION resourceAlloction;
    /* dciFormat for which PDCCH Region */
    DCI_FORMAT dciFormat;

    U16 tempCRNTI;
    U16 raRNTI; /* RARNTI for RAR PDU */

    U8 *pData;  /* message buffer */
    U8 dataLen; /* message buffer length */

    U8 harqProcessId;
    U8 cceIndex;
}DL_DATA_ENTRY_INFO;

typedef struct PdcchDLCtxT
{
    U8 countCommonEntry;
    U8 countEntry;
    DL_DATA_ENTRY_INFO dlDataInfoArr[10];
}PDCCH_DL_CTX;


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern U32 maxCceAvailable;

/* Array of allocated CCEs with corresponds DCIs */
extern DCI_CCE_CONTAINER dciCceInfo[1];


/****************************************************************************
 * Exported Functions
 ****************************************************************************/
RESULTCODE PdcchMgrProcess(PDCCH_DL_CTX *pPdcchDlCtx);
RESULTCODE PdcchMgrInit(void);
RESULTCODE PdcchMgrClean(void);
DL_TX_NODE* PdcchMgrAllocTxDlNode(void);
RESULTCODE PdcchMgrFreeTxDlNode(DL_TX_NODE* pDlTxNode);
RESULTCODE PdcchMgrPutTxEntryIntoProcessing(DL_TX_NODE *pDlTxNode);
U32 PdcchMgrGetCfiValue(void);
RESULTCODE PdcchMgrDciCceInit(void);


#endif /* _LTE_BS_MAC_PDCCH_MGR_H */


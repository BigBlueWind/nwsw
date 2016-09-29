/** @file LteMac2RrcInterface.c
 *
 * @brief Implemenataion of interface to Message based PHY interface
 * @author Mindspeed Technologies
 * @version $Revision: 1.15 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "osal.h"
#include "basetypes.h"
#include "resultcodes.h"
#include "LteMac2RrcInterface.h"
#include "LteBsMacCcch.h"
#include "LteMacUeContext.h"
#include "LteTestTables.h"

/****************************************************************************
 Private Definitions
 ****************************************************************************/
#define MAX_RRC_MESSAGE_SIZE 1024

/****************************************************************************
 Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
CELL_CONFIG_INFO cellConfig;

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
static U8 testRrcMsg[19] =
    { 8, 17, 11, 120, 5, 0, 0, 0, 0, 119, 0, 5, 112, 0, 0, 24, 144, 193, 65};

extern int QueueRrcToMac(U16* logicChanID, PTR pBuf, OSAL_SIZE blockSize);

RESULTCODE configeMacCell(CONFIG_CELL_REQ *pCellCongReq)
{
    cellConfig.dlAvailableRBs  = pCellCongReq->dlResBlocks;
    /* Configure the Uplink Resources */
    cellConfig.ulAvailableRBs = pCellCongReq->ulResBlocks;

    /* Configure the Start and End Range of RA-RNTI */
    cellConfig.startRARNTI    = pCellCongReq->startRARntiRange;
    cellConfig.endRARNTI      = pCellCongReq->endRARntiRange;

    /* Configure the Number of TX Antennas */
    cellConfig.numOfTxAnteenas = pCellCongReq->numOfTxAntennas;

    /* Set Total Number of Acwith default Value 0*/
    cellConfig.totalActiveUE  =  0;

    if ( cellConfig.ulAvailableRBs)
    {
       // initializeResourceMap();
    }

    cellConfig.maxHarqRetransmission = pCellCongReq->maxHarqRetrans;
    cellConfig.phichDuration         = pCellCongReq->phichDuration;
    cellConfig.phichResource         = pCellCongReq->phichResource;
    cellConfig.cyclicPrefix          = pCellCongReq->ulCyclicLenthPrefix;

    return SUCCESS;
}


RESULTCODE createMacUeEntity(ueContextDlConf *pDlUeInfo,
                             ueContextUlConf *pUlUeInfo)
{
    DlUeContext* pUe = NULL;
    U16 ueIdx;

    // DL
    ueIdx = pDlUeInfo->ueIndex;

    // check if this UE already exists
    if (ueDlContextArr[ueIdx] != NULL)
        return FAILURE;

    // allocate memory for new UE DL context
    pUe = UeContextDlAlloc();
    if (pUe == NULL)
        return FAILURE;

    // configure DL ue
    pUe->ueIndex = pDlUeInfo->ueIndex;
    //...

    // put in to processing
    ueDlContextArr[ueIdx] = pUe;

    // UL

    ++cellConfig.totalActiveUE;
    return SUCCESS;
}

RESULTCODE deleteMacUeEntity(U16 ueIdx)
{


    return SUCCESS;
}

RESULTCODE reconfigureMacUeEntity(ueContextDlConf *pDlUeInfo,
                                  ueContextUlConf *pUlUeInfo)
{

    return SUCCESS;
}

RESULTCODE RrcInterfaceTestConfig(void)
{
    /* general test config */
    CONFIG_CELL_REQ cellCfg =
    {
        /* dlResBlocks */           50,
        /* ulResBlocks */           50,
        /* maxHarqRetrans */        8,
        /* numOfTxAntennas */       1,
        /* startRARntiRange */      0,
        /* endRARntiRange */        0xFF,
        /* phichDuration */         3,
        /* phichResource */         0,
        /* ulCyclicLenthPrefix */   0,
    };

    /* Test Ue */
    ueContextDlConf UeDlConf = {0};
    ueContextUlConf UeUlConf = {0};

    // Init Cell;
    configeMacCell(&cellCfg);
    // Create one UE
    createMacUeEntity(&UeDlConf, &UeUlConf);

    // Create of Logical Channels

    return SUCCESS;
}

RESULTCODE RrcInterfaceClean(void)
{
    // clean stuff

    return SUCCESS;
}

/* Init RRC interface module */
RESULTCODE RrcInterfaceInit(void)
{
    // init staff

    // for test purpose
    RrcInterfaceTestConfig();

    return SUCCESS;
}

int FakeRrcToMac(U16* logicChanID, PTR pBuf, OSAL_SIZE blockSize)
{
    OSAL_SIZE msgSize;

    msgSize = 19;
    *logicChanID = 0;
  //  memcpy(pBuf, testPDCCH, 128);
  //  memset(pBuf, 0xff, 128);
    memcpy(pBuf, &testRrcMsg[3], 19-3);

    return msgSize;
}


/* Process incoming messages (such as CCCH, DCCH etc) from RRC layer */
RESULTCODE RrcProcessInterface(void)
{
    U16 logicChanType;
    U32 msgBuf[MAX_RRC_MESSAGE_SIZE];
    U32 rcvLen;
    char stop = FALSE;

    /* Dequeue messages */
    while((rcvLen = QueueRrcToMac(&logicChanType,
                                  &msgBuf,
                                  MAX_RRC_MESSAGE_SIZE)) && stop == FALSE)
    {
        switch(logicChanType)
        {
            case BCCH:
                printf("BCCH\n");
                break;

            case CCCH:
                printf("CCCH\n");
                CcchMgrRecive((U8*)&msgBuf, rcvLen);
                stop = TRUE;
                break;

            case DCCH:
                printf("DCCH\n");
                break;

            default:
                printf("Error in funtion %s (%s:%d)\n",
                        __FUNCTION__, __FILE__, __LINE__);
                break;
        }
    }

    return SUCCESS;
}

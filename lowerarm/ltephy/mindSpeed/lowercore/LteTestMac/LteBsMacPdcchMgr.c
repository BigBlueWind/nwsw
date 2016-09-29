/** @file LteBsMacPdcchMgr.c
 *
 * @brief PDCCH handling module
 * @author Mindspeed Technologies
 * @version $Revision: 1.14 $
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
#include "osal.h"
#include "LteMacUeContext.h"
#include "LteBsMacPdcchMgr.h"
#include "LteBsMacCcch.h"
#include "LteMac2RrcInterface.h"
#include "ltemaccore.h"
//#include "queue.h"

/****************************************************************************
 Private Definitions
 ****************************************************************************/
#define DLTXQUEUE_SIZE     4*MAX_UE_COUNT
#define DLRETXQUEUE_SIZE   4*MAX_UE_COUNT
#define ULTXQUEUE_SIZE     4*MAX_UE_COUNT

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

/* The Maximum Number of CCE Available for PDCCH Region */
//U32 maxCceAvailable;

/* container of CCE with DCI and coresponding data */
//DCI_CCE_CONTAINER dciCceInfo[1];


/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/* Tx Ue */
//static OSAL_QUEUE dlTxQueue;
static OSAL_MEMARRAY dlTxNodePart;

/* retransmitting Tx Ue */
//static OSAL_QUEUE dlReTxQueue;

/* For those UE for which PDCCH
   region need to be allocated so the Uplink Grant Message can be sent */
//static OSAL_QUEUE ulTxQueue;

/*This stores the Number of OFDMA Symbol used for PDCCH Region */
//static U32 cfiValue;

/*
static RESULTCODE PdcchMgrCfiValueInit(void)
{
    cfiValue = CFI_VALUE;
    return SUCCESS;
}*/

/*
static RESULTCODE PdcchMgrMaxCceInit(void)
{
    maxCceAvailable = MAX_CCE;

    return SUCCESS;
}*/

/*
U32 PdcchMgrGetCfiValue(void)
{
    return cfiValue;
}*/

/*RESULTCODE PdcchMgrDciCceInit(void)
{
    U32 i;
    for (i = 0; i < MAX_CCE; i++)
    {
       dciCceInfo[0].arrayCCEAllocated[i] = 0;
       dciCceInfo[0].dciCceInfo[i].cceIndex = i;
       dciCceInfo[0].dciCceInfo[i].isCCEFree = TRUE;
    }

    dciCceInfo[0].countOfCceAllocated = 0;

    return SUCCESS;
}*/

/*
RESULTCODE PdcchMgrProcess(PDCCH_DL_CTX *pPdcchDlCtx)
{
    DL_TX_NODE *pDlTxNode = NULL;
    DCI_CCE_CONTAINER *pDciCceContainer = &dciCceInfo[0];
    U32 cceAvailable = maxCceAvailable;
    U32 availableDlRbCount  =  cellConfig.dlAvailableRBs;

    printf("%s\n",__FUNCTION__);
    // Common stuff

    // BCCH

    // CCCH
    CcchMgrSchedule(pPdcchDlCtx,
                    &cceAvailable,
                    pDciCceContainer,
                    &availableDlRbCount);

    // New Dl Tx Dequeue
    while(OsalDequeue(&dlTxQueue,
                      (OSAL_PADDRESS *) pDlTxNode) == SUCCESS)
    {
        // allocate CCE

        // populate DCI

        // call Mux

        // put to Phy Interface queue
    }

    return SUCCESS;
}*/

/*RESULTCODE PdcchMgrPutTxEntryIntoProcessing(DL_TX_NODE *pDlTxNode)
{
    if (OsalEnqueue(&dlTxQueue, (OSAL_PADDRESS) pDlTxNode) == SUCCESS)
    {
        printf("Error in funtion %s (%s:%d)\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }
    return SUCCESS;
}*/

/*
DL_TX_NODE* PdcchMgrAllocTxDlNode(void)
{
    OSAL_PADDRESS pBlock;

    if (OsalAllocMemArray(&dlTxNodePart, &pBlock) == SUCCESS)
        return (DL_TX_NODE*)pBlock;
    else
        return NULL;
}*/

/*
RESULTCODE PdcchMgrFreeTxDlNode(DL_TX_NODE* pDlTxNode)
{
    return OsalFreeMemArray(&dlTxNodePart,(OSAL_PADDRESS) pDlTxNode);
}*/


RESULTCODE PdcchMgrClean(void)
{
    //TODO: clean

    return SUCCESS;
}

RESULTCODE PdcchMgrInit(void)
{
    OSAL_PADDRESS pMem = NULL;

    /* Tx Ue Queue  */
    if (OsalAllocHeap(&OsalGlobalHeap,
                           &pMem,
                           DLTXQUEUE_SIZE * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        printf("Error in funtion %s (%s:%d)\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }

/*
    if (OsalCreateQueue(&dlTxQueue,
                        pMem,
                        DLTXQUEUE_SIZE * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        printf("Error in funtion %s (%s:%d)\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }*/

    if (OsalAllocHeap(&OsalGlobalHeap, &pMem,
                          (OSAL_SIZE)(sizeof(DL_TX_NODE) * MAX_UE_COUNT))
                   != SUCCESS)
    {
        printf("Error in funtion %s (%s:%d)\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }

    if (OsalCreateMemArray(&dlTxNodePart,
                           pMem,
                          (OSAL_SIZE)(sizeof(DL_TX_NODE) * MAX_UE_COUNT),
                          (OSAL_SIZE) sizeof(DL_TX_NODE))
               != SUCCESS)
    {
        printf("Error in funtion %s (%s : %d)\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }

    /* retransmitting Tx Ue Queue */


    /* UL Ue Queue */


    /* Initializing CFI Value*/
//    PdcchMgrCfiValueInit();

    /* finding number of PHICH Group*/
    //calculatePHICHGroup();

    /* Calculating Max Number of CCEs*/
//    PdcchMgrMaxCceInit();

    /* Initialize DCI CCE Info Container*/
//    PdcchMgrDciCceInit();

    /*Funtion for finding number of CCE for DCI Format*/
    //calculateDCIFormatToCCEMapping();

    /*Function initialize DIrect Indexing Array in each
     *of the container*/
    //initializeDirectIndexingArray();


    return SUCCESS;
}


/** @file LteBsMacCcch.c
 *
 * @brief Implemenataion of Dl MAC Multiplexor
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#if ENODEB

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "basetypes.h"
#include "ltemaccore.h"
#include "LteBsMacCcch.h"
#include "LteMacUeContext.h"
#include "resultcodes.h"

/****************************************************************************
 Private Definitions
 ****************************************************************************/
#define CCCHQUEUE_SIZE  MAX_UE_COUNT * 5

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

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

extern void* MacAllocBlock(U32 size);
extern void MacFreeBlock(void** p);


static OSAL_QUEUE ccchQueue;
static OSAL_MEMARRAY ccchNodePart;

static RESULTCODE CcchMgrPutEntryIntoSchduling(CCCH_QUEUE_NODE *pNode)
{
    printf("CcchMgrPutEntryIntoSchduling\n");
    if (OsalEnqueue(&ccchQueue, (OSAL_PADDRESS) pNode) != SUCCESS)
    {
        printf("Error in funtion %s (%s:%d)\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }
    return SUCCESS;
}

static CCCH_QUEUE_NODE* CcchMgrAllocNode(void)
{
    OSAL_PADDRESS pBlock;

    if (OsalAllocMemArray(&ccchNodePart, &pBlock) == SUCCESS)
        return (CCCH_QUEUE_NODE*)pBlock;
    else
        return NULL;
}

static RESULTCODE CcchMgrFreeNode(CCCH_QUEUE_NODE* pNode)
{
    return OsalFreeMemArray(&ccchNodePart,(OSAL_PADDRESS) pNode);
}

RESULTCODE CcchMgrRecive(U8 *pData, U32 dataLen)
{
    //U16 ueIndex = 0;
    CCCH_QUEUE_NODE *pCcchNode = NULL;

    /* figure out RNTI
     NOTE: whatever RNTI it coresponds to first Ue */

    /* locate Ue context according to RNTI */

    /* alloc queue entry */
    pCcchNode = CcchMgrAllocNode();
    if (pCcchNode)
    {
        pCcchNode->pMsg = (U8*)malloc(dataLen);

        memcpy(pCcchNode->pMsg, pData, dataLen);
        pCcchNode->msgLen = dataLen;
        /* prepare information for the schduler */
        pCcchNode->rnti = 0;

        /* put to the scheduling */
        CcchMgrPutEntryIntoSchduling(pCcchNode);
    }
    else
    {
        printf("Error in funtion %s (%s:%d)\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }

    return SUCCESS;
}

U16 getUeIdxFromRnti(U16 rnti)
{
    /* TODO: find ueIndex base on rnti */
    return 0;
}


static DCI_DATA_INFO* CcchMgrAllocCce(U32 *pAvailableCCE,
                                      DCI_CCE_CONTAINER *pDciCceContainer)
{
    U32 i;
    DCI_2_CCE* pDci2Cce = &pDciCceContainer->dciCceInfo[0];
    /* search free CCE for CCCH channel */
    if (*pAvailableCCE > 0)
    {
        for (i = 0; i < MAX_CCE; i++)
        {
            if (pDci2Cce->isCCEFree == TRUE)
            {
                pDci2Cce->isCCEFree = FALSE;
                pDci2Cce->dciCCEInfo.cceIndex = pDci2Cce->cceIndex = i;
                pDciCceContainer->arrayCCEAllocated[pDci2Cce->cceIndex]
                    = CCE_ALLOCATED;
                (*pAvailableCCE)--;
                ++pDciCceContainer->countOfCceAllocated;
                return &pDci2Cce->dciCCEInfo;
            }
            else
                pDci2Cce++;
        }
    }

    return NULL;
}

RESULTCODE CcchMgrPrepareDci(DCI_DATA_INFO* pDciData,
                             CCCH_QUEUE_NODE *pCcchNode)
{
    pDciData->ueIndex = getUeIdxFromRnti(pCcchNode->rnti);
    pDciData->dciFormat = DCI_FORMAT_1;
    printf("%s\n",__FUNCTION__);

    /* fill DCI stuff */
//    pDciData->transmissionMode = pCcchNode->


    /* DCI should be sent via PDCCH */

    return SUCCESS;
}

U8* CcchMghConcatMacHdr(U16 hdrLen, U8* pMsg, U32 msgLen)
{
    MAC_HEADER* pHdr =  NULL;

    pHdr = (MAC_HEADER*) MacAllocBlock(hdrLen + msgLen);

    if (hdrLen == 2)
    {
        pHdr->Short.F = 0;
        pHdr->Short.E = 0;
        pHdr->Short.LCID = MAC_CCCH_LCID;
        pHdr->Short.L = msgLen;                 // 7 bits
        pHdr->Short.R = 0;
        memcpy(((U8*)pHdr) + 2, pMsg, msgLen);
    }
    else if (hdrLen == 3)
    {
        pHdr->Long.F = 1;
        pHdr->Long.E = 0;
        pHdr->Long.LCID = MAC_CCCH_LCID & 0x1F;
        pHdr->Long.L1 = (msgLen >> 8) & 0x7F;                 // 15 bits
        pHdr->Long.L2 = msgLen & 0xFF;
        pHdr->Long.R = 0;
        memcpy(((U8*)pHdr) + 3, pMsg, msgLen);
    }
    else
    {
        MacFreeBlock((void**)&pHdr);
        return NULL;
    }

    return (U8*)pHdr;
}


RESULTCODE CcchMgrSchedule(PDCCH_DL_CTX *pPdcchDlCtx,
                           U32 *pAvailableCCE,
                           DCI_CCE_CONTAINER *pDciCceContainer,
                           U32 *pAvailableDlRbCount)
{
    CCCH_QUEUE_NODE *pCcchNode       = NULL;
    DCI_DATA_INFO* pDciData          = NULL;
    DL_DATA_ENTRY_INFO *pDlDataEntry = NULL;
    U8 numRB = 0;

    printf("%s\n",__FUNCTION__);

    while(OsalDequeue(&ccchQueue, (OSAL_PADDRESS *)&pCcchNode) == SUCCESS)
    {
        U16 buffLen = pCcchNode->msgLen;
        U16 headerLen =  0;
        U32 x;
        U32 tbsize;

        (buffLen < 128)?(headerLen = 2):(headerLen = 3);

        tbsize = buffLen + headerLen;
        x = tbsize;

        numRB = determineRBs(CCCH_MCS_INDEX, &x);
        tbsize = x;

        printf("tbsize %d numRB %d\n", tbsize, numRB);

        if (*pAvailableDlRbCount > numRB)
        {
            *pAvailableDlRbCount -= numRB;
            pDciData = CcchMgrAllocCce(pAvailableCCE, pDciCceContainer);
            if (pDciData)
            {
                CcchMgrPrepareDci(pDciData, pCcchNode);

                /* update Pdcch Context */
                pDlDataEntry = &pPdcchDlCtx->dlDataInfoArr[ \
                                   pPdcchDlCtx->countCommonEntry++];

                /* bind Dl data entry with coresponding Dci */
                pDlDataEntry->cceIndex = pDciData->cceIndex;
                pDlDataEntry->dciFormat = DCI_FORMAT_1A;
                pDlDataEntry->entryType = CCCH;

                pDlDataEntry->resourceAlloction.requiredRB = numRB;
                pDlDataEntry->resourceAlloction.mcsIndex = CCCH_MCS_INDEX;

                /* prepare header and concatenate with data */
                pDlDataEntry->dataLen = pCcchNode->msgLen + headerLen;
                pDlDataEntry->pData = CcchMghConcatMacHdr(headerLen,
                                                          pCcchNode->pMsg,
                                                          pCcchNode->msgLen);

                CcchMgrFreeNode(pCcchNode);
                if (pDlDataEntry->pData == NULL)
                {
                    printf("Error in funtion %s (%s:%d)\n",
                        __FUNCTION__, __FILE__, __LINE__);
                    return FAILURE;
                }
            }
            else
            {
               printf("Error in funtion %s (%s:%d)\n",
                   __FUNCTION__, __FILE__, __LINE__);
               CcchMgrFreeNode(pCcchNode);
               return FAILURE;
            }
        }
    }

    return SUCCESS;
}

RESULTCODE CcchMgrClean(void)
{
    return SUCCESS;
}

RESULTCODE CcchMgrInit(void)
{
    OSAL_PADDRESS pMem = NULL;

    /* Tx Ue Queue  */
    if (OsalAllocHeap(&OsalGlobalHeap,
                           &pMem,
                           CCCHQUEUE_SIZE * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        printf("Error in funtion %s (%s:%d)\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }

    if (OsalCreateQueue(&ccchQueue,
                        pMem,
                        CCCHQUEUE_SIZE * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        printf("Error in funtion %s (%s:%d)\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }

    if (OsalAllocHeap(&OsalGlobalHeap, &pMem,
                          (OSAL_SIZE)(sizeof(CCCH_QUEUE_NODE) * MAX_UE_COUNT))
                   != SUCCESS)
    {
        printf("Error in funtion %s (%s:%d)\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }

    if (OsalCreateMemArray(&ccchNodePart,
                           pMem,
                          (OSAL_SIZE)(sizeof(CCCH_QUEUE_NODE) * MAX_UE_COUNT),
                          (OSAL_SIZE) sizeof(CCCH_QUEUE_NODE))
               != SUCCESS)
    {
        printf("Error in funtion %s (%s : %d)\n",
               __FUNCTION__, __FILE__, __LINE__);
        return FAILURE;
    }

    return SUCCESS;
}

#endif /* ENODEB */

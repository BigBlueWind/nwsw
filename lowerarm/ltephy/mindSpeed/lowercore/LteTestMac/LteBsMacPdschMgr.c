/** @file LteBsMacPdcchMgr.c
 *
 * @brief PDCCH handling module
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
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
#ifndef AC_MSPD_TDD
#include "LtePhyL2Api.h"
#else
#include "LtePhyL2Api_TDD.h"
#endif
#include "ltemaccore.h"
//#include "queue.h"

/****************************************************************************
 Private Definitions
 ****************************************************************************/

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

/*
static RESULTCODE PdschMgrProcessDataCh(PDCCH_DL_CTX *pPdcchDlCtx)
{
    // process Data entries
    U8 count = 0;

    count = pPdcchDlCtx->countEntry;
    while(count--)
    {

        // traffic data
    }

    return SUCCESS;
}*/

/*
RESULTCODE PdschMgrProcess(PDCCH_DL_CTX *pPdcchDlCtx)
{
    U8 count                   = 0;
    DL_DATA_ENTRY_INFO *pEntry = NULL;
    DCI_DATA_INFO *pDciData    = NULL;

    pEntry = pPdcchDlCtx->dlDataInfoArr;
    printf("%s\n",__FUNCTION__);

    // process Control entries
    count = pPdcchDlCtx->countCommonEntry;
    while(count--)
    {
        switch (pEntry->entryType)
        {
            case CCCH:
            {
                pDciData =
                    &dciCceInfo[0].dciCceInfo[pEntry->cceIndex].dciCCEInfo;
//              pDciData->resrcAssgmntBitMap = resrcAlloc.resourceAssignmentBitMap;
//              pDciData->resrcAllocType =  resrcAlloc.ratPolicy;
                pDciData->transmissionMode = SINGLEANT;
                pDciData->numTB = 1;
                pDciData->numLayer = 1;
//              pDciData->virtulResrcBlkType = resrcBlkType;
                pDciData->tbInfo[0].modulationType =
                                            pEntry->resourceAlloction.mcsIndex;
                pDciData->pTbData = pEntry->pData;
                pDciData->tbInfo[0].tbSize = pEntry->dataLen;
                pDciData->dciFormat = pEntry->dciFormat;
                pDciData->numOfRBs = pEntry->resourceAlloction.requiredRB;
//              pDciData->rankIndicator = rankInd;

                ++pEntry;
                continue;
            }
            break;
            case BCCH:
            {


            }
            break;
            default:
                printf("Error in funtion %s (%s:%d)\n",
                    __FUNCTION__, __FILE__, __LINE__);
                break;
        }
    }

    PdschMgrProcessDataCh(pPdcchDlCtx);

    return SUCCESS;
}*/

RESULTCODE PdschMgrClean(void)
{

    return SUCCESS;
}

RESULTCODE PdschMgrInit(void)
{

    return SUCCESS;
}


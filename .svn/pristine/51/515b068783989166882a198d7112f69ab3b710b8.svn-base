/** @file LteBsMacUlScheduler.c
 *
 * @brief Implementation of Ul scheduler
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.14 $
 *
 * COPYRIGHT(c) 2008-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <string.h>
#include "LteBsMacUlScheduler.h"
#include "ltemaccore.h"
#include "bsmac.h"
#include "LteMacTestCfgMgr.h"
#include "enbApiHdlr.h"
#include "enbMac2PhyIface.h"

#include "lte_mac_state.h"
#include "lte_vectors.h"


/****************************************************************************
 Private Definitions
 ****************************************************************************/

/****************************************************************************
 Private Types
 ****************************************************************************/

typedef RESULTCODE (*SchedulerCb)(BS_CONTEXT *pBsCtx, PULSUBFRDESC pSubFrame);
typedef RESULTCODE (*RxSduCheckCb)(PRXSDUIND pRxSduInd);
typedef PULSUBFRDESC (*PrevULCtxGetCb)(U32 fn, U32 sfn);

typedef struct SchedulerData
{
    SchedulerCb UlScheduler;
    RxSduCheckCb UlRxSduCheck;
    PrevULCtxGetCb PrevULCtxGet;
} UL_SCHEDULER_CONTEXT;


/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
	//HARQ
RESULTCODE ULRxSduStatusCheckOff(PRXSDUIND pRxSduInd);
static PULSUBFRDESC UlList_FindSubFrm(PRXSDUIND pRxSduInd);
RESULTCODE ULRxSduStatusCheck(PRXSDUIND pRxSduInd);
RESULTCODE ULSubFr_RetransmitionCheck(PULSUBFRDESC pUlSubFrame);
static void UlList_AddHead(PULSUBFRDESC pUlSubFr);
static void UlList_Add2Tail(PULSUBFRDESC pUlSubFr);
static PULSUBFRDESC UlList_GetHead(void);
static PULSUBFRDESC UlList_ReadTail(void);
static PULSUBFRDESC UlList_ReadNplus4(U32 fn, U32 sfn);

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
static UL_SCHEDULER_CONTEXT schedulerUlCtx;


/****************************************************************************
 * Exported functions
 ****************************************************************************/
PULSUBFRDESC LteBsMacUlVectorGet()
{
    return UlList_ReadTail();
}

static PULSUBFRDESC LteBsMacUlCtxGet_HARQ_On(U32 fn, U32 sfn)
{
    return UlList_ReadNplus4(fn, sfn);
}

static PULSUBFRDESC LteBsMacPrevUlCtxGet_HARQ_Off(U32 fn, U32 sfn)
{
    return NULL;
}
PULSUBFRDESC LteBsMacPrevUlCtxGet(U32 fn, U32 sfn)
{
    return schedulerUlCtx.PrevULCtxGet(fn, sfn);
}

/**
 *
 * @param pBsCtx
 * @param pSubFrame
 * @return
 */
RESULTCODE LteBsMacUlScheduler(BS_CONTEXT *pBsCtx, PULSUBFRDESC pSubFrame)
{
    return schedulerUlCtx.UlScheduler(pBsCtx, pSubFrame);
}

RESULTCODE LteBsMacUlSduStatus(PRXSDUIND pRxSduInd)
{
    return schedulerUlCtx.UlRxSduCheck(pRxSduInd);
}


/**
 *
 * @param pBsCtx
 * @param pSubFrame
 * @return
 */
RESULTCODE LteBsMacMainUlScheduler(BS_CONTEXT *pBsCtx, PULSUBFRDESC pSubFrame)
{
    /* iterate on allocation provided to UE */

    return SUCCESS;
}

/**
 *
 * @param pBsCtx
 * @param pSubFrame
 * @return
 */
RESULTCODE LteBsMacUlTestScheduler(BS_CONTEXT *pBsCtx, PULSUBFRDESC pSubFrame)
{

    // Prepare stub for RXVECTOR
    lte_vectors_processing_ul(pSubFrame);
    UlList_Add2Tail(pSubFrame);
    UlList_GetHead();

    //printUlVector(pSubFrame);

    return SUCCESS;
}

RESULTCODE LteBsMacUlTestSchedulerRt(BS_CONTEXT *pBsCtx, PULSUBFRDESC pSubFrame)
{
    // Prepare stub for RXVECTOR
    lte_vectors_processing_ul(pSubFrame);
    UlList_Add2Tail(pSubFrame);
    UlList_GetHead();
    //printUlVector(pSubFrame);
    return SUCCESS;
}
RESULTCODE LteBsMacUlTestSchedulerHARQ(BS_CONTEXT *pBsCtx, PULSUBFRDESC pSubFrame)
{
    RESULTCODE ret = SUCCESS;
    if( ULSubFr_RetransmitionCheck(pSubFrame)== 0)
        lte_vectors_processing_ul(pSubFrame);
    UlList_Add2Tail(pSubFrame);
    return ret;
}

/**
 *
 * @param pBsCtx
 * @return
 */
static RESULTCODE ScheduleUpdate(BS_CONTEXT *pBsCtx)
{
    if (pBsCtx->schedulingSchema == MAIN_SCHEMA)
    {
        schedulerUlCtx.UlScheduler = LteBsMacMainUlScheduler;
    }
    else if (pBsCtx->schedulingSchema == RCT_SCHEMA)
    {
        schedulerUlCtx.UlScheduler = LteBsMacUlTestSchedulerRt;
    }
    if (pBsCtx->HarqEna == HARQ_ON)
    {
        schedulerUlCtx.UlScheduler = LteBsMacUlTestSchedulerHARQ;
        schedulerUlCtx.UlRxSduCheck = ULRxSduStatusCheck;
        schedulerUlCtx.PrevULCtxGet = LteBsMacUlCtxGet_HARQ_On;
		uart_printf("Start HARQ scheme\n");
    }
    return SUCCESS;
}

RESULTCODE LteBsMacUlStart(BS_CONTEXT *pBsCtx, PULSUBFRDESC pSubFrame)
{
    RESULTCODE ret;

    lte_vectors_processing_ul(pSubFrame);
    UlList_AddHead(pSubFrame);
	
    /* Set callback to scheduler function */
    ret = ScheduleUpdate(pBsCtx);

    return ret;
}

/**
 *
 * @param pBsCtx
 * @return
 */
RESULTCODE LteBsMacUlSchedulerReconfig(BS_CONTEXT *pBsCtx)
{
    schedulerUlCtx.UlRxSduCheck = ULRxSduStatusCheckOff;
	schedulerUlCtx.UlScheduler = LteBsMacUlStart;
    schedulerUlCtx.PrevULCtxGet = LteBsMacPrevUlCtxGet_HARQ_Off;

    return SUCCESS;
}

/**
 *
 * @param pBsCtx
 * @return
 */
RESULTCODE LteBsMacUlSchedulerInit(BS_CONTEXT *pBsCtx)
{
    RESULTCODE ret;

    /* Init Sheduler context */
    memset(&schedulerUlCtx, 0, sizeof(UL_SCHEDULER_CONTEXT));
    /* Set callback to scheduler function */
    ret = LteBsMacUlSchedulerReconfig(pBsCtx);
    return ret;
}

/**
 *
 * @param pBsCtx
 * @return
 */
RESULTCODE LteBsMacUlSchedulerClean(BS_CONTEXT *pBsCtx)
{
    return SUCCESS;
}

/**
* 
**/
static PULSUBFRDESC UlList_FindSubFrm(PRXSDUIND pRxSduInd)
{
    PRxSubFrmList temp = pBsMacDesc->UlTail;
    PULSUBFRDESC pUlSubFr = temp->pUlSubFr;
    U32 i; 
    for( i = 0; i < pBsMacDesc->RxSubFrmCount; i++)
    {
        if(pUlSubFr->frameNumber == pRxSduInd->frameNum&&
            pUlSubFr->subframeNumber == pRxSduInd->subFrameNum)
        {
        //	uart_printf("Finded %d %d %d\n",i, pUlSubFr->frameNumber, pUlSubFr->subframeNumber);
            return pUlSubFr;
        }
    
        if(temp->prev)
        {
            temp = temp->prev;
            pUlSubFr = temp->pUlSubFr;
        }
        else
            return NULL;
    }
    return NULL;
}


RESULTCODE ULRxSduStatusCheckOff(PRXSDUIND pRxSduInd)
{
    return SUCCESS;
}

RESULTCODE ULRxSduStatusCheck(PRXSDUIND pRxSduInd)
{
    PULSUBFRDESC pUlSubFrame = UlList_FindSubFrm(pRxSduInd);
    U32 Num;

    if(pUlSubFrame == NULL)
    {
            uart_printf("cann't find the SubFr info for %d:%d\n", pRxSduInd->frameNum, pRxSduInd->subFrameNum);
            return FAILURE;
    }
    if((pRxSduInd->frameNum == 0 &&  pRxSduInd->subFrameNum == 0) ||
    	(pRxSduInd->frameNum == 0 &&  pRxSduInd->subFrameNum == 1))
    {
            pUlSubFrame->frameNumber = 0;
            pUlSubFrame->subframeNumber = 0;
            return SUCCESS;
    }
    if(pRxSduInd->status == 3)
    {
        pUlSubFrame->frameNumber = 0;
        pUlSubFrame->subframeNumber = 0;
        return SUCCESS;
    }
		
    if(pRxSduInd->chanType == PUSCH)
    {
        U8 ChanNum = pUlSubFrame->numberofChannelDescriptors;

        while(ChanNum)
        {
            if(pUlSubFrame->ulCh[ChanNum -1].channelId == pRxSduInd->channelId)
            {
                HARQINFO* harqInfo = &pUlSubFrame->ulCh[ChanNum -1].ulSubChInfo.harqInfo;
                if(pRxSduInd->status != 3)
                {
                    Num = pUlSubFrame->frameNumber * 10 + pUlSubFrame->subframeNumber + 8;
                    pUlSubFrame->frameNumber = Num /10;
                    pUlSubFrame->subframeNumber = Num % 10;
                    switch(harqInfo->rV)
                    {
                        case 3:
                            harqInfo->rV = 1;
                            harqInfo->nDi= 0;
                            break;
                        case 2:
                            harqInfo->rV = 3;
                            harqInfo->nDi= 0;
                            break;
                        case 1:
                            harqInfo->rV = 0;
                            harqInfo->nDi= 1;
                            pUlSubFrame->frameNumber = 0;
                            pUlSubFrame->subframeNumber = 0;
                            break;
                        case 0:
                            harqInfo->rV = 2;
                            harqInfo->nDi= 0;
                            break;
                        default:
                            return FAILURE;
                    }
                    return SUCCESS;

                }
                else
                {
                    pUlSubFrame->frameNumber = 0;
                    pUlSubFrame->subframeNumber = 0;
                    return SUCCESS;
                }
            }
            ChanNum--;
        }
    }
    return SUCCESS;
}
	
 RESULTCODE ULSubFr_RetransmitionCheck(PULSUBFRDESC pUlSubFrame)
 {
     RESULTCODE res = 0;
     PULSUBFRDESC ptmp;

     if(pBsMacDesc->RxSubFrmCount >= 7)
     {
        ptmp = UlList_GetHead();
        if(ptmp)
        {
            if(ptmp->frameNumber == getFrameNum() &&
                ptmp->subframeNumber == getSubFrameNum())
                    return  1;
        }
     }
     return res;
 }

static void UlList_AddHead(PULSUBFRDESC pUlSubFr)
{
    PRxSubFrmList temp = (PRxSubFrmList)malloc(sizeof(RxSubFrmList));

    if(temp)
    {
        temp->next = NULL;
        temp->prev = NULL;
        temp->pUlSubFr = pUlSubFr;
    }
    else
    {
        uart_printf("UlList_AddHead: can't allocate\r\n");
    }

    pBsMacDesc->UlHead = temp;
    pBsMacDesc->UlTail = temp;
    pBsMacDesc->RxSubFrmCount = 0;
}
 
static void UlList_Add2Tail(PULSUBFRDESC pUlSubFr)
{
    PRxSubFrmList temp = (PRxSubFrmList)malloc(sizeof(RxSubFrmList));

    if(temp)
    {
        temp->next = NULL;
        temp->prev = pBsMacDesc->UlTail;
        temp->pUlSubFr = pUlSubFr;
    }
    else
    {
        uart_printf("UlList_Add2Tail: can't allocate\r\n");
    }
    pBsMacDesc->UlTail->next = temp;
    pBsMacDesc->UlTail = temp;
    pBsMacDesc->RxSubFrmCount++;
}
static PULSUBFRDESC UlList_GetHead()
{
    PRxSubFrmList temp = pBsMacDesc->UlHead;
    PULSUBFRDESC pUlSubFr = NULL;

    if(temp)
    {
        pUlSubFr = temp->pUlSubFr;
        pBsMacDesc->UlHead = pBsMacDesc->UlHead->next;
        pBsMacDesc->UlHead->prev = NULL;
        free(temp);
    }
    else
    {
        uart_printf("UlList_GetHead: zero Head\r\n");
    }
    pBsMacDesc->RxSubFrmCount--;
    return pUlSubFr;
 
}

static PULSUBFRDESC UlList_ReadTail()
{
    return pBsMacDesc->UlTail->pUlSubFr;
}

static PULSUBFRDESC UlList_ReadNplus4(U32 fn, U32 sfn)
{
    U32 Num = fn * 10 + sfn;
    PULSUBFRDESC UlCtx;
    U8 i = 4;
    PRxSubFrmList Curr = NULL;

    if(Num > 4)
    {
        //Num -= 4;
        //fn = Num / 10;
        //sfn = Num % 10;
    }
    else
        return NULL;

    Curr = pBsMacDesc->UlTail;

    while(i--)
    {
        if(Curr == NULL)
            return NULL;
        UlCtx = Curr->pUlSubFr;
        Curr = Curr->prev;
    }
    return UlCtx;
}


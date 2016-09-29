/** @file LteBsMacUlScheduler.c
 *
 * @brief Implementation of Ul scheduler
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.38 $
 *
 * COPYRIGHT(c) 2008-2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef ENODEB

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "osal.h"
#include "basetypes.h"
#include "resultcodes.h"
#include "apidefs.h"
#include "ltemaccore.h"
#include "bsmac.h"
#include "fdxmac.h"
#include "LteMacTestCfgMgr.h"
//#include "ltephy.h"

#include "lte_vectors.h"

#ifdef AC_MSPD_TDD
#include "MgrCfgTdd.h"
#endif /*AC_MSPD_TDD*/
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

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
//static RXIODESC RxIo;
static UL_SCHEDULER_CONTEXT schedulerUlCtx;


/* TODO: Remove this */
extern U8 InputBytesTest4[1753];

/****************************************************************************
 * Exported functions
 ****************************************************************************/
extern RESULTCODE PhySendData(HANDLE h, PTR pMsg, OSAL_SIZE msgSize);

/**
 *
 * @param pRxIo
 * @param pMac
 * @param index
 */
#if 0
static void MacBsRxDataInit(PRXIODESC pRxIo, PLTEMACDESC pMac, U32 index)
{
    int i;

    // Copy G info and rx bytes for each sub-channel
    for (i = 0; i < 2; i++)
    {
        pRxIo->chentry[index].fecInOutBits[i]= pMac->tCh[index].fecInOutBits[i];
        pRxIo->chentry[index].numBytes[i] = pMac->tCh[index].tbBytes[i];
    }
    pRxIo->chentry[index].numRxAnts = 16;
}
#endif

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
RESULTCODE LteBsMacFdxUlScheduler(BS_CONTEXT *pBsCtx, PULSUBFRDESC pSubFrame)
{
    U32 k;
    U8* pUlInDat = &InputBytesTest4[0];
    U32  inUlDatSz = 1753;

    PTSTPDESC pTstpbsUl = &pBsCtx->tstpbsUl;

    // Next run the equivalent to mac_bs_tx() in Matlab in order to
    // setup the control,reference signals, synchronization signals and
    // the broadcast channel used resources in a global MAC structure
    LteFdxMacInit(&FdxMac, &pBsCtx->initParm);
	// TODO: warning C4133: 'function' : incompatible types - from 'PULSUBFRDESC' to 'PDLSUBFRDESC'
    MacBsTx(pSubFrame, &FdxMac);

    for (k = 0; k < pTstpbsUl->numCtlCh; k++)
    {
        // Read input file and allocate RB's in pSubFrame
		// TODO: warning C4133: 'function' : incompatible types - from 'PULSUBFRDESC' to 'PDLSUBFRDESC'
        MacBsReallocData(pSubFrame, &FdxMac, k, pUlInDat, inUlDatSz, 1 /*UL*/);
        // Copy Info from the Mac Channel for FEC into the Phy struct
        // Now the Phy structure is located in the lower ARM so we need
        // to issue a debug command until the MAC and upper layers reside
        // in the Upper Arm with access to share memory to the lower ARM

//        MacBsRxDataInit(&RxIo, &FdxMac, k);

        // Initialize the demodulation reference sequence for the entire
        // frame
        // Now send the information to the Lower Arm so it could initialize
        // correctly its database (In the final product the control channel
        // info for the Uplink Grant should have this information and the
        // MAC or RRC layer would provide the information to the PHY

//        PhySendData(NULL, &RxIo, sizeof(RXIODESC));
    }
    for (k = pTstpbsUl->numCtlCh;
         k < (U32)(pTstpbsUl->numDataCh + pTstpbsUl->numCtlCh);
         k++)
    {
   		// TODO: warning C4133: 'function' : incompatible types - from 'PULSUBFRDESC' to 'PDLSUBFRDESC'
        MacBsReallocData(pSubFrame, &FdxMac, k, pUlInDat, inUlDatSz, 1 /*UL*/);
        // Read input file and allocate RB's in pSubFrame
        // Copy Info from the Mac Channel for FEC into the Phy struct
        // Now the Phy structure is located in the lower ARM so we need
        // to issue a debug command until the MAC and upper layers reside
        // in the Upper Arm with access to share memory to the lower ARM

//        MacBsRxDataInit(&RxIo, &FdxMac, k);

        // Initialize the demodulation reference sequence for the entire
        // frame
        // Now send the information to the Lower Arm so it could initialize
        // correctly its database (In the final product the control channel
        // info for the Uplink Grant should have this information and the
        // MAC or RRC layer would provide the information to the PHY

//        PhySendData(NULL, &RxIo, sizeof(RXIODESC));
    }

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
    RESULTCODE ret;

    /* Prepare stub for RXVECTOR */
    ret = lte_vectors_processing_ul(pSubFrame);

    //printUlVector(pSubFrame);

    return ret;
}

RESULTCODE LteBsMacUlTestSchedulerRt(BS_CONTEXT *pBsCtx, PULSUBFRDESC pSubFrame)
{
    RESULTCODE ret;

    /* Prepare stub for RXVECTOR */
    ret = lte_vectors_processing_ul(pSubFrame);

    //printUlVector(pSubFrame);

    return ret;
}
#ifdef AC_MSPD_TDD
RESULTCODE ULSchedulerTdd(BS_CONTEXT *pBsCtx, PULSUBFRDESC pSubFrame)
{
    RESULTCODE ret;

    /* Prepare stub for RXVECTOR */
    ret = TestMgrProcessConfigUlTdd(pSubFrame);

    //printUlVector(pSubFrame);

    return ret;
}
#endif
/**
 *
 * @param pBsCtx
 * @return
 */
RESULTCODE LteBsMacUlSchedulerReconfig(BS_CONTEXT *pBsCtx)
{
#ifdef AC_MSPD_TDD
    schedulerUlCtx.UlScheduler = ULSchedulerTdd;
     return SUCCESS;
#endif
    if (pBsCtx->schedulingSchema == FDX_SCHEMA)
    {
        schedulerUlCtx.UlScheduler = LteBsMacFdxUlScheduler;
    }
    else if (pBsCtx->schedulingSchema == MAIN_SCHEMA)
    {
        printf("MAIN_SCHEMA doesn't support\n");
//        schedulerUlCtx.UlScheduler = LteBsMacMainUlScheduler;
    }
    else if (pBsCtx->schedulingSchema == RCT_SCHEMA)
    {
        if(MacRtMode == MACMODE_NRT)
            schedulerUlCtx.UlScheduler = LteBsMacUlTestScheduler;
        else
            schedulerUlCtx.UlScheduler = LteBsMacUlTestSchedulerRt;
    }
    else
        return FAILURE;

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

PULSUBFRDESC UlList_ReadTail(void)
{
    return pBsMacDesc->UlTail->pUlSubFr;
}

PULSUBFRDESC LteBsMacUlVectorGet(void)
{
    return UlList_ReadTail();
}

#endif // ENODEB

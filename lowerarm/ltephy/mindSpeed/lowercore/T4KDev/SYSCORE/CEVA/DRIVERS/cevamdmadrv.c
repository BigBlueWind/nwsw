//-------------------------------------------------------------------------------------------
/** @file cevamdmadrv.c
 *
 * @brief MDMA driver developed for CEVA code 
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "cevamdmadrv.h"
#include "cevadrvcfg.h"

CEVA_MDMA_DRV_CTX ceva_mdma_drv_ctx[3];

CEVA_MDMA_DRV_CTX * MdmaDrvGetCtx (UINT32 MdmaID) CEVAEXTERN;
void MdmaDrvGetLock (CEVA_MDMA_DRV_CTX * pCtx) CEVAEXTERN;
void MdmaDrvReleaseLock (CEVA_MDMA_DRV_CTX * pCtx) CEVAEXTERN;
void MdmaDrvAddToInactive (PMDMA_LIST_INF pListInfPtr, PMDMA_FDESC pFDesc) CEVAEXTERN;
MdmaCbDoneType MdmaDrvGetCbDone (UINT32 nCbDone) CEVAEXTERN;
void MdmaDrvProcCbDone(CEVA_MDMA_DRV_CTX * pCtx, MdmaCbDoneType CbDoneExclude) CEVAEXTERN;

CEVA_MDMA_DRV_CTX * MdmaDrvGetCtx (UINT32 MdmaID)
{
    if (MdmaID < MDMA_HW_MAX)
        return &ceva_mdma_drv_ctx[MdmaID];

    return NULL;
}

void MdmaDrvGetLock (CEVA_MDMA_DRV_CTX * pCtx)
{
    if (pCtx == NULL)
        return;

    if (pCtx->HwSema == 0)
        return;

    while (REG32(pCtx->HwSema) != 0)
    {
    }
}

void MdmaDrvReleaseLock (CEVA_MDMA_DRV_CTX * pCtx)
{
    if (pCtx == NULL)
        return;

    if (pCtx->HwSema == 0)
        return;

    REG32(pCtx->HwSema) = 0;
}

UINT32  MdmaDrvOpen(UINT32 MdmaID)
{
    PCEVA_DRV_CFG pCfg = CEVA_GET_DRV_CFG();
    CEVA_MDMA_DRV_CFG * pMdmaCfg;
    CEVA_MDMA_DRV_CTX * pCtx;
    MDMA_CTRL_REGS *    MdmaCtrlRegs;

    switch(MdmaID)
    {
        case MDMA_HW_SYS:
            pMdmaCfg = &pCfg->SysMdmaCfg;
            MdmaCtrlRegs = (MDMA_CTRL_REGS *)0xFE400000;
            break;

        case MDMA_HW_SPU:
            pMdmaCfg = &pCfg->SpuMdmaCfg;
            MdmaCtrlRegs = (MDMA_CTRL_REGS *)0xFD010000;
            break;

        case MDMA_HW_RAD:
            pMdmaCfg = &pCfg->RadMdmaCfg;
            MdmaCtrlRegs = (MDMA_CTRL_REGS *)0xFE8C0000;
            break;

        default:
            return MDMA_CEVA_RC_DEV_ERROR;
    }

    pCtx = MdmaDrvGetCtx(MdmaID);

    memset(pCtx, 0, sizeof (*pCtx));

    pCtx->HwSema        = pMdmaCfg->HwSema;
    pCtx->ListInfPtr    = (PMDMA_LIST_INF)pMdmaCfg->ListIntPtr;
    pCtx->PoolPtr       = (PMXPOOL)pMdmaCfg->PoolPtr;
    pCtx->MdmaCtrlRegs  = MdmaCtrlRegs;
    pCtx->OwnerCpu      = pMdmaCfg->OwnerCpu;

    if (pCtx->PoolPtr == NULL || pCtx->ListInfPtr == NULL)
        return MDMA_CEVA_RC_CFG_ERROR;

    pMdmaCfg->UsedMask |= (1 << MxGetCpuID ());

    return MDMA_CEVA_RC_OK;
}

UINT32  MdmaDrvClose(UINT32 MdmaID)
{
    PCEVA_DRV_CFG pCfg = CEVA_GET_DRV_CFG();
    CEVA_MDMA_DRV_CFG * pMdmaCfg;
    CEVA_MDMA_DRV_CTX * pCtx;

    switch(MdmaID)
    {
        case MDMA_HW_SYS:
            pMdmaCfg = &pCfg->SysMdmaCfg;
            break;

        case MDMA_HW_SPU:
            pMdmaCfg = &pCfg->SpuMdmaCfg;
            break;

        case MDMA_HW_RAD:
            pMdmaCfg = &pCfg->RadMdmaCfg;
            break;

        default:
            return MDMA_CEVA_RC_DEV_ERROR;
    }

    pCtx = MdmaDrvGetCtx(MdmaID);
    memset(pCtx, 0, sizeof (*pCtx));

    pMdmaCfg->UsedMask &= ~(1 << MxGetCpuID());

    return MDMA_CEVA_RC_OK;
}


void MdmaDrvSetCopy (PMDMA_FDESC pMdma)
{
    MDMA_HW * pMdmaHw = MDMA_GET_HW(pMdma);

    // COPY and BOffset = 1

    pMdmaHw->FControl = MDMA_OP_COPY | (1 << 4);
    pMdmaHw->FStatus0 = 0;
    pMdmaHw->FStatus1 = 0;
}

void MdmaDrvSetCRC (PMDMA_FDESC pMdma, UINT32 CrcInit, UINT32 PolyValue, UINT32 PolyOrder)
{
    MDMA_HW * pMdmaHw = MDMA_GET_HW(pMdma);

    // 4G SCR and BOffset = 1

    pMdmaHw->FControl = MDMA_OP_CRC | (1 << 4) | (((PolyValue & 0xFFFF)) << 16);

    pMdmaHw->FStatus0 = CrcInit;
    pMdmaHw->FStatus1 = ((PolyValue >> 16) & 0xFFFF) | ((PolyOrder & 0x7) << 16);
}

void MdmaDrvSet4GSCR (PMDMA_FDESC pMdma, UINT32 cInit0, UINT32 cInit1, UINT32 PackMode, UINT32 nScrOpt)
{
    MDMA_HW * pMdmaHw = MDMA_GET_HW(pMdma);

    // 4G SCR and BOffset = 1

    pMdmaHw->FControl = MDMA_OP_4SCR | (1 << 4);
    pMdmaHw->FStatus0 = cInit0 & 0x7FFFFFFF;
    pMdmaHw->FStatus1 = cInit1 & 0x7FFFFFFF;
    pMdmaHw->FControl |= ((nScrOpt  & 0x3) << 17) | ((PackMode & 0x1) << 16);
}

void MdmaDrvSet4GDSCR (PMDMA_FDESC pMdma, UINT32 cInit0, UINT32 cInit1, UINT32 nScrOpt)
{
    MDMA_HW * pMdmaHw = MDMA_GET_HW(pMdma);

    // 4G DESCR and BOffset = 1

    pMdmaHw->FControl = MDMA_OP_4DSC | (1 << 4);
    pMdmaHw->FStatus0 = cInit0 & 0x7FFFFFFF;
    pMdmaHw->FStatus1 = cInit1 & 0x7FFFFFFF;
    pMdmaHw->FControl |= ((nScrOpt  & 0x3) << 17);
}

void MdmaDrvSetMultiple4GDSCR (PMDMA_FDESC pMdma, UINT32 cInit0, UINT32 cInit1, UINT32 nScrOpt, UINT32 numDesc)
{
    MDMA_HW * pMdmaHw = MDMA_GET_HW(pMdma);

    // 4G DESCR and BOffset = 1

    pMdmaHw->FControl = MDMA_OP_4DSC | (numDesc << 4);
    pMdmaHw->FStatus0 = cInit0 & 0x7FFFFFFF;
    pMdmaHw->FStatus1 = cInit1 & 0x7FFFFFFF;
    pMdmaHw->FControl |= ((nScrOpt  & 0x3) << 17);
}

void MdmaDrvSetData(PMDMA_FDESC pFDesc, LPVOID pInData, UINT32 InDataSize, LPVOID pOutData, UINT32 OutDataSize)
{
    _ASSERT_PTR(pFDesc);

    pFDesc->IoBdesc[1].BPtr = (UINT32)pInData;
    pFDesc->IoBdesc[1].BCtrl = BCTRL_BLAST_MASK | InDataSize;

    pFDesc->IoBdesc[0].BPtr = (UINT32)pOutData;
    pFDesc->IoBdesc[0].BCtrl = BCTRL_BLAST_MASK | OutDataSize;
}

PMDMA_FDESC MdmaDrvAllocDesc (UINT32 MdmaID, UINT32 nNum)
{
    CEVA_MDMA_DRV_CTX * pMdmaCtx = MdmaDrvGetCtx(MdmaID);
    PMDMA_FDESC pFDesc, pFDescPrev = NULL, pFDescHead = NULL;
    UINT32 i;

    MXPOOL * pPool;
    pPool = pMdmaCtx->PoolPtr;

    // Lock is needed to make possible to allocate  
    // fdesc blocks from more than one CEVA by using the same pool
    // create by the ARM MDMA driver 

    #ifdef MDMA_CEVA_ALLOC_SYNC
    MdmaDrvGetLock(pMdmaCtx);
    #endif

    for (i = 0; i < nNum; i++)
    {
        pFDesc = (PMDMA_FDESC)CSAPoolAlloc (pPool);

        if (pFDesc == NULL)
        {
            while (pFDescHead != NULL)
            {
                pFDescPrev = pFDescHead;
                pFDescHead = pFDescPrev->Next;

                CSAPoolFree (pPool, pFDesc);
            }

            #ifdef MDMA_CEVA_ALLOC_SYNC
            MdmaDrvReleaseLock(pMdmaCtx);
            #endif
            
            return NULL;
        }

        pFDesc->OwnerCpuID = 0xFFFF;
        ((PUINT32)&pFDesc->HwContext)[0] = 0; // IRQE = 0;
        ((PUINT32)&pFDesc->HwContext)[2] = 0; // StatusComp = 0;

        pFDesc->Next        = NULL;
        pFDesc->CbDone      = NULL;
        pFDesc->CbContext   = NULL;

        if (pFDescPrev != NULL)
        {
            pFDescPrev->Next = pFDesc;
        }
        else
        {
            pFDescHead = pFDesc;
        }
        
        pFDescPrev = pFDesc;
        
    }

    #ifdef MDMA_CEVA_ALLOC_SYNC
    MdmaDrvReleaseLock(pMdmaCtx);
    #endif
    
    return pFDescHead;
}

UINT32 MdmaDrvSetLastDesc(PMDMA_FDESC pFDesc, void (*CbDone)(LPVOID), LPVOID CbContext)
{
    PMDMA_FDESC p = pFDesc;

    while (p != NULL)
    {
        p->HwContext.IRQEN = 0;
        p->HwContext.StatusComp = 0;

        // Specifics of CEVA compiler, 
        // the code is needed here,
        // not in 'while' header
        
        if (p->Next == NULL)
            break;
        
        p = p->Next;
    }

    p->HwContext.IRQEN  = 1;
    p->CbDone           = (MdmaCbDoneType)(((UINT32)CbDone) | MDMA_CEVA_CB_DONE_PROC);
    p->CbContext        = CbContext;

    return 0;
}

UINT32 MdmaDrvSetLastDescArm(PMDMA_FDESC pFDesc, void (*CbDone)(LPVOID), LPVOID CbContext)
{
    PMDMA_FDESC p = pFDesc;

    while (p != NULL)
    {
        p->HwContext.IRQEN = 0;
        p->HwContext.StatusComp = 0;

        // Specifics of CEVA compiler, 
        // the code is needed here,
        // not in 'while' header
        
        if (p->Next == NULL)
            break;
        
        p = p->Next;
    }

    p->HwContext.IRQEN  = 1;
    p->CbDone           = CbDone;
    p->CbContext        = CbContext;

    return 0;
}

void MdmaDrvAddToInactive (PMDMA_LIST_INF pListInfPtr, PMDMA_FDESC pFDesc)
{
    PMDMA_FDESC pTmp;

    pTmp = pListInfPtr->pFDescInactive;

    if (pTmp == NULL)
    {
        pListInfPtr->pFDescInactive = pFDesc;
    }
    else
    {
        while (pTmp->Next != NULL)
            pTmp = pTmp->Next;

        pTmp->Next = pFDesc;
    }
}

UINT32 MdmaDrvRun(UINT32 MdmaID, PMDMA_FDESC pFDesc)
{
    return MdmaDrvRunQ(MdmaID, pFDesc, 0);
}

UINT32 MdmaDrvRunQ(UINT32 MdmaID, PMDMA_FDESC pFDesc, UINT32 nNumUs)
{
    CEVA_MDMA_DRV_CTX * pCtx = MdmaDrvGetCtx(MdmaID);
    MDMA_CTRL_REGS *pMdmaCtrlRegs;

    if (pCtx == NULL)
        return MDMA_CEVA_RC_DEV_ERROR;

    if (pFDesc == NULL)
        return MDMA_CEVA_RC_PARAM_ERROR;

    MdmaDrvGetLock(pCtx);
    MdmaDrvAddToInactive(pCtx->ListInfPtr, pFDesc);

    if (pCtx->ListInfPtr->pFDescActive == NULL)
    {
        pMdmaCtrlRegs = pCtx->MdmaCtrlRegs;
        pCtx->ListInfPtr->HWBusy = 1;

        pFDesc = pCtx->ListInfPtr->pFDescInactive;
        pCtx->ListInfPtr->pFDescActive   = pFDesc;
        pCtx->ListInfPtr->pFDescInactive = NULL;

        pCtx->ListInfPtr->ActiveTotalTicks = (nNumUs*150);
        pCtx->ListInfPtr->StartTimeTicks   = MxGetTicks ();

        pMdmaCtrlRegs->DMAOutCtrl.HeadFDesc = (UINT32)pFDesc;
        pMdmaCtrlRegs->DMAInCtrl.HeadFDesc = (UINT32)pFDesc;
    }
    else
    {
        pCtx->ListInfPtr->InactiveTotalTicks += (nNumUs*150);
    }

    MdmaDrvReleaseLock(pCtx);
    return MDMA_CEVA_RC_OK;
}

MdmaCbDoneType MdmaDrvGetCbDone (UINT32 nCbDone)
{
    nCbDone &= ~MDMA_CEVA_CB_DONE_PROC;
    return (MdmaCbDoneType)nCbDone;
}

void MdmaDrvProcCbDone(CEVA_MDMA_DRV_CTX * pCtx, MdmaCbDoneType CbDoneExclude)
{
    PMDMA_LIST_INF pInf = pCtx->ListInfPtr;
    MdmaCbDoneType cbDone;

    while (pInf->CbDoneGet != pInf->CbDonePut)
    {
        cbDone = MdmaDrvGetCbDone(pInf->CbDone[pInf->CbDoneGet]);

        if (cbDone != NULL && cbDone != CbDoneExclude)
        {
            cbDone(pInf->CbDonePtr[pInf->CbDoneGet]);
        }

        pInf->CbDoneGet ++;
        
        if (pInf->CbDoneGet >= MAX_CEVA_CB_DONE_NUM)
            pInf->CbDoneGet = 0;
    }
}

UINT32  MdmaDrvWaitCompletion(UINT32 MdmaID, PMDMA_FDESC pFDesc)
{
    CEVA_MDMA_DRV_CTX * pCtx = MdmaDrvGetCtx(MdmaID);
    UINT32 bHWBusy;
    MdmaCbDoneType CbDone;

    if (pCtx == NULL)
        return MDMA_CEVA_RC_DEV_ERROR;

    // In thise case we need to wait only one 
    // MDMA descriptor, other scheduled descriptors 
    // do not matter for the application 

    if (pFDesc != NULL)
    {
        while (pFDesc->HwContext.StatusComp == 0);

        MdmaDrvGetLock(pCtx);
        
        CbDone = MdmaDrvGetCbDone((UINT32)pFDesc->CbDone);
        pFDesc->CbDone = NULL;  // To prevent putting this CB by ARM to cbDone queue
        
        MdmaDrvReleaseLock(pCtx);

        if (CbDone != NULL)
            CbDone(pFDesc->CbContext);

        MdmaDrvProcCbDone (pCtx, CbDone);

        return MDMA_CEVA_RC_OK;
    }

    // In this case we need to wait completion of all 
    // scheduled descriptors 
    // NOTE! Scheduled by both CEVA and ARM code 

    while (1)
    {
        MdmaDrvGetLock(pCtx);

        bHWBusy = pCtx->ListInfPtr->HWBusy;

        MdmaDrvReleaseLock(pCtx);

        if (bHWBusy == 0)
            break;
    }

    MdmaDrvProcCbDone (pCtx, NULL);
    return MDMA_CEVA_RC_OK;
}


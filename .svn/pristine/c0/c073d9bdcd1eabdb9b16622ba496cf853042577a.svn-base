//-------------------------------------------------------------------------------------------
/** @file cevafecdrv.c
 *
 * @brief FEC-DL/UL driver developed for CEVA code 
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "cevafecdrv.h"
#include "cevadrvcfg.h"

CEVA_FECDL_DRV_CTX ceva_fec_dl_drv_ctx;
CEVA_FECUL_DRV_CTX ceva_fec_ul_drv_ctx;

void FecDlDrvGetLock (CEVA_FECDL_DRV_CTX * pCtx) CEVAEXTERN;
void FecUlDrvGetLock (CEVA_FECUL_DRV_CTX * pCtx) CEVAEXTERN;
void FecDlDrvReleaseLock (CEVA_FECDL_DRV_CTX * pCtx) CEVAEXTERN;
void FecUlDrvReleaseLock (CEVA_FECUL_DRV_CTX * pCtx)  CEVAEXTERN;
void FecDlDrvAddToInactive (PFECDL_LIST_INF pListInfPtr, PFEC_DL_FDESC pFDesc)  CEVAEXTERN;
FecCbDoneType FecDlDrvGetCbDone (UINT32 nCbDone)  CEVAEXTERN;
void FecDlDrvProcActiveList(CEVA_FECDL_DRV_CTX * pCtx)  CEVAEXTERN;
void FecDlDrvRunInactiveList(CEVA_FECDL_DRV_CTX * pCtx)  CEVAEXTERN;
void FecDlDrvProcCbDone(CEVA_FECDL_DRV_CTX * pCtx, FecCbDoneType CbDoneExclude)  CEVAEXTERN;

void FecUlDrvAddToInactive (PFECUL_LIST_INF pListInfPtr, PFEC_UL_FDESC pFDesc) CEVAEXTERN;
FecCbDoneType FecUlDrvGetCbDone (UINT32 nCbDone) CEVAEXTERN;
void FecUlDrvProcActiveList(CEVA_FECUL_DRV_CTX * pCtx) CEVAEXTERN;
void FecUlDrvRunInactiveList(CEVA_FECUL_DRV_CTX * pCtx) CEVAEXTERN;
void FecUlDrvProcCbDone(CEVA_FECUL_DRV_CTX * pCtx, FecCbDoneType CbDoneExclude) CEVAEXTERN;

CEVA_FECDL_DRV_CTX * FecDlDrvGetCtx (void)
{
    return &ceva_fec_dl_drv_ctx;
}

CEVA_FECUL_DRV_CTX * FecUlDrvGetCtx (void)
{
    return &ceva_fec_ul_drv_ctx;
}

void FecDlDrvGetLock (CEVA_FECDL_DRV_CTX * pCtx)
{
    if (pCtx == NULL)
        return;

    if (pCtx->HwSema == 0)
        return;

    while (REG32(pCtx->HwSema) != 0)
    {
    }
}

void FecUlDrvGetLock (CEVA_FECUL_DRV_CTX * pCtx)
{
    if (pCtx == NULL)
        return;

    if (pCtx->HwSema == 0)
        return;

    while (REG32(pCtx->HwSema) != 0)
    {
    }
}

void FecDlDrvReleaseLock (CEVA_FECDL_DRV_CTX * pCtx)
{
    if (pCtx == NULL)
        return;

    if (pCtx->HwSema == 0)
        return;

    REG32(pCtx->HwSema) = 0;
}

void FecUlDrvReleaseLock (CEVA_FECUL_DRV_CTX * pCtx)
{
    if (pCtx == NULL)
        return;

    if (pCtx->HwSema == 0)
        return;

    REG32(pCtx->HwSema) = 0;
}

UINT32  FecDlDrvOpen(void)
{
    PCEVA_DRV_CFG       pCfg = CEVA_GET_DRV_CFG();
    CEVA_FEC_DRV_CFG *  pFecCfg;
    CEVA_FECDL_DRV_CTX* pCtx;

    pFecCfg = &pCfg->FecDLCfg;

    pCtx = FecDlDrvGetCtx();

    memset(pCtx, 0, sizeof (*pCtx));

    pCtx->HwSema        = pFecCfg->HwSema;
    pCtx->ListInfPtr    = (PFECDL_LIST_INF)pFecCfg->ListIntPtr;
    pCtx->PoolPtr       = (PMXPOOL)pFecCfg->PoolPtr;
    pCtx->FecCtrlRegsPtr= (FEC_CTRL_REGS *)0xFE410000;
    pCtx->OwnerCpu      = pFecCfg->OwnerCpu;

#if 0
    uart_printf ("   Open FEC-DL\r\n");
    uart_printf ("     HwSema: 0x%x\r\n", pCtx->HwSema);
    uart_printf ("    ListInf: 0x%x\r\n", pCtx->ListInfPtr);
    uart_printf ("    PoolPtr: 0x%x\r\n", pCtx->PoolPtr);
    uart_printf ("       Regs: 0x%x\r\n", pCtx->FecCtrlRegsPtr);
    uart_printf ("      Owner: 0x%x\r\n", pCtx->OwnerCpu);
#endif

    if (pCtx->PoolPtr == NULL || pCtx->ListInfPtr == NULL)
        return FEC_CEVA_RC_CFG_ERROR;

    pFecCfg->UsedMask |= (1 << MxGetCpuID ());

    return FEC_CEVA_RC_OK;
}

UINT32  FecDlDrvClose(void)
{
    PCEVA_DRV_CFG pCfg = CEVA_GET_DRV_CFG();
    CEVA_FEC_DRV_CFG * pFecCfg;
    CEVA_FECDL_DRV_CTX * pCtx;

    pFecCfg = &pCfg->FecDLCfg;

    pCtx = FecDlDrvGetCtx();
    memset(pCtx, 0, sizeof (*pCtx));

    pFecCfg->UsedMask &= ~(1 << MxGetCpuID());

    return FEC_CEVA_RC_OK;
}

UINT32 FecDlDrvSetHwCtx(PFEC_DL_FDESC pFDesc, DLHWCTX HwContext)
{
    _ASSERT_PTR(pFDesc);

    HwContext.IRQEN = 0;
    HwContext.BOffset = 1;
    HwContext.StatusComp = 0;

    pFDesc->HwContext = HwContext;

    return MX_OK;
}

PFEC_DL_FDESC FecDlDrvAllocDesc (UINT32 nNum)
{
    CEVA_FECDL_DRV_CTX * pCtx = FecDlDrvGetCtx();
    PFEC_DL_FDESC pFDesc, pFDescPrev = NULL, pFDescHead = NULL;
    UINT32 i;

    MXPOOL * pPool;
    pPool = pCtx->PoolPtr;

    // Lock is needed to make possible to allocate
    // fdesc blocks from more than one CEVA by using the same pool
    // create by the ARM FEC-DL driver 

    #if FEC_CEVA_ALLOC_SYNC
    FecDlDrvGetLock(pCtx);
    #endif

    for (i = 0; i < nNum; i++)
    {
        pFDesc = (PFEC_DL_FDESC)CSAPoolAlloc (pPool);

        if (pFDesc == NULL)
        {
            while (pFDescHead != NULL)
            {
                pFDescPrev = pFDescHead;
                pFDescHead = (PFEC_DL_FDESC)pFDescPrev->Next;

                CSAPoolFree (pPool, pFDesc);
            }

            #if FEC_CEVA_ALLOC_SYNC
            FecDlDrvReleaseLock(pCtx);
            #endif
            
            return NULL;
        }

        pFDesc->OwnerCpuID = 0xFFFF;
        
        ((PUINT32)&pFDesc->HwContext)[0] = 0; // disable IRQ
        ((PUINT32)&pFDesc->HwContext)[1] = 0;
        ((PUINT32)&pFDesc->HwContext)[2] = 0; // StatusComp

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

    #if FEC_CEVA_ALLOC_SYNC
    FecDlDrvReleaseLock(pCtx);
    #endif
    
    return pFDescHead;
}

UINT32 FecDlDrvSetLastDesc(PFEC_DL_FDESC pFDesc, FecCbDoneType cbDone, LPVOID CbContext)
{
    PFEC_DL_FDESC p = pFDesc;
    CEVA_FECDL_DRV_CTX * pCtx = FecDlDrvGetCtx();

    if (p == NULL)
        return FEC_CEVA_RC_PARAM_ERROR;

    while (p != NULL)
    {
        // Specifics of CEVA compiler, 
        // the code is needed here,
        // not in 'while' header
        
        if (p->Next == NULL)
            break;
    
        p = (PFEC_DL_FDESC)p->Next;
    }

    p->HwContext.IRQEN = !pCtx->OwnerCpu ? 1 : 0; // enable/disable IRQ generating to ARM processor
    
    p->CbDone = (FecCbDoneType)(((UINT32)cbDone) | FEC_CEVA_CB_DONE_PROC);
    p->CbContext = CbContext;

    return MX_OK;
}

UINT32 FecDlDrvSetLastDescArm(PFEC_DL_FDESC pFDesc, FecCbDoneType cbDone, LPVOID CbContext)
{
    PFEC_DL_FDESC p = pFDesc;
    CEVA_FECDL_DRV_CTX * pCtx = FecDlDrvGetCtx();

    if (p == NULL)
        return FEC_CEVA_RC_PARAM_ERROR;

    while (p != NULL)
    {
        // Specifics of CEVA compiler, 
        // the code is needed here,
        // not in 'while' header
        
        if (p->Next == NULL)
            break;
    
        p = (PFEC_DL_FDESC)p->Next;
    }

    p->HwContext.IRQEN = !pCtx->OwnerCpu ? 1 : 0; // enable/disable IRQ generating to ARM processor
    
    p->CbDone    = cbDone;
    p->CbContext = CbContext;

    return MX_OK;
}


UINT32 FecDlDrvSetData(PFEC_DL_FDESC pFDesc, LPVOID pInData, UINT32 InDataSize, LPVOID pOutData, UINT32 OutDataSize)
{
    if (pFDesc == 0)
        return FEC_CEVA_RC_PARAM_ERROR;

    pFDesc->IoBdesc[1].BPtr = (UINT32)pInData;
    pFDesc->IoBdesc[1].BCtrl = BCTRL_BLAST_MASK | InDataSize;
    pFDesc->IoBdesc[0].BPtr = (UINT32)pOutData;
    pFDesc->IoBdesc[0].BCtrl = BCTRL_BLAST_MASK | OutDataSize;
    
    return 0;
}

void FecDlDrvAddToInactive (PFECDL_LIST_INF pListInfPtr, PFEC_DL_FDESC pFDesc)
{
    PFEC_DL_FDESC pTmp;

    pTmp = pListInfPtr->pFDescInactive;

    if (pTmp == NULL)
    {
        pListInfPtr->pFDescInactive = pFDesc;
    }
    else
    {
        while (pTmp->Next != NULL)
            pTmp = (PFEC_DL_FDESC)pTmp->Next;

        pTmp->Next = pFDesc;
    }
}

FecCbDoneType FecDlDrvGetCbDone (UINT32 nCbDone)
{
    nCbDone &= ~FEC_CEVA_CB_DONE_PROC;
    return (FecCbDoneType)nCbDone;
}

void FecDlDrvProcActiveList(CEVA_FECDL_DRV_CTX * pCtx)
{
    PFEC_DL_FDESC pFDesc;
    FecCbDoneType cbDone;

    while ((pFDesc = (PFEC_DL_FDESC)pCtx->ListInfPtr->pFDescActive) != NULL)
    {
        if (pFDesc->HwContext.StatusComp == 0)
            break;

        cbDone = FecDlDrvGetCbDone((UINT32)pFDesc->CbDone);

        if (cbDone != NULL)
            cbDone(pFDesc->CbContext);

        pCtx->ListInfPtr->pFDescActive = (PFEC_DL_FDESC)pFDesc->Next;

        CSAPoolFree(pCtx->PoolPtr, pFDesc);
    }
}

void FecDlDrvRunInactiveList(CEVA_FECDL_DRV_CTX * pCtx)
{
    FEC_CTRL_REGS *pFecCtrlRegs;
    PFEC_DL_FDESC pFDesc;

    if (pCtx->ListInfPtr->pFDescInactive == NULL)
        return;

    pFecCtrlRegs = pCtx->FecCtrlRegsPtr;
    pCtx->ListInfPtr->bHwBusy = 1;

    pFDesc = pCtx->ListInfPtr->pFDescInactive;
    pCtx->ListInfPtr->pFDescActive   = pFDesc;
    pCtx->ListInfPtr->pFDescInactive = NULL;

    pCtx->ListInfPtr->ActiveTotalTicks   = pCtx->ListInfPtr->InactiveTotalTicks;
    pCtx->ListInfPtr->InactiveTotalTicks = 0;
    pCtx->ListInfPtr->StartTimeTicks     = MxGetTicks ();

    pFecCtrlRegs->DMA0InCtrl.Control     = DMA_FCOM_MODE;
    pFecCtrlRegs->DMA0InCtrl.BurstLength = 15;
    pFecCtrlRegs->DMA0OutCtrl.HeadFDesc  = (UINT32)pFDesc;
    pFecCtrlRegs->DMA0InCtrl.HeadFDesc   = (UINT32)pFDesc;
}

UINT32 FecDlDrvRunQ(PFEC_DL_FDESC pFDesc, UINT32 nNumUs)
{
    CEVA_FECDL_DRV_CTX * pCtx = FecDlDrvGetCtx();
    FEC_CTRL_REGS *pFecCtrlRegs;

    if (pCtx == NULL)
        return FEC_CEVA_RC_DEV_ERROR;

    if (pFDesc == NULL)
        return FEC_CEVA_RC_PARAM_ERROR;

    if (pCtx->ListInfPtr == NULL)
        return FEC_CEVA_RC_NOT_OPENED;

    FecDlDrvGetLock(pCtx);
    FecDlDrvAddToInactive(pCtx->ListInfPtr, pFDesc);

    if (pCtx->OwnerCpu != 0)
    {
        FecDlDrvProcActiveList(pCtx);
    }

    if (pCtx->ListInfPtr->pFDescActive == NULL)
    {
        pFecCtrlRegs = pCtx->FecCtrlRegsPtr;
        pCtx->ListInfPtr->bHwBusy = 1;

        pFDesc = pCtx->ListInfPtr->pFDescInactive;
        pCtx->ListInfPtr->pFDescActive   = pFDesc;
        pCtx->ListInfPtr->pFDescInactive = NULL;

        pCtx->ListInfPtr->ActiveTotalTicks = (nNumUs*150);
        pCtx->ListInfPtr->StartTimeTicks   = MxGetTicks ();

        pFecCtrlRegs->DMA0InCtrl.Control     = DMA_FCOM_MODE;
        pFecCtrlRegs->DMA0InCtrl.BurstLength = 15;
        pFecCtrlRegs->DMA0OutCtrl.HeadFDesc  = (UINT32)pFDesc;
        pFecCtrlRegs->DMA0InCtrl.HeadFDesc   = (UINT32)pFDesc;
    }
    else
    {
        pCtx->ListInfPtr->InactiveTotalTicks += (nNumUs*150);
    }

    FecDlDrvReleaseLock(pCtx);
    return FEC_CEVA_RC_OK;
}


UINT32 FecDlDrvRun(PFEC_DL_FDESC pFDesc)
{
    return FecDlDrvRunQ(pFDesc, 0);
}

void FecDlDrvProcCbDone(CEVA_FECDL_DRV_CTX * pCtx, FecCbDoneType CbDoneExclude)
{
    PFECDL_LIST_INF pInf = pCtx->ListInfPtr;
    FecCbDoneType cbDone;

    while (pInf->CbDoneGet != pInf->CbDonePut)
    {
        cbDone = FecDlDrvGetCbDone(pInf->CbDone[pInf->CbDoneGet]);

        if (cbDone != NULL && cbDone != CbDoneExclude)
        {
            cbDone(pInf->CbDonePtr[pInf->CbDoneGet]);
        }

        pInf->CbDoneGet ++;
        
        if (pInf->CbDoneGet >= MAX_CEVA_CB_DONE_NUM)
            pInf->CbDoneGet = 0;
    }
}

UINT32  FecDlDrvWaitCompletion(PFEC_DL_FDESC pFDesc, UINT32 nTime)
{
    CEVA_FECDL_DRV_CTX * pCtx = FecDlDrvGetCtx();
    UINT32 bHWBusy;
    FecCbDoneType CbDone;
    UINT32 t = 0;
    UINT32 rc = FEC_CEVA_RC_OK;

    if (pCtx == NULL)
        return FEC_CEVA_RC_DEV_ERROR;

    if (nTime != ~0)
    {
        t = MxGetTicks ();
    }

    // In thise case we need to wait only one 
    // FEC-DL descriptor, other scheduled descriptors 
    // do not matter for the application 

    if (pFDesc != NULL)
    {
        if (pCtx->OwnerCpu != 0)
        {
            // In this case we are responsible to process active and 
            // non-active queues, so we need to process queues till is needed 
            // element is putted to active queue and processed by DMA

            FecDlDrvGetLock(pCtx);

            while (1)
            {
                if (t != 0)
                {
                    if (MxGetTicks () - t > nTime)
                    {
                        rc = FEC_CEVA_RC_TIMEOUT;
                        break;
                    }
                }
            
                FecDlDrvProcActiveList(pCtx);

                if (pFDesc->HwContext.StatusComp != 0)
                    break;

                if (pCtx->ListInfPtr->pFDescActive == NULL)
                {
                    if (pCtx->ListInfPtr->pFDescInactive == NULL)
                        break;

                    // to run Inactive list 
                    FecDlDrvRunInactiveList(pCtx);
                }
            }
            
            FecDlDrvReleaseLock(pCtx);
        }
        else
        {
            // We can wait here, ARM is responsible to put this 
            // block to the queue and DMA will update status bit
        
            while (pFDesc->HwContext.StatusComp == 0)
            {
                if (t != 0)
                {
                    if (MxGetTicks () - t > nTime)
                    {
                        return FEC_CEVA_RC_TIMEOUT;
                    }
                }
            }

            FecDlDrvGetLock(pCtx);
            
            CbDone = FecDlDrvGetCbDone((UINT32)pFDesc->CbDone);
            pFDesc->CbDone = NULL;  // To prevent putting this CB by ARM to cbDone queue

            FecDlDrvReleaseLock(pCtx);

            if (CbDone != NULL)
                CbDone(pFDesc->CbContext);

            FecDlDrvProcCbDone (pCtx, CbDone);
        }

        return rc;
    }

    // In this case we need to wait completion of all 
    // scheduled descriptors 
    // NOTE! Scheduled by both CEVA and ARM code 

    if (pCtx->OwnerCpu != 0)
    {
        FecDlDrvGetLock(pCtx);
        
        while (1)
        {
            if (t != 0)
            {
                if (MxGetTicks () - t > nTime)
                {
                    rc = FEC_CEVA_RC_TIMEOUT;
                    break;
                }
            }
        
            FecDlDrvProcActiveList(pCtx);

            if (pCtx->ListInfPtr->pFDescActive == NULL)
            {
                if (pCtx->ListInfPtr->pFDescInactive == NULL)
                    break;

                // to run Inactive list 
                FecDlDrvRunInactiveList(pCtx);
            }
        }

        FecDlDrvReleaseLock(pCtx);
    }
    else
    {
        while (1)
        {
            bHWBusy = pCtx->ListInfPtr->bHwBusy;

            if (bHWBusy == 0)
                break;

            if (t != 0)
            {
                if (MxGetTicks () - t > nTime)
                {
                    rc = FEC_CEVA_RC_TIMEOUT;
                    break;
                }
            }
        }

        FecDlDrvProcCbDone (pCtx, NULL);
    }

    return rc;
}


// ------------------------------- FEC-UL --------------------------------------

UINT32 FecUlDrvOpen(void)
{
    PCEVA_DRV_CFG       pCfg = CEVA_GET_DRV_CFG();
    CEVA_FEC_DRV_CFG *  pFecCfg;
    CEVA_FECUL_DRV_CTX* pCtx;

    pFecCfg = &pCfg->FecULCfg;

    pCtx = FecUlDrvGetCtx();

    memset(pCtx, 0, sizeof (*pCtx));

    pCtx->HwSema        = pFecCfg->HwSema;
    pCtx->ListInfPtr    = (PFECUL_LIST_INF)pFecCfg->ListIntPtr;
    pCtx->PoolPtr       = (PMXPOOL)pFecCfg->PoolPtr;
    pCtx->FecCtrlRegsPtr= (FEC_CTRL_REGS *)0xFE410000;
    pCtx->OwnerCpu      = pFecCfg->OwnerCpu;

#if 0
    uart_printf ("   Open FEC-UL\r\n");
    uart_printf ("     HwSema: 0x%x\r\n", pCtx->HwSema);
    uart_printf ("    ListInf: 0x%x\r\n", pCtx->ListInfPtr);
    uart_printf ("    PoolPtr: 0x%x\r\n", pCtx->PoolPtr);
    uart_printf ("       Regs: 0x%x\r\n", pCtx->FecCtrlRegsPtr);
    uart_printf ("      Owner: 0x%x\r\n", pCtx->OwnerCpu);
#endif

    if (pCtx->PoolPtr == NULL || pCtx->ListInfPtr == NULL)
        return FEC_CEVA_RC_CFG_ERROR;

    pFecCfg->UsedMask |= (1 << MxGetCpuID ());

    return FEC_CEVA_RC_OK;
}


UINT32 FecUlDrvClose(void)
{
    PCEVA_DRV_CFG pCfg = CEVA_GET_DRV_CFG();
    CEVA_FEC_DRV_CFG * pFecCfg;
    CEVA_FECUL_DRV_CTX * pCtx;

    pFecCfg = &pCfg->FecULCfg;

    pCtx = FecUlDrvGetCtx();
    memset(pCtx, 0, sizeof (*pCtx));

    pFecCfg->UsedMask &= ~(1 << MxGetCpuID());
    
    return FEC_CEVA_RC_OK;
}

UINT32 FecUlDrvSetHwCtx(PFEC_UL_FDESC pFDesc, ULHWCTX HwContext)
{
    if (pFDesc == NULL)
        return FEC_CEVA_RC_PARAM_ERROR;

    HwContext.IRQEN = 0;
    HwContext.BOffset = 1;
    HwContext.StatusComp = 0;

    pFDesc->HwContext = HwContext;

    return FEC_CEVA_RC_OK;
}

UINT32 FecUlDrvSetLastDesc(PFEC_UL_FDESC pFDesc, FecCbDoneType CbDone, LPVOID CbContext)
{
    PFEC_UL_FDESC p = pFDesc;
    CEVA_FECUL_DRV_CTX * pCtx = FecUlDrvGetCtx();

    if (p == NULL)
        return FEC_CEVA_RC_PARAM_ERROR;

    while (p != NULL)
    {
        // Specifics of CEVA compiler, 
        // the code is needed here,
        // not in 'while' header
        
        if (p->Next == NULL)
            break;
        
        p = (PFEC_UL_FDESC)p->Next;
    }

    p->HwContext.IRQEN = !pCtx->OwnerCpu ? 1 : 0; // enable/disable IRQ generating to ARM processor
    
    p->CbDone = (FecCbDoneType)(((UINT32)CbDone) | FEC_CEVA_CB_DONE_PROC);
    p->CbContext = CbContext;

    return FEC_CEVA_RC_OK;
}

UINT32 FecUlDrvSetLastDescArm(PFEC_UL_FDESC pFDesc, FecCbDoneType CbDone, LPVOID CbContext)
{
    PFEC_UL_FDESC p = pFDesc;
    CEVA_FECUL_DRV_CTX * pCtx = FecUlDrvGetCtx();

    if (p == NULL)
        return FEC_CEVA_RC_PARAM_ERROR;

    while (p != NULL)
    {
        // Specifics of CEVA compiler, 
        // the code is needed here,
        // not in 'while' header
        
        if (p->Next == NULL)
            break;
    
        p = (PFEC_UL_FDESC)p->Next;
    }

    p->HwContext.IRQEN = !pCtx->OwnerCpu ? 1 : 0; // enable/disable IRQ generating to ARM processor
    
    p->CbDone = CbDone;
    p->CbContext = CbContext;

    return FEC_CEVA_RC_OK;
}

UINT32 FecUlDrvSetData(PFEC_UL_FDESC pFDesc, LPVOID pInData[], UINT32 InDataSize, LPVOID pOutData, UINT32 OutDataSize)
{
    volatile PFEC_CONFIG_REGS pFecConfigRegs;
    UINT32 BDescNum, i;
    DMABDESC locBdesc[MAX_UL_IO_BDESC];

    if (pFDesc == NULL)
        return FEC_CEVA_RC_PARAM_ERROR;

    pFecConfigRegs = (PFEC_CONFIG_REGS)(0xFE410000 + 0x400);

    if (pFecConfigRegs->bp_channel_interleaver == 1)
    {
        BDescNum = 1;
    }
    else
    {
        if (pFecConfigRegs->ci_symbol_number == 1)
            BDescNum = 14;
        else
            BDescNum = 12;
    }

    locBdesc[0].BPtr = (UINT32)pOutData;
    locBdesc[0].BCtrl = BCTRL_BLAST_MASK | OutDataSize;

    for (i = 0; i < BDescNum; ++i)
    {
        locBdesc[i + 1].BPtr = (UINT32)pInData[i];
        locBdesc[i + 1].BCtrl = InDataSize;
    }

    locBdesc[BDescNum].BCtrl |= BCTRL_BLAST_MASK;

    memcpy(&pFDesc->IoBdesc, &locBdesc, sizeof(DMABDESC)*(BDescNum + 1));

    return FEC_CEVA_RC_OK;
}

void FecUlDrvAddToInactive (PFECUL_LIST_INF pListInfPtr, PFEC_UL_FDESC pFDesc)
{
    PFEC_UL_FDESC pTmp;

    pTmp = pListInfPtr->pFDescInactive;

    if (pTmp == NULL)
    {
        pListInfPtr->pFDescInactive = pFDesc;
    }
    else
    {
        while (pTmp->Next != NULL)
            pTmp = (PFEC_UL_FDESC)pTmp->Next;

        pTmp->Next = pFDesc;
    }
}

FecCbDoneType FecUlDrvGetCbDone (UINT32 nCbDone)
{
    nCbDone &= ~FEC_CEVA_CB_DONE_PROC;
    return (FecCbDoneType)nCbDone;
}

void FecUlDrvProcActiveList(CEVA_FECUL_DRV_CTX * pCtx)
{
    PFEC_UL_FDESC pFDesc;
    FecCbDoneType cbDone;

    while ((pFDesc = (PFEC_UL_FDESC)pCtx->ListInfPtr->pFDescActive) != NULL)
    {
        if (pFDesc->HwContext.StatusComp == 0)
            break;

        cbDone = FecUlDrvGetCbDone((UINT32)pFDesc->CbDone);

        if (cbDone != NULL)
            cbDone(pFDesc->CbContext);

        pCtx->ListInfPtr->pFDescActive = (PFEC_UL_FDESC)pFDesc->Next;

        CSAPoolFree(pCtx->PoolPtr, pFDesc);
    }
}

void FecUlDrvRunInactiveList(CEVA_FECUL_DRV_CTX * pCtx)
{
    FEC_CTRL_REGS *pFecCtrlRegs;
    PFEC_UL_FDESC pFDesc;

    if (pCtx->ListInfPtr->pFDescInactive == NULL)
        return;

    pFecCtrlRegs = pCtx->FecCtrlRegsPtr;
    pCtx->ListInfPtr->bHwBusy = 1;

    pFDesc = pCtx->ListInfPtr->pFDescInactive;
    pCtx->ListInfPtr->pFDescActive   = pFDesc;
    pCtx->ListInfPtr->pFDescInactive = NULL;

    pCtx->ListInfPtr->ActiveTotalTicks   = pCtx->ListInfPtr->InactiveTotalTicks;
    pCtx->ListInfPtr->InactiveTotalTicks = 0;
    pCtx->ListInfPtr->StartTimeTicks     = MxGetTicks ();

    // start DMA
    pFecCtrlRegs->DMA1InCtrl.Control     = DMA_FCOM_MODE;
    pFecCtrlRegs->DMA1InCtrl.BurstLength = 15;
    pFecCtrlRegs->DMA1OutCtrl.HeadFDesc  = (UINT32)pFDesc;
    pFecCtrlRegs->DMA1InCtrl.HeadFDesc   = (UINT32)pFDesc;
}

UINT32 FecUlDrvRunQ(PFEC_UL_FDESC pFDesc, UINT32 nNumUs)
{
    CEVA_FECUL_DRV_CTX * pCtx = FecUlDrvGetCtx();
    FEC_CTRL_REGS *pFecCtrlRegs;

    if (pCtx == NULL)
        return FEC_CEVA_RC_DEV_ERROR;

    if (pFDesc == NULL)
        return FEC_CEVA_RC_PARAM_ERROR;

    if (pCtx->ListInfPtr == NULL)
        return FEC_CEVA_RC_NOT_OPENED;

    FecUlDrvGetLock(pCtx);
    FecUlDrvAddToInactive(pCtx->ListInfPtr, pFDesc);

    if (pCtx->OwnerCpu != 0)
    {
        FecUlDrvProcActiveList(pCtx);
    }

    if (pCtx->ListInfPtr->pFDescActive == NULL)
    {
        pFecCtrlRegs = pCtx->FecCtrlRegsPtr;
        pCtx->ListInfPtr->bHwBusy = 1;

        pFDesc = pCtx->ListInfPtr->pFDescInactive;
        pCtx->ListInfPtr->pFDescActive   = pFDesc;
        pCtx->ListInfPtr->pFDescInactive = NULL;

        pCtx->ListInfPtr->ActiveTotalTicks = (nNumUs*150);
        pCtx->ListInfPtr->StartTimeTicks   = MxGetTicks ();

        pFecCtrlRegs->DMA1InCtrl.Control     = DMA_FCOM_MODE;
        pFecCtrlRegs->DMA1InCtrl.BurstLength = 15;
        pFecCtrlRegs->DMA1OutCtrl.HeadFDesc  = (UINT32)pFDesc;
        pFecCtrlRegs->DMA1InCtrl.HeadFDesc   = (UINT32)pFDesc;
    }
    else
    {
        pCtx->ListInfPtr->InactiveTotalTicks += (nNumUs*150);
    }

    FecUlDrvReleaseLock(pCtx);

    return FEC_CEVA_RC_OK;
}

UINT32 FecUlDrvRun(PFEC_UL_FDESC pFDesc)
{
    return FecUlDrvRunQ(pFDesc, 0);
}

void FecUlDrvProcCbDone(CEVA_FECUL_DRV_CTX * pCtx, FecCbDoneType CbDoneExclude)
{
    PFECUL_LIST_INF pInf = pCtx->ListInfPtr;
    FecCbDoneType cbDone;

    while (pInf->CbDoneGet != pInf->CbDonePut)
    {
        cbDone = FecUlDrvGetCbDone(pInf->CbDone[pInf->CbDoneGet]);

        if (cbDone != NULL && cbDone != CbDoneExclude)
        {
            cbDone(pInf->CbDonePtr[pInf->CbDoneGet]);
        }

        pInf->CbDoneGet ++;
        
        if (pInf->CbDoneGet >= MAX_CEVA_CB_DONE_NUM)
            pInf->CbDoneGet = 0;
    }
}

UINT32 FecUlDrvWaitCompletion(PFEC_UL_FDESC pFDesc, UINT32 nTime)
{
    CEVA_FECUL_DRV_CTX * pCtx = FecUlDrvGetCtx();
    UINT32 bHWBusy;
    FecCbDoneType CbDone;
    UINT32 t = 0;
    UINT32 rc = FEC_CEVA_RC_OK;

    if (pCtx == NULL)
        return FEC_CEVA_RC_DEV_ERROR;

    if (nTime != ~0)
    {
        t = MxGetTicks ();
    }

    // In thise case we need to wait only one 
    // FEC-DL descriptor, other scheduled descriptors 
    // do not matter for the application 

    if (pFDesc != NULL)
    {
        if (pCtx->OwnerCpu != 0)
        {
            // In this case we are responsible to process active and 
            // non-active queues, so we need to process queues till is needed 
            // element is putted to active queue and processed by DMA

            FecUlDrvGetLock(pCtx);

            while (1)
            {
                if (t != 0)
                {
                    if (MxGetTicks () - t > nTime)
                    {
                        rc = FEC_CEVA_RC_TIMEOUT;
                        break;
                    }
                }
            
                FecUlDrvProcActiveList(pCtx);

                if (pFDesc->HwContext.StatusComp != 0)
                    break;

                if (pCtx->ListInfPtr->pFDescActive == NULL)
                {
                    if (pCtx->ListInfPtr->pFDescInactive == NULL)
                        break;

                    // to run Inactive list 
                    FecUlDrvRunInactiveList(pCtx);
                }
            }
            
            FecUlDrvReleaseLock(pCtx);
        }
        else
        {
            // We can wait here, ARM is responsible to put this 
            // block to the queue and DMA will update status bit
        
            while (pFDesc->HwContext.StatusComp == 0)
            {
                if (t != 0)
                {
                    if (MxGetTicks () - t > nTime)
                    {
                        return FEC_CEVA_RC_TIMEOUT;
                    }
                }
            }

            FecUlDrvGetLock(pCtx);
            
            CbDone = FecUlDrvGetCbDone((UINT32)pFDesc->CbDone);
            pFDesc->CbDone = NULL;  // To prevent putting this CB by ARM to cbDone queue

            FecUlDrvReleaseLock(pCtx);

            if (CbDone != NULL)
                CbDone(pFDesc->CbContext);

            FecUlDrvProcCbDone (pCtx, CbDone);
        }

        return rc;
    }

    // In this case we need to wait completion of all 
    // scheduled descriptors 
    // NOTE! Scheduled by both CEVA and ARM code 

    if (pCtx->OwnerCpu != 0)
    {
        FecUlDrvGetLock(pCtx);
        
        while (1)
        {
            if (t != 0)
            {
                if (MxGetTicks () - t > nTime)
                {
                    rc = FEC_CEVA_RC_TIMEOUT;
                    break;
                }
            }
        
            FecUlDrvProcActiveList(pCtx);

            if (pCtx->ListInfPtr->pFDescActive == NULL)
            {
                if (pCtx->ListInfPtr->pFDescInactive == NULL)
                    break;

                // to run Inactive list 
                FecUlDrvRunInactiveList(pCtx);
            }
        }

        FecUlDrvReleaseLock(pCtx);
    }
    else
    {
        while (1)
        {
            bHWBusy = pCtx->ListInfPtr->bHwBusy;

            if (bHWBusy == 0)
                break;

            if (t != 0)
            {
                if (MxGetTicks () - t > nTime)
                {
                    rc = FEC_CEVA_RC_TIMEOUT;
                    break;
                }
            }
        }

        FecUlDrvProcCbDone (pCtx, NULL);
    }

    return rc;
}

PFEC_UL_FDESC FecUlDrvAllocDesc(UINT32 NumDesc)
{
    CEVA_FECUL_DRV_CTX * pCtx = FecUlDrvGetCtx();
    PFEC_UL_FDESC pFDesc, pFDescPrev = NULL, pFDescHead = NULL;
    UINT32 i;

    MXPOOL * pPool;
    pPool = pCtx->PoolPtr;

    // Lock is needed to make possible to allocate
    // fdesc blocks from more than one CEVA by using the same pool
    // create by the ARM FEC-DL driver 

    #if FEC_CEVA_ALLOC_SYNC
    FecUlDrvGetLock(pCtx);
    #endif

    for (i = 0; i < NumDesc; i++)
    {
        pFDesc = (PFEC_UL_FDESC)CSAPoolAlloc (pPool);

        if (pFDesc == NULL)
        {
            while (pFDescHead != NULL)
            {
                pFDescPrev = pFDescHead;
                pFDescHead = (PFEC_UL_FDESC)pFDescPrev->Next;

                CSAPoolFree (pPool, pFDesc);
            }

            #if FEC_CEVA_ALLOC_SYNC
            FecUlDrvReleaseLock(pCtx);
            #endif
            
            return NULL;
        }

        pFDesc->OwnerCpuID = 0xFFFF;
        
        ((PUINT32)&pFDesc->HwContext)[0] = 0; // disable IRQ
        ((PUINT32)&pFDesc->HwContext)[1] = 0;
        ((PUINT32)&pFDesc->HwContext)[2] = 0; // StatusComp

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

    #if FEC_CEVA_ALLOC_SYNC
    FecUlDrvReleaseLock(pCtx);
    #endif
    
    return pFDescHead;
}


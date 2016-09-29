//-------------------------------------------------------------------------------------------
/** @file FECdrv.c
 *
 * @brief FEC (Channel Coding) HAL functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.58 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "diagnostics.h"
#include "FECdrv.h"
#include "dspdiag.h"
#include "heaps.h"
#include "appprintf.h"
#include "4gmx_serv.h"
#include "clkrst.h"
#include "initcpu.h"
#include "queue.h"
#include "mdmadrv.h"
#include "systimers.h"
#include "mmumgr.h"
#include "DSPdrv.h"

extern MDMA_CTX g_MdmaRadCtx;

VUINT32 FecDLLock = 0;
VUINT32 FecULLock = 0;

FEC_DL_CTX g_FecDlCtx = {0};
FEC_UL_CTX g_FecUlCtx = {0};
FEC_ROBUST_CTRL_CTX FecRobustCtx;

HANDLE g_FecDlPart = {0};
HANDLE g_FecUlPart = {0};
PUINT32 g_FecDlBuf = NULL;
PUINT32 g_FecUlBuf = NULL;

#ifdef FEC_ADDITIONAL_CHECK_ENABLED
MEMORY_REGIONS fec_mem_reg[FEC_NUM_MEM_REGIONS];

BOOL FecDlCheckHanging(void);
BOOL FecUlCheckHanging(void);
BOOL FecDlCheckIOBufAddress(PFEC_DL_FDESC pFDesc);
BOOL FecULCheckIOBufAddress (PFEC_UL_FDESC pFDesc);
void FecSmartReset(PFEC_DL_FDESC pLastFDlDesc, PFEC_UL_FDESC pLastFUlDesc, BOOL isDlLocked, BOOL isUlLocked);
void FECFillMemRegions(void);
BOOL FECIsMemRegions(UINT32 address);
#endif // #ifdef FEC_ADDITIONAL_CHECK_ENABLED

MXRC FecDlProcInt(void);
MXRC FecUlProcInt(void);
void FecDlDma(UINT32 TotalTicks);
void FecUlDma(UINT32 TotalTicks);
PFEC_DL_FDESC  FecDlShiftActive (PFEC_DL_CTX pFecCtx);
PFEC_UL_FDESC  FecUlShiftActive (PFEC_UL_CTX pFecCtx);

INLINE PFEC_DL_CTX FecDlGetCtx(void)
{
    return &g_FecDlCtx;
}

INLINE PFEC_UL_CTX FecUlGetCtx(void)
{
    return &g_FecUlCtx;
}

INLINE FEC_ROBUST_CTRL_CTX* FecGetRobustCtx(void)
{
    return &FecRobustCtx;
}

UINT32 FECGetPoolTotalSize(UINT32 id)
{
    UINT32 total_num = 0;
    UINT32 cpu_num = SysGetCpuNum();

    if (id == ID_FEC_DL)
    {
        total_num = FEC_DL_CORE_0_FDESC_NUM;

        if (cpu_num > 1)
            total_num += FEC_DL_CORE_1_FDESC_NUM;

        if (cpu_num > 2)
            total_num += FEC_DL_CORE_2_FDESC_NUM;

        if (cpu_num > 3)
            total_num += FEC_DL_CORE_3_FDESC_NUM;

        total_num += FEC_DL_CEVAX_FDESC_NUM;
    }
    else if (id == ID_FEC_UL)
    {
        total_num = FEC_UL_CORE_0_FDESC_NUM;

        if (cpu_num > 1)
            total_num += FEC_UL_CORE_1_FDESC_NUM;

        if (cpu_num > 2)
            total_num += FEC_UL_CORE_2_FDESC_NUM;

        if (cpu_num > 3)
            total_num += FEC_UL_CORE_3_FDESC_NUM;

        total_num += FEC_UL_CEVAX_FDESC_NUM;
    }

    return total_num;
}

UINT32 FECGetPoolSize(UINT32 id, UINT32 nCpuID)
{
    if (id == ID_FEC_UL)
    {
        switch (nCpuID)
        {
            case 0:
                return FEC_UL_CORE_0_FDESC_NUM;

            case 1:
                return FEC_UL_CORE_1_FDESC_NUM;

            case 2:
                return FEC_UL_CORE_2_FDESC_NUM;

            case 3:
                return FEC_UL_CORE_3_FDESC_NUM;

            case 0xFFFF:
                return FEC_UL_CEVAX_FDESC_NUM;

            default:
                return 0;
        }
    }
    else if (id == ID_FEC_DL)
    {
        switch (nCpuID)
        {
            case 0:
                return FEC_DL_CORE_0_FDESC_NUM;

            case 1:
                return FEC_DL_CORE_1_FDESC_NUM;

            case 2:
                return FEC_DL_CORE_2_FDESC_NUM;

            case 3:
                return FEC_DL_CORE_3_FDESC_NUM;

            case 0xFFFF:
                return FEC_DL_CEVAX_FDESC_NUM;

            default:
                return 0;
        }
    }

    return 0;
}

BOOL FecDlCheckFDescAddress(PFEC_DL_FDESC pFDesc)
{
    UINT32 PoolStorBase;
    UINT32 PoolStorLimit;
    UINT32 FDescSize = ROUND(sizeof (FEC_DL_FDESC), 16);
    PFEC_DL_CTX pCtx = FecDlGetCtx();
    
    _ASSERT_PTR(pFDesc);
    _ASSERT_PTR(pCtx);

    PoolStorBase = (UINT32)pCtx->PoolStorPtr;

    // Get pool total size
    PoolStorLimit = PoolStorBase + FECGetPoolTotalSize(ID_FEC_DL) * FDescSize;
    
    // Check address range for the frame descriptor
    if ((PoolStorBase > (UINT32)pFDesc) || ((UINT32)pFDesc >= PoolStorLimit))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL FecUlCheckFDescAddress(PFEC_UL_FDESC pFDesc)
{
    UINT32 PoolStorBase;
    UINT32 PoolStorLimit;
    UINT32 FDescSize = ROUND(sizeof (FEC_UL_FDESC), 16);
    PFEC_UL_CTX pCtx = FecUlGetCtx();
    
    _ASSERT_PTR(pFDesc);
    _ASSERT_PTR(pCtx);

    PoolStorBase = (UINT32)pCtx->PoolStorPtr;

    // Get pool total size
    PoolStorLimit = PoolStorBase + FECGetPoolTotalSize(ID_FEC_UL) * FDescSize;
    
    // Check address range for the frame descriptor
    if ((PoolStorBase > (UINT32)pFDesc) || ((UINT32)pFDesc >= PoolStorLimit))
    {
        return FALSE;
    }

    return TRUE;
}

void FecDlGetLock (PFEC_DL_CTX pFecDlCtx)
{
    if (pFecDlCtx->CevaPoolPtr != NULL)
    {
        MxiLockHwSema (FEC_DL_HW_SEMA);
    }
    else
    {
        MxGetLock (&FecDLLock);
    }
}

void FecDlReleaseLock (PFEC_DL_CTX pFecDlCtx)
{
    if (pFecDlCtx->CevaPoolPtr != NULL)
    {
        MxiUnlockHwSema (FEC_DL_HW_SEMA);
    }
    else
    {
        MxReleaseLock (&FecDLLock);
    }
}

void FecUlGetLock (PFEC_UL_CTX pFecUlCtx)
{
    if (pFecUlCtx->CevaPoolPtr != NULL)
    {
        MxiLockHwSema (FEC_UL_HW_SEMA);
    }
    else
    {
        MxGetLock (&FecULLock);
    }
}

void FecUlReleaseLock (PFEC_UL_CTX pFecUlCtx)
{
    if (pFecUlCtx->CevaPoolPtr != NULL)
    {
        MxiUnlockHwSema (FEC_UL_HW_SEMA);
    }
    else
    {
        MxReleaseLock (&FecULLock);
    }
}

/// ---------------------------- DL PART --------------------------------------

INLINE PFEC_DL_FDESC FecDlGetActiveFdesc (PFEC_DL_CTX pFecCtx)
{
    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    return pFecCtx->ListInfPtr->pFDescActive;
}

INLINE void FecDlSetActiveFdesc (PFEC_DL_CTX pFecCtx, PFEC_DL_FDESC pActFDesc)
{
    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    pFecCtx->ListInfPtr->pFDescActive = pActFDesc;
}

INLINE PFEC_DL_FDESC FecDlGetInactiveFdesc (PFEC_DL_CTX pFecCtx)
{
    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    return pFecCtx->ListInfPtr->pFDescInactive;
}

INLINE void FecDlSetInactiveFdesc (PFEC_DL_CTX pFecCtx, PFEC_DL_FDESC pInactFDesc)
{
    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    pFecCtx->ListInfPtr->pFDescInactive = pInactFDesc;
}

INLINE void FecDlSetBusy (PFEC_DL_CTX pFecCtx, UINT32 nBusy)
{
    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    pFecCtx->ListInfPtr->bHwBusy = nBusy;
}

INLINE UINT32 FecDlGetBusy (PFEC_DL_CTX pFecCtx)
{
    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    return pFecCtx->ListInfPtr->bHwBusy;
}

INLINE void FecDlSetRunTime (PFEC_DL_CTX pFecCtx, UINT32 nStartTime, UINT32 nTotalTicks)
{
    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    pFecCtx->ListInfPtr->StartTimeTicks   = nStartTime;
    pFecCtx->ListInfPtr->ActiveTotalTicks = nTotalTicks;
}

INLINE void FecDlAddInactTicks (PFEC_DL_CTX pFecCtx, UINT32 nTotalTicks)
{
    if (nTotalTicks == 0)
        return;

    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    pFecCtx->ListInfPtr->InactiveTotalTicks += nTotalTicks;
}

INLINE UINT32 FecDlGetInactTicks(PFEC_DL_CTX pFecCtx)
{
    UINT32 nVal;

    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    nVal = pFecCtx->ListInfPtr->InactiveTotalTicks;

    if (nVal != 0)
        pFecCtx->ListInfPtr->InactiveTotalTicks = 0;

    return nVal;
}

/// ---------------------------- UL PART --------------------------------------

INLINE PFEC_UL_FDESC FecUlGetActiveFdesc (PFEC_UL_CTX pFecCtx)
{
    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    return pFecCtx->ListInfPtr->pFDescActive;
}

INLINE void FecUlSetActiveFdesc (PFEC_UL_CTX pFecCtx, PFEC_UL_FDESC pActFDesc)
{
    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    pFecCtx->ListInfPtr->pFDescActive = pActFDesc;
}

INLINE PFEC_UL_FDESC FecUlGetInactiveFdesc (PFEC_UL_CTX pFecCtx)
{
    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    return pFecCtx->ListInfPtr->pFDescInactive;
}

INLINE void FecUlSetInactiveFdesc (PFEC_UL_CTX pFecCtx, PFEC_UL_FDESC pInactFDesc)
{
    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    pFecCtx->ListInfPtr->pFDescInactive = pInactFDesc;
}

INLINE void FecUlSetBusy (PFEC_UL_CTX pFecCtx, UINT32 nBusy)
{
    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    pFecCtx->ListInfPtr->bHwBusy = nBusy;
}

INLINE UINT32 FecUlGetBusy (PFEC_UL_CTX pFecCtx)
{
    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    return pFecCtx->ListInfPtr->bHwBusy;
}

INLINE void FecUlSetRunTime (PFEC_UL_CTX pFecCtx, UINT32 nStartTime, UINT32 nTotalTicks)
{
    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    pFecCtx->ListInfPtr->StartTimeTicks   = nStartTime;
    pFecCtx->ListInfPtr->ActiveTotalTicks = nTotalTicks;
}

INLINE void FecUlAddInactTicks (PFEC_UL_CTX pFecCtx, UINT32 nTotalTicks)
{
    if (nTotalTicks == 0)
        return;

    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    pFecCtx->ListInfPtr->InactiveTotalTicks += nTotalTicks;
}

INLINE UINT32 FecUlGetInactTicks(PFEC_UL_CTX pFecCtx)
{
    UINT32 nVal;

    _ASSERT_PTR(pFecCtx);
    _ASSERT_PTR(pFecCtx->ListInfPtr);

    nVal = pFecCtx->ListInfPtr->InactiveTotalTicks;

    if (nVal != 0)
        pFecCtx->ListInfPtr->InactiveTotalTicks = 0;

    return nVal;
}

//-------------------------------------------------------------------------------------------
/** @brief This function is the IRQ Handler for the DL FEC Unit
 *
 *
 *  @param   pIntClearReg [out]  IRQ clean register
 *  @param   nIrqID           [in]    IRQ Id
 *  @param   pParam         [in]    Pointer to the MDMA diagnostic context
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
void FecDlIrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, LPVOID pParam)
{
    PFEC_DL_CTX pFecDlCtx = FecDlGetCtx();
    volatile FEC_CTRL_REGS * pFecCtrl = pFecDlCtx->pFecCtrlRegs;

    pFecCtrl->DMA0OutCtrl.IrqStatusClear = DMAIRQ_FCTRL;
    pFecCtrl->DMA0InCtrl.IrqStatusClear = DMAIRQ_FCTRL;

    FecDlProcInt();
  
    *pIntClearReg = nIrqID;
}

//-------------------------------------------------------------------------------------------
/** @brief This function is the IRQ Handler for the UL FEC Unit
 *
 *
 *  @param   pIntClearReg [out]  IRQ clean register
 *  @param   nIrqID           [in]    IRQ Id
 *  @param   pParam         [in]    Pointer to the MDMA diagnostic context
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
void FecUlIrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, LPVOID pParam)
{
    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();
    volatile FEC_CTRL_REGS * pFecCtrl = pFecUlCtx->pFecCtrlRegs;

    pFecCtrl->DMA1OutCtrl.IrqStatusClear = DMAIRQ_FCTRL;
    pFecCtrl->DMA1InCtrl.IrqStatusClear = DMAIRQ_FCTRL;

    FecUlProcInt();

    *pIntClearReg = nIrqID;
}

//-------------------------------------------------------------------------------------------
/** @brief This function resets the UL and DL FEC driver variables
 *
 *
 *  @param   none
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
void FecReset(void)
{
    volatile PUINT32 p_regs = (PUINT32)FEC_CONFIG_REGS_BASE;
    UINT32 fec_config0;
    UINT32 delay;

    fec_config0 = *p_regs;

    //uart_printf ("FEC RESET!!!\r\n");

    REG32(SYS_AXI_RESET_0) |= 0x20;
    REG32(FEC_RESET_REG) = 0x31;

    REG32(SYS_AXI_RESET_0) &= 0xD0;
    REG32(FEC_RESET_REG) = 0;

    delay = 10;
    while (delay--);

    *p_regs = fec_config0;

    //MxExcSet((UINT32*)&FecUlBusy, 0);
}

void FecDLReset(void)
{
}

MXRC FECInitPool (UINT32 id)
{
    UINT32 FDescSize, core, fnum;
    PUINT8 pPoolStorage;
    PUINT32 pFreePtr, pUsedPtr;
    UINT32 nCevaPoolEna = 0;

    HANDLE hListIntHeap = AppHeap;
    UINT32 nListInfSize;

    PFEC_DL_CTX pDlCtx = FecDlGetCtx();
    PFEC_UL_CTX pUlCtx = FecUlGetCtx();

    _ASSERT_PTR(pDlCtx);
    _ASSERT_PTR(pUlCtx);

    if (id == ID_FEC_DL)
    {
        FDescSize = ROUND(sizeof (FEC_DL_FDESC), 16);
        nListInfSize = ROUND(sizeof(FECDL_LIST_INF), 4);
    }
    else
    {
        FDescSize = ROUND(sizeof (FEC_UL_FDESC), 16);
        nListInfSize = ROUND(sizeof(FECUL_LIST_INF), 4);
    }

    if (FECGetPoolSize(id, 0xFFFF) != 0)
    {
        nCevaPoolEna = 1;
        hListIntHeap = DspHeap;

        nListInfSize += sizeof (MXPOOL);
    }

    if (id == ID_FEC_DL)
    {
        pDlCtx->ListInfPtr = (FECDL_LIST_INF*)MxHeapAlloc (hListIntHeap, nListInfSize);
        pDlCtx->ListInfHeap = hListIntHeap;

        if(pDlCtx->ListInfPtr == NULL)
        {
            uart_printf ("FEC-DL: error to allocate interface obj\r\n");
            _ASSERT_PTR(pDlCtx->ListInfPtr);
            return RC_MDMA_DRV_ALLOC_INF_ERROR;
        }

        memset (pDlCtx->ListInfPtr, 0, nListInfSize);

        if (nCevaPoolEna != 0)
        {
            pDlCtx->CevaPoolPtr = (MXPOOL*)(pDlCtx->ListInfPtr+1);
        }
        else
        {
            pDlCtx->CevaPoolPtr = NULL;
        }
    }
    else
    {
        pUlCtx->ListInfPtr = (FECUL_LIST_INF*)MxHeapAlloc (hListIntHeap, nListInfSize);
        pUlCtx->ListInfHeap = hListIntHeap;

        if(pUlCtx->ListInfPtr == NULL)
        {
            uart_printf ("FECUL: error to allocate interface obj\r\n");
            _ASSERT_PTR(pUlCtx->ListInfPtr);
            return RC_MDMA_DRV_ALLOC_INF_ERROR;
        }

        memset (pUlCtx->ListInfPtr, 0, nListInfSize);

        if (nCevaPoolEna != 0)
        {
            pUlCtx->CevaPoolPtr = (MXPOOL*)(pUlCtx->ListInfPtr+1);
        }
        else
        {
            pUlCtx->CevaPoolPtr = NULL;
        }
    }

    // to allocate the storage for FDESC blocks for all possible cores 
    pPoolStorage = (PUINT8)MxHeapAlloc(FEC_HEAP, FECGetPoolTotalSize(id) * FDescSize);
    
    if (pPoolStorage == NULL)
    {
        _ASSERT_PTR(pPoolStorage);
        return RC_MDMA_DRV_ALLOC_BUF_ERROR;
    }

    // to allocate the storage for 'used' and 'free' indexes for all cores
    pUsedPtr = (PUINT32)MxHeapAlloc(hListIntHeap, (FECGetPoolTotalSize(id) + CPU_NUM + nCevaPoolEna) * sizeof (UINT32) * 2);

    if (pUsedPtr == NULL)
    {
        _ASSERT_PTR(pUsedPtr);
        MxHeapFree (FEC_HEAP, pPoolStorage);
        return RC_MDMA_DRV_ALLOC_BUF_ERROR;
    }

    pFreePtr = pUsedPtr + (FECGetPoolTotalSize(id) + CPU_NUM + nCevaPoolEna);

    if (id == ID_FEC_DL)
    {
        pDlCtx->PoolStorPtr = pPoolStorage;
        pDlCtx->PoolIndex   = pUsedPtr;
    }
    else
    {
        pUlCtx->PoolStorPtr = pPoolStorage;
        pUlCtx->PoolIndex   = pUsedPtr;
    }
    
    // to initialize the POOL for each core 
    for (core = 0; core < SysGetCpuNum(); core++)
    {
        fnum = FECGetPoolSize(id, core);
        
        if (fnum != 0)
        {
            if (id == ID_FEC_DL)
            {
                MxiPoolInit(&pDlCtx->Pool[core], pPoolStorage, fnum, FDescSize, pFreePtr, pUsedPtr);
            }
            else
            {
                MxiPoolInit(&pUlCtx->Pool[core], pPoolStorage, fnum, FDescSize, pFreePtr, pUsedPtr);
            }
            
            pPoolStorage += (FDescSize * fnum);

            pUsedPtr += (fnum + 1);
            pFreePtr += (fnum + 1);
        }
    }

    // To initialize CEVA pool, if this configuration is enabled

    if (nCevaPoolEna)
    {
        fnum = FECGetPoolSize(id, 0xFFFF);

        if (fnum != 0)
        {
            if (id == ID_FEC_DL)
            {
                MxiPoolInit(pDlCtx->CevaPoolPtr, pPoolStorage, fnum, FDescSize, pFreePtr, pUsedPtr);
            }
            else
            {
                MxiPoolInit(pUlCtx->CevaPoolPtr, pPoolStorage, fnum, FDescSize, pFreePtr, pUsedPtr);
            }
            
            pPoolStorage += (FDescSize * fnum);

            pUsedPtr += (fnum + 1);
            pFreePtr += (fnum + 1);
        }
    }

    return MX_OK;
}

MXRC FecShareAccess(UINT32 id)
{
    PFEC_DL_CTX pFecDlCtx = FecDlGetCtx();
    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();
    CEVA_DRV_CFG * pCfg = DspDrvGetCevaDrvCfgParams();

    _ASSERT_PTR(pFecDlCtx);
    _ASSERT_PTR(pFecUlCtx);
    _ASSERT_PTR(pCfg);

    if (pCfg == NULL)
        return RC_FEC_DRV_SHARE_INFO_ERR;

    switch (id)
    {
        case ID_FEC_DL:
        {
            if (FEC_DL_CEVAX_FDESC_NUM==0)
            {
                return MX_OK;
            }
            else
            {
                pCfg->FecDLCfg.HwSema     = CEVA_DRV_HW_SEMA_FEC_DL;
                pCfg->FecDLCfg.PoolPtr    = pFecDlCtx->CevaPoolPtr;
                pCfg->FecDLCfg.OwnerCpu   = FEC_DL_CEVAX_OWNER;
                pCfg->FecDLCfg.ListIntPtr = pFecDlCtx->ListInfPtr;
                pCfg->FecDLCfg.UsedMask   = 0;

                REG32(CEVA_DRV_HW_SEMA_FEC_DL) = 0;
            }

            break;
        }

        case ID_FEC_UL:
        {
            if (FEC_UL_CEVAX_FDESC_NUM==0)
            {
                return MX_OK;
            }
            else
            {
                pCfg->FecULCfg.HwSema     = CEVA_DRV_HW_SEMA_FEC_UL;
                pCfg->FecULCfg.PoolPtr    = pFecUlCtx->CevaPoolPtr;
                pCfg->FecULCfg.OwnerCpu   = FEC_UL_CEVAX_OWNER;
                pCfg->FecULCfg.ListIntPtr = pFecUlCtx->ListInfPtr;
                pCfg->FecULCfg.UsedMask   = 0;

                REG32(CEVA_DRV_HW_SEMA_FEC_UL) = 0;
            }

            break;
        }

        default:
        {
            return RC_FEC_DRV_DEV_ID_ERROR;
        }
    }

    return MX_OK;
}

MXRC FecRobustInit(void)
{
    MXRC rc = MX_OK;

    #ifdef FEC_ROBUST_CTRL_ENABLED
    if (FAILED(rc = FecRobustCtrl(1, FEC_ROBUST_CTRL_DEF_TIMER_PERIOD)))
        return rc;
    #endif

    return rc;
}

MXRC FecRobustShutdown(void)
{
    MXRC rc = MX_OK;

    #ifdef FEC_ROBUST_CTRL_ENABLED
    if (FAILED(rc = FecRobustCtrl(0, 0)))
        return rc;
    #endif

    return rc;
}

UINT32 FecRobustGetMasterCpu(void)
{
#ifdef FEC_ROBUST_CTRL_ENABLED
    FEC_ROBUST_CTRL_CTX * pCtx = FecGetRobustCtx();
    _ASSERT_PTR(pCtx);

    if (pCtx->Inited == 0)
        return 0xFFFF;
    
    return pCtx->CpuID;    
#else
    return 0xFFFF;
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief This function Allocates and Initalizes all driver variables for the DL FEC
 *
 *
 *  @param   pFecConfigRegs Pointer to the Config register
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecDlInit(PFEC_CONFIG_REGS pFecConfigRegs)
{
    PFEC_DL_CTX pFecDlCtx = FecDlGetCtx();
    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();
    char * pShareInfo, *pName;
    MXRC rc = MX_OK;

    if (pFecDlCtx->bInited)
    {
        FecDlShutdown();
    }

    // if FEC was not inited at all,
    // just to reset this block in
    // branch of DL initialization

    _ASSERT_PTR(pFecUlCtx);

    if (pFecUlCtx->bInited == FALSE)
    {
        volatile PUINT32 p_regs = (PUINT32)FEC_CONFIG_REGS_BASE;

        FecReset ();

        if (pFecConfigRegs != NULL)
            *p_regs = *(PUINT32)pFecConfigRegs;
    }

    memset(pFecDlCtx, 0, sizeof(*pFecDlCtx));

    pFecDlCtx->CpuID = MxGetCpuID ();

    pFecDlCtx->pFecCtrlRegs = (volatile FEC_CTRL_REGS *)FEC_CTRL_REGS_BASE;

    rc = FECInitPool(ID_FEC_DL);
    _ASSERT_RC(rc);
    pName = "POOL";

    #if FEC_DL_CEVAX_OWNER == 0
    MxSetIntHandler(HAL_IRQ_FEC_DL, FEC_INT_MODE, PRI_DL_FEC, FecDlIrqHandler, pFecDlCtx);
    MxEnableInt(HAL_IRQ_FEC_DL);
    #endif

    pFecDlCtx->pFecCtrlRegs->DMA0OutCtrl.DmaIrqEnable = DMAIRQ_FCTRL;

    pFecDlCtx->bInited = TRUE;
    pShareInfo = (FEC_DL_CEVAX_FDESC_NUM)?"/CEVA-SHARED-DRV": "";
    rc = FecShareAccess(ID_FEC_DL);
    uart_printf("FEC-DL[%s%s%s]:[%s {rc:%X}] [Master CPU-%d]\r\n", pName, pShareInfo, FEC_DL_CEVAX_OWNER ? "/CEVA-OWNER" : "", (SUCCESSFUL(rc))? "OK":"FAIL", rc, MxGetCpuID ());

    return rc;
}


//-------------------------------------------------------------------------------------------
/** @brief This function Allocates and Initalizes all driver variables for the UL FEC
 *
 *
 *  @param   pFecConfigRegs Pointer to the Config register
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecUlInit(PFEC_CONFIG_REGS pFecConfigRegs)
{
    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();
    PFEC_DL_CTX pFecDlCtx = FecDlGetCtx();
    char * pShareInfo, * pName;
    MXRC rc = MX_OK;

    if (pFecUlCtx->bInited)
    {
        FecUlShutdown();
    }

    // if FEC was not inited at all,
    // just to reset this block in
    // branch of UL initialization

    _ASSERT_PTR(pFecDlCtx);

    if (pFecDlCtx->bInited == FALSE)
    {
        volatile PUINT32 p_regs = (PUINT32)FEC_CONFIG_REGS_BASE;

        FecReset ();

        if (pFecConfigRegs != NULL)
            *p_regs = *(PUINT32)pFecConfigRegs;
    }

    memset(pFecUlCtx, 0, sizeof(*pFecUlCtx));

    pFecUlCtx->CpuID = MxGetCpuID ();

    pFecUlCtx->pFecCtrlRegs = (volatile FEC_CTRL_REGS *)FEC_CTRL_REGS_BASE;

    pName = "POOL";
    
    rc = FECInitPool(ID_FEC_UL);
    _ASSERT_RC(rc);

    MxSetIntHandler(HAL_IRQ_FEC_UL, FEC_INT_MODE, PRI_UL_FEC, FecUlIrqHandler, pFecUlCtx);
    MxEnableInt(HAL_IRQ_FEC_UL);

    pFecUlCtx->pFecCtrlRegs->DMA1OutCtrl.DmaIrqEnable = DMAIRQ_FCTRL;

    pFecUlCtx->bInited = TRUE;

    pShareInfo = (FEC_UL_CEVAX_FDESC_NUM)?"/CEVA-SHARED-DRV": "";
    rc = FecShareAccess(ID_FEC_UL);
    uart_printf("FEC-UL[%s%s%s]:[%s {rc:%X}] [Master CPU-%d]\r\n", pName, pShareInfo, FEC_UL_CEVAX_OWNER ? "/CEVA-OWNER" : "", (SUCCESSFUL(rc))? "OK":"FAIL", rc, MxGetCpuID ());

    return rc;
}


//-------------------------------------------------------------------------------------------
/** @brief This function identifies the ARM Core where the DL FEC Unit is being called from
 *
 *
 *  @param none
 *
 *  @return unsigned int CPU ID [0-3]
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 FecDLGetMasterCpu (void)
{
    PFEC_DL_CTX pFecDlCtx = FecDlGetCtx();

    _ASSERT_PTR(pFecDlCtx);

    if (!pFecDlCtx->bInited)
        return 0xFFFF;

    return pFecDlCtx->CpuID;
}


//-------------------------------------------------------------------------------------------
/** @brief This function identifies the ARM Core where the UL FEC Unit is being called from
 *
 *
 *  @param none
 *
 *  @return unsigned int CPU ID [0-3]
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 FecULGetMasterCpu (void)
{
    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();

    _ASSERT_PTR(pFecUlCtx);

    if (!pFecUlCtx->bInited)
        return 0xFFFF;

    return pFecUlCtx->CpuID;
}

void FecAckIrq(UINT32 irq)
{
    switch (irq)
    {
        case HAL_IRQ_FEC_DL:
        {
            volatile FEC_CTRL_REGS * pReg = (volatile FEC_CTRL_REGS *)FEC_CTRL_REGS_BASE;

            pReg->DMA0OutCtrl.IrqStatusClear = DMAIRQ_FCTRL;
            pReg->DMA0InCtrl.IrqStatusClear = DMAIRQ_FCTRL;

            break;
        }

        case HAL_IRQ_FEC_UL:
        {
            volatile FEC_CTRL_REGS * pReg = (volatile FEC_CTRL_REGS *)FEC_CTRL_REGS_BASE;

            pReg->DMA1OutCtrl.IrqStatusClear = DMAIRQ_FCTRL;
            pReg->DMA1InCtrl.IrqStatusClear = DMAIRQ_FCTRL;

            break;
        }
    }
}

BOOL FecIsDlHang (PFEC_DL_CTX pFecDlCtx)
{
    UINT32 total;

    _ASSERT_PTR(pFecDlCtx);

    if (pFecDlCtx->bInited == 0)
        return FALSE;

    if (pFecDlCtx->ListInfPtr->pFDescActive == NULL)
        return FALSE;

    total = pFecDlCtx->ListInfPtr->ActiveTotalTicks;

    if (total == 0)
        return FALSE;

    return (MxGetTicks () - pFecDlCtx->ListInfPtr->StartTimeTicks) >= total;
}

BOOL FecIsUlHang (PFEC_UL_CTX pFecUlCtx)
{
    UINT32 total;

    _ASSERT_PTR(pFecUlCtx);

    if (pFecUlCtx->bInited == 0)
        return FALSE;

    if (pFecUlCtx->ListInfPtr->pFDescActive == NULL)
        return FALSE;

    total = pFecUlCtx->ListInfPtr->ActiveTotalTicks;

    if (total == 0)
        return FALSE;

    return (MxGetTicks () - pFecUlCtx->ListInfPtr->StartTimeTicks) >= total;
}

BOOL FecIsDlFree (PFEC_DL_CTX pFecDlCtx)
{
    if (pFecDlCtx->bInited == 0)
        return TRUE;

    return (pFecDlCtx->ListInfPtr->pFDescActive == NULL);
}

BOOL FecIsUlFree (PFEC_UL_CTX pFecUlCtx)
{
    if (pFecUlCtx->bInited == 0)
        return TRUE;

    return (pFecUlCtx->ListInfPtr->pFDescActive == NULL);
}

void FecDlComplete(PFEC_DL_CTX pFecDlCtx)
{
    PFEC_DL_FDESC pFDesc;

    UINT32 cbDone, cbDoneIsCeva, put;

    #ifdef _MX_DEBUG
    UINT32 get;
    #endif

    pFecDlCtx->EndTicks = Get24MHzTick();
    pFecDlCtx->CompFlag = 1;

    while ((pFDesc = FecDlShiftActive(pFecDlCtx)) != NULL)
    {
        // The logic of this code is to detect is it ARM proc address
        // or CEVA proc address, in case of CEVA, we need to put this
        // address to the queue and CEVA FEC driver will call these callbacks

        cbDone = (UINT32)pFDesc->CbDone;
        cbDoneIsCeva = ((cbDone & FEC_CEVA_CB_DONE_PROC) == FEC_CEVA_CB_DONE_PROC);

        if (cbDoneIsCeva)
            cbDone &= ~FEC_CEVA_CB_DONE_PROC;

        if (cbDoneIsCeva && cbDone != NULL)
        {
            pFDesc->CbDone = NULL;  // To prevent usage of this CBDONE on CEVA again

            put = pFecDlCtx->ListInfPtr->CbDonePut;

            #ifdef _MX_DEBUG
            get = pFecDlCtx->ListInfPtr->CbDoneGet;
            #endif

            pFecDlCtx->ListInfPtr->CbDonePtr[put] = pFDesc->CbContext;
            pFecDlCtx->ListInfPtr->CbDone   [put] = cbDone;

            put ++;

            if (put >= MAX_CEVA_CB_DONE_NUM)
            {
                pFecDlCtx->ListInfPtr->CbDonePut = 0;
            }
            else
            {
                pFecDlCtx->ListInfPtr->CbDonePut = put;
            }

            // to detect queue overflow
            _ASSERT(put != get);

        }
        else if (cbDone != NULL)
        {
            ((FecCbDoneType)cbDone)(pFDesc->CbContext);
        }

        if (pFDesc->OwnerCpuID < CPU_NUM)
        {
            MxiPoolFree(&pFecDlCtx->Pool[pFDesc->OwnerCpuID], pFDesc);
        }
        else
        {
            MxiPoolFree(pFecDlCtx->CevaPoolPtr, pFDesc);
        }
    }
}

void FecUlComplete(PFEC_UL_CTX pFecUlCtx)
{
    PFEC_UL_FDESC pFDesc;

    UINT32 cbDone, cbDoneIsCeva, put;

    #ifdef _MX_DEBUG
    UINT32 get;
    #endif

    pFecUlCtx->EndTicks = Get24MHzTick();
    pFecUlCtx->CompFlag = 1;

    while ((pFDesc = FecUlShiftActive(pFecUlCtx)) != NULL)
    {
        // The logic of this code is to detect is it ARM proc address
        // or CEVA proc address, in case of CEVA, we need to put this
        // address to the queue and CEVA FEC driver will call these callbacks

        cbDone = (UINT32)pFDesc->CbDone;
        cbDoneIsCeva = ((cbDone & FEC_CEVA_CB_DONE_PROC) == FEC_CEVA_CB_DONE_PROC);

        if (cbDoneIsCeva)
            cbDone &= ~FEC_CEVA_CB_DONE_PROC;

        if (cbDoneIsCeva && cbDone != NULL)
        {
            pFDesc->CbDone = NULL;  // To prevent usage of this CBDONE on CEVA again

            put = pFecUlCtx->ListInfPtr->CbDonePut;

            #ifdef _MX_DEBUG
            get = pFecUlCtx->ListInfPtr->CbDoneGet;
            #endif

            pFecUlCtx->ListInfPtr->CbDonePtr[put] = pFDesc->CbContext;
            pFecUlCtx->ListInfPtr->CbDone   [put] = cbDone;

            put ++;

            if (put >= MAX_CEVA_CB_DONE_NUM)
            {
                pFecUlCtx->ListInfPtr->CbDonePut = 0;
            }
            else
            {
                pFecUlCtx->ListInfPtr->CbDonePut = put;
            }

            // to detect queue overflow
            _ASSERT(put != get);

        }
        else if (cbDone != NULL)
        {
            ((FecCbDoneType)cbDone)(pFDesc->CbContext);
        }

        if (pFDesc->OwnerCpuID < CPU_NUM)
        {
            MxiPoolFree(&pFecUlCtx->Pool[pFDesc->OwnerCpuID], pFDesc);
        }
        else
        {
            MxiPoolFree(pFecUlCtx->CevaPoolPtr, pFDesc);
        }
    }
}

void FecDlRunInactive(PFEC_DL_CTX pFecDlCtx)
{
    if (FecDlGetActiveFdesc(pFecDlCtx) == NULL)
    {
        if (FecDlGetInactiveFdesc(pFecDlCtx) != NULL)
        {
            FecDlDma(FecDlGetInactTicks(pFecDlCtx));
        }
        else
        {
            FecDlSetBusy(pFecDlCtx, FALSE);
        }
    }
}

void FecUlRunInactive(PFEC_UL_CTX pFecUlCtx)
{
    if (FecUlGetActiveFdesc(pFecUlCtx) == NULL)
    {
        if (FecUlGetInactiveFdesc(pFecUlCtx) != NULL)
        {
            FecUlDma(FecUlGetInactTicks(pFecUlCtx));
        }
        else
        {
            FecUlSetBusy(pFecUlCtx, FALSE);
        }
    }
}

void FecRobustHandler(void * pPtr)
{
    IRQTYPE irq;
    BOOL dl_free, ul_free, dl_hang, ul_hang, reset;

    FEC_ROBUST_CTRL_CTX * pCtx = (FEC_ROBUST_CTRL_CTX *)pPtr;

    PFEC_DL_CTX pFecDlCtx = FecDlGetCtx();
    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();

    if (pFecDlCtx->bInited == 0 || pFecUlCtx->bInited == 0)
        return;

    irq = ARM_INT_disable();

    FecDlGetLock(pFecDlCtx);
    FecUlGetLock(pFecUlCtx);

    dl_free = FecIsDlFree(pFecDlCtx);
    ul_free = FecIsUlFree(pFecUlCtx);
    dl_hang = FecIsDlHang(pFecDlCtx);
    ul_hang = FecIsUlHang(pFecUlCtx);

    reset = (dl_hang && ul_hang)||(dl_hang && ul_free)||(ul_hang && dl_free);

    if (reset)
    {
        if (dl_hang)
            FecDlComplete(pFecDlCtx);

        if (ul_hang)
            FecUlComplete(pFecUlCtx);

        FecReset();

        FecDlRunInactive(pFecDlCtx);
        FecUlRunInactive(pFecUlCtx);

        pCtx->ResetNum ++;
    }

    FecDlReleaseLock(pFecDlCtx);
    FecUlReleaseLock(pFecUlCtx);
    
    ARM_INT_restore(irq);
}

MXRC FecRobustCtrl (UINT32 bInit, UINT32 nPeriod)
{
    IRQTYPE r;
    MXRC rc;
    FEC_ROBUST_CTRL_CTX * pCtx = FecGetRobustCtx();
    _ASSERT_PTR(pCtx);

    if (bInit == 0)
    {
        if (pCtx->Inited == 0 || pCtx->CpuID != MxGetCpuID())
            return MX_OK;

        r = ARM_INT_disable();

        if (HANDLE_NOT_NULL(pCtx->hTimer))
        {
            MxStopTimer(pCtx->hTimer);
            MxRemoveTimer(pCtx->hTimer);
        }

        memset (pCtx, 0, sizeof (*pCtx));

        ARM_INT_restore(r);

        return MX_OK;
    }

    if (pCtx->Inited != 0)
        return MX_OK;

    rc = MxCreateTimer(TIMER_MODE_PERIOD, nPeriod, NULL, HNULL, &pCtx->hTimer);

    if (FAILED(rc))
        return rc;

    rc = MxSetTimerProc(pCtx->hTimer, FecRobustHandler, pCtx);

    if (FAILED(rc))
        return rc;

    pCtx->CpuID   = MxGetCpuID();
    pCtx->Period  = nPeriod;
    pCtx->Inited  = 1;

    rc = MxStartFracTimer(pCtx->hTimer);

    uart_printf ("FEC-DL/UL robust control [RC:0x%x] [Precision:%d us] [Master CPU-%d]\r\n", rc, nPeriod * (1000/MX_TIMER_FRACTION), MxGetCpuID());
    
    return rc;
}

UINT32 FecGetResetNum(UINT32 nResetCounter)
{
    FEC_ROBUST_CTRL_CTX * pCtx = FecGetRobustCtx();
    UINT32 num;
    _ASSERT_PTR(pCtx);

    num = pCtx->ResetNum;

    if (nResetCounter)
        pCtx->ResetNum = 0;

    return num;
}

//-------------------------------------------------------------------------------------------
/** @brief This function initalizes both the UL and DL FEC Units. Allocates memory needed by driver
 *
 *
 *  @param   pFecConfigRegs Pointer to the Config register
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecInit(PFEC_CONFIG_REGS pFecConfigRegs)
{
    MXRC rc;
    volatile PUINT32 p_regs = (PUINT32)FEC_CONFIG_REGS_BASE;

#ifdef FEC_ADDITIONAL_CHECK_ENABLED
    FECFillMemRegions();
#endif // #ifdef FEC_ADDITIONAL_CHECK_ENABLED

    FecReset();

    *p_regs = *(PUINT32)pFecConfigRegs;

    if (FAILED(rc = FecDlInit(NULL)))
        return rc;

    if (FAILED(rc = FecUlInit(NULL)))
        return rc;

    return rc;
}


//-------------------------------------------------------------------------------------------
/** @brief This function shuts down the DL FEC Unit. Cleans up all allocated memory
 *
 *
 *  @param none
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecDlShutdown(void)
{
    PFEC_DL_CTX pFecDlCtx = FecDlGetCtx();

    // The FEC-DL can be stopped only if HW block was initialized
    // and shutdown proc. is called on CPU that was used for initialization

    if (pFecDlCtx->bInited && (pFecDlCtx->CpuID == MxGetCpuID ()))
    {
        if (pFecDlCtx->PoolStorPtr != NULL)
            MxHeapFree(FEC_HEAP, pFecDlCtx->PoolStorPtr);

        if (pFecDlCtx->PoolIndex != NULL)
            MxHeapFree(pFecDlCtx->ListInfHeap, pFecDlCtx->PoolIndex);

        if (pFecDlCtx->ListInfPtr != NULL)
            MxHeapFree(pFecDlCtx->ListInfHeap, pFecDlCtx->ListInfPtr);

        memset (pFecDlCtx, 0, sizeof (*pFecDlCtx));

        MxDisableInt (HAL_IRQ_FEC_DL);

        pFecDlCtx->bInited = FALSE;
    }

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function just stops device and does not free any resources
 *          like allocated memory. It's needed to save allocated resources for
 *          the futher debugging and do not touch potentially corrupted heaps
 *
 *
 *  @param none
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecDlGenShedding(void)
{
    PFEC_DL_CTX pFecDlCtx = FecDlGetCtx();

    // The FEC-DL can be stopped only if HW block was initialized
    // and shutdown proc. is called on CPU that was used for initialization

    if (pFecDlCtx->bInited && (pFecDlCtx->CpuID == MxGetCpuID ()))
    {
        MxDisableInt (HAL_IRQ_FEC_DL);
        pFecDlCtx->bInited = FALSE;
    }

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief This function shuts down the UL FEC Unit. Cleans up all allocated memory
 *
 *
 *  @param none
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecUlShutdown(void)
{
    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();

    // The FEC-UL can be stopped only if HW block was initialized
    // and shutdown proc. is called on CPU that was used for initialization

    if (pFecUlCtx->bInited && (pFecUlCtx->CpuID == MxGetCpuID ()))
    {
        if (pFecUlCtx->PoolStorPtr != NULL)
            MxHeapFree(FEC_HEAP, pFecUlCtx->PoolStorPtr);

        if (pFecUlCtx->PoolIndex != NULL)
            MxHeapFree(pFecUlCtx->ListInfHeap, pFecUlCtx->PoolIndex);

        if (pFecUlCtx->ListInfPtr != NULL)
            MxHeapFree(pFecUlCtx->ListInfHeap, pFecUlCtx->ListInfPtr);

        memset (pFecUlCtx, 0, sizeof (*pFecUlCtx));

        MxDisableInt (HAL_IRQ_FEC_UL);

        pFecUlCtx->bInited = FALSE;
    }

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function just stops device and does not free any resources
 *          like allocated memory. It's needed to save allocated resources for
 *          the futher debugging and do not touch potentially corrupted heaps
 *
 *
 *  @param none
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecUlGenShedding(void)
{
    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();

    // The FEC-UL can be stopped only if HW block was initialized
    // and shutdown proc. is called on CPU that was used for initialization

    if (pFecUlCtx->bInited && (pFecUlCtx->CpuID == MxGetCpuID ()))
    {
        MxDisableInt (HAL_IRQ_FEC_DL);
        pFecUlCtx->bInited = FALSE;
    }

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function shuts down both the UL and DL FEC Units. Cleans up all allocated memory
 *
 *
 *  @param none
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecShutdown(void)
{
    MXRC rc;

    #ifdef FEC_ROBUST_CTRL_ENABLED
    if (FAILED(rc = FecRobustCtrl(0, 0)))
        return rc;
    #endif

    if (FAILED(rc = FecDlShutdown()))
        return rc;

    if (FAILED(rc = FecUlShutdown()))
        return rc;

    FecReset();

    return rc;
}

//-------------------------------------------------------------------------------------------
/** @brief This function just stops device and does not free any resources
 *          like allocated memory. It's needed to save allocated resources for
 *          the futher debugging and do not touch potentially corrupted heaps
 *
 *
 *  @param none
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecGenShedding(void)
{
    MXRC rc;

    if (FAILED(rc = FecDlGenShedding()))
        return rc;

    if (FAILED(rc = FecUlGenShedding()))
        return rc;

    //FecReset();

    return rc;
}



//-------------------------------------------------------------------------------------------
/** @brief This function allocates descriptors and initializes them as needed for the DL FEC Unit
 *
 *
 *  @param NumDesc Number of descriptors to allocate
 *
 *  @return PFEC_DL_FDESC Pointer to the allocated Descriptors
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
PFEC_DL_FDESC FecDlAllocDesc(UINT32 NumDesc)
{
    PFEC_DL_CTX pFecDlCtx = FecDlGetCtx();
    PFEC_DL_FDESC pFDesc, pFDescPrev = NULL, pFDescHead = NULL;
    UINT32 i, cpu = MxGetCpuID();
    
    MXPOOL * pPool;

    _ASSERT_PTR(pFecDlCtx);

    pPool = &pFecDlCtx->Pool[cpu];

    for (i = 0; i < NumDesc; i++)
    {
        pFDesc = (PFEC_DL_FDESC)MxiPoolAlloc(pPool);

        if (pFDesc == NULL)
        {
            while (pFDescHead != NULL)
            {
                pFDescPrev = pFDescHead;
                pFDescHead = (PFEC_DL_FDESC)pFDescPrev->Next;

                MxiPoolFree(pPool, pFDescPrev);
            }
            return NULL;
        }

        pFDesc->OwnerCpuID = cpu;
        
        pFDesc->Next = NULL;
        pFDesc->CbDone = NULL;
        pFDesc->CbContext = NULL;

        pFDesc->HwContext.IRQEN = 0; // disable IRQ
        pFDesc->HwContext.StatusComp = 0;

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

    return pFDescHead;
}


//-------------------------------------------------------------------------------------------
/** @brief This function allocates descriptors and initializes them as needed for the UL FEC Unit
 *
 *
 *  @param NumDesc Number of descriptors to allocate
 *
 *  @return PFEC_UL_FDESC Pointer to the allocated Descriptors
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
PFEC_UL_FDESC FecUlAllocDesc(UINT32 NumDesc)
{
    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();
    PFEC_UL_FDESC pFDesc, pFDescPrev = NULL, pFDescHead = NULL;
    UINT32 i, cpu = MxGetCpuID();

    MXPOOL * pPool;

    _ASSERT_PTR(pFecUlCtx);

    pPool = &pFecUlCtx->Pool[cpu];
    for (i = 0; i < NumDesc; i++)
    {
        pFDesc = (PFEC_UL_FDESC)MxiPoolAlloc(pPool);
        
        if (pFDesc == NULL)
        {
            while (pFDescHead != NULL)
            {
                pFDescPrev = pFDescHead;
                pFDescHead = (PFEC_UL_FDESC)pFDescPrev->Next;
                
                MxiPoolFree(pPool, pFDescPrev);
            }
            return NULL;
        }

        pFDesc->OwnerCpuID = cpu;
        
        pFDesc->Next = NULL;
        pFDesc->CbDone = NULL;
        pFDesc->CbContext = NULL;

        pFDesc->HwContext.IRQEN = 0; // disable IRQ
        pFDesc->HwContext.StatusComp = 0;

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

    return pFDescHead;
}


//-------------------------------------------------------------------------------------------
/** @brief This function allocates a new descriptor for the UL FEC Unit and chains it with the old descriptor
 *
 *
 *  @param pold Pointer to Old Descrptor
 *
 *  @return PFEC_UL_FDESC Pointer to the new allocated Descriptor
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
PFEC_UL_FDESC FecUlAllocNewDesc(PFEC_UL_FDESC pold)
{
    PFEC_UL_FDESC pnew;
    UINT32 cpu = MxGetCpuID();

    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();
    _ASSERT_PTR(pFecUlCtx);
    pnew = (PFEC_UL_FDESC)MxiPoolAlloc (&pFecUlCtx->Pool[cpu]);

    _ASSERT_PTR(pnew);

    if (pnew == NULL)
        return NULL;

    pnew->OwnerCpuID = cpu;

    pnew->Next = NULL;
    pnew->CbDone = NULL;
    pnew->CbContext = NULL;

    pnew->HwContext.IRQEN = 0; // disable IRQ
    pnew->HwContext.StatusComp = 0;

    if (pold != NULL)
    {
        pold->Next = pnew;
    }

    return pnew;
}


//-------------------------------------------------------------------------------------------
/** @brief This function setups a callback function to the last DL FEC descriptor in the chain of descriptors.
 *
 *  @param pFDesc       Pointer to the descriptor
 *  @param CbDone      Pointer to the callback function
 *  @param CbContext  Callback function's context
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecDlSetLastDesc(PFEC_DL_FDESC pFDesc, void (*CbDone)(LPVOID), LPVOID CbContext)
{
    PFEC_DL_FDESC p = pFDesc;

#ifdef FEC_ADDITIONAL_CHECK_ENABLED
    UINT32 num = 0;
    UINT32 max;
    UINT32 lr = __return_address () - 4;
#endif

    _ASSERT_PTR(pFDesc);

#ifdef FEC_ADDITIONAL_CHECK_ENABLED
    max = FECGetPoolTotalSize(ID_FEC_DL);
#endif

    while (p->Next)
    {
        p->CbDone = NULL;
        
        p->HwContext.IRQEN = 0; // to disable IRQ
        p->HwContext.StatusComp = 0;

#ifdef FEC_ADDITIONAL_CHECK_ENABLED
        if (!FecDlCheckFDescAddress(p))
        {
            uart_printf ("FECDL-SetLast: Detected wrong chain (NextPtr) for fdesc:0x%x, num=%d, cpu-%d, mode=0x%x, lr=0x%x (cbDone=%x(%x))\r\n", 
                        pFDesc,
                        num,
                        MxGetCpuID(),
                        MxGetCpuMode(),
                        lr,
                        CbDone,
                        CbContext
                );

            _ASSERT(0);
        }

        if (!FecDlCheckIOBufAddress(p))
        {
            uart_printf ("FECDL-SetLast Detected wrong IO Buf in FDesc(0x%x), cpu-%d, mode=0x%x, lr=0x%x\r\n",
                            p,
                            MxGetCpuID(),
                            MxGetCpuMode(),
                            lr);
            _ASSERT(0);
        }
        

        num ++;

        if (num >= max)
        {
            uart_printf ("FECDL-SetLast: Detected wrong chain for fdesc:0x%x, num=%d, cpu-%d, mode=0x%x, lr=0x%x (cbDone=%x(%x))\r\n", 
                        pFDesc,
                        num,
                        MxGetCpuID(),
                        MxGetCpuMode(),
                        lr,
                        CbDone,
                        CbContext
                );
            
            _ASSERT(0);
        }
#endif

        p = (PFEC_DL_FDESC)p->Next;
    }

    p->HwContext.IRQEN = 1; // enable IRQ
    p->HwContext.StatusComp = 0;
    
    p->CbDone = CbDone;
    p->CbContext = CbContext;

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief This function setups a callback function to the last UL FEC descriptor in the chain of descriptors.
 *
 *  @param pFDesc       Pointer to the descriptor
 *  @param CbDone      Pointer to the callback function
 *  @param CbContext  Callback function's context
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecUlSetLastDesc(PFEC_UL_FDESC pFDesc, void (*CbDone)(LPVOID), LPVOID CbContext)
{
    PFEC_UL_FDESC p = pFDesc;

#ifdef FEC_ADDITIONAL_CHECK_ENABLED
    UINT32 num = 0;
    UINT32 max;
    UINT32 lr = __return_address () - 4;
#endif

    _ASSERT_PTR(pFDesc);

#ifdef FEC_ADDITIONAL_CHECK_ENABLED
    max = FECGetPoolTotalSize(ID_FEC_UL);
#endif

    while (p->Next)
    {
        p->CbDone = NULL;
    
        p->HwContext.IRQEN = 0;      // to disable IRQ
        p->HwContext.StatusComp = 0; // to mark like non processed

#ifdef FEC_ADDITIONAL_CHECK_ENABLED
        if (!FecUlCheckFDescAddress(p))
        {
            uart_printf ("FECUL-SetLast: Detected wrong chain (NextPtr) for fdesc:0x%x, num=%d, cpu-%d, mode=0x%x, lr=0x%x (cbDone=%x(%x))\r\n", 
                        pFDesc,
                        num,
                        MxGetCpuID(),
                        MxGetCpuMode(),
                        lr,
                        CbDone,
                        CbContext
                );

            _ASSERT(0);
        }

        if (!FecULCheckIOBufAddress(p))
        {
            uart_printf ("FECUL-SetLast: Detected wrong IO Buf in FDesc(0x%x), cpu-%d, mode=0x%x, lr=0x%x\r\n",
                            (p),
                            MxGetCpuID(),
                            MxGetCpuMode(),
                            lr);
            _ASSERT(0);
        }
        

        num ++;

        if (num >= max)
        {
            uart_printf ("FECUL-SetLast: Detected wrong chain for fdesc:0x%x, num=%d, cpu-%d, mode=0x%x, lr=0x%x (cbDone=%x(%x))\r\n", 
                        pFDesc,
                        num,
                        MxGetCpuID(),
                        MxGetCpuMode(),
                        lr,
                        CbDone,
                        CbContext
                );
            
            _ASSERT(0);
        }
#endif
        
        p = (PFEC_UL_FDESC)p->Next;
    }

    p->HwContext.IRQEN = 1;      // enable IRQ
    p->HwContext.StatusComp = 0; // to mark like non processed0
    
    p->CbDone = CbDone;
    p->CbContext = CbContext;

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function initializes the DL FEC's DMA module and dispacthes to run. Automatically called by top level FEC functions
 *
 *  @param none
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
void FecDlDma(UINT32 TotalTicks)
{
    PFEC_DL_CTX pFecDlCtx = FecDlGetCtx();
    PFEC_DL_FDESC pAct;
    volatile FEC_CTRL_REGS *pFecCtrl = pFecDlCtx->pFecCtrlRegs;

    FecDlSetBusy(pFecDlCtx, TRUE);
    FecDlSetActiveFdesc(pFecDlCtx, (pAct = FecDlGetInactiveFdesc(pFecDlCtx)));
    FecDlSetInactiveFdesc(pFecDlCtx, NULL);
    pFecDlCtx->StartTicks = Get24MHzTick();
    pFecDlCtx->CompFlag = 0;

    FecDlSetRunTime (pFecDlCtx, pFecDlCtx->StartTicks, TotalTicks);

    // start DMA
    pFecCtrl->DMA0InCtrl.Control     = DMA_FCOM_MODE;
    pFecCtrl->DMA0InCtrl.BurstLength = 15;
    pFecCtrl->DMA0OutCtrl.HeadFDesc  = (UINT32)pAct;
    pFecCtrl->DMA0InCtrl.HeadFDesc   = (UINT32)pAct;
}


//-------------------------------------------------------------------------------------------
/** @brief This function initializes the UL FEC's DMA module and dispacthes to run. Automatically called by top level FEC functions
 *
 *  @param none
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
void FecUlDma(UINT32 TotalTicks)
{
    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();
    volatile FEC_CTRL_REGS *pFecCtrl = pFecUlCtx->pFecCtrlRegs;
    PFEC_UL_FDESC pAct;
    
    FecUlSetBusy(pFecUlCtx, TRUE);
    FecUlSetActiveFdesc(pFecUlCtx, (pAct = FecUlGetInactiveFdesc(pFecUlCtx)));
    FecUlSetInactiveFdesc(pFecUlCtx, NULL);
    pFecUlCtx->StartTicks = Get24MHzTick();
    pFecUlCtx->CompFlag = 0;

    FecUlSetRunTime (pFecUlCtx, pFecUlCtx->StartTicks, TotalTicks);
    
    // start DMA
    pFecCtrl->DMA1InCtrl.Control     = DMA_FCOM_MODE;
    pFecCtrl->DMA1InCtrl.BurstLength = 15;
    pFecCtrl->DMA1OutCtrl.HeadFDesc  = (UINT32)pAct;
    pFecCtrl->DMA1InCtrl.HeadFDesc   = (UINT32)pAct;
}

/** @brief This function is designed to be called in IRQ mode!!!
           the function shifts active pointer to the next element
           if this element is processed by DMA
           and returns a pointer to the current (non-shifted) element or NULL

    @param pFecCtx [in] - the pointer to the FEC-DL driver context

    @return [PFEC_DL_FDESC] the pointer to the non-shifted element (that is processed by MDMA) or NULL

    \ingroup group_lte_hal_fec

*/
PFEC_DL_FDESC  FecDlShiftActiveIfDone (PFEC_DL_CTX pFecCtx)
{
    PFEC_DL_FDESC pActFDesc = NULL;

    _ASSERT_PTR(pFecCtx);
    FecDlGetLock (pFecCtx);

    pActFDesc = FecDlGetActiveFdesc(pFecCtx);

    if (pActFDesc != NULL)
    {
        if (pActFDesc->HwContext.StatusComp != 0)
        {
            FecDlSetActiveFdesc(pFecCtx, (PFEC_DL_FDESC)pActFDesc->Next);
        }
        else
        {
            pActFDesc = NULL;
        }
    }

    FecDlReleaseLock (pFecCtx);

    return pActFDesc;
}

PFEC_DL_FDESC  FecDlShiftActive (PFEC_DL_CTX pFecCtx)
{
    PFEC_DL_FDESC pActFDesc = NULL;

    _ASSERT_PTR(pFecCtx);

    pActFDesc = FecDlGetActiveFdesc(pFecCtx);

    if (pActFDesc != NULL)
    {
        FecDlSetActiveFdesc(pFecCtx, (PFEC_DL_FDESC)pActFDesc->Next);
    }

    return pActFDesc;
}

/** @brief This function is designed to be called in IRQ mode!!!
           the function shifts active pointer to the next element
           if this element is processed by DMA
           and returns a pointer to the current (non-shifted) element or NULL

    @param pFecCtx [in] - the pointer to the FEC-UL driver context

    @return [PFEC_UL_FDESC] the pointer to the non-shifted element (that is processed by MDMA) or NULL

    \ingroup group_lte_hal_fec

*/
PFEC_UL_FDESC  FecUlShiftActiveIfDone (PFEC_UL_CTX pFecCtx)
{
    PFEC_UL_FDESC pActFDesc = NULL;

    _ASSERT_PTR(pFecCtx);
    FecUlGetLock (pFecCtx);

    pActFDesc = FecUlGetActiveFdesc(pFecCtx);

    if (pActFDesc != NULL)
    {
        if (pActFDesc->HwContext.StatusComp != 0)
        {
            FecUlSetActiveFdesc(pFecCtx, (PFEC_UL_FDESC)pActFDesc->Next);
        }
        else
        {
            pActFDesc = NULL;
        }
    }

    FecUlReleaseLock (pFecCtx);

    return pActFDesc;
}

PFEC_UL_FDESC  FecUlShiftActive (PFEC_UL_CTX pFecCtx)
{
    PFEC_UL_FDESC pActFDesc = NULL;

    _ASSERT_PTR(pFecCtx);

    pActFDesc = FecUlGetActiveFdesc(pFecCtx);

    if (pActFDesc != NULL)
    {
        FecUlSetActiveFdesc(pFecCtx, (PFEC_UL_FDESC)pActFDesc->Next);
    }

    return pActFDesc;
}

//-------------------------------------------------------------------------------------------
/** @brief This function processes the IRQ when the DL FEC returns from a task. It also dispacthes any other existing calls
 *              that have been queued up in the chain
 *
 *  @param none
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecDlProcInt(void)
{
    MXRC rc = MX_OK;
    PFEC_DL_CTX pFecDlCtx = FecDlGetCtx();
    PFEC_DL_FDESC pFDesc;

    UINT32 cbDone, cbDoneIsCeva, put;

    #ifdef _MX_DEBUG
    UINT32 get;
    #endif

    pFecDlCtx->EndTicks = Get24MHzTick();
    pFecDlCtx->CompFlag = 1;

    while ((pFDesc = FecDlShiftActiveIfDone(pFecDlCtx)) != NULL)
    {
        // The logic of this code is to detect is it ARM proc address
        // or CEVA proc address, in case of CEVA, we need to put this
        // address to the queue and CEVA FEC driver will call these callbacks

        cbDone = (UINT32)pFDesc->CbDone;
        cbDoneIsCeva = ((cbDone & FEC_CEVA_CB_DONE_PROC) == FEC_CEVA_CB_DONE_PROC);

        if (cbDoneIsCeva)
            cbDone &= ~FEC_CEVA_CB_DONE_PROC;

        if (cbDoneIsCeva && cbDone != NULL)
        {
            pFDesc->CbDone = NULL;  // To prevent usage of this CBDONE on CEVA again

            put = pFecDlCtx->ListInfPtr->CbDonePut;

            #ifdef _MX_DEBUG
            get = pFecDlCtx->ListInfPtr->CbDoneGet;
            #endif

            pFecDlCtx->ListInfPtr->CbDonePtr[put] = pFDesc->CbContext;
            pFecDlCtx->ListInfPtr->CbDone   [put] = cbDone;

            //uart_printf ("FEC-DL add cbDone(%x)\r\n", cbDone);

            put ++;

            if (put >= MAX_CEVA_CB_DONE_NUM)
            {
                pFecDlCtx->ListInfPtr->CbDonePut = 0;
            }
            else
            {
                pFecDlCtx->ListInfPtr->CbDonePut = put;
            }

            // to detect queue overflow
            _ASSERT(put != get);

        }
        else if (cbDone != NULL)
        {
            ((FecCbDoneType)cbDone)(pFDesc->CbContext);
        }

        if (pFDesc->OwnerCpuID < CPU_NUM)
        {
            MxiPoolFree(&pFecDlCtx->Pool[pFDesc->OwnerCpuID], pFDesc);
        }
        else
        {
            //uart_printf ("FEC-DL free ceva pool\r\n");
            MxiPoolFree(pFecDlCtx->CevaPoolPtr, pFDesc);
        }
    }

    FecDlGetLock (pFecDlCtx);

    // The lock was already gotten to process inactive lists
    // but in this time (before lock) CEVA might detected that Active list is NULL
    // and started new list from <inactive one>

    if (FecDlGetActiveFdesc(pFecDlCtx) == NULL)
    {
        if (FecDlGetInactiveFdesc(pFecDlCtx) != NULL)
        {
            FecDlDma(FecDlGetInactTicks(pFecDlCtx));
        }
        else
        {
            FecDlSetBusy(pFecDlCtx, FALSE);
        }
    }

    FecDlReleaseLock (pFecDlCtx);

    return rc;
}

//-------------------------------------------------------------------------------------------
/** @brief This function processes the IRQ when the UL FEC returns from a task. It also dispacthes any other existing calls
 *              that have been queued up in the chain
 *
 *  @param none
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecUlProcInt(void)
{
    MXRC rc = MX_OK;
    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();
    PFEC_UL_FDESC pFDesc;

    UINT32 cbDone, cbDoneIsCeva, put;

    #ifdef _MX_DEBUG
    UINT32 get;
    #endif

    pFecUlCtx->EndTicks = Get24MHzTick();
    pFecUlCtx->CompFlag = 1;

    while ((pFDesc = FecUlShiftActiveIfDone(pFecUlCtx)) != NULL)
    {
        // The logic of this code is to detect is it ARM proc address
        // or CEVA proc address, in case of CEVA, we need to put this
        // address to the queue and CEVA FEC driver will call these callbacks

        cbDone = (UINT32)pFDesc->CbDone;
        cbDoneIsCeva = ((cbDone & FEC_CEVA_CB_DONE_PROC) == FEC_CEVA_CB_DONE_PROC);

        if (cbDoneIsCeva)
            cbDone &= ~FEC_CEVA_CB_DONE_PROC;

        if (cbDoneIsCeva && cbDone != NULL)
        {
            pFDesc->CbDone = NULL;  // To prevent usage of this CBDONE on CEVA again

            put = pFecUlCtx->ListInfPtr->CbDonePut;

            #ifdef _MX_DEBUG
            get = pFecUlCtx->ListInfPtr->CbDoneGet;
            #endif

            pFecUlCtx->ListInfPtr->CbDonePtr[put] = pFDesc->CbContext;
            pFecUlCtx->ListInfPtr->CbDone   [put] = cbDone;

            put ++;

            if (put >= MAX_CEVA_CB_DONE_NUM)
            {
                pFecUlCtx->ListInfPtr->CbDonePut = 0;
            }
            else
            {
                pFecUlCtx->ListInfPtr->CbDonePut = put;
            }

            // to detect queue overflow
            _ASSERT(put != get);

        }
        else if (cbDone != NULL)
        {
            ((FecCbDoneType)cbDone)(pFDesc->CbContext);
        }

        if (pFDesc->OwnerCpuID < CPU_NUM)
            MxiPoolFree(&pFecUlCtx->Pool[pFDesc->OwnerCpuID], pFDesc);
        else
            MxiPoolFree(pFecUlCtx->CevaPoolPtr, pFDesc);
    }

    FecUlGetLock (pFecUlCtx);

    // The lock was already gotten to process inactive lists
    // but in this time (before lock) CEVA might detected that Active list is NULL
    // and started new list from <inactive one>

    if (FecUlGetActiveFdesc(pFecUlCtx) == NULL)
    {
        if (FecUlGetInactiveFdesc(pFecUlCtx) != NULL)
        {
            FecUlDma(FecUlGetInactTicks(pFecUlCtx));
        }
        else
        {
            FecUlSetBusy(pFecUlCtx, FALSE);
        }
    }

    FecUlReleaseLock (pFecUlCtx);

    return rc;
}

MXRC FecDlRunQ(PFEC_DL_FDESC pFDesc, UINT32 nNumUs)
{
    MXRC rc = MX_OK;
    PFEC_DL_CTX pFecDlCtx = FecDlGetCtx();
    IRQTYPE r;
    PFEC_DL_FDESC pFec;

#ifdef FEC_ADDITIONAL_CHECK_ENABLED
    UINT32 lr = __return_address () - 4;
#endif

    _ASSERT_PTR(pFDesc);

#ifdef FEC_ADDITIONAL_CHECK_ENABLED

    if (!FecDlCheckFDescAddress(pFDesc))
    {
        uart_printf ("FECDL-Run: Detected wrong FDesc(0x%x), cpu-%d, mode=0x%x, lr=0x%x\r\n",
                        pFDesc,
                        MxGetCpuID(),
                        MxGetCpuMode(),
                        lr);
        _ASSERT(0);
    }

    if (!FecDlCheckIOBufAddress(pFDesc))
    {
        uart_printf ("FECDL-Run: Detected wrong IO Buf in FDesc(0x%x), cpu-%d, mode=0x%x, lr=0x%x\r\n",
                        pFDesc,
                        MxGetCpuID(),
                        MxGetCpuMode(),
                        lr);
        _ASSERT(0);
    }

#endif 

    r = ARM_INT_disable();
    FecDlGetLock (pFecDlCtx);

    pFec = FecDlGetInactiveFdesc(pFecDlCtx);

    if (pFec == NULL)
    {
        FecDlSetInactiveFdesc(pFecDlCtx, pFDesc);
    }
    else
    {
        while (pFec->Next != NULL)
            pFec = (PFEC_DL_FDESC)pFec->Next;

        pFec->Next = pFDesc;
    }

    if (!FecDlGetBusy(pFecDlCtx))
    {
        FecDlDma(US_TO_TICKS(nNumUs));
    }
    else
    {
        FecDlAddInactTicks(pFecDlCtx, US_TO_TICKS(nNumUs));    
    }

    FecDlReleaseLock (pFecDlCtx);
    ARM_INT_restore(r);

    return rc;
}

//-------------------------------------------------------------------------------------------
/** @brief This function sends the DL FEC descriptor(s) for execution
 *
 *  @param pFDesc Pointer to the descriptor
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecDlRun(PFEC_DL_FDESC pFDesc)
{
#ifdef FEC_ADDITIONAL_CHECK_ENABLED
    UINT32 lr = __return_address () - 4;

    // in this part we need to check correctness of MDMA descriptors 
    // provided in parameter 

    if (!FecDlCheckFDescAddress(pFDesc))
    {
        uart_printf ("FECDL-Run: Detected wrong FDesc(0x%x), cpu-%d, mode=0x%x, lr=0x%x\r\n",
                        pFDesc,
                        MxGetCpuID(),
                        MxGetCpuMode(),
                        lr);
        _ASSERT(0);
    }

    if (!FecDlCheckIOBufAddress(pFDesc))
    {
        uart_printf ("FECDL-Run: Detected wrong IO Buf in FDesc(0x%x), cpu-%d, mode=0x%x, lr=0x%x\r\n",
                        pFDesc,
                        MxGetCpuID(),
                        MxGetCpuMode(),
                        lr);
        _ASSERT(0);
    }

#endif

    _ASSERT_PTR(pFDesc);
    return FecDlRunQ(pFDesc, 0);
}

//-------------------------------------------------------------------------------------------
/** @brief This function sends the UL FEC descriptor(s) for execution
 *
 *  @param pFDesc Pointer to the descriptor
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecUlRunQ(PFEC_UL_FDESC pFDesc, UINT32 nNumUs)
{
    MXRC rc = MX_OK;
    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();
    PFEC_UL_FDESC pFec;
    IRQTYPE r;
    
#ifdef FEC_ADDITIONAL_CHECK_ENABLED
    UINT32 lr = __return_address () - 4;

    if (!FecUlCheckFDescAddress(pFDesc))
    {
        uart_printf ("FECUL-Run Detected wrong FDesc(0x%x), cpu-%d, mode=0x%x, lr=0x%x\r\n",
                        pFDesc,
                        MxGetCpuID(),
                        MxGetCpuMode(),
                        lr);
        _ASSERT(0);
    }

    if (!FecULCheckIOBufAddress(pFDesc))
    {
        uart_printf ("FECUL-Run Detected wrong IO Buf in FDesc(0x%x), cpu-%d, mode=0x%x, lr=0x%x\r\n",
                        pFDesc,
                        MxGetCpuID(),
                        MxGetCpuMode(),
                        lr);
        _ASSERT(0);
    }

#endif // #ifdef FEC_ADDITIONAL_CHECK_ENABLED

    _ASSERT_PTR(pFDesc);

    r = ARM_INT_disable();
    FecUlGetLock (pFecUlCtx);

    pFec = FecUlGetInactiveFdesc(pFecUlCtx);

    if (pFec == NULL)
    {
        FecUlSetInactiveFdesc(pFecUlCtx, pFDesc);
    }
    else
    {
        while (pFec->Next != NULL)
            pFec = (PFEC_UL_FDESC)pFec->Next;

        pFec->Next = pFDesc;
    }

    if (!FecUlGetBusy(pFecUlCtx))
    {
        FecUlDma(US_TO_TICKS(nNumUs));
    }
    else
    {
        FecUlAddInactTicks(pFecUlCtx, US_TO_TICKS(nNumUs));
    }

    FecUlReleaseLock (pFecUlCtx);
    ARM_INT_restore(r);

    return rc;
}

MXRC FecUlRun(PFEC_UL_FDESC pFDesc)
{
#ifdef FEC_ADDITIONAL_CHECK_ENABLED

    UINT32 lr = __return_address () - 4;

    if (!FecUlCheckFDescAddress(pFDesc))
    {
        uart_printf ("FECUL-Run Detected wrong FDesc(0x%x), cpu-%d, mode=0x%x, lr=0x%x\r\n",
                        pFDesc,
                        MxGetCpuID(),
                        MxGetCpuMode(),
                        lr);
        _ASSERT(0);
    }

    if (!FecULCheckIOBufAddress(pFDesc))
    {
        uart_printf ("FECUL-Run Detected wrong IO Buf in FDesc(0x%x), cpu-%d, mode=0x%x, lr=0x%x\r\n",
                        pFDesc,
                        MxGetCpuID(),
                        MxGetCpuMode(),
                        lr);
        _ASSERT(0);
    }

#endif

    _ASSERT_PTR(pFDesc);

    return FecUlRunQ(pFDesc, 0);
}


//-------------------------------------------------------------------------------------------
/** @brief This function returns sets the Hardware context to the DL FEC Descriptor
 *
 *
 *  @param pFDesc           Pointer to the descriptor
 *  @param HwContext     Context to set
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecDlSetHwCtx(PFEC_DL_FDESC pFDesc, DLHWCTX HwContext)
{
    _ASSERT_PTR(pFDesc);

    HwContext.IRQEN = 0;
    HwContext.BOffset = 1;
    HwContext.StatusComp = 0;

    pFDesc->HwContext = HwContext;

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function returns sets the Hardware context to the UL FEC Descriptor
 *
 *
 *  @param pFDesc           Pointer to the descriptor
 *  @param HwContext     Context to set
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecUlSetHwCtx(PFEC_UL_FDESC pFDesc, ULHWCTX HwContext)
{
    _ASSERT_PTR(pFDesc);

    HwContext.IRQEN = 0;
    HwContext.BOffset = 1;
    HwContext.StatusComp = 0;

    pFDesc->HwContext = HwContext;

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief This function is used to set I/O pointers to the DL FEC Descriptor
 *
 *  @param pFDesc          Pointer to the MDMA descriptor
 *  @param pInData         Pointer to the Input array
 *  @param InDataSize     Size of the Input array
 *  @param pOutData       Pointer to the Output array
 *  @param OutDataSize   Size of the Output array
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
MXRC FecDlSetData(PFEC_DL_FDESC pFDesc, LPVOID pInData, UINT32 InDataSize, LPVOID pOutData, UINT32 OutDataSize)
{
    _ASSERT_PTR(pFDesc);

    pFDesc->IoBdesc[1].BPtr = (UINT32)pInData;
    pFDesc->IoBdesc[1].BCtrl = BCTRL_BLAST_MASK | InDataSize;
    if (SysCoreIsDCached((UINT32)pInData))
    {
        MxCacheClean(pInData, InDataSize);
    }

    pFDesc->IoBdesc[0].BPtr = (UINT32)pOutData;
    pFDesc->IoBdesc[0].BCtrl = BCTRL_BLAST_MASK | OutDataSize;
    if (SysCoreIsDCached((UINT32)pOutData))
    {
        // if buffer is not cache line aligned,
        // the non-aligned part should be cleaned before invalidation
        // to prevent data loss
        if (((UINT32)pOutData & 31) != 0 )
        {
            MxCacheClean(pOutData, 32);
        }
        MxCacheInvalidate(pOutData, OutDataSize);
    }

    return MX_OK;
}

MXRC FecDlSetDataEx(PFEC_DL_FDESC pFDesc, LPVOID pInData, UINT32 InDataSize, LPVOID pOutData, UINT32 OutDataSize,
    UINT32 CacheCtrl)
{
    _ASSERT_PTR(pFDesc);

    pFDesc->IoBdesc[1].BPtr = (UINT32)pInData;
    pFDesc->IoBdesc[1].BCtrl = BCTRL_BLAST_MASK | InDataSize;

    if (CacheCtrl & MDMA_CACHE_PROC_IN)
    {
        if (SysCoreIsDCached((UINT32)pInData))
        {
            MxCacheClean(pInData, InDataSize);
        }
    }

    pFDesc->IoBdesc[0].BPtr = (UINT32)pOutData;
    pFDesc->IoBdesc[0].BCtrl = BCTRL_BLAST_MASK | OutDataSize;

    if (CacheCtrl & MDMA_CACHE_PROC_OUT)
    {
        if (SysCoreIsDCached((UINT32)pOutData))
        {
            // if buffer is not cache line aligned,
            // the non-aligned part should be cleaned before invalidation
            // to prevent data loss
            if (((UINT32)pOutData & 31) != 0 )
            {
                MxCacheClean(pOutData, 32);
            }
            MxCacheInvalidate(pOutData, OutDataSize);
        }
    }

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief This function is used to set I/O pointers to the DL FEC Descriptor
 *
 *  @param pFDesc          Pointer to the MDMA descriptor
 *  @param pInData         Pointer to the Input array
 *  @param InDataSize     Size of the Input array
 *  @param pOutData       Pointer to the Output array
 *  @param OutDataSize   Size of the Output array
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------

ASMPROC void FecUlSetBDesc12 (void * pBdesc, LPVOID pInData[], UINT32 nSize)
{
    // r0 - pBdesc
    // r1 - pInData
    // r2 - nSize
    push {r3-r8}

    mov r4, r2
    mov r6, r2
    mov r8, r2
    
    ldmia r1!, {r3, r5, r7}
    stmia r0!, {r3, r4, r5, r6, r7, r8}

    ldmia r1!, {r3, r5, r7}
    stmia r0!, {r3, r4, r5, r6, r7, r8}

    ldmia r1!, {r3, r5, r7}
    stmia r0!, {r3, r4, r5, r6, r7, r8}

    ldmia r1!, {r3, r5, r7}
    orr r8, r8, #BCTRL_BLAST_MASK
    stmia r0!, {r3, r4, r5, r6, r7, r8}

    pop {r3-r8}
    bx lr
}

ASMPROC void FecUlSetBDesc14 (void * pBdesc, LPVOID pInData[], UINT32 nSize)
{
    // r0 - pBdesc
    // r1 - pInData
    // r2 - nSize
    push {r3-r8}

    mov r4, r2
    mov r6, r2
    mov r8, r2
    
    ldmia r1!, {r3, r5, r7}
    stmia r0!, {r3, r4, r5, r6, r7, r8}

    ldmia r1!, {r3, r5, r7}
    stmia r0!, {r3, r4, r5, r6, r7, r8}

    ldmia r1!, {r3, r5, r7}
    stmia r0!, {r3, r4, r5, r6, r7, r8}

    ldmia r1!, {r3, r5, r7}
    stmia r0!, {r3, r4, r5, r6, r7, r8}

    ldmia r1!, {r3, r5}
    orr r6, r6, #BCTRL_BLAST_MASK
    stmia r0!, {r3, r4, r5, r6}

    pop {r3-r8}
    bx lr
}

MXRC FecUlSetDataOpt(PFEC_UL_FDESC pFDesc, LPVOID pInData[], UINT32 InDataSize, LPVOID pOutData, UINT32 OutDataSize)
{
    volatile  PFEC_CONFIG_REGS pFecConfigRegs;
    UINT32 BDescNum = 1;

    _ASSERT_PTR(pFDesc);

    pFecConfigRegs = (PFEC_CONFIG_REGS)FEC_CONFIG_REGS_BASE;

    if (pFecConfigRegs->bp_channel_interleaver != 1)
    {
        BDescNum = (pFecConfigRegs->ci_symbol_number == 1) ? 14 : 12;
    }

    pFDesc->IoBdesc[0].BPtr = (UINT32)pOutData;
    pFDesc->IoBdesc[0].BCtrl = BCTRL_BLAST_MASK | OutDataSize;

    if (BDescNum == 1)
    {
        pFDesc->IoBdesc[1].BPtr = (UINT32)pInData[0];
        pFDesc->IoBdesc[1].BCtrl = InDataSize | BCTRL_BLAST_MASK;
    }
    else if (BDescNum == 12)
    {
        FecUlSetBDesc12((void*)&pFDesc->IoBdesc[1], pInData, InDataSize);
    }
    else if (BDescNum == 14)
    {
        FecUlSetBDesc14((void*)&pFDesc->IoBdesc[1], pInData, InDataSize);
    }

    return MX_OK;
}

MXRC FecUlSetData(PFEC_UL_FDESC pFDesc, LPVOID pInData[], UINT32 InDataSize[], LPVOID pOutData, UINT32 OutDataSize)
{
    volatile  PFEC_CONFIG_REGS pFecConfigRegs;
    //PFECDRV_CTX pFecDrvCtx = FECDrv_ctx();
    UINT32 BDescNum, i;
    DMABDESC locBdesc[MAX_UL_IO_BDESC];

    _ASSERT_PTR(pFDesc);

#if 1
    pFecConfigRegs = (PFEC_CONFIG_REGS)FEC_CONFIG_REGS_BASE;

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
    if (SysCoreIsDCached((UINT32)pOutData))
    {
        // if buffer is not cache line aligned,
        // the non-aligned part should be cleaned before invalidation
        // to prevent data loss
        if (((UINT32)pOutData & 31) != 0 )
        {
            MxCacheClean(pOutData, 32);
        }
        MxCacheInvalidate(pOutData, OutDataSize);
    }

    for (i = 0; i < BDescNum; ++i)
    {
        locBdesc[i + 1].BPtr = (UINT32)pInData[i];
        locBdesc[i + 1].BCtrl = 0x80000000 | InDataSize[i];
        if (SysCoreIsDCached((UINT32)pInData[i]))
        {
            MxCacheClean(pInData[i], InDataSize[i]);
        }
    }

    locBdesc[BDescNum].BCtrl |= BCTRL_BLAST_MASK;

    _memcpy(&pFDesc->IoBdesc, &locBdesc, sizeof(DMABDESC)*(BDescNum + 1));
#else
    FecUlSetDataOpt(pFDesc, pInData, InDataSize[0], pOutData, OutDataSize);
#endif

    return MX_OK;
}

MXRC FecUlSetDataEx(PFEC_UL_FDESC pFDesc, LPVOID pInData[], UINT32 InDataSize[], LPVOID pOutData, UINT32 OutDataSize,
    UINT32 CacheCtrl)
{
    volatile  PFEC_CONFIG_REGS pFecConfigRegs;
    //PFECDRV_CTX pFecDrvCtx = FECDrv_ctx();
    UINT32 BDescNum, i;
    DMABDESC locBdesc[MAX_UL_IO_BDESC];

    _ASSERT_PTR(pFDesc);

    pFecConfigRegs = (PFEC_CONFIG_REGS)FEC_CONFIG_REGS_BASE;

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
    locBdesc[0].BCtrl = 0x80000000 | BCTRL_BLAST_MASK | OutDataSize;
    
    if (CacheCtrl & MDMA_CACHE_PROC_OUT)
    {
        if (SysCoreIsDCached((UINT32)pOutData))
        {
            // if buffer is not cache line aligned,
            // the non-aligned part should be cleaned before invalidation
            // to prevent data loss
            if (((UINT32)pOutData & 31) != 0 )
            {
                MxCacheClean(pOutData, 32);
            }
            MxCacheInvalidate(pOutData, OutDataSize);
        }
    }

    for (i = 0; i < BDescNum; ++i)
    {
        locBdesc[i + 1].BPtr = (UINT32)pInData[i];
        locBdesc[i + 1].BCtrl = 0x80000000 | InDataSize[i];
        if (SysCoreIsDCached((UINT32)pInData[i]))
        {
            MxCacheClean(pInData[i], InDataSize[i]);
        }
    }

    locBdesc[BDescNum].BCtrl |= BCTRL_BLAST_MASK;

    _memcpy(&pFDesc->IoBdesc, &locBdesc, sizeof(DMABDESC)*(BDescNum + 1));

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  @param none
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 FecDlGetExecTicks(void)
{
    PFEC_DL_CTX pFecDlCtx = FecDlGetCtx();

    return pFecDlCtx->EndTicks - pFecDlCtx->StartTicks;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL
 *
 *  @param none
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_fec
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 FecUlGetExecTicks(void)
{
    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();

    return pFecUlCtx->EndTicks - pFecUlCtx->StartTicks;
}

//--------------------------------------------------------------------------------------------
/** @brief This function enables logging in FEC DL
 *  @parameters:
 *               logctrl - control word, can have FEC_LOGFDESCMODE value or FEC_LOGDATAMODE value or both values simultaneously
 *               pDst - pointer to buffer for logging
 *               datasize - size of buffer for logging
 *  @return value: if success - 0, otherwise - a code of error
 *  \ingroup group_lte_hal_fec
 *
**/
//--------------------------------------------------------------------------------------------
MXRC FecDlSetLog(UINT8 logctrl, LPVOID pDst, UINT32 datasize)
{
    MXRC rc = MX_OK;

    rc = LogMgrCreateLogEx(LOG_CTRL_RAD_MDMA, pDst, datasize, &g_FecDlCtx.hLog);
    if (rc != MX_OK)
        return rc;

    // initialization of radio MDMA driver
    rc = MdmaRadInit();
    if (rc != MX_OK)
    {
        uart_printf ("Cannot initialize radio MDMA driver.\n");
        return rc;
    }

    g_FecDlCtx.logctrl = logctrl;
    g_FecDlCtx.bLogEnable = TRUE;

    return rc;
}

//--------------------------------------------------------------------------------------------
/** @brief This function enables logging in FEC UL
 *  @parameters:
 *               logctrl - control word, can have FEC_LOGFDESCMODE value or FEC_LOGDATAMODE value or both values simultaneously
 *               pDst - pointer to buffer for logging
 *               datasize - size of buffer for logging
 *  @return value: if success - 0, otherwise - a code of error
 *  \ingroup group_lte_hal_fec
 *
**/
//--------------------------------------------------------------------------------------------
MXRC FecUlSetLog(UINT8 logctrl, LPVOID pDst, UINT32 datasize)
{
    MXRC rc = MX_OK;

    rc = LogMgrCreateLogEx(LOG_CTRL_RAD_MDMA, pDst, datasize, &g_FecUlCtx.hLog);
    if (rc != MX_OK)
        return rc;

    // initialization of radio MDMA driver
    rc = MdmaRadInit();
    if (rc != MX_OK)
    {
        uart_printf ("Cannot initialize radio MDMA driver.\n");
        return rc;
    }

    g_FecUlCtx.logctrl = logctrl;
    g_FecUlCtx.bLogEnable = TRUE;

    return rc;
}

//--------------------------------------------------------------------------------------------
/** @brief This function disables logging in FEC DL
 *  @parameters: no parameters
 *  @return value: a code of error
 *  \ingroup group_lte_hal_fec
 *
**/
//--------------------------------------------------------------------------------------------
MXRC FecDlCloseLog(void)
{
    MXRC rc = MX_OK;

    rc = LogMgrCloseLog(g_FecDlCtx.hLog);
    g_FecDlCtx.bLogEnable = FALSE;

    return rc;
}

//--------------------------------------------------------------------------------------------
/** @brief This function enables logging in FEC UL
 *  @parameters: no parameters
 *  @return value: a code of error
 *  \ingroup group_lte_hal_fec
 *
**/
//--------------------------------------------------------------------------------------------
MXRC FecUlCloseLog(void)
{
    MXRC rc = MX_OK;

    rc = LogMgrCloseLog(g_FecUlCtx.hLog);
    g_FecUlCtx.bLogEnable = FALSE;

    return rc;
}

void FecDlMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    if (FECGetPoolSize(ID_FEC_DL, nCpuID) != 0)
    {
        MmuMgrCfgMemAP(FEC_CTRL_REGS_BASE,  4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(FEC_RESET_REG,       4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
    }
}

void FecUlMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    if (FECGetPoolSize(ID_FEC_UL, nCpuID) != 0)
    {
        MmuMgrCfgMemAP(FEC_CTRL_REGS_BASE,  4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(FEC_RESET_REG,       4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
    }
}

UINT32 FecIsMemUsed(UINT32 nAddr, UINT32 nSize)
{
    return 0;
}

#ifdef FEC_ADDITIONAL_CHECK_ENABLED

BOOL FecDlCheckHanging(void)
{
    PFEC_DL_CTX pFecDlCtx = FecDlGetCtx();
    UINT32 timer = Get24MHzTick();

    if ( ((timer - pFecDlCtx->StartTicks) > FEC_RUN_TIMEOUT) && pFecDlCtx->pFDescActive )
    {
        pFecDlCtx->status |= FEC_DL_HANGED;
        
        return FALSE;
    }

    return TRUE;
}

BOOL FecUlCheckHanging(void)
{
    PFEC_UL_CTX pFecUlCtx = FecUlGetCtx();
    UINT32 timer = Get24MHzTick();

    if ( ((timer - pFecUlCtx->StartTicks) > FEC_RUN_TIMEOUT) && pFecUlCtx->pFDescActive )
    {
        pFecUlCtx->status |= FEC_UL_HANGED;
        
        return FALSE;
    }

    return TRUE;
}

BOOL FecDlCheckIOBufAddress(PFEC_DL_FDESC pFDesc)
{
    UINT32 Bptr,i = 0;

    for (i = 0; i < MAX_DL_IO_BDESC; ++i)
    {
        Bptr = pFDesc->IoBdesc[i].BPtr;

        if (!FECIsMemRegions(Bptr))
        {
            uart_printf("FecDlCheckIOBufAddress: wrong ptr = 0x%x\r\n", Bptr);
            return FALSE;        
        }

        if (pFDesc->IoBdesc[i].BCtrl & BCTRL_BLAST_MASK)
            break;
    }
    
    return TRUE;
}

BOOL FecULCheckIOBufAddress (PFEC_UL_FDESC pFDesc)
{
   UINT32 Bptr,i = 0;

    for (i = 0; i < MAX_UL_IO_BDESC; ++i)
    {
        Bptr = pFDesc->IoBdesc[i].BPtr;

        if (!FECIsMemRegions(Bptr))
        {
            uart_printf("FecULCheckIOBufAddress: wrong ptr = 0x%x\r\n", Bptr);
            return FALSE;        
        }

        if (pFDesc->IoBdesc[i].BCtrl & BCTRL_BLAST_MASK)
            break;
    }
    
    return TRUE;
}

void FECFillMemRegions(void)
{
    extern UINT8 Image$$PRV_MEM_CORE0$$Base [];

    memset (fec_mem_reg, 0x00, sizeof(fec_mem_reg));

    fec_mem_reg[0].start = (UINT32)Image$$PRV_MEM_CORE0$$Base;
    fec_mem_reg[0].end   = GetDDR0Size();   

    fec_mem_reg[1].start = GetDDR1Base();
    fec_mem_reg[1].end   = fec_mem_reg[1].start + GetDDR1Size();   

    fec_mem_reg[2].start = CRAM_BASEADDR;
    fec_mem_reg[2].end   = fec_mem_reg[2].start + CRAM_SIZE;

    fec_mem_reg[3].start = IRAM_HEAP_ADDR;
    fec_mem_reg[3].end   = fec_mem_reg[3].start + IRAM_HEAP_SIZE;

    
}

BOOL FECIsMemRegions(UINT32 address)
{
    UINT8 i = 0;

    for (i = 0; i < FEC_NUM_MEM_REGIONS; ++i)
        if ( (address > fec_mem_reg[i].start) && (address < fec_mem_reg[i].end))
            return TRUE;            

     return FALSE;
}

#endif //#ifdef FEC_ADDITIONAL_CHECK_ENABLED



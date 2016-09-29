//-------------------------------------------------------------------------------------------
/** @file Mdmadrv.c
 *
 * @brief MDMA (Memory DMA) HAL functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.83 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "mdmadrv.h"
#include "heaps.h"
#include "appprintf.h"
#include <string.h>
#include "FECdrv.h"
#include "queue.h"
#include "systimers.h"
#include "DSPdrv.h"
#include "dbgfunctab.h"

VUINT32 MdmaSysLock;
VUINT32 MdmaSpuLock;
VUINT32 MdmaRadLock;

HANDLE  g_RadPart = {0};
PUINT32 g_RadBuf = NULL;

MDMA_CTX g_MdmaSysCtx = {0};
MDMA_CTX g_MdmaSpuCtx = {0};
MDMA_CTX g_MdmaRadCtx = {0};

MDMA_ROBUST_CTRL_CTX MdmaSysRobustCtx;
MDMA_ROBUST_CTRL_CTX MdmaSpuRobustCtx;
MDMA_ROBUST_CTRL_CTX MdmaRadRobustCtx;

VUINT32 g_MdmaSysLock[2] = {0, 0};
VUINT32 g_MdmaSpuLock = 0;
VUINT32 g_MdmaRadLock = 0;

MDMA_PROMPT MDMA_HW_NAME[] = {
    {MDMA_HW_SYS, "SYS-MDMA"},
    {MDMA_HW_SPU, "SPU-MDMA"},
    {MDMA_HW_RAD, "RAD-MDMA"},

    {MDMA_HW_MAX, "NOT FOUND"},
};

MDMA_PROMPT MDMA_OP_TYPE_NAME[] = {
    {MDMA_OP_COPY,      "COPY"},
    {MDMA_OP_HSDSCH,    "HSDSCH"},
    {MDMA_OP_CRC,       "CRC"},
    {MDMA_OP_RAND,      "RAND"},
    {MDMA_OP_3GDLSPCG,  "3GDLSPCG"},
    {MDMA_OP_3GULSPCG,  "3GULSPCG"},
    {MDMA_OP_4SCR,      "4SCR"},
    {MDMA_OP_4DSC,      "4DSC"},
    {MDMA_OP_RAND_BE,   "RAND_BE"},
    {MDMA_OP_4SCR_BE,   "4SCR_BE"},
    {NULL,              "NOT FOUND"},
};

#define MDMA_NUMBER_OP_TYPE_NAME  (sizeof(MDMA_OP_TYPE_NAME) / sizeof(MDMA_PROMPT))

BOOL MdmaCheckFDescAddress (MDMA_HW_ID id, UINT32 pFDesc);
void MdmaDma(PMDMA_CTX pMdmaCtx, UINT32 TotalTicks);

UINT32 MdmaGetPoolTotalSize(MDMA_HW_ID id)
{
    UINT32 total_num = 0;
    UINT32 cpu_num = SysGetCpuNum();

    if (id == MDMA_HW_SYS)
    {
        total_num = SYS_MDMA_CPU_0_FDESC_NUM;

        if (cpu_num > 1)
            total_num += SYS_MDMA_CPU_1_FDESC_NUM;

        if (cpu_num > 2)
            total_num += SYS_MDMA_CPU_2_FDESC_NUM;

        if (cpu_num > 3)
            total_num += SYS_MDMA_CPU_3_FDESC_NUM;

        total_num += SYS_MDMA_CEVAX_FDESC_NUM;
    }

    if (id == MDMA_HW_SPU)
    {
        total_num = SPU_MDMA_CPU_0_FDESC_NUM;

        if (cpu_num > 1)
            total_num += SPU_MDMA_CPU_1_FDESC_NUM;

        if (cpu_num > 2)
            total_num += SPU_MDMA_CPU_2_FDESC_NUM;

        if (cpu_num > 3)
            total_num += SPU_MDMA_CPU_3_FDESC_NUM;

        total_num += SPU_MDMA_CEVAX_FDESC_NUM;
    }

    if (id == MDMA_HW_RAD)
    {
        total_num = RAD_MDMA_CPU_0_FDESC_NUM;

        if (cpu_num > 1)
            total_num += RAD_MDMA_CPU_1_FDESC_NUM;

        if (cpu_num > 2)
            total_num += RAD_MDMA_CPU_2_FDESC_NUM;

        if (cpu_num > 3)
            total_num += RAD_MDMA_CPU_3_FDESC_NUM;

        total_num += RAD_MDMA_CEVAX_FDESC_NUM;
    }

    return total_num;
}

UINT32 MdmaGetPoolSize(MDMA_HW_ID id, UINT32 nCpuID)
{
    if (id == MDMA_HW_SYS)
    {
        switch (nCpuID)
        {
            case 0:
                return SYS_MDMA_CPU_0_FDESC_NUM;

            case 1:
                return SYS_MDMA_CPU_1_FDESC_NUM;

            case 2:
                return SYS_MDMA_CPU_2_FDESC_NUM;

            case 3:
                return SYS_MDMA_CPU_3_FDESC_NUM;

            case 0xFFFF:
                return SYS_MDMA_CEVAX_FDESC_NUM;

            default:
                return 0;
        }
    }

    if (id == MDMA_HW_SPU)
    {
        switch (nCpuID)
        {
            case 0:
                return SPU_MDMA_CPU_0_FDESC_NUM;

            case 1:
                return SPU_MDMA_CPU_1_FDESC_NUM;

            case 2:
                return SPU_MDMA_CPU_2_FDESC_NUM;

            case 3:
                return SPU_MDMA_CPU_3_FDESC_NUM;

            case 0xFFFF:
                return SPU_MDMA_CEVAX_FDESC_NUM;

            default:
                return 0;
        }
    }

    if (id == MDMA_HW_RAD)
    {
        switch (nCpuID)
        {
            case 0:
                return RAD_MDMA_CPU_0_FDESC_NUM;

            case 1:
                return RAD_MDMA_CPU_1_FDESC_NUM;

            case 2:
                return RAD_MDMA_CPU_2_FDESC_NUM;

            case 3:
                return RAD_MDMA_CPU_3_FDESC_NUM;

            case 0xFFFF:
                return RAD_MDMA_CEVAX_FDESC_NUM;

            default:
                return 0;
        }
    }

    return 0;
}

void MdmaGetLock (PMDMA_CTX pMdmaCtx)
{
    if (pMdmaCtx->CevaPoolPtr != NULL)
    {
        MxiLockHwSema (pMdmaCtx->HwSema);
    }
    else
    {
        MxGetLock (pMdmaCtx->pSysLock);
    }
}

void MdmaReleaseLock (PMDMA_CTX pMdmaCtx)
{
    if (pMdmaCtx->CevaPoolPtr != NULL)
    {
        MxiUnlockHwSema (pMdmaCtx->HwSema);
    }
    else
    {
        MxReleaseLock (pMdmaCtx->pSysLock);
    }
}

INLINE PMDMA_FDESC MdmaGetActiveFdesc (PMDMA_CTX pMdmaCtx)
{
    _ASSERT_PTR(pMdmaCtx);
    _ASSERT_PTR(pMdmaCtx->ListInfPtr);

    return pMdmaCtx->ListInfPtr->pFDescActive;
}

INLINE void MdmaSetActiveFdesc (PMDMA_CTX pMdmaCtx, PMDMA_FDESC pActFDesc)
{
    _ASSERT_PTR(pMdmaCtx);
    _ASSERT_PTR(pMdmaCtx->ListInfPtr);

    pMdmaCtx->ListInfPtr->pFDescActive = pActFDesc;
}

INLINE PMDMA_FDESC MdmaGetInactiveFdesc (PMDMA_CTX pMdmaCtx)
{
    _ASSERT_PTR(pMdmaCtx);
    _ASSERT_PTR(pMdmaCtx->ListInfPtr);

    return pMdmaCtx->ListInfPtr->pFDescInactive;
}

INLINE void MdmaSetInactiveFdesc (PMDMA_CTX pMdmaCtx, PMDMA_FDESC pActFDesc)
{
    _ASSERT_PTR(pMdmaCtx);
    _ASSERT_PTR(pMdmaCtx->ListInfPtr);

    pMdmaCtx->ListInfPtr->pFDescInactive = pActFDesc;
}

INLINE void MdmaSetBusy (PMDMA_CTX pMdmaCtx, UINT32 nBusy)
{
    _ASSERT_PTR(pMdmaCtx);
    _ASSERT_PTR(pMdmaCtx->ListInfPtr);

    pMdmaCtx->ListInfPtr->HWBusy = nBusy;
}

INLINE UINT32 MdmaGetBusy (PMDMA_CTX pMdmaCtx)
{
    _ASSERT_PTR(pMdmaCtx);
    _ASSERT_PTR(pMdmaCtx->ListInfPtr);

    return pMdmaCtx->ListInfPtr->HWBusy;
}

INLINE UINT32 MdmaGetInactTicks(PMDMA_CTX pCtx)
{
    UINT32 nVal;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCtx->ListInfPtr);

    nVal = pCtx->ListInfPtr->InactiveTotalTicks;

    if (nVal != 0)
        pCtx->ListInfPtr->InactiveTotalTicks = 0;

    return nVal;
}

INLINE void MdmaSetRunTime (PMDMA_CTX pCtx, UINT32 nStartTime, UINT32 nTotalTicks)
{
    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCtx->ListInfPtr);

    pCtx->ListInfPtr->StartTimeTicks   = nStartTime;
    pCtx->ListInfPtr->ActiveTotalTicks = nTotalTicks;
}

INLINE void MdmaAddInactTicks (PMDMA_CTX pCtx, UINT32 nTotalTicks)
{
    if (nTotalTicks == 0)
        return;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCtx->ListInfPtr);

    pCtx->ListInfPtr->InactiveTotalTicks += nTotalTicks;
}

MXRC MdmaProcInt(PMDMA_CTX pMdmaCtx);

INLINE PMDMA_CTX MdmaGetCtx(MDMA_HW_ID mdma_id)
{
	switch(mdma_id)
	{
	    case MDMA_HW_SYS:
			return &g_MdmaSysCtx;
		case MDMA_HW_SPU:
			return &g_MdmaSpuCtx;
		case MDMA_HW_RAD:
            return &g_MdmaRadCtx;
		default:
			return NULL;
    }
}

INLINE MDMA_ROBUST_CTRL_CTX * MdmaGetRobustCtx(MDMA_HW_ID mdma_id)
{
	switch(mdma_id)
	{
	    case MDMA_HW_SYS:
			return &MdmaSysRobustCtx;
		case MDMA_HW_SPU:
			return &MdmaSpuRobustCtx;
		case MDMA_HW_RAD:
            return &MdmaRadRobustCtx;
		default:
			return NULL;
    }
}

UINT32 MdmaGetIdByFDesc (PMDMA_FDESC pFDesc)
{
    UINT32 i;

    for (i = 0; i < MDMA_HW_MAX; i++)
    {
        if (MdmaCheckFDescAddress ((MDMA_HW_ID)i, (UINT32)pFDesc))
        {
            return i;
        }
    }

    return MDMA_HW_MAX;
}

BOOL MdmaIsInited(MDMA_HW_ID mdma_id)
{
    PMDMA_CTX pMdmaCtx = MdmaGetCtx(mdma_id);
    _ASSERT_PTR(pMdmaCtx);
    return pMdmaCtx->bInited;
}

/** @brief This function calculates the CRC for the given buffer

    @param pRemainder [in/out]
    @param polynomial [in]
    @param width      [in] - the CRC width: 32/24/16/12/8 bits
    @param message    [in] - the source message for the CRC calculation
    @param nBytes     [in] - the size of message in bytes

    @return [UINT32] the CRC */

UINT32 MdmaProcCRC(UINT32 * pRemainder, UINT32 polynomial, UINT32 width, UINT8 const message[], UINT32 nBytes)
{
	UINT32 mask = 0, byte, bit;
	UINT32 topbit = 1 << (width - 1);

	switch (width)
	{
		case 8:
			mask = 0xFF;
			break;

		case 12:
			mask = 0xFFF;
			break;

		case 16:
			mask = 0xFFFF;
			break;

		case 24:
			mask = 0xFFFFFF;
			break;

		case 32:
			mask = 0xFFFFFFFF;
			break;

		default:
			return 0;
	}

    /*
     * Perform modulo-2 division, a byte at a time.
     */
    for (byte = 0; byte < nBytes; ++byte)
    {
        /*
         * Bring the next byte into the remainder.
         */
		*pRemainder ^= (message[byte] << (width - 8));

		// mycode
		*pRemainder &= mask;

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        for (bit = 8; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */
			if (*pRemainder & topbit)
            {
				*pRemainder = (((*pRemainder & mask)<< 1) ^ polynomial) & mask;
            }
            else
            {
				*pRemainder = (((*pRemainder & mask) << 1)) & mask;
            }
        }
    }

    /*
     * The final remainder is the CRC result.
     */
    return (*pRemainder) & mask;

}

UINT32 MdmaProcCRC_b(UINT32 * pRemainder, UINT32 polynomial, UINT32 width, UINT8 const message[], UINT32 nBytes, UINT32 nBits)
{
	UINT32 mask = 0, byte, bit;
	UINT32 topbit = 1 << (width - 1);

	switch (width)
	{
		case 8:
			mask = 0xFF;
			break;

		case 12:
			mask = 0xFFF;
			break;

		case 16:
			mask = 0xFFFF;
			break;

		case 24:
			mask = 0xFFFFFF;
			break;

		case 32:
			mask = 0xFFFFFFFF;
			break;

		default:
			return 0;
	}

    /*
     * Perform modulo-2 division, a byte at a time.
     */
    for (byte = 0; byte < nBytes; ++byte)
    {
        /*
         * Bring the next byte into the remainder.
         */
		*pRemainder ^= (message[byte] << (width - 8));

		// mycode
		*pRemainder &= mask;

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        if(byte == nBytes-1)
        {
            for (bit = nBits; bit > 0; --bit)
            {
                /*
                 * Try to divide the current data bit.
                 */
			    if (*pRemainder & topbit)
                {
				    *pRemainder = (((*pRemainder & mask)<< 1) ^ polynomial) & mask;
                }
                else
                {
				    *pRemainder = (((*pRemainder & mask) << 1)) & mask;
                }
            }
        }
        else
        {
            for (bit = 8; bit > 0; --bit)
            {
                /*
                 * Try to divide the current data bit.
                 */
			    if (*pRemainder & topbit)
                {
				    *pRemainder = (((*pRemainder & mask)<< 1) ^ polynomial) & mask;
                }
                else
                {
				    *pRemainder = (((*pRemainder & mask) << 1)) & mask;
                }
            }
        }
    }

    /*
     * The final remainder is the CRC result.
     */
    return (*pRemainder) & mask;

}


/** @brief The function puts an element to the non-active list,
            if the active queue is empty, this function starts MDMA

    @param pMdmaCtx [in] - the pointer to the MDMA driver context
    @param pElm     [in] - the pointer to the new one element
    @param pLock    [in] - the pointer to the lock element
    @param Run      [in] - the pointer to the ( Run ) function void(*)(void)

    \ingroup group_lte_hal_mdma

    */

void MdmaSchedAndRun (PMDMA_CTX pMdmaCtx, PMDMA_FDESC pElm, void(*Run)(PMDMA_CTX pMdmaCtx, UINT32 nTotalTicks), UINT32 nNumUs)
{
    IRQTYPE  irq;
    PMDMA_FDESC pTmp;

    _ASSERT_PTR(pMdmaCtx);
    _ASSERT_PTR(pElm);
    _ASSERT_PTR(Run);

    irq = ARM_INT_disable();
    MdmaGetLock (pMdmaCtx);

    pTmp = MdmaGetInactiveFdesc(pMdmaCtx);

    if (pTmp == NULL)
    {
        MdmaSetInactiveFdesc(pMdmaCtx, pElm);
    }
    else
    {
        while (pTmp->Next != NULL)
            pTmp = pTmp->Next;

        pTmp->Next = pElm;
    }

    if (MdmaGetActiveFdesc(pMdmaCtx) == NULL)
    {
        Run(pMdmaCtx, US_TO_TICKS(nNumUs));
    }
    else
    {
        MdmaAddInactTicks(pMdmaCtx, US_TO_TICKS(nNumUs));
    }

    MdmaReleaseLock (pMdmaCtx);
    ARM_INT_restore (irq);
}

PMDMA_FDESC  MdmaShiftActive (PMDMA_CTX pCtx)
{
    PMDMA_FDESC pActFDesc = NULL;

    _ASSERT_PTR(pCtx);

    pActFDesc = MdmaGetActiveFdesc(pCtx);

    if (pActFDesc != NULL)
    {
        MdmaSetActiveFdesc(pCtx, (PMDMA_FDESC)pActFDesc->Next);
    }

    return pActFDesc;
}

/** @brief This function is designed to be called in IRQ mode!!!
           the function shifts active pointer to the next element
           if this element is processed by MDMA
           and returns a pointer to the current (non-shifted) element or NULL

    @param pMdmaCtx [in] - the pointer to the MDMA driver context

    @return [PMDMA_FDESC ] the pointer to the non-shifted element (that is processed by MDMA) or NULL

    \ingroup group_lte_hal_mdma

*/
PMDMA_FDESC  MdmaShiftActiveIfDone (PMDMA_CTX pMdmaCtx)
{
    PMDMA_FDESC pActFDesc = NULL;

    _ASSERT_PTR(pMdmaCtx);
    _ASSERT_PTR(pMdmaCtx->pSysLock);

    MdmaGetLock (pMdmaCtx);

    pActFDesc = MdmaGetActiveFdesc(pMdmaCtx);

    if (pActFDesc != NULL)
    {
        if (pActFDesc->HwContext.StatusComp != 0)
        {
            MdmaSetActiveFdesc(pMdmaCtx, pActFDesc->Next);
        }
        else
        {
            pActFDesc = NULL;
        }
    }

    MdmaReleaseLock (pMdmaCtx);

    return pActFDesc;
}

//-------------------------------------------------------------------------------------------
/** @brief This function Is the IRQ Handler for the SYS MDMA Unit
 *
 *
 *  @param   pIntClearReg [out]  IRQ clean register
 *  @param   nIrqID           [in]    IRQ Id
 *  @param   pParam         [in]    Pointer to the MDMA diagnostic context
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
void MdmaIrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, LPVOID pParam)
{
    PMDMA_CTX pMdmaCtx = (PMDMA_CTX)pParam;
#ifdef MDMA_AXI_BUS_ERROR_PROCESS
    UINT32 AXIIrqStatus = pMdmaCtx->MdmaCtrlRegs->DMAOptCtrl.AXIErrIrqStatusClear;


    if (AXIIrqStatus & MDMA_IRQ_ERR_AXI_READ)
    {
        uart_printf("\nMdmaIrqHandler[%s]: happened AXI bus error on a read access IrqID = %x pParam = 0x%x\n",MdmaGetHWName(pMdmaCtx),nIrqID, pParam);
        pMdmaCtx->MdmaCtrlRegs->DMAOptCtrl.AXIErrIrqStatusClear |= MDMA_IRQ_ERR_AXI_READ;
        MdmaPrintInfo(pMdmaCtx);
        _ASSERT(0);
    }else if (AXIIrqStatus & MDMA_IRQ_ERR_AXI_WRITE)
    {
        uart_printf("\nMdmaIrqHandler[%s]: happened AXI bus error on a write access IrqID = %x pParam = 0x%x\n",MdmaGetHWName(pMdmaCtx),nIrqID, pParam);
        pMdmaCtx->MdmaCtrlRegs->DMAOptCtrl.AXIErrIrqStatusClear |= MDMA_IRQ_ERR_AXI_WRITE;
        MdmaPrintInfo(pMdmaCtx);
        _ASSERT(0);
    } else
#endif // MDMA_AXI_BUS_ERROR_PROCESS
    {
        pMdmaCtx->MdmaCtrlRegs->DMAOutCtrl.IrqStatusClear = DMAIRQ_FCTRL;
        pMdmaCtx->MdmaCtrlRegs->DMAInCtrl.IrqStatusClear = DMAIRQ_FCTRL;

        MdmaProcInt(pMdmaCtx);
    }

    *pIntClearReg = nIrqID;
}

MXRC MdmaHWReset(MDMA_HW_ID id)
{
    VUINT32 delay;
    volatile MDMA_CTRL_REGS *pMdmaCtrlRegs = NULL;

    if (id == MDMA_HW_SYS)
    {
        REG32(SYS_AXI_RESET_0) |= 0x10;

        delay = 10;
        while (delay--);

        REG32(SYS_AXI_RESET_0) &= ~0x10;

        delay = 10;
        while (delay--);

        pMdmaCtrlRegs = (volatile MDMA_CTRL_REGS *)MDMA_SYS_CTRL_REGS_BASE;
    }
    else if (id == MDMA_HW_SPU)
    {
        REG32(SPU_AXI_RESET_0) |= 0x10;

        delay = 10;
        while (delay--);

        REG32(SPU_AXI_RESET_0) &= ~0x10;

        delay = 10;
        while (delay--);

        pMdmaCtrlRegs = (volatile MDMA_CTRL_REGS *)MDMA_SPU_CTRL_REGS_BASE;
    }
    else
    {
        // TO BE DONE
    }

    if (pMdmaCtrlRegs != NULL)
    {
        pMdmaCtrlRegs->DMAInCtrl.Control       = DMA_FCOM_MODE;
        pMdmaCtrlRegs->DMAInCtrl.BurstLength   = 15;
        pMdmaCtrlRegs->DMAOutCtrl.BurstLength  = 15;
        pMdmaCtrlRegs->DMAOutCtrl.DmaIrqEnable = DMAIRQ_FCTRL;
        
        #ifdef MDMA_AXI_BUS_ERROR_PROCESS
        pMdmaCtrlRegs->DMAOptCtrl.AXIErrDmaIrqEnable = MDMA_IRQ_ERR_AXI_READ | MDMA_IRQ_ERR_AXI_WRITE;
        #endif 

        //uart_printf("Reset is done for MDMA (%d)\r\n", id);
    }

    return MX_OK;
}

MXRC MdmaReset(void)
{
#if 0
    VUINT32 delay;
    REG32(SYS_AXI_RESET_0) |= 0x10;

    delay = 10;
    while (delay--);

    REG32(SYS_AXI_RESET_0) &= 0xD0;

    delay = 10;
    while (delay--);
#endif

    return MX_OK;
}

MXRC MdmaInitPool (MDMA_HW_ID id)
{
    UINT32 FDescSize, core, fnum;
    PUINT8 pPoolStorage;
    PUINT32 pFreePtr, pUsedPtr;
    UINT32 nCevaPoolEna = 0;
    HANDLE hListIntHeap = AppHeap;
    UINT32 nListInfSize = ROUND(sizeof(MDMA_LIST_INF), 4);

    PMDMA_CTX pCtx = MdmaGetCtx(id);

    _ASSERT_PTR(pCtx);

    if (MdmaGetPoolSize(id, 0xFFFF) != 0)
    {
        nCevaPoolEna = 1;
        hListIntHeap = DspHeap;

        nListInfSize += sizeof (MXPOOL);
    }

    pCtx->ListInfPtr = (MDMA_LIST_INF*)MxHeapAlloc (hListIntHeap, nListInfSize);
    pCtx->ListInfHeap = hListIntHeap;

    if(pCtx->ListInfPtr == NULL)
    {
        uart_printf ("MDMA(id=%d): error to allocate interface obj\r\n", id);
        _ASSERT_PTR(pCtx->ListInfPtr);
        return RC_MDMA_DRV_ALLOC_INF_ERROR;
    }

    memset (pCtx->ListInfPtr, 0, nListInfSize);

    if (nCevaPoolEna != 0)
    {
        pCtx->CevaPoolPtr = (MXPOOL*)(pCtx->ListInfPtr+1);
    }
    else
    {
        pCtx->CevaPoolPtr = NULL;
    }

    FDescSize = ROUND(sizeof (MDMA_FDESC), 16);

    // to allocate the storage for FDESC blocks for all possible cores
    pPoolStorage = (PUINT8)MxHeapAlloc(MDMA_FEC_HEAP, MdmaGetPoolTotalSize(id) * FDescSize);

    if (pPoolStorage == NULL)
    {
        MxHeapFree(hListIntHeap, pCtx->ListInfPtr);
        pCtx->ListInfPtr = NULL;

        _ASSERT_PTR(pPoolStorage);
        return RC_MDMA_DRV_ALLOC_BUF_ERROR;
    }

    // to allocate the storage for 'used' and 'free' indexes for all cores
    pUsedPtr = (PUINT32)MxHeapAlloc(hListIntHeap, (MdmaGetPoolTotalSize(id) + CPU_NUM + nCevaPoolEna) * sizeof (UINT32) * 2);

    if (pUsedPtr == NULL)
    {
        MxHeapFree(hListIntHeap, pCtx->ListInfPtr);
        pCtx->ListInfPtr = NULL;

        _ASSERT_PTR(pUsedPtr);
        MxHeapFree (MDMA_FEC_HEAP, pPoolStorage);
        return RC_MDMA_DRV_ALLOC_BUF_ERROR;
    }

    pFreePtr = pUsedPtr + (MdmaGetPoolTotalSize(id) + CPU_NUM + nCevaPoolEna);

    pCtx->PoolStorPtr = pPoolStorage;
    pCtx->PoolIndex   = pUsedPtr;

    // to initialize the POOL for each core
    for (core = 0; core < SysGetCpuNum(); core++)
    {
        fnum = MdmaGetPoolSize(id, core);

        if (fnum != 0)
        {
            MxiPoolInit(&pCtx->Pool[core], pPoolStorage, fnum, FDescSize, pFreePtr, pUsedPtr);

            pPoolStorage += (FDescSize * fnum);

            pUsedPtr += (fnum + 1);
            pFreePtr += (fnum + 1);
        }
    }

    // To initialize CEVA pool, if this configuration is enabled

    if (nCevaPoolEna)
    {
        fnum = MdmaGetPoolSize(id, 0xFFFF);

        if (fnum != 0)
        {
            MxiPoolInit(pCtx->CevaPoolPtr, pPoolStorage, fnum, FDescSize, pFreePtr, pUsedPtr);

            pPoolStorage += (FDescSize * fnum);

            pUsedPtr += (fnum + 1);
            pFreePtr += (fnum + 1);
        }
    }

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function checks FDesc address and used for debug mode only

    @param id       [in] - the MDMA driver ID
    @param pFDesc   [in] - the address of FDesc

    @return [BOOL] TRUE / FALSE

 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
BOOL MdmaCheckFDescAddress (MDMA_HW_ID id, UINT32 pFDesc)
{
    UINT32 FDescSize;
    PMDMA_CTX pCtx = MdmaGetCtx (id);

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pFDesc);

    FDescSize = ROUND(sizeof (MDMA_FDESC), 16);

#ifdef MDMA_POOL_USAGE_ENABLED

    // Here we need to check range for the pool
    if ((pFDesc < (UINT32)pCtx->PoolStorPtr) || (pFDesc >= (UINT32)pCtx->PoolStorPtr + MdmaGetPoolTotalSize(id) * FDescSize))
    {
        return FALSE;
    }
#else

    if ((pFDesc < (UINT32)pCtx->PoolStorPtr) || pFDesc >= ((UINT32)pCtx->PoolStorPtr + (FEC_FDESC_POOL_SIZE * FDescSize)))
    {
        return FALSE;
    }

#endif

    return TRUE;
}

/** @brief This function is designed to provide information for CEVA MDMA driver
           to open correctly storage, to use correct HW semaphore, etc

    @param nDrvID[in] - the MDMA driver ID

    @return [MXRC] an error code */

MXRC MdmaShareAccess(UINT32 nDrvID)
{
    PMDMA_CTX pMdmaCtx = MdmaGetCtx((MDMA_HW_ID)nDrvID);
    CEVA_DRV_CFG * pCfg = DspDrvGetCevaDrvCfgParams();

    _ASSERT_PTR(pMdmaCtx);
    _ASSERT_PTR(pCfg);

    if (pCfg == NULL)
        return RC_MDMA_DRV_SHARE_INFO_ERR;

    pMdmaCtx->HwSema = 0;

#ifdef MDMA_POOL_USAGE_ENABLED
    switch (nDrvID)
    {
        case MDMA_HW_SYS:
        {
            if (SYS_MDMA_CEVAX_FDESC_NUM==0)
            {
                return MX_OK;
            }
            else
            {
                pCfg->SysMdmaCfg.HwSema     = CEVA_DRV_HW_SEMA_SYS_MDMA;
                pCfg->SysMdmaCfg.PoolPtr    = pMdmaCtx->CevaPoolPtr;
                pCfg->SysMdmaCfg.OwnerCpu   = SYS_MDMA_CEVAX_OWNER;
                pCfg->SysMdmaCfg.ListIntPtr = pMdmaCtx->ListInfPtr;
                pCfg->SysMdmaCfg.UsedMask   = 0;

                REG32(CEVA_DRV_HW_SEMA_SYS_MDMA) = 0;

                pMdmaCtx->HwSema = SYS_MDMA_HW_SEMA;
            }

            break;
        }

        case MDMA_HW_SPU:
        {
            if (SPU_MDMA_CEVAX_FDESC_NUM==0)
            {
                return MX_OK;
            }
            else
            {
                pCfg->SpuMdmaCfg.HwSema     = CEVA_DRV_HW_SEMA_SPU_MDMA;
                pCfg->SpuMdmaCfg.PoolPtr    = pMdmaCtx->CevaPoolPtr;
                pCfg->SpuMdmaCfg.UsedMask   = 0;
                pCfg->SpuMdmaCfg.OwnerCpu   = SPU_MDMA_CEVAX_OWNER;
                pCfg->SpuMdmaCfg.ListIntPtr = pMdmaCtx->ListInfPtr;

                REG32(CEVA_DRV_HW_SEMA_SPU_MDMA) = 0;

                pMdmaCtx->HwSema = SPU_MDMA_HW_SEMA;
            }

            break;
        }

        case MDMA_HW_RAD:
        {
            if (RAD_MDMA_CEVAX_FDESC_NUM==0)
            {
                return MX_OK;
            }
            else
            {
                pCfg->RadMdmaCfg.HwSema     = CEVA_DRV_HW_SEMA_RAD_MDMA;
                pCfg->RadMdmaCfg.PoolPtr    = pMdmaCtx->CevaPoolPtr;
                pCfg->RadMdmaCfg.UsedMask   = 0;
                pCfg->RadMdmaCfg.OwnerCpu   = RAD_MDMA_CEVAX_OWNER;
                pCfg->RadMdmaCfg.ListIntPtr = pMdmaCtx->ListInfPtr;

                REG32(CEVA_DRV_HW_SEMA_RAD_MDMA) = 0;

                pMdmaCtx->HwSema = RAD_MDMA_HW_SEMA;
            }

            break;
        }

        default:
        {
            return RC_MDMA_DRV_DEV_ID_ERROR;
        }
    }
#endif

    return MX_OK;
}

BOOL MdmaIsHang (PMDMA_CTX pCtx)
{
    UINT32 total;

    _ASSERT_PTR(pCtx);

    if (pCtx->bInited == 0)
        return FALSE;

    if (pCtx->ListInfPtr->pFDescActive == NULL || pCtx->ListInfPtr->HWBusy == 0)
        return FALSE;

    total = pCtx->ListInfPtr->ActiveTotalTicks;

    if (total == 0)
        return FALSE;

    return (MxGetTicks () - pCtx->ListInfPtr->StartTimeTicks) >= total;
}

BOOL MdmaIsFree (PMDMA_CTX pCtx)
{
    if (pCtx->bInited == 0)
        return TRUE;

    return (pCtx->ListInfPtr->pFDescActive == NULL);
}

void MdmaComplete(PMDMA_CTX pCtx)
{
    PMDMA_FDESC pFDesc;

    UINT32 cbDone, cbDoneIsCeva, put;

    #ifdef _MX_DEBUG
    UINT32 get;
    #endif

    pCtx->EndTicks = Get24MHzTick();
    pCtx->CompFlag = 1;

    while ((pFDesc = MdmaShiftActive(pCtx)) != NULL)
    {
        // The logic of this code is to detect is it ARM proc address
        // or CEVA proc address, in case of CEVA, we need to put this
        // address to the queue and CEVA MDMA driver will call these callbacks

        cbDone = (UINT32)pFDesc->CbDone;
        cbDoneIsCeva = ((cbDone & MDMA_CEVA_CB_DONE_PROC) == MDMA_CEVA_CB_DONE_PROC);

        if (cbDoneIsCeva)
            cbDone &= ~MDMA_CEVA_CB_DONE_PROC;

        if (cbDoneIsCeva && cbDone != NULL)
        {
            pFDesc->CbDone = NULL;  // To prevent usage of this CBDONE on CEVA again

            put = pCtx->ListInfPtr->CbDonePut;

            #ifdef _MX_DEBUG
            get = pCtx->ListInfPtr->CbDoneGet;
            #endif

            pCtx->ListInfPtr->CbDonePtr[put] = pFDesc->CbContext;
            pCtx->ListInfPtr->CbDone   [put] = cbDone;

            put ++;

            if (put >= MAX_CEVA_CB_DONE_NUM)
            {
                pCtx->ListInfPtr->CbDonePut = 0;
            }
            else
            {
                pCtx->ListInfPtr->CbDonePut = put;
            }

            // to detect queue overflow
            _ASSERT(put != get);

        }
        else if (cbDone != NULL)
        {
            ((MdmaCbDoneType)cbDone)(pFDesc->CbContext);
        }

        if (pFDesc->OwnerCpuID < CPU_NUM)
        {
            MxiPoolFree(&pCtx->Pool[pFDesc->OwnerCpuID], pFDesc);
        }
        else
        {
            MxiPoolFree(pCtx->CevaPoolPtr, pFDesc);
        }
    }
}

void MdmaRunInactive(PMDMA_CTX pCtx)
{
    if (MdmaGetActiveFdesc(pCtx) == NULL)
    {
        if (MdmaGetInactiveFdesc(pCtx) != NULL)
        {
            MdmaDma(pCtx, MdmaGetInactTicks(pCtx));
        }
        else
        {
            MdmaSetBusy(pCtx, FALSE);
        }
    }
}

void MdmaRobustHandler(void * mdma_id)
{
    IRQTYPE irq;

    MDMA_ROBUST_CTRL_CTX * pRobustCtx;
    PMDMA_CTX pMdmaCtx;

    pMdmaCtx = MdmaGetCtx((MDMA_HW_ID)((UINT32)mdma_id));
    pRobustCtx = MdmaGetRobustCtx((MDMA_HW_ID)((UINT32)mdma_id));

    _ASSERT_PTR(pMdmaCtx);
    _ASSERT_PTR(pRobustCtx);

    if (pRobustCtx->Inited == 0 || pMdmaCtx->bInited == 0)
        return;

    irq = ARM_INT_disable();
    MdmaGetLock(pMdmaCtx);

    //uart_printf ("MDMA-Robust:%d (reset:%d)\r\n", (UINT32)mdma_id, reset); 

    if (MdmaIsHang(pMdmaCtx))
    {
        //uart_printf ("\r\nMDMA ROBUST CTRL : %d\r\n", (UINT32)mdma_id);
    
        MdmaComplete(pMdmaCtx);

        //uart_printf ("MDMA RESET:%d\r\n", mdma_id);

        MdmaHWReset((MDMA_HW_ID)((UINT32)mdma_id));

        //uart_printf ("MDMA RUN INACTIVE:%d\r\n", mdma_id);

        MdmaRunInactive(pMdmaCtx);

        pRobustCtx->ResetNum ++;
    }

    MdmaReleaseLock(pMdmaCtx);
    ARM_INT_restore(irq);
}


MXRC MdmaRobustCtrl (MDMA_HW_ID mdmaid, UINT32 bInit, UINT32 nPeriod)
{
    IRQTYPE r;
    MXRC rc;
    
    MDMA_ROBUST_CTRL_CTX * pCtx = MdmaGetRobustCtx(mdmaid);
    PMDMA_CTX pMdmaCtx = MdmaGetCtx(mdmaid);
    
    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pMdmaCtx);

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

    rc = MxSetTimerProc(pCtx->hTimer, MdmaRobustHandler, (void*)((UINT32)mdmaid));

    if (FAILED(rc))
        return rc;

    pCtx->CpuID   = MxGetCpuID();
    pCtx->Period  = nPeriod;
    pCtx->Inited  = 1;

    rc = MxStartFracTimer(pCtx->hTimer);

    uart_printf ("%s robust control [RC:0x%x] [Precision:%d us] [Master CPU-%d]\r\n", MdmaGetHWName(pMdmaCtx), rc, nPeriod * (1000/MX_TIMER_FRACTION), MxGetCpuID());
    
    return rc;
}

UINT32 MdmaGetResetNum(MDMA_HW_ID mdma_id, UINT32 nResetCounter)
{
    MDMA_ROBUST_CTRL_CTX * pCtx = MdmaGetRobustCtx(mdma_id);
    UINT32 num;
    _ASSERT_PTR(pCtx);

    num = pCtx->ResetNum;

    if (nResetCounter)
        pCtx->ResetNum = 0;

    return num;
}

//-------------------------------------------------------------------------------------------
/** @brief This function initalizes the SYS MDMA Unit. Allocates memory needed by driver
 *
 *
 *  @param none
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MdmaSysInit(void)
{
    MXRC rc = MX_OK;
    PMDMA_CTX pMdmaSysCtx = MdmaGetCtx(MDMA_HW_SYS);
    volatile MDMA_CTRL_REGS *pMdmaSysCtrlRegs;
    char * pName, * pShareInfo;

#ifndef MDMA_POOL_USAGE_ENABLED
    UINT32 FDescSize = 0;
#endif

    if (pMdmaSysCtx->bInited)
        return MX_OK;

    memset(pMdmaSysCtx, 0, sizeof(MDMA_CTX));

    //printf ("SYS MDMA inited on ARM-%d\r\n", MxGetCpuID ());

    pMdmaSysCtx->CpuID = MxGetCpuID ();

    pMdmaSysCtx->MdmaCtrlRegs = (volatile MDMA_CTRL_REGS *)MDMA_SYS_CTRL_REGS_BASE;
    pMdmaSysCtrlRegs = pMdmaSysCtx->MdmaCtrlRegs;

	pMdmaSysCtx->pSysLock = &g_MdmaSysLock[0];
    pMdmaSysCtx->MdmaIrqId = HAL_IRQ_MDMA;

#ifdef MDMA_POOL_USAGE_ENABLED

    if (FAILED(rc = MdmaInitPool(MDMA_HW_SYS)))
        return rc;

    pName = "POOL";

#else
    FDescSize = ROUND(sizeof (MDMA_FDESC), 16);

    pMdmaSysCtx->ListInfHeap = AppHeap;
    pMdmaSysCtx->ListInfPtr  = (PMDMA_LIST_INF)MxHeapAlloc (AppHeap, sizeof (*pMdmaSysCtx->ListInfPtr));
    _ASSERT_PTR(pMdmaSysCtx->ListInfPtr);
    memset (pMdmaSysCtx->ListInfPtr, 0, sizeof (*pMdmaSysCtx->ListInfPtr));

    pMdmaSysCtx->PoolStorPtr = MxHeapAlloc(MDMA_FEC_HEAP, FEC_FDESC_POOL_SIZE * FDescSize);

    _ASSERT_PTR(pMdmaSysCtx->PoolStorPtr);

    if (HANDLE_NULL(pMdmaSysCtx->hPart))
    {
        if (FAILED(rc = MxCreatePart(pMdmaSysCtx->PoolStorPtr, FEC_FDESC_POOL_SIZE, FDescSize, NULL, HNULL, HNULL, &pMdmaSysCtx->hPart)))
        {
            _ASSERT_RC(rc);
            return RC_FEC_DRV_INIT_ERROR;
        }

        // the owner should share this object
        // for the other ARMs

        if (FAILED(rc = MxShareObject(pMdmaSysCtx->hPart, OBJ_SHARE_ON)))
        {
            _ASSERT_RC(rc);
            return RC_FEC_DRV_INIT_ERROR;
        }

        pMdmaSysCtx->pPart = &pMdmaSysCtx->hPart;
    }

    pName = "PART";

#endif

    // to mark driver like an storages owner
    pMdmaSysCtx->StorageOwner = TRUE;
    pMdmaSysCtx->bInited = TRUE;

    MdmaReset();

    pMdmaSysCtrlRegs->DMAInCtrl.Control       = DMA_FCOM_MODE;
    pMdmaSysCtrlRegs->DMAInCtrl.BurstLength   = 15;
    pMdmaSysCtrlRegs->DMAOutCtrl.BurstLength  = 15;
    pMdmaSysCtrlRegs->DMAOutCtrl.DmaIrqEnable = DMAIRQ_FCTRL;
#ifdef MDMA_AXI_BUS_ERROR_PROCESS
    pMdmaSysCtrlRegs->DMAOptCtrl.AXIErrDmaIrqEnable = MDMA_IRQ_ERR_AXI_READ | MDMA_IRQ_ERR_AXI_WRITE;
    uart_printf("%s: AXI error processing is enabled\n", MdmaGetHWName(pMdmaSysCtx));
#endif // #ifdef MDMA_AXI_BUS_ERROR_PROCESS

    if (SYS_MDMA_CEVAX_OWNER == 0)
    {
        MxSetIntHandler(pMdmaSysCtx->MdmaIrqId, MDMA_INT_MODE, PRI_MDMA_SYS, MdmaIrqHandler, pMdmaSysCtx);
        MxEnableInt(pMdmaSysCtx->MdmaIrqId);
    }

    #ifdef MDMA_POOL_USAGE_ENABLED
    pShareInfo = (SYS_MDMA_CEVAX_FDESC_NUM)?"/CEVA-SHARED-DRV": "";
    #else
    pShareInfo = "";
    #endif
    rc = MdmaShareAccess(MDMA_HW_SYS);
    uart_printf("SYS-MDMA[%s%s%s]:[%s {rc:%X}] [Master CPU-%d]\r\n", pName, pShareInfo, SYS_MDMA_CEVAX_OWNER ? "/CEVA-OWNER" : "", (SUCCESSFUL(rc))? "OK":"FAIL", rc, MxGetCpuID ());

    return rc;
}

//-------------------------------------------------------------------------------------------
/** @brief This function initalizes the SPU MDMA Unit. Allocates memory needed by driver
 *
 *
 *  @param none
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------

MXRC MdmaSpuInit(void)
{
    PMDMA_CTX pMdmaSpuCtx = MdmaGetCtx(MDMA_HW_SPU);
    volatile MDMA_CTRL_REGS *pMdmaSpuCtrlRegs;
    MXRC rc = MX_OK;
    char * pName, *pShareInfo;

#ifndef MDMA_POOL_USAGE_ENABLED
    UINT32 FDescSize = 0;
#endif

    if (pMdmaSpuCtx->bInited)
        return MX_OK;

    memset(pMdmaSpuCtx, 0, sizeof(MDMA_CTX));

    //printf ("SPU MDMA inited on ARM-%d\r\n", MxGetCpuID ());

    pMdmaSpuCtx->CpuID = MxGetCpuID ();

    pMdmaSpuCtx->MdmaCtrlRegs = (volatile MDMA_CTRL_REGS *)MDMA_SPU_CTRL_REGS_BASE;
    pMdmaSpuCtrlRegs = pMdmaSpuCtx->MdmaCtrlRegs;

	pMdmaSpuCtx->pSysLock = &g_MdmaSpuLock;
 	pMdmaSpuCtx->MdmaIrqId = HAL_IRQ_MDMA_SPU;

#ifdef MDMA_POOL_USAGE_ENABLED
    if (FAILED(rc = MdmaInitPool(MDMA_HW_SPU)))
        return rc;

    pName = "POOL";

#else
    FDescSize = ROUND(sizeof (MDMA_FDESC), 16);

    pMdmaSpuCtx->ListInfHeap = AppHeap;
    pMdmaSpuCtx->ListInfPtr  = (PMDMA_LIST_INF)MxHeapAlloc (AppHeap, sizeof (*pMdmaSpuCtx->ListInfPtr));
    _ASSERT_PTR(pMdmaSpuCtx->ListInfPtr);
    memset (pMdmaSpuCtx->ListInfPtr, 0, sizeof (*pMdmaSpuCtx->ListInfPtr));

    pMdmaSpuCtx->PoolStorPtr = MxHeapAlloc(MDMA_FEC_HEAP, FEC_FDESC_POOL_SIZE * FDescSize);

    _ASSERT_PTR(pMdmaSpuCtx->PoolStorPtr);

    if (HANDLE_NULL(pMdmaSpuCtx->hPart))
    {
        if (FAILED(rc = MxCreatePart(pMdmaSpuCtx->PoolStorPtr, FEC_FDESC_POOL_SIZE, FDescSize, NULL, HNULL, HNULL, &pMdmaSpuCtx->hPart)))
        {
            _ASSERT_RC(rc);
            return RC_FEC_DRV_INIT_ERROR;
        }

        // the owner should share this object
        // for the other ARMs

        if (FAILED(rc = MxShareObject(pMdmaSpuCtx->hPart, OBJ_SHARE_ON)))
        {
            _ASSERT_RC(rc);
            return RC_FEC_DRV_INIT_ERROR;
        }

        pMdmaSpuCtx->pPart = &pMdmaSpuCtx->hPart;
    }

    pName = "PART";

#endif

    // to mark the driver like an storages owner
    pMdmaSpuCtx->StorageOwner = TRUE;
    pMdmaSpuCtx->bInited = TRUE;

    // set MDMA extra APB cycles to 6
    REG32(SPU_CONFIG) |= (3 << 4);

    MdmaReset();

    pMdmaSpuCtrlRegs->DMAInCtrl.Control       = DMA_FCOM_MODE;
    pMdmaSpuCtrlRegs->DMAInCtrl.BurstLength   = 15;
    pMdmaSpuCtrlRegs->DMAOutCtrl.BurstLength  = 15;
    pMdmaSpuCtrlRegs->DMAOutCtrl.DmaIrqEnable = DMAIRQ_FCTRL;
#ifdef MDMA_AXI_BUS_ERROR_PROCESS
    pMdmaSpuCtrlRegs->DMAOptCtrl.AXIErrDmaIrqEnable = MDMA_IRQ_ERR_AXI_READ | MDMA_IRQ_ERR_AXI_WRITE;
    uart_printf("%s: AXI error processing is enabled\n", MdmaGetHWName(pMdmaSpuCtx));
#endif //  #ifdef MDMA_AXI_BUS_ERROR_PROCESS

    if (SPU_MDMA_CEVAX_OWNER == 0)
    {
        MxSetIntHandler(pMdmaSpuCtx->MdmaIrqId, MDMA_INT_MODE, PRI_MDMA_SPU, MdmaIrqHandler, pMdmaSpuCtx);
        MxEnableInt(pMdmaSpuCtx->MdmaIrqId);
    }

    #ifdef MDMA_POOL_USAGE_ENABLED
    pShareInfo = (SPU_MDMA_CEVAX_FDESC_NUM)?"/CEVA-SHARED-DRV": "";
    #else
    pShareInfo = "";
    #endif
    rc = MdmaShareAccess(MDMA_HW_SPU);
    uart_printf("SPU-MDMA[%s%s%s]:[%s {rc:%X}] [Master CPU-%d]\r\n", pName, pShareInfo, SPU_MDMA_CEVAX_OWNER ? "/CEVA-OWNER":"", (SUCCESSFUL(rc))? "OK":"FAIL", rc, MxGetCpuID ());

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief This function initalizes the RAD MDMA Unit. Allocates memory needed by driver
 *
 *
 *  @param none
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------

MXRC MdmaRadInit (void)
{
    PMDMA_CTX pMdmaRadCtx = MdmaGetCtx(MDMA_HW_RAD);
    volatile MDMA_CTRL_REGS *pMdmaRadCtrlRegs;
    MXRC rc = MX_OK;
    char * pName, *pShareInfo;

#ifndef MDMA_POOL_USAGE_ENABLED
    UINT32 FDescSize = 0;
#endif

    if (pMdmaRadCtx->bInited)
        return MX_OK;

    memset(pMdmaRadCtx, 0, sizeof(MDMA_CTX));

    pMdmaRadCtx->CpuID = MxGetCpuID ();

    pMdmaRadCtx->MdmaCtrlRegs = (volatile MDMA_CTRL_REGS *)MDMA_RAD_CTRL_REGS_BASE;
    pMdmaRadCtrlRegs = pMdmaRadCtx->MdmaCtrlRegs;

	pMdmaRadCtx->pSysLock = &g_MdmaRadLock;
	pMdmaRadCtx->MdmaIrqId = HAL_IRQ_MDMA_RAD;

#ifdef MDMA_POOL_USAGE_ENABLED
    if (FAILED(rc = MdmaInitPool(MDMA_HW_RAD)))
        return rc;

    pName = "POOL";

#else
    FDescSize = ROUND(sizeof (MDMA_FDESC), 16);

    pMdmaRadCtx->ListInfHeap = AppHeap;
    pMdmaRadCtx->ListInfPtr  = (PMDMA_LIST_INF)MxHeapAlloc (AppHeap, sizeof (*pMdmaRadCtx->ListInfPtr));
    _ASSERT_PTR(pMdmaRadCtx->ListInfPtr);
    memset (pMdmaRadCtx->ListInfPtr, 0, sizeof (*pMdmaRadCtx->ListInfPtr));

    pMdmaRadCtx->PoolStorPtr = MxHeapAlloc(MDMA_FEC_HEAP, FEC_FDESC_POOL_SIZE * FDescSize);

    _ASSERT_PTR(pMdmaRadCtx->PoolStorPtr);

    if (HANDLE_NULL(pMdmaRadCtx->hPart))
    {
        if (FAILED(rc = MxCreatePart(pMdmaRadCtx->PoolStorPtr, FEC_FDESC_POOL_SIZE, FDescSize, NULL, HNULL, HNULL, &pMdmaRadCtx->hPart)))
        {
            _ASSERT_RC(rc);
            return RC_FEC_DRV_INIT_ERROR;
        }

        // the owner should share this object
        // for the other ARMs

        if (FAILED(rc = MxShareObject(pMdmaRadCtx->hPart, OBJ_SHARE_ON)))
        {
            _ASSERT_RC(rc);
            return RC_FEC_DRV_INIT_ERROR;
        }

        pMdmaRadCtx->pPart = &pMdmaRadCtx->hPart;
    }

    pName = "PART";

#endif

    // to mark driver like an storages owner
    pMdmaRadCtx->StorageOwner = TRUE;
    pMdmaRadCtx->bInited = TRUE;

    MdmaReset();

    pMdmaRadCtrlRegs->DMAInCtrl.Control       = DMA_FCOM_MODE;
    pMdmaRadCtrlRegs->DMAInCtrl.BurstLength   = 15;
    pMdmaRadCtrlRegs->DMAOutCtrl.BurstLength  = 15;
    pMdmaRadCtrlRegs->DMAOutCtrl.DmaIrqEnable = DMAIRQ_FCTRL;
#ifdef MDMA_AXI_BUS_ERROR_PROCESS
    pMdmaRadCtrlRegs->DMAOptCtrl.AXIErrDmaIrqEnable = MDMA_IRQ_ERR_AXI_READ | MDMA_IRQ_ERR_AXI_WRITE;
    uart_printf("%s: AXI error processing is enabled\n", MdmaGetHWName(pMdmaRadCtx));
#endif //#ifdef MDMA_AXI_BUS_ERROR_PROCESS

    if (RAD_MDMA_CEVAX_OWNER == 0)
    {
        MxSetIntHandler(HAL_IRQ_MDMA_RAD, MDMA_INT_MODE, PRI_MDMA_RAD, MdmaIrqHandler, pMdmaRadCtx);
        MxEnableInt(HAL_IRQ_MDMA_RAD);
    }

    #ifdef MDMA_POOL_USAGE_ENABLED
    pShareInfo = (RAD_MDMA_CEVAX_FDESC_NUM)?"/CEVA-SHARED-DRV": "";
    #else
    pShareInfo = "";
    #endif

    rc = MdmaShareAccess(MDMA_HW_RAD);
    uart_printf("RAD-MDMA[%s%s%s]:[%s {rc:%X}] [Master CPU-%d]\r\n", pName, pShareInfo, RAD_MDMA_CEVAX_OWNER?"/CEVA-OWNER":"", (SUCCESSFUL(rc))? "OK":"FAIL", rc, MxGetCpuID ());

    return MX_OK;
}

void MdmaAckIrq (UINT32 irq)
{
    switch (irq)
    {
        case HAL_IRQ_MDMA_RAD:
        {

            volatile MDMA_CTRL_REGS * pMdmaReg = (volatile MDMA_CTRL_REGS *)MDMA_RAD_CTRL_REGS_BASE;

            pMdmaReg->DMAOutCtrl.IrqStatusClear = DMAIRQ_FCTRL;
            pMdmaReg->DMAInCtrl.IrqStatusClear = DMAIRQ_FCTRL;

            break;
        }

        case HAL_IRQ_MDMA_SPU:
        {

            volatile MDMA_CTRL_REGS * pMdmaReg = (volatile MDMA_CTRL_REGS *)MDMA_SPU_CTRL_REGS_BASE;

            pMdmaReg->DMAOutCtrl.IrqStatusClear = DMAIRQ_FCTRL;
            pMdmaReg->DMAInCtrl.IrqStatusClear = DMAIRQ_FCTRL;

            break;
        }

        case HAL_IRQ_MDMA:
        {

            volatile MDMA_CTRL_REGS * pMdmaReg = (volatile MDMA_CTRL_REGS *)MDMA_SYS_CTRL_REGS_BASE;

            pMdmaReg->DMAOutCtrl.IrqStatusClear = DMAIRQ_FCTRL;
            pMdmaReg->DMAInCtrl.IrqStatusClear = DMAIRQ_FCTRL;

            break;
        }
    }
}

BOOL MdmaIsBusy(UINT32 nMdmaID)
{
    PMDMA_CTX pMdmaRadCtx = MdmaGetCtx((MDMA_HW_ID)nMdmaID);

    if (pMdmaRadCtx == NULL)
        return 0;

    if (pMdmaRadCtx->ListInfPtr == NULL)
        return 0;

    return pMdmaRadCtx->ListInfPtr->HWBusy;
}

MXRC MdmaSysRobustInit(void)
{
    MXRC rc = MX_OK;

    #ifdef MDMA_SYS_ROBUST_CTRL_ENABLED
    if (FAILED(rc = MdmaRobustCtrl(MDMA_HW_SYS, 1, MDMA_SYS_ROBUST_CTRL_DEF_TIMER_PERIOD)))
        return rc;
    #endif

    return rc;
}

MXRC MdmaSpuRobustInit(void)
{
    MXRC rc = MX_OK;

    #ifdef MDMA_SPU_ROBUST_CTRL_ENABLED
    if (FAILED(rc = MdmaRobustCtrl(MDMA_HW_SPU, 1, MDMA_SPU_ROBUST_CTRL_DEF_TIMER_PERIOD)))
        return rc;
    #endif

    return rc;
}

MXRC MdmaRadRobustInit(void)
{
    MXRC rc = MX_OK;

    #ifdef MDMA_RAD_ROBUST_CTRL_ENABLED
    if (FAILED(rc = MdmaRobustCtrl(MDMA_HW_RAD, 1, MDMA_RAD_ROBUST_CTRL_DEF_TIMER_PERIOD)))
        return rc;
    #endif

    return rc;
}

MXRC MdmaSysRobustShutdown(void)
{
    MXRC rc = MX_OK;

    #ifdef MDMA_SYS_ROBUST_CTRL_ENABLED
    if (FAILED(rc = MdmaRobustCtrl(MDMA_HW_SYS, 0, 0)))
        return rc;
    #endif

    return rc;
}

MXRC MdmaSpuRobustShutdown(void)
{
    MXRC rc = MX_OK;

    #ifdef MDMA_SPU_ROBUST_CTRL_ENABLED
    if (FAILED(rc = MdmaRobustCtrl(MDMA_HW_SPU, 0, 0)))
        return rc;
    #endif

    return rc;
}

MXRC MdmaRadRobustShutdown(void)
{
    MXRC rc = MX_OK;

    #ifdef MDMA_RAD_ROBUST_CTRL_ENABLED
    if (FAILED(rc = MdmaRobustCtrl(MDMA_HW_RAD, 0, 0)))
        return rc;
    #endif

    return rc;
}

UINT32 MdmaSysRobustGetMasterCpu(void)
{
#ifdef MDMA_SYS_ROBUST_CTRL_ENABLED
    MDMA_ROBUST_CTRL_CTX * pCtx = MdmaGetRobustCtx(MDMA_HW_SYS);
    _ASSERT_PTR(pCtx);

    if (pCtx->Inited == 0)
        return 0xFFFF;
    
    return pCtx->CpuID;
#else
    return 0xFFFF;
#endif
}

UINT32 MdmaSpuRobustGetMasterCpu(void)
{
#ifdef MDMA_SYS_ROBUST_CTRL_ENABLED
    MDMA_ROBUST_CTRL_CTX * pCtx = MdmaGetRobustCtx(MDMA_HW_SPU);
    _ASSERT_PTR(pCtx);

    if (pCtx->Inited == 0)
        return 0xFFFF;
    
    return pCtx->CpuID;
#else
    return 0xFFFF;
#endif
}

UINT32 MdmaRadRobustGetMasterCpu(void)
{
#ifdef MDMA_SYS_ROBUST_CTRL_ENABLED
    MDMA_ROBUST_CTRL_CTX * pCtx = MdmaGetRobustCtx(MDMA_HW_RAD);
    _ASSERT_PTR(pCtx);

    if (pCtx->Inited == 0)
        return 0xFFFF;
    
    return pCtx->CpuID;
#else
    return 0xFFFF;
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief This function initalizes all 3 MDMA Units. Allocates memory needed by driver
 *
 *
 *  @param none
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------

MXRC MdmaInit(void)
{
    MXRC rc = MX_OK;

    if (MxGetCpuID () == 0)
    {
        if (FAILED(rc = MdmaSysInit()))
            return rc;

        if (FAILED(rc = MdmaSpuInit()))
            return rc;

        if (FAILED(rc = MdmaRadInit()))
            return rc;
    }

    return rc;
}


//-------------------------------------------------------------------------------------------
/** @brief This function identifies the ARM Core where the SYS MDMA Unit is being called from
 *
 *
 *  @param none
 *
 *  @return unsigned int CPU ID [0-3]
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 MdmaSysGetMasterCpu (void)
{
    PMDMA_CTX pMdmaSysCtx = MdmaGetCtx(MDMA_HW_SYS);

    _ASSERT_PTR(pMdmaSysCtx);

    if (!pMdmaSysCtx->bInited)
        return 0xFFFF;

    return pMdmaSysCtx->CpuID;
}


//-------------------------------------------------------------------------------------------
/** @brief This function identifies the ARM Core where the SPU MDMA Unit is being called from
 *
 *
 *  @param none
 *
 *  @return unsigned int CPU ID [0-3]
 *
 *  \ingroup group_lte_hal_mdma

 *
**/
//-------------------------------------------------------------------------------------------
UINT32 MdmaSpuGetMasterCpu (void)
{
    PMDMA_CTX pMdmaSpuCtx = MdmaGetCtx(MDMA_HW_SPU);

    _ASSERT_PTR(pMdmaSpuCtx);

    if (!pMdmaSpuCtx->bInited)
        return 0xFFFF;

    return pMdmaSpuCtx->CpuID;
}


//-------------------------------------------------------------------------------------------
/** @brief This function identifies the ARM Core where the RAD MDMA Unit is being called from
 *
 *
 *  @param none
 *
 *  @return unsigned int CPU ID [0-3]
 *
 *  \ingroup group_lte_hal_mdma

 *
**/
//-------------------------------------------------------------------------------------------
UINT32 MdmaRadGetMasterCpu (void)
{
    PMDMA_CTX pMdmaRadCtx = MdmaGetCtx(MDMA_HW_RAD);

    _ASSERT_PTR(pMdmaRadCtx);

    if (!pMdmaRadCtx->bInited)
        return 0xFFFF;

    return pMdmaRadCtx->CpuID;
}

//-------------------------------------------------------------------------------------------
/** @brief This function shuts down the SYS MDMA Unit. Cleans up all allocated memory
 *
 *
 *  @param none
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MdmaSysShutdown(void)
{
    PMDMA_CTX pMdmaCtx = MdmaGetCtx(MDMA_HW_SYS);

    _ASSERT_PTR(pMdmaCtx);

    if (pMdmaCtx->bInited && (pMdmaCtx->CpuID == MxGetCpuID ()))
    {
#ifdef MDMA_POOL_USAGE_ENABLED
        MxHeapFree(MDMA_FEC_HEAP,           pMdmaCtx->PoolStorPtr);

        if (pMdmaCtx->PoolIndex != NULL)
            MxHeapFree(pMdmaCtx->ListInfHeap,   pMdmaCtx->PoolIndex);
#else
        MxHeapFree(MDMA_FEC_HEAP,   pMdmaCtx->PoolStorPtr);
        MxShareObject(pMdmaCtx->hPart, OBJ_SHARE_OFF);
        MxRemovePart(pMdmaCtx->hPart);
#endif
        if (pMdmaCtx->ListInfPtr != NULL)
            MxHeapFree (pMdmaCtx->ListInfHeap, pMdmaCtx->ListInfPtr);

        MxDisableInt(pMdmaCtx->MdmaIrqId);

        memset (pMdmaCtx, 0, sizeof (*pMdmaCtx));
    }

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function shuts down the SPU MDMA Unit. Cleans up all allocated memory
 *
 *
 *  @param none
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MdmaSpuShutdown(void)
{
    PMDMA_CTX pMdmaSpuCtx = MdmaGetCtx(MDMA_HW_SPU);

    //printf ("(%d)SPU MDMA shutdown is done, cpu=%d\r\n", pMdmaSpuCtx->bInited, MxGetCpuID ());

    if (pMdmaSpuCtx->bInited && (pMdmaSpuCtx->CpuID == MxGetCpuID ()))
    {
#ifdef MDMA_POOL_USAGE_ENABLED
        MxHeapFree(MDMA_FEC_HEAP,   pMdmaSpuCtx->PoolStorPtr);

        if (pMdmaSpuCtx->PoolIndex != NULL)
            MxHeapFree(pMdmaSpuCtx->ListInfHeap,   pMdmaSpuCtx->PoolIndex);
#else
        MxHeapFree(MDMA_FEC_HEAP,   pMdmaSpuCtx->PoolStorPtr);
        MxShareObject(pMdmaSpuCtx->hPart, OBJ_SHARE_OFF);
        MxRemovePart(pMdmaSpuCtx->hPart);
#endif
        if (pMdmaSpuCtx->ListInfPtr != NULL)
            MxHeapFree (pMdmaSpuCtx->ListInfHeap, pMdmaSpuCtx->ListInfPtr);

        MxDisableInt(HAL_IRQ_MDMA_SPU);

        memset (pMdmaSpuCtx, 0, sizeof (*pMdmaSpuCtx));

        uart_printf ("SPU MDMA shutdown is done, cpu=%d\r\n", MxGetCpuID ());
    }

    return MX_OK;
}



//-------------------------------------------------------------------------------------------
/** @brief This function shuts down the RAD MDMA Unit. Cleans up all allocated memory
 *
 *
 *  @param none
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MdmaRadShutdown(void)
{
    PMDMA_CTX pMdmaRadCtx = MdmaGetCtx(MDMA_HW_RAD);

    //printf ("(%d)RAD MDMA shutdown is done, cpu=%d\r\n", pMdmaRadCtx->bInited, MxGetCpuID ());

    if (pMdmaRadCtx->bInited && (pMdmaRadCtx->CpuID == MxGetCpuID ()))
    {
#ifdef MDMA_POOL_USAGE_ENABLED
        MxHeapFree(MDMA_FEC_HEAP,   pMdmaRadCtx->PoolStorPtr);

        if (pMdmaRadCtx->PoolIndex != NULL)
            MxHeapFree(pMdmaRadCtx->ListInfHeap,   pMdmaRadCtx->PoolIndex);
#else
        MxHeapFree(MDMA_FEC_HEAP,   pMdmaRadCtx->PoolStorPtr);
        MxShareObject(pMdmaRadCtx->hPart, OBJ_SHARE_OFF);
        MxRemovePart(pMdmaRadCtx->hPart);
#endif

        if (pMdmaRadCtx->ListInfPtr != NULL)
            MxHeapFree (pMdmaRadCtx->ListInfHeap, pMdmaRadCtx->ListInfPtr);

        MxDisableInt(HAL_IRQ_MDMA_RAD);

        memset (pMdmaRadCtx, 0, sizeof (*pMdmaRadCtx));
    }

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief This function shuts down all 3 MDMA Units. Cleans up all allocated memory
 *
 *
 *  @param none
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MdmaShutdown(void)
{
    MXRC rc;

    if (FAILED(rc = MdmaSysShutdown()))
        return rc;

    if (FAILED(rc = MdmaSpuShutdown()))
        return rc;

    if (FAILED(rc = MdmaRadShutdown()))
        return rc;

    return rc;
}

/** @brief This function just stops device and does not free any resources
           like allocated memory. It's needed to save allocated resources for
           the futher debugging and do not touch potentially corrupted heaps

    @return [MXRC] an error code

    \ingroup group_lte_hal_mdma
 */

MXRC MdmaGenShedding(void)
{
    MXRC rc;

    if (FAILED(rc = MdmaShedding(MDMA_HW_SYS)))
        return rc;

    if (FAILED(rc = MdmaShedding(MDMA_HW_SPU)))
        return rc;

    if (FAILED(rc = MdmaShedding(MDMA_HW_RAD)))
        return rc;

    return rc;
}

//------------------------------------------------------------------------------
/** @brief This function just stops device and does not free any resources
 *          like allocated memory. It's needed to save allocated resources for
 *          the futher debugging and do not touch potentially corrupted heaps
 *
 *
 *  @param none
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//------------------------------------------------------------------------------

MXRC MdmaShedding(MDMA_HW_ID mdma_id)
{
    PMDMA_CTX pMdmaCtx = MdmaGetCtx(mdma_id);

	if(!pMdmaCtx)
		return RC_FEC_DRV_ALLOC_BUF_ERROR;

    if (pMdmaCtx->bInited && (pMdmaCtx->CpuID == MxGetCpuID()))
    {
        UINT32 t = MxGetTicks ();
        PMDMA_FDESC pFdesc;

        if ((pFdesc = MdmaGetActiveFdesc(pMdmaCtx)) != NULL)
        {
            while (pFdesc->Next != NULL)
            {
                pFdesc = pFdesc->Next;
            }

            while (pFdesc->HwContext.StatusComp == 0)
            {
                if (MxGetTicks () - t > 150000)
                    break;
            }
        }

        if (pMdmaCtx->MdmaCtrlRegs != NULL)
        {
            pMdmaCtx->MdmaCtrlRegs->DMAOutCtrl.IrqStatusClear = DMAIRQ_FCTRL;
            pMdmaCtx->MdmaCtrlRegs->DMAInCtrl.IrqStatusClear = DMAIRQ_FCTRL;
        }

        MxDisableInt(pMdmaCtx->MdmaIrqId);
        pMdmaCtx->bInited = FALSE;
    }

    return MX_OK;
    }


//-------------------------------------------------------------------------------------------
/** @brief This function allocates descriptors and initializes them as needed for the MDMA Unit
 *
 *
 *  @param NumDesc Number of descriptors to allocate
 *
 *  @return PMDMA_FDESC Pointer to the allocated Descriptors
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
PMDMA_FDESC MdmaAllocDesc(MDMA_HW_ID mdma_id, UINT32 NumDesc)
{
    PMDMA_CTX pMdmaCtx = MdmaGetCtx(mdma_id);
    PMDMA_FDESC pFDesc, pFDescPrev = NULL, pFDescHead = NULL;
    UINT32 i, cpu_id = MxGetCpuID ();

    MXPOOL * pPool;
    pPool = &pMdmaCtx->Pool[cpu_id];

    for (i = 0; i < NumDesc; i++)
    {
        if (pMdmaCtx->pPart == NULL)
        {
            pFDesc = (PMDMA_FDESC)MxiPoolAlloc (pPool);
        }
        else
        {
            pFDesc = MxAllocBlock(*(pMdmaCtx->pPart));
        }

        if (pFDesc == NULL)
        {
            uart_printf ("MDMA(id=%d): NO BLOCK, cpu=%d, mode=0x%x\r\n", mdma_id, MxGetCpuID(), MxGetCpuMode ());

            while (pFDescHead != NULL)
            {
                pFDescPrev = pFDescHead;
                pFDescHead = pFDescPrev->Next;

                if (pMdmaCtx->pPart == NULL)
                {
                    MxiPoolFree (pPool, pFDesc);
                }
                else
                {
                    MxFreeBlock(*(pMdmaCtx->pPart), pFDescPrev);
                }
            }

            return NULL;
        }

        pFDesc->OwnerCpuID          = cpu_id;
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

    return pFDescHead;
}

//-------------------------------------------------------------------------------------------
/** @brief This function just returns a pointer to the temporary FDesc block,
 *         this block can be used with blocking calls
 *
 *
 *  @param none
 *
 *  @return PMDMA_FDESC Pointer to the allocated Descriptors
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
PMDMA_FDESC MdmaGetSingleDesc (MDMA_HW_ID mdma_id)
{
    return MdmaAllocDesc(mdma_id, 1);
}

//-------------------------------------------------------------------------------------------
/** @brief This function returns sets the Hardware context to the SYS MDMA Descriptor
 *
 *
 *  @param pFDesc        Pointer to the descriptor
 *  @param HwContext     Context to set
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MdmaSetHwCtx(PMDMA_FDESC pFDesc, MDMAHWCTX HwContext)
{
    _ASSERT_PTR(pFDesc);

    HwContext.IRQEN = 0;
    HwContext.BOffset = 1;
    HwContext.StatusComp = 0;

    pFDesc->HwContext = HwContext;

    return MX_OK;
}

// Used for PHY Code Descrambler when Descrambler Input / Output size is > 64K
//-------------------------------------------------------------------------------------------
/** @brief This function is used to set I/O pointers to the SYS MDMA Descriptor if the Input / Output size is > 64K bytes.
 *               Since the MDMA unit can handle only 64K bytes of a data at a time, this function would include multiple I/O
 *               pointers to the same descriptor so that the MDMA will just continue to finish the job to completion. It can also
 *               perform the operation if the size of the data is <64K.
 *
 *  @param pFDesc     Pointer to the MDMA descriptor
 *  @param pInData    Pointer to the Input array
 *  @param pOutData  Pointer to the Output array
 *  @param DataSize   Size of the Input / Output array
 *
 *  @return unsigned int. This is the number of I/O pointers that the function had to assign to the descriptor. It is based on
 *                                   on the DataSize
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 MdmaSetDataMultiple(PMDMA_FDESC pFDesc, LPVOID pInData, LPVOID pOutData, UINT32 DataSize)
{

    UINT32 numDesc, i, DataSizeLocal;
    PUINT8 pInput, pOutput;

    _ASSERT_PTR(pFDesc);

    DataSizeLocal = DataSize;
    pInput = (PUINT8)pInData;
    pOutput = (PUINT8)pOutData;

    if(DataSizeLocal >
        MDMA_TRSIZE  * (sizeof(pFDesc->IoBdesc) / sizeof(pFDesc->IoBdesc[0])) / 2)
    {
        uart_printf("MdmaSetDataMultiple: out of bounds (%d > %d) !!!",
            DataSizeLocal, MDMA_TRSIZE * (sizeof(pFDesc->IoBdesc) / sizeof(pFDesc->IoBdesc[0])) / 2);
        while(1);
    }

    i = 0;
    while(DataSizeLocal > MDMA_TRSIZE)
    {
        _ASSERT(i<MDMA_BDESC_NUM);

        pFDesc->IoBdesc[i].BPtr = (UINT32)pOutput;
        pFDesc->IoBdesc[i].BCtrl = 0x80000000 | MDMA_TRSIZE;
        i++;

        pOutput += MDMA_TRSIZE;
        DataSizeLocal -= MDMA_TRSIZE;
    }

    pFDesc->IoBdesc[i].BPtr = (UINT32)pOutput;
    pFDesc->IoBdesc[i].BCtrl = BCTRL_BLAST_MASK | DataSizeLocal;

    i++;
    numDesc = i;

    DataSizeLocal = DataSize;
    while(DataSizeLocal > MDMA_TRSIZE)
    {
        _ASSERT(i<MDMA_BDESC_NUM);

        pFDesc->IoBdesc[i].BPtr = (UINT32)pInput;
        pFDesc->IoBdesc[i].BCtrl = MDMA_TRSIZE;
        i++;

        pInput += MDMA_TRSIZE;
        DataSizeLocal -= MDMA_TRSIZE;
    }

    _ASSERT(i<MDMA_BDESC_NUM);

    pFDesc->IoBdesc[i].BPtr = (UINT32)pInput;
    pFDesc->IoBdesc[i].BCtrl = BCTRL_BLAST_MASK | DataSizeLocal;

    if (SysCoreIsDCached((UINT32)pInData))
    {
        MxCacheClean(pInData, DataSize);
    }

    if (SysCoreIsDCached((UINT32)pOutData))
    {
        // if buffer is not cache line aligned,
        // the non-aligned part should be cleaned before invalidation
        // to prevent data loss
        if (((UINT32)pOutData & 31) != 0 )
        {
            MxCacheClean(pOutData, 32);
        }
        MxCacheInvalidate(pOutData, DataSize);
    }

    return (numDesc);

}

//-------------------------------------------------------------------------------------------
/** @brief This function setups a callback function to the last SYS descriptor in the chain of descriptors.
 *
 *  @param pFDesc       Pointer to the descriptor
 *  @param CbDone      Pointer to the callback function
 *  @param CbContext  Callback function's context
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MdmaSetLastDesc(PMDMA_FDESC pFDesc, void (*CbDone)(LPVOID), LPVOID CbContext)
{
    PMDMA_FDESC p = pFDesc;

#ifdef _MX_DEBUG
    UINT32 num = 0;
    UINT32 mdma_id, max;
    UINT32 lr = __return_address () - 4;
#endif
    _ASSERT_PTR(pFDesc);

#ifdef _MX_DEBUG
    mdma_id = MdmaGetIdByFDesc (pFDesc);
    _ASSERT(mdma_id < MDMA_HW_MAX);

    #ifdef MDMA_POOL_USAGE_ENABLED
        max = MdmaGetPoolTotalSize((MDMA_HW_ID)mdma_id);
    #else
        max = FEC_FDESC_POOL_SIZE;
    #endif

#endif

    while (p->Next)
    {
        p->CbDone = NULL;

#ifdef _MX_DEBUG

        if (!MdmaCheckFDescAddress((MDMA_HW_ID)mdma_id, (UINT32)p))
        {
            uart_printf ("MDMA(%d)-SetLast: Detected wrong chain (NextPtr) for fdesc:0x%x, num=%d, cpu-%d, mode=0x%x, lr=0x%x (cbDone=%x(%x))\r\n",
                        mdma_id,
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

        num ++;

        if (num >= max)
        {
            uart_printf ("MDMA(%d)-SetLast: Detected wrong chain for fdesc:0x%x, num=%d, cpu-%d, mode=0x%x, lr=0x%x (cbDone=%x(%x))\r\n",
                        mdma_id,
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
        p = p->Next;
    }

    p->HwContext.IRQEN = 1; // enable IRQ
    p->CbDone = CbDone;
    p->CbContext = CbContext;

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_TO_DO DOX_INTERNAL
 *
 *
 *  @param
 *
 *  @return
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
void MdmaDma(PMDMA_CTX pMdmaCtx, UINT32 TotalTicks)
{
    PMDMA_FDESC pFDesc;
    volatile MDMA_CTRL_REGS *pMdmaCtrlRegs = pMdmaCtx->MdmaCtrlRegs;

    MdmaSetBusy(pMdmaCtx, TRUE);

    pFDesc = MdmaGetInactiveFdesc(pMdmaCtx);

    MdmaSetActiveFdesc(pMdmaCtx, pFDesc);
    MdmaSetInactiveFdesc(pMdmaCtx, NULL);

    pMdmaCtx->StartTicks = Get24MHzTick();
    pMdmaCtx->CompFlag = 0;

    MdmaSetRunTime (pMdmaCtx, pMdmaCtx->StartTicks, TotalTicks);

    pMdmaCtrlRegs->DMAOutCtrl.HeadFDesc = (UINT32)pFDesc;
    pMdmaCtrlRegs->DMAInCtrl.HeadFDesc = (UINT32)pFDesc;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_TO_DO DOX_INTERNAL
 *
 *
 *  @param
 *
 *  @return
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MdmaRun(MDMA_HW_ID mdma_id, PMDMA_FDESC pFDesc, UINT32 nNumUs)
{
    PMDMA_CTX pMdmaCtx = MdmaGetCtx(mdma_id);

    _ASSERT_PTR(pFDesc);

	if(!pMdmaCtx)
		return RC_FEC_DRV_ALLOC_BUF_ERROR;

    MdmaSchedAndRun(pMdmaCtx, pFDesc, MdmaDma, nNumUs);

    return MX_OK;
}

MXRC MdmaRunEx(MDMA_HW_ID mdma_id, PMDMA_FDESC pFDesc, void (*CbDone)(LPVOID), LPVOID CbContext)
{
    return MdmaRunExQ(mdma_id, pFDesc, CbDone, CbContext, 0);
}

//-------------------------------------------------------------------------------------------
/** @brief This function sends the SYS MDMA descriptor(s) for execution
 *
 *
 *  @param pFDesc       Pointer to the descriptor
 *  @param CbDone      Pointer to the callback function. If NULL, then runs in a blocking mode to completion before returing back
 *  @param CbContext  Callback function's context
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MdmaRunExQ(MDMA_HW_ID mdma_id, PMDMA_FDESC pFDesc, void (*CbDone)(LPVOID), LPVOID CbContext, UINT32 nNumUs)
{
    PMDMA_CTX pMdmaCtx = MdmaGetCtx(mdma_id);

#ifdef MDMA_BURST_CONTROL_ENABLED
    MDMA_HW *pMdmaHw;
#endif

#ifdef _MX_DEBUG
    UINT32 lr = __return_address () - 4;
#endif

    MXRC rc = MX_OK;

    _ASSERT_PTR(pMdmaCtx);
    _ASSERT_PTR(pFDesc);

#ifdef MDMA_BURST_CONTROL_ENABLED
    // This is to fix bug with Descrambler
    pMdmaHw = MDMA_GET_HW(pFDesc);
    if ((pMdmaHw->FControl & MDMA_OP_4DSC) == MDMA_OP_4DSC)
        pMdmaCtx->MdmaCtrlRegs->DMAInCtrl.BurstLength = 15;
    else
        pMdmaCtx->MdmaCtrlRegs->DMAInCtrl.BurstLength = 15;
#endif //MDMA_BURST_CONTROL_ENABLED


#ifdef _MX_DEBUG
    // in this part we need to check correctness of MDMA descriptors
    // provided in parameter

    if (!MdmaCheckFDescAddress(mdma_id, (UINT32)pFDesc))
    {
        uart_printf ("MDMA-RunEx(id=%d) Detected wrong FDesc(0x%x), cpu-%d, mode=0x%x, lr=0x%x\r\n",
                        mdma_id,
                        pFDesc,
                        MxGetCpuID(),
                        MxGetCpuMode(),
                        lr);
        _ASSERT(0);
    }

#endif


    if (CbDone != NULL)
    {
        MdmaSetLastDesc (pFDesc, CbDone, CbContext);

        #if 0
        if (nNumUs && mdma_id == MDMA_HW_SYS)
        {
            PMDMA_FDESC pTmp = pFDesc;
        
            while (pTmp != NULL)
            {
                pTmp->HwContext.IRQEN = 0;
                pTmp = pTmp->Next;
            }
        }
        #endif
        
        MdmaRun (mdma_id, pFDesc, nNumUs);
    }
    else
    {
        PMDMA_FDESC pFLast = pFDesc;
        IRQTYPE r;

        while (pFLast->Next != NULL)
        {
            pFLast = pFLast->Next;
        }

        pFLast->HwContext.StatusComp = 0;
        pFLast->HwContext.IRQEN      = 0;

		while (TRUE)
		{
			r = ARM_INT_disable ();
			MdmaGetLock(pMdmaCtx);

			if (MdmaGetBusy(pMdmaCtx) == 0)
				break;

			MdmaReleaseLock(pMdmaCtx);
			ARM_INT_restore (r);
		}

		MdmaSetBusy(pMdmaCtx, TRUE);
		MdmaSetActiveFdesc(pMdmaCtx, pFDesc);

        pMdmaCtx->MdmaCtrlRegs->DMAOutCtrl.HeadFDesc = (UINT32)pFDesc;
        pMdmaCtx->MdmaCtrlRegs->DMAInCtrl.HeadFDesc  = (UINT32)pFDesc;

		MdmaReleaseLock(pMdmaCtx);
        ARM_INT_restore (r);

		// in this loop we need to wait
		// when non-bloking tasks will be completed
		// to prevent dead-lock we need to lock/unlock
		// the MDMA sema

        while (pFLast->HwContext.StatusComp == 0)
        {
            _NOP();
        }

		r = ARM_INT_disable ();
        MdmaGetLock(pMdmaCtx);

        MdmaSetActiveFdesc(pMdmaCtx, NULL);

        if (MdmaGetInactiveFdesc(pMdmaCtx) != NULL)
        {
            MdmaDma(pMdmaCtx, nNumUs);
        }
        else
        {
          	MdmaSetBusy(pMdmaCtx, FALSE);
        }

        MdmaReleaseLock(pMdmaCtx);
		ARM_INT_restore (r);
        MdmaFreeDesc (mdma_id, pFDesc);
    }

    return rc;
}

//-------------------------------------------------------------------------------------------
/** @brief This function is used to run MDMA Non-Blocking mode
 *
 *
 *  @param p        Parameters (ignored)
 *
 *  @return         NONE
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
void MdmaEmptyCallback(LPVOID p)
{
    return;
}


//-------------------------------------------------------------------------------------------
/** @brief This function is designed to be used to run the list of MDMA descriptors in non-interrupt mode
 *         Please use MdmaSysEnd to wait completeness of the scheduled list.
 *         Between  [MdmaSysStart] and [MdmaSysEnd] ARM may executes some code
 *
 *
 *  @param pFDesc       Pointer to the descriptor
  *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MdmaStart(MDMA_HW_ID mdma_id, PMDMA_FDESC pFDesc)
{
    PMDMA_CTX pMdmaCtx = MdmaGetCtx(mdma_id);
    //MDMA_HW *pMdmaHw;
    PMDMA_FDESC pFLast = pFDesc;
    IRQTYPE irq;

    _ASSERT_PTR(pFDesc);

	if(!pMdmaCtx)
		return RC_FEC_DRV_ALLOC_BUF_ERROR;

    irq = ARM_INT_disable();
    MdmaGetLock(pMdmaCtx);
    ARM_INT_restore(irq);

    while (pFLast->Next != NULL)
    {
        pFLast->HwContext.IRQEN = 0;
        pFLast->HwContext.StatusComp = 0;

        pFLast = pFLast->Next;
    }

    pFLast->HwContext.IRQEN = 0;
    pFLast->HwContext.StatusComp = 0;

    while (MdmaGetBusy(pMdmaCtx) != 0);

    pMdmaCtx->MdmaCtrlRegs->DMAOutCtrl.HeadFDesc = (UINT32)pFDesc;
    pMdmaCtx->MdmaCtrlRegs->DMAInCtrl.HeadFDesc  = (UINT32)pFDesc;

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function is designed to wait of the completeness in non-interrupt mode
 *         This function is designed to be used with MdmaSysStart function.
 *         Between  [MdmaSysStart] and [MdmaSysEnd] ARM may executes some code
 *
 *  @param pFDesc       Pointer to the descriptor
  *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MdmaEnd(MDMA_HW_ID mdma_id, PMDMA_FDESC pFDesc)
{
    PMDMA_CTX pMdmaCtx = MdmaGetCtx(mdma_id);
    PMDMA_FDESC pFLast = pFDesc;

    _ASSERT_PTR(pFDesc);

	if(!pMdmaCtx)
		return RC_FEC_DRV_ALLOC_BUF_ERROR;

    while (pFLast->Next != NULL)
    {
        pFLast = pFLast->Next;
    }

    while (pFLast->HwContext.StatusComp == 0);

    MdmaFreeDesc (mdma_id, pFDesc);
    MdmaReleaseLock(pMdmaCtx);

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function frees the allocated SYS descriptors once completed execution. It is automatically called once the MDMA
 *              Unit returns back from completion of the task
 *
 *  @param pFDesc       Pointer to the descriptor
 *
 *  @return  none
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
void MdmaFreeDesc (MDMA_HW_ID mdma_id, PMDMA_FDESC pFDesc)
{
    PMDMA_FDESC tmp;
    UINT32 n = 0;
	PMDMA_CTX pMdmaCtx = MdmaGetCtx(mdma_id);
    IRQTYPE irq = 0;

	if(!pMdmaCtx)
		return;

    if (pMdmaCtx->pPart == NULL)
    {
        irq = ARM_INT_disable();
    }

    while (pFDesc != NULL)
    {
        tmp = pFDesc->Next;

        if (pMdmaCtx->pPart == NULL)
        {
            MxiPoolFree (&pMdmaCtx->Pool[pFDesc->OwnerCpuID], pFDesc);
        }
        else
        {
            MxFreeBlock(*pMdmaCtx->pPart, pFDesc);
        }

        pFDesc = tmp;

        n++;
    }

    if (pMdmaCtx->pPart == NULL)
    {
        ARM_INT_restore(irq);
    }

    //uart_printf ("MDMA FREE, n=%d / %d\r\n", n, MxGetFreePartBlocks (g_FecDlPart));
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_TO_DO DOX_INTERNAL
 *
 *
 *  @param
 *
 *  @return
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 MdmaGetExecTicks(MDMA_HW_ID mdma_id)
{
    PMDMA_CTX pMdmaCtx = MdmaGetCtx(mdma_id);

	if(!pMdmaCtx)
		return RC_FEC_DRV_ALLOC_BUF_ERROR;

    return pMdmaCtx->EndTicks - pMdmaCtx->StartTicks;
}

//-------------------------------------------------------------------------------------------
/** @brief This function processes the IRQ when the SPU MDMA returns from a task. It also dispacthes any other existing calls
 *              that have been queued up in the chain
 *
 *  @param none
  *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MdmaProcInt(PMDMA_CTX pMdmaCtx)
{
	MXRC rc = MX_OK;
    PMDMA_FDESC pFDesc;
    UINT32 cbDone, cbDoneIsCeva, put;

    #ifdef _MX_DEBUG
    UINT32 get;
    #endif

    pMdmaCtx->EndTicks = Get24MHzTick();
    pMdmaCtx->CompFlag = 1;

    while ((pFDesc = MdmaShiftActiveIfDone(pMdmaCtx)) != NULL)
    {
        // The logic of this code is to detect is it ARM proc address
        // or CEVA proc address, in case of CEVA, we need to put this
        // address to the queue and CEVA MDMA driver will call these callbacks

        cbDone = (UINT32)pFDesc->CbDone;
        cbDoneIsCeva = ((cbDone & MDMA_CEVA_CB_DONE_PROC) == MDMA_CEVA_CB_DONE_PROC);

        if (cbDoneIsCeva)
            cbDone &= ~MDMA_CEVA_CB_DONE_PROC;

        if (cbDoneIsCeva && cbDone != NULL)
        {
            pFDesc->CbDone = NULL;  // To prevent usage of this CBDONE on CEVA again

            put = pMdmaCtx->ListInfPtr->CbDonePut;

            #ifdef _MX_DEBUG
            get = pMdmaCtx->ListInfPtr->CbDoneGet;
            #endif

            pMdmaCtx->ListInfPtr->CbDonePtr[put] = pFDesc->CbContext;
            pMdmaCtx->ListInfPtr->CbDone   [put] = cbDone;

            put ++;

            if (put >= MAX_CEVA_CB_DONE_NUM)
            {
                pMdmaCtx->ListInfPtr->CbDonePut = 0;
            }
            else
            {
                pMdmaCtx->ListInfPtr->CbDonePut = put;
            }

            // to detect queue overflow
            _ASSERT(put != get);

        }
        else if (cbDone != NULL)
        {
            ((MdmaCbDoneType)cbDone)(pFDesc->CbContext);
        }

        if (pMdmaCtx->pPart == NULL)
        {
            if (pFDesc->OwnerCpuID < CPU_NUM)
                MxiPoolFree (&pMdmaCtx->Pool[pFDesc->OwnerCpuID], pFDesc);
            else
                MxiPoolFree (pMdmaCtx->CevaPoolPtr, pFDesc);
        }
        else
        {
            MxFreeBlock(*(pMdmaCtx->pPart), pFDesc);
        }
    }

    MdmaGetLock(pMdmaCtx);

    // The lock was already gotten to process inactive lists
    // but in this time (before lock) CEVA might detected that Active list is NULL
    // and started new list from <inactive one>

    if (MdmaGetActiveFdesc(pMdmaCtx) == NULL)
    {
        if (MdmaGetInactiveFdesc(pMdmaCtx) != NULL)
        {
            MdmaDma(pMdmaCtx, MdmaGetInactTicks(pMdmaCtx));
        }
        else
        {
            MdmaSetBusy(pMdmaCtx, FALSE);
        }    
    }

    MdmaReleaseLock(pMdmaCtx);

    return rc;
}


////////////////////////////////////////////////////////////////////////////////
/// MDMA Log
////////////////////////////////////////////////////////////////////////////////

#if 0

static MXRC MdmaLogWrite(PMDMA_CTX pMdmaCtx, PMDMA_FDESC pFDesc, UINT32 InOut)
{
	MXRC rc = MX_OK;
	UINT32 ts;
	UINT32 i;
	PMDMA_CTX pRadMdmaCtx = MdmaGetCtx(MDMA_HW_RAD);

	if (pMdmaCtx == NULL)
        {
		uart_printf ("No Mdma device.\n");
		return RC_MDMA_DRV_LOGGER_ERROR;
        }
	if (pFDesc == NULL)
	{
		uart_printf ("Frame Descriptor is empty.\n");
		return RC_MDMA_DRV_LOGGER_ERROR;
    }

	if (pMdmaCtx->bInited == FALSE)
    {
		uart_printf ("Mdma device is not initialized.\n");
		return RC_MDMA_DRV_LOGGER_ERROR;
    }
	if (pRadMdmaCtx->bInited == FALSE)
    {
		uart_printf ("Radio Mdma device is not initialized.\n");
		return RC_MDMA_DRV_LOGGER_ERROR;
    }

	// while there is next Frame Descriptor
	while (pFDesc != 0)
	{
		// if opcode matches
		if (pFDesc->HwContext.CalcType == pMdmaCtx->opcode)
		{
			// if Frame Descriptor should be logged
			if (pMdmaCtx->logctrl & LOGFDESCMODE)
			{
				rc = LogMgrWriteData(pMdmaCtx->hLog, pFDesc, sizeof(MDMA_FDESC));
				if (rc != MX_OK)
					return RC_MDMA_DRV_LOGGER_ERROR;
				ts = SysTimerGetTick();
				while (pRadMdmaCtx->bHwBusy)
				{
					if ((SysTimerGetTick() - ts) > 500000000)
						return RC_MDMA_DRV_RUN_TIMEOUT;
}
			}
			// if data should be logged
			if (pMdmaCtx->logctrl & LOGDATAMODE)
{
				i = 0;
				// discovers output descriptors
				while (i < (MDMA_BDESC_NUM / 2))
    {
					// if output data should be logged
					if (InOut == MDMA_OUTBUF)
        {
						rc = LogMgrWriteData(pMdmaCtx->hLog, (LPVOID)(pFDesc->IoBdesc[i].BPtr), pFDesc->IoBdesc[i].BCtrl & MDMA_BLEN);
						if (rc != MX_OK)
							return RC_MDMA_DRV_LOGGER_ERROR;
						// wait while RadMDMA is being busy
						ts = SysTimerGetTick();
						while (pRadMdmaCtx->bHwBusy)
						{
							if ((SysTimerGetTick() - ts) > 500000000)
								return RC_MDMA_DRV_RUN_TIMEOUT;
        }
    }
					if (pFDesc->IoBdesc[i].BCtrl & MDMA_BLAST)
    {
						i += 1; // jumps to input descriptors
						break;
    }
					i += 1;
				}
				// discovers input descriptors
				while (i < MDMA_BDESC_NUM)
    {
					// if input data should be logged
					if (InOut == MDMA_INBUF)
					{
						rc = LogMgrWriteData(pMdmaCtx->hLog, (LPVOID)(pFDesc->IoBdesc[i].BPtr), pFDesc->IoBdesc[i].BCtrl & MDMA_BLEN);
						if (rc != MX_OK)
							return RC_MDMA_DRV_LOGGER_ERROR;
						// wait while RadMDMA is being busy
						ts = SysTimerGetTick();
						while (pRadMdmaCtx->bHwBusy)
						{
							if ((SysTimerGetTick() - ts) > 500000000)
								return RC_MDMA_DRV_RUN_TIMEOUT;
						}
					}
					if (pFDesc->IoBdesc[i].BCtrl & MDMA_BLAST)
						break;
					i += 1;
				}
			}
		}
		// next Frame Descriptor
		pFDesc = pFDesc->Next;
	}

	return rc;
    }


#endif

MXRC MdmaSysSetLog(UINT32 opcode, UINT8 logctrl, LPVOID pDst, UINT32 datasize)
{
	MXRC rc = MX_OK;

	rc = LogMgrCreateLogEx(LOG_CTRL_RAD_MDMA, pDst, datasize, &g_MdmaSysCtx.hLog);

	if (rc != MX_OK)
		return rc;

	// initialization of radio MDMA driver
	rc = MdmaRadInit();
	if (rc != MX_OK)
	{
	    uart_printf ("Cannot initialize radio MDMA driver.\n");
	    return rc;
	}

	g_MdmaSysCtx.opcode = opcode;
	g_MdmaSysCtx.logctrl = logctrl;
	g_MdmaSysCtx.bLogEnable = TRUE;

	return rc;
}

MXRC MdmaSpuSetLog(UINT32 opcode, UINT8 logctrl, LPVOID pDst, UINT32 datasize)
{
	MXRC rc = MX_OK;

	rc = LogMgrCreateLogEx(LOG_CTRL_RAD_MDMA, pDst, datasize, &g_MdmaSpuCtx.hLog);

	if (rc != MX_OK)
		return rc;

	// initialization of radio MDMA driver
	rc = MdmaRadInit();
	if (rc != MX_OK)
	{
	    uart_printf ("Cannot initialize radio MDMA driver.\n");
	    return rc;
	}

	g_MdmaSpuCtx.opcode = opcode;
	g_MdmaSpuCtx.logctrl = logctrl;
	g_MdmaSpuCtx.bLogEnable = TRUE;

	return rc;
}

MXRC MdmaCopy (MDMA_HW_ID mdma_id, LPVOID pDst, LPVOID pSrc, UINT32 nSize)
{
    PMDMA_FDESC pDesc = MdmaAllocDesc (mdma_id, 1);

    if (pDesc == NULL)
        return RC_MDMA_DRV_ALLOC_BUF_ERROR;

    MdmaSetCopy (pDesc);
    MdmaSetData(pDesc, pSrc, nSize, pDst, nSize);
    MdmaSetLastDesc (pDesc, NULL, NULL);

    return MdmaRun(mdma_id, pDesc, 0);
}

MXRC MdmaRadArraysCopy (LPVOID pDst, LPVOID* ppSrc, UINT32* pSize, UINT32 numberOfItems)
{
    PMDMA_FDESC pDesc = MdmaAllocDesc (MDMA_HW_RAD, numberOfItems);
    PMDMA_FDESC pFirstDesc = pDesc;
    UINT32 i = 0;

    for(i = 0; i < numberOfItems; i++)
    {
        if (pDesc == NULL)
            return RC_MDMA_DRV_ALLOC_BUF_ERROR;

        MdmaSetCopy (pDesc);
        pDst = (i == 0? pDst : (LPVOID)((char*)pDst + pSize[i - 1]));
        MdmaSetData(pDesc, ppSrc[i], pSize[i], pDst, pSize[i]);
        pDesc = pDesc->Next;
    }

    return MdmaRun(MDMA_HW_RAD, pFirstDesc, 0);
}

MXRC MdmaSysCloseLog(void)
{
	MXRC rc = MX_OK;

	rc = LogMgrCloseLog(g_MdmaSysCtx.hLog);

	g_MdmaSysCtx.bLogEnable = FALSE;

	return rc;
}

MXRC MdmaSpuCloseLog(void)
{
	MXRC rc = MX_OK;

	rc = LogMgrCloseLog(g_MdmaSpuCtx.hLog);
	g_MdmaSpuCtx.bLogEnable = FALSE;

	return rc;
}

void MdmaPrintInfo(PMDMA_CTX pMdmaCtx)
{
    int i = 0;

    PMDMA_FDESC pFDesc = MdmaGetActiveFdesc(pMdmaCtx);

    uart_printf("%s:\n", MdmaGetHWName(pMdmaCtx));

    while (pFDesc)
    {
        uart_printf("FDesc[0x%x][%s][%s]->\n", (UINT32*)pFDesc, MdmaGetOpName(pFDesc),(pFDesc->HwContext.StatusComp)?"DONE":"ACTIVE");
        uart_printf("Output buffers:[pointer] [size]\n");
        i = 0;
        do {
            uart_printf("[0x%x] [0x%x]\n", pFDesc->IoBdesc[i].BPtr, (pFDesc->IoBdesc[i].BCtrl & BCTRL_LEN_MASK));
        }
        while(!(pFDesc->IoBdesc[i++].BCtrl & BCTRL_BLAST_MASK));

        uart_printf("Input buffers:[pointer] [size]\n");

        do {
            uart_printf("[0x%x] [0x%x]\n", pFDesc->IoBdesc[i].BPtr, (pFDesc->IoBdesc[i].BCtrl & BCTRL_LEN_MASK));
        }
        while(!(pFDesc->IoBdesc[i++].BCtrl & BCTRL_BLAST_MASK));

        pFDesc = pFDesc->Next;
    }
}

PUINT8 MdmaGetHWName(PMDMA_CTX pMdmaCtx)
{
    int i = 0;

    for (i = 0; i < MDMA_HW_MAX; ++i)
        if (MdmaGetCtx((MDMA_HW_ID)i) == pMdmaCtx)
            return MDMA_HW_NAME[i].name;

    return MDMA_HW_NAME[MDMA_HW_MAX].name;
}

PUINT8 MdmaGetOpName(PMDMA_FDESC pFDesc)
{
    int i = 0;

    MDMA_HW * pMdmaHw = MDMA_GET_HW(pFDesc);

    for (i = 0; i < MDMA_NUMBER_OP_TYPE_NAME - 1; ++i)
        if ( MDMA_OP_TYPE_NAME[i].idCmd == (pMdmaHw->FControl & 0xF000) )
            return MDMA_OP_TYPE_NAME[i].name;

    return MDMA_OP_TYPE_NAME[MDMA_NUMBER_OP_TYPE_NAME - 1].name;
}

void MdmaSysMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    if (MdmaGetPoolSize(MDMA_HW_SYS, nCpuID) != 0)
    {
        MmuMgrCfgMemAP(MDMA_SYS_CTRL_REGS_BASE, 4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
    }
}

void MdmaSpuMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    if (MdmaGetPoolSize(MDMA_HW_SPU, nCpuID) != 0)
    {
        MmuMgrCfgMemAP(MDMA_SPU_CTRL_REGS_BASE, 4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
    }
}

void MdmaRadMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    if (MdmaGetPoolSize(MDMA_HW_RAD, nCpuID) != 0)
    {
        MmuMgrCfgMemAP(MDMA_RAD_CTRL_REGS_BASE, 4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
    }
}

UINT32 MdmaIsMemUsed(UINT32 nAddr, UINT32 nSize)
{
    return 1;
}


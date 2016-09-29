//-------------------------------------------------------------------------------------------
/** @file fpdrv.c
 *
 * @brief MAP (Mindspeed Advanced Processor) HAL functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.85 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "diagnostics.h"
#include "fpdrv.h"
#include "heaps.h"
#include "larmmsgid.h"
#include "appprintf.h"
#include "systimers.h"
#include "4gmx.h"
#include "mdmadrv.h"
#include "devinfo.h"
#include "initcpu.h"
#include "mmumgr.h"
#include "mlog.h"
#include "DSPdrv.h"
#include "rex_serv.h"

extern unsigned fp_fft0128_code[];
extern unsigned fp_fft0128_code_size;
extern unsigned fp_fft0256_code[];
extern unsigned fp_fft0256_code_size;
extern unsigned fp_fft0512_code[];
extern unsigned fp_fft0512_code_size;
extern unsigned fp_fft1024_code[];
extern unsigned fp_fft1024_code_size;
extern unsigned fp_fft2048_code[];
extern unsigned fp_fft2048_code_size;
extern unsigned fp_fft4096_code[];
extern unsigned fp_fft4096_code_size;
extern unsigned fp_fft_table_size;
extern unsigned short fp_fft_table[];
extern unsigned fp_fft_shift_table_size;
extern unsigned short fp_fft_shift_table[];
extern unsigned fp_idft_code[];
extern unsigned fp_idft_code_size;

PMAP_DRV_CTX pMapCtx =  NULL;

UINT16 MapStoredIdftMask = 0;
UINT16 MapStoredIfftMask = 0;
UINT16 MapStoredFftMask  = 0;

#define fp_uart_printf(...)

//-------------------------------------------------------------------------------------------------------------------------------------
// IRQ Handlers
//-------------

UINT32 FpCalcDMAChainSize (PDMAFDESC pDma)
{
    UINT32 nSize = 0;

    while (pDma != NULL)
    {
        nSize += (pDma->OutBDesc.BCtrl & 0xFFFF);
        pDma = pDma->NextFDesc;
    }

    return nSize;
}

PMDMA_FDESC FpAssignToLog (PMAP_DRV_CTX pMapDrvCtx, UINT32 nLogID, PDMAFDESC pDma)
{
    IRQTYPE irq;
    UINT32 nNum = 0;
    UINT32 nSize = 0;
    PDMAFDESC pDmaList = pDma;
    PMDMA_FDESC pMdma, pMdmaHead;
    FP_LOG_CTX * pLogCtx;

    pLogCtx = &pMapDrvCtx->FpLogCtx [nLogID];

    // if log storage is overflowed
    // to save the time, just immediately go out
    // from this proc

    if (pLogCtx->LogFlag & (1<<0))
        return NULL;

    irq = ARM_INT_disable ();

    while (pDmaList != NULL)
    {
        nNum ++;
        nSize += (pDmaList->OutBDesc.BCtrl & 0xFFFF);
        pDmaList = pDmaList->NextFDesc;
    }

    if ((pLogCtx->LogOffs + nSize) > pLogCtx->LogSize)
    {
        uart_printf ("FP: Overflow is detected!\r\n");

        pLogCtx->LogFlag |= (1<<0);  // Overflow
        ARM_INT_restore (irq);

        return NULL;
    }

    // to allocate the chain of descriptors
    // to cover all FP-DMA sub buffers

    //uart_printf ("Assing:%d, size=%d\r\n", nNum, nSize);

    if ((pMdma = MdmaAllocDesc (MDMA_HW_RAD, nNum)) == NULL)
    {
        uart_printf ("FP: Alloc MDMA error!\r\n");

        pLogCtx->LogFlag |= (1<<1);  // MDMA chain allocation error
        ARM_INT_restore (irq);

        return NULL;
    }

    pMdmaHead = pMdma;
    pDmaList  = pDma;

    while ((nNum --) && (pMdma != NULL))
    {
        MdmaSetCopy (pMdma);
        MdmaSetData(pMdma,
                       (LPVOID)pDmaList->OutBDesc.BPtr, pDmaList->OutBDesc.BCtrl&0xFFFF,
                       pLogCtx->LogPtr + pLogCtx->LogOffs, pDmaList->OutBDesc.BCtrl&0xFFFF);

        pLogCtx->LogOffs += pDmaList->OutBDesc.BCtrl&0xFFFF;

        pMdma = pMdma->Next;
        pDmaList = pDmaList->NextFDesc;
    }

    MdmaSetLastDesc (pMdmaHead, NULL, NULL);
    ARM_INT_restore (irq);
    return pMdmaHead;
}

INLINE BOOL MAP_IS_FFT_LOG(PMAP_DRV_CTX pMapDrvCtx)
{
    return (pMapDrvCtx->FpLogStream & (1 << FP_LOG_FFT));
}

INLINE BOOL MAP_IS_IFFT_LOG(PMAP_DRV_CTX pMapDrvCtx)
{
    return (pMapDrvCtx->FpLogStream & (1 << FP_LOG_IFFT));
}

INLINE BOOL MAP_IS_IDFT_LOG(PMAP_DRV_CTX pMapDrvCtx)
{
    return (pMapDrvCtx->FpLogStream & (1 << FP_LOG_IDFT));
}

void MapIrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, LPVOID pParam)
{
    PMAP_DRV_CTX pMapDrvCtx = (PMAP_DRV_CTX)pParam;
    volatile PMAP_MASTER_CTRL_REGS pMasterCtrlRegs = pMapDrvCtx->pMasterCtrlRegs;
    UINT32 FpId = 0;
    UINT32 IntMask;

    CEVA_DRV_CFG * pCevaCfg = DspDrvGetCevaDrvCfgParams();

    UINT32 IntStat = pMasterCtrlRegs->IrqStatAck;

    _ASSERT_PTR(pCevaCfg);

    //uart_printf ("\r\n[IDFT-IRQ]:0x%x\r\n", IntStat);

    do
    {
        IntMask = (1<<FpId);
        if (IntStat & IntMask)
        {
            // Clear Interrupt and acknowledge
            IntStat &= ~IntMask;
            pMasterCtrlRegs->IrqStatAck = IntMask;

            // Remove busy flag
            pMapDrvCtx->BusyDev &= ~(1<<FpId);

            // to run DMA to log out data
            // if log was assigned

            if (pMapDrvCtx->FpOutLog[FpId].pMdmaChain != NULL)
            {
                //fp_uart_printf ("Run copy, fp=%d\r\n", FpId);

                MdmaRunEx (MDMA_HW_RAD, pMapDrvCtx->FpOutLog[FpId].pMdmaChain, MdmaEmptyCallback, NULL);

                pMapDrvCtx->FpOutLog[FpId].Buf = NULL;
                pMapDrvCtx->FpOutLog[FpId].pMdmaChain = NULL;
            }

            if (IntMask & pMapDrvCtx->IdftFpMask)                 // IDFT
            {
                if (pMapDrvCtx->IdftRun == 0xFFFF)  // Special case where we are copying IDFT tables during INIT
                {
                    pMapDrvCtx->IdftRun = 0;
                    FpId++;
                    continue;
                }
                else
                {
                    if ((pMapDrvCtx->FpDrvCfg & FP_DRV_CFG_IDFT_ON_CEVA) != 0)
                    {
                        pCevaCfg->FpCfg.IdftRunMask &= ~IntMask;
                    }
                    else
                    {
                        // Cear Run Status
                        pMapDrvCtx->IdftRun &= ~IntMask;

                        // Service Interrupt Routines
                        if (pMapDrvCtx->IdftRun == 0)
                        {
                            if (pMapDrvCtx->IdftTcb != NULL)
                            {
                                pMapDrvCtx->IdftTcb->ExecTicks = GetTIMETICK() - pMapDrvCtx->IdftTcb->ExecTicks;
                                MLogEXEFinish(pMapDrvCtx->IdftTcb);
                                pMapDrvCtx->IdftPostProcCb(pMapDrvCtx->IdftTcb);
                                pMapDrvCtx->IdftTcb = NULL;
                            }
                        }
                    }
                }
            }
        }
        FpId++;
    }while (IntStat);

    *pIntClearReg = nIrqID;
}


void MapDma1IrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, void *pParam)
{
    PMAP_DRV_CTX pMapDrvCtx = (PMAP_DRV_CTX)pParam;
    UINT32 callBack, NewDispatchPool, ctxNum = 5;

    pMapDrvCtx->pFpDmaRegs[FP_GRP1]->DMAOutCtrl.IrqClear = DMAIRQ_FCTRL;

    pMapDrvCtx->pool1FpBusy = 0;

    // Save the Queue Element whose CallBack has to be made
    callBack = pMapDrvCtx->QueueFftIfft[pMapDrvCtx->QueueLocationToDo];
    pMapDrvCtx->QueueFftIfft[pMapDrvCtx->QueueLocationToDo] = NOT_RUNNING;

    pMapDrvCtx->QueueLocationToDo++;
    if (pMapDrvCtx->QueueLocationToDo >= 4)
        pMapDrvCtx->QueueLocationToDo = 0;

    // Send New Descriptors to FP to run if present
    NewDispatchPool = pMapDrvCtx->QueueFftIfft[pMapDrvCtx->QueueLocationToDo];

    if (NewDispatchPool < NOT_RUNNING)          // Means something new in Queue
    {
        if (NewDispatchPool <= FFT_SLOT1)        // FFT
        {
            // Set the master Control Regs
            MapDrvSetModeEx(
            (FP_GRP1|FP_DRV_MODE_FEXP_POOL|(FP_DRV_LOAD_TYPE0 << 8)), 0,
            *(PUINT64)&pMapDrvCtx->FftOutTrCtrl, pMapDrvCtx->FftOutBufSize, 0);
        }
        else                                    // IFFT
        {
            // Set the master Control Regs
            MapDrvSetModeEx(
            (FP_GRP1|FP_DRV_MODE_FEXP_POOL|(FP_DRV_LOAD_TYPE0 << 8)), 0,
            *(PUINT64)&pMapDrvCtx->IfftOutTrCtrl[0],
            (pMapDrvCtx->IfftCp1OutSize + pMapDrvCtx->IfftOutSize),
            0);
        }

        pMapDrvCtx->pool1FpBusy = 1;

        switch(NewDispatchPool)
        {
            case FFT_SLOT0:
                ctxNum = pMapDrvCtx->QueueToDoContext[FFT_SLOT0];
                pMapDrvCtx->QueueToDoContext[FFT_SLOT0] = 5;
                MLogEXEStart(pMapDrvCtx->FftCb[0], 5, 0);
                ((PTCB)pMapDrvCtx->FftCb[0])->ExecTicks = MxGetTicks();
            break;
            case FFT_SLOT1:
                ctxNum = pMapDrvCtx->QueueToDoContext[FFT_SLOT1];
                pMapDrvCtx->QueueToDoContext[FFT_SLOT1] = 5;
                MLogEXEStart(pMapDrvCtx->FftCb[1], 5, 0);
                ((PTCB)pMapDrvCtx->FftCb[1])->ExecTicks = MxGetTicks();
            break;

            case IFFT_SLOT0:
                ctxNum = pMapDrvCtx->QueueToDoContext[IFFT_SLOT0];
                pMapDrvCtx->QueueToDoContext[IFFT_SLOT0] = 5;
                MLogEXEStart(pMapDrvCtx->IfftCb[0], 5, 0);
                ((PTCB)pMapDrvCtx->IfftCb[0])->ExecTicks = MxGetTicks();
            break;

            case IFFT_SLOT1:
                ctxNum = pMapDrvCtx->QueueToDoContext[IFFT_SLOT1];
                pMapDrvCtx->QueueToDoContext[IFFT_SLOT1] = 5;
                MLogEXEStart(pMapDrvCtx->IfftCb[1], 5, 0);
                ((PTCB)pMapDrvCtx->IfftCb[1])->ExecTicks = MxGetTicks();
            break;

            default:
            break;
        }

        if (ctxNum == 5)
        {
            stop_printf("Something wrong with ctx Num: Queue[%d %d %d %d] Ctx: [%d %d %d %d] ToDo: [%d %d %d %d] NewPool: %d CB: %d\r\n",
                pMapDrvCtx->QueueFftIfft[0], pMapDrvCtx->QueueFftIfft[1],
                pMapDrvCtx->QueueFftIfft[2], pMapDrvCtx->QueueFftIfft[3],
                pMapDrvCtx->QueueContext[0], pMapDrvCtx->QueueContext[1],
                pMapDrvCtx->QueueContext[2], pMapDrvCtx->QueueContext[3],
                pMapDrvCtx->QueueToDoContext[0], pMapDrvCtx->QueueToDoContext[1],
                pMapDrvCtx->QueueToDoContext[2], pMapDrvCtx->QueueToDoContext[3],
                NewDispatchPool, callBack);
        }
        else
            MapDispatchFexpPool(1, pMapDrvCtx->pReadyListFftIfftHead[ctxNum][NewDispatchPool], pMapDrvCtx->pReadyListFftIfftHead[ctxNum][NewDispatchPool] + 1);
    }


    // Send Call Back
    switch(callBack)
    {
        case FFT_SLOT0:
            ((PTCB)pMapDrvCtx->FftCb[0])->ExecTicks =
                MxGetTicks() - ((PTCB)pMapDrvCtx->FftCb[0])->ExecTicks;
            MLogEXEFinish(pMapDrvCtx->FftCb[0]);
            if (pMapDrvCtx->FftPostProcCb)
                pMapDrvCtx->FftPostProcCb(pMapDrvCtx->FftCb[0]);
        break;

        case FFT_SLOT1:
            ((PTCB)pMapDrvCtx->FftCb[1])->ExecTicks =
                MxGetTicks() - ((PTCB)pMapDrvCtx->FftCb[1])->ExecTicks;
            MLogEXEFinish(pMapDrvCtx->FftCb[1]);
            if (pMapDrvCtx->FftPostProcCb)
                pMapDrvCtx->FftPostProcCb(pMapDrvCtx->FftCb[1]);
        break;

        case IFFT_SLOT0:
            ((PTCB)pMapDrvCtx->IfftCb[0])->ExecTicks =
                MxGetTicks() - ((PTCB)pMapDrvCtx->IfftCb[0])->ExecTicks;
            MLogEXEFinish(pMapDrvCtx->IfftCb[0]);
            if (pMapDrvCtx->IfftPostProcCb)
                pMapDrvCtx->IfftPostProcCb(pMapDrvCtx->IfftCb[0]);
        break;

        case IFFT_SLOT1:
            ((PTCB)pMapDrvCtx->IfftCb[1])->ExecTicks =
                MxGetTicks() - ((PTCB)pMapDrvCtx->IfftCb[1])->ExecTicks;
            MLogEXEFinish(pMapDrvCtx->IfftCb[1]);
            if (pMapDrvCtx->IfftPostProcCb)
                pMapDrvCtx->IfftPostProcCb(pMapDrvCtx->IfftCb[1]);
        break;

        default:
        break;
    }

    *pIntClearReg = nIrqID;
}

//-------------------------------------------------------------------------------------------------------------------------------------










//-------------------------------------------------------------------------------------------------------------------------------------
// Generic FP Functions
//--------------------



PMAP_DRV_CTX MapGetDrvCtx(void)
{
    return pMapCtx;
}


//-------------------------------------------------------------------------------------------
/** @brief This function resets the MAP Driver.
 *
 *
 *  @param   none
 *
 *  @return  returns MX_OK
 *
 * Functions it performs:
 *   - Resets MAP hardware registers
 *   - Resets MAPDMA hardware registers
 *   - Reset the PLL Clock source that the FPs are attached to
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapDrvReset(void)
{
    volatile PMAP_DMA_REGS pFpDmaRegs0, pFpDmaRegs1;
    volatile PMAP_MASTER_CTRL_REGS pMasterCtrlRegs = (PMAP_MASTER_CTRL_REGS)(FP_CTRL_BASEADDR + FP_MASTER_CTRL_REG_OFFSET);
    volatile PMAP_APB_REGS pApbRegs;
    UINT32 i;
    UINT32 FpDmaBurstLen = 16;

    pFpDmaRegs0 = (PMAP_DMA_REGS)(FP_CTRL_BASEADDR + FP_DMA_REG_OFFSET(0));
    pFpDmaRegs1 = (PMAP_DMA_REGS)(FP_CTRL_BASEADDR + FP_DMA_REG_OFFSET(1));

    REG32(FFT_RESET_0) = 0x0;
    REG32(FFT_RESET_1) = 0x0;

    for(i = 0; i < FPDRV_NUM_FP; ++i)
    {
        pApbRegs = (PMAP_APB_REGS)(FP_CTRL_BASEADDR + i*0x800);
        pApbRegs->Ctrl = FP_CTRL_RESET | FP_CTRL_RESET_IO; // put FP core in reset and Reset the state machines and “LOADED” signal in IO module
        _NOP();
        _NOP();
        pApbRegs->Ctrl = 0;
    }

    REG32(FFT_RESET_0) = 0xF0;
    REG32(FFT_RESET_1) = 0x3F;

    pFpDmaRegs0->DMAInCtrl.SoftReset = 1;
    pFpDmaRegs0->DMAOutCtrl.SoftReset = 1;
    pFpDmaRegs1->DMAInCtrl.SoftReset = 1;
    pFpDmaRegs1->DMAOutCtrl.SoftReset = 1;
    pMasterCtrlRegs->Ctrl1 |= (1<<16) | (1<<17);
    pFpDmaRegs0->DMAInCtrl.SoftReset = 1;
    pFpDmaRegs0->DMAOutCtrl.SoftReset = 1;
    pFpDmaRegs1->DMAInCtrl.SoftReset = 1;
    pFpDmaRegs1->DMAOutCtrl.SoftReset = 1;
    REG32(FFT_RESET_0) = 0x0;
    REG32(FFT_RESET_1) = 0x0;

    // Mem to IO
    REG32(FP_CTRL_BASEADDR + FP_DMA_REG_OFFSET(0) + 0x200) = 0x70; //read threshold
    // IO to Mem
    REG32(FP_CTRL_BASEADDR + FP_DMA_REG_OFFSET(0) + 0x204) = 0xf0; //write threshold

    pFpDmaRegs0->DMAInCtrl.BurstLength = FpDmaBurstLen-1;
    pFpDmaRegs0->DMAOutCtrl.BurstLength = FpDmaBurstLen-1;
    //pFpDmaRegs0->DMAOutCtrl.DmaIrqEnable = DMAIRQ_FCTRL;

    // Mem to IO
    REG32(FP_CTRL_BASEADDR + FP_DMA_REG_OFFSET(1) + 0x200) = 0x70; //read threshold
    // IO to Mem
    REG32(FP_CTRL_BASEADDR + FP_DMA_REG_OFFSET(1) + 0x204) = 0xf0; //write threshold

    pFpDmaRegs1->DMAInCtrl.BurstLength = FpDmaBurstLen-1;
    pFpDmaRegs1->DMAOutCtrl.BurstLength = FpDmaBurstLen-1;
    //pFpDmaRegs1->DMAOutCtrl.DmaIrqEnable = DMAIRQ_FCTRL;

    if (DevInfoIsT4Kx4 ())
    {
        REG32(FFT_CLK_CNTRL) = PLL_SOURCE(2);
    }

    return MX_OK;
}


UINT32 MapDrvResetEx(void)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    volatile PMAP_DMA_REGS pFpDmaRegs0, pFpDmaRegs1;
    UINT32 i;
    UINT32 FpDmaBurstLen=16; // 1..16


    pMapDrvCtx->pMasterCtrlRegs = (PMAP_MASTER_CTRL_REGS)(FP_CTRL_BASEADDR + FP_MASTER_CTRL_REG_OFFSET);
    pMapDrvCtx->pMasterCtrlRegs->Ctrl1 = 0x3FF; // configure all FPs to run in Pool mode

    pMapDrvCtx->pFpDmaRegs[FP_GRP0] = (PMAP_DMA_REGS)(FP_CTRL_BASEADDR + FP_DMA_REG_OFFSET(FP_GRP0));
    pMapDrvCtx->pFpDmaRegs[FP_GRP1] = (PMAP_DMA_REGS)(FP_CTRL_BASEADDR + FP_DMA_REG_OFFSET(FP_GRP1));
    pFpDmaRegs0 = pMapDrvCtx->pFpDmaRegs[FP_GRP0];
    pFpDmaRegs1 = pMapDrvCtx->pFpDmaRegs[FP_GRP1];

    pMapDrvCtx->pDmaExpRegs[FP_GRP0] = (PMAP_DMA_EXP_REGS)(FP_CTRL_BASEADDR + 0xE400); // use FPDMA0
    pMapDrvCtx->pDmaExpRegs[FP_GRP1] = (PMAP_DMA_EXP_REGS)(FP_CTRL_BASEADDR + 0xEC00); // use FPDMA1

    for (i = 0; i < FPDRV_NUM_FP; ++i)
    {
            pMapDrvCtx->pApbRegs[i] = (PMAP_APB_REGS)(FP_CTRL_BASEADDR + i*0x800);
    }

    pMapDrvCtx->pMasterCtrlRegs->Ctrl1 = 0;
    pMapDrvCtx->pMasterCtrlRegs->IrqEna = 0;
    pMapDrvCtx->pMasterCtrlRegs->Grp0OutTrLen = 0;//no pool as default
    pMapDrvCtx->pMasterCtrlRegs->Grp0OutLoad0Ctrl = 0;//no pool
    pMapDrvCtx->pMasterCtrlRegs->Grp0OutLoad1Ctrl = 0;//no pool
    pMapDrvCtx->pMasterCtrlRegs->Grp1OutTrLen = 0;//no pool as default
    pMapDrvCtx->pMasterCtrlRegs->Grp1OutLoad0Ctrl = 0;//no pool
    pMapDrvCtx->pMasterCtrlRegs->Grp1OutLoad1Ctrl = 0;//no pool

    pMapDrvCtx->pDmaExpRegs[FP_GRP0]->Enable = 0; //disable Fexp as default
    pMapDrvCtx->pDmaExpRegs[FP_GRP1]->Enable = 0;


    REG32(FFT_RESET_0) = 0x0;
    REG32(FFT_RESET_1) = 0x0;

    for(i = 0; i < FPDRV_NUM_FP; ++i)
    {
        pMapDrvCtx->pApbRegs[i]->Ctrl = FP_CTRL_RESET | FP_CTRL_RESET_IO;
        pMapDrvCtx->pApbRegs[i]->TReqEna = 0; //disable all target request as default
        _NOP();
        _NOP();
        pMapDrvCtx->pApbRegs[i]->Ctrl = 0;
    }

    REG32(FFT_RESET_0) = 0xF0;
    REG32(FFT_RESET_1) = 0x3F;

    pFpDmaRegs0->DMAInCtrl.SoftReset = 1;
    pFpDmaRegs0->DMAOutCtrl.SoftReset = 1;
    pFpDmaRegs1->DMAInCtrl.SoftReset = 1;
    pFpDmaRegs1->DMAOutCtrl.SoftReset = 1;
    pFpDmaRegs0->DMAInCtrl.SoftReset = 1;
    pFpDmaRegs0->DMAOutCtrl.SoftReset = 1;
    pFpDmaRegs1->DMAInCtrl.SoftReset = 1;
    pFpDmaRegs1->DMAOutCtrl.SoftReset = 1;

    REG32(FFT_RESET_0) = 0x0;
    REG32(FFT_RESET_1) = 0x0;

    // Group 0
    // Mem to IO
    REG32(FP_CTRL_BASEADDR + FP_DMA_REG_OFFSET(FP_GRP0) + 0x200) = 0x70; //read threshold
    // IO to Mem
    REG32(FP_CTRL_BASEADDR + FP_DMA_REG_OFFSET(FP_GRP0) + 0x204) = 0xf0; //write threshold

    pFpDmaRegs0->DMAInCtrl.BurstLength = FpDmaBurstLen-1;
    pFpDmaRegs0->DMAOutCtrl.BurstLength = FpDmaBurstLen-1;
    //pFpDmaRegs0->DMAOutCtrl.DmaIrqEnable = DMAIRQ_FCTRL;

    // Group 1
    // Mem to IO
    REG32(FP_CTRL_BASEADDR + FP_DMA_REG_OFFSET(FP_GRP1) + 0x200) = 0x70; //read threshold
    // IO to Mem
    REG32(FP_CTRL_BASEADDR + FP_DMA_REG_OFFSET(FP_GRP1) + 0x204) = 0xf0; //write threshold

    pFpDmaRegs1->DMAInCtrl.BurstLength = FpDmaBurstLen-1;
    pFpDmaRegs1->DMAOutCtrl.BurstLength = FpDmaBurstLen-1;
    //pFpDmaRegs1->DMAOutCtrl.DmaIrqEnable = DMAIRQ_FCTRL;

    if (DevInfoIsT4Kx4 ())
    {
        REG32(FFT_CLK_CNTRL) = PLL_SOURCE(2);
    }

    return MX_OK;
}





//-------------------------------------------------------------------------------------------
/** @brief This function Shutdown the FFT Driver. Cleaning up allocated memory
 *
 *
 *  @param   none
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapFftDrvShutdown(void)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    if (pMapDrvCtx == NULL)
        return MX_OK;

    if (pMapDrvCtx->pFftDmaDesc != NULL)
    {
        MxHeapFree(DspHeap, pMapDrvCtx->pFftDmaDesc);

        pMapDrvCtx->pFftDmaDesc = NULL;
    }

    return MX_OK;
}



//-------------------------------------------------------------------------------------------
/** @brief This function Shutdown the IFFT Driver. Cleaning up allocated memory
 *
 *
 *  @param   none
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIfftDrvShutdown(void)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    if (pMapDrvCtx == NULL)
        return MX_OK;

    if (pMapDrvCtx->pIfftDmaDesc != NULL)
    {
        MxHeapFree(DspHeap, pMapDrvCtx->pIfftDmaDesc);

        pMapDrvCtx->pIfftDmaDesc = NULL;
    }

    return MX_OK;
}



//-------------------------------------------------------------------------------------------
/** @brief This function Shutdown the IDFT Driver. Cleaning up allocated memory
 *
 *
 *  @param   none
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIdftDrvShutdown(void)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    if (pMapDrvCtx == NULL)
        return MX_OK;

    if (pMapDrvCtx->pIdftDmaDesc)
    {
        MxHeapFree(DspHeap, pMapDrvCtx->pIdftDmaDesc);
        pMapDrvCtx->pIdftDmaDesc = NULL;
    }

    if (pMapDrvCtx->IdftAppHeapAlloc)
    {
        MxHeapFree(AppHeap, pMapDrvCtx->IdftAppHeapAlloc);
        pMapDrvCtx->IdftAppHeapAlloc = NULL;
    }

    if (pMapDrvCtx->IdftCramHeapAlloc)
    {
        MxHeapFree(DspHeap, pMapDrvCtx->IdftCramHeapAlloc);
        pMapDrvCtx->IdftCramHeapAlloc = NULL;
    }

    return MX_OK;
}



//-------------------------------------------------------------------------------------------
/** @brief This function Shutdown the MAP Driver. Cleaning up allocated memory and resets the
 *         driver variables. Also disables all interrupts between AMR and MAP / MAPDMA
 *
 *
 *  @param   none
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapDrvShutdown (void)
{
    IRQTYPE irq;

    if (pMapCtx == NULL)
        return MX_OK;

    if (pMapCtx->CpuID != MxGetCpuID ())
        return MX_OK;

    //uart_printf ("FP shutdown ... ");

    irq = ARM_INT_disable ();

    MapFftDrvShutdown();
    MapIfftDrvShutdown();
    MapIdftDrvShutdown();

    MapDrvResetEx();

    MxHeapFree (AppHeap, pMapCtx);
    pMapCtx = NULL;

    MxDisableInt(HAL_IRQ_FP_TO_ARM);
    MxDisableInt(HAL_IRQ_FPDMA1_OUT);

    ARM_INT_restore (irq);

    //uart_printf ("OK\n");

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function just stops device and does not free any resources
 *          like allocated memory. It's needed to save allocated resources for
 *          the futher debugging and do not touch potentially corrupted heaps
 *
 *
 *  @param   none
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapDrvGenShedding (void)
{
    IRQTYPE irq;

    if (pMapCtx == NULL)
        return MX_OK;

    if (pMapCtx->CpuID != MxGetCpuID ())
        return MX_OK;

    irq = ARM_INT_disable ();

    MapDrvResetEx();

    MxDisableInt(HAL_IRQ_FP_TO_ARM);

    ARM_INT_restore (irq);

    return MX_OK;
}


MXRC MapDrvGetMasterCpu (void)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    if (pMapDrvCtx == NULL)
        return 0xFFFF;

    _ASSERT_PTR (pMapDrvCtx);

    return pMapDrvCtx->CpuID;
}

//-------------------------------------------------------------------------------------------
/** @brief This function enables/disables logging for particular FP processor
 *         It's possible to enable IN and OUT buffers, BUT to activate logging
 *         an application should also activate FFT or IFFT or IDFT stream logging
 *
 *
 *  @param   nMask [in] - the logging mask:  [MAP9(x,y):MAP8(x,y)..:MAP0(x,y)] y- in stream, x - out stream
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapSetDevLogMask (UINT32 nMask)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    _ASSERT_PTR (pMapDrvCtx);

    pMapDrvCtx->FpLogMask = nMask;

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function sets LOG pointers for FFT/IFFT/IDFT streams
 *
 *  @param   FpStream [in] - the MAP stream, see: FP_LOG_xxx
 *  @param   LogPtr   [in] - the pointer to the buffer
 *  @param   nLogSize [in] - the storage size in bytes
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapSetLogStorage (UINT32 FpStream, LPVOID LogPtr, UINT32 nLogSize)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    _ASSERT_PTR (pMapDrvCtx);

    if (FpStream < 3)
    {
        pMapDrvCtx->FpLogCtx[FpStream].LogPtr = LogPtr;
        pMapDrvCtx->FpLogCtx[FpStream].LogOffs= 0;
        pMapDrvCtx->FpLogCtx[FpStream].LogFlag= 0;
        pMapDrvCtx->FpLogCtx[FpStream].LogSize= nLogSize;

        memset (LogPtr, 0xA5, nLogSize);
    }

    return MX_OK;
}

BOOL MapIsInLogEnabled(UINT32 nFpID)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    _ASSERT_PTR (pMapDrvCtx);

    return (pMapDrvCtx->FpLogMask >> (nFpID*2)) & (1 << 0);
}

BOOL MapIsOutLogEnabled(UINT32 nFpID)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    _ASSERT_PTR (pMapDrvCtx);

    return (pMapDrvCtx->FpLogMask >> (nFpID*2)) & (1 << 1);
}

//-------------------------------------------------------------------------------------------
/** @brief This function enables/disables logging of FFT/IFFT/IDFT streams
 *
 *  @param   FpStream [in] - the MAP stream, see: FP_LOG_xxx
 *  @param   bEnable  [in] - 1 - to enable stream, 0 - to disable logging of this stream
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapEnableLogStream (UINT32 FpStream, BOOL bEnable)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    _ASSERT_PTR (pMapDrvCtx);

    if (bEnable)
        pMapDrvCtx->FpLogStream |= (1 << FpStream);
    else
        pMapDrvCtx->FpLogStream &= ~(1 << FpStream);

    return MX_OK;
}


MXRC MapDrvRestoreMapMask (void)
{
    if (pMapCtx == NULL)
        return FP_DRV_RC_INIT_ERROR;

    uart_printf ("\r\n");

    MapIdftDrvSetMapMask(MapStoredIdftMask);
    MapIfftDrvSetMapMask(MapStoredIfftMask);
    MapFftDrvSetMapMask(MapStoredFftMask);

    return MX_OK;
}

BOOL MapDrvIsPendIrq (void)
{
    if (pMapCtx == NULL)
        return FALSE;

    return pMapCtx->pMasterCtrlRegs->IrqStatAck != 0;
}


//-------------------------------------------------------------------------------------------
/** @brief This function initializes the MAP driver.
 *
 *
 *  @param   fp_mode This is basically the size of the FFT / IFFT routine to run. It is explained
 *                   in table below. 2^fp_mode = fftSize to be used
 *
 *  @param   fp_cfg [in] - the FP driver configurations (ARM/CEVA modes: see FP_DRV_CFG_xxx)
 *  @return  returns MX_OK
 *
 * Functions it performs
 *  - Allocate memory and initialize variables used by driver
 *  - Assign default MAPs to each function
 *     - 0-4 IDFT
 *     - 5-9 FFT and IFFT
 *  - Initialize FFT, IFFT and IDFT drivers
 *  - Loads MAP code to each of the MAPs as pre-designed
 *  - Runs the MAPs in the FEXP mode. FPOOL mode is not used
 *  - Registers FP to ARM interrupts and starts this.
 *
 * TABLE_START
 * ---------------------------------------------
 * |FP_MODE             |FFT/IFFT Size         |
 * ---------------------------------------------
 * |FPMODE_FFT0128 (7)  |128 pt. FFT / IFFT    |
 * ---------------------------------------------
 * |FPMODE_FFT0256 (8)  |256 pt. FFT / IFFT    |
 * ---------------------------------------------
 * |FPMODE_FFT0512 (9)  |512 pt. FFT / IFFT    |
 * ---------------------------------------------
 * |FPMODE_FFT1024 (10) |1024 pt. FFT / IFFT   |
 * ---------------------------------------------
 * |FPMODE_FFT2048 (11) |2048 pt. FFT / IFFT   |
 * ---------------------------------------------
 * TABLE_END
 *
 * <BR/><BR/>
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapDrvInit(UINT32 fp_mode, UINT32 fp_cfg)
{
    PMAP_DRV_CTX pMapDrvCtx;
    UINT32 FpId;
    MXRC rc = MX_OK;
    UINT32 IntStat, IntMask, i;
    IRQTYPE irq;

    if (pMapCtx == NULL)
    {
        pMapCtx = MxHeapAlloc(AppHeap, sizeof(MAP_DRV_CTX));
        _ASSERT_PTR(pMapCtx);

        if (pMapCtx == NULL)
            return FP_DRV_RC_ALLOC_ERROR;

        memset(pMapCtx, 0, sizeof(MAP_DRV_CTX));

        pMapCtx->FpDrvCfg   = fp_cfg;
        pMapCtx->CpuID      = MxGetCpuID ();
        pMapCtx->FpDevNum   = DevInfoGetNumber(DEVID_FP);

        // Allocate FPs to different programs

        uart_printf("MAPDRV Init: Setting Default Masks\n");

        // IDFT
        pMapCtx->IdftFpStart = pMapCtx->IdftCurFp = DevInfoGetValue(CONSTID_IDFT_FP_ID_START);
        pMapCtx->IdftFpStop = DevInfoGetValue(CONSTID_IDFT_FP_ID_STOP);
        //pMapCtx->IdftFpMask = 0x1F;   // 00000 11111
        pMapCtx->IdftFpMask = FP_DRV_GET_MASK(pMapCtx->IdftFpStart, pMapCtx->IdftFpStop);

        // IFFT
        pMapCtx->IfftFpStart = pMapCtx->IfftCurFp = DevInfoGetValue(CONSTID_IFFT_FP_ID_START);
        pMapCtx->IfftFpStop = DevInfoGetValue(CONSTID_IFFT_FP_ID_STOP);
        //pMapCtx->IfftFpMask = 0x3E0;   // 11111 00000
        pMapCtx->IfftFpMask = FP_DRV_GET_MASK(pMapCtx->IfftFpStart, pMapCtx->IfftFpStop);

        // FFT
        pMapCtx->FftFpStart = pMapCtx->FftCurFp = DevInfoGetValue(CONSTID_FFT_FP_ID_START);
        pMapCtx->FftFpStop = DevInfoGetValue(CONSTID_FFT_FP_ID_STOP);
        //pMapCtx->FFtFpMask  = 0x3E0;   // 11111 00000
        pMapCtx->FFtFpMask = FP_DRV_GET_MASK(pMapCtx->FftFpStart, pMapCtx->FftFpStop);

    }

    pMapDrvCtx = MapGetDrvCtx();
    MapDrvResetEx();

    _ASSERT_PTR (pMapDrvCtx);

    // protection to prevent FP driver initialization on
    // differect cores at the same time

    if (pMapDrvCtx->CpuID != MxGetCpuID ())
    {
        _ASSERT (pMapDrvCtx->CpuID == MxGetCpuID ());

        return FP_DRV_RC_CPU_INTERSECTION;
    }

    // Slot Based Processing Variables
    for (i = 0; i < NUM_MAP_SLOTS; i++)
    {
        pMapDrvCtx->FftCb[i] = NULL;
        pMapDrvCtx->IfftCb[i] = NULL;
    }

    // Initialize some variables for FFT/IFFT
    pMapDrvCtx->FftSize = 1<<fp_mode;
    pMapDrvCtx->nSymIFFT = 7;       // for Slot Based
    pMapDrvCtx->nSymFFT = 7;        // for Slot Based
    pMapDrvCtx->nNumIfftSlot = 2;   // Always 2 Slots

    // Reset queueing mechanism
    MapFttIfftDrvFlushQueue();

    // Initalize all context variables used in driver
    MapFftDrvInit(fp_mode);
    MapIfftDrvInit(fp_mode);
    MapIdftDrvInit();

    // Load code onto FPs
    MapDrvLoadCode((~(pMapDrvCtx->IdftFpMask)) & 0x3FF, FALSE);   // FFT / IFFT Code
    MapDrvLoadCode(pMapDrvCtx->IdftFpMask, TRUE);                  // IDFT Code


    // Set mode of operation for FPDMAs
    rc |= MapDrvSetModeEx((FP_GRP0|FP_DRV_MODE_FEXP|(FP_DRV_LOAD_TYPE1 << 8)), pMapDrvCtx->IdftFpMask, 0, 0, 1);
    rc |= MapDrvSetModeEx((FP_GRP1|FP_DRV_MODE_FEXP_POOL|(FP_DRV_LOAD_TYPE0 << 8)), ~(pMapDrvCtx->IdftFpMask),
        *(PUINT64)&pMapDrvCtx->FftOutTrCtrl, pMapDrvCtx->FftOutBufSize, 1);

    uart_printf ("MAP driver init - %s, cpu-%d (ret-%d)\r\n", FAILED(rc) ? "FAILED" : "OK", MxGetCpuID (), rc);

    // Clear all IRQs
    IntStat = pMapDrvCtx->pMasterCtrlRegs->IrqStatAck;

    FpId = 0;

    irq = ARM_INT_disable ();

    do
    {
        IntMask = (1<<FpId);
        if (IntStat & IntMask)
        {
            // Clear Interrupt and acknowledge
            IntStat &= ~IntMask;
            pMapDrvCtx->pMasterCtrlRegs->IrqStatAck = IntMask;
        }
        FpId++;
    }while(FpId != pMapCtx->FpDevNum);

    ARM_INT_restore(irq);

    // Assign interrupt handlers when interrupts are received
    MxSetIntHandler(HAL_IRQ_FP_TO_ARM, INT_TYPE_IRQ, PRI_MAP_ARM, MapIrqHandler, MapGetDrvCtx());
    MxSetIntHandler(HAL_IRQ_FPDMA1_OUT, INT_TYPE_IRQ, PRI_MAP_DMA, MapDma1IrqHandler, MapGetDrvCtx());

    MxEnableInt(HAL_IRQ_FP_TO_ARM);
    MxEnableInt(HAL_IRQ_FPDMA1_OUT);

    for (FpId = 0; FpId < pMapCtx->FpDevNum; FpId++)
    {
        if (pMapCtx->IdftFpMask & (1<<FpId))
            rc |= MapIdftDrvInitLoadBigTablesToB7(FpId);
    }

    _ASSERT_RC(rc);

    return rc;
}

//-------------------------------------------------------------------------------------------------------------------------------------








//-------------------------------------------------------------------------------------------------------------------------------------
// IDFT Functions
//-------------


//-------------------------------------------------------------------------------------------
/** @brief This function initializes the IDFT driver variables
 *
 *
 *  @param   none
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIdftDrvInit(void)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    UINT32 i, j, k;
    PDMAFDESC pIdftFDescHead, pInFDesc, pConfigFDesc, pMultTableFDesc;
    UINT32 appHeapAllocSize, dspHeapAllocSize;
    PUINT8 pAppHeap, pCramHeap;

    if (pMapDrvCtx->pIdftDmaDesc == NULL)
    {
        pMapDrvCtx->pIdftDmaDesc = MxHeapAlloc(DspHeap, 2*IDFT_DRV_QSIZE*(MAX_NUM_IDFT_DMA_DESC*sizeof(DMAFDESC)));     // 2 Contexts
        _ASSERT_PTR(pMapDrvCtx->pIdftDmaDesc);

        if (pMapDrvCtx->pIdftDmaDesc == NULL)
            return FP_DRV_RC_ALLOC_ERROR;
    }

    memset(pMapDrvCtx->pIdftDmaDesc, 0, 2*IDFT_DRV_QSIZE*(MAX_NUM_IDFT_DMA_DESC*sizeof(DMAFDESC)));

    // set up IDFT FPDMA Data descriptor: Input Data
    pMapDrvCtx->IdftInTrCtrl[0].FpId = 0;
    pMapDrvCtx->IdftInTrCtrl[0].LoadType = FP_DRV_LOAD_TYPE1;
    pMapDrvCtx->IdftInTrCtrl[0].LoadCont = 1;
    pMapDrvCtx->IdftInTrCtrl[0].StartBank = 1<<0;
    pMapDrvCtx->IdftInTrCtrl[0].SegStart = 0;
    pMapDrvCtx->IdftInTrCtrl[0].BusSize = 1;
    pMapDrvCtx->IdftInTrCtrl[0].DataType = 0;
    pMapDrvCtx->IdftInTrCtrl[0].TrInt = 0;
    pMapDrvCtx->IdftInTrCtrl[0].StartAddr = 512;
    pMapDrvCtx->IdftInTrCtrl[0].SegCount = 0;
    pMapDrvCtx->IdftInTrCtrl[0].TrId = 0;

    // set up IDFT FPDMA Data descriptor: Small Config
    pMapDrvCtx->IdftInTrCtrl[1].FpId = 0;
    pMapDrvCtx->IdftInTrCtrl[1].LoadType = FP_DRV_LOAD_TYPE1;
    pMapDrvCtx->IdftInTrCtrl[1].LoadCont = 1;
    pMapDrvCtx->IdftInTrCtrl[1].StartBank = 1<<0;
    pMapDrvCtx->IdftInTrCtrl[1].SegStart = 0;
    pMapDrvCtx->IdftInTrCtrl[1].BusSize = 1;
    pMapDrvCtx->IdftInTrCtrl[1].DataType = 0;
    pMapDrvCtx->IdftInTrCtrl[1].TrInt = 0;
    pMapDrvCtx->IdftInTrCtrl[1].StartAddr = 0;
    pMapDrvCtx->IdftInTrCtrl[1].SegCount = 0;
    pMapDrvCtx->IdftInTrCtrl[1].TrId = 0;

    // set up IDFT FPDMA Data descriptor: Tables
    pMapDrvCtx->IdftInTrCtrl[2].FpId = 0;
    pMapDrvCtx->IdftInTrCtrl[2].LoadType = FP_DRV_LOAD_TYPE1;
    pMapDrvCtx->IdftInTrCtrl[2].LoadCont = 0;
    pMapDrvCtx->IdftInTrCtrl[2].StartBank = 1<<2;
    pMapDrvCtx->IdftInTrCtrl[2].SegStart = 0;
    pMapDrvCtx->IdftInTrCtrl[2].BusSize = 1;
    pMapDrvCtx->IdftInTrCtrl[2].DataType = 0;
    pMapDrvCtx->IdftInTrCtrl[2].TrInt = 1;
    pMapDrvCtx->IdftInTrCtrl[2].StartAddr = 0;
    pMapDrvCtx->IdftInTrCtrl[2].SegCount = 0;
    pMapDrvCtx->IdftInTrCtrl[2].TrId = 0;

    // set up IDFT FPDMA Data descriptor: Output
    pMapDrvCtx->IdftOutTrCtrl.FpId = 0;
    pMapDrvCtx->IdftOutTrCtrl.LoadType = FP_DRV_LOAD_TYPE1;
    pMapDrvCtx->IdftOutTrCtrl.LoadCont = 0;
    pMapDrvCtx->IdftOutTrCtrl.StartBank = 1<<3;
    pMapDrvCtx->IdftOutTrCtrl.SegStart = 0;
    pMapDrvCtx->IdftOutTrCtrl.BusSize = 1;
    pMapDrvCtx->IdftOutTrCtrl.DataType = 0;
    pMapDrvCtx->IdftOutTrCtrl.TrInt = 0;
    pMapDrvCtx->IdftOutTrCtrl.StartAddr = 0;
    pMapDrvCtx->IdftOutTrCtrl.SegCount = 0;
    pMapDrvCtx->IdftOutTrCtrl.TrId = 0;

    appHeapAllocSize = (FP_DRV_MAX_NUM_USERS_PER_TTI+FP_DRV_MAX_NUM_USERS_PER_TTI+FP_DRV_MAX_NUM_USERS_PER_TTI)*sizeof(UINT16)*MAX_IDFT_TRANSACTIONS_PER_SYMBOL;
    if (pMapDrvCtx->IdftAppHeapAlloc == NULL)
        pMapDrvCtx->IdftAppHeapAlloc = MxHeapAlloc(AppHeap, appHeapAllocSize);
    pAppHeap = pMapDrvCtx->IdftAppHeapAlloc;

    dspHeapAllocSize = ((sizeof(MAP_DRV_IDFT_CONFIG)*FP_DRV_MAX_NUM_USERS_PER_TTI + 8) + (sizeof(MAP_DRV_IDFT_MULT_TABLE)*FP_DRV_MAX_NUM_USERS_PER_TTI))*2*MAX_IDFT_TRANSACTIONS_PER_SYMBOL;
    if (pMapDrvCtx->IdftCramHeapAlloc == NULL)
        pMapDrvCtx->IdftCramHeapAlloc = MxHeapAlloc(DspHeap, dspHeapAllocSize);
    pCramHeap = pMapDrvCtx->IdftCramHeapAlloc;

    // Initialize
    for (j = 0; j < MAX_IDFT_TRANSACTIONS_PER_SYMBOL; j++)
    {
        pMapDrvCtx->pIdftSizesInDesc[j]    = (PUINT16)pAppHeap;
        pAppHeap += sizeof(UINT16)*FP_DRV_MAX_NUM_USERS_PER_TTI;
        pMapDrvCtx->pIdftInpLocInDesc[j]   = (PUINT16)pAppHeap;
        pAppHeap += sizeof(UINT16)*FP_DRV_MAX_NUM_USERS_PER_TTI;
        pMapDrvCtx->pIdftOutLocInDesc[j]   = (PUINT16)pAppHeap;
        pAppHeap += sizeof(UINT16)*FP_DRV_MAX_NUM_USERS_PER_TTI;

        memset(pMapDrvCtx->pIdftSizesInDesc[j],  0, sizeof(UINT16)*FP_DRV_MAX_NUM_USERS_PER_TTI);
        memset(pMapDrvCtx->pIdftInpLocInDesc[j], 0, sizeof(UINT16)*FP_DRV_MAX_NUM_USERS_PER_TTI);
        memset(pMapDrvCtx->pIdftOutLocInDesc[j], 0, sizeof(UINT16)*FP_DRV_MAX_NUM_USERS_PER_TTI);

        for (i = 0; i < 2; i++)
        {
            pMapDrvCtx->ppConfigTables[i][j] = pCramHeap;
            pCramHeap += sizeof(MAP_DRV_IDFT_CONFIG)*FP_DRV_MAX_NUM_USERS_PER_TTI + 8; // 8 for 2 Loop counters
            pMapDrvCtx->ppMultTables[i][j]   = pCramHeap;
            pCramHeap += sizeof(MAP_DRV_IDFT_MULT_TABLE)*FP_DRV_MAX_NUM_USERS_PER_TTI;

            memset(pMapDrvCtx->ppConfigTables[i][j], 0, sizeof(MAP_DRV_IDFT_CONFIG)*FP_DRV_MAX_NUM_USERS_PER_TTI + 8);
            memset(pMapDrvCtx->ppMultTables[i][j], 0, sizeof(MAP_DRV_IDFT_MULT_TABLE)*FP_DRV_MAX_NUM_USERS_PER_TTI);
        }
    }

    for (j = 0; j < 2; j++)                     // 2 Contexts
    {
        pIdftFDescHead = pMapDrvCtx->pIdftDmaDesc + (j * IDFT_DRV_QSIZE * MAX_NUM_IDFT_DMA_DESC);
        for (i = 0; i < IDFT_DRV_QSIZE; i++)
        {
            pInFDesc = pIdftFDescHead + i*MAX_NUM_IDFT_DMA_DESC;                // Desc 0
            pConfigFDesc = pInFDesc + 1;                                        // Desc 1
            pMultTableFDesc = pInFDesc + 2;                                     // Desc 2

            pInFDesc->NextFDesc = pConfigFDesc;
            for (k = 0; k < MAX_IDFT_TRANSACTIONS_PER_SYMBOL; k++)
            {
                pConfigFDesc->NextFDesc = pMultTableFDesc;
                pConfigFDesc += 2;
                pMultTableFDesc += 2;
            }
        }
    }

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief This function initializes the MAPs (to perform IDFT) with static tables in Bank7
 *
 *
 *  @param   FpId [in] ID of the MAP that the tables would be loaded into.
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIdftDrvInitLoadBigTablesToB7(UINT32 FpId)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    PDMAFDESC pIdftInDmaDesc, pIdftOutDmaDesc;
    PMAP_DMA_TR_CTRL pIdftInTrCtrl, pIdftOutTrCtrl;
    UINT32 LoadTime;
    MXRC rc = MX_OK;

    PUINT8 localCompare;

    _ASSERT_PTR(pMapDrvCtx);

    // Memory Allocation
    //-----------------
    // DMA Descriptors
    pIdftInDmaDesc = MxHeapAlloc(DspHeap, sizeof(DMAFDESC)*2);
    _ASSERT_PTR(pIdftInDmaDesc);
    memset(pIdftInDmaDesc, 0, sizeof(DMAFDESC));

    pIdftOutDmaDesc = pIdftInDmaDesc + 1;
    memset(pIdftOutDmaDesc, 0, sizeof(DMAFDESC));

    // TR Control
    pIdftInTrCtrl = MxHeapAlloc(DspHeap, sizeof(MAP_DMA_TR_CTRL)*2);
    _ASSERT_PTR(pIdftInTrCtrl);
    memset(pIdftInTrCtrl, 0, sizeof(MAP_DMA_TR_CTRL));

    pIdftOutTrCtrl = pIdftInTrCtrl + 1;
    memset(pIdftOutTrCtrl, 0, sizeof(MAP_DMA_TR_CTRL));

    // Output of copy
    localCompare = MxHeapAlloc(DspHeap, fp_idft_big_config_size);
    _ASSERT_PTR(localCompare);
    memset(localCompare, 0xBC, fp_idft_big_config_size);


    // set up IDFT FPDMA Data descriptor: Input Data
    pIdftInTrCtrl->FpId = FpId;
    pIdftInTrCtrl->LoadType = FP_DRV_LOAD_TYPE1;
    pIdftInTrCtrl->LoadCont = 0;
    pIdftInTrCtrl->StartBank = 1<<0;
    pIdftInTrCtrl->SegStart = 0;
    pIdftInTrCtrl->BusSize = 1;
    pIdftInTrCtrl->DataType = 0;
    pIdftInTrCtrl->TrInt = 2;
    pIdftInTrCtrl->StartAddr = 0;
    pIdftInTrCtrl->SegCount = 0;

    // set up IDFT FPDMA Data descriptor: Output
    pIdftOutTrCtrl->FpId = FpId;
    pIdftOutTrCtrl->LoadType = FP_DRV_LOAD_TYPE1;
    pIdftOutTrCtrl->LoadCont = 0;
    pIdftOutTrCtrl->StartBank = 1<<7;
    pIdftOutTrCtrl->SegStart = 0;
    pIdftOutTrCtrl->BusSize = 1;
    pIdftOutTrCtrl->DataType = 0;
    pIdftOutTrCtrl->TrInt = 3;
    pIdftOutTrCtrl->StartAddr = 0;
    pIdftOutTrCtrl->SegCount = 0;

    // set up IDFT FPDMA Data descriptor: Input Data
    pIdftInDmaDesc->NextFDesc = NULL;
    pIdftInDmaDesc->FControl = 0;
    *(PMAP_DMA_TR_CTRL)&pIdftInDmaDesc->FStatus0 = *pIdftInTrCtrl;
    pIdftInDmaDesc->OutBDesc.BCtrl = BCTRL_BLAST_MASK | fp_idft_big_config_size;
    pIdftInDmaDesc->OutBDesc.BPtr = (UINT32)fp_idft_big_config;

    // set up IDFT FPDMA Data descriptor: Output
    pIdftOutDmaDesc->NextFDesc = NULL;
    pIdftOutDmaDesc->FControl = 0;
    *(PMAP_DMA_TR_CTRL)&pIdftOutDmaDesc->FStatus0 = *pIdftOutTrCtrl;
    pIdftOutDmaDesc->OutBDesc.BCtrl = BCTRL_BLAST_MASK | (fp_idft_big_config_size-4); // 4: remove the loop counter
    pIdftOutDmaDesc->OutBDesc.BPtr = (UINT32)localCompare;

    pMapDrvCtx->IdftRun = 0xFFFF;

    MapDispatchFexp(FpId, pIdftInDmaDesc, pIdftOutDmaDesc);

    if (1)
    {
        IRQTYPE t = ARM_INT_disable();
        MxDelayTicks (TICKS_TO_MS(4));
        ARM_INT_restore (t);
    }

    LoadTime = MxGetTicks ();

    while(pMapDrvCtx->IdftRun == (UINT16)0xFFFF)
    {
        _NOP();
        _NOP();
        _NOP();
        _NOP();

        MxDelayTicks (TICKS_TO_MS(1));

        if (MxGetTicks () - LoadTime > MS_TO_TICKS(1000))
        {
            rc = FP_DRV_RC_IDFT_LOAD_ERROR;
            break;
        }
    }

    // Free Allocated Memories
    //--------------------
    MxHeapFree(DspHeap, pIdftInDmaDesc);
    MxHeapFree(DspHeap, pIdftInTrCtrl);
    MxHeapFree(DspHeap, localCompare);

    return rc;
}



//-------------------------------------------------------------------------------------------
/** @brief This function lets the driver which MAPs (out of the pre-decided MAPs) to use for
 *         for the IDFT function
 *
 *
 *  @param   mask  So by default since MAPs 0-4 are set up for IDFT, this is 0x1F
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIdftDrvSetMapMask(UINT16 mask)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    SINT16 start = -1, stop = -1, i, calc;
    CEVA_DRV_CFG * pCevaCfg = DspDrvGetCevaDrvCfgParams();

    _ASSERT_PTR(pCevaCfg);

    uart_printf("MapIdftDrvSetMapMask: Setting Mask: 0x%04x\n", mask);

    MapStoredIdftMask = mask;

    // IDFT
    pMapDrvCtx->IdftFpMask = mask;

    for (i = 0; i < pMapCtx->FpDevNum; i++)
    {
        calc = 1<<i;
        if ((calc & mask) != 0)
        {
            start = (start == -1)?i:start;
            stop = i;
        }
    }

    pMapDrvCtx->IdftFpStart = pMapDrvCtx->IdftCurFp = start;
    pMapDrvCtx->IdftFpStop = stop;

    // to provide information to CEVA FP driver

    pCevaCfg->FpCfg.IdftStart = start;
    pCevaCfg->FpCfg.IdftStop  = stop;
    pCevaCfg->FpCfg.FpDevNum  = pMapCtx->FpDevNum;

    return MX_OK;
}



//-------------------------------------------------------------------------------------------
/** @brief This function setups a call back function to each IDFT call to the driver.
 *
 *
 *  @param   CbProc  Pointer to the CallBack function
 *  @param   CbCtx   Callback parameter
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIdftDrvSetCallback(MAPDONECB CbProc, LPVOID CbCtx)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    pMapDrvCtx->IdftPostProcCb = CbProc;
    pMapDrvCtx->IdftPostProcParam = CbCtx;

    return MX_OK;
}



//-------------------------------------------------------------------------------------------
/** @brief This function sets the MAP number to be used for the next IDFT function call
 *
 *
 *  @param   fpNum  Number of the MAP to be used: 0-4
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIdftDrvSetFpNum(UINT32 fpNum)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    pMapDrvCtx->IdftCurFp = fpNum;

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function registers The IDFT Input locations (in DDR / CRAM) with the IDFT Driver
 *         These locations are copied into the IDFT hardware descriptors so that the MAPDMA will
 *         copy from these locations into internal MAP memory for processing
 *
 *
 *  @param   pInBuf  Pointer to Input buffer
 *  @param   SymNum    Symbol Number
 *  @param   InputSize    Input Size in bytes
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIdftDrvSetInBuf(PUINT8 pInBuf, UINT32 SymNum, UINT32 InputSize)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    volatile PDMAFDESC pIdftFDescHead, pInFDesc;
    UINT32 QueueElement;

    if (SymNum > IDFT_DRV_QSIZE)
    {
        stop_printf("Out of Bounds: pInBuf: %x, SymNum: %d, Inputsize: %d\r\n", pInBuf, SymNum, InputSize);
        return FP_DRV_RC_IDFT_INOUTERR;
    }

    for (QueueElement = 0; QueueElement < 2; QueueElement++)
    {
        // Set Head to the correct context
        pIdftFDescHead = pMapDrvCtx->pIdftDmaDesc + (QueueElement * IDFT_DRV_QSIZE * MAX_NUM_IDFT_DMA_DESC);

        // Setup Descriptors
        pInFDesc = pIdftFDescHead + SymNum*MAX_NUM_IDFT_DMA_DESC;                           // Input Descriptor

        pInFDesc->OutBDesc.BCtrl = BCTRL_BLAST_MASK | InputSize;
        pInFDesc->OutBDesc.BPtr = (UINT32)pInBuf;
    }

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function registers The IDFT Output locations (in DDR / CRAM) with the IDFT Driver
 *         These locations are copied into the IDFT hardware descriptors so that the MAPDMA will
 *         copy from these locations into internal MAP memory for processing
 *
 *
 *  @param   pOutBuf  Pointer to Input buffer
 *  @param   SymNum    Symbol Number
 *  @param   OutputSize    Output Size in bytes
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIdftDrvSetOutBuf(PUINT8 pOutBuf, UINT32 SymNum, UINT32 OutputSize)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    volatile PDMAFDESC pIdftFDescHead, pInFDesc, pOutFDesc;
    UINT32 QueueElement, j;

    if (SymNum > IDFT_DRV_QSIZE)
    {
        stop_printf("Out of Bounds: pOutBuf: %x, SymNum: %d, OutputSize: %d\r\n", pOutBuf, SymNum, OutputSize);
        return FP_DRV_RC_IDFT_INOUTERR;
    }

    for (QueueElement = 0; QueueElement < 2; QueueElement++)
    {
        // Set Head to the correct context
        pIdftFDescHead = pMapDrvCtx->pIdftDmaDesc + (QueueElement * IDFT_DRV_QSIZE * MAX_NUM_IDFT_DMA_DESC);

        // Setup Descriptors
        pInFDesc = pIdftFDescHead + SymNum*MAX_NUM_IDFT_DMA_DESC;      // Input Descriptor
        pOutFDesc = pInFDesc + NUM_IDFT_OUTPUT_DESC_START;             // Last Output Descs

        for(j = 0; j < NUM_IDFT_OUTPUT_DMA_DESC; j++)
        {
            pOutFDesc->OutBDesc.BCtrl = BCTRL_BLAST_MASK | (2*sizeof(short)*2);   // Dummy Output
            pOutFDesc->OutBDesc.BPtr = (UINT32)pOutBuf;

            pOutFDesc++;
        }
    }

    pMapDrvCtx->IdftOutputSize = OutputSize;

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief This function queue's up the IDFT TCB to the hardware queue so that the MAPDMA can
 *         dispatch the task when the particular MAP is free
 *
 *
 *  @param   pTcb  Pointer to the TCB to be dispatched
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIdftQueue(PTCB pTcb, UINT32 SymMask)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    PMAP_DRV_IDFT_MULT_TABLE pMultTableStruct;
    PUINT8 pIdftMultTable[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    UINT32 MultTableSize[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    volatile PDMAFDESC pIdftFDescHead, pInFDesc, pOutFDesc, pOutFDescLast, pConfigFDesc, pMultTableFDesc, pFDesc = NULL, pFDesc1 = NULL;
    UINT32 SymNum, CurrMask, FpId, ResourceIndex;
    UINT32 i;
    UINT32 QueueElement;
    UINT32 NumDesc;
    IRQTYPE irq;

    pTcb->ExecTicks = 0;

    SymNum = 0;
    ResourceIndex = 0;

    // Disable ARM IRQs
    irq = ARM_INT_disable();

    QueueElement = pMapDrvCtx->IdftQueueToRun;
    pMapDrvCtx->IdftQueueToRun = 1 - pMapDrvCtx->IdftQueueToRun;

    NumDesc = pMapDrvCtx->NumDesc[QueueElement];

    for (i=0; i<NumDesc; i++)     // Allocate max IOBuffers. This will be updated later
    {
        pMultTableStruct = (PMAP_DRV_IDFT_MULT_TABLE)pMapDrvCtx->ppMultTables[QueueElement][i];
        pIdftMultTable[i] = pMultTableStruct->TableDMALoc;
        MultTableSize[i] = pMultTableStruct->TableSize << 2;
    }

    // Set Head to the correct context
    pIdftFDescHead = pMapDrvCtx->pIdftDmaDesc + (QueueElement * IDFT_DRV_QSIZE * MAX_NUM_IDFT_DMA_DESC);

    // Build a list if something is already runnin on the MAPs
    while(SymMask != 0)
    {
        CurrMask = 1<<SymNum;
        if (CurrMask & SymMask)
        {
            SymMask &= ~CurrMask;

            FpId = pMapDrvCtx->IdftCurFp++;
            if (pMapDrvCtx->IdftCurFp > pMapDrvCtx->IdftFpStop)
                pMapDrvCtx->IdftCurFp = pMapDrvCtx->IdftFpStart;

            ResourceIndex |= 1<<FpId;

            // Setup Descriptors based on TCB input
            pInFDesc = pIdftFDescHead + SymNum*MAX_NUM_IDFT_DMA_DESC;           // Desc 0
            pConfigFDesc = pInFDesc + 1;                                        // Desc 1
            pMultTableFDesc = pInFDesc + 2;                                     // Desc 2
            pOutFDesc = pInFDesc + NUM_IDFT_OUTPUT_DESC_START;                  // First Output Desc

            pOutFDescLast = pOutFDesc;

            // Input Data: Input
            pInFDesc->FControl = 0;
            *(PMAP_DMA_TR_CTRL)&(pInFDesc->FStatus0) = pMapDrvCtx->IdftInTrCtrl[0];
            pInFDesc->FStatus0 |= FpId;

            for (i = 0; i < NumDesc; i++)
            {
                // Chain descriptors together
                if (i > 0)
                {
                    pFDesc->NextFDesc = pConfigFDesc;           // Mult Descriptor
                    pFDesc1->NextFDesc = pOutFDescLast;         // Output Descriptor
                    pFDesc1->OutBDesc.BCtrl = BCTRL_BLAST_MASK | (2*sizeof(short)*2);   // Dummy Output
                }

                // Input Data: Config
                pConfigFDesc->FControl = 0;
                pConfigFDesc->OutBDesc.BCtrl = BCTRL_BLAST_MASK | pMapDrvCtx->ConfigTablesSize[QueueElement][i];
                pConfigFDesc->OutBDesc.BPtr = (UINT32)pMapDrvCtx->ppConfigTables[QueueElement][i];
                *(PMAP_DMA_TR_CTRL)&(pConfigFDesc->FStatus0) = pMapDrvCtx->IdftInTrCtrl[1];
                pConfigFDesc->FStatus0 |= FpId;

                // Input Data: Mult Tables
                pMultTableFDesc->FControl = 0;
                pMultTableFDesc->NextFDesc = NULL;
                pMultTableFDesc->OutBDesc.BCtrl = BCTRL_BLAST_MASK | MultTableSize[i];
                pMultTableFDesc->OutBDesc.BPtr = (UINT32)pIdftMultTable[i];
                *(PMAP_DMA_TR_CTRL)&(pMultTableFDesc->FStatus0) = pMapDrvCtx->IdftInTrCtrl[2];
                pMultTableFDesc->FStatus0 |= FpId;

                // Output Data: Output
                pOutFDescLast->FControl = 0;
                pOutFDescLast->NextFDesc = NULL;
                *(PMAP_DMA_TR_CTRL)&pOutFDescLast->FStatus0 = pMapDrvCtx->IdftOutTrCtrl;
                pOutFDescLast->FStatus0 |= FpId;
                pOutFDescLast->OutBDesc.BCtrl = BCTRL_BLAST_MASK | pMapDrvCtx->IdftOutputSize;

                // Save Descriptors
                pFDesc = pMultTableFDesc;
                pFDesc1 = pOutFDescLast;

                // Increment Descriptors
                pConfigFDesc = pConfigFDesc + 2;
                pMultTableFDesc = pMultTableFDesc + 2;
                pOutFDescLast = pOutFDescLast + 1;
            }

            // If a list does not exist yet and this tcb is the first one in the list
            if (pMapDrvCtx->pReadyListHead[QueueElement][FpId] == NULL)
            {
                pMapDrvCtx->pReadyListHead[QueueElement][FpId] = pMapDrvCtx->pReadyListTail[QueueElement][FpId] = pInFDesc;
            }

            // If list exists already and this tcb is to be appeneded
            else
            {
                // Set Input
                pFDesc = pMapDrvCtx->pReadyListTail[QueueElement][FpId];
                while (pFDesc->NextFDesc)
                    pFDesc = pFDesc->NextFDesc;
                pFDesc->NextFDesc = pInFDesc;

                // Set Output
                pFDesc = pMapDrvCtx->pReadyListTail[QueueElement][FpId] + NUM_IDFT_OUTPUT_DESC_START;
                while (pFDesc->NextFDesc)
                    pFDesc = pFDesc->NextFDesc;
                pFDesc->NextFDesc = pOutFDesc;

                pMapDrvCtx->pReadyListTail[QueueElement][FpId] = pInFDesc;
            }
        }
        SymNum++;
    }

    pTcb->ResourceIndex = ResourceIndex;

    // Enable ARM IRQs
    ARM_INT_restore(irq);

    return MX_OK;
}

MXRC MapIdftUseQueueFromTcb(UINT32 useTcbQueue)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    _ASSERT_PTR(pMapDrvCtx);

    pMapDrvCtx->IdftUseTcbQueue = useTcbQueue;

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function run's the queued up the IDFT TCBs to the hardware queue so that the MAPDMA can
 *         dispatch the task when the particular MAP is free
 *
 *
 *  @param   pTcb  Pointer to the TCB to be dispatched
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIdftDrvRun(PTCB pTcb)
{
    UINT32 t = MxGetTicks ();
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    PDMAFDESC *pTcbReadyListHead = NULL;
    PDMAFDESC *pTcbReadyListTail = NULL;

    volatile PDMAFDESC pInFDesc, pOutFDesc, pFDesc;
    UINT32 CurrMask, FpId, ResourceIndex, QueueElement;
    IRQTYPE irq;

    // Case should never occur
    if (pMapDrvCtx->IdftRun != 0)
    {
        stop_printf("IDFT MAPs Busy!!! Cannot run another TCB until previous one finished\r\n");
        return FP_DRV_RC_RUN_ERROR;
    }

    // Dispatch the list to each MAP
    pMapDrvCtx->IdftTcb = pTcb;

    if (pMapDrvCtx->IdftUseTcbQueue)
    {
        pTcbReadyListHead = (PDMAFDESC*) extractTCBIOBuf(pTcb, 0);
        pTcbReadyListTail = (PDMAFDESC*) extractTCBIOBuf(pTcb, 1);
    }

    // Disable ARM IRQs
    irq = ARM_INT_disable();

    ResourceIndex = pTcb->ResourceIndex;

    pTcb->ResourceIndex = 0x1F0;
    MLogEXEStart(pTcb, 0, t);
    pTcb->ExecTicks = GetTIMETICK();

    QueueElement = pMapDrvCtx->IdftQueueRun;
    pMapDrvCtx->IdftQueueRun = 1 - pMapDrvCtx->IdftQueueRun;

    //ResourceIndex = pTcb->ResourceIndex;
    FpId = pMapDrvCtx->IdftFpStart;
    while(ResourceIndex != 0)
    {
        CurrMask = (1<<FpId);
        if (CurrMask & ResourceIndex)
        {
            ResourceIndex &= ~CurrMask;
            pMapDrvCtx->IdftRun |= 1<<FpId;

            // Set Output IRQ for last descriptor
            if (pMapDrvCtx->IdftUseTcbQueue)
                pFDesc = pTcbReadyListTail[FpId] + NUM_IDFT_OUTPUT_DESC_START;
            else
                pFDesc = pMapDrvCtx->pReadyListTail[QueueElement][FpId] + NUM_IDFT_OUTPUT_DESC_START;

            while (pFDesc->NextFDesc)
                pFDesc = pFDesc->NextFDesc;
            pFDesc->FStatus0 |= 3<<20; // enable interrupt

            if (pMapDrvCtx->IdftUseTcbQueue)
                pInFDesc = pTcbReadyListHead[FpId];
            else
                pInFDesc = pMapDrvCtx->pReadyListHead[QueueElement][FpId];
            pOutFDesc = pInFDesc + NUM_IDFT_OUTPUT_DESC_START;

            MapDispatchFexp(FpId, pInFDesc, pOutFDesc);

            if (pMapDrvCtx->IdftUseTcbQueue == 0)
                pMapDrvCtx->pReadyListHead[QueueElement][FpId] = pMapDrvCtx->pReadyListTail[QueueElement][FpId] = NULL;
        }
        FpId++;
    }

    // Enable ARM IRQs
    ARM_INT_restore(irq);

    return MX_OK;

}


//-------------------------------------------------------------------------------------------
/** @brief This function is used to generate multi-user UL descriptors that will be used by MAP driver to create DMA descriptors
 *              and also will be used by MAP to process IDFT on a symbol basis. The Input to this function is a stream of IDFT PUSCH sizes
 *              generated by PHY code
 *
 *  @param   pIdftSizeArr         [in]  Stream of IDFT Sizes (PUSCH) that will be concatenated back to back as input to IDFT
 *  @param   ArrSize                [in]  Number of PUSCH Channels / Size of Array pIdftSizeArr
 *  @param   pNumDesc          [out] Number of DMA descriptors needed to perform IDFT on this symbol
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIdftGenerateTables(PUINT16 pIdftSizeArr, UINT32 ArrSize)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    UINT16 i, j, totalInputLoc, totalOutputLoc, IdftSizeIndex, NumIdftSizeInDesc, QueueElement;
    PMAP_DRV_IDFT_MULT_TABLE pMultTableStruct;
    PMAP_DRV_IDFT_CONFIG pConfigTableStruct;
    PUINT16 pConfigTableSize;

    if (ArrSize > FP_DRV_MAX_NUM_USERS_PER_TTI)
    {
        stop_printf("ArrSize [%d] > FP_DRV_MAX_NUM_USERS_PER_TTI [%d]\r\n", ArrSize, FP_DRV_MAX_NUM_USERS_PER_TTI);
        return FP_DRV_RC_RUN_ERROR;
    }

    // Initialize
    QueueElement = pMapDrvCtx->IdftQueueToRun;
    for (j = 0; j < MAX_IDFT_TRANSACTIONS_PER_SYMBOL; j++)
    {
        memset(pMapDrvCtx->pIdftSizesInDesc[j],  0, sizeof(UINT16)*ArrSize);
        memset(pMapDrvCtx->pIdftInpLocInDesc[j], 0, sizeof(UINT16)*ArrSize);
        memset(pMapDrvCtx->pIdftOutLocInDesc[j], 0, sizeof(UINT16)*ArrSize);
    }
    memset(pMapDrvCtx->MaxIdftSizeInDesc, 0, MAX_IDFT_TRANSACTIONS_PER_SYMBOL*sizeof(UINT16));
    memset(pMapDrvCtx->NumIdftSizeInDesc, 0, MAX_IDFT_TRANSACTIONS_PER_SYMBOL*sizeof(UINT16));
    pMapDrvCtx->NumDesc[QueueElement] = 0;


    // Calculate Number of Descriptors needed to perform IDFT on this symbol
    totalInputLoc = 0;
    totalOutputLoc = 0;
    for (i = 0; i < ArrSize; i++)
    {
        for (j = 0; j < MAX_IDFT_TRANSACTIONS_PER_SYMBOL; j++)
        {
            if (pMapDrvCtx->MaxIdftSizeInDesc[j])
            {
                if (pMapDrvCtx->MaxIdftSizeInDesc[j] <= MAX_IDFT_SIZE_SMALLER_TRANSACTION)
                {
                    if (pIdftSizeArr[i] <= MAX_IDFT_SIZE_SMALLER_TRANSACTION)
                    {
                        if (pMapDrvCtx->MaxIdftSizeInDesc[j] < pIdftSizeArr[i])
                            pMapDrvCtx->MaxIdftSizeInDesc[j] = pIdftSizeArr[i];

                        NumIdftSizeInDesc = pMapDrvCtx->NumIdftSizeInDesc[j]++;
                        pMapDrvCtx->pIdftSizesInDesc[j][NumIdftSizeInDesc] = pIdftSizeArr[i];
                        pMapDrvCtx->pIdftInpLocInDesc[j][NumIdftSizeInDesc] = totalInputLoc;
                        pMapDrvCtx->pIdftOutLocInDesc[j][NumIdftSizeInDesc] = totalOutputLoc;
                        break;
                    }
                }
                else
                {
                    if (pIdftSizeArr[i] == pMapDrvCtx->MaxIdftSizeInDesc[j])
                    {
                        NumIdftSizeInDesc = pMapDrvCtx->NumIdftSizeInDesc[j]++;
                        pMapDrvCtx->pIdftSizesInDesc[j][NumIdftSizeInDesc] = pIdftSizeArr[i];
                        pMapDrvCtx->pIdftInpLocInDesc[j][NumIdftSizeInDesc] = totalInputLoc;
                        pMapDrvCtx->pIdftOutLocInDesc[j][NumIdftSizeInDesc] = totalOutputLoc;
                        break;
                    }
                }
            }
            else
            {
                pMapDrvCtx->MaxIdftSizeInDesc[j] = pIdftSizeArr[i];

                NumIdftSizeInDesc = pMapDrvCtx->NumIdftSizeInDesc[j]++;
                pMapDrvCtx->pIdftSizesInDesc[j][NumIdftSizeInDesc] = pIdftSizeArr[i];
                pMapDrvCtx->pIdftInpLocInDesc[j][NumIdftSizeInDesc] = totalInputLoc;
                pMapDrvCtx->pIdftOutLocInDesc[j][NumIdftSizeInDesc] = totalOutputLoc;
                break;
            }
        }
        totalInputLoc += (pIdftSizeArr[i] + 2);     // 2 because of exponent in output will be written here
        totalOutputLoc += (pIdftSizeArr[i] + 2);    // 2 because of exponent

        if (pMapDrvCtx->NumDesc[QueueElement] < j)
            pMapDrvCtx->NumDesc[QueueElement] = j;
    }

    pMapDrvCtx->NumDesc[QueueElement]++;

    for (i = 0; i < pMapDrvCtx->NumDesc[QueueElement]; i++)
    {
        pMapDrvCtx->ConfigTablesSize[QueueElement][i] = sizeof(MAP_DRV_IDFT_CONFIG)*pMapDrvCtx->NumIdftSizeInDesc[i] + 8;
        memset(pMapDrvCtx->ppConfigTables[QueueElement][i], 0, sizeof(MAP_DRV_IDFT_CONFIG)*pMapDrvCtx->NumIdftSizeInDesc[i] + 8);
        memset(pMapDrvCtx->ppMultTables[QueueElement][i], 0, sizeof(MAP_DRV_IDFT_MULT_TABLE)*pMapDrvCtx->NumIdftSizeInDesc[i]);
    }

    // Initialize all tables to correct values
    for (i = 0; i < pMapDrvCtx->NumDesc[QueueElement]; i++)
    {
        // ppConfigTables
        pConfigTableSize = (PUINT16)pMapDrvCtx->ppConfigTables[QueueElement][i];
        pConfigTableSize[0] = (pMapDrvCtx->NumIdftSizeInDesc[i] * sizeof(MAP_DRV_IDFT_CONFIG)>>2) + 1; // Number of copies to be done from Bank 0 and Bank 6
        pConfigTableSize[2] = pMapDrvCtx->NumIdftSizeInDesc[i]; // Number of loops to do on Bank 6
        pConfigTableStruct = (PMAP_DRV_IDFT_CONFIG)&pConfigTableSize[4];

        // ppMultTables
        pMultTableStruct = (PMAP_DRV_IDFT_MULT_TABLE)pMapDrvCtx->ppMultTables[QueueElement][i];

        for (j = 0; j < pMapDrvCtx->NumIdftSizeInDesc[i]; j++)
        {
            IdftSizeIndex = fp_idft_size_index_map[pMapDrvCtx->pIdftSizesInDesc[i][j]];
            pConfigTableStruct[j].InputLoc[0] = 512 + pMapDrvCtx->pIdftInpLocInDesc[i][j];  // 512 because we copy input to location 512
            pConfigTableStruct[j].OutputLoc[0] = pMapDrvCtx->pIdftOutLocInDesc[i][j];
            pConfigTableStruct[j].ConfigLoc[0] = fp_idft_big_config_start[IdftSizeIndex];
            pConfigTableStruct[j].MultTableLoc[0] = fp_idft_mult_table_start[IdftSizeIndex];
        }

        IdftSizeIndex = fp_idft_size_index_map[pMapDrvCtx->MaxIdftSizeInDesc[i]];
        pMultTableStruct->TableDMALoc = (PUINT8)fp_idft_tables[IdftSizeIndex];
        pMultTableStruct->TableSize = fp_idft_table_sizes_with_offset[IdftSizeIndex];

    }

    return MX_OK;
}


//-------------------------------------------------------------------------------------------------------------------------------------






//-------------------------------------------------------------------------------------------------------------------------------------
// IFFT Functions
//-------------



//-------------------------------------------------------------------------------------------
/** @brief This function initializes the IFFT driver variables
 *
 *
 *  @param   IfftExp This is the IFFT size to be used. IfftExp is the exponent value for the
 *                   IfftSize. For example, if the IFFT size is 1024, IfftSize should be 10
 *                   2^10 = 1024
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIfftDrvInit(UINT32 IfftExp)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    PDMAFDESC pFpDmaDesc;
    UINT32 i, ifftSize, inputSize = 0, cp1 = 0, cp2 = 0, symNum;

    if (pMapDrvCtx->pIfftDmaDesc == NULL)
    {
        pMapDrvCtx->pIfftDmaDesc = MxHeapAlloc(DspHeap, 2*IFFT_DRV_QSIZE*(2*sizeof(DMAFDESC)));
        _ASSERT_PTR(pMapDrvCtx->pIfftDmaDesc);

        if (pMapDrvCtx->pIfftDmaDesc == NULL)
            return FP_DRV_RC_ALLOC_ERROR;
    }

    memset(pMapDrvCtx->pIfftDmaDesc, 0, IFFT_DRV_QSIZE*2*sizeof(DMAFDESC));

    // Set parameters based on IFFT size
    ifftSize = 1 << IfftExp;
    if (ifftSize == 128)
    {
        inputSize = 72;
        cp1 = 10;
        cp2 = 9;
    }
    else if (ifftSize == 256)
    {
        inputSize = 180;
        cp1 = 20;
        cp2 = 18;
    }
    else if (ifftSize == 512)
    {
        inputSize = 300;
        cp1 = 40;
        cp2 = 36;
    }
    else if (ifftSize == 1024)
    {
        inputSize = 600;
        cp1 = 80;
        cp2 = 72;
    }
    else if (ifftSize == 2048)
    {
        inputSize = 1200;
        cp1 = 160;
        cp2 = 144;
    }
    else
    {
        return FP_DRV_RC_INVALID_FP_MODE;
    }

    pMapDrvCtx->IfftInpSize = inputSize;
    pMapDrvCtx->IfftRndInpSize = ROUND(inputSize, 32); // 608
    pMapDrvCtx->IfftOutSize = ifftSize;
    pMapDrvCtx->IfftCp1OutSize = cp1;
    pMapDrvCtx->IfftCp2OutSize = cp2;

    // set up IFFT FPDMA Data descriptor: Input Data
    pMapDrvCtx->IfftInTrCtrl[0].FpId = 7;
    pMapDrvCtx->IfftInTrCtrl[0].LoadType = FP_DRV_LOAD_TYPE0;
    pMapDrvCtx->IfftInTrCtrl[0].LoadCont = 0;
    pMapDrvCtx->IfftInTrCtrl[0].StartBank = 1<<4;
    pMapDrvCtx->IfftInTrCtrl[0].SegStart = 0;
    pMapDrvCtx->IfftInTrCtrl[0].BusSize = 1;
    pMapDrvCtx->IfftInTrCtrl[0].DataType = 0;
    pMapDrvCtx->IfftInTrCtrl[0].TrInt = 4;             // Has to be set to 4 for CP0 and 2 for CP1
    pMapDrvCtx->IfftInTrCtrl[0].StartAddr = 0;
    pMapDrvCtx->IfftInTrCtrl[0].SegCount = 0;

    pMapDrvCtx->IfftInTrCtrl[1].FpId = 7;
    pMapDrvCtx->IfftInTrCtrl[1].LoadType = FP_DRV_LOAD_TYPE0;
    pMapDrvCtx->IfftInTrCtrl[1].LoadCont = 0;
    pMapDrvCtx->IfftInTrCtrl[1].StartBank = 1<<4;
    pMapDrvCtx->IfftInTrCtrl[1].SegStart = 0;
    pMapDrvCtx->IfftInTrCtrl[1].BusSize = 1;
    pMapDrvCtx->IfftInTrCtrl[1].DataType = 0;
    pMapDrvCtx->IfftInTrCtrl[1].TrInt = 2;             // Has to be set to 4 for CP0 and 2 for CP1
    pMapDrvCtx->IfftInTrCtrl[1].StartAddr = 0;
    pMapDrvCtx->IfftInTrCtrl[1].SegCount = 0;

    // set up IFFT FPDMA Data descriptor: Output
    pMapDrvCtx->IfftOutTrCtrl[0].FpId = 0;
    pMapDrvCtx->IfftOutTrCtrl[0].LoadType = FP_DRV_LOAD_TYPE0;
    pMapDrvCtx->IfftOutTrCtrl[0].LoadCont = 0;
    pMapDrvCtx->IfftOutTrCtrl[0].StartBank = 1<<6;
    pMapDrvCtx->IfftOutTrCtrl[0].SegStart = 0;
    pMapDrvCtx->IfftOutTrCtrl[0].BusSize = 1;
    pMapDrvCtx->IfftOutTrCtrl[0].DataType = 0;
    pMapDrvCtx->IfftOutTrCtrl[0].TrInt = 0;
    pMapDrvCtx->IfftOutTrCtrl[0].StartAddr = 0;
    pMapDrvCtx->IfftOutTrCtrl[0].SegCount = (ifftSize + cp1) / 2;     // (ifftSize +CP Size) / 2;

    pMapDrvCtx->IfftOutTrCtrl[1].FpId = 0;
    pMapDrvCtx->IfftOutTrCtrl[1].LoadType = FP_DRV_LOAD_TYPE0;
    pMapDrvCtx->IfftOutTrCtrl[1].LoadCont = 0;
    pMapDrvCtx->IfftOutTrCtrl[1].StartBank = 1<<6;
    pMapDrvCtx->IfftOutTrCtrl[1].SegStart = 0;
    pMapDrvCtx->IfftOutTrCtrl[1].BusSize = 1;
    pMapDrvCtx->IfftOutTrCtrl[1].DataType = 0;
    pMapDrvCtx->IfftOutTrCtrl[1].TrInt = 0;
    pMapDrvCtx->IfftOutTrCtrl[1].StartAddr = 0;
    pMapDrvCtx->IfftOutTrCtrl[1].SegCount = (ifftSize + cp2 + 1) / 2;     // (ifftSize +CP Size) / 2;       +1 is needed for size 128


    pFpDmaDesc = pMapDrvCtx->pIfftDmaDesc;
    for (i = 0; i < IFFT_DRV_QSIZE*2; i++)
    {
        symNum = i%14;
        // set up IFFT FPDMA Data descriptor: Input Data
        pFpDmaDesc->OutBDesc.BCtrl = BCTRL_BLAST_MASK | (pMapDrvCtx->IfftInpSize * 4);
        pFpDmaDesc->NextFDesc = NULL;
        pFpDmaDesc++;

        // set up IFFT FPDMA Data descriptor: Output
        if ((symNum==0)||(symNum==7))
            pFpDmaDesc->OutBDesc.BCtrl = BCTRL_BLAST_MASK | ((pMapDrvCtx->IfftOutSize + cp1) * 4);
        else
            pFpDmaDesc->OutBDesc.BCtrl = BCTRL_BLAST_MASK | ((pMapDrvCtx->IfftOutSize + cp2) * 4);
        pFpDmaDesc->FControl = 0; // disable interrupt
        pFpDmaDesc->FStatus0 = 0;
        pFpDmaDesc->FStatus1 = 0;
        pFpDmaDesc->NextFDesc = NULL;
        pFpDmaDesc++;

    }

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function lets the driver which MAPs (out of the pre-decided MAPs) to use for
 *         for the IFFT function
 *
 *
 *  @param   mask  So by default since MAPs 5-9 are set up for IFFT, this is 0x3E0
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIfftDrvSetMapMask(UINT16 mask)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    SINT16 start = -1, stop = -1, i, calc;

    uart_printf("MapIfftDrvSetMapMask: Setting Mask: 0x%04x\n", mask);

    MapStoredIfftMask = mask;

    // IFFT
    pMapDrvCtx->IfftFpMask = mask;

    for (i = 0; i < pMapCtx->FpDevNum; i++)
    {
        calc = 1<<i;
        if ((calc & mask) != 0)
        {
            start = (start == -1)?i:start;
            stop = i;
        }
    }

    pMapDrvCtx->IfftFpStart = pMapDrvCtx->IfftCurFp = start;
    pMapDrvCtx->IfftFpStop = stop;

    return MX_OK;
}



//-------------------------------------------------------------------------------------------
/** @brief This function setups a call back function to each IFFT call to the driver.
 *
 *
 *  @param   CbProc  Pointer to the CallBack function
 *  @param   CbCtx   Callback parameter
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIfftDrvSetCallback(MAPDONECB CbProc, LPVOID CbCtx)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    pMapDrvCtx->IfftPostProcCb = CbProc;
    pMapDrvCtx->IfftPostProcParam = CbCtx;

    return MX_OK;
}



//-------------------------------------------------------------------------------------------
/** @brief This function defines how many Tx antennas are there in the base-station. Based on
 *         this number, the IFFT driver will dispatch all antennas to be processed by the MAP
 *         in 1 shot. This helps utilizing the MAPDMA hardware dispatcher efficiently
 *
 *
 *  @param   nTxAnt  Number of Tx Antennas
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIfftDrvSetNumAntennas(UINT32 nTxAnt)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    UINT32 numAnt, numSym, SymStart, SymStop, arrIndex, currSlot, ctxNum;
    volatile PDMAFDESC pIfftDmaDescBase, pInFDesc, pOutFDesc;

    pMapDrvCtx->nTxAnt = nTxAnt;

    // Build Chain of descriptors in the List
    for (ctxNum = 0; ctxNum < 2; ctxNum++)
    {
        pIfftDmaDescBase = pMapDrvCtx->pIfftDmaDesc + ctxNum*IFFT_DRV_QSIZE*2;

        for (currSlot = 0; currSlot < pMapDrvCtx->nNumIfftSlot; currSlot++)
        {
            SymStart = currSlot * pMapDrvCtx->nSymIFFT;
            SymStop = SymStart + pMapDrvCtx->nSymIFFT;
            arrIndex = currSlot + 2;

            pMapDrvCtx->pReadyListFftIfftHead[ctxNum][arrIndex] = pMapDrvCtx->pReadyListFftIfftTail[ctxNum][arrIndex] = NULL;

            for (numSym = SymStart; numSym < SymStop; numSym++)
            {
                for (numAnt = 0; numAnt < nTxAnt; numAnt++)
                {
                    pInFDesc = pIfftDmaDescBase + (numSym*2) + (numAnt*14*2);
                    pOutFDesc = pInFDesc+1;

                    pInFDesc->NextFDesc = NULL;
                    pOutFDesc->NextFDesc = NULL;

                    // List building
                    //-----------
                    if (pMapDrvCtx->pReadyListFftIfftHead[ctxNum][arrIndex] == NULL)
                    {
                        pMapDrvCtx->pReadyListFftIfftHead[ctxNum][arrIndex] = pMapDrvCtx->pReadyListFftIfftTail[ctxNum][arrIndex] = pInFDesc;
                    }
                    else
                    {
                        pMapDrvCtx->pReadyListFftIfftTail[ctxNum][arrIndex]->NextFDesc = pInFDesc;       // Chain Input
                        (pMapDrvCtx->pReadyListFftIfftTail[ctxNum][arrIndex]+1)->NextFDesc = pOutFDesc;  // Chain Output

                        pMapDrvCtx->pReadyListFftIfftTail[ctxNum][arrIndex] = pInFDesc;
                    }
                }
            }
        }
    }

    return MX_OK;
}





//-------------------------------------------------------------------------------------------
/** @brief This function registers all IFFT Input locations (in DDR / CRAM) with the IFFT Driver
 *         These locations are copied into the IFFT hardware descriptors so that the MAPDMA will
 *         copy from these locations into internal MAP memory for processing
 *
 *
 *  @param   pInBuf  Pointer to Input buffer
 *  @param   Size    Number of symbols (across all antennas) to be processed
 *  @param   Start   Symbol start number.
 *
 *  This sets up each IFFT hardware descriptor with pointers to each symbol's input pointer. When we
 *  allocate memory for IFFT input, they are allocated in 1 big group (per antenna / all antennas).
 *  This driver function will use the IFFT Size to setup the pointers correctly
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIfftDrvSetInpBuf(PUINT32 pInBuf, UINT32 Size, UINT32 Start)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    PDMAFDESC pFpDmaDescBase = pMapDrvCtx->pIfftDmaDesc + pMapDrvCtx->QueueContext[IFFT_SLOT0]*IFFT_DRV_QSIZE*2;
    PDMAFDESC pFpDmaDesc = pFpDmaDescBase + (Start*2);
    UINT32 i;

    for (i = Start; i < Size+Start; i++)
    {
        // Input descriptor
        pFpDmaDesc->OutBDesc.BPtr = (UINT32)pInBuf;
        pInBuf += pMapDrvCtx->IfftRndInpSize;

        // skip Output descriptors
        pFpDmaDesc+=2;
    }

    return MX_OK;
}




//-------------------------------------------------------------------------------------------
/** @brief This function registers all IFFT Output locations (in DDR / CRAM) with the IFFT Driver
 *         These locations are copied into the IFFT hardware descriptors so that the MAPDMA will
 *         copy the output from MAP to these location after processing
 *
 *
 *  @param   pOutBuf Pointer to Output buffer
 *  @param   Size    Number of symbols (across all antennas) to be processed
 *  @param   Start   Symbol start number.
 *
 *  This sets up each IFFT hardware descriptor with pointers to each symbol's output pointer. When we
 *  allocate memory for IFFT output, they are allocated in 1 big group (per antenna / all antennas).
 *  This driver function will use the IFFT Size to setup the pointers correctly. It copies the CPs to
 *  to these locations correctly as well
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIfftDrvSetOutBuf(PUINT32 pOutBuf, UINT32 Size, UINT32 Start)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    PDMAFDESC pFpDmaDescBase = pMapDrvCtx->pIfftDmaDesc + pMapDrvCtx->QueueContext[IFFT_SLOT0]*IFFT_DRV_QSIZE*2;
    PDMAFDESC pFpDmaDesc = pFpDmaDescBase + (Start*2);
    UINT32 i, cp, symNum;

    for (i = Start; i < Size+Start; i++)
    {
        symNum = i%14;
        cp = ((symNum == 0 || symNum == 7) ? pMapDrvCtx->IfftCp1OutSize : pMapDrvCtx->IfftCp2OutSize);

        // skip input descriptor
        pFpDmaDesc++;

        // Output descriptor
        pFpDmaDesc->OutBDesc.BPtr = (UINT32)pOutBuf;
        pFpDmaDesc++;

        pOutBuf += (pMapDrvCtx->IfftOutSize + cp);
    }

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief This function sets the MAP number to be used for the next IFFT function call
 *
 *
 *  @param   fpNum  Number of the MAP to be used: 5-9
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIfftDrvSetFpNum(UINT32 fpNum)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    pMapDrvCtx->IfftCurFp = fpNum;

    return MX_OK;
}



//-------------------------------------------------------------------------------------------
/** @brief This function set's up the number of symbols to be processed by the IFFT driver. By
 *         default it is 14. This function is used for debugging the MAP code by running only 1
 *         symbol
 *
 *
 *  @param   numSym  Number of symbols to process
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIfftDrvSetNumSym(UINT32 numSym)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    pMapDrvCtx->nSymIFFT = numSym;

    // Special Case for PHY Code
    if (pMapDrvCtx->nSymIFFT == NUM_SYMBOLS)
        pMapDrvCtx->nNumIfftSlot = 1;

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function set's up the IFFT hardware descriptors in a chain with all symbols to be
 *         processed
 *
 *
 *  @param   void
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIfftDrvPreRun(LPVOID CbCtx)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    volatile PDMAFDESC pFpDmaDescBase;
    volatile PDMAFDESC pInFDesc, pOutFDesc;
    PTCB pTcb = (PTCB)CbCtx;

    UINT32 numAnt, numSym, currSlot, SymStart, SymStop;
    UINT32 arrIndex, ctxNum;

    currSlot = pTcb->isym;
    SymStart = currSlot * pMapDrvCtx->nSymIFFT;
    SymStop = SymStart + pMapDrvCtx->nSymIFFT;
    arrIndex = currSlot + 2;
    ctxNum = pMapDrvCtx->QueueContext[arrIndex];
    pFpDmaDescBase = pMapDrvCtx->pIfftDmaDesc + ctxNum*IFFT_DRV_QSIZE*2;

    for (numAnt = 0; numAnt < pMapDrvCtx->nTxAnt; numAnt++)
    {
        for (numSym = SymStart; numSym < SymStop; numSym++)
        {
            pInFDesc = pFpDmaDescBase + (numSym*2) + (numAnt*14*2);
            pOutFDesc = pInFDesc+1;

            // Input Data: Input
            pInFDesc->FControl = 0; // disable interrupt
            if ((numSym == 0) || (numSym == 7))
                *(PMAP_DMA_TR_CTRL)&pInFDesc->FStatus0 = pMapDrvCtx->IfftInTrCtrl[0];
            else
                *(PMAP_DMA_TR_CTRL)&pInFDesc->FStatus0 = pMapDrvCtx->IfftInTrCtrl[1];

            // Output Data: Output
            pOutFDesc->FControl = 0; // disable interrupt
            pOutFDesc->FStatus0 = 0;
            pOutFDesc->FStatus1 = 0;
        }
    }

    (pMapDrvCtx->pReadyListFftIfftTail[ctxNum][arrIndex] + 1)->FControl = 0x1;    // Set Interrupt
    pMapDrvCtx->UpdateQueueFftIfft[ctxNum][arrIndex] = 1;

    return MX_OK;
}



//-------------------------------------------------------------------------------------------
/** @brief This function set's starts the list to the MDMA engine, which disptaches it to the pool of
 *         MAPs assigned to run the IFFT
 *
 *
 *  @param   void
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapIfftDrvRun(LPVOID CbCtx)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    UINT32 currSlot, arrIndex, ctxNum;
    PTCB pTcb = (PTCB)CbCtx;
    IRQTYPE irq;

    currSlot = ((PTCB)CbCtx)->isym;
    arrIndex = currSlot + 2;
    ctxNum = pMapDrvCtx->QueueContext[arrIndex];
    pMapDrvCtx->QueueContext[arrIndex] = 1 - pMapDrvCtx->QueueContext[arrIndex];

    if (pMapDrvCtx->UpdateQueueFftIfft[ctxNum][arrIndex] == 0)
    {
        stop_printf("IFFT Run: Not Updated: Ctx: %d, arrIndex: %d\r\n", ctxNum, arrIndex);
        return FP_DRV_RC_RUN_ERROR;
    }

    // Disable all ARM interrupts
    irq = ARM_INT_disable();

    // Check to see if the Queue is full. If so, we need to stop
    if (pMapDrvCtx->QueueFftIfft[pMapDrvCtx->QueueLocationCurr] != NOT_RUNNING)
    {
        stop_printf("Condition should not occur: Queue Full [%d %d %d %d] %d!!!\r\n",
            pMapDrvCtx->QueueFftIfft[0], pMapDrvCtx->QueueFftIfft[1],
            pMapDrvCtx->QueueFftIfft[2], pMapDrvCtx->QueueFftIfft[3],
            pMapDrvCtx->QueueLocationCurr);

        // Enable all ARM interrupts
        ARM_INT_restore(irq);

        return FP_DRV_RC_RUN_ERROR;
    }

    // Store this context in variable. To be used for returing PTCB to Scheduler during MAP IRQ processing
    pMapDrvCtx->IfftCb[currSlot] = CbCtx;
    pTcb->ResourceIndex = 0x3E00;

    pMapDrvCtx->QueueFftIfft[pMapDrvCtx->QueueLocationCurr] = IFFT_SLOT0+currSlot;
    pMapDrvCtx->QueueLocationCurr++;
    if (pMapDrvCtx->QueueLocationCurr >= 4)
        pMapDrvCtx->QueueLocationCurr = 0;

    // If Map is not busy, just dispatch
    if (pMapDrvCtx->pool1FpBusy == 0)
    {
        // Set the master Control Regs
        MapDrvSetModeEx(
        (FP_GRP1|FP_DRV_MODE_FEXP_POOL|(FP_DRV_LOAD_TYPE0 << 8)), 0,
        *(PUINT64)&pMapDrvCtx->IfftOutTrCtrl[0],
        (pMapDrvCtx->IfftCp1OutSize + pMapDrvCtx->IfftOutSize),
        0);

        pMapDrvCtx->pool1FpBusy = 1;
        ((PTCB)pMapDrvCtx->IfftCb[currSlot])->ExecTicks = MxGetTicks();
        MLogEXEStart(CbCtx, 5, 0);
        MapDispatchFexpPool(1, pMapDrvCtx->pReadyListFftIfftHead[ctxNum][arrIndex], pMapDrvCtx->pReadyListFftIfftHead[ctxNum][arrIndex] + 1);
    }
    else
        pMapDrvCtx->QueueToDoContext[arrIndex] = ctxNum;

    pMapDrvCtx->UpdateQueueFftIfft[ctxNum][arrIndex] = 0;

    // Enable all ARM interrupts
    ARM_INT_restore(irq);

    return MX_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------------








//-------------------------------------------------------------------------------------------------------------------------------------
// FFT Functions
//-------------




//-------------------------------------------------------------------------------------------
/** @brief This function initializes the FFT driver variables
 *
 *
 *  @param   FftExp  This is the IFFT size to be used. FftExp is the exponent value for the
 *                   IfftSize. For example, if the FFT size is 1024, FftExp should be 10
 *                   2^10 = 1024
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapFftDrvInit(UINT32 FftExp)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    PDMAFDESC pFpDmaDesc;
    UINT32 i, symNum;
    UINT32 fft_size, fft_size1;
    UINT32 fft_output_size = 0;
    BOOL cond;

    if (pMapDrvCtx->pFftDmaDesc == NULL)
    {
        pMapDrvCtx->pFftDmaDesc = MxHeapAlloc(DspHeap, 2*FFT_DRV_QSIZE*(2*sizeof(DMAFDESC)));
        _ASSERT_PTR(pMapDrvCtx->pFftDmaDesc);

        if (pMapDrvCtx->pFftDmaDesc == NULL)
            return FP_DRV_RC_ALLOC_ERROR;
    }

    fft_size = 1 << FftExp;

    pFpDmaDesc = pMapDrvCtx->pFftDmaDesc;
    memset(pFpDmaDesc, 0, FFT_DRV_QSIZE*(2*sizeof(DMAFDESC)));

    if (fft_size == 128)
        fft_output_size = 72;
    else if (fft_size == 256)
        fft_output_size = 180;
    else if (fft_size == 512)
        fft_output_size = 300;
    else if (fft_size == 1024)
        fft_output_size = 600;
    else if (fft_size == 2048)
        fft_output_size = 1200;
    else
        return FP_DRV_RC_INVALID_FP_MODE;

    if (fft_size == 128)
    {
        pMapDrvCtx->FftOutBufSize = 72 + 2; // include scale factor
        pMapDrvCtx->FftCp1InSize = 10;
        pMapDrvCtx->FftCp2InSize = 9;
    }
    else if (fft_size == 256)
    {
        pMapDrvCtx->FftOutBufSize = 180 + 2; // include scale factor
        pMapDrvCtx->FftCp1InSize = 20;
        pMapDrvCtx->FftCp2InSize = 18;
    }
    else if (fft_size == 512)
    {
        pMapDrvCtx->FftOutBufSize = 300 + 2; // include scale factor
        pMapDrvCtx->FftCp1InSize = 40;
        pMapDrvCtx->FftCp2InSize = 36;
    }
    else if (fft_size == 1024)
    {
        pMapDrvCtx->FftOutBufSize = 600 + 2; // include scale factor
        pMapDrvCtx->FftCp1InSize = 80;
        pMapDrvCtx->FftCp2InSize = 72;
    }
    else if (fft_size == 2048)
    {
        pMapDrvCtx->FftOutBufSize = 1200 + 2; // include scale factor
        pMapDrvCtx->FftCp1InSize = 160;
        pMapDrvCtx->FftCp2InSize = 144;
    }

    // set up FFT FPDMA Input Data descriptors
    pMapDrvCtx->FftInTrCtrl.FpId = 7;
    pMapDrvCtx->FftInTrCtrl.LoadType = FP_DRV_LOAD_TYPE0; // LOAD0
    pMapDrvCtx->FftInTrCtrl.LoadCont = 0; // Last descriptor
    pMapDrvCtx->FftInTrCtrl.StartBank = 1<<4; // Bank 4
    pMapDrvCtx->FftInTrCtrl.BusSize = 1; // 32-bit
    pMapDrvCtx->FftInTrCtrl.TrInt = 1; // FFT start
    pMapDrvCtx->FftInTrCtrl.StartAddr = 0;
    pMapDrvCtx->FftInTrCtrl.SegCount = 0; // no jumping to next bank

    // set up FFT FPDMA Output Data descriptor
    pMapDrvCtx->FftOutTrCtrl.FpId = 0;
    pMapDrvCtx->FftOutTrCtrl.LoadType = FP_DRV_LOAD_TYPE0; // LOAD0
    pMapDrvCtx->FftOutTrCtrl.LoadCont = 0; // Last descriptor
    pMapDrvCtx->FftOutTrCtrl.StartBank = 1<<4; // Start in Bank 4
    pMapDrvCtx->FftOutTrCtrl.BusSize = 1; // 32-bit
    pMapDrvCtx->FftOutTrCtrl.TrInt = 0; // No interrupt
    pMapDrvCtx->FftOutTrCtrl.StartAddr = 0;
    pMapDrvCtx->FftOutTrCtrl.SegCount = fft_output_size/2;

    if (fft_size == 128)
    {
        // set up FFT FPDMA Input Data descriptors
        pMapDrvCtx->FftInTrCtrl128 = pMapDrvCtx->FftInTrCtrl;
        pMapDrvCtx->FftInTrCtrl128.TrInt = 5; // Set Interrupt 3 for non-aligned inputs
    }


    pMapDrvCtx->pFftDmaDesc = pFpDmaDesc;

    fft_size1 = fft_size + 2;
    cond = 0;
    for (i = 0; i < FFT_DRV_QSIZE*2; ++i)
    {
        if (fft_size == 128)
        {
            symNum = i%14;
                cond = 0;             // Symbols 0, 2, 4, 6, 7, 8, 11, 13
            if ((symNum<7) && ((symNum&1))) // Symbols 1, 3, 5
                    cond = 1;
            if ((symNum>7) && ((symNum&1)==0)) // Symbols 8, 10, 12
                    cond = 1;
        }

        // Set Input descriptor (length and next pointer)
        if (cond)
            pFpDmaDesc->OutBDesc.BCtrl = BCTRL_BLAST_MASK | (fft_size1*sizeof(short)*2);
        else
            pFpDmaDesc->OutBDesc.BCtrl = BCTRL_BLAST_MASK | (fft_size*sizeof(short)*2);
        pFpDmaDesc->NextFDesc = NULL;
        pFpDmaDesc++;

        // Set Output descriptor (length and next pointer)
        pFpDmaDesc->OutBDesc.BCtrl = BCTRL_BLAST_MASK | (pMapDrvCtx->FftOutBufSize*sizeof(short)*2);
        pFpDmaDesc->FControl = 0;
        pFpDmaDesc->FStatus0 = 0;
        pFpDmaDesc->FStatus1 = 0;
        pFpDmaDesc->NextFDesc = NULL;
        pFpDmaDesc++;

    }

    return MX_OK;
}




//-------------------------------------------------------------------------------------------
/** @brief This function lets the driver which MAPs (out of the pre-decided MAPs) to use for
 *         for the FFT function
 *
 *
 *  @param   mask  So by default since MAPs 5-9 are set up for FFT, this is 0x3E0
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapFftDrvSetMapMask(UINT16 mask)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    SINT16 start = -1, stop = -1, i, calc;

    uart_printf("MapFftDrvSetMapMask:  Setting Mask: 0x%04x\n", mask);

    MapStoredFftMask = mask;

    // FFT
    pMapDrvCtx->FFtFpMask = mask;

    for (i = 0; i < pMapCtx->FpDevNum; i++)
    {
        calc = 1<<i;
        if ((calc & mask) != 0)
        {
            start = (start == -1)?i:start;
            stop = i;
        }
    }

    pMapDrvCtx->FftFpStart = pMapDrvCtx->IfftCurFp = start;
    pMapDrvCtx->FftFpStop = stop;

    return MX_OK;
}




//-------------------------------------------------------------------------------------------
/** @brief This function setups a call back function to each FFT call to the driver.
 *
 *
 *  @param   CbProc  Pointer to the CallBack function
 *  @param   CbCtx   Callback parameter
 *
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapFftDrvSetCallback(MAPDONECB CbProc, LPVOID CbCtx)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    pMapDrvCtx->FftPostProcCb = CbProc;
    pMapDrvCtx->FftPostProcParam = CbCtx;

    return MX_OK;
}



//-------------------------------------------------------------------------------------------
/** @brief This function defines how many Rx antennas are there in the base-station. Based on
 *         this number, the FFT driver will dispatch all antennas to be processed by the MAP
 *         in 1 shot. This helps utilizing the MAPDMA hardware dispatcher efficiently
 *
 *
 *  @param   nRxAnt  Number of Rx Antennas
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapFftDrvSetNumAntennas(UINT32 nRxAnt)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    UINT32 numAnt, numSym, SymStart, SymStop, arrIndex, currSlot, ctxNum;
    volatile PDMAFDESC pFftDmaDescBase, pInFDesc, pOutFDesc;

    pMapDrvCtx->nRxAnt = nRxAnt;

    // Build Chain of descriptors in the List
    for (ctxNum = 0; ctxNum < 2; ctxNum++)
    {
        pFftDmaDescBase = pMapDrvCtx->pFftDmaDesc + ctxNum*FFT_DRV_QSIZE*2;

        for (currSlot = 0; currSlot < 2; currSlot++)
        {
            SymStart = currSlot * pMapDrvCtx->nSymFFT;
            SymStop = SymStart + pMapDrvCtx->nSymFFT;
            arrIndex = currSlot;

            pMapDrvCtx->pReadyListFftIfftHead[ctxNum][arrIndex] = pMapDrvCtx->pReadyListFftIfftTail[ctxNum][arrIndex] = NULL;

            for (numAnt = 0; numAnt < nRxAnt; numAnt++)
            {
                for (numSym = SymStart; numSym < SymStop; numSym++)
                {
                    pInFDesc = pFftDmaDescBase + (numSym*2) + (numAnt*14*2);
                    pOutFDesc = pInFDesc+1;

                    pInFDesc->NextFDesc = NULL;
                    pOutFDesc->NextFDesc = NULL;

                    // List building
                    //-----------
                    if (pMapDrvCtx->pReadyListFftIfftHead[ctxNum][arrIndex] == NULL)
                    {
                        pMapDrvCtx->pReadyListFftIfftHead[ctxNum][arrIndex] = pMapDrvCtx->pReadyListFftIfftTail[ctxNum][arrIndex] = pInFDesc;
                    }
                    else
                    {
                        pMapDrvCtx->pReadyListFftIfftTail[ctxNum][arrIndex]->NextFDesc = pInFDesc;       // Chain Input
                        (pMapDrvCtx->pReadyListFftIfftTail[ctxNum][arrIndex]+1)->NextFDesc = pOutFDesc;  // Chain Output

                        pMapDrvCtx->pReadyListFftIfftTail[ctxNum][arrIndex] = pInFDesc;
                    }
                }
            }
        }
    }

    return MX_OK;
}



//-------------------------------------------------------------------------------------------
/** @brief This function registers all FFT Input locations (in DDR / CRAM) with the FFT Driver
 *         These locations are copied into the FFT hardware descriptors so that the MAPDMA will
 *         copy from these locations into internal MAP memory for processing
 *
 *
 *  @param   pInBuf  Pointer to Input buffer
 *  @param   Size    Number of symbols (across all antennas) to be processed
 *  @param   Start   Symbol start number.
 *
 *  This sets up each FFT hardware descriptor with pointers to each symbol's input pointer. When we
 *  allocate memory for FFT input, they are allocated in 1 big group (per antenna / all antennas).
 *  This driver function will use the FFT Size to setup the pointers correctly. It skips over the
 *  CP so that these are not copied to memory un-necessarily
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapFftDrvSetInpBuf(PUINT32 pInpBuf, UINT32 Size, UINT32 Start)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    PDMAFDESC pFpDmaDescBase = pMapDrvCtx->pFftDmaDesc + pMapDrvCtx->QueueContext[FFT_SLOT0]*FFT_DRV_QSIZE*2;
    PDMAFDESC pFpDmaDesc = pFpDmaDescBase + (Start*2);
    UINT32 i, symNum, CpSize, cp1, cp2;
    BOOL cond;

    cp1 = pMapDrvCtx->FftCp1InSize;
    cp2 = pMapDrvCtx->FftCp2InSize;

    cond = 0;
    for (i = Start; i < Size+Start; i++)
    {
        symNum = i%14;
        CpSize = (symNum == 0 || symNum == 7) ? cp1 : cp2; // cycle prefix size

        if (pMapDrvCtx->FftSize == 128)
        {
            cond = 0;             // Symbols 0, 2, 4, 6, 7, 8, 11, 13
            if ((symNum<7) && ((symNum&1))) // Symbols 1, 3, 5
                cond = 1;
            if ((symNum>7) && ((symNum&1)==0)) // Symbols 8, 10, 12
                cond = 1;
        }

        // Set input descriptor
        pInpBuf += (CpSize-cond);
        pFpDmaDesc->OutBDesc.BPtr = (UINT32)pInpBuf;
        pInpBuf += (pMapDrvCtx->FftSize+cond);
        pFpDmaDesc++;

        // Skip output descriptor
        pFpDmaDesc++;

    }

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief This function registers all FFT Output locations (in DDR / CRAM) with the FFT Driver
 *         These locations are copied into the FFT hardware descriptors so that the MAPDMA will
 *         copy the output from MAP to these location after processing
 *
 *
 *  @param   pOutBuf Pointer to Output buffer
 *  @param   Size    Number of symbols (across all antennas) to be processed
 *  @param   Start   Symbol start number.
 *
 *  This sets up each FFT hardware descriptor with pointers to each symbol's output pointer. When we
 *  allocate memory for FFT output, they are allocated in 1 big group (per antenna / all antennas).
 *  This driver function will use the FFT Size to setup the pointers correctly.
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapFftDrvSetOutBuf(PUINT32 pOutBuf, UINT32 Size, UINT32 Start)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    PDMAFDESC pFpDmaDescBase = pMapDrvCtx->pFftDmaDesc + pMapDrvCtx->QueueContext[FFT_SLOT0]*FFT_DRV_QSIZE*2;
    PDMAFDESC pFpDmaDesc = pFpDmaDescBase + (Start*2);
    UINT32 i;

    for (i = Start; i < Size+Start; i++)
    {
        // Skip input descriptor
        pFpDmaDesc++;

        // Set output descriptor
        pFpDmaDesc->OutBDesc.BPtr = (UINT32)pOutBuf;
        pOutBuf += ROUND(pMapDrvCtx->FftOutBufSize, 8);
        pFpDmaDesc++;
    }

    return MX_OK;
}



//-------------------------------------------------------------------------------------------
/** @brief This function sets the MAP number to be used for the next FFT function call
 *
 *
 *  @param   fpNum  Number of the MAP to be used: 5-9
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapFftDrvSetFpNum(UINT32 fpNum)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    pMapDrvCtx->FftCurFp = fpNum;

    return MX_OK;
}



//-------------------------------------------------------------------------------------------
/** @brief This function set's up the number of symbols to be processed by the FFT driver. By
 *         default it is 14. This function is used for debugging the MAP code by running only 1
 *         symbol
 *
 *
 *  @param   numSym  Number of symbols to process
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapFftDrvSetNumSym(UINT32 numSym)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    pMapDrvCtx->nSymFFT = numSym;

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief This function set's up the FFT hardware descriptors in a chain with all symbols to be
 *         processed
 *
 *  @param   void
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapFftDrvPreRun(LPVOID CbCtx)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    volatile PDMAFDESC pFpDmaDescBase, pInFDesc, pOutFDesc;
    PTCB pTcb = (PTCB)CbCtx;

    UINT32 numAnt, numSym, currSlot, SymStart, SymStop, arrIndex, ctxNum;
    BOOL cond;

    currSlot = pTcb->isym;
    SymStart = currSlot * pMapDrvCtx->nSymFFT;
    SymStop = SymStart + pMapDrvCtx->nSymFFT;
    arrIndex = currSlot;
    ctxNum = pMapDrvCtx->QueueContext[arrIndex];
    pFpDmaDescBase = pMapDrvCtx->pFftDmaDesc + ctxNum*FFT_DRV_QSIZE*2;

    cond = 0;

    for (numAnt = 0; numAnt < pMapDrvCtx->nRxAnt; numAnt++)
    {
        for (numSym = SymStart; numSym < SymStop; numSym++)
        {
            // Special 128 Pt FFT
            if (pMapDrvCtx->FftSize == 128)
            {
                cond = 0;             // Symbols 0, 2, 4, 6, 7, 8, 11, 13
                if ((numSym<7) && ((numSym&1))) // Symbols 1, 3, 5
                    cond = 1;
                if ((numSym>7) && ((numSym&1)==0)) // Symbols 8, 10, 12
                    cond = 1;
            }

            pInFDesc = pFpDmaDescBase + (numSym*2) + (numAnt*14*2);
            pOutFDesc = pInFDesc+1;

            //Set Input Descriptor
            pInFDesc->FControl = 0; // disable interrupt
            if (cond)
                *(PMAP_DMA_TR_CTRL)&pInFDesc->FStatus0 = pMapDrvCtx->FftInTrCtrl128;
            else
                *(PMAP_DMA_TR_CTRL)&pInFDesc->FStatus0 = pMapDrvCtx->FftInTrCtrl;

            // Set Output Descriptor
            pOutFDesc->FControl = 0; // disable interrupt
            pOutFDesc->FStatus0 = 0;
            pOutFDesc->FStatus1 = 0;
        }
    }

    (pMapDrvCtx->pReadyListFftIfftTail[ctxNum][arrIndex] + 1)->FControl = 0x1;    // Set Interrupt
    pMapDrvCtx->UpdateQueueFftIfft[ctxNum][arrIndex] = 1;

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief This function starts the list to the MDMA engine, which disptaches it to the pool of
 *         MAPs assigned to run the FFT
 *
 *  @param   void
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_map
 *
**/
//-------------------------------------------------------------------------------------------
MXRC MapFftDrvRun(LPVOID CbCtx)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    PTCB pTcb = (PTCB)CbCtx;

    UINT32 currSlot, arrIndex, ctxNum;
    IRQTYPE irq;

    currSlot = pTcb->isym;
    arrIndex = currSlot;
    ctxNum = pMapDrvCtx->QueueContext[arrIndex];
    pMapDrvCtx->QueueContext[arrIndex] = 1 - pMapDrvCtx->QueueContext[arrIndex];

    if (pMapDrvCtx->UpdateQueueFftIfft[ctxNum][arrIndex] == 0)
    {
        stop_printf("FFT Run: Not Updated: Ctx: %d, arrIndex: %d\r\n", ctxNum, arrIndex);
        return FP_DRV_RC_RUN_ERROR;
    }

    // Disable all ARM interrupts
    irq = ARM_INT_disable();

    if (pMapDrvCtx->QueueFftIfft[pMapDrvCtx->QueueLocationCurr] != NOT_RUNNING)
    {
        stop_printf("Condition should not occur: Queue Full [%d %d %d %d] %d!!!\r\n",
            pMapDrvCtx->QueueFftIfft[0], pMapDrvCtx->QueueFftIfft[1],
            pMapDrvCtx->QueueFftIfft[2], pMapDrvCtx->QueueFftIfft[3],
            pMapDrvCtx->QueueLocationCurr);

        // Enable all ARM interrupts
        ARM_INT_restore(irq);

        return FP_DRV_RC_RUN_ERROR;
    }

    // Store this context in variable. To be used for returing PTCB to Scheduler during MAP IRQ processing
    pMapDrvCtx->FftCb[currSlot] = CbCtx;
    pTcb->ResourceIndex = 0x3E00;

    pMapDrvCtx->QueueFftIfft[pMapDrvCtx->QueueLocationCurr] = FFT_SLOT0+currSlot;
    pMapDrvCtx->QueueLocationCurr++;
    if (pMapDrvCtx->QueueLocationCurr >= 4)
        pMapDrvCtx->QueueLocationCurr = 0;

    if (pMapDrvCtx->pool1FpBusy == 0)
    {
        // Set the master Control Regs
        MapDrvSetModeEx(
        (FP_GRP1|FP_DRV_MODE_FEXP_POOL|(FP_DRV_LOAD_TYPE0 << 8)), 0,
        *(PUINT64)&pMapDrvCtx->FftOutTrCtrl, pMapDrvCtx->FftOutBufSize, 0);

        pMapDrvCtx->pool1FpBusy = 1;
        ((PTCB)pMapDrvCtx->FftCb[currSlot])->ExecTicks = MxGetTicks();
        MLogEXEStart(CbCtx, 5, 0);
        MapDispatchFexpPool(1, pMapDrvCtx->pReadyListFftIfftHead[ctxNum][arrIndex], pMapDrvCtx->pReadyListFftIfftHead[ctxNum][arrIndex] + 1);
    }
    else
        pMapDrvCtx->QueueToDoContext[arrIndex] = ctxNum;

    pMapDrvCtx->UpdateQueueFftIfft[ctxNum][arrIndex] = 0;

    // Enable all ARM interrupts
    ARM_INT_restore(irq);

    return MX_OK;
}

MXRC MapFftIfftDrvContextSwap(UINT32 swapFftIfft)
{
    UINT32 i;
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    if (swapFftIfft & SWAP_FFT)
    {
        for (i = FFT_SLOT0; i < IFFT_SLOT0; i++)
            pMapDrvCtx->QueueContext[i] = 1 - pMapDrvCtx->QueueContext[i];
    }

    if (swapFftIfft & SWAP_IFFT)
    {
        for (i = IFFT_SLOT0; i <= IFFT_SLOT1; i++)
            pMapDrvCtx->QueueContext[i] = 1 - pMapDrvCtx->QueueContext[i];
    }

    return MX_OK;
}

MXRC MapFttIfftDrvFlushQueue(void)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    UINT32 i, FpId, ctxNum;

    // Reset queueing mechanism
    for (FpId = 0; FpId < pMapCtx->FpDevNum; FpId++)
    {
        for (i = 0; i < 2; i++)
            pMapDrvCtx->pReadyListHead[i][FpId] = pMapDrvCtx->pReadyListTail[i][FpId] = NULL;
    }
    for (ctxNum = 0; ctxNum < 2; ctxNum++)
    {
        for (FpId = 0; FpId < 2*NUM_MAP_SLOTS; FpId++)
        {
            pMapDrvCtx->pReadyListFftIfftHead[ctxNum][FpId] = pMapDrvCtx->pReadyListFftIfftTail[ctxNum][FpId] = NULL;
            pMapDrvCtx->UpdateQueueFftIfft[ctxNum][FpId] = 0;
        }
    }
    for (FpId = 0; FpId < NUM_MAP_SLOTS*2; FpId++)
    {
        pMapDrvCtx->QueueFftIfft[FpId] = NOT_RUNNING;
        pMapDrvCtx->QueueContext[FpId] = 0;
    }
    pMapDrvCtx->IdftQueueToRun = 0;
    pMapDrvCtx->IdftQueueRun = 0;
    pMapDrvCtx->QueueLocationCurr = 0;
    pMapDrvCtx->QueueLocationToDo = 0;
    pMapDrvCtx->pool1FpBusy = 0;

    return MX_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------------










//-------------------------------------------------------------------------------------------------------------------------------------
// Other Functions
//----------------

void MapDispatchFexp(UINT32 fpID, PDMAFDESC pInFDesc, PDMAFDESC pOutFDesc)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    if (pMapDrvCtx->BusyDev & (1 << fpID))
    {
        stop_printf ("FP: device is busy (dev=%d)!!!\r\n", fpID);
        return;
    }

    pMapDrvCtx->BusyDev |= (1 << fpID);

    if (fpID < (pMapCtx->FpDevNum/ 2))
    {
        pMapDrvCtx->pDmaExpRegs[FP_GRP0]->FHeadOut[fpID] = (UINT32)pOutFDesc;
        pMapDrvCtx->pDmaExpRegs[FP_GRP0]->FHeadIn[fpID] = (UINT32)pInFDesc;
    }
    else
    {
        pMapDrvCtx->pDmaExpRegs[FP_GRP1]->FHeadOut[fpID%(pMapCtx->FpDevNum/ 2)] = (UINT32)pOutFDesc;
        pMapDrvCtx->pDmaExpRegs[FP_GRP1]->FHeadIn[fpID%(pMapCtx->FpDevNum/ 2)] = (UINT32)pInFDesc;
    }
}


void MapDispatchPool(UINT32 poolId, PDMAFDESC pInFDesc, PDMAFDESC pOutFDesc)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    pMapDrvCtx->pFpDmaRegs[poolId]->DMAOutCtrl.HeadFDesc = (UINT32)pOutFDesc;
    pMapDrvCtx->pFpDmaRegs[poolId]->DMAInCtrl.HeadFDesc = (UINT32)pInFDesc;
}

void MapDispatchFexpPool(UINT32 poolId, PDMAFDESC pInFDesc, PDMAFDESC pOutFDesc)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();

    pMapDrvCtx->pDmaExpRegs[poolId]->FHeadSharedOut = (UINT32)pOutFDesc;
    pMapDrvCtx->pDmaExpRegs[poolId]->FHeadSharedIn = (UINT32)pInFDesc;
}



UINT32 MapDrvSetModeEx(UINT32 GrpIdModeLoadType, UINT32 fp_mask, UINT64 tr_ctrl_out, UINT32 len_out, UINT32 init)
{
    UINT32 DevId;
    UINT8 GrpId = (UINT8)(GrpIdModeLoadType & 0xF);
    UINT8 Mode = (UINT8)(GrpIdModeLoadType & 0xF0);
    UINT8 LoadType = (UINT8)(GrpIdModeLoadType >> 8);
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    UINT64 tr_ctrl_out_set = tr_ctrl_out >> 5;

    if (init)
    {
        fp_mask &= ((GrpId)?0x3E0:0x1F);

        switch(Mode)
        {
            case FP_DRV_MODE_FEXP:
            case FP_DRV_MODE_FEXP_POOL:
            case FP_DRV_MODE_POOL:
                break;
            default:
                return FP_DRV_RC_INIT_ERROR;
        };

        //enable whole module
        if (Mode & (FP_DRV_MODE_FEXP|FP_DRV_MODE_FEXP_POOL))
        {
            pMapDrvCtx->pDmaExpRegs[GrpId]->Enable = (1<<6);

            if(Mode == FP_DRV_MODE_FEXP_POOL)
                pMapDrvCtx->pDmaExpRegs[GrpId]->Enable |= (1<<5);

            if (Mode == FP_DRV_MODE_FEXP)
            {
                pMapDrvCtx->pMasterCtrlRegs->Ctrl1 &= ~fp_mask;
                pMapDrvCtx->pMasterCtrlRegs->IrqEna |= fp_mask;

                //set target arbitration on FP ind accord. to mask fp_mask
                for (DevId = GrpId*FPDRV_NUM_FP_IN_GROUP; DevId < (GrpId*FPDRV_NUM_FP_IN_GROUP+FPDRV_NUM_FP_IN_GROUP); ++DevId)
                {
                    if (fp_mask & (1 << DevId))
                    {
                        pMapDrvCtx->pDmaExpRegs[GrpId]->Enable |= (1 << (DevId % FPDRV_NUM_FP_IN_GROUP));

                        //take in account only 2 types of loading LOAD0 or LOAD1
                        if(LoadType == FP_DRV_LOAD_TYPE0)
                            pMapDrvCtx->pApbRegs[DevId]->TReqEna = 0x00FF00FF; //enable read and write to banks for load0
                        else if(LoadType == FP_DRV_LOAD_TYPE1)
                            pMapDrvCtx->pApbRegs[DevId]->TReqEna = 0xFF00FF00; //enable read and write to banks for load1
                    }
                }
            }
        }
        if (Mode & (FP_DRV_MODE_POOL|FP_DRV_MODE_FEXP_POOL))
        {
            pMapDrvCtx->pMasterCtrlRegs->Ctrl1 |= fp_mask;
            pMapDrvCtx->pMasterCtrlRegs->IrqEna &= ~fp_mask;
            if (Mode == FP_DRV_MODE_POOL)
                pMapDrvCtx->pDmaExpRegs[GrpId]->Enable = 0;
            for (DevId = GrpId*FPDRV_NUM_FP_IN_GROUP; DevId < (GrpId*FPDRV_NUM_FP_IN_GROUP+FPDRV_NUM_FP_IN_GROUP); ++DevId)
                if (fp_mask & (1 << DevId))
                    pMapDrvCtx->pApbRegs[DevId]->TReqEna = 0;
        }
    }

    switch (GrpIdModeLoadType)
    {
        case (FP_GRP0|FP_DRV_MODE_POOL|(FP_DRV_LOAD_TYPE0 << 8)):
        case (FP_GRP0|FP_DRV_MODE_FEXP_POOL|(FP_DRV_LOAD_TYPE0 << 8)):
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutTrLen = len_out;
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutLoad0Ctrl = *(PUINT64)&tr_ctrl_out_set;
            break;
        case (FP_GRP0|FP_DRV_MODE_POOL|(FP_DRV_LOAD_TYPE1 << 8)):
        case (FP_GRP0|FP_DRV_MODE_FEXP_POOL|(FP_DRV_LOAD_TYPE1 << 8)):
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutTrLen = (len_out << 16);
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutLoad1Ctrl = *(PUINT64)&tr_ctrl_out_set;
            break;
        case (FP_GRP1|FP_DRV_MODE_POOL|(FP_DRV_LOAD_TYPE0 << 8)):
        case (FP_GRP1|FP_DRV_MODE_FEXP_POOL|(FP_DRV_LOAD_TYPE0 << 8)):
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutTrLen = len_out;
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutLoad0Ctrl = *(PUINT64)&tr_ctrl_out_set;
            break;
        case (FP_GRP1|FP_DRV_MODE_POOL|(FP_DRV_LOAD_TYPE1 << 8)):
        case (FP_GRP1|FP_DRV_MODE_FEXP_POOL|(FP_DRV_LOAD_TYPE1 << 8)):
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutTrLen = (len_out << 16);
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutLoad1Ctrl = *(PUINT64)&tr_ctrl_out_set;
            break;
        case (FP_GRP0|FP_DRV_MODE_FEXP|(FP_DRV_LOAD_TYPE0 << 8)):
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutTrLen = 0;
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutLoad0Ctrl = 0;
            break;
        case (FP_GRP0|FP_DRV_MODE_FEXP|(FP_DRV_LOAD_TYPE1 << 8)):
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutTrLen = 0;
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutLoad1Ctrl = 0;
            break;
        case (FP_GRP1|FP_DRV_MODE_FEXP|(FP_DRV_LOAD_TYPE0 << 8)):
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutTrLen = 0;
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutLoad0Ctrl = 0;
            break;
        case (FP_GRP1|FP_DRV_MODE_FEXP|(FP_DRV_LOAD_TYPE1 << 8)):
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutTrLen = 0;
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutLoad1Ctrl = 0;
            break;
        default:
            return FP_DRV_RC_INIT_ERROR;
    }

    return MX_OK;
}


MXRC MapDrvSetMode(UINT32 GrpId, UINT32 Mode)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    UINT32 DevId;

    switch (GrpId|Mode)
    {
        case (FP_GRP0|FP_DRV_MODE_POOL):
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutTrLen = pMapDrvCtx->FftSize / 2 + 1; // convert IQ samples number to 64-bit DMA words count
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutLoad0Ctrl = *(PUINT64)&pMapDrvCtx->FftOutTrCtrl;
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutTrLen = pMapDrvCtx->FftSize / 2 + 1; // convert IQ samples number to 64-bit DMA words count
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutLoad0Ctrl = *(PUINT64)&pMapDrvCtx->FftOutTrCtrl;
            pMapDrvCtx->pMasterCtrlRegs->Ctrl1 |= DEVINFO_USE(pMapDrvCtx->IdftFpMask);
            pMapDrvCtx->pMasterCtrlRegs->IrqEna &= (UINT32)(~ DEVINFO_USE(pMapDrvCtx->IdftFpMask));
            pMapDrvCtx->pDmaExpRegs[GrpId]->Enable = 0;
            for (DevId = DEVINFO_USE(pMapDrvCtx->IdftFpStart); DevId <= DEVINFO_USE(pMapDrvCtx->IdftFpStop); ++DevId)
            {
                pMapDrvCtx->pApbRegs[DevId]->TReqEna = 0;
                pMapDrvCtx->pApbRegs[DevId]->Ctrl = 0;
            }
            break;

        case (FP_GRP0|FP_DRV_MODE_FEXP):
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutTrLen = 0;
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutLoad0Ctrl = 0;
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutTrLen = 0;
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutLoad0Ctrl = 0;
            pMapDrvCtx->pMasterCtrlRegs->Ctrl1 &= (UINT32)(~ DEVINFO_USE(pMapDrvCtx->IdftFpMask));
            pMapDrvCtx->pMasterCtrlRegs->IrqEna |= DEVINFO_USE(pMapDrvCtx->IdftFpMask);
            for (DevId = DEVINFO_USE(pMapDrvCtx->IdftFpStart); DevId <= DEVINFO_USE(pMapDrvCtx->IdftFpStop); ++DevId)
            {
                pMapDrvCtx->pApbRegs[DevId]->TReqEna = 0xFF00FF00; // Tr Read for Load1
            }
            pMapDrvCtx->pDmaExpRegs[GrpId]->Enable = (1<<6) | DEVINFO_USE(pMapDrvCtx->IdftFpMask);
            break;

        case (FP_GRP1|FP_DRV_MODE_POOL):
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutTrLen = pMapDrvCtx->FftSize / 2 + 1; // convert IQ samples number to 64-bit DMA words count
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutLoad0Ctrl = *(PUINT64)&pMapDrvCtx->FftOutTrCtrl;
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutTrLen = pMapDrvCtx->FftSize / 2 + 1; // convert IQ samples number to 64-bit DMA words count
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutLoad0Ctrl = *(PUINT64)&pMapDrvCtx->FftOutTrCtrl;
            pMapDrvCtx->pMasterCtrlRegs->Ctrl1 |= DEVINFO_USE(pMapDrvCtx->IfftFpMask);
            pMapDrvCtx->pMasterCtrlRegs->IrqEna &= (UINT32)(~ DEVINFO_USE(pMapDrvCtx->IfftFpMask));
            pMapDrvCtx->pDmaExpRegs[GrpId]->Enable = 0;
            for (DevId = DEVINFO_USE(pMapDrvCtx->IfftFpStart); DevId <= DEVINFO_USE(pMapDrvCtx->IfftFpStop); ++DevId)
            {
                pMapDrvCtx->pApbRegs[DevId]->TReqEna = 0;
                pMapDrvCtx->pApbRegs[DevId]->Ctrl = 0;
            }
            break;

        case (FP_GRP1|FP_DRV_MODE_FEXP):
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutTrLen = 0;
            pMapDrvCtx->pMasterCtrlRegs->Grp0OutLoad0Ctrl = 0;
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutTrLen = 0;
            pMapDrvCtx->pMasterCtrlRegs->Grp1OutLoad0Ctrl = 0;
            pMapDrvCtx->pMasterCtrlRegs->Ctrl1 &= (UINT32)(~ DEVINFO_USE(pMapDrvCtx->IfftFpMask));
            pMapDrvCtx->pMasterCtrlRegs->IrqEna |= DEVINFO_USE(pMapDrvCtx->IfftFpMask);
            for (DevId = DEVINFO_USE(pMapDrvCtx->IfftFpStart); DevId <= DEVINFO_USE(pMapDrvCtx->IfftFpStop); ++DevId)
            {
                pMapDrvCtx->pApbRegs[DevId]->TReqEna = 0xFF00FF00; // Tr Read for Load1
            }
            pMapDrvCtx->pDmaExpRegs[GrpId]->Enable = (1<<6) | DEVINFO_USE(pMapDrvCtx->IdftFpMask);
            break;

        default:
            return FP_DRV_RC_PARAM_ERROR;
    }

    return MX_OK;
}

MXRC MapDrvLoadCode(UINT16 Mask, BOOL bIdft)
{
    PMAP_DRV_CTX pMapDrvCtx = MapGetDrvCtx();
    PUINT32 fft_code = NULL;
    UINT32 fft_code_size = 0;
    UINT16 fp_id, i;
    PUINT8 pBuf;
    static UINT32 segs[3072];
    memset(segs, 0, sizeof (segs));

    pBuf = (UINT8 *)MxHeapAlloc(DspHeap, 10*1024);
    _ASSERT_PTR(pBuf);

    if (!bIdft)
    {
        switch (pMapDrvCtx->FftSize)
        {
            case (1<<FPMODE_FFT0128):
                fft_code = fp_fft0128_code;
                fft_code_size = fp_fft0128_code_size;
                break;
            case (1<<FPMODE_FFT0256):
                fft_code = fp_fft0256_code;
                fft_code_size = fp_fft0256_code_size;
                break;
            case (1<<FPMODE_FFT0512):
                fft_code = fp_fft0512_code;
                fft_code_size = fp_fft0512_code_size;
                break;
            case (1<<FPMODE_FFT1024):
                fft_code = fp_fft1024_code;
                fft_code_size = fp_fft1024_code_size;
                break;
            case (1<<FPMODE_FFT2048):
                fft_code = fp_fft2048_code;
                fft_code_size = fp_fft2048_code_size;
                break;
            case (1<<FPMODE_FFT4096):
                fft_code = fp_fft4096_code;
                fft_code_size = fp_fft4096_code_size;
                break;
            default:
                return FP_DRV_RC_INVALID_FP_MODE;
        }
    }

    for (fp_id = 0; fp_id < pMapCtx->FpDevNum; ++fp_id)
    {
        if (Mask & (1 << fp_id))
        {

            // Initialize all the memory locations to 0
            for (i = 0; i < 8; i++)
                MapDbgUploadDram(fp_id, TRUE, BANK_MODE | i, 0, segs, 2048*2*(i > 3 ? 3 : 2));
            memset(pBuf, 0x42, 10*1024);
            MapUploadPram(fp_id, pBuf, 10*1024);

            if (bIdft)
            {
                memcpy(pBuf, fp_idft_code, fp_idft_code_size);
                MapUploadPram(fp_id, pBuf, fp_idft_code_size/*10*1024*/);

#ifdef LOWERARM_DEBUG_PRINT
                // check if PMEM is loaded correctly
                MapUnloadPram(fp_id, pBuf, fp_idft_code_size);

                if (memcmp(pBuf, fp_idft_code, fp_idft_code_size) == 0)
                    uart_printf("FP%u Init: Succeeded loading PMEM\n", fp_id);
                else
                    uart_printf("FP%u Init: Failed loading PMEM\n", fp_id);
#endif

                MapDbgUploadDram(fp_id, TRUE, BANK_MODE | 1, 0, fp_idft_exp_twiddle, fp_idft_exp_twiddle_size);
                MapDbgUnloadDram(fp_id, TRUE, BANK_MODE | 1, 0, pBuf, fp_idft_exp_twiddle_size);

#ifdef LOWERARM_DEBUG_PRINT
                if (memcmp(pBuf, fp_idft_exp_twiddle, fp_idft_exp_twiddle_size) == 0)
                    uart_printf("FP%u Init: Succeeded loading fp_idft_exp_twiddle (Bank 1)\n", fp_id);
                else
                    uart_printf("FP%u Init: Failed loading fp_idft_exp_twiddle (Bank 1)\n", fp_id);
#endif
            }
            else
            {
                memcpy(pBuf, fft_code, fft_code_size);
                MapUploadPram(fp_id, pBuf, fft_code_size/*10*1024*/);

#ifdef LOWERARM_DEBUG_PRINT
                // check if PMEM is loaded correctly
                MapUnloadPram(fp_id, pBuf, fft_code_size);
                if (memcmp(pBuf, fft_code, fft_code_size) == 0)
                    uart_printf("FP%u Init: Succeeded loading PMEM\n", fp_id);
                else
                    uart_printf("FP%u Init: Failed loading PMEM\n", fp_id);
#endif

                // load FFT table to bank0 (segments 0/1)
                MapDbgUploadDram(fp_id, TRUE, BANK_MODE | 0, 0, fp_fft_table, fp_fft_table_size);
                // read FFT table back from bank 0 (segments 0/1)
                MapDbgUnloadDram(fp_id, TRUE, BANK_MODE | 0, 0, pBuf, fp_fft_table_size);

#if defined(LOWERARM_DEBUG_PRINT)
                // check if bank 0 is loaded correctly
                if (memcmp(pBuf, fp_fft_table, fp_fft_table_size) == 0)
                    uart_printf("FP%u Init: Succeeded loading fp_fft_table (Bank 0)\n", fp_id);
                else
                    uart_printf("FP%u Init: Failed loading fp_fft_table (Bank 0)\n", fp_id);
#endif

                // load FFT shift table to segment 2 (bank 1)
                MapDbgUploadDram(fp_id, TRUE, 2, 0, fp_fft_shift_table, fp_fft_shift_table_size);
                MapDbgUnloadDram(fp_id, TRUE, 2, 0, pBuf, fp_fft_shift_table_size);

#if defined(LOWERARM_DEBUG_PRINT)
                // check if segment 2 is correctly
                if (memcmp(pBuf, fp_fft_shift_table, fp_fft_shift_table_size) == 0)
                    uart_printf("FP%u Init: Succeeded loading fp_fft_shift_table (Segment 2)\n", fp_id);
                else
                    uart_printf("FP%u Init: Failed loading fp_fft_shift_table (Segments 2)\n", fp_id);
#endif
            }
        }
    }

    MxHeapFree(DspHeap, pBuf);

    return MX_OK;
}

void MapRegWrite(UINT32 fpID, UINT32 reg_offset, UINT32 reg_val)
{
    *(UINT32 *)(FP_CTRL_BASEADDR + reg_offset) = reg_val;
}

UINT32 MapRegRead(UINT32 fpID, UINT32 reg_offset)
{
    return *(UINT32 *)(FP_CTRL_BASEADDR + reg_offset);
}
/*
void MapUploadPram(UINT32 devID, PVOID prog_buf, UINT32 prog_buf_size)
{
    UINT32 *pU32 = (UINT32 *)prog_buf;
    volatile PMAP_MASTER_CTRL_REGS pMasterCtrlRegs = (PMAP_MASTER_CTRL_REGS)(FP_CTRL_BASEADDR + FP_MASTER_CTRL_REG_OFFSET);
    volatile PFPAPBREGS pFpRegs;
    UINT32 i;

    if (devID == FP_BCST_ID)
    {
        pMasterCtrlRegs->BCastMask = 0;
        pFpRegs = (PFPAPBREGS)(FP_CTRL_BASEADDR + FP_WRITE_BCST_REG_OFFSET);
    }
    else
    {
        pFpRegs = (PFPAPBREGS)(FP_CTRL_BASEADDR + devID*0x800);
    }

    prog_buf_size = prog_buf_size / 20; // convert to a number of instructions

    pFpRegs->Ctrl = FP_CTRL_RESET | FP_CTRL_PMEM_ACCESS_ENABLE; // put FP core in reset and enable PMEM read/write

    pFpRegs->IAddr = (UINT32)5<<29; //set auto increment on each 5th access and set address to 0

    for (i = 0; i < prog_buf_size; i++)
    {
        pFpRegs->MAcc_159_128 = *pU32++;
        pFpRegs->MAcc_127_96 = *pU32++;
        pFpRegs->MAcc_95_64 = *pU32++;
        pFpRegs->MAcc_63_32 = *pU32++;
        pFpRegs->MAcc_31_0 = *pU32++;
    }
    pFpRegs->Ctrl = 0;
}*/
// Put executable data to program memory
void MapUploadPram(UINT32 fpID, LPVOID prog_buf, UINT32 prog_buf_size)
{
    UINT32 *pU32 = (UINT32 *)prog_buf;
    volatile PMAP_APB_REGS pFpRegs = (PMAP_APB_REGS)(FP_CTRL_BASEADDR + fpID*0x800);
    UINT32 i;

    prog_buf_size = prog_buf_size / 20; // convert to a number of instructions

    pFpRegs->Ctrl = FP_CTRL_RESET | FP_CTRL_RESET_IO | FP_CTRL_PMEM_ACCESS_ENABLE; // put FP core in reset and enable PMEM read/write

    pFpRegs->IAddr = (UINT32)5<<29; //set auto increment on each 5th access and set address to 0
//  pFpRegs->IAddr = 0; // set address to 0

    for (i = 0; i < prog_buf_size; i++)
    {
//        pFpRegs->IAddr = i;
        pFpRegs->MAcc_159_128 = *pU32++;
        pFpRegs->MAcc_127_96 = *pU32++;
        pFpRegs->MAcc_95_64 = *pU32++;
        pFpRegs->MAcc_63_32 = *pU32++;
        pFpRegs->MAcc_31_0 = *pU32++;
    }
    pFpRegs->Ctrl = 0;
}

// Get executable data to program memory
void MapUnloadPram(UINT32 fpID, LPVOID prog_buf, UINT32 prog_buf_size)
{
    volatile PMAP_APB_REGS pFpRegs = (PMAP_APB_REGS)(FP_CTRL_BASEADDR + fpID*0x800);
    UINT32 *pU32 = (UINT32 *)prog_buf;
    UINT32 i;

    prog_buf_size = prog_buf_size / 20; // convert to a number of instructions

    pFpRegs->Ctrl = FP_CTRL_RESET | FP_CTRL_RESET_IO | FP_CTRL_PMEM_ACCESS_ENABLE; // put FP core in reset and enable PMEM read/write

    pFpRegs->IAddr = (UINT32)5<<29; //set auto increment on each 5th access and set address to 0
//  pFpRegs->IAddr = 0; // set address to 0

    for (i = 0; i < prog_buf_size; i++)
    {
//      pFpRegs->IAddr = i;
        *pU32++ = pFpRegs->MAcc_159_128;
        *pU32++ = pFpRegs->MAcc_127_96;
        *pU32++ = pFpRegs->MAcc_95_64;
        *pU32++ = pFpRegs->MAcc_63_32;
        *pU32++ = pFpRegs->MAcc_31_0;
    }
    pFpRegs->Ctrl = 0;
}

// Put data to data banks/segments
void MapDbgUploadDram(UINT32 fpID, BOOL auto_inc, UINT32 bank, UINT32 offset, LPVOID data_buf, UINT32 data_buf_size)
{
    volatile PMAP_APB_REGS pFpRegs = (PMAP_APB_REGS)(FP_CTRL_BASEADDR + fpID*0x800);
    UINT32 int_reg;
    UINT32 val;
    UINT32 i = offset;
    UINT32 saved_ctrl_reg = pFpRegs->Ctrl;

    pFpRegs->Ctrl = FP_CTRL_DEBUG_MODE; // set debug mode

    int_reg = (bank<<16) | offset;  // bank select

    if (auto_inc)
        int_reg |= (1<<29);

    pFpRegs->IAddr = int_reg;

    if (bank & BANK_MODE)
    { // bank or registers access mode
        // 32-bit transaction mode (writing into 2 adjacent segments)
        UINT32 *pU32 = (UINT32 *)data_buf;

        data_buf_size = data_buf_size / sizeof(UINT32);
        while (data_buf_size--)
        {
            if (!auto_inc)
                pFpRegs->IAddr = int_reg | i;

            val = *pU32++;

            pFpRegs->MAcc_31_0 = val;

            i++;

        }
    }
    else if (bank < 8)
    {
        UINT16 *pU16 = (UINT16 *)data_buf;

        // 16-bit transaction mode
        data_buf_size = data_buf_size / sizeof(UINT16);
        while (data_buf_size--)
        {
            if (!auto_inc)
                pFpRegs->IAddr = int_reg | i;

            val = *pU16++;

            pFpRegs->MAcc_31_0 = val;

            i++;

        }
    }
    else
    {
        // 24-bit transaction mode
        UINT8 *pU8 = (UINT8 *)data_buf;

        data_buf_size = data_buf_size / 3;
        while (data_buf_size--)
        {
            if (!auto_inc)
                pFpRegs->IAddr = int_reg | i;

            val = *pU8++;
            val |= (UINT32)(*pU8++) << 8;
            val |= (UINT32)(*pU8++) << 16;

            pFpRegs->MAcc_31_0 = val;

            i++;

        }
    }
    pFpRegs->Ctrl = saved_ctrl_reg;
}
/*
        //increment block select
        if ((UINT16)(int_reg = pFpRegs->IAddr) == FP_SEGMENT_SIZE)
        {
            int_reg &= 0xFFFF0000; // reset segment address pointer
            int_reg += 0x00010000; // go to next segment(s)
            pFpRegs->IAddr = int_reg;
        }
*/

// Get data from data banks/segments
void MapDbgUnloadDram(UINT32 fpID, BOOL auto_inc, UINT32 bank, UINT32 offset, LPVOID data_buf, UINT32 data_buf_size)
{
    volatile PMAP_APB_REGS pFpRegs = (PMAP_APB_REGS)(FP_CTRL_BASEADDR + fpID*0x800);
    UINT32 int_reg;
    UINT32 val;
    UINT32 i = offset;
    UINT32 saved_ctrl_reg = pFpRegs->Ctrl;

    pFpRegs->Ctrl = FP_CTRL_DEBUG_MODE; // set debug mode

    int_reg = (bank<<16) | offset;  // bank select

    if (auto_inc)
        int_reg |= (1<<29);

    pFpRegs->IAddr = int_reg;

    if (bank & BANK_MODE)
    { // bank or registers access mode
        // 32-bit transaction mode (reading from 2 adjacent segments)
        UINT32 *pU32 = (UINT32 *)data_buf;

        data_buf_size = data_buf_size / sizeof(UINT32);
        while (data_buf_size--)
        {
            if (!auto_inc)
                pFpRegs->IAddr = int_reg | i;

            val = pFpRegs->MAcc_31_0;

            *pU32++ = val;
            i++;
        }
    }
    else if (bank < 8)
    {
        // 16-bit transaction mode
        UINT16 *pU16 = (UINT16 *)data_buf;

        data_buf_size = data_buf_size / sizeof(UINT16);
        while (data_buf_size--)
        {
            if (!auto_inc)
                pFpRegs->IAddr = int_reg | i;

            val = pFpRegs->MAcc_31_0;

            *pU16++ = val;
            i++;
        }
    }
    else
    {
        // 24-bit transaction mode
        UINT8 *pU8 = (UINT8 *)data_buf;

        data_buf_size = data_buf_size / 3;
        while (data_buf_size--)
        {
            if (!auto_inc)
                pFpRegs->IAddr = int_reg | i;

            val = pFpRegs->MAcc_31_0;

            *pU8++ = (UINT8)val;
            *pU8++ = (UINT8)(val>>8);
            *pU8++ = (UINT8)(val>>16);
            i++;
        }
    }
    pFpRegs->Ctrl = saved_ctrl_reg;

    //uart_printf("0x%x 0x%x 0x%x 0x%x 0x%x\n",
    //    *((PUINT16)data_buf), *((PUINT16)data_buf + 1), *((PUINT16)data_buf + 2), *((PUINT16)data_buf + 3), *((PUINT16)data_buf + 4)
    //    );
}
/*
        //increment block select
        if ((UINT16)(int_reg = pFpRegs->IAddr) == FP_SEGMENT_SIZE)
        {
            int_reg &= 0xFFFF0000; // reset segment address pointer
            int_reg += 0x00010000; // go to next segment(s)
            pFpRegs->IAddr = int_reg;
        }
*/

UINT32 MapDbgRun(UINT32 fpID, UINT32 nCycles)
{
    volatile PMAP_APB_REGS pFpRegs = (PMAP_APB_REGS)(FP_CTRL_BASEADDR + fpID*0x800);
    UINT32 start_tick;
    UINT32 exec_time;

    pFpRegs->Ctrl = FP_CTRL_DEBUG_MODE; // set debug mode

    pFpRegs->DbgRun = 0x80000000 | nCycles;

    start_tick = GetTIMETICK();

    // wait till FP stops
    do
        exec_time = GetExecBusTicks(start_tick);
    while (pFpRegs->DbgRun && exec_time <= FP_RUN_TIMEOUT);

    return (pFpRegs->DbgRun == 0) ? exec_time : 0;
}

void MapDbgSetPcTrigger(UINT32 fpID, UINT32 pc)
{
    volatile PMAP_APB_REGS pFpRegs = (PMAP_APB_REGS)(FP_CTRL_BASEADDR + fpID*0x800);

    pFpRegs->Ctrl = FP_CTRL_DEBUG_MODE; // set debug mode
    pFpRegs->DbgPcTrig = 0x80000000 | pc; // set and enable PC trigger
}

void MapDbgSetDebugTrigger(UINT32 fpID, UINT32 counter)
{
    volatile PMAP_APB_REGS pFpRegs = (PMAP_APB_REGS)(FP_CTRL_BASEADDR + fpID*0x800);

    pFpRegs->Ctrl = FP_CTRL_DEBUG_MODE; // set debug mode
    pFpRegs->DbgCntTrig = 0x80000000 | counter; // set and enable Debug Counter trigger
}

void FpMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    if (nCpuID == 0)
    {
        MmuMgrCfgMemAP(FP_CTRL_BASEADDR,    80*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(SPUCLKRST_BASEADDR,  4*1024,  AP_MGR_READ_WRITE, AP_NO_ACCESS);
    }

    #ifdef REX_SERV_ON_ARM

        if (nCpuID == REX_SERV_ARM_CORE_ID)
        {
            MmuMgrCfgMemAP(FP_CTRL_BASEADDR,    80*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
            MmuMgrCfgMemAP(SPUCLKRST_BASEADDR,  4*1024,  AP_MGR_READ_WRITE, AP_NO_ACCESS);
            //MmuMgrCfgMemAP(SPU_BASEADDR,        100*1024,AP_MGR_READ_WRITE, AP_NO_ACCESS);
        }

    #endif
}

//-------------------------------------------------------------------------------------------------------------------------------------

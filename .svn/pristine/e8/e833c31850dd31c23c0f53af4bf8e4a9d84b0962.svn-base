//-------------------------------------------------------------------------------------------
/** @file DsPdrv.c
 *
 * @brief DSP (Ceva) HAL functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.71 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "diagnostics.h"
#include "appprintf.h"
#include "timerdrv.h"
#include "DSPdrv.h"
#include "heaps.h"
#include "icdrv.h"
#include "initcpu.h"
#include "mlog.h"
#include "clkrst.h"
#include "mmumgr.h"
// ========================================
//         DSP driver global data
// ========================================

UINT32 ExtCodeDataLen = 0;
DSPDRV_CTX DspDrvCtx = {0};
UINT32 DspDrvRexServCeva = DSP_DRV_DEF_REX_SERV;
void (*DspDrvRexServCevaCb)(void * Ptr) = NULL;
void * DspDrvRexServCevaCbPtr = NULL;

static __align (1024) TCB NCNBVAR dsp_tcb[DEVID_MAX_DSP_CORE];

// The local CEVA drivers parameters
// it's used by the ARM drivers to provide parameters
// for CEVA driver (when CEVA code is not started yet)
// and used by DSP driver to configure CEVA drivers
// in a process of CEVA code startup

CEVA_DRV_CFG LocalCevaDrvCfg;

extern const unsigned cevabincode_size;
extern const unsigned char cevabincode[];
extern const unsigned cevabincodedmp_size;
extern const unsigned char cevabincodedmp[];
extern const unsigned rexservbincode_size;
extern const unsigned char rexservbincode[];
// ========================================

//-------------------------------------------------------------------------------------------
/** @brief This function returns the Interrupt status of the CEVA DSPs
 *
 *
 *  @param   none
 *
 *  @return  unsigned int  Interrupt status for all 10 Cevas. Bits [9:0]
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 DspDrvGetCevaIRQStatus (void)
{
    // The CEVA IRQs are routed to PEND_STATUS[1] and PEND_STATUS[2]
    // so we need to check only these pend_status words

    UINT32 nStatus1 = (ICDrvGetPendStatus (1) >> 30);
    UINT32 nStatus2 = ((ICDrvGetPendStatus (2) & 0xFF) << 2);

    // (nStatus1 | nStatus2) - is a bit field for 10 CEVAS [9:0]

    return (nStatus1 | nStatus2);
}

//-------------------------------------------------------------------------------------------
/** @brief This function returns the Interrupt status of the CEVA DSP in question
 *
 *
 *  @param   nThisOne  CEVA DSP Number (0-9)
 *
 *  @return  unsigned int  1 = Pending Interrupt. 0 = No Interrupt
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 DspDrvCheckCevaIrqStatus (UINT32 nThisOne)
{
    // The CEVA IRQs are routed to PEND_STATUS[1] and PEND_STATUS[2]
    // so we need to check only these pend_status words

    UINT32 nStatus1 = (ICDrvGetPendStatus (1) >> 30);
    UINT32 nStatus2 = ((ICDrvGetPendStatus (2) & 0xFF) << 2);

    // (nStatus1 | nStatus2) - is a bit field for 10 CEVAS [9:0]

    return (nStatus1 | nStatus2) & (1 << nThisOne);
}

//-------------------------------------------------------------------------------------------
/** @brief This handler is used for the asyncronious DSP running.
 *
 *
 *  @param   pIntClearReg [out] IRQ clean register
 *  @param   nIrqId [in]  IRQ Id
 *  @param   pParam [in]  the pointer to the DSP diagnostic context
 *
 *  @return  none
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------

void DspDrvIrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, LPVOID pParam)
{
    PTCB nextTCB;
    PDSP_CTX pDspCtx = (PDSP_CTX)pParam;
    PTCB pTcb = pDspCtx->pExecTcb;
    VUINT32 nDspId = pDspCtx->DspID;

#ifdef DSP_LEVEL_INT_TYPE
    UINT32 BitMask;
    PVUINT32 pSpiStatusReg;

    if (nDspId < 2)
    {
        pSpiStatusReg = (PUINT32)0xFFF01D04; // Distributor Status 0
        BitMask = 1 << (30+nDspId);
    }
    else
    {
        pSpiStatusReg = (PUINT32)0xFFF01D08; // Distributor Status 1
        BitMask = 1 << (nDspId-2);
    }

    if (pTcb->Status == STATUS_COMPLETE)
    {
        REG32(SET_NMI_REG) = (1<<nDspId); // CEVA Level Interrupt Ack
	      while (*pSpiStatusReg & BitMask);
    }
#endif

    // Just to process REX-SERV notification
    // if REX-SERV callback was set
    if (DspDrvRexServCeva == nDspId && DspDrvRexServCevaCb != NULL)
    {
        *pIntClearReg = nIrqID;
        DspDrvRexServCevaCb(DspDrvRexServCevaCbPtr);
        return;
    }

    //uart_printf ("DSP(%d):IRQ\r\n", nDspId);

    DspDrvAddToLog(pDspCtx, pTcb);

    if (pTcb->Status != STATUS_COMPLETE)
    {
        *pIntClearReg = nIrqID;
        uart_printf("Fake CEVA-%d interrupt\n", nDspId);
        return;
    }
    nextTCB = pTcb;

#ifdef MLOG_ENABLED

    while (nextTCB)
    {
        MLogEXEFinish2(nextTCB,nDspId);

#ifdef MLOG_SUBTASK_ENABLE
        if (nextTCB->SubTaskTimer)
        {
            UINT32 *pInput, ii, len;
            pInput = (UINT32 *) EXTRACTTCBIOBUF(nextTCB, 0);

            len = *pInput++;
            //uart_printf("len = %d\n", len);

            for (ii = 0; ii < len ; ii++, pInput+=2)
            {
                if (*(pInput) != 0)
                {
                    MLogSubTask(nextTCB, *(pInput), *(pInput+1));
                    //uart_printf("%d %d\n", *(pInput), *(pInput+1));
                }
            }
        }
#endif

        nextTCB= nextTCB->NextSupTcb;
    }

#endif // #ifdef MLOG_ENABLED

    //MLogEXEFinish(pTcb); //do it again.. this is because mlogfinish needs exeticks and we moved mlogfinish inside the loop instead cevacbdone.

    *pIntClearReg = nIrqID;

    if (pTcb->cbDone)
        pTcb->cbDone(pTcb);

}

void DspDrvShowCevaDrvUsedMask(DSP_INTERFACE * pCommInt)
{
    UINT32 num = 0;
    _ASSERT_PTR(pCommInt);

    uart_printf_set_header("\r\n");

    if (pCommInt->CevaDrvCfg.SysMdmaCfg.UsedMask != 0)
    {
        num++;
        uart_printf ("   * [SYS-MDMA] is opened by [0x%04x] CEVAs\r\n", pCommInt->CevaDrvCfg.SysMdmaCfg.UsedMask);
    }

    if (pCommInt->CevaDrvCfg.SpuMdmaCfg.UsedMask != 0)
    {
        num++;
        uart_printf ("   * [SPU-MDMA] is opened by [0x%04x] CEVAs\r\n", pCommInt->CevaDrvCfg.SpuMdmaCfg.UsedMask);
    }

    if (pCommInt->CevaDrvCfg.RadMdmaCfg.UsedMask != 0)
    {
        num++;
        uart_printf ("   * [RAD-MDMA] is opened by [0x%04x] CEVAs\r\n", pCommInt->CevaDrvCfg.RadMdmaCfg.UsedMask);
    }

    if (pCommInt->CevaDrvCfg.FecDLCfg.UsedMask != 0)
    {
        num++;
        uart_printf ("   * [FEC-DL  ] is opened by [0x%04x] CEVAs\r\n", pCommInt->CevaDrvCfg.FecDLCfg.UsedMask);
    }

    if (pCommInt->CevaDrvCfg.FecULCfg.UsedMask != 0)
    {
        num++;
        uart_printf ("   * [FEC-UL  ] is opened by [0x%04x] CEVAs\r\n", pCommInt->CevaDrvCfg.FecULCfg.UsedMask);
    }

    uart_printf_set_header(NULL);

    if (num)
        uart_printf ("\r\n");

}

//-------------------------------------------------------------------------------------------
/** @brief This function Initializes the DSP driver. Allocates needed memory for the driver
 *
 *
 *  @param   none
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------
MXRC DspDrvInit(void)
{
    MXRC rc = MX_OK, rc_tmp = 0, rc_serv = 0, rc_phy = 0;
    PDSP_CTX pDspCtx;
    UINT32 dspID;
    PUINT8 pDump = NULL;
    UINT32 nCevaDumpSize = DSP_DUMP_SIZE;

    DSP_INTERFACE * pInt = (DSP_INTERFACE *)TCB_BASE_ADDR;

    if (!DspDrvCtx.bInitialized)
    {
        DspDrvResetDSPAll();

        memset ((LPVOID)DSP_BOOT_DESC_ADDR, 0, DSP_EXT_SEGMENTS - DSP_BOOT_DESC_ADDR);

        DspDrvRexServCevaCb = NULL;
        DspDrvRexServCevaCbPtr = NULL;

        memset (pInt, 0, sizeof (DSP_INTERFACE));
        memset (dsp_tcb, 0, sizeof (dsp_tcb));

        memcpy (&pInt->CevaDrvCfg, &LocalCevaDrvCfg, sizeof (LocalCevaDrvCfg));

        DspDrvCtx.RexServID = 0xFF;
        DspDrvCtx.pHeadTcb = pInt;
        DspDrvCtx.CpuID = MxGetCpuID ();
        DspDrvCtx.hDspHeap = DspHeap;
        DspDrvCtx.numDSP = DevInfoGetNumber(DEVID_DSP_CORE);

        #ifdef DSP_DUMP_ENABLED
        pDump = (PUINT8)MxHeapAlloc (DSP_DUMP_HEAP, DspDrvCtx.numDSP * nCevaDumpSize);

        if (pDump == NULL)
        {
            uart_printf ("CEVA DUMP is turned off (no memory)\r\n");
        }
        else
        {
            memset(pDump, 0xA5, DspDrvCtx.numDSP * nCevaDumpSize);
        }
        #endif

        for (dspID = 0; dspID < DspDrvCtx.numDSP; dspID++)
        {
            pDspCtx = (PDSP_CTX)&DspDrvCtx.DspCtx[dspID];
            pDspCtx->DspID = dspID;
            pDspCtx->pDumpPtr = pDump;

            SetDspDump(dspID, pDump);

            if (pDump != NULL)
            {
                pDump += nCevaDumpSize;
            }

            // to setup TMP pointer to the TCB block per each DSP
            pInt->CevaTcb [dspID] = &(dsp_tcb[dspID]);
        }

        if (DspDrvRexServCeva != 0xFF)
        {
            DspDrvCtx.RexServID = DspDrvRexServCeva;

            // to load the REX CODE

            uart_printf ("Booting REX-SERV CEVA-%d at %dMhz ...", DspDrvRexServCeva, ClkRstGetFreq (CR_DEV_CEVA_0) / 1000000);
            rc_serv = DspDrvBootCeva(DspDrvRexServCeva, (PUINT8)rexservbincode, (UINT32)rexservbincode_size, DSP_BOOT_TIMEOUT);
            DspDrvCtx.RexServLoadErr = rc_serv;

            if (FAILED(rc_serv))
            {
                uart_printf ("DSB boot is failed, rc=%x\r\n", rc_serv);
            }
            else
            {
                uart_printf ("is completed\r\n");
            }
        }

        // In this case we always boot DSP processors

        uart_printf ("Booting CEVAs at %dMhz ...", ClkRstGetFreq (CR_DEV_CEVA_0) / 1000000);

        // to boot the CEVA code, the time limit is 300 ms per each DSP
        rc_phy = DspDrvBoot((PUINT8)cevabincode, (UINT32)cevabincode_size, DSP_BOOT_TIMEOUT);

        if (FAILED(rc_phy))
        {
            uart_printf ("DSB boot is failed, rc=%x\r\n", rc_phy);
        }
        else
        {
            uart_printf ("is completed\r\n");
        }

        // Use PLL0 (500 MHz frequency) for DSPs normal operation
        if (DevInfoIsT4Kx4())
        {
            REG32(CEVA_CLK_CNTRL) = PLL_SOURCE(5);
        }

        for (dspID = DevInfoGetValue(CONSTID_DSP_STARTID); dspID < DspDrvCtx.numDSP; dspID++)
        {
            pDspCtx = (PDSP_CTX)&DspDrvCtx.DspCtx[dspID];

#ifdef DSP_LEVEL_INT_TYPE
            MxSetIntHandler(HAL_IRQ_CEVA(dspID), INT_TYPE_IRQ, PRI_DSP, DspDrvIrqHandler, pDspCtx);
#else
            MxSetIntHandler(HAL_IRQ_CEVA(dspID), (ITYPE)(INT_TYPE_IRQ | INT_TYPE_EDGE), PRI_DSP, DspDrvIrqHandler, pDspCtx);
#endif
            MxEnableInt(HAL_IRQ_CEVA(dspID));

            if (DspDrvCtx.DevUpMask & (1 << dspID))
            {
                uart_printf (" *CEVA[%d] {DUMP:0x%08x} BISR", dspID, pDspCtx->pDumpPtr);

                rc_tmp = DspDrvBist(dspID);

                if (FAILED(rc_tmp) || !FAILED(rc))
                {
                    rc = rc_tmp;
                }

                if (FAILED(rc_tmp))
                {
                    DspDrvCtx.DevUpMask ^= (1 << dspID);
                }
            }
        }

        DspDrvShowCevaDrvUsedMask (pInt);

        DspDrvCtx.bInitialized = TRUE;

        uart_printf ("DSP-%s{ret-0x%x}, CPU-%d (passed:%x)(failed:%x)\n",
                FAILED(rc) ? "FAILED" : "OK",
                rc,
                MxGetCpuID (),
                DspDrvCtx.DevUpMask,
                DspDrvCtx.DevUpMask ^ DspDrvCtx.DevEnaMask);

    }

    if (FAILED(rc_serv))
        return rc_serv;

    if (FAILED(rc_phy))
        return rc_phy;

    return rc;
}

MXRC DspDrvGetMasterCpu (void)
{
    return DspDrvCtx.CpuID;
}

MXRC DspDrvResetAllDSPExcept(UINT32 exp_mask)
{
    UINT32 DspNum = DevInfoGetNumber(DEVID_DSP_CORE);
    UINT32 MaskAll = (1 << DspNum) - 1;

    MaskAll &= ~exp_mask;

    if((MaskAll >> 4 ) != 0)
    {
        REG32(CEVA_RESET_1) = (MaskAll >> 4) & 0x3F;
    }

    REG32(CEVA_RESET_0) = ((MaskAll << 4) & 0xF0) | 0;

    return 0;
}

MXRC DspDrvResetDSPAll(void)
{
    UINT32 DspNum = DevInfoGetNumber(DEVID_DSP_CORE);
    UINT32 MaskAll = (1 << DspNum) - 1;

    if ((REG32(CEVA_RESET_1) & 0x3F == 0x3F) && (REG32(CEVA_RESET_0) & 0xF0 == 0xF0))
        return MX_OK;

    if((MaskAll >> 4 ) != 0)
    {
        REG32(CEVA_RESET_1) = (MaskAll >> 4) & 0x3F;
    }

    REG32(CEVA_RESET_0) = ((MaskAll << 4) & 0xF0) | 0;

    return 0;
}

MXRC DspDrvResetCeva(UINT32 CevaID)
{
    if (CevaID < 4)
        REG32(CEVA_RESET_0) |= ((1 << CevaID) << 4);
    else
        REG32(CEVA_RESET_1) |= (1 << (CevaID - 4));

    return 0;
}

MXRC DspDrvResetCevaByMask(UINT32 CevaMask)
{
    UINT32 nMaskA, nMaskB;

    nMaskA = (CevaMask & 0xF);
    nMaskB = ((CevaMask >> 4) & 0x3F);

    if (nMaskA != 0)
        REG32(CEVA_RESET_0) |= (nMaskA << 4);

    if (nMaskB != 0)
        REG32(CEVA_RESET_1) |= nMaskB;

    return 0;
}

MXRC DspDrvTurnOnDSPAll(UINT32 Mask)
{
    REG32(CEVA_RESET_1) = ((~Mask) >> 4) & 0x3F;
    REG32(CEVA_RESET_0) = ((~Mask) << 4) & 0xF0;
    return 0;
}

MXRC DspDrvTurnOnDSP(UINT32 CevaID)
{
    if (CevaID < 4)
        REG32(CEVA_RESET_0) &= ~(((1 << CevaID) << 4) | 1);
    else
        REG32(CEVA_RESET_1) &= ~(1 << (CevaID - 4));

    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief This function shuts down the DSP Driver. Cleaning up allocated memory.
 *
 *
 *  @param   none
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------
MXRC DspDrvShutdown (void)
{
    UINT32 i; IRQTYPE irq;
    LPVOID pDump;

    // to shutdown the driver and set CEVAs to reset
    if (DspDrvCtx.bInitialized && (DspDrvCtx.CpuID == MxGetCpuID ()))
    {
        irq = ARM_INT_disable ();

        // to set CEVAs to the reset

        DspDrvResetDSPAll();

        pDump = DspDrvCtx.DspCtx[0].pDumpPtr;

        if (pDump != NULL)
        {
            uart_printf ("CEVA DUMP = %x", pDump);
            MxHeapFree (DSP_DUMP_HEAP, pDump);
            DspDrvCtx.DspCtx[0].pDumpPtr = NULL;
        }

        // to disable interrupts and destroy scheduler queues

        for (i = 0; i < DevInfoGetNumber(DEVID_DSP_CORE); i++)
        {
            MxDisableInt(HAL_IRQ_CEVA(i));

            DspDrvCtx.DspCtx[i].pDumpPtr = NULL;
            SetDspDump (i, NULL);
        }

        DspDrvCtx.bInitialized = FALSE;

        ARM_INT_restore (irq);
    }

    return MX_OK;
}

void DspDrvSetSyncMode(UINT32 dspID)
{
    PDSP_CTX pDspCtx = (PDSP_CTX)&DspDrvCtx.DspCtx[dspID];
    pDspCtx->bAsyncMode = FALSE;
}

void DspDrvSetAsynMode(UINT32 dspID)
{
    PDSP_CTX pDspCtx = (PDSP_CTX)&DspDrvCtx.DspCtx[dspID];
    pDspCtx->bAsyncMode = TRUE;
}


//-------------------------------------------------------------------------------------------
/** @brief This function tests DSP at system Init. Sends a known function encoded in Ceva with
 *         task ID \ref TASKID_COPY_DATA. It runs the Ceva in a blocking mode. Once the Ceva
 *         function verifies that the operation was completed and displays status of Ceva
 *
 *
 *  @param   dspID ID of DSP to run test on
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------
MXRC DspDrvBist(UINT32 dspID)
{
    PDSP_CTX pDspCtx = (PDSP_CTX)&DspDrvCtx.DspCtx[dspID];
    UINT32 res=1;
    PTCB pTcb = &dsp_tcb [dspID];
    MXRC rc;

    memset(pTcb, 0, sizeof(*pTcb));

    pTcb->InputDataLen = 4096;
    pTcb->OutputDataLen= 4096;
    pTcb->TaskID = TASKID_COPY_DATA;

    pTcb->InputDataPtr = MxHeapAlloc(DspDrvCtx.hDspHeap, pTcb->InputDataLen);
    _ASSERT_PTR(pTcb->InputDataPtr);

    pTcb->OutputDataPtr = MxHeapAlloc(DspDrvCtx.hDspHeap, pTcb->OutputDataLen);
    _ASSERT_PTR(pTcb->OutputDataPtr);

    memset(pTcb->InputDataPtr, 0x12, pTcb->InputDataLen);
    memset(pTcb->OutputDataPtr,0xFF, pTcb->OutputDataLen);

    pDspCtx->bAsyncMode = FALSE;

    rc = DspDrvRunBlocking(dspID, pTcb);

    if (SUCCESSFUL(rc))
    {
        res = memcmp(pTcb->InputDataPtr, pTcb->OutputDataPtr, MIN(pTcb->InputDataLen, pTcb->OutputDataLen));

        if (res == 0)
        {
            if (dspID != DSP_DRV_DEF_REX_SERV)
            {
                uart_printf ("[passed] {ticks:%d}\r\n", pTcb->ExecTicks);
            }
            else
            {
                uart_printf ("[passed] {REX-SERV}\r\n");
            }
        }
        else
        {
            uart_printf ("[failed]\r\n");
            rc = RC_DSP_DRV_BISR_FAILED;
         }
    }
    else
    {
        uart_printf ("Run TCB is failed, rc=%x\r\n", rc);
    }

    MxHeapFree(DspDrvCtx.hDspHeap, pTcb->InputDataPtr);
    MxHeapFree(DspDrvCtx.hDspHeap, pTcb->OutputDataPtr);

    return rc;
}



//-------------------------------------------------------------------------------------------
/** @brief This function tests DSP's DMA functioanlity at system Init. Sends a known function
 *         encoded in Ceva with task IDs \ref TASKID_CPY_FROM_EXT_MEM and \ref TASKID_CPY_TO_EXT_MEM.
 *         It runs the Ceva in a blocking mode. Once the Ceva function verifies that the operation
 *         was completed and displays status of Ceva DMA
 *
 *  @param   dspID     ID of DSP to run test on
 *  @param   ext_addr  Address of External Memory to Read / Write
 *  @param   int_addr  Address of Ceva Internal Memory to Read / Write
 *  @param   size      Length of buffers to read / write
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------
MXRC DspDrvBistDmaUtil(UINT32 dspID, UINT32 ext_addr, UINT32 int_addr, UINT32 size)
{
    PDSP_CTX pDspCtx = (PDSP_CTX)&DspDrvCtx.DspCtx[dspID];
    TCB tcb;
    PTCB pTcb = dsp_tcb;

    memset(&tcb, 0, sizeof(TCB));

    tcb.InputDataPtr = (void*)ext_addr;
    tcb.OutputDataPtr = (void*)int_addr;
    tcb.InputDataLen = size;
    tcb.OutputDataLen= size;
    tcb.TaskID = TASKID_CPY_FROM_EXT_MEM;

    memset(tcb.InputDataPtr, 0x12, tcb.InputDataLen);

    pDspCtx->bAsyncMode = FALSE;

    if (DspDrvRunBlocking(dspID, &tcb) == MX_OK)
    {
        VUINT32 count = 0;

        // in case of Non-Blocking wait and enabled DCache we need to wait status
        // because the code will run so fast

        while ((pTcb->Status != STATUS_COMPLETE) || (pTcb->Status <= STATUS_QUED))
        {
            if (++count > 1000000)
               break;
        }
    }

    if (pTcb->Status != STATUS_COMPLETE)
    {
        UINT32 i = 1;
        uart_printf ("DspDMAUtilTest fail ext 0x%08X, int 0x%08X size %u res %u\r\n", ext_addr, int_addr, size, pTcb->Status - STATUS_QUED);
        while(i);
    }

    memset(&tcb, 0, sizeof(TCB));

    tcb.InputDataPtr = (void*)int_addr;
    tcb.OutputDataPtr = (void*)ext_addr;
    tcb.InputDataLen = size;
    tcb.OutputDataLen= size;
    tcb.TaskID = TASKID_CPY_TO_EXT_MEM;

    //memset(tcb.OutputDataPtr, 0xAA, tcb.OutputDataLen);

    if (DspDrvRunBlocking(dspID, &tcb) == MX_OK)
    {
        VUINT32 count = 0;

        // in case of Non-Blocking wait and enabled DCache we need to wait status
        // because the code will run so fast

        while ((pTcb->Status != STATUS_COMPLETE) || (pTcb->Status <= STATUS_QUED))
        {
            if (++count > 1000000)
               break;
        }
    }

    {
        UINT32 i = 0;
        do
        {
            if((i+4 < size) && (*((UINT32*)tcb.OutputDataPtr+i) == 0x12121212))
            {
                i += 4;
                continue;
            }
            else if ((i+2 < size) && (*((UINT16*)tcb.OutputDataPtr+i) == 0x1212))
            {
                i += 2;
                continue;
            }
            else if ((i < size) && (*((UINT8*)tcb.OutputDataPtr+i) == 0x12))
                ++i;
            else
            {
                uart_printf ("DspDMAUtilTest fail ext 0x%08X, int 0x%08X size %u res %u\r\n", ext_addr, int_addr, size, pTcb->Status - STATUS_QUED);
                while(++i);
            }
        }
        while (i < size);
    }

    if (pTcb->Status == STATUS_COMPLETE)
        uart_printf ("DspDMAUtilTest ok size %u. SPU %d Ticks: %d\r\n", size, dspID, pTcb->ExecTicks);

    return MX_OK;
}




//-------------------------------------------------------------------------------------------
/** @brief This function sets a DSP call back function to return to once Ceva finishes running
 *         (in a non-blocking form) and iterrupts the ARM
 *
 *
 *  @param   dspID       Id of Ceva [0-9]
 *  @param   cbReqComp   Callback Function Poitner
 *  @param   pReqCompCtx Callback Parameter
 *
 *  @return  none
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------
void DSPDrvSetCallBack(UINT32 dspID, void (*cbReqComp)(void *p), void *pReqCompCtx)
{
    PDSP_CTX pDspCtx = (PDSP_CTX)&DspDrvCtx.DspCtx[dspID];

    pDspCtx->cbReqComp = cbReqComp;
    pDspCtx->hReqCompCtx = pReqCompCtx;
    pDspCtx->bAsyncMode = (cbReqComp) ? TRUE : FALSE;
}

//-------------------------------------------------------------------------------------------
/** @brief This function dispatches the TCB to DSP in a non-blocking form
 *
 *
 *  @param   dspID DSP id
 *  @param   *pTCB Pointer to TCB
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------
MXRC DspDrvRun(UINT32 dspID, TCB *pTCB)
{
    PTCB nextTCB;
    PDSP_CTX pDspCtx = (PDSP_CTX)&DspDrvCtx.DspCtx[dspID];

    _ASSERT (dspID < DEVINFO_GET_TABLE_VAL(DEVID_IDX, DEVID_DSP_CORE));

    nextTCB= pTCB;

#ifdef MLOG_ENABLED
    while (nextTCB)
    {
        MLogEXEStart(nextTCB,dspID,0); //here so that it captures the superTCB //put it here this is when driver sees it
        nextTCB = nextTCB->NextSupTcb;//nextTCB = nextTCB->NextTcb;
    }
#endif

    SetDspTCB (dspID, pTCB);
    pDspCtx->pExecTcb =  pTCB;

    DspDrvAddToLog(pDspCtx, pTCB);
    REG32(SET_IPI_REG) = (1 << dspID);

    return MX_OK;
}

MXRC DspDrvRunEx(UINT32 dspID, TCB *pTCB, UINT32 nMlog)
{
    PTCB nextTCB;
    PDSP_CTX pDspCtx = (PDSP_CTX)&DspDrvCtx.DspCtx[dspID];

    _ASSERT (dspID < DEVINFO_GET_TABLE_VAL(DEVID_IDX, DEVID_DSP_CORE));

    nextTCB= pTCB;

    if (nMlog & 1)
    {
        while (nextTCB)
        {
            MLogEXEStart(nextTCB,dspID,0); //here so that it captures the superTCB //put it here this is when driver sees it
            nextTCB = nextTCB->NextSupTcb;//nextTCB = nextTCB->NextTcb;
        }
    }

    SetDspTCB (dspID, pTCB);
    pDspCtx->pExecTcb =  pTCB;

    DspDrvAddToLog(pDspCtx, pTCB);
    REG32(SET_IPI_REG) = (1 << dspID);

    return MX_OK;
}


//-------------------------------------------------------------------------------------------
/** @brief This function dispatches the TCB to DSP in a blocking form
 *
 *
 *  @param   dspID DSP id
 *  @param   *pTCB Pointer to TCB
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------
MXRC DspDrvRunBlocking(UINT32 dspID, TCB *pTCB)
{
    PDSP_CTX pDspCtx = (PDSP_CTX)&DspDrvCtx.DspCtx[dspID];
    MXRC rc = RC_DSP_DRV_SPU_TIMEOUT;
    volatile UINT32 *pStatus;

    _ASSERT (dspID < DEVINFO_GET_TABLE_VAL(DEVID_IDX, DEVID_DSP_CORE));

#ifdef  _TCB_CACHED
    if (pTCB)
        MxCacheClean (pTCB, sizeof (TCB) + pTCB->ContextLen + pTCB->IOControlLen);

    if (pTCB->ContextPtr != NULL)
           MxCacheClean(pTCB->ContextPtr,pTCB->ContextLen); //clean context

    if (pTCB->IOControlPtr != NULL)
        MxCacheClean(pTCB->IOControlPtr,pTCB->IOControlLen); //clean what ioptr is pointing to
#endif

    SetDspTCB(dspID, pTCB);
    pDspCtx->pExecTcb =  pTCB;

    DspDrvAddToLog(pDspCtx, pTCB);

    pStatus = &pTCB->Status;

    pTCB->Status = 0;
    pTCB->ExecTicks = 0;

    // Generate INT0 interrupts to CEVA
    REG32(SET_IPI_REG) = (1 << dspID);

    pDspCtx->StartTick = Get24MHzTick();

    while (GetExecBusTicks(pDspCtx->StartTick) <= DSP_RUN_TIMEOUT)
   // while (1)
    {
        if (*pStatus==STATUS_COMPLETE)
        {
            MLogEXEFinish(pTCB);

            if (pTCB->NextTcb)
            {
                pTCB= pTCB->NextTcb;
                pTCB->Status = STATUS_RUNNING; //started
                MLogEXEStart(pTCB,dspID,0);
            }

            return MX_OK;
        }
    }

    return rc;
}

MXRC DspDrvRunInPullMode(UINT32 dspID, TCB *pTCB)
{
    PDSP_CTX pDspCtx = (PDSP_CTX)&DspDrvCtx.DspCtx[dspID];
    MXRC rc = RC_DSP_DRV_SPU_TIMEOUT;
    volatile UINT32 *pStatus;

    _ASSERT (dspID < DEVINFO_GET_TABLE_VAL(DEVID_IDX, DEVID_DSP_CORE));

#ifdef  _TCB_CACHED
    if (pTCB)
        MxCacheClean (pTCB, sizeof (TCB) + pTCB->ContextLen + pTCB->IOControlLen);

    if (pTCB->ContextPtr != NULL)
           MxCacheClean(pTCB->ContextPtr,pTCB->ContextLen); //clean context

    if (pTCB->IOControlPtr != NULL)
        MxCacheClean(pTCB->IOControlPtr,pTCB->IOControlLen); //clean what ioptr is pointing to
#endif

    SetDspTCB(dspID, pTCB);
    pDspCtx->pExecTcb =  pTCB;

    DspDrvAddToLog(pDspCtx, pTCB);

    pStatus = &pTCB->Status;

    pTCB->Status = TCB_CFG_STAT_DIS_INT;
    pTCB->ExecTicks = 0;

    // Generate INT0 interrupts to CEVA
    REG32(SET_IPI_REG) = (1 << dspID);

    pDspCtx->StartTick = Get24MHzTick();

    while (GetExecBusTicks(pDspCtx->StartTick) <= DSP_RUN_TIMEOUT)
    {
        if ((*pStatus) & TCB_CFG_STAT_MARK_DONE)
        {
            rc = MX_OK;
            break;
        }
    }

    return rc;
}


//-------------------------------------------------------------------------------------------
/** @brief This function loads DSP program memory with a Ceva Binary Image
 *
 *  @param   nCevaID       the specific CEVA ID
 *  @param   pDspImage     Pointer to program to load
 *  @param   DspImageSize  Size of program to load
 *  @param   timeout       System Timeout interval (after which it will display load error)
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 DspDrvBootCeva(UINT32 nCevaID, PUINT8 pDspImage, UINT32 DspImageSize, UINT32 timeout)
{
    UINT32 rc = 0;
    UINT32 DevID;
    UINT32 DevProcMask = 0;
    UINT32 nDesc = 0;
    BOOL next_desc;
    PDSPBOOTDESC p_src, p_dst;
    UINT32 ExtCodeDataOffset;
    UINT32 StartTick =0;
    UINT32 Length=0;
    PTCB pTcb;

    DspDrvResetCeva(nCevaID);

    memset (&dsp_tcb[nCevaID], 0, sizeof (TCB));
    memset ((LPVOID)DSP_BOOT_DESC_ADDR, 0, DSP_EXT_SEGMENTS - DSP_BOOT_DESC_ADDR);

    REG32(SET_IPI_REG)    = 0;
    REG32(CLEAR_IPI_REG)  = 0xFFFF; // to clear all interrupts

    _NOP();
    _NOP();

    p_src = (PDSPBOOTDESC)pDspImage;
    p_dst = (PDSPBOOTDESC)DSP_BOOT_DESC_ADDR;

    do
    {
        next_desc = p_src->next;
        *p_dst = *p_src++;
        p_dst->external += (UINT32)pDspImage;
        if (next_desc)
            p_dst->next += DSP_BOOT_DESC_ADDR;
        Length += p_dst->length;
        p_dst++;
        nDesc++;
    } while (next_desc);

    ExtCodeDataOffset = 0x800;

    // copy external code section to CRAM at 0xFC001000 address
    memcpy((PUINT8)DSP_EXT_SEGMENTS, &pDspImage[ExtCodeDataOffset], ExtCodeDataLen);

    *(PUINT32)SPU_CEVA_BOOT_ADDR = DSP_EXT_SEGMENTS;

    DevID = nCevaID;
    DevProcMask = 0;
    rc = MX_OK;

    _ASSERT_PTR(DspDrvCtx.pHeadTcb);
    _ASSERT_PTR(DspDrvCtx.pHeadTcb->CevaTcb[DevID]);

    // to reset the loading flags
    DspDrvCtx.pHeadTcb->Beef = 0;
    DspDrvCtx.pHeadTcb->Dead = 0;

    DevProcMask |= (1 << DevID);
    DspDrvTurnOnDSP(DevID);

    pTcb = DspDrvCtx.pHeadTcb->CevaTcb[DevID];

    if (timeout)
       StartTick = Get24MHzTick();

    while (TRUE)
    {
        if (pTcb->Status == STATUS_COMPLETE)
        {
            DspDrvCtx.DevUpMask |= 1<<DevID;
            break;
        }
        else if (timeout && (Get24MHzTick() - StartTick) > timeout)
        {
            rc = RC_DSP_DRV_BOOT_FAILED;
            uart_printf(" * DSP boot [%d] timeout code status:[0x%x 0x%x]\r\n", DevID, DspDrvCtx.pHeadTcb->Dead, DspDrvCtx.pHeadTcb->Beef);
            break;
        }
    }

    return rc;
}


//-------------------------------------------------------------------------------------------
/** @brief This function loads Each DSP's program memory with a Ceva Binary Image
 *
 *
 *  @param   pDspImage     Pointer to program to load
 *  @param   DspImageSize  Size of program to load
 *  @param   timeout       System Timeout interval (after which it will display load error)
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 DspDrvBoot(PUINT8 pDspImage, UINT32 DspImageSize, UINT32 timeout)
{
    UINT32 rc = 0;
    UINT32 DevID;
    UINT32 DevProcMask = 0;
    UINT32 nDesc = 0;
    BOOL next_desc;
    PDSPBOOTDESC p_src, p_dst;
    UINT32 ExtCodeDataOffset;
    UINT32 StartTick =0;
    UINT32 Length=0, i;
    PTCB pTcb;

    if (DspDrvRexServCeva != 0xFF)
    {
        DspDrvResetAllDSPExcept(1 << DspDrvRexServCeva);

        for (i = 0; i < sizeof(dsp_tcb)/sizeof (dsp_tcb[0]); i++)
        {
            if (i != DspDrvRexServCeva)
                memset (&dsp_tcb[i], 0, sizeof (dsp_tcb[0]));
        }
    }
    else
    {
        memset (dsp_tcb, 0, sizeof (dsp_tcb));
        DspDrvResetDSPAll();
    }

    REG32(SET_IPI_REG)    = 0;
    //REG32(IPI_STATUS_REG) = 0xFFFF; // to clear all interrupts
    REG32(CLEAR_IPI_REG)  = 0xFFFF; // to clear all interrupts

    _NOP();
    _NOP();

    p_src = (PDSPBOOTDESC)pDspImage;
    p_dst = (PDSPBOOTDESC)DSP_BOOT_DESC_ADDR;

    do
    {
        next_desc = p_src->next;
        *p_dst = *p_src++;
        p_dst->external += (UINT32)pDspImage;
//        uart_printf("ext_add 0x%08X int_add 0x%08X iscode %u length %u \n", p_dst->external, p_dst->internal, p_dst->code, p_dst->length);
        if (next_desc)
            p_dst->next += DSP_BOOT_DESC_ADDR;
        Length += p_dst->length;
        p_dst++;
        nDesc++;
    } while (next_desc);

    ExtCodeDataOffset = 0x800;

    // copy external code section to CRAM at 0xFC001000 address
    memcpy((PUINT8)DSP_EXT_SEGMENTS, &pDspImage[ExtCodeDataOffset], ExtCodeDataLen);

    *(PUINT32)SPU_CEVA_BOOT_ADDR = DSP_EXT_SEGMENTS;

    DspDrvCtx.DevEnaMask = 0;
    DspDrvCtx.DevUpMask = 0;

    for (DevID = DevInfoGetValue(CONSTID_DSP_STARTID); DevID < DspDrvCtx.numDSP; DevID++)
    {
        DspDrvCtx.DevEnaMask |= 1 << DevID;
        DspDrvCtx.pHeadTcb->CevaTcb [DevID] = &(dsp_tcb[DevID]);
    }

    DevID = 0;
    DevProcMask = 0;
    rc = MX_OK;

    // wait till all DSPs completed their initialization
    while ((DspDrvCtx.DevEnaMask != DevProcMask))
    {
        if ((DspDrvCtx.DevEnaMask & (1 << DevID)) == 0 || DspDrvRexServCeva == DevID)
        {
            if (DspDrvRexServCeva == DevID)
            {
                DevProcMask |= (1 << DevID);
                DspDrvCtx.DevUpMask |= (DspDrvCtx.RexServLoadErr == 0) ? (1<<DevID) : 0;
            }

            DevID ++;
            continue;
        }

        _ASSERT_PTR(DspDrvCtx.pHeadTcb);
        _ASSERT_PTR(DspDrvCtx.pHeadTcb->CevaTcb[DevID]);

        // to reset the loading flags
        DspDrvCtx.pHeadTcb->Beef = 0;
        DspDrvCtx.pHeadTcb->Dead = 0;

        DevProcMask |= (1 << DevID);
        DspDrvTurnOnDSP(DevID);

        pTcb = DspDrvCtx.pHeadTcb->CevaTcb[DevID];

        if (timeout)
           StartTick = Get24MHzTick();

        while (TRUE)
        {
            if (pTcb->Status == STATUS_COMPLETE)
            {
                DspDrvCtx.DevUpMask |= 1<<DevID;
                break;
            }
            else if (timeout && (Get24MHzTick() - StartTick) > timeout)
            {
                rc = RC_DSP_DRV_BOOT_FAILED;
                uart_printf(" * DSP boot [%d] timeout code status:[0x%x 0x%x]\r\n", DevID, DspDrvCtx.pHeadTcb->Dead, DspDrvCtx.pHeadTcb->Beef);
                break;
            }
        }

        DevID ++;
    }

    if (DspDrvCtx.DevEnaMask != DspDrvCtx.DevUpMask)
    {
        uart_printf ("\r\nBad CEVAs mask is [0x%X]\r\n", DspDrvCtx.DevEnaMask ^ DspDrvCtx.DevUpMask);
    }

    // no CEVAs at all
    if (DspDrvCtx.DevUpMask == 0)
        return RC_DSP_DRV_BOOT_FAILED;

    return rc;
}

UINT32 DspDrvGetDspHeapAddr(void)
{
    UINT32 ExtCodeDataOffset = 0x800;
    #ifndef MSEVM
        ExtCodeDataLen = ((PDSPBOOTDESC)cevabincode)->external - ExtCodeDataOffset;
    #else
        ExtCodeDataLen = ((PDSPBOOTDESC)cevamsbincode)->external - ExtCodeDataOffset;
    #endif

    return DSP_EXT_SEGMENTS + ROUND(ExtCodeDataLen, 32);
}

UINT32 DspDrvGetDspHeapSize(UINT32 DspHeapAddr)
{
    UINT32 Size = 0;
    UINT32 CramAvailable = DevInfoGetSize(MEMID_CRAM);

    if(CramAvailable >= (DspHeapAddr - CRAM_BASEADDR))
    {
        Size += CramAvailable - (DspHeapAddr - CRAM_BASEADDR);
    }

    return Size;
}

MXRC DspDrvDumpNoReset (UINT32 nCevaID, PUINT8 pOutBuf, PUINT32 pnSize)
{
    PDSP_CTX pDspCtx = (PDSP_CTX)&DspDrvCtx.DspCtx[nCevaID];
    PTCB pTcb = dsp_tcb;
    UINT32 nTick;

    _ASSERT (nCevaID < DEVINFO_GET_TABLE_VAL(DEVID_IDX, DEVID_DSP_CORE));
    _ASSERT_PTR (pOutBuf);
    _ASSERT_PTR (pnSize);

    if (*pnSize < sizeof(TCB))
        return RC_DSP_DRV_DUMP_BUF_SO_SMALL;

    // To store the current TCB used before CEVA crash/dump

    memcpy (pOutBuf, pTcb, HW_TCB_SIZE);

    memset (pTcb, 0, sizeof (*pTcb));
    pTcb->TaskID = 12345678;
    pTcb->InputDataPtr =  (pOutBuf + HW_TCB_SIZE);
    pTcb->OutputDataPtr = (pOutBuf + HW_TCB_SIZE);
    pTcb->InputDataLen =  (*pnSize - HW_TCB_SIZE);
    pTcb->OutputDataLen= 0;

    // to repare the CEVA dump TCB
    pDspCtx->bAsyncMode = FALSE;

    // Generate INT0 interrupts to CEVA
    DspDrvRunBlocking(nCevaID, pTcb);

    // to wait the status
    nTick = GetTIMETICK ();
    while (pTcb->Status == 0)
    {
        if ((GetTIMETICK () - nTick) > 150000000)
        {
            *pnSize = 0;
            uart_printf("DSP%d TimeOut\r\n", nCevaID);
            return RC_DSP_DRV_TIMEOUT;
        }
    }

    *pnSize = pTcb->OutputDataLen;

    uart_printf("Done: Status=%X Len=%d\r\n", pTcb->Status, *pnSize);

    // restore pTcb memory
    memcpy (pTcb, pOutBuf, HW_TCB_SIZE);

    return MX_OK;
}

//-------------------------------------------------------------------------------------------
/** @brief This function dumps status of Ceva registers to specified memroy
 *
 *
 *  @param   nCevaID  CEVA ID
 *  @param   pOutBuf  Pointer to Output
 *  @param   pnSize   Pointer to size of Ouptut
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------
MXRC DspDrvDump (UINT32 nCevaID, PUINT8 pOutBuf, PUINT32 pnSize)
{
    TCB *pTcb;
    UINT32 nTick;
    UINT32* pMagicNum = (UINT32*)(CRAM_BASEADDR + 0*nCevaID);

    _ASSERT (nCevaID < DEVINFO_GET_TABLE_VAL(DEVID_IDX, DEVID_DSP_CORE));
    _ASSERT_PTR (pOutBuf);
    _ASSERT_PTR (pnSize);

    if (*pnSize < sizeof(TCB))
        return RC_DSP_DRV_DUMP_BUF_SO_SMALL;

#if 0
    // Before using DspDrvDumpNoReset() the following code should be added into main() of CEVA
    // After this the both arrays may be compared to see that CEVA DRAM is mostly the same
/*
    else if(TaskID == 12345678)
    {
        U32* pOutData = pTCB->OutputDataPtr;
        U32 OutBufferSpace = pTCB->InputDataLen;

        if(OutBufferSpace)
        {
            MemCpyToExt(pOutData, (void *) 0, OutBufferSpace);
        }

        pTCB->OutputDataLen = OutBufferSpace;

        Status = STATUS_COMPLETE;
    }
*/
    {
        PUINT8 pBufNoReset;
        UINT32 SizeInp = *pnSize;

        pBufNoReset = MxHeapAlloc(NcnbHeap, SizeInp);
        if(pBufNoReset)
        {// perform no reset dump, if asked CEVA isn't hanging
            uart_printf("Do NoResetDump: to buffer 0x%X size=%d\r\n", pBufNoReset, SizeInp);
            DspDrvDumpNoReset(nCevaID, pBufNoReset, &SizeInp);
        }
    }
#endif
    uart_printf("Do AfterResetDump: 0x%X size=%d\r\n", pOutBuf, *pnSize);
    // Reset the CEVA

    DspDrvResetDSPAll();

    // To store the current TCB used before CEVA crash/dump
    pTcb = (GetDspTCB (nCevaID));

	if(!pTcb)
		return RC_DSP_DRV_ALLOC_ERROR;

    if (pTcb != NULL)
        memcpy (pOutBuf, pTcb, HW_TCB_SIZE);
    else
        memset (pOutBuf, 0x00, HW_TCB_SIZE);

    // Will check magic Word here
    *pMagicNum = 0;

    // to repare the CEVA dump TCB
    //pTcb = dsp_tcb;

    memset (pTcb, 0, sizeof (*pTcb));
    pTcb->InputDataPtr =  (pOutBuf + HW_TCB_SIZE);
    pTcb->OutputDataPtr = (pOutBuf + HW_TCB_SIZE);
    pTcb->InputDataLen =  (*pnSize - HW_TCB_SIZE);
    pTcb->OutputDataLen= 0;

    // Set CEVA out of reset
// use separate CEVA dmp image
    {
        PDSPBOOTDESC p_src, p_dst;
        PUINT8 pDspImage = (PUINT8)cevabincodedmp;
        UINT32 Length=0;
        UINT32 nDesc = 0;
        BOOL next_desc;
        UINT32 ExtCodeDataOffset;
        UINT32 ExtCodeDataLen;

        p_src = (PDSPBOOTDESC)pDspImage;
        p_dst = (PDSPBOOTDESC)DSP_BOOT_DESC_ADDR;

        do
        {
            next_desc = p_src->next;
            *p_dst = *p_src++;
            p_dst->external += (UINT32)pDspImage;
            uart_printf("ext_add 0x%08X int_add 0x%08X iscode %u length %u \n", p_dst->external, p_dst->internal, p_dst->code, p_dst->length);
            if (next_desc)
                p_dst->next += DSP_BOOT_DESC_ADDR;
            Length += p_dst->length;
            p_dst++;
            nDesc++;
        } while (next_desc);

        ExtCodeDataOffset = 0x800;

        ExtCodeDataLen = ((PDSPBOOTDESC)pDspImage)->external - ExtCodeDataOffset;

        // copy external code section to CRAM at 0xFC001000 address
        memcpy((PUINT8)DSP_EXT_SEGMENTS, &pDspImage[ExtCodeDataOffset], ExtCodeDataLen);
    }

    *(PUINT32)SPU_CEVA_BOOT_ADDR = DSP_EXT_SEGMENTS;

    SetDspTCB (nCevaID, pTcb);

    uart_printf("MagicNum=%X Status=%X pTCB=%X\r\n", *pMagicNum, pTcb->Status, pTcb);

    DspDrvTurnOnDSP(nCevaID);

    // to wait the status
    nTick = GetTIMETICK ();
    while (pTcb->Status == 0)
    {
        if ((GetTIMETICK () - nTick) > 150000000)
        {
            *pnSize = 0;
            uart_printf("DSP%d TimeOut\r\n", nCevaID);
            return RC_DSP_DRV_TIMEOUT;
        }
    }

    *pnSize = pTcb->OutputDataLen;

    uart_printf("Done: MagicNum=%X Status=%X Len=%d\r\n", *pMagicNum, pTcb->Status, *pnSize);

    return MX_OK;
}

/** @brief This function generates NMI to all cevas.

    @param nExcMask [in] - is used to exclude some CEVA from the dump pool, by default = 0
    @param pDoneMask[out]- the mask of cevas completed dump operation

    @return [MXRC] an error code

    \ingroup group_lte_hal_dsp

    */

MXRC DspDrvGetDump (UINT32 nExcMask, PUINT32 pDoneMask)
{
    UINT32 t, i, done_mask, check_mask;
    PUINT32 dump_ptr [DEVID_MAX_DSP_CORE];

    if (DspDrvCtx.DspCtx[0].pDumpPtr == NULL)
        return RC_DSP_DRV_DUMP_PTR_ERROR;

    REG32(SET_NMI_REG) = (DspDrvCtx.DevUpMask & (~nExcMask));

    // in this case the interrupt was generated to CEVA,
    // and we can run some ARM code while CEVAs generate dump

    check_mask = 0;

    for (i = 0; i < DspDrvCtx.numDSP; i++)
    {
        if (!(nExcMask & (1 << i)))
        {
            check_mask |= (1 << i);
            dump_ptr [i] = (PUINT32)((PUINT8)DspDrvCtx.DspCtx[i].pDumpPtr + DSP_DUMP_SIZE - 4);
        }
        else
            dump_ptr [i] = NULL;
    }

    t = MxGetTicks ();

    done_mask = 0;

    // if < 10ms
    while (MxGetTicks () - t < 150000 * 20)
    {
        for (i = 0; i < DspDrvCtx.numDSP; i++)
        {
            if (dump_ptr [i] != NULL)
            {
                if (*(dump_ptr[i]) != 0xA5A5A5A5)
                {
                    done_mask |= (1 << i);
                }
            }
        }

        if (done_mask == check_mask)
            break;
    }

    if (pDoneMask != NULL)
       *pDoneMask = done_mask;

    return (done_mask == check_mask) ? MX_OK : RC_DSP_DRV_DUMP_TIMEOUT;
}

/** @brief This function returns address of dump storage.

    @param nDspID [in] - ceva code id:{0...max_num}

    @return [LPVOID]  the dump storage

    \ingroup group_lte_hal_dsp

    */
LPVOID DspDrvGetDumpAddr(UINT32 nDspID)
{
    if (DspDrvCtx.DspCtx[0].pDumpPtr == NULL)
        return NULL;

    return ((PUINT8)DspDrvCtx.DspCtx[0].pDumpPtr) + nDspID*DSP_DUMP_SIZE;
}

MXRC  DspDrvSetLog(UINT32 nCevaID,  LPVOID pStorage, UINT32 nSize)
{
    return LogMgrCreateLogEx(LOG_CTRL_MEMCPY, pStorage, nSize, (LOGHANDLE*)&DspDrvCtx.DspCtx[nCevaID].hLog);
}

MXRC  DspDrvCloseLog(UINT32 nCevaID)
{
    PDSP_CTX pDspCtx = (PDSP_CTX)&DspDrvCtx.DspCtx[nCevaID];

    pDspCtx->LogMask = 0;

    return LogMgrCloseLog(pDspCtx->hLog);
}

void  DspDrvSetLogMask(UINT32 nCevaID, UINT32 mask)
{
    PDSP_CTX pDspCtx = (PDSP_CTX)&DspDrvCtx.DspCtx[nCevaID];
    pDspCtx->LogMask = mask;
}

MXRC DspDrvAddToLog(PDSP_CTX pDspCtx, PTCB pTcb)
{
    MXRC rc = 0;

    if ( !pDspCtx->LogMask )
        return MX_OK;

    rc = LogMgrWriteData (pDspCtx->hLog, pTcb, HW_TCB_SIZE);
    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    if (pDspCtx->LogMask & DSP_LOG_IOCTRL)
    {
        rc = LogMgrWriteData(pDspCtx->hLog, &pTcb->IOControlLen, sizeof(pTcb->IOControlLen));
        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        if (pTcb->IOControlLen && (pTcb->IOControlPtr != NULL))
        {
            rc = LogMgrWriteData(pDspCtx->hLog, pTcb->IOControlPtr, pTcb->IOControlLen);
            if (FAILED(rc))
            {
                _ASSERT_RC(rc);
                return rc;
            }
        }
    }

    if (pDspCtx->LogMask & DSP_LOG_CTX)
    {
        rc = LogMgrWriteData(pDspCtx->hLog, &pTcb->ContextLen, sizeof(pTcb->ContextLen));
        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        if (pTcb->ContextLen && (pTcb->ContextPtr != NULL))
        {
            rc = LogMgrWriteData(pDspCtx->hLog, pTcb->ContextPtr, pTcb->ContextLen);
            if (FAILED(rc))
            {
                _ASSERT_RC(rc);
                return rc;
            }
        }
    }

    if (pDspCtx->LogMask & DSP_LOG_INP)
    {
        rc = LogMgrWriteData(pDspCtx->hLog, &pTcb->InputDataLen, sizeof(pTcb->InputDataLen));
        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        if (pTcb->InputDataLen && (pTcb->InputDataPtr != NULL))
        {
            rc = LogMgrWriteData(pDspCtx->hLog, pTcb->InputDataPtr, pTcb->InputDataLen);
            if (FAILED(rc))
            {
                _ASSERT_RC(rc);
                return rc;
            }
        }
    }

    if (pDspCtx->LogMask & DSP_LOG_OUT)
    {
        rc = LogMgrWriteData(pDspCtx->hLog, &pTcb->OutputDataLen, sizeof(pTcb->OutputDataLen));
        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        if (pTcb->OutputDataLen && (pTcb->OutputDataPtr != NULL))
        {
            rc = LogMgrWriteData(pDspCtx->hLog, pTcb->OutputDataPtr, pTcb->OutputDataLen);
            if (FAILED(rc))
            {
                _ASSERT_RC(rc);
                return rc;
            }
        }
    }

    return rc;
}

/** @brief This function returns the mask of available CEVAs
           if drivers is inited, the mask contains the devices with passed BISR test

    @return [UINT32] the mask of CEVAs
     \ingroup group_lte_hal_dsp
     */

UINT32 DspDrvGetDevMask(void)
{
    UINT32 mask = 0;

    if (DspDrvCtx.bInitialized == 0)
    {
        UINT32 DspNum = DevInfoGetNumber(DEVID_DSP_CORE);
        UINT32 MaskAll = (1 << DspNum) - 1;

        if (DevInfoIsT4Kx4 ())
            mask = (MaskAll & 0x3FE);
        else
            mask = (MaskAll & ~(DSP_DRV_EXCLUDE_LIST));
    }
    else
    {
        mask = DspDrvCtx.DevUpMask & ~(DSP_DRV_EXCLUDE_LIST);
    }

    if (DspDrvRexServCeva != 0xFF)
    {
        // to exclude the REX-SERV CEVA from the
        // pool of CEVAs used by the PHY code

        mask &= ~(1 << DspDrvRexServCeva);
    }

    return mask;
}

/** @brief This function is designed to run BIST test for all detected CEVAs
           and planned to be used just for checking ceva pool

    @return [MXRC] an error code */

MXRC DspDrvCevaPoolBist (void)
{
    UINT32 nDev = 0;
    MXRC rc = MX_OK;

    if (DspDrvCtx.bInitialized == 0 || DspDrvCtx.DevUpMask == 0)
        return RC_DSP_DRV_NO_HW_FOUND;

    if (DspDrvCtx.CpuID != MxGetCpuID ())
    {
        uart_printf ("DSP driver is initialized on CORE-%d\r\n", DspDrvCtx.CpuID);
        return RC_DSP_DRV_NO_HW_FOUND;
    }

    while (nDev < 10)
    {
        if (DspDrvCtx.DevUpMask & (1 << nDev))
        {
            uart_printf ("CEVA[%d] bisr ... ", nDev);
            rc |= DspDrvBist (nDev);
        }

        nDev ++;
    }

    return rc;
}

/** @brief This function delivers TCB to the REX-SERVER side

    @param pTcb [in] - the pointer to the REX-SERVER TCB
    @param bWait[in] - to wait completeness of the TCB

    @return [MXRC] an error code */

MXRC DspDrvSendToRexServ(TCB * pTcb, UINT32 bWait)
{
    DSPDRV_CTX * pCtx = &DspDrvCtx;
    MXRC rc;

    _ASSERT_PTR(pTcb);

    if (pCtx->RexServID == 0xFF)
        return RC_DSP_DRV_NO_REX_SERVER;

    if (bWait)
    {
        rc = DspDrvRunInPullMode(pCtx->RexServID, pTcb);
    }
    else
    {
        rc = DspDrvRun(pCtx->RexServID, pTcb);
    }

    return rc;
}

/** @brief This function sets CEVA index where REX-SERVER code will be located
          this function has to be called before DspDrvInit

    @param dsp_id [in] - the CEVA devive id:[0...9]

    @return [MXRC] an error code */

MXRC DspDrvSetRexServCeva(UINT32 dsp_id)
{
    DspDrvRexServCeva = dsp_id;
    return MX_OK;
}

/** @brief This function sends interrupt to the CEVA processor
           to start processing some task prepared by the REX-client

    @return [MXRC] an error code

*/

MXRC DspDrvNotifyRex(void)
{
    if (DspDrvRexServCeva != 0xFF)
        REG32(SET_IPI_REG) = (1 << DspDrvRexServCeva);

    return MX_OK;
}

UINT32 DspDrvGetRexCeva (void)
{
    return DspDrvRexServCeva;
}

void DspDrvRegRexCb (void (*Proc)(void*), void * ptr)
{
    DspDrvRexServCevaCbPtr = ptr;
    DspDrvRexServCevaCb    = Proc;
}

CEVA_DRV_CFG * DspDrvGetCevaDrvCfgParams(void)
{
    DSP_INTERFACE * pInt = (DSP_INTERFACE *)TCB_BASE_ADDR;

    if (!DspDrvCtx.bInitialized)
        return &LocalCevaDrvCfg;

    // if CEVA code was loaded to CEVA processors,
    // we need to provide the pointer to real configurations
    // to allow drivers to put parameters directly to the drivers

    return &pInt->CevaDrvCfg;
}

/** @brief This code tries to restore CEVA processors by reloading
           CEVA code.

    @param  [nBadMask] - the mask of processors to be reloaded
    @return [UINT32] - the mask of restored processors */

UINT32 DspDrvReinitCeva(UINT32 nBadMask)
{
    UINT32 i = 0, upmask = 0;
    DSP_INTERFACE * pInt = (DSP_INTERFACE *)TCB_BASE_ADDR;
    TCB * pTcb;
    BOOL next_desc;
    PDSPBOOTDESC p_src, p_dst;
    //UINT32 ExtCodeDataOffset;
    UINT32 Length=0;
    //UINT32 StartTick;

    PUINT8 pDspImage = (PUINT8)cevabincode;

    //REG32(CLEAR_IPI_REG) = nBadMask;
    DspDrvResetCevaByMask(nBadMask);

#if 1
    p_src = (PDSPBOOTDESC)pDspImage;
    p_dst = (PDSPBOOTDESC)DSP_BOOT_DESC_ADDR;

    do
    {
        next_desc = p_src->next;
        *p_dst = *p_src++;
        p_dst->external += (UINT32)pDspImage;

        if (next_desc)
            p_dst->next += DSP_BOOT_DESC_ADDR;

        Length += p_dst->length;
        p_dst++;
        //nDesc++;

    }while (next_desc);

#endif

    //ExtCodeDataOffset = 0x800;
    // copy external code section to CRAM at 0xFC001000 address
    //memcpy((PUINT8)DSP_EXT_SEGMENTS, &pDspImage[ExtCodeDataOffset], ExtCodeDataLen);

    *(PUINT32)SPU_CEVA_BOOT_ADDR = DSP_EXT_SEGMENTS;

    for (i = 0; i < /*DspDrvCtx.numDSP*/ 10; i++)
    {
        if ((nBadMask&(1<<i)) == 0)
            continue;

        pTcb = &dsp_tcb[i];
        pTcb->Status = 0;
        pInt->CevaTcb[i]  = pTcb;

        DspDrvTurnOnDSP(i);

        #if 0
        StartTick = Get24MHzTick();
        while (1)
        {
            if (pTcb->Status != 0)
            {
                upmask |= (1 << i);
                break;
            }
            else if ((Get24MHzTick() - StartTick) > 10000 * 150)
            {
                DspDrvResetCeva(i);
                break;
            }
        }
        #else
        upmask |= (1 << i);
        #endif
    }

    return upmask;
}

void DspMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    if (nCpuID == 0)
    {
        MmuMgrCfgMemAP(SPU_BASEADDR,        4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(SPUCLKRST_BASEADDR,  4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(INTC_SPU_BASEADDR,   4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
    }
}


//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL evmfpga.c
 *
 * @brief PCIe EP / Radio control implemented on FPGA
 * @author Mindspeed Technologies
 * @version $Revision: 1.59 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "evmfpga.h"
#include "pciedrv.h"
#include "mdmadrv.h"
#include "cpridrv.h"
#include "appprintf.h"
#include "hal.h"
#include "icdrv.h"
#include "timerdrv.h"
#include "ad9361radio.h"
#include "globalstoragearrays.h"
#include "mlog.h"

//#define EVM_TIMER_EMUL
#define EVM_TIMER_EMUL_PERIOD   1
#define TRACE_NUM               0
//#define AN0_TO_AN0_AN1
//#define FORCED_MIMO_ENABLED
#define MDMA_FDESC_OPT_ENABLED
//#define EVM_DISABLE_RX_1            // for 2 antenna mode, to forced disable RX-1 and to allow RX-0 only

static EvmFpgaCtx evm_rad_ctx;
static UINT32     pcie_inited = 0;

EvmCfg EvmFpgaApiConfig = {0};
UINT32 EvmFpgaApiConfigSet = 0;

#if TRACE_NUM > 0

VUINT32 NCNBVAR max_tx_p [TRACE_NUM];
VUINT32 NCNBVAR max_tx_n [TRACE_NUM];
VUINT32 NCNBVAR max_tx_count;

VUINT32 NCNBVAR max_rx_p [TRACE_NUM];
VUINT32 NCNBVAR max_rx_n [TRACE_NUM];
VUINT32 NCNBVAR max_rx_count;

VUINT32 NCNBVAR tx_irq_ticks [TRACE_NUM];
VUINT32 NCNBVAR tx_irq_idx;
VUINT32 NCNBVAR rx_irq_ticks [TRACE_NUM];
VUINT32 NCNBVAR rx_irq_idx;

VUINT32 NCNBVAR evmtrace;
VUINT32 NCNBVAR evmirqnum_tx;
VUINT32 NCNBVAR evmirqnum_rx;
VUINT32 NCNBVAR evmirqnum;
VUINT32 NCNBVAR evm_tx_begin_tick;
VUINT32 NCNBVAR evm_rx_begin_tick;

#define EVM_TRACE(x)    evmtrace = (x);

#else
#define EVM_TRACE(x)
#endif

#if EVM_FPGA_TIMER_TRACE_NUM > 0
HANDLE  hEvmFpgaTraceTimer;
UINT32  evmfpga_trace_timer[EVM_FPGA_TIMER_TRACE_NUM];
VUINT32 evmfpga_trace_timer_put = 0;

void EvmFpgaTraceTimerProc(void*p)
{
    if (evmfpga_trace_timer_put >= EVM_FPGA_TIMER_TRACE_NUM)
        return;

    evmfpga_trace_timer[evmfpga_trace_timer_put++] = MxGetTicks();

    if (evmfpga_trace_timer_put >= EVM_FPGA_TIMER_TRACE_NUM)
        evmfpga_trace_timer_put = 0;
}

MXRC EvmFpgaCreateTraceTimer(void)
{
    MXRC rc;

    if (HANDLE_NOT_NULL(hEvmFpgaTraceTimer))
        return MX_OK;

    evmfpga_trace_timer_put = 0;

    rc = MxCreateTimer(TIMER_MODE_PERIOD, 10, NULL, HNULL, &hEvmFpgaTraceTimer);

    _ASSERT_RC(rc);

    if (FAILED(rc))
        return rc;

    rc = MxSetTimerProc(hEvmFpgaTraceTimer, EvmFpgaTraceTimerProc, NULL);

    _ASSERT_RC(rc);

    if (FAILED(rc))
        return rc;

    return rc;
}

MXRC EvmFpgaStartTimer (void)
{
    MXRC rc;

    if (HANDLE_NULL(hEvmFpgaTraceTimer))
        return RC_EVM_FPGA_NOT_INITED;

    rc = MxStartFracTimer(hEvmFpgaTraceTimer);

    _ASSERT_RC(rc);

    if (FAILED(rc))
        return rc;

    return rc;
}

void EvmFpgaRemoveTraceTimer(void)
{
    if (HANDLE_NULL(hEvmFpgaTraceTimer))
        return;

    MxStopTimer(hEvmFpgaTraceTimer);
    MxRemoveTimer(hEvmFpgaTraceTimer);

    hEvmFpgaTraceTimer = HNULL;
}

UINT32 EvmFpgaTraceTimerGetTime(UINT32 * pIter)
{
    _ASSERT_PTR(pIter);

    if (*pIter >= evmfpga_trace_timer_put)
    {
        *pIter = 0xFFFFFFFF;
        return 0;
    }

    return evmfpga_trace_timer[(*pIter)++];
}

#endif

#if EVM_FPGA_RX_TIME_TRACE_NUM > 0

VUINT32 rx_irq_time_trace [EVM_FPGA_RX_TIME_TRACE_NUM];
VUINT32 rx_irq_time_trace_put = 0;

void EvmFpgaRxTraceTime(void)
{
    if (rx_irq_time_trace_put >= EVM_FPGA_RX_TIME_TRACE_NUM)
        return;

    rx_irq_time_trace[rx_irq_time_trace_put++] = MxGetTicks ();

    if (rx_irq_time_trace_put >= EVM_FPGA_RX_TIME_TRACE_NUM)
        rx_irq_time_trace_put = 0;
}

UINT32 EvmFpgaRxTraceGetTime (UINT32 *pIter)
{
    _ASSERT_PTR(pIter);

    if (*pIter >= rx_irq_time_trace_put)
    {
        *pIter = 0xFFFFFFFF;
        return 0;
    }

    return rx_irq_time_trace[(*pIter)++];
}

void EvmFpgaRxTraceTimeInit(void)
{
    rx_irq_time_trace_put = 0;
}

#endif

#if EVM_FPGA_TX_TIME_TRACE_NUM > 0

VUINT32 tx_irq_time_trace [EVM_FPGA_TX_TIME_TRACE_NUM];
VUINT32 tx_irq_time_trace_put = 0;

void EvmFpgaTxTraceTime(void)
{
    if (tx_irq_time_trace_put >= EVM_FPGA_TX_TIME_TRACE_NUM)
        return;

    tx_irq_time_trace[tx_irq_time_trace_put++] = MxGetTicks ();

    if (tx_irq_time_trace_put >= EVM_FPGA_TX_TIME_TRACE_NUM)
        tx_irq_time_trace_put = 0;
}

UINT32 EvmFpgaTxTraceGetTime (UINT32 *pIter)
{
    _ASSERT_PTR(pIter);

    if (*pIter >= tx_irq_time_trace_put)
    {
        *pIter = 0xFFFFFFFF;
        return 0;
    }

    return tx_irq_time_trace[(*pIter)++];
}

void EvmFpgaTxTraceTimeInit(void)
{
    tx_irq_time_trace_put = 0;
}

#endif

INLINE EvmFpgaCtx * EvmGetCtx (void)
{
    return &evm_rad_ctx;
}

/** @brief This function returns the pointer to the current TX buffer of specified antenna
    @param annt [in] - the antenna ID

    @return [PUINT8] the pointer to the current buffer of antenna*/

INLINE PUINT8 EvmGetAntennaTxBuffer (UINT32 annt)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    PUINT8 pBuf;

    _ASSERT_PTR(pCtx);

    if (!pCtx->bInited)
        return NULL;

    pBuf = pCtx->TxBuf + (annt * pCtx->TxBufSize); // TxBufSize == 2 sub buffers per antenna (61440*2)

    // to point to the TX buffer that is not currently used to send data
    if (pCtx->TxBufPos < FPGA_RAD_TX_SIZE)
        return pBuf + FPGA_RAD_TX_SIZE;

    // to point to the TX buffer that is not currently used to send data
    return pBuf;
}

/** @brief This function returns the pointer to the current RX buffer of specified antenna
    @param annt [in] - the antenna ID

    @return [PUINT8] the pointer to the current buffer of antenna*/

INLINE PUINT8 EvmGetAntennaRxBuffer (UINT32 annt)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    PUINT8 pBuf;

    _ASSERT_PTR(pCtx);

    if (!pCtx->bInited)
        return NULL;

    pBuf = pCtx->RxBuf + (annt * pCtx->RxBufSize); // RxBufSize == 2 sub buffers per antenna (61440*2)

    // to point to the TX buffer that is not currently used to send data
    if (pCtx->RxBufPos < FPGA_RAD_RX_SIZE)
        return pBuf + FPGA_RAD_RX_SIZE;

    // to point to the RX buffer that is not currently used to send data
    return pBuf;
}

void EvmMdmaTxCb (void * pParam)
{
    EvmFpgaCtx * pCtx = (EvmFpgaCtx *)pParam;
    _ASSERT_PTR(pCtx);

    pCtx->MdmaIsDone = 1;
}
static UINT32 tTxDma;
void EvmMdmaTxCalcDelay (void * pParam)
{
    EvmFpgaCtx * pCtx = (EvmFpgaCtx *)pParam;
    UINT32 nTick;

    _ASSERT_PTR(pCtx);

    nTick = GetTIMETICK();

    MLogTask(1165, RESOURCE_RAD_AXI, tTxDma, nTick);

    pCtx->TxMdmaPeriod = nTick - pCtx->TxMdmaTicks;

    if (pCtx->TxMdmaPeriod > pCtx->MaxTxMdmaPeriod)
        pCtx->MaxTxMdmaPeriod = pCtx->TxMdmaPeriod;

    pCtx->TxMdmaComplete = 1;
}

static UINT32 tRxDma;
void EvmMdmaLogRxNonBlocking (void * pParam)
{
    MLogTask(1166, RESOURCE_RAD_AXI, tRxDma, GetTIMETICK());
    // it's just to run mdma copy in non-blocking mode
}

void EvmMdmaLogTxNonBlocking (void * pParam)
{
    // it's just to run mdma copy in non-blocking mode
}

void EvmStopTx (void* pCtx, void* pTxData)
{

}

#ifdef DL_IQ_PATTERN_ENABLED

static UINT32 pattern_offs = 0;

void EvmFpgaInitPatternEngine(void)
{
    pattern_offs = 0;
}

PUINT8 EvmFpgaGetPattern (EvmFpgaCtx * pCtx)
{
    extern const signed short iqbuf[];
    extern unsigned int iqnum;

    PUINT8 p;

    if (IS_PROF_PCIE_MASTER(pCtx) || IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        p = ((PUINT8)iqbuf) + pattern_offs * 61440;

        pattern_offs ++;

        if (pattern_offs >= 10)
            pattern_offs = 0;
    }
    else
    {
        p = ((PUINT8)iqbuf) + pattern_offs * 61440/4;
        pattern_offs++;

        //if (pattern_offs >= (iqnum / 15360))
        if (pattern_offs >= 40)
        {
            pattern_offs = 0;
        }
    }

    return p;
}

#endif // DL_IQ_PATTERN_ENABLED

INLINE UINT32 EvmFpgaIsTxIrq (EvmFpgaCtx * pCtx)
{
    _ASSERT_PTR(pCtx);

    if (pCtx->IntProfID == EVM_FPGA_INT_PROF_DEF || IS_PROF_PCIE_MASTER(pCtx))
    {
        return (REG32(GPIO_INT_STAT_REG) & GPIO_6);
    }
    else if (IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        return pCtx->PCIIntStatusPtr[0];
    }
    else
    {
        return (REG32(GPIO_INT_STAT_REG) & GPIO_0);
    }
}

INLINE void EvmFpgaAckTxIrq (EvmFpgaCtx * pCtx)
{
    _ASSERT_PTR(pCtx);

    if (pCtx->IntProfID == EVM_FPGA_INT_PROF_DEF || IS_PROF_PCIE_MASTER(pCtx))
    {
        REG32(GPIO_INT_CLEAR_REG) |= GPIO_6;
        REG32(GPIO_INT_CLEAR_REG) &= ~GPIO_6;
    }
    else if (IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        _ASSERT_PTR(pCtx->PCIIntStatusPtr);
        pCtx->PCIIntStatusPtr[0] = 0;
    }
    else
    {
        REG32(GPIO_INT_CLEAR_REG) |= GPIO_0;
        REG32(GPIO_INT_CLEAR_REG) &= ~GPIO_0;
    }
}

INLINE UINT32 EvmFpgaIsRxIrq (EvmFpgaCtx * pCtx)
{
    _ASSERT_PTR(pCtx);

    if (IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        _ASSERT_PTR(pCtx->PCIIntStatusPtr);
        return pCtx->PCIIntStatusPtr [1];
    }

    return (REG32(GPIO_INT_STAT_REG) & GPIO_2);
}

INLINE void EvmFpgaAckRxIrq (EvmFpgaCtx * pCtx)
{
    _ASSERT_PTR(pCtx);

    if (IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        _ASSERT_PTR(pCtx->PCIIntStatusPtr);
        pCtx->PCIIntStatusPtr [1] = 0;
    }
    else
    {
        REG32(GPIO_INT_CLEAR_REG) |= GPIO_2;
        REG32(GPIO_INT_CLEAR_REG) &= ~GPIO_2;
    }
}

UINT32 EvmFpgaAppTxCbRun (void * plpCtx)
{
    EvmFpgaCtx * pCtx = (EvmFpgaCtx *)plpCtx;

    _ASSERT_PTR(pCtx);

    pCtx->pTxDMACallBack (pCtx->pTxCbCtx, 0, (pCtx->TxBufPos < FPGA_RAD_TX_SIZE) ? (pCtx->TxBuf + FPGA_RAD_TX_SIZE) : pCtx->TxBuf, FPGA_RAD_TX_SIZE);

    if (IS_PROF_PCIE_MASTER(pCtx) || IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        pCtx->TxBufPos += (FPGA_RAD_TX_FIFO_SIZE * 4);
    }
    else
    {
        pCtx->TxBufPos += FPGA_RAD_TX_FIFO_SIZE;
    }

    if (pCtx->TxBufPos >= pCtx->TxBufSize)
    {
        pCtx->TxBufPos = 0;
    }

    return 0;
}

UINT32 EvmFpgaAppRxCbRun (void * plpCtx)
{
    EvmFpgaCtx * pCtx = (EvmFpgaCtx *)plpCtx;

    _ASSERT_PTR(pCtx);

    pCtx->pRxDMACallBack (pCtx->pRxCbCtx, 0, (LPVOID)(pCtx->RxBuf + pCtx->RxBufPos), FPGA_RAD_RX_SIZE);

    EVM_TRACE(22);

    if (pCtx->RxBufPos >= pCtx->RxBufSize)
        pCtx->RxBufPos = 0;

    return 0;
}

/** @brief This handler is called to run TX and RX operations
    @param pIntClearReg [out] - the pointer to the IRQ ack register
    @param nIrqID       [in]  - the interrupt ID
    @param pParam       [in]  - the pointer to the driver context     */

void EvmFpgaIrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, LPVOID pParam)
{
    PMDMA_FDESC pMdma = NULL, pFirstMdma = NULL;
    EvmFpgaCtx * pCtx = (EvmFpgaCtx *)pParam;
    UINT32 nDescrNum, txu = 0;
    MXRC rc = MX_OK;
    PUINT8 buf = 0, buf1 = 0;
    UINT32 nLog0 = 0, nLog1 = 0;

    _ASSERT_PTR(pCtx);

    #if TRACE_NUM > 0
        evmirqnum ++;
    #endif

    if (IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        REG32(RAD_CFG_RC_MSI_CATCHER) = 0; // clear PCIe interrupt
    }

    EVM_TRACE(1);

    // Just to prevent sending data to the FPGA
    if (pCtx->StopIrq != 0 || pCtx->bInited == 0)
    {
        EVM_TRACE(2);

        if (EvmFpgaIsTxIrq(pCtx))
            EvmFpgaAckTxIrq(pCtx);

        if (EvmFpgaIsRxIrq(pCtx))
            EvmFpgaAckRxIrq(pCtx);

        *pIntClearReg = nIrqID;

        EVM_TRACE(3);

        uart_printf("EVM-IRQ is shutdown\r\n");

        return;
    }

    #if EVM_FPGA_RX_TIME_TRACE_NUM > 0
        if (EvmFpgaIsRxIrq(pCtx))
            EvmFpgaRxTraceTime();
    #endif

    #if EVM_FPGA_TX_TIME_TRACE_NUM > 0
        if (EvmFpgaIsTxIrq(pCtx))
            EvmFpgaTxTraceTime();
    #endif

    #if TRACE_NUM > 0
        if (EvmFpgaIsTxIrq(pCtx))
        {
            if (tx_irq_idx < TRACE_NUM)
                tx_irq_ticks [tx_irq_idx ++] = MxGetTicks ();
        }

        if (EvmFpgaIsRxIrq(pCtx))
        {
            if (rx_irq_idx < TRACE_NUM)
                rx_irq_ticks [rx_irq_idx ++] = MxGetTicks ();
        }
    #endif

    if (pCtx->bReinitTimerOnRx)
    {
        if (EvmFpgaIsRxIrq(pCtx))
        {
            //pCtx->bReinitTimerOnRx = 0;
            TimerDrvRestart();

            #if EVM_FPGA_TIMER_TRACE_NUM > 0
            EvmFpgaStartTimer();
            #endif
        }
    }

    // ====================================================
    //            TX interrupt
    // ====================================================
    if (EvmFpgaIsTxIrq(pCtx))
    {
        EVM_TRACE(4);

        #if TRACE_NUM > 0
            if (evmirqnum_tx == 0)
                evm_tx_begin_tick = GetTIMETICK();

            evmirqnum_tx ++;
        #endif

        pCtx->IrqTxSubPeriod= GetTIMETICK()-pCtx->IrqTxSubTicks;
        pCtx->IrqTxSubTicks = GetTIMETICK();

        #if TRACE_NUM > 0
            // this code tracks the situation, when SUB TX IRQ time
            // is more than 150_000 / 4 and puts it to the array

            if ((pCtx->IrqTxSubPeriod > 40000 || pCtx->IrqTxSubPeriod < 36000 ) && max_tx_count < TRACE_NUM)
            {
                //uart_printf ("tx> %d %d\r\n", pCtx->IrqTxSubPeriod, evmirqnum_tx );
                max_tx_p [max_tx_count] = pCtx->IrqTxSubPeriod;
                max_tx_n [max_tx_count] = evmirqnum_tx;

                max_tx_count ++;
            }
        #endif

        pCtx->IrqSubTxPeriodSum += pCtx->IrqTxSubPeriod;

        if ((pCtx->IrqTxSubPeriod > pCtx->MaxTxSubPeriod))
            pCtx->MaxTxSubPeriod = pCtx->IrqTxSubPeriod;

        pCtx->TxIrqNum++;

        EVM_TRACE(5);

        // descriptors to handle one or two antennas
        #ifdef MDMA_FDESC_OPT_ENABLED
            nDescrNum = 1;
        #else
            nDescrNum = pCtx->AntennaNum;
        #endif

        if (IS_PROF_PCIE_MASTER(pCtx) || IS_PROF_PCIE_MASTER_IRQ(pCtx))
        {
            // in this case when we use FPGA master PCIe,
            // we do not need to copy PHY data to PCIe memory
            // only if we replace data with pattern

            #ifndef DL_IQ_PATTERN_ENABLED
                nDescrNum = 0;
            #endif
        }

        // to log TX#0 or TX#1 data if this feature is enabled
        // EVM_FPGA_TX_LOG_1_4_SF macro controls how are we logging data
        // by completed SF or by using 1/4 of SF

        #ifndef EVM_FPGA_TX_LOG_1_4_SF
        // to log one subframe, it's ready and can be logged
        if ((pCtx->TxBufPos == 0) || ((pCtx->TxBufPos == (pCtx->TxBufSize / 2))))
        #endif
        {
            if (pCtx->MaskCtrl & EVM_FPGA_CTRL_TX0_LOG)
            {
                nLog0 = 1;
            }

            if ((pCtx->MaskCtrl & EVM_FPGA_CTRL_TX1_LOG) && (pCtx->AntennaNum == 2))
            {
                nLog1 = 1;
            }

            if (nLog0 || nLog1)
            {
                #ifdef MDMA_FDESC_OPT_ENABLED
                    nDescrNum++;
                #else
                    nDescrNum += nLog0 + nLog1;
                #endif
            }
        }

        EVM_TRACE(6);

        if (nDescrNum != 0)
        {
            pFirstMdma = pMdma = MdmaAllocDesc(MDMA_HW_RAD, nDescrNum);
            _ASSERT_PTR (pMdma);

    		if(!pMdma)
            {
                uart_printf ("EVM-FPGA: Can't allocate TX MDMA descriptors, required=%d\r\n", nDescrNum);
    			return;
            }
        }

        #ifdef DL_IQ_PATTERN_ENABLED
            buf = EvmFpgaGetPattern(pCtx);
        #else
            buf = (PUINT8)(pCtx->TxBuf + pCtx->TxBufPos);
        #endif

        if (IS_PROF_PCIE_MASTER(pCtx) || IS_PROF_PCIE_MASTER_IRQ(pCtx))
        {
            #ifdef AN0_TO_AN0_AN1
               buf1 = buf;
            #else
               buf1 = (PUINT8)(pCtx->TxBuf + pCtx->TxBufSize + pCtx->TxBufPos);
            #endif

            // in this case we need to copy data to TX buffers
            // only if pattern is used

            #ifdef DL_IQ_PATTERN_ENABLED

                MdmaSetCopy(pMdma);

                if (pCtx->AntennaNum == 1)
                {
                    // to copy complete SF (61440 bytes)
                    MdmaSetDataEx (pMdma,
                                   buf,
                                   FPGA_RAD_TX_FIFO_SIZE * 4,
                                   pCtx->TxBuf + pCtx->TxBufPos,
                                   FPGA_RAD_TX_FIFO_SIZE * 4,
                                   0);
                }
                else
                {
                    #ifdef MDMA_FDESC_OPT_ENABLED
                        MdmaSetData2(pMdma,
                                     buf,
                                     FPGA_RAD_TX_FIFO_SIZE * 4,
                                     buf1,
                                     FPGA_RAD_TX_FIFO_SIZE * 4,
                                     (PUINT8)pCtx->TxBuf + pCtx->TxBufPos,
                                     FPGA_RAD_TX_FIFO_SIZE * 4,
                                     (PUINT8)pCtx->TxBuf + pCtx->TxBufPos + pCtx->TxBufSize,
                                     FPGA_RAD_TX_FIFO_SIZE * 4,
                                     0);
                    #else
                        MdmaSetData (pMdma,
                                     buf,
                                     FPGA_RAD_TX_FIFO_SIZE * 4,
                                     pCtx->TxBuf + pCtx->TxBufPos,
                                     FPGA_RAD_TX_FIFO_SIZE * 4);

                        pMdma = pMdma->Next;

                        MdmaSetCopy(pMdma);
                        MdmaSetData (pMdma,
                                     buf1,
                                     FPGA_RAD_TX_FIFO_SIZE * 4,
                                     (PUINT8)pCtx->TxBuf + pCtx->TxBufPos + pCtx->TxBufSize,
                                     FPGA_RAD_TX_FIFO_SIZE * 4);

                    #endif
                }

                pMdma = pMdma->Next;

            #endif
        }
        else
        {
            MdmaSetCopy(pMdma);

            if (pCtx->AntennaNum == 1)
            {
                MdmaSetDataEx (pMdma,
                               buf,
                               FPGA_RAD_TX_FIFO_SIZE,
                               (PUINT8)PCIE_RC_MEM,
                               FPGA_RAD_TX_FIFO_SIZE,0);
            }
            else
            {
                #ifdef AN0_TO_AN0_AN1
                   buf1 = buf;
                #else
                   buf1 = (PUINT8)(pCtx->TxBuf + pCtx->TxBufSize + pCtx->TxBufPos);
                #endif

                #ifdef MDMA_FDESC_OPT_ENABLED
                    MdmaSetData2(pMdma,
                                 buf,
                                 FPGA_RAD_TX_FIFO_SIZE,
                                 buf1,
                                 FPGA_RAD_TX_FIFO_SIZE,
                                 (PUINT8)PCIE_RC_MEM,
                                 FPGA_RAD_TX_FIFO_SIZE,
                                 (PUINT8)PCIE_RC_MEM,
                                 FPGA_RAD_TX_FIFO_SIZE,
                                 0);
                #else
                    MdmaSetData (pMdma,
                                 buf,
                                 FPGA_RAD_TX_FIFO_SIZE,
                                 (PUINT8)PCIE_RC_MEM,
                                 FPGA_RAD_TX_FIFO_SIZE);

                    pMdma = pMdma->Next;

                    MdmaSetCopy(pMdma);

                    MdmaSetData (pMdma,
                                 buf1,
                                 FPGA_RAD_TX_FIFO_SIZE,
                                 (PUINT8)PCIE_RC_MEM,
                                 FPGA_RAD_TX_FIFO_SIZE);

                #endif
            }

            pMdma = pMdma->Next;
        }

        EVM_TRACE(7);

        // to log TX#0 or TX#1 data if this feature is enabled
        // to log one subframe, it's ready and can be logged

        // the log for the first one antenna
        if (nLog0 != 0 || nLog1 != 0)
        {
            _ASSERT_PTR (pMdma);

            MdmaSetCopy(pMdma);

            if (nLog0 != 0 && nLog1 != 0)
            {
                #ifdef MDMA_FDESC_OPT_ENABLED
                    MdmaSetData2(pMdma,
                                buf,
                                FPGA_TX_LOG_PART_SIZE,
                                buf1,
                                FPGA_TX_LOG_PART_SIZE,
                                (LPVOID)(pCtx->pTxLog[0] + pCtx->TxLogPos),
                                FPGA_TX_LOG_PART_SIZE,
                                (LPVOID)(pCtx->pTxLog[1] + pCtx->TxLogPos),
                                FPGA_TX_LOG_PART_SIZE,
                                0);
                #else

                    MdmaSetData(pMdma,
                                buf,
                                FPGA_TX_LOG_PART_SIZE,
                                (LPVOID)(pCtx->pTxLog[0] + pCtx->TxLogPos),
                                FPGA_TX_LOG_PART_SIZE);

                    pMdma = pMdma->Next;

                    MdmaSetCopy(pMdma);

                    MdmaSetData(pMdma,
                                buf1,
                                FPGA_TX_LOG_PART_SIZE,
                                (LPVOID)(pCtx->pTxLog[1] + pCtx->TxLogPos),
                                FPGA_TX_LOG_PART_SIZE);

                #endif
            }
            else if (nLog0 != 0)
            {
                MdmaSetDataEx(pMdma,
                            buf,
                            FPGA_TX_LOG_PART_SIZE,
                            (LPVOID)(pCtx->pTxLog[0] + pCtx->TxLogPos),
                            FPGA_TX_LOG_PART_SIZE, 0);
            }
            else if (nLog1 != 0)
            {
                MdmaSetDataEx(pMdma,
                            buf1,
                            FPGA_TX_LOG_PART_SIZE,
                            (LPVOID)(pCtx->pTxLog[1] + pCtx->TxLogPos),
                            FPGA_TX_LOG_PART_SIZE, 0);
            }

            pCtx->TxLogPos += (FPGA_TX_LOG_PART_SIZE);

            // to handle overflow condition
            if (pCtx->TxLogPos >= pCtx->TxLogSize)
            {
                if (pCtx->MaskCtrl & EVM_FPGA_CTRL_TX_LOG_RR)
                {
                    // to set pointer to the beginning of the log
                    pCtx->TxLogPos = 0;
                }
                else
                {
                    // to turn off the TX logger when log is filled
                    pCtx->MaskCtrl &= ~(EVM_FPGA_CTRL_TX0_LOG | EVM_FPGA_CTRL_TX1_LOG);
                }
            }
        }

        EVM_TRACE(8);

        if (pCtx->TxMdmaComplete == 0)
        {
            //uart_printf ("RADIO: TX-MDMA is overrun!\r\n");
            //while (1);
            pCtx->TxMdmaOverrun ++;
        }

        // to reset the flag, it will be set by MDMA callback
        pCtx->TxMdmaComplete = 0;

        EVM_TRACE(9);

        // ---------------------------------------------------
        // To run MDMA blocks assigned to process TX direction
        // ---------------------------------------------------
        pCtx->TxMdmaTicks = GetTIMETICK();

        if (pFirstMdma != NULL)
        {
            tTxDma = GetTIMETICK();
            rc = MdmaRunEx(MDMA_HW_RAD, pFirstMdma, EvmMdmaTxCalcDelay, pCtx);
            _ASSERT_RC(rc);
        }
        else
        {
            pCtx->TxMdmaComplete = 1;
        }

        // to call the Application handler for the next frame
        // at the end stage of the current frame sending

        EVM_TRACE(10);

        if ((pCtx->TxBufPos == 0 || pCtx->TxBufPos == (pCtx->TxBufSize / 2)))
        {
            pCtx->IrqTxPeriod= GetTIMETICK()-pCtx->IrqTxTicks;
            pCtx->IrqTxTicks = GetTIMETICK();

            if ((pCtx->IrqTxPeriod > pCtx->MaxTxPeriod))
                pCtx->MaxTxPeriod = pCtx->IrqTxPeriod;

            if (pCtx->TxIrqGpioEna)
            {
                if(++pCtx->TxIrqGpioToggleCntr >= pCtx->TxIrqGpioToggleDelays[pCtx->TxIrqGpioToggleDelayIdx])
                {
                    REG32(GPIO_OUTPUT_REG) ^= (1 <<  pCtx->TxIrqGpioPort);
                    pCtx->TxIrqGpioToggleCntr = 0;
                    pCtx->TxIrqGpioToggleDelayIdx ^= 1;
                }
            }

            if (pCtx->pTxDMACallBack != NULL)
            {
                // to call the app callback with pointing to the correct TX buffer
                // that can be used to write data

                if (pCtx->MasterCpu == MxGetCpuID())
                {
                    pCtx->pTxDMACallBack (pCtx->pTxCbCtx, 0, (pCtx->TxBufPos < FPGA_RAD_TX_SIZE) ? (pCtx->TxBuf + FPGA_RAD_TX_SIZE) : pCtx->TxBuf, FPGA_RAD_TX_SIZE);
                }
                else
                {
                    txu = 1;
                    MxInjectCode(pCtx->MasterCpu, EvmFpgaAppTxCbRun, pCtx);
                }

                /*if (pCtx->AntennaNum == 2)
                    pCtx->pTxDMACallBack (pCtx->pTxCbCtx, 1, (pCtx->TxBufPos < FPGA_RAD_TX_SIZE) ? (pCtx->TxBuf + pCtx->TxBufSize + FPGA_RAD_TX_SIZE) : (pCtx->TxBuf + pCtx->TxBufSize), FPGA_RAD_TX_SIZE);*/
            }
        }

        EVM_TRACE(11);

        if (txu == 0)
        {
            if (IS_PROF_PCIE_MASTER(pCtx) || IS_PROF_PCIE_MASTER_IRQ(pCtx))
            {
                pCtx->TxBufPos += (FPGA_RAD_TX_FIFO_SIZE * 4);
            }
            else
            {
                pCtx->TxBufPos += FPGA_RAD_TX_FIFO_SIZE;
            }

            if (pCtx->TxBufPos >= pCtx->TxBufSize)
            {
                pCtx->TxBufPos = 0;
            }
        }

        EVM_TRACE(12);

        EvmFpgaAckTxIrq(pCtx);

        EVM_TRACE(13);

    }

    EVM_TRACE(14);

    // ==================================================
    //             RX - message
    // ==================================================
    if (EvmFpgaIsRxIrq(pCtx))
    {
        EVM_TRACE(15);

        #if TRACE_NUM > 0
            if (evmirqnum_rx == 0)
                evm_rx_begin_tick = GetTIMETICK();

            evmirqnum_rx ++;
        #endif

        if (pCtx->RxIrqGpioEna)
        {
            if(++pCtx->RxIrqGpioToggleCntr >= pCtx->RxIrqGpioToggleDelays[pCtx->RxIrqGpioToggleDelayIdx])
            {
                REG32(GPIO_OUTPUT_REG) ^= (1 <<  pCtx->RxIrqGpioPort);
                pCtx->RxIrqGpioToggleCntr = 0;
                pCtx->RxIrqGpioToggleDelayIdx ^= 1;
            }
        }

        pCtx->IrqRxPeriod= GetTIMETICK()-pCtx->IrqRxTicks;
        pCtx->IrqRxTicks = GetTIMETICK();

        pCtx->IrqRxPeriodSum += pCtx->IrqRxPeriod;

        #if TRACE_NUM > 0
            // this code tracks the situation, when RX IRQ time
            // is more than 150_000 (1ms) and puts it to the array

            if ((pCtx->IrqRxPeriod > 155000 || pCtx->IrqRxPeriod < 148000) && max_rx_count < TRACE_NUM)
            {
                //uart_printf ("rx> %d %d\r\n", pCtx->IrqRxPeriod, evmirqnum_rx);
                max_rx_p [max_rx_count] = pCtx->IrqRxPeriod;
                max_rx_n [max_rx_count] = evmirqnum_rx;

                max_rx_count ++;
            }
        #endif

        EVM_TRACE(16);

        if ((pCtx->IrqRxPeriod > pCtx->MaxRxPeriod))
            pCtx->MaxRxPeriod = pCtx->IrqRxPeriod;

        EVM_TRACE(17);

        pCtx->RxIrqNum++;

        EVM_TRACE(18);

        nLog0 = 0;
        nLog1 = 0;

        if (pCtx->MaskCtrl & (EVM_FPGA_CTRL_RX0_LOG))
        {
            nLog0 = 1;
        }

        if ((pCtx->MaskCtrl & (EVM_FPGA_CTRL_RX1_LOG)) && (pCtx->AntennaNum == 2))
        {
            nLog1 = 1;
        }

        // if RX#0 or RX#1 log is enabled, let's run it for RX stream
        if (nLog0 || nLog1)
        {
            EVM_TRACE(19);

            #ifdef MDMA_FDESC_OPT_ENABLED
                pFirstMdma = pMdma = MdmaAllocDesc(MDMA_HW_RAD, 1);
            #else
                pFirstMdma = pMdma = MdmaAllocDesc(MDMA_HW_RAD, nLog0 + nLog1);
            #endif

            _ASSERT_PTR (pMdma);

            MdmaSetCopy(pMdma);

            if (nLog0 && nLog1)
            {
                #ifdef MDMA_FDESC_OPT_ENABLED
                    MdmaSetData2  (pMdma,
                                   (LPVOID)(pCtx->RxBuf + pCtx->RxBufPos),
                                   FPGA_RAD_RX_FIFO_SIZE,
                                   (LPVOID)(pCtx->RxBuf + pCtx->RxBufSize + pCtx->RxBufPos),
                                   FPGA_RAD_RX_FIFO_SIZE,
                                   (LPVOID)(pCtx->pRxLog[0] + pCtx->RxLogPos),
                                   FPGA_RAD_RX_FIFO_SIZE,
                                   (LPVOID)(pCtx->pRxLog[1] + pCtx->RxLogPos),
                                   FPGA_RAD_RX_FIFO_SIZE,
                                   0);
                #else
                    MdmaSetData(pMdma,
                               (LPVOID)(pCtx->RxBuf + pCtx->RxBufPos),
                               FPGA_RAD_RX_FIFO_SIZE,
                               (LPVOID)(pCtx->pRxLog[0] + pCtx->RxLogPos),
                               FPGA_RAD_RX_FIFO_SIZE);

                    pMdma = pMdma->Next;

                    MdmaSetCopy(pMdma);

                    MdmaSetData(pMdma,
                               (LPVOID)(pCtx->RxBuf + pCtx->RxBufSize + pCtx->RxBufPos),
                               FPGA_RAD_RX_FIFO_SIZE,
                               (LPVOID)(pCtx->pRxLog[1] + pCtx->RxLogPos),
                               FPGA_RAD_RX_FIFO_SIZE);

                #endif
            }
            else if (nLog0)
            {
                MdmaSetDataEx (pMdma,
                               (LPVOID)(pCtx->RxBuf + pCtx->RxBufPos),
                               FPGA_RAD_RX_FIFO_SIZE,
                               (LPVOID)(pCtx->pRxLog[0] + pCtx->RxLogPos),
                               FPGA_RAD_RX_FIFO_SIZE,
                               0);
            }
            else if (nLog1)
            {
                 MdmaSetDataEx(pMdma,
                              (LPVOID)(pCtx->RxBuf + pCtx->RxBufSize + pCtx->RxBufPos),
                              FPGA_RAD_RX_FIFO_SIZE,
                              (LPVOID)(pCtx->pRxLog[1] + pCtx->RxLogPos),
                              FPGA_RAD_RX_FIFO_SIZE,
                              0);
            }

            tRxDma = GetTIMETICK();
            MdmaRunEx(MDMA_HW_RAD, pFirstMdma, EvmMdmaLogRxNonBlocking, pCtx);

            pCtx->RxLogPos += (FPGA_RAD_RX_FIFO_SIZE);

            EVM_TRACE(20);

            // to handle overflow condition
            if (pCtx->RxLogPos >= pCtx->RxLogSize)
            {
                if (pCtx->MaskCtrl & EVM_FPGA_CTRL_RX_LOG_RR)
                {
                    // to set pointer to the beginning of the log
                    pCtx->RxLogPos = 0;
                }
                else
                {
                    // to turn off the RX logger
                    pCtx->MaskCtrl &= ~(EVM_FPGA_CTRL_RX0_LOG | EVM_FPGA_CTRL_RX1_LOG);
                }
            }

            EVM_TRACE(21);

        }

        if (pCtx->pRxDMACallBack != NULL)
        {
            if (pCtx->MasterCpu == MxGetCpuID())
            {
                pCtx->pRxDMACallBack (pCtx->pRxCbCtx, 0, (LPVOID)(pCtx->RxBuf + pCtx->RxBufPos), FPGA_RAD_RX_SIZE);

                pCtx->RxBufPos += FPGA_RAD_RX_FIFO_SIZE;

                EVM_TRACE(22);

                if (pCtx->RxBufPos >= pCtx->RxBufSize)
                    pCtx->RxBufPos = 0;
            }
            else
            {
                MxInjectCode(pCtx->MasterCpu, EvmFpgaAppRxCbRun, pCtx);
            }

            /*if (pCtx->AntennaNum == 2)
                pCtx->pRxDMACallBack (pCtx->pRxCbCtx, 1, (LPVOID)(pCtx->RxBuf + pCtx->RxBufSize + pCtx->RxBufPos), FPGA_RAD_RX_SIZE);*/
        }

        EVM_TRACE(23);
        EvmFpgaAckRxIrq(pCtx);
        EVM_TRACE(24);

    }

    EVM_TRACE(25);

    _NOP();
    _NOP();
    _NOP();
    *pIntClearReg = nIrqID;

    EVM_TRACE(26);

    // to turn off the compiler warning
    rc = rc;
}

void EvmTestTimerProc (void * pParam)
{
    EvmFpgaCtx * pCtx = (EvmFpgaCtx *)pParam;

    if (pCtx->pTxDMACallBack != NULL)
    {
        #if EVM_TIMER_EMUL_PERIOD >= 100
        uart_printf ("Tick");
        #endif

        pCtx->pTxDMACallBack (pCtx->pTxCbCtx, 0, (pCtx->TxBufPos < FPGA_RAD_TX_SIZE) ? (pCtx->TxBuf + FPGA_RAD_TX_SIZE) : pCtx->TxBuf, FPGA_RAD_TX_SIZE);

        #if EVM_TIMER_EMUL_PERIOD >= 100
        uart_printf ("OK\r\n");
        #endif
    }
}

void EvmDumpData (PUINT32 pData, UINT32 nNum)
{
    while (nNum--)
    {
        uart_printf ("%08x ", *pData++);
    }

    uart_printf ("\r\n");
}

/** @brief This function runs AD9361 adapter loop back mode
           to check TX/RX operations
    @param pCtx [in] - the drier context
    @return [MXRC] an error code */

MXRC EvmFpgaRunLBTest (EvmFpgaCtx * pCtx)
{
    UINT32 i=0, j=0;
    VUINT32 * pRx, *pTx;

    // to wait about 10 ms, to be sure than systems works
    //MxDelayTicks(150000 * 150);
    MxDelay (20);

    // if no interrutps at all
    if (pCtx->IrqRxTicks == 0 || pCtx->IrqTxTicks == 0)
    {
        uart_printf ("Interrupts are not detected\r\n");
        return RC_EVM_FPGA_NO_IRQ;
    }

    // in any case the the buffers contain data

    pTx = (VUINT32*)pCtx->TxBuf;
    pRx = (VUINT32*)pCtx->RxBuf;

    // to run buffers comparation, to find the first element
    for (i = 0; i < 100; i++)
    {
        if ((pRx[i]) == (pTx[0]))
            break;
    }

    if (i >= 100)
    {
        uart_printf ("Pattern is not found\r\n");

        // To print in-data
        uart_printf ("TX:");
        EvmDumpData ((PUINT32)pTx, 8);

        // to print out-data
        uart_printf ("RX:");
        EvmDumpData ((PUINT32)pRx, 8);

        return RC_EVM_FPGA_NO_PATTERN;
    }

    // to check the buffer
    for (j = 0; j < 100; j++, i++)
    {
        if (pRx [i] != pTx [j])
        {
            uart_printf ("RX and TX are different\r\n");

            // To print in-data
            uart_printf ("TX:");
            EvmDumpData ((PUINT32)pTx, 8);

            // to print out-data
            uart_printf ("RX:");
            EvmDumpData ((PUINT32)pRx, 8);

            return RC_EVM_FPGA_COMPARE_ERROR;
        }
    }

    printf ("TX-IRQ:%02d, RX-IRQ:%02d, RX-AVGms:%002d, TX-AVGms:%003d\r\n",
            pCtx->TxIrqNum,
            pCtx->RxIrqNum,
            pCtx->IrqRxPeriod,
            pCtx->IrqTxPeriod);

    return MX_OK;
}

void EvmFpgaWaitTimerTicks()
{
    //TimerDrvWaitForTicks(MX_TIMER_FRACTION+1);
    TimerDrvReinitTimer();
}

/** @brief This function runs TX
    @param pCtx [in] - the drier context
    @return [MXRC] an error code */

MXRC EvmFpgaRunTx (EvmFpgaCtx * pCtx)
{
    PMDMA_FDESC pMdma = NULL, pBeginMdma = NULL;
    MXRC rc = MX_OK;
    UINT32 nDescs = 2*pCtx->AntennaNum; // We need to write twice to each antenna at the beginning by default
    PUINT8 buf, buf1;
    UINT32 nLog0 = 0, nLog1 = 0;
    UINT32 round_size = 1;
    UINT32 PcieMasterMode, nCopyInMaster = 0;

    #ifdef DL_IQ_PATTERN_ENABLED
    EvmFpgaInitPatternEngine();
    #endif

    #if EVM_FPGA_RX_TIME_TRACE_NUM > 0
        EvmFpgaRxTraceTimeInit();
    #endif

    #if EVM_FPGA_TX_TIME_TRACE_NUM > 0
        EvmFpgaTxTraceTimeInit();
    #endif

    #if EVM_FPGA_TIMER_TRACE_NUM > 0
        EvmFpgaCreateTraceTimer();
    #endif

    #ifdef EVM_FPGA_TX_LOG_1_4_SF
    round_size = 2;
    #endif

    #ifdef EVM_FPGA_REINIT_TIMER_ON_FIRST_RX
    pCtx->bReinitTimerOnRx = 1;
    #endif

    PcieMasterMode = (IS_PROF_PCIE_MASTER(pCtx) || IS_PROF_PCIE_MASTER_IRQ(pCtx));

    if (PcieMasterMode)
    {
        // the special case when FPGA is PCIe master
        // in this case we do not need to copy data
        // only pattern if this pattern is specified

        #ifdef DL_IQ_PATTERN_ENABLED
            nDescs = pCtx->AntennaNum; // It can handle one or two antennas
            nCopyInMaster = 1;
        #else
            nDescs = 0;
        #endif
    }

    // to allocate one descriptor for TX#0 LOG if it's enabled
    // 1 for LOG and 1 for FIFO

    if ((pCtx->MaskCtrl & EVM_FPGA_CTRL_TX0_LOG) && (pCtx->TxBufPos == 0))
    {
        nDescs += 1;
        nLog0   = 1;
    }

    if ((pCtx->MaskCtrl & EVM_FPGA_CTRL_TX1_LOG) && (pCtx->TxBufPos == 0) && (pCtx->AntennaNum == 2))
    {
        nDescs += 1;
        nLog1   = 1;
    }

    if (nDescs != 0)
    {
        if ((pBeginMdma = MdmaAllocDesc(MDMA_HW_RAD, nDescs)) == NULL)
        {
            _ASSERT_PTR (pBeginMdma);

            return RC_EVM_FPGA_NO_MDMA_DESCS;
        }
    }

    pMdma = pBeginMdma;

    #ifdef DL_IQ_PATTERN_ENABLED
        buf = EvmFpgaGetPattern(pCtx);
    #else
        buf = pCtx->TxBuf + pCtx->TxBufPos;
    #endif

    #ifdef AN0_TO_AN0_AN1
        buf1 = buf;
    #else
        buf1 = pCtx->TxBuf + pCtx->TxBufSize + pCtx->TxBufPos;
    #endif

    // to log TX data if this feature is enabled
    // to log one subframe, it's ready and can be logged

    if (nLog0 != 0)
    {
        MdmaSetCopy(pMdma);
        MdmaSetDataEx (pMdma,
            (LPVOID)(buf),
            FPGA_TX_LOG_PART_SIZE * round_size,
            (LPVOID)(pCtx->pTxLog[0] + pCtx->TxLogPos),
            FPGA_TX_LOG_PART_SIZE * round_size, 0);

        // to move the pointer to the next descriptor used for
        // FIFO or for Log#2 (antenna#2)
        pMdma = pMdma->Next;
    }

    if (nLog1 != 0)
    {
        MdmaSetCopy(pMdma);
        MdmaSetDataEx (pMdma,
            (LPVOID)(buf1),
            FPGA_TX_LOG_PART_SIZE * round_size,
            (LPVOID)(pCtx->pTxLog[1] + pCtx->TxLogPos),
            FPGA_TX_LOG_PART_SIZE * round_size, 0);

        pMdma = pMdma->Next;
    }

    // to move log pointer if TX#0 or TX#1 log is enabled
    //if (nLog0 != 0 || nLog1 != 0)
    {
        pCtx->TxLogPos += (FPGA_TX_LOG_PART_SIZE  * round_size);
    }

    // to handle overflow condition

    if (pCtx->TxLogPos >= pCtx->TxLogSize)
    {
        if (pCtx->MaskCtrl & EVM_FPGA_CTRL_TX_LOG_RR)
        {
            // to set pointer to the beginning of the log
            pCtx->TxLogPos = 0;
        }
        else
        {
            // to turn off the TX#0 and TX#1 logger when log is filled
            pCtx->MaskCtrl &= ~(EVM_FPGA_CTRL_TX0_LOG | EVM_FPGA_CTRL_TX1_LOG);
        }
    }

    // The previous interface, when PCIe is a slave
    // and we need to copy TX data to PCIe memory

    if (!PcieMasterMode)
    {
        pCtx->Regs->PcieIntEnable = 0;

        /*********************************************************
        *             The first one part of SF (1/4)             *
        **********************************************************/

        // the descriptors are already allocated
        // and can be used to program
        // antenna#0 - the first 1/4 of FIFO

        MdmaSetCopy(pMdma);
        MdmaSetDataEx (pMdma,
            buf,
            FPGA_RAD_TX_FIFO_SIZE,  // send 1/4 IQ sample
            (LPVOID)PCIE_RC_MEM,
            FPGA_RAD_TX_FIFO_SIZE,
            0);

        pMdma = pMdma->Next;

        // the descriptors are already allocated
        // and can be used to program
        // antenna#1 - the first 1/4 of FIFO

        if (pCtx->AntennaNum == 2)
        {
            MdmaSetCopy(pMdma);
            MdmaSetDataEx (pMdma,
                buf1,
                FPGA_RAD_TX_FIFO_SIZE,  // send 1/4 IQ sample
                (LPVOID)PCIE_RC_MEM,
                FPGA_RAD_TX_FIFO_SIZE,
                0);

            pMdma = pMdma->Next;
        }

        pCtx->TxBufPos += FPGA_RAD_TX_FIFO_SIZE;

        /*********************************************************
        *             The second one part of SF (1/4)            *
        **********************************************************/

        // the descriptors are already allocated
        // and can be used to program
        // antenna#0 - the first half of FIFO

        MdmaSetCopy(pMdma);

        #ifdef DL_IQ_PATTERN_ENABLED
            buf = EvmFpgaGetPattern(pCtx);
        #else
            buf = pCtx->TxBuf + pCtx->TxBufPos;
        #endif

        #ifdef AN0_TO_AN0_AN1
            buf1 = buf;
        #else
            buf1 = pCtx->TxBuf + pCtx->TxBufPos + pCtx->TxBufSize;
        #endif

        MdmaSetDataEx(pMdma,
                     buf,
                     FPGA_RAD_TX_FIFO_SIZE,  // send 1/4 IQ sample
                     (LPVOID)PCIE_RC_MEM,
                     FPGA_RAD_TX_FIFO_SIZE,
                     0);

        pMdma = pMdma->Next;

        // the descript(s) are already allocated
        // and can be used to program
        // antenna#1 - the first half of FIFO

        if (pCtx->AntennaNum == 2)
        {
            MdmaSetCopy(pMdma);
            MdmaSetDataEx (pMdma,
                           buf1,
                           FPGA_RAD_TX_FIFO_SIZE,  // send 1/4 IQ sample
                           (LPVOID)PCIE_RC_MEM,
                           FPGA_RAD_TX_FIFO_SIZE,
                           0);

            pMdma = pMdma->Next;
        }

        pCtx->TxBufPos += FPGA_RAD_TX_FIFO_SIZE;
    }
    else
    {
        // In this case we need to copy data only if
        // pattern is used, in other case copy operation is not needed

        if (nCopyInMaster)
        {
            // the descriptors are already allocated
            // and can be used to program
            // antenna#0 - the first SF (61440 bytes)

            MdmaSetCopy(pMdma);
            MdmaSetDataEx (pMdma,
                buf,
                FPGA_RAD_TX_FIFO_SIZE * 4,  // IQ sample (61440 bytes)
                (LPVOID)pCtx->TxBuf,
                FPGA_RAD_TX_FIFO_SIZE * 4,
                0);

            pMdma = pMdma->Next;

            // the descriptors are already allocated
            // and can be used to program
            // antenna#1 - the first 1/4 of FIFO

            if (pCtx->AntennaNum == 2)
            {
                MdmaSetCopy(pMdma);
                MdmaSetDataEx (pMdma,
                    buf1,
                    FPGA_RAD_TX_FIFO_SIZE * 4,  // send 1/4 IQ sample
                    (LPVOID)(pCtx->TxBuf + pCtx->TxBufSize),
                    FPGA_RAD_TX_FIFO_SIZE * 4,
                    0);

                pMdma = pMdma->Next;
            }

            pCtx->TxBufPos += (FPGA_RAD_TX_FIFO_SIZE * 4);
        }
    }

    pCtx->IrqTxTicks = pCtx->IrqTxSubTicks  = GetTIMETICK();
    pCtx->IrqRxTicks = GetTIMETICK();
    pCtx->TxMdmaComplete = 1; // the first initialization
    pCtx->StopIrq        = 0;
    pCtx->TxIrqNum       = 0;
    pCtx->RxIrqNum       = 0;

    #if TRACE_NUM > 0
    tx_irq_ticks [tx_irq_idx ++] = pCtx->IrqTxTicks;
    #endif

    if (pBeginMdma != NULL)
    {
        #ifdef MDMA_TX_MEASURE_TIME
            PMDMA_FDESC pEnd = pBeginMdma, tmp = pBeginMdma;
            UINT32 t, num = 1;

            uart_printf ("\r\n");

            while (tmp != NULL)
            {
                uart_printf ("  IN:%08x  SIZE:%d\r\n", tmp->IoBdesc[1].BPtr, tmp->IoBdesc[1].BCtrl & 0xFFFF);
                uart_printf (" OUT:%08x  SIZE:%d\r\n", tmp->IoBdesc[0].BPtr, tmp->IoBdesc[0].BCtrl & 0xFFFF);

                tmp = tmp->Next;

                uart_printf ("\r\n");
            }

            while (pEnd->Next != NULL)
            {
                pEnd = pEnd->Next;
                num++;
            }

            t = MxGetTicks();
        #endif

        #ifdef EVM_FPGA_REINIT_TIMER_ON_FIRST_RX
        TimerDrvRestart();
        #endif

        pCtx->MdmaIsDone = 0;
        rc = MdmaRunEx(MDMA_HW_RAD, pBeginMdma, EvmMdmaTxCb, pCtx);

        #ifdef MDMA_TX_MEASURE_TIME
            while (pEnd->HwContext.StatusComp == 0)
            {
                _NOP();
            }

            t = MxGetTicks() - t;
            printf ("Time=%d, num=%d\r\n", t, num);
        #endif
    }
    else
    {
        pCtx->MdmaIsDone = 1;
    }

    // for testmac it should be uncommented
    //pCtx->TxBufPos = 0;

    if (IS_PROF_PCIE_MASTER(pCtx) || IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        PMDMA_FDESC pLast = pBeginMdma;

        if (pCtx->MdmaIsDone == 0)
        {
            while (pLast->Next != NULL)
                pLast = pLast->Next;
        }

        // We need to wait till MDMA has completed pattern copy
        // if pattern is specified

        while (pCtx->MdmaIsDone == 0)
        {
            if (pLast->HwContext.StatusComp != 0)
                break;

            _NOP();
            _NOP();
        }

        #ifdef EVM_FPGA_REINIT_TIMER_ON_FIRST_RX
        TimerDrvRestart();
        #endif

        pCtx->Regs->PcieNumOutReq       = 8;
        pCtx->Regs->PcieIntEnable       = IS_PROF_PCIE_MASTER_IRQ(pCtx);
        pCtx->Regs->TxPcieMasterEnable  = 1;

    }

    if (!IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        if (pCtx->Regs->IntStat & RFPGA_IM_RX)
        {
           pCtx->Regs->IntStat |= RFPGA_IM_RX;
           pCtx->Regs->IntStat &= ~RFPGA_IM_RX;
        }

        if (pCtx->Regs->IntStat & RFPGA_IM_TX)
        {
           pCtx->Regs->IntStat |= RFPGA_IM_TX;
           pCtx->Regs->IntStat &= ~RFPGA_IM_TX;
        }

        pCtx->Regs->IntMask &= ~(RFPGA_IM_TX | RFPGA_IM_RX);
    }

    return rc;
}

/** @brief This function fills some buffer with specific pattern
           the pattern is an incremented counter that can be easily
           checked by receiver

    @param pInBuf [in] - the pointer to the buffer
    @param nSize  [in] - the size of buffer in 32bits word

    */

void EvmFpgaFillSFByPattern (PUINT32 pInBuf, UINT32 nSize)
{
    UINT32 i = 0;
    UINT32 iv = 0, qv = 0;

    while(i < nSize)
    {
        pInBuf [i] = ((qv << 16) | iv);

        iv = (iv == 0xFFFF) ? 0 : iv+0x10;
        qv = (qv == 0xFFFF) ? 0 : qv+0x10;

        i++;
    }
}

/** @brief Function to release the Radio FPGA's internal DLL and wait for lock OK status */
void FpgaWaitRadioDllLock(void)
{
    UINT8 DLLrst;
    PRADIOFPGA pRadFpga = (PRADIOFPGA)FPGA_BASEADDR; // Get FPGA expansion bus base address for R/W of registers

    // Release FPGA's DLL from Reset
    //*(PVUINT8)(FPGA_BASEADDR + 0x388) = 0;   // resetting DLL :rajiv
    pRadFpga->DllControl = 0; // Reset DLL

    // Wait for FPGA DLL to lock
    MxDelayTicks (2 * 150000); //TEMP FOR TEST, WAIT 2 MILLISECONDS BEFORE CHECKING DLL STATUS

    do
    {
        //DLLrst = *(PVUINT8)(FPGA_BASEADDR + 0x384);
        DLLrst = pRadFpga->DllLockStatus;
    }
    while ((DLLrst & 1) == 0);   // wait till Locked :rajiv

}

/** @brief Function performs Soft Reset for FPGA*/
void EvmFpgaSoftRest(void)
{
    EvmFpgaCtx *pCtx = EvmGetCtx ();
    PRADIOFPGA pRadFpga = pCtx->Regs;

    //
    // Soft Reset the FPGA
    //
    if (pCtx->FpgaID != FPGA_CODE_VERSION_SISO)
    {
        // Reset FPGA
        pRadFpga->FpgaSoftReset = 0x69;
        // wait 10 ms
        SysDelay(10);
        // Release FPGA reset
        pRadFpga->FpgaSoftReset = 0;
        // Reset FPGA DLL
        pRadFpga->DllControl = 1;
        // wait 10 ms
        SysDelay(10);
        // Release FPGA DLL reset
        pRadFpga->DllControl = 0;
        uart_printf ("EVM-FPGA: Soft reset... OK\r\n");
    }
}

/** @brief This function should be called to initialize FPGA radio module
           and start using radio board

    @param pCfg [in] - the configuration parameters
    @return [MXRC] an error code */
MXRC EvmFpgaInit(EvmCfg * pCfg)
{
    PCIECFG cfg;
    EvmFpgaCtx * pCtx = EvmGetCtx ();
    MXRC rc = MX_OK;
    IRQTYPE irq;
    PUINT8 pTxBuf, pRxBuf;
    UINT32 nConnectNum;
    VUINT32 tmp, i;
    VUINT32 * pPCIStatPtr;

    _ASSERT_PTR(pCtx);

    pTxBuf = pCtx->TxBuf;
    pRxBuf = pCtx->RxBuf;
    pPCIStatPtr = pCtx->PCIIntStatusPtr;

    memset (pCtx, 0, sizeof (*pCtx));

    pCtx->TxBuf = pTxBuf;
    pCtx->RxBuf = pRxBuf;
    pCtx->PCIIntStatusPtr = pPCIStatPtr;

    #ifdef EVM_FPGA_INT_FORCED_PROF
    pCfg->IntProfID = EVM_FPGA_INT_FORCED_PROF;
    #endif

    // FORCED_MIMO_ENABLED to select MIMO for test purpose
    // in spite of SISO

    #ifndef FORCED_MIMO_ENABLED
    if (pCfg->AntennaNum == 0)
    #endif
    {
        pCfg->AntennaNum = 2;

        if (pCfg->pTxLog[0] != NULL && pCfg->pTxLog[1] == NULL)
            pCfg->pTxLog[1] = pCfg->pTxLog[0] + pCfg->TxLogSize;

        if (pCfg->pRxLog[0] != NULL && pCfg->pRxLog[1] == NULL)
            pCfg->pRxLog[1] = pCfg->pRxLog[0] + pCfg->RxLogSize;
    }

    if (FAILED(rc = MdmaRadInit()))
    {
        uart_printf ("RAD MDMA Initialization error\r\n");
        return rc;
    }

    if (pCfg->CtrlMask & EVM_FPGA_CTRL_TX0_LOG)
    {
        if (pCfg->pTxLog[0] == NULL || pCfg->TxLogSize == 0)
        {
            rc = RC_EVM_FPGA_LOG_PARAM_ERROR;
            _ASSERT_RC(rc);
            return rc;
        }
    }

    if ((pCfg->CtrlMask & EVM_FPGA_CTRL_TX1_LOG) && pCfg->AntennaNum == 2)
    {
        if (pCfg->pTxLog[1] == NULL || pCfg->TxLogSize == 0)
        {
            rc = RC_EVM_FPGA_LOG_PARAM_ERROR;
            _ASSERT_RC(rc);
            return rc;
        }
    }

    if ((pCfg->CtrlMask & EVM_FPGA_CTRL_RX0_LOG))
    {
        if (pCfg->pRxLog[0] == NULL || pCfg->RxLogSize == 0)
        {
            rc = RC_EVM_FPGA_LOG_PARAM_ERROR;
            _ASSERT_RC(rc);
            return rc;
        }
    }

    if (pCfg->CtrlMask & EVM_FPGA_CTRL_RX1_LOG && pCfg->AntennaNum == 2)
    {
        if (pCfg->pRxLog[1] == NULL || pCfg->RxLogSize == 0)
        {
            rc = RC_EVM_FPGA_LOG_PARAM_ERROR;
            _ASSERT_RC(rc);
            return rc;
        }
    }

    // Main driver initialization

    pCtx->AntennaNum    = pCfg->AntennaNum;
    pCtx->IntProfID     = pCfg->IntProfID;

    pCtx->MaskCtrl      = pCfg->CtrlMask;
    pCtx->pRxCbCtx      = pCfg->pRxCbCtx;
    pCtx->pTxCbCtx      = pCfg->pTxCbCtx;

    for (i = 0; i < 4; i++)
    {
        pCtx->pTxLog[i] = pCfg->pTxLog[i];
        pCtx->pRxLog[i] = pCfg->pRxLog[i];
    }

    pCtx->TxLogSize    = pCfg->TxLogSize;
    pCtx->RxLogSize    = pCfg->RxLogSize;

    if ((pCtx->MaskCtrl & EVM_FPGA_CTRL_CHECK) == 0)
    {
        pCtx->pRxDMACallBack= pCfg->pRxDMACallBack;
        pCtx->pTxDMACallBack= pCfg->pTxDMACallBack;
    }

    if (pCtx->pTxLog[0] != NULL)
        memset (pCtx->pTxLog[0], 0xA0, pCtx->TxLogSize);

    if (pCtx->pTxLog[1] != NULL)
        memset (pCtx->pTxLog[1], 0xA1, pCtx->TxLogSize);

    if (pCtx->pRxLog[0] != NULL)
        memset (pCtx->pRxLog[0], 0xB0, pCtx->RxLogSize);

    if (pCtx->pRxLog[1] != NULL)
        memset (pCtx->pRxLog[1], 0xB1, pCtx->RxLogSize);

    // Setting base address
    pCtx->Regs = (PRADIOFPGA)FPGA_BASEADDR;

    pCtx->FpgaID = pCtx->Regs->FpgaId;

    // If this is SISO version of FPGA code
    // to check for 1xAC and to provide SISO specific initialziation

    if (pCtx->IntProfID == EVM_FPGA_INT_PROF_DEF)
    {
        if (pCtx->FpgaID == FPGA_CODE_VERSION_SISO)
        {
            if (pCfg->AntennaNum != 1)
            {
                uart_printf ("MIMO is not supported by FPGA\r\n");
                return RC_EVM_FPGA_MIMO_IS_NOT_SUP;
            }
        }
        else
        {
            // for the new code we can select SISO or MIMO
            // configuration, so specific new version FPGA code
            // is done here
        }
    }

    EvmFpgaSoftRest();

    //
    // Disable FPGA interrupts in the FPGA itself
    //

    if (!IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        pCtx->Regs->IntMask |= (1<<7) | (1<<5); // 0x001A: disable interrupts
    }
    else
    {
        if (pCtx->PCIIntStatusPtr == NULL)
        {
            // We need to allocate the buffers for the PCIe INT status
            pCtx->PCIIntStatusPtr = (VUINT32*)MxHeapAlloc (DspHeap, sizeof (VUINT32)*2);

            if (pCtx->PCIIntStatusPtr == NULL)
            {
                _ASSERT_PTR(pCtx->PCIIntStatusPtr);
                uart_printf ("EVMFPGA: Cannot allocate PCIeINT status buffers\r\n");

                return RC_EVM_FPGA_ALLOC_PCIE_STAT;
            }
        }

        pCtx->PCIIntStatusPtr[0] = 0;
        pCtx->PCIIntStatusPtr[1] = 0;

        pCtx->Regs->PcieIntStatAddr = (UINT32)pCtx->PCIIntStatusPtr;
    }

    //
    // Set TX and RX FIFO and radio control to shutdown/disabled state
    //
    pCtx->Regs->AdcFifoCtrl    = 0; // 0x01E: Disable RX FIFO(s) (two antennas, one FIFO per antenna)
    pCtx->Regs->DacFifoCtrl    = 0; // 0x01F: Disable TX FIFO (shared by both antennas on transmit) offset 0x1F, value 1
    pCtx->Regs->AxCEnable      = 0; // 0x201: Disable receiving data on one or both antennas
    pCtx->Regs->Rad0Ctrl       = 0; // 0x100: Disable radio module 0
    pCtx->Regs->Rad1Ctrl       = 0; // 0x110: Disable radio module 1 (NOTE: Not used and not currently supported)

    if (IS_PROF_DEF(pCtx) || IS_PROF_PCIE_MASTER(pCtx))
    {
        //
        // Clear any possible pending GPIO2 & GPIO6 interrupts (TX processing completed, RX data ready)
        //
        REG32(GPIO_INT_CLEAR_REG) |= (GPIO_6 | GPIO_2);

        //
        // Fully disable GPIO0 interrupts from occuring for FPGA
        //
        REG32(GPIO_INT_MASK_REG) &= ~(GPIO_6 | GPIO_2); // Disable GPIO interrupts
    }
    else if (IS_PROF_PROG_0_2(pCtx))
    {
        //
        // Clear any possible pending GPIO0 & GPIO2 interrupts (TX processing completed, RX data ready)
        //
        REG32(GPIO_INT_CLEAR_REG) |= (GPIO_0 | GPIO_2);

        //
        // Fully disable GPIO0 & GPIO2 interrupts from occuring for FPGA
        //
        REG32(GPIO_INT_MASK_REG) &= ~(GPIO_0 | GPIO_2); // Disable GPIO interrupts
    }

    //
    // Put in FPGA internal loopback mode so we don't listen to Radio noise
    // We put back in mission mode, once the Radio is fully programmed (happens at end of this function)
    //pCtx->Regs->FpgaLoopbackControl = 1;

    // To allocate the 2 TX buffers for 2 ms, and to provide the address to the PHY
    pCtx->TxBufSize = FPGA_RAD_TX_BUF_SIZE;
    pCtx->RxBufSize = FPGA_RAD_RX_BUF_SIZE;

    // This buffer is designed to handle 2-TX and 2-RX antennas, per 2 sub-buffers per TX and RX
    // totally 8 buffers: 2TX#0 + 2TX#1 + 2RX#0 + 2RX#1

    if (pCtx->TxBuf == NULL)
    {
        pCtx->TxBuf = (PUINT8)QueryCramIqBuf(0, 0, 0);
    }
    else if ((UINT32)pCtx->TxBuf != CPRI_DMA_BUF_ADDR)
    {
        uart_printf ("TX/RX buffers reinitialization ... ");
        HeapFreeByPtr (pCtx->TxBuf);
        pCtx->TxBuf = (PUINT8)QueryCramIqBuf(0, 0, 0);
        uart_printf ("OK\r\n");
    }

    if(pCtx->TxBuf == NULL)
    {
        rc = RC_EVM_FPGA_NO_MEMORY;
        _ASSERT_RC(rc);
        return rc;
    }

    pCtx->RxBuf  = (PUINT8)CPRI_RXDMA_BUF_ADDR;

    if (pCtx->RxBuf == NULL)
    {
        rc = RC_EVM_FPGA_NO_MEMORY;
        _ASSERT_RC(rc);
        return rc;
    }

    if (IS_PROF_PCIE_MASTER(pCtx) || IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        pCtx->Regs->AxCTxBaseAddr[0] = (UINT32)pCtx->TxBuf;
        // the size of two sub-buffes of one antenna in IQ samples
        // the size of one buffer is 15360 (IQs)
        // so two buffers are 15360*2
        pCtx->Regs->AxCTxBufSize [0] = FPGA_RAD_TX_FIFO_SIZE * 2;

        pCtx->Regs->AxCTxBaseAddr[1] = (UINT32)pCtx->TxBuf + FPGA_RAD_TX_BUF_SIZE;
        // the size of two sub-buffes of one antenna in IQ samples
        // the size of one buffer is 15360 (IQs)
        // so two buffers are 15360*2
        pCtx->Regs->AxCTxBufSize [1] = FPGA_RAD_TX_FIFO_SIZE * 2;
    }

    pCtx->RxBuf = (PUINT8)ROUND((UINT32)pCtx->RxBuf, 0x1000);

    uart_printf("\r\n==========================================");
    uart_printf("\r\n   FPGA version %d.%d.%d (on CPU-%d/%d)", pCtx->FpgaID >> 16, pCtx->FpgaID & 0xFFFF, pCtx->Regs->InternalTestRevision, MxGetCpuID (), EVM_FPGA_INT_HANDLER_CPU_ID);
    uart_printf("\r\n==========================================\r\n");
    uart_printf ("   Radio, EVM-FPGA init, ctrl=0x%x, [%s]\r\n", pCtx->MaskCtrl, pCtx->AntennaNum == 2 ? "MIMO" : "SISO");

    if (IS_PROF_DEF(pCtx))
    {
        uart_printf ("   EVM-FPGA INT profile [%s]", "GPIO-6/GPIO-2");
    }

    if (IS_PROF_PROG_0_2(pCtx))
    {
        uart_printf ("   EVM-FPGA INT profile [%s]", "GPIO-0/GPIO-2");
    }

    if (IS_PROF_PCIE_MASTER(pCtx))
    {
        uart_printf ("   EVM-FPGA INT profile [%s]", "PCIe master/GPIO-6/GPIO-2");
    }

    if (IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        uart_printf ("   EVM-FPGA INT profile [%s]", "PCIe master/PCIe INT");
    }

    if (IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        uart_printf ("\r\n   PCIe INT status PTR [0x%x]", pCtx->Regs->PcieIntStatAddr);
    }

    #ifdef EVM_FPGA_REINIT_TIMER_ON_FIRST_RX
        uart_printf ("\r\n   Timer synchronization is enabled");
    #endif

    #ifdef DL_IQ_PATTERN_ENABLED
        {
            extern unsigned int iqnum;
            extern const signed short iqbuf[];
            uart_printf ("\r\n   DL IQ pattern, [0x%x / %d]", iqbuf, iqnum);
        }
    #endif

    #ifdef AN0_TO_AN0_AN1
            if (pCtx->AntennaNum == 2)
                uart_printf ("\r\n   TX-0 is mapped to port0/1");
    #endif

    #ifdef EVM_FPGA_TX_LOG_1_4_SF
        uart_printf ("\r\n   TX log is by using 1/4 of SF");
    #endif

    uart_printf("\r\n==========================================\r\n");

    if (pCtx->AntennaNum == 2)
    {
        uart_printf (" - TX PTR[0]: 0x%08x | TX PTR[1]: 0x%08x\r\n", pCtx->TxBuf, pCtx->TxBuf + pCtx->TxBufSize);
        uart_printf (" - RX PTR[0]: 0x%08x | RX PTR[1]: 0x%08x\r\n", pCtx->RxBuf, pCtx->RxBuf + pCtx->RxBufSize);
    }
    else
    {
        uart_printf (" - TX PTR[0]: 0x%08x\r\n", pCtx->TxBuf);
        uart_printf (" - RX PTR[0]: 0x%08x\r\n", pCtx->RxBuf);
    }

    uart_printf (" - TX LOG[0]: 0x%08x | size:[%d] : [%s][%s]\r\n",
                        pCtx->pTxLog[0],
                        pCtx->TxLogSize,
                        (pCtx->MaskCtrl & EVM_FPGA_CTRL_TX0_LOG) ? "enabled" : "disabled",
                        (pCtx->MaskCtrl & EVM_FPGA_CTRL_TX_LOG_RR) ? "RR" : "non-RR");

    if (pCtx->AntennaNum == 2)
    {
        uart_printf (" - TX LOG[1]: 0x%08x | size:[%d] : [%s][%s]\r\n",
                            pCtx->pTxLog[1],
                            pCtx->TxLogSize,
                            (pCtx->MaskCtrl & EVM_FPGA_CTRL_TX1_LOG) ? "enabled" : "disabled",
                            (pCtx->MaskCtrl & EVM_FPGA_CTRL_TX_LOG_RR) ? "RR" : "non-RR");
    }

    uart_printf (" - RX LOG[0]: 0x%08x | size:[%d] : [%s][%s]\r\n",
                        pCtx->pRxLog[0],
                        pCtx->RxLogSize,
                        (pCtx->MaskCtrl & EVM_FPGA_CTRL_RX0_LOG) ? "enabled" : "disabled",
                        (pCtx->MaskCtrl & EVM_FPGA_CTRL_RX_LOG_RR) ? "RR" : "non-RR");

    if (pCtx->AntennaNum == 2)
    {
        uart_printf (" - RX LOG[1]: 0x%08x | size:[%d] : [%s][%s]\r\n",
                            pCtx->pRxLog[1],
                            pCtx->RxLogSize,
                            (pCtx->MaskCtrl & EVM_FPGA_CTRL_RX1_LOG) ? "enabled" : "disabled",
                            (pCtx->MaskCtrl & EVM_FPGA_CTRL_RX_LOG_RR) ? "RR" : "non-RR");
    }

#ifdef Korean_BMT_RF_PCIE
    memset (pCtx->TxBuf, 0x01, pCtx->TxBufSize);
    memset (pCtx->RxBuf, 0xB0, pCtx->RxBufSize);

    if (pCtx->AntennaNum > 1)
    {
        memset (pCtx->TxBuf + pCtx->TxBufSize, 0x03, pCtx->TxBufSize);
        memset (pCtx->RxBuf + pCtx->RxBufSize, 0xB1, pCtx->RxBufSize);
    }
#else
    memset (pCtx->TxBuf, 0xC5, pCtx->TxBufSize);
    memset (pCtx->RxBuf, 0xB0, pCtx->RxBufSize);

    if (pCtx->AntennaNum > 1)
    {
        memset (pCtx->TxBuf + pCtx->TxBufSize, 0xA1, pCtx->TxBufSize);
        memset (pCtx->RxBuf + pCtx->RxBufSize, 0xB1, pCtx->RxBufSize);
    }

    if (pCtx->MaskCtrl & EVM_FPGA_CTRL_CHECK)
    {
        // to specify the pattern to test the board in loopback mode
        EvmFpgaFillSFByPattern ((PUINT32)pCtx->TxBuf, (pCtx->TxBufSize)/4);
    }
    else
    {
        //memset (pCtx->TxBuf, 0xA5, pCtx->TxBufSize);
        EvmFpgaFillSFByPattern ((PUINT32)pCtx->TxBuf, (pCtx->TxBufSize)/4);
    }
#endif

    pCtx->PCIeBaseMem = pCtx->RxBuf;

    memset (&cfg, 0, sizeof(cfg));

    // To initialize the PCIe
    cfg.BaseMem = (UINT32)pCtx->PCIeBaseMem;
    cfg.IrqCB   = NULL;
    cfg.Mode    = 1; // RC
    cfg.Lanes   = pCfg->PcieLines;
    cfg.pIrqData= NULL;

    if (IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        cfg.MSICfg = 1;
    }

    uart_printf (" - PCIe x%d lane(s) initialization ...", pCfg->PcieLines);

    if (pcie_inited == 0)
    {
        if (FAILED(rc = PCIeDrvInit (&cfg)))
        {
            uart_printf ("EvmFpga: Pcie initialization error, rc=%x\r\n", rc);
            _ASSERT_RC(rc);
            return rc;
        }
    }
    else
    {
        uart_printf ("PCIe reinitialization is OK\r\n");
    }

    uart_printf(" - PCIe connection ...");

    nConnectNum = 0;

    while (nConnectNum < 3)
    {
#ifdef Korean_BMT_RF_PCIE
    	if (FAILED(rc = PCIeSrdsDrvConnect(EVM_FPGA_PCIE_CONNECTION_TIME, cfg.SerDesID)))
#else
    	if (FAILED(rc = PCIeDrvConnect(EVM_FPGA_PCIE_CONNECTION_TIME)))
#endif
    	{
    		uart_printf ("#%d", nConnectNum);
            nConnectNum++;
            continue;
    	}

        break;
    }

	if (nConnectNum >= 3)
    {
#ifdef Korean_BMT_RF_PCIE
        uart_printf (" FAILED, rc=%x (link sts = %d)\r\n", 
				rc, PCieDrvGetLinkStatus());
#else
        uart_printf (" FAILED, rc=%x\r\n", rc);
#endif
        _ASSERT_RC(rc);
        return rc;
    }

    if (pcie_inited == 0)
    {
    if (FAILED(rc = PCIeDrvSelectMaster ()))
    {
        uart_printf ("EvmFpga: Pcie select master, rc=%x\r\n", rc);
        _ASSERT_RC(rc);
        return rc;
    }

        // old SISO FPGA is not stable for re-init, so prevent crash here
        if (pCtx->FpgaID == FPGA_CODE_VERSION_SISO)
            pcie_inited = 1;
    }
    else
    {
        uart_printf ("PCIe reinitialization is OK\r\n");
    }

    uart_printf("OK\r\n");
    uart_printf("==========================================\r\n");

    pCtx->bInited = 1;
    pCtx->MasterCpu= MxGetCpuID ();

#ifdef EVM_TIMER_EMUL

    uart_printf (" - EVM-TIMER emulator for %dMs ...", EVM_TIMER_EMUL_PERIOD);

    TimerDrvSetProc (EVM_TIMER_EMUL_PERIOD, EVM_TIMER_EMUL_PERIOD, EvmTestTimerProc, pCtx);

    uart_printf ("OK\r\n");
#else

    if (1)
    {
        UINT32 TxIrqGpioEnaCfg = (EvmFpgaApiConfigSet)? EvmFpgaApiConfig.TxIrqGpioEna : pCfg->TxIrqGpioEna;
        UINT32 TxIrqGpioPortCfg = (EvmFpgaApiConfigSet)? EvmFpgaApiConfig.TxIrqGpioPort: pCfg->TxIrqGpioPort;
        UINT32 TxIrqGpioToggleDelays = (EvmFpgaApiConfigSet)? EvmFpgaApiConfig.TxIrqGpioToggleDelays: pCfg->TxIrqGpioToggleDelays;

        if(TxIrqGpioEnaCfg)
        {
            pCtx->TxIrqGpioEna = 1;
            pCtx->TxIrqGpioPort = TxIrqGpioPortCfg;
            REG32(GPIO_PIN_SELECT_REG) |= (1 << pCtx->TxIrqGpioPort); // GPIO functionality
            REG32(GPIO_OE_REG) |= (1 << pCtx->TxIrqGpioPort); // enable
            REG32(GPIO_OUTPUT_REG) &= ~(1 << pCtx->TxIrqGpioPort); // low at start

            pCtx->TxIrqGpioToggleDelays[0] = TxIrqGpioToggleDelays >> 16;
            if(pCtx->TxIrqGpioToggleDelays[0] == 0)
                pCtx->TxIrqGpioToggleDelays[0] = 1;
            pCtx->TxIrqGpioToggleDelays[1] = TxIrqGpioToggleDelays & 0xFFFF;
            if(pCtx->TxIrqGpioToggleDelays[1] == 0)
                pCtx->TxIrqGpioToggleDelays[1] = 1;
            pCtx->TxIrqGpioToggleDelayIdx = 0;
            pCtx->TxIrqGpioToggleCntr = 0;
        }
    }
    else
    {
        pCtx->TxIrqGpioEna = 0;
    }

    if (1)
    {
        UINT32 RxIrqGpioEnaCfg = (EvmFpgaApiConfigSet)? EvmFpgaApiConfig.RxIrqGpioEna : pCfg->RxIrqGpioEna;
        UINT32 RxIrqGpioPortCfg = (EvmFpgaApiConfigSet)? EvmFpgaApiConfig.RxIrqGpioPort: pCfg->RxIrqGpioPort;
        UINT32 RxIrqGpioToggleDelays = (EvmFpgaApiConfigSet)? EvmFpgaApiConfig.RxIrqGpioToggleDelays: pCfg->RxIrqGpioToggleDelays;

        if(RxIrqGpioEnaCfg)
        {
            pCtx->RxIrqGpioEna = 1;
            pCtx->RxIrqGpioPort = RxIrqGpioPortCfg;
            REG32(GPIO_PIN_SELECT_REG) |= (1 << pCtx->RxIrqGpioPort); // GPIO functionality
            REG32(GPIO_OE_REG) |= (1 << pCtx->RxIrqGpioPort); // enable
            REG32(GPIO_OUTPUT_REG) &= ~(1 << pCtx->RxIrqGpioPort); // low at start

            pCtx->RxIrqGpioToggleDelays[0] = RxIrqGpioToggleDelays >> 16;
            if(pCtx->RxIrqGpioToggleDelays[0] == 0)
                pCtx->RxIrqGpioToggleDelays[0] = 1;
            pCtx->RxIrqGpioToggleDelays[1] = RxIrqGpioToggleDelays & 0xFFFF;
            if(pCtx->RxIrqGpioToggleDelays[1] == 0)
                pCtx->RxIrqGpioToggleDelays[1] = 1;
            pCtx->RxIrqGpioToggleDelayIdx = 0;
            pCtx->RxIrqGpioToggleCntr = 0;
        }
    }
    else
    {
        pCtx->RxIrqGpioEna = 0;
    }

    irq = ARM_INT_disable();

    if (IS_PROF_DEF(pCtx) || IS_PROF_PCIE_MASTER(pCtx))
    {
        if (FAILED(rc = MxSetIntHandlerEx(EVM_FPGA_INT_HANDLER_CPU_ID, HAL_IRQ_GPIO_COMB, (ITYPE)(INT_TYPE_IRQ | INT_TYPE_EDGE), PRI_EVMFPGA, EvmFpgaIrqHandler, pCtx)))
        {
            ARM_INT_restore(irq);
            return rc;
        }

        if (FAILED(rc = MxEnableIntEx(EVM_FPGA_INT_HANDLER_CPU_ID, HAL_IRQ_GPIO_COMB)))
        {
            ARM_INT_restore(irq);
            return rc;
        }
    }
    else if (IS_PROF_PROG_0_2(pCtx))
    {
        // the same interrupt handler is used to handle TX and RX signals
        // there is no any impact to the system performance

        if (FAILED(rc = MxSetIntHandlerEx(EVM_FPGA_INT_HANDLER_CPU_ID, HAL_IRQ_GPIO_0, (ITYPE)(INT_TYPE_IRQ | INT_TYPE_EDGE), PRI_EVMFPGA, EvmFpgaIrqHandler, pCtx)))
        {
            ARM_INT_restore(irq);
            return rc;
        }

        if (FAILED(rc = MxSetIntHandlerEx(EVM_FPGA_INT_HANDLER_CPU_ID, HAL_IRQ_GPIO_2, (ITYPE)(INT_TYPE_IRQ | INT_TYPE_EDGE), PRI_EVMFPGA, EvmFpgaIrqHandler, pCtx)))
        {
            ARM_INT_restore(irq);
            return rc;
        }

        if (FAILED(rc = MxEnableIntEx(EVM_FPGA_INT_HANDLER_CPU_ID, HAL_IRQ_GPIO_0)))
        {
            ARM_INT_restore(irq);
            return rc;
        }

        if (FAILED(rc = MxEnableIntEx(EVM_FPGA_INT_HANDLER_CPU_ID, HAL_IRQ_GPIO_2)))
        {
            ARM_INT_restore(irq);
            return rc;
        }
    }
    else if (IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        if (FAILED(rc = MxSetIntHandlerEx(EVM_FPGA_INT_HANDLER_CPU_ID, HAL_IRQ_PCIE_MSI, (ITYPE)(INT_TYPE_IRQ), PRI_EVMFPGA, EvmFpgaIrqHandler, pCtx)))
        {
            ARM_INT_restore(irq);
            return rc;
        }

        if (FAILED(rc = MxEnableIntEx(EVM_FPGA_INT_HANDLER_CPU_ID, HAL_IRQ_PCIE_MSI)))
        {
            ARM_INT_restore(irq);
            return rc;
        }
    }
    else
    {
        ARM_INT_restore(irq);
        return RC_EVM_FPGA_INT_PROF_ERR;
    }

    pCtx->IrqRxTicks = GetTIMETICK();

    if (IS_PROF_DEF(pCtx) || IS_PROF_PCIE_MASTER(pCtx) || IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        if (pCtx->FpgaID != FPGA_CODE_VERSION_SISO)
        {
            if (pCtx->AntennaNum == 2)
            {
                pCtx->Regs->FifoStatusSel |= 1<<14; // 2 antenna mode
            }
            else
            {
                pCtx->Regs->FifoStatusSel &= ~(1<<14); // 2 antenna mode
            }

            // to point to 2 RX buffers, we need to setup 0 and 1 but to be sure
            // the code provides all 4 parameters

            pCtx->Regs->AxCBufSize[0]       = pCtx->RxBufSize / sizeof (UINT32);
            pCtx->Regs->AxCBufSize[1]       = pCtx->RxBufSize / sizeof (UINT32);
        }
        else
        {
            //
            // FPGA supports SISO only and has specific values
            // to be set (including some leftover code that
            // requires some bits related to second antenna even
            // though second antenna is not used, if not set
            // the older SISO FPGA does not run correctly).
            //
            pCtx->Regs->AxCBufSize[0]       = pCtx->RxBufSize / sizeof (UINT32);
            pCtx->Regs->AxCBufSize[2]       = pCtx->RxBufSize / sizeof (UINT32);
        }
    }
    else
    {
        // profile-1 (GPIO-0/GPIO-2) specific configuration
        // FPGA code specific

        pCtx->Regs->AxCBufSize[0]       = pCtx->RxBufSize / sizeof (UINT32);
        pCtx->Regs->AxCBufSize[1]       = pCtx->RxBufSize / sizeof (UINT32);
    }

    pCtx->Regs->AxCPcieBaseAddr[0]  = (UINT32)pCtx->RxBuf;

    if (pCtx->AntennaNum == 2)
    {
        pCtx->Regs->AxCPcieBaseAddr[1]  = (UINT32)pCtx->RxBuf + pCtx->RxBufSize; // RxBufSize == 2-sub buffers per RX
    }

    pCtx->Regs->DacToAdcWriteSyncDelay = 0; // 10 ??

    if (pCtx->IntProfID == EVM_FPGA_INT_PROF_DEF || IS_PROF_PCIE_MASTER(pCtx))
    {
        REG32(GPIO_INT_CONF_REG_0) |= (GPIO_RISING_EDGE << (6*2)) | (GPIO_RISING_EDGE << (2*2)); // rising edge interrupt type for GPIO 2 and 6
        REG32(GPIO_INT_CLEAR_REG)   =   GPIO_6 | GPIO_2;
        REG32(GPIO_INT_CLEAR_REG)  &= ~(GPIO_6 | GPIO_2);
        REG32(GPIO_INT_MASK_REG)   |=   GPIO_6 | GPIO_2;
    }
    else
    {
        REG32(GPIO_INT_CONF_REG_0) |= (GPIO_RISING_EDGE << (0*2)) | (GPIO_RISING_EDGE << (2*2)); // rising edge interrupt type for GPIO 2 and 6
        REG32(GPIO_INT_CLEAR_REG)   =   GPIO_0 | GPIO_2;
        REG32(GPIO_INT_CLEAR_REG)  &= ~(GPIO_0 | GPIO_2);
        REG32(GPIO_INT_MASK_REG)   |=   GPIO_0 | GPIO_2;
    }

    if (pCtx->FpgaID != FPGA_CODE_VERSION_SISO && pCtx->IntProfID != EVM_FPGA_INT_PROG_0_2)
    {
        // to wait Radio DLL lock for FPGA MIMO code revision
        FpgaWaitRadioDllLock();
    }

    //
    // Enable Radio interface 0 (1 or 2 antennas) register (offset 0x100)
    // Set bits 0 and 1, then also set bit 3, then also set bit 5
    //
    pCtx->Regs->Rad0Ctrl  = RFPGA_RADIO_MODULE_ENABLE   /*1<<0*/
                      //| RFPGA_PERFORM_RX_STOP_TX    /*1<<2*/
                        | RFPGA_PERFORM_FULL_DUPLEX;  /*1<<1*/
    pCtx->Regs->Rad0Ctrl |= RFPGA_ENABLE_DATA_TRANSFER; /*1<<3*/
    pCtx->Regs->Rad0Ctrl |= RFPGA_ENABLE_AGC;           /*1<<5*/
    //
    // Setup Radio interface 1 (NOT USED)
    //
    pCtx->Regs->Rad1Ctrl = RFPGA_RADIO_MODULE_ENABLE    /*1<<0*/
                       | RFPGA_PERFORM_RX_STOP_TX     /*1<<2*/
                       | RFPGA_ENABLE_DATA_TRANSFER   /*1<<3*/
                       ;                              /*0xd;*/ // 1101

    if (pCtx->IntProfID == EVM_FPGA_INT_PROF_DEF || IS_PROF_PCIE_MASTER(pCtx) || IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        pCtx->Regs->AdcFifoCtrl = 3;  //bit-0: allow data written to FIFO
                                      //bit-1: allow sending PCIe frames to T4K

        if (pCtx->AntennaNum == 2)
        {
            #ifdef EVM_DISABLE_RX_1
                // Set register offsets 0x201 and 0x01E to 3 (bits 0 and 1 TRUE)
                pCtx->Regs->AxCEnable   = RFPGA_ANTENNA_0; // enable receiving data on RX0 antenna
            #else
                // Set register offsets 0x201 and 0x01E to 3 (bits 0 and 1 TRUE)
                pCtx->Regs->AxCEnable   = RFPGA_ANTENNA_0 | RFPGA_ANTENNA_1; // enable receiving data on both antennas
            #endif
        }
        else
        {
            // Set register offsets 0x201 and 0x01E to 3 (bits 0 and 1 TRUE)
            pCtx->Regs->AxCEnable   = RFPGA_ANTENNA_0; // enable receiving data on both antennas
        }
    }
    else
    {
        if (pCtx->AntennaNum == 2)
    	{
        	pCtx->Regs->AdcFifoCtrl = RFPGA_ANTENNA_0 | RFPGA_ANTENNA_1;

            // @brief 0x01C-0x01D: Interrupt Status (16 bits)
    		REG8(FPGA_BASEADDR + 0x1d) = RFPGA_ANTENNA_0 | RFPGA_ANTENNA_1;
    	}
    	else
    	{
        	pCtx->Regs->AdcFifoCtrl = RFPGA_ANTENNA_0;

            // @brief 0x01C-0x01D: Interrupt Status (16 bits)
    		REG8(FPGA_BASEADDR + 0x1d) = RFPGA_ANTENNA_0;
    	}
    }

    pCtx->Regs->DacFifoCtrl = 1; // Enable TX FIFO (shared by both antennas on transmit) offset 0x1F, value 1

    if (pCtx->FpgaID != FPGA_CODE_VERSION_SISO && !IS_PROF_PROG_0_2(pCtx))
    {
        pCtx->Regs->FpgaLoopbackControl = 0;
        pCtx->Regs->TxRxDataPathEnable = 0x11; // Enable TX and RX Data path (0x10:TX, 0x01:RX)
    }

    if (pCtx->MaskCtrl & EVM_FPGA_CTRL_AUTO_RUN)
    {
        MxDelayTicks (US_TO_TICKS(533));
        rc = EvmFpgaRunTx (pCtx);
    }

    ARM_INT_restore (irq);

#endif

    return rc;

}

EvmFpgaCtx * EvmFpgaGetCtx (void)
{
    return EvmGetCtx();
}

/** @brief This function just stops RX and TX operations
            and can be called when device is inited

    @return [MXRC] an error code */

MXRC EvmFpgaStopRadio(void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;

#if TRACE_NUM > 0
    UINT32 i;
#endif

    _ASSERT_PTR(pCtx);

    if (!pCtx->bInited)
        return RC_EVM_FPGA_NOT_INITED;

    #if EVM_FPGA_TIMER_TRACE_NUM > 0
        EvmFpgaRemoveTraceTimer();
    #endif

#ifdef EVM_TIMER_EMUL
    TimerDrvSetProc (0, 0, NULL, NULL);
#endif

    irq = ARM_INT_disable();

    pCtx->StopIrq = 1;

    pCtx->Regs->IntMask |= (RFPGA_IM_RX) | (RFPGA_IM_TX);

    if (IS_PROF_PCIE_MASTER_IRQ(pCtx))
    {
        REG32(RAD_CFG_RC_MSI_CATCHER) = 0; // clear PCIe interrupt
    }

    if (EvmFpgaIsTxIrq(pCtx))
        EvmFpgaAckTxIrq(pCtx);

    if (EvmFpgaIsRxIrq(pCtx))
        EvmFpgaAckRxIrq(pCtx);

    // to reset registers
    pCtx->Regs->AxCEnable = 0;  // disable TX/RX operations
    
    //pCtx->Regs->AxCBufSize[0]= 0;
    //pCtx->Regs->AxCBufSize[1]= 0;
    //pCtx->Regs->AxCBufSize[2]= 0;
    //pCtx->Regs->AxCBufSize[3]= 0;
    //pCtx->Regs->AxCPcieBaseAddr[0]=0;
    
    pCtx->Regs->DacToAdcWriteSyncDelay = 6;
    pCtx->Regs->AdcFifoCtrl = 0; // ADC enable
    pCtx->Regs->DacFifoCtrl = 0;
    pCtx->Regs->Rad0Ctrl = 0;
    pCtx->Regs->Rad1Ctrl = 0; // 1101

    pCtx->TxBufPos = 0;
    pCtx->RxBufPos = 0;

#if TRACE_NUM > 0

    uart_printf ("\r\n");
    uart_printf ("ADI TX-IRQ : %d\r\n", evmirqnum_tx);
    MxDelayTicks (15000);
    uart_printf ("ADI RX-IRQ : %d\r\n", evmirqnum_rx);
    MxDelayTicks (15000);
    uart_printf ("ADI   IRQs : %d\r\n", evmirqnum);
    MxDelayTicks (15000);
    uart_printf ("\r\n");
    uart_printf ("MAX SUB-TX IRQ : %d\r\n", pCtx->MaxTxSubPeriod);
    MxDelayTicks (15000);
    uart_printf ("AVG SUB-TX IRQ : %d\r\n", pCtx->IrqSubTxPeriodSum / evmirqnum_tx);
    MxDelayTicks (15000);
    uart_printf ("MAX     TX IRQ : %d\r\n", pCtx->MaxTxPeriod);
    MxDelayTicks (15000);
    uart_printf ("MAX     RX IRQ : %d\r\n", pCtx->MaxRxPeriod);
    MxDelayTicks (15000);
    uart_printf ("AVG     RX IRQ : %d\r\n", pCtx->IrqRxPeriodSum / evmirqnum_rx);
    MxDelayTicks (15000);

    uart_printf ("(RX - TX) = %d\r\n",  - evm_rx_begin_tick - evm_tx_begin_tick);
    MxDelayTicks (15000);

    for (i = 0; i < max_tx_count; i++)
    {
        uart_printf ("TX-IRQ OVERTIME:[%d] happened on irq num [%d]\r\n", max_tx_p[i], max_tx_n[i]);
        MxDelayTicks (TICKS_TO_MS(50));
    }

    for (i = 0; i < max_rx_count; i++)
    {
        uart_printf ("RX-IRQ OVERTIME:[%d] happened on irq num [%d]\r\n", max_rx_p[i], max_rx_n[i]);
        MxDelayTicks (TICKS_TO_MS(50));
    }

    for (i = 0; i < tx_irq_idx; i++)
    {
        if (i == 0)
            uart_printf ("%02d. [%d]\r\n", i, tx_irq_ticks[i]);
        else
            uart_printf ("%02d. [%d], diff-[%d]\r\n", i, tx_irq_ticks[i], tx_irq_ticks[i] - tx_irq_ticks[i-1]);

        MxDelayTicks (TICKS_TO_MS(50));
    }

    for (i = 0; i < rx_irq_idx; i++)
    {
        if (i == 0)
            uart_printf ("%02d. [%d]\r\n", i, rx_irq_ticks[i]);
        else
            uart_printf ("%02d. [%d], diff-[%d]\r\n", i, rx_irq_ticks[i], rx_irq_ticks[i] - rx_irq_ticks[i-1]);

        MxDelayTicks (TICKS_TO_MS(50));
    }

#else
    uart_printf ("\r\n");
    uart_printf ("MAX SUB-TX IRQ : %d\r\n", pCtx->MaxTxSubPeriod);
    uart_printf ("MAX     TX IRQ : %d\r\n", pCtx->MaxTxPeriod);
    uart_printf ("MAX     RX IRQ : %d\r\n", pCtx->MaxRxPeriod);
#endif

    ARM_INT_restore(irq);

    return MX_OK;
}

/** @brief This function start TX/RX operations and has to be called
            after EVM initialization (see: EvmFpgaInit) if auto start was not
            specified

    @return [MXRC] an error code */

MXRC EvmFpgaStart (void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();

    _ASSERT_PTR(pCtx);

    if (!pCtx->bInited)
        return RC_EVM_FPGA_NOT_INITED;

    // to start the TX and RX
    return EvmFpgaRunTx (pCtx);
}

/** @brief This function returns the pointer to the TX buffer used to send data
    @param nAntenna [in] - an antenna ID
    @return [void*] the pointer to the buffer

    @NOTE : The EVM module should be initializeds */

void * EvmGetTxBuffer (UINT32 nAntenna)
{
    return EvmGetAntennaTxBuffer(nAntenna);
}

/** @brief This function returns the pointer to the TX buffer used to send data
    @param nAntenna [in] - an antenna ID
    @return [void*] the pointer to the buffer

    @NOTE : The EVM module should be initializeds */

void * EvmGetTxBufferEx(LPVOID pPtr, UINT32 nAntenna)
{
    return EvmGetTxBuffer(nAntenna);
}

/** @brief This function returns the pointer to the RX buffer used to receive data
    @param nAntenna [in] - an antenna ID
    @return [void*] the pointer to the buffer

    @NOTE : The EVM module should be initializeds */

void * EvmGetRxBuffer (UINT32 nAntenna)
{
    return EvmGetAntennaRxBuffer(nAntenna);
}

/** @brief This function just stops RX operation
            and can be called when device is inited

    @param nAntennaID [in] - the antenna ID

    @return [MXRC] an error code */

MXRC EvmFpgaStopRx(UINT32 nAntennaID)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();

    _ASSERT_PTR(pCtx);

    if (!pCtx->bInited)
        return RC_EVM_FPGA_NOT_INITED;

    pCtx->Regs->AxCEnable = 0; // disable receiving data on antenna-0

    return MX_OK;
}

/** @brief The fnction returns CpuID where this modle was initialized
    @return [UINT32] [0..3] */

UINT32 EvmFpgaGetMasterCpu (void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();

    _ASSERT_PTR(pCtx);

    if (!pCtx->bInited)
        return 0xFFFF;

    return pCtx->MasterCpu;
}

/** @brief This function should stop the device and free used buffers
    @return [MXRC] an error code */
MXRC EvmFpgaShutdown (void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();

    _ASSERT_PTR(pCtx);

    if (!pCtx->bInited || pCtx->MasterCpu != MxGetCpuID ())
        return RC_EVM_FPGA_NO_OWNER;

    EvmFpgaStopRadio ();

    //memset (pCtx, 0, sizeof (*pCtx));

    pCtx->bInited   = 0;
    pCtx->MasterCpu = 0;
    pCtx->StopIrq   = 0;

    return MX_OK;
}

MXRC EvmFpgaEnableCtrl (UINT32 nCtrl)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;

    _ASSERT_PTR(pCtx);

    if (!pCtx->bInited)
        return RC_EVM_FPGA_NOT_INITED;

    irq = ARM_INT_disable();
    pCtx->MaskCtrl |= nCtrl;
    ARM_INT_restore(irq);

    return MX_OK;
}

MXRC EvmFpgaDisableCtrl (UINT32 nCtrl)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;

    _ASSERT_PTR(pCtx);

    if (!pCtx->bInited)
        return RC_EVM_FPGA_NOT_INITED;

    irq = ARM_INT_disable();
    pCtx->MaskCtrl &= ~nCtrl;
    ARM_INT_restore(irq);

    return MX_OK;
}

UINT32 EvmFpgaGetCtrl (void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    _ASSERT_PTR(pCtx);
    return pCtx->MaskCtrl;
}

MXRC EvmFpgaSetCtrl (UINT32 nCtrl)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;

    _ASSERT_PTR(pCtx);

    if (!pCtx->bInited)
        return RC_EVM_FPGA_NOT_INITED;

    irq = ARM_INT_disable();
    pCtx->MaskCtrl = nCtrl;
    ARM_INT_restore(irq);

    return MX_OK;
}

MXRC EvmFpgaResetTxLog(void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;

    _ASSERT_PTR(pCtx);

    if (!pCtx->bInited)
        return RC_EVM_FPGA_NOT_INITED;

    irq = ARM_INT_disable();
    pCtx->TxLogPos = 0;
    ARM_INT_restore(irq);

    return MX_OK;
}

MXRC EvmFpgaResetRxLog(void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;

    _ASSERT_PTR(pCtx);

    if (!pCtx->bInited)
        return RC_EVM_FPGA_NOT_INITED;

    irq = ARM_INT_disable();
    pCtx->RxLogPos = 0;
    ARM_INT_restore(irq);

    return MX_OK;
}

UINT32 EvmFpgaGetMaxRxIrqPeriod (void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;
    UINT32 nVal;

    _ASSERT_PTR(pCtx);

    if (!pCtx->bInited)
        return (UINT32)0xFFFFFFFF;

    irq = ARM_INT_disable();
    nVal = pCtx->MaxRxPeriod;
    ARM_INT_restore(irq);

    //uart_printf ("MAX RX-IRQ: %d\r\n", nVal);

    return nVal;
}

UINT32 EvmFpgaGetMaxTxIrqPeriod (void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;
    UINT32 nVal;

    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable();
    nVal = pCtx->MaxTxPeriod;
    ARM_INT_restore(irq);

    //uart_printf ("MAX TX-IRQ: %d\r\n", nVal);

    return nVal;
}

UINT32 EvmFpgaGetMaxTxIrqSubPeriod (void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;
    UINT32 nVal;

    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable();
    nVal = pCtx->MaxTxSubPeriod;
    ARM_INT_restore(irq);

    //uart_printf ("MAX SUB-TX-IRQ: %d\r\n", nVal);

    return nVal;
}

UINT32 EvmFpgaGetMaxTxMdmaPeriod (void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;
    UINT32 nVal;

    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable();
    nVal = pCtx->MaxTxMdmaPeriod;
    ARM_INT_restore(irq);

    return nVal;
}

UINT32 EvmFpgaGetTxMdmaPeriod (void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;
    UINT32 nVal;

    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable();
    nVal = pCtx->TxMdmaPeriod;
    ARM_INT_restore(irq);

    return nVal;
}

UINT32 EvmFpgaGetRxIrqNum(void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;
    UINT32 nVal;

    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable();
    nVal = pCtx->RxIrqNum;
    ARM_INT_restore(irq);

    return nVal;
}

UINT32 EvmFpgaGetTxIrqNum(void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;
    UINT32 nVal;

    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable();
    nVal = pCtx->TxIrqNum;
    ARM_INT_restore(irq);

    return nVal;
}

UINT32 EvmFpgaGetRxLogSize(void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;
    UINT32 nVal;

    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable();
    nVal = pCtx->RxLogPos;
    ARM_INT_restore(irq);

    return nVal;
}

UINT32 EvmFpgaGetTxLogSize(void)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    IRQTYPE irq;
    UINT32 nVal;

    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable();
    nVal = pCtx->TxLogPos;
    ARM_INT_restore(irq);

    return nVal;
}

void EvmFpgaCfgProc(EvmCfg* pCfg)
{
    EvmFpgaCtx * pCtx = EvmGetCtx();
    UINT32 TxIrqGpioSet = 0;
    UINT32 RxIrqGpioSet = 0;
    IRQTYPE irq = 0;

    if(pCtx->bInited)
        irq = ARM_INT_disable ();

    if(pCfg->TxIrqGpioEna)
    {// to enable
        if (TxIrqGpioSet == 0)
        {
            if(pCtx->bInited)
            {
                pCtx->TxIrqGpioPort = pCfg->TxIrqGpioPort;
                pCtx->TxIrqGpioEna = 1;
                pCtx->TxIrqGpioToggleDelays[0] = pCfg->TxIrqGpioToggleDelays >> 16;
                if(pCtx->TxIrqGpioToggleDelays[0] == 0)
                    pCtx->TxIrqGpioToggleDelays[0] = 1;
                pCtx->TxIrqGpioToggleDelays[1] = pCfg->TxIrqGpioToggleDelays & 0xFFFF;
                if(pCtx->TxIrqGpioToggleDelays[1] == 0)
                    pCtx->TxIrqGpioToggleDelays[1] = 1;
                pCtx->TxIrqGpioToggleDelayIdx = 0;
                pCtx->TxIrqGpioToggleCntr = 0;
            }
            REG32(GPIO_PIN_SELECT_REG) |= (1 << pCfg->TxIrqGpioPort); // GPIO functionality
            REG32(GPIO_OE_REG) |= (1 << pCfg->TxIrqGpioPort); // enable
            REG32(GPIO_OUTPUT_REG) &= ~(1 << pCfg->TxIrqGpioPort); // low at start
            TxIrqGpioSet = 1;
        }
    }
    else if(pCtx->bInited && pCtx->TxIrqGpioEna)
    {// to disable
        REG32(GPIO_OE_REG) &= ~(1 << pCtx->TxIrqGpioPort); // disable
        pCtx->TxIrqGpioEna = 0;
    }

    if(pCfg->RxIrqGpioEna)
    {// to enable
        if (RxIrqGpioSet == 0)
        {
            if(pCtx->bInited)
            {
                pCtx->RxIrqGpioPort = pCfg->RxIrqGpioPort;
                pCtx->RxIrqGpioEna = 1;
                pCtx->RxIrqGpioToggleDelays[0] = pCfg->RxIrqGpioToggleDelays >> 16;
                if(pCtx->RxIrqGpioToggleDelays[0] == 0)
                    pCtx->RxIrqGpioToggleDelays[0] = 1;
                pCtx->RxIrqGpioToggleDelays[1] = pCfg->RxIrqGpioToggleDelays & 0xFFFF;
                if(pCtx->RxIrqGpioToggleDelays[1] == 0)
                    pCtx->RxIrqGpioToggleDelays[1] = 1;
                pCtx->RxIrqGpioToggleDelayIdx = 0;
                pCtx->RxIrqGpioToggleCntr = 0;
            }
            REG32(GPIO_PIN_SELECT_REG) |= (1 << pCfg->RxIrqGpioPort); // GPIO functionality
            REG32(GPIO_OE_REG) |= (1 << pCfg->RxIrqGpioPort); // enable
            REG32(GPIO_OUTPUT_REG) &= ~(1 << pCfg->RxIrqGpioPort); // low at start
            RxIrqGpioSet = 1;
        }
    }
    else if(pCtx->bInited && pCtx->RxIrqGpioEna)
    {// to disable
        REG32(GPIO_OE_REG) &= ~(1 << pCtx->RxIrqGpioPort); // disable
        pCtx->RxIrqGpioEna = 0;
    }

    memcpy(&EvmFpgaApiConfig, pCfg, sizeof(EvmFpgaApiConfig));
    EvmFpgaApiConfigSet = 1;

    if(pCtx && pCtx->bInited)
        ARM_INT_restore (irq);
}

MXRC EvmFpgaCfgApi(ApiHeader *pApi, ApiHeader *pResp)
{
    ApiParam * pParam;
    MXRC rc = MX_OK;
    EvmCfg EvmFpgaCfg;

    memset(&EvmFpgaCfg, 0, sizeof(EvmFpgaCfg));

    switch(pApi->MessageID)
    {
        case PHY_EVMFPGA_GPIO_CTRL:
        case DI_EVMFPGA_GPIO_CTRL:

            pParam = ApiGetNextParam(pApi, NULL);
            while (pParam)
            {
                switch (pParam->ParamID)
                {
                    case PAR_CPRI_GPIO_TXIRQ_ENA:
                        EvmFpgaCfg.TxIrqGpioEna = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_GPIO_TXIRQ_PORT:
                        EvmFpgaCfg.TxIrqGpioPort = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_GPIO_TXIRQ_TOGGLE_DELAYS:
                        EvmFpgaCfg.TxIrqGpioToggleDelays = pParam->ParamData[0];
                        break;

                    case PAR_CPRI_GPIO_RXIRQ_ENA:
                        EvmFpgaCfg.RxIrqGpioEna = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_GPIO_RXIRQ_PORT:
                        EvmFpgaCfg.RxIrqGpioPort = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_GPIO_RXIRQ_TOGGLE_DELAYS:
                        EvmFpgaCfg.RxIrqGpioToggleDelays = pParam->ParamData[0];
                        break;

                    default:
                        uart_printf("EvmFpgaCfg: unknown parameter ID\r\n", pParam->ParamID);
                        break;
                }
                pParam = ApiGetNextParam(pApi, pParam);
            }
            break;

        default:
            uart_printf("EvmFpgaCfg: unknown message ID\r\n", pApi->MessageID);
            break;
    }

    EvmFpgaCfgProc(&EvmFpgaCfg);

    return rc;
}

void EvmFpgaMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    if (nCpuID == 0 || nCpuID == EVM_FPGA_INT_HANDLER_CPU_ID)
    {
        MmuMgrCfgMemAP(GPIO_APB_BASEADDR,       8*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(RAD_APB_CONFIG_BASEADDR, 8*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(FPGA_BASEADDR,           8*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(RADIO_BASEADDR,          8*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(GPIO_APB_BASEADDR,       8*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
    }
}


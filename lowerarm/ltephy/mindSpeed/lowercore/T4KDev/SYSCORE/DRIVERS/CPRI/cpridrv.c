//-------------------------------------------------------------------------------------------
/** @file cpridrv.c
 *
 * @brief CPRI driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.86 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "4gmx.h"
#include "heaps.h"
#include "config.h"
#include "cpridrv.h"
#include "4gmx_serv.h"
#include "appprintf.h"
#include "i2cdrv.h"
#include "icdrv.h"
#include "timerdrv.h"
#include "globalstoragearrays.h"
#include "srdsdrv.h"
#include "mlog.h"

#pragma no_inline
#define GPIO_SRDS_LANE_CTRL_REG         (GPIO_APB_BASEADDR + 0x60)

#define AUTO_CPRI_LOOPBACK_SFN_LATENCY 3

PCPRIDRVCTX pCpriDrvCtx = NULL;

const char *hdlc_rate_string[HDLC_RATE_4800] =
{ "Any", "240", "480", "960", "1920", "2400" };

#define GETCURRTICKS() (*((volatile UINT32*)0xFE050004))

//#define ICPU_FORWARDING

CPDMAREGS CpriDmaEmulRegs [CPRI_DEV_NUM];
CPRIREGS  CpriEmulRegs [CPRI_DEV_NUM];

CPRICONFIG CpriApiConfig = {0};
UINT32 CpriApiConfigSet = 0;

CPRICONFIG CpriConfigByHost = {0}; // Configuration structure for CPRI configuring from host
BOOL bCpriConfigByHost = 0;        // The flag which informs that a host has configured CPRI
BOOL bSftCtrlEnabled = 0;
UINT32 SfpSrdsID = 1;
UINT32 SfpMask = 3;

U8 CpriRxLatency = 0;

#ifdef ICPU_FORWARDING
PUINT8 pTxEthFrame = NULL;
UINT8 TxEthFrameInd = 0;
PUINT8 pRxEthFrame = NULL;

#define MAX_ICPU_COMMAND_SIZE (CPRI_MAX_ETH_FRAME_SIZE+sizeof(ApiHeader)+4)

MXRC ICPURxChain(LPVOID data, UINT32 len);
APIHEC CPRIApiHandler (LPVOID pData, ApiHeader * pApi, ApiHeader * pResp);

#endif

extern MXRC LTEPhyStopHandler(void);

void Delay(UINT32 ms)
{
    UINT32 time = GETCURRTICKS();
    while ((GETCURRTICKS() - time) < ms*150000);
    //while (++time < ms*150000);
}

void CpdmaTxLogCb(LPVOID p)
{
    PCPRICTX pCpriCtx = (PCPRICTX)p;
    pCpriCtx->TxLogCount++;
    MLogTask(1163, RESOURCE_RAD_AXI, pCpriCtx->RxLogBufTime[0], MxGetTicks());
}

void CpdmaTxLog(PCPRICTX pCpriCtx)
{
    UINT32 antIdx = 0;
    UINT32 DmaIntSize = pCpriCtx->DmaIntSize;

    _ASSERT(pCpriCtx->NumAxC > 1);

    if ((pCpriCtx->CpriConfType == CPRI_MIMO_2AC) && (pCpriCtx->LogCtrl & CPRI_LOG_CTRL_TX0_ENABLED) && (pCpriCtx->LogCtrl & CPRI_LOG_CTRL_TX1_ENABLED))
    {
        UINT32 LogBufOffset;
        PUINT8 pDmaBuf1;
        PUINT8 pDmaBuf2;
        MDMA_FDESC *pMdma;

        if ((pCpriCtx->TxLogBufPos[0] + DmaIntSize) > pCpriCtx->TxLogBufSize || (pCpriCtx->TxLogBufPos[1] + DmaIntSize) > pCpriCtx->TxLogBufSize)
            return;

        LogBufOffset = (pCpriCtx->pCpdmaRegs->TX_AC_PTR[0] < (DmaIntSize >> 3)) ? 0 : DmaIntSize;

        pMdma = MdmaAllocDesc(MDMA_HW_RAD, 1);
        _ASSERT_PTR(pMdma);

        pDmaBuf1 = &pCpriCtx->pTxDmaBuf[LogBufOffset];
        pDmaBuf2 = &pCpriCtx->pTxDmaBuf[2 * DmaIntSize + LogBufOffset];

        _ASSERT_PTR(pMdma);

        if(!pMdma)
            return;

        MdmaSetCopy(pMdma);
        MdmaSetData2(pMdma, pDmaBuf1, DmaIntSize, pDmaBuf2, DmaIntSize, pCpriCtx->pTxLogBuf[0] + pCpriCtx->TxLogBufPos[0], DmaIntSize, pCpriCtx->pTxLogBuf[1] + pCpriCtx->TxLogBufPos[1], DmaIntSize, 0);
        pCpriCtx->TxLogBufTime[0] = MxGetTicks();
        MdmaRunEx(MDMA_HW_RAD, pMdma, CpdmaTxLogCb, pCpriCtx);
        pCpriCtx->TxLogBufPos[0] += DmaIntSize;
        pCpriCtx->TxLogBufPos[1] += DmaIntSize;

        if ((pCpriCtx->TxLogBufPos[0] >= pCpriCtx->TxLogBufSize) && (pCpriCtx->LogCtrl & CPRI_LOG_CTRL_TX_RR))
        {
            pCpriCtx->TxLogBufPos[0] = 0;
            pCpriCtx->TxLogBufReset[0]++;
        }


        if ((pCpriCtx->TxLogBufPos[1] >= pCpriCtx->TxLogBufSize) && (pCpriCtx->LogCtrl & CPRI_LOG_CTRL_TX_RR))
        {
            pCpriCtx->TxLogBufPos[1] = 0;
            pCpriCtx->TxLogBufReset[1]++;
        }
    }
    else
    {
        for (antIdx = 0; antIdx < pCpriCtx->NumAxC; antIdx++)
        {
            if (pCpriCtx->LogCtrl & (1 << antIdx ))
            {
                if ((pCpriCtx->TxLogBufPos[antIdx] + DmaIntSize) <= pCpriCtx->TxLogBufSize)
                {
                    UINT32 LogBufOffset = (pCpriCtx->pCpdmaRegs->TX_AC_PTR[antIdx] < (DmaIntSize >> 3)) ? 0 : DmaIntSize;
                    PUINT8 pDmaBuf;

                    MDMA_FDESC *pMdma = MdmaAllocDesc(MDMA_HW_RAD, 1);
                    _ASSERT_PTR(pMdma);

                    //RRH specific?
                    //    if(pCpriCtx->NumAxC != 1)
                    //        LogBufOffset += (pCpriCtx->DmaIntSize << 1);

                    pDmaBuf = &pCpriCtx->pTxDmaBuf[antIdx * 2 * DmaIntSize + LogBufOffset];

                    _ASSERT_PTR(pMdma);

                    if(!pMdma)
                        return;

                    MdmaSetCopy(pMdma);
                    MdmaSetData(pMdma, pDmaBuf, DmaIntSize, pCpriCtx->pTxLogBuf[antIdx] + pCpriCtx->TxLogBufPos[antIdx], DmaIntSize);
                    pCpriCtx->TxLogBufTime[0] = MxGetTicks();
                    MdmaRunEx(MDMA_HW_RAD, pMdma, CpdmaTxLogCb, pCpriCtx);
                    pCpriCtx->TxLogBufPos[antIdx] += DmaIntSize;

                    if ((pCpriCtx->TxLogBufPos[antIdx] >= pCpriCtx->TxLogBufSize) && (pCpriCtx->LogCtrl & CPRI_LOG_CTRL_TX_RR))
                    {
                        pCpriCtx->TxLogBufPos[antIdx] = 0;
                        pCpriCtx->TxLogBufReset[antIdx]++;
                    }
                }
            }
        }
    }
}


void CpdmaRxLogCb(LPVOID p)
{
    PCPRICTX pCpriCtx = (PCPRICTX)p;
    pCpriCtx->RxLogCount++;
    MLogTask(1164, RESOURCE_RAD_AXI, pCpriCtx->RxLogBufTime[0], MxGetTicks());
}

void CpdmaRxLog(PCPRICTX pCpriCtx)
{
    UINT32 antIdx = 0;
    UINT32 DmaIntSize = pCpriCtx->DmaIntSize;

    _ASSERT(pCpriCtx->NumAxC > 2);

    if ((pCpriCtx->CpriConfType == CPRI_MIMO_2AC) && (pCpriCtx->LogCtrl & CPRI_LOG_CTRL_RX0_ENABLED) && (pCpriCtx->LogCtrl & CPRI_LOG_CTRL_RX1_ENABLED))
    {
        UINT32 LogBufOffset;
        PUINT8 pDmaBuf1;
        PUINT8 pDmaBuf2;
        MDMA_FDESC *pMdma;

        if ((pCpriCtx->RxLogBufPos[0] + DmaIntSize) > pCpriCtx->RxLogBufSize || (pCpriCtx->RxLogBufPos[1] + DmaIntSize) > pCpriCtx->RxLogBufSize)
            return;

        LogBufOffset = (pCpriCtx->pCpdmaRegs->RX_AC_PTR[antIdx] < (DmaIntSize >> 3)) ? DmaIntSize : 0;

        pMdma = MdmaAllocDesc(MDMA_HW_RAD, 1);
        _ASSERT_PTR(pMdma);

        pDmaBuf1 = &pCpriCtx->pRxDmaBuf[LogBufOffset];
        pDmaBuf2 = &pCpriCtx->pRxDmaBuf[2 * DmaIntSize + LogBufOffset];

        _ASSERT_PTR(pMdma);

        if(!pMdma)
        {
            uart_printf ("CPRI:RX-LOG error, no MDMA descriptors\r\n");
            return;
        }

        MdmaSetCopy(pMdma);
        MdmaSetData2(pMdma, pDmaBuf1, DmaIntSize, pDmaBuf2, DmaIntSize, pCpriCtx->pRxLogBuf[0] + pCpriCtx->RxLogBufPos[0], DmaIntSize, pCpriCtx->pRxLogBuf[1] + pCpriCtx->RxLogBufPos[1], DmaIntSize, 0);
        pCpriCtx->RxLogBufTime[0] = MxGetTicks();
        MdmaRunEx(MDMA_HW_RAD, pMdma, CpdmaRxLogCb, pCpriCtx);
        pCpriCtx->RxLogBufPos[0] += DmaIntSize;
        pCpriCtx->RxLogBufPos[1] += DmaIntSize;

        if ((pCpriCtx->RxLogBufPos[0] >= pCpriCtx->RxLogBufSize) && (pCpriCtx->LogCtrl & CPRI_LOG_CTRL_RX_RR))
        {
            pCpriCtx->RxLogBufPos[0] = 0;
            pCpriCtx->RxLogBufReset[0]++;
        }


        if ((pCpriCtx->RxLogBufPos[1] >= pCpriCtx->RxLogBufSize) && (pCpriCtx->LogCtrl & CPRI_LOG_CTRL_RX_RR))
        {
            pCpriCtx->RxLogBufPos[1] = 0;
            pCpriCtx->RxLogBufReset[1]++;
        }
    }
    else
    {
        for (antIdx = 0; antIdx < pCpriCtx->NumAxC; antIdx++)
        {
            if (pCpriCtx->LogCtrl & (1 << antIdx + 2))
            {
                if ((pCpriCtx->RxLogBufPos[antIdx] + DmaIntSize) <= pCpriCtx->RxLogBufSize)
                {
                    UINT32 BufOffset = (pCpriCtx->pCpdmaRegs->RX_AC_PTR[antIdx] < (DmaIntSize >> 3)) ? DmaIntSize : 0;
                    PUINT8 pDmaBuf = &pCpriCtx->pRxDmaBuf[antIdx * 2 * DmaIntSize + BufOffset];

                    MDMA_FDESC * pMdma = MdmaAllocDesc(MDMA_HW_RAD, 1);
                    _ASSERT_PTR(pMdma);

                    if(!pMdma)
                        return;

                    MdmaSetCopy(pMdma);
                    MdmaSetData(pMdma, pDmaBuf, DmaIntSize, pCpriCtx->pRxLogBuf[antIdx] + pCpriCtx->RxLogBufPos[antIdx], DmaIntSize);
                    pCpriCtx->RxLogBufTime[0] = MxGetTicks();
                    MdmaRunEx(MDMA_HW_RAD, pMdma, CpdmaRxLogCb, pCpriCtx);

                    pCpriCtx->RxLogBufPos[antIdx] += DmaIntSize;

                    if ((pCpriCtx->RxLogBufPos[antIdx] >= pCpriCtx->RxLogBufSize) && (pCpriCtx->LogCtrl & CPRI_LOG_CTRL_RX_RR))
                    {
                        // to reset the log
                        pCpriCtx->RxLogBufPos[antIdx] = 0;
                        pCpriCtx->RxLogBufReset[antIdx]++;
                        // uart_printf("WARNING: Rx log has been overload\n");
                    }
                }
            }
        }
    }
}


CPDMAREGS * CpriGetCpDma (CPRICONFIG *pCfg, UINT32 nDevID)
{
    if (pCfg->CpriConfType != CPRI_EMULATOR)
        return (PCPDMAREGS)CPDMA_BASEADDR(nDevID);

    return &CpriDmaEmulRegs[nDevID];
}

CPRIREGS * CpriGetRegs (CPRICONFIG *pCfg, UINT32 nDevID)
{
    if (pCfg->CpriConfType != CPRI_EMULATOR)
        return (PCPRIREGS)RADIP_BASEADDR(nDevID);

    return &CpriEmulRegs[nDevID];
}

void CpriMdmaLogCb (void * pData)
{
    // non-blocking cb
}

void CpriIrqHandler(volatile UINT32 * pClear, UINT32 nIrqID, LPVOID pParam)
{
    PCPRICTX pCpriCtx = (PCPRICTX)pParam;
    PCPRIREGS pCpriRegs = pCpriCtx->pCpriRegs;
    UINT32 EthRxStat = pCpriRegs->ETH_RX_STATUS;
    UINT32 EthTxStat = pCpriRegs->ETH_TX_STATUS;

    if ((pCpriRegs->ETH_CONFIG_1 & CPRI_ETHC1_TXRBIE) && (EthTxStat & CPRI_ETHTXSTAT_RDY_BLK))
    {
        UINT32 BufPos = pCpriCtx->TxEthBDesc[pCpriCtx->TxEthBufGet].BCtrl >> 16;
        UINT32 BufLen = pCpriCtx->TxEthBDesc[pCpriCtx->TxEthBufGet].BCtrl & 0xFFFF;
        PUINT32 pU32 = (PUINT32)(pCpriCtx->TxEthBDesc[pCpriCtx->TxEthBufGet].BPtr + BufPos);

        if (BufLen > 32)
        {
            pCpriRegs->ETH_TX_DATA = *pU32++;
            pCpriRegs->ETH_TX_DATA = *pU32++;
            pCpriRegs->ETH_TX_DATA = *pU32++;
            pCpriRegs->ETH_TX_DATA = *pU32++;
            pCpriRegs->ETH_TX_DATA = *pU32++;
            pCpriRegs->ETH_TX_DATA = *pU32++;
            pCpriRegs->ETH_TX_DATA = *pU32++;
            pCpriRegs->ETH_TX_DATA = *pU32++;
            pCpriCtx->TxEthBDesc[pCpriCtx->TxEthBufGet].BCtrl = ((BufPos + 32) << 16) | (BufLen - 32);
        }
        else
        {
            do
            {
                if (BufLen <= 4)
                {
                    pCpriRegs->ETH_TX_CONTROL = ((BufLen-1) << 2) | 1;
                    BufLen = 0;
                }
                else
                    BufLen -= 4;

                pCpriRegs->ETH_TX_DATA = *pU32++;
            }
            while (BufLen);
            pCpriCtx->TxEthBDesc[pCpriCtx->TxEthBufGet].BCtrl = 0;
            pCpriCtx->TxEthBufGet = (pCpriCtx->TxEthBufGet + 1) & (NUM_CPRI_ETH_TX_BUF-1);
            if (pCpriCtx->TxEthBufGet == pCpriCtx->TxEthBufPut)
                pCpriRegs->ETH_CONFIG_1 &= ~CPRI_ETHC1_TXRBIE;
        }
    }
    if (EthTxStat & CPRI_ETHTXSTAT_ABORT)
    {
        pCpriRegs->ETH_TX_CONTROL = 1<<1; // Discard current TX packet
    }
    if (EthRxStat & CPRI_ETHRXSTAT_RDY_BLK)
    {
        PUINT32 pU32 = (PUINT32)(pCpriCtx->RxEthBDesc[pCpriCtx->RxEthBufPut].BPtr + pCpriCtx->RxEthBDesc[pCpriCtx->RxEthBufPut].BCtrl);

        *pU32++ = pCpriRegs->ETH_RX_DATA;
        *pU32++ = pCpriRegs->ETH_RX_DATA;
        *pU32++ = pCpriRegs->ETH_RX_DATA;
        *pU32++ = pCpriRegs->ETH_RX_DATA;
        *pU32++ = pCpriRegs->ETH_RX_DATA;
        *pU32++ = pCpriRegs->ETH_RX_DATA;
        *pU32++ = pCpriRegs->ETH_RX_DATA;
        *pU32++ = pCpriRegs->ETH_RX_DATA;
        pCpriCtx->RxEthBDesc[pCpriCtx->RxEthBufPut].BCtrl += 32;
    }
    if (EthRxStat & CPRI_ETHRXSTAT_RDY_END)
    {
        PUINT32 pU32 = (PUINT32)(pCpriCtx->RxEthBDesc[pCpriCtx->RxEthBufPut].BPtr + pCpriCtx->RxEthBDesc[pCpriCtx->RxEthBufPut].BCtrl);
        UINT32 RxEthBufPut;

        do
        {
            if (pCpriRegs->ETH_RX_STATUS & CPRI_ETHRXSTAT_EOF)
                pCpriCtx->RxEthBDesc[pCpriCtx->RxEthBufPut].BCtrl += ((pCpriRegs->ETH_RX_STATUS >>3) & 3) + 1;
            else
                pCpriCtx->RxEthBDesc[pCpriCtx->RxEthBufPut].BCtrl += 4;
            *pU32++ = pCpriRegs->ETH_RX_DATA;
        }
        while (pCpriRegs->ETH_RX_STATUS & CPRI_ETHRXSTAT_RDY);

        RxEthBufPut = (pCpriCtx->RxEthBufPut + 1) & (NUM_CPRI_ETH_RX_BUF-1);
        if (RxEthBufPut == pCpriCtx->RxEthBufGet)
        { // RX buffer is full. Discard received frame.
            pCpriCtx->RxEthBDesc[pCpriCtx->RxEthBufPut].BCtrl = 0;
        }
        else
        {
#ifdef ICPU_FORWARDING
//if frame has been sucessfully forwarded to ICPU at once - FIFO always empty //minus 4 - CRC size
            if ( ICPURxChain(   pCpriCtx->RxEthBDesc[pCpriCtx->RxEthBufPut].BPtr,
                                pCpriCtx->RxEthBDesc[pCpriCtx->RxEthBufPut].BCtrl-4 ) == CPRI_RC_OK )
                pCpriCtx->RxEthBDesc[pCpriCtx->RxEthBufPut].BCtrl = 0;
                pCpriCtx->RxEthBufGet = RxEthBufPut;
#endif
            pCpriCtx->RxEthBufPut = RxEthBufPut;
        }
    }
    *pClear = nIrqID;
}


void CPRIDrvEmulProc (void * pData)
{
    UINT32 i, nDev = 0;
    PCPRIDRVCTX pCtx = CPRIDrvGetCtx ();
#ifdef CPRI_IRQ_TICKS_MEASUREMENT
    UINT32 nTick;
#endif

    while (nDev < CPRI_DEV_NUM)
    {
        CPRICTX * pDevCtx;

        if (pCtx->DevEnaMask & (1 << nDev))
        {
            pDevCtx = &pCtx->CpriCtx [nDev];

#ifdef CPRI_DEBUG_TRACE
            uart_printf ("CPRI(EMUL) ");
#endif

#ifdef CPRI_IRQ_TICKS_MEASUREMENT
            nTick = GetTIMETICK();
            pDevCtx->MaxRxIrqPeriod = MAX(pDevCtx->MaxRxIrqPeriod, nTick - pDevCtx->RxIrqTick);
            pDevCtx->RxIrqTick = nTick;
            pDevCtx->MaxTxIrqPeriod = MAX(pDevCtx->MaxTxIrqPeriod, nTick - pDevCtx->TxIrqTick);
            pDevCtx->TxIrqTick = nTick;
#endif
            if (pDevCtx->pTxLogBuf != NULL)
            {
                pDevCtx->TxIrqNum ++;

                for(i = 0; i < pDevCtx->nTXAntenas; i++)
                {
                    pDevCtx->pEmulTxBuf[i] = pDevCtx->pTxLogBuf[i] + pDevCtx->TxLogBufPos[i];

#ifdef CPRI_DEBUG_TRACE
                uart_printf (" txcb:%08x(0x%x) ", pDevCtx->pTxDMACallBack, pDevCtx->pEmulTxBuf[i]);
#endif
                    // just to call TX-callback once
                    // to make logic the same to CPDMA

                    if(pDevCtx->pTxDMACallBack != NULL && (i == 0))
                        pDevCtx->pTxDMACallBack (pDevCtx->TxPtr, i, pDevCtx->pEmulTxBuf[i], pDevCtx->DmaIntSize);

                    if (pDevCtx->TxLogFreezeNum == 0)
                        pDevCtx->TxLogBufPos[i] += pDevCtx->DmaIntSize * pDevCtx->nTXAntenas;

                    if (pDevCtx->TxLogBufPos[i] >= pDevCtx->TxLogBufSize)
                        pDevCtx->TxLogBufPos[i] = 0;
                }

                if (pDevCtx->TxLogFreezeNum != 0)
                    pDevCtx->TxLogFreezeNum --;

            }

            if (pDevCtx->pRxLogBuf != NULL)
            {
                pDevCtx->RxIrqNum ++;

                for(i = 0; i < pDevCtx->nRXAntenas; i++)
                {
                    pDevCtx->pEmulRxBuf[i] = pDevCtx->pRxLogBuf[i] + pDevCtx->RxLogBufPos[i];
#ifdef CPRI_DEBUG_TRACE
                    uart_printf (" rxcb:%08x(0x%x) ", pDevCtx->pRxDMACallBack, pDevCtx->pEmulRxBuf[i]);
#endif
                    // just to call RX-callback once
                    // to make logic the same to CPDMA

                    if(pDevCtx->pRxDMACallBack != NULL && (i == 0))
                        pDevCtx->pRxDMACallBack (pDevCtx->RxPtr, i, pDevCtx->pEmulRxBuf[i], pDevCtx->DmaIntSize);

                    if (pDevCtx->RxLogFreezeNum == 0)
                        pDevCtx->RxLogBufPos[i] += pDevCtx->DmaIntSize * pDevCtx->nRXAntenas;

                    if (pDevCtx->RxLogBufPos[i] >= pDevCtx->RxLogBufSize)
                        pDevCtx->RxLogBufPos[i] = pDevCtx->RxLogBufRoudupPos;
                }
                if (pDevCtx->RxLogFreezeNum != 0)
                    pDevCtx->RxLogFreezeNum --;
            }

#ifdef CPRI_DEBUG_TRACE
            uart_printf ("ok\r\n");
#endif
        }

        nDev++;
    }
}

void CpdmaRxIrqHandler(volatile UINT32 * pClear, UINT32 nIrqID, LPVOID pParam)
{
    PCPRICTX pCpriCtx = (PCPRICTX)pParam;
    PCPDMAREGS pCpdmaRegs = pCpriCtx->pCpdmaRegs;
    UINT32 status = pCpdmaRegs->RX_INT_STAT;
    UINT32 DmaIntSize = pCpriCtx->DmaIntSize;
    UINT32 BufOffset = pCpriCtx->RxBufOffset = (pCpdmaRegs->RX_AC_PTR[(!pCpriCtx->UseRxAntenna)? 0 : (pCpriCtx->UseRxAntenna - 1)] < (DmaIntSize >> 3)) ? DmaIntSize : 0;

#ifdef CPRI_IRQ_TICKS_MEASUREMENT
    UINT32 nTick;
    nTick = GetTIMETICK();
    pCpriCtx->MaxRxIrqPeriod = MAX(pCpriCtx->MaxRxIrqPeriod, nTick - pCpriCtx->RxIrqTick);
    pCpriCtx->RxIrqTick = nTick;
#endif

    pCpriCtx->RxIrqNum ++;

#ifdef CPRI_DEBUG_TRACE
    uart_printf ("CpdmaRx\r\n");
#endif

#ifdef CPRI_SYNC_LOCAL_TIMER
    TimerDrvRestart();
#endif

    if (pCpriCtx->RxIrqGpioEna)
    {
        if(++pCpriCtx->RxIrqGpioToggleCntr >= pCpriCtx->RxIrqGpioToggleDelays[pCpriCtx->RxIrqGpioToggleDelayIdx])
        {
            REG32(GPIO_OUTPUT_REG) ^= (1 <<  pCpriCtx->RxIrqGpioPort);
            pCpriCtx->RxIrqGpioToggleCntr = 0;
            pCpriCtx->RxIrqGpioToggleDelayIdx ^= 1;
        }
    }

    /* start capturing on the same IRQ as TX does */
    if (CPRI_TX_LOG_ENABLED(pCpriCtx)
        && pCpriCtx->RxIrqNum >= pCpriCtx->RxLogIrqStart
        && pCpriCtx->RxIrqNum < pCpriCtx->RxStopCount)
    {
        CPRI_RX_LOG_ENABLE(pCpriCtx);
    }
    //else
        //CPRI_RX_LOG_DISABLE(pCpriCtx);

    if (pCpriCtx->RxLogIrqStart > 0 && /*CPRI_RX_LOG_ENABLED(pCpriCtx) &&*/
        pCpriCtx->RxIrqNum >= pCpriCtx->RxStopCount)
    {
        MxDisableInt(HAL_IRQ_CPRI_0 + 0);
        MxDisableInt(HAL_IRQ_CPDMA_TX_0 + 0);
        MxDisableInt(HAL_IRQ_CPDMA_RX_0 + 0);
        CPRIDrvDisableDma(0);
        SvsrLoggerFlush();
        if (MLogGetMask() != 0)
            MLogPrint();
        uart_printf("CPDMA: disable\r\n");
    }

    // to run the logging
    if (CPRI_RX_LOG_ENABLED(pCpriCtx))
        CpdmaRxLog(pCpriCtx);

    if ( pCpriCtx->pRxDMACallBack )
    {
        pCpriCtx->pRxDMACallBack(pCpriCtx->RxPtr, pCpriCtx->DevID, &pCpriCtx->pRxDmaBuf[BufOffset], DmaIntSize);
    //CpdmaRxLog(pCpriCtx);
    }

    pCpdmaRegs->RX_INT_STAT = status;

    *pClear = nIrqID;
}

void CpdmaTxIrqHandler(volatile UINT32 * pClear, UINT32 nIrqID, LPVOID pParam)
{
    PCPRICTX pCpriCtx = (PCPRICTX)pParam;
    PCPDMAREGS pCpdmaRegs = pCpriCtx->pCpdmaRegs;
    UINT32 status = pCpdmaRegs->TX_INT_STAT;
    UINT32 DmaIntSize = pCpriCtx->DmaIntSize;

    // BufOffset: it's true for the SISO/RADIOCOMP and MIMO
    UINT32 BufOffset = (pCpdmaRegs->TX_AC_PTR[(!pCpriCtx->UseTxAntenna) ? (pCpriCtx->NumAxC == 1 ? 0 : 1) : (pCpriCtx->UseTxAntenna - 1) ] < (DmaIntSize >> 3)) ? DmaIntSize : 0;
#ifdef CPRI_IRQ_TICKS_MEASUREMENT
    UINT32 nTick;
    nTick = GetTIMETICK();
    pCpriCtx->MaxTxIrqPeriod = MAX(pCpriCtx->MaxTxIrqPeriod, nTick - pCpriCtx->TxIrqTick);
    pCpriCtx->TxIrqTick = nTick;
#endif

#ifdef CPRI_DEBUG_TRACE
    uart_printf ("CpdmaTx\r\n");
#endif

    pCpriCtx->TxBufOffset = BufOffset;

    pCpriCtx->TxIrqNum++;

    if (pCpriCtx->TxIrqGpioEna)
    {
        if(++pCpriCtx->TxIrqGpioToggleCntr >= pCpriCtx->TxIrqGpioToggleDelays[pCpriCtx->TxIrqGpioToggleDelayIdx])
        {
            REG32(GPIO_OUTPUT_REG) ^= (1 <<  pCpriCtx->TxIrqGpioPort);
            pCpriCtx->TxIrqGpioToggleCntr = 0;
            pCpriCtx->TxIrqGpioToggleDelayIdx ^= 1;
        }
    }

#if 0
    if (CPRI_TX_LOG_ENABLED(pCpriCtx) &&
        pCpriCtx->TxIrqNum > pCpriCtx->TxLogIrqStart && pCpriCtx->TxIrqNum < pCpriCtx->TxStopCount)
    {
        CPRI_TX_LOG_ENABLE(pCpriCtx);
    }
    else if (CPRI_TX_LOG_ENABLED(pCpriCtx))
    {
        CPRI_TX_LOG_DISABLE(pCpriCtx);
    }
#endif

    if (CPRI_TX_LOG_ENABLED(pCpriCtx))
        CpdmaTxLog(pCpriCtx);

    if ( pCpriCtx->pTxDMACallBack )
    {
        PUINT8 pTxBuf = NULL;

        if (pCpriCtx->CpriConfType == CPRI_IXIA || pCpriCtx->CpriConfType == CPRI_MIMO_2AC)
        {
            pTxBuf = &pCpriCtx->pTxDmaBuf[BufOffset];
        }
        else if (pCpriCtx->CpriConfType == CPRI_RADIOCOMP)
        {
            pTxBuf = &pCpriCtx->pTxDmaBuf[(DmaIntSize << 1) + BufOffset]; // using AC1 for TX
        }

        _ASSERT_PTR(pTxBuf);

        pCpriCtx->pTxDMACallBack(pCpriCtx->TxPtr, pCpriCtx->DevID, pTxBuf, DmaIntSize);
    }

    pCpdmaRegs->TX_INT_STAT = status;

    *pClear = nIrqID;
}

void SerDesPcsWriteReg(UINT32 lane_mask, UINT32 addr, UINT32 data)
{
    REG32(RAD_CFG_SRDS_MMD) = 3;
    REG32(RAD_CFG_SRDS_LANE) = lane_mask;
    REG32(addr) = data;
}

UINT32 SerDesPcsReadReg(UINT32 lane_mask, UINT32 addr)
{
    REG32(RAD_CFG_SRDS_MMD) = 3;
    REG32(RAD_CFG_SRDS_LANE) = lane_mask;
    return REG32(addr);
}

void CpriSrds1InitX1(UINT32 CpriRate, UINT32 loopback, UINT32 prbs)
{
    //UINT32 test_id;
    UINT32 prbs_err;
    UINT32 prbs_timer;
    UINT32 saved_pcs_mode;
    UINT32 saved_pcs_prbs_conf;
    UINT32 saved_pcs_reset;
    UINT32 val;

    REG32(RAD_SRDS1_CNTRL) = 0x31;

    REG32(GPIO_SRDS_LANE_CTRL_REG) = 0x1010;

    switch (CpriRate)
    {
        case CPRIRATE_1228:
            // 153.6 x 32 = 4915.2 - PLL frequency
            // 4915.2 / 4 = 1228.8 - line rate
            // 4915.2 / 80 = 61.44 - controller rate
            REG32(RAD_SRDS1_TXIF_CLK_DIV_CNTRL) = 2; // div 2
            REG32(RAD_SRDS1_INIT_CFG_R0) = 0x000009DF; // mult 32, div 20
            REG32(RAD_SRDS1_INIT_CFG_R1) = 0xB00FF; // 1/8 rate
            //test_id = TESTID_CPRI_SRDS_1_R2;
            break;
        case CPRIRATE_2457:
            // 153.6 x 32 = 4915.2 - PLL frequency
            // 4915.2 / 2 = 2457.6 - line rate
            // 4915.2 / 40 = 122.8 - controller rate
//            REG32(RAD_SRDS1_TXIF_CLK_DIV_CNTRL) = 2; // div 2
//            REG32(RAD_SRDS1_INIT_CFG_R0) = 0x000004DF;
            REG32(RAD_SRDS1_INIT_CFG_R0) = 0x000009DF;
            REG32(RAD_SRDS1_INIT_CFG_R1) = 0xB00AA; // 1/4 rate
            //test_id = TESTID_CPRI_SRDS_1_R4;
            break;
        case CPRIRATE_3072:
            // 153.6 x 20 = 3072 - line rate
            // 3072 / 20 = 153.6 - controller rate
            REG32(RAD_SRDS1_INIT_CFG_R0) = 0x000004D3;
            REG32(RAD_SRDS1_INIT_CFG_R1) = 0xB0055; // 1/2 rate
            //test_id = TESTID_CPRI_SRDS_1_R5;
            break;
        case CPRIRATE_4915:
            // 153.6 x 32 = 4915.2 - PLL frequency
            // 4915.2 / 2 = 2457.6 - line rate
            // 4915.2 / 20 = 245.76 - controller rate
            REG32(RAD_SRDS1_INIT_CFG_R0) = 0x000004DF;
            REG32(RAD_SRDS1_INIT_CFG_R1) = 0xB0055; // 1/2 rate
            //test_id = TESTID_CPRI_SRDS_1_R8;
            break;
        case CPRIRATE_6144:
            // 153.6 x 20 = 3072 - line rate
            // 3072 / 10 = 307.2 - controller rate
            REG32(RAD_SRDS1_INIT_CFG_R0) = 0x00000253;
            REG32(RAD_SRDS1_INIT_CFG_R1) = 0xB0000; // full rate
            //test_id = TESTID_CPRI_SRDS_1_R10;
            break;
    }

    // Release serdes1 reset RAD_SRDS1_CNTRL
    REG32(RAD_SRDS1_CNTRL) = 0x0;
    Delay(10);

    REG32(RAD_CFG_SRDS_MMD) = 0x1;
    REG32(RAD_CFG_SRDS_LANE) = 3;

    switch (CpriRate)
    {
        case CPRIRATE_1228:
            // PLL clock ready assertion - section 3.2 of bringup doc
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC000) = 0x78DF;
            Delay(3);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC004) = 0xC440;
            Delay(3);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC008) = 0x0362;
            Delay(3);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC004) = 0xC640;
            Delay(3);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8034) = 0x0066; // RX CTLE Control
            Delay(3);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8038) = 0x0C83; // TX Config 1
            Delay(3);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x803C) = 0x0021; // TX Config 2
            Delay(3);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8040) = 0x00ac; // TX Pre-emphasis
            Delay(3);
            val = REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8048); // TX Clock Select 1
            val = val & 0xFFC0;
//            val |= 0x20 | 0x10;
            val |= 0x3;
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8048) = val; // TX Clock Select 1
            Delay(3);
            // CTLE peak-pole
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8030) = 0x00ff;
            Delay(3);

            // Phase Interpolator
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC078) = 0xC2F0;
            Delay(3);
            //RX error level - by default set to 0x100; setting error level 0x0
//            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8028) = 0x080;

            // Serdes DLL settings - Need this for 3.125GHz
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC008) = 0x262;
            Delay(3);
            break;
        case CPRIRATE_2457:
        case CPRIRATE_4915:
            // PLL clock ready assertion - section 3.2 of bringup doc
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC000) = 0x78DF;
            Delay(6);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC004) = 0xC440;
            Delay(6);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC008) = 0x0362;
            Delay(6);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC004) = 0xC640;
            Delay(6);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8034) = 0x0066;
            Delay(6);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8038) = 0x0C83;
            Delay(6);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x803C) = 0x0021;
            Delay(6);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8040) = 0x00ac;
            // CTLE peak-pole
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8030) = 0x00ff;
            Delay(6);

            val = REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8048); // TX Clock Select 1
            val = val & 0xFFC0;
            val |= 0x21;
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8048) = val; // TX Clock Select 1

            // Phase Interpolator
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC078) = 0xC2F0;
            Delay(6);
            //RX error level - by default set to 0x100; setting error level 0x0
//          REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8028) = 0x080;

            // Serdes DLL settings - Need this for 3.125GHz
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC008) = 0x262;
            Delay(6);
            break;
        case CPRIRATE_3072:

            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8034) = 0x0066;
            Delay(3);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8038) = 0x0C83;
            Delay(3);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x803C) = 0x0021;
            Delay(3);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8040) = 0x00ac; // Changing Preemp may help
            Delay(3);

            // Phase Interpolator
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC078) = 0xC2F0;
            Delay(3);
            //RX error level - by default set to 0x100; setting error level 0x0
//            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8028) = 0x080;

            // Serdes DLL settings - Need this for 3.125GHz
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC008) = 0x262;
            Delay(3);
            break;
        case CPRIRATE_6144:
            // Serdes DLL settings - Need this for 3.125GHz
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC008) = 0x262;
            Delay(3);

            // Preemp 0xc1 default, 0x9b works better, also 0xac
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8034) = 0x0066; // 7E Changing Tx amplitude may help
            Delay(3);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8038) = 0x0C83;
            Delay(3);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x803C) = 0x0021;
            Delay(3);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8040) = 0x00ac;
            Delay(3);
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8030) = 0x007f; // 0f or 7f
            Delay(3);

            // Phase Interpolator
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0xC078) = 0xC2F0;
            Delay(3);
            //RX error level - by default set to 0x100; setting error level 0x0
            REG32(RAD_APB_SERDES1_2CH_BASEADDR+0x8028) = 0x080;
            Delay(3);
            break;
    }

// DLL config 0 setup for 5G - seems to break pre-driver loopback

    if (loopback == 1)
    { // Serdes1 Internal loopback
        REG32(RAD_CFG_SRDS_MMD) = 0x3;   // mmd
        REG32(RAD_CFG_SRDS_LANE) = 3; // broadcast to 1 or 2 lanes
        REG32(0xFE908000) |= 1<<14;
    }
    else if (loopback == 2)
    { // Pre-driver loopback
        REG32(0xFE908050) = 0x2;
        REG32(0xFE908000) = 0x0B01;
    }

    if (prbs)
    { // Change SerDes1 to PRBS mode
        REG32(RAD_CFG_SRDS_MMD) = 3;
        // select lane0
        REG32(RAD_CFG_SRDS_LANE) = 1;

        saved_pcs_mode = REG32(RAD_SRDS1_PCS_MODE);
        saved_pcs_prbs_conf = REG32(RAD_SRDS1_PCS_PRBS_CONFIG);
        saved_pcs_reset = REG32(RAD_SRDS1_PCS_PRBS_RESET);

//      REG32(RAD_SRDS1_PCS_MODE) = 0x36CC; //clock pattern
        REG32(RAD_SRDS1_PCS_MODE) = 0x9CC;// | (1<<14); // PCS Loopback
        REG32(RAD_SRDS1_PCS_PRBS_CONFIG) = 0xF;

        // pattern
//      REG32(RAD_APB_SERDES1_2CH_BASEADDR + 0xC01C) = 0x1F; // clock

        REG32(RAD_SRDS1_PCS_PRBS_RESET) = 0xFFCF;

        Delay(20);

        prbs_timer = (REG32(RAD_SRDS1_PCS_PRBS_TIMER1) << 16) | REG32(RAD_SRDS1_PCS_PRBS_TIMER2);
        prbs_err = (REG32(RAD_SRDS1_PCS_PRBS_ERRCNT0) << 16) | REG32(RAD_SRDS1_PCS_PRBS_ERRCNT1);
        uart_printf("Serdes1 Lane0: prbs_timer=%d  prbs_err=%d\r\n", prbs_timer, prbs_err);

        REG32(RAD_SRDS1_PCS_MODE) = saved_pcs_mode;
        REG32(RAD_SRDS1_PCS_PRBS_CONFIG) = saved_pcs_prbs_conf;
        REG32(RAD_SRDS1_PCS_PRBS_RESET) = saved_pcs_reset;

        REG32(RAD_CFG_SRDS_MMD) = 3;
        // select lane1
        REG32(RAD_CFG_SRDS_LANE) = 2;

        saved_pcs_mode = REG32(RAD_SRDS1_PCS_MODE);
        saved_pcs_prbs_conf = REG32(RAD_SRDS1_PCS_PRBS_CONFIG);
        saved_pcs_reset = REG32(RAD_SRDS1_PCS_PRBS_RESET);

        REG32(RAD_SRDS1_PCS_MODE) = 0x9CC;
        REG32(RAD_SRDS1_PCS_PRBS_CONFIG) = 0xF;
        REG32(RAD_SRDS1_PCS_PRBS_RESET) = 0xFFCF;

        Delay(20);

        prbs_timer = (REG32(RAD_SRDS1_PCS_PRBS_TIMER1) << 16) | REG32(RAD_SRDS1_PCS_PRBS_TIMER2);
        prbs_err = (REG32(RAD_SRDS1_PCS_PRBS_ERRCNT0) << 16) | REG32(RAD_SRDS1_PCS_PRBS_ERRCNT1);

//        uart_printf("Serdes1 Lane1: prbs_timer=%d  prbs_err=%d\r\n", prbs_timer, prbs_err);

        REG32(RAD_SRDS1_PCS_MODE) = saved_pcs_mode;
        REG32(RAD_SRDS1_PCS_PRBS_CONFIG) = saved_pcs_prbs_conf;
        REG32(RAD_SRDS1_PCS_PRBS_RESET) = saved_pcs_reset;
    }
}

void CpriSrds1Init(UINT32 CpriRate, UINT32 loopback, UINT32 prbs)
{
    REG32(RAD_CPRI0_RESET_0) = (0xF << 4) | 1;
    REG32(RAD_CPRI0_RESET_1) = (0xF << 4) | 1;
    REG32(RAD_CPRI1_RESET_0) = (0xF << 4) | 1;
    REG32(RAD_CPRI1_RESET_1) = (0xF << 4) | 1;

    Delay(2);

    // to prevent Serdes reinitialization
    // if it was already initalized

    //if ((REG32(RAD_SRDS1_CNTRL) & 1) == 0)
    if (Serdes1IsInited(CpriRate, loopback))
    {
        uart_printf ("Serdes-1 is already initialized\r\n");
        return;
    }
    else
    {
        uart_printf ("Serdes-1 initialization ... ");
    }

    if (DevInfoIsT4Kx4())
    {
        CpriSrds1InitX1(CpriRate, loopback, prbs);
    }
    else
    {
        Serdes1Init(CpriRate, SRDSMODE_CPRI, loopback, prbs);
    }

    uart_printf ("OK\r\n");
}

PCPRICTX GetCPRICtx(UINT32 DevID)
{
    if(pCpriDrvCtx)
        return &pCpriDrvCtx->CpriCtx[DevID];
    return NULL;
}

PCPRIDRVCTX CPRIDrvGetCtx (void)
{
    return pCpriDrvCtx;
}

UINT32 GetCPRIDmaSize(UINT32 DevID)
{
    if(pCpriDrvCtx != NULL)
        return pCpriDrvCtx->CpriCtx[DevID].DmaIntSize;

    return 0;
}

LPVOID CPRIDrvGetTxBuffer (UINT32 nDevID)
{
    PCPRICTX pCpriCtx = NULL;
    LPVOID pBuf = NULL;

    // We need to check special MIMO mode (1 device + 2-AC)
    // in this case dev-0 is used

    pCpriCtx = GetCPRICtx(0);
    _ASSERT_PTR(pCpriCtx);

    if(!pCpriCtx)
            return NULL;

    if (pCpriCtx->CpriConfType == CPRI_EMULATOR)
    {
        pBuf = pCpriCtx->pEmulTxBuf[nDevID];
    }
    else if (pCpriCtx->CpriConfType == CPRI_MIMO_2AC)
    {
        // in this case nDevID is equal to the antenna buffer
        // nDevID * (pCpriCtx->DmaIntSize << 1) is not used due to performance
        pBuf = &pCpriCtx->pTxDmaBuf[pCpriCtx->TxBufOffset + ((nDevID != 0) ? (pCpriCtx->DmaIntSize << 1) : 0)];
    }
    else if ((pCpriCtx = GetCPRICtx(nDevID)) != NULL)
    {
        if (pCpriCtx->CpriConfType == CPRI_IXIA)
        {
            pBuf = &pCpriCtx->pTxDmaBuf[pCpriCtx->TxBufOffset];
        }
        else if (pCpriCtx->CpriConfType == CPRI_RADIOCOMP)
        {
            pBuf = &pCpriCtx->pTxDmaBuf[(pCpriCtx->DmaIntSize << 1) + pCpriCtx->TxBufOffset]; // using AC1 for TX
        }
    }

    _ASSERT_PTR(pBuf);

    return pBuf;
}

LPVOID CPRIDrvGetRxBuffer (UINT32 nDevID)
{
    PCPRICTX pCpriCtx;
    LPVOID pBuf = NULL;

    pCpriCtx = GetCPRICtx(0);
    _ASSERT_PTR(pCpriCtx);

    if (pCpriCtx->CpriConfType == CPRI_EMULATOR)
    {
           pBuf = pCpriCtx->pEmulRxBuf[nDevID];
    }
    else if (pCpriCtx->CpriConfType == CPRI_MIMO_2AC)
    {
        // in this case nDevID is equal to the antenna buffer
        // nDevID * (pCpriCtx->DmaIntSize << 1) is not used due to performance
        pBuf = &pCpriCtx->pRxDmaBuf[pCpriCtx->RxBufOffset + ((nDevID != 0) ? (pCpriCtx->DmaIntSize << 1) : 0)];
    }
    else if ((pCpriCtx = GetCPRICtx(nDevID)) != NULL)
    {
        if (pCpriCtx->CpriConfType == CPRI_IXIA)
        {
            pBuf = &pCpriCtx->pRxDmaBuf[pCpriCtx->RxBufOffset];
        }
        else if (pCpriCtx->CpriConfType == CPRI_RADIOCOMP)
        {
            //pBuf = &pCpriCtx->pRxDmaBuf[(pCpriCtx->DmaIntSize << 1) + pCpriCtx->RxBufOffset]; // using AC1 for TX

            // according to the CPRI initialization, we need to use AC0 for RX
            pBuf = &pCpriCtx->pRxDmaBuf[pCpriCtx->RxBufOffset]; // using AC0 for RX
        }
    }

    _ASSERT_PTR(pBuf);

    return pBuf;
}

MXRC CPRIDrvInit(PCPRICONFIG pCpriConfig)
{
    UINT32 NumDev = pCpriConfig->NumCpriDev;
    UINT32 CpriRate = pCpriConfig->CpriRate;
    UINT32 i;
    UINT32 DevID;
    PCPRICTX pCpriCtx;
    PCPRIREGS pCpriRegs;
    PCPDMAREGS pCpdmaRegs;
    UINT32 bufsize;
    UINT32 idx;
    UINT32 map_n_ac=pCpriConfig->SamplingRate / CPRI_CHIP_SRATE;
    UINT32 map_ac = 8*CpriRate*15/2/map_n_ac/pCpriConfig->SampleWidth;
    MXRC rc = CPRI_RC_OK;
    UINT32 TxIrqGpioSet = 0;
    UINT32 RxIrqGpioSet = 0;

    if (map_ac > pCpriConfig->MaxAcNum)
        map_ac = pCpriConfig->MaxAcNum;

    pCpriDrvCtx = (PCPRIDRVCTX)MxHeapAlloc(AppHeap, sizeof(CPRIDRVCTX));

    if (pCpriDrvCtx == NULL)
    {
        rc = CPRI_RC_ALLOC_ERROR;
        _ASSERT_RC (rc);
        return rc;
    }

    memset(pCpriDrvCtx, 0, sizeof(CPRIDRVCTX));

    pCpriDrvCtx->CpriRate     = CpriRate;
    pCpriDrvCtx->CpuID        = MxGetCpuID();
    pCpriDrvCtx->EmulTickTime = pCpriConfig->EmulTickTime;
    pCpriDrvCtx->DiUsed       = pCpriConfig->DiUsed;

    REG32(RAD_CFG_CPRI_RATE) = 0;
    for (DevID = 0; DevID < NumDev; DevID++, CpriRate <<= 4)
        REG32(RAD_CFG_CPRI_RATE) |= CpriRate;

    REG32(RAD_AXI_RESET_0) = 0; // release global reset and all CPRI AXIs

    REG32(RAD_CPRI0_RESET_0) = (0xF << 4) | 1;
    REG32(RAD_CPRI0_RESET_1) = (0xF << 4) | 1;
    REG32(RAD_CPRI1_RESET_0) = (0xF << 4) | 1;
    REG32(RAD_CPRI1_RESET_1) = (0xF << 4) | 1;
    Delay(2);
    REG32(RAD_CPRI0_RESET_0) = 0;
    REG32(RAD_CPRI0_RESET_1) = 0;
    REG32(RAD_CPRI1_RESET_0) = 0;
    REG32(RAD_CPRI1_RESET_1) = 0;

    REG32(RAD_RE_SRDS1_RX_CLK_OUT_CNTRL) |= 1 << 4; // enable output clock
    REG32(RAD_CFG_PDEN_REG) &= ~(1<<3);
    Delay(2);

    for (DevID = 0; DevID < NumDev; DevID++)
    {
        if (DevID < 2)
            REG32(RAD_CFG_PDEN_REG) &= ~(1<<(DevID + 3)); // power up CPRI

        pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];
        pCpriCtx->DevID = DevID;

        pCpriCtx->CpriConfType = pCpriConfig->CpriConfType;

        //pCpriRegs = (PCPRIREGS)RADIP_BASEADDR(DevID);
        pCpriRegs = CpriGetRegs(pCpriConfig, DevID);
        pCpriCtx->pCpriRegs = pCpriRegs;

        //pCpdmaRegs = (PCPDMAREGS)CPDMA_BASEADDR(DevID);
        pCpdmaRegs = CpriGetCpDma(pCpriConfig, DevID);
        pCpriCtx->pCpdmaRegs = pCpdmaRegs;

        pCpdmaRegs->RX_DMA_ENA |= (1<<1);
        pCpdmaRegs->TX_DMA_ENA |= (1<<1);
        Delay(1);
        pCpdmaRegs->RX_AC_PTR[DevID] = 0;
        pCpdmaRegs->TX_AC_PTR[DevID] = 0;

        pCpriCtx->RxPtr = pCpriCtx->TxPtr = pCpriCtx;
        pCpriCtx->RxStartDelayCounter = pCpriConfig->RxStartDelayCounter;

        // for the DataInterface phydi.c configures this variables
        if (pCpriConfig->DiUsed == 0)
        {
            if (pCpriCtx->CpriConfType == CPRI_MIMO_2AC)
            {
                /* antenna 0 */
                pCpriCtx->pTxLogBuf[0] = (UINT8*)gDlIqBuf;
                pCpriCtx->TxLogBufSize = DL_IQ_SAMPLES_BUF_SIZE * sizeof(SINT16)/2;
                pCpriCtx->pRxLogBuf[0] = (UINT8*)gUlIqBuf;
                pCpriCtx->RxLogBufSize = DL_IQ_SAMPLES_BUF_SIZE * sizeof(SINT16)/2; //0x8000000/2;
                pCpriCtx->TxLogBufReset[0] = 0;
                pCpriCtx->RxLogBufReset[0] = 0;

                /* antenna 1 */
                pCpriCtx->pTxLogBuf[1] = pCpriCtx->pTxLogBuf[0] + pCpriCtx->TxLogBufSize;
                pCpriCtx->TxLogBufSize = pCpriCtx->TxLogBufSize;
                pCpriCtx->pRxLogBuf[1] = pCpriCtx->pRxLogBuf[0] + pCpriCtx->RxLogBufSize;
                pCpriCtx->RxLogBufSize = pCpriCtx->RxLogBufSize; //0x8000000/2;
                pCpriCtx->TxLogBufReset[1] = 0;
                pCpriCtx->RxLogBufReset[1] = 0;
            }
            else
            {
                pCpriCtx->pTxLogBuf[0] = (UINT8*)gDlIqBuf;
                pCpriCtx->TxLogBufSize = DL_IQ_SAMPLES_BUF_SIZE * sizeof(SINT16); //0x8000000/2;
                pCpriCtx->pRxLogBuf[0] = (UINT8*)gUlIqBuf;
                pCpriCtx->RxLogBufSize = DL_IQ_SAMPLES_BUF_SIZE * sizeof(SINT16); //0x8000000/2;
                pCpriCtx->TxLogBufReset[0] = 0;
                pCpriCtx->RxLogBufReset[0] = 0;
            }
        }

        pCpriCtx->TxEthBDesc = (CPRIETHBDESC*)MxHeapAlloc (DspHeap, (NUM_CPRI_ETH_TX_BUF + NUM_CPRI_ETH_RX_BUF) * sizeof (CPRIETHBDESC));
        pCpriCtx->RxEthBDesc = pCpriCtx->TxEthBDesc + NUM_CPRI_ETH_TX_BUF;

        _ASSERT_PTR (pCpriCtx->TxEthBDesc);

        memset (pCpriCtx->TxEthBDesc, 0, NUM_CPRI_ETH_TX_BUF * sizeof (CPRIETHBDESC));
        memset (pCpriCtx->RxEthBDesc, 0, NUM_CPRI_ETH_RX_BUF * sizeof (CPRIETHBDESC));

        CPRI_RX_LOG_DISABLE(pCpriCtx);
        CPRI_TX_LOG_DISABLE(pCpriCtx);

        pCpriCtx->RxStopCount = 0;
        pCpriCtx->TxStopCount = 0;

        pCpriCtx->RxLogIrqStart = 0;
        pCpriCtx->TxLogIrqStart = 0;

        // IQ test
        pCpriCtx->DmaIntSize = 2*sizeof(UINT16)*pCpriConfig->SamplingRate; // 1 ms (one LTE subframe) interrupt size
        bufsize = 2*pCpriCtx->DmaIntSize;

        pCpriCtx->pTxDmaBuf = (PUINT8)QueryCramIqBuf(0, 0, 0);
        pCpriCtx->pRxDmaBuf = (PUINT8)QueryCramIqBuf(1, 0, 0);

        if(pCpriCtx->pTxDmaBuf == NULL)
        {
            uart_printf("Can't allocate CPRI DMA buffers (2*%d*%d=%d)\r\n", map_ac, bufsize, 2*map_ac*bufsize);
            rc = CPRI_RC_ALLOC_ERROR;
            break;
        }
        else
        {
            uart_printf("TxBuf=0x%X RxBuf=0x%X Size=%d\r\n", pCpriCtx->pTxDmaBuf, pCpriCtx->pRxDmaBuf, 2*map_ac*bufsize);
        }

        memset (pCpriCtx->pRxDmaBuf, 0, bufsize);

        pCpriDrvCtx->DevEnaMask |= 1 << DevID;

        pCpriCtx->NumAxC = map_ac;
        pCpriCtx->CpriMapConf = (map_ac<<8) | map_n_ac;

        for (i = 0; i < 255; i++)
        {
            pCpriRegs->CPRI_CTRL_INDEX = i;
            pCpriRegs->CPRI_TX_CTRL = 0;
        }

        pCpriRegs->CPRI_MAP_RX_READY_THR = 0x1F; // half of FIFO size
        pCpriRegs->CPRI_MAP_TX_READY_THR = 0x1F; // half of FIFO size

        pCpriRegs->CPRI_MAP_CONFIG = (pCpriConfig->SampleWidth == 15) ? (1<<4) : 0; // Basic Mapping Mode, TX and RX FIFO mapping Interface

        pCpriRegs->CPRI_IQ_RX_BUF_CONTROL = 0; //disable RX MAP interface

        // Fast C&M Configuration
        CPRIDrvSetReMac(pCpriRegs, pCpriConfig->ReMacAddr);
        CPRIDrvSetReIp(pCpriRegs, pCpriConfig->ReIpAddr);
        CPRIDrvSetRecIp(pCpriRegs, pCpriConfig->ReIpAddr);

        pCpriCtx->ReIpAddr = pCpriConfig->ReIpAddr;
        pCpriCtx->RecIpAddr = pCpriConfig->RecIpAddr;

        for (i = 0; i < 6; i++)
        {
            pCpriCtx->RecMacAddr[i] = pCpriConfig->RecMacAddr[i];
            pCpriCtx->ReMacAddr[i] = pCpriConfig->ReMacAddr[i];
        }

        pCpriRegs->CPRI_CM_CONFIG = pCpriConfig->EthRate; // 0x24..0x3f range. Use 0x3f for Powerwave.
        CPRIDrvSetCtrlWord(pCpriRegs, CPRI_PWAV_CM_SEL, 0xCC); // 0xCC

        pCpriCtx->HdlcEnable = pCpriConfig->HdlcEnable;
        pCpriCtx->EthEnable = pCpriConfig->EthEnable;
        if (pCpriCtx->EthEnable)
        {
            pCpriRegs->ETH_CONFIG_1 = 0;
            if (pCpriConfig->EthLittleEndian)
                pCpriRegs->ETH_CONFIG_1 |= CPRI_ETHC1_LEND;
            pCpriCtx->TxEthBufPut = 0;
            pCpriCtx->TxEthBufGet = 0;
            pCpriCtx->RxEthBufPut = 0;
            pCpriCtx->RxEthBufGet = 0;
            pCpriCtx->RxEthBDesc[0].BPtr = MxHeapAlloc(DspHeap, NUM_CPRI_ETH_RX_BUF*1536);
            pCpriCtx->RxEthBDesc[0].BCtrl = 0;
            for (i = 1; i < NUM_CPRI_ETH_RX_BUF; i++)
            {
                pCpriCtx->RxEthBDesc[i].BPtr = pCpriCtx->RxEthBDesc[i-1].BPtr + 1536;
                pCpriCtx->RxEthBDesc[1].BCtrl = 0;
            }
            if (pCpriConfig->EthFcsEnable)
                pCpriRegs->ETH_CONFIG_2 |= 1;
        }

        // CPDMA setup
        pCpdmaRegs->MEMSEG = (UINT32)pCpriCtx->pTxDmaBuf >> 24;
        pCpdmaRegs->BLK_SIZE = bufsize/8 - 1;
        pCpdmaRegs->INT_SIZE = pCpriCtx->DmaIntSize/8 - 1;

        // software reset
#if 0
        pCpdmaRegs->TX_AC_ENA |= CPDMA_RXSRESET;
        pCpdmaRegs->RX_AC_ENA |= CPDMA_TXSRESET;
#else
        pCpdmaRegs->RX_DMA_ENA |= CPDMA_RXSRESET;
        pCpdmaRegs->TX_DMA_ENA |= CPDMA_TXSRESET;

        // to wait 10 us
        MxDelayTicks (150*10);
#endif

        pCpriCtx->UseRxAntenna = pCpriConfig->UseRxAntenna;
        pCpriCtx->UseTxAntenna = pCpriConfig->UseTxAntenna;


        pCpdmaRegs->DMA_CTRL = 0xFFFF; // 8 64-bit words threshold and AXI burst size for TX and RX

        pCpdmaRegs->TX_DMA_ENA = 0;
        pCpdmaRegs->RX_DMA_ENA = 0;

        pCpdmaRegs->RX_INT_ENA = 0;
        pCpdmaRegs->TX_INT_ENA = 0;

        pCpdmaRegs->TX_AC_ENA  = 0;
        pCpdmaRegs->RX_AC_ENA  = 0;

        uart_printf("CPRI:map_ac = %d, DMAINTSIZE=%d\r\n", map_ac, pCpriCtx->DmaIntSize);

        // setting up buffer for each antenna carrier

        for (idx = 0; idx < map_ac; idx++)
        {
            if(pCpriCtx->UseTxAntenna == CPRI_USE_DEFAULT)
            {
            pCpdmaRegs->TX_AC_BASE[idx] = ((UINT32)pCpriCtx->pTxDmaBuf + idx*bufsize) & 0x00FFFFFF;
                pCpdmaRegs->TX_AC_PTR[idx] = 0;
            }

            if(pCpriCtx->UseRxAntenna == CPRI_USE_DEFAULT)
            {
                pCpdmaRegs->RX_AC_BASE[idx] = ((UINT32)pCpriCtx->pRxDmaBuf + idx*bufsize) & 0x00FFFFFF;
                pCpdmaRegs->RX_AC_PTR[idx] = 0;
            }

            uart_printf (" *TX=0x%x, RX=0x%x\r\n", ((UINT32)pCpriCtx->pTxDmaBuf + idx*bufsize), ((UINT32)pCpriCtx->pRxDmaBuf + idx*bufsize));

            // this is IXIA specific configuration, used for MIMO
            // based on 1 CPRI device and 2-AC
            if (pCpriCtx->CpriConfType == CPRI_MIMO_2AC)
            {
                if(pCpriCtx->UseTxAntenna == CPRI_USE_DEFAULT)
                {
                    pCpdmaRegs->TX_INT_ENA |= 1 << idx;
                    pCpdmaRegs->TX_AC_ENA |= 1 << idx;
                }

                if(pCpriCtx->UseRxAntenna == CPRI_USE_DEFAULT)
                {
                     pCpdmaRegs->RX_INT_ENA |= 1 << idx;
                     pCpdmaRegs->RX_AC_ENA |= 1 << idx;
                }
            }
            else
            {
                if ( (pCpriCtx->UseTxAntenna == CPRI_USE_DEFAULT) && (pCpriCtx->CpriConfType != CPRI_RADIOCOMP || idx == 1))
                {
                    pCpdmaRegs->TX_INT_ENA |= 1 << idx;
                    pCpdmaRegs->TX_AC_ENA |= 1 << idx;
                }

                if ( (pCpriCtx->UseRxAntenna == CPRI_USE_DEFAULT) && (pCpriCtx->CpriConfType != CPRI_RADIOCOMP || idx == 0))
                {
                    pCpdmaRegs->RX_INT_ENA |= 1 << idx;
                    pCpdmaRegs->RX_AC_ENA |= 1 << idx;
                }
            }
        }

        if(pCpriCtx->UseRxAntenna == CPRI_USE_ANTENNA_0)
        {
            pCpdmaRegs->RX_AC_BASE[0] = ((UINT32)pCpriCtx->pRxDmaBuf) & 0x00FFFFFF;
            pCpdmaRegs->RX_AC_PTR[0] = 0;
            pCpdmaRegs->RX_INT_ENA |= 1;
            pCpdmaRegs->RX_AC_ENA |= 1;
        }
        else if(pCpriCtx->UseRxAntenna == CPRI_USE_ANTENNA_1)
        {
            pCpdmaRegs->RX_AC_BASE[1] = ((UINT32)pCpriCtx->pRxDmaBuf) & 0x00FFFFFF;
            pCpdmaRegs->RX_AC_PTR[1] = 0;
            pCpdmaRegs->RX_INT_ENA |= 1 << 1;
            pCpdmaRegs->RX_AC_ENA |= 1 << 1;
        }

        if(pCpriCtx->UseTxAntenna == CPRI_USE_ANTENNA_0)
        {
            pCpdmaRegs->TX_AC_BASE[0] = ((UINT32)pCpriCtx->pTxDmaBuf) & 0x00FFFFFF;
            pCpdmaRegs->TX_AC_PTR[0] = 0;
            pCpdmaRegs->TX_INT_ENA |= 1;
            pCpdmaRegs->TX_AC_ENA |= 1;
        }
        else if(pCpriCtx->UseTxAntenna == CPRI_USE_ANTENNA_1)
        {
            pCpdmaRegs->TX_AC_BASE[1] = ((UINT32)pCpriCtx->pTxDmaBuf) & 0x00FFFFFF;
            pCpdmaRegs->TX_AC_PTR[1] = 0;
            pCpdmaRegs->TX_INT_ENA |= 1 << 1;
            pCpdmaRegs->TX_AC_ENA |= 1 << 1;
        }

        if ( pCpriConfig->pCPRICreateCallBack )
        {
            rc = pCpriConfig->pCPRICreateCallBack(pCpriDrvCtx);
            if (rc != 0)
                break;
        }

        for (idx = 0; idx < map_ac; idx++)
        {
            uart_printf("CPRI log ant %d DL @%08x %d\n", idx, pCpriCtx->pTxLogBuf[idx], pCpriCtx->TxLogBufSize);
            uart_printf("CPRI log ant %d UL @%08x %d\n", idx, pCpriCtx->pRxLogBuf[idx], pCpriCtx->RxLogBufSize);
        }

        CpriRxLatency = AUTO_CPRI_LOOPBACK_SFN_LATENCY;

        if (pCpriConfig->CpriConfType == CPRI_EMULATOR)
            continue;

        pCpdmaRegs->RX_INT_STAT = pCpdmaRegs->RX_INT_STAT;
        pCpdmaRegs->TX_INT_STAT = pCpdmaRegs->TX_INT_STAT;

        ICDrvClearIrq(HAL_IRQ_CPDMA_TX_0+DevID);
        ICDrvClearIrq(HAL_IRQ_CPDMA_RX_0+DevID);

        MxSetIntHandler(HAL_IRQ_CPDMA_TX_0+DevID, INT_TYPE_IRQ, PRI_CPRI, CpdmaTxIrqHandler,
                        (LPVOID)pCpriCtx);
        // TODO: Move to CPRIDrvEnableDma
        MxEnableInt(HAL_IRQ_CPDMA_TX_0+DevID);
        MxSetIntHandler(HAL_IRQ_CPDMA_RX_0+DevID, INT_TYPE_IRQ, PRI_CPRI, CpdmaRxIrqHandler,
                        (LPVOID)pCpriCtx);
        // TODO: Move to CPRIDrvEnableDma
        MxEnableInt(HAL_IRQ_CPDMA_RX_0+DevID);

        //pCpdmaRegs->TX_INT_ENA = pCpriDrvCtx->DevEnaMask;
        //pCpdmaRegs->RX_INT_ENA = pCpriDrvCtx->DevEnaMask;

        // enable antenna carriers
        //pCpdmaRegs->TX_AC_ENA = pCpriDrvCtx->DevEnaMask;
        //CpdmaRegs->RX_AC_ENA = pCpriDrvCtx->DevEnaMask;

        MxSetIntHandler(HAL_IRQ_CPRI_0+DevID, INT_TYPE_IRQ, PRI_CPRI, CpriIrqHandler, (LPVOID)pCpriCtx);
        // TODO: Move to CPRIDrvEnableDma
        MxEnableInt(HAL_IRQ_CPRI_0+DevID);

        pCpriRegs->ETH_CONFIG_1 |= CPRI_ETHC1_TXAIE | CPRI_ETHC1_RXREIE | CPRI_ETHC1_RXRBIE | CPRI_ETHC1_TXIE | CPRI_ETHC1_RXIE | CPRI_ETHC1_IE;

        pCpriRegs->CPRI_CONFIG = 0x21; // Tx enable, Global enable on control words insertion

        if(TxIrqGpioSet == 0)
        {
            UINT32 TxIrqGpioEnaCfg = (CpriApiConfigSet)? CpriApiConfig.TxIrqGpioEna : pCpriConfig->TxIrqGpioEna;
            UINT32 TxIrqGpioPortCfg = (CpriApiConfigSet)? CpriApiConfig.TxIrqGpioPort: pCpriConfig->TxIrqGpioPort;
            UINT32 TxIrqGpioToggleDelays = (CpriApiConfigSet)? CpriApiConfig.TxIrqGpioToggleDelays: pCpriConfig->TxIrqGpioToggleDelays;

            if(TxIrqGpioEnaCfg & (1 << DevID))
            {
                pCpriCtx->TxIrqGpioEna = 1;
                pCpriCtx->TxIrqGpioPort = TxIrqGpioPortCfg;
                REG32(GPIO_PIN_SELECT_REG) |= (1 << pCpriCtx->TxIrqGpioPort); // GPIO functionality
                REG32(GPIO_OE_REG) |= (1 << pCpriCtx->TxIrqGpioPort); // enable
                REG32(GPIO_OUTPUT_REG) &= ~(1 << pCpriCtx->TxIrqGpioPort); // low at start
                TxIrqGpioSet = 1;

                pCpriCtx->TxIrqGpioToggleDelays[0] = TxIrqGpioToggleDelays >> 16;
                if(pCpriCtx->TxIrqGpioToggleDelays[0] == 0)
                    pCpriCtx->TxIrqGpioToggleDelays[0] = 1;
                pCpriCtx->TxIrqGpioToggleDelays[1] = TxIrqGpioToggleDelays & 0xFFFF;
                if(pCpriCtx->TxIrqGpioToggleDelays[1] == 0)
                    pCpriCtx->TxIrqGpioToggleDelays[1] = 1;
                pCpriCtx->TxIrqGpioToggleDelayIdx = 0;
                pCpriCtx->TxIrqGpioToggleCntr = 0;
            }
        }

        if(RxIrqGpioSet == 0)
        {
            UINT32 RxIrqGpioEnaCfg = (CpriApiConfigSet)? CpriApiConfig.RxIrqGpioEna : pCpriConfig->RxIrqGpioEna;
            UINT32 RxIrqGpioPortCfg = (CpriApiConfigSet)? CpriApiConfig.RxIrqGpioPort: pCpriConfig->RxIrqGpioPort;
            UINT32 RxIrqGpioToggleDelays = (CpriApiConfigSet)? CpriApiConfig.RxIrqGpioToggleDelays: pCpriConfig->RxIrqGpioToggleDelays;

            if(RxIrqGpioEnaCfg & (1 << DevID))
            {
                pCpriCtx->RxIrqGpioEna = 1;
                pCpriCtx->RxIrqGpioPort = RxIrqGpioPortCfg;
                REG32(GPIO_PIN_SELECT_REG) |= (1 << pCpriCtx->RxIrqGpioPort); // GPIO functionality
                REG32(GPIO_OE_REG) |= (1 << pCpriCtx->RxIrqGpioPort); // enable
                REG32(GPIO_OUTPUT_REG) &= ~(1 << pCpriCtx->RxIrqGpioPort); // low at start
                RxIrqGpioSet = 1;

                pCpriCtx->RxIrqGpioToggleDelays[0] = RxIrqGpioToggleDelays >> 16;
                if(pCpriCtx->RxIrqGpioToggleDelays[0] == 0)
                    pCpriCtx->RxIrqGpioToggleDelays[0] = 1;
                pCpriCtx->RxIrqGpioToggleDelays[1] = RxIrqGpioToggleDelays & 0xFFFF;
                if(pCpriCtx->RxIrqGpioToggleDelays[1] == 0)
                    pCpriCtx->RxIrqGpioToggleDelays[1] = 1;
                pCpriCtx->RxIrqGpioToggleDelayIdx = 0;
                pCpriCtx->RxIrqGpioToggleCntr = 0;
            }
        }

        uart_printf("CPRI LogControl: 0x%x\r\n", pCpriCtx->LogCtrl);
        uart_printf("CPRI TxLogEnabled:%d (0x%x), RxLogEnabled:%d(0x%x)\r\n", pCpriCtx->TxLogEnable, pCpriCtx->pTxLogBuf[0], pCpriCtx->RxLogEnable, pCpriCtx->pRxLogBuf[0]);

        // Wait for link 1000 ms

        //rc = CPRIDrvWaitForLink(pCpriRegs, (pCpriConfig->UpDelay) ? pCpriConfig->UpDelay : CPRI_UP_DELAY);
		rc = CPRIDrvWaitForLink(pCpriRegs, 20000);
        if (rc != 0)
        {
            if (rc == 2)
                uart_printf("[NO CPRI link, status error bits are detected]\r\n");
            else if (rc == 3)
                uart_printf("[NO CPRI link, 10ms Radio frame pulse is not detected]\r\n");
            else
                uart_printf("[NO CPRI link], rc=%d\n", rc);

            break;
        }
    }

#ifdef ICPU_FORWARDING
    pTxEthFrame = (PUINT8)MxHeapAlloc(AppHeap, MAX_ICPU_COMMAND_SIZE*NUM_CPRI_ETH_TX_BUF);
    TxEthFrameInd = 0;
    pRxEthFrame = (PUINT8)MxHeapAlloc(AppHeap, MAX_ICPU_COMMAND_SIZE);

    rc = SvsrRegClient(31, CPRIApiHandler, NULL);
#endif

    if(rc != CPRI_RC_OK)
        uart_printf("CPRI INIT done: rc=%X\r\n", rc);

    return rc;
}

MXRC CPRIDrvShutdown(void)
{
    MXRC rc = CPRI_RC_OK;

    if(pCpriDrvCtx && (MxGetCpuID() == pCpriDrvCtx->CpuID))
    {
        PCPRICTX pCpriCtx;
        UINT32 DevID;

        for (DevID = 0; DevID < CPRI_DEV_NUM; DevID++)
        {
            pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];

            if(pCpriCtx->pCpriRegs != NULL)
            {// used dev
                MxDisableInt(HAL_IRQ_CPRI_0 + DevID);
                MxDisableInt(HAL_IRQ_CPDMA_TX_0 + DevID);
                MxDisableInt(HAL_IRQ_CPDMA_RX_0 + DevID);
                CPRIDrvDisableDma(DevID);

                if (pCpriCtx->TxEthBDesc)
                {
                    MxHeapFree(DspHeap, pCpriCtx->TxEthBDesc);
                    pCpriCtx->TxEthBDesc = NULL;
                }

                if(pCpriCtx->RxEthBDesc[0].BPtr)
                {
                    MxHeapFree(DspHeap, pCpriCtx->RxEthBDesc[0].BPtr);
                    pCpriCtx->RxEthBDesc[0].BPtr = NULL;
                }

               if(pCpriCtx->RxIrqGpioEna)
               {
                   REG32(GPIO_OE_REG) &= ~(1 << pCpriCtx->RxIrqGpioPort); // disable
               }
               if(pCpriCtx->TxIrqGpioEna)
               {
                   REG32(GPIO_OE_REG) &= ~(1 << pCpriCtx->TxIrqGpioPort); // disable
               }

            }

            if (DevID == 0)
            {
                if (pCpriCtx->CpriConfType == CPRI_MIMO_2AC)
                {
                    uart_printf("TxLog Buf0 Pos %d, Reset %d times\nTxLog Buf1 Pos %d, reset %d times\n",
                        pCpriCtx->TxLogBufPos[0] / pCpriCtx->DmaIntSize, pCpriCtx->TxLogBufReset[0],
                        pCpriCtx->TxLogBufPos[1] / pCpriCtx->DmaIntSize, pCpriCtx->TxLogBufReset[1]);
                    uart_printf("RxLog Buf0 Pos %d, Reset %d times\nRxLog Buf1 Pos %d, reset %d times\n",
                        pCpriCtx->RxLogBufPos[0] / pCpriCtx->DmaIntSize, pCpriCtx->RxLogBufReset[0],
                        pCpriCtx->RxLogBufPos[1] / pCpriCtx->DmaIntSize, pCpriCtx->RxLogBufReset[1]);
                }
                else
                {
                    uart_printf("TxLog Buf0 Pos %d, Reset %d times\n",
                        pCpriCtx->TxLogBufPos[0] / pCpriCtx->DmaIntSize, pCpriCtx->TxLogBufReset[0]);
                    uart_printf("RxLog Buf0 Pos %d, Reset %d times\n",
                        pCpriCtx->RxLogBufPos[0] / pCpriCtx->DmaIntSize, pCpriCtx->RxLogBufReset[0]);
                }
            }
        }

#ifdef ICPU_FORWARDING
        if (pTxEthFrame)
        {
            MxHeapFree(AppHeap, pTxEthFrame);
            pTxEthFrame = NULL;
        }
        if (pRxEthFrame)
        {
            MxHeapFree(AppHeap, pRxEthFrame);
            pRxEthFrame = NULL;
        }
#endif
        if (pCpriDrvCtx)
        {
            MxHeapFree(AppHeap, pCpriDrvCtx);
            pCpriDrvCtx = NULL;
        }
        uart_printf("CPRI is disabled\n");
    }

    return rc;
}

UINT32 CPRIDrvGetTxLogSize(UINT32 DevID, UINT32 AnId)
{
    PCPRICTX pCpriCtx;
    UINT32 ret = 0;

    if(pCpriDrvCtx)
    {
        pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];
        ret = pCpriCtx->TxLogBufPos[AnId];
    }

    return ret;
}

UINT32 CPRIDrvGetRxLogSize(UINT32 DevID, UINT32 AnId)
{
    PCPRICTX pCpriCtx;
    UINT32 ret = 0;

    if(pCpriDrvCtx)
    {
        pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];
        ret = pCpriCtx->RxLogBufPos[AnId];
    }

    return ret;
}

UINT32 CPRIDrvGetRxBufPtr(UINT32 DevID)
{
    PCPRICTX pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];

    return (UINT32)pCpriCtx->pRxDmaBuf;
}

UINT32 CPRIDrvGetTxIntCount(UINT32 DevID)
{
    PCPRICTX pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];

    return pCpriCtx->TxIrqNum;
}

UINT32 CPRIDrvGetRxIntCount(UINT32 DevID)
{
    PCPRICTX pCpriCtx;

    if (pCpriDrvCtx != NULL)
    {
        pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];

        return pCpriCtx->RxIrqNum;
    }
    else
        return 0;
}

UINT32 CPRIDrvGetRxLogStatus(UINT32 DevID)
    {
    PCPRICTX pCpriCtx;

    if (pCpriDrvCtx != NULL)
    {
        pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];

        return pCpriCtx->RxLogEnable;
    }
    else
        return 0;
}

UINT32 CPRIDrvGetTxLogStatus(UINT32 DevID)
{
    PCPRICTX pCpriCtx;

    if (pCpriDrvCtx != NULL)
    {
        pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];

        return pCpriCtx->TxLogEnable;
    }
    else
        return 0;
}


/** @brief This function just stops device and does not free any resources
           like allocated memory. It's needed to save allocated resources for
           the futher debugging and do not touch potentially corrupted heaps

    @return [MXRC] an error code
    \ingroup group_lte_hal
    */

MXRC CPRIDrvGenShedding(void)
{
    MXRC rc = CPRI_RC_OK;

    if(pCpriDrvCtx && (MxGetCpuID() == pCpriDrvCtx->CpuID))
    {
        PCPRICTX pCpriCtx;
        UINT32 DevID;

        for (DevID = 0; DevID < CPRI_DEV_NUM; DevID++)
        {
            pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];
            if(pCpriCtx->pCpriRegs != NULL)
            {// used dev
                MxDisableInt(HAL_IRQ_CPRI_0 + DevID);
                MxDisableInt(HAL_IRQ_CPDMA_TX_0 + DevID);
                MxDisableInt(HAL_IRQ_CPDMA_RX_0 + DevID);
                CPRIDrvDisableDma(DevID);
            }
        }
    }

    return rc;
}


UINT32 CPRIDrvWaitForLink(PCPRIREGS pCpriRegs, UINT32 timeout)
{
    UINT32 cpri_stat;
    UINT32 start_tick;
    UINT32 nPulse, nError, nConfCurVal, err_code;
    UINT32 ErrNum = 0;

    /**********************************************
    *                CFG Parameters               *
    ***********************************************/
    UINT32 nPulseTime  = 20*150*1000;   // 20ms MAX time period to get Pulse sent every 10ms
    UINT32 nConfirmNum = 10;            // the number of steps to confirm stable connection(it means time = (10ms puls x nConfirmNum))
    UINT32 nTryNum     = 100;           // The number of attempts to get connection without error
    UINT32 nTryDelay   = 500;           // The delay between next try in ms
    /**********************************************/

    if (timeout)
    {
        timeout = timeout*150000;
        start_tick = GETCURRTICKS();

        // the first step is to wait for connection
        // just connection determinated by 0xE bits

        do
        {
            cpri_stat = pCpriRegs->CPRI_STATUS & 0xE;
        }
        while (cpri_stat != 0xE && (GETCURRTICKS() - start_tick) < timeout);

        if (cpri_stat != 0xE)
        {
            uart_printf ("[NO connection:0x%x]", cpri_stat);
            return 1;
        }

        while (nTryNum != 0)
        {
            nConfCurVal = nConfirmNum;

            // the second step is to check connection
            // to check connection stability and to wait for radio pulses
            // sent every 10ms

            while (nConfCurVal != 0)
            {
                start_tick = GETCURRTICKS();
                nPulse     = 0;
                nError     = 0;
                err_code   = 0;

                while ((GETCURRTICKS() - start_tick) < nPulseTime)
                {
                    cpri_stat = pCpriRegs->CPRI_STATUS;

                    // Bit#00: Receive LOS indication

                    // Bit#08: rx_los hold

                    // Bit#09: rx_state hold

                    // Bit#10: This bit indicates that a CPRI receive clock is not
                    //         synchronous with the system clock.
                    //         The alarm will be asserted every time the phase of
                    //         the receive clock has changed more that 4 clock
                    //         periods of the system clock

                    if (cpri_stat & ((1<<10) | (1<<9) | (1<<8)))
                    {
                        nError = cpri_stat & ((1<<10) | (1<<9) | (1<<8) | (1<<0));
                        break;
                    }

                    // Bit#11: Radio frame pulse received. This bit will be asserted
                    // every 10 ms.

                    if (cpri_stat & (1<<11))
                    {
                        // to wait reset of of this bit
                        while ((pCpriRegs->CPRI_STATUS & (1<<11)) != 0)
                        {
                            _NOP();
                        }

                        nPulse = 1;
                        break;
                    }
                }

                if (nError != 0)
                {
                    uart_printf ("(%03d)Errors: 0x%x detected\r\n", ErrNum++, nError);
                    err_code = 2;
                    break;
                }

                if (nPulse != 1)
                {
                    uart_printf ("(%03d)10 ms pulse is not detected\r\n", ErrNum++);
                    err_code = 3;
                    break;
                }

                nConfCurVal --;
            }

            if (nConfCurVal != 0 || err_code != 0)
            {
                // In this case the attempt was failed,
                // let's try again in some period of time
                // specified by the parameter

                SysDelay(nTryDelay);
                nTryNum --;
                continue;
            }

            // in this case the stability of connnection was confirmed
            // we may return OK

            break;

        }

        return ((pCpriRegs->CPRI_STATUS & 0xE) == 0xE) ? MX_OK : 1;
    }
    else
    {
        while ((pCpriRegs->CPRI_STATUS & 0xE) != 0xE);
        return 0;
    }
}

void CPRIDrvSetCtrlWord(PCPRIREGS pCpriRegs, UINT32 Index, UINT32 Value)
{
    pCpriRegs->CPRI_CTRL_INDEX = Index;
    pCpriRegs->CPRI_TX_CTRL = (1<<8) | (Value & 0xFF); // set value, enable TX CTRL word insertion
}

UINT32 CPRIDrvGetCtrlWord(PCPRIREGS pCpriRegs, UINT32 Index)
{
    pCpriRegs->CPRI_CTRL_INDEX = Index;
    return pCpriRegs->CPRI_RX_CTRL;
}

void CpriEnableDma(PCPRIREGS pCpriRegs, PCPDMAREGS pCpdmaRegs, UINT32 CpriMapConf, UINT32 NumAxCMask)
{
    VUINT32 status_rx, status_tx;
    VUINT32 delay;
    PCPRICTX pCpriCtx;

    pCpriCtx = &pCpriDrvCtx->CpriCtx[0];

    status_rx = pCpdmaRegs->RX_INT_STAT;
    status_tx = pCpdmaRegs->TX_INT_STAT;

    pCpdmaRegs->RX_INT_STAT = status_rx;
    pCpdmaRegs->TX_INT_STAT = status_tx;

    //ICDrvClearIrq(HAL_IRQ_CPDMA_TX_0+0);
    //ICDrvClearIrq(HAL_IRQ_CPDMA_RX_0+0);

    pCpriRegs->CPRI_MAP_CNT_CONFIG = CpriMapConf;
    pCpdmaRegs->TX_DMA_ENA = 1;
    delay = (pCpriCtx->RxStartDelayCounter) ? pCpriCtx->RxStartDelayCounter : CPRI_RX_START_DELAY_COUNTER;
    while (delay--);
    pCpriRegs->CPRI_IQ_RX_BUF_CONTROL = NumAxCMask; //enable RX MAP interface
    pCpdmaRegs->RX_DMA_ENA = 1;
}

/* copied to preserve the same timing of execution as default CpriEnableDma */
void CpriEnableDmaRrh(PCPRIREGS pCpriRegs, PCPDMAREGS pCpdmaRegs, UINT32 CpriMapConf, UINT32 NumAxCMask)
{
    VUINT32 delay;
    PCPRICTX pCpriCtx;

    pCpriCtx = &pCpriDrvCtx->CpriCtx[0];
    pCpriRegs->CPRI_MAP_CNT_CONFIG = CpriMapConf;
    pCpdmaRegs->TX_DMA_ENA = 1;
    delay = (pCpriCtx->RxStartDelayCounter) ? pCpriCtx->RxStartDelayCounter : CPRI_RX_START_DELAY_COUNTER_RRH;
    while (delay--);
    pCpriRegs->CPRI_IQ_RX_BUF_CONTROL = NumAxCMask; //enable RX MAP interface
    pCpdmaRegs->RX_DMA_ENA = 1;
}

void CPRIDrvEnableDma(UINT32 DevID)
{
    PCPRICTX pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];
    PCPRIREGS pCpriRegs = pCpriCtx->pCpriRegs;
    UINT32 pre_bfn;
    VUINT32 cpri_stat;
    IRQTYPE irq;
    UINT32 j, i;

    if (pCpriCtx->CpriConfType == CPRI_EMULATOR)
    {
        IRQTYPE irq = ARM_INT_disable();

        pCpriCtx->MaxRxIrqPeriod = 0;
        pCpriCtx->MaxTxIrqPeriod = 0;

        memset(pCpriCtx->TxLogBufPos, 0, sizeof(pCpriCtx->TxLogBufPos[0]) * pCpriCtx->nTXAntenas);

        for(i = 0; i < pCpriCtx->nTXAntenas; i++)
        {

            pCpriCtx->pEmulTxBuf[i]  = pCpriCtx->pTxLogBuf[i];
        }

        memset(pCpriCtx->RxLogBufPos, 0, sizeof(pCpriCtx->RxLogBufPos[0]) * pCpriCtx->nRXAntenas);

        for(i = 0; i < pCpriCtx->nRXAntenas; i++)
        {
            pCpriCtx->pEmulRxBuf[i]  = pCpriCtx->pRxLogBuf[i];
        }

        TimerDrvSetProc (pCpriDrvCtx->EmulTickTime, 1, CPRIDrvEmulProc, pCpriDrvCtx);

        pCpriCtx->TxIrqTick = pCpriCtx->RxIrqTick = GetTIMETICK();

        ARM_INT_restore (irq);
        return;
    }

    pre_bfn = pCpriRegs->CPRI_BFN;
    for (j = 0; j < 10; j++)
    {
        while (pCpriRegs->CPRI_BFN == pre_bfn);
        pre_bfn = pCpriRegs->CPRI_BFN;
    }

    MxiDisInt(&irq);

    while (pCpriRegs->CPRI_BFN == pre_bfn);

//    pCpriCtx->pCpriRegs->CPRI_MAP_CNT_CONFIG = pCpriCtx->CpriMapConf;
//    pCpriCtx->pCpdmaRegs->TX_DMA_ENA = 1;
//    pCpriCtx->pCpriRegs->CPRI_IQ_RX_BUF_CONTROL = (1<<pCpriCtx->NumAxC)-1; //enable RX MAP interface
//    pCpriCtx->pCpdmaRegs->RX_DMA_ENA = 1;

    if (pCpriCtx->CpriConfType == CPRI_RADIOCOMP)
        CpriEnableDmaRrh(pCpriCtx->pCpriRegs, pCpriCtx->pCpdmaRegs, pCpriCtx->CpriMapConf, (1<<pCpriCtx->NumAxC)-1);
    else
        CpriEnableDma(pCpriCtx->pCpriRegs, pCpriCtx->pCpdmaRegs, pCpriCtx->CpriMapConf, (1<<pCpriCtx->NumAxC)-1);

    // to skip 10 Timer IRQ and to start CPRI in sycn with 
    // the timer IRQs
    TimerDrvRestart();

    pCpriCtx->TxIrqTick = GetTIMETICK();
    pCpriCtx->RxIrqTick = GetTIMETICK();

    MxiRestInt(irq);
}

void CPRIDrvDisableDma(UINT32 DevID)
{
    IRQTYPE irq;
    PCPRICTX pCpriCtx;

    if (pCpriDrvCtx)
    {
        pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];

        if (pCpriCtx->CpriConfType == CPRI_EMULATOR)
        {
            TimerDrvSetProc (0, 0, NULL, NULL);
            return;
        }

        MxiDisInt(&irq);

        pCpriCtx->pCpriRegs->CPRI_MAP_CNT_CONFIG = 0;
        pCpriCtx->pCpdmaRegs->TX_DMA_ENA = 0;
        pCpriCtx->pCpdmaRegs->RX_DMA_ENA = 0;
        pCpriCtx->pCpdmaRegs->TX_AC_ENA = 0;
        pCpriCtx->pCpdmaRegs->RX_AC_ENA = 0;
        pCpriCtx->pCpriRegs->CPRI_CONFIG = 0; // Disable Tx

        MxiRestInt(irq);
    }
}

void CPRIDrvPauseDma(UINT32 DevID)
{
    IRQTYPE irq;
    PCPRICTX pCpriCtx;

    if (pCpriDrvCtx)
    {
        irq = ARM_INT_disable();

        pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];

        if (pCpriCtx->CpriConfType == CPRI_EMULATOR)
        {
            TimerDrvSetProc (0, 0, NULL, NULL);
        }

        MxiRestInt(irq);
    }
    return;
}

void CPRIDrvResumeDma(UINT32 DevID)
{
    IRQTYPE irq;
    PCPRICTX pCpriCtx;

    if (pCpriDrvCtx)
    {
        irq = ARM_INT_disable();

        pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];

        if (pCpriCtx->CpriConfType == CPRI_EMULATOR)
        {
            TimerDrvSetProc (pCpriDrvCtx->EmulTickTime, 1, CPRIDrvEmulProc, pCpriDrvCtx);
        }

        MxiRestInt(irq);
    }
    return;
}


void CPRIDrvSetReMac(PCPRIREGS pCpriRegs, UINT8 *pMac)
{
    UINT32 Value;

    /*
    Z.18.0  RE MAC (3rd Byte)
    Z.19.0  RE MAC (2nd Byte)
    Z.20.0  RE MAC (1st Byte)
    Z.82.0  RE MAC (6th Byte)
    Z.83.0  RE MAC (5th Byte)
    Z.84.0  RE MAC (4th Byte)
     */
    CPRIDrvSetCtrlWord(pCpriRegs, 20, *pMac);
    Value = (UINT32)*pMac++;
    CPRIDrvSetCtrlWord(pCpriRegs, 19, *pMac);
    Value |= (UINT32)*pMac++ << 8;
    CPRIDrvSetCtrlWord(pCpriRegs, 18, *pMac);
    Value |= (UINT32)*pMac++ << 16;
    CPRIDrvSetCtrlWord(pCpriRegs, 84, *pMac);
    Value |= (UINT32)*pMac++ << 24;
    pCpriRegs->ETH_ADDR_LSB = Value;
    CPRIDrvSetCtrlWord(pCpriRegs, 83, *pMac);
    Value = (UINT32)*pMac++;
    CPRIDrvSetCtrlWord(pCpriRegs, 82, *pMac);
    Value |= (UINT32)*pMac++ << 8;
    pCpriRegs->ETH_ADDR_MSB = Value;
}

void CPRIDrvSetReIp(PCPRIREGS pCpriRegs, UINT32 ReIpAddr)
{
    /*
    Z.145.0 RE IP (4th Byte)
    Z.146.0 RE IP (3rd Byte)
    Z.147.0 RE IP (2nd Byte)
    Z.148.0 RE IP (1st Byte)
    */

    CPRIDrvSetCtrlWord(pCpriRegs, 145, ReIpAddr);
    ReIpAddr >>= 8;
    CPRIDrvSetCtrlWord(pCpriRegs, 146, ReIpAddr);
    ReIpAddr >>= 8;
    CPRIDrvSetCtrlWord(pCpriRegs, 147, ReIpAddr);
    ReIpAddr >>= 8;
    CPRIDrvSetCtrlWord(pCpriRegs, 148, ReIpAddr);
    ReIpAddr >>= 8;
 }

void CPRIDrvSetRecIp(PCPRIREGS pCpriRegs, UINT32 RecIpAddr)
{
    /*
    Z.209.0 REC IP (4th Byte)
    Z.210.0 REC IP (3rd Byte)
    Z.211.0 REC IP (2nd Byte)
    Z.212.0 REC IP (1st Byte)
    */

    CPRIDrvSetCtrlWord(pCpriRegs, 209, RecIpAddr);
    RecIpAddr >>= 8;
    CPRIDrvSetCtrlWord(pCpriRegs, 210, RecIpAddr);
    RecIpAddr >>= 8;
    CPRIDrvSetCtrlWord(pCpriRegs, 211, RecIpAddr);
    RecIpAddr >>= 8;
    CPRIDrvSetCtrlWord(pCpriRegs, 212, RecIpAddr);
    RecIpAddr >>= 8;
}

BOOL CPRIDrvEthTxReady(UINT32 DevID){
    PCPRICTX pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];
    if (    (pCpriCtx->pCpriRegs->ETH_CONFIG_1 & CPRI_ETHC1_TXRBIE) &&
            (pCpriCtx->TxEthBufGet == pCpriCtx->TxEthBufPut) )
    {
        return FALSE;
    }
    return TRUE;
}

void CPRIDrvEthTxFrame(UINT32 DevID, LPVOID pTxEthFrame, UINT32 TxFrameSize)
{
    PCPRICTX pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];
    PCPRIREGS pCpriRegs = pCpriCtx->pCpriRegs;

    MxDisableInt(HAL_IRQ_CPRI_0+DevID);

    printf("PRE TX: ETH TX enable 0x%08X\n", *(UINT32*)0xFAC00804);

    pCpriCtx->TxEthBDesc[pCpriCtx->TxEthBufPut].BPtr = pTxEthFrame;
    pCpriCtx->TxEthBDesc[pCpriCtx->TxEthBufPut].BCtrl = TxFrameSize;
    pCpriCtx->TxEthBufPut = (pCpriCtx->TxEthBufPut + 1) & (NUM_CPRI_ETH_TX_BUF-1);

    pCpriRegs->ETH_CONFIG_1 |= CPRI_ETHC1_TXRBIE;

    printf("POST TX: ETH TX enable 0x%08X\n", *(UINT32*)0xFAC00804);

    MxEnableInt(HAL_IRQ_CPRI_0+DevID);
}

BOOL CPRIDrvEthRxReady(UINT32 DevID)
{
    PCPRICTX pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];

    return (pCpriCtx->RxEthBufPut == pCpriCtx->RxEthBufGet) ? FALSE : TRUE;
}

UINT32 CPRIDrvEthRxFrame(UINT32 DevID, LPVOID pRxFrame)
{
    PCPRICTX pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];
    UINT32 RxFrameSize=0;

    MxDisableInt(HAL_IRQ_CPRI_0+DevID);

    if (pCpriCtx->RxEthBufPut != pCpriCtx->RxEthBufGet)
    {
        RxFrameSize = pCpriCtx->RxEthBDesc[pCpriCtx->RxEthBufGet].BCtrl;
        memcpy(pRxFrame, pCpriCtx->RxEthBDesc[pCpriCtx->RxEthBufGet].BPtr, RxFrameSize);
        pCpriCtx->RxEthBDesc[pCpriCtx->RxEthBufGet].BCtrl = 0;
        pCpriCtx->RxEthBufGet = (pCpriCtx->RxEthBufGet + 1) & (NUM_CPRI_ETH_RX_BUF-1);
    }

    MxEnableInt(HAL_IRQ_CPRI_0+DevID);

    return RxFrameSize;
}

UINT32 CPRIDrvGetActiveBufOffset(UINT32 DevID)
{
    PCPRICTX pCpriCtx = &pCpriDrvCtx->CpriCtx[DevID];

    return (pCpriCtx->pCpdmaRegs->RX_AC_PTR[0] < (pCpriCtx->DmaIntSize/8)) ? 0 : pCpriCtx->DmaIntSize;
}

/** @brief This function is designed to set CPRI HW loopback (TX->RX)

    @param nDev [in] - the CPRI dev ID [0..5]
    @param bLB  [in] - LB mode: 1 - set LB, 0 - reset LB

    @return [MXRC] an error code */

MXRC CPRIDrvSetLB (UINT32 nDev, BOOL bLB)
{
    if (bLB)
        REG32(RAD_CFG_CPRI_MODE_REG) |= (1 << nDev);
    else
        REG32(RAD_CFG_CPRI_MODE_REG) &= (1 << nDev);

    return MX_OK;
}

#ifdef ICPU_FORWARDING

APIHEC CPRIApiHandler (LPVOID pData, ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * p;
    //uart_printf("CPRI: header %u, fw to CPRI\n", pApi->Length);
    if( (p = ApiGetParam(pApi, 0)) )
    {
        //ApiAddParam(pResp, NULL, p->ParamID, p->ParamData, p->ParamLength);
//redirect to cpri
        if ( p->ParamLength )
        {
//pTxEthFrame buffer has to be encreased in NUM_CPRI_ETH_TX_BUF once
            if( CPRIDrvEthTxReady(0) ){
                //uart_printf("CPRI: got UARM msg, prmlen %u, fw to CPRI\n", p->ParamLength);
                memcpy(&pTxEthFrame[TxEthFrameInd*MAX_ICPU_COMMAND_SIZE], p->ParamData, p->ParamLength);
                CPRIDrvEthTxFrame(0, &pTxEthFrame[TxEthFrameInd*MAX_ICPU_COMMAND_SIZE], p->ParamLength);
                TxEthFrameInd = (TxEthFrameInd+1) & (NUM_CPRI_ETH_TX_BUF-1);
                return APIHEC_OK;
            }
            //uart_printf("CPRI: Queue is full\n");
        }
    }
    //uart_printf("CPRI: UARM msg ignore\n");
    return APIHEC_DROP_RESP;
}

MXRC ICPURxChain(LPVOID data, UINT32 len)
{
    MXRC rc = CPRI_RC_OK;
    PApiHeader pApi;

    //uart_printf("CPRI: CPRI msg len %u, fw to UARM\n", len);
    pApi = ApiCreate(   pRxEthFrame,
                        0x10000000,
                        31,
                        1,
                        2,
                        0 );

    ApiAddParam (   pApi,
                    (ApiParam *)(pApi+1),
                    3,
                    data,
                    len );

    rc = SvsrSendAPI(pApi);

    _ASSERT_RC(rc);

    return rc;
}
#endif

void CPRIDrvCfgProc(PCPRICONFIG CpriCfg)
{
    PCPRIDRVCTX pCpriCtx = CPRIDrvGetCtx();
    PCPRICTX pCpriDevCtx = NULL;
    UINT32 TxIrqGpioSet = 0;
    UINT32 RxIrqGpioSet = 0;
    UINT32 DevID = 0;
    IRQTYPE irq = 0;

    if(pCpriCtx && pCpriCtx->DevEnaMask)
        irq = ARM_INT_disable ();

    while(DevID < CPRI_DEV_NUM)
    {
        if(pCpriCtx)
            pCpriDevCtx = &pCpriCtx->CpriCtx[DevID];

        if(CpriCfg->TxIrqGpioEna & (1 << DevID))
        {// to enable
            if (TxIrqGpioSet == 0)
            {
                if(pCpriDevCtx)
                {
                    pCpriDevCtx->TxIrqGpioPort = CpriCfg->TxIrqGpioPort;
                    pCpriDevCtx->TxIrqGpioEna = 1;
                    pCpriDevCtx->TxIrqGpioToggleDelays[0] = CpriCfg->TxIrqGpioToggleDelays >> 16;
                    if(pCpriDevCtx->TxIrqGpioToggleDelays[0] == 0)
                        pCpriDevCtx->TxIrqGpioToggleDelays[0] = 1;
                    pCpriDevCtx->TxIrqGpioToggleDelays[1] = CpriCfg->TxIrqGpioToggleDelays & 0xFFFF;
                    if(pCpriDevCtx->TxIrqGpioToggleDelays[1] == 0)
                        pCpriDevCtx->TxIrqGpioToggleDelays[1] = 1;
                    pCpriDevCtx->TxIrqGpioToggleDelayIdx = 0;
                    pCpriDevCtx->TxIrqGpioToggleCntr = 0;
                }
                REG32(GPIO_PIN_SELECT_REG) |= (1 << CpriCfg->TxIrqGpioPort); // GPIO functionality
                REG32(GPIO_OE_REG) |= (1 << CpriCfg->TxIrqGpioPort); // enable
                REG32(GPIO_OUTPUT_REG) &= ~(1 << CpriCfg->TxIrqGpioPort); // low at start
                TxIrqGpioSet = 1;
            }
        }
        else if(pCpriDevCtx && pCpriDevCtx->TxIrqGpioEna)
        {// to disable
            REG32(GPIO_OE_REG) &= ~(1 << pCpriDevCtx->TxIrqGpioPort); // disable
            pCpriDevCtx->TxIrqGpioEna = 0;
        }

        if(CpriCfg->RxIrqGpioEna & (1 << DevID))
        {// to enable
            if (RxIrqGpioSet == 0)
            {
                if(pCpriDevCtx)
                {
                    pCpriDevCtx->RxIrqGpioPort = CpriCfg->RxIrqGpioPort;
                    pCpriDevCtx->RxIrqGpioEna = 1;
                    pCpriDevCtx->RxIrqGpioToggleDelays[0] = CpriCfg->RxIrqGpioToggleDelays >> 16;
                    if(pCpriDevCtx->RxIrqGpioToggleDelays[0] == 0)
                        pCpriDevCtx->RxIrqGpioToggleDelays[0] = 1;
                    pCpriDevCtx->RxIrqGpioToggleDelays[1] = CpriCfg->RxIrqGpioToggleDelays & 0xFFFF;
                    if(pCpriDevCtx->RxIrqGpioToggleDelays[1] == 0)
                        pCpriDevCtx->RxIrqGpioToggleDelays[1] = 1;
                    pCpriDevCtx->RxIrqGpioToggleDelayIdx = 0;
                    pCpriDevCtx->RxIrqGpioToggleCntr = 0;
                }
                REG32(GPIO_PIN_SELECT_REG) |= (1 << CpriCfg->RxIrqGpioPort); // GPIO functionality
                REG32(GPIO_OE_REG) |= (1 << CpriCfg->RxIrqGpioPort); // enable
                REG32(GPIO_OUTPUT_REG) &= ~(1 << CpriCfg->RxIrqGpioPort); // low at start
                RxIrqGpioSet = 1;
            }
        }
        else if(pCpriDevCtx && pCpriDevCtx->RxIrqGpioEna)
        {// to disable
            REG32(GPIO_OE_REG) &= ~(1 << pCpriDevCtx->RxIrqGpioPort); // disable
            pCpriDevCtx->RxIrqGpioEna = 0;
        }

        DevID++;
    }

    memcpy(&CpriApiConfig, CpriCfg, sizeof(CpriApiConfig));
    CpriApiConfigSet = 1;

    if(pCpriCtx && pCpriCtx->DevEnaMask)
        ARM_INT_restore (irq);
}

MXRC CPRIDrvCfgApi(ApiHeader *pApi, ApiHeader *pResp)
{
    ApiParam * pParam;
    MXRC rc = MX_OK;
    CPRICONFIG CpriCfg;

    memset(&CpriCfg, 0, sizeof(CpriCfg));

    switch(pApi->MessageID)
    {
        case PHY_CPRI_GPIO_CTRL:
            pParam = ApiGetNextParam(pApi, NULL);
            while (pParam)
            {
                switch (pParam->ParamID)
                {
                    case PAR_CPRI_GPIO_TXIRQ_ENA:
                        CpriCfg.TxIrqGpioEna = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_GPIO_TXIRQ_PORT:
                        CpriCfg.TxIrqGpioPort = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_GPIO_TXIRQ_TOGGLE_DELAYS:
                        CpriCfg.TxIrqGpioToggleDelays = pParam->ParamData[0];
                        break;

                    case PAR_CPRI_GPIO_RXIRQ_ENA:
                        CpriCfg.RxIrqGpioEna = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_GPIO_RXIRQ_PORT:
                        CpriCfg.RxIrqGpioPort = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_GPIO_RXIRQ_TOGGLE_DELAYS:
                        CpriCfg.RxIrqGpioToggleDelays = pParam->ParamData[0];
                        break;

                    default:
                        uart_printf("CPRICfg: unknown parameter ID\r\n", pParam->ParamID);
                        break;
                }
                pParam = ApiGetNextParam(pApi, pParam);
            }
            break;
        case PHY_CPRI_CONFIG:
            pParam = ApiGetNextParam(pApi, NULL);
            while (pParam)
            {
                switch (pParam->ParamID)
                {
                    case PAR_CPRI_CONF_NUM_CPRI_DEV:
                        CpriConfigByHost.NumCpriDev = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_SAMPLING_RATE:
                        CpriConfigByHost.SamplingRate = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_SAMPLE_WIDTH:
                        CpriConfigByHost.SampleWidth = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_REC_MAC_ADDR0:
                        CpriConfigByHost.RecMacAddr[0] = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_REC_MAC_ADDR1:
                        CpriConfigByHost.RecMacAddr[1] = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_REC_MAC_ADDR2:
                        CpriConfigByHost.RecMacAddr[2] = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_REC_MAC_ADDR3:
                        CpriConfigByHost.RecMacAddr[3] = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_REC_MAC_ADDR4:
                        CpriConfigByHost.RecMacAddr[4] = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_REC_MAC_ADDR5:
                        CpriConfigByHost.RecMacAddr[5] = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_RE_MAC_ADDR5:
                        CpriConfigByHost.ReMacAddr[5] = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_REC_IP_ADDR:
                        CpriConfigByHost.RecIpAddr = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_RE_IP_ADDR:
                        CpriConfigByHost.ReIpAddr = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_ETH_ENABLE:
                        CpriConfigByHost.EthEnable = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_ETH_LITTLE_ENDIAN:
                        CpriConfigByHost.EthLittleEndian = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_ETH_FCS_ENABLE:
                        CpriConfigByHost.EthFcsEnable = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_ETH_RATE:
                        CpriConfigByHost.EthRate = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_HDLC_ENABLE:
                        CpriConfigByHost.HdlcEnable = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_MAX_AC_NUM:
                        CpriConfigByHost.MaxAcNum = pParam->ParamData[0];
                        break;
                    case PAR_CPRI_CONF_CPRI_RATE:
                        CpriConfigByHost.CpriRate = pParam->ParamData[0];
                        break;
                    default :
                        uart_printf("CPRICfg: unknown parameter ID\r\n", pParam->ParamID);
                        break;
                }
                pParam = ApiGetNextParam(pApi, pParam);
            }
            bCpriConfigByHost = 1;
            break;

        case PHY_CPRI_SFP_CTRL:
        {
            bSftCtrlEnabled = 1;
            SfpSrdsID = ApiGetParamU32 (pApi, 1, 1);
            SfpMask = ApiGetParamU32 (pApi, 2, 3);

            break;
        }

        default:
            uart_printf("CPRICfg: unknown message ID\r\n", pApi->MessageID);
            break;
    }

    CPRIDrvCfgProc(&CpriCfg);

    return rc;
}

void CpriMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    if (nCpuID == 0)
    {
        MmuMgrCfgMemAP(RAD_APB_CONFIG_BASEADDR, 4*1024,     AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(RAD_APB_CLKRST_BASEADDR, 4*1024,     AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(RAD_CLUSTER_CNTRL,       4*1024,     AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(CPRI_BASEADDR,           1024*1024,  AP_MGR_READ_WRITE, AP_NO_ACCESS);
        MmuMgrCfgMemAP(RAD_APB_CPDMA_BASEADDR,  50*1024,    AP_MGR_READ_WRITE, AP_NO_ACCESS);
    }
}


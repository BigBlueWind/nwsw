//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL hbidrv.c
 *
 * @brief Host Bus Interface
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "hbidrv.h"
#include "config.h"
#include <string.h>
#include "mmumgr.h"

HBIDRVCTX HbiCtx;

#ifdef HBI_STAT_ENABLED

UINT32    StatTxNum;
UINT32    StatRxNum;
UINT32    StatTxTotalSize;
UINT32    StatRxTotalSize;

#endif

HBIDRVCTX * HBIDrvGetCtx (void)
{
     return &HbiCtx;
}

MXRC HBIDrvSetRxQueueHandler(HANDLE RxQueueHandler)
{
    HBIDRVCTX * pCtx = HBIDrvGetCtx();
    MXRC rc = HBI_RC_OK;

    pCtx->hThr = RxQueueHandler;

    return rc;
}

LPVOID HBIDrvAllocTxPart(void)
{
    HBIDRVCTX * pCtx = HBIDrvGetCtx();

    return MxAllocBlock(pCtx->TxP);
}

MXRC HBIDrvFreeRxPart(LPVOID pData)
{
    HBIDRVCTX * pCtx = HBIDrvGetCtx();

    return MxFreeBlock(pCtx->RxP, pData);
}

/** @brief The handler processes HBI IRQ interrupts

 @param pClear [in] - the pointer to interrupt clear register
 @param pParam [in] - the ISR parameter */

void HbiIrqHandler(volatile UINT32 * pClear, UINT32 nIrq, LPVOID pParam)
{
    HBIDRVCTX * pCtx = (HBIDRVCTX *)pParam;

    LPVOID pData = NULL;
    UINT32 nSize = 0;

    pData = MxAllocBlock (pCtx->RxP);

    if ((HBIDrvRecvMsg(pData, &nSize) == HBI_RC_OK) && HANDLE_NOT_NULL(pCtx->hThr))
    {
        MxPutQueueData (pCtx->RxQ, (LPVOID)MxPartPointerToIndex (pCtx->RxP, pData));

	// To schedule the supervisor API handler

        MxScheduleThread (pCtx->hThr);
    }
    else
    {
        MxFreeBlock (pCtx->RxP, pData);
    }

    *pClear = nIrq;
}


MXRC HBIDrv_init(UINT32 nInit, LPVOID * pDrvCtx)
{
    MXRC rc = HBI_RC_OK;

    HBIDRVCTX * pCtx = HBIDrvGetCtx();

    memset (pCtx, 0, sizeof (pCtx));

#ifdef HBI_STAT_ENABLED

    StatTxNum = 0;
    StatRxNum = 0;
    StatTxTotalSize = 0;
    StatRxTotalSize = 0;

#endif

    rc = MxCreateQueue (pCtx->RxQStor,
                        sizeof (pCtx->RxQStor)/sizeof (pCtx->RxQStor[0]),
                        sizeof(pCtx->RxQStor[0]),
                        NULL, HNULL, &pCtx->RxQ);

    if (FAILED(rc))
        return rc;

    rc = MxCreateQueue (pCtx->TxQStor,
                        sizeof (pCtx->TxQStor)/sizeof (pCtx->TxQStor[0]),
                        sizeof(pCtx->TxQStor[0]),
                        NULL, HNULL, &pCtx->TxQ);

    if (FAILED(rc))
        return rc;

    rc = MxCreatePart (pCtx->TxPStor,
                       sizeof(pCtx->TxPStor)/sizeof(pCtx->TxPStor[0]),
                       sizeof(pCtx->TxPStor[0]),
                       NULL,
                       HNULL,
                       HNULL,
                       &pCtx->TxP);

    if (FAILED(rc))
        return rc;

    rc = MxCreatePart (pCtx->RxPStor,
                       sizeof(pCtx->RxPStor)/sizeof(pCtx->RxPStor[0]),
                       sizeof(pCtx->RxPStor[0]),
                       NULL,
                       HNULL,
                       HNULL,
                       &pCtx->RxP);

    if (FAILED(rc))
        return rc;

    // To enable HBI interface
    REG32(XP_AXI_RESET_0) = 0;

    if (FAILED(rc = MxSetIntHandler(HAL_IRQ_HBI, INT_TYPE_IRQ, PRI_HBI, HbiIrqHandler, pCtx)))
        return rc;

    if (FAILED(rc = MxEnableInt(HAL_IRQ_HBI)))
        return rc;

    REG32(HBI_INT_ENABLE_REG) |= TXM3IE;

    REG32(HBI_INT_ACKNOWLEDGE_REG) = TXM3IAK;

    pCtx->cpuID = MxGetCpuID();

    return HBI_RC_OK;
}

MXRC HBIDrvRecvMsg(PUINT32 pU32, PUINT32 pDataLen)
{
    UINT32 u32Len;
    UINT32 FifoSize;

    if (REG32(HBI_INT_STATUS_REG) & TXM3I)
    {
        *pDataLen = REG32(HBI_TX_MAIL2_REG);

#ifdef HBI_STAT_ENABLED

        StatRxNum ++;
        StatRxTotalSize += *pDataLen;

#endif


        // To convert to 32Bits items

        u32Len = ((*pDataLen) + 3) / sizeof(UINT32);

        while (u32Len)
        {
            FifoSize = REG32(HBI_TX_FIFO_SIZE) >> 2;
            if (FifoSize)
            {
                u32Len -= FifoSize;
                while (FifoSize--)
                    *pU32++ = REG32(HBI_TX_FIFO_DATA_REG_32);
            }
        }

        REG32(HBI_INT_ACKNOWLEDGE_REG) = TXM3IAK;

        return HBI_RC_OK;
    }

    return HBI_RC_EMPTY;
}

MXRC HBIDrvXmitMsg(LPVOID pData, UINT32 DataLen)
{
    PUINT32 pU32 = (UINT32 *)pData;
    UINT32 u32Len;
    UINT32 nCopy;
    UINT32 FifoSize;
    IRQTYPE irq;
    UINT32 timeout = HBI_TIMEOUT;

    // The real size of API commands should be calculated here
    DataLen = (((PUINT32)pData) [1] + 16);

#ifdef HBI_STAT_ENABLED

    StatTxNum ++;
    StatTxTotalSize += DataLen;

#endif

    MxiDisInt(&irq);

    DataLen = ROUND(DataLen, sizeof(UINT32));
    u32Len = DataLen / sizeof (UINT32);

    nCopy = (u32Len > HBI_FIFO_MAX_SIZE_32W) ? HBI_FIFO_MAX_SIZE_32W : u32Len;
    u32Len -= nCopy;
    while (nCopy--)
        REG32(HBI_RX_FIFO_DATA_REG_32) = *pU32++;

    REG32(HBI_RX_MAIL0_REG) = 0;
    REG32(HBI_RX_MAIL1_REG) = 0;
    REG32(HBI_RX_MAIL2_REG) = DataLen;
    REG32(HBI_RX_MAIL3_REG) = ((PUINT16)pData)[7]; // Message ID

    while (u32Len)
    {
        FifoSize = REG32(HBI_RX_FIFO_SIZE) >> 2;
        if (FifoSize < HBI_FIFO_MAX_SIZE_32W)
        {
            REG32(HBI_RX_FIFO_DATA_REG_32) = *pU32++;
            u32Len--;
        }
    }

    MxiRestInt(irq);

    MxFreeBlock (HbiCtx.TxP, pData);

    // wait till previous message is processed
    while ((REG32(HBI_INT_STATUS_REG) & RXM3I) == 0 && timeout--);

    if ((REG32(HBI_INT_STATUS_REG) & RXM3I) == 0)
        return HBI_RC_TXERROR;

    REG32(HBI_INT_ACKNOWLEDGE_REG) = RXM3IAK;

    return HBI_RC_OK;
}

MXRC HBIShareDataObjects (void)
{
    HBIDRVCTX * pCtx = HBIDrvGetCtx ();

    MXRC rc = HBI_RC_OK;

    if (HANDLE_NOT_NULL(pCtx->RxP))
    {
        if (FAILED(rc = MxShareObject (pCtx->RxP, OBJ_SHARE_ON)))
            return rc;
    }

    if (HANDLE_NOT_NULL(pCtx->TxP))
    {
        if (FAILED(rc = MxShareObject (pCtx->TxP, OBJ_SHARE_ON)))
            return rc;
    }

    if (HANDLE_NOT_NULL(pCtx->RxQ))
    {
        if (FAILED(rc = MxShareObject (pCtx->RxQ, OBJ_SHARE_ON)))
            return rc;
    }

    if (HANDLE_NOT_NULL(pCtx->TxQ))
    {
        if (FAILED(rc = MxShareObject (pCtx->TxQ, OBJ_SHARE_ON)))
            return rc;
    }

    return rc;

}

MXRC HBIUnShareDataObjects (void)
{
    HBIDRVCTX * pCtx = HBIDrvGetCtx ();

    MXRC rc = HBI_RC_OK;

    if (HANDLE_NOT_NULL(pCtx->RxP))
    {
        rc = MxShareObject (pCtx->RxP, OBJ_SHARE_OFF);
    }

    if (HANDLE_NOT_NULL(pCtx->TxP))
    {
        MXRC rc1 = MxShareObject (pCtx->TxP, OBJ_SHARE_OFF);
        if(rc == MX_OK)
            rc = rc1;
    }

    if (HANDLE_NOT_NULL(pCtx->RxQ))
    {
        MXRC rc1 = MxShareObject (pCtx->RxQ, OBJ_SHARE_OFF);
        if(rc == MX_OK)
            rc = rc1;
    }

    if (HANDLE_NOT_NULL(pCtx->TxQ))
    {
        MXRC rc1 = MxShareObject (pCtx->TxQ, OBJ_SHARE_OFF);
        if(rc == MX_OK)
            rc = rc1;
    }

    return rc;
}

MXRC HBIDrvShutdown(void)
{
    HBIDRVCTX * pCtx = HBIDrvGetCtx();

    if(pCtx->cpuID == MxGetCpuID())
    {
        REG32(HBI_INT_ENABLE_REG) &= ~TXM3IE;

        MxDisableInt(HAL_IRQ_HBI);

        HBIUnShareDataObjects();

        if(HANDLE_NOT_NULL(pCtx->RxP))
        {
            MxRemovePart(pCtx->RxP);
        }

        if(HANDLE_NOT_NULL(pCtx->TxP))
        {
            MxRemovePart(pCtx->TxP);
        }

        if(HANDLE_NOT_NULL(pCtx->RxQ))
        {
            MxRemoveQueue(pCtx->RxQ);
        }

        if(HANDLE_NOT_NULL(pCtx->TxQ))
        {
            MxRemoveQueue(pCtx->TxQ);
        }

        memset (pCtx, 0, sizeof (pCtx));
    }

    return HBI_RC_OK;
}

void HBIMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    MmuMgrCfgMemAP(HBI_APB_BASEADDR, 100*1024, AP_MGR_READ_WRITE, AP_MGR_READ_ONLY);
}


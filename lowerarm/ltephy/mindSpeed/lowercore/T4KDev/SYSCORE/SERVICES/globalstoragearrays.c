//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Globalstoragearrays.c
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.24 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#define GLOBAL_STORAGE_ARRAY_CFILE
#include "config.h"
#include "4gmx.h"
#include "heaps.h"
#include "globalstoragearrays.h"
#include "appprintf.h"
#include "supervisor.h"
#include "mlog.h"
#include "svsrlogger.h"

GLOBALSTORAGEARRAYSCTX GlobalStorageArrayCtx;

PGLOBALSTORAGEARRAYSCTX GlobalStorageArrayGetCtx()
{
    return &GlobalStorageArrayCtx;
}



APIHEC GSApiHandler (LPVOID pCtx, ApiHeader * pApi, ApiHeader * pResp)
{
    MXRC rc = GS_RC_UNKNOWN_API;
    UINT32 tmp, tmp1;
    PGLOBALSTORAGEARRAYSCTX pAnntCtx = GlobalStorageArrayGetCtx();

    if (IS_QRY_API(pApi, INFORMER_GET_OBJ_INFO))
    {
        ApiParam * pParam = ApiGetParamByID (pApi, 2);

        if (pParam != NULL)
        {
            rc = MX_OK;

            switch (pParam->ParamData [0])
            {
                case GS_PARAM_DL_INFO:

                    if (pAnntCtx != NULL && pAnntCtx->nTxAnt != 0)
                    {
                        tmp = DL_IQ_SAMPLES_BUF_SIZE * sizeof(SINT16) / pAnntCtx->nTxAnt;
                    }
                    else
                    {
                        tmp = DL_IQ_SAMPLES_BUF_SIZE * sizeof(SINT16) / 2;
                    }

                    ApiAddParam (pResp, NULL, GS_INFO_OBJ_BASE, &gDlIqBuf, sizeof (gDlIqBuf));
                    ApiAddParam (pResp, NULL, GS_INFO_OBJ_SIZE, &tmp, sizeof (tmp));
                    break;

                case GS_PARAM_UL_INFO:

                    tmp = UL_IQ_SAMPLES_BUF_SIZE * sizeof(SINT16);

                    ApiAddParam (pResp, NULL, GS_INFO_OBJ_BASE, &gUlIqBuf, sizeof (gUlIqBuf));
                    ApiAddParam (pResp, NULL, GS_INFO_OBJ_SIZE, &tmp, sizeof (tmp));
                    break;


                 case GS_PARAM_DL_SDU_INFO:

                    tmp = DL_SDU_BUF_SIZE;

                    ApiAddParam (pResp, NULL, GS_INFO_OBJ_BASE, &gDlSduBuf, sizeof (gDlSduBuf));
                    ApiAddParam (pResp, NULL, GS_INFO_OBJ_SIZE, &tmp, sizeof (tmp));
                    break;

                 case GS_PARAM_UL_SDU_INFO:

                    tmp = UL_SDU_BUF_SIZE;

                    ApiAddParam (pResp, NULL, GS_INFO_OBJ_BASE, &gUlSduBuf, sizeof (gUlSduBuf));
                    ApiAddParam (pResp, NULL, GS_INFO_OBJ_SIZE, &tmp, sizeof (tmp));
                    break;

                 case GS_PARAM_MLOG_INFO:

                    tmp1= (UINT32)MLogGetFileLocation();
                    tmp = MLogGetFileSize();

                    ApiAddParam (pResp, NULL, GS_INFO_OBJ_BASE, &tmp1, sizeof (tmp1));
                    ApiAddParam (pResp, NULL, GS_INFO_OBJ_SIZE, &tmp, sizeof (tmp));
                    break;

                 case GS_PARAM_API_TRACE_INFO:

                    tmp1 = (UINT32)SvsrLoggerGetLocation();
                    tmp  = SvsrLoggerGetSize();

                    ApiAddParam (pResp, NULL, GS_INFO_OBJ_BASE, &tmp1, sizeof (tmp1));
                    ApiAddParam (pResp, NULL, GS_INFO_OBJ_SIZE, &tmp, sizeof (tmp));
                    break;

                 default:
                    rc = GS_RC_PARAM_ERROR;
                    break;

            }
        }
    }

    ApiAddErrorCode (pResp, rc);

    return APIHEC_OK;
}

MXRC InitializeStorageArrays(void)
{
    MXRC rc = 0;

    rc = SvsrRegClient (IID_INFORMER, GSApiHandler, NULL);

    if (FAILED(rc))
    {
        uart_printf ("Global-Storage: informer init error, rc=%x\r\n", rc);
        return rc;
    }

    gDlIqBuf    =(PSINT16)MxHeapAlloc(DDR1NcnbHeap, DL_IQ_SAMPLES_BUF_SIZE * sizeof(SINT16));
    gUlIqBuf    =(PSINT16)MxHeapAlloc(DDR1NcnbHeap, UL_IQ_SAMPLES_BUF_SIZE * sizeof(SINT16));
    gDlSduBuf   =(PSINT8)MxHeapAlloc(DDR1NcnbHeap, DL_SDU_BUF_SIZE);
    gUlSduBuf   =(PSINT8)MxHeapAlloc(DDR1NcnbHeap, UL_SDU_BUF_SIZE);
    PackHeapPtr =(PUINT8)MxHeapAlloc(DDR1NcnbHeap, PACK_HEAP_INFO_BUF);

    rc = LogMgrCreateLogEx ((LOG_CTRL_SPU_MDMA | LOG_CTRL_SYNCPU), gDlSduBuf , DL_SDU_BUF_SIZE, &hLogDlIQ);
    _ASSERT_RC(rc);

    if (FAILED(rc))
        return rc;

    rc = LogMgrCreateLogEx ((LOG_CTRL_SPU_MDMA | LOG_CTRL_SYNCPU), gUlSduBuf , UL_SDU_BUF_SIZE, &hLogUlIQ);
    _ASSERT_RC(rc);

    if (FAILED(rc))
        return rc;

    // to allocate in cachable heap
    if (HANDLE_NOT_NULL(DDR1Heap))
    {
        // this temporary allocation to prevent API log to ba allocated
        // at the very beginning of DDR1 where DDR init code in the uboot can overwrite it.
        // as result API trace won't be corrupted by uboot after reset
        LPVOID pTmp = MxHeapAlloc(DDR1Heap, 128*1024);

        gSvsrLoggerBuf = MxHeapAlloc(DDR1Heap, SVSR_API_LOGGER_BUF_SIZE);
        gMlogBuf = MxHeapAlloc(DDR1Heap /*DDR1NcnbHeap*/, MLOG_BUF_SIZE);

        // return memory back to heap as it can be allocated for something else
        MxHeapFree(DDR1Heap, pTmp);
    }
    else
    {
        gSvsrLoggerBuf = MxHeapAlloc(DDR1NcnbHeap, SVSR_API_LOGGER_BUF_SIZE);
        gMlogBuf = MxHeapAlloc(DDR1NcnbHeap, MLOG_BUF_SIZE);
    }

    if (MLOG_DEVICE_COUNT != 0)
    {
        MLogDevPtr = MxHeapAlloc(DDR1NcnbHeap, MLOG_DEVICE_COUNT * (MLOG_FRAME_LIMIT * MLOG_FRAME_SIZE));
    }
    else
    {
        MLogDevPtr = NULL;
    }

    //gMlogBuf = MxHeapAlloc(DDR1NcnbHeap, MLOG_BUF_SIZE);

/*
    uart_printf("\n\n--------------------------------------------------------\n\r\n");
    uart_printf("Initializing Storage Arrays:\r\n");
    uart_printf("----------------------------\r\n");
    uart_printf("Location of gDlIqBuf:       0x%08x\r\n", gDlIqBuf);
    uart_printf("Location of gUlIqBuf:       0x%08x\r\n", gUlIqBuf);
    uart_printf("Location of gDlSduBuf:      0x%08x\r\n", gDlSduBuf);
    uart_printf("Location of gUlSduBuf:      0x%08x\r\n", gUlSduBuf);
    uart_printf("Location of gSvsrLoggerBuf: 0x%08x\r\n", gSvsrLoggerBuf);
    uart_printf("Location of gMlogBuf:       0x%08x\r\n", gMlogBuf);
    uart_printf("Location of videoFileData:  0x%08x\r\n", videoFileData);
    uart_printf("\n--------------------------------------------------------\r\n");
*/
    return MX_OK;
}

void DestroyStorageArrays(void)
{

    uart_printf("\n\n--------------------------------------------------------\n\r\n");
    uart_printf("Destroying Storage Arrays:\r\n");
    MxHeapFree(DDR1NcnbHeap, gDlIqBuf);
    MxHeapFree(DDR1NcnbHeap, gUlIqBuf);
    MxHeapFree(DDR1NcnbHeap, gDlSduBuf);
    MxHeapFree(DDR1NcnbHeap, gUlSduBuf);

    if (HANDLE_NOT_NULL(DDR1Heap))
    {
        MxHeapFree(DDR1Heap, gSvsrLoggerBuf);
        MxHeapFree(DDR1Heap, gMlogBuf);
    }
    else
    {
        MxHeapFree(DDR1NcnbHeap, gSvsrLoggerBuf);
        MxHeapFree(DDR1NcnbHeap, gMlogBuf);
    }

    uart_printf("\n--------------------------------------------------------\r\n");
    return;
}

MXRC InitializeCramIqBufs(UINT32 nTxAnt, UINT32 nRxAnt, UINT32 bufSizePerAnt)
{
    PGLOBALSTORAGEARRAYSCTX pCtx = GlobalStorageArrayGetCtx();
    PUINT8 pDlBuf, pUlBuf;
    UINT32 i, j;

    pCtx->bufSizePerAnt = bufSizePerAnt;
    pCtx->nTxAnt = nTxAnt;
    pCtx->nRxAnt = nRxAnt;

    pCtx->pDlUlIqBuf = (PUINT8)MxHeapAllocAt(DspHeap, CPRI_DMA_BUF_ADDR , CPRI_DMA_BUF_SIZE + CPRI_RXDMA_BUF_SIZE + 32); // for 2x TX and RX
    if (pCtx->pDlUlIqBuf == NULL)
    {
        uart_printf("InitializeCramIqBufs: Could not allocate Memory\r\n");
        return GS_RC_ALLOC_ERROR;
    }

    pDlBuf = pCtx->pDlUlIqBuf;
    pUlBuf = pCtx->pDlUlIqBuf + CPRI_DMA_BUF_SIZE;        // Also pointing to CPRI_RXDMA_BUF_ADDR

    for(i = 0; i < pCtx->nTxAnt; i++)
    {
        for(j = 0; j < 2; j++)
        {
            pCtx->pDlIqCramBuf[i][j] = &pDlBuf[(pCtx->bufSizePerAnt * j) + (i * pCtx->bufSizePerAnt*2)];
        }
    }

    for(i = 0; i < pCtx->nRxAnt; i++)
    {
        for(j = 0; j < 2; j++)
        {
            pCtx->pUlIqCramBuf[i][j] = &pUlBuf[(pCtx->bufSizePerAnt * j) + (i * pCtx->bufSizePerAnt*2)];
        }
    }

    return MX_OK;
}

MXRC DestroyCramIqBufs(void)
{
    PGLOBALSTORAGEARRAYSCTX pCtx = GlobalStorageArrayGetCtx();
    _ASSERT_PTR(pCtx);

    MxHeapFree(DspHeap, pCtx->pDlUlIqBuf);

    memset(pCtx, 0, sizeof(GLOBALSTORAGEARRAYSCTX));

    return MX_OK;
}


PUINT8 QueryCramIqBuf(UINT32 direction, UINT32 context, UINT32 antId)
{
    PUINT8 pBuf;
    PGLOBALSTORAGEARRAYSCTX pCtx = GlobalStorageArrayGetCtx();

    if (direction == 0)     // DL
    {
        pBuf = pCtx->pDlIqCramBuf[antId][context];
    }
    else if(direction == 1) // UL
    {
        pBuf = pCtx->pUlIqCramBuf[antId][context];
    }
    else
    {
        pBuf = NULL;
    }

    return pBuf;
}


UINT32 GetStorageCapacity(UINT32 fftSize, UINT32 numAntennas)
{
    UINT32 numSubframes, iqMax;

    iqMax = (DL_IQ_SAMPLES_BUF_SIZE > UL_IQ_SAMPLES_BUF_SIZE) ? UL_IQ_SAMPLES_BUF_SIZE : DL_IQ_SAMPLES_BUF_SIZE;

    numSubframes = iqMax / (fftSize * 15 * 2 * 2 * numAntennas);

    return numSubframes;
}



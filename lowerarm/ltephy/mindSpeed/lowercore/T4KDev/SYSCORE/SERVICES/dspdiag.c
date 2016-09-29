//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL dspdiag.c
 *
 * @brief The DspDiag API module
 * @author Mindspeed Technologies
 * @version $Revision: 1.20 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "dspdiag.h"
#include "heaps.h"
#include <string.h>
#include "appprintf.h"
#include "diagnostics.h"
#include "timerdrv.h"
#include "icdrv.h"
#include "DSPdrv.h"
#include "devinfo.h"

typedef struct _CEVADUMP_
{
    UINT32  CevaID;         // It's used in the response
    PUINT8  pDumpBuf;       // The pointer to the dump buffer
    UINT32  DumpBufSize;    // The original buffer size
    UINT32  DumpSize;       // The size of data placed by CEVA
}CevaDumpInfo;

CevaDumpInfo NCNBVAR dumpinfo [DSPDIAG_NUM_CEVA];
DspDiagCtx   dspdiag_ctx[DSPDIAG_NUM_CEVA];

extern const char LTmrName[];

MXRC DspDiagFixInputDesc(TCB *pTcb);
MXRC DspDiagFixOutputDesc(TCB *pTcb);


MXRC DspDiagRun (DspDiagCtx * pCtx, ApiHeader * pApi)
{
    ApiParam * pTaskID;
    ApiParam * pResID;
    ApiParam * pSetIODesc;
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pApi);

    pTaskID = ApiGetParamByID(pApi, DIAG_TASK_PARAM_ID);
    pResID  = ApiGetParamByID(pApi, DIAG_RES_PARAM_ID);
    pSetIODesc = ApiGetParamByID(pApi, DIAG_SET_IODESC_PARAM_ID);

    pCtx->Tcb.TaskID = 0;
    pCtx->Tcb.ResourceID  = 0;

    if (pCtx->Tcb.InputDataPtr == NULL)
    {
        rc = RC_DSPDIAG_INPUT_ERROR;

        _ASSERT_RC(rc);

        return rc;
    }

    if (pTaskID != NULL)
    {
        pCtx->Tcb.TaskID = pTaskID->ParamData [0];
    }

    if (pResID != NULL)
    {
        pCtx->Tcb.ResourceID = pResID->ParamData [0];
    }

    if (FAILED(rc = DiagAllocOutput(pCtx->hDspHeap, &pCtx->Tcb, pApi)))
    {
        _ASSERT_RC(rc);

        return rc;
    }

    if (pSetIODesc && pCtx->Tcb.IOControlPtr)
    {
        rc = DspDiagFixInputDesc(&pCtx->Tcb);
        if (rc != MX_OK)
            return rc;
    }

    uart_printf ("Run DSP, taskID=%d, resID=%d\r\n", pCtx->Tcb.TaskID, pCtx->Tcb.ResourceID);

    rc = DspDrvRun(0, &pCtx->Tcb);

    if (rc == MX_OK)
    {
        if (pSetIODesc && pCtx->Tcb.IOControlPtr)
        {
            rc = DspDiagFixOutputDesc(&pCtx->Tcb);
            if (rc != MX_OK)
                return rc;
        }

        return MX_OK;
    }

   return RC_DSPDIAG_RUN_TIMEOUT;
}

MXRC DspDiagFixInputDesc(TCB *pTcb)
{
    PIOBUFDESC pBufDesc = (PIOBUFDESC)pTcb->IOControlPtr;
    UINT32 nBufDesc = pTcb->IOControlLen / sizeof(IOBUFDESC);
    PUINT8 pInBuf = (PUINT8)pTcb->InputDataPtr;
    PUINT8 pOutBuf = (PUINT8)pTcb->OutputDataPtr;

    // fix IO descriptors
/*
    uart_printf ("Fixing IO Buffer Descriptors... InputDataLen=%d OutputDataLen=%d NumIODesc=%d\r\n",
        pTcb->InputDataLen,
        pTcb->OutputDataLen,
        nBufDesc);
*/
    while (nBufDesc--)
    {
        if (pBufDesc->IOBufCtrl & IOBDESC_INP_MASK)
        {
//              uart_printf ("InputBufPtr=%08x InputBufLen=%d\r\n", (UINT32)pInBuf, IOBDESC_LEN(pBufDesc));
            pBufDesc->IOBufPtr = pInBuf;
            pInBuf += IOBDESC_LEN(pBufDesc);

            if ((UINT32)pInBuf > ((UINT32)pTcb->InputDataPtr + (UINT32)pTcb->InputDataLen))
            {
                return RC_DSPDIAG_INDESCR_ERROR;
            }
        }
        if (pBufDesc->IOBufCtrl & IOBDESC_OUT_MASK)
        {
//              uart_printf ("OutputBufPtr=%08x OutputBufLen=%d\r\n", (UINT32)pOutBuf, IOBDESC_LEN(pBufDesc));
            if (!(pBufDesc->IOBufCtrl & IOBDESC_INP_MASK))
                pBufDesc->IOBufPtr = pOutBuf;
            pOutBuf += IOBDESC_LEN(pBufDesc);

            if ((UINT32)pOutBuf > ((UINT32)pTcb->OutputDataPtr + (UINT32)pTcb->OutputDataLen))
            {
                return RC_DSPDIAG_OUTDESCR_ERROR;
            }
        }
        pBufDesc++;
    }

    return MX_OK;
}

MXRC DspDiagFixOutputDesc(TCB *pTcb)
{
    PIOBUFDESC pBufDesc = (PIOBUFDESC)pTcb->IOControlPtr;
    UINT32 nBufDesc = pTcb->IOControlLen / sizeof(IOBUFDESC);
    PUINT8 pOutBuf = (PUINT8)pTcb->OutputDataPtr;

    while (nBufDesc--)
    {
        if ((pBufDesc->IOBufCtrl & IOBDESC_IO_MASK) == IOBDESC_IO_MASK)
        {
            if (((UINT32)pOutBuf + IOBDESC_LEN(pBufDesc)) >
                ((UINT32)pTcb->OutputDataPtr + (UINT32)pTcb->OutputDataLen))
            {
                uart_printf ("The IO Descriptors have error  \r\n");
                return RC_DSPDIAG_INOUTDESCR_ERROR;
            }

            memcpy(pOutBuf, pBufDesc->IOBufPtr, IOBDESC_LEN(pBufDesc));
            pOutBuf += IOBDESC_LEN(pBufDesc);
        }
        pBufDesc++;
    }

    return MX_OK;
}

/** @brief This thread is scheduled automatically by the CEVA IRQ handler

    @param pParam [in] - The pointer to the DSP diagnostics context

    @return [MXRC] error code of operation */

UINT32 AThrTimeout (LPVOID pParam)
{
    DspDiagCtx * pCtx = (DspDiagCtx *)pParam;
    MXRC rc = MX_OK;
    ApiHeader * pApi;

    char buf [API_HDR_SIZE + sizeof (ApiParam) + sizeof (UINT32)];

    MxDisableInt(CEVA_IRQ);

    MxStopTimer(pCtx->AsyncTimer);
    MxRemoveTimer(pCtx->AsyncTimer);
    MxRemoveEvent(pCtx->AsyncEvent);
    MxRemoveThread(pCtx->AThrComplete);
    MxRemoveThread(pCtx->AThrTimeout);

    pCtx->AsyncTimer  = HNULL;
    pCtx->AsyncEvent  = HNULL;
    pCtx->AThrComplete= HNULL;
    pCtx->AThrTimeout = HNULL;

    uart_printf ("AsyncDSP timeout\r\n");

    // To create and send indication to the host

#ifdef EMULATE_SYNC_DSP_RUN

    pApi = ApiCreate(buf, API_DEF_FLAGS,
                      pCtx->nSInstance,
                      pCtx->nDInstance,
                      API_TYPE_CFG_RESP,
                      DIAG_RUN);
#else

    pApi = ApiCreate(buf, API_DEF_FLAGS,
                      pCtx->nSInstance,
                      pCtx->nDInstance,
                      API_TYPE_IND,
                      DIAG_RUNASYNC);
#endif


    _ASSERT_PTR(pApi);

    ApiAddErrorCode(pApi, RC_DSPDIAG_RUN_TIMEOUT);

    rc = SvsrSendAPI(pApi);

    _ASSERT_RC(rc);

    return rc;
}



/** @brief This thread is scheduled automatically by the CEVA IRQ handler

    @param pParam [in] - The pointer to the DSP diagnostics context

    @return [MXRC] error code of operation */

UINT32 AThrComplete (LPVOID pParam)
{
    MXRC rc = MX_OK;
    DspDiagCtx * pCtx = (DspDiagCtx *)pParam;
    ApiHeader * pApi;

    char buf [API_HDR_SIZE + (sizeof (ApiParam) + sizeof (UINT32)) * 2];

    MxDisableInt(CEVA_IRQ);

    MxStopTimer(pCtx->AsyncTimer);
    MxRemoveTimer(pCtx->AsyncTimer);
    MxRemoveEvent(pCtx->AsyncEvent);
    MxRemoveThread(pCtx->AThrComplete);
    MxRemoveThread(pCtx->AThrTimeout);

    pCtx->AsyncTimer  = HNULL;
    pCtx->AsyncEvent  = HNULL;
    pCtx->AThrComplete= HNULL;
    pCtx->AThrTimeout = HNULL;

    // To create and send indication to the host

#ifdef EMULATE_SYNC_DSP_RUN
    pApi = ApiCreate(buf, API_DEF_FLAGS,
                      pCtx->nSInstance,
                      pCtx->nDInstance,
                      API_TYPE_CFG_RESP,
                      DIAG_RUN);
#else
    pApi = ApiCreate(buf, API_DEF_FLAGS,
                      pCtx->nSInstance,
                      pCtx->nDInstance,
                      API_TYPE_IND,
                      DIAG_RUNASYNC);
#endif

    _ASSERT_PTR(pApi);

    ApiAddErrorCode(pApi, MX_OK);
    ApiAddParam(pApi, NULL, DIAG_DSP_CYCLES, 0, 4);

    if (pCtx->bProcIO)
    {
        DspDiagFixOutputDesc(&pCtx->Tcb);
    }

    uart_printf ("Async DSP task is complete \r\n");

    rc = SvsrSendAPI(pApi);

    _ASSERT_RC(rc);

    return rc;
}

void DspDiagDoneCb(void *p)
{
    PDspDiagCtx pCtx = (PDspDiagCtx)p;

    MxScheduleThread(pCtx->AThrComplete);
}

MXRC DspDiagAsyncRun (DspDiagCtx * pCtx, ApiHeader * pApi)
{
    MXRC rc = MX_OK;
    ApiParam * pTaskID;
    ApiParam * pResID;
    ApiParam * pSetIODesc;
    HANDLE hTimer;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pApi);
    _ASSERT_PTR(pCtx->Tcb.InputDataPtr);

    rc = MxGetObjByName(CUR_CPU, LTmrName, &hTimer);

    if (FAILED(rc))
    {
        hTimer = HNULL;
        uart_printf ("Warning: LTmr is not found\r\n");
    }

    pTaskID    = ApiGetParamByID(pApi, DIAG_TASK_PARAM_ID);
    pResID     = ApiGetParamByID(pApi, DIAG_RES_PARAM_ID);
    pSetIODesc = ApiGetParamByID(pApi, DIAG_SET_IODESC_PARAM_ID);

    pCtx->Tcb.TaskID = 0;
    pCtx->Tcb.ResourceID  = 0;

    // Specialy reversed
    pCtx->nDInstance  = pApi->SrcInstID;
    pCtx->nSInstance  = pApi->DstInstID;

    if (pCtx->Tcb.InputDataPtr == NULL)
    {
        rc = RC_DSPDIAG_INPUT_ERROR;

        _ASSERT_RC(rc);

        return rc;
    }

    if (pTaskID != NULL)
    {
        pCtx->Tcb.TaskID = pTaskID->ParamData [0];
    }

    if (pResID != NULL)
    {
        pCtx->Tcb.ResourceID = pResID->ParamData [0];
    }

    if (FAILED(rc = DiagAllocOutput(pCtx->hDspHeap, &pCtx->Tcb, pApi)))
    {
        _ASSERT_RC(rc);

        return rc;
    }

    rc = MxCreateThread(AThrComplete, NULL, pCtx, DSP_WAIT_PRIOR, &pCtx->AThrComplete);

    if (FAILED (rc))
    {
        _ASSERT_RC(rc);

        return rc;
    }

    rc = MxCreateThread(AThrTimeout, NULL, pCtx, DSP_WAIT_PRIOR, &pCtx->AThrTimeout);

    if (FAILED (rc))
    {
        _ASSERT_RC(rc);

        MxRemoveThread(pCtx->AThrComplete);
        pCtx->AThrComplete = HNULL;

        return rc;
    }

    rc = MxCreateEvent(EVENT_MODE_RESET, NULL, &pCtx->AsyncEvent);

    if (FAILED (rc))
    {
        _ASSERT_RC(rc);

        MxRemoveThread(pCtx->AThrComplete);
        MxRemoveThread(pCtx->AThrTimeout);

        pCtx->AThrComplete = HNULL;
        pCtx->AThrTimeout  = HNULL;

        return rc;
    }

    rc = MxCreateTimer(TIMER_MODE_ONE_TIME, DSP_RUN_TIMEOUT, NULL, pCtx->AsyncEvent, &pCtx->AsyncTimer);

    if (FAILED (rc))
    {
        _ASSERT_RC(rc);

        MxRemoveThread(pCtx->AThrComplete);
        MxRemoveThread(pCtx->AThrTimeout);
        MxRemoveEvent (pCtx->AsyncEvent);

        pCtx->AThrComplete = HNULL;
        pCtx->AThrTimeout  = HNULL;
        pCtx->AsyncEvent   = HNULL;

        return rc;
    }

    rc = MxScheduleThreadByEvent(pCtx->AThrTimeout, pCtx->AsyncEvent, THREAD_EVENT_AUTO_RESET);

    if (FAILED (rc))
    {
        _ASSERT_RC(rc);

        MxRemoveThread(pCtx->AThrComplete);
        MxRemoveThread(pCtx->AThrTimeout);
        MxRemoveEvent (pCtx->AsyncEvent);
        MxRemoveTimer (pCtx->AsyncTimer);

        pCtx->AThrComplete = HNULL;
        pCtx->AThrTimeout  = HNULL;
        pCtx->AsyncEvent   = HNULL;
        pCtx->AsyncTimer   = HNULL;

        return rc;
    }

    if (pSetIODesc && pCtx->Tcb.IOControlPtr)
    {
        rc = DspDiagFixInputDesc(&pCtx->Tcb);
        if (rc != MX_OK)
            return rc;
    }
    else
    {
        pCtx->bProcIO = FALSE;
    }

    pCtx->Tcb.Status = 0;

    uart_printf ("AsyncRun DSP, taskID=%d, resID=%d\r\n", pCtx->Tcb.TaskID, pCtx->Tcb.ResourceID);

    rc = MxStartTimer(pCtx->AsyncTimer);

    if (FAILED(rc))
    {
        uart_printf ("Start timer error: %x\r\n", rc);

        _ASSERT_RC(rc);

        MxRemoveEvent (pCtx->AsyncEvent);
        MxRemoveTimer (pCtx->AsyncTimer);
        MxRemoveThread(pCtx->AThrComplete);
        MxRemoveThread(pCtx->AThrTimeout);

        pCtx->AThrComplete = HNULL;
        pCtx->AThrTimeout  = HNULL;
        pCtx->AsyncEvent   = HNULL;
        pCtx->AsyncTimer   = HNULL;

        return rc;
    }

    DSPDrvSetCallBack(DevInfoGetValue(CONSTID_DSP_STARTID), DspDiagDoneCb, pCtx);
    DspDrvSetAsynMode(DevInfoGetValue(CONSTID_DSP_STARTID));

    MxEnableInt(CEVA_IRQ);

    DspDrvRun(DevInfoGetValue(CONSTID_DSP_STARTID), &pCtx->Tcb);

    return rc;
}

MXRC  DspDiagInit(UINT32* pReportData, UINT32* pReportSize)
{
    UINT32 i;
	UINT32 olddev = 0;
    MXRC rcdsp, rc = MX_OK;
    void * pUartBuf = NULL;
    UINT32 nLen = 3*1024;

    PDspDiagCtx pCtx;

   	olddev = uart_printf_get_dev();

    memset  (&dumpinfo, 0, sizeof (dumpinfo));

    #ifndef DIAG_DSP_PRINT_ALWAYS
    pUartBuf = MxHeapAlloc (AppHeap, nLen);
    if (pUartBuf != NULL)
    {
        uart_printf_select_dev(PRINT_DEV_BUF, (UINT32)pUartBuf, nLen);
    }
    #endif
    
    rcdsp = DspDrvInit();

    if (pUartBuf != NULL)
    {
        if (FAILED(rcdsp))
        {
            uart_printf_print_buf();
        }

        uart_printf_select_dev(olddev, 0, 0);
        MxHeapFree(AppHeap, pUartBuf);
    }

    for (i = 0; i < DSPDIAG_NUM_CEVA; i++)
    {
        pCtx = &dspdiag_ctx[i];
        memset(pCtx, 0, sizeof (DspDiagCtx));
        
        pCtx->hDspHeap = DspHeap;
        rc = SvsrRegClient(IID_DSP_DIAG_0 + i, DspDiagApiHandler, pCtx);
    }

    _ASSERT_RC(rcdsp);
    _ASSERT_RC(rc);

    if (FAILED(rcdsp))
        return rcdsp;

    return rc;
}

MXRC DspDiagCevaDump (ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * pMask, *pBufInfo;
    UINT32 nMask, i;
    MXRC rc = MX_OK;
    UINT32 nOutSize= (128 * 1024) + HW_TCB_SIZE;   // The default buffer size for CEVA dump

    _ASSERT_PTR (pApi);
    _ASSERT_PTR (pResp);

    uart_printf ("CEVA dumping ... \n");

    if ((pMask = ApiGetParamByID (pApi, DSPDIAG_PARAM_MASK)) == NULL)
        return RC_DSPDIAG_PARAM_ERROR;

    nMask = pMask->ParamData [0];

    // the optional parameter for the buffer size
    if ((pBufInfo = ApiGetParamByID (pApi, DSPDIAG_PARAM_BUFINFO)) != NULL)
    {
        nOutSize = pBufInfo->ParamData [0];
    }

    uart_printf("DspDiagCevaDump: use nMask=0x%X nOutSize=%d\n", nMask, nOutSize);

    for (i = 0; i < DSPDIAG_NUM_CEVA; i++)
    {
        if (nMask & (1 << i))
        {
            // The information for the host ,
            // it will be used in the response
            dumpinfo[i].CevaID = i;

            if ((dumpinfo [i].DumpBufSize < nOutSize) || (dumpinfo [i].pDumpBuf == NULL))
            {
                if (dumpinfo [i].pDumpBuf != NULL)
                    MxHeapFree (NcnbHeap, dumpinfo [i].pDumpBuf);

                dumpinfo [i].pDumpBuf = MxHeapAlloc (NcnbHeap, nOutSize);

                if (dumpinfo [i].pDumpBuf == NULL)
                    return RC_DSPDIAG_ALLOC_BUF_ERROR;

                dumpinfo [i].DumpBufSize = nOutSize;
            }

            dumpinfo [i].DumpSize = dumpinfo [i].DumpBufSize;

            if (FAILED(rc = DspDrvDump (i, dumpinfo [i].pDumpBuf, &dumpinfo [i].DumpSize)))
                return rc;

            uart_printf ("CEVA-%d dump info\n", i);
            uart_printf (" *buffer    = 0x%08x\n", dumpinfo [i].pDumpBuf);
            uart_printf (" *dump size = %d\n", dumpinfo [i].DumpSize);

            // response parameter: address of buffer, buffer size, and dump size
            ApiAddParam (pResp, NULL, DSPDIAG_PARAM_DUMP, &dumpinfo [i], sizeof (dumpinfo [i]));
        }
    }

    uart_printf ("OK\r\n");

    return MX_OK;
}

MXRC DspDiagQueryCevaDump (ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * pID, *pOffs, *pSize;
    UINT32 i;
    PUINT8 pData;
    UINT32 nSize;

    _ASSERT_PTR (pApi);
    _ASSERT_PTR (pResp);

    if ((pID = ApiGetParamByID (pApi, DSPDIAG_PARAM_DEVID)) == NULL)
        return RC_DSPDIAG_PARAM_ERROR;

    i = pID->ParamData [0];

    if (i >= DSPDIAG_NUM_CEVA)
        return RC_DSPDIAG_PARAM_ERROR;

    if (dumpinfo [i].pDumpBuf == NULL || dumpinfo [i].DumpSize == 0)
        return RC_DSPDIAG_BUF_EMPTY;

    if ((pOffs = ApiGetParamByID (pApi, DSPDIAG_PARAM_BUF_OFFS)) == NULL)
        return RC_DSPDIAG_PARAM_OFFS_ERROR;

    if ((pSize = ApiGetParamByID (pApi, DSPDIAG_PARAM_BUF_SIZE)) == NULL)
        return RC_DSPDIAG_PARAM_SIZE_ERROR;

    if (pOffs->ParamData [0] >= dumpinfo [i].DumpSize)
        return RC_DSPDIAG_PARAM_OFFS_ERROR;

    pData = dumpinfo [i].pDumpBuf + pOffs->ParamData [0];

    if ((pOffs->ParamData [0] + pSize->ParamData[0]) > dumpinfo [i].DumpSize)
    {
        nSize = dumpinfo [i].DumpSize - pOffs->ParamData [0];
    }
    else
    {
        nSize = pSize->ParamData[0];
    }

    ApiAddParam (pResp, NULL, DSPDIAG_PARAM_DUMP, pData, nSize);

    return MX_OK;
}

APIHEC DspDiagApiHandler (LPVOID pData, ApiHeader * pApi, ApiHeader * pResp)
{
    MXRC rc = MX_OK;
    PDspDiagCtx pCtx = (PDspDiagCtx)pData;
    UINT32 nCycles = 0;

    if (pApi->Type == API_TYPE_CFG_PARAM)
    {
        pResp->Type = API_TYPE_CFG_RESP;

        switch (pApi->MessageID)
        {
            case DIAG_RUN:
                {

#ifdef EMULATE_SYNC_DSP_RUN
                    rc = DspDiagAsyncRun (pCtx, pApi);
                    return APIHEC_DROP_RESP;
#else
                    rc = DspDiagRun (pCtx, pApi);
                    nCycles = pCtx->Tcb.ExecTicks;
                    break;
#endif
                }
            case DIAG_RUNASYNC:
                {
                    rc = DspDiagAsyncRun (pCtx, pApi);
                    break;
                }

            case DIAG_CONTEXT:
                {
                    rc = DiagSetContext(pCtx->hDspHeap, &pCtx->Tcb, pApi);
                    break;
                }

            case DIAG_CONTROL:
                {
                    rc = DiagSetControl(pCtx->hDspHeap, &pCtx->Tcb, pApi);
                    break;
                }

            case DIAG_INPUT:
                {
                    rc = DiagSetInput(pCtx->hDspHeap, &pCtx->Tcb, pApi);
                    break;
                }
            case DIAG_CEVA_DUMP:
                {
                    rc = DspDiagCevaDump(pApi, pResp);
                    break;
                }
            default:
                {
                    rc = RC_DSPDIAG_UNKNOWN_API;
                    break;
                }
        }
    }
    else if (pApi->Type == API_TYPE_QRY_PARAM)
    {
        pResp->Type = API_TYPE_QRY_RESP;

        switch (pApi->MessageID)
        {
            case DIAG_CONTEXT:
                {
                    rc = DiagGetContext(&pCtx->Tcb, pResp);
                    break;
                }

            case DIAG_CONTROL:
                {
                    rc = DiagGetControl(&pCtx->Tcb, pResp);
                    break;
                }

            case DIAG_INPUT:
                {
                    rc = DiagGetInput(&pCtx->Tcb, pApi, pResp);
                    break;
                }

            case DIAG_OUTPUT:
                {
                    rc = DiagGetOutput(&pCtx->Tcb, pApi, pResp);
                    break;
                }
            case DIAG_Q_CEVA_DUMP:
                {
                    rc = DspDiagQueryCevaDump(pApi, pResp);
                    break;
                }
            default:
                {
                    rc = RC_DSPDIAG_UNKNOWN_API;
                    break;
                }
        }

        if (SUCCESSFUL(rc))
            return APIHEC_OK;

    }
    else
    {
        rc = RC_DSPDIAG_UNSUP_TYPE;
    }

    ApiAddErrorCode(pResp, rc);

    if (pApi->MessageID == DIAG_RUN)
    {
        ApiAddParam(pResp, NULL, DIAG_DSP_CYCLES, &nCycles, sizeof (nCycles));
    }

    return APIHEC_OK;
}


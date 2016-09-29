//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL armdiag.c
 *
 * @brief The ArmDiagnostics (ARM software implementation
*       of the FPGA H/W components )
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "armdiag.h"
#include "heaps.h"
#include "appprintf.h"
#include "timerdrv.h"

extern const char LTmrName[];

MXRC  ArmDiagInit (UINT32* pReportData, UINT32* pReportSize)
{
    MXRC rc = RC_ARMDIAG_OK;

    ArmDiagCtx* pCtx;

    pCtx = (ArmDiagCtx*)MxHeapAlloc(AppHeap, sizeof (ArmDiagCtx));

    if (pCtx == NULL)
        return RC_ARMDIAG_CTX_ALLOC_ERROR;

    memset (pCtx, 0, sizeof (ArmDiagCtx));

    pCtx->hHeap = DspHeap;

    rc = SvsrRegClient(IID_SW_DIAG, ArmDiagApiHandler, pCtx);

    _ASSERT_RC(rc);

    return rc;
}


MXRC ArmDiagRun (PArmDiagCtx pCtx, ApiHeader * pApi, UINT32 * pnCycles)
{
    UINT32 i = 0;

    MXRC rc = MX_OK;

    ApiParam * pTaskID;
    ApiParam * pResID;
    ApiParam * pSetIODesc;

    HANDLE hTimer = HNULL;

    UINT32 nBufDesc;
    PIOBUFDESC pBufDesc;
    PUINT8 pInBuf;
    PUINT8 pOutBuf;

    UINT32  nWaitCount = 0, MsCount, nTicks=0, nTicks2=0, tmp;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pApi);

    rc = MxGetObjByName(CUR_CPU, LTmrName, &hTimer);

    if (FAILED(rc))
    {
        hTimer = HNULL;
        printf ("Warning: LTmr is not found\r\n");
    }

    pTaskID = ApiGetParamByID(pApi, DIAG_TASK_PARAM_ID);
    pResID  = ApiGetParamByID(pApi, DIAG_RES_PARAM_ID);
    pSetIODesc = ApiGetParamByID(pApi, DIAG_SET_IODESC_PARAM_ID);

    pCtx->Tcb.TaskID = 0;
    pCtx->Tcb.ResourceID  = 0;

    if (pCtx->Tcb.InputDataPtr == NULL)
    {
        rc = RC_ARMDIAG_INPUT_ERROR;

        _ASSERT_RC(rc);

        return rc;
    }

    if (FAILED(rc = DiagAllocOutput(pCtx->hHeap, &pCtx->Tcb, pApi)))
    {
        _ASSERT_RC(rc);

        return rc;
    }

    // To setup the IO descriptors

    if (pSetIODesc !=NULL && pCtx->Tcb.IOControlPtr != NULL)
    {
        // fix IO descriptors
        pInBuf = (PUINT8)pCtx->Tcb.InputDataPtr;
        pOutBuf = (PUINT8)pCtx->Tcb.OutputDataPtr;
        pBufDesc = (PIOBUFDESC)pCtx->Tcb.IOControlPtr;
        nBufDesc = pCtx->Tcb.IOControlLen / sizeof(IOBUFDESC);

        printf ("Fixing IO Buffer Descriptors... InputDataLen=%d OutputDataLen=%d NumIODesc=%d\r\n",
            pCtx->Tcb.InputDataLen,
            pCtx->Tcb.OutputDataLen,
            nBufDesc);

        while (nBufDesc--)
        {
            if (pBufDesc->IOBufCtrl & IOBDESC_INP_MASK)
            {
//              printf ("InputBufPtr=%08x InputBufLen=%d\r\n", (UINT32)pInBuf, IOBDESC_LEN(pBufDesc));
                pBufDesc->IOBufPtr = pInBuf;
                pInBuf += IOBDESC_LEN(pBufDesc);

                if ((UINT32)pInBuf > ((UINT32)pCtx->Tcb.InputDataPtr + (UINT32)pCtx->Tcb.InputDataLen))
                {
                    rc = RC_ARMDIAG_INDESCR_ERROR;

                    _ASSERT_RC(rc);

                    return rc;
                }
            }
            else if (pBufDesc->IOBufCtrl & IOBDESC_OUT_MASK)
            {
//              printf ("OutputBufPtr=%08x OutputBufLen=%d\r\n", (UINT32)pOutBuf, IOBDESC_LEN(pBufDesc));
                pBufDesc->IOBufPtr = pOutBuf;
                pOutBuf += IOBDESC_LEN(pBufDesc);

                if ((UINT32)pOutBuf > ((UINT32)pCtx->Tcb.OutputDataPtr + (UINT32)pCtx->Tcb.OutputDataLen))
                {
                    rc = RC_ARMDIAG_OUTDESCR_ERROR;

                    _ASSERT_RC(rc);

                    return rc;
                }
            }
            pBufDesc++;
        }
    }

    if (pTaskID != NULL)
    {
        pCtx->Tcb.TaskID = pTaskID->ParamData [0];
    }

    if (pResID != NULL)
    {
        pCtx->Tcb.ResourceID = pResID->ParamData [0];
    }

    pCtx->Tcb.Status = 0;

    i = 0;

    while (armtasks [i].Proc != NULL)
    {
        if (armtasks [i].nTaskID == pCtx->Tcb.TaskID)
        {
            printf ("Run ARM Diag, taskID=%d, resID=%d\r\n",
                      pCtx->Tcb.TaskID,
                      pCtx->Tcb.ResourceID);

            if (HANDLE_NOT_NULL(hTimer))
            {
               IRQTYPE irq;

                irq = ARM_INT_disable();

                nWaitCount = MxGetTime();

                nTicks = TimerDrvGetTicks ();

                ARM_INT_restore(irq);
            }

            // ARM Diag Task running
            armtasks [i].Proc (&pCtx->Tcb);

            if (HANDLE_NOT_NULL(hTimer))
            {
                IRQTYPE irq;

                irq = ARM_INT_disable();

                nTicks2 = TimerDrvGetTicks ();

                MsCount = MxGetTime() - nWaitCount;

                ARM_INT_restore(irq);

                // The timer is arranged to rise an interrupt every 1 ms
                rc = MxReadDrvData(hTimer, TIMER_CMD_GET_PERIOD, &tmp, NULL);

                // the timer countdowns

                if (nTicks > nTicks2)
                {
                    nTicks = nTicks - nTicks2;
                }
                else
                {
                    nTicks  = tmp - (nTicks2 - nTicks) + 1;
                    MsCount --;
                }

                // To add ticks stored in milliseconds
                nTicks += tmp * MsCount;

                // Convert to the CPU frequency
                nTicks *= (2);

                if (pnCycles != NULL)
                {
                    printf ("ARM cycles=%d\r\n", nTicks);

                    *pnCycles = nTicks;
                }
            }


            if (pSetIODesc !=NULL && pCtx->Tcb.IOControlPtr != NULL)
            {
                pBufDesc = (PIOBUFDESC)pCtx->Tcb.IOControlPtr;
                nBufDesc = pCtx->Tcb.IOControlLen / sizeof(IOBUFDESC);
                pInBuf = (PUINT8)pCtx->Tcb.InputDataPtr;
                pOutBuf = (PUINT8)pCtx->Tcb.OutputDataPtr;

                while (nBufDesc--)
                {
                    if ((pBufDesc->IOBufCtrl & IOBDESC_IO_MASK) == IOBDESC_IO_MASK)
                    {
                        if (((UINT32)pOutBuf + IOBDESC_LEN(pBufDesc)) >
                            ((UINT32)pCtx->Tcb.OutputDataPtr + (UINT32)pCtx->Tcb.OutputDataLen))
                        {
                            rc = RC_ARMDIAG_INOUTDESCR_ERROR;

                            printf ("The IO Descriptors have error  \r\n");

                            _ASSERT_RC(rc);

                            return rc;
                        }

                        memcpy(pOutBuf, pBufDesc->IOBufPtr, IOBDESC_LEN(pBufDesc));
                        pOutBuf += IOBDESC_LEN(pBufDesc);
                    }
                    pBufDesc++;
                }
            }

            printf ("The ARM is complete \r\n");

            return MX_OK;
        }

        i++;
    }

    printf ("ARM diag task ID(%d) error !\r\n", pCtx->Tcb.TaskID);

    return RC_ARMDIAG_TASKID_ERROR;
}


APIHEC ArmDiagApiHandler (LPVOID pData, ApiHeader * pApi, ApiHeader * pResp)
{
    MXRC rc = MX_OK;
    PArmDiagCtx pCtx = (PArmDiagCtx)pData;
    UINT32 nCycles = 0;

    if (pApi->Type == API_TYPE_CFG_PARAM)
    {
        pResp->Type = API_TYPE_CFG_RESP;

        switch (pApi->MessageID)
        {
            case DIAG_RUN:
                {
                    rc = ArmDiagRun (pCtx, pApi, &nCycles);
                    break;
                }

            case DIAG_CONTEXT:
                {
                    rc = DiagSetContext(pCtx->hHeap, &pCtx->Tcb, pApi);
                    break;
                }

            case DIAG_CONTROL:
                {
                    rc = DiagSetControl(pCtx->hHeap, &pCtx->Tcb, pApi);
                    break;
                }

            case DIAG_INPUT:
                {
                    rc = DiagSetInput(pCtx->hHeap, &pCtx->Tcb, pApi);
                    break;
                }
            default:
                {
                    rc = RC_ARMDIAG_UNKNOWN_API;
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
            default:
                {
                    rc = RC_ARMDIAG_UNKNOWN_API;
                    break;
                }
        }

        if (SUCCESSFUL(rc))
            return APIHEC_OK;

    }
    else
    {
        rc = RC_ARMDIAG_UNSUP_TYPE;
    }

    ApiAddErrorCode(pResp, rc);

    if (pApi->MessageID == DIAG_RUN)
    {
        ApiAddParam(pResp, NULL, DIAG_DSP_CYCLES, &nCycles, sizeof (nCycles));
    }

    return APIHEC_OK;
}



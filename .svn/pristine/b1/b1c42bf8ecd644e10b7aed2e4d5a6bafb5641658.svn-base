//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL diagnostics.c
 *
 * @brief The root module of LowARM diagnostics, it contains:
*          - DspDiag
*          - FECDiag
*          - FPDiag
 * @author Mindspeed Technologies
 * @version $Revision: 1.17 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "config.h"
#include <stdlib.h>
#include "diagnostics.h"
#include "lcorecfg.h"
#include "dspdiag.h"
#include "fecdiag.h"
#include "fpdiag.h"
#include "armdiag.h"
#include "appprintf.h"
#include <string.h>
#include "schdiagctrl.h"
#include "bootlog.h"
#include "i2cdiag.h"
#include "spaccdrv.h"
#include "spidiag.h"
#include "pciediag.h"
#include "swdiag.h"
/*****************************************************************************
*            The table of system diagnostics modules                         *
******************************************************************************/

BEGIN_DIAG_TABLE(DiagModules)

#ifdef DIAG_PCIE_ENABLED
    REG_DIAG_MODULE(PCIEDiag, PCIE_DIAG_ID, "PCIe diagnostics")
#endif

#ifdef DIAG_DSP_ENABLED
    REG_DIAG_MODULE(DspDiag, DSPDIAG_ID, "Dsp diagnostics")
#endif

#ifdef DIAG_FEC_ENABLED
    REG_DIAG_MODULE(FecDiag, FECDIAG_ID, "FEC diagnostics")
#endif

#ifdef DIAG_ARM_ENABLED
    REG_DIAG_MODULE(ArmDiag, ARMDIAG_ID, "ARM diagnostics")
#endif

#ifdef DIAG_SCH_ENABLED
    REG_DIAG_MODULE(SchDiag, SCH_DIAG_ID, "Scheduler diagnostics")
#endif

#ifdef DIAG_I2C_ENABLED
    REG_DIAG_MODULE(I2CDiag, I2C_DIAG_ID, "I2C diagnostics")
#endif

#ifdef DIAG_SPACC_ENABLED
    REG_DIAG_MODULE(SPAccDiag, SPACC_DIAG_ID, "SPAcc diagnostics")
#endif

#ifdef DIAG_SPI_ENABLED
    REG_DIAG_MODULE(SPIDiag, SPI_DIAG_ID, "SPI diagnostics")
#endif

#ifdef SWDIAG_EN
    REG_DIAG_MODULE(SWDiagnostics, SW_DIAG_ID, "SW diagnostics")
#endif

#ifdef DIAG_FP_ENABLED
    REG_DIAG_MODULE(FPDiag, FPDIAG_ID, "FP diagnostics")
#endif

END_DIAG_TABLE()


MXRC DiagSetContext (HANDLE hHeap, TCB * pTCB, ApiHeader * pApi)
{
    ApiParam * pParam = ApiGetParamByID(pApi, DIAG_CONTEXT_PARAM_ID);

    UINT32 nDataSize;

    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    if (pTCB->ContextPtr != NULL)
    {
        MxHeapFree(hHeap, pTCB->ContextPtr);
        pTCB->ContextPtr = NULL;
        pTCB->ContextLen = 0;
    }

    nDataSize = ApiGetParamDataSize(pParam);

    if (nDataSize)
    {

        pTCB->ContextPtr = MxHeapAlloc(hHeap, nDataSize);

        if (pTCB->ContextPtr == NULL)
            return RC_DIAG_ALLOC_BUF_ERROR;

        memcpy (pTCB->ContextPtr, pParam->ParamData, nDataSize);

        pTCB->ContextLen = nDataSize;
    }

    return MX_OK;
}

MXRC DiagSetControl (HANDLE hHeap, TCB * pTCB, ApiHeader * pApi)
{
    ApiParam * pParam = ApiGetParamByID(pApi, DIAG_CONTROL_PARAM_ID);
    UINT32 nDataSize;

    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    if (pTCB->IOControlPtr != NULL)
    {
        MxHeapFree(hHeap, pTCB->IOControlPtr);
        pTCB->IOControlPtr = NULL;
        pTCB->IOControlLen = 0;
    }

    nDataSize = ApiGetParamDataSize(pParam);

    if (nDataSize)
    {
        pTCB->IOControlPtr  = MxHeapAlloc(hHeap, nDataSize);

        if (pTCB->IOControlPtr  == NULL)
            return RC_DIAG_ALLOC_BUF_ERROR;

        memcpy (pTCB->IOControlPtr, pParam->ParamData, nDataSize);

        pTCB->IOControlLen = nDataSize;
    }

    return MX_OK;
}

MXRC DiagSetInput (HANDLE hHeap, TCB * pTCB, ApiHeader * pApi)
{
    if (pTCB->InputDataPtr != NULL)
    {
        MxHeapFree(hHeap, pTCB->InputDataPtr);
        pTCB->InputDataPtr = NULL;
        pTCB->InputDataLen = 0;
    }

    if (pApi->Length)
    {
        pTCB->InputDataPtr = MxHeapAlloc(hHeap, pApi->Length);

        if (pTCB->InputDataPtr == NULL)
            return RC_DIAG_ALLOC_BUF_ERROR;

        pTCB->InputDataLen = pApi->Length;

        memcpy (pTCB->InputDataPtr, GetApiPayload(pApi), pApi->Length);
    }

    return MX_OK;
}

MXRC DiagSetOutputSize (HANDLE hHeap, TCB * pTCB, UINT32 nSize)
{
    _ASSERT_PTR(pTCB);

    if (pTCB->OutputDataPtr != NULL)
    {
        MxHeapFree(hHeap, pTCB->OutputDataPtr);
        pTCB->OutputDataPtr = NULL;
        pTCB->OutputDataLen = 0;
    }

    if (nSize)
    {
        pTCB->OutputDataPtr = MxHeapAlloc(hHeap, nSize);

        if (pTCB->OutputDataPtr == NULL)
            return RC_DIAG_ALLOC_BUF_ERROR;

        pTCB->OutputDataLen = nSize;

        memset (pTCB->OutputDataPtr, 0, nSize);
    }

    return MX_OK;
}

MXRC DiagAllocOutput (HANDLE hHeap, TCB * pTCB, ApiHeader * pApi)
{
    MXRC rc;
    ApiParam * pOutSize;

    _ASSERT_PTR(pTCB);
    _ASSERT_PTR(pApi);

    pOutSize= ApiGetParamByID(pApi, DIAG_SET_OUTPUT_SIZE);

    if (pOutSize != NULL)
    {
        rc = DiagSetOutputSize(hHeap, pTCB, pOutSize->ParamData[0]);
        if (FAILED(rc))
        {
            uart_printf ("Set output buffer error (0x%x)\r\n", rc);
            return rc;
        }

//      uart_printf ("Set output buffer OK (size=%d)\r\n", pOutSize->ParamData[0]);
    }
    else
    {
        _ASSERT(pTCB->InputDataLen != 0);

        rc = DiagSetOutputSize(hHeap, pTCB, pTCB->InputDataLen);
        if (FAILED(rc))
        {
            uart_printf ("Set output buffer error (0x%x)\r\n", rc);
            return rc;
        }

//      uart_printf ("Output buffer size is equal to the input buffer size\r\n");
//      uart_printf ("Output size is equal to %d bytes\r\n", pTCB->InputDataLen);
    }

    return MX_OK;
}


MXRC DiagGetContext (TCB * pTCB, ApiHeader * pApi)
{
    ApiAddParam(pApi, NULL, DIAG_CONTEXT_PARAM_ID, pTCB->ContextPtr,
                 pTCB->ContextLen);

    return MX_OK;
}

MXRC DiagGetControl (TCB * pTCB, ApiHeader * pApi)
{
    ApiAddParam(pApi, NULL, DIAG_CONTROL_PARAM_ID, pTCB->IOControlPtr,
                  pTCB->IOControlLen);

    return MX_OK;
}

MXRC DiagGetInput (TCB * pTCB, ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * pParam;

    UINT32 nSize, nOffs;

    MXRC rc = MX_OK;

    // Size

    pParam = ApiGetParamByID(pApi, DIAG_BUF_SIZE_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    nSize = pParam->ParamData[0];

    // Offset

    pParam = ApiGetParamByID(pApi, DIAG_BUF_OFFS_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    nOffs = pParam->ParamData[0];

    if ((nSize + nOffs > pTCB->InputDataLen))
    {
        uart_printf ("DSPDIAG: GetInputParamError !\r\n");
        uart_printf ("OFFS:%d,  SIZE:%d, TOTAL:%d\r\n", nOffs, nSize, pTCB->InputDataLen);

        if(nOffs >= pTCB->InputDataLen)
            return RC_DIAG_PARAM_ERROR;

        nSize = pTCB->InputDataLen - nOffs;
    }

    ApiAddParam(pResp, NULL, DIAG_DATA_PARAM_ID,
                 (PUINT8)pTCB->InputDataPtr + nOffs, nSize);

    return rc;
}


MXRC DiagGetOutput (TCB * pTCB, ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * pParam;

    UINT32 nSize, nOffs;

    MXRC rc = MX_OK;

    // Size

    pParam = ApiGetParamByID(pApi, DIAG_BUF_SIZE_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    nSize = pParam->ParamData[0];

    // Offset

    pParam = ApiGetParamByID(pApi, DIAG_BUF_OFFS_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    nOffs = pParam->ParamData[0];

    if ((nSize + nOffs > pTCB->OutputDataLen))
    {
        uart_printf ("DSPDIAG: GetOutputParamError !\r\n");
        uart_printf ("OFFS:%d,  SIZE:%d, TOTAL:%d\r\n", nOffs, nSize, pTCB->InputDataLen);

        if(nOffs >= pTCB->OutputDataLen)
            return RC_DIAG_PARAM_ERROR;

        nSize = pTCB->OutputDataLen - nOffs;

    }

    ApiAddParam(pResp, NULL, DIAG_DATA_PARAM_ID,
                 (PUINT8)pTCB->OutputDataPtr + nOffs, nSize);

    return rc;
}



/** @brief The function initializes system diagnostics modules

    @return [MXRC] initialization error code */

MXRC  DiagInit (void)
{
    MXRC rc = MX_OK;
    UINT32 i = 0, nl;
    UINT32 DiagMask = 0x7FFFFFFF;//CoreCfg.DiagCtrl;

    ModuleDiagInfo* pDiagInfo  = DiagInfo;
    UINT8* pResBuf = ReportBuffer ;
    UINT32 Mask = 0;

    uart_printf ("------------------------------");
    uart_printf ("\r\n     L-ARM DIAGNOSTICS        ");
    uart_printf ("\r\n------------------------------\r\n");
    
    
    while (DiagModules [i].Proc != NULL)
    {
        if( ((DiagMask >> DiagModules[i].Id)&1) == 1 )
         {
            Mask = ((UINT32)1 << DiagModules[i].Id);
            DiagMask &= ~Mask;

            pDiagInfo->ModuleId = DiagModules[i].Id;
            pDiagInfo->ExtReportSize = 0;
            pDiagInfo->ExtReportData = pResBuf;

            uart_printf ("DIAGNOSTICS [%S]", DiagModules [i].Name);
            
            uart_printf_reset_num();
            uart_printf_set_align(2);
            uart_printf_set_header("\r\n------------------------------\r\n");
            //uart_printf_set_header("\r\n");

            rc = DiagModules [i].Proc ((UINT32*)pResBuf, &pDiagInfo->ExtReportSize);

            uart_printf_set_align(0);
            uart_printf_set_header(NULL);
            
            pDiagInfo->DiagResult = rc;
            pResBuf += pDiagInfo->ExtReportSize;
            pDiagInfo++;

            if ( (nl = uart_printf_get_num()) != 0)
            {
                uart_printf ("------------------------------\r\n");
                uart_printf ("[%S] result is ", DiagModules [i].Name);
            }

            if (FAILED(rc))
            {
                uart_printf ("{FAILED[rc=0x%08x]}\r\n", (UINT32)rc);
            }
            else
            {
                uart_printf ("{OK}\r\n");
            }

            if (nl)
                uart_printf ("\r\n");
        }

        if(DiagMask == NULL)
            return MX_OK;

        i++;
    }

    return MX_OK;
}


UINT32 DiagHandler(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    if (pParams->ParamCount == 0)
    {
        uart_printf("No parameters specified, must be one of:\n");
        uart_printf("\tready - send SVSR_READY\n");
        // TODO: Continue list
    }
    else
    {
        if (strcmp(pParams->ParamList[0], "ready") == 0)
        {
            uart_printf("Sending SVSR_READY...\n");
            return SvsrSendReady(MX_OK);
        }
        else
        {
            // TODO: More commands
        }
    }

    return MX_OK;
}

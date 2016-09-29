//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL schdiagctrl.c
 *
 * @brief The scheduler control API
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "config.h"
#include "schdiagctrl.h"
#include "supervisor.h"
#include "heaps.h"
#include <string.h>
#include <stdio.h>
#include "mlog.h"

LPVOID SchCtrlBuf = NULL;
UINT32 SchCtrlBufSize = 0;

static MXRC ProcCreateObj (ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * pInfo;

    pInfo = ApiGetParamByID(pApi, 2);

    if (pInfo == NULL)
        return RC_SCHDIAG_API_PARAM_ERR;

    if (pInfo->ParamData [0] == 1)
    {
        extern LPVOID SchCtrlBuf;
        extern UINT32 SchCtrlBufSize;

        ApiParam * pSize = ApiGetParamByID(pApi, 3);

        if (pSize == NULL)
            return RC_SCHDIAG_API_PARAM_ERR;

        if (SchCtrlBuf != NULL)
            MxHeapFree (AppHeap, SchCtrlBuf);

        SchCtrlBufSize = 0;

        SchCtrlBuf = MxHeapAlloc (AppHeap, pSize->ParamData [0]);

        if (SchCtrlBuf == NULL)
            return RC_SCHDIAG_NOT_MEMORY;

        SchCtrlBufSize = pSize->ParamData [0];

        ApiAddParam(pResp, NULL, 2, &SchCtrlBuf, sizeof (UINT32));

    }


    return MX_OK;

}

static MXRC ProcDestroyObj (ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * pInfo;

    pInfo = ApiGetParamByID(pApi, 2);

    if (pInfo == NULL)
        return RC_SCHDIAG_API_PARAM_ERR;

    if (pInfo->ParamData [0] == 1)
    {
        extern LPVOID SchCtrlBuf;
        extern UINT32 SchCtrlBufSize;

        if (SchCtrlBuf != NULL)
            MxHeapFree (AppHeap, SchCtrlBuf);

        SchCtrlBufSize = 0;
        SchCtrlBuf     = NULL;
    }

    return MX_OK;
}

APIHEC SchDiagApiHandler (LPVOID pData, ApiHeader * pApi, ApiHeader * pResp)
{
    if (IS_QRY_API (pApi, DIAG_SCH_GET_OBJ_INFO))
    {
        ApiParam * p = ApiGetParamByID(pApi, 2);

        if (p == NULL)
        {
            ApiAddErrorCode(pResp, RC_SCHDIAG_API_PARAM_ERR);
            return APIHEC_OK;
        }

        if (p->ParamData [0] == 0)
        {
            UINT32 addr = (UINT32)MLogGetFileLocation();
            UINT32 size = (UINT32)MLogGetFileSize();

            ApiAddParam(pResp, NULL, 3, &addr, sizeof (UINT32));
            ApiAddParam(pResp, NULL, 4, &size, sizeof (UINT32));
        }
        else if (p->ParamData [0] == 1)
        {
            UINT32 tmp;
            extern LPVOID SchCtrlBuf;
            extern UINT32 SchCtrlBufSize;

            tmp = (UINT32)SchCtrlBuf;

            ApiAddParam(pResp, NULL, 3, &tmp, sizeof (UINT32));
            ApiAddParam(pResp, NULL, 4, &SchCtrlBufSize, sizeof (UINT32));
        }
        else
        {
            ApiAddErrorCode(pResp, RC_SCHDIAG_API_PARAM_ERR);
            return APIHEC_OK;
        }

    }
    else if (IS_QRY_API (pApi, DIAG_SCH_READ))
    {
        ApiParam * pAddr;
        ApiParam * pSize;

        pAddr = ApiGetParamByID(pApi, 3);
        pSize = ApiGetParamByID(pApi, 4);

        if (pAddr == NULL || pSize == NULL)
        {
            ApiAddErrorCode(pResp, RC_SCHDIAG_API_PARAM_ERR);
            return APIHEC_OK;
        }

        ApiAddParam(pResp, NULL, 3, (LPVOID)pAddr->ParamData[0], pSize->ParamData[0]);
    }
    else if (IS_CFG_API (pApi, DIAG_SCH_WRITE))
    {
        ApiParam * pAddr;
        ApiParam * pData;

        pAddr = ApiGetParamByID(pApi, 3);
        pData = ApiGetParamByID(pApi, 4);

        if (pAddr == NULL || pData == NULL)
        {
            ApiAddErrorCode(pResp, RC_SCHDIAG_API_PARAM_ERR);
            return APIHEC_OK;
        }

        memcpy ((LPVOID)pAddr->ParamData [0], pData->ParamData, pData->ParamLength);
    }
    else if (IS_CFG_API(pApi, DIAG_SCH_CREATE_OBJ))
    {
        MXRC rc;

        rc = ProcCreateObj (pApi, pResp);

        if (FAILED(rc))
            ApiAddErrorCode(pResp, rc);
    }
    else if (IS_CFG_API(pApi, DIAG_SCH_DESTROY_OBJ))
    {
        MXRC rc;

        rc = ProcDestroyObj (pApi, pResp);

        if (FAILED(rc))
            ApiAddErrorCode(pResp, rc);
    }
    else
    {
        ApiAddErrorCode (pResp, RC_SCHDIAG_UNKNOW_API);
    }

    return APIHEC_OK;
}

MXRC SchDiagInit (UINT32* pReportData, UINT32* pReportSize)
{
	MXRC rc = MX_OK;

    PSchDiagCtx pCtx;

    if ((pCtx = (PSchDiagCtx)MxHeapAlloc(AppHeap, sizeof (SchDiagCtx))) == NULL)
        return RC_SCHDIAG_CTX_ALLOC_ERROR;

    memset(pCtx, 0, sizeof (SchDiagCtx));

    rc = SvsrRegClient(IID_SCH_CONTROL, SchDiagApiHandler, pCtx);

    _ASSERT_RC(rc);

    return rc;
}


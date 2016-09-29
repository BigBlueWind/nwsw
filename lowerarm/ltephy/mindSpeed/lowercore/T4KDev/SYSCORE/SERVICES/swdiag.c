//-------------------------------------------------------------------------------------------
/** @file swdiag.c
 *
 * @brief Turner Software Diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.11 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "swdiag.h"
#include "heaps.h"
#include "appprintf.h"

#ifdef SWDIAG_EN

static SWDIAGCTX SWDiagCtx[MEMBERS_LIST_SIZE];

MXRC SWDiagFreeResource(PSWDIAGCTX pSwdiagctx);
APIHEC SWDiagAPIHandler(LPVOID pClientCtx,  ApiHeader * pApi, ApiHeader * pResp);

/** @brief This function returns the pointer to module's context which corresponds module's name
 * 
 *  @param unsigned char* p_modulename - the pointer to the string with module's name
 * 
 *  @return value - the pointer to module's context
 */
PSWDIAGCTX SWDiagGetCtx(UINT32 nModID)
{
    UINT32 i = 0;
    
    if (nModID >= MEMBERS_LIST_SIZE)
        return NULL;

    while(i < MEMBERS_LIST_SIZE)
    {
        if (SWDiagCtx[i].ModID == nModID)
            return &SWDiagCtx[i];
         
        i++;
    }

    return NULL;
}

SWDIAGCTX * SWDiagGetFreeModule (void)
{
    UINT32 i = 0;

    while (i < MEMBERS_LIST_SIZE)
    {
        if (SWDiagCtx[i].ModID == SWDIAG_MOD_ID_RES)
             return &SWDiagCtx[i];

        i ++;
     }

    return NULL;
}

/** @brief This function initializes Turner Software Diagnostics module and inserts its name and pointer to context into the module's table
 * 
 *  @param PSWDIAGCTX pSwdiagctx - pointer to module's context
 * 
 *  @param PSWDIAGINITDATA pSwdiagInitdata - pointer to initial structure
 * 
 *  @returns MXRC value 
 */
MXRC SWDiagInit (PSWDIAGCTX pSwdiagctx, PSWDIAGINITDATA pSwdiagInitdata)
{
	MXRC rc = MX_OK;
	
	if (pSwdiagctx == NULL)
	{
		uart_printf ("Turner Software Diagnostics: the pointer is NULL.\r\n");
		return RC_SWDIAG_ALLOC_ERR;
	}
	
	if (pSwdiagctx->bSwdiagInited == TRUE)
	{
		uart_printf ("Turner Software Diagnostics: the module has been initialized.\r\n");
		return rc;
	}

	// diagnostic's initialization
	pSwdiagctx->LogDataPtr = (unsigned char*)MxHeapAlloc(NcnbHeap, pSwdiagInitdata->LogDataSize);
    
	if (pSwdiagctx->LogDataPtr == NULL)
	{
		uart_printf ("Turner Software Diagnostics: cannot allocate the buffer.\r\n");
		return RC_SWDIAG_ALLOC_ERR;
	}
    
	memset (pSwdiagctx->LogDataPtr, 0x0, pSwdiagInitdata->LogDataSize);

    pSwdiagctx->ModID           = pSwdiagInitdata->ModID;
    pSwdiagctx->LogDataSize     = pSwdiagInitdata->LogDataSize;
    pSwdiagctx->bSWDiagEn       = pSwdiagInitdata->bSWDiagEn;
    pSwdiagctx->LogGet          = 0;
    pSwdiagctx->LogPut          = 0;
    pSwdiagctx->bSWDiagEn       = pSwdiagInitdata->bSWDiagEn;
    pSwdiagctx->bSwdiagInited   = TRUE;
	
	return rc;
}

/** @brief This function releases allocated resources and removes the pointer to its context and its name from the module’s table
 * 
 *  @param PSWDIAGCTX pSwdiagctx - pointer to module's context
 * 
 *  @returns MXRC value
 */
MXRC SWDiagClose(PSWDIAGCTX pSwdiagctx)
{
	return SWDiagFreeResource(pSwdiagctx);
}

/** @brief This function initializes Turner Software Diagnostics module but does not insert its name and pointer to context
 *  into the module's table
 * 
 *  @param PSWDIAGCTX pSwdiagctx - pointer to module's context
 * 
 *  @param PSWDIAGINITDATA pSwdiagInitdata - pointer to initial structure
 * 
 *  @returns MXRC value 
 */
MXRC SWDiagAllocResource(PSWDIAGCTX pSwdiagctx, PSWDIAGINITDATA pSwdiagInitdata)
{
	return SWDiagInit(pSwdiagctx, pSwdiagInitdata);
}

/** @brief This function releases allocated resources but does not remove the pointer to its context and its name from the module’s table
 * 
 *  @param PSWDIAGCTX pSwdiagctx - pointer to module's context
 * 
 *  @returns MXRC value
 */
MXRC SWDiagFreeResource(PSWDIAGCTX pSwdiagctx)
{
	MXRC rc = MX_OK;
		
	if (pSwdiagctx == NULL)
	{
		uart_printf ("Turner Software Diagnostics: the pointer is NULL.\n");
		return RC_SWDIAG_ERROR;
	}
		
	if (pSwdiagctx->bSwdiagInited == FALSE)
	{
		uart_printf ("Turner Software Diagnostics: the module has not been initialised yet.\n");
		return rc;
	}
	
	if (pSwdiagctx->LogDataPtr != NULL)
	    MxHeapFree (NcnbHeap, pSwdiagctx->LogDataPtr);
    
	memset (pSwdiagctx, 0, sizeof (*pSwdiagctx));
			
	return rc;
}

MXRC SWDiagEnable(PSWDIAGCTX pSwdiagctx)
{	
	if (pSwdiagctx == NULL)
	{
		uart_printf ("Turner Software Diagnostics: the pointer is NULL.\n");
		return RC_SWDIAG_ERROR;
	}
	
	if (pSwdiagctx->bSwdiagInited == FALSE)
	{
		uart_printf ("Turner Software Diagnostics: the module is not initialized.\n");
		return RC_SWDIAG_ERROR;
	}
	
	pSwdiagctx->bSWDiagEn = TRUE;
	
	return MX_OK;
}

MXRC SWDiagDisable(PSWDIAGCTX pSwdiagctx)
{
	if (pSwdiagctx == NULL)
	{
		uart_printf ("Turner Software Diagnostics: the pointer is NULL.\n");
		return RC_SWDIAG_ERROR;
	}
		
	if (pSwdiagctx->bSwdiagInited == FALSE)
	{
		uart_printf ("Turner Software Diagnostics: the module is not initialized.\n");
		return RC_SWDIAG_ERROR;
	}
		
	pSwdiagctx->bSWDiagEn = FALSE;
	
	return MX_OK;
}

MXRC SWDiagnosticsInit(UINT32* pReportData, UINT32* pReportSize)
{
	MXRC rc = MX_OK;

	// registration of SWDiag API Handler
	rc = SvsrRegClient(IID_SWDIAG, SWDiagAPIHandler, NULL);

	if (rc != MX_OK)
	{
		uart_printf ("Turner Software Diagnostics: cannot add API handler or API handler has been already added.\n");
		return rc;
	}

	memset (SWDiagCtx, 0x0, sizeof(SWDiagCtx));
	return rc;
}

MXRC SWDiagnosticsDeinit(void)
{
	MXRC rc = MX_OK;
	
	SvsrRemClient(IID_SWDIAG);
	uart_printf ("Turner Software Diagnostics: API handler removed.\n");
	
	return rc;
}

APIHEC SWDiagAPIHandler(LPVOID pClientCtx,  ApiHeader * pApi, ApiHeader * pResp)
{
	MXRC rc = MX_OK;
	UINT32 ModID, nOffs, nSize, get;
	ApiParam * pParam;
    PSWDIAGCTX pSwdiagctx;
    PUINT8 pApiData;

	SWDIAGINITDATA mInitdata = {0,0,0};
    
	switch (pApi->MessageID)
	{
	case DIAG_STREAM_ENA:
		
		pParam = ApiGetParamByID(pApi, 2);

        if (pParam == NULL)
        {
            uart_printf ("SWDiag: Module ID error\r\n");
            ApiAddErrorCode (pResp, RC_SWDIAG_NOT_FOUND);
            return APIHEC_OK;
        }

        pSwdiagctx = SWDiagGetCtx(pParam->ParamData[0]);
        _ASSERT_PTR(pSwdiagctx);
        
    	rc = SWDiagEnable (pSwdiagctx);
		ApiAddErrorCode (pResp, rc);
        
		break;
		
	case DIAG_STREAM_DIS:
		
		pParam = ApiGetParamByID(pApi, 2);

        if (pParam == NULL)
        {
            uart_printf ("SWDiag: Module ID error\r\n");
            ApiAddErrorCode (pResp, RC_SWDIAG_NOT_FOUND);
            return APIHEC_OK;
        }

        pSwdiagctx = SWDiagGetCtx(pParam->ParamData[0]);
        _ASSERT_PTR(pSwdiagctx);
        
    	rc = SWDiagDisable(pSwdiagctx);
		ApiAddErrorCode (pResp, rc);
        
		break;
		
	case DIAG_BUFF_ACCESS:

        ModID = ApiGetParamU32(pApi, 2, MEMBERS_LIST_SIZE);

        if (ModID >= MEMBERS_LIST_SIZE)
        {
            uart_printf ("SWDiag: Error module ID\r\n");
            rc = RC_SWDIAG_MODULE_ERR;
            ApiAddErrorCode (pResp, rc);
            break;
        }

        pSwdiagctx = SWDiagGetCtx(ModID);
        _ASSERT_PTR(pSwdiagctx);

        if (pSwdiagctx == NULL)
        {
            uart_printf ("SWDiag: Error module ptr ID\r\n");
            rc = RC_SWDIAG_MODULE_ERR;
            ApiAddErrorCode (pResp, rc);
            break;
        }

        if (pSwdiagctx->bSwdiagInited == 0)
        {
            uart_printf ("SWDiag: Module [%d] is not inited\r\n", ModID);
            rc = RC_SWDIAG_MOD_INIT_ERR;
            ApiAddErrorCode (pResp, rc);
            break;
        }

        nOffs = ApiGetParamU32(pApi, 3, 0);
        nSize = ApiGetParamU32(pApi, 4, 0);
        nSize = MIN(nSize, 0x2FFF);

        uart_printf ("SWDiag: Query buffer mod[%d], nOffs=%d, nSize=%d\r\n", ModID, nOffs, nSize);

        get = pSwdiagctx->LogGet;

        if (pSwdiagctx->LogPut < get)
            get = pSwdiagctx->LogPut;

        // to skipp the the specific number of bytes

        while (nOffs != 0)
        {
            get ++;

            if (get >= pSwdiagctx->LogDataSize)
                get = 0;

            nOffs --;
        }

        // To allocate the buffer in API
        ApiAddParam(pResp, NULL, 2, NULL, nSize);

        if (ApiGetParamByID(pResp, 2) != NULL)
        {
            pApiData = (PUINT8)&(ApiGetParamByID(pResp, 2)->ParamData[0]);
        }
        else
        {
            pApiData = NULL;
            uart_printf ("SWDiag: Error to create API parameter\r\n");

            rc = RC_SWDIAG_BUF_ERR;
            ApiAddErrorCode (pResp, rc);
            break;
        }

        while (nSize -- )
        {
            *pApiData++ = pSwdiagctx->LogDataPtr[get++];

            if (get >= pSwdiagctx->LogDataSize)
                get = 0;
        }

        ApiAddErrorCode (pResp, rc);        
		break;
		
	case DIAG_IND:
        ModID = ApiGetParamU32(pApi, 2, MEMBERS_LIST_SIZE);

        if (ModID >= MEMBERS_LIST_SIZE)
        {
            uart_printf ("SWDiag: Error module ID [%x]\r\n", ModID);
            rc = RC_SWDIAG_MODULE_ERR;
            ApiAddErrorCode (pResp, rc);
            break;
        }
        pSwdiagctx = SWDiagGetCtx(ModID);
        _ASSERT_PTR(pSwdiagctx);

        if (pSwdiagctx == NULL)
        {
            uart_printf ("SWDiag: Error module ptr ID\r\n");
            rc = RC_SWDIAG_MODULE_ERR;
            ApiAddErrorCode (pResp, rc);
            break;
        }
        
        if (pSwdiagctx->bSwdiagInited == 0)
        {
            uart_printf ("SWDiag: Module [%d] is not inited\r\n", ModID);
            rc = RC_SWDIAG_MOD_INIT_ERR;
            ApiAddErrorCode (pResp, rc);
            break;
        }

        SWDiagPrintLogData(pSwdiagctx);

		
		break;
		
	case DIAG_API_TEST_INIT:

        mInitdata.ModID       = ApiGetParamU32(pApi, 2, SWDIAG_MOD_ID_TEST);
		mInitdata.LogDataSize = ApiGetParamU32(pApi, 3, 128);

        mInitdata.LogDataSize = MIN(mInitdata.LogDataSize, 0x2FFF);

        if (mInitdata.LogDataSize == 0)
        {
            uart_printf ("SWDiag:Error log data size (LogSize:%d)\r\n",mInitdata.LogDataSize);
            rc = RC_SWDIAG_MOD_INIT_ERR;
            ApiAddErrorCode (pResp, rc);
            break;
        }
            
        
		mInitdata.bSWDiagEn   = ApiGetParamU32(pApi, 4, 0);

        uart_printf ("SWDiag: Reg new module(name:%d, LogSize:%d, Ena:%d)\r\n", 
            mInitdata.ModID,
            mInitdata.LogDataSize,
            mInitdata.bSWDiagEn);

        pParam = ApiGetParamByID(pApi, 5);

    	if ((rc = SWDiagApiTestInit(&mInitdata, (pParam != NULL) ? (PUINT8)&(pParam->ParamData[0]) : 0, (pParam != NULL) ? (pParam->ParamLength): 0)) != MX_OK)
		{
			uart_printf ("Turner Software Diagnostics: API test module error, rc=0x%x\r\n", rc);
		}
        
		ApiAddErrorCode (pResp, rc);
		break;
	
	case DIAG_API_TEST_CLOSE:

        ModID = ApiGetParamU32(pApi, 2, MEMBERS_LIST_SIZE);

        if (ModID >= MEMBERS_LIST_SIZE)
        {
            uart_printf ("SWDiag: Error module ID\r\n");
            rc = RC_SWDIAG_MODULE_ERR;
            ApiAddErrorCode (pResp, rc);
            break;
        }

        pSwdiagctx = SWDiagGetCtx(ModID);
        _ASSERT_PTR(pSwdiagctx);

        if (pSwdiagctx == NULL)
        {
            uart_printf ("SWDiag: Error module ptr ID\r\n");
            rc = RC_SWDIAG_MODULE_ERR;
            ApiAddErrorCode (pResp, rc);
            break;
        }

		SWDiagClose(pSwdiagctx);
		uart_printf ("Turner Software Diagnostics: API test module closed.\r\n");
		ApiAddErrorCode (pResp, rc);
		break;
		
	default :
		uart_printf ("Turner Software Diagnostics: unknown Message ID.\r\n");
		ApiAddErrorCode (pResp, RC_SWDIAG_NOT_FOUND);
		break;
	}
	
	return APIHEC_OK;
}

void SWDiagAddLogData(PSWDIAGCTX pSwdiagctx, LPVOID data, UINT32 data_size)
{
    UINT32 i = 0;

    _ASSERT_PTR(pSwdiagctx);

    if (pSwdiagctx->LogDataPtr == NULL)
        return;

    if (pSwdiagctx->bSWDiagEn == 0)
        return;

    while (data_size != 0)
    {
        pSwdiagctx->LogDataPtr[pSwdiagctx->LogPut++] = ((PUINT8)data)[i++];

        if (pSwdiagctx->LogPut >= pSwdiagctx->LogDataSize)
            pSwdiagctx->LogPut = 0;

        if (pSwdiagctx->LogPut == pSwdiagctx->LogGet)
        {
            pSwdiagctx->LogGet ++;

            if (pSwdiagctx->LogGet >= pSwdiagctx->LogDataSize)
                pSwdiagctx->LogGet = 0;
        }
    
        data_size --;
    }
}

void SWDiagPrintLogData(PSWDIAGCTX pSwdiagctx)
{
    extern void DumpProc (UINT32 nAddr, UINT32 nSize);

     _ASSERT_PTR(pSwdiagctx);

    if (pSwdiagctx->LogDataPtr == NULL)
        return;

    if (pSwdiagctx->bSWDiagEn == 0)
        return;

    DumpProc((UINT32)pSwdiagctx->LogDataPtr + pSwdiagctx->LogGet, pSwdiagctx->LogDataSize - pSwdiagctx->LogGet);
    DumpProc((UINT32)pSwdiagctx->LogDataPtr, pSwdiagctx->LogGet);
    
}

MXRC SWDiagApiTestInit(PSWDIAGINITDATA pSwdiagInitdata, PUINT8 pData, UINT32 fillSize)
{
	MXRC rc = MX_OK;
	UINT32 i;

    PSWDIAGCTX pMod;

	if (SWDiagGetCtx(pSwdiagInitdata->ModID) != NULL)
		return rc;
    
	uart_printf("SWDiag: To create new module [%d]\r\n", pSwdiagInitdata->ModID);

    pMod = SWDiagGetFreeModule();

    if (pMod == NULL)
    {
        uart_printf ("SWDiag: No free storage or new module\r\n");
        return RC_SWDIAG_REG_MOD_ERR;
    }
    
    rc = SWDiagInit (pMod, pSwdiagInitdata);
    
	if (FAILED(rc))
	{
	    uart_printf ("SWDiag: Module init error, rc=0x%x\r\n", rc);
		return rc;
	}

    if (pData != NULL && pMod->ModID == SWDIAG_MOD_ID_TEST)
    {
    	for (i = 0; i < fillSize; i++)
    	{
    		SWDiagAddLogData(pMod, &pData[i], sizeof(UINT8));
    	}
    }
	
	return rc;
}


#endif


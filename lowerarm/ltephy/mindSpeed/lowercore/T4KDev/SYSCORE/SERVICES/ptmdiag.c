//-------------------------------------------------------------------------------------------
/** @file ptmdiag.c
 *
 * @brief Turner Software Diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------
#ifdef PTM_ENABLED
#include "supervisor.h"
#include "ptmdiag.h"
#include "coresight.h"

MXRC  PtmDiagInit (void)
{
    PtmDiagCtx* pCtx;
    MXRC rc = MX_OK;
	
    trace_init();
	
    pCtx = (PtmDiagCtx*)MxHeapAlloc(AppHeap, sizeof (PtmDiagCtx));
    if (pCtx == NULL)
        return RC_PTMDIAG_ALLOC_ERR;

    memset (pCtx, 0, sizeof (PtmDiagCtx));
    pCtx->hHeap = DDR1NcnbHeap;
	pCtx->EtbBufSz = trace_getbufsz();
    rc = SvsrRegClient(IID_PTMDIAG, PtmDiagApiHandler, pCtx);

    _ASSERT_RC(rc);

    return rc;
}

MXRC PtmDiagConfig (PPtmDiagCtx pCtx, ApiHeader * pApi)
{
    MXRC          rc = RC_PTMDIAG_OK;
    ApiParam *    pParam = NULL;
	PPtmConfig    pstPtmCfg = NULL;
	UINT8         ucPtmID;
	

	pParam = ApiGetParamByID(pApi, PTMID_PARAID);

	if(pParam == NULL)
	{
	    rc = RC_PTMDIAG_NO_PTMID;
		return rc;
	}

	ucPtmID = pParam->ParamData[0];
    if(ucPtmID < MAX_PTM)
    {
        pstPtmCfg = trace_getptmcfg(ucPtmID);
    }
    else
    {
    	pstPtmCfg = trace_getptmcfg(0);
    }

	pParam = NULL;	     
	while ((pParam = ApiGetNextParam(pApi, pParam)) != NULL)
    {
        switch(pParam->ParamID)
        {
		    case TRACE_BLOCK_SIZE_PARAID:
				pCtx->TraceBlockSize = pParam->ParamData[0];
				break;

			case TRACE_REQUEST_PARAID:
				break;

			case POWER_DOWN_PARAID:
				pstPtmCfg->PowerDown = (UINT8)(pParam->ParamData[0] & 0x1);
				break;

			case BRANCH_OUTPUT_PARAID:
				pstPtmCfg->BranchOutput = (UINT8)(pParam->ParamData[0] & 0x1);
				break;
							
			case CYCLE_ACCURATE_PARAID:
				pstPtmCfg->CycleAccurate = (UINT8)(pParam->ParamData[0] & 0x1);
				break;

			case CONTEXTID_SIZE_PARAID:
				pstPtmCfg->ContextIDSize = (UINT8)(pParam->ParamData[0] & 0x3);
				break;

			case TIMESTAMP_ENABLE_PARAID:
				pstPtmCfg->TimestampEnable = (UINT8)(pParam->ParamData[0] & 0x1);
				break;

			case RETURN_STACK_PARAID:
				pstPtmCfg->ReturnStackEnable = (UINT8)(pParam->ParamData[0] & 0x1);
				break;

			case TIMESTAMP_EVENT_PARAID:
				pstPtmCfg->TimestampEvent = pParam->ParamData[0];
				break;

			case SYNC_FREQ_PARAID:
				pstPtmCfg->SyncFreq = pParam->ParamData[0];
				break;

			case FOREC_SYNC_PACKET_PARAID:
				pstPtmCfg->ForceSyncPacket = (UINT8)(pParam->ParamData[0] & 1);
				break;

			case ADDR_COMP_VAL1_PARAID:
				pstPtmCfg->AddrCompVal[0] = pParam->ParamData[0];
				break;

			case ADDR_COMP_VAL2_PARAID:
				pstPtmCfg->AddrCompVal[1] = pParam->ParamData[0];
				break;

			case ADDR_COMP_VAL3_PARAID:
				pstPtmCfg->AddrCompVal[2] = pParam->ParamData[0];
				break;

			case ADDR_COMP_VAL4_PARAID:
				pstPtmCfg->AddrCompVal[3] = pParam->ParamData[0];
				break;

			case ADDR_COMP_VAL5_PARAID:
				pstPtmCfg->AddrCompVal[4] = pParam->ParamData[0];
				break;

			case ADDR_COMP_VAL6_PARAID:
				pstPtmCfg->AddrCompVal[5] = pParam->ParamData[0];
				break;

			case ADDR_COMP_VAL7_PARAID:
				pstPtmCfg->AddrCompVal[6] = pParam->ParamData[0];
				break;

			case ADDR_COMP_VAL8_PARAID:
				pstPtmCfg->AddrCompVal[7] = pParam->ParamData[0];
				break;

			case CONTEXTID_COMP_VAL_PARAID:
				pstPtmCfg->ContextIDCompVal = pParam->ParamData[0];
				break;

			case CONTEXTID_COMP_MASK_PARAID:
				pstPtmCfg->ContextIDCompMask = pParam->ParamData[0];
				break;

			case COUNTER_VAL1_PARAID:
				pstPtmCfg->CounterVal[0] = pParam->ParamData[0];
				break;

			case COUNTER_VAL2_PARAID:
				pstPtmCfg->CounterVal[1] = pParam->ParamData[0];
				break;

			case COUNTER_ENABLE1_PARAID:
				pstPtmCfg->CounterEnable[0] = pParam->ParamData[0];
				break;

			case COUNTER_ENABLE2_PARAID:
				pstPtmCfg->CounterEnable[1] = pParam->ParamData[0];
				break;

			case COUNTER_RELOAD_VAL1_PARAID:
				pstPtmCfg->CounterReloadVal[0] = pParam->ParamData[0];
				break;

			case COUNTER_RELOAD_VAL2_PARAID:
				pstPtmCfg->CounterReloadVal[1] = pParam->ParamData[0];
				break;

			case COUNTER_RELOAD_EVT1_PARAID:
				pstPtmCfg->CounterReloadEvt[0] = pParam->ParamData[0];
				break;

			case COUNTER_RELOAD_EVT2_PARAID:
				pstPtmCfg->CounterReloadEvt[1] = pParam->ParamData[0];
				break;

			case SEQUENCER_STATE_PARAID:
				pstPtmCfg->SeqState = (UINT8)(pParam->ParamData[0] & 3);
				break;

			case SEQ_TRAN_EVT_1TO2_PARAID:
				pstPtmCfg->SeqTranEvt1To2 = pParam->ParamData[0];
				break;

			case SEQ_TRAN_EVT_1TO3_PARAID:
				pstPtmCfg->SeqTranEvt1To3 = pParam->ParamData[0];
				break;

			case SEQ_TRAN_EVT_2TO1_PARAID:
				pstPtmCfg->SeqTranEvt2To1 = pParam->ParamData[0];
				break;

			case SEQ_TRAN_EVT_2TO3_PARAID:
				pstPtmCfg->SeqTranEvt2To3 = pParam->ParamData[0];
				break;

			case SEQ_TRAN_EVT_3TO1_PARAID:
				pstPtmCfg->SeqTranEvt3To1 = pParam->ParamData[0];
				break;

			case SEQ_TRAN_EVT_3TO2_PARAID:
				pstPtmCfg->SeqTranEvt3To2 = pParam->ParamData[0];
				break;

			case TRACE_ENABLE_START_PARAID:
				pstPtmCfg->TraceEnableStart = (UINT16)(pParam->ParamData[0] & 0xFFFF);
				break;

			case TRACE_ENABLE_STOP_PARAID:
				pstPtmCfg->TraceEnableStop = (UINT16)(pParam->ParamData[0] & 0xFFFF);
				break;

			case TRACE_ENABLE_ACTIVE_PARAID:
				pstPtmCfg->TraceEnableActive = (UINT8)(pParam->ParamData[0] & 1);
				break;

			case TRACE_RANGE_PARAID:
				pstPtmCfg->TraceRange = (UINT16)(pParam->ParamData[0] & 0xFFFF);
				break;

			case TRACE_RANGE_EXCLUDE_PARAID:
				pstPtmCfg->TraceRangeExclude = (UINT8)(pParam->ParamData[0] & 1);
				break;

			case TRACE_ENABLE_EVT_PARAID:
				pstPtmCfg->TraceEnableEvt = pParam->ParamData[0];
				break;

			default:
			    break;
        }
    }

	if(ucPtmID == ENABLE_ALL_PTM)
    {
	    UINT8  i;
		
		for(i=1; i<MAX_PTM; i++)
		{
		    trace_configure(i, pstPtmCfg);
		}
    }

	return rc;
	
}

MXRC PtmDiagStreamEnable(ApiHeader * pApi)
{
    UINT8         i,ucPtmID;
	ApiParam *    pParam = NULL;
	MXRC          rc = RC_PTMDIAG_OK;
	

	pParam = ApiGetParamByID(pApi, PTMID_PARAID);

	if(pParam == NULL)
	{
	    rc = RC_PTMDIAG_NO_PTMID;
		return rc;
	}

	ucPtmID = pParam->ParamData[0];

    if(ucPtmID == ENABLE_ALL_PTM)
	{
	    for(i=0; i<MAX_PTM; i++)
		{
		    trace_enable(i, TRUE);
		}
	}
	else
	{
	    trace_enable(ucPtmID, TRUE);
	}

	return rc;
}

MXRC PtmDiagStreamDisable(ApiHeader * pApi)
{
    UINT8         i,ucPtmID;
	ApiParam *    pParam = NULL;
	MXRC          rc = RC_PTMDIAG_OK;
	

	pParam = ApiGetParamByID(pApi, PTMID_PARAID);

	if(pParam == NULL)
	{
	    rc = RC_PTMDIAG_NO_PTMID;
		return rc;
	}

	ucPtmID = pParam->ParamData[0];

    if(ucPtmID == ENABLE_ALL_PTM)
	{
	    for(i=0; i<MAX_PTM; i++)
		{
		    trace_enable(i, FALSE);
		}
	}
	else
	{
	    trace_enable(ucPtmID, FALSE);
	}

	return rc;
}

MXRC PtmDiagEtbBufAccess(PPtmDiagCtx pCtx, ApiHeader * pApi)
{
    MXRC          rc = RC_PTMDIAG_OK;
	
    pCtx->EtbBufPtr = MxHeapAlloc(pCtx->hHeap, pCtx->EtbBufSz);

	if(pCtx->EtbBufPtr == NULL)
		return RC_PTMDIAG_ALLOC_ERR;
					
	trace_access((char *)pCtx->EtbBufPtr, pCtx->EtbBufSz);

	ApiAddParam(pApi, NULL, TRACE_DATA_PARAID, pCtx->EtbBufPtr, pCtx->EtbBufSz);

	#if 0
	uart_printf("ID=0x%x, value=0x%x\r\n", pCtx->EtbBufPtr, pCtx->EtbBufSz);
	#else
	MxHeapFree(pCtx->hHeap, pCtx->EtbBufPtr);
	#endif

	return rc;
}



APIHEC PtmDiagApiHandler (LPVOID pData, ApiHeader * pApi, ApiHeader * pResp)
{
    MXRC          rc = RC_PTMDIAG_OK;
	PPtmDiagCtx pCtx = (PPtmDiagCtx)pData;

    if (pApi->Type == API_TYPE_CFG_PARAM)
    {
        pResp->Type = API_TYPE_CFG_RESP;

        switch (pApi->MessageID)
        {
            case DIAG_CONFIGURE:
            {  
				rc = PtmDiagConfig(pCtx, pApi);
				
                break;
            }
            case DIAG_STREAM_ENA:
            {
				rc = PtmDiagStreamEnable(pApi);
				
                break;
            }
            case DIAG_STREAM_DIS:
            {
				rc = PtmDiagStreamDisable(pApi);
				
                break;
            }
            default:
            {
                rc = RC_PTMDIAG_UNKNOWN_API;
                break;
            }
        }
    }
    else if (pApi->Type == API_TYPE_QRY_PARAM)
    {
        pResp->Type = API_TYPE_QRY_RESP;

        switch (pApi->MessageID)
        {
            case DIAG_BUFF_ACCESS:
            {
                rc = PtmDiagEtbBufAccess(pCtx, pResp);
                break;
            }

            default:
            {
                rc = RC_PTMDIAG_UNKNOWN_API;
                break;
            }
        }

        if (SUCCESSFUL(rc))
            return APIHEC_OK;

    }
    else
    {
        rc = RC_PTMDIAG_UNSUP_TYPE; 
    }

    ApiAddErrorCode(pResp, rc);

    return APIHEC_OK;
}
#endif



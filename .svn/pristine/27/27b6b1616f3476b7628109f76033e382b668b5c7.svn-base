/**
 * @file    pmh_set_table.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions which handles the SET requests.
 *
 * @author:  Ankur Agrawal
 * @date:    22-Aug-2012
 * @details: Initial Version
 **/
#include "pmh.h"

/**
 * @param   setReq      SET_OBJ_REQ
 * @param   bsErrorValue  error number
 * @param   errTagInfo    error tag info
 * @brief This function will create and send the SET_OBJ_FAIL to BSM.
 **/
void pmhFillAndSendSetObjFail(
  IN SetObjReq          *setReq,
  IN BsErrorValue        bsErrorValue,
  IN TagInfo            *errTagInfo
  )
{
  SetObjRspFail   rspFail;
  EncBuf          encBuf;
  initEncBuf( &encBuf );
  clearBytes(&rspFail, sizeof(SetObjRspFail));

  copyBytes( &(setReq->cmdRef), sizeof(CmdRef), &(rspFail.cmdRef));
  copyBytes( &(setReq->tlvArray), sizeof(TlvArray), &(rspFail.tlvArray));
  rspFail.setErrRsp.errRspCode = bsErrorValue;
  if ( errTagInfo NOTEQ NULL )
  {
    copyBytes( errTagInfo, sizeof(TagInfo), &(rspFail.setErrRsp.failedTagInfo));
  }
  encSetObjRspFailMsg( &rspFail, &encBuf );

  createAndSendMsg( MSG_PMH_BSM_SET_OBJ_FAIL, (UINT8*)&encBuf.value, encBuf.len );
}


/**
 * @param   setReq      SET_OBJ_REQ
* @brief This function will create and send the SET_OBJ_SUCC to BSM.
 **/
void pmhFillAndSendSetObjSucc(
  IN SetObjReq   *setObj
  )
{
  SetObjRspSucc   rspSucc;
  EncBuf          encBuf;
  initEncBuf( &encBuf );
  clearBytes( &rspSucc, sizeof(SetObjRspSucc) );

  /* Fill the Rsp with the same request received */
  copyBytes( setObj, sizeof(SetObjReq), &rspSucc );
  /* Encode the Response message */
  encSetObjRspSuccMsg( &rspSucc, &encBuf );

  createAndSendMsg( MSG_PMH_BSM_SET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);
}

/**
 * @param   setReq      SET_OBJ_REQ
 * @param   pmhContext  PMH context
 * @brief This function will process the SET_OBJ_REQ on CounterConfig table.
 **/
void pmhProcSetReqOnCounterConfigTable(
  IO SetObjReq  *setReq,
  IO PmhContext *pmhContext
  )
{
  UINT32 tlvIndex;
  RowId rowId;
  ProfileId profileId;
  CounterId counterId;

  /* No need to update the configuration if no configuration received till now
   * On OPSTART PMH will receive the whole configuration again */
  if (FALSE EQ pmhContext->isOpStartRcvd)
  {
    logInfo("%s", "Config change is received in LOCK state so PMH is not updating its Context");
    pmhFillAndSendSetObjSucc(setReq);
    return;
  }

  rowId     = setReq->tlvArray.tlvInfo[0].tag.rowId;
  profileId = pmhContext->counterContext[rowId-1].l2CounterConfig.profileId;
  counterId = pmhContext->counterContext[rowId-1].l2CounterConfig.counterId;

  if(TRUE EQ pmhContext->counterProfileContext[profileId-1].isProfileRunning)
  {
    BS_ERROR_CONFIG_CHANGE_RCVD_WHILE_REQUESTED_PROFILE_IS_RUNNING(profileId, counterId);
    pmhFillAndSendSetObjFail(setReq,BS_ERR_CONFIG_CHANGE_NOT_ALLOWED_WHILE_REQUESTED_PROFILE_IS_RUNNING,
                           &(setReq->tlvArray.tlvInfo[0].tag));
    return;
  }

  for(tlvIndex=0; tlvIndex<setReq->tlvArray.numTlvs; tlvIndex++)
  {
    if(setReq->tlvArray.tlvInfo[tlvIndex].tag.colId EQ COL_COUNTERCONFIG_ISENABLED)
    {
      pmhContext->counterContext[rowId-1].l2CounterConfig.isEnabled  =  getUint8FromArray(setReq->tlvArray.tlvInfo[tlvIndex].data);
      logInfo("isEnabled value of CounterId = %s is updated to %d", getCounterIdStr(counterId), pmhContext->counterContext[rowId-1].l2CounterConfig.isEnabled);
    }
  }

  pmhFillAndSendSetObjSucc( setReq );
  return;
}

/**
 * @param   reportingDuration      reporting interval
 * @brief This function will convert reporting interval ENUM to value in seconds.
 **/
UINT32 getReportingPeriodInSec(
  IN ReportingDuration   reportingDuration
  )
{
  switch(reportingDuration)
  {
    case REPORTINGDURATION_2MINS:    return 2*60;
    case REPORTINGDURATION_15MINS:   return 15*60;
    case REPORTINGDURATION_30MINS:   return 30*60;
    case REPORTINGDURATION_1HOUR:    return 1*60*60;
    case REPORTINGDURATION_2HOUR:    return 2*60*60;
    default:
      BS_ERROR_UNKNOWN_ENUM_VALUE("Unknown ReportingDuration", reportingDuration);
      return 0;
  }
}

/**
 * @param   measurementDuration      measurement interval
 * @brief This function will convert measurement interval ENUM to value in seconds.
 **/
UINT32 getMeasurementDurationInsecs(
  IN MeasurementDuration   measurementDuration
  )
{
  switch(measurementDuration)
  {
    case MEASUREMENTDURATION_2MINS:    return 2*60;
    case MEASUREMENTDURATION_15MINS:   return 15*60;
    case MEASUREMENTDURATION_30MINS:   return 30*60;
    case MEASUREMENTDURATION_1HOUR:    return 1*60*60;
    case MEASUREMENTDURATION_2HOUR:    return 2*60*60;
    case MEASUREMENTDURATION_INFINITE: return 0;
    default:
      BS_ERROR_UNKNOWN_ENUM_VALUE("Unknown MeasurementDuration", measurementDuration);
      return 0;
  }
}

/**
 * @param   counterControl  Counter Control message
 * @param   pmhContext      PMH context
 * @param   setReq          SET_OBH_REQ received from BSM
 * @brief This function will send COUNTER_START_REQ to UEH and start the respective
 *        timer to guard the procedure.
 **/
BsErrorValue pmhFillAndSendCounterStartReqAndStartTimer(
  IO CounterControl *counterControl,
  IO PmhContext      *pmhContext,
  IO SetObjReq       *setReq
  )
{
  BsErrorValue error = BS_NO_ERROR;

  UINT32 index;
  UINT32 samplingIntervalInSec=0;
  UINT32 measurementDurationInsecs=0;
  UINT32 reportingDurationInsecs=0;
  TimerId timerId;
  MsgPmhUehCounterStartReq startReq;
  CounterProfileContext     *pCtx = getCounterProfileCtx(counterControl->profileId);

  clearBytes(&startReq, sizeof(MsgPmhUehCounterStartReq));

  /* Allocate transId and store CmdRef and TagInfo*/
  error = pmhAllocTransId(&setReq->cmdRef, &(setReq->tlvArray.tlvInfo[0].tag), &startReq.transId, pmhContext);
  if(BS_NO_ERROR NOTEQ error)
  {
    BS_ERROR_TRANSID_ALLOC_FAILED();
    return error;
  }

  /* Filling the MsgPmhUehCounterStartReq message */
  for(index=0;index<MAX_ROWS_IN_COUNTERCONFIG_TABLE;index++)
  {
    if((pmhContext->counterContext[index].l2CounterConfig.profileId EQ counterControl->profileId) &&
       (pmhContext->counterContext[index].l2CounterConfig.isEnabled EQ TRUE))
    {
      startReq.counterId[startReq.numOfCounters] = pmhContext->counterContext[index].l2CounterConfig.counterId;
      startReq.numOfCounters++;
    }
  }

  /* If none of the counter is configured then reject the request */
  if(0 EQ startReq.numOfCounters)
  {
    BS_ERROR_NO_COUNTER_IS_CONFIGURED_FOR_REQUESTED_PROFILE(pCtx->profileId);
    return BS_ERR_NO_COUNTER_IS_CONFIGURED_FOR_REQUESTED_PROFILE;
  }

  startReq.profileId     = counterControl->profileId;

  if(PROFILEID_PROFILE1 EQ startReq.profileId)
  {
    startReq.samplingInterval = SAMPLING_INTEVAL_PROFILE1;
    timerId = TIMER_INDEX_WAIT_L2PMCOUNTERCONTROLPROFILE1;
  }
  else if(PROFILEID_PROFILE2 EQ startReq.profileId)
  {
    startReq.samplingInterval = SAMPLING_INTEVAL_PROFILE2;
    timerId = TIMER_INDEX_WAIT_L2PMCOUNTERCONTROLPROFILE2;
  }

  /* If measurement duration is infinite then number of samples will be 0 */
  samplingIntervalInSec     = getSamplingIntervalInSec(startReq.samplingInterval);
  measurementDurationInsecs = getMeasurementDurationInsecs(counterControl->measurementDuration);
  reportingDurationInsecs   = getReportingPeriodInSec(counterControl->reportingDuration);
  startReq.numOfSamples     = measurementDurationInsecs / samplingIntervalInSec;

  /* Start the WAIT_COUNTER_CONTROL timer */
  if(NULL_TIMER_ID EQ pmhStartTimer(timerId, startReq.transId, 0))
  {
    BS_ERROR_PMH_TIMER_NOT_STARTED("WAIT_COUNTER_CONTROL");
    return BS_ERR_INTERNAL_ERROR_OCCURED;
  }
  createAndSendMsg( MSG_PMH_UEH_COUNTER_START_REQ, (UINT8 *)&startReq, sizeof(MsgPmhUehCounterStartReq) );

  /* Updating the profile context */
  pCtx->isProfileRunning                       = TRUE;
  pCtx->reportingDuration                      = counterControl->reportingDuration;
  pCtx->measurementDuration                    = counterControl->measurementDuration;
  pCtx->numOfSamplesRcvd                       = 0;
  pCtx->numOfSamplesRemaining                  = startReq.numOfSamples;
  pCtx->lastSeqNum                             = 0;
  pCtx->totalSamplePerReportingPeriod          = reportingDurationInsecs/samplingIntervalInSec;
  pCtx->numOfSamplesRemainingOfReportingPeriod = pCtx->totalSamplePerReportingPeriod;

  pmhLogProfileContext(pCtx, "Counter Profile Context after sending COUNTER_START_REQ to UEH");
  return BS_NO_ERROR;
}

/**
 * @param   counterControl  Counter Control message
 * @param   pmhContext      PMH context
 * @param   setReq          SET_OBH_REQ received from BSM
 * @brief This function will send COUNTER_STOP_REQ to UEH and start the respective
 *        timer to guard the procedure.
 **/
BsErrorValue pmhFillAndSendCounterStopReqAndStartTimer(
  IO CounterControl *counterControl,
  IO PmhContext      *pmhContext,
  IO SetObjReq       *setReq
  )
{
  BsErrorValue error = BS_NO_ERROR;
  TimerId timerId;
  MsgPmhUehCounterStopReq stopReq;

  clearBytes(&stopReq, sizeof(MsgPmhUehCounterStopReq));

  /* Allocate transId and store CmdRef and TagInfo*/
  error = pmhAllocTransId(&setReq->cmdRef, &setReq->tlvArray.tlvInfo[0].tag, &stopReq.transId, pmhContext);
  if(BS_NO_ERROR NOTEQ error)
  {
    BS_ERROR_TRANSID_ALLOC_FAILED();
    return error;
  }

  stopReq.profileId     = counterControl->profileId;

   /* start the timer */
  if(PROFILEID_PROFILE1 EQ stopReq.profileId)
    timerId = TIMER_INDEX_WAIT_L2PMCOUNTERCONTROLPROFILE1;
  else if(PROFILEID_PROFILE2 EQ stopReq.profileId)
    timerId = TIMER_INDEX_WAIT_L2PMCOUNTERCONTROLPROFILE2;

  if(NULL_TIMER_ID EQ pmhStartTimer(timerId, stopReq.transId, 0))
  {
    BS_ERROR_PMH_TIMER_NOT_STARTED("WAIT_COUNTER_CONTROL");
    return BS_ERR_INTERNAL_ERROR_OCCURED;
  }

  pmhContext->counterProfileContext[counterControl->profileId-1].isProfileRunning = FALSE;

  createAndSendMsg( MSG_PMH_UEH_COUNTER_STOP_REQ, (UINT8 *)&stopReq, sizeof(MsgPmhUehCounterStopReq) );
  pmhClosePmLogFileAndUpdateStatusTablesAndStopTimerAndSendNoti(stopReq.profileId, OPERATOR_INITIATED, pmhContext);

  pmhLogProfileContext(&(pmhContext->counterProfileContext[counterControl->profileId-1]),
		                "Counter Profile Context after sending COUNTER_STOP_REQ to UEH");
  return BS_NO_ERROR;
}

/**
 * @param   setReq          SET_OBJ_REQ received from BSM
 * @param   pmhContext      PMH context
 * @brief This function will process the SET request on Counter Control table.
 **/
void pmhProcCounterControlReq(
  IO SetObjReq  *setReq,
  IO PmhContext *pmhContext
  )
{
  BsErrorValue  error = BS_NO_ERROR;

  TableId tableId = setReq->tlvArray.tlvInfo[0].tag.tableId;
  CounterControl  counterCtrl;
  GenericTable   *tableFunc = getTableFunctions(tableId);

  clearBytes(&counterCtrl, sizeof(CounterControl));


  /* Convert TLV to structure */
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setReq->tlvArray, &counterCtrl );
  }
  else
  {
    BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "COUNTERCONTROL" );
    pmhFillAndSendSetObjFail( setReq, BS_ERR_INTERNAL_ERROR_OCCURED, &setReq->tlvArray.tlvInfo[0].tag );
    return;
  }

  switch(counterCtrl.counterCommand)
  {
    case COUNTERCOMMAND_START:
      if(TRUE EQ pmhContext->counterProfileContext[counterCtrl.profileId -1].isProfileRunning)
      {
        pmhFillAndSendSetObjFail( setReq, BS_ERR_MEASUREMENT_IS_ALREADY_RUNNING_FOR_REQUESTED_PROFILE, &setReq->tlvArray.tlvInfo[0].tag );
        return;
      }

      error = pmhFillAndSendCounterStartReqAndStartTimer(&counterCtrl, pmhContext, setReq);
      if(BS_NO_ERROR NOTEQ error)
      {
        pmhFillAndSendSetObjFail( setReq, error, &setReq->tlvArray.tlvInfo[0].tag );
        return;
      }
      break;
    case COUNTERCOMMAND_STOP:
      if(FALSE EQ pmhContext->counterProfileContext[counterCtrl.profileId -1].isProfileRunning)
      {
        pmhFillAndSendSetObjFail( setReq, BS_ERR_NO_MEASUREMENT_IS_RUNNING_FOR_REQUESTED_PROFILE, &setReq->tlvArray.tlvInfo[0].tag );
        return;
      }

      error = pmhFillAndSendCounterStopReqAndStartTimer(&counterCtrl, pmhContext, setReq);
      if(BS_NO_ERROR NOTEQ error)
      {
        pmhFillAndSendSetObjFail( setReq, error, &setReq->tlvArray.tlvInfo[0].tag );
        return;
      }
      break;
    default:
      pmhFillAndSendSetObjFail( setReq, BS_ERR_INVALID_COLID_RCVD, &setReq->tlvArray.tlvInfo[0].tag );
      return;
  }

}
/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function handles the SET_OBJ_REQ message received from BSM.
 */
PmhState pmhProcSetObjReq(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  TagInfo         tag;
  SetObjReq       setReq;
  clearBytes( &setReq, sizeof(SetObjReq));

  decSetObjReqMsg( (UINT8*)msg->data, msg->len, &setReq );

  tag = setReq.tlvArray.tlvInfo[0].tag;

  switch( tag.tableId )
  {
    case COUNTERCONFIG:
      pmhProcSetReqOnCounterConfigTable(&setReq, pmhContext);
      break;
    case COUNTERCONTROL:
      pmhProcCounterControlReq(&setReq,pmhContext);
      break;
    default:
      BS_ERROR_UNKNOWN_TABLE_ID(tag.tableId);
      break;
  }
  return pmhContext->pmhState;
}

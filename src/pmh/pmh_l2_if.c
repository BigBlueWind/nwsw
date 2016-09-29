/**
 * @file    pmh_l2_if.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will contains functions to handle the L2Counters related messages
 *
 * @author  Ankur Agrawal
 * @date    24-Aug-2012
 */


#include "pmh.h"

void pmhLogProfileContext(
  CounterProfileContext *pCtx,
  UINT8                 *info
  )
{
  logInfo("%s",info);

  logInfo(" profileId=%d isProfileRunning=%d numOfSamplesRcvd=%d numOfSamplesRemaining %d",
           pCtx->profileId, pCtx->isProfileRunning, pCtx->numOfSamplesRcvd, pCtx->numOfSamplesRemaining);

  logInfo(" totalNumOfSamples=%d numOfSamplesRemainingOfReportingPeriod=%d totalSamplePerReportingPeriod=%d",
           pCtx->totalNumOfSamples, pCtx->numOfSamplesRemainingOfReportingPeriod, pCtx->totalSamplePerReportingPeriod);

  logInfo(" lastSeqNum=%d reportingDuration=%d measurementDuration=%d",
           pCtx->lastSeqNum, pCtx->reportingDuration, pCtx->measurementDuration);

}
void pmhClosePmLogFileAndUpdateStatusTablesAndStopTimerAndSendNoti(
  IN ProfileId             profileId,
  IN MeasurementStopCause  cause,
  IO PmhContext           *pmhContext
  )
{
  CounterCollectionStatus  *status = (CounterCollectionStatus*)getRowOfTable(COUNTERCOLLECTIONSTATUS,profileId-1);
  CounterProfileContext    *pCtx = getCounterProfileCtx(profileId);

  /* Stop the timer */
  if (PROFILEID_PROFILE1 EQ profileId)
  {
    stopTimer(pmhContext->pmhTimersInfo.modTimerInfo[TIMER_INDEX_WAIT_L2PMREPORTPROFILE1].timerId);
  }
  else if (PROFILEID_PROFILE2 EQ profileId)
  {
    stopTimer(pmhContext->pmhTimersInfo.modTimerInfo[TIMER_INDEX_WAIT_L2PMREPORTPROFILE2].timerId);
  }

  /* Close the PM logFile */
  pmhClosePmLogFileForProfile(pCtx);

  /* Update Counter Collection status table */
  status->isRunning = FALSE;
  status->numOfamplesRemaining = 0;
  getAndFillDateAndTimeStamp( MAX_SIZE_COUNTERCOLLECTIONSTATUS_STOPPEDAT,  status->stoppedAt );
  snprintf(status->stoppedReason,MAX_SIZE_COUNTERCOLLECTIONSTATUS_STOPPEDREASON, "%s", getMeasurementStoppedCauseStr(cause));

  pCtx->isProfileRunning = FALSE;

  /* Send NOTI to BSM */
  sendNotiToBsmIfFileIsNonZeroOrClearFileIfZero( pCtx->currPmLogFileName );
  pmhSendMeasurementCompleteNotiToBsm(profileId, cause);

  return;
}

void pmhCloseAndOpenNewPmLogFileAndRestartTimer(
  IN ProfileId    profileId,
  IO PmhContext  *pmhContext
  )
{
  CounterProfileContext  *pCtx = getCounterProfileCtx(profileId);

  /* Restart the REPORT DURATION timer */
  if (PROFILEID_PROFILE1 EQ profileId)
  {
    stopTimer(pmhContext->pmhTimersInfo.modTimerInfo[TIMER_INDEX_WAIT_L2PMREPORTPROFILE1].timerId);
    pmhStartTimer(TIMER_INDEX_WAIT_L2PMREPORTPROFILE1, 0, getReportingPeriodInSec(pCtx->reportingDuration));
  }
  else if (PROFILEID_PROFILE2 EQ profileId)
  {
    stopTimer(pmhContext->pmhTimersInfo.modTimerInfo[TIMER_INDEX_WAIT_L2PMREPORTPROFILE2].timerId);
    pmhStartTimer(TIMER_INDEX_WAIT_L2PMREPORTPROFILE2, 0, getReportingPeriodInSec(pCtx->reportingDuration));
  }

  pmhClosePmLogFileForProfile(pCtx);
  sendNotiToBsmIfFileIsNonZeroOrClearFileIfZero( pCtx->currPmLogFileName );
  pmhOpenPmLogFileForProfile(profileId, pmhContext);

  //TODO: Commented the below line as File Rotation will not be supported for the L2M till the next design.
  //pmhAddEntryInPmLogFileStatusTableForProfile(pCtx);
}
/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @details This function processes MSG_UEH_PMH_COUNTER_START_SUCC message receive from UEH.
 * */
PmhState pmhProcCounterStartSucc(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  )
{
  SetObjReq  setReq;
  UINT32     reportingDurationInsecs;

  clearBytes(&setReq, sizeof(SetObjReq));

  MsgUehPmhCounterStartSucc *rcvdRsp    = (MsgUehPmhCounterStartSucc *)msgIn->data;
  CounterCollectionStatus   *status     = (CounterCollectionStatus*)getRowOfTable(COUNTERCOLLECTIONSTATUS,rcvdRsp->profileId-1);
  CounterProfileContext  *pCtx = getCounterProfileCtx(rcvdRsp->profileId);


  if(BS_NO_ERROR NOTEQ getCmdRefByTransIdAndDeallocTransId( rcvdRsp->transId, &(setReq.cmdRef), &(setReq.tlvArray.tlvInfo[0].tag), pmhContext))
  {
    BS_ERROR_TRANSID_DEALLOC_FAILED(rcvdRsp->transId);
    return pmhContext->pmhState;
  }

  setReq.tlvArray.numTlvs++;
  pmhFillAndSendSetObjSucc(&setReq);

  /* Stop Counter control timer and start Reprot Profile Timer */
  reportingDurationInsecs = getReportingPeriodInSec(pCtx->reportingDuration);

  if (PROFILEID_PROFILE1 EQ rcvdRsp->profileId)
  {
    stopTimer(pmhContext->pmhTimersInfo.modTimerInfo[TIMER_INDEX_WAIT_L2PMCOUNTERCONTROLPROFILE1].timerId);
    pmhStartTimer(TIMER_INDEX_WAIT_L2PMREPORTPROFILE1, 0, reportingDurationInsecs);
  }
  else if (PROFILEID_PROFILE2 EQ rcvdRsp->profileId)
  {
    stopTimer(pmhContext->pmhTimersInfo.modTimerInfo[TIMER_INDEX_WAIT_L2PMCOUNTERCONTROLPROFILE2].timerId);
    pmhStartTimer(TIMER_INDEX_WAIT_L2PMREPORTPROFILE2, 0, reportingDurationInsecs);
  }

  /* Open the PM log File */
  pmhOpenPmLogFileForProfile(rcvdRsp->profileId, pmhContext);

  //TODO: Commented the below line as File Rotation will not be supported for the L2M till the next design.
  //pmhAddEntryInPmLogFileStatusTableForProfile(pCtx);

  status->isRunning = TRUE;
  status->measurementDuration = pCtx->measurementDuration;
  status->reportingDuration = pCtx->reportingDuration;
  status->numOfSamplesCollected = pCtx->numOfSamplesRcvd;
  status->numOfamplesRemaining = pCtx->numOfSamplesRemaining;

  snprintf((char *)&status->pmLogFileName, MAX_SIZE_COUNTERCOLLECTIONSTATUS_PMLOGFILENAME+1,"%s",pCtx->currPmLogFileName);
  getAndFillDateAndTimeStamp( MAX_SIZE_COUNTERCOLLECTIONSTATUS_STARTEDAT, status->startedAt );

  snprintf((char *)&status->stoppedAt, MAX_SIZE_COUNTERCOLLECTIONSTATUS_STOPPEDAT+1,"%s","00000000000000000");
  snprintf((char *)&status->stoppedReason, MAX_SIZE_COUNTERCOLLECTIONSTATUS_STOPPEDREASON+1,"%s", "None");
  pmhLogProfileContext(pCtx, "Counter Profile Context After Processing COUNTER_START_SUCC from UEH");
  return pmhContext->pmhState;
}

/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @details This function processes MSG_UEH_PMH_COUNTER_START_FAIL message receive from UEH.
 * */
PmhState pmhProcCounterStartFail(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  )
{
  SetObjReq  setReq;
  clearBytes(&setReq, sizeof(SetObjReq));

  MsgUehPmhCounterStartFail *rcvdRsp    = (MsgUehPmhCounterStartFail *)msgIn->data;

  if(BS_NO_ERROR NOTEQ getCmdRefByTransIdAndDeallocTransId( rcvdRsp->transId, &setReq.cmdRef, &setReq.tlvArray.tlvInfo[0].tag, pmhContext))
  {
    BS_ERROR_TRANSID_DEALLOC_FAILED(rcvdRsp->transId);
    return pmhContext->pmhState;

  }
  setReq.tlvArray.numTlvs++;
  pmhFillAndSendSetObjFail(&setReq, rcvdRsp->errorDesc.errorNum, &setReq.tlvArray.tlvInfo[0].tag);

  /* Stop Counter Control timer */
  if (PROFILEID_PROFILE1 EQ rcvdRsp->profileId)
  {
    stopTimer(pmhContext->pmhTimersInfo.modTimerInfo[TIMER_INDEX_WAIT_L2PMCOUNTERCONTROLPROFILE1].timerId);
  }
  else if (PROFILEID_PROFILE2 EQ rcvdRsp->profileId)
  {
    stopTimer(pmhContext->pmhTimersInfo.modTimerInfo[TIMER_INDEX_WAIT_L2PMCOUNTERCONTROLPROFILE2].timerId);
  }

  pmhContext->counterProfileContext[rcvdRsp->profileId].isProfileRunning = FALSE;

  return pmhContext->pmhState;
}

/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @details This function processes MSG_UEH_PMH_COUNTER_STOP_SUCC message receive from UEH.
 * */
PmhState pmhProcCounterStopSucc(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  )
{
  SetObjReq  setReq;
  clearBytes(&setReq, sizeof(SetObjReq));

  MsgUehPmhCounterStopSucc *rcvdRsp    = (MsgUehPmhCounterStopSucc *)msgIn->data;

  /* TransId will be zero if STOP_REQ is send due to some internal error */
  if (0 NOTEQ rcvdRsp->transId)
  {
    /* Stop Counter Control timer */
    if (PROFILEID_PROFILE1 EQ rcvdRsp->profileId)
    {
      stopTimer(pmhContext->pmhTimersInfo.modTimerInfo[TIMER_INDEX_WAIT_L2PMCOUNTERCONTROLPROFILE1].timerId);
    }
    else if (PROFILEID_PROFILE2 EQ rcvdRsp->profileId)
    {
      stopTimer(pmhContext->pmhTimersInfo.modTimerInfo[TIMER_INDEX_WAIT_L2PMCOUNTERCONTROLPROFILE2].timerId);
    }

    if(BS_NO_ERROR NOTEQ getCmdRefByTransIdAndDeallocTransId( rcvdRsp->transId, &setReq.cmdRef, &setReq.tlvArray.tlvInfo[0].tag, pmhContext))
    {
      BS_ERROR_TRANSID_DEALLOC_FAILED(rcvdRsp->transId);
      return pmhContext->pmhState;
    }
    setReq.tlvArray.numTlvs++;
    pmhFillAndSendSetObjSucc(&setReq);
  }
  pmhLogProfileContext(&(pmhContext->counterProfileContext[rcvdRsp->profileId]),
		  "Counter Profile Context After Processing COUNTER_STOP_SUCC from UEH");
  return pmhContext->pmhState;

}

/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @details This function processes MSG_UEH_PMH_COUNTER_STOP_IND message receive from UEH.
 * */
PmhState pmhProcCounterStopInd(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  )
{
  MsgUehPmhCounterStopInd  *ind    = (MsgUehPmhCounterStopInd *)msgIn->data;

  if(TRUE EQ pmhContext->counterProfileContext[ind->profileId-1].isProfileRunning)
  {
    pmhClosePmLogFileAndUpdateStatusTablesAndStopTimerAndSendNoti(ind->profileId, ind->measurementStopCause, pmhContext);
  }
  return pmhContext->pmhState;

}
/**
 * @param   profileId          profileId
 * @brief This function will send COUNTER_STOP_REQ to UEH.
 **/
void pmhSendCounterStopReq(
  IO ProfileId                profileId,
  IO CounterProfileContext   *pCtx,
  IO MeasurementStopCause     cause
  )
{
  MsgPmhUehCounterStopReq stopReq;

  clearBytes(&stopReq, sizeof(MsgPmhUehCounterStopReq));

  stopReq.transId       = 0;
  stopReq.profileId     = profileId;
  stopReq.cause         = cause;

  pCtx->isProfileRunning = FALSE;

  createAndSendMsg( MSG_PMH_UEH_COUNTER_STOP_REQ, (UINT8 *)&stopReq, sizeof(MsgPmhUehCounterStopReq) );
}

/**
 * @param   msgIn Message received
 * @param   pmhContext PMH Context
 * @details This function processes MSG_UEH_PMH_COUNTER_REPORT_IND message receive from UEH.
 *
 * */
PmhState pmhProcCounterReportInd(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  )
{

  MsgUehPmhCounterReportInd *ind    = (MsgUehPmhCounterReportInd *)msgIn->data;
  CounterCollectionStatus   *status = (CounterCollectionStatus*)getRowOfTable(COUNTERCOLLECTIONSTATUS,ind->profileId-1);
  CounterProfileContext     *pCtx = getCounterProfileCtx(ind->profileId);

  pmhLogProfileContext(pCtx, "Counter Profile Context Before Processing the Report");
  if(FALSE EQ pCtx->isProfileRunning)
  {
	  /* Add seq num convert to macro */
    logError("Report Received for Profile%d which is not running", ind->profileId);
    return pmhContext->pmhState;
  }

  pmhWriteToPmLogFilePerProfileAndUpdateCtx(ind, pmhContext);

  if(ind->sampleSeqNum NOTEQ pCtx->lastSeqNum +1)
  {
	/* Log the error */
    pmhSendCounterStopReq(ind->profileId, pCtx, UNEXPECTED_SEQ_NUM_RCVD);
    pmhClosePmLogFileAndUpdateStatusTablesAndStopTimerAndSendNoti(ind->profileId, UNEXPECTED_SEQ_NUM_RCVD, pmhContext);
    return pmhContext->pmhState;
  }

  pCtx->numOfSamplesRcvd++;
  pCtx->numOfSamplesRemaining--;
  pCtx->numOfSamplesRemainingOfReportingPeriod--;
  pCtx->lastSeqNum = ind->sampleSeqNum;

  status->numOfSamplesCollected = pCtx->numOfSamplesRcvd;
  status->numOfamplesRemaining  = pCtx->numOfSamplesRemaining;

  if( 0 EQ status->numOfamplesRemaining)
  {
    pmhClosePmLogFileAndUpdateStatusTablesAndStopTimerAndSendNoti(ind->profileId, MEASUREMENT_COMPLETED, pmhContext);
    return pmhContext->pmhState;
  }
  if(0 EQ pCtx->numOfSamplesRemainingOfReportingPeriod )
  {
    pmhCloseAndOpenNewPmLogFileAndRestartTimer(ind->profileId, pmhContext);
    pCtx->numOfSamplesRemainingOfReportingPeriod = pCtx->totalSamplePerReportingPeriod;
  }

  pmhLogProfileContext(pCtx, "Counter Profile Context After Processing the Report");

  return pmhContext->pmhState;

}
/**
 * @param   ind      MsgUehPmhCounterReportInd
 * @param   ctx      PmhContext
 * @brief This function will write the received measurement to PM log file.
 **/
void pmhWriteToPmLogFilePerProfileAndUpdateCtx(
  IO MsgUehPmhCounterReportInd *ind,
  IO PmhContext                *ctx
  )
{
  UINT32        index;
  UINT32        ctxIdx;
  L2CounterCtx *l2CounterCtx = &(ctx->l2CounterCtx);

  l2CounterCtx->numOfRows = ind->numOfCounters;

  for(index=0; index < l2CounterCtx->numOfRows; index++)
  {
    l2CounterCtx->l2Counters[index].index     = index+1;
    l2CounterCtx->l2Counters[index].counterId = ind->counterVal[index].counterId;
    l2CounterCtx->l2Counters[index].value     = ind->counterVal[index].val;
    snprintf((char *)&(l2CounterCtx->l2Counters[index].lastCollectedAt), MAX_SIZE_L2COUNTERS_LASTCOLLECTEDAT+1,"%s",ind->collectedAt);


    /* Update the value in PmhContext */
    for(ctxIdx=0;ctxIdx<MAX_ROWS_IN_COUNTERCONFIG_TABLE;ctxIdx++)
    {
      if(ind->counterVal[index].counterId EQ ctx->counterContext[ctxIdx].l2CounterConfig.counterId)
      {
        ctx->counterContext[ctxIdx].value = ind->counterVal[index].val;
        snprintf((char *)&ctx->counterContext[ctxIdx].lastCollectedAt, MAX_SIZE_L2COUNTERS_LASTCOLLECTEDAT+1,"%s",ind->collectedAt);
        break;
      }
    }
  }
  if(TRUE NOTEQ pmhWriteToFileL2CountersTable((L2Counters*)&(l2CounterCtx->l2Counters), ctx->counterProfileContext[ind->profileId-1].fp, l2CounterCtx->numOfRows))
  {
    BS_ERROR_CONFIG_WRITE_FAILED();
  }
  logInfo(" %d Number of counter value are written to logFile", ind->numOfCounters);
}

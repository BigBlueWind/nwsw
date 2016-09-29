/**
 * @file    pmh_timers.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will contains functions to handle timer functions.
 *
 * @author  Shweta Polepally
 * @date    1 April 2011
 */

#include "pmh.h"

/**
 * @param   numTimers  Number of timers
 * @param   config    Timers configuration structure
 * @param   ctx       PMH context
 * @details This function will update the timer details in PMH context
 * */
static void pmhStoreTimersInCtx(
  IN UINT32        numTimers,
  IN TimersConfig *config,
  IO PmhContext   *ctx
  )
{
  UINT8   timerIndex =0, pmhTimerIndex=2;

  ctx->pmhTimersInfo.modTimerInfo[TIMER_INDEX_SAMPLE_INTERVAL].timerDesc.moduleId          = MODID_PMH;
  ctx->pmhTimersInfo.modTimerInfo[TIMER_INDEX_SAMPLE_INTERVAL].timerDesc.timeSpec.duration = 0;
  ctx->pmhTimersInfo.modTimerInfo[TIMER_INDEX_SAMPLE_INTERVAL].timerDesc.timeSpec.timeUnit = TIMEUNIT_MINUTE;
  ctx->pmhTimersInfo.modTimerInfo[TIMER_INDEX_SAMPLE_INTERVAL].timerDesc.timerType         = TIMERNAME_WAITSAMPLEINTERVAL;
  ctx->pmhTimersInfo.modTimerInfo[TIMER_INDEX_SAMPLE_INTERVAL].timerDesc.timeoutMsgId      = MSG_TIMEKEEPER_PMH_TIMEOUT_IND;


  ctx->pmhTimersInfo.modTimerInfo[TIMER_INDEX_PM_LOGFILE_CREATE_INTERVAL].timerDesc.moduleId          = MODID_PMH;
  ctx->pmhTimersInfo.modTimerInfo[TIMER_INDEX_PM_LOGFILE_CREATE_INTERVAL].timerDesc.timeSpec.duration = 0;
  ctx->pmhTimersInfo.modTimerInfo[TIMER_INDEX_PM_LOGFILE_CREATE_INTERVAL].timerDesc.timeSpec.timeUnit = TIMEUNIT_MINUTE;
  ctx->pmhTimersInfo.modTimerInfo[TIMER_INDEX_PM_LOGFILE_CREATE_INTERVAL].timerDesc.timerType         = TIMERNAME_WAITPMFILECLOSE;
  ctx->pmhTimersInfo.modTimerInfo[TIMER_INDEX_PM_LOGFILE_CREATE_INTERVAL].timerDesc.timeoutMsgId      = MSG_TIMEKEEPER_PMH_TIMEOUT_IND;

  for ( timerIndex=0; timerIndex < numTimers; timerIndex++ )
  {
    if ( config->moduleId EQ MODID_PMH )
    {
      ctx->pmhTimersInfo.modTimerInfo[pmhTimerIndex].timerDesc.moduleId          = config->moduleId;
      ctx->pmhTimersInfo.modTimerInfo[pmhTimerIndex].timerDesc.timeSpec.duration = config->duration;
      ctx->pmhTimersInfo.modTimerInfo[pmhTimerIndex].timerDesc.timeSpec.timeUnit = config->timeUnit;
      ctx->pmhTimersInfo.modTimerInfo[pmhTimerIndex].timerDesc.timerType         = config->timerName;
      ctx->pmhTimersInfo.modTimerInfo[pmhTimerIndex].timerDesc.timeoutMsgId      = MSG_TIMEKEEPER_PMH_TIMEOUT_IND;
      ctx->pmhTimersInfo.totalTimersInMod++;
      pmhTimerIndex++;
    }
    config++;
  }
}

/**
 * @param ctx  PMH context
 * @details This function will read the BS timers configuration file.
 **/
void pmhReadTimersFromCfg(
  IO PmhContext  *ctx
  )
{
  UINT8   isError = FALSE;
  UINT8   fullFilePath[MAX_BUFFER_SIZE];

  GenericTable  *tableFunc       = getTableFunctions(TIMERSCONFIG);
  TimersConfig  *rowOfTimerTable = (TimersConfig*)getFirstRowOfTable(TIMERSCONFIG);

  clearBytes( &fullFilePath, sizeof(fullFilePath));
  constructFullPath ( ctx->configDir, "enb_pre_config.cfg", fullFilePath );

  if ( NULL EQ tableFunc->getReadFromFileFunc )
  {
    BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("BSTIMERSCONFIG");
    exit(EXIT_FAILURE);
  }
  else
  {
    /* Copy PMH timers in PMH context */
    pmhStoreTimersInCtx( tableFunc->getReadFromFileFunc( fullFilePath, rowOfTimerTable, &isError ), rowOfTimerTable, ctx );
    if ( isError EQ TRUE )
    {
      BS_ERROR_CONFIG_READ_FAILED()
      exit(EXIT_FAILURE);
    }
  }
}

/**
 * @param   ctx   PMH context
 * @details This function processes timeout indication of PM Log file creation
 * */
void closePmLogFileAndNotifyBsm(
  IO PmhContext *ctx
  )
{
  ctx->isCounterCollectionInProgress = FALSE;
  stopTimer( ctx->pmhTimersInfo.modTimerInfo[TIMER_INDEX_PM_LOGFILE_CREATE_INTERVAL].timerId );
  pmhClosePmLogFile(ctx);

  sendNotiToBsmIfFileIsNonZeroOrClearFileIfZero( ctx->currPmLogFileName );

}

/**
 * @param   ctx  PMH context
 * @details This function processes timeout indication of PM Log file creation
 * */
void pmhProcPmFileTimeOutInd(
  IO PmhContext *ctx
  )
{
  /* Close the current opened file */
  pmhClosePmLogFile(ctx);

  /* Check if anything was written in the file, by getting the file size, else remove the entry from the table */
  sendNotiToBsmIfFileIsNonZeroOrClearFileIfZero( ctx->currPmLogFileName );

  /* Check if new file can be created , else stop all the timers, log error and return */
  if ( TRUE EQ listHasSpace(getPmLogFileStatusTable()) )
  {
    if(TRUE EQ pmhOpenPmLogFile(ctx))
    {
      pmhAddEntryInPmLogFileStatusTable(ctx);
    }
  }
  else
  {
    if(TRUE EQ pmhOpenPmLogFile(ctx))
    {
      pmhUpdateEntryInPmLogFileStatusTable(ctx);
    }
  }

  /* Restart PMFile timer to collect the information in this new created file */
  if(NULL_TIMER_ID EQ pmhStartTimer(TIMER_INDEX_PM_LOGFILE_CREATE_INTERVAL, 0,pmhGetPmLogFileCreationTimeVal(ctx->pmCounterLogConfig.pmFileCreationInterval)))
  {
    BS_ERROR_PMH_TIMER_NOT_STARTED("PMLogFile");
    ctx->isCounterCollectionInProgress = FALSE;
    return;
  }
}

/**
 * @param   ctx  PMH context
 * @details This function sends GEN_STS_REQ message to all RRM modules
 * */
void sendStatReqToAllModules(
  IO PmhContext   *ctx
  )
{
  if( TRUE EQ ctx->sendCounterReq.macGenCounter )
  {
    createAndSendMacGenStatReq(NULL, NULL, ctx);
  }
  if( TRUE EQ ctx->sendCounterReq.macUplaneCounter )
  {
    ctx->rspRspFromMacRclAndMacPhyCounter = 0;
    createAndSendMacUplaneStatReq(NULL, NULL, ctx);
  }
  if( TRUE EQ ctx->sendCounterReq.rlcGenCounter )
  {
    createAndSendRlcGenStatReq(NULL, NULL, ctx);
  }
  if( TRUE EQ ctx->sendCounterReq.pdcpGenCounter )
  {
    createAndSendPdcpGenStatReq(NULL, NULL, ctx);
  }
  if( TRUE EQ ctx->sendCounterReq.pdcpUplaneCounter )
  {
    createAndSendPdcpUplaneStatReq(NULL, NULL, ctx);
  }
  if( TRUE EQ ctx->sendCounterReq.gtpGenCounter )
  {
    createAndSendGtpGenStatReq(NULL, NULL, ctx);
  }
  if( TRUE EQ ctx->sendCounterReq.sctpGenCounter )
  {
    createAndSendSctpGenStatReq(NULL, NULL, ctx);
  }
  if( TRUE EQ ctx->sendCounterReq.s1apGenCounter )
  {
    createAndSendS1apGenStatReq(NULL, NULL, ctx);
  }
  if( TRUE EQ ctx->sendCounterReq.s1apPeerNodeCounter )
  {
    createAndSendS1apPeerNodeStatReq(NULL, NULL, ctx->peerId, ctx);
  }
  if( TRUE EQ ctx->sendCounterReq.rrcGenCounter )
  {
    createAndSendRrcGenStatReq(NULL, NULL, ctx);
  }
  if( TRUE EQ ctx->sendCounterReq.rrcCellCounter )
  {
    createAndSendRrcCellStatReq(NULL, NULL, ctx->cellId, ctx);
  }
  if( ctx->sendCounterReq.rrmCounter )
  {
    createAndSendUehRrmStatReq(NULL, NULL, ctx);
  }
  if( TRUE EQ ctx->sendCounterReq.ipCounter )
  {
    pmhProcIpStatusReq(FALSE,ctx, NULL);
  }
  if( TRUE EQ ctx->sendCounterReq.ethernetCounter )
  {
    pmhProcEthernetStatusReq(FALSE, ctx, NULL);
  }
}

/**
 * @param   ctx  PMH context
 * @details This function sends GEN_STS_REQ message to all RRM modules to clear the counters
 * */
void sendCounterResetReqToAllModule(
  IO PmhContext   *pmhContext,
  IO UINT32        transId
  )
{
  MsgPmhSmMacGenStatReq        msgPmhSmMacGenStatReq;
  MsgPmhSmMacRlcsapStatReq     msgPmhSmMacRlcsapStatReq;
  MsgPmhSmMacPhysapStatReq     msgPmhSmMacPhysapStatReq;
  MsgPmhSmRlcGenStatReq        msgPmhSmRlcGenStatReq;
  MsgPmhSmPdcpGenStatReq       msgPmhSmPdcpGenStatReq;
  MsgPmhSmPdcpUplaneStatReq    msgPmhSmPdcpUplaneStatReq;
  MsgPmhSmGtpGenStatReq        msgPmhSmGtpGenStatReq;
  MsgPmhSmSctpGenStatReq       msgPmhSmSctpGenStatReq;
  MsgPmhSmS1apGenStatReq       msgPmhSmS1apGenStatReq;
  MsgPmhSmS1apPeerNodeStatReq  msgPmhSmS1apPeerNodeStatReq;
  MsgPmhSmRrcGenStatReq        msgPmhSmRrcGenStatReq;
  MsgPmhSmRrcCellStatReq       msgPmhSmRrcCellStatReq;
  MsgPmhUehRrmStatReq          msgPmhUehRrmStatReq;

  clearBytes(&msgPmhSmMacGenStatReq,      sizeof(MsgPmhSmMacGenStatReq));
  clearBytes(&msgPmhSmMacRlcsapStatReq,    sizeof(MsgPmhSmMacRlcsapStatReq));
  clearBytes(&msgPmhSmMacPhysapStatReq,    sizeof(MsgPmhSmMacPhysapStatReq));
  clearBytes(&msgPmhSmRlcGenStatReq,       sizeof(MsgPmhSmRlcGenStatReq));
  clearBytes(&msgPmhSmPdcpGenStatReq,      sizeof(MsgPmhSmPdcpGenStatReq));
  clearBytes(&msgPmhSmPdcpUplaneStatReq,   sizeof(MsgPmhSmPdcpUplaneStatReq));
  clearBytes(&msgPmhSmGtpGenStatReq,       sizeof(MsgPmhSmGtpGenStatReq));
  clearBytes(&msgPmhSmSctpGenStatReq,      sizeof(MsgPmhSmSctpGenStatReq));
  clearBytes(&msgPmhSmS1apGenStatReq,      sizeof(MsgPmhSmS1apGenStatReq));
  clearBytes(&msgPmhSmS1apPeerNodeStatReq, sizeof(MsgPmhSmS1apPeerNodeStatReq));
  clearBytes(&msgPmhSmRrcGenStatReq,       sizeof(MsgPmhSmRrcGenStatReq));
  clearBytes(&msgPmhSmRrcCellStatReq,      sizeof(MsgPmhSmRrcCellStatReq));
  clearBytes(&msgPmhUehRrmStatReq,         sizeof(MsgPmhUehRrmStatReq));

  msgPmhSmMacGenStatReq.transId       = transId;
  msgPmhSmMacRlcsapStatReq.transId    = transId;
  msgPmhSmMacPhysapStatReq.transId    = transId;
  msgPmhSmRlcGenStatReq.transId       = transId;
  msgPmhSmPdcpGenStatReq.transId      = transId;
  msgPmhSmPdcpUplaneStatReq.transId   = transId;
  msgPmhSmGtpGenStatReq.transId       = transId;
  msgPmhSmSctpGenStatReq.transId      = transId;
  msgPmhSmS1apGenStatReq.transId      = transId;
  msgPmhSmS1apPeerNodeStatReq.transId = transId;
  msgPmhSmRrcGenStatReq.transId       = transId;
  msgPmhSmRrcCellStatReq.transId      = transId;
  msgPmhUehRrmStatReq.transId         = transId;

  msgPmhSmMacGenStatReq.resetAllCounters       = TRUE;
  msgPmhSmMacRlcsapStatReq.resetAllCounters    = TRUE;
  msgPmhSmMacPhysapStatReq.resetAllCounters    = TRUE;
  msgPmhSmRlcGenStatReq.resetAllCounters       = TRUE;
  msgPmhSmPdcpGenStatReq.resetAllCounters      = TRUE;
  msgPmhSmPdcpUplaneStatReq.resetAllCounters   = TRUE;
  msgPmhSmGtpGenStatReq.resetAllCounters       = TRUE;
  msgPmhSmSctpGenStatReq.resetAllCounters      = TRUE;
  msgPmhSmS1apGenStatReq.resetAllCounters      = TRUE;
  msgPmhSmS1apPeerNodeStatReq.resetAllCounters = TRUE;
  msgPmhSmRrcGenStatReq.resetAllCounters       = TRUE;
  msgPmhSmRrcCellStatReq.resetAllCounters      = TRUE;
  msgPmhUehRrmStatReq.resetAllCounters         = TRUE;

  pmhContext->rspRspFromMacRclAndMacPhyCounter = 0;
  msgPmhSmS1apPeerNodeStatReq.peerId           = pmhContext->peerId;
  msgPmhSmRrcCellStatReq.cellId                = pmhContext->cellId;

  createAndSendMsg( MSG_PMH_SM_MAC_GEN_STAT_REQ,        (UINT8 *)&msgPmhSmMacGenStatReq,       sizeof(MsgPmhSmMacGenStatReq) );
  createAndSendMsg( MSG_PMH_SM_MAC_RLCSAP_STAT_REQ,     (UINT8 *)&msgPmhSmMacRlcsapStatReq,    sizeof(MsgPmhSmMacRlcsapStatReq) );
  createAndSendMsg( MSG_PMH_SM_MAC_PHYSAP_STAT_REQ,     (UINT8 *)&msgPmhSmMacPhysapStatReq,    sizeof(MsgPmhSmMacPhysapStatReq) );
  createAndSendMsg( MSG_PMH_SM_RLC_GEN_STAT_REQ,        (UINT8 *)&msgPmhSmRlcGenStatReq,       sizeof(MsgPmhSmRlcGenStatReq) );
  createAndSendMsg( MSG_PMH_SM_PDCP_GEN_STAT_REQ,       (UINT8 *)&msgPmhSmPdcpGenStatReq,      sizeof(MsgPmhSmPdcpGenStatReq) );
  createAndSendMsg( MSG_PMH_SM_PDCP_UPLANE_STAT_REQ,    (UINT8 *)&msgPmhSmPdcpUplaneStatReq,   sizeof(MsgPmhSmPdcpUplaneStatReq) );
  createAndSendMsg( MSG_PMH_SM_EGTP_GEN_STAT_REQ,       (UINT8 *)&msgPmhSmGtpGenStatReq,       sizeof(MsgPmhSmGtpGenStatReq) );
  createAndSendMsg( MSG_PMH_SM_SCTP_GEN_STAT_REQ,       (UINT8 *)&msgPmhSmSctpGenStatReq,      sizeof(MsgPmhSmSctpGenStatReq) );
  createAndSendMsg( MSG_PMH_SM_S1AP_GEN_STAT_REQ,       (UINT8 *)&msgPmhSmS1apGenStatReq,      sizeof(MsgPmhSmS1apGenStatReq) );
  createAndSendMsg( MSG_PMH_SM_RRC_GEN_STAT_REQ,        (UINT8 *)&msgPmhSmRrcGenStatReq,       sizeof(MsgPmhSmRrcGenStatReq) );
  createAndSendMsg( MSG_PMH_SM_RRC_CELL_STAT_REQ,       (UINT8 *)&msgPmhSmRrcCellStatReq,      sizeof(MsgPmhSmRrcCellStatReq) );
  createAndSendMsg( MSG_PMH_UEH_RRM_STAT_REQ,           (UINT8 *)&msgPmhUehRrmStatReq,         sizeof(MsgPmhUehRrmStatReq) );

  /*TODO this code will be removed when crashing problem of cp.exe on
   * GET on S1APPEERNODECOUNTERS before first UNLOCK resolved
   * */
  if(TRUE EQ pmhContext->isOpStartRcvd)
  {
    createAndSendMsg( MSG_PMH_SM_S1AP_PEER_NODE_STAT_REQ, (UINT8 *)&msgPmhSmS1apPeerNodeStatReq, sizeof(MsgPmhSmS1apPeerNodeStatReq) );
  }
  else
  {
    logInfo("%s", "CLEAR COUNTER request on S1APPEERNODECOUNTERS is not forwarded to S1AP");
  }
}

/**
 * @param   sendReqCounters         Runtime counter flags to indicate wether response from standard protocol is received
 * @param   rcvdRspCounters         Runtime counter flags to indicate wether response from standard protocol is received
 * @details This function checks if all reponses are received from standard protocols for counter request messages
 * */
static void pmhLogErrForModulesNotResponded(
  IO CounterFlags *sendReqCounters,
  IO CounterFlags *rcvdRspCounters
  )
{
  if(sendReqCounters->macGenCounter NOTEQ rcvdRspCounters->macGenCounter )
  {
    BS_ERROR_COUNTER_VALUES_NOT_RECEIVED("MACGEN");
  }
  if(sendReqCounters->rlcGenCounter NOTEQ rcvdRspCounters->rlcGenCounter )
  {
    BS_ERROR_COUNTER_VALUES_NOT_RECEIVED("RLCGEN");
  }
  if(sendReqCounters->pdcpGenCounter NOTEQ rcvdRspCounters->pdcpGenCounter )
  {
    BS_ERROR_COUNTER_VALUES_NOT_RECEIVED("PDCPGEN");
  }
  if(sendReqCounters->pdcpUplaneCounter NOTEQ rcvdRspCounters->pdcpUplaneCounter)
  {
    BS_ERROR_COUNTER_VALUES_NOT_RECEIVED("PDCPUPLANE");
  }
  if(sendReqCounters->gtpGenCounter NOTEQ rcvdRspCounters->gtpGenCounter)
  {
    BS_ERROR_COUNTER_VALUES_NOT_RECEIVED("GTPGEN");
  }
  if(sendReqCounters->sctpGenCounter NOTEQ rcvdRspCounters->sctpGenCounter)
  {
    BS_ERROR_COUNTER_VALUES_NOT_RECEIVED("SCTPGEN");
  }
  if(sendReqCounters->s1apGenCounter NOTEQ rcvdRspCounters->s1apGenCounter)
  {
    BS_ERROR_COUNTER_VALUES_NOT_RECEIVED("S1APGEN");
  }
  if(sendReqCounters->s1apPeerNodeCounter EQ rcvdRspCounters->s1apPeerNodeCounter)
  {
    BS_ERROR_COUNTER_VALUES_NOT_RECEIVED("S1APPEERNODE");
  }
  if(sendReqCounters->rrcGenCounter NOTEQ rcvdRspCounters->rrcGenCounter)
  {
    BS_ERROR_COUNTER_VALUES_NOT_RECEIVED("RRCGEN");
  }
  if(sendReqCounters->rrcCellCounter NOTEQ rcvdRspCounters->rrcCellCounter)
  {
    BS_ERROR_COUNTER_VALUES_NOT_RECEIVED("RRCCELL");
  }
  if(sendReqCounters->ipCounter NOTEQ rcvdRspCounters->ipCounter)
  {
    BS_ERROR_COUNTER_VALUES_NOT_RECEIVED("IP");
  }
  if(sendReqCounters->ethernetCounter NOTEQ rcvdRspCounters->ethernetCounter)
  {
    BS_ERROR_COUNTER_VALUES_NOT_RECEIVED("ETHERNET");
  }
  if(sendReqCounters->rrmCounter NOTEQ rcvdRspCounters->rrmCounter)
  {
    BS_ERROR_COUNTER_VALUES_NOT_RECEIVED("RRM");
  }
  clearBytes(rcvdRspCounters, sizeof(CounterFlags));
  clearBytes(sendReqCounters, sizeof(CounterFlags));
  //TODO Raise the Alarm, or inform the BSM
}


/**
 * @param   ctx                          PMH context
 * @details This function processes timeout indication for Sample collection Interval
 * */
static void pmhProcSampleIntervalTimerExpiry(
  IO PmhContext *ctx
  )
{
  UINT32  dirSize=0;
  if( FALSE EQ pmhIsAnyCounterEnabled() )
  {
    /* Send message counter collection failed to BSM */
    BS_ERROR_ALL_PM_GROUPS_DISABLED();
    pmhSendCntrCollectFailMsgToBsm( BS_ERR_ALL_PM_GROUPS_DISABLED, "ALL PM groups disabled" );

    closePmLogFileAndNotifyBsm( ctx );
    return;
  }

  /* Directory size exceeded, So no new file can be created
   * Stop File creation timer, otherwise We create a file in the directory whose size is already exceeded
   */
  if( TRUE EQ pmhHasPmLogDirSizeExceeded( ctx, &dirSize ) )
  {
    BS_ERROR_COMBINED_STAT_FILE_SIZE_EXCEEDED( dirSize );
    pmhSendCntrCollectFailMsgToBsm( BS_ERR_COMBINED_STAT_FILE_SIZE_EXCEEDED, "Combined statistics File-Size Exceeded." );
    closePmLogFileAndNotifyBsm( ctx );
    return;
  }

  /* Start SampleInterval timer Again so that we can collect the
   * Counters on timer expiry
   */
  if( NULL_TIMER_ID EQ pmhStartTimer(TIMER_INDEX_SAMPLE_INTERVAL, 0 ,pmhGetSampleIntervalTimeVal(ctx->pmCounterLogConfig.sampleInterval)) )
  {
    BS_ERROR_PMH_TIMER_NOT_STARTED("SampleInterval");
    ctx->isCounterCollectionInProgress = FALSE;
  }
  else
  {
    clearBytes( &ctx->rcvdCounterRsp, sizeof(CounterFlags));
    ctx->resetAllCounters = FALSE;
    sendStatReqToAllModules( ctx );
    setPmhState( ctx, PMH_WAIT_BULK_COUNTER_COLLECTION );

    /* Start timer T_WAIT_COUNTER_COLLECTION */
    if(NULL_TIMER_ID EQ pmhStartTimer(TIMER_INDEX_WAIT_COUNTER_COLLECTION, 0,0))
    {
      BS_ERROR_PMH_TIMER_NOT_STARTED("Wait Counter Collection");
    }
    ctx->isSampleIntervalOrFileCreationTimeModified = FALSE;
  }
}


/**
 * @param   ctx                          PMH context
 * @details This function processes timeout indication for Counter Reset Interval
 * */
static void pmhProcWaitL2PmCounterControlTimerExpiry(
  IO PmhContext *pmhContext,
  IO UINT32      transId
  )
{
  SetObjReq  setReq;

  clearBytes(&setReq, sizeof(SetObjReq));

  if(BS_NO_ERROR NOTEQ getCmdRefByTransIdAndDeallocTransId( transId, &setReq.cmdRef, &setReq.tlvArray.tlvInfo[0].tag, pmhContext))
  {
    return;
  }
  setReq.tlvArray.numTlvs++;
  pmhFillAndSendSetObjFail(&setReq,BS_ERR_COUNTER_CONTROL_GUARD_TIMER_EXPIRED,&setReq.tlvArray.tlvInfo[0].tag);
  return;
}


static void pmhProcReportingTimerExpiry(
  IN ProfileId   profileId,
  IO PmhContext *pmhContext
  )
{
  MsgPmhUehCounterStopReq stopReq;

  clearBytes(&stopReq, sizeof(MsgPmhUehCounterStopReq));

  pmhContext->counterProfileContext[profileId-1].isProfileRunning = FALSE;

  stopReq.profileId = profileId;
  stopReq.transId   = 0;
  stopReq.cause     = REPORTING_TIMER_EXPIRED;

  createAndSendMsg( MSG_PMH_UEH_COUNTER_STOP_REQ, (UINT8 *)&stopReq, sizeof(MsgPmhUehCounterStopReq) );
  pmhClosePmLogFileAndUpdateStatusTablesAndStopTimerAndSendNoti(profileId, REPORTING_TIMER_EXPIRED, pmhContext);
  return;
}
/**
 * @param   ctx                          PMH context
 * @details This function processes timeout indication for Counter Reset Interval
 * */
static void pmhProcWaitCounterResetTimerExpiry(
  IO PmhContext *pmhContext,
  IO UINT32      transId
  )
{
  CmdRef   cmdRef;
  TagInfo  tagInfo;
  MsgPmhBsmCounterClearFail   fail;

  clearBytes(&cmdRef, sizeof(CmdRef));
  clearBytes(&tagInfo, sizeof(TagInfo));
  clearBytes(&fail, sizeof(MsgPmhBsmCounterClearFail));
  pmhLogErrForModulesNotResponded( &pmhContext->sendClearCounterReq, &pmhContext->rcvdClearCounterRsp );

  pmhContext->resetAllCounters = FALSE;
  clearBytes( &pmhContext->sendClearCounterReq, sizeof(CounterFlags));
  clearBytes( &pmhContext->rcvdClearCounterRsp, sizeof(CounterFlags));

  if(FALSE EQ pmhStsRspRcvdFromAllModules( &pmhContext->sendClearCounterReq, &pmhContext->rcvdClearCounterRsp ) )
  {
    if(BS_NO_ERROR NOTEQ getCmdRefByTransIdAndDeallocTransId( transId, &cmdRef, &tagInfo, pmhContext))
    {
      return;
    }
    /* Send COUNTER_CLEAR_CFM to BSM */
    copyBytes(&cmdRef, sizeof(CmdRef), &fail.cmdRef);
    createAndSendMsg( MSG_PMH_BSM_COUNTER_CLEAR_FAIL, (UINT8*)&fail, sizeof(MsgPmhBsmCounterClearFail));
  }
  return;
}

/**
 * @param   msg     Message received
 * @param   pmhContext     PMH context
 * @details This function processes timeout indication for Sample collection Interval
 * */
PmhState pmhProcTimeOutIndInIdleState(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  MsgTimeoutInd *pmhTimeOutInd = (MsgTimeoutInd*)msg->data;

  switch( pmhTimeOutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITSAMPLEINTERVAL:
    {
      pmhProcSampleIntervalTimerExpiry(pmhContext);
    }
    break;

    case TIMERNAME_WAITPMFILECLOSE:
    {
      pmhProcPmFileTimeOutInd(pmhContext);
    }
    break;
    case TIMERNAME_WAITCOUNTERRESET:
    {
      pmhProcWaitCounterResetTimerExpiry(pmhContext, pmhTimeOutInd->timerDesc.userInfo);
    }
    break;
    case TIMERNAME_WAITL2PMCOUNTERCONTROLPROFILE1:
    {
      BS_ERROR_TIMER_EXPIRED(pmhTimeOutInd->timerDesc.timerType);
      pmhProcWaitL2PmCounterControlTimerExpiry(pmhContext, pmhTimeOutInd->timerDesc.userInfo);
      pmhContext->counterProfileContext[PROFILEID_PROFILE1-1].isProfileRunning = FALSE;
    }
    break;
    case TIMERNAME_WAITL2PMCOUNTERCONTROLPROFILE2:
    {
      BS_ERROR_TIMER_EXPIRED(pmhTimeOutInd->timerDesc.timerType);
      pmhProcWaitL2PmCounterControlTimerExpiry(pmhContext, pmhTimeOutInd->timerDesc.userInfo);
      pmhContext->counterProfileContext[PROFILEID_PROFILE2-1].isProfileRunning = FALSE;
    }
    break;
    case TIMERNAME_WAITL2PMREPORTPROFILE1:
    {
      BS_ERROR_TIMER_EXPIRED(pmhTimeOutInd->timerDesc.timerType);
      pmhProcReportingTimerExpiry(PROFILEID_PROFILE1, pmhContext);
    }
    break;
    case TIMERNAME_WAITL2PMREPORTPROFILE2:
    {
      BS_ERROR_TIMER_EXPIRED(pmhTimeOutInd->timerDesc.timerType);
      pmhProcReportingTimerExpiry(PROFILEID_PROFILE2, pmhContext);
    }
    break;
    default:
    {
      BS_ERROR_INVALID_PMH_TIMERID_EXPIRED( pmhTimeOutInd->timerDesc.timerType )
      break;
    }
  }
  return pmhContext->pmhState;
}

/**
 * @param   msg     Message received
 * @param   pmhContext     PMH context
 * @details This function processes timeout indication for Counter Clear Interval
 * */
PmhState pmhProcTimeOutIndInWaitInitAndWaitOpStartState(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  MsgTimeoutInd *pmhTimeOutInd = (MsgTimeoutInd*)msg->data;

  switch( pmhTimeOutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITCOUNTERRESET:
    {
      pmhProcWaitCounterResetTimerExpiry(pmhContext, pmhTimeOutInd->timerDesc.userInfo);
    }
    break;
    case TIMERNAME_WAITL2PMCOUNTERCONTROLPROFILE1:
    {
      BS_ERROR_TIMER_EXPIRED(pmhTimeOutInd->timerDesc.timerType);
      pmhProcWaitL2PmCounterControlTimerExpiry(pmhContext, pmhTimeOutInd->timerDesc.userInfo);
      pmhContext->counterProfileContext[PROFILEID_PROFILE1-1].isProfileRunning = FALSE;
    }
    break;
    case TIMERNAME_WAITL2PMCOUNTERCONTROLPROFILE2:
    {
      BS_ERROR_TIMER_EXPIRED(pmhTimeOutInd->timerDesc.timerType);
      pmhProcWaitL2PmCounterControlTimerExpiry(pmhContext, pmhTimeOutInd->timerDesc.userInfo);
      pmhContext->counterProfileContext[PROFILEID_PROFILE2-1].isProfileRunning = FALSE;
    }
    case TIMERNAME_WAITL2PMREPORTPROFILE1:
    {
      BS_ERROR_TIMER_EXPIRED(pmhTimeOutInd->timerDesc.timerType);
      pmhProcReportingTimerExpiry(PROFILEID_PROFILE1, pmhContext);
    }
    break;
    case TIMERNAME_WAITL2PMREPORTPROFILE2:
    {
      BS_ERROR_TIMER_EXPIRED(pmhTimeOutInd->timerDesc.timerType);
      pmhProcReportingTimerExpiry(PROFILEID_PROFILE2, pmhContext);
    }
    break;
    default:
    {
      BS_ERROR_INVALID_PMH_TIMERID_EXPIRED( pmhTimeOutInd->timerDesc.timerType )
      break;
    }
  }
  return pmhContext->pmhState;
}

/**
 * @param   msg         Message received
 * @param   pmhContext  PMH context
 * @details This function processes timeout indication for WAIT_BULK_COUNTER_COLLECTION state
 * */
PmhState pmhProcTimeOutIndInWaitBulkCounterCollectionState(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  MsgTimeoutInd *pmhTimeOutInd = (MsgTimeoutInd*)msg->data;

  switch( pmhTimeOutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITPMFILECLOSE:
    {
      pmhProcPmFileTimeOutInd(pmhContext);
    }
    break;
    case TIMERNAME_WAITCOUNTERRESET:
    {
      pmhProcWaitCounterResetTimerExpiry(pmhContext, pmhTimeOutInd->timerDesc.userInfo);
    }
    break;

    case TIMERNAME_WAITCOUNTERCOLLECTION:
    {
      pmhLogErrForModulesNotResponded( &pmhContext->sendCounterReq, &pmhContext->rcvdCounterRsp );
      /*
       * Update the fileSizeInKB in PMLogFile status table
       */
      pmhUpdatePmLogFileSize( pmhContext->currPmLogFileName );

      /* Write entries to persistent memory */
      pmWritePmLogFileStatusTableToPersistentMemory( "PM Log File status Updated - iteration completed" );

      if( TRUE EQ pmhContext->isSampleIntervalOrFileCreationTimeModified )
      {
        pmhContext->isSampleIntervalOrFileCreationTimeModified = FALSE;
        pmhHandleSampleIntervalTimeChange(pmhContext);
      }
      setPmhState( pmhContext, PMH_IDLE );
    }
    break;
    case TIMERNAME_WAITL2PMCOUNTERCONTROLPROFILE1:
    {
      BS_ERROR_TIMER_EXPIRED(pmhTimeOutInd->timerDesc.timerType);
      pmhProcWaitL2PmCounterControlTimerExpiry(pmhContext, pmhTimeOutInd->timerDesc.userInfo);
      pmhContext->counterProfileContext[PROFILEID_PROFILE1-1].isProfileRunning = FALSE;
    }
    break;
    case TIMERNAME_WAITL2PMCOUNTERCONTROLPROFILE2:
    {
      BS_ERROR_TIMER_EXPIRED(pmhTimeOutInd->timerDesc.timerType);
      pmhProcWaitL2PmCounterControlTimerExpiry(pmhContext, pmhTimeOutInd->timerDesc.userInfo);
      pmhContext->counterProfileContext[PROFILEID_PROFILE2-1].isProfileRunning = FALSE;
    }
    break;
    case TIMERNAME_WAITL2PMREPORTPROFILE1:
    {
      BS_ERROR_TIMER_EXPIRED(pmhTimeOutInd->timerDesc.timerType);
      pmhProcReportingTimerExpiry(PROFILEID_PROFILE1, pmhContext);
    }
    break;
    case TIMERNAME_WAITL2PMREPORTPROFILE2:
    {
      BS_ERROR_TIMER_EXPIRED(pmhTimeOutInd->timerDesc.timerType);
      pmhProcReportingTimerExpiry(PROFILEID_PROFILE2, pmhContext);
    }
    break;
    default:
    {
      BS_ERROR_INVALID_PMH_TIMERID_EXPIRED( pmhTimeOutInd->timerDesc.timerType )
      break;
    }
  }
  return pmhContext->pmhState;
}


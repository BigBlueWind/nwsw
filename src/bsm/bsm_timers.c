/**
 * @file    bsm_timers.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains actions to be taken on timer expiry/ creation of timers.
 *          Any timer relation functions will be added here.
 *
 * @author:  Shweta Polepally
 * @date:    Sep 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param   traceDuration Trace Duration enum value.
 * @details This function will the actual duration based in the enum value.
 **/
Duratn  getTraceDurationFrmEnum(
  IN TraceDuration  traceDuration
  )
{
  switch( traceDuration )
  {
    case TRACEDURATION_5MIN:   return 5;
    case TRACEDURATION_15MIN:  return 15;
    case TRACEDURATION_30MIN:  return 30;
    case TRACEDURATION_45MIN:  return 45;
    case TRACEDURATION_60MIN:  return 60;
    default:                   return 60;
  }
}

/**
 * @param timerType  Type of timer
 * @param timerInfo  This structure contains timer information - Timer description and timer Id
 * @details This function creates and starts Procedure Timer.
 **/
void startBsmProcTimer(
    IN TimerName         timerType,
    IO BsmTimersInfo     *timerInfo
    )
{
  TBOOL       timerFound = FALSE;
  UINT8       i=0;
  for ( i=0; i< timerInfo->totalTimersInMod; i++ )
  {
    if ( timerType EQ timerInfo->modTimerInfo[i].timerDesc.timerType )
    {
      timerFound = TRUE;
      break;
    }
  }
  if ( TRUE EQ timerFound )
  {
    /* Create and start Timer */
    timerInfo->modTimerInfo[i].timerId = createAndStartTimer( &timerInfo->modTimerInfo[i].timerDesc, NULL, NULL);
  }
  else
  {
    BS_ERROR_TIMER_NOT_FOUND( timerNameStr(timerType) );
  }
}

/**
 * @param traceDuration  Trace Duration
 * @param timerInfo      Timer Info
 * @details This function create and start Trace Timer.
 **/
void startTraceDurationTimer(
  IN TraceDuration      traceDuration,
  IO BsmTimersInfo     *timerInfo
  )
{
  TBOOL       timerFound = FALSE;
  UINT8       i=0;
  for ( i=0; i< timerInfo->totalTimersInMod; i++ )
  {
    if ( TIMERNAME_WAITTRACEDURATION EQ timerInfo->modTimerInfo[i].timerDesc.timerType )
    {
      timerFound = TRUE;
      timerInfo->modTimerInfo[i].timerDesc.timeSpec.duration = getTraceDurationFrmEnum(traceDuration);
      timerInfo->modTimerInfo[i].timerDesc.timeSpec.timeUnit = TIMEUNIT_MINUTE;
      break;
    }
  }
  if ( TRUE EQ timerFound )
  {
    /* Create and start Timer */
    timerInfo->modTimerInfo[i].timerId = createAndStartTimer( &timerInfo->modTimerInfo[i].timerDesc, NULL, NULL);
  }
  else
  {
    BS_ERROR_TIMER_NOT_FOUND( timerNameStr(TIMERNAME_WAITTRACEDURATION) );
  }
}

/**
 * @param   type Type of Timer.
 * @details This function will look for the health check timer in timerInfo table
 *          and stop this timer.
 **/
void  stopBsmProcedureTimer(
  IO TimerName   type
  )
{
  BsmContext  *bsmContext = getBsmContext();
  UINT8        i;
  String50    buf;
  clearBytes( &buf, sizeof(String50));

  for ( i=0; i< bsmContext->bsmTimersInfo.totalTimersInMod; i++ )
  {
    if (bsmContext->bsmTimersInfo.modTimerInfo[i].timerDesc.timerType EQ type )
    {
      stopTimer( bsmContext->bsmTimersInfo.modTimerInfo[i].timerId );
      break;
    }
  }
}

/**
 * @param   msg        Message received after timer expiry.
 * @param   bsmContext BSM Context
 * @details This function will handle timer expire in IDLE state.
 **/
inline BsmProceduralState bsmProcTimerExpiryInIdle(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    )
{
  MsgTimeoutInd *msgTimeoutInd= (MsgTimeoutInd*)msg->data;

  switch( msgTimeoutInd->timerDesc.timerType )
  {

    case TIMERNAME_WAITMMELINKESTABLISH:
    {
      /* Check if enodeB is unlocked and S1AP link is Down , if yes, do the enable procedure,
       * else do nothing */
      logInfo("%s", "Timer expired: TIMERNAME_WAITMMELINKESTABLISH ");
      if ( FALSE EQ isEnodeBLocked() )
      {
        logInfo( "%s", "EnodeB is unlocked- Do MME link recovery. Retry enable procedure to establish connection" );
        /* BSM needs to remember enable procedure reason, so that lock can be rejected accordingly
         * S1AP recovery - accept lock
         *  */
        bsmContext->enbEnableReason = ENABLE_REASON_S1AP_RECOVERY;
        enbEnable( bsmContext );
      }
    }
    break;

    default:
    {
      BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType, getBsmProceduralStateName(bsmContext->bsmProceduralState) )
      break;
    }
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param   msg        Message received after timer expiry.
 * @param   bsmContext BSM Context
 * @details This function will handle timer expiry in WAIT_INITAIL_CONFIG state
 **/
inline BsmProceduralState bsmProcTimerExpiryInWaitInitialConfigCmpl(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    )
{
  CreateAlarmDetails   alarmDetails;

  MsgTimeoutInd *msgTimeoutInd= (MsgTimeoutInd*)msg->data;

  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));

  switch( msgTimeoutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITCONFIGCOMPLETE:
    {
      checkForInitConfigProcedureCompletion( bsmContext );
      if ( FALSE EQ bsmContext->isInitConfigSuccessful )
      {
        BS_ERROR_CONFIG_PROCEDURE_FAILED();
      }
      /* Continue with the next procedure */
      doDeviceInitProcedure(bsmContext);
    }
    break;
    case TIMERNAME_WAITINITIPADDRCOMPLETE:
    {
      BS_ERROR_SET_IP_ADDR_REQ_TIMEOUTED();
      if ( TRUE EQ getAlarmDetailsByBsErrorCode( BS_ERR_SET_IP_ADDR_FAILED, &alarmDetails ) )
      {
        strcat( alarmDetails.alarmLocation, getModuleName(MODID_DEH) );
        snprintf( alarmDetails.alarmAdditionalText, MAX_SIZE_ACTIVEALARMSTATUS_ALARMADDITIONALTEXT+1,
                  "%s", "No Response for SET_IP_ADDR_REQ" );
        doSaveAlarm( &alarmDetails );
      }
    }
    break;

    default:
    {
      BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType, getBsmProceduralStateName(bsmContext->bsmProceduralState) )
      break;
    }
  }
  return bsmContext->bsmProceduralState;
}


/**
 * @param   msg        Message received after timer expiry.
 * @param   bsmContext BSM Context
 * @details This function will handle timer expiry in WAIT_DEVICE_INIT state
 **/
inline BsmProceduralState bsmProcTimerExpiryInWaitDeviceInitCmpl(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    )
{
  MsgTimeoutInd *msgTimeoutInd= (MsgTimeoutInd*)msg->data;

  switch( msgTimeoutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITDEVICEINITCOMPLETE:
    {
      logError("%s","Timer TIMERNAME_WAITDEVICEINITCOMPLETE expired. DEH did not respond for DEVICE_INIT_REQ.");
      doCollectionInfoProcedureFailureSteps( "Response for Device request not received", bsmContext );
      /* Start Registration procedure */
      doEmsRegistration( bsmContext );
    }
    break;

    default:
    {
      BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType, getBsmProceduralStateName(bsmContext->bsmProceduralState) )
      break;
    }
  }
  return bsmContext->bsmProceduralState;
}
/**
 * @param   msg        Message received after timer expiry.
 * @param   bsmContext BSM Context
 * @details This function will handle timer expiry in WAIT_COLLECT_INFO state
 **/
inline BsmProceduralState bsmProcTimerExpiryInWaitCollectInfoCmpl(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    )
{
  MsgTimeoutInd *msgTimeoutInd= (MsgTimeoutInd*)msg->data;

  switch( msgTimeoutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITGPSPARAMETERSRSP:
    {
      doCollectionInfoProcedureFailureSteps( "Time out occurred while waiting for GPS details message", bsmContext );
      /* Even if parameters are not received from DEH module,
       * go ahead and do registration procedure */
      doEmsRegistration( bsmContext );
    }
    break;

    default:
    {
      BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType, getBsmProceduralStateName(bsmContext->bsmProceduralState) )
      break;
    }
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param   msg        Message received after timer expiry.
 * @param   bsmContext BSM Context
 * @details This function will handle timer expiry in WAIT_REGISTRATION_CMPL state
 **/
inline BsmProceduralState bsmProcTimerExpiryInWaitRegistrationCmpl(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    )
{
  MsgTimeoutInd *msgTimeoutInd= (MsgTimeoutInd*)msg->data;

  switch( msgTimeoutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITEMSREGRSP:
    {
      /* TODO: Add a count - which registration attempt is now started */
      BS_ERROR_EMS_REGISTRATION_CONFIRM_NOT_RECEIVED();
      /* Raise alarm */
      createEmsRegFailAlarm();
      /* Retry till success is received */
      doEmsRegistration( bsmContext );
      bsmCreateAndSendSetLedColor( LEDID_EMSREGISTRATION, LEDCOLOR_RED, bsmContext );
    }
    break;

    default:
    {
      BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType, getBsmProceduralStateName(bsmContext->bsmProceduralState) )
      break;
    }
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param   msg        Message received after timer expiry.
 * @param   bsmContext BSM Context
 * @details This function will handle timer expiry in WAIT_CONFIG_CMPL state
 **/
inline BsmProceduralState bsmProcTimerExpiryInWaitConfigCmpl(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    )
{
  MsgTimeoutInd *msgTimeoutInd= (MsgTimeoutInd*)msg->data;

  setBsmProceduralState( bsmContext, BS_IDLE );
  switch( msgTimeoutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITCONFIGCOMPLETE:
    {
       /* Timer expired and say some module did not respond */
      checkForConfigProcedureCompletion( bsmContext );
      if ( TRUE EQ bsmContext->isConfigSuccessful )
      {
        enbEnable( bsmContext );
      }
      else
      {
        BS_ERROR_CONFIG_PROCEDURE_FAILED();
        BS_ERROR_ENABLE_PROCEDURE_NOT_STARTED();
      }
    }
    break;

    default:
    {
      BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType, getBsmProceduralStateName(bsmContext->bsmProceduralState) )
      break;
    }
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param   msg        Message received after timer expiry.
 * @param   bsmContext BSM Context
 * @details This function will handle timer expiry in WAIT_ENABLE_CMPL state
 **/
inline BsmProceduralState bsmProcTimerExpiryInWaitEnableCmpl(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    )
{
  MsgTimeoutInd *msgTimeoutInd= (MsgTimeoutInd*)msg->data;

  switch( msgTimeoutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITENABLECOMPLETE:
    {
      /* Generate an alarm when timer expires and some module has not responded */
      ModuleId    moduleIdFailedToRespond = bsmContext->opStartOpStopInfo.sequence[bsmContext->opStartOpStopInfo.opStartSeqIndex-1];
      createConfigurationErrorAlarm( moduleIdFailedToRespond );
      /* Timer expired, means enable procedure is failed or it was not stopped after procedure completion */
      /* Here EnodeBOperationalState will remain disabled and adminState will be Unlocked */
      /* Send OPSTOP_REQ to modules which have responded to OPSTART_REQ to disable them */
      /* Start in the order in which they need to be sent OPSTOP */
      doDisableProcInitStepsWhenEnableIsInterrupted( bsmContext );
      if ( FALSE EQ sendOpStopReqMsg( &bsmContext->opStartOpStopInfo ) )
      {
        /* Disable procedure is completed. Move to idle state */
        setBsmProceduralState( bsmContext, BS_IDLE );
      }
    }
    break;
    default:
    {
      BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType, getBsmProceduralStateName(bsmContext->bsmProceduralState) )
      break;
    }
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param   msg        Message received after timer expiry.
 * @param   bsmContext BSM Context
 * @details This function will handle timer expiry in WAIT_DISABLE_CMPL state
 **/
inline BsmProceduralState bsmProcTimerExpiryInWaitDisableCmpl(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    )
{
  MsgTimeoutInd *msgTimeoutInd= (MsgTimeoutInd*)msg->data;

  switch( msgTimeoutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITDISABLECOMPLETE:
    {
      /* Check which module did not respond to the opstop request message, log an error
       * and continue sending opstop request to the next module in sequence
       * BSM will remain in disable state till all modules are not sent opstop */
      OpStartOpStopInfo *procInfo = &bsmContext->opStartOpStopInfo;
      if ( TRUE EQ procInfo->opStopCmpl )
      {
        /* Procedure is considered to be completed here */
        logInfo("%s", "Enodeb Disable procedure is completed!!");
        setBsmProceduralState( bsmContext, BS_IDLE );
      }
      else
      {
        ModuleId  moduleIdFailedToRespond = procInfo->sequence[procInfo->numModulesToSendOpStop];
        BS_ERROR_OPSTOP_SUCC_NOT_RCVD( getModuleName(moduleIdFailedToRespond) );
        startBsmProcTimer( TIMERNAME_WAITDISABLECOMPLETE, &bsmContext->bsmTimersInfo );
        sendOpStopReqMsg( &bsmContext->opStartOpStopInfo );
      }
    }
    break;
    case TIMERNAME_WAITMMELINKESTABLISH:
    {
      logInfo("%s", "Timer expired: TIMERNAME_WAITMMELINKESTABLISH ");
      if ( FALSE EQ isEnodeBLocked() )
      {
        /* Just restart the timer as disable procedure is in progress */
        startBsmProcTimer( TIMERNAME_WAITMMELINKESTABLISH, &bsmContext->bsmTimersInfo );
      }
    }
    break;
    default:
    {
      BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType, getBsmProceduralStateName(bsmContext->bsmProceduralState) )
      break;
    }
  }
  return bsmContext->bsmProceduralState;
}

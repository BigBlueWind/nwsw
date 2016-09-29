/**
 * @file    bsm_cmd_unlock.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains actions to be taken for Unlock/Enable procedure.
 *
 * @author:  Shweta Polepally
 * @date:    Sep 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param reason    reason for EnodeB enable
 * @details This function will return the reason in string Format.
 */
char *getEnodebEnableReasonName(
  IN EnbEnableReason  reason
  )
{
  switch( reason )
  {
    case ENABLE_REASON_USER_INITAITED_UNLOCK:
      return "ENABLE_REASON_USER_INITAITED_UNLOCK";
    case ENABLE_REASON_S1AP_RECOVERY:
      return "ENABLE_REASON_S1AP_RECOVERY";
    case ENABLE_REASON_COMPONENT_ALARM_CLEARED:
      return "ENABLE_REASON_COMPONENT_ALARM_CLEARED";
    default:
      logError("%s","No name for Enable Reason");
      break;
  }
  return "No name for Enable Reason";
}

/**
 * @param info     Information about the opStart/opstop maintained at BSM.
 * @details This function will send OPSTART REQ message to the modules.
 */
void sendOpStartReqMsg(
  IO OpStartOpStopInfo *info
  )
{
  if ( NUM_MODULES_OPSTART_STOP_SENT EQ info->opStartSeqIndex )
  {
    return;
  }
  ModuleId sendOpStartToModuleId = info->sequence[info->opStartSeqIndex];
  switch( sendOpStartToModuleId )
  {
    case MODID_S1M:
      createAndSendMsg( MSG_BSM_S1M_OPSTART_REQ, NULL, 0);
      break;
    case MODID_CEM:
      createAndSendMsg( MSG_BSM_CEM_OPSTART_REQ, NULL, 0);
      break;
    case MODID_DEH:
      createAndSendMsg( MSG_BSM_DEH_OPSTART_REQ, NULL, 0);
      break;
    case MODID_PMH:
      createAndSendMsg( MSG_BSM_PMH_OPSTART_REQ, NULL, 0);
      break;
    case MODID_UDH:
      createAndSendMsg( MSG_BSM_UDH_OPSTART_REQ, NULL, 0);
      break;
    default:
      logInfo("%s", "OPSTART REQ cannot to sent!! Case for this module is not handled. Programming error. Check ");
      break;
  }
  info->opStartSeqIndex++;

  if ( NUM_MODULES_OPSTART_STOP_SENT EQ info->opStartSeqIndex )
  {
    info->opStartCmpl = TRUE;
    logInfo("%s","Sending of OPSTART REQ message to all modules is completed !!");
  }
}

/**
 * @param info     Information about the opStart/opstop maintained at BSM.
 * @details This function will send OPSTOP REQ message to the modules.
 */
TBOOL sendOpStopReqMsg(
  IO OpStartOpStopInfo *info
  )
{
  BsmContext         *ctx = getBsmContext();
  MsgBsmModOpStopReq  opStopReq;
  clearBytes( &opStopReq, sizeof(MsgBsmModOpStopReq) );
  opStopReq.enbDisableReason = ctx->enbDisableReason;

  if ( 0 EQ info->numModulesToSendOpStop )
  {
    logInfo("%s","There is no need to roll back, as 1st module trying to OPSTART itself failed.." );
    return FALSE;
  }
  /* Send OPSTOP to modules which has started and responded to OPSTART REQ*/
  ModuleId sendOpStartToModuleId = info->sequence[info->numModulesToSendOpStop-1]; /* -1 as index starts from 0 */
  switch( sendOpStartToModuleId )
  {
    case MODID_S1M:
      createAndSendMsg( MSG_BSM_S1M_OPSTOP_REQ, (UINT8*)&opStopReq, sizeof(MsgBsmModOpStopReq));
      break;
    case MODID_CEM:
      createAndSendMsg( MSG_BSM_CEM_OPSTOP_REQ, (UINT8*)&opStopReq, sizeof(MsgBsmModOpStopReq));
      break;
    case MODID_DEH:
      createAndSendMsg( MSG_BSM_DEH_OPSTOP_REQ, (UINT8*)&opStopReq, sizeof(MsgBsmModOpStopReq));
      break;
    case MODID_PMH:
      createAndSendMsg( MSG_BSM_PMH_OPSTOP_REQ, (UINT8*)&opStopReq, sizeof(MsgBsmModOpStopReq));
      break;
    case MODID_UDH:
      createAndSendMsg( MSG_BSM_UDH_OPSTOP_REQ, (UINT8*)&opStopReq, sizeof(MsgBsmModOpStopReq));
      break;
    default:
      logInfo("%s", "OPSTOP REQ cannot to sent!! case for this module is not handled. Programming error. Check ");
      break;
  }
  info->numModulesToSendOpStop--;
  if ( 0 EQ info->numModulesToSendOpStop )
  {
    info->opStopCmpl = TRUE;
    logInfo("%s","OPStop sent to last module in sequence..");
  }
  return TRUE;
}

/**
 * @param   bsmContext  BSM context.
 * @details This function will do enable procedure completion steps.
 */
static void doEnableProcedureCompletionSteps(
  IO BsmContext       *bsmContext
  )
{
  stopBsmProcedureTimer( TIMERNAME_WAITENABLECOMPLETE );
  setBsmProceduralState( bsmContext, BS_IDLE );
  setOperationalStateInEnbStatusTable ( OPERATIONALSTATE_ENABLED );
  bsmCreateAndSendSetLedColor( LEDID_ENBOPERATIONAL, LEDCOLOR_GREEN, bsmContext );

  /* Send UEEVENTHISTORYLOGCONFIG to UEH */
  bsmSendUeEventHistoryLogConfigToUeh();
}

/**
 * @param   bsmContext  BSM context.
 * @details This function will do enable procedure completion steps when interrupted.
 */
void doEnableProcedureInterreptedSteps(
  IO BsmContext       *bsmContext
  )
{
  /* Step 1: Stop enable procedure timer as enable procedure is interrupted due to opstart failure
   *         received from some module */
  stopBsmProcedureTimer( TIMERNAME_WAITENABLECOMPLETE );

  /* Step 2: Check if opStop was already started. */
  doDisableProcInitStepsWhenEnableIsInterrupted( bsmContext );

  /* Step 3:*/
  if ( FALSE EQ sendOpStopReqMsg( &bsmContext->opStartOpStopInfo ) )
  {
    /* Disable procedure is completed. Move to idle state */
    setBsmProceduralState( bsmContext, BS_IDLE );
  }
}

/**
 * @param   opStartFail  Opstart Fail message.
 * @details This function will create S1AP opStart fail alarm.
 */
void createS1apOpStartFailAlarm(
  IN OpStartFail *opStartFail
  )
{
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));

  if ( TRUE EQ getAlarmDetailsByBsErrorCode( opStartFail->errorNum, &alarmDetails ) )
  {
    strcat( alarmDetails.alarmLocation, "MODID_S1M" );
    doSaveAlarm( &alarmDetails );
  }
}

/**
 * @param   msgId  Message ID
 * @param   ipAddr IP Address
 * @param   bsmContext BSM context.
 * @details This function will clear all S1AP alarms.
 */
void clearS1apAlarms(
  IN MsgId        msgId,
  IN UINT8        *ipAddr,
  IO BsmContext   *bsmContext
  )
{
  Index   alarmDbIndex;
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));

  for ( alarmDbIndex = 0; alarmDbIndex < bsmContext->afhCtx.alarmDbTable.numOfAlarms; alarmDbIndex++ )
  {
    if ( TRUE EQ getAlarmDetailsByClearingMsgId( msgId, alarmDbIndex, &alarmDetails ) )
    {
      if ( MMELINKDOWN EQ alarmDetails.alarmCause )
        strcat( alarmDetails.alarmLocation, ipAddr );

      doClearAlarm( &alarmDetails );
      clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));
    }
  }
}

/**
 * @param   moduleId  Module ID
 * @details This function will create configuration error alarm.
 */
void createConfigurationErrorAlarm(
  IN ModuleId   moduleId
  )
{
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));
  if ( TRUE EQ getAlarmDetailsByBsErrorCode( BS_ERR_CONFIGURATION_OR_CUSTOMIZATION_ERROR, &alarmDetails ) )
  {
    strcat( alarmDetails.alarmLocation, getModuleName(moduleId) );
    doSaveAlarm( &alarmDetails );
  }
}

/**
 * @param   msgId       Message ID
 * @param   moduleName  Module name
 * @param   bsmContext  BSM context.
 * @details This function will clear configuration alarm.
 */
void clearConfigurationErrorAlarm(
  IN MsgId         msgId,
  IN UINT8        *moduleName,
  IO BsmContext   *bsmContext
  )
{
  Index                alarmDbIndex;
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));

  for ( alarmDbIndex = 0; alarmDbIndex < bsmContext->afhCtx.alarmDbTable.numOfAlarms; alarmDbIndex++ )
  {
    if ( TRUE EQ getAlarmDetailsByClearingMsgId( msgId, alarmDbIndex, &alarmDetails ) )
    {
      if ( CONFIGURATIONORCUSTOMIZATIONERROR EQ alarmDetails.alarmCause )
      {
        strcat( alarmDetails.alarmLocation, moduleName );
        doClearAlarm( &alarmDetails );
        break;
      }
    }
  }
}


/**
 * @details This function will create EMS registration failure alarm.
 */
void createEmsRegFailAlarm()
{
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));
  if ( TRUE EQ getAlarmDetailsByBsErrorCode( BS_ERR_EMS_REGISTRATION_CONFIRM_NOT_RECEIVED, &alarmDetails ) )
  {
    doSaveAlarm( &alarmDetails );
  }
}

/**
 * @details This function will clear EMS registration failed alarm.
 */
void clearEmsRegFailedAlarm()
{
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));

  if ( TRUE EQ getAlarmDetailsByBsErrorCode( BS_ERR_EMS_REGISTRATION_CONFIRM_NOT_RECEIVED, &alarmDetails ) )
  {
    doClearAlarm( &alarmDetails );
  }
}

/**
 * @param msg        Message from S1AP
 * @param bsmContext BSM module related information.
 * @details This function will process MSG_S1AP_BSM_OPSTART_SUCC message and
 *          send OPSTART to UEH module.
 */
BsmProceduralState bsmProcS1apOpStartSuccInWaitEnableState(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  /* Note: All other alarms (expect CONFIGURATIONORCUTOMIZATIONERROR alarm) are getting cleared when LINK_UP is received */
  /* Clear CONFIGURATIONORCUSTOMIZATIONERROR alarm if previously generated */
  clearConfigurationErrorAlarm( msg->msgId, "MODID_S1M", bsmContext );

  setEnbEnableDisableParams( msg->msgId, TRUE, bsmContext );
  if ( TRUE EQ bsmContext->opStartOpStopInfo.opStartCmpl )
  {
    doEnableProcedureCompletionSteps( bsmContext );
  }
  else
  {
    sendOpStartReqMsg( &bsmContext->opStartOpStopInfo );
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param msg        Message from PRM
 * @param bsmContext BSM Context
 * @details This function will process MSG_UEH_BSM_OPSTART_SUCC message and sends
 *          MSG_BSM_PMH_OPSTART_REQ message to PMH Module
 */
BsmProceduralState bsmProcUehOpStartSuccInWaitEnableState(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  /* Clear FAIL alarm if previously generated */
  clearConfigurationErrorAlarm( msg->msgId, "MODID_CEM", bsmContext );

  setEnbEnableDisableParams( msg->msgId, TRUE, bsmContext );

  bsmUpdateCellOperationalStateInfo(getEnodebEnableReasonName(bsmContext->enbEnableReason));
  bsmSetIsOperationalInCellStatusTable(TRUE);

  /* Notify EMS if Cell is barrred */
  if(CELLCOMMAND_BAR EQ bsmContext->cellCmdRcvd)
  {
    sendEventCellBarredInd();
  }
  if ( TRUE EQ bsmContext->opStartOpStopInfo.opStartCmpl )
  {
    doEnableProcedureCompletionSteps( bsmContext );
  }
  else
  {
    sendOpStartReqMsg( &bsmContext->opStartOpStopInfo );
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param msg        Message from DEH
 * @param bsmContext BSM module related information.
 * @details This function will process MSG_DEH_BSM_OPSTART_SUCC message and send
 *          MSG_BSM_S1AP_OPSTART_REQ messge to S1AP
 */
BsmProceduralState bsmProcDehOpStartSuccInWaitEnableState(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  /* Clear FAIL alarm if previously generated */
  clearConfigurationErrorAlarm( msg->msgId, "MODID_DEH", bsmContext );

  setEnbEnableDisableParams( msg->msgId, TRUE, bsmContext );
  if ( TRUE EQ bsmContext->opStartOpStopInfo.opStartCmpl )
  {
    doEnableProcedureCompletionSteps( bsmContext );
  }
  else
  {
    sendOpStartReqMsg( &bsmContext->opStartOpStopInfo );
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param msg        Message from UDH
 * @param bsmContext BSM module related information.
 * @details This function will process MSG_UDH_BSM_OPSTART_SUCC message
 */
BsmProceduralState bsmProcUdhOpStartSuccInWaitEnableState(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  /* Clear FAIL alarm if previously generated */
  clearConfigurationErrorAlarm( msg->msgId, "MODID_UDH", bsmContext );

  setEnbEnableDisableParams( msg->msgId, TRUE, bsmContext );
  if ( TRUE EQ bsmContext->opStartOpStopInfo.opStartCmpl )
  {
    doEnableProcedureCompletionSteps( bsmContext );
  }
  else
  {
    sendOpStartReqMsg( &bsmContext->opStartOpStopInfo );
  }
  return bsmContext->bsmProceduralState;
}


/**
 * @param msg        Message from PMH
 * @param bsmContext BSM module related information.
 * @details This function will process MSG_PMH_BSM_OPSTART_SUCC message
 */
BsmProceduralState bsmProcPmhOpStartSuccInWaitEnableState(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  /* Clear FAIL alarm if previously generated */
  clearConfigurationErrorAlarm( msg->msgId, "MODID_PMH", bsmContext );

  setEnbEnableDisableParams( msg->msgId, TRUE, bsmContext );
  if ( TRUE EQ bsmContext->opStartOpStopInfo.opStartCmpl )
  {
    doEnableProcedureCompletionSteps( bsmContext );
  }
  else
  {
    sendOpStartReqMsg( &bsmContext->opStartOpStopInfo );
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param msg        Message from S1AP
 * @param bsmContext BSM module related information.
 * @details This function will process MSG_S1AP_BSM_OPSTART_FAIL message and starts disable procedure
 *           Enable Procedure is considered to be completed and failed here.
 */
BsmProceduralState bsmProcS1apOpStartFailInWaitEnableState(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  bsmContext->enbDisableReason = S1AP_OPSTART_FAIL;
  doEnableProcedureInterreptedSteps( bsmContext );

  if (  FALSE EQ bsmContext->isMmeLinkEstablished )
  {
    /* If Link is down Start timer and Retry to Establish the Link
     * on Expiry of the timer.
     */
    startBsmProcTimer( TIMERNAME_WAITMMELINKESTABLISH, &bsmContext->bsmTimersInfo );
    return bsmContext->bsmProceduralState;
  }
  /* Except PLMN Error which needs operator intervention, We retry to establish
   * The connection always.
   */
  OpStartFail *opStartFail=(OpStartFail*)msg->data;
  if ( opStartFail->errorNum NOTEQ BS_ERR_S1AP_MISC_UNKNOWN_PLMN )
  {
    /* If Guard timer Expired, We want to retry this Procedure,
     * So Start the TIMERNAME_WAITMMELINKESTABLISH
     */
    startBsmProcTimer( TIMERNAME_WAITMMELINKESTABLISH, &bsmContext->bsmTimersInfo );
  }

  /* Generate an alarm */
  createS1apOpStartFailAlarm( opStartFail );

  return bsmContext->bsmProceduralState;
}

/**
 * @param msg        Message from DEH
 * @param bsmContext BSM module related information.
 * @details This function will process MSG_DEH_BSM_OPSTART_FAIL message and moves to IDLE state.
 *          Procedure is considered to be completed and failed here.
 */
BsmProceduralState bsmProcDehOpStartFailInWaitEnableState(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  UNUSED(msg);
  /* Create opstart failed alarm */
  createConfigurationErrorAlarm( MODID_DEH );

  bsmContext->enbDisableReason = DEH_OPSTART_FAIL;
  doEnableProcedureInterreptedSteps( bsmContext );
  return bsmContext->bsmProceduralState;
}

/**
 * @param msg        Message from UEH
 * @param bsmContext BSM module related information.
 * @details This function will process MSG_UEH_BSM_OPSTART_FAIL message and starts the
 *          disable procedure.
 *          Enable Procedure is considered to be completed and failed here.

 */
BsmProceduralState bsmProcUehOpStartFailInWaitEnableState(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  UNUSED(msg);
  /* Create opstart failed alarm */
  createConfigurationErrorAlarm( MODID_CEM );

  bsmContext->enbDisableReason = UEH_OPSTART_FAIL;
  doEnableProcedureInterreptedSteps( bsmContext );
  return bsmContext->bsmProceduralState;
}

/**
 * @param msg        Message from PMH
 * @param bsmContext BSM module related information.
 * @details This function will process MSG_PMH_BSM_OPSTART_FAIL message
 */
BsmProceduralState bsmProcPmhOpStartFailInWaitEnableState(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  UNUSED(msg);
  /* Create opstart failed alarm */
  createConfigurationErrorAlarm( MODID_PMH );

  bsmContext->enbDisableReason = PMH_OPSTART_FAIL;
  doEnableProcedureInterreptedSteps( bsmContext );
  return bsmContext->bsmProceduralState;
}


/**
 * @param msg        Message from UDH
 * @param bsmContext BSM module related information.
 * @details This function will process MSG_UDH_BSM_OPSTART_FAIL message
 */
BsmProceduralState bsmProcUdhOpStartFailInWaitEnableState(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  UNUSED(msg);
  /* Create opstart failed alarm */
  createConfigurationErrorAlarm( MODID_UDH );

  bsmContext->enbDisableReason = PMH_OPSTART_FAIL;
  doEnableProcedureInterreptedSteps( bsmContext );
  return bsmContext->bsmProceduralState;
}

/**
 * @param   bsmContext BSM Module related Information
 * @details This function will carry out EnodeB Enable procedure.
 */
void enbEnable(
  IO BsmContext  *bsmContext
  )
{
  BsmTimersInfo   *timerInfo = getBsmTimersInfo();
  logInfo(" EnodeB Enable Reason : %s", getEnodebEnableReasonName(bsmContext->enbEnableReason) );
  /* LET CCPU Stacks create SAP and bind with each other */
  sleep(5);

  /* Enable the traces after config request is received */
   /* enableOrDisableProtocolTraces(); */

  /* Reset all internal context variables */
  clearBytes( &(bsmContext->enbEnableDisableParams), sizeof(EnbEnableDisableParams));

  /* Reset opStart and opstop values in the context and start enable procedure(Send Opstart one by one)*/
  bsmContext->opStartOpStopInfo.opStartSeqIndex        = 0;
  bsmContext->opStartOpStopInfo.numModulesToSendOpStop = 0;
  bsmContext->opStartOpStopInfo.opStartCmpl            = FALSE;
  bsmContext->opStartOpStopInfo.opStopCmpl             = FALSE;

  setBsmProceduralState( bsmContext, ENODEB_WAIT_ENABLE_CMPL );
  /* Start timer TIMERNAME_WAIT_ENABLE_COMPLETE to guard this procedure */
  startBsmProcTimer( TIMERNAME_WAITENABLECOMPLETE, timerInfo );

  /* Sequence of Opstart and opstop is defined when BSM is initialized */
  /* Start enable procedure here */
  sendOpStartReqMsg( &bsmContext->opStartOpStopInfo );
}

/**
 * @param   bsmContext BSM Module related Information
 * @details This function will carry out EnodeB Configuration procedure.
 */
static void enbConfig(
  IO BsmContext  *bsmContext
  )
{
  BsmTimersInfo   *timerInfo = getBsmTimersInfo();
  clearBytes(&(bsmContext->cfgRspFlags), sizeof(CfgRspFlags));

  setBsmProceduralState( bsmContext, ENODEB_WAIT_CONFIG_CMPL );

  bsmContext->configTransId++;  /* Everytime Config Req is sent, New transaction ID will be generated by BSM*/
                                /* As of now its just an incremented value */

  bsmContext->isConfigSuccessful           = FALSE;
  bsmContext->isCfgRspWithWrongTransIdRcvd = FALSE;

  bsmFillAndSendS1apCfgReqMsg();
  bsmFillAndSendDehRhCfgReqMsg();
  /* EnodeB configuration is the event on which SM will start configuring all the stacks with the
   * values provided by BSM */
  bsmFillAndSendUehEnbCfgReqMsg();
  bsmFillAndSendPmhCfgReqMsg();
  bsmFillAndSendUdhCfgReqMsg();
  bsmFillAndSendPmhCntrInputCfgReqMsg();

  /* Start timer TIMERNAME_WAITCONFIGCOMPLETE to guard this procedure */
  startBsmProcTimer( TIMERNAME_WAITCONFIGCOMPLETE, timerInfo );
}

/**
 * @param   bsmContext  BSM Module related Information
 * @details This function will check if the configured time and location source is same
 *          as the one getting used currently at the ENB or not.
 */
static TBOOL isConfiguredTimeAndLocationSourceSameAsCurrentlyUsed(
  IO BsmContext  *bsmContext
  )
{
  EnbRegistrationConfig  *config = (EnbRegistrationConfig*)getFirstRowOfTable(ENBREGISTRATIONCONFIG);
  if ( (TIMEANDLOCATIONSOURCETOUSE_GPS EQ config->timeAndLocationSourceToUse) &&
       ( DATEANDTIMESOURCE_GPS EQ bsmContext->dateAndTimeSource ) )
  {
    return TRUE;
  }
  else if ( (TIMEANDLOCATIONSOURCETOUSE_OPERCONFIGURED EQ config->timeAndLocationSourceToUse) &&
      ( DATEANDTIMESOURCE_CONFIGURED EQ bsmContext->dateAndTimeSource ) )
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
 * @param   bsmContext  BSM Module related Information
 * @details This function will check whether MME IP Address is configured or not
 */
TBOOL isMmeIpAddressConfigured()
{
  MmeConfig   *mmeConfig = (MmeConfig*)getFirstRowOfTable(MMECONFIG);
  if(0 EQ strncmp(mmeConfig->mmeIpAddr, "0.0.0.0", MAX_SIZE_MMECONFIG_MMEIPADDR + 1))
  {
    BS_ERROR_REQ_REJ_MME_IP_ADDRESS_NOT_CONFIGURED( mmeConfig->mmeIpAddr);
    return FALSE;
  }
  return TRUE;
}

/**
 * @param   bsmContext  BSM Module related Information
 * @details This function will check whether PLMN is configured or not
 */
TBOOL isPlmnConfigured()
{
  CellConfig  *cellConfig = (CellConfig*)getFirstRowOfTable(CELLCONFIG);
  if(0 EQ strncmp(cellConfig->plmnMCC, "000", MAX_SIZE_CELLCONFIG_PLMNMCC + 1))
  {
    BS_ERROR_REQ_REJ_MCC_VALUE_NOT_CONFIGURED(cellConfig->plmnMCC);
    return FALSE;
  }
  if(0 EQ strncmp(cellConfig->plmnMNC, "00", MAX_SIZE_CELLCONFIG_PLMNMNC + 1))
  {
    BS_ERROR_REQ_REJ_MNC_VALUE_NOT_CONFIGURED(cellConfig->plmnMNC);
    return FALSE;
  }
  return TRUE;
}

/**
 * @param   setObj     SET Object request.
 * @param   bsmContext BSM Module related Information.
 * @details This function will check for all preconditions to unlock enodeB.
 */
static TBOOL areAllPreConditionsMetForUnlock(
  IN SetObjReq   *setObj,
  IO BsmContext  *bsmContext
  )
{
  PhyConfig  *phyConfig = (PhyConfig*)getFirstRowOfTable(PHYCONFIG);
  BsmOamTableInfo *oamTableInfo = bsmGetTablePtrByTableId(PHYCONFIG);
  OamColInfo  *oamColInfo = &oamTableInfo->colInfo[COL_PHYCONFIG_TOTALTXPOWER];

  TBOOL         areAllPreConditionMet = TRUE;
  BsErrorValue  error;

  if ( FALSE EQ isEnodeBLocked() )
  {
    BS_ERROR_ENODEB_NOT_LOCKED();
    areAllPreConditionMet = FALSE;
    error = BS_ERR_ENODEB_NOT_LOCKED;
  }
  else if ( bsmContext->areAllModulesUp EQ FALSE )
  {
    BS_ERROR_REQ_REJ_ALL_MODULES_ARE_NOT_UP();
    areAllPreConditionMet = FALSE;
    error = BS_ERR_REQ_REJ_ALL_MODULES_ARE_NOT_UP;
  }
  else if ( bsmContext->isInitConfigSuccessful EQ FALSE )
  {
    BS_ERROR_REQ_REJ_INITIAL_CONFIGURATION_FAILED();
    areAllPreConditionMet = FALSE;
    error = BS_ERR_REQ_REJ_INITIAL_CONFIGURATION_FAILED;
  }
  else if( ( phyConfig->totalTxPower > oamColInfo->colMaxVal) || ( phyConfig->totalTxPower < oamColInfo->colMinVal ) )
  {
    BS_ERROR_TOTALTXPOWER_NOT_IN_RANGE( phyConfig->totalTxPower, oamColInfo->colMinVal, oamColInfo->colMaxVal );
    areAllPreConditionMet = FALSE;
    error = BS_ERR_TOTALTXPOWER_NOT_IN_RANGE;
  }
  else if ( bsmContext->emsRegSucc EQ FALSE )
  {
    BS_ERROR_EMS_REGISTRATION_CONFIRM_NOT_RECEIVED();
    areAllPreConditionMet = FALSE;
    error = BS_ERR_EMS_REGISTRATION_CONFIRM_NOT_RECEIVED;
  }
  else if ( 0 EQ getEnodeBId() )
  {
    BS_ERROR_INVALID_ENODEB_ID();
    areAllPreConditionMet = FALSE;
    error = BS_ERR_INVALID_ENODEB_ID;
  }
  else if ( bsmContext->isRhEnabled EQ FALSE )
  {
    BS_ERROR_REQ_REJ_RH_IS_NOT_ENABLED();
    areAllPreConditionMet = FALSE;
    error = BS_ERR_REQ_REJ_RH_IS_NOT_ENABLED;
  }
  else if( 0 NOTEQ getNumActiveCriticalAlarms() )
  {
    BS_ERROR_REQ_REJ_CRTICIAL_ALARMS_PRESENT( getNumActiveCriticalAlarms() );
    areAllPreConditionMet = FALSE;
    error = BS_ERR_REQ_REJ_CRTICIAL_ALARMS_PRESENT;
  }
  else if ( FALSE EQ isConfiguredTimeAndLocationSourceSameAsCurrentlyUsed(bsmContext) )
  {
    BS_ERROR_REQ_REJ_UNABLE_TO_USE_CONFIGURED_TIME_AND_LOCATION_SOURCE( );
    areAllPreConditionMet = FALSE;
    error = BS_ERR_REQ_REJ_GPS_PARA_EXPECTED_TO_BE_USED_BUT_NOT_RCVD;
  }
  else if ( FALSE EQ isMmeIpAddressConfigured() )
  {
    areAllPreConditionMet = FALSE;
    error = BS_ERR_REQ_REJ_MME_IP_ADDRESS_NOT_CONFIGURED;
  }
  else if ( FALSE EQ isPlmnConfigured() )
  {
    areAllPreConditionMet = FALSE;
    error = BS_ERR_REQ_REJ_PLMN_NOT_CONFIGURED;
  }

  if (FALSE EQ areAllPreConditionMet)
  {
    if(setObj) /* setObj will be NULL at the time of auto-unlock */
      bsmFillAndSendSetObjFail( setObj, error, &setObj->tlvArray.tlvInfo[0].tag );
    return FALSE;
  }
  return TRUE;
}

/**
 * @details This function will check if atleast one Euthra Neighbor is configured.
 */
static TBOOL isAtleastOneEutraNeighborConfigured()
{
  UINT8                  i = 0;
  EutraNeighCellConfig  *rowOfTable;

  for ( i=0; i < MAX_ROWS_IN_EUTRANEIGHCELLCONFIG_TABLE; i++ )
  {
     rowOfTable = (EutraNeighCellConfig*)getRowOfTable(EUTRANEIGHCELLCONFIG, i);
     if( TRUE EQ rowOfTable->isUsed )
     {
       return TRUE;
     }
  }
  return FALSE;
}

/**
 * @param   setObj SET Object request.
 * @param   bsmContext BSM Module related Information.
 * @details This function will carry out unlock procedure to enable the enodeB.
 */
void bsmDoBist(
  IN Msg         *msg,
  IN SetObjReq   *setObj,
  IO BsmContext  *bsmContext
  )
{
  UNUSED(bsmContext);
  /**
   * To Proceed with BIST, preconditions should be met.
   */
  if ( FALSE EQ isEnodeBLocked() )
  {
    BS_ERROR_ENODEB_NOT_LOCKED( );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_ENODEB_NOT_LOCKED, &setObj->tlvArray.tlvInfo[0].tag );
  }
  else
  {
    createAndSendMsg( MSG_BSM_DEH_SET_OBJ_REQ, msg->data, msg->len);
  }
}

/**
 * @param   setObj SET Object request.
 * @param   bsmContext BSM Module related Information.
 * @details This function will carry out unlock procedure to enable the enodeB.
 */
void bsmDoLongPost(
  IN Msg         *msg,
  IN SetObjReq   *setObj,
  IO BsmContext  *bsmContext
  )
{
  UNUSED(bsmContext);
  /**
  * To Proceed with BIST, preconditions should be met.
  */
  if ( FALSE EQ isEnodeBLocked() )
  {
    BS_ERROR_ENODEB_NOT_LOCKED( );
    bsmFillAndSendSetObjFail( setObj, BS_ERR_ENODEB_NOT_LOCKED, &setObj->tlvArray.tlvInfo[0].tag );
  }
  else
  {
    createAndSendMsg( MSG_BSM_DEH_SET_OBJ_REQ, msg->data, msg->len);
  }
}

/**
 * @param txPower is eNodeb total Transmit power in DL
 * @section This function sets Tx Attenuation in Radio Head configuration
 */
void bsmSetTxAttenInRhCfg(
     IN SINT8 txPower
     )
{
  BsmOamTableInfo *oamTableInfo = bsmGetTablePtrByTableId(PHYCONFIG);
  OamColInfo  *oamColInfo = &oamTableInfo->colInfo[COL_PHYCONFIG_TOTALTXPOWER];
  RadioHeadConfig *rhConfig = (RadioHeadConfig*)getFirstRowOfTable(RADIOHEADCONFIG);

  rhConfig->txAttenPathA = (oamColInfo->colMaxVal - txPower) * 10;
  rhConfig->txAttenPathB = (oamColInfo->colMaxVal - txPower) * 10;
}

/**
 * @param   setObj SET Object request.
 * @param   bsmContext BSM Module related Information.
 * @details This function will carry out unlock procedure to enable the enodeB.
 */
void bsmDoUnLock(
  IN SetObjReq   *setObj,
  IO BsmContext  *bsmContext
  )
{
  PhyConfig  *phyConfig = (PhyConfig*)getFirstRowOfTable(PHYCONFIG);
  /**
   * To Proceed with unlock, preconditions should be met.
   */
  if ( FALSE EQ areAllPreConditionsMetForUnlock( setObj, bsmContext ) )
  {
    /* Errors are logged inside the above function */
    return;
  }

  if ( TRUE EQ isEnodeBLocked() )   /* Locked condition */
  {
    if ( FALSE EQ isEnodeBEnabled() )
    {
      bsmContext->enbEnableReason = ENABLE_REASON_USER_INITAITED_UNLOCK;

      /* Locked and disabled condition */
      if(setObj)
        bsmFillAndSendSetObjSucc( setObj );
      setAdminStateInEnbStatusTable ( ADMINSTATE_UNLOCKED );
      /* Check If atleast one neighbor is present, else sent notification to SIH */
      if( FALSE EQ isAtleastOneEutraNeighborConfigured())
      {
         /* Send notification to SIH */
        sendEventEnbNoNeighborConfigured();
        logInfo("%s","Since no neighbor is configured, ENB shall not be able to perform HO.");
      }
      /* Calculate Refrerence signal power in phyconfig and set Tx attenuation in radio head config table */
      /*
       * TODO uncoment this when Tx power is fully tested
       */
      bsmCalculateRefSigPower(phyConfig);
      bsmSetTxAttenInRhCfg(phyConfig->totalTxPower);
      /* Configure and then enable enodeB */
      enbConfig( bsmContext );
      /* Note: enbEnable() will happen after enbConfig procedure is completed */
    }
    /* else - locked and enabled cannot be the condition */
  }
}

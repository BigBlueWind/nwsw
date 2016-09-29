/**
 * @file    bsm_init_exit.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions related to Init/Exit Procedure.
 *
 * @author:  Shweta Polepally
 * @date:    Sep 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param msg        Message from PRM
 * @param bsmContext BSM module information.
 * @details  This function handles ALL_MOD_UP indication sent by PRM and
 *           send message MSG_BSM_DEH_SET_IP_ADDR_REQ to DEH..
 */
BsmProceduralState bsmProcAllModulesUpInd(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  MsgPrmBsmAllModulesUpInd  *ind = (MsgPrmBsmAllModulesUpInd*)msg->data;

  /* This means, all expected modules are UP */
  logInfo( "All modules UP indication received from PRM.. Number Modules Up are: %d", ind->numModUp );
  bsmContext->areAllModulesUp = TRUE;

  setBsmProceduralState( bsmContext, BS_WAIT_INITIAL_CONFIG_CMPL );
  /* Note: For Host Environment (x86), skip the procedure of sending IP addresses to DEH.
   * For x86, IP address should be read from EnodebConfig file.
   * Hence continue with Init Config Procedure. */
#ifndef HOST_ENV
  bsmCreateAndSendSetIpAddrReq();
  startBsmProcTimer( TIMERNAME_WAITINITIPADDRCOMPLETE, &bsmContext->bsmTimersInfo );
#else
  EnbStatus           *enbStatus    = (EnbStatus*)getFirstRowOfTable(ENBSTATUS);
  EnodebConfig        *enodebConfig = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
  snprintf( enbStatus->enodebIpAddress, MAX_SIZE_ENBSTATUS_ENODEBIPADDRESS + 1 , "%s", enodebConfig->enodebStaticIpAddr );
  doInitConfigProcedure( bsmContext );
#endif
  return ( bsmContext->bsmProceduralState );
}

/**
 * @details  This function returns the message name based on the module id given
 */
MsgId getMsgId(
  IN ModuleId modId
  )
{
  if(((MODID_EGTP <= modId) && (MODID_UDH >= modId)) || (MODID_UEH EQ modId))
  {
    return MSG_BSM_SM_SET_LOG_CFG;
  }
  else
  {
    switch( modId )
    {
      case MODID_FTH: return MSG_BSM_FTH_SET_LOG_CFG;
      case MODID_PRM: return MSG_BSM_PRM_SET_LOG_CFG;
      case MODID_DEH: return MSG_BSM_DEH_SET_LOG_CFG;
      case MODID_LTH: return MSG_BSM_LTH_SET_LOG_CFG;
      case MODID_PMH: return MSG_BSM_PMH_SET_LOG_CFG;
      case MODID_SIH: return MSG_BSM_SIH_SET_LOG_CFG;
      case MODID_WIH: return MSG_BSM_WIH_SET_LOG_CFG;
      default:
        logError("getMsgId: Failed to return MsgId as ModuleId is invalid. (Received ModId:%d)", modId);
      break;
    }
  }
  return MSG_UNDEFINED;
}

/**
 * @details  This function reads and sends the logconfig info to all
 *           modules at the BS start time (i.e, after getting ALL_MODULES_UP ind from PRM
 */
void bsmInitSetLogLevelProcedure()
{
  ModuleId        modId;
  SetLogConfig    setLogCfg;

  LogContext *logContext = getLogContext();

  /* Send SET_LOG_CFG to all modules except CIH & BSM (along with S1M n CEM which don't have provision for setting loglevel */
  for (modId = MODID_SIH; modId <= MODID_UDH; modId++)
  {
    LogConfig  *logConfig = (LogConfig*)getRowOfTable(LOGCONFIG, modId-1);

    if (MODID_BSM NOTEQ modId)
    {
      copyBytes(logConfig, sizeof(LogConfig), &setLogCfg);
      createAndSendMsg( getMsgId(modId), (UINT8*)&setLogCfg, sizeof(SetLogConfig));
    }
    else
    {
      copyBytes(logConfig, sizeof(LogConfig), &(logContext->logConfig));
    }
  }

}

/**
 * @param msg        Message from PRM
 * @param bsmContext BSM module information.
 * @details  This function handles MOD_UP indication sent by PRM.
 *           MSG_BSM_SIH_NOTI_IND with id=ALARM_ID_SW_MOD_UP will be posted to SIH.
 */
BsmProceduralState bsmProcModUpInd(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  Index                alarmDbIndex;
  MsgPrmBsmModUpInd   *upInd = (MsgPrmBsmModUpInd*)msg->data;
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));
  logInfo("%s Module is Up !! ", getModuleName(upInd->moduleId) );

  /* loop is run over the entire DB so that all alarm details related to the clearing Message ID
   * is taken from the DB and cleared accordingly */
  for ( alarmDbIndex = 0; alarmDbIndex < bsmContext->afhCtx.alarmDbTable.numOfAlarms; alarmDbIndex++ )
  {
    if ( TRUE EQ getAlarmDetailsByClearingMsgId( msg->msgId, alarmDbIndex, &alarmDetails ) )
    {
      strcat( alarmDetails.alarmLocation, getModuleName(upInd->moduleId) );
      snprintf(alarmDetails.alarmAdditionalText, sizeof(String256),"%s", upInd->errorDescForPrevModDown.failReason);
      doClearAlarm( &alarmDetails );
    }
  }
  if ( 0 EQ getNumModDownAlarms())
  {
    bsmContext->areAllModulesUp = TRUE;
  }
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param msg        Message from PRM
 * @param bsmContext BSM module information.
 * @details  This function handles MOD_UP indication sent by PRM.
 *           BSM will disable the enodeB, if enabled.
 *           MSG_BSM_SIH_NOTI_IND with id=ALARM_ID_SW_MOD_DOWN will be posted to SIH.
 */
BsmProceduralState bsmProcModDownInd(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgPrmBsmModDownInd  *downInd = (MsgPrmBsmModDownInd*)msg->data;
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));

  if ( TRUE EQ getAlarmDetailsByBsErrorCode( downInd->errorDesc.errorNum, &alarmDetails ) )
  {
    strcat( alarmDetails.alarmLocation, getModuleName(downInd->moduleId) );
    if( NULL NOTEQ downInd->errorDesc.failReason )
    {
      snprintf( alarmDetails.alarmAdditionalText, MAX_SIZE_ACTIVEALARMSTATUS_ALARMADDITIONALTEXT+1,
                "%s", downInd->errorDesc.failReason );
    }
    doSaveAlarm( &alarmDetails );
  }

  bsmContext->areAllModulesUp = FALSE;

  if(alarmDetails.alarmSeverity EQ ALARMSEVERITY_CRITICAL)
  {
    /* If the BSM procedural state is in BS_WAIT_INIT_CMPL, no need of initiating enbDisable process
     * Reason is: EnodeB will already be disabled during INIT process */
    bsmContext->enbDisableReason = MOD_DOWN_OCCURED;
    if ( TRUE EQ isEnodeBEnabled() )
      enbDisable( TRUE, bsmContext );   /* Note: Here in this function, state will be changed
                                                    to ENODEB_WAIT_DISABLE_CMPL state */
  }
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param msg        Message from PRM
 * @param bsmContext BSM module information.
 * @details  This function handles MOD_UP indication sent by PRM.
 *           BSM will disable the enodeB, if enabled.
 *           MSG_BSM_SIH_NOTI_IND with id=ALARM_ID_SW_MOD_DOWN will be posted to SIH.
 */
BsmProceduralState bsmProcModDownIndInInitComplState(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgPrmBsmModDownInd  *downInd = (MsgPrmBsmModDownInd*)msg->data;
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));

  if ( TRUE EQ getAlarmDetailsByBsErrorCode( downInd->errorDesc.errorNum, &alarmDetails ) )
  {
    strcat( alarmDetails.alarmLocation, getModuleName(downInd->moduleId) );
    if( NULL NOTEQ downInd->errorDesc.failReason )
    {
      snprintf( alarmDetails.alarmAdditionalText, MAX_SIZE_ACTIVEALARMSTATUS_ALARMADDITIONALTEXT+1,
                "%s", downInd->errorDesc.failReason );
    }
    doSaveAlarm( &alarmDetails );
  }

  logInfo(" Module Down Indication received in %s state. Initialisation procedure failed. Continue with Next procedure ", getBsmProceduralStateName(bsmContext->bsmProceduralState) );
  bsmContext->areAllModulesUp = FALSE;

  if(MODID_DEH EQ downInd->moduleId)
  {
    logError("%s", "DEH module is DOWN so can not set IP Address. This is a fatal error.");
  }
  else
  {
    setBsmProceduralState( bsmContext, BS_WAIT_INITIAL_CONFIG_CMPL );
  /* Note: For Host Environment (x86), skip the procedure of sending IP addresses to DEH.
   * For x86, IP address should be read from EnodebConfig file.
   * Hence continue with Init Config Procedure. */
#ifndef HOST_ENV
    bsmCreateAndSendSetIpAddrReq();
    startBsmProcTimer( TIMERNAME_WAITINITIPADDRCOMPLETE, &bsmContext->bsmTimersInfo );
#else
    EnbStatus           *enbStatus    = (EnbStatus*)getFirstRowOfTable(ENBSTATUS);
    EnodebConfig        *enodebConfig = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
    snprintf( enbStatus->enodebIpAddress, MAX_SIZE_ENBSTATUS_ENODEBIPADDRESS + 1 , "%s", enodebConfig->enodebStaticIpAddr );
    doInitConfigProcedure( bsmContext );
#endif
  }
  return ( bsmContext->bsmProceduralState );
}

BsmProceduralState bsmProcModDownIndInInitStates(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgPrmBsmModDownInd  *downInd = (MsgPrmBsmModDownInd*)msg->data;
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));

  logInfo(" Module Down Indication received in %s state. Raise alarm ", getBsmProceduralStateName(bsmContext->bsmProceduralState) );
  if ( TRUE EQ getAlarmDetailsByBsErrorCode( downInd->errorDesc.errorNum, &alarmDetails ) )
  {
    strcat( alarmDetails.alarmLocation, getModuleName(downInd->moduleId) );
    if( NULL NOTEQ downInd->errorDesc.failReason )
    {
      snprintf( alarmDetails.alarmAdditionalText, MAX_SIZE_ACTIVEALARMSTATUS_ALARMADDITIONALTEXT+1,
                "%s", downInd->errorDesc.failReason );
    }
    doSaveAlarm( &alarmDetails );
  }
  bsmContext->areAllModulesUp = FALSE;

  return ( bsmContext->bsmProceduralState );
}

BsmProceduralState bsmProcS1apLinkClosedInd(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  UNUSED(msg);
  bsmContext->isMmeLinkEstablished = FALSE;
  if ( bsmContext->numOfMmesConnected > 0 )
    bsmContext->numOfMmesConnected--;
  logInfo("Num of MME's connected are : %d ", bsmContext->numOfMmesConnected );
  return ( bsmContext->bsmProceduralState );
}

static TBOOL isWaitMmeLinkEstablishTimerRunning(
  void
  )
{
  BsmContext  *bsmContext = getBsmContext();
  UINT8        i;

  for ( i=0; i< bsmContext->bsmTimersInfo.totalTimersInMod; i++ )
  {
    if ( TIMERNAME_WAITMMELINKESTABLISH EQ bsmContext->bsmTimersInfo.modTimerInfo[i].timerDesc.timerType )
    {
      return checkTimerStatus(bsmContext->bsmTimersInfo.modTimerInfo[i].timerId );
    }
  }
  return FALSE;
}

/**
 * @param msg        Message received from S1AP
 * @param bsmContext BSM module information.
 * @details This function handles LINK_UP message from S1AP module.
 *          BSM will enable the enodeB.
 *          MSG_BSM_SIH_NOTI_IND with id=ALARM_ID_S1AP_LINK_UP will be posted to SIH.
 */
BsmProceduralState bsmProcS1apLinkUpInd(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgS1apBsmLinkUpInd  *upInd =  (MsgS1apBsmLinkUpInd*)msg->data;
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));

  bsmContext->isMmeLinkEstablished = TRUE;

  bsmContext->numOfMmesConnected++;
  logInfo("Num of MME's connected are : %d ", bsmContext->numOfMmesConnected );

  /* loop is run over the entire DB so that all alarm details related to the clearing Message ID
   * is taken from the DB and cleared accordingly */

  clearS1apAlarms( msg->msgId, upInd->mmeIpAddr , bsmContext );

  if ( TRUE EQ isWaitMmeLinkEstablishTimerRunning() )
  {
    stopBsmProcedureTimer( TIMERNAME_WAITMMELINKESTABLISH );
  }
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param msg        Message received from S1AP
 * @param bsmContext BSM module information.
 * @details This function handles LINk_DOWN message from S1AP module.
 *          BSM will disable the enodeB, if enabled.
 *          MSG_BSM_SIH_NOTI_IND with id=ALARM_ID_S1AP_LINK_DOWN will be posted to SIH.
 */
BsmProceduralState bsmProcS1apLinkDownInd(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  /* If EnodeB is already locked, it means that operator wants a MME link to be down, so ignore
   * the message */
  if ( TRUE EQ isEnodeBLocked() )
    return ( bsmContext->bsmProceduralState );
  

  /* Step 1: Create and alarm and store it in  alarm active table */
  MsgS1apBsmLinkDownInd  *downInd =  (MsgS1apBsmLinkDownInd*)msg->data;
  CreateAlarmDetails   mmeLinkDownAlarm;
  clearBytes( &mmeLinkDownAlarm, sizeof(CreateAlarmDetails));

  if ( TRUE EQ getAlarmDetailsByBsErrorCode( BS_ERR_S1AP_MME_LINK_DOWN, &mmeLinkDownAlarm ) )
  {
    strcat( mmeLinkDownAlarm.alarmLocation, downInd->mmeIpAddr );
    doSaveAlarm( &mmeLinkDownAlarm );
  }

  /* Step2: Set enableDisbale variable for S1AP */
  setEnbEnableDisableParams( msg->msgId, FALSE, bsmContext );

  if ( bsmContext->numOfMmesConnected > 0 )
    bsmContext->numOfMmesConnected--;
  logInfo("Num of MME's connected are : %d ", bsmContext->numOfMmesConnected );

  /* if number of MMEs to connect with is greater than link down alarms, just return
   * and wait till LINK_DOWN/LINK_UP is received */
  if ( bsmContext->maxMmesToConnectWith > getNumS1apActiveLinkDownAlarms() )
  {
    return ( bsmContext->bsmProceduralState );
  }

  /* Step 3: If the max num of mme's to connect and numS1apLinkDownAlarms are equal
   *         Create alarm- All Mme link down
   *         Start Disable procedure
   **/
  bsmContext->isMmeLinkEstablished = FALSE;

  /* Create all mme link down alarm */
  CreateAlarmDetails   allMmeLinkDownAlarm;
  clearBytes( &allMmeLinkDownAlarm, sizeof(CreateAlarmDetails));
  if ( TRUE EQ getAlarmDetailsByBsErrorCode( BS_ERR_S1AP_ALL_MME_LINK_DOWN, &allMmeLinkDownAlarm ) )
  {
    doSaveAlarm( &allMmeLinkDownAlarm );
  }
  /* If enodeB was enabled previously, disable it and start timer for recovery */
  if ( TRUE EQ isEnodeBEnabled() )
  {
    bsmContext->enbDisableReason = S1AP_LINK_DOWN;
    enbDisable( TRUE, bsmContext ); /* Note: Here in this function, state will be changed to ENODEB_WAIT_DISABLE_CMPL state */
  }
  else
  {
    /* If enable is in progress, terminate the enable procedure and start disable here */
    if ( ENODEB_WAIT_ENABLE_CMPL EQ bsmContext->bsmProceduralState )
    {
      bsmContext->enbDisableReason = S1AP_LINK_DOWN;
      doEnableProcedureInterreptedSteps( bsmContext );
    }
  }
  /* Start a timer. On expiry of this timer, BSM should check if the S1AP link has come
   * up. This will be done by doing enable procedure */
  startBsmProcTimer( TIMERNAME_WAITMMELINKESTABLISH, &bsmContext->bsmTimersInfo );

  setOperationalStateInEnbStatusTable( OPERATIONALSTATE_DISABLED );
  return ( bsmContext->bsmProceduralState );
}


BsmProceduralState bsmProcUehUnknownEnbUeId(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{

  /* Create and alarm and store it in  alarm active table */
  MsgUehBsmUnknownEnbUeS1apId  *ind =  (MsgUehBsmUnknownEnbUeS1apId*)msg->data;
  CreateAlarmDetails   unknownEnbUeId;
  String256 alarmLocation;
  
  clearBytes( &alarmLocation, sizeof(alarmLocation)); 
  clearBytes( &unknownEnbUeId, sizeof(CreateAlarmDetails));

  if ( TRUE EQ getAlarmDetailsByBsErrorCode( BS_ERR_S1AP_RADIONW_UNKNOWN_ENB_UE_S1AP_ID, &unknownEnbUeId ) )
  {
    strcpy( alarmLocation, unknownEnbUeId.alarmLocation);
    snprintf(unknownEnbUeId.alarmLocation, MAX_SIZE_ACTIVEALARMSTATUS_ALARMLOCATION+1 , "%s->%d", alarmLocation, ind->ueId );
    doSaveAlarm( &unknownEnbUeId );
  }

  return ( bsmContext->bsmProceduralState );
}

/**
 * @param msg        Message received from S1AP
 * @param bsmContext BSM module information.
 * @details This function handles RESET_IND message from S1AP module.
 *          BSM will notify SIH about this event.
 *          State is unchanged.
 */
BsmProceduralState bsmProcS1apResetInd(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  MsgS1apBsmS1ResetInd *resetInd = (MsgS1apBsmS1ResetInd*)msg->data;
  sendEventMmeReset( resetInd->mmeIpAddr );
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param msg        Message received from PRM Module
 * @param bsmContext BSM module information.
 * @details This function handles EXIT message received from PRM.
 *           BSM will get terminated.
 */
inline BsmProceduralState  bsmProcExitReqInAllStates(
  IN Msg         *msg,
  IO BsmContext  *bsmContext
  )
{
  UNUSED(bsmContext);
  MsgPrmModExitCmd  *exitCmd = (MsgPrmModExitCmd*)msg->data;
  logInfo("BSM exited due to :%d", exitCmd->modExitCause);
  /* Log the Exit Cause here */
  modExit(MODID_BSM, &(getModuleContext()->exeName[0]));
  exit(EXIT_SUCCESS);
}


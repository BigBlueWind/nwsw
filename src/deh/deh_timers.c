/**
 * @file    deh_timers.c
 * @brief This file is contains code get detail from GPS module
 * @details Copyright Powerwave Technologies, Inc., 2011
 *
 * @author   Shweta Polepally
 * @date     25-May-2011
 * @details Change History:
 */

#include "deh.h"

/**
 * @param   timerType  type of timer
 * @param   dehContext DEH Context
 * @details This function creates and starts Procedure Timer.
 */
void startDehTimer(
  IN TimerName       timerType,
  IO DehContext      *dehContext,
  IO UINT32          *userInfo
  )
{
  UINT8   i = 0;
  for ( i=0; i< dehContext->dehTimersInfo.totalTimersInMod; i++ )
  {
    if (timerType EQ dehContext->dehTimersInfo.modTimerInfo[i].timerDesc.timerType )
    {
      break;
    }
  }
  /* Create and start Timer */
  dehContext->dehTimersInfo.modTimerInfo[i].timerId = createAndStartTimer( &dehContext->dehTimersInfo.modTimerInfo[i].timerDesc, NULL, NULL );
  dehContext->dehTimersInfo.modTimerInfo[i].timerDesc.userInfo = *userInfo;
}

/**
 * @param timerType  Type of timer
 * @details This function is used to stop Timeout Timer.
 */
void  stopDehTimer(
  IO TimerName   type
  )
{
  UINT8            timerIndex;
  String50         buf;

  clearBytes( &buf, sizeof(String50));
  DehContext *ctx = getDehContext();

  for ( timerIndex=0; timerIndex< ctx->dehTimersInfo.totalTimersInMod; timerIndex++ )
  {
    if (ctx->dehTimersInfo.modTimerInfo[timerIndex].timerDesc.timerType EQ type )
    {
      stopTimer( ctx->dehTimersInfo.modTimerInfo[timerIndex].timerId );
      break;
    }
  }
}

/**
 * @param totalTimers  Total number of timers used in DEH
 * @param config  TimerConfig, Timers Information read from Configuration file
 * @details This function is used to store timers information into DEH context
 */

static void dehStoreTimersInCtx(
  IN UINT8         totalTimers,
  IN TimersConfig  *config
  )
{
  UINT8         i =0, index=0;
  DehTimersInfo *ctxTimerInfo = getDehTimersInfo();

  for ( i=0; i < totalTimers; i++ )
  {
    if ( config->moduleId EQ MODID_DEH )
    {
      ctxTimerInfo->modTimerInfo[index].timerDesc.moduleId          = config->moduleId;
      ctxTimerInfo->modTimerInfo[index].timerDesc.timeSpec.duration = config->duration;
      ctxTimerInfo->modTimerInfo[index].timerDesc.timeSpec.timeUnit = config->timeUnit;
      ctxTimerInfo->modTimerInfo[index].timerDesc.timerType         = config->timerName;
      ctxTimerInfo->modTimerInfo[index].timerDesc.timeoutMsgId      = MSG_TIMEKEEPER_DEH_TIMEOUT_IND;
      ctxTimerInfo->totalTimersInMod++;
      index++;
    }
    config++;
  }
}

/**
 * @param configDir  This is the configuration path, either will be NULL / or some valid path
 * @details This function will read the BS timers configuration file.
 */
void readDehTimersFromCfg(
  IN UINT8       *configFilePath
  )
{
  UINT8       isError = FALSE;
  UINT8       fullFilePath[MAX_BUFFER_SIZE];

  GenericTable  *tableFunc       = getTableFunctions(TIMERSCONFIG);
  TimersConfig  *rowOfTimerTable = (TimersConfig*)getFirstRowOfTable(TIMERSCONFIG);
  clearBytes( &fullFilePath, sizeof(fullFilePath));
  constructFullPath ( configFilePath, "enb_pre_config.cfg", fullFilePath );

  if ( NULL EQ tableFunc->getReadFromFileFunc )
  {
    BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("BSTIMERSCONFIG");
    exit(EXIT_FAILURE);
  }
  else
  {
    DehContext *ctx = getDehContext();
    ctx->totalTimers = tableFunc->getReadFromFileFunc( fullFilePath, rowOfTimerTable, &isError );
    /* Copy DEH timers in DEH context */
    rowOfTimerTable = (TimersConfig*)getFirstRowOfTable(TIMERSCONFIG);
    dehStoreTimersInCtx( ctx->totalTimers, rowOfTimerTable );
    if ( isError EQ TRUE )
    {
      BS_ERROR_CONFIG_READ_FAILED()
      exit(EXIT_FAILURE);
    }
  }
}

/**
 * @details This function is used to change Context of Global Commands Status for Timers i.e used two pointers
 * Pointing to 2 Structures that contains Present and Previous context of commands with respect to timer start time
 */
static void changeStatusInfo()
{
  DeviceStatusContext *deviceStatusContext = getDeviceStatusContext();
  SINT16 index = 0;
  
  /*Updating the last saved status with the present Status */
  for(index = 1; index < MAX_ROWS_IN_COMPONENTSTATUS_TABLE + 1; index++)
  {
    deviceStatusContext->lastSavedDevStatus.devConnectionStatus[index] = deviceStatusContext->presentDevStatus.devConnectionStatus[index];
    strncpy(deviceStatusContext->lastSavedDevStatus.errorString[index], deviceStatusContext->presentDevStatus.errorString[index], MAX_SIZE_COMPONENTSTATUS_ERRORSTRING );
  }

  /*temp = deviceStatusContext->presentStatus;
  clearBytes(&deviceStatusContext->lastSavedDevStatus, sizeof(DevStatus));
  deviceStatusContext->presentStatus = deviceStatusContext->lastSaved;
  deviceStatusContext->lastSaved   = temp;*/
}

/**
 * @details This function will processStatus Report TImeout
 */
static void processStatusReportTimeout()
{
  /**
   * @todo need to det status of all devices and send the response@n
   * Currently we are assuming that there is no operational state change of any device@n
   * so now no message MSG_DEH_BSM_DEVICE_UP_IND/MSG_DEH_BSM_DEVICE_DOWN_IND is sent to BSM */

  UINT32  userInfo = 0;
  UINT8   result = FALSE;
  DehContext   *dehContext = getDehContext();

  /* Changes Present state Device info to Last Saved Device Info */
  changeStatusInfo();

  /* Updating the Present state Device Info */
  dehUpdateDeviceStatus();

  /* Send ChangeIndication to BSM if Device status changes */
  dehBsmReportDeviceChangeStatus();

  /* Starting the Device Status Timer */
  startDehTimer( TIMERNAME_WAITDEHSTATUSREPORTPERIOD, dehContext, &userInfo );

  /*Sending Operating Mode Info REQ Msg to check the status of RadioHead Device*/
  dehContext->isRhRspDeviceStatusReceived = FALSE;
  result = dehRhOperatingModeInfo();

  if( result EQ TRUE )
  {
    logInfo("Operating Mode Info Request message sent to Check RH Device Status", NULL);
  }
  else
  {
    logError("Failed to send Operating Mode Info Request message to Check RH Device Status", NULL);
  }
}

/**
 * @details This function is called on Radio head Reset timer Expire
 **/
static void doActionsOnWaitRhInitTimeout()
{
  DehContext *dehContext = getDehContext();
  /**
   * @todo This part can be Raise an Alarm and Stop Base station
   * Since without INIT Base station may not going to work
   */
  /* Retrieve the command Reference from Global context for sending the response to the same Interface
   * From which Reset request is received*/
  DehRhRspTimersContext  *timersContext = getRhRspTimersContext();
  if(timersContext->rhInitTimerInfo.dehCmdType EQ RSP_USER_INITIATED)
  {
    /* Send Set OBJ Fail RSP To BSM */
    BS_ERROR_RH_RESET_TIMER_EXPIRED("TIMERNAME_WAITRHINIT",1);
    BS_ERROR_RH_RSP_NOT_RECEIVED("DEH","ResetRh",getDehStateName(dehContext->dehState));
    dehFillAndSendSetObjFail( &timersContext->rhInitTimerInfo.setReq, BS_ERR_RH_RSP_NOT_RECEIVED, NULL );

    clearBytes(&timersContext->rhInitTimerInfo.setReq , sizeof(SetObjReq));
  }
  timersContext->rhInitTimerInfo.dehCmdType = RSP_UNEXPECTED;
  /**
   * @note No Need to Update the Global Context Since before stating Timer@n
   * Global context is already updated to RH Connected Status = FALSE
   */
}
/**
 * @details This function is called on Device INIT  timer Expire
 **/
void createAndSendComponentStatusToBsm()
{
  SINT16 deviceno;
  DehContext    *dehContext = getDehContext();
  UINT8 index = 0;
  UINT8 initStatus = TRUE;
  MsgDehBsmDeviceInitSucc  initSucc;
  DevStatus *presentStatus  = getDevicePresentStatusPtr();
  clearBytes(&initSucc, sizeof(MsgDehBsmDeviceInitSucc));

  ComponentStatus   *rowOfTable = (ComponentStatus*)getFirstRowOfTable(COMPONENTSTATUS);


  /* Updating component status table and present status in DEH Global Context */
  initSucc.numOfDevices = MAX_ROWS_IN_COMPONENTSTATUS_TABLE;
  for ( deviceno = DEV_BASEBAND; deviceno < DEV_MAX; deviceno++,index++)
  {
    if( DEV_CONNECTED EQ presentStatus->devConnectionStatus[rowOfTable->index])
    {
    	initSucc.componentOpState[index].componentOperationalState = COMPONENTOPERATIONALSTATE_UP;
    }
    else if( DEV_UNCONNECTED EQ presentStatus->devConnectionStatus[rowOfTable->index])
    {
      initStatus = FALSE;
      initSucc.componentOpState[index].componentOperationalState = COMPONENTOPERATIONALSTATE_DOWN;
      initSucc.componentOpState[index].bsErrorForDeviceDown.errorNum = dehAlarmMapping(deviceno);
      strncpy( initSucc.componentOpState[index].bsErrorForDeviceDown.failReason, presentStatus->errorString[rowOfTable->index], MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1);
    }
    else if( DEV_NOTMONITORED EQ presentStatus->devConnectionStatus[rowOfTable->index])
    {
    	initSucc.componentOpState[index].componentOperationalState = COMPONENTOPERATIONALSTATE_NOTMONITORED;
    }
    else
    {
      initSucc.componentOpState[index].componentOperationalState = COMPONENTOPERATIONALSTATE_UNKNOWN;
      initSucc.componentOpState[index].bsErrorForDeviceDown.errorNum = dehAlarmMapping(deviceno);
      strncpy( initSucc.componentOpState[index].bsErrorForDeviceDown.failReason, presentStatus->errorString[rowOfTable->index], MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1);
    }
    initSucc.componentOpState[index].componentType              = rowOfTable->componentType;
    initSucc.componentOpState[index].componentInstance          = rowOfTable->componentInstance;
    getNextRowOfTable(rowOfTable);
  }

  /*
   *  Note: While sending dEV_INIT_FAIL - Fill the error string in the format
   *  BS_ERR_XXX:<errNum>. This will be used while creating the alarms at BSM .
   * */
  if ( TRUE EQ initStatus )
  {
    createAndSendMsg( MSG_DEH_BSM_DEVICE_INIT_SUCC, (UINT8*)&initSucc, sizeof(MsgDehBsmDeviceInitSucc) );
    dehContext->isBsmDehDeviceInitReqReceived = FALSE;
  }
  else
  {
    createAndSendMsg( MSG_DEH_BSM_DEVICE_INIT_FAIL, (UINT8*)&initSucc, sizeof(MsgDehBsmDeviceInitSucc) );
    dehContext->isBsmDehDeviceInitReqReceived = FALSE;
  }
}

/**
 * @details This function is called on Radio head Disable Timer Expire
 */
static void doActionsOnWaitDisableRspTimeout()
{
  RhDisableRspInfo  *rhDisableRspInfo = getDisableRspInfo();
  DehContext *dehContext = getDehContext();
  rhDisableRspInfo->noOfMsgsWaitingForRsp = 0;

  createAndSendMsg(MSG_DEH_BSM_OPSTOP_SUCC, NULL, 0 );
  dehContext->isUnlocked = FALSE;
  dehSetState(dehContext, DEH_STATE_IDLE);
  setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
  return;
}

/**
 * @details This function is called on Radio head Enable Timer Expire
 */
static void doActionsOnWaitEnableRspTimeout()
{
  RhEnableRspInfo  *rhEnableRspInfo = getEnableRspInfo();
  DehContext *dehContext = getDehContext();
  rhEnableRspInfo->noOfMsgsWaitingForRsp = 0;

  MsgDehBsmOpstartFail opstartFail;
  clearBytes(&opstartFail, sizeof(MsgDehBsmOpstartFail));
  /**
   * @todo Raise an Alarm -- Radio head must send Response with in timer expiry -- Check timer time
   */
  opstartFail.errorNum = BS_ERR_RH_CONFIG_FAILED;
  snprintf((char*)&opstartFail.failReason, sizeof(opstartFail.failReason), "%s", " OP-Start Failed Due to Timeout ");
  createAndSendMsg(MSG_DEH_BSM_OPSTART_FAIL, (UINT8*)&opstartFail, sizeof(MsgDehBsmOpstartFail));
  dehContext->isUnlocked = FALSE;
  dehSetState(dehContext, DEH_STATE_IDLE);
  setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
  return;
}

/**
 * @details This function is called on Radio head Config  Timer Expire
 */
static void doActionsOnWaitConfigRspTimeout()
{
  RhConfigRspInfo   *rhConfigRspInfo = getConfigRspInfo();
  DehContext *dehContext = getDehContext();
  /* Send Set OBJ Fail RSP To BSM */
  BS_ERROR_RH_CONFIG_TIMER_EXPIRED("TIMERNAME_WAITRHCONFIGRSP",rhConfigRspInfo->noOfMsgsWaitingForRsp);
  BS_ERROR_RH_RSP_NOT_RECEIVED("DEH","ResetRh",getDehStateName(dehContext->dehState));
  dehFillAndSendSetObjFail( &rhConfigRspInfo->setReq, BS_ERR_RH_RSP_NOT_RECEIVED, NULL );
  rhConfigRspInfo->noOfMsgsWaitingForRsp = 0;
  clearBytes(&rhConfigRspInfo->setReq , sizeof(SetObjReq));
  return;
}

/**
 * @details This function is called on Radio head Status  Timer Expire
 */
static void doActionsOnWaitStatusRspTimeoutForCih()
{
  RhStatusRspInfo   *rhStatusRspInfo = getStatusRspInfo(OAMIF_CIH);
  DehContext *dehContext = getDehContext();
  /* Send Get OBJ Fail RSP To BSM */
  BS_ERROR_RH_STATUS_TIMER_EXPIRED("TIMERNAME_WAITRHSTATUSRSPFORCIH",rhStatusRspInfo->noOfMsgsWaitingForRsp);
  BS_ERROR_RH_RSP_NOT_RECEIVED("DEH","ResetRh",getDehStateName(dehContext->dehState));
  dehFillAndSendGetObjFail( &rhStatusRspInfo->getReq, BS_ERR_RH_RSP_NOT_RECEIVED );
  rhStatusRspInfo->noOfMsgsWaitingForRsp = 0;
  clearBytes(&rhStatusRspInfo->getReq , sizeof(GetObjReq));
  return;
}

/**
 * @details This function is called on Radio head Status  Timer Expire
 */
static void doActionsOnWaitStatusRspTimeoutForSih()
{
  RhStatusRspInfo   *rhStatusRspInfo = getStatusRspInfo(OAMIF_SIH);
  DehContext *dehContext = getDehContext();
  /* Send Get OBJ Fail RSP To BSM */
  BS_ERROR_RH_STATUS_TIMER_EXPIRED("TIMERNAME_WAITRHSTATUSRSPFORSIH",rhStatusRspInfo->noOfMsgsWaitingForRsp);
  BS_ERROR_RH_RSP_NOT_RECEIVED("DEH","ResetRh",getDehStateName(dehContext->dehState));
  dehFillAndSendGetObjFail( &rhStatusRspInfo->getReq, BS_ERR_RH_RSP_NOT_RECEIVED );
  rhStatusRspInfo->noOfMsgsWaitingForRsp = 0;
  clearBytes(&rhStatusRspInfo->getReq , sizeof(GetObjReq));
  return;
}

/**
 * @details This function is called on Radio head Status  Timer Expire
 */
static void doActionsOnWaitStatusRspTimeoutForWih()
{
  RhStatusRspInfo   *rhStatusRspInfo = getStatusRspInfo(OAMIF_WIH);
  DehContext *dehContext = getDehContext();
  /* Send Get OBJ Fail RSP To BSM */
  BS_ERROR_RH_STATUS_TIMER_EXPIRED("TIMERNAME_WAITRHSTATUSRSPFORWIH",rhStatusRspInfo->noOfMsgsWaitingForRsp);
  BS_ERROR_RH_RSP_NOT_RECEIVED("DEH","ResetRh",getDehStateName(dehContext->dehState));
  dehFillAndSendGetObjFail( &rhStatusRspInfo->getReq, BS_ERR_RH_RSP_NOT_RECEIVED );
  rhStatusRspInfo->noOfMsgsWaitingForRsp = 0;
  clearBytes(&rhStatusRspInfo->getReq , sizeof(GetObjReq));
  return;
}

/**
 * @details This function is called on BIST Timer Expire
 */
static void doActionsOnWaitBistCompleteTimeout()
{
  DehBistContext  *dehBistContext = getGlobalDehBistContext();

  if( pthread_cancel( g_bistThread ) NOTEQ 0 )
  {
	logError("%s", "BIST Thread cancellation failed");
  }

  pthread_mutex_lock( &g_bistMutex );
  dehBistContext->isBistRunning = FALSE;
  dehBistContext->isBistPerformed = TRUE;
  pthread_mutex_unlock( &g_bistMutex );
  return;
}
/**
 * @details This function check the cpri link status on OPSTART Process
 */
static void doActionsOnWaitCpriLinkUp()
{
  DehContext *dehContext = getDehContext();
  UINT32 userInfo = 0;
  if(dehContext->noOfCpriReqSent < CPRI_REQ)
  {
    dehRhOperatingModeInfo();
    dehContext->noOfCpriReqSent += 1;
    startDehTimer( TIMERNAME_WAITCPRILINKUP, dehContext, &userInfo );
  }
  else
  {
    MsgDehBsmOpstartFail opstartFail;
    clearBytes(&opstartFail,sizeof(MsgDehBsmOpstartFail));
    /* Change DEH State to DEH_STATE_WAIT_DEV_DISABLE_RSP */

    dehSetState(dehContext, DEH_STATE_IDLE);
    opstartFail.errorNum = BS_ERR_CPRI_LINK_DOWN;
    snprintf((char*)&opstartFail.failReason, sizeof(opstartFail.failReason)," CPRI Link is not established");
    createAndSendMsg(MSG_DEH_BSM_OPSTART_FAIL, (UINT8*)&opstartFail, sizeof(MsgDehBsmOpstartFail));
    dehContext->isUnlocked = FALSE;
  }
}
/**
 * @param   msg  Message received from BSM
 * @param   dehContext DEH Context
 * @details This function will process Messages on Timer expire in All states other than Init State
 */
inline DehState dehProcTimerExpiry(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
  MsgTimeoutInd *msgTimeoutInd= (MsgTimeoutInd*)msg->data;

  switch( msgTimeoutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITDEHSTATUSREPORTPERIOD:
    {
      processStatusReportTimeout();
      break;
    }
    case TIMERNAME_WAITRHINIT:
    {
      /* This timer is expected to expiry only in these 3 states */
      if((dehContext->dehState EQ DEH_STATE_WAIT_CFG) ||
         (dehContext->dehState EQ DEH_STATE_WAIT_INIT_NOTI) ||
         (dehContext->dehState EQ DEH_STATE_IDLE))
      {
        doActionsOnWaitRhInitTimeout();
      }
      else
      {
        BS_ERROR_TIMER_EXPIRED_IN_WRONG_STATE("DEH", "TIMERNAME_WAITRHINIT", dehContext->dehState);
      }
      break;
    }
    case TIMERNAME_WAITALLDEVICERSP:
    {
      setProcedureCompleted(DEV_RADIOHEAD,PROC_DEV_INIT);
      dehContext->isBsmDehDeviceInitReqReceived = FALSE;
      createAndSendComponentStatusToBsm();
      break;
    }
    case TIMERNAME_WAITRHDISABLERSP:
    {
      doActionsOnWaitDisableRspTimeout();
      break;
    }
    case TIMERNAME_WAITRHENABLERSP:
    {
      doActionsOnWaitEnableRspTimeout();
      break;
    }
    case TIMERNAME_WAITRHCONFIGRSP:
    {
      doActionsOnWaitConfigRspTimeout();
      break;
    }
    case TIMERNAME_WAITRHSTATUSRSPFORCIH:
    {
      doActionsOnWaitStatusRspTimeoutForCih();
      break;
    }
    case TIMERNAME_WAITRHSTATUSRSPFORWIH:
    {
      doActionsOnWaitStatusRspTimeoutForWih();
      break;
    }
    case TIMERNAME_WAITRHSTATUSRSPFORSIH:
    {
      doActionsOnWaitStatusRspTimeoutForSih();
      break;
    }
    case TIMERNAME_WAITBISTCOMPLETE:
    {
      doActionsOnWaitBistCompleteTimeout();
      break;
    }
    case TIMERNAME_WAITCPRILINKUP:
    {
      doActionsOnWaitCpriLinkUp();
      break;
    }
    default:
    {
      BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType, getDehStateName(dehContext->dehState) );
      break;
    }
  }
  return dehContext->dehState;
}


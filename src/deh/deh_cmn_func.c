 /**
 * @file    deh_cmn_func.c
 * @brief   This file will handle all the messages coming from different modules in any DEH state.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author  mkata, spolepally, kgummad, aagrawal
 * @date    03-06-2011
 * @details Contains functions related to@n
 *          1) Device Status@n
 *          3) Device Shutdown@n
 *          2) DEH Context@n
 *          3) DEH Health Check@n
 *          4) DEH Default/Initial Configuration@n
 *          5) OPSTART Procedure@n
 *          6) OPSTOP Procedure@n
 *
 */

#include "deh.h"

/**
 * @param state This parameter contains DEh state info .
 * @details This functions gets the DEH state name
 */
char *getDehStateName(
  IN DehState state
  )
{
  switch( state )
  {
    case DEH_STATE_UNDEFINED:              return "DEH_STATE_UNDEFINED";
    case DEH_STATE_WAIT_CFG:               return "DEH_STATE_WAIT_CFG";
    case DEH_STATE_WAIT_INIT_NOTI:         return "DEH_STATE_WAIT_INIT_NOTI";
    case DEH_STATE_IDLE:                   return "DEH_STATE_IDLE";
    case DEH_STATE_WAIT_DEV_ENABLE_RSP:    return "DEH_STATE_WAIT_DEV_ENABLE_RSP";
    case DEH_STATE_WAIT_DEV_DISABLE_RSP:   return "DEH_STATE_WAIT_DEV_DISABLE_RSP";
    default:
    	BS_ERROR_UNKNOWN_ENUM_VALUE("No name defined for this DEH state", state);
      break;
  }
  return "No name defined for this DEH state";
}

/********************************************************************/
/**************************   Device Status       *******************/
/********************************************************************/

/**
 * @details  This function is used to update the status of all the devices.
 */
void dehUpdateDeviceStatus()
{

  SINT16 deviceno;
  DehContext *dehContext = getDehContext();
  DevStatus *presentStatus  = getDevicePresentStatusPtr();
  ComponentStatus *rowOfTable = (ComponentStatus*)getFirstRowOfTable(COMPONENTSTATUS);

  /* Process the timeout */
  /* Updating the present status of each device in DEH Context */
  /* Starting from BASEBAND to CLOCK */
  for ( deviceno = DEV_BASEBAND; deviceno < MAX_ROWS_IN_COMPONENTSTATUS_TABLE + 1; deviceno++ )
  {
    if(isDeviceMonitored(deviceno))
    {
      presentStatus->devConnectionStatus[deviceno] = dehGetStatusFromDevice(deviceno);
      if( presentStatus->devConnectionStatus[deviceno] NOTEQ DEV_CONNECTED )
      {
        dehGetDeviceErrorInfo( deviceno, presentStatus->errorString[deviceno] );
      }
      else
      {
        clearBytes(presentStatus->errorString[deviceno], MAX_SIZE_COMPONENTSTATUS_ERRORSTRING );
      }
    }
    else
    {
      presentStatus->devConnectionStatus[deviceno] = DEV_NOTMONITORED;
      clearBytes(presentStatus->errorString[deviceno], MAX_SIZE_COMPONENTSTATUS_ERRORSTRING );
    }
  }

  /* Updating the Component Status Table with status of all devices */
  /* Starting from BASEBAND to CLOCK */
  for ( deviceno = DEV_BASEBAND; deviceno < MAX_ROWS_IN_COMPONENTSTATUS_TABLE + 1; deviceno++ )
  {
    if( DEV_CONNECTED EQ presentStatus->devConnectionStatus[deviceno])
    {
      rowOfTable->componentOperationalState = COMPONENTOPERATIONALSTATE_UP;
      strncpy(rowOfTable->errorString, "", MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1 );
      if( deviceno EQ DEV_BASEBAND )
      {
    	if( dehContext->bistCtx.isBistRunning EQ TRUE )
        {
          strncpy(rowOfTable->errorString, "BIST In Progress", MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1 );
        }
      }
      if( deviceno EQ DEV_TEMPSENSOR)/*Adds the temperature of Baseband to info string*/
      {
    	snprintf( rowOfTable->infoString, MAX_SIZE_COMPONENTSTATUS_INFOSTRING + 1, "%s%s%.2f%s", dehGetDeviceInfo(DEV_TEMPSENSOR)," Temp = ", dehContext->temperature," C" );
      }

    }
    else if( DEV_UNCONNECTED EQ presentStatus->devConnectionStatus[deviceno])
    {
      rowOfTable->componentOperationalState = COMPONENTOPERATIONALSTATE_DOWN;
      strncpy(rowOfTable->errorString, presentStatus->errorString[deviceno], MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1);
    }
    else if( DEV_NOTMONITORED EQ presentStatus->devConnectionStatus[deviceno])
    {
      rowOfTable->componentOperationalState = COMPONENTOPERATIONALSTATE_NOTMONITORED;
      strncpy(rowOfTable->errorString, "", MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1 );
    }
    else
    {
      rowOfTable->componentOperationalState = COMPONENTOPERATIONALSTATE_UNKNOWN;
      strncpy(rowOfTable->errorString, presentStatus->errorString[deviceno], MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1);
    }
    getNextRowOfTable(rowOfTable);
  }
}

/**
 * @details  This function is used to note the alarm status of a device during DEH Init.
 *
 */
void dehUpdateDeviceAlarmStatus()
{
  Device deviceno;
  BsErrorValue errNum;
  DevStatus         *presentStatus  = getDevicePresentStatusPtr();

  for ( deviceno = DEV_BASEBAND; deviceno < MAX_ROWS_IN_COMPONENTSTATUS_TABLE + 1; deviceno++)
  {
    /* If there is any change in Status Then we need to send Message to BSM */
    if( DEV_CONNECTED EQ presentStatus->devConnectionStatus[deviceno] )
    {
      if( TRUE EQ dehIsAlarmPresent(deviceno))
      {
        errNum = dehAlarmMapping(deviceno);
        dehSetDeviceAlarmInfo(deviceno,FALSE);
      }
    }
    else if( DEV_UNCONNECTED EQ presentStatus->devConnectionStatus[deviceno])
    {
      if( FALSE EQ dehIsAlarmPresent(deviceno) )
      {
        errNum = dehAlarmMapping(deviceno);
        dehSetDeviceAlarmInfo(deviceno,TRUE);
      }
    }
  }
}

/**
 * @details  This function is used to send change indication/alarm to
 * BSM if the status of a device changes.
 */
void dehBsmReportDeviceChangeStatus()
{
  Device deviceno;
  BsErrorValue errNum;
  DevStatus         *presentStatus  = getDevicePresentStatusPtr();

  for ( deviceno = DEV_BASEBAND; deviceno < MAX_ROWS_IN_COMPONENTSTATUS_TABLE + 1; deviceno++)
  {
    /* If there is any change in Status Then we need to send Message to BSM */
    if( DEV_CONNECTED EQ presentStatus->devConnectionStatus[deviceno] )
    {
      if( TRUE EQ dehIsAlarmPresent(deviceno))
      {
        errNum = dehAlarmMapping(deviceno);
        dehSetDeviceAlarmInfo(deviceno,FALSE);
        dehCreateAndSendComponentUpInd(deviceno,errNum,NULL);
      }
    }
    else if( DEV_UNCONNECTED EQ presentStatus->devConnectionStatus[deviceno])
    {
      if( FALSE EQ dehIsAlarmPresent(deviceno) )
      {
        errNum = dehAlarmMapping(deviceno);
        dehSetDeviceAlarmInfo(deviceno,TRUE);
        dehCreateAndSendComponentDownInd(deviceno,errNum, presentStatus->errorString[deviceno],NULL);
      }
    }
  }
}

/**
 * @param  deviceId is the name of the device(Ex : CPRI,LK,WIFI...)
 * @details  This function is used to find the status of the device.
 */
DevConnectionStatus dehGetStatusFromDevice(
  IO Device device
  )
{
  TBOOL status = FALSE;

  /* Remove the dummy return for these devices when supported in Platform API */
  /* Enable Baseband after uboot with POST is supported */
  switch(device)
  {
    case  DEV_BASEBAND:
      status = dehIsBasebandActive();
      break;
    case  DEV_RADIOHEAD:
      status = dehIsRadioheadActive();
      break;
    case  DEV_BACKHAULPORT:
      status = dehIsEthernetLinkActive(ETHERNETLINK_BACK_HAUL);
      break;
    case  DEV_DEBUGPORT:
      status = dehIsEthernetLinkActive(ETHERNETLINK_DEBUG_PORT);
      break;
    case  DEV_TEMPSENSOR:
      status = dehIsTempSensorActive();
      break;
    case  DEV_GPS:
      status = dehIsGpsActive();
      break;
    case  DEV_CPRI:
      {
        DevConnectionStatus cpriStatus = dehIsCpriActive();
        return cpriStatus;
      }
      break;
    case  DEV_CLOCK:
      status = dehIsCdce62005ClockActive();
      break;
    default:
      break;
  }
  if ( TRUE EQ status )
  {
    setConnected(device);
    return DEV_CONNECTED;
  }
  setNotConnected(device);
  return DEV_UNCONNECTED;
}


/**
 * @param  device is the name of the device(Ex : CPRI,LK,WIFI...)
 * @param  deviceErrorDetail contains the Device Error Detail.
 * @details  This function is used to get the Info/Description of the device.
 */
void dehGetDeviceErrorInfo(
  IO Device device,
  IO UINT8 *deviceErrorDetail
  )
{
  /* Remove When GPS Device is Used and APIs are available for ethernet status */
  /* Enable Baseband after uboot with POST is supported */
  if (!isDeviceMonitored(device))
  {
	strncpy( deviceErrorDetail, "Unknown", MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1 );
	return;
  }
  switch(device)
  {
   case  DEV_BASEBAND:
	  snprintf(deviceErrorDetail, MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1, "%s%s", dehBasebandPostErrorString(), dehBasebandBistErrorString());
      break;
    case  DEV_RADIOHEAD:
      strncpy( deviceErrorDetail, dehRadioHeadErrorString(), MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1 );
      break;
    case  DEV_BACKHAULPORT:
      strncpy( deviceErrorDetail, dehEthernetLinkErrorString(ETHERNETLINK_BACK_HAUL), MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1 );
      break;
    case  DEV_DEBUGPORT:
      strncpy( deviceErrorDetail, dehEthernetLinkErrorString(ETHERNETLINK_DEBUG_PORT), MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1 );
      break;
    case  DEV_TEMPSENSOR:
      strncpy( deviceErrorDetail, dehTempSensorErrorString(), MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1 );
      break;
    case  DEV_GPS:
      strncpy( deviceErrorDetail, dehGpsErrorString(), MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1 );
      break;
    case  DEV_CLOCK:
      strncpy( deviceErrorDetail, dehCdce62005ErrorString(), MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1 );
      break;
    default:
      //strncpy( deviceStatus->errorString, dehIsBasebandActive(), MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1 );
      break;
  }
}

/**
 * @param  device is the name of the device(Ex : CPRI,LK,WIFI...)
 * @details  This function is used to get the Information of the device.
 */
UINT8* dehGetDeviceInfo(
  IO Device device
  )
{
  switch(device)
  {
    case  DEV_BASEBAND:
      return "Pico Baseband";
    case  DEV_RADIOHEAD:
      return dehRadioheadInfo();
    case  DEV_BACKHAULPORT:
      return "Backhaul Interface";
    case  DEV_DEBUGPORT:
      return "Debug Interface";
    case  DEV_TEMPSENSOR:
      return "Baseband Temperature Sensor";
    case  DEV_GPS:
      return "Picocell GPS Device";
    case  DEV_CPRI:
      return "CPRI Towards Radiohead";
    case  DEV_CLOCK:
      return "Clock Generator/Jitter Cleaner";
    case  DEV_MAX:
      return "Not Available";
  }
  return "Not Available";
}

/**
 * @param  deviceId is the name of the device(Ex : CPRI,LK,WIFI...)
 * @details  This function is used to find the Firmware Version of the device.
 */
UINT8* dehGetDeviceVersion(
  IO Device device
  )
{
  DehContext *dehContext = getDehContext();
  switch(device)
  {
    case  DEV_BASEBAND:
    case  DEV_BACKHAULPORT:
    case  DEV_DEBUGPORT:
    case  DEV_TEMPSENSOR:
    case  DEV_GPS:
    case  DEV_CPRI:
    case  DEV_CLOCK:
      return dehContext->baseBandversionInfo.hwRev;
    case  DEV_RADIOHEAD:
      return dehContext->rhFwInfoTable.version;
    case  DEV_MAX:
      return "Not Available";
  }
  return "Not Available";
}

/********************************************************************/
/**************************         Sw Exit       *******************/
/********************************************************************/
/**
 * @param   msgId        Received message Id
 * @details This function will shutdown Radio head Device
 **/
void shutDownRh(
  IN MsgId msgId
  )
{
  logInfo("ShutDown Radio Head for Message Received from PRM with MsgId = [%d] \n", msgId);
}
/**
 * @param   msgId        Received message Id
 * @details This function will shutdown GPS Device
 **/
void shutdownGps(
  IN MsgId msgId
  )
{
  logInfo("ShutDown GPS for Message Received from PRM with MsgId = [%d] \n", msgId);
}

/**
 * @param   msgId        Received message Id
 * @details This function will shutdown Wifi Device
 **/
void shutdownWiFi(
  IN MsgId msgId
  )
{
  logInfo("ShutDown WiFi Device for Message Received from PRM with MsgId = [%d] \n", msgId);
}

/**
 * @param   msg Received message.
 * @param   dehContext DEH Context.
 * @details This function will process Exit message received in all states of DEH.
 **/
inline DehState dehProcExitInAllStates(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
   UNUSED(msg);
   UNUSED(dehContext);
   /* Need to call actual functions to shutdown devices
    * Stop all timers Running
    * Send Failure Messages if pending messages present
    **/
    //shutDownRh(msgId);
    dehGpsReset();
    //shutdownWiFi(msgId);
    modExit(MODID_DEH, &(getModuleContext()->exeName[0]));
    exit(EXIT_SUCCESS);
}


/**
 * @param   msg        Received message
 * @param   dehContext DEH Context
 * @details This function will process Set Message Trace message received from BSM
 **/
inline DehState dehProcSetMsgTraceInAllStates(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
  MsgBsmDehSetMsgTrace   *msgTrace    = (MsgBsmDehSetMsgTrace*)msg->data;
  MsgDesc                *msgDesc        = getMsgDesc(msgTrace->msgId);

  /* Set the values in the table */
  msgDesc->msgTraceLevel     = msgTrace->msgTraceLevel;
  msgDesc->msgTraceDirection = msgTrace->msgTraceDirection;
  return dehContext->dehState;
}

/**
 * @param   msg        Received message
 * @details This function will process Set Log Level message received from BSM
 **/
inline DehState dehProcSetLogCfgInAllStates(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
  MsgBsmDehSetLogCfg *updateLogConfig = (MsgBsmDehSetLogCfg *)msg->data;
  LogContext  *logContext  = getLogContext();
  copyBytes(updateLogConfig, sizeof(LogConfig), &(logContext->logConfig));
  logInfo("%s", "Loglevel is set to new values");
  return dehContext->dehState;
}

/********************************************************************/
/**************************      Health Check     *******************/
/********************************************************************/

/**
 * @param   msg        Received message
 * @param   dehContext DEH Context
 * @details This function will process Hello message received in all states of DEH
 **/
inline DehState dehProcHelloInAllStates(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
  MsgPrmModHello     *hello = (MsgPrmModHello*)msg->data;
  MsgModPrmHelloAck   helloAck;
  clearBytes( &helloAck, sizeof(MsgModPrmHelloAck) );
  helloAck.helloAckSeqNum = hello->helloSeqNum;

  createAndSendMsg( MSG_DEH_PRM_HELLO_ACK, (UINT8*)&helloAck, sizeof(MsgModPrmHelloAck) );

  return dehContext->dehState;
}
/***************** Health Check End Here ***********************/

/********************************************************************/
/******************       DEH Configuration            **************/
/********************************************************************/

/**
 * @param   msg        Received message
 * @param   dehContext DEH Context
 * @details This function will process Initial Configuration message received from BSM in all states of DEH.
 **/
DehState dehProcInitConfigReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
  MsgBsmDehRhInitConfigReq *msgRhConfigReq = (MsgBsmDehRhInitConfigReq *)msg->data;
  MsgDehBsmRhInitConfigSucc rhConfigSucc;

  clearBytes(&rhConfigSucc, sizeof(MsgDehBsmRhInitConfigSucc));

  /* Fill The Global Data base with the Data received from BSM */
  RhDefaultCfg    *rhDefaultCfg = getRhDefaultCfg();

  rhDefaultCfg->radioHeadProfileId = 1;
  rhDefaultCfg->c1IQMap = msgRhConfigReq->c1IQMap;
  rhDefaultCfg->txEarfcn = msgRhConfigReq->txEarfcn;
  rhDefaultCfg->rxEarfcn = msgRhConfigReq->rxEarfcn;
  rhDefaultCfg->txAttenPathA = msgRhConfigReq->txAttenPathA;
  rhDefaultCfg->txAttenPathB = msgRhConfigReq->txAttenPathB;
  rhDefaultCfg->lowpowerAlarmThreshold = msgRhConfigReq->lowpowerAlarmThreshold;
  rhDefaultCfg->overpowerAlarmThreshold = msgRhConfigReq->overpowerMinorAlarmThreshold;
  rhDefaultCfg->vswrShutdownAlarmThreshold = msgRhConfigReq->vswrShutdownAlarmThreshold;
  rhDefaultCfg->rxDelay = msgRhConfigReq->rxDelay;
  rhDefaultCfg->txDelay = msgRhConfigReq->txDelay;

  /* Updating the values in Table Context */
  dehUpdateRhConfigTable();

  /* Send Configuration Success To BSM */
  rhConfigSucc.transId = msgRhConfigReq->transId;
  createAndSendMsg(MSG_DEH_BSM_INIT_CONFIG_SUCC, (UINT8*)&rhConfigSucc, sizeof(MsgDehBsmRhInitConfigSucc));
  if(dehContext->dehState EQ DEH_STATE_WAIT_CFG)
  {
    /* Change DEH State to DEH_STATE_IDLE */
    if(!isConnected(DEV_RADIOHEAD))
    {
      dehSetState(dehContext, DEH_STATE_WAIT_INIT_NOTI);
    }
    else
    {
      dehSetState(dehContext, DEH_STATE_IDLE);
    }
  }
  return dehContext->dehState;
}

/**
 * @details This function updates Radio head Configuration table.
 **/

DehState dehProcConfigReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
  MsgBsmDehRhConfigReq *msgRhConfigReq = (MsgBsmDehRhConfigReq *)msg->data;
  MsgDehBsmRhConfigSucc  rhConfigSucc;

  clearBytes(&rhConfigSucc, sizeof(MsgDehBsmRhConfigSucc));

  /* Fill The Global Data base with the Data received from BSM */
  RhDefaultCfg    *rhDefaultCfg = getRhDefaultCfg();

  rhDefaultCfg->c1IQMap = msgRhConfigReq->c1IQMap;
  rhDefaultCfg->txAttenPathA = msgRhConfigReq->txAttenPathA;
  rhDefaultCfg->txAttenPathB = msgRhConfigReq->txAttenPathB;
  rhDefaultCfg->lowpowerAlarmThreshold = msgRhConfigReq->lowpowerAlarmThreshold;
  rhDefaultCfg->overpowerAlarmThreshold = msgRhConfigReq->overpowerMinorAlarmThreshold;
  rhDefaultCfg->vswrShutdownAlarmThreshold = msgRhConfigReq->vswrShutdownAlarmThreshold;
  rhDefaultCfg->txEarfcn = msgRhConfigReq->txEarfcn;
  rhDefaultCfg->rxEarfcn = msgRhConfigReq->rxEarfcn;
  /* Updating the values in Table Context */
  dehUpdateRhConfigTable();

  /* Send Configuration Success To BSM */
  rhConfigSucc.transId = msgRhConfigReq->transId;
  createAndSendMsg(MSG_DEH_BSM_RH_CONFIG_SUCC, (UINT8*)&rhConfigSucc, sizeof(MsgDehBsmRhConfigSucc));
  if(dehContext->dehState EQ DEH_STATE_WAIT_CFG)
  {
    /* Change DEH State to DEH_STATE_IDLE */
    dehSetState(dehContext, DEH_STATE_IDLE);
  }
  return dehContext->dehState;
}

/**
 * @details This function updates Radio head Configuration table.
 **/
void dehUpdateRhConfigTable()
{
  RhDefaultCfg    *rhDefaultCfg = getRhDefaultCfg();
  RadioHeadConfig *configTable = (RadioHeadConfig *)getFirstRowOfTable(RADIOHEADCONFIG);

  /* Updating the values in Table Context */
  configTable->radioHeadProfileId = rhDefaultCfg->radioHeadProfileId;
  configTable->txAttenPathA = rhDefaultCfg->txAttenPathA;
  configTable->txAttenPathB = rhDefaultCfg->txAttenPathB;
  configTable->txDelay = rhDefaultCfg->txDelay;
  configTable->rxDelay = rhDefaultCfg->rxDelay;
  configTable->vswrShutdownAlarmThreshold = rhDefaultCfg->vswrShutdownAlarmThreshold;
  configTable->overpowerAlarmThreshold = rhDefaultCfg->overpowerAlarmThreshold;
  configTable->lowpowerAlarmThreshold = rhDefaultCfg->lowpowerAlarmThreshold;
}


/***************** DEH Configuration End Here ***********************/

/********************************************************************/
/******************       eNb-Enable  ---> OP-START    **************/
/********************************************************************/


/**
 * @param   rhDefaultCfg    Radio Head Default parameters Received from BSM at Start up of DEH
 * @details This function creates Radio head Configuration
 **/
TBOOL dehRhDefaultConfig( )
{
  RhEnableRspInfo  *rhEnableRspInfo = getEnableRspInfo();

  /* Data Path Setting */
  if( FALSE EQ dehRhSetDataPath( NULL ))
  {
    return FALSE;
  }
  /* Update Global Context */
  rhEnableRspInfo->noOfMsgsWaitingForRsp = 1; /* First Message Waiting For Response */
  logInfo("dehRhSetDataPath  and No Of Messages Waiting for RSP = %d\n",rhEnableRspInfo->noOfMsgsWaitingForRsp);

  /* FA Control */
  /* Attenuation Setting for Path A and B */
  if( FALSE EQ dehRhFaControl( NULL ))
  {
    return FALSE;
  }
  rhEnableRspInfo->noOfMsgsWaitingForRsp += 1;
  logInfo("dehRhFaControl  and No Of Messages Waiting for RSP = %d\n",rhEnableRspInfo->noOfMsgsWaitingForRsp);

/* Alarm Threshold Setting 
  if( FALSE EQ dehRhAlarmThresholdSet( NULL ))
  {
    return FALSE;
  }
  rhEnableRspInfo->noOfMsgsWaitingForRsp += 1;
  logInfo("dehRhAlarmThresholdSet  and No Of Messages Waiting for RSP = %d\n",rhEnableRspInfo->noOfMsgsWaitingForRsp);
*//*Commented out as Digial IQ overpower in LK disabled */

  /* Delay Setting */
  if( FALSE EQ dehRhSetDelay( NULL ))
  {
    return FALSE;
  }
  rhEnableRspInfo->noOfMsgsWaitingForRsp += 1;
  logInfo("dehRhSetDelay  and No Of Messages Waiting for RSP = %d\n",rhEnableRspInfo->noOfMsgsWaitingForRsp);

  /* Tx On/Off setting for Path A and B */
  if( FALSE EQ dehRhSetTxOnOff(0,1))
  {
    return FALSE;
  }
  rhEnableRspInfo->noOfMsgsWaitingForRsp += 1;
  logInfo("dehRhSetTxOnOff(0,1) and No Of Messages Waiting for RSP = %d\n",rhEnableRspInfo->noOfMsgsWaitingForRsp);

  if( FALSE EQ dehRhSetTxOnOff(1,1))
  {
    return FALSE;
  }
  rhEnableRspInfo->noOfMsgsWaitingForRsp += 1;
  logInfo("dehRhSetTxOnOff(1,1)  and No Of Messages Waiting for RSP = %d\n",rhEnableRspInfo->noOfMsgsWaitingForRsp);
  return TRUE;
}

/**
 * @param   msg        Received message
 * @param   dehContext DEH Context
 * @details This function will process Op-Start message received in all states of DEH
 **/
DehState dehProcOpStartReqInInitNoti(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
  MsgDehBsmOpstartFail opstartFail;
  clearBytes(&opstartFail, sizeof(MsgDehBsmOpstartFail));

  UNUSED(msg);
  UNUSED(dehContext);
  BS_ERROR_DEVICE_NOT_CONNECTED("DEH","RH");
  opstartFail.errorNum = BS_ERR_DEVICE_NOT_CONNECTED;
  snprintf((char*)&opstartFail.failReason, sizeof(opstartFail.failReason),"%s"," RH is not connected ");
  createAndSendMsg(MSG_DEH_BSM_OPSTART_FAIL, (UINT8*)&opstartFail, sizeof(MsgDehBsmOpstartFail));
  dehContext->isUnlocked = FALSE;
  return dehContext->dehState;
}

/**
 * @param   msg        Received message
 * @param   dehContext DEH Context
 * @details This function will process OPSTART message received in all states of DEH.
 **/
DehState dehProcOpStartReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
  MsgDehBsmOpstartFail opstartFail;
  UINT32   userInfo = 0;

  UNUSED(msg);
  clearBytes(&opstartFail, sizeof(MsgDehBsmOpstartFail));

  /* Device status needs to be checked here */
  if(!isConnected(DEV_RADIOHEAD))
  {
    BS_ERROR_DEVICE_NOT_CONNECTED("DEH","RH");
    opstartFail.errorNum = BS_ERR_DEVICE_NOT_CONNECTED;
    snprintf((char*)&opstartFail.failReason, sizeof(opstartFail.failReason),"%s"," RH is not connected ");
    createAndSendMsg(MSG_DEH_BSM_OPSTART_FAIL, (UINT8*)&opstartFail, sizeof(MsgDehBsmOpstartFail));
    dehContext->isUnlocked = FALSE;
    return dehContext->dehState;
  }
  if(!isConnected(DEV_CLOCK))
  {
    logError("%s", "CDCE62005 clock is not locked");
  }
  if(FALSE EQ isConnected( DEV_CPRI ))
  {
    dehContext->noOfCpriReqSent = 0;
    dehRhOperatingModeInfo();
    dehContext->noOfCpriReqSent += 1;
    startDehTimer( TIMERNAME_WAITCPRILINKUP, dehContext, &userInfo );
  }
  else
  {
    doRhConfigAndSendOpstartRspToBsm();
    setProcedureOngoing( DEV_RADIOHEAD, PROC_OPSTART );
  }

  /* Change DEH State to DEH_STATE_WAIT_DEV_DISABLE_RSP */
  dehSetState(dehContext, DEH_STATE_WAIT_DEV_ENABLE_RSP);
  return dehContext->dehState;
}
/*****************  eNb-Enable  ---> OP-START End Here ***********************/

/********************************************************************/
/******************       eNb-Disable  ---> OP-STOP    **************/
/********************************************************************/
/**
 * @param   msg        Received message
 * @param   dehContext DEH Context
 * @details This function will process OPSTOP message received in all states of DEH
 **/
DehState dehProcOpStopReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{

  UNUSED(msg);
  UINT32 userInfo = 0;
  /* Change DEH State to DEH_STATE_WAIT_DEV_DISABLE_RSP */
  dehSetState(dehContext, DEH_STATE_WAIT_DEV_DISABLE_RSP);

  RhDisableRspInfo  *rhDisableRspInfo = getDisableRspInfo();
  /* Start Timer TIMERNAME_WAITRHDISABLERSP */
  startDehTimer( TIMERNAME_WAITRHDISABLERSP, dehContext, &userInfo );

  dehRhSetTxOnOff( 0, 0);
  rhDisableRspInfo->noOfMsgsWaitingForRsp = 1;
  dehRhSetTxOnOff( 1, 0);
  rhDisableRspInfo->noOfMsgsWaitingForRsp += 1;
  setProcedureOngoing( DEV_RADIOHEAD, PROC_OPSTOP );

  return dehContext->dehState;
}

/*****************  eNb-Disable  ---> OP-STOP End Here ***********************/

/**
 * @param   msg  Message received from BSM
 * @param   dehContext DEH Context
 * @details This function will process device init Request message in DEH all states
 */
DehState dehProcDeviceInitReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
  UNUSED(msg);
  if(isConnected(DEV_RADIOHEAD))
  {
    createAndSendComponentStatusToBsm();
    dehContext->isBsmDehDeviceInitReqReceived = FALSE;
  }
  else
  {
    UINT32 userInfo = 0;
    dehContext->isBsmDehDeviceInitReqReceived = TRUE;
    /* Start Timer TIMERNAME_WAITALLDEVICERSP */
    startDehTimer( TIMERNAME_WAITALLDEVICERSP, dehContext, &userInfo );
  }
  initLedColorAndUpdateStatusTable();
  return dehContext->dehState;
}

/**
 * @details This function will check whether Radiohead is active or not
 **/
TBOOL dehIsRadioheadActive()
{
  DehContext *dehContext = getDehContext();
  if(!isConnected(DEV_RADIOHEAD))
  {
    return FALSE;
  }
  else if((dehContext->isRhRspDeviceStatusReceived EQ FALSE) && (isProcedureOnGoing(DEV_RADIOHEAD,PROC_SW_ACTIVATE) EQ FALSE))
  {
    return FALSE;
  }
  logInfo("RH is Active", NULL);
  return TRUE;
}


/**
 *  @details This function will check whether CPRI is active or not.
 */
TBOOL dehIsCpriActive()
{
  DehContext *dehContext = getDehContext();

  if(dehContext->isUnlocked)
  {
    if(isConnected(DEV_CPRI))
    {
      return DEV_CONNECTED;
    }
    else
    {
      return DEV_UNCONNECTED;
    }
  }
  else
  {
    return DEV_UNKNOWN;
  }
}
/**
 * @details This function will return the error string of Radiohead inactive.
 **/
UINT8* dehRadioHeadErrorString()
{
  DehContext *dehContext = getDehContext();
  if((dehContext->dehState EQ DEH_STATE_WAIT_INIT_NOTI) || (dehContext->dehState EQ DEH_STATE_WAIT_DEV_DISABLE_RSP) )
  {
	logError("RH is not connected. Waiting for NOTI", NULL);
	return "NOTI not received";
  }
  else if( dehContext->isRhRspDeviceStatusReceived EQ FALSE)
  {
	logError("NOTI/ACK Completed. RH Disconnected", NULL);
	return "RH Disconnected";
  }
  return "Unknown Reason";
}

/**
 * @details This function will return Radiohead Hardware Information.
 **/
UINT8* dehRadioheadInfo()
{
  DehContext *dehContext = getDehContext();
  return dehContext->rhHwInfoTable.description;
}

/**
 * @param   component  It containsDevice information
 * @details This function Maps and returns the specific Alarm of device
 */
BsErrorValue dehAlarmMapping(
  IN Device deviceno
)
{
   switch(deviceno)
  {
    case  DEV_BASEBAND:	    return BS_ERR_DIAGNOSTIC_FAILED;
    case  DEV_RADIOHEAD:    return BS_ERR_RH_DISABLED;
    case  DEV_BACKHAULPORT: return BS_ERR_BACKHAUL_ETH_LINK_DOWN;
    case  DEV_DEBUGPORT:    return BS_ERR_DEBUG_ETH_LINK_DOWN;
    case  DEV_TEMPSENSOR:   return BS_ERR_TEMPERATURE_SENSOR_INACTIVE;
    case  DEV_GPS:          return BS_ERR_GPS_RECV_NOT_RESPONDING;
    case  DEV_CPRI:         return BS_ERR_CPRI_INTERFACE_DOWN;
    case  DEV_CLOCK:        return BS_ERR_CLOCK_FAIL;
    default:                return BS_ERR_ALARM_ENTRY_NOT_FOUND;
  }
}
/**
 * @param   component  It containsDevice information
 * @details This function  returns Alarm Information of device
 */
TBOOL dehIsAlarmPresent(
  IN Device deviceno
)
{
  DeviceAlarmsInfo *deviceAlarms   = getDeviceAlarmsInfo();

  switch(deviceno)
  {
    case  DEV_BASEBAND:     return deviceAlarms->baseBand;
    case  DEV_RADIOHEAD:    return deviceAlarms->radioHead;
    case  DEV_BACKHAULPORT: return deviceAlarms->ethBackhaul;
    case  DEV_DEBUGPORT:    return deviceAlarms->ethDebug;
    case  DEV_TEMPSENSOR:   return deviceAlarms->tempSensor;
    case  DEV_GPS:          return deviceAlarms->gps;
    case  DEV_CPRI:         return deviceAlarms->cpri;
    case  DEV_CLOCK:        return deviceAlarms->baseBandClock;
    default:                return TRUE;
  }
}

/**
 * @param   component  It containsDevice information
 * @param   alarmInfo Alarm information flag
 * @details This function  change the Alarm Information of device at the time of clearance and creating of an alarm
 */
void dehSetDeviceAlarmInfo(
  IN Device deviceno,
  IN TBOOL alarmInfo
)
{
  DeviceAlarmsInfo *deviceAlarms   = getDeviceAlarmsInfo();

  switch(deviceno)
  {
    case  DEV_BASEBAND:      deviceAlarms->baseBand = alarmInfo;      break;
    case  DEV_RADIOHEAD:     deviceAlarms->radioHead = alarmInfo;     break;
    case  DEV_BACKHAULPORT:  deviceAlarms->ethBackhaul = alarmInfo;   break;
    case  DEV_DEBUGPORT:     deviceAlarms->ethDebug = alarmInfo;      break;
    case  DEV_TEMPSENSOR:    deviceAlarms->tempSensor = alarmInfo;    break;
    case  DEV_GPS:           deviceAlarms->gps = alarmInfo;           break;
    case  DEV_CPRI:          deviceAlarms->cpri = alarmInfo;          break;
    case  DEV_CLOCK:         deviceAlarms->baseBandClock = alarmInfo; break;
    default:                                                          break;
  }
}


/**
 * @param   device         Device of which the status has been changed/down
 * @param   errNum         Error Number of the device
 * @param   errString      Error String of the device
 * @param   resourceString Resource String of the device(Instance)
 * @details This function will create and send message Device down indication to BSM
 */
void dehCreateAndSendComponentDownInd(
  IN Device deviceno,
  IN BsErrorValue errNum,
  IN UINT8 *errString,
  IN UINT8 *resourceString
  )
{
  MsgDehBsmComponentDownInd msgDehBsmDeviceAlarm;
  ComponentStatus   *rowOfTable = (ComponentStatus*)getRowOfTable(COMPONENTSTATUS , deviceno - 1);

  clearBytes(&msgDehBsmDeviceAlarm, sizeof(MsgDehBsmComponentDownInd));

  msgDehBsmDeviceAlarm.componentType = rowOfTable->componentType;
  msgDehBsmDeviceAlarm.componentInstance = rowOfTable->componentInstance;
  msgDehBsmDeviceAlarm.componentOperationalState = COMPONENTOPERATIONALSTATE_DOWN;
  msgDehBsmDeviceAlarm.bsErrorForDeviceDown.errorNum = errNum;
  snprintf(msgDehBsmDeviceAlarm.bsErrorForDeviceDown.failReason,sizeof(String256),"%s",errString);
  if (NULL NOTEQ resourceString)
  {
    snprintf(msgDehBsmDeviceAlarm.infoString,sizeof(msgDehBsmDeviceAlarm.infoString),"%s",resourceString);
  }
  createAndSendMsg(MSG_DEH_BSM_COMPONENT_DOWN_IND, (UINT8*)&msgDehBsmDeviceAlarm, sizeof(MsgDehBsmComponentDownInd) );

}

/**
 * @param   device         Device of which the status has been changed/down
 * @param   errNum         Error Number of the device
 * @param   resourceString Resource String of the device(Instance)
 * @details This function will create and send message Device up indication to BSM
 */
void dehCreateAndSendComponentUpInd(
  IN Device deviceno,
  IN BsErrorValue errNum,
  IN UINT8 *resourceString
  )
{
  MsgDehBsmComponentDownInd msgDehBsmDeviceAlarm;
  ComponentStatus   *rowOfTable = (ComponentStatus*)getRowOfTable(COMPONENTSTATUS , deviceno - 1);

  clearBytes(&msgDehBsmDeviceAlarm, sizeof(MsgDehBsmComponentDownInd));

  msgDehBsmDeviceAlarm.componentType = rowOfTable->componentType;
  msgDehBsmDeviceAlarm.componentInstance = rowOfTable->componentInstance;
  msgDehBsmDeviceAlarm.componentOperationalState = COMPONENTOPERATIONALSTATE_UP;
  msgDehBsmDeviceAlarm.bsErrorForDeviceDown.errorNum = errNum;
  if (NULL NOTEQ resourceString)
  {
    snprintf(msgDehBsmDeviceAlarm.infoString,sizeof(msgDehBsmDeviceAlarm.infoString),"%s",resourceString);
  }
  createAndSendMsg(MSG_DEH_BSM_COMPONENT_UP_IND, (UINT8*)&msgDehBsmDeviceAlarm, sizeof(MsgDehBsmComponentDownInd) );
}

/**
 * @param   rspMsg Alarm info response
 * @details This function will process Radio Head Alarms
 */
void dehProcSendRhAlarmMsgToBsm(
  IN LkAlarmTable *rspMsg
  )
{
  LkAlarmTable *alarmStatus = getRhAlarmsInfo();

/*Alarm Data Common*/
  /*Condition to check Alarm is present and alarm didn't sent to BSM then Alarm sent to BSM*/
  if((TRUE EQ rspMsg->clockFail) && (FALSE EQ alarmStatus->clockFail))
  {
    dehCreateAndSendComponentDownInd(DEV_RADIOHEAD, BS_ERR_RH_CLOCK_FAIL, "Cpri Interface Down",NULL);
    alarmStatus->clockFail = TRUE;
  }
  /*Condition to check Alarm is cleared and is alarms sent already then Clear indication sent to BSM*/
  else if((FALSE EQ rspMsg->clockFail) && (TRUE EQ alarmStatus->clockFail))
  {
    dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_RH_CLOCK_FAIL, NULL);
    alarmStatus->clockFail = FALSE;
  }
  if((TRUE EQ rspMsg->highTemperature) && (FALSE EQ alarmStatus->highTemperature))
  {
    dehCreateAndSendComponentDownInd(DEV_RADIOHEAD,BS_ERR_RH_HIGH_TEMPERATURE, "Rh down due to high temperature",NULL);
    alarmStatus->highTemperature = TRUE;
  }
  else if((FALSE EQ rspMsg->highTemperature) && (TRUE EQ alarmStatus->highTemperature))
  {
    dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_RH_HIGH_TEMPERATURE, NULL);
    alarmStatus->highTemperature = FALSE;
  }
  if((TRUE EQ rspMsg->lowTemperature) && (FALSE EQ alarmStatus->lowTemperature) )
  {
    dehCreateAndSendComponentDownInd(DEV_RADIOHEAD, BS_ERR_RH_LOW_TEMPERATURE, "Rh down due to low temperature",NULL);
    alarmStatus->lowTemperature = TRUE;
  }
  else if((FALSE EQ rspMsg->lowTemperature) && (TRUE EQ alarmStatus->lowTemperature))
  {
    dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_RH_LOW_TEMPERATURE,NULL);
    alarmStatus->lowTemperature = FALSE;
  }

/*Alarms for Data PATHA*/
  if((TRUE EQ rspMsg->overPowerSideA) && (FALSE EQ alarmStatus->overPowerSideA))
  {
    dehCreateAndSendComponentDownInd(DEV_RADIOHEAD, BS_ERR_RH_OVER_POWER, "Over Power on sideA","PATH_A");
    alarmStatus->overPowerSideA = TRUE;
  }
  else if((FALSE EQ rspMsg->overPowerSideA) && (TRUE EQ alarmStatus->overPowerSideA))
  {
    dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_RH_OVER_POWER,"PATH_A");
    alarmStatus->overPowerSideA = FALSE;
  }
  if((TRUE EQ rspMsg->vswrFailsideA) && (FALSE EQ alarmStatus->vswrFailsideA))
  {
    dehCreateAndSendComponentDownInd(DEV_RADIOHEAD, BS_ERR_VSWR_FAIL, "VSWR failed on sideA","PATH_A");
    alarmStatus->vswrFailsideA = TRUE;
  }
  else if((FALSE EQ rspMsg->vswrFailsideA) && (TRUE EQ alarmStatus->vswrFailsideA))
  {
    dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_VSWR_FAIL,"PATH_A");
    alarmStatus->vswrFailsideA = FALSE;
  }
  if((TRUE EQ rspMsg->lowGainsideA) && (FALSE EQ alarmStatus->lowGainsideA))
  {
    dehCreateAndSendComponentDownInd(DEV_RADIOHEAD, BS_ERR_LOW_GAIN, "Low Gain on sideA","PATH_A");
    alarmStatus->lowGainsideA = TRUE;
  }
  else if((FALSE EQ rspMsg->lowGainsideA) && (TRUE EQ alarmStatus->lowGainsideA))
  {
    dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_LOW_GAIN,"PATH_A");
    alarmStatus->lowGainsideA = FALSE;
  }
  if((TRUE EQ rspMsg->lnaFailSideA) && (FALSE EQ alarmStatus->lnaFailSideA))
  {
    //dehCreateAndSendComponentDownInd(DEV_RADIOHEAD, BS_ERR_LNA_FAIL, "LNA is failed on sideA","PATH_A");
    alarmStatus->lnaFailSideA = TRUE;
  }
  else if((FALSE EQ rspMsg->lnaFailSideA) && (TRUE EQ alarmStatus->lnaFailSideA))
  {
    //dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_LNA_FAIL,"PATH_A");
    alarmStatus->lnaFailSideA = FALSE;
  }
  if((TRUE EQ rspMsg->disabledSideA) && (FALSE EQ alarmStatus->disabledSideA))
  {
    dehCreateAndSendComponentDownInd(DEV_RADIOHEAD, BS_ERR_RH_DISABLED, "RH path is disabled on sideA","PATH_A");
    alarmStatus->disabledSideA = TRUE;
  }
  else if((FALSE EQ rspMsg->disabledSideA) && (TRUE EQ alarmStatus->disabledSideA))
  {
    dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_RH_DISABLED,"PATH_A");
    alarmStatus->disabledSideA = FALSE;
  }
  if((TRUE EQ rspMsg->shutdownSideA) && (FALSE EQ alarmStatus->shutdownSideA))
  {
    dehCreateAndSendComponentDownInd(DEV_RADIOHEAD, BS_ERR_RH_SHUTDOWN, "RH Path is shutdown on sideA","PATH_A");
    alarmStatus->shutdownSideA = TRUE;
  }
  else if((FALSE EQ rspMsg->shutdownSideA) && (TRUE EQ alarmStatus->shutdownSideA))
  {
    dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_RH_SHUTDOWN,"PATH_A");
    alarmStatus->shutdownSideA = FALSE;
  }
/*Alarm for Data Path B*/
  if((TRUE EQ rspMsg->overPowerSideB) && (FALSE EQ alarmStatus->overPowerSideB))
  {
    dehCreateAndSendComponentDownInd(DEV_RADIOHEAD, BS_ERR_RH_OVER_POWER, "Over Power on sideB","PATH_B");
    alarmStatus->overPowerSideB = TRUE;
  }
  else if((FALSE EQ rspMsg->overPowerSideB) && (TRUE EQ alarmStatus->overPowerSideB))
  {
    dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_RH_OVER_POWER,"PATH_B");
    alarmStatus->overPowerSideB = FALSE;
  }
  if((TRUE EQ rspMsg->vswrFailSideB) &&(FALSE EQ alarmStatus->vswrFailSideB))
  {
    dehCreateAndSendComponentDownInd(DEV_RADIOHEAD, BS_ERR_VSWR_FAIL, "VSWR failed on sideB","PATH_B");
    alarmStatus->vswrFailSideB = TRUE;
  }
  else if((FALSE EQ rspMsg->vswrFailSideB) && (TRUE EQ alarmStatus->vswrFailSideB))
  {
    dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_VSWR_FAIL,"PATH_B");
    alarmStatus->vswrFailSideB = FALSE;
  }
  if((TRUE EQ rspMsg->lowGainSideB) && (FALSE EQ alarmStatus->lowGainSideB))
  {
    dehCreateAndSendComponentDownInd(DEV_RADIOHEAD, BS_ERR_LOW_GAIN, "Low Gain on sideB","PATH_B");
    alarmStatus->lowGainSideB = TRUE;
  }
  else if((FALSE EQ rspMsg->lowGainSideB) && (TRUE EQ alarmStatus->lowGainSideB))
  {
    dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_LOW_GAIN,"PATH_B");
    alarmStatus->lowGainSideB = FALSE;
  }
  if((TRUE EQ rspMsg->lnaFailSideB) && (FALSE EQ alarmStatus->lnaFailSideB))
  {
    //dehCreateAndSendComponentDownInd(DEV_RADIOHEAD, BS_ERR_LNA_FAIL, "LNA  is failed on sideB","PATH_B");
    alarmStatus->lnaFailSideB = TRUE;
  }
  else if((FALSE EQ rspMsg->lnaFailSideB) && (TRUE EQ alarmStatus->lnaFailSideB))
  {
    //dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_LNA_FAIL,"PATH_B");
    alarmStatus->lnaFailSideB = FALSE;
  }
  if((TRUE EQ rspMsg->disabledSideB) && (FALSE EQ alarmStatus->disabledSideB))
  {
    dehCreateAndSendComponentDownInd(DEV_RADIOHEAD, BS_ERR_RH_DISABLED, "RH path is disabled on sideB","PATH_B");
    alarmStatus->disabledSideB = TRUE;
  }
  else if((FALSE EQ rspMsg->disabledSideB) && (TRUE EQ alarmStatus->disabledSideB))
  {
    dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_RH_DISABLED,"PATH_B");
    alarmStatus->disabledSideB = FALSE;
  }
  if((TRUE EQ rspMsg->shutdownSideB) && (FALSE EQ alarmStatus->shutdownSideB))
  {
    dehCreateAndSendComponentDownInd(DEV_RADIOHEAD, BS_ERR_RH_SHUTDOWN, "RH Path is shutdown on sideB","PATH_B");
    alarmStatus->shutdownSideB = TRUE;
  }
  else if((FALSE EQ rspMsg->shutdownSideB) && (TRUE EQ alarmStatus->shutdownSideB))
  {
      dehCreateAndSendComponentUpInd(DEV_RADIOHEAD, BS_ERR_RH_SHUTDOWN,"PATH_B");
      alarmStatus->shutdownSideB = FALSE;
  }

}

/**
 * @param msg       Set Object Request Encoded Message Received from
 *                  operator mainly used to store in context for sending Response
 * @param setObj    Set Object Request which was Sent by Operator
 * @details This function perform BIST
 **/
DehState dehProcBistReq(
  IO SetObjReq   *setReq,
  IO DehContext  *dehContext
  )
{
  UINT32  userInfo = 0;
  /* Get Baseband status row from component status table*/
  ComponentStatus   *rowOfTable = (ComponentStatus*)getFirstRowOfTable(COMPONENTSTATUS);

  if( TRUE EQ dehContext->bistCtx.isBistRunning )
  {
	BS_ERR_BIST_ALREADY_RUNNING("MODID_DEH");
	dehFillAndSendSetObjFail( setReq, BS_ERR_BIST_ALREADY_RUNNING, NULL );
	return dehContext->dehState;
  }

  /* Start BIST Guard-Timer */
  startDehTimer( TIMERNAME_WAITBISTCOMPLETE, dehContext, &userInfo );

  /* Spawn BIST Test Thread*/
  if ( 0 EQ pthread_create( &g_bistThread, NULL, bistThreadMain, NULL))
  {
   	logInfo("BIST Test Thread spawning successful.", NULL);
   	dehFillAndSendSetObjSucc(setReq);
  }
  else
  {
   	logError("Error in spawning BIST Test Thread.", NULL);
   	dehFillAndSendSetObjFail( setReq, BS_ERR_BIST_START_FAILED, NULL );
   	/* Stop BIST Guard-Timer */
   	stopDehTimer(TIMERNAME_WAITBISTCOMPLETE);
   	return dehContext->dehState;
  }
  /* Update Component Status with BIST Status */
  snprintf(rowOfTable->errorString, MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1, "%s", "BIST In Progress");
  return dehContext->dehState;
}

/**
 * @param msg       Set Object Request Encoded Message Received from
 *                  operator mainly used to store in context for sending Response
 * @param setObj    Set Object Request which was Sent by Operator
 * @details This function perform Long POST
 **/
DehState dehProcLongPostReq(
  IO SetObjReq   *setReq,
  IO DehContext  *dehContext
  )
{
  // SET Long Post using Platform API.
  if( TRUE EQ dehApplyLongPost() )
  {
    dehFillAndSendSetObjSucc(setReq);
  }
  else
  {
	BS_ERR_SET_LONG_POST_FAILED("MODID_DEH");
	dehFillAndSendSetObjFail( setReq, BS_ERR_SET_LONG_POST_FAILED, NULL );
  }
  return dehContext->dehState;
}

/**
 * @param   req  : MsgPrmDehRhSwActivateReq
 * @param   rhSideToActivate :rhSideToActivate
 * @details This function will process and set the Side or RH to Activate
 */
UINT8 getRhNonRunningSide(
)
{
  DehContext *dehContext = getDehContext();
  UINT16 rhActiveSide;
  rhActiveSide = dehContext->rhPackageStatus.runningSide;

  if(RH_SIDE_A EQ rhActiveSide)
  {
   return RH_SIDE_B;
  }
  else
  {
   return RH_SIDE_A;
  }
}


/**
 * @param   msg  Message received from OAM-Master
 * @param   dehContext DEH Context
 * @details This function will process RH-SW install request message in DEH all states
 */
DehState dehProcRhSwInstallReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
  MsgPrmDehRhSwInstallReq   *req = (MsgPrmDehRhSwInstallReq*)msg->data;
  MsgDehPrmRhSwInstallFail  installFail;

  clearBytes(&installFail, sizeof(MsgDehPrmRhSwInstallFail));

  if (!isConnected(DEV_RADIOHEAD))
  {
    installFail.errorDesc.errorNum   = BS_ERR_DEVICE_NOT_CONNECTED;
    snprintf(installFail.errorDesc.failReason,sizeof(String256),"%s NOT CONNECTED", getDevName(DEV_RADIOHEAD));
    createAndSendMsg(MSG_DEH_PRM_RH_SW_INSTALL_FAIL, (UINT8*)&installFail, sizeof(MsgDehPrmRhSwInstallFail) );
  }
  else if ( isAnyProcedureOnGoing(DEV_RADIOHEAD))
  {
    installFail.errorDesc.errorNum   = BS_ERR_MSG_RCVD_IN_UNEXPECTED_STATE;
    snprintf(installFail.errorDesc.failReason,sizeof(String256),"%s ONGOING", getOngoingProcedureName(DEV_RADIOHEAD));
    createAndSendMsg(MSG_DEH_PRM_RH_SW_INSTALL_FAIL, (UINT8*)&installFail, sizeof(MsgDehPrmRhSwInstallFail) );
  }
  else /* Success case */
  {
    setProcedureOngoing(DEV_RADIOHEAD,PROC_SW_INSTALL);
    dehSendRhSwInstallReq(req);
    /** This procedure is gaurded by PRM **/
    /* wait for MSG_LK_BS_SW_INSTALL_RSP from RH */
  }
  return dehContext->dehState;
}

/**
 * @param   msg  Message received from OAM-Master
 * @param   dehContext DEH Context
 * @details This function will process RH-SW activate request message in DEH all states
 */
DehState dehProcRhSwActivateReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
  MsgPrmDehRhSwActivateReq     *req = (MsgPrmDehRhSwActivateReq*)msg->data;

  UINT8 side;
  MsgDehPrmRhSwActivateFail  activateFail;

  clearBytes(&activateFail, sizeof(MsgDehPrmRhSwActivateFail));

  if (!isConnected(DEV_RADIOHEAD))
  {
    activateFail.errorDesc.errorNum   = BS_ERR_DEVICE_NOT_CONNECTED;
    snprintf(activateFail.errorDesc.failReason,sizeof(String256),"%s NOT CONNECTED",getDevName(DEV_RADIOHEAD));
    createAndSendMsg(MSG_DEH_PRM_RH_SW_ACTIVATE_FAIL, (UINT8*)&activateFail, sizeof(MsgDehPrmRhSwActivateFail) );
  }
  else if ( !isProcedureOnGoing(DEV_RADIOHEAD,PROC_NONE))
  {
    activateFail.errorDesc.errorNum   = BS_ERR_MSG_RCVD_IN_UNEXPECTED_STATE;
    snprintf(activateFail.errorDesc.failReason,sizeof(String256),"%s", getOngoingProcedureName(DEV_RADIOHEAD));
    createAndSendMsg(MSG_DEH_PRM_RH_SW_ACTIVATE_FAIL, (UINT8*)&activateFail, sizeof(MsgDehPrmRhSwActivateFail) );
  }
  /** ASSUMPTION: PRM will check that the version requested for activation is not already active/running */

  else
  {
    /*store in DEH context to verify activated successfully or not*/
    snprintf(dehContext->rhPackageStatus.versionToActivate,MAX_SIZE_SWACTIVATIONSTATUS_VERSION+1,req->version);
    /*Get RH side which is need to to be activated by using version*/
    side = getRhNonRunningSide();
    setProcedureOngoing(DEV_RADIOHEAD,PROC_SW_ACTIVATE);
    dehSendRhSwActivateReq(side);
    /** This procedure is gaurded by PRM **/
    /*  wait for MSG_LK_BS_SET_ACTIVE_SIDE_RSP */
  }
  return dehContext->dehState;
}

/**
 * @param   ip      Ip in string format
 * @param   ipValue Ip value in hex.
 * @details This function will convert the ip from string to hex.
 */
void getIpToHexValue(
  IN UINT8  *ip,
  IO UINT32 *ipValue
  )
{
  UINT32 oct1,oct2,oct3,oct4;

  sscanf(ip, "%d.%d.%d.%d", &oct1, &oct2, &oct3, &oct4 );
  *ipValue = (oct1*256*256*256)+(oct2*256*256)+(oct3*256)+oct4;
}


/**
 * @param   state   DEH state enum value
 * @details Returns the state name in string format
 **/
char *getDehProceduralStateName(
  IN DehState state
  )
{
  switch( state )
  {
    case DEH_STATE_WAIT_CFG:              return "DEH_STATE_WAIT_CFG";
    case DEH_STATE_WAIT_INIT_NOTI:        return "DEH_STATE_WAIT_INIT_NOTI";
    case DEH_STATE_IDLE:                  return "DEH_STATE_IDLE";
    case DEH_STATE_WAIT_DEV_ENABLE_RSP:   return "DEH_STATE_WAIT_DEV_ENABLE_RSP";
    case DEH_STATE_WAIT_DEV_DISABLE_RSP:  return "DEH_STATE_WAIT_DEV_DISABLE_RSP";
    case DEH_STATE_UNDEFINED:             return "DEH_STATE_UNDEFINED";
    default:
      logError("%s","No name defined for this DEH state");
      break;
  }
  return "No name defined for this DEH state";
}

/**
 * @param   deviceno   Device to know the connected state
 * @details Returns the state of the device TRUE-Connected FALSE-NotConnected
 **/
TBOOL isConnected(
  IN Device deviceno
  )
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();
  return deviceFunctionalStatus->deviceConnectedState[deviceno];
}

/**
 * @param   deviceno   Device to know the connected state
 * @details Sets the state of the device to TRUE-Connected
 **/
void setConnected(
  IN Device deviceno
)
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();
  deviceFunctionalStatus->deviceConnectedState[deviceno] = TRUE;
}

/**
 * @param   deviceno   Device to know the connected state
 * @details Sets the state of the device  FALSE-NotConnected
 **/
void setNotConnected(
  IN Device deviceno
)
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();
  deviceFunctionalStatus->deviceConnectedState[deviceno] = FALSE;
}

/**
 * @param   deviceno   Device to know the Configured state
 * @details Returns the state of the device TRUE-Configured FALSE-NotConfigured
 **/
TBOOL isConfigured(
  IN Device deviceno
  )
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();
  return deviceFunctionalStatus->deviceConfigureState[deviceno];
}

/**
 * @param   deviceno   Device to know the Configured state
 * @details Sets the state of the device to TRUE-Configured
 **/
void setConfigured(
  IN Device deviceno
)
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();
  deviceFunctionalStatus->deviceConfigureState[deviceno] = TRUE;
}

/**
 * @param   deviceno   Device to know the Configured state
 * @details Sets the state of the device  FALSE-NotConfigured
 **/
void setNotConfigured(
  IN Device deviceno
)
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();
  deviceFunctionalStatus->deviceConfigureState[deviceno] = FALSE;
}

/**
 * @param   deviceno   Device to know the Operational state
 * @details Returns the state of the device TRUE-Operational FALSE-NotOperational
 **/
TBOOL isOperational(
  IN Device deviceno
  )
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();
  return deviceFunctionalStatus->deviceOperationalState[deviceno];
}

/**
 * @param   deviceno   Device to know the Operational state
 * @details Sets the state of the device to TRUE-Operational
 **/
void setOperational(
  IN Device deviceno
)
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();
  deviceFunctionalStatus->deviceOperationalState[deviceno] = TRUE;
}

/**
 * @param   deviceno   Device to know the Operational state
 * @details Sets the state of the device  FALSE-NotOperational
 **/
void setNotOperational(
  IN Device deviceno
)
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();
  deviceFunctionalStatus->deviceOperationalState[deviceno] = FALSE;
}

/**
 * @param   deviceno   Device to know the Procedure state.
 * @param   procedure  procedure of device.
 * @details Returns the state TRUE-if Procedure is running FALSE-If procedure is not running.
 **/
TBOOL isProcedureOnGoing(
  IN Device deviceno,
  IN Procedures procedure
  )
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();

  if(procedure EQ deviceFunctionalStatus->deviceProcedureState[deviceno])
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
 * @param   deviceno   Device to know the Procedure state.
 * @param   onGoingProcedure  procedure of device to set currently running.
 * @details Sets the state of Procedure of a device to onGoingProcedure.
 **/
void setProcedureOngoing(
  IN Device deviceno,
  IN Procedures onGoingProcedure
)
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();
  switch(deviceno)
  {
    case DEV_RADIOHEAD:
      deviceFunctionalStatus->deviceProcedureState[deviceno] = onGoingProcedure;
      break;

    default: logError("%s","NO other procedure defined"); break;
  }

}

/**
 * @param   deviceno   Device to know the Procedure state.
 * @param   completedProcedure  completed Procedure of device  to set.
 * @details Sets the state of Procedure of a device to PROC_NONE and last completed procedure of device to completedProcedure.
 **/
void setProcedureCompleted(
  IN Device deviceno,
  IN Procedures completedProcedure
  )
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();
  switch(deviceno)
  {
    case DEV_RADIOHEAD:
      deviceFunctionalStatus->deviceLastCompletedProcedure[deviceno] = completedProcedure;
      deviceFunctionalStatus->deviceProcedureState[deviceno] = PROC_NONE;
      /* Disable the Alarm if any for Radiohead*/
      dehSetDeviceAlarmInfo( deviceno, FALSE );
      break;
    default:
      logError("%s","NO other procedure defined");
      break;
  }

}
/**
 * @param   deviceno   Device to know the Procedure state.
 * @details Returns TRUE-Procedure ongoing FALSE-Procedure not running
 **/
TBOOL isAnyProcedureOnGoing(
  IN Device deviceno
  )
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();

  if(PROC_NONE EQ deviceFunctionalStatus->deviceProcedureState[deviceno])
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

/**
 * @param   deviceno   Device to know the Procedure state.
 * @details returns the current procedure of a device in string format.
 **/
char *getOngoingProcedureName(
  IN Device deviceno
)
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();
  switch(deviceFunctionalStatus->deviceProcedureState[deviceno])
  {
    case PROC_DEV_INIT:    return "PROC_DEV_INIT";
    case PROC_RESET:       return "PROC_RESET";
    case PROC_OPSTART:     return "PROC_OPSTART";
    case PROC_OPSTOP:      return "PROC_OPSTOP";
    case PROC_SW_INSTALL:  return "PROC_RH_SW_INSTALL";
    case PROC_SW_ACTIVATE: return "PROC_RH_SW_ACTIVATE";
    default:               return "PROC_NONE";
  }
}

/**
 * @param   deviceno   Device
 * @details returns the device name in string format.
 **/
char *getDevName(
  IN Device deviceno
  )
{
  switch (deviceno)
  {
    case DEV_BASEBAND:     return "BASEBAND";
    case DEV_RADIOHEAD:    return "RADIOHEAD";
    case DEV_BACKHAULPORT: return "BACKHAULPORT";
    case DEV_DEBUGPORT:    return "DEBUGPORT";
    case DEV_TEMPSENSOR:   return "TEMPSENSOR";
    case DEV_CPRI:         return "CPRI";
    case DEV_GPS:          return "GPS";
    case DEV_CLOCK:        return "CLOCK";
    default:               return "UNKNOWN";
  }
}

/**
 * @param   path   RH path
 * @details returns the RH path in string format.
 **/
char *getRhPathName(
IN UINT16 path
)
{
  if(RH_SIDE_A EQ path)
  {
    return "SIDE_A";
  }
  else
  {
    return "SIDE_B";
  }
}

/**
 * @details returns who is initiated the RH RESET procedure.
 **/
DehCmdType isUserInitiatedResetProcedure()
{
  DehRhRspTimersContext  *timersContext = getRhRspTimersContext();

  return timersContext->rhInitTimerInfo.dehCmdType;
}

/**
 * @details returns user input RH version is activated or not.
 **/
TBOOL isVersionActivated()
{

  DehContext *dehContext = getDehContext();

  if(!strncmp(dehContext->rhPackageStatus.runningVersion,dehContext->rhPackageStatus.versionToActivate,
       MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
 * @details RH configuration and send the op-start response to BSM.
 **/
void doRhConfigAndSendOpstartRspToBsm()
{
  UINT32  userInfo = 0;
  TBOOL configResult;
  MsgDehBsmOpstartFail opstartFail;
  DehContext *dehContext = getDehContext();
  RhEnableRspInfo  *rhEnableRspInfo = getEnableRspInfo();

  clearBytes(&opstartFail,sizeof(MsgDehBsmOpstartFail));

  /* Start Timer TIMERNAME_WAITRHENABLERSP */
  startDehTimer( TIMERNAME_WAITRHENABLERSP, dehContext, &userInfo );
  dehContext->isUnlocked = TRUE;
/* Configure Radio Head from Data Received from BSM at DEH Init */
  configResult = dehRhDefaultConfig();
  if(configResult EQ FALSE)
  {
    BS_ERROR_RH_CONFIG_FAILED("DEH");
    opstartFail.errorNum = BS_ERR_RH_CONFIG_FAILED;
    snprintf((char*)&opstartFail.failReason, sizeof(opstartFail.failReason)," RH Configuration Failed ");
    createAndSendMsg(MSG_DEH_BSM_OPSTART_FAIL, (UINT8*)&opstartFail, sizeof(MsgDehBsmOpstartFail));
    dehContext->isUnlocked = FALSE;
    /* Stop Timer TIMERNAME_WAITRHENABLERSP */
    stopDehTimer( TIMERNAME_WAITRHENABLERSP);
    dehSetState(dehContext, DEH_STATE_IDLE);
    setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
  }

  if( TRUE EQ dehRhEnableOrDisablePath( RH_PATH_A , RH_DU_ENABLE ))
  {
    rhEnableRspInfo->noOfMsgsWaitingForRsp += 1;
    logInfo("Enable Path A  and No Of Messages Waiting for RSP = %d \n",rhEnableRspInfo->noOfMsgsWaitingForRsp);
  }
  else
  {
    BS_ERROR_RH_CONFIG_FAILED("DEH");
    opstartFail.errorNum = BS_ERR_RH_CONFIG_FAILED;
    snprintf((char*)&opstartFail.failReason, sizeof(opstartFail.failReason)," RH Configuration Failed ");
    createAndSendMsg(MSG_DEH_BSM_OPSTART_FAIL, (UINT8*)&opstartFail, sizeof(MsgDehBsmOpstartFail));
    dehContext->isUnlocked = FALSE;
    /* Stop Timer TIMERNAME_WAITRHENABLERSP */
    stopDehTimer( TIMERNAME_WAITRHENABLERSP);
    dehSetState(dehContext, DEH_STATE_IDLE);
    setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
  }

  if( TRUE EQ dehRhEnableOrDisablePath( RH_PATH_B , RH_DU_ENABLE ))
  {
    rhEnableRspInfo->noOfMsgsWaitingForRsp += 1;
    logInfo("Enable Path B  and No Of Messages Waiting for RSP = %d\n",rhEnableRspInfo->noOfMsgsWaitingForRsp);
  }
  else
  {
    BS_ERROR_RH_CONFIG_FAILED("DEH");
    opstartFail.errorNum = BS_ERR_RH_CONFIG_FAILED;
    snprintf((char*)&opstartFail.failReason, sizeof(opstartFail.failReason)," RH Configuration Failed ");
    createAndSendMsg(MSG_DEH_BSM_OPSTART_FAIL, (UINT8*)&opstartFail, sizeof(MsgDehBsmOpstartFail));
    dehContext->isUnlocked = FALSE;
    /* Stop Timer TIMERNAME_WAITRHENABLERSP */
    stopDehTimer( TIMERNAME_WAITRHENABLERSP);
    dehSetState(dehContext, DEH_STATE_IDLE);
    setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
  }
}

/**
 * @param   deviceno   Device to know the Monitored state
 * @details Returns the state of the device TRUE-Monitored FALSE-NotMonitored
 **/
TBOOL isDeviceMonitored(
  IN Device deviceno
  )
{
  DeviceFunctionalStatus *deviceFunctionalStatus = getDeviceFunctionalStatus();
  return deviceFunctionalStatus->deviceMonitoredState[deviceno];
}



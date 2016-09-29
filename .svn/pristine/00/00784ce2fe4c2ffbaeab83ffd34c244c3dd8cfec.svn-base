/**
 * @file    deh_get_set_func.c
 * @brief   This file contains code to get details from GPS module.
 * @details Copyright Powerwave Technologies, Inc., 2011
 *
 * @author   Shweta Polepally, kgummad
 * @date     25-May-2011
 * @details  Contains GET and SET functions for RH/LED/GPS
 */

#include "deh.h"

/*****************************************/
/********** GET_OBJ functions*************/
/*****************************************/

/**
 * @param getReq       GET Object Request which was Sent by BSM
 * @param error        Error value carrying the reason of failure.
 * @details This function will fill and send MSG_DEH_BSM_GET_ENB_ADDRESS_FAIL Failure Message to BSM
 */
static void dehCreateAndSendGetEnbAddressFail(
  IO MsgBsmDehGetEnbAddrReq     *getReq,
  IO BsErrorValue               error
  )
{
  MsgDehBsmGetEnbAddrFail   getReqFail;
  clearBytes(&getReqFail, sizeof(MsgDehBsmGetEnbAddrFail));
  getReqFail.cmdRef = getReq->cmdRef;
  getReqFail.tagInfo = getReq->tagInfo;
  getReqFail.bsError = error;
  createAndSendMsg( MSG_DEH_BSM_GET_ENB_ADDR_FAIL, (UINT8*)&getReqFail, sizeof(MsgDehBsmGetEnbAddrFail) );
}

/**
 * @param getReq       GET Object Request which was Sent by BSM
 * @param bsErrorValue Error value carrying the reason of failure.
 * @details This function will fill and send GET OBJ Failure Message to BSM
 */
void dehFillAndSendGetObjFail(
  IN GetObjReq          *getReq,
  IN BsErrorValue        bsErrorValue
  )
{
  GetObjRspFail rspFail;
  EncBuf        encBuf;

  clearBytes(&rspFail, sizeof(GetObjRspFail));
  initEncBuf( &encBuf );

  copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspFail.cmdRef));
  copyBytes( &(getReq->tagArray), sizeof(TagArray), &(rspFail.tagArray));
  rspFail.getErrRsp.errRspCode = bsErrorValue;
  copyBytes( &getReq->tagArray.tagInfo[0], sizeof(TagInfo), &(rspFail.getErrRsp.failedTagInfo));
  encGetObjRspFailMsg( &rspFail, &encBuf );
  createAndSendMsg( MSG_DEH_BSM_GET_OBJ_FAIL, (UINT8*)&encBuf.value, encBuf.len );
  return;
}

/**
 * @param getReq       GET Object Request which was Sent by BSM
 * @param dehContext   DEH Context
 * @details This function will do a GET on Radio Head Status for both Configuration and Status request.
 */
void getAndSendRadioHeadStatusTable(
  IN  GetObjReq   *getReq,
  IO  DehContext  *dehContext
  )
{
  TBOOL       isSucc = FALSE;
  UINT32      userInfo  = 0;
  DevStatus *presentStatus  = getDevicePresentStatusPtr();

  /* Get the Commands Info Stored for Status Timer */
  RhStatusRspInfo   *rhStatusRspInfo = getStatusRspInfo(getReq->cmdRef.origin);

  /* Device status needs to be checked here -- Failure Case */
  if(presentStatus->devConnectionStatus[DEV_RADIOHEAD] NOTEQ DEV_CONNECTED)
  {
    BS_ERROR_RH_NOT_RUNNING( "MODID_DEH" );
    dehFillAndSendGetObjFail( getReq, BS_ERR_RH_NOT_RUNNING );
    return;
  }

  /* Checking whether Timer is Running i.e Previous Get RH Status command is Running */
  if(rhStatusRspInfo->noOfMsgsWaitingForRsp > 0)
  {
    BS_ERROR_DEH_ALREADY_PROCESSING_SAME_REQ( "MODID_DEH" );
    dehFillAndSendGetObjFail( getReq, BS_ERR_DEH_ALREADY_PROCESSING_SAME_REQ );
    return;
  }

  /* Sending RadioHead Status Info REQ */
  isSucc = dehRhStatusInfo(getReq->cmdRef.origin);

  if(isSucc EQ FALSE)
  {
    clearBytes(&rhStatusRspInfo->getReq , sizeof(GetObjReq));
    /* Send Get OBJ Fail RSP To BSM */
    dehFillAndSendGetObjFail(getReq, BS_ERR_DATA_NOT_SENT_ON_RH_SOCKET);
    return;
  }
  /* Start Timer */
  if(getReq->cmdRef.origin EQ OAMIF_CIH)
  {
    startDehTimer( TIMERNAME_WAITRHSTATUSRSPFORCIH, dehContext, &userInfo );
  }
  else if(getReq->cmdRef.origin EQ OAMIF_SIH)
  {
    startDehTimer( TIMERNAME_WAITRHSTATUSRSPFORSIH, dehContext, &userInfo );
  }
  else
  {
    startDehTimer( TIMERNAME_WAITRHSTATUSRSPFORWIH, dehContext, &userInfo );
  }
  rhStatusRspInfo->noOfMsgsWaitingForRsp = 1;
  copyBytes(getReq, sizeof(GetObjReq), &rhStatusRspInfo->getReq );
}

/**
 * @param getReq       GET Object Request which was Sent by BSM
 * @details This function will do GET operation for GPS Status.
 */
void getAndSendGpsStatusTable(
  IO GetObjReq   *getReq
  )
{
  TBOOL           isSucc = FALSE;
  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  TagInfo        *tagInfo = &getReq->tagArray.tagInfo[0];
  DevStatus      *presentStatus  = getDevicePresentStatusPtr();
  MsgDehBsmGpsStatusSucc  gpsStatusSucc;

  initEncBuf(&encBuf);
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  /* Fill CMD REF details */
  copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

  clearBytes(&gpsStatusSucc, sizeof(MsgDehBsmGpsDetailSucc));

  if(presentStatus->devConnectionStatus[DEV_GPS] NOTEQ DEV_CONNECTED)
  {
    BS_ERROR_GPS_NOT_CONNECTED( "MODID_DEH" );
    dehFillAndSendGetObjFail( getReq, BS_ERR_GPS_NOT_CONNECTED );
    return;
  }

  isSucc = dehProcGpsStatusReq(&gpsStatusSucc);

  if(isSucc EQ FALSE)
  {
    /* Send Get OBJ Fail RSP To BSM */
    dehFillAndSendGetObjFail(getReq, BS_ERR_GPS_RSP_NOT_RECEIVED);
    return;
  }
  GpsStatus *statusTable = (GpsStatus *)getFirstRowOfTable(GPSSTATUS);
  decGpsDehStatusInfoRsp( &gpsStatusSucc, statusTable );
  if(getReq->tagArray.tagInfo[0].colId > 0)
  {
    getElementGpsStatusTable( tagInfo->tableId, getReq->tagArray.tagInfo[0].rowId - 1, getReq->tagArray.tagInfo[0].colId, &rspTlv.tlvArray );
  }
  else
  {
    getTableGpsStatusTable( tagInfo->tableId, &rspTlv.tlvArray );
  }
  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );
  /* Send */
  createAndSendMsg( MSG_DEH_BSM_GET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);
}

/**
 * @param getReq       GET Object Request which was Sent by BSM
 * @details This function will do GET operation for LED Status.
 */
void getAndSendLedStatusTable(
  IO  GetObjReq   *getReq
)
{
  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  TagInfo        *tagInfo = &getReq->tagArray.tagInfo[0];

  initEncBuf(&encBuf);
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  /* Fill CMD REF details */
  copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

  /* Get on Column */
  if(getReq->tagArray.tagInfo[0].colId > 0)
  {
	getElementLedStatusTable( tagInfo->tableId, getReq->tagArray.tagInfo[0].rowId - 1, getReq->tagArray.tagInfo[0].colId, &rspTlv.tlvArray );
  }
  /* Get on Row */
  else if(getReq->tagArray.tagInfo[0].rowId > 0)
  {
	getRowLedStatusTable( tagInfo->tableId, getReq->tagArray.tagInfo[0].rowId, &rspTlv.tlvArray );
  }
  /* Get on Table */
  else
  {
	getTableLedStatusTable( tagInfo->tableId, &rspTlv.tlvArray );
  }

  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );
  /* Send */
  createAndSendMsg( MSG_DEH_BSM_GET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);
}

/**
 * @param getReq       GET Object Request which was Sent by BSM
 * @details This function will do GET operation for Component Status.
 */
void getAndSendComponentStatusTable(
  IO  GetObjReq   *getReq
  )
{
  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  TagInfo        *tagInfo = &getReq->tagArray.tagInfo[0];

  initEncBuf(&encBuf);
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  /* Fill CMD REF details */
  copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

  /* Get on Column */
  if(getReq->tagArray.tagInfo[0].colId > 0)
  {
  	getElementComponentStatusTable( tagInfo->tableId, getReq->tagArray.tagInfo[0].rowId - 1 , getReq->tagArray.tagInfo[0].colId, &rspTlv.tlvArray );
  }
  /* Get on Row */
  else if(getReq->tagArray.tagInfo[0].rowId > 0)
  {
  	getRowComponentStatusTable( tagInfo->tableId, getReq->tagArray.tagInfo[0].rowId, &rspTlv.tlvArray );
  }
  /* Get on Table */
  else
  {
  	getTableComponentStatusTable( tagInfo->tableId, &rspTlv.tlvArray );
  }

  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );
  /* Send */
  createAndSendMsg( MSG_DEH_BSM_GET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);
}

/**
 * @param   msg    Get Obj message coming from BSM
 * @param   dehContext   Deh Context
 * @details This function process all get request messages received from BSM.
 **/
DehState dehProcGetObjReq(
 IN Msg              *msg,
 IO DehContext       *dehContext
 )
{
  GetObjReq   getReq;
  TableId     tableId;
  /* Decode Received message  */
  clearBytes( &getReq, sizeof(GetObjReq));
  decGetObjReqMsg( (UINT8*)msg->data, msg->len, &getReq );

  tableId = getReq.tagArray.tagInfo[0].tableId;

  switch(tableId)
  {
    case COMPONENTSTATUS:
    {
      getAndSendComponentStatusTable(&getReq);
      return dehContext->dehState;
    }
    case LEDSTATUS :
    {
      getAndSendLedStatusTable(&getReq);
      return dehContext->dehState;
    }
    case GPSSTATUS :
    {
      getAndSendGpsStatusTable(&getReq);
      return dehContext->dehState;
    }
    case RADIOHEADSTATUS :
    {
      getAndSendRadioHeadStatusTable(&getReq, dehContext);
      return dehContext->dehState;
    }
    default:
    {
      BS_ERROR_UNKNOWN_TABLE_ID(getReq.tagArray.tagInfo[0].tableId);
      dehFillAndSendGetObjFail(&getReq, BS_ERR_UNKNOWN_TABLE_ID);
      break;
    }
  }
  return dehContext->dehState;
}

/**
 * @param   msg          Get Obj message coming from BSM
 * @param   dehContext   Deh Context
 * @details This function process get request messages received from BSM on EnbAddr
 **/
DehState dehProcGetEnbAddrReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
  MsgDehBsmGetEnbAddrRsp   getRsp;
  clearBytes( &getRsp, sizeof(MsgDehBsmGetEnbAddrRsp));

  MsgBsmDehGetEnbAddrReq  *getReq = (MsgBsmDehGetEnbAddrReq*)msg->data;

  getRsp.cmdRef  = getReq->cmdRef;
  getRsp.tagInfo = getReq->tagInfo;

  /* Get the Ethernet address of backhaulPortMacAddress */
  if (FALSE EQ dehGetMacAddress(getRsp.backhaulPortMacAddress, MACADDRESS_BACKHAUL_PORT))
  {
    dehCreateAndSendGetEnbAddressFail(getReq, BS_ERR_INTERNAL_ERROR_OCCURED);
    return dehContext->dehState;
  }
  if (FALSE EQ dehGetMacAddress(getRsp.debugPortMacAddress, MACADDRESS_DEBUG_PORT))
  {
    dehCreateAndSendGetEnbAddressFail(getReq, BS_ERR_INTERNAL_ERROR_OCCURED);
    return dehContext->dehState;
  }
  if (FALSE EQ dehGetBasebandProductId(getRsp.productId))
  {
    dehCreateAndSendGetEnbAddressFail(getReq, BS_ERR_INTERNAL_ERROR_OCCURED);
    return dehContext->dehState;
  }

  createAndSendMsg( MSG_DEH_BSM_GET_ENB_ADDR_RSP, (UINT8*)&getRsp, sizeof(MsgDehBsmGetEnbAddrRsp) );
  return dehContext->dehState;
}

/*****************************************/
/********** SET_OBJ functions*************/
/*****************************************/

/**
 * @param setObj       Set Object Request which was Sent by Operator
 * @details This function will fill and send Set Obj Success to BSM
 */

void dehFillAndSendSetObjSucc(
  IN SetObjReq         *setReq
  )
{
  SetObjRspSucc   rspSucc;
  EncBuf          encBuf;

  initEncBuf( &encBuf );
  clearBytes( &rspSucc, sizeof(SetObjRspSucc) );

  copyBytes( &setReq->cmdRef, sizeof(CmdRef), &(rspSucc.cmdRef));
  copyBytes( &setReq->tlvArray, sizeof(TlvArray), &(rspSucc.tlvArray));

  encSetObjRspSuccMsg( &rspSucc, &encBuf );

  createAndSendMsg( MSG_DEH_BSM_SET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len );
}


/**
 * @param setObj       Set Object Request which was Sent by Operator
 * @param bsErrorValue BS Error Value - Reason why the Request was rejected.
 * @param errTagInfo   Tag Information which failed.
 * @details This function will fill and send MSG_BSM_<OAM-IF>_SET_OBJ_FAIL to CIH/WIH/SIH module.
 */

void dehFillAndSendSetObjFail(
  IN SetObjReq         *setReq,
  IN BsErrorValue       bsErrorValue,
  IN TagInfo           *errTagInfo
  )
{
  SetObjRspFail   rspFail;
  EncBuf          encBuf;

  initEncBuf( &encBuf );
  clearBytes( &rspFail, sizeof(SetObjRspFail) );

  copyBytes( &setReq->cmdRef, sizeof(CmdRef), &(rspFail.cmdRef));
  copyBytes( &setReq->tlvArray, sizeof(TlvArray), &(rspFail.tlvArray));

  rspFail.setErrRsp.errRspCode = bsErrorValue;

  if ( errTagInfo NOTEQ NULL )
  {
    copyBytes( errTagInfo, sizeof(TagInfo), &(rspFail.setErrRsp.failedTagInfo));
  }

  encSetObjRspFailMsg( &rspFail, &encBuf );

  createAndSendMsg( MSG_DEH_BSM_SET_OBJ_FAIL, (UINT8*)&encBuf.value, encBuf.len );
}

/**
 * @param setObj Set Object Request which was Sent by Operator
 * @details This function perform GPS Reset
 */
static void resetGps(
  IO SetObjReq   *setReq
  )
{
  DevStatus   *devStatusRsp = getDevicePresentStatusPtr();

  /* Find the status of GPS Device */
  if( devStatusRsp->devConnectionStatus[DEV_GPS] EQ DEV_UNCONNECTED)
  {
    BS_ERROR_DEVICE_NOT_CONNECTED("MODID_DEH","GPS");
    dehFillAndSendSetObjFail(setReq, BS_ERR_GPS_NOT_CONNECTED, NULL );
    return ;
  }
  else if( devStatusRsp->devConnectionStatus[DEV_GPS] EQ DEV_UNKNOWN)
  {
    BS_ERROR_DEVICE_NOT_CONNECTED("MODID_DEH","GPS");
    dehFillAndSendSetObjFail(setReq, BS_ERR_GPS_NOT_CONNECTED, NULL );
    return ;
  }

  /* Reset GPS */
  dehGpsReset();
  dehFillAndSendSetObjSucc(setReq);
}

/**
 * @param msg       Set Object Request Encoded Message Received from
 *                  operator mainly used to store in context for sending Response
 * @param setObj    Set Object Request which was Sent by Operator
 * @details This function perform Radio head Reset
 **/
static void resetRh(
  IO SetObjReq   *setReq
  )
{
  DevStatus   *devStatusRsp = getDevicePresentStatusPtr();
  DehRhRspTimersContext  *timersContext = getRhRspTimersContext();

  /* Device status needs to be checked here -- Failure Case */
  if(devStatusRsp->devConnectionStatus[DEV_RADIOHEAD] NOTEQ DEV_CONNECTED)
  {
    BS_ERROR_DEVICE_NOT_CONNECTED("MODID_DEH","RH");
    dehFillAndSendSetObjFail(setReq, BS_ERR_RH_NOT_RUNNING, NULL );
    return ;
  }

  /* Reset Radio Head is Already processing By different Interface */
  if(timersContext->rhInitTimerInfo.isRhNotiReceived EQ FALSE)
  {
    BS_ERROR_DEH_ALREADY_PROCESSING_SAME_REQ("MODID_DEH");
    dehFillAndSendSetObjFail( setReq, BS_ERR_DEH_ALREADY_PROCESSING_SAME_REQ, NULL );
    return ;
  }

  /* Send Reset Request */
  if(FALSE EQ dehSendRhResetReq())
  {
    BS_ERROR_DATA_NOT_SENT_ON_RH_SOCKET("MODID_DEH");
    dehFillAndSendSetObjFail( setReq, BS_ERR_DATA_NOT_SENT_ON_RH_SOCKET, NULL );
    return ;
  }
  setProcedureOngoing(DEV_RADIOHEAD,PROC_RESET);
  /* Fill Global Context with the Reference Information */
  timersContext->rhInitTimerInfo.dehCmdType = RSP_USER_INITIATED;
  timersContext->rhInitTimerInfo.isRhNotiReceived = FALSE;
  copyBytes(setReq ,sizeof(SetObjReq), &timersContext->rhInitTimerInfo.setReq);
}

/**
 * @param setObj    Set Object Request which was Sent by Operator
 * @details This function perform Radio head Configuration of Single Element set by the operator
 **/
TBOOL configRhSingleElement(
  IO SetObjReq   *setObj,
  IO RhDefaultCfg *rhConfig
  )
{
  /* Get the Commands Info Stored for Configuration Timer */
  RhConfigRspInfo  *rhConfigRspInfo = getConfigRspInfo();

  switch( setObj->tlvArray.tlvInfo[0].tag.colId )
  {
    case COL_RADIOHEADCONFIG_RADIOHEADPROFILEID:
      logInfo("User trying to Set on Radio Head Id to 1, Received Column  id : %d",setObj->tlvArray.tlvInfo[0].tag.colId);
    case COL_RADIOHEADCONFIG_TXATTENPATHA:
    case COL_RADIOHEADCONFIG_TXATTENPATHB:
      /* FA Control --> TX Attenuation Setting for Path A and B */
      if(FALSE EQ dehRhFaControl( rhConfig ))
      {
        return FALSE;
      }
      rhConfigRspInfo->isFaControlRspReceived = FALSE;
      break;
    case COL_RADIOHEADCONFIG_TXDELAY:
    case COL_RADIOHEADCONFIG_RXDELAY:
      if(FALSE EQ dehRhSetDelay( rhConfig ))
      {
        return FALSE;
      }
      rhConfigRspInfo->isSetDelayRspReceived = FALSE;
      break;
    case COL_RADIOHEADCONFIG_VSWRSHUTDOWNALARMTHRESHOLD:
    case COL_RADIOHEADCONFIG_OVERPOWERALARMTHRESHOLD:
    case COL_RADIOHEADCONFIG_LOWPOWERALARMTHRESHOLD:
      /* Alarm Threshold Setting */
      if(FALSE EQ dehRhAlarmThresholdSet( rhConfig ))
      {
        return FALSE;
      }
      rhConfigRspInfo->isAlarmThresholdRspReceived = FALSE;
    break;
  }
  rhConfigRspInfo->noOfMsgsWaitingForRsp = 1; /* First Message Waiting For Response */
  return TRUE;
}

/**
 * @param setObj    Set Object Request which was Sent by Operator
 * @details This function perform Radio head Configuration of Single Element set by the operator
 **/
TBOOL configRhProfileId(
  IO SetObjReq   *setObj
  )
{
  switch( setObj->tlvArray.tlvInfo[0].tag.colId )
  {
    case COL_RADIOHEADCONFIG_RADIOHEADPROFILEID:
      logInfo("User trying to Set on Radio Head Id to 1, Received Column  id : %d",setObj->tlvArray.tlvInfo[0].tag.colId);
      return  TRUE;
  }
  return FALSE;
}

/**
 * @param setObj Set Object Request which was Sent by Operator
 * @details This function perform Radio head Configuration
 **/
TBOOL procDehRhUserConfigInUnlockState(
  IO SetObjReq   *setObj
  )
{
  UINT32 index     = 0;
  TBOOL  isSuccess = FALSE;
  TBOOL  isSetFaControl = FALSE;
  TBOOL  isSetDelay = FALSE;
  TBOOL  isSetAlarmThreshold = FALSE;

  RhDefaultCfg   rhUserConfig;
  RhDefaultCfg   *rhDefaultCfg = getRhDefaultCfg();

  clearBytes(&rhUserConfig , sizeof(RhDefaultCfg));
  copyBytes(rhDefaultCfg, sizeof(RhDefaultCfg), &rhUserConfig );

  /* If User Configured Entire Radio Head */
  for( index=0; index < setObj->tlvArray.numTlvs; index++ )
  {
    switch( setObj->tlvArray.tlvInfo[index].tag.colId )
    {
      case COL_RADIOHEADCONFIG_RADIOHEADPROFILEID:
        rhUserConfig.radioHeadProfileId  = getUint32FromArray(setObj->tlvArray.tlvInfo[index].data);
      break;
      case COL_RADIOHEADCONFIG_TXATTENPATHA:
        rhUserConfig.txAttenPathA  = getUint16FromArray(setObj->tlvArray.tlvInfo[index].data);
        isSetFaControl = TRUE;
      break;
      case COL_RADIOHEADCONFIG_TXATTENPATHB:
        rhUserConfig.txAttenPathB  = getUint16FromArray(setObj->tlvArray.tlvInfo[index].data);
        isSetFaControl = TRUE;
      break;
      case COL_RADIOHEADCONFIG_TXDELAY:
        rhUserConfig.txDelay  = getUint32FromArray(setObj->tlvArray.tlvInfo[index].data);
        isSetDelay = TRUE;
      break;
      case COL_RADIOHEADCONFIG_RXDELAY:
        rhUserConfig.rxDelay  = getUint32FromArray(setObj->tlvArray.tlvInfo[index].data);
        isSetDelay = TRUE;
      break;
      case COL_RADIOHEADCONFIG_VSWRSHUTDOWNALARMTHRESHOLD:
        rhUserConfig.vswrShutdownAlarmThreshold  = getUint8FromArray(setObj->tlvArray.tlvInfo[index].data);
        isSetAlarmThreshold = TRUE;
      break;
      case COL_RADIOHEADCONFIG_OVERPOWERALARMTHRESHOLD:
        rhUserConfig.overpowerAlarmThreshold  = getUint32FromArray(setObj->tlvArray.tlvInfo[index].data);
        isSetAlarmThreshold = TRUE;
      break;
      case COL_RADIOHEADCONFIG_LOWPOWERALARMTHRESHOLD:
        rhUserConfig.lowpowerAlarmThreshold  = getUint32FromArray(setObj->tlvArray.tlvInfo[index].data);
        isSetAlarmThreshold = TRUE;
      break;
    }
  }

  /* Get the Commands Info Stored for Configuration Timer */
  RhConfigRspInfo  *rhConfigRspInfo = getConfigRspInfo();

  if(setObj->tlvArray.numTlvs EQ 1)
  {
    isSuccess = configRhSingleElement( setObj, &rhUserConfig );
    return isSuccess;
  }
  /* FA Control */
  /* Attenuation Setting for Path A and B */
  if (isSetFaControl EQ TRUE)
  {
    isSuccess = dehRhFaControl( &rhUserConfig );
    if(isSuccess EQ FALSE)
    {
      isSetFaControl = FALSE;
      return isSuccess;
    }
    rhConfigRspInfo->noOfMsgsWaitingForRsp = 1; /* First Message Waiting For Response */
    rhConfigRspInfo->isFaControlRspReceived = FALSE;
    isSetFaControl = FALSE;
  }

  /* Delay Setting */
  if(isSetDelay EQ TRUE)
  {
    isSuccess = dehRhSetDelay( &rhUserConfig );
    if(isSuccess EQ FALSE)
    {
          isSetDelay = FALSE;
      return isSuccess;
    }
    if( rhConfigRspInfo->noOfMsgsWaitingForRsp EQ 0)
    {
      rhConfigRspInfo->noOfMsgsWaitingForRsp = 1;
    }
    else
    {
      rhConfigRspInfo->noOfMsgsWaitingForRsp += 1;
    }
    rhConfigRspInfo->isSetDelayRspReceived = FALSE;
    isSetDelay = FALSE;
  }

  /* Alarm Threshold Setting */
  if(isSetAlarmThreshold EQ TRUE)
  {
    isSuccess = dehRhAlarmThresholdSet( &rhUserConfig );
    if(isSuccess EQ FALSE)
    {
      isSetAlarmThreshold = FALSE;
      return isSuccess;
    }
    if( rhConfigRspInfo->noOfMsgsWaitingForRsp EQ 0)
    {
      rhConfigRspInfo->noOfMsgsWaitingForRsp = 1;
    }
    else
    {
      rhConfigRspInfo->noOfMsgsWaitingForRsp += 1;
    }
    rhConfigRspInfo->isAlarmThresholdRspReceived = FALSE;
    isSetAlarmThreshold = FALSE;
  }
  return isSuccess;
}

/**
 * @param setObj Set Object Request which was Sent by Operator
 * @details This function perform Radio head Configuration
 **/
void procDehRhUserConfigInLockState(
  IO SetObjReq   *setObj
  )
{
  UINT32 index     = 0;
  RhDefaultCfg   *rhDefaultCfg = getRhDefaultCfg();

  /* If User Configured Entire Radio Head */
  for( index=0; index < setObj->tlvArray.numTlvs; index++ )
  {
    switch( setObj->tlvArray.tlvInfo[index].tag.colId )
    {
      case COL_RADIOHEADCONFIG_RADIOHEADPROFILEID:
        rhDefaultCfg->radioHeadProfileId  = getUint32FromArray(setObj->tlvArray.tlvInfo[index].data);
      break;
      case COL_RADIOHEADCONFIG_TXATTENPATHA:
        rhDefaultCfg->txAttenPathA  = getUint16FromArray(setObj->tlvArray.tlvInfo[index].data);
      break;
      case COL_RADIOHEADCONFIG_TXATTENPATHB:
        rhDefaultCfg->txAttenPathB  = getUint16FromArray(setObj->tlvArray.tlvInfo[index].data);
      break;
      case COL_RADIOHEADCONFIG_TXDELAY:
        rhDefaultCfg->txDelay  = getUint32FromArray(setObj->tlvArray.tlvInfo[index].data);
      break;
      case COL_RADIOHEADCONFIG_RXDELAY:
        rhDefaultCfg->rxDelay  = getUint32FromArray(setObj->tlvArray.tlvInfo[index].data);
      break;
      case COL_RADIOHEADCONFIG_VSWRSHUTDOWNALARMTHRESHOLD:
        rhDefaultCfg->vswrShutdownAlarmThreshold  = getUint8FromArray(setObj->tlvArray.tlvInfo[index].data);
      break;
      case COL_RADIOHEADCONFIG_OVERPOWERALARMTHRESHOLD:
        rhDefaultCfg->overpowerAlarmThreshold  = getUint32FromArray(setObj->tlvArray.tlvInfo[index].data);
      break;
      case COL_RADIOHEADCONFIG_LOWPOWERALARMTHRESHOLD:
        rhDefaultCfg->lowpowerAlarmThreshold  = getUint32FromArray(setObj->tlvArray.tlvInfo[index].data);
      break;
    }
  }
  /* Send SET OBJ Success to BSM */
  dehFillAndSendSetObjSucc(setObj);
}

static void dehProcSetOnEnbAdmin(
  IO SetObjReq   *setReq,
  IO DehContext  *dehContext
  )
{
  EnbAdmin   enbAdmin;
  clearBytes(&enbAdmin, sizeof(EnbAdmin));

  GenericTable  *tableFunc = getTableFunctions(ENBADMIN);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setReq->tlvArray, &enbAdmin );
  }

  if ( enbAdmin.eNbCommand EQ ENBCOMMAND_RESETRADIOHEAD )
  {
    resetRh(setReq);
  }
  else if ( enbAdmin.eNbCommand EQ ENBCOMMAND_RESETGPS )
  {
    resetGps(setReq);
  }
  else if( enbAdmin.eNbCommand EQ ENBCOMMAND_RUNBUILTINSELFTEST )
  {
	dehProcBistReq( setReq, dehContext );
	return;
  }
  else if( enbAdmin.eNbCommand EQ ENBCOMMAND_SETLONGPOST )
  {
	dehProcLongPostReq( setReq, dehContext );
  	return;
  }
}

static void dehProcSetOnRadioHeadConfig(
  IO SetObjReq   *setReq,
  IO DehContext  *dehContext
  )
{
  UINT32      userInfo  = 0;

  /* Device status needs to be checked here -- Failure Case */
  DevStatus *presentStatus  = getDevicePresentStatusPtr();
  if(presentStatus->devConnectionStatus[DEV_RADIOHEAD] NOTEQ DEV_CONNECTED)
  {
    BS_ERROR_DEVICE_NOT_CONNECTED("MODID_DEH","RH");
    dehFillAndSendSetObjFail( setReq, BS_ERR_DEVICE_NOT_CONNECTED, NULL );
    return;
  }

  /* Get the Commands Info Stored for Configuration Timer */
  RhConfigRspInfo  *rhConfigRspInfo = getConfigRspInfo();
  /* Checking whether Timer is Running i.e Previous Set command is Running */
  if(rhConfigRspInfo->noOfMsgsWaitingForRsp > 0)
  {
    BS_ERROR_DEH_ALREADY_PROCESSING_SAME_REQ("MODID_DEH");
    dehFillAndSendSetObjFail( setReq, BS_ERR_DEH_ALREADY_PROCESSING_SAME_REQ, NULL );
    return;
  }

  /* If SET is only on Radio Head Profile Identifier */
  if( 1 EQ setReq->tlvArray.numTlvs )
  {
    if( TRUE EQ configRhProfileId(setReq) )
    {
      dehFillAndSendSetObjSucc(setReq);
      return;
    }
  }
  /* Start Timer */
  startDehTimer( TIMERNAME_WAITRHCONFIGRSP, dehContext, &userInfo );
  /* Store User Message in Context */
  copyBytes( setReq ,sizeof(SetObjReq), &rhConfigRspInfo->setReq);

  if( FALSE EQ procDehRhUserConfigInUnlockState( setReq ) )
  {
    /* Stop Timer */
    stopDehTimer( TIMERNAME_WAITRHCONFIGRSP );
    rhConfigRspInfo->noOfMsgsWaitingForRsp = 0;
    clearBytes(&rhConfigRspInfo->setReq , sizeof(SetObjReq));
    /* Send Set OBJ Fail RSP To BSM */
    BS_ERROR_DATA_NOT_SENT_ON_RH_SOCKET("MODID_DEH");
    dehFillAndSendSetObjFail( setReq, BS_ERR_DATA_NOT_SENT_ON_RH_SOCKET, NULL );
  }
}

/**
 * @param   setReq       Set Obj message on EnbAddresses
 * @param   dehContext   Deh Context
 * @details This functionProcess set Request messages received from BSM on EnbAddresses table
 */
void dehProcSetOnEnbAddress(
  IO SetObjReq   *setReq,
  IO DehContext  *dehContext
  )
{
  MACADDRESS     strBackhaulPortMacAddress[MAX_SIZE_ENBADDRESSES_BACKHAULPORTMACADDRESS + 1 ];
  MACADDRESS     strDebugPortMacAddress[MAX_SIZE_ENBADDRESSES_DEBUGPORTMACADDRESS + 1 ];

  UNUSED(dehContext);
  EnbAddresses   enbAddr;
  clearBytes( &enbAddr, sizeof(EnbAddresses));
  clearBytes( &strBackhaulPortMacAddress, MAX_SIZE_ENBADDRESSES_BACKHAULPORTMACADDRESS +1 );
  clearBytes( &strDebugPortMacAddress, MAX_SIZE_ENBADDRESSES_DEBUGPORTMACADDRESS + 1 );

  /* Convert TLV to structure */
  GenericTable  *tableFunc = getTableFunctions(ENBADDRESSES);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setReq->tlvArray, &enbAddr );
  }

  /* Check for multiple fields are requested to SET */
  if( (0 NOTEQ strlen(enbAddr.backhaulPortMacAddress)) && ( 0 NOTEQ strlen(enbAddr.debugPortMacAddress) ) )
  {
    dehFillAndSendSetObjFail(setReq, BS_ERR_MULTIPLE_ADDR_CHANGE_NOT_ALLOWED, NULL);
    BS_ERROR_MULTIPLE_ADDR_CHANGE_NOT_ALLOWED();
    return;
  }

  /* Read the backhaulPortMacAddress and debugPortMacAddress */
  if (FALSE EQ dehGetMacAddress(strBackhaulPortMacAddress, MACADDRESS_BACKHAUL_PORT))
  {
    dehFillAndSendSetObjFail(setReq, BS_ERR_INTERNAL_ERROR_OCCURED, NULL);
    return;
  }
  if (FALSE EQ dehGetMacAddress(strDebugPortMacAddress, MACADDRESS_DEBUG_PORT))
  {
    dehFillAndSendSetObjFail(setReq, BS_ERR_INTERNAL_ERROR_OCCURED, NULL);
    return;
  }

  /* Update backhaulPortMacAddress (if requested ) */
  if(0 NOTEQ strlen(enbAddr.backhaulPortMacAddress))
  {
    if( (0 EQ strncmp(strBackhaulPortMacAddress, enbAddr.backhaulPortMacAddress, strlen(enbAddr.backhaulPortMacAddress) )) ||
        (0 EQ strncmp(strDebugPortMacAddress, enbAddr.backhaulPortMacAddress, strlen(enbAddr.backhaulPortMacAddress) )))
    {
      dehFillAndSendSetObjFail(setReq, BS_ERR_ETHERNET_ADDR_ALREADY_IN_USE, NULL);
      BS_ERROR_ETHERNET_ADDR_ALREADY_IN_USE(enbAddr.backhaulPortMacAddress);
      return;
    }
    if (FALSE EQ dehSetMacAddress(enbAddr.backhaulPortMacAddress, MACADDRESS_BACKHAUL_PORT))
    {
      dehFillAndSendSetObjFail(setReq, BS_ERR_INTERNAL_ERROR_OCCURED, NULL);
      return;
    }
  }

  /* Update debugPortMacAddress (if requested ) */
  if(0 NOTEQ strlen(enbAddr.debugPortMacAddress))
  {
    if( (0 EQ strncmp(strBackhaulPortMacAddress, enbAddr.debugPortMacAddress, strlen(enbAddr.debugPortMacAddress) )) ||
        (0 EQ strncmp(strDebugPortMacAddress, enbAddr.debugPortMacAddress, strlen(enbAddr.debugPortMacAddress) )))
    {
      dehFillAndSendSetObjFail(setReq, BS_ERR_ETHERNET_ADDR_ALREADY_IN_USE, NULL);
      BS_ERROR_ETHERNET_ADDR_ALREADY_IN_USE(enbAddr.debugPortMacAddress);

      return;
    }
    if (FALSE EQ dehSetMacAddress(enbAddr.debugPortMacAddress, MACADDRESS_DEBUG_PORT))
    {
      dehFillAndSendSetObjFail(setReq, BS_ERR_INTERNAL_ERROR_OCCURED, NULL);
      return;
    }
  }
  /* Send the success */
  dehFillAndSendSetObjSucc(setReq);
}

/**
 * @param   msg    Set Obj message coming from BSM (RH COnfig and Reset may come in this message)
 * @param   dehContext   Deh Context
 * @details This functionProcess all set Request messages received from BSM
 */
DehState dehProcSetObjReq(
 IN Msg              *msg,
 IO DehContext       *dehContext
 )
{
  SetObjReq   setReq;
  clearBytes( &setReq, sizeof(SetObjReq));

  /* Decode Received message  */
  decSetObjReqMsg( (UINT8*)msg->data, msg->len, &setReq );

  TableId   tableId = setReq.tlvArray.tlvInfo[0].tag.tableId;

  switch ( tableId )
  {
    case RADIOHEADCONFIG:
    {
      /* Set on radioHeadConfig table is allowed only when DEH is in IDLE state */
      if( dehContext->dehState EQ DEH_STATE_IDLE && dehContext->isUnlocked EQ TRUE)
      {
    	/* Send to LK directly and upon success update local context of DEH */
    	dehProcSetOnRadioHeadConfig( &setReq, dehContext );
      }
      else
      if( dehContext->dehState EQ DEH_STATE_IDLE && dehContext->isUnlocked EQ FALSE )
      {
        /* Update only local context of DEH as SET only allowed in Unlocked state*/
        procDehRhUserConfigInLockState( &setReq );
      }
      else
      {
        BS_ERROR_MSG_RCVD_IN_UNEXPECTED_STATE( msg->msgId, getDehStateName(dehContext->dehState) );
        dehFillAndSendSetObjFail( &setReq, BS_ERR_MSG_RCVD_IN_UNEXPECTED_STATE, NULL );
      }
    }
    break;
    case ENBADMIN:
    {
      dehProcSetOnEnbAdmin( &setReq, dehContext );
    }
    break;
    case ENBADDRESSES:
    {
      dehProcSetOnEnbAddress( &setReq, dehContext );
    }
    break;
    default:
    {
      BS_ERROR_UNKNOWN_TABLE_ID( setReq.tlvArray.tlvInfo[0].tag.tableId );
      dehFillAndSendSetObjFail( &setReq, BS_ERR_UNKNOWN_TABLE_ID, NULL );
    }
    break;
  }
  return dehContext->dehState;
}

/**
 * @param   Msg          Set IP Address Request messaged received from BSM
 * @param   DehContext   dehContext
 * @details This function will process the IP Address request recevied from BSM
 */
DehState dehProcSetIpAddrReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  )
{
  MsgBsmDehSetIpAddrReq *setIpAddrReq = (MsgBsmDehSetIpAddrReq *)msg->data;
  MsgDehBsmSetIpAddrRsp setIpAddrRsp;

  clearBytes( &setIpAddrRsp, sizeof(MsgDehBsmSetIpAddrRsp));
  setAndUpdateEnodebIpAddr(dehContext,setIpAddrReq,&setIpAddrRsp);
  logInfo("%s","Sending the response for IP Address configuration");
  createAndSendMsg(MSG_DEH_BSM_SET_IP_ADDR_RSP,(UINT8*)&setIpAddrRsp,sizeof(MsgDehBsmSetIpAddrRsp));
  return dehContext->dehState;
}


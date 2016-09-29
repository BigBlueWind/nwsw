/**
 * @file    bsm_enb_init_cfg.c
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @brief   This file contains functions which will handle and fill functions related to EnodeB Initial Configurations.
 *
 * @author:  Shweta Polepally
 * @date:    April 2011
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param msg        Message Received from DEH
 * @param bsmContext BSM Module related Information.
 * @details This function will process MSG_DEH_BSM_SET_IP_ADDR_RSP message received from DEH.
 */
BsmProceduralState bsmProcSetIpAddrRsp(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  CreateAlarmDetails   alarmDetails;

  MsgDehBsmSetIpAddrRsp  *setRsp = (MsgDehBsmSetIpAddrRsp*)msg->data;
  EnbStatus           *enbStatus = (EnbStatus*)getFirstRowOfTable(ENBSTATUS);

  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));

  stopBsmProcedureTimer(TIMERNAME_WAITINITIPADDRCOMPLETE);
  if((FALSE EQ setRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].isSetSucc))
  {
    BS_ERROR_SET_IP_ADDR_FAILED("BACKHAUL");
    if ( TRUE EQ getAlarmDetailsByBsErrorCode( setRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].errDesc.errorNum, &alarmDetails ) )
    {
      strcat( alarmDetails.alarmLocation, getModuleName(msg->src) );
      if( NULL NOTEQ setRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].errDesc.failReason )
      {
        snprintf( alarmDetails.alarmAdditionalText, MAX_SIZE_ACTIVEALARMSTATUS_ALARMADDITIONALTEXT+1,
                  "%s", setRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].errDesc.failReason );
      }
      doSaveAlarm( &alarmDetails );
    }
  }
  else if((FALSE EQ setRsp->ipAddrSetRsp[INTERFACE_DEBUG].isSetSucc))
  {
    BS_ERROR_SET_IP_ADDR_FAILED("BACKHAUL");
    if ( TRUE EQ getAlarmDetailsByBsErrorCode( setRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].errDesc.errorNum, &alarmDetails ) )
    {
      strcat( alarmDetails.alarmLocation, getModuleName(msg->src) );
      if( NULL NOTEQ setRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].errDesc.failReason )
      {
        snprintf( alarmDetails.alarmAdditionalText, MAX_SIZE_ACTIVEALARMSTATUS_ALARMADDITIONALTEXT+1,
                  "%s", setRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].errDesc.failReason );
      }
      doSaveAlarm( &alarmDetails );
    }
  }
  else
  {
    snprintf( enbStatus->enodebIpAddress, MAX_SIZE_ENBSTATUS_ENODEBIPADDRESS + 1 , "%s", setRsp->ipAddrSetRsp[INTERFACE_BACKHAUL].ipAddress );
    doInitConfigProcedure( bsmContext );
  }
  return ( bsmContext->bsmProceduralState );
}


/**
 * @details This function will send message MSG_BSM_DEH_SET_IP_ADDR_REQ to DEH.
 */
void bsmCreateAndSendSetIpAddrReq()
{
  MsgBsmDehSetIpAddrReq   setReq;
  EnodebConfig     *enodebConfig = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
  EnbAddresses     *enbAddresses = (EnbAddresses*)getFirstRowOfTable(ENBADDRESSES);

  clearBytes(&setReq, sizeof(MsgBsmDehSetIpAddrReq));

  /* Filling request for BACKHAUL Port*/
  setReq.ipAddrSetReq[INTERFACE_BACKHAUL].interfaceType = INTERFACE_BACKHAUL;
  if(TRUE EQ enodebConfig->useStaticIpAddress )
  {
    setReq.ipAddrSetReq[INTERFACE_BACKHAUL].typeOfIpAddr = IPADDR_STATIC;
    snprintf(setReq.ipAddrSetReq[INTERFACE_BACKHAUL].ipAddress, sizeof(IpAddrStr), "%s", enodebConfig->enodebStaticIpAddr);
    snprintf(setReq.ipAddrSetReq[INTERFACE_BACKHAUL].netMask,   sizeof(IpAddrStr), "%s", enodebConfig->enodebNetmaskAddr);
  }
  else
  {
    setReq.ipAddrSetReq[INTERFACE_BACKHAUL].typeOfIpAddr = IPADDR_DHCP;
  }

  /* Filling request for DEBUG Port*/
  setReq.ipAddrSetReq[INTERFACE_DEBUG].interfaceType = INTERFACE_DEBUG;
  setReq.ipAddrSetReq[INTERFACE_DEBUG].typeOfIpAddr  = IPADDR_STATIC;
  snprintf(setReq.ipAddrSetReq[INTERFACE_DEBUG].ipAddress, sizeof(IpAddrStr), "%s", enbAddresses->staticIPAddressUsedAtDebugPort);
  snprintf(setReq.ipAddrSetReq[INTERFACE_DEBUG].netMask,   sizeof(IpAddrStr), "%s", enbAddresses->netmaskForDebugPort);

  createAndSendMsg( MSG_BSM_DEH_SET_IP_ADDR_REQ, (UINT8*)&setReq, sizeof(MsgBsmDehSetIpAddrReq) );
}

/**
 * @param bsmContext BSM Module related Information.
 * @details This function will check if initial configuration response is received from expected modules,
 *          else will log error ad return FALSE.
 */
TBOOL checkForInitConfigProcedureCompletion(
  IO BsmContext  *bsmContext
  )
{
  TBOOL retVal = TRUE;

  if ( FALSE EQ bsmContext->initCfgRspFlags.sihConfigRspRcvd )
  {
    BS_ERROR_NO_RSP_FOR_INIT_CONFIG_REQ(getModuleName(MODID_SIH) );
    createConfigurationFailedAlarm(MODID_SIH, "No Response for INIT CONFIG REQ");
    retVal = FALSE;
  }
  if ( FALSE EQ bsmContext->initCfgRspFlags.s1apConfigRspRcvd )
  {
    BS_ERROR_NO_RSP_FOR_INIT_CONFIG_REQ(getModuleName(MODID_S1AP) );
    createConfigurationFailedAlarm(MODID_S1AP, "No Response for INIT CONFIG REQ");
    retVal = FALSE;
  }
  if ( FALSE EQ bsmContext->initCfgRspFlags.sctpConfigRspRcvd )
  {
    BS_ERROR_NO_RSP_FOR_INIT_CONFIG_REQ( getModuleName(MODID_SCTP) );
    createConfigurationFailedAlarm(MODID_SCTP, "No Response for INIT CONFIG REQ");
    retVal = FALSE;
  }
  if ( FALSE EQ bsmContext->initCfgRspFlags.egtpConfigRspRcvd )
  {
    BS_ERROR_NO_RSP_FOR_INIT_CONFIG_REQ( getModuleName(MODID_EGTP) );
    createConfigurationFailedAlarm(MODID_EGTP, "No Response for INIT CONFIG REQ");
    retVal = FALSE;
  }
  if ( FALSE EQ bsmContext->initCfgRspFlags.dehConfigRspRcvd )
  {
    BS_ERROR_NO_RSP_FOR_INIT_CONFIG_REQ( getModuleName(MODID_DEH) );
    createConfigurationFailedAlarm(MODID_DEH, "No Response for INIT CONFIG REQ");
    retVal = FALSE;
  }

  if ( (FALSE EQ retVal) || (TRUE EQ bsmContext->isCfgRspWithWrongTransIdRcvd ) )
  {
    bsmContext->isInitConfigSuccessful = FALSE;
    bsmContext->isCfgRspWithWrongTransIdRcvd = FALSE;
  }
  else
  {
    bsmContext->isInitConfigSuccessful = TRUE;
  }
  return retVal;
}

/**
 * @param msgId  Received message ID
 * @param bsmContext BSM Module related Information.
 * @details This function will switch on the message received and mark the response received flag.
 */
ModuleId setInitConfigRspRcvdFromModule(
  IN MsgId       msgId,
  IO BsmContext *bsmContext
  )
{
  switch( msgId )
  {
    case MSG_SM_BSM_S1AP_INIT_CONFIG_SUCC:
    case MSG_SM_BSM_S1AP_INIT_CONFIG_FAIL:
      bsmContext->initCfgRspFlags.s1apConfigRspRcvd = TRUE;
      return MODID_S1AP;

    case MSG_SM_BSM_EGTP_INIT_CONFIG_SUCC:
    case MSG_SM_BSM_EGTP_INIT_CONFIG_FAIL:
      bsmContext->initCfgRspFlags.egtpConfigRspRcvd = TRUE;
      return MODID_EGTP;

    case MSG_SM_BSM_SCTP_INIT_CONFIG_SUCC:
    case MSG_SM_BSM_SCTP_INIT_CONFIG_FAIL:
      bsmContext->initCfgRspFlags.sctpConfigRspRcvd = TRUE;
      return MODID_SCTP;

    case MSG_DEH_BSM_INIT_CONFIG_SUCC:
    case MSG_DEH_BSM_INIT_CONFIG_FAIL:
      bsmContext->initCfgRspFlags.dehConfigRspRcvd = TRUE;
      return MODID_DEH;

    case MSG_SIH_BSM_INIT_CONFIG_SUCC:
    case MSG_SIH_BSM_INIT_CONFIG_FAIL:
      bsmContext->initCfgRspFlags.sihConfigRspRcvd = TRUE;
      return MODID_SIH;

    case MSG_LTH_BSM_INIT_CONFIG_SUCC:
    case MSG_LTH_BSM_INIT_CONFIG_FAIL:
      bsmContext->initCfgRspFlags.lthConfigRspRcvd = TRUE;
      return MODID_LTH;
    default:
      logInfo("%s", "Init Config Message not handled.. Coding Error... Check !! ");
      break;
  }
  logInfo("%s", "Init Config Message not handled.. Coding Error... Check !! ");
  return MODID_MAX;
}


/**
 * @details  This function fills Initial Configuration Request message and sends it to SIH Module.
 */
void fillAndSendSihInitCfgReqMsg()
{
  MsgBsmSihInitConfigReq configReq;
  clearBytes( &configReq, sizeof(MsgBsmSihInitConfigReq) );

  configReq.transId = getConfigTransId();
  EnbRegistrationConfig  *config = (EnbRegistrationConfig*)getFirstRowOfTable(ENBREGISTRATIONCONFIG);
  snprintf( configReq.emsAddr, (strlen(config->emsAddr)+1), "%s", config->emsAddr );
  snprintf( configReq.snmpCommunityString, (strlen(config->snmpCommunityString)+1), "%s", config->snmpCommunityString );

  createAndSendMsg( MSG_BSM_SIH_INIT_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmSihInitConfigReq) );
}

/**
 * @details  This function fills Initial Configuration Request message and sends it to SCTP Module.
 */
static void fillAndSendSctpInitCfgReqMsg()
{
  MsgBsmSctpInitConfigReq configReq;
  clearBytes( &configReq, sizeof(MsgBsmSctpInitConfigReq) );
  configReq.transId = getConfigTransId();
  getAndfillEnodebIpAddressFromEnodebStatus( configReq.enodebIpAddr );
  createAndSendMsg( MSG_BSM_SM_SCTP_INIT_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmSctpInitConfigReq) );
}

/**
 * @details  This function fills Initial Configuration Request message and sends it to S1AP Module.
 */
static void fillAndSendS1apInitCfgReqMsg()
{
  MsgBsmS1apInitConfigReq configReq;
  clearBytes( &configReq, sizeof(MsgBsmS1apInitConfigReq) );

  configReq.transId = getConfigTransId();
  getAndfillEnodebIpAddressFromEnodebStatus( configReq.enodebStaticIpAddr );

  EnodebConfig  *config    = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
  snprintf( configReq.enodebNetmaskAddr, (strlen(config->enodebNetmaskAddr)+1), "%s", config->enodebNetmaskAddr);

  createAndSendMsg( MSG_BSM_SM_S1AP_INIT_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmS1apInitConfigReq) );
}

/**
 * @details  This function fills Initial Configuration Request message and sends it to EGTP Module.
 */
static void fillAndSendEgtpInitCfgReqMsg()
{
  MsgBsmEgtpInitConfigReq configReq;
  clearBytes( &configReq, sizeof(MsgBsmEgtpInitConfigReq) );

  configReq.transId = getConfigTransId();
  getAndfillEnodebIpAddressFromEnodebStatus( configReq.enodebIpAddr );

  createAndSendMsg( MSG_BSM_SM_EGTP_INIT_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmEgtpInitConfigReq) );
}

static void fillAndSendDehInitCfgReqMsg()
{
  MsgBsmDehRhInitConfigReq   configReq;
  clearBytes( &configReq, sizeof(MsgBsmDehRhInitConfigReq) );

  fillDehRhCfgReqMsg( &configReq );
  createAndSendMsg( MSG_BSM_DEH_INIT_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmDehRhInitConfigReq) );
}

static void fillAndSendLthInitCfgReqMsg()
{
  MsgBsmLthInitConfigReq   bsmLthInitConfigReq;
  clearBytes( &bsmLthInitConfigReq, sizeof(MsgBsmLthInitConfigReq) );

  fillLthInitCfgReqMsg( &bsmLthInitConfigReq );
  createAndSendMsg( MSG_BSM_LTH_INIT_CONFIG_REQ, (UINT8*)&bsmLthInitConfigReq, sizeof(MsgBsmLthInitConfigReq) );
}
/**
 * @param    bsmContext BSM context information
 * @details  This function does/starts Initial Configuration Procedure.
 */
void doInitConfigProcedure(
  IO BsmContext  *bsmContext
  )
{
  clearBytes(&(bsmContext->cfgRspFlags), sizeof(CfgRspFlags));

  bsmContext->configTransId++;  /* Everytime Config Req is sent, New transaction ID will be generated by BSM*/
                                /* As of now its just an incremented value */
  bsmContext->isInitConfigSuccessful       = FALSE;
  bsmContext->isCfgRspWithWrongTransIdRcvd = FALSE;

  fillAndSendSihInitCfgReqMsg();
  fillAndSendSctpInitCfgReqMsg();
  fillAndSendS1apInitCfgReqMsg();
  fillAndSendEgtpInitCfgReqMsg();
  /* Use same function which is used for sending configReq, as the contents are same */
  fillAndSendDehInitCfgReqMsg();
  fillAndSendLthInitCfgReqMsg();

  startBsmProcTimer( TIMERNAME_WAITCONFIGCOMPLETE, &bsmContext->bsmTimersInfo );
}

/**
 * @param   flags   configuration response flags, if received is set to TRUE else FALSE.
 * @details This function will check if configuration response is received from expected modules.
 *          Returns FALSE it not, else returns TRUE.
 */
static TBOOL isInitConfigRspRcvdFromAllMod(
  IN InitCfgRspFlags  *flags
  )
{
  if ( ( flags->s1apConfigRspRcvd EQ FALSE ) || ( flags->sctpConfigRspRcvd EQ FALSE ) ||
       ( flags->egtpConfigRspRcvd EQ FALSE ) || ( flags->sihConfigRspRcvd EQ FALSE ) ||
       ( flags->dehConfigRspRcvd  EQ FALSE ) || ( flags->lthConfigRspRcvd EQ FALSE))
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

void doDeviceInitProcedure(
  IO BsmContext  *bsmContext
  )
{
  /* After initial configuration is done, BSM will enter in DEVICE_INIT_CMPL state */
  setBsmProceduralState( bsmContext, BS_WAIT_DEVICE_INIT_CMPL );
  /* Fill and send message to DEH to init all devices */
  createAndSendMsg( MSG_BSM_DEH_DEVICE_INIT_REQ, NULL, 0 );
  /* Start timer and wait for the response from DEH module */
  startBsmProcTimer( TIMERNAME_WAITDEVICEINITCOMPLETE, getBsmTimersInfo() );
}

/**
 * @param    bsmContext BSM context information
 * @details  This function does/starts Collect Information Procedure.
 */
void doCollectInfoProcedure(
  IO BsmContext  *bsmContext
  )
{
  /* After initial configuration is done, BSM will enter in COLLECT_INFO state and get
   * information from other modules. As of now -
   * 1. Get GPS details from DEH Module */
  setBsmProceduralState( bsmContext, BS_WAIT_COLLECT_INFO_CMPL );
  /* Fill and send message to DEH to get GPS details */
  sendGpsDetailReqMsg();
}

/**
 * @param msg        Message from configured module
 * @param bsmContext BSM Context
 * @details This function will process MSG_XXX_BSM_INIT_CONFIG_SUCC message in IDLE state.
 */
BsmProceduralState bsmProcInitConfigSuccFromSih(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  EnbConfigSucc   *rspSucc = (EnbConfigSucc*)msg->data;

  if ( rspSucc->transId NOTEQ bsmContext->configTransId )
  {
    BS_ERROR_INVALID_TRANSACTION_ID( rspSucc->transId, getModuleName(MODID_SIH), bsmContext->configTransId );
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param msg        Message from configured module
 * @param bsmContext BSM Context
 * @details This function will process MSG_XXX_BSM_INIT_CONFIG_FAIL message in IDLE state.
 */
BsmProceduralState bsmProcInitConfigFailFromSih(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  EnbConfigFail   *rspFail = (EnbConfigFail*)msg->data;

  BS_ERROR_RCVD_CONFIG_FAIL( getModuleName(MODID_SIH),rspFail->transId,
                             rspFail->errorDesc.errorNum, rspFail->errorDesc.failReason );

  if ( rspFail->transId NOTEQ bsmContext->configTransId )
  {
    BS_ERROR_INVALID_TRANSACTION_ID( rspFail->transId, getModuleName(MODID_SIH), bsmContext->configTransId );
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param msg        Message from configured module
 * @param bsmContext BSM Context
 * @details This function will process MSG_XXX_BSM_INIT_CONFIG_SUCC message.
 */
BsmProceduralState bsmProcInitConfigSucc(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  ModuleId         msgRvcdFromMod = MODID_MAX;
  EnbConfigSucc   *rspSucc = (EnbConfigSucc*)msg->data;

  msgRvcdFromMod = setInitConfigRspRcvdFromModule( msg->msgId, bsmContext );

  if ( rspSucc->transId EQ bsmContext->configTransId )
  {
    clearConfigurationFailedAlarm( msg->msgId, msgRvcdFromMod, bsmContext );
  }
  else
  {
    bsmContext->isCfgRspWithWrongTransIdRcvd = TRUE;
    BS_ERROR_INVALID_TRANSACTION_ID( rspSucc->transId, getModuleName(msgRvcdFromMod), bsmContext->configTransId );
  }

  if( TRUE EQ isInitConfigRspRcvdFromAllMod( &bsmContext->initCfgRspFlags ) )
  {
    stopBsmProcedureTimer( TIMERNAME_WAITCONFIGCOMPLETE );
    checkForInitConfigProcedureCompletion( bsmContext );
    if ( TRUE NOTEQ bsmContext->isInitConfigSuccessful )
    {
      BS_ERROR_CONFIG_PROCEDURE_FAILED();
    }
    doDeviceInitProcedure(bsmContext);
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param msg        Message from configured module
 * @param bsmContext BSM Context
 * @details This function will process MSG_XXX_BSM_INIT_CONFIG_FAIL message.
 */
BsmProceduralState bsmProcInitConfigFail(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  ModuleId        msgRvcdFromMod = MODID_MAX;
  EnbConfigFail  *rspFail = (EnbConfigFail*)msg->data;
  msgRvcdFromMod = setInitConfigRspRcvdFromModule( msg->msgId, bsmContext );

  logInfo(" Message received from moduleId : %s", getModuleName(msgRvcdFromMod) );

  /* Create configuration fail occurred alarm */
  createConfigurationFailedAlarm( msgRvcdFromMod ,"INIT CONFIG REQ Failed");
  BS_ERROR_RCVD_CONFIG_FAIL(getModuleName(msgRvcdFromMod),rspFail->transId,
                                        rspFail->errorDesc.errorNum, rspFail->errorDesc.failReason );

  if ( rspFail->transId NOTEQ bsmContext->configTransId )
  {
    bsmContext->isCfgRspWithWrongTransIdRcvd = TRUE;
    /* if transaction ID don't match, log that error as well */
    BS_ERROR_INVALID_TRANSACTION_ID( rspFail->transId, getModuleName(msgRvcdFromMod), bsmContext->configTransId );
  }

  if( TRUE EQ isInitConfigRspRcvdFromAllMod( &bsmContext->initCfgRspFlags ) )
  {
    BS_ERROR_CONFIG_PROCEDURE_FAILED();
    stopBsmProcedureTimer( TIMERNAME_WAITCONFIGCOMPLETE );
    /* Init config procedure failed. Hence stop this procedure and continue with next proceedure */
    doDeviceInitProcedure(bsmContext);
  }
  return bsmContext->bsmProceduralState;
}


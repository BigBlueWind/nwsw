 /**
 * @file    rh_msgHandlers.c
 * @brief This file will handle all the messages coming from Radio head
 *          in any DEH state.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author    mkata, spolepally, aagrawal, kgummad
 * @date      27-05-2011
 * @details   Change History: Basic version
 */

#include "deh.h"
#include "rh_dec.h"
#include <time.h>
#include "get_oam_func.h"


/**
 * @param    decBuf : Static buffer used for decode Message
 * @details  This function will carry out the Init procedure.
 *           1. Expect the Noti REQ msg from LK and decode the msg.
 *           2. Check the expected result.
 *           3. Fill and encode the ACK msg ( IPC Header and Application data ).
 *           2. Send the Message to LK
 */

void procRhDehInitNoti(
  IO DecBuf     *decBuf,
  IO UINT32      msgId
  )
{
  LkBsInitializationReqNoti noti;
  
  UNUSED(msgId);
  DehContext *dehContext = getDehContext();

  clearBytes(&noti, sizeof(LkBsInitializationReqNoti));

  /*Decode the Remote fusing complete notification*/
  decRhDehInitializationReqNoti( decBuf, &noti);


  dehSendRhNotiAck();
  DehRhRspTimersContext  *timersContext = getRhRspTimersContext();

 /* TODO: if(isTimerRunning(TIMERNAME_WAITRHINIT))
  {
    stopDehTimer(TIMERNAME_WAITRHINIT);
  }
*/

  DevStatus   *devStatusRsp = getDevicePresentStatusPtr();
  if(isConnected(DEV_RADIOHEAD))
  {
     logInfo("%s", "NOTI received from RH but RH is already connected, ignored");
  }
  else if(isProcedureOnGoing(DEV_RADIOHEAD, PROC_DEV_INIT))
  {
    stopDehTimer(TIMERNAME_WAITRHINIT);/*TODO need to remove after isTimerrunnning implemented*/
    timersContext->rhInitTimerInfo.isRhNotiReceived = TRUE;
    setConnected(DEV_RADIOHEAD);
    devStatusRsp->devConnectionStatus[DEV_RADIOHEAD] = DEV_CONNECTED;/*TODO Need to remove at optimisation*/
    dehContext->isRhRspDeviceStatusReceived = TRUE;

    setProcedureCompleted(DEV_RADIOHEAD,PROC_DEV_INIT);
    stopDehTimer( TIMERNAME_WAITALLDEVICERSP);
    createAndSendComponentStatusToBsm();
    dehContext->isBsmDehDeviceInitReqReceived = FALSE;
    dehRhHardwareInfo();
    dehRhFirmwareInfo();
  }
  if(isProcedureOnGoing(DEV_RADIOHEAD, PROC_SW_ACTIVATE))
  {
    setProcedureCompleted(DEV_RADIOHEAD,PROC_SW_ACTIVATE);
    if(TRUE EQ isVersionActivated())
    {
      MsgDehPrmRhSwActivateSucc  activateSucc;
      clearBytes(&activateSucc,  sizeof(MsgDehPrmRhSwActivateSucc));

      snprintf(activateSucc.version , MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1,"%s", dehContext->rhPackageStatus.runningVersion);
      createAndSendMsg(MSG_DEH_PRM_RH_SW_ACTIVATE_SUCC, (UINT8*)&activateSucc, sizeof(MsgDehPrmRhSwActivateSucc) );
    }
    else
    {
      MsgDehPrmRhSwActivateFail  activateFail;
      clearBytes(&activateFail,  sizeof(MsgDehPrmRhSwActivateFail));

      activateFail.errorDesc.errorNum = BS_ERR_INTERNAL_ERROR_OCCURED;
      snprintf(activateFail.version , MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1,"%s", dehContext->rhPackageStatus.runningVersion);
      createAndSendMsg(MSG_DEH_PRM_RH_SW_ACTIVATE_FAIL, (UINT8*)&activateFail, sizeof(MsgDehPrmRhSwActivateFail) );
    }
    dehRhFirmwareInfo(); /* sending this to get the date information */
  }
  else if(isProcedureOnGoing(DEV_RADIOHEAD, PROC_RESET) && isUserInitiatedResetProcedure()) /* send succ to BSM; invoke FW info */
  {
    setProcedureCompleted(DEV_RADIOHEAD,PROC_RESET);
    timersContext->rhInitTimerInfo.isRhNotiReceived = TRUE;
    dehFillAndSendSetObjSucc(&timersContext->rhInitTimerInfo.setReq);
    dehRhFirmwareInfo();
  }

  if(dehContext->dehState NOTEQ DEH_STATE_WAIT_CFG) /* In case of DEH_STATE_WAIT_CFG -- No need to change State*/
  {
    dehSetState(dehContext, DEH_STATE_IDLE);
  }
}

/**
 * @param   decBuf contains Response message received from RH(LK)
 * @details This function Decodes Data Path Response received from Radio Head
 */
void procRhDehDataPathRsp(
  IO DecBuf     *decBuf,
  IN UINT32      msgId
  )
{
  LkBsDataPathRsp       rspMsg;
  MsgDehBsmOpstartFail  opstartFail;

  clearBytes(&opstartFail , sizeof(MsgDehBsmOpstartFail));
  clearBytes(&rspMsg , sizeof(LkBsDataPathRsp));
  decRhDehDataPathRsp( decBuf, &rspMsg );
  DehContext    *dehContext = getDehContext();
  if(dehContext->dehState EQ DEH_STATE_WAIT_DEV_ENABLE_RSP)
  {
    RhEnableRspInfo  *rhEnableRspInfo = getEnableRspInfo();
    /* Check the result */
    if( rspMsg.varResult NOTEQ 0 )
    {
     /* OP-START Failure */
      opstartFail.errorNum = BS_ERR_RH_RSP_ERROR;
      snprintf((char*)&opstartFail.failReason, sizeof(opstartFail.failReason), (char *) decRhDehRspError(rspMsg.varResult, msgId));
      createAndSendMsg(MSG_DEH_BSM_OPSTART_FAIL, (UINT8*)&opstartFail, sizeof(MsgDehBsmOpstartFail));
      dehContext->isUnlocked = FALSE;
      /* Stop Timer */
      stopDehTimer(TIMERNAME_WAITRHENABLERSP);
      rhEnableRspInfo->noOfMsgsWaitingForRsp = 0; /* First Message Waiting For Response */
      dehSetState(dehContext, DEH_STATE_IDLE);
      setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
      return;
    }
    else
    {
      rhEnableRspInfo->noOfMsgsWaitingForRsp -=1;
      logInfo("Number of Messages Still Waiting for RSP = %d in state= %s",
    		      rhEnableRspInfo->noOfMsgsWaitingForRsp,"DEH_STATE_WAIT_DEV_ENABLE_RSP");
      if(rhEnableRspInfo->noOfMsgsWaitingForRsp EQ 0)
      {
        stopDehTimer(TIMERNAME_WAITRHENABLERSP);
        dehSetState(dehContext, DEH_STATE_IDLE);
        dehContext->isUnlocked = TRUE;
        createAndSendMsg(MSG_DEH_BSM_OPSTART_SUCC, NULL, 0);
        setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
        return;
      }
    }
  }
  else
  {
    logError("Delayed Response from Radio head for Dath path Set in DEH state = %d ",dehContext->dehState);
  }
}

/**
 * @param   decBuf contains Response message received from RH(LK)
 * @param   msgId contains the ID of message received from LK
 * @details This function Decodes FA Control Response
 */
void procRhDehFaControlRsp(
  IO DecBuf     *decBuf,
  IN UINT32 msgId
  )
{
  UINT32 index     = 0;
  LkBsFaControlRsp      rspMsg;
  MsgDehBsmOpstartFail  opstartFail;
  RhDefaultCfg    *rhDefaultCfg = getRhDefaultCfg();
  RadioHeadConfig *configTable = (RadioHeadConfig *)getFirstRowOfTable(RADIOHEADCONFIG);

  clearBytes(&rspMsg , sizeof(LkBsFaControlRsp));
  clearBytes(&opstartFail , sizeof(MsgDehBsmOpstartFail));
  decRhDehFaControlRsp( decBuf, &rspMsg );
  RhConfigRspInfo  *rhConfigRspInfo =  getConfigRspInfo();
  DehContext    *dehContext = getDehContext();
  if(dehContext->dehState EQ DEH_STATE_WAIT_DEV_ENABLE_RSP)
  {
    RhEnableRspInfo  *rhEnableRspInfo = getEnableRspInfo();
    /* Check the result */
    if( rspMsg.varResult NOTEQ 0 )
    {
     /* OP-START Failure */
      opstartFail.errorNum = BS_ERR_RH_RSP_ERROR;
      snprintf((char*)&opstartFail.failReason, sizeof(opstartFail.failReason), "%s", (char *) decRhDehRspError(rspMsg.varResult, msgId));
      createAndSendMsg(MSG_DEH_BSM_OPSTART_FAIL, (UINT8*)&opstartFail, sizeof(MsgDehBsmOpstartFail));
      dehContext->isUnlocked = FALSE;
      /* Stop Timer */
      stopDehTimer(TIMERNAME_WAITRHENABLERSP);
      rhEnableRspInfo->noOfMsgsWaitingForRsp = 0; /* First Message Waiting For Response */
      dehSetState(dehContext, DEH_STATE_IDLE);
      setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
      return;
    }
    else
    {
      rhEnableRspInfo->noOfMsgsWaitingForRsp -=1;
      logInfo("Number of Messages Still Waiting for RSP = %d in state= %s",
    		      rhEnableRspInfo->noOfMsgsWaitingForRsp,"DEH_STATE_WAIT_DEV_ENABLE_RSP");
      if(rhEnableRspInfo->noOfMsgsWaitingForRsp EQ 0)
      {
        stopDehTimer(TIMERNAME_WAITRHENABLERSP);
        dehSetState(dehContext, DEH_STATE_IDLE);
        dehContext->isUnlocked = TRUE;
        createAndSendMsg(MSG_DEH_BSM_OPSTART_SUCC, NULL, 0);
        setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
        return;
      }
    }
  }
  else if(rhConfigRspInfo->noOfMsgsWaitingForRsp > 0)
  {
    /* Check Whether FA Control Rsp is Received or not */
    if(rhConfigRspInfo->isFaControlRspReceived EQ FALSE)
    {
      rhConfigRspInfo->isFaControlRspReceived = TRUE;
      rhConfigRspInfo->noOfMsgsWaitingForRsp -=1;
      /**
       *  Data Path Config Failure Case
       *  @todo Fill TLV and Send to BSM
       */
      if(rspMsg.varResult NOTEQ 0)
      {
    	stopDehTimer(TIMERNAME_WAITRHCONFIGRSP);
        dehFillAndSendSetObjFail(&rhConfigRspInfo->setReq, BS_ERR_RH_CONFIG_FAILED, NULL);
    	rhConfigRspInfo->noOfMsgsWaitingForRsp = 0;
        rhConfigRspInfo->isFaControlRspReceived = TRUE;
    	clearBytes(&rhConfigRspInfo->setReq , sizeof(SetObjReq));
    	dehSetState(dehContext, DEH_STATE_IDLE);

      }
      else
      {
        /* If User Configured */
    	for( index=0; index < rhConfigRspInfo->setReq.tlvArray.numTlvs; index++ )
    	{
    	  switch( rhConfigRspInfo->setReq.tlvArray.tlvInfo[index].tag.colId )
    	  {
    	    case COL_RADIOHEADCONFIG_RADIOHEADPROFILEID:
    	      rhDefaultCfg->radioHeadProfileId  = getUint32FromArray(rhConfigRspInfo->setReq.tlvArray.tlvInfo[index].data);
    	      break;
    	    case COL_RADIOHEADCONFIG_TXATTENPATHA:
    	      rhDefaultCfg->txAttenPathA  = getUint16FromArray(rhConfigRspInfo->setReq.tlvArray.tlvInfo[index].data);
    	      break;
    	    case COL_RADIOHEADCONFIG_TXATTENPATHB:
    	      rhDefaultCfg->txAttenPathB  = getUint16FromArray(rhConfigRspInfo->setReq.tlvArray.tlvInfo[index].data);
    	      break;
    	   }
    	 }
    	  /**
           * @todo Fill TLV Data Path Success Case
           */
        if(rhConfigRspInfo->noOfMsgsWaitingForRsp EQ 0)
        {
          /* Stop TIMERNAME_WAITCONFIGCOMPLETE Timer */
          stopDehTimer(TIMERNAME_WAITRHCONFIGRSP);
          /* Send Set OBJ Success to BSM */
      	  dehFillAndSendSetObjSucc(&rhConfigRspInfo->setReq);
      	  /* Updating values in Table */
      	  configTable->txAttenPathA = rhDefaultCfg->txAttenPathA;
          configTable->txAttenPathB = rhDefaultCfg->txAttenPathB;
        }
      }
    }
    else
    {
      logError("Delayed Response from Radio head for Fa Control in DEH state = %d ",dehContext->dehState);
    }
  }
  else
  {
    logError("Delayed Response from Radio head for Fa Control in DEH state = %d ",dehContext->dehState);
  }
}



/**
 * @param   decBuf contains Response message received from RH(LK)
 * @details This function Decodes FA Control Response
 */
void procRhDehAlarmThresholdSetRsp(
  IO DecBuf     *decBuf,
  IN UINT32 msgId
  )
{
  UINT32 index     = 0;
  LkBsAlarmThresholdSettingRsp    rspMsg;
  MsgDehBsmOpstartFail opstartFail;
  RhDefaultCfg    *rhDefaultCfg = getRhDefaultCfg();
  RadioHeadConfig *configTable = (RadioHeadConfig *)getFirstRowOfTable(RADIOHEADCONFIG);

  clearBytes(&rspMsg , sizeof(LkBsAlarmThresholdSettingRsp));
  clearBytes(&opstartFail , sizeof(MsgDehBsmOpstartFail));
  decRhDehAlarmThresholdSettingRsp( decBuf, &rspMsg );
  RhConfigRspInfo  *rhConfigRspInfo =  getConfigRspInfo();
  DehContext    *dehContext = getDehContext();
  if(dehContext->dehState EQ DEH_STATE_WAIT_DEV_ENABLE_RSP)
  {
    RhEnableRspInfo  *rhEnableRspInfo = getEnableRspInfo();
    /* Check the result */
    if( rspMsg.varResult NOTEQ 0 )
    {
    /* OP-START Failure */
      opstartFail.errorNum = BS_ERR_RH_RSP_ERROR;
      snprintf((char*)&opstartFail.failReason, sizeof(opstartFail.failReason), "%s",(char *) decRhDehRspError(rspMsg.varResult, msgId));
      createAndSendMsg(MSG_DEH_BSM_OPSTART_FAIL, (UINT8*)&opstartFail, sizeof(MsgDehBsmOpstartFail));
      dehContext->isUnlocked = FALSE;
      /* Stop Timer */
      stopDehTimer(TIMERNAME_WAITRHENABLERSP);
      rhEnableRspInfo->noOfMsgsWaitingForRsp = 0; /* First Message Waiting For Response */
      dehSetState(dehContext, DEH_STATE_IDLE);
      setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
      return;
    }
    else
    {
      rhEnableRspInfo->noOfMsgsWaitingForRsp -=1;
      logInfo("Number of Messages Still Waiting for RSP = %d in state= %s",
    		      rhEnableRspInfo->noOfMsgsWaitingForRsp,"DEH_STATE_WAIT_DEV_ENABLE_RSP");
      if(rhEnableRspInfo->noOfMsgsWaitingForRsp EQ 0)
      {
        stopDehTimer(TIMERNAME_WAITRHENABLERSP);
        dehSetState(dehContext, DEH_STATE_IDLE);
        dehContext->isUnlocked = TRUE;
        createAndSendMsg(MSG_DEH_BSM_OPSTART_SUCC, NULL, 0);
        setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
        return;
      }
    }
  }
  else if(rhConfigRspInfo->noOfMsgsWaitingForRsp > 0)
  {
    /* Check Whether Alarm Threshold Control Rsp is Received or not */
    if(rhConfigRspInfo->isAlarmThresholdRspReceived EQ FALSE)
    {
      rhConfigRspInfo->isAlarmThresholdRspReceived = TRUE;
      rhConfigRspInfo->noOfMsgsWaitingForRsp -=1;

      /**
       * Data Path Config Failure Case
       * @todo Fill TLV and Send to BSM
       */
      if(rspMsg.varResult NOTEQ 0)
      {
        stopDehTimer(TIMERNAME_WAITRHCONFIGRSP);
        dehFillAndSendSetObjFail(&rhConfigRspInfo->setReq, BS_ERR_RH_CONFIG_FAILED, NULL);
     	rhConfigRspInfo->noOfMsgsWaitingForRsp = 0;
     	rhConfigRspInfo->isAlarmThresholdRspReceived = TRUE;
     	clearBytes(&rhConfigRspInfo->setReq , sizeof(SetObjReq));
     	dehSetState(dehContext, DEH_STATE_IDLE);
      }
      else
      {
    	/* If User Configured */
    	for( index=0; index < rhConfigRspInfo->setReq.tlvArray.numTlvs; index++ )
    	{
    	  switch( rhConfigRspInfo->setReq.tlvArray.tlvInfo[index].tag.colId )
    	  {
    	    case COL_RADIOHEADCONFIG_VSWRSHUTDOWNALARMTHRESHOLD:
    	      rhDefaultCfg->vswrShutdownAlarmThreshold  = getUint8FromArray(rhConfigRspInfo->setReq.tlvArray.tlvInfo[index].data);
    	      break;
    	    case COL_RADIOHEADCONFIG_OVERPOWERALARMTHRESHOLD:
    	      rhDefaultCfg->overpowerAlarmThreshold  = getUint32FromArray(rhConfigRspInfo->setReq.tlvArray.tlvInfo[index].data);
    	      break;
    	    case COL_RADIOHEADCONFIG_LOWPOWERALARMTHRESHOLD:
    	      rhDefaultCfg->lowpowerAlarmThreshold  = getUint32FromArray(rhConfigRspInfo->setReq.tlvArray.tlvInfo[index].data);
    	      break;
    	  }
    	}
    	/**
    	*  @todo Fill TLV Data Path Success Case
    	*/
        if(rhConfigRspInfo->noOfMsgsWaitingForRsp EQ 0)
        {
          /* Stop TIMERNAME_WAITCONFIGCOMPLETE Timer */
          stopDehTimer(TIMERNAME_WAITRHCONFIGRSP);
          /* Send Set OBJ Success to BSM */
      	  dehFillAndSendSetObjSucc(&rhConfigRspInfo->setReq);
      	  /* Updating values in Table */
      	  configTable->vswrShutdownAlarmThreshold = rhDefaultCfg->vswrShutdownAlarmThreshold;
          configTable->overpowerAlarmThreshold = rhDefaultCfg->overpowerAlarmThreshold;
          configTable->lowpowerAlarmThreshold = rhDefaultCfg->lowpowerAlarmThreshold;
        }
      }
    }
    else
    {
      logError("Delayed Response from Radio head for Alarm Threshold Setting in DEH state = %d ",dehContext->dehState);
    }
  }
  else
  {
    logError("Delayed Response from Radio head for Alarm Threshold Setting in DEH state = %d ",dehContext->dehState);
  }
}



/**
 * @param   decBuf contains Response message received from RH(LK)
 * @details This function Decodes Delay Setting Response
 */
void procRhDehDelaySettingRsp(
  IO DecBuf     *decBuf,
  IN UINT32 msgId
  )
{
  UINT32 index     = 0;
  LkBsDelaySettingRsp    rspMsg;
  MsgDehBsmOpstartFail   opstartFail;
  RhDefaultCfg    *rhDefaultCfg = getRhDefaultCfg();
  RadioHeadConfig *configTable = (RadioHeadConfig *)getFirstRowOfTable(RADIOHEADCONFIG);

  clearBytes(&opstartFail , sizeof(MsgDehBsmOpstartFail));
  clearBytes(&rspMsg , sizeof(LkBsDelaySettingRsp));
  decRhDehDelaySettingRsp( decBuf, &rspMsg );
  RhConfigRspInfo  *rhConfigRspInfo =  getConfigRspInfo();
  DehContext    *dehContext = getDehContext();
  if(dehContext->dehState EQ DEH_STATE_WAIT_DEV_ENABLE_RSP)
  {
    RhEnableRspInfo  *rhEnableRspInfo = getEnableRspInfo();
    /* Check the result */
    if( rspMsg.varResult NOTEQ 0 )
    {
    /* OP-START Failure */
      opstartFail.errorNum = BS_ERR_RH_RSP_ERROR;
      snprintf((char*)&opstartFail.failReason, sizeof(opstartFail.failReason), "%s", (char *) decRhDehRspError(rspMsg.varResult, msgId));
      createAndSendMsg(MSG_DEH_BSM_OPSTART_FAIL, (UINT8*)&opstartFail, sizeof(MsgDehBsmOpstartFail));
      dehContext->isUnlocked = FALSE;
      /* Stop Timer */
      stopDehTimer(TIMERNAME_WAITRHENABLERSP);
      rhEnableRspInfo->noOfMsgsWaitingForRsp = 0; /* First Message Waiting For Response */
      dehSetState(dehContext, DEH_STATE_IDLE);
      setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
      return;
    }
    else
    {
      rhEnableRspInfo->noOfMsgsWaitingForRsp -=1;
      logInfo("Number of Messages Still Waiting for RSP = %d in state= %s",
    		      rhEnableRspInfo->noOfMsgsWaitingForRsp,"DEH_STATE_WAIT_DEV_ENABLE_RSP");
      if(rhEnableRspInfo->noOfMsgsWaitingForRsp EQ 0)
      {
        stopDehTimer(TIMERNAME_WAITRHENABLERSP);
        dehSetState(dehContext, DEH_STATE_IDLE);
        dehContext->isUnlocked = TRUE;
        createAndSendMsg(MSG_DEH_BSM_OPSTART_SUCC, NULL, 0);
        setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
        return;
      }
    }
  }
  else if(rhConfigRspInfo->noOfMsgsWaitingForRsp > 0)
  {
    /* Check Whether Delay setting Rsp is Received or not */
    if(rhConfigRspInfo->isSetDelayRspReceived EQ FALSE)
    {
      rhConfigRspInfo->isSetDelayRspReceived = TRUE;
      rhConfigRspInfo->noOfMsgsWaitingForRsp -=1;

      /**
       * Data Path Config Failure Case
       * @todo Fill TLV and Send to BSM
       */
      if(rspMsg.varResult NOTEQ 0)
      {
        stopDehTimer(TIMERNAME_WAITRHCONFIGRSP);
        dehFillAndSendSetObjFail(&rhConfigRspInfo->setReq, BS_ERR_RH_CONFIG_FAILED, NULL);
     	rhConfigRspInfo->noOfMsgsWaitingForRsp = 0;
     	rhConfigRspInfo->isSetDelayRspReceived = TRUE;
     	clearBytes(&rhConfigRspInfo->setReq , sizeof(SetObjReq));
     	dehSetState(dehContext, DEH_STATE_IDLE);
      }
      else
      {
    	/* If User Configured */
    	for( index=0; index < rhConfigRspInfo->setReq.tlvArray.numTlvs; index++ )
    	{
    	  switch( rhConfigRspInfo->setReq.tlvArray.tlvInfo[index].tag.colId )
    	  {
    	    case COL_RADIOHEADCONFIG_TXDELAY:
    	      rhDefaultCfg->txDelay  = getUint32FromArray(rhConfigRspInfo->setReq.tlvArray.tlvInfo[index].data);
    	      break;
    	    case COL_RADIOHEADCONFIG_RXDELAY:
    	      rhDefaultCfg->rxDelay  = getUint32FromArray(rhConfigRspInfo->setReq.tlvArray.tlvInfo[index].data);
    	      break;
    	  }
    	}

    	/**
    	*  @todo Fill TLV Data Path Success Case
    	*/
        if(rhConfigRspInfo->noOfMsgsWaitingForRsp EQ 0)
        {
          /* Stop TIMERNAME_WAITCONFIGCOMPLETE Timer */
          stopDehTimer(TIMERNAME_WAITRHCONFIGRSP);
          /* Send Set OBJ Success to BSM */
      	  dehFillAndSendSetObjSucc(&rhConfigRspInfo->setReq);
      	  /* Updating values in Table */
      	  configTable->txDelay = rhDefaultCfg->txDelay;
          configTable->rxDelay = rhDefaultCfg->rxDelay;
        }
      }
    }
    else
    {
      logError("Delayed Response from Radio head for Delay Setting in DEH state = %d ",dehContext->dehState);
    }
  }
  else
  {
    logError("Delayed Response from Radio head for Delay Setting in DEH state = %d ",dehContext->dehState);
  }
}




/**
 * @param   decBuf contains Response message received from RH(LK)
 * @details This function Decodes TX On/Off Message
 */
void procRhDehTxOnOffRsp(
  IO DecBuf     *decBuf,
  IN UINT32 msgId
  )
{
  LkBsTxOnOffRsp         rspMsg;
  MsgDehBsmOpstartFail   opstartFail;

  clearBytes(&opstartFail , sizeof(MsgDehBsmOpstartFail));
  clearBytes(&rspMsg , sizeof(LkBsTxOnOffRsp));
  decRhDehTxOnOffRsp( decBuf, &rspMsg );
  DehContext    *dehContext = getDehContext();
  if(dehContext->dehState EQ DEH_STATE_WAIT_DEV_ENABLE_RSP)
  {
    RhEnableRspInfo  *rhEnableRspInfo = getEnableRspInfo();
    /* Check the result */
    if( rspMsg.varResult NOTEQ 0 )
    {
      /* OP-START Failure */
      opstartFail.errorNum = BS_ERR_RH_RSP_ERROR;
      snprintf((char*)&opstartFail.failReason, sizeof(opstartFail.failReason), "%s", (char *) decRhDehRspError(rspMsg.varResult, msgId));
      createAndSendMsg(MSG_DEH_BSM_OPSTART_FAIL, (UINT8*)&opstartFail, sizeof(MsgDehBsmOpstartFail));
      dehContext->isUnlocked = FALSE;
      /* Stop Timer */
      stopDehTimer(TIMERNAME_WAITRHENABLERSP);
      rhEnableRspInfo->noOfMsgsWaitingForRsp = 0; /* First Message Waiting For Response */
      dehSetState(dehContext, DEH_STATE_IDLE);
      return;
    }
    else
    {
      rhEnableRspInfo->noOfMsgsWaitingForRsp -= 1;
      logInfo("Number of Messages Still Waiting for RSP = %d in state= %s",
    		      rhEnableRspInfo->noOfMsgsWaitingForRsp,"DEH_STATE_WAIT_DEV_ENABLE_RSP");
      if(rhEnableRspInfo->noOfMsgsWaitingForRsp EQ 0)
      {
        stopDehTimer(TIMERNAME_WAITRHENABLERSP);
        dehSetState(dehContext, DEH_STATE_IDLE);
        dehContext->isUnlocked = TRUE;
        createAndSendMsg(MSG_DEH_BSM_OPSTART_SUCC, NULL, 0);
        setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
 	    return;
      }
    }
  }
  else if(dehContext->dehState EQ DEH_STATE_WAIT_DEV_DISABLE_RSP)
  {
    RhDisableRspInfo  *rhDisableRspInfo = getDisableRspInfo();
    /* Check the result */
    if( rspMsg.varResult NOTEQ 0 )
    {
    /* OP-STOP Failure */
      /* Stop Timer */
      stopDehTimer(TIMERNAME_WAITRHDISABLERSP);
      rhDisableRspInfo->noOfMsgsWaitingForRsp = 0; /* First Message Waiting For Response */
      /* OP-STOP is always success */
      createAndSendMsg(MSG_DEH_BSM_OPSTOP_SUCC, NULL, 0 );
      dehSetState(dehContext, DEH_STATE_IDLE);
      dehContext->isUnlocked = FALSE;
      setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
      return;
    }
    else
    {
      rhDisableRspInfo->noOfMsgsWaitingForRsp -= 1;
      logInfo("Number of Messages Still Waiting for RSP = %d in state= %s",
    		  rhDisableRspInfo->noOfMsgsWaitingForRsp,"DEH_STATE_WAIT_DEV_DISABLE_RSP");
      if(rhDisableRspInfo->noOfMsgsWaitingForRsp EQ 0)
      {
        stopDehTimer(TIMERNAME_WAITRHDISABLERSP);
        dehSetState(dehContext, DEH_STATE_IDLE);
        createAndSendMsg(MSG_DEH_BSM_OPSTOP_SUCC, NULL, 0);
        dehContext->isUnlocked = FALSE;
        setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
        return;
      }
    }
  }
  else
  {
    logError("Delayed Response from Radio head for Tx On Off Set in DEH state = %d ",dehContext->dehState);
  }
}

/**
 * @param    decBuf : Static buffer used for decode Message
 * @details  This function will process Radio head Enable and Disable procedures
 */
void procRhEnableDisableRsp(
  IO DecBuf     *decBuf,
  IN UINT32     msgId
  )
{
  LkBsEnableOrDisableRsp  rspMsg;
  MsgDehBsmOpstartFail  opstartFail;

  clearBytes(&opstartFail , sizeof(MsgDehBsmOpstartFail));
  clearBytes(&rspMsg , sizeof(LkBsEnableOrDisableRsp));
  decRhDehEnableOrDisableRsp( decBuf, &rspMsg );
  DehContext    *dehContext = getDehContext();
  if(dehContext->dehState EQ DEH_STATE_WAIT_DEV_ENABLE_RSP)
  {
    RhEnableRspInfo  *rhEnableRspInfo = getEnableRspInfo();
    /* Check the result */
    if( rspMsg.varResult NOTEQ 0 )
    {
    /* OP-START Failure */
      opstartFail.errorNum = BS_ERR_RH_RSP_ERROR;
      snprintf((char*)&opstartFail.failReason, sizeof(opstartFail.failReason), "%s", (char *) decRhDehRspError(rspMsg.varResult, msgId));
      createAndSendMsg(MSG_DEH_BSM_OPSTART_FAIL, (UINT8*)&opstartFail, sizeof(MsgDehBsmOpstartFail));
      dehContext->isUnlocked = FALSE;
      /* Stop Timer */
      stopDehTimer(TIMERNAME_WAITRHENABLERSP);
      rhEnableRspInfo->noOfMsgsWaitingForRsp = 0; /* First Message Waiting For Response */
      dehSetState(dehContext, DEH_STATE_IDLE);
      setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
      return;
    }
    else
    {
      rhEnableRspInfo->noOfMsgsWaitingForRsp -= 1;
      logInfo("Number of Messages Still Waiting for RSP = %d in state= %s",
    		  rhEnableRspInfo->noOfMsgsWaitingForRsp,"DEH_STATE_WAIT_DEV_DISABLE_RSP");
      if(rhEnableRspInfo->noOfMsgsWaitingForRsp EQ 0)
      {
        dehSetState(dehContext, DEH_STATE_IDLE);
        stopDehTimer(TIMERNAME_WAITRHENABLERSP);
        dehContext->isUnlocked = TRUE;
        createAndSendMsg(MSG_DEH_BSM_OPSTART_SUCC, NULL, 0);
        setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
        return;
      }
    }
  }
  else if(dehContext->dehState EQ DEH_STATE_WAIT_DEV_DISABLE_RSP)
  {
    RhDisableRspInfo  *rhDisableRspInfo = getDisableRspInfo();
    /* Check the result */
    if( rspMsg.varResult NOTEQ 0 )
    {
    /* OP-STOP Failure */
      /* Stop Timer */
      stopDehTimer(TIMERNAME_WAITRHDISABLERSP);
      rhDisableRspInfo->noOfMsgsWaitingForRsp = 0; /* First Message Waiting For Response */
      /* OP-STOP is always success */
      createAndSendMsg(MSG_DEH_BSM_OPSTOP_SUCC, NULL, 0 );
      dehSetState(dehContext, DEH_STATE_IDLE);
      dehContext->isUnlocked = FALSE;
      setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
      return;
    }
    else
    {
      rhDisableRspInfo->noOfMsgsWaitingForRsp -= 1;
      if(rhDisableRspInfo->noOfMsgsWaitingForRsp EQ 0)
      {
        stopDehTimer(TIMERNAME_WAITRHDISABLERSP);
        dehSetState(dehContext, DEH_STATE_IDLE);
        createAndSendMsg(MSG_DEH_BSM_OPSTOP_SUCC, NULL, 0);
        dehContext->isUnlocked = FALSE;
        setProcedureOngoing( DEV_RADIOHEAD, PROC_NONE );
        return;
      }
    }
  }
  else
  {
     logError("Delayed Response from Radio head for Tx Enable/Disable in DEH state = %d ",dehContext->dehState);
  }
}

/**
 * @param getReq    Get Object Request message
 * @details This function will get the function pointer for getStruct2TlvFunc function of
 *          the respective Table, encode the information and send to the OAM-IF.
 **/
void dehGetAndSendTableData(
  IO GetObjReq   *getReq
  )
{
  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  initEncBuf( &encBuf );
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  TagInfo       *tagInfo = &getReq->tagArray.tagInfo[0]; /* One Get = One Req message */

  GenericTable  *tableFunc = getTableFunctions(tagInfo->tableId);

  /* Fill CMD REF details */
  copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

  if ( tagInfo->colId EQ 0 )
  {
    /* Fill Structure */
    if( NULL NOTEQ tableFunc->getStruct2TlvFunc )
    {
      tableFunc->getStruct2TlvFunc( tagInfo, &rspTlv.tlvArray );
    }
    else
    {
      BS_ERROR_NO_STRUCT2TLV_FUNCTION_DEFINED( tagInfo->tableId );
      /**
       * @todo Get table name in string
       */
    }
  }
  else
  {
    GenericTable  *tableFunc = getTableFunctions(tagInfo->tableId);
    /* Fill Structure */
    if( NULL NOTEQ tableFunc->getElementFunc )
    {
      tableFunc->getElementFunc( tagInfo->tableId, tagInfo->rowId-1, tagInfo->colId, &rspTlv.tlvArray );
    }
    else
    {
      logInfo("No getElementFunc Function defined for table %d, Row = %d and Col = %d ", tagInfo->tableId,
                                                                          tagInfo->rowId, tagInfo->colId );
    }
  }
  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );
  /* Send */
  createAndSendMsg(MSG_DEH_BSM_GET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);
}

/**
 * @param    decBuf : Static buffer used for decode Message
 * @details  This function will process Radio head Status Rsp
 */
void procRhDehStatusRsp(
  IO DecBuf     *decBuf
  )
{
  DehContext        *dehContext = getDehContext();
  TBOOL              result = FALSE;
  UINT16             seqNum = 0;
  RhStatusRspInfo   *rhStatusRspInfo = NULL;
  TimerName          timer;

  RadioHeadStatus *statusTable = (RadioHeadStatus *)getFirstRowOfTable(RADIOHEADSTATUS);
  result = decRhDehStatusInfoRsp( decBuf, statusTable , &seqNum );

  /* Identifying the interface from where the request is received */
  if(seqNum EQ OAMIF_CIH)
  {
    rhStatusRspInfo = getStatusRspInfo(OAMIF_CIH);
    timer = TIMERNAME_WAITRHSTATUSRSPFORCIH;
  }
  else if(seqNum EQ OAMIF_WIH)
  {
    rhStatusRspInfo = getStatusRspInfo(OAMIF_WIH);
    timer = TIMERNAME_WAITRHSTATUSRSPFORWIH;
  }
  else
  {
    rhStatusRspInfo = getStatusRspInfo(OAMIF_SIH);
    timer = TIMERNAME_WAITRHSTATUSRSPFORSIH;
  }

  /* Checking whether a delayed response is received */
  if(rhStatusRspInfo->noOfMsgsWaitingForRsp < 1)
  {
    logError("Delayed Response from Radio head for RH Status Request in DEH state = %d ",dehContext->dehState);
    return;
  }

  /* STOP Timer */
  stopDehTimer( timer );
  rhStatusRspInfo->noOfMsgsWaitingForRsp = 0;

  /* Send Get OBJ Fail to BSM if decoding the response from RH fails*/
  if(result EQ FALSE)
  {
    dehFillAndSendGetObjFail( &rhStatusRspInfo->getReq, BS_ERR_RH_RSP_ERROR );
    clearBytes(&rhStatusRspInfo->getReq, sizeof(GetObjReq));
    return;
  }
  /*  Send Get OBJ SUCC to BSM for Single Element */
  if(rhStatusRspInfo->getReq.tagArray.numTags EQ 1)
  {
    dehGetAndSendTableData( &rhStatusRspInfo->getReq );
    clearBytes(&rhStatusRspInfo->getReq, sizeof(GetObjReq));
  }
  else
  {
    /*  Send Get OBJ SUCC to BSM for whole table */
    dehGetAndSendTableData( &rhStatusRspInfo->getReq );
    clearBytes(&rhStatusRspInfo->getReq, sizeof(GetObjReq));
  }
}


/**
 * @param   decBuf contains Response message received from RH(LK)
 * @details This function Process Incoming Message from Other Modules to DEH and calls respective action.
 */
void procRhDehResetRsp(
  IO DecBuf     *decBuf
  )
{
  UINT32         userInfo;
  LkBsResetRsp   rspMsg;
  DehContext    *dehContext = getDehContext();
  DeviceStatusContext *deviceStatusContext = getDeviceStatusContext();
  DehRhRspTimersContext  *timersContext = getRhRspTimersContext();

  clearBytes(&rspMsg,sizeof(LkBsResetRsp));

  decRhDehResetRsp( decBuf, &rspMsg );

  /* Fill Global Context with the Reference Information */
  if(timersContext->rhInitTimerInfo.dehCmdType EQ RSP_USER_INITIATED)
  {
    /* Stop Timer */
    stopDehTimer( TIMERNAME_WAITRHINIT);
    timersContext->rhInitTimerInfo.dehCmdType = RSP_UNEXPECTED;
    /* Check the result */
    if( rspMsg.varResult NOTEQ 0 )
    {
      BS_ERROR_RH_RSP_ERROR("MODID_DEH","RESET RH",dehContext->dehState );
      dehFillAndSendSetObjFail(&timersContext->rhInitTimerInfo.setReq, BS_ERR_RH_RSP_ERROR, NULL );
      dehSetState(dehContext, DEH_STATE_IDLE);
      return;
    }
    dehFillAndSendSetObjSucc(&timersContext->rhInitTimerInfo.setReq);
    /* Send Success Message and change State */
    /* Update Global Database */
    deviceStatusContext->presentDevStatus.devConnectionStatus[DEV_RADIOHEAD] = DEV_UNCONNECTED;
    deviceStatusContext->lastSavedDevStatus.devConnectionStatus[DEV_RADIOHEAD] = DEV_UNCONNECTED;
    dehSetState(dehContext, DEH_STATE_WAIT_INIT_NOTI);
    /* Start Timer */
    startDehTimer( TIMERNAME_WAITRHINIT, dehContext, &userInfo );
  }
  /* Ignore Reset Response , Wait for Init Message From RH and then send send system reset message to Operator */
}
/**
 * @param    alarmInfo : Alarm information received from RH
 * @param    alarmMacro : Alarm Macro
 * @details  This function will check alarm is present and sent the status of alarm.
 */

TBOOL isAlarmPresent(
    UINT32 alarmInfo,
    UINT32 alarmMacro
    )
{
	if(alarmInfo & alarmMacro)
	{
	return TRUE;
	}
	return FALSE;
}
/**
 * @param    encodedRsp : Alarms Response received from Radio Head in Encoded format
 * @param    rspMsg : Response message contains all alarms
 * @details  This function will decode encodedRsp and fills rspMsg
 */
void fillAlarmInfoRsp(
  IN LkBsAlarmInfoRsp *encodedRsp,
  IO LkAlarmTable     *rspMsg
  )
{
  UINT32 alarmInfoCommon = encodedRsp->alarmDataCommon;
  UINT32 alarmInfoPathA  = encodedRsp->alarmDataPathA;
  UINT32 alarmInfoPathB  = encodedRsp->alarmDataPathB;

  rspMsg->clockFail         = isAlarmPresent(alarmInfoCommon , ALARM_DATA_CLOCK_FAIL);
  rspMsg->highTemperature   = isAlarmPresent(alarmInfoCommon , ALARM_DATA_HIGH_TEMP);
  rspMsg->lowTemperature    = isAlarmPresent(alarmInfoCommon , ALARM_DATA_LOW_TEMP);

  rspMsg->overPowerSideA    = isAlarmPresent(alarmInfoPathA , ALARM_DATA_OVER_POWER);
  rspMsg->vswrFailsideA     = isAlarmPresent(alarmInfoPathA , ALARM_DATA_VSWR_FAIL);
  rspMsg->lowGainsideA      = isAlarmPresent(alarmInfoPathA , ALARM_DATA_LOW_GAIN);
  rspMsg->lnaFailSideA      = isAlarmPresent(alarmInfoPathA , ALARM_DATA_LNA_FAIL);
  rspMsg->disabledSideA     = isAlarmPresent(alarmInfoPathA , ALARM_DATA_DISABLED);
  rspMsg->shutdownSideA     = isAlarmPresent(alarmInfoPathA , ALARM_DATA_SHUTDOWN);

  rspMsg->overPowerSideB    = isAlarmPresent(alarmInfoPathB , ALARM_DATA_OVER_POWER);
  rspMsg->vswrFailSideB     = isAlarmPresent(alarmInfoPathB , ALARM_DATA_VSWR_FAIL);
  rspMsg->lowGainSideB      = isAlarmPresent(alarmInfoPathB , ALARM_DATA_LOW_GAIN);
  rspMsg->lnaFailSideB      = isAlarmPresent(alarmInfoPathB , ALARM_DATA_LNA_FAIL);
  rspMsg->disabledSideB     = isAlarmPresent(alarmInfoPathB , ALARM_DATA_DISABLED);
  rspMsg->shutdownSideB     = isAlarmPresent(alarmInfoPathB , ALARM_DATA_SHUTDOWN);
}

/**
 * @param    decBuf : Static buffer used for decode Message
 * @details  This function will process Radio head Alarm Change Notification
 */
void procRhDehAlarmChangeNoti(
  IO DecBuf     *decBuf
  )
{
  LkBsAlarmChangeNoti  notiMsg;
  EncBuf               encBuf;
  LkAlarmTable         rspMsg;
  BsLkAlarmChangeAck   ackMsg;
  LkHdr                hdr;
  TBOOL result = FALSE;

  clearBytes(&notiMsg , sizeof(LkBsAlarmChangeNoti));
  initEncBuf( &encBuf );
  clearBytes(&hdr , sizeof(LkHdr));
  clearBytes(&rspMsg , sizeof(LkAlarmTable));

  result = decRhDehAlarmChangeNoti( decBuf, &notiMsg );
  /**
   *  @todo Fill actual Buffer to be sent to CIH/SIH/WIH -- This may change to TLV format
   */
  fillAlarmInfoRsp(&notiMsg, &rspMsg);
  /* Send ACK to Radio Head */
  fillHdr( MSG_BS_LK_ALARM_CHANGE_ACK_ID, &hdr );
  fillDehRhAlarmChangeAck( notiMsg.varSeqNum , &ackMsg );

  initEncBuf(&encBuf);
  encDehRhHeader( &hdr, &encBuf );
  encDehRhAlarmChangeAck( &ackMsg, &encBuf );

  sendMsgToRh( &encBuf );
  logMsgOut("ACK Sent to RH: message is %s","MSG_BS_LK_ALARM_CHANGE_ACK_ID");
  /*Process and send alarm to BSM*/
  dehProcSendRhAlarmMsgToBsm(&rspMsg);
}
#if 0
/**
 * @param setObj       Set Object Request which was Sent by Operator
 * @details This function will fill and send Set Obj Success to BSM
 */

void dehFillAndSendHwInfoToBsm(
  IN LkHwInfoTable   *rspMsg
  )
{
  SetObjReq   req;
  EncBuf      encBuf;

  clearBytes( &encBuf, sizeof(EncBuf) );
  clearBytes( &req, sizeof(SetObjReq) );

  strncpy(tlvArray->tlvInfo[0].data,*colValue,tlvArray->tlvInfo[0].len);

  copyBytes( &setReq->cmdRef, sizeof(CmdRef), &(rspSucc.cmdRef));
  copyBytes( &setReq->tlvArray, sizeof(TlvArray), &(rspSucc.tlvArray));

  encSetObjRspSuccMsg( &rspSucc, &encBuf );

  createAndSendMsg( MSG_DEH_BSM_SET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len );
}
#endif
/**
 * @param    decBuf : Static buffer used for decode Message
 * @details  This function will process Radio head Hardware Info Response
 */
void procRhDehHwInfoRsp(
  IO DecBuf     *decBuf
  )
{
  LkHwInfoTable   *rspMsg = NULL;
  TBOOL           result = FALSE;
  ComponentStatus   *rowOfTable = NULL;

  rspMsg = getRhHardWareInfo();

  result = decRhDehHwInfoRsp( decBuf, rspMsg);

  if(result EQ TRUE)
  {
	/* Update Component Status Table with Radiohead Hardware Description */
	rowOfTable = (ComponentStatus*)getRowOfTable(COMPONENTSTATUS, DEV_RADIOHEAD - 1 );
	snprintf(rowOfTable->infoString, MAX_SIZE_COMPONENTSTATUS_INFOSTRING + 1, "%s", rspMsg->description);
  }
}

/**
 * @param    decBuf : Static buffer used for decode Message
 * @details  This function will process Radio head Firmware Info Response
 */
void procRhDehFwInfoRsp(
  IO DecBuf     *decBuf
  )
{
  TBOOL result;
  ComponentStatus   *rowOfTable = NULL;

  MsgDehPrmRhVersionInfo       rhVersionInfo;
  LkBsFwInfoRsp                rspMsg;

  clearBytes(&rhVersionInfo, sizeof(MsgDehPrmRhVersionInfo));
  clearBytes(&rspMsg,        sizeof(LkBsFwInfoRsp));

  DehContext *dehContext = getDehContext();

  result = decRhDehFwInfoRsp( decBuf , &rspMsg);

  if(result EQ TRUE)
  {
	/* Update Component Status Table with Radiohead Firmware Version */
	rowOfTable = (ComponentStatus*)getRowOfTable(COMPONENTSTATUS, DEV_RADIOHEAD - 1);
	snprintf(rowOfTable->componentVersion,  MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1, "%s", dehContext->rhPackageStatus.runningVersion);
  }

 /** CR:22-May-2012: with new design, this check is not required, always update PRM */

  snprintf(rhVersionInfo.runningSideInfo.version , MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1,"%s", dehContext->rhPackageStatus.runningVersion);
  snprintf(rhVersionInfo.runningSideInfo.versionBuildDate , MAX_FWBUILDINFO_DESC + 1,"%s", dehContext->rhPackageStatus.runningVersionBuildDate);
  snprintf(rhVersionInfo.runningSideInfo.rhSide , RH_SIDE_INFO + 1,"%s", getRhPathName(dehContext->rhPackageStatus.runningSide));
  snprintf(rhVersionInfo.nonRunningSideInfo.version , MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1,"%s", dehContext->rhPackageStatus.nonRunningVersion);
  snprintf(rhVersionInfo.nonRunningSideInfo.versionBuildDate , MAX_FWBUILDINFO_DESC + 1,"%s", dehContext->rhPackageStatus.nonRunningVersionBuildDate);
  snprintf(rhVersionInfo.nonRunningSideInfo.rhSide , RH_SIDE_INFO + 1,"%s", getRhPathName(dehContext->rhPackageStatus.nonRunningSide));
  createAndSendMsg(MSG_DEH_PRM_RH_VERSION_INFO, (UINT8*)&rhVersionInfo, sizeof(MsgDehPrmRhVersionInfo) );

}

/**
 * @param    decBuf : Static buffer used for decode Message
 * @details  This function will process Radio head Firmware Info Response
 */
void procRhDehOperatingModeInfoRsp(
  IO DecBuf     *decBuf
  )
{
  LkBsOperationalModeInfoRsp rspMsg;
  DehContext *dehContext = getDehContext();
  /* Device Status for Radio Head Device is Received */
  dehContext->isRhRspDeviceStatusReceived = TRUE;
  decRhDehOperationalModeInfoRsp( decBuf, &rspMsg );
  if (isProcedureOnGoing(DEV_RADIOHEAD,PROC_DEV_INIT))
  {
    setProcedureCompleted(DEV_RADIOHEAD,PROC_DEV_INIT);
  }
  if(!isConnected(DEV_RADIOHEAD))
  {
    DevStatus *presentStatus  = getDevicePresentStatusPtr();
    presentStatus->devConnectionStatus[DEV_RADIOHEAD] = DEV_CONNECTED;
    setConnected(DEV_RADIOHEAD);


    dehRhHardwareInfo();
    dehRhFirmwareInfo();
    /* In case of DEH_STATE_WAIT_CFG -- No need to change State*/
    if(dehContext->dehState NOTEQ DEH_STATE_WAIT_CFG || dehContext->dehState EQ DEH_STATE_WAIT_INIT_NOTI)
    {
      dehSetState(dehContext, DEH_STATE_IDLE);
    }

  }
  if ( ( TRUE EQ dehContext->isBsmDehDeviceInitReqReceived ) && ( isConnected( DEV_RADIOHEAD ) ) )
  {
    createAndSendComponentStatusToBsm();
    dehContext->isBsmDehDeviceInitReqReceived = FALSE;
  }
  if( isConnected( DEV_CPRI ) && ( dehContext->dehState EQ DEH_STATE_WAIT_DEV_ENABLE_RSP ) )
  {
    stopDehTimer( TIMERNAME_WAITCPRILINKUP );
    if( FALSE EQ isProcedureOnGoing( DEV_RADIOHEAD, PROC_OPSTART ) )
    {
      doRhConfigAndSendOpstartRspToBsm();
      setProcedureOngoing( DEV_RADIOHEAD, PROC_OPSTART );
    }
  }
}

/**
 * @param    noti : LkBsRemoteFusingCompleteNoti
 * @param    fail : MsgDehPrmRhSwInstallFail
 * @details  This function fills the error details if software installation Response failed sent from RH in
 *            Remote Fusion NOTI.
 */
void fillErrorDescriptionFusionNoti(
  IN LkBsRemoteFusingCompleteNoti *noti,
  IO MsgDehPrmRhSwInstallFail *fail
  )
{
  switch(noti->varResult)
  {
    case -101:
    {
      fail->errorDesc.errorNum = BS_ERR_INSTALLATION_TEMPORARY_DISABLED;
      snprintf(fail->errorDesc.failReason, sizeof(String256), "Rh Installation feature disabled temporarily");
      break;
    }
    case -110:
    {
      fail->errorDesc.errorNum = BS_ERR_RAM_MEMORY_INSUFFICIENT;
      snprintf(fail->errorDesc.failReason, sizeof(String256), "RAM Memory is not sufficient");
      break;
    }
    case -111:
    {
      fail->errorDesc.errorNum = BS_ERR_PERSISTENT_MEMORY_INSUFFICIENT;
      snprintf(fail->errorDesc.failReason, sizeof(String256), "Persistent Memory is not sufficient");
      break;
    }
    case -203:
    {
      fail->errorDesc.errorNum = BS_ERR_IMAGE_CRC_CHECK_FAIL;
      snprintf(fail->errorDesc.failReason, sizeof(String256), "Rh Image CRC check failed");
      break;
    }
    default:
    {
      fail->errorDesc.errorNum = BS_ERR_UNSPECIFIED;
      snprintf(fail->errorDesc.failReason, sizeof(String256), "Unknown error");
      break;
    }
  }
}

/**
 * @param    rsp : BsLkSwInstallRsp
 * @param    fail : MsgDehPrmRhSwInstallFail
 * @details  This function fills the error details if software installation Response failed from RH
 */
void fillErrorDescriptionInstallRsp(
  IN LKBsSwInstallRsp         *rsp,
  IO MsgDehPrmRhSwInstallFail *fail
  )
{
  switch(rsp->varResult)
  {
    case -207:
    {
      fail->errorDesc.errorNum = BS_ERR_FAILED_CONNECTION_INIT;
      snprintf(fail->errorDesc.failReason, sizeof(String256), "Failed to connect to host");
      break;
    }
    case -208:
    {
      fail->errorDesc.errorNum = BS_ERR_FTP_SERVER_ABORT;
      snprintf(fail->errorDesc.failReason, sizeof(String256), "FTP server aborted");
      break;
    }
    case -209:
    {
      fail->errorDesc.errorNum = BS_ERR_FTP_WEIRD_USER_REPLY;
      snprintf(fail->errorDesc.failReason, sizeof(String256), "FTP rejects due to wrong username");
      break;
    }
    case -210:
    {
      fail->errorDesc.errorNum = BS_ERR_FTP_WEIRD_PASS_REPLY;
      snprintf(fail->errorDesc.failReason, sizeof(String256), "FTP rejects due to wrong password");
      break;
    }
    case -211:
    {
      fail->errorDesc.errorNum = BS_ERR_INSTALL_FILE_SIZE_EXCEEDED;
      snprintf(fail->errorDesc.failReason, sizeof(String256), "Image file size exceeded");
      break;
    }
    default:
    {
      fail->errorDesc.errorNum = BS_ERR_UNSPECIFIED;
      snprintf(fail->errorDesc.failReason, sizeof(String256), "Unknown error");
      break;
    }

  }
}

/**
 * @param    decBuf : Static buffer used for decode Message
 * @details  This function will process Radio head Software installation Response
 */
void procRhDehSwInstallRsp(
  IO DecBuf     *decBuf
  )
{
  LKBsSwInstallRsp rsp;
  clearBytes(&rsp, sizeof(LKBsSwInstallRsp));

  /*Decode the Software Install Response*/
  decRhDehSwInstallRsp(decBuf, &rsp);

  if(rsp.varResult NOTEQ 0)
  {
    MsgDehPrmRhSwInstallFail     installFail;
    clearBytes(&installFail, sizeof(MsgDehPrmRhSwInstallFail));

    setProcedureCompleted(DEV_RADIOHEAD,PROC_SW_INSTALL);
    fillErrorDescriptionInstallRsp(&rsp,&installFail);
    createAndSendMsg(MSG_DEH_PRM_RH_SW_INSTALL_FAIL, (UINT8*)&installFail, sizeof(MsgDehPrmRhSwInstallFail) );
  }
  /* else: wait for MSG_LK_BS_FUSING_COMPLETE_NOTI_ID */
}

/**
 * @param    decBuf : Static buffer used for decode Message
 * @details  This function will process Radio head Remote Fusing Complete Notification
 **/
void procRhDehFusingCompleteNoti(
  IO DecBuf     *decBuf
  )
{
  LkBsRemoteFusingCompleteNoti noti;

  clearBytes(&noti, sizeof(LkBsRemoteFusingCompleteNoti));


  /*Decode the Remote fusing complete notification*/
  decRhDehRemoteFusingCompleteNoti( decBuf, &noti);

  /* Send ACK */
  dehSendRhFusingNotiAck();

  if(noti.varResult NOTEQ 0)
  {
    MsgDehPrmRhSwInstallFail     installFail;
    clearBytes(&installFail, sizeof(MsgDehPrmRhSwInstallFail));

    setProcedureCompleted(DEV_RADIOHEAD,PROC_SW_INSTALL);
    fillErrorDescriptionFusionNoti(&noti,&installFail);
    createAndSendMsg(MSG_DEH_PRM_RH_SW_INSTALL_FAIL, (UINT8*)&installFail, sizeof(MsgDehPrmRhSwInstallFail) );
  }
  else/* send success: procedure complete */
  {
    MsgDehPrmRhSwInstallSucc     installSucc;
    clearBytes(&installSucc, sizeof(MsgDehPrmRhSwInstallSucc));
    DehContext *dehContext = getDehContext();

    setProcedureCompleted(DEV_RADIOHEAD,PROC_SW_INSTALL);
    snprintf(installSucc.version , MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1,"%s", dehContext->rhPackageStatus.nonRunningVersion);
    createAndSendMsg(MSG_DEH_PRM_RH_SW_INSTALL_SUCC, (UINT8*)&installSucc, sizeof(MsgDehPrmRhSwInstallSucc) );

    /* SW INSTALL has completed. However, we wait for the SW version numbers and BuildOnDate to be provided by RH  */
    dehRhFirmwareInfo();
    /* Wait for MSG_LK_BS_FW_INFO_RSP from RH */
  }
}

/**
 * @param    decBuf : Static buffer used for decode Message
 * @details  This function will process Radio head Software Activation Response
 */
void procRhDehSwActivateRsp(
  IO DecBuf     *decBuf,
  IN UINT32     msgId
  )
{

  LkBsSwActivateRsp rsp;
  clearBytes(&rsp, sizeof(LkBsSwActivateRsp));

  /*Decode the Software Activation Response*/
  decRhDehSwActivateRsp(decBuf, &rsp);

  if(rsp.varResult NOTEQ 0)
  {
    MsgDehPrmRhSwActivateFail fail;
    clearBytes(&fail, sizeof(MsgDehPrmRhSwActivateFail));

    setProcedureCompleted(DEV_RADIOHEAD,PROC_SW_ACTIVATE);
    fail.errorDesc.errorNum = BS_ERR_INTERNAL_ERROR_OCCURED;
    snprintf((char*)&fail.errorDesc.failReason, sizeof(String256), "%s",(char *) decRhDehRspError(rsp.varResult, msgId));
    createAndSendMsg(MSG_DEH_PRM_RH_SW_ACTIVATE_FAIL, (UINT8*)&fail, sizeof(MsgDehPrmRhSwActivateFail) ); /** CR:22-May-2012: MsgDehPrmRhSwInstallFail -> MsgDehPrmRhSwActivateFail */
  }
  else
  {/*Send the reset request as the RH Software will activate on restart of RH*/
    dehSendRhResetReq();/*TODO* need to initialise INIT timer*/
    /* wait for MSG_LK_BS_RESET_RSP, ignore it.
       wait for MSG_LK_BS_INITIALIZATION_NOTI_ID */
  }
}


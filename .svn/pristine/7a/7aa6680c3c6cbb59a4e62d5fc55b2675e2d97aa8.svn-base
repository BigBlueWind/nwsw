/**
 * @file    bsm_cmd_set.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions which handles all the set messages from user interfaces (CIH, SIH, WIH)
 *
 * @author:  Shweta Polepally
 * @date:    August 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**                                                                                                                                                  
 * @param   instnaceId  Instance Id of the Cih
 * @details             This function is used get MsgId for Set Object Success for each instnace
 */

MsgId bsmCihSetObjSuccMsgId(
    IN  InstanceId instanceId
    )
{
  MsgId msgId;
  switch(instanceId)
  {
    case 1:
      msgId = MSG_BSM_CIH1_SET_OBJ_SUCC;
      break;
    case 2:
      msgId = MSG_BSM_CIH2_SET_OBJ_SUCC;
      break;
    case 3:
      msgId = MSG_BSM_CIH3_SET_OBJ_SUCC;
      break;
    default:
      msgId = MSG_UNDEFINED;break;
  }

  return msgId;
}

/**                                                                                                                                                  
 * @param   instanceId  Instance Id of the Cih
 * @details             This function is used get MsgId for Set Object Fail for each instnace
 */
MsgId bsmCihSetObjFailMsgId(
    IN InstanceId instanceId
    )
{
  MsgId msgId;
  switch(instanceId)
  {
    case 1:
      msgId = MSG_BSM_CIH1_SET_OBJ_FAIL;
      break;
    case 2:
      msgId = MSG_BSM_CIH2_SET_OBJ_FAIL;
      break;
    case 3:
      msgId = MSG_BSM_CIH3_SET_OBJ_FAIL;
      break;
    default:
      msgId = MSG_UNDEFINED;break;
  }

  return msgId;
}

/**
 * @param setObj   Set Object Request which was Sent by CIH
 * @details This function will fill and send MSG_BSM_<OAM-IF>_SET_OBJ_SUCC to OAM-Interface (CIH/SIH/WIH).
 */
void bsmFillAndSendSetObjSucc(
    IN SetObjReq   *setObj
    )
{
  SetObjRspSucc   rspSucc;
  EncBuf          encBuf;
  MsgId           msgId = MSG_UNDEFINED;
  initEncBuf( &encBuf );
  clearBytes( &rspSucc, sizeof(SetObjRspSucc) );

  /* Fill the Rsp with the same request received */
  copyBytes( setObj, sizeof(SetObjReq), &rspSucc );
  /* Encode the Response message */
  encSetObjRspSuccMsg( &rspSucc, &encBuf );

  switch(setObj->cmdRef.origin)
  {
    case OAMIF_CIH: msgId = bsmCihSetObjSuccMsgId(setObj->cmdRef.instanceId); break;
    case OAMIF_WIH: msgId = MSG_BSM_WIH_SET_OBJ_SUCC; break;
    case OAMIF_SIH: msgId = MSG_BSM_SIH_SET_OBJ_SUCC; break;
    default:
         logError("Unknown OAMIF=%d", setObj->cmdRef.origin);
         break;
  }
  createAndSendMsg( msgId, (UINT8*)&encBuf.value, encBuf.len);
}

/**
 * @param setReq       Set Object Request which was Sent by CIH
 * @param bsErrorValue BS Error Value - Reason why the Request was rejected.
 * @param errTagInfo   Tag Information which failed.
 * @details This function will fill and send MSG_BSM_<OAM-IF>_SET_OBJ_FAIL to CIH/WIH/SIH module.
 */
void bsmFillAndSendSetObjFail(
    IN SetObjReq          *setReq,
    IN BsErrorValue        bsErrorValue,
    IN TagInfo            *errTagInfo
    )
{
  SetObjRspFail   rspFail;
  EncBuf          encBuf;
  MsgId           msgId = MSG_UNDEFINED;
  initEncBuf( &encBuf );
  clearBytes(&rspFail, sizeof(SetObjRspFail));

  copyBytes( &(setReq->cmdRef), sizeof(CmdRef), &(rspFail.cmdRef));
  copyBytes( &(setReq->tlvArray), sizeof(TlvArray), &(rspFail.tlvArray));
  rspFail.setErrRsp.errRspCode = bsErrorValue;
  if ( errTagInfo NOTEQ NULL )
  {
    copyBytes( errTagInfo, sizeof(TagInfo), &(rspFail.setErrRsp.failedTagInfo));
  }
  encSetObjRspFailMsg( &rspFail, &encBuf );

  switch(setReq->cmdRef.origin)
  {
    case OAMIF_CIH: msgId = bsmCihSetObjFailMsgId(setReq->cmdRef.instanceId); break;
    case OAMIF_WIH: msgId = MSG_BSM_WIH_SET_OBJ_FAIL; break;
    case OAMIF_SIH: msgId = MSG_BSM_SIH_SET_OBJ_FAIL; break;
    default:
         logError("Unknown OAMIF=%d", setReq->cmdRef.origin);
         break;
  }
  createAndSendMsg( msgId, (UINT8*)&encBuf.value, encBuf.len );
}

/**
 * @param   setObjFail   Set Object Request
 * @details This function will create and send MSG_BSM_<OAM-IF>_SET_OBJ_FAIL to OAM-Interface (CIH/SIH/WIH).
 */
void bsmCreateAndSendSetObjFail(
    IO SetObjRspFail   *setObjFail
    )
{
  EncBuf          encBuf;
  MsgId           msgId;
  initEncBuf( &encBuf );

  /* Encode the Response message */
  encSetObjRspFailMsg( setObjFail, &encBuf );

  switch(setObjFail->cmdRef.origin)
  {
    case OAMIF_CIH: msgId = bsmCihSetObjFailMsgId(setObjFail->cmdRef.instanceId); break;
    case OAMIF_WIH: msgId = MSG_BSM_WIH_SET_OBJ_FAIL; break;
    case OAMIF_SIH: msgId = MSG_BSM_SIH_SET_OBJ_FAIL; break;
    default:
         logError("Unknown OAMIF=%d", setObjFail->cmdRef.origin);
         break;
  }
  createAndSendMsg( msgId, (UINT8*)&encBuf.value, encBuf.len);
}

/**
 * @param   tagInfo  tag information of each element.
 * @details This function will check it lock is required to perform SET on this element or not.
 * */
static TBOOL isLockRequired(
    IN TagInfo   *tagInfo
    )
{
  BsmOamTableInfo *tableInfo = bsmGetTablePtrByTableId(tagInfo->tableId);
  //logInfo( "isLockRequired func : tagInfo->tableId = %d coldId =%d attributes = %d ", tagInfo->tableId, tagInfo->colId, tableInfo->colInfo[tagInfo->colId].attributes );
  /* if Lock is required to set the element, return TRUE */
  if ( tableInfo->colInfo[tagInfo->colId].attributes EQ  WRITE_WHEN_LOCK_ONLY )
  {
    logInfo("Lock required for table:%d, row:%d, col%d", tagInfo->tableId, tagInfo->rowId, tagInfo->colId );
    return TRUE;
  }
  else
  {
    logInfo("Lock NOT required for table:%d, row:%d, col%d", tagInfo->tableId, tagInfo->rowId, tagInfo->colId );
    return FALSE;
  }
}

/**
 * @details This function will get config change ACK by EMS field.
 **/
static TBOOL getConfigChangeAckByEmsFieldValue()
{
  EnodebConfig   *enodebConfig = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
  return enodebConfig->configChangeAckByEms;
}

/**
 * @param   setObj Set Object Request Message.
 * @details This function will inform SIH about the config change.
 **/
static void informSihAbtTheConfigChange(
    IO SetObjReq      *setObj
    )
{
  /* Send ConfigurationChanged Event to SIH if SET was not requested by SIH Module */
  if ( setObj->cmdRef.origin && ( setObj->cmdRef.origin NOTEQ OAMIF_SIH ) )
  {
    /* In case of CIH or WIH Set Object request */
    if ( TRUE EQ getConfigChangeAckByEmsFieldValue() )
    {
      sendEventConfigurationChanged( setObj->cmdRef.origin );
    }
  }
}

/**
 * @param msg          Message received.
 * @param setObj       Set Object Request which was Sent by CIH
 * @param bsmContext   BSM context information
 * @details This function will process the SET request message on Operator command table.
 */
static void bsmProcSetObjReqOnOperCmdTable(
    IN Msg            *msg,
    IO SetObjReq      *setObj,
    IO BsmContext     *bsmContext
    )
{
  TableId tableId = setObj->tlvArray.tlvInfo[0].tag.tableId;
  UINT8 i = 0;
  TBOOL retVal = FALSE;

  if ( FALSE EQ isEnodeBLocked() )
  {
    logInfo( "setObj->tlvArray.numTlvs = %d", setObj->tlvArray.numTlvs );
    /* if not locked, check isLockRequired to set the element */
    for ( i=0; i < setObj->tlvArray.numTlvs; i++ )
    {
      retVal = isLockRequired ( &setObj->tlvArray.tlvInfo[i].tag );
      if( retVal EQ TRUE )
      {
        bsmFillAndSendSetObjFail( setObj, BS_ERR_ENODEB_NOT_LOCKED, &setObj->tlvArray.tlvInfo[0].tag );
        return;
      }
    }
  }

  switch( tableId )
  {
    case ENBADMIN:
      bsmPerformOperationOnEnbAdmin( msg, setObj, bsmContext );
      break;
    case FILEDOWNLOAD:
    case FILEUPLOAD:
      createAndSendMsg( MSG_BSM_FTH_SET_OBJ_REQ, msg->data, msg->len );
      break;
    case SWACTIVATION:
      bsmPerformOperationOnSwActivation( setObj, bsmContext );
      break;
    case SWINSTALL:
      bsmPerformOperationOnSwInstall( setObj );
      break;
    case TRACECONTROL:
#ifdef L3MSGTRC_SUPPORTED
      createAndSendMsg( MSG_BSM_LTH_SET_OBJ_REQ, msg->data, msg->len );
#else
      bsmFillAndSendSetObjFail(setObj,BS_ERR_FEATURE_NOT_SUPPORTED,  &setObj->tlvArray.tlvInfo[0].tag);
#endif
      break;
    case CELLADMIN:
      bsmPerformOperationOnCellAdminTable(setObj, bsmContext);
      break;
    case COUNTERCONTROL:
#ifdef L2M_SUPPORTED
      bsmPerformOperationOnCounterControl(setObj);
#else
      bsmFillAndSendSetObjFail(setObj,BS_ERR_FEATURE_NOT_SUPPORTED,  &setObj->tlvArray.tlvInfo[0].tag);
#endif
      break;
  }
  return;
}

/**
 * @param msg       Received Message.
 * @param setObj    Set Object Request which was Sent by CIH
 * @details This function will process the SET request message on Operator Configuration table.
 */
static void bsmProcSetObjReqOnOperConfigTable(
    IN Msg            *msg,
    IO SetObjReq      *setObj
    )
{
  TableId tableId = setObj->tlvArray.tlvInfo[0].tag.tableId;
  /* NOTE: This row ID is updated assuming that only one row entries will be sent
   * in each SET Request message to modify.
   * For multi Row table, if 'n' number of rows needs modification,
   * 'n' SET messages will be sent by OAM-IF */
  RowId   rowIdToUpdate = setObj->tlvArray.tlvInfo[0].tag.rowId;
  rowIdToUpdate = rowIdToUpdate - 1; /* This is done, because the actual data is stored from index 0 and row Id starts with 1*/
  UINT8  i=0;
  TBOOL  retVal = FALSE;

  if ( FALSE EQ isEnodeBLocked() )
  {
    /* if not locked, check isLockRequired to set the element */
    for ( i=0; i < setObj->tlvArray.numTlvs; i++ )
    {
      retVal = isLockRequired ( &setObj->tlvArray.tlvInfo[i].tag );
      if( retVal EQ TRUE )
      {
        bsmFillAndSendSetObjFail( setObj, BS_ERR_ENODEB_NOT_LOCKED, &setObj->tlvArray.tlvInfo[0].tag );
        return;
      }
    }
  }

  switch( tableId )
  {
    case ENBREGISTRATIONCONFIG:
      retVal = bsmPerformOperationOnEnbRegistrationConfig( rowIdToUpdate, setObj );
      break;
    case ENODEBCONFIG:
      retVal = bsmPerformOperationOnEnbConfig( rowIdToUpdate, setObj );
      break;
    case CELLCONFIG:
      retVal = bsmPerformOperationOnCellConfig( rowIdToUpdate, setObj );
      break;
    case SYSINFO1CONFIG:
      retVal = bsmPerformOperationOnSysInfo1Config( rowIdToUpdate, setObj );
      break;
    case SYSINFO2CONFIG:
      retVal = bsmPerformOperationOnSysInfo2Config( rowIdToUpdate, setObj );
      break;
    case SYSINFO3CONFIG:
      retVal = bsmPerformOperationOnSysInfo3Config( rowIdToUpdate, setObj );
      break;
    case SCHEDULERCONFIG:
      retVal = bsmPerformOperationOnSchedulerConfig( rowIdToUpdate, setObj );
      break;
    case PHYCONFIG:
      retVal = bsmPerformOperationOnPhyConfig( rowIdToUpdate, setObj );
      break;
    case RADIOHEADCONFIG:
      /* Send to DEH . Write to table only when success response is received from DEH */
      createAndSendMsg( MSG_BSM_DEH_SET_OBJ_REQ, msg->data, msg->len );
      return;
    case QOSCONFIG:
      retVal = bsmPerformOperationOnQosConfig( rowIdToUpdate, setObj );
      break;
    case EUTRAHOPROFILECONFIG:
      retVal = bsmPerformOperationOnEutraHoProfileConfig( rowIdToUpdate, setObj );
      break;
    case EUTRANEIGHCELLCONFIG:
      retVal = bsmPerformOperationOnEutraNeighCellConfig( rowIdToUpdate, setObj );
      break;
    case TRACECONFIG:
#ifdef L3MSGTRC_SUPPORTED
      createAndSendMsg( MSG_BSM_LTH_SET_OBJ_REQ, msg->data, msg->len );
#else
      bsmFillAndSendSetObjFail(setObj,BS_ERR_FEATURE_NOT_SUPPORTED,  &setObj->tlvArray.tlvInfo[0].tag);
#endif
      break;
    case PMCOUNTERLOGCONFIG:
      retVal = bsmPerformOperationOnPmConfig( rowIdToUpdate, setObj );
      break;
    case LOGCONFIG:
      retVal = bsmPerformOperationOnLogConfig( rowIdToUpdate, setObj );
      break;
    case MMECONFIG:
      retVal = bsmPerformOperationOnMmeConfig( rowIdToUpdate, setObj );
      break;
    case SONCONFIG:
      retVal = bsmPerformOperationOnSonConfig( rowIdToUpdate, setObj );
      break;
    case SRSCONFIGDEDICATED:
      retVal = bsmPerformOperationOnSrsConfigDedicated( rowIdToUpdate, setObj );
      break;
    case SECURITYCONFIG:
      retVal = bsmPerformOperationOnSecurityConfig( rowIdToUpdate, setObj );
      break;
    case UEEVENTHISTORYLOGCONFIG:
      //retVal = bsmPerformOperationOnUeEventHistoryLogConfig( rowIdToUpdate, setObj );
      bsmFillAndSendSetObjFail(setObj,BS_ERR_FEATURE_NOT_SUPPORTED,  &setObj->tlvArray.tlvInfo[0].tag);
      break;
    case COUNTERCONFIG:
      /* Send to PMH . Write to table only when success response is received from PMH */
#ifdef L2M_SUPPORTED
      createAndSendMsg( MSG_BSM_PMH_SET_OBJ_REQ, msg->data, msg->len );
#else
      bsmFillAndSendSetObjFail(setObj,BS_ERR_FEATURE_NOT_SUPPORTED,  &setObj->tlvArray.tlvInfo[0].tag);
#endif
      break;

  }
  if ( RADIOHEADCONFIG NOTEQ tableId )
  {
    if ( retVal EQ TRUE )
    {
      if ( TRUE EQ bsmWriteEnbUserRunningConfigToPersistentMemory( ))
      {
        informSihAbtTheConfigChange( setObj );
      }
      else
      {
        /* TODO: Replace with BS error */
        logInfo("%s", "Contents of table are NOT written to persistent memory (enb_user_running.cfg)...ERROR !!" );
      }
    }
  }
  return;
}

/**
 * @param setObj       Set Object Request which was Sent by CIH
 * @param bsmContext   BSM context information
 * @details This function will process the SET request message on Advanced Config table.
 */
static void bsmProcSetObjReqOnPreConfigTable(
    IN Msg            *msg,
    IO SetObjReq      *setObj
    )
{
  BsErrorValue    bsErrorValue = BS_NO_ERROR;
  TableId tableId = setObj->tlvArray.tlvInfo[0].tag.tableId;
  UINT8  i=0;
  TBOOL  retVal = FALSE;

  if ( FALSE EQ isEnodeBLocked() )
  {
    /* if not locked, check isLockRequired to set the element */
    for ( i=0; i < setObj->tlvArray.numTlvs; i++ )
    {
      retVal = isLockRequired ( &setObj->tlvArray.tlvInfo[i].tag );
      if( retVal EQ TRUE )
      {
        bsmFillAndSendSetObjFail( setObj, BS_ERR_ENODEB_NOT_LOCKED, &setObj->tlvArray.tlvInfo[0].tag );
        return;
      }
    }
  }

  switch( tableId )
  {
    case MSGTRACE:
      retVal = bsmPerformOperationOnMsgTrace( setObj );
      if ( TRUE EQ retVal )
      {
        informSihAbtTheConfigChange( setObj );
      }
      break;
    case TIMERSCONFIG:   /* SET would not be allowed as of now. - Reject the request */
    case PROCESSSTART:
    case MODULESTOP:
    case UDHCONFIG:
    case UEHCONFIG:
    case EGTPCONFIG:
    case SCTPCONFIG:
    case S1APCONFIG:
      bsErrorValue = BS_ERR_REQ_REJ_SET_TABLE_NOT_ALLOWED;
      bsmFillAndSendSetObjFail( setObj, bsErrorValue, &setObj->tlvArray.tlvInfo[0].tag );
      break;
    case EVENTTHRESHOLDS:
      retVal = bsmPerformOperationOnEventThresholds( setObj );
      if ( TRUE EQ retVal )
      {
        informSihAbtTheConfigChange( setObj );
      }
      break;
    case ENBADDRESSES:
      createAndSendMsg( MSG_BSM_DEH_SET_OBJ_REQ, msg->data, msg->len );
      break;
  }
  return;
}

/**
 * @param msg          Message arrived at BSM.
 * @param bsmContext   BSM context information
 * @details This function will process the SET request message from any OAM-Interface in IDLE state.
 */
BsmProceduralState bsmProcSetObjReq(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  BsErrorValue    bsErrorValue = BS_NO_ERROR;
  TagInfo         tag;
  SetObjReq       setReq;
  clearBytes( &setReq, sizeof(SetObjReq));

  decSetObjReqMsg( (UINT8*)msg->data, msg->len, &setReq );
  tag = setReq.tlvArray.tlvInfo[0].tag;

  logInfo("Set Object Request Received for Table Id : [%d] ; Row Id : [%d] ", tag.tableId, tag.rowId );

  /* Validate TableId */
  if((tag.tableId <MIN_OPR_CMD_TABLEID ) ||(tag.tableId >MAX_PRE_CFG_TABLEID))
  {
    bsErrorValue = BS_ERR_UNKNOWN_TABLE_ID;
    BS_ERROR_UNKNOWN_TABLE_ID( tag.tableId );
    bsmFillAndSendSetObjFail( &setReq, bsErrorValue, &tag );
    return ( bsmContext->bsmProceduralState );
  }

  BsmOamTableInfo *oamTableInfo = bsmGetTablePtrByTableId(tag.tableId);

  switch( oamTableInfo->oamTableType )
  {
    case TABLE_STATUS:
      /* Reject */
      bsErrorValue = BS_ERR_REQ_REJ_SET_TABLE_NOT_ALLOWED;
      bsmFillAndSendSetObjFail( &setReq, bsErrorValue, &tag );
      break;
    case TABLE_PRECONFIG:
      bsmProcSetObjReqOnPreConfigTable( msg, &setReq );
      break;
    case TABLE_OPERCMD:
      bsmProcSetObjReqOnOperCmdTable( msg, &setReq, bsmContext );
      break;
    case TABLE_OPERCONFIG:
      bsmProcSetObjReqOnOperConfigTable( msg, &setReq );
      break;
    case TABLE_UNKNOWNTYPE:
      bsErrorValue = BS_ERR_UNKNOWN_TABLE_TYPE;
      BS_ERROR_UNKNOWN_TABLE_TYPE( TABLE_UNKNOWNTYPE );
      bsmFillAndSendSetObjFail( &setReq, bsErrorValue, NULL );
      break;
  }
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param setReq   Set Object received in TLV format.
 * @details This function will check if lock command was received.
 */
TBOOL isLockCmdRcvd(
    IO SetObjReq   *setReq
    )
{
  BsmOamTableInfo *oamTableInfo = bsmGetTablePtrByTableId(setReq->tlvArray.tlvInfo[0].tag.tableId);

  switch( oamTableInfo->oamTableType )
  {
    case TABLE_OPERCMD:
      {
        if ( setReq->tlvArray.tlvInfo[0].tag.tableId EQ ENBADMIN )
        {
          EnbAdmin  enbAdmin;
          clearBytes( &enbAdmin, sizeof(EnbAdmin) );
          GenericTable     *tableFunc = getTableFunctions(ENBADMIN);
          if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
          {
            tableFunc->getTlv2StructFunc( &setReq->tlvArray, &enbAdmin );
          }
          if ( ENBCOMMAND_LOCKENB EQ enbAdmin.eNbCommand )
          {
            return TRUE;
          }
        }
      }
      break;
    default:
      break;
  }
  return FALSE;
}

/**
 * @param   msg          Message arrived at BSM.
 * @param   bsmContext   BSM context information
 * @details This function will process the SET request message from any OAM-Interface
 *          when not in IDLE state.
 */
BsmProceduralState bsmProcSetObjReqWhenBusy(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  BsErrorValue    bsErrorValue = BS_NO_ERROR;
  SetObjReq       setReq;
  clearBytes( &setReq, sizeof(SetObjReq));

  decSetObjReqMsg( (UINT8*)msg->data, msg->len, &setReq );

  switch( bsmContext->bsmProceduralState )
  {
    case BS_WAIT_INIT_CMPL:
      bsErrorValue = BS_ERR_REQ_REJ_INIT_IN_PROGRESS;
      BS_ERROR_REQ_REJ_INIT_IN_PROGRESS();
      break;

    case BS_WAIT_INITIAL_CONFIG_CMPL:
      bsErrorValue = BS_ERR_REQ_REJ_INITIAL_CONFIG_IN_PROGRESS;
      BS_ERROR_REQ_REJ_INITIAL_CONFIG_IN_PROGRESS();
      break;

    case BS_WAIT_COLLECT_INFO_CMPL:
      bsErrorValue = BS_ERR_REQ_REJ_COLLECT_INFO_IN_PROGRESS;
      BS_ERROR_REQ_REJ_COLLECT_INFO_IN_PROGRESS();
      break;

    case BS_WAIT_REBOOT_CMPL:
      bsErrorValue = BS_ERR_REQ_REJ_REBOOT_IN_PROGRESS;
      BS_ERROR_REQ_REJ_REBOOT_IN_PROGRESS();
      break;

    case ENODEB_WAIT_RESTART_CMPL:
      bsErrorValue = BS_ERR_REQ_REJ_RESTART_IN_PROGRESS;
      BS_ERROR_REQ_REJ_RESTART_IN_PROGRESS();
      break;

    case ENODEB_WAIT_CONFIG_CMPL:
      /* Even if config is in progress, it is sufficient for user
       * to know that unlock is in progress.
       * */
      bsErrorValue = BS_ERR_REQ_REJ_UNLOCK_IN_PROGRESS;
      BS_ERROR_REQ_REJ_UNLOCK_IN_PROGRESS();
      break;

    case ENODEB_WAIT_ENABLE_CMPL:
      {
        if ( TRUE EQ isLockCmdRcvd( &setReq ) )
        {
          if (( bsmContext->enbEnableReason EQ ENABLE_REASON_S1AP_RECOVERY ) || (bsmContext->enbEnableReason EQ ENABLE_REASON_COMPONENT_ALARM_CLEARED))
          {
            bsmFillAndSendSetObjSucc( &setReq );
            bsmDoLock( &setReq, bsmContext );
            return ( bsmContext->bsmProceduralState );
          }
          else if ( bsmContext->enbEnableReason EQ ENABLE_REASON_USER_INITAITED_UNLOCK )
          {
            bsErrorValue = BS_ERR_REQ_REJ_UNLOCK_IN_PROGRESS;
            BS_ERROR_REQ_REJ_UNLOCK_IN_PROGRESS();
          }
        }
        else
        {
          bsErrorValue = BS_ERR_REQ_REJ_UNLOCK_IN_PROGRESS;
          BS_ERROR_REQ_REJ_UNLOCK_IN_PROGRESS();
        }
      }
      break;
    case ENODEB_WAIT_DISABLE_CMPL:
      {
        if ( TRUE EQ isLockCmdRcvd( &setReq ) )
        {
          bsmFillAndSendSetObjSucc( &setReq );
          bsmDoLock( &setReq, bsmContext );
          return ( bsmContext->bsmProceduralState );
        }
        else if ( bsmContext->enbDisableReason EQ USER_INITAITED_LOCK )
        {
          bsErrorValue = BS_ERR_REQ_REJ_LOCK_IN_PROGRESS;
          BS_ERROR_REQ_REJ_LOCK_IN_PROGRESS();
        }
        else if ( bsmContext->enbDisableReason EQ MOD_DOWN_OCCURED )
        {
          bsErrorValue = BS_ERR_REQ_REJ_MOD_DOWN_DISABLE_IN_PROGRESS;
          BS_ERROR_REQ_REJ_MOD_DOWN_DISABLE_IN_PROGRESS();
        }
        else if ( bsmContext->enbDisableReason EQ S1AP_LINK_DOWN )
        {
          bsErrorValue = BS_ERR_REQ_REJ_LINK_DOWN_DISABLE_IN_PROGRESS;
          BS_ERROR_REQ_REJ_LINK_DOWN_DISABLE_IN_PROGRESS();
        }
      }
      break;
    case WAIT_SW_ACTIVATE_CMPL:
      bsErrorValue = BS_ERR_REQ_REJ_SW_ACTIVATION_IN_PROGRESS;
      BS_ERROR_REQ_REJ_SW_ACTIVATION_IN_PROGRESS();
      break;
    case BS_WAIT_DEVICE_INIT_CMPL:
      bsErrorValue = BS_ERR_DEVICE_INIT_IN_PROGRESS;
      BS_ERR_DEVICE_INIT_IN_PROGRESS();
      break;
    default:
      bsErrorValue = BS_ERR_MSG_RCVD_IN_UNKNOWN_STATE;
      BS_ERROR_MSG_RCVD_IN_UNKNOWN_STATE( msg->msgId, getBsmProceduralStateName(bsmContext->bsmProceduralState) );
      break;
  }
  /* Reject */
  bsmFillAndSendSetObjFail( &setReq, bsErrorValue, &setReq.tlvArray.tlvInfo[0].tag );

  return ( bsmContext->bsmProceduralState );
}


/**
 * @param   msg          Message arrived at BSM.
 * @param   bsmContext   BSM context information
 * @details This function will process the SET request message received at BSM,
 *          decodes the message, and writes into the config file.
 *          Success Message is then sent to the OAM-IF Module.
 */
BsmProceduralState bsmProcSetRspSucc(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  SetObjReq       setReq;
  clearBytes( &setReq, sizeof(SetObjReq));

  decSetObjReqMsg( (UINT8*)msg->data, msg->len, &setReq );
  TableId tableId = setReq.tlvArray.tlvInfo[0].tag.tableId;

  GenericTable  *tableFunc = getTableFunctions(tableId);

  switch( tableId )
  {
    case RADIOHEADCONFIG:
      {
        RadioHeadConfig  *config = (RadioHeadConfig*)getFirstRowOfTable(RADIOHEADCONFIG);
        if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
        {
          tableFunc->getTlv2StructFunc( &setReq.tlvArray, config );
        }
        else
        {
          BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "RADIOHEADCONFIG" );
          bsmFillAndSendSetObjFail( &setReq, BS_ERR_INTERNAL_ERROR_OCCURED, &setReq.tlvArray.tlvInfo[0].tag );
          return ( bsmContext->bsmProceduralState );
        }
        if ( TRUE EQ bsmWriteEnbUserRunningConfigToPersistentMemory( ))
        {
          informSihAbtTheConfigChange( &setReq );
        }
        else
        {
          logInfo("%s", "Contents of table are NOT written to persistent memory (enb_user_running.cfg)...ERROR !!" );
        }
        bsmFillAndSendSetObjSucc( &setReq );
      }
      break;
    case ENBADDRESSES:
      {
        EnbAddresses  *config = tableFunc->getData(FIRST_ROW);
        if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
        {
          tableFunc->getTlv2StructFunc( &setReq.tlvArray, config );
        }
        else
        {
          BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "ENBADDRESSES" );
          bsmFillAndSendSetObjFail( &setReq, BS_ERR_INTERNAL_ERROR_OCCURED, &setReq.tlvArray.tlvInfo[0].tag );
          return ( bsmContext->bsmProceduralState );
        }
        if ( TRUE EQ bsmWriteEnbUserRunningConfigToPersistentMemory( ))
        {
          informSihAbtTheConfigChange( &setReq );
        }
        else
        {
          logError("%s", "Contents of table are NOT written to persistent memory (enb_user_running.cfg)...ERROR !!" );
        }
        bsmFillAndSendSetObjSucc( &setReq );
      }
      break;
    case ENBADMIN:
      {
        EnbAdmin  enbAdmin;
        clearBytes( &enbAdmin, sizeof(EnbAdmin) );
        GenericTable     *tableFunc = getTableFunctions(ENBADMIN);
        if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
        {
          tableFunc->getTlv2StructFunc( &setReq.tlvArray, &enbAdmin );
        }
        switch( enbAdmin.eNbCommand )
        {
          case ENBCOMMAND_RESETRADIOHEAD:
          case ENBCOMMAND_RESETGPS:
          case ENBCOMMAND_RUNBUILTINSELFTEST:
          case ENBCOMMAND_SETLONGPOST:
          case ENBCOMMAND_DELETEFILEEVENTHISTORY:
          case ENBCOMMAND_DELETESWINSTALLHISTORY:
            bsmFillAndSendSetObjSucc( &setReq );
            break;
          default :
            BS_ERROR_UNKNOWN_ACTION_RECD( enbAdmin.eNbCommand );
            break;
        }
      }
      break;
    case COUNTERCONFIG:
      {
        CounterConfig  *config = (CounterConfig*)getRowOfTable(COUNTERCONFIG,setReq.tlvArray.tlvInfo[0].tag.rowId-1);
        if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
        {
          tableFunc->getTlv2StructFunc( &setReq.tlvArray, config );
        }
        else
        {
          BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "COUNTERCONFIG" );
          bsmFillAndSendSetObjFail( &setReq, BS_ERR_INTERNAL_ERROR_OCCURED, &setReq.tlvArray.tlvInfo[0].tag );
          return ( bsmContext->bsmProceduralState );
        }
        if ( TRUE EQ bsmWriteEnbUserRunningConfigToPersistentMemory( ))
        {
          informSihAbtTheConfigChange( &setReq );
        }
        else
        {
          logInfo("%s", "Contents of table are NOT written to persistent memory (enb_user_running.cfg)...ERROR !!" );
        }
        bsmFillAndSendSetObjSucc( &setReq );
      }
      break;
    case TRACECONFIG:
      {
        TraceConfig  *traceConfig = (TraceConfig*)getRowOfTable(TRACECONFIG,setReq.tlvArray.tlvInfo[0].tag.rowId-1);
        if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
        {
          tableFunc->getTlv2StructFunc( &setReq.tlvArray, traceConfig );
        }
        else
        {
          BS_ERROR_NO_TLV2STRUCT_FUNCTION_DEFINED( "TRACECONFIG" );
          bsmFillAndSendSetObjFail( &setReq, BS_ERR_INTERNAL_ERROR_OCCURED, &setReq.tlvArray.tlvInfo[0].tag );
          return ( bsmContext->bsmProceduralState );
        }
        if ( TRUE EQ bsmWriteEnbUserRunningConfigToPersistentMemory( ))
        {
          informSihAbtTheConfigChange( &setReq );
        }
        else
        {
          logInfo("%s", "Contents of table are NOT written to persistent memory (enb_user_running.cfg)...ERROR !!" );
        }
        bsmFillAndSendSetObjSucc( &setReq );
      }
      break;
    case FILEDOWNLOAD:
    case FILEUPLOAD:
    case SWINSTALL:
    case COUNTERCONTROL:
    case TRACECONTROL:
      bsmFillAndSendSetObjSucc( &setReq );
      break;
  }
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param   msg          Message arrived at BSM
 * @param   bsmContext   BSM context information
 * @details This function will process the SET request message received at BSM,
 *          decodes the message. Decoding id done to get the command reference information.
 *          Failure Message is then sent to the OAM-IF Module.
 */
BsmProceduralState bsmProcSetRspFail(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  SetObjRspFail     setObjFail;
  clearBytes(&setObjFail, sizeof(SetObjRspFail));

  /* Decoding of the received message is required to know the origin of the Requested message
   * Response is sent back to the OAM-IF */
  decSetObjRspFailMsg((UINT8*)msg->data, msg->len, &setObjFail );
  bsmCreateAndSendSetObjFail(&setObjFail);
  return bsmContext->bsmProceduralState;
}

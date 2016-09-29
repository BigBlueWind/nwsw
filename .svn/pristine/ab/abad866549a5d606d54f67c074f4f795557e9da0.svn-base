/**
 * @file    bsm_cmd_get.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file has functions which handles all the get messages from user interfaces (CIH, SIH, WIH)
 *
 * @author:  Shweta Polepally
 * @date:    August 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**                                                                                                                                                  
 *@param    instanceId Indicates the instanceid of the Cih 
 *@details             This function is used get MsgId for Get Object Success for each instance
 */

MsgId bsmCihGetObjSuccMsgId(
    IN InstanceId instanceId
    )
{
  MsgId msgId;
  switch(instanceId)
  {
    case 1:
      msgId = MSG_BSM_CIH1_GET_OBJ_SUCC;
      break;
    case 2:
      msgId = MSG_BSM_CIH2_GET_OBJ_SUCC;
      break;
    case 3:
      msgId = MSG_BSM_CIH3_GET_OBJ_SUCC;
      break;
    default:
      msgId = MSG_UNDEFINED;
      break;
  }

  return msgId;
}

/**                                                                                                                                                  
 *@param    instanceId Indicates the instanceid of the Cih 
 *@details            This function is used get MsgId for Get Object Fail for each instance
 */
MsgId bsmCihGetObjFailMsgId(
    IN InstanceId instanceId
    )
{
  MsgId msgId;
  switch(instanceId)
  {
    case 1:
      msgId = MSG_BSM_CIH1_GET_OBJ_FAIL;
      break;
    case 2:
      msgId = MSG_BSM_CIH2_GET_OBJ_FAIL;
      break;
    case 3:
      msgId = MSG_BSM_CIH3_GET_OBJ_FAIL;
      break;
    default:
      msgId = MSG_UNDEFINED;break;
  }

  return msgId;
}

/**
 * @param   getObj  GET Object Request which was Sent by OAM Interface. (CIH/WIH/SIH)
 * @param   encBuf  Encoded buffer which is already filled with the information to be sent.
 * @details This function will send MSG_BSM_<OAM-IF>_SET_OBJ_SUCC to CIH/WIH/SIH.
 */
static void bsmSendGetObjRspSucc( 
    IO GetObjReq *getObj, 
    IO EncBuf *encBuf
    ) 
{
  MsgId msgId;
  switch (getObj->cmdRef.origin) 
  {
    case OAMIF_CIH: msgId = bsmCihGetObjSuccMsgId(getObj->cmdRef.instanceId); break;
    case OAMIF_WIH: msgId = MSG_BSM_WIH_GET_OBJ_SUCC; break;
    case OAMIF_SIH: msgId = MSG_BSM_SIH_GET_OBJ_SUCC; break;
    default:
         logError("Unknown OAMIF=%d", getObj->cmdRef.origin); 
         break;

  }
  createAndSendMsg(msgId, (UINT8*) encBuf->value, encBuf->len);  
}

/**
 * @param   msg        Message received from some module.
 * @param   bsmContext BSM context information.
 * @details This function relays the response received by some module to OAM interface
 *          by sending MSG_BSM_<OAM-IF>_SET_OBJ_SUCC message to CIH/WIH/SIH.
 **/
BsmProceduralState bsmProcGetRspSucc(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  MsgId            msgId;
  GetObjRspSucc    rspSucc;

  clearBytes( &rspSucc, sizeof(GetObjRspSucc));
  decGetObjRspSuccMsg( (UINT8*)msg->data, msg->len, &rspSucc );

  switch (rspSucc.cmdRef.origin) 
  {
    case OAMIF_CIH: msgId = bsmCihGetObjSuccMsgId(rspSucc.cmdRef.instanceId); break;
    case OAMIF_WIH: msgId = MSG_BSM_WIH_GET_OBJ_SUCC; break;
    case OAMIF_SIH: msgId = MSG_BSM_SIH_GET_OBJ_SUCC; break;
    default:
         logError("Unknown OAMIF=%d", rspSucc.cmdRef.origin); 
         break;
  }
  createAndSendMsg(msgId, (UINT8*) msg->data, msg->len);
  return (bsmContext->bsmProceduralState);
}

/**
 * @param   msg        Message received from some module.
 * @param   bsmContext BSM context information.
 * @details This function relays the response received by some module to OAM interface
 *          by sending MSG_BSM_<OAM-IF>_SET_OBJ_FAIL message to CIH/WIH/SIH.
 **/
BsmProceduralState bsmProcGetRspFail(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  MsgId            msgId;
  GetObjRspFail    rspFail;

  clearBytes( &rspFail, sizeof(GetObjRspFail));
  decGetObjRspFailMsg( (UINT8*)msg->data, msg->len, &rspFail );

  switch (rspFail.cmdRef.origin) 
  {
    case OAMIF_CIH: msgId = bsmCihGetObjFailMsgId(rspFail.cmdRef.instanceId); break;
    case OAMIF_WIH: msgId = MSG_BSM_WIH_GET_OBJ_FAIL; break;
    case OAMIF_SIH: msgId = MSG_BSM_SIH_GET_OBJ_FAIL; break;
    default:
         logError("Unknown OAMIF=%d", rspFail.cmdRef.origin);
         break;
  }
  createAndSendMsg(msgId, (UINT8*) msg->data, msg->len);
  return (bsmContext->bsmProceduralState);
}

/**
 * @param getReq       Get Object Request which was Sent by OAM-IF
 * @param bsErrorValue BS Error Value - Reason why the Request was rejected.
 * @param errTagInfo   Tag Information which failed.
 * @details This function will fill and send MSG_BSM_<OAM-IF>_GET_OBJ_FAIL to CIH/WIH/SIH module.
 */
void bsmFillAndSendGetObjFail(
    IN GetObjReq          *getReq,
    IN BsErrorValue        bsErrorValue,
    IN TagInfo            *errTagInfo
    )
{
  GetObjRspFail   rspFail;
  EncBuf          encBuf;
  MsgId           msgId;
  initEncBuf( &encBuf );
  clearBytes(&rspFail, sizeof(GetObjRspFail));

  copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspFail.cmdRef));
  copyBytes( &(getReq->tagArray), sizeof(TagArray), &(rspFail.tagArray));
  rspFail.getErrRsp.errRspCode = bsErrorValue;
  if ( errTagInfo NOTEQ NULL )
  {
    copyBytes( errTagInfo, sizeof(TagInfo), &(rspFail.getErrRsp.failedTagInfo));
  }
  encGetObjRspFailMsg( &rspFail, &encBuf );

  switch (getReq->cmdRef.origin) 
  {
    case OAMIF_CIH: msgId = bsmCihGetObjFailMsgId(getReq->cmdRef.instanceId); break;
    case OAMIF_WIH: msgId = MSG_BSM_WIH_GET_OBJ_FAIL; break;
    case OAMIF_SIH: msgId = MSG_BSM_SIH_GET_OBJ_FAIL; break;
    default:
         logError("Unknown OAMIF=%d", getReq->cmdRef.origin);
         break;
  }
  createAndSendMsg(msgId, (UINT8*) &encBuf.value, encBuf.len);
}

/**
 * @details This function will clear Active Status table entries.
 **/
static void clearActiveAlarmStatusTable()
{
  GenericTable  *tableFunc = getTableFunctions(ACTIVEALARMSTATUS);
  if( NULL NOTEQ tableFunc->getEmptyTableFunc )
  {
    tableFunc->getEmptyTableFunc();
  }
}

/**
 * @details This function will clear Alarm History table entries.
 **/
static void clearAlarmHistoryStatusTable()
{
  GenericTable  *tableFunc = getTableFunctions(ALARMHISTORY);
  if( NULL NOTEQ tableFunc->getEmptyTableFunc )
  {
    tableFunc->getEmptyTableFunc();
  }
}

/**
 * @param bsmContext    BSM context information
 * @details This function will calculate the uptime of the process and
 *          update the EnodeB status table.
 **/
static void updateEnodeBStatusTable(
    IN BsmContext       *bsmContext
    )
{
  EnodebConfig   *enodebConfig   = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
  EnbStatus      *enbStatus = (EnbStatus*)getFirstRowOfTable(ENBSTATUS);

  /* Update if it is connected to MME or not */
  if ( TRUE EQ bsmContext->enbEnableDisableParams.isS1SetupSuccessful )
    enbStatus->connectedToMME = CONNECTEDTOMME_CONNECTED;
  else
    enbStatus->connectedToMME = CONNECTEDTOMME_NOTCONNECTED;

  enbStatus->enodebId                = enodebConfig->enodebId;
  enbStatus->numActiveCriticalAlarms = getNumActiveCriticalAlarms();
  enbStatus->numActiveMajorAlarms    = getNumActiveMajorAlarms();
  enbStatus->uptime                  = (UINT32)getUpTime( bsmContext->moduleStartTime ) + (UINT32)( bsmContext->moduleUpTimeDifference );

  /*  Update localTime Format : YYYYMMDDHHNNSSTTT */
  getAndFillDateAndTimeStamp( MAX_SIZE_ENBSTATUS_LOCALTIME, enbStatus->localTime );
  snprintf( enbStatus->localTimeOffsetFromUTC, strlen(bsmContext->currTimeOffsetFromUTC)+1, "%s", bsmContext->currTimeOffsetFromUTC );
  enbStatus->dateAndTimeSource = bsmContext->dateAndTimeSource;
}

/**
 * @param   numOfElements Number of alarm elements.
 * @param   getReq        GET Object request received.
 * @details This function will send the Active/History alarm table.
 **/
static void sendAlarmTable(
    IN UINT32       numOfElements,
    IO GetObjReq   *getReq
    )
{
  RowId           rowId=0;
  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  TagInfo        *tagInfo = &getReq->tagArray.tagInfo[0]; /* One Get = One Req message */

  initEncBuf(&encBuf);
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  /* Fill CMD REF details */
  copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

  /* If entire table needs to be send, handling is done in a different way,
   * else the procedure remains same */
  GenericTable  *tableFunc = getTableFunctions(tagInfo->tableId);
  GetCmdType     cmdType;
  cmdType      = getCmdType(tagInfo);

  if ( numOfElements NOTEQ 0 )
  {
    switch( cmdType )
    {
      case GET_TABLE:
        {
          for ( rowId=1; rowId <= numOfElements; rowId++ )
          {
            if ( tagInfo->tableId EQ ACTIVEALARMSTATUS )
              getRowActiveAlarmStatusTable( tagInfo->tableId, rowId, &rspTlv.tlvArray );
            else if ( tagInfo->tableId EQ ALARMHISTORY )
              getRowAlarmHistoryTable( tagInfo->tableId, rowId, &rspTlv.tlvArray );
          }
        }
        break;
      case GET_ROW:
        tableFunc->getRowFunc( tagInfo->tableId, tagInfo->rowId, &rspTlv.tlvArray );
        break;
      case GET_ELEMENT:
        tableFunc->getElementFunc( tagInfo->tableId, tagInfo->rowId, tagInfo->colId, &rspTlv.tlvArray );
        break;
      default:
        /* Error Condition */
        break;
    }
  }
  else
  {
    /* Fill One row atleast with values 0 . This is needed for SIH */
    if ( tagInfo->tableId EQ ACTIVEALARMSTATUS )
    {
      /* Fill 1st entry with default values. This is required only for display purpose at CIH and logging purpose */
      ActiveAlarmStatus         *activeAlarmStatus = (ActiveAlarmStatus*)getFirstRowOfTable(ACTIVEALARMSTATUS);
      snprintf(activeAlarmStatus->alarmDateAndTimeStamp, MAX_SIZE_ACTIVEALARMSTATUS_ALARMDATEANDTIMESTAMP +1, "%s","00000000000000000");
      snprintf(activeAlarmStatus->alarmLocation,         MAX_SIZE_ACTIVEALARMSTATUS_ALARMLOCATION +1,         "%s","None");
      snprintf(activeAlarmStatus->alarmAdditionalText,   MAX_SIZE_ACTIVEALARMSTATUS_ALARMADDITIONALTEXT +1,   "%s","None");
      getRowActiveAlarmStatusTable( tagInfo->tableId, 1, &rspTlv.tlvArray ); /* 1 = rowId*/
    }
    else if ( tagInfo->tableId EQ ALARMHISTORY )
    {
      /* Fill 1st entry with default values. This is required only for display purpose at CIH and logging purpose */
      AlarmHistory         *alarmHistory = (AlarmHistory*)getFirstRowOfTable(ALARMHISTORY);
      snprintf(alarmHistory->alarmDateAndTimeStamp, MAX_SIZE_ALARMHISTORY_ALARMDATEANDTIMESTAMP +1, "%s","00000000000000000");
      snprintf(alarmHistory->alarmLocation,         MAX_SIZE_ALARMHISTORY_ALARMLOCATION +1,         "%s","None");
      snprintf(alarmHistory->alarmAdditionalText,   MAX_SIZE_ALARMHISTORY_ALARMADDITIONALTEXT +1,   "%s","None");
      getRowAlarmHistoryTable( tagInfo->tableId, 1, &rspTlv.tlvArray );
    }
  }
  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );
  /* Send */
  bsmSendGetObjRspSucc( getReq, &encBuf );
}

/**
 * @param   getReq  GET Object request received.
 * @details This function will update active alarm table will current entries and post it to OAM-IF.
 **/
static void updateAndSendActiveAlarmStatus(
    IO GetObjReq   *getReq
    )
{
  ActiveAlarmStatusTable    *list       = getActiveAlarmStatusTable();
  ActiveAlarmStatus         *rowOfTable = (ActiveAlarmStatus*)getFirstRowOfTable(ACTIVEALARMSTATUS);
  ActiveAlarmStatus         *entry = NULL;
  ListNode         *node;
  ListNode         *head = NULL;
  ListNode         *end  = NULL;
  UINT32            numOfElements = list->numElemPresent;
  UINT32            indexCount = 0;

  /* Empty Active alarm table and fill the updated values */
  clearActiveAlarmStatusTable();

  /* Reverse the list so that latest requested entries can be sent */
  head = list->first;
  end  = NULL;

  /* Print the entire table posted to BSM */
  if (head EQ NULL)
  {
    logInfo("%s","No Contents in this table - TABLE EMPTY \n");
  }
  rowOfTable->index = 0; /* Reset the index counter */
  while(head NOTEQ end)
  {
    node = head;
    while(node->next NOTEQ end)
      node = node->next;

    entry = (ActiveAlarmStatus*)node->data;
    copyBytes( entry, sizeof(ActiveAlarmStatus), rowOfTable );
    indexCount++;
    rowOfTable->index = indexCount;
    getNextRowOfTable(rowOfTable);
    end = node;
    numOfElements--;
    if ( numOfElements EQ 0 )
      break;
  }
  /* Sending table to OAM-IF module */
  sendAlarmTable( list->numElemPresent, getReq );
}

/**
 * @param   getReq GET Object request received.
 * @details This function will send MOD_STATE_REQ to UDH and UEH.
 **/
static void bsmCreateAndSendModStateReqToUehAndUdh(
    IO GetObjReq  *getObjReq,
    IO BsmContext *bsmContext
    )
{
  TBOOL                 isUehDetailNeeded = FALSE;
  TBOOL                 isUdhDetailNeeded = FALSE;
  MsgBsmUehModStateReq  uehModStateReq;
  MsgBsmUdhModStateReq  udhModStateReq;
  clearBytes( &uehModStateReq, sizeof(MsgBsmUehModStateReq) );
  clearBytes( &udhModStateReq, sizeof(MsgBsmUdhModStateReq) );

  uehModStateReq.cmdRef   = getObjReq->cmdRef;
  uehModStateReq.tagInfo  = getObjReq->tagArray.tagInfo[0];
  udhModStateReq.cmdRef   = getObjReq->cmdRef;
  udhModStateReq.tagInfo  = getObjReq->tagArray.tagInfo[0];

  /* Check if GET_OBJ_REQ is done single element(for single element colId will not 0) */
  if(0 NOTEQ getObjReq->tagArray.tagInfo[0].colId)
  {
    /* Check whether detail is required from UEH or UDH or BSM can fill the detail */
    switch( getObjReq->tagArray.tagInfo[0].colId )
    {
      case COL_ENBSTATUS_NUMOFUESCONNECTED:         isUehDetailNeeded = TRUE; break;
      case COL_ENBSTATUS_LOADSTATE:                 isUdhDetailNeeded = TRUE; break;
      case COL_ENBSTATUS_AGGREGATEDLTHROUGHPUTKBPS: isUdhDetailNeeded = TRUE; break;
      case COL_ENBSTATUS_AGGREGATEULTHROUGHPUTKBPS: isUdhDetailNeeded = TRUE; break;
    }
  }
  else
  {
    isUehDetailNeeded = TRUE;
    isUdhDetailNeeded = TRUE;
  }
  bsmContext->isModStateReqSentToUeh = FALSE;
  bsmContext->isModStateReqSentToUdh = FALSE;

  if((TRUE EQ isUehDetailNeeded) && (TRUE EQ isUdhDetailNeeded))
  {
    createAndSendMsg(MSG_BSM_UEH_MOD_STATE_REQ, (UINT8*)&uehModStateReq, sizeof(MsgBsmUehModStateReq));
    createAndSendMsg(MSG_BSM_UDH_MOD_STATE_REQ, (UINT8*)&udhModStateReq, sizeof(MsgBsmUdhModStateReq));

    bsmContext->isModStateReqSentToUeh = TRUE;
    bsmContext->isModStateReqSentToUdh = TRUE;
  }
  else if((TRUE EQ isUehDetailNeeded) && (FALSE EQ isUdhDetailNeeded))
  {
    createAndSendMsg(MSG_BSM_UEH_MOD_STATE_REQ, (UINT8*)&uehModStateReq, sizeof(MsgBsmUehModStateReq));
    bsmContext->isModStateReqSentToUeh = TRUE;
  }
  else if((FALSE EQ isUehDetailNeeded) && (TRUE EQ isUdhDetailNeeded))
  {
    createAndSendMsg(MSG_BSM_UDH_MOD_STATE_REQ, (UINT8*)&udhModStateReq, sizeof(MsgBsmUdhModStateReq));
    bsmContext->isModStateReqSentToUdh = TRUE;
  }
  else
  {
    updateEnodeBStatusTable( bsmContext );
    getAndSendTableData(getObjReq);
  }
}
/**
 * @param   getReq GET Object request received.
 * @details This function will update Alarm History table will current entries and post it to OAM-IF.
 **/
static void updateAndSendAlarmHistoryStatus(
    IO GetObjReq   *getReq
    )
{
  AlarmHistoryStatusTable   *circularBuf = getAlarmHistoryStatusTable();
  AlarmHistory              *rowOfTable = (AlarmHistory*)getFirstRowOfTable(ALARMHISTORY);
  SINT32    i = 0;
  UINT32    j = 0;
  UINT32    numAlarms  =  circularBuf->numOfElem;
  UINT32    indexCount = 0;

  /* Empty alarm history table and fill the updated values */
  clearAlarmHistoryStatusTable();
  rowOfTable->index = 0; /* Reset the index counter */
  if ( numAlarms > 0 )
  {
    if ( circularBuf->isTableFull EQ FALSE )
    {
      for ( j=0; j < numAlarms; j++ )
      {
        copyBytes(  &circularBuf->alarmHistoryEntry[j], sizeof(AlarmHistory), rowOfTable );
        indexCount++;
        rowOfTable->index = indexCount;
        getNextRowOfTable( rowOfTable );
      }
    }
    else  /* If table is already full */
    {
      for ( i = circularBuf->front; i < MAX_ROWS_IN_ALARMHISTORY_TABLE; i++ )
      {
        copyBytes(  &circularBuf->alarmHistoryEntry[i], sizeof(AlarmHistory), rowOfTable );
        indexCount++;
        rowOfTable->index = indexCount;
        getNextRowOfTable( rowOfTable );
      }
      for ( i=0; i <= circularBuf->rear; i++ )
      {
        copyBytes(  &circularBuf->alarmHistoryEntry[i], sizeof(AlarmHistory), rowOfTable );
        indexCount++;
        rowOfTable->index = indexCount;
        getNextRowOfTable( rowOfTable );
      }
    }
  }

  /* Sending table to OAM-IF module */
  sendAlarmTable( circularBuf->numOfElem, getReq );
}

/**
 * @param   tagInfo  TagInfo of the GetObjReq message
 * @details This function will check that if single row or single element GET_OBJ_REQ
 *          is done on history table Then it will return FALSE else TRUE
 **/
static TBOOL isGetAllowedForSingleRowOrParameter(
    IO TagInfo        *tagInfo,
    IO BsErrorValue   *bsErr
    )
{
  switch( tagInfo->tableId )
  {
    case FILEEVENTHISTORY:
    case SWINSTALLHISTORY:
    case ACTIVEUESTATUS:
    case ACTIVERABSTATUS:
    case RABHISTORY:
    case UEEVENTHISTORY:
    case ACTIVEALARMSTATUS:
    case ALARMHISTORY:
    case PMLOGFILESTATUS:
    case ETHERNETCOUNTERS:
    case IPCOUNTERS:
      {
        if(0 NOTEQ tagInfo->colId)
        {
          BS_ERROR_GET_ON_SINGLE_PARAMETER_UNSUPPORTED(tagInfo->tableId);
          *bsErr = BS_ERR_GET_ON_SINGLE_PARAMETER_UNSUPPORTED;
          return FALSE;
        }
        if(0 NOTEQ tagInfo->rowId)
        {
          BS_ERROR_GET_ON_SINGLE_ROW_UNSUPPORTED(tagInfo->tableId);
          *bsErr = BS_ERR_GET_ON_SINGLE_ROW_UNSUPPORTED;
          return FALSE;
        }
        return TRUE;
      }
    default:
      return TRUE;
  }
}

/**
 * @param   getReq  Get Object Request message
 * @details This function will get the function pointer for getStruct2TlvFunc function of
 *          the respective Table, encode the information and send to the OAM-IF.
 **/
void getAndSendTableData(
    IO GetObjReq   *getReq
    )
{
  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  TagInfo        *tagInfo = &getReq->tagArray.tagInfo[0]; /* One Get = One Req message */

  initEncBuf(&encBuf);
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  GenericTable  *tableFunc = getTableFunctions(tagInfo->tableId);

  /* Calculate reference signal power if eNB is LOCKED */
  if( ( PHYCONFIG EQ tagInfo->tableId ) && ( TRUE EQ isEnodeBLocked() ) )
  {
    PhyConfig      *config = (PhyConfig*)getFirstRowOfTable(PHYCONFIG);

    bsmCalculateRefSigPower(config);
  }

  /* Fill CMD REF details */
  copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

  /* Fill Structure */
  if( NULL NOTEQ tableFunc->getStruct2TlvFunc )
  {
    tableFunc->getStruct2TlvFunc( tagInfo, &rspTlv.tlvArray );
  }
  else
  {
    BS_ERROR_NO_STRUCT2TLV_FUNCTION_DEFINED( tagInfo->tableId );  /* TODO: Get table name here in string */
  }
  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );
  /* Send */
  bsmSendGetObjRspSucc( getReq, &encBuf );
}

/**
 * @param   msg         GET Object Request which was Sent by OAM Interface. (CIH/WIH/SIH)
 * @param   bsmContext  BSM context.
 * @details This function will fill the status message or relay the request message to the
 *           module to get the status/config table.
 */
BsmProceduralState bsmProcGetObjReqInAllStates(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  GetObjReq       getObj;
  TableId         tableId;
  BsErrorValue    bsErr;
  clearBytes( &getObj, sizeof(GetObjReq));

  decGetObjReqMsg( (UINT8*)msg->data, msg->len, &getObj );

  /* Only one TLV for get command, hence take from index 0 */
  tableId = getObj.tagArray.tagInfo[0].tableId;

  logInfo("Get Object Request Received for Table Id : [%d] ; Row Id : [%d] ;Col Id : [%d]",\
      tableId, getObj.tagArray.tagInfo[0].rowId, getObj.tagArray.tagInfo[0].colId);

  /* 
     Checking whether single-row or single-column value based retrieval is allowed or not,
     if row or column based tagInfo are supplied in the GET request.
   */
  if((0 NOTEQ getObj.tagArray.tagInfo[0].colId) || (0 NOTEQ getObj.tagArray.tagInfo[0].rowId))
  {
    if(FALSE EQ isGetAllowedForSingleRowOrParameter(&(getObj.tagArray.tagInfo[0]), &bsErr))
    {
      bsmFillAndSendGetObjFail(&getObj, bsErr, &(getObj.tagArray.tagInfo[0]));
      return ( bsmContext->bsmProceduralState );
    }
  }

  /* Process the GET request based on the table ID */
  switch( tableId )
  {
    /* Get on Operator command tables
     * GET on OPR_CMD table are allowed because of SIH
     * SIH does GET before any SET */
    case ENBADMIN:
    case FILEDOWNLOAD:
    case FILEUPLOAD:
    case SWACTIVATION:
    case SWINSTALL:
    case TRACECONTROL:
    case CELLADMIN:
    case COUNTERCONTROL:
    /* Get on Operator Configuration tables */
    case ENBREGISTRATIONCONFIG:
    case ENODEBCONFIG:
    case CELLCONFIG:
    case SYSINFO1CONFIG:
    case SYSINFO2CONFIG:
    case SYSINFO3CONFIG:
    case SCHEDULERCONFIG:
    case PHYCONFIG:
    case QOSCONFIG:
    case EUTRAHOPROFILECONFIG:
    case EUTRANEIGHCELLCONFIG:
    case PMCOUNTERLOGCONFIG:
    case LOGCONFIG:
    case MMECONFIG:
    case MSGTRACE:
    case SRSCONFIGDEDICATED:
    case UEEVENTHISTORYLOGCONFIG:
    case SONCONFIG:
    case SECURITYCONFIG:
      getAndSendTableData( &getObj );
      break;
      /* Get on PreConfiguration tables */
    case COUNTERCONFIG:
#ifdef L2M_SUPPORTED
      getAndSendTableData( &getObj );
#else
      bsmFillAndSendGetObjFail(&getObj,BS_ERR_FEATURE_NOT_SUPPORTED,  &getObj.tagArray.tagInfo[0]);
#endif
      break;
    case TRACECONFIG:
#ifdef L3MSGTRC_SUPPORTED
      getAndSendTableData( &getObj );
#else
      bsmFillAndSendGetObjFail(&getObj,BS_ERR_FEATURE_NOT_SUPPORTED,  &getObj.tagArray.tagInfo[0]);
#endif
      break;
    case TRACEHISTORY:
#ifdef L3MSGTRC_SUPPORTED
      createAndSendMsg(MSG_BSM_LTH_GET_OBJ_REQ, msg->data, msg->len);
#else
      bsmFillAndSendGetObjFail(&getObj,BS_ERR_FEATURE_NOT_SUPPORTED,  &getObj.tagArray.tagInfo[0]);
#endif
      break;
    case UEHCONFIG:
    case EGTPCONFIG:
    case SCTPCONFIG:
    case S1APCONFIG:
    case DEVICEMONITORCONFIG:
    case TIMERSCONFIG:
      getAndSendTableData( &getObj );
      break;
    case PROCESSSTART:
    case MODULESTOP:
      createAndSendMsg(MSG_BSM_PRM_GET_OBJ_REQ, msg->data, msg->len);
      break;
    case EVENTTHRESHOLDS:
    case UDHCONFIG:
      getAndSendTableData( &getObj );
      break;
    case ENBADDRESSES:
      {
        MsgBsmDehGetEnbAddrReq  req;
        clearBytes( &req, sizeof(MsgBsmDehGetEnbAddrReq) );
        req.cmdRef  = getObj.cmdRef;
        req.tagInfo = getObj.tagArray.tagInfo[0];
        createAndSendMsg(MSG_BSM_DEH_GET_ENB_ADDR_REQ, (UINT8*)&req, sizeof(MsgBsmDehGetEnbAddrReq));
      }
      break;
      /* Get on status tables */
    case FILEDOWNLOADSTATUS:
    case FILEUPLOADSTATUS:
      /* Ask from FTH */
      createAndSendMsg(MSG_BSM_FTH_GET_OBJ_REQ, msg->data, msg->len);
      break;
    case SWACTIVATIONSTATUS:
    case SWPACKAGESTATUS:
      createAndSendMsg(MSG_BSM_PRM_GET_OBJ_REQ, msg->data, msg->len);
      break;
    case ENBSTATUS:
      bsmCreateAndSendModStateReqToUehAndUdh(&getObj, bsmContext);
      break;
    case MACGENCOUNTERS:
    case MACUPLANECOUNTERS:
    case RLCGENCOUNTERS:
    case PDCPGENCOUNTERS:
    case PDCPUPLANECOUNTERS:
    case GTPGENCOUNTERS:
    case SCTPGENCOUNTERS:
    case RRCGENCOUNTERS:
    case RRCCELLCOUNTERS:
    case S1APGENCOUNTERS:
    case S1APPEERNODECOUNTERS:
    case ETHERNETCOUNTERS:
    case IPCOUNTERS:
    case RRMCOUNTERS:
    case PMLOGFILESTATUS:
      createAndSendMsg(MSG_BSM_PMH_GET_OBJ_REQ, msg->data, msg->len);
      break;
    case COUNTERCOLLECTIONSTATUS:
#ifdef L2M_SUPPORTED
      createAndSendMsg(MSG_BSM_PMH_GET_OBJ_REQ, msg->data, msg->len);
#else
      bsmFillAndSendGetObjFail(&getObj,BS_ERR_FEATURE_NOT_SUPPORTED,  &getObj.tagArray.tagInfo[0]);
#endif
      break;
      
    case L2COUNTERS:
#ifdef L2M_SUPPORTED
    /* GET on locked state is not allowed as There will be no counter available */
    if(FALSE EQ isEnodeBLocked())
      createAndSendMsg(MSG_BSM_PMH_GET_OBJ_REQ, msg->data, msg->len);
    else
      bsmFillAndSendGetObjFail(&getObj, BS_ERR_ENODEB_IS_LOCKED, &(getObj.tagArray.tagInfo[0]));
#else
      bsmFillAndSendGetObjFail(&getObj,BS_ERR_FEATURE_NOT_SUPPORTED,  &getObj.tagArray.tagInfo[0]);
#endif
      break;
    case RADIOHEADCONFIG:
      getAndSendTableData( &getObj );
      break;
    case RADIOHEADSTATUS:
      createAndSendMsg(MSG_BSM_DEH_GET_OBJ_REQ, msg->data, msg->len);
      break;
    case TRACESTATUS:
#ifdef L3MSGTRC_SUPPORTED
      createAndSendMsg(MSG_BSM_LTH_GET_OBJ_REQ, msg->data, msg->len);
#else
      bsmFillAndSendGetObjFail(&getObj,BS_ERR_FEATURE_NOT_SUPPORTED,  &getObj.tagArray.tagInfo[0]);
#endif
      break;
    case LOGINSTATUS:
    case CELLSTATUS:
      getAndSendTableData(&getObj);
      break;
    case FILEEVENTHISTORY:
      createAndSendMsg(MSG_BSM_FTH_GET_OBJ_REQ, msg->data, msg->len);
      break;
    case SWINSTALLHISTORY:
      createAndSendMsg(MSG_BSM_PRM_GET_OBJ_REQ, msg->data, msg->len);
      break;
    case ACTIVEUESTATUS:
      {
        MsgBsmUehActiveUeStatusReq  req;
        clearBytes( &req, sizeof(MsgBsmUehActiveUeStatusReq) );
        req.cmdRef = getObj.cmdRef;
        createAndSendMsg(MSG_BSM_UEH_GET_ACTIVE_UE_STATUS_REQ, (UINT8*)&req, sizeof(MsgBsmUehActiveUeStatusReq));
      }
      break;
    case ACTIVERABSTATUS:
      {
        MsgBsmUdhActiveRabStatusReq  req;
        clearBytes( &req, sizeof(MsgBsmUdhActiveRabStatusReq) );
        req.cmdRef = getObj.cmdRef;
        createAndSendMsg(MSG_BSM_UDH_GET_ACTIVE_RAB_STATUS_REQ, (UINT8*)&req, sizeof(MsgBsmUdhActiveRabStatusReq));
      }
      break;
    case UEEVENTHISTORY:
      {
        MsgBsmUehUeEventHistoryReq  req;
        clearBytes( &req, sizeof(MsgBsmUehUeEventHistoryReq) );
        req.cmdRef = getObj.cmdRef;
        createAndSendMsg(MSG_BSM_UEH_GET_UE_EVENT_HISTORY_REQ, (UINT8*)&req, sizeof(MsgBsmUehUeEventHistoryReq));
      }
      break;
    case RABHISTORY:
      {
        MsgBsmUdhRabHistoryReq  req;
        clearBytes( &req, sizeof(MsgBsmUdhRabHistoryReq) );
        req.cmdRef = getObj.cmdRef;
        createAndSendMsg(MSG_BSM_UDH_GET_RAB_HISTORY_REQ, (UINT8*)&req, sizeof(MsgBsmUdhRabHistoryReq));
      }
      break;
    case ACTIVEALARMSTATUS:
      updateAndSendActiveAlarmStatus(  &getObj );
      break;
    case ALARMHISTORY:
      updateAndSendAlarmHistoryStatus( &getObj );
      break;
    case MODULELOGFILESTATUS:
      getAndSendTableData( &getObj );
      break;
    case MODULESTATUS:
      createAndSendMsg(MSG_BSM_PRM_GET_OBJ_REQ, msg->data, msg->len);
      break;
    case LOCATIONSTATUS:
      getAndSendTableData( &getObj );
      break;
    case COMPONENTSTATUS:
    case GPSSTATUS:
    case LEDSTATUS:
      createAndSendMsg(MSG_BSM_DEH_GET_OBJ_REQ, msg->data, msg->len);
      break;
  }
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param   tableId       Table Id for which we have to fill the details
 * @param   rowId         Row Id of that particular table
 * @param   colId         column Id of that particular row.
 * @param   tlvRspArray   Tlv response array which we have to fill.
 * @param   rabHistory    RAB History structure
 * @details This function will fill the element detail of given table,row,column for RabHistory status table.
 * */
static void fillRabHistoryTlvElemt(
    IN TableId          tableId,
    IN RowId            rowId,
    IN ColId            colId,
    IO TlvArray         *tlvRspArray,
    IO RabHistory       *rabHistory
    )
{
  switch( colId )
  {
    case COL_RABHISTORY_INDEX:
      fillTagLenUint32( tableId, rowId, colId, rabHistory->index, tlvRspArray );
      break;
    case COL_RABHISTORY_INTERNALUEID:
      fillTagLenUint32( tableId, rowId, colId, rabHistory->internalUeId, tlvRspArray );
      break;
    case COL_RABHISTORY_RABIDENTITY:
      fillTagLenUint8( tableId, rowId, colId, rabHistory->rabIdentity, tlvRspArray );
      break;
    case COL_RABHISTORY_DRBIDENTITY:
      fillTagLenUint8( tableId, rowId, colId, rabHistory->drbIdentity, tlvRspArray );
      break;
    case COL_RABHISTORY_RABQCI:
      fillTagLenUint8( tableId, rowId, colId, rabHistory->rabQci, tlvRspArray );
      break;
    case COL_RABHISTORY_RABSETUPTIME:
      fillTagLenString( tableId, rowId, colId, rabHistory->rabSetupTime, MAX_SIZE_RABHISTORY_RABSETUPTIME, tlvRspArray );
      break;
    case COL_RABHISTORY_RABRELEASETIME:
      fillTagLenString( tableId, rowId, colId, rabHistory->rabReleaseTime, MAX_SIZE_RABHISTORY_RABRELEASETIME, tlvRspArray );
      break;
    case COL_RABHISTORY_RABRELEASECAUSE:
      fillTagLenUint32( tableId, rowId, colId, rabHistory->rabReleaseCause, tlvRspArray );
      break;
    case COL_RABHISTORY_DLGTPTUNNELID:
      fillTagLenUint32( tableId, rowId, colId, rabHistory->dlGtpTunnelId, tlvRspArray );
      break;
    case COL_RABHISTORY_ULGTPTUNNELID:
      fillTagLenUint32( tableId, rowId, colId, rabHistory->ulGtpTunnelId, tlvRspArray );
      break;
    case COL_RABHISTORY_GTPKILOBYTESDL:
      fillTagLenUint32( tableId, rowId, colId, rabHistory->gtpKiloBytesDL, tlvRspArray );
      break;
    case COL_RABHISTORY_GTPKILOBYTESUL:
      fillTagLenUint32( tableId, rowId, colId, rabHistory->gtpKiloBytesUL, tlvRspArray );
      break;

    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, rowId, colId );
      break;
  }
}
/**
 * @param   tableId       Table Id for which we have to fill the details
 * @param   rowId         Row Id of that particular table
 * @param   colId         column Id of that particular row.
 * @param   tlvRspArray   Tlv response array which we have to fill.
 * @param   ueEventHistory UE event History structure
 * @details This function will fill the element detail of given table,row,column for UeEventHistory status table.
 * */
static void fillUeEventHistoryTlvElemt(
    IN TableId          tableId,
    IN RowId            rowId,
    IN ColId            colId,
    IO TlvArray         *tlvRspArray,
    IO UeEventHistory   *ueEventHistory
    )
{
  switch( colId )
  {
    case COL_UEEVENTHISTORY_INDEX:
      fillTagLenUint32( tableId, rowId, colId, ueEventHistory->index, tlvRspArray );
      break;
    case COL_UEEVENTHISTORY_MMEC:
      fillTagLenUint8( tableId, rowId, colId, ueEventHistory->mmec, tlvRspArray );
      break;
    case COL_UEEVENTHISTORY_MTMSI:
      fillTagLenUint32( tableId, rowId, colId, ueEventHistory->mTmsi, tlvRspArray );
      break;
    case COL_UEEVENTHISTORY_CRNTI:
      fillTagLenUint16( tableId, rowId, colId, ueEventHistory->cRnti, tlvRspArray );
      break;
    case COL_UEEVENTHISTORY_EVENTTYPE:
      fillTagLenUint32( tableId, rowId, colId, ueEventHistory->eventType, tlvRspArray );
      break;
    case COL_UEEVENTHISTORY_EVENTDETAIL:
      fillTagLenString( tableId, rowId, colId, ueEventHistory->eventDetail, MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL, tlvRspArray );
      break;
    case COL_UEEVENTHISTORY_EVENTTIMESTAMP:
      fillTagLenString( tableId, rowId, colId, ueEventHistory->eventTimestamp, MAX_SIZE_UEEVENTHISTORY_EVENTTIMESTAMP, tlvRspArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, rowId, colId );
      break;
  }
}

/**
 * @param   tableId       Table Id for which we have to fill the details
 * @param   rowId         Row Id of that particular table
 * @param   colId         column Id of that particular row.
 * @param   tlvRspArray   Tlv response array which we have to fill.
 * @param   activeUeStatus  Active UE status structure
 * @details This function will fill the element detail of given table,row,column for ActiveUeStatus status table.
 * */
static void fillActivUeTlvElemt(
    IN TableId          tableId,
    IN RowId            rowId,
    IN ColId            colId,
    IO TlvArray         *tlvRspArray,
    IO ActiveUeStatus   *activeUeStatus
    )
{
  switch( colId )
  {
    case COL_ACTIVEUESTATUS_INDEX:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->index, tlvRspArray );
      break;
    case COL_ACTIVEUESTATUS_INTERNALUEIDENTITY:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->internalUeIdentity, tlvRspArray );
      break;
    case COL_ACTIVEUESTATUS_MMEC:
      fillTagLenUint8( tableId, rowId, colId, activeUeStatus->mmec, tlvRspArray );
      break;
    case COL_ACTIVEUESTATUS_MTMSI:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->mTmsi, tlvRspArray );
      break;
    case COL_ACTIVEUESTATUS_CRNTI:
      fillTagLenUint16( tableId, rowId, colId, activeUeStatus->cRnti, tlvRspArray );
      break;
    case COL_ACTIVEUESTATUS_S1APENBUEID:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->s1apEnbUeId, tlvRspArray );
      break;
    case COL_ACTIVEUESTATUS_S1APMMEUEID:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->s1apMmeUeId, tlvRspArray );
      break;
    case COL_ACTIVEUESTATUS_ENTRYTIME:
      fillTagLenString( tableId, rowId, colId, activeUeStatus->entryTime, MAX_SIZE_ACTIVEUESTATUS_ENTRYTIME, tlvRspArray );
      break;
    case COL_ACTIVEUESTATUS_DLAMBR:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->dlAmbr, tlvRspArray );
      break;
    case COL_ACTIVEUESTATUS_ULAMBR:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->ulAmbr, tlvRspArray );
      break;
    case COL_ACTIVEUESTATUS_UECATEGORY:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->ueCategory, tlvRspArray );
      break;
    case COL_ACTIVEUESTATUS_UEACCESSSTRATUMRELEASE:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->ueAccessStratumRelease, tlvRspArray );
      break;
    case COL_ACTIVEUESTATUS_ACTIVEINTPROTALGO:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->activeIntProtAlgo, tlvRspArray );
      break;
    case COL_ACTIVEUESTATUS_ACTIVECIPHALGO:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->activeCiphAlgo, tlvRspArray );
      break;
    case COL_ACTIVEUESTATUS_NUMERABS:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->numERabs, tlvRspArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, rowId, colId );
      break;
  }
}

/**
 * @param msg         GET Object Response sent by UEH Module
 * @param bsmContext  BSM context.
 * @details This function will Process Active UE status table received from UEH Module.
 */
BsmProceduralState bsmProcUehActiveUeStatusRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  MsgUehBsmActiveUeStatusRsp   *rspMsg = (MsgUehBsmActiveUeStatusRsp*)msg->data;

  UINT8         colIndex    = 0;
  UINT8         rowIndex    = 0;
  TableId       tableId     = ACTIVEUESTATUS;

  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  MsgId           msgId;

  initEncBuf(&encBuf);
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  /* copy CmdRef */
  rspTlv.cmdRef = rspMsg->cmdRef;

  /* Fill TLV */
  if(rspMsg->numElem EQ 0)
  {
    /* Fill 1st entry with default values. This is required only for display purpose at CIH and logging purpose */
    ActiveUeStatus status;
    clearBytes(&status, sizeof(ActiveUeStatus));
    snprintf(status.entryTime, MAX_SIZE_ACTIVEUESTATUS_ENTRYTIME +1, "%s","00000000000000000");
    for ( colIndex=1; colIndex <= MAX_COLS_IN_ACTIVEUESTATUS_TABLE; colIndex++ )
    {
      fillActivUeTlvElemt(tableId, 1, colIndex, &(rspTlv.tlvArray), &(status));
    }
  }
  else
  {
    for ( rowIndex=1; rowIndex <= rspMsg->numElem; rowIndex++ )
    {
      for ( colIndex=1; colIndex <= MAX_COLS_IN_ACTIVEUESTATUS_TABLE; colIndex++ )
      {
        fillActivUeTlvElemt(tableId, rowIndex, colIndex, &(rspTlv.tlvArray), &(rspMsg->activeUeStatus[rowIndex-1]));
      }
    }
  }

  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );

  switch (rspTlv.cmdRef.origin) {
    case OAMIF_CIH:
      msgId = bsmCihGetObjSuccMsgId(rspTlv.cmdRef.instanceId);
      break;
    case OAMIF_WIH:
      msgId = MSG_BSM_WIH_GET_OBJ_SUCC;
      break;
    case OAMIF_SIH:
      msgId = MSG_BSM_SIH_GET_OBJ_SUCC;
      break;
    default:
      logError("Unknown OAMIF=%d", rspTlv.cmdRef.origin);
      break;
  }
  /* Send */
  createAndSendMsg(msgId, (UINT8*) &encBuf.value, encBuf.len);
  return ( bsmContext->bsmProceduralState );
}
/**
 * @param   tableId         Table Id for which we have to fill the details
 * @param   rowId           Row Id of that particular table
 * @param   colId           column Id of that particular row.
 * @param   tlvRspArray     Tlv response array which we have to fill.
 * @param   activeUeStatus  Active Rab status structure
 * @details This function will fill the element detail of given table,row,column for ActiveUeStatus status table.
 * */
static void fillActivRabTlvElemt(
    IN TableId          tableId,
    IN RowId            rowId,
    IN ColId            colId,
    IO TlvArray         *tlvRspArray,
    IO ActiveRabStatus  *activeUeStatus
    )
{
  switch( colId )
  {
    case COL_ACTIVERABSTATUS_INDEX:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->index, tlvRspArray );
      break;
    case COL_ACTIVERABSTATUS_INTERNALUEID:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->internalUeId, tlvRspArray );
      break;
    case COL_ACTIVERABSTATUS_RABIDENTITY:
      fillTagLenUint8( tableId, rowId, colId, activeUeStatus->rabIdentity, tlvRspArray );
      break;
    case COL_ACTIVERABSTATUS_DRBIDENTITY:
      fillTagLenUint8( tableId, rowId, colId, activeUeStatus->drbIdentity, tlvRspArray );
      break;
    case COL_ACTIVERABSTATUS_RABQCI:
      fillTagLenUint8( tableId, rowId, colId, activeUeStatus->rabQci, tlvRspArray );
      break;
    case COL_ACTIVERABSTATUS_RABSETUPTIME:
      fillTagLenString( tableId, rowId, colId, activeUeStatus->rabSetupTime, MAX_SIZE_ACTIVERABSTATUS_RABSETUPTIME, tlvRspArray );
      break;
    case COL_ACTIVERABSTATUS_DLGTPTUNNELID:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->dlGtpTunnelId, tlvRspArray );
      break;
    case COL_ACTIVERABSTATUS_ULGTPTUNNELID:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->ulGtpTunnelId, tlvRspArray );
      break;
    case COL_ACTIVERABSTATUS_GTPKILOBYTESDL:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->gtpKiloBytesDL, tlvRspArray );
      break;
    case COL_ACTIVERABSTATUS_GTPKILOBYTESUL:
      fillTagLenUint32( tableId, rowId, colId, activeUeStatus->gtpKiloBytesUL, tlvRspArray );
      break;
    default:
      BS_ERROR_INVALID_COLID_RCVD( tableId, rowId, colId );
      break;
  }
}

/**
 * @param msg         GET Object Response sent by UEH Module
 * @param bsmContext  BSM context.
 * @details This function will Process Active UE status table received from UDH Module.
 */
BsmProceduralState bsmProcUdhActiveRabStatusRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  MsgUdhBsmActiveRabStatusRsp   *rspMsg = (MsgUdhBsmActiveRabStatusRsp*)msg->data;

  UINT8         colIndex    = 0;
  UINT8         rowIndex    = 0;
  TableId       tableId     = ACTIVERABSTATUS;

  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  MsgId           msgId;

  initEncBuf(&encBuf);
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  /* copy CmdRef */
  rspTlv.cmdRef = rspMsg->cmdRef;

  /* Fill TLV */
  if(rspMsg->numElem EQ 0)
  {
    /* Fill 1st entry with default values. This is required only for display purpose at CIH and logging purpose */
    ActiveRabStatus status;
    clearBytes(&status, sizeof(ActiveRabStatus));
    snprintf(status.rabSetupTime, MAX_SIZE_ACTIVERABSTATUS_RABSETUPTIME +1, "%s","00000000000000000");
    for ( colIndex=1; colIndex <= MAX_COLS_IN_ACTIVERABSTATUS_TABLE; colIndex++ )
    {
      fillActivRabTlvElemt(tableId, 1, colIndex, &(rspTlv.tlvArray), &(status));
    }
  }
  else
  {
    for ( rowIndex=1; rowIndex <= rspMsg->numElem; rowIndex++ )
    {
      for ( colIndex=1; colIndex <= MAX_COLS_IN_ACTIVERABSTATUS_TABLE; colIndex++ )
      {
        fillActivRabTlvElemt(tableId, rowIndex, colIndex, &(rspTlv.tlvArray), &(rspMsg->activeRabStatus[rowIndex-1]));
      }
    }
  }

  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );

  switch (rspTlv.cmdRef.origin)
  {
    case OAMIF_CIH: msgId = bsmCihGetObjSuccMsgId(rspTlv.cmdRef.instanceId); break;
    case OAMIF_WIH: msgId = MSG_BSM_WIH_GET_OBJ_SUCC; break;
    case OAMIF_SIH: msgId = MSG_BSM_SIH_GET_OBJ_SUCC; break;
    default:
                    logError("Unknown OAMIF=%d", rspTlv.cmdRef.origin);
                    break;
  }
  /* Send */
  createAndSendMsg(msgId, (UINT8*) &encBuf.value, encBuf.len);
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param msg         GET Object Response sent by UEH Module
 * @param bsmContext  BSM context.
 * @details This function will Process UE  EVENT History status table received from UEH Module.
 */
BsmProceduralState bsmProcUehUeEventHistoryRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  MsgUehBsmUeEventHistoryRsp   *rspMsg = (MsgUehBsmUeEventHistoryRsp*)msg->data;

  UINT8         colIndex    = 0;
  UINT8         rowIndex    = 0;
  TableId       tableId     = UEEVENTHISTORY;

  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  MsgId           msgId;

  initEncBuf(&encBuf);
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  /* copy CmdRef */
  rspTlv.cmdRef = rspMsg->cmdRef;

  /* Fill TLV */
  if(rspMsg->numElem EQ 0)
  {
    /* Fill 1st entry with default values. This is required only for display purpose at CIH and logging purpose */
    UeEventHistory status;
    clearBytes(&status, sizeof(UeEventHistory));
    snprintf(status.eventTimestamp, MAX_SIZE_UEEVENTHISTORY_EVENTTIMESTAMP +1, "%s", "00000000000000000");
    snprintf(status.eventDetail,    MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL +1,    "%s", "None");
    for ( colIndex=1; colIndex <= MAX_COLS_IN_UEEVENTHISTORY_TABLE; colIndex++ )
    {
      fillUeEventHistoryTlvElemt(tableId, 1, colIndex, &(rspTlv.tlvArray), &(status));
    }
  }
  else
  {
    for ( rowIndex=1; rowIndex <= rspMsg->numElem; rowIndex++ )
    {
      for ( colIndex=1; colIndex <= MAX_COLS_IN_UEEVENTHISTORY_TABLE; colIndex++ )
      {
        fillUeEventHistoryTlvElemt(tableId, rowIndex, colIndex, &(rspTlv.tlvArray), &(rspMsg->ueEventHistory[rowIndex-1]));
      }
    }
  }

  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );

  switch (rspTlv.cmdRef.origin) 
  {
    case OAMIF_CIH: msgId = bsmCihGetObjSuccMsgId(rspTlv.cmdRef.instanceId); break;
    case OAMIF_WIH: msgId = MSG_BSM_WIH_GET_OBJ_SUCC; break;
    case OAMIF_SIH: msgId = MSG_BSM_SIH_GET_OBJ_SUCC; break;
    default:
         logError("Unknown OAMIF=%d", rspTlv.cmdRef.origin);
         break;
  }
  /* Send */
  createAndSendMsg(msgId, (UINT8*) &encBuf.value, encBuf.len);
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param msg         GET Object Response sent by UDH Module
 * @param bsmContext  BSM context.
 * @details This function will Process rab History status table received from UDH Module.
 */
BsmProceduralState bsmProcUdhRabHistoryRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  MsgUdhBsmRabHistoryRsp   *rspMsg = (MsgUdhBsmRabHistoryRsp*)msg->data;

  UINT8         colIndex    = 0;
  UINT8         rowIndex    = 0;
  TableId       tableId     = RABHISTORY;

  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  MsgId           msgId;

  initEncBuf(&encBuf);
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  /* copy CmdRef */
  rspTlv.cmdRef = rspMsg->cmdRef;

  /* Fill TLV */
  if(rspMsg->numElem EQ 0)
  {
    /* Fill 1st entry with default values. This is required only for display purpose at CIH and logging purpose */
    RabHistory status;
    clearBytes(&status, sizeof(RabHistory));
    snprintf(status.rabReleaseTime, MAX_SIZE_RABHISTORY_RABRELEASETIME +1, "%s","00000000000000000");
    snprintf(status.rabSetupTime,   MAX_SIZE_RABHISTORY_RABSETUPTIME   +1, "%s","00000000000000000");
    for ( colIndex=1; colIndex <= MAX_COLS_IN_RABHISTORY_TABLE; colIndex++ )
    {
      fillRabHistoryTlvElemt(tableId, 1, colIndex, &(rspTlv.tlvArray), &(status));
    }
  }
  else
  {
    for ( rowIndex=1; rowIndex <= rspMsg->numElem; rowIndex++ )
    {
      for ( colIndex=1; colIndex <= MAX_COLS_IN_RABHISTORY_TABLE; colIndex++ )
      {
        fillRabHistoryTlvElemt(tableId, rowIndex, colIndex, &(rspTlv.tlvArray), &(rspMsg->rabHistory[rowIndex-1]));
      }
    }
  }

  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );

  switch (rspTlv.cmdRef.origin) {
    case OAMIF_CIH: msgId = bsmCihGetObjSuccMsgId(rspTlv.cmdRef.instanceId); break;
    case OAMIF_WIH: msgId = MSG_BSM_WIH_GET_OBJ_SUCC; break;
    case OAMIF_SIH: msgId = MSG_BSM_SIH_GET_OBJ_SUCC; break;
    default:
                    logError("Unknown OAMIF=%d", rspTlv.cmdRef.origin);
                    break;
  }
  /* Send */
  createAndSendMsg(msgId, (UINT8*) &encBuf.value, encBuf.len);
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param msg         MOD_STATE_REQ Response sent by UEH Module
 * @param bsmContext  BSM context.
 * @details This function will Process MOD_STATE_RSP received from UEH
 */
BsmProceduralState bsmProcUehModStateRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  GetObjReq   getReq;
  clearBytes( &getReq, sizeof(GetObjReq));

  MsgUehBsmModStateRsp   *rspMsg = (MsgUehBsmModStateRsp*)msg->data;
  EnbStatus *enbStatus = (EnbStatus*)getFirstRowOfTable(ENBSTATUS);

  enbStatus->numOfUesConnected  = rspMsg->numOfUesConnected;

  bsmContext->isModStateReqSentToUeh = FALSE;

  if((FALSE EQ bsmContext->isModStateReqSentToUeh ) && (FALSE EQ bsmContext->isModStateReqSentToUdh ))
  {
    getReq.cmdRef = rspMsg->cmdRef;
    getReq.tagArray.tagInfo[0] = rspMsg->tagInfo;

    updateEnodeBStatusTable( bsmContext );
    if(ENBSTATUS NOTEQ rspMsg->tagInfo.tableId)
    {
      BS_ERROR_UNKNOWN_TABLE_ID(rspMsg->tagInfo.tableId);
      return ( bsmContext->bsmProceduralState );
    }
    getAndSendTableData(&getReq);
  }
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param msg         MOD_STATE_REQ Response sent by UDH Module
 * @param bsmContext  BSM context.
 * @details This function will Process MOD_STATE_RSP received from UDH
 */
BsmProceduralState bsmProcUdhModStateRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  UINT32      totalThroughputMbps = 0;
  GetObjReq   getReq;
  clearBytes( &getReq, sizeof(GetObjReq));

  MsgUdhBsmModStateRsp   *rspMsg = (MsgUdhBsmModStateRsp*)msg->data;
  EnbStatus              *enbStatus = (EnbStatus*)getFirstRowOfTable(ENBSTATUS);
  EventThresholds        *evntThrhlds = (EventThresholds*)getFirstRowOfTable(EVENTTHRESHOLDS);
  /**/
  enbStatus->aggregateDLThroughputKbps  = rspMsg->aggregateDLThroughputKbps;
  enbStatus->aggregateULThroughputKbps  = rspMsg->aggregateULThroughputKbps;

  /* Calculate Load state */
  /* Get value in Mbps => 1 Mbps = 1000kbps */
  totalThroughputMbps = ( rspMsg->aggregateDLThroughputKbps / 1000) + ( rspMsg->aggregateULThroughputKbps / 1000);

  if ( totalThroughputMbps < evntThrhlds->enbLoadNormalMbps )
  {
    enbStatus->loadState = LOADSTATE_LOWLOAD;
  }
  else if ((evntThrhlds->enbLoadNormalMbps < totalThroughputMbps ) && (totalThroughputMbps < evntThrhlds->enbLoadHighMbps))
  {
    enbStatus->loadState = LOADSTATE_NORMALLOAD;
  }
  else if ( totalThroughputMbps > evntThrhlds->enbLoadHighMbps )
  {
    enbStatus->loadState = LOADSTATE_OVERLOADED;
  }

  bsmContext->isModStateReqSentToUdh = FALSE;

  /* Check whether response are received from both UEH and UDH */
  if((FALSE EQ bsmContext->isModStateReqSentToUeh ) && (FALSE EQ bsmContext->isModStateReqSentToUdh ))
  {
    getReq.cmdRef = rspMsg->cmdRef;
    getReq.tagArray.tagInfo[0] = rspMsg->tagInfo;

    updateEnodeBStatusTable( bsmContext );
    if(ENBSTATUS NOTEQ rspMsg->tagInfo.tableId)
    {
      BS_ERROR_UNKNOWN_TABLE_ID(rspMsg->tagInfo.tableId);
      return ( bsmContext->bsmProceduralState );
    }
    getAndSendTableData(&getReq);
  }
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param msg         GET_ENB_ADDR_REQ Response sent by DEH Module
 * @param bsmContext  BSM context.
 * @details This function will Process GET_ENB_ADDR_REQ received from DEH
 */
BsmProceduralState bsmProcGetEnbAddrRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  GetObjReq   getReq;
  clearBytes( &getReq, sizeof(GetObjReq));

  MsgDehBsmGetEnbAddrRsp   *rspMsg = (MsgDehBsmGetEnbAddrRsp*)msg->data;
  EnbAddresses *enbAddr = (EnbAddresses*)getFirstRowOfTable(ENBADDRESSES);

  /* Update the data base */
  snprintf(enbAddr->backhaulPortMacAddress,MAX_SIZE_ENBADDRESSES_BACKHAULPORTMACADDRESS + 1,"%s",rspMsg->backhaulPortMacAddress);
  snprintf(enbAddr->debugPortMacAddress,MAX_SIZE_ENBADDRESSES_DEBUGPORTMACADDRESS + 1,"%s",rspMsg->debugPortMacAddress);
  snprintf(enbAddr->uniqueProductId,MAX_SIZE_ENBADDRESSES_UNIQUEPRODUCTID + 1,"%s",rspMsg->productId);

  /* Fill and send GET_OBJ_SUCC */
  getReq.cmdRef = rspMsg->cmdRef;
  getReq.tagArray.tagInfo[0] = rspMsg->tagInfo;
  getAndSendTableData(&getReq);
  return ( bsmContext->bsmProceduralState );
}

/**
 * @param msg         GET_ENB_ADDR_REQ failure Response sent by DEH Module
 * @param bsmContext  BSM context.
 * @details This function will Process GET_ENB_ADDR_REQ received from DEH
 */
BsmProceduralState bsmProcGetEnbAddrFail(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    )
{
  GetObjReq   getReq;
  clearBytes( &getReq, sizeof(GetObjReq));

  MsgDehBsmGetEnbAddrFail   *rspMsg = (MsgDehBsmGetEnbAddrFail*)msg->data;

  /* Fill and send GET_OBJ_FAIL */
  getReq.cmdRef = rspMsg->cmdRef;
  getReq.tagArray.tagInfo[0] = rspMsg->tagInfo;
  getReq.tagArray.numTags = 1;
  bsmFillAndSendGetObjFail(&getReq, rspMsg->bsError, &(rspMsg->tagInfo));
  return ( bsmContext->bsmProceduralState );
}

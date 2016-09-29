/**
 * @file    pmh_cmn.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will contains functions to process different messages received from BSM, RRC, MAC and S1AP
 *
 * @author  Ramprasad Reddy Chinta
 * @date    10-02-2011
 * @details This file contains functions to process different messages received from other modules
 */

#include "pmh.h"

UINT32 pmhGetTimeInSecFromTimeStamp(
  UINT8        *timeStamp
  )
{
  char year[5];
  char month[3];
  char date[3];
  char hour[3];
  char min[3];
  char sec[3];

  snprintf((char *)&year,5,"%s",timeStamp); timeStamp+=4;
  snprintf((char *)&month,3,"%s",timeStamp); timeStamp+=2;
  snprintf((char *)&date,3,"%s",timeStamp); timeStamp+=2;
  snprintf((char *)&hour,3,"%s",timeStamp); timeStamp+=2;
  snprintf((char *)&min,3,"%s",timeStamp); timeStamp+=2;
  snprintf((char *)&sec,3,"%s",timeStamp); timeStamp+=2;

  int iyear  = atoi((char *)&year);
  int imonth = atoi((char *)&month);
  int idate  = atoi((char *)&date);
  int ihour  = atoi((char *)&hour);
  int imin   = atoi((char *)&min);
  int isec   = atoi((char *)&sec);

  //logInfo("year=%d,month=%d,date=%d,hour=%d,min=%d,sec=%d\n",iyear,imonth,idate,ihour,imin,isec);

  unsigned int tmonths=0;
  unsigned int tdays=0;
  unsigned int thours=0;
  unsigned int tmins=0;
  unsigned int tsecs=0;

  tmonths=(iyear-1970)*12;
  tdays  =(tmonths+imonth)*30;
  thours =(tdays+idate)*24;
  tmins  =(thours+ihour)*60;
  tsecs  =(tmins+imin)*60;
  tsecs  =tsecs+isec;

  //logInfo("Total number of seconds = %d",tsecs);

  return tsecs;
}
void pmhRemoveOldestEntryFromList(
  List *list
  )
{
  TBOOL                entryDel = FALSE;
  PmLogFileStatus      *logEntry;
  ListNode             *node=NULL;
  ListNode             *prev=NULL;
  FileName              oldestOpenFile;
  UINT32                oldestOpenFileTimeInSec = 1555200000; /* This will work up to 31Dec2020 */
  UINT32                fileOpenTimeInSec;
  clearBytes(&oldestOpenFile, sizeof(FileName));

  node = list->first;

  while (node)
  {
    logEntry = (PmLogFileStatus*)node->data;

    fileOpenTimeInSec = pmhGetTimeInSecFromTimeStamp(logEntry->openedAt);

    if(oldestOpenFileTimeInSec >= fileOpenTimeInSec)
    {
      oldestOpenFileTimeInSec = fileOpenTimeInSec;
      snprintf((char *)&oldestOpenFile, sizeof(FileName), "%s", logEntry->fileName);
    }
    node = node->next;
  }

  logInfo("File which should be deleted = %s", oldestOpenFile);
  node = list->first;
  while (node)
  {
    logEntry = (PmLogFileStatus*)node->data;
    if( 0 EQ strncmp( logEntry->fileName, oldestOpenFile, sizeof(FileName)) )
    {
      entryDel = TRUE;
      /* Remove file name from the status table */
      listDeleteByNode( list, node, prev, sizeof(PmLogFileStatus) );
      /* Remove file physically from the location if present.
       * In case upload, file would be deleted by FTH, but in case the file size was 0,
       * this file should be deleted by PMH */
      if ( 0 EQ remove(oldestOpenFile) )
      {
        logInfo(" File [%s] removed from the pmLogs directory ", oldestOpenFile );
      }
      else
      {
        logError(" File [%s] was not removed from the pmLogs directory. No such file exists in this folder", oldestOpenFile );
      }
      break;
    }
    prev = node;
    node = node->next;
  }

  if (entryDel EQ TRUE )
  {
    logInfo("Deleted entry from PM Log File Status table with Filename:%s. Total entries are : %d ", oldestOpenFile, list->numElemPresent );
    /* Write entries to persistent memory */
    pmWritePmLogFileStatusTableToPersistentMemory( "Entry cleared from PM Log File Status Table" );
  }
  else
  {
    logError("No entry found in PM Log File Status table with file name:%s. ", oldestOpenFile );
  }

}
/**
 * @param   cmdRef   CmdRef received in GET_OBJ_REQ
 * @param   tagInfo  TagInfo received in GET_OBJ_REQ
 * @param   transId  transId
 * @param   pmhContext PMH Context
 * @details This function will allocate a transId and store CmdRef and TagInfo in global context
 * */
BsErrorValue   pmhAllocTransId(
  IN CmdRef      *cmdRef,
  IN TagInfo     *tagInfo,
  IO UINT32      *transId,
  IO PmhContext  *pmhContext
  )
{
  UINT8 i;
  /* CmdRef == NULL for regular counter collection else it is for GET_OBJ_REQ */
  if(NULL EQ cmdRef)
  {
    *transId = 0;
    return BS_NO_ERROR;
  }
  else
  {
    for(i=0; i<MAX_PARALLEL_PMH_OAM_GET_PROCS; i++)
    {
      if(FALSE EQ pmhContext->transId2CmdRefTable[i].isUsed)
      {
        pmhContext->transId2CmdRefTable[i].isUsed  = TRUE;
        pmhContext->transId2CmdRefTable[i].cmdRef  = *cmdRef;
        if(NULL NOTEQ tagInfo )
          pmhContext->transId2CmdRefTable[i].tagInfo = *tagInfo;
        *transId = i+1;
        logInfo("LogInfo:Table ID =%u, RowId=%u,ColId=%u",
              pmhContext->transId2CmdRefTable[i].tagInfo.tableId,
              pmhContext->transId2CmdRefTable[i].tagInfo.rowId,
              pmhContext->transId2CmdRefTable[i].tagInfo.colId);
        return BS_NO_ERROR;
      }
    }
  }
  BS_ERROR_QUEUE_FULL();
  return BS_ERR_QUEUE_SIZE_EXCEEDED;
}

/**
 * @param   transId  transId
 * @param   cmdRef   CmdRef which need to be fetched from transId
 * @param   tagInfo  TagInfo which need to be fetched from transId
 * @param   pmhContext PMH Context
 * @details This function will fetch cmdRef and TagInfo using transId and deAlloc transId.
 * */
BsErrorValue   getCmdRefByTransIdAndDeallocTransId(
  IN UINT32      transId,
  IO CmdRef      *cmdRef,
  IO TagInfo     *tagInfo,
  IO PmhContext  *pmhContext
  )
{
  /* check whether transId is valid or not if not log the error */
  if ((transId > MAX_PARALLEL_PMH_OAM_GET_PROCS) || (transId <= 0))
  {
    logError("TransId %d is out of range MAX_PARALLEL_PMH_OAM_GET_PROCS = %d",transId, MAX_PARALLEL_PMH_OAM_GET_PROCS);
    return BS_ERR_INTERNAL_ERROR_OCCURED;
  }
  copyBytes(&(pmhContext->transId2CmdRefTable[transId-1].cmdRef), sizeof(CmdRef), cmdRef);
  copyBytes(&(pmhContext->transId2CmdRefTable[transId-1].tagInfo), sizeof(TagInfo), tagInfo);
  pmhContext->transId2CmdRefTable[transId-1].isUsed  = FALSE;
  return BS_NO_ERROR;
}

/**
 * @param   pmhContext PMH Context
 * @details This function allocates memory for PM Log status table
 * */
void initPmFileLogStatusTable(
  IO PmhContext  *pmhContext
  )
{
  /* Initialise the list and allocate the memory for PM LOG file status table */
  listInit( MAX_ROWS_IN_PMLOGFILESTATUS_TABLE, sizeof(PmLogFileStatus), &pmhContext->pmLogFileStatusTable );
}

/**
 * @details This function returns address of pmLogFileStatusTable
 * */
PmLogFileStatusTable *getPmLogFileStatusTable( void )  /* Linked List implementation */
{
  PmhContext   *ctx = getPmhContext();
  return &(ctx->pmLogFileStatusTable);
}

/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function sends message COUNTER_FILE_AVAILABLE to BSM
 * */
PmhState pmhProcMsgInUnexpectedState(
    IN Msg           *msg,
    IO PmhContext    *pmhContext
    )
{
  MsgPmhBsmOpstartFail  opstartFail;
  clearBytes(&opstartFail, sizeof(MsgPmhBsmOpstartFail));

  opstartFail.errorNum = BS_ERR_MSG_RCVD_IN_UNEXPECTED_STATE;
  BS_ERROR_MSG_RCVD_IN_UNEXPECTED_STATE( msg->msgId, getPmhStateName(pmhContext->pmhState));
  snprintf((char*)&opstartFail.failReason, sizeof(opstartFail.failReason), "%s", " OP-Start Req Received in Unexpected State");
  createAndSendMsg(MSG_PMH_BSM_OPSTART_FAIL, (UINT8*)&opstartFail, sizeof(MsgPmhBsmOpstartFail));
  return pmhContext->pmhState;
}

/**
 * @param   activePmLogFileName		active Log file name
 * @details This function sends message COUNTER_FILE_AVAILABLE to BSM
 * */
void pmhSendCntrFileAvailableMsgToBsm(
  IN FilePath    activePmLogFileName
  )
{
  MsgPmhBsmCntrFileAvailable ind;
  clearBytes( &ind, sizeof(MsgPmhBsmCntrFileAvailable) );

  snprintf( ind.pmLogFileName, sizeof(FilePath),"%s", activePmLogFileName );
  createAndSendMsg( MSG_PMH_BSM_COUNTER_FILE_AVAILABLE, (UINT8 *)&ind, sizeof(MsgPmhBsmCntrFileAvailable) );
}


/**
 * @param   peofileId     Profileid
 * @param   cause         measurement stopped cause
 * @details This function sends message MEASUREMENT_COMPLETE to BSM
 * */
void pmhSendMeasurementCompleteNotiToBsm(
  IN ProfileId             profileId,
  IN MeasurementStopCause  cause
  )
{
  MsgPmhBsmMeasurementCompleteInd ind;
  clearBytes( &ind, sizeof(MsgPmhBsmMeasurementCompleteInd) );

  ind.profileId = profileId;

  snprintf( ind.cause, MAX_SIZE_COUNTERCOLLECTIONSTATUS_STOPPEDREASON +1,"%s", getMeasurementStoppedCauseStr(cause) );
  createAndSendMsg( MSG_PMH_BSM_MEASUREMENT_CMPL_IND, (UINT8 *)&ind, sizeof(MsgPmhBsmMeasurementCompleteInd) );
}

/**
 * @param   bsErrorValue    BS Error value
 * @param   failReason      Failure reason string
 * @details This function will send the message COUNTER_COLLECTION_FAILED to BSM
 * */
void pmhSendCntrCollectFailMsgToBsm(
  IN BsErrorValue bsErrorValue,
  IN String256    failReason
  )
{
  MsgPmhBsmCntrCollectFail failMsg;
  clearBytes(&failMsg, sizeof(MsgPmhBsmCntrCollectFail));

  failMsg.errorDesc.errorNum = bsErrorValue;
  snprintf( failMsg.errorDesc.failReason, sizeof(String256), "%s", failReason );

  createAndSendMsg(MSG_PMH_BSM_COUNTER_COLLECTION_FAILED, (UINT8 *)&failMsg, sizeof(MsgPmhBsmCntrCollectFail));
}

/**
 * @param   strWriteReason string write reason
 * @details This function writes PmLogFileStatusTable to persisten memory.
 * */
void pmWritePmLogFileStatusTableToPersistentMemory(
  UINT8    *strWriteReason
  )
{
  PmhContext   *pmhContext = getPmhContext();
  UINT8 isError = FALSE;
  UINT8 fullFilePath[MAX_BUFFER_SIZE];
  UINT8 cmd[MAX_BUFFER_SIZE];
  clearBytes( &fullFilePath, sizeof(fullFilePath));
  clearBytes( &cmd, sizeof(cmd));

  /* Take current entries from the list and store in the PM file log table global */
  updatePmLogFileStatusTable();
  logInfo ( " Write to the persistent memory (pmLogFile.cfg) [ Reason :: %s ]!! ", strWriteReason );
  /* Write sw_package information to the persistent memory */
  GenericTable     *tableFunc = getTableFunctions(PMLOGFILESTATUS);
  PmLogFileStatus  *status = (PmLogFileStatus*)getFirstRowOfTable(PMLOGFILESTATUS);
  /* Firstly write the configuration to temporary file to avoid corruption of pmLogFile.cfg */
  /* BS-2383 Copy config file to temporary file*/
  snprintf( cmd, sizeof(cmd), "cp %s/pmLogFile.cfg %s/pmLogFile.cfg.tmp",
		    &pmhContext->configDir[0], &pmhContext->configDir[0] );
  if( 0 NOTEQ system(cmd) )
  {
	BS_ERROR_SYSTEM_COMMAND_FAILED(cmd);
  }
  snprintf( fullFilePath, MAX_BUFFER_SIZE, "%s/pmLogFile.cfg.tmp", &pmhContext->configDir[0] );
  if ( NULL EQ tableFunc->getWriteToFileFunc )
  {
    BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED(" PMLOGFILESTATUS ");
  }
  else
  {
    tableFunc->getWriteToFileFunc( status, fullFilePath, &isError, NULL );
    if ( TRUE EQ isError )
    {
      BS_ERROR_CONFIG_WRITE_FAILED( );
    }
    else
    {
      /* Move temporary file to config file */
      clearBytes( &cmd, sizeof(cmd));
      snprintf( cmd, sizeof(cmd), "mv %s/pmLogFile.cfg.tmp %s/pmLogFile.cfg",
	            &pmhContext->configDir[0], &pmhContext->configDir[0] );
      if( 0 NOTEQ system(cmd) )
      {
        BS_ERROR_SYSTEM_COMMAND_FAILED(cmd);
      }
    }
  }
}

/**
 * @param   ctx PMH Context
 * @details This function will update the row in PM Log file status table
 * */
void pmhUpdateEntryInPmLogFileStatusTable(
  IO PmhContext *ctx
  )
{
  PmLogFileStatus       *logEntry;

  /* get the row number to update */
  if(ctx->pmLogFileStatusRowNumToUpdate EQ MAX_ROWS_IN_PMLOGFILESTATUS_TABLE)
  {
    ctx->pmLogFileStatusRowNumToUpdate = 1;
  }
  else
  {
    ctx->pmLogFileStatusRowNumToUpdate += 1;
  }

  logEntry = (PmLogFileStatus *)getRowOfTable(PMLOGFILESTATUS, ctx->pmLogFileStatusRowNumToUpdate - 1);

  logEntry->index = ctx->pmLogFileStatusRowNumToUpdate;

  /* 
   * Remove file physically before overwriting row in pm log file status table
   */
  if ( 0 EQ remove(logEntry->fileName) )
  {
    logInfo(" File [%s] removed from the pmLogs directory ", logEntry->fileName );
  }
  else
  {
    logInfo(" File [%s] was not removed from the pmLogs directory. No such file exists in this folder", logEntry->fileName );
  }

  snprintf( logEntry->fileName, MAX_SIZE_PMLOGFILESTATUS_FILENAME+1, "%s", ctx->currPmLogFileName );
  snprintf( logEntry->openedAt, MAX_SIZE_PMLOGFILESTATUS_OPENEDAT+1, "%s", ctx->currPmLogFileOpenTimeStamp );

  snprintf( logEntry->closedAt, MAX_SIZE_PMLOGFILESTATUS_CLOSEDAT+1, "00000000000000000" );

  logInfo("PmLogFile Entry details :\n{\n fileName=%s\n fileSizeInKB=%d\n openedAt=%s\n closedAt=%s\n}\n",
        logEntry->fileName, logEntry->fileSizeInBytes, logEntry->openedAt, logEntry->closedAt ) ;

  /* Write entries to persistent memory */
  pmWritePmLogFileStatusTableToPersistentMemory( "entry updated in PM Log File Status Table" );
}

/**
 * @param   ctx PMH Context
 * @details This function will add the row in PM Log file status table
 * */
TBOOL pmhAddEntryInPmLogFileStatusTable(
  IO PmhContext *ctx
  )
{
  TBOOL                 entryAdded;
//  List                  *list = getPmLogFileStatusTable();
  List                  *list = &ctx->pmLogFileStatusTable;
  PmLogFileStatus        logEntry;
  clearBytes( &logEntry, sizeof(PmLogFileStatus));

  logEntry.index = 0;
  snprintf( logEntry.fileName, MAX_SIZE_PMLOGFILESTATUS_FILENAME+1, "%s", ctx->currPmLogFileName );
  snprintf( logEntry.openedAt, MAX_SIZE_PMLOGFILESTATUS_OPENEDAT+1, "%s", ctx->currPmLogFileOpenTimeStamp );

  snprintf( logEntry.closedAt, MAX_SIZE_PMLOGFILESTATUS_CLOSEDAT+1, "00000000000000000" );

  entryAdded = listInsert(list, (void*)&logEntry, sizeof(PmLogFileStatus) );
  if ( entryAdded EQ TRUE )
  {
    logInfo("PmLogFileStatus: Added entry. Total entries are: %d ", list->numElemPresent );
    logInfo("PmLogFile Entry details :\n{\n fileName=%s\n fileSizeInKB=%d\n openedAt=%s\n closedAt=%s\n}\n",
        logEntry.fileName, logEntry.fileSizeInBytes, logEntry.openedAt, logEntry.closedAt ) ;
  }
  else
  {
    /* Log Error - Table full */
    BS_ERROR_ALL_PMLOGFILESTATUS_ROWS_FILLED("PmLogFileStatus");
    stopallTimers();
    pmhClosePmLogFile(ctx);
    return FALSE;
  }
  /* Write entries to persistent memory */
  pmWritePmLogFileStatusTableToPersistentMemory( "New entry added to PM Log File Status Table" );

  return TRUE;
}


TBOOL pmhAddEntryInPmLogFileStatusTableForProfile(
  IO CounterProfileContext *pCtx
  )
{
  TBOOL                 entryAdded;
  List                  *list = getPmLogFileStatusTable();
  PmLogFileStatus        logEntry;

  clearBytes( &logEntry, sizeof(PmLogFileStatus));

  logEntry.index = 0;
  snprintf( logEntry.fileName, MAX_SIZE_PMLOGFILESTATUS_FILENAME+1, "%s", pCtx->currPmLogFileName );
  snprintf( logEntry.openedAt, MAX_SIZE_PMLOGFILESTATUS_OPENEDAT+1, "%s", pCtx->currPmLogFileOpenTimeStamp );

  snprintf( logEntry.closedAt, MAX_SIZE_PMLOGFILESTATUS_CLOSEDAT+1, "00000000000000000" );

  if(listHasSpace(list) EQ FALSE)
  {
    /* if list has no space then remove the oldest entry and then add */
    logInfo("List=%x of size=%d is full", list, listGetMaxLen(list));
    logInfo("%s","Removing the oldest PM log entry from the list");
    pmhRemoveOldestEntryFromList(list);
  }

  entryAdded = listInsert(list, (void*)&logEntry, sizeof(PmLogFileStatus) );
  if ( entryAdded EQ TRUE )
  {
    logInfo("PmLogFileStatus: Added entry. Total entries are: %d ", list->numElemPresent );
    logInfo("PmLogFile Entry details :\n{\n fileName=%s\n fileSizeInKB=%d\n openedAt=%s\n closedAt=%s\n}\n",
        logEntry.fileName, logEntry.fileSizeInBytes, logEntry.openedAt, logEntry.closedAt ) ;
  }
  else
  {
    /* Log Error - Table full */
    BS_ERROR_ALL_PMLOGFILESTATUS_ROWS_FILLED("PmLogFileStatus");
    return FALSE;
  }
  /* Write entries to persistent memory */
  pmWritePmLogFileStatusTableToPersistentMemory( "New entry added to PM Log File Status Table" );

  return TRUE;
}

/**
 * @param   pmLogFileName		pm Log file name
 * @details This function clears respective row of active pm log file name
 * */
void pmhClearPmLogFileStatusRow(
  IN FilePath      pmLogFileName
  )
{
  TBOOL                entryDel = FALSE;
  PmLogFileStatus      *logEntry;
  ListNode             *node=NULL;
  ListNode             *prev=NULL;
  List                 *list = getPmLogFileStatusTable();
  node = list->first;

  while (node)
  {
    logEntry = (PmLogFileStatus*)node->data;

    if( 0 EQ strcmp(strstr(pmLogFileName,"Pico_Enb_"), strstr(logEntry->fileName,"Pico_Enb_") ) )
    {
      entryDel = TRUE;
      /* Remove file name from the status table */
      listDeleteByNode( list, node, prev, sizeof(PmLogFileStatus) );
      /* Remove file physically from the location if present.
       * In case upload, file would be deleted by FTH, but in case the file size was 0,
       * this file should be deleted by PMH */
      if ( 0 EQ remove(pmLogFileName) )
      {
        logInfo(" File [%s] removed from the pmLogs directory ", pmLogFileName );
      }
      else
      {
        logInfo(" File [%s] was not removed from the pmLogs directory. No such file exists in this folder", pmLogFileName );
      }
      break;
    }
    prev = node;
    node = node->next;
  }

  if (entryDel EQ TRUE )
  {
    logInfo("Deleted entry from PM Log File Status table with Filename:%s. Total entries are : %d ", pmLogFileName, list->numElemPresent );
    /* Write entries to persistent memory */
    pmWritePmLogFileStatusTableToPersistentMemory( "Entry cleared from PM Log File Status Table" );
  }
  else
  {
    logInfo("No entry found in PM Log File Status table with file name:%s. ", pmLogFileName );
  }
}

/**
 * @param   pmLogFileName  pm Log file name
 * @details This function gets the size of file.
 * */
UINT32 pmhGetPmFileSizeInBytes(
  IN FilePath   pmLogFileName
  )
{
  PmLogFileStatus      *logEntry;
  ListNode             *node;
  List                 *list = getPmLogFileStatusTable();

  node = list->first;
  while (node)
  {
    logEntry = (PmLogFileStatus*)node->data;
    if( 0 EQ strncmp(pmLogFileName, logEntry->fileName, strlen(pmLogFileName)) )
    {
      return logEntry->fileSizeInBytes;
    }
    node = node->next;
  }
  return 0;
}

/**
 * @param   pmLogFileName PM Log file name
 * @details This function sends notification to BSM
 * */
void sendNotiToBsmIfFileIsNonZeroOrClearFileIfZero(
  IN FilePath   pmLogFileName
  )
{
  if( 0 NOTEQ pmhGetPmFileSizeInBytes(pmLogFileName) )
  {
    pmhSendCntrFileAvailableMsgToBsm( pmLogFileName );
  }
  else
  {
    pmhClearPmLogFileStatusRow( pmLogFileName );
  }
}

/**
 * @param   pmLogFileName PM Log file name
 * @details This function will update the closing time of PM Log file
 * */
void pmhUpdatePmLogFileCloseTime(
  IN FilePath   pmLogFileName
  )
{
  PmLogFileStatus *logEntry;
  ListNode        *node;
  List            *list = getPmLogFileStatusTable();

  node = list->first;
  while (node)
  {
    logEntry = (PmLogFileStatus*)node->data;
    if( 0 EQ strncmp(pmLogFileName, logEntry->fileName, strlen(pmLogFileName)) )
    {
      getAndFillDateAndTimeStamp( MAX_SIZE_PMLOGFILESTATUS_CLOSEDAT,  logEntry->closedAt );
      break;
    }
    node = node->next;
  }
}

/**
 * @param   pmLogFileName PM Log file name
 * @details This function will update the fileSizeinKb of PM Log file
 * */
void pmhUpdatePmLogFileSize(
  IN FilePath  pmLogFileName
  )
{
  PmLogFileStatus *logEntry;
  ListNode        *node;
  List            *list = getPmLogFileStatusTable();
  struct stat fileCtx;

  clearBytes(&fileCtx, sizeof(struct stat));

  node = list->first;
  while (node)
  {
    logEntry = (PmLogFileStatus*)node->data;
    if( 0 EQ strncmp(pmLogFileName, logEntry->fileName, strlen(pmLogFileName)) )
    {
      if(0 EQ stat(pmLogFileName, &fileCtx))
      {
        logEntry->fileSizeInBytes = fileCtx.st_size;
        clearBytes(&fileCtx, sizeof(struct stat));
        logInfo(" FileSize is : %d", logEntry->fileSizeInBytes );
        break;
      }
    }
    node = node->next;
  }
}

/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function processes FILE UPLOADED message from BSM
 * */
PmhState pmhProcPmFileUploadedMsg(
    IN Msg           *msg,
    IO PmhContext    *pmhContext
    )
{
  MsgBsmPmhPmFileUploaded *fileUploadedInd = (MsgBsmPmhPmFileUploaded *)msg->data;
  pmhClearPmLogFileStatusRow( fileUploadedInd->pmLogFileName );

  return pmhContext->pmhState;
}

/**
 * @param   msg Message received
 * @details This function processes SetLogCfg message from BSM
 * */
PmhState pmhProcSetLogCfgInAllStates(
    IN Msg           *msg,
    IO PmhContext    *pmhContext
    )
{
   MsgBsmPmhSetLogCfg *updateLogConfig = (MsgBsmPmhSetLogCfg *)msg->data;
   LogContext  *logContext  = getLogContext();

   copyBytes(updateLogConfig, sizeof(LogConfig), &(logContext->logConfig));
   logInfo("%s", "Loglevel is set to new values");
   return pmhContext->pmhState;
}

/**
 * @details This function checks if any PM counter group is enabled
 * */
TBOOL pmhIsAnyCounterEnabled()
{
  PmhContext *ctx = getPmhContext();
  TBOOL    atleastOneCounterEnabled = FALSE;

  clearBytes( &ctx->sendCounterReq, sizeof(CounterFlags) );

  if(ctx->pmCounterLogConfig.collectMacGenCounters EQ TRUE)
  {
    ctx->sendCounterReq.macGenCounter = TRUE;
    atleastOneCounterEnabled = TRUE;
  }
  if(ctx->pmCounterLogConfig.collectMacUplaneCounters EQ TRUE)
  {
    ctx->sendCounterReq.macUplaneCounter = TRUE;
    atleastOneCounterEnabled = TRUE;
  }
  if(ctx->pmCounterLogConfig.collectRlcGenCounters EQ TRUE)
  {
    ctx->sendCounterReq.rlcGenCounter = TRUE;
    atleastOneCounterEnabled = TRUE;
  }
  if(ctx->pmCounterLogConfig.collectPdcpGenCounters EQ TRUE)
  {
    ctx->sendCounterReq.pdcpGenCounter = TRUE;
    atleastOneCounterEnabled = TRUE;
  }
  if(ctx->pmCounterLogConfig.collectPdcpUplaneCounters EQ TRUE)
  {
    ctx->sendCounterReq.pdcpUplaneCounter = TRUE;
    atleastOneCounterEnabled = TRUE;
  }
  if(ctx->pmCounterLogConfig.collectGtpGenCounters EQ TRUE)
  {
    ctx->sendCounterReq.gtpGenCounter = TRUE;
    atleastOneCounterEnabled = TRUE;
  }
  if(ctx->pmCounterLogConfig.collectSctpGenCounters EQ TRUE)
  {
    ctx->sendCounterReq.sctpGenCounter = TRUE;
    atleastOneCounterEnabled = TRUE;
  }
  if(ctx->pmCounterLogConfig.collectS1apGenCounters EQ TRUE)
  {
    ctx->sendCounterReq.s1apGenCounter = TRUE;
    atleastOneCounterEnabled = TRUE;
  }
  if(ctx->pmCounterLogConfig.collectS1apPeerNodeCounters EQ TRUE)
  {
    ctx->sendCounterReq.s1apPeerNodeCounter = TRUE;
    atleastOneCounterEnabled = TRUE;
  }
  if(ctx->pmCounterLogConfig.collectRrcGenCounters EQ TRUE)
  {
    ctx->sendCounterReq.rrcGenCounter = TRUE;
    atleastOneCounterEnabled = TRUE;
  }
  if(ctx->pmCounterLogConfig.collectRrcCellCounters EQ TRUE)
  {
    ctx->sendCounterReq.rrcCellCounter = TRUE;
    atleastOneCounterEnabled = TRUE;
  }
  if(ctx->pmCounterLogConfig.collectRrmCounters EQ TRUE)
  {
    ctx->sendCounterReq.rrmCounter = TRUE;
    atleastOneCounterEnabled = TRUE;
  }
  if(ctx->pmCounterLogConfig.collectIPCounters EQ TRUE)
  {
    ctx->sendCounterReq.ipCounter = TRUE;
    atleastOneCounterEnabled = TRUE;
  }
  if(ctx->pmCounterLogConfig.collectEthernetCounters EQ TRUE)
  {
    ctx->sendCounterReq.ethernetCounter = TRUE;
    atleastOneCounterEnabled = TRUE;
  }
  return atleastOneCounterEnabled;
}

/**
 * @param   ctx	PMH context
 * @details This function will open pm Log file to write counter values
 * */
TBOOL pmhOpenPmLogFile(
  IO PmhContext *ctx
  )
{
  UINT32      dirSize = 0;
  time_t      noSecs  = 0;
  String256   timeStamp;

  if( TRUE EQ pmhHasPmLogDirSizeExceeded(ctx, &dirSize) )
  {
    BS_ERROR_COMBINED_STAT_FILE_SIZE_EXCEEDED( dirSize );
    pmhSendCntrCollectFailMsgToBsm( BS_ERR_COMBINED_STAT_FILE_SIZE_EXCEEDED, "Combined statistics File-Size Exceeded" );
    stopallTimers();
    return FALSE;
  }

  clearBytes(timeStamp, sizeof(String256));

  noSecs = time(NULL);
  strftime( timeStamp, sizeof(String256), "%Y_%b_%d_%H_%M_%S", localtime(&noSecs) );
  snprintf( ctx->currPmLogFileName, MAX_SIZE_PMLOGFILESTATUS_FILENAME+1, "%s/Pico_Enb_%d_PM_log_%s.txt", &ctx->pmLogDir[0], ctx->enodebId, timeStamp);

  getAndFillDateAndTimeStamp( MAX_SIZE_PMLOGFILESTATUS_OPENEDAT, ctx->currPmLogFileOpenTimeStamp );

  ctx->fp = fopen(ctx->currPmLogFileName, "w+");

  if(ctx->fp NOTEQ NULL)
  {
    logInfo("PM snapshot file [%s] opened successfully",ctx->currPmLogFileName);
    setvbuf( ctx->fp, NULL, _IOLBF, 0);
  }
  else
  {
    BS_ERROR_IN_OPENING_LOG_FILE( ctx->currPmLogFileName );
    return FALSE;
  }

  return TRUE;
}

/**
 * @param   ctx PMH context
 * @details This function will open pm Log file to write counter values per profile
 * */
TBOOL pmhOpenPmLogFileForProfile(
  IN ProfileId   profileId,
  IO PmhContext *ctx
  )
{
  time_t      noSecs  = 0;
  String256   timeStamp;
  CounterProfileContext     *pCtx = getCounterProfileCtx(profileId);

  /* TODO need to add check on file size and folder size */

  clearBytes(timeStamp, sizeof(String256));

  noSecs = time(NULL);
  strftime( timeStamp, sizeof(String256), "%Y_%b_%d_%H_%M_%S", localtime(&noSecs) );
  snprintf( pCtx->currPmLogFileName, MAX_SIZE_PMLOGFILESTATUS_FILENAME+1,
           "%s/Pico_Enb_%d_PM_log_%s.txt", &ctx->pmLogDir[0], ctx->enodebId, timeStamp);

  getAndFillDateAndTimeStamp( MAX_SIZE_PMLOGFILESTATUS_OPENEDAT, pCtx->currPmLogFileOpenTimeStamp );

  pCtx->fp = fopen(pCtx->currPmLogFileName, "w+");

  if(pCtx->fp NOTEQ NULL)
  {
    logInfo("PM snapshot file [%s] opened successfully",pCtx->currPmLogFileName);
    setvbuf( pCtx->fp, NULL, _IOLBF, 0);
  }
  else
  {
    BS_ERROR_IN_OPENING_LOG_FILE( pCtx->currPmLogFileName );
    return FALSE;
  }

  return TRUE;
}

/**
 * @param   ctx				 PMH context
 * @details This function will close the PM Log file
 * */
void pmhClosePmLogFile(
  IO PmhContext *ctx
  )
{
  pmhUpdatePmLogFileSize(ctx->currPmLogFileName);
  pmhUpdatePmLogFileCloseTime(ctx->currPmLogFileName);

  if(NULL NOTEQ ctx->fp)
  {
    if(EOF EQ fclose(ctx->fp))
    {
      BS_ERROR_IN_CLOSING_LOG_FILE(ctx->currPmLogFileName);
    }
    else
    {
      logInfo("PM snapshot file [%s] closed successfully", ctx->currPmLogFileName );
    }
    ctx->fp = NULL;
  }
  /* Write entries to persistent memory */
  pmWritePmLogFileStatusTableToPersistentMemory( "Current PM Log File Closed" );
}

void pmhClosePmLogFileForProfile(
  IO CounterProfileContext *pCtx
  )
{

  //pmhUpdatePmLogFileSize(pCtx->currPmLogFileName);
  //pmhUpdatePmLogFileCloseTime(pCtx->currPmLogFileName);

  if(NULL NOTEQ pCtx->fp)
  {
    if(EOF EQ fclose(pCtx->fp))
    {
      BS_ERROR_IN_CLOSING_LOG_FILE(pCtx->currPmLogFileName);
    }
    else
    {
      logInfo("PM snapshot file [%s] closed successfully", pCtx->currPmLogFileName );
    }
    pCtx->fp = NULL;
  }
  /* Write entries to persistent memory */
  pmWritePmLogFileStatusTableToPersistentMemory( "Current PM Log File Closed" );
}

/**
 * @param   ctx				 PMH context
 * @param   dirSize Directory size
 * @details This function returns whether PM Log file size is exceeded threshold
 * */
TBOOL pmhHasPmLogDirSizeExceeded(
  IO PmhContext *ctx,
  IO UINT32     *dirSize
  )
{
  FILE *fp=NULL;
  String100 buf;
  String256 cmd;
  String150 fileName;

  clearBytes(cmd, sizeof(String256));
  clearBytes(fileName, sizeof(String150));
  clearBytes(buf, sizeof(String100));

  snprintf( cmd, sizeof(String256),"du --block-size=1>%sfileSize.txt",ctx->pmLogDir );
  if (0 NOTEQ system(cmd))
  {
	  BS_ERROR_SYSTEM_COMMAND_FAILED( cmd );
  }
  snprintf(fileName, sizeof(fileName), "%sfileSize.txt",ctx->pmLogDir);
  fp = fopen(fileName, "r");
  fscanf(fp, "%s", (char*)buf);
  sscanf(buf, "%d", (int*)dirSize);

  fclose(fp);

  /*
   * If PMLog directory size exceeded threshold then set pmStopeedFlag to TRUE
   */
  if( *dirSize > ctx->maxCombinedPmFileSize )
  {
    ctx->isCounterCollectionInProgress = FALSE;
    return TRUE;
  }

  return FALSE;
}

/**
 * @param   sendCounterReq   Runtime counter flags to indicate wether response from standard protocol is received
 * @param   rcvdRspMsg  Received response message
 * @details This function checks if all reponses are received from standard protocols for counter request messages
 * */
TBOOL pmhStsRspRcvdFromAllModules(
    IN CounterFlags *sendCounterReq,
    IO CounterFlags *rcvdRspMsg
    )
{
  if ( 0 EQ memcmp( sendCounterReq, rcvdRspMsg, sizeof(CounterFlags) ) )
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
 * @param   ctx       PMH context
 * @details This function opens pmLog file and starts timers
 * */
static TBOOL pmhOpenFileAndStartTimers(
  IO PmhContext *ctx
  )
{
  /* Start SampleInterval timer to collect Counter values */
  if( NULL_TIMER_ID EQ pmhStartTimer(TIMER_INDEX_SAMPLE_INTERVAL, 0, pmhGetSampleIntervalTimeVal(ctx->pmCounterLogConfig.sampleInterval)) )
  {
    BS_ERROR_PMH_TIMER_NOT_STARTED("SampleInterval");
    return FALSE;
  }

  /* Open PM Log file to collect all counter values */
  if ( FALSE EQ pmhOpenPmLogFile(ctx) )
  {
    BS_ERROR_IN_OPENING_LOG_FILE(ctx->currPmLogFileName);
    return FALSE;
  }

  /* Start PMFile timer to close the log file */
  if( NULL_TIMER_ID EQ pmhStartTimer(TIMER_INDEX_PM_LOGFILE_CREATE_INTERVAL, 0, pmhGetPmLogFileCreationTimeVal(ctx->pmCounterLogConfig.pmFileCreationInterval)) )
  {
    BS_ERROR_PMH_TIMER_NOT_STARTED("PMLogFile");
    return FALSE;
  }

  if( FALSE EQ pmhAddEntryInPmLogFileStatusTable(ctx) )
  {
    return FALSE;
  }
  return TRUE;
}

/**
 * @param   ctx       PMH context
 * @details This function executes config change scenario
 * */
void pmhHandleSampleIntervalTimeChange(
  IO PmhContext *ctx
  )
{
  stopallTimers();
  pmhClosePmLogFile(ctx);

  sendNotiToBsmIfFileIsNonZeroOrClearFileIfZero( ctx->currPmLogFileName );

  /* Reset the flag */
  ctx->isCounterCollectionInProgress = FALSE;

  /* Check 3 : Send message counter collection failed to BSM, if total number of statistics files reached threshold */
  if ( FALSE EQ listHasSpace(getPmLogFileStatusTable()) )
  {
    BS_ERROR_ALL_PMLOGFILESTATUS_ROWS_FILLED("PmLogFileStatus");
    pmhSendCntrCollectFailMsgToBsm( BS_ERR_NUM_STS_FILES_REACH_THRESHOLD, "Total number of statistics files reached threshold" );
    return;
  }

  if ( TRUE EQ pmhOpenFileAndStartTimers(ctx))
    ctx->isCounterCollectionInProgress = TRUE;
}

/**
 * @details This function initialises all counter status tables
 * */
static void pmhClearAllCounters()
{
  GenericTable  *tableFunc;
  tableFunc = getTableFunctions(MACGENCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("MACGENCOUNTERS");
  }

  tableFunc = getTableFunctions(MACUPLANECOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("MACUPLANECOUNTERS");
  }

  tableFunc = getTableFunctions(RLCGENCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("RLCGENCOUNTERS");
  }

  tableFunc = getTableFunctions(PDCPGENCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("PDCPGENCOUNTERS");
  }

  tableFunc = getTableFunctions(PDCPUPLANECOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("PDCPUPLANECOUNTERS");
  }

  tableFunc = getTableFunctions(GTPGENCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("GTPGENCOUNTERS");
  }

  tableFunc = getTableFunctions(SCTPGENCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("SCTPGENCOUNTERS");
  }
  tableFunc = getTableFunctions(RRCGENCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("RRCGENCOUNTERS");
  }

  tableFunc = getTableFunctions(RRMCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("RRMCOUNTERS");
  }

  tableFunc = getTableFunctions(RRCCELLCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("RRCCELLCOUNTERS");
  }

  tableFunc = getTableFunctions(S1APGENCOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("S1APGENCOUNTERS");
  }

  tableFunc = getTableFunctions(S1APPEERNODECOUNTERS);
  if(NULL NOTEQ tableFunc->getEmptyTableFunc)
  {
    tableFunc->getEmptyTableFunc();
  }
  else
  {
    BS_ERROR_NO_EMPTY_FUNCTION_DEFINED("S1APPEERNODECOUNTERS");
  }
}

/**
 * @details This function updates the combined size of all pm Log files in PM log directory
 * */
void pmhGetCombinedPmLogSizeThreshold()
{
  PmhContext     *ctx       = getPmhContext();
  UINT8           isError   = FALSE;
  UINT8           fullFilePath[MAX_BUFFER_SIZE];
  EnbLimitsConfig enbLimitsConfig;

  clearBytes( &fullFilePath, sizeof(fullFilePath));
  clearBytes(&enbLimitsConfig, sizeof(EnbLimitsConfig));

  snprintf( fullFilePath, MAX_BUFFER_SIZE, "%s/enbLimits.cfg", ctx->configDir);

  if ( TRUE NOTEQ  readFromFileEnbLimitsConfigTable(fullFilePath, &enbLimitsConfig, &isError))
  {
    BS_ERROR_CONFIG_READ_FAILED();
    exit(EXIT_FAILURE);
  }
   /* Update the max PMLog directory size in PMH context
     */
  ctx->maxCombinedPmFileSize = enbLimitsConfig.maxCombinedPmLogFileSize;
  ctx->maxCombinedPmFileSize = ctx->maxCombinedPmFileSize * BYTES_PER_ONE_MEGABYTE;
  logInfo("Max Combined PmLog file size is %d bytes",ctx->maxCombinedPmFileSize);
}

/**
 * @param pmLogFileName PM Log file name
 * @details This function checks if pm log file size is zero or non-zero
 * */
static UINT32 isFileSizeNonZero(
  IN UINT8  *pmLogFileName
  )
{
  struct stat fileCtx;
  clearBytes(&fileCtx, sizeof(struct stat));

  if(0 EQ stat(pmLogFileName, &fileCtx))
  {
    return fileCtx.st_size;
  }
  return 0;
}

/**
 * @details This function updates PM log file status table
 * */
void pmhReadPmLogFileTableAndUpdateFileStatusTable()
{
  PmhContext     *ctx       = getPmhContext();
  UINT8           isError   = FALSE;
  UINT8           fullFilePath[MAX_BUFFER_SIZE];
  UINT8           numElemIndex;
  UINT32          validEntries = 0;
  PmLogFileStatus pmLogFileStatus[MAX_ROWS_IN_PMLOGFILESTATUS_TABLE];

  clearBytes( &fullFilePath, sizeof(fullFilePath));
  clearBytes( &pmLogFileStatus, sizeof(PmLogFileStatus)*MAX_ROWS_IN_PMLOGFILESTATUS_TABLE );

  snprintf( fullFilePath, MAX_BUFFER_SIZE, "%s/pmLogFile.cfg", &ctx->configDir[0] );

  readFromFilePmLogFileStatusTable(fullFilePath, &pmLogFileStatus[0], &isError);

  if ( TRUE EQ isError )
  {
    BS_ERROR_CONFIG_READ_FAILED();
    exit(EXIT_FAILURE);
  }

  /* Read from the config file and store them in the list[context]*/
  PmLogFileStatus       logEntry;
  PmLogFileStatusTable  *list = getPmLogFileStatusTable();

  for ( numElemIndex=0; numElemIndex < list->maxElem; numElemIndex++ )
  {
    if ( 0 NOTEQ pmLogFileStatus[numElemIndex].index )
    {
      /* Check if the file size is not equal to 0, if 0, dont add in the list and delete the file */
      if ( 0 EQ isFileSizeNonZero( pmLogFileStatus[numElemIndex].fileName ) )
      {
        /* If file size is 0, remove the file */
        if ( 0 EQ remove(pmLogFileStatus[numElemIndex].fileName) )
          logInfo(" File [%s] removed from the pmLogs directory as file size was 0", pmLogFileStatus[numElemIndex].fileName );
      }
      else
      {
        clearBytes( &logEntry, sizeof(PmLogFileStatus));
        logEntry.index           = pmLogFileStatus[numElemIndex].index;
        logEntry.fileSizeInBytes = pmLogFileStatus[numElemIndex].fileSizeInBytes;
        copyBytes( pmLogFileStatus[numElemIndex].fileName, strlen(pmLogFileStatus[numElemIndex].fileName)+1, logEntry.fileName );
        copyBytes( pmLogFileStatus[numElemIndex].openedAt, strlen(pmLogFileStatus[numElemIndex].openedAt)+1, logEntry.openedAt );
        copyBytes( pmLogFileStatus[numElemIndex].closedAt, strlen(pmLogFileStatus[numElemIndex].closedAt)+1, logEntry.closedAt );

        listInsert(list, (void*)&logEntry, sizeof(PmLogFileStatus) );
        validEntries++;
      }
    }
  }
  logInfo(" [%d] files are already present in the PM log file persistent memory", validEntries );
  pmWritePmLogFileStatusTableToPersistentMemory( "PMH started. Read pmLogFile.cfg, valid entries taken and rest deleted. pmLogConfig updated" );
}

/**
 * @param   pmhContext PMH Context
 * @details This function will carry out Counter collection procedure completion steps for each iteration.
 * */
void doCounterCollectionProcCompletionSteps(
  IO PmhContext   *pmhContext
  )
{
  /* Stop Wait Counter Collection timer as procedure is completed */
  stopTimer(pmhContext->pmhTimersInfo.modTimerInfo[TIMER_INDEX_WAIT_COUNTER_COLLECTION].timerId);
  /* Update file size in the status table */
  pmhUpdatePmLogFileSize( pmhContext->currPmLogFileName );

  /* Write entries to persistent memory */
  pmWritePmLogFileStatusTableToPersistentMemory( "PM Log File status Updated - iteration completed" );

  /* Check if sample interval time was modified during this procedure in progress, if yes,
   * close the current file and re-open a new file */
  if( TRUE EQ pmhContext->isSampleIntervalOrFileCreationTimeModified )
  {
    pmhContext->isSampleIntervalOrFileCreationTimeModified = FALSE;
    pmhHandleSampleIntervalTimeChange(pmhContext);
  }
  /* Move to IDLE State */
  setPmhState( pmhContext, PMH_IDLE );
}

/**
 * @param   fileName File Name
 * @details This function checks if Statistics log file exists with given file name
 * */
TBOOL checkIfCurrentStsLogFileExists(
  UINT8    *fileName
  )
{
  struct stat fileCtx;
  clearBytes(&fileCtx, sizeof(struct stat));

  if ( -1 EQ stat( fileName, &fileCtx) )
    return FALSE;
  else
    return TRUE;
}

/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function processes OPSTOP message
 * */
PmhState pmhProcOpStopReqInWaitOpStart(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  /* System is configured, Yet to start,
   * Nothing to do, Send success and return same state
   */
  createAndSendMsg( MSG_PMH_BSM_OPSTOP_SUCC, NULL, 0);
  UNUSED(msg);
  UNUSED(pmhContext);
  return PMH_WAIT_OPSTART;
}

/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function processes OPSTOP message
 * */
PmhState pmhProcOpStopReqInWaitInitCfg(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  /* System is not configured,
   * Nothing to do, Send success and return same state
   */
  createAndSendMsg( MSG_PMH_BSM_OPSTOP_SUCC, NULL, 0);
  UNUSED(msg);
  UNUSED(pmhContext);
  return PMH_WAIT_INIT_CFG;
}


/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function processes OPSTOP message
 * */
PmhState pmhProcOpStopReq(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  UNUSED(msg);
  /* Stop all PM timers
   * Close the LogFile
   * Send OP_STOP Success
   * Send counterFile available message to BSM
   */
  stopallTimers();
  pmhClearAllCounters();

  if( TRUE EQ pmhContext->isCounterCollectionInProgress )
  {
    pmhClosePmLogFile(pmhContext);

    sendNotiToBsmIfFileIsNonZeroOrClearFileIfZero( pmhContext->currPmLogFileName );
  }

  createAndSendMsg( MSG_PMH_BSM_OPSTOP_SUCC, NULL, 0);
  setPmhState( pmhContext, PMH_WAIT_OPSTART );

  return pmhContext->pmhState;
}

/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function processes OPSTART message
 * */
PmhState pmhProcOpStartReq(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  UINT32      dirSize = 0;
  /* Reset the flag */
  pmhContext->isOpStartRcvd = TRUE;
  pmhContext->isCounterCollectionInProgress = FALSE;
  UNUSED(msg);

  /* There is nothing that PM does on receiving OPSTART_REQ, hence as of now, OPSTART_FAIL is not
   * implemented. Send OPSTART_SUCC immediately */
  createAndSendMsg( MSG_PMH_BSM_OPSTART_SUCC, NULL, 0);
  logInfo("%s","Sending MSG_PMH_TFU_DSTATS_CFG_REQ..");

  pmhClearAllCounters();

  /* Check 1 : If no Counters are to be collected, we Simply change the state*/
  if( FALSE EQ pmhIsAnyCounterEnabled() )
  {
    BS_ERROR_ALL_PM_GROUPS_DISABLED();
    pmhSendCntrCollectFailMsgToBsm( BS_ERR_ALL_PM_GROUPS_DISABLED, "All PM groups disabled" );
    setPmhState( pmhContext, PMH_IDLE );
    return pmhContext->pmhState;
  }

  /* Check 2 : Send message counter collection failed to BSM, if file size exceeds */
  if( TRUE EQ pmhHasPmLogDirSizeExceeded( pmhContext, &dirSize ) )
  {
    BS_ERROR_COMBINED_STAT_FILE_SIZE_EXCEEDED( pmhContext->maxCombinedPmFileSize );
    pmhSendCntrCollectFailMsgToBsm( BS_ERR_COMBINED_STAT_FILE_SIZE_EXCEEDED, "Combined statistics File-Size Exceeded" );
    setPmhState( pmhContext, PMH_IDLE );
    return pmhContext->pmhState;
  }

  /* Check 3 : Send message counter collection failed to BSM, if total number of statistics files reached threshold */
  if ( FALSE EQ listHasSpace(getPmLogFileStatusTable()) )
  {
    BS_ERROR_ALL_PMLOGFILESTATUS_ROWS_FILLED("PmLogFileStatus");
    pmhSendCntrCollectFailMsgToBsm( BS_ERR_NUM_STS_FILES_REACH_THRESHOLD, "Total number of statistics files reached threshold" );
    setPmhState( pmhContext, PMH_IDLE );
    return pmhContext->pmhState;
  }

  if ( TRUE EQ pmhOpenFileAndStartTimers(pmhContext))
    pmhContext->isCounterCollectionInProgress = TRUE;

  setPmhState( pmhContext, PMH_IDLE );
  return pmhContext->pmhState;
}

/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function processes FILE UPLOADED message from BSM, when PMH is in IDLE state
 * */
PmhState pmhProcPmFileUploadedMsgInIdleState(
    IN Msg           *msg,
    IO PmhContext    *pmhContext
    )
{
  MsgBsmPmhPmFileUploaded *fileUploadedInd = (MsgBsmPmhPmFileUploaded *)msg->data;
  pmhClearPmLogFileStatusRow( fileUploadedInd->pmLogFileName );

  /* We need to check whether statistics gathering was blocked earlier, 
     due to file threshold having reached or directory size. If this is the case PMH
     should start PM data collection as soon as space is available. */
  if( FALSE EQ pmhContext->isCounterCollectionInProgress )
  {
        logInfo("%s","Restarting Counter collection upon PM log file removal." );
	pmhClearAllCounters();

	/* Check: If no Counters are to be collected */
  	if( FALSE EQ pmhIsAnyCounterEnabled() )
  	{
    		return pmhContext->pmhState;
  	}

	if ( TRUE EQ pmhOpenFileAndStartTimers(pmhContext))
	{
    		pmhContext->isCounterCollectionInProgress = TRUE;
	}
  }
  else
  {
	logInfo("%s","Counter collection is in progress, NOT DOING ANYTHING UPON PM LOG FILE UPLOAD.");
  }

  return pmhContext->pmhState;
}

/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function processes CONFIG_REQ message
 * */
PmhState pmhProcConfigReqInWaitInitAndWaitOpstart(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  UINT32 i;

  MsgBsmPmhConfigReq *configReq = (MsgBsmPmhConfigReq *)msg->data;
  MsgPmhBsmConfigSucc configSucc;
  clearBytes(&configSucc, sizeof(MsgPmhBsmConfigSucc));

  for (i=0;i<MAX_ROWS_IN_COUNTERCONFIG_TABLE;i++)
  {
    copyBytes(&(configReq->counterConfig[i]), sizeof(CounterConfig), &(pmhContext->counterContext[i].l2CounterConfig));
  }

  /** 1. Send CONFIG_SUCC success
   *  2. Store the received values in the context.
   **/
  configSucc.transId = configReq->transId;
  createAndSendMsg( MSG_PMH_BSM_CONFIG_SUCC, (UINT8*)&configSucc, sizeof(MsgPmhBsmConfigSucc));

  pmhContext->pmCounterLogConfig = configReq->pmCounterLogConfig;
  pmhContext->enodebId           = configReq->enodebId;

  return pmhContext->pmhState;
}

/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function processes CONFIG_REQ message in IDLE state
 * */
PmhState pmhProcConfigReqInIdleState(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  UINT32 i;

  MsgBsmPmhConfigReq *configReq = (MsgBsmPmhConfigReq *)msg->data;
  MsgPmhBsmConfigSucc configSucc;
  clearBytes(&configSucc, sizeof(MsgPmhBsmConfigSucc));

  for (i=0;i<MAX_ROWS_IN_COUNTERCONFIG_TABLE;i++)
  {
    copyBytes(&(configReq->counterConfig[i]), sizeof(CounterConfig), &(pmhContext->counterContext[i].l2CounterConfig));
  }

  /** 1. Send config success
   *  2. Check if the sample interval timer / file creation timer value was changed, if yes,
   *     a. Delete the current logging file
   *     b. Send notification that file available
   *     c. Create a new file and update currLoggingFile in the PMH context.
   *        Further logging will be done in this newly created file.
   *  3. Store the received values in the context.
   **/
  configSucc.transId = configReq->transId;
  createAndSendMsg( MSG_PMH_BSM_CONFIG_SUCC, (UINT8*)&configSucc, sizeof(MsgPmhBsmConfigSucc));

  if(TRUE EQ pmhContext->isCounterCollectionInProgress)
  {
    logInfo("%s","Counter collection in progress " );
    if ( (configReq->pmCounterLogConfig.sampleInterval NOTEQ pmhContext->pmCounterLogConfig.sampleInterval) ||
         (configReq->pmCounterLogConfig.pmFileCreationInterval NOTEQ pmhContext->pmCounterLogConfig.pmFileCreationInterval))
    {
      pmhContext->pmCounterLogConfig = configReq->pmCounterLogConfig;
      pmhContext->enodebId           = configReq->enodebId;
      /** Do Step 2 here */
      pmhHandleSampleIntervalTimeChange( pmhContext );
      return pmhContext->pmhState;
    }

    pmhContext->pmCounterLogConfig = configReq->pmCounterLogConfig;
    pmhContext->enodebId           = configReq->enodebId;
  }
  else
  {
    logInfo("%s","Counter collection not in progress " );

    pmhContext->pmCounterLogConfig = configReq->pmCounterLogConfig;
    pmhContext->enodebId           = configReq->enodebId;

    /* Check: If no Counters are to be collected */
    if( FALSE EQ pmhIsAnyCounterEnabled() )
    {
      return pmhContext->pmhState;
    }
    logInfo("%s","Restarting Counter collection upon dynamic configuration change " );
    pmhClearAllCounters();

    if ( TRUE EQ pmhOpenFileAndStartTimers(pmhContext))
    {
      pmhContext->isCounterCollectionInProgress = TRUE;
    }
  }

  return pmhContext->pmhState;
}

/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function processes CONFIG_REQ message
 * */
PmhState pmhProcConfigReqInWaitBulkCollection(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  UINT32 i;
  MsgBsmPmhConfigReq *configReq = (MsgBsmPmhConfigReq *)msg->data;
  MsgPmhBsmConfigSucc configSucc;
  clearBytes(&configSucc, sizeof(MsgPmhBsmConfigSucc));

  for (i=0;i<MAX_ROWS_IN_COUNTERCONFIG_TABLE;i++)
  {
    copyBytes(&(configReq->counterConfig[i]), sizeof(CounterConfig), &(pmhContext->counterContext[i].l2CounterConfig));
  }

  /** 1. Send opstart success
   *  2. Check if the sample interval timer / file creation timer value was changed, if yes, mark this change in the flag.
   *     This is done, because PMH in this state is waiting for the response from all modules.
   *     So delete the file and create new one ONLY after this sample interval counter collection iteration is over.
   *  3. Store the received values in the context.
   **/
  configSucc.transId = configReq->transId;
  createAndSendMsg( MSG_PMH_BSM_CONFIG_SUCC, (UINT8*)&configSucc, sizeof(MsgPmhBsmConfigSucc));

  pmhContext->isSampleIntervalOrFileCreationTimeModified = FALSE;

  if ( (configReq->pmCounterLogConfig.sampleInterval NOTEQ pmhContext->pmCounterLogConfig.sampleInterval) || 
       (configReq->pmCounterLogConfig.pmFileCreationInterval NOTEQ pmhContext->pmCounterLogConfig.pmFileCreationInterval))
  {
    pmhContext->isSampleIntervalOrFileCreationTimeModified = TRUE;
  }

  pmhContext->pmCounterLogConfig = configReq->pmCounterLogConfig;
  pmhContext->enodebId           = configReq->enodebId;

  return pmhContext->pmhState;
}

/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function processes COUNTER_INPUT_CONFIG_REQ message
 * */
PmhState pmhProcCntrInputConfigReq(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  MsgBsmPmhCntrInputConfigReq *configReq = (MsgBsmPmhCntrInputConfigReq *)msg->data;
  MsgPmhBsmCntrInputConfigSucc configSucc;

  clearBytes(&configSucc, sizeof(MsgPmhBsmCntrInputConfigSucc));

  configSucc.transId = configReq->transId;
  createAndSendMsg( MSG_PMH_BSM_COUNTER_INPUT_CONFIG_SUCC, (UINT8*)&configSucc, sizeof(MsgPmhBsmCntrInputConfigSucc));

  pmhContext->cellId   = configReq->cellId;
  pmhContext->peerId   = configReq->peerId;

  /* This will clear stored counter tables at PMH. */
  pmhClearAllCounters();

  setPmhState( pmhContext, PMH_WAIT_OPSTART );

  return pmhContext->pmhState;
}

/**
 * @param   sampleInterval			sampleInterval enum value
 * @details This function returns actual duration of sampleInterval based on enum values
 * */
Duratn pmhGetSampleIntervalTimeVal(
  IN SampleInterval sampleInterval
  )
{
  switch(sampleInterval)
  {
    case SAMPLEINTERVAL_1MIN:  return 1;
    case SAMPLEINTERVAL_5MIN:  return 5;
    case SAMPLEINTERVAL_10MIN: return 10;
    case SAMPLEINTERVAL_15MIN: return 15;
    case SAMPLEINTERVAL_30MIN: return 30;
    case SAMPLEINTERVAL_1H:    return 60;
  }
  logInfo("%s", "Sample interval did not matched with any of the value- Coding error..Check !! ");
  return 0;
}

/**
 * @param   Interval			pmFileCreationInterval enum value
 * @details This function returns actual duration of pmFileCreationInterval based on enum values
 * */
Duratn pmhGetPmLogFileCreationTimeVal(
  IN PmFileCreationInterval Interval
  )
{
  switch(Interval)
  {
    case PMFILECREATIONINTERVAL_1H:  return 60;
    case PMFILECREATIONINTERVAL_2H:  return 120;
    case PMFILECREATIONINTERVAL_4H:  return 240;
    case PMFILECREATIONINTERVAL_8H:  return 480;
  }
  logInfo("%s", "PM Log File Creation Interval did not matched with any of the value- Coding error..Check !! ");
  return 0;
}

/**
 * @param   timerIndex			Timer Index
 * @details This function starts timer with given index
 * */
TimerId pmhStartTimer(
  IN UINT8   timerIndex,
  IO UINT32  userInfo,
  IO UINT32  duration
  )
{
  TimerInfo  *timerToStart = getPmhTimersInfo().modTimerInfo[timerIndex];

  if((timerIndex EQ TIMER_INDEX_SAMPLE_INTERVAL)||
     (timerIndex EQ TIMER_INDEX_PM_LOGFILE_CREATE_INTERVAL)||
     (timerIndex EQ TIMER_INDEX_WAIT_L2PMREPORTPROFILE1)||
     (timerIndex EQ TIMER_INDEX_WAIT_L2PMREPORTPROFILE2))
  {
    timerToStart->timerDesc.timeSpec.duration = duration;
  }

  timerToStart->timerDesc.userInfo = userInfo;

  /* Start Timer */
  timerToStart->timerId = createAndStartTimer( &timerToStart->timerDesc, NULL, NULL );
  return timerToStart->timerId;
}

/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function sends HELLO_ACK message
 */
PmhState pmhProcHelloReq(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  MsgPrmModHello     *hello = (MsgPrmModHello*)msg->data;
  MsgModPrmHelloAck   helloAck;
  clearBytes( &helloAck, sizeof(MsgModPrmHelloAck) );
  helloAck.helloAckSeqNum = hello->helloSeqNum;

  createAndSendMsg( MSG_PMH_PRM_HELLO_ACK, (UINT8*)&helloAck, sizeof(MsgModPrmHelloAck) );

  return pmhContext->pmhState;
}

/**
 * @param   msg Message received
 * @param   pmhContext PMH Context
 * @details This function handles exit message from PRM
 *          if counters been collecting, it will close the counters file and then exit gracefully.
 */
PmhState pmhProcExit(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  UNUSED(msg);
  pmhClosePmLogFile(pmhContext);

  modExit(MODID_PMH, &(getModuleContext()->exeName[0]));
  exit(EXIT_SUCCESS);
}


/**
 * @param   msg         Message received
 * @param   pmhContext PMH Context
 * @details This function processes COUNTER_CLEAR_REQ message
 * */
PmhState pmhProcCounterClearReqInWaitInitAndWaitOpstart(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  BsErrorValue  error;
  UINT32        transId;
  MsgBsmPmhCounterClearReq    *clearReq = (MsgBsmPmhCounterClearReq *)msg->data;

  clearBytes( &pmhContext->sendClearCounterReq, sizeof(CounterFlags));
  clearBytes( &pmhContext->rcvdClearCounterRsp, sizeof(CounterFlags));

  /* Clear all the counter values in the protocol stacks. */
  pmhContext->resetAllCounters = TRUE;

  /* We need to reset all counters, help set these fields so that
   * Request is sent to all Protocol stacks */
  pmhContext->sendClearCounterReq.macGenCounter       = TRUE;
  pmhContext->sendClearCounterReq.macUplaneCounter    = TRUE;
  pmhContext->sendClearCounterReq.rlcGenCounter       = TRUE;
  pmhContext->sendClearCounterReq.pdcpGenCounter      = TRUE;
  pmhContext->sendClearCounterReq.pdcpUplaneCounter   = TRUE;
  pmhContext->sendClearCounterReq.gtpGenCounter       = TRUE;
  pmhContext->sendClearCounterReq.sctpGenCounter      = TRUE;
  pmhContext->sendClearCounterReq.s1apGenCounter      = TRUE;
  pmhContext->sendClearCounterReq.rrcGenCounter       = TRUE;
  pmhContext->sendClearCounterReq.rrcCellCounter      = TRUE;
  pmhContext->sendClearCounterReq.rrmCounter          = TRUE;

  /*TODO this code will be removed when crashing problem of cp.exe on
   * GET on S1APPEERNODECOUNTERS before first UNLOCK resolved
   * */
  if(TRUE EQ pmhContext->isOpStartRcvd)
  {
    pmhContext->sendClearCounterReq.s1apPeerNodeCounter = TRUE;
  }

  error = pmhAllocTransId(&clearReq->cmdRef, NULL, &transId, pmhContext);
  if(BS_NO_ERROR NOTEQ error)
  {
    return pmhContext->pmhState;
  }
  sendCounterResetReqToAllModule(pmhContext, transId);

  if( NULL_TIMER_ID EQ pmhStartTimer(TIMER_INDEX_WAIT_COUNTER_RESET, transId, 0) )
  {
    BS_ERROR_PMH_TIMER_NOT_STARTED("SampleInterval");
    return pmhContext->pmhState;
  }

  return pmhContext->pmhState;
}

/**
 * @param   msg         Message received
 * @param   pmhContext PMH Context
 * @details This function processes DIAG_COUNTER_CMD message
 * */
PmhState pmhProcDiagCounterCmdInAllState(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  )
{
  MsgBsmPmhDiagCounterCmd    *diagReq = (MsgBsmPmhDiagCounterCmd *)msg->data;
  MsgPmhBsmDiagCounterCmdRsp  diagRsp;
  UINT32                      cmd; 
  clearBytes(&diagRsp, sizeof(MsgPmhBsmDiagCounterCmdRsp));

  copyBytes(&diagReq->cmdRef, sizeof(CmdRef),&diagRsp.cmdRef);
  diagRsp.eNbCommand = diagReq->eNbCommand;

  createAndSendMsg( MSG_PMH_BSM_DIAG_COUNTER_CMD_CMPL, (UINT8*)&diagRsp, sizeof(MsgPmhBsmDiagCounterCmdRsp));
 
  if (diagReq->eNbCommand EQ ENBCOMMAND_STARTDIAG)
  cmd = STCMD_START;
  else 
  cmd = STCMD_STOP;

  createAndSendUdhDStatReq(cmd, 1000);
  createAndSendTfuDstatsCfgReq (cmd,1000);

  return pmhContext->pmhState;
}

void pmhProcClearCounterRsp(
  IO PmhContext    *pmhContext,
  IO UINT32         transId
  )
{
  MsgPmhBsmCounterClearSucc   succ;
  CmdRef   cmdRef;
  TagInfo  tagInfo;

  clearBytes(&succ, sizeof(MsgPmhBsmCounterClearSucc));
  clearBytes(&cmdRef, sizeof(CmdRef));
  clearBytes(&tagInfo, sizeof(TagInfo));

  if(TRUE EQ pmhStsRspRcvdFromAllModules( &pmhContext->sendClearCounterReq, &pmhContext->rcvdClearCounterRsp ) )
  {
    pmhContext->resetAllCounters = FALSE;
    clearBytes( &pmhContext->sendClearCounterReq, sizeof(CounterFlags));
    clearBytes( &pmhContext->rcvdClearCounterRsp, sizeof(CounterFlags));

    stopTimer(pmhContext->pmhTimersInfo.modTimerInfo[TIMER_INDEX_WAIT_COUNTER_RESET].timerId);
    if(BS_NO_ERROR NOTEQ getCmdRefByTransIdAndDeallocTransId( transId, &cmdRef, &tagInfo, pmhContext))
    {
      return;
    }
    /* Send COUNTER_CLEAR_CFM to BSM */
    copyBytes(&cmdRef, sizeof(CmdRef), &succ.cmdRef);
    createAndSendMsg( MSG_PMH_BSM_COUNTER_CLEAR_SUCC, (UINT8*)&succ, sizeof(MsgPmhBsmCounterClearSucc));
  }
  return;
}

void pmhInitCounterCollectionStatusTable()
{
  CounterCollectionStatus  *rowOfTable = (CounterCollectionStatus*)getFirstRowOfTable(COUNTERCOLLECTIONSTATUS);

  rowOfTable->index = 1;
  rowOfTable->profileId = PROFILEID_PROFILE1;
  rowOfTable->isRunning = FALSE;
  rowOfTable->measurementDuration = 0;
  rowOfTable->reportingDuration =0;
  rowOfTable->numOfSamplesCollected =0;
  rowOfTable->numOfamplesRemaining =0;

  snprintf((char *)&rowOfTable->pmLogFileName, MAX_SIZE_COUNTERCOLLECTIONSTATUS_PMLOGFILENAME + 1, "%s", "None");
  snprintf((char *)&rowOfTable->startedAt, MAX_SIZE_COUNTERCOLLECTIONSTATUS_STARTEDAT + 1, "%s", "00000000000000000");
  snprintf((char *)&rowOfTable->stoppedAt, MAX_SIZE_COUNTERCOLLECTIONSTATUS_STOPPEDAT + 1, "%s", "00000000000000000");
  snprintf((char *)&rowOfTable->stoppedReason, MAX_SIZE_COUNTERCOLLECTIONSTATUS_STOPPEDREASON + 1, "%s", "None");

  getNextRowOfTable(rowOfTable);

  rowOfTable->index = 2;
  rowOfTable->profileId = PROFILEID_PROFILE2;
  rowOfTable->isRunning = FALSE;
  rowOfTable->measurementDuration = 0;
  rowOfTable->reportingDuration =0;
  rowOfTable->numOfSamplesCollected =0;
  rowOfTable->numOfamplesRemaining =0;

  snprintf((char *)&rowOfTable->pmLogFileName, MAX_SIZE_COUNTERCOLLECTIONSTATUS_PMLOGFILENAME + 1, "%s", "None");
  snprintf((char *)&rowOfTable->startedAt, MAX_SIZE_COUNTERCOLLECTIONSTATUS_STARTEDAT + 1, "%s", "00000000000000000");
  snprintf((char *)&rowOfTable->stoppedAt, MAX_SIZE_COUNTERCOLLECTIONSTATUS_STOPPEDAT + 1, "%s", "00000000000000000");
  snprintf((char *)&rowOfTable->stoppedReason, MAX_SIZE_COUNTERCOLLECTIONSTATUS_STOPPEDREASON + 1, "%s", "None");
}

void pmhInitCounterContext(
  IO PmhContext  *pmhContext
  )
{
  UINT32 index;
  for (index = 0; index< MAX_ROWS_IN_L2COUNTERS_TABLE; index++)
  {
    pmhContext->counterContext[index].value = 0;
    snprintf((char *)&pmhContext->counterContext[index].lastCollectedAt, MAX_SIZE_L2COUNTERS_LASTCOLLECTEDAT + 1, "%s", "00000000000000000");
  }
}

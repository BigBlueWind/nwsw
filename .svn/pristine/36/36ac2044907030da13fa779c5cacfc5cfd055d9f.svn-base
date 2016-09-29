/**
 * @file    bsm_alarm_func.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions related to handling of alarm, update /delete of alarm
 *          entries from the alarm table's.
 *
 * @author:  Shweta Polepally
 * @date:
 * @details: Initial Version
 **/

#include "bsm.h"

/***************************************************/
/************ALARM HANDLING*************************/
/***************************************************/

/**
 * @details This function returns the address of the global AFH context.
 **/
AlarmContext *getAlarmContext( void )
{
  return &(g_BsmContext.afhCtx);
}
/**
 * @details This function intialize default values to AlarmHistoryStatusTable structure members.
 **/
void initAlarmHistoryStatusTable(AlarmHistoryStatusTable *circularBuf)
{
  circularBuf->front       = -1;
  circularBuf->rear        = -1;
  circularBuf->numOfElem   =  0;
  circularBuf->isTableFull = FALSE;

}
ActiveAlarmStatusTable *getActiveAlarmStatusTable( void )  /* Linked List implementation */
{
  return &(getAlarmContext()->activeAlarmStatusTable);
}

/**
 * @details This function returns the address of the global Active History status table context.
 **/
AlarmHistoryStatusTable  *getAlarmHistoryStatusTable( void )  /* Circular Buffer */
{
  return &(getAlarmContext()->alarmHistoryStatusTable );
}

/**
 * @details This function initialises the Alarm context.
 **/
void alarmInitCtx(
  IO AlarmContext *alarmCtx
  )
{
  clearBytes( alarmCtx, sizeof(AlarmContext));

  alarmCtx->alarmHistoryStatusTable.rear  = -1;
  alarmCtx->alarmHistoryStatusTable.front = -1;

  /* Allocate memory for alarmActive Table */
  listInit( MAX_ROWS_IN_ACTIVEALARMSTATUS_TABLE, sizeof(ActiveAlarmStatus), &alarmCtx->activeAlarmStatusTable );
}

/**
 * @details This function will return the number of active Critical alarms currently present in
 *          enodeB.
 **/
UINT32 getNumActiveCriticalAlarms()
{
  ActiveAlarmStatus    *activeEntry;
  ListNode             *node;
  List                 *list = getActiveAlarmStatusTable();
  UINT32                numOfActiveCriticalAlarms=0;
  node = list->first;
  while (node)
  {
    activeEntry = (ActiveAlarmStatus*)node->data;
    if ( activeEntry->alarmSeverity EQ  ALARMSEVERITY_CRITICAL )
    {
      numOfActiveCriticalAlarms++;
    }
    node = node->next;
  }
  return numOfActiveCriticalAlarms;
}

/**
 * @details This function will return the number of active Major alarms currently present in
 *          enodeB.
 **/
UINT32 getNumActiveMajorAlarms()
{
  ActiveAlarmStatus    *activeEntry;
  ListNode             *node;
  List                 *list = getActiveAlarmStatusTable();
  UINT32                numOfActiveMajorAlarms=0;
  node = list->first;
  while (node)
  {
    activeEntry = (ActiveAlarmStatus*)node->data;
    if ( activeEntry->alarmSeverity EQ  ALARMSEVERITY_MAJOR )
    {
      numOfActiveMajorAlarms++;
    }
    node = node->next;
  }
  return numOfActiveMajorAlarms;
}

/**
 * @details This function will return number of LINKDOWN S1AP alarms.
 **/
UINT32 getNumS1apActiveLinkDownAlarms()
{
  ActiveAlarmStatus    *activeEntry;
  ListNode             *node;
  List                 *list = getActiveAlarmStatusTable();
  UINT32                numOfS1apLinkDownAlarms=0;
  node = list->first;
  while (node)
  {
    activeEntry = (ActiveAlarmStatus*)node->data;
    if ( MMELINKDOWN EQ activeEntry->alarmCause )
    {
      numOfS1apLinkDownAlarms++;
    }
    node = node->next;
  }
  return numOfS1apLinkDownAlarms;
}

/**
 * @details This function will return number of LINKDOWN S1AP alarms.
 **/
UINT32 getNumModDownAlarms()
{
  ActiveAlarmStatus    *activeEntry;
  ListNode             *node;
  List                 *list = getActiveAlarmStatusTable();
  UINT32                numOfModDownAlarms=0;
  node = list->first;
  while (node)
  {
    activeEntry = (ActiveAlarmStatus*)node->data;
    if ( (SWCRASH EQ activeEntry->alarmId) || (NORESPONSETOHEALTHCHECK EQ activeEntry->alarmId) )
    {
      numOfModDownAlarms++;
    }
    node = node->next;
  }
  return numOfModDownAlarms;
}

/**
 * @param   alarmDetails CreateAlarmDetails Structure
 * @details This function checks if the active alarm already exists in Active alarm table.
 *           Returns TRUE if yes, else FALSE
 **/
TBOOL checkIfActiveAlarmExists(
  IO CreateAlarmDetails  *alarmDetails
  )
{
  ActiveAlarmStatus    *tab;
  ListNode             *node;
  List                 *list = getActiveAlarmStatusTable();
  node = list->first;
  while (node)
  {
    tab = (ActiveAlarmStatus*)node->data;
    /** Compare : 1. AlarmCause
     *            2. AlarmType
     *            3. AlarmLocation */
    if ( (alarmDetails->alarmCause EQ tab->alarmCause) && (alarmDetails->alarmType EQ tab->alarmType)
        && (0 EQ strncmp(alarmDetails->alarmLocation, tab->alarmLocation, strlen(alarmDetails->alarmLocation))) )
      return TRUE;
    node = node->next;
  }
  return FALSE;
}

/**
 * @details This function returns next Notification Sequence Number
 **/
UINT32  getNextAlarmSequenceNumber( void )
{
  AlarmContext  *ctx = getAlarmContext();
  ctx->alarmSequenceNumber++;
  /* Return incremented value of notificationSequenceNumber */
  return ctx->alarmSequenceNumber;
}

/**
 * @details This function returns the next notification Sequence Number
 **/
UINT32  getNextNotificationSequenceNumber( void )
{
  AlarmContext  *ctx = getAlarmContext();
  ctx->notificationSequenceNumber++;
  /* Return incremented value of notificationSequenceNumber */
  return ctx->notificationSequenceNumber;
}

/**
 * @details This function returns if the alarm needs to be posted to SIH or not.
 *          This is a dummy function as of now. Always returns TRUE as of now.
 **/
static TBOOL postToEms()
{
  return TRUE;
}

/**
 * @param   eventMessage   CreateAlarmDetails Structure
 * @param   ind            MsgBsmSihAlarmNotification Message
 * @details This function fills MsgBsmSihAlarmNotification structure.
 **/
static void fillMsgBsmSihAlarmNotification(
  IO CreateAlarmDetails         *eventMessage,
  IO MsgBsmSihAlarmNotification *ind
  )
{
  ind->enodebId              = getEnodeBId();
  ind->alarmSequenceNumber   = eventMessage->alarmSequenceNumber;
  snprintf( ind->dateAndTimeStamp, strlen(eventMessage->dateAndTimeStamp)+1, "%s", eventMessage->dateAndTimeStamp );
  ind->alarmType             = eventMessage->alarmType;
  ind->alarmId               = eventMessage->alarmId;
  ind->alarmSeverity         = eventMessage->alarmSeverity;
  ind->alarmCause            = eventMessage->alarmCause;
  snprintf( ind->alarmLocation, strlen(eventMessage->alarmLocation)+1, "%s", eventMessage->alarmLocation );
  snprintf( ind->alarmAdditionalText, strlen(eventMessage->alarmAdditionalText)+1, "%s", eventMessage->alarmAdditionalText );
}

/**
 * @param   alarmDetails   CreateAlarmDetails Structure
 * @details This function will post active alarm to SIH based on the type of notification.
 **/
static void postActiveAlarmMsgToSih(
  IO CreateAlarmDetails  *alarmDetails
  )
{
  MsgBsmSihAlarmNotification ind;
  NotiObj                    notiMsg;
  EncBuf                     encBuf;
  clearBytes( &ind, sizeof(MsgBsmSihAlarmNotification) );
  clearBytes( &notiMsg, sizeof(NotiObj));
  initEncBuf( &encBuf );

  fillMsgBsmSihAlarmNotification( alarmDetails, &ind  );
  convertStruct2TlvAlarmNotification( &ind, &notiMsg );
  encNotiObjMsg( &notiMsg, &encBuf );

  createAndSendMsg( MSG_BSM_SIH_NOTI_IND, (UINT8*)&encBuf.value, encBuf.len );
}

/**
 * @details This function post all the active alarms to SIH after registration is completed.
 **/
void postActiveAlarmsToSihIfAny()
{
  CreateAlarmDetails   alarmDetails;
  ActiveAlarmStatus    *activeEntry = NULL;
  ListNode             *node = NULL;
  List                 *list = getActiveAlarmStatusTable();

  if ( 0 EQ list->numElemPresent )
    return;

  node = list->first;
  while (node)
  {
    clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));
    activeEntry = (ActiveAlarmStatus*)node->data;

    alarmDetails.alarmSequenceNumber   = activeEntry->alarmSequenceNumber;
    alarmDetails.alarmNotificationType = ALARMNOTIFICATIONTYPE_OCCURRED;
    snprintf( alarmDetails.dateAndTimeStamp, strlen(activeEntry->alarmDateAndTimeStamp)+1, "%s", activeEntry->alarmDateAndTimeStamp );
    alarmDetails.alarmType             = activeEntry->alarmType;
    alarmDetails.alarmId               = activeEntry->alarmId;
    alarmDetails.alarmSeverity         = activeEntry->alarmSeverity;
    alarmDetails.alarmCause            = activeEntry->alarmCause;
    snprintf( alarmDetails.alarmLocation, strlen(activeEntry->alarmLocation)+1, "%s", activeEntry->alarmLocation );
    snprintf( alarmDetails.alarmAdditionalText, strlen(activeEntry->alarmAdditionalText)+1, "%s", activeEntry->alarmAdditionalText );

    postActiveAlarmMsgToSih( &alarmDetails );

    node = node->next;
  }
}

/**
 * @param   eventMessage   CreateAlarmDetails Structure
 * @details This function saves active alarm in ActiveAlarmStatus Table.
 **/
static void saveAlarmInActiveAlarmTable(
  IO CreateAlarmDetails  *eventMessage
  )
{
  TBOOL                 entryAdded = FALSE;
  List                  *list = getActiveAlarmStatusTable();
  ActiveAlarmStatus     activeAlarmEntry;
  clearBytes( &activeAlarmEntry, sizeof(ActiveAlarmStatus));

  activeAlarmEntry.alarmSequenceNumber = eventMessage->alarmSequenceNumber;
  snprintf( activeAlarmEntry.alarmDateAndTimeStamp, strlen(eventMessage->dateAndTimeStamp)+1, "%s", eventMessage->dateAndTimeStamp );
  activeAlarmEntry.alarmType     = eventMessage->alarmType;
  activeAlarmEntry.alarmId       = eventMessage->alarmId;
  activeAlarmEntry.alarmSeverity = eventMessage->alarmSeverity;
  activeAlarmEntry.alarmCause    = eventMessage->alarmCause;
  snprintf( activeAlarmEntry.alarmLocation, strlen(eventMessage->alarmLocation)+1, "%s", eventMessage->alarmLocation );
  if ( NULL NOTEQ eventMessage->alarmAdditionalText )
    snprintf( activeAlarmEntry.alarmAdditionalText, strlen(eventMessage->alarmAdditionalText)+1, "%s", eventMessage->alarmAdditionalText );
  entryAdded = listInsert(list, (void*)&activeAlarmEntry, sizeof(ActiveAlarmStatus) );
  if ( entryAdded EQ TRUE )
  {
    logInfo("ActiveAlarmStatus: Added entry. Total alarms Active are: %d ", list->numElemPresent );
    logInfo("Alarm Details :\n{\n alarmSequenceNumber=%d\n DateAndTime=%s\n alarmType=%s\n alarmId=%d\n alarmSeverity=%s \n alarmCause=%s\n alarmLocation=%s\n alarmText=%s\n}\n",
        activeAlarmEntry.alarmSequenceNumber, activeAlarmEntry.alarmDateAndTimeStamp, alarmTypeStr(activeAlarmEntry.alarmType), activeAlarmEntry.alarmId, alarmSeverityStr(activeAlarmEntry.alarmSeverity), alarmCauseStr(activeAlarmEntry.alarmCause), activeAlarmEntry.alarmLocation, activeAlarmEntry.alarmAdditionalText );
  }
  else
  {
    BS_ERROR_ALARM_ACTIVE_TABLE_FULL( activeAlarmEntry.alarmCause );
  }
}

/**
 * @details This function checks enbStatus and send LED colour request.
 **/
static void setLed4Color()
{
  EnbStatus   *enbStatus  = (EnbStatus *)getFirstRowOfTable(ENBSTATUS);
  BsmContext  *bsmContext = getBsmContext();

  /* EnodeB is Locked - In this case, LED will always to AMBER */
  if ( ADMINSTATE_LOCKED EQ enbStatus->adminState )
  {
    bsmCreateAndSendSetLedColor( LEDID_ENBOPERATIONAL, LEDCOLOR_AMBER, bsmContext );
  }
  else  /* EnodeB is Unlocked - Enabled or Disabled */
  {
    /* If any critical alarms are present, set the LED color to RED */
    if( 0 NOTEQ getNumActiveCriticalAlarms() )
    {
      bsmCreateAndSendSetLedColor( LEDID_ENBOPERATIONAL, LEDCOLOR_RED, bsmContext );
    }
    else  /* If not alarms are present and enodeb is operation, set this color to GREEN */
    {
      if( OPERATIONALSTATE_ENABLED EQ enbStatus->operationalState )
      {
        bsmCreateAndSendSetLedColor( LEDID_ENBOPERATIONAL, LEDCOLOR_GREEN, bsmContext );
      }
      else /* TODO: need to check what has to done when ENODEB is Unlocked, disabled and no critical alarms present */
      {
        bsmCreateAndSendSetLedColor( LEDID_ENBOPERATIONAL, LEDCOLOR_AMBER, bsmContext );
      }
    }
  }
}

/**
 * @param   eventMessage   CreateAlarmDetails Structure
 * @details This function saves alarm in AlarmHistoryStatus Table.
 **/
static void saveAlarmInAlarmHistoryTable(
  IO CreateAlarmDetails  *eventMessage
  )
{
  AlarmHistoryStatusTable    *tab = getAlarmHistoryStatusTable();

  tab->rear++;
  tab->rear = tab->rear % MAX_ROWS_IN_ALARMHISTORY_TABLE;
  if ( (tab->front EQ tab->rear ) || (tab->isTableFull EQ TRUE))
  {
    tab->isTableFull = TRUE;
    logInfo("%s","ALARMHISTORY_TABLE is full !! Oldest entries will be replaced with the new entries");
  }

  tab->alarmHistoryEntry[tab->rear].alarmNotificationType = eventMessage->alarmNotificationType;
  tab->alarmHistoryEntry[tab->rear].alarmSequenceNumber   = eventMessage->alarmSequenceNumber;
  snprintf( tab->alarmHistoryEntry[tab->rear].alarmDateAndTimeStamp, strlen(eventMessage->dateAndTimeStamp)+1, "%s", eventMessage->dateAndTimeStamp );
  tab->alarmHistoryEntry[tab->rear].alarmType     = eventMessage->alarmType;
  tab->alarmHistoryEntry[tab->rear].alarmId       = eventMessage->alarmId;
  tab->alarmHistoryEntry[tab->rear].alarmSeverity = eventMessage->alarmSeverity;
  tab->alarmHistoryEntry[tab->rear].alarmCause    = eventMessage->alarmCause;
  snprintf( tab->alarmHistoryEntry[tab->rear].alarmLocation, strlen(eventMessage->alarmLocation)+1, "%s", eventMessage->alarmLocation );
  if ( NULL NOTEQ eventMessage->alarmAdditionalText )
    snprintf( tab->alarmHistoryEntry[tab->rear].alarmAdditionalText, strlen(eventMessage->alarmAdditionalText)+1, "%s", eventMessage->alarmAdditionalText );

  /* Increase the number of elements present in the table if the table is not full */
  if (tab->isTableFull EQ FALSE)
    tab->numOfElem++;

  logInfo("AlarmHistoryStatus: Added entry. Total alarms entries in history table are: %d ", tab->numOfElem );

  if( tab->front EQ -1 )
    tab->front = 0;
  setLed4Color();
}

/**
 * @param   eventMessage   CreateAlarmDetails Structure
 * @details This function deletes active alarm from activeAlarmStatus Table.
 **/
static void deleteAlarmInActiveAlarmTable(
  IO CreateAlarmDetails   *eventMessage
  )
{
  ActiveAlarmStatus    *activeEntry = NULL;
  ListNode             *node = NULL;
  ListNode             *prev = NULL;
  List                 *list = getActiveAlarmStatusTable();
  node = list->first;
  while (node)
  {
    activeEntry = (ActiveAlarmStatus*)node->data;
    if ( (eventMessage->alarmCause EQ activeEntry->alarmCause) &&
         (eventMessage->alarmType  EQ activeEntry->alarmType ) )
    {
      listDeleteByNode( list, node, prev, sizeof(ActiveAlarmStatus) );
      logInfo("ActiveAlarmStatus: Deleted entry. Total alarms entries are: %d ", list->numElemPresent );
      logInfo("Alarm Details:\n{\n alarmType:%s\n alarmId=%d\n alarmCause=%s\n alarmLocation=%s\n}\n",
          alarmTypeStr(eventMessage->alarmType), eventMessage->alarmId, alarmCauseStr(eventMessage->alarmCause), eventMessage->alarmLocation );
      break;
    }
    prev = node;
    node = node->next;
  }
}

/**
 * @details This function checks if alarm lof file size has exceeded
 **/
TBOOL hasAlarmLogFileSizeExceeded()
{
  return FALSE;
}

/**
 * @param   alarmDetails  CreateAlarmDetails Structure
 * @details This function will save the active alarm in the ActiveAlarmStatus table.
 *          If the alarm is required to be posted to SIH, it will construct the
 *          message and send to SIH Module.
 *          Alarm is also stored in the alarm History table.
 **/
void doSaveAlarm(
  IO CreateAlarmDetails  *alarmDetails
  )
{
  BsmContext  *ctx = getBsmContext();
  alarmDetails->alarmNotificationType = ALARMNOTIFICATIONTYPE_OCCURRED;
  /* Check if this alarm is already present in the Active alarm table */
  /* If yes, don't make a duplicate entry */
  /* If no, only then make an entry in the active alarm table */
  if ( FALSE EQ checkIfActiveAlarmExists( alarmDetails ) )
  {
    alarmDetails->alarmSequenceNumber = getNextAlarmSequenceNumber();
    saveAlarmInActiveAlarmTable( alarmDetails );
    /* Filer : Check if alarm needs to be posted to SIH */
    if ( TRUE EQ postToEms() )
    {
      if ( TRUE EQ ctx->emsRegSucc )
      {
        postActiveAlarmMsgToSih( alarmDetails );
      }
    }
    saveAlarmInAlarmHistoryTable( alarmDetails );
  }
  else
  {
    logInfo("%s","ActiveAlarmStatus: Alarm already occurred! Duplicate alarm NOT added");
    logInfo("Duplicated Alarm Details:\n{\n alarmType:%s\n alarmCause=%s\n}\n",
        alarmTypeStr(alarmDetails->alarmType), alarmCauseStr(alarmDetails->alarmCause) );
  }
}

/**
 * @param   alarmDetails  CreateAlarmDetails Structure.
 * @details This function will delete the active alarm in the ActiveAlarmStatus table.
 *          If the alarm is required to be posted to SIH, it will construct the
 *          message and send to SIH Module.
 *          Alarm is also stored in the alarm History table.
 **/
void doClearAlarm(
  IO CreateAlarmDetails  *alarmDetails
  )
{
  BsmContext  *ctx = getBsmContext();

  alarmDetails->alarmNotificationType = ALARMNOTIFICATIONTYPE_CLEARED;
  if ( TRUE EQ checkIfActiveAlarmExists( alarmDetails ) )
  {
    alarmDetails->alarmSequenceNumber = getNextAlarmSequenceNumber();
    deleteAlarmInActiveAlarmTable( alarmDetails );
    alarmDetails->alarmSeverity = ALARMSEVERITY_CLEARED;
    /* Filter : Check if alarm needs to be posted to SIH */
    if ( TRUE EQ postToEms() )
    {
      if ( TRUE EQ ctx->emsRegSucc )
      {
        postActiveAlarmMsgToSih( alarmDetails );
      }
    }
    saveAlarmInAlarmHistoryTable( alarmDetails );
  }
  else
  {
    /* This print is not required as it creates lot of unnecessary logging in the log file */
    /*logInfo( "ActiveAlarmStatus: Occurred alarm does NOT exists for this clearing alarm. Details: AlarmType :%d AlarmCause:%d",
        alarmDetails->alarmType, alarmDetails->alarmCause );*/
  }
}

/**
 * @param   activeEntry   Active Alarm Entry
 * @param   alarmDetails  CreateAlarmDetails Structure.
 * @details This function will fill the active alarm details from the active alarm entry.
 **/
void fillAlarmDetailsToSaveInAlarmHistory(
  IN ActiveAlarmStatus    *activeEntry,
  IO CreateAlarmDetails   *alarmDetails
  )
{
  alarmDetails->alarmSequenceNumber   = activeEntry->alarmSequenceNumber;
  snprintf( alarmDetails->alarmLocation, strlen(activeEntry->alarmLocation)+1, "%s", activeEntry->alarmLocation );
  snprintf( alarmDetails->alarmAdditionalText, strlen(activeEntry->alarmAdditionalText)+1, "%s", activeEntry->alarmAdditionalText );
  alarmDetails->alarmCause    = activeEntry->alarmCause;
  alarmDetails->alarmType     = activeEntry->alarmType;
  alarmDetails->alarmId       = activeEntry->alarmId;

  alarmDetails->alarmNotificationType = ALARMNOTIFICATIONTYPE_CLEARED;
  alarmDetails->alarmSeverity         = ALARMSEVERITY_CLEARED;
  getAndFillDateAndTimeStamp( MAX_SIZE_ACTIVEALARMSTATUS_ALARMDATEANDTIMESTAMP, alarmDetails->dateAndTimeStamp );
  alarmDetails->alarmSequenceNumber = getNextAlarmSequenceNumber();
}

/**
 * @param   alarmCause    Alarm Cause - Set of Enum values
 * @details This function will check if the alarm passed as parameter to this function needs
 *          to be cleared upon lock command or not.
 *          Returns TRUE or FALSE accordingly.
 **/
TBOOL alarmToBeClearedUponLock(
  IN AlarmCause  alarmCause
  )
{

  Index          alarmDbIndex;
  AlarmContext   *ctx = getAlarmContext();
  AlarmDb        *alarmEntryInDb;

  for ( alarmDbIndex = 0; alarmDbIndex < ctx->alarmDbTable.numOfAlarms; alarmDbIndex++ )
  {
    alarmEntryInDb = &ctx->alarmDbTable.alarmDb[alarmDbIndex];
    if ( alarmCause EQ alarmEntryInDb->alarmCause )
    {
      if ( TRUE EQ alarmEntryInDb->clearUponLockCmd )
      {
        return TRUE;
      }
      else
      {
        return FALSE;
      }
    }
  }
  return FALSE;
}

/**
 * @details This function will clear all the alarms upon lock command.
 **/
void doClearAlarmsUponLock()
{
  UINT8                i = 0;
  ActiveAlarmStatus    *activeEntry;
  ListNode             *node=NULL;
  ListNode             *prev=NULL;
  List                 *list = getActiveAlarmStatusTable();
  UINT32                maxNumOfAlarms = list->maxElem;

  for ( i=0; i < maxNumOfAlarms; i++)
  {
    prev = NULL;
    list = getActiveAlarmStatusTable();
    node = list->first;
    while(node)
    {
      activeEntry = (ActiveAlarmStatus*)node->data;
      if ( TRUE EQ alarmToBeClearedUponLock( activeEntry->alarmCause ) )
      {
        CreateAlarmDetails alarmDetails;
        clearBytes( &alarmDetails, sizeof(CreateAlarmDetails) );
        fillAlarmDetailsToSaveInAlarmHistory( activeEntry, &alarmDetails );
	snprintf(alarmDetails.alarmAdditionalText, MAX_SIZE_ACTIVEALARMSTATUS_ALARMADDITIONALTEXT+1, "Cleared upon ENB LOCK");

        listDeleteByNode( list, node, prev, sizeof(ActiveAlarmStatus) );

        logInfo("ActiveAlarmStatus: Deleted entry. Total alarms entries are: %d ", list->numElemPresent );
        logInfo("Alarm Details:\n{\n alarmType:%s\n alarmId=%d\n alarmCause=%s\n alarmLocation=%s\n}\n",
            alarmTypeStr(alarmDetails.alarmType), alarmDetails.alarmId, alarmCauseStr(alarmDetails.alarmCause), alarmDetails.alarmLocation );

        if ( TRUE EQ postToEms())
        {
          postActiveAlarmMsgToSih( &alarmDetails );
        }
        saveAlarmInAlarmHistoryTable( &alarmDetails );
      }
      prev = node;
      node = node->next;
    }
  }
}

/**
 * @param   bsError       BS Error value
 * @param   alarmDetails  CreateAlarmDetails Structure to be filled
 * @details This function is used when alarm is raised. It will get the alarm information from the
 *          alarm DB based on the BS error value and fill alarmDetails structure.
 **/
TBOOL getAlarmDetailsByBsErrorCode(
  IN BsErrorValue        bsError,
  IO CreateAlarmDetails *alarmDetails
  )
{
  Index          alarmDbIndex;
  AlarmContext   *ctx = getAlarmContext();
  AlarmDb        *alarmEntryInDb;

  for ( alarmDbIndex = 0; alarmDbIndex < ctx->alarmDbTable.numOfAlarms; alarmDbIndex++ )
  {
    alarmEntryInDb = &ctx->alarmDbTable.alarmDb[alarmDbIndex];
    if ( bsError EQ alarmEntryInDb->bsErrorValue )
    {
      getAndFillDateAndTimeStamp( MAX_SIZE_ACTIVEALARMSTATUS_ALARMDATEANDTIMESTAMP, alarmDetails->dateAndTimeStamp );
      snprintf( alarmDetails->alarmLocation, strlen(alarmEntryInDb->alarmLocation)+1, "%s", alarmEntryInDb->alarmLocation );
      alarmDetails->alarmCause    = alarmEntryInDb->alarmCause;
      alarmDetails->alarmType     = alarmEntryInDb->alarmType;
      alarmDetails->alarmId       = alarmEntryInDb->alarmId;
      alarmDetails->alarmSeverity = alarmEntryInDb->alarmSeverity;
      return TRUE;
    }
  }
  BS_ERROR_ALARM_ENTRY_NOT_FOUND( errorCodeStr(bsError), bsError );
  return FALSE;
}

/**
 * @param   clearingMsgId  Clearing Message ID
 * @param   alarmDbIndex   alarm Data base index
 * @param   alarmDetails   CreateAlarmDetails Structure to be filled
 * @details This function is used when alarm needs to be cleared. It will get the alarm information from the
 *          alarm DB based on the alarmCause, clearingMsgId and alarmDbIndex and fill alarmDetails structure.
 **/
TBOOL getAlarmDetailsByClearingMsgId(
  IN MsgId              clearingMsgId,
  IN Index              alarmDbIndex,
  IO CreateAlarmDetails *alarmDetails
  )
{
  AlarmContext   *ctx = getAlarmContext();
  AlarmDb        *alarmEntryInDb;

  alarmEntryInDb = &ctx->alarmDbTable.alarmDb[alarmDbIndex];
  if ( clearingMsgId EQ alarmEntryInDb->clearingMsgId )
  {
    getAndFillDateAndTimeStamp( MAX_SIZE_ACTIVEALARMSTATUS_ALARMDATEANDTIMESTAMP, alarmDetails->dateAndTimeStamp );
    snprintf( alarmDetails->alarmLocation, strlen(alarmEntryInDb->alarmLocation)+1, "%s", alarmEntryInDb->alarmLocation );
    alarmDetails->alarmCause    = alarmEntryInDb->alarmCause;
    alarmDetails->alarmType     = alarmEntryInDb->alarmType;
    alarmDetails->alarmId       = alarmEntryInDb->alarmId;
    alarmDetails->alarmSeverity = alarmEntryInDb->alarmSeverity;
    return TRUE;
  }
  return FALSE;
}

/**
 * @param   setObj  SetObjReq
 * @details this function will clear the alarms history
 **/
void bsmDeleteAlarmHistoryTable(
  IN SetObjReq *setObj
  )
{
  AlarmHistoryStatusTable   *circularBuf = getAlarmHistoryStatusTable();
  logInfo("Number of Alarms Present in Alarm History Table Before Deletion:%d",circularBuf->numOfElem);
  clearBytes(&circularBuf->alarmHistoryEntry[0],sizeof(circularBuf->alarmHistoryEntry));  
  initAlarmHistoryStatusTable(circularBuf);
  bsmFillAndSendSetObjSucc(setObj);
} 

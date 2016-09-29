/**
 * @file    bsm_cmn_func.c
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @brief   This file contains common functions related to functionality of BSM.
 *
 * @author:  Shweta Polepally
 * @date:    20 May 2011
 * @details: Initial Version
 **/

#include <math.h>
#include "bsm.h"

/**
 * @details This function returns enodeB ID from enb status table.
 **/
UINT32 getEnodeBId()
{
  EnodebConfig *rowOfTable = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
  return rowOfTable->enodebId;
}

/**
 * @param   state   BSM state enum value
 * @details Returns the state name in string format
 **/
char *getBsmProceduralStateName(
  IN BsmProceduralState state
  )
{
  switch( state )
  {
    case BS_WAIT_INIT_CMPL:            return "BS_WAIT_INIT_CMPL";
    case BS_WAIT_INITIAL_CONFIG_CMPL:  return "BS_WAIT_INITIAL_CONFIG_CMPL";
    case BS_WAIT_COLLECT_INFO_CMPL:    return "BS_WAIT_COLLECT_INFO_CMPL";
    case BS_WAIT_REGISTRATION_CMPL:    return "BS_WAIT_REGISTRATION_CMPL";
    case BS_IDLE:                      return "BS_IDLE";
    case BS_WAIT_REBOOT_CMPL:          return "BS_WAIT_REBOOT_CMPL";
    case ENODEB_WAIT_RESTART_CMPL:     return "ENODEB_WAIT_RESTART_CMPL";
    case ENODEB_WAIT_CONFIG_CMPL:      return "ENODEB_WAIT_CONFIG_CMPL";
    case ENODEB_WAIT_ENABLE_CMPL:      return "ENODEB_WAIT_ENABLE_CMPL";
    case ENODEB_WAIT_DISABLE_CMPL:     return "ENODEB_WAIT_DISABLE_CMPL";
    case WAIT_SW_ACTIVATE_CMPL:        return "WAIT_SW_ACTIVATE_CMPL";
    case BS_WAIT_DEVICE_INIT_CMPL:     return "BS_WAIT_DEVICE_INIT_CMPL";
    default:
      logError("%s","No name defined for this BSM state");
      break;
  }
  return "No name defined for this BSM state";
}

/**
 * @param   ctx     BSM context information
 * @param   state   BSM procedural State
 * @details Sets BSM state
 **/
void setBsmProceduralState(
  IO BsmContext          *ctx,
  IN BsmProceduralState  state
  )
{
  if(ctx->bsmProceduralState NOTEQ state )
  {
    ctx->bsmProceduralState = state;
    logInfo(" BSM state changed to :%s" , getBsmProceduralStateName(state));
  }
}

/**
 * @param   state EnodeB operational state
 * @details Sets operational state
 **/
void setOperationalStateInEnbStatusTable(
  IN OperationalState   state
  )
{
  EnbStatus   *rowOfTable = (EnbStatus*)getFirstRowOfTable(ENBSTATUS);
  /* If  previous state is not same as current state then only send event, else avoid sending event */
  if ( state NOTEQ rowOfTable->operationalState )
  {
    logInfo("Set OperationalState to :%s, previous state:%s", operationalStateStr(state), operationalStateStr(rowOfTable->operationalState) );
    rowOfTable->operationalState = state;
    sendEventEnbOperationalStateChanged ( state );
  }
}

/**
 * @param   state EnodeB operational state
 * @details Sets operational state
 **/
void bsmSetIsOperationalInCellStatusTable(
  IN TBOOL   isOperational
  )
{
  CellStatus   *rowOfTable = (CellStatus*)getFirstRowOfTable(CELLSTATUS);

  logInfo("Set isOperational to :%d from %d", isOperational, rowOfTable->isOperational);
  rowOfTable->isOperational = isOperational;
}

/**
 * @param   state EnodeB Admin state
 * @details Sets Admin state
 **/
void setAdminStateInEnbStatusTable(
  IN AdminState        state
  )
{
  EnbStatus *rowOfTable = (EnbStatus*)getFirstRowOfTable(ENBSTATUS);
  /* If  previous state is not same as current state then only send event, else avoid sending event */
  if ( state NOTEQ rowOfTable->adminState )
  {
    logInfo("Set AdminState to :%s, previous state:%s", adminStateStr(state), adminStateStr(rowOfTable->adminState) );
    rowOfTable->adminState = state;
    sendEventEnbAdminStateChanged( state );
  }
}

/**
 * @details This function checks if enodeB admin state is locked or not.
 *           if locked, return TRUE, else FALSE
 **/
TBOOL isEnodeBLocked()
{
  EnbStatus *rowOfTable = (EnbStatus*)getFirstRowOfTable(ENBSTATUS);
  if ( rowOfTable->adminState EQ ADMINSTATE_LOCKED )
    return TRUE;
  else
    return FALSE;
}

/**
 * @details This function checks if enodeB operational state is enabled or not
 *           if locked, return TRUE, else FALSE
 **/
TBOOL isEnodeBEnabled()
{
  EnbStatus *rowOfTable = (EnbStatus*)getFirstRowOfTable(ENBSTATUS);
  if ( rowOfTable->operationalState EQ OPERATIONALSTATE_ENABLED )
    return TRUE;
  else
    return FALSE;
}

/**
 * @param   enodebIpAddr  To be filled
 * @details This function gets and fills the ip address field with the IP address
 *          present in EnodeB status table
 **/
void getAndfillEnodebIpAddressFromEnodebStatus(
  IO UINT8   *enodebIpAddr
  )
{
  EnbStatus  *enbStatus    = (EnbStatus*)getFirstRowOfTable(ENBSTATUS);
  snprintf( enodebIpAddr, (strlen(enbStatus->enodebIpAddress)+1), "%s", enbStatus->enodebIpAddress);
}

/**
 * @param   dateAndTime  Date and time info to be set
 * @details This function sets the date and time of the system with the value specified in
 *          dateAndTime field.
 **/
TBOOL bsmSetDateAndTimeOfSystem(
  UINT8    *dateAndTime
  )
{
  BsmContext  *bsmContext = getBsmContext();
  String50   toSetDateAndTimeStamp;

  clearBytes( toSetDateAndTimeStamp, sizeof(String50) );

  char year[5], month[3], date[3], hours[3], mins[3], secs[3];
  clearBytes( &year, 5 );
  clearBytes( &month, 3 );
  clearBytes( &date, 3 );
  clearBytes( &hours, 3 );
  clearBytes( &mins, 3 );
  clearBytes( &secs, 3 );

  /* Set the new time configured by user in the system */
  snprintf( year,  5, "%s", &dateAndTime[0]);
  snprintf( month, 3, "%s", &dateAndTime[4]);
  snprintf( date,  3, "%s", &dateAndTime[6]);
  snprintf( hours, 3, "%s", &dateAndTime[8]);
  snprintf( mins,  3, "%s", &dateAndTime[10]);
  snprintf( secs,  3, "%s", &dateAndTime[12]);

  snprintf( toSetDateAndTimeStamp, sizeof(toSetDateAndTimeStamp), "date -s \"%d %s %d %d:%d:%d\" > /dev/null 2>&1",
           atoi(date), getMonthInString( atoi(month) ), atoi(year),
           atoi(hours), atoi(mins), atoi(secs) );
  /* Calculating Uptime Difference when updating timeanddatesource */
  bsmContext->moduleUpTimeDifference = bsmContext->moduleUpTimeDifference + (UINT32)getUpTime( bsmContext->moduleStartTime );

  /* Disabled as per BS-1946 */
  if ( 0 NOTEQ system( toSetDateAndTimeStamp ) )
  {
    BS_ERROR_DATE_AND_TIME_COULD_NOT_BE_SET( toSetDateAndTimeStamp, "operator" );
    return FALSE;
  }
  bsmContext->moduleStartTime = getCurrentTime();
  return TRUE;
}

/**
 * @param   ledId      LED Identity.
 * @param   ledColor   LED Color.
 * @param   bsmContext BSM Module related Information.
 * @details This function creates and sends Color set command to DEH.
 */
void bsmCreateAndSendSetLedColor(
  IN  LedId        ledId,
  IN  LedColor     ledColor,
  IO  BsmContext  *bsmContext
  )
{
  UINT8                i=0;
  TBOOL                found = FALSE;
  MsgBsmDehSetLedColor setColorReq;
  clearBytes(&setColorReq, sizeof(MsgBsmDehSetLedColor));
  LedInfoTable  *ledInfo = NULL;

  for ( i=0; i< LEDID_MAX; i++ )
  {
    ledInfo = &bsmContext->ledInfoTable[i];
    if( (ledInfo->ledId EQ ledId) )
    {
      found = TRUE;
      if (ledInfo->ledColor NOTEQ ledColor )
      {
        setColorReq.ledId     = ledId;
        setColorReq.ledColor  = ledColor;
        createAndSendMsg( MSG_BSM_DEH_SET_LED_COLOR, (UINT8*)&setColorReq , sizeof(MsgBsmDehSetLedColor) );
        /* Update info in BSM context */
        ledInfo->ledColor = ledColor;
      }
      else
      {
        logInfo(" Color of %s is same as expected color to be set:%s. Set color not done", ledIdStr(ledId), ledColorStr(ledColor) );
      }
    }
  }
  if ( FALSE EQ found )
  {
    /* Log error: Invalid LED ID expected to be set */
  }
}

/**
 * @details This function initialises Index values for all operator command tables.
 */
void initIndexForCmdTables()
{
  EnbAdmin   *enbAdmin = (EnbAdmin*)getFirstRowOfTable(ENBADMIN);
  enbAdmin->index = 1;

  FileDownload   *fileDw = (FileDownload*)getFirstRowOfTable(FILEDOWNLOAD);
  fileDw->index = 1;

  FileUpload   *fileUp = (FileUpload*)getFirstRowOfTable(FILEUPLOAD);
  fileUp->index = 1;

  SwActivation  *swAct = (SwActivation*)getFirstRowOfTable(SWACTIVATION);
  swAct->index = 1;

  SwInstall     *swInstall = (SwInstall*)getFirstRowOfTable(SWINSTALL);
  swInstall->index = 1;

  TraceControl  *trace = (TraceControl*)getFirstRowOfTable(TRACECONTROL);
  trace->index = 1;
}

/**
 * @param   bsmContext BSM Context information.
 * @details This function sets All LED colour to AMBER in bsmContext during stating of ENODEEB
 */
void initLedColorInfo(
  IO BsmContext *bsmContext
  )
{
  UINT8 index=0;
  /* This is just to initialise the information present at BSM about LED's to Amber, assuming all colors
   * are set to AMBER by DEH on init. We need not send SetColor message to DEH */
  bsmContext->ledInfoTable[index].ledId    = LEDID_BBPOWER;
  bsmContext->ledInfoTable[index].ledColor = LEDCOLOR_AMBER;
  index++;
  bsmContext->ledInfoTable[index].ledId    = LEDID_GPS;
  bsmContext->ledInfoTable[index].ledColor = LEDCOLOR_AMBER;
  index++;
  bsmContext->ledInfoTable[index].ledId    = LEDID_EMSREGISTRATION;
  bsmContext->ledInfoTable[index].ledColor = LEDCOLOR_AMBER;
  index++;
  bsmContext->ledInfoTable[index].ledId    = LEDID_ENBOPERATIONAL;
  bsmContext->ledInfoTable[index].ledColor = LEDCOLOR_AMBER;
  index++;
}

/**
 * @param   mibVersion MIB Version.
 * @details This function gets versionInfo and fills the mib version information in enbStatus
 *           mibVersion field.
 */
void getAndFillMibVersionInfo(
  IO UINT8   *mibVersion
  )
{
  BsmContext   *ctx = getBsmContext();
  VersionCfgContents  versionInfo;
  clearBytes( &versionInfo, sizeof(VersionCfgContents) );

  convertVersionInfoToVersionCfgContents( &ctx->versionInfo, &versionInfo );
  snprintf( mibVersion, MAX_SIZE_ENBSTATUS_MIBVERSION, "%d.%d", versionInfo.releaseVersion, versionInfo.featureVersion );
}

/**
 * @details This function initialises EnbStatus table with default values
 */
void initEnodeBStatusTable()
{
  EnbStatus      *rowOfTable = (EnbStatus*)getFirstRowOfTable(ENBSTATUS);
  EnodebConfig   *config     = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);


  rowOfTable->index                        = 1;
  rowOfTable->enodebId                     = config->enodebId;
  snprintf( rowOfTable->vendorName, MAX_SIZE_ENBSTATUS_VENDORNAME, "pwav" );
  /* Get MibVersionInfo */
  getAndFillMibVersionInfo( rowOfTable->mibVersion );
  snprintf( rowOfTable->enodebIpAddress, (strlen(config->enodebStaticIpAddr)+1), "%s", config->enodebStaticIpAddr );
  rowOfTable->connectedToMME               = CONNECTEDTOMME_NOTCONNECTED;
  rowOfTable->adminState                   = ADMINSTATE_LOCKED;
  rowOfTable->operationalState             = OPERATIONALSTATE_DISABLED;
  rowOfTable->numOfUesConnected            = 0;
  rowOfTable->aggregateDLThroughputKbps    = 0;
  rowOfTable->aggregateULThroughputKbps    = 0;
  rowOfTable->loadState                    = LOADSTATE_NORMALLOAD;
  rowOfTable->numActiveCriticalAlarms      = 0;
  rowOfTable->numActiveMajorAlarms         = 0;
  rowOfTable->dateAndTimeSource            = DATEANDTIMESOURCE_NOSOURCEAVAILABLE;
}

/**
 * @details This function initialises RabHistory table with default values
 */
void initRabHistoryStatusTable()
{
  RabHistory      *rowOfTable = (RabHistory*)getFirstRowOfTable(RABHISTORY);
  rowOfTable->rabReleaseCause = RABRELEASECAUSE_NOTAPPLICABLE;
  snprintf( rowOfTable->rabReleaseTime, MAX_SIZE_RABHISTORY_RABRELEASETIME+1, "00000000000000000" );
  snprintf( rowOfTable->rabSetupTime, MAX_SIZE_RABHISTORY_RABSETUPTIME+1, "00000000000000000" );
}

/**
 * @details This function initialises ActiveRabStatus table with default values
 */
void initActiveRabStatusTable()
{
  ActiveRabStatus      *rowOfTable = (ActiveRabStatus*)getFirstRowOfTable(ACTIVERABSTATUS);
  snprintf( rowOfTable->rabSetupTime, MAX_SIZE_ACTIVERABSTATUS_RABSETUPTIME+1, "00000000000000000" );
}

/**
 * @details This function initialises UeEventHistory table with default values
 */
void initUeEventHistoryTable()
{
  UeEventHistory      *rowOfTable = (UeEventHistory*)getFirstRowOfTable(UEEVENTHISTORY);
  rowOfTable->eventType = EVENTTYPE_OTHER;
  snprintf( rowOfTable->eventDetail, MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL+1, "Not Available" );
  snprintf( rowOfTable->eventTimestamp, MAX_SIZE_UEEVENTHISTORY_EVENTTIMESTAMP+1, "00000000000000000" );
}

/**
 * @details This function initialises ActiveUeStatus table with default values
 */
void initActiveUeStatusTable()
{
  ActiveUeStatus      *rowOfTable = (ActiveUeStatus*)getFirstRowOfTable(ACTIVEUESTATUS);
  snprintf( rowOfTable->entryTime, MAX_SIZE_ACTIVEUESTATUS_ENTRYTIME+1, "00000000000000000" );
}

/**
 * @details This function initialises ModuleLogStatusTable table with default values
 */
void initModuleLogStatusTable()
{
  UINT8 index;
  ModuleLogFileStatus  *rowOfTable = (ModuleLogFileStatus*)getFirstRowOfTable(MODULELOGFILESTATUS);

  for(index = 0; index < MAX_ROWS_IN_MODULELOGFILESTATUS_TABLE; index++)
  {
    snprintf( rowOfTable->fileName, MAX_SIZE_MODULELOGFILESTATUS_FILENAME+1, "None" );
    snprintf( rowOfTable->openedAt, MAX_SIZE_MODULELOGFILESTATUS_OPENEDAT+1, "00000000000000000" );
    snprintf( rowOfTable->closedAt, MAX_SIZE_MODULELOGFILESTATUS_CLOSEDAT+1, "00000000000000000" );
    getNextRowOfTable(rowOfTable);
  }
}

/**
 * @details This function initialises initLoginStatusTable table with default values
 */
void initLoginStatusTable()
{
  UINT8 index;
  LoginStatus  *rowOfTable = (LoginStatus*)getFirstRowOfTable(LOGINSTATUS);

  for(index = 0; index < MAX_ROWS_IN_LOGINSTATUS_TABLE; index++)
  {
    snprintf( rowOfTable->loggedInAt, MAX_SIZE_LOGINSTATUS_LOGGEDINAT+1, "00000000000000000" );
    getNextRowOfTable(rowOfTable);
  }
}

/**
 * @details This function initialises CellStatus table with default values
 */
void initCellStatusTable()
{
  CellStatus      *cellStatus = (CellStatus*)getFirstRowOfTable(CELLSTATUS);
  CellConfig      *cellConfig = (CellConfig*)getFirstRowOfTable(CELLCONFIG);

  cellStatus->index         =1;
  cellStatus->cellId        = cellConfig->cellId;
  cellStatus->isBarred      = FALSE;
  cellStatus->isOperational = FALSE;
  cellStatus->isOverloaded  = FALSE;
  cellStatus->spare1        = FALSE;
  snprintf( cellStatus->info, MAX_SIZE_CELLSTATUS_INFO+1, "ENB is locked |-|-" );
}

/**
 * @details This function will get maximum number of MMEs to be connected by reading mmeconfig table
 *          index value.
 */
void getNumOfMaxMmesToConnectWith(
  IO BsmContext *ctx
  )
{
  MmeConfig  *rowOfTable = (MmeConfig*)getFirstRowOfTable(MMECONFIG);
  ctx->maxMmesToConnectWith = rowOfTable->index;
  logInfo("Max MME's to connect are : %d ", ctx->maxMmesToConnectWith );
}

/**
 * @param   cfg   Lib Configuration parameters
 * @param   tableNum represents table number of OAM tables
 * details This reads each table and its column information from oamConfig.tpl file
 */
static void bsmReadTable(
  IN config_t           *cfg,
  IN UINT32             *tableNum,
  IO BsmOamTableContext *oamCtx
  )
{
  char             tableInfo[MAX_TABLE_NAME];
  config_setting_t *setting = NULL ;
  BsmOamTableInfo  *oamTableInfo = NULL ;

  snprintf(tableInfo,MAX_TABLE_NAME, "oamTableConfig.table%dCfg",*tableNum);
  setting = config_lookup(cfg, tableInfo);
  if(setting NOTEQ NULL)
  {
    int count = config_setting_length(setting);
    int i;

    for(i = 0; i < count; ++i)
    {
      config_setting_t *tableInfo = config_setting_get_elem(setting, i);
      /* Only output the record if all of the expected fields are present. */
      const char *tableType;
      int actualTableNumber, tempTableNumber,tableEnumType;

      if(! ( config_setting_lookup_int(tableInfo, "tableNumber", &tempTableNumber)
         && config_setting_lookup_string(tableInfo, "tableType",&tableType)))
        continue;

      if (strcmp(tableType, "OprCmd") EQ 0)
      {
        actualTableNumber = tempTableNumber - OPR_CMD_TABLES_OFFSET;
        oamTableInfo = &oamCtx->oprCmdTableInfo[actualTableNumber];
        tableEnumType = TABLE_OPERCMD;
      }
      else if (strcmp(tableType, "OprConfig") EQ 0)
      {
        actualTableNumber = tempTableNumber - OPR_CFG_TABLES_OFFSET ;
        oamTableInfo = &oamCtx->oprCfgTableInfo[actualTableNumber];
        tableEnumType = TABLE_OPERCONFIG;
      }
      else if(strcmp(tableType, "Status") EQ 0)
      {
        actualTableNumber = tempTableNumber - STATUS_TABLES_OFFSET ;
        oamTableInfo = &oamCtx->statusTableInfo[actualTableNumber];
        tableEnumType = TABLE_STATUS;
      }
      else if(strcmp(tableType, "PreConfig") EQ 0)
      {
        actualTableNumber = tempTableNumber - PRE_CFG_TABLES_OFFSET ;
        oamTableInfo = &oamCtx->preCfgTableInfo[actualTableNumber];
        tableEnumType = TABLE_PRECONFIG;
      }
      else
      {
        logError("wrong table type [%s]. Please check oamConfig.tpl", tableType);
      }

      clearBytes(oamTableInfo,sizeof(BsmOamTableInfo));

      /* Fill GLobal context with OAM table data */
      oamTableInfo->oamTableType = tableEnumType;
      readCol(cfg, tableNum, &oamTableInfo->colInfo[0]);
    }
  }
}

/**
 * @param   configFilePath represents configuration  path
 * @param   bsmOamCtx  BSM Oam Table context
 * @section This reads the configuration file 'oamTable.cfg' and stores it in its context.
 */
TBOOL bsmParseOamCfg(
  IN UINT8*              configFilePath,
  IO BsmOamTableContext *bsmOamCtx
  )
{
  config_t   cfg;
  UINT8      fullFilePath[MAX_BUFFER_SIZE];
  const char *str ;
  int        noOfTables,tableIndex;
  UINT32     *noOfValidTables = NULL ;

  config_init(&cfg);
  constructFullPath ( configFilePath, "oamConfig.tpl", fullFilePath );

  /* Read the file. If there is an error, report it and exit. */
  if(!config_read_file(&cfg, fullFilePath))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    return FALSE;
  }
  /* Get the configuration file name. */
  config_lookup_string(&cfg, "name", &str);
  /* To find Number Of Tables */
  if(config_lookup_int(&cfg, "noOfTables", &noOfTables))
  {
    noOfValidTables = &bsmOamCtx->noOfvalidTables;
    *noOfValidTables = noOfTables;
  }

  /* Operator Commands */
  for (tableIndex = MIN_OPR_CMD_TABLEID ; tableIndex <= MAX_OPR_CMD_TABLEID ; tableIndex ++)
  {
    bsmReadTable(&cfg,&tableIndex,bsmOamCtx);
  }
  /* Operator Config */
  for (tableIndex = MIN_OPR_CFG_TABLEID ; tableIndex <= MAX_OPR_CFG_TABLEID ; tableIndex ++)
  {
    bsmReadTable(&cfg,&tableIndex,bsmOamCtx);
  }
  /* Status */
  for (tableIndex = MIN_STATUS_TABLEID ; tableIndex <= MAX_STATUS_TABLEID ; tableIndex ++)
  {
    bsmReadTable(&cfg,&tableIndex,bsmOamCtx);
  }
  /* Pre Config */
  for (tableIndex = MIN_PRE_CFG_TABLEID ; tableIndex <= MAX_PRE_CFG_TABLEID ; tableIndex ++)
  {
    bsmReadTable(&cfg,&tableIndex,bsmOamCtx);
  }

  config_destroy(&cfg);
  return TRUE;
}

/**
 * @param phyConfig is phy configuration
 * @section This function calculates reference signal power in phy config using parameters Pa, Pb and other phy parameters
 */
void bsmCalculateRefSigPower(
     IO PhyConfig  *phyConfig
     )
{
  CellConfig *cellConfig = (CellConfig*)getFirstRowOfTable(CELLCONFIG);
  UehConfig  *uehConfig = (UehConfig*)getFirstRowOfTable(UEHCONFIG);
  UINT8       numRbs = 0;
  UINT8       na = 12;  //Hardcode this using #define later
  UINT8       nr = 2;  //Hardcode this using #define later
  UINT8       nb = 0;  //Rename this properly
  double      valuesPa[8] = { -6.00, -4.77, -3.00, -1.77, 0.00, 1.00, 2.00, 3.00 };
  double      valuesPb[4] = { 1.25, 1.00, 0.75, 0.5 };
  double      powerOne;
  double      powerTwo;

  if(cellConfig->dlBandwidth EQ DLBANDWIDTH_5MHZ)
  {
    numRbs = 25;
  }
  else if(cellConfig->dlBandwidth EQ DLBANDWIDTH_10MHZ)
  {
    numRbs = 50;
  }
  else if(cellConfig->dlBandwidth EQ DLBANDWIDTH_20MHZ)
  {
    numRbs = 100;
  }

  nb = na - nr * uehConfig->numOfTxAnt;

  powerOne = ( pow ( 10, (double)phyConfig->totalTxPower/10 ) )/ ( pow( 10, ( valuesPa[phyConfig->cellPhyPa]/10 ) ) * valuesPb[phyConfig->cellPhyPb] * numRbs * nb + numRbs * nr );
  powerTwo = ( pow ( 10, (double)phyConfig->totalTxPower/10 ) )/ ( pow( 10, ( valuesPa[phyConfig->cellPhyPa]/10 ) ) * numRbs * na );

  if( powerOne < powerTwo )
  {
    phyConfig->referenceSignalPower = floor( 10 * log10(powerOne) );
  }
  else
  {
     phyConfig->referenceSignalPower = floor( 10 * log10(powerTwo) );
  }
}

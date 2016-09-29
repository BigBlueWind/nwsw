/**
 * @file    bsm_enb_cfg.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions which will handle and fill functions related to EnodeB Configurations.
 *
 * @author:  Shweta Polepally
 * @date:    Oct 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param bsmContext BSM Module related Information.
 * @details This function will check if configuration response is received from expected modules,
 *          else will log error ad return FALSE.
 */
TBOOL checkForConfigProcedureCompletion(
  IO BsmContext       *bsmContext
  )
{
  TBOOL result = TRUE;

  if ( FALSE EQ bsmContext->cfgRspFlags.uehConfigRspRcvd )
  {
    BS_ERROR_NO_RSP_FOR_CONFIG_REQ(getModuleName(MODID_UEH) );
    createConfigurationFailedAlarm(MODID_UEH, "No Response for CONFIG REQ");
    result = FALSE;
  }
  if ( FALSE EQ bsmContext->cfgRspFlags.s1apConfigRspRcvd )
  {
    BS_ERROR_NO_RSP_FOR_CONFIG_REQ(getModuleName(MODID_S1AP) );
    createConfigurationFailedAlarm(MODID_S1AP, "No Response for CONFIG REQ");
    result = FALSE;
  }
  if ( FALSE EQ bsmContext->cfgRspFlags.deRhConfigRspRcvd )
  {
    BS_ERROR_NO_RSP_FOR_CONFIG_REQ( getModuleName(MODID_DEH) );
    createConfigurationFailedAlarm(MODID_DEH, "No Response for CONFIG REQ");
    result = FALSE;
  }
  if ( FALSE EQ bsmContext->cfgRspFlags.pmhConfigRspRcvd )
  {
    BS_ERROR_NO_RSP_FOR_CONFIG_REQ( getModuleName(MODID_PMH) );
    createConfigurationFailedAlarm(MODID_PMH, "No Response for CONFIG REQ");
    result = FALSE;
  }
  if ( FALSE EQ bsmContext->cfgRspFlags.pmhCtrlInputConfigRspRcvd )
  {
    BS_ERROR_NO_RSP_FOR_CONFIG_REQ( getModuleName(MODID_PMH) );
    createConfigurationFailedAlarm(MODID_PMH, "No Response for INPUT CONFIG REQ");
    result = FALSE;
  }

  if ( (FALSE EQ result) || (TRUE EQ bsmContext->isCfgRspWithWrongTransIdRcvd ) )
  {
    bsmContext->isConfigSuccessful = FALSE;
    bsmContext->isCfgRspWithWrongTransIdRcvd = FALSE;
  }
  else
  {
    bsmContext->isConfigSuccessful = TRUE;
  }
  return result;
}

/**
 * @param   msgId       Received message ID
 * @param   bsmContext  BSM Module related Information.
 * @details This function will switch on the message received and mark the response received flag.
 */
ModuleId setConfigRspRcvdFromModule(
  IN MsgId       msgId,
  IO BsmContext *bsmContext
  )
{
  switch( msgId )
  {
    case MSG_S1M_BSM_CONFIG_SUCC:
    case MSG_S1M_BSM_CONFIG_FAIL:
      bsmContext->cfgRspFlags.s1apConfigRspRcvd = TRUE;
      return MODID_S1M;

    case MSG_CEM_BSM_ENB_CONFIG_SUCC:
    case MSG_CEM_BSM_ENB_CONFIG_FAIL:
      bsmContext->cfgRspFlags.uehConfigRspRcvd = TRUE;
      return MODID_CEM;

    case MSG_DEH_BSM_RH_CONFIG_SUCC:
    case MSG_DEH_BSM_RH_CONFIG_FAIL:
      bsmContext->cfgRspFlags.deRhConfigRspRcvd = TRUE;
      return MODID_DEH;

    case MSG_PMH_BSM_CONFIG_SUCC:
    case MSG_PMH_BSM_CONFIG_FAIL:
      bsmContext->cfgRspFlags.pmhConfigRspRcvd = TRUE;
      return MODID_PMH;

    case MSG_UDH_BSM_CONFIG_SUCC:
    case MSG_UDH_BSM_CONFIG_FAIL:
      bsmContext->cfgRspFlags.udhConfigRspRcvd = TRUE;
      return MODID_PMH;

    case MSG_PMH_BSM_COUNTER_INPUT_CONFIG_SUCC:
    case MSG_PMH_BSM_COUNTER_INPUT_CONFIG_FAIL:
      bsmContext->cfgRspFlags.pmhCtrlInputConfigRspRcvd = TRUE;
      return MODID_PMH;

    default:
      logInfo("%s", "Configuration Message not handled.. Coding Error... Check !! ");
      break;
  }
  logInfo("%s", "Configuration Message not handled.. Coding Error... Check !! ");
  return MODID_MAX;
}

/**
 * @param   failedModuleId   Module ID which failed to respond
 * @param   addText          Additional Text Information
 * @details This function will save the Configuration fail alarm and store
 *          it in the active alarm table.
 * */
void createConfigurationFailedAlarm(
  IN ModuleId    failedModuleId,
  IN UINT8	 *addText
  )
{
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));

  if ( TRUE EQ getAlarmDetailsByBsErrorCode( BS_ERR_CONFIGURATION_OR_CUSTOMIZATION_ERROR, &alarmDetails ) )
  {
    strcat( alarmDetails.alarmLocation, getModuleName(failedModuleId) );
    strcpy( alarmDetails.alarmAdditionalText, addText);
    doSaveAlarm( &alarmDetails );
  }
}

/**
 * @param   clearingMsgId    Clearing Message ID
 * @param   moduleId         Module ID
 * @param   bsmContext       BSM context
 * @details This function will clear configuration failed alarm from the alarm list.
 *          Function calls doClearAlarm() func.
 */
void clearConfigurationFailedAlarm(
  IN MsgId       clearingMsgId,
  IN ModuleId    moduleId,
  IO BsmContext  *bsmContext
  )
{
  Index                alarmDbIndex;
  CreateAlarmDetails   alarmDetails;
  clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));

  for ( alarmDbIndex = 0; alarmDbIndex < bsmContext->afhCtx.alarmDbTable.numOfAlarms; alarmDbIndex++ )
  {
    if ( TRUE EQ getAlarmDetailsByClearingMsgId( clearingMsgId, alarmDbIndex, &alarmDetails ) )
    {
      strcat( alarmDetails.alarmLocation, getModuleName(moduleId) );
      doClearAlarm( &alarmDetails );
      clearBytes( &alarmDetails, sizeof(CreateAlarmDetails));
    }
  }
}

/**
 * @param   flags   configuration response flags, if received is set to TRUE else FALSE.
 * @details This function will check if configuration response is received from expected modules.
 *          Returns FALSE it not, else returns TRUE.
 */
static TBOOL isConfigRspRcvdFromAllMod(
  IN CfgRspFlags  *flags
  )
{
  if ( ( flags->uehConfigRspRcvd EQ FALSE ) || ( flags->s1apConfigRspRcvd EQ FALSE ) ||
       ( flags->deRhConfigRspRcvd EQ FALSE ) || ( flags->pmhConfigRspRcvd EQ FALSE ) ||
       ( flags->pmhCtrlInputConfigRspRcvd EQ FALSE ) || ( flags->udhConfigRspRcvd EQ FALSE ))
    return FALSE;
  else
    return TRUE;
}

/**
 * @details This function will fill and send MSG_BSM_UEH_ENB_CONFIG_REQ message to UEH
 */
void bsmFillAndSendUehEnbCfgReqMsg()
{
  UINT8                   rowNumber;
  MsgBsmUehEnbConfigReq   configReq;
  BsmContext             *bsmContext = getBsmContext();

  clearBytes( &configReq, sizeof(MsgBsmUehEnbConfigReq) );

  /* Do a consistency check here- this is required because default config file values might be wrong.
   * Hence this check will prevent wrong values to be sent to UEH */
  if ( FALSE EQ doConsistencyCheck() )
  {
    logError("%s", "Consistency Check Failed. CONFIG Request is not sent to UEH ");
    return;
  }
  /*
   * If consistency check succeeds, fill & send Configuration Request message
   **/
  configReq.transId = getConfigTransId();

  EnodebConfig  *enbConfig     =  (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
  configReq.enodebId           = enbConfig->enodebId;

  if(CELLCOMMAND_BAR EQ bsmContext->cellCmdRcvd)
    configReq.cellBarred = CELLBARRED_BARRED;
  else
    configReq.cellBarred = CELLBARRED_NOTBARRED;

  CellConfig  *cellConfig  =  (CellConfig*)getFirstRowOfTable(CELLCONFIG);
  copyBytes( cellConfig, sizeof(CellConfig), &configReq.cellCfg );

  SchedulerConfig  *schConfig = (SchedulerConfig*)getFirstRowOfTable(SCHEDULERCONFIG);
  copyBytes( schConfig, sizeof(SchedulerConfig), &configReq.schCfg );

  PhyConfig     *phyConfig = (PhyConfig*)getFirstRowOfTable(PHYCONFIG);
  copyBytes( phyConfig, sizeof(PhyConfig), &configReq.phyCfg );

  SysInfo1Config  *sys1Config = (SysInfo1Config*)getFirstRowOfTable(SYSINFO1CONFIG);
  copyBytes( sys1Config, sizeof(SysInfo1Config), &configReq.sib1Cfg );

  SysInfo2Config  *sys2Config = (SysInfo2Config*)getFirstRowOfTable(SYSINFO2CONFIG);
  copyBytes( sys2Config, sizeof(SysInfo2Config), &configReq.sib2Cfg );

  SysInfo3Config  *sys3Config = (SysInfo3Config*)getFirstRowOfTable(SYSINFO3CONFIG);
  copyBytes( sys3Config, sizeof(SysInfo3Config), &configReq.sib3Cfg );

  QosConfig  *qosConfig = (QosConfig*)getFirstRowOfTable(QOSCONFIG);
  copyBytes( qosConfig, sizeof(QosConfig), &configReq.qosCfg );

  SrsConfigDedicated  *srsDedCfg = (SrsConfigDedicated*)getFirstRowOfTable(SRSCONFIGDEDICATED);
  copyBytes( srsDedCfg, sizeof(SrsConfigDedicated), &configReq.srsCfgDedicated );

  /* Filling SIB4 Parameters */
  EutraNeighCellConfig  *eutraNeighCellConfig = (EutraNeighCellConfig*)getFirstRowOfTable(EUTRANEIGHCELLCONFIG);

  configReq.sib4Cfg.intraFreqNeighCellQOffsetCell = eutraNeighCellConfig->intraFreqNeighCellQOffsetCell;
  configReq.sib4Cfg.neighEutraPhyCellId = eutraNeighCellConfig->neighEutraPhyCellId;

  EutraHoProfileConfig  *eutraHoProfileConfig = (EutraHoProfileConfig*)getFirstRowOfTable(EUTRAHOPROFILECONFIG);
  copyBytes( eutraHoProfileConfig, sizeof(EutraHoProfileConfig), &configReq.eutraHoProfileConfig );
  
  SecurityConfig  *securityConfig = (SecurityConfig*)getFirstRowOfTable(SECURITYCONFIG);
  copyBytes( securityConfig, sizeof(SecurityConfig), &configReq.securityConfig );
   
  for (rowNumber = 1 ; rowNumber <= MAX_ROWS_IN_EUTRANEIGHCELLCONFIG_TABLE; rowNumber++ )
  {
    if (TRUE EQ eutraNeighCellConfig->isUsed)
    {
      copyBytes( eutraNeighCellConfig, sizeof(EutraNeighCellConfig), &configReq.eutraNeighCellConfig[rowNumber-1] );
      getNextRowOfTable(eutraNeighCellConfig);
      configReq.numEutraNeighborCells++;
    }
  }

  createAndSendMsg( MSG_BSM_CEM_ENB_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmUehEnbConfigReq));
}

void bsmFillAndSendUdhCfgReqMsg()
{
  CellConfig  *config = (CellConfig *)getFirstRowOfTable(CELLCONFIG);
  UdhConfig   *udhConfig = (UdhConfig*)getFirstRowOfTable(UDHCONFIG);
  MsgBsmUdhCfgReq configReq;

  clearBytes(&configReq,  sizeof(MsgBsmUdhCfgReq));

  configReq.transId                       = getConfigTransId();
  configReq.startRnti                     = config->startRnti;
  configReq.ueInactivityCheckIntervalSecs = config->ueInactivityCheckIntervalSecs;
  configReq.maxConnectedUes               = config->maxConnectedUes;
  configReq.ulInactivityIntervalInSecs    = udhConfig->ulInactivityIntervalInSecs;
  configReq.ueThroughtputTimerInSecs      = udhConfig->ueThroughtputTimerInSecs;
  configReq.memoryDiagCfg                 = udhConfig->memoryDiagCfg;
 createAndSendMsg( MSG_BSM_UDH_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmUdhCfgReq));
}
void bsmFillAndSendPmhCfgReqMsg()
{

  UINT8              rowNumber;
  MsgBsmPmhConfigReq configReq;

  PmCounterLogConfig  *pmCounterLogConfig   =  (PmCounterLogConfig*)getFirstRowOfTable(PMCOUNTERLOGCONFIG);
  CounterConfig       *counterConfig        =  (CounterConfig*)getFirstRowOfTable(COUNTERCONFIG);

  clearBytes(&configReq, sizeof(MsgBsmPmhConfigReq));

  configReq.transId            =  getConfigTransId();
  configReq.enodebId           =  getEnodeBId();
  configReq.pmCounterLogConfig =  *pmCounterLogConfig;

  for (rowNumber = 1 ; rowNumber <= MAX_ROWS_IN_COUNTERCONFIG_TABLE; rowNumber++ )
  {
    copyBytes( counterConfig, sizeof(CounterConfig), &configReq.counterConfig[rowNumber-1] );
    getNextRowOfTable(counterConfig);
  }

  createAndSendMsg( MSG_BSM_PMH_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmPmhConfigReq));
}

void bsmFillAndSendPmhCntrInputCfgReqMsg()
{
  MsgBsmPmhCntrInputConfigReq msgBsmPmhCntrInputConfigReq;
  clearBytes(&msgBsmPmhCntrInputConfigReq, sizeof(MsgBsmPmhCntrInputConfigReq));


  msgBsmPmhCntrInputConfigReq.transId        =  getConfigTransId();
  CellConfig  *cellConfig  =  (CellConfig*)getFirstRowOfTable(CELLCONFIG);
  msgBsmPmhCntrInputConfigReq.cellId = cellConfig->cellId;

  MmeConfig  *mmeConfig  =  (MmeConfig*)getFirstRowOfTable(MMECONFIG);
  msgBsmPmhCntrInputConfigReq.peerId = mmeConfig->index;
  createAndSendMsg( MSG_BSM_PMH_COUNTER_INPUT_CONFIG_REQ, (UINT8*)&msgBsmPmhCntrInputConfigReq, sizeof(MsgBsmPmhCntrInputConfigReq));
}

/**
 * @details This function will fill and send MSG_BSM_S1AP_CONFIG_REQ message to S1AP
 */
void bsmFillAndSendS1apCfgReqMsg()
{
  MsgBsmS1apConfigReq   configReq;
  clearBytes( &configReq, sizeof(MsgBsmS1apConfigReq) );

  configReq.transId = getConfigTransId();

  EnodebConfig  *enbConfig     =  (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
  configReq.enodebId = enbConfig->enodebId;
  snprintf( configReq.enodebName, (strlen(enbConfig->enodebName)+1), "%s", enbConfig->enodebName );
  CellConfig    *cellConfig    = (CellConfig*)getFirstRowOfTable(CELLCONFIG);
  snprintf( configReq.plmnMCC, (strlen(cellConfig->plmnMCC)+1), "%s", cellConfig->plmnMCC );
  snprintf( configReq.plmnMNC, (strlen(cellConfig->plmnMNC)+1), "%s", cellConfig->plmnMNC );
  configReq.tac             = cellConfig->tac;
  configReq.maxConnectedUes = cellConfig->maxConnectedUes;

  SysInfo2Config  *sysInfo2Config = (SysInfo2Config*)getFirstRowOfTable(SYSINFO2CONFIG);
  configReq.defaultPagCycle = sysInfo2Config->defaultPagCycle;

  MmeConfig    *mmeConfig = (MmeConfig*)getFirstRowOfTable(MMECONFIG);
  configReq.mmeConfig = *mmeConfig;

  createAndSendMsg( MSG_BSM_S1M_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmS1apConfigReq));
}

void fillLthInitCfgReqMsg( 
    IO  MsgBsmLthInitConfigReq *configReq)
{
    TraceConfig *traceConfig = (TraceConfig*)getFirstRowOfTable(TRACECONFIG);
    configReq->transId         = getConfigTransId();
    configReq->traceConfig.traceRRC = traceConfig->traceRRC;
    configReq->traceConfig.traceX2AP = traceConfig->traceX2AP;
    configReq->traceConfig.traceS1AP = traceConfig->traceS1AP;
    configReq->traceConfig.traceReserved1 = traceConfig->traceReserved1;
    configReq->traceConfig.traceReserved2 = traceConfig->traceReserved2;
    configReq->traceConfig.traceReserved3 = traceConfig->traceReserved3;
    configReq->traceConfig.traceDestination = traceConfig->traceDestination;
    configReq->traceConfig.traceDestUdpPort = traceConfig->traceDestUdpPort;
    copyBytes( traceConfig->traceDestIPAddress, sizeof(traceConfig->traceDestIPAddress), &configReq->traceConfig.traceDestIPAddress );

}
void fillDehRhCfgReqMsg(
  IO MsgBsmDehRhConfigReq  *configReq
  )
{
  RadioHeadConfig *rhConfig     =  (RadioHeadConfig*)getFirstRowOfTable(RADIOHEADCONFIG);
  CellConfig      *cellConfig   =  (CellConfig*)getFirstRowOfTable(CELLCONFIG);

  configReq->transId         = getConfigTransId();
  configReq->radioHeadId     = 1;       /* TODO: Check what value needs to be filled */
  switch ( cellConfig->dlBandwidth )
  {
    case DLBANDWIDTH_5MHZ:  configReq->c1IQMap = 5;  break;
    case DLBANDWIDTH_10MHZ: configReq->c1IQMap = 10; break;
    case DLBANDWIDTH_20MHZ: configReq->c1IQMap = 20; break;
    default: BS_ERROR_UNSUPPORTED_PARAMETER( "COL_CELLCONFIG_DLBANDWIDTH", cellConfig->dlBandwidth);
    return;
  }
  configReq->txAttenPathA = rhConfig->txAttenPathA;
  configReq->txAttenPathB = rhConfig->txAttenPathB;
  configReq->txDelay = rhConfig->txDelay;
  configReq->rxDelay = rhConfig->rxDelay;


  BsmOamTableInfo *oamTableInfo = bsmGetTablePtrByTableId(CELLCONFIG);
  OamColInfo   *oamColInfo = &oamTableInfo->colInfo[COL_CELLCONFIG_DLEARFCN];

  if ( ( cellConfig->dlEarfcn >= oamColInfo->colMinVal && cellConfig->dlEarfcn <= oamColInfo->colMaxVal) )
  {
    configReq->txEarfcn =  cellConfig->dlEarfcn;
  }
  else
  {
    /* Look for special value */
    if ( ( oamColInfo->isSpecialValuePresent EQ TRUE ) && (oamColInfo->specialValue NOTEQ NULL) )
    {
      UINT32 specialValue = atoi(oamColInfo->specialValue);
      if ( specialValue EQ cellConfig->dlEarfcn )
      {
        configReq->txEarfcn = cellConfig->dlEarfcn;
      }
      else
      {
        BS_ERROR_UNSUPPORTED_PARAMETER( "COL_CELLCONFIG_DLEARFCN", cellConfig->dlEarfcn );
        return;
      }
    }
    else
    {
      BS_ERROR_UNSUPPORTED_PARAMETER( "COL_CELLCONFIG_DLEARFCN", cellConfig->dlEarfcn );
      return;
    }
  }

  oamColInfo = &oamTableInfo->colInfo[COL_CELLCONFIG_ULEARFCN];

  if ( ( cellConfig->ulEarfcn >= oamColInfo->colMinVal && cellConfig->ulEarfcn <= oamColInfo->colMaxVal) )
  {
    configReq->rxEarfcn =  cellConfig->ulEarfcn;
  }
  else
  {
    /* Look for special value */
    if ( ( oamColInfo->isSpecialValuePresent EQ TRUE ) && (oamColInfo->specialValue NOTEQ NULL) )
    {
      UINT32 specialValue = atoi(oamColInfo->specialValue);
      if ( specialValue EQ cellConfig->ulEarfcn )
      {
        configReq->rxEarfcn =  cellConfig->ulEarfcn;
      }
      else
      {
        BS_ERROR_UNSUPPORTED_PARAMETER( "COL_CELLCONFIG_ULEARFCN", cellConfig->ulEarfcn );
        return;
      }
    }
    else
    {
      BS_ERROR_UNSUPPORTED_PARAMETER( "COL_CELLCONFIG_ULEARFCN", cellConfig->ulEarfcn );
      return;
    }
  }
  
  configReq->vswrShutdownAlarmThreshold   = rhConfig->vswrShutdownAlarmThreshold;
  configReq->overpowerMinorAlarmThreshold = rhConfig->overpowerAlarmThreshold;
  configReq->lowpowerAlarmThreshold       = rhConfig->lowpowerAlarmThreshold;
}

/**
 * @details This function will fill and send MSG_BSM_DEH_RH_CONFIG_REQ message to DEH
 */
void bsmFillAndSendDehRhCfgReqMsg()
{
  MsgBsmDehRhConfigReq   configReq;
  clearBytes( &configReq, sizeof(MsgBsmDehRhConfigReq) );

  fillDehRhCfgReqMsg( &configReq );
  createAndSendMsg( MSG_BSM_DEH_RH_CONFIG_REQ, (UINT8*)&configReq, sizeof(MsgBsmDehRhConfigReq) );
}


/**
 * @param msg        Message from configured module
 * @param bsmContext BSM Context
 * @details This function will process MSG_XXX_BSM_CONFIG_SUCC message.
 */
BsmProceduralState bsmProcEnbConfigSucc(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  ModuleId       msgRvcdFromMod = MODID_MAX;
  EnbConfigSucc  *rspSucc = (EnbConfigSucc*)msg->data;

  msgRvcdFromMod = setConfigRspRcvdFromModule( msg->msgId, bsmContext );

  if ( rspSucc->transId EQ bsmContext->configTransId )
  {
    clearConfigurationFailedAlarm( msg->msgId, msgRvcdFromMod, bsmContext );
  }
  else
  {
    bsmContext->isCfgRspWithWrongTransIdRcvd = TRUE;
    BS_ERROR_INVALID_TRANSACTION_ID( rspSucc->transId, getModuleName(msgRvcdFromMod), bsmContext->configTransId );
  }

  if( TRUE EQ isConfigRspRcvdFromAllMod( &bsmContext->cfgRspFlags ) )
  {
    stopBsmProcedureTimer( TIMERNAME_WAITCONFIGCOMPLETE );
    setBsmProceduralState( bsmContext, BS_IDLE );
    checkForConfigProcedureCompletion( bsmContext );
    if ( TRUE EQ bsmContext->isConfigSuccessful  )
    {
      /* Read and send log config to all modules */
      bsmInitSetLogLevelProcedure();
      enbEnable( bsmContext );
    }
    else
    {
      BS_ERROR_CONFIG_PROCEDURE_FAILED();
      BS_ERROR_ENABLE_PROCEDURE_NOT_STARTED();
    }
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param msg        Message from PMH module
 * @param bsmContext BSM Context
 * @details This function will process MSG_PMH_BSM_CONFIG_SUCC message.
 */
BsmProceduralState bsmProcPmhConfigSucc(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  ModuleId       msgRvcdFromMod = MODID_MAX;
  EnbConfigSucc  *rspSucc = (EnbConfigSucc*)msg->data;

  if ( rspSucc->transId EQ bsmContext->configTransId )
  {
    clearConfigurationFailedAlarm( msg->msgId, msgRvcdFromMod, bsmContext );
  }
  else
  {
    bsmContext->isCfgRspWithWrongTransIdRcvd = TRUE;
    BS_ERROR_INVALID_TRANSACTION_ID( rspSucc->transId, getModuleName(msgRvcdFromMod), bsmContext->configTransId );
  }
  return bsmContext->bsmProceduralState;
}
/**
 * @param msg        Message from configured module
 * @param bsmContext BSM Context
 * @details This function will process MSG_XXX_BSM_CONFIG_FAIL message.
 */
BsmProceduralState bsmProcEnbConfigFail(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  ModuleId  msgRvcdFromMod = MODID_MAX;
  MsgUehBsmEnbConfigFail  *rspFail = (MsgUehBsmEnbConfigFail*)msg->data;

  msgRvcdFromMod = setConfigRspRcvdFromModule( msg->msgId, bsmContext );

  /* Create configuration fail occurred alarm */
  createConfigurationFailedAlarm( msgRvcdFromMod, "CONFIG REQ Failed" );
  BS_ERROR_RCVD_CONFIG_FAIL(getModuleName(msgRvcdFromMod),rspFail->transId,
                                    rspFail->errorDesc.errorNum, rspFail->errorDesc.failReason );

  if ( rspFail->transId NOTEQ bsmContext->configTransId )
  {
    bsmContext->isCfgRspWithWrongTransIdRcvd = TRUE;
    BS_ERROR_INVALID_TRANSACTION_ID( rspFail->transId, getModuleName(msgRvcdFromMod), bsmContext->configTransId );
  }

  if( TRUE EQ isConfigRspRcvdFromAllMod( &bsmContext->cfgRspFlags ) )
  {
    BS_ERROR_CONFIG_PROCEDURE_FAILED();
    BS_ERROR_ENABLE_PROCEDURE_NOT_STARTED();
    stopBsmProcedureTimer( TIMERNAME_WAITCONFIGCOMPLETE );
    setBsmProceduralState( bsmContext, BS_IDLE );
  }
  return bsmContext->bsmProceduralState;
}

/**
 * @param msg        Message from PMH module
 * @param bsmContext BSM Context
 * @details This function will process MSG_PMH_BSM_CONFIG_FAIL message.
 */
BsmProceduralState bsmProcPmhConfigFail(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  )
{
  ModuleId  msgRvcdFromMod = MODID_MAX;
  MsgUehBsmEnbConfigFail  *rspFail = (MsgUehBsmEnbConfigFail*)msg->data;

  /* Create configuration fail occurred alarm */
  createConfigurationFailedAlarm( msgRvcdFromMod, "CONFIG REQ Failed" );
  BS_ERROR_RCVD_CONFIG_FAIL(getModuleName(msgRvcdFromMod),rspFail->transId,
                                    rspFail->errorDesc.errorNum, rspFail->errorDesc.failReason );

  if ( rspFail->transId NOTEQ bsmContext->configTransId )
  {
    bsmContext->isCfgRspWithWrongTransIdRcvd = TRUE;
    BS_ERROR_INVALID_TRANSACTION_ID( rspFail->transId, getModuleName(msgRvcdFromMod), bsmContext->configTransId );
  }
  return bsmContext->bsmProceduralState;
}

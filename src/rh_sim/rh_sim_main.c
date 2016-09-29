 /**
 * @file    rh_sim_main.c
 * @brief This file will handle all the messages coming from different modules
 *          in any DEH state.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author : 
 * Date   :
 * Description :  
 */

#include "rh_sim.h"
#include <time.h>
#include "imc_timer.h"
#include "basic_dec.h"

extern SINT32 g_sockFd;
extern FILE *rhSimulatorLog;

TBOOL g_NotiAckReceived = FALSE;

void initDehMtCtx()
{
  /* Get RH SIM Context */
  DehMtContext  *dehMtCtx = getGlobalDehMtContext();
  clearBytes(dehMtCtx, sizeof(DehMtContext));
}

TBOOL readSimulatorConfigFile( )
{
  config_t          cfg;
  config_setting_t *table;
  UINT8             fullFilePath[MAX_BUFFER_SIZE];
  const char *enodebIpAddr, *radioHeadIpAddr, *version;

  clearBytes( &fullFilePath, sizeof(fullFilePath));

  /* Get RH SIM Context */
  DehMtContext  *dehMtCtx = getGlobalDehMtContext();

  config_init(&cfg);
  snprintf( fullFilePath, MAX_BUFFER_SIZE, "%s", "../simulators/simulatorConfig.cfg" );

  /* Read the file. If there is an error, report it and exit. */
  if(!config_read_file(&cfg, fullFilePath))
  {
    fprintf(rhSimulatorLog, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    fprintf(rhSimulatorLog, "Please add simulatorConfig.cfg file in simulator directory.\n");
    return FALSE;
  }
  /* Reading IP addresses of RH and EnodeB and Version of RH-SIM running */
  table = config_lookup(&cfg, "radioHeadSimulatorConfig");
  config_setting_lookup_string(table, "enodebIpAddr", &enodebIpAddr);
  strncpy(dehMtCtx->eNodeBIpAddr, enodebIpAddr , MAX_SIZE_ENBSTATUS_ENODEBIPADDRESS );
  config_setting_lookup_string(table, "radioHeadIpAddr", &radioHeadIpAddr);
  strncpy(dehMtCtx->rhIpAddr, radioHeadIpAddr, MAX_SIZE_ENBADDRESSES_RADIOHEADIPADDRESS );
  config_setting_lookup_string(table, "version",  &version );
  strncpy(dehMtCtx->version, version , MAX_SIZE_SWPACKAGESTATUS_VERSION );
  config_destroy(&cfg);
  return TRUE;
}

TBOOL readRhSimConfigFile()
{
  config_t          cfg;
  config_setting_t *table;
  UINT8      fullFilePath[MAX_BUFFER_SIZE];
  const char *desc;
  int        val;

  clearBytes( &fullFilePath, sizeof(fullFilePath));

  /* Get RH SIM Context */
  DehMtContext  *dehMtCtx = getGlobalDehMtContext();
  config_init(&cfg);

  snprintf( fullFilePath, MAX_BUFFER_SIZE, "%s", "../simulators/lksimconfig.cfg" );

   /* Read the file. If there is an error, report it and exit. */
  if(!config_read_file(&cfg, fullFilePath))
  {
    fprintf(rhSimulatorLog, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    fprintf(rhSimulatorLog, "Please add lksimconfig.cfg file in config file directory.\n");
    return FALSE;
  }
  table = config_lookup(&cfg, "rhResetRsp");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->rhResetRsp = (SINT16)val;

  table = config_lookup(&cfg, "rhInitNoti");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->rhInitNoti = (SINT16)val;

  table = config_lookup(&cfg, "rhFaControlRsp");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->rhFaControlRsp = (SINT16)val;

  table = config_lookup(&cfg, "rhDataPathSetRsp");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->rhDataPathSetRsp = (SINT16)val;

  table = config_lookup(&cfg, "rhDelaySettingRsp");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->rhDelaySettingRsp = (SINT16)val;

  table = config_lookup(&cfg, "rhTxOnPathA");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->rhTxOnPathA = (SINT16)val;

  table = config_lookup(&cfg, "rhTxOnPathB");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->rhTxOnPathB = (SINT16)val;

  table = config_lookup(&cfg, "rhTxOffPathA");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->rhTxOffPathA = (SINT16)val;

  table = config_lookup(&cfg, "rhTxOffPathB");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->rhTxOffPathB = (SINT16)val;

  table = config_lookup(&cfg, "rhTxEnablePathA");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->rhTxEnablePathA = (SINT16)val;

  table = config_lookup(&cfg, "rhTxEnablePathB");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->rhTxEnablePathB = (SINT16)val;

  table = config_lookup(&cfg, "rhTxDisablePathA");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->rhTxDisablePathA = (SINT16)val;

  table = config_lookup(&cfg, "rhTxDisablePathB");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->rhTxDisablePathB = (SINT16)val;
  /* Alarm Threshold  RSP */
  table = config_lookup(&cfg, "rhAlarmThresholdRsp");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->rhAlarmThresholdRsp = (SINT16)val;
  /* HW Info RSP */
  table = config_lookup(&cfg, "rhHwInfoRsp");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->hwInfoRsp.varResult = (SINT16)val;
  config_setting_lookup_int(table, "version", &val);
  dehMtCtx->hwInfoRsp.hwVer     = (UINT32)val;
  config_setting_lookup_string(table, "desc",  &desc );
  memcpy(dehMtCtx->hwInfoRsp.description , desc, MAX_HWINFO_DESC);
  /* FW Info RSP */
  table = config_lookup(&cfg, "rhFwInfoRsp");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->fwInfoRsp.varResult = (SINT16)val;
  config_setting_lookup_int(table, "version", &val);
  dehMtCtx->fwInfoRsp.runningVersion     = (UINT32)val;
  config_setting_lookup_int(table, "buildDate", &val);
  dehMtCtx->fwInfoRsp.runningVersionBuildDate    = (UINT32)val;
  /* Status RSP */
  table = config_lookup(&cfg, "rhStatusRsp");
  config_setting_lookup_int(table, "varResult", &val);
  dehMtCtx->statusInfoRsp.varResult = (SINT16)val;
  config_setting_lookup_int(table, "sequenceNo", &val);
  dehMtCtx->statusInfoRsp.sequenceNo     = (UINT16)val;
  config_setting_lookup_int(table, "c1IQMap", &val);
  dehMtCtx->statusInfoRsp.iqMap[0]    = (UINT8)val;
  config_setting_lookup_int(table, "c2IQMap", &val);
  dehMtCtx->statusInfoRsp.iqMap[1]    = (UINT8)val;
  config_setting_lookup_int(table, "c3IQMap", &val);
  dehMtCtx->statusInfoRsp.iqMap[2]    = (UINT8)val;
  config_setting_lookup_int(table, "c4IQMap", &val);
  dehMtCtx->statusInfoRsp.iqMap[3]    = (UINT8)val;
  config_setting_lookup_int(table, "fwMode", &val);
  dehMtCtx->statusInfoRsp.fwMode    = (UINT8)val;
  config_setting_lookup_int(table, "numOfFa", &val);
  dehMtCtx->statusInfoRsp.numberOfFA  = (UINT8)val;
  config_setting_lookup_int(table, "mappingOf1FAStatus", &val);
  dehMtCtx->statusInfoRsp.mappingOfFAStatus[0]    = (UINT8)val;
  config_setting_lookup_int(table, "mappingOf2FAStatus", &val);
  dehMtCtx->statusInfoRsp.mappingOfFAStatus[1]    = (UINT8)val;
  config_setting_lookup_int(table, "mappingOf3FAStatus", &val);
  dehMtCtx->statusInfoRsp.mappingOfFAStatus[2]    = (UINT8)val;
  config_setting_lookup_int(table, "mappingOf4FAStatus", &val);
  dehMtCtx->statusInfoRsp.mappingOfFAStatus[3]    = (UINT8)val;

  config_setting_lookup_int(table, "cvc1", &val);
  dehMtCtx->statusInfoRsp.cvc[0]    = (UINT8)val;
  config_setting_lookup_int(table, "cvc2", &val);
  dehMtCtx->statusInfoRsp.cvc[1]    = (UINT8)val;
  config_setting_lookup_int(table, "cvc3", &val);
  dehMtCtx->statusInfoRsp.cvc[2]    = (UINT8)val;
  config_setting_lookup_int(table, "cvc4", &val);
  dehMtCtx->statusInfoRsp.cvc[3]    = (UINT8)val;

  config_setting_lookup_int(table, "txFilterBandwidth", &val);
  dehMtCtx->statusInfoRsp.txFilterBandwidth    = (UINT8)val;
  config_setting_lookup_int(table, "rxFilterBandwidth", &val);
  dehMtCtx->statusInfoRsp.rxFilterBandwidth    = (UINT8)val;

  config_setting_lookup_int(table, "dlRatio", &val);
  dehMtCtx->statusInfoRsp.dlRatio    = (UINT8)val;
  config_setting_lookup_int(table, "ulRatio", &val);
  dehMtCtx->statusInfoRsp.ulRatio    = (UINT8)val;

  config_setting_lookup_int(table, "vswrShutdownAlarmThreshold", &val);
  dehMtCtx->statusInfoRsp.vswrShutdownAlarmThreshold    = (UINT16)val;
  config_setting_lookup_int(table, "overpowerShutdownAlarmThreshold", &val);
  dehMtCtx->statusInfoRsp.overPowerShutdownAlarmThreshold    = (UINT32)val;
  config_setting_lookup_int(table, "lowPowerAlarmThreshold", &val);
  dehMtCtx->statusInfoRsp.lowPowerAlarmThreshold    = (UINT32)val;
  config_setting_lookup_int(table, "centerFrequency", &val);
  dehMtCtx->statusInfoRsp.centerFrequency    = (UINT32)val;

  config_setting_lookup_int(table, "fa1ChannelBandwidthStatus", &val);
  dehMtCtx->statusInfoRsp.faChannelBandwidthStatus[0]    = (UINT8)val;
  config_setting_lookup_int(table, "fa2ChannelBandwidthStatus", &val);
  dehMtCtx->statusInfoRsp.faChannelBandwidthStatus[1]   = (UINT8)val;
  config_setting_lookup_int(table, "fa3ChannelBandwidthStatus", &val);
  dehMtCtx->statusInfoRsp.faChannelBandwidthStatus[2]    = (UINT8)val;
  config_setting_lookup_int(table, "fa4ChannelBandwidthStatus", &val);
  dehMtCtx->statusInfoRsp.faChannelBandwidthStatus[3]   = (UINT8)val;

  config_setting_lookup_int(table, "tx1stFaFrequency", &val);
  dehMtCtx->statusInfoRsp.txFaFrequency[0]    = (UINT32)val;
  config_setting_lookup_int(table, "tx2ndFaFrequency", &val);
  dehMtCtx->statusInfoRsp.txFaFrequency[1]    = (UINT32)val;
  config_setting_lookup_int(table, "tx3rdFaFrequency", &val);
  dehMtCtx->statusInfoRsp.txFaFrequency[2]    = (UINT32)val;
  config_setting_lookup_int(table, "tx4thFaFrequency", &val);
  dehMtCtx->statusInfoRsp.txFaFrequency[3]    = (UINT32)val;

  config_setting_lookup_int(table, "rx1stFaFrequency", &val);
  dehMtCtx->statusInfoRsp.rxFaFrequency[0]    = (UINT32)val;
  config_setting_lookup_int(table, "rx2ndFaFrequency", &val);
  dehMtCtx->statusInfoRsp.rxFaFrequency[1]    = (UINT32)val;
  config_setting_lookup_int(table, "rx3rdFaFrequency", &val);
  dehMtCtx->statusInfoRsp.rxFaFrequency[2]    = (UINT32)val;
  config_setting_lookup_int(table, "rx4thFaFrequency", &val);
  dehMtCtx->statusInfoRsp.rxFaFrequency[3]    = (UINT32)val;

  config_setting_lookup_int(table, "txDelay", &val);
  dehMtCtx->statusInfoRsp.txDelay    = (UINT32)val;
  config_setting_lookup_int(table, "rxDelay", &val);
  dehMtCtx->statusInfoRsp.rxDelay    = (UINT32)val;

  config_setting_lookup_int(table, "txPathAFaControl", &val);
  dehMtCtx->statusInfoRsp.txPathFaControl[0]    = (UINT8)val;
  config_setting_lookup_int(table, "txPathBFaControl", &val);
  dehMtCtx->statusInfoRsp.txPathFaControl[1]   = (UINT8)val;
  config_setting_lookup_int(table, "txPathCFaControl", &val);
  dehMtCtx->statusInfoRsp.txPathFaControl[2]    = (UINT8)val;
  config_setting_lookup_int(table, "txPathDFaControl", &val);
  dehMtCtx->statusInfoRsp.txPathFaControl[3]   = (UINT8)val;

  config_setting_lookup_int(table, "rxPathAFaControl", &val);
  dehMtCtx->statusInfoRsp.rxPathFaControl[0]    = (UINT8)val;
  config_setting_lookup_int(table, "rxPathBFaControl", &val);
  dehMtCtx->statusInfoRsp.rxPathFaControl[1]   = (UINT8)val;
  config_setting_lookup_int(table, "rxPathCFaControl", &val);
  dehMtCtx->statusInfoRsp.rxPathFaControl[2]    = (UINT8)val;
  config_setting_lookup_int(table, "rxPathDFaControl", &val);
  dehMtCtx->statusInfoRsp.rxPathFaControl[3]   = (UINT8)val;

  config_setting_lookup_int(table, "multiMode", &val);
  dehMtCtx->statusInfoRsp.multiMode   = (UINT8)val;
  config_setting_lookup_int(table, "powerBoost", &val);
  dehMtCtx->statusInfoRsp.powerBoost   = (UINT8)val;
  config_setting_lookup_int(table, "filterSwithingMode", &val);
  dehMtCtx->statusInfoRsp.filterSwithingMode   = (UINT8)val;


  /* PATH A Status */
  config_setting_lookup_int(table, "txOnOrOffPathA", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].txOnOrOff    = (UINT8)val;
  config_setting_lookup_int(table, "txEnableOrDisablePathA", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].txEnableOrDisable     = (UINT8)val;
  config_setting_lookup_int(table, "temperaturePathA", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].temperature    = (SINT16)val;
  config_setting_lookup_int(table, "txRfPowerPathA", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].txRfPower     = (SINT16)val;
  config_setting_lookup_int(table, "pathAReturnLoss", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].returnLoss    = (SINT16)val;
  config_setting_lookup_int(table, "pathAFa1RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].digitalIqLevelFaRssi[0]     = (UINT32)val;
  config_setting_lookup_int(table, "pathAFa2RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].digitalIqLevelFaRssi[1]     = (UINT32)val;
  config_setting_lookup_int(table, "pathAFa3RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].digitalIqLevelFaRssi[2]     = (UINT32)val;
  config_setting_lookup_int(table, "pathAFa4RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].digitalIqLevelFaRssi[3]     = (UINT32)val;
  config_setting_lookup_int(table, "pathAFa1TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].digitalIqLevelFaTssi[0]     = (UINT32)val;
  config_setting_lookup_int(table, "pathAFa2TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].digitalIqLevelFaTssi[1]     = (UINT32)val;
  config_setting_lookup_int(table, "pathAFa3TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].digitalIqLevelFaTssi[2]     = (UINT32)val;
  config_setting_lookup_int(table, "pathAFa4TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].digitalIqLevelFaTssi[3]     = (UINT32)val;
  config_setting_lookup_int(table, "pathAFa1TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].pathFaTxAtten[0]     = (UINT16)val;
  config_setting_lookup_int(table, "pathAFa2TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].pathFaTxAtten[1]     = (UINT16)val;
  config_setting_lookup_int(table, "pathAFa3TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].pathFaTxAtten[2]     = (UINT16)val;
  config_setting_lookup_int(table, "pathAFa4TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[0].pathFaTxAtten[3]     = (UINT16)val;

  /* PATH B Status */
  config_setting_lookup_int(table, "txOnOrOffPathB", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].txOnOrOff    = (UINT8)val;
  config_setting_lookup_int(table, "txEnableOrDisablePathB", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].txEnableOrDisable     = (UINT8)val;
  config_setting_lookup_int(table, "temperaturePathB", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].temperature    = (SINT16)val;
  config_setting_lookup_int(table, "txRfPowerPathB", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].txRfPower     = (SINT16)val;
  config_setting_lookup_int(table, "pathBReturnLoss", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].returnLoss    = (SINT16)val;
  config_setting_lookup_int(table, "pathBFa1RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].digitalIqLevelFaRssi[0]     = (UINT32)val;
  config_setting_lookup_int(table, "pathBFa2RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].digitalIqLevelFaRssi[1]     = (UINT32)val;
  config_setting_lookup_int(table, "pathBFa3RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].digitalIqLevelFaRssi[2]     = (UINT32)val;
  config_setting_lookup_int(table, "pathBFa4RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].digitalIqLevelFaRssi[3]     = (UINT32)val;
  config_setting_lookup_int(table, "pathBFa1TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].digitalIqLevelFaTssi[0]     = (UINT32)val;
  config_setting_lookup_int(table, "pathBFa2TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].digitalIqLevelFaTssi[1]     = (UINT32)val;
  config_setting_lookup_int(table, "pathBFa3TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].digitalIqLevelFaTssi[2]     = (UINT32)val;
  config_setting_lookup_int(table, "pathBFa4TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].digitalIqLevelFaTssi[3]     = (UINT32)val;
  config_setting_lookup_int(table, "pathBFa1TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].pathFaTxAtten[0]     = (UINT16)val;
  config_setting_lookup_int(table, "pathBFa2TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].pathFaTxAtten[1]     = (UINT16)val;
  config_setting_lookup_int(table, "pathBFa3TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].pathFaTxAtten[2]     = (UINT16)val;
  config_setting_lookup_int(table, "pathBFa4TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[1].pathFaTxAtten[3]     = (UINT16)val;

 /* PATH C Status */
  config_setting_lookup_int(table, "txOnOrOffPathC", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].txOnOrOff    = (UINT8)val;
  config_setting_lookup_int(table, "txEnableOrDisablePathC", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].txEnableOrDisable     = (UINT8)val;
  config_setting_lookup_int(table, "temperaturePathC", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].temperature    = (SINT16)val;
  config_setting_lookup_int(table, "txRfPowerPathC", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].txRfPower     = (SINT16)val;
  config_setting_lookup_int(table, "pathCReturnLoss", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].returnLoss    = (SINT16)val;
  config_setting_lookup_int(table, "pathCFa1RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].digitalIqLevelFaRssi[0]     = (UINT32)val;
  config_setting_lookup_int(table, "pathCFa2RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].digitalIqLevelFaRssi[1]     = (UINT32)val;
  config_setting_lookup_int(table, "pathCFa3RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].digitalIqLevelFaRssi[2]     = (UINT32)val;
  config_setting_lookup_int(table, "pathCFa4RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].digitalIqLevelFaRssi[3]     = (UINT32)val;
  config_setting_lookup_int(table, "pathCFa1TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].digitalIqLevelFaTssi[0]     = (UINT32)val;
  config_setting_lookup_int(table, "pathCFa2TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].digitalIqLevelFaTssi[1]     = (UINT32)val;
  config_setting_lookup_int(table, "pathCFa3TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].digitalIqLevelFaTssi[2]     = (UINT32)val;
  config_setting_lookup_int(table, "pathCFa4TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].digitalIqLevelFaTssi[3]     = (UINT32)val;
  config_setting_lookup_int(table, "pathCFa1TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].pathFaTxAtten[0]     = (UINT16)val;
  config_setting_lookup_int(table, "pathCFa2TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].pathFaTxAtten[1]     = (UINT16)val;
  config_setting_lookup_int(table, "pathCFa3TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].pathFaTxAtten[2]     = (UINT16)val;
  config_setting_lookup_int(table, "pathCFa4TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[2].pathFaTxAtten[3]     = (UINT16)val;

/* PATH D Status */
  config_setting_lookup_int(table, "txOnOrOffPathD", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].txOnOrOff    = (UINT8)val;
  config_setting_lookup_int(table, "txEnableOrDisablePathD", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].txEnableOrDisable     = (UINT8)val;
  config_setting_lookup_int(table, "temperaturePathD", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].temperature    = (SINT16)val;
  config_setting_lookup_int(table, "txRfPowerPathD", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].txRfPower     = (SINT16)val;
  config_setting_lookup_int(table, "pathDReturnLoss", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].returnLoss    = (SINT16)val;
  config_setting_lookup_int(table, "pathDFa1RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].digitalIqLevelFaRssi[0]     = (UINT32)val;
  config_setting_lookup_int(table, "pathDFa2RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].digitalIqLevelFaRssi[1]     = (UINT32)val;
  config_setting_lookup_int(table, "pathDFa3RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].digitalIqLevelFaRssi[2]     = (UINT32)val;
  config_setting_lookup_int(table, "pathDFa4RssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].digitalIqLevelFaRssi[3]     = (UINT32)val;
  config_setting_lookup_int(table, "pathDFa1TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].digitalIqLevelFaTssi[0]     = (UINT32)val;
  config_setting_lookup_int(table, "pathDFa2TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].digitalIqLevelFaTssi[1]     = (UINT32)val;
  config_setting_lookup_int(table, "pathDFa3TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].digitalIqLevelFaTssi[2]     = (UINT32)val;
  config_setting_lookup_int(table, "pathDFa4TssiDigitalIQLevel", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].digitalIqLevelFaTssi[3]     = (UINT32)val;
  config_setting_lookup_int(table, "pathDFa1TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].pathFaTxAtten[0]     = (UINT16)val;
  config_setting_lookup_int(table, "pathDFa2TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].pathFaTxAtten[1]     = (UINT16)val;
  config_setting_lookup_int(table, "pathDFa3TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].pathFaTxAtten[2]     = (UINT16)val;
  config_setting_lookup_int(table, "pathDFa4TxAtten", &val);
  dehMtCtx->statusInfoRsp.dataPath[3].pathFaTxAtten[3]     = (UINT16)val;

  /* Alarm RSP */
  table = config_lookup(&cfg, "rhAlarmRsp");
  config_setting_lookup_int(table, "result", &val);
  dehMtCtx->alarmInfoRsp.varResult = (SINT16)val;
  config_setting_lookup_int(table, "varSeqNum", &val);
  dehMtCtx->alarmInfoRsp.varSeqNum     = (UINT16)val;
  config_setting_lookup_int(table, "dataValidInd", &val);
  dehMtCtx->alarmInfoRsp.dataValidInd    = (UINT32)val;
  config_setting_lookup_int(table, "alarmDataCommon", &val);
  dehMtCtx->alarmInfoRsp.alarmDataCommon    = (UINT32)val;
  config_setting_lookup_int(table, "alarmDataPathA", &val);
  dehMtCtx->alarmInfoRsp.alarmDataPathA    = (UINT32)val;
  config_setting_lookup_int(table, "alarmDataPathB", &val);
  dehMtCtx->alarmInfoRsp.alarmDataPathB    = (UINT32)val;
  config_setting_lookup_int(table, "alarmDataPathC", &val);
  dehMtCtx->alarmInfoRsp.alarmDataPathC    = (UINT32)val;
  config_setting_lookup_int(table, "alarmDataPathD", &val);
  dehMtCtx->alarmInfoRsp.alarmDataPathD    = (UINT32)val;
  /* Timers Info */
  table = config_lookup(&cfg, "deh_mt_timers_cfg_table");
  TimersConfig    *dehMtTimerInfo = getTimerInfoRsp();
  const char       *moduleName = NULL, *description = NULL;
  int timerType = 0, timeUnit = 0, duration = 0;
  config_setting_lookup_string(table, "usedBy", &moduleName );
  config_setting_lookup_int(table, "timerName", &timerType );
  config_setting_lookup_int(table, "timeUnit",  &timeUnit );
  config_setting_lookup_int(table, "duration",  &duration );
  config_setting_lookup_string(table, "description",  &description );
  dehMtTimerInfo->moduleId    = MODID_MAX;
  dehMtTimerInfo->timerName = timerType;
  dehMtTimerInfo->timeUnit  = timeUnit;
  dehMtTimerInfo->duration  = duration;
  strncpy( dehMtTimerInfo->description, description, MAX_SIZE_TIMERSCONFIG_DESCRIPTION );

  config_destroy(&cfg);
  return TRUE;
}

/*
 * @details  This function encodes DehRhResetRsp message
 * @param    BsLkResetReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehHwInfoRsp(
  IN LkBsHwInfoRsp   *rsp,
  IO EncBuf         *encBuf
  )
{
  putUint16( rsp->varResult,encBuf );
  putUint16( rsp->varDummy,encBuf );
  putUint32( rsp->hwVer,encBuf );
  copyBytes( rsp->description, MAX_HWINFO_DESC, (encBuf->value) + 18);
  encBuf->len = encBuf->len + 243;
}

/*
 * @details  This function encodes DehRhResetRsp message
 * @param    BsLkResetReq : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehFwInfoRsp(
  IN LkBsFwInfoRsp   *rsp,
  IO EncBuf         *encBuf
  )
{
  putUint16( rsp->varResult,encBuf );
  putUint16( rsp->activeSide,encBuf );
  putUint32( rsp->runningVersion,encBuf );
  putUint32( rsp->runningVersionBuildDate,encBuf );
}


/*
 * @details  This function encodes ALARM message
 * @param    LkBsAlarmInfoRsp : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhAlarmRsp(
  IN LkBsAlarmInfoRsp    *rsp,
  IO EncBuf              *encBuf
  )
{
  putUint16( rsp->varResult, encBuf );
  putUint16( rsp->varSeqNum, encBuf );
  putUint32( rsp->dataValidInd, encBuf );
  putUint32( rsp->dataDummy1, encBuf);
  putUint32( rsp->dataDummy2, encBuf);
  putUint32( rsp->alarmDataCommon, encBuf);
  putUint32( rsp->alarmDataPathA, encBuf);
  putUint32( rsp->alarmDataPathB, encBuf);
  putUint32( rsp->alarmDataPathC, encBuf);
  putUint32( rsp->alarmDataPathD, encBuf);
}

/*
 * @details  This function encodes DehRhResetRsp message
 * @param    RhDehRsp : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhDefaultRsp(
  IN RhDehRsp        *rsp,
  IO EncBuf          *encBuf
  )
{
  putUint16( rsp->varResult,encBuf );
  putUint16( rsp->varDummy,encBuf );
}

/*
 * @details  This function encodes DehRhFaControlRsp message
 * @param    LkBsFaControlRsp : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhFaControlRsp(
  IN LkBsFaControlRsp        *rsp,
  IO EncBuf          *encBuf
  )
{
  putUint16( rsp->varResult,encBuf );
  putUint16( rsp->sequenceNo,encBuf );
  putUint8( rsp->dataDummy1,encBuf );
  putUint8( rsp->dataDummy2,encBuf );
  putUint16( rsp->dataDummy3,encBuf );
}

/*
 * @details  This function encodes DehRhEnableOrDisableRsp message
 * @param    LkBsEnableOrDisableRsp : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhEnableOrDisableRsp(
  IN LkBsEnableOrDisableRsp        *rsp,
  IO EncBuf          *encBuf
  )
{
  putUint16( rsp->varResult,encBuf );
  putUint16( rsp->sequenceNo,encBuf );
}
/*
 * @details  This function encodes DehRhTxOnOffRsp message
 * @param    LkBsTxOnOffRsp : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhTxOnOffRsp(
  IN LkBsTxOnOffRsp        *rsp,
  IO EncBuf          *encBuf
  )
{
  putUint16( rsp->varResult,encBuf );
  putUint16( rsp->sequenceNo,encBuf );
  putUint16( rsp->faId,encBuf );
  putUint16( rsp->dataDummy,encBuf );
}

/*
 * @details  This function encodes DehRhDataPathRsp message
 * @param    LkBsDataPathRsp : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhDataPathRsp(
  IN LkBsDataPathRsp        *rsp,
  IO EncBuf          *encBuf
  )
{
  putUint16( rsp->varResult, encBuf );
  putUint16( rsp->sequenceNo, encBuf );
  putUint8( rsp->dataDummy1, encBuf );
  putUint16( rsp->dataDummy2,encBuf );
}

/*
 * @details  This function encodes DehRhAlarmThresholdRsp message
 * @param    LkBsAlarmThresholdSettingRsp : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhAlarmThresholdRsp(
  IN LkBsAlarmThresholdSettingRsp        *rsp,
  IO EncBuf          *encBuf
  )
{
  putUint16( rsp->varResult, encBuf );
  putUint16( rsp->sequenceNo, encBuf );
}

/*
 * @details  This function encodes DehRhDelaySettingRsp message
 * @param    LkBsAlarmThresholdSettingRsp : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhDelaySettingRsp(
  IN LkBsDelaySettingRsp        *rsp,
  IO EncBuf          *encBuf
  )
{
  putUint16( rsp->varResult, encBuf );
  putUint16( rsp->sequenceNo, encBuf );
}

/*
 * @details  This function encodes DehRhDelaySettingRsp message
 * @param    LkBsAlarmThresholdSettingRsp : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehRhOperatingModeInfoRsp(
  IN LkBsOperationalModeInfoRsp        *rsp,
  IO EncBuf          *encBuf
  )
{
  putUint16( rsp->varResult, encBuf );
  putUint16( rsp->varMode, encBuf );
}

/*
 * @details  This function encodes STATUS message
 * @param    LkBsStatusInfoRsp : REQ structure input data
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encDehStatusRsp(
  IN LkBsStatusInfoRsp    *rsp,
  IO EncBuf               *encBuf
  )
{
  SINT16 multipleDummy = 0 ;
  putUint16( rsp->varResult,encBuf );
  putUint16( rsp->sequenceNo,encBuf );
  putUint8( rsp->iqMap[0],encBuf );
  putUint8( rsp->iqMap[1],encBuf );
  putUint8( rsp->iqMap[2],encBuf );
  putUint8( rsp->iqMap[3],encBuf );
  putUint8( rsp->dataDummy1,encBuf );
  putUint8( rsp->dataDummy2,encBuf );
  putUint8( rsp->dataDummy3,encBuf );
  putUint8( rsp->dataDummy4,encBuf );
  putUint8( rsp->fwMode,encBuf );
  putUint8( rsp->numberOfFA,encBuf );
  putUint16( rsp->dataDummy5,encBuf );
  putUint8( rsp->mappingOfFAStatus[0],encBuf );
  putUint8( rsp->mappingOfFAStatus[1],encBuf );
  putUint8( rsp->mappingOfFAStatus[2],encBuf );
  putUint8( rsp->mappingOfFAStatus[3],encBuf );
  putUint8( rsp->dataDummy6,encBuf );
  putUint8( rsp->dataDummy7,encBuf );
  putUint8( rsp->dataDummy8,encBuf );
  putUint8( rsp->dataDummy9,encBuf );
  putUint8( rsp->cvc[0],encBuf );
  putUint8( rsp->cvc[1],encBuf );
  putUint8( rsp->cvc[2],encBuf );
  putUint8( rsp->cvc[3],encBuf );
  putUint8( rsp->dataDummy10,encBuf );
  putUint8( rsp->dataDummy11,encBuf );
  putUint8( rsp->dataDummy12,encBuf );
  putUint8( rsp->dataDummy13,encBuf );
  putUint8( rsp->txFilterBandwidth,encBuf );
  putUint8( rsp->rxFilterBandwidth,encBuf );
  putUint8( rsp->dlRatio,encBuf );
  putUint8( rsp->ulRatio,encBuf );
  putUint16( rsp->vswrShutdownAlarmThreshold,encBuf );
  putUint16( rsp->dataDummy14,encBuf );
  putUint32( rsp->overPowerShutdownAlarmThreshold,encBuf );
  putUint32( rsp->lowPowerAlarmThreshold,encBuf );
  putUint32( rsp->centerFrequency,encBuf );
  putUint8( rsp->dataDummy15,encBuf );
  putUint8( rsp->dataDummy16,encBuf );
  putUint8( rsp->dataDummy17,encBuf );
  putUint8( rsp->dataDummy18,encBuf );
  putUint8( rsp->dataDummy19,encBuf );
  putUint8( rsp->dataDummy20,encBuf );
  putUint8( rsp->dataDummy21,encBuf );
  putUint8( rsp->dataDummy22,encBuf );
  putUint8( rsp->dataDummy23,encBuf );
  putUint8( rsp->dataDummy24,encBuf );
  putUint16( rsp->dataDummy25,encBuf );
  putUint16( rsp->dataDummy26,encBuf );
  putUint16( rsp->dataDummy27,encBuf );
  putUint8( rsp->faChannelBandwidthStatus[0],encBuf );
  putUint8( rsp->faChannelBandwidthStatus[1],encBuf );
  putUint8( rsp->faChannelBandwidthStatus[2],encBuf );
  putUint8( rsp->faChannelBandwidthStatus[3],encBuf );
  putUint8( rsp->dataDummy28,encBuf );
  putUint8( rsp->dataDummy29,encBuf );
  putUint8( rsp->dataDummy30,encBuf );
  putUint8( rsp->dataDummy31,encBuf );
  putUint32( rsp->txFaFrequency[0],encBuf );
  putUint32( rsp->txFaFrequency[1],encBuf );
  putUint32( rsp->txFaFrequency[2],encBuf );
  putUint32( rsp->txFaFrequency[3],encBuf );
  putUint32( rsp->dataDummy32,encBuf );
  putUint32( rsp->dataDummy33,encBuf );
  putUint32( rsp->dataDummy34,encBuf );
  putUint32( rsp->dataDummy35,encBuf );
  putUint32( rsp->rxFaFrequency[0],encBuf );
  putUint32( rsp->rxFaFrequency[1],encBuf );
  putUint32( rsp->rxFaFrequency[2],encBuf );
  putUint32( rsp->rxFaFrequency[3],encBuf );
  putUint32( rsp->dataDummy36,encBuf );
  putUint32( rsp->dataDummy37,encBuf );
  putUint32( rsp->dataDummy38,encBuf );
  putUint32( rsp->dataDummy39,encBuf );
  putUint32( rsp->dataDummy40,encBuf );
  putUint32( rsp->txDelay,encBuf );
  putUint32( rsp->rxDelay,encBuf );
  putUint16( rsp->dataDummy41,encBuf );
  putUint16( rsp->dataDummy42,encBuf );
  putUint16( rsp->dataDummy43,encBuf );
  putUint16( rsp->dataDummy44,encBuf );
  putUint16( rsp->dataDummy45,encBuf );
  putUint16( rsp->dataDummy46,encBuf );
  putUint16( rsp->dataDummy47,encBuf );
  putUint16( rsp->dataDummy48,encBuf );
  putUint8( rsp->dataDummy49,encBuf );
  putUint8( rsp->dataDummy50,encBuf );
  putUint8( rsp->dataDummy51,encBuf );
  putUint8( rsp->dataDummy52,encBuf );
  putUint8( rsp->dataDummy53,encBuf );
  putUint8( rsp->dataDummy54,encBuf );
  putUint8( rsp->dataDummy55,encBuf );
  putUint8( rsp->dataDummy56,encBuf );
  putUint8( rsp->dataDummy57,encBuf );

  putUint8( rsp->dataDummy58,encBuf );
  putUint16( rsp->dataDummy59,encBuf );

  putUint16( rsp->dataDummy60,encBuf );
  putUint16( rsp->dataDummy61,encBuf );
  putUint8( rsp->dataDummy62,encBuf );
  putUint8( rsp->dataDummy63,encBuf );
  putUint8( rsp->txPathFaControl[0],encBuf );
  putUint8( rsp->txPathFaControl[1],encBuf );
  putUint8( rsp->txPathFaControl[2],encBuf );
  putUint8( rsp->txPathFaControl[3],encBuf );
  putUint8( rsp->rxPathFaControl[0],encBuf );
  putUint8( rsp->rxPathFaControl[1],encBuf );
  putUint8( rsp->rxPathFaControl[2],encBuf );
  putUint8( rsp->rxPathFaControl[3],encBuf );
  for ( multipleDummy=0;multipleDummy< 42; multipleDummy++)
  {
	  putUint8( rsp->dataDummy64[multipleDummy],encBuf );
  }
  putUint8( rsp->multiMode,encBuf );
  putUint8( rsp->powerBoost,encBuf );
  putUint8( rsp->filterSwithingMode,encBuf );
  putUint8( rsp->dataDummy65,encBuf );

  /* PATH A */
  putUint8( rsp->dataPath[0].txOnOrOff,encBuf );
  putUint8( rsp->dataPath[0].txEnableOrDisable,encBuf );
  putUint8( rsp->dataPath[0].dataDummy1,encBuf );
  putUint8( rsp->dataPath[0].dataDummy2,encBuf );
  putUint16( rsp->dataPath[0].dataDummy3,encBuf );
  putUint16( rsp->dataPath[0].temperature,encBuf );
  putUint16( rsp->dataPath[0].txRfPower,encBuf );
  putUint16( rsp->dataPath[0].dataDummy4,encBuf );
  putUint16( rsp->dataPath[0].returnLoss,encBuf );
  putUint16( rsp->dataPath[0].dataDummy5,encBuf );
  putUint32( rsp->dataPath[0].digitalIqLevelFaRssi[0],encBuf );
  putUint32( rsp->dataPath[0].digitalIqLevelFaRssi[1],encBuf );
  putUint32( rsp->dataPath[0].digitalIqLevelFaRssi[2],encBuf );
  putUint32( rsp->dataPath[0].digitalIqLevelFaRssi[3],encBuf );
  putUint32( rsp->dataPath[0].dataDummy6,encBuf );
  putUint32( rsp->dataPath[0].dataDummy7,encBuf );
  putUint32( rsp->dataPath[0].dataDummy8,encBuf );
  putUint32( rsp->dataPath[0].dataDummy9,encBuf );
  putUint32( rsp->dataPath[0].digitalIqLevelFaTssi[0],encBuf );
  putUint32( rsp->dataPath[0].digitalIqLevelFaTssi[1],encBuf );
  putUint32( rsp->dataPath[0].digitalIqLevelFaTssi[2],encBuf );
  putUint32( rsp->dataPath[0].digitalIqLevelFaTssi[3],encBuf );
  putUint32( rsp->dataPath[0].dataDummy10,encBuf );
  putUint32( rsp->dataPath[0].dataDummy11,encBuf );
  putUint32( rsp->dataPath[0].dataDummy12,encBuf );
  putUint32( rsp->dataPath[0].dataDummy13,encBuf );
  putUint8( rsp->dataPath[0].dataDummy14,encBuf );
  putUint8( rsp->dataPath[0].dataDummy15,encBuf );
  putUint16( rsp->dataPath[0].dataDummy16,encBuf );
  putUint16( rsp->dataPath[0].pathFaTxAtten[0],encBuf );
  putUint16( rsp->dataPath[0].pathFaTxAtten[1],encBuf );
  putUint16( rsp->dataPath[0].pathFaTxAtten[2],encBuf );
  putUint16( rsp->dataPath[0].pathFaTxAtten[3],encBuf );
  putUint16( rsp->dataPath[0].dataDummy17,encBuf );
  putUint16( rsp->dataPath[0].dataDummy18,encBuf );
  putUint16( rsp->dataPath[0].dataDummy19,encBuf );
  putUint16( rsp->dataPath[0].dataDummy20,encBuf );


  /* PATH B */
  putUint8( rsp->dataPath[1].txOnOrOff,encBuf );
  putUint8( rsp->dataPath[1].txEnableOrDisable,encBuf );
  putUint8( rsp->dataPath[1].dataDummy1,encBuf );
  putUint8( rsp->dataPath[1].dataDummy2,encBuf );
  putUint16( rsp->dataPath[1].dataDummy3,encBuf );
  putUint16( rsp->dataPath[1].temperature,encBuf );
  putUint16( rsp->dataPath[1].txRfPower,encBuf );
  putUint16( rsp->dataPath[1].dataDummy4,encBuf );
  putUint16( rsp->dataPath[1].returnLoss,encBuf );
  putUint16( rsp->dataPath[1].dataDummy5,encBuf );
  putUint32( rsp->dataPath[1].digitalIqLevelFaRssi[0],encBuf );
  putUint32( rsp->dataPath[1].digitalIqLevelFaRssi[1],encBuf );
  putUint32( rsp->dataPath[1].digitalIqLevelFaRssi[2],encBuf );
  putUint32( rsp->dataPath[1].digitalIqLevelFaRssi[3],encBuf );
  putUint32( rsp->dataPath[1].dataDummy6,encBuf );
  putUint32( rsp->dataPath[1].dataDummy7,encBuf );
  putUint32( rsp->dataPath[1].dataDummy8,encBuf );
  putUint32( rsp->dataPath[1].dataDummy9,encBuf );
  putUint32( rsp->dataPath[1].digitalIqLevelFaTssi[0],encBuf );
  putUint32( rsp->dataPath[1].digitalIqLevelFaTssi[1],encBuf );
  putUint32( rsp->dataPath[1].digitalIqLevelFaTssi[2],encBuf );
  putUint32( rsp->dataPath[1].digitalIqLevelFaTssi[3],encBuf );
  putUint32( rsp->dataPath[1].dataDummy10,encBuf );
  putUint32( rsp->dataPath[1].dataDummy11,encBuf );
  putUint32( rsp->dataPath[1].dataDummy12,encBuf );
  putUint32( rsp->dataPath[1].dataDummy13,encBuf );
  putUint8( rsp->dataPath[1].dataDummy14,encBuf );
  putUint8( rsp->dataPath[1].dataDummy15,encBuf );
  putUint16( rsp->dataPath[1].dataDummy16,encBuf );
  putUint16( rsp->dataPath[1].pathFaTxAtten[0],encBuf );
  putUint16( rsp->dataPath[1].pathFaTxAtten[1],encBuf );
  putUint16( rsp->dataPath[1].pathFaTxAtten[2],encBuf );
  putUint16( rsp->dataPath[1].pathFaTxAtten[3],encBuf );
  putUint16( rsp->dataPath[1].dataDummy17,encBuf );
  putUint16( rsp->dataPath[1].dataDummy18,encBuf );
  putUint16( rsp->dataPath[1].dataDummy19,encBuf );
  putUint16( rsp->dataPath[1].dataDummy20,encBuf );

    /* PATH C */
  putUint8( rsp->dataPath[2].txOnOrOff,encBuf );
  putUint8( rsp->dataPath[2].txEnableOrDisable,encBuf );
  putUint8( rsp->dataPath[2].dataDummy1,encBuf );
  putUint8( rsp->dataPath[2].dataDummy2,encBuf );
  putUint16( rsp->dataPath[2].dataDummy3,encBuf );
  putUint16( rsp->dataPath[2].temperature,encBuf );
  putUint16( rsp->dataPath[2].txRfPower,encBuf );
  putUint16( rsp->dataPath[2].dataDummy4,encBuf );
  putUint16( rsp->dataPath[2].returnLoss,encBuf );
  putUint16( rsp->dataPath[2].dataDummy5,encBuf );
  putUint32( rsp->dataPath[2].digitalIqLevelFaRssi[0],encBuf );
  putUint32( rsp->dataPath[2].digitalIqLevelFaRssi[1],encBuf );
  putUint32( rsp->dataPath[2].digitalIqLevelFaRssi[2],encBuf );
  putUint32( rsp->dataPath[2].digitalIqLevelFaRssi[3],encBuf );
  putUint32( rsp->dataPath[2].dataDummy6,encBuf );
  putUint32( rsp->dataPath[2].dataDummy7,encBuf );
  putUint32( rsp->dataPath[2].dataDummy8,encBuf );
  putUint32( rsp->dataPath[2].dataDummy9,encBuf );
  putUint32( rsp->dataPath[2].digitalIqLevelFaTssi[0],encBuf );
  putUint32( rsp->dataPath[2].digitalIqLevelFaTssi[1],encBuf );
  putUint32( rsp->dataPath[2].digitalIqLevelFaTssi[2],encBuf );
  putUint32( rsp->dataPath[2].digitalIqLevelFaTssi[3],encBuf );
  putUint32( rsp->dataPath[2].dataDummy10,encBuf );
  putUint32( rsp->dataPath[2].dataDummy11,encBuf );
  putUint32( rsp->dataPath[2].dataDummy12,encBuf );
  putUint32( rsp->dataPath[2].dataDummy13,encBuf );
  putUint8( rsp->dataPath[2].dataDummy14,encBuf );
  putUint8( rsp->dataPath[2].dataDummy15,encBuf );
  putUint16( rsp->dataPath[2].dataDummy16,encBuf );
  putUint16( rsp->dataPath[2].pathFaTxAtten[0],encBuf );
  putUint16( rsp->dataPath[2].pathFaTxAtten[1],encBuf );
  putUint16( rsp->dataPath[2].pathFaTxAtten[2],encBuf );
  putUint16( rsp->dataPath[2].pathFaTxAtten[3],encBuf );
  putUint16( rsp->dataPath[2].dataDummy17,encBuf );
  putUint16( rsp->dataPath[2].dataDummy18,encBuf );
  putUint16( rsp->dataPath[2].dataDummy19,encBuf );
  putUint16( rsp->dataPath[2].dataDummy20,encBuf );

  /* PATH D */
  putUint8( rsp->dataPath[3].txOnOrOff,encBuf );
  putUint8( rsp->dataPath[3].txEnableOrDisable,encBuf );
  putUint8( rsp->dataPath[3].dataDummy1,encBuf );
  putUint8( rsp->dataPath[3].dataDummy2,encBuf );
  putUint16( rsp->dataPath[3].dataDummy3,encBuf );
  putUint16( rsp->dataPath[3].temperature,encBuf );
  putUint16( rsp->dataPath[3].txRfPower,encBuf );
  putUint16( rsp->dataPath[3].dataDummy4,encBuf );
  putUint16( rsp->dataPath[3].returnLoss,encBuf );
  putUint16( rsp->dataPath[3].dataDummy5,encBuf );
  putUint32( rsp->dataPath[3].digitalIqLevelFaRssi[0],encBuf );
  putUint32( rsp->dataPath[3].digitalIqLevelFaRssi[1],encBuf );
  putUint32( rsp->dataPath[3].digitalIqLevelFaRssi[2],encBuf );
  putUint32( rsp->dataPath[3].digitalIqLevelFaRssi[3],encBuf );
  putUint32( rsp->dataPath[3].dataDummy6,encBuf );
  putUint32( rsp->dataPath[3].dataDummy7,encBuf );
  putUint32( rsp->dataPath[3].dataDummy8,encBuf );
  putUint32( rsp->dataPath[3].dataDummy9,encBuf );
  putUint32( rsp->dataPath[3].digitalIqLevelFaTssi[0],encBuf );
  putUint32( rsp->dataPath[3].digitalIqLevelFaTssi[1],encBuf );
  putUint32( rsp->dataPath[3].digitalIqLevelFaTssi[2],encBuf );
  putUint32( rsp->dataPath[3].digitalIqLevelFaTssi[3],encBuf );
  putUint32( rsp->dataPath[3].dataDummy10,encBuf );
  putUint32( rsp->dataPath[3].dataDummy11,encBuf );
  putUint32( rsp->dataPath[3].dataDummy12,encBuf );
  putUint32( rsp->dataPath[3].dataDummy13,encBuf );
  putUint8( rsp->dataPath[3].dataDummy14,encBuf );
  putUint8( rsp->dataPath[3].dataDummy15,encBuf );
  putUint16( rsp->dataPath[3].dataDummy16,encBuf );
  putUint16( rsp->dataPath[3].pathFaTxAtten[0],encBuf );
  putUint16( rsp->dataPath[3].pathFaTxAtten[1],encBuf );
  putUint16( rsp->dataPath[3].pathFaTxAtten[2],encBuf );
  putUint16( rsp->dataPath[3].pathFaTxAtten[3],encBuf );
  putUint16( rsp->dataPath[3].dataDummy17,encBuf );
  putUint16( rsp->dataPath[3].dataDummy18,encBuf );
  putUint16( rsp->dataPath[3].dataDummy19,encBuf );
  putUint16( rsp->dataPath[3].dataDummy20,encBuf );
}

/*
 * @details  This function will encode header data.
 * @param    LkHdr : Header structure
 * @param    EncBuf : Buffer in which data will be encoded.
 */
void encRhDehHeader (
  IN LkHdr      *hdr,
  IO EncBuf     *buf
  )
{
  putUint16( hdr->dstMailbox, buf );
  putUint16( hdr->srcMailbox, buf );
  putUint32( hdr->msgId,      buf );
}

/*
 * @details  This function will send data on UPD socket to Lk.
 * @param    EncBuf  Data Buffer to be sent on the socket
 */
TBOOL sendMsgToDehRsp(
  IN EncBuf *encBuf
  )
{
  SINT32   numBytesSent=0;
 // UINT8    *dehIpAddr = DEH_IP_ADDR;

  UINT8    *dehIpAddr = getNodeBIpAddress();

 // printf("\n DEH_IP_ADDR is = %s",dehIpAddr);
  /* Send Message to LK using UDP socket */
  numBytesSent = sendUdpMsg( g_sockFd, dehIpAddr,BS_MAILBOX_ID, encBuf->value, encBuf->len);
  /* TODO : Need to check this condition */
  if(numBytesSent EQ -1 )
  {
    return FALSE;
  }
  return TRUE;
}

/*
 * @details  This function will send data on UPD socket to Lk.
 * @param    EncBuf  Data Buffer to be sent on the socket
 */
TBOOL sendMsgToDehNoti(
  IN EncBuf *encBuf
  )
{
  SINT32   numBytesSent=0;
 // UINT8    *dehIpAddr = DEH_IP_ADDR;
  UINT8    *dehIpAddr = getNodeBIpAddress();
  fprintf(rhSimulatorLog, "DEH_IP_ADDR is = %s\n",dehIpAddr);
  /* Send Message to LK using UDP socket */
  numBytesSent = sendUdpMsg( g_sockFd, dehIpAddr,BS_MAILBOX_ID, encBuf->value, encBuf->len);
  /* TODO : Need to check this condition */
  if(numBytesSent EQ -1 )
  {
    return FALSE;
  }
  return TRUE;
}

void procDehReqNoti(
  IN UINT32   msgId,
  IO LkHdr    *hdr
  )
{
  EncBuf     encBuf;
  RhDehRsp   rsp;

  clearBytes(&encBuf, sizeof(EncBuf));
  clearBytes(&rsp , sizeof(RhDehRsp));

  DehMtContext     *dehMtCtx = getGlobalDehMtContext();
  rsp.varDummy  = 0;
  rsp.varResult = dehMtCtx->rhInitNoti;
  hdr->msgId   =  msgId;
  encRhDehHeader( hdr, &encBuf );
  encDehRhDefaultRsp( &rsp, &encBuf );
  sendMsgToDehNoti(&encBuf);
}

void procDehAlarmNoti(
  IN UINT32   msgId,
  IO LkHdr    *hdr
  )
{
  EncBuf                encBuf;
  LkBsAlarmChangeNoti   *noti;

  clearBytes(&encBuf, sizeof(EncBuf));

  noti = getAlarmInfoRsp();
  hdr->msgId   =  msgId;
  encRhDehHeader( hdr, &encBuf );
  encDehRhAlarmRsp( noti, &encBuf );
  sendMsgToDehNoti(&encBuf);
}

void procDehAlarmInfoReq(
  IN UINT32   msgId,
  IN UINT32   sequenceNo,
  IO LkHdr    *hdr
  )
{
  EncBuf                encBuf;
  LkBsAlarmInfoRsp      *noti;

  clearBytes(&encBuf, sizeof(EncBuf));

  noti = getAlarmInfoRsp();
  hdr->msgId   =  msgId;
  noti->varSeqNum = sequenceNo;
  encRhDehHeader( hdr, &encBuf );
  encDehRhAlarmRsp( noti, &encBuf );
  sendMsgToDehRsp(&encBuf);
}


void procDehHwInfoReq(
  IN UINT32   msgId,
  IO LkHdr    *hdr
  )
{
  EncBuf          encBuf;
  LkBsHwInfoRsp   *rsp;

  clearBytes(&encBuf, sizeof(EncBuf));
  rsp = gethwInfoRsp();
  hdr->msgId   =  msgId;
  encRhDehHeader( hdr, &encBuf );
  encDehHwInfoRsp( rsp, &encBuf );
  sendMsgToDehRsp(&encBuf);
}

void procDehFwInfoReq(
  IN UINT32   msgId,
  IO LkHdr    *hdr
  )
{
  EncBuf          encBuf;
  LkBsFwInfoRsp   *rsp;

  clearBytes(&encBuf, sizeof(EncBuf));

  rsp = getfwInfoRsp();
  hdr->msgId   =  msgId;
  encRhDehHeader( hdr, &encBuf );
  encDehFwInfoRsp( rsp, &encBuf );
  sendMsgToDehRsp(&encBuf);
}

void procDehResetReq(
  IN UINT32   msgId,
  IO LkHdr    *hdr
  )
{
  EncBuf     encBuf;
  RhDehRsp   resetRsp;

  clearBytes(&encBuf, sizeof(EncBuf));
  clearBytes(&resetRsp , sizeof(RhDehRsp));

  DehMtContext     *dehMtCtx = getGlobalDehMtContext();
  resetRsp.varDummy  = 0;
  resetRsp.varResult = dehMtCtx->rhResetRsp;
  hdr->msgId   =  msgId;
  encRhDehHeader( hdr, &encBuf );
  encDehRhDefaultRsp( &resetRsp, &encBuf );
  sendMsgToDehRsp(&encBuf);
  g_NotiAckReceived = FALSE;   /* Radio head is in Not connected state */
}


void procDehOnOffReq(
  IN UINT32   msgId,
  IN UINT16   sequenceNo,
  IO LkHdr    *hdr,
  IO UINT8    path,
  IO UINT8    txOnOrOff
  )
{
  EncBuf     encBuf;
  LkBsTxOnOffRsp  rsp;

  clearBytes(&encBuf, sizeof(EncBuf));
  clearBytes(&rsp , sizeof(RhDehRsp));

  DehMtContext     *dehMtCtx = getGlobalDehMtContext();
  if (path EQ 0)
  {
    /* PATH A, TX OFF */
    if(txOnOrOff EQ 0)
    {
      rsp.varResult = dehMtCtx->rhTxOffPathA;
    }/* PATH A, TX ON */
    else
    {
      rsp.varResult = dehMtCtx->rhTxOnPathA;
    }
  }
  else
  {
    /* PATH B, TX OFF */
    if(txOnOrOff EQ 0)
    {
      rsp.varResult = dehMtCtx->rhTxOffPathB;
    }/* PATH B, TX ON */
    else
    {
      rsp.varResult = dehMtCtx->rhTxOnPathB;
    }
  }
  rsp.sequenceNo=sequenceNo;
  rsp.faId = 0;
  rsp.dataDummy = 0;
  hdr->msgId   =  msgId;
  encRhDehHeader( hdr, &encBuf );
  encDehRhTxOnOffRsp( &rsp, &encBuf );
  sendMsgToDehRsp(&encBuf);
}

TBOOL sendMsgToRhFromSim(
  IN EncBuf *encBuf
  )
{
  SINT32   numBytesSent=0;

  UINT8    *lkIpAddr = getDehMtRhIpAddress();
  fprintf(rhSimulatorLog, "\n LK IP ADDRESS is = %s",lkIpAddr);
  /* Send Message to LK using UDP socket */
  numBytesSent = sendUdpMsg( g_sockFd, lkIpAddr,LK_MAILBOX_ID, encBuf->value, encBuf->len);
  if(numBytesSent EQ -1 )
  {
    return FALSE;
  }
  return TRUE;
}


static void dehTimerHandler ()
{
  /* Another clock interval elapsed */
  TimerCtx *ctx = getDehMtTimerCtx();

  MsgTimeoutInd    msgInd;

  if( ctx->ticksRemaining > 0 )
  {
    ctx->ticksRemaining--;
    if(ctx->ticksRemaining EQ 0)
    {
      /* Timer expiry: inform user */
      /*logInfo("timer=%d expired", ctx->timerId);*/
      msgInd.timerDesc = ctx->timerDesc;
      msgInd.timerId   = ctx->timerId;
      // createAndSendMsg( ctx->timerDesc.timeoutMsgId, (UINT8*)&msgInd, sizeof(msgInd) );

      EncBuf             encBuf;
      LkHdr              hdr;
      TBOOL              result = FALSE;
      clearBytes( &hdr, sizeof(LkHdr) );
      hdr.dstMailbox = LK_MAILBOX_ID;
      hdr.srcMailbox = BS_MAILBOX_ID;
      hdr.msgId      = MSG_TIMEKEEPER_DEH_TIMEOUT_IND;

      initEncBuf(&encBuf);

      putUint16( hdr.dstMailbox, &encBuf );
      putUint16( hdr.srcMailbox, &encBuf );
      putUint32( hdr.msgId,      &encBuf );

      result = sendMsgToRhFromSim( &encBuf);
      ctx->isAllocated = FALSE;
      ctx->timerId = 0;
    }
  }
}

/**
 * @param timeSpec The duration of timer in minutes, or seconds, or milliseconds or micro seconds
 * @details This function Converts a timeSpec to equivalent in microsecond
 */
Duratn getDehMtDurationInUsec(
  IN TimeSpec *timeSpec
  )
{
  if(TIMEUNIT_USEC EQ timeSpec->timeUnit)
   return timeSpec->duration;

  if(TIMEUNIT_MSEC EQ timeSpec->timeUnit)
   return (timeSpec->duration * 1000);

  if(TIMEUNIT_SEC EQ timeSpec->timeUnit)
   return (timeSpec->duration * 1000 * 1000);

  if(TIMEUNIT_MINUTE EQ timeSpec->timeUnit)
   return (timeSpec->duration * 1000 * 1000 * 60);

  BS_ERROR_UNKNOWN_TIMEER_UNIT(timeSpec->timeUnit );
  return 0;
}
void startTimerForSim(
  IN TimeSpec tick
  )
{
  struct sigaction sa;
  struct itimerval timer;
  TimerCtx *timerCtx = getDehMtTimerCtx();

  memset(&timer, 0, sizeof(struct itimerval));

  timerCtx->isAllocated = TRUE;
  timerCtx->timerId = 1; /* Must never be changed again */
  timerCtx->timerDesc.timeoutMsgId = MSG_TIMEKEEPER_DEH_TIMEOUT_IND;
  timerCtx->timerDesc.timeSpec = tick;
  timerCtx->ticksRemaining = timerCtx->timerDesc.timeSpec.duration;

  /* OS can give a basic clock to framework in form of SIGALRM.
   * Register for alarm signal with OS
   */
  memset (&sa, 0, sizeof (sa));
  sa.sa_handler = &dehTimerHandler;
  sigaction (SIGALRM, &sa, NULL);
  siginterrupt( SIGALRM, 0);

  /* usec value cannot be greater than 1 sec. see setitimer man page */

  if (TIMEUNIT_SEC EQ tick.timeUnit)
  {
    timer.it_value.tv_sec = tick.duration;
    timer.it_interval.tv_sec = tick.duration;
    fprintf(rhSimulatorLog, "clock will tick at %u seconds \n", (UINT32) timer.it_value.tv_sec);
  }
  else
  {
    timer.it_value.tv_usec = getDehMtDurationInUsec(&tick);
    timer.it_interval.tv_usec = getDehMtDurationInUsec(&tick);
    fprintf(rhSimulatorLog, "clock will tick at %u usec", (UINT32) timer.it_value.tv_usec);
  }

  /* Start a virtual timer.  It counts down whenever this process is executing
   * MS:18May2010: TODO: comment does not match with actual usage.
   *               What do we want: real or virtual?
   */
  if(setitimer (ITIMER_REAL, &timer, NULL) EQ -1)
  {
    fprintf(rhSimulatorLog, "FATAL: could not start timer service, errno=%d, strerror=%s", errno, strerror(errno));
  }
}

void procDehEnableOrDisableReq(
  IN UINT32   msgId,
  IN UINT16   sequenceNo,
  IO LkHdr    *hdr,
  IO UINT8    path,
  IO UINT8    enableOrDisable
  )
{
  EncBuf     encBuf;
  LkBsEnableOrDisableRsp   rsp;

  clearBytes(&encBuf, sizeof(EncBuf));
  clearBytes(&rsp , sizeof(RhDehRsp));

  DehMtContext     *dehMtCtx = getGlobalDehMtContext();
  if (path EQ 0)
  {
    /* PATH A, TX DISABLE */
    if(enableOrDisable EQ 0)
    {
      rsp.varResult = dehMtCtx->rhTxDisablePathA;
      /* TODO : Start timer
       * After timer Expire send Alarm Change NOTI to DEH
       * */
      TimersConfig  *dehMtTimerInfo = getTimerInfoRsp();
      TimeSpec tick;
      clearBytes( &tick, sizeof(TimeSpec));
      tick.duration = dehMtTimerInfo->duration;
      tick.timeUnit = dehMtTimerInfo->timeUnit;
      startTimerForSim( tick );
    }/* PATH A, TX ENABLE */
    else
    {
      rsp.varResult = dehMtCtx->rhTxEnablePathA;
    }
  }
  else
  {
    /* PATH B, TX DISABLE */
    if(enableOrDisable EQ 0)
    {
      rsp.varResult = dehMtCtx->rhTxDisablePathB;
    }/* PATH B, TX ENABLE */
    else
    {
      rsp.varResult = dehMtCtx->rhTxEnablePathB;
    }
  }
  rsp.sequenceNo = sequenceNo;
  hdr->msgId   =  msgId;
  encRhDehHeader( hdr, &encBuf );
  encDehRhEnableOrDisableRsp( &rsp, &encBuf );
  sendMsgToDehRsp(&encBuf);
}

void procDehDataPathReq(
  IN UINT32   msgId,
  IN UINT16   sequenceNo,
  IO LkHdr    *hdr
  )
{
  EncBuf     encBuf;
  LkBsDataPathRsp   rsp;

  clearBytes(&encBuf, sizeof(EncBuf));
  clearBytes(&rsp , sizeof(RhDehRsp));

  DehMtContext     *dehMtCtx = getGlobalDehMtContext();
  rsp.varResult = dehMtCtx->rhDataPathSetRsp;
  rsp.sequenceNo= sequenceNo;
  rsp.dataDummy1= 0;
  rsp.dataDummy2= 0;
  hdr->msgId   =  msgId;
  encRhDehHeader( hdr, &encBuf );
  encDehRhDataPathRsp( &rsp, &encBuf );
  sendMsgToDehRsp(&encBuf);
}

void procFaControlReq(
  IN UINT32   msgId,
  IN UINT16   sequenceNo,
  IO LkHdr    *hdr,
  IO DecBuf   *decBuf
  )
{
  EncBuf     encBuf;
  RhDehFaControlRsp   rsp;

  clearBytes(&encBuf, sizeof(EncBuf));
  clearBytes(&rsp , sizeof(RhDehRsp));

  DehMtContext     *dehMtCtx = getGlobalDehMtContext();
  dehMtCtx->statusInfoRsp.dataPath[0].pathFaTxAtten[0] = getUint16FromDecBuf(decBuf, 16);
  dehMtCtx->statusInfoRsp.dataPath[0].pathFaTxAtten[1] = getUint16FromDecBuf(decBuf, 18);
  dehMtCtx->statusInfoRsp.txFaFrequency[0] = getUint32FromDecBuf(decBuf, 24);
  dehMtCtx->statusInfoRsp.rxFaFrequency[0] = getUint32FromDecBuf(decBuf, 28);
  dehMtCtx->statusInfoRsp.txPathFaControl[0] = getUint8FromDecBuf(decBuf, 32);
  dehMtCtx->statusInfoRsp.txPathFaControl[1] = getUint8FromDecBuf(decBuf, 33);
  dehMtCtx->statusInfoRsp.txPathFaControl[2] = getUint8FromDecBuf(decBuf, 34);
  dehMtCtx->statusInfoRsp.txPathFaControl[3] = getUint8FromDecBuf(decBuf, 35);
  dehMtCtx->statusInfoRsp.rxPathFaControl[0] = getUint8FromDecBuf(decBuf, 36);
  dehMtCtx->statusInfoRsp.rxPathFaControl[1] = getUint8FromDecBuf(decBuf, 37);
  dehMtCtx->statusInfoRsp.rxPathFaControl[2] = getUint8FromDecBuf(decBuf, 38);
  dehMtCtx->statusInfoRsp.rxPathFaControl[3] = getUint8FromDecBuf(decBuf, 39);
  rsp.varResult = dehMtCtx->rhFaControlRsp;
  rsp.sequenceNo = sequenceNo;
  rsp.dataDummy1 = 0;
  rsp.dataDummy2 = 0;
  rsp.dataDummy3 = 0;

  hdr->msgId   =  msgId;
  encRhDehHeader( hdr, &encBuf );
  encDehRhFaControlRsp( &rsp, &encBuf );
  sendMsgToDehRsp(&encBuf);
}

void procDehAlarmThresholdReq(
  IN UINT32   msgId,
  IN UINT16   sequenceNo,
  IO LkHdr    *hdr,
  IO DecBuf   *decBuf
  )
{
  EncBuf     encBuf;
  LkBsAlarmThresholdSettingRsp   rsp;

  clearBytes(&encBuf, sizeof(EncBuf));
  clearBytes(&rsp , sizeof(RhDehRsp));

  DehMtContext     *dehMtCtx = getGlobalDehMtContext();
  dehMtCtx->statusInfoRsp.vswrShutdownAlarmThreshold = getUint16FromDecBuf( decBuf, 14);
  dehMtCtx->statusInfoRsp.overPowerShutdownAlarmThreshold = getUint32FromDecBuf( decBuf, 16);
  dehMtCtx->statusInfoRsp.lowPowerAlarmThreshold = getUint32FromDecBuf( decBuf, 20);
  rsp.varResult = dehMtCtx->rhAlarmThresholdRsp;
  rsp.sequenceNo = sequenceNo;
  hdr->msgId   =  msgId;
  encRhDehHeader( hdr, &encBuf );
  encDehRhAlarmThresholdRsp( &rsp, &encBuf );
  sendMsgToDehRsp(&encBuf);
}

void procDehDelaySettingReq(
  IN UINT32   msgId,
  IN UINT16   sequenceNo,
  IO LkHdr    *hdr,
  IO DecBuf   *decBuf
  )
{
  EncBuf     encBuf;
  LkBsDelaySettingRsp   rsp;

  clearBytes(&encBuf, sizeof(EncBuf));
  clearBytes(&rsp , sizeof(RhDehRsp));

  DehMtContext     *dehMtCtx = getGlobalDehMtContext();
  dehMtCtx->statusInfoRsp.txDelay = getUint32FromDecBuf( decBuf, 16);
  dehMtCtx->statusInfoRsp.rxDelay = getUint32FromDecBuf( decBuf, 20);
  rsp.varResult = dehMtCtx->rhDelaySettingRsp;
  rsp.sequenceNo = sequenceNo;
  hdr->msgId   =  msgId;
  encRhDehHeader( hdr, &encBuf );
  encDehRhDelaySettingRsp( &rsp, &encBuf );
  sendMsgToDehRsp(&encBuf);
}

void procDehOperatingModeInfoReq(
  IN UINT32   msgId,
  IO UINT16   sequenceNo,
  IO LkHdr    *hdr
  )
{
  EncBuf     encBuf;
  LkBsOperationalModeInfoRsp   rsp;
  UNUSED(sequenceNo);
  clearBytes(&encBuf, sizeof(EncBuf));
  clearBytes(&rsp , sizeof(RhDehRsp));

  rsp.varResult = 0;
  rsp.varMode = 32768;
  hdr->msgId   =  msgId;
  encRhDehHeader( hdr, &encBuf );
  encDehRhOperatingModeInfoRsp( &rsp, &encBuf );
  sendMsgToDehRsp(&encBuf);
}


void procDehStatusReq(
  IN UINT32   msgId,
  IO UINT16   sequenceNo,
  IO LkHdr    *hdr
  )
{
  EncBuf     encBuf;
  clearBytes(&encBuf, sizeof(EncBuf));

  LkBsStatusInfoRsp   *statusRsp = getStatusInfoRsp();

  hdr->msgId   =  msgId;
  statusRsp->sequenceNo = sequenceNo;
  fprintf(rhSimulatorLog ,"\n varSeqNum = %d", statusRsp->sequenceNo);
  encRhDehHeader( hdr, &encBuf );
  encDehStatusRsp( statusRsp, &encBuf );
  sendMsgToDehRsp(&encBuf);
}

/**
 * This can also used to send periodic Alarm reports to DEH --TODO
 * */
void procRhSimTimeout()
{
  return;
}
/**
* @details DESCRIPTION
* This is the entry point for all messages coming from DEH
*/

void dehMtMsgHdlr(
  IO MsgBuf *rhMsgIn
  )
{
  DecBuf     decBuf;
  MailBox    dstMailbox;
  MailBox    srcMailbox;
  MsgId      msgId;
  UINT16     sequenceNo =0 ;
  UINT16     dummy = 0;

  //initDecBuf(decBuf, rhMsgIn->len, &(rhMsgIn->val));
  decBuf.index = 0;
  decBuf.len   = rhMsgIn->len;
  decBuf.value = &(rhMsgIn->val[0]);

  dstMailbox = getUint16( &decBuf );
  srcMailbox = getUint16( &decBuf );
  msgId      = getUint32( &decBuf );
  dummy      = getUint16( &decBuf );
  sequenceNo = getUint16( &decBuf );


  LkHdr        hdr;
  hdr.dstMailbox = srcMailbox;
  hdr.srcMailbox = dstMailbox;

  switch(msgId)
  {
    /* Reset Request */
    case MSG_BS_LK_RESET_REQ:
    {           
      fprintf(rhSimulatorLog,"MSG_BS_LK_RESET_REQ Received \n");
      procDehResetReq(MSG_LK_BS_RESET_RSP, &hdr);
      fprintf(rhSimulatorLog,"MSG_LK_BS_RESET_RSP Sent to DEH \n");
      g_NotiAckReceived = FALSE;
      break;
    }
    case MSG_BS_LK_INITIALIZATION_ACK_ID:
    {
      g_NotiAckReceived = TRUE;
      fprintf(rhSimulatorLog,"ACK For NOTI is Received \n");
      fprintf(rhSimulatorLog, "ACK For NOTI is Received is =%d \n", msgId);
      break;
    }
    case MSG_BS_LK_ENABLE_OR_DISABLE_REQ:
    {
      UINT8   path,enableOrDisable ;
      path = getUint8( &decBuf );
      enableOrDisable = getUint8( &decBuf );
      fprintf(rhSimulatorLog,"MSG_BS_LK_ENABLE_OR_DISABLE_REQ Received \n");
      procDehEnableOrDisableReq(MSG_LK_BS_ENABLE_OR_DISABLE_RSP, sequenceNo, &hdr , path , enableOrDisable );
      fprintf(rhSimulatorLog,"MSG_BS_LK_ENABLE_OR_DISABLE_RSP Sent to DEH \n");
      break;
    }
    case MSG_BS_LK_TX_ON_OFF_SET_REQ:
    {
      UINT8   path,txOnOrOff ;
      path = getUint8( &decBuf );
      txOnOrOff = getUint8( &decBuf );
      fprintf(rhSimulatorLog,"MSG_BS_LK_TX_ON_OFF_SET_REQ Received \n");
      procDehOnOffReq(MSG_LK_BS_TX_ON_OFF_SET_RSP, sequenceNo, &hdr, path , txOnOrOff);
      fprintf(rhSimulatorLog,"MSG_LK_BS_TX_ON_OFF_SET_RSP Sent to DEH \n");
      break;
    }
    case MSG_BS_LK_DATA_PATH_REQ:
    {
      fprintf(rhSimulatorLog,"MSG_BS_LK_DATA_PATH_REQ Received \n");
      procDehDataPathReq(MSG_LK_BS_DATA_PATH_RSP, sequenceNo, &hdr);
      fprintf(rhSimulatorLog,"MSG_BS_LK_DATA_PATH_RSP Sent to DEH \n");
      break;
    }
    case MSG_BS_LK_FA_CONTROL_REQ:
    {
      fprintf(rhSimulatorLog,"MSG_BS_LK_FA_CONTROL_REQ Received \n");
      procFaControlReq(MSG_LK_BS_FA_CONTROL_RSP, sequenceNo, &hdr, &decBuf);
      fprintf(rhSimulatorLog,"MSG_LK_BS_FA_CONTROL_RSP Sent to DEH \n");
      break;
    }
    case MSG_BS_LK_ALARM_THRESHOLD_SETTING_REQ:
    {
      fprintf(rhSimulatorLog,"MSG_BS_LK_ALARM_THRESHOLD_SETTING_REQ Received \n");
      procDehAlarmThresholdReq(MSG_LK_BS_ALARM_THRESHOLD_SETTING_RSP, sequenceNo, &hdr, &decBuf);
      fprintf(rhSimulatorLog,"MSG_LK_BS_ALARM_THRESHOLD_SETTING_RSP Sent to DEH \n");
      break;
    }
    case MSG_BS_LK_DELAY_SETTING_REQ:
    {
      fprintf(rhSimulatorLog,"MSG_BS_LK_DELAY_SETTING_REQ Received \n");
      procDehDelaySettingReq(MSG_LK_BS_DELAY_SETTING_RSP, sequenceNo, &hdr, &decBuf);
      fprintf(rhSimulatorLog,"MSG_LK_BS_DELAY_SETTING_RSP Sent to DEH \n");
      break;
    }
    case MSG_BS_LK_HW_INFO_REQ:
    {
      fprintf(rhSimulatorLog,"MSG_BS_LK_HW_INFO_REQ Received \n");
      procDehHwInfoReq(MSG_LK_BS_HW_INFO_RSP, &hdr);
      fprintf(rhSimulatorLog,"MSG_LK_BS_HW_INFO_RSP Sent to DEH \n");
      break;
    }
    case MSG_BS_LK_FW_INFO_REQ:
    {
      fprintf(rhSimulatorLog,"MSG_BS_LK_FW_INFO_REQ Received \n");
      procDehFwInfoReq(MSG_LK_BS_FW_INFO_RSP, &hdr);
      fprintf(rhSimulatorLog,"MSG_LK_BS_FW_INFO_RSP Sent to DEH \n");
      break;
    }
    case MSG_BS_LK_STATUS_INFO_REQ:
    {
      fprintf(rhSimulatorLog,"MSG_BS_LK_STATUS_INFO_REQ Received \n");
      procDehStatusReq(MSG_LK_BS_STATUS_INFO_RSP, sequenceNo, &hdr);
      fprintf(rhSimulatorLog,"MSG_LK_BS_STATUS_INFO_RSP Sent to DEH \n");
      break;
    }
    case MSG_BS_LK_ALARM_INFO_REQ:
    {
      fprintf(rhSimulatorLog,"MSG_BS_LK_ALARM_INFO_REQ Received \n");
      procDehAlarmInfoReq(MSG_LK_BS_STATUS_INFO_RSP, sequenceNo, &hdr);
      fprintf(rhSimulatorLog,"MSG_LK_BS_ALARM_INFO_RSP Sent to DEH \n");
      break;
    }
    case MSG_BS_LK_OPERATION_MODE_INFO_REQ:
    {
      fprintf(rhSimulatorLog,"MSG_BS_LK_OPERATION_MODE_INFO_REQ \n");
      procDehOperatingModeInfoReq(MSG_LK_BS_OPERATION_MODE_INFO_RSP, sequenceNo, &hdr);
      fprintf(rhSimulatorLog,"MSG_LK_BS_OPERATION_MODE_INFO_RSP Sent to DEH \n");
      break;
    }
    case MSG_TIMEKEEPER_DEH_TIMEOUT_IND:
    {
      fprintf(rhSimulatorLog," Received MSG_TIMEKEEPER_DEH_TIMEOUT_IND\n");
      procRhSimTimeout();
      break;
    }

    default:
    {
      fprintf(rhSimulatorLog, "Valid Msg Not found,Msg Id Received is =%d\n", msgId);
      break;
    }
  }


}



/**
 * @param modId The module is identified by this identifier in framework
 * @param ec    The error code returned by OS
 * @details    This function checks for reception of message at module identified by modId.
 *             if no message is waiting in the queue then NULL is returned.
 *             This is strictly meant for LK
 */
inline TBOOL recvDehMsg(
  IN SINT32    *sockFd,
  IO MsgBuf     *buf
  )
{
  buf->len = recv(*sockFd, &buf->val[0], SIZE_RECV_DATA, SOCK_FLAGS);
  if (buf->len EQ -1)
  {
    return FALSE;
  }
  return TRUE;
}

/**
 * @param modId The module where the message is received, this is used to identify the dest queue.
 * @param msgHandler The function to invoke after a message is received at the queue.
 * @details This function is a runs forever, schedDehMsgHandler  Schedules Messages
 * coming from IPC framework and also from Radio head.
 */
void schedDehMtMsgHandler()
{
  clock_t  timeAtMsgIn;
  clock_t  msgProcDuration;
  clock_t  timeAtStart;
  time_t   elapsed; /* clock time */
  MsgBuf buf;
  buf.len = 0;

  while(1)
  {
    timeAtStart = clock();

#if 1
    if(g_NotiAckReceived EQ FALSE)
    {
      usleep( 3 * MAX_SCHED_DELAY  );
      LkHdr        hdr;
      clearBytes(&hdr , sizeof(LkHdr));

      hdr.dstMailbox = BS_MAILBOX_ID;
      hdr.srcMailbox = LK_MAILBOX_ID;
      procDehReqNoti(MSG_LK_BS_INITIALIZATION_NOTI_ID, &hdr);
      fprintf(rhSimulatorLog,"NOTI Sent to DEH \n");
    }
#endif
    /* Handle Messages coming from DEH */
    if (recvDehMsg(&g_sockFd, &buf) EQ TRUE)
    {
      timeAtMsgIn = clock();
      dehMtMsgHdlr(&buf);
      msgProcDuration = clock() - timeAtMsgIn;
      if(msgProcDuration > MAX_MSG_PROCESSING_DURATION)
      {
        BS_ERROR_MSG_PROC_DURAT_EXCEEDED(  MSG_UNDEFINED , msgProcDuration );
      }
    }

    /* Handler one message at a time. Release CPU */
    usleep(Q_POLL_INTERVAL);

    elapsed = clock() - timeAtStart;
    if(elapsed > MAX_SCHED_DELAY)
    {
      BS_ERROR_MAX_SCHED_DELAY_EXCEEDED( elapsed );
    }
  }
}



/**
 * @file    rh_sim.h
 * @brief API for Device Handler (Abbrev: DEH)
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author : Madhu K
 * Date   :
 * Description :
 */

#ifndef RHSIM_H
#define RHSIM_H
/* Common .h files*/
//#include "string.h"
#include "all_cmn.h"
#include "cmn_types.h"
#include "imc_timer.h"
#include "imc_proc.h"
#include "log.h"
#include "bs_tables.h"
#include "bsm_deh_if.h"
#include "bsm_deh_if.h"
#include "deh_rh_func.h"

#include "all_cfg.h"

#include "generic_table.h"
#include "bs_config.h"

#include "deh.h"

//#define DEH_IP_ADDR     "10.131.17.23"
#define RH_RSP_SUCC     TRUE


typedef struct s_DehMtContext
{
  FilePath           defaultConfigFilePath;
  FilePath           rhSimConfigFilePath;
  UINT8              version[MAX_SIZE_SWPACKAGESTATUS_VERSION + 1];
  UINT8              eNodeBIpAddr[MAX_SIZE_ENBSTATUS_ENODEBIPADDRESS + 1];
  UINT8              rhIpAddr[MAX_SIZE_ENBADDRESSES_RADIOHEADIPADDRESS + 1];
  SINT16             rhResetRsp;
  SINT16             rhInitNoti;
  SINT16             rhFaControlRsp;
  SINT16             rhDataPathSetRsp;
  SINT16             rhDelaySettingRsp;
  SINT16             rhTxOnPathA;
  SINT16             rhTxOnPathB;
  SINT16             rhTxOffPathA;
  SINT16             rhTxOffPathB;
  SINT16             rhTxEnablePathA;
  SINT16             rhTxEnablePathB;
  SINT16             rhTxDisablePathA;
  SINT16             rhTxDisablePathB;
  SINT16             rhAlarmThresholdRsp;
  LkBsHwInfoRsp      hwInfoRsp;
  LkBsFwInfoRsp      fwInfoRsp;
  LkBsStatusInfoRsp  statusInfoRsp;
  LkBsAlarmInfoRsp   alarmInfoRsp;
  //ModTimersInfo      dehMtTimerInfo;
  TimerCtx           timerCtx;
  TimersConfig       dehMtTimerInfo;
} DehMtContext;

extern DehMtContext     g_dehMtCtx;


/* To get the Global context */
#define getGlobalDehMtContext()           &(g_dehMtCtx)

/* To get eNodeB IP address */
#define getNodeBIpAddress()           &(g_dehMtCtx.eNodeBIpAddr[0])

/* To get Radio Head IP address */
#define getDehMtRhIpAddress()           &(g_dehMtCtx.rhIpAddr[0])

/* Hard ware Information Response */
#define gethwInfoRsp()    &(g_dehMtCtx.hwInfoRsp)
/* Firm ware Information Response */
#define getfwInfoRsp()    &(g_dehMtCtx.fwInfoRsp)
/* Status Information Response */
#define getStatusInfoRsp()    &(g_dehMtCtx.statusInfoRsp)
/* Alarm Information Response */
#define getAlarmInfoRsp()    &(g_dehMtCtx.alarmInfoRsp)

/* Timer Information Response */
#define getTimerInfoRsp()    &(g_dehMtCtx.dehMtTimerInfo)

/* Timer Context */
#define getDehMtTimerCtx()    &(g_dehMtCtx.timerCtx)

typedef LkBsFaControlRsp RhDehFaControlRsp;

void schedDehMtMsgHandler();

void initDehMtCtx();
TBOOL readSimulatorConfigFile();
TBOOL readRhSimConfigFile();
void startTimerForSim(
  IN TimeSpec tick
  );
#endif /* RHSIM_H */

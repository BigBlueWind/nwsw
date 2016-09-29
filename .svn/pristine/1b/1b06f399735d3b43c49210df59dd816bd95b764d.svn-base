/**
 * @file    deh_rh_fill_func.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains BS - LK interface fill functions.
 *
 * @author      mkata, kgummad, m_sharma,spolepally
 * @date        24-08-2010
 * @details     Basic version
 *
 */

#ifndef DEH_RH_FILL_FUNC_H
#define DEH_RH_FILL_FUNC_H

#include "rh_enc.h"
#include "bsm_deh_if.h"
#include "prm_deh_if.h"
#include "deh_rh_func.h"

void fillHdr( IN UINT32  msgId, IO LkHdr * hdr );
void fillDehRhHwInfoReq( IO BsLkHwInfoReq *req );
void fillDehRhHwInfoReq( IO BsLkHwInfoReq   *req );
void fillDehRhFwInfoReq( IO BsLkFwInfoReq   *req );
void fillDehRhHistoryInfoReadReq( IO BsLkHistoryInfoReadReq   *req );
void fillDehRhHistoryInfoWriteReq (
  IN LkHistoryWriteTable       *historyWriteReq,
  IO BsLkHistoryInfoWriteReq   *req
  );
void fillDehRhOperationalModeInfoReq( IO BsLkOperationalModeInfoReq   *req );
void fillDehRhRemoteFusingReq( IO BsLkRemoteFusingReq   *req );
void fillDehRhRemoteFusingCompleteAck( IO BsLkRemoteFusingCompleteAck   *req );
void fillDehRhResetReq( IO BsLkResetReq   *req );
void fillDehRhInitializationAck( IO BsLkInitializationAck *ack  );
void fillDehRhAlarmInfoReq(
  IN UINT32            seqNum,
  IO BsLkAlarmInfoReq  *req
  );
void fillDehRhAlarmChangeAck(
  IN UINT16              seqNoReceivedInNoti,
  IO BsLkAlarmChangeAck  *req
  );
void fillDehRhStatusInfoReq(
  IN UINT32             *seqNum,
  IO BsLkStatusInfoReq  *req
 );


void fillDehRhEnableReqPathA( IO BsLkEnableOrDisableReq  *req );
void fillDehRhEnableReqPathB( IO BsLkEnableOrDisableReq  *req );
void fillDehRhDisableReqPathA( IO BsLkEnableOrDisableReq  *req );
void fillDehRhDisableReqPathB( IO BsLkEnableOrDisableReq  *req );

void fillDehRhEnableOrDisableReqPath(
  IO BsLkEnableOrDisableReq  *req,
  UINT8 path,
  UINT8 control
  );
void fillDehRhDlUlRatioReq( IO  BsLkDlUlRatioReq  *req );

/* RH Configuration */
void fillDehRhDataPathReq(
  IO  BsLkDataPathReq   *req,
  IO  RhDefaultCfg *rhConfig
  );
void fillDehRhFaControlReq(
  IO BsLkFaControlReq       *req,
  IO RhDefaultCfg   *rhConfig
  );
void fillDehRhDelaySettingReq(
  IO BsLkDelaySettingReq  *req,
  IO RhDefaultCfg *rhConfig
  );
void fillDehRhTxOnOffReq(
  IO BsLkTxOnOffReq  *req,
  IN RhPath          *path,
  IN OnOff           *txOnOff
  );
void fillDehRhFa1ControlReq(
  IO BsLkFaControlReq       *req
  );
void fillDehRhFa2ControlReq(
  IN MsgBsmDehRhConfigReq   *lkConfigReq,
  IO BsLkFaControlReq       *req
  );
void fillDehRhAlarmThresholdSettingReq(
  IO BsLkAlarmThresholdSettingReq   *req,
  IO RhDefaultCfg *rhConfig
  );
void fillDehRhCarrierLOSettingReq(
  IN RhFANum                   *fANum,
  IN LoSettings                *loEvenOrOdd,
  IO BsLkCarrierLOSettingReq   *req
  );

void fillDehRhTimeSetReq(
  IN TimeTick         *time,
  IO BsLkTimeSetReq   *req
  );
void fillDehRhFaControlRxReq(
  IN TBOOL             askInputFromCli,
  IO BsLkFaControlRxReq  *req
  );
void fillDehRhOperationalModeInfoReq(
  IO BsLkOperationalModeInfoReq  *req
  );
void fillDehRhSwInstallReq(
  IN MsgPrmDehRhSwInstallReq *data,
  IO BsLkSwInstallreq         *req
  );
void fillDehRhSwActivateReq(
  IN UINT8              side,
  IO BsLkSwActivateReq  *req
  );



#endif

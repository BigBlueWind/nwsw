/**
 * @file    rh_enc.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains LK interface encoding functions.
 *
 * @author    mkata, spolepally, aagrawal, kgummad, m_sharma
 * @date      27-05-2011
 * @details   Change History: Basic version
 *
 */

#ifndef RH_ENC_H
#define RH_ENC_H  1

#include "rh_deh_if.h"

void encDehRhHeader (
  IN LkHdr      *hdr,
  IO EncBuf     *buf
  );
void encDehRhHwInfoReq(
  IN BsLkHwInfoReq   *req,
  IO EncBuf          *encBuf
  );
void encDehRhFwInfoReq(
  IN BsLkFwInfoReq   *req,
  IO EncBuf          *encBuf
  );
void encDehRhHistoryInfoReadReq(
  IN BsLkHistoryInfoReadReq   *req,
  IO EncBuf                   *encBuf
  );
void encDehRhHistoryInfoWriteReq (
  IN BsLkHistoryInfoWriteReq   *req,
  IO EncBuf                    *encBuf
  );
void encDehRhOperationalModeInfoReq(
  IN BsLkOperationalModeInfoReq   *req,
  IO EncBuf                       *encBuf
  );
void encDehRhRemoteFusingReq(
  IN BsLkRemoteFusingReq   *req,
  IO EncBuf                *encBuf
  );
void encDehRhRemoteFusingCompleteAck(
  IN BsLkRemoteFusingCompleteAck   *req,
  IO EncBuf                        *encBuf
  );
void encDehRhResetReq(
  IN BsLkResetReq   *req,
  IO EncBuf         *encBuf
  );
void encDehRhInitializationAck(
  IN BsLkInitializationAck   *req,
  IO EncBuf                  *encBuf
  );
void encDehRhAlarmInfoReq(
  IN BsLkAlarmInfoReq   *req,
  IO EncBuf             *encBuf
  );
void encDehRhAlarmChangeAck(
  IN BsLkAlarmChangeAck   *req,
  IO EncBuf               *encBuf
  );
void encDehRhStatusInfoReq(
  IN BsLkStatusInfoReq   *req,
  IO EncBuf             *encBuf
  );
void encDehRhDelaySettingReq(
  IN BsLkDelaySettingReq   *req,
  IO EncBuf                *encBuf
  );
void encDehRhTxOnOffReq(
  IN BsLkTxOnOffReq   *req,
  IO EncBuf           *encBuf
  );
void encDehRhEnableOrDisableReq(
  IN BsLkEnableOrDisableReq   *req,
  IO EncBuf                   *encBuf
  );
void encDehRhDlUlRatioReq(
  IN BsLkDlUlRatioReq   *req,
  IO EncBuf             *encBuf
  );
void encDehRhDataPathReq(
  IN BsLkDataPathReq   *req,
  IO EncBuf             *encBuf
  );
void encDehRhFaControlReq(
  IN BsLkFaControlReq   *req,
  IO EncBuf             *encBuf
  );
void encDehRhFaControlRxReq(
  IN BsLkFaControlRxReq   *req,
  IO EncBuf             *encBuf
  );
void encDehRhAlarmThresholdSettingReq(
  IN BsLkAlarmThresholdSettingReq   *req,
  IO EncBuf                         *encBuf
  );
void encDehRhCarrierLOSettingReq(
  IN BsLkCarrierLOSettingReq   *req,
  IO EncBuf                         *encBuf
  );
void encDehRhTimeSetReq(
  IN BsLkTimeSetReq   *req,
  IO EncBuf           *encBuf
  );
void sendMsgToLk(
  IN EncBuf *encBuf
  );
void encDehRhSwInstallReq(
  IN BsLkSwInstallreq *req,
  IO EncBuf           *encBuf
  );
void encDehRhSwActivateReq(
  IN BsLkSwActivateReq *req,
  IO EncBuf           *encBuf
  );


#endif

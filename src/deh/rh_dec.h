/**
 * @file    rh_dec.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains LK interface decoding functions.
 *
 * @author  mkata, spolepally, aagrawal, kgummad
 * @date    26-04-2011
 * @details Change History: Basic version
 *
 */

#ifndef RH_DEC_H
#define RH_DEC_H  1

#include "rh_deh_if.h"
#include "deh_rh_func.h"
#include "bsm_deh_if.h"

/* RH To DEH Error Messages */
#define RH_MSGID_ERROR -2
#define RH_VAR_ERROR -3
#define RH_MULTI_MODE_ERROR -4
#define RH_ENABLE_ERROR -5
#define RH_SET_RESTRICT -101

TBOOL decRhDehHwInfoRsp(
  IO DecBuf          *decBuf,
  IO LkHwInfoTable   *rsp
  );

TBOOL decRhDehFwInfoRsp(
  IO DecBuf          *decBuf,
  IO LkBsFwInfoRsp   *rspMsg
  );

TBOOL decRhDehHistoryInfoReadRsp(
  IO DecBuf          *decBuf,
  IO LkHistoryTable  *rsp
);
TBOOL decRhDehHistoryInfoWriteRsp(
  IO DecBuf     *decBuf
);
void decRhDehOperationalModeInfoRsp(
  IO DecBuf                      *decBuf,
  IO LkBsOperationalModeInfoRsp     *rsp
  );
void decRhDehRemoteFusingRsp(
  IO DecBuf              *decBuf,
  IO LkBsRemoteFusingRsp  *rsp
  );
void decRhDehRemoteFusingCompleteNoti(
  IO DecBuf                       *decBuf,
  IO LkBsRemoteFusingCompleteNoti  *noti
  );
void decRhDehResetRsp(
  IO DecBuf        *decBuf,
  IO LkBsResetRsp  *rsp
  );
void decRhDehInitializationReqNoti(
  IO DecBuf                     *decBuf,
  IO LkBsInitializationReqNoti  *noti
  );
TBOOL decRhDehAlarmInfoRsp(
  IO DecBuf            *decBuf,
  IO LkBsAlarmInfoRsp  *rsp
);
TBOOL decRhDehAlarmChangeNoti(
  IO DecBuf               *decBuf,
  IO LkBsAlarmChangeNoti  *noti
  );

TBOOL decRhDehStatusInfoRsp(
  IO DecBuf             *decBuf,
  IO RadioHeadStatus    *rspMsg,
  IO UINT16             *varSeqNum
  );

TBOOL decRhDehConfigStatusInfoRsp(
  IO DecBuf             *decBuf,
  IO RadioHeadConfig    *rspMsg,
  IO UINT16             *varSeqNum
  );

void decRhDehDelaySettingRsp(
  IO DecBuf              *decBuf,
  IO LkBsDelaySettingRsp  *rspMsg
  );
void decRhDehTxOnOffRsp(
  IO DecBuf          *decBuf,
  IO LkBsTxOnOffRsp  *rspMsg
  );
void decRhDehEnableOrDisableRsp(
  IO DecBuf                  *decBuf,
  IO LkBsEnableOrDisableRsp  *rspMsg
  );
void decRhDehDlUlRatioRsp(
  IO DecBuf            *decBuf,
  IO LkBsDlUlRatioRsp  *rspMsg
  );
void decRhDehDataPathRsp(
  IO DecBuf           *decBuf,
  IO LkBsDataPathRsp  *rspMsg
  );
void decRhDehFaControlRsp(
  IO DecBuf            *decBuf,
  IO LkBsFaControlRsp  *rspMsg
  );
void decRhDehFaControlRxRsp(
  IO DecBuf            *decBuf,
  IO LkBsFaControlRxRsp  *rsp
  );
void decRhDehAlarmThresholdSettingRsp(
  IO DecBuf                        *decBuf,
  IO LkBsAlarmThresholdSettingRsp  *rspMsg
  );
void decRhDehCarrierLoSettingRsp(
  IO DecBuf                   *decBuf,
  IO LkBsCarrierLoSettingRsp  *rspMsg
  );
void decRhDehTimeSetRsp(
  IO DecBuf          *decBuf,
  IO LkBsTimeSetRsp  *rspMsg
  );
char *decRhDehRspError(
  IO SINT16 rhRspError,
  IN UINT32 msgId
  );
void decRhDehSwInstallRsp(
  IO DecBuf            *decBuf,
  IO LKBsSwInstallRsp  *rsp
  );
void decRhDehSwActivateRsp(
  IO DecBuf            *decBuf,
  IO LkBsSwActivateRsp  *rsp
  );
#endif

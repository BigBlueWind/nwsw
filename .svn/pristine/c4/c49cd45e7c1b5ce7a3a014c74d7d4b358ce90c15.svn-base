/**
 * @file    deh_rh_func.h
 * This file contains LK-BS customer interface test functions and structures
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author  mkata, kgummad, m_sharma, spolepally, aagrawal
 * @date    25-05-2011
 * @details Basic version
 *
 */

/* Include files */
#ifndef DEH_RH_FUNC_H
#define DEH_RH_FUNC_H
#include "prm_deh_if.h"
extern UINT8*  lkIpAddr;

/**************************************************************************/
/**************  Need to be generated from OAM Excel Sheet ****************/
/**************************************************************************/

#define RH_PATH_A   0
#define RH_PATH_B   1

#define RH_FA0      0
#define RH_FA1      1
#define RH_FA2      2
#define RH_FA3      3
#define RH_FA4      4
#define RH_FA5      5
#define RH_FA6      6
#define RH_FA7      7
#define RH_FA8      8
#define RH_FA9      9
#define RH_FA10     10
#define RH_FA11     11
#define RH_FA12     12
#define RH_FA13     13
#define RH_FA14     14
#define RH_FA15     15


#define RH_MAX_PATHS       2
#define RH_MAX_FA          2
#define RH_MAX_CHANNELS    2

typedef UINT32 RhDelay;
typedef UINT32 RhAttenuation;
typedef UINT32 RhFrequency;
typedef UINT8  RhFANum;
typedef TBOOL  OnOff;
typedef UINT8  LoSettings;
typedef UINT8  RhPath;
typedef UINT32 TimeTick;

typedef enum s_Command
{
  commandgetStatus,
  commandgetHwInfo,
  commandgetSwInfo,
  commandgetAlarms,
  commandgetHistory,
  commanddoReset
}Command;

typedef enum s_Fa1ChannelBandwidth
{
  Fa1ChannelBandwidth5,
  Fa1ChannelBandwidth10,
  Fa1ChannelBandwidth20
}Fa1ChannelBandwidth;

typedef enum s_Fa2ChannelBandwidth
{
  Fa2ChannelBandwidth0,
  Fa2ChannelBandwidth5,
  Fa2ChannelBandwidth10
}Fa2ChannelBandwidth;

typedef struct s_Time
{
  UINT8 hours;
  UINT8 mins;
  UINT8 secs;
}Time;

typedef struct s_Date
{
  UINT16  year;
  UINT8   month;
  UINT8   date;
}Date;

typedef struct s_LkAdminTable
{
  /*Allows operator to request for information on LK*/
  Command command;
}LkAdminTable;

typedef struct s_LkHwInfoTable
{
  /*V1.2.3.40 indicates 0x01020328.*/
  UINT8    version[MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1];
  /* Up to 111 characters can be displayed. CR, LF, TAB cannot be used. Identifier = SEMICOLON, Format:= {CoName};{CoIni};{PrDesc};{PrIni};{SN};{CLEI};{MN}*/
  UINT8    description[MAX_SIZE_COMPONENTSTATUS_INFOSTRING + 1];
}LkHwInfoTable;

typedef struct s_LkFwInfoTable
{
  /*V1.2.3.40 indicates 0x01020328.*/
  UINT8   version[MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1];
  /*Sets UTC Jan. 1, 1970 0:0:0 to 0. Count the value by 1 every second.*/
  UINT8   date[MAX_FWBUILDINFO_DESC + 1];
}LkFwInfoTable;


typedef struct s_LkHistoryTable
{
  /*UTC*/
  UINT32   ManufactureDate;
  /*UTC*/
  UINT32   InstallDate;
  /*Value counted by 1 whenever repair history is entered*/
  UINT32   RepairCount;
  /*Repair Date UTC*/
  UINT32   RepairDate;
  /*Repair description (Repair Code )*/
  UINT32   RepairCode;
}LkHistoryTable;

/*1 = Alarm ON 0 = Alarm OFF(Normal)*/
typedef struct s_LkAlarmTable
{
  TBOOL    clockFail;
  TBOOL    highTemperature;
  TBOOL    lowTemperature;

  TBOOL    overPowerSideA;
  TBOOL    vswrFailsideA;
  TBOOL    lowGainsideA;
  TBOOL    lnaFailSideA;
  TBOOL    disabledSideA ;
  TBOOL    shutdownSideA;

  TBOOL    overPowerSideB;
  TBOOL    vswrFailSideB;
  TBOOL    lowGainSideB;
  TBOOL    lnaFailSideB;
  TBOOL    disabledSideB ;
  TBOOL    shutdownSideB;
}LkAlarmTable;

typedef struct s_LkStatusTable
{
  /*0: no use
5: use IQ mapping in 5MHz Ch BW
10: use IQ mapping in 10MHz Ch BW
20: use IQ mapping in 20MHz Ch BW
In this case, C2 will be used for an extended C1. Then C2 IQ Map will be not available.*/
  C1IQMap C1IQMap;
  /*0: no use
5: use IQ mapping in 5MHz Ch BW
10: use IQ mapping in 10MHz Ch BW*/
  C2IQMap C2IQMap;
  /*Number of valid FA*/
  UINT8  NumberOfFA;
  /*1: C1
2: C2*/
  MappingOf1FAStatus MappingOf1FA;
  /*1: C1
2: C2*/
  MappingOf2FAStatus MappingOf2FA;
  /*Connection Validity Check(CVC) value, written in the CVC field of C1 VSS.
L1RU send 0 in case of CPRI failure.*/
  UINT8  CVC1;
  /*Written in the CVC field of C2 VSS. L1RU send 0 in case of CPRI failure.*/
  UINT8  CVC2;
  /*Down Link Ratio*/
  UINT8  DlRatio;
  /*Up Link Ratio*/
  UINT8  UlRatio;
  /*Real Range: 5~200 [MHz]*/
  UINT8  FilterBandwidth;
  /*Real Range: 0.0 ~ 8.0[dB]*/
  UINT32   VSWRShutdownAlarmThreshold;
  /*Digital IQ Level for Tx*/
  UINT32   OverpowerMinorAlarmThreshold;
  /*Digital IQ Level for Tx*/
  UINT32   OverpowerShutdownAlarmThreshold;
  /*Digital IQ Level for Tx*/
  UINT32   LowpowerAlarmThreshold;
  /*Real Range: 0~3000.00 [MHz]*/
  UINT32   CenterFrequency;
  /*FA On / OFF*/
  TBOOL    Fa1OnOff;
  /*FA On / OFF*/
  TBOOL    Fa2OnOff;
  /*0: Normal Output
1: Generated Signal Output*/
  TBOOL    SignalGenerationFa1;
  /*0: Normal Output
1: Generated Signal Output*/
  TBOOL    SignalGenerationFa2;
  /*Real Value: 5, 10, 20 [MHz]*/
  Fa1ChannelBandwidth Fa1ChannelBandwidth;
  /*Real Value: 0, 5, 10 [MHz]*/
  Fa2ChannelBandwidth Fa2ChannelBandwidth;
  /*Real Range: 0~27.0 [dB]*/
  UINT32   Fa1TxAttenPathA;
  /*Real Range: 0~27.0 [dB]*/
  UINT32   Fa1TxAttenPathB;
  /*Real Range: 0~3000.00 [MHz]*/
  UINT32   Fa1TxFrequency;
  /*Real Range: 0~27.0 [dB]*/
  UINT32   Fa1RxAttenPathA;
  /*Real Range: 0~27.0 [dB]*/
  UINT32   Fa1RxAttenPathB;
  /*Real Range: 0~3000.00 [MHz]*/
  UINT32   Fa1RxFrequency;
  /*Real Range: 0~27.0 [dB]*/
  UINT32   Fa2TxAttenPathA;
  /*Real Range: 0~27.0 [dB]*/
  UINT32   Fa2TxAttenPathB;
  /*Real Range: 0~3000.00 [MHz]*/
  UINT32   Fa2TxFrequency;
  /*Real Range: 0~27.0 [dB]*/
  UINT32   Fa2RxAttenPathA;
  /*Real Range: 0~27.0 [dB]*/
  UINT32   Fa2RxAttenPathB;
  /*Real Range: 0~3000.00 [MHz]*/
  UINT32   Fa2RxFrequency;
  /*Unit: ns.Real Range: 0.000~100.000 [us]. Out-of-Range Control: set to the nearest value in the range. This is an normal RSP case.*/
  UINT32   TxDelay;
  /*Unit: ns.Real Range: 0.000~100.000 [us]. Out-of-Range Control: set to the nearest value in the range. This is an normal RSP case.*/
  UINT32   RxDelay;
  /*0: Off
1: On*/
  /*TX Path On/Off is based on this element*/
  TBOOL    TxPathAOn;
  /*0: Off
1: On*/
  /*TX Path On/Off is based on this element*/
  TBOOL    TxPathBOn;
  /*Real Range: -40.0~100.0 [°C]*/
  SINT16  PathATemperature;
  /*Real Range: -40.0~100.0 [°C]*/
  SINT16  PathBTemperature;
  /*Real Range: 0~50.0 [dBm]*/
  UINT16  PathATxRFPower;
  /*Real Range: 0~50.0 [dBm]*/
  UINT16  PathBTxRFPower;
  /*Real Range(Return Loss): 0.00~30.00 [dB] Converted from VSWR range: 1.0~5.0*/
  UINT16  PathAVSWR;
  /*Real Range(Return Loss): 0.00~30.00 [dB] Converted from VSWR range: 1.0~5.0*/
  UINT16  PathBVSWR;
  /*1st FA RU RSSI Real Range: -327.60 ~ 0 dBm*/
  SINT32  PathAFa1RURSSI ;
  /*1st FA RU RSSI Real Range: -327.60 ~ 0 dBm*/
  SINT32  PathBFa1RURSSI ;
  /*2nd FA RU RSSI Real Range: -327.60 ~ 0 dBm*/
  SINT32  PathAFa2RURSSI;
  /*2nd FA RU RSSI Real Range: -327.60 ~ 0 dBm*/
  SINT32  PathBFa2RURSSI ;
  /*For Tx*/
  UINT32   PathAFa1DigitalIQLevel;
  /*For Tx*/
  UINT32   PathBFa1DigitalIQLevel;
  /*For Tx*/
  UINT32   PathAFa2DigitalIQLevel;
  /*For Tx*/
  UINT32   PathBFa2DigitalIQLevel;
}LkStatusTable;

typedef struct s_LkHistoryWriteTable
{
  /*0- Install Date 1- Repair Date*/
  TBOOL    Choice;
  /*Date UTC*/
  UINT32   Date;
  /*description (Ex :Repair Code )*/
  UINT32   Code;
}LkHistoryWriteTable;

/* Configuration Table */
typedef struct s_RhConfig
{
  /* Radio Head Profile Id */
  UINT32   radioHeadProfileId;
  /*Unit: ns.Real Range: 0.000~100.000 [us]. Out-of-Range Control: set to the nearest value in the range. This is an normal RSP case.*/
  UINT32   txDelay;
  /*Unit: ns.Real Range: 0.000~100.000 [us]. Out-of-Range Control: set to the nearest value in the range. This is an normal RSP case.*/
  UINT32   rxDelay;
  /* 0: no use                        5: use IQ mapping in 5MHz Ch BW
    10: use IQ mapping in 10MHz Ch BW    20: use IQ mapping in 20MHz Ch BW */
  C1IQMap  c1IQMap;
  /*Real Range: 0~30.0 [dB]*/
  UINT32   txAttenPathA;
  /*Real Range: 0~30.0 [dB]*/
  UINT32   txAttenPathB;
  /*Real Range: 0~65535*/
  UINT32   txEarfcn;
  /*Real Range: 0~65535*/
  UINT32   rxEarfcn;
  /*Real Range: 3.0 ~ 10.0[dB]*/
  UINT32   vswrShutdownAlarmThreshold;
  /*Digital IQ Level for Tx*/
  UINT32   overpowerAlarmThreshold;
  /*Digital IQ Level for Tx*/
  UINT32   lowpowerAlarmThreshold;
}RhDefaultCfg;

/****************************************************************************/


void expectLkMsg(
  IN UINT32      expMsgId,
  IO DecBuf      *decBuf
  );

TBOOL sendMsgToRh(
  IN EncBuf *encBuf
  );

TBOOL sendNotiAckMsgToRh(
  IN EncBuf *encBuf
  );

/* RH Reset */
TBOOL dehSendRhResetReq();

TBOOL dehRhSetDataPath(
  IO RhDefaultCfg *rhConfig
  );

void procRhDehResetRsp(
  IO DecBuf     *decBuf
  );

void procRhDehAlarmChangeNoti(
  IO DecBuf     *decBuf
  );

TBOOL dehRhSetDelay(
  IO RhDefaultCfg *rhConfig
  );

TBOOL dehRhSetTxOnOff(
  IO RhPath      path,
  IO OnOff       txOnOff
  );

TBOOL dehRhFaControl(
  IO RhDefaultCfg *rhConfig
  );

TBOOL dehRhAlarmThresholdSet(
  IO RhDefaultCfg *rhConfig
  );

TBOOL dehRhSetCarrierLo(
  IN RhFANum    fANum,
  IN LoSettings loEvenOrOdd
  );

TBOOL dehRhOperatingModeInfo();

TBOOL dehRhStatusInfo( );

TBOOL dehRhHardwareInfo( );

TBOOL dehRhFirmwareInfo( );

TBOOL dehRhHistoryReadInfo(
  IO LkHistoryTable   *rspMsg
  );

TBOOL dehRhHistoryWriteInfo(
  IO LkHistoryWriteTable   *historyWriteReq
  );

TBOOL dehRhAlarmInfo(
  IO LkAlarmTable   *rspMsg
  );

void lkDlUlRatio();

TBOOL dehRhEnablePathA();

TBOOL dehRhEnablePathB();

TBOOL dehRhDisablePathA();

TBOOL dehRhDisablePathB();

TBOOL dehRhEnableOrDisablePath( UINT8 path, UINT8 control );

TBOOL dehRhSetTime(
  IN TimeTick  *time
  );
void dehSendRhSwInstallReq(
  IO MsgPrmDehRhSwInstallReq *data
  );
void dehSendRhSwActivateReq(
  IN UINT8  side
  );
void dehSendRhFusingNotiAck();
void dehSendRhNotiAck();
void procRhDehInitNoti(
  IO DecBuf     *decBuf,
  IO UINT32 msgId
  );
void procRhDehDataPathRsp(
  IO DecBuf     *decBuf,
  IN UINT32 msgId
  );
void procRhDehFaControlRsp(
  IO DecBuf     *decBuf,
  IN UINT32 msgId
  );
void procRhDehAlarmThresholdSetRsp(
  IO DecBuf     *decBuf,
  IN UINT32 msgId
  );
void procRhDehDelaySettingRsp(
  IO DecBuf     *decBuf,
  IN UINT32 msgId
  );
void procRhDehTxOnOffRsp(
  IO DecBuf     *decBuf,
  IN UINT32 msgId
  );
void procRhEnableDisableRsp(
  IO DecBuf     *decBuf,
  IN UINT32 msgId
  );
void procRhDehHwInfoRsp(
  IO DecBuf     *decBuf
  );
void procRhDehStatusRsp(
  IO DecBuf     *decBuf
  );
void procRhDehFwInfoRsp(
  IO DecBuf     *decBuf
  );
void procRhDehOperatingModeInfoRsp(
  IO DecBuf     *decBuf
  );
void procRhDehSwInstallRsp(
  IO DecBuf     *decBuf
  );
void procRhDehFusingCompleteNoti(
  IO DecBuf     *decBuf
  );
void procRhDehSwActivateRsp(
  IO DecBuf     *decBuf,
  IN UINT32     msgId
  );


#endif /* DEH_RH_FUNC_H */

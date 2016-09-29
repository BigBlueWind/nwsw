 /**
 * @file    rh_deh_if.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains REQ and RSP structures for RadioHead(LK)-Device Handler(DEH) interface.
 *
 * @author     : Madhusudana Rao Kata
 * Date       : 10-05-2010
 * Description: 1. Basic version
 *              2. Removed unused code and we are not able to follow variable naming coding convention
 *                 since we have to compare structures given in LK Doc for further modification.(Readability point of view)
 *              3. Renamed Lk to Rh. on 31st July 2010
 *
 */

/* 1. LK BS Encodes predefined Messages using Fixed Message IDs, Source and Destination MailBoxes
 * 2. Use getBits and SetBits functions for Encoding and Decoding Bits
 * 3. Use getByte and putByte functions for Encoding and Decoding Bytes
 */

#ifndef RH_DEH_IF_H
#define RH_DEH_IF_H   1

#include "all_cmn.h"
#include "imc_udp.h"

#define LK_NUM_HISTORY_RECORD   7
#define MAX_HWINFO_DESC         243 /* Bytes 12~255 in HwInfoRsp Cmd is used for HW Description */
#define MAX_FWBUILDINFO_DESC    17
#define MailBox                 UINT32

#define  LEN_OF_LK_HDR                               2 + 2 + 4 /* DestMailBox + SrcMailBix + MSgid */
#define  BS_MAILBOX_ID                               9002 //22114     /* 0x5662 */
#define  LK_MAILBOX_ID                               9001 //8192      /* 0x2000*/
#define  GUI_PORT                                    25000
#define  MAX_CPRI_PORTS                              4
#define  MAX_FAS                                     16
#define  MAX_PATHS                                   4
#define  VENDOR_NAME                                 16

/************************************************************************
 *   STRUCTURES FOR ALL MESSAGES
 ***********************************************************************/
/* IPC Header */
typedef struct s_LkHdr
{
  UINT16    dstMailbox;
  UINT16    srcMailbox;
  UINT32    msgId;
} LkHdr;

/*****************************
 *  Hardware Information     *
 *****************************/
#define  MSG_BS_LK_HW_INFO_REQ                   0x00210001

typedef struct s_BsLkHwInfoReq
{
  UINT32        varDummy;  /* Dummy */
} BsLkHwInfoReq;

#define  MSG_LK_BS_HW_INFO_RSP                    0x01210001
#define  MAX_IP_ADDRESS                           20
typedef struct s_LkBsHwInfoRsp
{
  SINT16          varResult;
  UINT16          varDummy;
  UINT32          hwVer;
  UINT8           description[MAX_HWINFO_DESC + 1]; /* MANU: why 111? */
} LkBsHwInfoRsp;

/*************************
  Firmware Information   *
 *************************/
#define  MSG_BS_LK_FW_INFO_REQ                    0x00210002
typedef  BsLkHwInfoReq BsLkFwInfoReq;

#define  MSG_LK_BS_FW_INFO_RSP                    0x01210002
typedef struct s_LkBsFwInfoRsp
{
  SINT16       varResult;
  UINT16       activeSide;
  UINT32       runningVersion;
  UINT32       nonRunningVersion;
  UINT32       runningVersionBuildDate;
  UINT8        vendorName[VENDOR_NAME];
  UINT32       nonRunningVersionBuildDate;
} LkBsFwInfoRsp;

/****************************
 * History Read Information *
 ****************************/
typedef struct s_LkBsHistoryRecord
{
  UINT32        repairDate;
  UINT32        repairCode;
} LkBsHistoryRecord;

#define  LK_BS_HISTORY_REC_MAX                              300
typedef struct s_LkBsHistoryInfo
{
  UINT32                manufactureDate;
  UINT32                installDate;
  UINT32                repairCount;
  LkBsHistoryRecord     historyRecord[LK_BS_HISTORY_REC_MAX];
} LkBsHistoryInfo;

#define  MSG_BS_LK_HISTORY_INFO_READ_REQ          0x00210003
typedef BsLkHwInfoReq BsLkHistoryInfoReadReq;

#define  MSG_LK_BS_HISTORY_INFO_READ_RSP          0x01210003
typedef struct s_LkBsHistoryInfoReadRsp
{
  SINT16             varResult;
  UINT16             varDummy;
  LkBsHistoryInfo    historyInfo;
} LkBsHistoryInfoReadRsp;

/* History Write Information */
#define  MSG_BS_LK_HISTORY_INFO_WRITE_REQ         0x00210013
typedef struct s_BsLkHistoryInfoWriteReq
{
  UINT16          varDummy;
  UINT16          varType;
  UINT32          date;
  UINT32          repairCode;
} BsLkHistoryInfoWriteReq;

#define  MSG_LK_BS_HISTORY_INFO_WRITE_RSP         0x01210013
typedef struct s_LkBsHistoryInfoWriteRsp
{
  SINT16          varResult;
  UINT16          varType;
} LkBsHistoryInfoWriteRsp;

/********************************
 * Operational Mode Information *
 ********************************/
#define  OP_MODE_SET_NORMAL           -1
#define  OP_MODE_SET_RX_ONLY           2
#define  OP_MODE_GET_CURRENT_MODE      0

#define  MSG_BS_LK_OPERATION_MODE_INFO_REQ        0x00401003
typedef BsLkHwInfoReq BsLkOperationalModeInfoReq;

#define  MSG_LK_BS_OPERATION_MODE_INFO_RSP        0x00401803
typedef struct s_LkBsOperationalModeInfoRsp
{
  SINT16     varResult;
  UINT16     varMode;
} LkBsOperationalModeInfoRsp;

/**************************
 *     Remote Fusing      *
 **************************/
#define FLAG_REMOTE_FUSING_START                                  0x0000
#define FLAG_REMOTE_FUSING_DATA                                   0x0001
#define FLAG_REMOTE_FUSING_TERM_COPY_RESTART                      0x0002
#define FLAG_REMOTE_FUSING_TERM_COPY                              0x0004
#define FLAG_REMOTE_FUSING_TERM_ABORT                             0x0008

typedef struct s_StartReq
{
  UINT16     varImageSize;
} StartReq;

typedef struct s_TransmissionReq
{
  UINT16     varDataNum;
  UINT16     len;
  UINT8      data[1024];  /* This is in multiples of 4, Min = 4, Max = 1024 */
} TransmissionReq;

typedef struct s_TerminationReq
{
  UINT16     varImageMod;
} TerminationReq;

typedef union u_ReqMsg
{
  StartReq           startReq;             /* Flag = 0000H */
  TransmissionReq    transmissionReq;      /* Flag = 0001H */
  TerminationReq     terminationReq;       /* Flag = 0002H, 0004H, 0008H */
} ReqMsg;

#define  MSG_BS_LK_REMOTE_FUSING_REQ                0x00210011
typedef struct s_BsLkRemoteFusingReq
{
  UINT16     varFlag;
  ReqMsg     reqMsg;
} BsLkRemoteFusingReq;

#define  MSG_LK_BS_REMOTE_FUSING_RSP                0x01210011
typedef struct s_LkBsRemoteFusingRsp
{
  SINT16     varResult;
 /* For StartRsp var2 = dummy
  * For TxRsp    var2 = data number
  * For TermRsp  var2 = action
  * */
  UINT16     var2;
} LkBsRemoteFusingRsp;

#define  MSG_LK_BS_FUSING_COMPLETE_NOTI_ID                   0x04210011
typedef struct s_LkBsRemoteFusingCompleteNoti
{
  SINT16     varResult;
  UINT16     varAction;
  UINT32     fwVersion;
} LkBsRemoteFusingCompleteNoti;

#define  MSG_BS_LK_FUSING_COMPLETE_ACK_ID                    0x08210011
typedef BsLkHwInfoReq BsLkRemoteFusingCompleteAck;

/*******************
 * Reset Message   *
********************/
#define  MSG_BS_LK_RESET_REQ                             0x002101FF
typedef struct s_LkBsResetReq
{
  UINT16       varDummy;
  UINT16       sequenceNo;
} BsLkResetReq;

#define  MSG_LK_BS_RESET_RSP                             0x012101FF
typedef struct s_LkBsResetRsp
{
  SINT16       varResult;
  UINT16       varDummy;
} LkBsResetRsp;



/********************************
 * Initialization Request NOTI  *
 ********************************/
#define  MSG_LK_BS_INITIALIZATION_NOTI_ID               0x04220101
typedef  struct s_LkBsInitializationReqNoti
{
  UINT8    varFwMode;
  UINT8    varLogAvailable;
  UINT16   varDummy;
  UINT16   dataDummy1;
  UINT16   dataDummy2;
  UINT32   fwVersion;
}LkBsInitializationReqNoti;

#define  MSG_BS_LK_INITIALIZATION_ACK_ID                0x08220101
typedef struct s_BsLkInitializationAck
{
  UINT32   varDummy;
  UINT32   dataTickTime;
  UINT8    systemId;
  UINT8    alarmAck;
  UINT16   dataDummy;
} BsLkInitializationAck;

/********************************/
/* Alarm Information */
/********************************/
#define  BIT_0                   0x00000001
#define  BIT_1                   0x00000002
#define  BIT_2                   0x00000004
#define  BIT_3                   0x00000008
#define  BIT_4                   0x00000010
#define  BIT_5                   0x00000020
#define  BIT_6                   0x00000040
#define  BIT_7                   0x00000080
#define  BIT_8                   0x00000100
#define  BIT_9                   0x00000200
#define  BIT_10                  0x00000400
#define  BIT_11                  0x00000800
#define  BIT_12                  0x00001000
#define  BIT_13                  0x00002000
#define  BIT_14                  0x00004000
#define  BIT_15                  0x00008000
#define  BIT_16                  0x00010000
#define  BIT_17                  0x00020000
#define  BIT_18                  0x00040000
#define  BIT_19                  0x00080000
#define  BIT_20                  0x00100000
#define  BIT_21                  0x00200000
#define  BIT_22                  0x00400000
#define  BIT_23                  0x00800000
#define  BIT_24                  0x01000000
#define  BIT_25                  0x02000000
#define  BIT_26                  0x04000000
#define  BIT_27                  0x08000000
#define  BIT_28                  0x10000000
#define  BIT_29                  0x20000000
#define  BIT_30                  0x40000000
#define  BIT_31                  0x80000000

#define BIT_CPRI                   0x8000 /*CPRI LINK indication bit*/
/* BIT_0 to BIT_28 are reserved for Common data alarm*/
#define  ALARM_DATA_LOW_TEMP          BIT_29
#define  ALARM_DATA_HIGH_TEMP         BIT_30
#define  ALARM_DATA_CLOCK_FAIL        BIT_31

#define  ALARM_DATA_VENDOR_SPEC       BIT_1
/* BIT_2 to BIT_7 are reserved */


#define  ALARM_DATA_OVER_POWER        BIT_31
#define  ALARM_DATA_VSWR_FAIL         BIT_29
#define  ALARM_DATA_LOW_GAIN          BIT_27
#define  ALARM_DATA_LNA_FAIL          BIT_26
#define  ALARM_DATA_DISABLED          BIT_25
#define  ALARM_DATA_SHUTDOWN          BIT_24



#define  MSG_BS_LK_ALARM_INFO_REQ       0x00310001
typedef struct s_BsLkAlarmInfoReq
{
  UINT16   varDummy;
  UINT16   varSeqNum;
} BsLkAlarmInfoReq;

#define  MSG_LK_BS_ALARM_INFO_RSP       0x01310001
typedef struct s_LkBsAlarmInfoRsp
{
  SINT16    varResult;
  UINT16    varSeqNum;
  UINT32    dataValidInd;
  UINT32    dataDummy1;
  UINT32    dataDummy2;
  UINT32    alarmDataCommon;
  UINT32    alarmDataPathA;
  UINT32    alarmDataPathB;
  UINT32    alarmDataPathC;
  UINT32    alarmDataPathD;
} LkBsAlarmInfoRsp;

/********************************
 *     Alarm Change NOTI        *
 ********************************/
/* NOTI */
#define  MSG_LK_BS_ALARM_CHANGE_NOTI_ID    0x04310002

typedef  LkBsAlarmInfoRsp LkBsAlarmChangeNoti;

#if 0
typedef struct s_LkBsAlarmChangeNoti
{
  SINT16   varResult;
  UINT16   varSeqNum;
  UINT16   dataDummy;
  UINT16   dataReserved;
  UINT32   alarmDataPathA;
  UINT32   alarmDataPathB;
} LkBsAlarmChangeNoti;
#endif
/* ACK */
#define  MSG_BS_LK_ALARM_CHANGE_ACK_ID     0x08310002
typedef struct s_BsLkAlarmChangeAck
{
  SINT16   varResult;
  UINT16   varSeqNum;
} BsLkAlarmChangeAck;

/*******************************
 *      Status Information     *
 *******************************/
/* Define max and min values for real world values reported in status message  */
#define STATUS_INFO_MAX_VSWR           3000
#define STATUS_INFO_MAX_TEMP           1000
#define STATUS_INFO_MIN_TEMP           -400
#define STATUS_INFO_MAX_TX_RF_PWR      500
#define STATUS_INFO_MIN_TX_RF_PWR      -3300
#define STATUS_INFO_MAX_DIG_IQ_LEV     10000000

#define  MSG_BS_LK_STATUS_INFO_REQ            0x00220021
typedef struct s_BsLkStatusInfoReq
{
	UINT16 varDummy;
	UINT16 sequenceNo;
}BsLkStatusInfoReq;

typedef struct s_StatusPathData
{
  UINT8        txOnOrOff;                 /* (0) 0=off,      1=on */
  UINT8        txEnableOrDisable;         /* (1) 0=disable,  1=enable  */
  UINT8        dataDummy1;                    /* (2) */
  UINT8        dataDummy2;                    /* (3) */
  UINT16       dataDummy3;                    /* (4~5) */
  SINT16       temperature;               /* (6~7) -400 to 1000 for -40.0 to 100.0 C */
  SINT16       txRfPower;                 /* (8~9) -3300 to 500 for -330 to 50.0 dBm */
  UINT16       dataDummy4;                    /* (10~11) */
  UINT16       returnLoss;                      /* (12~13) -3000 to 0, -30.00 to 0.00 dB (converted from 1.0 to 5.0) */
  UINT16       dataDummy5;                    /* (14~15) */
  UINT32       digitalIqLevelFaRssi[MAX_FAS];                 /* (20~23) */
  UINT32       dataDummy6;
  UINT32       dataDummy7;
  UINT32       dataDummy8;
  UINT32       dataDummy9;
  UINT32       digitalIqLevelFaTssi[MAX_FAS];
  UINT32       dataDummy10;
  UINT32       dataDummy11;
  UINT32       dataDummy12;
  UINT32       dataDummy13;
  UINT8        dataDummy14;
  UINT8        dataDummy15;
  UINT16       dataDummy16;
  UINT16       pathFaTxAtten[MAX_FAS];
  UINT16       dataDummy17;
  UINT16       dataDummy18;
  UINT16       dataDummy19;
  UINT16       dataDummy20;
} StatusPathData;

#define  MSG_LK_BS_STATUS_INFO_RSP            0x01220021
typedef struct s_LkBsStatusInfoRsp
{
  SINT16             varResult;
  UINT16             sequenceNo;
  UINT8              iqMap[MAX_CPRI_PORTS];
  UINT8              dataDummy1;                         /* (12) */
  UINT8              dataDummy2;                         /* (13) */
  UINT8              dataDummy3;                         /* (14) */
  UINT8              dataDummy4;                         /* (15) */
  UINT8              fwMode;                          /* (16) */
  UINT8              numberOfFA;                     /* (17)*/
  UINT16             dataDummy5;                         /* (14~15) */
  UINT8              mappingOfFAStatus[MAX_FAS];                 /* (16-17) 1 = C1,2 = C2*/
  UINT8              dataDummy6;                         /* (18) */
  UINT8              dataDummy7;                         /* (19) */
  UINT8              dataDummy8;                         /* (18) */
  UINT8              dataDummy9;                         /* (19) */
  UINT8              cvc[MAX_CPRI_PORTS];
  UINT8              dataDummy10;                         /* (22) */
  UINT8              dataDummy11;                         /* (23) */
  UINT8              dataDummy12;                         /* (22) */
  UINT8              dataDummy13;                         /* (23) */
  UINT8              txFilterBandwidth;
  UINT8              rxFilterBandwidth;
  UINT8              dlRatio;                        /* (24) */
  UINT8              ulRatio;                        /* (25) */
  UINT16             vswrShutdownAlarmThreshold;     /* (27) */
  UINT16             dataDummy14;
  UINT32             overPowerShutdownAlarmThreshold;   /* (28~31) */
  UINT32             lowPowerAlarmThreshold;         /* (36~39) */
  UINT32             centerFrequency;                     /* (40~43) */
  UINT8              dataDummy15;                         /* (23) */
  UINT8              dataDummy16;                         /* (23) */
  UINT8              dataDummy17;                         /* (23) */
  UINT8              dataDummy18;                         /* (23) */
  UINT8              dataDummy19;                         /* (23) */
  UINT8              dataDummy20;                         /* (23) */
  UINT8              dataDummy21;                         /* (23) */
  UINT8              dataDummy22;                         /* (23) */
  UINT8              dataDummy23;                         /* (23) */
  UINT8              dataDummy24;                         /* (23) */
  UINT16             dataDummy25;                         /* (23) */
  UINT16             dataDummy26;                         /* (23) */
  UINT16             dataDummy27;                         /* (23) */
  UINT8              faChannelBandwidthStatus[MAX_FAS];
  UINT8              dataDummy28;                         /* (23) */
  UINT8              dataDummy29;                         /* (23) */
  UINT8              dataDummy30;                         /* (23) */
  UINT8              dataDummy31;                         /* (23) */
  UINT32             txFaFrequency[MAX_FAS];
  UINT32             dataDummy32;                         /* (23) */
  UINT32             dataDummy33;                         /* (23) */
  UINT32             dataDummy34;                         /* (23) */
  UINT32             dataDummy35;                         /* (23) */
  UINT32             rxFaFrequency[MAX_FAS];
  UINT32             dataDummy36;                         /* (23) */
  UINT32             dataDummy37;                         /* (23) */
  UINT32             dataDummy38;                         /* (23) */
  UINT32             dataDummy39;                         /* (23) */
  UINT32             dataDummy40;                         /* (23) */
  UINT32             txDelay;                         /* (23) */
  UINT32             rxDelay;                         /* (23) */

  UINT16             dataDummy41;                         /* (23) */
  UINT16             dataDummy42;                         /* (23) */
  UINT16             dataDummy43;                         /* (23) */
  UINT16             dataDummy44;                         /* (23) */
  UINT16             dataDummy45;                         /* (23) */
  UINT16             dataDummy46;                         /* (23) */
  UINT16             dataDummy47;                         /* (23) */
  UINT16             dataDummy48;                         /* (23) */

  UINT8              dataDummy49;                         /* (23) */
  UINT8              dataDummy50;                         /* (23) */
  UINT8              dataDummy51;                         /* (23) */
  UINT8              dataDummy52;                         /* (23) */
  UINT8              dataDummy53;                         /* (23) */
  UINT8              dataDummy54;                         /* (23) */
  UINT8              dataDummy55;                         /* (23) */
  UINT8              dataDummy56;                         /* (23) */
  UINT8              dataDummy57;                         /* (23) */

  UINT8				 dataDummy58;
  UINT16   			 dataDummy59;

  UINT16   			 dataDummy60;
  UINT16   			 dataDummy61;
  UINT8   			 dataDummy62;
  UINT8   			 dataDummy63;

  UINT8              txPathFaControl[MAX_PATHS];
  UINT8              rxPathFaControl[MAX_PATHS];
  UINT8              dataDummy64[42];
  UINT8              multiMode;
  UINT8              powerBoost;
  UINT8              filterSwithingMode;
  UINT8              dataDummy65;
  StatusPathData     dataPath[MAX_PATHS];
} LkBsStatusInfoRsp;

/*****************
 * Delay Set     *
 *****************/
#define  DELAY_SET_MAX_NS          90000
#define  DELAY_SET_MIN_NS          15000

#define  MSG_BS_LK_DELAY_SETTING_REQ                 0x00220006
typedef struct s_BsLkDelaySettingReq
{
  UINT16         varDummy;
  UINT16         sequenceNo;
  UINT32         dataDummy;
  UINT32         txDelay;  /* Unit : Ns */
  UINT32         rxDelay;  /* Unit : Ns */
} BsLkDelaySettingReq;

#define  MSG_LK_BS_DELAY_SETTING_RSP                 0x01220006
typedef struct s_LkBsDelaySettingRsp
{
  SINT16       varResult;
  UINT16       sequenceNo;
} LkBsDelaySettingRsp;

/**********************
 * Tx ON/OFF Setting  *
 **********************/
#define  TX_ON_OFF_SET_OFF                           0
#define  TX_ON_OFF_SET_ON                            1

#define  MSG_BS_LK_TX_ON_OFF_SET_REQ                 0x0022000A
typedef struct s_BsLkTxOnOffReq
{
  UINT16         varDummy;
  UINT16         sequenceNo;
  UINT8          faId;
  UINT8          dataDummy1;
  UINT8          path;
  UINT8          txOnOrOff;
  UINT8          dataDummy2;
  UINT16         dataDummy3;
} BsLkTxOnOffReq;

#define  MSG_LK_BS_TX_ON_OFF_SET_RSP                 0x0122000A
typedef struct s_LkBsTxOnOffRsp
{
	SINT16       varResult;
	UINT16       sequenceNo;
	UINT16       faId;
	UINT16       dataDummy;
}LkBsTxOnOffRsp;

/************************
 * ENABLE/DISABLE SET   *
************************/
#define  MSG_BS_LK_ENABLE_OR_DISABLE_REQ                0x00220008
typedef struct s_BsLkEnableOrDisableReq
{
  UINT16         varDummy;
  UINT16         sequenceNo;
  UINT8          path;
  UINT8          control;
  UINT16         dataDummy;
} BsLkEnableOrDisableReq;

#define  MSG_LK_BS_ENABLE_OR_DISABLE_RSP                0x01220008
typedef  struct s_LkBsEnableOrDisableRsp
{
	SINT16 varResult;
	UINT16 sequenceNo;
}LkBsEnableOrDisableRsp;

/********************
 * DL-UL Ratio Set  *
*********************/
#define  DL_RATIO_MIN             26
#define  DL_RATIO_MAX             35
#define  UL_RATIO_MIN             12
#define  UL_RATIO_MAX             21

#define  MSG_BS_LK_DL_UL_RATIO_REQ                   0x0022000E
typedef struct s_BsLkDlUlRatioReq
{
  UINT32         varDummy;
  UINT8          dataDummy1;
  UINT8          dataDummy2;
  UINT8          dlRatio;
  UINT8          ulRatio;
} BsLkDlUlRatioReq;

#define  MSG_LK_BS_DL_UL_RATIO_RSP                   0x0122000E
typedef  LkBsDelaySettingRsp LkBsDlUlRatioRsp;

/*********************
 * Data Path Setting *
 *********************/

#define  MSG_BS_LK_DATA_PATH_REQ                     0x0022000C

typedef struct s_BsLkDataPathReq
{
  UINT16         varDummy;
  UINT16         sequenceNo;
  UINT8          iqMap[MAX_CPRI_PORTS];       /* 0, 1, 3, 5, 10, 15 or 20  */
  UINT8          dataDummy1;
  UINT8          dataDummy2;
  UINT8          dataDummy3;
  UINT8          dataDummy4;
  UINT8          dataDummy5;
  UINT8          numberOfFa;            /* Number of FAs, 1 or 2 */
  UINT16         dataDummy6;
  UINT8          mappingOfFa[MAX_FAS];
  UINT16         reportPeriod;
  UINT16         dataDummy7;
  UINT8          dataDummy8;
  UINT16         dataDummy9;
  UINT8          pathAChipDelay;
  UINT8          pathBChipDelay;
  UINT8          pathCChipDelay;
  UINT8          pathDChipDelay;
  UINT32        dataDummy10;
} BsLkDataPathReq;

#define  MSG_LK_BS_DATA_PATH_RSP                     0x0122000C
typedef  struct s_LkBsDataPathRsp
{
	SINT16    varResult;
	UINT16    sequenceNo;
	UINT8     dataDummy1;
	UINT16    dataDummy2;
}LkBsDataPathRsp;

/****************
 * FA Control   *
 ****************/
#define  FA_CONTROL_FA_NUM_MIN        1
#define  FA_CONTROL_FA_NUM_MAX        2
#define  FA_CONTROL_TX_ATTEN_MAX      270
#define  FA_CONTROL_GEN_NORMAL        0
#define  FA_CONTROL_GEN_INTERNAL      1

#define  MSG_BS_LK_FA_CONTROL_REQ                    0x0022000F
typedef struct s_BsLkFaControlReq
{
  UINT16         varDummy;
  UINT16         sequenceNo;
  UINT8          faId;
  UINT8          dataDummy1;
  UINT8          dataDummy2;
  UINT8          dataDummy3;
  UINT16         txAttenPathA;
  UINT16         txAttenPathB;
  UINT16         dataDummy4;
  UINT16         dataDummy5;
  UINT32         txEarfcn;
  UINT32         rxEarfcn;
  UINT8          txPathAOnOff;
  UINT8          txPathBOnOff;
  UINT8          txPathCOnOff;
  UINT8          txPathDOnOff;
  UINT8          rxPathAOnOff;
  UINT8          rxPathBOnOff;
  UINT8          rxPathCOnOff;
  UINT8          rxPathDOnOff;
  UINT8          dataDummy6;
  UINT8          dataDummy7;
  UINT8          dataDummy8;
  UINT8          dataDummy9;
  UINT16         dataDummy10;
  UINT16         dataDummy11;
  UINT32         dataDummy12;
} BsLkFaControlReq;

#define  MSG_LK_BS_FA_CONTROL_RSP                    0x0122000F

typedef struct s_LkBsFaControlRsp
{
  SINT16       varResult;
  UINT16       sequenceNo;
  UINT8        dataDummy1;
  UINT8        dataDummy2;
  UINT16       dataDummy3;
} LkBsFaControlRsp;


/****************
 * FA Control For Rx  *
 ****************/

#define  MSG_BS_LK_FA_CONTROL_RX_REQ                    0x00220032
typedef struct s_BsLkFaControlRxReq
{
  UINT32         varDummy;
  UINT8          faNum;
  UINT8          onOff;
  UINT8          dataDummy;
  UINT8          signalGen;
  UINT16         rxAttenTPathA;      /* DUC control path A */
  UINT16         rxAttenTPathB;      /* DUC control path B */
  UINT32         freq;
} BsLkFaControlRxReq;

#define  MSG_LK_BS_FA_CONTROL_RX_RSP                    0x01220032


typedef  LkBsDelaySettingRsp LkBsFaControlRxRsp;


/****************************
 * Alarm Threshold Setting  *
 ****************************/
#define  ALARM_THRESHOLD_VSWR_MIN                 0
#define  ALARM_THRESHOLD_VSWR_MAX                 80
#define  ALARM_THRESHOLD_IQ_MIN                   0
#define  ALARM_THRESHOLD_IQ_MAX                   1000000
#define  ALARM_THRESHOLD_RSSI_MIN                 (-32760)
#define  ALARM_THRESHOLD_RSSI_MAX                 (-1)

#define  MSG_BS_LK_ALARM_THRESHOLD_SETTING_REQ       0x00220031
typedef struct s_BsLkAlarmThresholdSettingReq
{
  UINT16         dummy1;
  UINT16         sequenceNo;
  UINT16         dummy2;
  UINT16         vswrShutdownThreshold;
  UINT32         overpowerAlarmThreshold;
  UINT32         lowPowerThreshold;
} BsLkAlarmThresholdSettingReq;

#define  MSG_LK_BS_ALARM_THRESHOLD_SETTING_RSP       0x01220031
typedef struct s_LkBsAlarmThresholdSettingRsp
{
  SINT16       varResult;
  UINT16       sequenceNo;
} LkBsAlarmThresholdSettingRsp;


/*******************
 * Carrier LO Set  *
 *******************/
#define  CARRIER_LO_SET_FA_NUM_MIN           1
#define  CARRIER_LO_SET_FA_NUM_MAX           22
#define  CARRIER_LO_SET_POSITION_NONE        0
#define  CARRIER_LO_SET_POSITION_ODD         1
#define  CARRIER_LO_SET_POSITION_EVEN        2

#define  MSG_BS_LK_CARRIER_LO_SETTING_REQ            0x0022000D
typedef struct s_BsLkCarrierLOSettingReq
{
  UINT32         varDummy;
  UINT8          faNum;
  UINT8          localOscillatorOddOrEven;
  UINT8          dataDummy1;
  UINT8          dataDummy2;
} BsLkCarrierLOSettingReq;

#define  MSG_LK_BS_CARRIER_LO_SETTING_RSP            0x0122000D
typedef  LkBsDelaySettingRsp LkBsCarrierLoSettingRsp;

/*******************
 * Time Set  *
 *******************/
#define  MSG_BS_LK_TIME_SET_REQ            0x00210005
typedef struct s_BsLkTimeSetReq
{
  UINT32         varDummy;
  UINT32         timeTick;
  UINT8          dataReserved[8];
} BsLkTimeSetReq;

#define  MSG_LK_BS_TIME_SET_RSP            0x01210005
typedef  LkBsDelaySettingRsp LkBsTimeSetRsp;

/************************************************************************************************/
/* Initialization */

/* Tx Attenuation Setting */
/*TODO Need to check with Latest Document */
#define MSG_BS_LK_TX_ATTEN_REQ                                 0x00220001
typedef struct s_BsLkTxAttenReq
{
  UINT32         varDummy;
  UINT8          path;
  UINT8          dummy;
  UINT16         attenTenthsOfDb;
  UINT8          dummyA;
} BsLkTxAttenReq;

#define MSG_LK_BS_TX_ATTEN_RSP                                 0x01220001
typedef struct s_LkBsTxAttenRsp
{
  SINT16      varResult;
  UINT16      dummy;
} LkBsTxAttenRsp;


/* LNA Attenuation Setting */
/*TODO Need to check with Latest Document */
#define MSG_BS_LK_LNA_ATTEN_REQ                                 0x00220010
typedef struct s_BsLkLnaAttenReq
{
  UINT32         varDummy;
  UINT8          path;
  UINT8          dummy[3];
  UINT16         attenTenthsOfDb;
  UINT8          dummyA;
} BsLkLnaAttenReq;

#define MSG_LK_BS_LNA_ATTEN_RSP                                 0x01220010
typedef struct s_LkBsLnaAttenRsp
{
  SINT16      varResult;
  UINT16      dummy;
} LkBsLnaAttenRsp;

/* Functions Prototypes */
TBOOL procMsgRecvdFromLk(
  IN UINT32   msgId,
  IO DecBuf   *decBuf
  );
TBOOL LkBsMsgHandler(
  IO UINT8   *data,
  IN UINT32   len
  );
TimerId startTestTimer(UINT16 timeDur, MsgId msgId );


TBOOL procLkBsResetRsp(
  IO DecBuf    *decBuf
  );

typedef  LkBsResetRsp RhDehRsp;

/************************************************************************************************/
/* Installation*/
#define MSG_BS_LK_SW_INSTALL_REQ                                    0x00210014
typedef struct s_BsLkSwInstallReq
{
  UINT16     varDummy;
  UINT16     sequenceNo;
  UINT32     ipAddr;
  String256  fileName;
  String50   userName;
  String50   password;
  UINT32     port;
}BsLkSwInstallreq;

#define MSG_LK_BS_SW_INSTALL_RSP                                   0x01210014
typedef struct s_LKBsSwInstallRsp
{
  SINT16     varResult;
  UINT16     sequenceNo;
  UINT16     dummy;
}LKBsSwInstallRsp;


/************************************************************************************************/
/* Activation*/
#define MSG_BS_LK_SET_ACTIVE_SIDE_REQ                                  0x00210015
typedef struct s_BsLkSwActivateReq
{
  UINT16     varDummy;
  UINT16     sequenceNo;
  UINT8      activeSide;
}BsLkSwActivateReq;

#define MSG_LK_BS_SET_ACTIVE_SIDE_RSP                                   0x01210015
typedef struct s_LkBsSwActivateRsp
{
  SINT16     varResult;
  UINT16     sequenceNo;
  UINT16     dummy;
}LkBsSwActivateRsp;

#endif

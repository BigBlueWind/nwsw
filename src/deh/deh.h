/**
 * @file    deh.h
 * @brief API for Device Handler (Abbrev: DEH)
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author Amar Ramteke, mkata, spolepally, aagrawal, kgummad
 * @date   27-05-2011
 * @details
 */


#ifndef DEH_H
#define DEH_H
#include <pthread.h>
/* Common .h files*/
#include "all_cmn.h"
#include "cmn_types.h"
#include "imc_timer.h"
#include "imc_proc.h"

#include "bs_tables.h"
#include "bsm_deh_if.h"
#include "deh_rh_func.h"
#include "deh_rh_fillFunc.h"
#include "rh_deh_if.h"
#include "all_cfg.h"
#include "generic_table.h"
#include "bs_config.h"
/* Print Functions for Radio Head Messages */
#include "deh_rh_msg_print.h"
#include "print_tlv.h"
#include "lte_bb_platform.h"
#include "get_oam_func.h"
#include "start_indication.h"
#include "prm_deh_if.h"


#define RH_PATH_A 0
#define RH_PATH_B 1

#define RH_USR_DISABLE 1
#define RH_USR_ENABLE  2
#define RH_DU_ENABLE   3

#define DEBUG_PORT    "eth1"
#define BACKHAUL_PORT "eth2"

#define ERRORSTRING_LENGTH 256
#define RH_SIDE_A  1
#define RH_SIDE_B  2
#define CPRI_REQ   9
/*hash defines*/

/* ################# DEH TYPES ########################## */

typedef struct s_MsgBuf
{
  SINT32 len;
  UINT8  val[SIZE_RECV_DATA];
} MsgBuf;

/* DEH States */
typedef enum e_DehState
{
  DEH_STATE_UNDEFINED = 1,        /*DEH in Undefined state*/
  DEH_STATE_WAIT_CFG,             /*DEH is waiting INIT configuration from BSM*/
  DEH_STATE_WAIT_INIT_NOTI,       /*DEH is waiting for INIT_NOTI from RH->RH not connected*/
  DEH_STATE_IDLE,                 /*DEH is IDLE when RH connected*/
  DEH_STATE_WAIT_DEV_ENABLE_RSP,  /*DEH is waiting for Responses from RH at OPSTART*/
  DEH_STATE_WAIT_DEV_DISABLE_RSP, /*DEH is waiting for Responses from RH at OPSTOP*/
  DEH_STATE_MAX,
} DehState;

typedef enum e_Device
{
  DEV_BASEBAND = 1,
  DEV_RADIOHEAD,
  DEV_BACKHAULPORT,
  DEV_DEBUGPORT,
  DEV_TEMPSENSOR,
  DEV_CPRI,
  DEV_GPS,
  DEV_CLOCK,
  DEV_MAX
}Device;

typedef enum e_Procedures
{
  PROC_NONE =1,
  PROC_DEV_INIT,
  PROC_RESET ,
  PROC_OPSTART,
  PROC_OPSTOP ,
  PROC_SW_INSTALL,
  PROC_SW_ACTIVATE,
  PROC_MAX
}Procedures;

typedef enum e_DevConnectionStatus
{
  DEV_UNCONNECTED = 0,
  DEV_CONNECTED,
  DEV_UNKNOWN,
  DEV_NOTMONITORED
} DevConnectionStatus;

typedef MsgTrace MsgBsmDehSetMsgTrace;

typedef struct s_DevStatus
{
  DevConnectionStatus  devConnectionStatus[MAX_ROWS_IN_COMPONENTSTATUS_TABLE + 1];
  UINT8                errorString[MAX_ROWS_IN_COMPONENTSTATUS_TABLE + 1][MAX_SIZE_COMPONENTSTATUS_ERRORSTRING + 1];
} DevStatus;

typedef ModTimersInfo DehTimersInfo;

/* BasebandHwInfo */
/* If you want to add more info refer to Inventory Info structure of platform API */
typedef struct s_BasebandHwInfo
{
  SINT8            hwRev[EE_HW_REV_SIZE];
}BasebandHwInfo;

typedef struct s_DeviceAlarmsInfo
{
  TBOOL  radioHead;
  TBOOL  gps;
  TBOOL  ethBackhaul;
  TBOOL  ethDebug;
  TBOOL  tempSensor;
  TBOOL  baseBand;
  TBOOL  cpri;
  TBOOL  baseBandClock;
}DeviceAlarmsInfo;

typedef struct s_RhPackageStatus
{
  UINT16  runningSide;
  UINT16  nonRunningSide;
  UINT8   runningVersion[MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1];
  UINT8   nonRunningVersion[MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1];
  UINT8   versionToActivate[MAX_SIZE_COMPONENTSTATUS_COMPONENTVERSION + 1];
  UINT8   runningVersionBuildDate[MAX_FWBUILDINFO_DESC + 1];
  UINT8   nonRunningVersionBuildDate[MAX_FWBUILDINFO_DESC + 1];
}RhPackageStatus;

typedef struct s_DeviceFunctionalStatus
{
  TBOOL      deviceConnectedState[DEV_MAX];
  TBOOL      deviceConfigureState[DEV_MAX];
  TBOOL      deviceOperationalState[DEV_MAX];
  TBOOL      deviceMonitoredState[DEV_MAX];
  Procedures deviceProcedureState[DEV_MAX];
  Procedures deviceLastCompletedProcedure[DEV_MAX];
}DeviceFunctionalStatus;

/*************************************************************/
/************       GPS Status Context        *************/
typedef struct s_DehGpsContext
{
  TBOOL            isGpsDataValid;
  LatitudeInfo     dehGpsLatitudeInfo;
  LongitudeInfo    dehGpsLongitudeInfo;
  DateInfo         dehGpsDateInfo;
  TimeInfo         dehGpsTimeInfo;
} DehGpsContext;

extern pthread_mutex_t         g_gpsMutex;

/**************** GPS Status Context Ends Here ****************/

/***************** POST Test *******************/

typedef enum PostDevice
{
  POST_SHORT_DDR = 0,
  POST_LONG_DDR,
  POST_SHORT_NANDFLASH,
  POST_LONG_NANDFLASH,
  POST_SWITCH,
  POST_MAX
} PostDevice;

typedef struct s_PostDeviceStatus
{
  TBOOL isPerformed;
  TBOOL status;
}PostDeviceStatus;

typedef struct s_DehPostContext
{
  TBOOL            isPostSuccess;
  PostDeviceStatus postDeviceStatus[POST_MAX];
  UINT8            errorString[ERRORSTRING_LENGTH];
} DehPostContext;

/**************** POST Status Context Ends Here ****************/

/***************** BIST Test *******************/

typedef enum BistDevice
{
  BIST_BACKHAUL_PORT = 0,
  BIST_DEBUG_PORT,
  BIST_MAX
} BistDevice;

typedef struct s_BistDeviceStatus
{
  TBOOL  status;
} BistDeviceStatus;

typedef struct s_DehBistContext
{
  TBOOL             isBistRunning;
  TBOOL             isBistPerformed;
  TBOOL             isBistSuccess;
  BistDeviceStatus  bistDeviceStatus[BIST_MAX];
  UINT8             errorString[ERRORSTRING_LENGTH];
} DehBistContext;

extern pthread_mutex_t   g_bistMutex;
extern pthread_t         g_bistThread;

/**************** BIST Status Context Ends Here ****************/

/* DEH Context */
typedef struct s_DehContext
{
  VersionInfo      versionInfo;
  BasebandHwInfo   baseBandversionInfo;
  FilePath         configDir;
  SINT32           sockFd;
  SINT32           sockFdNoti;
  UINT8            rhIpAddr[MAX_SIZE_ENBADDRESSES_RADIOHEADIPADDRESS + 1];
  UINT8            backhaulIpAddr[MAX_SIZE_ENODEBCONFIG_ENODEBSTATICIPADDR + 1];
  UINT8            backhaulNetMaskAddr[MAX_SIZE_ENODEBCONFIG_ENODEBNETMASKADDR +1];
  UINT8            debugIpAddr[MAX_SIZE_ENODEBCONFIG_ENODEBSTATICIPADDR + 1];
  UINT8            debugNetMaskAddr[MAX_SIZE_ENODEBCONFIG_ENODEBNETMASKADDR +1];
  LkFwInfoTable    rhFwInfoTable;
  LkHwInfoTable    rhHwInfoTable;
  DehState         dehState;
  UINT8            totalTimers;
  DehTimersInfo    dehTimersInfo;
  RhDefaultCfg     rhDefaultCfg;
  CmdRef           resetRhCmdRef;
  TBOOL            isRhRspDeviceStatusReceived;
  TBOOL            isRhRspTimerRunning;
  TBOOL            isRhRspReceived;
  TBOOL            isBackHaulIpRecieved;
  TBOOL            isUnlocked;
  TBOOL            isBsmDehDeviceInitReqReceived;
  LkAlarmTable     rhAlarmsInfo;
  DeviceAlarmsInfo devAlarmsInfo;
  REAL             temperature;
  RhPackageStatus  rhPackageStatus;
  UINT16           noOfCpriReqSent;
  DehGpsContext    gpsCtx;
  DehPostContext   postCtx;
  DehBistContext   bistCtx;
  DeviceFunctionalStatus deviceFunctionalStatus;
} DehContext;

extern DehContext           g_DehCtx;

/* Gets Global DEH Context */
#define getDehContext()      &(g_DehCtx)

/* Gets Timers Info in Global DEH Context */
#define getDehTimersInfo()   &(g_DehCtx.dehTimersInfo)

/* Gets Radio Head Command Reference Number
 * If timeout occurs then this can be used to send Failure Response to BSM*/
#define getRhRef()           &(g_DehCtx.rhCmdRef)

/* Gets Radio Head IP address */
#define getRhIpAddress()     &(g_DehCtx.rhIpAddr[0])

/* Gets Socket FD */
#define getSocketFd()        &(g_DehCtx.sockFd)

/* Gets Socket FD NOTI */
#define getSocketFdNoti()    &(g_DehCtx.sockFdNoti)

/* Get Radio Head Configuration from Context */
#define getRhDefaultCfg()    &(g_DehCtx.rhDefaultCfg)

/* Gets whether Radio Head Response timer is Running or Not */
#define getRhRspTimerState() &(g_DehCtx.isRhRspTimerRunning)

/* Gets Radio Head FirmWare Info */
#define getRhFirmWareInfo()  &(g_DehCtx.rhFwInfoTable)

/* Gets Radio Head Hardware Info */
#define getRhHardWareInfo()  &(g_DehCtx.rhHwInfoTable)

/* Gets Radio Head Alarms Info in Global DEH Context */
#define getRhAlarmsInfo()    &(g_DehCtx.rhAlarmsInfo)

/* Gets Dev Alarms Info in Global DEH Context */
#define getDeviceAlarmsInfo() &(g_DehCtx.devAlarmsInfo)

/* Gets Dev Alarms Info in Global DEH Context */
#define getRhPackageStatus()  &(g_DehCtx.rhPackageStatus)

/* Gets Dev Alarms Info in Global DEH Context */
#define getDeviceFunctionalStatus() &(g_DehCtx.deviceFunctionalStatus)

/* To get the Global context of GPS*/
#define getGlobalDehGpsContext()    &(g_DehCtx.gpsCtx)

/* Latitude Information Response */
#define getLatitudeInfoRsp()    &(g_DehCtx.gpsCtx.dehGpstLatitudeInfoRsp)
/* Longitude Information Response */
#define getLongitudeInfoRsp()   &(g_DehCtx.gpsCtx.dehGpsLongitudeInfoRsp)
/* Date Information Response */
#define getDateInfoRsp()        &(g_DehCtx.gpsCtx.dehGpsDateInfoRsp)
/* Time Information Response */
#define getTimeInfoRsp()        &(g_DehCtx.gpsCtx.dehGpsTimeInfoRsp)

/* To get the Global context of BIST*/
#define getGlobalDehBistContext()  &(g_DehCtx.bistCtx)

/* To get the Global context of POST */
#define getGlobalDehPostContext()  &(g_DehCtx.postCtx)


/*************************************************************/
/************       Device Status Context        *************/
/* Device Status Context  */
typedef struct s_DeviceStatusContext
{
  DevStatus   lastSavedDevStatus;
  DevStatus   presentDevStatus;
  DevStatus   *presentStatus;
  DevStatus   *lastSaved;
} DeviceStatusContext;

extern DeviceStatusContext  g_DeviceStatusCtx;

#define getDeviceStatusContext()       &(g_DeviceStatusCtx)

/* Gets Present Status Info Pointer */
#define getDevicePresentStatusPtr()    (g_DeviceStatusCtx.presentStatus)

/* Gets Old Status Info  Pointer */
#define getDeviceLastSavedStatusPtr()  (g_DeviceStatusCtx.lastSaved)

/********************** Device Status Context Ends Here ******/


/*************************************************************/
/************ Radio head Response Timers Context *************/

typedef struct s_RhConfigRspTimerInfo
{
  UINT8  noOfMsgsWaitingForRsp;
  CmdRef cmdRef;
}RhConfigRspTimerInfo;

typedef enum e_OpStartStopTimerName
{
  DEH_TIMER_OP_START = 0,
  DEH_TIMER_OP_STOP,
  DEH_TIMER_NONE,
}OpStartStopTimerName;

typedef enum e_DehCmdType
{
  RSP_USER_INITIATED = 0,
  RSP_USER_NOTI,
  RSP_UNEXPECTED
}DehCmdType;

/* For Op-Start and Op-Stop Cmd Ref is not required
 * No need to maintain separate variables for Both Paths as we are
 * receiving same message ID from Radio head Response */
typedef struct s_RhOpStartStopRspTimerInfo
{
  OpStartStopTimerName  timerActive;
  UINT8                 noOfMsgsWaitingForRsp;
}RhOpStartStopRspTimerInfo;

typedef struct s_WaitRhInitTimerInfo
{
  DehCmdType  dehCmdType;
  TBOOL       isRhNotiReceived;
  SetObjReq   setReq;
}WaitRhInitTimerInfo;

typedef struct s_RhEnableRspInfo
{
  UINT8     noOfMsgsWaitingForRsp;
}RhEnableRspInfo;

typedef struct s_RhDisableRspInfo
{
  UINT8     noOfMsgsWaitingForRsp;
}RhDisableRspInfo;

typedef struct s_RhConfigRspInfo
{
  UINT8           noOfMsgsWaitingForRsp;
  TBOOL           isFaControlRspReceived;
  TBOOL           isAlarmThresholdRspReceived;
  TBOOL           isSetDelayRspReceived;
  SetObjReq       setReq;
}RhConfigRspInfo;

typedef struct s_RhStatusRspInfo
{
  UINT8           noOfMsgsWaitingForRsp;
  GetObjReq       getReq;
}RhStatusRspInfo;

typedef struct s_DehRhRspTimersContext
{
  WaitRhInitTimerInfo         rhInitTimerInfo;
  RhEnableRspInfo             rhEnableRspInfo;
  RhDisableRspInfo            rhDisableRspInfo;
  RhConfigRspInfo             rhConfigRspInfo;
  RhStatusRspInfo             rhStatusRspInfo[3];
}DehRhRspTimersContext;

extern DehRhRspTimersContext     g_DehRhRspTimersCtx;

#define getRhRspTimersContext()  &(g_DehRhRspTimersCtx)

/* Gets Enable Rsp Timer Info */
#define getEnableRspInfo()  &(g_DehRhRspTimersCtx.rhEnableRspInfo)

/* Gets Disable Rsp Timer Info */
#define getDisableRspInfo() &(g_DehRhRspTimersCtx.rhDisableRspInfo)

/* Gets Configure Rsp Timer Info */
#define getConfigRspInfo()  &(g_DehRhRspTimersCtx.rhConfigRspInfo)

/* Gets RH Status Rsp Timer Info */
#define getStatusRspInfo(cmdOrigin)  &(g_DehRhRspTimersCtx.rhStatusRspInfo[cmdOrigin])

/********************** Timers Context Ends Here **********************/

typedef DehState (*dehMsgHandlerFp)(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );

typedef struct s_DehStateMachine
{
  dehMsgHandlerFp  fp;
  DehState         nextStates[2]; /*Can we change it to 1 for DEH?*/
} DehStateMachine;

void schedDehMsgHandler(
  IN ModuleId   modId,
  IN MsgHandler msgHandler
);

/***************** Macros **********************/

  /* Macro to check the state of DEH */
#define dehCheckState(context, state) \
                                 (context->dehState EQ state) ? TRUE : FALSE
 /* Macro to set the state of DEH */
#define dehSetState( context, state) \
                                 (context)->dehState = state



/***************** DEH init/config/exit functions ***********************/
void dehInit(
  IO UINT8       *configFilePath
  );
void initDehStatusCtx(void);

void initDehPostCtx();

void initDehGpsCtx(void);

void initComponentStatusTable(void);

void getBaseBandHardwareInfo();

void dehMain(Msg* msg);

DehState dehProcInitConfigReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );
DehState dehProcConfigReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );
DehState dehProcDeviceInitReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );
inline DehState dehProcExitInAllStates(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );

void dehUpdateRhConfigTable();

/***************** DEH common functions ***********************/

void initDehStateMachine(void);

char *getDehStateName(
  IN DehState state
  );

void dehGetAndSendTableData(
  IO GetObjReq   *getReq
  );

DehState dehProcSetObjReq(
 IN Msg              *msg,
 IO DehContext       *dehContext
 );

void dehFillAndSendSetObjSucc(
  IN SetObjReq         *setReq
  );

void dehFillAndSendSetObjFail(
  IN SetObjReq         *setReq,
  IN BsErrorValue      bsErrorValue,
  IN TagInfo           *errTagInfo
  );

DehState dehProcGetObjReq(
 IN Msg              *msg,
 IO DehContext       *dehContext
 );

void dehFillAndSendGetObjFail(
  IN GetObjReq          *getReq,
  IN BsErrorValue        bsErrorValue
  );

inline DehState dehProcSetMsgTraceInAllStates(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );

inline DehState dehProcSetLogCfgInAllStates(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );

inline DehState dehProcHelloInAllStates(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );

BsErrorValue dehAlarmMapping(
  IN Device deviceNo
);

void dehSetDeviceAlarmInfo(
  IN Device deviceNo,
  IN TBOOL alarmInfo
);

TBOOL dehIsAlarmPresent(
  IN Device deviceNo
);

void dehCreateAndSendComponentDownInd(
  IN Device device,
  IN BsErrorValue errNum,
  IN UINT8 *errString,
  IN UINT8 *resourceString
  );

void dehCreateAndSendComponentUpInd(
  IN Device device,
  IN BsErrorValue errNum,
  IN UINT8 *resourceString
  );

/************** DEH OPSTART and OPSTOP message handling ********/
DehState dehProcOpStartReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );
DehState dehProcOpStartReqInInitNoti(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );

DehState dehProcOpStopReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );
/***************** DEH Timers functions ***********************/
void readDehTimersFromCfg(
  IN UINT8       *configFilePath
  );
void startDehTimer(
  IN TimerName       timerType,
  IO DehContext      *dehContext,
  IO UINT32          *userInfo
  );
void  stopDehTimer(
  IO TimerName   type
  );
inline DehState dehProcTimerExpiry(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );
void doActionsOnWaitAllDeviceTimeout();
/******************   DEH Component status functions ********************/
void getAndSendComponentStatusTable(
  IO  GetObjReq   *getReq
  );

void dehUpdateDeviceStatus();

void dehUpdateDeviceAlarmStatus();

void dehBsmReportDeviceChangeStatus();

DevConnectionStatus dehGetStatusFromDevice(
  IO Device device
  );

void dehGetDeviceErrorInfo(
  IO Device device,
  IO UINT8 *deviceErrorDetail
  );

UINT8* dehGetDeviceInfo(
  IO Device device
  );

UINT8* dehGetDeviceVersion(
  IO Device device
  );
/******************   DEH GPS functions ********************/

void *gpsThreadMain();

DehState dehProcGpsDetailsReq(
 IN Msg              *msg,
 IO DehContext       *dehContext
 );

void getAndSendGpsStatusTable(
 IO  GetObjReq   *getReq
 );

TBOOL dehProcGpsStatusReq(
 MsgDehBsmGpsStatusSucc  *gpsDetSucc
 );

void decGpsDehStatusInfoRsp(
 IO  MsgDehBsmGpsStatusSucc    *gpsStatusSucc,
 IO  GpsStatus *statusTable
 );

TBOOL dehIsGpsActive();
UINT8* dehGpsErrorString();
TBOOL dehGpsReset();


/******************   DEH RH functions ********************/
void getAndSendRadioHeadStatusTable(
  IN  GetObjReq   *getReq,
  IO  DehContext  *dehContext
  );

TBOOL dehIsRadioheadActive();

TBOOL dehIsCpriActive();

UINT8* dehRadioHeadErrorString();

UINT8* dehRadioheadInfo();

void dehProcSendRhAlarmMsgToBsm(
  IN LkAlarmTable *rspMsg
  );

/******************   DEH LED functions ********************/
void initLedColorAndUpdateStatusTable(void );

void getAndSendLedStatusTable(
  IO  GetObjReq   *getReq
  );

DehState dehProcSetLedColor(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );

/******************   DEH Temperature Sensor functions ********************/
TBOOL dehInitTempSensor(void );

TBOOL dehReadTempSensor(
  REAL *temperature
  );

TBOOL dehIsTempSensorActive();

UINT8* dehTempSensorErrorString();

/****************   DEH Baseband Functions ******************/

void *bistThreadMain();

TBOOL dehApplyLongPost();

TBOOL dehIsBasebandActive();

UINT8* dehBasebandPostErrorString();

UINT8* dehBasebandBistErrorString();

void dehUpdateBasebandHardwareVersion(
  BasebandHwInfo *dehVersionInfo
  );

TBOOL dehGetBasebandProductId(
  UINT8 *productId
  );

DehState dehProcBistReq(
  IO SetObjReq   *setReq,
  IO DehContext  *dehContext
  );

DehState dehProcLongPostReq(
  IO SetObjReq   *setReq,
  IO DehContext  *dehContext
  );

/****************   DEH Ethernet Functions ************/
void  getAndUpdateEnodebIpAddr();

void  setAndUpdateEnodebIpAddr();

void dehProcSetOnEnbAddress(
  IO SetObjReq   *setReq,
  IO DehContext  *dehContext
  );
DehState dehProcGetEnbAddrReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );

TBOOL dehIsEthernetLinkActive(
  EthernetLink linkNum
  );

UINT8* dehEthernetLinkErrorString(
  EthernetLink linkNum
  );

UINT8* ethernetLinkName(
  EthernetLink linkNum
  );
TBOOL dehGetMacAddress(
  IO UINT8 *ethernetAddr,
  IO UINT8  portNum
  );
TBOOL dehSetMacAddress(
  IO UINT8  *ethernetAddr,
  IO UINT8   portNum
  );
DehState dehProcSetIpAddrReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );

/****************   DEH Clock Functions ************/

UINT8* getdehAD9548State();

TBOOL dehIsCdce62005ClockActive();

UINT8* dehCdce62005ErrorString();

/****************   DEH RH Software Upgrade Functions ************/
DehState dehProcRhSwInstallReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );
DehState dehProcRhSwActivateReq(
  IN Msg           *msg,
  IO DehContext    *dehContext
  );
void getIpToHexValue(
  IN UINT8  *ip,
  IO UINT32 *ipValue
  );
char *getDehProceduralStateName(
  IN DehState state
  );



/******************Added for Optimisation**********/
TBOOL isConnected(
  IN Device deviceno
  );
void setConnected(
  IN Device deviceno
  );
void setNotConnected(
  IN Device deviceno
  );
TBOOL isConfigured(
  IN Device deviceno
  );
void setConfigured(
  IN Device deviceno
  );
void setNotConfigured(
  IN Device deviceno
  );
TBOOL isOperational(
  IN Device deviceno
  );
void setOperational(
  IN Device deviceno
  );
void setNotOperational(
  IN Device deviceno
  );
TBOOL isProcedureOnGoing(
  IN Device deviceno,
  IN Procedures onGoingProcedure
  );
void setProcedureOngoing(
  IN Device deviceno,
  IN Procedures onGoingProcedure
  );
void setProcedureCompleted(
  IN Device deviceno,
  IN Procedures completedProcedure
  );
TBOOL isAnyProcedureOnGoing(
  IN Device deviceno
  );
char *getOngoingProcedureName(
  IN Device deviceno
  );
char *getDevName(
  IN Device deviceno
  );
char *getRhPathName(
IN UINT16 path
);
void createAndSendComponentStatusToBsm();
DehCmdType isUserInitiatedResetProcedure();
TBOOL isVersionActivated();
void doRhConfigAndSendOpstartRspToBsm();
void dehStoreDeviceMonitoredInCtx(
  IN DeviceMonitorConfig *config
);
TBOOL isDeviceMonitored(
  IN Device deviceno
);
#endif /* DEH_H */

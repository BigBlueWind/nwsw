/**
 * @file    bsm.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   API for Base Station Manager (Abbrev: BSM).
 *
 * @author:  Shweta Polepally
 * @date:
 * @details: Initial Version
 **/

#ifndef BSM_H
#define BSM_H
/* Common .h files*/
#include "all_cmn.h"
#include "cmn_types.h"
/* BSM module .h files */
/* BSM to other module interface .h files */
#include "bsm_lth_if.h"
#include "bsm_fth_if.h"
#include "bsm_prm_if.h"
#include "bsm_sih_if.h"
#include "bsm_deh_if.h"
#include "bsm_pmh_if.h"
#include "bsm_udh_if.h"
#include "generic_table.h"
#include "bs_tables.h"
#include "oper_config_read_write.h"
#include "bsm_rrm_if.h"
#include "bsm_ueh_if.h"
#include "bsm_sctp_if.h"
#include "bsm_s1ap_if.h"
#include "bsm_egtp_if.h"
#include "tlv_enc_dec.h"
#include "bsm_cih_if.h"
#include "all_cfg.h"
#include "print_tlv.h"
#include "print_noti.h"
#include "get_oam_func.h"
#include "noti_msg_get_set.h"
#include "start_indication.h"

#include "noti_msg_get_set.h"
#include "empty_tables.h"
#include "print_cmn.h"

/********************/
/********ENUMS*******/
/********************/
typedef enum e_BsmProceduralState
{
  BS_WAIT_INIT_CMPL = 1,
  BS_WAIT_INITIAL_CONFIG_CMPL = 2,
  BS_WAIT_DEVICE_INIT_CMPL = 3,
  BS_WAIT_COLLECT_INFO_CMPL = 4,
  BS_WAIT_REGISTRATION_CMPL = 5,
  BS_IDLE = 6,
  BS_WAIT_REBOOT_CMPL,
  ENODEB_WAIT_RESTART_CMPL,
  ENODEB_WAIT_CONFIG_CMPL,
  ENODEB_WAIT_ENABLE_CMPL,    /* Unlock in progress */
  ENODEB_WAIT_DISABLE_CMPL,   /* Lock in progress */
  WAIT_SW_ACTIVATE_CMPL,
  BSM_STATE_MAX
} BsmProceduralState;



/* Enum with values stating why BSM is performing disable procedure */

typedef enum e_EnodebEnableReason
{
  ENABLE_REASON_USER_INITAITED_UNLOCK,
  ENABLE_REASON_S1AP_RECOVERY,
  ENABLE_REASON_COMPONENT_ALARM_CLEARED
} EnbEnableReason;

/*************************/
/********STRUCTURES*******/
/*************************/
typedef struct s_CreateAlarmDetails
{
  UINT32                   alarmSequenceNumber;
  AlarmNotificationType    alarmNotificationType;
  UINT8                    dateAndTimeStamp[MAX_SIZE_ACTIVEALARMSTATUS_ALARMDATEANDTIMESTAMP + 1 ];
  AlarmType                alarmType;
  UINT32                   alarmId;
  AlarmSeverity            alarmSeverity;
  AlarmCause               alarmCause;
  UINT8                    alarmLocation[MAX_SIZE_ACTIVEALARMSTATUS_ALARMLOCATION + 1 ];
  UINT8                    alarmAdditionalText[MAX_SIZE_ACTIVEALARMSTATUS_ALARMADDITIONALTEXT + 1 ];
} CreateAlarmDetails;

/* Alarm related structures */

typedef List ActiveAlarmStatusTable;

typedef struct s_AlarmHistoryStatusTable
{
  AlarmHistory   alarmHistoryEntry[MAX_ROWS_IN_ALARMHISTORY_TABLE];
  TBOOL          isTableFull;
  SINT32         rear;
  SINT32         front;
  UINT32         numOfElem;
} AlarmHistoryStatusTable;

typedef struct s_AlarmInstanceList
{
  UINT32   other;
  UINT32   comminucationsAlarm;
  UINT32   qualityServiceAlarm;
  UINT32   processingErrorAlarm;
  UINT32   equipmentAlarm;
  UINT32   environmentAlarm;
  UINT32   integrityViolation;
  UINT32   operationViolation;
  UINT32   physicalViolation;
  UINT32   securityServiceOrMechanismViolation;
  UINT32   timeDomainViolation;
} AlarmInstanceList;

typedef struct s_AlarmDbTable
{
  Index     numOfAlarms;
  AlarmDb   alarmDb[MAX_ROWS_IN_ALARMHISTORY_TABLE];
  /* Total number of entries in context = Total number of entries in Alarm History */
} AlarmDbTable;

typedef struct s_AlarmContext
{
  FilePath                  alarmLogFilePathAndName;
  FILE                      *fp;
  UINT32                    bytesWrittenToAlarmLog;
  UINT32                    maxFileSizeOfAlarmLog;
  AlarmDbTable              alarmDbTable;
  CreateAlarmDetails        createAlarmDetails;
  AlarmInstanceList         alarmInstanceList;
  UINT32                    alarmSequenceNumber;
  UINT32                    notificationSequenceNumber;
  ActiveAlarmStatusTable    activeAlarmStatusTable;   /* Active alarm - Linked List Implementation */
  AlarmHistoryStatusTable   alarmHistoryStatusTable;  /* History - Circular Buffer Implementation*/
} AlarmContext;

typedef ModTimersInfo BsmTimersInfo;

/* These flags are set and reset during EnodeB configuration procedure */
typedef struct s_CfgRspFlags
{
  TBOOL   uehConfigRspRcvd;
  TBOOL   s1apConfigRspRcvd;
  TBOOL   deRhConfigRspRcvd;
  TBOOL   pmhConfigRspRcvd;
  TBOOL   udhConfigRspRcvd;
  TBOOL   pmhCtrlInputConfigRspRcvd;
} CfgRspFlags;

typedef struct s_InitCfgRspFlags
{
  TBOOL   sihConfigRspRcvd;
  TBOOL   sctpConfigRspRcvd;
  TBOOL   s1apConfigRspRcvd;
  TBOOL   egtpConfigRspRcvd;
  TBOOL   dehConfigRspRcvd;
  TBOOL   lthConfigRspRcvd;
} InitCfgRspFlags;

#define NUM_MODULES_OPSTART_STOP_SENT  5
typedef struct s_OpStartOpStopInfo
{
  TBOOL      opStopCmpl;
  TBOOL      opStartCmpl;
  UINT8      opStartSeqIndex;      /* Index to be updated for each opStart sent */
  UINT8      numModulesToSendOpStop;       /* Index to be updated for each opStart sent */
  ModuleId   sequence[NUM_MODULES_OPSTART_STOP_SENT];
} OpStartOpStopInfo;

typedef struct s_EnbEnableDisableParams
{
  TBOOL     isS1SetupSuccessful;   /* Updated when OPSTART/OPSTOP is received from S1AP */
  TBOOL     isS1Tx;                /* Updated when OPSTART/OPSTOP is received from UEH */
  TBOOL     isRfTx;                /* Updated when OPSTART/OPSTOP is received from DEH */
  TBOOL     isPmhFunctional;       /* Updated when OPSTART/OPSTOP is received from PMH */
  TBOOL     isUdhFunctional;       /* Updated when OPSTART/OPSTOP is received from UDH */
} EnbEnableDisableParams;


typedef LedStatus LedInfoTable;

typedef struct s_BsmOamTableInfo
{
  OamTableType   oamTableType;
  OamColInfo     colInfo[MAX_ELEMENTS_IN_TABLE];
} BsmOamTableInfo;

typedef struct s_BsmOamTableContext
{
  UINT32           noOfvalidTables;
  BsmOamTableInfo  oprCmdTableInfo[MAX_NUM_OPR_CMD_TABLES];
  BsmOamTableInfo  oprCfgTableInfo[MAX_NUM_OPR_CFG_TABLES];
  BsmOamTableInfo  statusTableInfo[MAX_NUM_STATUS_TABLES];
  BsmOamTableInfo  preCfgTableInfo[MAX_NUM_PRE_CFG_TABLES];
} BsmOamTableContext;

typedef struct s_BsmContext
{
  /* General Information */
  VersionInfo            versionInfo;        /* This information will be filled from the version.cfg file */
  time_t                 moduleStartTime;    /* Time when first time BSM was started */
  UINT16                 configTransId;      /* Transaction ID for enodeB configuration */
  FilePath               rootDir;
  FilePath               configDir;
  FilePath               traceDir;
  FileName               localFilename;
  BsmProceduralState     bsmProceduralState;
  /* Used for status tables */
  TBOOL                  isModStateReqSentToUeh;
  TBOOL                  isModStateReqSentToUdh;

  /* OAM Related Context in BSM */
  BsmOamTableContext     bsmOamTableContext;

  /* Date and time information */
  DateAndTimeSource      dateAndTimeSource;   /* This is needed for updating EnodeB Status */
  UINT8                  currTimeOffsetFromUTC[MAX_SIZE_ENBSTATUS_LOCALTIMEOFFSETFROMUTC + 1 ];
  time_t                 moduleUpTimeDifference;/* This is needed for calculating uptime in case of dateandtime source changes*/

  /* Timers Related Context in BSM */
  UINT8                  totalTimers;
  BsmTimersInfo          bsmTimersInfo; /* This information will be filled from timers config file */

  /* BSM Initialisation flags */
  TBOOL                  areAllModulesUp;        /* Flag to check if all modules are UP or not */
  TBOOL                  isInitConfigSuccessful; /* Flag to check if initial configuration is succeeded */
  TBOOL                  emsRegSucc;             /* Flag to check EMS registration is successful or not */
  TBOOL                  isRhEnabled;            /* Flag updated when device status is received from DEH */

  /* S1AP related information in BSM */
  TBOOL                  isMmeLinkEstablished;  /* Updated when LINK UP / LINK DOWN is received from S1AP */
  UINT32                 numOfRetyForMmeLinkEstablish;
  UINT8                  maxMmesToConnectWith;  /* This is a MME count with which BSM will be expecting link up/down indication */
  UINT8                  numOfMmesConnected;    /* Num of MME's currently connected with enodeB */

  /* Configuration flags */
  TBOOL                  isConfigSuccessful;
  InitCfgRspFlags        initCfgRspFlags;
  CfgRspFlags            cfgRspFlags;           /* Updated when CONFIG_SUCC/FAIL is received */
  TBOOL                  isCfgRspWithWrongTransIdRcvd;

  /* Enb enable / disable information */
  OpStartOpStopInfo      opStartOpStopInfo;     /* OPstart and Opstop Info */
  EnbEnableDisableParams enbEnableDisableParams;
  EnbEnableReason        enbEnableReason;
  EnbDisableReason       enbDisableReason;

  /* Alarm Context */
  AlarmContext           afhCtx;

  /* Information of LED at BSM - received from DEH. */
  LedInfoTable           ledInfoTable[LEDID_MAX];
  CellCommand            cellCmdRcvd;
  TBOOL                  isAutoUnlockTried;
} BsmContext;

/**** All Globals in BSM module ***/
extern BsmContext g_BsmContext;

#define getBsmContext()              &(g_BsmContext)
#define getBsmTimersInfo()           &(g_BsmContext.bsmTimersInfo)
#define getConfigTransId()            (g_BsmContext.configTransId)
#define getBsmOamTableContext()      &(g_BsmContext.bsmOamTableContext)

/**** End of Globals in BSM module ***/

/**** BSM state Machine ***/
typedef BsmProceduralState  (*bsmMsgHandlerFp)(IN Msg *msg, IO BsmContext *bsmContext);

typedef struct s_BsmStateMachine
{
  bsmMsgHandlerFp fp;
} BsmStateMachine;

/*** Macro to SET/GET on BS enodeb operational, admin and BSM state ***/
void setBsmProceduralState(
    IO BsmContext          *ctx,
    IN BsmProceduralState  state
    );
#define getBsmProceduralState( context )            (context)->bsmProceduralState
#define getENodeBOperationalState( context )        (context)->eNodeBOperationalState
#define getENodeBAdminState( context )              (context)->eNodeBAdminState

void setConnectedToMMEStateInEnbStatusTable(
    IN ConnectedToMME        state
    );
void setAdminStateInEnbStatusTable(
    IN AdminState        state
    );
void setOperationalStateInEnbStatusTable(
  IN OperationalState   state
  );
void bsmSetIsOperationalInCellStatusTable(
  IN TBOOL   isOperational
  );
void setEnbEnableDisableParams(
    IN MsgId       msgId,
    IN TBOOL       value,
    IO BsmContext  *bsmContext
    );
TBOOL isEnodeBLocked();
TBOOL isEnodeBEnabled();
void getAndfillEnodebIpAddressFromEnodebStatus(
    IO UINT8   *enodebIpAddr
    );
TBOOL bsmSetDateAndTimeOfSystem(
    UINT8    *dateAndTime
    );
void getAndSendTableData(
    IO GetObjReq   *getReq
    );

/******    Init, Timer and other common functions ******/

void initBsmContext(
    IO BsmContext *ctx
    );

void initBsmStateMachine();

void startBsmProcTimer(
    IN TimerName         timerType,
    IO BsmTimersInfo     *timerInfo
    );
void  stopBsmProcedureTimer(
    IO TimerName   type
    );
void readBsmBsTimersCfgTable(IN UINT8 *configFilePath);

void bsmCheckAllModulesAreUp();

TBOOL bsmLoadCfgFromFiles(
    IN UINT8    *configFilePath
    );
TBOOL bsmWriteRunningConfigToNextStartConfig(
    IN UINT8    *configFilePath
    );

void bsmMain(Msg* msg);

TBOOL bsmReadEnbUserRunningConfigValues(
    IN UINT8    *configFilePath,
    IN UINT8    *fileName
    );

void initEnodeBStatusTable();
void initRabHistoryStatusTable();
void initActiveRabStatusTable();
void initUeEventHistoryTable();
void initActiveRabStatusTable();
void initActiveUeStatusTable();
void initModuleLogStatusTable();
void initLoginStatusTable();
void initIndexForCmdTables();
void initCellStatusTable();
void getNumOfMaxMmesToConnectWith(
    IO BsmContext *ctx
    );
void initLedColorInfo(
    IO BsmContext *bsmContext
    );

char *getBsmProceduralStateName(IN BsmProceduralState state);

void doCollectInfoProcedure(
    IO BsmContext  *bsmContext
    );
void doDeviceInitProcedure(
    IO BsmContext  *bsmContext
    );
TBOOL bsmParseOamCfg(
    IN UINT8*              configFilePath,
    IO BsmOamTableContext *bsmOamCtx
    );
MsgId getMsgId( 
    IN ModuleId modId 
    ); 
/********** Initial configuration procedure ******************/
BsmProceduralState bsmProcInitConfigSucc(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcInitConfigFail(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcInitConfigSuccFromSih(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcInitConfigFailFromSih(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
TBOOL checkForInitConfigProcedureCompletion(
    IO BsmContext  *bsmContext
    );
BsmProceduralState bsmProcDeviceInitSucc(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcDeviceInitFail(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcSetIpAddrRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
void fillAndSendSihInitCfgReqMsg();
void bsmCreateAndSendSetIpAddrReq();
/*************** Fill and send Object functions **********/
MsgId bsmCihSetObjSuccMsgId(
    IN  InstanceId instanceId
    );
MsgId bsmCihSetObjFailMsgId(
    IN InstanceId instanceId
    );
MsgId bsmCihGetObjSuccMsgId(
    IN  InstanceId instanceId
    );
MsgId bsmCihGetObjFailMsgId(
    IN InstanceId instanceId
    );

void bsmFillAndSendSetObjSucc(
    IN SetObjReq   *setObj
    );
void bsmFillAndSendSetObjFail(
    IN SetObjReq     *setReq,
    IN BsErrorValue   bsErrorValue,
    IN TagInfo       *errTagInfo
    );
void bsmFillAndSendGetObjFail(
    IN GetObjReq          *getReq,
    IN BsErrorValue        bsErrorValue,
    IN TagInfo            *errTagInfo
    );
void bsmCreateAndSendSetObjFail(
    IO SetObjRspFail   *setObjFail
    );

/*************************   SET/GET request/Response  *********************************/
BsmOamTableInfo* bsmGetTablePtrByTableId(
    IN TableId      tableId
    );
BsmProceduralState bsmProcGetObjReqInAllStates(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcSetObjReq(
    IN Msg *msg,
    IO BsmContext *bsmContext
    );
BsmProceduralState bsmProcSetObjReqWhenBusy(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcSetRspSucc(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcSetRspFail(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcGetRspSucc(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcGetRspFail(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcUehActiveUeStatusRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcUdhActiveRabStatusRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcUehUeEventHistoryRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcUdhRabHistoryRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcUdhModStateRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcUehModStateRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcGetEnbAddrRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcGetEnbAddrFail(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcUeEventHistoryLogConfigSuccFromUeh(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  );
BsmProceduralState bsmProcUeEventHistoryLogConfigFailFromUeh(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  );


/*******************  Device status ***************************************/
inline BsmProceduralState  bsmProcDeviceDownInd(
    IN Msg         *msg,
    IO BsmContext  *bsmContext
    );
inline BsmProceduralState  bsmProcComponentUpInd(
    IN Msg         *msg,
    IO BsmContext  *bsmContext
    );
inline BsmProceduralState  bsmProcComponentDownInd(
    IN Msg         *msg,
    IO BsmContext  *bsmContext
    );

/*************************   BS Hello Procedure Functions *********************************/
inline BsmProceduralState bsmProcHelloInAllStates(
    IN Msg *msg,
    IO BsmContext *bsmContext
    );

/************** Feature Software Install **************/
inline BsmProceduralState  bsmProcSwInstallSuccInAllStates(IN Msg *msg, IO BsmContext *bsmContext);
inline BsmProceduralState  bsmProcSwInstallFailInAllStates(IN Msg *msg, IO BsmContext *bsmContext);
/************** Feature Configuration Install **************/
inline BsmProceduralState bsmProcConfigInstallReq( IN Msg *msg, IO BsmContext *bsmContext );

/************** Feature Software Download **************/
inline BsmProceduralState  bsmProcSwDownloadCmplInAllstates(IN Msg *msg, IO BsmContext *bsmContext);
inline BsmProceduralState  bsmProcSwDownloadFailInAllStates(IN Msg *msg, IO BsmContext *bsmContext);

/************** Feature Software Upload **************/
inline BsmProceduralState bsmProcSwUploadCmplInAllstates(IN Msg *msg, IO BsmContext *bsmContext);
inline BsmProceduralState bsmProcSwUploadFailInAllStates(IN Msg *msg, IO BsmContext *bsmContext);

/*************** Feature Exit Procedure ****************/
inline BsmProceduralState bsmProcExitReqInAllStates(IN Msg *msg, IO BsmContext *bsmContext);

/*************** Cell barring Unbarring feature ********/
inline BsmProceduralState bsmProcCellBarSuccInAllStates( IN Msg *msg, IO BsmContext *bsmContext);
inline BsmProceduralState bsmProcCellUnBarSuccInAllStates( IN Msg *msg, IO BsmContext *bsmContext);

/*************** Cell Admin ********/
void bsmPerformOperationOnCellAdminTable(
    IO SetObjReq   *setObj,
    IO BsmContext  *bsmContext
    );
/******************************** BSM MOD Up/Down Procedure ***************************/
BsmProceduralState bsmProcAllModulesUpInd(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcModUpInd(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcModDownInd(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcModDownIndInInitComplState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcModDownIndInInitStates(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcS1apLinkClosedInd(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcS1apLinkDownInd(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcS1apLinkUpInd(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcS1apResetInd(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcUehUnknownEnbUeId(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );

/***************** Timer expiry functions***********/
inline BsmProceduralState bsmProcTimerExpiryInWaitDeviceInitCmpl(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    );
inline BsmProceduralState bsmProcTimerExpiryInWaitInitialConfigCmpl(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    );
inline BsmProceduralState bsmProcTimerExpiryInWaitCollectInfoCmpl(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    );
inline BsmProceduralState bsmProcTimerExpiryInWaitRegistrationCmpl(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    );
inline BsmProceduralState bsmProcTimerExpiryInIdle(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    );
inline BsmProceduralState bsmProcTimerExpiryInWaitConfigCmpl(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    );
inline BsmProceduralState bsmProcTimerExpiryInWaitEnableCmpl(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    );
inline BsmProceduralState bsmProcTimerExpiryInWaitDisableCmpl(
    IN Msg           *msg,
    IO BsmContext    *bsmContext
    );

/*************************** BSM Unlock Procedure **********************/
void bsmDoUnLock(
    IN SetObjReq   *setObj,
    IO BsmContext  *bsmContext
    );
BsmProceduralState bsmProcS1apOpStartSuccInWaitEnableState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcPmhOpStartSuccInWaitEnableState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcS1apOpStartFailInWaitEnableState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcUehOpStartFailInWaitEnableState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcDehOpStartSuccInWaitEnableState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcUdhOpStartSuccInWaitEnableState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcDehOpStartFailInWaitEnableState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcPmhOpStartFailInWaitEnableState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcUdhOpStartFailInWaitEnableState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcUehOpStartSuccInWaitEnableState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
/******************************** BSM Lock Procedure ***************************/
BsmProceduralState bsmProcS1apOpStopSuccInWaitDisableState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcUehOpStopSuccInWaitDisableState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcDehOpStopSuccInWaitDisableState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcOpStopSuccInWaitDisableState(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );

/****************************** EnodeB Configuration Procedure ***********************/
void createConfigurationFailedAlarm(
    IN ModuleId    failedModuleId,
    IN UINT8	 *addText
    );
void clearConfigurationFailedAlarm(
    IN MsgId       clearingMsgId,
    IN ModuleId    moduleId,
    IO BsmContext  *bsmContext
    );
void doInitConfigProcedure(
    IO BsmContext  *bsmContext
    );
void bsmFillAndSendUehEnbCfgReqMsg();
void bsmFillAndSendSctpCfgReqMsg();
void bsmFillAndSendS1apCfgReqMsg();
void bsmFillAndSendEgtpCfgReqMsg();
void fillDehRhCfgReqMsg(
    IO MsgBsmDehRhConfigReq  *configReq
    );
void fillLthInitCfgReqMsg(
    IO MsgBsmLthInitConfigReq  *configReq
    );
void bsmCalculateRefSigPower(
     IO PhyConfig  *phyConfig
     );
void bsmFillAndSendDehRhCfgReqMsg();
void bsmFillAndSendPmhCfgReqMsg();
void bsmFillAndSendPmhCntrInputCfgReqMsg();
void bsmFillAndSendUdhCfgReqMsg();
void initAlarmHistoryStatusTable(AlarmHistoryStatusTable *circularBuf);
BsmProceduralState bsmProcEnbConfigSucc(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcEnbConfigFail(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
TBOOL checkForConfigProcedureCompletion(
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcPmhConfigSucc(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcPmhConfigFail(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );

/********************* Reboot / Shutdown Procedure *****************/
void writeDateAndTimeInNextStartConfig(
    IN UINT8  *filePath
    );
void bsmDoReboot(
    IN SetObjReq   *setObj,
    IO BsmContext  *bsmContext
    );
void bsmDoRestart(
    IN SetObjReq   *setObj,
    IO BsmContext  *bsmContext
    );
/******************** EnodeB Lock/Unlock/Enable/Disable Procedure ***************/
void enbDisable(
    IN TBOOL        doDisableProcedure,
    IO BsmContext  *bsmContext
    );
void enbEnable(
    IO BsmContext  *bsmContext
    );
void bsmDoLock(
    IN SetObjReq   *setObj,
    IO BsmContext  *bsmContext
    );
void bsmDoUnLock(
    IN SetObjReq   *setObj,
    IO BsmContext  *bsmContext
    );
TBOOL sendOpStopReqMsg(
    IO OpStartOpStopInfo *info
    );
void doDisableProcInitStepsWhenEnableIsInterrupted(
    IO BsmContext       *bsmContext
    );
void doEnableProcedureInterreptedSteps(
    IO BsmContext       *bsmContext
    );
/*****************  Configuration Save/Roll Back Procedure ***************************/
void bsmDoSaveConfig(
    IN SetObjReq   *setObj,
    IO BsmContext  *bsmContext
    );
void bsmDoRollBackConfig(
    IN SetObjReq   *setObj,
    IO BsmContext  *bsmContext
    );
TBOOL bsmWriteEnbUserRunningConfigToPersistentMemory(
    void );
/****************  Software Install Procedure ******************/
void bsmDoSwInstall(
    IO SetObjReq   *setObj,
    IO SwInstall   *swInstall
    );
TBOOL isConfigInstall(
    IN FileName   filename
    );
void revertBackEnodeBIdAndRegAcceptedInConfigTable(
    IN UINT32         enodeBId,
    IN TBOOL          enbRegAccepted,
    IO EnodebConfig   *enodebConfig
    );
/******************* OAM table related functions ***************/
void bsmPerformOperationOnEnbAdmin(
    IN Msg         *msg,
    IO SetObjReq   *setObj,
    IO BsmContext  *bsmContext
    );
void bsmPerformOperationOnSwActivation(
    IO SetObjReq   *setObj,
    IO BsmContext  *bsmContext
    );
void bsmPerformOperationOnSwInstall(
    IO SetObjReq   *setObj
    );
void bsmPerformOperationOnCounterControl(
  IO SetObjReq   *setObj
  );
TBOOL bsmPerformOperationOnMsgTrace(
    IN SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnEventThresholds(
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnEnbAddresses(
    IO SetObjReq   *setObj
    );
BsmProceduralState bsmProcCounterClearSucc(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcCounterClearFail(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcDiagCounterCmdCmpl(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );

TBOOL isMmeIpAddressConfigured();
TBOOL isPlmnConfigured();

/** Operator Configuration table **/
TBOOL bsmPerformOperationOnEnbRegistrationConfig(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnEnbConfig(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnCellConfig(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnSysInfo1Config(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnSysInfo2Config(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnSysInfo3Config(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnSchedulerConfig(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnPhyConfig(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnQosConfig(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnEutraHoProfileConfig(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnEutraNeighCellConfig(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnPmConfig(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnLogConfig(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnMmeConfig(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnSonConfig(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnSrsConfigDedicated(
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
TBOOL bsmPerformOperationOnSecurityConfig(                                                                               
    IN RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
void bsmSendUeEventHistoryLogConfigToUeh(void);
TBOOL bsmPerformOperationOnUeEventHistoryLogConfig(                                                                               
    IO RowId        rowIdToUpdate,
    IO SetObjReq   *setObj
    );
/* Function prototypes of fns for sending initial log configurations to all modules */
void bsmInitSetLogLevelProcedure ();
/** Consistency Check functions **/
TBOOL doConsistencyCheckOnEutraNeighCellConfig(
    IN EutraNeighCellConfig  *eutraNeighCellConfig
    );
TBOOL bsmConsistencyCheckOnCellConfig(
    IO TlvArray    *tlvArray,
    IO TlvInfo     *failedTlvInfo
    );

TBOOL bsmMimoFeatureCheckOnCellConfig(
  IO TlvArray    *tlvArray,
  IO TlvInfo     *failedTlvInfo
  );

TBOOL bsmConsistencyCheckOnEutraNeighCellConfig(
    IO TlvArray    *tlvArray,
    IO TlvInfo     *failedTlvInfo,
    IO UINT8       *errInfoStr
    );
TBOOL bsmConsistencyCheckOnPhyConfig(
    IO TlvArray    *tlvArray,
    IO TlvInfo     *failedTlvInfo,
    IO BsErrorValue  *bsError
    );
TBOOL bsmConsistencyCheckOnSysInfo2Config(
    IO TlvArray    *tlvArray,
    IO TlvInfo     *failedTlvInfo
    );

TBOOL doConsistencyCheckOnCellConfigTable(
    IN CellConfig  *cellConfig
    );
TBOOL doConsistencyCheckOnPhyConfigTable(
    IN PhyConfig  *phyConfig
    );
TBOOL doConsistencyCheckOnSysInfo2ConfigTable(
    IN SysInfo2Config  *sysInfo2Config
    );
TBOOL doConsistencyCheck();
/** Software Activation functions */
void bsmConvertVersionStrToStruct(
    IO UINT8                    *versionInfo,
    IO MsgBsmPrmSwActivateReq   *req
    );
BsmProceduralState bsmProcSwActivateSucc(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcSwActivateFail(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcSwActivateRej(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcSwActivateRsp(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcEnbRebootRsp(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  );

/** Alarm related functions */
void getAndFillDateAndTime(
    UINT8   *dateAndTime
    );
void alarmInitCtx(
    IO AlarmContext *alarmCtx
    );
AlarmContext             *getAlarmContext( void );
ActiveAlarmStatusTable   *getActiveAlarmStatusTable( void );
AlarmHistoryStatusTable  *getAlarmHistoryStatusTable( void );

TBOOL checkIfActiveAlarmExists(
  IO CreateAlarmDetails  *alarmDetails
  );

void doSaveAlarm(
    IO CreateAlarmDetails    *eventMessage
    );
void doClearAlarm(
    IO CreateAlarmDetails  *alarmDetails
    );
void doClearAlarmsUponLock();
UINT32 getNumActiveCriticalAlarms();
UINT32 getNumActiveMajorAlarms();
UINT32 getNumS1apActiveLinkDownAlarms();
UINT32 getNumModDownAlarms();

TBOOL getAlarmDetailsByBsErrorCode(
    IN BsErrorValue        bsError,
    IO CreateAlarmDetails *alarmDetails
    );

TBOOL getAlarmDetailsByClearingMsgId(
    IN MsgId              clearingMsgId,
    IN Index              alarmIndex,
    IO CreateAlarmDetails *alarmDetails
    );

void clearS1apAlarms(
    IN MsgId        msgId,
    IN UINT8        *ipAddr,
    IO BsmContext   *bsmContext
    );
void createConfigurationErrorAlarm(
    IN ModuleId   moduleId
    );
void createEmsRegFailAlarm();
void clearEmsRegFailedAlarm();

void createEmsRegFailAlarm();
void clearEmsRegFailedAlarm();

/******** Event related functions *********/
UINT32 getEnodeBId();
UINT32  getNotificationSequenceNumber( void );
UINT32  getNextNotificationSequenceNumber( void );

void postActiveAlarmsToSihIfAny();

void sendEventMmeReset(
    IN UINT8     *mmeAddr
    );
void sendEventFileUploadCompleted(
    IN UINT8         *fileName
    );
void sendEventFileUploadFailed(
    IN UINT8         *fileName,
    IN UINT8         *errorInfo
    );
void sendEventFileDownloadCompleted(
    IN UINT8         *fileName
    );
void sendEventFileDownloadFailed(
    IN UINT8         *fileName,
    IN UINT8         *errorInfo
    );
void sendEventCounterFileAvailable(
    IN UINT8         *fileName
    );
void sendEventCounterCollectionFailed(
    IN UINT8   *errorInfo
    );
void sendEventMeasurementCompleteInd(
  IO MsgPmhBsmMeasurementCompleteInd   *ind
  );
void sendEventConfigurationChanged(
    IN OamIfType      oamIf
    );
void sendEventConfigurationSaved(
    IN OamIfType      oamIf
    );
void sendEventConfigurationRollBack(
    IN OamIfType      oamIf
    );
void sendEventEnbReboot(
  IN UINT8*     reason
  );
void sendEventSwActivationFailed(
    IN SwType     swType,
    IN UINT8*     version,
    IN UINT8*     errorInfo
    );
void sendEventSwActivationSuccess(
    IN SwType     swType,
    IN UINT8*     version
    );
void sendEventSwInstallFailed(
    IN UINT8*     fileName,
    IN UINT8*     errorInfo
    );
void sendEventSwInstallSuccess(
    IN UINT8*     fileName,
    IN UINT8*     version
    );
void sendEventEnbRegistrationReq(void);

void sendEventEnbOperationalStateChanged(
    IN OperationalState    operationalState
    );
void sendEventEnbAdminStateChanged(
    IN AdminState          adminState
    );
void sendEventEnbLocationChanged();
void sendEventEnbNoNeighborConfigured();
void sendEventCellBarredInd();

/************ Collect Information Procedure **********/
void updateLocationStatusTable(
    IN LocationSource  locationSource,
    IN UINT8           *latitude,
    IN UINT8           *longitude
    );
void sendGpsDetailReqMsg();
BsmProceduralState bsmProcGpsDetailSucc(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcGpsDetailFail(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
void doCollectionInfoProcedureFailureSteps(
    IN UINT8            *errString,
    IO BsmContext       *bsmContext
    );

/*********** EMS Registration Feature **************/
void doEmsRegistration();

char *getMonthInString(
    UINT8   monthInNum
    );

/*********** Collection Counter Feature **************/
BsmProceduralState bsmProcPmhCounterFileAvailable(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
BsmProceduralState bsmProcPmhCounterCollectionFailed(
    IN Msg              *msg,
    IO BsmContext       *bsmContext
    );
/************ LED colour setting *******************/
void bsmCreateAndSendSetLedColor(
    IN  LedId       ledNumber,
    IN  LedColor    ledState,
    IO  BsmContext  *bsmContext
    );
/************ BIST/POST Feature *******************/

void bsmDoLongPost(
    IN Msg         *msg,
    IN SetObjReq   *setObj,
    IO BsmContext  *bsmContext
    );

void bsmDoBist(
    IN Msg         *msg,
    IN SetObjReq   *setObj,
    IO BsmContext  *bsmContext
    );

void bsmUpdateCellBarUnbarInfo(
    IN UINT8   *barUnbarInfo
    );
void bsmUpdateCellOperationalStateInfo(
    IN UINT8   *operationalStateInfo
    );

BsmProceduralState bsmProcMeasCmplInd(
  IN Msg              *msg,
  IO BsmContext       *bsmContext
  );

void bsmSetTxAttenInRhCfg(
     IN SINT8 txPower
     );
void bsmDeleteAlarmHistoryTable(
     IN SetObjReq *setObj
      );

#endif /* BSM_H */

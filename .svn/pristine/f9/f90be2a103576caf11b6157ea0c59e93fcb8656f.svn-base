/**
 * @file    prm.h
 * @brief Structures and prototypes for PRM
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author        : Shweta Polepally
 * Date          : July 2010
 * Change History:
 */

#ifndef PRM_H
#define PRM_H

#include "all_cmn.h"
#include "all_cfg.h"
#include "generic_table.h"
/* Interface .h files */
#include "bsm_prm_if.h"
#include "print_tlv.h"
#include "get_oam_func.h"
#include "prm_deh_if.h"

/***** MACROS ************/
#define getPrmContext()                 &(g_PrmContext);
#define getPrmTimersInfo()              &(g_PrmContext.prmTimersInfo)
#define prmGetOprCfgTableInfo(tableId)  &(g_PrmContext.oprCfgTableInfo[tableId])


/**************************/
/******* ENUMS ************/
/**************************/
/* PRM States */
typedef enum e_PrmState
{
  UNDEFINED           = 0,
  IDLE                = 1,
  WAIT_INIT_COMPLETE  = 2,
  WAIT_HELLO_ACK      = 3,
  WAIT_EXIT_COMPLETE  = 4,
  MAX_STATE           = 5
} PrmState;

#define EXIT_PHASE_LEVEL_1   1
#define EXIT_PHASE_LEVEL_2   2
#define EXIT_PHASE_LEVEL_MAX 2
typedef enum e_TimerStartReason
{
  REASON_NONE,
  REASON_INIT,
  REASON_HELLO_ACK,
  REASON_HEALTH_CHECK,       /* health Check */
  REASON_RESTART,
  REASON_REBOOT,
  REASON_SW_ACTIVATION,
  REASON_SW_ACTIVATION_FAILED,
  REASON_L2L3_PATCH_INSTALL,
  REASON_L2L3_PATCH_INSTALL_FAILED,
  REASON_RH_SW_INSTALL,
  REASON_RH_SW_ACTIVATION,
  REASON_MAX
} PrmTimersStartReason;

typedef enum e_prmRestartReason
{
	REASON_NORMAL,
	REASON_ACTIVATE,
	REASON_ACTIVATE_FAIL    /* Required for Fall Back */
} PrmRestartReason;

/**************************/
/******* Structures *******/
/**************************/
typedef struct s_PrevModuleStatusEntry
{
  ModuleId       moduleId;
  TBOOL          isRunning;
  RestartReason  restartReason;
  TBOOL          modDownReported;
} PrevModuleStatusEntry;

typedef struct s_PrevModuleStatusTable
{
  NumEntry                 numOfModules;
  PrevModuleStatusEntry    entry[MODID_MAX];
} PrevModuleStatusTable;

typedef struct s_ProcessModuleMappingEntry
{
  ModuleId     moduleName;
  UINT8        exeName[MAX_SIZE_PROCESSSTART_EXENAME + 1 ];
  UINT32       pid;
} ProcessModuleMappingEntry;

typedef struct s_ProcessModuleMappingTable
{
  NumEntry                   numOfMappedModules;
  ProcessModuleMappingEntry  entry[MODID_MAX];
} ProcessModuleMappingTable;

typedef ModTimersInfo PrmTimersInfo;

typedef struct s_ProcessUsageInfo
{
  SINT32       pid;
  time_t       lastHcCompletedAt;
  UINT32       lastUsageInSec;
} ProcessUsageInfo;

typedef struct s_SwPackageInfo
{
  FilePath      rootDir;        /* absolute path of inst_x directory */
  FilePath      binDir;         /* absolute path of inst_x/bin */
  FilePath      logDir;         /* absolute path of inst_x/log */
  FilePath      configDir;      /* absolute path of inst_x/config */
  VersionInfo   versionInfo;    /* sw version of this package */
} SwPackageInfo;

typedef struct s_PrmOamTableInfo
{
  TableName          tableName;
  UINT32             noOfColumns;
  UINT32             noOfRows;
  OamColInfo         colInfo[MAX_ELEMENTS_IN_TABLE];
}PrmOamTableInfo;

typedef struct s_PrmContext
{
  /* inst_A: bin, log, config */
  ProcessUsageInfo          processUsageInfo[MAX_ROWS_IN_PROCESSSTART_TABLE];
  FilePath                  cfgFilePath;
  FilePath                  picoBsHomePath;
  UINT8                     moduleVersionStr[MAX_SIZE_SWPACKAGESTATUS_VERSION];
  SwPackageInfo             prmInfo;
  SwPackageInfo             instA;          /* Installable A directory information */
  SwPackageInfo             instB;          /* Installable B directory information */
  SwPackageInfo            *isRunning;      /* Pointer to the running directory */
  SwPackageInfo            *isNotRunning;   /* Pointer to the non-running directory */
  PrmState                  prmState;        /* Possible states in PRM */
  ProcessModuleMappingTable mappingTable;    /* Process-Module mapping table */
  UINT8                     totalTimers;     /* This information will be filled from be timers config file */
  PrmTimersInfo             prmTimersInfo;   /* Timers information related to PRM */
  PrevModuleStatusTable     prevModuleStatusTable;
  TBOOL                     areAllModulesUp; /* This will be true when Process table gets changed, BSM will be notified. */
  UINT8                     numOfProcesses;  /* Total number of Processes PRM has to start */
  UINT8                     numOfModules;    /* Total number of Modules PRM will communicate with */
  UINT8                     numOfRspRcvd;    /* Number of modules responded - Health check/Init Message */
  UINT8                     helloSendCount;  /* Number of modules PRM has sent Hello Message */
  SwInstall                 swinstall;       /* Copy of SwInstall Message received at PRM. Used to send response later after receiving SUCC from BSM during configuration install */
  TBOOL                     isInstallInProgress; /* To check if installation is in progress */
  TBOOL                     isActivationInProgress; /* To check if activation is in progress */
  UINT32                    indexOfSwInstallHistory; /* index of SwInstallHistory table shows current installation */
  UINT8                     noOfEntriesInSwInstallHistory; /* no of entries in swInstallHistory table */
  PrmOamTableInfo           oprCfgTableInfo[MAX_NUM_OPR_CFG_TABLES];
  ModExitCause              moduleExitCause;
  UINT8                     exitPhaseLevel; /* If reached 2, means both phases are completed */
  TBOOL                     exitProcCmpl;
  UINT32                    helloSeqNum;
  FileName                  rhSwInstallBinFile;
  TBOOL                     isSwActivationPerformed; /* This flag tells PRM is started due to SW activation */
  TBOOL                     isSwActivationDueToFallBack; /* This flag tells SW activation started due to fallback*/
} PrmContext;

typedef MsgTrace MsgBsmPrmSetMsgTrace;

typedef void (*prmMsgHandlerFp)(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  );
typedef struct s_PrmStateMachine
{
  prmMsgHandlerFp  fp;
  PrmState         nextStates[2];
} PrmStateMachine;

/***** Global PRM Context **/
extern PrmContext     g_PrmContext;

/*****************************************/
/*************FUNCTION PROTOTYPES*********/
/*****************************************/

/********** Initialisation/Configuration read functions ********/

inline void prmSetState(
  IN PrmState    state,
  IO PrmContext *ctx
  );
void initPrmContext(
  IO PrmContext   *prmContext
  );
void initPrmStateMachine( void );
void prmPrintTimeStamp( void );
void prmInitModuleStatusTable( void );
void prmInitSwActivationStatusTable( void );
void prmInitSwInstallHistoryTable( void );
void prmMain( FR Msg *msgIn );
TBOOL prmReadProcessModuleMappingTable(void);
TBOOL prmReadModuleStopTable(void);
TBOOL prmReadProcessStartTable(void);
TBOOL readPrmTimersFromCfg(void);
void prmStoreTimersInCtx(
  IN UINT8         totalTimers,
  IN TimersConfig  *config
  );
void prmReadSwPackageStatusTable(
  IN UINT8   *configDir
  );
void writeSwInfoStatusTable(
  IN UINT8           *srcDir
  );
TBOOL prmParseOamOprCfgTables(
  IN UINT8*                 configFilePath,
  IO PrmOamTableInfo    *oamCtx
  );
TBOOL prmGetPicoBsHomePath( IO UINT8 *picoBsHomePath );
TBOOL prmInitTablesAndStartModules(
  IO   PrmTimersStartReason
  );
void prmStartProcesses(
  IN TBOOL       bootLowerArm,
  IO PrmContext *ctx
  );
char *getPrmStateName(
  IN PrmState state
  );

void prmCopyCfgToNonRunningPackage(
  IN PrmContext            *ctx
  );
/*********  Init And Exit Procedure *********/
void prmDoProcessInit(
  IO PrmTimersStartReason reason
  );
void prmDoFallBack(
  IO PrmContext  *prmContext
  );
void prmDoExitProcedure(
  IO PrmContext  *prmContext
  );
void prmCreateAndSendAllModulesUpInd(
  IO ModuleStatus     *modStatus
  );
void prmCreateAndSendModuleUpInd(
  IN BsErrorValue      bsError,
  IO ModuleStatus     *modStatus
  );
void prmCreateAndSendModuleDownInd(
  IN BsErrorValue        bsError,
  IO ModuleStatus        *modStatus
  );
void prmProcStartIndInIdle(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  );
void prmProcStartIndInWaitInitComplete(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  );
TBOOL prmCheckForInitProcedureCompletion(
  IO PrmContext    *prmContext
  );
void prmSendExitMsgToExitPhaseModules(
  IN UINT8         exitPhaseLevel,
  IO PrmContext   *prmContext
  );
void prmCheckForExitProcedureCompletion(
  IO PrmContext    *prmContext,
  IO ModuleStatus  *moduleStatus
  );
void prmWriteSwPackageStatusToPersistentMemory( void );
TBOOL checkForPackageToRunAndUpdatePrmInfo(
  IO PrmContext *ctx
  );
void prmUpdateSwPackageCfgFileWithVersionInfoAtInit(
  IN SwPackageInfo   *prmInfo,
  IO PrmContext      *prmContext
  );
void prmFormModuleVersionStringPart1(
  IN SwPackageInfo   *prmInfo,
  IO UINT8           *versionInfo
  );
void prmReplaceExeWithRunningOne(
  IO PrmContext  *ctx
  );

/************ GET on tables **************/
void prmProcGetObjReqInAllStates(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  );

/************Hello Check Procedure Function ************/
void prmDoHeartBeatCheck(
  IO PrmContext   *prmContext
  );
void prmProcHelloAckInWaitHelloAck(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  );
void prmProcHelloAckInIdle(
  IN Msg         *msg,
  IO PrmContext  *prmContext
  );
void prmCheckForHealthCheckProcedureCompletion(
  IO PrmContext    *prmContext
  );

/********** Restart Functions ******************/
void  prmProcRestartReqInIdle(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  );
void prmProcRestartReq(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  );

/*************** BS Reboot Functions **************/
void prmProcRebootReq(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  );

/*************** BS Sw Activation procedure ********/
void prmProcSwActivateReq(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  );
void prmWriteSwInfoTable(
  IO UINT8  *willRunNextInstXConfigDir
  );
TBOOL prmReadSwPackageVersion(
  IN UINT8 *srcDir,
  IO VersionInfo *versionInfo
  );
void prmUpdateSwActivationStatusTable(
  IN UINT8           *version,
  IN SwStatus         swStatus
  );
void prmUpdateSwInfoStatusTable(
  IN PrmContext            *ctx
  );
/*************** BS Sw Installation procedure ********/
void prmUpdateSwPackageStatusTableForEnbSw(
  IN PrmContext            *ctx
  );
void prmUpdateSwPackageStatusTableForRhSw(
  IN UINT8   *version
  );
void prmProcSwInstallReq(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  );
void prmProcSwConfigInstallSucc(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  );
void prmProcSwConfigInstallFail(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  );
void prmProcDeleteSwInstallHistoryTableReq(
  IN Msg         *msg,
  IO PrmContext  *prmContext
  );
void prmCopyPatchFilesToRunningPackage(
  IO PrmContext *prmContext
  );
void prmCopyBackupFilesToRunningPackage(
  IO PrmContext *prmContext
  );
void prmHandleL2L3PatchStartupFailed(
  IO PrmContext  *prmContext
  );
void prmHandleL2L3PatchStartupSucc(
  IO PrmContext  *prmContext
  );
void prmProcRhSwInstallSucc(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  );
void prmProcRhversionInfo(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  );
void prmProcRhSwInstallFail(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  );

TBOOL isPrmStartedDueToSwActivation(
  IO PrmContext  *prmContext
  );

/********* Timer related functions ***********/
void startPrmProcTimer(
  IN TimerName         timerType,
  IN PrmTimersStartReason  reason,
  IO PrmTimersInfo     *timerInfo
  );
void  stopPrmProcedureTimer(
  IO TimerName   type
  );
void prmProcTimerExpiryInIdle(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  );
void prmProcTimerExpiryInWaitHelloAck(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  );
void prmProcTimerExpiryInWaitInitComplete(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  );
void prmProcTimerExpiryInWaitExitComplete(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  );
void prmProcUpdateTimerReq(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  );

/* Message tracing */
void prmProcSetMsgTraceInAllStates(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  );

NumEntry prmReadFromFileMappingCfgTable(
  IN UINT8                        *fileToRead,
  IO ProcessModuleMappingEntry    *cfgTable,
  IO UINT8                        *error
  );
void prmProcRhSwActivateFail(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  );
void prmProcRhSwActivateSucc(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  );
void prmUpdateSwInstallStatusTable(
  IN UINT8               *fileName,
  IN TBOOL                status,
  IN UINT8               *errDetail
  );
void prmCreateAndSendSwInstallFail(
  IN BsErrorValue                error,
  IO PrmContext                 *prmContext
  );
void prmCreateAndSendSwActivateSucc(
  IN SwType          swType,
  IN UINT8          *version
  );
void prmCreateAndSendSwActivateFail(
  IN SwType          swType,
  IN UINT8          *version,
  IO ErrorDesc      *errDesc
  );
void prmCreateAndSendSetObjSucc(
  IN MsgBsmPrmSetObjReq *setObjReq
    );

char *prmGetRhNonRunningVersion();
/***PRM Initiate Physhutdown **/
void prmProcPhyshutdown(
);

#endif /* PRM_H */


/**
 * @file    pmh.h
 * @brief   API for Performance Manager (Abbrev: PMH)
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author  Ramprasad Reddy Chinta
 * @date    10-02-2011
 */

/**
 * PMH = Performance Management Handler
 */

#ifndef PMH_H
#define PMH_H

#include "ueh_pmh_if.h"
#include "all_cfg.h"
#include "counter_write_functions.h"
#include "bsm_pmh_if.h"
#include "fill_tlv_func.h"
#include "get_oam_func.h"
#include "print_tlv.h"
#include "bs_tables.h"
#include "start_indication.h"
#include <dirent.h>
#include <time.h>
#include "ctr_id.h"

/* Define macros to indicate timer indexes */
#define TIMER_INDEX_SAMPLE_INTERVAL                          0
#define TIMER_INDEX_PM_LOGFILE_CREATE_INTERVAL               1
#define TIMER_INDEX_WAIT_COUNTER_COLLECTION                  2
#define TIMER_INDEX_WAIT_COUNTER_RESET                       3
#define TIMER_INDEX_WAIT_L2PMCOUNTERCONTROLPROFILE1          4
#define TIMER_INDEX_WAIT_L2PMCOUNTERCONTROLPROFILE2          5
#define TIMER_INDEX_WAIT_L2PMREPORTPROFILE1                  6
#define TIMER_INDEX_WAIT_L2PMREPORTPROFILE2                  7

#define EXPECTED_RSP_FROM_MAC_COUNTERS  2

typedef struct s_CounterFlags
{
  /* 1 */  TBOOL macGenCounter;
  /* macUplaneCounter = TRUE only if, macRlcCounter & macPhyCounter are TRUE */
  /* 2 */  TBOOL macUplaneCounter;
  /* 2.a */ //TBOOL macRlcCounter;
  /* 2.b */// TBOOL macPhyCounter;
  /* 3 */  TBOOL rlcGenCounter;
  /* 4 */  TBOOL pdcpGenCounter;
  /* 5 */  TBOOL pdcpUplaneCounter;
  /* 6 */  TBOOL gtpGenCounter;
  /* 7 */  TBOOL sctpGenCounter;
  /* 8 */  TBOOL s1apGenCounter;
  /* 9 */  TBOOL s1apPeerNodeCounter;
  /* 10 */ TBOOL rrcGenCounter;
  /* 11 */ TBOOL rrcCellCounter;
  /* 12 */ TBOOL rrmCounter;
  /* 13 */ TBOOL ipCounter;
  /* 14 */ TBOOL ethernetCounter;
} CounterFlags;

/* State of PMH */
typedef enum e_PmhState
{
  PMH_WAIT_INIT_CFG,
  PMH_WAIT_OPSTART,
  PMH_IDLE,               /* PMH enters this state during the start of the module*/
  PMH_WAIT_BULK_COUNTER_COLLECTION,
  PMH_MAX_STATE
} PmhState;

typedef List PmLogFileStatusTable; /* Pm log file is a list */

typedef struct s_EthernetHwInfoTable
{
  TBOOL  isHwInfoAlreadyRead;  /* TRUE: Already read before FALSE: To read in next get on Ethernet->hwinfo table */
  UINT8  interfaceName[IFNAMSIZ];
} EthernetHwInfoTable;

/** Mapping of CmdRef <-> TransId
 * Need:
 * cih/wih/sih --(Req (cmdRef, ...))---> PMH ---SzMngmt( transId, ...)--> CCPU_MODULE
 * At PMH:
 * 1. cmdRef needs to be mapped to transId while sending request to CCPU Module.
 * 2. transId needs to be mapping to cmdRef on reception of response from CCPU MODULE.
 *
 * We use an array instead of a hash or list for the following reasons:
 * 1. This mapping is only required till the procedure is ongoing.
 *    On completion of the procedure, the mapping is deleted.
 * 2. In general, it can be safely assumed that only a few (< 10)
 *    simultaneous procedures can be active between PMH and CCPU MODULES at a time.
 *    For this size, a linear array search vs linked list offer the same performance.
 *    Linked list is not adopted to avoid dynamic memory allocation.
 *
 * Mapping: transId2CmdRefTable
 * @param arrayIndex transId 0 .. (MAX_PARALLEL_PMH_OAM_GET_PROCS -1)
 * @param contains   cmdRef corresponding to that transId
 * At init:
 * 1. set the array to be empty: all elements.isUsed = FALSE
 * When a GET REQ is received by PMH, calls allocTransId()
 * where: ErrInfo allocTransId(IN cmdRef, IO transId);
 * allocTransId():
 *   It will search the array for an element with isUsed == FALSE.
 *   It will set element.cmdRef = passed cmdRef
 *   It will set element.isUsed = TRUE
 *   It will return the index of that element and errorCode.
 *
 *   This index will be the transId.
 *
 * PMH will include the trandId in messages to CCPU MODULES
 * Whenever a new GET REQ is received, the same procedure will be called.
 * Hence, parallel procedures will have different transIds (may not be sequential).
 *
 * When PMH receives response from a CCPU Module, it will:
 * a) Search for cmdRef corresponding to the transId:
 *    ErrInfo  getCmdRefByTransId(IN trandId, IO cmdRef)
 *    ErrInfo  DeallocTransId(IN trandId)
 *    This function will
 *    - search transId: just index the array (so long as the index is valid),
 *    - put the corresponding cmdRef as output
 *    - deallocate the transId i.e mark isUsed = FALSE
 */

#define MAX_PARALLEL_PMH_OAM_GET_PROCS 10

typedef struct s_CmdRefOpt {
  TBOOL    isUsed;
  CmdRef   cmdRef;
  TagInfo  tagInfo;
} CmdRefOpt;

typedef struct s_CounterProfileConfig {
  ProfileId            profileId;               /* Profile ID*/
  TBOOL                isProfileRunning;        /* Is profile Running*/
  UINT32               numOfSamplesRcvd;        /* Number of sample received*/
  UINT32               numOfSamplesRemaining;   /* Number of samples remaining */
  UINT32               totalNumOfSamples;       /* total number of samples */
  UINT32               numOfSamplesRemainingOfReportingPeriod;  /* Number of samples remaining in Reporting period */
  UINT32               totalSamplePerReportingPeriod;           /* Number of samples per Reporting period */
  ReportingDuration    reportingDuration;       /* Reporting Duration */
  MeasurementDuration  measurementDuration;     /* Measurement Duration */
  FILE                 *fp;                     /* Opened PM log file pointer */
  UINT8                currPmLogFileName[MAX_SIZE_PMLOGFILESTATUS_FILENAME+1];  /* Current opened PM log file */
  UINT8                currPmLogFileOpenTimeStamp[MAX_SIZE_PMLOGFILESTATUS_OPENEDAT+1]; /* Opened TimeStamp for PM file */
  UINT32               lastSeqNum;              /* sequence number received in last report */
  /* Note: Collection stop reason will be present in Counter Collection status table */
} CounterProfileContext;

typedef struct s_CounterContext {
  UINT32          value;
  UINT8           lastCollectedAt[MAX_SIZE_L2COUNTERS_LASTCOLLECTEDAT + 1 ];
  CounterConfig   l2CounterConfig;
} CounterContext;

typedef struct s_L2CounterCtx {
  UINT32          numOfRows;
  L2Counters      l2Counters[MAX_ROWS_IN_L2COUNTERS_TABLE];
} L2CounterCtx;

/* PMH CONTECXT*********************************/
typedef struct s_PmhContext
{
  /* PMH Internal procedural state */
  PmhState              pmhState;
  CounterProfileContext counterProfileContext[MAX_NUMBER_OF_PROFILE];
  CounterContext        counterContext[MAX_ROWS_IN_COUNTERCONFIG_TABLE];

  /* This is defined to avoid the stack usage while PMH write the report to PM log file */
  L2CounterCtx          l2CounterCtx;

  /* Copy of ConfigReq message and Control Input Config Request Message */
  PmCounterLogConfig    pmCounterLogConfig;
  UINT32                peerId;
  UINT8                 cellId;

  /* Store row number to update in pmLogFile Status table */
  UINT8                 pmLogFileStatusRowNumToUpdate;

  /* Current PM Log file information */
  UINT8                 currPmLogFileName[MAX_SIZE_PMLOGFILESTATUS_FILENAME+1];
  UINT8                 currPmLogFileOpenTimeStamp[MAX_SIZE_PMLOGFILESTATUS_OPENEDAT+1];

  /* PM Log file status information */
  PmLogFileStatusTable  pmLogFileStatusTable;

  CounterFlags          sendClearCounterReq;  /* Enable if COUNTER CLEAR REQ needs to be sent */
  CounterFlags          rcvdClearCounterRsp;  /* Enable if COUNTER CLAR RSP is received from expected module */
  CounterFlags          sendCounterReq;  /* Enable if COUNTER STAT REQ needs to be sent */
  CounterFlags          rcvdCounterRsp;  /* Enable if COUNTER STAT RSP is received from expected module */
  UINT8                 rspRspFromMacRclAndMacPhyCounter;
  TBOOL                 resetAllCounters;

  /* Used for get on all tables except PMLogFileStatus table */
  UINT8                 rspRspFromMacRclAndMacPhyCounterForGetCmd;
  UINT32                enodebId;

  ModTimersInfo         pmhTimersInfo;
  FilePath		configDir;
  FilePath		pmLogDir;

  VersionInfo           versionInfo;
  FILE                  *fp;
  TBOOL                 isCounterCollectionInProgress;
  TBOOL                 isSampleIntervalOrFileCreationTimeModified;

  UINT32                maxCombinedPmFileSize;
  UINT32                maxStsFileSize;

  EthernetHwInfoTable   ethernetHwInfoTable[MAX_ROWS_IN_ETHERNETCOUNTERS_TABLE];

  CmdRefOpt transId2CmdRefTable[MAX_PARALLEL_PMH_OAM_GET_PROCS];
  TBOOL                 isOpStartRcvd;
  String256             logBuf; /* for logging etc. temporary usage */
} PmhContext;

/**** PMH state Machine ***/
typedef PmhState  (*pmhMsgHandlerFp)(IN Msg *msg, IO PmhContext *pmhContext);

typedef struct s_PmhStateMachine
{
  pmhMsgHandlerFp fp;
} PmhStateMachine;

void initPmhStateMachine(
    void
    );
/**** PMH Common functions ***/

extern  PmhContext  g_PmhContext;
#define getPmhContext()                 &(g_PmhContext)
#define getPmhTimersInfo()              &(g_PmhContext.pmhTimersInfo)
#define getEthernetHwInfoTable()        &(g_PmhContext.ethernetHwInfoTable)
#define getEthernetHwInfoEntry(index)   &(g_PmhContext.ethernetHwInfoTable[index])
#define getCounterProfileCtx(profileId) &(g_PmhContext.counterProfileContext[profileId-1])

#define getSamplingIntervalInSec(secs)  secs

void pmhMain(
  FR Msg *msgIn
  );
void setPmhState(
  IO PmhContext   *ctx,
  IN PmhState     state
  );
char *getPmhStateName(
  IN PmhState state
  );
void initPmFileLogStatusTable(
  IO PmhContext  *pmhContext
  );

void pmhReadPmLogFileTableAndUpdateFileStatusTable();

void pmhHandleSampleIntervalTimeChange(
  IO PmhContext *ctx
  );
void doCounterCollectionProcCompletionSteps(
  IO PmhContext   *pmhContext
  );
TBOOL checkIfCurrentStsLogFileExists(
  UINT8    *fileName
  );
PmLogFileStatusTable *getPmLogFileStatusTable( void );

void sendGetObjRspSuccToBsm(
  IN UINT32         transId,
  IO PmhContext    *pmhContext
  );
void sendGetObjRspFailToBsm(
  IN CmdRef        *cmdRef,
  IN TagInfo       *tagInfo,
  IN BsErrorValue  *error
  );

void updatePmLogFileStatusTable( void );

PmhState pmhProcGetObjReq(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );

PmhState pmhProcSetLogCfgInAllStates(
    IN Msg           *msg,
    IO PmhContext    *pmhContext
    );

TBOOL pmhHasPmLogDirSizeExceeded(
  IO PmhContext *ctx,
  IO UINT32     *dirSize
  );
void sendStatReqToAllModules(
  IO PmhContext   *ctx
  );
void sendCounterResetReqToAllModule(
  IO PmhContext   *pmhContext,
  IO UINT32        transId
  );
void pmhProcClearCounterRsp(
  IO PmhContext    *pmhContext,
  IO UINT32         transId
  );
PmhState pmhProcCounterClearReqInWaitInitAndWaitOpstart(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcDiagCounterCmdInAllState(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcStatRspInInitCfgAndWaitOpstart(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
/** Hello Procedure Function */
PmhState pmhProcHelloReq(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
/** Exit Procedure Function */
PmhState pmhProcExit(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );

TBOOL pmhIsAnyCounterEnabled();

UINT32 pmhGetPmFileSizeInBytes(
  IN FilePath   pmLogFileName
  );
void sendNotiToBsmIfFileIsNonZeroOrClearFileIfZero(
  IN FilePath   pmLogFileName
  );
void pmhUpdatePmLogFileCloseTime(
  IN FilePath   pmLogFileName
  );
void pmhUpdatePmLogFileSize(
  IN FilePath  pmLogFileName
  );
TBOOL pmhAddEntryInPmLogFileStatusTable(
  IO PmhContext *ctx
  );
TBOOL pmhAddEntryInPmLogFileStatusTableForProfile(
  IO CounterProfileContext *pCtx
  );

void pmhClearPmLogFileStatusRow(
  IN FilePath pmLogFileName
  );
/*** PMH Timer related function prototypes */
void pmhReadTimersFromCfg(
  IO PmhContext  *ctx
  );
void closePmLogFileAndNotifyBsm(
  IO PmhContext *ctx
  );
TimerId pmhStartTimer(
  IN UINT8   timerIndex,
  IO UINT32  userInfo,
  IO UINT32  duration
  );
PmhState pmhProcTimeOutIndInIdleState(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcTimeOutIndInWaitInitAndWaitOpStartState(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcTimeOutIndInWaitBulkCounterCollectionState(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcTimeOutIndInWaitInitAndWaitOpStartState(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );

/** PMH Configuration functions ****/
void pmWritePmLogFileStatusTableToPersistentMemory(
  UINT8    *strWriteReason
  );
void pmhUpdateEntryInPmLogFileStatusTable(
  IO PmhContext *ctx
  );
PmhState pmhProcConfigReqInWaitInitAndWaitOpstart(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcConfigReqInIdleState(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcConfigReqInWaitBulkCollection(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcCntrInputConfigReq(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcMsgInUnexpectedState(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
void pmhGetCombinedPmLogSizeThreshold();

PmhState pmhProcOpStartReq(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcOpStopReqInWaitOpStart(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcOpStopReqInWaitInitCfg(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcOpStopReq(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );

void pmhSendCntrCollectFailMsgToBsm(
  IN BsErrorValue bsErrorValue,
  IN String100    failReason
  );

TBOOL pmhOpenPmLogFile(
  IO PmhContext *ctx
  );
TBOOL pmhOpenPmLogFileForProfile(
  IN ProfileId   profileId,
  IO PmhContext *ctx
  );

void pmhClosePmLogFile(
  IO PmhContext *ctx
  );
void pmhClosePmLogFileForProfile(
  IO CounterProfileContext *pCtx
  );
void pmhSendMeasurementCompleteNotiToBsm(
  IN ProfileId             profileId,
  IN MeasurementStopCause  cause
  );
void pmhSendCntrFileAvailableMsgToBsm(
  IN  FilePath    activePmLogFileName
  );
PmhState pmhProcPdcpGenStsCfm(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcPdcpUplaneStsCfm(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcSctpGenStsRsp(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcRrcGenStsRsp(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcRrcCellStsRsp(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcEthernetStatusReq(
  IN TBOOL          isGetReqProcessing,
  IO PmhContext    *pmhContext,
  IO GetObjReq     *getReq
  );
void pmhFillEthernetCountersTable(
  IO EthernetCounters  *ethCounters
  );
void pmhFillHardwareEthernetInfo(
  EthernetCounters *ethCounters,
  UINT8            *interfaceName,
  UINT32            index
  );
PmhState pmhProcIpStatusReq(
  IN TBOOL          isGetReqProcessing,
  IO PmhContext    *pmhContext,
  IO GetObjReq     *getObjReq
  );
void pmhFillIpCountersTable(
    IpCounters         *ipCounters
  );
/* Get counter data request message processing functions */
void pmhProcGetTableReq(
  IO GetObjReq   *getReq
  );
PmhState pmhProcPmFileUploadedMsg(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcPmFileUploadedMsgInIdleState(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );
TBOOL pmhStsRspRcvdFromAllModules(
    IN CounterFlags *sendCounterReq,
    IO CounterFlags *rcvdRspMsg
    );
PmhState pmhCheckRunTimeCounterFlags(
  IO PmhContext *pmhContext
  );
PmhState pmhProcGtpGenStsRsp(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcRlcGenStsRsp(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcMacGenStsRsp(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcMacRlcsapStatRsp(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcMacPhysapStatRsp(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcS1apGenStsRsp(
  IN Msg *msgIn,
  IO PmhContext *pmhContext
  );
PmhState pmhProcS1apPeerNodeStsRsp(
  IN Msg *msgIn,
  IO PmhContext *pmhContext
  );
PmhState pmhProcRrmStsRsp(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcDstatsInd(
    IN Msg           *msgIn,
    IO PmhContext    *pmhContext
    );
void createAndSendMacGenStatReq(
  IN CmdRef       *cmdRef,
  IN TagInfo      *tagInfo,
  IO PmhContext   *pmhContext
  );
void createAndSendMacUplaneStatReq(
  IN CmdRef      *cmdRef,
  IN TagInfo     *tagInfo,
  IO PmhContext  *pmhContext
  );
void createAndSendRlcGenStatReq(
  IN CmdRef       *cmdRef,
  IN TagInfo      *tagInfo,
  IO PmhContext   *pmhContext
  );
void createAndSendPdcpGenStatReq(
  IN CmdRef       *cmdRef,
  IN TagInfo      *tagInfo,
  IO PmhContext   *pmhContext
  );
void createAndSendPdcpUplaneStatReq(
  IN CmdRef       *cmdRef,
  IN TagInfo      *tagInfo,
  IO PmhContext   *pmhContext
  );
void createAndSendGtpGenStatReq(
  IN CmdRef      *cmdRef,
  IN TagInfo     *tagInfo,
  IO PmhContext  *pmhContext
  );
void createAndSendSctpGenStatReq(
  IN CmdRef      *cmdRef,
  IN TagInfo     *tagInfo,
  IO PmhContext  *pmhContext
  );
void createAndSendRrcGenStatReq(
  IN CmdRef   *cmdRef,
  IN TagInfo  *tagInfo,
  IO PmhContext  *pmhContext
  );
void createAndSendRrcCellStatReq(
  IN CmdRef      *cmdRef,
  IN TagInfo     *tagInfo,
  IN UINT8        cellId,
  IO PmhContext  *pmhContext
  );
void createAndSendS1apGenStatReq(
  IN CmdRef      *cmdRef,
  IN TagInfo     *tagInfo,
  IO PmhContext  *pmhContext
  );
void createAndSendS1apPeerNodeStatReq(
  IN CmdRef       *cmdRef,
  IN TagInfo      *tagInfo,
  IN UINT32        peerId,
  IO PmhContext   *pmhContext
  );
void createAndSendUehRrmStatReq(
  IN CmdRef       *cmdRef,
  IN TagInfo      *tagInfo,
  IO PmhContext   *pmhContext
  );
BsErrorValue   pmhAllocTransId(
  IN CmdRef      *cmdRef,
  IN TagInfo     *tagInfo,
  IO UINT32      *transId,
  IO PmhContext  *pmhContext
  );
BsErrorValue   getCmdRefByTransIdAndDeallocTransId(
  IN UINT32      transId,
  IO CmdRef      *cmdRef,
  IO TagInfo     *tagInfo,
  IO PmhContext  *pmhContext
  );

void createAndSendUdhDStatReq(
IN DStatsCmd cmd,
IN UINT32    interval_ms
);

void createAndSendTfuDstatsCfgReq(
IN DStatsCmd cmd,
IN UINT32    interval_ms
);

UINT8 * getIcpuCtrName (
DCounterId id
);
PmhState pmhProcSetObjReq(
  IN Msg           *msg,
  IO PmhContext    *pmhContext
  );

PmhState pmhProcUdhDStatsInd(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
void pmhInitCounterCollectionStatusTable();
void pmhInitCounterContext(
  IO PmhContext  *pmhContext
  );
PmhState pmhProcCounterStartSucc(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcCounterStartFail(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcCounterStopSucc(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcCounterStopInd(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
PmhState pmhProcCounterReportInd(
  IN Msg           *msgIn,
  IO PmhContext    *pmhContext
  );
Duratn pmhGetSampleIntervalTimeVal(
  IN SampleInterval sampleInterval
  );
Duratn pmhGetPmLogFileCreationTimeVal(
  IN PmFileCreationInterval Interval
  );
void pmhFillAndSendSetObjFail(
  IN SetObjReq          *setReq,
  IN BsErrorValue        bsErrorValue,
  IN TagInfo            *errTagInfo
  );
void pmhFillAndSendSetObjSucc(
  IN SetObjReq   *setObj
  );
void pmhWriteToPmLogFilePerProfileAndUpdateCtx(
  IO MsgUehPmhCounterReportInd *ind,
  IO PmhContext                *ctx
  );

UINT32 getReportingPeriodInSec(
  IN ReportingDuration   reportingDuration
  );
void pmhClosePmLogFileAndUpdateStatusTablesAndStopTimerAndSendNoti(
  IN ProfileId             profileId,
  IN MeasurementStopCause  cause,
  IO PmhContext           *pmhContext
  );
void pmhCloseAndOpenNewPmLogFileAndRestartTimer(
  IN ProfileId    profileId,
  IO PmhContext  *pmhContext
  );
void pmhLogProfileContext(
  CounterProfileContext *pCtx,
  UINT8                 *info
  );


#endif /* PMH_H_ */

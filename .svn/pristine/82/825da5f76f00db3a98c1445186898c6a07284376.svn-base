/**
 * @file    cih.h
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains all function definitions and Macros required by CLI module.
 *
 * @author  mkata
 * @date    10-05-2010
 */
#ifndef CIH_H
#define CIH_H

#include <stropts.h>
#include <poll.h>
#include <fcntl.h>
#include <ctype.h>
#include <setjmp.h>
#include <sys/select.h>
#include <signal.h>
#include <regex.h>

#include "print_tlv.h"

#include "libtecla.h"

#include "all_cmn.h"
#include "tlv_enc_dec.h"
#include "bs_modules.h"
#include "generic_table.h"
#include "get_oam_func.h"
#include "set_oam_func.h"

#include "all_cfg.h"
/* Interface file */
#include "bsm_cih_if.h"

#include "enumstr.h"

extern SINT32 g_cmdsSent;
extern SINT32 g_respRecd;

/* Macros */
#define MAX_OBJECTS_IN_CIH_COMMAND      10
#define MAX_CHARECTERS_IN_USER_CMD      1024     /* fgets buff */
#define MAX_CHARECTERS_IN_LINE          256

#define CIH_EXIT                        "exit\n"
#define CIH_LOGOUT                      "logout\n"
#define CIH_QUIT                        "quit\n"
#define CIH_QUIT_Q                      "q\n"
#define CIH_QUIT_CAPS_Q                 "Q\n"
#define CIH_HELP                        "help"
#define CIH_CLEAR                       "clear\n"
#define CIH_HISTORY                     "history\n"
#define NULL_STR                        "NULL_STR"

#define POLL_TIMEOUT_SECS               20
#define BUFFER_SIZE                     500 /* max length of line to be read from stdin */
#define MAX_TOKENS_IN_CIH_COMMAND       10
#define MAX_CMDS_IN_BATCH_FILE          50
#define MAX_CMDS_IN_ONELINE_IN_FILE     MAX_CMDS_IN_BATCH_FILE
#define cihIncrRefNum(refNum)           refNum++;
#define MIN_TOKENS_IN_CMD               2
#define OBJ_IN_SET_CMD                  2
#define MAX_TOKENS_IN_SET_OBJID_STR     3
#define MAX_TOKENS_IN_GET_OBJID_STR     3
#define MAX_TOKENS_OBJVAL_STR          50
#define MAX_ENUMS                      100
#define MAX_BUFFER_BATCH_FILE_PROC     2000
#define  MAX_CIH_CMD_RSP_WAIT_TIME     10 /* max sleep time is 10 seconds */
#define CIH_CMD_RSP_SLEEP_DURATION     1   /*max sleep time is 10 seconds */



#define TABLEID "^[0-9]*$"
#define ROWID "^[0-9]*$"
#define COLUMNID "^[0-9]*$"
#define OPTIONALPARAMETERPATTERN "^[#]$"
#define READONLYPARAMETERPATTERN "^[#]$"
#define SIGNEDINTEGER "^[-]{0,1}[0-9]+$"
#define UNSIGNEDINTEGER "^[0-9]+$"
#define BOOLEAN "^[01]$"
#define IPV4PATTERN "^(([0-9]|[1-9][0-9]{1,2})[.]){1,3}([0-9]|[1-9][0-9]{1,2})$" /* Pattern for IP address Validation */
/*Pattern for MAC address validation */
#define MACADDRPATTERN "^[0-9a-fA-F]{2}[:][0-9a-fA-f]{2}[:][0-9a-fA-F]{2}[:][0-9a-fA-f]{2}[:][0-9a-fA-F]{2}[:][0-9a-fA-f]{2}$"
#define FILENAMEPATTERN "^[a-zA-Z0-9./][a-zA-Z0-9./_-]{1,256}$" /* Pattern for Filename Validation */
#define SWVERSIONPATTERN "^([0-9]|[1-9][0-9]*)[.]([0-9]|[1-9][0-9]*)[.]([0-9]|[1-9][0-9]*)[.]([0-9]|[1-9][0-9]*)[.]([0-9]|[1-9][0-9]*)$" /* Pattern for SW version Validation */
#define MCCPATTERN "^[0-9]{3}$" /* Pattern for MCC Validation */
#define MNCPATTERN "^[0-9]{2,3}$" /* Pattern for MNC Validation */
#define LOCALTIMEPATTERN "^[0-9]{4}([1][1-2]|[0][1-9])([0][1-9]|[1-2][0-9]|[3][0-1])([0-1][0-9]|[2][0-3])[0-5][0-9][0-5][0-9][0-9]{3}$"
#define OFFSETTIMEPATTERN "^[+-]([0-1][0-9]|[2][0-3])[0-5][0-9]$" /* Pattern for Offset Validation */
#define LATITUDEPATTERN "^[0-9]{4}[.][0-9]{6}[NS]$" /* Pattern for Latitude Validation */
#define LONGITUDEPATTERN "^[0-9]{5}[.][0-9]{6}[EW]$" /* Pattern for Longitude Validation */
#define ENODEBNAMEPATTERN "^[a-zA-Z][a-zA-Z0-9-]{2,149}$" /* Pattern for Enodeb name Validation */
#define PROFILENAMEPATTERN "^[a-z]{3,15}$" /* Pattern for Profile name Validation */
typedef struct pollfd Pollfd;
typedef int Ret;

typedef String50    Action;
typedef String50    Object;
typedef String50    Prompt;
typedef String100   Buffer;
typedef String100   EnumStr;
typedef String512   ObjIdStr;
typedef String512   ObjStr;
typedef String5kb   DisplayBuffer;
typedef String1kb   ObjValStr;
/**
 * The SignalActions object provides a way to temporarily install
 * a signal handler to a given set of signals, and later restore all
 * of the signal handlers that this displaced.
 */
typedef struct {
  int nsignal;               /* The number of signals on the host OS */
  sigset_t mask;             /* The set of signals who's signal handlers */
                             /*  are stored in the following actions[] array. */
  struct sigaction *actions; /* An array of nsignal actions */
} SignalActions;

typedef struct timeval TimeInfoSeconds;
typedef struct tm TimeInfoStandard;

typedef struct s_CmdsInfo
{
  CmdRef   cmdRef;
  TBOOL    isRspExpected;
  ObjStr   userCmdStr;
} CmdsInfo;
#define MAX_CMDS_IN_TIMER_UNIT 100

typedef struct s_CmdStatus
{
  UINT32    noOfCommands;
  CmdsInfo  cmdsInfo[MAX_CMDS_IN_TIMER_UNIT];
}CmdStatus;

/* Timers  */
typedef ModTimersInfo CihTimersInfo;

typedef struct s_CihContext
{
  VersionInfo        versionInfo;
  FilePath           rootDir;
  FilePath           configDir;
  FILE               *cihCmdLogFilePointer;
  TBOOL              isBatchFile;
  TBOOL              isCmdLineMode;
  TBOOL              isRunScriptFile;
  ObjStr             userCmdStr;        /* User commands will be stored for display at timeout */
  UINT8              totalTimers;
  CihTimersInfo      cihTimersInfo;
  CmdStatus          cmdsWaitingForRsp; /* Commands Sent to BSM and still waiting for Response from BSM */
  CmdStatus          cmdsAtTimerStart;  /* Commands Waiting for Response at timer starts */
  CmdStatus          *presentState;
  CmdStatus          *prevState;
  OamTableContext    cihOamTableContext;
  InstanceId         instanceId;  /*CLI Instance */ 
} CihContext;

/* Message Trace */
typedef MsgTrace MsgBsmCihSetMsgTrace;

/* Global Context */
extern CihContext     g_cihCtx;

/* To get the Global context */
#define getGlobalCihContext()           &(g_cihCtx)

/* To get the cli instance id */
#define getCihInstanceId()              g_cihCtx.instanceId

/* To get the Global context */
#define getUserCommand()                &(g_cihCtx.userCmdStr)

/* CIH Module Timers info */
#define getCihTimersInfo()              &(g_cihCtx.cihTimersInfo)

/* Commands Waiting for Response */
#define getCihCmdsWaitingForRsp()       &(g_cihCtx.cmdsWaitingForRsp)

/* Commands Waiting for Response */
#define getCihCmdsAtTimerStart()        &(g_cihCtx.cmdsAtTimerStart)

/* Get Present State */
#define getPresentCmdsStatus()          (g_cihCtx.presentState)

/* Get Previous state */
#define getPreviousCmdsStatus()         (g_cihCtx.prevState)

/* To get Log file pointer */
#define getGlobalCihLogFileName()       (g_cihCtx.cihCmdLogFilePointer)

/* To get Config File Path */
#define getGlobalCihConfigFilePath()    &(g_cihCtx.configDir)

/* Get OAM Table Context */
#define getOamTableContext()           &(g_cihCtx.cihOamTableContext)

/* Get Oam Table Information on table Id */
#define cihGetOprCmdTableInfo(tableId)   &(g_cihCtx.cihOamTableContext.oprCmdTableInfo[tableId])
#define cihGetOprCfgTableInfo(tableId)   &(g_cihCtx.cihOamTableContext.oprCfgTableInfo[tableId])
#define cihGetStatusTableInfo(tableId)   &(g_cihCtx.cihOamTableContext.statusTableInfo[tableId])
#define cihGetPreCfgTableInfo(tableId)   &(g_cihCtx.cihOamTableContext.preCfgTableInfo[tableId])

/* Number of Valid Tables Available at CIH */
#define getNumOfValidTables()          &(g_cihCtx.cihOamTableContext.noOfvalidTables)
/* Error Info Macros */
#define cihErrInOpeningFile( filename ) \
  BS_ERROR_IN_OPENING_LOG_FILE(filename );


/* PROTO TYPES */

void printBanner(
  IN VersionInfo   *versionInfo
  );

UINT8* getUsername();

UINT8* cihDateTimeConversion(
  IN UINT8 *builtOn,
  IO UINT8 *dateAndTimeString
  );

void cihCloseLog();

UINT8 cihParseBatchFileString(
  IO UINT8  *inputStr,
  IO ObjStr tokens[]
  );

void procBatchCmds(
  IN UINT8    noOfCmds,
  IO ObjStr   *userCommands
  );

TBOOL procBatchCmdsInCmdLine(
  IN UINT8    noOfCmds,
  IO ObjStr   *userCommands
  );

void cihProcessBachFile(
  IN char     *fileName,
  IO ObjStr   *userCommands
  );

TBOOL procCommandsInCmdLine(
  IO char     *inputStr,
  IO ObjStr   *userCommands
  );

TBOOL cihProcessRunScriptFiles(
  IN char     *fileName
  );




/* Added for TLV */
TBOOL cihProcessSetCmd(
  IO CmdRef   *cmdRef,
  IO ObjStr   *setCmdStr
  );
MsgId getCihSetObjReqMsgId(void);

TBOOL cihProcessGetCmd(
  IO CmdRef   *cmdRef,
  IO ObjStr   *getCmdStr
  );
MsgId getCihGetObjReqMsgId(void);

TBOOL cihProcessRunScriptCmd(
  IN UINT8    noOfObjsInCmd,
  IO ObjStr   *tokens
  );

UINT8 cihParseCmdStr(
  IO UINT8  *inputStr,
  IO ObjStr *tokens
  );

TBOOL cihProcBsmRsp();

void procSetObjSucc(
  IN Msg      *msgIn,
  IN TimeInfoSeconds  *time
  );

void procGetObjSucc(
  IN Msg      *msgIn,
  IN TimeInfoSeconds  *time
  );

void procSetObjFail(
  IN Msg      *msgIn,
  IN TimeInfoSeconds  *time
  );

void procGetObjFail(
  IN Msg      *msgIn,
  IN TimeInfoSeconds  *time
  );

UINT8 cihParseObjIdStr(
  IO UINT8    *inputObjIdStr,
  IO ObjIdStr *tokens
  );

UINT8 cihParseObjValStr(
  IO UINT8     *inputObjValStr,
  IO ObjValStr *tokens
  );

void cihParseSetcmd(
  IO UINT8    *setCmdstr,
  IO ObjIdStr *tableAndValueStr
  );

void printRangeCheckFailHeader();
void printAttributeCheckFailHeader();
void printOptionalCheckFailHeader();
void printExpectedDataCheckFailHeader();
void printFilenameCheckFailHeader();
void printSoftwareVersionCheckFailHeader();
void printTimeCheckFailHeader();
void printExpectedDataCheckFail();

void printRangeCheckFail(
  IN  TagInfo     *tableInfo,
  IN  UINT32      *minVal,
  IN  UINT32      *maxVal,
  IO  ObjValStr   *colValue,
  IO  CmdRef      *cmdRef
);

void printAttributeCheckFail(
  IN  TagInfo     *tableInfo,
  IN  UINT8        colId,
  IO  ObjValStr   *colValue,
  IO  CmdRef      *cmdRef
);

void printOptionalCheckFail(
  IN  TagInfo     *tableInfo,
  IN  UINT8        colId,
  IO  ObjValStr   *colValue,
  IO  CmdRef      *cmdRef
);

TBOOL cihSpecialValCheck(
  IO  TagInfo   *setCmdTableInfo,
  IO  ObjValStr *colValue,
  IO  UINT8     colId
  );

UINT32 stringlen(
  IN UINT8 *colValue
  );

TBOOL validParameterCheck(
  IN UINT8 *colValue
  );

TBOOL readOnlyParameterCheck(
  IN UINT8 *colValue
  );

TBOOL optionalParameterCheck(
  IN UINT8 *colValue
  );

TBOOL signedIntegerCheck(
  IN UINT8 *colValue
  );

TBOOL unsignedIntegerCheck(
  IN UINT8 *colValue
  );

TBOOL booleanCheck(
  IN UINT8 *colValue
  );

TBOOL fillTlvColInfo(
  IN UINT8     colId,
  IO ObjValStr *colValue,
  IO TlvArray  *tlvArray,
  IO TagInfo   *setCmdTableInfo,
  IO CmdRef    *cmdRef
  );

void cihDisplayObjIdFail(
  IO CmdRef   *cmdRef
  );

void cihDisplaySetTableFail(
  IO CmdRef   *cmdRef
  );

void cihDisplaySetRowFail(
  IN TableId  tableId,
  IN UINT32   *maxColsInTable,
  IO CmdRef   *cmdRef
  );

void cihDisplaySetColFail(
  IO CmdRef   *cmdRef
  );

TBOOL validateObjId(
  IO UINT32   tableId,
  IO UINT32   rowId,
  IO UINT32   colId
  );

void cihLogRsp(
  IO UINT8* rspString
  );

void cihDisplayCmdRefSucc(
  IN CmdRef   *cmdRef,
  IN TimeInfoSeconds  *time
  );

void cihDisplayCmdRefFail(
  IO CmdRef   *cmdRef,
  IN TimeInfoSeconds  *time
  );

TBOOL checkNumericTableId(
  IN UINT8 *value
  );

TBOOL checkNumericRowId(
  IN UINT8 *value
  );

TBOOL checkNumericColumnId(
  IN UINT8 *value
  );

TBOOL validateTableId(
  IN UINT32   tableId
  );
TBOOL validateRowId(
  IN UINT32   tableId,
  IN UINT32   rowId
  );
TBOOL validateColId(
  IN UINT32   tableId,
  IN UINT32   colId
  );

/* Added for TLV--ENDS */
/* Find the action needs to be taken */
TBOOL cihProcCmdRcvdFromUser(
  IN UINT8   noOfObjsInCmd,
  IN UINT32  refNum,
  IO ObjStr  tokens[]
  );

void cihExecuteCmdsInCmdLine (
  SINT32   argc,
  char*    argv[]
  );

void cihPrintBanner(
  IO SINT32   argc,
  IO char*   argv[],
  IN VersionInfo *versioninfo
  );

void getCihRootDir(
  IO SINT32   argc,
  IO char     *argv[],
  IO FileName rootDir
  );

void cihHelpCfgHandler(
  IN UINT8*   configFilePath
  );

void cihLogCommands(
  IO UINT8* userCmd,
  IO UINT32 cmdNumber
  );

void cihLogInit(
  IN UINT8   *rootDir
  );

TBOOL isStatusTable(
  IO TableId  tableId,
  IO CmdRef   *cmdRef
  );

TBOOL isOprCmdTable(
  IO TableId  tableId,
  IO CmdRef   *cmdRef
  );

int bsmCihMsgHandler(
  IN Msg      *msgIn
  );

void procCihCommands(
  IO GetLine       *gl
  );

UINT8 cihParseBatchFileForComments(
  IO UINT8  *inputStr,
  IO ObjStr *tokens
  );

void cihDisplayCmdRefDelayedRsp(
  IN TimeInfoSeconds  *time
  );

void cihDisplayCmdRefDeclined(
  IO CmdRef   *cmdRef
  );

/* CIH_HELP.C */

TBOOL cihProcHelpCmd(
  IN UINT8     *noOfObjsInCmd,
  IO ObjStr    *helpCmdStr
  );

TBOOL cihProcVersionCmd(
  IN UINT8     *noOfObjsInCmd
  );

void cihDisplayTablesInfo();

TBOOL initial_match(
  const char *line1,
  const char *line2,
  int word_end
  );

CPL_MATCH_FN(cmd_cpl_fn);

/* Timers Function Proto Types */
void readCihTimersFromCfg(
  IN UINT8   *configFilePath
  );

void startCihRspTimer(
  IN TimerName         timerType
  );

void cihProcTimerExpiry(
  IN Msg           *msg
  );

void  stopCihRspTimer(
  IO TimerName   type
  );

void initCmdStatus();

void addEntryToCmdStatus();

TBOOL removeEntryFromCmdStatus(
  IN CmdRef *cmdRef
  );
void changeTimersRspTable();

void cihDisplayTimeoutMsgs(
  IN UINT32      cmdNumber,
  IO CmdStatus   *cmdsAtTimerStart
  );

void cihStoreTimersInCtx(
  IN UINT8         totalTimers,
  IN TimersConfig  *config
  );

/********************    cih_oamtable_func.c   *****************/

inline OamTableType getTableTypeByTableId(
  IN TableId       tableId
  );

OamTableInfo* getTablePtrByTableId(
  IN TableId      tableId
  );


/******************** Response Handling functions **************/

void cihPrintHeaderForFailCmd();

void cihPrintInfoFailCmd();

void cihPrintHeader();

EnumStrInfo *cihGetEnumStrFunction(
  IO TagInfo   *tag
  );

void displayObjInfo(
  IO TlvInfo  *objTlvInfo
  );

void displayObjTagInfo(
  IO TagInfo      *tlvTag
  );

void cihDisplayObjRsp(
  IO TlvArray    *tlvArray
  );

void cihDisplayError(
  IO ErrRsp      *errRsp
  );

void procSetMsgTrace(
  IN Msg      *msgIn
  );

void cihPrintTableEmpty(
  IO TableId  tableId
  );



/******************* Validation Functions ********************/

TBOOL ipv4AddressCheck(
  IN UINT8 *colValue
  );

TBOOL macAddressCheck(
  IN UINT8 *colValue
  );

TBOOL fileNameCheck(
  IN UINT8 *colValue
  );

TBOOL localTimeCheck(
  IN UINT8 *colValue
  );

TBOOL offsetTimeCheck(
  IN UINT8 *colValue
  );

TBOOL swVersionCheck(
  IN UINT8 *colValue
  );

TBOOL mccCheck(
  IN UINT8 *colValue
  );

TBOOL mncCheck(
  IN UINT8 *colValue
  );

TBOOL latitudeCheck(
  IN UINT8 *colValue
  );

TBOOL longitudeCheck(
  IN UINT8 *colValue
  );

TBOOL enodebNameCheck(
  IN UINT8 *colValue
  );

TBOOL profileNameCheck(
  IN UINT8 *colValue
  );

TBOOL cihRangeCheck(
  IO  TagInfo   *setCmdTableInfo,
  IO  ObjValStr *colValue,
  IO  UINT8     colId,
  IO  CmdRef    *cmdRef
  );

/**************************** CIH Help Functions **************************/

void convertAttributeToStr(
  IN Attributes     *attributes,
  IO AttributeStr   *attributeStr
  );

void convertDataTypeToStr(
  IO DataType       *dataType,
  IO DataTypeStr    *dataTypeStr
  );

void printHelpGetCmd();
void printHelpRunScriptCmd();
void printHelpVersionCmd();
void printHelpQuitCmd();
void printHelpHistoryCmd();
void printHelpClearCmd();
void printCihCmdsHelp();
void printCihVersion();

void printHelpTablesHeader();

void printTableHelpHeader();

void printHelpTables(
  IN  TableId        tableId
  );

void printUnknownTableId();

void printColumnHelp(
  IO  OamColInfo  *colInfo
  );

void printUnknownColumnId();
void printRowIdNotAllowed();

void printTableHelp(
  IN  TableId        tableId,
  IN  OamTableInfo   *cihHelp
  );

void printRowHelp(
  IN  TableId        tableId,
  IN  RowId          rowId
  );

void printColHeaderHelp();

void printOamHelpHeader();

void printLine(
  IO  UINT8         *printBuffer
  );

void printHelpSetCmd();

TBOOL checkBsmRsp();


#endif



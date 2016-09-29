/**
 * @file    fth.h
 * @brief   File Transfer Handler, This file holds the functions and message API.
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @author  Lakshmi Narayana Madala
 * @date    15-Jul-2010
 */

/**
 * FTH : FILE TRANSFER HANDLER
 */

#ifndef FTH_H
#define FTH_H

/* Common .h files */
#include "generic_table.h"
/* FTH to other module interface .h files */
#include "libgen.h"
#include <dirent.h>
#include "print_tlv.h"
#include "start_indication.h"
#include "get_oam_func.h"

/* System .h Files */
/* This flag is added as gcc 4.3 will go for a toss,
 * As this flag is checked in curl, so put it ahead of curl.h inclusion
 */
#define CURL_DISABLE_TYPECHECK
#include "curl.h"
#include "types.h"
#include "easy.h"

/* Macros */
#define FTH_LOG_FILENAME                        "fth_trace.log"
#define LEN_OF_FTP_URL  ( sizeof(ServerName) + sizeof(LoginId) + sizeof(Password) + sizeof(FilePath) )

/**
 * FTH state ENUM
 */
typedef enum e_FthState
{
  FTH_STATE_IDLE = 1,               /* FTH will enters this state during the start of the module*/
  FTH_STATE_DOWNLOAD_IN_PROG,       /* FTH will enters this state while downloading the file*/
  FTH_STATE_UPLOAD_IN_PROG,         /* FTH will enters this state while uploading the file*/
  FTH_MAX_STATE						/* NO OPstop message is received at FTH as per new design */
}FthState;

typedef ModTimersInfo FthTimersInfo;

typedef struct s_FthSetObjReq
{
  CmdRef      cmdRef;
  NumTags     numTlvs;
  TlvInfo     tlvInfo[MAX_COLS_IN_FILEUPLOAD_TABLE];
}FthSetObjReq;

/**
 * This is passed to CURL as call back structure,
 * It is returned by the CURL, when there is data or any progress
 * in FTP download
 */
typedef struct s_FthContext
{
  VersionInfo           versionInfo;
  FthCfgTable           fthCfg;
  FileName              configDir;
  FthState              fthState;
  CURL                 *curlHandle;
  FILE                 *lclFilePtr;
  FILE                 *logFilePtr;
  FileName              lclFilePath;
  TBOOL                 isStopFileTransfer;
  TBOOL                 isSuccSent;
  TBOOL                 isFileSizeExceed;
	TBOOL									isScheduled;
	UINT8									totalTimers;
	FthTimersInfo					fthTimersInfo;
  BsErrorValue          bsError;
  UINT32                ulFileSize;
  UINT32                dlFileSize;
  UINT32	        indexFileStatusTable;
  TBOOL                 isFileSizeChecked;
  UINT8                 noOfEntriesInFileEventHistory;
  FileDownload          fileDownload;
  FileUpload            fileUpload;
  FthSetObjReq          setObjReq;
}FthContext;

/* Global Context */

typedef void (*fthMsgHandlerFp)(
  IN Msg           *msg,
  IO FthContext    *fthContext
  );

typedef struct s_FthStateMachine
{
  fthMsgHandlerFp  fp;
  FthState         nextState;
}FthStateMachine;


/* ################ START OF MACROS ####################### */

/* Set the state machine */
#define InitFthStateMachine( presentState, msgIn, procFunc, newState) \
            fthStateMachine[ presentState ][msgIn].fp = procFunc; \
            fthStateMachine[ presentState ][msgIn].nextState = newState;

   /* Set the state of FTH */
#define fthSetState( context, state) \
                                 (context)->fthState = state

#define getFthTimersInfo()       &(g_FthContext.fthTimersInfo)
#define SCHEDULE_DOWNLOAD_LIMIT  43200

/* ################## FTH API ##################### */
FthContext    *getFthContext();
extern FthContext    g_FthContext;

/* ************** FUNCTION PROTOTYPES ************************ */
void initFthStateMachine( void );
void fthMain(
  FR Msg *msgIn
  );
void fthReadAndProcessRecvMsg(
  Msg *msgIn
  );
void fthCreateAndSendUploadCmpl(
  IN FthContext *fthContext
  );
void fthCreateAndSendUploadFail(
  IN BsErrorValue   error
  );
void fthCreateAndSendDwCmpl(
  IO FthContext    *fthContext
  );
void fthCreateAndSendDwFail(
  IN BsErrorValue  error
  );
void fthCreateAndSendHelloAck(
  IN Msg    *msg
  );
void fthStartFileDownload(
  IN FileDownload        *dwReq,
  IO FthContext          *fthContext
  );
void fthStartFileUpload(
  IN FileUpload    *uploadReq,
  IO FthContext    *fthContext
  );
void fthGetObjReq(
  IN Msg *msg,
  IO FthContext  *fthContext
  );
void fthStopDownloadAndClearLocalFile(
  IO FthContext   *fthContext
  );
void fthStopUploadAndClearRemoteFile(
  IO FthContext   *fthContext
  );
void fthInitStatusTables(
  IO FthContext   *fthContext
  );
void fthUpdateDownloadDir(
  IN UINT8   *rootDir,
  IO UINT8   *downloadDirPath
  );
TBOOL readFthCfgTable(
  IN UINT8    *configFilePath
  );
char *getFthProceduralStateName(
  IN FthState state
  );
void fthFillAndSendSetObjFail(
  IN SetObjReq          *setReq,
  IN BsErrorValue        bsErrorValue,
  IO FthContext         *fthContext
  );
void fthFillAndSendSetObjSucc(
  IN SetObjReq          *setReq,
  IO FthContext         *fthContext
  );
void fthFillAndSendSetObjSuccRsp(
	IN SetObjReq          *setReq,
	IN TBOOL              isDelayed
	);
void fthFillAndSendSetObjFailRsp(
	IN SetObjReq          *setReq,
	IN BsErrorValue       bsErrorValue,
	IN TBOOL              isDelayed
	);
void fthUpdateFileDownloadStatusTable(
	IN FileDownloadCommandReceived commandReceived,
	IN UINT8                       *fileName,
	IN DownloadStatus              downloadStatus,
	IN UINT32                      percentageComplete,
	IN UINT8                       *scheduleDateAndTime
	);

TBOOL checkAndFillTimeDuration (
	char *scheduleDateAndTime,
	SINT32 *duration
	);
void fthStartTimer(
	IN TimerName        timerType,
	IN SINT32           duration
	);
void  fthStopTimer(
	IO TimerName   type
	);
	
void readFthTimersFromCfg(
	IN UINT8   *configFilePath
	);
void fthStoreTimersInCtx(
	IN UINT8         totalTimers,
	IN TimersConfig  *config
	);

void fthProcTimerExpiryInAllStates(
		IN Msg           *msg,
		IO FthContext    *fthContext
		);

void fthCopyTlvArrayInCtx(
	IN SetObjReq   *setReq,
	IO FthContext  *fthContext
	);
void fthProcFileDownCmdInIdleState(
	IO SetObjReq   *setReq,
	IN FileDownload fileDownload
	);
void fthProcFileDownCmdInDownloadInProgState(
	IO SetObjReq   *setReq,
	IN FileDownload fileDownload
	);


#endif /* FTH_H */

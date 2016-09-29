/**
 * @file    lth.h
 * @brief   API for Logging and Tracing Handler (Abbrev: LTH)
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author  Muktheshwar Madishetty
 * @date    12-10-2012
 */

/**
 * LTH = Logging and Tracing Handler
 */

#ifndef LTH_H_
#define LTH_H_

#include "bsm_lth_if.h"
#include "lth_sm_if.h"
#include "all_cfg.h"
#include "generic_table.h"
#include "start_indication.h"
#include "imc_udp.h"
#include <dirent.h>
#include <time.h>

/* Maximum wireshark file size */
#define MAX_TRACE_FILE_LINE_SIZE 1024

#define TRACE_S1AP_STRING       "S1AP"
#define TRACE_RRC_STRING        "RRC"
#define TRACE_MAC_STRING        "MAC"
#define TRACE_RLC_STRING        "RLC"
#define TRACE_PDCP_STRING        "PDCP"

typedef struct s_LthSetObjReq {
	CmdRef cmdRef;
	NumTags numTlvs;
	TlvInfo tlvInfo[MAX_COLS_IN_FILEUPLOAD_TABLE];
} LthSetObjReq;

typedef ModTimersInfo LthTimersInfo;


typedef struct s_LthTime {
  int tm_min;			/* Minutes.	[0-59] */
  int tm_hour;			/* Hours.	[0-23] */
} LthTime;

/* LTH CONTECXT*********************************/

typedef struct s_LthTraceContext {
	FilePath swConfigDir;
	FilePath swRootDir;
	FileName traceFileName;
	VersionInfo versionInfo;
	FILE *fp;
	UINT32 bytesWritten;
	TBOOL isTraceOn;
	TBOOL maxTraceFileSizeReached;
	UINT32 maxTraceFileSize;
	TraceConfig currentTraceConfig;
	TraceConfig oldTraceConfig;
	UINT32 traceDuration;
	SINT32 sockFd;
	UINT16 numOfTraceCtrlReqSent;
	UINT16 numOfTraceCtrlRspRcvd;
	TBOOL dynamicConfigChange;
	LthTimersInfo timersInfo;
	LthSetObjReq setObjReq;
	UINT8 currentListIndex;
    LthTime traceStartTime;
} LthTraceContext;





extern LthTraceContext g_LthTraceContext;

#define getLthTraceContext()  &(g_LthTraceContext)

typedef MsgTrace MsgBsmLthSetMsgTrace;

void readEnbLimitsFromCfg(IN UINT8 *configFilePath);

void lthProcHelloReq(Msg *msg);
void lthExit(void);
void lthMain( FR Msg *msgIn);

void lthWriteTraceMessageToFile( IO TraceLen traceLen, IO UINT8 *traceMsg,
		IO struct timespec timeVal, IN UINT8 *frmMod);
void lthProcTraceInd(IN Msg *msg);
void lthProcTimeoutInd(IN Msg *msg);
void lthProcSetMsgTrace(IN Msg *msg);
void lthProcSetObjReq(IN Msg *msg);
void sendTrcModifyReqToAllProtocols();
TBOOL isTraceRunning();
TBOOL isAlteastOneProtocolEnabled(IN TraceConfig *config);
void sendStartTraceReq(IN TraceConfigMod protocolName);
void stopProtocolTrace();
void sendStopTraceReq(IN TraceConfigMod protocolName);
Duratn getTraceDurationFrmEnum(IN TraceDuration traceDuration);
void lthProcTraceStartSucc(IN Msg *msgIn);
void lthProcTraceStartFail(IN Msg *msgIn);
void lthProcTraceStopSucc(IN Msg *msgIn);
void lthProcTraceStopFail(IN Msg *msgIn);
void lthFillAndSendSetObjFail(IN BsErrorValue bsErrorValue);
void lthFillAndSendSetObjSucc();
void stopLthProcedureTimer( IO TimerName type);
void initTraceStatusTable();
void initTimersInfoInCtx();
void initLthTraceContext();
void initTraceHistoryTable();
void initTraceConfig(TraceConfig *traceConfig);
void copyTraceConfig(const TraceConfig *srcTraceConfig, TraceConfig *dstTraceConfig);
void lthProcGetObjReq(IN Msg *msg);
void fillTraceStatusTlvElemt(IN TableId tableId, IN RowId rowId,
		IN ColId colId, IO TlvArray *tlvRspArray);
void fillTraceHistoryTlvElemt(IN TableId tableId, IN RowId rowId,
		IN ColId colId, IO TlvArray *tlvRspArray);
void lthProcInitConfigReq(IN Msg *msg);
TBOOL IsSpaceAvailableinTraceFile(IN UINT32 maxTraceMessageSize);
UINT8 *getTraceModString(IN TraceConfigMod protocolName);
#endif /* LTH_H_ */

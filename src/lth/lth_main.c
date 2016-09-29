/**
 * @file    lth_main.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will handle all the messages coming from different modules
 *
 * @author  Muktheshwar Madishetty
 * @date    12-10-2012
 * @details This file contains functions to process different messages received from other modules
 */

#include "lth.h"

/* ################### GLOBAL VARIABLES ############### */
LthTraceContext g_LthTraceContext;


/**
 * @details This function initializes the module timers
 */
void initTimersInfoInCtx(void) {
	LthTraceContext *lthTrcCtx = getLthTraceContext();

	UINT8 timerArrayIndex = 0;
	lthTrcCtx->timersInfo.totalTimersInMod = 0;

	lthTrcCtx->timersInfo.modTimerInfo[timerArrayIndex].timerDesc.moduleId =
			MODID_LTH;
	lthTrcCtx->timersInfo.modTimerInfo[timerArrayIndex].timerDesc.timeSpec.duration =
			5;
	lthTrcCtx->timersInfo.modTimerInfo[timerArrayIndex].timerDesc.timeSpec.timeUnit =
			TIMEUNIT_MINUTE;
	lthTrcCtx->timersInfo.modTimerInfo[timerArrayIndex].timerDesc.timerType =
			TIMERNAME_WAITTRACEDURATION;
	lthTrcCtx->timersInfo.modTimerInfo[timerArrayIndex].timerDesc.timeoutMsgId =
			MSG_TIMEKEEPER_LTH_TIMEOUT_IND;

	lthTrcCtx->timersInfo.totalTimersInMod++;
	timerArrayIndex++;
}

/**
 * @details This function initializes Lth Ctx fields
 */
void initLthTraceContext() {
	LthTraceContext *lthTrcCtx = getLthTraceContext();

	copyBytes("file-does-not-exist", strlen("file-does-not-exist"),
			lthTrcCtx->traceFileName);
	initTraceConfig(&lthTrcCtx->oldTraceConfig);
	initTraceConfig(&lthTrcCtx->currentTraceConfig);
	lthTrcCtx->currentListIndex = 1;
	lthTrcCtx->isTraceOn = FALSE;
	lthTrcCtx->bytesWritten = 0;
	lthTrcCtx->traceStartTime.tm_min = 0;
	lthTrcCtx->traceStartTime.tm_hour = 0;
	lthTrcCtx->maxTraceFileSizeReached = FALSE;
}

/**
 * @details This function initializes trace config structure
 */
void initTraceConfig(TraceConfig *traceConfig) {

	// Trace Config Table Update
	traceConfig->traceDestination = TRACEDESTINATION_COLLECTTOFILE;
	traceConfig->traceRRC = FALSE;
	traceConfig->traceS1AP = FALSE;
	traceConfig->traceX2AP = FALSE;
	traceConfig->traceReserved1 = FALSE;
	traceConfig->traceReserved2 = FALSE;
	traceConfig->traceReserved3 = FALSE;
	copyBytes("No IP Configured", strlen("No IP Configured"),
			traceConfig->traceDestIPAddress);
	traceConfig->traceDestUdpPort = 0;
}

/**
 * @details This function initializes trace status table
 */
void initTraceStatusTable() {
	TraceStatus *traceStatus = (TraceStatus*) getFirstRowOfTable(TRACESTATUS);

	traceStatus->index = 1;
	traceStatus->traceDestination = TRACEDESTINATION_COLLECTTOFILE;
	traceStatus->traceDurationConfiguredInMins = 0;
	traceStatus->traceDurationElapsedInMins = 0;
	traceStatus->traceId = 0;
	traceStatus->traceMessageCounter = 0;
	traceStatus->traceState = TRACESTATE_TRACEOFF;

}

/**
 * @details This function initializes trace history table
 */
void initTraceHistoryTable() {

	UINT8 trcHistoryIndex;
	LthTraceContext *lthTrcCtx = getLthTraceContext();

	TraceHistory *traceHistory = NULL;

	for (trcHistoryIndex = 1; trcHistoryIndex < MAX_ROWS_IN_TRACEHISTORY_TABLE;
			trcHistoryIndex++)

	{
		traceHistory = (TraceHistory*) getRowOfTable(TRACEHISTORY,
				trcHistoryIndex);

		traceHistory->traceId = trcHistoryIndex;
		snprintf(&traceHistory->traceStartedAt[0],
				MAX_SIZE_TRACEHISTORY_TRACESTARTEDAT, "00000000000000000");
		snprintf(&traceHistory->traceStoppedAt[0],
				MAX_SIZE_TRACEHISTORY_TRACESTOPPEDAT, "00000000000000000");
		traceHistory->traceDurationElapsedInMins = 0;
		traceHistory->traceDestination = TRACEDESTINATION_COLLECTTOFILE;
		copyBytes("NO FILE CREATED", MAX_SIZE_TRACEHISTORY_TRACEFILENAME,
				lthTrcCtx->traceFileName);
	}

}

/**
 * @param  configFilePath is file path to identify location of config file
 * @details This function reads enbLimits config file and loads the values in trace context
 */
void readEnbLimitsFromCfg(IN UINT8 *configFilePath) {
	UINT8 isError = FALSE;
	UINT8 fullFilePath[MAX_BUFFER_SIZE];
	EnbLimitsConfig enbLimitsConfig;

	clearBytes( &fullFilePath, sizeof(fullFilePath));
	clearBytes(&enbLimitsConfig, sizeof(EnbLimitsConfig));
	snprintf(fullFilePath, MAX_BUFFER_SIZE, "%s/enbLimits.cfg", configFilePath);

	if (TRUE
			NOTEQ readFromFileEnbLimitsConfigTable(fullFilePath,
					&enbLimitsConfig, &isError)) {
		BS_ERROR_CONFIG_READ_FAILED()
		;
		exit(EXIT_FAILURE);
	} else {
		LthTraceContext *trcCtx = getLthTraceContext();
		/*
		 * Update the max trace file size in trace context
		 */
		trcCtx->maxTraceFileSize = enbLimitsConfig.maxTraceFileSize * 1024
				* 1024;
		logInfo("Max trace file size is [%d] in bytes",
				trcCtx->maxTraceFileSize);
	}
}

/**
 * @param  protocolName  Protocol Name enum
 * @details This function Protocol Name in string
 */
UINT8 *getTraceModString(IN TraceConfigMod protocolName) {
	switch (protocolName) {
	case TRACECONFIGMOD_RRC:
		return "RRC";
	case TRACECONFIGMOD_S1AP:
		return "S1AP";
	case TRACECONFIGMOD_RLC:
		return "RLC";
	case TRACECONFIGMOD_PDCP:
		return "PDCP";

	default:
		return "This message ID is not handled .. check LTH code";
	}
}

/**
 * @param   msgIn  message received
 * @details This is the entry point for all messages coming into LTH
 */
void lthMain( FR Msg *msgIn) {
	LthTraceContext *lthTrcCtx = getLthTraceContext();

	switch (msgIn->msgId) {

	case MSG_TIMEKEEPER_LTH_TIMEOUT_IND: {
		lthProcTimeoutInd(msgIn);
	}
		break;

	case MSG_PRM_LTH_HELLO: {
		lthProcHelloReq(msgIn);
		break;
	}
	case MSG_PRM_LTH_EXIT_CMD: {
		lthExit();
		break;
	}
	case MSG_BSM_LTH_INIT_CONFIG_REQ: {
		lthProcInitConfigReq(msgIn);
		break;
	}
	case MSG_BSM_LTH_SET_OBJ_REQ: {
		lthProcSetObjReq(msgIn);
		break;
	}

	case MSG_BSM_LTH_GET_OBJ_REQ: {
		lthProcGetObjReq(msgIn);
		break;
	}

	case MSG_SM_LTH_TRACE_START_SUCC: {
		lthProcTraceStartSucc(msgIn);
		break;
	}

	case MSG_SM_LTH_TRACE_START_FAIL: {
		lthProcTraceStartFail(msgIn);
		break;
	}

	case MSG_SM_LTH_TRACE_STOP_SUCC: {
		TraceHistory *traceHistory = (TraceHistory*) getRowOfTable(TRACEHISTORY,
				lthTrcCtx->currentListIndex);
		traceHistory->traceEndCause = TRACEENDCAUSE_STOPPEDBYOPERATOR;
		lthProcTraceStopSucc(msgIn);
		break;
	}

	case MSG_SM_LTH_TRACE_STOP_FAIL: {
		lthProcTraceStopFail(msgIn);
		break;
	}
	case MSG_SM_LTH_TRACE_IND: {
		lthProcTraceInd(msgIn);
		break;
	}
	case MSG_BSM_LTH_SET_MSG_TRACE: {
		lthProcSetMsgTrace(msgIn);
		break;
	}
		/* Added handling of SET_LOG_CFG from BSM */
	case MSG_BSM_LTH_SET_LOG_CFG: {
		MsgBsmLthSetLogCfg *updateLogConfig = (MsgBsmLthSetLogCfg *) msgIn->data;
		LogContext *logContext = getLogContext();
		copyBytes(updateLogConfig, sizeof(LogConfig), &(logContext->logConfig));
		logInfo("%s", "Loglevel is set to new values");
		break;
	}
	default: {
		/* Unknown Message received */
		BS_ERROR_RECEIVED_INVALID_MSG( msgIn->msgId, 0);
	}
		break;
	}

	/* Free received message */
	freeMsgIn(msgIn);
	printAndEmptyErrorChain(&(getModuleContext()->errChain));
}

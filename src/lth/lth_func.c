/**
 * @file    lth_func.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will contains functions to process different messages received from BSM, RRC, MAC and S1AP
 *
 * @author  Muktheshwar Madishetty
 * @date    12-10-2012
 * @details This file contains functions to process different messages received from other modules
 */

#include "lth.h"

/**
 * @param    msg received message from PRM
 * @details  This function sends HELLO_ACK message
 */
void lthProcHelloReq(Msg *msg) {
	MsgPrmModHello *hello = (MsgPrmModHello*) msg->data;
	MsgModPrmHelloAck helloAck;
	clearBytes( &helloAck, sizeof(MsgModPrmHelloAck));
	helloAck.helloAckSeqNum = hello->helloSeqNum;

	createAndSendMsg(MSG_LTH_PRM_HELLO_ACK, (UINT8*) &helloAck,
			sizeof(MsgModPrmHelloAck));
}

/**
 * @param   traceDuration Trace Duration enum value.
 * @details This function will return the duration in integer value
 *          based in the enum value.
 **/
Duratn getTraceDurationFrmEnum(IN TraceDuration traceDuration) {
	switch (traceDuration) {
	case TRACEDURATION_5MIN:
		return 5;
	case TRACEDURATION_15MIN:
		return 15;
	case TRACEDURATION_30MIN:
		return 30;
	case TRACEDURATION_45MIN:
		return 45;
	case TRACEDURATION_60MIN:
		return 60;
	default:
		return 60;
	}
}

/**
 * @param traceDuration  Trace Duration enum
 * @param timerInfo      Timer Info holds all timers in the module
 * @details This function create and start Trace Duration Timer.
 **/
void startTraceDurationTimer(IN TraceDuration traceDuration,
		IO LthTimersInfo *timerInfo) {
	TBOOL timerFound = FALSE;
	UINT8 i = 0;
	for (i = 0; i < timerInfo->totalTimersInMod; i++) {
		if (TIMERNAME_WAITTRACEDURATION
				EQ timerInfo->modTimerInfo[i].timerDesc.timerType) {
			timerFound = TRUE;
			timerInfo->modTimerInfo[i].timerDesc.timeSpec.duration =
					getTraceDurationFrmEnum(traceDuration);
			timerInfo->modTimerInfo[i].timerDesc.timeSpec.timeUnit =
					TIMEUNIT_MINUTE;
			break;
		}
	}
	if (TRUE EQ timerFound) {
		/* Create and start Timer */
		logInfo("%s-%d", "Timer Found", 1);
		timerInfo->modTimerInfo[i].timerId = createAndStartTimer(
				&timerInfo->modTimerInfo[i].timerDesc, NULL, NULL );
	} else {
		BS_ERROR_TIMER_NOT_FOUND( timerNameStr(TIMERNAME_WAITTRACEDURATION));
	}
}

/**
 * @details  This function handles exit message from PRM
 */
void lthExit(void) {
	LthTraceContext *lthTrcCtx = getLthTraceContext();

	if (TRUE EQ lthTrcCtx->isTraceOn) {

		stopLthProcedureTimer(TIMERNAME_WAITTRACEDURATION);
		stopProtocolTrace(lthTrcCtx);

		if (TRACEDESTINATION_RELAYTOREMOTEHOST
				NOTEQ lthTrcCtx->currentTraceConfig.traceDestination) {

			if (EOF EQ fclose(lthTrcCtx->fp)) {
				perror("fclose");
			}
			lthTrcCtx->fp = NULL;
		} else {
			lthTrcCtx->sockFd = -1;
		}
	}
	modExit(MODID_LTH, &(getModuleContext()->exeName[0]));
	exit(EXIT_SUCCESS);
}

/**
 * @param       maxTraceMessageSize     Length of max trace message size
 * @details     This function checks if space available to write current
 *              trace message into that file
 */
TBOOL IsSpaceAvailableinTraceFile(IN UINT32 maxTraceMessageSize) {

	LthTraceContext *lthTrcCtx = getLthTraceContext();
	TBOOL retVal = FALSE;

	if (lthTrcCtx->maxTraceFileSize
			> (lthTrcCtx->bytesWritten + maxTraceMessageSize)) {
		retVal = TRUE;
	}

	return retVal;
}
/**
 * @param    config     current trace config
 * @details  This function checks and returns TRUE if at least one
 *           protocol enabled for trace else returns FALSE.
 */
TBOOL isAlteastOneProtocolEnabled(IN TraceConfig *config) {
	if ((config->traceRRC EQ FALSE) && (config->traceS1AP EQ FALSE)) {
		return FALSE;
	} else {
		return TRUE;
	}
}

/**
 * @details  This function returns TRUE if trace is running else  FALSE.
 */
TBOOL isTraceRunning() {
	TraceStatus *traceStatus = (TraceStatus*) getFirstRowOfTable(TRACESTATUS);
	if (traceStatus->traceState EQ TRACESTATE_TRACEON) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
 * @param    srcTraceConfig     const source trace config
 * @param    dstTraceConfig     destination trace config 
 * @details  This function copies the source trace config structure content into
 *           destination trace config structure
 */
void copyTraceConfig(const TraceConfig *srcTraceConfig,
		TraceConfig *dstTraceConfig) {

	dstTraceConfig->traceDestination = srcTraceConfig->traceDestination;
	dstTraceConfig->traceRRC = srcTraceConfig->traceRRC;
	dstTraceConfig->traceS1AP = srcTraceConfig->traceS1AP;
	dstTraceConfig->traceX2AP = srcTraceConfig->traceX2AP;
	dstTraceConfig->traceReserved1 = srcTraceConfig->traceReserved1;
	dstTraceConfig->traceReserved2 = srcTraceConfig->traceReserved2;
	dstTraceConfig->traceReserved3 = srcTraceConfig->traceReserved3;
    
	clearBytes( &dstTraceConfig->traceDestIPAddress, sizeof(dstTraceConfig->traceDestIPAddress));
	copyBytes(srcTraceConfig->traceDestIPAddress,
			strlen(srcTraceConfig->traceDestIPAddress),
			dstTraceConfig->traceDestIPAddress);
	dstTraceConfig->traceDestUdpPort = srcTraceConfig->traceDestUdpPort;
}

/**
 * @param    msg     received message from BSM
 * @details  This function processes the GET OBJ REQ from BSM
 */
void lthProcGetObjReq(IN Msg *msg) {

	GetObjReq getObjReq;
	TableId tableId;
	GetObjRspSucc rspTlv;
	EncBuf encBuf;
	UINT8 colIndex = 0;
	UINT8 rowIndex = 0;
	struct timeval tv;
	struct timezone tz;
	struct tm* tm;

	TagInfo *tagInfo = &(getObjReq.tagArray.tagInfo[0]); /* One Get = One Req message */

	LthTraceContext *lthTrcCtx = getLthTraceContext();
	TraceStatus *traceStatus = (TraceStatus*) getFirstRowOfTable(TRACESTATUS);

	clearBytes( &getObjReq, sizeof(GetObjReq));
	decGetObjReqMsg((UINT8*) msg->data, msg->len, &getObjReq);

	tableId = getObjReq.tagArray.tagInfo[0].tableId;

	switch (tableId) {
	case TRACEHISTORY: {

		initEncBuf(&encBuf);
		clearBytes( &rspTlv, sizeof(GetObjRspSucc));

		/* Fill CmdRef details */
		copyBytes( &(getObjReq.cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

		for (rowIndex = 1; rowIndex < lthTrcCtx->currentListIndex; rowIndex++) {
			for (colIndex = 1; colIndex <= MAX_COLS_IN_TRACEHISTORY_TABLE;
					colIndex++) {
				fillTraceHistoryTlvElemt(tagInfo->tableId, rowIndex, colIndex,
						&(rspTlv.tlvArray));
			}
		}
	}
		break;
	case TRACESTATUS: {

		initEncBuf(&encBuf);
		clearBytes( &rspTlv, sizeof(GetObjRspSucc));

		/* Fill CmdRef details */
		copyBytes( &(getObjReq.cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

		/* Get the current time to calculate the trace duration elapsed */
		gettimeofday(&tv, &tz);
		tm = localtime(&tv.tv_sec);

		if (TRUE EQ isTraceRunning()) {
		    traceStatus->traceDurationElapsedInMins = (tm->tm_hour
				- lthTrcCtx->traceStartTime.tm_hour) * 60
				+ (tm->tm_min - lthTrcCtx->traceStartTime.tm_min);
        }
		for (colIndex = 1; colIndex <= MAX_COLS_IN_TRACESTATUS_TABLE;
				colIndex++) {
			fillTraceStatusTlvElemt(tagInfo->tableId, 1, colIndex,
					&(rspTlv.tlvArray));
		}
	}
		break;
	}
	/* Encode */
	encGetObjRspSuccMsg(&rspTlv, &encBuf);
	/* Send */
	createAndSendMsg(MSG_LTH_BSM_GET_OBJ_SUCC, (UINT8*) &encBuf.value,
			encBuf.len);
}

/**
 * @param   tableId         Table Id for which we have to fill the details
 * @param   rowId           Row Id of that particular table
 * @param   colId           column Id of that particular row.
 * @param   tlvRspArray     Tlv response array which we have to fill.
 * @details This function will fill the element details of given table,row,column for trace history table.
 * */
void fillTraceHistoryTlvElemt(IN TableId tableId, IN RowId rowId,
		IN ColId colId, IO TlvArray *tlvRspArray) {

	TraceHistory *traceHistory = (TraceHistory*) getRowOfTable(TRACEHISTORY,
			rowId);

	switch (colId) {
	case COL_TRACEHISTORY_INDEX:
		fillTagLenUint32(tableId, rowId, colId, traceHistory->index,
				tlvRspArray);
		break;
	case COL_TRACEHISTORY_TRACEID:
		fillTagLenUint32(tableId, rowId, colId, traceHistory->traceId,
				tlvRspArray);
		break;
	case COL_TRACEHISTORY_TRACESTARTEDAT:
		fillTagLenString(tableId, rowId, colId, traceHistory->traceStartedAt,
				MAX_SIZE_TRACEHISTORY_TRACESTARTEDAT, tlvRspArray);
		break;

	case COL_TRACEHISTORY_TRACESTOPPEDAT:
		fillTagLenString(tableId, rowId, colId, traceHistory->traceStoppedAt,
				MAX_SIZE_TRACEHISTORY_TRACESTOPPEDAT, tlvRspArray);
		break;
	case COL_TRACEHISTORY_TRACEDURATIONELAPSEDINMINS:
		fillTagLenUint32(tableId, rowId, colId,
				traceHistory->traceDurationElapsedInMins, tlvRspArray);
		break;
	case COL_TRACEHISTORY_TRACEDESTINATION:
		fillTagLenUint32(tableId, rowId, colId, traceHistory->traceDestination,
				tlvRspArray);
		break;

	case COL_TRACEHISTORY_TRACEFILENAME:
		fillTagLenString(tableId, rowId, colId, traceHistory->traceFileName,
				MAX_SIZE_TRACEHISTORY_TRACEFILENAME, tlvRspArray);
		break;
	case COL_TRACEHISTORY_TRACEENDCAUSE:
		fillTagLenUint32(tableId, rowId, colId, traceHistory->traceEndCause,
				tlvRspArray);
		break;
	default:
		BS_ERROR_INVALID_COLID_RCVD( tableId, rowId, colId);
		break;
	}
}
/**
 * @param   tableId         Table Id for which we have to fill the details
 * @param   rowId           Row Id of that particular table
 * @param   colId           column Id of that particular row.
 * @param   tlvRspArray     Tlv response array which we have to fill.
 * @details This function will fill the element details of given table,row,column for trace status table.
 * */
void fillTraceStatusTlvElemt(IN TableId tableId, IN RowId rowId, IN ColId colId,
		IO TlvArray *tlvRspArray) {

	TraceStatus *traceStatus = (TraceStatus*) getFirstRowOfTable(TRACESTATUS);

	switch (colId) {
	case COL_TRACESTATUS_INDEX:
		fillTagLenUint32(tableId, rowId, colId, traceStatus->index,
				tlvRspArray);
		break;
	case COL_TRACESTATUS_TRACEID:
		fillTagLenUint32(tableId, rowId, colId, traceStatus->traceId,
				tlvRspArray);
		break;
	case COL_TRACESTATUS_TRACESTATE:
		fillTagLenUint32(tableId, rowId, colId, traceStatus->traceState,
				tlvRspArray);
		break;

	case COL_TRACESTATUS_TRACEDURATIONCONFIGUREDINMINS:
		fillTagLenUint32(tableId, rowId, colId,
				traceStatus->traceDurationConfiguredInMins, tlvRspArray);
		break;
	case COL_TRACESTATUS_TRACEDURATIONELAPSEDINMINS:
		fillTagLenUint32(tableId, rowId, colId,
				traceStatus->traceDurationElapsedInMins, tlvRspArray);
		break;
	case COL_TRACESTATUS_TRACEMESSAGECOUNTER:
		fillTagLenUint32(tableId, rowId, colId,
				traceStatus->traceMessageCounter, tlvRspArray);
		break;
	case COL_TRACESTATUS_TRACEDESTINATION:
		fillTagLenUint32(tableId, rowId, colId, traceStatus->traceDestination,
				tlvRspArray);
		break;

	default:
		BS_ERROR_INVALID_COLID_RCVD( tableId, rowId, colId);
		break;
	}
}

/**
 * @param msg received msg from BSM
 * @details This function process INIT_CONFIG_REQ from BSM.
 */
void lthProcInitConfigReq(IN Msg *msg) {

	MsgBsmLthInitConfigReq *msgInitConfigReq =
			(MsgBsmLthInitConfigReq *) msg->data;
	MsgLthBsmInitConfigSucc msgInitConfigSucc;
	LthTraceContext *lthTrcCtx = getLthTraceContext();

	copyTraceConfig(&(msgInitConfigReq->traceConfig),
			&(lthTrcCtx->currentTraceConfig));
	clearBytes(&msgInitConfigSucc, sizeof(MsgLthBsmInitConfigSucc));
	msgInitConfigSucc.transId = msgInitConfigReq->transId;
	createAndSendMsg(MSG_LTH_BSM_INIT_CONFIG_SUCC, (UINT8*) &msgInitConfigSucc,
			sizeof(MsgLthBsmInitConfigSucc));
}
/**
 * @param msg  received msg from BSM
 * @details This function process SET_OBJ_REQ message from BSM.
 */
void lthProcSetObjReq(IN Msg *msg) {

	SetObjReq setObjReq;
	TableId tableId;
	clearBytes( &setObjReq, sizeof(SetObjReq));
	decSetObjReqMsg((UINT8*) msg->data, msg->len, &setObjReq);
	tableId = setObjReq.tlvArray.tlvInfo[0].tag.tableId;

	LthTraceContext *lthTrcCtx = getLthTraceContext();
	String256 timeStamp;
	time_t noSecs = 0;

	TraceControl traceControl;
	TraceConfig traceConfig;

	lthTrcCtx->setObjReq.numTlvs = setObjReq.tlvArray.numTlvs;
	copyBytes(&(setObjReq.cmdRef), sizeof(CmdRef),
			&(lthTrcCtx->setObjReq.cmdRef));
	copyBytes(&(setObjReq.tlvArray.tlvInfo),
			sizeof(TlvInfo)*(lthTrcCtx->setObjReq.numTlvs),
			&(lthTrcCtx->setObjReq.tlvInfo));

	switch (tableId) {
	case TRACECONFIG:

		clearBytes( &traceConfig, sizeof(TraceConfig));

		GenericTable *tableFunc = getTableFunctions(TRACECONFIG);

		if (NULL NOTEQ tableFunc->getTlv2StructFunc) {

			tableFunc->getTlv2StructFunc(&(setObjReq.tlvArray), &traceConfig);
		}

		if (FALSE EQ isAlteastOneProtocolEnabled(&traceConfig)) {

			lthFillAndSendSetObjFail(BS_ERR_TRACE_CONFIG_ALL_FALSE);
		}

		if (TRUE EQ isTraceRunning()) {
        
		   copyTraceConfig(&(lthTrcCtx->currentTraceConfig),
				&(lthTrcCtx->oldTraceConfig));
        }
		copyTraceConfig(&traceConfig, &(lthTrcCtx->currentTraceConfig));

		if (TRUE EQ isTraceRunning()) {

			lthTrcCtx->dynamicConfigChange = TRUE;
			sendTrcModifyReqToAllProtocols(lthTrcCtx);

		} else {

			lthFillAndSendSetObjSucc();
		}

		break;
	case TRACECONTROL: {

		clearBytes( &traceControl, sizeof(traceControl));

		GenericTable *tableFunc = getTableFunctions(TRACECONTROL);

		if (NULL NOTEQ tableFunc->getTlv2StructFunc) {

			tableFunc->getTlv2StructFunc(&(setObjReq.tlvArray), &traceControl);
		}

		/* Check all the TLV are present or not. Index column is not necessary */
		if (MAX_COLS_IN_TRACECONTROL_TABLE - 1 > setObjReq.tlvArray.numTlvs) {

			lthFillAndSendSetObjFail(
					BS_ERR_SET_ON_SINGLE_PARAMETER_UNSUPPORTED);
			return;

		}

		switch (traceControl.traceCommand) {
		case TRACECOMMAND_STARTTRACE: {

			if (FALSE EQ isTraceRunning()) {
				if (TRUE
						EQ isAlteastOneProtocolEnabled(
								&lthTrcCtx->currentTraceConfig)) {
					lthTrcCtx->isTraceOn = TRUE;
					lthTrcCtx->traceDuration = traceControl.traceDuration;

					if (TRACEDESTINATION_COLLECTTOFILE
							EQ lthTrcCtx->currentTraceConfig.traceDestination) {

						clearBytes(timeStamp, sizeof(String256));
						noSecs = time(NULL );
						strftime(timeStamp, sizeof(String256),
								"%Y_%b_%d_%H_%M_%S", localtime(&noSecs));
						snprintf(lthTrcCtx->traceFileName, sizeof(String256),
								"%sPico_Enb_%d_TRACE_log_%s.txt",
								(char*) &lthTrcCtx->swRootDir[0],1/*eNodeBID*/,
								timeStamp);

						lthTrcCtx->fp = fopen(lthTrcCtx->traceFileName, "w+");

						if (lthTrcCtx->fp NOTEQ NULL ) {

							setvbuf(lthTrcCtx->fp, NULL, _IONBF /*_IOLBF*/, 80);
							sendTrcModifyReqToAllProtocols(lthTrcCtx);
						} else {
							perror("fopen");
							BS_ERROR_IN_OPENING_LOG_FILE(
									lthTrcCtx->traceFileName);

							lthFillAndSendSetObjFail(
									BS_ERR_TRACE_FILE_OPEN_ERROR);

						}

					} else if (TRACEDESTINATION_RELAYTOREMOTEHOST
							EQ lthTrcCtx->currentTraceConfig.traceDestination) {

						/* Open UDP Socket */
						if (openUdpSocket(
								(lthTrcCtx->currentTraceConfig.traceDestUdpPort-1),
								TRUE, &lthTrcCtx->sockFd)) {
							lthTrcCtx->isTraceOn = TRUE;
							lthTrcCtx->traceDuration =
									traceControl.traceDuration;
							sendTrcModifyReqToAllProtocols(lthTrcCtx);
						} else {
							lthFillAndSendSetObjFail(
									BS_ERR_TRACE_UDP_SOCKET_OPEN_ERROR);
						}

					}

				} else {

					lthFillAndSendSetObjFail(BS_ERR_TRACE_CONFIG_ALL_FALSE);

				}

			} else {
				lthFillAndSendSetObjFail(BS_ERR_TRACE_ALREADY_RUNNING);

			}
		}
			break;
		case TRACECOMMAND_STOPTRACE: {
			if (TRUE EQ isTraceRunning()) {

				lthTrcCtx->isTraceOn = FALSE;
				TraceHistory *traceHistory = (TraceHistory*) getRowOfTable(
						TRACEHISTORY, lthTrcCtx->currentListIndex);
				traceHistory->traceEndCause = TRACEENDCAUSE_STOPPEDBYOPERATOR;
				getAndFillDateAndTimeStamp(MAX_SIZE_TRACEHISTORY_TRACESTOPPEDAT,
						&traceHistory->traceStoppedAt[0]);
				stopLthProcedureTimer(TIMERNAME_WAITTRACEDURATION);
				stopProtocolTrace(lthTrcCtx);
			} else {
				lthFillAndSendSetObjFail(BS_ERR_TRACE_NOT_RUNNING);
			}

		}
			break;

		}

	}
		break;
	}

}

/**
 * @param   type Type of Timer.
 * @details This function stops the timer
 */
void stopLthProcedureTimer( IO TimerName type) {
	LthTraceContext *lthTraceContext = getLthTraceContext();
	UINT8 timerCounter;
	String50 buf;
	clearBytes( &buf, sizeof(String50));

	for (timerCounter = 0;
			timerCounter < lthTraceContext->timersInfo.totalTimersInMod;
			timerCounter++) {
		if (lthTraceContext->timersInfo.modTimerInfo[timerCounter].timerDesc.timerType
				EQ type) {
			stopTimer(
					lthTraceContext->timersInfo.modTimerInfo[timerCounter].timerId);
			break;
		}
	}
}

/**
 * @details This function will send the disable trace request to configured protoccols
 * in current trace config in Lth Ctx
 */
void stopProtocolTrace() {

	MsgLthSmTraceCntrlReq cntrlReq;
	LthTraceContext *lthTrcCtx = getLthTraceContext();
	clearBytes(&cntrlReq, sizeof(MsgLthSmTraceCntrlReq));

	lthTrcCtx->numOfTraceCtrlReqSent = 0;
	lthTrcCtx->numOfTraceCtrlRspRcvd = 0;

	if (lthTrcCtx->currentTraceConfig.traceDestination
			NOTEQ TRACEDESTINATION_RELAYTOREMOTEHOST) {

		if (lthTrcCtx->currentTraceConfig.traceS1AP)
			sendStopTraceReq(TRACECONFIGMOD_S1AP);

	}
	if (lthTrcCtx->currentTraceConfig.traceRRC) {
		sendStopTraceReq(TRACECONFIGMOD_PDCP);
		sendStopTraceReq(TRACECONFIGMOD_RLC);
	}

}

/**
 * @details This function initiates either start/stop trace requests based on dynamicConfigChange flag in Lth Ctx.
 */
void sendTrcModifyReqToAllProtocols() {

	LthTraceContext *lthTrcCtx = getLthTraceContext();
	lthTrcCtx->numOfTraceCtrlReqSent = 0;
	lthTrcCtx->numOfTraceCtrlRspRcvd = 0;
	if (TRACEDESTINATION_RELAYTOREMOTEHOST
			NOTEQ lthTrcCtx->currentTraceConfig.traceDestination) {

		if (lthTrcCtx->oldTraceConfig.traceS1AP
				NOTEQ lthTrcCtx->currentTraceConfig.traceS1AP) {
			if (lthTrcCtx->currentTraceConfig.traceS1AP)
				sendStartTraceReq(TRACECONFIGMOD_S1AP);
			else
				sendStopTraceReq(TRACECONFIGMOD_S1AP);
		}

	}

	if (lthTrcCtx->oldTraceConfig.traceRRC
			NOTEQ lthTrcCtx->currentTraceConfig.traceRRC) {

		if (lthTrcCtx->currentTraceConfig.traceRRC) {
			sendStartTraceReq(TRACECONFIGMOD_PDCP);
			sendStartTraceReq(TRACECONFIGMOD_RLC);
		} else {
			sendStopTraceReq(TRACECONFIGMOD_PDCP);
			sendStopTraceReq(TRACECONFIGMOD_RLC);
		}
	}

}

/**
 * @param   protocolName   Name of the protocol in enum
 * @details This function checks type of the protocol and initiates the stop trace req.
 */
void sendStopTraceReq(IN TraceConfigMod protocolName) {

	MsgLthSmTraceCntrlReq cntrlReq;
	LthTraceContext *lthTrcCtx = getLthTraceContext();
	UINT16 sizeOfMsgLthSmTraceCntrlReq = sizeof(MsgLthSmTraceCntrlReq);
	clearBytes(&cntrlReq, sizeOfMsgLthSmTraceCntrlReq);

	cntrlReq.traceCtrlCmd = TRACECOMMAND_STOPTRACE;

	switch (protocolName) {
	case TRACECONFIGMOD_S1AP:
		cntrlReq.protocolName = TRACECONFIGMOD_S1AP;
		createAndSendMsg(MSG_LTH_SM_TRACE_STOP_REQ, (UINT8 *) &cntrlReq,
				sizeOfMsgLthSmTraceCntrlReq);
		break;

	case TRACECONFIGMOD_RRC:
		cntrlReq.protocolName = TRACECONFIGMOD_RRC;
		createAndSendMsg(MSG_LTH_SM_TRACE_STOP_REQ, (UINT8 *) &cntrlReq,
				sizeOfMsgLthSmTraceCntrlReq);
		break;
	case TRACECONFIGMOD_RLC:
		cntrlReq.protocolName = TRACECONFIGMOD_RLC;
		createAndSendMsg(MSG_LTH_SM_TRACE_STOP_REQ, (UINT8 *) &cntrlReq,
				sizeOfMsgLthSmTraceCntrlReq);
		break;
	case TRACECONFIGMOD_PDCP:
		cntrlReq.protocolName = TRACECONFIGMOD_PDCP;
		createAndSendMsg(MSG_LTH_SM_TRACE_STOP_REQ, (UINT8 *) &cntrlReq,
				sizeOfMsgLthSmTraceCntrlReq);
		break;

	}
	lthTrcCtx->numOfTraceCtrlReqSent++;

}

/**
 * @param   protocolName   Name of the protocol in enum
 * @details This function checks type of the protocol and initiates the start trace req.
 */
void sendStartTraceReq(IN TraceConfigMod protocolName) {

	MsgLthSmTraceCntrlReq cntrlReq;
	LthTraceContext *lthTrcCtx = getLthTraceContext();
	UINT16 sizeOfMsgLthSmTraceCntrlReq = sizeof(MsgLthSmTraceCntrlReq);
	clearBytes(&cntrlReq, sizeOfMsgLthSmTraceCntrlReq);

	cntrlReq.traceCtrlCmd = TRACECOMMAND_STARTTRACE;

	switch (protocolName) {
	case TRACECONFIGMOD_S1AP:
		cntrlReq.protocolName = TRACECONFIGMOD_S1AP;
		createAndSendMsg(MSG_LTH_SM_TRACE_START_REQ, (UINT8 *) &cntrlReq,
				sizeOfMsgLthSmTraceCntrlReq);
		break;

	case TRACECONFIGMOD_RRC:
		cntrlReq.protocolName = TRACECONFIGMOD_RRC;
		createAndSendMsg(MSG_LTH_SM_TRACE_START_REQ, (UINT8 *) &cntrlReq,
				sizeOfMsgLthSmTraceCntrlReq);
		break;
	case TRACECONFIGMOD_RLC:
		cntrlReq.protocolName = TRACECONFIGMOD_RLC;
		createAndSendMsg(MSG_LTH_SM_TRACE_START_REQ, (UINT8 *) &cntrlReq,
				sizeOfMsgLthSmTraceCntrlReq);
		break;
	case TRACECONFIGMOD_PDCP:
		cntrlReq.protocolName = TRACECONFIGMOD_PDCP;
		createAndSendMsg(MSG_LTH_SM_TRACE_START_REQ, (UINT8 *) &cntrlReq,
				sizeOfMsgLthSmTraceCntrlReq);
		break;

	}
	lthTrcCtx->numOfTraceCtrlReqSent++;

}

/**
 * @param time      timeStamp containing u_secs and secs
 * @param traceLen  indicates length of trace message
 * @param traceMsg  indicates content of trace message
 * @param frmMod    module ID in string format
 * @details       This function process write the hex data to .txt file in
 *                K12.txt format which wireshark can read.
 *
 * +---------+---------------+----------+
 * 06:31:41,477,064   ETHER
 * |0   |00|00|00|00|0a|17|6e|0c|01|29|00|30|00|01|04|17|11|83|0a|3c|8e|01|04|5b|10|83|0a|00|00|00|00|00|00|00|00|20|0e|00|03|00|00|00|
 *
 */
TBOOL lthWriteTraceToFile( IO struct timespec *time,
		IO TraceLen traceLen, IO UINT8 *traceMsg, IN char *frmMod) {

	SINT8 buf[MAX_TRACE_FILE_LINE_SIZE];
	size_t bytesLeft = MAX_TRACE_FILE_LINE_SIZE;
	UINT32 tempBufIndex;
	SINT8 *p = buf;
	UINT32 i;
	UINT32 microSecs;
	UINT32 milliSecs;
	TBOOL retVal = FALSE;

	LthTraceContext *lthTrcCtx = getLthTraceContext();
	milliSecs = time->tv_nsec / 1000000;
	microSecs = (time->tv_nsec - (1000000 * milliSecs)) / 1000;

	/* Print the packet seperator */
	strftime(p, 90, "+---------+---------------+----------+\r\n%H:%M:%S,",
			localtime(&time->tv_sec));
	tempBufIndex = strlen(p);
	p += tempBufIndex;
	bytesLeft -= tempBufIndex;

	/* Print the frame format */
	tempBufIndex = snprintf(p, bytesLeft, "%.3d,%.3d   %s\r\n|0   |", milliSecs,
			microSecs, frmMod);
	p += tempBufIndex;
	bytesLeft -= tempBufIndex;

	/* Print the data */
	for (i = 0; i < traceLen && bytesLeft > 2; i++) {
		tempBufIndex = snprintf(p, bytesLeft, "%.2x|", traceMsg[i]);
		p += tempBufIndex;
		bytesLeft -= tempBufIndex;
	}

	tempBufIndex = snprintf(p, bytesLeft, "\r\n\r\n");
	p += tempBufIndex;
	bytesLeft -= tempBufIndex;

	fwrite(buf, 1, strlen(buf), lthTrcCtx->fp);
	lthTrcCtx->bytesWritten += strlen(buf);
	retVal = TRUE;

	return retVal;
}


/**
 * @param   msg  time out indication msg
 * @details This function checks type of the timer and takes the corresponding action.
 */
void lthProcTimeoutInd(IN Msg *msg) {

	MsgTimeoutInd *msgTimeoutInd = (MsgTimeoutInd*) msg->data;
	LthTraceContext *lthTrcCtx = getLthTraceContext();
	switch (msgTimeoutInd->timerDesc.timerType) {

	case TIMERNAME_WAITTRACEDURATION: {
		logInfo("%s",
				"Timer expired: TIMERNAME_WAITTRACEDURATION: Tracing is completed for expected duration");
		lthTrcCtx->isTraceOn = FALSE;

		TraceHistory *traceHistory = (TraceHistory*) getRowOfTable(TRACEHISTORY,
				lthTrcCtx->currentListIndex);
		traceHistory->traceEndCause = TRACEENDCAUSE_TIMEDURATIONELAPSED;
		lthTrcCtx->bytesWritten = 0;
		getAndFillDateAndTimeStamp(MAX_SIZE_TRACEHISTORY_TRACESTOPPEDAT,
				&traceHistory->traceStoppedAt[0]);
		stopProtocolTrace(lthTrcCtx);

	}
		break;

	default: {
		BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType,
				"LTH Timeout")

	}
		break;
	}
}


/**
 * @param   msg  trace indication msg
 * @details This function processes the received trace ind from RRM modules.
 */
void lthProcTraceInd(IN Msg *msg) {

	MsgSmLthTrcInd *trcInd = (MsgSmLthTrcInd *) msg->data;
	LthTraceContext *lthTrcCtx = getLthTraceContext();
	SINT32 numBytesSent = 0;

	TraceStatus *traceStatus = (TraceStatus*) getFirstRowOfTable(TRACESTATUS);
	traceStatus->traceMessageCounter++;

	if (lthTrcCtx->currentTraceConfig.traceDestination
			!= TRACEDESTINATION_RELAYTOREMOTEHOST) {

		if (TRUE EQ IsSpaceAvailableinTraceFile(MAX_TRACE_FILE_LINE_SIZE)) {

			lthWriteTraceMessageToFile(trcInd->traceLen, trcInd->traceMsg,
					trcInd->timeVal, getTraceModString(trcInd->protocolName));
		} else {
			lthTrcCtx->maxTraceFileSizeReached = TRUE;
			TraceHistory *traceHistory = (TraceHistory*) getRowOfTable(
					TRACEHISTORY, lthTrcCtx->currentListIndex);
			traceHistory->traceEndCause = TRACEENDCAUSE_MAXTRCFILESIZEREACHED;
			lthTrcCtx->bytesWritten = 0;
			getAndFillDateAndTimeStamp(MAX_SIZE_TRACEHISTORY_TRACESTOPPEDAT,
					&traceHistory->traceStoppedAt[0]);
			stopLthProcedureTimer(TIMERNAME_WAITTRACEDURATION);
			stopProtocolTrace(lthTrcCtx);
		}
	} else {
		/* Send Message to Remote Host Wireshark using UDP socket */
		numBytesSent = sendUdpMsg(lthTrcCtx->sockFd,
				(UINT8*) &lthTrcCtx->currentTraceConfig.traceDestIPAddress,
				lthTrcCtx->currentTraceConfig.traceDestUdpPort,
				trcInd->traceMsg, trcInd->traceLen);
	}

}
/**
 * @param traceLen   Length of trace message
 * @param traceMsg   Trace message
 * @param timeVal    Time Stamp
 * @param moduleName Module name
 * @details      This function write to .txt file in folder /picobs_mnt/rel/inst_X/trace
 */
void lthWriteTraceMessageToFile( IO TraceLen traceLen, IO UINT8 *traceMsg,
		IO struct timespec timeVal, IN UINT8 *moduleName) {
	LthTraceContext *trcCtx = NULL;

	trcCtx = getLthTraceContext();

	if (trcCtx->fp EQ NULL ) {
		BS_ERROR_TRC_FILE_NOT_OPENED()
		;
		return;
	} else {
		/* The below function will not write in the file, if size of the file exceeds the max limit
		 * Hence will return FALSE */
		if (FALSE
				EQ lthWriteTraceToFile(&timeVal, traceLen, traceMsg,
						moduleName)) {
			/* close the trace file */
			if (trcCtx->fp NOTEQ NULL ) {
				if (EOF EQ fclose(trcCtx->fp)) {
					perror("fclose");
				}
				trcCtx->fp = NULL;
			}
		}
	}
}

/**
 * @param   msg  set trace msg
 * @details      This function sets the message trace level
 */
void lthProcSetMsgTrace(IN Msg *msg) {
	MsgBsmLthSetMsgTrace *msgTrace = (MsgBsmLthSetMsgTrace*) msg->data;
	MsgDesc *msgDesc = getMsgDesc(msgTrace->msgId);

	/* Set the values in the table */
	msgDesc->msgTraceLevel = msgTrace->msgTraceLevel;
	msgDesc->msgTraceDirection = msgTrace->msgTraceDirection;
}

/**
 * @param   msg        Trace Start Succ message received from SM
 * @details This function processes TRACE_START_SUCC message.
 */
void lthProcTraceStartSucc(IN Msg *msg) {

	LthTraceContext *lthTrcCtx = getLthTraceContext();
	MsgSmLthTraceCntrlCfm *cntrlCfm = (MsgSmLthTraceCntrlCfm *) msg->data;
	struct timeval tv;
	struct timezone tz;
	struct tm* tm;
    logInfo("Trace Start Succ Received from %s-%d", getTraceModString(cntrlCfm->protocolName), cntrlCfm->traceCtrlConfirm);
	clearBytes(&lthTrcCtx->traceStartTime, sizeof(LthTime));

	TraceStatus *traceStatus = (TraceStatus*) getFirstRowOfTable(TRACESTATUS);

	lthTrcCtx->numOfTraceCtrlRspRcvd++;

	if ((lthTrcCtx->numOfTraceCtrlRspRcvd EQ lthTrcCtx->numOfTraceCtrlReqSent)) {

		if (!lthTrcCtx->dynamicConfigChange) {
			/* Update status table */

			traceStatus->traceId = lthTrcCtx->currentListIndex;
			traceStatus->traceState = TRACESTATE_TRACEON;
			traceStatus->traceDurationConfiguredInMins =
					getTraceDurationFrmEnum(lthTrcCtx->traceDuration);
			traceStatus->traceDurationElapsedInMins = 0;
			traceStatus->traceMessageCounter = 0;
			traceStatus->traceDestination =
					lthTrcCtx->currentTraceConfig.traceDestination;

			/* Update the trace start time in LTH CTX*/

			gettimeofday(&tv, &tz);
			tm = localtime(&tv.tv_sec);

			lthTrcCtx->traceStartTime.tm_min = tm->tm_min;
			lthTrcCtx->traceStartTime.tm_hour = tm->tm_hour;

			/* Start filling trace history table */

			TraceHistory *traceHistory = (TraceHistory*) getRowOfTable(
					TRACEHISTORY, lthTrcCtx->currentListIndex);

			traceHistory->traceId = lthTrcCtx->currentListIndex;
			getAndFillDateAndTimeStamp(MAX_SIZE_TRACEHISTORY_TRACESTARTEDAT,
					&traceHistory->traceStartedAt[0]);
			snprintf(&traceHistory->traceStoppedAt[0],
					MAX_SIZE_TRACEHISTORY_TRACESTOPPEDAT, "00000000000000000");
			traceHistory->traceDurationElapsedInMins = getTraceDurationFrmEnum(
					lthTrcCtx->traceDuration);
			traceHistory->traceDestination =
					lthTrcCtx->currentTraceConfig.traceDestination;
			copyBytes(lthTrcCtx->traceFileName,
					MAX_SIZE_TRACEHISTORY_TRACEFILENAME,
					traceHistory->traceFileName);

			startTraceDurationTimer(lthTrcCtx->traceDuration,
					&lthTrcCtx->timersInfo);

		} else {

			lthTrcCtx->dynamicConfigChange = FALSE;

		}
		lthFillAndSendSetObjSucc();

	}
}

/**
 * @param   msg        Message received from SM
 * @details This function processes TRACE_STOP_SUCC message.
 */
void lthProcTraceStopSucc(IN Msg *msg) {

	MsgSmLthTraceCntrlCfm *cntrlCfm = (MsgSmLthTraceCntrlCfm *) msg->data;

	LthTraceContext *lthTrcCtx = getLthTraceContext();

	lthTrcCtx->numOfTraceCtrlRspRcvd++;

    logInfo("Trace Stop Succ Received from %s-%d", getTraceModString(cntrlCfm->protocolName), cntrlCfm->traceCtrlConfirm);
	if ((lthTrcCtx->numOfTraceCtrlRspRcvd EQ lthTrcCtx->numOfTraceCtrlReqSent)) {
		{
		    if (lthTrcCtx->currentListIndex < 10) {
			    lthTrcCtx->currentListIndex++;
		    } else {
			    lthTrcCtx->currentListIndex = 0;
		    }

			if (!lthTrcCtx->dynamicConfigChange) {

				if (lthTrcCtx->currentTraceConfig.traceDestination
						!= TRACEDESTINATION_RELAYTOREMOTEHOST) {

					if (lthTrcCtx->fp NOTEQ NULL ) {
						if (EOF EQ fclose(lthTrcCtx->fp)) {
							perror("fclose");
							lthFillAndSendSetObjFail(
									BS_ERR_TRACE_FILE_CLOSE_ERROR);
						}
						lthTrcCtx->fp = NULL;

					} else {
						BS_ERROR_TRC_FILE_NOT_OPENED()
						;
					}
				} else {

					lthTrcCtx->sockFd = -1;
				}

				lthTrcCtx->isTraceOn = FALSE;
				lthTrcCtx->bytesWritten = 0;
				lthTrcCtx->traceStartTime.tm_hour = 0;
				lthTrcCtx->traceStartTime.tm_min = 0;
				/* Update status table */
				initTraceStatusTable();

			} else {
				lthTrcCtx->dynamicConfigChange = FALSE;
			}
			if (!(lthTrcCtx->maxTraceFileSizeReached))
				lthFillAndSendSetObjSucc();
			else {
				lthTrcCtx->maxTraceFileSizeReached = FALSE;
			}
		}
	}
}

/**
 * @param   msg        Message received from SM
 * @details This function processes TRACE_START_FAIL message.
 */
void lthProcTraceStartFail(IN Msg *msg) {

	MsgSmLthTraceCntrlCfm *cntrlCfm = (MsgSmLthTraceCntrlCfm *) msg->data;
	LthTraceContext *lthTrcCtx = getLthTraceContext();

	BS_ERROR_TRACE_CTRL_CONFIRM_FAILED(
			getTraceModString(cntrlCfm->protocolName),
			cntrlCfm->traceCtrlConfirm);
	if (lthTrcCtx->currentTraceConfig.traceDestination
			!= TRACEDESTINATION_RELAYTOREMOTEHOST) {

		if (lthTrcCtx->fp NOTEQ NULL ) {
			if (EOF EQ fclose(lthTrcCtx->fp)) {
				perror("fclose");
				lthFillAndSendSetObjFail(BS_ERR_TRACE_FILE_CLOSE_ERROR);
			}
			lthTrcCtx->fp = NULL;

		} else {
			BS_ERROR_TRC_FILE_NOT_OPENED()
			;
		}
	} else {

		lthTrcCtx->sockFd = -1;
	}
	lthFillAndSendSetObjFail(BS_ERR_TRACE_START_FAILED);

}

/**
 * @param   msg        Message received from SM
 * @details This function processes TRACE_STOP_FAIL message.
 */
void lthProcTraceStopFail(IN Msg *msg) {

	MsgSmLthTraceCntrlCfm *cntrlCfm = (MsgSmLthTraceCntrlCfm *) msg->data;
	LthTraceContext *lthTrcCtx = getLthTraceContext();
	BS_ERROR_TRACE_CTRL_CONFIRM_FAILED(
			getTraceModString(cntrlCfm->protocolName),
			cntrlCfm->traceCtrlConfirm);

	if (lthTrcCtx->currentTraceConfig.traceDestination
			!= TRACEDESTINATION_RELAYTOREMOTEHOST) {

		if (lthTrcCtx->fp NOTEQ NULL ) {
			if (EOF EQ fclose(lthTrcCtx->fp)) {
				perror("fclose");
				lthFillAndSendSetObjFail(BS_ERR_TRACE_FILE_CLOSE_ERROR);
			}
			lthTrcCtx->fp = NULL;

		} else {
			BS_ERROR_TRC_FILE_NOT_OPENED()
			;
		}
	} else {

		lthTrcCtx->sockFd = -1;
	}
	lthFillAndSendSetObjFail(BS_ERR_TRACE_STOP_FAILED);

}

/**
 * @param bsErrorValue    Error code
 * @details This function creates a message MSG_LTH_BSM_SET_OBJ_FAIL, fills the
 * bsErrorValue and sends the message to BSM.
 */
void lthFillAndSendSetObjFail(IN BsErrorValue bsErrorValue) {
	SetObjRspFail rspFail;
	EncBuf encBuf;
	initEncBuf( &encBuf);
	clearBytes(&rspFail, sizeof(SetObjRspFail));

	LthTraceContext *lthTrcCtx = getLthTraceContext();

	rspFail.tlvArray.numTlvs = lthTrcCtx->setObjReq.numTlvs;
	copyBytes(&(lthTrcCtx->setObjReq.cmdRef), sizeof(CmdRef),
			&(rspFail.cmdRef));
	copyBytes(&(lthTrcCtx->setObjReq.tlvInfo),
			sizeof(TlvInfo)*(lthTrcCtx->setObjReq.numTlvs),
			&(rspFail.tlvArray.tlvInfo));
	copyBytes(&(lthTrcCtx->setObjReq.tlvInfo[0].tag), sizeof(TagInfo),
			&(rspFail.setErrRsp.failedTagInfo));

	rspFail.setErrRsp.errRspCode = bsErrorValue;

	/* encode the message */
	encSetObjRspFailMsg(&rspFail, &encBuf);
	createAndSendMsg(MSG_LTH_BSM_SET_OBJ_FAIL, (UINT8*) &encBuf.value,
			encBuf.len);
}

/**
 * @details This function creates a message MSG_LTH_BSM_SET_OBJ_SUCC, fills the
 * parameters and sends the message to BSM.
 */
void lthFillAndSendSetObjSucc() {
	SetObjRspSucc rspSucc;
	EncBuf encBuf;
	initEncBuf( &encBuf);
	clearBytes(&rspSucc, sizeof(SetObjRspSucc));

	LthTraceContext *lthTrcCtx = getLthTraceContext();

	rspSucc.tlvArray.numTlvs = lthTrcCtx->setObjReq.numTlvs;
	copyBytes(&(lthTrcCtx->setObjReq.cmdRef), sizeof(CmdRef),
			&(rspSucc.cmdRef));
	copyBytes(&(lthTrcCtx->setObjReq.tlvInfo),
			sizeof(TlvInfo)*(lthTrcCtx->setObjReq.numTlvs),
			&(rspSucc.tlvArray.tlvInfo));
	/* encode the message */
	encSetObjRspSuccMsg(&rspSucc, &encBuf);
	createAndSendMsg(MSG_LTH_BSM_SET_OBJ_SUCC, (UINT8*) &encBuf.value,
			encBuf.len);

}


/**
 * @file      fth_timers.c
 * @brief     This file contains Timer handling functions of FTH modules
 * @details   Copyright Powerwave Technologies, Inc., 2010
 *
 * @author    Praneeth
 * @date      Oct 2010
 *
 *
 */

#include "fth.h"


/**
 * @param   timerType  Type of timer
 * @details            This function creates and starts RSP Timeout Timer.
 **/
void fthStartTimer(
		IN TimerName         timerType,
		IN SINT32						 duration
		)
{
	Index       timerIndex=0;
	FthTimersInfo      *timerInfo = getFthTimersInfo();

	for ( timerIndex = 0; timerIndex< timerInfo->totalTimersInMod; timerIndex++ )
	{
		if( timerType EQ timerInfo->modTimerInfo[timerIndex].timerDesc.timerType )
		{
			if(timerType EQ TIMERNAME_WAITSCHEDULEDFILETRANSFERTOSTART)
			{
				timerInfo->modTimerInfo[timerIndex].timerDesc.timeSpec.duration = duration;
			}
			break;
		}
	}
  
	/* Create and start Timer */
	timerInfo->modTimerInfo[timerIndex].timerId = createAndStartTimer( &timerInfo->modTimerInfo[timerIndex].timerDesc , NULL, NULL);
}

/**
 * @param   type  Type of timer
 * @details       This function is used to stop RSP Timeout Timer.
 **/
void  fthStopTimer(
		IO TimerName   type
		)
{
	FthTimersInfo    *timerInfo = getFthTimersInfo();
	Index           timerIndex;

	for ( timerIndex=0; timerIndex< timerInfo->totalTimersInMod; timerIndex++ )
	{
		if (timerInfo->modTimerInfo[timerIndex].timerDesc.timerType EQ type )
		{
			stopTimer( timerInfo->modTimerInfo[timerIndex].timerId );
			break;
		}
	}
}

/**
 * @param   totalTimers total timers used in FTH module
 * @param   config      pointer to timer configuration structure with timer information
 * @details             This function is used to update entry to Status table after receiving Response from BSM
 */
void fthStoreTimersInCtx(
		IN UINT8         totalTimers,
		IN TimersConfig  *config
		)
{
	Index          i =0, timerArrayIndex = 0;
	FthTimersInfo  *timerInfo = getFthTimersInfo();

	MsgId       timeOutMsgId = MSG_TIMEKEEPER_FTH_TIMEOUT_IND;

	for ( i=0; i < totalTimers; i++ )
	{
		if ( config->moduleId EQ MODID_FTH )
		{
			timerInfo->modTimerInfo[timerArrayIndex].timerDesc.moduleId         = config->moduleId;
			timerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeSpec.duration = config->duration;
			timerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeSpec.timeUnit = config->timeUnit;
			timerInfo->modTimerInfo[timerArrayIndex].timerDesc.timerType         = config->timerName;
			timerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeoutMsgId      = timeOutMsgId;
			timerInfo->totalTimersInMod++;
			timerArrayIndex++;
		}
		config++;
	}
}

/**
 * @param   configFilePath This is the configuration path, either will be NULL / or some valid path
 * @details                This function will read the bs_timers Configuration file
 **/
void readFthTimersFromCfg(
		IN UINT8   *configFilePath
		)
{

	UINT8       isError = FALSE;
	UINT8       fullFilePath[MAX_BUFFER_SIZE];

	GenericTable  *tableFunc = getTableFunctions(TIMERSCONFIG);
	TimersConfig  *config = (TimersConfig*)getFirstRowOfTable(TIMERSCONFIG);
	clearBytes( &fullFilePath, sizeof(fullFilePath));
	constructFullPath ( configFilePath, "enb_pre_config.cfg", fullFilePath );

	if ( NULL EQ tableFunc->getReadFromFileFunc )
	{
		logError("%s","No getReadFromFileFunc for BSTIMERSCONFIG");
		exit(EXIT_FAILURE);
	}
	else
	{
		FthContext  *fthContext = getFthContext();
		fthContext->totalTimers = tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
		if ( isError EQ TRUE )
		{
			BS_ERROR_CONFIG_READ_FAILED()
			printAndEmptyErrorChain(&(getModuleContext()->errChain));
			exit(EXIT_FAILURE);
		}
		/* Copy FTH timers in FTH context */
		fthStoreTimersInCtx( fthContext->totalTimers, config );
	}
}

void fthProcTimerExpiryInAllStates(
	IN Msg           *msg,
	IO FthContext    *fthContext
	)
{
	MsgTimeoutInd *msgTimeoutInd= (MsgTimeoutInd*)msg->data;
	SetObjReq setReq;
  FileDownload fileDownload;
	setReq.tlvArray.numTlvs = fthContext->setObjReq.numTlvs;
	copyBytes(&(fthContext->setObjReq.cmdRef),sizeof(CmdRef),&(setReq.cmdRef));
	copyBytes(&(fthContext->setObjReq.tlvInfo),sizeof(TlvInfo)*(setReq.tlvArray.numTlvs),&(setReq.tlvArray.tlvInfo));

	GenericTable     *tableFunc = getTableFunctions(FILEDOWNLOAD);
	if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
	{
		tableFunc->getTlv2StructFunc( &setReq.tlvArray, &fileDownload );
	}

	if (fthContext->fthState EQ FTH_STATE_IDLE)
	{
		switch( msgTimeoutInd->timerDesc.timerType )
		{
			case TIMERNAME_WAITSCHEDULEDFILETRANSFERTOSTART:
			{
				fthStartFileDownload(&fileDownload,fthContext);
			}
			break;
			default:
				BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType,getFthProceduralStateName(fthContext->fthState));
				break;
		}
	}
	else
	{
		BS_ERROR_TIMER_EXPIRED_IN_WRONG_STATE("FTH", "TIMERNAME_WAITSCHEDULEDFILETRANSFERTOSTART", fthContext->fthState);
	}
}
				 
TBOOL checkAndFillTimeDuration (
	char *scheduleDateAndTime,
	SINT32 *duration
	)
{
	char temp[4];
	struct tm scheduledTime;
	strncpy(temp,&scheduleDateAndTime[0],4);
	scheduledTime.tm_year=atoi(temp)-1900;
	memset(temp,0,4);
	strncpy(temp,&scheduleDateAndTime[4],2);
	scheduledTime.tm_mon=atoi(temp)-1;
	memset(temp,0,4);
	strncpy(temp,&scheduleDateAndTime[6],2);
	scheduledTime.tm_mday=atoi(temp);
	memset(temp,0,4);
	strncpy(temp,&scheduleDateAndTime[8],2);
	scheduledTime.tm_hour=atoi(temp);
	memset(temp,0,4);
	strncpy(temp,&scheduleDateAndTime[10],2);
	scheduledTime.tm_min=atoi(temp);
	memset(temp,0,4);
	strncpy(temp,&scheduleDateAndTime[12],2);
	scheduledTime.tm_sec=atoi(temp);
	*duration=mktime(&scheduledTime)-time(NULL);
	if ( (*duration < 0) || (*duration > SCHEDULE_DOWNLOAD_LIMIT) )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


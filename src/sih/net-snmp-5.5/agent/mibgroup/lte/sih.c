/**
 * @file      sih.c
 * @details   Copyright Powerwave Technologies, Inc., 2010
 * @brief     This module contains the SIH module related routines.
 *  -------------------------------
 * |   EMS <-> SIH <-> BSM        |
 *  --------------------------------
 *
 * Author     :  Prashant Gupta
 * Date       :  31-08-2010
 * Description:  1.Created basic version.
 *
 *
 */
#include "bs_modules.h"
#include "bs_events.h"
#include "tlv_enc_dec.h"

#include "sih.h"
#include "bsm_sih_if.h"
#include "print_noti.h"
#include "start_indication.h"

#include "syslog.h"


/* globals */
SihContext    g_SihCtx;
SihStateMachine      sihStateMachine[SIH_STATE_MAX][MSG_ID_MAX];
SINT32 g_sihReqSent  = 0;
SINT32 g_sihRespRecd = 0;
SihGetResponse  sihGetResponse[TABLE_MAX_ID];
/*
 * Here errCode and errTableId are used to support BSM Error Codes in netsnmp Agent.
 * If BSM Sends MSG_BSM_SIH_GET_OBJ_FAIL to SIH then 
 *  	errCode is assigned with table Id.
 * 	errTableId is request Table Id. (This added to handle the snmp GET-NEXT Request)
 */
ErrRspCode 	errCode = 0;
int		errTableId = 0;

extern int reconfig;

/**
* @details 
*          This functions returns the SIH Context
*/
SihContext  *getSihContext()
{
  return (&g_SihCtx);
}

/* SIH Module Timers info */
#define getSihTimersInfo()              &(g_SihCtx.sihTimersInfo)

/**
 * @param   timerType  Type of timer
 * @details            This function creates and starts RSP Timeout Timer.
 **/
void startSihRspTimer(
  IN TimerName         timerType
  )
{
  Index       timerIndex=0;
  String50    buf;
  clearBytes( &buf, sizeof(String50));

  SihTimersInfo      *timerInfo = getSihTimersInfo();

  for ( timerIndex = 0; timerIndex< timerInfo->totalTimersInMod; timerIndex++ )
  {
    if( timerType EQ timerInfo->modTimerInfo[timerIndex].timerDesc.timerType )
        break;
  }
  timerInfo->modTimerInfo[timerIndex].timerDesc.userInfo = SIH_COMMANDS;

  /* Create and start Timer */
  timerInfo->modTimerInfo[timerIndex].timerId = createAndStartTimer( &timerInfo->modTimerInfo[timerIndex].timerDesc, NULL, NULL );
}

/**
 * @param   type  Type of timer
 * @details       This function is used to stop RSP Timeout Timer.
 **/
void  stopSihRspTimer(
  IO TimerName   type
  )
{
  SihTimersInfo    *timerInfo = getSihTimersInfo();
  Index           timerIndex;
  String50         buf;

  clearBytes( &buf, sizeof(String50));

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
* @details This is the routine for sending the hello ack message response.
*/
static void sihCreateAndSendHelloAck(
  IN Msg          *msg
  )
{
  MsgPrmModHello     *hello = (MsgPrmModHello*)msg->data;
  MsgModPrmHelloAck   helloAck;
  clearBytes( &helloAck, sizeof(MsgModPrmHelloAck) );
  helloAck.helloAckSeqNum = hello->helloSeqNum;

  logInfo("%s","SIH Sending MSG_SIH_PRM_HELLO_ACK ... ");
  createAndSendMsg( MSG_SIH_PRM_HELLO_ACK, (UINT8*)&helloAck, sizeof(MsgModPrmHelloAck) );

}


/**
* @details 
*          This is the routine for handling the hello message message in all
*          state.
* @param msg is used to store the message.
* @param sihContext contains the sih context information.
*/

static inline void sihProcHelloReqInAllStates(
    IN Msg          *msg,
    IO SihContext   *sihContext
    )
{
    logInfo("%s", "SIH Received MSG_PRM_SIH_HELLO. ");
	sihCreateAndSendHelloAck(msg);
}

/**
* @details 
*          This is the routine for handling the software exit message.
* @param sihContext contains the sih context information.
*/

static void sihExit(
    IO SihContext   *sihContext
    )
{
  logInfo("%s", "SIH ... modExit");
  modExit (MODID_SIH, &(getModuleContext()->exeName[0]));
  exit(EXIT_SUCCESS);
}


/**
* @details 
*          This is the routine for handling the software exit message in all 
*          state.
* @param msg is used to store the message.
* @pa/ram sihContext contains the sih context information.
*/

static inline void sihProcExitReqInAllStates(
    IN Msg          *msg,
    IO SihContext   *sihContext
    )
{
  logInfo("%s","SIH Received MSG_PRM_SIH_EXIT_CMD. ");
  sihExit(sihContext);
}
/*
 * When BSM Send's Config request to SIH, then SIH will change IpAddress and Community String in snmpd.conf
 * and modify reconfig to 1 which makes reconfiguring of net-snmp Agent.
 */
static inline void sihProcConfigReqInAllStates(
    IN Msg          *msg,
    IO SihContext   *sihContext
    )
{
  	MsgBsmSihInitConfigReq msgBsmSihConfigReq;
  	FILE *fp;
	FilePath configPath;
  	String100 buf;
	String100 temp;
	int modified;

        logInfo("Received Message=%d, MSG_BSM_SIH_CONFIG_REQ", msg->msgId);
	
	/* Fetching EMS Address and community String from MsgBsmSihInitConfigReq Object */
	clearBytes(&msgBsmSihConfigReq, sizeof(MsgBsmSihInitConfigReq));
        memcpy(&msgBsmSihConfigReq, &msg->data[0], sizeof(MsgBsmSihInitConfigReq));

	/* copying current configuration transID to sihContext */
        sihContext->curRecvTransId = msgBsmSihConfigReq.transId;

  	/* Code for getting snmpd.conf Path */
	sprintf(configPath,"%s%s",sihContext->configDir,"/snmpd.conf");
	
  	/* Opened the File in reading and writing mode */
  	if((fp = fopen(configPath,"r+")) EQ NULL ){
		logInfo("%s file is not found in config Directory",configPath);
		return;
  	}
  	else {
		logInfo("%s is opened for Configuration Changes",configPath);
  	}

 	while(fgets(buf,sizeof(buf),fp) NOTEQ NULL)
  	{
		modified = 0;
		if(buf[0] EQ '#' || buf[0] EQ '\n')
		continue;
			
		memset(temp,0,sizeof(String100));

          	if(strstr(buf, "informsink")){
			fseek(fp, -(strlen(buf)), SEEK_CUR);
			sprintf(temp,"informsink %s %s\n",msgBsmSihConfigReq.emsAddr,msgBsmSihConfigReq.snmpCommunityString);
			fprintf(fp,"%s",temp);
			modified = 1;
		}		
		if(strstr(buf, "trapcommunity")){
			fseek(fp, -(strlen(buf)), SEEK_CUR);
			sprintf(temp,"trapcommunity %s \n",msgBsmSihConfigReq.snmpCommunityString);
			fprintf(fp,"%s",temp);
			modified = 1;
		}
		if(strstr(buf, "rwcommunity")){
			fseek(fp, -(strlen(buf)), SEEK_CUR);
                        sprintf(temp,"rwcommunity %s \n",msgBsmSihConfigReq.snmpCommunityString);
                        fprintf(fp,"%s",temp);
			modified = 1;
		}
                if(strlen(temp) < strlen(buf) && modified EQ 1){
 	               int diff = strlen(buf) - strlen(temp);
        	        fprintf(fp,"%*s",diff,"");
                }		
 	}	
  
  	fclose(fp);
	
	reconfig = 1;

	snmp_log(LOG_INFO, "Reconfig is Set to 1\n");
}

void refreshAgentConfig(void)
{
	SihContext *sihContext = getSihContext();
	MsgSihBsmInitConfigSucc msgSihBsmInitConfigSucc;

	clearBytes(&msgSihBsmInitConfigSucc, sizeof(msgSihBsmInitConfigSucc));
	msgSihBsmInitConfigSucc.transId = sihContext->curRecvTransId;

	createAndSendMsg( MSG_SIH_BSM_INIT_CONFIG_SUCC, (UINT8*)&msgSihBsmInitConfigSucc, sizeof(MsgSihBsmInitConfigSucc));
	
	/* Reseting the curRecvTransID to 0 After MSG_SIH_BSM_INIT_CONFIG_SUCC is sent to BSM */
	sihContext->curRecvTransId = 0;

	logInfo("%s", "Agent ReConfig Done....");
}


static inline void sihProcNotiIndInAllStates(
    IN Msg          *msgIn,
    IO SihContext   *sihContext
    )
{
  	NotiObj notiObj;
	int iRetVal = -1;
  
  	logInfo("%s","SIH Received MSG_BSM_SIH_NOTI_IND. ");

	clearBytes( &notiObj, sizeof(NotiObj));
   	decNotiObjMsg((UINT8*)&(msgIn->data[0]), msgIn->len,&notiObj);

        switch(notiObj.notiId)
	{
		case STRUCT_ENBREGISTRATIONREQ_ID:
		{
			logInfo("Received EMS Registration Notification ID = %d ", notiObj.notiId);
			iRetVal = send_enbRegistrationReq_trap(&notiObj);
			break;
		}
		case STRUCT_ENBOPERATIONALSTATECHANGED_ID:
		{
			logInfo("Received Operational State Change Notification ID = %d ", notiObj.notiId);
			iRetVal = send_enbOperationalStateChanged_trap(&notiObj);
			break;
		}
		case STRUCT_ENBADMINSTATECHANGED_ID:
		{
			logInfo("Received Admin State Change Notification ID = %d ", notiObj.notiId);
			iRetVal = send_enbAdminStateChanged_trap(&notiObj);
			break;
		}
		case STRUCT_NONEIGHBORCONFIGURED_ID:
		{
			logInfo("Received No Neighbor Configured Notification ID = %d ", notiObj.notiId);
			iRetVal = send_enbNoNeighborConfigured_trap(&notiObj);
			break;
		}
		case STRUCT_ALARMNOTIFICATION_ID: 
		{
			logInfo("Received Alarm Notification ID = %d ", notiObj.notiId);
			iRetVal = send_enbAlarmInd_trap(&notiObj);
			break;
		}
		case STRUCT_SWINSTALLFAILED_ID:
		{
			logInfo("Received Sw Install Failed Notification ID = %d ", notiObj.notiId);
			iRetVal = send_swInstallFailed_trap(&notiObj);
			break;
		}
		case STRUCT_SWINSTALLSUCCESS_ID:
		{
			logInfo("Received Sw Install Successful Notification ID = %d ", notiObj.notiId);
			iRetVal = send_swInstallSuccess_trap(&notiObj);
			break;
		}
		case STRUCT_COUNTERFILEAVAILABLE_ID:
		{
			logInfo("Received Counter File Available Notification ID = %d ", notiObj.notiId);
			iRetVal = send_counterFileAvailable_trap(&notiObj);
			break;
		}
		case STRUCT_MMERESET_ID:
		{
			logInfo("Received MME Reset Notification ID = %d ", notiObj.notiId);
			iRetVal = send_mmeReset_trap(&notiObj);
			break;
		}
		case STRUCT_FILEUPLOADCOMPLETED_ID:
		{
			logInfo("Received File Upload Completed Notification ID = %d ", notiObj.notiId);
			iRetVal = send_fileUploadCompleted_trap(&notiObj);
			break;
		}
		case STRUCT_FILEUPLOADFAILED_ID:
		{
			logInfo("Received File Upload Failed Notification ID = %d ", notiObj.notiId);
			iRetVal = send_fileUploadFailed_trap(&notiObj);
			break;
		}
		case STRUCT_CONFIGURATIONCHANGED_ID:
		{
			logInfo("Received Config Change Notification ID = %d ", notiObj.notiId);
			iRetVal = send_configurationChanged_trap(&notiObj);
			break;
		}
		case STRUCT_SWACTIVATIONFAILED_ID:
		{
			logInfo("Received Sw Activation Failed Notification ID = %d ", notiObj.notiId);
			iRetVal = send_swActivationFailed_trap(&notiObj);
			break;
		}
		case STRUCT_SWACTIVATIONSUCCESS_ID:
		{
			logInfo("Received Sw Activation Successful Notification ID = %d ", notiObj.notiId);
			iRetVal = send_swActivationSuccess_trap(&notiObj);
			break;
		}
		case STRUCT_FILEDOWNLOADCOMPLETED_ID:
		{
			logInfo("Received File Download Completed Notification ID = %d ", notiObj.notiId);
			iRetVal = send_fileDownloadCompleted_trap(&notiObj);
			break;
		}
		case STRUCT_FILEDOWNLOADFAILED_ID:
		{
			logInfo("Received File Download Failed Notification ID = %d ", notiObj.notiId);
			iRetVal = send_fileDownloadFailed_trap(&notiObj);
			break;
		}
		case STRUCT_CONFIGURATIONSAVED_ID:
		{
			logInfo("Received Configuration Saved Notification ID = %d ", notiObj.notiId);
			iRetVal = send_configurationSaved_trap(&notiObj);
			break;
		}
		case STRUCT_CONFIGURATIONROLLBACK_ID:
		{
			logInfo("Received Configuration Rollback Notification ID = %d ", notiObj.notiId);
			iRetVal = send_configurationRollBack_trap(&notiObj);
			break;
		}
		case STRUCT_COUNTERCOLLECTIONFAILED_ID:
		{
			logInfo("Received Counter Collection Failure Notification ID = %d ", notiObj.notiId);
			iRetVal = send_counterCollectionFailed_trap(&notiObj);
			break;
		}
		case STRUCT_MESSAGETRACECOMPLETED_ID:
		{
			logInfo("Received Trace Completed Notification ID = %d ", notiObj.notiId);
			iRetVal = send_messageTraceCompleted_trap(&notiObj);
			break;
		}
		case STRUCT_MESSAGETRACEFAILED_ID:
		{
			logInfo("Received Trace Failed Notification ID = %d ", notiObj.notiId);
			iRetVal = send_messageTraceFailed_trap(&notiObj);
			break;
		}
		case STRUCT_ENBLOCATIONCHANGED_ID:
		{
			logInfo("Received Location Change Notification ID = %d ", notiObj.notiId);
			iRetVal = send_enbLocationChanged_trap(&notiObj);
			break;
		}
		default:
		logInfo("Received Undefined Notification ID = %d from BSM.", notiObj.notiId);
			
	}/* End of switch */
  
	if(iRetVal NOTEQ 0)
	{
		logInfo("Could not send  Notification ID = %d to EMS.", notiObj.notiId);
	}

}

/**
* @details 
*          This is the routine which handles get response from SIH Get Processor
*          and prepare the data structure for SNMP GET response for EMS. 
* @param   tlvArray is used to store the TLV array.
*/
TBOOL sendSnmpGetResponsetoEms (
  IO TlvArray    * tlvArray
  )
{
 TBOOL status = FALSE;

 logInfo("Get Message response of Table Id %d", tlvArray->tlvInfo[0].tag.tableId);

  if ( sihGetResponse[tlvArray->tlvInfo[0].tag.tableId].fp  NOTEQ NULL )
  {
    status = sihGetResponse[tlvArray->tlvInfo[0].tag.tableId].fp(tlvArray);
  }
  else
   {
	  logInfo("%s", "Get Message is not handled, Table Id %d",
           tlvArray->tlvInfo[0].tag.tableId);
    status = FALSE;
   }
 if ( status EQ TRUE )
	 logInfo("%s", "sihGetResponse Succedded");
 else
	 logError("%s","sihGetResponse Failed");

 return status;
}

/**
* @details 
*             This is the routine for handling the 
*             message and updating logConfigurations.
* @param msg  Message structure
*/
static inline void sihProcSetLogCfgInAllStates( 
  IN Msg *msg 
)
{
   MsgBsmSihSetLogCfg *updateLogConfig=(MsgBsmSihSetLogCfg*)msg->data;
   LogContext  *logContext  = getLogContext();
   copyBytes(updateLogConfig, sizeof(LogConfig), &(logContext->logConfig));
   logInfo("%s", "Loglevel is set to new values");
}

/**
* @details 
*          This is the routine for initialising the handler for handling the 
*          message and updating the state.
*/

void initSihStateMachine( void )
{
  int i,j;

#ifndef PWAV_SIH
  /* Initialise the Function pointer to process Invalid Message
   * Then fill the valid processing functions
   */
  for(i=0; i< SIH_STATE_MAX; i++)
  {
    for(j=0; j<MSG_ID_MAX; j++ )
    {
      sihStateMachine[i][j].fp = NULL;
    }
  }

  InitSihStateMachine( SIH_STATE_INITIALIZING,  MSG_PRM_SIH_HELLO, 
                       sihProcHelloReqInAllStates, SIH_STATE_INITIALIZING );
  InitSihStateMachine( SIH_STATE_IDLE, 		MSG_PRM_SIH_HELLO, 
                       sihProcHelloReqInAllStates, SIH_STATE_IDLE );
  InitSihStateMachine( SIH_STATE_BUSY,   	MSG_PRM_SIH_HELLO, 
                       sihProcHelloReqInAllStates, SIH_STATE_BUSY );

  InitSihStateMachine( SIH_STATE_INITIALIZING, MSG_PRM_SIH_EXIT_CMD, 
                       sihProcExitReqInAllStates,  SIH_STATE_INITIALIZING );
  InitSihStateMachine( SIH_STATE_IDLE,         MSG_PRM_SIH_EXIT_CMD, 
                       sihProcExitReqInAllStates,  SIH_STATE_INITIALIZING );
  InitSihStateMachine( SIH_STATE_BUSY,         MSG_PRM_SIH_EXIT_CMD, 
                       sihProcExitReqInAllStates,  SIH_STATE_INITIALIZING );

  InitSihStateMachine( SIH_STATE_INITIALIZING,  MSG_BSM_SIH_SET_MSG_TRACE, 
                       sihProcSetTraceCfgInAllStates, SIH_STATE_INITIALIZING );
  InitSihStateMachine( SIH_STATE_IDLE,          MSG_BSM_SIH_SET_MSG_TRACE, 
                       sihProcSetTraceCfgInAllStates, SIH_STATE_IDLE );
  InitSihStateMachine( SIH_STATE_BUSY,          MSG_BSM_SIH_SET_MSG_TRACE, 
                       sihProcSetTraceCfgInAllStates, SIH_STATE_BUSY );

#endif /* PWAV_SIH */ 

  /* Initialise the Function pointer to process Invalid Message
   * Then fill the valid processing functions
   */
  for(i=0; i< TABLE_MAX_ID; i++)
   {
     sihGetResponse[i].fp = NULL;
   }  

   /* Command Tables */
   InitSihGetRespFuncs ( ENBADMIN,     sendEnbAdminSnmpGetResponse);
   InitSihGetRespFuncs ( FILEDOWNLOAD, sendFileDownloadSnmpGetResponse);
   InitSihGetRespFuncs ( FILEUPLOAD,   sendFileUploadSnmpGetResponse);
   InitSihGetRespFuncs ( SWACTIVATION, sendSwActivationSnmpGetResponse);
   InitSihGetRespFuncs ( SWINSTALL,    sendSwInstallSnmpGetResponse);
   InitSihGetRespFuncs ( TRACECONTROL, sendTraceControlSnmpGetResponse);
   InitSihGetRespFuncs ( CELLADMIN,    sendCellAdminSnmpGetResponse);

   /* Config Tables */
   InitSihGetRespFuncs ( ENBREGISTRATIONCONFIG, sendEnbRegConfigSnmpGetResponse);
   InitSihGetRespFuncs ( ENODEBCONFIG, sendEnbConfigSnmpGetResponse);
   InitSihGetRespFuncs ( CELLCONFIG, sendCellConfigSnmpGetResponse);
   InitSihGetRespFuncs ( SYSINFO1CONFIG, sendSysInfo1ConfigSnmpGetResponse);
   InitSihGetRespFuncs ( SYSINFO2CONFIG, sendSysInfo2ConfigSnmpGetResponse);
   InitSihGetRespFuncs ( SCHEDULERCONFIG, sendSchedulerConfigSnmpGetResponse);
   InitSihGetRespFuncs ( PHYCONFIG, sendPhyConfigSnmpGetResponse);
   InitSihGetRespFuncs ( RADIOHEADCONFIG, sendRadioHeadConfigSnmpGetResponse);
   InitSihGetRespFuncs ( QOSCONFIG, sendQosConfigSnmpGetResponse);
   InitSihGetRespFuncs ( EUTRAHOPROFILECONFIG, sendEutraHoConfigSnmpGetResponse);
   InitSihGetRespFuncs ( EUTRANEIGHCELLCONFIG, sendEutraNeighCellConfigSnmpGetResponse);
   InitSihGetRespFuncs ( TRACECONFIG, sendTraceConfigSnmpGetResponse);
   InitSihGetRespFuncs ( PMCOUNTERLOGCONFIG, sendPmCounterLogConfigSnmpGetResponse);
   InitSihGetRespFuncs ( LOGCONFIG, sendLogConfigSnmpGetResponse);
   InitSihGetRespFuncs ( SYSINFO3CONFIG, sendSysInfo3ConfigSnmpGetResponse);
   InitSihGetRespFuncs ( MMECONFIG, sendMmeConfigSnmpGetResponse);
   InitSihGetRespFuncs ( SONCONFIG, sendSonConfigSnmpGetResponse);
   InitSihGetRespFuncs ( SRSCONFIGDEDICATED, sendSrsConfigDedicatedSnmpGetResponse);
   InitSihGetRespFuncs ( SECURITYCONFIG, sendSecurityConfigSnmpGetResponse);
   InitSihGetRespFuncs ( UEEVENTHISTORYLOGCONFIG, sendUeEventHistoryLogConfigSnmpGetResponse);
   /* Status Tables */
   InitSihGetRespFuncs ( FILEDOWNLOADSTATUS, sendFileDownloadStatusSnmpGetResponse);
   InitSihGetRespFuncs ( FILEUPLOADSTATUS, sendFileUploadStatusSnmpGetResponse);
   InitSihGetRespFuncs ( SWACTIVATIONSTATUS, sendSwActivationStatusSnmpGetResponse);
   InitSihGetRespFuncs ( SWPACKAGESTATUS, sendSwPackageStatusSnmpGetResponse);
   InitSihGetRespFuncs ( COMPONENTSTATUS, sendComponentStatusSnmpGetResponse);
   InitSihGetRespFuncs ( ENBSTATUS, sendEnbStatusSnmpGetResponse);
   InitSihGetRespFuncs ( MACGENCOUNTERS, sendMacGenCountersSnmpGetResponse);
   InitSihGetRespFuncs ( RLCGENCOUNTERS, sendRlcGenCountersSnmpGetResponse);
   InitSihGetRespFuncs ( PDCPGENCOUNTERS, sendPdcpGenCountersSnmpGetResponse);
   InitSihGetRespFuncs ( GTPGENCOUNTERS, sendGtpGenCountersSnmpGetResponse);
   InitSihGetRespFuncs ( SCTPGENCOUNTERS, sendSctpGenCountersSnmpGetResponse);
   InitSihGetRespFuncs ( RADIOHEADSTATUS, sendRadioHeadStatusSnmpGetResponse);
   InitSihGetRespFuncs ( TRACESTATUS, sendTraceStatusSnmpGetResponse);
   InitSihGetRespFuncs ( FILEEVENTHISTORY, sendFileEventHistorySnmpGetResponse);
   InitSihGetRespFuncs ( SWINSTALLHISTORY, sendSwInstallHistorySnmpGetResponse);
   InitSihGetRespFuncs ( ACTIVEUESTATUS, sendActiveUeStatusSnmpGetResponse);
   InitSihGetRespFuncs ( ACTIVERABSTATUS, sendActiveRabStatusSnmpGetResponse);
   InitSihGetRespFuncs ( RABHISTORY, sendRabHistorySnmpGetResponse);
   InitSihGetRespFuncs ( UEEVENTHISTORY, sendUeEventHistorySnmpGetResponse);
   InitSihGetRespFuncs ( ACTIVEALARMSTATUS, sendActiveAlarmStatusSnmpGetResponse);
   InitSihGetRespFuncs ( ALARMHISTORY, sendAlarmHistorySnmpGetResponse);
   InitSihGetRespFuncs ( PMLOGFILESTATUS, sendPmLogFileStatusSnmpGetResponse);
   InitSihGetRespFuncs ( MODULELOGFILESTATUS, sendModuleLogFileStatusSnmpGetResponse);
   InitSihGetRespFuncs ( MODULESTATUS, sendModuleStatusSnmpGetResponse);
   InitSihGetRespFuncs ( ETHERNETCOUNTERS, sendEthernetCountersSnmpGetResponse);
   InitSihGetRespFuncs ( LOCATIONSTATUS, sendLocationStatusSnmpGetResponse);
   InitSihGetRespFuncs ( RRCGENCOUNTERS, sendRrcGenCountersSnmpGetResponse);
   InitSihGetRespFuncs ( RRMCOUNTERS, sendRrmCountersSnmpGetResponse);
   InitSihGetRespFuncs ( RRCCELLCOUNTERS, sendRrcCellCountersSnmpGetResponse);
   InitSihGetRespFuncs ( S1APGENCOUNTERS, sendS1apGenCountersSnmpGetResponse);
   InitSihGetRespFuncs ( S1APPEERNODECOUNTERS, sendS1apPeerNodeCountersSnmpGetResponse);
   /* InitSihGetRespFuncs ( LOGINSTATUS, sendLoginStatusSnmpGetResponse); */
   InitSihGetRespFuncs ( MACUPLANECOUNTERS, sendMacUplaneCountersSnmpGetResponse);
   InitSihGetRespFuncs ( PDCPUPLANECOUNTERS, sendPdcpUplaneCountersSnmpGetResponse);
   InitSihGetRespFuncs ( IPCOUNTERS, sendIpCountersSnmpGetResponse);
   InitSihGetRespFuncs ( LEDSTATUS, sendLedStatusSnmpGetResponse);
   InitSihGetRespFuncs ( GPSSTATUS, sendGpsStatusSnmpGetResponse);
   InitSihGetRespFuncs ( CELLSTATUS, sendCellStatusSnmpGetResponse);
   InitSihGetRespFuncs ( TRACEHISTORY, sendTraceHistorySnmpGetResponse);

   /* Pre Config Tables */
   InitSihGetRespFuncs ( TIMERSCONFIG, sendTimersConfigSnmpGetResponse);
   InitSihGetRespFuncs ( MSGTRACE, sendMsgTraceSnmpGetResponse);
   InitSihGetRespFuncs ( ENBADDRESSES, sendEnbAddressesSnmpGetResponse);
   InitSihGetRespFuncs ( EVENTTHRESHOLDS, sendEventThresholdsSnmpGetResponse);
}

/**
* @details 
*          This is the entry point for all messages coming into BSM to SIH.
* @param   msgIn is used to store the message.
*/

void sihMain(
    FR Msg *msgIn 
    )
{
  SihContext *sihContext = getSihContext();
#ifndef PWAV_SIH
  logInfo("Received Message=%d, in state=%d", msgIn->msgId, 
          sihContext->sihState );
  if( NULL NOTEQ sihStateMachine[sihContext->sihState][msgIn->msgId].fp )
  {
    sihStateMachine[sihContext->sihState][msgIn->msgId].fp( msgIn, sihContext);
    sihContext->sihState = 
        sihStateMachine[sihContext->sihState][msgIn->msgId].nextState;
  }
  else
  {
    logError("Unhandled Message Received; Message=%s, in state=%d", 
             getMsgName(msgIn->msgId), sihContext->sihState );
  }

  /* Free the received Message */
  freeMsgIn(msgIn);
#else

  logInfo("SIH Received Message=%d", msgIn->msgId);

  if (msgIn->msgId EQ MSG_PRM_SIH_HELLO)
      sihProcHelloReqInAllStates(msgIn, sihContext);
  else if (msgIn->msgId EQ MSG_PRM_SIH_EXIT_CMD)
      sihProcExitReqInAllStates(msgIn, sihContext);
  else if (msgIn->msgId EQ MSG_BSM_SIH_INIT_CONFIG_REQ)
      sihProcConfigReqInAllStates(msgIn, sihContext);
  else if (msgIn->msgId EQ MSG_BSM_SIH_NOTI_IND)
      sihProcNotiIndInAllStates(msgIn, sihContext);
  else if (msgIn->msgId EQ MSG_BSM_SIH_SET_LOG_CFG)
      sihProcSetLogCfgInAllStates(msgIn);

  /* Free the received Message */
  freeMsgIn(msgIn);
#endif /* PWAV_SIH */  
}


/**
* @details 
*          SIH Get Message Handler & Processor.
*          This is the routine which handles get request and response between 
*          SIH and BSM.
* @param sihTagInfo contains information about sih tags.
*/
TBOOL sihGetProcessor (
    TagInfo sihTagInfo
    )
{
    GetObjReq   getObjReq;
    EncBuf      encGetObjBuf;
    CmdRef      sihCmdRef;
    TBOOL       status = FALSE;
    static unsigned cmdRefNum = 0;
    UINT16      recvMsgRetVal = ERROR;
    GetObjRspSucc   rspSucc;
    Msg    *    msgIn = NULL;
    UINT32      msgId = MSG_UNDEFINED;    
    char *      getResponse = NULL;
    int         getResponseLen = ERROR;
    clock_t     startTime;
    
    SihContext *sihContext = getSihContext();

    clearBytes(&sihCmdRef,sizeof(sihCmdRef));

    /* Prepare Sih Command Reference. */
    sihCmdRef.instanceId = SIH_INSTANCE_ID;
    sihCmdRef.origin     = OAMIF_SIH;
    sihCmdRef.cmdRefNum  = ++cmdRefNum;

    clearBytes(&getObjReq, sizeof(GetObjReq));
    clearBytes(&encGetObjBuf, sizeof(EncBuf));
    /* Prepare Get object request, Set Command ref and tag Info. */
    getObjReq.tagArray.numTags = 1; 
    getObjReq.cmdRef =  sihCmdRef;
    getObjReq.tagArray.tagInfo[0].tableId = sihTagInfo.tableId;
    getObjReq.tagArray.tagInfo[0].rowId   = sihTagInfo.rowId;
    getObjReq.tagArray.tagInfo[0].colId   = sihTagInfo.colId;

    /* Encode in get Object Request Message into TLV Buffer */
    encGetObjReqMsg(&getObjReq, &encGetObjBuf);

    /* Create and Send Get Request Message to BSM */
    status = createAndSendMsg (MSG_SIH_BSM_GET_OBJ_REQ, 
                               (UINT8*)&encGetObjBuf.value, encGetObjBuf.len);

    /* Start the Timer for Command Response */
    startSihRspTimer(TIMERNAME_WAITSNMPCMDRSP);

    if (status EQ TRUE)
    {
    	logInfo( "Succedded sihGetProcessor...createAndSendMsg for "
             "Table %d", getObjReq.tagArray.tagInfo[0].tableId);
    }
    else
    {
    	logError( "Failed sihGetProcessor...createAndSendMsg for "
             "Table %d", getObjReq.tagArray.tagInfo[0].tableId);
      status = FALSE;
    }


    /* Wait for the response ? */
    logInfo("%s", "sihGetProcessor...Waiting for GET Response");
    sleep(1);

    while(FOREVER)
    {
      /* Process Get Response */
      msgIn = recvMsg (MODID_SIH, &recvMsgRetVal);
      if( msgIn NOTEQ NULL )
      {
        msgId = msgIn->msgId;
        logInfo("sihGetProcessor...msgId %d", msgId);
        g_sihRespRecd++;
        switch(msgId)
        {
            case MSG_TIMEKEEPER_SIH_TIMEOUT_IND:
            {
                status = FALSE;
                errCode = BS_ERR_OPERATION_TIMED_OUT;
                errTableId = sihTagInfo.tableId;
                logInfo("%s","sihGetProcessor...MSG_TIMEKEEPER_SIH_TIMEOUT_IND");
                freeMsgIn(msgIn);
                /* set the status of get response as Failure */
                break;
            }
            case MSG_PRM_SIH_HELLO:
            {
              	/* Process hello request */
              	sihProcHelloReqInAllStates(msgIn, sihContext);
              	/* Free the received Message */
              	freeMsgIn(msgIn);
              	/* set the status of get response as Failure */
              	status = FALSE;  
              	continue;
            }
            case MSG_BSM_SIH_NOTI_IND:
	    {
            	/* Process Notification */
      	     	sihProcNotiIndInAllStates(msgIn, sihContext);
              	/* Free the received Message */
              	freeMsgIn(msgIn);
              	/* set the status of get response as Failure */
              	status = FALSE;  
              	continue;
            }
            case MSG_PRM_SIH_EXIT_CMD:
            {
              	/* Process exit command request */
              	sihProcExitReqInAllStates(msgIn, sihContext);
              	/* Free the received Message */
              	freeMsgIn(msgIn);
              	/* set the status of get response as Failure */
              	status = FALSE;  
              	continue;
            }
            case MSG_BSM_SIH_GET_OBJ_SUCC:
            {
                stopSihRspTimer(TIMERNAME_WAITSNMPCMDRSP);
             	clearBytes( &rspSucc, sizeof(GetObjRspSucc));
             	decGetObjRspSuccMsg((UINT8*)&(msgIn->data[0]), msgIn->len,
                                 &rspSucc );
             	logInfo("%s", "sihGetProcessor...MSG_BSM_SIH_GET_OBJ_SUCC");
	     	/* set the status of get response as success */
             	status = TRUE;
                freeMsgIn(msgIn);
             	break;
            }
            case MSG_BSM_SIH_GET_OBJ_FAIL:
            {
                stopSihRspTimer(TIMERNAME_WAITSNMPCMDRSP);
             	GetObjRspFail   rspFail;
             	clearBytes( &rspFail, sizeof(GetObjRspFail));
             	decGetObjRspFailMsg( (UINT8*)&(msgIn->data[0]), msgIn->len, 
                                  &rspFail );
             	/* set the status of get response as Failure */
             	status = FALSE;
		       errCode = rspFail.getErrRsp.errRspCode;
		       errTableId = sihTagInfo.tableId;
                logError("%s","sihGetProcessor...MSG_BSM_SIH_GET_OBJ_FAIL");
                freeMsgIn(msgIn);
             	break;
            }
            default:
            {
             	/* Free the received Message */
             	freeMsgIn(msgIn);
             	/* all other responses are not handled. */
             	status = FALSE;  
             	continue;
            }
        } //End of Switch Case
      }
      else
      {
        status = FALSE;
      } //End of If Case

      if(errCode NOTEQ OK )
      {
        if(errCode EQ BS_ERR_OPERATION_TIMED_OUT)
          logInfo("%s","sihGetProcessor... got SIH_TIMEOUT_IND");
        else
          logError("%s","sihGetProcessor... got GET_OBJ_FAIL from BSM");
        return status;
      }
      else if(status EQ TRUE)
      {
        break;
      }
      usleep(Q_POLL_INTERVAL);
    }// End of While loop

    if (status EQ TRUE) 
    {
      /* Read the value */
    	logInfo("sihGetProcessor...NO. OF TAGs "
             "rspSucc.tlvArray.numTlvs=%d", rspSucc.tlvArray.numTlvs);

      /* Extract the value from TLV and fill in SNMP data structure for SNMP Response */
      status = sendSnmpGetResponsetoEms(&(rspSucc.tlvArray));
    }
    return status; 
}


/**
* @details 
*          SIH Set Message Handler & Processor.
*          This is the routine which handles set request and response between 
*          SIH and BSM.
* @param sihTagInfo contains sih tag information.
* @param tlvArray is used to store the TLV array.
*/
TBOOL sihSetProcessor (
      TagInfo     sihTagInfo,
      TlvArray  * tlvArray
      )
{
    SetObjReq   setObjReq;
    EncBuf      encSetObjBuf;
    CmdRef      sihCmdRef;
    TBOOL       status = FALSE;
    static unsigned cmdRefNum = 0;
    UINT16      recvMsgRetVal = ERROR;
    Msg *       msgIn = NULL;
    UINT32      maxRows=0, maxCols =0;
    UINT32      msgId = MSG_UNDEFINED;    
    SetObjRspSucc   rspSucc;
    clock_t     startTime;

    SihContext *sihContext = getSihContext();

    clearBytes(&sihCmdRef,sizeof(sihCmdRef));
    /* Prepare Sih Command Reference. */
    sihCmdRef.instanceId = SIH_INSTANCE_ID;
    sihCmdRef.origin     = OAMIF_SIH;
    sihCmdRef.cmdRefNum  = ++cmdRefNum;

    clearBytes(&setObjReq, sizeof(SetObjReq));
    clearBytes(&encSetObjBuf, sizeof(EncBuf));
    /* Prepare set object request, fill sih command ref, tag. */
    setObjReq.cmdRef =  sihCmdRef;

    /* Set Table is not permitted */
    if(sihTagInfo.rowId EQ 0 && sihTagInfo.colId EQ 0)
    {
    	logInfo("%s","sihSetProcessor...Set Table is not permitted");
      return FALSE;
    }
    else if(sihTagInfo.colId EQ 0)
    {
      /* row */
      maxRows = 1; /* In one row */
      copyBytes( tlvArray, sizeof(TlvArray), &(setObjReq.tlvArray));
    }
    else /* element/column */
    {
    	logInfo("%s","Set Col Value");
      copyBytes( tlvArray, sizeof(TlvArray), &(setObjReq.tlvArray));
    }

    printTLV(&setObjReq.tlvArray);

    /* Encode in set Object Request Message into TLV Buffer */
    encSetObjReqMsg(&setObjReq, &encSetObjBuf);

    /* Create and Send Set Request Message to BSM */
    status = createAndSendMsg (MSG_SIH_BSM_SET_OBJ_REQ,
                               (UINT8*)&encSetObjBuf.value, encSetObjBuf.len);

    /* Start the Timer for Command Response */
    startSihRspTimer(TIMERNAME_WAITSNMPCMDRSP);

    if (status EQ TRUE)
    {
    	logInfo("%s", "sihSetProcessor...Succedded createAndSendMsg");
    }
    else
    {
    	logError("%s","sihSetProcessor...Failed createAndSendMsg");
      status = FALSE;
    }


    /* Wait for the response ? */
    logInfo("%s","sihSetProcessor...Waiting for SET Response");
    sleep(1);

    while(FOREVER)
    {
      startTime = clock();
      /* Process Set Response */
      msgIn = recvMsg (MODID_SIH, &recvMsgRetVal);
      if( msgIn NOTEQ NULL )
      {
        msgId = msgIn->msgId;
        logInfo("sihSetProcessor...msgId %d", msgId);
        g_sihRespRecd++;
        switch(msgId)
        {
             case MSG_TIMEKEEPER_SIH_TIMEOUT_IND:
             {
                status = FALSE;
                errCode = BS_ERR_OPERATION_TIMED_OUT;
                errTableId = sihTagInfo.tableId;
                logInfo("%s", "sihSetProcessor...MSG_TIMEKEEPER_SIH_TIMEOUT_IND");
                freeMsgIn(msgIn);
                /* set the status of get response as Failure */
                break;
             }
             case MSG_PRM_SIH_HELLO:
             {
               	/* Process hello request */
               	sihProcHelloReqInAllStates(msgIn, sihContext);
               	/* Free the received Message */
               	freeMsgIn(msgIn);
               	/* set the status of get response as Failure */
               	status = FALSE;  
               	continue;
             }
             case MSG_BSM_SIH_NOTI_IND:
	     {
             	/* Process Notification */
      	     	sihProcNotiIndInAllStates(msgIn, sihContext);
              	/* Free the received Message */
              	freeMsgIn(msgIn);
              	/* set the status of get response as Failure */
              	status = FALSE;  
              	continue;
             }
             case MSG_PRM_SIH_EXIT_CMD:
             {
              	/* Process exit command request */
              	sihProcExitReqInAllStates(msgIn, sihContext);
              	/* Free the received Message */
              	freeMsgIn(msgIn);
              	/* set the status of get response as Failure */
              	status = FALSE;  
              	continue;
             }
             case MSG_BSM_SIH_SET_OBJ_SUCC:
             {
                stopSihRspTimer(TIMERNAME_WAITSNMPCMDRSP);
              	clearBytes( &rspSucc, sizeof(SetObjRspSucc));
              	decSetObjRspSuccMsg( (UINT8*)&(msgIn->data[0]), msgIn->len, 
                                  &rspSucc );
              	logInfo("%s", "sihSetProcessor...MSG_BSM_SIH_SET_OBJ_SUCC");
              	logInfo( "Command Reference origin=%d, instanceId=%d,"
                    "cmdRefNum=%d", rspSucc.cmdRef.origin,
                    rspSucc.cmdRef.instanceId, rspSucc.cmdRef.cmdRefNum);
 
              	/* set the Status of Get Response as Success */
              	status = TRUE;
              	freeMsgIn(msgIn);
              	break;
             }
             case MSG_BSM_SIH_SET_OBJ_FAIL:
             {
                stopSihRspTimer(TIMERNAME_WAITSNMPCMDRSP);
              	SetObjRspFail   rspFail;
              	clearBytes( &rspFail, sizeof(SetObjRspFail));
              	decSetObjRspFailMsg( (UINT8*)&(msgIn->data[0]), msgIn->len, 
                                   &rspFail );
              	logError("%s","sihSetProcessor...MSG_BSM_SIH_SET_OBJ_FAIL");
              	/* set the Status of Get Response as Failure */
              	status = FALSE;
                errCode = rspFail.setErrRsp.errRspCode;
                errTableId = sihTagInfo.tableId;
                freeMsgIn(msgIn);
              	break;
             }
             default:
             {
             	/* Free the received Message */
             	freeMsgIn(msgIn);
               	/* all other responses are not handled. */
               	status = FALSE;  
               	continue;
             }
        }//End of Switch Case
      }
      else
      {
        status = FALSE;
      }//End of If Case

      if(errCode NOTEQ OK )
      {
        if(errCode EQ BS_ERR_OPERATION_TIMED_OUT)
        	logInfo("%s", "sihGetProcessor... got SIH_TIMEOUT_IND");
        else
        	logError("%s","sihSetProcessor... got SET_OBJ_FAIL from BSM");
	return status;
      }
      else if (status EQ TRUE)
      {
        logInfo("%s", "sihSetProcessor...Succedded recvMsg ");
        return TRUE;
      }
      usleep(Q_POLL_INTERVAL);
    }//End of While Loop
}

/**
 * @param   totalTimers total timers used in SIH module
 * @param   config      pointer to timer configuration structure with timer information
 * @details             This function is used to update entry to Status table after receiving Response from BSM
 */
static void sihStoreTimersInCtx(
  IN UINT8         totalTimers,
  IN TimersConfig  *config
  )
{
  Index          i =0, timerArrayIndex = 0;
  SihTimersInfo  *ctxTimerInfo = getSihTimersInfo();

  for ( i=0; i < totalTimers; i++ )
  {
    if ( config->moduleId EQ MODID_SIH )
    {
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.moduleId         = config->moduleId;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeSpec.duration = config->duration;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeSpec.timeUnit = config->timeUnit;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timerType         = config->timerName;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeoutMsgId      = MSG_TIMEKEEPER_SIH_TIMEOUT_IND;
      ctxTimerInfo->totalTimersInMod++;
      timerArrayIndex++;
    }
    config++;
  }
}

/**
 * @param   configFilePath This is the configuration path, either will be NULL / or some valid path
 * @details                This function will read the bs_timers Configuration file
 **/
void readSihTimersFromCfg(
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
    logError("%s","No getReadFromFileFunc Function defined for table BSTIMERSCONFIG");
    exit(EXIT_FAILURE);
  }
  else
  {
    SihContext *sihContext = getSihContext();
    sihContext->totalTimers = tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
    /* Copy timers in SIH context */
    config = (TimersConfig*)getFirstRowOfTable(TIMERSCONFIG);
    sihStoreTimersInCtx( sihContext->totalTimers, config );
    if ( isError EQ TRUE )
    {
      BS_ERROR_CONFIG_READ_FAILED()
      exit(EXIT_FAILURE);
    }
  }
}


/**
 * @param argc used for finding number of arguments passed.
 * @param argv arguments passed for SIH.
 * @details This function is the Main Function for SIH Module.
 */
int sihInit (int argc, char **argv)
{
    UINT8      status = FALSE;
    UINT8      *exeName = "sih.exe";
    SihContext *sihContext = getSihContext();
    TimeSpec   tick = { TIMEUNIT_SEC, 1 };
    char       *rootDir = NULL;
    char       *configDir = NULL;
    char       *offset = NULL;
    
    /* Config Dir Path */
    if ( argv[5] NOTEQ NULL )
        {
         offset = strstr(argv[5], "/snmpd.conf");
         if (offset NOTEQ NULL)
             *offset = '\0';
         
         configDir = argv[5];
       
         clearBytes (sihContext->configDir, sizeof(FilePath));
	 sprintf(sihContext->configDir,"%s",configDir); 
        }

    /* Root Dir Path */
    if (configDir NOTEQ NULL)
        {
         offset = strstr(configDir, "/config");
         if (offset NOTEQ NULL)
            *offset = '\0';
         rootDir = argv[5];
        }
    else
        exit(EXIT_FAILURE);

    fillGenericFunctionTable();
    emptyAllTables();

    /* Handle command line arguments before modInit. */
    cmdLineParamHandler(0, NULL, &sihContext->configDir[0], 
                        &sihContext->versionInfo );

    /* Initialise the SIH module */
    status = modInit( MODID_SIH, exeName, (char*) rootDir, tick );
    if (status EQ TRUE)
        {
        /* Inform PRM (process manager that I(SIH) have been initialised. */
        sendStartIndToPrm(MODID_SIH, MSG_SIH_PRM_START_IND);
        }
    else
        {
        logError("Could not register SIH Module, Status returned=%d", status);
        exit(EXIT_FAILURE);
        }

    /* This will read all the timers related to SIH module and store them in the SIH context */
    readSihTimersFromCfg( &sihContext->configDir[0]);

    /* Initialise Lib print for TLV  */
    printTlvInit( &sihContext->configDir[0] );

    /* Initialise Lib print for NotiMsg TLV */
    printNotiMsgTlvInit( &sihContext->configDir[0] );

    /* Start the boot up procedure */
    initSihStateMachine();

    return 0;
}

void fillTableInfo( 
     IO TagInfo *tagInfo,
     IN TableId  tableId,
     IN RowId    rowId,
     IN ColId    colId
     )
{
  tagInfo->tableId = tableId;
  tagInfo->rowId   = rowId;
  tagInfo->colId   = colId;
}

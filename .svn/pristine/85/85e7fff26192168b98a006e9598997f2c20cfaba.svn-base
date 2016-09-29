//////////////////////////////////////////////////////////////////////////////////
// @file    wih_oamInterface.cpp
// @details Copyright Powerwave Technologies, Inc., 2010
// @brief This file has the methods for class COamInterface. @@ Singleton
//
// Author     : Tara Shankar Maji
// Date       : 21-09-2010
// Description: This file contains methods for OAM IPC interface functionalities.
// Change History: 21-09-10 Initial version.
///////////////////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include "wih_oamInterface.h"
#include "wih_sessionMgr.h"

extern bool g_RspRcvd;
extern bool g_RspProcessed;
extern pthread_mutex_t g_bsm_rsp_mutex;
extern pthread_cond_t  g_bsm_rsp_cv;
extern Msg      *g_msgIn;
extern UINT8   *g_configFilePath;

COamInterface * COamInterface::mInstance = NULL;

////////////////////////////////////////////////////////////////////
// Operation	:  
// Description	: Constructor 
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
COamInterface::COamInterface()
{
}

////////////////////////////////////////////////////////////////////
// Operation	: 
// Description	: Destructor  
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
COamInterface::~COamInterface()
{
}

////////////////////////////////////////////////////////////////////
// Operation	: processModuleExitReq
// Description	: This method is used for processing the EXIT msg
// 		  received from PRM module 
// Input        : None
// Output	: None
///////////////////////////////////////////////////////////////////
void COamInterface::processModuleExitReq()
{
	logInfo("%s","Received EXIT message...");
	// First close the web server application
	int ret=system("pkill -9 lighttpd.exe");
        if(ret EQ -1)
        {
                logError("%s","FAILED to stop Web Server Application... \n");
        }

	UINT8 exeName[8];
	strcpy((char *)&exeName[0],"wih.exe");

	// Now exit WIH module
	modExit(MODID_WIH, exeName);
  	exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////
// Operation	: processPrmHelloMsg
// Description	: This method is used for processing the HELLO msgs
// 		  received from PRM module 
// Input        : None
// Output	: None
///////////////////////////////////////////////////////////////////
void COamInterface::processPrmHelloMsg(Msg *msgIn)
{
  MsgPrmModHello     *hello = (MsgPrmModHello*)msgIn->data;
  MsgModPrmHelloAck   helloAck;
  clearBytes( &helloAck, sizeof(MsgModPrmHelloAck) );
  helloAck.helloAckSeqNum = hello->helloSeqNum;

  logInfo("%s","Received HELLO message...");

  // Send the ACK msg to PRM
  createAndSendMsg( MSG_WIH_PRM_HELLO_ACK, (UINT8*)&helloAck, sizeof(MsgModPrmHelloAck) );
}

////////////////////////////////////////////////////////////////////
// Operation	: processSetLogConfig
// Description	: This method is used for processing the LOGCONFG msg
// 		  received from BSM module 
// Input        : None
// Output	: None
///////////////////////////////////////////////////////////////////
void COamInterface::processSetLogConfig(
 Msg           *msg
)
{
  SetLogConfig       *msgSetLog = (SetLogConfig*)(msg->data);

  LogContext  *logContext  = getLogContext();
  copyBytes(msgSetLog, sizeof(LogConfig), &(logContext->logConfig));
  logInfo ("%s","Loglevel is set to new values");
}

////////////////////////////////////////////////////////////////////
// Operation	: processInactTimerMsg
// Description	: This method is used for processing the INACTIVITY 
//		  timer timeout msgs
// Input        : None
// Output	: None
///////////////////////////////////////////////////////////////////
void COamInterface::processInactTimerMsg()
{
	logInfo("%s","Received timeout message...");
	CSessionManager::getInstance().checkInactiveUsers();

	// Re-Start the inactivity timer
        TimerDesc usrInactTmr;
        clearBytes(&usrInactTmr, sizeof(usrInactTmr));

        usrInactTmr.moduleId = MODID_WIH;
        usrInactTmr.userInfo = MODID_WIH;
        usrInactTmr.timeoutMsgId = MSG_TIMEKEEPER_WIH_TIMEOUT_IND;
        usrInactTmr.timeSpec.timeUnit = TIMEUNIT_SEC;
        usrInactTmr.timeSpec.duration = 5;
        usrInactTmr.timerType = TIMERNAME_WAITCMDRSP;

        createAndStartTimer(&usrInactTmr, NULL, NULL);
}

////////////////////////////////////////////////////////////////////
// Operation	: wihProcessRecvIpcMsg
// Description	: This method is used for processing the OAM IPC msgs
// 		  received from other modules (BSM, PRM) 
// Input	: message buffer
// Output	: None
///////////////////////////////////////////////////////////////////
void COamInterface::wihProcessRecvIpcMsg(Msg *msgIn)
{
        switch(msgIn->msgId)
        {
		case MSG_TIMEKEEPER_WIH_TIMEOUT_IND:
		{
			processInactTimerMsg();
			break;
		}
		case MSG_PRM_WIH_HELLO:
		{
			processPrmHelloMsg(msgIn);
			break;
		}
		case MSG_BSM_WIH_SET_LOG_CFG:
		{
			processSetLogConfig(msgIn);
			break;
		}
		case MSG_BSM_WIH_GET_OBJ_SUCC:
		case MSG_BSM_WIH_GET_OBJ_FAIL:
		case MSG_BSM_WIH_SET_OBJ_SUCC:
		case MSG_BSM_WIH_SET_OBJ_FAIL:
		{
			logInfo("%s","Received GET/SET Response Message...");
			UINT32 tranxId = 0;

			if(msgIn->msgId EQ MSG_BSM_WIH_GET_OBJ_SUCC ||
			   msgIn->msgId EQ MSG_BSM_WIH_SET_OBJ_SUCC)
			{
				SetObjRspSucc   rspSucc;
				clearBytes( &rspSucc, sizeof(SetObjRspSucc));
				/* Decode TLV's */
				decSetObjRspSuccMsg( (UINT8*)&(msgIn->data[0]), msgIn->len, &rspSucc );
				tranxId = rspSucc.cmdRef.cmdRefNum;
			}
			else if(msgIn->msgId EQ MSG_BSM_WIH_GET_OBJ_FAIL)
			{
				GetObjRspFail   rspFail;
	                        clearBytes( &rspFail, sizeof(GetObjRspFail));

        	                // Decode TLV's for Get Message
                	        decGetObjRspFailMsg( (UINT8*)&(msgIn->data[0]), msgIn->len, &rspFail );
				tranxId = rspFail.cmdRef.cmdRefNum;
			}
			else
			{
				SetObjRspFail   rspFail;
	                        clearBytes( &rspFail, sizeof(SetObjRspFail));

        	                // Decode TLV's for Set Message
                	        decSetObjRspFailMsg( (UINT8*)&(msgIn->data[0]), msgIn->len, &rspFail );
				tranxId = rspFail.cmdRef.cmdRefNum;
			}

			// Check if cmdref is still valid or it has timed out already.
			// , also there can be a case when the user has logged out after
			// firing a command
			if(true EQ CSessionManager::getInstance().isCmdStillValid(tranxId))
			{
				// Aquire the mutex
				pthread_mutex_lock(&g_bsm_rsp_mutex);

				// Setting the global params
				g_RspRcvd = true;
				g_msgIn = msgIn;
				g_RspProcessed = false;

				// Signal the waiting thread for processing the BSM
				// response
				pthread_cond_signal(&g_bsm_rsp_cv);			

				// Unlocking so that processing can complete
				pthread_mutex_unlock(&g_bsm_rsp_mutex);

				// Now we need to wait for the processing to be complete
				// before we begin the clean up.
				// So that main thread gets hold of bsm_rsp_mutex
				while(g_RspProcessed NOTEQ true)
				{
				  usleep(1200);
				}

				pthread_mutex_lock(&g_bsm_rsp_mutex);
				g_RspProcessed = false;
				logInfo("%s","BSM msg processing over, doing the CLEAN-UP...");
				pthread_mutex_unlock(&g_bsm_rsp_mutex);
			}
			else
			{
				logInfo("%s","GET/SET command is INVALID. Dumping the Message");
			}

			break;
		}
		case MSG_PRM_WIH_EXIT_CMD:
		{
			processModuleExitReq();
			break;
		}
        	default:
          	{
          		// Unknown Message received
            		BS_ERROR_RECEIVED_INVALID_MSG( msgIn->msgId, 0 );
       			break;
          	}
        } //end of switch
}

////////////////////////////////////////////////////////////////////
// Operation	: getInstance
// Description	: This method returns the reference to the singleton 
//		  object for OAM interface.
// Input	: None
// Output	: COamInterface object reference
///////////////////////////////////////////////////////////////////
COamInterface & COamInterface::getInstance(void)
{
	if(mInstance EQ NULL) {
		mInstance = new COamInterface;
	}

	return *mInstance;
}

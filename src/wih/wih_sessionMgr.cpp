//////////////////////////////////////////////////////////////////////////////////
// @file    wih_sessionMgr.cpp
// @details Copyright Powerwave Technologies, Inc., 2010
// @brief This file has the methods for class CSessionMgr.
//
// Author     : Tara Shankar Maji
// Date       : 13-09-2010
// Description: This file contains methods for WIH session manager functionalities.
// Change History: 13-09-10 Initial version.
///////////////////////////////////////////////////////////////////////////////////

#include "wih.h"
#include "wih_sessionMgr.h"
#include "wih_loginCmdSession.h"
#include "wih_getCmdSession.h"
#include "wih_setCmdSession.h"
#include "wih_webResponse.h"
#include "fcgi_stdio.h"

extern pthread_mutex_t g_session_table_mutex;
CSessionManager * CSessionManager::mInstance = NULL;
static ContextID ctxtId = 0;

////////////////////////////////////////////////////////////////////
// Operation	: 
// Description	: Destructor  
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
CSessionManager::~CSessionManager()
{
}

////////////////////////////////////////////////////////////////////
// Operation    : petInactivityTime
// Description  : Resets the inactivity time for a context to zero
// Input        : Context Identifier
// Output       : None
///////////////////////////////////////////////////////////////////
void CSessionManager::petInactivityTime(const ContextID &Cntx)
{
	pthread_mutex_lock(&g_session_table_mutex);
        // Search for the context in session table
        WebSessionList::iterator session_iter;
        for(session_iter = mSessions.begin(); session_iter NOTEQ mSessions.end(); session_iter++)
        {
                if(session_iter->mContext EQ Cntx)
		{
		    // Reset the inactivity time value
                    session_iter->mInactiveTime = 0;
		    break;
		}	
        }

	pthread_mutex_unlock(&g_session_table_mutex);
	
	return;
}

////////////////////////////////////////////////////////////////////
// Operation	: validateContext 
// Description	: Checks for the context id to be present in the
//		  mMap (CommandSession Map)  
// Input	: none 
// Output	: true/false based on context being found/not-found
///////////////////////////////////////////////////////////////////
bool CSessionManager::validateContext(const ContextID &Cntx)
{
	pthread_mutex_lock(&g_session_table_mutex);
	// Session Table is still empty
	if(mSessions.empty())
	{
		pthread_mutex_unlock(&g_session_table_mutex);
		return false;
	}

	// Search for the context in session table
	WebSessionList::iterator session_iter;
	for(session_iter = mSessions.begin(); session_iter NOTEQ mSessions.end(); session_iter++)
	{
		if(session_iter->mContext EQ Cntx)
		{
			pthread_mutex_unlock(&g_session_table_mutex);
			return true;
		}
	}
	
	pthread_mutex_unlock(&g_session_table_mutex);
	return false;
}

////////////////////////////////////////////////////////////////////
// Operation	: createCmdSession 
// Description	: Creates a command object based upon the request, 
//		  this is a factory method to generate appropriate
//		  command object.
// Input	: CCommandInfo object pointer, pointer to newly created
//		  CmdSession object 
// Output	: true/false based on command creation success
///////////////////////////////////////////////////////////////////
bool CSessionManager::createCmdSession(CCommandInfo *pCmdInfo, CmdSession **cs)
{
	// Check for the requested page name
	const char* cmdName = pCmdInfo->getCmdName();

	if(!strcmp(cmdName,LOGIN_CMD))
	{
		logInfo("%s","Got a LOGIN request.");
		*cs = new LoginCmdSession();
		return true;
	}
	else if(!strcmp(cmdName,GET_CMD))
	{
		logInfo("%s","Got a GET request.");
		*cs = new GetCmdSession();
		return true;
	}
	else if(!strcmp(cmdName,SET_CMD))
	{
		logInfo("%s","Got a SET request.");
		*cs = new SetCmdSession();
		return true;
	}
	
	return false;
}

////////////////////////////////////////////////////////////////////
// Operation	: isStaticPageRequest
// Description	: This method is used for checking whether the page
// 		  request is static and needs special processing. 
// Input	: CCommandInfo object pointer
// Output	: true/false as per static page or not
///////////////////////////////////////////////////////////////////
bool CSessionManager::isStaticPageRequest(CCommandInfo *pCmdInfo)
{
	// Check for the requested page name
	const char* cmdName = pCmdInfo->getCmdName();

	if((!strcmp(cmdName,INDEX_PAGE))||
	   (!strcmp(cmdName,TOP_PAGE))||
	   (!strcmp(cmdName,CONFIG_INDEX_PAGE))||
	   (!strcmp(cmdName,MONITOR_INDEX_PAGE))||
	   (!strcmp(cmdName,COMMANDS_INDEX_PAGE))||
	   (!strcmp(cmdName,BOTTOM_PAGE))||
	   (!strcmp(cmdName,CONFIG_PAGE))||
	   (!strcmp(cmdName,MONITOR_PAGE))||
	   (!strcmp(cmdName,COMMAND_PAGE))||
	   (!strcmp(cmdName,RESULT_PAGE))||
	   (!strcmp(cmdName,HELP_PAGE))||

	   (!strcmp(cmdName,ENB_CMD_PAGE))||
	   (!strcmp(cmdName,TRACE_CMD_PAGE))||
	   (!strcmp(cmdName,DWNLD_CMD_PAGE))||
	   (!strcmp(cmdName,UPLD_CMD_PAGE))||
	   (!strcmp(cmdName,SWINST_CMD_PAGE))||
	   (!strcmp(cmdName,SWACTV_CMD_PAGE))||
	   (!strcmp(cmdName,CELLADMIN_CMD_PAGE))||
	   
	   (!strcmp(cmdName,GEN_DETAIL_PAGE))||
	   (!strcmp(cmdName,ENBREG_CFG_PAGE))||
	   (!strcmp(cmdName,ENB_CFG_PAGE))||
	   (!strcmp(cmdName,CELL_CFG_PAGE))||
	   (!strcmp(cmdName,SCHDL_CFG_PAGE))||
	   (!strcmp(cmdName,PHY_CFG_PAGE))||
	   (!strcmp(cmdName,SIB1_CFG_PAGE))||
	   (!strcmp(cmdName,SIB2_CFG_PAGE))||
	   (!strcmp(cmdName,SIB3_CFG_PAGE))||
	   (!strcmp(cmdName,LOG_CFG_PAGE))||
	   (!strcmp(cmdName,RADIOHD_CFG_PAGE))||
	   (!strcmp(cmdName,TRC_CFG_PAGE))||
	   (!strcmp(cmdName,QOS_CFG_PAGE))||
	   (!strcmp(cmdName,PMCOUNTERLOG_CFG_PAGE))||
	   (!strcmp(cmdName,HO_CFG_PAGE))||
	   (!strcmp(cmdName,NEIG_CFG_PAGE))||
	   (!strcmp(cmdName,MME_CFG_PAGE))||
	   (!strcmp(cmdName,MSGTRC_CFG_PAGE))||
	   (!strcmp(cmdName,TIMER_CFG_PAGE))||
	   (!strcmp(cmdName,EVTTHR_CFG_PAGE))||
	   (!strcmp(cmdName,ENBADDR_CFG_PAGE))||
	   (!strcmp(cmdName,SRSDEDICATED_CFG_PAGE))||
	   (!strcmp(cmdName,SECURITY_CFG_PAGE))||
	   (!strcmp(UEEVENTHISTORYLOG_CFG_PAGE, cmdName))||

	   (!strcmp(cmdName,UPLD_STS_PAGE))||
	   (!strcmp(cmdName,DWNLD_STS_PAGE))||
	   (!strcmp(cmdName,SWACT_STS_PAGE))||
	   (!strcmp(cmdName,SWPKG_STS_PAGE))||
	   (!strcmp(cmdName,COMP_STS_PAGE))||
	   (!strcmp(cmdName,ENB_STS_PAGE))||
	   (!strcmp(cmdName,MODULE_STS_PAGE))||
	   (!strcmp(cmdName,RDH_STS_PAGE))||
	   (!strcmp(cmdName,TRC_STS_PAGE))||
	   (!strcmp(cmdName,FEVTHIS_STS_PAGE))||
	   (!strcmp(cmdName,SWINST_STS_PAGE))||
	   (!strcmp(cmdName,ACTUE_STS_PAGE))||
	   (!strcmp(cmdName,ACTRAB_STS_PAGE))||
	   (!strcmp(cmdName,UEEVTHIS_STS_PAGE))||
	   (!strcmp(cmdName,RABHIS_STS_PAGE))||
	   (!strcmp(cmdName,PMLOG_STS_PAGE))||
	   (!strcmp(cmdName,MODLOG_STS_PAGE))||
	   (!strcmp(cmdName,ACTALM_STS_PAGE))||
	   (!strcmp(cmdName,ALMHIS_STS_PAGE))||
	   (!strcmp(cmdName,MACGENCOUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,MACUPLANECOUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,RLCGENCOUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,PDCPGENCOUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,PDCPUPLANECOUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,GTPGENCOUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,SCTPGENCOUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,S1APGENCOUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,S1APPEERNODECOUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,RRCGENCOUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,RRCCELLCOUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,RRMCOUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,ETHERNETCOUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,IPCOUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,LOCATION_STS_PAGE)) ||
	   (!strcmp(cmdName,LED_STS_PAGE)) ||
	   (!strcmp(cmdName,GPS_STS_PAGE))||
	   (!strcmp(cmdName,CELL_STS_PAGE))||
	   (!strcmp(cmdName,L2COUNTERS_STS_PAGE))||
	   (!strcmp(cmdName,COUNTERCOLLECTIONSTATUS_STS_PAGE))||
	   (!strcmp(cmdName,COUNTERCTRL_CMD_PAGE))||
	   (!strcmp(cmdName,COUNTER_CFG_PAGE))
	)
	{
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////
// Operation	: handleRequest
// Description	: This method is used for handling new requests from
// 		  the web clients. 
// Input	: CCommandInfo object pointer
// Output	: true/false as per success/failure towards handling
//		  the web client request.
///////////////////////////////////////////////////////////////////
bool CSessionManager::handleRequest(CCommandInfo *pCmdInfo)
{
	// *** LOGIC ***
	// if context id is 0 and request is for login
	//    authenticate the user
	//    generate new context id
	//    create the index page
	//
	// if context id is invalid
	//    create and send the login page (with text)
	// else if context is valid
	//    if the request is for GET
	//	Process GET request
	//    if the request is for SET
	//	Process SET request
	// END

	// Get the context identifier in the received request
	const ContextID cid = pCmdInfo->getContextId();

	// Check for the requested page name
	const char* cmdName = pCmdInfo->getCmdName();

	if(cid EQ NULL_CONTEXT)
	{
		// Check if login page
		if(!strcmp(cmdName,LOGIN_CMD))
		{
			int iTs = mSessions.size();
		
			// Check if MAXIMUM users is reached ???
			if(iTs>=(MAX_WEBCRAFT_USERS-1))
			{
				// Maximum users supported is reached
				// Can not LOGIN, serve the busy page
				// Send JSON based BUSY response
				CWebResponse::getInstance().createAndSendResponse(RES_BUSY);
				return true;
			}

			CmdSession *newSession;
			bool bResult = createCmdSession(pCmdInfo,&newSession);
		
			if((bResult NOTEQ true) || (newSession EQ NULL))
			{
				// Send JSON based command NOT supported response
				CWebResponse::getInstance().createAndSendResponse(RES_CMD_NOT_SUPPORTED);
			}

			// Execute the command
			newSession->executeCmd(pCmdInfo);
		}
		else
		{
			// Someone needs to get the login page here
			printf("X-Sendfile: %s\r\n\r\n", "/tmp/web/droot/lte_login.htm");
		}
	}
	else
	{
		// If we are here that means context is not NULL 
		// Validate context id	
		if(true NOTEQ validateContext(cid))
		{
			// CASE: Operator got logged out due to inactivity
			// Invalid Context Received, the user was logged out due
			// to inactivity --- send the output so that GUI closes
			// current window and opens new window with login page
			
			// Send JSON based web response for INACTIVE user logout
			// when the user has opted for logout
			if(!strcmp(cmdName,LOGOUT_CMD)) {
			CWebResponse::getInstance().createAndSendResponse(RES_INACTIVE_LOGOUT);
			} else {
			// Some link other than logout was clicked then send login page
			printf("X-Sendfile: %s\r\n\r\n", "/tmp/web/html/lte_relogin.htm");
			}
			return true;
		}
		

		// Check if we have got LOGOUT !!!
		if(!strcmp(cmdName,LOGOUT_CMD))
		{
			logInfo("Got a LOGOUT request for Context ID : %ld", cid);

			pthread_mutex_lock(&g_session_table_mutex);
			// Search for the session in the session table	
			WebSessionList::iterator session_iter = mSessions.begin();
			while(session_iter NOTEQ mSessions.end())
			{
				if(session_iter->mContext EQ cid)
				{
					// Remove the session from session table
					// session_iter points to the next element
					// after calling erase
					session_iter = mSessions.erase(session_iter);
					break;
				}
				else
				{
					++session_iter;
				}
			}
			
			pthread_mutex_unlock(&g_session_table_mutex);
			CWebResponse::getInstance().createAndSendResponse(RES_LOGOUT_SUCCESS);
			return true;
		}

		// Pet the inactivity time first since its a valid context
		petInactivityTime(cid);
		
		// Check whether the request is for STATIC web page
		// Handle the same and send the appropriate page to browser
		if(true EQ isStaticPageRequest(pCmdInfo))
		{
			CWebResponse::getInstance().sendStaticPage(pCmdInfo);
			return true;
		}
		else
		{
			// Here we will handle the command GET, SET
			CmdSession *newSession;
			bool bResult = createCmdSession(pCmdInfo,&newSession);
		
			if((bResult NOTEQ true) || (newSession EQ NULL))
			{
				// Send JSON based command NOT supported response
				CWebResponse::getInstance().createAndSendResponse(RES_CMD_NOT_SUPPORTED);
				return true;
			}

			pthread_mutex_lock(&g_session_table_mutex);
			// Add the new command session object in the session table 
			// for the session context identifier 
			WebSessionList::iterator session_iter;
				
			// Find the session as per the context ID and add the new command session
			for(session_iter = mSessions.begin(); session_iter NOTEQ mSessions.end(); session_iter++)
			{
				if(session_iter->mContext EQ  cid)
				{
					session_iter->mContextSessions.push_back(newSession);
					break;
				}
			}

			pthread_mutex_unlock(&g_session_table_mutex);
	
			// Execute the command
			newSession->executeCmd(pCmdInfo);
		}	
		
	}
	return true;
}

////////////////////////////////////////////////////////////////////
// Operation	: updateSessionInfo
// Description	: This method is called by the CmdSession objects 
//		  to notify their status and update their details
//		  with the session manager
// Input	: CmdSession object
// Output	: none
///////////////////////////////////////////////////////////////////
void CSessionManager::updateSessionInfo(CmdSession *cs)
{
	ContextID cid = cs->getCmdContext();

	// Check whether its the login command
	if(cid EQ NULL_CONTEXT)
	{
		// Dynamic cast does a strong type checking for login cmd object
		LoginCmdSession *loginCmd = dynamic_cast <LoginCmdSession *>(cs);
	
		// Check for the authentication status
		if(AUTH_SUCCESS EQ loginCmd->getAuthStatus())
		{
			// Generate a new context now
			cid = generateNewContext();

			// Update the WIH session details
			ContextInfo newWihSession;
			newWihSession.mContext = cid;
			newWihSession.mInactiveTime = 0;

			pthread_mutex_lock(&g_session_table_mutex);
			mSessions.push_back(newWihSession);
			pthread_mutex_unlock(&g_session_table_mutex);

			// Send Success towards obtaining the index page
			// Send JSON based SUCCESS response
			CWebResponse::getInstance().createAndSendResponse(RES_LOGIN_SUCCESS, cid);
		}
		else
		{
			// Send failure
			// Send JSON based FAILURE response
			CWebResponse::getInstance().createAndSendResponse(RES_LOGIN_FAIL);
		}
	}
	else
	{
		// Check if the command has completed
		if(CMD_COMPLETED EQ cs->getCmdStatus())
		{
			pthread_mutex_lock(&g_session_table_mutex);
			WebSessionList::iterator session_iter; 
				
			// Find the session first
			for(session_iter = mSessions.begin(); session_iter NOTEQ mSessions.end(); session_iter++)
			{
				if(session_iter->mContext EQ cid)
				{
					// Delete the command session from the session table
					// for the session's context identifier
					session_iter->mContextSessions.remove(cs);
					break;
				}
			}

			pthread_mutex_unlock(&g_session_table_mutex);
		}
	}

	// STL List member function 'remove' calls the destructor of the 
	// object removed, so checking is needed for 'cs' not to be NULL
	if(cs NOTEQ NULL)
	{
		// Check if the command has completed then terminate the command session object
		if(CMD_COMPLETED EQ cs->getCmdStatus())
		{
			cs->terminateCmd();
		}
	}

	return;	
}

////////////////////////////////////////////////////////////////////
// Operation	: isCmdStillValid
// Description	: This method checks for command session object to
//		  be still VALID whose tansaction id has been passed
//		  as an INPUT  
// Input	: transaction id
// Output	: None
///////////////////////////////////////////////////////////////////
bool CSessionManager::isCmdStillValid(UINT32  uiTranxId)
{
	pthread_mutex_lock(&g_session_table_mutex);
	// Session Table is still empty
	if(mSessions.empty())
	{
		pthread_mutex_unlock(&g_session_table_mutex);
		return false;
	}

	WebSessionList::iterator session_iter; 
	CmdSessionList::iterator cmdses_iter;
				
	// Iterate through all command session objects
	// to find the matching transaction id based
	// valid/waiting command object
	for(session_iter = mSessions.begin(); session_iter NOTEQ mSessions.end(); session_iter++)
	{
		for(cmdses_iter = session_iter->mContextSessions.begin();
		    cmdses_iter NOTEQ session_iter->mContextSessions.end();
		    cmdses_iter++)
		{
			// Check if the transaction id matches
			if(uiTranxId EQ (*cmdses_iter)->getCmdTransactionId())
			{
				pthread_mutex_unlock(&g_session_table_mutex);
				logInfo("%s","Found the waiting command session object.");
				return true;
			}
		}
	}

	pthread_mutex_unlock(&g_session_table_mutex);
	return false;
}

////////////////////////////////////////////////////////////////////
// Operation	: checkInactiveUsers
// Description	: This method checks for current user sessions based 
//		  inactivity, increases the tics for valid user sessions
//		  and logs out the sessions that have surpassed the
//		  inactivity timeout value. 
// Input	: None
// Output	: None
///////////////////////////////////////////////////////////////////
void CSessionManager::checkInactiveUsers(void)
{
	// Session Table is still empty
	if(mSessions.empty())
	    return;

	pthread_mutex_lock(&g_session_table_mutex);
	// Search for the context in session table
	WebSessionList::iterator session_iter = mSessions.begin();

	while( session_iter NOTEQ mSessions.end())
	{
		if(session_iter->mInactiveTime > WEBCRAFT_INACTIVITY_TIME)
		{
			logInfo("Removing inactive session with context %ld",session_iter->mContext);
			session_iter = mSessions.erase(session_iter);
		}
		else
		{
			// Our timer runs after every 5 secs
			// So we reset session based inactivity time 
			// in multiples of 5
			session_iter->mInactiveTime += 5;
			//logInfo("Session %d INACTIVE for '%d' seconds, Seconds left to logout %d", session_iter->mContext,
			//	session_iter->mInactiveTime, (WEBCRAFT_INACTIVITY_TIME-session_iter->mInactiveTime)); 			
			++session_iter;
		}
	}

	pthread_mutex_unlock(&g_session_table_mutex);

	return;
}

////////////////////////////////////////////////////////////////////
// Operation	: generateNewContext
// Description	: This method generates a new context identifier, for 
//		  the new operator who has been successfully authenticated.
// Input	: None
// Output	: Context Identifier
///////////////////////////////////////////////////////////////////
ContextID CSessionManager::generateNewContext(void)
{
	if(!ctxtId)
	{
		srand (time(NULL));
		while(!(ctxtId = rand() % CTX_RAND_RANGE)) ;
	}

	return ++ctxtId;
}

////////////////////////////////////////////////////////////////////
// Operation	: getInstance
// Description	: This method returns the reference to the singleton 
//		  object for WIH Session Manager.
// Input	: None
// Output	: CSessionManager object reference
///////////////////////////////////////////////////////////////////
CSessionManager & CSessionManager::getInstance(void)
{
	if(mInstance EQ NULL) {
		mInstance = new CSessionManager;
	}

	return *mInstance;
}

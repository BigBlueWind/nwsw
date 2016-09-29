//////////////////////////////////////////////////////////////////////////////////
// @file    wih_sessionMgr.h
// @brief Class definition for WIH Session Manager @@ Singleton
// @details Copyright Powerwave Technologies, Inc., 2009
//
// Author         : Tara Shankar Maji
// Date           : 13-09-2010
// Change History : 13-09-10 Initial version created.
//////////////////////////////////////////////////////////////////////////////////

#ifndef __SESSION_MGR__
#define __SESSION_MGR__

#include<iostream>
#include<list>
#include "wih_commandInfo.h"
#include "wih_cmdSession.h"

#define LOGIN_CMD       "login.fcgi"
#define LOGOUT_CMD      "logout.fcgi"
#define GET_CMD         "LTEWebGet.fcgi"
#define SET_CMD         "LTEWebSet.fcgi"

#define CTX_RAND_RANGE	0xFFFF

using namespace std;

typedef list < CmdSession * > CmdSessionList; 

typedef struct {
	ContextID mContext;
	unsigned int mInactiveTime;
	CmdSessionList mContextSessions;
}ContextInfo;

typedef list < ContextInfo > WebSessionList;

/////////////////// Class Definition /////////////////

class CSessionManager 
{
	private:
		//Attributes
		static CSessionManager *mInstance;
		WebSessionList	mSessions;

		// Methods
		bool validateContext(const ContextID &);
		bool createCmdSession(CCommandInfo *, CmdSession **);
		ContextID generateNewContext(void);
		void petInactivityTime(const ContextID &);
		bool isStaticPageRequest(CCommandInfo *);

	protected:
		CSessionManager(){};
		virtual ~CSessionManager();

	public:
		void updateSessionInfo(CmdSession *);
		bool handleRequest(CCommandInfo *);
		void checkInactiveUsers(void);
		bool isCmdStillValid(UINT32  uiTranxId);
		static CSessionManager & getInstance(void);
};


#endif // __SESSION_MGR__


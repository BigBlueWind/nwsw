//////////////////////////////////////////////////////////////////////////////////
// @file    wih_loginCmdSession.h
// @brief Class definition for LOGIN command session class
// @details Copyright Powerwave Technologies, Inc., 2009
//
// Author         : Tara Shankar Maji
// Date           : 13-09-2010
// Change History : 13-09-10 Initial version created.
//////////////////////////////////////////////////////////////////////////////////

#ifndef __LOGIN_CMD_SESSION__
#define __LOGIN_CMD_SESSION__

#include "wih_cmdSession.h"

#define FIELDNAME_UNAME "userName"
#define FIELDNAME_PSSWD "passWord"

enum {
	AUTH_SUCCESS = 1,
	AUTH_FAIL = -1
};

/////////////////// Class Definition /////////////////

class LoginCmdSession:public CmdSession 
{
	public:
		LoginCmdSession(){mAuthStatus = AUTH_FAIL;};
	 	~LoginCmdSession();
		int getAuthStatus(void);

	private:
		int mAuthStatus;
		void authenticateOperator(char *uId, char *pWd);
		void doExecuteCmd(CCommandInfo *cInfo);
		void doTerminateCmd (void);
};


#endif // __LOGIN_CMD_SESSION__


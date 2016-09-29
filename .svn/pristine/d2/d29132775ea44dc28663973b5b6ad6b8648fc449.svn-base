//////////////////////////////////////////////////////////////////////////////////
// @file    wih_loginCmdSession.cpp
// @details Copyright Powerwave Technologies, Inc., 2010
// @brief This file has the methods for class LoginCmdSession.
//
// Author     : Tara Shankar Maji
// Date       : 13-09-2010
// Description: This file contains methods for LoginCmdSession based functionalities.
// Change History: 13-09-10 Initial version.
///////////////////////////////////////////////////////////////////////////////////
#include <shadow.h>
#include "wih_loginCmdSession.h"

////////////////////////////////////////////////////////////////////
// Operation	: 
// Description	: Destructor  
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
LoginCmdSession::~LoginCmdSession()
{
}

////////////////////////////////////////////////////////////////////
// Operation	: getAuthStatus
// Description	: This method is used for obtaining authentication
//		  status.
// Input	: none 
// Output	: Authentication Status
///////////////////////////////////////////////////////////////////
int LoginCmdSession::getAuthStatus(void)
{
	return mAuthStatus;
}

////////////////////////////////////////////////////////////////////
// Operation	: doExecuteCmd
// Description	: This method is used for executing the command
// Input	: Command Info object pointer 
// Output	: none
///////////////////////////////////////////////////////////////////
void LoginCmdSession::doExecuteCmd(CCommandInfo *cInfo)
{
	char uname[MAX_VALUE_LENGTH], uname1[MAX_VALUE_LENGTH];
	char psswd[MAX_VALUE_LENGTH], psswd1[MAX_VALUE_LENGTH];

	strcpy(uname1,FIELDNAME_UNAME);
	strcpy(psswd1,FIELDNAME_PSSWD);

	if(true EQ cInfo->getParamByName(uname1,uname))
        {
                if(true EQ cInfo->getParamByName(psswd1,psswd))
                {
			authenticateOperator(uname,psswd);
                }
        }

	// Mark the command complete for clean-up
	mStatus = CMD_COMPLETED;
}

////////////////////////////////////////////////////////////////////
// Operation	: authenticateOperator
// Description	: This method is used for autheticating the user
// Input	: username and password  
// Output	: none
///////////////////////////////////////////////////////////////////
void LoginCmdSession::authenticateOperator(char *uname, char *psswd)
{
	struct spwd *user;

	if ((user= getspnam(uname)) NOTEQ NULL)
	{
		if (!strcmp(user->sp_pwdp, crypt(psswd, user->sp_pwdp)))
		{
                	mAuthStatus = AUTH_SUCCESS ;
		}
	}
}

////////////////////////////////////////////////////////////////////
// Operation	: terminateCmd
// Description	: This method is used for terminating the command
// Input	: Command Info object pointer 
// Output	: none
///////////////////////////////////////////////////////////////////
void LoginCmdSession::doTerminateCmd(void)
{
}


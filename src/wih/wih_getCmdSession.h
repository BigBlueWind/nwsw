//////////////////////////////////////////////////////////////////////////////////
// @file    wih_getCmdSession.h
// @details Class definition for GET command session class
// @details Copyright Powerwave Technologies, Inc., 2009
//
// Author         : Tara Shankar Maji
// Date           : 24-09-2010
// Change History : 24-09-10 Initial version created.
//////////////////////////////////////////////////////////////////////////////////

#ifndef __GET_CMD_SESSION__
#define __GET_CMD_SESSION__

#include "wih_cmdSession.h"

/////////////////// Class Definition /////////////////

class GetCmdSession:public CmdSession 
{
	public:
		GetCmdSession(){ mStatus = CMD_NOT_COMPLETED;};
	 	~GetCmdSession();

	private:
		bool getTableId(const char *, TableId *);
		void processOamResponse(Msg *msgIn);
		void doExecuteCmd(CCommandInfo *cInfo);
		void doTerminateCmd (void);
};


#endif // __GET_CMD_SESSION__


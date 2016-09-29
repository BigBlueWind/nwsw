//////////////////////////////////////////////////////////////////////////////////
// @file    wih_cmdSession.h
// @brief Class definition for base command session class
// @details Copyright Powerwave Technologies, Inc., 2009
//
// Author         : Tara Shankar Maji
// Date           : 13-09-2010
// Change History : 13-09-10 Initial version created.
//////////////////////////////////////////////////////////////////////////////////

#ifndef __CMD_SESSION__
#define __CMD_SESSION__

#include "wih_commandInfo.h"

#define TXN_RAND_RANGE  0xFFFF

typedef enum {
	CMD_COMPLETED = 0,
	CMD_NOT_COMPLETED = -1
}enCmdStatus;

/////////////////// Class Definition /////////////////

class CmdSession 
{
	public:
		CmdSession();
		void executeCmd(CCommandInfo *cInfo);
		void terminateCmd(void);
		ContextID getCmdContext(void);
		enCmdStatus getCmdStatus(void); 
		UINT32 getCmdTransactionId(void);

	protected:
		enCmdStatus  mStatus;
		ContextID    mContext;
		UINT32       mTransactionId;

	 	virtual ~CmdSession();
		virtual void doExecuteCmd(CCommandInfo *cInfo) = 0;
		virtual void doTerminateCmd (void) = 0;
		void generateTxnId(void);
};


#endif // __CMD_SESSION__


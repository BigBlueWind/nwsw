//////////////////////////////////////////////////////////////////////////////////
// @file    wih_cmdSession.cpp
// @details Copyright Powerwave Technologies, Inc., 2010
// @brief This file has the methods for class CmdSession.
//
// Author     : Tara Shankar Maji
// Date       : 13-09-2010
// Description: This file contains methods for abstract command session base object.
// Change History: 13-09-10 Initial version.
///////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "wih_cmdSession.h"
#include "wih_sessionMgr.h"

////////////////////////////////////////////////////////////////////
// Operation	:  
// Description	: Constructor 
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
CmdSession::CmdSession()
{
	mStatus = CMD_NOT_COMPLETED;
	mContext = 0;
	mTransactionId = 0;
}

////////////////////////////////////////////////////////////////////
// Operation	: 
// Description	: Destructor  
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
CmdSession::~CmdSession()
{
}

////////////////////////////////////////////////////////////////////
// Operation	: getCmdStatus
// Description	: This method is used for returning the cmd Status
// Input	: none
// Output	: Command's Status
///////////////////////////////////////////////////////////////////
enCmdStatus CmdSession::getCmdStatus(void)
{
	return mStatus;
}

////////////////////////////////////////////////////////////////////
// Operation	: getCmdTransactionId
// Description	: This method is used for returning the transaction ID
// Input	: none
// Output	: Command's Request ID
///////////////////////////////////////////////////////////////////
UINT32 CmdSession::getCmdTransactionId(void)
{
	return mTransactionId;
}


////////////////////////////////////////////////////////////////////
// Operation	: getCmdContext
// Description	: This method is used for returning the context ID
// Input	: none
// Output	: Command's Context ID
///////////////////////////////////////////////////////////////////
ContextID CmdSession::getCmdContext(void)
{
	return mContext;
}

////////////////////////////////////////////////////////////////////
// Operation	: executeCmd
// Description	: This method is used for executing the command
// Input	: Command Info object pointer 
// Output	: none
///////////////////////////////////////////////////////////////////
void CmdSession::executeCmd(CCommandInfo *cInfo)
{
	mContext = cInfo->getContextId();
	doExecuteCmd(cInfo);
	CSessionManager::getInstance().updateSessionInfo(this);
}

////////////////////////////////////////////////////////////////////
// Operation	: generateTxnId
// Description	: This method is used for generating a transaction ID
//		  for the newly arrived command
// Input	: None 
// Output	: None
///////////////////////////////////////////////////////////////////
void CmdSession::generateTxnId(void)
{
	if(!mTransactionId)
        {
                srand (time(NULL));
                while(!(mTransactionId = rand() % TXN_RAND_RANGE)) ;
        }
}

////////////////////////////////////////////////////////////////////
// Operation	: terminateCmd
// Description	: This method is used for terminating the command
// Input	: None 
// Output	: None
///////////////////////////////////////////////////////////////////
void CmdSession::terminateCmd(void)
{
	doTerminateCmd();
	delete(this);
}


//////////////////////////////////////////////////////////////////////////////////
// @file    wih_getCmdSession.cpp
// @details Copyright Powerwave Technologies, Inc., 2010
// @brief This file has the methods for class GetCmdSession.
//
// Author     : Tara Shankar Maji
// Date       : 24-09-2010
// Description: This file contains methods for GET command processing functionalities.
// Change History: 24-09-10 Initial version.
///////////////////////////////////////////////////////////////////////////////////

#include "wih_getCmdSession.h"
#include "wih_sessionMgr.h"
#include "wih_webResponse.h"

extern "C"
{
#include "tlv_enc_dec.h"
}

extern OamTableContext g_OamTableContext;
extern bool g_RspRcvd;
extern bool g_RspProcessed;
extern pthread_mutex_t g_bsm_rsp_mutex;
extern pthread_cond_t  g_bsm_rsp_cv;
extern Msg      *g_msgIn;

////////////////////////////////////////////////////////////////////
// Operation	: 
// Description	: Destructor  
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
GetCmdSession::~GetCmdSession()
{
}

////////////////////////////////////////////////////////////////////
// Operation	: getTableId 
// Description	: Gets the table ID for the specified table name  
// Input	: table name, table number
// Output	: true/false based on whether the name was found or not
///////////////////////////////////////////////////////////////////
bool GetCmdSession::getTableId(const char *tblName, TableId *tValue)
{
        TableId iLoop;
        UINT32 maxStringLength = 0;

        for(iLoop=0;iLoop<MAX_NUM_OPR_CFG_TABLES;iLoop++)
        {
		  maxStringLength = strlen((const char*)(g_OamTableContext.oprCfgTableInfo[iLoop].tableName));
		  if( maxStringLength < strlen(tblName))
		  {
			  maxStringLength = strlen(tblName);
		  }
          if(!strncmp((const char*)(g_OamTableContext.oprCfgTableInfo[iLoop].tableName), tblName, maxStringLength))
          {
                  *tValue = (iLoop + MIN_OPR_CFG_TABLEID);
                  return true;
          }
        }
        maxStringLength = 0;
        for(iLoop=0;iLoop<MAX_NUM_OPR_CMD_TABLES;iLoop++)
        {
          maxStringLength = strlen((const char*)(g_OamTableContext.oprCmdTableInfo[iLoop].tableName));
          if( maxStringLength < strlen(tblName))
		  {
			  maxStringLength = strlen(tblName);
		  }
          if(!strncmp((const char*)(g_OamTableContext.oprCmdTableInfo[iLoop].tableName), tblName, maxStringLength))
          {
                  *tValue = (iLoop + MIN_OPR_CMD_TABLEID);
                  return true;
          }
       }
       maxStringLength = 0;
       for(iLoop=0;iLoop<MAX_NUM_STATUS_TABLES;iLoop++)
        {
    	   maxStringLength = strlen((const char*)(g_OamTableContext.statusTableInfo[iLoop].tableName));
    	   if( maxStringLength < strlen(tblName))
		   {
			  maxStringLength = strlen(tblName);
		  }
          if(!strncmp((const char*)(g_OamTableContext.statusTableInfo[iLoop].tableName), tblName, maxStringLength ))
          {
                  *tValue = (iLoop + MIN_STATUS_TABLEID);
                  return true;
          }
        }

       for(iLoop=0;iLoop<MAX_NUM_PRE_CFG_TABLES;iLoop++)
        {
          if(!strncmp((const char*)(g_OamTableContext.preCfgTableInfo[iLoop].tableName),
                    tblName, strlen((const char*)(g_OamTableContext.preCfgTableInfo[iLoop].tableName))))
          {
                  *tValue = (iLoop + MIN_PRE_CFG_TABLEID);
                return true;
          }
        }
	return false;
}

////////////////////////////////////////////////////////////////////
// Operation	: doExecuteCmd
// Description	: This method is used for executing the command
// Input	: Command Info object pointer 
// Output	: none
///////////////////////////////////////////////////////////////////
void GetCmdSession::doExecuteCmd(CCommandInfo *cInfo)
{
	char tblName[MAX_TABLE_NAME_LENGTH];
	char key[KEY_LENGTH];
	char path[MAX_VALUE_LENGTH];
	
	clearBytes(tblName,MAX_TABLE_NAME_LENGTH);
	clearBytes(key,KEY_LENGTH);
	strcpy(key,FIELDNAME_KEY);

	if(true EQ cInfo->getParamByName(key,tblName))
	{
		GetObjReq   getObjReq;
		EncBuf      encGetObjBuf;

		clearBytes(&getObjReq, sizeof(GetObjReq));
		clearBytes(&encGetObjBuf, sizeof(EncBuf));

		// Lets put the table ID at the begining
		if(false EQ getTableId(tblName, &(getObjReq.tagArray.tagInfo[0].tableId)))
		{
			char resBuff[RES_BUFF_LEN];
			snprintf(resBuff,  sizeof(resBuff), "Received Table name : %s NOT FOUND...",tblName);
			logError("%s",resBuff);

			// Send proper output to web browser
			CWebResponse::getInstance().createAndSendResponse(RES_TABLE_NOT_FOUND,resBuff);
			
			// we just mark it complete, session manager
                	// will do the clean-up
                	mStatus = CMD_COMPLETED;
			
			return;
		}

		logInfo(" GET Table Id : %d", getObjReq.tagArray.tagInfo[0].tableId);

		/* Added File Broswing Support for File Upload Table */
		if( 3 EQ getObjReq.tagArray.tagInfo[0].tableId && cInfo->getParamByName((char *)"dir", path))
		{
			// Send proper output to web browser
			CWebResponse::getInstance().createAndSendResponse(RES_FILE_DETAILS, path);

			// we just mark it complete, session manager
			// will do the clean-up
			mStatus = CMD_COMPLETED;
			return;
		}
 
		// Lets make the WIH command reference
		CmdRef   wihRef;
		clearBytes(&wihRef,sizeof(CmdRef));
		
		// Generate a new transaction id
		generateTxnId();

		wihRef.instanceId = WIH_INSTANCE_ID;
		wihRef.origin     = OAMIF_WIH;
		wihRef.cmdRefNum  = mTransactionId;

		getObjReq.tagArray.numTags = 1; /* One command at a time */
		getObjReq.cmdRef = wihRef;
		// We are going to get the entire table
		// so fill rowId and colId to '0'
		getObjReq.tagArray.tagInfo[0].rowId   = 0;
		getObjReq.tagArray.tagInfo[0].colId   = 0;

		// Encode in get Object Request Message into TLV Buffer
  		encGetObjReqMsg(&getObjReq, &encGetObjBuf);
 
		// Create and Send Get Request Message to BSM 
  		createAndSendMsg(MSG_WIH_BSM_GET_OBJ_REQ, (UINT8*)&encGetObjBuf.value, encGetObjBuf.len);

		// Now we need to wait for the BSM response
		struct timespec ts;
		struct timeval tp;

		gettimeofday(&tp, NULL);

		// Converting timeval to timespec
		ts.tv_sec = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec * 1000;
		ts.tv_sec += WEBCRAFT_CMD_TIMEOUT_VAL; 
		
		pthread_mutex_lock(&g_bsm_rsp_mutex);

		g_RspRcvd = false;

		int rc;
		// Block till we get the BSM response
		while(g_RspRcvd EQ false)
		{
			// OAM interface will signal the receipt of message
			rc=pthread_cond_timedwait(&g_bsm_rsp_cv, &g_bsm_rsp_mutex,&ts);

			if(rc EQ ETIMEDOUT)
			{
				g_RspProcessed = true;
				pthread_mutex_unlock(&g_bsm_rsp_mutex);
                		char resBuff[RES_BUFF_LEN];
                		snprintf(resBuff,  sizeof(resBuff), "Command timeout occured for GET on table-> %s", tblName);
                		logError("%s",resBuff);

				// Send command time out to the web browser
				CWebResponse::getInstance().createAndSendResponse(RES_CMD_TIMEOUT,"COMMAND TIMEOUT");
				
				// we just mark it complete, session manager
                		// will do the clean-up
                		mStatus = CMD_COMPLETED;

				break;
			}
		}

		if(rc EQ ETIMEDOUT){
			g_RspProcessed = true;
			return;
		}


		logInfo("%s", "Got the BSM response received signal...");

		// Process the response, while OAM Interface waits for
		// processing to be complete
		processOamResponse(g_msgIn);
		g_RspProcessed = true;

		// After unlocking the mutex OAM Interface can proceed with
		// clean-up activity towards intercepting next message
		pthread_mutex_unlock(&g_bsm_rsp_mutex);
                
		// Command complete
		mStatus = CMD_COMPLETED;
	}
	else
	{
		// The request is not well formed
                char resBuff[RES_BUFF_LEN];
                snprintf(resBuff, sizeof(resBuff), "Request does not have 'key' parameter for table");
                logError("%s",resBuff);

                // Send proper output to web browser
                CWebResponse::getInstance().createAndSendResponse(RES_KEY_NOT_FOUND,resBuff);

                // we just mark it complete, session manager
                // will do the clean-up
                mStatus = CMD_COMPLETED;
	}

	return;
}

////////////////////////////////////////////////////////////////////
// Operation	: processOamResponse
// Description	: This method is used for processing the BSM response
//		  , the method is called after getting a trigger from 
//		  the oam msg reciving thread. 
// Input	: Msg received
// Output	: None
///////////////////////////////////////////////////////////////////
void GetCmdSession::processOamResponse(Msg *msgIn)
{
	if(msgIn EQ NULL)
	{
		logError("%s","msgIn is recieved as NULL... ");
		// Send error to the web browser
		
		return;
	}

	// Send JSON based GET Response
        CWebResponse::getInstance().createAndSendResponse(msgIn);

	// Marking the Command complete, for clean-up
	mStatus = CMD_COMPLETED;
}

////////////////////////////////////////////////////////////////////
// Operation	: terminateCmd
// Description	: This method is used for terminating the command
// Input	: None 
// Output	: None
///////////////////////////////////////////////////////////////////
void GetCmdSession::doTerminateCmd(void)
{
}


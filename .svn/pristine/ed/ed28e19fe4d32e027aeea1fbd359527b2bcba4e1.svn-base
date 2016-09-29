//////////////////////////////////////////////////////////////////////////////////
// @file    wih_setCmdSession.cpp
// @details Copyright Powerwave Technologies, Inc., 2010
// @brief This file has the methods for class SetCmdSession.
//
// Author     : Tara Shankar Maji
// Date       : 24-09-2010
// Description: This file contains methods for SET command processing functionalities.
// Change History: 24-09-10 Initial version.
///////////////////////////////////////////////////////////////////////////////////

#include "wih_setCmdSession.h"
#include "wih_sessionMgr.h"
#include "wih_webResponse.h"

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
SetCmdSession::~SetCmdSession()
{
}

////////////////////////////////////////////////////////////////////
// Operation	: getTableId 
// Description	: Gets the table ID for the specified table name  
// Input	: table name (I), table number (IO)
// Output	: true/false based on whether the name was found or not
///////////////////////////////////////////////////////////////////
bool SetCmdSession::getTableId(const char *tblName, TableId *tValue)
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
// Operation	: fillTlvInfo
// Description	: This method is used for filling the SET request
//		  based TLV information in the message.
// Input	: SetObjReq pointer (IO) , Command Info object pointer (I),
//		  TagInfo object (I) 
// Output	: true/false based on success/failure to fill the values
///////////////////////////////////////////////////////////////////
bool SetCmdSession::fillTlvInfo(SetObjReq *psetObjReq, CCommandInfo *cInfo, TagInfo   setCmdTableInfo)
{
	UINT8 iLoop, value8Bit;
        SINT8     signed8Bit;
	UINT16    value16Bit;
	SINT16    signed16Bit;
	UINT32 iNumCols,value32Bit,tempVal;
	SINT32    signed32Bit;	
	char paramVal[MAX_VALUE_LENGTH];

	OamTableInfo  *tableInfo = getTablePtrByTableId(setCmdTableInfo.tableId);

	logInfo(" Table Id : %d", setCmdTableInfo.tableId);
	iNumCols = tableInfo->noOfColumns;

	logInfo(" Number of columns in table : %d", iNumCols);

	psetObjReq->tlvArray.numTlvs = 0;
	for(iLoop=0;iLoop<iNumCols;iLoop++)
	{
		logInfo(" Column Name : %s", getColName(setCmdTableInfo.tableId,iLoop+1));

		if(true EQ cInfo->getParamByName((char *)getColName(setCmdTableInfo.tableId,iLoop+1),paramVal))
		{
			psetObjReq->tlvArray.tlvInfo[psetObjReq->tlvArray.numTlvs].tag.tableId  = setCmdTableInfo.tableId;
			psetObjReq->tlvArray.tlvInfo[psetObjReq->tlvArray.numTlvs].tag.rowId    = setCmdTableInfo.rowId;
			psetObjReq->tlvArray.tlvInfo[psetObjReq->tlvArray.numTlvs].tag.colId    = (iLoop+1);
			UINT16    *colLen;
			colLen  = getColLength(setCmdTableInfo.tableId, (iLoop+1));
			psetObjReq->tlvArray.tlvInfo[psetObjReq->tlvArray.numTlvs].len = *colLen;
				
			// Based on the column type we need to put the value
			DataType  *colType;
			colType = getColDataType(setCmdTableInfo.tableId, (iLoop+1));
			
			if(*colType EQ BS_TYPE_STRING )
			{
				strncpy((char *)psetObjReq->tlvArray.tlvInfo[psetObjReq->tlvArray.numTlvs].data,paramVal,*colLen);
			}
			else if ( *colType EQ BS_TYPE_UINT8 )
			{
				sscanf(paramVal,"%d",&tempVal);
				value8Bit = (UINT8)tempVal;
				putUint8InArray( value8Bit,psetObjReq->tlvArray.tlvInfo[psetObjReq->tlvArray.numTlvs].data);
			}
			else if ( *colType EQ BS_TYPE_SINT8 )
			{
				sscanf(paramVal,"%d",&tempVal);
				signed8Bit = (SINT8)tempVal;
				putUint8InArray( signed8Bit,psetObjReq->tlvArray.tlvInfo[psetObjReq->tlvArray.numTlvs].data);
			}
			else if ( *colType EQ BS_TYPE_UINT16 )
			{
				sscanf(paramVal,"%d",&tempVal);
				value16Bit = (UINT16)tempVal;
				putUint16InArray( value16Bit,psetObjReq->tlvArray.tlvInfo[psetObjReq->tlvArray.numTlvs].data);
			}
			else if ( *colType EQ BS_TYPE_SINT16 )
			{
				sscanf(paramVal,"%d",&tempVal);
				signed16Bit = (SINT16)tempVal;
				putUint16InArray( signed16Bit,psetObjReq->tlvArray.tlvInfo[psetObjReq->tlvArray.numTlvs].data);
			}
			else if (( *colType EQ BS_TYPE_UINT32 )||(*colType EQ BS_TYPE_MODID)||(*colType EQ BS_TYPE_MSGID) || (*colType EQ BS_TYPE_COUNTERID))
			{
				sscanf(paramVal,"%d",&value32Bit);
				putUint32InArray( value32Bit,psetObjReq->tlvArray.tlvInfo[psetObjReq->tlvArray.numTlvs].data);
			}
			else if (*colType EQ BS_TYPE_SINT32 )
			{
				sscanf(paramVal,"%d",&signed32Bit);
				putUint32InArray( signed32Bit,psetObjReq->tlvArray.tlvInfo[psetObjReq->tlvArray.numTlvs].data);
			}
			else if ( *colType EQ BS_TYPE_ENUM )
			{
				// Web request needs to send the exact ENUM value --- NOT NAME
				sscanf(paramVal,"%d",&value32Bit);
				putUint32InArray( value32Bit,psetObjReq->tlvArray.tlvInfo[psetObjReq->tlvArray.numTlvs].data);
			}
			else if ( *colType EQ BS_TYPE_BOOL )
			{
				sscanf(paramVal,"%d",&tempVal);
				value8Bit = (UINT8)tempVal;
				putUint8InArray( value8Bit,psetObjReq->tlvArray.tlvInfo[psetObjReq->tlvArray.numTlvs].data);
			}
			else  // Treat as string
			{
				strncpy((char *)psetObjReq->tlvArray.tlvInfo[psetObjReq->tlvArray.numTlvs].data,paramVal,*colLen);
			}

			psetObjReq->tlvArray.numTlvs++;
		}
	}	

	if( psetObjReq->tlvArray.numTlvs > 0)
	{
		return true;
	}
	else
	{
		logError("%s","No TLV could be created, possibly due to parameter name MISMATCH !!!");
		return false;
	}
}

////////////////////////////////////////////////////////////////////
// Operation	: doExecuteCmd
// Description	: This method is used for executing the command
// Input	: Command Info object pointer 
// Output	: none
///////////////////////////////////////////////////////////////////
void SetCmdSession::doExecuteCmd(CCommandInfo *cInfo)
{
	char tblName[MAX_TABLE_NAME_LENGTH];
	char key[KEY_LENGTH];
	
	clearBytes(tblName,MAX_TABLE_NAME_LENGTH);
	clearBytes(key,KEY_LENGTH);
	strcpy(key,FIELDNAME_KEY);

	if(true EQ cInfo->getParamByName(key,tblName))
        {
		SetObjReq   setObjReq;
		EncBuf      encSetObjBuf;
		TagInfo   setCmdTableInfo;

		clearBytes(&setObjReq, sizeof(SetObjReq));
		clearBytes(&encSetObjBuf, sizeof(EncBuf));
		clearBytes(&setCmdTableInfo, sizeof(TagInfo));

		// Lets find the table id as per table name at the begining
		if(false EQ getTableId(tblName, &(setCmdTableInfo.tableId)))
		{
			char resBuff[RES_BUFF_LEN];
                        snprintf(resBuff, sizeof(resBuff), "Received Table name : %s NOT FOUND...", tblName);
                        logError("%s",resBuff);

                        // Send proper output to web browser
                        CWebResponse::getInstance().createAndSendResponse(RES_TABLE_NOT_FOUND,resBuff);
			
			// we just mark it complete, session manager
                	// will do the clean-up
                	mStatus = CMD_COMPLETED;
			
			return;
		}

		// SET command works for each row of the table, and does not
		// work on the entire table in a single go. So if we have a
		// table with multiple rows, GUI needs to send multiple SET
		// commands for the same table, with rowid for all SET requests.

		char tblRow[ROW_LENGTH], tblRowId[KEY_LENGTH];
		clearBytes(tblRow,ROW_LENGTH);
		clearBytes(tblRowId,KEY_LENGTH);
		strcpy(tblRow,FIELDNAME_ROW);

		// Row ID needs to be put, if it has been sent by the GUI
		// We check for the parameter named 'webGuiRow' in the request
   		// parameters, if its found we update the row no. for TLV,
		// else we put row id as 1 always by default.

		if(true EQ cInfo->getParamByName(tblRow,tblRowId))
		{
			// setting the table row as received from GUI
			setCmdTableInfo.rowId = atoi(tblRowId);
		}
		else
		{
			// default row id set to 1
			setCmdTableInfo.rowId = 1;
		}  

		// Lets now fill the SET request body before encoding
		if(false EQ fillTlvInfo(&setObjReq, cInfo, setCmdTableInfo))
		{
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

		setObjReq.cmdRef = wihRef;

		// Encode in set Object Request Message into TLV Buffer
		encSetObjReqMsg(&setObjReq, &encSetObjBuf);

		pthread_mutex_lock(&g_bsm_rsp_mutex);
 
		// Create and Send Set Request Message to BSM 
		createAndSendMsg(MSG_WIH_BSM_SET_OBJ_REQ, (UINT8*)&encSetObjBuf.value, encSetObjBuf.len);

		// Now we need to wait for the BSM response
                struct timespec ts;
                struct timeval tp;

                gettimeofday(&tp, NULL);

                // Converting timeval to timespec
                ts.tv_sec = tp.tv_sec;
                ts.tv_nsec = tp.tv_usec * 1000;
                ts.tv_sec += WEBCRAFT_CMD_TIMEOUT_VAL;

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
                                snprintf(resBuff, sizeof(resBuff), "Command timeout occured for SET on table-> %s", tblName);
                                logError("%s",resBuff);

                                // Send command time out to the web browser
                                CWebResponse::getInstance().createAndSendResponse(RES_CMD_TIMEOUT,"COMMAND TIMEOUT");

                                // we just mark it complete, session manager
                                // will do the clean-up
                                mStatus = CMD_COMPLETED;

				break;
                        }
                }

                if(rc EQ ETIMEDOUT)
                {
                	g_RspProcessed = true;
                    return;
                }

                logInfo("%s", "Got the BSM response received signal...");

		// Process the response, while OAM Interface waits for
		// processing to be complete
		processOamResponse(g_msgIn);

		// After unlocking the mutex OAM Interface can proceed with
		// clean-up activity towards intercepting next message
		pthread_mutex_unlock(&g_bsm_rsp_mutex);
		g_RspProcessed = true;

		// Command Complete
		mStatus = CMD_COMPLETED;
        }
	else
	{
		// The request is not well formed
		char resBuff[RES_BUFF_LEN];
                snprintf(resBuff,  sizeof(resBuff), "Request does not have 'key' parameter for table");
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
void SetCmdSession::processOamResponse(Msg *msgIn)
{
	if(msgIn EQ NULL )
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
void SetCmdSession::doTerminateCmd(void)
{
}


////////////////////////////////////////////////////////////////////
// Operation	: getTableTypeByTableId
// Description	: This method is used for obtaining the type of OAM
//		  table based upon the table id passed as argument.
// Input	: Table Id
// Output	: OamTableType
///////////////////////////////////////////////////////////////////
OamTableType SetCmdSession::getTableTypeByTableId( TableId tableId )
{
  if (tableId >= MIN_STATUS_TABLEID && tableId <= MAX_STATUS_TABLEID)
    return TABLE_STATUS;
  else if (tableId >= MIN_PRE_CFG_TABLEID && tableId <= MAX_PRE_CFG_TABLEID)
    return TABLE_PRECONFIG;
  else if (tableId >= MIN_OPR_CFG_TABLEID && tableId <= MAX_OPR_CFG_TABLEID)
    return TABLE_OPERCONFIG;
  else if (tableId >= MIN_OPR_CMD_TABLEID && tableId <= MAX_OPR_CMD_TABLEID)
    return TABLE_OPERCMD;
  else
    return TABLE_UNKNOWNTYPE;
}


////////////////////////////////////////////////////////////////////
// Operation	: getTablePtrByTableId
// Description	: This method is used for getting the pointer towards
//		  the table's information (contents like data, datatype, etc.)
// Input	: Table id
// Output	: OamTableInfo*
///////////////////////////////////////////////////////////////////
OamTableInfo* SetCmdSession::getTablePtrByTableId( TableId tableId )
{
  OamTableInfo *tableInfo = NULL ;
  OamTableType  tableType = getTableTypeByTableId(tableId);

  switch(tableType)
  {
    case TABLE_OPERCONFIG:
    tableInfo = wihGetOprCfgTableInfo(tableId-MIN_OPR_CFG_TABLEID);
    break;

    case TABLE_OPERCMD:
    tableInfo = wihGetOprCmdTableInfo(tableId-MIN_OPR_CMD_TABLEID);
    break;

    case TABLE_STATUS:
    tableInfo = wihGetStatusTableInfo(tableId-MIN_STATUS_TABLEID);
    break;

    case TABLE_PRECONFIG:
    tableInfo = wihGetPreCfgTableInfo(tableId-MIN_PRE_CFG_TABLEID);
    break;

    case TABLE_UNKNOWNTYPE:
    default:
    tableInfo = NULL ;
  }
  return tableInfo;
}


////////////////////////////////////////////////////////////////////
// Operation	: getColName
// Description	: This method is used for getting the column's name by
//		  specifying a table id and the column id whose name
//		  is needed.
// Input	: Table id , Column id
// Output	: ColName*
///////////////////////////////////////////////////////////////////
ColName* SetCmdSession::getColName( TableId tableId, ColId colId )
{
  OamTableType  tableType = getTableTypeByTableId(tableId);

  switch(tableType)
  {
    case TABLE_OPERCONFIG:
      return &(g_OamTableContext.oprCfgTableInfo[tableId-MIN_OPR_CFG_TABLEID].colInfo[colId].colName);
    case TABLE_OPERCMD:
      return &(g_OamTableContext.oprCmdTableInfo[tableId-MIN_OPR_CMD_TABLEID].colInfo[colId].colName);
    case TABLE_STATUS:
      return &(g_OamTableContext.statusTableInfo[tableId-MIN_STATUS_TABLEID].colInfo[colId].colName);
    case TABLE_PRECONFIG:
      return &(g_OamTableContext.preCfgTableInfo[tableId-MIN_PRE_CFG_TABLEID].colInfo[colId].colName);
    default:
      return NULL;
  }
}


////////////////////////////////////////////////////////////////////
// Operation	: getColDataType
// Description	: This method is used for obtaining the data type of
//		  the column for a table
// Input	: Table id , Column id
// Output	: DataType*
///////////////////////////////////////////////////////////////////
DataType* SetCmdSession::getColDataType( TableId tableId, ColId colId )
{
  OamTableType  tableType = getTableTypeByTableId(tableId);
  switch(tableType)
  {
    case TABLE_OPERCONFIG:
      return &(g_OamTableContext.oprCfgTableInfo[tableId-MIN_OPR_CFG_TABLEID].colInfo[colId].dataType);
    case TABLE_OPERCMD:
      return &(g_OamTableContext.oprCmdTableInfo[tableId-MIN_OPR_CMD_TABLEID].colInfo[colId].dataType);
    case TABLE_STATUS:
      return &(g_OamTableContext.statusTableInfo[tableId-MIN_STATUS_TABLEID].colInfo[colId].dataType);
    case TABLE_PRECONFIG:
      return &(g_OamTableContext.preCfgTableInfo[tableId-MIN_PRE_CFG_TABLEID].colInfo[colId].dataType);
    default:
      return NULL;
  }
}


////////////////////////////////////////////////////////////////////
// Operation	: getColLength
// Description	: This method is used for obtaining the length of
//		  the column for a table
// Input	: Table id , Column id
// Output	: UINT16*
///////////////////////////////////////////////////////////////////
UINT16* SetCmdSession::getColLength( TableId tableId, ColId colId )
{
  OamTableType  tableType = getTableTypeByTableId(tableId);
  switch(tableType)
  {
    case TABLE_OPERCONFIG:
      return &(g_OamTableContext.oprCfgTableInfo[tableId-MIN_OPR_CFG_TABLEID].colInfo[colId].length);
    case TABLE_OPERCMD:
      return &(g_OamTableContext.oprCmdTableInfo[tableId-MIN_OPR_CMD_TABLEID].colInfo[colId].length);
    case TABLE_STATUS:
      return &(g_OamTableContext.statusTableInfo[tableId-MIN_STATUS_TABLEID].colInfo[colId].length);
    case TABLE_PRECONFIG:
      return &(g_OamTableContext.preCfgTableInfo[tableId-MIN_PRE_CFG_TABLEID].colInfo[colId].length);
    default:
      return NULL;
  }
}


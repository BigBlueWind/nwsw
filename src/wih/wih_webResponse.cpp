//////////////////////////////////////////////////////////////////////////////////
// @file    wih_webResponse.cpp
// @details Copyright Powerwave Technologies, Inc., 2010
// @brief This file has the methods for class CWebResponse.
//
// Author     : Tara Shankar Maji
// Date       : 17-09-2010
// Description: This file contains methods for creating and sending web browser based JSON
//		format responses.
// Change History: 17-09-10 Initial version.
///////////////////////////////////////////////////////////////////////////////////

#include "wih_webResponse.h"
#include "fcgi_stdio.h"
extern "C"
{
#include "enumstr.h"
}


extern OamTableContext g_OamTableContext;

CWebResponse * CWebResponse::mInstance = NULL;

////////////////////////////////////////////////////////////////////
// Operation	:  
// Description	: Constructor 
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
CWebResponse::CWebResponse()
{
}

////////////////////////////////////////////////////////////////////
// Operation	: 
// Description	: Destructor  
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
CWebResponse::~CWebResponse()
{
}

/* Get Column Name As String */
ColName* CWebResponse::getColName( TableId tableId, ColId colId )
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


/* Get Type of the column i.e Integer or Enum or String... */
DataType* CWebResponse::getColDataType( TableId tableId, ColId colId )
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

OamTableType CWebResponse::getTableTypeByTableId( TableId tableId )
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
// Operation	: sendStaticPage
// Description	: This method is used for sending appropriate
// 		  static page to the  web client as response over 
//     		  the web server's FastCGI interface. 
// Input	: Command Info object pointer
// Output	: none
///////////////////////////////////////////////////////////////////
void CWebResponse::sendStaticPage(CCommandInfo *pCmdInfo)
{
	// Get the requested static page name
        const char* cmdName = pCmdInfo->getCmdName();

        printf("X-Sendfile: %s/%s\r\n\r\n", DOC_ROOT, cmdName);
}

////////////////////////////////////////////////////////////////////
// Operation	: displayObjValue
// Description	: This method is used for printing the output for
//		  a column value as per the received BSM TLV based o/p 
// Input	: TlvInfo object pointer
// Output	: none
///////////////////////////////////////////////////////////////////
void CWebResponse::displayObjValue(TlvInfo  *objTlvInfo)
{
	ColName        *colName;
	colName   = getColName(objTlvInfo->tag.tableId,objTlvInfo->tag.colId);

	DataType       *dataType;
	dataType  = getColDataType(objTlvInfo->tag.tableId,objTlvInfo->tag.colId);
	
	if(*dataType EQ BS_TYPE_STRING)
	{
		printf("{\"param\":\"%s\",\"value\":\"%s\"}",colName,objTlvInfo->data);
		logInfo("Column : %s , Value %s",colName,objTlvInfo->data);
	}
	else if(*dataType EQ BS_TYPE_UINT32)
	{
		UINT32 value32;
		value32 = getUint32FromArray(objTlvInfo->data);
		printf("{\"param\":\"%s\",\"value\":\"%u\"}",colName,value32);
		logInfo("Column : %s , Value %u",colName,value32);
	}
	else if(*dataType EQ BS_TYPE_SINT32)
	{
		SINT32 value32;
		value32 = getUint32FromArray(objTlvInfo->data);
		printf("{\"param\":\"%s\",\"value\":\"%d\"}",colName,value32);
		logInfo("Column : %s , Value %d",colName,value32);
	}
	else if(*dataType EQ BS_TYPE_UINT16) 
	{
		UINT16 value16;
		value16 = getUint16FromArray(objTlvInfo->data);
		printf("{\"param\":\"%s\",\"value\":\"%u\"}",colName,value16);
		logInfo("Column : %s , Value %u",colName,value16);
	}
	else if(*dataType EQ BS_TYPE_SINT16)
	{
		SINT16 value16;
		value16 = getUint16FromArray(objTlvInfo->data);
		printf("{\"param\":\"%s\",\"value\":\"%d\"}",colName,value16);
		logInfo("Column : %s , Value %d",colName,value16);
	}
	else if(*dataType EQ BS_TYPE_UINT8)
	{
		UINT8	value8;
		value8 = getUint8FromArray(objTlvInfo->data);
		printf("{\"param\":\"%s\",\"value\":\"%u\"}",colName,value8);
		logInfo("Column : %s , Value %u",colName,value8);
	}
	else if(*dataType EQ BS_TYPE_SINT8)
	{
		SINT8	value8;
		value8 = getUint8FromArray(objTlvInfo->data);
		printf("{\"param\":\"%s\",\"value\":\"%d\"}",colName,value8);
		logInfo("Column : %s , Value %d",colName,value8);
	}
	else if(*dataType EQ BS_TYPE_ENUM)
	{
		// We are sending ENUM value, GUI will show in the drop-down
		// and point to the exact element in the drop-down list
		UINT32 value32;
		value32 = getUint32FromArray(objTlvInfo->data);
		printf("{\"param\":\"%s\",\"value\":\"%d\"}",colName,value32);
		logInfo("Column : %s , Value %d",colName,value32);
	}
	else if(*dataType EQ BS_TYPE_BOOL)
	{
		UINT8	value8;
		value8 = getUint8FromArray(objTlvInfo->data);
		printf("{\"param\":\"%s\",\"value\":\"%d\"}",colName,value8);
		logInfo("Column : %s , Value %d",colName,value8);
	}
	else if(*dataType EQ BS_TYPE_MODID)
        {
                UINT32 value32;
                value32 = getUint32FromArray(objTlvInfo->data);
                printf("{\"param\":\"%s\",\"value\":\"%s\"}",colName,(char *)enumModIdToStr(value32));
                logInfo("Column : %s , Value %s",colName,(char *)enumModIdToStr(value32));
        }
	else if(*dataType EQ BS_TYPE_MSGID)
        {
                UINT32 value32;
                value32 = getUint32FromArray(objTlvInfo->data);
                MsgId  msgId = (MsgId)value32;
                printf("{\"param\":\"%s\",\"value\":\"%s\"}",colName,(char*)getMsgName(msgId));
                logInfo("Column : %s , Value %s",colName,(char *)getMsgName(msgId));
        }
        else if(*dataType EQ BS_TYPE_COUNTERID)
        {
                UINT32 value32;
                value32 = getUint32FromArray(objTlvInfo->data);
                CounterId  counterId = (CounterId)value32;
                printf("{\"param\":\"%s\",\"value\":\"%s\"}",colName,(char*)getCounterIdStr(counterId));
                logInfo("Column : %s , Value %s",colName,(char *)getCounterIdStr(counterId));
        }
	else // treat is as string only
	{
		printf("{\"param\":\"%s\",\"value\":\"%s\"}",colName,objTlvInfo->data);
		logInfo("Column : %s , Value %s",colName,objTlvInfo->data);
	}
	
}

////////////////////////////////////////////////////////////////////
// Operation	: createAndSendResponse
// Description	: This method is used for creating and sending JSON
// 		  based web client response over the web server's 
//		  FastCGI interface for GET/SET requests
// Input	: OAM Message pointer
// Output	: none
///////////////////////////////////////////////////////////////////
void CWebResponse::createAndSendResponse(Msg *msgIn)
{
	// Check the msg type and send JSON response
	switch(msgIn->msgId)
	{
		case MSG_BSM_WIH_GET_OBJ_SUCC:
		case MSG_BSM_WIH_SET_OBJ_SUCC:
		{
			GetObjRspSucc   rspSucc;
		    clearBytes( &rspSucc, sizeof(GetObjRspSucc));

		    // Decode TLV's for Get Message
		    decGetObjRspSuccMsg((UINT8*)&(msgIn->data[0]), msgIn->len, &rspSucc );

			printf("Content-Type: text/plain;charset=us-ascii\n\n");
                	printf("{\n");
                	printf("\"result\":\"SUCCESS\",");
                	printf("\"data\":");

			// In case no valid rows are found i.e. no. of TLVs is 0, 
			// then result as SUCCESS with empty data needs to be sent
			if(rspSucc.tlvArray.numTlvs NOTEQ 0)
			{
				printf("{\"row\":[");

				UINT32 iTlv, iRow, iCol;
				bool rowSame=false;
				iRow=iCol=0;
			
				for(iTlv=0;iTlv<rspSucc.tlvArray.numTlvs;iTlv++)
				{
					if(rspSucc.tlvArray.tlvInfo[iTlv].tag.rowId NOTEQ iRow)
					{
						if(iRow NOTEQ 0)
						{
							printf("\n]\n},");
						}

						iRow = rspSucc.tlvArray.tlvInfo[iTlv].tag.rowId;
						rowSame = false;
						printf("{\"col\":[");
					}
					else
					{
						rowSame = true;
					}

					if(rowSame EQ true)
					printf(",\n");

					displayObjValue(&rspSucc.tlvArray.tlvInfo[iTlv]);
				
				}
 
				printf("\n]\n}\n]\n}"); // close the last col and row 
	              		printf("\n}");
			}
			else
			{
				printf("\"\"\n}");
			}

			break;
		}
		case MSG_BSM_WIH_GET_OBJ_FAIL:
		{
			GetObjRspFail   rspFail;
		    clearBytes( &rspFail, sizeof(GetObjRspFail));

   		    // Decode TLV's for Get Message
		    decGetObjRspFailMsg( (UINT8*)&(msgIn->data[0]), msgIn->len, &rspFail );

			printf("Content-Type: text/plain;charset=us-ascii\n\n");
            printf("{\n");
            printf("\"result\":\"FAIL\",");
            printf("\"data\":\"%s\"", errorCodeStr(rspFail.getErrRsp.errRspCode));
            printf("\n}");
			break;
		}
		case MSG_BSM_WIH_SET_OBJ_FAIL:
		{
			SetObjRspFail   rspFail;
			clearBytes( &rspFail, sizeof(SetObjRspFail));

			// Decode TLV's for Set Message
			decSetObjRspFailMsg( (UINT8*)&(msgIn->data[0]), msgIn->len, &rspFail );

			printf("Content-Type: text/plain;charset=us-ascii\n\n");
                	printf("{\n");
                	printf("\"result\":\"FAIL\",");
                	printf("\"data\":\"%s\"",
			errorCodeStr(rspFail.setErrRsp.errRspCode));
                	printf("\n}");
			
			break;
		}
		default:
		{
			logError("Got UNKNOWN message type %d",msgIn->msgId);
		}
	}
}

////////////////////////////////////////////////////////////////////
// Operation	: createAndSendResponse
// Description	: This method is used for creating and sending JSON
// 		  based web client response over the web server's 
//		  FastCGI interface. 
// Input	: response code, response string
// Output	: none
///////////////////////////////////////////////////////////////////
void CWebResponse::createAndSendResponse(const enRspCode eRsp, const char *eStr)
{
	SINT32 index = 0;
	struct dirent **namelist;
	SINT32 noOfContents = 0;
	const char  *path=eStr;
	if ( eRsp EQ RES_TABLE_NOT_FOUND ||
	     eRsp EQ RES_PARAM_NOT_FOUND ||
	     eRsp EQ RES_INCOMPLETE_PARAMS ||
	     eRsp EQ RES_KEY_NOT_FOUND ||
	     eRsp EQ RES_CMD_TIMEOUT
	)
	{
		printf("Content-Type: text/plain;charset=us-ascii\n\n");
		printf("{\n");
		printf("\"result\":\"FAIL\",");
		printf("\"data\":\"%s\"",eStr);
		printf("\n}");
	}
	else if( eRsp EQ RES_FILE_DETAILS )
	{
		noOfContents = scandir(path, &namelist, 0, alphasort);
		printf("Content-Type: text/plain;charset=us-ascii\n\n");
		printf("<ul class=\"jqueryFileTree\" style=\"display: none;\">");
        if (noOfContents <= 0)
        {
        	printf("</ul>");
        	return;
        }
        for( index = 0; index < noOfContents; index++ )
        {
        	if( namelist[index]->d_type EQ DT_DIR || namelist[index]->d_type EQ DT_LNK
        		|| namelist[index]->d_type EQ DT_REG )
        	{
        		if( namelist[index]->d_name[0] EQ '.' )
        		{
        			continue;
        		}
        		if( namelist[index]->d_type EQ DT_DIR)
        		{
        			printf("<li class=\"directory collapsed\">");
        			printf("<a href=\"#\" rel=\"%s%s/\">", path, namelist[index]->d_name);
        			printf("%s</a></li>", namelist[index]->d_name);
        		}
        		else
        		{
        			printf("<li class=\"file\">");
        			printf("<a href=\"#\" rel=\"%s%s\">", path, namelist[index]->d_name);
        			printf("%s</a></li>", namelist[index]->d_name);
        		}
        	}
			free(namelist[index]);
        }
        free(namelist);
        printf("</ul>");
	}
	return;
}

////////////////////////////////////////////////////////////////////
// Operation	: createAndSendResponse
// Description	: This method is used for creating and sending JSON
// 		  based web client response over the web server's 
//		  FastCGI interface. 
// Input	: response code, context identifier
// Output	: none
///////////////////////////////////////////////////////////////////
void CWebResponse::createAndSendResponse(const enRspCode eRsp, const ContextID cid)
{
	if(eRsp EQ RES_LOGIN_SUCCESS)
	{
		printf("Content-Type: text/plain;charset=us-ascii\n\n");
                printf("{\n");
                printf("\"result\":\"SUCCESS\",");
                printf("\"data\":\"%d\"",cid);
                printf("\n}");
	}
	else if(eRsp EQ RES_LOGIN_FAIL)
	{
		printf("Content-Type: text/plain;charset=us-ascii\n\n");
                printf("{\n");
                printf("\"result\":\"FAIL\",");
                printf("\"data\":\"%s\"","Invalid username OR password. Please Try Again.");
                printf("\n}");
	}
	else if(eRsp EQ RES_BUSY)
	{
		printf("Content-Type: text/plain;charset=us-ascii\n\n");
                printf("{\n");
                printf("\"result\":\"FAIL\",");
                printf("\"data\":\"%s\"","Maximum Webcraft Users operating. Please Try Again.");
                printf("\n}");
	}
	else if(eRsp EQ RES_CMD_NOT_SUPPORTED)
	{
		printf("Content-Type: text/plain;charset=us-ascii\n\n");
                printf("{\n");
                printf("\"result\":\"FAIL\",");
                printf("\"data\":\"%s\"","Command NOT Supported.");
                printf("\n}");
	}
	else if(eRsp EQ RES_INACTIVE_LOGOUT)
	{
		printf("Content-Type: text/plain;charset=us-ascii\n\n");
                printf("{\n");
                printf("\"result\":\"FAIL\",");
                printf("\"data\":\"%s\"","User Logged out due to INACTIVITY.");
                printf("\n}");
	}
	else if (eRsp EQ RES_LOGOUT_SUCCESS)
	{
		printf("Content-Type: text/plain;charset=us-ascii\n\n");
                printf("{\n");
                printf("\"result\":\"SUCCESS\"");
                printf("\n}");
	}
}

////////////////////////////////////////////////////////////////////
// Operation	: getInstance
// Description	: This method returns the reference to the singleton 
//		  object for web client response.
// Input	: None
// Output	: CWebResponse object reference
///////////////////////////////////////////////////////////////////
CWebResponse & CWebResponse::getInstance(void)
{
	if(mInstance EQ NULL) {
		mInstance = new CWebResponse;
	}

	return *mInstance;
}

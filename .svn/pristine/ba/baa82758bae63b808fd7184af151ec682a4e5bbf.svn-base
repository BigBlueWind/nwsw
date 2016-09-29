//////////////////////////////////////////////////////////////////////////////////
// @file    wih_commandInfo.cpp
// @details Copyright Powerwave Technologies, Inc., 2010
// @brief This file has the methods for class CCommandInfo. This is used for 
//	    storing the web request based parameters for further processing.	
// Author     : Tara Shankar Maji
// Date       : 13-09-2010
// Description: This file contains methods for web request parameters.
// Change History: 13-09-10 Initial version.
///////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include "wih.h"
#include "wih_commandInfo.h"
#include "fcgi_stdio.h"

////////////////////////////////////////////////////////////////////
// Operation	:  
// Description	: Constructor 
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
CCommandInfo::CCommandInfo()
{
	mContextId = 0;
	memset(mReqPageName,0,sizeof(mReqPageName));
	memset(&mParams,0,sizeof(mParams));
	mTotalParams = 0;
}

////////////////////////////////////////////////////////////////////
// Operation	: 
// Description	: Destructor  
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
CCommandInfo::~CCommandInfo()
{
}

////////////////////////////////////////////////////////////////////
// Operation	: parseParams 
// Description	: This method is used for parsing out the parameter
//		  name and value pairs as received in the HTTP request.  
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
void CCommandInfo::parseParams(const char *pQryStr)
{
	char sWebReqParamStr[MAX_QUERY_STR_LENGTH]={0};
        unsigned int ucCount=0;
        char *pcPos;
        int iCnt;

	strncpy(sWebReqParamStr,pQryStr,sizeof(sWebReqParamStr));
	pcPos = sWebReqParamStr;

        for(;ucCount<=MAX_REQ_PARAMS;++ucCount)
        {
                iCnt=0;
                while(*pcPos NOTEQ '='&& iCnt<(MAX_PARAM_NAME_LENGTH-1))
                {
                        mParams[ucCount].sName[iCnt]=*pcPos;
                        pcPos++; iCnt++;
                }
                mParams[ucCount].sName[iCnt+1]='\0';
                pcPos++; iCnt=0;
                while((*pcPos NOTEQ '&' && pcPos NOTEQ '\0')&& iCnt<(MAX_VALUE_LENGTH-1))
                {
                        mParams[ucCount].sValue[iCnt]=*pcPos;
                        pcPos++; iCnt++;
                }
                mParams[ucCount].sValue[iCnt+1]='\0';

		unescapeValue(mParams[ucCount].sValue);

                pcPos++;

                if(*pcPos EQ '\0')
                break;
        }

        mTotalParams = ++ucCount;
}

////////////////////////////////////////////////////////////////////
// Operation	: parseWebRequest
// Description	: This method is used for parsing the HTTP query
//		  string and filling the parameters in a structure
//		  for using it further for processing. 
// Input	: None 
// Output	: true/false based on parsing
///////////////////////////////////////////////////////////////////
bool CCommandInfo::parseWebRequest(void)
{
	// Get the web request parameters and update them in gWebCmdDetail
	char sScriptName[MAX_SCRIPT_LENGTH] = {0};
	char sTempStr[MAX_SCRIPT_LENGTH] = {0};

	strncpy(sScriptName,getenv("SCRIPT_NAME"),sizeof(sScriptName));
	strncpy(sTempStr,strtok(sScriptName,"/"), sizeof(sTempStr));

	// Fill the Context ID if present	
	if(strstr(sTempStr,"-") NOTEQ NULL)
        {
                mContextId = atoi(strtok(sTempStr,"-"));
	
		// Fill the requested page name
        	strncpy(mReqPageName,strstr(sScriptName,"-")+1,
			sizeof(mReqPageName));
        }
	else
	{
		//Fill the requested page name
		strncpy(mReqPageName,strstr(sScriptName,"/")+1,
			sizeof(mReqPageName));
	}


        /******* Check if we have parameters *******/
	// Depending upon the request being HTTP GET or HTTP POST the
	// mechanism of reading the received parameter string is different.

	char QueryString[MAX_QUERY_STR_LENGTH];
	memset(QueryString,0,sizeof(QueryString));

	// For HTTP-POST
	char *contentLength = getenv("CONTENT_LENGTH");

	int len;

	if (contentLength NOTEQ  NULL) {
            len = strtol(contentLength, NULL, 10);
        }
        else {
            len = 0;
        }

	if (len <= 0) {
        }
        else {
            int i, ch;
		
	    if(len > (MAX_QUERY_STR_LENGTH-1)) {
		logError("Need to increase query string length from %d as %d is rcvd.",
			MAX_QUERY_STR_LENGTH, len);
	    }

            for (i = 0; i < len; i++) {
                if ((ch = getchar()) < 0) {
		    logError("%s","Error: Not enough bytes received on standard input....");                    
		    break;
                }
		QueryString[i] = ch;
                //putchar(ch);
            }
	    QueryString[i+1] = '\0';

	    logInfo("WebCraft Query String : %s",QueryString);
	    parseParams(QueryString);
        }

	return true;
}

////////////////////////////////////////////////////////////////////
// Operation	: getContextId 
// Description	: Gets the web request's context id 
// Input	: None
// Output	: Command's context identifier
///////////////////////////////////////////////////////////////////
ContextID CCommandInfo::getContextId(void)
{
	return mContextId;
}

////////////////////////////////////////////////////////////////////
// Operation	: getTotalParamsRcvd 
// Description	: Gets total number of web request's parameters 
// Input	: None
// Output	: Number of parameters received
///////////////////////////////////////////////////////////////////
UINT32 CCommandInfo::getTotalParamsRcvd(void)
{
	return (UINT32)mTotalParams;
}


////////////////////////////////////////////////////////////////////
// Operation	: getCmdName 
// Description	: Gets the web request's command name (web page name) 
// Input	: None
// Output	: Command's Name
///////////////////////////////////////////////////////////////////
const char* CCommandInfo::getCmdName(void)
{
	return &mReqPageName[0];
}

////////////////////////////////////////////////////////////////////
// Operation	: getParamByName 
// Description	: Gets the value for the parameter specified 
// Input	: parameter name
// Output	: true/false based on found/not-found
///////////////////////////////////////////////////////////////////
bool CCommandInfo::getParamByName(char *pName, char *pValue)
{
	if(mTotalParams>0)
	{
		for(int count=0;count<mTotalParams;count++)
		{
			if(!strcmp(pName,mParams[count].sName))
			{
				strcpy(pValue,mParams[count].sValue);
				return true;
			}
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////
// Operation	: unescapeValue 
// Description	: unescape the value string based on URL encoding &
//		  decoding rules 
// Input	: value string
// Output	: None
///////////////////////////////////////////////////////////////////
void CCommandInfo::unescapeValue(char *valString)
{
	int localcount, digit, count2;

	for (localcount = 0; valString[localcount]; localcount++) // While current char is not \0
	{
        	// Convert '+' to ' ' in query
                if (valString[localcount] EQ '+')
                        valString[localcount] = ' ';

                // Convert "%xx" to ASCII character
                if (valString[localcount] EQ '%')
                {
                        digit = (valString[localcount + 1] >= 'A' ?
                                ((valString[localcount + 1] & 0xdf) - 'A') +
                                10 : (valString[localcount + 1] - '0'));
                        digit *= 16;
                        digit += (valString[localcount + 2] >= 'A' ?
                                ((valString[localcount + 2] & 0xdf) - 'A') +
                                10 : (valString[localcount + 2] - '0'));

                        valString[localcount] = (char) digit;
                        for (count2 = (localcount + 3); valString[count2]; count2++)
                                valString[count2 - 2] = valString[count2];
                        valString[count2 - 2] = '\0';
                }
	}
}

////////////////////////////////////////////////////////////////////
// Operation	: dumpContent 
// Description	: Dumps the contents od this object, for testing 
// Input	: None
// Output	: None
///////////////////////////////////////////////////////////////////
void CCommandInfo::dumpContent(void)
{
                        unsigned char ucCount = 0;
		
			logInfo("%s","Contents of CCommandInfo OBJECT are...."); 
                        logInfo(" Context ID : %d ", mContextId);
                        logInfo(" Requested Page Name : %s",mReqPageName);
                        if(mTotalParams>0)
                        {
                                logInfo("%s"," Parameters....");
                                for(;ucCount<mTotalParams;ucCount++)
                                {
                                        logInfo(" Name : %s Value : %s",mParams[ucCount].sName,
                                        mParams[ucCount].sValue);
                                }
                        }
                        else
                        {
                                logInfo("%s"," NO Parameters.");
                        }
}



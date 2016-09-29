//////////////////////////////////////////////////////////////////////////////////
// @file    wih_Interface.cpp
// @details Copyright Powerwave Technologies, Inc., 2010
// @brief This file has the methods for class CWebInterface.
//
// Author     : Tara Shankar Maji
// Date       : 13-09-2010
// Description: This file contains methods for web server interface functionalities.
// Change History: 13-09-10 Initial version.
///////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "wih.h"
#include "wih_wsInterface.h"
#include "wih_commandInfo.h"
#include "wih_sessionMgr.h"
#include "fcgi_stdio.h"

CWebInterface * CWebInterface::mInstance = NULL;

////////////////////////////////////////////////////////////////////
// Operation	:  
// Description	: Constructor 
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
CWebInterface::CWebInterface()
{
}

////////////////////////////////////////////////////////////////////
// Operation	: 
// Description	: Destructor  
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
CWebInterface::~CWebInterface()
{
}

////////////////////////////////////////////////////////////////////
// Operation	: acceptRequest
// Description	: This method is used for accepting new requests from
// 		  the web server over FastCGI interface. 
// Input	: 
// Output	: 
///////////////////////////////////////////////////////////////////
void CWebInterface::acceptNewRequest(void)
{
	while(FCGI_Accept() >= 0)
	{
		CCommandInfo *pCmdInfo;
		// Create a new command detail object that will hold
		// the web request based details for further processing
		pCmdInfo = new CCommandInfo;

                if(true NOTEQ pCmdInfo->parseWebRequest())
                {
                        logError("%s","Parsing the web request FAILED !!!");
                }
                else
                {
			// Parsing the web request is successful
			// lets handle the request now
			pCmdInfo->dumpContent();

			CSessionManager::getInstance().handleRequest(pCmdInfo);

                }		

		// Delete the coammand detail object
		delete pCmdInfo;
	}
}

////////////////////////////////////////////////////////////////////
// Operation	: getInstance
// Description	: This method returns the reference to the singleton 
//		  object for web server interface.
// Input	: None
// Output	: CWebInterface object reference
///////////////////////////////////////////////////////////////////
CWebInterface & CWebInterface::getInstance(void)
{
	if(mInstance EQ NULL) {
		mInstance = new CWebInterface;
	}

	return *mInstance;
}

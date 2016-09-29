//////////////////////////////////////////////////////////////////////////////////
// @file    wih_commandInfo.h
// @brief Class definition for Web Request Parameters
// @details Copyright Powerwave Technologies, Inc., 2009
//
// Author         : Tara Shankar Maji
// Date           : 13-09-2010
// Change History : 13-09-10 Initial version created.
//////////////////////////////////////////////////////////////////////////////////

#ifndef __CMD_INFO__
#define __CMD_INFO__

#include "wih.h"

typedef struct
{
        char    sName[MAX_PARAM_NAME_LENGTH];
        char    sValue[MAX_VALUE_LENGTH];
}stParam;

/////////////////// Class Definition /////////////////

class CCommandInfo 
{
	private:
		//Attributes
		ContextID 	mContextId;
		char		mReqPageName[MAX_REQ_PAGE_LENGTH];
		stParam         mParams[MAX_REQ_PARAMS];
		unsigned char   mTotalParams;

		// Methods
		void parseParams(const char *);
		void unescapeValue(char *);

	public:
		CCommandInfo();
	 	~CCommandInfo();

		bool parseWebRequest(void);
		ContextID getContextId(void);
		const char * getCmdName(void);
		bool getParamByName(char *, char *);
		void dumpContent(void);
		UINT32 getTotalParamsRcvd(void);
};


#endif // __CMD_INFO__


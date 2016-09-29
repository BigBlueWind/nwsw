//////////////////////////////////////////////////////////////////////////////////
// @file    wih_webResponse.h
// @brief Class definition for Web Client Response @@ Singleton
// @details Copyright Powerwave Technologies, Inc., 2009
//
// Author         : Tara Shankar Maji
// Date           : 13-09-2010
// Change History : 13-09-10 Initial version created.
//////////////////////////////////////////////////////////////////////////////////

#ifndef __WEB_RESPONSE__
#define __WEB_RESPONSE__

#include "wih.h"
#include "wih_commandInfo.h"
#include "generic_table.h"

extern "C"
{
#include "tlv_enc_dec.h"
}

#define wihGetOprCmdTableInfo(tableId)   &(g_OamTableContext.oprCmdTableInfo[tableId])
#define wihGetOprCfgTableInfo(tableId)   &(g_OamTableContext.oprCfgTableInfo[tableId])
#define wihGetStatusTableInfo(tableId)   &(g_OamTableContext.statusTableInfo[tableId])
#define wihGetPreCfgTableInfo(tableId)   &(g_OamTableContext.preCfgTableInfo[tableId])

typedef enum {
	RES_LOGIN_SUCCESS = 0,
	RES_LOGIN_FAIL,
	RES_BUSY,
	RES_CMD_NOT_SUPPORTED,
	RES_LOGOUT_SUCCESS,
	RES_INACTIVE_LOGOUT,
	RES_TABLE_NOT_FOUND,
	RES_PARAM_NOT_FOUND,
	RES_INCOMPLETE_PARAMS,
	RES_KEY_NOT_FOUND,
	RES_CMD_TIMEOUT,
	RES_FILE_DETAILS,
	RES_MAX
} enRspCode;

/////////////////// Class Definition /////////////////

class CWebResponse 
{
	private:
		//Attributes
		static CWebResponse *mInstance;
		void displayObjValue(TlvInfo *);
		ColName*        getColName(TableId, ColId );
		DataType*       getColDataType(TableId, ColId);
		OamTableType    getTableTypeByTableId(TableId);

	protected:
		CWebResponse();
		virtual ~CWebResponse();

	public:
		void createAndSendResponse(const enRspCode eRsp, const ContextID cid = 0);
		void createAndSendResponse(const enRspCode eRsp, const char *eStr);
		void createAndSendResponse(Msg *msgIn);
		void sendStaticPage(CCommandInfo *);
		static CWebResponse & getInstance(void);

};


#endif // __WEB_RESPONSE__


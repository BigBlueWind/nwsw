//////////////////////////////////////////////////////////////////////////////////
// @file    wih_setCmdSession.h
// @brief Class definition for SET command session class
// @details Copyright Powerwave Technologies, Inc., 2009
//
// Author         : Tara Shankar Maji
// Date           : 24-09-2010
// Change History : 24-09-10 Initial version created.
//////////////////////////////////////////////////////////////////////////////////

#ifndef __SET_CMD_SESSION__
#define __SET_CMD_SESSION__

extern "C"
{
#include "tlv_enc_dec.h"
}
#include "wih_cmdSession.h"
#include "generic_table.h"

/////////////////// Class Definition /////////////////

class SetCmdSession:public CmdSession 
{
	public:
		SetCmdSession(){ mStatus = CMD_NOT_COMPLETED;};
	 	~SetCmdSession();

	private:
		bool getTableId(const char *, TableId *);
		bool fillTlvInfo(SetObjReq *, CCommandInfo *, TagInfo);
		void processOamResponse(Msg *);
		void doExecuteCmd(CCommandInfo *);
		void doTerminateCmd (void);
		OamTableInfo* getTablePtrByTableId(TableId);
                ColName*      getColName(TableId, ColId );
                DataType*     getColDataType(TableId, ColId);
                UINT16*       getColLength(TableId, ColId);
                OamTableType  getTableTypeByTableId(TableId);

};


#endif // __SET_CMD_SESSION__


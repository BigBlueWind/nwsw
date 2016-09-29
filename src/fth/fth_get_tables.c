/**
 * @file    fth_get_tables.c
 * @brief this file contains functions related GET_OBJ_REQ Procedure.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author         Ankur Agrawal
 * @date           01-Oct-2010
 */
#include "fth.h"
/**
 * @param getReq     get object request
 * @param fthContext context of FTH
 * @details This function fills GET_OBJ_SUCC for fileEventHistory table.
 */
static void fthGetAndSendFileEventHistoryTable(
  IO GetObjReq   *getReq,
  IO FthContext  *fthContext
  )
{
  RowId           rowId=0;
  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  TagInfo        *tagInfo = &getReq->tagArray.tagInfo[0]; /* One Get = One Req message */

  initEncBuf(&encBuf);
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  /* Fill CmdRef details */
  copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

  /* if there is not entry in status table then we send only first row of table. */
  if(fthContext->noOfEntriesInFileEventHistory EQ 0)
  {
    getRowFileEventHistoryTable( tagInfo->tableId, 1, &(rspTlv.tlvArray));
  }
  else
  {
    for ( rowId=1; rowId <= fthContext->noOfEntriesInFileEventHistory; rowId++ )
    {
       getRowFileEventHistoryTable( tagInfo->tableId, rowId, &rspTlv.tlvArray );
    }
  }
  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );
  /* Send */
  createAndSendMsg( MSG_FTH_BSM_GET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);
}

/**
 * @param getReq     get object request
 * @details This function fills GET_OBJ_SUCC for fileDownload and fileUpload status table.
 */
static void fthGetAndSendTableData(
  IO GetObjReq   *getReq
  )
{
  GetObjRspSucc   rspTlv;
  EncBuf          encBuf;
  initEncBuf( &encBuf );
  clearBytes( &rspTlv, sizeof(GetObjRspSucc));

  TagInfo       *tagInfo = &getReq->tagArray.tagInfo[0]; /* One Get = One Req message */
  GenericTable  *tableFunc       = getTableFunctions(tagInfo->tableId);

  /* Fill CMD REF details */
  copyBytes( &(getReq->cmdRef), sizeof(CmdRef), &(rspTlv.cmdRef));

  /* Fill Structure */
  if( NULL NOTEQ tableFunc->getStruct2TlvFunc )
  {
    tableFunc->getStruct2TlvFunc( tagInfo, &rspTlv.tlvArray );
  }
  else
  {
    logInfo("No getStruct2TlvFunc Function defined for table %d", tagInfo->tableId );
  }
  /* Encode */
  encGetObjRspSuccMsg( &rspTlv, &encBuf );
  /* Send */
  createAndSendMsg( MSG_FTH_BSM_GET_OBJ_SUCC, (UINT8*)&encBuf.value, encBuf.len);
}
/**
 * @param msg        get object message
 * @param fthContext context of FTH
 * @details This function process GET_OBJ_REQ message.
 */
void fthGetObjReq(
  IN Msg         *msg,
  IO FthContext  *fthContext
  )
{
  MsgBsmFthGetObjReq       getObj;
  TableId                  tableId;

  clearBytes( &getObj, sizeof(MsgBsmFthGetObjReq));
  decGetObjReqMsg( (UINT8*)msg->data, msg->len, &getObj );

  tableId = getObj.tagArray.tagInfo[0].tableId;

  switch( tableId )
  {
    case FILEDOWNLOADSTATUS:
    case FILEUPLOADSTATUS:
      fthGetAndSendTableData( &getObj );
      break;
    case FILEEVENTHISTORY:
      fthGetAndSendFileEventHistoryTable( &getObj, fthContext );
      break;
  }
}

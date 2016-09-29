/**
 * @file      cih_cmd_rsp_proc.c
 * @details   Copyright Powerwave Technologies, Inc., 2010
 * @brief     This file contains processing of messages which comes from User and also from BSM.
 *
 * @author    Madhusudana Rao Kata
 * @date      10-01-2011
 */

#include "cih.h"
#include "cih_tecla.h"

/**********************************************************************************/
/*                           RESPONSE MESSAGES FROM BSM                           */
/**********************************************************************************/
/**
 * @param   cmdRef  Contains Source and Command Number of the Commands entered at CIH
 * @details         Display Command Information for Command Success case
 */
void cihDisplayCmdRefSucc(
    IN CmdRef   *cmdRef,
    IN TimeInfoSeconds  *time
    )
{
  DisplayBuffer       displayBuffer;
  TimeInfoStandard    *currtime = NULL;
  currtime = localtime( (time_t *) &(time->tv_sec) );
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  /*Time stamp format in Hr:Min:Sec:milliSec*/
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "[%d:%02d:%02d:%ld RspToCmdRefNum=%d, SUCCESS]\n",
      currtime->tm_hour, currtime->tm_min, currtime->tm_sec, time->tv_usec/1000, cmdRef->cmdRefNum);
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param   cmdRef   Contains Source and Command Number of the Commands entered at CIH
 * @details          Display Command Information for Command Failure case
 */
void cihDisplayCmdRefFail(
    IO CmdRef   *cmdRef,
    IN TimeInfoSeconds  *time
    )
{
  DisplayBuffer       displayBuffer;
  TimeInfoStandard    *currtime = NULL;
  currtime = localtime( (time_t *) &(time->tv_sec) );
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  /*Time stamp format in Hr:Min:Sec:milliSec*/
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "[%d:%02d:%02d:%ld RspToCmdRefNum=%d, FAILURE]\n",
      currtime->tm_hour, currtime->tm_min, currtime->tm_sec, time->tv_usec/1000, cmdRef->cmdRefNum);
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @details          Display Command Information for Command Failure case
 */
void cihDisplayCmdRefDelayedRsp(
    IN TimeInfoSeconds  *time
    )
{
  DisplayBuffer       displayBuffer;
  TimeInfoStandard    *currtime = NULL;
  currtime = localtime( (time_t *) &(time->tv_sec) );
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  /*Time stamp format in Hr:Min:Sec:milliSec*/
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "\n[%d:%02d:%02d:%ld Delayed RSP]\n",
      currtime->tm_hour, currtime->tm_min, currtime->tm_sec, time->tv_usec/1000);
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param    cmdRef  Contains Source and Command Number of the Commands entered at CIH
 * @details          Display Command Information for Command Failure case
 */
void cihDisplayCmdRefDeclined(
    IO CmdRef   *cmdRef
    )
{
  DisplayBuffer       displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),"[RspToCmdRefNum=%d, DECLINED]\n",cmdRef->cmdRefNum);
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @details          Display Table Name, Column Name and Data
 */
void cihPrintHeader()
{
  DisplayBuffer       displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),"%-30s  %-10s   %-10s   %-25s   %-6s \n", "TableName", "RowNumber", "ColNumber", "ColName", "Value");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @details          Display Table Name, Column Name and Data
 */
void cihPrintHeaderForFailCmd()
{
  DisplayBuffer       displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),"%-30s  %-10s   %-10s   %-25s \n", "TableName", "RowNumber", "ColNumber", "ColName");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @details          Display a string "Values Entered By the user in the command:"for user in case of failure
 */
void cihPrintInfoFailCmd()
{
  DisplayBuffer       displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), " \nValues Entered By the user:\n");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param   tag      Contains Table Id, Row Id and column Id
 * @details          Find the action needs to be taken
 * */
EnumStrInfo *cihGetEnumStrFunction(
    IO TagInfo   *tag
    )
{
  Index index = 0 ;

  for(index=0; (c_EnumStrInfoTab[index].tableId NOTEQ 0); index++)
  {
    if(c_EnumStrInfoTab[index].tableId EQ tag->tableId)
    {
      if(c_EnumStrInfoTab[index].colId EQ tag->colId )
      {
        return (EnumStrInfo*)&(c_EnumStrInfoTab[index]);
      }
    }
  }
  return NULL; /* In case of Enum is not found */
}



/**
 * @param objTlvInfo Contains Data received from BSM in TLV format for each Object
 * @details          Display ObjInfo i.e Table Name, Column Name and Data received from BSM
 */
void displayObjInfo(
    IO TlvInfo  *objTlvInfo
    )
{
  TableName      *tableName = NULL;
  ColName        *colName = NULL;
  DataType       *dataType = NULL;
  DisplayBuffer   displayBuffer;
  UINT32          value32Bit = 0;
  UINT16          value16Bit = 0;
  UINT8           value8Bit = 0;
  SINT32          signedValue32Bit = 0;
  SINT16          signedValue16Bit = 0;
  SINT8           signedValue8Bit = 0;

  EnumStrInfo    *enumToStrInfo = NULL;
  const UINT8          *colNameForEnum = NULL;
  OamTableInfo   *oamTableInfo =  NULL;

  clearBytes(displayBuffer,sizeof(DisplayBuffer));

  oamTableInfo =  getTablePtrByTableId(objTlvInfo->tag.tableId);
  if(oamTableInfo EQ NULL)
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%s   \n","Not Able to Find Table Info from Database");
    fputs(displayBuffer, stdout);
    cihLogRsp(displayBuffer);
    return;
  }

  tableName = &oamTableInfo->tableName;
  colName   = &oamTableInfo->colInfo[objTlvInfo->tag.colId].colName;
  dataType  = &oamTableInfo->colInfo[objTlvInfo->tag.colId].dataType;

  if(*dataType EQ BS_TYPE_STRING )
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_UINT32 )
  {
    value32Bit =  getUint32FromArray(objTlvInfo->data);
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
        "%-30s  %-10d    %-10d   %-25s   %-6u \n",\
        (char*)tableName, objTlvInfo->tag.rowId,   objTlvInfo->tag.colId, (char*)colName , value32Bit);
  }
  else if(*dataType EQ BS_TYPE_SINT32)
  {
    signedValue32Bit =  getUint32FromArray(objTlvInfo->data);
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
        "%-30s  %-10d    %-10d   %-25s   %-6d \n",\
        (char*)tableName, objTlvInfo->tag.rowId,   objTlvInfo->tag.colId, (char*)colName , signedValue32Bit);
  }
  else if(*dataType EQ BS_TYPE_UINT16 )
  {
    value16Bit =  getUint16FromArray(objTlvInfo->data);
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
        "%-30s  %-10d    %-10d   %-25s   %-6u \n",\
        (char*)tableName, objTlvInfo->tag.rowId,   objTlvInfo->tag.colId, (char*)colName , value16Bit);
  }
  else if(*dataType EQ BS_TYPE_SINT16)
  {
    signedValue16Bit =  getUint16FromArray(objTlvInfo->data);
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
        "%-30s  %-10d    %-10d   %-25s   %-6d \n",\
        (char*)tableName, objTlvInfo->tag.rowId,   objTlvInfo->tag.colId, (char*)colName , signedValue16Bit);
  }
  else if(*dataType EQ BS_TYPE_UINT8 )
  {
    value8Bit =  getUint8FromArray(objTlvInfo->data);
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
        "%-30s  %-10d    %-10d   %-25s   %-6u \n",\
        (char*)tableName, objTlvInfo->tag.rowId,   objTlvInfo->tag.colId, (char*)colName , value8Bit);
  }
  else if(*dataType EQ BS_TYPE_SINT8)
  {
    signedValue8Bit =  getUint8FromArray(objTlvInfo->data);
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
        "%-30s  %-10d    %-10d   %-25s   %-6d \n",\
        (char*)tableName, objTlvInfo->tag.rowId,   objTlvInfo->tag.colId, (char*)colName , signedValue8Bit);
  }
  else if(*dataType EQ BS_TYPE_ENUM )
  {
    value32Bit =  getUint32FromArray(objTlvInfo->data);

    enumToStrInfo = cihGetEnumStrFunction(&objTlvInfo->tag);
    if(enumToStrInfo EQ NULL)
    {
      logError("%s","EnumToString Function not found; Please generate enumstr_oamtables.c and .h functions and replace in code");
    }
    else
    {
      colNameForEnum = enumToStrInfo->enumStrHdlr(value32Bit);
    }
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
        "%-30s  %-10d    %-10d   %-25s   %d [%-6s] \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , value32Bit, colNameForEnum);
  }
  else if(*dataType EQ BS_TYPE_BOOL )
  {
    value8Bit =  getUint8FromArray(objTlvInfo->data);
    if(value8Bit EQ 0)
    {
      snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
          "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
          (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , "FALSE");
    }
    else
    {
      snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
          "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
          (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , "TRUE");
    }
  }
  else if(*dataType EQ BS_TYPE_IPV4ADDRESS )
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_MACADDRESS )
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_FILENAME )
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_LOCALTIME )
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_OFFSETTIME )
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_SWVERSION )
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_MCC )
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_MNC )
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_LATITUDE )
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_LONGITUDE )
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_ENODEBNAME )
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_PROFILENAME )
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%-30s  %-10d    %-10d   %-25s   %-6s \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_MODID )
  {
    value32Bit =  getUint32FromArray(objTlvInfo->data);

    colNameForEnum = enumModIdToStr(value32Bit);
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
        "%-30s  %-10d    %-10d   %-25s   %d [%-6s] \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , value32Bit, colNameForEnum);
  }
  else if(*dataType EQ BS_TYPE_MSGID )
  {
    value32Bit =  getUint32FromArray(objTlvInfo->data);

    colNameForEnum = getMsgName(value32Bit);
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
        "%-30s  %-10d    %-10d   %-25s   %d [%-6s] \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , value32Bit, colNameForEnum);
  }
  else if(*dataType EQ BS_TYPE_COUNTERID )
  {
    value32Bit =  getUint32FromArray(objTlvInfo->data);

    colNameForEnum = getCounterIdStr(value32Bit);
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
        "%-30s  %-10d    %-10d   %-25s   %d [%-6s] \n",\
        (char*)tableName, objTlvInfo->tag.rowId,  objTlvInfo->tag.colId, (char*)colName , value32Bit, colNameForEnum);
  }
  else
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
        "%-30s  %-10d    %-10d   %-25s   %-6s \n",
        (char*)tableName, objTlvInfo->tag.rowId,   objTlvInfo->tag.colId, (char*)colName , objTlvInfo->data);
  }
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param   tlvTag   Contains Data received from BSM in TLV format for each Object
 * @details          Display Tag Info i.e Table Name, Column Name and row Id received from BSM
 */
void displayObjTagInfo(
    IO TagInfo      *tlvTag
    )
{
  TableName      *tableName = NULL;
  ColName        *colName = NULL;
  DisplayBuffer   displayBuffer;

  OamTableInfo   *oamTableInfo =  NULL;

  clearBytes(displayBuffer,sizeof(DisplayBuffer));

  oamTableInfo =  getTablePtrByTableId(tlvTag->tableId);
  if(oamTableInfo EQ NULL)
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%s   \n","Not Able to Find Table Info from Database");
    fputs(displayBuffer, stdout);
    cihLogRsp(displayBuffer);
    return;
  }

  tableName = &oamTableInfo->tableName;
  colName   = &oamTableInfo->colInfo[tlvTag->colId].colName;

  snprintf((UINT8*)&displayBuffer, sizeof(TableName)+sizeof(UINT32)+sizeof(ColName), "%-30s  %-10d %-10s",(char*)tableName, tlvTag->rowId, (char*)colName );
  //  snprintf((UINT8*)&displayBuffer, , " ", );
  //  snprintf((UINT8*)&displayBuffer, , "  ",  );
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param   tlvArray Contains Data received from BSM in TLV format
 * @details          This function calls displayObjInfo for all tlvs received from BSM
 */
void cihDisplayObjRsp(
    IO TlvArray    *tlvArray
    )
{
  Index  index;

  for(index = 0; index < (tlvArray->numTlvs) ; index++)
  {
    displayObjInfo(&tlvArray->tlvInfo[index]);
  }
}
/**
 * @param   errRsp   Contains Error Id and Error TLV Index
 * @details          This function call displays Error Information received from BSM
 */
void cihDisplayError(
    IO ErrRsp      *errRsp
    )
{
  Buffer     displayBuffer;
  clearBytes(displayBuffer,sizeof(Buffer));
  snprintf((UINT8*)&displayBuffer, sizeof(Buffer) ,"ErrorCode=%s \n", errorCodeStr(errRsp->errRspCode));
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);

  clearBytes(displayBuffer,sizeof(Buffer));
  snprintf((UINT8*)&displayBuffer, sizeof(Buffer) ,"ErrorInfo=%s \n", errRsp->errorInfo);
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param   msgIn    is a Message coming from BSM
 * @details          This function is used to Process Set Object Success Response and print data Received
 */
void procSetObjSucc(
    IN Msg      *msgIn,
    IN TimeInfoSeconds  *time
    )
{
  SetObjRspSucc   rspSucc;
  TBOOL isCmdReceivedIntime = FALSE;
  clearBytes( &rspSucc, sizeof(SetObjRspSucc));
  /* Decode TLV's */
  decSetObjRspSuccMsg( (UINT8*)&(msgIn->data[0]), msgIn->len, &rspSucc );
  isCmdReceivedIntime = removeEntryFromCmdStatus(&rspSucc.cmdRef);
  if(isCmdReceivedIntime EQ FALSE)
  {
    cihDisplayCmdRefDelayedRsp( time );
  }
  cihDisplayCmdRefSucc( &(rspSucc.cmdRef), time);
  cihPrintHeader();
  cihDisplayObjRsp(&(rspSucc.tlvArray));
  return ;
}

/**
 * @param   msgIn    is a Message coming from BSM
 * @details          This function is used to Process Set Message trace received from BSM
 */
void procSetMsgTrace(
    IN Msg      *msgIn
    )
{
  MsgBsmCihSetMsgTrace  *msgTrace    = (MsgBsmCihSetMsgTrace*)msgIn->data;
  MsgDesc   *msgDesc        = getMsgDesc(msgTrace->msgId);

  /* Set the values in the table */
  msgDesc->msgTraceLevel     = msgTrace->msgTraceLevel;
  msgDesc->msgTraceDirection = msgTrace->msgTraceDirection;
  return ;
}

/**
 * @param   tableId  TableId received from BSM
 * @details          This function is used to Display Table Empty Message to User
 */
void cihPrintTableEmpty(
    IO TableId  tableId
    )
{
  TableName      *tableName = NULL;
  Buffer          displayBuffer;
  OamTableInfo   *oamTableInfo =  NULL;
  clearBytes(displayBuffer,sizeof(Buffer));

  oamTableInfo =  getTablePtrByTableId(tableId);
  if(oamTableInfo NOTEQ NULL)
  {
    tableName = &oamTableInfo->tableName;
    snprintf((UINT8*)&displayBuffer, sizeof(Buffer) ,"%s -- Table is Empty\n",(UINT8*)tableName );
  }
  snprintf((UINT8*)&displayBuffer, sizeof(Buffer) ,"%s -- Table is Empty\n","UNKNOWN TABLE" );
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param   msgIn    is a Message coming from BSM
 * @details          This function is used to Process Get Object Success Response and print data Received
 */
void procGetObjSucc(
    IN Msg      *msgIn,
    IN TimeInfoSeconds  *time
    )
{
  GetObjRspSucc   rspSucc;
  TBOOL isCmdReceivedIntime = FALSE;
  clearBytes( &rspSucc, sizeof(GetObjRspSucc));
  /* Decode TLV's for Get Message */
  decGetObjRspSuccMsg((UINT8*)&(msgIn->data[0]), msgIn->len, &rspSucc );
  isCmdReceivedIntime = removeEntryFromCmdStatus(&rspSucc.cmdRef);
  if(isCmdReceivedIntime EQ FALSE)
  {
    cihDisplayCmdRefDelayedRsp( time );
  }
  cihDisplayCmdRefSucc( &(rspSucc.cmdRef), time);
  if(rspSucc.tlvArray.numTlvs EQ 0)
  {
    cihPrintTableEmpty(rspSucc.tlvArray.tlvInfo[0].tag.tableId );
    return ;
  }
  cihPrintHeader();
  cihDisplayObjRsp(&(rspSucc.tlvArray) );
  return ;
}

/**
 * @param   msgIn    is a Message coming from BSM
 * @details          This function is used to Process Set Object Failure Response and print data Received
 */
void procSetObjFail(
    IN Msg      *msgIn,
    IN TimeInfoSeconds  *time
    )
{
  SetObjRspFail   rspFail;
  TBOOL isCmdReceivedIntime = FALSE;
  clearBytes( &rspFail, sizeof(SetObjRspFail));

  decSetObjRspFailMsg( (UINT8*)&(msgIn->data[0]), msgIn->len, &rspFail );
  isCmdReceivedIntime = removeEntryFromCmdStatus(&rspFail.cmdRef);
  if(isCmdReceivedIntime EQ FALSE)
  {
    cihDisplayCmdRefDelayedRsp( time );
  }
  cihDisplayCmdRefFail(&rspFail.cmdRef, time );
  cihDisplayError(&rspFail.setErrRsp);
  //cihPrintHeaderForFailCmd();
  /* Display Where Error error occurs */
  //displayObjTagInfo(&rspFail.setErrRsp.failedTagInfo);
  /* Display Data Entered By the user */
  cihPrintInfoFailCmd();
  cihPrintHeader();
  cihDisplayObjRsp(&(rspFail.tlvArray) );
  return ;
}

/**
 * @param   msgIn    is a Message coming from BSM
 * @details          This function is used to Process Get Object Failure Response and print data Received
 */
void procGetObjFail(
    IN Msg      *msgIn,
    IN TimeInfoSeconds  *time
    )
{
  GetObjRspFail   rspFail;
  TBOOL isCmdReceivedIntime = FALSE;
  clearBytes( &rspFail, sizeof(GetObjRspFail));

  decGetObjRspFailMsg( (UINT8*)&(msgIn->data[0]), msgIn->len, &rspFail );

  isCmdReceivedIntime = removeEntryFromCmdStatus(&rspFail.cmdRef);
  if(isCmdReceivedIntime EQ FALSE)
  {
    cihDisplayCmdRefDelayedRsp( time );
  }
  cihDisplayCmdRefFail(&rspFail.cmdRef, time );
  cihDisplayError(&rspFail.getErrRsp);
  //cihPrintHeaderForFailCmd();
  /*Display Where Error error occurs -- Only Tag Info */
  //displayObjTagInfo(&rspFail.getErrRsp.failedTagInfo);
  return ;
}

/**
 * @param   msgIn    is a Message coming from BSM
 * @details          This function is used to Process Messages coming from BSM
 *                   i.e BSM-CIH Message Handler
 */
int bsmCihMsgHandler(
    IN Msg      *msgIn
    )
{
  UINT32     msgId = 0;

  /* Recording the time when the RSP from BSM is Received */
  TimeInfoSeconds  time;
  gettimeofday( &time, NULL );

  msgId = msgIn->msgId;

  g_respRecd++;
  switch( msgId )
  {
    case MSG_BSM_CIH1_SET_MSG_TRACE:
    case MSG_BSM_CIH2_SET_MSG_TRACE:
    case MSG_BSM_CIH3_SET_MSG_TRACE:
      {
        procSetMsgTrace( msgIn );
        break;
      }
    case MSG_BSM_CIH1_SET_OBJ_SUCC:
    case MSG_BSM_CIH2_SET_OBJ_SUCC:
    case MSG_BSM_CIH3_SET_OBJ_SUCC:
      {
        procSetObjSucc( msgIn, &time );
        break;
      }
    case MSG_BSM_CIH1_GET_OBJ_SUCC:
    case MSG_BSM_CIH2_GET_OBJ_SUCC:
    case MSG_BSM_CIH3_GET_OBJ_SUCC:
      {
        procGetObjSucc( msgIn, &time );
        break;
      }
    case MSG_BSM_CIH1_SET_OBJ_FAIL:
    case MSG_BSM_CIH2_SET_OBJ_FAIL:
    case MSG_BSM_CIH3_SET_OBJ_FAIL:
      {
        procSetObjFail( msgIn, &time );
        break;
      }
    case MSG_BSM_CIH1_GET_OBJ_FAIL:
    case MSG_BSM_CIH2_GET_OBJ_FAIL:
    case MSG_BSM_CIH3_GET_OBJ_FAIL:
      {
        procGetObjFail( msgIn, &time );
        break;
      }
      /* Timer Handler */
    case MSG_TIMEKEEPER_CIH1_TIMEOUT_IND:
    case MSG_TIMEKEEPER_CIH2_TIMEOUT_IND:
    case MSG_TIMEKEEPER_CIH3_TIMEOUT_IND:
      {
        /* This message is received from CIH only */
        g_respRecd--;
        cihProcTimerExpiry( msgIn );
        break;
      }
    default:
      {
        BS_ERROR_UNKNOWN_MSG(msgId );
        return 0;
      }
  }
  return 0;
}

/**
 * @details          This is the main function Used to process BSM Responses.
 */
TBOOL cihProcBsmRsp()
{
  UINT16   recvMsgRetVal = 0;
  Msg      *msgIn=NULL;
  InstanceId instanceId = getCihInstanceId(); 
  ModuleId moduleId = MODID_NONE;

  if(instanceId EQ 1)
  {
    moduleId = MODID_CIH1;
  }
  else if(instanceId EQ 2)
  {
    moduleId = MODID_CIH2;
  }
  else if(instanceId EQ 3)
  {
    moduleId = MODID_CIH3;
  }


  while(1)
  {
    msgIn = recvMsg(moduleId, &recvMsgRetVal);

    if( msgIn NOTEQ NULL)
    {
      bsmCihMsgHandler(msgIn);
      freeMsgIn(msgIn);
      return TRUE;
    }
    else
    {
      break;
    }
  }
  printAndEmptyErrorChain(&(getModuleContext()->errChain));
  return FALSE;
}

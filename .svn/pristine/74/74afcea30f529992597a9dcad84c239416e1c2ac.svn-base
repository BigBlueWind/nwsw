/**
 * @file    print_tlv.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file is the starting point for CIH module.
 *
 * @author     : Madhusudana Rao Kata
 * Date       : 11-10-2010
 * Description: 1. Basic version
 *
 */

#include "print_tlv.h"

/* OAM Table Context */
OamTableContextForTlvPrint     g_PrintTlvOamTableContext;

/* Used for printing Enums into string */
extern EnumStrInfo c_EnumStrInfoTab[];

/**
 * @param globalConfigFilePath configuration file path
 * @details This function parse OamConfig.tpl for libPrint
 * */
void printTlvInit(
  IO UINT8 *globalConfigFilePath
  )
{
  /* Parse OAM Configuration Template */
  OamTableContextForTlvPrint *oamCtx = getOamTableTlvInfo();
  parseOamCfgForLibPrint(globalConfigFilePath,oamCtx);
}

/**
 * @param ref Command Ref
 * @details This function prints command ref.
 * */
void printTlvCmdRef(
  IN CmdRef *ref
  )
{
  logMsgParams(" origin:%d\n cmdRefNum:%d\n instanceId:%d\n",ref->origin, ref->cmdRefNum,ref->instanceId );
}

/**
 * @param errRsp  Contains Error Id and Error TLV Index
 * @details This function calls display Error Information received from BSM
 */
void printErrorInfo(
  IO ErrRsp      *errRsp
  )
{
  logMsgParams(" ErrorCode=%s\n ErrorInfo=%s \n", errorCodeStr(errRsp->errRspCode), errRsp->errorInfo);
}

/**
 * @param info contains Table Id, Row Id and column Id
 * @details Prints the TAG info
 * */
UINT32 printTlvTagInfo(
  IN TagInfo *info
  )
{
  TableName      *tableName = NULL;
  OamTableInfoForTlvPrint *tableInfo = NULL ;

  logMsgParams(" TableId=%d ", info->tableId);

  tableInfo = printTlvGetTablePtrByTableId(info->tableId);
  if(tableInfo NOTEQ NULL)
  {
    tableName = &tableInfo->tableName;
    logMsgParams("  TableName=%s ", tableName);
  }
  else
  {
	logMsgParams("  TableName=%s ", "UNKNOWN TABLE");
  }
  if(0 NOTEQ info->rowId )
  {
	logMsgParams("  RowId=%d", info->rowId);
  }
  if(0 NOTEQ info->colId )
  {
    logMsgParams("  ColId=%d", info->colId);
  }
  logMsgParams("\n", NULL);
  return info->tableId;
}

/**
 * @param info contains Table Id, Row Id and column Id
 * @details Prints the TAG info of RspFail
 * */
UINT32 printFailTlvTagInfo(
  IN TagInfo *info
  )
{
  TableName      *tableName = NULL;
  OamTableInfoForTlvPrint *tableInfo = NULL ;

  logMsgParams(" TableId=%d ", info->tableId);

  tableInfo = printTlvGetTablePtrByTableId(info->tableId);
  if(tableInfo NOTEQ NULL)
  {
    tableName = &tableInfo->tableName;
    logMsgParams("  TableName=%s   RowId=%d  ColId=%d\n", tableName, info->rowId, info->colId);
  }
  else
  {
    logMsgParams("  TableName=UNKNOWN TABLE  RowId=%d  ColId=%d\n", info->rowId, info->colId );
  }
  return info->tableId;
}

/**
 * @param tableId contains Table Id
 * @details This functions prints table Id.
 * */
inline OamTableType printTlvGetTableTypeByTableId(
  IN TableId       tableId
  )
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


/**
 * @param tableId contains Table Id
 * @details This functions prints TLV based o table ID.
 * */
OamTableInfoForTlvPrint* printTlvGetTablePtrByTableId(
  IN TableId      tableId
  )
{
  OamTableInfoForTlvPrint *tableInfo = NULL ;
  OamTableType  tableType;

  tableType= printTlvGetTableTypeByTableId(tableId);
  switch(tableType)
  {
    case TABLE_OPERCONFIG:
    tableInfo = tlvPrintGetOprCfgTableInfo(tableId-MIN_OPR_CFG_TABLEID);
	break;

    case TABLE_OPERCMD:
    tableInfo = tlvPrintGetOprCmdTableInfo(tableId-MIN_OPR_CMD_TABLEID);
    break;

    case TABLE_STATUS:
	tableInfo = tlvPrintGetStatusTableInfo(tableId-MIN_STATUS_TABLEID);
	break;

    case TABLE_PRECONFIG:
    tableInfo = tlvPrintGetPreCfgTableInfo(tableId-MIN_PRE_CFG_TABLEID);
    break;

    case TABLE_UNKNOWNTYPE:
    default:
    tableInfo = NULL ;
  }
  return tableInfo;
}

/**
 * @param tag contains Table Id, Row Id and column Id
 * @details Find the action needs to be taken
 * */
EnumStrInfo *getEnumStrFunction(
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
 * @param objTlvInfo      it contains TlvInfo of a particular element
 * @param columnName      name of Column
 * @param data            data of particular element whose detail is given in objTlvInfo
 * @details This function return the data and colName of particular element in TLV whose info is
 *          is given in objTlvInfo
 */
void getColNameAndData(
  IO TlvInfo  *objTlvInfo,
  IO UINT8    *columnName,
  IO UINT8    *data
  )
{
  ColName        *colName = NULL;
  DataType       *dataType = NULL;
  UINT32          value32Bit = 0;
  UINT16          value16Bit = 0;
  UINT8           value8Bit = 0;
  SINT32          signedValue32Bit = 0;
  SINT16          signedValue16Bit = 0;
  SINT8           signedValue8Bit = 0;

  EnumStrInfo    *enumToStrInfo = NULL;
  UINT8          *colNameForEnum = NULL;
  OamTableInfoForTlvPrint *tableInfo = NULL ;
  clearBytes(columnName, sizeof(ColName));
  clearBytes(data, MAX_TLV_VALUE_IN_BYTES);

  tableInfo = printTlvGetTablePtrByTableId(objTlvInfo->tag.tableId);
  if(tableInfo EQ NULL)
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s","UNKNOWN DATA");
    snprintf(columnName, sizeof(ColName), "%s","UNKNOWN COLUMN");
    return;
  }

  colName   = &tableInfo->colInfo[objTlvInfo->tag.colId].colName;
  dataType  = &tableInfo->colInfo[objTlvInfo->tag.colId].dataType;
  snprintf(columnName, sizeof(ColName), "%s",(char*)colName);
  if(*dataType EQ BS_TYPE_STRING )
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s",objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_UINT32 )
  {
    value32Bit =  getUint32FromArray(objTlvInfo->data);
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%u",value32Bit);
  }
  else if(*dataType EQ BS_TYPE_SINT32)
  {
    signedValue32Bit =  getUint32FromArray(objTlvInfo->data);
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%d",signedValue32Bit);
  }
  else if(*dataType EQ BS_TYPE_UINT16 )
  {
    value16Bit =  getUint16FromArray(objTlvInfo->data);
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%hu",value16Bit);
  }
  else if(*dataType EQ BS_TYPE_SINT16)
  {
    signedValue16Bit =  getUint16FromArray(objTlvInfo->data);
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%hd",signedValue16Bit);
  }
  else if(*dataType EQ BS_TYPE_UINT8 )
  {
    value8Bit =  getUint8FromArray(objTlvInfo->data);
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%u",value8Bit);
  }
  else if(*dataType EQ BS_TYPE_SINT8)
  {
    signedValue8Bit =  getUint8FromArray(objTlvInfo->data);
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%d",signedValue8Bit);
  }
  else if(*dataType EQ BS_TYPE_ENUM )
  {
    value32Bit =  getUint32FromArray(objTlvInfo->data);

    enumToStrInfo = getEnumStrFunction(&objTlvInfo->tag);
    if(enumToStrInfo EQ NULL)
    {
      snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s","EnumToString Function not found; Please generate enumstr_oamtables.c and .h functions and replace in code");
      exit(EXIT_FAILURE);
      //return;
    }
    else
    {
      colNameForEnum = enumToStrInfo->enumStrHdlr(value32Bit);
    }
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%d [%s]",value32Bit, colNameForEnum);
  }
  else if(*dataType EQ BS_TYPE_BOOL )
  {
    value8Bit =  getUint8FromArray(objTlvInfo->data);
    if(value8Bit EQ 0)
    {
      snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s","FALSE");
    }
    else
    {
      snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s","TRUE");
    }
  }
  else if(*dataType EQ BS_TYPE_MODID )
  {
    value32Bit =  getUint32FromArray(objTlvInfo->data);

    colNameForEnum = enumModIdToStr(value32Bit);
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%d [%s]",value32Bit, colNameForEnum);
  }
  else if(*dataType EQ BS_TYPE_IPV4ADDRESS )
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s",objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_MACADDRESS )
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s",objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_FILENAME )
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s",objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_SWVERSION )
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s",objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_LOCALTIME )
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s",objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_OFFSETTIME )
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s",objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_LATITUDE )
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s",objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_LONGITUDE )
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s",objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_MCC )
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s",objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_MNC )
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s",objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_ENODEBNAME )
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s",objTlvInfo->data);
  }
  else if(*dataType EQ BS_TYPE_PROFILENAME )
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s",objTlvInfo->data);
  }
  else
  {
    snprintf(data, MAX_TLV_VALUE_IN_BYTES, "%s",objTlvInfo->data);
  }
}


/**
 * @param objTlvInfo  Contains Data received from BSM in TLV format for each Object
 * @details display ObjInfo i.e Table Name, Column Name and Data received from BSM
 */
void displayTlvInfo(
  IO TlvInfo  *objTlvInfo
  )
{
  ColName        *colName = NULL;
  DataType       *dataType = NULL;
  UINT32          value32Bit = 0;
  UINT16          value16Bit = 0;
  UINT8           value8Bit = 0;
  SINT32          signedValue32Bit = 0;
  SINT16          signedValue16Bit = 0;
  SINT8           signedValue8Bit = 0;
  SINT32		  tempVal;

  EnumStrInfo    *enumToStrInfo = NULL;
  UINT8          *colNameForEnum = NULL;
  OamTableInfoForTlvPrint *tableInfo = NULL ;

  tableInfo = printTlvGetTablePtrByTableId(objTlvInfo->tag.tableId);
  if(tableInfo EQ NULL)
  {
	logMsgParams("  TableName=%s ", "UNKNOWN TABLE");
	return;
  }

  colName   = &tableInfo->colInfo[objTlvInfo->tag.colId].colName;
  dataType  = &tableInfo->colInfo[objTlvInfo->tag.colId].dataType;

  if(*dataType EQ BS_TYPE_STRING )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_UINT32 )
  {
    value32Bit =  getUint32FromArray(objTlvInfo->data);
    logMsgParams(" colId:%d colName:%s Data:%u\n",objTlvInfo->tag.colId, (char*)colName, value32Bit );
  }
  else if(*dataType EQ BS_TYPE_SINT32)
  {
    tempVal =  getUint32FromArray(objTlvInfo->data);
    signedValue32Bit = (SINT32)tempVal;
    logMsgParams(" colId:%d colName:%s Data:%d\n",objTlvInfo->tag.colId, (char*)colName, signedValue32Bit );
  }
  else if(*dataType EQ BS_TYPE_UINT16 )
  {
    value16Bit =  getUint16FromArray(objTlvInfo->data);
    logMsgParams(" colId:%d colName:%s Data:%hu\n",objTlvInfo->tag.colId, (char*)colName, value16Bit );
  }
  else if(*dataType EQ BS_TYPE_SINT16)
  {
	tempVal = getUint16FromArray( objTlvInfo->data );
    signedValue16Bit = (SINT16)tempVal;
    logMsgParams(" colId:%d colName:%s Data:%hd\n",objTlvInfo->tag.colId, (char*)colName, signedValue16Bit );
  }
  else if(*dataType EQ BS_TYPE_UINT8 )
  {
    value8Bit =  getUint8FromArray(objTlvInfo->data);
    logMsgParams(" colId:%d colName:%s Data:%u\n",objTlvInfo->tag.colId, (char*)colName, value8Bit );
  }
  else if(*dataType EQ BS_TYPE_SINT8)
  {
    tempVal = getUint8FromArray( objTlvInfo->data );
    signedValue8Bit = (SINT8)tempVal;
    logMsgParams(" colId:%d colName:%s Data:%d\n",objTlvInfo->tag.colId, (char*)colName, signedValue8Bit );
  }
  else if(*dataType EQ BS_TYPE_ENUM )
  {
    value32Bit =  getUint32FromArray(objTlvInfo->data);

    enumToStrInfo = getEnumStrFunction(&objTlvInfo->tag);
    if(enumToStrInfo EQ NULL)
    {
      logError("%s", "EnumToString Function not found; Please generate enumstr_oamtables.c and .h functions and replace in code");
      exit(EXIT_FAILURE);
    }
    else
    {
      colNameForEnum = enumToStrInfo->enumStrHdlr(value32Bit);
    }
    logMsgParams(" colId:%d colName:%s Data:%d [%s]\n",objTlvInfo->tag.colId, (char*)colName, value32Bit,colNameForEnum );
  }
  else if(*dataType EQ BS_TYPE_IPV4ADDRESS )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_MACADDRESS )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_FILENAME )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_LOCALTIME )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_OFFSETTIME )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_SWVERSION )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_MCC )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_MNC )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_LATITUDE )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_LONGITUDE )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_ENODEBNAME )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_PROFILENAME )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_BOOL )
  {
    value8Bit =  getUint8FromArray(objTlvInfo->data);
    if(value8Bit EQ 0)
    {
      logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, "FALSE" );
    }
    else
    {
      logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tag.colId, (char*)colName, "TRUE" );
    }
  }
  else if(*dataType EQ BS_TYPE_MODID )
  {
    value32Bit =  getUint32FromArray(objTlvInfo->data);

    colNameForEnum = enumModIdToStr(value32Bit);
    logMsgParams(" colId:%d  colName:%s  Data:%d [%s]\n",objTlvInfo->tag.colId, (char*)colName, value32Bit, colNameForEnum );
  }
  else if(*dataType EQ BS_TYPE_MSGID )
  {
    value32Bit     = getUint32FromArray(objTlvInfo->data);
    const UINT8 *msgName = getMsgName(value32Bit);
    logMsgParams(" colId:%d  colName:%s  Data:%d [%s]\n",objTlvInfo->tag.colId, (char*)colName, value32Bit, msgName );
  }
  else if(*dataType EQ BS_TYPE_COUNTERID )
  {
    value32Bit     = getUint32FromArray(objTlvInfo->data);
    const UINT8 *counterIdName = getCounterIdStr(value32Bit);
    logMsgParams(" colId:%d  colName:%s  Data:%d [%s]\n",objTlvInfo->tag.colId, (char*)colName, value32Bit, counterIdName );
  }
  else
  {
    logMsgParams(" colId:%d  colName:%s  Data:%s\n",objTlvInfo->tag.colId, (char*)colName, objTlvInfo->data );
  }
}

/**
 * @param tlvArray  Contains Data received from BSM in TLV format
 * @details This function calls displayObjInfo for all tlvs received from BSM
 */
void printTlv(
  IO TlvArray   *tlvArray
  )
{
  Index    index;
  TableId  tempTableId;

  /* Logs Table Id, Table Name, Row Id */
  tempTableId = printTlvTagInfo(&tlvArray->tlvInfo[0].tag);

  for(index = 0; index < (tlvArray->numTlvs) ; index++)
  {
	if( tlvArray->tlvInfo[index].tag.colId EQ 1 )
	{
      logMsgParams(" Information of RowId=%d \n", tlvArray->tlvInfo[index].tag.rowId );
	}
	displayTlvInfo(&tlvArray->tlvInfo[index]);
  }
}

#if 0  /* Previous code. This is the way tlv's were printed. but as of now,
          get on tables will lead to different GET/SET commands.
          Kept code for reference. To be removed later - 4th July 2011 */
void printTlv(
  IO TlvArray   *tlvArray
  )
{
  Index    index;
  TableId  tempTableId;

  /* Logs Table Id, Table Name, Row Id */
  tempTableId = printTlvTagInfo(&tlvArray->tlvInfo[0].tag);

  for(index = 0; index < (tlvArray->numTlvs) ; index++)
  {
    if(tempTableId EQ tlvArray->tlvInfo[index].tag.tableId)
    {
      displayTlvInfo(&tlvArray->tlvInfo[index]);
    }
    else /* There is a chance of Encode Different tables Info in single Message  */
    {
      /* Logs Table Id, Table Name, Row Id */
      tempTableId = printTlvTagInfo(&tlvArray->tlvInfo[index].tag);
      displayTlvInfo(&tlvArray->tlvInfo[index]);
    }
  }
}

#endif

/**
 * @param tagArray  Contains Data received from BSM in TLV format
 * @details This function calls displayObjInfo for all tlvs received from BSM
 */
void printTag(
  IO TagArray   *tagArray
  )
{
  Index  index;
  Index tempTableId;

  /* Logs Table Id, Table Name, Row Id */
  tempTableId = printTlvTagInfo(&tagArray->tagInfo[0]);

  for(index = 1; index < (tagArray->numTags) ; index++)
  {
    tempTableId = printTlvTagInfo(&tagArray->tagInfo[index]);
  }
}
/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs GetObjReqMsg
 */
void printGetObjReqMsg(
  IO void    *data,
  IO UINT16  len
  )
{
  GetObjReq       getReq;
  clearBytes( &getReq, sizeof(GetObjReq));

  decGetObjReqMsg( (UINT8*)data, len, &getReq );
  logMsgParams("%s", "{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&getReq.cmdRef);
  printTag(&getReq.tagArray);
  logMsgParams("%s", "}\n");
}


/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs GetObjRspSuccMsg
 */
void printGetObjRspSuccMsg(
  IO void    *data,
  IO UINT16  len
  )
{
  GetObjRspSucc    rspSucc;

  clearBytes( &rspSucc, sizeof(GetObjRspSucc));
  decGetObjRspSuccMsg( (UINT8*)data, len, &rspSucc );
  logMsgParams("%s", "{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&rspSucc.cmdRef);
  printTlv(&rspSucc.tlvArray);
  logMsgParams("%s", "}\n");
}

/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs GetObjRspFailMsg
 */
void printGetObjRspFailMsg(
  IO void    *data,
  IO UINT16  len
  )
{
  GetObjRspFail   rspFail;

  clearBytes( &rspFail, sizeof(GetObjRspFail));

  decGetObjRspFailMsg( (UINT8*)data, len, &rspFail );
  logMsgParams("%s", "{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&rspFail.cmdRef);
  printErrorInfo(&rspFail.getErrRsp);
  logMsgParams("%s", "Failed Tag Info :\n");
  printTlvTagInfo(&rspFail.getErrRsp.failedTagInfo);
  logMsgParams("%s", "TLVs sent in Request :\n");
  printTag( &rspFail.tagArray);
  logMsgParams("%s", "}\n");
}

/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs SetObjReqMsg
 */
void printSetObjReqMsg(
  IO void    *data,
  IO UINT16  len
  )
{
  SetObjReq       setReq;
  clearBytes( &setReq, sizeof(SetObjReq));

  decSetObjReqMsg( (UINT8*)data, len, &setReq );
  logMsgParams("%s", "{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&setReq.cmdRef);
  printTlv( &setReq.tlvArray);
  logMsgParams("%s", "}\n");
}


/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs SetObjRspSuccMsg
 */
void printSetObjRspSuccMsg(
  IO void    *data,
  IO UINT16  len
  )
{
  SetObjRspSucc   rspSucc;
  clearBytes( &rspSucc, sizeof(SetObjRspSucc));
  /* Decode TLV's */
  decSetObjRspSuccMsg( (UINT8*)data, len, &rspSucc );
  logMsgParams("%s", "{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&rspSucc.cmdRef);
  printTlv(&rspSucc.tlvArray);
  logMsgParams("%s", "}\n");
}

/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs SetObjRspFailMsg
 */
void printSetObjRspFailMsg(
  IO void    *data,
  IO UINT16  len
  )
{
  SetObjRspFail   rspFail;
  clearBytes( &rspFail, sizeof(SetObjRspFail));

  decSetObjRspFailMsg( (UINT8*)data, len, &rspFail );
  logMsgParams("%s", "{\n");
  printTlvCmdRef(&rspFail.cmdRef);
  printErrorInfo(&rspFail.setErrRsp);
  logMsgParams("%s", "Failed Tag Info:\n");
  printFailTlvTagInfo(&rspFail.setErrRsp.failedTagInfo);
  logMsgParams("%s", "TLVs sent in Request :\n");
  printTlv(&rspFail.tlvArray);
  logMsgParams("%s", "}\n");
}

/**
 * @param   cfg           LibConfig parameters
 * @param   tableNum      represents table number of OAM tables
 * @param   oamTableInfo  global context information
 * @details This function reads information from configuration file for
 *          OAM tables columns information and fills global context.
 */
void readColForLibPrint(
  IN config_t                 *cfg,
  IN UINT32                   *tableNum,
  IO OamTableInfoForTlvPrint  *oamTableInfo
  )
{
  char             colInfo[MAX_COL_NAME];
  config_setting_t *setting;

  /* Reading Column Info */
  sprintf(colInfo,"oamTableConfig.table%dcolCfg",*tableNum);
  setting = config_lookup(cfg, colInfo);
  if(setting NOTEQ NULL)
  {
   unsigned int count = config_setting_length(setting);
   unsigned int i;

   for(i = 0; i < count; ++i)
   {
     config_setting_t *colInfoSett = config_setting_get_elem(setting, i);
     /* Only output the record if all of the expected fields are present. */
     const char *colName, *dataType ,*desc, *attributes, *specialValue, *persistent;
     int colId,length,colDataType,colMinVal,colMaxVal,isSpecialValue;

     if(!(config_setting_lookup_string(colInfoSett, "colName", &colName)
        && config_setting_lookup_int(colInfoSett, "colId", &colId)
        && config_setting_lookup_string(colInfoSett, "dataType", &dataType)
        && config_setting_lookup_int(colInfoSett, "length", &length)
        && config_setting_lookup_int(colInfoSett, "colMinVal", &colMinVal)
        && config_setting_lookup_int(colInfoSett, "colMaxVal", &colMaxVal)
        && config_setting_lookup_string(colInfoSett, "desc", &desc)
        && config_setting_lookup_string(colInfoSett, "attributes", &attributes)
        && config_setting_lookup_string(colInfoSett, "persistent", &persistent)
        && config_setting_lookup_int(colInfoSett, "isSpecialValue", &isSpecialValue)
        && config_setting_lookup_string(colInfoSett, "specialValue", &specialValue)))
       continue;

     strncpy(oamTableInfo->colInfo[colId].colName,colName,sizeof(ColName));
    // oamTableInfo->colInfo[colId].colId = colId;
     if (strcmp(dataType, "SINT8") EQ 0)
     {
       colDataType = BS_TYPE_SINT8;
     }
     else if (strcmp(dataType, "SINT16") EQ 0)
     {
       colDataType = BS_TYPE_SINT16;
     }
     else if (strcmp(dataType, "SINT32") EQ 0)
     {
       colDataType = BS_TYPE_SINT32;
     }
     else if (strcmp(dataType, "UINT8") EQ 0)
     {
       colDataType = BS_TYPE_UINT8;
     }
     else if (strcmp(dataType, "UINT16") EQ 0)
     {
       colDataType = BS_TYPE_UINT16;
     }
     else if (strcmp(dataType, "UINT32") EQ 0)
     {
       colDataType = BS_TYPE_UINT32;
     }
     else if (strcmp(dataType, "OCTET-STRING") EQ 0)
     {
       colDataType = BS_TYPE_STRING;
     }
     else if (strcmp(dataType, "ENUM") EQ 0)
     {
       colDataType = BS_TYPE_ENUM;
     }
     else if (strcmp(dataType, "BOOLEAN") EQ 0)
     {
       colDataType = BS_TYPE_BOOL;
     }
     else if (strcmp(dataType, "NodeAddr") EQ 0)
     {
       colDataType = BS_TYPE_STRING;
     }
     else if (strcmp(dataType, "ModuleId") EQ 0)
     {
       colDataType = BS_TYPE_MODID;
     }
     else if (strcmp(dataType, "MsgId") EQ 0)
     {
       colDataType = BS_TYPE_MSGID;
     }
     else if (strcmp(dataType, "ENUM") EQ 0)
     {
       colDataType = BS_TYPE_ENUM;
     }
     else if (strcmp(dataType, "IPV4ADDRESS") EQ 0)
     {
       colDataType = BS_TYPE_IPV4ADDRESS;
     }
     else if (strcmp(dataType, "MACADDRESS") EQ 0)
     {
       colDataType = BS_TYPE_MACADDRESS;
     }
     else if (strcmp(dataType, "FILENAME") EQ 0)
     {
       colDataType = BS_TYPE_FILENAME;
     }
     else if (strcmp(dataType, "LOCALTIME") EQ 0)
     {
       colDataType = BS_TYPE_LOCALTIME;
     }
     else if (strcmp(dataType, "OFFSETTIME") EQ 0)
     {
       colDataType = BS_TYPE_OFFSETTIME;
     }
     else if (strcmp(dataType, "SWVERSION") EQ 0)
     {
       colDataType = BS_TYPE_SWVERSION;
     }
     else if (strcmp(dataType, "MCC") EQ 0)
     {
       colDataType = BS_TYPE_MCC;
     }
     else if (strcmp(dataType, "MNC") EQ 0)
     {
       colDataType = BS_TYPE_MNC;
     }
     else if (strcmp(dataType, "LATITUDE") EQ 0)
     {
       colDataType = BS_TYPE_LATITUDE;
     }
     else if (strcmp(dataType, "LONGITUDE") EQ 0)
     {
       colDataType = BS_TYPE_LONGITUDE;
     }
     else if (strcmp(dataType, "ENODEBNAME") EQ 0)
     {
       colDataType = BS_TYPE_ENODEBNAME;
     }
     else if (strcmp(dataType, "PROFILENAME") EQ 0)
     {
       colDataType = BS_TYPE_PROFILENAME;
     }
     else if (strcmp(dataType, "COUNTERID") EQ 0)
     {
       colDataType = BS_TYPE_COUNTERID;
     }
     else
     {
       colDataType = BS_TYPE_UINT32;
     }
     oamTableInfo->colInfo[colId].dataType = colDataType;
   }
  }
}

/**
 * @param   cfg         Lib Configuration parameters
 * @param   tableNum    represents table number of OAM tables
 * @param   oamCtx      OAM table context
 * @details This function reads information from configuration file and fills global context.
 */
void readTableForLibPrint(
  IN config_t                   *cfg,
  IN UINT32                     *tableNum,
  IO OamTableContextForTlvPrint *oamCtx
  )
{
  char                        tableInfo[MAX_TABLE_NAME];
  config_setting_t            *setting = NULL;
  OamTableInfoForTlvPrint     *oamTableInfo = NULL;

  sprintf(tableInfo,"oamTableConfig.table%dCfg",*tableNum);
  setting = config_lookup(cfg, tableInfo);
  if(setting NOTEQ NULL)
  {
    int count = config_setting_length(setting);
    int i;

    for(i = 0; i < count; ++i)
    {
      config_setting_t *tableInfo = config_setting_get_elem(setting, i);
      /* Only output the record if all of the expected fields are present. */
      const char *tableName, *desc, *tableType;
      int actualTableNumber, tempTableNumber ,maxRows,noOfColumns,tableEnumType;

      if(!(config_setting_lookup_string(tableInfo, "tableName", &tableName)
          && config_setting_lookup_int(tableInfo, "noOfColumns", &noOfColumns)
         && config_setting_lookup_int(tableInfo, "tableNumber", &tempTableNumber)
         && config_setting_lookup_int(tableInfo, "maxRows", &maxRows)
         && config_setting_lookup_string(tableInfo, "tableType",&tableType)
         && config_setting_lookup_string(tableInfo, "desc", &desc)))
        continue;


      if (strcmp(tableType, "OprCmd") EQ 0)
      {
    	actualTableNumber = tempTableNumber - OPR_CMD_TABLES_OFFSET;
    	oamTableInfo = &oamCtx->oprCmdTableInfo[actualTableNumber];
        tableEnumType = TABLE_OPERCMD;
      }
      else if (strcmp(tableType, "OprConfig") EQ 0)
      {
    	actualTableNumber = tempTableNumber - OPR_CFG_TABLES_OFFSET ;
    	oamTableInfo = &oamCtx->oprCfgTableInfo[actualTableNumber];
        tableEnumType = TABLE_OPERCONFIG;
      }
      else if(strcmp(tableType, "Status") EQ 0)
      {
    	actualTableNumber = tempTableNumber - STATUS_TABLES_OFFSET ;
    	oamTableInfo = &oamCtx->statusTableInfo[actualTableNumber];
        tableEnumType = TABLE_STATUS;
      }
      else if(strcmp(tableType, "PreConfig") EQ 0)
      {
        actualTableNumber = tempTableNumber - PRE_CFG_TABLES_OFFSET ;
    	oamTableInfo = &oamCtx->preCfgTableInfo[actualTableNumber];
        tableEnumType = TABLE_PRECONFIG;
      }
      else
      {
        logError("wrong table type [%d]. Please check oamConfig.tpl", tableType);
      }

      clearBytes(oamTableInfo,sizeof(OamTableInfoForTlvPrint));
      strncpy(oamTableInfo->tableName, tableName, sizeof(TableName));
      readColForLibPrint(cfg, tableNum, oamTableInfo);
    }
  }
}


/**
 * @param   configFilePath represents configuration path
 * @param   oamCtx         OAM table context
 * This reads the configuration file 'oamConfig.tpl' and displays some of its contents.
 */
int parseOamCfgForLibPrint(
  IN UINT8*                     configFilePath,
  IO OamTableContextForTlvPrint *oamCtx
  )
{
  config_t   cfg;
  UINT8      fullFilePath[MAX_BUFFER_SIZE];
  const char *str;
  int        noOfTables,tableIndex;
  UINT32     *noOfValidTables;

  config_init(&cfg);
  constructFullPath ( configFilePath, "oamConfig.tpl", fullFilePath );

  /* Read the file. If there is an error, report it and exit. */
  if(!config_read_file(&cfg, fullFilePath))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    return(EXIT_FAILURE);
  }
  /* Get the configuration file name. */
  config_lookup_string(&cfg, "name", &str);
  /* To find Number Of Tables */
  if(config_lookup_int(&cfg, "noOfTables", &noOfTables))
  {
    noOfValidTables = &oamCtx->noOfvalidTables;
    *noOfValidTables = noOfTables;
  }

  /* Operator Commands */
  for (tableIndex = MIN_OPR_CMD_TABLEID ; tableIndex <= MAX_OPR_CMD_TABLEID ; tableIndex ++)
  {
    readTableForLibPrint(&cfg,&tableIndex,oamCtx);
  }
  /* Operator Config */
  for (tableIndex = MIN_OPR_CFG_TABLEID ; tableIndex <= MAX_OPR_CFG_TABLEID ; tableIndex ++)
  {
    readTableForLibPrint(&cfg,&tableIndex,oamCtx);
  }
  /* Status */
  for (tableIndex = MIN_STATUS_TABLEID ; tableIndex <= MAX_STATUS_TABLEID ; tableIndex ++)
  {
    readTableForLibPrint(&cfg,&tableIndex,oamCtx);
  }
  /* Pre Config */
  for (tableIndex = MIN_PRE_CFG_TABLEID ; tableIndex <= MAX_PRE_CFG_TABLEID ; tableIndex ++)
  {
    readTableForLibPrint(&cfg,&tableIndex,oamCtx);
  }

  config_destroy(&cfg);
  return(EXIT_SUCCESS);
}



/** errorCodeStr 
 * @file    print_noti.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file is the starting point for SIH module.
 *
 * @author     : Girish Vakati
 * Date       : 15-03-2011
 * Description: 1. Basic version
 *
 */

#include "print_noti.h"

#define MAX_TABLE_NAME 50
#define MAX_COL_NAME   50

/* NOTI MSG Context */
NotiMsgContext     g_PrintTlvNotiMsgContext;

/* Used for printing Enums into string */
extern NotiEnumStrInfo c_NotiMsgEnumStrInfoTab[];

/**
 * @param globalConfigFilePath configuration file path
 * @details This function parse notiMsgCfg.tpl for libPrint.
 * */
void printNotiMsgTlvInit(
  IO UINT8 *globalConfigFilePath
  )
{
  /* Parse Noti Msg Configuration Template */
  NotiMsgContext *notiMsgCtx = getNotiMsgTlvInfo();
  parseNotiMsgCfg(globalConfigFilePath,notiMsgCtx);
}

/**
 * @param notiMsgId notification ID
 * @param colId     column ID
 * @details This function returns EnumStrInfo structure for the given notiMsgId
 * */
NotiEnumStrInfo *getNotiMsgEnumStrFunction(
  IO NotiId notiMsgId,
  IO ColId  colId
  )
{
  Index index = 0 ;

  for(index=0; (c_NotiMsgEnumStrInfoTab[index].notiId NOTEQ 0); index++)
  {
    if(c_NotiMsgEnumStrInfoTab[index].notiId EQ notiMsgId)
    {
      if(c_NotiMsgEnumStrInfoTab[index].colId EQ colId )
      {
        return (NotiEnumStrInfo*)&(c_NotiMsgEnumStrInfoTab[index]);
      }
    }
  }
  return NULL; /* In case of Enum is not found */
}

/**
 * @param notiMsgId Specifies the Notification Message Id
 * @details Prints the NotiMsg info
 * */
void printNotiMsgInfo(
  IN NotiId notiMsgId
  )
{
  NotiMsgName   *notiMsgName = NULL;
  MsgBsmSihNotiInfo 	*notiMsgInfo = NULL ;

  logMsgParams(" NotiMsg Id=%d\n", notiMsgId);

  notiMsgInfo = printTlvGetNotiMsgPtrByNotiMsgId(notiMsgId);
  if(notiMsgInfo NOTEQ NULL)
  {
    notiMsgName = &notiMsgInfo->notiMsgName;
    logMsgParams(" NotiMsg Name=%s\n", notiMsgName);
  }
  else
  {
	logMsgParams(" NotiMsg Name=%s\n", "UNKNOWN NOTIMSG");
  }
}

/**
 * @param notiMsgId Specifies the Notification Message Id
 * @details This functions returns returns MsgPtr by using notiMsgId.
 * */
MsgBsmSihNotiInfo* printTlvGetNotiMsgPtrByNotiMsgId(
  IN NotiId      notiMsgId
  )
{
  MsgBsmSihNotiInfo *notiMsgInfo = NULL ;

  notiMsgInfo = tlvPrintGetNotiMsgInfo(notiMsgId-MIN_NOTI_MSG_ID);

  return notiMsgInfo;
}

/**
 * @param notiObj     NotiObj
 * @param objTlvInfo  Noti TLV info
 * @param colName     Column Name
 * @param dataType    Data Type
 * @details This function prints data based on dataType.
 * */
void getAndPrintDataType(
  IO NotiObj  *notiObj,
  IO NotiTlvInfo *objTlvInfo,
  IO ColName  *colName,
  IO DataType *dataType 
  )
{
  UINT32          value32Bit = 0;
  UINT16          value16Bit = 0;
  UINT8           value8Bit = 0;
  SINT32          signedValue32Bit = 0;
  SINT16          signedValue16Bit = 0;
  SINT8           signedValue8Bit = 0;
  SINT32	  tempVal;
 
  NotiEnumStrInfo *enumToStrInfo = NULL;
  UINT8           *colNameForEnum = NULL;

  if(*dataType EQ BS_TYPE_STRING )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_UINT32 )
  {
    value32Bit =  getUint32FromArray(objTlvInfo->data);
    logMsgParams(" colId:%d colName:%s Data:%u\n",objTlvInfo->tagColId, (char*)colName, value32Bit );
  }
  else if(*dataType EQ BS_TYPE_SINT32)
  {
	tempVal =  getUint32FromArray(objTlvInfo->data);
    signedValue32Bit = (SINT32)tempVal;
    logMsgParams(" colId:%d colName:%s Data:%d\n",objTlvInfo->tagColId, (char*)colName, signedValue32Bit );
  }
  else if(*dataType EQ BS_TYPE_UINT16 )
  {
    value16Bit =  getUint16FromArray(objTlvInfo->data);
    logMsgParams(" colId:%d colName:%s Data:%hu\n",objTlvInfo->tagColId, (char*)colName, value16Bit );
  }
  else if(*dataType EQ BS_TYPE_SINT16)
  {
    tempVal = getUint16FromArray( objTlvInfo->data );
    signedValue16Bit = (SINT16)tempVal;
    logMsgParams(" colId:%d colName:%s Data:%hd\n",objTlvInfo->tagColId, (char*)colName, signedValue16Bit );
  }
  else if(*dataType EQ BS_TYPE_UINT8 )
  {
    value8Bit =  getUint8FromArray(objTlvInfo->data);
    logMsgParams(" colId:%d colName:%s Data:%u\n",objTlvInfo->tagColId, (char*)colName, value8Bit );
  }
  else if(*dataType EQ BS_TYPE_SINT8)
  {
    tempVal = getUint8FromArray( objTlvInfo->data );
    signedValue8Bit = (SINT8)tempVal;
    logMsgParams(" colId:%d colName:%s Data:%d\n",objTlvInfo->tagColId, (char*)colName, signedValue8Bit );
  }
  else if(*dataType EQ BS_TYPE_ENUM )
  {
    value32Bit =  getUint32FromArray(objTlvInfo->data);
  
    enumToStrInfo = getNotiMsgEnumStrFunction( notiObj->notiId, objTlvInfo->tagColId);
    if(enumToStrInfo EQ NULL)
    {
      logError("%s", "NotiMsgEnumToString Function not found; Please generate enumstr_notimsg.c functions and replace in code");
      exit(EXIT_FAILURE);
    }
    else
    {
      colNameForEnum = enumToStrInfo->enumStrHdlr(value32Bit);
    }
    logMsgParams(" colId:%d colName:%s Data:%d [%s]\n",objTlvInfo->tagColId, (char*)colName, value32Bit, colNameForEnum);
  }
  else if(*dataType EQ BS_TYPE_BOOL )
  {
    value8Bit =  getUint8FromArray(objTlvInfo->data);
    if(value8Bit EQ 0)
    {
      logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, "FALSE" );
    }
    else
    {
      logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, "TRUE" );
    }
  }
  else if(*dataType EQ BS_TYPE_MODID )
  {
    value32Bit =  getUint32FromArray(objTlvInfo->data);

    colNameForEnum = enumModIdToStr(value32Bit);
    logMsgParams(" colId:%d  colName:%s  Data:%d [%s]\n",objTlvInfo->tagColId, (char*)colName, value32Bit, colNameForEnum );
  }
  else if(*dataType EQ BS_TYPE_IPV4ADDRESS )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_MACADDRESS )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_FILENAME )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_LOCALTIME )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_OFFSETTIME )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_SWVERSION )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_MCC )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_MNC )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_LATITUDE )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_LONGITUDE )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_ENODEBNAME )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, objTlvInfo->data );
  }
  else if(*dataType EQ BS_TYPE_PROFILENAME )
  {
    logMsgParams(" colId:%d colName:%s Data:%s\n",objTlvInfo->tagColId, (char*)colName, objTlvInfo->data );
  }
  else
  {
    logMsgParams(" colId:%d  colName:%s  Data:%s\n",objTlvInfo->tagColId, (char*)colName, objTlvInfo->data );
  }
}


/**
 * @param notiObj     NotiObj
 * @param objTlvInfo  Contains Data received from BSM in TLV format for each Object
 * @details display ObjInfo i.e NotiMsgName, Column Name and Data received from BSM
 */
void displayNotiTlvInfo(
  IO NotiObj	  *notiObj,
  IO NotiTlvInfo  *objTlvInfo
  )
{
  ColName        *colName = NULL;
  DataType       *dataType = NULL;
  
  MsgBsmSihNotiInfo *notiMsgInfo = NULL ;

  notiMsgInfo = printTlvGetNotiMsgPtrByNotiMsgId(notiObj->notiId);
  if(notiMsgInfo EQ NULL)
  {
	logMsgParams("  NOTI MSG Name=%s ", "UNKNOWN NOTI MSG");
	return;
  }

  colName   = &notiMsgInfo->notiMsgColInfo[objTlvInfo->tagColId].colName;
  dataType  = &notiMsgInfo->notiMsgColInfo[objTlvInfo->tagColId].dataType;
  getAndPrintDataType( notiObj, objTlvInfo, colName, dataType );	
}

/**
 * @param notiObj  NotiObj
 * @details This function calls displayObjInfo for all tlvs received from BSM
 */
void printNotiMsgTlv(
  IO NotiObj   *notiObj
  )
{
  Index    index;

  /* Prints NotiMsg Id, NotiMsg Name */
  printNotiMsgInfo(notiObj->notiId);
  for(index = 0; index < (notiObj->numTlvs) ; index++)
  {
      displayNotiTlvInfo(notiObj, &notiObj->notiTlvInfo[index]);
  }
}

/**
 * @param data  Contains Encoded Data in TLV format
 * @param len length of the message sent on socket
 * @details This function will Logs NotiObjReqMsg
 */
void printNotiObjReqMsg(
  IO void    *data,
  IO UINT16  len
  )
{
  NotiObj   notiObj;
  clearBytes( &notiObj, sizeof(NotiObj));

  decNotiObjMsg( (UINT8*)data, len, &notiObj );
  logMsgParams("%s","{\n");
  printNotiMsgTlv(&notiObj);
  logMsgParams("%s","}\n");
}

/**
 * @param   cfg   LibConfig parameters
 * @param   notiMsgNum represents notification message number of NotiMsg
 * @param   notiMsgInfo global context information
 * @details This function reads information from configuration file for
 *          NotiMsg columns information and fills global context.
 */
void readNotiMsgCol(
  IN config_t      *cfg,
  IN UINT32        *notiMsgNum,         /* Temporary NotiMsg  number for reading from tpl file */
  IO MsgBsmSihNotiInfo  *notiMsgInfo
  )
{
  char             colInfo[MAX_COL_NAME];
  config_setting_t *setting = NULL;

  /* Reading Column Info */
  sprintf(colInfo,"notiMsgConfig.notiMsg%dcolCfg",*notiMsgNum);
  setting = config_lookup(cfg, colInfo);
  if(setting NOTEQ NULL)
  {
   unsigned int count = config_setting_length(setting);
   unsigned int i;

   for(i = 0; i < count; ++i)
   {
     config_setting_t *colInfoSett = config_setting_get_elem(setting, i);
     /* Only output the record if all of the expected fields are present. */
     const char *colName, *dataType;
     int colId,length,colDataType;

     if(!(config_setting_lookup_string(colInfoSett, "ColName", &colName)
        && config_setting_lookup_int(colInfoSett, "colId", &colId)
        && config_setting_lookup_string(colInfoSett, "dataType", &dataType)
        && config_setting_lookup_int(colInfoSett, "length", &length)))
       continue;

     strncpy(notiMsgInfo->notiMsgColInfo[colId].colName,colName,sizeof(ColName));
     notiMsgInfo->notiMsgColInfo[colId].colId = colId;
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
     notiMsgInfo->notiMsgColInfo[colId].dataType = colDataType;
     notiMsgInfo->notiMsgColInfo[colId].length = length;

   }
  }
}

/**
 * @param   cfg          Lib Configuration parameters
 * @param   notiMsgNum   represents NotiMsg number of Notification Messages
 * @param   notiMsgCtx   NotiMsgContext
 * @details This function reads information from configuration file and fills global context.
 */
void readNotiMsgTable(
  IN config_t        *cfg,
  IN UINT32          *notiMsgNum,
  IO NotiMsgContext *notiMsgCtx
  )
{
  char             tableInfo[MAX_TABLE_NAME];
  config_setting_t *setting = NULL ;
  MsgBsmSihNotiInfo      *notiMsgInfo = NULL ;

  sprintf(tableInfo,"notiMsgConfig.notiMsg%dCfg",*notiMsgNum);
  setting = config_lookup(cfg, tableInfo);
  if(setting NOTEQ NULL)
  {
    int count = config_setting_length(setting);
    int i;

    for(i = 0; i < count; ++i)
    {
      config_setting_t *tableInfo = config_setting_get_elem(setting, i);
      /* Only output the record if all of the expected fields are present. */
      const char *notiMsgName;
      int actualNotiMsgId, tempNotiMsgId, noOfColumns;

      if(!(config_setting_lookup_string(tableInfo, "notiMsgName", &notiMsgName)
          && config_setting_lookup_int(tableInfo, "noOfColumns", &noOfColumns)
         && config_setting_lookup_int(tableInfo, "notiMsgId", &tempNotiMsgId)))
        continue;


      actualNotiMsgId = tempNotiMsgId - NOTI_MSGS_OFFSET;
      notiMsgInfo = &notiMsgCtx->notiMsgInfo[actualNotiMsgId];
      clearBytes(notiMsgInfo,sizeof(MsgBsmSihNotiInfo));

      /* Fill GLobal context with NOTI MESG data */
      notiMsgInfo->notiMsgId = tempNotiMsgId;
      notiMsgInfo->noOfColumns = noOfColumns;
      strncpy(notiMsgInfo->notiMsgName, notiMsgName, sizeof(NotiMsgName));

      readNotiMsgCol(cfg, notiMsgNum, notiMsgInfo);
    }
  }
}


/**
 * @param   configFilePath represents configuration  path
 * @param   notiMsgCtx     NotiMsgContext
 * This reads the configuration file 'NotiMsgCfg.tpl' and displays
 * some of its contents.
 */
int parseNotiMsgCfg(
  IN UINT8*          configFilePath,
  IO NotiMsgContext *notiMsgCtx
  )
{
  config_t   cfg;
  UINT8      fullFilePath[MAX_BUFFER_SIZE];
  const char *str ;
  int        noOfNotiMsg,notiMsgIndex;
  UINT32     *noOfValidNotiMsg = NULL ;

  config_init(&cfg);
  constructFullPath ( configFilePath, "notiMsgCfg.tpl", fullFilePath );

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
  /* To find Number Of Noti Messages */
  if(config_lookup_int(&cfg, "noOfNotiMsg", &noOfNotiMsg))
  {
    noOfValidNotiMsg = &notiMsgCtx->noOfvalidNotiMsg;
    *noOfValidNotiMsg = noOfNotiMsg;
  }

  for (notiMsgIndex = MIN_NOTI_MSG_ID; notiMsgIndex <= MAX_NOTI_MSG_ID ; notiMsgIndex ++)
  {
    readNotiMsgTable(&cfg,&notiMsgIndex,notiMsgCtx);
  }

  config_destroy(&cfg);
  return(EXIT_SUCCESS);
}

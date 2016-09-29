/**
 * @file     cih_func.c
 * @details  Copyright Powerwave Technologies, Inc., 2010
 * @brief    This file contains utility functions that process, validate, parse and log responses of CLI Commands.
 *
 * @author   Madhusudana Rao Kata
 * @date     10-05-2010
 * @details     1. Merged cli_main.c and cli_oihMsgHandler.c @n
 *              2. Modified printing Response functions on 18-05-2010 @n
 *              3. Replaced Cli Help function with the function generated from Excel on 10-06-2010 @n
 *              4. Cih Config file is generated form Excel sheet and and generated help from cfg file on 14-06-2010 @n
 *              5. History Information command is added on 15-06-2010 @n
 *              6. Log file will be generated with all commands entered by the user @n
 *              7. Added procCihCmds and procOihRsp functions added for readability @n
 *              8. Display buffer is removed in Get success messages @n
 *              9. constructFullPath function is used for config files on July 01 2010 --Madhu @n
 *              10.TLV Changes(All functionalities are Modified) added @n
 *              11.Print enums in Strings functionality is added @n
 *              12.For status tables only GET is possible --16 Sept2010 @n
 */

#include "cih.h"
#include "cih_tecla.h"
#include<termios.h>

/* Used for printing Enums into string */
extern EnumStrInfo c_EnumStrInfoTab[];

/**
 *
 * @details    This function is used get MsgId for Set Object Request on each instance.
 */
MsgId getCihSetObjReqMsgId(void)
{
  MsgId msgId;
  InstanceId instanceId = getCihInstanceId();

  switch(instanceId)
  {
    case 1:
      msgId = MSG_CIH1_BSM_SET_OBJ_REQ;
      break;
    case 2:
      msgId = MSG_CIH2_BSM_SET_OBJ_REQ;
      break;
    case 3:
      msgId = MSG_CIH3_BSM_SET_OBJ_REQ;
      break;
    default:
      msgId = MSG_UNDEFINED;
  }

  return msgId;
}

/**
 *
 * @details This function is used get MsgId for Get Object Request on each instnace
 */
MsgId getCihGetObjReqMsgId(void)
{
  MsgId msgId;
  InstanceId instanceId = getCihInstanceId();

  switch(instanceId)
  {
    case 1:
      msgId = MSG_CIH1_BSM_GET_OBJ_REQ;
      break;
    case 2:
      msgId = MSG_CIH2_BSM_GET_OBJ_REQ;
      break;
    case 3:
      msgId = MSG_CIH3_BSM_GET_OBJ_REQ;
      break;
    default:
      msgId = MSG_UNDEFINED;
  }

  return msgId;
}
/**
 * @param   versionInfo Version Information
 * @details             This function is used to Print Banner
 */
void printBanner(
  IN VersionInfo   *versionInfo
  )
{
  String100   verString;
  String50    builtOnString;
  clearBytes( &verString, sizeof(String100) );
  clearBytes( &builtOnString, sizeof(String50));
  fprintf(stdout,"\n***********************************************\n");
  fprintf(stdout," Powerwave Technologies\n");
  fprintf(stdout," ENode-B Command Line Interface\n");
  fprintf(stdout," Version      : %s \n", versionInfo->version );
  fprintf(stdout," Build Date   : %s\n",  cihDateTimeConversion(versionInfo->builtOn, builtOnString) );
  fprintf(stdout," Username     : %s\n",  getUsername());
  fprintf(stdout,"*************************************************\n");
  fprintf(stdout," Type: \"help\" or \"?\" for more information \n");
}

/**
 * @details   This function is returns the username of who started CLI
 */
UINT8* getUsername()
{
  UINT8 *username = NULL;
  username = getenv("USER");
  if( NULL EQ username )
  {
    return "Missing/Unknown";
  }
  return username;
}

/**
 * @details   This function converts time to standard Linux format.
 */
UINT8* cihDateTimeConversion(
    IN UINT8 *builtOn,
    IO UINT8 *dateAndTimeString
    )
{
  SINT16 date = 0;
  SINT16 month = 0;
  SINT16 year = 0;
  SINT16 hours = 0;
  SINT16 minutes = 0;
  SINT16 seconds = 0;
  SINT16 milliseconds = 0;
  UINT8 temptime[5];

  /* To get year */
  snprintf( temptime, 5, "%s",  &builtOn[0]  );
  year = atoi(temptime);
  clearBytes( temptime, sizeof(temptime));
  /*To get month */
  snprintf( temptime,  3, "%s", &builtOn[4]  );
  month = atoi(temptime);
  clearBytes( temptime, sizeof(temptime));
  /*To get date */
  snprintf( temptime,  3, "%s", &builtOn[6]  );
  date = atoi(temptime);
  clearBytes( temptime, sizeof(temptime));
  /*To get hours */
  snprintf( temptime,  3, "%s", &builtOn[8]  );
  hours = atoi(temptime);
  clearBytes( temptime, sizeof(temptime));
  /*To get minutes */
  snprintf( temptime, 3, "%s",  &builtOn[10] );
  minutes = atoi(temptime);
  clearBytes( temptime, sizeof(temptime));
  /*To get seconds */
  snprintf( temptime, 3, "%s",  &builtOn[12] );
  seconds = atoi(temptime);
  clearBytes( temptime, sizeof(temptime));
  /*To get milliseconds */
  snprintf(temptime, 4, "%s",  &builtOn[14] );
  milliseconds = atoi(temptime);
  clearBytes( temptime, sizeof(temptime));

  switch(month)
  {
    case 1: strncpy(temptime, "Jan", 4);
            break;
    case 2: strncpy(temptime, "Feb", 4);
            break;
    case 3: strncpy(temptime, "Mar", 4);
            break;
    case 4: strncpy(temptime, "Apr", 4);
            break;
    case 5: strncpy(temptime, "May", 4);
            break;
    case 6: strncpy(temptime, "Jun", 4);
            break;
    case 7: strncpy(temptime, "Jul", 4);
            break;
    case 8: strncpy(temptime, "Aug", 4);
            break;
    case 9: strncpy(temptime, "Sep", 4);
            break;
    case 10: strncpy(temptime, "Oct", 4);
             break;
    case 11: strncpy(temptime, "Nov", 4);
             break;
    case 12: strncpy(temptime, "Dec", 4);
             break;
  }
  /* Constructing date and time in standard linux format */
  snprintf(dateAndTimeString, sizeof(String50), "%s %02d %02d:%02d:%02d %04d ", temptime, date, hours,
      minutes, seconds, year);

  return dateAndTimeString;
}

/**
 * @details This function is used to Close CIH Log file
 */
void cihCloseLog()
{
  fclose(g_cihCtx.cihCmdLogFilePointer);
}

/**
 * @param   userCmd   is the Command entered by the user at command prompt
 * @param   cmdNumber is the Command Number entered by the user at command prompt
 * @details           This function is used to Log CIH commands in a Log file.
 */
void cihLogCommands(
    IO UINT8*   userCmd,
    IO UINT32   cmdNumber
    )
{
  fprintf(g_cihCtx.cihCmdLogFilePointer,"cmdNumber:%d :%s\n",cmdNumber,userCmd);
}


/**
 * @param   rspString is the Response Message received from BSM or CIH(in case of wrong command entered by user)
 * @details           This function is used to Log CIH commands Responses in a Log file.
 */
void cihLogRsp(
    IO UINT8*   rspString
    )
{
  fprintf(g_cihCtx.cihCmdLogFilePointer,"%s\n",rspString);
}

/**
 * @param   rootDir   Path of Root Directory
 * @details           This function is used to Initialise Log file.
 */
void cihLogInit(
    IN UINT8   *rootDir
    )
{
  struct tm *sysTime = NULL;
  FilePath fullFilePath;
  time_t   timeVal = 0;
  timeVal = time(NULL);

  InstanceId instanceId = getCihInstanceId();
  sysTime = localtime(&timeVal);

  clearBytes(&fullFilePath, sizeof(FilePath) );
  if(instanceId)
  {
    snprintf( fullFilePath, sizeof(FilePath), "%s/log/cihCmdLog%d.txt", rootDir, instanceId );
  }
  else
  {
    snprintf( fullFilePath, sizeof(FilePath), "%s/log/cihCmdLog.txt", rootDir );
  }

  g_cihCtx.cihCmdLogFilePointer = fopen(fullFilePath , "a+");
  if( NULL EQ g_cihCtx.cihCmdLogFilePointer )
  {
    g_cihCtx.cihCmdLogFilePointer = stdout;
    cihErrInOpeningFile( fullFilePath );
  }
  fprintf(g_cihCtx.cihCmdLogFilePointer,"/*****Command Line Interface is Opened******/\n");
  fprintf(g_cihCtx.cihCmdLogFilePointer,"Date = %d - %d - %d and Time is %dH - %dM - %dS,\n",sysTime->tm_mday,sysTime->tm_mon+1,2000 + sysTime->tm_year,sysTime->tm_hour,sysTime->tm_min,sysTime->tm_sec);
}

/**********************************************************************************/
/*                           REQUEST MESSAGES FROM USER                           */
/**********************************************************************************/
/**
 * @param   tableId is the tableId extracted from user command
 * @param   cmdRef  is used to display Origin ,Command Reference number and
 *                  Instance Id of command in case of error
 * @details         This function is used to check Table type is Status Table or not,
 *                  if status table set command will send to operator.
 */
TBOOL isStatusTable(
    IO TableId  tableId,
    IO CmdRef   *cmdRef
    )
{
  TableName     *tableName = NULL;
  OamTableInfo  *oamTableInfo =  NULL;
  OamTableType   oamTableType = TABLE_UNKNOWNTYPE;

  oamTableType = getTableTypeByTableId(tableId);
  oamTableInfo =  getTablePtrByTableId(tableId);

  if(oamTableType EQ  TABLE_STATUS)
  {
    Buffer   displayBuffer;

    clearBytes(displayBuffer,sizeof(Buffer));
    cihDisplayCmdRefDeclined(cmdRef);
    if(oamTableInfo EQ NULL)
    {
      snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
          "%s   \n","Not Able to Find Table Info from Database");
      fputs(displayBuffer, stdout);
      cihLogRsp(displayBuffer);
      return FALSE;
    }
    tableName = &oamTableInfo->tableName;
    snprintf((UINT8*)&displayBuffer, sizeof(Buffer), "%s is Status Table, SET is not permitted on Status tables \n",(UINT8*)tableName);
    fputs(displayBuffer, stdout);
    cihLogRsp(displayBuffer);
    return FALSE;
  }
  return TRUE;
}

/**
 * @param   tableId  is the tableId extracted from user command
 * @param   cmdRef   is used to display Origin ,Command Reference number and
 *                   Instance Id of command in case of error
 * @details          This function is used to check Table type is Operator Cmd Table or not,
 *                   if status table get command will send to operator
 */
TBOOL isOprCmdTable(
    IO TableId  tableId,
    IO CmdRef   *cmdRef
    )
{
  OamTableInfo   *oamTableInfo =  NULL;
  TableName     *tableName = NULL;
  OamTableType   oamTableType = TABLE_UNKNOWNTYPE;

  oamTableType = getTableTypeByTableId(tableId);
  oamTableInfo =  getTablePtrByTableId(tableId);
  if(oamTableType EQ  TABLE_OPERCMD)
  {
    Buffer   displayBuffer;

    clearBytes(displayBuffer,sizeof(Buffer));
    cihDisplayCmdRefDeclined(cmdRef);

    if(oamTableInfo EQ NULL)
    {
      snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
          "%s   \n","Not Able to Find Table Info from Database");
      fputs(displayBuffer, stdout);
      cihLogRsp(displayBuffer);
      return FALSE;
    }
    tableName = &oamTableInfo->tableName;
    snprintf((UINT8*)&displayBuffer, sizeof(Buffer), "%s is Operator Command Table, GET is not permitted for Operator Command tables \n",(UINT8*)tableName);
    fputs(displayBuffer, stdout);
    cihLogRsp(displayBuffer);
    return FALSE;
  }
  return TRUE;
}

/**
 * @param   value  Operator data entered  at prompt as string
 * @details  This function is used to validate whether the data entered by user contains non-numeric
 * characters or negative number for Table/Row/Column Id.
 */
TBOOL checkNumericTableId(
    IN UINT8  *value
    )
{
  regex_t pattern;
  Buffer   displayBuffer;

  if( 0 EQ regcomp( &pattern, TABLEID, REG_EXTENDED ))
  {
    if( 0 EQ regexec( &pattern, value, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User entered non-numeric characters or negative number for Table ID");
      clearBytes(displayBuffer,sizeof(Buffer));
      snprintf((UINT8*)&displayBuffer, sizeof(Buffer), "Please Enter valid Table ID. Use 'help tables' Command for more information. \n");
      fputs(displayBuffer, stdout);
      cihLogRsp(displayBuffer);
      return FALSE;
    }
  }
  else
  {
    return FALSE;
  }
}

/**
 * @param   value  Operator data entered  at prompt as string
 * @details  This function is used to validate whether the data entered by user contains non-numeric
 * characters or negative number for Table/Row/Column Id.
 */
TBOOL checkNumericRowId(
    IN UINT8  *value
    )
{
  regex_t pattern;
  Buffer   displayBuffer;

  if( 0 EQ regcomp( &pattern, ROWID, REG_EXTENDED ))
  {
    if( 0 EQ regexec( &pattern, value, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User entered non-numeric characters or negative number for Row Id");
      clearBytes(displayBuffer,sizeof(Buffer));
      snprintf((UINT8*)&displayBuffer, sizeof(Buffer), "Please Enter valid Row Number. Use 'help tables' Command for more information. \n");
      fputs(displayBuffer, stdout);
      cihLogRsp(displayBuffer);
      return FALSE;
    }
  }
  else
  {
    return FALSE;
  }
}

/**
 * @param   value  Operator data entered  at prompt as string
 * @details  This function is used to validate whether the data entered by user contains non-numeric
 * characters or negative number for Table/Row/Column Id.
 */
TBOOL checkNumericColumnId(
    IN UINT8  *value
    )
{
  regex_t pattern;
  Buffer   displayBuffer;

  if( 0 EQ regcomp( &pattern, COLUMNID, REG_EXTENDED ))
  {
    if( 0 EQ regexec( &pattern, value, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User entered non-numeric characters or negative number for Column Id");
      clearBytes(displayBuffer,sizeof(Buffer));
      snprintf((UINT8*)&displayBuffer, sizeof(Buffer), "Please Enter valid Column Number. Use 'help <tableID>' Command for more information. \n");
      fputs(displayBuffer, stdout);
      cihLogRsp(displayBuffer);
      return FALSE;
    }
  }
  else
  {
    return FALSE;
  }
}

/**
 * @param   tableId is the tableId extracted from user command
 * @param   cmdRef  is used to display Origin ,Command Reference number and
 *                  Instance Id of command in case of error
 * @details         This function is used to Validate Table Number entered by the user.
 */
TBOOL validateTableId(
    IN UINT32   tableId
    )
{
  Buffer   displayBuffer;

  OamTableType    tableType = 0 ;

  tableType = getTableTypeByTableId(tableId);

  if(tableType EQ TABLE_UNKNOWNTYPE)
  {
    clearBytes(displayBuffer,sizeof(Buffer));
    snprintf((UINT8*)&displayBuffer, sizeof(Buffer), "Please Enter valid Table Number. Use 'help tables' Command for more information. \n");
    fputs(displayBuffer, stdout);
    cihLogRsp(displayBuffer);
    return FALSE;
  }
  return TRUE;
}
/**
 * @param   tableId is the tableId extracted from user command
 * @param   rowId   is the Row Id extracted from user command
 * @param   cmdRef  is used to display Origin ,Command Reference number and
 *                  Instance Id of command in case of error
 * @details         This function is used to Validate Row Number entered by the user.
 */
TBOOL validateRowId(
    IN UINT32   tableId,
    IN UINT32   rowId
    )
{
  OamTableInfo  *oamTableInfo = NULL;

  oamTableInfo =  getTablePtrByTableId(tableId);

  if(oamTableInfo EQ NULL)
  {
    Buffer   displayBuffer;
    clearBytes(displayBuffer,sizeof(Buffer));
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "%s   \n","Not Able to Find Table Info from Database");
    fputs(displayBuffer, stdout);
    cihLogRsp(displayBuffer);
    return FALSE;
  }
  if(rowId > oamTableInfo->noOfRows )
  {
    Buffer   displayBuffer;
    clearBytes(displayBuffer,sizeof(Buffer));
    snprintf((UINT8*)&displayBuffer, sizeof(Buffer), "Please Enter valid Row Number - Range is 1 to %d \n.", oamTableInfo->noOfRows);
    fputs(displayBuffer, stdout);
    cihLogRsp(displayBuffer);
    return FALSE;
  }
  return TRUE;
}

/**
 * @param   tableId is the tableId extracted from user command
 * @param   colId   is the Column Id extracted from user command
 * @param   cmdRef  is used to display Origin ,Command Reference number and
 *                  Instance Id of command in case of error
 * @details         This function is used to Validate Row Number entered by the user.
 */
TBOOL validateColId(
    IN UINT32   tableId,
    IN UINT32   colId
    )
{
  OamTableInfo  *oamTableInfo = NULL;

  oamTableInfo =  getTablePtrByTableId(tableId);

  if(oamTableInfo EQ NULL)
  {
    Buffer   displayBuffer;
    clearBytes(displayBuffer,sizeof(Buffer));
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "%s   \n","Not Able to Find Table Info from Database");
    fputs(displayBuffer, stdout);
    cihLogRsp(displayBuffer);
    return FALSE;
  }
  if(colId > oamTableInfo->noOfColumns)
  {
    Buffer   displayBuffer;
    clearBytes(displayBuffer,sizeof(Buffer));
    snprintf((UINT8*)&displayBuffer, sizeof(Buffer), "Please Enter valid Column Number - Range is 1 to %d. \n", oamTableInfo->noOfColumns);
    fputs(displayBuffer, stdout);
    cihLogRsp(displayBuffer);
    return FALSE;
  }
  return TRUE;
}

/**
 * @param tableId  is the table  ID extracted from user command.
 * @param rowId    is the row    ID extracted from user command.
 * @param colId    is the column ID extracted from user command.
 * @param cmdRef   is used to display Origin, Command Reference number and
 *                 Instance Id of command in case of error.
 * @details        This function is used to Validate Table,Row and Column Numbers entered by the user.
 */
TBOOL validateObjId(
    IO UINT32   tableId,
    IO UINT32   rowId,
    IO UINT32   colId
    )
{
  if(!(validateTableId(tableId) ))
  {
    return FALSE;
  }
  if(!(validateRowId(tableId, rowId)))
  {
    return FALSE;
  }
  if(!(validateColId(tableId, colId)))
  {
    return FALSE;
  }
  return TRUE;
}


/**
 * @param   inputStr is user Command String entered at CIH Interface
 * @param   tokens   is the array of tokens after parsing user string
 * @details          This function is used to Parse CIH string and divide it into tokens.
 */
UINT8 cihParseCmdStr(
    IO UINT8  *inputStr,
    IO ObjStr *tokens
    )
{
  UINT8 *word = NULL;
  UINT8 index = 0;
  UINT8 *delim   = " \n";

  clearBytes(tokens,(sizeof(ObjStr) * MAX_TOKENS_IN_CIH_COMMAND));
  word = strtok(inputStr, delim);
  if(word NOTEQ NULL)
  {
    strcpy(tokens[index++],word);
  }
  word = strtok(NULL, delim);
  while(word NOTEQ NULL)
  {
    strcat(tokens[index],word);
    word = strtok(NULL,delim);
    if(word EQ NULL)
      index++;
  }
  *tokens[index]='\0';
  return index;
}

/**
 * @param   inputObjIdStr is a string contains Object Id coming from CIH
 * @param   tokens        is an array of strings used to store output tokens after parsing input string
 * @details               This function Parse Incoming ObjId string as per delimiters
 */
UINT8 cihParseObjIdStr(
    IO UINT8    *inputObjIdStr,
    IO ObjIdStr *tokens
    )
{
  UINT8 *word = NULL;
  UINT8 index = 0;
  UINT8 *delim   = ".";
  word = strtok(inputObjIdStr, delim);
  while(word NOTEQ NULL)
  {
    strcpy(tokens[index++],word);
    word = strtok(NULL,delim);
  }
  return index;
}

/**
 * @param   inputObjValStr is a string contains Object Value string coming from CIH
 * @details                This function Checks the invalid commands entered
 */

UINT8 CheckObjValStr(
    IO UINT8     *inputObjValStr
    )
{
  UINT32 index;
  UINT32 strlength=0;
  UINT32 flag=0;
  strlength=strlen(inputObjValStr);  

  if((inputObjValStr[0] EQ '{') && (inputObjValStr[strlength - 1] EQ '}'))
  {
    flag = TRUE;
  }
  else if ((inputObjValStr[0] NOTEQ '{') && (inputObjValStr[strlength - 1] NOTEQ '}'))
  {
    flag = TRUE;
  }
  else
  {
    flag = FALSE;
  }
  for(index=1;index<strlen(inputObjValStr) - 1;index++)
  {
    if ((inputObjValStr[index] EQ '}') || (inputObjValStr[index] EQ '{'))
    {
      flag = FALSE;
      break;
    }
  }
  return flag;
}

/**
 * @param   inputObjValStr is a string contains Object Value string coming from CIH
 * @param   tokens         is an array of strings used to store output tokens after parsing input string
 * @details                This function Parse Incoming ObjVal string as per delimiters
 */

UINT8 cihParseObjValStr(
    IO UINT8     *inputObjValStr,
    IO ObjValStr *tokens
    )
{
  UINT8 *word = NULL;
  UINT8 index = 0;
  UINT8 *delim   = ",{ }";
  UINT8 status = FALSE;

  if (status EQ (CheckObjValStr(inputObjValStr)))
  {
    return TRUE;
  }
  else
  {
    word = strtok(inputObjValStr, delim);
    while(word NOTEQ NULL)
    {
      strcpy(tokens[index++],word);
      word = strtok(NULL,delim);
    }
    return index;
  }
}

/**
 * @param   setCmdstr        is a string contains Object Value str and ObjId string
 * @param   tableAndValueStr is a pointer to an array of strings used to store output tokens after parsing input string
 * @details                  This function Parse Incoming string as per delimiters
 */
void cihParseSetcmd(
    IO UINT8    *setCmdstr,
    IO ObjIdStr *tableAndValueStr
    )
{
  UINT8 *word = NULL;
  UINT8 index = 0;
  UINT8 *delim  = "=";
  word = strtok(setCmdstr, delim);
  while(word NOTEQ NULL)
  {
    strcpy(tableAndValueStr[index++],word);
    word = strtok(NULL,delim);
  }
}

/**
 * @details         Display Table Name, Column Name and Min value, Max value
 *                  and data entered by the user on failure at range check
 */
void printRangeCheckFailHeader()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "%-30s  %-10s   %-10s   %-30s  %-20s  %-8s  %-8s \n","tableName", "rowId", "colId", "colName" , "userEntered" ,"minVal" ,"maxVal");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @details         Display Table Name, Column Name and Min value, Max value
 *                  and data entered by the user on failure at range check
 */
void printAttributeCheckFailHeader()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "%-30s  %-10s   %-10s   %-30s  %-20s %-20s %-20s\n","tableName", "rowId", "colId", "colName" , "userEntered", "expected" , "reason");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @details         Display Table Name, Column Name and data entered by the user on failure.
 */
void printOptionalCheckFailHeader()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "%-30s  %-10s   %-10s   %-30s  %-20s  %-30s\n","tableName", "rowId", "colId", "colName" , "userEntered", "reason");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @details         Display Table Name, Column Name and data entered by the user on failure.
 */
void printExpectedDataCheckFailHeader()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "%-30s  %-10s   %-10s   %-30s  %-20s \n","tableName", "rowId", "colId", "colName" , "userEntered");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @details         Display Table Name, Column Name and data entered by the user on failure.
 */
void printFilenameCheckFailHeader()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "%-30s  %-10s   %-10s   %-30s  %-20s \n","tableName", "rowId", "colId", "colName" , "userEntered");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @details         Display Table Name, Column Name and data entered by the user on failure.
 */
void printSoftwareVersionCheckFailHeader()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "%-30s  %-10s   %-10s   %-30s  %-20s \n","tableName", "rowId", "colId", "colName" , "userEntered");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @details         Display Table Name, Column Name and data entered by the user on failure.
 */
void printTimeCheckFailHeader()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "%-30s  %-10s   %-10s   %-30s  %-20s \n","tableName", "rowId", "colId", "colName" , "userEntered");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}
/**
 * @param   tableInfo Contains Row Id, Column Id and Table Id extracted from user command
 * @param   colValue  Actual value entered by the user
 * @param   cmdRef    is used to display Origin ,Command Reference number and
 *                    Instance Id of command in case of error
 * @details           This function used to print IPv4 Address Check failure
 */
void printExpectedDataCheckFail(
    IN  TagInfo     *tableInfo,
    IO  ObjValStr   *colValue,
    IO  CmdRef      *cmdRef
    )
{
  DisplayBuffer  displayBuffer;
  TableName      *tableName = NULL;
  ColName        *colName = NULL;

  OamTableInfo   *oamTableInfo =  NULL;
  oamTableInfo =  getTablePtrByTableId(tableInfo->tableId);
  if(oamTableInfo EQ NULL)
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%s   \n","Not Able to Find Table Info from Database");
    fputs(displayBuffer, stdout);
    cihLogRsp(displayBuffer);
    return;
  }

  tableName = &oamTableInfo->tableName;
  colName   = &oamTableInfo->colInfo[tableInfo->colId].colName;

  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  cihDisplayCmdRefDeclined(cmdRef);
  printExpectedDataCheckFailHeader();
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "%-30s  %-10d   %-10d   %-29s   %-23s \n",\
      (char*)tableName, tableInfo->rowId, tableInfo->colId, (char*)colName , (char*)colValue);
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param   tableInfo Contains Row Id, Column Id and Table Id extracted from user command
 * @param   minVal    is the minimum possible value
 * @param   maxVal    is the maximum possible value
 * @param   colValue  Actual value entered by the user
 * @param   cmdRef    is used to display Origin ,Command Reference number and
 *                    Instance Id of command in case of error
 * @details           This function used to print range check failure
 */
void printRangeCheckFail(
    IN  TagInfo     *tableInfo,
    IN  UINT32      *minVal,
    IN  UINT32      *maxVal,
    IO  ObjValStr   *colValue,
    IO  CmdRef      *cmdRef
    )
{
  DisplayBuffer  displayBuffer;
  TableName      *tableName = NULL;
  ColName        *colName = NULL;

  OamTableInfo   *oamTableInfo =  NULL;
  oamTableInfo =  getTablePtrByTableId(tableInfo->tableId);
  if(oamTableInfo EQ NULL)
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%s   \n","Not Able to Find Table Info from Database");
    fputs(displayBuffer, stdout);
    cihLogRsp(displayBuffer);
    return;
  }

  tableName = &oamTableInfo->tableName;
  colName   = &oamTableInfo->colInfo[tableInfo->colId].colName;

  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  cihDisplayCmdRefDeclined(cmdRef);
  printRangeCheckFailHeader();
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "%-30s  %-10d   %-10d   %-29s   %-23s  %-8d  %-8d \n",\
      (char*)tableName, tableInfo->rowId, tableInfo->colId, (char*)colName , (char*)colValue ,*minVal ,*maxVal);
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param   tableInfo contains row Id , Column Id and Table Id extracted from user command
 * @param   colId     column ID extracted from user command
 * @param   colValue  Actual value entered by the user
 * @param   cmdRef    is used to display Origin ,Command Reference number and
 *                    Instance Id of command in case of error
 * @details           This function used to print Attribute check failure
 */
void printAttributeCheckFail(
    IN  TagInfo     *tableInfo,
    IN  UINT8        colId,
    IO  ObjValStr   *colValue,
    IO  CmdRef      *cmdRef
    )
{
  DisplayBuffer  displayBuffer;
  TableName      *tableName = NULL;
  ColName        *colName = NULL;
  OamTableInfo   *oamTableInfo =  NULL;

  oamTableInfo =  getTablePtrByTableId(tableInfo->tableId);
  if(oamTableInfo EQ NULL)
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%s   \n","Not Able to Find Table Info from Database");
    fputs(displayBuffer, stdout);
    cihLogRsp(displayBuffer);
    return;
  }
  tableName = &oamTableInfo->tableName;
  colName   = &oamTableInfo->colInfo[colId].colName;

  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  cihDisplayCmdRefDeclined(cmdRef);
  printAttributeCheckFailHeader();
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "%-30s  %-10u   %-10u   %-29s   %-23s  %s  %20s\n",\
      (char*)tableName, tableInfo->rowId, colId, (char*)colName , (char*)colValue , "#" ,"Read Only Parameter");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param   tableInfo contains row Id , Column Id and Table Id extracted from user command
 * @param   colId     column ID extracted from user command
 * @param   colValue  Actual value entered by the user
 * @param   cmdRef    is used to display Origin ,Command Reference number and
 *                    Instance Id of command in case of error
 * @details           This function used to print Optional check failure
 */
void printOptionalCheckFail(
    IN  TagInfo     *tableInfo,
    IN  UINT8        colId,
    IO  ObjValStr   *colValue,
    IO  CmdRef      *cmdRef
    )
{
  DisplayBuffer  displayBuffer;
  TableName      *tableName = NULL;
  ColName        *colName = NULL;
  OamTableInfo   *oamTableInfo =  NULL;

  oamTableInfo =  getTablePtrByTableId(tableInfo->tableId);
  if(oamTableInfo EQ NULL)
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
        "%s   \n","Not Able to Find Table Info from Database");
    fputs(displayBuffer, stdout);
    cihLogRsp(displayBuffer);
    return;
  }
  tableName = &oamTableInfo->tableName;
  colName   = &oamTableInfo->colInfo[colId].colName;

  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  cihDisplayCmdRefDeclined(cmdRef);
  printOptionalCheckFailHeader();
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "%-30s  %-10u   %-10u   %-29s   %-20s  %s\n",\
      (char*)tableName, tableInfo->rowId, colId, (char*)colName , (char*)colValue , "Mandatory Parameter");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param   setCmdTableInfo  Contains Data received from BSM in TLV format for each Object
 * @param   colValue         Actual value entered by the user
 * @param   colId            is column Id extracted from user command
 *
 * @details                  This function is used to perform check on special values
 */
TBOOL cihSpecialValCheck(
    IO  TagInfo   *setCmdTableInfo,
    IO  ObjValStr *colValue,
    IO  UINT8     colId
    )
{
  OamTableType   oamTableType = TABLE_UNKNOWNTYPE;
  TBOOL         *isSpecialValue = NULL;
  SpecialValue  *specialValue = NULL;
  DataType      *dataType = NULL;
  UINT32        specialValue32Bit = 0 , value32Bit = 0;
  UINT16        specialValue16Bit = 0 , value16Bit = 0;
  UINT8         specialValue8Bit = 0 , value8Bit = 0 ;
  SINT32        signedSpecialValue32Bit = 0 , signedValue32Bit = 0;
  SINT16        signedSpecialValue16Bit = 0 , signedValue16Bit = 0;
  SINT8         signedSpecialValue8Bit = 0 , signedValue8Bit = 0 ;

  OamTableInfo   *oamTableInfo =  NULL;
  oamTableInfo =  getTablePtrByTableId(setCmdTableInfo->tableId);
  if(oamTableInfo EQ NULL)
  {
    fprintf(stdout, "%s   \n","Not Able to Find Table Info from Database");
    return FALSE;
  }
  oamTableType = getTableTypeByTableId(setCmdTableInfo->tableId);
  /* Only for Operator Configuration Tables We need to handle special values */
  if(oamTableType EQ TABLE_OPERCONFIG)
  {
    isSpecialValue = &oamTableInfo->colInfo[colId].isSpecialValuePresent;
    if(*isSpecialValue EQ TRUE)
    {
      specialValue = &oamTableInfo->colInfo[colId].specialValue;
      if(specialValue EQ NULL)
      {
        return FALSE; /* This is a safety check ... Not required */
      }

      dataType  = &oamTableInfo->colInfo[colId].dataType;

      if(*dataType EQ BS_TYPE_STRING )
      {
        if(strncmp((char*)colValue , (char*)specialValue , sizeof(SpecialValue) ) EQ 0) /* */
        {
          return TRUE;
        }
        return FALSE;
      }
      else if(*dataType EQ BS_TYPE_IPV4ADDRESS )
      {
        if(strncmp((char*)colValue , (char*)specialValue , sizeof(SpecialValue) ) EQ 0) /* */
        {
          return TRUE;
        }
        return FALSE;
      }
      else if(*dataType EQ BS_TYPE_MACADDRESS)
      {
        if(strncmp((char*)colValue , (char*)specialValue , sizeof(SpecialValue) ) EQ 0) /* */
        {
          return TRUE;
        }
        return FALSE;
      }
      else if(*dataType EQ BS_TYPE_FILENAME )
      {
        if(strncmp((char*)colValue , (char*)specialValue , sizeof(SpecialValue) ) EQ 0) /* */
        {
          return TRUE;
        }
        return FALSE;
      }
      else if(*dataType EQ BS_TYPE_LOCALTIME )
      {
        if(strncmp((char*)colValue , (char*)specialValue , sizeof(SpecialValue) ) EQ 0) /* */
        {
          return TRUE;
        }
        return FALSE;
      }
      else if(*dataType EQ BS_TYPE_OFFSETTIME )
      {
        if(strncmp((char*)colValue , (char*)specialValue , sizeof(SpecialValue) ) EQ 0) /* */
        {
          return TRUE;
        }
        return FALSE;
      }
      else if(*dataType EQ BS_TYPE_SWVERSION )
      {
        if(strncmp((char*)colValue , (char*)specialValue , sizeof(SpecialValue) ) EQ 0) /* */
        {
          return TRUE;
        }
        return FALSE;
      }
      else if(*dataType EQ BS_TYPE_MCC )
      {
        if(strncmp((char*)colValue , (char*)specialValue , sizeof(SpecialValue) ) EQ 0) /* */
        {
          return TRUE;
        }
        return FALSE;
      }
      else if(*dataType EQ BS_TYPE_MNC )
      {
        if(strncmp((char*)colValue , (char*)specialValue , sizeof(SpecialValue) ) EQ 0) /* */
        {
          return TRUE;
        }
        return FALSE;
      }
      else if(*dataType EQ BS_TYPE_LATITUDE )
      {
        if(strncmp((char*)colValue , (char*)specialValue , sizeof(SpecialValue) ) EQ 0) /* */
        {
          return TRUE;
        }
        return FALSE;
      }
      else if(*dataType EQ BS_TYPE_LONGITUDE )
      {
        if(strncmp((char*)colValue , (char*)specialValue , sizeof(SpecialValue) ) EQ 0) /* */
        {
          return TRUE;
        }
        return FALSE;
      }
      else if(*dataType EQ BS_TYPE_ENODEBNAME )
      {
        if(strncmp((char*)colValue , (char*)specialValue , sizeof(SpecialValue) ) EQ 0) /* */
        {
          return TRUE;
        }
        return FALSE;
      }
      else if(*dataType EQ BS_TYPE_PROFILENAME )
      {
        if(strncmp((char*)colValue , (char*)specialValue , sizeof(SpecialValue) ) EQ 0) /* */
        {
          return TRUE;
        }
        return FALSE;
      }
      /* For BOOLEAN MODID and ENUM Data types, There is no special value */
      else if(( *dataType EQ BS_TYPE_BOOL )||(*dataType EQ  BS_TYPE_MODID) ||(*dataType EQ BS_TYPE_ENUM ) ||(*dataType EQ BS_TYPE_MSGID )||(*dataType EQ BS_TYPE_COUNTERID ))
      {
        return FALSE;
      }
      else if (*dataType EQ BS_TYPE_UINT8 )
      {
        value32Bit = atoi(*colValue);
        value8Bit = (UINT8)value32Bit;
        specialValue32Bit = atoi(*specialValue);
        specialValue8Bit = (UINT8)specialValue32Bit;
        if(value8Bit EQ specialValue8Bit )
        {
          return TRUE;  /* If special value return TRUE */
        }
        return FALSE;
      }
      else if (*dataType EQ BS_TYPE_SINT8 )
      {
        sscanf((char*)colValue,"%d",&signedValue32Bit);
        signedValue8Bit = (SINT8)signedValue32Bit;
        sscanf((char*)specialValue,"%d",&signedSpecialValue32Bit);
        signedSpecialValue8Bit = (SINT8)signedSpecialValue32Bit;
        if(signedValue8Bit EQ signedSpecialValue8Bit )
        {
          return TRUE;  /* If special value return TRUE */
        }
        return FALSE;
      }
      else if (*dataType EQ BS_TYPE_UINT16 )
      {
        value32Bit = atoi(*colValue);
        value16Bit = (UINT16)value32Bit;
        specialValue32Bit = atoi(*specialValue);
        specialValue16Bit = (UINT16)specialValue32Bit;
        if(value16Bit EQ specialValue16Bit )
        {
          return TRUE;  /* If special value return TRUE */
        }
        return FALSE;
      }
      else if (*dataType EQ BS_TYPE_SINT16 )
      {
        sscanf((char*)colValue,"%d",&signedValue32Bit);
        signedValue16Bit = (SINT16)signedValue32Bit;
        sscanf((char*)specialValue,"%d",&signedSpecialValue32Bit);
        signedSpecialValue16Bit = (SINT16)signedSpecialValue32Bit;
        if(signedValue16Bit EQ signedSpecialValue16Bit )
        {
          return TRUE;  /* If special value return TRUE */
        }
        return FALSE;
      }
      else if (*dataType EQ BS_TYPE_SINT32 )
      {
        sscanf((char*)colValue,"%d",&signedValue32Bit);
        sscanf((char*)specialValue,"%d",&signedSpecialValue32Bit);
        if(signedValue32Bit EQ signedSpecialValue32Bit )
        {
          return TRUE;  /* If special value return TRUE */
        }
        return FALSE;
      }
      else
      {
        value32Bit = atoi(*colValue);
        specialValue32Bit = atoi(*specialValue);
        if(value32Bit EQ specialValue32Bit )
        {
          return TRUE;  /* If special value return TRUE */
        }
        return FALSE;
      }
    }
  } /*   if(*oamTableType EQ TABLE_OPERCONFIG) */
  return FALSE;
}
/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to find length of string entered by the user
 */
UINT32 stringlen(
    IN UINT8 *colValue
    )
{
  UINT32 index = 0;
  for(index = 0 ; index < MAX_CHARECTERS_IN_USER_CMD ; index ++)
  {
    if(colValue[index] EQ '\0')
    {
      return index;
    }
  }
  logInfo("User string Entered is having Huge Data, No of char received is = [%d]",index);
  return index;
}

/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to check the parameter entered by the user is valid or not.
 */
TBOOL validParameterCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;
  if( 0 EQ regcomp(&pattern, OPTIONALPARAMETERPATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User string entered is INVALID Parameter");
      return FALSE;
    }
  }
  else
  {
    logError("%s","Invalid Parameter pattern in header file is Invalid");
    return FALSE;
  }
}


/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate IPv4 address entered by the user.
 */
TBOOL readOnlyParameterCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;
  if( 0 EQ regcomp(&pattern, READONLYPARAMETERPATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    return FALSE;
  }
  else
  {
    logError("%s","Read Only Parameter pattern in header file is Invalid");
    return FALSE;
  }
}

/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate Optional Parameter entered by the user.
 */
TBOOL optionalParameterCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;
  if( 0 EQ regcomp(&pattern, OPTIONALPARAMETERPATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    return FALSE;
  }
  else
  {
    logError("%s","Optional Parameter pattern in header file is Invalid");
    return FALSE;
  }
}

/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate Signed Integer entered by the user.
 */
TBOOL signedIntegerCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;

  if( 0 EQ regcomp(&pattern, SIGNEDINTEGER, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User string entered is INVALID Signed Integer");
      return FALSE;
    }
  }
  else
  {
    logError("%s","Signed Integer pattern in header file is Invalid");
    return FALSE;
  }
}

/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate Unsigned Integer entered by the user.
 */
TBOOL unsignedIntegerCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;

  if( 0 EQ regcomp(&pattern, UNSIGNEDINTEGER, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User string entered is INVALID Unsigned Integer");
      return FALSE;
    }
  }
  else
  {
    logError("%s","Unsigned Integer pattern in header file is Invalid");
    return FALSE;
  }
}

/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate Unsigned Integer entered by the user.
 */
TBOOL booleanCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;

  if( 0 EQ regcomp(&pattern, BOOLEAN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User string entered is INVALID Boolean value");
      return FALSE;
    }
  }
  else
  {
    logError("%s","Boolean pattern in header file is Invalid");
    return FALSE;
  }
}
/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate IPv4 address entered by the user.
 */
TBOOL ipv4AddressCheck(
    IN UINT8 *colValue
    )
{
  SINT32 firstOctet  = 0;
  SINT32 secondOctet = 0;
  SINT32 thirdOctet  = 0;
  SINT32 fourthOctet = 0;
  regex_t pattern;

  if( 0 EQ regcomp(&pattern, IPV4PATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      if( 4 EQ sscanf(colValue,"%d.%d.%d.%d", &firstOctet, &secondOctet, &thirdOctet, &fourthOctet))
      {
        if(firstOctet EQ 0)
        {
          logError("%s","User string entered is INVALID IPv4 Address");
          return FALSE;
        }
        else if( (firstOctet EQ 255) && (secondOctet EQ 255) EQ (thirdOctet EQ 255) && (fourthOctet EQ 255))
        {
          logError("%s","User string entered is INVALID IPv4 Address");
          return FALSE;
        }
        else if( (firstOctet >=1 && firstOctet <=255) && (secondOctet >=0 && secondOctet <=255) && \
            (thirdOctet >=0 && thirdOctet <=255) && (fourthOctet >=0 && fourthOctet <=255))
        {
          return TRUE;
        }
      }
      else
      {
        logError("%s","Unable to scan IPv4 Address from string entered");
        return FALSE;
      }
    }
    else
    {
      logError("%s","User string entered is INVALID IPv4 Address");
      return FALSE;
    }
  }
  logError("%s","IPv4 Address pattern in header file is Invalid");
  return FALSE;
}

/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate MAC address entered by the user.
 */
TBOOL macAddressCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;

  if( 0 EQ regcomp(&pattern, MACADDRPATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User string entered is INVALID MAC Address");
      return FALSE;
    }
  }
  else
  {
    logError("%s","MAC Address pattern in header file is Invalid");
    return FALSE;
  }
}

/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate filename entered by the user.
 */
TBOOL fileNameCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;

  if( 0 EQ regcomp(&pattern, FILENAMEPATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User string entered is Invalid File name");
      return FALSE;
    }
  }
  else
  {
    logError("%s","File name pattern in header file is Invalid");
    return FALSE;
  }
}

/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate local entered by the user.
 */
TBOOL localTimeCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;

  if( 0 EQ regcomp(&pattern, LOCALTIMEPATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User string entered is INVALID Local Time");
      return FALSE;
    }
  }
  else
  {
    logError("%s","Local Time pattern in header file is Invalid");
    return FALSE;
  }
}

/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate offset time entered by the user.
 */
TBOOL offsetTimeCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;

  if( 0 EQ regcomp(&pattern, OFFSETTIMEPATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User string entered is INVALID Offset Time");
      return FALSE;
    }
  }
  else
  {
    logError("%s","Offset Time pattern in header file is Invalid");
    return FALSE;
  }
}
/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate SW address entered by the user.
 */
TBOOL swVersionCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;
  SINT16  swVersionLength = 0;

  if( 0 EQ regcomp(&pattern, SWVERSIONPATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      swVersionLength = strlen( colValue );
      if(  swVersionLength <= MAX_SIZE_SWACTIVATION_VERSION )
      {
        return TRUE;
      }
      else
      {
        logError("%s","User string entered for software version is INVALID Length");
        return FALSE;
      }
    }
    else
    {
      logError("%s","User string entered is INVALID software version");
      return FALSE;
    }
  }
  else
  {
    logError("%s","Software version pattern in header file is Invalid");
    return FALSE;
  }
}
/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate PLMN MCC entered by the user.
 */
TBOOL mccCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;

  if( 0 EQ regcomp(&pattern, MCCPATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User string entered is INVALID MCC");
      return FALSE;
    }
  }
  else
  {
    logError("%s","MCC pattern in header file is Invalid");
    return FALSE;
  }
}
/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate PLMN MNC entered by the user.
 */
TBOOL mncCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;

  if( 0 EQ regcomp(&pattern, MNCPATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User string entered is INVALID MNC");
      return FALSE;
    }
  }
  else
  {
    logError("%s","MNC pattern in header file is Invalid");
    return FALSE;
  }
}
/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate Latitude entered by the user.
 */
TBOOL latitudeCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;

  if( 0 EQ regcomp(&pattern, LATITUDEPATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User string entered is INVALID Latitude");
      return FALSE;
    }
  }
  else
  {
    logError("%s","Latitude pattern in header file is Invalid");
    return FALSE;
  }
}
/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate Longitude entered by the user.
 */
TBOOL longitudeCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;

  if( 0 EQ regcomp(&pattern, LONGITUDEPATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User string entered is INVALID Longitude");
      return FALSE;
    }
  }
  else
  {
    logError("%s","Longitude pattern in header file is Invalid");
    return FALSE;
  }
}

/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate Enodeb name entered by the user.
 */
TBOOL enodebNameCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;

  if( 0 EQ regcomp(&pattern, ENODEBNAMEPATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User string entered is INVALID Enodeb name");
      return FALSE;
    }
  }
  else
  {
    logError("%s","Enodeb name pattern in header file is Invalid");
    return FALSE;
  }
}

/**
 * @param   colValue  Operator data entered  at prompt as string
 * @details           This function is used to validate Profile name entered by the user.
 */
TBOOL profileNameCheck(
    IN UINT8 *colValue
    )
{
  regex_t pattern;

  if( 0 EQ regcomp(&pattern, PROFILENAMEPATTERN, REG_EXTENDED ))
  {
    if( 0 EQ regexec(&pattern, colValue, 0, NULL, 0))
    {
      return TRUE;
    }
    else
    {
      logError("%s","User string entered is INVALID Profile name");
      return FALSE;
    }
  }
  else
  {
    logError("%s","Profile name pattern in header file is Invalid");
    return FALSE;
  }
}

/**
 * @param  setCmdTableInfo  Contains Data received from BSM in TLV format for each Object
 * @param  colValue         Actual value entered by the user
 * @param  colId            Column Id extracted from user command
 * @param  cmdRef           is used to display Origin ,Command Reference number and
 *                          Instance Id of command in case of error
 * @details                 This function is used to perform range check
 */
TBOOL cihRangeCheck(
    IO  TagInfo   *setCmdTableInfo,
    IO  ObjValStr *colValue,
    IO  UINT8     colId,
    IO  CmdRef    *cmdRef
    )
{
  DataType       *dataType = NULL;
  UINT32         *maxVal = NULL ,*minVal = NULL;
  UINT32         colValueInInteger = 0;
  SINT32         signed32BitValue = 0 ;
  SINT32         *signedMinVal = NULL , *signedMaxVal = NULL;

  UINT8          value8Bit = 0;
  OamTableInfo   *oamTableInfo =  NULL;

  oamTableInfo =  getTablePtrByTableId(setCmdTableInfo->tableId);
  if(oamTableInfo EQ NULL)
  {
    fprintf(stdout, "%s   \n","Not Able to Find Table Info from Database");
    return FALSE;
  }

  dataType  = &oamTableInfo->colInfo[colId].dataType;
  if((*dataType EQ BS_TYPE_SINT8) || (*dataType EQ BS_TYPE_SINT16 ) || (*dataType EQ BS_TYPE_SINT32 )|| (*dataType EQ BS_TYPE_ENUM))
  {
    signedMinVal    = &oamTableInfo->colInfo[colId].colMinVal;
    signedMaxVal    = &oamTableInfo->colInfo[colId].colMaxVal;
  }
  else
  {
    minVal    = &oamTableInfo->colInfo[colId].colMinVal;
    maxVal    = &oamTableInfo->colInfo[colId].colMaxVal;
  }
  /* Checking whether the operator entered valid Integer/Boolean values */
  if((*dataType EQ BS_TYPE_SINT8) || (*dataType EQ BS_TYPE_SINT16 ) || (*dataType EQ BS_TYPE_SINT32 ) || (*dataType EQ BS_TYPE_ENUM))
  {
    if( FALSE EQ signedIntegerCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
  }

  if((*dataType EQ BS_TYPE_UINT8) || (*dataType EQ BS_TYPE_UINT16 ) || (*dataType EQ BS_TYPE_UINT32 ))
  {
    if( FALSE EQ unsignedIntegerCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
  }

  if(*dataType EQ BS_TYPE_BOOL)
  {
    if( FALSE EQ booleanCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
  }

  /* Range Checking */
  if(*dataType EQ BS_TYPE_STRING )
  {
    if((stringlen((UINT8*)colValue) > *maxVal) || ((stringlen((UINT8*)colValue) < *minVal)))
    {
      if(TRUE EQ cihSpecialValCheck(setCmdTableInfo, colValue, colId))
      {
        return TRUE;
      }
      setCmdTableInfo->colId = colId;
      printRangeCheckFail(setCmdTableInfo, minVal , maxVal, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_BOOL )
  {
    colValueInInteger = atoi(*colValue);
    value8Bit = (UINT8)colValueInInteger;
    if((value8Bit NOTEQ 0 ) && (value8Bit NOTEQ 1))
    {
      if(TRUE EQ cihSpecialValCheck(setCmdTableInfo, colValue, colId))
      {
        return TRUE;
      }
      setCmdTableInfo->colId = colId;
      printRangeCheckFail(setCmdTableInfo, minVal , maxVal, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if (*dataType EQ BS_TYPE_SINT8 )
  {
    sscanf((char*)colValue,"%d",&signed32BitValue);
    if((signed32BitValue < *signedMinVal) ||(signed32BitValue > *signedMaxVal))
    {
      if(TRUE EQ cihSpecialValCheck(setCmdTableInfo, colValue, colId))
      {
        return TRUE;
      }
      setCmdTableInfo->colId = colId;
      printRangeCheckFail(setCmdTableInfo, signedMinVal , signedMaxVal, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if ( *dataType EQ BS_TYPE_UINT8 )
  {
    colValueInInteger = atoi(*colValue);
    if((colValueInInteger < *minVal) ||(colValueInInteger > *maxVal))
    {
      if(TRUE EQ cihSpecialValCheck(setCmdTableInfo, colValue, colId))
      {
        return TRUE;
      }
      setCmdTableInfo->colId = colId;
      printRangeCheckFail(setCmdTableInfo, minVal , maxVal, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if (*dataType EQ BS_TYPE_SINT16 )
  {
    sscanf((char*)colValue,"%d",&signed32BitValue);
    if((signed32BitValue < *signedMinVal) ||(signed32BitValue > *signedMaxVal))
    {
      if(TRUE EQ cihSpecialValCheck(setCmdTableInfo, colValue, colId))
      {
        return TRUE;
      }
      setCmdTableInfo->colId = colId;
      printRangeCheckFail(setCmdTableInfo, signedMinVal , signedMaxVal, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if ( *dataType EQ BS_TYPE_UINT16 )
  {
    colValueInInteger = atoi(*colValue);
    if((colValueInInteger < *minVal) ||(colValueInInteger > *maxVal))
    {
      if(TRUE EQ cihSpecialValCheck(setCmdTableInfo, colValue, colId))
      {
        return TRUE;
      }
      setCmdTableInfo->colId = colId;
      printRangeCheckFail(setCmdTableInfo, minVal , maxVal, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_SINT32)
  {
    sscanf((char*)colValue,"%d",&signed32BitValue);
    if((signed32BitValue < *signedMinVal) ||(signed32BitValue > *signedMaxVal))
    {
      if(TRUE EQ cihSpecialValCheck(setCmdTableInfo, colValue, colId))
      {
        return TRUE;
      }
      setCmdTableInfo->colId = colId;
      printRangeCheckFail(setCmdTableInfo, signedMinVal , signedMaxVal, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_UINT32)/* BS_TYPE_UINT32 */
  {
    colValueInInteger = atoi(*colValue);
    if((colValueInInteger < *minVal) ||(colValueInInteger > *maxVal))
    {
      if(TRUE EQ cihSpecialValCheck(setCmdTableInfo, colValue, colId))
      {
        return TRUE;
      }
      setCmdTableInfo->colId = colId;
      printRangeCheckFail(setCmdTableInfo, minVal , maxVal, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_IPV4ADDRESS )
  {
    if( FALSE EQ ipv4AddressCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_MACADDRESS )
  {
    if( FALSE EQ macAddressCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_FILENAME )
  {
    if( FALSE EQ fileNameCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_LOCALTIME )
  {
    if( FALSE EQ localTimeCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_OFFSETTIME )
  {
    if( FALSE EQ offsetTimeCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_SWVERSION )
  {
    if( FALSE EQ swVersionCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_MCC )
  {
    if( FALSE EQ mccCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_MNC )
  {
    if( FALSE EQ mncCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_LATITUDE )
  {
    if( FALSE EQ latitudeCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_LONGITUDE )
  {
    if( FALSE EQ longitudeCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_ENODEBNAME )
  {
    if( FALSE EQ enodebNameCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_PROFILENAME )
  {
    if( FALSE EQ profileNameCheck((UINT8 *)colValue))
    {
      setCmdTableInfo->colId = colId;
      printExpectedDataCheckFail(setCmdTableInfo, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  else if(*dataType EQ BS_TYPE_ENUM )
  {
    sscanf((char*)colValue,"%d",&signed32BitValue);
    if((signed32BitValue < *signedMinVal) ||(signed32BitValue > *signedMaxVal))
    {
      if(TRUE EQ cihSpecialValCheck(setCmdTableInfo, colValue, colId))
      {
        return TRUE;
      }
      setCmdTableInfo->colId = colId;
      printRangeCheckFail(setCmdTableInfo, signedMinVal , signedMaxVal, colValue , cmdRef);
      return FALSE;
    }
    return TRUE;
  }
  logError("%s","Invalid Parameter Type");
  return FALSE;
}

/**
 * @param   colId         Column Id extracted from user command
 * @param   colValue      Actual value entered by the user
 * @param   tlvArray      is array used to fill data in TLV format
 * @param setCmdTableInfo contains table Id, column Id and RowId of set command
 * @param   cmdRef        is used to display Origin ,Command Reference number and
 *                        Instance Id of command in case of error
 * @details               This function used to Fill TLV values in to TLV array
 */
TBOOL fillTlvColInfo(
    IN UINT8     colId,
    IO ObjValStr *colValue,
    IO TlvArray  *tlvArray,
    IO TagInfo   *setCmdTableInfo,
    IO CmdRef    *cmdRef
    )
{
  DataType   *colType;
  Attributes *attributes;
  ParamType  *paramtype;
  UINT16     *colLen;
  TBOOL      result = FALSE;
  UINT8      value8Bit = 0;
  SINT8      signed8Bit = 0;
  UINT16     value16Bit = 0;
  SINT16     signed16Bit = 0;
  UINT32     value32Bit = 0;
  int        tempVal = 0;
  SINT32     signed32Bit = 0;
  OamTableInfo   *oamTableInfo =  NULL;

  oamTableInfo =  getTablePtrByTableId(setCmdTableInfo->tableId);
  if(oamTableInfo EQ NULL)
  {
    fprintf(stdout, "%s   \n","Not Able to Find Table Info from Database");
    return FALSE;
  }


  tlvArray->tlvInfo[tlvArray->numTlvs].tag.tableId  = setCmdTableInfo->tableId;
  tlvArray->tlvInfo[tlvArray->numTlvs].tag.rowId    = setCmdTableInfo->rowId;
  tlvArray->tlvInfo[tlvArray->numTlvs].tag.colId    = colId;

  colLen  = &oamTableInfo->colInfo[colId].length;
  tlvArray->tlvInfo[tlvArray->numTlvs].len = *colLen;
  colType = &oamTableInfo->colInfo[colId].dataType;

  /* Consistency check for not accepting data for RO parameters
   * Expecting "#" for Read only parameters
   * */
  attributes  = &oamTableInfo->colInfo[colId].attributes;
  if(*attributes EQ READ_ONLY)
  {
    if(TRUE EQ readOnlyParameterCheck((UINT8 *)colValue))
    {
      fprintf(stdout,"\n Read Only Parameter for column Id = [%d]  and is Ignored at CIH\n",colId);
      return TRUE;
    }
    else
    {
      printAttributeCheckFail(setCmdTableInfo, colId,  colValue , cmdRef);
      return FALSE;
    }
  }

  /* Consistency check for Optional parameters
   * Expecting "#" for Optional parameters
   * */
  paramtype  = &oamTableInfo->colInfo[colId].paramtype;
  if(*paramtype EQ PARAM_OPTIONAL)
  {
    if(TRUE EQ optionalParameterCheck((UINT8 *)colValue))
    {
      return TRUE;
    }
  }
  else
  {
    if(TRUE EQ optionalParameterCheck((UINT8 *)colValue))
    {
      printOptionalCheckFail(setCmdTableInfo, colId,  colValue , cmdRef);
      return FALSE;
    }
  }

  /* Range check */
  result = cihRangeCheck(setCmdTableInfo, colValue, colId, cmdRef);
  if(result EQ FALSE)
  {
    return FALSE;
  }
  if(*colType EQ BS_TYPE_STRING )
  {
    tlvArray->tlvInfo[tlvArray->numTlvs].len = stringlen((UINT8 *)colValue);
    strncpy(tlvArray->tlvInfo[tlvArray->numTlvs].data,*colValue,tlvArray->tlvInfo[tlvArray->numTlvs].len);
  }
  else if ( *colType EQ BS_TYPE_UINT8 )
  {
    sscanf((char*)colValue,"%d",&tempVal);
    value8Bit = (UINT8)tempVal;
    putUint8InArray( value8Bit, tlvArray->tlvInfo[tlvArray->numTlvs].data );
  }
  else if (*colType EQ BS_TYPE_SINT8 )
  {
    sscanf((char*)colValue,"%d",&tempVal);
    signed8Bit = (SINT8)tempVal;
    putUint8InArray( signed8Bit, tlvArray->tlvInfo[tlvArray->numTlvs].data );
  }
  else if ( *colType EQ BS_TYPE_UINT16 )
  {
    sscanf((char*)colValue,"%d",&tempVal);
    value16Bit = (UINT16)tempVal;
    putUint16InArray( value16Bit, tlvArray->tlvInfo[tlvArray->numTlvs].data );
  }
  else if (*colType EQ BS_TYPE_SINT16 )
  {
    sscanf((char*)colValue,"%d",&tempVal);
    signed16Bit = (SINT16)tempVal;
    putUint16InArray( signed16Bit, tlvArray->tlvInfo[tlvArray->numTlvs].data );
  }
  else if ( *colType EQ BS_TYPE_UINT32 )
  {
    sscanf((char*)colValue,"%d",&value32Bit);
    putUint32InArray( value32Bit, tlvArray->tlvInfo[tlvArray->numTlvs].data );
  }
  else if (*colType EQ BS_TYPE_SINT32 )
  {
    sscanf((char*)colValue,"%d",&signed32Bit);
    putUint32InArray( signed32Bit, tlvArray->tlvInfo[tlvArray->numTlvs].data );
  }
  else if ( *colType EQ BS_TYPE_ENUM )
  {
    sscanf((char*)colValue,"%d",&value32Bit);
    putUint32InArray( value32Bit, tlvArray->tlvInfo[tlvArray->numTlvs].data );
  }
  else if ( *colType EQ BS_TYPE_MODID )
  {
    sscanf((char*)colValue,"%d",&value32Bit);
    putUint32InArray( value32Bit, tlvArray->tlvInfo[tlvArray->numTlvs].data );
  }
  else if ( *colType EQ BS_TYPE_MSGID )
  {
    sscanf((char*)colValue,"%d",&value32Bit);
    putUint32InArray( value32Bit, tlvArray->tlvInfo[tlvArray->numTlvs].data );
  }
  else if ( *colType EQ BS_TYPE_COUNTERID )
  {
    sscanf((char*)colValue,"%d",&value32Bit);
    putUint32InArray( value32Bit, tlvArray->tlvInfo[tlvArray->numTlvs].data );
  }
  else if ( *colType EQ BS_TYPE_BOOL )
  {
    sscanf((char*)colValue,"%d",&tempVal);
    value8Bit = (UINT8)tempVal;
    putUint8InArray( value8Bit, tlvArray->tlvInfo[tlvArray->numTlvs].data );
  }
  else if(*colType EQ BS_TYPE_IPV4ADDRESS )
  {
    tlvArray->tlvInfo[tlvArray->numTlvs].len = stringlen((UINT8 *)colValue);
    strncpy(tlvArray->tlvInfo[tlvArray->numTlvs].data,*colValue,tlvArray->tlvInfo[tlvArray->numTlvs].len);
  }
  else if(*colType EQ BS_TYPE_MACADDRESS )
  {
    tlvArray->tlvInfo[tlvArray->numTlvs].len = stringlen((UINT8 *)colValue);
    strncpy(tlvArray->tlvInfo[tlvArray->numTlvs].data,*colValue,tlvArray->tlvInfo[tlvArray->numTlvs].len);
  }
  else if(*colType EQ BS_TYPE_FILENAME )
  {
    tlvArray->tlvInfo[tlvArray->numTlvs].len = stringlen((UINT8 *)colValue);
    strncpy(tlvArray->tlvInfo[tlvArray->numTlvs].data,*colValue,tlvArray->tlvInfo[tlvArray->numTlvs].len);
  }
  else if(*colType EQ BS_TYPE_LOCALTIME )
  {
    tlvArray->tlvInfo[tlvArray->numTlvs].len = stringlen((UINT8 *)colValue);
    strncpy(tlvArray->tlvInfo[tlvArray->numTlvs].data,*colValue,tlvArray->tlvInfo[tlvArray->numTlvs].len);
  }
  else if(*colType EQ BS_TYPE_OFFSETTIME )
  {
    tlvArray->tlvInfo[tlvArray->numTlvs].len = stringlen((UINT8 *)colValue);
    strncpy(tlvArray->tlvInfo[tlvArray->numTlvs].data,*colValue,tlvArray->tlvInfo[tlvArray->numTlvs].len);
  }
  else if(*colType EQ BS_TYPE_SWVERSION )
  {
    tlvArray->tlvInfo[tlvArray->numTlvs].len = stringlen((UINT8 *)colValue);
    strncpy(tlvArray->tlvInfo[tlvArray->numTlvs].data,*colValue,tlvArray->tlvInfo[tlvArray->numTlvs].len);
  }
  else if(*colType EQ BS_TYPE_MCC )
  {
    tlvArray->tlvInfo[tlvArray->numTlvs].len = stringlen((UINT8 *)colValue);
    strncpy(tlvArray->tlvInfo[tlvArray->numTlvs].data,*colValue,tlvArray->tlvInfo[tlvArray->numTlvs].len);
  }
  else if(*colType EQ BS_TYPE_MNC )
  {
    tlvArray->tlvInfo[tlvArray->numTlvs].len = stringlen((UINT8 *)colValue);
    strncpy(tlvArray->tlvInfo[tlvArray->numTlvs].data,*colValue,tlvArray->tlvInfo[tlvArray->numTlvs].len);
  }
  else if(*colType EQ BS_TYPE_LATITUDE )
  {
    tlvArray->tlvInfo[tlvArray->numTlvs].len = stringlen((UINT8 *)colValue);
    strncpy(tlvArray->tlvInfo[tlvArray->numTlvs].data,*colValue,tlvArray->tlvInfo[tlvArray->numTlvs].len);
  }
  else if(*colType EQ BS_TYPE_LONGITUDE )
  {
    tlvArray->tlvInfo[tlvArray->numTlvs].len = stringlen((UINT8 *)colValue);
    strncpy(tlvArray->tlvInfo[tlvArray->numTlvs].data,*colValue,tlvArray->tlvInfo[tlvArray->numTlvs].len);
  }
  else if(*colType EQ BS_TYPE_ENODEBNAME )
  {
    tlvArray->tlvInfo[tlvArray->numTlvs].len = stringlen((UINT8 *)colValue);
    strncpy(tlvArray->tlvInfo[tlvArray->numTlvs].data,*colValue,tlvArray->tlvInfo[tlvArray->numTlvs].len);
  }
  else if(*colType EQ BS_TYPE_PROFILENAME )
  {
    tlvArray->tlvInfo[tlvArray->numTlvs].len = stringlen((UINT8 *)colValue);
    strncpy(tlvArray->tlvInfo[tlvArray->numTlvs].data,*colValue,tlvArray->tlvInfo[tlvArray->numTlvs].len);
  }
  else
  {
    strncpy(tlvArray->tlvInfo[tlvArray->numTlvs].data,*colValue,tlvArray->tlvInfo[tlvArray->numTlvs].len);
  }
  tlvArray->numTlvs++;
  return TRUE;
}

/**
 * @param   cmdRef is used to display Origin ,Command Reference number and
 *                 Instance Id of command in case of error
 * @details        This function used to Display ObjId Failure
 */
void cihDisplayObjIdFail(
    IO CmdRef   *cmdRef
    )
{
  Buffer   displayBuffer;
  clearBytes(&displayBuffer,sizeof(Buffer));
  cihDisplayCmdRefDeclined(cmdRef);
  snprintf((UINT8*)&displayBuffer, sizeof(Buffer), "Enter a Valid Command. Use \"help\" or \"?\" command for available options\n");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param   cmdRef  is used to display Origin ,Command Reference number and
 *                  Instance Id of command in case of error
 * @details         This function used to Display Set Table Failure
 */
void cihDisplaySetTableFail(
    IO CmdRef   *cmdRef
    )
{
  Buffer   displayBuffer;
  clearBytes(&displayBuffer,sizeof(Buffer));
  cihDisplayCmdRefDeclined(cmdRef);
  snprintf((UINT8*)&displayBuffer, sizeof(Buffer), "Set Table is not permitted\n");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param tableId         is the ID of the Table.
 * @param maxColsInTable  is the maximum no of columns in Table.
 * @param cmdRef          is used to display Origin ,Command Reference number and
 *                        Instance Id of command in case of error.
 *
 * @details               This function used to Display Set Row Failure
 */
void cihDisplaySetRowFail(
    IN TableId  tableId,
    IN UINT32   *maxColsInTable,
    IO CmdRef   *cmdRef
    )
{
  Buffer   displayBuffer;
  TableName  *tableName;
  OamTableInfo   *oamTableInfo =  NULL;

  clearBytes(&displayBuffer,sizeof(Buffer));
  cihDisplayCmdRefDeclined(cmdRef);
  oamTableInfo =  getTablePtrByTableId(tableId);
  if(oamTableInfo NOTEQ NULL)
  {
    tableName = &oamTableInfo->tableName;
    snprintf((UINT8*)&displayBuffer, sizeof(Buffer), \
        "Please enter values for %u columns to set a Row in %s\n",*maxColsInTable,(UINT8*)tableName);
  }
  else
  {
    snprintf((UINT8*)&displayBuffer, sizeof(Buffer), \
        "Please enter values for %u columns to set a Row in %s\n",*maxColsInTable,"UNKNOWN TABLE");
  }
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param   cmdRef is used to display Origin ,Command Reference number and
 *                 Instance Id of command in case of error
 * @details        This function used to Display Set Column Failure
 */
void cihDisplaySetColFail(
    IO CmdRef   *cmdRef
    )
{
  Buffer   displayBuffer;
  clearBytes(&displayBuffer,sizeof(Buffer));
  cihDisplayCmdRefDeclined(cmdRef);
  snprintf((UINT8*)&displayBuffer, sizeof(Buffer), "Please enter values for column to set \n");
  fputs(displayBuffer, stdout);
  cihLogRsp(displayBuffer);
}

/**
 * @param   cmdRef    is the reference number of the command.
 * @param   setCmdStr is the array of tokens after parsing user string.
 * @details           This function is used to Find the action needs to be taken on User command.
 */
TBOOL cihProcessSetCmd(
    IO CmdRef   *cmdRef,
    IO ObjStr   *setCmdStr
    )
{
  UINT8     elementsInObjIdStr = 0,elementsInObjValStr = 0;
  ObjIdStr  tableInfo[MAX_TOKENS_IN_SET_OBJID_STR];
  ObjValStr values[MAX_TOKENS_OBJVAL_STR];
  ObjIdStr  tableAndValueStr[OBJ_IN_SET_CMD];
  SetObjReq setObjReq;
  TagInfo   setCmdTableInfo;
  UINT32    *maxColsInTable =0;
  Index      col = 0;
  EncBuf    encSetObjBuf;
  OamTableInfo   *oamTableInfo =  NULL;
  MsgId     msgId;

  clearBytes(&setObjReq, sizeof(SetObjReq));
  clearBytes(&setCmdTableInfo, sizeof(TagInfo));

  clearBytes(tableInfo, MAX_TOKENS_IN_SET_OBJID_STR * sizeof(ObjIdStr));
  clearBytes(values, MAX_TOKENS_OBJVAL_STR * sizeof(ObjIdStr));
  clearBytes(tableAndValueStr, OBJ_IN_SET_CMD * sizeof(ObjIdStr));

  initEncBuf( &encSetObjBuf );

  /* Process strings received as
   * 1. tableId.rowId={value1,value2...}
   * 2. tableId.rowId.colId=value
   */
  cihParseSetcmd((char*)&setCmdStr[1],tableAndValueStr);
  int len = strlen(tableAndValueStr[1]);
  if (len EQ 0)
  {
      return FALSE;
  }
  elementsInObjIdStr  = cihParseObjIdStr((UINT8*)&tableAndValueStr[0],tableInfo);
  elementsInObjValStr = cihParseObjValStr((UINT8*)&tableAndValueStr[1],values);

  /* Check whether the user entered non-numeric characters for Table, Row, Column Id */
  if( !(checkNumericTableId(tableInfo[0])) || (atoi(tableInfo[0]) > 256))
  {
    return FALSE;
  }
  if( !(checkNumericRowId(tableInfo[1])))
  {
    return FALSE;
  }
  if( !(checkNumericColumnId(tableInfo[2])))
  {
    return FALSE;
  }

  /* tempTableId, tempRowId and tempColId are taken as UINT32 since atoi() returns UINT32
   * If user enters greater than UINT8 value at terminal then wrong value will receive
   * if we directly assign to table Id  */
  setCmdTableInfo.tableId     = atoi(tableInfo[0]);
  setCmdTableInfo.rowId       = atoi(tableInfo[1]);
  setCmdTableInfo.colId       = atoi(tableInfo[2]);

  /* Validates Table Number, RowId and ColId  */
  if(!validateObjId(setCmdTableInfo.tableId,setCmdTableInfo.rowId, setCmdTableInfo.colId))
  {
    return FALSE;
  }

  /* Check Table Type --> For status tables Set operation is not permitted */
  if(!isStatusTable(setCmdTableInfo.tableId, cmdRef))
  {
    return FALSE;
  }
  /* Set Table is not permitted */
  if(setCmdTableInfo.rowId EQ 0 && setCmdTableInfo.colId EQ 0)
  {
    cihDisplaySetTableFail(cmdRef);
    return FALSE;
  }
  else if(setCmdTableInfo.colId EQ 0)
  {
    /* setCmdType = SET_ROW; */
    /* Get Maximum number of columns in the given table */
    oamTableInfo =  getTablePtrByTableId(setCmdTableInfo.tableId);
    if(oamTableInfo EQ NULL)
    {
      cihDisplaySetRowFail(setCmdTableInfo.tableId, maxColsInTable, cmdRef);
      return FALSE;
    }
    maxColsInTable = &oamTableInfo->noOfColumns;
    if(*maxColsInTable NOTEQ elementsInObjValStr)
    {
      cihDisplaySetRowFail(setCmdTableInfo.tableId, maxColsInTable, cmdRef);
      return FALSE;
    }
    for(col = 0 ; col < (SINT32)*maxColsInTable ; col++)
    {
      if(FALSE EQ (fillTlvColInfo(col+1, &values[col], &(setObjReq.tlvArray), &setCmdTableInfo, cmdRef)))
      {
        return FALSE;
      }
    }
  }
  else /* setCmdType = SET_ELEMENT */
  {
    /* Validates ObjValue string */
    if(elementsInObjValStr NOTEQ 1 )
    {
      cihDisplaySetColFail(cmdRef);
      return FALSE;
    }
    if(TRUE EQ validParameterCheck((UINT8 *)values[0]))
    {
      cihDisplaySetColFail(cmdRef);
      return FALSE;
    }
    setObjReq.tlvArray.tlvInfo[0].tag  = setCmdTableInfo;
    /* Encode in set Object Request Message into TLV Buffer */
    if(FALSE EQ (fillTlvColInfo(setCmdTableInfo.colId, &values[0], \
            &(setObjReq.tlvArray), &setCmdTableInfo, cmdRef)))
    {
      return FALSE;
    }
  }
  setObjReq.cmdRef = *cmdRef;
  encSetObjReqMsg(&setObjReq, &encSetObjBuf);
  /* Create and Send Set Request Message to BSM */
  msgId = getCihSetObjReqMsgId();
  createAndSendMsg(msgId, (UINT8*)&encSetObjBuf.value, encSetObjBuf.len);
  addEntryToCmdStatus(cmdRef);
  g_cmdsSent++;
  return TRUE;
}

/**
 * @param   cmdRef     is the reference number of the command.
 * @param   getCmdStr  is the array of tokens after parsing user string
 * @details            This function is used to Find the action needs to be taken on User Get command.
 */
TBOOL cihProcessGetCmd(
    IO CmdRef   *cmdRef,
    IO ObjStr   *getCmdStr
    )
{
  UINT8       elementsInObjIdStr = 0;
  ObjIdStr    tableInfo[MAX_TOKENS_IN_SET_OBJID_STR];
  GetObjReq   getObjReq;
  EncBuf      encGetObjBuf;
  MsgId       msgId;

  clearBytes(tableInfo[0], sizeof(ObjIdStr));
  clearBytes(tableInfo[1], sizeof(ObjIdStr));
  clearBytes(tableInfo[2], sizeof(ObjIdStr));
  clearBytes(&getObjReq, sizeof(GetObjReq));

  initEncBuf( &encGetObjBuf );

  /* Process strings received as
   * 1. get tableId
   * 2. get tableId.rowId
   * 3. get tableId.rowId.colId
   */
  getObjReq.tagArray.numTags = 1; /* One command at a time */
  elementsInObjIdStr = cihParseObjIdStr((UINT8*)&getCmdStr[1],tableInfo);
  getObjReq.cmdRef =  *cmdRef;

  /* Check whether the user entered non-numeric characters  for Table, Row, Column Id  */
  if( !(checkNumericTableId(tableInfo[0])) || (atoi(tableInfo[0]) > 256))
  {
    return FALSE;
  }
  if( !(checkNumericRowId(tableInfo[1])) || (atoi(tableInfo[1]) > 256))
  {
    return FALSE;
  }
  if( !(checkNumericColumnId(tableInfo[2])) || (atoi(tableInfo[2]) > 256))
  {
    return FALSE;
  }

  /* tempTableId, tempRowId and tempColId are taken as UINT32 since atoi() returns UINT32
   * If user enters greater than UINT8 value at terminal then wrong value will receive
   * if we directly assign to table Id  */
  getObjReq.tagArray.tagInfo[0].tableId     = atoi(tableInfo[0]);
  getObjReq.tagArray.tagInfo[0].rowId       = atoi(tableInfo[1]);
  getObjReq.tagArray.tagInfo[0].colId       = atoi(tableInfo[2]);

  /* Validates Table Number, RowId and ColId  */
  if(!validateObjId(getObjReq.tagArray.tagInfo[0].tableId,getObjReq.tagArray.tagInfo[0].rowId, getObjReq.tagArray.tagInfo[0].colId ))
  {
    return FALSE;
  }
  /* Check Table Type --> For Command tables Get operation is not permitted */
  if(!isOprCmdTable(getObjReq.tagArray.tagInfo[0].tableId, cmdRef))
  {
    return FALSE;
  }

  /* Encode in get Object Request Message into TLV Buffer */
  encGetObjReqMsg(&getObjReq, &encGetObjBuf);
  /* Create and Send Get Request Message to BSM */
  msgId = getCihGetObjReqMsgId();
  createAndSendMsg(msgId, (UINT8*)&encGetObjBuf.value, encGetObjBuf.len);
  addEntryToCmdStatus(cmdRef);
  g_cmdsSent++;
  return TRUE;
}


/**
 * @param   noOfObjsInCmd is the number of objects present in user command after parsing.
 * @param   refNum        is the number of command.
 * @param   tokens        is the array of tokens after parsing user string.
 * @details               This function is used to Find the action needs to be taken on User command.
 */
TBOOL cihProcCmdRcvdFromUser(
    IN UINT8    noOfObjsInCmd,
    IN UINT32   refNum,
    IO ObjStr   *tokens
    )
{
  CmdRef   cihRef;
  TBOOL status = FALSE;

  clearBytes(&cihRef,sizeof(CmdRef));
  cihRef.instanceId = getCihInstanceId();
  cihRef.origin     = OAMIF_CIH;
  cihRef.cmdRefNum  = refNum;

  if( (0 EQ strncmp( "help", (UINT8*)tokens[0], sizeof("help"))) | (0 EQ strncmp( "?", (UINT8*)tokens[0], sizeof("?")))) /* First token is Help or ? then we will Display help  */
  {
    status = cihProcHelpCmd( &noOfObjsInCmd, tokens);
    return status;
  }
  if( (0 EQ strncmp( "version", (UINT8*)tokens[0], sizeof("version")))) /* First token is version then we will Display CIH Version  */
  {
    status = cihProcVersionCmd( &noOfObjsInCmd );
    return status;
  }
  if( 0 EQ strncmp( "runscript", (UINT8*)tokens[0], sizeof("runscript")) )
  {
    status = cihProcessRunScriptCmd( noOfObjsInCmd, tokens);
    return status;
  }
  /* Set and Get commands Need Minimum 2 elements after parsing user string
   * Ex : set 1.1.2={5} will be parsed as set and 1.1.2={3} i.e 2 elements in user string
   * Ex : get 1.1       will be parsed as get and 1.1 i.e 2 elements in user string
   */
  if(noOfObjsInCmd NOTEQ MIN_TOKENS_IN_CMD)
  {
    cihDisplayObjIdFail(&cihRef);
    return status;
  }

  if( 0 EQ strncmp( "set", (UINT8*)tokens[0], sizeof("set")) )
  {
    status = cihProcessSetCmd( &cihRef, tokens);
    return status;
  }
  if( 0 EQ strncmp( "get", (UINT8*)tokens[0], sizeof("get")) )
  {
    status = cihProcessGetCmd( &cihRef, tokens);
    return status;
  }
  printf("\nPlease use 'help' to find commands supported in CIH \n");
  return status;
}

/**
 * @param   gl Provide an opaque handle for the resource object that is
 *             defined in getline.h.
 * @details    This is the main function Used to process CIH Commands.
 */
void procCihCommands(
    IO GetLine       *gl
    )
{
  TBOOL  isUserCmdProcessed = FALSE;;
  static unsigned cmdNum = 0;
  ObjStr tokens[MAX_TOKENS_IN_CIH_COMMAND];
  UINT8  noOfObjsInCmd = 0;
  Prompt prompt;
  ObjStr *userCmdStr;
  CihContext *ctx = getGlobalCihContext();

  userCmdStr = getUserCommand();
  clearBytes(userCmdStr, sizeof(ObjStr));

  snprintf(prompt,sizeof("ENB[%d]$ "),"ENB[%d]$ ", cmdNum);
  /* Start or continue editing an input line.   */
  char  *line = gl_get_line(gl, prompt, NULL, 0);
  /* Did the user finish entering a new line?  */
  if(line)
  {
    /*
     * Before writing messages to the terminal, start a new line and
     * switch back to normal terminal I/O.
     */
    gl_normal_io(gl);

    cihLogCommands(line,cmdNum);
    /*
     * Implement a few simple commands.
     */
    if( 0 EQ strncmp( CIH_HISTORY, line, strlen(CIH_HISTORY)))
    {
      gl_show_history(gl, stdout, "%N  %T   %H\n", 0, -1);
    }
    else if( 0 EQ strncmp( CIH_CLEAR, line, strlen(CIH_CLEAR)))
    {
      gl_erase_terminal(gl);
    }
    else if(( 0 EQ strncmp( CIH_EXIT, line, strlen(CIH_EXIT)) )|\
        ( 0 EQ strncmp( CIH_LOGOUT, line, strlen(CIH_LOGOUT)))|\
        ( 0 EQ strncmp( CIH_QUIT_Q, line, strlen(CIH_QUIT_Q)))|\
        ( 0 EQ strncmp( CIH_QUIT_CAPS_Q, line, strlen(CIH_QUIT_CAPS_Q)))|\
        ( 0 EQ strncmp( CIH_QUIT, line, strlen(CIH_QUIT))))   /* Check for exit and logout */
    {
      cihCloseLog();
      exit(0);
    }
    else
    {
      strncpy((char*)userCmdStr, line ,strlen(line));
      /* Parse User string into tokens with delimiters  space and ;
       * Ex : set 2.3.6={4} will be parsed as
       *  set
       *  2.3.6={4}
       * */
      noOfObjsInCmd = cihParseCmdStr(line, tokens);  /* Convert CIH string into tokens */
      if(noOfObjsInCmd EQ 0)
      {
        cmdNum++; 
        printf("Cmds Sent=%d, Responses: Recd=%d, Missing/Expected=%d\n", g_cmdsSent, g_respRecd, (g_cmdsSent - g_respRecd));
        return ;
      }
      isUserCmdProcessed = cihProcCmdRcvdFromUser( noOfObjsInCmd, cmdNum, tokens);  /* Find the action needs to be taken */
      /* Display what was entered.*/
      if( 0 EQ strncmp( (char *)tokens[0], "runscript", strlen("runscript")) )
      {
        if ( isUserCmdProcessed EQ TRUE )
        {
          printf("\nScript executed Successfully. \n");
        }
        ctx->isRunScriptFile = FALSE;
        return;
      }
      if( isUserCmdProcessed EQ TRUE )
      {
        printf("ACCEPTED: You have Entered => %s \n", (char*)userCmdStr);
        cmdNum++;
        return;
      }
      printf("DECLINED: You have Entered => %s \n", (char*)userCmdStr); /* In DECLINED case */
      cmdNum++;
    }
  }
}


/****************************************************************************/
/************************   BATCH FILE PROCESSING   *************************/
/****************************************************************************/

/**
 * @param   inputStr  is user Cmd String taken from file at CIH Interface
 * @param   tokens[]  is the array of tokens after parsing user string
 * @details           This function is used to Parse CIH strings coming as batch file,will parse for \n .
 */
UINT8 cihParseBatchFileForComments(
    IO UINT8  *inputStr,
    IO ObjStr *tokens
    )
{
  UINT8 *word;
  UINT8 index = 0;
  UINT8 *delim   = "\n";

  clearBytes(tokens,sizeof(ObjStr));
  word = strtok(inputStr, delim);
  if(word NOTEQ NULL)
  {
    strcpy(tokens[index++],word);
  }
  word = strtok(NULL, delim);
  while(word NOTEQ NULL)
  {
    strcpy(tokens[index++],word);
    word = strtok(NULL,delim);
  }
  *tokens[index]='\0';
  return index;
}
/**
 * @param   inputStr  Input String
 * @param   tokens[]  is the array of tokens after parsing user string
 * @details           This function is used to Parse CIH strings coming as batch
 *                    i.e with -c option at command line interface and divide it into tokens.
 */
UINT8 cihParseBatchFileString(
    IO UINT8  *inputStr,
    IO ObjStr *tokens
    )
{
  UINT8 *word;
  UINT8 index = 0;
  UINT8 *delim   = ";";

  clearBytes(tokens,sizeof(ObjStr));
  word = strtok(inputStr, delim);
  if(word NOTEQ NULL)
  {
    strcpy(tokens[index++],word);
  }
  word = strtok(NULL, delim);
  while(word NOTEQ NULL)
  {
    if(index EQ MAX_CMDS_IN_BATCH_FILE)
    {
      fprintf(stdout,"\n Maximum Commands in Batch mode should not exceed [%d]", MAX_CMDS_IN_BATCH_FILE );
      exit(EXIT_SUCCESS);
    }
    strcpy(tokens[index++],word);
    word = strtok(NULL,delim);
  }
  *tokens[index]='\0';
  return index;
}
/**
 * @param   noOfCmds     no of commands received from user
 * @param   userCommands Actual commands string
 * @details              This function is used to process user commands in Batch mode.
 */
void procBatchCmds(
    IN UINT8    noOfCmds,
    IO ObjStr   *userCommands
    )
{
  SINT32 tempNoOfArgs = 0;
  TBOOL  isUserCmdProcessed = FALSE;
  Index  noOfObjsInCmd = 0 ;
  ObjStr tokens[MAX_TOKENS_IN_CIH_COMMAND];
  static UINT32 cmdNum = 0;
  ObjStr *userCmdStr;

  struct termios ttyOrig;
  /* Gets the terminals parameters  and stores in ttyOrig structure */
  tcgetattr(STDIN_FILENO, &ttyOrig);

  userCmdStr = getUserCommand();

  clearBytes(tokens,sizeof(ObjStr)* MAX_TOKENS_IN_CIH_COMMAND);

  for(tempNoOfArgs =0 ; tempNoOfArgs < noOfCmds; tempNoOfArgs++  )
  {
    clearBytes(userCmdStr, sizeof(ObjStr));
    strncpy((char*)userCmdStr, userCommands[tempNoOfArgs], strlen(userCommands[tempNoOfArgs]));
    /* Logging CIH commands */
    cihLogCommands((char *)userCmdStr, tempNoOfArgs + 1 );

    /* Check for comments in between commands  */
    if(0 EQ strncmp(&userCommands[tempNoOfArgs][0],"#",1))
    {
      fprintf(stdout,"\nComments In File : %s\n", userCommands[tempNoOfArgs]);
      continue;
    }

    noOfObjsInCmd = cihParseCmdStr(userCommands[tempNoOfArgs], tokens);  /* Convert CIH string into tokens */
    if(noOfObjsInCmd EQ 0)
    {
      continue;
    }
    gl_normal_io(getGlobalCihTeclaContextt());
    fprintf(stdout,"\ncmdEntered[%d] = %s : ", cmdNum, (char*)userCmdStr);
    isUserCmdProcessed = cihProcCmdRcvdFromUser( noOfObjsInCmd, cmdNum, tokens);  /* Find the action needs to be taken */
    if(isUserCmdProcessed EQ FALSE)
    {
      printf("DECLINED\n");
      return;
    }
    printf("ACCEPTED\n");
    gl_abandon_line(getGlobalCihTeclaContextt());
    gl_normal_io(getGlobalCihTeclaContextt());
    gl_raw_io(getGlobalCihTeclaContextt());
    /* Sets the terminals parameters ,when terminals settings getting changed */
    tcsetattr(STDIN_FILENO, TCSANOW, &ttyOrig);
    gl_abandon_line(getGlobalCihTeclaContextt());
    /*Wait for response from BSM */
    if (FALSE EQ checkBsmRsp())
    {
      /* command has timedout. stop processing here. print this info */
      logInfo("%s\n","NO RESPONSE FROM BSM");
      exit(EXIT_FAILURE);
    }
    cmdNum++;
  }
}


/**
 * @param   noOfCmds     no of commands received from user
 * @param   userCommands Actual commands string
 * @details              This function is used to process user commands in Batch mode.
 */
TBOOL procBatchCmdsInCmdLine(
    IN UINT8    noOfCmds,
    IO ObjStr   *userCommands
    )
{
  SINT32 tempNoOfArgs = 0;
  TBOOL  isUserCmdProcessed = FALSE;
  Index  noOfObjsInCmd = 0 ;
  ObjStr tokens[MAX_TOKENS_IN_CIH_COMMAND];
  static UINT32 cmdNum = 0;
  ObjStr *userCmdStr;

  userCmdStr = getUserCommand();

  clearBytes(tokens,sizeof(ObjStr)* MAX_TOKENS_IN_CIH_COMMAND);

  for(tempNoOfArgs =0 ; tempNoOfArgs < noOfCmds; tempNoOfArgs++  )
  {
    clearBytes(userCmdStr, sizeof(ObjStr));
    strncpy((char*)userCmdStr, userCommands[tempNoOfArgs], strlen(userCommands[tempNoOfArgs]));
    /* Logging CIH commands */
    cihLogCommands((char *)userCmdStr, tempNoOfArgs + 1 );

    /* Check for comments in between commands  */
    if(0 EQ strncmp(&userCommands[tempNoOfArgs][0],"#",1))
    {
      fprintf(stdout,"\nComments In File : %s\n", userCommands[tempNoOfArgs]);
      continue;
    }
    noOfObjsInCmd = cihParseCmdStr(userCommands[tempNoOfArgs], tokens);  /* Convert CIH string into tokens */
    if(noOfObjsInCmd EQ 0)
    {
      continue;
    }
    gl_normal_io(getGlobalCihTeclaContextt());
    if(noOfObjsInCmd NOTEQ MIN_TOKENS_IN_CMD)
    {
      return FALSE;
    }
    fprintf(stdout,"\ncmdEntered[%d] = %s : ", cmdNum, (char*)userCmdStr);
    isUserCmdProcessed = cihProcCmdRcvdFromUser( noOfObjsInCmd, cmdNum, tokens);  /* Find the action needs to be taken */

    if(isUserCmdProcessed EQ FALSE)
    {
      printf("DECLINED\n");
      return FALSE;
    }
    printf("ACCEPTED\n");
    gl_abandon_line(getGlobalCihTeclaContextt());
    gl_normal_io(getGlobalCihTeclaContextt());
    gl_raw_io(getGlobalCihTeclaContextt());
    gl_abandon_line(getGlobalCihTeclaContextt());
    /* Wait for response from BSM */
    if (FALSE EQ checkBsmRsp())
    {
      /* command has timedout. stop processing here. print this info */
      logInfo("%s\n","NO RESPONSE FROM BSM");
      return FALSE;
    }
    cmdNum++;
  }
  return TRUE;
}

TBOOL checkBsmRsp()
{   

  UINT16    waited = 0;
  struct timespec duration;
  struct timespec rem_duration;
  duration.tv_sec = CIH_CMD_RSP_SLEEP_DURATION;
  duration.tv_nsec =  0;
  while(waited < MAX_CIH_CMD_RSP_WAIT_TIME)
  {
    if (FALSE EQ cihProcBsmRsp())
    {
      nanosleep(&duration, &rem_duration);
      waited += CIH_CMD_RSP_SLEEP_DURATION;
    }
    else
    {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * @param   filename     contains the file name.
 * @param   userCommands contains user commands.
 * @details This function is used to process commands present in a file given using -f option.
 */

void cihProcessBachFile(
    IN char     *fileName,
    IO ObjStr   *userCommands
    )
{
  Index  noOfObjsInBatchCmd = 0 , noOfObjsInOneLine = 0, i;
  ObjStr commandsInOneLine[MAX_CMDS_IN_ONELINE_IN_FILE];

  CihContext *ctx = getGlobalCihContext();
  ctx->isBatchFile = TRUE;

  /* Open commands File */
  FILE *cmdFile = fopen( fileName, "rt");
  if(!cmdFile)
  {
    fprintf(stdout,"\nFile Does not Exists\n");
    exit(EXIT_FAILURE);
  }
  char buffer[MAX_BUFFER_BATCH_FILE_PROC];
  clearBytes(&buffer[0],(sizeof(UINT8)*MAX_BUFFER_BATCH_FILE_PROC));
  fread(buffer,1,MAX_BUFFER_BATCH_FILE_PROC,cmdFile);
  noOfObjsInBatchCmd = cihParseBatchFileForComments(buffer,userCommands);

  for(i = 0 ; i < noOfObjsInBatchCmd ; i++)
  {
    clearBytes(&commandsInOneLine,(sizeof(ObjStr)*MAX_CMDS_IN_ONELINE_IN_FILE));
    noOfObjsInOneLine = cihParseBatchFileString(userCommands[i],commandsInOneLine);
    procBatchCmds(noOfObjsInOneLine, commandsInOneLine);
  }
  fclose(cmdFile);  /* close the file prior to exiting the routine */
  fprintf(stdout,"\ncmdFile is closed Successfully\n");
  exit(EXIT_SUCCESS);
}

/**
 * @param   filename     contains the file name.
 * @param   userCommands contains user commands.
 * @details This function is used to process commands present in a file given using -c option.
 */

TBOOL procCommandsInCmdLine(
    IO char     *inputStr,
    IO ObjStr   *userCommands
    )
{
  UINT8  noOfObjsInBatchCmd = 0 ;

  CihContext *ctx = getGlobalCihContext();
  ctx->isCmdLineMode = TRUE;

  noOfObjsInBatchCmd = cihParseBatchFileString(inputStr,userCommands);
  return procBatchCmdsInCmdLine(noOfObjsInBatchCmd, userCommands);
}
/**
 * @param   filename     contains the file name.
 * @param   userCommands contains user commands.
 * @details This function is used to process commands present in a file given using runscript command.
 */
TBOOL cihProcessRunScriptFiles(
    IN char     *fileName
    )
{
  ObjStr line[1]; /* USED ONLY HERE */
  /* Open commands File */

  FILE *cmdFile = fopen(fileName, "rt");
  if(!cmdFile)
  {
    return FALSE;
  }
  while(!feof(cmdFile))
  {
    if(NULL != fgets(line[0],sizeof(line[0]),cmdFile))
    {
      procBatchCmds(1,line);
    }
  }
  fclose(cmdFile);  /*close the file prior to exiting the routine */
  return TRUE;
}


/**
 * @param   argc used for finding number of arguments passed
 * @param   argv arguments passed while running CIH module
 * @details      This function is used to parse command line arguments in CIH module.
 */
void cihExecuteCmdsInCmdLine(
    IO SINT32   argc,
    IO char*    argv[]
    )
{
  ObjStr userCommands[MAX_CMDS_IN_BATCH_FILE];
  UINT8  index;

  clearBytes(&userCommands,(sizeof(ObjStr)*MAX_CMDS_IN_BATCH_FILE));
  if ( argc > 3 )
  {
    for(index = 1; index < argc; index++)
    {
      /* Support for Multiple commands at command prompt */
      if (0 EQ strncmp(argv[index], "-c", sizeof("-c")))
      {
        if((argc-1 EQ index) || (!procCommandsInCmdLine(argv[index+1], userCommands)))
        {
          printf("Please enter the commands in double quotes, separated by a semicolon,\nFormat is : ./cih.exe -c \"set 2.3.6={4};get 1.1.2;\" \n");
          exit(EXIT_FAILURE);
        }
        else
        {
          exit(EXIT_SUCCESS);
        }
      }
      /* Support for Batch file */
      else if(0 EQ strncmp(argv[index], "-f", sizeof("-f")))
      {
        if(argc-1 EQ index)
        {
          printf("Please enter the file to process at CIH,\nFormat is : ./cih.exe -f sample.txt \n");
          exit(EXIT_FAILURE);
        }
        cihProcessBachFile(argv[index+1], userCommands);
      }
      else if(0 EQ strncmp(argv[index], "--rootDir", sizeof("-rootDir")))
      {
        if(argc-1 EQ index)
        {
          printf("Please enter the valid root directory path\n");
          exit(EXIT_FAILURE);
        }
        /* skip the immediate argument since it is part of '--rootDir' argument */ 
        index++;
      }
      else if(0 EQ strncmp(argv[index],"-q",sizeof("-q"))) 
      {
        continue;
      }
      else
      {
        printf("\nSupported Options are : 1. --version 2. -c  3. -f 4. -q\n");
        exit(EXIT_FAILURE);
      }
    } 
  }
}

/**
 * @param   noOfObjsInCmd is the number of objects present in user command after parsing.
 * @param   tokens  is the array of tokens after parsing user string
 * @details This function is used to run scripts form CLI command prompt.
 */

TBOOL cihProcessRunScriptCmd(
    IN UINT8    noOfObjsInCmd,
    IO ObjStr   *tokens
    )
{
  TBOOL  status = FALSE;
  CihContext *ctx = getGlobalCihContext();
  /* Checking for filename */
  if( noOfObjsInCmd < 2)
  {
    printf( "Error: Script file missing. \nRefer 'help runscript'.\n" );
    return FALSE;
  }
  ctx->isRunScriptFile = TRUE;
  /* Running the script */
  status = cihProcessRunScriptFiles(tokens[1]);
  if ( FALSE EQ status )
  {
    printf( "Error: Unable to find script file '%s'. \n", tokens[1]);
    return FALSE;
  }
  return TRUE;
}

/**
 * @param   argc number of arguments passed
 * @param   argv arguments passed while running CIH module
 * @details      This function is added quiet command to quit the banner in CIH module.
 */

void cihPrintBanner(
    IO SINT32   argc,
    IO char     *argv[],
    IN VersionInfo *versionInfo
    )
{
  UINT8        index;  

  /* Print CIH Banner if quiet command is not given*/
  if (argc > 3) 
  {
    for(index=1;index < argc;index++) 
    {
      if(strcmp(argv[index], "-q") EQ 0)   
      {
        return ;
      }
    }
  }
  printBanner( versionInfo );
}

/**
 * @param   argc number of arguments passed
 * @param   argv arguments passed while running CIH module
 * @details      This function is added quiet command to quit the banner in CIH module.
 */

void getCihRootDir(
    IO SINT32   argc,
    IO char     *argv[],
    IO FilePath rootDir
    )
{
  UINT8        index = 0 ;
  for(index = 1; (strcmp (argv[index],"--rootDir") NOTEQ 0) && ( index < argc-1); index++) ;
  *rootDir = '\0';
  snprintf( rootDir, sizeof(FileName), "%s", argv[index+1] );
}


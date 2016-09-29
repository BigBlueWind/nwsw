/**
 * @file      cih_help.c
 * @details   Copyright Powerwave Technologies, Inc., 2010
 * @brief     This file contains processing of Help
 *
 * @author    Madhusudana Rao Kata
 * @date      15-07-2010 Basic version
 * @date      27-09-2010 Help is updated
 */

#include "cih.h"

/**
 * @param   attributes   Contains attributes of a column
 * @param   attributeStr Contains  attributes in string format
 * @details              Converts attributes type from ENUM to string
 */
void convertAttributeToStr(
  IN Attributes     *attributes,
  IO AttributeStr   *attributeStr
  )
{
  if(*attributes EQ READ_ONLY )
  {
    snprintf((UINT8*)attributeStr, sizeof(AttributeStr), "%s","READ_ONLY");
  }
  else if(*attributes EQ READ_WRITE )
  {
    snprintf((UINT8*)attributeStr, sizeof(AttributeStr), "%s","READ_WRITE");
  }
  else
  {
    snprintf((UINT8*)attributeStr, sizeof(AttributeStr), "%s","WRITE_WHEN_LOCK_ONLY");
  }
}

/**
 * @param   dataType    Contains Data type of the column
 * @param   dataTypeStr Contains Data type in string format
 * @details             Converts data type from ENUM to string
 */
void convertDataTypeToStr(
  IO DataType       *dataType,
  IO DataTypeStr    *dataTypeStr
  )
{
  if(*dataType EQ BS_TYPE_STRING )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_STRING");
  }
  else if(*dataType EQ BS_TYPE_UINT32 )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_UINT32");
  }
  else if(*dataType EQ BS_TYPE_SINT32 )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_SINT32");
  }
  else if(*dataType EQ BS_TYPE_UINT16 )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_UINT16");
  }
  else if(*dataType EQ BS_TYPE_SINT16 )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_SINT16");
  }
  else if(*dataType EQ BS_TYPE_UINT8 )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_UINT8");
  }
  else if(*dataType EQ BS_TYPE_SINT8 )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_SINT8");
  }
  else if(*dataType EQ BS_TYPE_ENUM )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_ENUM");
  }
  else if(*dataType EQ BS_TYPE_BOOL )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_BOOL");
  }
  else if(*dataType EQ BS_TYPE_IPV4ADDRESS )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_STRING");
  }
  else if(*dataType EQ BS_TYPE_MACADDRESS )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_STRING");
  }
  else if(*dataType EQ BS_TYPE_FILENAME )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_STRING");
  }
  else if(*dataType EQ BS_TYPE_LOCALTIME )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_STRING");
  }
  else if(*dataType EQ BS_TYPE_OFFSETTIME )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_STRING");
  }
  else if(*dataType EQ BS_TYPE_SWVERSION )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_STRING");
  }
  else if(*dataType EQ BS_TYPE_MCC )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_STRING");
  }
  else if(*dataType EQ BS_TYPE_MNC )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_STRING");
  }
  else if(*dataType EQ BS_TYPE_LATITUDE )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_STRING");
  }
  else if(*dataType EQ BS_TYPE_LONGITUDE )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_STRING");
  }
  else if(*dataType EQ BS_TYPE_ENODEBNAME )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_STRING");
  }
  else if(*dataType EQ BS_TYPE_PROFILENAME )
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_STRING");
  }
  else
  {
    snprintf((UINT8*)dataTypeStr, sizeof(DataTypeStr), "%s","TYPE_INT");
  }
}
/**
 * @details          This function is used To Help user by displaying Help for SET command with Example.
 */
void printHelpSetCmd()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
    "*****  Usage of SET Command ******** \n\
      set objId={objVal} \n\
      1. Set an Element of table : \n\
         set TableId.RowId.ColId={ValueToSet} \n\
         EXAMPLE: \n\
         set 1.2.3={4}, \n\
      2. Set a Row of table : \n\
         set TableId.RowId={ValueToColumn1,ValueToColumn2,ValueToColumn3 .... ValueToColumnN} \n\
         EXAMPLE: \n\
         set 1.2={3,4,5,6,7,8} \n\
         \n");
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}


/**
 * @details          This function is used To Help user by displaying Help for GET command with Example.
 */
void printHelpGetCmd()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
    "*****  Usage of GET Command ******** \n\
      get objId \n\
      1. Get an Element of table : \n\
         get TableId.RowId.ColId \n\
         EXAMPLE: \n\
         get 201.1.1 \n\
      2. Get a Row of table : \n\
         get TableId.RowId \n\
         EXAMPLE: \n\
         get 201.1 \n\
      3. Get Table : \n\
         get TableId \n\
         EXAMPLE: \n\
         get 201 \n\
         \n");

  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}

/**
 * @details  This function is used To help user by displaying Help for runscript with Example.
 */
void printHelpRunScriptCmd()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),"\
     Run commands present in a Script file. \n\
     [USAGE]: runscript <filename>\n");
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}


/**
 * @details  This function is used To Help user by displaying Help for Version command with Example.
 */
void printHelpVersionCmd()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),"\
     Get CLI version information in following format: \n\
     [release].[revision].[bugfix].[vendor].[svnversion]\n");
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}

/**
 * @details  This function is used To Help user by displaying Help for Quit command with Example.
 */
void printHelpQuitCmd()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),"\
     Exit from current CLI Session.\n\
     Available options to exit: \n\
     quit \n\
     exit \n\
     logout \n\
     q\n");
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}

/**
 * @details  This function is used To Help user by displaying Help for Clear command with Example.
 */
void printHelpHistoryCmd()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),"\
     Displays list of CLI commands entered.\n\
     *****  Usage of History Command ******** \n\
     history\n");
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}

/**
 * @details  This function is used To Help user by displaying Help for Clear command with Example.
 */
void printHelpClearCmd()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),"\
     Clears the Screen.\n\
     *****  Usage of Clear Command ******** \n\
     clear\n");
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}
/**
 * @details  This function is used To Help user by displaying Help for GET command with Example.
 */
void printCihCmdsHelp()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
    "  Available Options are: \n\
       help tables                - List of available tables \n\
       help <tableID>             - Details of table of tableID \n\
       help <tableID>.<ColumnID>  - Column details of tableID \n\
       help set                   - How to set an Object \n\
       help get                   - How to read a value \n\
       help runscript             - How to run script file \n\
       version                    - Get version Information \n\
       history                    - List of CLI commands entered \n\
       exit/quit/q/Q              - Exit from current CLI Session\n\
       clear                      - Clears the Screen\n");
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}

/**
 * @details   This function is used to get the version of CIH.
 */
void printCihVersion()
{
  CihContext   *ctx = getGlobalCihContext();
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),"%s\n", ctx->versionInfo.version);
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}

/**
 * @details          Display Table Help Header for display purpose
 */
void printLine(
  IO  UINT8         *printBuffer
  )
{
  //UINT8 *p = printBuffer;
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  UINT32 i = 0;
  while (*printBuffer NOTEQ '\0')
  {
    snprintf((UINT8*)&displayBuffer[i], sizeof(DisplayBuffer),\
        "%s","*");
    printBuffer++;
    i++;
  }
  snprintf((UINT8*)&displayBuffer[i], sizeof(DisplayBuffer),\
      "\n");
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
  return ;
}
/**
 * @details          Display Table Help Header for display purpose
 */
void printHelpTablesHeader()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
      "%-9s  %-28s  \n",\
      "TableId", "TableName" );
  printLine((UINT8*)&displayBuffer);
  fputs(displayBuffer, stderr);
  printLine((UINT8*)&displayBuffer);
  cihLogRsp(displayBuffer);
}

/**
 * @details          Display Table Help Header for display purpose
 */
void printTableHelpHeader()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
      "%-9s  %-28s  \n",\
      "TableId", "TableName" );

  fputs(displayBuffer, stderr);
  printLine((UINT8*)&displayBuffer);
  cihLogRsp(displayBuffer);
}

/**
 * @param   tableId Table ID to display Help
 * @details         Display Table Help Requested by the user
 */
void printHelpTables(
  IN  TableId        tableId
  )
{
  DisplayBuffer   displayBuffer;
  TableName       *tableName;
  OamTableInfo    *oamTableInfo =  NULL;

  oamTableInfo =  getTablePtrByTableId(tableId);
  if(oamTableInfo EQ NULL)
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
	          "%s   \n","Not Able to Find Table Info from Database");
    fputs(displayBuffer, stderr);
    cihLogRsp(displayBuffer);
    return;
  }
  /* Some tables defined in OAM sheet with out any valid information  */
  if(oamTableInfo->noOfRows <= 0)
  {
    return;
  }
  tableName = &oamTableInfo->tableName;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
      "  %-9d %-30s  \n",\
      tableId, (char*)tableName);
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}

/**
 * @param   tableId Table ID to display Help
 * @details Display Unknown table Id Help Requested by the user
 */
void printUnknownTableId()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
                  "%s   \n","Not Able to Find Table Info from Database. Invalid Table Id Entered. Refer help.");
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}

/**
 * @param   colInfo Column Information to display Help
 * @details         Display Column Help Requested by the user
 */
void printColumnHelp(
  IO  OamColInfo  *colInfo
  )
{
  DataType       *dataType;
  DataTypeStr    dataTypeStr;
  Attributes     *attributes;
  AttributeStr   attributeStr;
  DisplayBuffer  displayBuffer;

  clearBytes(dataTypeStr,sizeof(DataTypeStr));
  clearBytes(attributeStr,sizeof(AttributeStr));
  clearBytes(displayBuffer,sizeof(DisplayBuffer));

  dataType   = &colInfo->dataType ;
  attributes = &colInfo->attributes ;
  convertDataTypeToStr(dataType, &dataTypeStr);
  convertAttributeToStr(attributes, &attributeStr);

  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
      "  ColumnId            : %-9d\n  ColumnName          : %s\n  DataType            : %-10s\n  MinValue            : %d\n  MaxValue            : %d\n  Attributes          : %-10s\n  Description         : %-200s\n",\
      colInfo->colId,(char*)colInfo->colName, dataTypeStr, colInfo->colMinVal, colInfo->colMaxVal,attributeStr,colInfo->desc );
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}

/**
 * @param   tableId Table ID to display Help
 * @details Display Unknown table Id Help Requested by the user
 */
void printUnknownColumnId()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
                  "%s   \n","Not Able to Find Column Info from Database. Invalid Column Id Entered. Refer help.");
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}

/**
 * @param   tableId Table ID to display Help
 * @details Display help on Row is Invalid
 */
void printRowIdNotAllowed()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
                  "%s   \n","Help on Row is Invalid. Refer 'help' command.");
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}


/**
 * @param   tableId Table ID to display Help
 * @param   cihHelp Table Information to display Help
 * @details         Display Table Help Requested by the user
 */
void printTableHelp(
  IN  TableId        tableId,
  IN  OamTableInfo   *cihHelp
  )
{

  DisplayBuffer   displayBuffer;
  TableName       *tableName;
  OamTableInfo    *oamTableInfo =  NULL;

  oamTableInfo =  getTablePtrByTableId(tableId);
  if(oamTableInfo EQ NULL)
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
	          "%s   \n","Not Able to Find Table Info from Database");
    fputs(displayBuffer, stderr);
    cihLogRsp(displayBuffer);
    return;
  }
  tableName = &oamTableInfo->tableName;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
      "-----------------------------------------------------------------------------------------------------------\n\
  TableId             : %-9d\n  TableName           : %-30s\n  Description         : %-200s\n  MaxRows             : %-9d\n  NoOfColumns         : %-10d\n\
  -----------------------------------------------------------------------------------------------------------\n",\
      tableId,(char*)tableName, cihHelp->desc, cihHelp->noOfRows, cihHelp->noOfColumns );
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}

/**
 * @param tableId Table ID to display Help
 * @param rowId   Row ID to display Help
 * @details       Display Row Help Requested by the user
 */
void printRowHelp(
  IN  TableId        tableId,
  IN  RowId          rowId
  )
{
  DisplayBuffer   displayBuffer;
  TableName       *tableName;
  OamTableInfo    *oamTableInfo =  NULL;

  oamTableInfo =  getTablePtrByTableId(tableId);
  if(oamTableInfo EQ NULL)
  {
    snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), \
	          "%s   \n","Not Able to Find Table Info from Database");
    fputs(displayBuffer, stderr);
    cihLogRsp(displayBuffer);
    return;
  }
  tableName = &oamTableInfo->tableName;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer),\
      "TableName[%s] RowId[%d] \n",\
      (char*)tableName,  rowId);
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}

/**
 * @details          Display Column Help Header for display purpose
 */
void printColHeaderHelp()
{
  DisplayBuffer   displayBuffer;
  clearBytes(displayBuffer,sizeof(DisplayBuffer));
  snprintf((UINT8*)&displayBuffer, sizeof(DisplayBuffer), "%-30s  %-10s   %-10s   %-10s  %-8s  %-8s  %-8s\n","colName", "colId", "Datatype" ,"minVal" ,"maxVal", "attributes","description");
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}

/**
 * @details          Display OAM Table Help Header for display purpose
 */
void printOamHelpHeader()
{
  String100   displayBuffer;
  clearBytes(displayBuffer,sizeof(String100));
  snprintf((UINT8*)&displayBuffer, sizeof(String100),\
      "Available tables with Table Ids are as follows\n" );
  fputs(displayBuffer, stderr);
  cihLogRsp(displayBuffer);
}

/**
 * @details          This function is used To Help user by displaying all Tables available at CIH .
 */
void cihDisplayTablesInfo()
{
  Index tableIndex = 0;

  printOamHelpHeader();
  printHelpTablesHeader();

  /* Operator Commands */
  for (tableIndex = MIN_OPR_CMD_TABLEID ; tableIndex <= MAX_OPR_CMD_TABLEID ; tableIndex ++)
  {
    printHelpTables(tableIndex);
  }
  /* Operator Config */
  for (tableIndex = MIN_OPR_CFG_TABLEID ; tableIndex <= MAX_OPR_CFG_TABLEID ; tableIndex ++)
  {
    printHelpTables(tableIndex);
  }
  /* Status */
  for (tableIndex = MIN_STATUS_TABLEID ; tableIndex <= MAX_STATUS_TABLEID ; tableIndex ++)
  {
    printHelpTables(tableIndex);
  }
  /* Pre Config */
  for (tableIndex = MIN_PRE_CFG_TABLEID ; tableIndex <= MAX_PRE_CFG_TABLEID ; tableIndex ++)
  {
    printHelpTables(tableIndex);
  }

  printf("\n");
}

/**
 * @param   noOfObjsInCmd Number of Objects in Command received from User.
 * @param   cmdRef        is used to display Origin ,Command Reference number and
 *                        Instance Id of command in case of error
 * @param   helpCmdStr    is a string contains Object ID
 *                        i.e tableId.RowId.Element format Ex :1.2.3 or set, get
 * @details               This function is used to Process Help command.
 */
TBOOL cihProcHelpCmd(
  IN UINT8     *noOfObjsInCmd,
  IO ObjStr    *helpCmdStr
  )
{
  OamTableInfo     *cihHelp = NULL;
  OamColInfo       *colInfo = NULL;
  UINT8            elementsInObjIdStr;
  TagInfo          tagInfo;
  ObjIdStr         tableInfo[MAX_TOKENS_IN_SET_OBJID_STR]; /* To store TableId,ColId and RowId in string format after parsing  */
  Index            *noOfCols,colIndex;

  clearBytes(&tagInfo, sizeof(TagInfo));
  clearBytes(&tableInfo, sizeof(ObjIdStr)*MAX_TOKENS_IN_SET_OBJID_STR);

  if(*noOfObjsInCmd <= 1)
  {
    printCihCmdsHelp();
    return TRUE;
  }
  if( 0 EQ strncmp( "tables", (UINT8*)&helpCmdStr[1], sizeof("tables")) )
  {
    cihDisplayTablesInfo();
    return TRUE;
  }
  else if( 0 EQ strncmp( "set", (UINT8*)&helpCmdStr[1], sizeof("set")) )
  {
    printHelpSetCmd();
    return TRUE;
  }
  else if( 0 EQ strncmp( "get", (UINT8*)&helpCmdStr[1], sizeof("get")) )
  {
    printHelpGetCmd();
    return TRUE;
  }
  else if( 0 EQ strncmp( "runscript", (UINT8*)&helpCmdStr[1], sizeof("runscript")) )
  {
	printHelpRunScriptCmd();
	return TRUE;
  }
  else if( 0 EQ strncmp( "version", (UINT8*)&helpCmdStr[1], sizeof("version")) )
  {
    printHelpVersionCmd();
    return TRUE;
  }
  else if( 0 EQ strncmp( "quit", (UINT8*)&helpCmdStr[1], sizeof("quit")) )
  {
    printHelpQuitCmd();
    return TRUE;
  }
  else if( 0 EQ strncmp( "exit", (UINT8*)&helpCmdStr[1], sizeof("exit")) )
  {
    printHelpQuitCmd();
    return TRUE;
  }
  else if( 0 EQ strncmp( "clear", (UINT8*)&helpCmdStr[1], sizeof("clear")) )
  {
    printHelpClearCmd();
    return TRUE;
  }
  else if( 0 EQ strncmp( "history", (UINT8*)&helpCmdStr[1], sizeof("history")) )
  {
    printHelpHistoryCmd();
    return TRUE;
  }
  else
  {
    /* Parse ObjId string */
    elementsInObjIdStr = cihParseObjIdStr((UINT8*)&helpCmdStr[1],tableInfo);

    /* There is no help on Row */
    if ( elementsInObjIdStr > 2 )
    {
      printRowIdNotAllowed();
      return FALSE;
    }

    /* Check whether the user entered non-numeric characters for Table, Row, Column Id */
    if( !(checkNumericTableId(tableInfo[0]))  || (atoi(tableInfo[0]) > 256))
    {
      return FALSE;
    }
    if( !(checkNumericColumnId(tableInfo[1])))
    {
      return FALSE;
    }

    /* tempTableId, tempRowId and tempColId are taken as UINT32 since atoi() returns UINT32
     * If user enters greater than UINT8 value at terminal then wrong value will receive
     * if we directly assign to table Id  */
    tagInfo.tableId     = atoi(tableInfo[0]);
    tagInfo.colId       = atoi(tableInfo[1]);/* Second element entered by the user is column Id */

    /* Validates Table Number, RowId and ColId  */
    if(!(validateTableId(tagInfo.tableId)))
    {
      return FALSE;
    }
    /* Check whether the user entered column id */
    if ( elementsInObjIdStr EQ 2)
    {
      if( 0 NOTEQ tagInfo.colId )
      {
        if(!(validateColId(tagInfo.tableId, tagInfo.colId)))
        {
          return FALSE;
        }
      }
      else
      {
        return FALSE;
      }
    }
    cihHelp = getTablePtrByTableId(tagInfo.tableId);
    if(cihHelp EQ NULL)
    {
      printUnknownTableId();
      return FALSE;
    }

    /* Help on Table  */
    if( tagInfo.colId EQ 0 )
    {
      printTableHelp(tagInfo.tableId, cihHelp);
      noOfCols = &cihHelp->noOfColumns;
      for(colIndex = 1; colIndex <= *noOfCols ; colIndex++)
      {
        tagInfo.colId = colIndex;
        colInfo = &cihHelp->colInfo[tagInfo.colId];
        printColumnHelp(colInfo);
      }
    }
    /* Help on Element
     * command from user is $ help <table_Num>.<col_Number>
     * There is no help on Row
     * */
    else /* Help on Element */
    {
      colInfo = &cihHelp->colInfo[tagInfo.colId]; /* Here rowId is actual element Id */
      printColumnHelp(colInfo);
    }
    return TRUE;
  }
}

/**
 * @param    noOfObjsInCmd Number of Objects in Command received from User. *
 * @details  This function is used to Process Version command.
 */
TBOOL cihProcVersionCmd(
  IN UINT8     *noOfObjsInCmd
  )
{
  if(*noOfObjsInCmd <= 1)
  {
    printCihVersion();
    return TRUE;
  }
  return FALSE;
}






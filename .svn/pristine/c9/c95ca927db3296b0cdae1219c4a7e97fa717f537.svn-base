/**
 * @file    oam_cfg_table.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains processing of OAM tables information
 *
 * @author     : Madhusudana Rao Kata
 * Date       : 15-07-2010
 * Description: 1.Basic version
 */

#include <libconfig.h>
#include "all_cfg.h"

#define MAX_TABLE_NAME 50
#define MAX_COL_NAME   50

/**
 * @param   cfg   LibConfig parameters
 * @param   tableNum represents table number of OAM tables
 * @oaram   oamTableInfo global context information
 * @details This function reads information from configuration file for
 *          OAM tables columns information and fills global context.
 */
void readCol(
  IN config_t      *cfg,
  IN UINT32        *tableNum,         /* Temporary  table number for reading from tpl file */
  IO OamColInfo    *oamColInfo
  )
{
  char             colInfo[MAX_COL_NAME];
  config_setting_t *setting = NULL;

  /* Reading Column Info */
  snprintf(colInfo, MAX_COL_NAME, "oamTableConfig.table%dcolCfg",*tableNum);
  setting = config_lookup(cfg, colInfo);
  if(setting NOTEQ NULL)
  {
   unsigned int count = config_setting_length(setting);
   unsigned int i;

   for(i = 0; i < count; ++i)
   {
     config_setting_t *colInfoSett = config_setting_get_elem(setting, i);
     /* Only output the record if all of the expected fields are present. */
     const char *colName, *dataType ,*desc, *attributes, *specialValue, *persistent, *optionalValue;
     int colId,length,colDataType,colMinVal,colMaxVal,attributeEnum,isSpecialValue, persistentEnum, optionalValueEnum;

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
        && config_setting_lookup_string(colInfoSett, "specialValue", &specialValue)
        && config_setting_lookup_string(colInfoSett, "optionalValue", &optionalValue)))
       continue;

     strncpy(oamColInfo[colId].colName,colName,sizeof(ColName));
     oamColInfo[colId].colId = colId;
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
     else if (strcmp(dataType, "CounterId") EQ 0)
     {
       colDataType = BS_TYPE_COUNTERID;
     }
     else
     {
       colDataType = BS_TYPE_UINT32;
     }

     oamColInfo[colId].dataType = colDataType;
     oamColInfo[colId].length = length;
     oamColInfo[colId].colMinVal = colMinVal;
     oamColInfo[colId].colMaxVal = colMaxVal;
     strncpy(oamColInfo[colId].desc,desc,sizeof(Description));

     if (strcmp(attributes, "RW") EQ 0)
     {
       attributeEnum = READ_WRITE;
     }
     else if (strcmp(attributes, "RO") EQ 0)
     {
       attributeEnum = READ_ONLY;
     }
     else if (strcmp(attributes, "RW-LO") EQ 0)
     {
       attributeEnum = WRITE_WHEN_LOCK_ONLY;
     }
     else
     {
       attributeEnum = READ_ONLY;
     }
     oamColInfo[colId].attributes = attributeEnum;

     if( strcmp(optionalValue, "O") EQ 0)
     {
    	 optionalValueEnum = PARAM_OPTIONAL;
     }
     else if(strcmp(optionalValue, "M") EQ 0)
     {
    	 optionalValueEnum = PARAM_MANDATORY;
     }
     oamColInfo[colId].paramtype = optionalValueEnum;


     if (strcmp(persistent, "P") EQ 0)
     {
       persistentEnum = PERSISTENT;
     }
     else if (strcmp(persistent, "NP") EQ 0)
     {
       persistentEnum = NON_PERSISTENT;
     }
     else
     {
       persistentEnum = NON_PERSISTENT;
     }
     oamColInfo[colId].persistent = persistentEnum;
     oamColInfo[colId].isSpecialValuePresent = isSpecialValue;
     strncpy(oamColInfo[colId].specialValue, specialValue, sizeof(SpecialValue));
   }
  }
}

/**
 * @param   cfg   Lib Configuration parameters
 * @param   tableNum represents table number of OAM tables
 * @details This function reads information from configuration file and fills global context.
 */
void readTable(
  IN config_t        *cfg,
  IN UINT32          *tableNum,
  IO OamTableContext *oamCtx
  )
{
  char             tableInfo[MAX_TABLE_NAME];
  config_setting_t *setting = NULL ;
  OamTableInfo     *oamTableInfo = NULL ;

  snprintf(tableInfo,MAX_TABLE_NAME, "oamTableConfig.table%dCfg",*tableNum);
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
        logError("wrong table type [%s]. Please check oamConfig.tpl", tableType);
      }

      clearBytes(oamTableInfo,sizeof(OamTableInfo));

      /* Fill GLobal context with OAM table data */
      oamTableInfo->noOfColumns  = noOfColumns;
      strncpy(oamTableInfo->tableName, tableName, sizeof(TableName));
      strncpy(oamTableInfo->desc, desc , sizeof(Description));
      oamTableInfo->noOfRows     = maxRows;
      oamTableInfo->oamTableType = tableEnumType;

      readCol(cfg, tableNum, &oamTableInfo->colInfo[0]);
    }
  }
}


/**
 * @param   configFilePath represents configuration  path
 * This reads the configuration file 'oamTable.cfg' and displays
 * some of its contents.
 */
int parseOamCfg(
  IN UINT8*          configFilePath,
  IO OamTableContext *oamCtx
  )
{
  config_t   cfg;
  UINT8      fullFilePath[MAX_BUFFER_SIZE];
  const char *str ;
  int        noOfTables,tableIndex;
  UINT32     *noOfValidTables = NULL ;

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
    readTable(&cfg,&tableIndex,oamCtx);
  }
  /* Operator Config */
  for (tableIndex = MIN_OPR_CFG_TABLEID ; tableIndex <= MAX_OPR_CFG_TABLEID ; tableIndex ++)
  {
    readTable(&cfg,&tableIndex,oamCtx);
  }
  /* Status */
  for (tableIndex = MIN_STATUS_TABLEID ; tableIndex <= MAX_STATUS_TABLEID ; tableIndex ++)
  {
    readTable(&cfg,&tableIndex,oamCtx);
  }
  /* Pre Config */
  for (tableIndex = MIN_PRE_CFG_TABLEID ; tableIndex <= MAX_PRE_CFG_TABLEID ; tableIndex ++)
  {
    readTable(&cfg,&tableIndex,oamCtx);
  }

  config_destroy(&cfg);
  return(EXIT_SUCCESS);
}

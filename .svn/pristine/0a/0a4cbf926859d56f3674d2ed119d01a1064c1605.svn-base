/**
 * @file    prm_read_write_cfg.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains functions which will read and write config files.
 *
 * @author          : Shweta Polepally
 * Creation Date   : Dec 2010
 * Change History  :
 *
 */

#include "prm.h"


/* @param     fileToRead : This is the full file path of the configuration file which will be read
 * @param     cfgTable   : Output structure.
 * @details   This function will read Version Information and return the filled structure.
 *            If error, will report and exit from the function.
 * */
NumEntry prmReadFromFileMappingCfgTable(
  IN UINT8                        *fileToRead,
  IO ProcessModuleMappingEntry    *cfgTable,
  IO UINT8                        *error
  )
{
  config_t         cfg;
   config_setting_t *setting;

   config_init(&cfg); /* init LibConfig */

   /* Read the file. If there is an error, report it and exit. */
    if(! config_read_file(&cfg, fileToRead))
    {
      fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
              config_error_line(&cfg), config_error_text(&cfg));
      config_destroy(&cfg);
      printf("\n Please enter valid Path for process Configuration file \n");
      *error = TRUE;
      return FALSE;
    }

    setting = config_lookup(&cfg, "mapping_table");
    if(setting NOTEQ NULL)
    {
      int count = config_setting_length(setting);
      int        i;
      UINT32     modIdIndex = 0;
      NumEntry   modules = 0;

      for(i = 0; i < count; ++i)
      {
        config_setting_t *table = config_setting_get_elem(setting, i);
        /* Only output the record if all of the expected fields are present. */
        const char *moduleName = NULL;
        const char *exeName = NULL;
        config_setting_lookup_string(table, "moduleName", &moduleName );
        config_setting_lookup_string(table, "exeName", &exeName );
        for(modIdIndex = 0 ; modIdIndex < MODID_MAX ; modIdIndex++)
        {
          /* TODO: can be optimised: read ModuleId as number rather than string */
          if(strcmp(getModuleName(modIdIndex), moduleName) EQ 0)
          {
            cfgTable->moduleName  = modIdIndex;
            strncpy( cfgTable->exeName, exeName, strlen(exeName) );
            /* Pid is not read from the configuration file, although entry is there  */
            modules++;
            break;
          }
        }
        cfgTable++;
      }
      config_destroy(&cfg);
      return modules;
    }
    config_destroy(&cfg);
    return FALSE;
}

/**
 * @param   cfg      Lib Configuration parameters
 * @param   tableNum represents table number of OAM tables
 * @param   oamCtx   OAM cofig template context
 * @details This reads each table and its column information from oamConfig.tpl file
 */
static void prmReadOprCfgTableEntry(
  IN config_t           *cfg,
  IN UINT32             *tableNum,
  IO PrmOamTableInfo    *oamCtx
  )
{
  char                tableInfo[MAX_TABLE_NAME];
  config_setting_t    *setting = NULL ;
  PrmOamTableInfo     *oamTableInfo = NULL ;

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
      const char *tableName, *tableType;
      int tempTableNumber ,maxRows,noOfColumns;
      int actualTableNumber;

      if(!(config_setting_lookup_string(tableInfo, "tableName", &tableName)
          && config_setting_lookup_int(tableInfo, "noOfColumns", &noOfColumns)
         && config_setting_lookup_int(tableInfo, "tableNumber", &tempTableNumber)
         && config_setting_lookup_int(tableInfo, "maxRows", &maxRows)
         && config_setting_lookup_string(tableInfo, "tableType",&tableType)))
        continue;

      actualTableNumber =  tempTableNumber - OPR_CFG_TABLES_OFFSET;

      if (strcmp(tableType, "OprConfig") EQ 0)
      {
        oamTableInfo = &oamCtx[actualTableNumber];
      }
      else
      {
        logError("wrong table type [%s]. Please check oamConfig.tpl", tableType);
      }

      clearBytes(oamTableInfo,sizeof(PrmOamTableInfo));

      /* Fill GLobal context with OAM table data */
      oamTableInfo->noOfColumns  = noOfColumns;
      oamTableInfo->noOfRows     = maxRows;
      strncpy(oamTableInfo->tableName, tableName, sizeof(TableName));
      readCol(cfg, tableNum, &oamTableInfo->colInfo[0]);
    }
  }
}

/**
 * @param   configFilePath   represents configuration  path
 * @param   oamCfgTableInfo  OAM cofig template context
 * This reads the configuration file 'oamTable.cfg' and displays
 * some of its contents.
 */
TBOOL prmParseOamOprCfgTables(
  IN UINT8*              configFilePath,
  IO PrmOamTableInfo    *oamCfgTableInfo
  )
{
  config_t   cfg;
  UINT8      fullFilePath[MAX_BUFFER_SIZE];
  int        tableIndex;

  config_init(&cfg);
  constructFullPath ( configFilePath, "oamConfig.tpl", fullFilePath );

  /* Read the file. If there is an error, report it and exit. */
  if(!config_read_file(&cfg, fullFilePath))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    return FALSE;
  }

  /* Operator Config */
  for (tableIndex = MIN_OPR_CFG_TABLEID ; tableIndex <= MAX_OPR_CFG_TABLEID ; tableIndex ++)
  {
    prmReadOprCfgTableEntry(&cfg,&tableIndex,oamCfgTableInfo);
  }

  config_destroy(&cfg);
  return TRUE;
}

/**
 * @details This function will read the processStartTable Configuration file
 **/
TBOOL prmReadProcessStartTable(
  void
  )
{
  UINT8       isError = FALSE;
  UINT8       fullFilePath[MAX_BUFFER_SIZE];

  PrmContext  *ctx = getPrmContext();

  GenericTable      *tableFunc    = getTableFunctions(PROCESSSTART);
  ProcessStart      *rowOfTable   = (ProcessStart*)getFirstRowOfTable(PROCESSSTART);

  /* Read the default configurations from the processStartTable.cfg file and store it in
   * the prm context config table */
  memset( &fullFilePath, 0, sizeof(fullFilePath));
  constructFullPath ( &ctx->cfgFilePath[0], "process_start_table.cfg", fullFilePath );

  if ( NULL EQ tableFunc->getReadFromFileFunc )
  {
    BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("PROCESSSTART");
    return FALSE;
  }
  else
  {
    ctx->numOfProcesses = tableFunc->getReadFromFileFunc( fullFilePath, rowOfTable, &isError );
    if ( TRUE EQ isError )
    {
      BS_ERROR_CONFIG_READ_FAILED()
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * @details This function will read the moduleStopTable Configuration file
 **/
TBOOL prmReadModuleStopTable(
  void
  )
{
  UINT8       isError = FALSE;
  UINT8       fullFilePath[MAX_BUFFER_SIZE];

  GenericTable      *tableFunc             = getTableFunctions(MODULESTOP);
  ModuleStop        *rowOfModuleStopTable  = (ModuleStop*)getFirstRowOfTable(MODULESTOP);

  PrmContext  *ctx = getPrmContext();

  /* Read the default configurations from the processStartTable.cfg file and store it in
   * the prm context config table */
  memset( &fullFilePath, 0, sizeof(fullFilePath));
  constructFullPath ( &ctx->cfgFilePath[0], "module_stop_table.cfg", fullFilePath );

  if ( NULL EQ tableFunc->getReadFromFileFunc )
  {
    BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("MODULESTOP");
    return FALSE;
  }
  else
  {
    ctx->numOfModules = tableFunc->getReadFromFileFunc( fullFilePath, rowOfModuleStopTable, &isError );
    if ( TRUE EQ isError )
    {
      BS_ERROR_CONFIG_READ_FAILED()
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * @details This function will read the process module mapping Configuration file
 **/
TBOOL prmReadProcessModuleMappingTable(
  void
  )
{
  UINT8       isError = FALSE;
  UINT8       fullFilePath[MAX_BUFFER_SIZE];

  clearBytes( &fullFilePath, sizeof(fullFilePath));
  PrmContext  *ctx = getPrmContext();

  /* Read the default configurations from the processStartTable.cfg file and store it in
   * the prm context config table */
  memset( &fullFilePath, 0, sizeof(fullFilePath));
  constructFullPath ( &ctx->cfgFilePath[0], "mapping_table.cfg", fullFilePath );

  ctx->mappingTable.numOfMappedModules = prmReadFromFileMappingCfgTable( fullFilePath, &ctx->mappingTable.entry[0], &isError );
  if ( TRUE EQ isError )
  {
    BS_ERROR_CONFIG_READ_FAILED();
    return FALSE;
  }
  return TRUE;
}

/**
 * @param confiigDir  Config Directory path
 * @details This function will read the swInfoStatusTable Configuration file
 **/
void prmReadSwPackageStatusTable(
  IN UINT8   *configDir
  )
{
  UINT8       isError = FALSE;
  FilePath    fullFilePath;

  GenericTable      *tableFunc = getTableFunctions(SWPACKAGESTATUS);
  SwPackageStatus   *rowOfSwPackageTable = (SwPackageStatus*)getFirstRowOfTable(SWPACKAGESTATUS);

  /* Read the default configurations from the processStartTable.cfg file and store it in
   * the prm context config table */
  snprintf( (UINT8*)&fullFilePath, sizeof(FilePath), "%s/sw_package.cfg", configDir );

  if ( NULL EQ tableFunc->getReadFromFileFunc )
  {
    BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("SWPACKAGESTATUS");
    exit(EXIT_FAILURE);
  }
  else
  {
    tableFunc->getReadFromFileFunc( fullFilePath, rowOfSwPackageTable, &isError );
    if ( TRUE EQ isError )
    {
      BS_ERROR_CONFIG_READ_FAILED()
      exit(EXIT_FAILURE);
    }
  }
}

/**
 * @details This functions will write swPackageStatus to persistent memory.
 */
void prmWriteSwPackageStatusToPersistentMemory(
  void
  )
{
  PrmContext   *prmContext = getPrmContext();
  UINT8 isError = FALSE;
  UINT8 fullFilePath[MAX_BUFFER_SIZE];
  clearBytes( &fullFilePath, sizeof(fullFilePath));

  /* Write sw_package information to the persistent memory */
  GenericTable       *tableFunc = getTableFunctions(SWPACKAGESTATUS);
  SwPackageStatus    *config    = (SwPackageStatus*)getFirstRowOfTable(SWPACKAGESTATUS);

  snprintf( fullFilePath, MAX_BUFFER_SIZE, "%s/sw_package.cfg", &prmContext->prmInfo.configDir[0] );
  if ( NULL EQ tableFunc->getWriteToFileFunc )
  {
    BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED(" SWPACKAGESTATUS ");
  }
  else
  {
    tableFunc->getWriteToFileFunc( config, fullFilePath, &isError, NULL );
    if ( TRUE EQ isError )
    {
      BS_ERROR_CONFIG_WRITE_FAILED( );
    }
  }
}

/**
 * @details This function will read the bs_timers Configuration file
 **/
TBOOL readPrmTimersFromCfg(void)
{
  UINT8       isError = FALSE;
  UINT8       fullFilePath[MAX_BUFFER_SIZE];
  PrmContext  *ctx = getPrmContext();
  GenericTable  *tableFunc       = getTableFunctions(TIMERSCONFIG);
  TimersConfig  *rowOfTimerTable = (TimersConfig*)getFirstRowOfTable(TIMERSCONFIG);

  clearBytes( &fullFilePath, sizeof(fullFilePath));
  constructFullPath ( &ctx->cfgFilePath[0], "enb_pre_config.cfg", fullFilePath );

  if ( NULL EQ tableFunc->getReadFromFileFunc )
  {
    BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("TIMERSCONFIG");
    return FALSE;
  }
  else
  {
    ctx->totalTimers = tableFunc->getReadFromFileFunc( fullFilePath, rowOfTimerTable, &isError );
    rowOfTimerTable = (TimersConfig*)getFirstRowOfTable(TIMERSCONFIG);
    /* Copy BSM timers in BSM context */
    prmStoreTimersInCtx( ctx->totalTimers, rowOfTimerTable );
    if ( TRUE EQ isError )
    {
      BS_ERROR_CONFIG_READ_FAILED()
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * @param    srcDir        Source Directory for PRM module.
 * @param    versionInfo   Version Information.
 * @details  This function will read version.cfg file from the specified source directory.
 */
TBOOL prmReadSwPackageVersion(
  IN UINT8       *srcDir,
  IO VersionInfo *versionInfo
  )
{

  UINT8         isError = FALSE;
  FilePath      filePath;
  VersionCfgContents  versionInfoReadWriteAs;
  clearBytes( &versionInfoReadWriteAs, sizeof(VersionCfgContents) );

  snprintf(filePath, sizeof(FilePath), "%s/version.cfg", srcDir );
  logInfo("Trying to read version file from directory: %s ", filePath );
  readFromFileVersionCfgContentsTable( filePath, &versionInfoReadWriteAs, &isError );
  if ( TRUE EQ isError )
  {
    BS_ERROR_CONFIG_READ_FAILED();
    return FALSE;
    /* Check what should be done here */
  }
  convertVersionCfgContentsToVersionInfo( &versionInfoReadWriteAs, versionInfo );
  return TRUE;
}

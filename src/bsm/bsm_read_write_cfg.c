/**
 * @file    bsm_read_write_cfg.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions for reading and writing default configuration values from
 *          operator configuration table, module configuration table and status table(HwInfo only)
 *
 * @author:  Shweta Polepally
 * @date:    22 Nov 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param configFilePath   This is the configuration path, either will be NULL / or some valid path
 * @param fileName         configuration file name
 * @details This function will read the default Configuration values of all operator configuration table.
 **/
TBOOL bsmReadEnbUserRunningConfigValues(
  IN UINT8    *configFilePath,
  IN UINT8    *fileName
  )
{
  UINT8 isError = FALSE;
  UINT8 fullFilePath[MAX_BUFFER_SIZE] ;

  clearBytes( &fullFilePath, sizeof(fullFilePath));
  constructFullPath ( configFilePath, fileName, fullFilePath );

  if( isError EQ FALSE )
  {
    GenericTable               *tableFunc = getTableFunctions(ENBREGISTRATIONCONFIG);
    EnbRegistrationConfig      *config = (EnbRegistrationConfig*)getFirstRowOfTable(ENBREGISTRATIONCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED(" ENBREGISTRATIONCONFIG ");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
      else
      {
        /* check EMS IP, if it is not 127.0.0.1 disable auto UNLOCK. Because EMS do UNLOCK */
        if(strcmp("127.0.0.1",config->emsAddr))
        {
          BsmContext *ctx = getBsmContext();
          ctx->isAutoUnlockTried = TRUE;
          logInfo("%s", "SET isAutoUnlockTried to TRUE");
        }
        else
        {
          logInfo("%s", "SET isAutoUnlockTried to FALSE");
        }
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable  *tableFunc = getTableFunctions(ENODEBCONFIG);
    EnodebConfig  *config = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("ENODEBCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
      else
      {
        /*
         * Set the enodeb ID and registration accepted flag to zero
         */
        config->enodebId = 0;
        config->enodebRegistrationAccepted = 0; 
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable  *tableFunc = getTableFunctions(CELLCONFIG);
    CellConfig    *config = (CellConfig*)getFirstRowOfTable(CELLCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("CELLCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable    *tableFunc = getTableFunctions(SYSINFO1CONFIG);
    SysInfo1Config   *config = (SysInfo1Config*)getFirstRowOfTable(SYSINFO1CONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("SYSINFO1CONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable    *tableFunc = getTableFunctions(SYSINFO2CONFIG);
    SysInfo2Config   *config = (SysInfo2Config*)getFirstRowOfTable(SYSINFO2CONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("SYSINFO2CONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }


  if( isError EQ FALSE )
  {
    GenericTable    *tableFunc = getTableFunctions(SYSINFO3CONFIG);
    SysInfo3Config   *config = (SysInfo3Config*)getFirstRowOfTable(SYSINFO3CONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("SYSINFO3CONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable     *tableFunc = getTableFunctions(SCHEDULERCONFIG);
    SchedulerConfig  *config = (SchedulerConfig*)getFirstRowOfTable(SCHEDULERCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("SCHEDULERCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable   *tableFunc = getTableFunctions(PHYCONFIG);
    PhyConfig      *config = (PhyConfig*)getFirstRowOfTable(PHYCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("PHYCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable      *tableFunc = getTableFunctions(RADIOHEADCONFIG);
    RadioHeadConfig   *config = (RadioHeadConfig*)getFirstRowOfTable(RADIOHEADCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("RADIOHEADCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if(isError EQ FALSE)
  {
    GenericTable   *tableFunc = getTableFunctions(QOSCONFIG);
    QosConfig      *config = (QosConfig*)getFirstRowOfTable(QOSCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("QOSCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if(isError EQ FALSE)
  {
    GenericTable           *tableFunc = getTableFunctions(EUTRAHOPROFILECONFIG);
    EutraHoProfileConfig    *config = (EutraHoProfileConfig*)getFirstRowOfTable(EUTRAHOPROFILECONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("EUTRAHOPROFILECONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if(isError EQ FALSE)
  {
    GenericTable           *tableFunc = getTableFunctions(EUTRANEIGHCELLCONFIG);
    EutraNeighCellConfig   *config = (EutraNeighCellConfig*)getFirstRowOfTable(EUTRANEIGHCELLCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("EUTRANEIGHCELLCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if(isError EQ FALSE)
  {
    GenericTable   *tableFunc = getTableFunctions(TRACECONFIG);
    TraceConfig    *config = (TraceConfig*)getFirstRowOfTable(TRACECONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("TRACECONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if(isError EQ FALSE)
  {
    GenericTable  *tableFunc = getTableFunctions(PMCOUNTERLOGCONFIG);
    PmCounterLogConfig      *config = (PmCounterLogConfig*)getFirstRowOfTable(PMCOUNTERLOGCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("PMCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable  *tableFunc = getTableFunctions(LOGCONFIG);
    LogConfig     *config = (LogConfig*)getFirstRowOfTable(LOGCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("LOGCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable  *tableFunc = getTableFunctions(MMECONFIG);
    MmeConfig     *config = (MmeConfig*)getFirstRowOfTable(MMECONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("MMECONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }
  if( isError EQ FALSE )
  {
    GenericTable  *tableFunc = getTableFunctions(SONCONFIG);
    SonConfig     *config = (SonConfig*)getFirstRowOfTable(SONCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("SONCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }
  if( isError EQ FALSE )
  {
    GenericTable  *tableFunc       = getTableFunctions(SRSCONFIGDEDICATED);
    SrsConfigDedicated     *config = (SrsConfigDedicated*)getFirstRowOfTable(SRSCONFIGDEDICATED);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("SRSCONFIGDEDICATED");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable  *tableFunc       = getTableFunctions(SECURITYCONFIG);
    SecurityConfig    *config = (SecurityConfig*)getFirstRowOfTable(SECURITYCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("SECURITYCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable  *tableFunc       = getTableFunctions(UEEVENTHISTORYLOGCONFIG);
    UeEventHistoryLogConfig     *config = (UeEventHistoryLogConfig*)getFirstRowOfTable(UEEVENTHISTORYLOGCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("UEEVENTHISTORYLOGCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }
  if( isError EQ FALSE )
  {
    GenericTable  *tableFunc = getTableFunctions(COUNTERCONFIG);
    CounterConfig *config    = (CounterConfig*)getFirstRowOfTable(COUNTERCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("COUNTERCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  return TRUE;
}

static void bsmStoreTimersInCtx(
  IN UINT8         totalTimers,
  IN TimersConfig  *config
  )
{
  UINT8         i =0, timerArrayIndex=0;
  BsmTimersInfo *ctxTimerInfo = getBsmTimersInfo();

  for ( i=0; i < totalTimers; i++ )
  {
    if ( config->moduleId EQ MODID_BSM )
    {
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.moduleId          = config->moduleId;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeSpec.duration = config->duration;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeSpec.timeUnit = config->timeUnit;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timerType         = config->timerName;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeoutMsgId      = MSG_TIMEKEEPER_BSM_TIMEOUT_IND;
      ctxTimerInfo->totalTimersInMod++;
      timerArrayIndex++;
    }
    config++;
  }
}

/**
 * @param configFilePath  This is the configuration path, either will be NULL / or some valid path
 * @details This function will read the default Configuration values of all Advanced configuration table.
 **/
static TBOOL bsmReadEnbPreConfigValues(
  IN UINT8    *configFilePath
  )
{
  UINT8   isError = FALSE;
  UINT8   fullFilePath[MAX_BUFFER_SIZE] ;
  UINT32  numEntries;

  clearBytes( &fullFilePath, sizeof(fullFilePath));
  constructFullPath ( configFilePath, "enb_pre_config.cfg", fullFilePath );

  /* Message Trace table */
  if( isError EQ FALSE )
  {
    GenericTable   *tableFunc = getTableFunctions(MSGTRACE);
    MsgTrace       *config    = (MsgTrace*)getFirstRowOfTable(MSGTRACE);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("MSGTRACE");
      return FALSE;
    }
    else
    {
      /* return value not used - remove for all tables - TODO */
      numEntries = tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  /* Read Default Advanced table timers configuration files */
  if( isError EQ FALSE )
  {
    GenericTable  *tableFunc = getTableFunctions(TIMERSCONFIG);
    TimersConfig  *config = (TimersConfig*)getFirstRowOfTable(TIMERSCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("TIMERSCONFIG");
      return FALSE;
    }
    else
    {
      BsmContext  *ctx = getBsmContext();
      ctx->totalTimers = tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      /* Copy BSM timers in BSM context */
      config = (TimersConfig*)getFirstRowOfTable(TIMERSCONFIG);
      bsmStoreTimersInCtx( ctx->totalTimers, config );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable      *tableFunc = getTableFunctions(EVENTTHRESHOLDS);
    EventThresholds   *config = (EventThresholds*)getFirstRowOfTable(EVENTTHRESHOLDS);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("EVENTTHRESHOLDS");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable     *tableFunc = getTableFunctions(ENBADDRESSES);
    EnbAddresses     *config = (EnbAddresses*)getFirstRowOfTable(ENBADDRESSES);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("ENBADDRESSES");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }
  if( isError EQ FALSE )
  {
    GenericTable     *tableFunc = getTableFunctions(UDHCONFIG);
    UdhConfig        *config = (UdhConfig*)getFirstRowOfTable(UDHCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("UDHCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }
  if( isError EQ FALSE )
  {
    GenericTable     *tableFunc = getTableFunctions(UEHCONFIG);
    UehConfig        *config = (UehConfig*)getFirstRowOfTable(UEHCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("UEHCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }
  if( isError EQ FALSE )
  {
    GenericTable     *tableFunc = getTableFunctions(EGTPCONFIG);
    EgtpConfig        *config = (EgtpConfig*)getFirstRowOfTable(EGTPCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("EGTPCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }
  if( isError EQ FALSE )
  {
    GenericTable     *tableFunc = getTableFunctions(SCTPCONFIG);
    SctpConfig        *config = (SctpConfig*)getFirstRowOfTable(SCTPCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("SCTPCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }
  if( isError EQ FALSE )
  {
    GenericTable     *tableFunc = getTableFunctions(S1APCONFIG);
    S1apConfig        *config = (S1apConfig*)getFirstRowOfTable(S1APCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("S1APCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }
  if( isError EQ FALSE )
  {
    GenericTable              *tableFunc = getTableFunctions(DEVICEMONITORCONFIG);
    DeviceMonitorConfig       *config = (DeviceMonitorConfig*)getFirstRowOfTable(DEVICEMONITORCONFIG);

    if ( NULL EQ tableFunc->getReadFromFileFunc )
    {
      BS_ERROR_NO_CFG_READ_FUNCTION_DEFINED("DEVICEMONITORCONFIG");
      return FALSE;
    }
    else
    {
      tableFunc->getReadFromFileFunc( fullFilePath, config, &isError );
      if ( isError EQ TRUE )
      {
        return FALSE;
      }
    }
  }

  return TRUE;
}

void removeEnbUserRunningConfigTempFileAndPtr(
  FILE   *fp,
  UINT8  *filePath
  )
{
  UINT8 cmd[256];
  clearBytes( &cmd, sizeof(cmd));
  snprintf(cmd, 256, "rm -rf %s", filePath );
  if(0 NOTEQ system(cmd))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(cmd);
  }

  fclose(fp);
}

TBOOL bsmWriteEnbUserRunningConfigToPersistentMemory(
  void
  )
{
  BsmContext  *ctx = getBsmContext();
  UINT8 isError = FALSE;
  String256   string1;
  String256   string2;
  UINT8 cmd[256];
  SINT32 fileDescriptorTempRunningCfg = 0;

  UINT8 filePathRunningCfg[MAX_BUFFER_SIZE] ;
  UINT8 filePathTempRunningCfg[MAX_BUFFER_SIZE] ;

  FILE  *ptrTempRunningCfg = NULL;
  FILE  *prtRunningCfg     = NULL;
  clearBytes(&string1, sizeof(String256));
  clearBytes(&string2, sizeof(String256));
  clearBytes( &filePathRunningCfg, MAX_BUFFER_SIZE );
  clearBytes( &filePathTempRunningCfg, MAX_BUFFER_SIZE );

  constructFullPath ( ctx->configDir, "temp_enb_user_running.cfg", filePathTempRunningCfg );
  constructFullPath ( ctx->configDir, "enb_user_running.cfg", filePathRunningCfg );

  /* Write fist two lines of enb_user_running.cfg file to new file */
  ptrTempRunningCfg = fopen(filePathTempRunningCfg, "a");
  if(ptrTempRunningCfg EQ NULL)
  {
    return FALSE;
  }
  prtRunningCfg = fopen(filePathRunningCfg, "r");
  if(prtRunningCfg EQ NULL)
  {
    return FALSE;
  }
  fgets(string1, sizeof(String256), prtRunningCfg);
  fgets(string2, sizeof(String256), prtRunningCfg);

  fprintf(ptrTempRunningCfg, "%s", string1);
  fprintf(ptrTempRunningCfg, "%s", string2);

  fclose(prtRunningCfg);

  if( isError EQ FALSE )
  {
    GenericTable            *tableFunc = getTableFunctions(ENBREGISTRATIONCONFIG);
    EnbRegistrationConfig   *config    = (EnbRegistrationConfig*)getFirstRowOfTable(ENBREGISTRATIONCONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("ENBREGISTRATIONCONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }


  if( isError EQ FALSE )
  {
    GenericTable  *tableFunc = getTableFunctions(ENODEBCONFIG);
    EnodebConfig  *config = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
    EnodebConfig   tempConfig;
    copyBytes( config, sizeof(EnodebConfig), &tempConfig );
    tempConfig.enodebId                   = 0;
    tempConfig.enodebRegistrationAccepted = FALSE;

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("ENODEBCONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( &tempConfig, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable  *tableFunc = getTableFunctions(CELLCONFIG);
    CellConfig    *config = (CellConfig*)getFirstRowOfTable(CELLCONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("CELLCONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
   {
     GenericTable    *tableFunc = getTableFunctions(SYSINFO1CONFIG);
     SysInfo1Config   *config = (SysInfo1Config*)getFirstRowOfTable(SYSINFO1CONFIG);

     if ( NULL EQ tableFunc->getWriteToFileFunc )
     {
       BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("SYSINFO1CONFIG");
       removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
       return FALSE;
     }
     else
     {
       tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
       if ( isError EQ TRUE )
       {
         removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
         return FALSE;
       }
     }
   }

  if( isError EQ FALSE )
  {
    GenericTable    *tableFunc = getTableFunctions(SYSINFO2CONFIG);
    SysInfo2Config   *config = (SysInfo2Config*)getFirstRowOfTable(SYSINFO2CONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("SYSINFO2CONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable     *tableFunc = getTableFunctions(SCHEDULERCONFIG);
    SchedulerConfig  *config = (SchedulerConfig*)getFirstRowOfTable(SCHEDULERCONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("SCHEDULERCONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable   *tableFunc = getTableFunctions(PHYCONFIG);
    PhyConfig      *config = (PhyConfig*)getFirstRowOfTable(PHYCONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("PHYCONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable      *tableFunc = getTableFunctions(RADIOHEADCONFIG);
    RadioHeadConfig   *config = (RadioHeadConfig*)getFirstRowOfTable(RADIOHEADCONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("RADIOHEADCONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable   *tableFunc = getTableFunctions(QOSCONFIG);
    QosConfig      *config = (QosConfig*)getFirstRowOfTable(QOSCONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("QOSCONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable           *tableFunc = getTableFunctions(EUTRAHOPROFILECONFIG);
    EutraHoProfileConfig   *config = (EutraHoProfileConfig*)getFirstRowOfTable(EUTRAHOPROFILECONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("EUTRAHOPROFILECONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable             *tableFunc = getTableFunctions(EUTRANEIGHCELLCONFIG);
    EutraNeighCellConfig      *config = (EutraNeighCellConfig*)getFirstRowOfTable(EUTRANEIGHCELLCONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("EUTRANEIGHCELLCONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable   *tableFunc = getTableFunctions(TRACECONFIG);
    TraceConfig      *config = (TraceConfig*)getFirstRowOfTable(TRACECONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("TRACECONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable   *tableFunc = getTableFunctions(PMCOUNTERLOGCONFIG);
    PmCounterLogConfig      *config = (PmCounterLogConfig*)getFirstRowOfTable(PMCOUNTERLOGCONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("PMCONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }

   if( isError EQ FALSE )
   {
     GenericTable  *tableFunc = getTableFunctions(LOGCONFIG);
     LogConfig     *config = (LogConfig*)getFirstRowOfTable(LOGCONFIG);

     if ( NULL EQ tableFunc->getWriteToFileFunc )
     {
       BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("LOGCONFIG");
       removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
       return FALSE;
     }
     else
     {
       tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
       if ( isError EQ TRUE )
       {
         removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
         return FALSE;
       }
     }
   }

   if( isError EQ FALSE )
   {
     GenericTable    *tableFunc = getTableFunctions(SYSINFO3CONFIG);
     SysInfo3Config   *config = (SysInfo3Config*)getFirstRowOfTable(SYSINFO3CONFIG);

     if ( NULL EQ tableFunc->getWriteToFileFunc )
     {
       BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("SYSINFO3CONFIG");
       removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
       return FALSE;
     }
     else
     {
       tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
       if ( isError EQ TRUE )
       {
         removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
         return FALSE;
       }
     }
   }

   if( isError EQ FALSE )
   {
     GenericTable  *tableFunc = getTableFunctions(MMECONFIG);
     MmeConfig     *config = (MmeConfig*)getFirstRowOfTable(MMECONFIG);

     if ( NULL EQ tableFunc->getWriteToFileFunc )
     {
       BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("MMECONFIG");
       removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
       return FALSE;
     }
     else
     {
       tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
       if ( isError EQ TRUE )
       {
         removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
         return FALSE;
       }
     }
   }

  if( isError EQ FALSE )
  {
    GenericTable    *tableFunc = getTableFunctions(SONCONFIG);
    SonConfig   *config = (SonConfig*)getFirstRowOfTable(SONCONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("SONCONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable    *tableFunc = getTableFunctions(SRSCONFIGDEDICATED);
    SrsConfigDedicated   *config = (SrsConfigDedicated*)getFirstRowOfTable(SRSCONFIGDEDICATED);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("SRSCONFIGDEDICATED");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }
  if( isError EQ FALSE )
  {
    GenericTable    *tableFunc = getTableFunctions(SECURITYCONFIG);
    SecurityConfig  *config = (SecurityConfig*)getFirstRowOfTable(SECURITYCONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("SECURITYCONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable    *tableFunc = getTableFunctions(UEEVENTHISTORYLOGCONFIG);
    UeEventHistoryLogConfig   *config = (UeEventHistoryLogConfig*)getFirstRowOfTable(UEEVENTHISTORYLOGCONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("UEEVENTHISTORYLOGCONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }

  if( isError EQ FALSE )
  {
    GenericTable  *tableFunc = getTableFunctions(COUNTERCONFIG);
    CounterConfig *config = (CounterConfig*)getFirstRowOfTable(COUNTERCONFIG);

    if ( NULL EQ tableFunc->getWriteToFileFunc )
    {
      BS_ERROR_NO_CFG_WRITE_FUNCTION_DEFINED("COUNTERCONFIG");
      removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
      return FALSE;
    }
    else
    {
      tableFunc->getWriteToFileFunc( config, filePathRunningCfg, &isError, ptrTempRunningCfg );
      if ( isError EQ TRUE )
      {
        removeEnbUserRunningConfigTempFileAndPtr( ptrTempRunningCfg, filePathTempRunningCfg );
        return FALSE;
      }
    }
  }
  /* Flush the file contents immediately. Here extracting file descriptor using fileno */
  fileDescriptorTempRunningCfg= fileno(ptrTempRunningCfg);
  fsync(fileDescriptorTempRunningCfg);

  fclose(ptrTempRunningCfg);
  clearBytes(cmd, sizeof(cmd));
  snprintf(cmd, 256, "mv %s %s", filePathTempRunningCfg, filePathRunningCfg );
  if(0 NOTEQ system(cmd))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(cmd);
  }
  return TRUE;
}

/**
 * @param configFilePath  This is the configuration path, either will be NULL / or some valid path
 * @details This function will read the Alarm DB.
 **/
static TBOOL bsmReadAlarmDb(
  IN UINT8    *configFilePath
  )
{
  AlarmContext   *afhCtx = getAlarmContext();
  UINT8 isError = FALSE;
  UINT8 fullFilePath[MAX_BUFFER_SIZE] ;

  if( isError EQ FALSE )
  {
    clearBytes( &fullFilePath, sizeof(fullFilePath));
    constructFullPath ( configFilePath, "alarmDb.cfg", fullFilePath );

    afhCtx->alarmDbTable.numOfAlarms = readFromFileAlarmDb( fullFilePath, &afhCtx->alarmDbTable.alarmDb[0], &isError );
    if ( isError EQ TRUE )
    {
      BS_ERROR_CONFIG_READ_FAILED()
      exit(EXIT_FAILURE);
    }
  }
  return TRUE;
}

TBOOL bsmLoadCfgFromFiles(
  IN UINT8    *configDir
  )
{
  if ( FALSE EQ bsmReadEnbUserRunningConfigValues( configDir, "enb_user_config.cfg" ) )
    return FALSE;
  if ( FALSE EQ bsmReadEnbPreConfigValues( configDir ) )
    return FALSE;
  if ( FALSE EQ bsmReadAlarmDb( configDir ) )
     return FALSE;
  return TRUE;
}

TBOOL bsmWriteRunningConfigToNextStartConfig(
  IN UINT8    *configDir
  )
{
  String512   cmdStr;
  clearBytes( cmdStr, 512 );

  snprintf( cmdStr, 512, "cp -f %s/enb_user_running.cfg %s/enb_user_config.cfg", configDir, configDir  );
  if ( 0 NOTEQ system( cmdStr ))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(cmdStr);
    return FALSE;
  }
  return TRUE;
}

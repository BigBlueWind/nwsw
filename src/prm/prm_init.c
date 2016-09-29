/**
 * @file    prm_init.c
 * @brief This file contains Init procedure code.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author        : Shweta Polepally
 * Date          : July 2010
 * Change History:
 *
 */

#include "prm.h"
#include "imc_proc.h"
#include "syslog.h"

/**
 * @details This function will initialise ModuleStatusTable. It will read the ModuleStopTable
 *          to get the list of modules.
 **/
void prmInitModuleStatusTable(
  void
  )
{
  UINT8  i = 0;

  ModuleStatus  *rowOfModuleStatus = (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);
  ModuleStop    *rowOfStopTable    = (ModuleStop*)getFirstRowOfTable(MODULESTOP);

  PrmContext   *ctx = getPrmContext();
  /* TODO need to think about this
   * here we are assuming that both moduleStatus and moduleStop tables are having same sequence of MODID */
  for ( i=0; i < ctx->numOfModules; i++ )
  {
    rowOfModuleStatus->index                  = i+1;
    rowOfModuleStatus->moduleIdString         = rowOfStopTable->moduleIdString;
    rowOfModuleStatus->isRunning              = FALSE;
    rowOfModuleStatus->processOrTaskId        = 0;
    rowOfModuleStatus->memUsage               = 0;
    rowOfModuleStatus->numRestartsDueToErrors = 0;
    rowOfModuleStatus->restartReason          = RESTARTREASON_UNKOWN;

    snprintf( rowOfModuleStatus->moduleDesc, sizeof(rowOfStopTable->moduleDesc), "%s", rowOfStopTable->moduleDesc );
    snprintf( rowOfModuleStatus->version, MAX_SIZE_MODULESTATUS_VERSION, "0.0.0.0.0" );
    snprintf( (UINT8*)&rowOfModuleStatus->startedAt, MAX_SIZE_MODULESTATUS_STARTEDAT, "00000000000000");
    snprintf( (UINT8*)&rowOfModuleStatus->lastHealthCheckAt, MAX_SIZE_MODULESTATUS_LASTHEALTHCHECKAT, "00000000000000");

    getNextRowOfTable(rowOfModuleStatus);
    getNextRowOfTable(rowOfStopTable);
  }
}

/**
 * @details This function will initialise Software Activation Status table with default values.
 **/
void prmInitSwActivationStatusTable(
  void
  )
{
  SwActivationStatus  *rowOfActStatus = (SwActivationStatus*)getFirstRowOfTable(SWACTIVATIONSTATUS);
  rowOfActStatus->index    = 0;
  rowOfActStatus->swStatus = SWSTATUS_NOTINITIATED;
  snprintf( (UINT8*)&rowOfActStatus->version, MAX_SIZE_SWACTIVATIONSTATUS_VERSION, "0.0.0.0.0" );
}

/**
 * @details This function will initialise Software Install Status table with default values.
 **/
void prmInitSwInstallHistoryTable(
  void
  )
{
  SwInstallHistory  *rowOfSwInstallHistory = (SwInstallHistory*)getFirstRowOfTable(SWINSTALLHISTORY);
  PrmContext        *ctx                   =  getPrmContext();
  ctx->indexOfSwInstallHistory       = 0;
  ctx->noOfEntriesInSwInstallHistory = 0;

  rowOfSwInstallHistory->index = 0;
  snprintf( (UINT8*)&rowOfSwInstallHistory->filename, MAX_SIZE_SWINSTALLHISTORY_FILENAME, "none" );
  rowOfSwInstallHistory->wasSuccessful = FALSE;
  snprintf( (UINT8*)&rowOfSwInstallHistory->installErrorDetail, MAX_SIZE_SWINSTALLHISTORY_INSTALLERRORDETAIL, "No error" );
}

/**
 * @details : Initialisation of the PRM context.
 * */
void initPrmContext(
  IO PrmContext   *prmContext
  )
{
  prmContext->prmState        = IDLE;
  prmContext->areAllModulesUp = FALSE;
  prmContext->helloSeqNum     = 0;

  /* initialise PRM context: set Prm Config Dir  */
  clearBytes(&prmContext->prmInfo, sizeof(SwPackageInfo));
  snprintf(&prmContext->prmInfo.rootDir[0], sizeof(FilePath), "%s", prmContext->picoBsHomePath);
  snprintf(&prmContext->prmInfo.configDir[0], sizeof(FilePath), "%s/run", prmContext->picoBsHomePath);
  snprintf(&prmContext->prmInfo.logDir[0], sizeof(FilePath), "%s/log",  prmContext->picoBsHomePath);
  /* prmContext->prmInfo.binDir: not used */

  /* set path of instA */
  clearBytes(&prmContext->instA, sizeof(SwPackageInfo));
  snprintf(&prmContext->instA.rootDir[0], sizeof(FilePath),   "%s/rel/inst_A", prmContext->picoBsHomePath);
  snprintf(&prmContext->instA.configDir[0], sizeof(FilePath), "%s/config", prmContext->instA.rootDir);
  snprintf(&prmContext->instA.logDir[0], sizeof(FilePath),    "%s/log",  prmContext->instA.rootDir);
  snprintf(&prmContext->instA.binDir[0], sizeof(FilePath),    "%s/bin",  prmContext->instA.rootDir);

  if ( FALSE EQ prmReadSwPackageVersion(&prmContext->instA.configDir[0], &prmContext->instA.versionInfo) )
  {
    printAndEmptyErrorChain(&(getModuleContext()->errChain));
    exit(EXIT_CONFIG_READ_FAIL);
  }
  /* set path of instB */
  clearBytes(&prmContext->instB, sizeof(SwPackageInfo));
  snprintf(&prmContext->instB.rootDir[0], sizeof(FilePath),   "%s/rel/inst_B", prmContext->picoBsHomePath);
  snprintf(&prmContext->instB.configDir[0], sizeof(FilePath), "%s/config", prmContext->instB.rootDir);
  snprintf(&prmContext->instB.logDir[0], sizeof(FilePath),    "%s/log",  prmContext->instB.rootDir);
  snprintf(&prmContext->instB.binDir[0], sizeof(FilePath),    "%s/bin",  prmContext->instB.rootDir);
  if ( FALSE EQ prmReadSwPackageVersion(&prmContext->instB.configDir[0], &prmContext->instB.versionInfo) )
  {
    printAndEmptyErrorChain(&(getModuleContext()->errChain));
    logError("%s Not Found. SwInstall and SwActivation procedure cannot be performed.",prmContext->instB.rootDir );
    logError("%s","Proceeding under assumption that user will not invoke SwInstallation and activation procedure.");
    logError("Assuming that %s is the running Directory.",prmContext->instA.rootDir);
    /* TODO: These changes are done for release 1.1. only. inst_B would be present other later releases
     * exit(EXIT_CONFIG_READ_FAIL); */
  }
  /* Read sw_package.cfg file */
  prmReadSwPackageStatusTable( &prmContext->prmInfo.configDir[0] );

  /* Check if any package is set to running then update prmInfo
   * else make inst_A as default running and then update prmInfo accordingly */
  if ( FALSE EQ checkForPackageToRunAndUpdatePrmInfo( prmContext ) )
  {
    /* If NO, then mark one package as running i.e. instA and inst_B as notRunning */
    copyBytes( &prmContext->instA.versionInfo, sizeof(VersionInfo), &prmContext->prmInfo.versionInfo  );
    prmUpdateSwPackageCfgFileWithVersionInfoAtInit( &prmContext->prmInfo, prmContext );
    prmContext->isRunning     = &prmContext->instA;
    prmContext->isNotRunning  = &prmContext->instB;
    snprintf(&prmContext->cfgFilePath[0], sizeof(FilePath), "%s", prmContext->instA.configDir);
  }
  prmWriteSwPackageStatusToPersistentMemory();
}

/**
 * @param   exeName   exe Name of the process whose PID needs to be stored.
 * @param   pid       Process ID of the executable.
 * @details This function will match the exeName in the Process-Modules Mapping table and
 *          fill the PID value across the appropriate executable.
 **/
static void prmUpdatePidInMappingTable(
  UINT8     *exeName,
  SINT32     pid
  )
{
  PrmContext *ctx = getPrmContext();
  UINT8  i;

  if( NULL EQ exeName )
  {
    /* Log Error */
    BS_ERROR_INTERNAL_ERROR_OCCURED( "ExeName was expected. Pointer is NULL" );
    return;
  }

  for ( i=0; i< ctx->mappingTable.numOfMappedModules; i++ )
  {
    if( FALSE EQ strncmp( ctx->mappingTable.entry[i].exeName, exeName, sizeof(exeName) ))
    {
      ctx->mappingTable.entry[i].pid = pid;
      /* Note: break statement is not required as PID needs to be updated through out the list */
    }
  }
}

/**
 * @param   entry    Pointer to the 1st entry of the Module status table.
 * @details This function will match the Module from the Process-Modules Mapping table and Module status table
 *          and fill the PID value across the appropriate Module.
 **/
static void prmUpdatePidInModuleStatusTable(
  IO ModuleStatus    *entry
  )
{
  PrmContext *ctx = getPrmContext();
  UINT8  i;

  for ( i=0; i< ctx->mappingTable.numOfMappedModules; i++ )
  {
    if( ctx->mappingTable.entry[i].moduleName EQ  entry->moduleIdString )
    {
      entry->processOrTaskId = ctx->mappingTable.entry[i].pid;
      /* Note: break statement is not required as PID needs to be updated through out the list */
    }
  }
}

static void prmUpdateModuleVersionInfoInModuleStatusTable(
  IN UINT32           moduleSvnVersion,
  IO ModuleStatus    *entry
  )
{
  PrmContext *ctx = getPrmContext();

  snprintf( entry->version, MAX_SIZE_MODULESTATUS_VERSION, "%s.%d", (char*)ctx->moduleVersionStr, moduleSvnVersion );
}

/**
 * @param    ctx PRM Module related Information
 * @details  This function will start all the modules and set the process state to
 *            PROCESS_STATE_CHECKING.
 * */
/** TODO : Instead of calling prmStartProcesses() func with TBOOL flag as parameter,
 *  split the functions for clarity. Although functionality would not be affected */
void prmStartProcesses(
  IN TBOOL       bootLowerArm,
  IO PrmContext *ctx
  )
{
  UINT8     i=0;
  SINT8      j;
  char * argList[16];
  String100 cmd;
  String150 logPath, configPath;
  String256 lowerArmPath;
  SINT32    pid;
  SINT32    retVal = 0;
  static UINT8 count=1;

  ProcessStart   *rowOfProcessStartTable = (ProcessStart*)getFirstRowOfTable(PROCESSSTART);
  ModuleStatus   *rowOfModuleStatusTable = (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);

  for ( i=0; i< ctx->numOfModules; i++ )
  {
    rowOfModuleStatusTable->isRunning = FALSE;
    rowOfModuleStatusTable->restartReason = RESTARTREASON_UNKOWN;
    getNextRowOfTable(rowOfModuleStatusTable);
  }

  /* Start all modules */
  for ( i=0; i < ctx->numOfProcesses; i++)
  {
    pid = 0;
    /* Take Exe Name from the Process Start Table */
    if( !strncmp(rowOfProcessStartTable->exeName, "lowerarm.elf", sizeof("lowerarm.elf")) )
    {
      if(TRUE EQ bootLowerArm)
      {
        /* Booting lower ARM in 4GMX */
        clearBytes(lowerArmPath, sizeof(String256) );
        snprintf(lowerArmPath, sizeof(String256), "bootarm %s/lowerarm.elf &", ctx->isRunning->binDir);
        logInfo("--- BootArm Command: %s", lowerArmPath );
        /* Booting lower ARM as a background process */
        /* PRM will sleep waiting for lower ARM to boot */
        /* The looping will be replaced by a sleep call */
        retVal = system(lowerArmPath);
        if( 0 EQ retVal )
        {
          logInfo( "Booting of Lower ARM Image Started. Try number:%d", count );
          startPrmProcTimer( TIMERNAME_WAITBOOTLOWERARMCOMPLETE, REASON_NONE, &ctx->prmTimersInfo );
          return;
        }
        else
        {
          BS_ERROR_LOWER_ARM_BOOTING_FAILED(retVal);
        }
      }
      else
      {
        clearBytes(lowerArmPath, sizeof(String256) );
        snprintf(lowerArmPath, sizeof(String256), "grep -q lowarm /proc/devices");
        retVal = system(lowerArmPath);
        if( 0 EQ retVal )
        {
	      logInfo( "%s", "Lower ARM booted Successfully!!" );
        }
        /* Waiting for 3 times the timer for LA to boot */
        else if( count < 3 )
        {
          count++;
          /* Start timer as LowerARM didn't come up yet */
          logInfo( "Waiting for Lower ARM boot. Try number:%d", count );
	      startPrmProcTimer( TIMERNAME_WAITBOOTLOWERARMCOMPLETE, REASON_NONE, &ctx->prmTimersInfo );
	      return;
        }
        else
        {
          BS_ERROR_LOWER_ARM_BOOTING_FAILED(retVal);
        }
      }
    }
    else if(!strncmp(rowOfProcessStartTable->exeName,"wih.exe",7))
    {
      clearBytes(cmd, sizeof(String100) );
      snprintf(&cmd[0],sizeof(String100), ctx->isRunning->binDir );
      strcat(&cmd[0], "/");
      strcat(&cmd[0], "wih.exe");

      argList[0] = "spawn-fcgi.exe";      
      argList[1] = "-a";
      argList[2] = "127.0.0.1";
      argList[3] = "-p";
      argList[4] = "9000";
      argList[5] = "-n";
      argList[6] = "-f";
      /* Passing parameters to wih.exe. Please refer spawn-fcgi documentation for further clarity */
      argList[7] = "wih.exe";
      argList[8] = cmd;
      argList[9] = ctx->isRunning->rootDir;
      argList[10] = NULL;
      startExe( ctx->isRunning->binDir, "spawn-fcgi.exe", &pid, argList );
    }
    else if(!strncmp(rowOfProcessStartTable->exeName,"sih.exe",7))
    {
    /* set sih cmd line arguments */
    /* sih.exe -d -a -m /usr/local/share/snmp/mibs/ 
     * -Lf /var/log/snmpd.log -c /usr/local/share/snmp/snmpd.conf */
      argList[0] = "sih.exe";
      argList[1] = "-f";
  	  argList[2] = "-Lf";
  	  clearBytes( logPath, sizeof(String150));
	  snprintf(logPath, sizeof(String150), ctx->isRunning->rootDir );
      strcat ( logPath,"/log/snmpd.log");
  	  argList[3] = logPath;
  	  argList[4] = "-c";
  	  clearBytes( configPath, sizeof(String150));
	  snprintf(configPath, sizeof(String150), ctx->isRunning->rootDir );
      strcat ( configPath,"/config/snmpd.conf");
  	  argList[5] = configPath;
  	  argList[6] =  NULL;

      startExe( ctx->isRunning->binDir, "sih.exe", &pid, argList );
    }
    else if((0 EQ strcmp(rowOfProcessStartTable->exeName,"cpup.exe")) ||
    		(0 EQ strcmp(rowOfProcessStartTable->exeName,"cp.exe")) ||
    		(0 EQ strcmp(rowOfProcessStartTable->exeName,"gtp.exe")) ||
    		(0 EQ strcmp(rowOfProcessStartTable->exeName,"l2sim.exe")) ||
    		(0 EQ strcmp(rowOfProcessStartTable->exeName,"radioup.exe")))
    {
      argList[0] = &rowOfProcessStartTable->exeName[0];
      argList[1] = "-r";
      argList[2] = &ctx->isRunning->rootDir[0];
      argList[3] =  NULL;
      startExe( ctx->isRunning->binDir, &rowOfProcessStartTable->exeName[0], &pid, argList );
    }
    else
    {
      argList[0] = &rowOfProcessStartTable->exeName[0];
      argList[1] = "--rootDir";
      argList[2] = &ctx->isRunning->rootDir[0];
      argList[3] =  NULL;
      startExe( ctx->isRunning->binDir, &rowOfProcessStartTable->exeName[0], &pid, argList );
    }
    /* Check if PID exists */
    /* 0 means don't send any signal to the pid, just check if the pid exists */
    if ( pid NOTEQ -1 )
    {
      if( strncmp(rowOfProcessStartTable->exeName, "lowerarm.elf", sizeof("lowerarm.elf")) &&
          strncmp(rowOfProcessStartTable->exeName, "l2sim.exe", sizeof("l2sim.exe")) )
      {
        j = killChld(pid, 0);
        if ( j EQ 0 )
        {
          /* Look in module Process mapping table and update the PID value in it */
          prmUpdatePidInMappingTable( &rowOfProcessStartTable->exeName[0], pid );
        }
        else
        {
          BS_ERROR_MODULE_DID_NOT_RESPOND_AT_INIT(&rowOfProcessStartTable->exeName[0], j);
        }
      }
    }
    else
    {
      BS_ERROR_PROCESS_DID_NOT_START( &rowOfProcessStartTable->exeName[0] );
    }
    getNextRowOfTable( rowOfProcessStartTable );
  }
}

/**
 * @details This function will start all the modules present in the configuration table
 *          and start timer TIMERNAME_WAITINITCOMPLETE.
 *          On expiry : Do health Check.
 *          Reason    : This timer is required to give the processes enough time to start.
 * */
void prmDoProcessInit(
  IO PrmTimersStartReason reason
  )
{
  PrmContext  *prmContext = getPrmContext();

  prmStartProcesses( TRUE, prmContext );
  startPrmProcTimer( TIMERNAME_WAITINITCOMPLETE, reason, &prmContext->prmTimersInfo );
  prmSetState( WAIT_INIT_COMPLETE, prmContext );
}

void prmDoFallBack(
  IO PrmContext  *prmContext
  )
{
  UINT8   i;
  FilePath swActivationFilePath;
  PrmRestartReason reason;
  FILE *swActivateFileHandle = NULL;
  UINT8 buf[2];
  clearBytes(swActivationFilePath, sizeof(FilePath));
  clearBytes(buf, sizeof(buf));
  ModuleStatus  *rowOfModuleStatus = (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);
  /* Send SW_ACTIVATE_FAIL to BSM and Restart eNodeB when SW activation fails */
  if( prmContext->isSwActivationPerformed EQ TRUE )
  {
    rowOfModuleStatus = (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);
    prmContext->isSwActivationPerformed = FALSE;
    ErrorDesc      errDesc;
    clearBytes(&errDesc, sizeof(ErrorDesc));

    for ( i=0; i< prmContext->numOfModules; i++)
    {
	  if ( FALSE EQ rowOfModuleStatus->isRunning )
	  {
	    strncat(errDesc.failReason, getModuleName(rowOfModuleStatus->moduleIdString), sizeof(errDesc.failReason));
	    strncat(errDesc.failReason, " ", sizeof(errDesc.failReason));
	  }
	  getNextRowOfTable(rowOfModuleStatus);
    }
    strncat(errDesc.failReason, "modules crashed", sizeof(errDesc.failReason));
    errDesc.errorNum = BS_ERR_SW_CRASH;
    logError("%s %s", "SW Activation failed due to ", errDesc.failReason);
    /* Send SW_ACTIVATE_FAIL message to BSM */
    prmCreateAndSendSwActivateFail( SWTYPE_ENB_L2L3_SW, prmContext->isRunning->versionInfo.version, &errDesc);

    /* Write to file to record SW activation failed */
    snprintf( swActivationFilePath, sizeof(FilePath), "%s/run/%s", prmContext->picoBsHomePath, ".enbSwStatus.txt" );
    swActivateFileHandle = fopen(swActivationFilePath, "w+");
    if( swActivateFileHandle EQ NULL)
    {
      logError("Unable to open file %s to record SW activation fail", swActivationFilePath);
    }
    reason = REASON_ACTIVATE_FAIL;

    if( swActivateFileHandle NOTEQ NULL)
    {
      snprintf(buf, sizeof(buf),"%d\n", reason);
      if( 0 EQ fwrite( buf, sizeof(UINT8), 1, swActivateFileHandle ))
      {
        logError("Unable to write to file %s to record SW activation fail", swActivationFilePath);
      }
      fclose(swActivateFileHandle);
    }
    prmContext->isSwActivationDueToFallBack = TRUE;

    /* Perform SW Activation again to fall back to previous version */
    prmUpdateSwActivationStatusTable( prmContext->isNotRunning->versionInfo.version, SWSTATUS_INPROGRESS );
    prmCopyCfgToNonRunningPackage(prmContext);
    /* Bring down the currently running version */
    prmContext->moduleExitCause = MODEXITCAUSE_SWACT;
    prmDoExitProcedure( prmContext );  /* Start gets changed to "WAIT_EXIT_COMPLETE" */
  }
  return;
}

/**
 * @param   prmContext   PRM Context information.
 * @details This function will log error if any of the modules has not responded with START_IND.
 *          If not responded, will create and send MODULE_DOWN Indication.
 * */
TBOOL prmCheckForInitProcedureCompletion(
  IO PrmContext  *prmContext
  )
{
  UINT8   i;
  prmContext->areAllModulesUp = TRUE;

  ModuleStatus  *rowOfModuleStatus = (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);

  for ( i=0; i< prmContext->numOfModules; i++)
  {
    if ( FALSE EQ rowOfModuleStatus->isRunning )
    {
      prmContext->areAllModulesUp = FALSE;
      BS_ERROR_MODULE_DID_NOT_RESPOND( getModuleName(rowOfModuleStatus->moduleIdString) );
      prmCreateAndSendModuleDownInd( BS_ERR_SW_CRASH, rowOfModuleStatus );
    }
    getNextRowOfTable(rowOfModuleStatus);
  }
  if ( TRUE EQ prmContext->areAllModulesUp )
  {
    rowOfModuleStatus = (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);
    prmCreateAndSendAllModulesUpInd( rowOfModuleStatus );
    return TRUE;
  }
  else
  {
	return FALSE;
  }
}

/**
 * @param    msg        Message received at PRM from different modules.
 * @param    prmContext PRM Module related Information
 * @details This function will handle START IND if received in IDLE State
 * */
void prmProcStartIndInIdle(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  )
{
  BS_ERROR_MSG_RCVD_IN_UNEXPECTED_STATE( msg->msgId, getPrmStateName(prmContext->prmState) );
}
/**
 * @param    msg        message received
 * @param    prmContext PRM Module related Information
 * @details This function will process the start indication message received from
 *          other modules.
 *          If prm has received from all the modules,
 *          it will stop TIMERNAME_WAITINITCOMPLETE timer and start T_WAIT_HEALTH_CHECK
 *          timer for periodic health check.
 * */
void prmProcStartIndInWaitInitComplete(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  )
{
  MsgStartInd   *startInd = (MsgStartInd*)msg->data;
  UINT8         index;
  ModuleStatus  *rowOfModuleStatus = (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);

  for ( index=0; index< prmContext->numOfModules; index++)
  {
    if ( rowOfModuleStatus->moduleIdString EQ msg->src )
    {
      /* Update startedAt Format : YYYYMMDDHHNNSSTTT */
      getAndFillDateAndTimeStamp( MAX_SIZE_MODULESTATUS_STARTEDAT, &rowOfModuleStatus->startedAt[0] );
      rowOfModuleStatus->isRunning = TRUE;
      prmUpdatePidInModuleStatusTable( rowOfModuleStatus );
      prmUpdateModuleVersionInfoInModuleStatusTable( startInd->svnVersion, rowOfModuleStatus );
      prmContext->numOfRspRcvd++;
      break;
    }
    getNextRowOfTable(rowOfModuleStatus);
    /* STATE : State remains same here */
  }

  if ( prmContext->numOfRspRcvd EQ prmContext->numOfModules )
  {
    prmContext->numOfRspRcvd = 0; /* Reset the counter */

    stopPrmProcedureTimer( TIMERNAME_WAITINITCOMPLETE );
    prmCheckForInitProcedureCompletion( prmContext );
    /* Send SW_ACTIVATE_SUCC to BSM if done*/
    if( prmContext->isSwActivationPerformed EQ TRUE )
    {
      prmContext->isSwActivationPerformed = FALSE;
      prmCreateAndSendSwActivateSucc(SWTYPE_ENB_L2L3_SW, prmContext->isRunning->versionInfo.version);
    }
    startPrmProcTimer( TIMERNAME_HEALTHCHECKINTERVAL, REASON_HEALTH_CHECK, &prmContext->prmTimersInfo );
    /* Move the state to IDLE, as response is received from all modules */
    prmSetState( IDLE, prmContext );
    if (TRUE EQ prmContext->areAllModulesUp )
    {
      if (MODEXITCAUSE_L2L3_PATCH_INSTALL EQ prmContext->moduleExitCause)
      {
        prmHandleL2L3PatchStartupSucc(prmContext);
      }
      if (MODEXITCAUSE_L2L3_PATCH_INSTALL_FAILED EQ prmContext->moduleExitCause)
      {
        prmHandleL2L3PatchStartupFailed(prmContext);
      }
    }
  }
}

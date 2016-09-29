/**
 * @file    prm_cmn.c
 * @brief This file is contains the code common across all features of PRM
 *          It also functions will calls config reading/Writing function.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author        : Shweta Polepally
 * Change History:
 *
 */

#include "prm.h"
#include <time.h>

/**
 * @param   picoBsHomePath   PICOBS_HOME path
 * @details This function will return the PICO HOME BS path.
 *          If found, will return TRUE, else returns FALSE.
 * */
TBOOL prmGetPicoBsHomePath(
  IO UINT8 *picoBsHomePath
  )
{
  char *picoBsHomeDir = NULL;
  picoBsHomeDir = getenv("PICOBS_HOME");
  if(NULL NOTEQ picoBsHomeDir )
  {
    strncpy(&picoBsHomePath[0],picoBsHomeDir,sizeof(FilePath));
    return TRUE;
  }
  else
  {
    /* printf is used because log has not been initialised yet*/
    printf("Environment variable PICOBS_HOME is not set; set this to the folder containing configuration files and restart system. Example $export PICOBS_HOME=/home/picobs/");
    return FALSE;
  }
}

/**
 * @param   state  PRM state
 * @details This function will return string of PRM_STATE
 * */
char *getPrmStateName(
  IN PrmState state
  )
{
  switch( state )
  {
    case UNDEFINED:            return "UNDEFINED";
    case IDLE:                 return "IDLE";
    case WAIT_INIT_COMPLETE:   return "WAIT_INIT_COMPLETE";
    case WAIT_HELLO_ACK:       return "WAIT_HELLO_ACK";
    case WAIT_EXIT_COMPLETE:   return "WAIT_EXIT_COMPLETE";
    default:                   return "No name defined for this PRM state";
  }
}

/**
 * @param  state   PRM state
 * @param  ctx     PRM context
 * @details : This function will SET the PRM state.
 **/
inline void prmSetState(
  IN PrmState    state,
  IO PrmContext *ctx
  )
{
  ctx->prmState = state;
}


/**
 * @param   ctx    PRM context
 * @details This functions will check which package is running and accordingly
 *          updates the prmInfo in the PRM context.
 */
TBOOL checkForPackageToRunAndUpdatePrmInfo(
  IO PrmContext *ctx
  )
{
  SwPackageStatus   *rowOfSwPackageTable = (SwPackageStatus*)getFirstRowOfTable(SWPACKAGESTATUS);
  UINT8 i = 0;

  for ( i=0; i < MAX_ROWS_IN_SWPACKAGESTATUS_TABLE; i++ )
  {
    if( (rowOfSwPackageTable->swType EQ SWTYPE_ENB_L2L3_SW) && (rowOfSwPackageTable->isRunning EQ TRUE) )
    {
      copyBytes( &rowOfSwPackageTable->version, strlen(rowOfSwPackageTable->version)+1, &ctx->prmInfo.versionInfo.version  );
      copyBytes( &rowOfSwPackageTable->builtOn, strlen(rowOfSwPackageTable->builtOn)+1, &ctx->prmInfo.versionInfo.builtOn  );
      ctx->prmInfo.versionInfo.swType = rowOfSwPackageTable->swType;
      /* Check (0 EQ i%2) is added to avoid the problem when both instance are having same version */
      if( ( 0 EQ strncmp( &rowOfSwPackageTable->version[0], &ctx->instA.versionInfo.version[0], strlen(rowOfSwPackageTable->version) ) ) && ( 0 EQ i%2 ))
      {
        ctx->isRunning           = &ctx->instA;
        ctx->isNotRunning        = &ctx->instB;
        snprintf(&ctx->cfgFilePath[0], sizeof(FilePath), "%s", ctx->instA.configDir);
      }
      else if( ( 0 EQ strncmp( &rowOfSwPackageTable->version[0], &ctx->instB.versionInfo.version[0], strlen(rowOfSwPackageTable->version) ) ) && ( 1 EQ i%2 ))
      {
        ctx->isRunning           = &ctx->instB;
        ctx->isNotRunning        = &ctx->instA;
        snprintf(&ctx->cfgFilePath[0], sizeof(FilePath), "%s", ctx->instB.configDir);
      }
      else
      {
        BS_ERROR_INTERNAL_ERROR_OCCURED("This condition should not have occurred. Check code ");
      }
      return TRUE;
    }
    getNextRowOfTable(rowOfSwPackageTable);
  }
  return FALSE;
}

/**
 * @param prmInfo    Software package Information
 * @param prmContext PRM context
 * @details This functions will update SwPackage Configuration file
 */
void prmUpdateSwPackageCfgFileWithVersionInfoAtInit(
  IN SwPackageInfo   *prmInfo,
  IO PrmContext      *prmContext
  )
{
  /* Update swPackageStatus table entry with the information present in the version.cfg file */
  SwPackageStatus   *rowOfSwPackageTable = (SwPackageStatus*)getFirstRowOfTable(SWPACKAGESTATUS);
  UINT8 index = 0;

  for ( index=0; index < MAX_ROWS_IN_SWPACKAGESTATUS_TABLE; index++ )
  {
	/* Updating only the rows of INSTA */
    /* (index%2 EQ 0 ) condition will make sure that inst_A will be updated,
     * This is valid as the function is called only at PRM initialization */
    if( ( rowOfSwPackageTable->swType EQ SWTYPE_ENB_L2L3_SW ) && ( index%2 EQ 0 ))
    {
     /* (index%2 EQ 0 ) condition will make sure that inst_A will be updated (1st Entry of each type),
      * This is valid as the function is called only at PRM initialisation */
      rowOfSwPackageTable->isRunning = TRUE;
      snprintf( rowOfSwPackageTable->version, MAX_SIZE_SWPACKAGESTATUS_VERSION+1, "%s", prmInfo->versionInfo.version );
      snprintf( rowOfSwPackageTable->builtOn, MAX_SIZE_SWPACKAGESTATUS_BUILTON+1, "%s", prmInfo->versionInfo.builtOn );
      snprintf(rowOfSwPackageTable->packageDir, (strlen(prmContext->instA.rootDir)+1), prmContext->instA.rootDir );
    }
    getNextRowOfTable(rowOfSwPackageTable);
  }
}

/**
 * @version prmInfo       Software package Information
 * @version versionInfo   version Information string
 * @details This functions will create versionInfo string using SwPackageInfo
 */
void prmFormModuleVersionStringPart1(
  IN SwPackageInfo   *prmInfo,
  IO UINT8           *versionInfo
  )
{
  VersionCfgContents versionCfgContents;
  clearBytes( &versionCfgContents, sizeof(VersionCfgContents) );

  convertVersionInfoToVersionCfgContents( &prmInfo->versionInfo, &versionCfgContents );

  snprintf( versionInfo, MAX_SIZE_MODULESTATUS_VERSION, "%d.%d.%d.%d", versionCfgContents.releaseVersion, versionCfgContents.featureVersion,
      versionCfgContents.bugFixVersion, versionCfgContents.customerId );
}

/**
 * @param   totalTimers  Total timers read by PRM module from the bs_timers.cfg file.
 * @param   config       TimersConfig structure
 * @details This function will list out all the PRM timers and store them in its context.
 **/
void prmStoreTimersInCtx(
  IN UINT8         totalTimers,
  IN TimersConfig  *config
  )
{
  UINT8         i =0, timerArrayIndex=0;
  PrmTimersInfo *ctxTimerInfo = getPrmTimersInfo();

  for ( i=0; i < totalTimers; i++ )
  {
    if ( config->moduleId EQ MODID_PRM )
    {
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.moduleId          = config->moduleId;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeSpec.duration = config->duration;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeSpec.timeUnit = config->timeUnit;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timerType         = config->timerName;
      ctxTimerInfo->modTimerInfo[timerArrayIndex].timerDesc.timeoutMsgId      = MSG_TIMEKEEPER_PRM_TIMEOUT_IND;
      ctxTimerInfo->totalTimersInMod++;
      timerArrayIndex++;
    }
    getNextRowOfTable( config );
  }
}

/**
 * @param modStatus    Module Status table, Pointer to the 1st row of the table.
 * @details This function will send ALL_MODULES_UP indication to BSM.
 **/
void prmCreateAndSendAllModulesUpInd(
  IO ModuleStatus     *modStatus
  )
{
  PrmContext *ctx = getPrmContext();
  UINT8       i=0;
  UINT8       index = 0;
  MsgPrmBsmAllModulesUpInd   ind;
  clearBytes( &ind, sizeof(MsgPrmBsmAllModulesUpInd));

  for ( index = 0; index < ctx->numOfModules; index ++)
  {
    if ( TRUE EQ modStatus->isRunning )
    {
      ind.moduleId[i] = modStatus->moduleIdString;
      ind.numModUp++;
      i++;
    }
    getNextRowOfTable( modStatus );
  }
  createAndSendMsg( MSG_PRM_BSM_ALL_MODULES_UP_IND, (UINT8*)&ind, sizeof(MsgPrmBsmAllModulesUpInd));
}

/**
 * @param cause        Reason because of which module is UP
 * @param modStatus    Module Status table, Pointer to the 1st row of the table.
 * @details This function will send MODULE_UP indication to BSM.
 **/
void prmCreateAndSendModuleUpInd(
  IN BsErrorValue      bsError,
  IO ModuleStatus     *modStatus
  )
{
  MsgPrmBsmModUpInd     ind;
  PrmContext            *prmContext = getPrmContext();

  clearBytes( &ind, sizeof(MsgPrmBsmModUpInd));

  ind.moduleId                            = modStatus->moduleIdString;
  ind.errorDescForPrevModDown.errorNum    = bsError;
  snprintf( (char*)ind.errorDescForPrevModDown.failReason, sizeof(String256), "%s:%d:Curr Seq Num:%d",
            errorCodeStr(bsError), bsError, prmContext->helloSeqNum );

  createAndSendMsg( MSG_PRM_BSM_MOD_UP_IND, (UINT8*)&ind, sizeof(MsgPrmBsmModUpInd));
}

/**
 * @param cause        Reason for which module is DOWN
 * @param modStatus    Module Status table, Pointer to the 1st row of the table.
 * @details This function will send MODULE_DOWN indication to BSM.
 **/
void prmCreateAndSendModuleDownInd(
  IN BsErrorValue        bsError,
  IO ModuleStatus        *modStatus
  )
{
  MsgPrmBsmModDownInd   ind;
  PrmContext            *prmContext = getPrmContext();
  clearBytes( &ind, sizeof(MsgPrmBsmModDownInd));

  ind.moduleId              = modStatus->moduleIdString;
  ind.errorDesc.errorNum    = bsError;
  snprintf( (char*)ind.errorDesc.failReason, sizeof(String256), "%s:%d:Missed Seq Num:%d",
            errorCodeStr(bsError), bsError, prmContext->helloSeqNum );

  createAndSendMsg( MSG_PRM_BSM_MOD_DOWN_IND, (UINT8*)&ind, sizeof(MsgPrmBsmModDownInd));
}

/**
 * @param    reason   reason starting of ENB
 * @details  This function will read configuration files, initialise the status tables,
 *           and start the processInit procedure.
 */
TBOOL prmInitTablesAndStartModules(
  IO   PrmTimersStartReason   reason
  )
{
  if( FALSE EQ prmReadProcessStartTable())
    return FALSE;
  if( FALSE EQ prmReadModuleStopTable())
    return FALSE;
  if( FALSE EQ prmReadProcessModuleMappingTable())
    return FALSE;
  if( FALSE EQ readPrmTimersFromCfg())
    return FALSE;

  /* Initailize status tables */
  prmInitModuleStatusTable();
  prmInitSwActivationStatusTable();
  if (reason EQ REASON_INIT )
  {
    prmInitSwInstallHistoryTable();
  }
  /* Start INIT procedure */
  prmDoProcessInit( reason );

  return TRUE;
}

/**
 * @details  This function will print time stamp in log
 */
void prmPrintTimeStamp( void )
{
  String256               timeStamp;
  time_t                  noSecs = 0;

  clearBytes(timeStamp, sizeof(String256));
  noSecs = time(NULL);

  strftime( timeStamp, 200, "%b-%d-%y %H:%M:%S", localtime(&noSecs) );
  logInfo("Date And Time : %s",timeStamp);
}

/**
 * @param    msgIn      : Message received from BSM
 * @param    prmContext : PRM context Information
 * @details  This function will handle RH_VERSION_INFO and update the SwPackageStatus table.
 */
void prmProcRhversionInfo(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  )
{
  String256      str;
  UINT8          i = 0;
  clearBytes(str, sizeof(String256) );

  UNUSED(prmContext);
  MsgDehPrmRhVersionInfo  *versioninfo = (MsgDehPrmRhVersionInfo*)msgIn->data;
  SwPackageStatus         *rowOfSwPackageStatus = (SwPackageStatus*)getFirstRowOfTable(SWPACKAGESTATUS);

  for ( i=0; i < MAX_ROWS_IN_SWPACKAGESTATUS_TABLE; i++ )
  {
    if( SWTYPE_RH_SW EQ rowOfSwPackageStatus->swType )
    {
      snprintf( rowOfSwPackageStatus->version,MAX_SIZE_SWPACKAGESTATUS_VERSION +1,"%s",versioninfo->runningSideInfo.version);
      rowOfSwPackageStatus->isRunning = TRUE;
      snprintf( rowOfSwPackageStatus->builtOn,MAX_FWBUILDINFO_DESC +1,"%s",versioninfo->runningSideInfo.versionBuildDate);
      snprintf( rowOfSwPackageStatus->packageDir,RH_SIDE_INFO +1,"%s",versioninfo->runningSideInfo.rhSide);
      getNextRowOfTable( rowOfSwPackageStatus );

      snprintf( rowOfSwPackageStatus->version,MAX_SIZE_SWPACKAGESTATUS_VERSION +1,"%s",versioninfo->nonRunningSideInfo.version);
      rowOfSwPackageStatus->isRunning = FALSE;
      snprintf( rowOfSwPackageStatus->builtOn,MAX_FWBUILDINFO_DESC +1,"%s",versioninfo->nonRunningSideInfo.versionBuildDate);
      snprintf( rowOfSwPackageStatus->packageDir,RH_SIDE_INFO +1,"%s",versioninfo->nonRunningSideInfo.rhSide);
    }
    getNextRowOfTable( rowOfSwPackageStatus );
  }

  prmWriteSwPackageStatusToPersistentMemory();
}

/**
 * @param    prmContext : PRM context Information
 * @details  This function will PRM is started due to SW Activation
 */
TBOOL isPrmStartedDueToSwActivation(
  IO PrmContext  *prmContext
  )
{
  FilePath swActivationFilePath;
  PrmRestartReason reason;
  FILE *swActivateFileHandle=NULL;
  UINT8 buf[2];
  TBOOL result=FALSE;
  clearBytes(swActivationFilePath, sizeof(FilePath));
  clearBytes(buf, sizeof(buf));
  logInfo("%s", "SW activation check");
  /* Write to file to record SW activation status */
  snprintf( swActivationFilePath, sizeof(FilePath), "%s/run/%s", prmContext->picoBsHomePath, ".enbSwStatus.txt" );
  swActivateFileHandle = fopen(swActivationFilePath, "r+");

  if( swActivateFileHandle EQ NULL)
  {
    logError("Unable to open file %s to read SW activation", swActivationFilePath);
    return FALSE;
  }
  
  if( 0 EQ fread( buf, sizeof(UINT8), 1,  swActivateFileHandle ))
  {
    logError("Unable to read from the file %s to get SW activation", swActivationFilePath);
    return FALSE;
  }
 
  reason = atoi(buf);

  if( reason EQ REASON_ACTIVATE)
  {
	logInfo("%s", "SW activation performed");
	result = TRUE;
  }
  else if( reason EQ REASON_ACTIVATE_FAIL)
  {
	result = FALSE;
	logError("%s", "Previous SW activation failed. Fallback performed");
  }

  fseek( swActivateFileHandle, 0L, SEEK_SET );
  if( 0 EQ fwrite( "0", sizeof(UINT8), 1, swActivateFileHandle ))
  {
    logError("Unable to write to file %s to reset SW activation", swActivationFilePath);
  }
  fclose(swActivateFileHandle);

  return result;
}

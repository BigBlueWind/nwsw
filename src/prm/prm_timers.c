/**
 * @file    prm_timers.c
 * @brief This file contains Timer handling functions of PRM modules
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author        : Shweta Polepally
 * Date          : Sep 2010
 * Change History:
 *
 */

#include "prm.h"
#include "imc_proc.h"
#include <sys/reboot.h>

/**
 * @param timerType  Type of timer
 * @param reason     This is the reason why timer was started.
 * @param timerInfo  This structure contains timer information - Timer description and timer Id
 * @details This function creates and starts Procedure Timer.
 **/
void startPrmProcTimer(
    IN TimerName             timerType,
    IN PrmTimersStartReason  reason,
    IO PrmTimersInfo        *timerInfo
    )
{
  UINT8       i=0;
  String50    buf;
  clearBytes( &buf, sizeof(String50));

  for ( i=0; i< timerInfo->totalTimersInMod; i++ )
  {
    if ( timerType EQ timerInfo->modTimerInfo[i].timerDesc.timerType )
        break;
  }
  timerInfo->modTimerInfo[i].timerDesc.userInfo = reason;

  /* Create and start Timer */
  timerInfo->modTimerInfo[i].timerId = createAndStartTimer( &timerInfo->modTimerInfo[i].timerDesc , NULL, NULL);
}

/**
 * @param   type  Type of timer
 * @details This function will look for the health check timer in timerInfo table
 *          and stop this timer.
 **/
void  stopPrmProcedureTimer(
  IO TimerName   type
  )
{
  PrmContext  *prmContext = getPrmContext();
  UINT8        i;
  String50    buf;
  clearBytes( &buf, sizeof(String50));

  for ( i=0; i< prmContext->prmTimersInfo.totalTimersInMod; i++ )
  {
    if (prmContext->prmTimersInfo.modTimerInfo[i].timerDesc.timerType EQ type )
    {
      stopTimer( prmContext->prmTimersInfo.modTimerInfo[i].timerId );
      break;
    }
  }
}

/** Timer expiry handling functions in all states **/
/**
 * @param  msg        Message received
 * @param  prmContext PRM Module related Information
 * @details  This function will handle Timer expiry message when PRM is in IDLE state.
 *           This would be an invalid case.
 * */
void prmProcTimerExpiryInIdle(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  )
{
  MsgTimeoutInd *msgTimeoutInd= (MsgTimeoutInd*)msg->data;
  String50    buf;
  clearBytes( &buf, sizeof(String50));

  switch( msgTimeoutInd->timerDesc.timerType )
  {
    case TIMERNAME_HEALTHCHECKINTERVAL:
    {
      prmDoHeartBeatCheck( prmContext );
      prmSetState( WAIT_HELLO_ACK, prmContext );
    }
    break;
    case TIMERNAME_WAITRHSWINSTALLRSP:
    {
      String256 str;
      /* Delete the composite image file */
      clearBytes(str, sizeof(String256) );
      snprintf( str, MAX_SIZE_SWINSTALL_FILENAME + 1, "%s/download/%s", prmContext->picoBsHomePath, prmContext->rhSwInstallBinFile );
      if(0 NOTEQ system(str))
      {
        BS_ERROR_SYSTEM_COMMAND_FAILED(str);
      }

      prmCreateAndSendSwInstallFail(BS_ERR_RH_INSTALL_TIMEOUT, prmContext);
      prmUpdateSwInstallStatusTable( prmContext->swinstall.filename, FALSE, "TIMERNAME_WAITRHSWINSTALLRSP Expired. DEH did not respond for RH_SW_INSTALL_REQ" );
      clearBytes(&(prmContext->swinstall), sizeof(SwInstall));
      prmContext->isInstallInProgress = FALSE;
    }
    break;
    case TIMERNAME_WAITRHACTIVATERSP:
    {
      ErrorDesc      errDesc;
      clearBytes(&errDesc, sizeof(ErrorDesc));
      errDesc.errorNum = BS_ERR_RH_ACTIVATION_TIMEOUT;
      snprintf(errDesc.failReason, sizeof(errDesc.failReason), "%s", "Timer TIMERNAME_WAITRHACTIVATERSP expired. DEH did not responded for ACTIVATE request.");
      prmCreateAndSendSwActivateFail(SWTYPE_RH_SW, prmGetRhNonRunningVersion(), &errDesc  );
      prmUpdateSwActivationStatusTable( prmGetRhNonRunningVersion(), SWSTATUS_FAILED);
      prmContext->isActivationInProgress = FALSE;
    }
    break;

    default:
    {
      BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType, getPrmStateName(prmContext->prmState) );
      break;
    }
  }
}

/**
 * @param  msg        Message received at PRM
 * @param  prmContext PRM Module related Information
 * @details  This function will handle timer expiry in WAIT_HELLO_ACK state.
 *           Timer Type handled - TIMERNAME_WAITHELLOACK
 * */
void prmProcTimerExpiryInWaitHelloAck(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  )
{
  MsgTimeoutInd *msgTimeoutInd     = (MsgTimeoutInd*)msg->data;
  switch( msgTimeoutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITHELLOACK:
    {
      prmCheckForHealthCheckProcedureCompletion( prmContext );
      startPrmProcTimer( TIMERNAME_HEALTHCHECKINTERVAL, REASON_HEALTH_CHECK, &prmContext->prmTimersInfo );
      /* Move the state to IDLE, as response is received from all modules */
      prmSetState( IDLE, prmContext );
    }
    break;
    case TIMERNAME_WAITRHSWINSTALLRSP:
    {
      String256 str;
      /* Delete the composite image file */
      clearBytes(str, sizeof(String256) );
      snprintf( str, MAX_SIZE_SWINSTALL_FILENAME + 1, "%s/download/%s", prmContext->picoBsHomePath, prmContext->rhSwInstallBinFile );
      if(0 NOTEQ system(str))
      {
        BS_ERROR_SYSTEM_COMMAND_FAILED(str);
      }

      prmCreateAndSendSwInstallFail(BS_ERR_RH_INSTALL_TIMEOUT, prmContext);
      prmUpdateSwInstallStatusTable( prmContext->swinstall.filename, FALSE, "TIMERNAME_WAITRHSWINSTALLRSP Expired. DEH did not respond for RH_SW_INSTALL_REQ" );
      clearBytes(&(prmContext->swinstall), sizeof(SwInstall));
      prmContext->isInstallInProgress = FALSE;
    }
    break;

    case TIMERNAME_WAITRHACTIVATERSP:
    {
      ErrorDesc      errDesc;
      clearBytes(&errDesc, sizeof(ErrorDesc));
      errDesc.errorNum = BS_ERR_RH_ACTIVATION_TIMEOUT;
      snprintf(errDesc.failReason, sizeof(errDesc.failReason), "%s", "Timer TIMERNAME_WAITRHACTIVATERSP expired. DEH did not responded for ACTIVATE request.");
      prmCreateAndSendSwActivateFail(SWTYPE_RH_SW, prmGetRhNonRunningVersion(), &errDesc  );
      prmUpdateSwActivationStatusTable( prmGetRhNonRunningVersion(), SWSTATUS_FAILED);
      prmContext->isActivationInProgress = FALSE;
    }
    break;

    default:
    {
      BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType, getPrmStateName(prmContext->prmState) );
      break;
    }
  }
}


/**
 * @param  msg        Message received at PRM
 * @param  prmContext PRM Module related Information
 * @details  This function will handle timer expiry in WAIT_START_COMPLETE state.
 *           Timer Type handled - TIMERNAME_WAITINITCOMPLETE
 * */
void prmProcTimerExpiryInWaitInitComplete(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  )
{
  MsgTimeoutInd *msgTimeoutInd    = (MsgTimeoutInd*)msg->data;

  switch( msgTimeoutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITINITCOMPLETE:
    {
      if( FALSE EQ prmCheckForInitProcedureCompletion( prmContext ))
      {
        prmDoFallBack( prmContext );
        break;
      }
      else
      {
        /* Send SW_ACTIVATE_SUCC to BSM */
        if( prmContext->isSwActivationPerformed EQ TRUE )
        {
	      prmContext->isSwActivationPerformed = FALSE;
	      prmCreateAndSendSwActivateSucc(SWTYPE_ENB_L2L3_SW, prmContext->isRunning->versionInfo.version);
        }
      }
      if(msgTimeoutInd->timerDesc.userInfo EQ REASON_L2L3_PATCH_INSTALL)
      {
        prmContext->moduleExitCause = MODEXITCAUSE_L2L3_PATCH_INSTALL_FAILED;
        prmDoExitProcedure( prmContext );
      }
      else if(msgTimeoutInd->timerDesc.userInfo EQ REASON_L2L3_PATCH_INSTALL_FAILED)
      {
        startPrmProcTimer( TIMERNAME_HEALTHCHECKINTERVAL, REASON_HEALTH_CHECK, &prmContext->prmTimersInfo );
        /* Move the state to IDLE, as response is received from all modules */
        prmSetState( IDLE, prmContext );
        prmHandleL2L3PatchStartupFailed( prmContext );
      }
      else
      {
        startPrmProcTimer( TIMERNAME_HEALTHCHECKINTERVAL, REASON_HEALTH_CHECK, &prmContext->prmTimersInfo );
        /* Move the state to IDLE, as response is received from all modules */
        prmSetState( IDLE, prmContext );
      }
    }
    break;
    case TIMERNAME_WAITBOOTLOWERARMCOMPLETE:
    {
      prmStartProcesses( FALSE, prmContext );
      break;
    }
    default:
    {
      BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType, getPrmStateName(prmContext->prmState) );
      break;
    }
  }
}

/**
 * @param  msg        Message received at PRM
 * @param  prmContext PRM Module related Information
 * @details  This function will handle timer expiry in WAIT_EXIT_COMPLETE state.
 *           Timer Type handled - TIMERNAME_WAITEXITCOMPLETE
 * */
void prmProcTimerExpiryInWaitExitComplete(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  )
{
  MsgTimeoutInd  *msgTimeoutInd    = (MsgTimeoutInd*)msg->data;
  String512      cmdStr;
  TimeSpec       tick = { TIMEUNIT_SEC, 1 };
  ErrorDesc      errDesc;
  clearBytes(&errDesc, sizeof(ErrorDesc));
  clearBytes(cmdStr, sizeof(String512));

  switch( msgTimeoutInd->timerDesc.timerType )
  {
    case TIMERNAME_WAITEXITCOMPLETE:
      {
         if ( FALSE EQ prmContext->exitProcCmpl )
         {
           prmSendExitMsgToExitPhaseModules( EXIT_PHASE_LEVEL_2, prmContext );
         }
         else  /* Do below if exit procedure is completed */
         {
           ModuleStatus  *rowOfStatusTable =  (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);
           PrmTimersStartReason reasonTimer     = msgTimeoutInd->timerDesc.userInfo;
           prmCheckForExitProcedureCompletion( prmContext, rowOfStatusTable );

           if ( reasonTimer EQ REASON_RESTART )
           {
             prmDoProcessInit( REASON_RESTART ); /* Carry out Start Up process */
             /* prm State is changed to WAIT_INIT_COMPLETE state in doProcessInit() function */
           }
           else if ( reasonTimer EQ REASON_REBOOT )
           {
             prmSetState( WAIT_EXIT_COMPLETE, prmContext );
             char *cmd = "/sbin/reboot";
             if(0 NOTEQ system(cmd))
             {
               BS_ERROR_SYSTEM_COMMAND_FAILED(cmd);
             }

            /*ret = reboot(RB_AUTOBOOT); ===> Dont use this, as it caused OS to crash
              if ( ret EQ -1 )
               perror(&ret);*/
           }
           else if ( reasonTimer EQ REASON_SW_ACTIVATION )
           {
             /* Activate the new version i.e. which was currently not running */
        	 FilePath swActivationFilePath;
        	 PrmRestartReason reason;
        	 FILE *swActivateFileHandle = NULL;
        	 UINT8 buf[2];
        	 clearBytes(swActivationFilePath, sizeof(FilePath));
        	 clearBytes(buf, sizeof(buf));
             SwPackageInfo *tmp    = prmContext->isRunning;
             prmContext->isRunning = prmContext->isNotRunning;
             prmContext->isNotRunning = tmp;
             snprintf(&prmContext->cfgFilePath[0], sizeof(FilePath), "%s", prmContext->isRunning->configDir);
             prmUpdateSwActivationStatusTable( &prmContext->isRunning->versionInfo.version[0], SWSTATUS_SUCCESSFUL );
             prmUpdateSwPackageStatusTableForEnbSw( prmContext );
             prmWriteSwPackageStatusToPersistentMemory();
             prmReplaceExeWithRunningOne(prmContext);

             /* Reboot the eNodeB */
             prmSetState( WAIT_EXIT_COMPLETE, prmContext );

             /* Write to file to record SW activation is performed */
             snprintf( swActivationFilePath, sizeof(FilePath), "%s/run/%s", prmContext->picoBsHomePath, ".enbSwStatus.txt" );

             swActivateFileHandle = fopen(swActivationFilePath, "w+");

             if( swActivateFileHandle EQ NULL)
             {
               logError("Unable to open file %s to record SW activation", swActivationFilePath);
             }
             /* Exact reason for SW activation */
             if( prmContext->isSwActivationDueToFallBack EQ FALSE)
             {
               reason = REASON_ACTIVATE;
             }
             else
             {
               reason = REASON_ACTIVATE_FAIL;
             }
             if( swActivateFileHandle NOTEQ NULL)
             {
               snprintf(buf, sizeof(buf),"%d\n", reason);
               if( 0 EQ fwrite( buf, sizeof(UINT8), 1, swActivateFileHandle ))
               {
                 logError("Unable to write to file %s to record SW activation", swActivationFilePath);
               }
               fclose(swActivateFileHandle);
             }

             /* Release all resources */
             modExit(MODID_PRM, &(getModuleContext()->exeName[0]));
             UNUSED(tick);
             /* Reboot eNodeB */
             UINT8 *cmd = "/sbin/reboot";
             if(0 NOTEQ system(cmd))
             {
               BS_ERROR_SYSTEM_COMMAND_FAILED(cmd);
               tmp    = prmContext->isRunning;
               prmContext->isRunning = prmContext->isNotRunning;
               prmContext->isNotRunning = tmp;
               snprintf(&prmContext->cfgFilePath[0], sizeof(FilePath), "%s", prmContext->isRunning->configDir);
               prmUpdateSwActivationStatusTable( &prmContext->isRunning->versionInfo.version[0], SWSTATUS_SUCCESSFUL );
               prmUpdateSwPackageStatusTableForEnbSw( prmContext );
               prmWriteSwPackageStatusToPersistentMemory();
               prmReplaceExeWithRunningOne(prmContext);
               if( FALSE EQ modInit( MODID_PRM, getModuleName(MODID_PRM), &prmContext->isRunning->rootDir[0], tick ) )
               {
                 BS_ERROR_MOD_REGISTER("PRM", FALSE);
                 printAndEmptyErrorChain(&(getModuleContext()->errChain));
                 exit(EXIT_MOD_INIT_FAILED);
               }
               errDesc.errorNum = BS_ERR_FILE_NOT_FOUND;
               snprintf(errDesc.failReason, sizeof(errDesc.failReason), "%s %s", getModuleName(MODID_PRM), "restart for SW activation file not found");
             }
             else
             {
               /* This log doesnot get printed as log file  is closed. Just used for clarity */
               //logInfo("SW Activation performed successfully. PRM started pid = %d", pid);
               exit(EXIT_SUCCESS);
             }

             /* Execution continues from here in case of SW Activation fails */
             prmInitTablesAndStartModules( REASON_SW_ACTIVATION );
             prmContext->isActivationInProgress = FALSE;
             logError("%s %s", "SW Activation failed due to ", errDesc.failReason);

             /* Send SW_ACTIVATE_FAIL message to BSM */
             prmCreateAndSendSwActivateFail( SWTYPE_ENB_L2L3_SW, prmContext->isRunning->versionInfo.version, &errDesc);

             /*Update oamTableCol Info in prmContext */
             clearBytes(&prmContext->oprCfgTableInfo, sizeof(PrmOamTableInfo));
             prmParseOamOprCfgTables( &(prmContext->cfgFilePath[0]), &prmContext->oprCfgTableInfo[0]);
           }
           else if ( reasonTimer EQ REASON_L2L3_PATCH_INSTALL )
           {
             prmCopyPatchFilesToRunningPackage( prmContext );
             prmDoProcessInit( REASON_L2L3_PATCH_INSTALL ); /* Carry out Start Up process */
           }
           else if ( reasonTimer EQ REASON_L2L3_PATCH_INSTALL_FAILED )
           {
             prmCopyBackupFilesToRunningPackage( prmContext );
             prmDoProcessInit( REASON_L2L3_PATCH_INSTALL_FAILED ); /* Carry out Start Up process */
           }
           else
           {
            /* Do nothing, just inform - Make a note of it */
             logInfo("Received TimerId =%d, reason =%d, id not known, in state=%s", msgTimeoutInd->timerId, reasonTimer, getPrmStateName(prmContext->prmState) );
           }
         }
      }
      break;

    default:
    {
      BS_ERROR_INVALID_TIMERID_EXPIRED( msgTimeoutInd->timerDesc.timerType, getPrmStateName(prmContext->prmState) );
      break;
    }
  }
}

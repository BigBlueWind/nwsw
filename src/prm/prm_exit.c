/**
 * @file    prm_exit.c
 * @brief This file contains Exit procedure code.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author        : Shweta Polepally
 * Date          : Sep 2010
 * Change History:
 *
 */

#include "prm.h"
#include <sys/wait.h>

/**
 * @param cause    Cause for Sending Module Exit
 * @details This function will return the exit cause in string Format.
 */
static char *prmGetExitCauseName(
  IN ModExitCause cause
  )
{
  switch( cause)
  {
    case MODEXITCAUSE_RESTART:                   return "MODEXITCAUSE_RESTART";
    case MODEXITCAUSE_REBOOT:                    return "MODEXITCAUSE_REBOOT";
    case MODEXITCAUSE_SWACT:                     return "MODEXITCAUSE_SWACT";
    case MODEXITCAUSE_L2L3_PATCH_INSTALL:        return "MODEXITCAUSE_PATCH_INSTALL";
    case MODEXITCAUSE_L2L3_PATCH_INSTALL_FAILED: return "MODEXITCAUSE_PATCH_INSTALL_FAILED";
    default:                                     return "Cause name not handled-Update Code";
  }
}

/**
 * @param   cause  Cause for Sending Module Exit
 * @param   modId  Module ID
 * @details This function will sends Exit message to given module.
 */

static void prmCreateAndSendExitMsg(
  IN ModExitCause cause,
  IN ModuleId     modId
  )
{
  MsgPrmModExitCmd   exitCmd;
  clearBytes( &exitCmd, sizeof(MsgPrmModExitCmd));
  exitCmd.modExitCause= cause;

  switch(modId)
  {
    case MODID_FTH:
      createAndSendMsg( MSG_PRM_FTH_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_EGTP:
      createAndSendMsg( MSG_PRM_EGTP_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_TUCL:
      createAndSendMsg( MSG_PRM_TUCL_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_PDCP:
      createAndSendMsg( MSG_PRM_PDCP_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_RLC:
      createAndSendMsg( MSG_PRM_RLC_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_MAC:
      createAndSendMsg( MSG_PRM_MAC_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_RRC:
      createAndSendMsg( MSG_PRM_RRC_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_SCTP:
      createAndSendMsg( MSG_PRM_SCTP_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_S1AP:
      createAndSendMsg( MSG_PRM_S1AP_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_BSM:
      createAndSendMsg( MSG_PRM_BSM_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_DEH:
      createAndSendMsg( MSG_PRM_DEH_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_UEH:
      createAndSendMsg( MSG_PRM_UEH_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_SIH:
      createAndSendMsg( MSG_PRM_SIH_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_WIH:
      createAndSendMsg( MSG_PRM_WIH_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_LTH:
      createAndSendMsg( MSG_PRM_LTH_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_PMH:
      createAndSendMsg( MSG_PRM_PMH_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    case MODID_UDH:
      createAndSendMsg( MSG_PRM_UDH_EXIT_CMD, (UINT8*)&exitCmd, sizeof(MsgPrmModExitCmd) );
      break;
    default :
      BS_ERROR_UNKNOWN_MOD_ID(modId );
      break;
  }
}

/**
 * @param   
 * @details This function will send Exit command to MAC 
 *         MAC will call Physhutdown, Next MAC will be killed forcefully.
 **/
void prmProcPhyshutdown(
)
{
  createAndSendMsg( MSG_PRM_MAC_EXIT_CMD,NULL,0 );
}

/**
 * @param   prmContext    PRM Context
 * @param   moduleStatus  module status structure
 * @details This function will check for the Exit procedure completion
 *            If any process still exists, it will be killed forcefully.
 **/
void prmCheckForExitProcedureCompletion(
  IO PrmContext           *prmContext,
  IO ModuleStatus         *moduleStatus
  )
{
  UINT32    i;
  int       j; /* leave it as int */
  String50  str;

  clearBytes(&str, sizeof(String50));

  for ( i=0 ; i< prmContext->numOfModules; i++)
  {
    waitForChld( (pid_t) moduleStatus->processOrTaskId);
    getNextRowOfTable(moduleStatus);
  }

  /* Step 1: Check if each of the process has exited, else kill forcefully */
  moduleStatus =  (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);
  for ( i=0 ; i< prmContext->numOfModules; i++)
  {
    j = killChld( moduleStatus->processOrTaskId, 0 );
    if(j EQ NO_PROCESS_EXISTS )
    {
      logInfo(" PID=0 for moduleId=%s..Updation of module status is wrong or the Module was never started", getModuleName(moduleStatus->moduleIdString));
    }
    else if ( j EQ 0 )
    {
      /* Kill the process forcefully */
      killChld ( moduleStatus->processOrTaskId, SIGKILL );
      BS_ERROR_PROCESS_STILL_RUNNING(getModuleName(moduleStatus->moduleIdString), moduleStatus->processOrTaskId );
    }
    moduleStatus->isRunning        = 0;
    moduleStatus->memUsage         = 0;
    moduleStatus->processOrTaskId  = 0;
    getNextRowOfTable(moduleStatus);
  }
}


static void prmUpdateRestartCauseInModuleStatusTable(
   IN ModExitCause    cause,
   IO ModuleStatus   *entry
   )
{
  switch ( cause )
  {
    case MODEXITCAUSE_RESTART:
    case MODEXITCAUSE_REBOOT:
    case MODEXITCAUSE_SWACT:
    case MODEXITCAUSE_L2L3_PATCH_INSTALL:
    case MODEXITCAUSE_L2L3_PATCH_INSTALL_FAILED:
      entry->restartReason = RESTARTREASON_OPERATORCMD;
      break;
    default:
      entry->restartReason = RESTARTREASON_UNKOWN;
      break;
    /* To add more if required */
  }
}

static PrmTimersStartReason prmGetPrmTimerStartReasonForExit(
  IN ModExitCause moduleExitCause
  )
{
  switch( moduleExitCause )
  {
    case MODEXITCAUSE_RESTART:                   return REASON_RESTART;
    case MODEXITCAUSE_REBOOT:                    return REASON_REBOOT;
    case MODEXITCAUSE_SWACT:                     return REASON_SW_ACTIVATION;
    case MODEXITCAUSE_L2L3_PATCH_INSTALL:        return REASON_L2L3_PATCH_INSTALL;
    case MODEXITCAUSE_L2L3_PATCH_INSTALL_FAILED: return REASON_L2L3_PATCH_INSTALL_FAILED;
    default:                                     return REASON_MAX;
  }
}

/* Send exit to modules with exitPriority as 1 */
void prmSendExitMsgToExitPhaseModules(
  IN UINT8         exitPhaseLevel,
  IO PrmContext   *prmContext
  )
{
  SINT8        i=0;
  ModuleStop   *rowOfModuleStop   = (ModuleStop*)getFirstRowOfTable(MODULESTOP);

  for ( i=0; i < prmContext->numOfModules; i++ )
  {
    if( exitPhaseLevel EQ rowOfModuleStop->exitPriority )
    {
      prmCreateAndSendExitMsg( prmContext->moduleExitCause, rowOfModuleStop->moduleIdString );
    }
    getNextRowOfTable(rowOfModuleStop);
  }

  prmContext->exitPhaseLevel++;
  if ( EXIT_PHASE_LEVEL_MAX EQ prmContext->exitPhaseLevel )
  {
    prmContext->exitProcCmpl = TRUE;
  }
  /* This timer is started to give all processes enough time to stop safely. */
  startPrmProcTimer( TIMERNAME_WAITEXITCOMPLETE, prmGetPrmTimerStartReasonForExit(prmContext->moduleExitCause), &prmContext->prmTimersInfo );
}

/**
 * @param   prmContext PRM Context
 * @details : This function will carry out Exit Process procedure.
 *            1. Send EXIT_CMD to all modules/
 *            2. Update the ProcessStatusTable with processRunState = EXITING.
 **/
void prmDoExitProcedure(
  IO PrmContext  *prmContext
  )
{
  SINT8      i;
  prmContext->numOfRspRcvd = 0;

  stopPrmProcedureTimer( TIMERNAME_HEALTHCHECKINTERVAL );
  logInfo("PRM has asked to exit all modules due to %s", prmGetExitCauseName(prmContext->moduleExitCause) );

  /* Update restart cause in Module status table */
  ModuleStatus  *rowOfModuleStatus = (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);
  for ( i=0; i < prmContext->numOfModules; i++ )
  {
    prmUpdateRestartCauseInModuleStatusTable( prmContext->moduleExitCause, rowOfModuleStatus );
    getNextRowOfTable(rowOfModuleStatus);
  }
  /* Exit Procedure to be performed in 2 phases:
   * 1. Exit all ccpu Modules and oam modules(except BSM and interface modules)
   * 2. Exit interface modules (WIH,SIH) and BSM Module */

  prmSetState( WAIT_EXIT_COMPLETE, prmContext );

  /* Phase 1 : Send exit to only phase 1 modules */
  prmContext->exitPhaseLevel      = 0;
  prmContext->exitProcCmpl        = FALSE;
  prmSendExitMsgToExitPhaseModules( EXIT_PHASE_LEVEL_1, prmContext ); /* Phase level - 1 */
  /* Phase 2 to start when exit timer expires */
}

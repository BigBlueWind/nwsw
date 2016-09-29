/**
 * @file    prm_reboot.c
 * @brief This file contains functions related to Reboot Procedure.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author        : Shweta Polepally
 * Date          : July 2010
 * Change History:
 */

#include "prm.h"

/**
 * @param  msgIn      Message received at PRM
 * @param  prmContext : PRM Module related Information
 * @brief  This function will handle Reboot Request message in all states.
 *           1. Sends Exit message to all modules.
 *           2. Starts timer TIMERNAME_WAITEXITCOMPLETE with reason "reboot"
 *              Reason : This will give enough time for all the modules to exit gracefully.
 *              On expiry: Reboots the system.
 * */
void prmProcRebootReq(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  )
{
  UNUSED(msgIn);
  prmContext->moduleExitCause = MODEXITCAUSE_REBOOT;
  prmDoExitProcedure( prmContext );  /* Start gets changed to "WAIT_EXIT_COMPLETE" */
}


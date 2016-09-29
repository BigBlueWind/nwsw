/**
 * @file    prm_restart.c
 * @brief This file contains functions related to Restart Procedure.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author        : Shweta Polepally
 * Date          : July 2010
 * Change History:
 *
 */
#include "prm.h"

/**
 * @param  msgIn      Message received
 * @param  prmContext PRM Module related Information
 * @details  This function will handle Restart Request message in all states.
 *           1. Sends Exit message to all modules.
 *           2. Starts timer TIMERNAME_WAITEXITCOMPLETE with reason "restart"
 *              Reason : This will give enough time for all the modules to exit gracefully.
 *              On expiry : Restarts all the BS Software modules.
 **/
void prmProcRestartReq(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  )
{
  UNUSED(msgIn);
  prmContext->moduleExitCause = MODEXITCAUSE_RESTART;
  prmDoExitProcedure( prmContext );
}


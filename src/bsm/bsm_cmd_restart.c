/**
 * @file    bsm_cmd_restart.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions related to restart procedure.
 *
 * @author:  Shweta Polepally
 * @date:    Sep 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param    setObj     SET Object message received from <OAM-IF> module.
 * @param    bsmContext BSM module related information.
 * @details  This function will ask PRM to carry out restart procedure by sending
 *           RESTART_REQ message.
 */
void bsmDoRestart(
  IN SetObjReq   *setObj,
  IO BsmContext  *bsmContext
  )
{
  if ( TRUE EQ isEnodeBLocked() )
  {
    bsmFillAndSendSetObjSucc( setObj );
    /* Disable Operational State, so that we can reject any other user action
     * Set the procedure as restart */
    setBsmProceduralState( bsmContext, ENODEB_WAIT_RESTART_CMPL );
    /* Write the date and time values in the persistent memory - enb_user_config*/
    writeDateAndTimeInNextStartConfig( &bsmContext->configDir[0] );
    createAndSendMsg( MSG_BSM_PRM_RESTART_REQ, NULL, 0);
  }
  else
  {
    BS_ERROR_ENODEB_NOT_LOCKED();
    bsmFillAndSendSetObjFail( setObj, BS_ERR_ENODEB_NOT_LOCKED, &setObj->tlvArray.tlvInfo[0].tag );
  }
}

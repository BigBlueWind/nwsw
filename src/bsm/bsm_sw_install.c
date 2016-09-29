/**
 * @file    bsm_sw_install.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions for software Install procedure at BSM.
 *
 * @author:  Ankur Agrawal
 * @date:    28-Oct-2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param   configInstallReq  config install request received from PRM
 * @param   bsError           error value
 * @param   errReason         error string
 * @details This function sends MSG_BSM_PRM_CONFIG_INSTALL_FAIL message to PRM module.
 */
static void createAndSendConfigInstallFail(
  IN MsgPrmBsmConfigInstallReq  *configInstallReq,
  IN BsErrorValue                bsError,
  IN UINT8                      *errReason
  )
{
  MsgBsmPrmConfigInstallFail   installFail;
  clearBytes( &installFail, sizeof(MsgBsmPrmConfigInstallFail) );

  installFail.errorDesc.errorNum = bsError;
  snprintf( installFail.configFilePath, strlen(configInstallReq->configFilePath)+1, "%s", configInstallReq->configFilePath );
  snprintf( installFail.filename, strlen(configInstallReq->filename)+1, "%s", configInstallReq->filename );
  snprintf( installFail.errorDesc.failReason, sizeof(String256), "%s", errReason);
  createAndSendMsg( MSG_BSM_PRM_CONFIG_INSTALL_FAIL, (UINT8*)&installFail, sizeof(MsgBsmPrmConfigInstallFail));
}

/**
 * @param   configInstallReq config install request
 * @details This function sends MSG_BSM_PRM_CONFIG_INSTALL_SUCC message to PRM module.
 */
static void createAndSendConfigInstallSucc(
  IN MsgPrmBsmConfigInstallReq  *configInstallReq
  )
{
  MsgBsmPrmConfigInstallSucc   installSucc;
  clearBytes( &installSucc, sizeof(MsgBsmPrmConfigInstallSucc) );
  snprintf( installSucc.configFilePath, strlen(configInstallReq->configFilePath)+1, "%s", configInstallReq->configFilePath );
  snprintf( installSucc.filename, strlen(configInstallReq->filename)+1, "%s", configInstallReq->filename );
  createAndSendMsg( MSG_BSM_PRM_CONFIG_INSTALL_SUCC, (UINT8*)&installSucc, sizeof(MsgBsmPrmConfigInstallSucc));
}

/**
 * @param   msg        Message received from PRM
 * @param   bsmContext BSM module information.
 * @details This function handles SW_INSTALL_CMPL message from PRM module,
 *          MSG_BSM_SIH_NOTI_IND with id=ALARM_ID_SW_INSTALL_SUCCESS will be posted to SIH.
 */
inline BsmProceduralState bsmProcSwInstallSuccInAllStates(
  IN Msg        *msg,
  IO BsmContext *bsmContext
  )
{
  MsgPrmBsmSwInstallSucc  *succMsg = (MsgPrmBsmSwInstallSucc*)msg->data;
  sendEventSwInstallSuccess( succMsg->filename, succMsg->version );
  return bsmContext->bsmProceduralState;
}
/**
 * @param   msg        Message received from PRM
 * @param   bsmContext BSM module information.
 * @details This function handles SW_INSTALL_FAIL message from PRM module.
 *          MSG_BSM_SIH_NOTI_IND with id=ALARM_ID_SW_INSTALL_FAIL will be posted to SIH.
 */
inline BsmProceduralState bsmProcSwInstallFailInAllStates(
  IN Msg        *msg,
  IO BsmContext *bsmContext
  )
{
  MsgPrmBsmSwInstallFail  *failMsg = (MsgPrmBsmSwInstallFail*)msg->data;
  sendEventSwInstallFailed( failMsg->filename, errorCodeStr(failMsg->error) );

  return bsmContext->bsmProceduralState;
}

/**
 * @param   msg        Message received from PRM
 * @param   bsmContext BSM module information.
 * @details This function handles MSG_PRM_BSM_CONFIG_INSTALL_REQ message from PRM module and
 *          reads configuration file and does sanity checks.
 */
inline BsmProceduralState bsmProcConfigInstallReq(
  IN Msg        *msg,
  IO BsmContext *bsmContext
  )
{
  String512   cmdStr;

  EnodebConfig   *enodebConfig = (EnodebConfig*)getFirstRowOfTable(ENODEBCONFIG);
  UINT32    currEnodebId          = enodebConfig->enodebId;
  TBOOL     currEnodebRegAccepted = enodebConfig->enodebRegistrationAccepted;

  clearBytes( cmdStr, sizeof(String512) );

  MsgPrmBsmConfigInstallReq  *configInstallReq = (MsgPrmBsmConfigInstallReq*)msg->data;

  /* Flush all configuration tables */
  emptyOprConfigTables();

  /* Fill the operator configuration tables by reading the new config file */
  if( FALSE EQ bsmReadEnbUserRunningConfigValues( &configInstallReq->configFilePath[0], &configInstallReq->filename[0] ))
  {
    bsmReadEnbUserRunningConfigValues( &bsmContext->configDir[0], "enb_user_running.cfg" );
    revertBackEnodeBIdAndRegAcceptedInConfigTable( currEnodebId, currEnodebRegAccepted, enodebConfig );
    createAndSendConfigInstallFail(configInstallReq, BS_ERR_CONFIG_FILE_INVALID_SYNTAX, "Configuration file read failed");
    return bsmContext->bsmProceduralState;
  }
  revertBackEnodeBIdAndRegAcceptedInConfigTable( currEnodebId, currEnodebRegAccepted, enodebConfig );

  /* Do Consistency check */
   if ( TRUE EQ doConsistencyCheck() )
   {
     /* If consistency checks succeeds, copy the new config file to runningDir configuration folder,
      * and rename it to enb_user_config.cfg
      * copy enb_user_config.cfg to enb_user_running.cfg
      * Send CONFIG_INSTALL_SUCC */
     snprintf( cmdStr,  sizeof(String512), "cp -f %s/%s %s/enb_user_config.cfg", configInstallReq->configFilePath, configInstallReq->filename,
         bsmContext->configDir  );

     if( 0 NOTEQ system(cmdStr) )
     {
       BS_ERROR_SYSTEM_COMMAND_FAILED(cmdStr);
       /* Read the old config file and send CONFIG_INSTALL_FAIL */
       bsmReadEnbUserRunningConfigValues( &bsmContext->configDir[0], "enb_user_running.cfg" );
       createAndSendConfigInstallFail(configInstallReq, BS_ERR_FILE_NOT_FOUND, "Saving New Config values failed");
       return bsmContext->bsmProceduralState;
     }

     clearBytes( cmdStr, sizeof(String512) );
     snprintf( cmdStr,  sizeof(String512), "cp -f %s/%s %s/enb_user_running.cfg", configInstallReq->configFilePath, configInstallReq->filename,
           bsmContext->configDir  );
     if( 0 NOTEQ system(cmdStr) )
     {
       BS_ERROR_SYSTEM_COMMAND_FAILED(cmdStr);
     }

     createAndSendConfigInstallSucc(configInstallReq);

     /* send notification to DEH that configuration file is updated.
      * DEH will take necessary actions on that.*/
     createAndSendMsg( MSG_BSM_DEH_RH_RECONFIG_IND, NULL, 0);
     /* Send indication to SIH Module after the configuration change */
     if ( OAMIF_SIH NOTEQ configInstallReq->cmdRef.origin )
     {
       /* In case of CIH or WIH Set Object request */
       sendEventConfigurationChanged( configInstallReq->cmdRef.origin );
     }
   }
   else
   {
     /* If consistency check fails,  read the old config file and send CONFIG_INSTALL_FAIL */
     bsmReadEnbUserRunningConfigValues( &bsmContext->configDir[0], "enb_user_running.cfg" );
     createAndSendConfigInstallFail(configInstallReq, BS_ERR_CONSISTENCY_CHECK_FAILED, "Consistency check failed at BSM module");
   }
  return bsmContext->bsmProceduralState;
}

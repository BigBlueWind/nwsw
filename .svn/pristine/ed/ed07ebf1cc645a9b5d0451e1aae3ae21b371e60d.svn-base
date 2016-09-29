/**
 * @file    bsm_cmd_reboot.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file contains functions related to reboot procedure.
 *
 * @author:  Shweta Polepally
 * @date:    Sep 2010
 * @details: Initial Version
 **/

#include "bsm.h"

/**
 * @param   fullFilePath                 Full File Path name.
 * @param   dateAndTimeInNextStartConfig Date And Time in Next Start Configuration file.
 * @details This function will set the value - enabled/disabled.
 */
TBOOL readAndGetDateAndTimeInNextStartConfig(
  IN UINT8  *fullFilePath,
  IO UINT8  *dateAndTimeInNextStartConfig
  )
{
  TBOOL                      isError = FALSE;
  EnbRegistrationConfig      config;
  clearBytes( &config, sizeof(EnbRegistrationConfig) );

  readFromFileEnbRegistrationConfigTable( fullFilePath, &config, &isError );
  if ( isError EQ TRUE )
  {
    return FALSE;
  }
  else
  {
    snprintf( dateAndTimeInNextStartConfig, strlen(config.initialLocalTime)+1, "%s", (char*)&config.initialLocalTime );
    return TRUE;
  }
}

/**
 * @param   filePath   Full File Path name.
 * @details This function will write date and time in Next Start Config file.
 */
void writeDateAndTimeInNextStartConfig(
  IN UINT8  *filePath
  )
{
  UINT8      dateAndTimeInNextStartConfig[MAX_SIZE_ENBREGISTRATIONCONFIG_INITIALLOCALTIME+1];
  String256  cmd;

  clearBytes( cmd, sizeof(String256));
  clearBytes( dateAndTimeInNextStartConfig,MAX_SIZE_ENBREGISTRATIONCONFIG_INITIALLOCALTIME+1 );

  FileName filenameWithPath;
  FileName tempFilenameWithPath;
  clearBytes( filenameWithPath, sizeof(FileName));
  clearBytes( tempFilenameWithPath, sizeof(FileName));

  snprintf( filenameWithPath, sizeof(FileName), "%s/enb_user_config.cfg", filePath );
  snprintf( tempFilenameWithPath, sizeof(FileName), "%s/tempEnbUserConfig.cfg", filePath );

  if ( FALSE EQ readAndGetDateAndTimeInNextStartConfig( filenameWithPath, dateAndTimeInNextStartConfig ))
  {
    BS_ERROR_CONFIG_READ_FAILED();
    return;
  }

  EnbRegistrationConfig   *rowOfCfgTable = (EnbRegistrationConfig*)getFirstRowOfTable(ENBREGISTRATIONCONFIG);
  snprintf( cmd, sizeof(String256), "sed 's/%s/%s/' %s>%s",
                                        rowOfCfgTable->initialLocalTime,
                                        dateAndTimeInNextStartConfig,
                                        filenameWithPath,
                                        tempFilenameWithPath
                                        );

  if ( 0 NOTEQ system( cmd ))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED( cmd );
    return;
  }

  clearBytes( cmd, sizeof(String256));
  snprintf( cmd, sizeof(String256), "mv %s %s", tempFilenameWithPath, filenameWithPath );

  if ( 0 NOTEQ system(cmd))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED( cmd );
    return;
  }
}

/**
 * @param setObj     Set Object Request Message.
 * @param bsmContext BSM module related information.
 * @details  This function will ask PRM to carry out reboot procedure by sending
 *           REBOOT_REQ message.
 */
void bsmDoReboot(
  IN SetObjReq   *setObj,
  IO BsmContext  *bsmContext
  )
{
  if ( TRUE EQ isEnodeBLocked() )
  {
    bsmFillAndSendSetObjSucc( setObj );
    /* Disable Operational State, so that we can reject any other user action
     * Set the procedure as Reboot */
    setBsmProceduralState( bsmContext, BS_WAIT_REBOOT_CMPL );
    /* Write the date and time values in the persistent memory - enb_user_config*/
    writeDateAndTimeInNextStartConfig( &bsmContext->configDir[0] );
    createAndSendMsg( MSG_BSM_PRM_REBOOT_REQ, NULL, 0 );
  }
  else
  {
    BS_ERROR_ENODEB_NOT_LOCKED();
    bsmFillAndSendSetObjFail( setObj, BS_ERR_ENODEB_NOT_LOCKED, &setObj->tlvArray.tlvInfo[0].tag );
  }
}

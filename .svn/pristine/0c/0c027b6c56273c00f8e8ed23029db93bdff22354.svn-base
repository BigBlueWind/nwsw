/**
 * @file    prm_sw_install.c
 * @brief   This file contains functions related to Software Installation Procedure.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author   Ankur Agrawal / Shweta Polepally
 * @date     3-Nov-2010
 */

#include "prm.h"

/**
 * @param    versionInfo  Version information to be stored in sw_info status table.
 * @param    rootDir      root directory of /picobs/rel/(inst_A/inst_B).
 * @details This functions will check which version is not the running version and update
 *          that row with the new version information.
 */
static void prmUpdateSwPackageStatusTableAfterEnbSwInstall(
  IO SwPackageInfo    *swPackageInfo,
  IN UINT8            *rootDir
  )
{
  UINT8          i = 0;
  SwPackageStatus  *rowOfSwPackageStatus = (SwPackageStatus*)getFirstRowOfTable(SWPACKAGESTATUS);

  for ( i=0; i < MAX_ROWS_IN_SWPACKAGESTATUS_TABLE; i++ )
  {
    if( (SWTYPE_ENB_L2L3_SW EQ rowOfSwPackageStatus->swType ) &&( FALSE EQ rowOfSwPackageStatus->isRunning ))
    {
      strncpy( rowOfSwPackageStatus->version, swPackageInfo->versionInfo.version, strlen(swPackageInfo->versionInfo.version) );
      strncpy( rowOfSwPackageStatus->builtOn, swPackageInfo->versionInfo.builtOn, strlen(swPackageInfo->versionInfo.builtOn) );
      strncpy( rowOfSwPackageStatus->packageDir, rootDir, strlen(rootDir) );
    }
    getNextRowOfTable( rowOfSwPackageStatus );
  }
}

/**
 * @param    version  RH Version which is being installed
 * @details This functions will update the non-running version of RH in SwPackageStatus table
 */
static void prmUpdateSwPackageStatusTableAfterRhSwInstall(
  IO  UINT8  *version
  )
{
  UINT8          i = 0;
  SwPackageStatus  *rowOfSwPackageStatus = (SwPackageStatus*)getFirstRowOfTable(SWPACKAGESTATUS);

  for ( i=0; i < MAX_ROWS_IN_SWPACKAGESTATUS_TABLE; i++ )
  {
    if( (SWTYPE_RH_SW EQ rowOfSwPackageStatus->swType ) &&( FALSE EQ rowOfSwPackageStatus->isRunning ))
    {
      snprintf( rowOfSwPackageStatus->version,MAX_SIZE_SWPACKAGESTATUS_VERSION +1,"%s",version);
    }
    getNextRowOfTable( rowOfSwPackageStatus );
  }
}

/**
 * @param   bsError error value
 * @details This functions will convert error numbers to BsErrorValue
 */
static BsErrorValue prmGetBsErrorFromReturnCodeOfInstaller(
  IO UINT32 bsError
  )
{
  switch(bsError)
  {
    case 0: return BS_ERR_INSTALL_INVALID_INSTALLER_SCRIPT;
    case 1: return BS_ERR_INSTALL_PKG_UNPACKING_FAILED;
    case 2: return BS_ERR_INSTALL_VERIFICATION_FAILED;
    case 4: return BS_ERR_INSTALL_FLASH_ACCESS_FAILED;
    case 5: return BS_ERR_INSTALL_FILE_NOT_FOUND;
    case 6: return BS_ERR_INSTALL_UNSUPPORTED_FILE_FORMAT;
    case 7: return BS_ERR_INSTALL_FILE_SIZE_EXCEEDED;
    case 8: return BS_ERR_INSTALL_FTP_SERVER_NOT_RUNNING;
  }
  return 0;
}

/**
 * @param   dataType Data type
 * @details This functions will convert dataType ENUM to string
 */
static char *prmGetDataType(
  UINT8 dataType
  )
{
  switch(dataType)
  {
    case BS_TYPE_SINT8:           return "SINT8";
    case BS_TYPE_SINT16:          return "SINT16";
    case BS_TYPE_SINT32:          return "SINT32";
    case BS_TYPE_UINT8:           return "UINT8";
    case BS_TYPE_UINT16:          return "UINT16";
    case BS_TYPE_UINT32:          return "UINT32";
    case BS_TYPE_STRING:          return "STRING";
    case BS_TYPE_BOOL:            return "BOOL";
    case BS_TYPE_MODID:           return "MODID";
    case BS_TYPE_MSGID:           return "MSGID";
    case BS_TYPE_ENUM:            return "ENUM";
    case BS_TYPE_IPV4ADDRESS:     return "IPV4ADDRESS";
    case BS_TYPE_MACADDRESS:      return "MACADDRESS";
    case BS_TYPE_FILENAME:        return "FILENAME";
    case BS_TYPE_LOCALTIME:       return "LOCALTIME";
    case BS_TYPE_OFFSETTIME:      return "OFFSETTIME";
    case BS_TYPE_SWVERSION:       return "SWVERSION";
    case BS_TYPE_MCC:             return "MCC";
    case BS_TYPE_MNC:             return "MNC";
    case BS_TYPE_LATITUDE:        return "LATITUDE";
    case BS_TYPE_LONGITUDE:       return "LONGITUDE";
    case BS_TYPE_ENODEBNAME:      return "ENODEBNAME";
    case BS_TYPE_PROFILENAME:     return "PROFILENAME";
    case BS_TYPE_COUNTERID:       return "COUNTERID";
  }
  return "INVALID_TYPE";
}
/**
 * @param    prmContext   PRM context
 * @param    version      version which is installed
 * @details This functions will fill and send MSG_PRM_BSM_SW_INSTALL_SUCC to BSM
 */
static void prmCreateAndSendSwInstallSucc(
  IN PrmContext     *prmContext,
  IN UINT8          *version
  )
{
  MsgPrmBsmSwInstallSucc   swInstallCmpl;

  clearBytes( &swInstallCmpl, sizeof(MsgPrmBsmSwInstallSucc));
  snprintf(swInstallCmpl.filename, strlen(prmContext->swinstall.filename)+1, "%s", prmContext->swinstall.filename);
  snprintf(swInstallCmpl.version, strlen(version)+1, "%s", version);
  createAndSendMsg( MSG_PRM_BSM_SW_INSTALL_SUCC, (UINT8*)&swInstallCmpl, sizeof(swInstallCmpl) );
}

/**
 * @param    setObj   Set Object request message
 * @details This functions will fill and send MSG_PRM_BSM_SW_INSTALL_RSP to BSM
 */
static void prmCreateAndSendSwInstallAccepted(
  IN SetObjReq  *setObj
  )
{
  MsgPrmBsmSwInstallRsp       setObjSucc;
  EncBuf                      encBuf;

  clearBytes( &setObjSucc, sizeof(MsgPrmBsmSwInstallRsp));
  initEncBuf( &encBuf );

  setObjSucc.cmdRef = setObj->cmdRef;
  setObjSucc.tlvArray =setObj->tlvArray;
  encSetObjRspSuccMsg(&setObjSucc,&encBuf);
  createAndSendMsg( MSG_PRM_BSM_SW_INSTALL_ACCEPTED, (UINT8*)&encBuf.value, encBuf.len );
  prmPrintTimeStamp();
}

/**
 * @param    setObj    Set Object request message
 * @param    error     Error Value
 * @param    errInfo   Error Information string
 * @details This functions will fill and send MSG_PRM_BSM_SW_INSTALL_REJ to BSM
 */
static void prmCreateAndSendSwInstallRej(
  IN SetObjReq 	   *setObj,
  IN BsErrorValue   error,
  IN UINT8         *errInfo
  )
{
  MsgPrmBsmSwInstallRej setObjFail;
  EncBuf		encBuf;

  clearBytes( &setObjFail, sizeof(MsgPrmBsmSwInstallRej));
  initEncBuf( &encBuf );

  setObjFail.cmdRef = setObj->cmdRef;
  setObjFail.setErrRsp.errRspCode = error;
  snprintf( (UINT8*)&setObjFail.setErrRsp.errorInfo, (MAX_TLV_VALUE_IN_BYTES+1), "%s", errInfo );
  setObjFail.setErrRsp.failedTagInfo = setObj->tlvArray.tlvInfo[0].tag;
  setObjFail.tlvArray = setObj->tlvArray;

  encSetObjRspFailMsg(&setObjFail,&encBuf);
  createAndSendMsg( MSG_PRM_BSM_SW_INSTALL_REJ, (UINT8*)&encBuf.value, encBuf.len );

}

/**
 * @param    error        Error Value
 * @param    prmContext   PRM context
 * @details This functions will fill and send MSG_PRM_BSM_SW_INSTALL_FAIL to BSM
 */
void prmCreateAndSendSwInstallFail(
  IN BsErrorValue                error,
  IO PrmContext                 *prmContext
  )
{
  MsgPrmBsmSwInstallFail       swInstallFail;

  clearBytes( &swInstallFail, sizeof(MsgPrmBsmSwInstallFail));
  snprintf(swInstallFail.filename, strlen(prmContext->swinstall.filename)+1, "%s", prmContext->swinstall.filename);
  swInstallFail.error = error;
  createAndSendMsg( MSG_PRM_BSM_SW_INSTALL_FAIL, (UINT8*)&swInstallFail, sizeof(MsgPrmBsmSwInstallFail));
}

/**
 * @param    fileName   This is the file name of SW installation request
 * @param    status     status of software installation
 * @param    errDetail  Error information string
 * @details  This function update software installation status table.
 */
void prmUpdateSwInstallStatusTable(
  IN UINT8		 *fileName,
  IN TBOOL                status,
  IN UINT8		 *errDetail
  )
{
  UINT8 i;
  PrmContext *prmContext = getPrmContext();
  SwInstallHistory  *rowOfSwInstallStatus = (SwInstallHistory*)getFirstRowOfTable(SWINSTALLHISTORY);

  prmContext->indexOfSwInstallHistory = (prmContext->indexOfSwInstallHistory) % MAX_ROWS_IN_SWINSTALLHISTORY_TABLE;
  for ( i=0; i < prmContext->indexOfSwInstallHistory; i++ )
  {
        getNextRowOfTable(rowOfSwInstallStatus);
  }
  snprintf( (UINT8*)&rowOfSwInstallStatus->filename, (strlen(fileName)+1), "%s", fileName );
  rowOfSwInstallStatus->wasSuccessful = status;
  snprintf( (UINT8*)&rowOfSwInstallStatus->installErrorDetail, (strlen(errDetail)+1), "%s", errDetail);
  rowOfSwInstallStatus->index = (prmContext->indexOfSwInstallHistory)+1;

  prmContext->indexOfSwInstallHistory++;
  prmContext->noOfEntriesInSwInstallHistory++;
  if(prmContext->noOfEntriesInSwInstallHistory > MAX_ROWS_IN_SWINSTALLHISTORY_TABLE)
  {
    prmContext->noOfEntriesInSwInstallHistory = MAX_ROWS_IN_SWINSTALLHISTORY_TABLE;
  }
}
/**
 * @param    msg          Message received from BSM
 * @param    prmContext   PRM context Information
 * @details  This function will clear SwInstallHistory Table.
 */

void prmProcDeleteSwInstallHistoryTableReq(
  IN Msg         *msg,
  IO PrmContext  *prmContext
  )
{
  MsgBsmPrmSetObjReq setObjReq ;
  //SwInstallHistory  *rowOfSwInstallHistory = (SwInstallHistory*)getFirstRowOfTable(SWINSTALLHISTORY);
  clearBytes(&setObjReq, sizeof(MsgBsmPrmSetObjReq)) ;

  decSetObjReqMsg( (UINT8*)msg->data, msg->len, &setObjReq );

  logConfig("total number of rows present in swInstallHistory : %d",prmContext->noOfEntriesInSwInstallHistory);//logInfo is disabled 
  prmInitSwInstallHistoryTable();
  prmCreateAndSendSetObjSucc( &setObjReq );

}
/**
 * @param    setObjReq    setObjectRequest        
 * @details This functions will fill and send MSG_PRM_BSM_SET_OBJ_SUCC to BSM
 */

void prmCreateAndSendSetObjSucc(
  IN MsgBsmPrmSetObjReq *setObjReq
  )
{
  SetObjRspSucc setObjRspSucc;
  EncBuf encBuf;
  clearBytes( &setObjRspSucc, sizeof(MsgPrmBsmSwInstallRsp));
  initEncBuf( &encBuf );

  setObjRspSucc.cmdRef = setObjReq->cmdRef;
  setObjRspSucc.tlvArray =setObjReq->tlvArray;
  encSetObjRspSuccMsg(&setObjRspSucc,&encBuf);
  createAndSendMsg(MSG_PRM_BSM_SET_OBJ_SUCC , (UINT8*)&encBuf.value, encBuf.len );

}


/**
 * @param    setObj     Set Object request
 * @param    swInstall  Software installation information to be filled in this structure.
 * @details  This function will decode installation information from TLV format
 */
static void prmDecSwInstallCmd(
  IO SetObjReq   *setObj,
  IO SwInstall   *swInstall
  )
{
  GenericTable     *tableFunc = getTableFunctions(SWINSTALL);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, swInstall );
  }
}

/**
 * @param    readFromFile  Information read from /PICOBS/log/install_success or /PICOBS/log/install_failure
 * @param    bsError       error value
 * @param    prmContext    PRM context
 * @details  This function will read the install_success or install_failed present in PICOBS_HOME/log folder.
 */
static TBOOL prmIsOutputOfInstallerScriptSucc(
  IO  UINT8       *readFromFile,
  IO  UINT32      *bsError,
  IO  PrmContext  *prmContext
  )
{
  String150 cmdStr;
  FILE 	    *fp;

  /* Checking whether script is executed successfully or not
   * Successful case: PICOBS_HOME/log/install_success must be present
   * Failed case : PICOBS_HOME/log/install_failed will be present or no file will be present */
  clearBytes(cmdStr,sizeof(String150));
  snprintf(cmdStr, sizeof(String150),"%s/log/install_success", prmContext->picoBsHomePath);
  if((fp = fopen (cmdStr, "r" ) ) EQ NULL)
  {
    /* PICOBS_HOME/log/install_succes file is not present
     * Now trying to open PICOBS_HOME/log/install_failed */
    clearBytes(cmdStr,sizeof(String150));
    snprintf(cmdStr, sizeof(String150),"%s/log/install_failed", prmContext->picoBsHomePath);
    if((fp = fopen (cmdStr, "r" ) ) EQ NULL)
    {
      /* PICOBS_HOME/log/install_failed file also not present i.e. installer is not valid */
      *bsError = 0;
      if ( INSTALLATIONTYPE_L2L3RELEASE EQ prmContext->swinstall.installationType)
        snprintf(readFromFile, sizeof(String256), "Invalid Installer file encountered during full release installation ");
      else if ( INSTALLATIONTYPE_L2L3CONFIGURATION EQ prmContext->swinstall.installationType)
        snprintf(readFromFile, sizeof(String256), "Invalid Installer file encountered during configuration file installation ");
      else if ( INSTALLATIONTYPE_L2L3PATCH EQ prmContext->swinstall.installationType)
        snprintf(readFromFile, sizeof(String256), "Invalid Installer file encountered during patch installation ");
      else if ( INSTALLATIONTYPE_RHRELEASE EQ prmContext->swinstall.installationType)
        snprintf(readFromFile, sizeof(String256), "Invalid Installer file encountered during RH-SW installation ");
      else
        BS_ERROR_UNKNOWN_ENUM_VALUE("Unknown value received for installationType ENUM", prmContext->swinstall.installationType);
      return FALSE;
    }
    /* PICOBS_HOME/log/install_failed file is present. i.e. installer is valid and some check has failed */
    fgets(readFromFile,sizeof(String256),fp);
    fscanf (fp, "%d",bsError);
    fclose(fp);
    clearBytes(cmdStr,sizeof(String150));
    /* Deleting PICOBS_HOME/log/install_failed file */
    snprintf(cmdStr, sizeof(String150),"rm -f %s/log/install_failed", prmContext->picoBsHomePath);
    if ( 0 NOTEQ system( cmdStr ))
    {
      BS_ERROR_SYSTEM_COMMAND_FAILED(cmdStr);
    }
    return FALSE;
  }
  /* PICOBS_HOME/log/install_success is present. i.e. installer script has executed successfully. */
  /*  read the RH-SW bin file name from install_success file */
  if(INSTALLATIONTYPE_RHRELEASE EQ prmContext->swinstall.installationType  )
    fgets(readFromFile,sizeof(String256),fp);
  fclose(fp);
  clearBytes(cmdStr,sizeof(String150));
  /* Deleting PICOBS_HOME/log/install_success file */
  snprintf(cmdStr, sizeof(String150), "rm -f %s/log/install_success", prmContext->picoBsHomePath);
  if ( 0 NOTEQ system( cmdStr ))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(cmdStr);
  }
  return TRUE;
}

/**
 * @param    prmContext  PRM context Information
 * @details  This function reads version.cfg and updates non running version info.
 */
static void prmUpdateNonRunningVersionInfo(
  IO PrmContext  *prmContext
  )
{
  TBOOL               isError;
  FileName            versionCfgPath;
  VersionCfgContents  versionCfgContents;
  VersionInfo         newSwVersion;

  clearBytes( &versionCfgContents, sizeof(VersionCfgContents) );
  clearBytes( &newSwVersion,       sizeof(VersionInfo) );
  clearBytes( versionCfgPath,      sizeof(FileName) );

  /* read version.cfg */
  snprintf(versionCfgPath, sizeof(FileName),"%s/config/version.cfg", prmContext->isNotRunning->rootDir );
  readFromFileVersionCfgContentsTable( versionCfgPath, &versionCfgContents, &isError );
  if ( TRUE EQ isError )
  {
    BS_ERROR_CONFIG_READ_FAILED();
  }
  convertVersionCfgContentsToVersionInfo( &versionCfgContents, &newSwVersion );
  prmContext->isNotRunning->versionInfo = newSwVersion;
}

/**
 * @param    installReq          Software install request
 * @param    prmContext          PRM context Information
 * @param    installerPath       path where package installer exists
 * @param    isInstallerDefault  checks whether installer is specified by user or not
 * @details  This function creates the installer path using the installer specified by user or default path
 */
static TBOOL prmGetInstallerPath(
  IO MsgBsmPrmSwInstallReq   *installReq,
  IO PrmContext              *prmContext,
  IO UINT8                   *installerPath,
  IO TBOOL                   *isInstallerDefault
  )
{
  FILE *fp;

  /* Check whether any installer name is specified
   * if yes then take that as installer path else use the default installer */
  if( strlen(prmContext->swinstall.installer) EQ 0)
  {
    /* Installer is not specified so using default installer */
    if ( INSTALLATIONTYPE_L2L3RELEASE EQ prmContext->swinstall.installationType)
      snprintf(installerPath, sizeof(String150), "%s/pkg_installer.sh", prmContext->isRunning->rootDir);
    else if ( INSTALLATIONTYPE_L2L3CONFIGURATION EQ prmContext->swinstall.installationType)
      snprintf(installerPath, sizeof(String150), "%s/config_installer.sh", prmContext->isRunning->rootDir);
    else if ( INSTALLATIONTYPE_L2L3PATCH EQ prmContext->swinstall.installationType)
      snprintf(installerPath, sizeof(String150), "%s/patch_installer.sh", prmContext->isRunning->rootDir);
    else if ( INSTALLATIONTYPE_RHRELEASE EQ prmContext->swinstall.installationType)
      snprintf(installerPath, sizeof(String150), "%s/rh_installer.sh", prmContext->isRunning->rootDir);
    else
      BS_ERROR_UNKNOWN_ENUM_VALUE("Unknown value received for installationType ENUM", prmContext->swinstall.installationType);
    *isInstallerDefault = TRUE;
    return TRUE;
  }
  else
  {
    /* Installer is specified in install request.
     * so checking whether specified installer is present in download directory
     * If present then continue else send reject message to BSM. */
    snprintf(installerPath, sizeof(String150), "%s/download/%s",prmContext->picoBsHomePath, prmContext->swinstall.installer);
    if ( (fp = fopen (installerPath, "r" ) ) EQ NULL )
    {
      if ( INSTALLATIONTYPE_L2L3RELEASE EQ prmContext->swinstall.installationType)
      {
        prmUpdateSwInstallStatusTable( prmContext->swinstall.filename, FALSE, "Specified installer file was not found for full release installation" );
        BS_ERROR_INSTALL_INSTALLER_NOT_FOUND( "Specified installer file was not found for full release installation" );
        prmCreateAndSendSwInstallRej( installReq, BS_ERR_INSTALL_INSTALLER_NOT_FOUND, "Specified installer file was not found for full release installation"  );
      }
      else if ( INSTALLATIONTYPE_L2L3CONFIGURATION EQ prmContext->swinstall.installationType)
      {
        prmUpdateSwInstallStatusTable( prmContext->swinstall.filename, FALSE, "Specified installer file was not found for configuration file installation" );
        BS_ERROR_INSTALL_INSTALLER_NOT_FOUND( "Specified installer file was not found for configuration file installation" );
        prmCreateAndSendSwInstallRej( installReq, BS_ERR_INSTALL_INSTALLER_NOT_FOUND, "Specified installer file was not found for configuration file installation" );
      }
      else if ( INSTALLATIONTYPE_L2L3PATCH EQ prmContext->swinstall.installationType)
      {
        prmUpdateSwInstallStatusTable( prmContext->swinstall.filename, FALSE, "Specified installer file was not found for patch installation" );
        BS_ERROR_INSTALL_INSTALLER_NOT_FOUND( "Specified installer file was not found for patch installation" );
        prmCreateAndSendSwInstallRej( installReq, BS_ERR_INSTALL_INSTALLER_NOT_FOUND, "Specified installer file was not found for patch installation" );
      }
      else if ( INSTALLATIONTYPE_RHRELEASE EQ prmContext->swinstall.installationType)
      {
        prmUpdateSwInstallStatusTable( prmContext->swinstall.filename, FALSE, "Specified installer file was not found for RH-SW installation" );
        BS_ERROR_INSTALL_INSTALLER_NOT_FOUND( "Specified installer file was not found for RH-SW installation" );
        prmCreateAndSendSwInstallRej( installReq, BS_ERR_INSTALL_INSTALLER_NOT_FOUND, "Specified installer file was not found for RH-SW installation" );
      }
      else
      {
        BS_ERROR_UNKNOWN_ENUM_VALUE("Unknown value received for installationType ENUM", prmContext->swinstall.installationType);
      }
      clearBytes(&(prmContext->swinstall), sizeof(SwInstall));
      prmContext->isInstallInProgress = FALSE;
      return FALSE;
    }
    *isInstallerDefault = FALSE;
    fclose(fp);
    return TRUE;
  }
}

/**
 * @details  This function will creates /tmp/enbConfigVerificationTemplate.txt which is used
 *           for configuration file verification by config_installer.sh script.
 */
void prmCreateEnbConfigVerificationTemplate( void )
{
  UINT8 index,colId;
  FILE *fp;
  if(NULL EQ (fp = fopen("/tmp/enbConfigVerificationTemplate.txt", "w+")))
  {
    BS_ERROR_FILE_OPEN_FAILED("/tmp/enbConfigVerificationTemplate.txt ");
    return;
  }
  fprintf(fp,"%d\n",MAX_NUM_OPR_CFG_TABLES);
  fprintf(fp,"\n");
  for(index=0; index<MAX_NUM_OPR_CFG_TABLES; index++)
  {
    PrmOamTableInfo *oprCfgInfo = prmGetOprCfgTableInfo(index);
    fprintf(fp, "Table:%s,%d,%d\n",oprCfgInfo->tableName, oprCfgInfo->noOfRows, oprCfgInfo->noOfColumns);
    for(colId=1; colId<=oprCfgInfo->noOfColumns; colId++)
    {
      fprintf(fp,"%s,%s,%d,%d,%d,%s\n",oprCfgInfo->colInfo[colId].colName, prmGetDataType(oprCfgInfo->colInfo[colId].dataType), \
                                 oprCfgInfo->colInfo[colId].colMinVal, oprCfgInfo->colInfo[colId].colMaxVal, \
                                 oprCfgInfo->colInfo[colId].isSpecialValuePresent, oprCfgInfo->colInfo[colId].specialValue);
    }
    fprintf(fp,"\n");
  }
  fclose(fp);

}

/**
 * @param    prmContext    : PRM context Information
 * @param    installReq    : swInstall request
 * @details  This function runs the installer and checks output of script.
 */
static TBOOL prmRunInstaller(
  IO PrmContext              *prmContext,
  IO MsgBsmPrmSwInstallReq   *installReq
  )
{
  String512     commandStr;
  String256     scriptReturnedString;
  UINT32        errorRetunedByScript = 0;
  String150     installerPath;
  BsErrorValue  error;
  TBOOL         isInstallerDefault = FALSE;

  clearBytes(scriptReturnedString, sizeof(String256));
  clearBytes(commandStr,sizeof(String512));
  clearBytes(installerPath,sizeof(String150));

  /* Get installer path from install request,if specified else get default installer path */
  if( FALSE EQ prmGetInstallerPath(installReq, prmContext, installerPath, &isInstallerDefault ))
  {
    return FALSE;
  }
  /* Creating string to run installer for first level of testing
   * "installer.sh <level> <fileName> <nonRunningRootDir><runningVersionInfo><runningRootDir>"*/
  snprintf(commandStr, sizeof(String512), "%s %s %s %s %s FirstLevel",
                                            installerPath,
                                            prmContext->swinstall.filename,
                                            prmContext->isNotRunning->rootDir,
                                            prmContext->isRunning->versionInfo.version,
                                            prmContext->isRunning->rootDir);
  /* Run installer script */
  if ( 0 NOTEQ system(commandStr))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(commandStr);
  }
  /* Check output after running installer
   * if script executed successfully then we will continue with second level of check
   * else update swInstallHistory table and send reject to BSM. */
  if(FALSE EQ prmIsOutputOfInstallerScriptSucc( scriptReturnedString, &errorRetunedByScript, prmContext ) )
  {
    scriptReturnedString[strlen(scriptReturnedString)-1] = '\0';
    error = prmGetBsErrorFromReturnCodeOfInstaller(errorRetunedByScript);
    prmUpdateSwInstallStatusTable( prmContext->swinstall.filename, FALSE, scriptReturnedString );
    BS_ERROR_INSTALL_INSTALLER_FAILED(scriptReturnedString);
    prmCreateAndSendSwInstallRej( installReq, error, scriptReturnedString  );
    clearBytes(&(prmContext->swinstall), sizeof(SwInstall));
    prmContext->isInstallInProgress = FALSE;
    return FALSE;
  }
  else
  {
    /* send SW_INSTALL_ACCEPTED to BSM */
    prmCreateAndSendSwInstallAccepted(installReq);

    /* If installation type is INSTALLATIONTYPE_L2L3CONFIGURATION then
     * create /tmp/enbConfigVerificationTemplate.txt which is used for configuration file verification */
    if(prmContext->swinstall.installationType EQ INSTALLATIONTYPE_L2L3CONFIGURATION)
    {
      prmCreateEnbConfigVerificationTemplate();
    }

    clearBytes(commandStr,sizeof(String512));
    /* Creating string to run installer for send level of testing
     * "installer.sh <level> <fileName> <nonRunningRootDir><runningVersionInfo><runningRootDir>"*/
    snprintf(commandStr, sizeof(String512), "%s %s %s %s %s SecondLevel", installerPath, prmContext->swinstall.filename, prmContext->isNotRunning->rootDir, prmContext->isRunning->versionInfo.version, prmContext->isRunning->rootDir);
    if ( 0 NOTEQ system(commandStr))
    {
      BS_ERROR_SYSTEM_COMMAND_FAILED(commandStr);
    }
    /* Check output after running installer
     * if script executed successfully then we will remove installer(if specified in install request)
     * else update swInstallHistory table and send reject to BSM. */
    if(FALSE EQ prmIsOutputOfInstallerScriptSucc( scriptReturnedString, &errorRetunedByScript, prmContext ) )
    {
      scriptReturnedString[strlen(scriptReturnedString)-1] = '\0';
      error = prmGetBsErrorFromReturnCodeOfInstaller(errorRetunedByScript);
      prmUpdateSwInstallStatusTable( prmContext->swinstall.filename, FALSE, scriptReturnedString );
      BS_ERROR_INSTALL_INSTALLER_FAILED(scriptReturnedString);
      prmCreateAndSendSwInstallFail( error, prmContext );
      clearBytes(&(prmContext->swinstall), sizeof(SwInstall));
      prmContext->isInstallInProgress = FALSE;
      return FALSE;
    }
    else
    {
      scriptReturnedString[strlen(scriptReturnedString)-1] = '\0';
      snprintf( prmContext->rhSwInstallBinFile, sizeof(FileName), "%s", scriptReturnedString);

      /* If installer is not default then we will remove specified installer */
      if ( FALSE EQ isInstallerDefault)
      {
        clearBytes(commandStr,sizeof(String512));
        snprintf(commandStr, sizeof(String512), "rm -rf %s", installerPath );
        if ( 0 NOTEQ system(commandStr) )
        {
          BS_ERROR_SYSTEM_COMMAND_FAILED(commandStr);
        }
      }
    }
  }
  return TRUE;
}


/**
 * @param    msgIn      : Message received from BSM
 * @param    prmContext : PRM context Information
 * @details  This function will handle SW install request from BSM.
 */
void prmProcSwConfigInstallFail(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  )
{
  MsgBsmPrmConfigInstallFail  *configInstallFail = (MsgBsmPrmConfigInstallFail*)msgIn->data;
  prmCreateAndSendSwInstallFail(configInstallFail->errorDesc.errorNum,prmContext);
  prmUpdateSwInstallStatusTable( prmContext->swinstall.filename, FALSE, configInstallFail->errorDesc.failReason );
  BS_ERROR_CONFIG_FILE_INVALID_SYNTAX(configInstallFail->errorDesc.failReason);
  clearBytes(&(prmContext->swinstall), sizeof(SwInstall));
  prmContext->isInstallInProgress = FALSE;
}

/**
 * @param    msgIn      : Message received from BSM
 * @param    prmContext : PRM context Information
 * @details  This function will handle RH-SW install failure from DEH.
 */
void prmProcRhSwInstallFail(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  )
{
  String256  str;
  MsgDehPrmRhSwInstallFail  *rhSwInstallFail = (MsgDehPrmRhSwInstallFail*)msgIn->data;

  /* Stop the timer */
  stopPrmProcedureTimer(TIMERNAME_WAITRHSWINSTALLRSP);

  /* Delete the .bin file */
  clearBytes(str, sizeof(String256) );
  snprintf(str, sizeof(String256), "rm -rf %s",prmContext->rhSwInstallBinFile);
  if(0 NOTEQ system(str))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(str);
  }

  prmCreateAndSendSwInstallFail(rhSwInstallFail->errorDesc.errorNum,prmContext);
  prmUpdateSwInstallStatusTable( prmContext->swinstall.filename, FALSE, rhSwInstallFail->errorDesc.failReason );
  clearBytes(&(prmContext->swinstall), sizeof(SwInstall));
  prmContext->isInstallInProgress = FALSE;
}

/**
 * @param    msgIn      : Message received from BSM
 * @param    prmContext : PRM context Information
 * @details  This function will handle RH-SW install success from DEH.
 */
void prmProcSwConfigInstallSucc(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  )
{
  String150 str;
  clearBytes(str, sizeof(String150) );

  MsgBsmPrmConfigInstallSucc  *configInstallSucc = (MsgBsmPrmConfigInstallSucc*)msgIn->data;
  snprintf(str, sizeof(String150), "rm -rf %s/%s",configInstallSucc->configFilePath, configInstallSucc->filename);
  /* wait until BSM copies this file */
  sleep(5);
  if(0 NOTEQ system(str))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(str);
  }
  prmCreateAndSendSwInstallSucc(prmContext, &prmContext->isRunning->versionInfo.version[0]);
  prmUpdateSwInstallStatusTable( prmContext->swinstall.filename, TRUE, "Configuration file successfully installed" );
  clearBytes(&(prmContext->swinstall), sizeof(SwInstall));
  prmContext->isInstallInProgress = FALSE;
}

/**
 * @param    msgIn      : Message received from BSM
 * @param    prmContext : PRM context Information
 * @details  This function will handle SW install request from BSM.
 */
void prmProcRhSwInstallSucc(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  )
{
  String256 str;

  MsgDehPrmRhSwInstallSucc  *rhSwInstallSucc = (MsgDehPrmRhSwInstallSucc*)msgIn->data;

  /* Stop the timer */
  stopPrmProcedureTimer(TIMERNAME_WAITRHSWINSTALLRSP);

  /* wait until BSM copies this file */
  sleep(5);

  /* Delete the .bin file */
  clearBytes(str, sizeof(String256) );
  snprintf(str, sizeof(String256), "rm -rf %s",prmContext->rhSwInstallBinFile);
  if(0 NOTEQ system(str))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(str);
  }

  /* Delete the .tgz file */
  clearBytes(str, sizeof(String256) );
  snprintf( str, MAX_SIZE_SWINSTALL_FILENAME + 1, "rm -rf %s/download/%s", prmContext->picoBsHomePath, prmContext->swinstall.filename );
  if(0 NOTEQ system(str))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(str);
  }

  prmUpdateSwPackageStatusTableAfterRhSwInstall(&(rhSwInstallSucc->version[0]));
  prmWriteSwPackageStatusToPersistentMemory();
  prmCreateAndSendSwInstallSucc(prmContext, &(rhSwInstallSucc->version[0]));
  prmUpdateSwInstallStatusTable( prmContext->swinstall.filename, TRUE, "RH-SW successfully installed" );
  clearBytes(&(prmContext->swinstall), sizeof(SwInstall));
  prmContext->isInstallInProgress = FALSE;
}

/**
 * @param    prmContext : PRM context Information
 * @details  This function creates back-up of running package at /tmp/patch location
 */
static void prmCreateBackupOfL2L3BinsAndConfig(
  IN PrmContext *prmContext
  )
{
  String150 str;
  clearBytes(str,  sizeof(String150));
  snprintf(str, sizeof(String150), "cp -rf %s/* %s/rel/inst_C/backup/bin/",prmContext->isRunning->binDir, prmContext->picoBsHomePath);
  logInfo("%s",str);
  if ( 0 NOTEQ system( str ))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(str);
  }
  clearBytes(str,  sizeof(String150));
  snprintf(str, sizeof(String150), "cp -rf %s/* %s/rel/inst_C/backup/config/",prmContext->isRunning->configDir, prmContext->picoBsHomePath);
  logInfo("%s",str);
  if ( 0 NOTEQ system( str ))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(str);
  }
}

/**
 * @param    dirName : name of directory
 * @details  This function checks whether any file present at given directory location
 */
static TBOOL prmIsAnyFilePresentInDir(
  String150 dirName
  )
{
  String150 str;
  FILE *fp;
  UINT32 dirSize=0;

  clearBytes(str, sizeof(String150));
  snprintf(str, sizeof(String150), "ls -l %s | head -1|cut -d \" \" -f2|tr -d \" \">>pwav.txt",(char *)dirName);
  if(0 NOTEQ system(str))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(str);
  }
  clearBytes(str,150);
  fp = fopen("pwav.txt","r");
  fscanf( fp,"%s",str);
  fclose(fp);

  dirSize = atoi(str);
  if(0 NOTEQ system("rm pwav.txt"))
  {
     BS_ERROR_SYSTEM_COMMAND_FAILED("rm pwav.txt");
  }
  logInfo("size of directory is = %d",dirSize );
  if(dirSize EQ 0)
    return FALSE;
  return TRUE;
}


/**
 * @param    prmContext : PRM context Information
 * @details  This function copies patch files to running package.
 */
void prmCopyPatchFilesToRunningPackage(
  IO PrmContext *prmContext
  )
{
  String150 str;
  clearBytes(str,  sizeof(String150));
  snprintf(str, sizeof(String150), "cp -rf %s/rel/inst_C/bin/* %s",prmContext->picoBsHomePath ,prmContext->isRunning->binDir);
  logInfo("%s",str);
  if ( 0 NOTEQ system( str ))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(str);
  }
  clearBytes(str,  sizeof(String150));
  snprintf(str, sizeof(String150), "%s/rel/inst_C/config/",prmContext->picoBsHomePath);
  logInfo("%s",str);
  if(TRUE EQ prmIsAnyFilePresentInDir(str))
  {
    clearBytes(str,  sizeof(String150));
    snprintf(str, sizeof(String150), "cp -rf %s/rel/inst_C/config/* %s",prmContext->picoBsHomePath, prmContext->isRunning->configDir);
    logInfo("%s",str);
    if ( 0 NOTEQ system( str ))
    {
      BS_ERROR_SYSTEM_COMMAND_FAILED(str);
    }
  }
}

/**
 * @param    prmContext : PRM context Information
 * @details  This function copies back-up fileprmContext->rhSwInstallBinFiles to running package
 */
void prmCopyBackupFilesToRunningPackage(
  IO PrmContext *prmContext
  )
{
  String150 str;
  clearBytes(str,  sizeof(String150));
  snprintf(str, sizeof(String150), "cp -rf %s/rel/inst_C/backup/bin/* %s",prmContext->picoBsHomePath, prmContext->isRunning->binDir);
  logInfo("%s",str);
  if ( 0 NOTEQ system( str ))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(str);
  }
  clearBytes(str,  sizeof(String150));
  snprintf(str, sizeof(String150), "cp -rf %s/rel/inst_C/backup/config/* %s",prmContext->picoBsHomePath, prmContext->isRunning->configDir);
  logInfo("%s",str);
  if ( 0 NOTEQ system( str ))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(str);
  }
}

/**
 * @param    prmContext : PRM context Information
 * @details  This function processes when all modules start-up fails.
 */
void prmHandleL2L3PatchStartupFailed(
  IO PrmContext  *prmContext
  )
{
  String150 str;
  clearBytes(str,  sizeof(String150));

  prmCreateAndSendSwInstallFail( BS_ERR_INSTALL_L2L3_PATCH_STARTUP_FAILED, prmContext);
  snprintf(str,  sizeof(String150), "rm -rf %s/rel/inst_C", prmContext->picoBsHomePath);
  logInfo("%s",str);
  if ( 0 NOTEQ system( str ))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(str);
  }
  prmUpdateSwInstallStatusTable(prmContext->swinstall.filename, FALSE, "All module startup failed during patch installation");
  BS_ERROR_INSTALL_PATCH_STARTUP_FAILED("All module startup failed during patch installation");
  clearBytes(&(prmContext->swinstall), sizeof(SwInstall));
  prmContext->isInstallInProgress = FALSE;

}

/**
 * @param    prmContext : PRM context Information
 * @details  This function processes when all patch modules starts successfully.
 */
void prmHandleL2L3PatchStartupSucc(
  IO PrmContext  *prmContext
  )
{
  String150 str;
  clearBytes(str,  sizeof(String150));

  prmCreateAndSendSwInstallSucc( prmContext, prmContext->isRunning->versionInfo.version);
  snprintf(str,  sizeof(String150), "rm -rf %s/rel/inst_C %s/download/%s",
                prmContext->picoBsHomePath, prmContext->picoBsHomePath, prmContext->swinstall.filename);
  logInfo("%s",str);
  if ( 0 NOTEQ system( str ))
  {
    BS_ERROR_SYSTEM_COMMAND_FAILED(str);
  }

  prmUpdateSwInstallStatusTable(prmContext->swinstall.filename, TRUE, "Patch Installation Successful");
  clearBytes(&(prmContext->swinstall), sizeof(SwInstall));
  prmContext->isInstallInProgress = FALSE;
}

/**
 * @param    prmContext : PRM context Information
 * @param    installReq : software installation request
 * @details  This function checks whether any previous installation is in progress.
 *           If yes then sends reject to BSM.
 */
static TBOOL prmIsInstallationInProgress(
  IO PrmContext              *prmContext,
  IO MsgBsmPrmSwInstallReq   *installReq
  )
{
  SwInstall               req;
  clearBytes(&req, sizeof(SwInstall));
  if( (TRUE EQ prmContext->isInstallInProgress) )
  {
    prmDecSwInstallCmd( installReq, &(req) );
    if ( INSTALLATIONTYPE_L2L3RELEASE EQ req.installationType )
    {
      prmUpdateSwInstallStatusTable( req.filename, FALSE, "Previous installation is under progress can not perform release installation" );
      BS_ERROR_INSTALL_IN_PROGRESS("Previous installation is under progress can not perform release installation");
      prmCreateAndSendSwInstallRej( installReq, BS_ERR_INSTALL_IN_PROGRESS, "Previous installation is under progress can not perform release installation" );
    }
    else if ( INSTALLATIONTYPE_L2L3CONFIGURATION EQ req.installationType )
    {
      prmUpdateSwInstallStatusTable( req.filename, FALSE, "Previous installation is under progress can not perform configuration file installation" );
      BS_ERROR_INSTALL_IN_PROGRESS("Previous installation is under progress can not perform configuration file installation");
      prmCreateAndSendSwInstallRej( installReq, BS_ERR_INSTALL_IN_PROGRESS, "Previous installation is under progress can not perform configuration file installation" );
    }
    else if ( INSTALLATIONTYPE_L2L3PATCH EQ req.installationType )
    {
      prmUpdateSwInstallStatusTable( req.filename, FALSE, "Previous installation is under progress can not perform patch installation" );
      BS_ERROR_INSTALL_IN_PROGRESS("Previous installation is under progress can not perform patch installation");
      prmCreateAndSendSwInstallRej( installReq, BS_ERR_INSTALL_IN_PROGRESS, "Previous installation is under progress can not perform patch installation" );
    }
    else if ( INSTALLATIONTYPE_RHRELEASE EQ req.installationType )
    {
      prmUpdateSwInstallStatusTable( req.filename, FALSE, "Previous installation is under progress can not perform RH-SW installation" );
      BS_ERROR_INSTALL_IN_PROGRESS("Previous installation is under progress can not perform RH-SW installation");
      prmCreateAndSendSwInstallRej( installReq, BS_ERR_INSTALL_IN_PROGRESS, "Previous installation is under progress can not perform RH-SW installation" );
    }
    else
    {
      BS_ERROR_UNKNOWN_ENUM_VALUE("Unknown value received for installationType ENUM", req.installationType);
    }
    return TRUE;
  }
  logInfo("%s", "None of the installation is in Progress ! ");
  return FALSE;
}

/**
 * @param    msgIn      : Message received from BSM
 * @param    prmContext : PRM context Information
 * @details  This function will handle SW install request from BSM.
 */
void prmProcSwInstallReq(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  )
{
  String150               commandStr;
  MsgBsmPrmSwInstallReq   installReq;

  prmPrintTimeStamp();
  clearBytes( &installReq, sizeof(MsgBsmPrmSwInstallReq));
  clearBytes( commandStr, sizeof(String150) );

  decSetObjReqMsg( (UINT8*)msgIn->data, msgIn->len, &installReq );

  if (TRUE EQ prmContext->isActivationInProgress)
  {
    BS_ERROR_REQ_REJ_SW_ACT_IN_PROGRESS();
    prmCreateAndSendSwInstallRej( &installReq, BS_ERR_REQ_REJ_SW_ACTIVATION_IN_PROGRESS, "Previous Activation is under progress can not perform release installation" );
    return;
  }
  /* Check whether any installation is already in Progress */
  if( TRUE EQ prmIsInstallationInProgress(prmContext, &installReq) )
  {
    return;
  }
  prmDecSwInstallCmd( &installReq, &(prmContext->swinstall) );
  prmContext->isInstallInProgress = TRUE;

  if(FALSE EQ prmRunInstaller(prmContext, &installReq))
  {
    return;
  }
  /* After passing all sanity check and other verification steps are different for difernt installation */
  if ( INSTALLATIONTYPE_L2L3RELEASE EQ prmContext->swinstall.installationType )
  {
    prmUpdateNonRunningVersionInfo(prmContext);
    prmUpdateSwPackageStatusTableAfterEnbSwInstall( prmContext->isNotRunning, prmContext->isNotRunning->rootDir );
    prmWriteSwPackageStatusToPersistentMemory();

    /* If all checks are successful and install procedure is completed, send success */
    prmCreateAndSendSwInstallSucc(prmContext, &prmContext->isNotRunning->versionInfo.version[0]);
    prmUpdateSwInstallStatusTable( prmContext->swinstall.filename, TRUE, "Full Release Successfully Installed" );
    clearBytes(&(prmContext->swinstall), sizeof(SwInstall));
    prmContext->isInstallInProgress = FALSE;
  }
  else if ( INSTALLATIONTYPE_L2L3CONFIGURATION EQ prmContext->swinstall.installationType )
  {
    /* Create and Send CONFIG_INSTALL_REQ to BSM */
    MsgPrmBsmConfigInstallReq   configInstallReq;
    clearBytes( &configInstallReq, sizeof(MsgPrmBsmConfigInstallReq));
    configInstallReq.cmdRef = installReq.cmdRef;
    snprintf( configInstallReq.configFilePath, MAX_SIZE_SWINSTALL_FILENAME + 1, "%s/download", prmContext->picoBsHomePath );
    snprintf( configInstallReq.filename, MAX_SIZE_SWINSTALL_FILENAME + 1, "%s", prmContext->swinstall.filename );
    createAndSendMsg( MSG_PRM_BSM_CONFIG_INSTALL_REQ, (UINT8*)&configInstallReq, sizeof(MsgPrmBsmConfigInstallReq));
  }
  else if ( INSTALLATIONTYPE_L2L3PATCH EQ prmContext->swinstall.installationType )/* PATCH INSTALLATION */
  {
    prmCreateBackupOfL2L3BinsAndConfig( prmContext );
    prmContext->moduleExitCause = MODEXITCAUSE_L2L3_PATCH_INSTALL;
    prmDoExitProcedure( prmContext );
  }
  else if ( INSTALLATIONTYPE_RHRELEASE EQ prmContext->swinstall.installationType )/* RH-SW INSTALLATION */
  {
    /* Create and Send RH_INSTALL_REQ to BSM */
    MsgPrmDehRhSwInstallReq   rhInstallReq;
    PrmTimersInfo            *timerInfo         = getPrmTimersInfo();
    clearBytes( &rhInstallReq, sizeof(MsgPrmDehRhSwInstallReq));

    /* Start timer and wait for RH to respond for RH-SW INSTALL request */
    startPrmProcTimer( TIMERNAME_WAITRHSWINSTALLRSP, REASON_RH_SW_INSTALL, timerInfo );
    snprintf( rhInstallReq.filename, MAX_SIZE_SWINSTALL_FILENAME + 1, "%s/download/%s", prmContext->picoBsHomePath, prmContext->rhSwInstallBinFile );
    createAndSendMsg( MSG_PRM_DEH_RH_SW_INSTALL_REQ, (UINT8*)&rhInstallReq, sizeof(MsgPrmDehRhSwInstallReq));
  }
  else
  {
    BS_ERROR_UNKNOWN_ENUM_VALUE("Unknown value received for installationType ENUM", prmContext->swinstall.installationType);
  }
  return;
}

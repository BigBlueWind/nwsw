/**
 * @file    prm_sw_act.c
 * @brief his file contains functions related to Software Activation Procedure.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author        : Shweta Polepally
 * Date          : July 2010
 * Change History:
 *
 */
#include "prm.h"

void prmReplaceExeWithRunningOne(
  IO PrmContext  *ctx
  )
{
  String512  currentLink;
  String512  newPath;
  String512  newLink;
  clearBytes(&currentLink,sizeof(String512));
  clearBytes(&newPath,sizeof(String512));
  clearBytes(&newLink,sizeof(String512));

  /* Relink bootarm_bs.sh to running instance */
  snprintf((char *)&currentLink, sizeof(String512), "%s/run/bootarm_bs.sh", ctx->picoBsHomePath);
  if( 0 NOTEQ unlink(currentLink))
  {
	logError("%s", "unlinking of bootarm_bs.sh script failed");
	return;
  }
  snprintf( (char *)&newPath, sizeof(String512), "%s/bootarm_bs.sh",      ctx->isRunning->rootDir);
  snprintf( (char *)&newLink, sizeof(String512), "%s/run/bootarm_bs.sh",  ctx->picoBsHomePath);
  if(0 NOTEQ symlink(newPath, newLink))
  {
    logError("%s", "linking of bootarm_bs.sh script failed");
    return;
  }

  /* Relink bs.sh to running instance */
  snprintf((char *)&currentLink, sizeof(String512), "%s/run/bs.sh", ctx->picoBsHomePath);
  if( 0 NOTEQ unlink(currentLink))
  {
    logError("%s", "unlinking of bs.sh script failed");
    return;
  }
  snprintf( (char *)&newPath, sizeof(String512), "%s/bs.sh",      ctx->isRunning->rootDir);
  snprintf( (char *)&newLink, sizeof(String512), "%s/run/bs.sh",  ctx->picoBsHomePath);
  if(0 NOTEQ symlink(newPath, newLink))
  {
    logError("%s", "linking of bs.sh script failed");
    return;
  }

  /* Relink dump_lowerarm.sh to running instance */
  snprintf((char *)&currentLink, sizeof(String512), "%s/run/dump_lowerarm.sh", ctx->picoBsHomePath);
  if( 0 NOTEQ unlink(currentLink))
  {
    logError("%s", "unlinking of dump_lowerarm.sh script failed");
    return;
  }
  snprintf( (char *)&newPath, sizeof(String512), "%s/dump_lowerarm.sh",      ctx->isRunning->rootDir);
  snprintf( (char *)&newLink, sizeof(String512), "%s/run/dump_lowerarm.sh",  ctx->picoBsHomePath);
  if(0 NOTEQ symlink(newPath, newLink))
  {
    logError("%s", "linking of dump_lowerarm.sh script failed");
    return;
  }

  /* Relink prm.exe to running instance */
  snprintf((char *)&currentLink, sizeof(String512), "%s/run/prm.exe", ctx->picoBsHomePath);
  if( 0 NOTEQ unlink(currentLink))
  {
    logError("%s", "unlinking of prm.exe failed");
    printf("\n\nPRM = %d\n\n", errno);
    return;
  }
  snprintf( (char *)&newPath, sizeof(String512), "%s/prm.exe",      ctx->isRunning->binDir);
  snprintf( (char *)&newLink, sizeof(String512), "%s/run/prm.exe",  ctx->picoBsHomePath);
  if(0 NOTEQ symlink(newPath, newLink))
  {
    logError("%s", "linking of prm.exe failed");
    return;
  }

  /* Relink logutil.exe to running instance */
  snprintf((char *)&currentLink, sizeof(String512), "%s/run/logutil.exe", ctx->picoBsHomePath);
  if( 0 NOTEQ unlink(currentLink))
  {
    logError("%s", "unlinking of logutil.exe failed");
    return;
  }
  snprintf( (char *)&newPath, sizeof(String512), "%s/logutil.exe",      ctx->isRunning->binDir);
  snprintf( (char *)&newLink, sizeof(String512), "%s/run/logutil.exe",  ctx->picoBsHomePath);
  if(0 NOTEQ symlink(newPath, newLink))
  {
    logError("%s", "linking of logutil.exe failed");
    return;
  }

  /* Relink rdmem_arm to running instance */
  snprintf((char *)&currentLink, sizeof(String512), "%s/run/rdmem_arm", ctx->picoBsHomePath);
  if( 0 NOTEQ unlink(currentLink))
  {
    logError("%s", "unlinking of rdmem_arm failed");
    return;
  }
  snprintf( (char *)&newPath, sizeof(String512), "%s/rdmem_arm",      ctx->isRunning->rootDir);
  snprintf( (char *)&newLink, sizeof(String512), "%s/run/rdmem_arm",  ctx->picoBsHomePath);
  if(0 NOTEQ symlink(newPath, newLink))
  {
    logError("%s", "linking of rdmem_arm failed");
    return;
  }
}

/**
 * @param    swType   Software Type
 * @param    version      version which is Activated
 * @details This functions will fill and send MSG_PRM_BSM_SW_ACTIVATE_SUCC to BSM
 */
void prmCreateAndSendSwActivateSucc(
  IN SwType          swType,
  IN UINT8          *version
  )
{
  MsgPrmBsmSwActivateSucc   swActvSucc;

  clearBytes( &swActvSucc, sizeof(MsgPrmBsmSwActivateSucc));
  /* Fill the message */
  swActvSucc.swType = swType;
  snprintf(swActvSucc.version, strlen(version)+1, "%s", version);
  createAndSendMsg( MSG_PRM_BSM_SW_ACTIVATE_SUCC, (UINT8*)&swActvSucc, sizeof(swActvSucc) );
}

/**
 * @param    swType       Software Type
 * @param    version      version which is asked to Activated
 * @param    errDesc      error desc, why activation failed.
 * @details This functions will fill and send MSG_PRM_BSM_SW_ACTIVATE_FAIL to BSM
 */
void prmCreateAndSendSwActivateFail(
  IN SwType          swType,
  IN UINT8          *version,
  IO ErrorDesc      *errDesc
  )
{
  MsgPrmBsmSwActivateFail   swActvFail;

  clearBytes( &swActvFail, sizeof(MsgPrmBsmSwActivateFail));
  /* Fill the message */
  swActvFail.swType = swType;
  snprintf(swActvFail.version, strlen(version)+1, "%s", version);
  copyBytes(errDesc,sizeof(ErrorDesc),&(swActvFail.errorDesc));
  createAndSendMsg( MSG_PRM_BSM_SW_ACTIVATE_FAIL, (UINT8*)&swActvFail, sizeof(swActvFail) );
}

/** @param    msg        : Message received
  * @param    prmContext : PRM context
  * @details  This function will handle RH software Activations success message from DEH
  **/
void prmProcRhSwActivateSucc(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  )
{
  String256 str;
  clearBytes(str, sizeof(String256) );

  MsgDehPrmRhSwActivateSucc  *rhActivateSucc = (MsgDehPrmRhSwActivateSucc*)msgIn->data;

  stopPrmProcedureTimer(TIMERNAME_WAITRHACTIVATERSP);
  prmCreateAndSendSwActivateSucc(SWTYPE_RH_SW, &(rhActivateSucc->version[0]));
  prmUpdateSwActivationStatusTable( rhActivateSucc->version, SWSTATUS_SUCCESSFUL );
  prmUpdateSwPackageStatusTableForRhSw(rhActivateSucc->version);
  prmWriteSwPackageStatusToPersistentMemory();
  prmContext->isActivationInProgress = FALSE;
}

/** @param    msg        : Message received
  * @param    prmContext : PRM context
  * @details  This function will handle RH software Activations failure message from DEH
  **/
void prmProcRhSwActivateFail(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  )
{
  String256 str;
  clearBytes(str, sizeof(String256) );

  MsgDehPrmRhSwActivateFail  *rhActivateFail = (MsgDehPrmRhSwActivateFail*)msgIn->data;

  stopPrmProcedureTimer(TIMERNAME_WAITRHACTIVATERSP);

  prmCreateAndSendSwActivateFail(SWTYPE_RH_SW, &(rhActivateFail->version[0]), &(rhActivateFail->errorDesc)  );
  prmUpdateSwActivationStatusTable( rhActivateFail->version, SWSTATUS_FAILED);
  prmContext->isActivationInProgress = FALSE;
}

/**
 * @param    setObj    Set Object request message
 * @param    error     Error Value
 * @param    errInfo   Error Information string
 * @details This functions will fill and send MSG_PRM_BSM_SW_ACTIVATE_REJ to BSM
 */
static void prmCreateAndSendActivateRej(
  IN SetObjReq     *setObj,
  IN BsErrorValue   error,
  IN UINT8         *errInfo
  )
{
  MsgPrmBsmSwActivateRej setObjFail;
  EncBuf                 encBuf;

  clearBytes( &setObjFail, sizeof(MsgPrmBsmSwActivateRej));
  initEncBuf( &encBuf );

  /* Fill the message */
  setObjFail.cmdRef = setObj->cmdRef;
  setObjFail.setErrRsp.errRspCode    = error;
  snprintf( (UINT8*)&setObjFail.setErrRsp.errorInfo, (MAX_TLV_VALUE_IN_BYTES+1), "%s", errInfo );
  setObjFail.setErrRsp.failedTagInfo = setObj->tlvArray.tlvInfo[0].tag;
  setObjFail.tlvArray = setObj->tlvArray;

  /* Encode and Send */
  encSetObjRspFailMsg(&setObjFail,&encBuf);
  createAndSendMsg( MSG_PRM_BSM_SW_ACTIVATE_REJ, (UINT8*)&encBuf.value, encBuf.len );
}

/**
 * @param    setObj  : Set Object request message
 * @details This functions will fill and send MSG_PRM_BSM_SW_ACTIVATE_RSP to BSM
 */
static void prmCreateAndSendSwActivateRsp(
  IN SetObjReq  *setObj
  )
{
  MsgPrmBsmSwActivateRsp      setObjSucc;
  EncBuf                      encBuf;

  clearBytes( &setObjSucc, sizeof(MsgPrmBsmSwActivateRsp));
  initEncBuf( &encBuf );
  /* Fill the message */
  setObjSucc.cmdRef = setObj->cmdRef;
  setObjSucc.tlvArray =setObj->tlvArray;
  /* Encode and Send */
  encSetObjRspSuccMsg(&setObjSucc,&encBuf);
  createAndSendMsg( MSG_PRM_BSM_SW_ACTIVATE_RSP, (UINT8*)&encBuf.value, encBuf.len );
}

/**
 * @param    version    : version information
 * @param    swStatus   : Sw status - running/inProgress/not-Initiated/failed
 * @details  This function will update prmUpdateSwActivationStatusTable with swStatus and version information
 */
void prmUpdateSwActivationStatusTable(
  IN UINT8           *version,
  IN SwStatus         swStatus
  )
{
  SwActivationStatus  *rowOfTable = (SwActivationStatus*)getFirstRowOfTable( SWACTIVATIONSTATUS );

  copyBytes( version, strlen(version), rowOfTable->version );
  rowOfTable->swStatus = swStatus;
}

/**
 * @param    ctx        PRM context
 * @details  This function will update SwPackageStatus table for software activation.
 *           Matched version will be marked isRunning=TRUE.
 */
void prmUpdateSwPackageStatusTableForEnbSw(
  IN PrmContext            *ctx
  )
{
  SwPackageStatus   *swPackageStatus = (SwPackageStatus*)getFirstRowOfTable(SWPACKAGESTATUS);
  UINT8 i = 0;

  for ( i=0; i < MAX_ROWS_IN_SWPACKAGESTATUS_TABLE; i++ )
  {
    if( swPackageStatus->swType EQ SWTYPE_ENB_L2L3_SW )
    {
      if( 0 EQ strncmp(swPackageStatus->version, ctx->isRunning->versionInfo.version, strlen(swPackageStatus->version)) )
      {
        swPackageStatus->isRunning = TRUE;
      }
      else
      {
        swPackageStatus->isRunning = FALSE;
      }
    }
    getNextRowOfTable(swPackageStatus);
  }
}

/**
 * @param    version       Version string
 * @details  This function will update SwPackageStatus table for software activation.
 *           Matched version will be marked isRunning=TRUE.
 */
void prmUpdateSwPackageStatusTableForRhSw(
  IN UINT8   *version
  )
{
  SwPackageStatus   *swPackageStatus = (SwPackageStatus*)getFirstRowOfTable(SWPACKAGESTATUS);
  UINT8 i = 0;

  for ( i=0; i < MAX_ROWS_IN_SWPACKAGESTATUS_TABLE; i++ )
  {
    if( swPackageStatus->swType EQ SWTYPE_RH_SW )
    {
      if( 0 EQ strncmp(swPackageStatus->version, version, strlen(version)) )
      {
        swPackageStatus->isRunning = TRUE;
      }
      else
      {
        swPackageStatus->isRunning = FALSE;
      }
    }
    getNextRowOfTable(swPackageStatus);
  }
}

/**
 * @param    ctx        : PRM context
 * @details  This function will copy snmpd.conf from running version to non running version.
 */
void prmCopyCfgToNonRunningPackage(
  IN PrmContext            *ctx
  )
{
  String150 str;
  clearBytes(str, sizeof(String150));
  snprintf(str, sizeof(String150), "cp -rf %s/snmpd.conf %s",ctx->isRunning->configDir, ctx->isNotRunning->configDir);
  if(0 NOTEQ system(str))
    BS_ERROR_SYSTEM_COMMAND_FAILED(str);
}

/**
 * @param    setObj    : Set Object request
 * @param    swAct     : Software Activation information to be filled in this structure.
 * @details  This function will decode activation information from TLV format
 */
static void prmDecSwActivateCmd(
  IO SetObjReq      *setObj,
  IO SwActivation   *swAct
  )
{
  GenericTable     *tableFunc = getTableFunctions(SWACTIVATION);
  if ( NULL NOTEQ tableFunc->getTlv2StructFunc )
  {
    tableFunc->getTlv2StructFunc( &setObj->tlvArray, swAct );
  }
}

/**
 * @details  This function returns Non running version of RH from SwPackageStatus table.
 */
char *prmGetRhNonRunningVersion()
{
  UINT8          i = 0;
  SwPackageStatus  *rowOfSwPackageStatus = (SwPackageStatus*)getFirstRowOfTable(SWPACKAGESTATUS);

  for ( i=0; i < MAX_ROWS_IN_SWPACKAGESTATUS_TABLE; i++ )
  {
    if( (SWTYPE_RH_SW EQ rowOfSwPackageStatus->swType ) &&( FALSE EQ rowOfSwPackageStatus->isRunning ))
    {
      return rowOfSwPackageStatus->version;
    }
    getNextRowOfTable( rowOfSwPackageStatus );
  }
  return "0.0.0.0.0";

}

/**
 * @param    version    : version
 * @param    isRunning  : boolean
 * @details  This function will compare the version with the isRunning version of RH
 */
TBOOL prmCompareVersionFromSwPackageTable(
  IN UINT8  *version,
  IN TBOOL   isRunning
  )
{
  UINT8          i = 0;
  SwPackageStatus  *rowOfSwPackageStatus = (SwPackageStatus*)getFirstRowOfTable(SWPACKAGESTATUS);

  for ( i=0; i < MAX_ROWS_IN_SWPACKAGESTATUS_TABLE; i++ )
  {
    if( (SWTYPE_RH_SW EQ rowOfSwPackageStatus->swType ) &&( isRunning EQ rowOfSwPackageStatus->isRunning ))
    {
      if (0 EQ strncmp( version, rowOfSwPackageStatus->version, strlen(version)))
        return TRUE;
      else
        return FALSE;
    }
    getNextRowOfTable( rowOfSwPackageStatus );
  }
  return FALSE;
}

/**
 * @param    prmContext     : PRM Context
 * @param    swActivateReq  : SwActivation request in TLV format
 * @param    swActReq       : SwActivation request in structure format
 * @details  This function will perform operation for RH_SW_ACTIVATION
 */
void prmPerformRhSwActivation(
  IO PrmContext             *prmContext,
  IO MsgBsmPrmSwActivateReq *swActivateReq,
  IO SwActivation           *swActReq
  )
{
  MsgPrmDehRhSwActivateReq  rhSwActivateReq;
  String256                 str;
  PrmTimersInfo            *timerInfo = getPrmTimersInfo();

  clearBytes( &rhSwActivateReq, sizeof(MsgPrmDehRhSwActivateReq) );
  clearBytes(&str,sizeof(String256));

  /* Compare the requested version with the RH Running version */
  if( TRUE EQ prmCompareVersionFromSwPackageTable((UINT8 *)&(swActReq->version), TRUE))
  {
    /** Send fail */
    BS_ERROR_VER_ALREADY_ACTIVE( swActReq->version );
    prmUpdateSwActivationStatusTable( swActReq->version, SWSTATUS_FAILED );
    snprintf(str, sizeof(String256), "Version %s is already active", swActReq->version);
    prmCreateAndSendActivateRej(swActivateReq, BS_ERR_VER_ALREADY_ACTIVE, str );
    prmContext->isActivationInProgress = FALSE;
    return;
  }

  /* Compare the requested version with the RH install non-running version */
  if( FALSE EQ prmCompareVersionFromSwPackageTable((UINT8 *)&(swActReq->version), FALSE))
  {
    /** Send fail */
    BS_ERROR_SW_VER_NOT_AVAILABLE( swActReq->version );
    prmUpdateSwActivationStatusTable( swActReq->version, SWSTATUS_FAILED );
    snprintf(str, sizeof(String256), "Version %s is not available on EnodeB", swActReq->version);
    prmCreateAndSendActivateRej(swActivateReq, BS_ERR_SW_VER_NOT_AVAILABLE, str );
    prmContext->isActivationInProgress = FALSE;
    return;
  }
  prmCreateAndSendSwActivateRsp(swActivateReq);
  prmUpdateSwActivationStatusTable( swActReq->version, SWSTATUS_INPROGRESS );

  /* Start timer and wait for RH to respond for RH-SW ACTIVATE request */
  startPrmProcTimer( TIMERNAME_WAITRHACTIVATERSP, REASON_RH_SW_ACTIVATION, timerInfo );
  /* Send RH_SW_ACTIVATE_REQ message to DEH */
  copyBytes( swActReq->version, sizeof(swActReq->version), rhSwActivateReq.version );
  createAndSendMsg( MSG_PRM_DEH_RH_SW_ACTIVATE_REQ, (UINT8*)&rhSwActivateReq, sizeof(MsgPrmDehRhSwActivateReq));
}

/**
 * @param    prmContext     : PRM Context
 * @param    swActivateReq  : SwActivation request in TLV format
 * @param    swActReq       : SwActivation request in structure format
 * @details  This function will perform operation for ENB SW_ACTIVATION
 */
void prmPerformEnbSwActivation(
  IO PrmContext             *prmContext,
  IO MsgBsmPrmSwActivateReq *swActivateReq,
  IO SwActivation           *swActReq
  )
{
  String256                str;

  clearBytes(&str,sizeof(String256));

  /* Compare the requested version with the ENB running version */
  if( 0 EQ strncmp( swActReq->version, prmContext->isRunning->versionInfo.version, strlen(prmContext->isRunning->versionInfo.version)) )
  {
    /** Send fail */
    BS_ERROR_VER_ALREADY_ACTIVE( swActReq->version );
    prmUpdateSwActivationStatusTable( swActReq->version, SWSTATUS_FAILED );
    snprintf(str, sizeof(String256), "Version %s is already active", swActReq->version);
    prmCreateAndSendActivateRej(swActivateReq, BS_ERR_VER_ALREADY_ACTIVE, str );
    prmContext->isActivationInProgress = FALSE;
    return;
  }

  /* Compare the requested version with the ENB install non-running version */
  if( 0 NOTEQ strncmp( swActReq->version, prmContext->isNotRunning->versionInfo.version, strlen(prmContext->isNotRunning->versionInfo.version)) )
  {
    /** Send fail */
    BS_ERROR_SW_VER_NOT_AVAILABLE( swActReq->version );
    prmUpdateSwActivationStatusTable( swActReq->version, SWSTATUS_FAILED );
    snprintf(str, sizeof(String256), "Version %s is not available on EnodeB", swActReq->version);
    prmCreateAndSendActivateRej(swActivateReq, BS_ERR_SW_VER_NOT_AVAILABLE, str );
    prmContext->isActivationInProgress = FALSE;
    return;
  }
  prmCreateAndSendSwActivateRsp(swActivateReq);
  prmUpdateSwActivationStatusTable( swActReq->version, SWSTATUS_INPROGRESS );

  /* Send SW_ACTIVATE_SUCC message to BSM */
  //prmCreateAndSendSwActivateSucc(SWTYPE_ENB_L2L3_SW,swActReq->version);
  prmCopyCfgToNonRunningPackage(prmContext);
  /* Bring down the currently running version */
  prmContext->moduleExitCause = MODEXITCAUSE_SWACT;
  prmDoExitProcedure( prmContext );  /* Start gets changed to "WAIT_EXIT_COMPLETE" */

}

/** @param    msg        : Message received
  * @param    prmContext : PRM context
  * @details  This function will handle Software Activations Request message from BSM
  **/
void prmProcSwActivateReq(
  IN Msg         *msg,
  IO PrmContext  *prmContext
  )
{
  MsgBsmPrmSwActivateReq   swActivateReq;
  SwActivation             swActReq;

  clearBytes( &swActivateReq, sizeof(MsgBsmPrmSwActivateReq) );
  clearBytes( &swActReq, sizeof(SwActivation));

  decSetObjReqMsg( (UINT8*)msg->data, msg->len, &swActivateReq );

  prmDecSwActivateCmd( &swActivateReq, &swActReq );
  if (TRUE EQ prmContext->isActivationInProgress)
  {
    BS_ERROR_REQ_REJ_SW_ACT_IN_PROGRESS();
    prmCreateAndSendActivateRej(&swActivateReq, BS_ERR_REQ_REJ_SW_ACTIVATION_IN_PROGRESS, "Activation In progress " );
    return;
  }
  if (TRUE EQ prmContext->isInstallInProgress)
  {
    BS_ERROR_INSTALL_IN_PROGRESS("Previous installation is under progress can not perform activation");
    prmCreateAndSendActivateRej(&swActivateReq, BS_ERR_INSTALL_IN_PROGRESS, "Previous Installation is In progress " );
    return;
  }
  prmContext->isActivationInProgress = TRUE;
  if (SWTYPE_ENB_L2L3_SW EQ swActReq.swType)
  {
    prmPerformEnbSwActivation(prmContext, &swActivateReq, &swActReq);
  }
  else if (SWTYPE_RH_SW EQ swActReq.swType)
  {
    prmPerformRhSwActivation(prmContext, &swActivateReq, &swActReq);
  }
  else
  {
    BS_ERROR_UNKNOWN_ENUM_VALUE("Unknown value received for swType ENUM", swActReq.swType);
  }
}


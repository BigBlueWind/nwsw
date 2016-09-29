/**
 * @file    prm_hc.c
 * @brief This file contains Health check procedure code.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author        : Shweta Polepally
 * Date          : July 2010
 * Change History:
 *
 */

#include "prm.h"

/**
 * @param modId     Module Identity
 * @details This function will send Hello message to the module mentioned in modId.
 */
static void prmCreateAndSendHelloMsg(
  IN ModuleId      modId,
  IN UINT32        helloSeqNum
  )
{
  MsgPrmModHello  hello;
  clearBytes( &hello, sizeof(MsgPrmModHello) );

  hello.helloSeqNum = helloSeqNum;

  switch( modId )
  {
    case MODID_FTH:    createAndSendMsg( MSG_PRM_FTH_HELLO,  (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_SCTP:   createAndSendMsg( MSG_PRM_SCTP_HELLO, (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_EGTP:   createAndSendMsg( MSG_PRM_EGTP_HELLO, (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_TUCL:   createAndSendMsg( MSG_PRM_TUCL_HELLO, (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_MAC:    createAndSendMsg( MSG_PRM_MAC_HELLO,  (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_PDCP:   createAndSendMsg( MSG_PRM_PDCP_HELLO, (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_RLC:    createAndSendMsg( MSG_PRM_RLC_HELLO,  (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_RRC:    createAndSendMsg( MSG_PRM_RRC_HELLO,  (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_S1AP:   createAndSendMsg( MSG_PRM_S1AP_HELLO, (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_UEH:    createAndSendMsg( MSG_PRM_UEH_HELLO,  (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_DEH:    createAndSendMsg( MSG_PRM_DEH_HELLO,  (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_SIH:    createAndSendMsg( MSG_PRM_SIH_HELLO,  (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_WIH:    createAndSendMsg( MSG_PRM_WIH_HELLO,  (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_BSM:    createAndSendMsg( MSG_PRM_BSM_HELLO,  (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_LTH:    createAndSendMsg( MSG_PRM_LTH_HELLO,  (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_PMH:    createAndSendMsg( MSG_PRM_PMH_HELLO,  (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    case MODID_UDH:    createAndSendMsg( MSG_PRM_UDH_HELLO,  (UINT8*)&hello, sizeof(MsgPrmModHello) );    break;
    default:           BS_ERROR_UNKNOWN_MOD_ID(modId );
  }
}

static void prmStorePreviousModuleStatus(
  IN ModuleStatus           *currentEntry,
  IO PrevModuleStatusTable  *prevEntry
  )
{
  prevEntry->entry[prevEntry->numOfModules].isRunning     = currentEntry->isRunning;
  prevEntry->entry[prevEntry->numOfModules].moduleId      = currentEntry->moduleIdString;
  prevEntry->entry[prevEntry->numOfModules].restartReason = currentEntry->restartReason;
  prevEntry->numOfModules++;
}

static PrevModuleStatusEntry *prmGetPreviousStatusOfModule(
  IN ModuleId  moduleId
  )
{
  Index        index;
  PrmContext  *ctx = getPrmContext();
  PrevModuleStatusTable  *prevModuleStatusTable = &ctx->prevModuleStatusTable;

  for ( index=0; index < MODID_MAX; index++ )
  {
    if ( moduleId EQ prevModuleStatusTable->entry[index].moduleId )
      return &prevModuleStatusTable->entry[index];
  }
  return NULL;
}

static BsErrorValue prmGetPrevModuleDownBsErrorCode(
  IN RestartReason   reason
  )
{
  if ( reason EQ RESTARTREASON_CRASH )                         return BS_ERR_SW_CRASH;
  else if ( reason EQ RESTARTREASON_NORESPONSETOHEALTHCHECK )  return BS_ERR_NO_RESPONSE_TO_HEALTH_CHECK;
  else return BS_ERR_UNSPECIFIED;
}
/**
 * @param prmContext PRM Module related information.
 * @details This function will do the heart beat check by sending HELLO Messages to
 *          all the modules that are running.
 */
void prmDoHeartBeatCheck(
  IO PrmContext   *prmContext
  )
{
  PrmTimersInfo   *timerInfo         = getPrmTimersInfo();
  ModuleStatus    *rowOfModuleStatus = (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);

  UINT8  i  = 0;

  prmContext->numOfRspRcvd   = 0;
  prmContext->helloSendCount = 0;
  prmContext->prevModuleStatusTable.numOfModules = 0;

  prmContext->helloSeqNum++;

  for ( i=0; i < prmContext->numOfModules; i++ )
  {
    prmStorePreviousModuleStatus( rowOfModuleStatus, &prmContext->prevModuleStatusTable );        
    rowOfModuleStatus->isRunning = FALSE;
    /* The above flag will be set to TRUE when HELLO_ACK is received from the modules */
    prmCreateAndSendHelloMsg( rowOfModuleStatus->moduleIdString, prmContext->helloSeqNum );
    prmContext->helloSendCount++;
    
    getNextRowOfTable(rowOfModuleStatus);
  }
  /* Start timer and wait for the ack to be received from all modules */
  startPrmProcTimer( TIMERNAME_WAITHELLOACK, REASON_HEALTH_CHECK, timerInfo );
}

/**
 * @param    prmContext PRM Module related information.
 * @details  This function will check for the completion of Hello Procedure.
 *           If any process has not started, that will be reported to the BSM with the
 *           reason - Crashed / Not responding.
 *           Send MODULE_DOWN indication to BSM if any module has not responded with HELLO_ACK.
 **/
void prmCheckForHealthCheckProcedureCompletion(
  IO PrmContext    *prmContext
  )
{
  UINT32        i;
  SINT32        j;
  ModuleStatus           *rowOfModuleStatus = (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);
  PrevModuleStatusEntry  *prevModuleStatusEntry;
  /* Check if any process State is still "Checking" - This means HELLO_ACK was not received */
  prmContext->areAllModulesUp = TRUE;

  for ( i=0 ; i< prmContext->numOfModules; i++)
  {
    prevModuleStatusEntry = prmGetPreviousStatusOfModule( rowOfModuleStatus->moduleIdString );
    if ( FALSE EQ rowOfModuleStatus->isRunning )
    {
      prmContext->areAllModulesUp = FALSE;
      if ( NULL NOTEQ prevModuleStatusEntry )
      {
        /* report only if not reported previously */
        if (( FALSE EQ prevModuleStatusEntry->modDownReported ) ||
            (TRUE EQ prevModuleStatusEntry->isRunning))
        {
          prevModuleStatusEntry->modDownReported = TRUE;
          /* Check if the PID exists, else report as CRASH */
          /* 0 means don't send any signal to the pid, just check if the pid exists */
          j = killChld(rowOfModuleStatus->processOrTaskId, 0);
          if ( j EQ 0 )
          {
            rowOfModuleStatus->restartReason = RESTARTREASON_NORESPONSETOHEALTHCHECK;
            BS_ERROR_MODULE_DID_NOT_RESPOND( getModuleName(rowOfModuleStatus->moduleIdString) );
            prmCreateAndSendModuleDownInd( BS_ERR_NO_RESPONSE_TO_HEALTH_CHECK, rowOfModuleStatus );
          }
          else
          {
            rowOfModuleStatus->restartReason = RESTARTREASON_CRASH;
            BS_ERROR_PROCESS_CRASHED(getModuleName(rowOfModuleStatus->moduleIdString),rowOfModuleStatus->processOrTaskId );
            prmCreateAndSendModuleDownInd( BS_ERR_SW_CRASH, rowOfModuleStatus );
          }
        }
      }
      else
      {
        /* Log Error -- This should have never occur */
        logInfo( "%s", "This should have not occur-- check. Previous status of this module does not exists !! " );
      }
    }
    else
    {
      /* isRunning = TRUE. Check if module was previously down, then send MOD_UP_IND to BSM */
      if ( NULL NOTEQ prevModuleStatusEntry )
      {
        /* Condition: Previous Status: isRunning=FALSE
         *            Current  Status: isRunning=TRUE
         *            Action         : Send MOD_UP_IND to BSM
         */
        if ( FALSE EQ prevModuleStatusEntry->isRunning )
        {
          prmCreateAndSendModuleUpInd( prmGetPrevModuleDownBsErrorCode(prevModuleStatusEntry->restartReason), rowOfModuleStatus );
          rowOfModuleStatus->restartReason = RESTARTREASON_UNKOWN;
        }
      }
    }
    getNextRowOfTable(rowOfModuleStatus);
  }
}

/**
 * @param    msgIn      : Message received
 * @param    prmContext : PRM Module related Information
 * @details  This function will handle Hello Ack message received from
 *           all the modules.
 * */
void prmProcHelloAckInWaitHelloAck(
  IN Msg         *msgIn,
  IO PrmContext  *prmContext
  )
{
  UINT8 i;
  ModuleStatus       *rowOfModuleStatus = (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);
  MsgModPrmHelloAck  *helloAck = (MsgModPrmHelloAck*)msgIn->data;

  if ( helloAck->helloAckSeqNum NOTEQ prmContext->helloSeqNum )
  {
    /* Log Error and continue. This is to check if any of the messages were dropped/ sequence number mis-match */
    logError("HELLO_ACK for Module=[%s] was expected to be received with sequence number:[%d], but received seqNum:[%d]",
              getModuleName(msgIn->src),
              prmContext->helloSeqNum,
              helloAck->helloAckSeqNum
            );
  }

  for ( i=0; i< prmContext->numOfModules; i++)
  {
    if ( rowOfModuleStatus->moduleIdString EQ msgIn->src )
    {
      rowOfModuleStatus->isRunning = TRUE;
      /* Update startedAt Format : YYYYMMDDHHNNSSTTT */
      getAndFillDateAndTimeStamp( MAX_SIZE_MODULESTATUS_LASTHEALTHCHECKAT, &rowOfModuleStatus->lastHealthCheckAt[0] );
      prmContext->numOfRspRcvd++;
      break;
    }
    getNextRowOfTable(rowOfModuleStatus);
  }
  if ( prmContext->numOfRspRcvd EQ prmContext->helloSendCount )
  {
    prmContext->numOfRspRcvd   = 0;  /* Reset the counter */
    prmContext->helloSendCount = 0;

    stopPrmProcedureTimer( TIMERNAME_WAITHELLOACK );

    prmCheckForHealthCheckProcedureCompletion( prmContext );
    if ( FALSE EQ prmContext->areAllModulesUp )
    {
      /* get the first Row of the table again and then pass it to the function */
      rowOfModuleStatus = (ModuleStatus*)getFirstRowOfTable(MODULESTATUS);
    }

    startPrmProcTimer( TIMERNAME_HEALTHCHECKINTERVAL, REASON_HEALTH_CHECK, &prmContext->prmTimersInfo );
    /* Move the state to IDLE, as response is received from all modules */
    prmSetState( IDLE, prmContext );
  }
}

/**
 * @param    msg        : Message received
 * @param    prmContext : PRM Module related Information
 * @details  This function will handle Hello Ack message received from
 *           all the modules.
 * */
void prmProcHelloAckInIdle(
  IN Msg         *msg,
  IO PrmContext  *prmContext
  )
{
  BS_ERROR_MSG_RCVD_IN_UNEXPECTED_STATE(msg->msgId, getPrmStateName(prmContext->prmState) );
  return;
}





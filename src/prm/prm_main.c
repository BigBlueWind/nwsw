/**
 * @file    prm_main.c
 * @brief This file contains the functions that handles all the messages which will arrive
 *          at PRM Module.
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author         : Shweta Polepally
 * Date           : 05 July 2010
 * Change History :
 *
 */

#include "prm.h"

/**
* @details This functions returns the PRM Context
*/
PrmContext         g_PrmContext;
PrmStateMachine    prmStateMachine[MAX_STATE][MSG_ID_MAX];

/* Set the state machine */
#define InitPrmStateMachine( presentState, msgIn, procFunc, newState0, newState1 ) \
        prmStateMachine[ presentState ][msgIn].fp = procFunc; \
        prmStateMachine[ presentState ][msgIn].nextStates[0] = newState0;  \
        prmStateMachine[ presentState ][msgIn].nextStates[1] = newState1;

/**
 * @details This function is used to set the log level dynamically
 */
static inline void prmProcSetLogCfgInAllStates
(
  IN Msg           *msg,
  IO PrmContext    *prmContext
)
{
  UNUSED(prmContext);
  MsgBsmPrmSetLogCfg *updateLogConfig = (MsgBsmPrmSetLogCfg *)msg->data;
  LogContext  *logContext  = getLogContext();
  copyBytes(updateLogConfig,sizeof(LogConfig),&(logContext->logConfig));
  logInfo("%s", "Loglevel is set to new values");
}

/*
 * @details : This function will initialize the PRM state machine.
 **/
void initPrmStateMachine(
  void
  )
{
  int i,j;
  /* Initialize the Function pointer to process Invalid Message
   * Then fill the valid processing functions
   */
  for(i=0; i < MAX_STATE; i++)
  {
    for(j=0; j < MSG_ID_MAX; j++ )
    {
      prmStateMachine[i][j].fp = NULL;
    }
  }
  /*                   Present state    Message Received        Function to process      Next State */

  /*Lowerarm Exit Cmd for Base station Stop*/
  InitPrmStateMachine( IDLE,               MSG_IMC_PRM_PHYSHUTDOWN_CMD, prmProcPhyshutdown,             IDLE,                WAIT_EXIT_COMPLETE );
  InitPrmStateMachine( WAIT_HELLO_ACK,     MSG_IMC_PRM_PHYSHUTDOWN_CMD, prmProcPhyshutdown,             WAIT_HELLO_ACK,               WAIT_EXIT_COMPLETE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_IMC_PRM_PHYSHUTDOWN_CMD, prmProcPhyshutdown,             WAIT_INIT_COMPLETE,           WAIT_EXIT_COMPLETE );
  InitPrmStateMachine( WAIT_EXIT_COMPLETE, MSG_IMC_PRM_PHYSHUTDOWN_CMD, prmProcPhyshutdown,             WAIT_INIT_COMPLETE, WAIT_EXIT_COMPLETE );
  /* Any timer Expiry */
  InitPrmStateMachine( IDLE,               MSG_TIMEKEEPER_PRM_TIMEOUT_IND, prmProcTimerExpiryInIdle,             IDLE,               WAIT_HELLO_ACK     );
  InitPrmStateMachine( WAIT_HELLO_ACK,     MSG_TIMEKEEPER_PRM_TIMEOUT_IND, prmProcTimerExpiryInWaitHelloAck,     IDLE,               UNDEFINED          );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_TIMEKEEPER_PRM_TIMEOUT_IND, prmProcTimerExpiryInWaitInitComplete, IDLE,               WAIT_INIT_COMPLETE );
  InitPrmStateMachine( WAIT_EXIT_COMPLETE, MSG_TIMEKEEPER_PRM_TIMEOUT_IND, prmProcTimerExpiryInWaitExitComplete, WAIT_INIT_COMPLETE, WAIT_EXIT_COMPLETE );

  /* SET Message Trace in all states */
  InitPrmStateMachine( IDLE,               MSG_BSM_PRM_SET_MSG_TRACE, prmProcSetMsgTraceInAllStates, IDLE,               UNDEFINED );
  InitPrmStateMachine( WAIT_HELLO_ACK,     MSG_BSM_PRM_SET_MSG_TRACE, prmProcSetMsgTraceInAllStates, WAIT_HELLO_ACK,     UNDEFINED );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_BSM_PRM_SET_MSG_TRACE, prmProcSetMsgTraceInAllStates, WAIT_INIT_COMPLETE, UNDEFINED );
  InitPrmStateMachine( WAIT_EXIT_COMPLETE, MSG_BSM_PRM_SET_MSG_TRACE, prmProcSetMsgTraceInAllStates, WAIT_INIT_COMPLETE, UNDEFINED );

/* IDLE STATE = Received START_IND */
  InitPrmStateMachine( IDLE, MSG_FTH_PRM_START_IND,  prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_SCTP_PRM_START_IND, prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_EGTP_PRM_START_IND, prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_TUCL_PRM_START_IND, prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_MAC_PRM_START_IND,  prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_PDCP_PRM_START_IND, prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_RLC_PRM_START_IND,  prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_RRC_PRM_START_IND,  prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_S1AP_PRM_START_IND, prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_UEH_PRM_START_IND,  prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_DEH_PRM_START_IND,  prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_SIH_PRM_START_IND,  prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_WIH_PRM_START_IND,  prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_BSM_PRM_START_IND,  prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_LTH_PRM_START_IND,  prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_PMH_PRM_START_IND,  prmProcStartIndInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_UDH_PRM_START_IND,  prmProcStartIndInIdle, IDLE, UNDEFINED );
  /* WAIT_INIT_COMPLETE STATE = Received START_IND */
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_FTH_PRM_START_IND,  prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_SCTP_PRM_START_IND, prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_EGTP_PRM_START_IND, prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_TUCL_PRM_START_IND, prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_MAC_PRM_START_IND,  prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_PDCP_PRM_START_IND, prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_RLC_PRM_START_IND,  prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_RRC_PRM_START_IND,  prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_S1AP_PRM_START_IND, prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_UEH_PRM_START_IND,  prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_DEH_PRM_START_IND,  prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_SIH_PRM_START_IND,  prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_WIH_PRM_START_IND,  prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_BSM_PRM_START_IND,  prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_LTH_PRM_START_IND,  prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_PMH_PRM_START_IND,  prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_UDH_PRM_START_IND,  prmProcStartIndInWaitInitComplete, WAIT_INIT_COMPLETE, IDLE );

  /* IDLE state = Received HELLO_ACK */
  InitPrmStateMachine( IDLE, MSG_FTH_PRM_HELLO_ACK,  prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_SCTP_PRM_HELLO_ACK, prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_EGTP_PRM_HELLO_ACK, prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_TUCL_PRM_HELLO_ACK, prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_MAC_PRM_HELLO_ACK,  prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_PDCP_PRM_HELLO_ACK, prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_RLC_PRM_HELLO_ACK,  prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_RRC_PRM_HELLO_ACK,  prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_S1AP_PRM_HELLO_ACK, prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_UEH_PRM_HELLO_ACK,  prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_DEH_PRM_HELLO_ACK,  prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_SIH_PRM_HELLO_ACK,  prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_WIH_PRM_HELLO_ACK,  prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_BSM_PRM_HELLO_ACK,  prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_LTH_PRM_HELLO_ACK,  prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_PMH_PRM_HELLO_ACK,  prmProcHelloAckInIdle, IDLE, UNDEFINED );
  InitPrmStateMachine( IDLE, MSG_UDH_PRM_HELLO_ACK,  prmProcHelloAckInIdle, IDLE, UNDEFINED );
  /* WAIT_HELLO_ACK STATE = Received HELLO_ACK */
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_FTH_PRM_HELLO_ACK,  prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_SCTP_PRM_HELLO_ACK, prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_EGTP_PRM_HELLO_ACK, prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_TUCL_PRM_HELLO_ACK, prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_MAC_PRM_HELLO_ACK,  prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_PDCP_PRM_HELLO_ACK, prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_RLC_PRM_HELLO_ACK,  prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_RRC_PRM_HELLO_ACK,  prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_S1AP_PRM_HELLO_ACK, prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_UEH_PRM_HELLO_ACK,  prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_DEH_PRM_HELLO_ACK,  prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_SIH_PRM_HELLO_ACK,  prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_WIH_PRM_HELLO_ACK,  prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_BSM_PRM_HELLO_ACK,  prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_LTH_PRM_HELLO_ACK,  prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_PMH_PRM_HELLO_ACK,  prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_UDH_PRM_HELLO_ACK,  prmProcHelloAckInWaitHelloAck, WAIT_HELLO_ACK, IDLE );

  /* RESTART REQ */
  InitPrmStateMachine( IDLE,           MSG_BSM_PRM_RESTART_REQ, prmProcRestartReq, WAIT_EXIT_COMPLETE, UNDEFINED );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_BSM_PRM_RESTART_REQ, prmProcRestartReq, WAIT_EXIT_COMPLETE, UNDEFINED );

  /* SW Activation REQ */
  InitPrmStateMachine( IDLE,           MSG_BSM_PRM_SW_ACTIVATE_REQ, prmProcSwActivateReq, IDLE,           WAIT_EXIT_COMPLETE );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_BSM_PRM_SW_ACTIVATE_REQ, prmProcSwActivateReq, WAIT_HELLO_ACK, WAIT_EXIT_COMPLETE );

  /* SW Install REQ */
  InitPrmStateMachine( IDLE,               MSG_BSM_PRM_SW_INSTALL_REQ, prmProcSwInstallReq, IDLE,               WAIT_EXIT_COMPLETE );
  InitPrmStateMachine( WAIT_HELLO_ACK,     MSG_BSM_PRM_SW_INSTALL_REQ, prmProcSwInstallReq, WAIT_HELLO_ACK,     WAIT_EXIT_COMPLETE );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_BSM_PRM_SW_INSTALL_REQ, prmProcSwInstallReq, WAIT_INIT_COMPLETE, WAIT_EXIT_COMPLETE );
  InitPrmStateMachine( WAIT_EXIT_COMPLETE, MSG_BSM_PRM_SW_INSTALL_REQ, prmProcSwInstallReq, WAIT_EXIT_COMPLETE, WAIT_INIT_COMPLETE );

 /* Delete  SW Install History table */ 
  InitPrmStateMachine( IDLE,               MSG_BSM_PRM_SET_OBJ_REQ, prmProcDeleteSwInstallHistoryTableReq, IDLE, UNDEFINED  );
  InitPrmStateMachine( WAIT_HELLO_ACK,     MSG_BSM_PRM_SET_OBJ_REQ, prmProcDeleteSwInstallHistoryTableReq, WAIT_HELLO_ACK, UNDEFINED  );

  InitPrmStateMachine( IDLE,               MSG_BSM_PRM_CONFIG_INSTALL_SUCC, prmProcSwConfigInstallSucc, IDLE,               UNDEFINED );
  InitPrmStateMachine( WAIT_HELLO_ACK,     MSG_BSM_PRM_CONFIG_INSTALL_SUCC, prmProcSwConfigInstallSucc, WAIT_HELLO_ACK,     UNDEFINED );

  InitPrmStateMachine( IDLE,               MSG_BSM_PRM_CONFIG_INSTALL_FAIL, prmProcSwConfigInstallFail, IDLE,               UNDEFINED );
  InitPrmStateMachine( WAIT_HELLO_ACK,     MSG_BSM_PRM_CONFIG_INSTALL_FAIL, prmProcSwConfigInstallFail, WAIT_HELLO_ACK,     UNDEFINED );

  /* REBOOT REQ */
  InitPrmStateMachine( IDLE,           MSG_BSM_PRM_REBOOT_REQ, prmProcRebootReq, WAIT_EXIT_COMPLETE, UNDEFINED );
  InitPrmStateMachine( WAIT_HELLO_ACK, MSG_BSM_PRM_REBOOT_REQ, prmProcRebootReq, WAIT_EXIT_COMPLETE, UNDEFINED );

  /* Get Request on tables which are with PRM */
  InitPrmStateMachine( IDLE,               MSG_BSM_PRM_GET_OBJ_REQ, prmProcGetObjReqInAllStates, IDLE,               UNDEFINED );
  InitPrmStateMachine( WAIT_HELLO_ACK,     MSG_BSM_PRM_GET_OBJ_REQ, prmProcGetObjReqInAllStates, WAIT_HELLO_ACK,     UNDEFINED );
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_BSM_PRM_GET_OBJ_REQ, prmProcGetObjReqInAllStates, WAIT_INIT_COMPLETE, UNDEFINED );
  InitPrmStateMachine( WAIT_EXIT_COMPLETE, MSG_BSM_PRM_GET_OBJ_REQ, prmProcGetObjReqInAllStates, WAIT_EXIT_COMPLETE, UNDEFINED );

  InitPrmStateMachine( IDLE,               MSG_BSM_PRM_SET_LOG_CFG, prmProcSetLogCfgInAllStates, IDLE,               IDLE);
  InitPrmStateMachine( WAIT_HELLO_ACK,     MSG_BSM_PRM_SET_LOG_CFG, prmProcSetLogCfgInAllStates, WAIT_HELLO_ACK,     WAIT_HELLO_ACK);
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_BSM_PRM_SET_LOG_CFG, prmProcSetLogCfgInAllStates, WAIT_INIT_COMPLETE, WAIT_INIT_COMPLETE);
  InitPrmStateMachine( WAIT_EXIT_COMPLETE, MSG_BSM_PRM_SET_LOG_CFG, prmProcSetLogCfgInAllStates, WAIT_EXIT_COMPLETE, WAIT_EXIT_COMPLETE);

  /* Success Response of RH_SW_INSTALL_REQ */
  InitPrmStateMachine( IDLE,               MSG_DEH_PRM_RH_SW_INSTALL_SUCC, prmProcRhSwInstallSucc, IDLE,               IDLE);
  InitPrmStateMachine( WAIT_HELLO_ACK,     MSG_DEH_PRM_RH_SW_INSTALL_SUCC, prmProcRhSwInstallSucc, WAIT_HELLO_ACK,     WAIT_HELLO_ACK);
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_DEH_PRM_RH_SW_INSTALL_SUCC, prmProcRhSwInstallSucc, WAIT_INIT_COMPLETE, WAIT_INIT_COMPLETE);
  InitPrmStateMachine( WAIT_EXIT_COMPLETE, MSG_DEH_PRM_RH_SW_INSTALL_SUCC, prmProcRhSwInstallSucc, WAIT_EXIT_COMPLETE, WAIT_EXIT_COMPLETE);

  /* Failure Response of RH_SW_INSTALL_REQ */
  InitPrmStateMachine( IDLE,               MSG_DEH_PRM_RH_SW_INSTALL_FAIL, prmProcRhSwInstallFail, IDLE,               IDLE);
  InitPrmStateMachine( WAIT_HELLO_ACK,     MSG_DEH_PRM_RH_SW_INSTALL_FAIL, prmProcRhSwInstallFail, WAIT_HELLO_ACK,     WAIT_HELLO_ACK);
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_DEH_PRM_RH_SW_INSTALL_FAIL, prmProcRhSwInstallFail, WAIT_INIT_COMPLETE, WAIT_INIT_COMPLETE);
  InitPrmStateMachine( WAIT_EXIT_COMPLETE, MSG_DEH_PRM_RH_SW_INSTALL_FAIL, prmProcRhSwInstallFail, WAIT_EXIT_COMPLETE, WAIT_EXIT_COMPLETE);

  /* Success Response of RH_SW_ACTIVATE_REQ */
  InitPrmStateMachine( IDLE,               MSG_DEH_PRM_RH_SW_ACTIVATE_SUCC, prmProcRhSwActivateSucc, IDLE,               IDLE);
  InitPrmStateMachine( WAIT_HELLO_ACK,     MSG_DEH_PRM_RH_SW_ACTIVATE_SUCC, prmProcRhSwActivateSucc, WAIT_HELLO_ACK,     WAIT_HELLO_ACK);
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_DEH_PRM_RH_SW_ACTIVATE_SUCC, prmProcRhSwActivateSucc, WAIT_INIT_COMPLETE, WAIT_INIT_COMPLETE);
  InitPrmStateMachine( WAIT_EXIT_COMPLETE, MSG_DEH_PRM_RH_SW_ACTIVATE_SUCC, prmProcRhSwActivateSucc, WAIT_EXIT_COMPLETE, WAIT_EXIT_COMPLETE);

  /* Failure Response of RH_SW_ACTIVATE_REQ */
  InitPrmStateMachine( IDLE,               MSG_DEH_PRM_RH_SW_ACTIVATE_FAIL, prmProcRhSwActivateFail, IDLE,               IDLE);
  InitPrmStateMachine( WAIT_HELLO_ACK,     MSG_DEH_PRM_RH_SW_ACTIVATE_FAIL, prmProcRhSwActivateFail, WAIT_HELLO_ACK,     WAIT_HELLO_ACK);
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_DEH_PRM_RH_SW_ACTIVATE_FAIL, prmProcRhSwActivateFail, WAIT_INIT_COMPLETE, WAIT_INIT_COMPLETE);
  InitPrmStateMachine( WAIT_EXIT_COMPLETE, MSG_DEH_PRM_RH_SW_ACTIVATE_FAIL, prmProcRhSwActivateFail, WAIT_EXIT_COMPLETE, WAIT_EXIT_COMPLETE);

  /* LK version info received from DEH */
  InitPrmStateMachine( IDLE,               MSG_DEH_PRM_RH_VERSION_INFO, prmProcRhversionInfo, IDLE,               IDLE);
  InitPrmStateMachine( WAIT_HELLO_ACK,     MSG_DEH_PRM_RH_VERSION_INFO, prmProcRhversionInfo, WAIT_HELLO_ACK,     WAIT_HELLO_ACK);
  InitPrmStateMachine( WAIT_INIT_COMPLETE, MSG_DEH_PRM_RH_VERSION_INFO, prmProcRhversionInfo, WAIT_INIT_COMPLETE, WAIT_INIT_COMPLETE);
  InitPrmStateMachine( WAIT_EXIT_COMPLETE, MSG_DEH_PRM_RH_VERSION_INFO, prmProcRhversionInfo, WAIT_EXIT_COMPLETE, WAIT_EXIT_COMPLETE);

}

/**
 * @details This is the entry point for all messages coming into PRM
 */
void prmMain(
  FR Msg *msgIn
  )
{
  PrmState prevState;
  PrmContext *prmContext = getPrmContext();

  logInfo("Received Message=%s, in state=%s", getMsgName(msgIn->msgId), getPrmStateName(prmContext->prmState) );

  if( NULL NOTEQ prmStateMachine[prmContext->prmState][msgIn->msgId].fp ) /* TODO: This needs to be optimised as the array is sparse */
  {
    prevState = prmContext->prmState;
    prmStateMachine[prmContext->prmState][msgIn->msgId].fp( msgIn, prmContext);
    if( (prmContext->prmState NOTEQ prmStateMachine[ prevState ][msgIn->msgId].nextStates[0]) &&
        (prmContext->prmState NOTEQ prmStateMachine[ prevState ][msgIn->msgId].nextStates[1]) )
    {
      logInfo("PROGRAMMING ERROR !! Not In expected State.  Message=%s, Previous state=%d , Current State = %d (Expected States :%d and %d )",
                                          getMsgName(msgIn->msgId), prevState, prmContext->prmState,
                                          prmStateMachine[ prevState ][msgIn->msgId].nextStates[0],
                                          prmStateMachine[ prevState ][msgIn->msgId].nextStates[1]);
    }
  }
  else
  {
    BS_ERROR_RECEIVED_INVALID_MSG( msgIn->msgId, prmContext->prmState );
  }
  printAndEmptyErrorChain(&(getModuleContext()->errChain));
  /* Free the received Message */
  freeMsgIn(msgIn);
}

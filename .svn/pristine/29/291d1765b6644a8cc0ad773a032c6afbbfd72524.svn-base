/**
 * @file    pmh_main.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will handle all the messages coming from different modules
 *
 * @author  Ramprasad Reddy Chinta
 * @date    10-02-2011
 * @details This file contains functions to process different messages received from other modules
 */

#include "pmh.h"

/* Set the state machine */
#define InitPmhStateMachine( presentState, msgIn, procFunc ) \
            pmhStateMachine[ presentState ][msgIn].fp = procFunc; \

/* ################### GLOBAL VARIABLES ############### */
PmhContext          g_PmhContext;
PmhStateMachine     pmhStateMachine[PMH_MAX_STATE][MSG_ID_MAX];

/**
 * @param   state   PMH state enum value
 * @details Returns the state name in string format
 **/
char *getPmhStateName(
  IN PmhState state
  )
{
  switch( state )
  {
    case PMH_IDLE:                           return "PMH_IDLE";
    case PMH_WAIT_INIT_CFG:                  return "PMH_WAIT_INIT_CFG";
    case PMH_WAIT_OPSTART:                   return "PMH_WAIT_OPSTART";
    case PMH_WAIT_BULK_COUNTER_COLLECTION:   return "PMH_WAIT_BULK_COUNTER_COLLECTION";
    default:
      logError( "No name defined for this PMH state:%d", state );
      return "No name defined for this BSM state";
  }
}

/**
 * @param   ctx     PMH context information.
 * @param   state   PMH State value
 * @details Sets PMH state.
 **/
void setPmhState(
  IO PmhContext   *ctx,
  IN PmhState     state
  )
{
  ctx->pmhState = state;
  logInfo(" PMH state changed to :%s" , getPmhStateName(state));
}

/**
 * @details This function initialises PMH state machine
 **/
void initPmhStateMachine(
    void
    )
{
  int i,j;

  /* Initialize the Function pointer to process Invalid Message
   * Then fill the valid processing functions
   */
  for(i=0; i< PMH_MAX_STATE; i++)
  {
    for(j=0; j<MSG_ID_MAX; j++ )
    {
      pmhStateMachine[i][j].fp = NULL;
    }
  }

  /* Initialise PMH state machine for TIMEOUT procedure */
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,                MSG_TIMEKEEPER_PMH_TIMEOUT_IND, pmhProcTimeOutIndInWaitInitAndWaitOpStartState );
  InitPmhStateMachine( PMH_WAIT_OPSTART,                 MSG_TIMEKEEPER_PMH_TIMEOUT_IND, pmhProcTimeOutIndInWaitInitAndWaitOpStartState );
  InitPmhStateMachine( PMH_IDLE,                         MSG_TIMEKEEPER_PMH_TIMEOUT_IND, pmhProcTimeOutIndInIdleState );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION, MSG_TIMEKEEPER_PMH_TIMEOUT_IND, pmhProcTimeOutIndInWaitBulkCounterCollectionState );

  /* Initialise PMH state machine for Health Checkprocedure */
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,                MSG_PRM_PMH_HELLO, pmhProcHelloReq );
  InitPmhStateMachine( PMH_WAIT_OPSTART,                 MSG_PRM_PMH_HELLO, pmhProcHelloReq );
  InitPmhStateMachine( PMH_IDLE,                         MSG_PRM_PMH_HELLO, pmhProcHelloReq );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION, MSG_PRM_PMH_HELLO, pmhProcHelloReq );

  /* Initialise PMH state machine for config procedure */
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,                MSG_BSM_PMH_CONFIG_REQ, pmhProcConfigReqInWaitInitAndWaitOpstart );
  InitPmhStateMachine( PMH_WAIT_OPSTART,                 MSG_BSM_PMH_CONFIG_REQ, pmhProcConfigReqInWaitInitAndWaitOpstart );
  InitPmhStateMachine( PMH_IDLE,                         MSG_BSM_PMH_CONFIG_REQ, pmhProcConfigReqInIdleState );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION, MSG_BSM_PMH_CONFIG_REQ, pmhProcConfigReqInWaitBulkCollection );

  /* Initialise PMH state machine for COUNTER_INPUT config procedure */
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,                MSG_BSM_PMH_COUNTER_INPUT_CONFIG_REQ, pmhProcCntrInputConfigReq );
  InitPmhStateMachine( PMH_WAIT_OPSTART,                 MSG_BSM_PMH_COUNTER_INPUT_CONFIG_REQ, pmhProcCntrInputConfigReq );
  InitPmhStateMachine( PMH_IDLE,                         MSG_BSM_PMH_COUNTER_INPUT_CONFIG_REQ, pmhProcMsgInUnexpectedState);
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION, MSG_BSM_PMH_COUNTER_INPUT_CONFIG_REQ, pmhProcMsgInUnexpectedState);

  /* Initialise PMH state machine for COUNTER_CLEAR Request procedure */
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,                MSG_BSM_PMH_COUNTER_CLEAR_REQ, pmhProcCounterClearReqInWaitInitAndWaitOpstart );
  InitPmhStateMachine( PMH_WAIT_OPSTART,                 MSG_BSM_PMH_COUNTER_CLEAR_REQ, pmhProcCounterClearReqInWaitInitAndWaitOpstart );
  InitPmhStateMachine( PMH_IDLE,                         MSG_BSM_PMH_COUNTER_CLEAR_REQ, pmhProcMsgInUnexpectedState );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION, MSG_BSM_PMH_COUNTER_CLEAR_REQ, pmhProcMsgInUnexpectedState );

  /* Initialise PMH state machine for DIAG_COUNTER_CMD procedure */
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,                MSG_BSM_PMH_DIAG_COUNTER_CMD, pmhProcDiagCounterCmdInAllState );
  InitPmhStateMachine( PMH_WAIT_OPSTART,                 MSG_BSM_PMH_DIAG_COUNTER_CMD, pmhProcDiagCounterCmdInAllState );
  InitPmhStateMachine( PMH_IDLE,                         MSG_BSM_PMH_DIAG_COUNTER_CMD, pmhProcDiagCounterCmdInAllState );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION, MSG_BSM_PMH_DIAG_COUNTER_CMD, pmhProcDiagCounterCmdInAllState );

  /* Initialise PMH state machine for UNLOCK procedure */
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,                MSG_BSM_PMH_OPSTART_REQ, pmhProcMsgInUnexpectedState );
  InitPmhStateMachine( PMH_WAIT_OPSTART,                 MSG_BSM_PMH_OPSTART_REQ, pmhProcOpStartReq );
  InitPmhStateMachine( PMH_IDLE,                         MSG_BSM_PMH_OPSTART_REQ, pmhProcMsgInUnexpectedState );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION, MSG_BSM_PMH_OPSTART_REQ, pmhProcMsgInUnexpectedState );

  /* Initialise PMH state machine for LOCK procedure */
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,                MSG_BSM_PMH_OPSTOP_REQ, pmhProcOpStopReqInWaitInitCfg);
  InitPmhStateMachine( PMH_WAIT_OPSTART,                 MSG_BSM_PMH_OPSTOP_REQ, pmhProcOpStopReqInWaitOpStart);
  InitPmhStateMachine( PMH_IDLE,                         MSG_BSM_PMH_OPSTOP_REQ, pmhProcOpStopReq);
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION, MSG_BSM_PMH_OPSTOP_REQ, pmhProcOpStopReq);

  /* Initialise PMH state machine for GET_OBJ procedure */
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,                MSG_BSM_PMH_GET_OBJ_REQ, pmhProcGetObjReq );
  InitPmhStateMachine( PMH_WAIT_OPSTART,                 MSG_BSM_PMH_GET_OBJ_REQ, pmhProcGetObjReq );
  InitPmhStateMachine( PMH_IDLE,                         MSG_BSM_PMH_GET_OBJ_REQ, pmhProcGetObjReq );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION, MSG_BSM_PMH_GET_OBJ_REQ, pmhProcGetObjReq );

  /* Iniliaze PMH state machine for File Uploaded indication */
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,                MSG_BSM_PMH_PM_FILE_UPLOADED, pmhProcPmFileUploadedMsg );
  InitPmhStateMachine( PMH_WAIT_OPSTART,                 MSG_BSM_PMH_PM_FILE_UPLOADED, pmhProcPmFileUploadedMsg );
  InitPmhStateMachine( PMH_IDLE,                         MSG_BSM_PMH_PM_FILE_UPLOADED, pmhProcPmFileUploadedMsgInIdleState );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION, MSG_BSM_PMH_PM_FILE_UPLOADED, pmhProcPmFileUploadedMsg );

  //Initialise SET_LOG_CFG handler in all states
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,                MSG_BSM_PMH_SET_LOG_CFG, pmhProcSetLogCfgInAllStates);
  InitPmhStateMachine( PMH_WAIT_OPSTART,                 MSG_BSM_PMH_SET_LOG_CFG, pmhProcSetLogCfgInAllStates);
  InitPmhStateMachine( PMH_IDLE,                         MSG_BSM_PMH_SET_LOG_CFG, pmhProcSetLogCfgInAllStates);
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION, MSG_BSM_PMH_SET_LOG_CFG, pmhProcSetLogCfgInAllStates);

  /* Initialise PMH state machine for EXIT procedure */
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,               MSG_PRM_PMH_EXIT_CMD, pmhProcExit );
  InitPmhStateMachine( PMH_WAIT_OPSTART,                MSG_PRM_PMH_EXIT_CMD, pmhProcExit );
  InitPmhStateMachine( PMH_IDLE,                        MSG_PRM_PMH_EXIT_CMD, pmhProcExit );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,MSG_PRM_PMH_EXIT_CMD, pmhProcExit );

  /* Initialise PMH state machine for Statistics responses */
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_SM_PMH_PDCP_GEN_STAT_RSP,       pmhProcPdcpGenStsCfm );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_SM_PMH_PDCP_UPLANE_STAT_RSP,    pmhProcPdcpUplaneStsCfm );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_SM_PMH_SCTP_GEN_STAT_RSP,       pmhProcSctpGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_SM_PMH_EGTP_GEN_STAT_RSP,       pmhProcGtpGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_SM_PMH_RLC_GEN_STAT_RSP,        pmhProcRlcGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_SM_PMH_MAC_GEN_STAT_RSP,        pmhProcMacGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_SM_PMH_MAC_RLCSAP_STAT_RSP,     pmhProcMacRlcsapStatRsp );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_SM_PMH_MAC_PHYSAP_STAT_RSP,     pmhProcMacPhysapStatRsp );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_SM_PMH_RRC_GEN_STAT_RSP,        pmhProcRrcGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_SM_PMH_RRC_CELL_STAT_RSP,       pmhProcRrcCellStsRsp );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_SM_PMH_S1AP_GEN_STAT_RSP,       pmhProcS1apGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_SM_PMH_S1AP_PEER_NODE_STAT_RSP, pmhProcS1apPeerNodeStsRsp );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_UEH_PMH_RRM_STAT_RSP,           pmhProcRrmStsRsp );

  InitPmhStateMachine( PMH_IDLE,  MSG_SM_PMH_PDCP_GEN_STAT_RSP,       pmhProcPdcpGenStsCfm );
  InitPmhStateMachine( PMH_IDLE,  MSG_SM_PMH_PDCP_UPLANE_STAT_RSP,    pmhProcPdcpUplaneStsCfm );
  InitPmhStateMachine( PMH_IDLE,  MSG_SM_PMH_SCTP_GEN_STAT_RSP,       pmhProcSctpGenStsRsp );
  InitPmhStateMachine( PMH_IDLE,  MSG_SM_PMH_EGTP_GEN_STAT_RSP,       pmhProcGtpGenStsRsp );
  InitPmhStateMachine( PMH_IDLE,  MSG_SM_PMH_RLC_GEN_STAT_RSP,        pmhProcRlcGenStsRsp );
  InitPmhStateMachine( PMH_IDLE,  MSG_SM_PMH_MAC_GEN_STAT_RSP,        pmhProcMacGenStsRsp );
  InitPmhStateMachine( PMH_IDLE,  MSG_SM_PMH_MAC_RLCSAP_STAT_RSP,     pmhProcMacRlcsapStatRsp );
  InitPmhStateMachine( PMH_IDLE,  MSG_SM_PMH_MAC_PHYSAP_STAT_RSP,     pmhProcMacPhysapStatRsp );
  InitPmhStateMachine( PMH_IDLE,  MSG_SM_PMH_RRC_GEN_STAT_RSP,        pmhProcRrcGenStsRsp );
  InitPmhStateMachine( PMH_IDLE,  MSG_SM_PMH_RRC_CELL_STAT_RSP,       pmhProcRrcCellStsRsp );
  InitPmhStateMachine( PMH_IDLE,  MSG_SM_PMH_S1AP_GEN_STAT_RSP,       pmhProcS1apGenStsRsp );
  InitPmhStateMachine( PMH_IDLE,  MSG_SM_PMH_S1AP_PEER_NODE_STAT_RSP, pmhProcS1apPeerNodeStsRsp );
  InitPmhStateMachine( PMH_IDLE,  MSG_UEH_PMH_RRM_STAT_RSP,           pmhProcRrmStsRsp );

  InitPmhStateMachine( PMH_WAIT_INIT_CFG,  MSG_SM_PMH_PDCP_GEN_STAT_RSP,       pmhProcPdcpGenStsCfm );
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,  MSG_SM_PMH_PDCP_UPLANE_STAT_RSP,    pmhProcPdcpUplaneStsCfm );
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,  MSG_SM_PMH_SCTP_GEN_STAT_RSP,       pmhProcSctpGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,  MSG_SM_PMH_EGTP_GEN_STAT_RSP,       pmhProcGtpGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,  MSG_SM_PMH_RLC_GEN_STAT_RSP,        pmhProcRlcGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,  MSG_SM_PMH_MAC_GEN_STAT_RSP,        pmhProcMacGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,  MSG_SM_PMH_MAC_RLCSAP_STAT_RSP,     pmhProcMacRlcsapStatRsp );
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,  MSG_SM_PMH_MAC_PHYSAP_STAT_RSP,     pmhProcMacPhysapStatRsp );
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,  MSG_SM_PMH_RRC_GEN_STAT_RSP,        pmhProcRrcGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,  MSG_SM_PMH_RRC_CELL_STAT_RSP,       pmhProcRrcCellStsRsp );
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,  MSG_SM_PMH_S1AP_GEN_STAT_RSP,       pmhProcS1apGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,  MSG_SM_PMH_S1AP_PEER_NODE_STAT_RSP, pmhProcS1apPeerNodeStsRsp );
  InitPmhStateMachine( PMH_WAIT_INIT_CFG,  MSG_UEH_PMH_RRM_STAT_RSP,           pmhProcRrmStsRsp );

  InitPmhStateMachine( PMH_WAIT_OPSTART,  MSG_SM_PMH_PDCP_GEN_STAT_RSP,       pmhProcPdcpGenStsCfm );
  InitPmhStateMachine( PMH_WAIT_OPSTART,  MSG_SM_PMH_PDCP_UPLANE_STAT_RSP,    pmhProcPdcpUplaneStsCfm );
  InitPmhStateMachine( PMH_WAIT_OPSTART,  MSG_SM_PMH_SCTP_GEN_STAT_RSP,       pmhProcSctpGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_OPSTART,  MSG_SM_PMH_EGTP_GEN_STAT_RSP,       pmhProcGtpGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_OPSTART,  MSG_SM_PMH_RLC_GEN_STAT_RSP,        pmhProcRlcGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_OPSTART,  MSG_SM_PMH_MAC_GEN_STAT_RSP,        pmhProcMacGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_OPSTART,  MSG_SM_PMH_MAC_RLCSAP_STAT_RSP,     pmhProcMacRlcsapStatRsp );
  InitPmhStateMachine( PMH_WAIT_OPSTART,  MSG_SM_PMH_MAC_PHYSAP_STAT_RSP,     pmhProcMacPhysapStatRsp );
  InitPmhStateMachine( PMH_WAIT_OPSTART,  MSG_SM_PMH_RRC_GEN_STAT_RSP,        pmhProcRrcGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_OPSTART,  MSG_SM_PMH_RRC_CELL_STAT_RSP,       pmhProcRrcCellStsRsp );
  InitPmhStateMachine( PMH_WAIT_OPSTART,  MSG_SM_PMH_S1AP_GEN_STAT_RSP,       pmhProcS1apGenStsRsp );
  InitPmhStateMachine( PMH_WAIT_OPSTART,  MSG_SM_PMH_S1AP_PEER_NODE_STAT_RSP, pmhProcS1apPeerNodeStsRsp );
  InitPmhStateMachine( PMH_WAIT_OPSTART,  MSG_UEH_PMH_RRM_STAT_RSP,           pmhProcRrmStsRsp );

  InitPmhStateMachine( PMH_IDLE,  MSG_TFU_PMH_DSTATS_IND,           pmhProcDstatsInd );
  InitPmhStateMachine( PMH_IDLE,  MSG_UDH_PMH_DSTATS_IND,            pmhProcUdhDStatsInd );

  InitPmhStateMachine( PMH_WAIT_INIT_CFG,                 MSG_BSM_PMH_SET_OBJ_REQ, pmhProcSetObjReq );
  InitPmhStateMachine( PMH_WAIT_OPSTART,                  MSG_BSM_PMH_SET_OBJ_REQ, pmhProcSetObjReq );
  InitPmhStateMachine( PMH_IDLE,                          MSG_BSM_PMH_SET_OBJ_REQ, pmhProcSetObjReq );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_BSM_PMH_SET_OBJ_REQ, pmhProcSetObjReq );

  InitPmhStateMachine( PMH_WAIT_OPSTART,                  MSG_UEH_PMH_COUNTER_START_SUCC, pmhProcCounterStartSucc );
  InitPmhStateMachine( PMH_IDLE,                          MSG_UEH_PMH_COUNTER_START_SUCC, pmhProcCounterStartSucc );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_UEH_PMH_COUNTER_START_SUCC, pmhProcCounterStartSucc );

  InitPmhStateMachine( PMH_WAIT_OPSTART,                  MSG_UEH_PMH_COUNTER_START_FAIL, pmhProcCounterStartFail );
  InitPmhStateMachine( PMH_IDLE,                          MSG_UEH_PMH_COUNTER_START_FAIL, pmhProcCounterStartFail );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_UEH_PMH_COUNTER_START_FAIL, pmhProcCounterStartFail );

  InitPmhStateMachine( PMH_WAIT_OPSTART,                  MSG_UEH_PMH_COUNTER_STOP_SUCC, pmhProcCounterStopSucc );
  InitPmhStateMachine( PMH_IDLE,                          MSG_UEH_PMH_COUNTER_STOP_SUCC, pmhProcCounterStopSucc );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_UEH_PMH_COUNTER_STOP_SUCC, pmhProcCounterStopSucc );

  InitPmhStateMachine( PMH_WAIT_OPSTART,                  MSG_UEH_PMH_COUNTER_STOP_IND, pmhProcCounterStopInd );
  InitPmhStateMachine( PMH_IDLE,                          MSG_UEH_PMH_COUNTER_STOP_IND, pmhProcCounterStopInd );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_UEH_PMH_COUNTER_STOP_IND, pmhProcCounterStopInd );

  InitPmhStateMachine( PMH_WAIT_OPSTART,                  MSG_UEH_PMH_COUNTER_REPORT_IND, pmhProcCounterReportInd );
  InitPmhStateMachine( PMH_IDLE,                          MSG_UEH_PMH_COUNTER_REPORT_IND, pmhProcCounterReportInd );
  InitPmhStateMachine( PMH_WAIT_BULK_COUNTER_COLLECTION,  MSG_UEH_PMH_COUNTER_REPORT_IND, pmhProcCounterReportInd );

}

/**
 * @param   msgIn Received message
 * @details This is the entry point for all messages coming into PMH
 */
void pmhMain(
  FR Msg *msgIn
  )
{
  PmhContext *pmhContext = getPmhContext();

  logInfo("Received Message=%s in State=%s ", getMsgName(msgIn->msgId), getPmhStateName(pmhContext->pmhState) );

  if( NULL NOTEQ pmhStateMachine[pmhContext->pmhState][msgIn->msgId].fp )
  {
    pmhContext->pmhState = pmhStateMachine[pmhContext->pmhState][msgIn->msgId].fp( msgIn, pmhContext );
  }
  else
  {
    logError("No Processing function defined for this Message=%s", getMsgName(msgIn->msgId) );
  }

  /* Free the received Message */
  freeMsgIn(msgIn);
  printAndEmptyErrorChain(&(getModuleContext()->errChain));
}

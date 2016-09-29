/**
 * @file    bsm_main.c
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @brief   This file will handle all the messages coming from different module in any BSM state.
 *          Contains BSM state machine.
 *
 * @author:  Lakshmi Narayana Madala / Shweta Polepally
 * @date:    Oct 2009
 * @details: Initial Version
 *           Changes are made in the code as per the new design
 **/

#include "bsm.h"

/* Set the state machine */
#define InitBsmStateMachine( presentState, msgIn, procFunc ) \
            bsmStateMachine[ presentState ][msgIn].fp = procFunc;

/* ################### GLOBAL VARIABLES ############### */
BsmContext          g_BsmContext;
BsmStateMachine     bsmStateMachine[BSM_STATE_MAX][MSG_ID_MAX];
/* ################### Generic functions ############### */

/**
 * @param   ctx : BSM context information
 * @details This function initialises BSM context.
 **/
void initBsmContext(
  IO BsmContext *ctx
  )
{
  clearBytes( ctx, sizeof(BsmContext));
  ctx->bsmProceduralState = BS_WAIT_INIT_CMPL;
  /* Fix OPstart and OPstop sequence */
  ctx->opStartOpStopInfo.sequence[0] = MODID_PMH;
  ctx->opStartOpStopInfo.sequence[1] = MODID_S1M;
  ctx->opStartOpStopInfo.sequence[2] = MODID_CEM;
  ctx->opStartOpStopInfo.sequence[3] = MODID_UDH;
  ctx->opStartOpStopInfo.sequence[4] = MODID_DEH;

  ctx->cellCmdRcvd = CELLCOMMAND_UNBAR;
}

/**
 * @param   tableId : Table ID
 * @details This function returns pointer to BsmOamTableInfo for the table ID provided
 *          switching on the basis of table type.
 **/
BsmOamTableInfo* bsmGetTablePtrByTableId(
  IN TableId      tableId
  )
{
  OamTableType  tableType = getTableTypeByTableId(tableId);

  switch(tableType)
  {
    case TABLE_OPERCMD:     return getBsmOamTableContext().oprCmdTableInfo[tableId-MIN_OPR_CMD_TABLEID];
    case TABLE_OPERCONFIG:  return getBsmOamTableContext().oprCfgTableInfo[tableId-MIN_OPR_CFG_TABLEID];
    case TABLE_STATUS:      return getBsmOamTableContext().statusTableInfo[tableId-MIN_STATUS_TABLEID];
    case TABLE_PRECONFIG:   return getBsmOamTableContext().preCfgTableInfo[tableId-MIN_PRE_CFG_TABLEID];
    case TABLE_UNKNOWNTYPE: return NULL;
  }
  return NULL;
}

/**
 * @details This function initializes BSM state machine.
 **/
void initBsmStateMachine()
{
  int i,j;

  /* Initialize the Function pointer to process Invalid Message
   * Then fill the valid processing functions
   */
  for(i=0; i< BSM_STATE_MAX; i++)
  {
    for(j=0; j<MSG_ID_MAX; j++ )
    {
      bsmStateMachine[i][j].fp = NULL;
    }
  }

  /* Timer Expiry Message handling */
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_TIMEKEEPER_BSM_TIMEOUT_IND, bsmProcTimerExpiryInWaitInitialConfigCmpl );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,     MSG_TIMEKEEPER_BSM_TIMEOUT_IND, bsmProcTimerExpiryInWaitDeviceInitCmpl );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,    MSG_TIMEKEEPER_BSM_TIMEOUT_IND, bsmProcTimerExpiryInWaitCollectInfoCmpl );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,    MSG_TIMEKEEPER_BSM_TIMEOUT_IND, bsmProcTimerExpiryInWaitRegistrationCmpl );
  InitBsmStateMachine( BS_IDLE,                      MSG_TIMEKEEPER_BSM_TIMEOUT_IND, bsmProcTimerExpiryInIdle );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,      MSG_TIMEKEEPER_BSM_TIMEOUT_IND, bsmProcTimerExpiryInWaitConfigCmpl );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,      MSG_TIMEKEEPER_BSM_TIMEOUT_IND, bsmProcTimerExpiryInWaitEnableCmpl );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,     MSG_TIMEKEEPER_BSM_TIMEOUT_IND, bsmProcTimerExpiryInWaitDisableCmpl );

  /* All Modules Up in Wait Init Complete state */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,            MSG_PRM_BSM_ALL_MODULES_UP_IND, bsmProcAllModulesUpInd );

  /* Set Ipaddress response from DEH in Wait Init Config Complete state */
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_DEH_BSM_SET_IP_ADDR_RSP,    bsmProcSetIpAddrRsp );

  /* Config Succ or fail from SIH module during initial configuration */
  InitBsmStateMachine( BS_IDLE,                      MSG_SIH_BSM_INIT_CONFIG_SUCC,     bsmProcInitConfigSuccFromSih );
  InitBsmStateMachine( BS_IDLE,                      MSG_SIH_BSM_INIT_CONFIG_FAIL,     bsmProcInitConfigFailFromSih );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,    MSG_SIH_BSM_INIT_CONFIG_SUCC,     bsmProcInitConfigSuccFromSih );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,    MSG_SIH_BSM_INIT_CONFIG_FAIL,     bsmProcInitConfigFailFromSih );

  /* Config Succ or fail from UEH module for UE Event log configuration */
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_SUCC,     bsmProcUeEventHistoryLogConfigSuccFromUeh );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,     MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_SUCC,     bsmProcUeEventHistoryLogConfigSuccFromUeh );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,    MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_SUCC,     bsmProcUeEventHistoryLogConfigSuccFromUeh );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,    MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_SUCC,     bsmProcUeEventHistoryLogConfigSuccFromUeh );
  InitBsmStateMachine( BS_IDLE,                      MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_SUCC,     bsmProcUeEventHistoryLogConfigSuccFromUeh );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,     MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_SUCC,     bsmProcUeEventHistoryLogConfigSuccFromUeh );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,      MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_SUCC,     bsmProcUeEventHistoryLogConfigSuccFromUeh );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,      MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_SUCC,     bsmProcUeEventHistoryLogConfigSuccFromUeh );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,     MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_SUCC,     bsmProcUeEventHistoryLogConfigSuccFromUeh );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,        MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_SUCC,     bsmProcUeEventHistoryLogConfigSuccFromUeh );

  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_FAIL,     bsmProcUeEventHistoryLogConfigFailFromUeh );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,     MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_FAIL,     bsmProcUeEventHistoryLogConfigFailFromUeh );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,    MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_FAIL,     bsmProcUeEventHistoryLogConfigFailFromUeh );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,    MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_FAIL,     bsmProcUeEventHistoryLogConfigFailFromUeh );
  InitBsmStateMachine( BS_IDLE,                      MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_FAIL,     bsmProcUeEventHistoryLogConfigFailFromUeh );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,     MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_FAIL,     bsmProcUeEventHistoryLogConfigFailFromUeh );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,      MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_FAIL,     bsmProcUeEventHistoryLogConfigFailFromUeh );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,      MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_FAIL,     bsmProcUeEventHistoryLogConfigFailFromUeh );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,     MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_FAIL,     bsmProcUeEventHistoryLogConfigFailFromUeh );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,        MSG_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_FAIL,     bsmProcUeEventHistoryLogConfigFailFromUeh );

  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_SIH_BSM_INIT_CONFIG_SUCC,     bsmProcInitConfigSucc );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_SIH_BSM_INIT_CONFIG_FAIL,     bsmProcInitConfigFail );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_SM_BSM_S1AP_INIT_CONFIG_SUCC, bsmProcInitConfigSucc );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_SM_BSM_S1AP_INIT_CONFIG_FAIL, bsmProcInitConfigFail );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_SM_BSM_SCTP_INIT_CONFIG_SUCC, bsmProcInitConfigSucc );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_SM_BSM_SCTP_INIT_CONFIG_FAIL, bsmProcInitConfigFail );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_SM_BSM_EGTP_INIT_CONFIG_SUCC, bsmProcInitConfigSucc );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_SM_BSM_EGTP_INIT_CONFIG_FAIL, bsmProcInitConfigFail );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_DEH_BSM_INIT_CONFIG_SUCC,     bsmProcInitConfigSucc );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_DEH_BSM_INIT_CONFIG_FAIL,     bsmProcInitConfigFail );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_LTH_BSM_INIT_CONFIG_SUCC,     bsmProcInitConfigSucc );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_LTH_BSM_INIT_CONFIG_FAIL,     bsmProcInitConfigFail );

  /* Configuration Response from UEH, S1AP, SCTP */
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL, MSG_CEM_BSM_ENB_CONFIG_SUCC,           bsmProcEnbConfigSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL, MSG_CEM_BSM_ENB_CONFIG_FAIL,           bsmProcEnbConfigFail );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL, MSG_S1M_BSM_CONFIG_SUCC,               bsmProcEnbConfigSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL, MSG_S1M_BSM_CONFIG_FAIL,               bsmProcEnbConfigFail );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL, MSG_DEH_BSM_RH_CONFIG_SUCC,            bsmProcEnbConfigSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL, MSG_DEH_BSM_RH_CONFIG_FAIL,            bsmProcEnbConfigFail );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL, MSG_PMH_BSM_CONFIG_SUCC,               bsmProcEnbConfigSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL, MSG_PMH_BSM_CONFIG_FAIL,               bsmProcEnbConfigFail );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL, MSG_UDH_BSM_CONFIG_SUCC,               bsmProcEnbConfigSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL, MSG_UDH_BSM_CONFIG_FAIL,               bsmProcEnbConfigFail );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL, MSG_PMH_BSM_COUNTER_INPUT_CONFIG_SUCC, bsmProcEnbConfigSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL, MSG_PMH_BSM_COUNTER_INPUT_CONFIG_FAIL, bsmProcEnbConfigFail );

  /* Configuration Response from PMH */
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL, MSG_PMH_BSM_CONFIG_SUCC,               bsmProcPmhConfigSucc );
  InitBsmStateMachine( BS_IDLE,                   MSG_PMH_BSM_CONFIG_SUCC,               bsmProcPmhConfigSucc );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,   MSG_PMH_BSM_CONFIG_SUCC,               bsmProcPmhConfigSucc );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,  MSG_PMH_BSM_CONFIG_SUCC,               bsmProcPmhConfigSucc );

  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL, MSG_PMH_BSM_CONFIG_FAIL,               bsmProcPmhConfigFail );
  InitBsmStateMachine( BS_IDLE,                   MSG_PMH_BSM_CONFIG_FAIL,               bsmProcPmhConfigFail );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,   MSG_PMH_BSM_CONFIG_FAIL,               bsmProcPmhConfigFail );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,  MSG_PMH_BSM_CONFIG_FAIL,               bsmProcPmhConfigFail );

  /* Device init response from DEH */
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,  MSG_DEH_BSM_DEVICE_INIT_SUCC, bsmProcDeviceInitSucc );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,  MSG_DEH_BSM_DEVICE_INIT_FAIL, bsmProcDeviceInitFail );

  /* GPS details response from DEH */
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,  MSG_DEH_BSM_GPS_DETAIL_SUCC, bsmProcGpsDetailSucc );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,  MSG_DEH_BSM_GPS_DETAIL_SUCC, bsmProcGpsDetailSucc );
  InitBsmStateMachine( BS_IDLE,                    MSG_DEH_BSM_GPS_DETAIL_SUCC, bsmProcGpsDetailSucc );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,  MSG_DEH_BSM_GPS_DETAIL_SUCC, bsmProcGpsDetailSucc );

  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,  MSG_DEH_BSM_GPS_DETAIL_FAIL, bsmProcGpsDetailFail );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,  MSG_DEH_BSM_GPS_DETAIL_FAIL, bsmProcGpsDetailFail );
  InitBsmStateMachine( BS_IDLE,                    MSG_DEH_BSM_GPS_DETAIL_FAIL, bsmProcGpsDetailFail );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,  MSG_DEH_BSM_GPS_DETAIL_FAIL, bsmProcGpsDetailFail );

  /* Clear Counter SUCC Handling in all states */
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_PMH_BSM_COUNTER_CLEAR_SUCC,  bsmProcCounterClearSucc );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_PMH_BSM_COUNTER_CLEAR_SUCC,  bsmProcCounterClearSucc );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_PMH_BSM_COUNTER_CLEAR_SUCC,  bsmProcCounterClearSucc );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_PMH_BSM_COUNTER_CLEAR_SUCC,  bsmProcCounterClearSucc );
  InitBsmStateMachine( BS_IDLE,                     MSG_PMH_BSM_COUNTER_CLEAR_SUCC,  bsmProcCounterClearSucc );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_PMH_BSM_COUNTER_CLEAR_SUCC,  bsmProcCounterClearSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_PMH_BSM_COUNTER_CLEAR_SUCC,  bsmProcCounterClearSucc );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_PMH_BSM_COUNTER_CLEAR_SUCC,  bsmProcCounterClearSucc );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_PMH_BSM_COUNTER_CLEAR_SUCC,  bsmProcCounterClearSucc );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_PMH_BSM_COUNTER_CLEAR_SUCC,  bsmProcCounterClearSucc );

  /* Clear Counter Fail Handling in all states */
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_PMH_BSM_COUNTER_CLEAR_FAIL,  bsmProcCounterClearFail );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_PMH_BSM_COUNTER_CLEAR_FAIL,  bsmProcCounterClearFail );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_PMH_BSM_COUNTER_CLEAR_FAIL,  bsmProcCounterClearFail );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_PMH_BSM_COUNTER_CLEAR_FAIL,  bsmProcCounterClearFail );
  InitBsmStateMachine( BS_IDLE,                     MSG_PMH_BSM_COUNTER_CLEAR_FAIL,  bsmProcCounterClearFail );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_PMH_BSM_COUNTER_CLEAR_FAIL,  bsmProcCounterClearFail );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_PMH_BSM_COUNTER_CLEAR_FAIL,  bsmProcCounterClearFail );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_PMH_BSM_COUNTER_CLEAR_FAIL,  bsmProcCounterClearFail );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_PMH_BSM_COUNTER_CLEAR_FAIL,  bsmProcCounterClearFail );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_PMH_BSM_COUNTER_CLEAR_FAIL,  bsmProcCounterClearFail );

  /* DIAG Counter Cmd Response Handling in all states */
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_PMH_BSM_DIAG_COUNTER_CMD_CMPL,  bsmProcDiagCounterCmdCmpl );
  InitBsmStateMachine( BS_IDLE,                     MSG_PMH_BSM_DIAG_COUNTER_CMD_CMPL,  bsmProcDiagCounterCmdCmpl );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_PMH_BSM_DIAG_COUNTER_CMD_CMPL,  bsmProcDiagCounterCmdCmpl );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_PMH_BSM_DIAG_COUNTER_CMD_CMPL,  bsmProcDiagCounterCmdCmpl );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_PMH_BSM_DIAG_COUNTER_CMD_CMPL,  bsmProcDiagCounterCmdCmpl );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_PMH_BSM_DIAG_COUNTER_CMD_CMPL,  bsmProcDiagCounterCmdCmpl );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_PMH_BSM_DIAG_COUNTER_CMD_CMPL,  bsmProcDiagCounterCmdCmpl );

  /* Mod Up Handling in all states */
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_PRM_BSM_MOD_UP_IND,  bsmProcModUpInd );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_PRM_BSM_MOD_UP_IND,  bsmProcModUpInd );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_PRM_BSM_MOD_UP_IND,  bsmProcModUpInd );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_PRM_BSM_MOD_UP_IND,  bsmProcModUpInd );
  InitBsmStateMachine( BS_IDLE,                     MSG_PRM_BSM_MOD_UP_IND,  bsmProcModUpInd );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_PRM_BSM_MOD_UP_IND,  bsmProcModUpInd );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_PRM_BSM_MOD_UP_IND,  bsmProcModUpInd );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_PRM_BSM_MOD_UP_IND,  bsmProcModUpInd );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_PRM_BSM_MOD_UP_IND,  bsmProcModUpInd );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_PRM_BSM_MOD_UP_IND,  bsmProcModUpInd );

  /* Mod Down Handling in all states */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_PRM_BSM_MOD_DOWN_IND, bsmProcModDownIndInInitComplState );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_PRM_BSM_MOD_DOWN_IND, bsmProcModDownIndInInitStates );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_PRM_BSM_MOD_DOWN_IND, bsmProcModDownIndInInitStates );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_PRM_BSM_MOD_DOWN_IND, bsmProcModDownIndInInitStates );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_PRM_BSM_MOD_DOWN_IND, bsmProcModDownIndInInitStates );
  InitBsmStateMachine( BS_IDLE,                     MSG_PRM_BSM_MOD_DOWN_IND, bsmProcModDownInd );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_PRM_BSM_MOD_DOWN_IND, bsmProcModDownInd );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_PRM_BSM_MOD_DOWN_IND, bsmProcModDownInd );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_PRM_BSM_MOD_DOWN_IND, bsmProcModDownInd );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_PRM_BSM_MOD_DOWN_IND, bsmProcModDownInd );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_PRM_BSM_MOD_DOWN_IND, bsmProcModDownInd );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_PRM_BSM_MOD_DOWN_IND, bsmProcModDownInd );

  /* Handle Link Up indication from S1AP */
  InitBsmStateMachine( BS_IDLE,                  MSG_S1M_BSM_LINK_UP_IND, bsmProcS1apLinkUpInd );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,  MSG_S1M_BSM_LINK_UP_IND, bsmProcS1apLinkUpInd );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL, MSG_S1M_BSM_LINK_UP_IND, bsmProcS1apLinkUpInd );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,    MSG_S1M_BSM_LINK_UP_IND, bsmProcS1apLinkUpInd );

  /* Handle Link Down indication from S1AP */
  InitBsmStateMachine( BS_IDLE,                  MSG_S1M_BSM_LINK_DOWN_IND, bsmProcS1apLinkDownInd );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL, MSG_S1M_BSM_LINK_DOWN_IND, bsmProcS1apLinkDownInd );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,  MSG_S1M_BSM_LINK_DOWN_IND, bsmProcS1apLinkDownInd );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,  MSG_S1M_BSM_LINK_DOWN_IND, bsmProcS1apLinkDownInd );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL, MSG_S1M_BSM_LINK_DOWN_IND, bsmProcS1apLinkDownInd );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,    MSG_S1M_BSM_LINK_DOWN_IND, bsmProcS1apLinkDownInd );

  /* Handle Link Down indication from S1AP */
  InitBsmStateMachine( BS_IDLE,                  MSG_S1M_BSM_UNKNOWN_ENB_UE_ID, bsmProcUehUnknownEnbUeId );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL, MSG_S1M_BSM_UNKNOWN_ENB_UE_ID, bsmProcUehUnknownEnbUeId );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,  MSG_S1M_BSM_UNKNOWN_ENB_UE_ID, bsmProcUehUnknownEnbUeId );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,  MSG_S1M_BSM_UNKNOWN_ENB_UE_ID, bsmProcUehUnknownEnbUeId );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL, MSG_S1M_BSM_UNKNOWN_ENB_UE_ID, bsmProcUehUnknownEnbUeId );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,    MSG_S1M_BSM_UNKNOWN_ENB_UE_ID, bsmProcUehUnknownEnbUeId );

  /* S1AP reset Indication */
  InitBsmStateMachine( BS_IDLE,                  MSG_S1M_BSM_RESET_IND, bsmProcS1apResetInd );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,      MSG_S1M_BSM_RESET_IND, bsmProcS1apResetInd );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL, MSG_S1M_BSM_RESET_IND, bsmProcS1apResetInd );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,  MSG_S1M_BSM_RESET_IND, bsmProcS1apResetInd );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,  MSG_S1M_BSM_RESET_IND, bsmProcS1apResetInd );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL, MSG_S1M_BSM_RESET_IND, bsmProcS1apResetInd );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,    MSG_S1M_BSM_RESET_IND, bsmProcS1apResetInd );

  /* Handle Link Down indication from S1AP */
  InitBsmStateMachine( BS_IDLE,                  MSG_S1M_BSM_LINK_CLOSED_IND, bsmProcS1apLinkClosedInd );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL, MSG_S1M_BSM_LINK_CLOSED_IND, bsmProcS1apLinkClosedInd );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,  MSG_S1M_BSM_LINK_CLOSED_IND, bsmProcS1apLinkClosedInd );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,  MSG_S1M_BSM_LINK_CLOSED_IND, bsmProcS1apLinkClosedInd );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL, MSG_S1M_BSM_LINK_CLOSED_IND, bsmProcS1apLinkClosedInd );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,    MSG_S1M_BSM_LINK_CLOSED_IND, bsmProcS1apLinkClosedInd );

  /* Handle Device DOWN status received from DEH module */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,            MSG_DEH_BSM_COMPONENT_DOWN_IND, bsmProcComponentDownInd );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_DEH_BSM_COMPONENT_DOWN_IND, bsmProcComponentDownInd );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,     MSG_DEH_BSM_COMPONENT_DOWN_IND, bsmProcComponentDownInd );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,    MSG_DEH_BSM_COMPONENT_DOWN_IND, bsmProcComponentDownInd );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,    MSG_DEH_BSM_COMPONENT_DOWN_IND, bsmProcComponentDownInd );
  InitBsmStateMachine( BS_IDLE,                      MSG_DEH_BSM_COMPONENT_DOWN_IND, bsmProcComponentDownInd );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,     MSG_DEH_BSM_COMPONENT_DOWN_IND, bsmProcComponentDownInd );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,      MSG_DEH_BSM_COMPONENT_DOWN_IND, bsmProcComponentDownInd );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,      MSG_DEH_BSM_COMPONENT_DOWN_IND, bsmProcComponentDownInd );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,     MSG_DEH_BSM_COMPONENT_DOWN_IND, bsmProcComponentDownInd );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,        MSG_DEH_BSM_COMPONENT_DOWN_IND, bsmProcComponentDownInd );

  /* Handle Device UP status received from DEH module */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,            MSG_DEH_BSM_COMPONENT_UP_IND, bsmProcComponentUpInd );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL,  MSG_DEH_BSM_COMPONENT_UP_IND, bsmProcComponentUpInd );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,     MSG_DEH_BSM_COMPONENT_UP_IND, bsmProcComponentUpInd );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,    MSG_DEH_BSM_COMPONENT_UP_IND, bsmProcComponentUpInd );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,    MSG_DEH_BSM_COMPONENT_UP_IND, bsmProcComponentUpInd );
  InitBsmStateMachine( BS_IDLE,                      MSG_DEH_BSM_COMPONENT_UP_IND, bsmProcComponentUpInd );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,     MSG_DEH_BSM_COMPONENT_UP_IND, bsmProcComponentUpInd );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,      MSG_DEH_BSM_COMPONENT_UP_IND, bsmProcComponentUpInd );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,      MSG_DEH_BSM_COMPONENT_UP_IND, bsmProcComponentUpInd );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,     MSG_DEH_BSM_COMPONENT_UP_IND, bsmProcComponentUpInd );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,        MSG_DEH_BSM_COMPONENT_UP_IND, bsmProcComponentUpInd );

  /* Hello Message Handling in all states */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_PRM_BSM_HELLO, bsmProcHelloInAllStates );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_PRM_BSM_HELLO, bsmProcHelloInAllStates );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_PRM_BSM_HELLO, bsmProcHelloInAllStates );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_PRM_BSM_HELLO, bsmProcHelloInAllStates );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_PRM_BSM_HELLO, bsmProcHelloInAllStates );
  InitBsmStateMachine( BS_IDLE,                     MSG_PRM_BSM_HELLO, bsmProcHelloInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_PRM_BSM_HELLO, bsmProcHelloInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_PRM_BSM_HELLO, bsmProcHelloInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_PRM_BSM_HELLO, bsmProcHelloInAllStates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_PRM_BSM_HELLO, bsmProcHelloInAllStates );

  /* Handle Sw Activate Succ/Fail Message from PRM */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_PRM_BSM_SW_ACTIVATE_SUCC, bsmProcSwActivateSucc );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_PRM_BSM_SW_ACTIVATE_SUCC, bsmProcSwActivateSucc );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_PRM_BSM_SW_ACTIVATE_SUCC, bsmProcSwActivateSucc );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_PRM_BSM_SW_ACTIVATE_SUCC, bsmProcSwActivateSucc );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_PRM_BSM_SW_ACTIVATE_SUCC, bsmProcSwActivateSucc );
  InitBsmStateMachine( BS_IDLE,                     MSG_PRM_BSM_SW_ACTIVATE_SUCC, bsmProcSwActivateSucc );

  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_PRM_BSM_SW_ACTIVATE_FAIL, bsmProcSwActivateFail );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_PRM_BSM_SW_ACTIVATE_FAIL, bsmProcSwActivateFail );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_PRM_BSM_SW_ACTIVATE_FAIL, bsmProcSwActivateFail );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_PRM_BSM_SW_ACTIVATE_FAIL, bsmProcSwActivateFail );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_PRM_BSM_SW_ACTIVATE_FAIL, bsmProcSwActivateFail );
  InitBsmStateMachine( BS_IDLE,                     MSG_PRM_BSM_SW_ACTIVATE_FAIL, bsmProcSwActivateFail );

  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,   MSG_PRM_BSM_SW_ACTIVATE_RSP,  bsmProcSwActivateRsp );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,   MSG_PRM_BSM_SW_ACTIVATE_REJ,  bsmProcSwActivateRej );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,     MSG_PRM_BSM_REBOOT_RSP,       bsmProcEnbRebootRsp );

  /* SET REQ from CIH in all states */

  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_CIH1_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_CIH1_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_CIH1_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_CIH1_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_CIH1_BSM_SET_OBJ_REQ, bsmProcSetObjReq );
  InitBsmStateMachine( BS_IDLE,                     MSG_CIH1_BSM_SET_OBJ_REQ, bsmProcSetObjReq );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_CIH1_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_CIH1_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_CIH1_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_CIH1_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_CIH1_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_CIH1_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );

  /* GET REQ from CIH in all states */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_CIH1_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_CIH1_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_CIH1_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_CIH1_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_CIH1_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_IDLE,                     MSG_CIH1_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_CIH1_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_CIH1_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_CIH1_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_CIH1_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_CIH1_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_CIH1_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );

  /* SET REQ from CIH1 in all states */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_CIH2_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_CIH2_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_CIH2_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_CIH2_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_CIH2_BSM_SET_OBJ_REQ, bsmProcSetObjReq );
  InitBsmStateMachine( BS_IDLE,                     MSG_CIH2_BSM_SET_OBJ_REQ, bsmProcSetObjReq );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_CIH2_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_CIH2_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_CIH2_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_CIH2_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_CIH2_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_CIH2_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );

  /* GET REQ from CIH1 in all states */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_CIH2_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_CIH2_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_CIH2_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_CIH2_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_CIH2_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_IDLE,                     MSG_CIH2_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_CIH2_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_CIH2_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_CIH2_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_CIH2_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_CIH2_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_CIH2_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );

  /* SET REQ from CIH2 in all states */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_CIH3_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_CIH3_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_CIH3_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_CIH3_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_CIH3_BSM_SET_OBJ_REQ, bsmProcSetObjReq );
  InitBsmStateMachine( BS_IDLE,                     MSG_CIH3_BSM_SET_OBJ_REQ, bsmProcSetObjReq );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_CIH3_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_CIH3_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_CIH3_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_CIH3_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_CIH3_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_CIH3_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );

  /* GET REQ from CIH2 in all states */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_CIH3_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_CIH3_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_CIH3_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_CIH3_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_CIH3_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_IDLE,                     MSG_CIH3_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_CIH3_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_CIH3_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_CIH3_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_CIH3_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_CIH3_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_CIH3_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );

  /* SET REQ from WIH in all states */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_WIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_WIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_WIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_WIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_WIH_BSM_SET_OBJ_REQ, bsmProcSetObjReq );
  InitBsmStateMachine( BS_IDLE,                     MSG_WIH_BSM_SET_OBJ_REQ, bsmProcSetObjReq );
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_WIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_WIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_WIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_WIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_WIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_WIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_WIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );

  /* GET REQ from WIH in all states */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_WIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_WIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_WIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_WIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_WIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_IDLE,                     MSG_WIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_WIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_WIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_WIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_WIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_WIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_WIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );

  /* SET REQ from SIH in all states */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_SIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_SIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_SIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_SIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_SIH_BSM_SET_OBJ_REQ, bsmProcSetObjReq );
  InitBsmStateMachine( BS_IDLE,                     MSG_SIH_BSM_SET_OBJ_REQ, bsmProcSetObjReq );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_SIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_SIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_SIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_SIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_SIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_SIH_BSM_SET_OBJ_REQ, bsmProcSetObjReqWhenBusy );

  /* GET REQ from SIH in all states */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_SIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_SIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_SIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_SIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_SIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_IDLE,                     MSG_SIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_SIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_SIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_SIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_SIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_SIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_SIH_BSM_GET_OBJ_REQ, bsmProcGetObjReqInAllStates );

  /* GET Response from FTH */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_FTH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_FTH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_FTH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_FTH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_FTH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_IDLE,                     MSG_FTH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_FTH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_FTH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_FTH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_FTH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_FTH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_FTH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );

  /* Get Response from PRM */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_PRM_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_PRM_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_PRM_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_PRM_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_PRM_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_IDLE,                     MSG_PRM_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_PRM_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_PRM_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_PRM_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_PRM_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_PRM_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_PRM_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );

  /* Get Response from PMH */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_PMH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_PMH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_PMH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_PMH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_PMH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_IDLE,                     MSG_PMH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_PMH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_PMH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_PMH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_PMH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_PMH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_PMH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );

  /* Get Response for Active UE status REQ message from UEH */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_UEH_BSM_GET_ACTIVE_UE_STATUS_RSP, bsmProcUehActiveUeStatusRsp );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_UEH_BSM_GET_ACTIVE_UE_STATUS_RSP, bsmProcUehActiveUeStatusRsp );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_UEH_BSM_GET_ACTIVE_UE_STATUS_RSP, bsmProcUehActiveUeStatusRsp );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_UEH_BSM_GET_ACTIVE_UE_STATUS_RSP, bsmProcUehActiveUeStatusRsp );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_UEH_BSM_GET_ACTIVE_UE_STATUS_RSP, bsmProcUehActiveUeStatusRsp );
  InitBsmStateMachine( BS_IDLE,                     MSG_UEH_BSM_GET_ACTIVE_UE_STATUS_RSP, bsmProcUehActiveUeStatusRsp );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_UEH_BSM_GET_ACTIVE_UE_STATUS_RSP, bsmProcUehActiveUeStatusRsp );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_UEH_BSM_GET_ACTIVE_UE_STATUS_RSP, bsmProcUehActiveUeStatusRsp );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_UEH_BSM_GET_ACTIVE_UE_STATUS_RSP, bsmProcUehActiveUeStatusRsp );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_UEH_BSM_GET_ACTIVE_UE_STATUS_RSP, bsmProcUehActiveUeStatusRsp );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_UEH_BSM_GET_ACTIVE_UE_STATUS_RSP, bsmProcUehActiveUeStatusRsp );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_UEH_BSM_GET_ACTIVE_UE_STATUS_RSP, bsmProcUehActiveUeStatusRsp );

  /* Get Response for Active UE status REQ message from UEH */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_UDH_BSM_GET_ACTIVE_RAB_STATUS_RSP, bsmProcUdhActiveRabStatusRsp );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_UDH_BSM_GET_ACTIVE_RAB_STATUS_RSP, bsmProcUdhActiveRabStatusRsp );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_UDH_BSM_GET_ACTIVE_RAB_STATUS_RSP, bsmProcUdhActiveRabStatusRsp );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_UDH_BSM_GET_ACTIVE_RAB_STATUS_RSP, bsmProcUdhActiveRabStatusRsp );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_UDH_BSM_GET_ACTIVE_RAB_STATUS_RSP, bsmProcUdhActiveRabStatusRsp );
  InitBsmStateMachine( BS_IDLE,                     MSG_UDH_BSM_GET_ACTIVE_RAB_STATUS_RSP, bsmProcUdhActiveRabStatusRsp );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_UDH_BSM_GET_ACTIVE_RAB_STATUS_RSP, bsmProcUdhActiveRabStatusRsp );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_UDH_BSM_GET_ACTIVE_RAB_STATUS_RSP, bsmProcUdhActiveRabStatusRsp );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_UDH_BSM_GET_ACTIVE_RAB_STATUS_RSP, bsmProcUdhActiveRabStatusRsp );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_UDH_BSM_GET_ACTIVE_RAB_STATUS_RSP, bsmProcUdhActiveRabStatusRsp );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_UDH_BSM_GET_ACTIVE_RAB_STATUS_RSP, bsmProcUdhActiveRabStatusRsp );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_UDH_BSM_GET_ACTIVE_RAB_STATUS_RSP, bsmProcUdhActiveRabStatusRsp );

  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_UEH_BSM_GET_UE_EVENT_HISTORY_RSP, bsmProcUehUeEventHistoryRsp );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_UEH_BSM_GET_UE_EVENT_HISTORY_RSP, bsmProcUehUeEventHistoryRsp );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_UEH_BSM_GET_UE_EVENT_HISTORY_RSP, bsmProcUehUeEventHistoryRsp );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_UEH_BSM_GET_UE_EVENT_HISTORY_RSP, bsmProcUehUeEventHistoryRsp );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_UEH_BSM_GET_UE_EVENT_HISTORY_RSP, bsmProcUehUeEventHistoryRsp );
  InitBsmStateMachine( BS_IDLE,                     MSG_UEH_BSM_GET_UE_EVENT_HISTORY_RSP, bsmProcUehUeEventHistoryRsp );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_UEH_BSM_GET_UE_EVENT_HISTORY_RSP, bsmProcUehUeEventHistoryRsp );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_UEH_BSM_GET_UE_EVENT_HISTORY_RSP, bsmProcUehUeEventHistoryRsp );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_UEH_BSM_GET_UE_EVENT_HISTORY_RSP, bsmProcUehUeEventHistoryRsp );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_UEH_BSM_GET_UE_EVENT_HISTORY_RSP, bsmProcUehUeEventHistoryRsp );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_UEH_BSM_GET_UE_EVENT_HISTORY_RSP, bsmProcUehUeEventHistoryRsp );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_UEH_BSM_GET_UE_EVENT_HISTORY_RSP, bsmProcUehUeEventHistoryRsp );

  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_UDH_BSM_GET_RAB_HISTORY_RSP, bsmProcUdhRabHistoryRsp );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_UDH_BSM_GET_RAB_HISTORY_RSP, bsmProcUdhRabHistoryRsp );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_UDH_BSM_GET_RAB_HISTORY_RSP, bsmProcUdhRabHistoryRsp );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_UDH_BSM_GET_RAB_HISTORY_RSP, bsmProcUdhRabHistoryRsp );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_UDH_BSM_GET_RAB_HISTORY_RSP, bsmProcUdhRabHistoryRsp );
  InitBsmStateMachine( BS_IDLE,                     MSG_UDH_BSM_GET_RAB_HISTORY_RSP, bsmProcUdhRabHistoryRsp );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_UDH_BSM_GET_RAB_HISTORY_RSP, bsmProcUdhRabHistoryRsp );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_UDH_BSM_GET_RAB_HISTORY_RSP, bsmProcUdhRabHistoryRsp );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_UDH_BSM_GET_RAB_HISTORY_RSP, bsmProcUdhRabHistoryRsp );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_UDH_BSM_GET_RAB_HISTORY_RSP, bsmProcUdhRabHistoryRsp );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_UDH_BSM_GET_RAB_HISTORY_RSP, bsmProcUdhRabHistoryRsp );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_UDH_BSM_GET_RAB_HISTORY_RSP, bsmProcUdhRabHistoryRsp );

  /*Get Response for MOD_STATUS_REQ message from UEH */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_UEH_BSM_MOD_STATE_RSP, bsmProcUehModStateRsp );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_UEH_BSM_MOD_STATE_RSP, bsmProcUehModStateRsp );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_UEH_BSM_MOD_STATE_RSP, bsmProcUehModStateRsp );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_UEH_BSM_MOD_STATE_RSP, bsmProcUehModStateRsp );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_UEH_BSM_MOD_STATE_RSP, bsmProcUehModStateRsp );
  InitBsmStateMachine( BS_IDLE,                     MSG_UEH_BSM_MOD_STATE_RSP, bsmProcUehModStateRsp );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_UEH_BSM_MOD_STATE_RSP, bsmProcUehModStateRsp );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_UEH_BSM_MOD_STATE_RSP, bsmProcUehModStateRsp );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_UEH_BSM_MOD_STATE_RSP, bsmProcUehModStateRsp );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_UEH_BSM_MOD_STATE_RSP, bsmProcUehModStateRsp );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_UEH_BSM_MOD_STATE_RSP, bsmProcUehModStateRsp );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_UEH_BSM_MOD_STATE_RSP, bsmProcUehModStateRsp );

  /*Get Response for MOD_STATUS_REQ message from UDH */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_UDH_BSM_MOD_STATE_RSP, bsmProcUdhModStateRsp );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_UDH_BSM_MOD_STATE_RSP, bsmProcUdhModStateRsp );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_UDH_BSM_MOD_STATE_RSP, bsmProcUdhModStateRsp );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_UDH_BSM_MOD_STATE_RSP, bsmProcUdhModStateRsp );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_UDH_BSM_MOD_STATE_RSP, bsmProcUdhModStateRsp );
  InitBsmStateMachine( BS_IDLE,                     MSG_UDH_BSM_MOD_STATE_RSP, bsmProcUdhModStateRsp );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_UDH_BSM_MOD_STATE_RSP, bsmProcUdhModStateRsp );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_UDH_BSM_MOD_STATE_RSP, bsmProcUdhModStateRsp );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_UDH_BSM_MOD_STATE_RSP, bsmProcUdhModStateRsp );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_UDH_BSM_MOD_STATE_RSP, bsmProcUdhModStateRsp );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_UDH_BSM_MOD_STATE_RSP, bsmProcUdhModStateRsp );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_UDH_BSM_MOD_STATE_RSP, bsmProcUdhModStateRsp );

  /* Get Response from DEH for ENB ADDRESSES */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_DEH_BSM_GET_ENB_ADDR_RSP, bsmProcGetEnbAddrRsp );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_DEH_BSM_GET_ENB_ADDR_RSP, bsmProcGetEnbAddrRsp );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_DEH_BSM_GET_ENB_ADDR_RSP, bsmProcGetEnbAddrRsp );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_DEH_BSM_GET_ENB_ADDR_RSP, bsmProcGetEnbAddrRsp );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_DEH_BSM_GET_ENB_ADDR_RSP, bsmProcGetEnbAddrRsp );
  InitBsmStateMachine( BS_IDLE,                     MSG_DEH_BSM_GET_ENB_ADDR_RSP, bsmProcGetEnbAddrRsp );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_DEH_BSM_GET_ENB_ADDR_RSP, bsmProcGetEnbAddrRsp );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_DEH_BSM_GET_ENB_ADDR_RSP, bsmProcGetEnbAddrRsp );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_DEH_BSM_GET_ENB_ADDR_RSP, bsmProcGetEnbAddrRsp );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_DEH_BSM_GET_ENB_ADDR_RSP, bsmProcGetEnbAddrRsp );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_DEH_BSM_GET_ENB_ADDR_RSP, bsmProcGetEnbAddrRsp );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_DEH_BSM_GET_ENB_ADDR_RSP, bsmProcGetEnbAddrRsp );

  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_DEH_BSM_GET_ENB_ADDR_FAIL, bsmProcGetEnbAddrFail );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_DEH_BSM_GET_ENB_ADDR_FAIL, bsmProcGetEnbAddrFail );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_DEH_BSM_GET_ENB_ADDR_FAIL, bsmProcGetEnbAddrFail );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_DEH_BSM_GET_ENB_ADDR_FAIL, bsmProcGetEnbAddrFail );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_DEH_BSM_GET_ENB_ADDR_FAIL, bsmProcGetEnbAddrFail );
  InitBsmStateMachine( BS_IDLE,                     MSG_DEH_BSM_GET_ENB_ADDR_FAIL, bsmProcGetEnbAddrFail );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_DEH_BSM_GET_ENB_ADDR_FAIL, bsmProcGetEnbAddrFail );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_DEH_BSM_GET_ENB_ADDR_FAIL, bsmProcGetEnbAddrFail );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_DEH_BSM_GET_ENB_ADDR_FAIL, bsmProcGetEnbAddrFail );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_DEH_BSM_GET_ENB_ADDR_FAIL, bsmProcGetEnbAddrFail );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_DEH_BSM_GET_ENB_ADDR_FAIL, bsmProcGetEnbAddrFail );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_DEH_BSM_GET_ENB_ADDR_FAIL, bsmProcGetEnbAddrFail );
  /* Get Response from DEH */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_DEH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_DEH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_DEH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_DEH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_DEH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_IDLE,                     MSG_DEH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_DEH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_DEH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_DEH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_DEH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_DEH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_DEH_BSM_GET_OBJ_SUCC, bsmProcGetRspSucc );

  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_DEH_BSM_GET_OBJ_FAIL, bsmProcGetRspFail );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_DEH_BSM_GET_OBJ_FAIL, bsmProcGetRspFail );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_DEH_BSM_GET_OBJ_FAIL, bsmProcGetRspFail );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_DEH_BSM_GET_OBJ_FAIL, bsmProcGetRspFail );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_DEH_BSM_GET_OBJ_FAIL, bsmProcGetRspFail );
  InitBsmStateMachine( BS_IDLE,                     MSG_DEH_BSM_GET_OBJ_FAIL, bsmProcGetRspFail );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_DEH_BSM_GET_OBJ_FAIL, bsmProcGetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_DEH_BSM_GET_OBJ_FAIL, bsmProcGetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_DEH_BSM_GET_OBJ_FAIL, bsmProcGetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_DEH_BSM_GET_OBJ_FAIL, bsmProcGetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_DEH_BSM_GET_OBJ_FAIL, bsmProcGetRspFail );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_DEH_BSM_GET_OBJ_FAIL, bsmProcGetRspFail );

  /* Set Response from DEH */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_DEH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_DEH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_DEH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_DEH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_DEH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( BS_IDLE,                     MSG_DEH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_DEH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_DEH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_DEH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_DEH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_DEH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_DEH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );

  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_DEH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_DEH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_DEH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_DEH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_DEH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( BS_IDLE,                     MSG_DEH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_DEH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_DEH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_DEH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_DEH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_DEH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_DEH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );

  /* Exit Message handling in all states */
  InitBsmStateMachine( BS_WAIT_INIT_CMPL,           MSG_PRM_BSM_EXIT_CMD, bsmProcExitReqInAllStates );
  InitBsmStateMachine( BS_WAIT_INITIAL_CONFIG_CMPL, MSG_PRM_BSM_EXIT_CMD, bsmProcExitReqInAllStates );
  InitBsmStateMachine( BS_WAIT_DEVICE_INIT_CMPL,    MSG_PRM_BSM_EXIT_CMD, bsmProcExitReqInAllStates );
  InitBsmStateMachine( BS_WAIT_COLLECT_INFO_CMPL,   MSG_PRM_BSM_EXIT_CMD, bsmProcExitReqInAllStates );
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_PRM_BSM_EXIT_CMD, bsmProcExitReqInAllStates );
  InitBsmStateMachine( BS_IDLE,                     MSG_PRM_BSM_EXIT_CMD, bsmProcExitReqInAllStates );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_PRM_BSM_EXIT_CMD, bsmProcExitReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_PRM_BSM_EXIT_CMD, bsmProcExitReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_PRM_BSM_EXIT_CMD, bsmProcExitReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_PRM_BSM_EXIT_CMD, bsmProcExitReqInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_PRM_BSM_EXIT_CMD, bsmProcExitReqInAllStates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_PRM_BSM_EXIT_CMD, bsmProcExitReqInAllStates );

  /* OPSTART Success and failure messages from CCPU modules */
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL, MSG_DEH_BSM_OPSTART_SUCC, bsmProcDehOpStartSuccInWaitEnableState );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL, MSG_DEH_BSM_OPSTART_FAIL, bsmProcDehOpStartFailInWaitEnableState );

  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL, MSG_S1M_BSM_OPSTART_SUCC, bsmProcS1apOpStartSuccInWaitEnableState );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL, MSG_S1M_BSM_OPSTART_FAIL, bsmProcS1apOpStartFailInWaitEnableState );

  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL, MSG_CEM_BSM_OPSTART_SUCC, bsmProcUehOpStartSuccInWaitEnableState );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL, MSG_CEM_BSM_OPSTART_FAIL, bsmProcUehOpStartFailInWaitEnableState );

  /* OPSTART REQ from UDH Module */
   InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL, MSG_UDH_BSM_OPSTART_SUCC, bsmProcUdhOpStartSuccInWaitEnableState );
   InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL, MSG_UDH_BSM_OPSTART_FAIL, bsmProcUdhOpStartFailInWaitEnableState );

  /* OPSTART REQ from PMH Module */
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL, MSG_PMH_BSM_OPSTART_SUCC, bsmProcPmhOpStartSuccInWaitEnableState );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL, MSG_PMH_BSM_OPSTART_FAIL, bsmProcPmhOpStartFailInWaitEnableState );

  /* OPSTOP Success messages from CCPU modules */
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL, MSG_PMH_BSM_OPSTOP_SUCC,  bsmProcOpStopSuccInWaitDisableState );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL, MSG_DEH_BSM_OPSTOP_SUCC,  bsmProcOpStopSuccInWaitDisableState );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL, MSG_CEM_BSM_OPSTOP_SUCC,  bsmProcOpStopSuccInWaitDisableState );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL, MSG_S1M_BSM_OPSTOP_SUCC,  bsmProcOpStopSuccInWaitDisableState );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL, MSG_UDH_BSM_OPSTOP_SUCC,  bsmProcOpStopSuccInWaitDisableState );

  /* Message from PMH Module */
  InitBsmStateMachine( BS_IDLE,                     MSG_PMH_BSM_COUNTER_FILE_AVAILABLE, bsmProcPmhCounterFileAvailable );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_PMH_BSM_COUNTER_FILE_AVAILABLE, bsmProcPmhCounterFileAvailable );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_PMH_BSM_COUNTER_FILE_AVAILABLE, bsmProcPmhCounterFileAvailable );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_PMH_BSM_COUNTER_FILE_AVAILABLE, bsmProcPmhCounterFileAvailable );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_PMH_BSM_COUNTER_FILE_AVAILABLE, bsmProcPmhCounterFileAvailable );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_PMH_BSM_COUNTER_FILE_AVAILABLE, bsmProcPmhCounterFileAvailable );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_PMH_BSM_COUNTER_FILE_AVAILABLE, bsmProcPmhCounterFileAvailable );

  InitBsmStateMachine( BS_IDLE,                     MSG_PMH_BSM_COUNTER_COLLECTION_FAILED, bsmProcPmhCounterCollectionFailed );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_PMH_BSM_COUNTER_COLLECTION_FAILED, bsmProcPmhCounterCollectionFailed );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_PMH_BSM_COUNTER_COLLECTION_FAILED, bsmProcPmhCounterCollectionFailed );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_PMH_BSM_COUNTER_COLLECTION_FAILED, bsmProcPmhCounterCollectionFailed );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_PMH_BSM_COUNTER_COLLECTION_FAILED, bsmProcPmhCounterCollectionFailed );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_PMH_BSM_COUNTER_COLLECTION_FAILED, bsmProcPmhCounterCollectionFailed );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_PMH_BSM_COUNTER_COLLECTION_FAILED, bsmProcPmhCounterCollectionFailed );

  /* Start Trace Success/Failure/Complete Message handling */
  InitBsmStateMachine( BS_IDLE,                  MSG_LTH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( BS_IDLE,                  MSG_LTH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( BS_IDLE,                  MSG_LTH_BSM_GET_OBJ_SUCC,   bsmProcGetRspSucc );


  /* SET Response from FTH */
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_FTH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( BS_IDLE,                     MSG_FTH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_FTH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_FTH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_FTH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_FTH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_FTH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_FTH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );

  /* SET Response from FTH */
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_FTH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( BS_IDLE,                     MSG_FTH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( BS_WAIT_REBOOT_CMPL,         MSG_FTH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_FTH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_FTH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_FTH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_FTH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_FTH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );

  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL, MSG_FTH_BSM_DOWNLOAD_CMPL_IND,  bsmProcSwDownloadCmplInAllstates );
  InitBsmStateMachine( BS_IDLE,                   MSG_FTH_BSM_DOWNLOAD_CMPL_IND,  bsmProcSwDownloadCmplInAllstates );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,  MSG_FTH_BSM_DOWNLOAD_CMPL_IND,  bsmProcSwDownloadCmplInAllstates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,   MSG_FTH_BSM_DOWNLOAD_CMPL_IND,  bsmProcSwDownloadCmplInAllstates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,   MSG_FTH_BSM_DOWNLOAD_CMPL_IND,  bsmProcSwDownloadCmplInAllstates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,  MSG_FTH_BSM_DOWNLOAD_CMPL_IND,  bsmProcSwDownloadCmplInAllstates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,     MSG_FTH_BSM_DOWNLOAD_CMPL_IND,  bsmProcSwDownloadCmplInAllstates );

  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL, MSG_FTH_BSM_DOWNLOAD_FAIL, bsmProcSwDownloadFailInAllStates );
  InitBsmStateMachine( BS_IDLE,                   MSG_FTH_BSM_DOWNLOAD_FAIL, bsmProcSwDownloadFailInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,  MSG_FTH_BSM_DOWNLOAD_FAIL, bsmProcSwDownloadFailInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,   MSG_FTH_BSM_DOWNLOAD_FAIL, bsmProcSwDownloadFailInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,   MSG_FTH_BSM_DOWNLOAD_FAIL, bsmProcSwDownloadFailInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,  MSG_FTH_BSM_DOWNLOAD_FAIL, bsmProcSwDownloadFailInAllStates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,     MSG_FTH_BSM_DOWNLOAD_FAIL, bsmProcSwDownloadFailInAllStates );

  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL, MSG_FTH_BSM_UPLOAD_CMPL_IND,  bsmProcSwUploadCmplInAllstates );
  InitBsmStateMachine( BS_IDLE,                   MSG_FTH_BSM_UPLOAD_CMPL_IND,  bsmProcSwUploadCmplInAllstates );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,  MSG_FTH_BSM_UPLOAD_CMPL_IND,  bsmProcSwUploadCmplInAllstates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,   MSG_FTH_BSM_UPLOAD_CMPL_IND,  bsmProcSwUploadCmplInAllstates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,   MSG_FTH_BSM_UPLOAD_CMPL_IND,  bsmProcSwUploadCmplInAllstates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,  MSG_FTH_BSM_UPLOAD_CMPL_IND,  bsmProcSwUploadCmplInAllstates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,     MSG_FTH_BSM_UPLOAD_CMPL_IND,  bsmProcSwUploadCmplInAllstates );

  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL, MSG_FTH_BSM_UPLOAD_FAIL, bsmProcSwUploadFailInAllStates );
  InitBsmStateMachine( BS_IDLE,                   MSG_FTH_BSM_UPLOAD_FAIL, bsmProcSwUploadFailInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,  MSG_FTH_BSM_UPLOAD_FAIL, bsmProcSwUploadFailInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,   MSG_FTH_BSM_UPLOAD_FAIL, bsmProcSwUploadFailInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,   MSG_FTH_BSM_UPLOAD_FAIL, bsmProcSwUploadFailInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,  MSG_FTH_BSM_UPLOAD_FAIL, bsmProcSwUploadFailInAllStates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,     MSG_FTH_BSM_UPLOAD_FAIL, bsmProcSwUploadFailInAllStates );

  /* SW INSTALL Procedure */
  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL, MSG_PRM_BSM_SW_INSTALL_ACCEPTED, bsmProcSetRspSucc );
  InitBsmStateMachine( BS_IDLE,                   MSG_PRM_BSM_SW_INSTALL_ACCEPTED, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,  MSG_PRM_BSM_SW_INSTALL_ACCEPTED, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,   MSG_PRM_BSM_SW_INSTALL_ACCEPTED, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,   MSG_PRM_BSM_SW_INSTALL_ACCEPTED, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,  MSG_PRM_BSM_SW_INSTALL_ACCEPTED, bsmProcSetRspSucc );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,     MSG_PRM_BSM_SW_INSTALL_ACCEPTED, bsmProcSetRspSucc );

  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL, MSG_PRM_BSM_SW_INSTALL_REJ, bsmProcSetRspFail );
  InitBsmStateMachine( BS_IDLE,                   MSG_PRM_BSM_SW_INSTALL_REJ, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,  MSG_PRM_BSM_SW_INSTALL_REJ, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,   MSG_PRM_BSM_SW_INSTALL_REJ, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,   MSG_PRM_BSM_SW_INSTALL_REJ, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,  MSG_PRM_BSM_SW_INSTALL_REJ, bsmProcSetRspFail );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,     MSG_PRM_BSM_SW_INSTALL_REJ, bsmProcSetRspFail );

  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,    MSG_PRM_BSM_SW_INSTALL_SUCC,  bsmProcSwInstallSuccInAllStates );
  InitBsmStateMachine( BS_IDLE,                      MSG_PRM_BSM_SW_INSTALL_SUCC,  bsmProcSwInstallSuccInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,     MSG_PRM_BSM_SW_INSTALL_SUCC,  bsmProcSwInstallSuccInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,      MSG_PRM_BSM_SW_INSTALL_SUCC,  bsmProcSwInstallSuccInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,      MSG_PRM_BSM_SW_INSTALL_SUCC,  bsmProcSwInstallSuccInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,     MSG_PRM_BSM_SW_INSTALL_SUCC,  bsmProcSwInstallSuccInAllStates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,        MSG_PRM_BSM_SW_INSTALL_SUCC,  bsmProcSwInstallSuccInAllStates );

  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_PRM_BSM_SW_INSTALL_FAIL,  bsmProcSwInstallFailInAllStates );
  InitBsmStateMachine( BS_IDLE,                     MSG_PRM_BSM_SW_INSTALL_FAIL,  bsmProcSwInstallFailInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_PRM_BSM_SW_INSTALL_FAIL,  bsmProcSwInstallFailInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_PRM_BSM_SW_INSTALL_FAIL,  bsmProcSwInstallFailInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_PRM_BSM_SW_INSTALL_FAIL,  bsmProcSwInstallFailInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_PRM_BSM_SW_INSTALL_FAIL,  bsmProcSwInstallFailInAllStates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_PRM_BSM_SW_INSTALL_FAIL,  bsmProcSwInstallFailInAllStates );

  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL, MSG_PRM_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( BS_IDLE,                   MSG_PRM_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL , MSG_PRM_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,   MSG_PRM_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,   MSG_PRM_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,  MSG_PRM_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,     MSG_PRM_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );

  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL, MSG_PRM_BSM_CONFIG_INSTALL_REQ, bsmProcConfigInstallReq );
  InitBsmStateMachine( BS_IDLE,                   MSG_PRM_BSM_CONFIG_INSTALL_REQ, bsmProcConfigInstallReq );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,  MSG_PRM_BSM_CONFIG_INSTALL_REQ, bsmProcConfigInstallReq );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,  MSG_PRM_BSM_CONFIG_INSTALL_REQ, bsmProcConfigInstallReq );

  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_CEM_BSM_CELL_BAR_SUCC,  bsmProcCellBarSuccInAllStates );
  InitBsmStateMachine( BS_IDLE,                     MSG_CEM_BSM_CELL_BAR_SUCC,  bsmProcCellBarSuccInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_CEM_BSM_CELL_BAR_SUCC,  bsmProcCellBarSuccInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_CEM_BSM_CELL_BAR_SUCC,  bsmProcCellBarSuccInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_CEM_BSM_CELL_BAR_SUCC,  bsmProcCellBarSuccInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_CEM_BSM_CELL_BAR_SUCC,  bsmProcCellBarSuccInAllStates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_CEM_BSM_CELL_BAR_SUCC,  bsmProcCellBarSuccInAllStates );

  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL,   MSG_CEM_BSM_CELL_UNBAR_SUCC,  bsmProcCellUnBarSuccInAllStates );
  InitBsmStateMachine( BS_IDLE,                     MSG_CEM_BSM_CELL_UNBAR_SUCC,  bsmProcCellUnBarSuccInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,    MSG_CEM_BSM_CELL_UNBAR_SUCC,  bsmProcCellUnBarSuccInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,     MSG_CEM_BSM_CELL_UNBAR_SUCC,  bsmProcCellUnBarSuccInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,     MSG_CEM_BSM_CELL_UNBAR_SUCC,  bsmProcCellUnBarSuccInAllStates );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,    MSG_CEM_BSM_CELL_UNBAR_SUCC,  bsmProcCellUnBarSuccInAllStates );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,       MSG_CEM_BSM_CELL_UNBAR_SUCC,  bsmProcCellUnBarSuccInAllStates );

  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL, MSG_PMH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( BS_IDLE,                   MSG_PMH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,  MSG_PMH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,   MSG_PMH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,   MSG_PMH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,  MSG_PMH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,     MSG_PMH_BSM_SET_OBJ_SUCC, bsmProcSetRspSucc );

  InitBsmStateMachine( BS_WAIT_REGISTRATION_CMPL, MSG_PMH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( BS_IDLE,                   MSG_PMH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,  MSG_PMH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,   MSG_PMH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,   MSG_PMH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,  MSG_PMH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,     MSG_PMH_BSM_SET_OBJ_FAIL, bsmProcSetRspFail );

  InitBsmStateMachine( BS_IDLE,                   MSG_PMH_BSM_MEASUREMENT_CMPL_IND, bsmProcMeasCmplInd );
  InitBsmStateMachine( ENODEB_WAIT_RESTART_CMPL,  MSG_PMH_BSM_MEASUREMENT_CMPL_IND, bsmProcMeasCmplInd );
  InitBsmStateMachine( ENODEB_WAIT_CONFIG_CMPL,   MSG_PMH_BSM_MEASUREMENT_CMPL_IND, bsmProcMeasCmplInd );
  InitBsmStateMachine( ENODEB_WAIT_ENABLE_CMPL,   MSG_PMH_BSM_MEASUREMENT_CMPL_IND, bsmProcMeasCmplInd );
  InitBsmStateMachine( ENODEB_WAIT_DISABLE_CMPL,  MSG_PMH_BSM_MEASUREMENT_CMPL_IND, bsmProcMeasCmplInd );
  InitBsmStateMachine( WAIT_SW_ACTIVATE_CMPL,     MSG_PMH_BSM_MEASUREMENT_CMPL_IND, bsmProcMeasCmplInd );

}

/**
 * @details This is the entry point for all messages coming into BSM
 */
void bsmMain( FR Msg *msgIn )
{
  BsmContext *bsmContext = getBsmContext();

  logInfo("Received Message=[ %s ] in state=[ %d ] stateName=[ %s ]", getMsgName(msgIn->msgId), bsmContext->bsmProceduralState, getBsmProceduralStateName(bsmContext->bsmProceduralState) );

  if( NULL NOTEQ bsmStateMachine[bsmContext->bsmProceduralState][msgIn->msgId].fp )
  {
    bsmContext->bsmProceduralState = bsmStateMachine[bsmContext->bsmProceduralState][msgIn->msgId].fp( msgIn, bsmContext );
  }
  else
  {
    logError("No Processing function defined for this Message=%s, in this state=%s", getMsgName(msgIn->msgId), getBsmProceduralStateName(bsmContext->bsmProceduralState) );
  }
  /* Free the received Message */
  freeMsgIn(msgIn);
  printAndEmptyErrorChain(&(getModuleContext()->errChain));
}

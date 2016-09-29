 /**
 * @file    deh_main.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief   This file will handle all the messages coming from different modules in any DEH state.
 *
 * @author Madhusudana Rao Kata, Shweta Polepally, Krupakar Reddy, Ankur Agrawal,satish reddy
 * @date   25-05-2011
 * @details  1. Added Hello Message handling --Shweta ( 08th July )@n
 *           2. Added errChain parameter to all functions.--Shweta ( 04 Aug 2010 )@n
 *           3. Added RH Functionality(Configure,HwInfo,SwInfo,AlarmInfo)  --Madhu@n
 *           4. Removed Warnings, added comments          --Madhu (08 Sep 2010)@n
 *           5. Added Sw Exit                             --Madhu (18 Sep 2010)@n
 *           6. Added RH Message handler                  --Madhu (28 Oct 2010)@n
 *           7. BB (GPS/LED) APIs Added                   --Madhu (31-01-2011)@n
 *           7. LK Messages updated to latest C&M Specification --Krupakar (26-04-2011)@n
 *           8. Added support for LED color setting       -- Aagrawal ( 19-05-20111)@n
 *
 */

#include "deh.h"
#include <time.h>

/* Set the state machine */
#define InitDehStateMachine( presentState, msgIn, procFunc, newState0, newState1 ) \
            dehStateMachine[ presentState ][msgIn].fp = procFunc; \
            dehStateMachine[ presentState ][msgIn].nextStates[0] = newState0; \
            dehStateMachine[ presentState ][msgIn].nextStates[1] = newState1;

/* ################### GLOBAL VARIABLES ############### */
DehContext                g_DehCtx;
DeviceStatusContext       g_DeviceStatusCtx;
DehRhRspTimersContext     g_DehRhRspTimersCtx;
pthread_mutex_t           g_gpsMutex;
pthread_mutex_t           g_bistMutex;
pthread_t                 g_bistThread;

DehStateMachine  dehStateMachine[DEH_STATE_MAX][MSG_ID_MAX];
/* ################### Generic functions ############### */

/**
 * @details This function is used to initialise state machine in DEH
 **/
void initDehStateMachine()
{
  int i,j;

  /* Initialise the Function pointer to process Invalid Message
   * Then fill the valid processing functions
   */
  for(i=0; i< DEH_STATE_MAX; i++)
  {
    for(j=0; j<MSG_ID_MAX; j++ )
    {
      dehStateMachine[i][j].fp = NULL;
    }
  }

  /* Any timer Expiry */
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_TIMEKEEPER_DEH_TIMEOUT_IND, dehProcTimerExpiry, DEH_STATE_WAIT_CFG, DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_TIMEKEEPER_DEH_TIMEOUT_IND, dehProcTimerExpiry, DEH_STATE_WAIT_INIT_NOTI, DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_TIMEKEEPER_DEH_TIMEOUT_IND, dehProcTimerExpiry, DEH_STATE_IDLE, DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_ENABLE_RSP,  MSG_TIMEKEEPER_DEH_TIMEOUT_IND, dehProcTimerExpiry, DEH_STATE_WAIT_DEV_ENABLE_RSP , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_DISABLE_RSP, MSG_TIMEKEEPER_DEH_TIMEOUT_IND, dehProcTimerExpiry, DEH_STATE_WAIT_DEV_DISABLE_RSP , DEH_STATE_UNDEFINED );

  /* Set message trace from BSM */
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_BSM_DEH_SET_MSG_TRACE, dehProcSetMsgTraceInAllStates, DEH_STATE_WAIT_CFG , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_BSM_DEH_SET_MSG_TRACE, dehProcSetMsgTraceInAllStates, DEH_STATE_WAIT_INIT_NOTI , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_BSM_DEH_SET_MSG_TRACE, dehProcSetMsgTraceInAllStates, DEH_STATE_IDLE , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_ENABLE_RSP,  MSG_BSM_DEH_SET_MSG_TRACE, dehProcSetMsgTraceInAllStates, DEH_STATE_WAIT_DEV_ENABLE_RSP , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_DISABLE_RSP, MSG_BSM_DEH_SET_MSG_TRACE, dehProcSetMsgTraceInAllStates, DEH_STATE_WAIT_DEV_DISABLE_RSP , DEH_STATE_UNDEFINED );

  //Added handling for message SET_LOG_CFG from BSM
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_BSM_DEH_SET_LOG_CFG, dehProcSetLogCfgInAllStates, DEH_STATE_WAIT_CFG, DEH_STATE_WAIT_CFG);
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_BSM_DEH_SET_LOG_CFG, dehProcSetLogCfgInAllStates, DEH_STATE_WAIT_INIT_NOTI, DEH_STATE_WAIT_INIT_NOTI);
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_BSM_DEH_SET_LOG_CFG, dehProcSetLogCfgInAllStates, DEH_STATE_IDLE, DEH_STATE_IDLE );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_ENABLE_RSP,  MSG_BSM_DEH_SET_LOG_CFG, dehProcSetLogCfgInAllStates, DEH_STATE_WAIT_DEV_ENABLE_RSP, DEH_STATE_WAIT_DEV_ENABLE_RSP );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_DISABLE_RSP, MSG_BSM_DEH_SET_LOG_CFG, dehProcSetLogCfgInAllStates, DEH_STATE_WAIT_DEV_DISABLE_RSP, DEH_STATE_WAIT_DEV_DISABLE_RSP );

  /* Sw Exit */
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_PRM_DEH_EXIT_CMD, dehProcExitInAllStates, DEH_STATE_IDLE , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_PRM_DEH_EXIT_CMD, dehProcExitInAllStates, DEH_STATE_IDLE , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_PRM_DEH_EXIT_CMD, dehProcExitInAllStates, DEH_STATE_IDLE , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_ENABLE_RSP,  MSG_PRM_DEH_EXIT_CMD, dehProcExitInAllStates, DEH_STATE_IDLE , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_DISABLE_RSP, MSG_PRM_DEH_EXIT_CMD, dehProcExitInAllStates, DEH_STATE_IDLE , DEH_STATE_UNDEFINED );

  /* Health Check */
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_PRM_DEH_HELLO, dehProcHelloInAllStates, DEH_STATE_WAIT_CFG ,            DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_PRM_DEH_HELLO, dehProcHelloInAllStates, DEH_STATE_WAIT_INIT_NOTI ,      DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_PRM_DEH_HELLO, dehProcHelloInAllStates, DEH_STATE_IDLE ,                DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_ENABLE_RSP,  MSG_PRM_DEH_HELLO, dehProcHelloInAllStates, DEH_STATE_WAIT_DEV_ENABLE_RSP , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_DISABLE_RSP, MSG_PRM_DEH_HELLO, dehProcHelloInAllStates, DEH_STATE_WAIT_DEV_DISABLE_RSP, DEH_STATE_UNDEFINED );

  /* IP Address configuration */
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_BSM_DEH_SET_IP_ADDR_REQ, dehProcSetIpAddrReq, DEH_STATE_WAIT_CFG  , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_BSM_DEH_SET_IP_ADDR_REQ, dehProcSetIpAddrReq, DEH_STATE_WAIT_INIT_NOTI , DEH_STATE_IDLE );

  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_BSM_DEH_INIT_CONFIG_REQ, dehProcInitConfigReq, DEH_STATE_IDLE  , DEH_STATE_WAIT_INIT_NOTI );
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_BSM_DEH_INIT_CONFIG_REQ, dehProcInitConfigReq, DEH_STATE_IDLE , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_BSM_DEH_INIT_CONFIG_REQ, dehProcInitConfigReq, DEH_STATE_WAIT_INIT_NOTI , DEH_STATE_IDLE );

  /* Radio Head Connects To Base Station is Handled in Radio Head Message handler and also in Time out */
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_BSM_DEH_RH_CONFIG_REQ, dehProcConfigReq, DEH_STATE_IDLE  , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_BSM_DEH_RH_CONFIG_REQ, dehProcConfigReq, DEH_STATE_IDLE , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_BSM_DEH_RH_CONFIG_REQ, dehProcConfigReq, DEH_STATE_WAIT_INIT_NOTI , DEH_STATE_UNDEFINED );

  /* eNb-Enable  ---> OP-START  */
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_BSM_DEH_OPSTART_REQ, dehProcOpStartReq,             DEH_STATE_WAIT_DEV_ENABLE_RSP , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_BSM_DEH_OPSTART_REQ, dehProcOpStartReqInInitNoti,   DEH_STATE_WAIT_INIT_NOTI , DEH_STATE_UNDEFINED );

  /* eNb-Disable ---> OP-STOP  */
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_BSM_DEH_OPSTOP_REQ,  dehProcOpStopReq,    DEH_STATE_WAIT_DEV_DISABLE_RSP , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_BSM_DEH_OPSTOP_REQ,  dehProcOpStopReq,    DEH_STATE_WAIT_INIT_NOTI , DEH_STATE_UNDEFINED );

  /* RadioHead(LK) SET-OBJ - Handle SET in all states as EnbAdmin table commands needs to be honoured in all states */
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_BSM_DEH_SET_OBJ_REQ, dehProcSetObjReq, DEH_STATE_WAIT_INIT_NOTI,       DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_BSM_DEH_SET_OBJ_REQ, dehProcSetObjReq, DEH_STATE_WAIT_CFG,             DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_BSM_DEH_SET_OBJ_REQ, dehProcSetObjReq, DEH_STATE_IDLE,                 DEH_STATE_UNDEFINED ); /* TODO : Output Status May change */
  InitDehStateMachine( DEH_STATE_WAIT_DEV_ENABLE_RSP,  MSG_BSM_DEH_SET_OBJ_REQ, dehProcSetObjReq, DEH_STATE_WAIT_DEV_ENABLE_RSP,  DEH_STATE_WAIT_INIT_NOTI );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_DISABLE_RSP, MSG_BSM_DEH_SET_OBJ_REQ, dehProcSetObjReq, DEH_STATE_WAIT_DEV_DISABLE_RSP, DEH_STATE_WAIT_INIT_NOTI );

  /* GET-OBJ */
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_BSM_DEH_GET_OBJ_REQ, dehProcGetObjReq, DEH_STATE_IDLE ,                DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_BSM_DEH_GET_OBJ_REQ, dehProcGetObjReq, DEH_STATE_WAIT_CFG ,            DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_BSM_DEH_GET_OBJ_REQ, dehProcGetObjReq, DEH_STATE_WAIT_INIT_NOTI ,      DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_ENABLE_RSP,  MSG_BSM_DEH_GET_OBJ_REQ, dehProcGetObjReq, DEH_STATE_WAIT_DEV_ENABLE_RSP , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_DISABLE_RSP, MSG_BSM_DEH_GET_OBJ_REQ, dehProcGetObjReq, DEH_STATE_WAIT_DEV_DISABLE_RSP, DEH_STATE_UNDEFINED );

  /* GET-ENB-ADDR */
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_BSM_DEH_GET_ENB_ADDR_REQ, dehProcGetEnbAddrReq, DEH_STATE_IDLE ,                DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_BSM_DEH_GET_ENB_ADDR_REQ, dehProcGetEnbAddrReq, DEH_STATE_WAIT_CFG ,            DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_BSM_DEH_GET_ENB_ADDR_REQ, dehProcGetEnbAddrReq, DEH_STATE_WAIT_INIT_NOTI ,      DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_ENABLE_RSP,  MSG_BSM_DEH_GET_ENB_ADDR_REQ, dehProcGetEnbAddrReq, DEH_STATE_WAIT_DEV_ENABLE_RSP , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_DISABLE_RSP, MSG_BSM_DEH_GET_ENB_ADDR_REQ, dehProcGetEnbAddrReq, DEH_STATE_WAIT_DEV_DISABLE_RSP, DEH_STATE_UNDEFINED );

  /* GPS detail */
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_BSM_DEH_GPS_DETAIL_REQ, dehProcGpsDetailsReq,   DEH_STATE_IDLE , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_BSM_DEH_GPS_DETAIL_REQ, dehProcGpsDetailsReq,   DEH_STATE_WAIT_CFG , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_BSM_DEH_GPS_DETAIL_REQ, dehProcGpsDetailsReq,   DEH_STATE_WAIT_INIT_NOTI , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_ENABLE_RSP,  MSG_BSM_DEH_GPS_DETAIL_REQ, dehProcGpsDetailsReq,   DEH_STATE_WAIT_DEV_ENABLE_RSP , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_DISABLE_RSP, MSG_BSM_DEH_GPS_DETAIL_REQ, dehProcGpsDetailsReq,   DEH_STATE_WAIT_DEV_DISABLE_RSP , DEH_STATE_UNDEFINED );

  /* BSM DEH DEVICE INIT request Procedure */
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_BSM_DEH_DEVICE_INIT_REQ, dehProcDeviceInitReq,    DEH_STATE_IDLE,            DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_BSM_DEH_DEVICE_INIT_REQ, dehProcDeviceInitReq,    DEH_STATE_WAIT_CFG ,       DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_BSM_DEH_DEVICE_INIT_REQ, dehProcDeviceInitReq,    DEH_STATE_WAIT_INIT_NOTI , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_ENABLE_RSP,  MSG_BSM_DEH_DEVICE_INIT_REQ, dehProcDeviceInitReq,    DEH_STATE_WAIT_DEV_ENABLE_RSP , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_DISABLE_RSP, MSG_BSM_DEH_DEVICE_INIT_REQ, dehProcDeviceInitReq,    DEH_STATE_WAIT_DEV_DISABLE_RSP , DEH_STATE_UNDEFINED );

  /* BSM DEH LED SET request Procedure */
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_BSM_DEH_SET_LED_COLOR, dehProcSetLedColor,    DEH_STATE_IDLE,            DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_BSM_DEH_SET_LED_COLOR, dehProcSetLedColor,    DEH_STATE_WAIT_CFG ,       DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_BSM_DEH_SET_LED_COLOR, dehProcSetLedColor,    DEH_STATE_WAIT_INIT_NOTI , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_ENABLE_RSP,  MSG_BSM_DEH_SET_LED_COLOR, dehProcSetLedColor,    DEH_STATE_WAIT_DEV_ENABLE_RSP , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_DISABLE_RSP, MSG_BSM_DEH_SET_LED_COLOR, dehProcSetLedColor,    DEH_STATE_WAIT_DEV_DISABLE_RSP , DEH_STATE_UNDEFINED );

  /* PRM DEH RH-SW INSTALL request Procedure */
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_PRM_DEH_RH_SW_INSTALL_REQ, dehProcRhSwInstallReq,    DEH_STATE_IDLE,            DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_PRM_DEH_RH_SW_INSTALL_REQ, dehProcRhSwInstallReq,    DEH_STATE_WAIT_CFG ,       DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_PRM_DEH_RH_SW_INSTALL_REQ, dehProcRhSwInstallReq,    DEH_STATE_WAIT_INIT_NOTI , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_ENABLE_RSP,  MSG_PRM_DEH_RH_SW_INSTALL_REQ, dehProcRhSwInstallReq,    DEH_STATE_WAIT_DEV_ENABLE_RSP , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_DISABLE_RSP, MSG_PRM_DEH_RH_SW_INSTALL_REQ, dehProcRhSwInstallReq,    DEH_STATE_WAIT_DEV_DISABLE_RSP , DEH_STATE_UNDEFINED );

  /* PRM DEH RH-SW ACTIVATE request Procedure */
  InitDehStateMachine( DEH_STATE_IDLE,                 MSG_PRM_DEH_RH_SW_ACTIVATE_REQ, dehProcRhSwActivateReq,    DEH_STATE_IDLE,            DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_CFG,             MSG_PRM_DEH_RH_SW_ACTIVATE_REQ, dehProcRhSwActivateReq,    DEH_STATE_WAIT_CFG ,       DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_INIT_NOTI,       MSG_PRM_DEH_RH_SW_ACTIVATE_REQ, dehProcRhSwActivateReq,    DEH_STATE_WAIT_INIT_NOTI , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_ENABLE_RSP,  MSG_PRM_DEH_RH_SW_ACTIVATE_REQ, dehProcRhSwActivateReq,    DEH_STATE_WAIT_DEV_ENABLE_RSP , DEH_STATE_UNDEFINED );
  InitDehStateMachine( DEH_STATE_WAIT_DEV_DISABLE_RSP, MSG_PRM_DEH_RH_SW_ACTIVATE_REQ, dehProcRhSwActivateReq,    DEH_STATE_WAIT_DEV_DISABLE_RSP , DEH_STATE_UNDEFINED );

}

/* Handler for Messages coming from RH */
/**
 * @param   msgIn contains Message received from RH(LK)
 * @details This function is to process Incoming Messages from Other Modules to DEH and calls respective handling function.
 */
void rhMsgHandler(
  IO MsgBuf *rhMsgIn
  )
{
  DecBuf     decBuf;
  MailBox    dstMailbox;
  MailBox    srcMailbox;
  MsgId      msgId;

  decBuf.index = 0;
  decBuf.len = rhMsgIn->len;
  decBuf.value = &(rhMsgIn->val[0]);

  dstMailbox = getUint16( &decBuf );
  srcMailbox = getUint16( &decBuf );
  msgId      = getUint32( &decBuf );

  /* Messages received from Radio Head */
  switch(msgId)
  {
    case MSG_LK_BS_INITIALIZATION_NOTI_ID:    /* Initialisation Notification */
    {
      logMsgIn("Received Msg Name is [%s] ", "MSG_LK_BS_INITIALIZATION_NOTI_ID");
      procRhDehInitNoti( &decBuf, msgId);
      break;
    }
    case MSG_LK_BS_DATA_PATH_RSP:			  /* Data Path Setting RSP */
    {
      logMsgIn("Received Msg Name is [%s] ", "MSG_LK_BS_DATA_PATH_RSP");
      procRhDehDataPathRsp( &decBuf, msgId);
      break;
    }
    case MSG_LK_BS_FA_CONTROL_RSP:           /* FA Control Setting RSP */
    {
      logMsgIn("Received Msg Name is [%s] ", "MSG_LK_BS_FA_CONTROL_RX_RSP");
      procRhDehFaControlRsp( &decBuf, msgId);
      break;
    }
    case MSG_LK_BS_ALARM_THRESHOLD_SETTING_RSP:     /* Alarm Threshold Setting RSP */
    {
      logMsgIn("Received Msg Name is [%s]  ", "MSG_LK_BS_ALARM_THRESHOLD_SETTING_RSP");
      procRhDehAlarmThresholdSetRsp( &decBuf, msgId);
      break;
    }
    case MSG_LK_BS_DELAY_SETTING_RSP:       /* Delay Setting RSP */
    {
      logMsgIn("Received Msg Name is [%s] ", "MSG_LK_BS_DELAY_SETTING_RSP");
      procRhDehDelaySettingRsp( &decBuf, msgId);
      break;
    }
    case MSG_LK_BS_TX_ON_OFF_SET_RSP:      /* Set TX ON/OFF Setting RSP */
    {
      logMsgIn("Received Msg Name is [%s] ", "MSG_LK_BS_TX_ON_OFF_SET_RSP");
      procRhDehTxOnOffRsp( &decBuf, msgId);
      break;
    }
    case MSG_LK_BS_ENABLE_OR_DISABLE_RSP:   /* Enable/Disable Path RSP */
    {
      logMsgIn("Received Msg Name is [%s] ", "MSG_LK_BS_ENABLE_OR_DISABLE_RSP");
      procRhEnableDisableRsp( &decBuf, msgId);
      break;
    }
    case MSG_LK_BS_RESET_RSP:             /*  Reset RSP */
    {
      logMsgIn("Received Msg Name is [%s]  ", "MSG_LK_BS_RESET_RSP");
      procRhDehResetRsp( &decBuf);
      break;
    }
    case MSG_LK_BS_STATUS_INFO_RSP:       /*  Status RSP */
    {
      logMsgIn("Received Msg Name is [%s] ", "MSG_LK_BS_STATUS_INFO_RSP");
      procRhDehStatusRsp( &decBuf);
      break;
    }
    case MSG_LK_BS_ALARM_CHANGE_NOTI_ID:      /* Alarm Change Notification */
    {
      logMsgIn("Received Msg Name is [%s]  ", "MSG_LK_BS_ALARM_CHANGE_NOTI_ID");
      procRhDehAlarmChangeNoti( &decBuf);
      break;
    }
    case MSG_LK_BS_FW_INFO_RSP:           /*  Firmware Info RSP */
    {
      logMsgIn("Received Msg Name is [%s] ", "MSG_LK_BS_FW_INFO_RSP");
      procRhDehFwInfoRsp( &decBuf);
      break;
    }
    case MSG_LK_BS_HW_INFO_RSP:           /*  Hardware Info RSP */
    {
      logMsgIn("Received Msg Name is [%s] ", "MSG_LK_BS_HW_INFO_RSP");
      procRhDehHwInfoRsp( &decBuf);
      break;
    }
    case MSG_LK_BS_OPERATION_MODE_INFO_RSP:  /*  Operating Mode Info RSP */
    {
      logMsgIn("Received Msg Name is [%s] ", "MSG_LK_BS_OPERATION_MODE_INFO_RSP");
      procRhDehOperatingModeInfoRsp( &decBuf);
      break;
    }
    case MSG_LK_BS_SW_INSTALL_RSP:  /*  RH Software Install RSP*/
    {
      logMsgIn("Received Msg Name is [%s] ", "MSG_BS_LK_SW_INSTALL_RSP");
      procRhDehSwInstallRsp(&decBuf);
      break;
    }
    case MSG_LK_BS_FUSING_COMPLETE_NOTI_ID:  /*  Remote Fusing Complete Noti */
    {
      logMsgIn("Received Msg Name is [%s] ", "MSG_LK_BS_FUSING_COMPLETE_NOTI_ID");
      procRhDehFusingCompleteNoti( &decBuf);
      break;
    }
    case MSG_LK_BS_SET_ACTIVE_SIDE_RSP:  /*  Set RH  Active side RSP*/
    {
      logMsgIn("Received Msg Name is [%s] ", "MSG_BS_LK_SW_ACTIVATE_RSP");
      procRhDehSwActivateRsp( &decBuf , msgId);
      break;
    }
    default:                              /* Invalid Message */
      logError("Valid Msg Not found,Msg Id Received is =%d", msgId);
    break;
  }

}

/* Receive LK Message */

/**
 * @param sockFd This parameter corresponds to the socket used to receive messages from LK.
 * @param buf    This parameter points to buffer that contains the message received from LK.
 * @details      This function checks for reception of message from LK.@n
 *               This is strictly meant for LK.
 */
inline TBOOL recvRhMsg(
  IN SINT32    *sockFd,
  IO MsgBuf     *buf
  )
{
  buf->len = recv(*sockFd, &buf->val[0], SIZE_RECV_DATA, SOCK_FLAGS);
  if (buf->len EQ -1)
  {
    return FALSE;
  }
  return TRUE;
}

/**
 * @param modId      The module where the message is received, this is used to identify the dest queue.
 * @param msgHandler The function to invoke after a message is received at the queue.
 * @details This function runs forever. Schedules Messages @n
 * coming from IPC framework and also from Radio head.
 */
void schedDehMsgHandler(
  IN ModuleId   modId,
  IN MsgHandler msgHandler
)
{
  Msg     *msgIn;
  MsgId    msgId;
  UINT16   ec;
  MsgBuf     buf;
  fd_set   masterReadFdSet;
  fd_set   readFdSet;
  buf.len = 0;
  struct timeval timeAtStart,timeAtMsgIn,timeAtMsgOut,msgProcDuration;
  int    retVal = 0;

  ModAddr *modAddr = getModAddr(modId);
  /* Creating readFdSet */
  IMC_INET_FD_ZERO(&masterReadFdSet);
  IMC_INET_FD_SET( modAddr->recvFd,    &masterReadFdSet);
  IMC_INET_FD_SET( *getSocketFd(),     &masterReadFdSet);

  while(1)
  {
    gettimeofday (&timeAtStart, NULL);
    /* Initialising readFdSet. Needed Init after every select() call */
    readFdSet = masterReadFdSet;
    /* Waiting for any message received on any of the port */
    retVal = select(65000 ,&readFdSet,NULL,NULL,NULL);
    if (-1 EQ retVal)
    {
      if (errno EQ EINTR )
      {
        continue;
      }
      logError("Error in selecting the FDs: selectRetVal=%d, errno =%d", retVal, errno);
    }
    else
    {
      /* Handle if any Messages received from BSM, PRM */
      if (IMC_INET_FD_ISSET(modAddr->recvFd, &readFdSet))
      {
        while((msgIn = recvMsg(modId, &ec)))
        {
          gettimeofday (&timeAtMsgIn, NULL);
          msgId = msgIn->msgId;
          msgHandler(msgIn);
          gettimeofday (&timeAtMsgOut, NULL);
          getLapsedTime(&timeAtMsgIn, &timeAtMsgOut, &msgProcDuration);
          if(msgProcDuration.tv_usec > MAX_MSG_PROCESSING_DURATION)
          {
            BS_ERROR_MSG_PROC_DURAT_EXCEEDED( msgId, msgProcDuration.tv_usec );
          }
        }
      }
      /* Handle if any Messages received from LK (RSP) */
      if ( IMC_INET_FD_ISSET(*getSocketFd(), &readFdSet))
      {
        if (recvRhMsg(getSocketFd(), &buf) EQ TRUE)
        {
          gettimeofday (&timeAtMsgIn, NULL);
          rhMsgHandler(&buf);
          gettimeofday (&timeAtMsgOut, NULL);
          getLapsedTime(&timeAtMsgIn, &timeAtMsgOut, &msgProcDuration);
          if(msgProcDuration.tv_usec > MAX_MSG_PROCESSING_DURATION)
          {
            BS_ERROR_MSG_PROC_DURAT_EXCEEDED(  MSG_UNDEFINED /* @todo replace with LK message; this is not IPC msg */, msgProcDuration.tv_usec );
          }
        }
      }
    }
    printAndEmptyErrorChain(&(getModuleContext()->errChain));
  }
}

/**
 * @param msgIn This parameter points to the message received from the BSM.
 * @details This function is the entry point for all messages coming into DEH from BSM
 */
void dehMain( FR Msg *msgIn )
{
  DehContext *dehContext = getDehContext();

  DehState   prevState;
  logInfo("Received Message=%d, in state=%s", msgIn->msgId, getDehStateName(dehContext->dehState) );
  if( NULL NOTEQ dehStateMachine[dehContext->dehState][msgIn->msgId].fp )
  {
    prevState = dehContext->dehState;
    dehStateMachine[dehContext->dehState][msgIn->msgId].fp( msgIn, dehContext );

    if( (dehContext->dehState NOTEQ dehStateMachine[ prevState ][msgIn->msgId].nextStates[0]) &&
        (dehContext->dehState NOTEQ dehStateMachine[ prevState ][msgIn->msgId].nextStates[1]) )
    {
      logError("PROGRAMMING ERROR !! Not In expected State.  Message=%d, Previous state=%d , Current State = %d (Expected States :%d and %d )",
                                          msgIn->msgId, prevState, dehContext->dehState,
                                          dehStateMachine[ prevState ][msgIn->msgId].nextStates[0],
                                          dehStateMachine[ prevState ][msgIn->msgId].nextStates[1]);
    }
  }
  else
  {
    BS_ERROR_RECEIVED_INVALID_MSG( msgIn->msgId, dehContext->dehState );
  }

  printAndEmptyErrorChain(&(getModuleContext()->errChain));
  /* Free the received Message */
  freeMsgIn(msgIn);
}


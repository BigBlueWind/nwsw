/*
 * s1ap.h
 *
 * This file contains S1AP Manager functions.
 * @author: lmadala
 *
 *
 */

#ifndef S1AP_H
#define S1AP_H

/* Common include files */
#include "cmn_types.h"
#include "s1ap_cm.h"

#define MAX_SIZE_PLMN_STRING            150

typedef enum e_S1apMgrState
{
  S1_STATE_INIT,
  S1_STATE_SETUP_IN_PROG,
  S1_STATE_SETUP_DONE,
  S1_STATE_RESET_IN_PROG,
  S1_STATE_MAX
}S1apMgrState;

typedef enum e_S1apMgrEvent
{
  S1_MSG_OPSTART  = 0,
  S1_MSG_OPSTOP = 1,
  S1_MSG_ID_RESET = 2,
  S1_MSG_ID_RESET_ACK = 3,
  S1_MSG_ID_SETUP_RSP = 4,
  S1_MSG_ID_SETUP_FAIL = 5,
  S1_MSG_ID_PAGING = 6,
  S1_MSG_PROC_GUARD_TMR_EXPIRY = 7,
  S1_MSG_SETUP_TMR_EXPIRY = 8,
  S1_MSG_LINK_DOWN = 9,
  S1_MSG_LINK_UP = 10,
  S1_MSG_MME_CFG_UPDATE = 11,
  S1_MSG_OVERLOAD_START = 12,
  S1_MSG_OVERLOAD_STOP = 13,
  S1_MSG_SND_RESET = 14,
  S1_MSG_WARNING_REQ = 15,
  S1_MSG_ABORT_CFM = 16,
  S1_MSG_MAX
}S1apMgrEvent;

typedef struct s_EnodeBS1apConfig
{
  UINT8             enbName[MAX_SIZE_ENBNAME];
  GlobalEnodeBId    globalEnodeBId;
  UINT16            numOfSupportedTac;
  SupportedTac      tacs[MAX_TAC_SUPPORTED];
  UINT16            pagingDrx;
  S1apMgrState      s1State;
  PcmTimer          guardTimer;
  PcmTimer          setupTimer;
  UINT16            s1SetupRetryCount;
  // ADD MME Specific Fields
  UINT8             mmeName[MAX_SIZE_MMENAME];
  UINT8             mmeIpAddr[MAX_SIZE_MMECONFIG_MMEIPADDR + 1 ];
  ServedGummeies    servedGummeIes;
  RelativeMmeCapacity relativeMmeCapacity;
  UINT16            procGuardTimerDuration;
  UINT32            peerId;     /* This value is given in the config request from BSM */
  UINT16            maxS1SetupAllowed;
  TBOOL             isS1LinkDownReported;
  TBOOL             isS1LinkClosed;
  OverloadAction    overloadState;
}S1apMgrCb;

S1apMgrCb *getS1MgrCb(UINT32 peerId);
#endif /* S1AP_H */

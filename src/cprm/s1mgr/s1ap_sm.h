/**
 * @file    s1ap_sm.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @details
 * This file contains S1AP Manager State Machine function prototypes and the MAcro to
 * invoke the state machine and set the state of s1Cb
 *
 * @author Lakshmi Narayana Madala
 *
 */

#ifndef S1AP_SM_H
#define S1AP_SM_H

/* Common include files */
#include "cmn_types.h"
#include "s1ap_cm.h"
#include "s1ap_err.h"
#include "s1ap.h"


#define s1apSetState( s1Cb, newState ) \
  logState("S1AP|state %s(%d)->%s(%d)", \
      s1apGetUeStateName(s1Cb->s1State),\
      s1Cb->s1State, \
      s1apGetUeStateName(newState), \
      newState); s1Cb->s1State = newState

#define S1AP_ERROR_UNEXPECTED_EVENT(_s1Cb,_s1apEvent) logError("Unexpected Event [%d:%s] received in state [%s]", s1apEvent, s1apGetUeEventName(s1apEvent), s1apGetUeStateName(s1Cb->s1State))


PUBLIC U8 s1apFsm(
  IO S1apMgrCb    *Cb,
  IN S1apMgrEvent  s1apEvent,
  IN SztUDatEvnt  *uDatEvnt
);


U8 s1apHndlOpStart(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    );

U8 s1apHndlOpStop(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    );

U8 s1apHndlReset(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    );

U8 s1apHndlResetAck(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    );

U8 s1apHndlSetupRsp(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    );

U8 s1apHndlSetupFail(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    );

U8 s1apHndlPaging(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    );

U8 s1apHndlGuardTmrExpiry(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    );

U8 s1apHndlSetupTmrExpiry(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    );

U8 s1apHndlLinkDown(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    );

U8 s1apHndlLinkUp(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    );

U8 s1apHndlMmeCfgUpdate(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    );

U8 s1apHndlOverloadStart(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    );

U8 s1apHndlOverloadStop(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    );

U8 s1apHndlSndReset(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    );

U8 s1apHndlWarningReq(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    );

U8 s1apHndlAbortCfm (
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    );

char *s1apGetUeEventName(
  IN S1apMgrEvent s1apMsg
  );

char *s1apGetUeStateName(
  IN S1apMgrState s1apState
  );

void s1apFillSetupReq(
    IO S1SetupReq *setupReq,
    IN S1apMgrCb  *s1apConfig
    );
void s1apFillUeResetReq(
  IO S1ResetReq    *resetReq,
     S1Cause        resetCause,
     MmeUeS1apId    mmeUeS1apId,
     EnbUeS1apId    enbUeS1apId
);
void s1apFillGlobalResetReq(
    IO S1ResetReq *resetReq
    );
void s1apFillResetAck(
    IO S1ResetAck    *resetAck,
    IN S1apMgrCb     *s1Cb,
    IN S1ResetReq    *reset
    );
void s1apFillMmeCfgUpdateFail(
    IO MmeCfgUpdateFail    *mmeCfgUpdateFail,
    IN S1apMgrCb           *s1Cb
    );
void s1apSendS1SetupReq(
    IN U32         peerId,
    IN S1SetupReq *setupReq
  );
void s1apSendAbortReq(
     SztAbortS1 *s1Abort
  );
void s1apSendResetReq(
    IN U32         peerId,
    IN S1ResetReq *resetReq
  );
void s1apSendResetAck(
    IN U32         peerId,
    IN S1ResetAck *resetAck
  );
void s1apSendMmeCfgUpdateFail(
    IN U32                 peerId,
    IN MmeCfgUpdateFail    *mmeCfgUpdFail
  );

void s1apProcOpStartInInit(
    IO S1apMgrCb   *s1Cb
    );
void s1apProcOpStopInInit(
    IO S1apMgrCb   *s1Cb
    );

void s1apProcSetupRspInSetupInProg(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    );
void s1apProcGuardTmrExpiryInSetupInProg(
    IO S1apMgrCb   *s1Cb
    );
void s1apProcSetupTmrExpiryInSetupInProg(
    IO S1apMgrCb   *s1Cb
    );
void s1apProcSetupFailInSetupInProg(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    );
void s1apProcOpStopInSetupInProg(
    IO S1apMgrCb   *s1Cb
    );
void s1apProcLinkDownInSetupInProg(
    IO S1apMgrCb   *s1Cb
    );
void s1apProcLinkUpInSetupInProg(
    IO S1apMgrCb   *s1Cb
    );

void s1apProcSndResetInSetupDone(
    IO S1apMgrCb   *s1Cb
    );
void s1apProcOpStopInSetupDone(
    IO S1apMgrCb   *s1Cb
    );
void s1apProcPagingInSetupDone(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    );
void s1apProcResetInSetupDone(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    );
void s1apProcLinkDownInSetupDone(
    IO S1apMgrCb   *s1Cb
    );
void s1apProcMmeCfgUpdateInSetupDone(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    );
void s1apProcOverloadStartInSetupDone(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    );
void s1apProcOverloadStopInSetupDone(
    IO S1apMgrCb   *s1Cb
    );
void s1apProcGuardTmrExpiryInResetInProg(
    IO S1apMgrCb   *s1Cb
    );
void s1apProcOpStopInResetInProg(
    IO S1apMgrCb   *s1Cb
    );
void s1apProcResetAckInResetInProg(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    );
void s1apProcResetInResetInProg(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    );
void s1apProcLinkDownInResetInProg(
    IO S1apMgrCb   *s1Cb
    );
void s1apProcWarningReqInAllStates(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    );
void s1apProcAbortCfmInAllStates(
    IO S1apMgrCb *s1Cb
    );
void s1apReTrySetupReqMsg(
    IO S1apMgrCb *s1Cb
    );
#endif /* S1AP_SM_H */

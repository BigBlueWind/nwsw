/**
 * @file    ueh_crm_sm.h
 * @brief API for
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * Change History :
 */

#ifndef  UEH_CRM_SM_H
#define  UEH_CRM_SM_H

#include "ccpu_cmn.h"

/* RRM related includes */
#include "../../s1mgr/s1ap_cm.h"
#include "ueh_eut.h"
#include "lueh.h"
#include "ueh_eut_x.h"
#include "lueh.x"

#if 0
/*CELL Specific events*/

typedef enum e_UehCellEvent {
  CELL_EVT_OP_STOP        =     0,
  CELL_EVT_S1_LINK_DOWN   =     1,
  CELL_EVT_S1_RESET       =     2,
  CELL_EVT_OP_START       =     3,
  CELL_EVT_S1_LINK_UP     =     4,
  CELL_EVT_OP_STOP_TMR_EXPIRY = 5,
  CELL_EVT_UE_CNT_ZERO    =     6,
  CELL_EVT_PAGING         =     7,
  CELL_MAX_EVT
} UehCellEvent;

#define getCellAvState(cb)       ( ( ((cb)->isS1LinkUp) && ((cb)->isUnlocked) ) ? UEH_CELL_STATE_AVAILABLE : UEH_CELL_STATE_UNAVAILABLE)

/* Cell procedues */
typedef enum e_UehCellProc 
{
  UEH_CELL_PROC_NONE                   =  0,
  UEH_CELL_PROC_OP_STOP_ONGOING        =  1,
  UEH_CELL_PROC_S1_LINK_DOWN_ONGOING   =  2,
  UEH_CELL_PROC_S1_RESET_ONGOING       =  3,
  UEH_CELL_PROC_MAX
} UehCellProc;
#endif

/*
#define CELL_EVT_OP_STOP                     0
#define CELL_EVT_S1_LINK_DOWN                1
#define CELL_EVT_S1_RESET                    2
#define CELL_EVT_OP_START                    3
#define CELL_EVT_S1_LINK_UP                  4
#define CELL_EVT_OP_STOP_TMR_EXPIRY          5
#define CELL_EVT_UE_CNT_ZERO                 6
#define CELL_EVT_PAGING                      7
#define CELL_MAX_EVT                         8
*/

void cellFsm2 (UehCellCb *cb, UehCellEvent event, void *data);
#if 0
typedef void(*CellFsm)   (UehCellCb *cellCb, void *sdu);
extern  CellFsm cellFsm[UEH_CELL_MAX_STATE][CELL_MAX_EVT];
#endif
#if 0
#define crmInvokeCellSm( cellCb, event, pdu ) \
{ \
  if( NULL != cellCb ) \
  { \
    char currTime[CEM_EVNT_TIME_STR_MAX_LEN]; \
    char trc[CEM_EVNT_TRC_MAX_LEN];\
    UehStrTime(currTime);\
    snprintf(trc, CEM_EVNT_TRC_MAX_LEN, "%s|%s|%u|%d|%s|%s|%s:%d|Received in state=%d\n", currTime, "EVENT", (unsigned int)((cellCb)->cellId), (event), "CELSM", "CELSM", __FILE__, __LINE__, (cellCb)->state ); \
    UehSaveTrace(trc);\
    cellFsm[ (cellCb)->state][ (event) ]( (cellCb), (pdu) ); \
  }\
}
#endif

#define crmInvokeCellSm( cellCb, event, pdu ) \
do {\
  logStateChange("CELL|Before|Rx %s %d|isUnlocked=%d|isUnbarred=%d|isS1LinkUp=%d|numUeCfg=%d|sfn=%d|proc=%d",\
    #event, \
    event,  \
    (cellCb)->isUnlocked,\
    (cellCb)->isUnbarred,\
    (cellCb)->isS1LinkUp,\
    (cellCb)->numUeCfg,\
    (cellCb)->sfn,\
    (cellCb)->proc);\
    \
    cellFsm2(cellCb, event, pdu);\
    \
  logStateChange("CELL|After|Rx %s %d|isUnlocked=%d|isUnbarred=%d|isS1LinkUp=%d|numUeCfg=%d|sfn=%d|proc=%d",\
    #event, \
    event,  \
    (cellCb)->isUnlocked,\
    (cellCb)->isUnbarred,\
    (cellCb)->isS1LinkUp,\
    (cellCb)->numUeCfg,\
    (cellCb)->sfn,\
    (cellCb)->proc);\
} while(0)

//    cellFsm[ (cellCb)->state][ (event) ]( (cellCb), (pdu) )

#if 0
#define crmSetState( cellCb, newState ) \
{\
  char currTime[CEM_EVNT_TIME_STR_MAX_LEN]; \
  char trc[CEM_EVNT_TRC_MAX_LEN];\
  UehStrTime(currTime);\
  snprintf(trc, CEM_EVNT_TRC_MAX_LEN, "%s|%s|%u|%s|%s|%s|%s:%d|State change %d to %d\n", currTime, "STATE", ((cellCb)->cellId), "StateChange", "CELSM", "CELSM", __FILE__, __LINE__, ((cellCb)->state), newState); \
  UehSaveTrace(trc);\
  (cellCb)->state = newState; \
}


#define crmSetState( cellCb, newState ) \
{ logStateChange("cell=%d|%d->%d %s", (cellCb)->cellId, (cellCb)->state, newState, #newState); (cellCb)->state = newState; }

#define crmUpdateCellState(state) crmSetState( uehGetCellCb(), state)
#endif


void crmNoOpr(
    UehCellCb   *cellCb,
    void        *pdu
  );
void crmProcOpStartInUnexpectedState(
  UehCellCb   *cellCb,
  void        *pdu
  );
void crmProcS1ResetInUnexpectedState(
  UehCellCb   *cellCb,
  void        *pdu
  );
void crmProcS1LinkDownInUnexpectedState(
  UehCellCb   *cellCb,
  void        *pdu
  );
void crmProcOpstopInUnexpectedState(
  UehCellCb   *cellCb,
  void        *pdu
  );

void crmProcOpStartInUnavail(
  UehCellCb   *cellCb,
  void        *null
  );

void crmStartCallClearing(
    UehCellCb   *cellCb,
    EventType    event,
    S1Cause     *cause
    );
void crmStartCallClearingForReset(
    UehCellCb   *cellCb,
    EventType    event,
    S1ResetReq  *reset
    );
void crmProcS1LinkDownInAvail(
    UehCellCb   *cellCb,
    void        *null
  );
void crmProcResetReqInAvail(
    UehCellCb   *cellCb,
    void        *pdu
  );
void crmProcPagingMsgInAvail(
    UehCellCb   *cellCb,
    void        *pdu
  );
void crmProcOpStopReqInAvail(
    UehCellCb   *cellCb,
    void        *null
  );
void crmProcOpStopReqInUnavail(
    UehCellCb   *cellCb,
    void        *null
  );

void crmProcS1LinkdownInOpStop(
    UehCellCb   *cellCb,
    void        *null
  );
void crmProcResetReqInOpStop(
    UehCellCb   *cellCb,
    void        *pdu
  );
void crmProcOpStopTmrExpiryInOpStop(
    UehCellCb   *cellCb,
    void        *null
  );
void crmProcOpStartInOpStop(
    UehCellCb   *cellCb,
    void        *null
  );
void crmProcZeroActCallInOpStop(
    UehCellCb   *cellCb,
    void        *null
  );

void crmProcOpStopInS1LinkDown(
    UehCellCb   *cellCb,
    void        *null
  );
void crmProcZeroActCallInS1LinkDown(
    UehCellCb   *cellCb,
    void        *null
  );

void crmProcOpStopInS1Reset(
    UehCellCb   *cellCb,
    void        *null
  );
void crmProcS1LinkDownInS1Reset(
    UehCellCb   *cellCb,
    void        *null
  );
void crmProcZeroActCallInReset(
    UehCellCb   *cellCb,
    void        *null
  );
void crmProcResetReqInReset(
    UehCellCb   *cellCb,
    void        *pdu
  );
/*BS-1799 cell barring */
void crmProcOpStopInCellBarred(
    UehCellCb   *cellCb,
    void        *null
  );
void crmProcS1LinkDownInCellBarred(
    UehCellCb   *cellCb,
    void        *null
  );
void crmProcResetReqInCellBarred(
    UehCellCb   *cellCb,
    void        *null
  );
void crmProcPagingMsgInCellBArred(
    UehCellCb   *cellCb,
    void        *null
  );

#endif /* UEH_CRM_SM_H */

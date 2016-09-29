/**
 * @file    ueh_crm_sm.c
 * @brief This file contains call state machine for Cell
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 */

#include "ccpu_cmn.h"

/* RRM related includes */
//#include "ueh_eut.h"
//#include "lueh.h"
//#include "lueh.x"
#include "ueh.h"
#include "ueh_crm.h"
#include "ueh_crm_sm.h"
//#include "s1ap_wrap.h"
//#include "s1ap.h"
#include "s1ap_mgr.h"
#include "s1ap_unwrap.h"
#include "s1ap_cmn_proc.h"
//#include "ueh_s1ap_cm.h"
#include "pst_events.h"
#include "log.h"

#define canCellBeEnabled(cb)     ((cb)->isS1LinkUp && (cb)->isUnlocked)
#define isCellUnbarred(cb)       (cb)->isUnbarred
#define crmStartProc(cb, nproc)   logProc("START|CELL|prev=%d|new=%d", (cb)->proc, nproc), (cb)->proc = nproc
#define crmStopProc(cb)           logProc("STOP|CELL|%d", (cb)->proc),  (cb)->proc = UEH_CELL_PROC_NONE
#define crmAbortProc(cb)         logProc("ABORT|CELL|%d", (cb)->proc),(cb)->proc = UEH_CELL_PROC_NONE

void deallocCellResources(UehCellCb *cb);
void crmHandleOpstop(UehCellCb *cb, MsgBsmModOpStopReq *req);
void crmHandleOpstart(UehCellCb *cb);
void crmHandleUeCountZero(UehCellCb *cb);
void crmHandleS1LinkUp(UehCellCb *cb);
void crmHandleS1LinkDown(UehCellCb *cb);
void crmHandleOpstopExpiry(UehCellCb *cb);
void crmHandleS1Reset(UehCellCb *cb, S1ResetReq  *reset);
void crmHandlePaging(UehCellCb *cb, S1apPdu *pdu);

void deallocCellResources(UehCellCb *cb)
{
  uehFreePendingPagingList();
  uehCrmDeinitializeSfSrsTables();
  uehCrmDeinitializePucchResTables();
  deallocTeidPool();
  if(TRUE EQ cb->isSysInfoNeedToSend)
  {
    /* Apply the new configuration - this will be used at next opstart */
    copyBytes(&uehCb.modiUehCfgCb, sizeof(UehCfgCb), &uehCb.uehCfgCb);
    cb->isSysInfoNeedToSend = FALSE;
  }

  uehStopTmr(UEH_UESTATE_AUDIT_TMR, &uehCb.uehCtxHangDetection.ueStateAuditTmr, NULLP, 0);
  uehStopTmr(UEH_UEPROCEDURE_AUDIT_TMR, &uehCb.uehCtxHangDetection.ueProcedureAuditTmr, NULLP, 0);
  uehStopTmr(UEH_PUCCH_AUDIT_TMR, &uehCb.uehCtxHangDetection.pucchAuditTmr, NULLP, 0);
}

void crmProcOpStartInUnavail(
    UehCellCb   *cellCb,
    void        *null
  )
{
  OpStartFail opstartFail;
  clearBytes(&opstartFail, sizeof(OpStartFail));

  UECM_EVENTHDLR(-1,crmProcOpStartInUnavail,"OPSTART in AVBL state");
  logInfo("%s", "OPSTART in AVBL state");

  /* Initialize SRS Table */
  if(-1 == uehCrmInitializeSfSrsTables())
  {
    opstartFail.errorNum = BS_ERR_INTERNAL_ERROR_OCCURED;
    sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_FAIL, sizeof(OpStartFail), (U8*) &opstartFail);
    return;
  }

  /* Initialize PUCCH Resource Management */
  if(-1 == uehCrmInitializePucchResTables())
  {
    opstartFail.errorNum = BS_ERR_INTERNAL_ERROR_OCCURED;
    sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_FAIL, sizeof(OpStartFail), (U8*) &opstartFail);
    return;
  }

  /* Create the tunnel Id Allocation List */
  allocInitTeidPool();

  /* System Information Scheduling parameter handling */
  if(RFAILED == uehInitSysInfSch())
  {
    opstartFail.errorNum = BS_ERR_INTERNAL_ERROR_OCCURED;
    sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_FAIL, sizeof(OpStartFail), (U8*) &opstartFail);
    return;
  }

  /* Initialise pending paging List */
  if(RFAILED == uehInitPendingPagingList())
  {
    opstartFail.errorNum = BS_ERR_INTERNAL_ERROR_OCCURED;
    sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_FAIL, sizeof(OpStartFail), (U8*) &opstartFail);
    return;
  }

  //psuedophyTtiFlag = 1;
  /*BS-1799 Changes*/
  if (CELLBARRED_BARRED EQ uehCb.uehCfgCb.cellBarred)
  {
    cellCb->isUnbarred = FALSE;
    //crmUpdateCellState(UEH_CELL_STATE_BARRED);
  }
  else 
  {
    cellCb->isUnbarred = TRUE;
    //crmUpdateCellState(UEH_CELL_STATE_AVAILABLE);
  }
#ifdef LTE_L2_MEAS
  uehClearL2MeasCtx();
#endif
  cellCb->isUnlocked = TRUE;
  uehCb.systemOperational = 1;
  sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_SUCC, 0, NULL);
}

/*
 * Handler Functions in UEH_CELL_STATE_AVAILABLE state
 */
/**
 * @details We stop the calls under three scenarios,
 * When OP-STOP command is received from BSM,
 * When S1LinkDown is received from S1AP Manager,
 * When Reset is received from MME
 **/
void crmStartCallClearing(
    UehCellCb   *cellCb,
    EventType    event,
    S1Cause      *s1Cause
    )
{
  UehS1ConCb   *s1ConCb = NULLP;
  UehUeCb      *ueCb=NULLP;
  U32          indx = 0;

#if 0
  /* Stopping the UE Ctx Hang audit timers */
<<<<<<< .mine

  uehStopTmr(NULLP, UEH_UESTATE_AUDIT_TMR);
  uehStopTmr(NULLP, UEH_UEPROCEDURE_AUDIT_TMR);
  uehStopTmr(NULLP, UEH_PUCCH_AUDIT_TMR);
=======
  uehStopTmr(UEH_UESTATE_AUDIT_TMR,&(uehCb.uehCtxHangDetection.ueStateAuditTmr),NULLP,0);
  uehStopTmr(UEH_UEPROCEDURE_AUDIT_TMR,&(uehCb.uehCtxHangDetection.ueProcedureAuditTmr),NULLP,0);
  uehStopTmr(UEH_PUCCH_AUDIT_TMR,&(uehCb.uehCtxHangDetection.pucchAuditTmr),NULLP,0);
>>>>>>> .r8824
#endif

  /* If the number of Active calls in the system are ZERO
   * There is no point in starting Call Clearing, Send UE Count as ZERO.
   */
  if( 0 == cellCb->numUeCfg )
  {
    crmInvokeCellSm( cellCb, CELL_EVT_UE_CNT_ZERO, NULL );
    return ;
  }

  /* Clear the calls whose RRC Connection is Established.
   */

  for (indx = 0; indx < cellCb->maxRntiPool; indx++)
  {
    UEH_GET_S1CONCB(s1ConCb,indx);
    if ((NULLP !=  s1ConCb) && (NULLP != s1ConCb->ueCb))
    {
      if (NULLP != s1Cause)
      {
        s1ConCb->s1Cause = s1Cause->causeVal;
        s1ConCb->s1CauseType = s1Cause->causeType;
      }
    }
    UEH_GET_UECB_FROM_UEIDX(ueCb, indx);
    if(NULLP != ueCb && ueCb->isUeCbAllocated)
    {
      ueFsm(ueCb, event, NULLP);
    }
  }
}


void crmHandleOpstop(UehCellCb *cb, MsgBsmModOpStopReq *req)
{
  cb->enbDisableReason = req->enbDisableReason;
  uehCb.systemOperational = 0;
#ifdef LTE_L2_MEAS
  stopCounterCollection();
#endif
  switch(getCellAvState(cb))
  {
    case UEH_CELL_STATE_UNAVAILABLE:
      switch(cb->proc)
      {
        case UEH_CELL_PROC_NONE: 
          sendCemMsgToBsm(EVT_CEM_BSM_OPSTOP_CMPL,0, NULL);
        break;
        case UEH_CELL_PROC_S1_LINK_DOWN_ONGOING:
          /* call clearing is already on. wait for UE count to be 0. */
          uehStartTmr (UE_TMR_OP_STOP_REQ, &(cb->opStopTmr), (PTR)cb, 0);
          crmStartProc(cb, UEH_CELL_PROC_OP_STOP_ONGOING);
        break;
        case UEH_CELL_PROC_OP_STOP_ONGOING:
          /* duplicate - ignore */
        break;
        default: /* unexpected */
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_OP_STOP);
        break;
      } /* proc */
    break;
    case UEH_CELL_STATE_AVAILABLE:
      switch(cb->proc)
      {
        case UEH_CELL_PROC_NONE: 
          uehStartTmr (UE_TMR_OP_STOP_REQ, &(cb->opStopTmr), (PTR)cb, 0);
          crmStartProc( cb, UEH_CELL_PROC_OP_STOP_ONGOING );
          /* Report the OAM Intervention to MME by sending S1 Global Reset */
          s1apSndResetToPeerOamInt();
          crmStartCallClearing( cb, EVENTTYPE_OPSTOPRELEASE, NULLP);
         break;
        case UEH_CELL_PROC_S1_RESET_ONGOING:
          /* call clearing is already on.  wait for UE count to be 0.  */
          uehStartTmr (UE_TMR_OP_STOP_REQ, &(cb->opStopTmr), (PTR)cb, 0);
          crmStartProc(cb, UEH_CELL_PROC_OP_STOP_ONGOING);
        break;
        default: /* unexpected */
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_OP_STOP);
        break;
      } /* proc */
    break; 
    default:
      CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_OP_STOP);
    break;
  } /* state */
  cb->isUnlocked = FALSE;
}


void crmHandleS1LinkUp(UehCellCb *cb)
{
  switch(getCellAvState(cb))
  {
    case UEH_CELL_STATE_UNAVAILABLE:
      /* nothing to do */
    break;
    case UEH_CELL_STATE_AVAILABLE:
      switch (cb->proc)
      {
        case UEH_CELL_PROC_NONE:
          /* nothing to do */
        break;
        case UEH_CELL_PROC_OP_STOP_ONGOING: 
          /* ignore - opstop continues */
        break;
        case UEH_CELL_PROC_S1_RESET_ONGOING: 
          /* let it continue */
        break;
        case UEH_CELL_PROC_S1_LINK_DOWN_ONGOING:
          crmAbortProc(cb); /* as that condition is no longer true */
        break;
        default:
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_S1_LINK_UP);
        break;
      } /* proc */
    break;
    default:
      CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_S1_LINK_UP);
    break;
  } /* state */
  cb->isS1LinkUp = TRUE;
}

void crmHandleS1LinkDown(UehCellCb *cb)
{
  switch(getCellAvState(cb))
  {
    case UEH_CELL_STATE_UNAVAILABLE:
      /* nothing to do */
    break;
    case UEH_CELL_STATE_AVAILABLE:
      switch (cb->proc)
      {
        case UEH_CELL_PROC_NONE:
          crmStartProc( cb, UEH_CELL_PROC_S1_LINK_DOWN_ONGOING );
          crmStartCallClearing( cb, EVENTTYPE_S1CLEAR, NULLP );
        break;
        case UEH_CELL_PROC_OP_STOP_ONGOING: 
          /* ignore - opstop continues */
        break;
        case UEH_CELL_PROC_S1_RESET_ONGOING: 
          /* Since link is down, reset will not complete. */
          crmAbortProc(cb);
        break;
        case UEH_CELL_PROC_S1_LINK_DOWN_ONGOING:
          /* duplicate - ignore */
        break;
        default:
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_S1_LINK_DOWN);
        break;
      } /* proc */  
    break;
    default:
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_S1_LINK_DOWN);
    break;
  } /* state */
  cb->isS1LinkUp = FALSE;
}

/* This should be done by S1 manager */
void crmHandleS1Reset(UehCellCb *cb, S1ResetReq  *reset)
{
  switch(getCellAvState(cb))
  {
    case UEH_CELL_STATE_UNAVAILABLE:
      /* nothing to do */
    break;
    case UEH_CELL_STATE_AVAILABLE:
      switch (cb->proc)
      {
        case UEH_CELL_PROC_NONE:
          if( ResetType_GlobalS1Reset == reset->resetType )
          {
            /* Global Reset received, So clear all Calls*/
            crmStartProc( cb, UEH_CELL_PROC_S1_RESET_ONGOING );
            crmStartCallClearing( cb, EVENTTYPE_S1CLEAR ,&reset->resetCause);
            /* end the proc when UE count becomes 0 */
          }
          else /* selected UEs */
          {
            UehS1ConCb   *s1ConCb = NULLP;
            U32           indx = 0;
            for( indx=0; indx < reset->u.ueAssocLogConnList.numOfS1UeAssocLogConnItems; indx++ )
            {
              s1ConCb = s1apGetS1apCbFrmEnbId(cb,(S1UeAssocLogConnItem *)&(reset->u.ueAssocLogConnList.s1ConnItem[indx]));
              if(NULLP != s1ConCb)
              {
                s1ConCb->s1Cause = reset->resetCause.causeVal;
                s1ConCb->s1CauseType = reset->resetCause.causeType;
                ueFsm( (s1ConCb->ueCb), EVENTTYPE_S1CLEAR, NULL );
              }
            }
          }

        break;
        case UEH_CELL_PROC_OP_STOP_ONGOING: 
          /* ignore - opstop continues */
        break;
        case UEH_CELL_PROC_S1_RESET_ONGOING: 
          /* duplicate - ignore */
        break;
        case UEH_CELL_PROC_S1_LINK_DOWN_ONGOING:
          /* Since link is down, reset will not complete. do not initiate */
        break;
        default:
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_S1_RESET);
        break;
      } /* proc */
    break;
    default:
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_S1_RESET);
    break;
  } /* state */
}

void  crmHandleOpstopExpiry(UehCellCb *cb)
{
  switch(getCellAvState(cb))
  {
    case UEH_CELL_STATE_UNAVAILABLE:
      switch (cb->proc)
      {
        case UEH_CELL_PROC_NONE:
          /* unexpected - coding error */
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_OP_STOP_TMR_EXPIRY);
        break;
        case UEH_CELL_PROC_OP_STOP_ONGOING: 
          crmStopProc(cb);
          sendCemMsgToBsm( EVT_CEM_BSM_OPSTOP_CMPL,0, NULL);
          deallocCellResources(cb);
        break;
        case UEH_CELL_PROC_S1_RESET_ONGOING: 
          /* unexpected - coding error - ignore */
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_OP_STOP_TMR_EXPIRY);
        break;
        case UEH_CELL_PROC_S1_LINK_DOWN_ONGOING:
        default:
          /* unexpected - coding error - ignore */
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_OP_STOP_TMR_EXPIRY);
        break;
      } /* proc */
    break;
    case UEH_CELL_STATE_AVAILABLE:
    default:
      /* unexpected */
      CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_OP_STOP_TMR_EXPIRY);
    break;
  } /* state */

}

void crmHandleUeCountZero(UehCellCb *cb)
{
  switch(getCellAvState(cb))
  {
    case UEH_CELL_STATE_UNAVAILABLE:
      switch (cb->proc)
      {
        case UEH_CELL_PROC_NONE:
          /* unexpected - coding error */
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_UE_CNT_ZERO);
        break;
        case UEH_CELL_PROC_OP_STOP_ONGOING:
          crmStopProc(cb);
          uehStopTmr(UE_TMR_OP_STOP_REQ, &cb->opStopTmr, NULLP, 0);
          sendCemMsgToBsm( EVT_CEM_BSM_OPSTOP_CMPL,0, NULLP);
          deallocCellResources(cb);
        break;
        case UEH_CELL_PROC_S1_RESET_ONGOING: 
          crmStopProc(cb);
        break;
        case UEH_CELL_PROC_S1_LINK_DOWN_ONGOING:
          crmStopProc(cb);
        break;
        default:
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_UE_CNT_ZERO);
        break;
      } /* proc */
    break;
    case UEH_CELL_STATE_AVAILABLE:
      switch (cb->proc)
      {
        case UEH_CELL_PROC_NONE:
          /* normal - ignore */
        break;
        case UEH_CELL_PROC_OP_STOP_ONGOING:
          /* coding error - should not be doing this proc in AV */
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_UE_CNT_ZERO);
        break;
        case UEH_CELL_PROC_S1_RESET_ONGOING:
          crmStopProc(cb);
        break;
        case UEH_CELL_PROC_S1_LINK_DOWN_ONGOING:
        default:
          /* coding error - should not doing this proc in AV */
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_UE_CNT_ZERO);
        break;
      } /* proc */
    break;
    default:
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_UE_CNT_ZERO);
    break;
  } /* state */
}

void crmHandleOpstart(UehCellCb *cb)
{
  OpStartFail opstartFail;
  char *restrict reason = (char *) &(opstartFail.failReason[0]);

  clearBytes(&opstartFail, sizeof(OpStartFail));
  opstartFail.errorNum = BS_ERR_MSG_RCVD_IN_UNEXPECTED_STATE;
  snprintf(reason, sizeof(opstartFail.failReason) -1, "ongoing proc=%d", cb->proc);

  switch(getCellAvState(cb))
  {
    case UEH_CELL_STATE_UNAVAILABLE:
      switch (cb->proc)
      {
        case UEH_CELL_PROC_NONE:
          crmProcOpStartInUnavail(cb, NULL);
        break;
        case UEH_CELL_PROC_OP_STOP_ONGOING:
        case UEH_CELL_PROC_S1_RESET_ONGOING: 
        case UEH_CELL_PROC_S1_LINK_DOWN_ONGOING:
          sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_FAIL, sizeof(OpStartFail), (U8*) &opstartFail);
        break;
        default:
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_OP_START);
        break;
      } /* proc */
    break;
    case UEH_CELL_STATE_AVAILABLE:
      switch (cb->proc)
      {
        case UEH_CELL_PROC_NONE:
          /* should not happen but let us inform */
          sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_SUCC, 0, NULLP);
          cb->isUnlocked = TRUE;
        break;
        case UEH_CELL_PROC_OP_STOP_ONGOING:
        case UEH_CELL_PROC_S1_RESET_ONGOING:
        case UEH_CELL_PROC_S1_LINK_DOWN_ONGOING:
        default:
          /* should not be doing this proc in AV */
          sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_FAIL, sizeof(OpStartFail), (U8*) &opstartFail);
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_OP_START);
        break;
      } /* proc */
    break;
    default:
      sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_FAIL, sizeof(OpStartFail), (U8*) &opstartFail);
      CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_OP_START);
    break;
  } /* state */
}

void crmHandlePaging(UehCellCb *cb, S1apPdu *pdu)
{
  S1PagingInfo  pagingInfo;

  /* Decode CCPU to PWAV structure */
   if (FALSE == s1apUnwrapS1PagingReqPdu(pdu , &pagingInfo)) { return; }

 
  switch(getCellAvState(cb))
  {
    case UEH_CELL_STATE_UNAVAILABLE:
      /* Discard */
    break;
    case UEH_CELL_STATE_AVAILABLE:
      switch (cb->proc)
      {
        case UEH_CELL_PROC_NONE: /* TODO: Should we allow if cell is barred? */
        case UEH_CELL_PROC_S1_RESET_ONGOING: /* TODO: should we allow in this state? */
          /* Store Paging Message in Pending Paging Record */
          uehFillAndAddPagingRecord(&pagingInfo); /* TODO: pass cb to it. */
        break;
        case UEH_CELL_PROC_OP_STOP_ONGOING:
        case UEH_CELL_PROC_S1_LINK_DOWN_ONGOING:
        default:
          /* coding error - should not be doing this proc in AV */
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_PAGING);
        break;
      } /* proc */
    break;
    default:
          CEM_ERROR_UNEXPECTED_EVENT(cb, CELL_EVT_PAGING);
    break;
  } /* state */
}



/*State machine per Cell*/
void cellFsm2 (UehCellCb *cb, UehCellEvent event, void *data)
{
  switch(event)
  {
    case CELL_EVT_OP_STOP:
      crmHandleOpstop(cb, (MsgBsmModOpStopReq *) data);
    break;
    case CELL_EVT_S1_LINK_DOWN:
      crmHandleS1LinkDown(cb);
    break;
    case CELL_EVT_S1_RESET:
      crmHandleS1Reset(cb, (S1ResetReq *) data);
    break;
    case CELL_EVT_OP_START:
      crmHandleOpstart(cb);
    break;
    case CELL_EVT_S1_LINK_UP:
      crmHandleS1LinkUp(cb);
    break;
    case CELL_EVT_OP_STOP_TMR_EXPIRY:
      crmHandleOpstopExpiry(cb);
    break;
    case CELL_EVT_UE_CNT_ZERO:
      crmHandleUeCountZero(cb);
    break;
    case CELL_EVT_PAGING:
      crmHandlePaging(cb, (S1apPdu *) data);
    break;
    default:
    break;
  }
}

#if 0
PUBLIC  CellFsm cellFsm[UEH_CELL_MAX_STATE][CELL_MAX_EVT] = {};
PUBLIC  CellFsm cellFsm[UEH_CELL_MAX_STATE][CELL_MAX_EVT] =
{
    /* UEH_CELL_STATE_UNAVAILABLE State*/
    {
      crmProcOpStopReqInUnavail,                     /*CELL_EVT_OP_STOP*/
      crmProcS1LinkDownInUnexpectedState,            /*CELL_EVT_S1_LINK_DOWN*/
      crmProcS1ResetInUnexpectedState,               /*CELL_EVT_S1_RESET*/
      crmProcOpStartInUnavail,                       /*CELL_EVT_OP_START*/
      crmNoOpr,                                      /*CELL_EVT_S1_LINK_UP*/
      crmNoOpr,                                      /*CELL_EVT_OP_STOP_TMR_EXPIRY*/
      crmNoOpr,                                      /*CELL_EVT_UE_CNT_ZERO*/
      crmNoOpr,                                      /*CELL_EVT_PAGING*/
    },
    /* UEH_CELL_STATE_AVAILABLE State*/
    {
      crmProcOpStopReqInAvail,                       /*CELL_EVT_OP_STOP*/
      crmProcS1LinkDownInAvail,                      /*CELL_EVT_S1_LINK_DOWN*/
      crmProcResetReqInAvail,                        /*CELL_EVT_S1_RESET*/
      crmProcOpStartInUnexpectedState,               /*CELL_EVT_OP_START*/
      crmNoOpr,                                      /*CELL_EVT_S1_LINK_UP*/
      crmNoOpr,                                      /*CELL_EVT_OP_STOP_TMR_EXPIRY*/
      crmNoOpr,                                      /*CELL_EVT_UE_CNT_ZERO*/
      crmProcPagingMsgInAvail,                       /*CELL_EVT_PAGING*/
    },
    /* UEH_CELL_STATE_OP_STOP_ONGOING State*/
    {
      crmProcOpstopInUnexpectedState,                /*CELL_EVT_OP_STOP*/
      crmProcS1LinkdownInOpStop,                     /*CELL_EVT_S1_LINK_DOWN*/
      crmProcResetReqInOpStop,                       /*CELL_EVT_S1_RESET*/
      crmProcOpStartInOpStop,                        /*CELL_EVT_OP_START*/
      crmNoOpr,                                      /*CELL_EVT_S1_LINK_UP*/
      crmProcOpStopTmrExpiryInOpStop,                /*CELL_EVT_OP_STOP_TMR_EXPIRY*/
      crmProcZeroActCallInOpStop,                    /*CELL_EVT_UE_CNT_ZERO*/
      crmNoOpr,                                      /*CELL_EVT_PAGING*/
    },
    /* UEH_CELL_STATE_S1_LINK_DOWN_ONGOING State*/
    {
      crmProcOpStopInS1LinkDown,                     /*CELL_EVT_OP_STOP*/
      crmProcS1LinkDownInUnexpectedState,            /*CELL_EVT_S1_LINK_DOWN*/
      crmProcS1ResetInUnexpectedState,               /*CELL_EVT_S1_RESET*/
      crmProcOpStartInUnexpectedState,               /*CELL_EVT_OP_START*/
      crmNoOpr,                                      /*CELL_EVT_S1_LINK_UP*/
      crmNoOpr,                                      /*CELL_EVT_OP_STOP_TMR_EXPIRY*/
      crmProcZeroActCallInS1LinkDown,                /*CELL_EVT_UE_CNT_ZERO*/
      crmNoOpr,                                      /*CELL_EVT_PAGING*/
    },
    /* UEH_CELL_STATE_S1_RESET_ONGOING State*/
    {
      crmProcOpStopInS1Reset,                        /*CELL_EVT_OP_STOP*/
      crmProcS1LinkDownInS1Reset,                    /*CELL_EVT_S1_LINK_DOWN*/
      crmProcResetReqInReset,                        /*CELL_EVT_S1_RESET*/
      crmProcOpStartInUnexpectedState,               /*CELL_EVT_OP_START*/
      crmNoOpr,                                      /*CELL_EVT_S1_LINK_UP*/
      crmNoOpr,                                      /*CELL_EVT_OP_STOP_TMR_EXPIRY*/
      crmProcZeroActCallInReset,                     /*CELL_EVT_UE_CNT_ZERO*/
      crmNoOpr,                                      /*CELL_EVT_PAGING*/
    },
    //BS-1799
    /*Added to handle cell barring */
    /* UEH_CELL_STATE_BARRED state*/
    {
      crmProcOpStopInCellBarred,                     /*CELL_EVT_OP_STOP*/
      crmProcS1LinkDownInCellBarred,                 /*CELL_EVT_S1_LINK_DOWN*/
      crmProcResetReqInCellBarred,                   /*CELL_EVT_S1_RESET*/
      crmProcOpStartInUnavail,                       /*CELL_EVT_OP_START*/ /* BS-2088 Added Handling when Cell is BARRED*/
      crmNoOpr,                                      /*CELL_EVT_S1_LINK_UP*/
      crmNoOpr,                                      /*CELL_EVT_OP_STOP_TMR_EXPIRY*/
      crmNoOpr,                                      /*CELL_EVT_UE_CNT_ZERO*/
      crmProcPagingMsgInCellBArred,                  /*CELL_EVT_PAGING*/
     }

};

void crmNoOpr(
    UehCellCb   *cellCb,
    void        *pdu
  )
{
   logInfo("No operation hit in state:%d ", cellCb->state);

   /* PDU Will be freed by the Caller of StateMachine.*/
}

void crmProcOpStartInUnexpectedState(
    UehCellCb   *cellCb,
    void        *pdu
  )
{
   logError("CELL_EVT_OP_START event Received in Unexpected state:%d ", cellCb->state);

   /* PDU Will be freed by the Caller of StateMachine.*/
}

void crmProcS1ResetInUnexpectedState(
    UehCellCb   *cellCb,
    void        *pdu
  )
{
   logError("CELL_EVT_S1_RESET event Received in Unexpected state:%d ", cellCb->state);

   /* PDU Will be freed by the Caller of StateMachine.*/
}

void crmProcS1LinkDownInUnexpectedState(
    UehCellCb   *cellCb,
    void        *pdu
  )
{
   logError("CELL_EVT_S1_LINK_DOWN event Received in Unexpected state:%d ", cellCb->state);

   /* PDU Will be freed by the Caller of StateMachine.*/
}

void crmProcOpstopInUnexpectedState(
    UehCellCb   *cellCb,
    void        *pdu
  )
{
   logError("CELL_EVT_OP_STOP event Received in Unexpected state:%d ", cellCb->state);

   /* PDU Will be freed by the Caller of StateMachine.*/
}
/*
 * Handler Functions in UEH_CELL_STATE_UNAVAILABLE state
 */
/**
 * @details Make cell-state = AVAILABLE
 **/
void crmProcOpStartInUnavail(
    UehCellCb   *cellCb,
    void        *null
  )
{
  OpStartFail opstartFail;
  clearBytes(&opstartFail, sizeof(OpStartFail));

  UECM_EVENTHDLR(-1,crmProcOpStartInUnavail,"OPSTART in AVBL state");
  logInfo("%s", "OPSTART in AVBL state");

  /* Initialize SRS Table */
  if(-1 == uehCrmInitializeSfSrsTables())
  {
    opstartFail.errorNum = BS_ERR_INTERNAL_ERROR_OCCURED;
    sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_FAIL, sizeof(OpStartFail), (U8*) &opstartFail);
    return;
  }

  /* Initialize PUCCH Resource Management */
  if(-1 == uehCrmInitializePucchResTables())
  {
    opstartFail.errorNum = BS_ERR_INTERNAL_ERROR_OCCURED;
    sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_FAIL, sizeof(OpStartFail), (U8*) &opstartFail);
    return;
  }

  /* Create the tunnel Id Allocation List */
  allocInitTeidPool();

  /* System Information Scheduling parameter handling */
  if(RFAILED == uehInitSysInfSch())
  {
    opstartFail.errorNum = BS_ERR_INTERNAL_ERROR_OCCURED;
    sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_FAIL, sizeof(OpStartFail), (U8*) &opstartFail);
    return;
  }

  /* Initialise pending paging List */
  if(RFAILED == uehInitPendingPagingList())
  {
    opstartFail.errorNum = BS_ERR_INTERNAL_ERROR_OCCURED;
    sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_FAIL, sizeof(OpStartFail), (U8*) &opstartFail);
    return;
  }

  //psuedophyTtiFlag = 1;
  /*BS-1799 Changes*/
  if (CELLBARRED_BARRED EQ uehCb.uehCfgCb.cellBarred)
  {
    cellCb->isUnbarred = FALSE;
    //crmUpdateCellState(UEH_CELL_STATE_BARRED);
  }
  else 
  {
    cellCb->isUnbarred = TRUE;
    //crmUpdateCellState(UEH_CELL_STATE_AVAILABLE);
  }
  sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_SUCC, 0, NULL);
  cb->isUnlocked = TRUE;
}

/*
 * Handler Functions in UEH_CELL_STATE_AVAILABLE state
 */
/**
 * @details We stop the calls under three scenarios,
 * When OP-STOP command is received from BSM,
 * When S1LinkDown is received from S1AP Manager,
 * When Reset is received from MME
 **/
void crmStartCallClearing(
    UehCellCb   *cellCb,
    EventType    event,
    S1Cause      *s1Cause
    )
{
  UehS1ConCb   *s1ConCb = NULLP;
  UehUeCb      *ueCb=NULLP;
  U32          indx = 0;

  /* If the number of Active calls in the system are ZERO
   * There is no point in starting Call Clearing, Send UE Count as ZERO.
   */
  if( 0 == cellCb->numUeCfg )
  {
    crmInvokeCellSm( cellCb, CELL_EVT_UE_CNT_ZERO, NULL );
    return ;
  }

  /* Clear the calls whose RRC Connection is Established.
   */

  for (indx = 0; indx < cellCb->maxRntiPool; indx++)
  {
    UEH_GET_S1CONCB(s1ConCb,indx);
    if ((NULLP !=  s1ConCb) && (NULLP != s1ConCb->ueCb))
    {
      if (NULLP != s1Cause)
      {
        s1ConCb->s1Cause = s1Cause->causeVal;
        s1ConCb->s1CauseType = s1Cause->causeType;
      }
    }
    UEH_GET_UECB_FROM_UEIDX(ueCb, indx);
    if(NULLP != ueCb && ueCb->isUeCbAllocated)
    {
      ueFsm(ueCb, event, NULLP);
    }
  }
}

/**
 * @details We stop the calls under three scenarios,
 * When Reset is received from MME with UE Associated Logical Signalling,
 * We delete only those UE's
 **/
void crmStartCallClearingForReset(
    UehCellCb   *cellCb,
    EventType    event,
    S1ResetReq  *reset
    )
{
  UehS1ConCb   *s1ConCb = NULLP;
  U32          indx = 0;

  /* Only Certain UE's are Reset */
  for( indx=0; indx < reset->u.ueAssocLogConnList.numOfS1UeAssocLogConnItems; indx++ )
  {
    s1ConCb = s1apGetS1apCbFrmEnbId(cellCb,(S1UeAssocLogConnItem *)&(reset->u.ueAssocLogConnList.s1ConnItem[indx]));
    if(NULLP != s1ConCb)
    {
      s1ConCb->s1Cause = reset->resetCause.causeVal;
      s1ConCb->s1CauseType = reset->resetCause.causeType;
      ueFsm( (s1ConCb->ueCb), event, NULL );
    }
  }
}

/**
 * @details Trigger call clearing, cell-State= S1FAIL_ONGOING
 **/
void crmProcS1LinkDownInAvail(
    UehCellCb   *cellCb,
    void        *null
  )
{
  UECM_EVENTHDLR(-1,crmProcS1LinkDownInAvail,"S1 LINK Donw in AVBL state");
  logInfo("%s", "S1 LINK Donw in AVBL state");

  crmUpdateCellState( UEH_CELL_STATE_S1_LINK_DOWN_ONGOING );
  crmStartCallClearing( cellCb, EVENTTYPE_S1CLEAR,NULLP );
}

/**
 * @details Send ACK, Trigger call clearing, cell-State= S1RESET_ONGOING
 **/
void crmProcResetReqInAvail(
    UehCellCb   *cellCb,
    void        *pdu
  )
{
  S1ResetReq      *reset = (S1ResetReq*)pdu;

  UECM_EVENTHDLR(-1,crmProcResetReqInAvail,"S1 Reset in AVBL state");
  logInfo("%s", "S1 Reset in AVBL state");

  crmStartCallClearingForReset( cellCb, EVENTTYPE_S1CLEAR, reset );
}


/**
 * @details Paging Message Processing in S1AP
 **/
void crmProcPagingMsgInAvail(
    UehCellCb   *cellCb,
    void        *pdu
  )
{
  S1apPdu      *s1apPagingPdu = (S1apPdu*)pdu;
  S1PagingInfo  pagingInfo;
  TBOOL         isValidPagingMsg;

  UECM_EVENTHDLR(-1,crmProcPagingMsgInAvail,"Paging Message in AVBL state");
  logInfo("%s", "Paging Message in AVBL state");

  /* Decode CCPU to PWAV structure */
   isValidPagingMsg = s1apUnwrapS1PagingReqPdu(s1apPagingPdu , &pagingInfo);
   if(isValidPagingMsg EQ FALSE)
   {
     return;
   }

  /* Store Paging Message in Pending Paging Record */
  uehFillAndAddPagingRecord(&pagingInfo);
  logInfo("%s", "Filled Pending Paging Record");
}

/**
 * @details Start opStopAckTimer, Trigger call clearing, cell-State= OPSTOP_ONGOING
 **/
void crmProcOpStopReqInAvail(
    UehCellCb   *cellCb,
    void        *null
  )
{
  UECM_EVENTHDLR(-1,crmProcOpStopReqInAvail,"OP STOP in AVBL state");
  logInfo("%s", "OP STOP in AVBL state");

  uehStartTmr (UE_TMR_OP_STOP_REQ,&((UehCellCb *)cellCb)->opStopTmr,(PTR)cellCb,0);
  crmUpdateCellState( UEH_CELL_STATE_OP_STOP_ONGOING );

  /* Report the OAM Intervention to MME by sending S1 Global Reset
   * Start Local Call Clearing*/
  s1apSndResetToPeerOamInt();
  crmStartCallClearing( cellCb, EVENTTYPE_OPSTOPRELEASE,NULLP);

  /* Clear Pending Paging messages */
  uehFreePendingPagingList();
}

/**
 * @details Start opStopAckTimer, Trigger call clearing, cell-State= UNAVBL
 **/
void crmProcOpStopReqInUnavail(
    UehCellCb   *cellCb,
    void        *null
  )
{
  UECM_EVENTHDLR(-1,crmProcOpStopReqInUnavail,"OP STOP in UNAVBL state");
  logInfo("%s", "OP STOP in UNAVBL state");
  /* Send OP-STOP SUCC */
  sendCemMsgToBsm(EVT_CEM_BSM_OPSTOP_CMPL,0, NULL);
}
/*
 * Handler Functions in UEH_CELL_STATE_OP_STOP_ONGOING state
 */
/**
 * @details Just stop sending S1 messages to MME (if feasible), continue the ongoing processing for OPSTOP,
 * no new call clearing needed State should be retained as OPSTOP_ONGOING as we need to supervise the procedure
 * and send ACK to BSM. In current design this is not possible as Context Release request is sent immediately
 * (in loop) on getting OP_STOP.
 **/
void crmProcS1LinkdownInOpStop(
    UehCellCb   *cellCb,
    void        *null
  )
{

  UECM_EVENTHDLR(-1,crmProcS1LinkDownInAvail,"S1 LINK DN during OPSTOP ");
  logError("%s", "S1 LINK DN during OPSTOP");
  /* TODO Add code to stop S1 Interface */
}

/**
 * @details Send ACK, no new call clearing needed, no state change either
 **/
void crmProcResetReqInOpStop(
    UehCellCb   *cellCb,
    void        *pdu
  )
{
  UECM_EVENTHDLR(-1,ccrmProcResetReqInOpStop,"RESET REQ during OPSTOP");
  logError("%s", "RESET REQ during OPSTOP");

}

/**
 * @details Sends NACK, no change in state and processing
 *          This should not happen in BSM anyway
 **/
void crmProcOpStartInOpStop(
    UehCellCb   *cellCb,
    void        *null
  )
{
  OpStartFail opstartFail;
  clearBytes(&opstartFail, sizeof(OpStartFail));

  UECM_EVENTHDLR(-1,crmProcOpStartInOpStop,"OPSTART during OPSTOP");
  logError("%s", "OPSTART during OPSTOP");

  UEH_ERROR_RECEIVED_FAILURE_EVENT( EVT_BSM_CEM_OPSTART, ENT_BSM, NOT_APPLICABLE, CELL, cellCb->cellId);
  opstartFail.errorNum = BS_ERR_MSG_RCVD_IN_UNEXPECTED_STATE;
  sendCemMsgToBsm(EVT_CEM_BSM_OPSTART_FAIL, sizeof(OpStartFail), (U8*) &opstartFail);
}

/**
 * @details Send ACK to BSM, cell-state = UNAVAILBALE   The timer is just to give enough time to
 *          UECM to release calls, UECM also uses its own timer to do implicit release, so no need for
 *          handle implicit handing
 **/
void crmProcOpStopTmrExpiryInOpStop(
    UehCellCb   *cellCb,
    void        *null
  )
{
  UECM_EVENTHDLR(-1,crmProcOpStopTmrExpiryInOpStop,"OPSTOP Timer expiry");

  logTimeout("OPSTOP|numUeCfg=%d|cellId=%lu", cellCb->numUeCfg, cellCb->cellId);
  sendCemMsgToBsm(EVT_CEM_BSM_OPSTOP_CMPL,0, NULL);
  crmUpdateCellState( UEH_CELL_STATE_UNAVAILABLE );
}

/**
 * @details Send ACK to BSM, cell-state = UNAVAILBALE, stop opStopAckTimer
 *          See above rationale.
 **/
void crmProcZeroActCallInOpStop(
    UehCellCb   *cellCb,
    void        *null
  )
{
  UECM_EVENTHDLR(-1,crmProcZeroActCallInOpStop,"All calls cleared during OPSTOP");
  logInfo("%s", "All calls cleared during OPSTOP");

  uehStopTmr(UE_TMR_OP_STOP_REQ,&((UehCellCb *)cellCb)->opStopTmr,(PTR)(cellCb),0);
  sendCemMsgToBsm(EVT_CEM_BSM_OPSTOP_CMPL,0, NULL);
  crmUpdateCellState( UEH_CELL_STATE_UNAVAILABLE );

  /* Clear the Resources */
  uehCrmDeinitializeSfSrsTables();
  uehCrmDeinitializePucchResTables();
  deallocTeidPool();
}

/*
 * Handler Functions in UEH_CELL_STATE_S1_LINK_DOWN_ONGOING state
 */
/**
 * @details Change state to OPSTOP_ONGOING, no new call clearing but ACK needs to be sent to BSM after
 *          calls cleared.   OPSTOP is operator trigger hence it needs ACK
 **/
void crmProcOpStopInS1LinkDown(
    UehCellCb   *cellCb,
    void        *null
  )
{
  UECM_EVENTHDLR(-1,crmProcOpStopInS1LinkDown,"OPSTOP during S1 link down");
  logInfo("%s", "OPSTOP during S1 link down");

  uehStartTmr (UE_TMR_OP_STOP_REQ,&((UehCellCb *)cellCb)->opStopTmr,(PTR)cellCb,0);
  crmUpdateCellState( UEH_CELL_STATE_OP_STOP_ONGOING );

  /* Clear Pending Paging messages */
  uehFreePendingPagingList();
}

/**
 * @details Set cell-state = UNAVAILBALE
 **/
void crmProcZeroActCallInS1LinkDown(
    UehCellCb   *cellCb,
    void        *null
  )
{
  UECM_EVENTHDLR(-1,crmProcZeroActCallInS1LinkDown,"All calls cleared during S1 link down");
  logInfo("%s", "All calls cleared during S1 link down");

  crmUpdateCellState( UEH_CELL_STATE_UNAVAILABLE );
}

/*
 * Handler Functions in UEH_CELL_STATE_S1_RESET_ONGOING state
 */
/**
 * @details Change state to OPSTOP_ONGOING, no new call clearing but ACK needs to be sent to BSM
 *          after calls cleared.   OPSTOP is operator trigger hence it needs ACK
 **/
void crmProcOpStopInS1Reset(
    UehCellCb   *cellCb,
    void        *null
  )
{
  UECM_EVENTHDLR(-1,crmProcOpStopInS1Reset,"OPSTOP during S1 RESET");
  logInfo("%s", "OPSTOP during S1 RESET");

  uehStartTmr (UE_TMR_OP_STOP_REQ, &((UehCellCb *)cellCb)->opStopTmr,(PTR)cellCb,0);
  crmUpdateCellState( UEH_CELL_STATE_OP_STOP_ONGOING );

  /* Clear Pending Paging messages */
  uehFreePendingPagingList();
}

/**
 * @details Current actions shall continue, Change cell-state to S1FAIL_ONGOING
 *          Cell-state is changed because S1 failure has happened, and so at the end of call clearing,
 *          the cell-state should be changed to UNAVAILABLE.
 **/
void crmProcS1LinkDownInS1Reset(
    UehCellCb   *cellCb,
    void        *null
  )
{
  UECM_EVENTHDLR(-1,crmProcS1LinkDownInS1Reset,"S1 link down during S1 RESET");
  logInfo("%s", "S1 link down during S1 RESET");

  crmUpdateCellState( UEH_CELL_STATE_S1_LINK_DOWN_ONGOING );
}

/**
 * @details Change cell state to AVAILABLE
 **/
void crmProcZeroActCallInReset(
    UehCellCb   *cellCb,
    void        *null
  )
{
  UECM_EVENTHDLR(-1,crmProcZeroActCallInReset,"All calls cleared during S1 RESET");
  logInfo("%s", "All calls cleared during S1 RESET");

  crmUpdateCellState( UEH_CELL_STATE_AVAILABLE );
}

/**
 * @details Send ACK
 **/
void crmProcResetReqInReset(
    UehCellCb   *cellCb,
    void        *pdu
  )
{
  UECM_EVENTHDLR(-1,crmProcResetReqInReset,"S1 RESET REQ during ongoing RESET");
  logInfo("%s", "S1 RESET REQ during ongoing RESET");
}
/*BS-1799 cell barring*/
void crmProcOpStopInCellBarred(
    UehCellCb   *cellCb,
    void        *null
  )
{
  UECM_EVENTHDLR(-1,crmProcOpStopInCellBarred,"OPSTOP during Cell Barred");
  logInfo("%s", "OPSTOP during cell Barred");

  uehStartTmr (UE_TMR_OP_STOP_REQ,&((UehCellCb *)cellCb)->opStopTmr,(PTR)cellCb,0);
  crmUpdateCellState( UEH_CELL_STATE_OP_STOP_ONGOING );

  /* Clear Pending Paging messages */
  uehFreePendingPagingList();
}

void crmProcS1LinkDownInCellBarred(
    UehCellCb   *cellCb,
    void        *null
  )
{


  UECM_EVENTHDLR(-1,crmProcS1LinkDownInCellBarred,"S1 link down in cell Barred");
  logInfo("%s", "S1 link down in cell barred");

  crmUpdateCellState( UEH_CELL_STATE_S1_LINK_DOWN_ONGOING );
  crmStartCallClearing( cellCb, EVENTTYPE_S1CLEAR, NULLP );
}

void crmProcResetReqInCellBarred(
    UehCellCb   *cellCb,
    void        *pdu
  )
{
  S1ResetReq      *reset = (S1ResetReq*)pdu;

  UECM_EVENTHDLR(-1,crmProcResetReqInCellBarred,"S1 Reset in cell barred state");
  logInfo("%s", "S1 Reset in cell barred state");

  crmUpdateCellState( UEH_CELL_STATE_S1_RESET_ONGOING );
  crmStartCallClearingForReset( cellCb, EVENTTYPE_S1CLEAR, reset );
}
/**
 * @details Paging Message Processing in S1AP
 **/
void crmProcPagingMsgInCellBArred(
    UehCellCb   *cellCb,
    void        *pdu
  )
{
  S1apPdu      *s1apPagingPdu = (S1apPdu*)pdu;
  S1PagingInfo  pagingInfo;
  TBOOL         isValidPagingMsg;

  UECM_EVENTHDLR(-1,crmProcPagingMsgInCellBArred,"Paging Message in cell barred state");
  logInfo("%s", "Paging Message in cell barred state");

  /* Decode CCPU to PWAV structure */
   isValidPagingMsg = s1apUnwrapS1PagingReqPdu(s1apPagingPdu , &pagingInfo);
   if(isValidPagingMsg EQ FALSE)
   {
     return;
   }

  /* Store Paging Message in Pending Paging Record */
  uehFillAndAddPagingRecord(&pagingInfo);
  logInfo("%s", "Filled Pending Paging Record");
}
#endif

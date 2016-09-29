/**
 * @file    s1ap_sm.c
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @details
 * This file contains S1AP Manager State Machine functions
 *
 * @author Lakshmi Narayana Madala
 *
 */

#include "ccpu_cmn.h"

/* RRM related includes */
#include "ueh.h"
#include "lueh.h"
#include "lueh.x"
#include "ueh_eut_x.h"
#include "s1ap.h"
#include "s1ap_cm.h"
#include "s1ap_wrap.h"
#include "s1ap_unwrap.h"
#include "s1ap_cmn_proc.h"
#include "s1ap_mgr.h"
#include "s1ap_sm.h"
#include "s1ap_err.h"
#include "bs_err.h"
#include "ueh_crm_sm.h"
#include "szt.x"
#include "log.h"
#include "pst_events.h"
#include "enumstr.h"
/*State machine per MME*/

PUBLIC U8 s1apFsm(
  IO S1apMgrCb     *Cb,
  IN S1apMgrEvent   s1apEvent,
  IN SztUDatEvnt   *uDatEvnt
)
{
  S16 ret;
  S1apMgrState      prevState = Cb->s1State;
  switch(s1apEvent)
  {
    case S1_MSG_OPSTART:                 ret = s1apHndlOpStart(Cb,s1apEvent); break;
    case S1_MSG_OPSTOP:                  ret = s1apHndlOpStop(Cb,s1apEvent); break;
    case S1_MSG_ID_RESET:                ret = s1apHndlReset(Cb,s1apEvent,uDatEvnt); break;  
    case S1_MSG_ID_RESET_ACK:            ret = s1apHndlResetAck(Cb,s1apEvent,uDatEvnt); break;   
    case S1_MSG_ID_SETUP_RSP:            ret = s1apHndlSetupRsp(Cb,s1apEvent,uDatEvnt); break; 
    case S1_MSG_ID_SETUP_FAIL:           ret = s1apHndlSetupFail(Cb,s1apEvent,uDatEvnt); break; 
    case S1_MSG_ID_PAGING:               ret = s1apHndlPaging(Cb,s1apEvent,uDatEvnt); break;
    case S1_MSG_PROC_GUARD_TMR_EXPIRY:   ret = s1apHndlGuardTmrExpiry(Cb,s1apEvent); break;
    case S1_MSG_SETUP_TMR_EXPIRY:        ret = s1apHndlSetupTmrExpiry(Cb,s1apEvent); break; 
    case S1_MSG_LINK_DOWN:               ret = s1apHndlLinkDown(Cb,s1apEvent);break; 
    case S1_MSG_LINK_UP:                 ret = s1apHndlLinkUp(Cb,s1apEvent);break; 
    case S1_MSG_MME_CFG_UPDATE:          ret = s1apHndlMmeCfgUpdate(Cb,s1apEvent,uDatEvnt); break; 
    case S1_MSG_OVERLOAD_START:          ret = s1apHndlOverloadStart(Cb,s1apEvent,uDatEvnt); break; 
    case S1_MSG_OVERLOAD_STOP:           ret = s1apHndlOverloadStop(Cb,s1apEvent); break; 
    case S1_MSG_SND_RESET:               ret = s1apHndlSndReset(Cb,s1apEvent);break; 
    case S1_MSG_WARNING_REQ:             ret = s1apHndlWarningReq(Cb,s1apEvent,uDatEvnt);break; 
    case S1_MSG_ABORT_CFM:               ret = s1apHndlAbortCfm(Cb,s1apEvent);break; 
    default:
      logError("Invalid Event Received :%d", s1apEvent);
      ret = RFAILED;
  }
  logStateChange("S1AP|st=%s|evt=%s (%d)|newst=%s", 
                 s1apGetUeStateName(prevState),
                 s1apGetUeEventName(s1apEvent),
                 s1apEvent,
                 s1apGetUeStateName(Cb->s1State));
  RETVALUE(ret);
}



/**
 * @param   s1Cb   S1 Call block
 * @details
 * S1_MSG_ID_OPSTOP message handled based on S1AP State 
 */
U8 s1apHndlOpStart(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_INIT:  
      s1apProcOpStartInInit(s1Cb); 
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * S1_MSG_ID_OPSTOP message handled based on S1AP State 
 */
U8 s1apHndlOpStop(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_INIT:
      s1apProcOpStopInInit(s1Cb);
      RETVALUE(ROK);
    case S1_STATE_SETUP_IN_PROG:   
      s1apProcOpStopInSetupInProg(s1Cb); 
      RETVALUE(ROK);
    case S1_STATE_SETUP_DONE:      
      s1apProcOpStopInSetupDone(s1Cb);
      RETVALUE(ROK);
    case S1_STATE_RESET_IN_PROG:
      s1apProcOpStopInResetInProg(s1Cb);  
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    Reset request from MME
 * @details
 * S1_MSG_ID_RESET message handled based on S1AP State 
 */
U8 s1apHndlReset(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_SETUP_DONE:
      s1apProcResetInSetupDone(s1Cb,uDatEvnt);
      RETVALUE(ROK);
    case S1_STATE_RESET_IN_PROG:
      s1apProcResetInResetInProg(s1Cb,uDatEvnt);  
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    Reset Ack PDU from MME
 * @details
 * S1_MSG_ID_RESET_ACK message handled based on S1AP State 
 */
U8 s1apHndlResetAck(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_RESET_IN_PROG:
      s1apProcResetAckInResetInProg(s1Cb,uDatEvnt);  
      RETVALUE(ROK);
    case S1_STATE_INIT :
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}


/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    Setup Response
 * @details
 * S1_MSG_ID_SETUP_RSP message handled based on S1AP State 
 */
U8 s1apHndlSetupRsp(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_SETUP_IN_PROG:
      s1apProcSetupRspInSetupInProg(s1Cb,uDatEvnt); 
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}



/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    S1 Setup Fail received from MME
 * @details
 * S1_MSG_ID_SETUP_FAIL message handled based on S1AP State 
 */
U8 s1apHndlSetupFail(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_SETUP_IN_PROG:
      s1apProcSetupFailInSetupInProg(s1Cb,uDatEvnt); 
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    Paging PDU from MME
 * @details
 * S1_MSG_ID_PAGING message handled based on S1AP State 
 */
U8 s1apHndlPaging(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_SETUP_DONE:
      s1apProcPagingInSetupDone(s1Cb,uDatEvnt);
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * S1_PROC_GUARD_TMR_EXPIRY message handled based on S1AP State 
 */
U8 s1apHndlGuardTmrExpiry(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_SETUP_IN_PROG:
      s1apProcGuardTmrExpiryInSetupInProg(s1Cb); 
      RETVALUE(ROK);
    case S1_STATE_RESET_IN_PROG:
      s1apProcGuardTmrExpiryInResetInProg(s1Cb);  
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * S1_SETUP_TMR_EXPIRY message handled based on S1AP State 
 */
U8 s1apHndlSetupTmrExpiry(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_SETUP_IN_PROG:
      s1apProcSetupTmrExpiryInSetupInProg(s1Cb); 
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * S1_MSG_ID_LINK_DOWN message handled based on S1AP State 
 */
U8 s1apHndlLinkDown(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_SETUP_IN_PROG:
      s1apProcLinkDownInSetupInProg(s1Cb); 
      RETVALUE(ROK);
    case S1_STATE_SETUP_DONE:
      s1apProcLinkDownInSetupDone(s1Cb);
      RETVALUE(ROK);
    case S1_STATE_RESET_IN_PROG:
      s1apProcLinkDownInResetInProg(s1Cb);  
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * S1_MSG_ID_LINK_UP message handled based on S1AP State 
 */
U8 s1apHndlLinkUp(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_SETUP_IN_PROG:
      s1apProcLinkUpInSetupInProg(s1Cb); 
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    MME Config Update
 * @details
 * S1_MSG_ID_MME_CFG_UPDATE message handled based on S1AP State 
 */
U8 s1apHndlMmeCfgUpdate(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_SETUP_DONE:
      s1apProcMmeCfgUpdateInSetupDone(s1Cb,uDatEvnt);
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    MME Overload
 * @details
 * S1_MSG_ID_OVERLOAD_START message handled based on S1AP State 
 */
U8 s1apHndlOverloadStart(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_SETUP_DONE: 
      s1apProcOverloadStartInSetupDone(s1Cb,uDatEvnt);
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    MME Overload Stop
 * @details
 * S1_MSG_ID_OVERLOAD_STOP message handled based on S1AP State 
 */
U8 s1apHndlOverloadStop(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_SETUP_DONE:
      s1apProcOverloadStopInSetupDone(s1Cb);
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * S1_MSG_ID_SND_RESET message handled based on S1AP State 
 */
U8 s1apHndlSndReset(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_SETUP_DONE:
      s1apProcSndResetInSetupDone(s1Cb);
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    MME Warning Request
 * @details
 * S1_MSG_ID_WARNING_REQ message handled based on S1AP State 
 */
U8 s1apHndlWarningReq(
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_INIT:            
    case S1_STATE_SETUP_IN_PROG:    
    case S1_STATE_SETUP_DONE:      
    case S1_STATE_RESET_IN_PROG:
      s1apProcWarningReqInAllStates(s1Cb,uDatEvnt);  
      RETVALUE(ROK);
    default: 
      S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
      RETVALUE(RFAILED);
  }
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    S1ap Abort Conform
 * @details
 * S1_MSG_ABORT_CFM message handled based on S1AP State 
 */
U8 s1apHndlAbortCfm (
    IO S1apMgrCb   *s1Cb,
    IN U8          s1apEvent
    )
{
  switch(s1Cb->s1State)
  {
    case S1_STATE_INIT:            
    case S1_STATE_SETUP_IN_PROG:    
    case S1_STATE_SETUP_DONE:      
    case S1_STATE_RESET_IN_PROG:
       s1apProcAbortCfmInAllStates(s1Cb); 
       RETVALUE(ROK);
    default:
       S1AP_ERROR_UNEXPECTED_EVENT(s1Cb,s1apEvent);
       RETVALUE(RFAILED);
  }
}
/**
 *       Fun:   s1apGetUeStateName
 *       Desc:  Returns State Name in string format from Enum Value
 *       Ret:   char
 *       Notes: None
 */
char *s1apGetUeEventName(
  S1apMgrEvent s1apEvent
  )
{
  switch( s1apEvent )
  {
    case S1_MSG_OPSTART:                 return "S1_MSG_OPSTART";
    case S1_MSG_OPSTOP:                  return "S1_MSG_OPSTOP";
    case S1_MSG_ID_RESET:                return "S1_MSG_ID_RESET";  
    case S1_MSG_ID_RESET_ACK:            return "S1_MSG_ID_RESET_ACK";   
    case S1_MSG_ID_SETUP_RSP:            return "S1_MSG_ID_SETUP_RSP"; 
    case S1_MSG_ID_SETUP_FAIL:           return "S1_MSG_ID_SETUP_FAIL"; 
    case S1_MSG_ID_PAGING:               return "S1_MSG_ID_PAGING"; 
    case S1_MSG_PROC_GUARD_TMR_EXPIRY:   return "S1_MSG_PROC_GUARD_TMR_EXPIRY"; 
    case S1_MSG_SETUP_TMR_EXPIRY:        return "S1_MSG_SETUP_TMR_EXPIRY"; 
    case S1_MSG_LINK_DOWN:               return "S1_MSG_LINK_DOWN";
    case S1_MSG_LINK_UP:                 return "S1_MSG_LINK_UP";
    case S1_MSG_MME_CFG_UPDATE:          return "S1_MSG_MME_CFG_UPDATE"; 
    case S1_MSG_OVERLOAD_START:          return "S1_MSG_OVERLOAD_START"; 
    case S1_MSG_OVERLOAD_STOP:           return "S1_MSG_OVERLOAD_STOP"; 
    case S1_MSG_SND_RESET:               return "S1_MSG_SND_RESET";
    case S1_MSG_WARNING_REQ:             return "S1_MSG_WARNING_REQ";
    default:                             return "S1-MSG-NAME-INVALID";
  }
}

/**
 *       Fun:   s1apGetUeStateName
 *       Desc:  Returns State Name in string format from Enum Value
 *       Ret:   char
 *       Notes: None
 */
char *s1apGetUeStateName(
  S1apMgrState s1apState
  )
{
  switch( s1apState )
  {
    case S1_STATE_INIT:             return "S1_INIT";
    case S1_STATE_SETUP_IN_PROG:    return "S1_SETUP_IN_PROG";
    case S1_STATE_SETUP_DONE:       return "S1_SETUP_DONE";
    case S1_STATE_RESET_IN_PROG:    return "S1_RESET_IN_PROG";
    default:                        return "S1_STATE_UNKNOWN";
  }
}

/************************************ Filling Routines ********************************* */
/**
 * @param    setupReq   SetupRequest to be sent to MME
 * @param    s1apConfig S1AP Configuartion received from BSM
 * @details
 * We are filling the S1setup Request from From The global structure.
 * As of today Mutiliple TAC are not supported.
 * No CSGList is supported
 **/
void s1apFillSetupReq(
    IO S1SetupReq *setupReq,
    IN S1apMgrCb  *s1apConfig
    )
{
  UINT16 i;

  setupReq->globalEnodeBId      = s1apConfig->globalEnodeBId;
  setupReq->isEnodeBNamePresent = TRUE;
  /* warning ignored: conversion from unsigned to signed */
  strncpy( (char*)setupReq->enbName, (const char*)s1apConfig->enbName, MAX_SIZE_ENBNAME  );

  setupReq->supportedTacs.numOfSupportedTac = s1apConfig->numOfSupportedTac;
  for( i=0; i< s1apConfig->numOfSupportedTac; i++ )
  {
    setupReq->supportedTacs.tacs[i] = s1apConfig->tacs[i];
  }
  setupReq->isCsgListPresent = FALSE;
  setupReq->pagingDrx = s1apConfig->pagingDrx;
}

/**
 * @param    resetReq   Reset Request for a UE context to be sent to MME
 * @details
 * We are fill the Reset Request for a UE
 * 
 **/
void s1apFillUeResetReq(
  IO S1ResetReq    *resetReq,
     S1Cause        resetCause,
     MmeUeS1apId    mmeUeS1apId,
     EnbUeS1apId    enbUeS1apId
)
{
  clearBytes(resetReq, sizeof(S1ResetReq));
  resetReq->resetType                                        = ResetType_UeS1Reset;
  resetReq->resetCause                                       = resetCause;
  resetReq->u.ueAssocLogConnList.numOfS1UeAssocLogConnItems  = 1;
  if(mmeUeS1apId != 0) 
  {
     resetReq->u.ueAssocLogConnList.s1ConnItem[0].isMmeUeS1apId = TRUE;
     resetReq->u.ueAssocLogConnList.s1ConnItem[0].mmeUeS1apId   = mmeUeS1apId;
  }
  if(enbUeS1apId != 0) 
  {
     resetReq->u.ueAssocLogConnList.s1ConnItem[0].isEnbUeS1apId = TRUE;
     resetReq->u.ueAssocLogConnList.s1ConnItem[0].enbUeS1apId   = enbUeS1apId;
  }
}

/**
 * @param    resetReq   Reset Request to be sent to MME
 * @details
 * We are filling the Reset Request
 * As of today we support only global Reset
 **/
void s1apFillGlobalResetReq(
    IO S1ResetReq *resetReq
    )
{
  resetReq->resetType      = ResetType_GlobalS1Reset;
  resetReq->resetCause.causeType = CauseType_Misc;
  resetReq->resetCause.causeVal = SztCauseMiscom_interventionEnum;
}

/**
 * @param   s1Cb                S1 Call block
 * @param   reset               Reset Message received
 * @param   resetAck            Reset Ack Message
 * @details
 * This function fills resetAck message from the received Reset Message
 */
void s1apFillResetAck(
    IO S1ResetAck    *resetAck,
    IN S1apMgrCb     *s1Cb,
    IN S1ResetReq    *reset
    )
{
  resetAck->isCriticalDiagnostic = FALSE;

  if( ResetType_UeS1Reset != reset->resetType )
  {
    resetAck->isUeAssociatedLogConnList = FALSE;
    return;
  }
  resetAck->isUeAssociatedLogConnList = TRUE;
  resetAck->ueAssocLogConnList = reset->u.ueAssocLogConnList;
}

/**
 * @param   s1Cb                S1 Call block
 * @param   mmeCfgUpdateFail
 * @details
 * This function fills mmeCfgUpdateFail message
 */
void s1apFillMmeCfgUpdateFail(
    IO MmeCfgUpdateFail    *mmeCfgUpdateFail,
    IN S1apMgrCb           *s1Cb
    )
{

  mmeCfgUpdateFail->failCause.causeType = CauseType_Misc;
  mmeCfgUpdateFail->failCause.causeVal = SztCauseMiscunspecifiedEnum;

  mmeCfgUpdateFail->isTimeToWait = FALSE;
  mmeCfgUpdateFail->isCriticalDiagnostic = FALSE;
}
/* ********************************** Sending Routines ********************************* */
/**
 * @param    setupReq   Setup Message
 * @param    peerId     The peer identifier in the list
 * @details
 * This function fills and send setup request to MME identified by peerId
 */
void s1apSendS1SetupReq(
    IN U32         peerId,
    IN S1SetupReq *setupReq
  )
{
   UehLiSapCb    *sap = NULLP;
   SztUDatEvnt  uDatEvnt;
   S16          ret = RFAILED;

   logInfo("Triggering S1 Setup Request towards peer:%d", peerId);
   UehCellCb     *cellCb = uehGetCellCb();
   sap = uehCb.sztSap[0];

   fillTransId( uDatEvnt, 1);
   fillPeerId( uDatEvnt, peerId);

   /* Create a ASN.1 PDU for the given setup structure */
   s1apWrapSetupReqPdu( &(uDatEvnt.pdu), setupReq );
   cellCb->uehRrmCellCounters.numS1apSetupReq++;
   /* Trigger Common Procedure data Request to S1AP Stack. */
   ret = UehLiSztUDatReq(&(sap->pst), UEH_SZ_SPID, &uDatEvnt);
}

/* ********************************** Sending Routines ********************************* */
/**
 * @param    abortReq   Abort Message
 * @param    peerId     The peer identifier in the list
 * @details
 * This function fills and send Abort request to S1ap protocol identified by peerId
 */
void s1apSendAbortReq(
     SztAbortS1 *s1Abort
  )
{
  UehLiSztUAbortReq (&uehCb.sztSap[0]->pst,uehCb.sztSap[0]->spId,s1Abort);
}

/**
 * @param    resetReq   Reset request
 * @param    peerId     The peer identifier in the list
 * @details
 * This function fills and sends Reset Request to MME identified by peerId
 */
void s1apSendResetReq(
    IN U32         peerId,
    IN S1ResetReq *resetReq
  )
{
   UehLiSapCb    *sap = NULLP;
   SztUDatEvnt  uDatEvnt;
   S16          ret = RFAILED;

   logInfo("Triggering S1 Reset Request towards peer:%d", peerId);
   UehCellCb     *cellCb = uehGetCellCb();
   sap = uehCb.sztSap[0];

   fillTransId( uDatEvnt, 1);
   fillPeerId( uDatEvnt, peerId);

   /* TODO: For UE specific Reset, should we use UDAT or DAT - need to check S1AP spec.
    * For now, use UDAT */
   /* Create a ASN.1 PDU for the given setup structure */
   if (ret == s1apWrapResetReqPdu( &(uDatEvnt.pdu), resetReq ))
   {
       logError("s1ap reset request mem allocation failed, msg dropped",1);
       return;
   }

   cellCb->uehRrmCellCounters.numS1apResetTx++;

   /* Trigger Common Procedure data Request to S1AP Stack. */
   ret = UehLiSztUDatReq(&(sap->pst), UEH_SZ_SPID, &uDatEvnt);
}


/**
 * @param    resetAck   Reset Ack Message
 * @param    peerId     The peer identifier in the list
 * @details
 * This function fills and Sends Reset Ack Message to MME identified by peerId
 */
void s1apSendResetAck(
    IN U32                 peerId,
    IN S1ResetAck          *resetAck
  )
{
   UehLiSapCb    *sap = NULLP;
   SztUDatEvnt  uDatEvnt;
   S16          ret = RFAILED;

   logInfo("Triggering S1 Reset Ack towards peer:%d", peerId);
   UehCellCb     *cellCb = uehGetCellCb();

   sap = uehCb.sztSap[0];

   fillTransId( uDatEvnt, 1);
   fillPeerId( uDatEvnt, peerId);

   /* Create a ASN.1 PDU for the given setup structure */
   s1apWrapS1ResetAckPdu( &(uDatEvnt.pdu), resetAck );
   cellCb->uehRrmCellCounters.numS1apResetRx++;

   /* Trigger Common Procedure data Request to S1AP Stack. */
   ret = UehLiSztUDatReq(&(sap->pst), UEH_SZ_SPID, &uDatEvnt);
}

/**
 * @param    mmeCfgUpdFail      MME Config Update Failure
 * @param    peerId             The peer identifier in the list
 * @details
 * This function will fills MME Config Update Failure and sends it to MME identified by peerId
 */
void s1apSendMmeCfgUpdateFail(
    IN U32                 peerId,
    IN MmeCfgUpdateFail    *mmeCfgUpdFail
  )
{
   UehLiSapCb    *sap = NULLP;
   SztUDatEvnt  uDatEvnt;
   S16          ret = RFAILED;

   logInfo("Triggering MME Config Update Fail towards peer:%d", peerId);

   sap = uehCb.sztSap[0];

   fillTransId( uDatEvnt, 1);
   fillPeerId( uDatEvnt, peerId);

   /* Create a ASN.1 PDU for the given setup structure */
   s1apWrapMmeCfgUpdateFailPdu( &(uDatEvnt.pdu), mmeCfgUpdFail );

   /* Trigger Common Procedure data Request to S1AP Stack. */
   ret = UehLiSztUDatReq(&(sap->pst), UEH_SZ_SPID, &uDatEvnt);
}

/* ****************************************************************************************** */
/* ********************************** StateMachine Routines ********************************* */
/* ****************************************************************************************** */

/* ********************************** S1_STATE_INIT ********************************* */
/**
 * @param   s1Cb   S1 Call block
 * @details
 * On receiving opstart request S1AP Manager sends the S1Setup request to MME
 * After S1Setup Response is received from MME then S1 Setup Procedure is considered complete
 */
void s1apProcOpStartInInit(
    IO S1apMgrCb   *s1Cb
    )
{
  S1SetupReq     setupReq;

  logInfo("%s", "Received OPSTART  in INIT state");

  /* Create the S1Setup request from the S1apEnodeBConfig
   * Send the request to PEER, as we have only one peer the index of the peer is 1
   */
  s1apFillSetupReq(&setupReq, s1Cb);
  s1apSendS1SetupReq( s1Cb->peerId, &setupReq);

  /* Change the state to Setup Pending, start guard Timer */
  s1apSetState( s1Cb, S1_STATE_SETUP_IN_PROG );
  s1Cb->s1SetupRetryCount++;

  uehStartTmr(UEH_TMR_S1AP_PROC_GUARD, &((S1apMgrCb*)s1Cb)->guardTimer,(PTR)s1Cb,0);
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * On receiving opstop from BSM send OPSTOP_COMPLETE to BSM
 */
void s1apProcOpStopInInit(
    IO S1apMgrCb   *s1Cb
    )
{
  logInfo("%s", "Received OPSTOP in INIT state");

  sendS1mMsgToBsm( EVT_S1M_BSM_OPSTOP_CMPL, 0, NULL );
  s1apSetState( s1Cb, S1_STATE_INIT );
  s1apBuildSzPeerDel( s1Cb->peerId, s1Cb );
}

/* ********************************** S1_STATE_SETUP_IN_PROG ********************************* */
/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    Setup Response
 * @details
 * Unwrap the received PDU, Stop procedure guard timer
 * Send OPSTART_SUCC to BSM, and change the state to SETUP_DONE
 */
void s1apProcSetupRspInSetupInProg(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  S1SetupRsp     setupRsp;
  UehCellCb     *cellCb = uehGetCellCb();

  logInfo("%s", "Received S1SETUP_RSP  in SETUP_PROG state");

  /* Stop the procedure guard timer and Unwrap the received Message */
  uehStopTmr( UEH_TMR_S1AP_PROC_GUARD,&(((S1apMgrCb*)s1Cb)->guardTimer),(PTR)s1Cb,0);
  s1apUnwrapSetupResPdu( uDatEvnt->pdu, &setupRsp );

  sendS1mMsgToBsm( EVT_S1M_BSM_OPSTART_SUCC, 0, NULL );

  /* Save the MME Information in the global call block,
   * EnodeB don't have to check whether PLMN are supported at MME as it is MME job to do this */
  if( TRUE == setupRsp.isMmeName )
    copyBytes( setupRsp.mmeName, strlen((char*)setupRsp.mmeName),  s1Cb->mmeName );
  s1Cb->relativeMmeCapacity = setupRsp.relativeMmeCapacity;
  s1Cb->servedGummeIes = setupRsp.servedGummeIes;

  s1Cb->s1SetupRetryCount = 0;
  s1apSetState( s1Cb, S1_STATE_SETUP_DONE );
  /* Initialize the UE Ctx Hang Detection Counters and start the timers*/
    uehCb.uehCtxHangDetection.pucchAuditCounter = 0;
    uehCb.uehCtxHangDetection.ueStateAuditCounter = 0;
    uehCb.uehCtxHangDetection.ueProcedureAuditCounter = 0;
    cellCb->uehRrmCellCounters.numS1apSetupRsp++;
    uehStartTmr (UEH_PUCCH_AUDIT_TMR, &uehCb.uehCtxHangDetection.pucchAuditTmr, NULLP, 0);
    uehStartTmr (UEH_UESTATE_AUDIT_TMR,&uehCb.uehCtxHangDetection.ueStateAuditTmr,NULLP,0);
    uehStartTmr (UEH_UEPROCEDURE_AUDIT_TMR, &uehCb.uehCtxHangDetection.ueProcedureAuditTmr, NULLP,0);
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * No response from MME is received for the Setup request,
 * Send OPSTART_FAIL to BSM, and close the SCTP association with MME
 */
void s1apProcGuardTmrExpiryInSetupInProg(
    IO S1apMgrCb   *s1Cb
    )
{
  SztAbortS1     s1Abort;
  logInfo("%s", "Received S1GUARD_TMR_EXPRY  in SETUP_PROG state");
  
  clearBytes( &s1Abort, sizeof(SztAbortS1) ); 
  s1Abort.peerId.pres = PRSNT_NODEF;
  s1Abort.peerId.val = s1Cb->peerId; 
  s1apSendAbortReq(&s1Abort);
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * S1 Setup retry timer expired, send the Setup request to MME
 * If maximum retries expired send OPSTART_FAIL to BSM, and close the SCTP association with MME
 */
void s1apProcSetupTmrExpiryInSetupInProg(
    IO S1apMgrCb   *s1Cb
    )
{

  logInfo("%s", "Received S1SETUP_TMR_EXPRY in SETUP_PROG state");

  s1apReTrySetupReqMsg(s1Cb);
  return;
}

void s1apReTrySetupReqMsg(IO S1apMgrCb *s1Cb)
{
  S1SetupReq     setupReq;

  if( s1Cb->maxS1SetupAllowed <= s1Cb->s1SetupRetryCount )
  {
    /* Delete SCTP association
     * Set state to Idle
     * Send Op start Fail*/
    s1apSetState( s1Cb, S1_STATE_INIT );
    s1apSendOpstartFailClearSctp( BS_ERR_S1AP_MAX_RETRY_EXPIRED, (SINT8*)ERR_RCVD_MAX_RETRY_EXHAUSTED, s1Cb );
    return;
  }

  /* Create the S1Setup request from the S1apEnodeBConfig
   * Send the request to PEER, as we have only one peer the index of the peer is 1
   */
  s1apFillSetupReq(&setupReq,s1Cb);
  s1apSendS1SetupReq( s1Cb->peerId, &setupReq);

  s1Cb->s1SetupRetryCount++;
  uehStartTmr(UEH_TMR_S1AP_PROC_GUARD,&((S1apMgrCb*)s1Cb)->guardTimer,(PTR)s1Cb,0);

}

static char *getCauseTypeName(
  IN CauseType causeType
  )
{
  switch( causeType )
  {
  case CauseType_RadioNw:     return "CauseType_RadioNw";     break;
  case CauseType_Transport:   return "CauseType_Transport";   break;
  case CauseType_Nas:         return "CauseType_Nas";         break;
  case CauseType_Prototcol:   return "CauseType_Prototcol";   break;
  case CauseType_Misc:        return "CauseType_Misc";        break;
  default:                    return "CauseType Not Handled"; break;
  }
}
/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    S1 Setup Fail received from MME
 * @details
 * S1 Setup fail received from MME, If retry timer is provided, start the timer
 * after ethe expiry of retry timer send Setup again to MME.
 * For other errors send OPSTART_FAIL to BSM, and close the SCTP association with MME
 */
void s1apProcSetupFailInSetupInProg(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    )
{

  S1SetupFail   setupFail;
  BsErrorValue  bsErrorNum = BS_NO_ERROR;
// RRM Changes
  UehCellCb     *cellCb = uehGetCellCb();
  cellCb->uehRrmCellCounters.numS1apRejected++; /* Number of S1AP Requests Rejected */

// RRM Changes Done
  logInfo("%s", "Received S1SETUP_FAIL  in SETUP_PROG state");

  /* Stop the procedure guard timer and Unwrap the received Message */
  uehStopTmr( UEH_TMR_S1AP_PROC_GUARD,&(((S1apMgrCb*)s1Cb)->guardTimer),(PTR)s1Cb,0);
  s1apUnwrapSetupFailPdu( uDatEvnt->pdu, &setupFail );

  /* IF Time to wait is provided, then we can start a timer and wait for the time mentioned by MME
   * After timer expiry then we can Re-send the setup Request
   */
  if( TRUE == setupFail.isTimeToWait )
  {
    logInfo("Received setup Failure with TimeToWait value=%d", setupFail.timetoWait );
    uehCb.uehTimerDescTable[UEH_TMR_S1AP_SETUP].duration = TMR_UINTS_PER_SEC * getS1SetupTmrValFromEnum(setupFail.timetoWait);
    uehStartTmr(UEH_TMR_S1AP_SETUP, &((S1apMgrCb*)s1Cb)->setupTimer,(PTR)s1Cb,0);
    return;
  }

  /* If no timer is given, this implies MME is not interested in talking to us
   * Set state to Idle
   * Send Op start Fail
   */
  String256  errString;
  clearBytes( &errString, 256 );

  logError("Error CauseChoice=%d Error Value=%d", setupFail.failCause.causeType, setupFail.failCause.causeVal );
  switch ( setupFail.failCause.causeType )
  {
    case CauseType_RadioNw:
      s1apFillErrorValueByRadioNwCauseVal( setupFail.failCause.causeVal, s1Cb, &bsErrorNum );
      break;
    case CauseType_Transport:
      s1apFillErrorValueByTransPortCauseVal( setupFail.failCause.causeVal, s1Cb, &bsErrorNum );
      break;
    case CauseType_Nas:
      s1apFillErrorValueByNasCauseVal( setupFail.failCause.causeVal, s1Cb, &bsErrorNum );
      break;
    case CauseType_Prototcol:
      s1apFillErrorValueByProtocolCauseVal( setupFail.failCause.causeVal, s1Cb, &bsErrorNum );
      break;
    case CauseType_Misc:
      s1apFillErrorValueByMiscCauseVal( setupFail.failCause.causeVal, s1Cb, &bsErrorNum, errString );
      break;
    default:
      logError( "Cause Type %d not handled. Hence Mapping with BS error is not done. Check code.",  setupFail.failCause.causeType );
      break;
  }

  snprintf((S8 *)errString, 256, "%s:%d / 3GPP:S1AP:%s:%d", errorCodeStr(bsErrorNum), bsErrorNum, getCauseTypeName(setupFail.failCause.causeType), setupFail.failCause.causeType );
  s1apSendOpstartFailClearSctp( bsErrorNum, (SINT8*)errString, s1Cb );
  s1apSetState( s1Cb, S1_STATE_INIT );
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * received OPSTOP from BSM, send OPSTOP_COMPLETE to BSM,
 * Close the SCTP connection with MME
 */
void s1apProcOpStopInSetupInProg(
    IO S1apMgrCb   *s1Cb
    )
{
  logInfo("%s", "OPSTOP  in SETUP_PROG state");

   sendS1mMsgToBsm( EVT_S1M_BSM_OPSTOP_CMPL, 0, NULL );
   /* Delete SCTP association and set to Idle */
   s1apBuildSzPeerDel( s1Cb->peerId, s1Cb );
   s1apSetState( s1Cb, S1_STATE_INIT );
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * SCTP connection is broken with MME, send LINK_DOWN to BSM
 * Send OPSTART_FAIL to BSM and clear the SCTP connection
 */
void s1apProcLinkDownInSetupInProg(
    IO S1apMgrCb   *s1Cb
    )
{
  logInfo("%s", "LinkDown  in SETUP_PROG state");

  /* Set the link status and change the procedure status */
  s1apLinkStatusToBsm( S1_LINK_DOWN, s1Cb );
  s1apSendLinkStatusToUeh( S1_LINK_DOWN );

  s1apSendOpstartFailClearSctp( BS_ERR_S1AP_NO_RESP_FROM_MME, (SINT8*)ERR_RCVD_SCTP_ASSOC_DOWN_IND, s1Cb );
  s1apSetState( s1Cb, S1_STATE_INIT );
}

/**
 * @param   s1Cb   S1 Call block
 * @param   null   NO PDU is expected
 * @details
 * SCTP connection is established with MME, send LINK_UP to BSM
 */
void s1apProcLinkUpInSetupInProg(
    IO S1apMgrCb   *s1Cb
    )
{
  logInfo("%s", "LinkUp  in SETUP_PROG state");

  s1apLinkStatusToBsm( S1_LINK_UP, s1Cb );
  s1apSendLinkStatusToUeh( S1_LINK_UP );
}

/* ********************************** S1_STATE_SETUP_DONE ********************************* */
/**
 * @param   s1Cb   S1 Call block
 * @details
 * S1M sends creates a global reset request and sends this to MME,
 * Change the state to  RESET_IN_PROG
 */
void s1apProcSndResetInSetupDone(
    IO S1apMgrCb   *s1Cb
    )
{
  S1ResetReq reset;

  logInfo("%s", "Send Reset In SETUP_DONE state");

  /* Create the Reset Request
  * Send the request to PEER, we send Global Reset
  */
  s1apFillGlobalResetReq(&reset);
  s1apSendResetReq( s1Cb->peerId, &reset);

  s1apSetState( s1Cb, S1_STATE_RESET_IN_PROG );
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * S1M sends Abort conformation to S1AP user
 */

void s1apProcAbortCfmInAllStates(IO S1apMgrCb *s1Cb)
{
  s1apReTrySetupReqMsg(s1Cb);  
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * send OPSTOP_CMPL to BSM, close the SCTP connection towards MME
 */
void s1apProcOpStopInSetupDone(
    IO S1apMgrCb   *s1Cb
    )
{
  logInfo("%s", "OPSTOP  in SETUP_DONE state");

  sendS1mMsgToBsm( EVT_S1M_BSM_OPSTOP_CMPL, 0, NULL );
  /* Delete SCTP association and set to Idle */
  s1apBuildSzPeerDel( s1Cb->peerId, s1Cb );
  s1apSetState( s1Cb, S1_STATE_INIT );
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    Paging PDU from MME
 * @details
 * Received Paging from MME, invoke the Cell State MAchine with event EVT_PAGING
 */
void s1apProcPagingInSetupDone(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  S1apPdu       *s1Pdu = uDatEvnt->pdu;
  UehCellCb     *cellCb = uehGetCellCb();

  logInfo("%s", "PAGING  in SETUP_DONE state");

  crmInvokeCellSm( cellCb, CELL_EVT_PAGING, (void *)s1Pdu );
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    Reset request from MME
 * @details
 * Received Reset from MME, invoke the Cell State MAchine with event EVT_S!_RESET
 * After all calls are cleared, send RESET_ACK to MME
 * Report RESET to BSM
 */
void s1apProcResetInSetupDone(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  UehCellCb             *cellCb = uehGetCellCb();
  MsgS1apBsmS1ResetInd   s1Reset;
  S1ResetAck             resetAck;
  S1ResetReq             reset;

  logInfo("%s", "RESET  in SETUP_DONE state");

  s1apUnwrapS1ResetReqPdu( uDatEvnt->pdu, &reset );

  /* Create the ResetAck Response
   * Send the response to PEER, as we have only one peer the index of the peer is 1
   * After receiving Reset the S1Manager state don't change it is in Available, Reset is Similar to
   * Setup, and all contexts are cleared in S1.
   */
  crmInvokeCellSm( cellCb, CELL_EVT_S1_RESET, (void *)&reset );

  s1apGetMmeIpAddr( &s1Reset, s1Cb );
  sendS1mMsgToBsm( EVT_S1M_RESET_IND, sizeof(MsgS1apBsmS1ResetInd), (U8*)&s1Reset );

  s1apFillResetAck(&resetAck, s1Cb, &reset);
  s1apSendResetAck( s1Cb->peerId, &resetAck);
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * SCTP connection towards MME is terminated,
 * Sen the LINK_DOWN message to BSM,
 * Change the state to INIT
 */
void s1apProcLinkDownInSetupDone(
    IO S1apMgrCb   *s1Cb
    )
{
  logInfo("%s", "LinkDown  in SETUP_DONE state");

  /* In Rel 1.0 We don't support MME Configuration update,
  * We send MmeConfigFail */
  s1apLinkStatusToBsm( S1_LINK_DOWN, s1Cb );
  s1apSendLinkStatusToUeh( S1_LINK_DOWN );

  s1apSetState( s1Cb, S1_STATE_INIT );
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    MME Config Update
 * @details
 * Received MME Config update from MME,
 * Update the MME related parameters
 */
void s1apProcMmeCfgUpdateInSetupDone(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  MmeCfgUpdateFail       mmeCfgUpdateFail;
  MmeCfgUpdate           mmeCfgUpdate;

  logInfo("%s", "MME Config Update  in SETUP_DONE state");

  s1apUnwrapMmeConfigUpdatePdu( uDatEvnt->pdu, &mmeCfgUpdate );

  /* TODO In Rel 1.0 We don't support configuration update,
  * We send Failure always for this.
  */
  s1apFillMmeCfgUpdateFail(&mmeCfgUpdateFail, s1Cb );
  s1apSendMmeCfgUpdateFail( s1Cb->peerId, &mmeCfgUpdateFail);
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    MME Overload
 * @details
 * Received MME Overload Stop
 * Update the MME related overload parameters
 */
void s1apProcOverloadStartInSetupDone(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  OverloadStart           overloadStart;

  logInfo("%s", "MME OverloadStart SETUP_DONE state");

  s1apUnwrapOverloadStartPdu( uDatEvnt->pdu, &overloadStart );
  /* Update the Overload state in S1 Manager call block.
  */
  s1Cb->overloadState = overloadStart.overloadResponse.overloadAction;
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * Received MME Overload Stop
 * Clear the MME related overload parameters
 */
void s1apProcOverloadStopInSetupDone(
    IO S1apMgrCb   *s1Cb
    )
{
  logInfo("%s", "MME OverloadStop SETUP_DONE state");

  /* Update the Overload state in S1 Manager call block,
  * So that all calls are permitted
  */
  s1Cb->overloadState = OverloadAction_PermitAllCalls;
}

/* ********************************** S1_STATE_RESET_IN_PROG ********************************* */
/**
 * @param   s1Cb   S1 Call block
 * @details
 * This functions process Procedure Gurad timer expiry in Reset state
 * When Reset Ack is not received within the procedure guard time, we terminate the SCTP connection
 * towards the MME.
 */
void s1apProcGuardTmrExpiryInResetInProg(
    IO S1apMgrCb   *s1Cb
    )
{
  logInfo("%s", "S1GUARD_TMR_EXPRY  in RESET_PROG state");

  /* Delete SCTP association and set to Idle */
  s1apBuildSzPeerDel( s1Cb->peerId, s1Cb );
  s1apSetState( s1Cb, S1_STATE_INIT );
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * This functions process OP Stop in reset state,
 * We delete the SCTP connection and change the s1Cn state to INIT
 */
void s1apProcOpStopInResetInProg(
    IO S1apMgrCb   *s1Cb
    )
{
  logInfo("%s", "OPSTOP  in RESET_PROG state");

  sendS1mMsgToBsm( EVT_S1M_BSM_OPSTOP_CMPL, 0, NULL );
  /* Delete SCTP association and set to Idle */
  s1apBuildSzPeerDel( s1Cb->peerId, s1Cb );
  s1apSetState( s1Cb, S1_STATE_INIT );
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    Reset Ack PDU from MME
 * @details
 * This functions unwraps the received Reset ACK PDU,
 * Stops the procedure guard timer
 * Changes the s1Cb state to SETUP Done
 */
void s1apProcResetAckInResetInProg(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  S1ResetAck     resetAck;

  logInfo("%s", "RESETACK  in RESET_INPROG state");

  uehStopTmr( UEH_TMR_S1AP_PROC_GUARD,&(((S1apMgrCb*)s1Cb)->guardTimer),(PTR)s1Cb,0);
  s1apUnwrapResetAckPdu( uDatEvnt->pdu, &resetAck );

  s1apSetState( s1Cb, S1_STATE_SETUP_DONE );
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    Reset PDU from MME
 * @details
 * This functions unwraps the received Reset request, fills the reset ACK
 * and sends the Reset ACK to MME
 */
void s1apProcResetInResetInProg(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  S1ResetReq     reset;
  S1ResetAck     resetAck;

  logInfo("%s", "RESETACK  in RESET_INPROG state");

  s1apUnwrapS1ResetReqPdu( uDatEvnt->pdu, &reset );

  /* Create the ResetAck Response
   * Send the request to PEER, we send all the UEID's in response, as we cleared the local context
   */
  s1apFillResetAck(&resetAck, s1Cb, &reset);
  s1apSendResetAck( s1Cb->peerId, &resetAck);
}

/**
 * @param   s1Cb   S1 Call block
 * @details
 * This functions Stops Procedure guard timer, and reports LINK_DOWN event to BSM and UEH
 */
void s1apProcLinkDownInResetInProg(
    IO S1apMgrCb   *s1Cb
    )
{
  logInfo("%s", "LinkDown  in RESET_INPROG state");

  uehStopTmr( UEH_TMR_S1AP_PROC_GUARD,&(((S1apMgrCb*)s1Cb)->guardTimer),(PTR)s1Cb,0);
  /* Set the link status and change the procedure status */
  s1apLinkStatusToBsm( S1_LINK_DOWN, s1Cb );
  s1apSendLinkStatusToUeh( S1_LINK_DOWN );

  s1apSetState( s1Cb, S1_STATE_INIT );
}

/**
 * @param    peerId              The peer identifier in the list
 * @param    writeReplcWarnRsp    Write Replace Warning Response message
 * @details
 * This function sends writeReplcWarnRsp MME identified by peerId
 */
static void s1apSendWriteReplaceWarnRsp(
    IN U32                  peerId,
    IN WriteReplcWarnRsp   *writeReplcWarnRsp
  )
{
   UehLiSapCb    *sap = NULLP;
   SztUDatEvnt  uDatEvnt;
   S16          ret = RFAILED;

   logInfo("Triggering MME Write-Replace Warning Response towards peer:%d", peerId);

   sap = uehCb.sztSap[0];

   fillTransId( uDatEvnt, 1);
   fillPeerId( uDatEvnt, peerId);

   /* Create a ASN.1 PDU for the given setup structure */
   s1apWrapMmeWriteRplcWarnRsp( &(uDatEvnt.pdu), writeReplcWarnRsp );

   /* Trigger Common Procedure data Request to S1AP Stack. */
   ret = UehLiSztUDatReq(&(sap->pst), UEH_SZ_SPID, &uDatEvnt);
}

/**
 * @param   s1Cb                S1 Call block
 * @param   mmeCfgUpdateFail
 * @details
 * This function fills mmeCfgUpdateFail message
 */
static void s1apFillWriteReplaceWarnRspAsFail(
    IO WriteReplcWarnRsp      *writeReplcWarnRsp,
    IN WriteReplcWarnReq      *writeReplcWarnReq
    )
{
  copyBytes(writeReplcWarnReq->warnMsgId, MAX_SIZE_WARNMSGID, writeReplcWarnRsp->warnMsgId );
  copyBytes(writeReplcWarnReq->serialNum, MAX_SIZE_SERIALNUM, writeReplcWarnRsp->serialNum );
  writeReplcWarnRsp->isBroadcastCmpltAreaListPresent = FALSE;
  writeReplcWarnRsp->isCriticalDiagnosticPresent     = FALSE;
}

/**
 * @param   s1Cb   S1 Call block
 * @param   pdu    MME Warning Request
 * @details
 * Received MME Write-Replace Warning Request from MME,
 * In Rel 1.0 We don't support Write-Replace Warning Procedure
 * We will always send Failure Response for this message
 */
void s1apProcWarningReqInAllStates(
    IO S1apMgrCb   *s1Cb,
    IN SztUDatEvnt  *uDatEvnt
    )
{
  WriteReplcWarnReq      writeReplcWarnReq;
  WriteReplcWarnRsp      writeReplcWarnRsp;

  clearBytes( &writeReplcWarnReq, sizeof(WriteReplcWarnReq) );
  clearBytes( &writeReplcWarnRsp, sizeof(WriteReplcWarnRsp) );


  s1apUnwrapMmeWriteReplaceWarningReq( uDatEvnt->pdu, &writeReplcWarnReq );

  /* TODO In Rel 1.0 We don't support Write-Replace Warning Procedure
  * We will always send Failure Response for this message
  */
  s1apFillWriteReplaceWarnRspAsFail( &writeReplcWarnRsp, &writeReplcWarnReq );
  s1apSendWriteReplaceWarnRsp( s1Cb->peerId, &writeReplcWarnRsp);

}

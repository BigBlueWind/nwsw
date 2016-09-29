/**
 * @file    ueh_li.c
 * @brief This file contains call back functions for all lower
               interfaces to UEH
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
 *     a) Created initial version from sample application
 * 2. Shweta Polepally 11/11/11
 *    State Machine changed from Array based func pointer implementation to Switch Case format.
 *    Refer ueFsm () function
 */


#include"ccpu_cmn.h"

/* RRM related includes */
//#include "ueh_eut.h"
#include "ueh.h" 
#include "lueh.h"
#include "ueh_eut_x.h"
#include "lueh.x" 
#include "log.h"
#include "ueh_crm.h"
#include "ueh_sib.h"
#include "bs_err_macro.h"
#include "s1ap.h" 
#include "s1ap_sm.h" 
#ifdef RRM_PRL_FLAG
#include "trace.h"
#endif

U8  countUe = 0;
U8  countProc = 0;
U32 crntiIdx = 0;
/****
 * Note::
 * State Machine has been changed from Array based func pointer implementation to Switch Case format.
 * Date: 11/11/11
 * Refer ueFsm () function
 * */

char *uehGetUeProcedureName(
  UehUeProcedure proc	
)
{
  switch (proc)
  {
    case UEH_RRC_CON_SETUP: return "UEH_RRC_CON_SETUP";
    case UEH_INITIAL_CXT_SETUP: return "UEH_INITIAL_CXT_SETUP";
    case UEH_UE_CXT_REL: return "UEH_UE_CXT_REL";
    case UEH_ERAB_SETUP: return "UEH_ERAB_SETUP";
    case UEH_ERAB_REL: return "UEH_ERAB_REL";
    case UEH_MEAS_CFG: return "UEH_MEAS_CFG";
    case UEH_OUT_HO_ONGOING: return "UEH_OUT_HO_ONGOING";
    case UEH_IN_HO_ONGOING: return "UEH_IN_HO_ONGOING";
    case UEH_REEST_ONGOING: return "UEH_REEST_ONGOING";
    case UEH_UE_CXT_REL_DUE_TO_RL_FAIL: return "UEH_UE_CXT_REL_DUE_TO_RL_FAIL";
    case UEH_UE_CONN_REL_ONGOING: return "UEH_UE_CONN_REL_ONGOING";
    case UEH_UE_CTXT_CLEAR_ONGOING: return "UEH_UE_CTXT_CLEAR_ONGOING";
    case UEH_INITIAL_CXT_SETUP_FAILURE: return "UEH_INITIAL_CXT_SETUP_FAILURE";
    case UEH_UE_REL_REQ_SENT: return "UEH_UE_REL_REQ_SENT";
    case UEH_UNUSED_PROC: return "UEH_UNUSED_PROC";
    case UEH_PUCCH_PWR_REC_ONGOING: return "UEH_PUCCH_PWR_REC_ONGOING";
    case UEH_INITIAL_UE_MSG: return "UEH_INITIAL_UE_MSG";
    case UEH_NO_PROCEDURE: return "UEH_NO_PROCEDURE";
    default: return "UNKNOWN-PROCEDURE";
  }
}

/**
 *       Fun:   uehGetUeStateName
 *       Desc:  Returns State Name in string format from Enum Value
 *       Ret:   char
 *       Notes: None
 *       File:  ueh_li.c
 */
char *uehGetUeStateName(
  UeState ueState
  )
{
  switch( ueState )
  {
    case UE_STATE_RRC_IDLE:         return "UE_STATE_RRC_IDLE";
    case UE_STATE_RRC_CONNECTING:   return "UE_STATE_RRC_CONNECTING";
    case UE_STATE_S1_CONNECTING:    return "UE_STATE_S1_CONNECTING";
    case UE_STATE_CONNECTED:        return "UE_STATE_CONNECTED";
    case UE_STATE_SMC_ONGOING:      return "UE_STATE_SMC_ONGOING";
    case UE_STATE_UE_CAP_ONGOING:   return "UE_STATE_UE_CAP_ONGOING";
    case UE_STATE_REC_ONGOING:      return "UE_STATE_REC_ONGOING";
    case UE_STATE_CALL_CONNECTED:   return "UE_STATE_CALL_CONNECTED";
    case UE_STATE_REL_ONGOING:      return "UE_STATE_REL_ONGOING";
    case UE_STATE_HANDOVER_ONGOING: return "UE_STATE_HANDOVER_ONGOING";
    default:                        return "UE-STATE-NAME-INVALID";
  }
}

/**
 *       Fun:   uehHndlEvntRrcConnReq
 *       Desc:  Handles RRC Connection Request Message based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntRrcConnReq(
  IO UehUeCb       *ueCb,
  IO NhuDatIndSdus *datIndSdu,
  IO U8            ueEvt
  )
{
  NhuRRCConRqst_r8_IEs        *rrcConReq        = NULLP;
  U8                          *mtmsi            = NULLP;
  U32                          m_tmsi           = 0;
  U16                          ueIdx            = 0;

  rrcConReq = &(datIndSdu->sdu.m.ulCcchMsg.ccchMsg.message.val.c1.val.rrcConRqst.criticalExtns.val.rrcConRqst_r8);
  if(rrcConReq->ue_Identity.choice.val == INITUE_IDENTITY_S_TMSI)
  {
    ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
    mtmsi = rrcConReq->ue_Identity.val.s_TMSI.m_TMSI.val;
    m_tmsi = (mtmsi[3]) |  (mtmsi[2] << 8) | (mtmsi[1] << 16) | (mtmsi[0] << 24);
    ueCb->isSTmsi = TRUE;
    ueCb->sTmsi.mmeCode = rrcConReq->ue_Identity.val.s_TMSI.mmec.val[0];
    ueCb->sTmsi.mTmsi = m_tmsi;
    UEH_MEM_COPY(&ueCb->s1Stmsi.mTimsi.mTimsi[0], &rrcConReq->ue_Identity.val.s_TMSI.m_TMSI.val[0], rrcConReq->ue_Identity.val.s_TMSI.m_TMSI.len/8);
    ueCb->s1Stmsi.mmeCode = rrcConReq->ue_Identity.val.s_TMSI.mmec.val[0]; 
  }
  switch( ueCb->ueState )
  {
    case UE_STATE_RRC_IDLE:
      return uehProcRrcConReq( ueCb, datIndSdu );
    default:
      UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
      RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntRrcConnReEst
 *       Desc:  Handles RRC Connection Reestablish Event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntRrcConnReEst(
  IO UehUeCb  *ueCb,
  IO void     *pdu,
  IO U8       ueEvt
  )
{
  if (ueCb != NULLP)
  {
    switch( ueCb->ueState )
    {
      case UE_STATE_CALL_CONNECTED :
      case UE_STATE_SMC_ONGOING :
      case UE_STATE_UE_CAP_ONGOING :
      case UE_STATE_REC_ONGOING :
      case UE_STATE_REL_ONGOING :
      case UE_STATE_HANDOVER_ONGOING:
        return uehProcRrcConReEstblReq (ueCb,pdu);
      break; 
      default:
       UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
       RETVALUE(RFAILED);
    }
  }
  RETVALUE(ROK);
}

/**
 *       Fun:   uehHndlEvntRrcConnReCfgCmpl
 *       Desc:  Handles RRC Connection Re-Config Complete Event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntRrcConnReCfgCmpl(
  IO UehUeCb       *ueCb,
  IO NhuDatIndSdus *pdu,
  IO U8            ueEvt
  )
{
  ueCb->reCfgProcComp = TRUE;
  switch( ueCb->ueState )
  {
    case UE_STATE_REC_ONGOING:
         uehStopTmr(UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
         PcmTimer *rlfDetectionTmr = &ueCb->ueCqiBasedRlfDetectionTmr;
         if (rlfDetectionTmr->isTmrRunning == FALSE)
         {
           uehStartTmr(UEH_CQI_BASED_RLF_DETECTION_TMR, rlfDetectionTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
         }  
         return uehProcRrcReCfgCmp( ueCb, pdu );
    case UE_STATE_CALL_CONNECTED:
         /** case where RrcConnection Reconfig is being used for power reset. Ignore. **/
         RETVALUE(ROK);
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntRrcConnSetupCmpl
 *       Desc:  Handles RRC Connection Setup Complete Event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntRrcConnSetupCmpl(
  IO UehUeCb       *ueCb,
  IO NhuDatIndSdus *pdu,
  IO U8            ueEvt
  )
{ 
  U16 ueIdx = 0;
  switch( ueCb->ueState )
  {
    case UE_STATE_RRC_CONNECTING:
         uehStopTmr(UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
         ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
         //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus =  uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus | RRC_CONN_EST_SUCC;
         return uehProcRrcConSetCmp( ueCb, pdu );

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntRrcConnSecModeCmpl
 *       Desc:  Handles RRC Connection Security Mode Complete Event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntRrcConnSecModeCmpl(
  IO UehUeCb       *ueCb,
  IO NhuDatIndSdus *pdu,
  IO U8            ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_SMC_ONGOING:
      uehStopTmr(UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
      return uehProcRrcSecModCmp( ueCb, pdu );

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntRrcConnSecModeFail
 *       Desc:  Handles RRC Connection Security Mode failure Event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntRrcConnSecModeFail(
  IO UehUeCb       *ueCb,
  IO NhuDatIndSdus *pdu,
  IO U8            ueEvt
  )
{
  U16 ueIdx = 0;
  switch( ueCb->ueState )
  {
    case UE_STATE_SMC_ONGOING:
         uehStopTmr(UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
         ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
         //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueFailureEvt = EVENTTYPE_SECMODECOMMAND;
         UEH_ERROR_RECEIVED_FAILURE_EVENT(EVTNHUDATIND, ENTNH, SztCauseRadioNwfailure_in_radio_interface_procedureEnum, UE, ueCb->crnti); 
        /* SztCauseRadioNwencryption_and_or_integrity_protection_algorithms_not_supportedEnum */
        uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_RadioNw, SztCauseRadioNwfailure_in_radio_interface_procedureEnum);
        RETVALUE(ROK);
       /*   return uehProcRrcSecModFail( ueCb, pdu );*/

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntRrcUlInfoTrnsfr
 *       Desc:  Handles RRC UL Info Transfer Event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntRrcUlInfoTrnsfr(
  IO UehUeCb       *ueCb,
  IO NhuDatIndSdus *pdu,
  IO U8            ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_CONNECTED:
    case UE_STATE_SMC_ONGOING:
    case UE_STATE_UE_CAP_ONGOING:
    case UE_STATE_REC_ONGOING:
    case UE_STATE_CALL_CONNECTED:
    case UE_STATE_REL_ONGOING:
      return uehProcUlNasTrans( ueCb, pdu );

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntRrcLiCfgCfm
 *       Desc:  Handles RRC Li Config Confirm Event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntRrcLiCfgCfm(
  IO UehUeCb       *ueCb,
  IO NhuCfgCfmSdus *pdu,
  IO U8            ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_RRC_CONNECTING:
   
      /* isUehUeProcFailed checks whether the given condition is true or false
         On failure case it takes corresponding action and prints procId responsible for failure */ 
      if(isUehUeProcFailed(ueCb,PROC_RRC_CONN_SETUP,(C3_RRCCONSETUP == pdu->hdr.transId)))
      {
        if(isUehUeProcFailed(ueCb,PROC_RRC_CONN_SETUP,(TRUE != pdu->sdu.isCfgSuccess)))
         {
           uehSndRrcConRej(ueCb);
           freeUeCb(ueCb);
         }
      }    
      else
      {
        UEH_ERROR_ELMNT_OUT_OF_RANGE("Trans Id", pdu->hdr.transId, NOT_APPLICABLE, C3_RRCCONSETUP);
      }
      RETVALUE(ROK);
      break;

    case UE_STATE_SMC_ONGOING:
      if(isUehUeProcFailed(ueCb,PROC_RRC_SEC_MODE,(C4_SECURITYMODECMD == pdu->hdr.transId)))
      {
        if(isUehUeProcFailed(ueCb,PROC_RRC_SEC_MODE,(TRUE != pdu->sdu.isCfgSuccess)))
        {
          uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_RadioNw, SztCauseRadioNwradio_resources_not_availableEnum);
         freeUeCb(ueCb);
        }
      } 
     else
     {
       UEH_ERROR_ELMNT_OUT_OF_RANGE("Trans Id", pdu->hdr.transId, NOT_APPLICABLE, C4_SECURITYMODECMD);
     }
     RETVALUE(ROK);
     break;

    case UE_STATE_REC_ONGOING:
    
     
      if(isUehUeProcFailed(ueCb,PROC_RRC_CONN_RECONF,(C4_RRCCONRECFGN == pdu->hdr.transId)))
      { 
        if(isUehUeProcFailed(ueCb,PROC_RRC_CONN_RECONF,(TRUE != pdu->sdu.isCfgSuccess)))
        {
          uehSndEgtpDelInitCtxtSetupFail(ueCb, CauseType_RadioNw, SztCauseRadioNwradio_resources_not_availableEnum);
          freeUeCb(ueCb);
        }
      }
      else
      {
        UEH_ERROR_ELMNT_OUT_OF_RANGE("Trans Id", pdu->hdr.transId, NOT_APPLICABLE, C4_RRCCONRECFGN);
      }
      RETVALUE(ROK);
      break;
    
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntRrcErrInd
 *       Desc:  Handles RRC Error Indication Event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntRrcErrInd(
    IO UehUeCb       *ueCb,
    IO NhuErrIndSdus *pdu,
    IO U8            ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_RRC_IDLE:        return uehProcRrcIdleRrcErrInd( ueCb, pdu );
    case UE_STATE_RRC_CONNECTING:  return uehProcRrcConRrcErrInd( ueCb, pdu );
    case UE_STATE_S1_CONNECTING:   return uehProcS1ConRrcErrInd( ueCb, pdu );
    case UE_STATE_CONNECTED:       return uehProcConnectedRrcErrInd( ueCb, pdu );
    case UE_STATE_SMC_ONGOING:  
         uehStopTmr(UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
         return uehProcSmcOngoingRrcErrInd( ueCb, pdu );
    case UE_STATE_UE_CAP_ONGOING: 
         uehStopTmr(UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
         return uehProcUeCapOngoingRrcErrInd( ueCb, pdu );
    case UE_STATE_REC_ONGOING:     return uehProcRecOngoingRrcErrInd( ueCb, pdu );
    case UE_STATE_CALL_CONNECTED:  return uehProcCallConnectedRrcErrInd( ueCb, pdu );
    case UE_STATE_REL_ONGOING:     return uehProcRelOngoingRrcErrInd(ueCb, pdu);

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntSztUeCtxSetup
 *       Desc:  Handles S1AP Ue Context Setup Event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntSztUeCtxSetup(
  IO UehUeCb  *ueCb,
  IO S1apPdu  *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_S1_CONNECTING:   
      uehStopTmr(UE_TMR_UE_SRB1,&((UehUeCb *)ueCb)->ueSrb1Tmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti );
      uehStartTmr(UE_TMR_INTERNAL_CONF,&((UehUeCb *)ueCb)->ueInternalConfTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
      return uehProcInitCtxtSetupInS1Con( ueCb, pdu );

    case UE_STATE_CONNECTED:      
      uehStopTmr(UE_TMR_UE_SRB1,&((UehUeCb *)ueCb)->ueSrb1Tmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti );

      /* Timer needed to delete UE in case no responce to configuration from lower layers. Timer duration 3 sec*/
      uehStartTmr(UE_TMR_INTERNAL_CONF,&((UehUeCb *)ueCb)->ueInternalConfTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
      
      return uehProcInitCtxtSetupInUeCntd( ueCb, pdu );

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntSztUeCtxRel
 *       Desc:  Handles S1AP Ue Context Release Event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntSztUeCtxRel(
  IO UehUeCb  *ueCb,
  IO S1apPdu  *pdu,
  IO U8       ueEvt
  )
{
  U16 ueIdx = 0;

  ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
  switch( ueCb->ueState )
  {
    case UE_STATE_CONNECTED:
    case UE_STATE_SMC_ONGOING:
    case UE_STATE_REC_ONGOING:
    case UE_STATE_CALL_CONNECTED:
    case UE_STATE_UE_CAP_ONGOING:
    /* UE_TMR_UE_CTX_REL Timer is started when UE Release Request is initiated from ENB and it is stoppped when ENB receive the UE Context Release Command from MME in response to UE Release Request*/
    if (ueCb->ueProcedure == UEH_UE_REL_REQ_SENT)
    {
      uehStopTmr(UE_TMR_UE_SRB1,&((UehUeCb *)ueCb)->ueSrb1Tmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti );
    }
    if (UEH_INITIAL_CXT_SETUP_FAILURE == ueCb->ueProcedure)
    {
      uehStopTmr(UEH_TMR_WAIT_MME_REL,&((UehUeCb *)ueCb)->mmeRelIndTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti );
    }

    if (UEH_INITIAL_CXT_SETUP == ueCb->ueProcedure)
    {
    //  uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueFailureEvt = EVENTTYPE_INITCONTEXTSETUPREQ;
    }
    else if(UEH_INITIAL_UE_MSG == ueCb->ueProcedure)
    {
    //  uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueFailureEvt = EVENTTYPE_INITIALUEMSG;
    }
    return uehProcUeCtxRelCmd( ueCb, pdu );

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

static U8 uehHndlEvntSztDlNasTransport(
  IO UehUeCb  *ueCb,
  IO S1apPdu  *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_S1_CONNECTING:
      return uehProcDlNasInS1Con( ueCb, pdu );
    case UE_STATE_CONNECTED:
    case UE_STATE_SMC_ONGOING:
    case UE_STATE_UE_CAP_ONGOING:
    case UE_STATE_REC_ONGOING:
    case UE_STATE_CALL_CONNECTED:
    case UE_STATE_REL_ONGOING:
      return uehProcDlNasMsg( ueCb, pdu );

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntOpStopRel
 *       Desc:  Handles OpStop Release Event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntOpStopRel(
  IO UehUeCb  *ueCb,
  IO U8       ueEvt
  )
{
  U16 ueIdx = 0;


  ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
 //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueFailureEvt = EVENTTYPE_OPSTOPRELEASE;
  switch( ueCb->ueState )
  {
    case UE_STATE_RRC_IDLE:
    case UE_STATE_RRC_CONNECTING:
         uehStopTmr(UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
         uehClearCallInRrcConnectingState(ueCb);
         RETVALUE(ROK);
    case UE_STATE_S1_CONNECTING:
      return uehProcOpStopInS1Con(ueCb);
    case UE_STATE_CONNECTED:
    case UE_STATE_SMC_ONGOING:
    case UE_STATE_UE_CAP_ONGOING:
    case UE_STATE_REC_ONGOING:
    case UE_STATE_CALL_CONNECTED:
      return uehProcOpStopInUeCntd(ueCb);

    default:

     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntUeSchCfgCfm
 *       Desc:  Handles UE Scheduler Config Confirm Event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntUeSchCfgCfm(
  IO UehUeCb     *ueCb,
  IO UeRgrCfgCfm *pdu,
  IO U8          ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_RRC_CONNECTING: return uehProcRrcConRgrCfgCfm(ueCb, pdu);
    case UE_STATE_REC_ONGOING:    return uehProcRecOngoingRgrCfgCfm(ueCb, pdu);
    case UE_STATE_REL_ONGOING:    return uehProcRelOngoingRgrCfgCfm(ueCb, pdu);
     case UE_STATE_CALL_CONNECTED: 
      logInfo("%s","UESCHCFGCFM ueId:%d, state:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->ueState, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id); 
      RETVALUE(ROK);
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntUeNhuDataCfm
 *       Desc:  Handles UE Nhu Data Confirm Event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntUeNhuDataCfm(
  IO UehUeCb       *ueCb,
  IO NhuDatCfmSdus *pdu,
  IO U8            ueEvt
  )
{
  if ((NHU_ERRCAUSE_MAX_RLC_TRANS_REACH  EQ  pdu->datCfmStatus) && (ueCb->ueState != UE_STATE_RRC_IDLE)  && (ueCb->ueState != UE_STATE_REL_ONGOING))
  {
	  logError("MAX RLC retransmissions reached. Releasing UE crnti: %d,state: %d eNbs1apId:%d mmeS1apId:%d",ueCb->crnti,ueCb->ueState, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id );
    STOP_UE_TMR_IF_RUNNING((PTR)ueCb,&(ueCb)->ueRspTmr,UE_TMR_UE_RESPONSE,ueCb->crnti);
	  uehProcUeRspTmrExpiryForRlf(ueCb);
    RETVALUE(ROK);
  }

  switch(ueCb->ueState)
  {
    case UE_STATE_REL_ONGOING:
      uehStopTmr(UE_TMR_RRCCONNRELL2ACK,&((UehUeCb *)ueCb)->ueRrcConRelL2Ack,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);  
      logInfo ("Data Cfm Status:%d,UE id:%d eNbs1apId:%d mmeS1apId:%d",pdu->datCfmStatus, ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id );
      /* UE Context deletion  in RLC,MAC and RRC is done whenever */
      if( (NHU_ERRCAUSE_MAX_RLC_TRANS_REACH == pdu->datCfmStatus) || (NHU_ERRCAUSE_PDCP_SDU_DIS_TMR_EXP == pdu->datCfmStatus) )
      {
         UehLiNhuCnclUeReq(&uehCb.nhuSap[0]->pst, uehCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);
      }
      return uehProcRrcDatCfmInUeCtxRel(ueCb);
    
    case UE_STATE_UE_CAP_ONGOING:
      return uehProcRrcDatCfmInUeCapOngoing(ueCb, pdu);

    case UE_STATE_REC_ONGOING:
      return uehProcRrcDatCfmInRecOngoing(ueCb, pdu);


    default:
      RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntS1Clear
 *       Desc:  Handles S1 Clear Event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntS1Clear(
  IO UehUeCb  *ueCb,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_RRC_CONNECTING:
         uehStopTmr(UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
         return uehProcS1ClearInRrcCon(ueCb);
    case UE_STATE_S1_CONNECTING:
      return uehProcS1LinkDownInS1Con(ueCb);
    case UE_STATE_CONNECTED:
    case UE_STATE_SMC_ONGOING:
    case UE_STATE_UE_CAP_ONGOING:
    case UE_STATE_REC_ONGOING:
    case UE_STATE_CALL_CONNECTED:
      return uehProcS1LinkDownInUeCntd(ueCb);
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntUeRspTmrExp
 *       Desc:  Handles UE Response Timer Expiry events based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntUeRspTmrExp(
  IO UehUeCb  *ueCb,
  IO U8       ueEvt
  )
{
  U16 ueIdx = 0;

  ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
  switch( ueCb->ueState )
  {
    case UE_STATE_RRC_CONNECTING:
     // uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueFailureEvt = EVENTTYPE_RRCCONNSETUP;
      
      return uehProcUeRspTmrExpiryInRrcCon(ueCb);
    /** This timer is now started for RRCConnectionSetup. It thus cannot expire in other states */
    case UE_STATE_SMC_ONGOING:
    case UE_STATE_UE_CAP_ONGOING:
      if (UE_STATE_SMC_ONGOING)
      {
        //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueFailureEvt = EVENTTYPE_SECMODECOMMAND;
      }
      else
      {
        //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueFailureEvt = EVENTTYPE_UECAPABILITYENQ;
      }
      return uehProcUeRspTmrExpiryAftrS1Conctd(ueCb);
    case UE_STATE_REC_ONGOING:
      //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueFailureEvt = EVENTTYPE_RRCCONNRECONFIG;
      return uehProcUeRspTmrExpiryInRecOngoing(ueCb);
    case UE_STATE_CALL_CONNECTED:
      //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueFailureEvt = EVENTTYPE_RRCCONNRECONFIG;
      logError("TIMER EXPIRY FOR CAPABILITY INFO IN ULINACTIVITY |ueId=%d|eNbs1apId=%d|mmeS1apId=%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
        return uehProcUeRspTmrExpiryForRlf(ueCb);
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntSrb1TmrExp
 *       Desc:  Handles SRB1 Timer Expiry events based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntSrb1TmrExp(
  IO UehUeCb  *ueCb,
  IO U8       ueEvt
  )
{
  SztRelReq      relReq;
  //U16 ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
  switch( ueCb->ueState )
  {
    case UE_STATE_S1_CONNECTING:
    case UE_STATE_CONNECTED:
      /* Delete local context. TODO: This is not adequate for CONNECTED STATE. Need to add handling */
      relReq.relType      = SZT_LOCAL_REL;
      relReq.connIdType   = SZT_CONN_TYPE_SU;
      relReq.connId       = ueCb->s1ConCb.suConnId;
      uehSndMsgOut (ENTSZ, EVENTTYPE_RESERVED1, ueCb, TRANSID_NONE, &relReq);

      //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueFailureEvt = EVENTTYPE_INITIALUEMSG;
      uehStartRrcConnClear( ueCb );
      uehUeProcStart(ueCb,UEH_UE_CONN_REL_ONGOING);
      uehSetState(ueCb, UE_STATE_REL_ONGOING);
      RETVALUE(ROK);

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntRrcConnRelL2AckTmrExp
 *       Desc:  Handles RRC Connection Release L2 Ack Timer Expiry event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntRrcConnRelL2AckTmrExp(
  IO UehUeCb  *ueCb,
  IO U8       ueEvt
  )
{
  U16 ueIdx = 0;
  switch( ueCb->ueState )
  {
    case UE_STATE_REL_ONGOING:
      ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
      return uehProcRrcConRelL2AckTmrExpiry(ueCb);

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntUeCtxRelTmrExp
 *       Desc:  Handles UE Context Release Timer Expiry event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntUeCtxRelTmrExp(
  IO UehUeCb  *ueCb,
  IO U8       ueEvt
  )
{
  U16 ueIdx = 0;
  switch( ueCb->ueState )
  {
    case UE_STATE_CONNECTED        : 
    case UE_STATE_SMC_ONGOING      :      
    case UE_STATE_UE_CAP_ONGOING   :     
    case UE_STATE_REC_ONGOING      :    
    case UE_STATE_CALL_CONNECTED   :   
    case UE_STATE_REL_ONGOING      :
      ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
      //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueFailureEvt = EVENTTYPE_UECONTEXTRELREQ;
      return uehProcCtxRelTmrExpiry(ueCb);

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntMmeRelIndTmrExp
 *       Desc:  Handles MME Release Indication Timer Expiry event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */

static U8 uehHndlEvntMmeRelIndTmrExp(
  IO UehUeCb  *ueCb,
  IO U8       ueEvt
  )
{
  if (ueCb != NULLP)
  {
    switch( ueCb->ueState )
    {
      case UE_STATE_CONNECTED:
      case UE_STATE_CALL_CONNECTED :
      case UE_STATE_SMC_ONGOING :
      case UE_STATE_UE_CAP_ONGOING :
      case UE_STATE_REC_ONGOING :
        return uehProcMmeRelIndTmrExp(ueCb);
        break; 
      default:
      UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
      RETVALUE(RFAILED);
    }
  } 
  RETVALUE(ROK);
}
/**
 *       Fun:   uehHndlEvntUeCrntiRetenTmrExp
 *       Desc:  Handles UE CRNT Retention Timer Expiry event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntUeCrntiRetenTmrExp(
  IO UehUeCb  *ueCb,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_RRC_IDLE:
    case UE_STATE_RRC_CONNECTING:
      return uehProcUeCrntiRetTmrExpiry(ueCb);

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

static U8 uehHndlEvntInternalConfTmrExp(
  IO UehUeCb *ueCb,
  IO U8      ueEvt
  )
{
  U16 ueIdx = 0;

  ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
  switch(ueCb->ueState)
  {
     case UE_STATE_REC_ONGOING    :
     case UE_STATE_S1_CONNECTING  :  
     case UE_STATE_CONNECTED      :
     case UE_STATE_SMC_ONGOING    : 
     case UE_STATE_UE_CAP_ONGOING :
        //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueFailureEvt = EVENTTYPE_INITCONTEXTSETUPREQ;
        return uehProcInternalReconfTmrExpiry(ueCb);
     default:
      UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
      RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntSztErrInd
 *       Desc:  Handles Sxt Error Indication event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntSztErrInd(
  IO UehUeCb  *ueCb,
  IO S1apPdu  *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_S1_CONNECTING:
    case UE_STATE_CONNECTED:
    case UE_STATE_SMC_ONGOING:
    case UE_STATE_UE_CAP_ONGOING:
    case UE_STATE_REC_ONGOING:
    case UE_STATE_CALL_CONNECTED:
      return uehProcS1ErrInd( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntEgtTunnelMgmtCfm
 *       Desc:  Handles Egt Tunnel Management Config event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntEgtTunnelMgmtCfm(
  IO UehUeCb  *ueCb,
  IO EgtUEvnt *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_REC_ONGOING:   return uehProcEgtpTunnelCfmInRecOngoing( ueCb, pdu );
    case UE_STATE_REL_ONGOING:   return uehProcEgtpTunnelCfmInRelOngoing( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntUeCapEnqInfo
 *       Desc:  Handles UE Capability enquiry Information event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntUeCapEnqInfo(
  IO UehUeCb       *ueCb,
  IO NhuDatIndSdus *pdu,
  IO U8            ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_UE_CAP_ONGOING:
         return uehProcUeCapEnqInfo( ueCb, pdu );
    case UE_STATE_CALL_CONNECTED:
         uehStopTmr(UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
         /* No need to decode the ue capability info, because this is used only to detect the ul inactivity*/
         RETVALUE(ROK);

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntRrcEncCfm
 *       Desc:  Handles RRC Encode Confirm event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntRrcEncCfm(
  IO UehUeCb       *ueCb,
  IO NhuEncCfmSdus *pdu,
  IO U8            ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_UE_CAP_ONGOING:
      return uehProcUeCapEncCfm( ueCb, pdu );
    case UE_STATE_HANDOVER_ONGOING:
      return uehProcHovrPrepInfoEncCfm( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntRrcDecCfm
 *       Desc:  Handles RRC Decode Confirm event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntRrcDecCfm(
  IO UehUeCb         *ueCb,
  IO NhuDecodeSduCfm *pdu,
  IO U8              ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_UE_CAP_ONGOING:
    case UE_STATE_CALL_CONNECTED:
      return uehProcUeCapDecCfm( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntEgtErrInd
 *       Desc:  Handles EGTP Error Indication event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntEgtErrInd(
  IO UehUeCb  *ueCb,
  IO EgtUEvnt *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_CALL_CONNECTED:
      return uehProcEgtpErrIndInCallCntd(ueCb, pdu);
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntSztUeLocReportCntrl
 *       Desc:  Handles UE Loc Report Control event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntSztUeLocReportCntrl(
  IO UehUeCb  *ueCb,
  IO S1apPdu  *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_CONNECTED:
    case UE_STATE_SMC_ONGOING:
    case UE_STATE_UE_CAP_ONGOING:
    case UE_STATE_REC_ONGOING:
    case UE_STATE_CALL_CONNECTED:
      return uehProcLocReport( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntSztTraceStart
 *       Desc:  Handles Trace Start event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntSztTraceStart(
  IO UehUeCb  *ueCb,
  IO S1apPdu  *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_CONNECTED:
    case UE_STATE_SMC_ONGOING:
    case UE_STATE_UE_CAP_ONGOING:
    case UE_STATE_REC_ONGOING:
    case UE_STATE_CALL_CONNECTED:
      return uehProcTraceStart( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntSztDeactivateTrace
 *       Desc:  Handles Deactivate Trace event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntSztDeactivateTrace(
  IO UehUeCb  *ueCb,
  IO S1apPdu  *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_CONNECTED:
    case UE_STATE_SMC_ONGOING:
    case UE_STATE_UE_CAP_ONGOING:
    case UE_STATE_REC_ONGOING:
    case UE_STATE_CALL_CONNECTED:
      return uehProcDeactivateTrace( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntSztErabSetupReq
 *       Desc:  Handles E-Rab Setup Request event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntSztErabSetupReq(
  IO UehUeCb  *ueCb,
  IO S1apPdu  *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_CALL_CONNECTED:
      return uehProcERabSetupReq( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntSztErabModifyReq
 *       Desc:  Handles E-Rab Modify Request event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntSztErabModifyReq(
  IO UehUeCb  *ueCb,
  IO S1apPdu  *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_CALL_CONNECTED:
      return uehProcERabModifyReq( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntSztErabRelReq
 *       Desc:  Handles E-Rab Release Request event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntSztErabRelReq(
  IO UehUeCb  *ueCb,
  IO S1apPdu  *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_CALL_CONNECTED:
      return uehProcERabReleaseReq( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntSztUeCtxModReq
 *       Desc:  Handles UE context Modify Request event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntSztUeCtxModReq(
  IO UehUeCb  *ueCb,
  IO S1apPdu  *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_CALL_CONNECTED:
      return uehProcUeCtxModReq( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntUdhErabAddCfm
 *       Desc:  Handles UEH E-Rab Add Confirm event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntUdhErabAddCfm(
  IO UehUeCb         *ueCb,
  IO UdhUehAddRabCfm *pdu,
  IO U8              ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_REC_ONGOING:
      return uehProcUdhAddRabCfm( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntUdhUeInactivityInd
 *       Desc:  Handles UDH UE Inactivity Indication event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntUdhUeInactivityInd(
  IO UehUeCb  *ueCb,
  IO void     *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_CALL_CONNECTED:
      return uehProcUdhUeInactvInd( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntUdhErabDelCfm
 *       Desc:  Handles UDH E-Rab Delete Confirm event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntUdhErabDelCfm(
  IO UehUeCb         *ueCb,
  IO UdhUehDelRabCfm *pdu,
  IO U8              ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_REL_ONGOING:
      return uehProcUdhRabDelCfm( ueCb, pdu );

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntUdhUeDelCfm
 *       Desc:  Handles UDH UE Delete Confirm event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntUdhUeDelCfm(
  IO UehUeCb         *ueCb,
  IO UdhUehDelUeCfm *pdu,
  IO U8              ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_REL_ONGOING:
      return uehProcUdhUeDelCfm( ueCb, pdu );

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntUeCtfCfgCfm
 *       Desc:  Handles UE CTF Config Confirm event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntUeCtfCfgCfm(
  IO UehUeCb      *ueCb,
  IO UeCtfCfgCfm  *pdu,
  IO U8           ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_RRC_CONNECTING:
      return uehProcCtfCfgCfm( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntUdhUlInactivityInd
 *       Desc:  Handles UDH UL Inactivity Indication event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntUdhUlInactivityInd(
  IO UehUeCb  *ueCb,
  IO void     *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_CALL_CONNECTED:
      return uehProcUdhUlInactivityInd( ueCb, pdu );

    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntRrcCountCfm
 *       Desc:  Handles RRC Count Confirm event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntRrcCountCfm(
  IO UehUeCb            *ueCb,
  IO NhuDRBCountInfoLst *pdu,
  IO U8                 ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_CALL_CONNECTED:
      return uehProcRrcCountCfm( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntUeCounterChkRsp
 *       Desc:  Handles UE Counter Check Response event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntUeCounterChkRsp(
  IO UehUeCb       *ueCb,
  IO NhuDatIndSdus *pdu,
  IO U8            ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_CALL_CONNECTED:
         uehStopTmr(UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
         return uehProcRrcCounterChkRsp( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntSztHandoverCmd
 *       Desc:  Handles Handover Command event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntSztHandoverCmd(
  IO UehUeCb  *ueCb,
  IO S1apPdu  *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_HANDOVER_ONGOING:
      return uehProcHandoverCmd( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntSztHandoverPrepFailed
 *       Desc:  Handles Handover Preparation Failed event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntSztHandoverPrepFailed(
  IO UehUeCb  *ueCb,
  IO S1apPdu  *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    case UE_STATE_HANDOVER_ONGOING:
      return uehProcHandoverPrepFailed( ueCb, pdu );
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   uehHndlEvntSztEncDecCfm
 *       Desc:  Handles Encode Decode Confirm event based on the current state.
 *              Logs Error in case if unexpected event occurs in an unexpected state.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
static U8 uehHndlEvntSztEncDecCfm(
  IO UehUeCb  *ueCb,
  IO S1apPdu  *pdu,
  IO U8       ueEvt
  )
{
  switch( ueCb->ueState )
  {
    default:
     UEH_ERROR_UNEXPECTED_EVENT(ueCb,ueEvt);
     RETVALUE(RFAILED);
  }
}

/**
 *       Fun:   ueFsm
 *       Desc:  Switch Case Based State Machine.
 *              Handles all the events based on the current state.
 *              Logs Error in case of unexpected events.
 *       Ret:   U8
 *       Notes: None
 *       File:  ueh_li.c
 */
PUBLIC U8 ueFsm(
  UehUeCb  *ueCb,
  EventType ueEvt,
  void     *pdu
  )
{
  S16             ret       = ROK;
  UeState         prevState = ueCb->ueState;
  UehUeProcedure  prevProc  = ueCb->ueProcedure;
  uehCb.eventTab.n          = 0; /* mark empty */

  uehRecordUeEvnt( ueCb, ueEvt, pdu,TRUE );

  switch(ueEvt)
  {
    /* Events Received from RRC */
    case EVENTTYPE_RRCCONNREQ:                 ret = uehHndlEvntRrcConnReq(ueCb, (NhuDatIndSdus *)pdu, ueEvt); break;
    case EVENTTYPE_RRCREESTABREQ:              ret = uehHndlEvntRrcConnReEst(ueCb, (NhuDatIndSdus *)pdu, ueEvt); break;
    case EVENTTYPE_RRCCONNRECONFCOMPL:         ret = uehHndlEvntRrcConnReCfgCmpl(ueCb, (NhuDatIndSdus *)pdu, ueEvt); break;
    case EVENTTYPE_RRCCONNSETUPCOMPL:          ret = uehHndlEvntRrcConnSetupCmpl(ueCb, (NhuDatIndSdus *)pdu, ueEvt); break;
    case EVENTTYPE_SECMODECOMPL:               ret = uehHndlEvntRrcConnSecModeCmpl(ueCb, (NhuDatIndSdus *)pdu, ueEvt); break;
    case EVENTTYPE_SECMODEFAIL:                ret = uehHndlEvntRrcConnSecModeFail(ueCb, (NhuDatIndSdus *)pdu, ueEvt); break;
    case EVENTTYPE_ULINFTRANSFER:              ret = uehHndlEvntRrcUlInfoTrnsfr(ueCb, (NhuDatIndSdus *)pdu, ueEvt); break;
    case EVENTTYPE_RRCLICFGCFM:                ret = uehHndlEvntRrcLiCfgCfm(ueCb, (NhuCfgCfmSdus *)pdu, ueEvt); break;
    case EVENTTYPE_RRCERRIND:                  ret = uehHndlEvntRrcErrInd(ueCb, (NhuErrIndSdus *)pdu, ueEvt); break;
    case EVENTTYPE_RRCENCCFM:                  ret = uehHndlEvntRrcEncCfm(ueCb, (NhuEncCfmSdus *)pdu, ueEvt); break;
    case EVENTTYPE_RRCDECCFM:                  ret = uehHndlEvntRrcDecCfm(ueCb, (NhuDecodeSduCfm *)pdu, ueEvt); break;
    case EVENTTYPE_RRCCOUNTCFM:                ret = uehHndlEvntRrcCountCfm(ueCb, (NhuDRBCountInfoLst *)pdu, ueEvt); break;
    case EVENTTYPE_UENHUDATACFM:               ret = uehHndlEvntUeNhuDataCfm(ueCb, (NhuDatCfmSdus *)pdu, ueEvt); break;
    case EVENTTYPE_UECAPABILITYINFO:           ret = uehHndlEvntUeCapEnqInfo(ueCb, (NhuDatIndSdus *)pdu, ueEvt); break;
    case EVENTTYPE_UECOUNTERCHECKRSP:          ret = uehHndlEvntUeCounterChkRsp(ueCb, (NhuDatIndSdus *)pdu, ueEvt); break;
 
    /* Events Received from S1AP */
    case EVENTTYPE_INITCONTEXTSETUPREQ:        ret = uehHndlEvntSztUeCtxSetup(ueCb, (S1apPdu *)pdu, ueEvt); break;
    case EVENTTYPE_UECONTEXTRELCMD:            ret = uehHndlEvntSztUeCtxRel(ueCb, (S1apPdu *)pdu, ueEvt); break;
    case EVENTTYPE_DLNASTRANSPORTMSG:          ret = uehHndlEvntSztDlNasTransport(ueCb, (S1apPdu *)pdu, ueEvt); break;
    case EVENTTYPE_LOCATIONREPORTCTRL:         ret = uehHndlEvntSztUeLocReportCntrl(ueCb, (S1apPdu *)pdu, ueEvt); break;
    case EVENTTYPE_TRACESTART:                 ret = uehHndlEvntSztTraceStart(ueCb, (S1apPdu *)pdu, ueEvt); break;
    case EVENTTYPE_DEACTIVATETRACE:            ret = uehHndlEvntSztDeactivateTrace(ueCb, (S1apPdu *)pdu, ueEvt); break;
    case EVENTTYPE_RABSETUPREQ:                ret = uehHndlEvntSztErabSetupReq(ueCb, (S1apPdu *)pdu, ueEvt); break;
    case EVENTTYPE_RABMODREQ:                  ret = uehHndlEvntSztErabModifyReq(ueCb, (S1apPdu *)pdu, ueEvt); break;
    case EVENTTYPE_RABRELREQ:                  ret = uehHndlEvntSztErabRelReq(ueCb, (S1apPdu *)pdu, ueEvt); break;
    case EVENTTYPE_UECONTEXTMODREQ:            ret = uehHndlEvntSztUeCtxModReq(ueCb, (S1apPdu *)pdu, ueEvt); break;
    case EVENTTYPE_S1APERRIND:                 ret = uehHndlEvntSztErrInd(ueCb, (S1apPdu *)pdu, ueEvt); break;
    case EVENTTYPE_S1APHOCMD:                  ret = uehHndlEvntSztHandoverCmd(ueCb, (S1apPdu *)pdu, ueEvt); break;
    case EVENTTYPE_S1APHOPREPFAILED:           ret = uehHndlEvntSztHandoverPrepFailed(ueCb, (S1apPdu *)pdu, ueEvt); break;
    case EVENTTYPE_S1APENCDECCFM:              ret = uehHndlEvntSztEncDecCfm(ueCb, (S1apPdu *)pdu, ueEvt); break;

    /* Events for Timer expiry */
    case EVENTTYPE_RRCCONNRELL2ACKTMREXP:      ret = uehHndlEvntRrcConnRelL2AckTmrExp(ueCb, ueEvt); break;
    case EVENTTYPE_UERESTMREXP:                ret = uehHndlEvntUeRspTmrExp(ueCb, ueEvt); break;
    case EVENTTYPE_UECTXRELTMREXP:             ret = uehHndlEvntUeCtxRelTmrExp(ueCb, ueEvt); break;
    case EVENTTYPE_UECRNTIRETENTMREXP:         ret = uehHndlEvntUeCrntiRetenTmrExp(ueCb, ueEvt); break;
    case EVENTTYPE_SRB1TMREXP:                 ret = uehHndlEvntSrb1TmrExp(ueCb, ueEvt); break;
    case EVENTTYPE_MMERELINDTMREXP:            ret = uehHndlEvntMmeRelIndTmrExp(ueCb, ueEvt); break;
    case EVENTTYPE_INTCONFTMREXP :             ret = uehHndlEvntInternalConfTmrExp(ueCb, ueEvt); break;

    /* Events Received from RGR/CTF */
    case EVENTTYPE_UESCHCFGCFM:                ret = uehHndlEvntUeSchCfgCfm(ueCb, (UeRgrCfgCfm *)pdu, ueEvt); break;
    case EVENTTYPE_UECTFCFGCFM:                ret = uehHndlEvntUeCtfCfgCfm(ueCb, (UeCtfCfgCfm *)pdu, ueEvt); break;

    /* Events received from CELL-CRM */
    case EVENTTYPE_OPSTOPRELEASE:              ret = uehHndlEvntOpStopRel(ueCb, ueEvt); break;
    case EVENTTYPE_S1CLEAR:                    ret = uehHndlEvntS1Clear(ueCb, ueEvt); break;

    /* Events Received from GTPU  */
    case EVENTTYPE_EGTPTUNNELMGMTCFM:          ret = uehHndlEvntEgtTunnelMgmtCfm(ueCb, (EgtUEvnt *)pdu, ueEvt); break;
    case EVENTTYPE_EGTERRIND:                  ret = uehHndlEvntEgtErrInd(ueCb, (EgtUEvnt *)pdu, ueEvt); break;
    /* Events Received from UDH */
    case EVENTTYPE_UDHERABADDCFM:              ret = uehHndlEvntUdhErabAddCfm(ueCb, (UdhUehAddRabCfm *)pdu, ueEvt); break;
    case EVENTTYPE_UDHERABDELCFM:              ret = uehHndlEvntUdhErabDelCfm(ueCb, (UdhUehDelRabCfm *)pdu, ueEvt); break;
    case EVENTTYPE_UDHDELUECFM:                ret = uehHndlEvntUdhUeDelCfm(ueCb, (UdhUehDelUeCfm *)pdu, ueEvt); break;
    case EVENTTYPE_UDHUEINACTVIND:             ret = uehHndlEvntUdhUeInactivityInd(ueCb, pdu, ueEvt); break;/* Presently pdu was not used */
    case EVENTTYPE_UDHULINACTVIND:             ret = uehHndlEvntUdhUlInactivityInd(ueCb, pdu, ueEvt); break; /* Presently pdu was not used */

    default:
      UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("Fsm Event", ueEvt, "UEH FSM Events"); 
      RETVALUE(RFAILED);
  }
  /* internal ue Id, crnti, enbS1apId, mmeS1apId, state, proc, event, new state, new proc */
  logStateChange ("%ld | %d | %ld | %ld | %s | %s | %s | %s | %s ",
    ueCb->enbUeInternalId,
    ueCb->crnti,
    ueCb->s1ConCb.enb_ue_s1ap_id,
    ueCb->s1ConCb.mme_ue_s1ap_id,
    uehGetUeStateName(prevState),
    uehGetUeProcedureName(prevProc),
    uehGetEventName(ueEvt),
    uehGetUeStateName(ueCb->ueState),
    uehGetUeProcedureName(ueCb->ueProcedure));

  /* The intent is to print all events have went out when this came in. */
  {
    S16 i = 0;
    while(i < uehCb.eventTab.n)
    {
      logStateChange(" %s (%d)", uehGetEventName(uehCb.eventTab.val[i]), uehCb.eventTab.val[i]);
      i++;
    }
  }

  RETVALUE(ret);
}

char * getS1Cause (const UehUeCb *ueCb)
{ 
  switch (ueCb->s1ConCb.s1CauseType)
  {
    case CauseType_RadioNw :
      switch (ueCb->s1ConCb.s1Cause)
      {
        case SztCauseRadioNwunspecifiedEnum: 
              return "RadioNwunspecified";
              break;

        case  SztCauseRadioNwtx2relocoverall_expiryEnum: 
              return "Radio Network tx2relocoverall expiry";
              break;

        case  SztCauseRadioNwsuccessful_handoverEnum: 
              return "Radio Network successful handover";
              break;

        case  SztCauseRadioNwrelease_due_to_eutran_generated_reasonEnum: 
              return "Radio Network release due to eutran generated reason";
              break;

        case  SztCauseRadioNwhandover_cancelledEnum: 
              return "Radio Network handover cancelled";
              break;

        case  SztCauseRadioNwpartial_handoverEnum: 
              return "Radio Network partial handover";
              break;

        case  SztCauseRadioNwho_failure_in_target_EPC_eNB_or_target_systemEnum: 
              return "Radio Network HO failure in target EPC eNB or target system";
              break;
 
        case  SztCauseRadioNwho_target_not_allowedEnum: 
              return "Radio Network HO target not allowed";
              break;

        case  SztCauseRadioNwtS1relocoverall_expiryEnum: 
              return "Radio Network tS1relocoverall expiry";
              break;

        case  SztCauseRadioNwtS1relocprep_expiryEnum: 
              return "Radio Network tS1relocprep expiry";
              break;

        case  SztCauseRadioNwcell_not_availableEnum: 
              return "Radio Network cell not available";
              break;

        case  SztCauseRadioNwunknown_targetIDEnum: 
              return "Radio Network unknown targetID";
              break;

        case  SztCauseRadioNwno_radio_resources_available_in_target_cellEnum: 
              return "Radio Network no radio resources available in target cell";
              break;

        case  SztCauseRadioNwunknown_mme_ue_s1ap_idEnum: 
              return "Radio Network unknown mme ue s1ap id";
              break;

        case  SztCauseRadioNwunknown_enb_ue_s1ap_idEnum: 
              return "Radio Network unknown enb ue s1ap id";
              break;

        case  SztCauseRadioNwunknown_pair_ue_s1ap_idEnum: 
              return "Radio Network unknown pair ue s1ap id";
              break;

        case  SztCauseRadioNwhandover_desirable_for_radio_reasonEnum: 
              return "Radio Network handover desirable for radio reason";
              break;

        case  SztCauseRadioNwtime_critical_handoverEnum: 
              return "Radio Network time critical handover";
              break;

        case  SztCauseRadioNwresource_optimisation_handoverEnum: 
              return "Radio Network resource optimisation handover";
              break;

        case  SztCauseRadioNwreduce_load_in_serving_cellEnum: 
              return "Radio Network reduce load in serving_cell";
              break;

        case  SztCauseRadioNwuser_inactivityEnum: 
              return "RadioNwuser inactivity";
              break;

        case  SztCauseRadioNwradio_connection_with_ue_lostEnum: 
              return "Radio NWradio connection with ue lost";
              break;

        case  SztCauseRadioNwload_balancing_tau_requiredEnum: 
              return "Radio Nw load balancing tau required";
              break;

        case  SztCauseRadioNwcs_fallback_triggeredEnum: 
              return "Radio Nw cs Fallback triggered";
              break;

        case  SztCauseRadioNwue_not_available_for_ps_serviceEnum: 
              return "RadioNwue not available for ps service";
              break;

        case  SztCauseRadioNwradio_resources_not_availableEnum: 
              return "RadioNwradio resources not available";
              break;

        case  SztCauseRadioNwfailure_in_radio_interface_procedureEnum: 
              return "RadioNwfailure in radio interface procedure";
              break;

        case  SztCauseRadioNwinvalid_qos_combinationEnum: 
              return "RadioNwinvalid qos combination";
              break;

        case  SztCauseRadioNwinterrat_redirectionEnum: 
              return "RadioNwinterrat redirection";
              break;

        case  SztCauseRadioNwinteraction_with_other_procedureEnum: 
              return "RadioNwinteraction with other procedure";
              break;

        case  SztCauseRadioNwunknown_E_RAB_IDEnum: 
              return "RadioNwunknown ERAB ID";
              break;
 
        case  SztCauseRadioNwmultiple_E_RAB_ID_instancesEnum: 
              return "RadioNwmultiple ERAB ID instances";
              break;
 
        case  SztCauseRadioNwencryption_and_or_integrity_protection_algorithms_not_supportedEnum: 
              return "Integrity protection algorithms not supported";
              break;

        case  SztCauseRadioNws1_intra_system_handover_triggeredEnum: 
              return "RadioNws1 intra system handover triggered";
              break;
 
        case  SztCauseRadioNws1_inter_system_handover_triggeredEnum: 
              return "RadioNws1 inter system handover triggered";
              break;

        case  SztCauseRadioNwx2_handover_triggeredEnum : 
              return "RadioNwx2 handover triggered";
              break;

        case SztCauseRadioNwredirection_towards_1xRTTEnum:
              return "Radio NW Redirection towards 1xrtt";
              break;

        case SztCauseRadioNwnot_supported_QCI_valueEnum:
             return "Radio Nw Not supported QCI value";
             break;

        default:
             UEH_ERROR_ELMNT_OUT_OF_RANGE("s1 Cause", ueCb->s1ConCb.s1Cause, SztCauseRadioNwunspecifiedEnum, SztCauseRadioNwnot_supported_QCI_valueEnum);
             break;
      }
      break;

    case CauseType_Transport:
      switch (ueCb->s1ConCb.s1Cause)
      {
        case SztCauseTporttransport_resource_unavailableEnum: 
             return "transport resource unavailable";
             break;

        case SztCauseTportunspecifiedEnum: 
             return "Tportunspecified";
             break;

        default:
           UEH_ERROR_ELMNT_OUT_OF_RANGE("s1 cause", ueCb->s1ConCb.s1Cause, SztCauseTporttransport_resource_unavailableEnum, SztCauseTportunspecifiedEnum);
           
          break;
    }
    break;

  case CauseType_Nas:
    switch (ueCb->s1ConCb.s1Cause)
    {
      case  SztCauseNasnormal_releaseEnum: 
            return "Nasnormal release";
            break;

      case  SztCauseNasauthentication_failureEnum: 
            return "Nasauthentication failure";
            break;

      case  SztCauseNasdetachEnum: 
            return "Nas detach";
            break;

        case  SztCauseNasunspecifiedEnum: 
              return "Nas unspecified";
              break;
              
        default:
          UEH_ERROR_ELMNT_OUT_OF_RANGE ("s1 cause", ueCb->s1ConCb.s1Cause, SztCauseNasnormal_releaseEnum, SztCauseNasunspecifiedEnum);
          break;
      }
      break;

    case CauseType_Prototcol:
      switch (ueCb->s1ConCb.s1Cause)
      {
        case SztCauseProttransfer_syntax_errorEnum: 
              return "Prottransfer syntax error";
              break;

        case SztCauseProtabstract_syntax_error_rejectEnum: 
              return "Protabstract syntax error reject";
              break;

        case SztCauseProtabstract_syntax_error_ignore_and_notifyEnum: 
              return "Protabstract syntax error ignore and notify";
              break;

        case SztCauseProtmessage_not_compatible_with_receiver_stateEnum: 
              return "Protmessage not compatible with receiver state";
              break;

        case SztCauseProtsemantic_errorEnum: 
              return "Protsemantic error";
              break;

        case SztCauseProtabstract_syntax_error_falsely_constructed_messageEnum: 
              return "Protabstract syntax error falsely constructed message";
              break;

        case SztCauseProtunspecifiedEnum: 
              return "Protunspecified";
              break;
              
        default:
          UEH_ERROR_ELMNT_OUT_OF_RANGE("s1 cause", ueCb->s1ConCb.s1Cause, SztCauseProttransfer_syntax_errorEnum, SztCauseProtunspecifiedEnum);          
          break;
      }
      break;

    case CauseType_Misc:
      switch (ueCb->s1ConCb.s1Cause)
      {
        case SztCauseMisccontrol_processing_overloadEnum: 
              return "Misccontrol processing overload";
              break;

        case SztCauseMiscnot_enough_user_plane_processing_resourcesEnum: 
              return "Miscnot enough user plane processing resources";
              break;

        case SztCauseMischardware_failureEnum: 
              return "Mischardware failure";
              break;

        case SztCauseMiscom_interventionEnum: 
              return "Miscom intervention";
              break;

        case SztCauseMiscunspecifiedEnum: 
              return "Miscunspecified";
              break;

        case SztCauseMiscunknown_PLMNEnum: 
              return "Miscunknown PLMN";
              break;

        default:
          UEH_ERROR_ELMNT_OUT_OF_RANGE("s1 cause", ueCb->s1ConCb.s1Cause, SztCauseMisccontrol_processing_overloadEnum, SztCauseMiscunknown_PLMNEnum);
          break;
      }
      break;

      default:
       UEH_ERROR_ELMNT_OUT_OF_RANGE("Cause Type", ueCb->s1ConCb.s1CauseType, CauseType_RadioNw, CauseType_Misc);
  }
  return "Unknown ENUM value Found";
}

/** CR_28Sep: collection of what APIs should be there. Further discussion needed.

  UE Event History APIs:
 
  getAllEvents() - returns all events stored
  getAllFailureEvents() - returns all failure events, including ueId
  getAllEventsForUe(iueId) - returns all events for ueId
  getFailureEventsForUe(iueId) - returns all failure events for ueId

  For object Proc:
  getAllProcs() - returns all procedures {iueId, procId, S|F|NE}[]
  getAllFailedProcs() - returns all procedures that failed - {iueId, procId, S|F|NE}[]
  getAllProcsForUe(ueId) - returns all procedures for ueId - {iueId, procId, S|F|NE}[]
  getAllFailedProcsForUe(ueId) - returns all procedures that failed for ueId - {ueId, procId, S|F|NE}[]

  For object UE:
  getAllUes() - returns UEs which entered ENB {iueId, cnrti, e-s1apId, m-s1apId, S|R}[]
  getAllFailedUes() - returns all UEs for which had a failure - {iueId}[]

  Implementation:
  getAllEvents() - returns all events stored
  getAllFailureEvents() - returns all failure events {iueId, eventId, cause}[]

  getAllEventsForUe(iueId) - returns all events for ueId
  getFailureEventsForUe(iueId) - returns all failure events for ueId
  getAllUes() - return the history table as is; no processing is needed.
  getAllFailedUes() - return all ues for which the following is true: if (hasFailureEvent(ue, eventHistoryTab))
  
*/
/**
 *       Fun:   uehRecordUeEvnt
 *       Desc:  Records the events and their details.
 *              Updates the counters corresponding to each event which is asked to be recoeded.
 *              Checks whether an event should be recorded or not by using funcyion isEventToBeAddedToHistory( ).
 *              if TRUE,
 *                Allocate and fill the event into historyTable.   
 *              else
 *                Do not record.
 *       Ret:   None 
 *       Notes: None
 *       File:  ueh_li.c
 */

void uehRecordUeEvnt( const UehUeCb *ueCb, EventType ueEvt , void *sdu,TBOOL isIncoming)
{
  UeEventHistory    *ueEventHistory = NULL; 
  /* Update UEH Counters */
  uehCb.uehCounters[ueEvt]++;
  if(isEventToBeAddedToHistory(ueEvt,ueCb))
  {
    ueEventHistory = allocUeHistoryElement( );
    uehFillEventDetail(ueEvt,ueEventHistory,sdu,ueCb);
  }  
      
 } 

/** The func allocUeHistoryElement( ) allocates the event into HistoryTable,
 *  which is a circular buffer
 */

void *allocUeHistoryElement( )
{
   UehUeHisTbl    *ueHistoryTable = getUeHistoryTable();
   UeEventHistory *ueEventHistory = &(ueHistoryTable->ueHis[ueHistoryTable->ueHisCurrIdx]);
   clearBytes(ueEventHistory, sizeof(UeEventHistory));
   
   ueHistoryTable->ueHisCurrIdx = (ueHistoryTable->ueHisCurrIdx + 1) % MAX_ROWS_IN_UEEVENTHISTORY_TABLE;

   if(ueHistoryTable->totalEvtInUeEvtHisTbl NOTEQ MAX_ROWS_IN_UEEVENTHISTORY_TABLE)
    ueHistoryTable->totalEvtInUeEvtHisTbl++;
  
  return ueEventHistory;
}
/** The func isEventToBeAddedToHistory( ) Checks whether an event should be recorded or not,
    It provides required filtering of the events **/

TBOOL isEventToBeAddedToHistory(EventType ueEvt,const UehUeCb *ueCb)
{
  if ((ueCb EQ NULL) && (ueEvt NOTEQ EVENTTYPE_RRCREESTABREJ))
  {
    return FALSE;
  }

  switch(ueEvt)
  {
    case EVENTTYPE_RRCCONNRECONFIG:    if(ueCb->ueProcedure EQ UEH_PUCCH_PWR_REC_ONGOING) return FALSE;
    case EVENTTYPE_RRCCONNRECONFCOMPL: if(ueCb->ueProcedure EQ UEH_PUCCH_PWR_REC_ONGOING) return FALSE;
    case EVENTTYPE_PAGING:              return FALSE;
    case EVENTTYPE_RRCDECCFM:           return FALSE;
    case EVENTTYPE_UNKNOWN:             return FALSE;
    case EVENTTYPE_OTHER:               return FALSE;             
    /* Events which we do not want to record. List then Above */
    default: return TRUE;
  }
  return TRUE;
}    
/** uehFillEventDetail( ):It fills event Details of an event **/
    

void uehFillEventDetail(
  EventType         ueEvt,
  UeEventHistory    *ueEventHistory,
  void              *sdu,
  const UehUeCb     *ueCb
  )

{
  if (ueCb)
  {
    ueEventHistory->cRnti  = ueCb->crnti;
  }
  else if (ueEvt EQ EVENTTYPE_RRCREESTABREJ)
  {
    ueEventHistory->cRnti  = ((NhuDatRspSdus *)sdu)->sdu.m.dlCcchMsg.tmpCrnti;
  }
  else 
  {
    logError("%s","This code should never hit");
    return;  
  }
  
  ueEventHistory->eventType = ueEvt;
  fillDataAndTimeStampField(&(ueEventHistory->eventTimestamp[0]));
 /* if (ueCb && (1 EQ ueCb->isSTmsi))
  {
      ueEventHistory->mmec   = ueCb->sTmsi.mmeCode;
      ueEventHistory->mTmsi  = ueCb->sTmsi.mTmsi;
  }
*/
  switch(ueEvt)
  {
	  case EVENTTYPE_RRCCONNREQ:
         snprintf((char*)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,
           "Est Cause %d rnti is temporary",
				  ((int )((NhuDatIndSdus *)sdu)->sdu.m.ulCcchMsg.ccchMsg.message.val.c1.val.rrcConRqst.criticalExtns.val.rrcConRqst_r8.establishmentCause.val)
                 );
		break; 
    
    case EVENTTYPE_RRCREESTABREQ:
         ueEventHistory->cRnti= ((int)((NhuDatIndSdus *)sdu)->sdu.m.ulCcchMsg.tmpCrnti);
         snprintf((char*)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,"%s","UE->ENB");
      
    break;
    case EVENTTYPE_RRCCONNRECONFCOMPL:
    case EVENTTYPE_RRCCONNSETUPCOMPL:
    case EVENTTYPE_SECMODECOMPL:
    case EVENTTYPE_SECMODEFAIL: 
    case EVENTTYPE_ULINFTRANSFER:         
    case EVENTTYPE_UECOUNTERCHECKRSP:        
    case EVENTTYPE_UECAPABILITYINFO:
         snprintf((char*)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,"%s","UE->ENB");
    break;
         
    case EVENTTYPE_RRCLICFGCFM:
    if (((NhuCfgCfmSdus *)sdu)->sdu.isCfgSuccess != TRUE)
      snprintf((char *)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,
          "RrcCfgFail|Cause=%d|Pr=%d|St=%d", 
          ((NhuCfgCfmSdus *)sdu)->sdu.cfgError.errCause,
          ueCb->ueProcedure,
          ueCb->ueState
        );
    break;

    case EVENTTYPE_RRCERRIND:
    snprintf((char *)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,
        "RrcErr=%d|Cause=%d|Pr=%d|St=%d", 
        ((NhuErrIndSdus *)sdu)->sdu.errType,
        ((NhuErrIndSdus *)sdu)->sdu.errCause,
        ueCb->ueProcedure,
        ueCb->ueState
      );
    break;

	  case EVENTTYPE_INITCONTEXTSETUPREQ:
    case EVENTTYPE_UECONTEXTRELCMD:         
    case EVENTTYPE_DLNASTRANSPORTMSG:       
    case EVENTTYPE_LOCATIONREPORTCTRL:   
    case EVENTTYPE_TRACESTART:      
    case EVENTTYPE_DEACTIVATETRACE:  
    case EVENTTYPE_RABSETUPREQ:     
    case EVENTTYPE_RABMODREQ:     
    case EVENTTYPE_RABRELREQ:       
    case EVENTTYPE_UECONTEXTMODREQ:     
         snprintf((char*)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,"%s","MME->ENB");
    break;

    case EVENTTYPE_S1APERRIND:
         snprintf((char*)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,"%s","MME->ENB : SZT ERR IND"); 
    break;
     
    case EVENTTYPE_RRCCONNRELL2ACKTMREXP:
    case EVENTTYPE_UERESTMREXP:
    case EVENTTYPE_UECTXRELTMREXP:
    case EVENTTYPE_SRB1TMREXP:
    case EVENTTYPE_MMERELINDTMREXP:
    case EVENTTYPE_INTCONFTMREXP:
         snprintf((char*)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL," %s", "Timer Expiry"); 
    break;

  
    case EVENTTYPE_UESCHCFGCFM:
		  if (((UeRgrCfgCfm *)sdu)->status != ROK)
			  snprintf((char *)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,
					  "SchCfgFail|Cause=%d|Pr=%d|St=%d", 
					  ((UeRgrCfgCfm *)sdu)->status,
					  ueCb->ueProcedure,
					  ueCb->ueState
				  );
		break;

	  case EVENTTYPE_UECTFCFGCFM:
    		  if (((UeCtfCfgCfm *)sdu)->status != CTF_CFG_CFM_OK)
	 	    	  snprintf((char*)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,
				          	  "CtfCfgFail|Cause=%d|Pr=%d|St=%d", 
					            ((UeCtfCfgCfm *)sdu)->status,
          					  ueCb->ueProcedure,
					            ueCb->ueState
             				  );
		  
    break;
    
    case EVENTTYPE_UDHUEINACTVIND:
    case EVENTTYPE_UDHULINACTVIND:
    break;

    case EVENTTYPE_RRCREESTABREJ:
         snprintf((char*)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,"%s","ENB->UE");
         ueEventHistory->cRnti = ((int)((NhuDatRspSdus *)sdu)->sdu.m.dlCcchMsg.tmpCrnti); 

    break; 
    
    case EVENTTYPE_RRCCONNRELEASE :
         snprintf((char*)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,"ENB->UE relCause:CB:%ld|sdu=%d",
         ueCb->ueReleaseCauseVal,((int)((NhuDatReqSdus *)sdu)->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg.message.val.c1.val.rrcConRls.criticalExtns.val.c1.val.rrcConRls_r8.releaseCause.val));

    break;     
	  
    case EVENTTYPE_RRCCONNRECONFIG :
    case EVENTTYPE_RRCCOUNTERCHECKREQ :
    case EVENTTYPE_DLINFTRANSFER :
    case EVENTTYPE_RRCCONNSETUP : 
    case EVENTTYPE_RRCCONNREJ :
    case EVENTTYPE_UECAPABILITYENQ : 
         snprintf((char*)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,"%s","ENB->UE");
    break;
    
    case EVENTTYPE_S1APLOCALERRIND:
         snprintf((char*)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,"%s",getS1Cause(ueCb));
	  break;

	  case EVENTTYPE_SECMODECOMMAND : 
         snprintf((char *)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,
				  "SecAlgo=%s |Pr=%d|St=%d",((1 EQ ueCb->secCfg.ciphAlgoSelected) ? "SNOW3G" :
					(2 EQ ueCb->secCfg.ciphAlgoSelected) ? "AES" : "NULL" ),
				  ueCb->ueProcedure,
				  ueCb->ueState
			  );
		break;
      
    case EVENTTYPE_UECAPABILITYINFOIND :
    case EVENTTYPE_INITCONTEXTSETUPRESP :
    case EVENTTYPE_ULNASTRANSPORTMSG :
    case EVENTTYPE_DLNASNONDELIVERYIND :
    case EVENTTYPE_TRACEFAILUREIND :
    case EVENTTYPE_DEACTIVATETRACEFAILUREIND :
    case EVENTTYPE_RABRELCOMPL :
    case EVENTTYPE_UECONTEXTRELREQ : 
    case EVENTTYPE_UECONTEXTMODFAIL :
    case EVENTTYPE_UECONTEXTMODRESP :
    case EVENTTYPE_LOCATIONREPORTFAILUREIND :
    case EVENTTYPE_LOCATIONREPORTRES :
    case EVENTTYPE_RABMODFAIL :  
    case EVENTTYPE_INITIALUEMSG :
    case EVENTTYPE_UECONTEXTRELCOM :
         snprintf((char*)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,"%s","ENB->MME");
    break;


	  case EVENTTYPE_INITCONTEXTSETUPFAIL:
         snprintf((char*)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,"ENB->UE %s",getS1Cause(ueCb));
    break;

         
	  case EVENTTYPE_UEDELETED:
        snprintf((char *)&ueEventHistory->eventDetail[0], MAX_SIZE_UEEVENTHISTORY_EVENTDETAIL,
				  "Cause=%s|Pr=%d|St=%d", 
				  getS1Cause(ueCb),
				  ueCb->ueProcedure,
             ueCb->ueState
            );
    break;

    case EVENTTYPE_PAGING:
        /* TODO */
    break;  

    default :
        logInfo("There is no handling for the event = %s currently",uehGetEventName(ueEvt));
    break;
    }
}



/**************************************************************
 Following functions handles the Events received from RRC Layer at UEH,
 Receive the event, extract the UE and invoke the UE State Machine
    NHU Interface
***************************************************************/
/*
*
*       Fun:   UehLiNhuBndCfm - NHU Bind Cfm
*
*       Desc:  This function is used by to present Bind Cfm to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/

PUBLIC S16 UehLiNhuBndCfm(
    Pst     *pst,
    SuId    suId,
    U8      status
  )
{
  logFunc("%s", "UehLiNhuBndCfm");


  /* Send alarm */
  if (CM_BND_OK == status)
  {
    logInfo("UehLiNhuBndCfm: NHU Bind Confirm primitive status: %u ", status);
    uehStopTmr(UEH_TMR_NHU_SAP_BND,&(((UehLiSapCb *)uehCb.nhuSap[0])->timer),(PTR)uehCb.nhuSap[0],0);
    ((UehLiSapCb *)uehCb.nhuSap[0])->bndRetryCnt = 0;
    uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, LUEH_CAUSE_NHU_SAP_BOUND);
  }
  else
  {
    logError("UehLiNhuBndCfm: NHU Bind Confirm primitive status: %u", status);
    uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
  }

  RETVALUE(ROK);
}



/*
*
*       Fun:   UehLiNhuDatInd - NHU Data Indication
*
*       Desc:  This function is used by to present Data Indication to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiNhuDatInd(
    Pst                 *pst,
    SuId                suId,
    NhuDatIndSdus       *nhuDatIndSdus
  )
{
   int ueId=-1;

   logFunc("%s", "UehLiNhuDatInd");
   if(nhuDatIndSdus->sdu.msgCategory == NHU_MSG_ULCCCH)
   {
     ueId = nhuDatIndSdus->sdu.m.ulCcchMsg.tmpCrnti;
     UECM_INPUT(ueId,UehLiNhuDatInd,RRC, "UL CCCH Data Indication from RRC");
   }
   else
   {
     ueId = nhuDatIndSdus->hdr.ueId;
     UECM_INPUT(ueId,UehLiNhuDatInd,RRC, "UL DCCH Data Indication from RRC");
   }

   /* Call the Dat Ind handler */
 uehHdlNhuDatInd(nhuDatIndSdus);

  RETVALUE(ROK); 
}

/*
*
*       Fun:   UehLiNhuDatCfm - NHU Data Confirm
*
*       Desc:  This function is used by to present Data Confirmation to NHU.
*
*       Ret:   None
*
*       Notes: We are interested in confirmation for the Last message (RRCConnectionRelease)
*       We want to clear the lower layers, only after RRCConnectionRelease is delivered to UE
*       This way we can ensure that there is no packet drop, when we release lower layers
*       File:  ueh_li.c
*
*/

PUBLIC S16 UehLiNhuDatCfm(
    Pst                 *pst,
    SuId                suId,
    NhuDatCfmSdus       *nhDatCfmSdus
  )
{
  UehUeCb       *ueCb = NULLP;
  UehCellCb     *cellCb = NULLP;
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif

  logFunc("%s", "UehLiNhuDatCfm");
  UECM_INPUT(nhDatCfmSdus->hdr.ueId,UehLiNhuDatCfm,RRC,"RRC DATA CONFIRM");

  //logError("NHU Data Confirm|ueId=%d", nhDatCfmSdus->hdr.ueId);

  cellCb = uehGetCellCb();
  if(cellCb == NULLP)
  {
    UEH_ERROR_NO_CB_LOCATED(CELL,  NOT_APPLICABLE, EVTNHUDATCFM ,ENTNH );


#ifdef RRM_PRL_FLAG  
  if(islogApiEnabled())
   {
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhUiNhuDatCfm( nhDatCfmSdus, buff);
     logAPI("RRM - RRC :NxLiNhuDatCfm:nhDatCfmSdus STRUCTURE %s", buff->startAddr);
     buff->numBytesWritten = 0;
   }
#endif
    RETVALUE(RFAILED);
  }
 
  UEH_GET_UECB_FROM_CRNTI(ueCb,nhDatCfmSdus->hdr.ueId);
  if(NULLP == ueCb)
  {
    UEH_ERROR_NO_CB_LOCATED(UE, nhDatCfmSdus->hdr.ueId, EVTNHUDATCFM, ENTNH);   
#ifdef RRM_PRL_FLAG  
  if(islogApiEnabled())
   {    
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhUiNhuDatCfm( nhDatCfmSdus, buff);
     logAPI("RRM - RRC :NxLiNhuDatCfm:nhDatCfmSdus STRUCTURE %s", buff->startAddr);
     buff->numBytesWritten = 0;
   }
#endif
    
    RETVALUE(RFAILED);
  }
#ifdef RRM_PRL_FLAG  
  
  if(islogApiEnabled())
   {
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhUiNhuDatCfm( nhDatCfmSdus, buff);
     logAPI("RRM - RRC :NxLiNhuDatCfm:nhDatCfmSdus STRUCTURE %s", buff->startAddr);
     buff->numBytesWritten = 0;
   }
#endif
    ueFsm(ueCb, EVENTTYPE_UENHUDATACFM, nhDatCfmSdus);
  /* As we are not interested in this SDU, Free the SDU */
  

  RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiNhuStaInd - NHU status Indication
*
*       Desc:  This function is used by to present status indication to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/

PUBLIC S16 UehLiNhuStaInd(
    Pst           *pst,
    SuId           suId,
    NhuStaIndSdus *nhStaIndSdus
  )
{
   logFunc("%s", "UehLiNhuStaInd");
   //UECM_INPUT(nhStaIndSdus->hdr.ueId,UehLiNhuDatCfm,RRC,"RRC STATUS INDICATION");
   logError("Rx=NhuStaInd|ueId=%d|pdcpId=%d|cause=%d|Not Handled",
         nhStaIndSdus->hdr.ueId,
         nhStaIndSdus->pdcpId,
         nhStaIndSdus->cause);
   RETVALUE(ROK);

} /* end of UehLiNhuStaInd */

/*
*
*       Fun:   UehLiNhuErrInd - Error Indication
*
*       Desc:  This function is used to present the 
*              error information to NHU
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiNhuErrInd(
    Pst                 *pst,
    SuId                suId,
    NhuErrIndSdus       *nhErrIndSdus
  )
{
  UehUeCb          *ueCb = NULLP;
  UehCellCb        *cellCb = NULLP;
  String50         details;

  logFunc("%s", "UehLiNhuErrInd");
  snprintf((char *)details, sizeof(String50), "errType=%d,errType=%d", nhErrIndSdus->sdu.errType, nhErrIndSdus->sdu.errCause);

  logError("NHUErrInd|cellId=%d|ueId=%d|transId=%d|errPres=%d|errType=%d|errCause=%d", \
       nhErrIndSdus->hdr.cellId, \
       nhErrIndSdus->hdr.ueId, \
       nhErrIndSdus->hdr.transId, \
       nhErrIndSdus->sdu.errPres, \
       nhErrIndSdus->sdu.errType, \
       nhErrIndSdus->sdu.errCause);

  cellCb = uehGetCellCb();
  if(cellCb == NULLP)
  {
    UEH_ERROR_NO_CB_LOCATED( CELL, NOT_APPLICABLE, EVTNHUERRIND, ENTNH);
    RETVALUE(RFAILED);
  }

  /* Error indication while RRC Con Setup Processing by NH/RRC */
  if (NH_UEID_ZERO EQ nhErrIndSdus->hdr.ueId)
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUERRIND, ENTNH, nhErrIndSdus->sdu.errType, INST_OTHER, nhErrIndSdus->sdu.errCause);
    RETVALUE(RFAILED);
  }
  else
  {
    UEH_GET_UECB_FROM_CRNTI(ueCb,nhErrIndSdus->hdr.ueId);
  }
  if(NULL == ueCb)
  {
    UEH_ERROR_NO_CB_LOCATED(UE, nhErrIndSdus->hdr.ueId, EVTNHUERRIND, ENTNH);
    RETVALUE(RFAILED);
  }

  UECM_INPUT(ueCb->crnti,UehLiNhuErrInd,RRC,details);
  ueFsm(ueCb, EVENTTYPE_RRCERRIND, nhErrIndSdus);

  RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiNhuCfgCfm - Lower layer config confirm
*
*       Desc:  This function is used by to present configuration confirm
*              to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiNhuCfgCfm(
    Pst                 *pst,
    SuId                suId,
    NhuCfgCfmSdus       *nhuCfgCfmSdus
  )
{
  UehCellCb     *cellCb = NULLP;
  UehUeCb       *ueCb = NULLP;

  logFunc("%s", "UehLiNhuCfgCfm");
  UECM_INPUT(nhuCfgCfmSdus->hdr.ueId,UehLiNhuCfgCfm,RRC,"-");

  logInfo("%s", "UehLiNhuCfgCfm: NHU Config Cfm ");

  cellCb = uehGetCellCb();
  if(cellCb == NULLP)
  {
    UEH_ERROR_NO_CB_LOCATED(CELL,  NOT_APPLICABLE, EVTNHUCFGCFM, ENTNH);
    RETVALUE(RFAILED);
  }

  UEH_GET_UECB_FROM_CRNTI(ueCb,nhuCfgCfmSdus->hdr.ueId);
  if(NULL == ueCb)
  {
    UEH_ERROR_NO_CB_LOCATED(UE, nhuCfgCfmSdus->hdr.ueId, EVTNHUCFGCFM, ENTNH);
    RETVALUE(RFAILED);
  }
  ueFsm(ueCb, EVENTTYPE_RRCLICFGCFM, (void *) nhuCfgCfmSdus);

  RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiNhuCellCfgCfm - Cell Configuration Confirm
*
*       Desc:  This function is used by to present Cell Config Status Confirm
*              information to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiNhuCellCfgCfm(
    Pst               *pst,
    SuId               suId,
    NhuCellCfgCfmSdus *nhuCellCfgCfmSdus
  )
{
  UehCellCb *cellCb = uehGetCellCb();
  

  logFunc("%s", "UehLiNhuCellCfgCfm");
  UECM_INPUT(nhuCellCfgCfmSdus->hdr.ueId,UehLiNhuCellCfgCfm,RRC,"RRC CELL CONFIGURATION CONFIRM");

  logInfo("%s", "RRC CELL CONFIGURATION CONFIRM RECEIVED");

  if(nhuCellCfgCfmSdus->sdu.isCfgSuccess == FALSE)
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( pst->event, pst->srcEnt, nhuCellCfgCfmSdus->sdu.isCfgSuccess, CELL, nhuCellCfgCfmSdus->hdr.cellId);
    uehSendLmAlarm(LCM_CATEGORY_PROTOCOL, LUEH_EVENT_CELL_CFG_FAILED, nhuCellCfgCfmSdus->sdu.cfgError.errType);
    RETVALUE(RFAILED);
  }

  if(ROK != uehHdlRgrCellCfgReq(cellCb->cellId))
  {
    UEH_ERROR_SSI_EVENT_SEND_FAILED((&uehCb.rgrSap[0]->pst), EVENTTYPE_SCHCELLCONFIG, CELL, cellCb->cellId);
    RETVALUE(RFAILED);
  }
   RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiNhuDecodeCfm - Decode Confirm
*
*       Desc:  This function is used to inform decode confirmation to RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiNhuDecodeCfm(
    Pst                 *pst,
    SuId                suId,
    NhuDecodeSduCfm     *nhuDecodeSduCfm
  )
{
  UehUeCb        *ueCb = NULLP;


  logFunc("%s", "UehLiNhuDecodeCfm");
  UECM_INPUT(nhuDecodeSduCfm->hdr.ueId,UehLiNhuDecodeCfm,RRC,"RRC DECODE CONFIRM");

  logInfo("%s", "UehLiNhuDecodeCfm:NHU Decode Cfm ");

  UEH_GET_UECB_FROM_CRNTI(ueCb,nhuDecodeSduCfm->hdr.ueId);

  if(NULL == ueCb)
  {
    UEH_ERROR_NO_CB_LOCATED(UE, nhuDecodeSduCfm->hdr.ueId, EVTNHUDCODECFM, ENTNH);
    RETVALUE(RFAILED);
  }

  ueFsm(ueCb, EVENTTYPE_RRCDECCFM, (void *) nhuDecodeSduCfm);

 

  RETVALUE(ROK);
} /* end of UehLiNhuDecodeCfm */

/*
*
*       Fun:   UehLiNhuCountCfm - Count Confirm
*
*       Desc:  This function is used to inform COUNT values of DRBs to RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiNhuCountCfm(
    Pst                 *pst,
    SuId                suId,
    NhuDRBCountInfoLst  *nhuDRBCountInfoLst
  )
{
#ifdef COUNTER_CHECK 
  U16            ueIdx = 0;
  UehUeCb        *ueCb = NULLP;
#endif

  logFunc("%s", "UehLiNhuCountCfm");
  UECM_INPUT(nhuDRBCountInfoLst->countHdr.ueId,UehLiNhuCountCfm,RRC,"RRC COUNT CONFIRM");

  logInfo("%s", "UehLiNhuCountCfm: NHU Count Confirm ");
#ifdef COUNTER_CHECK

  UEH_GET_UECB_FROM_CRNTI(ueCb,nhuDRBCountInfoLst->countHdr.ueId);

  if(NULL == ueCb)
  {
    UEH_ERROR_NO_CB_LOCATED( UE, nhuDRBCountInfoLst->countHdr.ueId, EVTNHUCOUNTCFM, ENTNH);
    
    RETVALUE(RFAILED);
  }

  ueFsm(ueCb, EVENTTYPE_RRCCOUNTCFM, (void *) nhuDRBCountInfoLst);
#endif

  RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiNhuEncodeCfm - Encode Confirm
*
*       Desc:  This function is used to inform Encode confirmation to RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiNhuEncodeCfm(
    Pst                 *pst,
    SuId                suId,
    NhuEncCfmSdus       *nhuEncCfmSdus
  )
{
  UehUeCb        *ueCb = NULLP;
  UehCellCb      *cellCb = NULLP;
  logFunc("%s", "UehLiNhuEncodeCfm");
  UECM_INPUT(nhuEncCfmSdus->hdr.ueId,UehLiNhuEncodeCfm,RRC,"RRC ENCODE CONFIRM");

  logInfo("%s", "UehLiNhuEncodeCfm: NHU Encode Confirm ");

  cellCb = uehGetCellCb();
  if(cellCb == NULLP)
  {
    UEH_ERROR_NO_CB_LOCATED( CELL, NOT_APPLICABLE, EVTNHUENCODECFM, ENTNH);
    RETVALUE(RFAILED);
  }

  UEH_GET_UECB_FROM_CRNTI(ueCb,nhuEncCfmSdus->hdr.ueId);
  if(NULL == ueCb)
  {
    UEH_ERROR_NO_CB_LOCATED( UE, nhuEncCfmSdus->hdr.ueId, EVTNHUENCODECFM, ENTNH);
    
    RETVALUE(RFAILED);
  }

  ueFsm(ueCb, EVENTTYPE_RRCENCCFM, (void *) nhuEncCfmSdus);

  RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiNhuPdcpSduStaCfm - PDCP SDU Status Confirm
*
*       Desc:  This function is used to inform PDCP SDU Status confirmation to 
*              RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiNhuPdcpSduStaCfm(
    Pst                 *pst,
    SuId                suId,
    NhuPdcpSduStaCfm    *nhuPdcpSduStaCfm
  )
{

   logFunc("%s", "UehLiNhuPdcpSduStaCfm");
   UECM_INPUT(nhuPdcpSduStaCfm->hdr.ueId,UehLiNhuPdcpSduStaCfm,PDCP,"PDCP STATUS CONFIRM");

   logInfo("%s", "UehLiNhuPdcpSduStaCfm: NHU PDCP Status Confirm ");


   RETVALUE(ROK); 
}

/**************************************************************
  The following functions handles the events from MAc Layer
  RGR Interface
***************************************************************/
/*
*
*       Fun:   UehLiRgrBndCfm - RGR Bind Cfm
*
*       Desc:  This function is used by to present Bind Cfm to RGR.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiRgrBndCfm(
    Pst     *pst,
    SuId    suId,
    U8      status
  )
{
   logFunc("%s", "UehLiRgrBndCfm");

   logInfo("UEH RGR Bind Cfm status: %u ", status);

   pst->event = EVTRGRBNDCFM;
   /* Send alarm */
   if (CM_BND_OK == status)
   {
      uehStopTmr(UEH_TMR_RGR_SAP_BND,&(((UehLiSapCb *)uehCb.rgrSap[0])->timer),(PTR)uehCb.rgrSap[0],0);
      ((UehLiSapCb *)uehCb.rgrSap[0])->bndRetryCnt = 0;
      uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, LUEH_CAUSE_RGR_SAP_BOUND);
   }
   else
   {
      uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
   }

   RETVALUE(ROK);
}

#ifdef RGR_RRM_TICK
/*
*
*       Fun:   UehLiRgrTtiInd
*
*       Desc:  TTI Indication from MAC, Indicate the SIB TMR Event
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/

#if 0
PUBLIC S16 UehLiRgrTtiInd (
    Pst            *pst,
    SuId           suId,
    RgrTtiIndInfo  *ttiInd
)
{
   logFunc("%s", "UehLiRgrTtiInd");

  /* call UEH call trace handler for RRMTTI tick */
  #if 0 
    uehHdlTmrInd(ttiInd); 
  #endif
#ifndef SI_NEW
   uehSibHdlTmrInd(ttiInd);
#endif
   uehPagHdlTmrInd(ttiInd);

   RETVALUE(ROK);
}
#endif
#endif 


/**************************************************************
 * The following functions handles the interface towards PHY
   CTF Interface
***************************************************************/
/*
*
*       Fun:   UehLiCtfBndCfm
*
*       Desc:  Bind Confirm 
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/

PUBLIC S16 UehLiCtfBndCfm (
  Pst       *pst,         
  SuId      suId,        
  U8        status      
  )
{
   logFunc("%s", "UehLiCtfBndCfm");

   /* Send alarm */
   if (CM_BND_OK == status)
   {
      logInfo("%s","BIND OF UEH TO CTF SUCCESSFULL");
      uehStopTmr(UEH_TMR_CTF_SAP_BND,&(((UehLiSapCb *)uehCb.ctfSap[0])->timer),(PTR)uehCb.ctfSap[0],0);
      ((UehLiSapCb *)uehCb.ctfSap[0])->bndRetryCnt = 0;
      uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, LUEH_CAUSE_CTF_SAP_BOUND);
   }
   else
   {
     UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTCTFBNDCFM, ENTTF, status, NOT_APPLICABLE, NOT_APPLICABLE);
     uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiCtfCfgCfm
*
*       Desc:  Config Cfm
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/

PUBLIC S16 UehLiCtfCfgCfm(
    Pst           *pst,
    SuId          suId,
    CtfCfgTransId transId,
    U8            status
  )
{
   UehUeCb      *ueCb = NULLP;
   UeCtfCfgCfm  ueCtfCfgCfm;
   U8           identity =0,cfgType = 0;
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif
   logFunc("%s", "UehLiCtfCfgCfm");

#ifdef RRM_PRL_FLAG
  if(islogApiEnabled())
   {
     buff = GET_UEHCB_PRNT_LIB_BUFF();   
     printCtfCfgCfm(transId, status, buff);
     logAPI("CTF -  RRM : %s", buff->startAddr);
     buff->numBytesWritten = 0;
   }
#endif

   /*CTF UE CFG CFM*/
   if (CTF_UE_CFG == cfgType)
   {
     UEH_GET_VALS_CTF_TRANS(transId.trans, cfgType, identity);
     ueCtfCfgCfm.crnti = identity;
     UEH_GET_UECB_FROM_CRNTI(ueCb, ueCtfCfgCfm.crnti);
     if (ueCb == NULLP)
     {
       /* Reached here because of ctf config cfm for ue deletion */
       RETVALUE(ROK);
     }
     if(status != CTF_CFG_CFM_OK)
     {
       UEH_ERROR_RECEIVED_FAILURE_EVENT(pst->event, pst->srcEnt, status, UE, identity);
       RETVALUE(RFAILED);
     }

     ueCtfCfgCfm.cfgType = cfgType;
     ueCtfCfgCfm.status = status;
     ueFsm(ueCb, EVENTTYPE_UECTFCFGCFM, (void *)&ueCtfCfgCfm);

     RETVALUE(ROK);
   }
   /*CTF CELL CFG CFM */
      
  if(status != CTF_CFG_CFM_OK)
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT(pst->event, pst->srcEnt, status, CELL, identity);
    RETVALUE(RFAILED);
  }

  uehSendLmAlarm(LCM_CATEGORY_PROTOCOL, LUEH_EVENT_CELL_CFG_SUCCESS, LCM_CAUSE_UNKNOWN);

  RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiCtfUeIdChgCfm
*
*       Desc:  Ue Id Change Cfm
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/

PUBLIC S16 UehLiCtfUeIdChgCfm(
    Pst                 *pst,
    SuId                suId,
    CtfCfgTransId       transId,
    CtfUeInfo           *ueInfo,
    U8                  status
  )
{
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
  CmStatus sta;
#endif
   logMsgIn("CTF UeId Change Cfm, status: %u ", status);

#ifdef RRM_PRL_FLAG
 if(islogApiEnabled())
  {
    buff = GET_UEHCB_PRNT_LIB_BUFF();
    printCtfUeIdChgCfm(transId, ueInfo, sta, buff);
    logAPI("CTF -  RRM : %s", buff->startAddr);
    buff->numBytesWritten = 0;
}
    sta.status = status;
    sta.reason = 0;
#endif
   RETVALUE(ROK);
}

/**************************************************************
 The following functions handles the events from S1AP Layer,
 Receive the event, Extract the UE for the event, invoke the UE State Machine
 SZT Interface
***************************************************************/
/*
*
*       Fun:   UehLiSztBndCfm
*
*       Desc:  SZT Bind Confirm
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiSztBndCfm(
    Pst            *pst,
    SuId           suId,
    U8             status
  )
{

   logMsgIn("Received Bind confirm from S1AP, status: %u ", status);

   /* Send alarm */
   if (CM_BND_OK == status)
   {
      uehStopTmr(UEH_TMR_SZT_SAP_BND,&(((UehLiSapCb *)uehCb.sztSap[0])->timer),(PTR)uehCb.sztSap[0],0);
    ((UehLiSapCb *)uehCb.sztSap[0])->bndRetryCnt = 0;
      /* Send the Alarm/Status indication to LM */
      uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, LUEH_CAUSE_SZT_SAP_BOUND);
   }
   else
   {
     /* Let the timer expire,  we send bind request again after the expiry of timer */
      uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
   }
   RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiSztAbortCfm
*
*       Desc:  SZT Abort Confirm
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiSztAbortCfm (Pst *pst,SuId suId,SztAbortS1 *abortS1)
{

  logMsgIn("Received Abort confirm from S1AP status: %d Reason: %d ", abortS1->abortS1Cfm.status, abortS1->abortS1Cfm.reason);
  /*TODO if status has recieved in negative */
  S1apMgrCb *s1Cb =  (S1apMgrCb *)getS1MgrCb( abortS1->peerId.val );
  s1apFsm( s1Cb, S1_MSG_ABORT_CFM, NULLP);
  RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiSztConCfm
*
*       Desc:  SZT Connection Confirm 
*
*       Ret:   None
*
*       Notes: Connection confirm is called from S1AP, after we get
*                       SZT_MSG_ID_CXT_SETUP_REQ,
*                       SZT_MSG_ID_PATH_SWTCH_REQ_ACK:
*                       SZT_MSG_ID_DL_NAS_TPT:
*
*
*       File:  ueh_li.c
*
*/

PUBLIC S16 UehLiSztConCfm(
    Pst             *pst,
    SuId             suId,
    SztConCfm       *conCfm
  )
{
  U8           evnt;
  U16          msgIndex = 0;
  UehS1ConCb   *s1ConCb = NULLP;

  logFunc("%s", "UehLiSztConCfm");
  logInfo("SZT Conn Cfm |suConnId=%d", conCfm->suConnId);

  /* Locate S1ConCb */ 
  UEH_GET_S1CONCB(s1ConCb,conCfm->suConnId);
  if(s1ConCb == NULLP)
  {
    UEH_ERROR_NO_CB_LOCATED(S1AP_CONNECTION, conCfm->suConnId, EVTSZTCONCFM, ENTSZ);
    RETVALUE(RFAILED);
  }
  UECM_INPUT(s1ConCb->ueCb->crnti,UehLiSztConCfm,S1AP,"S1AP CONNECTION CONFIRM");
  /* Save the SPID from S1ap Stack */
  s1ConCb->spConnId = conCfm->spConnId;

  /* Fetch info from pdu */
  uehSztUtilsGetMsgIdx (&evnt, conCfm->pdu);

  msgIndex = uehMsgTypeToIndex(evnt, UE_SZ_MSG_CAT);
  if( UE_UNSUPPORTED_EVT ==  msgIndex )
  {
    UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("UEH S1AP Event", msgIndex, "FSM Events");
    RETVALUE(RFAILED);
  }

  ueFsm( (s1ConCb->ueCb), msgIndex, (void *)conCfm->pdu );

  RETVALUE(ROK);

}

/*
*
*       Fun:   UehLiSztConInd
*
*       Desc:  SZT Con Ind
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
/* TODO: This function gets invoked on MME side (should be removed) */
PUBLIC S16 UehLiSztConInd(
    Pst            *pst,
    SuId           suId,
    SztConInd      *conInd
  )
{
   logFunc("%s", "UehLiSztConInd");

   logInfo("%s", "UehLiSztConInd: SZT Conn. Ind ");

   RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiSztRelCfm
*
*       Desc:  SZT Release Confirm
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/

PUBLIC S16 UehLiSztRelCfm(
    Pst            *pst,
    SuId           suId,
    SztRelCfm      *relCfm
  )
{
   logFunc("%s", "UehLiSztRelCfm");

   logInfo("%s", "UehLiSztRelCfm: SZT Release Cfm ");

   RETVALUE(ROK);
}


/*
*
*       Fun:   UehLiSztDatInd
*
*       Desc:  SZT Data Ind.
*
*       Ret:   None
*
*       Notes: Connection confirm is called from S1AP, when a UE Specific message is received.
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiSztDatInd(
    Pst            *pst,
    SuId           suId,
    SztDatEvntInd  *datEvnt
  )
{
  UehS1ConCb *s1ConCb = NULLP;
  U8          evnt;
  U16         msgIndex = 0;

  logFunc("%s", "UehLiSztDatInd");

  logInfo("%s", "UehLiSztDatInd: SZT Data Ind ");

  /* Locate S1 Connection Call Block */
  UEH_GET_S1CONCB(s1ConCb,datEvnt->suConnId);
  if( NULL == s1ConCb )
  {
    UEH_ERROR_NO_CB_LOCATED(S1AP_CONNECTION, datEvnt->suConnId, EVTSZTDATIND, ENTSZ);
    RETVALUE(RFAILED);
  }
  UECM_INPUT(s1ConCb->ueCb->crnti,UehLiSztDatInd,S1AP,"S1AP DATA INDICATION");
  /* Fetch the UE event and call appropriate state machine function */
  uehSztUtilsGetMsgIdx(&evnt, datEvnt->pdu);
  msgIndex = uehMsgTypeToIndex(evnt, UE_SZ_MSG_CAT);
  if (msgIndex == UE_UNSUPPORTED_EVT)
  {
    UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("UEH S1AP Event", msgIndex, "FSM Events");
    RETVALUE(RFAILED);
  }

  ueFsm( (s1ConCb->ueCb), msgIndex, (void *)datEvnt->pdu );

  RETVALUE(ROK);
}

/**
  *
  *       Fun:   UehLiSztEncDecCfm
  *
  *       Desc:  SZT Enc Dec Confirm.
  *
  *       Ret:   None
  *
  *      Notes: Encode Decode Message TO-add
  *
  *       File:  ueh_li.c
  *
  */
PUBLIC S16 UehLiSztEncDecCfm(
    Pst            *pst,
    SuId           suId,
    SztEncDecEvnt  *sztEncDecEvnt,
    SztReqStatus   *status
  )
{

  logFunc("%s", "UehLiSztEncDecCfm");

  if ( (SZT_PRIM_OK EQ status->status) && (SZT_CAUSE_NOT_APPL EQ status->cause) )
  {
    /* Success Condition */
    RETVALUE(ROK);
  }
  else
  {
    RETVALUE(RFAILED);
  }
  /* TODO */
  //ueFsm( (s1ConCb->ueCb), EVENTTYPE_S1APENCDECCFM, (void *)sztEncDecEvnt->u.pdu );


  RETVALUE(ROK);

}
/*
*
*       Fun:   UehLiSztErrInd
*
*       Desc:  SZT Data Ind.
*
*       Ret:   None
*
*       Notes: Error Indication is received from MME.
*       TODO Presently we handle Erros because of Data sent from Remote node,
*       Error generated by S1AP Stack because of input data From UEH are simply ignored.
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiSztErrInd(
    Pst            *pst,
    SuId           suId,
    SztLclErrInd   *sztLclErrInd
  )
{
  UehS1ConCb *s1ConCb = NULLP;

  logFunc("%s", "UehLiSztErrInd");

  if( SZT_CAUSE_RX_ERRONEOUS_RSP != sztLclErrInd->causeType )
  {
    RETVALUE(RFAILED);
  }

  /* Locate S1 Connection Call Block */
  UEH_GET_S1CONCB(s1ConCb,sztLclErrInd->suConnId.val);
  if( NULL == s1ConCb )
  {
    UEH_ERROR_NO_CB_LOCATED(S1AP_CONNECTION , sztLclErrInd->suConnId.val, EVTSZTERRIND, ENTSZ);
    RETVALUE(RFAILED);
  }

  logInfo("S1ErrInd Received|Crnti=%d|State=%d eNbs1apId:%d mmeS1apId:%d", s1ConCb->ueCb->crnti,  s1ConCb->ueCb->ueState, s1ConCb->enb_ue_s1ap_id, s1ConCb->mme_ue_s1ap_id);

  ueFsm( (s1ConCb->ueCb), EVENTTYPE_S1APERRIND, NULL );

  RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiSztRelInd
*
*       Desc:  SZT Release Ind.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/

PUBLIC S16 UehLiSztRelInd(
    Pst            *pst,
    SuId           suId,
    SztRelInd      *relInd
  )
{
   UehS1ConCb   *s1ConCb = NULLP;
   U8           evnt;
   U16          msgIndex = 0;

   logFunc("%s", "UehLiSztRelInd");
   logInfo("%s", "UehLiSztRelInd: SZT Release Indication ");

   UEH_GET_S1CONCB(s1ConCb,relInd->suConnId.val);
   if(s1ConCb == NULLP)
   {
     UEH_ERROR_NO_CB_LOCATED(S1AP_CONNECTION, relInd->suConnId.val, EVTSZTRELIND, ENTSZ);
     RETVALUE(RFAILED);
   }
   UECM_INPUT(s1ConCb->ueCb->crnti,UehLiSztRelInd,S1AP,"S1AP RELEASE INDICATION");
   uehSztUtilsGetMsgIdx (&evnt, relInd->pdu);
   msgIndex = uehMsgTypeToIndex(evnt, UE_SZ_MSG_CAT);
   if (msgIndex == UE_UNSUPPORTED_EVT)
   {
     UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("UEH S1AP Event", msgIndex, "FSM Events");
     RETVALUE(RFAILED);
   }
   ueFsm( (s1ConCb->ueCb), msgIndex, (void *)relInd->pdu );
   RETVALUE(ROK);
}

PUBLIC S16 NxLiRgrUeStaInd(
    Pst            *pst,
    SuId           suId,
    RgrUeStaIndInfo *ueStaInd
  )
{
  if (RGR_UESTA_MAC_CRNTI_CE_RECVD == ueStaInd->status)
  {
    logError("MAC CRNTI CE recieved for crnti (%d)", ueStaInd->crnti);
  }
  SPutSBuf(pst->region, pst->pool, (Data *)ueStaInd, sizeof(RgrUeStaIndInfo));
  RETVALUE(ROK);
}

/*
*
*       Fun:   NxLiRgrStaInd
*
*       Desc:  Receives CQI info in status ind and restarts the rlf timer and reset the consecutive CQI not received count to zero.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 NxLiRgrStaInd(
    Pst            *pst,
    SuId           suId,
    RgrStaIndInfo *statusInd
  )
{
  UehUeCb *ueCb= NULL;
  UEH_GET_UECB_FROM_CRNTI(ueCb ,statusInd->crnti);
  logInfo("NxLiRgrStaInd: CQI report received for UE =%d", statusInd->crnti);
  uehStopTmr(UEH_CQI_BASED_RLF_DETECTION_TMR, &((UehUeCb *)ueCb)->ueCqiBasedRlfDetectionTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti );
  uehStartTmr(UEH_CQI_BASED_RLF_DETECTION_TMR, &((UehUeCb *)ueCb)->ueCqiBasedRlfDetectionTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
  
  SPutSBuf(pst->region, pst->pool, (Data *)statusInd, sizeof(RgrStaIndInfo));
  RETVALUE(OK);
}
/**************************************************************
  CCPU Names their User of RRC as NX,
  The following functions are basically wrappers over UEH functions
  TODO Replace these Nx wrappers in file nh_ptui.c,  ueh_ex_ms.c
  NX  functions (Wrapper function)
***************************************************************/
#ifdef RGR_RRM_TICK
/*
*
*       Fun:   NxLiRgrTtiInd
*
*       Desc:  Tti Indication
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/

PUBLIC S16 NxLiRgrTtiInd(
    Pst            *pst,
    SuId           suId,
    RgrTtiIndInfo  *ttiInd
  )
{

#ifndef SI_NEW
   S16 ret = uehSibHdlTmrInd(ttiInd);
#endif
   uehPagHdlTmrInd(ttiInd);
   
   if(ttiInd->sfn NOTEQ ((uehCb.lastRcvdSfn+1)%1024))
     logError("TTI IND Dropped: Last rcvd SFN=%d Curr SFN=%d",uehCb.lastRcvdSfn,ttiInd->sfn);
   uehCb.lastRcvdSfn = ttiInd->sfn;


   uehGetCellCb()->tickCount++;
   /* Moved contents of UehLiRgrTtiInd here.
    * ret = UehLiRgrTtiInd(pst, suId, ttiInd);
    */ 
   /*   ttiInd is decoded into a variable in uehCb.
    *   Hence, free is not needed.
    *   UEH_FREE(ttiInd,sizeof(RgrTtiIndInfo));
    */  
 
#if 0
   /* We get ticks every 10 ms i.e. 100 ticks/sec. We want to print the counter every 10 sec. 
      100 ticks/sec * 10 sec = 1000 ticks */
   if (uehGetCellCb()->tickCount % 1000 EQ 0)
   {
     static struct timeval now, prev = {0,0};
     static int elapsed_sec, elapsed_ms;

     gettimeofday(&now, NULL);

     elapsed_sec   = now.tv_sec - prev.tv_sec;
     elapsed_ms    = (now.tv_usec - prev.tv_usec)/1000;
     if(elapsed_ms < 0) {
        elapsed_sec--;
        elapsed_ms += 1000;
     }
     logCtr("Recd TtiInd=%u, sfn=%u, mibsTx=%u, timediff=%d s:%d ms", 
                uehGetCellCb()->tickCount,
                ttiInd->sfn,
               (uehGetCellCb())->numMibsTx,
               elapsed_sec, elapsed_ms);
     prev = now;
   }
#endif
   RETVALUE(ret);
}
#endif
/*
*
*       Fun:   NxLiNhuBndCfm - NHU Bind Cfm
*
*       Desc:  This function is used by to present Bind Cfm to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 NxLiNhuBndCfm(
    Pst     *pst,
    SuId    suId,
    U8      status
  )
{
   logFunc("%s", "NxLiNhuBndCfm");
   UehLiNhuBndCfm(pst, suId, status);
   RETVALUE(ROK);
}

/*
*
*       Fun:   NxLiNhuDatInd - NHU Data Indication
*
*       Desc:  This function is used by to present Data Indication to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 NxLiNhuDatInd(
    Pst                 *pst,
    SuId                suId,
    NhuDatIndSdus       *nhuDatIndSdus
  )
{
   logFunc("%s", "NxLiNhuDatInd");
#ifdef RRM_PRL_FLAG 
   Buff *buff = NULL;
#endif  
   
#ifdef RRM_PRL_FLAG 
  if (islogApiEnabled())
  { 
    buff = GET_UEHCB_PRNT_LIB_BUFF();
    printNhuDatInd(nhuDatIndSdus, buff);
    logAPI("NhuDatIndSdus %s", buff->startAddr);  
    buff->numBytesWritten = 0;
  }
#endif
   /* cmFreeMem is used for freeing since cmAllocEvnt is used while allocating */
   UehLiNhuDatInd (pst, suId, nhuDatIndSdus);
   UEH_FREEEVNT(nhuDatIndSdus);
   RETVALUE(ROK);
}

/*
*
*       Fun:   NxLiNhuDatCfm - NHU Data Confirm
*
*       Desc:  This function is used by to present Data Confirmation to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 NxLiNhuDatCfm(
    Pst           *pst,
    SuId           suId,
    NhuDatCfmSdus *nhDatCfmSdus
  )
{
    S16 ret=0;
    logFunc("%s", "NxLiNhuDatCfm");
    ret=UehLiNhuDatCfm (pst, suId, nhDatCfmSdus);
    UEH_FREE(nhDatCfmSdus,sizeof(nhDatCfmSdus));
    RETVALUE(ret);
}

/*
*
*       Fun:   NxLiNhuStaInd - NHU status Indication
*
*       Desc:  This function is used by to present status indication to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 NxLiNhuStaInd(
    Pst                 *pst,
    SuId                suId,
    NhuStaIndSdus       *nhStaIndSdus
  )
{
   logFunc("%s", "NxLiNhuStaInd");
   
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL; 
 if (islogApiEnabled())
  { 
   buff = GET_UEHCB_PRNT_LIB_BUFF();
   printNhuStaIndSdus(nhStaIndSdus, buff);
   logAPI("RRM_Print_Library_RRMtoRRC:UehLiNhuStaInd:UehLiNhuStaInd STRUCTURE: %s", buff->startAddr);   
   buff->numBytesWritten = 0;
   }
#endif
   UehLiNhuStaInd (pst, suId, nhStaIndSdus);
   UEH_FREE(nhStaIndSdus,sizeof(nhStaIndSdus));
   RETVALUE(ROK);
}

/*
*
*       Fun:   NxLiNhuErrInd - Error Indication
*
*       Desc:  This function is used by to present configuration 
*              confirm information to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 NxLiNhuErrInd(
    Pst                 *pst,
    SuId                suId,
    NhuErrIndSdus       *nhErrIndSdus
  )
{
#ifdef RRM_PRL_FLAG 
   Buff *buff = NULL;
#endif  
   S16 ret=0;
   logFunc("%s", "NxLiNhuErrInd");
   
#ifdef RRM_PRL_FLAG  
  if (islogApiEnabled())
  { 
    buff = GET_UEHCB_PRNT_LIB_BUFF();
    printNhuErrIndSdus(nhErrIndSdus, buff);
    logAPI("RRM - RRC :NhuErrIndSdus:nhErrIndSdus:STRUCTURE %s", buff->startAddr);  
    buff->numBytesWritten = 0;
  }  
#endif
   /* SPutSBuf is used for freeing since SGetSBuf is used while allocating */
   ret=UehLiNhuErrInd(pst, suId, nhErrIndSdus);
   UEH_FREE(nhErrIndSdus, sizeof(NhuErrIndSdus));
   RETVALUE(ret);
}

/*
*
*       Fun:   NxLiNhuCfgCfm - Lower layer config confirm
*
*       Desc:  This function is used by to present configuration confirm
*              to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 NxLiNhuCfgCfm(
    Pst                 *pst,
    SuId                suId,
    NhuCfgCfmSdus       *nhuCfgCfmSdus
  )
{
#ifdef RRM_PRL_FLAG 
   Buff *buff = NULL;
#endif  
   logFunc("%s", "NxLiNhuCfgCfm");
   S16 ret = 0;
   
#ifdef RRM_PRL_FLAG  
  if (islogApiEnabled())
   { 
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhuCfgCfmSdus(nhuCfgCfmSdus, buff);
     logAPI("RRM - RRC :NhuCfgCfmSdus:nhuCfgCfmSdus:STRUCTURE %s", buff->startAddr);   
     buff->numBytesWritten = 0;
   }  
#endif
   /* SPutSBuf is used for freeing since SGetSBuf is used while allocating */
   ret = UehLiNhuCfgCfm(pst, suId, nhuCfgCfmSdus);
   UEH_FREE(nhuCfgCfmSdus, sizeof(NhuCfgCfmSdus));
   RETVALUE(ret);
}

/*
*
*       Fun:   NxLiNhuCellCfgCfm - Cell Configuration Confirm
*
*       Desc:  This function is used by to present Cell Config Status Confirm
*              information to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 NxLiNhuCellCfgCfm(
    Pst                 *pst,
    SuId                suId,
    NhuCellCfgCfmSdus   *nhuCellCfgCfmSdus
  )
{
#ifdef RRM_PRL_FLAG 
   Buff *buff = NULL;
#endif  
   logFunc("%s", "NxLiNhuCellCfgCfm");
   S16 ret = 0;
  
#ifdef RRM_PRL_FLAG  
  if (islogApiEnabled())
   { 
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhuCellCfgCfmSdus(nhuCellCfgCfmSdus, buff);
     logAPI("RRM - RRC :NhuCellCfgCfmSdus:nhuCellCfgCfmSdus:STRUCTURE: %s", buff->startAddr);
     buff->numBytesWritten = 0;
   }  
#endif
   /* SPutSBuf is used for freeing since SGetSBuf is used while allocating */
   ret = UehLiNhuCellCfgCfm(pst, suId, nhuCellCfgCfmSdus);
   UEH_FREE(nhuCellCfgCfmSdus, sizeof(NhuCellCfgCfmSdus));
   RETVALUE(ret);
}

/*
*
*       Fun:   NxLiNhuDecodeCfm - Decode Confirm
*
*       Desc:  This function is used to inform decode confirmation to RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 NxLiNhuDecodeCfm(
    Pst                 *pst,
    SuId                suId,
    NhuDecodeSduCfm     *nhuDecodeSduCfm
  )
{
   S16 ret=0;
   logFunc("%s", "NxLiNhuDecodeCfm");
   
#ifdef RRM_PRL_FLAG  
  Buff *buff = NULL;
  if(islogApiEnabled())
   {
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhuDecodeCfm(nhuDecodeSduCfm, buff);
     logAPI("RRM - RRC :UehLiNhuDecodeCfm:nhuDecodeSduCfm STRUCTURE %s", buff->startAddr);
     buff->numBytesWritten = 0;
   } 
#endif 
   ret = UehLiNhuDecodeCfm(pst, suId, nhuDecodeSduCfm);
   UEH_FREEEVNT(nhuDecodeSduCfm);
   RETVALUE(ret);


}

/*
*
*       Fun:   NxLiNhuCountCfm - Count Confirm
*
*       Desc:  This function is used to inform COUNT values of DRBs to RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 NxLiNhuCountCfm(
    Pst                 *pst,
    SuId                suId,
    NhuDRBCountInfoLst  *nhuDRBCountInfoLst
  )
{
   S16 ret=0;
   logFunc("%s", "NxLiNhuCountCfm");
   
   
#ifdef RRM_PRL_FLAG  
 Buff    *buff      =NULL;
  if (islogApiEnabled())
   { 
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhuCountCfm(nhuDRBCountInfoLst, buff);
     logAPI("RRM - RRC :UehLiNhuCountCfm:nhuDRBCountInfoLst %s", buff->startAddr);
     buff->numBytesWritten = 0;
   } 
#endif
     ret=UehLiNhuCountCfm(pst, suId, nhuDRBCountInfoLst);
     UEH_FREEEVNT(nhuDRBCountInfoLst);
     RETVALUE(ret);
}

/*
*
*       Fun:   NxLiNhuEncodeCfm - Encode Confirm
*
*       Desc:  This function is used to inform Encode confirmation to RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 NxLiNhuEncodeCfm(
    Pst                 *pst,
    SuId                suId,
    NhuEncCfmSdus       *nhuEncCfmSdus
  )
{
   S16 ret=0;
   logFunc("%s", "NxLiNhuEncodeCfm");
   
#ifdef RRM_PRL_FLAG  
  Buff   *buff    =NULL;  
  if (islogApiEnabled())
   {
      buff = GET_UEHCB_PRNT_LIB_BUFF();
      printNhuEncCfmSdus(nhuEncCfmSdus, buff);   
      logAPI("RRM - RRC :UehLiNhuEncodeCfm:nhuEncCfmSdus %s", buff->startAddr);
      buff->numBytesWritten = 0;
    } 
#endif 
   ret=UehLiNhuEncodeCfm(pst,suId,nhuEncCfmSdus);
   UEH_FREE(nhuEncCfmSdus,sizeof(nhuEncCfmSdus));
   RETVALUE(ret);
}

/*
*
*       Fun:   NxLiNhuPdcpSduStaCfm - PDCP SDU Status Confirm
*
*       Desc:  This function is used to inform PDCP SDU Status confirmation to 
*              RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 NxLiNhuPdcpSduStaCfm(
    Pst                 *pst,
    SuId                suId,
    NhuPdcpSduStaCfm    *nhuPdcpSduStaCfm
  )
{
   logFunc("%s", "NxLiNhuPdcpSduStaCfm");
   
#ifdef RRM_PRL_FLAG  
   Buff     *buff     =NULL;   
  if (islogApiEnabled())
   { 
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhuPdcpSduStaCfm(nhuPdcpSduStaCfm, buff);
     logAPI("RRM - RRC :UehLiNhuPdcpSduStaCfm:nhuPdcpSduStaCfm STRUCTURE %s", buff->startAddr);   
     buff->numBytesWritten = 0;
   }     
#endif
   UehLiNhuPdcpSduStaCfm(pst,suId,nhuPdcpSduStaCfm);
   UEH_FREEEVNT(nhuPdcpSduStaCfm);
   RETVALUE(ROK);
}

/*
*
*       Fun:   NxLiRgrBndCfm - RGR Bind Cfm
*
*       Desc:  This function is used by to present Bind Cfm to RGR.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 NxLiRgrBndCfm(
    Pst     *pst,
    SuId    suId,
    U8      status
  )
{

   logFunc("%s", "NxLiRgrBndCfm");
   RETVALUE(UehLiRgrBndCfm(pst,suId,status));
}



/*
*
*       Fun:   NxLiRgrCfgCfm - Cell Configuration Confirm
*
*       Desc:  This function is used by to present Cell Config Status Confirm
*              information to RGR.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 NxLiRgrCfgCfm(
    Pst            *pst,
    SuId           suId,
    RgrCfgTransId  transId,
    U8             status
  )
{
  U16           crnti = 0;
  UehUeCb       *ueCb = NULLP;
  UeRgrCfgCfm   ueRgrCfgCfm;
  UehCellCb     *cellCb = NULLP;
#ifdef RRM_PRL_FLAG 
  Buff *buff = NULL;
#endif

  logFunc("%s", "NxLiRgrCfgCfm");

  UEH_GET_VALS_RGR_TRANS(transId.trans, ueRgrCfgCfm.cfgType, ueRgrCfgCfm.lchId, crnti);
  UECM_INPUT(crnti,NxLiRgrCfgCfm,SCHED,"RgrCfgCfm");

  cellCb = uehGetCellCb();
  if(cellCb == NULLP)
  {
    UEH_ERROR_NO_CB_LOCATED(CELL,  NOT_APPLICABLE, EVTRGRCFGCFM, ENTRG);
    RETVALUE(RFAILED);
  }

  ueRgrCfgCfm.status = status;

  if (RGR_CELL_CFG == ueRgrCfgCfm.cfgType)
  {
    /* Cell Rgr configuration confirm */
    if (ROK != uehProcCellRgrCfgCfm (status))
    {
      RETVALUE(RFAILED);
    }
    RETVALUE(ROK);
  } 

#ifdef SI_NEW
  else if (((5+RGR_SI_CFG_TYPE_MIB) == ueRgrCfgCfm.cfgType) ||
      ((5+RGR_SI_CFG_TYPE_SIB1) == ueRgrCfgCfm.cfgType) ||
      ((5+RGR_SI_CFG_TYPE_SI) == ueRgrCfgCfm.cfgType))
  {
#ifdef RRM_PRL_FLAG
  if (islogApiEnabled())
   { 
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printRgrSiCfgCfm(transId, status, buff);
     logAPI("SCH - RRM : %s", buff->startAddr);
     buff->numBytesWritten = 0;
   }     
#endif
    /* TODO: Failure cases needs to be added */
    if (ROK != ueRgrCfgCfm.status)
    {
      UEH_ERROR_RECEIVED_FAILURE_EVENT(  EVTRGRCFGCFM, ENTRG, ueRgrCfgCfm.status, CELL, uehCb.cellCb[0]->cellId);
      RETVALUE(RFAILED);
    }
    RETVALUE(ROK);
  }
#endif
  if (UEH_RGR_UE_DEL_TRANSID == ueRgrCfgCfm.cfgType)
  {
    RETVALUE(ROK);
  }
  UEH_GET_UECB_FROM_CRNTI(ueCb,crnti);
  if(NULL == ueCb)
  {
    UEH_ERROR_NO_CB_LOCATED(UE, crnti, EVTRGRCFGCFM, ENTRG);
    RETVALUE(RFAILED);
  }

  
#ifdef RRM_PRL_FLAG
  if (islogApiEnabled())
   { 
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printRgrCfgCfm(transId, status, buff);
     logAPI("SCH - RRM : %s", buff->startAddr);
     buff->numBytesWritten = 0;
   }      
#endif
  ueFsm(ueCb, EVENTTYPE_UESCHCFGCFM, (void *)&ueRgrCfgCfm);
  RETVALUE(ROK);
} /* end of NxLiRgrCfgCfm */

/*
*
*       Fun:   UehLiEgtEguLclTnlMgmtCfm
*
*       Desc:  Call handler for eGTP-U Tunnel Mgmt Confirm.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiEgtEguLclTnlMgmtCfm(
    Pst         *pst,
    SuId        suId,
    EgtUEvnt    *eguMsg
  )
{
  U32              lclTeid = 0;
  UehUeCb          *ueCb = NULLP;

  logFunc("%s", "UehLiEgtEguLclTnlMgmtCfm");

  logInfo("%s ", "UehLiEgtEguLclTnlMgmtCfm: EGTP Tunnel Mgmt Cfm");

  lclTeid = eguMsg->u.tnlMgmt.teId;
  if (lclTeid >= UEH_MAX_TUNNEL_ID)
  {
    UEH_ERROR_ELMNT_OUT_OF_RANGE("Tunnel Id", lclTeid, 0, UEH_MAX_TUNNEL_ID-1);
    RETVALUE(RFAILED);
  }

  ueCb = uehCb.locTeIdToUeCb[lclTeid];
  if(NULLP == ueCb)
  {
    UEH_ERROR_NO_CB_LOCATED( UE, NOT_APPLICABLE, EVTEGTULCLTNLMGMTCFM, ENTUDH); 
    RETVALUE(RFAILED);
  }

  ueFsm(ueCb, EVENTTYPE_EGTPTUNNELMGMTCFM, (void *)eguMsg);

  RETVALUE(ROK);
}

/*
*
*       Fun:   EuLiEgtEguStaCfm
*
*       Desc:  Call handler for eGTP-U Status Confirm.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 EuLiEgtEguStaCfm(
    Pst         *pst,
    SuId        suId,
    EgtUEvnt    *eguMsg
  )
{
   logFunc("%s", "EuLiEgtEguStaCfm");
   S16 ret = 0;
   ret = UehLiEgtEguStaCfm(pst, suId, eguMsg);
   SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));
   RETVALUE(ret);
}


/*
*
*       Fun:   UehLiEgtEguStaCfm
*
*       Desc:  Call handler for eGTP-U Status Confirm.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiEgtEguStaCfm(
    Pst         *pst,
    SuId        suId,
    EgtUEvnt    *eguMsg
  )
{
   logInfo("%s", "UehLiEgtEguStaCfm: EGT Status Confirm ");

   RETVALUE(ROK);
}

/*
*
*       Fun:   EuLiEgtEguUStaInd
*
*       Desc:  Call handler for eGTP-U Unsolicited Status Indication.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 EuLiEgtEguUStaInd(
    Pst         *pst,
    SuId        suId,
    EgtUEvnt    *eguMsg
  )
{
   logFunc("%s", "EuLiEgtEguUStaInd");
   S16 ret = 0;
   ret = UehLiEgtEguUStaInd(pst, suId, eguMsg);
   SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));
   RETVALUE(ret);
}


/*
*
*       Fun:   UehLiEgtEguUStaInd
*
*       Desc:  Call handler for eGTP-U Unsolicited Status Indication.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiEgtEguUStaInd(
    Pst         *pst,
    SuId        suId,
    EgtUEvnt    *eguMsg
  )
{
   logInfo("%s", "UehLiEgtEguUStaInd: EGT Unsolicited Status Indication ");

   RETVALUE(ROK);
}

/*
*
*       Fun:   EuLiEgtEguErrInd
*
*       Desc:  Call handler for eGTP-U Error Indication.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 EuLiEgtEguErrInd(
    Pst         *pst,
    SuId        suId,
    EgtUEvnt    *eguMsg
  )
{
   logFunc("%s", "EuLiEgtEguErrInd");
   S16 ret = UehLiEgtEguErrInd(pst, suId, eguMsg);
   SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));
   RETVALUE(ret);
}


/*
*
*       Fun:   UehLiEgtEguErrInd
*
*       Desc:  Call handler for eGTP-U Error Indication.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/

PUBLIC S16 UehLiEgtEguErrInd(
    Pst         *pst,
    SuId        suId,
    EgtUEvnt    *eguMsg
  )
{
  U32              lclTeid = 0;
  UehUeCb          *ueCb = NULLP;

  logError("EGTErrorInd|lclTeid=%u|remTeId=%u|cause=%d|diagInfo=%d", eguMsg->u.errInd.localTeid, eguMsg->u.errInd.remTeid, eguMsg->u.errInd.errCause, eguMsg->u.errInd.diagInfo);

  /* Get Local Tunnel ID,
   * Extract the EGTP CallBlock for this tunnel ID
   */
  lclTeid = eguMsg->u.errInd.localTeid;
  if (lclTeid >= UEH_MAX_TUNNEL_ID)
  {
    UEH_ERROR_ELMNT_OUT_OF_RANGE("Tunnel Id", lclTeid, 0, UEH_MAX_TUNNEL_ID-1);
    RETVALUE(RFAILED);
  }

  ueCb = uehCb.locTeIdToUeCb[lclTeid];
  if(NULLP == ueCb)
  {
    UEH_ERROR_NO_CB_LOCATED( UE, NOT_APPLICABLE, pst->event, pst->srcEnt); 
    RETVALUE(RFAILED);
  }
  ueFsm(ueCb, EVENTTYPE_EGTERRIND, (void *)eguMsg);

  RETVALUE(ROK);
}

/*
*
*       Fun:   EuLiEgtEguLclTnlMgmtCfm
*
*       Desc:  Call handler for eGTP-U Tunnel Mgmt Confirm.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 EuLiEgtEguLclTnlMgmtCfm(
    Pst         *pst,
    SuId        suId,
    EgtUEvnt    *eguMsg
  )
{
   logFunc("%s", "EuLiEgtEguLclTnlMgmtCfm");
   S16 ret = 0;
   ret = UehLiEgtEguLclTnlMgmtCfm(pst, suId, eguMsg);
   SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));
   RETVALUE(ret);
}

/*
*
*       Fun:   UehLiUdhBndCfm - UDH Bind Cfm
*
*       Desc:  This function is used by to present Bind Cfm to UEH.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/

PUBLIC S16 UehLiUdhBndCfm(
    Pst     *pst,
    SuId    suId,
    U8      status
  )
{
  logFunc("%s", "UehLiUdhBndCfm");

  /* Send alarm */
  if (CM_BND_OK == status)
  {
    uehStopTmr(UEH_TMR_UDH_SAP_BND,&(((UehLiSapCb *)uehCb.udhSap[0])->timer),(PTR)uehCb.udhSap[0],0);
    ((UehLiSapCb *)uehCb.udhSap[0])->bndRetryCnt = 0;
    logInfo("%s","UEH Bind to UDH is Successful");
    uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, LUEH_CAUSE_UDH_SAP_BOUND);
  }
  else
  {
    logInfo("%s","UEH Bind to UDH is Failed");
    uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
  }
  RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiUdhAddRabCfm
*
*       Desc:  UDH Add Rab Confirm
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiUdhAddRabCfm(
    Pst              *pst,
    UdhUehAddRabCfm  *udhUehAddRabCfm
  )
{
  UehCellCb    *cellCb = NULLP;
  UehUeCb      *ueCb = NULLP;

  logFunc("%s", "UehLiUdhAddRabCfm");

  logInfo("%s", "UehLiUdhAddRabCfm: Udh Add Rab Confirm Handler ");
  cellCb = uehGetCellCb();
  if(cellCb == NULLP)
  {
     UEH_ERROR_NO_CB_LOCATED(CELL, NOT_APPLICABLE, EVT_UDH_UEH_ADD_RAB_CFM, ENTUDH);
     RETVALUE(RFAILED);
  }

  UEH_GET_UECB_FROM_CRNTI(ueCb,udhUehAddRabCfm->pdcpId.ueId);
  if(ueCb == NULLP)
  {
    UEH_ERROR_NO_CB_LOCATED(UE, udhUehAddRabCfm->pdcpId.ueId, EVT_UDH_UEH_ADD_RAB_CFM, ENTUDH);
    RETVALUE(RFAILED);
  }

  ueFsm(ueCb, EVENTTYPE_UDHERABADDCFM, (void *)udhUehAddRabCfm);

  RETVALUE(ROK);
}


/*
*
*       Fun:   UehLiUdhUeInactvInd
*
*       Desc:  UDH UE Inactivity Indication
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiUdhUeInactvInd(
    Pst                   *pst,
    UdhUehUeInactiveInd   *udhUehInactiveUeInd
  )
{
  U16          ueCnt = 0;
  UehCellCb    *cellCb = NULLP;
  UehUeCb      *ueCb = NULLP;

  logFunc("%s", "UehLiUdhUeInactvInd");

  cellCb = uehGetCellCb();
  if(cellCb == NULLP)
  {
     UEH_ERROR_NO_CB_LOCATED( CELL, NOT_APPLICABLE, EVT_UDH_UEH_INACTIVE_UES_IND, ENTUDH);
     RETVALUE(RFAILED);
  }

  for(ueCnt = 0; ueCnt < udhUehInactiveUeInd->noofInactiveUes; ueCnt++)
  {
    UEH_GET_UECB_FROM_CRNTI(ueCb,udhUehInactiveUeInd->ueId[ueCnt]);
    if(ueCb == NULLP)
    {
      UEH_ERROR_NO_CB_LOCATED(UE, udhUehInactiveUeInd->ueId[ueCnt], EVT_UDH_UEH_INACTIVE_UES_IND, ENTUDH);
      RETVALUE(RFAILED);
    }

    ueFsm(ueCb, EVENTTYPE_UDHUEINACTVIND, (void *)NULLP);
  }

  RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiUdhUlInactvInd
*
*       Desc:  UDH UL Inactivity Indication
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiUdhUlInactvInd(
    Pst                   *pst,
    UdhUehUlInactiveInd   *udhUehUlInactiveInd
  )
{
  U16          ueCnt = 0;
  UehCellCb    *cellCb = NULLP;
  UehUeCb      *ueCb = NULLP;

  logInfo("%s", "UehLiUdhUlInactvInd: UehLiUdhUlInactvInd: UDH UL Inactivity Indication ");

  cellCb = uehGetCellCb();
  if(cellCb == NULLP)
  {
     UEH_ERROR_NO_CB_LOCATED(CELL, NOT_APPLICABLE, EVT_UDH_UEH_UL_INACTIVE_IND, ENTUDH);
     RETVALUE(RFAILED);
  }

  for(ueCnt = 0; ueCnt < udhUehUlInactiveInd->noofInactiveUes; ueCnt++)
  {
    UEH_GET_UECB_FROM_CRNTI(ueCb, udhUehUlInactiveInd->ueId[ueCnt]);
    if(ueCb == NULLP)
    {
      UEH_ERROR_NO_CB_LOCATED(UE, udhUehUlInactiveInd->ueId[ueCnt], EVT_UDH_UEH_UL_INACTIVE_IND, ENTUDH);
      RETVALUE(RFAILED);
    }
    ueFsm(ueCb, EVENTTYPE_UDHULINACTVIND, (void *)NULLP);
  }
  RETVALUE(ROK);
}



/*
*
*       Fun:   UehLiUdhDelRabCfm
*
*       Desc:  UDH UE Inactivity Indication
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 UehLiUdhDelRabCfm(
    Pst                   *pst,
    UdhUehDelRabCfm       *delRabCfm
  )
{
  UehCellCb    *cellCb = NULLP;
  UehUeCb      *ueCb = NULLP;

  logFunc("%s", "UehLiUdhDelRabCfm");

  logInfo("%s", "UehLiUdhDelRabCfm: UDH Del Rab Confirm ");

  cellCb = uehGetCellCb();
  if(cellCb == NULLP)
  {
    UEH_ERROR_NO_CB_LOCATED(CELL, NOT_APPLICABLE, EVT_UDH_UEH_DEL_RAB_CFM, ENTUDH);
    RETVALUE(RFAILED);
  }

  UEH_GET_UECB_FROM_CRNTI(ueCb,delRabCfm->pdcpId.ueId);
  if(ueCb == NULLP)
  {
    UEH_ERROR_NO_CB_LOCATED(UE, delRabCfm->pdcpId.ueId, EVT_UDH_UEH_DEL_RAB_CFM, ENTUDH);
    RETVALUE(RFAILED);
  }

  ueFsm(ueCb, EVENTTYPE_UDHERABDELCFM, (void *)delRabCfm);

  RETVALUE(ROK);
}

/*
*
*       Fun:   UehLiUdhDelUeCfm
*
*       Desc:  UDH UE Inactivity Indication
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_li.c
*/

PUBLIC S16 UehLiUdhDelUeCfm(
    Pst                   *pst,
    UdhUehDelUeCfm       *delUeCfm
  )
{
  UehCellCb    *cellCb = NULLP;
  UehUeCb      *ueCb = NULLP;

  logInfo("%s", "UehLiUdhDelUeCfm: UDH Del UE Confirm ");

  cellCb = uehGetCellCb();
  if(cellCb == NULLP)
  {
     UEH_ERROR_NO_CB_LOCATED( CELL, NOT_APPLICABLE, EVT_UDH_UEH_DEL_UE_CFM, ENTUDH);
     RETVALUE(RFAILED);
  }

  UEH_GET_UECB_FROM_CRNTI(ueCb,delUeCfm->ueId);
  if(ueCb == NULLP)
  {
    UEH_ERROR_NO_CB_LOCATED( UE, delUeCfm->ueId, EVT_UDH_UEH_DEL_UE_CFM, ENTUDH);
    RETVALUE(RFAILED);
  }

  ueFsm(ueCb, EVENTTYPE_UDHDELUECFM , (void *)delUeCfm);

  RETVALUE(ROK);
}
/***************SEC_CHANGE_START*****************/

PUBLIC S16 uehLiCtfDatRsp
(
Pst *pst,
SuId suId,
CtfCfgTransId transId,
CtfKdfCfmInfo *datRspSdu,
U8   status
)
{

  UehUeCb *ueCb = NULLP;
  U16 crnti     = 0;

  copyBytes(&transId,sizeof(U16),&crnti);
  UEH_GET_UECB_FROM_CRNTI(ueCb,crnti);
  logInfo("uehLiCtfDatRsp:status %d,ueState=%d,keyDerType:%d",status,ueCb->ueState,datRspSdu->keyDerType);

  if(ueCb == NULLP)
  { 
    /* Manu:10Aug:RC: logError() should not be used. UEH_ERROR() should be used */
    RETVALUE(RFAILED);
  } /* end of if statement */
  
  if (UE_STATE_SMC_ONGOING == ueCb->ueState)
  {
    copyBytes(datRspSdu->u.kEnbInf.cpCiphKey, CTF_SEC_KEY_LEN, ueCb->secCfg.cpCiphKey);
    copyBytes(datRspSdu->u.kEnbInf.upCiphKey, CTF_SEC_KEY_LEN, ueCb->secCfg.upCiphKey);
    copyBytes(datRspSdu->u.kEnbInf.intKey, CTF_SEC_KEY_LEN, ueCb->secCfg.intgKey);

    if (ROK != uehSndRrcSecModeCmd(ueCb))
    {
      // TODO Decide on what to be done in case of error 
      UEH_ERROR_SSI_EVENT_SEND_FAILED( (&uehCb.nhuSap[0]->pst), EVENTTYPE_SECMODECOMMAND, UE, ueCb->crnti);
      RETVALUE(RFAILED);
    }

  }
  RETVALUE(ROK); 
}

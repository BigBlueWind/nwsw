/**
 * @file    ueh_tmr.c
 * @brief This file contains the UEH Timer code
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 16Sep2010 :
*     a) Created initial version from sample application
*     b) Shweta Polepally :30th Aug 2011 : Added Code under HO_SUPPORTED
 */
 
/* Header includes */
#include "ccpu_cmn.h"

/* RRM related includes */
#include"ueh_eut.h"
#include "ueh.h" 
#include "lueh.h"
#include "lueh.x"
#include "ueh_perf.h" 
#include"ueh_eut_x.h"
#include "s1ap.h"
#include "s1ap_mgr.h"
#include "ueh_crm_sm.h"

#define TMR_EVNT_NAME_LEN                                  25
#define TMR_EVNT_DETAIL_LEN                                30
#define UEH_MAX_TIMERS                                     50

EXTERN PucchRegionResCqiRi  *pucchCqiResTable;

PRIVATE Void uehProcTmr ARGS ((Ptr cb, UehTimerId /*S16*/ tmrEvnt));

/*
 *
 *       Fun:   uehRegInitTmr
 *
 *       Desc:  Register and initialize timer queue structures in 
 *              LTE eNodeB during general config.
 *              
 *       Ret:   ROK - ok; 
 *              RFAILED - failed;
 *
 *       Notes: none
 *
 *       File:  ueh_tmr.c
 *
 */
PUBLIC S16 uehRegInitTmr(Void)
{
   U16 idx;
   S16 ret;
#ifdef UEH_PERF_DL_DATA
   U16 cnt;
#endif 

   logFunc("%s", "uehRegInitTmr");

   ret = SRegTmr((Ent)ENTUEH, (Inst)0, UEH_TMR_RES, uehActvTmr);
   if (ret != ROK)
   {
      UEH_PROCEDURE_FAILED("Timer Registration", NOT_APPLICABLE, NOT_APPLICABLE, "Invalid parameter recieved");
      RETVALUE(RFAILED);
   }

   /* initialize timing queues */
   uehCb.tqCp.tmrLen      = UEH_APP_TQSIZE;

   for (idx = 0; idx < UEH_APP_TQSIZE; idx++)
   {
      uehCb.tq[idx].first    = NULLP;
      uehCb.tq[idx].tail     = NULLP;
   }
#ifdef UEH_PERF_DL_DATA
     uehEuCb.tqCp.tmrLen = UEH_NUM_TQENTRY;
     for(cnt = 0; cnt < UEH_NUM_TQENTRY; cnt++)
     {
      uehEuCb.tq[cnt].first = NULLP;
      uehEuCb.tq[cnt].tail = NULLP;
     }
#endif

   RETVALUE(ROK);

} /* uehRegInitTmr */

/*
 *
 *       Fun:   Activate Task - timers
 *
 *       Desc:  Invoked by system services to activate a task with
 *              a timer tick.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  ueh_tmr.c
 *
 */
PUBLIC S16 uehActvTmr(void)
{
//	logFunc("%s", "uehActvTmr");

   cmPrcTmr(&uehCb.tqCp, uehCb.tq, (PFV) uehProcTmr);

#ifdef UEH_PERF_DL_DATA
   cmPrcTmr(&uehEuCb.tqCp, uehEuCb.tq, (PFV) uehPerfTmrExp);
#endif

   RETVALUE(ROK);

} /* end of uehActvTmr */


/*
*
*       Fun:    uehStartTmr
*
*       Desc:   start a timer
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   ueh_tmr.c
*
*/
void uehStartTmr(
  UehTimerId   timerId,
  PcmTimer    *tmr,
  PTR          cb,
  U32          userId
)
{
  CmTmrArg     arg;
  U8           maxTmrs = 1;
  
  if(uehCb.uehTimerDescTable[timerId].duration EQ  0) RETVOID;

  //tmr->sentEvt = uehCb.uehTimerDescTable[timerId].sentEvt;
  //tmr->expEvt  = uehCb.uehTimerDescTable[timerId].expEvt;

  UECM_TIMERSTART(userId,uehCb.uehTimerDescTable[timerId].name,timerId,uehCb.uehTimerDescTable[timerId].duration);
  
  cmInitTimers( &tmr->cmTimer, maxTmrs);
  
  arg.tqCp   = &(uehCb.tqCp);
  arg.tq     = uehCb.tq;
  arg.cb     = cb;
  arg.timers = &tmr->cmTimer;
  arg.evnt   = timerId;
  arg.wait   = uehCb.uehTimerDescTable[timerId].duration;
  arg.tNum   = 0;
  arg.max    = UEH_MAX_TIMERS;

  cmPlcCbTq(&arg);

  tmr->isTmrRunning = TRUE;

  RETVOID;

}
/*
*
*       Fun:    uehStopTmr
*
*       Desc:   stop a timer
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   ueh_tmr.c
*
*/

PUBLIC Void uehStopTmr(
  UehTimerId   timerId,
  PcmTimer    *tmr,
  PTR          cb,
  U32          userId
  )
{

   CmTmrArg     arg;
   U8           max         = 1;

   UECM_TIMERSTOP(userId,uehCb.uehTimerDescTable[timerId].name,timerId,uehCb.uehTimerDescTable[timerId].duration);

   tmr->isTmrRunning = FALSE;

   /* initialize argument for common timer function */
   arg.tqCp    = &uehCb.tqCp;
   arg.tq      = uehCb.tq; 
   arg.timers  = &tmr->cmTimer;
   arg.cb      = cb;
   arg.evnt    = timerId;
   arg.wait    = 0;
   arg.max     = UEH_MAX_TIMERS;
   arg.tNum    = 0;

   cmRmvCbTq(&arg);

   /* After stopping the timer
    * Reset the timer fields
    */
   cmInitTimers( &tmr->cmTimer,max);
   RETVOID;
}



PUBLIC Void uehStopAllUeTmrs(UehUeCb *ueCb)
{

  STOP_UE_TMR_IF_RUNNING((PTR)ueCb,&(ueCb)->ueRspTmr,UE_TMR_UE_RESPONSE,ueCb->crnti);
  STOP_UE_TMR_IF_RUNNING((PTR)ueCb,&(ueCb)->crntiRtnTmr,UE_TMR_CRNTI_RETENTION,ueCb->crnti);
  STOP_UE_TMR_IF_RUNNING((PTR)ueCb,&(ueCb)->ueCtxRelTmr,UE_TMR_UE_CTX_REL,ueCb->crnti);
  STOP_UE_TMR_IF_RUNNING((PTR)ueCb,&(ueCb)->ueSrb1Tmr,UE_TMR_UE_SRB1,ueCb->crnti);
  STOP_UE_TMR_IF_RUNNING((PTR)ueCb,&(ueCb)->ueRrcConRelL2Ack,UE_TMR_RRCCONNRELL2ACK,ueCb->crnti);
  STOP_UE_TMR_IF_RUNNING((PTR)ueCb,&(ueCb)->mmeRelIndTmr,UEH_TMR_WAIT_MME_REL,ueCb->crnti);
  STOP_UE_TMR_IF_RUNNING((PTR)ueCb,&(ueCb)->ueInternalConfTmr,UE_TMR_INTERNAL_CONF,ueCb->crnti);
  STOP_UE_TMR_IF_RUNNING((PTR)ueCb,&(ueCb)->uePwrReconfTmr,UE_PWR_RECONF_TMR,ueCb->crnti);
  STOP_UE_TMR_IF_RUNNING((PTR)ueCb,&(ueCb)->ueCqiBasedRlfDetectionTmr, UEH_CQI_BASED_RLF_DETECTION_TMR, ueCb->crnti); 
  RETVOID;
}

/*
*
*       Fun:    uehProcTmr
*
*       Desc:   Handle the expiration of timers
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   ueh_tmr.c
*
*/

PRIVATE Void uehProcTmr(
    Ptr            cb,
    UehTimerId     event
  )
{
  UehLiSapCb    *sapCb=NULL;
  UehUeCb       *ueCb=NULL;
  PcmTimer       *tmr=NULL;
  UehCellCb     *cellCb = uehGetCellCb();
  U16            ueIdx=0;
  logFunc("%s", "uehProcTmr");

  logTmr("Timer TimerId=%d Expired", event);

  switch(event)
  {
    /* Check the evt, if the event is SAP Bind timer expiry, then
     * Call the bind function again, If the maximum attempts exhausted then send a Alarm
     */
    case UEH_TMR_NHU_SAP_BND:
      sapCb = (UehLiSapCb *)cb;
      UECM_TIMEREXP(-1,UEH_TMR_NHU_SAP_BND,event);
      if( (++sapCb->bndRetryCnt < sapCb->maxBndRetry) &&
            (sapCb->sapState != LUEH_SAP_BOUND))
      {
        uehStartTmr (UEH_TMR_NHU_SAP_BND,&(( UehLiSapCb*)cb)->timer,(PTR)cb,0);
        uehSndMsgOut (ENTNH, EVENTTYPE_RRCBNDREQ, NULLP, TRANSID_NONE, NULLP);
      }
      else
      {
        sapCb->bndRetryCnt = 0;
        sapCb->sapState    = LUEH_SAP_UNBOUND;
        uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,LUEH_CAUSE_NHU_SAP_BOUND);
      }
    break;
    case UEH_TMR_RGR_SAP_BND:
      sapCb = (UehLiSapCb *)cb;
      UECM_TIMEREXP(-1,UEH_TMR_RGR_SAP_BND,event);
      if(++sapCb->bndRetryCnt < sapCb->maxBndRetry)
      {
        uehStartTmr (UEH_TMR_RGR_SAP_BND,&(( UehLiSapCb*)cb)->timer,(PTR)cb,0);
        UehLiRgrBndReq(&sapCb->pst, sapCb->suId,sapCb->spId);
      }
      else
      {
        sapCb->bndRetryCnt = 0;
        sapCb->sapState    = LUEH_SAP_UNBOUND;
        uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LUEH_CAUSE_RGR_SAP_BOUND);
      }
    break;
 #ifndef RRC_PHY_CFG_SUPP
    case UEH_TMR_CTF_SAP_BND:
      sapCb = (UehLiSapCb *)cb;
      UECM_TIMEREXP(-1,UEH_TMR_CTF_SAP_BND,event);
      if(++sapCb->bndRetryCnt < sapCb->maxBndRetry)
      {
        uehStartTmr (UEH_TMR_CTF_SAP_BND,&(( UehLiSapCb*)cb)->timer,(PTR)cb,0);
        uehSndMsgOut (ENTTF, EVENTTYPE_PHYBNDREQ, NULLP,TRANSID_NONE,NULLP);
      }
      else
      {
        sapCb->bndRetryCnt = 0;
        sapCb->sapState    = LUEH_SAP_UNBOUND;
        uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,LUEH_CAUSE_CTF_SAP_BOUND);
      }
    break;
 #endif
#ifndef LTE_ENB_PERF
    case UEH_TMR_SZT_SAP_BND:
      sapCb = (UehLiSapCb *)cb;
      UECM_TIMEREXP(-1,UEH_TMR_SZT_SAP_BND,event);
      if(++sapCb->bndRetryCnt < sapCb->maxBndRetry)
      {
        uehStartTmr (UEH_TMR_SZT_SAP_BND,&(( UehLiSapCb*)cb)->timer,(PTR)cb,0);
        UehLiSztBndReq(&sapCb->pst, sapCb->suId,sapCb->spId);
      }
      else
      {
        sapCb->bndRetryCnt = 0;
        sapCb->sapState    = LUEH_SAP_UNBOUND;
        uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,LUEH_CAUSE_SZT_SAP_BOUND);
      }
    break;
#if 0
    case UEH_TMR_EGT_SAP_BND:
    {
      sapCb = (UehLiSapCb *)cb;
      UECM_TIMEREXP(-1,UEH_TMR_EGT_SAP_BND,event);
      if(++sapCb->bndRetryCnt < sapCb->maxBndRetry)
      {
         uehStartTmr ((PTR)cb, event, sapCb->bndTmr.val);
         UehLiEgtBndReq(&sapCb->pst, sapCb->suId,sapCb->spId);
      }
      else
      {
         sapCb->bndRetryCnt = 0;
         sapCb->sapState    = LUEH_SAP_UNBOUND;
         uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,LUEH_CAUSE_EGT_SAP_BOUND);
      }
      break;
    }
    case UEH_TMR_PJU_SAP_BND:
    {
      sapCb = (UehLiSapCb *)cb;
      UECM_TIMEREXP(-1,UEH_TMR_PJU_SAP_BND,event);
      if(++sapCb->bndRetryCnt < sapCb->maxBndRetry)
      {
        uehStartTmr ((PTR)cb, event, sapCb->bndTmr.val);
        UehLiPjuBndReq(&sapCb->pst, sapCb->suId,sapCb->spId);
      }
      else
      {
        sapCb->bndRetryCnt = 0;
        sapCb->sapState    = LUEH_SAP_UNBOUND;
        uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LUEH_CAUSE_PJU_SAP_BOUND);
      }
      break;
    }
#endif
#endif

    case UEH_TMR_UDH_SAP_BND:
      sapCb = (UehLiSapCb *)cb;
      UECM_TIMEREXP(-1, UEH_TMR_UDH_SAP_BND, event);
      if(++sapCb->bndRetryCnt < sapCb->maxBndRetry)
      {
        uehStartTmr (UEH_TMR_UDH_SAP_BND,&(( UehLiSapCb*)cb)->timer,(PTR)cb,0);
        uehSndMsgOut (ENTUDH, EVENTTYPE_UDHBNDREQ, NULLP,TRANSID_NONE,NULLP);
      }
      else
      {
        sapCb->bndRetryCnt = 0;
        sapCb->sapState    = LUEH_SAP_UNBOUND;
        uehSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LUEH_CAUSE_UDH_SAP_BOUND);
      }
    break;
    /* MME, asked to retry for S1 Setup after timer Expiry */
    case UEH_TMR_S1AP_SETUP:
      UECM_TIMEREXP(-1,UEH_TMR_S1AP_SETUP,event);
      s1apProcS1setupTmrExpiry( (S1apMgrCb*) cb );
    break;
    /* No Response is received from MME */
    case UEH_TMR_S1AP_PROC_GUARD:
      UECM_TIMEREXP(-1,UEH_TMR_S1AP_PROC_GUARD,event);
      s1apProcS1GuardTmrExpiry( (S1apMgrCb*) cb);
    break;
    /* No Response is received from UE */
    case UE_TMR_UE_RESPONSE:
      ueCb = (UehUeCb *)cb;
      tmr  = &ueCb->ueRspTmr;
      tmr->isTmrRunning = FALSE;
      UEH_UE_TIMER_EXPIRED(ueCb->crnti, UE_TMR_UE_RESPONSE, cellCb->ueRspTmrDur, ueCb->ueRspTmr);
      ueFsm( ueCb, EVENTTYPE_UERESTMREXP, NULL );
    break;
    case UE_TMR_CRNTI_RETENTION:
      ueCb = (UehUeCb *)cb;          
      tmr  = &ueCb->crntiRtnTmr;
      tmr->isTmrRunning = FALSE;
      UEH_UE_TIMER_EXPIRED(ueCb->crnti, event, cellCb->crntiRtnTmrDur, ueCb->crntiRtnTmr);
      ueFsm( ueCb, EVENTTYPE_UECRNTIRETENTMREXP,NULL );
    break;
    case UE_TMR_UE_CTX_REL:
      ueCb = (UehUeCb *)cb;
      tmr  = &ueCb->ueCtxRelTmr;
      tmr->isTmrRunning = FALSE;
      UEH_UE_TIMER_EXPIRED(ueCb->crnti, event, cellCb->ueCtxRelTmrDur, ueCb->ueCtxRelTmr);
      ueFsm( ueCb, EVENTTYPE_UECTXRELTMREXP, NULL );
    break;
    case UE_TMR_UE_SRB1:
      ueCb = (UehUeCb *)cb;
      tmr  = &ueCb->ueSrb1Tmr;
      tmr->isTmrRunning = FALSE;
      UEH_UE_TIMER_EXPIRED(ueCb->crnti, event, cellCb->ueSrb1TmrDur, ueCb->ueSrb1Tmr);
      ueFsm( ueCb, EVENTTYPE_SRB1TMREXP, NULL );
    break;
    case UE_TMR_RRCCONNRELL2ACK:
      ueCb = (UehUeCb *)cb;
      tmr  = &ueCb->ueRrcConRelL2Ack;
      tmr->isTmrRunning = FALSE;
      UEH_UE_TIMER_EXPIRED(ueCb->crnti, event, cellCb->ueRrcConRelL2AckDur, ueCb->ueRrcConRelL2Ack);
      ueFsm( ueCb, EVENTTYPE_RRCCONNRELL2ACKTMREXP, NULL );
    break;
    case UE_TMR_OP_STOP_REQ:
      UECM_TIMEREXP(-1,UE_TMR_OP_STOP_REQ,event);
      crmInvokeCellSm(cellCb, CELL_EVT_OP_STOP_TMR_EXPIRY, NULL);
    break;
  
    case UEH_TMR_WAIT_MME_REL:
      ueCb = (UehUeCb *)cb;
      tmr  = &ueCb->mmeRelIndTmr;
      tmr->isTmrRunning = FALSE;
      UEH_UE_TIMER_EXPIRED(ueCb->crnti, event, cellCb->mmeRelIndTmrDur, ueCb->mmeRelIndTmr);
      ueFsm( ueCb,EVENTTYPE_MMERELINDTMREXP, NULL );
      break;

    case UE_TMR_INTERNAL_CONF:
      ueCb = (UehUeCb *)cb;
      UECM_TIMEREXP(-1,UE_TMR_INTERNAL_CONF,event);
      ueFsm( ueCb,EVENTTYPE_INTCONFTMREXP, NULL );
    break;

    case UE_PWR_RECONF_TMR:
   {
      ueCb = ((UehUeCb *)cb);
      UEH_UE_TIMER_EXPIRED(ueCb->crnti, event, cellCb->uePwrReconfTmrDurinMs, ueCb->uePwrReconfTmr);
      switch(ueCb->ueState)
      {
        case UE_STATE_CALL_CONNECTED:
          if (FALSE EQ  ueCb->reCfgProcComp)
          {
            S1Cause cause = {.causeType = CauseType_RadioNw, 
                             .causeVal = SztCauseRadioNwfailure_in_radio_interface_procedureEnum
                            };
            logError("%d | %d | error=RrcConnReconfCompl NOT received|proc=%s",
               ueCb->enbUeInternalId,
               ueCb->crnti,
               uehGetUeProcedureName(ueCb->ueProcedure)
            ); /* wait for response. Set to true when response=rrcConnRecongCmpl is recd */

           /* Trigger Context Release */
            uehSendUeCtxRelReq(ueCb, cause);
          }
          else 
          {
            logInfo("UePowerReset|RrcReconf|ueId=%d|eNbs1apId=%d|mmeS1apId=%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
            ueCb->ueProcedure = UEH_PUCCH_PWR_REC_ONGOING;
            uehFillAndSndUlPwrReconf(ueCb);
            uehStartTmr (UE_PWR_RECONF_TMR,&((UehUeCb *)cb)->uePwrReconfTmr,(PTR)ueCb,((UehUeCb *)cb)->crnti);
          }
        break;
        case UE_STATE_REL_ONGOING:
           logInfo ("UE_PWR_RECONF_TMR expired in ueState=%d. NOT RESTARTED. OK IN THIS STATE.", ueCb->ueState); 
        break;
        default:
            logError ("UE_PWR_RECONF_TMR expired in ueState=%d. NOT RESTARTED. Is this expected?", ueCb->ueState); 
        break;
      }
      break;
    }
    case UEH_CQI_BASED_RLF_DETECTION_TMR:
    {
      ueCb = (UehUeCb *)cb;
      tmr  = &ueCb->ueCqiBasedRlfDetectionTmr;
      tmr->isTmrRunning = FALSE;
      UEH_UE_TIMER_EXPIRED(ueCb->crnti, event, uehCb.uehCfgCb.cellCfg.ueCqiBasedRlfDetectionTimer, ueCb->ueCqiBasedRlfDetectionTmr);
      logError("CQI Report Not received from MAC, RLF detected for UE=%d", ueCb->crnti);      
      uehProcUeRspTmrExpiryForRlf(ueCb); 
      break;
    }
#ifdef LTE_L2_MEAS
    case UEH_L2M_SAMPLE_PROFILE1:
    case UEH_L2M_SAMPLE_PROFILE2:
      uehSamplingTimerExpiry((UehL2MeasProfileCfg *) cb);
    break;
#endif
/*Handle the UE Ctx Hang Timers Expiry*/
    case UEH_PUCCH_AUDIT_TMR:
    {

      for (ueIdx = 0; ueIdx < cellCb->maxRntiPool; ueIdx++)
      {
        if (-1 != pucchCqiResTable[ueIdx].ueId)
        {
          UEH_GET_UECB_FROM_CRNTI(ueCb ,pucchCqiResTable[ueIdx].ueId);
          if (NULLP == ueCb)
          {
             /* Free the PUCCH Resource for this UE */
             logError ("PUCCH Audit Failed for UEID:%d",pucchCqiResTable[ueIdx].ueId); 
             uehCrmDeleteUe(pucchCqiResTable[ueIdx].ueId);
          }
        }
      }
      uehCb.uehCtxHangDetection.pucchAuditCounter++; 
      uehStartTmr (UEH_PUCCH_AUDIT_TMR, &uehCb.uehCtxHangDetection.pucchAuditTmr, NULLP, 0);
    }
    break;

    case UEH_UESTATE_AUDIT_TMR:
    {

      for (ueIdx = 0; ueIdx < cellCb->maxRntiPool; ueIdx++)
      {
          UEH_GET_UECB_FROM_CRNTI(ueCb, ueIdx);
          if (NULLP NOTEQ ueCb)
          {
             if(uehCtxHangList[ueIdx].isLastStateAuditPassed)
             {
                if((uehCtxHangList[ueIdx].prevUeState EQ ueCb->ueState) &&
                    (ueCb->ueState NOTEQ UE_STATE_CALL_CONNECTED)) 
                {
                  logError ("UESTATE Audit Failed for UEID:%d," \
                            "UEState:%d, UEProcedure:%d eNbS1apId:%d, MmeS1apId:%d", \
                            (ueIdx + uehCb.uehCfgCb.cellCfg.startRnti), ueCb->ueState, ueCb->ueProcedure, \
                            ueCb->s1ConCb.enb_ue_s1ap_id,ueCb->s1ConCb.mme_ue_s1ap_id); 
                  uehCtxHangList[ueIdx].isLastStateAuditPassed = FALSE;
                }
                else
                {
                    uehCtxHangList[ueIdx].prevUeState = ueCb->ueState;
                }
             }
          }
      }
      uehCb.uehCtxHangDetection.ueStateAuditCounter++; 

    uehStartTmr (UEH_UESTATE_AUDIT_TMR,&uehCb.uehCtxHangDetection.ueStateAuditTmr,NULLP,0);
    }
    break;
    case UEH_UEPROCEDURE_AUDIT_TMR:
    {

      for (ueIdx = 0; ueIdx < cellCb->maxRntiPool; ueIdx++)
      {
          UEH_GET_UECB_FROM_CRNTI(ueCb, ueIdx);
          if (NULLP NOTEQ ueCb)
          {
             if(uehCtxHangList[ueIdx].isLastProcedureAuditPassed)
             {
                if((uehCtxHangList[ueIdx].prevUeProcedure EQ ueCb->ueProcedure) &&
                    (ueCb->ueProcedure NOTEQ UEH_NO_PROCEDURE)) 
                {
              logError ("UEPROCEDURE Audit Failed for UEID:%d," \
                        "UEState:%d, UEProcedure:%d eNbS1apId:%d, MmeS1apId:%d", \
                        (ueIdx + uehCb.uehCfgCb.cellCfg.startRnti), ueCb->ueState, ueCb->ueProcedure, \
                        ueCb->s1ConCb.enb_ue_s1ap_id,ueCb->s1ConCb.mme_ue_s1ap_id); 
              uehCtxHangList[ueIdx].isLastProcedureAuditPassed = FALSE;
            }
            else
            {
                uehCtxHangList[ueIdx].prevUeProcedure = ueCb->ueProcedure;
            }
             }
          }
      }
    uehCb.uehCtxHangDetection.ueProcedureAuditCounter++; 
    uehStartTmr (UEH_UEPROCEDURE_AUDIT_TMR, &uehCb.uehCtxHangDetection.ueProcedureAuditTmr, NULLP,0);
    }
    break;

    default:
      UEH_ERROR_ELMNT_OUT_OF_RANGE("TimerId for Expiry", event, UEH_MIN_TIMER_ID, UEH_MAX_TIMER_ID);
    break;
  } 
  RETVOID;
}
/*
        Fun:uehInitTmrTable
        Desc : 
        Ret:     None. 
 *
 *      Notes :  
 *
 *      File:    ueh_mi.c
*/
void uehInitTmrDescTable()
{
  UehConfig          *uehConfig  = &(smCb.cfgCb.uehConfig);
   /*UEH_TMR_NHU_SAP_BND*/
   uehCb.uehTimerDescTable[UEH_TMR_NHU_SAP_BND].duration = uehCb.nhuSap[0]->bndTmr.val;
   uehCb.uehTimerDescTable[UEH_TMR_NHU_SAP_BND].sentEvt = EVENTTYPE_RRCBNDREQ ;
   uehCb.uehTimerDescTable[UEH_TMR_NHU_SAP_BND].expEvt =  EVENTTYPE_OTHER;
   snprintf(&uehCb.uehTimerDescTable[UEH_TMR_NHU_SAP_BND].name[0],MAX_TIMER_NAME_LEN,"UEH_TMR_NHU_SAP_BND");
     
   /*UEH_TMR_RGR_SAP_BND*/
   uehCb.uehTimerDescTable[UEH_TMR_RGR_SAP_BND].duration =  uehCb.rgrSap[0]->bndTmr.val;
   uehCb.uehTimerDescTable[UEH_TMR_RGR_SAP_BND].sentEvt = EVENTTYPE_SCHBNDREQ;
   uehCb.uehTimerDescTable[UEH_TMR_RGR_SAP_BND].expEvt = EVENTTYPE_OTHER;
   snprintf(&uehCb.uehTimerDescTable[UEH_TMR_RGR_SAP_BND].name[0],MAX_TIMER_NAME_LEN,"UEH_TMR_RGR_SAP_BND");
   /*UEH_TMR_CTF_SAP_BND*/
   uehCb.uehTimerDescTable[UEH_TMR_CTF_SAP_BND].duration = uehCb.ctfSap[0]->bndTmr.val; 
   uehCb.uehTimerDescTable[UEH_TMR_CTF_SAP_BND].sentEvt = EVENTTYPE_PHYBNDREQ;
   uehCb.uehTimerDescTable[UEH_TMR_CTF_SAP_BND].expEvt = EVENTTYPE_OTHER;
   snprintf(&uehCb.uehTimerDescTable[UEH_TMR_CTF_SAP_BND].name[0],MAX_TIMER_NAME_LEN,"UEH_TMR_CTF_SAP_BND");
   /*UEH_TMR_SZT_SAP_BND*/
   uehCb.uehTimerDescTable[UEH_TMR_SZT_SAP_BND].duration = uehCb.sztSap[0]->bndTmr.val;  
   uehCb.uehTimerDescTable[UEH_TMR_SZT_SAP_BND].sentEvt = EVENTTYPE_S1BNDREQ;
   uehCb.uehTimerDescTable[UEH_TMR_SZT_SAP_BND].expEvt = EVENTTYPE_OTHER;
   snprintf(&uehCb.uehTimerDescTable[UEH_TMR_SZT_SAP_BND].name[0],MAX_TIMER_NAME_LEN,"UEH_TMR_SZT_SAP_BND");
#if 0
   /*UEH_TMR_EGT_SAP_BND*/
   uehCb.uehTimerDescTable[UEH_TMR_EGT_SAP_BND].duration = 
   uehCb.uehTimerDescTable[UEH_TMR_EGT_SAP_BND].sentEvt = 
   uehCb.uehTimerDescTable[UEH_TMR_EGT_SAP_BND].expEvt = 
   snprintf(&uehCb.uehTimerDescTable[UEH_TMR_EGT_SAP_BND].name[0],MAX_TIMER_NAME_LEN,"UEH_TMR_EGT_SAP_BND");
   /*UEH_TMR_PJU_SAP_BND*/
   uehCb.uehTimerDescTable[UEH_TMR_PJU_SAP_BND].duration = 
   uehCb.uehTimerDescTable[UEH_TMR_PJU_SAP_BND].sentEvt = 
   uehCb.uehTimerDescTable[UEH_TMR_PJU_SAP_BND].expEvt = 
   snprintf(&uehCb.uehTimerDescTable[UEH_TMR_PJU_SAP_BND].name[0],MAX_TIMER_NAME_LEN,"UEH_TMR_PJU_SAP_BND");
#endif
   /*UEH_TMR_UDH_SAP_BND*/
   uehCb.uehTimerDescTable[UEH_TMR_UDH_SAP_BND].duration = uehCb.udhSap[0]->bndTmr.val; 
   uehCb.uehTimerDescTable[UEH_TMR_UDH_SAP_BND].sentEvt = EVENTTYPE_UDHBNDREQ;
   uehCb.uehTimerDescTable[UEH_TMR_UDH_SAP_BND].expEvt = EVENTTYPE_OTHER;
   snprintf(&uehCb.uehTimerDescTable[UEH_TMR_UDH_SAP_BND].name[0],MAX_TIMER_NAME_LEN,"UEH_TMR_UDH_SAP_BND");
   /*UEH_TMR_S1AP_SETUP*/
   uehCb.uehTimerDescTable[UEH_TMR_S1AP_SETUP].sentEvt = EVENTTYPE_OTHER;
   uehCb.uehTimerDescTable[UEH_TMR_S1AP_SETUP].expEvt = EVENTTYPE_OTHER;
   snprintf(&uehCb.uehTimerDescTable[UEH_TMR_S1AP_SETUP].name[0],MAX_TIMER_NAME_LEN,"UEH_TMR_S1AP_SETUP");
   /*UEH_TMR_S1AP_PROC_GUARD*/
   uehCb.uehTimerDescTable[UEH_TMR_S1AP_PROC_GUARD].duration = smCb.cfgCb.s1apConfig.s1apGuardTimer * TMR_UINTS_PER_SEC ;
   uehCb.uehTimerDescTable[UEH_TMR_S1AP_PROC_GUARD].sentEvt = EVENTTYPE_OTHER;
   uehCb.uehTimerDescTable[UEH_TMR_S1AP_PROC_GUARD].expEvt = EVENTTYPE_OTHER;
   snprintf(&uehCb.uehTimerDescTable[UEH_TMR_S1AP_PROC_GUARD].name[0],MAX_TIMER_NAME_LEN,"UEH_TMR_S1AP_PROC_GUARD");
   /*UE_TMR_UE_RESPONSE*/
   uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].duration = uehGetUeRespTmrInMsFromEnum[uehCb.uehCfgCb.cellCfg.ueResponseTimer];
   snprintf(&uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].name[0],MAX_TIMER_NAME_LEN,"UE_TMR_UE_RESPONSE");
   /*UE_TMR_CRNTI_RETENTION*/
   uehCb.uehTimerDescTable[UE_TMR_CRNTI_RETENTION].duration = uehGetTimerValFrmEnum[smCb.cfgCb.uehConfig.crntiRtnTimer];
   uehCb.uehTimerDescTable[UE_TMR_CRNTI_RETENTION].sentEvt = EVENTTYPE_RRCCONNREJ;
   uehCb.uehTimerDescTable[UE_TMR_CRNTI_RETENTION].expEvt = EVENTTYPE_OTHER; 
   snprintf(&uehCb.uehTimerDescTable[UE_TMR_CRNTI_RETENTION].name[0],MAX_TIMER_NAME_LEN,"UE_TMR_CRNTI_RETENTION");
   /*UE_TMR_UE_CTX_REL*/
   uehCb.uehTimerDescTable[UE_TMR_UE_CTX_REL].duration = uehGetTimerValFrmEnum[smCb.cfgCb.uehConfig.contextReleaseTimer];
   uehCb.uehTimerDescTable[UE_TMR_UE_CTX_REL].sentEvt = EVENTTYPE_UECONTEXTRELREQ;
   uehCb.uehTimerDescTable[UE_TMR_UE_CTX_REL].expEvt = EVENTTYPE_UECONTEXTRELCMD;
   snprintf(&uehCb.uehTimerDescTable[UE_TMR_UE_CTX_REL].name[0],MAX_TIMER_NAME_LEN,"UE_TMR_UE_CTX_REL");
   /*UE_TMR_UE_SRB1*/
   uehCb.uehTimerDescTable[UE_TMR_UE_SRB1].duration = uehGetTimerValFrmEnum[smCb.cfgCb.uehConfig.standaloneSrb1Timer];
   uehCb.uehTimerDescTable[UE_TMR_UE_SRB1].sentEvt = EVENTTYPE_INITIALUEMSG;
   uehCb.uehTimerDescTable[UE_TMR_UE_SRB1].expEvt = EVENTTYPE_DLNASTRANSPORTMSG;
   snprintf(&uehCb.uehTimerDescTable[UE_TMR_UE_SRB1].name[0],MAX_TIMER_NAME_LEN,"UE_TMR_UE_SRB1");
   /*UE_TMR_RRCCONNRELL2ACK*/
   uehCb.uehTimerDescTable[UE_TMR_RRCCONNRELL2ACK].duration = uehGetTimerValFrmEnum[smCb.cfgCb.uehConfig.rrcConnRelL2AckTimer];
   uehCb.uehTimerDescTable[UE_TMR_RRCCONNRELL2ACK].sentEvt = EVENTTYPE_RRCCONNRELEASE;
   uehCb.uehTimerDescTable[UE_TMR_RRCCONNRELL2ACK].expEvt = EVENTTYPE_OTHER;
   snprintf(&uehCb.uehTimerDescTable[UE_TMR_RRCCONNRELL2ACK].name[0],MAX_TIMER_NAME_LEN,"UE_TMR_RRCCONNRELL2ACK");
   /*UE_TMR_OP_STOP_REQ*/
   uehCb.uehTimerDescTable[UE_TMR_OP_STOP_REQ].duration = 1000;/*TODO Make this configurable */
   uehCb.uehTimerDescTable[UE_TMR_OP_STOP_REQ].sentEvt = EVENTTYPE_OTHER;
   uehCb.uehTimerDescTable[UE_TMR_OP_STOP_REQ].expEvt = EVENTTYPE_OPSTOPRELEASE;
   snprintf(&uehCb.uehTimerDescTable[UE_TMR_OP_STOP_REQ].name[0],MAX_TIMER_NAME_LEN,"UE_TMR_OP_STOP_REQ");
   /*UEH_TMR_WAIT_MME_REL*/
   uehCb.uehTimerDescTable[UEH_TMR_WAIT_MME_REL].duration = uehGetTimerValFrmEnum[smCb.cfgCb.uehConfig.mmeRelIndTimer];
   snprintf(&uehCb.uehTimerDescTable[UEH_TMR_WAIT_MME_REL].name[0],MAX_TIMER_NAME_LEN,"UEH_TMR_WAIT_MME_REL");
   /*UEH_TMR_S1AP_TS1_RELOC_PREP*/
   uehCb.uehTimerDescTable[UEH_TMR_S1AP_TS1_RELOC_PREP].duration = uehGetUeRespTmrInMsFromEnum [UERESPONSETIMER_MS5000];
   uehCb.uehTimerDescTable[UEH_TMR_S1AP_TS1_RELOC_PREP].sentEvt = EVENTTYPE_OUTHOTRIGGERED;
   uehCb.uehTimerDescTable[UEH_TMR_S1AP_TS1_RELOC_PREP].expEvt = EVENTTYPE_OTHER;
   snprintf(&uehCb.uehTimerDescTable[UEH_TMR_S1AP_TS1_RELOC_PREP].name[0],MAX_TIMER_NAME_LEN,"UEH_TMR_S1AP_TS1_RELOC_PREP");
   /*UE_TMR_INTERNAL_CONF*/
   uehCb.uehTimerDescTable[UE_TMR_INTERNAL_CONF].duration = uehGetTimerValFrmEnum[smCb.cfgCb.uehConfig.internalUeConfigTimer];
   uehCb.uehTimerDescTable[UE_TMR_INTERNAL_CONF].sentEvt = EVENTTYPE_INITCONTEXTSETUPRESP;
   uehCb.uehTimerDescTable[UE_TMR_INTERNAL_CONF].expEvt = EVENTTYPE_INITCONTEXTSETUPREQ;
   snprintf(&uehCb.uehTimerDescTable[UE_TMR_INTERNAL_CONF].name[0],MAX_TIMER_NAME_LEN,"UE_TMR_INTERNAL_CONF");
   /*UE_PWR_RECONF_TMR*/
   uehCb.uehTimerDescTable[UE_PWR_RECONF_TMR].duration = smCb.cfgCb.uehConfig.uePwrReconfTimerinSec * 1000;
   uehCb.uehTimerDescTable[UE_PWR_RECONF_TMR].sentEvt = EVENTTYPE_RRCCONNRECONFIG;
   uehCb.uehTimerDescTable[UE_PWR_RECONF_TMR].expEvt = EVENTTYPE_RRCCONNRECONFCOMPL;
   snprintf(&uehCb.uehTimerDescTable[UE_PWR_RECONF_TMR].name[0],MAX_TIMER_NAME_LEN,"UE_PWR_RECONF_TMR");
   /*UEH_L2M_SAMPLE_PROFILE1*/
   snprintf(&uehCb.uehTimerDescTable[UEH_L2M_SAMPLE_PROFILE1].name[0],MAX_TIMER_NAME_LEN,"UEH_L2M_SAMPLE_PROFILE1");
   /*UEH_L2M_SAMPLE_PROFILE2*/
   snprintf(&uehCb.uehTimerDescTable[UEH_L2M_SAMPLE_PROFILE2].name[0],MAX_TIMER_NAME_LEN,"UEH_L2M_SAMPLE_PROFILE2");
 /* Add the ue ctx hang detection timers to uehTimerDescTable*/
 
 /* UEH PUCCH AUDIT TIMER */
   uehCb.uehTimerDescTable[UEH_PUCCH_AUDIT_TMR].duration = TMR_UINTS_PER_SEC * uehConfig->uehPucchAuditTmrDurInSecs;
   uehCb.uehTimerDescTable[UEH_PUCCH_AUDIT_TMR].sentEvt = EVENTTYPE_OTHER;
   uehCb.uehTimerDescTable[UEH_PUCCH_AUDIT_TMR].expEvt = EVENTTYPE_OTHER;
   snprintf(&uehCb.uehTimerDescTable[UEH_PUCCH_AUDIT_TMR].name[0],MAX_TIMER_NAME_LEN,"UEH_PUCCH_AUDIT_TMR");
   /* UEH UESTATE AUDIT TIMER */
   uehCb.uehTimerDescTable[UEH_UESTATE_AUDIT_TMR].duration = TMR_UINTS_PER_SEC * uehConfig->uehStateAuditTmrDurInSecs;
   uehCb.uehTimerDescTable[UEH_UESTATE_AUDIT_TMR].sentEvt = EVENTTYPE_OTHER;
   uehCb.uehTimerDescTable[UEH_UESTATE_AUDIT_TMR].expEvt = EVENTTYPE_OTHER;
   snprintf(&uehCb.uehTimerDescTable[UEH_UESTATE_AUDIT_TMR].name[0],MAX_TIMER_NAME_LEN,"UEH_UESTATE_AUDIT_TMR");
   /* UEH UEPROCEDURE AUDIT TIMER */
   uehCb.uehTimerDescTable[UEH_UEPROCEDURE_AUDIT_TMR].duration = TMR_UINTS_PER_SEC * uehConfig->uehProcedureAuditTmrDurInSecs;
   uehCb.uehTimerDescTable[UEH_UEPROCEDURE_AUDIT_TMR].sentEvt = EVENTTYPE_OTHER;
   uehCb.uehTimerDescTable[UEH_UEPROCEDURE_AUDIT_TMR].expEvt = EVENTTYPE_OTHER;
   snprintf(&uehCb.uehTimerDescTable[UEH_UEPROCEDURE_AUDIT_TMR].name[0],MAX_TIMER_NAME_LEN,"UEH_UEPROCEDURE_AUDIT_TMR");
   /* UEH RLF DETECTION TIMER */
   uehCb.uehTimerDescTable[UEH_CQI_BASED_RLF_DETECTION_TMR].duration = uehGetUeRlfDetectionTmrInMsFromEnum[uehCb.uehCfgCb.cellCfg.ueCqiBasedRlfDetectionTimer];
   uehCb.uehTimerDescTable[UEH_CQI_BASED_RLF_DETECTION_TMR].sentEvt = EVENTTYPE_OTHER;
   uehCb.uehTimerDescTable[UEH_CQI_BASED_RLF_DETECTION_TMR].expEvt = EVENTTYPE_OTHER;
   snprintf(&uehCb.uehTimerDescTable[UEH_CQI_BASED_RLF_DETECTION_TMR].name[0],MAX_TIMER_NAME_LEN,"UEH_CQI_BASED_RLF_DETECTION_TMR");   
}

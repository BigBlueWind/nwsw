/**
 * @file    ueh_li_hdl.c
 * @brief This file contains lower layer interface handlers for UEH module
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 16Sep2010 :
*     a) Created initial version from sample application
*     b) Shweta Polepally :30th Aug 2011 : Added Code under HO_SUPPORTED
 */

#include "ccpu_cmn.h"
#include "ueh_pmh_if.h"
/* RRM related includes */
#include "ueh_eut.h"
#include "ueh_types.h"
#include "ueh.h" 
#include "lueh.h"
#include "lueh.x" 
#include "ueh_sib.h"
#include "ueh_pucch_res_mgmt.h"
#include "ueh_crm.h"
#include "s1ap_cm.h"
#include "s1ap_wrap.h"
#include "s1ap_unwrap.h"
#include "ueh_s1ap_cm.h"
#include "ueh_srs_res_mgmt.h"
#include "ueh_cmn_sec.h"
#include "log.h"
#include "nh.h"            /* RRC defines */
#include "nh.x"
#include "rgr.x"
#include "ctf.x"
#include "ueh_crm_sm.h"
#include "pst_events.h"
#include "trace.h"
#ifdef PWAV_TS
#include "ts.h"
#endif


#ifdef NHU_VER2
EXTERN U32 NhBcchPcchTbSzTbl[NH_BCCH_PCCH_TBSZTBL_SIZE];
#endif /* NHU_VER1 */
extern U8  uehGetMaxUlHarqTx[];
extern U8  countUe;
extern U8  countProc;
U8 uehDfltDeltaHqOffst = 0;

/* Frees UeCb */
PUBLIC S16 freeUeCb(UehUeCb *ueCb)
{

  UehCellCb   *cellCb = uehGetCellCb();
  //U16          ueIdx  = UEH_GET_UE_IDX(ueCb->crnti);

  if(!ueCb->isUeCbAllocated)
  {
    logError("freeUeCb|crnti=%d|Cb is already free|not freed", ueCb->crnti);
    RETVALUE(RFAILED);
  }
  if(ueCb->acParams.isAllocated EQ FALSE)
  {
    logError("freeueCb|crnti=%d|acParams not allocated|not freed", ueCb->crnti);
    RETVALUE(RFAILED);
  }

  uehStopAllUeTmrs(ueCb);
  uehRecordUeEvnt(ueCb,EVENTTYPE_UEDELETED, NULLP,FALSE);
  /* Free the PUCCH Resource for this UE */
  uehCrmDeleteUe(ueCb->crnti);
  /* Free the SRS Resource for this UE */
  uehCrmDeallocateSrs(uehCrmFindSfIndex(ueCb->ueSrsInfo.ueSrsCfgIndex),
                      ueCb->ueSrsInfo.ueTransmissionComb,
                      ueCb->ueSrsInfo.ueDedCyclicShift);
 
  ueCb->acParams.isAllocated = FALSE;
  ueCb->isUeCbAllocated      = FALSE;
  ueCb->ueState              = UE_STATE_RRC_IDLE;
  ueCb->ueProcedure          = UEH_NO_PROCEDURE; 

  //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus =  uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus | UE_CTX_RLS_SUCC;
  uehCb.s1UeConLst[ueCb->s1ConCb.suConnId] = NULL;


  cellCb->numUeCfg--;

  logProc("iUeId=%d|freeUe|OK|crnti=%d|relCause=%d|mmeUeS1apId=%d|enbUeS1apId=%d|numUeCfg=%d", \
           ueCb->enbUeInternalId, \
           ueCb->crnti, \
           ueCb->ueReleaseCauseVal, \
           ueCb->s1ConCb.mme_ue_s1ap_id,\
           ueCb->s1ConCb.enb_ue_s1ap_id, \
           cellCb->numUeCfg);
  logStateChange("iUeId=%d|freeUe|OK|crnti=%d|relCause=%d|mmeUeS1apId=%d|enbUeS1apId=%d|suConnId=%d|spConnId=%d|numUeCfg=%d", \
           ueCb->enbUeInternalId,
           ueCb->crnti,
           ueCb->ueReleaseCauseVal,
           ueCb->s1ConCb.mme_ue_s1ap_id,
           ueCb->s1ConCb.enb_ue_s1ap_id,
           ueCb->s1ConCb.suConnId,
           ueCb->s1ConCb.spConnId,
           cellCb->numUeCfg);

  ueCb->s1ConCb.suConnId     = 0;
  ueCb->s1ConCb.spConnId     = 0;

  if(0 == cellCb->numUeCfg)
  {
    crmInvokeCellSm(cellCb, CELL_EVT_UE_CNT_ZERO, NULLP);
  }
  RETVALUE(ROK);
}

/* This function will MAP External events received from RRC, S1AP and schedular into RRM specific Events */
U16 uehMsgTypeToIndex(
    U8 msgType,
    U8 msgCat
  )
{

  switch (msgCat)
  {
    case NHU_MSG_ULCCCH:
    {
      if (msgType == C5_RRCCONRQST)
        return EVENTTYPE_RRCCONNREQ;
      else if (msgType == C5_RRCCONREESTBRQST)
        return EVENTTYPE_RRCREESTABREQ;
      break;
    }
    case NHU_MSG_ULDCCH:
    {
      if (msgType == C6_RRCCONRECFGNCOMPL)
        return EVENTTYPE_RRCCONNRECONFCOMPL;
      else if (msgType == C6_RRCCONSETUPCOMPL)
        return EVENTTYPE_RRCCONNSETUPCOMPL;
      else if (msgType == C6_SECURITYMODECOMPL)
        return EVENTTYPE_SECMODECOMPL;
      else if (msgType == C6_SECURITYMODEFAIL)
        return EVENTTYPE_SECMODEFAIL;
      else if (msgType == C6_ULINFOTFR)
        return EVENTTYPE_ULINFTRANSFER;
      else if (msgType == C6_UECAPINFO)
        return EVENTTYPE_UECAPABILITYINFO;
#ifdef COUNTER_CHECK
      else if (msgType == C6_COUNTERCHECKRSP)
        return EVENTTYPE_UECOUNTERCHECKRSP;
#endif
      break;
    }
    case UE_SZ_MSG_CAT:
    {
      if (msgType == SZT_MSG_ID_CXT_SETUP_REQ )
        return EVENTTYPE_INITCONTEXTSETUPREQ;
      else if (msgType == SZT_MSG_ID_UE_CXT_REL_CMD )
        return EVENTTYPE_UECONTEXTRELCMD;
      else if (msgType == SZT_MSG_ID_UE_CXT_MOD_REQ )
        return EVENTTYPE_UECONTEXTMODREQ;

      else if (msgType == SZT_MSG_ID_DL_NAS_TPT )
        return EVENTTYPE_DLNASTRANSPORTMSG;

      else if(msgType == SZT_MSG_ID_LOC_REPORT_CNTRL)
        return EVENTTYPE_LOCATIONREPORTCTRL;

      else if( msgType ==  SZT_MSG_ID_DEACTV_TRACE )
        return EVENTTYPE_DEACTIVATETRACE;
      else if( msgType == SZT_MSG_ID_TRACE_START)
        return EVENTTYPE_TRACESTART;

      else if( msgType == SZT_MSG_ID_E_RAB_BRR_SETUP_REQ)
        return EVENTTYPE_RABSETUPREQ;
      else if( msgType == SZT_MSG_ID_E_RAB_BRR_MOD_REQ)
        return EVENTTYPE_RABMODREQ;
      else if( msgType == SZT_MSG_ID_E_RAB_BRR_REL_CMD)
        return EVENTTYPE_RABRELREQ;

      else if( msgType == SZT_MSG_ID_ERR_IND )
        return EVENTTYPE_S1APERRIND;

      else if( msgType == SZT_MSG_ID_HO_CMD )
        return EVENTTYPE_S1APHOCMD;
      else if( msgType == SZT_MSG_ID_HO_PREP_FAIL )
        return EVENTTYPE_S1APHOPREPFAILED;

      break;
     }
    default:
    {
      return UE_UNSUPPORTED_EVT;
    }
  }
  return UE_UNSUPPORTED_EVT;
}

void uehStartRrcConnClear(
    UehUeCb    *ueCb
    )
{
  UECM_FUNC(ueCb->crnti,uehStartRrcConnClear,UECM,UECM, "RRC CONNECTION CLEAR FUNC" );
  uehSndRrcConRel(ueCb);
}

/* Sends Context release Request to PHY, MAC,RRC and Tunnel delete request to Egtp */
void  uehClearUeCtxInNodeB(
	UehUeCb    *ueCb
	)
{
#ifndef RRC_PHY_CFG_SUPP
  /* send PHY UE Context Release*/
  uehSndCtfUeRls(ueCb);
#endif
  /* send RGR UE Context Release*/
  uehSndRgrUeRls(ueCb);
#if 0
  /* Send NHU UE Context Release */
  UehLiNhuCnclUeReq(&uehCb.nhuSap[0]->pst, uehCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);
#endif
  
  /*Tunnel deletion should be done for each Drb. For now only default*/
  uehSndEgtpCom(EGTP_TUN_DEL, ueCb);

  if(TRUE == ueCb->rbInfo.drb[0].isUdhRabEnbl)
  {
    uehSndUdhDelUeReq(ueCb);
  }

}

/* NO SDU is expected, we receive null for SDU
 * Stop the RRCL2Ack Timer,
 * Clear the PHY*/
PUBLIC S16 uehProcRrcDatCfmInUeCtxRel(
    UehUeCb    *ueCb
  )
{
  SztRelReq      relReq;

  logInfo("uehProcRrcDatCfmInUeCtxRel: UeID=%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id );

  /* Checking Ue Procedure type to avoid repetitive acks from RLC */
  if (UEH_UE_CTXT_CLEAR_ONGOING != ueCb->ueProcedure)
  {
     /* Sends Context release Request to PHY, MAC,RRC and Tunnel delete request to Egtp */

     relReq.relType = SZT_LOCAL_REL;
     relReq.connIdType = SZT_CONN_TYPE_SU;
     relReq.connId = ueCb->s1ConCb.suConnId;
     uehUeProcStart(ueCb,UEH_UE_CTXT_CLEAR_ONGOING);
     uehClearUeCtxInNodeB(ueCb);
  }
  else
  {
     /* No need to clear UE */
     logInfo("%s","UE already freed: L2 timer already expired or this is false nhuDatCfm");
  }
  RETVALUE(ROK);
}


PUBLIC S16 uehProcRrcDatCfmInUeCapOngoing(
    UehUeCb         *ueCb,
    NhuDatCfmSdus   *nhDatCfmSdus
  )
{

  logInfo("uehProcRrcDatCfmInUeCapOngoing: RRC Data Confirm received for UeId: %d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  if (NHU_ERRCAUSE_NONE NOTEQ nhDatCfmSdus->datCfmStatus)
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUDATCFM , ENTNH, nhDatCfmSdus->datCfmStatus, UE, ueCb->crnti);
    /*TODO: Failure case actions needs to be taken here */
  }

  RETVALUE(ROK);
}


PUBLIC S16 uehProcRrcDatCfmInRecOngoing(
    UehUeCb        *ueCb,
    NhuDatCfmSdus  *nhDatCfmSdus
  )
{

  logInfo("uehProcRrcDatCfmInRecOngoing: RRC Data Confirm received for UeId: %d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id); 

  if (NHU_ERRCAUSE_NONE NOTEQ nhDatCfmSdus->datCfmStatus)
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUDATCFM , ENTNH, nhDatCfmSdus->datCfmStatus, UE, ueCb->crnti);
    /*TODO: Failure case actions needs to be taken here */
  }

  RETVALUE(ROK);
}
/*
 *
 *      Fun:   uehProcRrcDatCfmInRRCConnected
 *
 *      Desc:  Dat Cfm
 *
 *      Ret:   ROK
 *
 *      Notes: This function is invoked when data cfm is received from Lower Layers
 *       		in RRC connected state. It targets the cfm for UE Capability Enquiry message
 *       		for detecting UE UL Inactivity. If there is a RL failure then it clears the
 *       		UE resources and stops the RESPONSE TIMER
 *
 *      File:  ueh_li_hdl.c
 *
 */

/* TODO:Ankur 31 July 2012 Commented Unused function

PUBLIC S16 uehProcRrcDatCfmInRRCConnected(
    UehUeCb        *ueCb,
    NhuDatCfmSdus  *nhDatCfmSdus
  )
{

  logInfo("uehProcRrcDatCfmInRRCConnected: RRC Data Confirm received for UeId: %d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);


  if (NHU_ERRCAUSE_MAX_RLC_TRANS_REACH  EQ  nhDatCfmSdus->datCfmStatus)
  {
	  logInfo("uehProcRrcDatCfmInRRCConnected: MAX retransmissions reached. Releasing the connection with crnti: %d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id); 
	  uehStopTmr((PTR)ueCb, UE_TMR_UE_RESPONSE);
	  uehProcUeRspTmrExpiryForRlf(ueCb);
  }

  RETVALUE(ROK);
}

*/



/*
 *
 *      Fun:   uehHdlNhuDatInd
 *
 *      Desc:  Dat Ind
 *
 *      Ret:   ROK
 *
 *      Notes: This function is invoked when data is received from Lower Layers,
 *      Check that the event is freed by caller
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehHdlNhuDatInd(
    NhuDatIndSdus       *datIndSdu
  )
{
  NhuUL_CCCH_Msg       *ulCcchPdu = NULLP;  /* UL CCCH structure */
  NhuUL_DCCH_Msg       *ulDcchPdu = NULLP;  /* UL DCCH structure */
  NhuCrnti             crnti = 0;
  NhuReestabUE_Identity  *ueIdentity = NULLP;
  U8                   messageType = 0;
  UehUeCb              *ueCb = NULLP;
  U16                  msgIndex = 0;
  UehCellCb            *cellCb = uehGetCellCb();
  U16                  ueIdx;
  U32                  cause;
  U8                   messageCat = datIndSdu->sdu.msgCategory;
  /*int                  ueId=-1;
    char                 details[30];*/

  logFunc("%s", "uehHdlNhuDatInd");

  /* Decode the message category,
   * Check whether If message is received on CCCH or DCCH,
   * Check for the received PDU, decode the message ID, If message is supported then create a
   * control block */

  if (NHU_MSG_ULCCCH == messageCat)
  {
    ulCcchPdu = &datIndSdu->sdu.m.ulCcchMsg.ccchMsg;
    if( FALSE == ulCcchPdu->pres.pres )
    {
      UEH_ERROR_NULL_POINTER_IDENTIFIED();
      RETVALUE(RFAILED);
    }
    messageType = ulCcchPdu->message.val.c1.choice.val;
    msgIndex = uehMsgTypeToIndex(messageType, messageCat);
    /* Only RRC-Connection request and Re-establishment are supported on CCCH
     * Create UE control block, for connection request, if already present search the ueCb
     * TODO Should we check for tmpCrnti valid or not?
     * Invoke the state machine at the end
     */
    if (msgIndex == EVENTTYPE_RRCCONNREQ)
    {
      cause = datIndSdu->sdu.m.ulCcchMsg.ccchMsg.message.val.c1.val.rrcConRqst.criticalExtns.val.rrcConRqst_r8.establishmentCause.val;
      crnti = datIndSdu->sdu.m.ulCcchMsg.tmpCrnti;
      logInfo("Received RRCCONNREQ:ueId=%d", crnti);


      ueCb = AdmissionRequest(cause, crnti);

      if (NULLP == ueCb)
      {
        UehUeCb cb; /* dummy - only to record data */
        cb.isUeCbAllocated = FALSE;
        cb.cellId = datIndSdu->hdr.cellId;
        cb.ueId   = datIndSdu->hdr.ueId;
        cb.crnti  = crnti;
        uehRecordUeEvnt( &cb, msgIndex, datIndSdu,TRUE );
        uehSndRrcConRej(&cb);
        RETVALUE(ROK);
      }
/* Add the entry to uehCtxHangList when ueCb created*/
      ueIdx  = UEH_GET_UE_IDX(crnti);
      uehCtxHangList[ueIdx].prevUeState =  UE_MAX_STATE;
      uehCtxHangList[ueIdx].isLastStateAuditPassed = TRUE;
      uehCtxHangList[ueIdx].prevUeProcedure = UEH_NO_PROCEDURE;
      uehCtxHangList[ueIdx].isLastProcedureAuditPassed = TRUE;

      logInfo("RRCCONNREQ:ueId=%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id); 
    }
    else if (msgIndex == EVENTTYPE_RRCREESTABREQ)
    {
      ueIdentity = &(datIndSdu->sdu.m.ulCcchMsg.ccchMsg.message.val.c1.val.rrcConReestbRqst.criticalExtns.val.rrcConReestbRqst_r8.ue_Identity);
      logInfo("Received RRCREESTREQ|ue_Id=%d|crnti=%d", ueIdentity, crnti);
      crnti = GET_CRNTI_FROM_UEID (ueIdentity);
      ueIdx = UEH_GET_UE_IDX(crnti);
      UEH_GET_UECB_FROM_CRNTI(ueCb, crnti);
      if((ueIdx >= cellCb->maxRntiPool) || (NULLP == ueCb))
      {
        UEH_ERROR_NO_CB_LOCATED( UE, crnti, EVTNHUDATIND, ENTNH);
        if (ROK != uehSndRrcConReEstblRej(NULLP, datIndSdu->hdr, datIndSdu->sdu.m.ulCcchMsg.tmpCrnti))
        {
          UEH_ERROR_SSI_EVENT_SEND_FAILED( (&uehCb.nhuSap[0]->pst), EVENTTYPE_RRCREESTABREJ, UE, crnti);
          RETVALUE(RFAILED);
        }
        if (ROK != uehSndRgrUeRlsOnRrcReEstabFail (datIndSdu->sdu.m.ulCcchMsg.tmpCrnti))
        {
          UEH_ERROR_SSI_EVENT_SEND_FAILED( (&uehCb.rgrSap[0]->pst), EVENTTYPE_SCHUEDELETE, UE, datIndSdu->sdu.m.ulCcchMsg.tmpCrnti);
          RETVALUE(RFAILED);
        }
        RETVALUE(ROK);
      }
      logInfo("RRCREESTREQ ueid:%d, eNbs1apId:%d, mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id); 
    }

    ueFsm(ueCb, msgIndex, (void *)datIndSdu);

    RETVALUE(ROK);
  }
  else if (NHU_MSG_ULDCCH == datIndSdu->sdu.msgCategory)
  {
    ulDcchPdu = &(datIndSdu->sdu.m.ulDcchMsg.dcchMsg);
    if ((FALSE == ulDcchPdu->pres.pres) )
    {
      UEH_ERROR_NULL_POINTER_IDENTIFIED();
      RETVALUE(RFAILED);
    }
    messageType = ulDcchPdu->message.val.c1.choice.val;
    msgIndex = uehMsgTypeToIndex(messageType, messageCat);

    if (msgIndex == UE_UNSUPPORTED_EVT)
    {
      UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("Message Index", msgIndex, "FSM Event");
      RETVALUE(RFAILED);
    }
    
    UEH_GET_UECB_FROM_CRNTI(ueCb,datIndSdu->hdr.ueId);
    if(NULLP EQ ueCb)
    {
      UEH_ERROR_NO_CB_LOCATED( UE, datIndSdu->hdr.ueId, EVTNHUDATIND, ENTNH);
      RETVALUE(RFAILED);
    }

    ueFsm(ueCb, msgIndex, (void *) datIndSdu);
  }
  RETVALUE(ROK);
}

#ifndef SI_NEW
/*
 *
 *      Fun:   uehSndCmnNhuDatReq
 *
 *      Desc:  Common Dat Req
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndCmnNhuDatReq(
    U8               msgCategory,
    U8               msgType,
    CmLteTimingInfo  tmnInf
  )
{
  Pst             *pst = NULLP;
  S16             ret = RFAILED;
  NhuDatReqSdus   *nhuDatReqEvntSdu = NULLP;

   logFunc("%s", "uehSndCmnNhuDatReq");

  UEH_ALLOCEVNT_OR_RET(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
  nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo.sfn = tmnInf.sfn;
  nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo.subframe = tmnInf.subframe;

  if(ROK NOTEQ uehFillSysInfMsg(nhuDatReqEvntSdu, msgCategory, msgType))
  {
    UEH_ERROR_MSG_FILLING_FAILED( "System Information Message", CELL, (uehGetCellCb())->cellId);
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(ret);
  }

  pst = &uehCb.nhuSap[0]->pst;
  pst->event = EVTNHUDATREQ;

  /* Send the Prepared MIB/SIB1/SIB2 to User */
  if(ROK NOTEQ UehLiNhuDatReq(pst, uehCb.nhuSap[0]->spId, nhuDatReqEvntSdu))
  {
    UEH_ERROR_SSI_EVENT_SEND_FAILED(pst, msgType, CELL, (uehGetCellCb())->cellId);
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(ret);
  }

  RETVALUE(ROK);
} /* uehSndCmnNhuDatReq */
#endif /* SI_NEW */

/*
 *
 *      Fun:   uehFillSysInfMsg
 *
 *      Desc:  Ueh Fill System Information Message
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehFillSysInfMsg(
    NhuDatReqSdus *nhuDatReqEvntSdu,
    U8            msgCategory,
    U8            msgType
  )
{
  S16          ret     = RFAILED;
  U16          cellId  = 0;
  UehCellCb    *cellCb = NULLP;

  TRC2(uehFillSysInfMsg);

  cellCb = uehGetCellCb();
  /* Cell ID */
  cellId = cellCb->cellId;
  /* Fill HDR Info */
  uehFillNhuEvntHdr(&(nhuDatReqEvntSdu->hdr), cellId, UEH_VAL_ZERO, msgType);
  nhuDatReqEvntSdu->sdu.isUeCfgPres = FALSE;

  switch(msgCategory)
  {
    case NHU_MSG_DLBCCHBCH:
    {
      nhuDatReqEvntSdu->sdu.ueMsg.isMsgDed = FALSE;
      nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory = NHU_MSG_DLBCCHBCH;
#ifndef NHU_VER2
      //nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.tbsSize = 4/*uehDfltBchTbsSz*/;
#endif
      switch(msgType)
      {
        case UEH_SYS_MIB:
        {
          NhuBCCH_BCH_Msg   *bchMsg = &(nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchBchMsg);
          NhuTimingInfo     *timingInfo = &(nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo);

          nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId = BCCH_BCH_ID;

          UEH_FILL_TKN_UINT(bchMsg->pres, PRSNT_NODEF);

          ret = uehFillMibMsg((Ptr)nhuDatReqEvntSdu, &(bchMsg->message), timingInfo);
          if (ROK NOTEQ ret)
          {
            UEH_ERROR_MSG_FILLING_FAILED("MIB Message" , CELL, cellId);
            RETVALUE(RFAILED);
          }
          break;
        }

        default:
          break;
      }
      break;
    }
    case NHU_MSG_DLBCCHDLSCH:
    {
      /* Fill the message category */
      nhuDatReqEvntSdu->sdu.ueMsg.isMsgDed = FALSE;
      nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory = NHU_MSG_DLBCCHDLSCH;
#ifndef NHU_VER2
      nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.tbsSize = uehDfltDlschTbsSz;
#endif
      switch(msgType)
      {
        case UEH_SYS_SIB1:
        {
          NhuTimingInfo         *timingInfo = &(nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo);
          NhuSysInfoBlockTyp1   *sib1       = NULLP;
          
          nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId = BCCH_SIB1_ID;
          sib1 = &(nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg.message.val.c1.val.systemInfoBlockTyp1);   
          /*This check is for filling the SIB1 parameters if the following condtions are satisfied
           * 1) if SIB1 is modified and 
           * 2) any of the below condition is true
           *    i)  sib1 is filling for first time or
           *    ii) modification period is met*/
          if ((cellCb->isSysInfoNeedToSend EQ TRUE) && ((timingInfo->sfn % cellCb->modiPeriodTmr) EQ 0))
          {
            logProc("SYS-INFO-CHANGED at %d", timingInfo->sfn);
            copyBytes(&(uehCb.modiUehCfgCb), sizeof (UehCfgCb), &(uehCb.uehCfgCb));
            cellCb->isSysInfoNeedToSend = FALSE;
          }
          uehFillCmnSib1(nhuDatReqEvntSdu);
          ret = uehFillSib1Msg((Ptr)nhuDatReqEvntSdu, sib1);
          if (ROK NOTEQ ret)
          {
            UEH_ERROR_MSG_FILLING_FAILED("SIB1 Message", CELL, cellId);
            RETVALUE(RFAILED);
          }
        }
        break;

        case UEH_SYS_SIB2:
        {
          NhuSysInfoBlockTyp2          *sib2 = NULLP;
          NhuBCCH_DL_SCH_Msg           *bcchSchMsg = NULLP;

          bcchSchMsg = &(nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg);
          nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId = BCCH_SIB1_ID;

          if (uehFillCmnSib2((Ptr)nhuDatReqEvntSdu, bcchSchMsg) == ROK)
           {

          sib2 = &(bcchSchMsg->message.val.c1.val.systemInfo.criticalExtns.val.systemInfo_r8.sib_TypAndInfo.member[0].val.sib2);
          ret = uehFillSib2Msg((Ptr)nhuDatReqEvntSdu, sib2);
          if (ROK NOTEQ ret)
          {
            UEH_ERROR_MSG_FILLING_FAILED("SIB2 Message", CELL, cellId);
            RETVALUE(RFAILED);
          }
          }
          else
          {
                    logError("SIB2 Message Memory Allocation Failed, message dropped: %d",cellId);
                    RETVALUE(RFAILED);
              }
          break;
        }

        case UEH_SYS_SIB3:
        {
          nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId = BCCH_SIB_OTHERS_ID;
          ret = uehFillSib3Msg(nhuDatReqEvntSdu);
          if (ROK NOTEQ ret)
          {
            UEH_ERROR_MSG_FILLING_FAILED("SIB3 Message", CELL, cellId);
            RETVALUE(RFAILED);
          }
          break;
        }
#ifdef PWAV_SIB4
        case UEH_SYS_SIB4:
        {
          nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId = BCCH_SIB_OTHERS_ID;
          ret = uehFillSib4Msg(nhuDatReqEvntSdu);
          if (ROK NOTEQ ret)
          {
            UEH_ERROR_MSG_FILLING_FAILED("SIB4 Message", CELL, cellId);
            RETVALUE(RFAILED);
          }
          break;
        }
#endif
#ifdef PWAV_SIB5
        case UEH_SYS_SIB5:
        {
          nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId = BCCH_SIB_OTHERS_ID;
          ret = uehSibBldSIB5Msg(nhuDatReqEvntSdu);
          if (ROK NOTEQ ret)
          {
            UEH_ERROR_MSG_FILLING_FAILED("SIB5 Message", CELL, cellId);
            RETVALUE(RFAILED);
          }
          break;
        }
#endif
        default:
          break;
      }
    }
    break;

    default:
      /* Shouldnt be here */
      break;
  }
  RETVALUE(ROK);

} /* uehFillSysInfMsg */


#ifdef SI_NEW
/*
 *
 *      Fun:   uehEncSibPdu
 *
 *      Desc:  ASN encode for SIB pdu
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehEncSibPdu(
    NhuDatReqSdus *nhuDatReqEvntSdu,
    Buffer        **encBuf
  )
{
  S16           ret = RFAILED;
  U32           length;
  U32           idx;
  U32           padOctets;
  U32           msgOctets;
#ifdef NHU_VER1
  U32           tbSize;
#endif /* NHU_VER1 */
  Void          *pdu = NULLP;
  NhuCmnMsg     *cmnMsg = NULLP;

  TRC2(uehEncSibPdu);

  cmnMsg = &(nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg);
  /* Encode the System info */
  if (NHU_MSG_DLBCCHBCH EQ cmnMsg->cmnMsgCategory)
  {
    pdu = (Void *) &(cmnMsg->cmn.bcchBchMsg);
  }
  else
  {
    pdu = (Void *) &(cmnMsg->cmn.bcchDlSchMsg);
  }

  ret = nhEDMEncReq(pdu, cmnMsg->cmnMsgCategory, encBuf, &length);
  if (ret NOTEQ ROK)
  {
    /* Handle encode error here */
    RETVALUE(ret);
  }

  /* Address padding requirement */
  msgOctets = (length/8) + ((length%8)==0? 0:1);
#ifdef NHU_VER1
  NH_GET_BCCH_PCCH_TBSZ((msgOctets*8), tbSize);

  padOctets = (tbSize/8) - msgOctets;
#else /* NHU_VER1 */
  padOctets = (cmnMsg->cmnMsgInfo.tbsSize) - msgOctets;

  if (padOctets < 0)
  {
    /* Handle the length error */
    UEH_ERROR_ELMNT_OUT_OF_RANGE("Message Octets", msgOctets, 0, cmnMsg->cmnMsgInfo.tbsSize);
    UEH_FREEMBUF(*encBuf);
    RETVALUE(RFAILED);
  }
#endif

  for (idx=0; idx < padOctets; idx++)
  {
    SAddPstMsg(0, *encBuf);
  }
  
  RETVALUE(ROK);
}
#endif /* SI_NEW */

/*
 *
 *      Fun:   uehSndUENhuDatReq
 *
 *      Desc:  UE NHU Dat Req
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndUENhuDatReq(
    NhuDatReqSdus *evt
  )
{
   Pst           *pst = NULLP;

   logFunc("%s", "uehSndUENhuDatReq");

   pst = &uehCb.nhuSap[0]->pst;
   pst->event   = EVTNHUDATREQ;
   if(ROK != UehLiNhuDatReq(pst, uehCb.nhuSap[0]->spId, evt))
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehHdlNhuDatRsp
 *
 *      Desc:  Send Data Response
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehHdlNhuDatRsp(
    NhuDatRspSdus *nhuDatRspEvntSdu
  )
{
   Pst           *pst = NULLP;

   logFunc("%s", "uehHdlNhuDatRsp");

   pst = &uehCb.nhuSap[0]->pst;
   pst->event   = EVTNHUDATREQ;

   /* Issue the primitive */
   return ( UehLiNhuDatRsp(pst, uehCb.nhuSap[0]->spId, nhuDatRspEvntSdu) );
}


/* ==================================== */
/* RRC Downlink Message Handlers  */
/* ==================================== */
/*
 *
 *      Fun:   uehSndRrcConRej 
 *
 *      Desc:  RRC Con Rej
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */


PUBLIC S16 uehSndRrcConRej(
   UehUeCb   *cb
  )
{
  NhuDatRspSdus   *nhuDatRspEvntSdu = NULLP;
  
  /* Allocate memory for event structure,
   * Fill HDR Info
   * Build RRC Con Reject Message */
  UEH_ALLOCEVNT_OR_RET(&nhuDatRspEvntSdu, sizeof(NhuDatRspSdus));
  uehFillNhuEvntHdr(&(nhuDatRspEvntSdu->hdr), cb->cellId, cb->ueId, C3_RRCCONREJ);
  nhuDatRspEvntSdu->sdu.m.dlCcchMsg.rbId      = CCCH_DL_ID;
  nhuDatRspEvntSdu->sdu.m.dlCcchMsg.tmpCrnti  = cb->crnti;

  uehFillRRC_ConReject( nhuDatRspEvntSdu);
 
  /* Send RRC Connection Reject through NhuDatRsp Primitive*/
  RETVALUE(uehSndMsgOut (ENTNH, EVENTTYPE_RRCCONNREJ, cb, TRANSID_NONE, nhuDatRspEvntSdu));
}

/*
 *
 *      Fun:   uehSndRrcConReqSetup
 *
 *      Desc:  RRC Con Req Setup
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */

PUBLIC S16 uehSndRrcConSetup(
    UehUeCb       *ueCb
  )
{
  S16           ret = RFAILED;
  NhuDatRspSdus *nhuDatRspEvntSdu = NULLP;
  NhuUeCfgInfo  *ueCfg = NULLP;

  logFunc("%s", "uehSndRrcConSetup");
  
  /* Allocate memory for event structure.
   * Fill HDR Info
   * Build RRC Con Setup Message */
  UEH_ALLOCEVNT_OR_RET(&nhuDatRspEvntSdu, sizeof(NhuDatRspSdus));
  uehFillNhuEvntHdr(&(nhuDatRspEvntSdu->hdr), ueCb->cellId, ueCb->ueId, C3_RRCCONSETUP);
  nhuDatRspEvntSdu->sdu.m.dlCcchMsg.rbId = CCCH_DL_ID;
  nhuDatRspEvntSdu->sdu.m.dlCcchMsg.tmpCrnti = ueCb->crnti;

  ret = uehFillRRC_ConSetup(nhuDatRspEvntSdu, ueCb);
  if (ret != ROK)
  {
    UEH_ERROR_MSG_FILLING_FAILED("RRC Connection Setup", UE, ueCb->crnti);
    UEH_FREEEVNT(nhuDatRspEvntSdu);
    uehSndRrcConRej(ueCb);
    RETVALUE(RFAILED);
  }
  
  /* Fill the UE configuration information that should be applied at lower layers */
  nhuDatRspEvntSdu->sdu.isUeCfgPres = TRUE;
  ueCfg = &(nhuDatRspEvntSdu->sdu.ueCfg);
  ret = uehFillNhuUeCfgInfo(nhuDatRspEvntSdu, ueCfg, ueCb, C3_RRCCONSETUP);
  if (ret != ROK)
  {
     UEH_ERROR_MSG_FILLING_FAILED("RRC UE Config Info", UE, ueCb->crnti);
     UEH_FREEEVNT(nhuDatRspEvntSdu);
     uehSndRrcConRej(ueCb);
     RETVALUE(RFAILED);
  }
  
  fillDataAndTimeStampField(ueCb->ueEntryTime);
  /* Send RRC Connection Setup through NhuDatRsp Primitive */
  if (RFAILED == uehSndMsgOut (ENTNH, EVENTTYPE_RRCCONNSETUP, ueCb, TRANSID_NONE, nhuDatRspEvntSdu))
  {
    UEH_FREEEVNT(nhuDatRspEvntSdu);
    RETVALUE(RFAILED);
  }
  logInfo("RRC Connection Setup sent to UE:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id); 

  RETVALUE(ROK);
}

PUBLIC S16 uehProcRrcConReq(
    UehUeCb       *ueCb,
    NhuDatIndSdus *datIndSdu
  )
{
  NhuInitUE_Identity    *ueID = NULLP;
  RgrCfgReqInfo         *cfgInfo;
  
  logFunc("%s", "uehProcRrcConReq");
  /* Initialize SRB1 related info */
  uehInitUeSrbInfo(&ueCb->rbInfo, UEH_PDCP_ID_SRB1);
  
  ueCb->ueId = datIndSdu->hdr.ueId; /*Store NH Ue Id*/
  ueCb->cellId = datIndSdu->hdr.cellId;
  ueCb->establishCause = datIndSdu->sdu.m.ulCcchMsg.ccchMsg.message.val.c1.val.rrcConRqst.criticalExtns.val.rrcConRqst_r8.establishmentCause.val;
  /* Extract the connection Reference
   * Save establishmentCause, S-TMSI for S1AP */
  ueID = &(datIndSdu->sdu.m.ulCcchMsg.ccchMsg.message.val.c1.val.rrcConRqst.criticalExtns.val.rrcConRqst_r8.ue_Identity);

   UEH_ALLOC(&cfgInfo, sizeof(RgrCfgReqInfo));
   if(NULLP EQ cfgInfo)
   {
      UEH_ERROR_MEM_ALLOC_FAILED(sizeof(RgrCfgReqInfo), "RgrCfgReqInfo|msg dropped");
      /** TODO: how will ueCb be freed */
      RETVALUE(RFAILED);
   }

   UehPrepRgrCfgReq(ueCb, cfgInfo);
   uehSndRgrUeCfgReq(ueCb, cfgInfo);
#ifndef RRC_PHY_CFG_SUPP
   uehSndCtfUeCfg(ueCb);
#endif 
  uehUeProcStart(ueCb,UEH_RRC_CON_SETUP);
  uehSetState(ueCb, UE_STATE_RRC_CONNECTING);
  
  RETVALUE(ROK);
}

/*      Fun:    uehSndRrcConReEstblRej
 *
 *      Desc:   RRC Connection Re-Establishment Reject
 *
 *      Ret:    ROK
 *
 *      Notes: 
 *       
 *        File:  ueh_li_hdl.c
 */
PUBLIC S16 uehSndRrcConReEstblRej(
    UehUeCb   *ueCb,
    NhuHdr    hdr, 
    NhuCrnti  tmpCrnti
  )
{
  NhuDatRspSdus   *nhuDatRspEvntSdu = NULLP;

    UEH_ALLOCEVNT_OR_RET(&nhuDatRspEvntSdu, sizeof(NhuDatRspSdus));
    nhuDatRspEvntSdu->hdr = hdr;
    nhuDatRspEvntSdu->sdu.m.dlCcchMsg.rbId = CCCH_DL_ID;
    nhuDatRspEvntSdu->sdu.m.dlCcchMsg.tmpCrnti = tmpCrnti;
    uehFillRRC_ConReEstblReject (nhuDatRspEvntSdu);

  /* Send RRC Connection Reject through NhuDatRsp Primitive*/
  RETVALUE(uehSndMsgOut(ENTNH, EVENTTYPE_RRCREESTABREJ, ueCb, TRANSID_NONE, nhuDatRspEvntSdu));
}
TBOOL isUehUeProcFailed(
  UehUeCb    *ueCb,
  ProceId     procId,
  TBOOL       condition
  )
{
  /* The below code checks the incoming condition
     if true -> it prints corresponding failure procId received from the calling function  
  */
  if(condition == TRUE)
  {
    logInfo("the procedure failed with procId as %d",procId);
    return TRUE;
  }
  else
  {
   return FALSE;
  }
}       

/*      Fun:    uehProcRrcConReEstblReq
 *
 *      Desc:   RRC Connection Re-Establishment Request
 *
 *      Ret:    ROK
 *
 *      Notes: 
 *              get the t-RNTI
 *              Send RRC Connection Re-Estbalishment Reject
 *              get ueCb for c-RNTI
 *              Send UE Context Release Req
 *
 *       File:  ueh_li_hdl.c
 */
PUBLIC S16 uehProcRrcConReEstblReq ( 
    UehUeCb        *ueCb,
    NhuDatIndSdus  *nhuSdu
  )
{
   S1Cause cause = { .causeType = CauseType_RadioNw, .causeVal = SztCauseRadioNwradio_connection_with_ue_lostEnum };
   if (ROK != uehSndRrcConReEstblRej( ueCb,nhuSdu->hdr, nhuSdu->sdu.m.ulCcchMsg.tmpCrnti)) { RETVALUE(RFAILED); }
   if (ROK != uehSndRgrUeRlsOnRrcReEstabFail (nhuSdu->sdu.m.ulCcchMsg.tmpCrnti)) { RETVALUE(RFAILED); }
   if (ROK != uehSndS1UeCntxtRlsReq(ueCb, &cause)) { RETVALUE(RFAILED); }

   uehUeProcStart(ueCb,UEH_UE_CXT_REL_DUE_TO_RL_FAIL);
   
   return ROK;
}
/*
 *
 *      Fun:   uehSndRrcRecfgReq
 *
 *      Desc:  RRC Reconfig Req
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndRrcRecfgReq(
    UehUeCb        *ueCb
  )
{
  S16       ret = RFAILED;
  NhuDatReqSdus *nhuDatReqEvntSdu = NULLP;
  NhuUeCfgInfo  *ueCfg = NULLP;

  logFunc("%s", "uehSndRrcRecfgReq");
   
  /* Allocate memory for event structure,
   * Fill HDR Info,
   * Build RRCReconfiguration Message */
  UEH_ALLOCEVNT_OR_RET(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));

  uehFillNhuEvntHdr( &(nhuDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_RRCCONRECFGN );
  uehFillNhuRrcConReCfgn(ueCb, nhuDatReqEvntSdu);
  nhuDatReqEvntSdu->sdu.isUeCfgPres = TRUE;
  ueCfg = &(nhuDatReqEvntSdu->sdu.ueCfg);
  ret = uehFillNhuUeCfgInfo(nhuDatReqEvntSdu, ueCfg, ueCb, C4_RRCCONRECFGN);
  if (ret != ROK)
  {
    UEH_ERROR_MSG_FILLING_FAILED("RRC Connection Reconfig Request", UE, ueCb->crnti);
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    /* Sends the GTP tunnel Del and 
     * Initial Context Setup Fail */
    uehSndEgtpDelInitCtxtSetupFail(ueCb, CauseType_RadioNw, SztCauseRadioNwradio_resources_not_availableEnum);
    RETVALUE(RFAILED);
  }

  /* Send RRCReconfigMessage through NhuDatRsp Primitive*/
  if (RFAILED EQ uehSndMsgOut (ENTNH, EVENTTYPE_RRCCONNRECONFIG, ueCb, TRANSID_NONE, nhuDatReqEvntSdu))
  { 
   RETVALUE(RFAILED);
  }
  logInfo("Sent=RRC Conn Reconf | ueId=%d",ueCb->crnti);
  RETVALUE(ROK);
}


/*
 *
 *      Fun:   uehSndRrcDlInfoTfr
 *
 *      Desc:  RRC DL Info Tfr
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndRrcDlInfoTfr(
    UehUeCb     *ueCb,
    U16         nasPduLen,
    U8          *nasPdu
  )
{
  SztDatEvntReq      datEvt;
  NasNonDeliveryInd  nasNonDelInd;
  S16                ret = RFAILED;
  NhuDatReqSdus      *nhuDatReqEvntSdu = NULLP;

  logFunc("%s", "uehSndRrcDlInfoTfr");

  /* Allocate the message,
   * Fill HDR Info,
   * Build DlNAS Message  */
  UEH_ALLOCEVNT_OR_RET(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
  uehFillNhuEvntHdr(&(nhuDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_DLINFOTFR);
  nhuDatReqEvntSdu->sdu.isUeCfgPres = FALSE;
  nhuDatReqEvntSdu->sdu.ueMsg.isMsgDed = TRUE;

  if( uehGetState(ueCb) != UEH_SRB2_ESTABLISHED)
  {
    ret = uehFillRRC_DlInfTr(nhuDatReqEvntSdu, nasPduLen, nasPdu, UEH_PDCP_ID_SRB1);
  }
  else   
  {
    ret = uehFillRRC_DlInfTr(nhuDatReqEvntSdu, nasPduLen, nasPdu, UEH_PDCP_ID_SRB2);
  }

  /* Send DL NAs message through NhuDatRsp Primitive */
  if(ROK != uehSndMsgOut (ENTNH, EVENTTYPE_DLINFTRANSFER, ueCb, TRANSID_NONE, nhuDatReqEvntSdu))
  {

    /* The data Couldn't be delivered to UE,
     * Inform the MME about this by sending NasNonDelivery Indication
     * Construct S1AP Message */
    UEH_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));

    uehFillNasNonDeliveryIndMsg( &nasNonDelInd, &ueCb->s1ConCb, nasPduLen, nasPdu );
    s1apWrapNasNonDeliveryIndPdu( &datEvt.pdu, &nasNonDelInd );
    datEvt.spConnId = ueCb->s1ConCb.spConnId;

    /* Send the message to S1ap Stack */
    uehSndMsgOut (ENTSZ, EVENTTYPE_DLNASNONDELIVERYIND, ueCb, TRANSID_NONE,&datEvt);
    logInfo("Sending NAS non delivery IND to SZ for UE:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id); 
    RETVALUE(RFAILED);
  }
  RETVALUE(ROK);
} /* uehSndRrcDlInfoTfr */

/*
 *
 *      Fun:   uehSndRrcConRel
 *
 *      Desc:  RRC Con Rel
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndRrcConRel(
    UehUeCb        *ueCb
  )
{
  NhuDatReqSdus *nhuDatReqEvntSdu = NULLP;

  logFunc("%s", "uehSndRrcConRel");

  /* Allocate memory for event structure,
   * Fill HDR Info,
   * Build RRCConnection Release Message */
  UEH_ALLOCEVNT_OR_RET(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
  uehFillNhuEvntHdr(&(nhuDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_RRCCONRLS);
  uehFillRRC_ConRls(nhuDatReqEvntSdu,ueCb);
  nhuDatReqEvntSdu->sdu.isUeCfgPres = FALSE;
  /* Send RRCConnectionRelease through NhuDatRsp Primitive*/
  if (RFAILED == uehSndMsgOut (ENTNH, EVENTTYPE_RRCCONNRELEASE, ueCb, TRANSID_NONE,(void *)nhuDatReqEvntSdu))
  {
   RETVALUE(RFAILED);
   }
  logInfo("RRC Connection Release sent for UE:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id); 
  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehSndRrcSecModeCmd
 *
 *      Desc:  RRC Security Mode Command
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndRrcSecModeCmd(
    UehUeCb    *ueCb
    )
{
  NhuUeCfgInfo  *ueCfg = NULLP;
  NhuDatReqSdus *nhuDatReqEvntSdu = NULLP;

  logFunc("%s", "uehSndRrcSecModeCmd");

  /* Allocate memory for event structure,
   * Fill HDR Info
   * Build Security Mode Message */
  UEH_ALLOCEVNT_OR_RET(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
  uehFillNhuEvntHdr( &(nhuDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_SECURITYMODECMD);
  uehFillRRC_SecModeCmd( nhuDatReqEvntSdu, ueCb);

  nhuDatReqEvntSdu->sdu.isUeCfgPres = TRUE;
  ueCfg = &(nhuDatReqEvntSdu->sdu.ueCfg);
  if(RFAILED == uehFillNhuUeCfgInfo(nhuDatReqEvntSdu, ueCfg, ueCb, C4_SECURITYMODECMD))
  {
    UEH_ERROR_MSG_FILLING_FAILED("RRC Security Mode Command", UE, ueCb->crnti);
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }
  /* Send the SecurityMode command to UE using NhuDatRsp Primitive*/
  if (RFAILED == uehSndMsgOut (ENTNH, EVENTTYPE_SECMODECOMMAND, ueCb, TRANSID_NONE, nhuDatReqEvntSdu))
  {
    RETVALUE(RFAILED);
  }
  logInfo("RRC Security Mode Command sent for UE:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  RETVALUE(ROK);
} /* uehSndRrcSecModeCmd */

/*
 *
 *      Fun:   uehLIMRrcConReEst
 *
 *      Desc:  RRC Connection Reestablishment
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndRrcConReEst(
    NhuDatIndSdus *datIndSdu
  )
{
  logFunc("%s", "uehSndRrcConReEst");

  logInfo ("Recived RRC Connection ReEstablishment Request from UeId:%x ", datIndSdu->hdr.ueId);

  /* Currently assuming that reconfiguration message was rejected by the
   * UE; hence triggering initial context setup failure */

  RETVALUE(ROK);
}

/* ==================================== */
/* RRC Uplink Message Handlers  */
/* ==================================== */

/*
 *
 *      Fun:   uehSaveRegisteredMmeIe
 *
 *      Desc:  Save RegisteredMme IE in ueCb->gummei context
 *
 *      Ret:   None
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
static void uehSaveRegisteredMmeIe(
  IO TBOOL              *plmnIdIeRcvd,
  IO UeGummei           *gummei,
  IO NhuRegisteredMME   *registeredMme
  )
{
  /* RegisteredMME
   * 1. PLMN-Identity - Optional
   * 2. mmegi         - Mandatory
   * 3. mmec          - Mandatory */

  /* IE - 1 - Optional  */
  if ( registeredMme->plmn_Identity.pres.pres )
  {
    *plmnIdIeRcvd = TRUE;
    /* MCC is of three digits */
    gummei->mcc[0] = (U8) registeredMme->plmn_Identity.mcc.member[0].val;
    gummei->mcc[1] = (U8) registeredMme->plmn_Identity.mcc.member[1].val;
    gummei->mcc[2] = (U8) registeredMme->plmn_Identity.mcc.member[2].val;
    /* MNC is of 2 or 3 digits */
    gummei->mnc[0] = (U8) registeredMme->plmn_Identity.mnc.member[0].val;
    gummei->mnc[1] = (U8) registeredMme->plmn_Identity.mnc.member[1].val;
    if(UEH_MNC_SIZE == registeredMme->plmn_Identity.mnc.noComp.val)
    {
      gummei->mnc[2] = (U8) registeredMme->plmn_Identity.mnc.member[2].val;
    }
  }
  else
  {
    /* This IE is not received that means the selected PLMN is same the previous one for the UE */
    *plmnIdIeRcvd = FALSE;
  }

  /* IE - 2 */
  UEH_MEM_COPY(&(gummei->mmeGrpId), &registeredMme->mmegi.val[0], registeredMme->mmegi.len/8); /* 16 bits*/
  /* IE - 3 */
  UEH_MEM_COPY(&(gummei->mmeId), &registeredMme->mmec.val[0], registeredMme->mmec.len/8); /* 8 bits size*/
} /* uehSaveRegisteredMmeIe */

/*
 *
 *      Fun:   uehSaveRrcConnSetupCompleteIEs
 *
 *      Desc:  Save RRCConnectionSetupComplete-r8-IEs IE in ueCb context
 *
 *      Ret:   None
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
static void uehSaveRrcConnSetupCompleteIEs(
  IO TBOOL              *includeGummeiIe,
  IO TBOOL              *plmnIdIeRcvd,
  IO UeGummei           *gummei,
  IO UehUeCb            *ueCb,
  IO InitialUeMessage   *initialUeMsg,
  IO NhuDatIndSdus      *datIndSdu
  )
{
  NhuRegisteredMME     *registeredMme = NULLP;
  /* RRCConnectionSetupComplete-r8-IEs
   * 1. selectedPLMN-Identity - Mandatory
   * 2. registeredMME         - Optional
   * 3. dedicatedInfoNAS      - Mandatory */

  /* IE 1. Save selectedPLMN-Identity (M) IE */
  ueCb->selectedPlmnInfo = datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.rrcConSetupCompl.criticalExtns.val.c1.val.rrcConSetupCompl_r8.selectedPLMN_Identity.val;

  /* IE 2. Save registeredMME (O) IE, if present */
  registeredMme = &(datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.rrcConSetupCompl.criticalExtns.val.c1.val.rrcConSetupCompl_r8.registeredMME);
  if ( registeredMme->pres.pres )
  {
    /* If registeredMme is received, then we need to send GUMMEI IE in Initial UE message to S1AP */
    *includeGummeiIe = TRUE;
    uehSaveRegisteredMmeIe( plmnIdIeRcvd, gummei, registeredMme );
  }
  else
  {
    /* If registeredMME is not received, then we need to omit sending of Gummei IE in Initial UE message to S1AP
     * Hence set a flag to remember */
    *includeGummeiIe = FALSE;
  }

  /* IE 3. Save dedicatedInfoNAS (M) IE */
  initialUeMsg->nasPdu.pduLen = getNasPduLenFromRrcConSetupRsp( datIndSdu );
  if (MAX_SIZE_NAS_PDU > initialUeMsg->nasPdu.pduLen) 
  {
      UEH_MEM_COPY( initialUeMsg->nasPdu.pdu,
          getNasPduFromRrcConSetupRsp(datIndSdu),
          initialUeMsg->nasPdu.pduLen );
  }
  else
  {
    UEH_ERROR_ELMNT_OUT_OF_RANGE("nasPduLen", initialUeMsg->nasPdu.pduLen, 0, MAX_SIZE_NAS_PDU);
    initialUeMsg->nasPdu.pduLen = 0;
  }
  
} /* uehSaveRrcConnSetupCompleteIEs */

/*
 *
 *      Fun:   uehLIMRrcConSetCmpHndlr
 *
 *      Desc:  RRC Connection setup Complete
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcRrcConSetCmp(
    UehUeCb         *ueCb,
    NhuDatIndSdus   *datIndSdu
  )
{
  U32                  s1ueIdx = 0;
  SztConReq            datEvt;
  InitialUeMessage     initialUeMsg;
  TBOOL                includeGummeiIe; /* TRUE = While sending Initial UE message, include GUMMIE optional IE, FALSE = Exclude this optional IE */
  TBOOL                plmnIdIeRcvd = FALSE;    /* TRUE = Received PLMN ID in RegisteredMME IE, FALSE = Not received PLMN ID in RegisteredMME IE */
  UeGummei             gummei;
  logFunc("%s", "uehProcRrcConSetCmp");

  logInfo("RRCConnSetupCmpl Received|UeId=%d|crnti=%d eNbs1apId:%d mmeS1apId:%d",datIndSdu->hdr.ueId, ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id); 
  /* ueId used in the NH till RRC Con Setup Complete comes */
  ueCb->ueId = NH_UEID_INVALID; 

  UEH_SET_ZERO(&datEvt, sizeof(SztConReq));
  
  /* Note: dedicatedInfoNAS IE received in RRCConnSetupCompl to be directly copied to initialUeMsg. */
  clearBytes( &initialUeMsg, sizeof(InitialUeMessage) );
  clearBytes( &gummei, sizeof(UeGummei) );
  uehSaveRrcConnSetupCompleteIEs( &includeGummeiIe, &plmnIdIeRcvd, &gummei, ueCb, &initialUeMsg, datIndSdu );
  
  s1ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
  ueCb->s1ConCb.suConnId           = s1ueIdx;
  /* ueCb->s1ConCb.enb_ue_s1ap_id is allocated by stack in ConRsp */
  ueCb->s1ConCb.mmeCb              = &uehCb.mmeCb;
  ueCb->s1ConCb.ueCb               = ueCb;
  /* Initialize control blocks */
  uehCb.s1UeConLst[s1ueIdx]        = ueCb;

  /* Build Initial UE Message */
  uehFillS1apInitMsg( &initialUeMsg, ueCb, includeGummeiIe, plmnIdIeRcvd, &gummei );
  s1apWrapInitialUeMsgPdu( &datEvt.pdu, &initialUeMsg );
  fillPeerId( datEvt, uehGetPeerIdForUeFromUeCb( ueCb ) );
  datEvt.u.suConnId = s1ueIdx;

  /* Trigger UE Connection Request to S1AP */
  logInfo("Initial UE Message|Tx| crnti=%d| eNbs1apId=%d| mmeS1apId=%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  if(ROK NOTEQ uehSndMsgOut (ENTSZ, EVENTTYPE_INITIALUEMSG, ueCb, TRANSID_NONE,&datEvt))
  {
    RETVALUE(RFAILED);
  }
  uehSetState(ueCb, UE_STATE_S1_CONNECTING);
  uehUeProcStart(ueCb,UEH_INITIAL_UE_MSG);
  RETVALUE(ROK);
} /* uehLIMRrcConSetCmpHndlr */

/*
 *
 *      Fun:   uehLIMRrcUlInfoTfrHndlr
 *
 *      Desc:  RRC UL Info transfer
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcUlNasTrans(
    UehUeCb          *ueCb,
    NhuDatIndSdus    *datIndSdu
  )
{
  SztDatEvntReq        datEvt;
  UlNasMessage         ulNasMsg;


  logInfo("uehProcUlNasTrans: UL Info Transfer Message Rcvd. from UeId:%d, eNBs1apId:%d mmeS1ApId:%d", datIndSdu->hdr.ueId, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.enb_ue_s1ap_id);
  /* Get the S1 Callblock for this UE */

  UEH_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));

  /* Construct S1AP Message */
  uehFillUlNasMsg( &ulNasMsg, &ueCb->s1ConCb, datIndSdu );
  s1apWrapUlNasMsgPdu( &datEvt.pdu, &ulNasMsg );
  datEvt.spConnId = ueCb->s1ConCb.spConnId;
  /* Send the message to S1ap Stack */
  logInfo("S1 UL NAS Message sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
 RETVALUE( uehSndMsgOut (ENTSZ, EVENTTYPE_ULNASTRANSPORTMSG, ueCb, TRANSID_NONE,&datEvt));

}

/*
 *
 *      Fun:   uehSndS1InitCntxtSetupRsp
 *
 *      Desc:  Send S1 Ue Context Setup Response 
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndS1InitCntxtSetupRsp(
    UehUeCb              *ueCb
  )
{
  InitialContextSetupRsp     initCtxSetupRspMsg;
  SztDatEvntReq              datEvt;

  logFunc("%s","uehSndS1InitCntxtSetupRsp");

  /* Construct S1AP Message */
  uehFillInitCtxSetupRsp(&initCtxSetupRspMsg, ueCb);
  s1apWrapInitialContextSetupRspPdu(&datEvt.pdu, &initCtxSetupRspMsg);
  datEvt.spConnId = ueCb->s1ConCb.spConnId;

  /* Trigger S1AP Primitive */
  logInfo("S1 Initial Context Setup Response sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  RETVALUE(uehSndMsgOut (ENTSZ, EVENTTYPE_INITCONTEXTSETUPRESP, ueCb, TRANSID_NONE,&datEvt));
}

/*
 *
 *      Fun:   uehSndS1UeCntxtRlsReq
 *
 *      Desc:  Send S1 Ue Context Release Request
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndS1UeCntxtRlsReq(
    UehUeCb              *ueCb,
    S1Cause              *cause
  )
{
  SztDatEvntReq         datEvt;
  UeContextRelReq       ueCtxRelReqMsg;

  logFunc("%s", "uehSndS1UeCntxtRlsReq");

  /* Get the S1 Callblock for this UE */

  UEH_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));

  /* Construct UeContextRelase S1AP Message*/

  uehFillUeCtxRelReq(&ueCtxRelReqMsg, &ueCb->s1ConCb, cause );
  s1apWrapUeContextRelReqPdu(&datEvt.pdu, &ueCtxRelReqMsg);
  datEvt.spConnId = ueCb->s1ConCb.spConnId;

  /* Send the message to S1ap Stack */

  logInfo("S1 UE Context Release sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  RETVALUE(uehSndMsgOut (ENTSZ, EVENTTYPE_UECONTEXTRELREQ, ueCb, TRANSID_NONE, &datEvt));

}


/*
 *
 *      Fun:   uehProcEgtpTunnelCfmInRecOngoing
 *
 *      Desc:  Process the Egtp Tunnel cfm
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcEgtpTunnelCfmInRecOngoing(
    UehUeCb        *ueCb,
    EgtUEvnt       *eguMsg
  )
{
  U8                      idx = 0;
  U8                      drbIdx = 0;
  U8                      countEgtpCfm = 0;

  logInfo("uehProcEgtpTunnelCfmInRecOngoing: EGTP Tunnel Cfm for Crnti:%d, eNbs1apId:%d mmeS1apId:%d, teId:%d Status:%d",ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id, eguMsg->u.tnlMgmt.teId, eguMsg->u.tnlMgmt.cfmStatus.status);

  if (LCM_PRIM_OK == eguMsg->u.tnlMgmt.cfmStatus.status)
  {
    UEH_UECB_RB_SET_TNL_STATUS(ueCb, eguMsg->u.tnlMgmt.teId);

    /*TODO: Allocation and Retention Priority (ARP) 
     * will be consided in next releases 
     */
    for(idx = 0; idx < ueCb->rbInfo.numOfDrbAdd; idx++)
    {
      drbIdx = ueCb->rbInfo.drbAddLstIndx[idx];

      if(TRUE == ueCb->rbInfo.drb[drbIdx].isEgtpTnlEnbl)
      {
        countEgtpCfm++;
      }
      else
      {
        break;
      }
    }

    if (countEgtpCfm == ueCb->rbInfo.numOfDrbAdd)
    {
      /* Configure the RGR for LCH and LCG for SRB2 and DRBs */
      uehUeProcStart(ueCb,UEH_INITIAL_CXT_SETUP);
      uehSndRgrCfgForRrcReCfg(ueCb);
    }
  }
  else
  {
    /* Deallocate the TeId */
    UEH_UECB_RB_DEALLOCATE_TNL(ueCb, eguMsg->u.tnlMgmt.teId); 
    /* Send Initial Context Setup Failure */
    uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_Transport, SztCauseTporttransport_resource_unavailableEnum);
  }

  RETVALUE(ROK);
}

PUBLIC S16 uehProcEgtpTunnelCfmInRelOngoing(
    UehUeCb        *ueCb,
    EgtUEvnt       *eguMsg
  )
{

  logInfo("EGTP Tunnel Cfm for Crnti:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  if (LCM_PRIM_OK != eguMsg->u.tnlMgmt.cfmStatus.status)
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTEGTULCLTNLMGMTCFM, ENTUDH, eguMsg->u.tnlMgmt.cfmStatus.status, UE, ueCb->crnti); 
    RETVALUE(ROK);
  }

  /* Presently only supported one DRB and thats why this condition is checked and if more RABS are supported the checking condition will be changed */

  if(TRUE == ueCb->rbInfo.drb[0].isUdhRabEnbl)
  {
    uehSndUdhDelUeReq(ueCb);
  }
  else
  {
    freeUeCb(ueCb);
  }
  
  /* This function is used when RABS need to be deleted. Presently RRM is supporting only one ERAB. ERAB Procedures are not supported in present rel*/ 
  //uehSndUdhDelRabReq(ueCb);

  RETVALUE(ROK);
}

/*
*
*      Fun:   uehSndRgrLcgCfg
*
*      Desc:  Send Rgr Logical group config
*
*      Ret:   ROK
*
*      Notes: None
*
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndRgrCfgForRrcReCfg(
    UehUeCb *ueCb
    )    
{
  U8            idx;
  U8            drbIdx;
#ifdef LTEMAC_MIMO

  U16           ueRecfgTypes = 0;
  RgrCfgReqInfo  *cfgInfo = NULLP;

  if(UEH_UECAP_RGRMSK_DONE != ueCb->ueEutraCap.rgrUeCatMask)
  {
    ueRecfgTypes |= RGR_UE_UECAT_RECFG;
    ueCb->ueEutraCap.rgrUeCatMask = UEH_UECAP_RGRMSK_DONE;
  }
//Srikanth:  ueRecfgTypes |= RGR_DL_LC_QOS_RECFG;
//ueRecfgTypes |= RGR_UE_QOS_RECFG;

  cfgInfo = uehCreateAndFillRgrUeReCfgReq( ueCb, ueRecfgTypes);
  if( NULL == cfgInfo)
  {
    RETVALUE(RFAILED);
  }
  uehSndRgrUeReCfgReq(ueCb,cfgInfo);
#endif

  /* Trigger RGR LCH Cfg for SRB2 */
  if (UEH_RB_RGRMSK_DONE != ueCb->rbInfo.srb[1].rgrCfgMask)
  {
    uehSndRgrLchCfg(ueCb, &ueCb->rbInfo.srb[1]);
  }
  /* Trigger RGR LCH Cfg for DRBs */
  for (idx = 0; idx < ueCb->rbInfo.numOfDrbAdd; idx++)
  {
    drbIdx = ueCb->rbInfo.drbAddLstIndx[idx];
    uehSndRgrLchCfg(ueCb, &ueCb->rbInfo.drb[drbIdx]);
  }
  logInfo("RgrLchCfg sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
   
  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehProcOpStopInUeCntd
 *
 *      Desc:  OP-Stop Request is received from OAM
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcOpStopInUeCntd(
    UehUeCb     *ueCb
  )
{

  logInfo("uehProcOpStopInUeCntd: Op-Stop Message Rcvd. In UE Connected State for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

/* We shouldn't send Too much signalling towards MME, Send Global Reset
 * As there is no point in waiting for ContextClearCommand from MME
 * So we commented the underlying code
  cause.causeType = CauseType_Misc;
  cause.causeVal = SztCauseMiscom_interventionEnum;
  RETVALUE (uehSndS1UeCntxtRlsReq( ueCb, &cause ));
*/

 /* Updating the Rab Release Cause */
  ueCb->rabReleaseCause = RABRELEASECAUSE_UECONTEXTRELEASED;
  SET_REL_CAUSE_IN_UECB(ueCb, CauseType_Misc, SztCauseMiscom_interventionEnum);
  uehStartRrcConnClear(ueCb);
  uehUeProcStart(ueCb,UEH_UE_CONN_REL_ONGOING);
  uehSetState(ueCb, UE_STATE_REL_ONGOING);

  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehProcEgtpErrIndInCallCntd 
 *
 *      Desc:  OP-Stop Request is received from OAM
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcEgtpErrIndInCallCntd(
    UehUeCb    *ueCb,
    EgtUEvnt   *sdu
  )
{
  S1Cause     cause;

  cause.causeType = sdu->u.errInd.errCause;
  cause.causeVal = SztCauseTportunspecifiedEnum;
  UEH_ERROR_RECEIVED_FAILURE_EVENT(  sdu->event, ENTEG, cause.causeType, UE, ueCb->crnti);

  uehSndS1UeCntxtRlsReq( ueCb, &cause );
  RETVALUE(ROK);

}

/*
 *
 *      Fun:   uehProcCallConnectedRrcErrInd
 *
 *      Desc:  Err Ind Recieved from NH
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcCallConnectedRrcErrInd(
    UehUeCb    *ueCb,
    NhuErrIndSdus     *errIndSdu
  )
{

  UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUERRIND, ENTNH,  errIndSdu->sdu.errCause, UE, ueCb->crnti);

  SET_REL_CAUSE_IN_UECB(ueCb, CauseType_RadioNw, SztCauseRadioNwfailure_in_radio_interface_procedureEnum);
  /* Send Initial Context Setup Failure */
  uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_RadioNw, SztCauseRadioNwradio_resources_not_availableEnum);

  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehProcRelOngoingRrcErrInd
 *
 *      Desc:  Err Ind Recieved from NH
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcRelOngoingRrcErrInd(
    UehUeCb           *ueCb,
    NhuErrIndSdus     *nhErrIndSdus
  )
{
  
  UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUERRIND, ENTNH, nhErrIndSdus->sdu.errCause, UE, ueCb->crnti);

  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehProcConnectedRrcErrInd
 *
 *      Desc:  Err Ind Recieved from NH
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcConnectedRrcErrInd(
    UehUeCb       *ueCb,
    NhuErrIndSdus *nhErrIndSdus
  )
{
  
  UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUERRIND, ENTNH, nhErrIndSdus->sdu.errCause, UE, ueCb->crnti);

  /* Send Initial Context Setup Failure */
  uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_RadioNw, SztCauseRadioNwradio_resources_not_availableEnum);

  RETVALUE(ROK);
}


/*
 *
 *      Fun:   uehProcSmcOngoingRrcErrInd
 *
 *      Desc:  Err Ind Recieved from NH
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcSmcOngoingRrcErrInd(
    UehUeCb       *ueCb,
    NhuErrIndSdus *nhErrIndSdus
  )
{
  UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUERRIND, ENTNH, nhErrIndSdus->sdu.errCause, UE, ueCb->crnti);

  /* Send Initial Context Setup Failure */
  uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_RadioNw, SztCauseRadioNwradio_resources_not_availableEnum);

  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehProcUeCapOngoingRrcErrInd
 *
 *      Desc:  Err Ind Recieved from NH
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcUeCapOngoingRrcErrInd(
    UehUeCb       *ueCb,
    NhuErrIndSdus *nhErrIndSdus
  )
{
  UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUERRIND, ENTNH, nhErrIndSdus->sdu.errCause, UE, ueCb->crnti);

  /* Send Initial Context Setup Failure */
  uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_Prototcol, SztCauseProtsemantic_errorEnum);

  RETVALUE (ROK);
}


/*
 *
 *      Fun:   uehProcRecOngoingRrcErrInd
 *
 *      Desc:  Err Ind Recieved from NH
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcRecOngoingRrcErrInd(
    UehUeCb         *ueCb,
    NhuErrIndSdus   *errIndSdu
  )
{

  UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUERRIND, ENTNH, errIndSdu->sdu.errCause, UE, ueCb->crnti);

  if (C4_RRCCONRECFGN == errIndSdu->hdr.transId)
  {
    /* Stop UE Response timer */
    uehStopTmr(UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
    /* Sends the GTP tunnel Del and 
     * Initial Context Setup Fail */
    uehSndEgtpDelInitCtxtSetupFail(ueCb, CauseType_RadioNw, SztCauseRadioNwradio_resources_not_availableEnum);
  }

  RETVALUE(ROK);
}

PUBLIC S16 uehProcS1LinkDownInUeCntd(
    UehUeCb      *ueCb
  )
{

  UEH_ERROR_RECEIVED_FAILURE_EVENT( EVT_S1M_LINK_DOWN_IND, ENTS1M, NOT_APPLICABLE, UE, ueCb->crnti);

  /* Updating the Rab Release Cause */
  ueCb->rabReleaseCause = RABRELEASECAUSE_UECONTEXTRELEASED;
  uehStartRrcConnClear( ueCb );
  uehUeProcStart(ueCb,UEH_UE_CONN_REL_ONGOING);

  uehSetState(ueCb, UE_STATE_REL_ONGOING);
  RETVALUE(ROK);
}

PUBLIC S16 uehProcOpStopOrS1LinkDown(
    UehUeCb     *ueCb
  )
{
  logError("Received Op-Stop/S1 Linkdown Message for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  uehSndRrcConRel(ueCb);
  uehSetState(ueCb, UE_STATE_REL_ONGOING);

  RETVALUE(ROK);
}


/*
 *
 *      Fun:   uehLIOpStopReqInS1ConnnectingHndlr
 *
 *      Desc:  OP-Stop Request is received from OAM
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcOpStopInS1Con(
    UehUeCb  *ueCb
  )
{
  return(uehProcOpStopOrS1LinkDown(ueCb));
}

PUBLIC S16 uehProcS1LinkDownInS1Con(
    UehUeCb  *ueCb
  )
{
  return(uehProcOpStopOrS1LinkDown(ueCb));
}

/*
 *
 *      Fun:   uehLIMRrcSecModCmpHndlr
 *
 *      Desc:  Security Mode complete
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcRrcSecModCmp(
    UehUeCb     *ueCb,
    void        *sdu
  )
{
  U16  ueIdx = 0;

  logInfo("<- SecModeComplete %s crnti=%d eNbs1apId:%d mmeS1apId:%d", __func__, ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id); 
  ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
  //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus =  uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus | SEC_PROC_SUCC;
  uehProcUeCapabilityInfo(ueCb);

  RETVALUE(ROK);
} /*uehLIMRrcSecModCmpHndlr */

/*
 *
 *      Fun:   uehLIMRrcSecModFailHndlr
 *
 *      Desc:  RRC Reconfiguration complete
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcRrcSecModFail(
    UehUeCb     *ueCb,
    void        *sdu
  )
{

  UEH_ERROR_RECEIVED_FAILURE_EVENT(EVTNHUDATIND, ENTNH, SztCauseRadioNwfailure_in_radio_interface_procedureEnum, UE, ueCb->crnti);
  /* SztCauseRadioNwencryption_and_or_integrity_protection_algorithms_not_supportedEnum */

  uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_RadioNw, SztCauseRadioNwfailure_in_radio_interface_procedureEnum);

  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehProcUeCapabilityInfo
 *
 *      Desc:  Ue Capability Information
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcUeCapabilityInfo(
    UehUeCb *ueCb
  )
{
  /* CS2: UE Capability Enquiry */
  if (TRUE != ueCb->ueRadCap.isUeRadAccCap)
  {
    /* UE Capability Enquiry from UE*/
    uehSndUeCapEnqiry(ueCb);
  }
  else
  {
    /* Send RRC to decode the Ue Capability */
    uehSndUeRadioAccCapInfoDecReq(ueCb);
  }
  uehSetState(ueCb, UE_STATE_UE_CAP_ONGOING);

  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehProcRrcReCfgCmp
 *
 *      Desc:  RRC Reconfiguration complete
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcRrcReCfgCmp(
    UehUeCb         *ueCb,
    NhuDatIndSdus   *datIndSdu
  )
{
  U16 ueIdx = 0;
  UehCellCb   *cellCb = uehGetCellCb();
  logInfo("RRC Recfg Complete Message Received for UeId:%d", datIndSdu->hdr.ueId);

  /* CS7: Add ERAB at UDH */
//  uehSndUdhAddRabReq(ueCb);

  ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
  //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus =  uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus | RRC_RE_CFG_PROC_SUCC;

  /* Initial Context Setup Response to MME */
  uehSndS1InitCntxtSetupRsp(ueCb);
  uehUeProcSuccess(ueCb, UEH_INITIAL_CXT_SETUP);
  /* update RRC State */
  uehSetState(ueCb, UE_STATE_CALL_CONNECTED);
  uehStartTmr (UE_PWR_RECONF_TMR,&((UehUeCb *)ueCb)->uePwrReconfTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
  logInfo("Timer started|UE Power Reset|ms=%d|ueId=%d", cellCb->uePwrReconfTmrDurinMs, datIndSdu->hdr.ueId);
  uehUeProcStart(ueCb, UEH_PUCCH_PWR_REC_ONGOING);

  RETVALUE(ROK);
} /* uehLIMRrcReconfigCmpHndlr */


/*
 *
 *      Fun:   uehProcRrcIdleRrcErrInd
 *
 *      Desc:  RRC Error Ind
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcRrcIdleRrcErrInd (
    UehUeCb         *ueCb,
    NhuErrIndSdus   *nhErrIndSdus
  )
{
  U8  transId = nhErrIndSdus->hdr.transId;

  UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUERRIND, ENTNH, nhErrIndSdus->sdu.errCause, UE, ueCb->crnti);

  if (C3_RRCCONREJ == transId)
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUERRIND, ENTNH, transId, UE, ueCb->crnti);
  }
  else
  {
    UEH_ERROR_MSG_RCVD_IN_UNEXPECTED_STATE( EVTNHUERRIND, ueCb->ueState);
  }

  RETVALUE(ROK);
} /* end of uehProcRrcIdleRrcErrInd */


/*
 *
 *      Fun:   uehProcRrcConRrcErrInd
 *
 *      Desc:  RRC Error Ind
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcRrcConRrcErrInd (
    UehUeCb         *ueCb,
    NhuErrIndSdus   *nhErrIndSdus
  )
{
  U8  transId = nhErrIndSdus->hdr.transId;

  UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUERRIND, ENTNH, nhErrIndSdus->sdu.errCause, UE, ueCb->crnti);

  if (C3_RRCCONREJ == transId)
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUERRIND, ENTNH, transId, UE, ueCb->crnti);
  }
  else if (C3_RRCCONSETUP == transId)
  {
    uehSndRrcConRej(ueCb);
  }
  else
  {
    UEH_ERROR_MSG_RCVD_IN_UNEXPECTED_STATE( EVTNHUERRIND, ueCb->ueState);
  }

  RETVALUE(ROK);
} /* end of uehProcRrcConRrcErrInd */


/*
 *
 *      Fun:   uehProcS1ConRrcErrInd
 *
 *      Desc:  RRC Error Ind
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcS1ConRrcErrInd (
    UehUeCb         *ueCb,
    NhuErrIndSdus   *nhErrIndSdus
  )
{
  U8                  transId = nhErrIndSdus->hdr.transId;

  UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUERRIND, ENTNH, nhErrIndSdus->sdu.errCause, UE, ueCb->crnti);

  if (C4_DLINFOTFR == transId)
  {
    UEH_ERROR_SSI_EVENT_SEND_FAILED( (&uehCb.nhuSap[0]->pst), EVENTTYPE_DLINFTRANSFER, UE, ueCb->crnti);
  }
  else
  {
    UEH_ERROR_MSG_RCVD_IN_UNEXPECTED_STATE( EVTNHUERRIND, ueCb->ueState);
  }

  RETVALUE(ROK);
} /* end of uehProcS1ConRrcErrInd */


/*
 *
 *      Fun:   uehSndCtfUeRls
 *
 *      Desc:  Send Ctf Ue Release
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
#ifndef RRC_PHY_CFG_SUPP
PUBLIC S16 uehSndCtfUeRls(
    UehUeCb        *ueCb
  )
{
  Pst           *pst = NULLP;
  CtfCfgReqInfo *cfgInfo;
  CtfCfgTransId transId;

  logFunc("%s", "uehSndCtfUeRls");

  pst = &uehCb.ctfSap[0]->pst;
  pst->event = EVTCTFCFGREQ;

  UEH_SET_ZERO(&transId, sizeof(CtfCfgTransId));
  UEH_SET_CTF_TRANS(transId.trans, CTF_UE_CFG, ueCb->crnti);

  UEH_ALLOC(&cfgInfo, sizeof(CtfCfgReqInfo));
  if(cfgInfo == NULLP)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(sizeof(CtfCfgReqInfo), "CtfCfgReqInfo");
    RETVALUE(RFAILED);
  }

  cfgInfo->cfgType = CTF_DELETE;
  cfgInfo->u.release.cfgElem = CTF_UE_CFG;
  cfgInfo->u.release.u.dedRel.cellId = ueCb->cellId;
  cfgInfo->u.release.u.dedRel.ueId = ueCb->crnti;

  RETVALUE( uehSndMsgOut (ENTTF, EVENTTYPE_PHYUEDELETE, ueCb,&transId, cfgInfo));

} /* uehSndCtfUeRls */
#endif



/*
*       Fun:   UehLiRgrCfgCfm - Cell Scheduler Configuration Confirm
*
*       Desc:  This function is used by Handle Cell Config Status Confirm
*              information from RGR.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li_hdl.c
*
*/
PUBLIC S16 uehProcCellRgrCfgCfm(
    U8 status
  )
{
  UehCellCb *cellCb = uehGetCellCb();

  if (RGR_CFG_CFM_NOK != status)
  {
    /* Send the Ctf Cell Configuration */
#ifndef RRC_PHY_CFG_SUPP
    if(ROK != uehSndCtfCellCfgReq(cellCb->cellId))
    {
      UEH_ERROR_SSI_EVENT_SEND_FAILED((&uehCb.ctfSap[0]->pst), EVENTTYPE_PHYCELLCONFIG, CELL, cellCb->cellId);
      RETVALUE(RFAILED);
    }
#endif
#ifdef SI_NEW
    if(ROK != uehSndRgrSiCfgReq(cellCb->cellId))
    {
      UEH_ERROR_SSI_EVENT_SEND_FAILED((&uehCb.rgrSap[0]->pst), EVENTTYPE_SCHSICONFIG, CELL, cellCb->cellId);
      RETVALUE(RFAILED);
    }
#endif
  }
  else
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTRGRCFGCFM, ENTRG, status, CELL, cellCb->cellId);
    RETVALUE(RFAILED);
  }

  RETVALUE(ROK);  
}

/*
*
*       Fun:   uehProcRgrUeCfgCfm - UE Scheduler Lch Lcg Configuration Confirm
*
*       Desc:  This function is used by Handle UE Config Status Confirm
*              information from RGR.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li_hdl.c
*
*/
PUBLIC S16 uehProcRgrUeCfgCfm (
    UehUeCb     *ueCb,
    UeRgrCfgCfm *ueRgrCfgCfm
  )
{
  U8 idx = 0;

  logFunc("%s", "uehProcRgrUeCfgCfm");

  //UECM_EVENTHDLR(ueCb->crnti, uehProcRgrCfgCfm, "-") ;
  logInfo("Received RGR UE CFG Confirm for UeId:%d eNbs1apId:%d mmeS1apId:%d with status:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id, ueRgrCfgCfm->status);

  if (ueRgrCfgCfm->cfgType != ueCb->ueRgrCfgState)
  {
    UEH_ERROR_MSG_RCVD_IN_UNEXPECTED_STATE( ueRgrCfgCfm->cfgType, ueCb->ueRgrCfgState);
    
    RETVALUE(RFAILED);
  }

  ueCb->ueRgrCfgState = RGR_UE_CFG_DONE;
  /* Trigger RGR LCH Cfg for SRBs */
  for (idx = 0; idx < ueCb->rbInfo.numOfSrbCfgd; idx++)
  {
    uehSndRgrLchCfg(ueCb, &ueCb->rbInfo.srb[idx]);
  }

  RETVALUE(ROK);
}


/*
*
*       Fun:   uehProcRgrUeReCfgCfm - UE Scheduler Lch Lcg Configuration Confirm
*
*       Desc:  This function is used by Handle UE Config Status Confirm
*              information from RGR.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li_hdl.c
*
*/
PUBLIC S16 uehProcRgrUeReCfgCfm (
    UehUeCb     *ueCb,
    UeRgrCfgCfm *ueRgrCfgCfm
  )
{
  U8 countRgrCfm = 0;

  logFunc("%s", "uehProcRgrUeReCfgCfm");

  //UECM_EVENTHDLR(ueCb->crnti, uehProcRgrCfgCfm, "-") ;
  logInfo("Received RGR UE RE-CFG Confirm for UeId:%d with status:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueRgrCfgCfm->status, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  if (ueRgrCfgCfm->cfgType != ueCb->ueRgrCfgState)
  {
    UEH_ERROR_MSG_RCVD_IN_UNEXPECTED_STATE( ueRgrCfgCfm->cfgType, ueCb->ueRgrCfgState);
    RETVALUE(RFAILED);
  }
  
  if (RGR_CFG_CFM_OK != ueRgrCfgCfm->status)
  {
     uehSndEgtpDelInitCtxtSetupFail(ueCb, CauseType_RadioNw, SztCauseRadioNwradio_resources_not_availableEnum);
     
  }
  else
  {
     ueCb->ueRgrCfgState = RGR_UE_CFG_DONE;

     if ((UEH_INITIAL_CXT_SETUP == ueCb->ueProcedure) && (CTF_CFG_CFM_OK == ueCb->isCtfConfigured))
     {
       GET_RGRCFGCFM_MASK(ueCb, countRgrCfm);

       if ((countRgrCfm == ueCb->rbInfo.numOfDrbAdd) && (RGR_UE_CFG_DONE == ueCb->ueRgrCfgState)) 
       {
         uehSndRrcRecfgReq(ueCb);
       }
     }
   }
  RETVALUE(ROK);
}


/*
*
*       Fun:   uehProcRgrLchLcgCfgCfm - UE Scheduler Lch Lcg Configuration Confirm
*
*       Desc:  This function is used by Handle LCH and LCG Config Status Confirm
*              information from RGR.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li_hdl.c
*
*/
PUBLIC S16 uehProcRgrLchLcgCfgCfm (
    UehUeCb     *ueCb,
    UeRgrCfgCfm *ueRgrCfgCfm
  )
{
  U8 countRgrCfm = 0;
  U8 rgrSubState = 0;

  logFunc("%s", "uehProcRgrLchLcgCfgCfm");

  //UECM_EVENTHDLR(ueCb->crnti, uehProcRgrCfgCfm, "-") ;
  logInfo("Received LCH LCG confirm from RGR(MAC) for UeId:%d eNbs1apId:%d mmeS1apId:%d LchId:%d status:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id, ueRgrCfgCfm->lchId, ueRgrCfgCfm->status);

  UEH_GET_RGR_SUBSTATE(ueCb, ueRgrCfgCfm->lchId, rgrSubState);

  if (ueRgrCfgCfm->cfgType != rgrSubState) 
  {
    UEH_ERROR_MSG_RCVD_IN_UNEXPECTED_STATE(ueRgrCfgCfm->cfgType, rgrSubState);
    RETVALUE(RFAILED);
  }

  switch(rgrSubState)
  {
    case RGR_LCH_CFG:
    {
      if((ueRgrCfgCfm->lchId == UEH_PDCP_ID_SRB1) || (ueRgrCfgCfm->lchId == UEH_PDCP_ID_SRB2))
      {
        UEH_SRB_RGRMSK(ueCb, ueRgrCfgCfm->lchId) |= UEH_RGR_RB_CFG_LCH;
        if(!(UEH_SRB_RGRMSK(ueCb, ueRgrCfgCfm->lchId) & UEH_RGR_RB_CFG_LCG))
        {
          /* Send RGR LCG Cfg Group 0*/
          uehSndRgrLcgCfg(ueCb, ueRgrCfgCfm->lchId, uehCb.uehCfgCb.qosCfg.logicalChannelGroup);// uehDfltSrbLogchcfgGrp);
        }
      }
      else
      {
        UEH_DRB_RGRMSK(ueCb, ueRgrCfgCfm->lchId) |= UEH_RGR_RB_CFG_LCH;
        if(!(UEH_DRB_RGRMSK(ueCb, ueRgrCfgCfm->lchId) & UEH_RGR_RB_CFG_LCG))
        {
          /* Send RGR LCG Cfg for Group 1 */
          uehSndRgrLcgCfg(ueCb, ueRgrCfgCfm->lchId, smCb.cfgCb.uehConfig.drbGrp);//uehDfltDrbGrp);
        }
      }
      break;
    }
       
    case RGR_LCG_CFG:
    {
      if((ueRgrCfgCfm->lchId == UEH_PDCP_ID_SRB1) || (ueRgrCfgCfm->lchId == UEH_PDCP_ID_SRB2))
      {
        UEH_SRB_RGRMSK(ueCb, ueRgrCfgCfm->lchId) |= UEH_RGR_RB_CFG_LCG;
      }
      else
      {
        UEH_DRB_RGRMSK(ueCb, ueRgrCfgCfm->lchId) |= UEH_RGR_RB_CFG_LCG;
      }

      /* Phy Configuration confirm should be checked */
      if (CTF_CFG_CFM_OK == ueCb->isCtfConfigured)
      {
        /* Send RRC Connection Setup after recieving the LCG Cfg Cfm */
        if ((UEH_RRC_CON_SETUP == ueCb->ueProcedure) && 
           (UEH_RB_RGRMSK_DONE == ueCb->rbInfo.srb[0].rgrCfgMask))
        {
          uehSndRrcConSetup ((void *)ueCb);
        }
        /* Send RRC Connection Reconfiguration after recieving the LCG Cfg Cfm */
        else if (UEH_INITIAL_CXT_SETUP == ueCb->ueProcedure)
        {
          GET_RGRCFGCFM_MASK(ueCb, countRgrCfm);

          if ((countRgrCfm == ueCb->rbInfo.numOfDrbAdd) && 
             (RGR_UE_CFG_DONE == ueCb->ueRgrCfgState))
          {
            /* CS7: Add ERAB at UDH */
            uehSndUdhAddRabReq(ueCb);
            ueCb->isCtfConfigured = UEH_VAL_ZERO;
            // uehSndRrcRecfgReq (ueCb);
          }
        }
      }
      break;
    }

    default:
    {
      UEH_ERROR_ELMNT_OUT_OF_RANGE("rgrSubState", rgrSubState, RGR_LCH_CFG, RGR_LCG_CFG);
      break;
    }
  }
   
  RETVALUE(ROK);
}


/*
*
*       Fun:   uehProcRrcConRgrCfgCfm - UE Scheduler Configuration Confirm
*
*       Desc:  This function is used by Handle UE Config Status Confirm
*              information from RGR.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li_hdl.c
*
*/

PUBLIC S16 uehProcRrcConRgrCfgCfm (
    UehUeCb     *ueCb,
    UeRgrCfgCfm *ueRgrCfgCfm
  )
{
  U8 rgrSubState = 0;

  UEH_GET_RGR_SUBSTATE(ueCb, ueRgrCfgCfm->lchId, rgrSubState);
  logInfo("uehProcRrcConRgrCfgCfm: UeId=%d Procedure=%d RgrCfgState=%d Status=%d", \
            ueCb->crnti, ueCb->ueProcedure, ueCb->ueRgrCfgState, ueRgrCfgCfm->status);


  /* TODO: CTF config confirmation handling needs to done */
  ueCb->isCtfConfigured = CTF_CFG_CFM_OK; // temporary
  if (RGR_CFG_CFM_OK != ueRgrCfgCfm->status)
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTRGRCFGCFM, ENTRG, ueRgrCfgCfm->status, UE, ueCb->crnti);
    if (UEH_RRC_CON_SETUP == ueCb->ueProcedure)
    {
      uehSndRrcConRej(ueCb);
    }

    RETVALUE(ROK);
  }

  if (RGR_UE_CFG == ueCb->ueRgrCfgState)
  {
    uehProcRgrUeCfgCfm(ueCb, ueRgrCfgCfm);
  }
  else
  {
    uehProcRgrLchLcgCfgCfm(ueCb, ueRgrCfgCfm);
  }

  RETVALUE(ROK);
} /* end of uehProcRrcConRgrCfgCfm */


/*
*
*       Fun:   uehProcRecOngoingRgrCfgCfm - UE Scheduler Configuration Confirm
*
*       Desc:  This function is used by Handle UE Config Status Confirm
*              information from RGR.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li_hdl.c
*
*/
PUBLIC S16 uehProcRecOngoingRgrCfgCfm (
    UehUeCb     *ueCb,
    UeRgrCfgCfm *ueRgrCfgCfm
  )
{
  U8 rgrSubState = RGR_UE_CFG; /* default to RGR_UE_CFG there is no _lchId val for this */

  logFunc("%s", "uehProcRecOngoingRgrCfgCfm");

  UEH_GET_RGR_SUBSTATE(ueCb, ueRgrCfgCfm->lchId, rgrSubState);
  logInfo("Received RRC Connection Reconfig in RGR Config confirm for for UeId:%d eNbs1apId:%d mmeS1apId:%d Procedure:%d RgrCfgType:%d, Status:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id, ueCb->ueProcedure, ueRgrCfgCfm->cfgType, ueRgrCfgCfm->status);

  /* TODO: CTF config confirmation handling needs to done */
  ueCb->isCtfConfigured = CTF_CFG_CFM_OK; // temporary

  if (RGR_CFG_CFM_OK != ueRgrCfgCfm->status)
  {
    if (UEH_INITIAL_CXT_SETUP == ueCb->ueProcedure)
    {
      /* Sends the GTP tunnel Del and sends
       * Initial Context Setup Fail */
      uehSndEgtpDelInitCtxtSetupFail(ueCb, CauseType_RadioNw, SztCauseRadioNwradio_resources_not_availableEnum);
    }

    RETVALUE(ROK);
  }

  if (RGR_UE_CFG == ueRgrCfgCfm->cfgType)
  {
    uehProcRgrUeReCfgCfm(ueCb, ueRgrCfgCfm);
  }
  else
  {
    uehProcRgrLchLcgCfgCfm(ueCb, ueRgrCfgCfm);
  }

  RETVALUE(ROK);
} /* end of uehProcRecOngoingRgrCfgCfm */
static CtfUeTxAntMode getAntInfoTxModeActualValue(
  IN AntInfoTxMode txMode
  )
{
  CtfUeTxAntMode ctfUeTxAntMode = 0 ;
  switch(txMode)
  {
    case ANTINFOTXMODE_TM1:  ctfUeTxAntMode = CTF_UE_TX_ANT_MODE_TM1; break;
    case ANTINFOTXMODE_TM2:  ctfUeTxAntMode = CTF_UE_TX_ANT_MODE_TM2; break;
    case ANTINFOTXMODE_TM3:  ctfUeTxAntMode = CTF_UE_TX_ANT_MODE_TM3; break;
    case ANTINFOTXMODE_TM4:  ctfUeTxAntMode = CTF_UE_TX_ANT_MODE_TM4; break;
    case ANTINFOTXMODE_TM5:  ctfUeTxAntMode = CTF_UE_TX_ANT_MODE_TM5; break;
    case ANTINFOTXMODE_TM6:  ctfUeTxAntMode = CTF_UE_TX_ANT_MODE_TM6; break;
    case ANTINFOTXMODE_TM7:  ctfUeTxAntMode = CTF_UE_TX_ANT_MODE_TM7; break;
  }
  return ctfUeTxAntMode;
}

/*     Fun:   uehProcRelOngoingRgrCfgCfm - UE Scheduler Configuration Confirm
*
*       Desc:  This function is used by Handle UE Config Status Confirm
*              information from RGR.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li_hdl.c
*
*/

PUBLIC S16 uehProcRelOngoingRgrCfgCfm (
    UehUeCb     *ueCb,
    UeRgrCfgCfm *ueRgrCfgCfm
  )
{
  U8 rgrSubState = 0;

  UEH_GET_RGR_SUBSTATE(ueCb, ueRgrCfgCfm->lchId, rgrSubState);
  logInfo("Received RGR Config confirm for RRC Connection Release for UeId:%d eNbs1apId:%d mmeS1apId:%d Procedure:%d Status:%d",ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id, ueCb->ueProcedure, ueRgrCfgCfm->status);

  if (RGR_CFG_CFM_OK != ueRgrCfgCfm->status)
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTRGRCFGCFM, ENTRG, ueRgrCfgCfm->status, UE, ueCb->crnti);
    /* TODO: Need to add the failure case handling here */
  }
  RETVALUE(ROK);
} /* end of uehProcRelOngoingRgrCfgCfm */

/*
 *
 *      Fun:   uehSndCtfUeCfg
 *
 *      Desc:  Send Ctf Ue config
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
#ifndef RRC_PHY_CFG_SUPP
PUBLIC S16 uehSndCtfUeCfg(
    UehUeCb        *ueCb
  )
{
  Pst *pst = NULLP;
  CtfCfgReqInfo *cfgInfo;
  CtfCfgTransId transId;

  CtfDedCfgInfo  *ueCfgParam = NULLP;

  logFunc("%s", "uehSndCtfUeCfg");

  pst = &uehCb.ctfSap[0]->pst;
  pst->event = EVTCTFCFGREQ;

  /* Initialize */
  UEH_SET_ZERO(&transId, sizeof(CtfCfgTransId));
  UEH_SET_CTF_TRANS(transId.trans, CTF_UE_CFG, ueCb->crnti);

  UEH_ALLOC(&cfgInfo, sizeof(CtfCfgReqInfo));
  if(cfgInfo == NULLP)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(sizeof(CtfCfgReqInfo),"CtfCfgReqInfo");
    RETVALUE(RFAILED);
  }


  cfgInfo->cfgType = CTF_CONFIG;
  cfgInfo->u.cfg.cfgElem = CTF_UE_CFG;

  ueCfgParam = &cfgInfo->u.cfg.u.dedCfg;
  ueCfgParam->cellId = ueCb->cellId;
  ueCfgParam->ueId = ueCb->crnti;

  /* Ue category */
  ueCfgParam->ueCatCfg.pres = TRUE;
  ueCfgParam->ueCatCfg.ueCategory = smCb.cfgCb.uehConfig.ueCategory; //uehDfltUECategory;

  /* PDSCH Cfg */
  ueCfgParam->pdschCfg.pres = TRUE;
  ueCfgParam->pdschCfg.pA = uehCb.uehCfgCb.phyCfg.cellPhyPa; //smCb.cfgCb.uehConfig.pdschPA; //uehDfltPdschPA;

  /* PUCCH Cfg */
  ueCfgParam->pucchCfg.pres = FALSE;
  ueCfgParam->pucchCfg.dedPucchCfgType = CTF_IE_CFG_SETUP;
  ueCfgParam->pucchCfg.pucchSetup.repFact = smCb.cfgCb.uehConfig.repFact; //uehDfltRepFact;
  ueCfgParam->pucchCfg.pucchSetup.n1PUCCHRep = smCb.cfgCb.uehConfig.n1PucchAnRep; //uehDfltN1PUCCHRep;

  /* PUSCH Cfg */
  ueCfgParam->puschCfg.pres = TRUE;
  ueCfgParam->puschCfg.betaOffsetAckIdx = uehCb.uehCfgCb.phyCfg.betaOffsetAckIndex; /* uehDfltBetaOffACKIdx */
  ueCfgParam->puschCfg.betaOffsetRiIdx = uehCb.uehCfgCb.phyCfg.betaOffsetRiIndex; /* uehDfltBetaOffRIIdx */
  ueCfgParam->puschCfg.betaOffsetCqiIdx = uehCb.uehCfgCb.phyCfg.betaOffsetCqiIndex; /* uehDfltBetaOffCQIIdx */

#ifndef TFU_UPGRADE
  /* CQI Reporting */
  ueCfgParam->cqiRptCfg.pres = FALSE; /*4GMX Sep13th : Changed from TRUE to FALSE */
  ueCfgParam->cqiRptCfg.reportingMode = CTF_CQI_RPTMODE_PRDIOC;

  /* APERIODIC is not supported for now */
  ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiPeriodicCfgType = CTF_IE_CFG_SETUP;
  /* def PUCCH_RESOURCE_MGMT  */
  ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPUCCHRsrcIndx = ueCb->acParams.cqipmiriInfo.cqiPucchRscIndex;
  ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPmiCfgIndx = ueCb->acParams.cqipmiriInfo.cqipmiConfigIndex;

  ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.formatIndicator = smCb.cfgCb.uehConfig.formatInd;//uehDfltFormatInd;
  /* Rel-1 supports wideband CQI */
  /* ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.subbandCqi.k = uehDfltSubBandK; */
  ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgPres = FALSE;
  ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.simultaneousAckNack = smCb.cfgCb.uehConfig.simultAckNackAndCQI;//uehDfltSimultAckNackAndCQI;

  /* UL SRS Cfg */
//  if( 0 == uehGetSrsPeriodicity() )
  {
    ueCfgParam->srsUlCfg.pres = NOTPRSNT;
  }
//  else
 /* {
    ueCfgParam->srsUlCfg.pres = PRSNT_NODEF;
    ueCfgParam->srsUlCfg.dedSrsUlCfgType = CTF_IE_CFG_SETUP;
    ueCfgParam->srsUlCfg.dedSrsSetup.srsBw = ueCb->ueSrsInfo.ueSrsBandwidth;
    ueCfgParam->srsUlCfg.dedSrsSetup.srsHopngBw = ueCb->ueSrsInfo.ueSrsHoppingBw;
    ueCfgParam->srsUlCfg.dedSrsSetup.freqDmnPos = ueCb->ueSrsInfo.ueFreqDomainPos;
    ueCfgParam->srsUlCfg.dedSrsSetup.duration = ueCb->ueSrsInfo.ueDuration;
    ueCfgParam->srsUlCfg.dedSrsSetup.srsCfgIdx = ueCb->ueSrsInfo.ueSrsCfgIndex;
    ueCfgParam->srsUlCfg.dedSrsSetup.txComb = ueCb->ueSrsInfo.ueTransmissionComb;
    ueCfgParam->srsUlCfg.dedSrsSetup.cyclicShift = ueCb->ueSrsInfo.ueDedCyclicShift;
  } */

  /* Dedicated SR Cfg */
  ueCfgParam->dedSRCfg.pres = TRUE;
  ueCfgParam->dedSRCfg.dedSRCfgType = CTF_IE_CFG_SETUP;
#if 1 /* def PUCCH_RESOURCE_MGMT  */
  ueCfgParam->dedSRCfg.dedSrSetup.srPUCCHRi = ueCb->acParams.srInfo.srPucchRscIndex;/* uehDfltSrPUCCHResIdx; */
  ueCfgParam->dedSRCfg.dedSrSetup.srCfgIdx = ueCb->acParams.srInfo.srConfigIndex;

  //ueCfgParam->dedSRCfg.dedSrSetup.srPUCCHRi = uehDfltSrPUCCHResIdx;
  //ueCfgParam->dedSRCfg.dedSrSetup.srCfgIdx = uehDfltSrConfigIdx;

#endif
#endif

  /* Antenna Cfg */
  ueCfgParam->antInfo.pres = TRUE;
  ueCfgParam->antInfo.txMode = getAntInfoTxModeActualValue(uehCb.uehCfgCb.cellCfg.antInfoTxMode);/*uehDfltAntInfoTxMode;*/
  ueCfgParam->antInfo.ueTxAntSelection.cfgType = CTF_IE_CFG_RELEASE; //CTF_IE_CFG_SETUP;
  ueCfgParam->antInfo.ueTxAntSelection.txAntSelect = smCb.cfgCb.uehConfig.antennaSel; //uehDfltAntennaSel;

  logInfo("Tx|CTF CFG Req|crnti=%d|enb_ue_s1apId=%d|mme_ue_s1ap_id=%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  RETVALUE( uehSndMsgOut (ENTTF, EVENTTYPE_PHYUECONFIG, NULLP, &transId, cfgInfo));
}
#endif


/*
 *
 *      Fun:   uehProcCtfUeCfgCfm
 *
 *      Desc:  Send Ctf Ue config/Re Config
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcCtfCfgCfm (
    UehUeCb     *ueCb,
    UeCtfCfgCfm *ueCtfCfgCfm
  )
{                            

  if(UEH_RRC_CON_SETUP == ueCb->ueProcedure)
  { 
    if(CTF_CFG_CFM_OK != ueCtfCfgCfm->status)
    {
      logError("Received the PHY configuration Failure for UEID : %d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
      /* Send RRC Connection Reject */
      uehSndRrcConRej(ueCb);
      RETVALUE(RFAILED);
    }

    ueCb->isCtfConfigured = CTF_CFG_CFM_OK;
    logInfo("CTF config Cfm received for UE ID %d eNbs1apId:%d mmeS1apId:%d", ueCtfCfgCfm->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id); 
    /* Send RRC Connection Setup after recieving the LCG Cfg Cfm */
    if (UEH_RB_RGRMSK_DONE == ueCb->rbInfo.srb[0].rgrCfgMask)
    {
      uehSndRrcConSetup ((void *)ueCb);
      ueCb->isCtfConfigured = UEH_VAL_ZERO;
    }
    RETVALUE(ROK);
  }
  RETVALUE(ROK);
}


/*
 *
 *      Fun:   uehSndRgrLcgCfg
 *
 *      Desc:  Send Rgr Logical group config
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndRgrLchCfg(
    UehUeCb        *ueCb,
    UehUeRAB       *rabInfo
  )
{
  Pst *pst = NULLP;
  RgrCfgReqInfo *cfgInfo = NULLP;
  RgrCfgTransId transId;
  RgrLchCfg *dedLchCfg = NULLP;

  logFunc("%s", "uehSndRgrLchCfg");

  pst = &uehCb.rgrSap[0]->pst;
  pst->event = EVTRGRCFGREQ;

  /* Initialize */
  UEH_ALLOC(&cfgInfo, sizeof(RgrCfgReqInfo));
  if(cfgInfo == NULLP)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(sizeof(RgrCfgReqInfo), "RgrCfgReqInfo");
    RETVALUE(RFAILED);
  }
  UEH_SET_ZERO(&transId, sizeof(RgrCfgTransId));
  cfgInfo->action = RGR_CONFIG;
  cfgInfo->u.cfgInfo.cfgType = RGR_LCH_CFG;
  rabInfo->rgrSubState = RGR_LCH_CFG;
  dedLchCfg = &(cfgInfo->u.cfgInfo.u.lchCfg);

  dedLchCfg->cellId = ueCb->cellId;
  dedLchCfg->crnti = ueCb->crnti;
  /* Logical channel ID  */
  dedLchCfg->lcId = rabInfo->rbId;

  if(rabInfo->rbType == CM_LTE_SRB)
  {
    dedLchCfg->lcType            = CM_LTE_LCH_DCCH;
    dedLchCfg->dlInfo.dlTrchType = CM_LTE_TRCH_DL_SCH;
  }
  else
  {
    dedLchCfg->lcType = CM_LTE_LCH_DTCH;
    /* Patch ve001.101 */      
    /* Downlink logical channel configuration info */      

    dedLchCfg->dlInfo.dlTrchType = CM_LTE_TRCH_DL_SCH;
    dedLchCfg->dlInfo.dlQos.qci  = rabInfo->qci;
    dedLchCfg->dlInfo.dlQos.gbr  = rabInfo->gbr.dl;      
    dedLchCfg->dlInfo.dlQos.mbr  = rabInfo->mbr.dl;
  }

  /* Fill appropriate TransId details */ 
  UEH_SET_RGR_TRANS(transId.trans, RGR_LCH_CFG, rabInfo->rbId, ueCb->crnti);

  /* Rgr Cfg Req primitive */
  logInfo("RGR LCH CFG request sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  RETVALUE( uehSndMsgOut (ENTRG, EVENTTYPE_SCHLCHCONFIG, ueCb,&transId, cfgInfo));

} /* uehSndRgrLchCfg */

/*
 *
 *      Fun:   uehSndRgrLcgCfg
 *
 *      Desc:  Send Rgr Logical group config
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndRgrLcgCfg(
    UehUeCb       *ueCb,
    U8            lcId,
    U8            grpId
  )
{
  Pst *pst = NULLP;
  RgrCfgReqInfo *cfgInfo;
  RgrCfgTransId transId;
  RgrLcgCfg *lcgCfg = NULLP;

  logFunc("%s", "uehSndRgrLcgCfg");

  pst = &uehCb.rgrSap[0]->pst;
  pst->event = EVTRGRCFGREQ;

  UEH_ALLOC(&cfgInfo, sizeof(RgrCfgReqInfo));
  if(cfgInfo == NULLP)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(sizeof(RgrCfgReqInfo), "RgrCfgReqInfo");
    RETVALUE(RFAILED);
  }

  UEH_SET_ZERO(&transId, sizeof(RgrCfgTransId));

  cfgInfo->action = RGR_CONFIG;
  cfgInfo->u.cfgInfo.cfgType = RGR_LCG_CFG;
  UEH_SET_RGR_SUBSTATE(ueCb, lcId, RGR_LCG_CFG)
  lcgCfg = &(cfgInfo->u.cfgInfo.u.lcgCfg);

  lcgCfg->cellId = ueCb->cellId;
  lcgCfg->crnti = ueCb->crnti;

  /* Uplink logical channel configuration information */
  lcgCfg->ulInfo.lcgId = grpId;

  /* Fill appropriate TransId details */
  UEH_SET_RGR_TRANS(transId.trans, RGR_LCG_CFG, lcId, ueCb->crnti);

  /* Rgr Cfg Req primitive */
  logInfo("RGR LCG CFG request sent for UeId:%d, eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  RETVALUE( uehSndMsgOut (ENTRG, EVENTTYPE_SCHLCGCONFIG, ueCb, &transId, cfgInfo));

} /* uehSndRgrLcgCfg */

/*
 *
 *      Fun:   uehSndRgrUeRls
 *
 *      Desc:  Send Rgr Ue Release 
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndRgrUeRls(
    UehUeCb        *ueCb
  )
{
  RgrCfgReqInfo *cfgInfo = NULLP;
  RgrCfgTransId transId;
  Pst *pst = NULLP;

  logFunc("%s", "uehSndRgrUeRls");

  pst = &uehCb.rgrSap[0]->pst;
  pst->event = EVTRGRCFGREQ;

  /* Initialize */
  UEH_SET_ZERO(&transId, sizeof(RgrCfgTransId));
  UEH_SET_RGR_TRANS(transId.trans, UEH_RGR_UE_DEL_TRANSID, UEH_VAL_ZERO, ueCb->crnti);

  UEH_ALLOC(&cfgInfo, sizeof(RgrCfgReqInfo));
  if(cfgInfo == NULLP)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(sizeof(RgrCfgReqInfo), "RgrCfgReqInfo");
    RETVALUE(RFAILED);
  }

  cfgInfo->action = RGR_DELETE;
  cfgInfo->u.delInfo.delType = RGR_UE_CFG;
  cfgInfo->u.delInfo.u.ueDel.cellId = ueCb->cellId;
  cfgInfo->u.delInfo.u.ueDel.crnti = ueCb->crnti;

  logInfo("RGR UE Delete request sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  RETVALUE(uehSndMsgOut (ENTRG, EVENTTYPE_SCHUEDELETE, ueCb,&transId, cfgInfo));
}

/*
 *
 *      Fun:   uehSndRgrUeRlsOnRrcReEstabFail
 *
 *      Desc:  Send Rgr Ue Release (used with RRC Connection ReEstablishment Reject)
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */

PUBLIC S16 uehSndRgrUeRlsOnRrcReEstabFail(
    U16         crnti
  )
{
  UehCellCb *cellCb = uehGetCellCb();
  RgrCfgReqInfo *cfgInfo = NULLP;
  RgrCfgTransId transId;
  Pst *pst = NULLP;

  logFunc("%s", "uehSndRgrUeRls");

  pst = &uehCb.rgrSap[0]->pst;
  pst->event = EVTRGRCFGREQ;

  /* Initialize */
  UEH_SET_ZERO(&transId, sizeof(RgrCfgTransId));
  UEH_SET_RGR_TRANS(transId.trans, UEH_RGR_UE_DEL_TRANSID, UEH_VAL_ZERO, crnti);

  UEH_ALLOC(&cfgInfo, sizeof(RgrCfgReqInfo));
  if(cfgInfo == NULLP)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(sizeof(RgrCfgReqInfo), "RgrCfgReqInfo");
    RETVALUE(RFAILED);
  }

  cfgInfo->action = RGR_DELETE;
  cfgInfo->u.delInfo.delType = RGR_UE_CFG;
  cfgInfo->u.delInfo.u.ueDel.cellId = cellCb->cellId;
  cfgInfo->u.delInfo.u.ueDel.crnti = crnti;

  logInfo("RGR UE Delete request sent for UeId:%d", crnti);
  RETVALUE(uehSndMsgOut (ENTRG, EVENTTYPE_SCHUEDELETE, NULLP,&transId, cfgInfo));
}
/*
 *
 *      Fun:   uehSndEgtpCom
 *
 *      Desc:  This function handles EGTP tunnel creation and destruction
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndEgtpCom(
    U8          egtpTunOpt,
    UehUeCb     *ueCb
  )
{
  Pst          *pst = NULLP;
  U8           idx;
  U8           drbIdx;
  EgtUEvnt    *eguEvtMsg = NULLP;

  logFunc("%s", "uehSndEgtpCom");

  pst = &uehCb.udhSap[0]->pst;


  /*Create/Delete EGTP tunnels as many eRabs configured*/
  if(EGTP_TUN_CREATE == egtpTunOpt)
  {
    logInfo("ADD ueCb->rbInfo.numOfDrbAdd= %d :: ueCb->rbInfo.numOfRbCfgd = %d ", ueCb->rbInfo.numOfDrbAdd ,ueCb->rbInfo.numOfRbCfgd);
    for(idx = 0; idx < ueCb->rbInfo.numOfDrbAdd; idx++)
    {
      drbIdx = ueCb->rbInfo.drbAddLstIndx[idx];
      ueCb->rbInfo.drb[drbIdx].locEgtpTunId = allocateTeid();

      if (-1 != ueCb->rbInfo.drb[drbIdx].locEgtpTunId)
      {
        /* Mapping between the UEID and LocTeId will be used at Hadling of Cfm msg from Egtp to UEH via UDH */
        uehCb.locTeIdToUeCb[ueCb->rbInfo.drb[drbIdx].locEgtpTunId] = ueCb;

        UEH_ALLOC((Data **)&eguEvtMsg, sizeof(EgtUEvnt));
        if (eguEvtMsg EQ NULLP)
        {
          UEH_ERROR_MEM_ALLOC_FAILED( sizeof(EgtUEvnt), "EGTP Event Message");
          RETVALUE(RFAILED);
        }

        eguEvtMsg->u.tnlMgmt.action      = EGT_TMGMT_REQTYPE_ADD;
        eguEvtMsg->u.tnlMgmt.intfType    = EGU_DFLT_INTFTYPE;
        eguEvtMsg->u.tnlMgmt.reOrderPres = FALSE;
        eguEvtMsg->u.tnlMgmt.teId        = ueCb->rbInfo.drb[drbIdx].locEgtpTunId;
        eguEvtMsg->u.tnlMgmt.reordAlgo   = EG_REORDER_TYPE_1;
        eguEvtMsg->u.tnlMgmt.expSeqNmb   = EGU_DFLT_EXPSEQNMB;
        uehCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.srcAddr),&(uehCb.datAppAddr)); /* srcIp */
        uehCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.dstIpAddr),&(ueCb->rbInfo.drb[drbIdx].sgwAddr));
        eguEvtMsg->u.tnlMgmt.maxTPduSize = EGU_DFLT_MAXTPDUSZ ;
        /* frmDstIpAddr is not needed at setup. Only needed when tunnel is modify */

        /* post message to ENTEU inorder to trigger tunnel creation */ 
        eguEvtMsg->event = EVTEGTULCLTNLMGMTREQ;
        logInfo("Sending EGTP Tunnel Add Request for UeId:%d eNbs1apId:%d mmeS1apId:%d, drbIdx:%d, locEgtpTunId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id, drbIdx , ueCb->rbInfo.drb[drbIdx].locEgtpTunId);
        UehLiEgtEguLclTnlMgmtReq(pst, 0, eguEvtMsg);
      }
      else
      {
        UEH_PROCEDURE_FAILED("EGTP Tunnel Id allocation", UE, ueCb->crnti, "No free TE ID");
        RETVALUE(RFAILED);
      }
    }
  }
  else if(EGTP_TUN_DEL == egtpTunOpt)/* EGTP_TUN_DEL */
  {
    logInfo("DEL ueCb->rbInfo.numOfDrbAdd= %d, ueCb->rbInfo.numOfRbCfgd = %d ", ueCb->rbInfo.numOfDrbAdd ,ueCb->rbInfo.numOfRbCfgd);
    if( 0 == ueCb->rbInfo.numOfRbCfgd )
    {
      freeUeCb(ueCb);
      RETVALUE(ROK);
    }
    for(drbIdx = 0; drbIdx < ueCb->rbInfo.numOfRbCfgd; drbIdx++)
    {
      if ((-1 != ueCb->rbInfo.drb[drbIdx].locEgtpTunId)
          && (TRUE == ueCb->rbInfo.drb[drbIdx].isEgtpTnlEnbl))
      {
    	UEH_ALLOC((Data **)&eguEvtMsg, sizeof(EgtUEvnt));
        if (eguEvtMsg == NULLP)
        {
          UEH_ERROR_MEM_ALLOC_FAILED( sizeof(EgtUEvnt), "EGTP Event Message");
          RETVALUE(RFAILED);
        }

        eguEvtMsg->u.tnlMgmt.action      = EGT_TMGMT_REQTYPE_DEL;
        eguEvtMsg->u.tnlMgmt.teId        = ueCb->rbInfo.drb[drbIdx].locEgtpTunId;
        eguEvtMsg->u.tnlMgmt.reordAlgo   = EG_REORDER_TYPE_1;
        uehCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.srcAddr),&(uehCb.datAppAddr)); /* srcIp */
        uehCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.dstIpAddr),&(ueCb->rbInfo.drb[drbIdx].sgwAddr));

        eguEvtMsg->event = EVTEGTULCLTNLMGMTREQ;

        logInfo("Sending EGTP Tunnel Delete Request for UeId:%d, eNbs1apId:%d mmeS1apId:%d, drbIdx:%d, eguEvtMsg->u.tnlMgmt.teId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id, drbIdx, eguEvtMsg->u.tnlMgmt.teId);
        UehLiEgtEguLclTnlMgmtReq(pst, 0, eguEvtMsg);

        /* Delete the Local tunnel Id */
        freeTeid(ueCb->rbInfo.drb[drbIdx].locEgtpTunId);
        ueCb->rbInfo.drb[drbIdx].locEgtpTunId = -1;
        ueCb->rbInfo.drb[drbIdx].isEgtpTnlEnbl = FALSE;
      }
      else
      {
        freeUeCb(ueCb);
      }
    }
  }

  RETVALUE(ROK);
}

#if 0
#ifdef ANSI
PUBLIC S16 uehSndEgtpComDed
(
U8 egtpCom,
UehS1ConCb *s1apConCb
)
#else
PUBLIC S16 uehSndEgtpComDed(egtpCom,s1apConCb)
U8 egtpCom;
UehS1ConCb *s1apConCb;
#endif
{
   EgtpTunInf tunelInf;
   Pst pst;
   U8 idx = 0;
   UehUeCb *ueCb = NULLP;

   logFunc("%s", "uehSndEgtpCom");

      ueCb = s1apConCb->ueCb;
   UEH_SET_ZERO(&pst, sizeof(Pst));

   /*Currently hard coding the pst structure*/
   pst.dstEnt = ENTUEH;
   pst.srcEnt = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_UEH_PROC;


#ifdef FT_INACTIVITY_TMR
   for (idx = 0; idx < UEH_MAX_RABS_IN_UE; idx++)
   {

      if((ueCb->rbInfo.drb[idx].inUse == TRUE) &&
            (ueCb->rbInfo.drb[idx].rbId == UEH_PDCP_ID_DRB2))
      {
         break;
      }
   }
   if(idx == UEH_MAX_RABS_IN_UE)
   {
      /* Raise Error */
      RETVALUE(RFAILED);
   }
#endif


   if(egtpCom == EGTP_TUN_CREATE)
   {
      UEH_SET_ZERO(&tunelInf, sizeof(EgtpTunInf));
      /* Tuning the local tunnel Id and PDCP Id for proper indexing of
         EGTP Tunnel List and PDCP Id List */
      tunelInf.remTeid = ueCb->rbInfo.drb[idx].remEgtpTunId;
      tunelInf.locTeid = ueCb->rbInfo.drb[idx].locEgtpTunId;
      tunelInf.pdcpId.cellId = ueCb->cellId;
      tunelInf.pdcpId.ueId = (CmLteRnti)ueCb->crnti;
      tunelInf.pdcpId.rbId = ueCb->rbInfo.drb[idx].pdcpId;
      tunelInf.pdcpId.rbType = CM_LTE_DRB;
      /* memcpy the serv. GW */
      cmMemcpy((U8 *)&tunelInf.dstIpAddr, (U8 *)&ueCb->rbInfo.drb[idx].sgwAddr, sizeof(CmTptAddr));
      /* post message to ENTEU inorder to trigger tunnel creation */
      cmPkEuCreattLocTunReq(&pst, 0, &tunelInf);
   }
   else /* EGTP_TUN_DEL */
   {
      /* Trigger Deletion */
      cmPkEuDelLocTunReq(&pst, 0, ueCb->rbInfo.drb[idx].locEgtpTunId);
   }

   RETVALUE(ROK);
}

#endif

/*
 *
 *      Fun:   uehFillRgrPuschDedCfg
 *
 *      Desc:  Populate puschDedCfg Structure
 *
 *      Ret:   void
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrPuschDedCfg(
    RgrUePuschDedCfg *puschDedCfg
  )
{
  logFunc("%s", "uehFillRgrPuschDedCfg");
  puschDedCfg->pres = TRUE;
  puschDedCfg->bCQIIdx = uehCb.uehCfgCb.phyCfg.betaOffsetCqiIndex;
  puschDedCfg->bACKIdx = uehCb.uehCfgCb.phyCfg.betaOffsetAckIndex;
  puschDedCfg->bRIIdx = uehCb.uehCfgCb.phyCfg.betaOffsetRiIndex;

} /* uehFillRgrUeTxModeCfg */
/*
 *
 *      Fun:   uehFillRgrUeTxModeCfg
 *
 *      Desc:  Populate RgrUeTxModeCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrUeTxModeCfg(
    RgrUeTxModeCfg *ueCfg
  )
{
  logFunc("%s", "uehFillRgrUeTxModeCfg");

  ueCfg->pres       = TRUE;
  /* Adding one to the actual value to map with the RGR interface*/
  ueCfg->txModeEnum = (getAntInfoTxModeActualValue(uehCb.uehCfgCb.cellCfg.antInfoTxMode)) + 1;
#ifdef LTEMAC_MIMO
  ueCfg->tmTrnstnState = RGR_TXMODE_RECFG_START;
#endif

} /* uehFillRgrUeTxModeCfg */

/*
 *
 *      Fun:   uehFillRgrUeDlCqiCfg
 *
 *      Desc:  Populate RgrUeDlCqiCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrUeDlCqiCfg(
    RgrUeDlCqiCfg       *ueCfg,
    UehUeCb             *ueCb,
    RgrTxMode           txMode      
  )
{
  logFunc("%s", "uehFillRgrUeDlCqiCfg");

  ueCfg->aprdCqiCfg.pres         = FALSE;
  ueCfg->aprdCqiCfg.aprdModeEnum = smCb.cfgCb.uehConfig.aPeriodicMode;
  /*TFU_UPGRADE*/
   if (1 == smCb.cfgCb.uehConfig.isDlLaEnabled)
   {
    ueCfg->prdCqiCfg.type                  = RGR_SCH_PCQI_SETUP;
    ueCfg->prdCqiCfg.cqiSetup.cqiPResIdx   = ueCb->acParams.cqipmiriInfo.cqiPucchRscIndex;
    ueCfg->prdCqiCfg.cqiSetup.cqiPCfgIdx   = ueCb->acParams.cqipmiriInfo.cqipmiConfigIndex;
    ueCfg->prdCqiCfg.cqiSetup.cqiRepType   = 1; //Wideband CQI
    ueCfg->prdCqiCfg.cqiSetup.k            = smCb.cfgCb.uehConfig.cqiCfgk;
    ueCfg->prdCqiCfg.cqiSetup.sANCQI       = smCb.cfgCb.uehConfig.simultAckNackAndCQI;
    ueCfg->prdCqiCfg.cqiSetup.prdModeEnum  = smCb.cfgCb.uehConfig.periodicMode;
    if ((2 == smCb.cfgCb.uehConfig.numOfTxAnt) && (RGR_UE_TM_2 != txMode))
    {
      ueCfg->prdCqiCfg.cqiSetup.riEna        = 1;
      ueCfg->prdCqiCfg.cqiSetup.riCfgIdx     = ueCb->acParams.cqipmiriInfo.riConfigIndex;
    }
    else
    {
      ueCfg->prdCqiCfg.cqiSetup.riEna        = 0;
      ueCfg->prdCqiCfg.cqiSetup.riCfgIdx     = 0;
    }
  }
  else
  {
    ueCfg->prdCqiCfg.type                  = RGR_SCH_PCQI_REL;
  }
} /* uehFillRgrUeDlCqiCfg */





RgrUeDsrTransMax rgrUeSrCfgMapping(
     EnumNhuSchedulingRqstConfigsetupdsr_TransMax dsrTransMax)
{

switch (dsrTransMax)
  {
  case NhuSchedulingRqstConfigsetupdsr_TransMaxn4Enum:
 	return RGR_DSR_TXMAX_4;
  case NhuSchedulingRqstConfigsetupdsr_TransMaxn16Enum:
        return RGR_DSR_TXMAX_16;
  case NhuSchedulingRqstConfigsetupdsr_TransMaxn32Enum:
        return RGR_DSR_TXMAX_32;
  case NhuSchedulingRqstConfigsetupdsr_TransMaxn64Enum:
        return RGR_DSR_TXMAX_64;
  case NhuSchedulingRqstConfigsetupdsr_TransMaxn8Enum:
  case NhuSchedulingRqstConfigsetupdsr_TransMaxspare3Enum:
  case NhuSchedulingRqstConfigsetupdsr_TransMaxspare2Enum:
  case NhuSchedulingRqstConfigsetupdsr_TransMaxspare1Enum:
  default:
	//UEH_ERROR_ELMNT_OUT_OF_RANGE("dsrTransMax",dsrTransMax,);
	UEH_ERROR_VALUE_NOT_SUPPORTED("dsrTransMax",dsrTransMax);
        return RGR_DSR_TXMAX_4;
  }

}




/*
 *
 *      Fun:   uehFillRgrUeUlHqCfg
 *
 *      Desc:  Populate RgrUeUlHqCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrUeSrCfg(
    RgrUeSrCfg *ueCfg,
    UehUeCb *ueCb
    )
{
  ueCfg->type = RGR_SCH_PCQI_SETUP;
  ueCfg->srSetup.srResIdx = ueCb->acParams.srInfo.srPucchRscIndex;
  ueCfg->srSetup.srCfgIdx =  ueCb->acParams.srInfo.srConfigIndex;

  ueCfg->srSetup.dTMax    = rgrUeSrCfgMapping(smCb.cfgCb.uehConfig.dsrTransMax);
}
/*
 *
 *      Fun:   uehFillRgrUeUlHqCfg
 *
 *      Desc:  Populate RgrUeUlHqCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrUeUlHqCfg(
    RgrUeUlHqCfg *ueCfg
  )
{
  logFunc("%s", "uehFillRgrUeUlHqCfg");

  ueCfg->maxUlHqTx = uehGetMaxUlHarqTx[uehCb.uehCfgCb.schCfg.maxUlHarqTx];
  ueCfg->deltaHqOffst = uehDfltDeltaHqOffst;//smCb.cfgCb.uehConfig.maxUlHqTx; //uehDfltDeltaHqOffst;
} /* uehFillRgrUeUlHqCfg */


/*
 *
 *      Fun:   uehFillRgrUeUlPwrCfg
 *
 *      Desc:  Populate RgrUeUlPwrCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrUeUlPwrCfg(
    RgrUeUlPwrCfg *ueCfg
  )
{
  logFunc("%s", "uehFillRgrUeUlPwrCfg");

  ueCfg->uePuschPwr.pres   = FALSE;
  ueCfg->uePucchPwr.pres   = FALSE;
  ueCfg->isAccumulated     = smCb.cfgCb.uehConfig.accumulationEnabled;//uehDfltAccumulationEnabled;
  ueCfg->isDeltaMCSEnabled = smCb.cfgCb.uehConfig.deltaMcsEnabled;//uehDfltDeltaMCSEnabled;
  /*ueCfg->p0UePusch       = uehCb.uehCfgCb.sib2Cfg.poNominalPusch; */
  ueCfg->p0UePusch         = uehCb.uehCfgCb.phyCfg.poUePusch;
  ueCfg->p0UePucch         = uehCb.uehCfgCb.phyCfg.poUePucch;
  ueCfg->pSRSOffset        = uehCb.uehCfgCb.phyCfg.pSrsOffset;
  ueCfg->trgCqi            = smCb.cfgCb.uehConfig.ueUlPwrTrgCqi; //uehDfltUeUlPwrTrgCqi;
  logInfo("UE cfg UL CQI: %d", ueCfg->trgCqi);
} /* uehFillRgrUeUlPwrCfg */


/*
 *
 *      Fun:   uehFillRgrUeQosCfg
 *
 *      Desc:  Populate RgrUeQosCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrUeQosCfg(
    RgrUeQosCfg *ueCfg
  )
{
  logFunc("%s", "uehFillRgrUeQosCfg");

  ueCfg->ambrPres = FALSE;
  ueCfg->dlAmbr   = UEH_VAL_ZERO;
  ueCfg->ueBr     = smCb.cfgCb.uehConfig.ueUlByteRate; //uehDfltUeBr;
} /* uehFillRgrUeQosCfg */



/*
 *
 *      Fun:   uehConvertTimeAlignmentTimerEnumToVal
 *
 *      Desc:  This function is used to convert Time Alignment timer values from ENUM to actual values
 *
 *      Ret:   UINT16
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */

UINT16 uehConvertTimeAlignmentTimerEnumToVal(TimeAlignmentTimer  timeAlignmentTimerEnum)
{
  UINT16 timeAlignmentTimer = 0xFFFF ; /* 320 == RETXBSRTIMER_SF320 */
  switch(timeAlignmentTimerEnum)
  {
    case TIMEALIGNMENTTIMER_SF500:    timeAlignmentTimer = 500; break;
    case TIMEALIGNMENTTIMER_SF750:    timeAlignmentTimer = 750; break;
    case TIMEALIGNMENTTIMER_SF1280:   timeAlignmentTimer = 1280; break;
    case TIMEALIGNMENTTIMER_SF1920:   timeAlignmentTimer = 1920; break;
    case TIMEALIGNMENTTIMER_SF2560:   timeAlignmentTimer = 2560; break;
    case TIMEALIGNMENTTIMER_SF5120:   timeAlignmentTimer = 5120; break;
    case TIMEALIGNMENTTIMER_SF10240:  timeAlignmentTimer = 10240; break;
    case TIMEALIGNMENTTIMER_INFINITY: timeAlignmentTimer = 0xFFFF; break;
    default:
      UEH_ERROR_ELMNT_OUT_OF_RANGE("timeAlignmentTimerEnum", timeAlignmentTimerEnum, TIMEALIGNMENTTIMER_SF500, TIMEALIGNMENTTIMER_INFINITY);
    break;
  }
  return timeAlignmentTimer;
}
/*
 *
 *      Fun:   uehFillRgrUeTaTmrCfg
 *
 *      Desc:  Populate RgrUeTaTmrCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrUeTaTmrCfg(
    RgrUeTaTmrCfg *ueCfg
  )
{
  logFunc("%s", "uehFillRgrUeTaTmrCfg");

  ueCfg->pres = FALSE; /*for testing purpose HER7*/
  ueCfg->taTmr = uehConvertTimeAlignmentTimerEnumToVal(uehCb.uehCfgCb.sib2Cfg.timeAlignmentTimer);/* uehDfltTaTmr */  /* value does not matter */
} /* uehFillRgrUeTaTmrCfg */


/*
 *
 *      Fun:   uehConvertPeriodicBsrEnumToVal
 *
 *      Desc:  This function is used to convert periodic BSR timer values from ENUM to actual values
 *
 *      Ret:   UINT16
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
UINT16 uehConvertPeriodicBsrEnumToVal(PeriodicBsrTimer  periodicBsrTimerEnum)
{
  UINT16 periodicBsrTimer = 0xFFFF ; /* 0xFFFF == Infinity */
  switch(periodicBsrTimerEnum)
  {
    case PERIODICBSRTIMER_SF5:     periodicBsrTimer = 5; break;
    case PERIODICBSRTIMER_SF10:    periodicBsrTimer = 10; break;
    case PERIODICBSRTIMER_SF16:    periodicBsrTimer = 16; break;
    case PERIODICBSRTIMER_SF20:    periodicBsrTimer = 20; break;
    case PERIODICBSRTIMER_SF32:    periodicBsrTimer = 32; break;
    case PERIODICBSRTIMER_SF40:    periodicBsrTimer = 40; break;
    case PERIODICBSRTIMER_SF64:    periodicBsrTimer = 64; break;
    case PERIODICBSRTIMER_SF80:    periodicBsrTimer = 80; break;
    case PERIODICBSRTIMER_SF128:   periodicBsrTimer = 128; break;
    case PERIODICBSRTIMER_SF160:   periodicBsrTimer = 160; break;
    case PERIODICBSRTIMER_SF320:   periodicBsrTimer = 320; break;
    case PERIODICBSRTIMER_SF640:   periodicBsrTimer = 640; break;
    case PERIODICBSRTIMER_SF1280:  periodicBsrTimer = 1280; break;
    case PERIODICBSRTIMER_SF2560:  periodicBsrTimer = 2560; break;
    case PERIODICBSRTIMER_INFINITY:periodicBsrTimer = 0xFFFF; break;
    default:
      UEH_ERROR_ELMNT_OUT_OF_RANGE("periodicBsrTimerEnum", periodicBsrTimerEnum, PERIODICBSRTIMER_SF5, PERIODICBSRTIMER_INFINITY);
    break;
  }
  return periodicBsrTimer;
}

/*
 *
 *      Fun:   uehConvertRetxBsrTimerEnumToVal
 *
 *      Desc:  This function is used to convert Retransmit BSR timer values from ENUM to actual values
 *
 *      Ret:   UINT16
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
UINT16 uehConvertRetxBsrTimerEnumToVal(RetxBsrTimer  retxBsrTimerEnum)
{
  UINT16 retxBsrTimer = 320 ; /* 320 == RETXBSRTIMER_SF320 */
  switch(retxBsrTimerEnum)
  {
    case RETXBSRTIMER_SF320:   retxBsrTimer = 320; break;
    case RETXBSRTIMER_SF640:   retxBsrTimer = 640; break;
    case RETXBSRTIMER_SF1280:  retxBsrTimer = 1280; break;
    case RETXBSRTIMER_SF2560:  retxBsrTimer = 2560; break;
    case RETXBSRTIMER_SF5120:  retxBsrTimer = 5120; break;
    case RETXBSRTIMER_SF10240: retxBsrTimer = 10240; break;
    default:
      UEH_ERROR_ELMNT_OUT_OF_RANGE("retxBsrTimerEnum", retxBsrTimerEnum, RETXBSRTIMER_SF320, RETXBSRTIMER_SF10240);
    break;
  }
  return retxBsrTimer;
}

#ifdef RGR_V1
/*
 *
 *      Fun:   uehFillRgrUeBsrTmrCfg
 *
 *      Desc:  Populate RgrUeBsrTmrCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrUeBsrTmrCfg(
    RgrUeBsrTmrCfg     *ueCfg
  )
{
  logFunc("%s", "uehFillRgrUeBsrTmrCfg");

  ueCfg->isPrdBsrTmrPres = TRUE;

  ueCfg->prdBsrTmr  = uehConvertPeriodicBsrEnumToVal(uehCb.uehCfgCb.schCfg.periodicBsrTimer);
  ueCfg->retxBsrTmr = uehConvertRetxBsrTimerEnumToVal(uehCb.uehCfgCb.schCfg.retxBsrTimer);
} /* uehFillRgrUeBsrTmrCfg */
#endif


#ifdef RGR_CQI_REPT

/*
 *
 *      Fun:   uehFillRgrUeNumCqiRept
 *
 *      Desc:  Populate RgrUeCqiReptCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void  uehFillRgrUeNumCqiRept(RgrUeCqiReptCfg *ueCfg)
{
  logFunc("%s", "uehFillRgrUeNumCqiRept");

  ueCfg->numColltdCqiRept  = 0;
} /* uehFillRgrUeNumCqiRept */
#endif


/*
 *
 *      Fun:   uehFillRgrUeDrxCfg
 *
 *      Desc:  Populate RgrUeDrxCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrUeDrxCfg(
    RgrUeDrxCfg *ueCfg
  )
{
  logFunc("%s", "uehFillRgrUeDrxCfg");

  ueCfg->isDrxEnabled = FALSE;
  ueCfg->drxOnDurTmr = UEH_VAL_ZERO;
  ueCfg->drxInactvTmr = UEH_VAL_ZERO;
  ueCfg->drxRetxTmr = UEH_VAL_ZERO;
  ueCfg->drxLongCycleOffst.longDrxCycle = UEH_VAL_ZERO;
  ueCfg->drxLongCycleOffst.drxStartOffst = UEH_VAL_ZERO;
  ueCfg->drxShortDrx.pres = UEH_VAL_ZERO;
  ueCfg->drxShortDrx.shortDrxCycle = UEH_VAL_ZERO;
  ueCfg->drxShortDrx.drxShortCycleTmr = UEH_VAL_ZERO;

} /* uehFillRgrUeDrxCfg */


/*
 *
 *      Fun:   uehFillRgrUeAckNackRepCfg
 *
 *      Desc:  Populate RgrUeAckNackRepCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrUeAckNackRepCfg(
    RgrUeAckNackRepCfg *ueCfg
  )
{
  logFunc("%s", "uehFillRgrUeAckNackRepCfg");

  ueCfg->isAckNackEnabled = FALSE;
  ueCfg->pucchAckNackRep = UEH_VAL_ZERO;
  ueCfg->ackNackRepFactor = smCb.cfgCb.uehConfig.ackNackRepFactor; //uehDfltAckNackRepFactor;
} /* uehFillRgrUeAckNackRepCfg */

/*
 *
 *      Fun:   uehFillRgrUeMeasGapCfg
 *
 *      Desc:  Populate RgrUeMeasGapCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrUeMeasGapCfg(
    RgrUeMeasGapCfg *ueCfg
  )
{
  logFunc("%s", "uehFillRgrUeMeasGapCfg");

  ueCfg->isMesGapEnabled = FALSE;
  ueCfg->gapPrd = 40;
  ueCfg->gapOffst = 20;
} /* uehFillRgrUeMeasGapCfg */

/*
 *
 *      Fun:   uehFillRgrUeCapCfg
 *
 *      Desc:  Populate RgrUeCapCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrUeCapCfg(
    RgrUeCapCfg *ueCfg
  )
{
  logFunc("%s", "uehFillRgrUeCapCfg");

  ueCfg->pwrClass = 23;
  ueCfg->intraSfFeqHop = FALSE;
  ueCfg->resAloocType1 = FALSE;
  ueCfg->simCqiAckNack = FALSE;
  ueCfg->txAntSel      = FALSE;
} /* uehFillRgrUeCapCfg */

/*
 *
 *      Fun:   uehFillRgrCodeBookRstCfg
 *
 *      Desc:  Populate RgrCodeBookRstCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrCodeBookRstCfg(
    RgrCodeBookRstCfg *ueCfg
  )
{
  logFunc("%s", "uehFillRgrCodeBookRstCfg");

  ueCfg->pres = 0;
  ueCfg->pmiBitMap[0] = 1;
  ueCfg->pmiBitMap[1] = 1;

}


#ifdef LTE_TDD
/*
 *
 *      Fun:   uehFillRgrTddAckNackMode
 *
 *      Desc:  Populate RgrTddAckNackMode Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehFillRgrTddAckNackMode(
    RgrTddAckNackMode *mode
  )
{
  logFunc("%s", "uehFillRgrTddAckNackMode");

  *mode = RGR_TDD_ACKNACK_MODE_BUNDL;
}
#endif /* LTE_TDD */

/*
 *
 *      Fun:   uehSndRgrUeCfgReq
 *
 *      Desc:  Send Rgr UE Cfg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndRgrUeCfgReq(
    UehUeCb        *ueCb,
    RgrCfgReqInfo *cfgInfo
  )
{
  Pst *pst = NULLP;
  RgrCfgTransId transId;

  logFunc("%s", "uehSndRgrUeCfgReq");

  pst = &uehCb.rgrSap[0]->pst;
  pst->event = EVTRGRCFGREQ;

  UEH_SET_ZERO(&transId, sizeof(RgrCfgTransId));

  /* Fill appropriate Trans details */
  UEH_SET_RGR_TRANS(transId.trans, RGR_UE_CFG, UEH_VAL_ZERO, ueCb->crnti);

  
  logInfo("RGR UE CFG request sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  if(ROK != uehSndMsgOut (ENTRG, EVENTTYPE_SCHUECONFIG, NULLP, &transId, cfgInfo))
  {
    RETVALUE(RFAILED);
  }

  RETVALUE(ROK);
} /* uehSndRgrUeCfgReq */

RgrCfgReqInfo     *uehCreateAndFillRgrUeReCfgReq(
    UehUeCb        *ueCb,
    U16            ueRecfgTypes
    )
{
  RgrCfgReqInfo     *cfgInfo = NULLP;

  UEH_ALLOC(&cfgInfo, sizeof(RgrCfgReqInfo));
  if(NULLP == cfgInfo)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(sizeof(RgrCfgReqInfo), "RgrCfgReqInfo");
    return NULL;
  }
  UEH_SET_ZERO(cfgInfo, sizeof(RgrCfgReqInfo));

  cfgInfo->u.recfgInfo.u.ueRecfg.ueRecfgTypes = ueRecfgTypes;

  UehPrepRgrReCfgReq(ueCb, cfgInfo);

  return cfgInfo;
}

/*
 *
 *      Fun:   uehSndRgrUeReCfgReq
 *
 *      Desc:  Send Rgr UE Cfg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndRgrUeReCfgReq (
    UehUeCb           *ueCb,
    RgrCfgReqInfo     *cfgInfo
  )
{
  Pst               *pst = NULLP;
  RgrCfgTransId     transId;

  logFunc("%s", "uehSndRgrUeReCfgReq");

  UEH_SET_ZERO(&transId, sizeof(RgrCfgTransId));
  UEH_SET_RGR_TRANS(transId.trans, RGR_UE_CFG, UEH_VAL_ZERO, ueCb->crnti);

  pst = &uehCb.rgrSap[0]->pst;
  pst->event = EVTRGRCFGREQ;

  logInfo("RGR UE Re-CFG request sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  if(ROK != uehSndMsgOut (ENTRG, EVENTTYPE_SCHUEDELETE, ueCb,&transId, cfgInfo))
  {
    RETVALUE(RFAILED);
  }

  RETVALUE(ROK);
} /* uehSndRgrUeReCfgReq */


/**************************************************************
  Following functions handles the S1AP Events received at UEH
***************************************************************/
/**
 * @details This function is invoked when DlNasTransport is received from MME in S1_CONNECTING state.
 * Save the enbId Sent by S1AP stack, mmeId sent buy MME
 * Send the message to UE
 **/
PUBLIC S16 uehProcDlNasInS1Con(
    UehUeCb       *ueCb,
    S1apPdu       *pdu
  )
{
  DlNasMessage  dlNasMsg;
  UehS1ConCb    *s1ConCb = &ueCb->s1ConCb;

  logInfo("S1 DLNASTRPT RX crnti=%d eNbs1apId=%d mmeS1apId=%d", 
            ueCb->crnti, 
            ueCb->s1ConCb.enb_ue_s1ap_id, 
            ueCb->s1ConCb.mme_ue_s1ap_id);

  /* Decode the received Pdu */
  s1apUnwrapDlNasPdu( pdu, &dlNasMsg );

  /* Save the MmeUeS1apId,
   * EnbUeS1apId, CCPU expects the place holder for this in InitialUeMessage, The value filled by RRM is
   * ignored and a local reference is filled by S1AP Stack. Save this after Connection confirmation
   * Stop the SRB1 timer*/
  if (0 EQ s1ConCb->mme_ue_s1ap_id) s1ConCb->mme_ue_s1ap_id = dlNasMsg.mmeUeS1apId;
  if (0 EQ s1ConCb->enb_ue_s1ap_id) s1ConCb->enb_ue_s1ap_id = dlNasMsg.enbUeS1apId;

  if (RFAILED == uehValidateEnbAndMmeUeS1apIds(s1ConCb->mme_ue_s1ap_id,s1ConCb->enb_ue_s1ap_id,ueCb->crnti))
  {
    RETVALUE(RFAILED);
  }
  /* TODO Code to be added to handle Handover */

  /* Fill RRC DL Information Transfer*/
  if(ROK !=uehSndRrcDlInfoTfr( ueCb, dlNasMsg.nasPdu.pduLen, dlNasMsg.nasPdu.pdu ) )
  {
    RETVALUE(RFAILED);
  }
  uehSetState(ueCb, UE_STATE_CONNECTED);

  RETVALUE(ROK);
}

/**
 * @details This function is invoked when Location report request message is received from MME.
 **/
PUBLIC S16 uehProcLocReport(
    UehUeCb     *ueCb,
    S1apPdu     *pdu
  )
{
  LocReqReportCntrl  locReqReportCntrl;
  LocReqReportFail   locReqReportFail;
  LocReqReport       locReqReport;
  S1Cause            cause;
  SztDatEvntReq      datEvt;

  logInfo("uehProcLocReport: S1 Location Reporting Control Message Received for UeId:%d, eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  /* Decode the received Pdu */
  s1apUnwrapLocReportCntrlPdu( pdu, &locReqReportCntrl );

  if (RFAILED == uehValidateEnbAndMmeUeS1apIds(locReqReportCntrl.mmeUeS1apId,locReqReportCntrl.enbUeS1apId,ueCb->crnti))
  {
    RETVALUE(RFAILED);
  }
  /* TODO in Rel 1.0 we support only Direct, For other cases we send error */
  switch( locReqReportCntrl.locReportCntrlReqType.locReportTypeEvent )
  {
    case LOCREQTYPE_EVENT_DIRECT:
    {
      uehFillUeLocReportSucc( &locReqReportCntrl, &locReqReport );
      s1apWrapLocReportPdu( &datEvt.pdu, &locReqReport);
      datEvt.spConnId = ueCb->s1ConCb.spConnId;
      if(ROK != uehSndMsgOut (ENTSZ, EVENTTYPE_LOCATIONREPORTRES, ueCb, TRANSID_NONE,&datEvt))
      {
        RETVALUE(RFAILED);
      }
      break;
    }
    case LOCREQTYPE_EVENT_CHANGE_OF_SERVICE_CELL:
    {
      /* TODO Check the number of configured cells */
      cause.causeType = CauseType_Misc;
      cause.causeVal  = SztCauseMiscunspecifiedEnum;
      uehFillUeLocReportFailInd( &locReqReportCntrl, &locReqReportFail, &cause );
      s1apWrapLocReportFailIndPdu( &datEvt.pdu, &locReqReportFail);

      /* Send the message to S1ap Stack */
      datEvt.spConnId = ueCb->s1ConCb.spConnId;
      if(ROK != uehSndMsgOut (ENTSZ, EVENTTYPE_LOCATIONREPORTFAILUREIND , ueCb, TRANSID_NONE,&datEvt))
      {
        RETVALUE(RFAILED);
      }
      break;
    }
    case LOCREQTYPE_EVENT_STOP_CHANGE_OF_SERVICE_CELL:
    {
      cause.causeType = CauseType_Prototcol;
      cause.causeVal  = SztCauseProtmessage_not_compatible_with_receiver_stateEnum;
      uehFillUeLocReportFailInd( &locReqReportCntrl, &locReqReportFail, &cause );
      s1apWrapLocReportFailIndPdu( &datEvt.pdu, &locReqReportFail);

      datEvt.spConnId = ueCb->s1ConCb.spConnId;
      if(ROK != uehSndMsgOut (ENTSZ, EVENTTYPE_LOCATIONREPORTFAILUREIND , ueCb, TRANSID_NONE,&datEvt))
      {
        RETVALUE(RFAILED);
       }
      break;
    }
    default:
    {
      UEH_ERROR_ELMNT_OUT_OF_RANGE( "Location Report Type Event" , locReqReportCntrl.locReportCntrlReqType.locReportTypeEvent, LOCREQTYPE_EVENT_DIRECT, LOCREQTYPE_EVENT_STOP_CHANGE_OF_SERVICE_CELL);
      RETVALUE(RFAILED);
      break;
    }
  }

  logInfo("S1 Location Reporting Response Message Sent for UeId:%d, ueCb->crnti, eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  RETVALUE(ROK);
}

void uehHandleTraceStart(
    UehUeCb         *ueCb,
    TraceActivation *traceActivation
  )
{

  TraceFailInd          traceFailInd;
  S1Cause               cause;
  SztDatEvntReq         datEvt;
  
  /* TODO in Rel 1.0 we reject Trace Start */
  cause.causeType = CauseType_Misc;
  cause.causeVal  = SztCauseMiscunspecifiedEnum;
  uehFillTraceFailInd( &ueCb->s1ConCb, &(traceActivation->eUtranTraceId), &traceFailInd, &cause );
  s1apWrapTraceFailIndPdu( &datEvt.pdu, &traceFailInd);

  /* Send the message to S1ap Stack */
  datEvt.spConnId = ueCb->s1ConCb.spConnId;
  uehSndMsgOut (ENTSZ, EVENTTYPE_TRACEFAILUREIND, ueCb, TRANSID_NONE,&datEvt);
  logInfo("S1 Trace Fail Indication sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
}

/**
 * @details This function is invoked when Trace Start message is received from MME.
 **/
PUBLIC S16 uehProcTraceStart(
    UehUeCb     *ueCb,
    S1apPdu     *pdu
  )
{
  TraceStart            traceStart;

  logInfo("uehProcTraceStart: S1 Start Trace Message received for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  /* Decode the received Pdu */
  s1apUnwrapTraceStartPdu( pdu, &traceStart );

  if (RFAILED == uehValidateEnbAndMmeUeS1apIds(traceStart.mmeUeS1apId,traceStart.enbUeS1apId,ueCb->crnti))
  {
    RETVALUE(RFAILED);
  }
  uehHandleTraceStart(  ueCb, &(traceStart.traceActivation) );

  RETVALUE(ROK);
}

/**
 * @details This function is invoked when Deactivate Trace message is received from MME.
 **/
PUBLIC S16 uehProcDeactivateTrace(
    UehUeCb     *ueCb,
    S1apPdu     *pdu
  )
{
  DeactivateTrace       deactivateTrace;
  TraceFailInd          traceFailInd;
  S1Cause               cause;
  SztDatEvntReq         datEvt;

  logInfo("uehProcDeactivateTrace: S1 Deactivate Trace Message received for UeId:%d, eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  /* Decode the received Pdu */
  s1apUnwrapDeactivateTracePdu( pdu, &deactivateTrace );
  if (RFAILED == uehValidateEnbAndMmeUeS1apIds(deactivateTrace.mmeUeS1apId,deactivateTrace.enbUeS1apId,ueCb->crnti))
  {
    RETVALUE(RFAILED);
  }

  /* TODO in Rel 1.0 we reject Trace Start, So De activate Trace Don't Arise */
  cause.causeType = CauseType_Misc;
  cause.causeVal  = SztCauseMiscunspecifiedEnum;
  uehFillTraceFailInd( &ueCb->s1ConCb, &(deactivateTrace.eUtranTraceId), &traceFailInd, &cause );
  s1apWrapTraceFailIndPdu( &datEvt.pdu, &traceFailInd);

  /* Send the message to S1ap Stack */
  datEvt.spConnId = ueCb->s1ConCb.spConnId;
  logInfo("S1 Trace Fail Indication sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  RETVALUE(uehSndMsgOut (ENTSZ, EVENTTYPE_DEACTIVATETRACEFAILUREIND, ueCb, TRANSID_NONE,&datEvt));
}

/**
 * @details This function is invoked when ERab Setup Request message is received from MME.
 **/
PUBLIC S16 uehProcERabSetupReq(
    UehUeCb       *ueCb,
    S1apPdu       *pdu
  )
{
  ERabSetupReq      eRabSetupReq;
  ERabSetupRsp      eRabSetupRsp;
  S1Cause           cause;
  SztDatEvntReq     datEvt;
  TBOOL             sendRgrReConfig = FALSE;

  logInfo("uehProcERabSetupReq: S1 ERab Setup Request Message received for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  /* Decode the received Pdu */
  s1apUnwrapERabSetupReqPdu( pdu, &eRabSetupReq );

  if (RFAILED == uehValidateEnbAndMmeUeS1apIds(eRabSetupReq.mmeUeS1apId,eRabSetupReq.enbUeS1apId,ueCb->crnti))
  {
    RETVALUE(RFAILED);
  }
  /* TODO in Rel 1.0 we reject ERabSetup Request */
  cause.causeType = CauseType_Misc;
  cause.causeVal  = SztCauseMiscunspecifiedEnum;
  uehFillERabSetupRsp( &(eRabSetupReq), &eRabSetupRsp, &cause );
  s1apWrapERabSetupRspPdu( &datEvt.pdu, &eRabSetupRsp);

  /* Send the message to S1ap Stack */
  datEvt.spConnId = ueCb->s1ConCb.spConnId;
  logInfo("S1 ERab Setup Request Message sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  if(ROK != uehSndMsgOut (ENTSZ, EVENTTYPE_RABSETUPREQ, ueCb, TRANSID_NONE,&datEvt))
  {
   RETVALUE(RFAILED);
  }

  /* Check UE Aggregate Max Bit Rate */
  if( TRUE == eRabSetupReq.isUeAggMaxBitRate)
  {
/*    ueCb->aggMaxBr.dl = getUint32FromArray( eRabSetupReq.ueAggMaxBitRate.ueAggMaxBitRateDl.bitRate );
    ueCb->aggMaxBr.ul = getUint32FromArray( eRabSetupReq.ueAggMaxBitRate.ueAggMaxBitRateUl.bitRate );
  */  sendRgrReConfig = FALSE;
  }

  if( TRUE == sendRgrReConfig )
  {
    uehSndRgrCfgForRrcReCfg (ueCb);
  }

  RETVALUE(ROK);
}

/**
 * @details This function is invoked when ERab Modify Request message is received from MME.
 **/
PUBLIC S16 uehProcERabModifyReq(
    UehUeCb     *ueCb,
    S1apPdu     *pdu
  )
{
  TBOOL                   sendRgrReConfig = FALSE;
  ERabModifyReq           eRabModifyReq;
  ERabModifyRsp           eRabModifyRsp;
  S1Cause                 cause;
  SztDatEvntReq           datEvt;

  logInfo("uehProcERabModifyReq: S1 ERab Modify Request Message received for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  /* Decode the received Pdu */
  s1apUnwrapERabModifyReqPdu( pdu, &eRabModifyReq );
  if (RFAILED == uehValidateEnbAndMmeUeS1apIds(eRabModifyReq.mmeUeS1apId,eRabModifyReq.enbUeS1apId,ueCb->crnti))
  {
    RETVALUE(RFAILED);
  }

  /* TODO in Rel 1.0 we reject ERabModiy Request, until QOS support is enabled */
  cause.causeType = CauseType_Misc;
  cause.causeVal  = SztCauseMiscunspecifiedEnum;

  uehFillERabModifyRsp( &(eRabModifyReq), &eRabModifyRsp, &cause, ueCb );
  s1apWrapERabModifyRspPdu( &datEvt.pdu, &eRabModifyRsp);

  /* Send the message to S1ap Stack */
  datEvt.spConnId = ueCb->s1ConCb.spConnId;
  logInfo("S1 ERab Modify Response Message sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
 if(ROK !=  uehSndMsgOut (ENTSZ, EVENTTYPE_RABMODFAIL, ueCb, TRANSID_NONE,&datEvt))
 {
   RETVALUE(RFAILED);
 }

  /* Check UE Aggregate Max Bit Rate */
  if( TRUE == eRabModifyReq.isUeAggMaxBitRate)
  {
    ueCb->aggMaxBr.dl = getUint32FromArray( eRabModifyReq.ueAggMaxBitRate.ueAggMaxBitRateDl.bitRate );
    ueCb->aggMaxBr.ul = getUint32FromArray( eRabModifyReq.ueAggMaxBitRate.ueAggMaxBitRateUl.bitRate );
    sendRgrReConfig = TRUE;
  }

  if( TRUE == sendRgrReConfig )
  {
    uehSndRgrCfgForRrcReCfg (ueCb);
  }

  RETVALUE(ROK);
}

/**
 * @details This function is invoked when ERab Release Request message is received from MME.
 **/
PUBLIC S16 uehProcERabReleaseReq(
    UehUeCb     *ueCb,
    S1apPdu     *pdu
  )
{
  TBOOL             sendRgrReConfig = FALSE;
  ERabReleaseReq    eRabReleaseReq;
  ERabReleaseRsp    eRabReleaseRsp;
  S1Cause           cause;
  SztDatEvntReq     datEvt;

  logInfo("uehProcERabReleaseReq: S1 ERab Release Request Message received for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  /* Decode the received Pdu */
  s1apUnwrapERabReleaseCmdPdu( pdu, &eRabReleaseReq );
  if (RFAILED == uehValidateEnbAndMmeUeS1apIds(eRabReleaseReq.mmeUeS1apId,eRabReleaseReq.enbUeS1apId,ueCb->crnti))
  {
    RETVALUE(RFAILED);
  }

  /* TODO in Rel 1.0 we reject ERabRelease Request, until QOS support is enabled */
  cause.causeType = CauseType_Misc;
  cause.causeVal  = SztCauseMiscunspecifiedEnum;
  uehFillERabReleaseRsp( &(eRabReleaseReq), &eRabReleaseRsp, &cause, ueCb );
  s1apWrapERabReleaseRspPdu( &datEvt.pdu, &eRabReleaseRsp);

  /* Send the message to S1ap Stack */
  datEvt.spConnId = ueCb->s1ConCb.spConnId;
  logInfo("S1 ERab Release Response Message Sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  if(ROK !=  uehSndMsgOut (ENTSZ,EVENTTYPE_RABRELCOMPL, ueCb, TRANSID_NONE,&datEvt))
  {
    RETVALUE(RFAILED);
   }

  /* Check UE Aggregate Max Bit Rate */
  if( TRUE == eRabReleaseReq.isUeAggMaxBitRate)
  {
    ueCb->aggMaxBr.dl = getUint32FromArray( eRabReleaseReq.ueAggMaxBitRate.ueAggMaxBitRateDl.bitRate );
    ueCb->aggMaxBr.ul = getUint32FromArray( eRabReleaseReq.ueAggMaxBitRate.ueAggMaxBitRateUl.bitRate );
    sendRgrReConfig = TRUE;
  }

  if( TRUE == sendRgrReConfig )
  {
    uehSndRgrCfgForRrcReCfg (ueCb);
  }

  RETVALUE(ROK);
}

/**
 * @details This function is invoked when UE Context Modification Request message is received from MME.
 **/
PUBLIC S16 uehProcUeCtxModReq(
    UehUeCb     *ueCb,
    S1apPdu     *pdu
  )
{
  TBOOL                    sendRgrReConfig = FALSE;
  UeContextModReq          ueCtxModReq;
  UeContextModRsp          ueContextModRsp;
  SztDatEvntReq            datEvt;

  logInfo("uehProcUeCtxModReq: S1 UE Context Modification Request Message received for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  
  /* Decode the received PDU */
  s1apUnwrapUeContextModReqPdu( pdu, &ueCtxModReq );
  if (RFAILED == uehValidateEnbAndMmeUeS1apIds(ueCtxModReq.mmeUeS1apId,ueCtxModReq.enbUeS1apId,ueCb->crnti))
  {
    RETVALUE(RFAILED);
  }

  /* Fetch the security related values */
  if( TRUE == ueCtxModReq.isSecurityKey )
  {
    //uehExtEncIntKeysFromKeNB( &ueCb->secCfg, &ueCtxModReq.securityKey );
  }

  /* Check UE Aggregate Max Bit Rate */
  if( TRUE == ueCtxModReq.isUeAggMaxBitRate)
  {
    ueCb->aggMaxBr.dl = getUint32FromArray( ueCtxModReq.ueAggMaxBitRate.ueAggMaxBitRateDl.bitRate );
    ueCb->aggMaxBr.ul = getUint32FromArray( ueCtxModReq.ueAggMaxBitRate.ueAggMaxBitRateUl.bitRate );
    sendRgrReConfig = TRUE;
  }

  if( TRUE == sendRgrReConfig )
  {
    uehSndRgrCfgForRrcReCfg (ueCb);
  }

  uehFillUeCtxModRsp( &(ueContextModRsp), &ueCb->s1ConCb );
  s1apWrapUeContextModRspPdu( &datEvt.pdu, &ueContextModRsp);

  UECM_EVENTHDLR( ueCb->crnti, uehProcUeCtxModReq,"Sending UE Context Modification Response.") ;

  /* Send the message to S1ap Stack */
  datEvt.spConnId = ueCb->s1ConCb.spConnId;
  logInfo("S1 UE Context Modification Response Sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
 RETVALUE( uehSndMsgOut (ENTSZ, EVENTTYPE_UECONTEXTMODRESP, ueCb, TRANSID_NONE,&datEvt));

}

/**
 * @details This function is invoked when DlNasMessage from MME is received
 * Send this message to Ue
 **/
PUBLIC S16 uehProcDlNasMsg(
    UehUeCb     *ueCb,
    S1apPdu     *pdu
  )
{
  DlNasMessage  dlNasMsg;

  logInfo("uehProcDlNasMsg: S1 DL Nas Message received for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  /* Decode the received Pdu */
  s1apUnwrapDlNasPdu( pdu, &dlNasMsg );
  if (RFAILED == uehValidateEnbAndMmeUeS1apIds(dlNasMsg.mmeUeS1apId,dlNasMsg.enbUeS1apId,ueCb->crnti))
  {
    RETVALUE(RFAILED);
  }

  /* TODO Code to be added to handle Handover
   * Simply send the message to RRC, Fill RRC DL Information Transfer*/
  if( ROK != uehSndRrcDlInfoTfr( ueCb, dlNasMsg.nasPdu.pduLen, dlNasMsg.nasPdu.pdu ) )
  {
    RETVALUE(RFAILED);
  }

  RETVALUE(ROK);
}

/**
 * @details This function is invoked when UeContextReleaseCommand from MME is received
 * Send RrcRelease to UE
 * Send UeContextReleaseComplete to MME.
 **/
PUBLIC S16 uehProcUeCtxRelCmd(
    UehUeCb      *ueCb,
    S1apPdu      *pdu
  )
{
  UehS1ConCb      *s1ConCb;
  SztRelRsp        relEvnt;
  UeContextRelCmp  ueCtxRelCmpl;
  UeContextRelCmd  ueContextRelCmd;
  
  logInfo("uehProcUeCtxRelCmd: S1 UE Context Release Command received for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  clearBytes(&relEvnt,sizeof(SztRelRsp));
  clearBytes(&ueCtxRelCmpl,sizeof(UeContextRelCmp));
  clearBytes(&ueContextRelCmd,sizeof(UeContextRelCmd));


  s1ConCb = &ueCb->s1ConCb;
  /* Decode the received Pdu */
  s1apUnwrapUeContextRelCmdPdu( pdu, &ueContextRelCmd );
   
  switch(ueContextRelCmd.ueS1apId.choice)
  {
    case UeS1apIdChoice_MmeUeS1apId:
    if (RFAILED == uehValidateEnbAndMmeUeS1apIds(ueContextRelCmd.ueS1apId.u.mmeUeS1apId,-1,ueCb->crnti))
    {
      RETVALUE(RFAILED);
    }
    break;
    case UeS1apIdChoice_UeIdPair:
    if (RFAILED == uehValidateEnbAndMmeUeS1apIds(ueContextRelCmd.ueS1apId.u.ueS1apIdPair.mmeUeS1apId,ueContextRelCmd.ueS1apId.u.ueS1apIdPair.enbUeS1apId,ueCb->crnti))
    {
      RETVALUE(RFAILED);
    }
    break;
  }
     /* Updating the Rab Release Cause */
  ueCb->rabReleaseCause = RABRELEASECAUSE_UECONTEXTRELEASED;
  SET_REL_CAUSE_IN_UECB(ueCb, ueContextRelCmd.cause.causeType, ueContextRelCmd.cause.causeVal);
  if(SztCauseRadioNwload_balancing_tau_requiredEnum == ueContextRelCmd.cause.causeVal)
  {
    ueCb->ueReleaseCauseVal = NhuRlsCauseloadBalancingTAUrequiredEnum;
  } 
  
  
  uehFillUeCtxRelCmpl(&ueCtxRelCmpl, s1ConCb);
  s1apWrapUeContextRelCmpPdu( &(relEvnt.pdu), &ueCtxRelCmpl);

  /* Send the Ue Context Release Complete to MME */
  relEvnt.spConnId = s1ConCb->spConnId;

  logInfo("UE Context Release Response is sent to MME for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  if(ROK !=  uehSndMsgOut (ENTSZ, EVENTTYPE_UECONTEXTRELCOM, ueCb, TRANSID_NONE,&relEvnt))
  {
   RETVALUE(RFAILED);
  }

  /* UE Procedure type is updaed here to handle RLC CFM messages */
  if (UEH_UE_CXT_REL_DUE_TO_RL_FAIL == ueCb->ueProcedure)
  {
    /* Sends Context release Request to PHY, MAC,RRC and Tunnel delete request to Egtp */
    /* Send NHU UE Context Release */
    UehLiNhuCnclUeReq(&uehCb.nhuSap[0]->pst, uehCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);
    uehClearUeCtxInNodeB(ueCb);
  }
  else
  {
    uehStartRrcConnClear(ueCb);
  }


  uehUeProcStart(ueCb,UEH_UE_CONN_REL_ONGOING);
  uehSetState(ueCb, UE_STATE_REL_ONGOING);

  RETVALUE(ROK);
}

/* @details Releases all resources allocated to this UE CB 
 * @TODO: Release based on which are allocated.
 */
void uehDeallocateResources(
  FR UehUeCb    *ueCb
)
{
  SztRelReq      relReq;

  uehSetState(ueCb, UE_STATE_REL_ONGOING);
#ifndef RRC_PHY_CFG_SUPP
  /* 1. CL */ 
  uehSndCtfUeRls(ueCb);
#endif
  /* 2. Scheduler */ 
  uehSndRgrUeRls(ueCb);
  /* 3. RRC - this should take care of RLC, MAC and PDCP also */
  UehLiNhuCnclUeReq(&uehCb.nhuSap[0]->pst, uehCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);
  /* 4. GTPU */
  uehSndEgtpCom(EGTP_TUN_DEL, ueCb);
  /* 5. UDH */
  if(TRUE == ueCb->rbInfo.drb[0].isUdhRabEnbl)
  {
    uehSndUdhDelUeReq(ueCb);
  }
  /* 6. S1AP */
  relReq.relType     = SZT_LOCAL_REL;
  relReq.connIdType  = SZT_CONN_TYPE_SU;
  relReq.connId      = ueCb->s1ConCb.suConnId;
  uehSndMsgOut (ENTSZ, EVENTTYPE_RESERVED1, ueCb, TRANSID_NONE, &relReq);

  freeUeCb(ueCb);
}

/**
 * @details Received Error Indication from MME.
 **/
PUBLIC S16 uehProcS1ErrInd(
    UehUeCb    *ueCb,
    S1apPdu    *pdu
  )
{
  SztRelReq      relReq;
  ErrInd         errInd;
  S1Cause        cause;

  s1apUnwrapErrInd( pdu, &errInd );
  UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTSZTERRIND, ENTSZ, errInd.cause.causeVal, UE, ueCb->crnti);
  /* Updating the Rab Release Cause */
  ueCb->rabReleaseCause = RABRELEASECAUSE_UECONTEXTRELEASED;
  SET_REL_CAUSE_IN_UECB(ueCb, errInd.cause.causeType,errInd.cause.causeVal );
 
  if(UEH_UE_CXT_REL_DUE_TO_RL_FAIL EQ ueCb->ueProcedure)
  {
     /* Manu, 29-Oct-12: Added
      * Scenario:
      * 1. RLF detected at ENB.
      * 2. Context Release Request sent to MME.
      * 3. MME Responds with ErrorInd.
      * Handling: Delete all resources in ENB Locally.
      */
    uehDeallocateResources(ueCb);
    RETVALUE(ROK);
  }
    /* Sends Context release Request to PHY, MAC,RRC and Tunnel delete request to Egtp */
    /* Send NHU UE Context Release */
  if ((UEH_UE_REL_REQ_SENT == ueCb->ueProcedure) || (UE_STATE_S1_CONNECTING == ueCb->ueState))
  {
    /*In UE_STATE_S1_CONNECTING UE_TMR_UE_CTX_REL timer will not be started and stopping the timer is not required,but for optimization in both UEH_UE_REL_REQ_SENT procedure and UE_STATE_S1_CONNECTING state are kept in single condition*/
    uehStopTmr(UE_TMR_UE_CTX_REL,&((UehUeCb *)ueCb)->ueCtxRelTmr,(PTR)ueCb, ((UehUeCb *)ueCb)->crnti); 
    uehStartRrcConnClear(ueCb);
    uehSetState(ueCb, UE_STATE_REL_ONGOING);
    RETVALUE(ROK);
  }
  
  if (UEH_INITIAL_CXT_SETUP == ueCb->ueProcedure)
  {
    relReq.relType = SZT_LOCAL_REL;
    relReq.connIdType = SZT_CONN_TYPE_SU;
    relReq.connId = ueCb->s1ConCb.suConnId;
    uehUeProcStart(ueCb,UEH_INITIAL_CXT_SETUP_FAILURE);
   RETVALUE( uehSndMsgOut (ENTSZ, EVENTTYPE_INITCONTEXTSETUPFAIL, ueCb, TRANSID_NONE, &relReq));
  }

  cause.causeType = errInd.cause.causeVal;
  cause.causeVal =  errInd.cause.causeType;
  
  if (ROK != uehSndS1UeCntxtRlsReq(ueCb, &cause))
  {
    UEH_ERROR_SSI_EVENT_SEND_FAILED( (&uehCb.sztSap[0]->pst), EVENTTYPE_UECONTEXTRELREQ, UE, ueCb->crnti);
    RETVALUE(RFAILED);
  }
  
  RETVALUE(ROK);
}


/**************************************************************
  Following functions handles the Timer Expiry Events received at UEH
***************************************************************/
/**
 * @details UE_TMR_RRCCONNRELL2ACK, guards against the L2 ACK Event,
 * When we send RrcConnectionRelease to User we wait for the ACK from lower layers,
 * before we clear the EGTP tunnels of this user
 * Stop: After receiving L2Ack from RRC
 * Expiry: we clear all EGTP tunnels, and clear the UE Context
 **/
PUBLIC S16 uehProcRrcConRelL2AckTmrExpiry(
    UehUeCb    *ueCb
  )
{
  /* Error case: No response from UE. Clear contexts in RRC, PDCP, RLC, MAC */
  uehUeProcStart(ueCb,UEH_UE_CTXT_CLEAR_ONGOING);
  UehLiNhuCnclUeReq(&uehCb.nhuSap[0]->pst, uehCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);
  /* Delete contexts in PHY, SCH, GTP */ 
  uehClearUeCtxInNodeB(ueCb);
  RETVALUE(ROK);
}

void uehClearCallInRrcConnectingState(
    UehUeCb        *ueCb
  )
{

  /* Delete RRC level UE context and lower layers, except Phy */
  UehLiNhuCnclUeReq(&uehCb.nhuSap[0]->pst, uehCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);

  /* send RGR UE Context Release*/
  uehSndRgrUeRls(ueCb);
  /* Delete the Phy context */
#ifndef RRC_PHY_CFG_SUPP
  uehSndCtfUeRls(ueCb);
#endif
  /* Clear ueCb and s1ConCb */
  freeUeCb(ueCb);
}

/**
 * @details Received S1_CLEAR from MME, so clear the UE, No need of handling RRC connection furthur
 * Presently we don't have a way to identify the UE's in this state, We need to review this
 **/
PUBLIC S16 uehProcS1ClearInRrcCon(
    UehUeCb     *ueCb
  )
{
  uehClearCallInRrcConnectingState(ueCb);
  RETVALUE(ROK);
}

/**
 * @details UE_TMR_UE_RESPONSE, guards against the delayed responses from UE
 * When we send RrcSetup Request to UE, we wait for response from UE,
 * Stop: After receiving RrcSetupComplete from UE, we stop this timer.
 * Expiry: We send RRC Connection Release to UE
 **/
PUBLIC S16 uehProcUeRspTmrExpiryInRrcCon(
    UehUeCb    *ueCb
  )
{
  uehClearCallInRrcConnectingState(ueCb);

  RETVALUE(ROK);
}

/**
 * @details UE_TMR_UE_SRB1, guards against the delayed responses from MME
 * When we send InitialUeMessage to MME, we wait for response from MME,
 * Stop: After receiving either a DL NAS/InitialContextSetup for this UE, we stop this timer.
 * Expiry: We send RRC Connection Release to UE
 **/
PUBLIC S16 uehProcSrb1TmrExpiry(
    UehUeCb   *ueCb
  )
{
  logFunc("%s", "uehProcSrb1TmrExpiry");

  uehStartRrcConnClear( ueCb );
  uehUeProcStart(ueCb,UEH_UE_CONN_REL_ONGOING);
  uehSetState(ueCb, UE_STATE_REL_ONGOING);

  RETVALUE(ROK);
}

/**
 * @details UE_TMR_UE_CTX_REL, guards against early release of UE PHY
 * TODO Add proper comments
 * Stop:
 * Expiry:
 **/
PUBLIC S16 uehProcCtxRelTmrExpiry(
    UehUeCb     *ueCb
  )
{
  logFunc("%s", "uehProcCtxRelTmrExpiry");
  /* UE Procedure type is updaed here to handle RLC CFM messages */
  if (UE_STATE_REL_ONGOING == ueCb->ueState || UEH_UE_CXT_REL_DUE_TO_RL_FAIL == ueCb->ueProcedure)
  {
    /* Sends Context release Request to PHY, MAC,RRC and Tunnel delete request to Egtp */
    /* Send NHU UE Context Release */
    logInfo("Ctx Rel timer expiry handling for UE: %d", ueCb->crnti);
    UehLiNhuCnclUeReq(&uehCb.nhuSap[0]->pst, uehCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);
    uehClearUeCtxInNodeB(ueCb);
  }
  else
  {
    uehStartRrcConnClear(ueCb);
  }

  if(ueCb->isUeCbAllocated)
  {
    uehUeProcStart(ueCb,UEH_UE_CONN_REL_ONGOING);
    uehSetState(ueCb, UE_STATE_REL_ONGOING);
  }
  RETVALUE(ROK);
}

/**
 * @details,UEH_TMR_WAIT_MME_REL Waits for the ue context release command from MME
 * Stop:
 * Expiry:
 **/
PUBLIC S16 uehProcMmeRelIndTmrExp (
    UehUeCb     *ueCb
  )
{
  SztDatEvntReq     datEvt;
  UeContextRelReq   ueCtxRelReqMsg;
  S1Cause           cause;

  UEH_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));
  /* Construct UeContextRelase S1AP Message,
   */
  cause.causeType = ueCb->s1ConCb.s1CauseType;
  cause.causeVal =  ueCb->s1ConCb.s1Cause;

  uehFillUeCtxRelReq(&ueCtxRelReqMsg, &ueCb->s1ConCb, &cause );
  s1apWrapUeContextRelReqPdu(&datEvt.pdu, &ueCtxRelReqMsg);
  datEvt.spConnId = ueCb->s1ConCb.spConnId;

  /* Send the message to S1ap Stack */
 RETVALUE( uehSndMsgOut (ENTSZ, EVENTTYPE_UECONTEXTRELREQ, ueCb, TRANSID_NONE,&datEvt));

}



/**
 * @details UE_TMR_CRNTI_RETENTION, guards against early release of UE PHY
 * TODO Add proper comments
 * Stop:
 * Expiry:
 **/
PUBLIC S16 uehProcUeCrntiRetTmrExpiry(
    UehUeCb   *ueCb
  )
{
  if (UEH_RRC_CON_SETUP == ueCb->ueProcedure)
  {
    /* send RGR UE Context Release*/
    uehSndRgrUeRls(ueCb);
#ifndef RRC_PHY_CFG_SUPP
    /* send PHY UE Context Release*/
    uehSndCtfUeRls(ueCb);
#endif
  }

  freeUeCb(ueCb);

  RETVALUE(ROK);
}

/**
 * @details UE_TMR_UE_RESPONSE, guards against the delayed responses from UE
 * When we send messages to UE after S1 Connected, we wait for response 
 * from UE,
 * Stop: After receiving confirm nessage from this UE, we stop this timer.
 * Expiry: We send InitialContextSetupFail to MME
 **/
PUBLIC S16 uehProcUeRspTmrExpiryAftrS1Conctd(
    UehUeCb   *ueCb
  )
{
  uehSndInitCtxtSetupFail( &ueCb->s1ConCb, CauseType_RadioNw, SztCauseRadioNwfailure_in_radio_interface_procedureEnum );
  
  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehSndEgtpDelInitCtxtSetupFail 
 *
 *      Desc:  Sends the GTP tunnel Del and 
 *             Initial Context Setup Fail 
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndEgtpDelInitCtxtSetupFail(
    UehUeCb       *ueCb,
    UINT16        causeType,
    UINT32        causeVal
  )
{
  logFunc("%s", "uehSndEgtpDelInitCtxtSetupFail");

  /* Send Initial Context Setup Failure */
  uehSndInitCtxtSetupFail(&ueCb->s1ConCb, causeType, causeVal);

  RETVALUE(ROK);
} /* end of uehCanclUeSndRrcConRej */


/*
 *
 *      Fun:   uehProcUeRspTmrExpiryInRecOngoing
 *
 *      Desc:  UE Response Timer Expiry in REC Ongoing
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcUeRspTmrExpiryInRecOngoing(
    UehUeCb    *ueCb
  )
{
  /* Send Initial Context Setup Failure */
  uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_RadioNw, SztCauseRadioNwfailure_in_radio_interface_procedureEnum);

  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehSndUENhuDelCfgTransReq
 *
 *      Desc:  UE NHU Delete Configuration Transaction
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndUENhuDelCfgTransReq(
    NhuCellId  cellId,
    NhuCrnti   ueId,
    U32        transId
  )
{
  Pst           *pst = NULLP;

  logFunc("%s", "uehSndUENhuDelCfgTransReq");

  pst = &uehCb.nhuSap[0]->pst;
  pst->event   = EVTNHUDELETETRANSREQ;

  if(ROK != UehLiNhuDeleteTransReq(pst, uehCb.nhuSap[0]->spId, cellId, ueId, transId))
  {
    UEH_ERROR_SSI_EVENT_SEND_FAILED( pst, EVENTTYPE_OTHER, UE, ueId);
    RETVALUE(RFAILED);
  }

  RETVALUE(ROK);
}


/*
 *
 *      Fun:   uehSndUeCapEnqiry
 *
 *      Desc:  Sends the UE Capability Enquiry 
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndUeCapEnqiry(
    UehUeCb     *ueCb
  )
{
  S16            ret = RFAILED;
  NhuDatReqSdus *nhuDatReqEvntSdu = NULLP;

  logFunc("%s", "uehSndUeCapEnqiry");

  /* Manu, 21Aug: Incorrect allocation
   * orig code  :  UEH_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(nhuDatReqEvntSdu));
   * issue      :  sizeof(nhuDatReqEvntSdu) is 4 bytes, size of pointer
   * correct    :  replace by sizeof(NhuDatReqSdus)
   */ 
  UEH_ALLOCEVNT_OR_RET(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));

  uehFillNhuEvntHdr( &(nhuDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_UECAPENQUIRY);
  ret = uehFillNhuUeCapEnquiry (ueCb, nhuDatReqEvntSdu);
  if (ROK != ret)
  {
    UEH_ERROR_MSG_FILLING_FAILED("UE Capability Request", UE, ueCb->crnti);
    UEH_FREEEVNT(nhuDatReqEvntSdu);
    RETVALUE(RFAILED);
  }

  nhuDatReqEvntSdu->sdu.isUeCfgPres = FALSE;


  /* Send UE Capability Enquiry through NhuDatRsp Primitive*/
  logInfo("UE Capability Request sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  RETVALUE( uehSndMsgOut (ENTNH, EVENTTYPE_UECAPABILITYENQ, ueCb, TRANSID_NONE, nhuDatReqEvntSdu));

}


/*
 *
 *      Fun:   uehProcUeCapEnqInfo
 *
 *      Desc:  UE Capability Information
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcUeCapEnqInfo(
    UehUeCb           *ueCb,
    NhuDatIndSdus     *datIndSdu
  )
{
  NhuUE_CapRAT_ContLst  *ueCapList;

  logFunc("%s", "uehProcUeCapEnqInfo");

  uehStopTmr(UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);

  ueCapList = &(datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.ueCapInfo.criticalExtns.val.c1.val.ueCapInfo_r8.ue_CapRAT_ContLst);
  
  /* Store the UE Cap RAT Container */
  ueCb->ueCapRatCntr.ratType = ueCapList->member[0].rat_Typ.val; /* supporting only E-UTRAN */
  ueCb->ueCapRatCntr.infoLen = ueCapList->member[0].ueCapRAT_Cont.len;
  UEH_MEM_COPY(ueCb->ueCapRatCntr.ueCapRatCntr, ueCapList->member[0].ueCapRAT_Cont.val, ueCb->ueCapRatCntr.infoLen);

  /* Send RRC to decode the UE-EUTRA-Capability Information */
  uehSndUeEutraCapDecodeReq(ueCb);

  RETVALUE(ROK);
} /* uehProcUeCapEnqInfo */


/*
 *
 *      Fun:   uehSndUeEutraCapDecodeReq
 *
 *      Desc:  UE-EUTRA-Capability Decode Request
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndUeEutraCapDecodeReq(
    UehUeCb        *ueCb
  )
{
  NhuDecodeSduReq       *nhuDecodeSduReq = NULLP;

  logFunc("%s", "uehSndUeEutraCapDecodeReq");

  UEH_ALLOC(&nhuDecodeSduReq, sizeof(NhuDecodeSduReq));
  if( NULL == nhuDecodeSduReq)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(sizeof(NhuDecodeSduReq), "NhuDecodeSduReq");
    RETVALUE(RFAILED);
  }

  uehFillNhuEvntHdr(&(nhuDecodeSduReq->hdr), ueCb->cellId, ueCb->crnti, E_UTRAN_UE_EUTRACAP_DECODE);
  nhuDecodeSduReq->sduType = NHU_DEC_SDUTYPE_EUTRA_UECAP_IE;
  UEH_ALLOCMBUF(DFLT_REGION, DFLT_POOL, &nhuDecodeSduReq->mbuf);
  if(NULLP ==  nhuDecodeSduReq->mbuf)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(NOT_APPLICABLE,"nhuDecodeSduReq");
    UEH_FREE(nhuDecodeSduReq, sizeof(NhuDecodeSduReq));
    RETVALUE(RFAILED);
  }

  SAddPstMsgMult((Data *)(ueCb->ueCapRatCntr.ueCapRatCntr), ueCb->ueCapRatCntr.infoLen, nhuDecodeSduReq->mbuf);
  if (RFAILED == uehSndNhuDecodeReq(nhuDecodeSduReq))
  {
    RETVALUE(RFAILED);
  }
  logInfo("UE EUTRA Capability Decode request sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  RETVALUE(ROK);
}


/*
 *
 *      Fun:   uehProcUeCapDecCfm
 *
 *      Desc:  UE Capability Info Decode Confirm
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcUeCapDecCfm(
    UehUeCb             *ueCb,
    NhuDecodeSduCfm     *datIndSdu
  )
{
  NhuUE_EUTRA_Cap          *nhuUE_EUTRA_Cap = NULLP;
  NhuUERadioAccessCapInfo  *ueRadioCapInfo = NULLP;
  NhuUE_CapRAT_ContLst     *ueCapRATContLst = NULLP;
  U16 ueIdx = 0;
  
  logInfo("uehProcUeCapDecCfm: UE Capability Information Decode Confirm ueId:%d Status:%d for trandId:%d", datIndSdu->hdr.ueId,  datIndSdu->status, datIndSdu->hdr.transId);

  

  if (NHU_DECODE_SUCCESS != datIndSdu->status)
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUDCODECFM, ENTNH, datIndSdu->status, UE, ueCb->crnti);
    /* Send Initial Context Setup Failure */
    uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_RadioNw, SztCauseRadioNwradio_resources_not_availableEnum);
    RETVALUE(ROK);
  }

  switch(datIndSdu->hdr.transId)
  {
    case E_UTRAN_UE_RADACCCAPINFO_DECODE:
    {
      ueRadioCapInfo = &(datIndSdu->ie.ueRadioCapInfo);
      /* Choice 0 corresponds to ueRadioAccessCapabilityInformation-r8
       * other than 0 is spare, so send ICS failure
       */
      if ((0 == ueRadioCapInfo->criticalExtns.choice.val) && (0 == ueRadioCapInfo->criticalExtns.val.c1.choice.val))
      {
        /* Store the UE Radio Access Capability information */
        ueCb->ueRadCap.ueCapInfo.infoLen = ueRadioCapInfo->criticalExtns.val.c1.val.ueRadioAccessCapInfo_r8.ue_RadioAccessCapInfo.len;
        UEH_MEM_COPY(ueCb->ueRadCap.ueCapInfo.capInfopdu, ueRadioCapInfo->criticalExtns.val.c1.val.ueRadioAccessCapInfo_r8.ue_RadioAccessCapInfo.val, \
            ueCb->ueRadCap.ueCapInfo.infoLen);

        uehSndUeCapInfoDecodeReq(ueCb);
      }
      else
      {
        uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_Prototcol, SztCauseProtsemantic_errorEnum);
      }

      break;
    }

    case E_UTRAN_UE_CAPINFO_DECODE:
    {
      /* Choice 0 corresponds to ueCapInfo_r8
       * other than 0 is spare, so send ICS failure
       */
      if(0 == datIndSdu->ie.ueCapInfo.criticalExtns.val.c1.choice.val)
      {
        ueCapRATContLst = &(datIndSdu->ie.ueCapInfo.criticalExtns.val.c1.val.ueCapInfo_r8.ue_CapRAT_ContLst);
        /* Store the UE Capability information */
        ueCb->ueCapRatCntr.ratType = ueCapRATContLst->member[0].rat_Typ.val;
        ueCb->ueCapRatCntr.infoLen = ueCapRATContLst->member[0].ueCapRAT_Cont.len;
        UEH_MEM_COPY(ueCb->ueCapRatCntr.ueCapRatCntr, ueCapRATContLst->member[0].ueCapRAT_Cont.val, ueCb->ueCapRatCntr.infoLen);

        uehSndUeEutraCapDecodeReq(ueCb);
      }
      else
      { 
        uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_Prototcol, SztCauseProtsemantic_errorEnum);
      }

      break;
    }

    case E_UTRAN_UE_EUTRACAP_DECODE:
    {
      nhuUE_EUTRA_Cap = &(datIndSdu->ie.nhuUE_EUTRA_Cap);
      /* Store the UE E-UTRAN RAT Capability information */
      ueCb->ueEutraCap.accessStratumRls = nhuUE_EUTRA_Cap->accessStratumRls.val;
      ueCb->ueEutraCap.ueCategory = nhuUE_EUTRA_Cap->ue_Category.val;
      ueCb->ueEutraCap.rgrUeCatMask = UEH_UECAP_RGRMSK_NOTDONE;

      /* Store the ActiveUeStatus params */
      ueCb->ueAccessStratumRelease = ueCb->ueEutraCap.accessStratumRls;

      if (TRUE == nhuUE_EUTRA_Cap->featureGroupIndicators.pres)
      {
        ueCb->ueEutraCap.isFeatureGrpInd = TRUE;
        UEH_MEM_COPY(ueCb->ueEutraCap.featureGrpInd, nhuUE_EUTRA_Cap->featureGroupIndicators.val, 4); 
      }
      else
      {
        ueCb->ueEutraCap.isFeatureGrpInd = FALSE;
        UEH_SET_ONE(ueCb->ueEutraCap.featureGrpInd, 4);
      }

      /* If the UE Radio Access Cap Info Recieved from MME then 
       * no need to do UE capability Enquiry from UE 
       */
      if (TRUE != ueCb->ueRadCap.isUeRadAccCap)
      {
        /* Send the UE Capability information to NH for Encode */
        uehSndUeCapInfoEncodeReq(ueCb);
      }
      else
      {
        uehProcAdmissionReqSndEgtpCreateReq(ueCb);
      }

      break;
    }

    default:
    {
      UEH_ERROR_ELMNT_OUT_OF_RANGE("UE capability decode Info", datIndSdu->hdr.transId, E_UTRAN_UE_EUTRACAP_DECODE, E_UTRAN_UE_CAPINFO_DECODE);
      break;
    }
  } 
  ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
  //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus =  uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus | CAPAB_PROC_SUCC;
  RETVALUE(ROK);
} /* uehProcUeCapDecCfm */


/*
 *
 *      Fun:   uehProcUeCapEncCfm
 *
 *      Desc:  UE Capability Info Encode Confirm
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcUeCapEncCfm(
    UehUeCb           *ueCb,
    NhuEncCfmSdus     *nhuEncCfmSdus
  )
{
  logInfo("uehProcUeCapEncCfm: UE Capability Encode Confirm ueId:%d MsgCategory:%d Status:%d",nhuEncCfmSdus->hdr.ueId, nhuEncCfmSdus->sdu.msgCategory, nhuEncCfmSdus->sdu.status);

  if (NHU_ENCODE_SUCCESS != nhuEncCfmSdus->sdu.status)
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUENCODECFM, ENTNH, nhuEncCfmSdus->sdu.status, UE, ueCb->crnti);
    UEH_FREEMBUF(nhuEncCfmSdus->sdu.mBuf);
    RETVALUE(RFAILED);
  }

  if (NHU_MSG_UECAPINFO == nhuEncCfmSdus->sdu.msgCategory)
  {
    /* Store the UE Capability Information encoded buffer */
    ueCb->ueRadCap.isUeCapInfo = TRUE;
    UEH_BIT_TO_BYTE_CONVSN(ueCb->ueRadCap.ueCapInfo.infoLen, nhuEncCfmSdus->sdu.msgLen);
    SRemPreMsgMult(ueCb->ueRadCap.ueCapInfo.capInfopdu, ueCb->ueRadCap.ueCapInfo.infoLen, nhuEncCfmSdus->sdu.mBuf);

    /* Send the UE Radio Access Capability Information to NH for Encode */
    uehSndUeRadAccCapInfoEncodeReq(ueCb);
  }
  else if (NHU_MSG_UERACAPINFO == nhuEncCfmSdus->sdu.msgCategory)
  {
    /* Store the UE Radio Access Capability Information encoded buffer */
    ueCb->ueRadCap.isUeRadAccCap = TRUE;
    UEH_BIT_TO_BYTE_CONVSN(ueCb->ueRadCap.ueRadAccCap.len, nhuEncCfmSdus->sdu.msgLen);
    SRemPreMsgMult(ueCb->ueRadCap.ueRadAccCap.UeRadioCapability, ueCb->ueRadCap.ueRadAccCap.len, nhuEncCfmSdus->sdu.mBuf);

    /* Send UE CAPABILITY INFO IDICATION to MME */
    if (ROK != uehSndS1UeCapInfoIndReq(ueCb))
    {
      UEH_ERROR_SSI_EVENT_SEND_FAILED((&uehCb.sztSap[0]->pst), EVENTTYPE_UECAPABILITYINFOIND, UE, ueCb->crnti);
      UEH_FREEMBUF(nhuEncCfmSdus->sdu.mBuf);
      RETVALUE(RFAILED);
    }

    uehProcAdmissionReqSndEgtpCreateReq(ueCb);
  }
  else
  {
    UEH_ERROR_ELMNT_OUT_OF_RANGE( "Message category", nhuEncCfmSdus->sdu.msgCategory, NHU_MSG_UERACAPINFO, NHU_MSG_UECAPINFO);
  }

  /* Manu:10Aug: Memleak fix. Remove comment after code review */
  UEH_FREEMBUF(nhuEncCfmSdus->sdu.mBuf);
  RETVALUE(ROK);
} /* uehProcUeCapEncCfm */


/*
 *
 *      Fun:   uehSndNhuDecodeReq
 *
 *      Desc:  UE NHU Dat Req
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndNhuDecodeReq(
    NhuDecodeSduReq  *evt
  )
{
   Pst           *pst = NULLP;
   UehUeCb       *ueCb = NULLP;

   logFunc("%s", "uehSndNhuDecodeReq");
   UEH_GET_UECB_FROM_CRNTI (ueCb,evt->hdr.ueId);
   pst = &uehCb.nhuSap[0]->pst;
   pst->event   = EVTNHUDCODEREQ;
   RETVALUE( uehSndMsgOut (ENTNH, EVENTTYPE_RRCUECAPDECODEREQ, ueCb, TRANSID_NONE, evt));

}

/*
 *
 *      Fun:   uehSndUeCapInfoEncodeReq
 *
 *      Desc:  UE Capability Information Encode Request 
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndUeCapInfoEncodeReq(
    UehUeCb       *ueCb
  )
{
  NhuEncReqSdus         *nhuEncReqSdus = NULLP;
  NhuUECapInfo          *ueCapInfo;
  NhuUE_CapRAT_ContLst  *nhuUE_CapRAT_ContLst;
  Pst                   *pst = NULLP;

  logFunc("%s", "uehSndUeCapInfoEncodeReq");

  UEH_ALLOCEVNT_OR_RET(&nhuEncReqSdus, sizeof(NhuEncReqSdus));

  uehFillNhuEvntHdr(&(nhuEncReqSdus->hdr), ueCb->cellId, ueCb->crnti, E_UTRAN_UE_CAPINFO_ENCODE);
  nhuEncReqSdus->sdu.msgCategory = NHU_MSG_UECAPINFO;
  ueCapInfo = &(nhuEncReqSdus->sdu.u.ueCapInfo);
  UEH_FILL_TKN_UINT(ueCapInfo->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(ueCapInfo->rrc_TrnsxnId, UEH_VAL_ONE);
  UEH_FILL_TKN_UINT(ueCapInfo->criticalExtns.choice, CRITICALEXTNS_C20);
  UEH_FILL_TKN_UINT(ueCapInfo->criticalExtns.val.c1.choice, C20_UECAPINFO_R8);
  UEH_FILL_TKN_UINT(ueCapInfo->criticalExtns.val.c1.val.ueCapInfo_r8.pres, PRSNT_NODEF);

  nhuUE_CapRAT_ContLst = &(ueCapInfo->criticalExtns.val.c1.val.ueCapInfo_r8.ue_CapRAT_ContLst);
  UEH_FILL_TKN_UINT(nhuUE_CapRAT_ContLst->noComp, 1);

  UEH_GET_MEM(nhuEncReqSdus, (nhuUE_CapRAT_ContLst->noComp.val) * (sizeof(NhuUE_CapRAT_Cont)), &(nhuUE_CapRAT_ContLst->member));
  UEH_FILL_TKN_UINT(nhuUE_CapRAT_ContLst->member[0].pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(nhuUE_CapRAT_ContLst->member[0].rat_Typ, 0); /* E-UTRAN */

  nhuUE_CapRAT_ContLst->member[0].ueCapRAT_Cont.pres = PRSNT_NODEF;
  UEH_GET_MEM(nhuEncReqSdus, ueCb->ueCapRatCntr.infoLen, &(nhuUE_CapRAT_ContLst->member[0].ueCapRAT_Cont.val));
  nhuUE_CapRAT_ContLst->member[0].ueCapRAT_Cont.len = ueCb->ueCapRatCntr.infoLen;
  UEH_MEM_COPY(nhuUE_CapRAT_ContLst->member[0].ueCapRAT_Cont.val, ueCb->ueCapRatCntr.ueCapRatCntr, ueCb->ueCapRatCntr.infoLen);

  pst = &uehCb.nhuSap[0]->pst;
  pst->event = EVTNHUENCODEREQ;

  if(ROK != UehLiNhuEncodeReq(pst, uehCb.nhuSap[0]->spId, nhuEncReqSdus))
  {
    UEH_ERROR_SSI_EVENT_SEND_FAILED(pst, EVENTTYPE_UECAPABILITYINFOIND, UE, ueCb->crnti);
    UEH_FREE(nhuEncReqSdus, sizeof(NhuEncReqSdus));
    RETVALUE(RFAILED);
      
  }
  logInfo("UE Capability Encode Request sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehSndUeRadAccCapInfoEncodeReq
 *
 *      Desc:  UE Radio Access Capability Information Encode Request 
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndUeRadAccCapInfoEncodeReq(
    UehUeCb        *ueCb
  )
{
  NhuEncReqSdus           *nhuEncReqSdus = NULLP;
  NhuUERadioAccessCapInfo *ueRadAccCapInfo;
  NhuUERadioAccessCapInfo_r8_IEsue_RadioAccessCapInfo *ueCapInfo;
  Pst                     *pst = NULLP;

  logFunc("%s", "uehSndUeRadAccCapInfoEncodeReq");

  UEH_ALLOCEVNT_OR_RET(&nhuEncReqSdus, sizeof(NhuEncReqSdus));

  uehFillNhuEvntHdr(&(nhuEncReqSdus->hdr), ueCb->cellId, ueCb->crnti, E_UTRAN_UE_RADACCCAPINFO_ENCODE);
  nhuEncReqSdus->sdu.msgCategory = NHU_MSG_UERACAPINFO;
  ueRadAccCapInfo = &(nhuEncReqSdus->sdu.u.ueRadioCapInfo);
  UEH_FILL_TKN_UINT(ueRadAccCapInfo->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(ueRadAccCapInfo->criticalExtns.choice, CRITICALEXTNS_C24);
  UEH_FILL_TKN_UINT(ueRadAccCapInfo->criticalExtns.val.c1.choice, C24_UERADIOACCESSCAPINFO_R8);
  UEH_FILL_TKN_UINT(ueRadAccCapInfo->criticalExtns.val.c1.val.ueRadioAccessCapInfo_r8.pres, PRSNT_NODEF);

  ueCapInfo = &(ueRadAccCapInfo->criticalExtns.val.c1.val.ueRadioAccessCapInfo_r8.ue_RadioAccessCapInfo);
  ueCapInfo->pres = PRSNT_NODEF;
  ueCapInfo->len = ueCb->ueRadCap.ueCapInfo.infoLen;
  UEH_GET_MEM(nhuEncReqSdus,  ueCapInfo->len, &(ueCapInfo->val));
  if( NULL == ueCapInfo->val)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(ueCapInfo->len, "ueCapInfo->val");
    RETVALUE(RFAILED);
  }

  UEH_MEM_COPY(ueCapInfo->val, ueCb->ueRadCap.ueCapInfo.capInfopdu, ueCapInfo->len);

  pst = &uehCb.nhuSap[0]->pst;
  pst->event = EVTNHUENCODEREQ;

  if(ROK != UehLiNhuEncodeReq(pst, uehCb.nhuSap[0]->spId, nhuEncReqSdus))
  {
    UEH_ERROR_SSI_EVENT_SEND_FAILED( pst, EVENTTYPE_OTHER, UE, ueCb->crnti); 
    UEH_FREE(nhuEncReqSdus, sizeof(NhuEncReqSdus));
    RETVALUE(RFAILED);
  }
  logInfo("UE Radio Access Capability Encode Request sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  
  RETVALUE(ROK);
}


/*
 *
 *      Fun:   uehSndS1UeCapInfoIndReq
 *
 *      Desc:  RRC UL Info transfer
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndS1UeCapInfoIndReq(
    UehUeCb       *ueCb
  )
{
  SztDatEvntReq        datEvt;
  UeCapInfoInd         ueCapInfoInd;

  logFunc("%s", "uehSndS1UeCapInfoIndReq");
  
  UEH_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));

  /* Construct S1AP Message */
  uehFillUeCapInfoIndMsg(ueCb, &ueCapInfoInd);
  s1apWrapUeCapInfoIndPdu(&datEvt.pdu, &ueCapInfoInd);
  datEvt.spConnId = ueCb->s1ConCb.spConnId;

  /* Send the message to S1ap Stack */
  RETVALUE(uehSndMsgOut (ENTSZ, EVENTTYPE_UECAPABILITYINFOIND, ueCb, TRANSID_NONE,&datEvt));
 


}

/*
 *
 *      Fun:   uehSndUeRadioAccCapInfoDecReq
 *
 *      Desc:  Ue Radio Access Capability Information Decode Request
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndUeRadioAccCapInfoDecReq(
    UehUeCb     *ueCb
  )
{
  NhuDecodeSduReq       *nhuDecodeSduReq = NULLP;

  logFunc("%s", "uehSndUeRadioAccCapInfoDecReq");
  //UECM_EVENTHDLR(ueCb->crnti, uehSndUeRadioAccCapInfoDecReq, "UE Radio Access Capability Info Decode Request \n");

  UEH_ALLOC(&nhuDecodeSduReq, sizeof(NhuDecodeSduReq));
  if( NULL == nhuDecodeSduReq)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(sizeof(NhuDecodeSduReq), "NhuDecodeSduReq");
    RETVALUE(RFAILED);
  }

  uehFillNhuEvntHdr(&(nhuDecodeSduReq->hdr), ueCb->cellId, ueCb->crnti, E_UTRAN_UE_RADACCCAPINFO_DECODE);
  nhuDecodeSduReq->sduType = NHU_MSG_UERACAPINFO;
  UEH_ALLOCMBUF(DFLT_REGION, DFLT_POOL, &nhuDecodeSduReq->mbuf);
  if(NULLP ==  nhuDecodeSduReq->mbuf)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(NOT_APPLICABLE,"nhuDecodeSduReq");
    UEH_FREE(nhuDecodeSduReq, sizeof(NhuDecodeSduReq));
    RETVALUE(RFAILED);
  }

  SAddPstMsgMult((Data *)(ueCb->ueRadCap.ueRadAccCap.UeRadioCapability), ueCb->ueRadCap.ueRadAccCap.len, nhuDecodeSduReq->mbuf);

  if (RFAILED == uehSndNhuDecodeReq(nhuDecodeSduReq))
  {
    UEH_FREEMBUF(nhuDecodeSduReq->mbuf);
    UEH_FREE(nhuDecodeSduReq, sizeof(NhuDecodeSduReq));
    RETVALUE(RFAILED);
  }
  logInfo("UE Radio Access Cap Decode Request sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  RETVALUE(ROK);
}


/*
 *
 *      Fun:   uehSndUeCapInfoDecodeReq
 *
 *      Desc:  Ue Capability Information Decode Request
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndUeCapInfoDecodeReq(
    UehUeCb     *ueCb
  )
{
  NhuDecodeSduReq       *nhuDecodeSduReq = NULLP;

  logFunc("%s", "uehSndUeCapInfoDecodeReq");

  UEH_ALLOC(&nhuDecodeSduReq, sizeof(NhuDecodeSduReq));
  if( NULL == nhuDecodeSduReq)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(sizeof(NhuDecodeSduReq), "NhuDecodeSduReq");
    RETVALUE(RFAILED);
  }

  uehFillNhuEvntHdr(&(nhuDecodeSduReq->hdr), ueCb->cellId, ueCb->crnti, E_UTRAN_UE_CAPINFO_DECODE);
  nhuDecodeSduReq->sduType = NHU_MSG_UECAPINFO;
  UEH_ALLOCMBUF(DFLT_REGION, DFLT_POOL, &nhuDecodeSduReq->mbuf);
  if(NULLP ==  nhuDecodeSduReq->mbuf)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(NOT_APPLICABLE,"nhuDecodeSduReq");
    UEH_FREE(nhuDecodeSduReq, sizeof(NhuDecodeSduReq));
    RETVALUE(RFAILED);
  }

  SAddPstMsgMult((Data *)(ueCb->ueRadCap.ueCapInfo.capInfopdu), ueCb->ueRadCap.ueCapInfo.infoLen, nhuDecodeSduReq->mbuf);

  if (RFAILED == uehSndNhuDecodeReq(nhuDecodeSduReq))
  {
    UEH_FREEMBUF(nhuDecodeSduReq->mbuf);
    UEH_FREE(nhuDecodeSduReq, sizeof(NhuDecodeSduReq));
    RETVALUE(RFAILED);
  }
  logInfo("UE Capability Decode Request sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  RETVALUE(ROK);
}


/*
 *
 *      Fun:   uehProcAdmissionReqSndEgtpCreateReq
 *
 *      Desc:  Admission Request and Egtp tunnel create Req
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcAdmissionReqSndEgtpCreateReq(
    UehUeCb     *ueCb
  )
{
  UINT8  causeType, causeVal;

  logFunc("%s", "uehProcAdmissionReqSndEgtpCreateReq");

  /* CS3: Admission Control */
  if(ROK != initContxtAdmissionRequest(ueCb , &causeType, &causeVal ))
  {
    UEH_PROCEDURE_FAILED("Initial Context admission request", UE, ueCb->crnti, "Admission Failure");
    uehSndInitCtxtSetupFail(&ueCb->s1ConCb, causeType, causeVal);
    
    RETVALUE(RFAILED);
  }


  /* CS4: GTP Tunnel Creation */
  uehSndEgtpCom(EGTP_TUN_CREATE, ueCb);
  uehSetState(ueCb, UE_STATE_REC_ONGOING);

  RETVALUE(ROK);
}


/*
 *
 *      Fun:   uehSndUdhAddRabReq
 *
 *      Desc:  Send UDH ERAB Add Request
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndUdhAddRabReq(
    UehUeCb      *ueCb
  )
{
  U8                       idx = 0;
  U8                       drbIdx = 0;
  UehUdhAddRabReq          rabReq ;
  Pst                      *pst = NULLP;


  clearBytes(&rabReq ,sizeof(UehUdhAddRabReq));

  /* TODO: right now UdhAddRabReq interface supports single RAB
   * in R2 this API supports multiple ERABS
   */
  for(idx = 0; idx < ueCb->rbInfo.numOfDrbAdd; idx++)
  {
    drbIdx = ueCb->rbInfo.drbAddLstIndx[idx];

    rabReq.locTeId       = ueCb->rbInfo.drb[drbIdx].locEgtpTunId;
    rabReq.remTeid       = ueCb->rbInfo.drb[drbIdx].remEgtpTunId;
    rabReq.qci           = ueCb->rbInfo.drb[drbIdx].qci;
    rabReq.rabId         = ueCb->rbInfo.drb[drbIdx].eRABId;
    rabReq.srcIpAddr     = uehCb.datAppAddr;
    rabReq.dstIpAddr     = ueCb->rbInfo.drb[drbIdx].sgwAddr;
    rabReq.pdcpId.cellId = ueCb->cellId;
    rabReq.pdcpId.ueId   = ueCb->crnti;
    rabReq.pdcpId.rbId   = ueCb->rbInfo.drb[drbIdx].pdcpId;
    rabReq.pdcpId.rbType = CM_LTE_DRB;
  }

  pst = &uehCb.udhSap[0]->pst;
 
  logInfo("ADD RAB Request UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  RETVALUE(uehSndMsgOut (ENTUDH, EVENTTYPE_UDHADDRABREQ, ueCb,TRANSID_NONE,&rabReq));

}

/*
 *
 *      Fun:   uehProcUdhAddRabCfm
 *
 *      Desc:  UDH ERAB Add Confirm handning
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcUdhAddRabCfm(
    UehUeCb            *ueCb,
    UdhUehAddRabCfm    *udhUehAddRabCfm
  )
{


  logInfo("AddRabCfm received for UeId:%d, eNbs1apId:%d mmeS1apId:%d, status:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id, udhUehAddRabCfm->status);
  if (ROK != udhUehAddRabCfm->status)
  {
    uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_Transport, SztCauseTportunspecifiedEnum);
    RETVALUE(ROK);
  }
  ueCb->numERabs++;
  /* Setting the RAB isUdhRabEnbl flag */
  UEH_UECB_RB_SET_RAB_STATUS(ueCb, udhUehAddRabCfm->pdcpId.rbId);

  /* RRC Connection Reconfiguration Request to UE*/
  uehSndRrcRecfgReq(ueCb);

  RETVALUE(ROK);
}

PUBLIC S16 uehSendUeCtxRelReq(
  UehUeCb    *ueCb,
  S1Cause    cause
)
{
  SztDatEvntReq         datEvt;
  UeContextRelReq       ueCtxRelReqMsg;

  logFunc("%s", "uehProcUdhUeInactvInd");
 
  UEH_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));

  ueCb->s1ConCb.s1CauseType = cause.causeType;
  ueCb->s1ConCb.s1Cause     = cause.causeVal;
  
  uehFillUeCtxRelReq(&ueCtxRelReqMsg, &ueCb->s1ConCb, &cause);
  s1apWrapUeContextRelReqPdu(&datEvt.pdu, &ueCtxRelReqMsg);
  datEvt.spConnId = ueCb->s1ConCb.spConnId;
  /* Send the message to S1ap Stack */
  logInfo("ueId=%d|eNbs1apId=%d|mmeS1apId=%d|Tx=UECONTEXTRELREQ|cause=%d %d", 
           ueCb->crnti,
           ueCb->s1ConCb.enb_ue_s1ap_id,
           ueCb->s1ConCb.mme_ue_s1ap_id,
           cause.causeType, 
           cause.causeVal);
  RETVALUE( uehSndMsgOut(ENTSZ, EVENTTYPE_UECONTEXTRELREQ, ueCb, TRANSID_NONE,&datEvt));
}
/*
 *
 *      Fun:   uehProcUdhUeInactvInd
 *
 *      Desc:  UDH UE Inactivity handling
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcUdhUeInactvInd(
    UehUeCb    *ueCb,
    void       *sdu
  )
{
  S1Cause cause = {.causeType = CauseType_RadioNw, .causeVal = SztCauseRadioNwuser_inactivityEnum};
  RETVALUE(uehSendUeCtxRelReq(ueCb, cause));
}

/*
 *
 *      Fun:   uehSndUdhDelUeReq
 *
 *      Desc:  Send UDH Del UE Request
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndUdhDelUeReq(
    UehUeCb     *ueCb
  )
{
  UehUdhDelUeReq           delUeReq;
  Pst                      *pst = NULLP;

  logFunc("%s", "uehSndUdhDelUeReq");

  clearBytes(&delUeReq, sizeof(UehUdhDelUeReq));

  delUeReq.ueId = ueCb->crnti;

  pst = &uehCb.udhSap[0]->pst;
 
  logInfo("UDH Delete request sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
 RETVALUE( uehSndMsgOut (ENTUDH, EVENTTYPE_UDHDELUEREQ, ueCb,TRANSID_NONE, &delUeReq));

}

/*
 *
 *      Fun:   uehSndUdhDelRabReq
 *
 *      Desc:  Send UDH Del Rab Request
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehSndUdhDelRabReq(
    UehUeCb     *ueCb
  )
{
  U8                drbIdx = 0;
  Pst               *pst = NULLP;
  UehUdhDelRabReq   delRabReq;

  logFunc("%s", "uehSndUdhDelRabReq");
  //UECM_EVENTHDLR(ueCb->crnti, uehSndUdhDelRabReq, "Send UDH Del Rab Request ");
  logInfo("%s","Sending UDH Del Rab Request"); 

  for(drbIdx = 0; drbIdx < ueCb->rbInfo.numOfRbCfgd; drbIdx++)
  {
    if(TRUE == ueCb->rbInfo.drb[drbIdx].isUdhRabEnbl)
    {
      clearBytes(&delRabReq, sizeof(UehUdhDelRabReq));

      delRabReq.pdcpId.cellId = ueCb->cellId;
      delRabReq.pdcpId.ueId = ueCb->crnti;
      delRabReq.pdcpId.rbId = ueCb->rbInfo.drb[drbIdx].rbId;
      delRabReq.pdcpId.rbType = ueCb->rbInfo.drb[drbIdx].rbType;
      delRabReq.rabReleaseCause = ueCb->rabReleaseCause;

      pst = &uehCb.udhSap[0]->pst;

      if(ROK != uehSndMsgOut (ENTUDH, EVENTTYPE_UDHDELRABREQ, ueCb,TRANSID_NONE, &delRabReq))
      {
        RETVALUE(RFAILED);
      }
      ueCb->rbInfo.drb[drbIdx].isUdhRabEnbl = FALSE;
    }
    else
    {
      freeUeCb(ueCb);
    }
  }
  RETVALUE(ROK);
}


/*
 *
 *      Fun:   uehProcUdhRabDelCfm
 *
 *      Desc:  UDH Del Rab Confirm handling
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcUdhRabDelCfm(
    UehUeCb          *ueCb,
    UdhUehDelRabCfm  *delRabCfm
  )
{

  logInfo("uehProcUdhRabDelCfm: Udh Rab Confirm for Crnti:%d eNbs1apId:%d mmeS1apId:%d RabId:%d Status:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id, delRabCfm->pdcpId.rbId, delRabCfm->status); 

  if (LCM_PRIM_OK != delRabCfm->status)
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVT_UDH_UEH_DEL_RAB_CFM, ENTUDH, delRabCfm->status, UE, ueCb->crnti);
    RETVALUE(ROK);
  }
  ueCb->numERabs--;

  /*TODO: in R2 Ue Context clearence should be done after 
   * recieving all the UDH Del Rab Confirm
   */
  freeUeCb(ueCb);

  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehProcUdhUeDelCfm
 *
 *      Desc:  UDH Del UE Confirm handling
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcUdhUeDelCfm(
    UehUeCb          *ueCb,
    UdhUehDelUeCfm  *delUeCfm
  )
{

  logInfo("uehProcUdhUeDelCfm: Udh Delete Confirm for Crnti:%d eNbs1apId:%d mmeS1apId:%d, Status:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id, delUeCfm->status);

  if (LCM_PRIM_OK != delUeCfm->status)
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT(EVT_UDH_UEH_DEL_UE_CFM,  ENTUDH, delUeCfm->status, UE, ueCb->crnti);
  }

  /* Presently supporting only one default bearer thats why numERabs value is decremented here. Support of dedicated bearers is in next release */
  ueCb->numERabs--;
  freeUeCb(ueCb);

  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehProcActiveUeStatusReq
 *
 *      Desc:  Process the Active UE status
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehProcActiveUeStatusReq(
    Buffer *mBuf
)
{
  UehCellCb     *cellCb = NULLP;
  U16           ueIdx = 0;
  UehUeCb       *ueCb = NULLP;
  MsgLen        cCnt = 0;
  MsgBsmUehActiveUeStatusReq  msgBsmUehActiveUeStatusReq;
  MsgUehBsmActiveUeStatusRsp  actUeSts;
  S16 numAllocatedCbs=0, numFreeCbs =0;
  logFunc("%s", "uehProcActiveUeStatusReq");
  logInfo("%s", "Received Active UE Status Request from BSM");
  
  SCpyMsgFix(mBuf, 0, sizeof(MsgBsmUehActiveUeStatusReq), (Data*)&msgBsmUehActiveUeStatusReq, &cCnt);
  clearBytes(&actUeSts, sizeof(MsgUehBsmActiveUeStatusRsp));

  actUeSts.cmdRef = msgBsmUehActiveUeStatusReq.cmdRef;
  actUeSts.numElem = 0;

  if(NULLP != uehCb.cellCb)
  {
    cellCb = uehGetCellCb();
    if(cellCb != NULLP)
    {
      for (ueIdx = 0; ueIdx < cellCb->maxRntiPool; ueIdx++)
      {
        UEH_GET_UECB_FROM_UEIDX(ueCb, ueIdx);
        if(NULLP != ueCb && ueCb->isUeCbAllocated)
        {
          numAllocatedCbs++;
          fillUeEventStatus(&actUeSts,ueCb);
        }
        else {
          numFreeCbs++;
        }
#if 0        
        if (actUeSts.numElem == cellCb->numUeCfg)
        {
          break;
        }
#endif
        /* Numer of Active UEs in Active UE Status Response message 
         * limited to MAX_ROWS_IN_ACTIVEUESTATUS_TABLE 
         */
        if(actUeSts.numElem >= MAX_ROWS_IN_ACTIVEUESTATUS_TABLE)
        {
          break;
        }
      }
    }
  }
  sendUehMsgToBsm(EVT_UEH_BSM_ACTIVE_UE_STATUS_RSP, sizeof(MsgUehBsmActiveUeStatusRsp), (UINT8*)&actUeSts);
  logCtr("numAllocatedCbs=%d,numFreeCbs=%d",numAllocatedCbs,numFreeCbs);
}

/* Filling the UE Status Table */
void fillUeEventStatus(MsgUehBsmActiveUeStatusRsp *actUeSts,UehUeCb *ueCb)
{
  actUeSts->activeUeStatus[actUeSts->numElem].ueAccessStratumRelease = ueCb->ueAccessStratumRelease ;
  actUeSts->activeUeStatus[actUeSts->numElem].activeIntProtAlgo = ueCb->activeIntProtAlgo;
  UEH_MEM_COPY(actUeSts->activeUeStatus[actUeSts->numElem].entryTime,ueCb->ueEntryTime,MAX_SIZE_ACTIVEUESTATUS_ENTRYTIME + 1);
  actUeSts->activeUeStatus[actUeSts->numElem].cRnti =  ueCb->crnti;
  actUeSts->activeUeStatus[actUeSts->numElem].internalUeIdentity = ueCb->enbUeInternalId;
  actUeSts->activeUeStatus[actUeSts->numElem].mmec = ueCb->sTmsi.mmeCode;
  actUeSts->activeUeStatus[actUeSts->numElem].mTmsi = ueCb->sTmsi.mTmsi;
  actUeSts->activeUeStatus[actUeSts->numElem].s1apEnbUeId = ueCb->s1ConCb.enb_ue_s1ap_id;
  actUeSts->activeUeStatus[actUeSts->numElem].s1apMmeUeId =  ueCb->s1ConCb.mme_ue_s1ap_id;
  actUeSts->activeUeStatus[actUeSts->numElem].ueCategory = ueCb->ueEutraCap.ueCategory;
  actUeSts->activeUeStatus[actUeSts->numElem].dlAmbr = ueCb->aggMaxBr.dl/DATA_RATE_IN_MBPS;
  actUeSts->activeUeStatus[actUeSts->numElem].ulAmbr = ueCb->aggMaxBr.ul/DATA_RATE_IN_MBPS;
  actUeSts->activeUeStatus[actUeSts->numElem].activeCiphAlgo = ueCb->secCfg.ciphAlgoSelected;
  actUeSts->activeUeStatus[actUeSts->numElem].numERabs =  ueCb->numERabs;
  actUeSts->activeUeStatus[actUeSts->numElem].index = actUeSts->numElem + 1;
  actUeSts->numElem++; 
}
/*
*
*      Fun:  uehProcUeEventHistoryStatusReq
*
*      Desc:  Process the event history status request
*
*      Ret:   None
*
*      Notes: None
*
*      File:  ueh_li_hdl.c
*
*/ 
void uehProcUeEventHistoryStatusReq(Buffer *mBuf)
{
  MsgLen         cCnt = 0;
  MsgBsmUehUeEventHistoryReq ueEventHisReq;
  MsgUehBsmUeEventHistoryRsp ueEventHisRsp;

  SCpyMsgFix(mBuf, 0, sizeof(MsgBsmUehUeEventHistoryReq), (Data*)&ueEventHisReq, &cCnt);
  clearBytes(&ueEventHisRsp, sizeof(MsgUehBsmUeEventHistoryRsp));

  ueEventHisRsp.cmdRef = ueEventHisReq.cmdRef;
  ueEventHisRsp.numElem = 0;
  uehFillUeEventHistoryRsp(&ueEventHisRsp); 
  sendUehMsgToBsm(EVT_UEH_BSM_UE_EVENT_HISTORY_RSP, sizeof(MsgUehBsmUeEventHistoryRsp), (UINT8*)&ueEventHisRsp);
}

#if 0
TBOOL isFailureEvent(EventType evt) 
{
  TBOOL ret;
  switch(evt)
  {
    case EVENTTYPE_RRCCONNREJ:
    case EVENTTYPE_SECMODEFAIL:
    case EVENTTYPE_INITCONTEXTSETUPFAIL:
    case EVENTTYPE_RRCREESTABREJ:
    case EVENTTYPE_LOCATIONREPORTFAILUREIND:
    case EVENTTYPE_DEACTIVATETRACEFAILUREIND:
    case EVENTTYPE_OUTHOFAIL:
    case EVENTTYPE_INHOFAIL:
    case EVENTTYPE_PATHSWITCHFAIL:
    case EVENTTYPE_RABSETUPFAIL:
    case EVENTTYPE_RABMODFAIL:
    case EVENTTYPE_UECONTEXTMODFAIL:
    case EVENTTYPE_RLFAILURE:
    case EVENTTYPE_UERESTMREXP:
    case EVENTTYPE_SRB1TMREXP:
    case EVENTTYPE_RRCCONNRELL2ACKTMREXP:
    case EVENTTYPE_UECTXRELTMREXP:
    case EVENTTYPE_UECRNTIRETENTMREXP:
    case EVENTTYPE_EGTERRIND:
    case EVENTTYPE_UDHUEINACTVIND:
    case EVENTTYPE_UDHULINACTVIND:
    case EVENTTYPE_S1APHOPREPFAILED:
    case EVENTTYPE_MMERELINDTMREXP:
    case EVENTTYPE_INTCONFTMREXP:
    case EVENTTYPE_S1APLOCALERRIND:
    case EVENTTYPE_RRCLOCALERRIND:
         ret = TRUE;
    break;

    default: 
       ret =  FALSE;
    break;
  }
  RETVALUE(ret);
}

#endif
/*
*
*      Fun:  uehFillUeEventHistoryRsp
*
*      Desc:  Send the event logs to bsm based on requirement
              It checks whether the historyTable, a circular buffer is full or not.
              If TRUE
                It copies all the events from the circular buffer to the Rsp Structure.
*             else
                It copies the events present in buffer to Rsp Structure.
              Ex: total event=100; currIdx=28;
                  then copy will be done in 2 stages
                  1. copy from 28th to 99th index
                  2. copy from 0th  to 27th index
*      Ret:   None
*
*      Notes: None
*
*      File:  ueh_li_hdl.c
*
*/
 

void uehFillUeEventHistoryRsp(MsgUehBsmUeEventHistoryRsp *ueEventHisRsp)
{
  U8              idx       = 0;
  UehUeHisTbl    *ueHisTbl = getUeHistoryTable();

  if(isUeHistoryTableFull(ueHisTbl) == TRUE)
  {
    for(idx=ueHisTbl->ueHisCurrIdx; idx < MAX_ROWS_IN_UEEVENTHISTORY_TABLE; idx++)
    {
      copyBytes(&(ueHisTbl->ueHis[idx]),sizeof(UeEventHistory),  &ueEventHisRsp->ueEventHistory[ueEventHisRsp->numElem]);
      ueEventHisRsp->ueEventHistory[ueEventHisRsp->numElem].index = ueEventHisRsp->numElem+1; 
      ueEventHisRsp->numElem++;

    }
  }
  for(idx=0;idx< ueHisTbl->ueHisCurrIdx;idx++)
  {
    copyBytes(&(ueHisTbl->ueHis[idx]),sizeof(UeEventHistory),  &ueEventHisRsp->ueEventHistory[ueEventHisRsp->numElem]);
    ueEventHisRsp->ueEventHistory[ueEventHisRsp->numElem].index = ueEventHisRsp->numElem+1; 
    ueEventHisRsp->numElem++;
  }
}

/* The func isUeHistoryTableFull( ):Checks whether the HistoryTable is full or not **/

TBOOL isUeHistoryTableFull(UehUeHisTbl *ueHisTbl)
{
  if( ueHisTbl->totalEvtInUeEvtHisTbl EQ MAX_ROWS_IN_UEEVENTHISTORY_TABLE)
   {
    return TRUE;
   }
  else
  {
    return FALSE;
  }
}
    
/*
 *
 *      Fun:   uehProcUeHistoryCfgReq
 *
 *      Desc:  Process the UE History Configuartion Req
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */

void uehProcUeHistoryCfgReq(Buffer *mBuf)
{
  MsgLen        cCnt    = 0;

  MsgBsmUehUeEventHistoryLogConfigReq  ueEventHistoryLogConfigReq;
  MsgUehBsmUeEventHistoryLogConfigSucc ueEventHistoryLogConfigSucc;
  MsgUehBsmUeEventHistoryLogConfigFail ueEventHistoryLogConfigFail;

  SCpyMsgFix(mBuf, 0, sizeof(MsgBsmUehUeEventHistoryLogConfigReq), (Data*)&ueEventHistoryLogConfigReq, &cCnt);
  if ((ueEventHistoryLogConfigReq.ueEventHistoryLogConfig.ueEventMeasures > 16 ) || (0 == ueEventHistoryLogConfigReq.ueEventHistoryLogConfig.ueEventMeasures))
  {
    clearBytes(&ueEventHistoryLogConfigFail, sizeof(MsgUehBsmUeEventHistoryLogConfigFail));
    ueEventHistoryLogConfigFail = ueEventHistoryLogConfigReq;
    sendUehMsgToBsm(EVT_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_FAIL,sizeof(MsgUehBsmUeEventHistoryLogConfigFail), (UINT8*)&ueEventHistoryLogConfigFail);
    return;
  }
  copyBytes(&ueEventHistoryLogConfigReq.ueEventHistoryLogConfig,sizeof(UeEventHistoryLogConfig),&uehCb.ueEventHistoryLogConfig);
  clearBytes(&ueEventHistoryLogConfigSucc, sizeof(MsgUehBsmUeEventHistoryLogConfigSucc));
  ueEventHistoryLogConfigSucc = ueEventHistoryLogConfigReq;
  sendUehMsgToBsm(EVT_UEH_BSM_UEEVENTHISTORYLOG_CONFIG_SUCC,sizeof(MsgUehBsmUeEventHistoryLogConfigSucc), (UINT8*)&ueEventHistoryLogConfigSucc);
}

/*
 *
 *      Fun:   uehProcModStartReq
 *
 *      Desc:  Process the Mod Start Req
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
void uehProcModStateReq(Buffer *mBuf)
{ 
  UehCellCb     *cellCb = uehGetCellCb();
  MsgLen        cCnt = 0;
  MsgBsmUehModStateReq  msgBsmUehModStateReq;
  MsgUehBsmModStateRsp  modStateRsp;

  TRC2(uehProcModStartReq);
  logInfo("%s", "Received Mod State Request from BSM");
  
  SCpyMsgFix(mBuf, 0, sizeof(MsgBsmUehModStateReq), (Data*)&msgBsmUehModStateReq, &cCnt);
  clearBytes(&modStateRsp, sizeof(MsgUehBsmModStateRsp));
  
  modStateRsp.cmdRef  = msgBsmUehModStateReq.cmdRef;
  modStateRsp.tagInfo = msgBsmUehModStateReq.tagInfo;
  if (cellCb != NULLP)
  {
    modStateRsp.numOfUesConnected=cellCb->numUeCfg;
  }
 
  sendUehMsgToBsm(EVT_UEH_BSM_MOD_STATE_RSP, sizeof(MsgUehBsmModStateRsp), (UINT8*)&modStateRsp);
}

PUBLIC S16 uehSndBindCtrlRgToTfuSap(
    U32       action
  )
{

   RgMngmt  *rgMngmt = NULLP;
   RgCntrl  *cntrl=NULLP;
   Pst      pst;
  
   logFunc("%s", "uehSndUnBindRgToTfuSap");
 
   SGetSBuf(0, 0, (Data **)&rgMngmt, sizeof(RgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(rgMngmt, sizeof(RgMngmt));

   cntrl = &(rgMngmt->t.cntrl);

   cntrl->action             =  action;
   cntrl->subAction          =  SM_ZERO_VAL;
   cntrl->s.rgSapCntrl.suId  =  SM_ZERO_VAL;
   cntrl->s.rgSapCntrl.spId  =  SM_ZERO_VAL;


   /* Fill Header */
   rgMngmt->hdr.msgType      = TCNTRL;
   rgMngmt->hdr.entId.ent    = ENTRG;
   rgMngmt->hdr.entId.inst   = SM_INST_ZERO;
   rgMngmt->hdr.elmId.elmnt  = STTFUSAP; 

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "MAC: TFU lower SAP(STTFUSAP) Bind request sent");

   /* Send the request to the RRC */
   SmMiLrgCntrlReq(&pst, rgMngmt);

   RETVALUE(ROK);
} /* end of uehSndUnBindRgToTfuSap */

// RRM Changes
/*
 *
 *      Fun:  uehProcRrmCellCounter 
 *
 *      Desc:  Process the RRM Cell Counters
 *
 *      Ret:   None
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */

void uehProcRrmCellCounter(
    Buffer *mBuf
  )
{
  MsgLen                     cCnt;
  MsgPmhUehRrmStatReq        rrmStatReq;
  MsgUehPmhRrmStatRsp        rrmStatRsp;
  UehCellCb                  *cellCb = uehGetCellCb();
  SCpyMsgFix( mBuf, 0, sizeof(MsgPmhUehRrmStatReq), (Data*)&rrmStatReq , &cCnt );
  clearBytes(&rrmStatRsp, sizeof(MsgUehPmhRrmStatRsp));
   

  logInfo("%s", " Filling RRM STAT RSP");
  rrmStatRsp.transId = rrmStatReq.transId;
  if(NULL NOTEQ cellCb)
  {
    if( TRUE EQ rrmStatReq.resetAllCounters)
    {
      clearBytes(&cellCb->uehRrmCellCounters,sizeof(cellCb->uehRrmCellCounters));
      clearBytes(&uehCb.uehCounters[0],sizeof(uehCb.uehCounters[0]) * (EVENTTYPE_OTHER+1)); 	
    }
    else
    {

      rrmStatRsp.rrmCounters.numIncomingHoRejected   = cellCb->uehRrmCellCounters.numIncomingHoRejected;
      rrmStatRsp.rrmCounters.numIncomingHoFailed     = cellCb->uehRrmCellCounters.numIncomingHoFailed;
      rrmStatRsp.rrmCounters.numIncomingHoSuccessful = cellCb->uehRrmCellCounters.numIncomingHoSuccessful;
      rrmStatRsp.rrmCounters.numRlFailures           = cellCb->uehRrmCellCounters.numRlFailures;
      rrmStatRsp.rrmCounters.nS1SetupReq             = cellCb->uehRrmCellCounters.numS1apSetupReq; /* TODO: Need to decide from where we will get this counter */
      rrmStatRsp.rrmCounters.nS1SetupRsp             = cellCb->uehRrmCellCounters.numS1apSetupRsp; 
      rrmStatRsp.rrmCounters.numS1apRejected         = cellCb->uehRrmCellCounters.numS1apRejected;
      rrmStatRsp.rrmCounters.nS1ResetTx              = cellCb->uehRrmCellCounters.numS1apResetTx;
      rrmStatRsp.rrmCounters.nS1ResetRx              = cellCb->uehRrmCellCounters.numS1apResetRx;
      rrmStatRsp.rrmCounters.nRrcConnReq             = uehCb.uehCounters[EVENTTYPE_RRCCONNREQ];
      rrmStatRsp.rrmCounters.nRrcConnSetup           = uehCb.uehCounters[EVENTTYPE_RRCCONNSETUP];
      rrmStatRsp.rrmCounters.nRrcConnSetupCmpl       = uehCb.uehCounters[EVENTTYPE_RRCCONNSETUPCOMPL];
      rrmStatRsp.rrmCounters.nRrcConnRej             = uehCb.uehCounters[EVENTTYPE_RRCCONNREJ];
      rrmStatRsp.rrmCounters.nRrcConnRel             = uehCb.uehCounters[EVENTTYPE_RRCCONNRELEASE];
      rrmStatRsp.rrmCounters.nDlInfoTrf              = uehCb.uehCounters[EVENTTYPE_DLINFTRANSFER];
      rrmStatRsp.rrmCounters.nUlInfoTrf              = uehCb.uehCounters[EVENTTYPE_ULINFTRANSFER];
      rrmStatRsp.rrmCounters.nUlNasTrfMsg            = uehCb.uehCounters[EVENTTYPE_ULNASTRANSPORTMSG];
      rrmStatRsp.rrmCounters.nDlNasTrfMsg            = uehCb.uehCounters[EVENTTYPE_DLNASTRANSPORTMSG];
      rrmStatRsp.rrmCounters.nInitCtxtSetReq         = uehCb.uehCounters[EVENTTYPE_INITCONTEXTSETUPREQ];
      rrmStatRsp.rrmCounters.nInitCtxtSetRsp         = uehCb.uehCounters[EVENTTYPE_INITCONTEXTSETUPRESP];
      rrmStatRsp.rrmCounters.nInitCtxtSetFail        = uehCb.uehCounters[EVENTTYPE_INITCONTEXTSETUPFAIL];
      rrmStatRsp.rrmCounters.nRrcReconfReq           = uehCb.uehCounters[EVENTTYPE_RRCCONNRECONFIG];
      rrmStatRsp.rrmCounters.nRrcReconfCmpl          = uehCb.uehCounters[EVENTTYPE_RRCCONNRECONFCOMPL];
      rrmStatRsp.rrmCounters.nRrcReestReq            = uehCb.uehCounters[EVENTTYPE_RRCREESTABREQ];
      rrmStatRsp.rrmCounters.nUeCapEnq               = uehCb.uehCounters[EVENTTYPE_UECAPABILITYENQ];
      rrmStatRsp.rrmCounters.nUeCapInfo              = uehCb.uehCounters[EVENTTYPE_UECAPABILITYINFO];
      rrmStatRsp.rrmCounters.nUeCapInfoInd           = uehCb.uehCounters[EVENTTYPE_UECAPABILITYINFOIND];
      rrmStatRsp.rrmCounters.nSecModeCmd             = uehCb.uehCounters[EVENTTYPE_SECMODECOMMAND];
      rrmStatRsp.rrmCounters.nSecModeCmpl            = uehCb.uehCounters[EVENTTYPE_SECMODECOMPL];
      rrmStatRsp.rrmCounters.nSecModeFail            = uehCb.uehCounters[EVENTTYPE_SECMODEFAIL];
//    rrmStatRsp.rrmCounters.nS1SetupFail          = 0;/* TODO:  Need to decide from where we will get this counter */
      rrmStatRsp.rrmCounters.nUeCtxtRelCmd           = uehCb.uehCounters[EVENTTYPE_UECONTEXTRELCMD];
      rrmStatRsp.rrmCounters.nUeCtxtRelCmpl          = uehCb.uehCounters[EVENTTYPE_UECONTEXTRELCOM];
      rrmStatRsp.rrmCounters.nUeCtxtRelReq           = uehCb.uehCounters[EVENTTYPE_UECONTEXTRELREQ];
      rrmStatRsp.rrmCounters.nPaging                 = uehCb.uehCounters[EVENTTYPE_PAGING];

    }
  }
  sendUehMsgToPmh(EVT_UEH_PMH_RRM_CELL_STAT_RSP, sizeof(MsgUehPmhRrmStatRsp), (UINT8*)&rrmStatRsp);
}

TBOOL uehProcTgtToSrcTprntContDecCfm(
  UehUeCb   *ueCb,
  void      *sdu
  )
{
  uehSndUeCapInfoDecodeReq(ueCb);   
  RETVALUE(ROK);
}


/**
  *
  *       Fun:   uehProcSrcToTgtTprntContEncCfm
  *
  *       Desc:  This function will handle confirmation message from S1AP for encode
  *              Source To Target Transparent Container.
  *
  *       Ret:   None
  *
  *       Notes: None
  *
  *       File:  ueh_li_hdl.c
  *
  */
TBOOL uehProcSrcToTgtTprntContEncCfm(
    UehUeCb           *ueCb,
    SztEncDecEvnt     *encDecCfm
  )
{
  HandoverRequired  handoverRequired;
  S1Cause           s1Cause;
  SztDatEvntReq     datEvt;

  clearBytes( &handoverRequired, sizeof(HandoverRequired) );
  clearBytes( &s1Cause, sizeof(S1Cause) );
  UEH_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));

  logFunc("%s", "uehProcSrcEnbToTgtEnbTrnspContEncCfm");

  s1Cause.causeType = CauseType_RadioNw;
  s1Cause.causeVal  = SztCauseRadioNws1_intra_system_handover_triggeredEnum;

  uehFillHandoverRequired( &handoverRequired, ueCb, &s1Cause, encDecCfm->u.pdu );

  if ( FALSE EQ s1apWrapHandoverRequiredMsgPdu( &datEvt.pdu, &handoverRequired ))
  {
    UEH_PROCEDURE_FAILED("S1AP wrapping for Handover required PDU", UE, ueCb->crnti, "Memory allocation");
    RETVALUE(RFAILED);
  }
  datEvt.spConnId = ueCb->s1ConCb.spConnId;

  if(ROK NOTEQ uehSndMsgOut (ENTSZ, EVENTTYPE_OUTHOTRIGGERED, ueCb, TRANSID_NONE,&datEvt))
  { 
    RETVALUE(RFAILED);
  }

  uehSetState(ueCb, UE_STATE_HANDOVER_ONGOING);
  RETVALUE(ROK);
}



/**
  *
  *       Fun:   uehProcHovrPrepInfoEncCfm
  *
  *       Desc:  This function will handle confirmation message from RRC for encode
  *              Handover Preperation Information message.
  *
  *       Ret:   None
  *
  *       Notes: None
  *
  *       File:  ueh_li_hdl.c
  *
  */
TBOOL uehProcHovrPrepInfoEncCfm(
    UehUeCb          *ueCb,
    NhuEncCfmSdus    *nhuEncCfmSdus
  )
{
  SztEncDecEvnt                     encDecEvnt;
  SrcToTargetTransparentContainer   srcToTarget;

  logFunc("%s", "uehProcHovrPrepInfoEncCfm");
  logInfo("RRC Container of Handover Prep Info Encode Confirm ueId:%d MsgCategory:%d Status:%d",
           nhuEncCfmSdus->hdr.ueId, nhuEncCfmSdus->sdu.msgCategory, nhuEncCfmSdus->sdu.status);

  UEH_SET_ZERO(&encDecEvnt, sizeof(SztEncDecEvnt));
  clearBytes( &srcToTarget, sizeof(SrcEnbToTargetEnbTransparentCont) );


  if (NHU_ENCODE_SUCCESS != nhuEncCfmSdus->sdu.status)
  {
    /*TODO: Actions for failure Cases needs to be added */
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUENCODECFM, ENTNH, nhuEncCfmSdus->sdu.status, UE, ueCb->crnti);
    RETVALUE(ROK);
  }

  UEH_BIT_TO_BYTE_CONVSN(ueCb->ueHovrInfo.rrcCtnrHovrPrepInfo.msgLen, nhuEncCfmSdus->sdu.msgLen );
  SRemPreMsgMult(ueCb->ueHovrInfo.rrcCtnrHovrPrepInfo.handoverPrepInfoMsgPdu,
                 ueCb->ueHovrInfo.rrcCtnrHovrPrepInfo.msgLen,
                 nhuEncCfmSdus->sdu.mBuf);
  ueCb->ueHovrInfo.isRrcCtnrHovrPrepInfo = TRUE;

  /* TODO:
   * Fill and send Src ENB to Target ENB Transparent Container to S1AP for encoding
   */
  ueCb->ueHovrInfo.handoverType = HandoverType_IntraLte;
  ueCb->ueHovrInfo.targetIdType = TargetType_TargetEnbId;
  uehFillSrcToTargetTransparentContainer( &srcToTarget, &(ueCb->ueHovrInfo) );

  encDecEvnt.transId = 0; /*TODO */
  encDecEvnt.type.val = SZT_REQ_TYPE_ENCODE;

  s1apWrapSrcToTargetTransparentContainer(encDecEvnt.u.pdu, &srcToTarget, &(ueCb->ueHovrInfo));

  UehLiSztEncDecReq(&uehCb.sztSap[0]->pst,uehCb.sztSap[0]->spId, &encDecEvnt);

  RETVALUE(ROK);
}

/**
  *
  *       Fun:   uehSndHoverPrepInfoEncodeReq
  *
  *       Desc:  This function will encode the Handover Preperation Information message and send this
  *              encoded message to RRC to get RRC Container.
  *
  *       Ret:   None
  *
  *       Notes: None
  *
  *       File:  ueh_li_hdl.c
  *
  */
static S16 uehSndHoverPrepInfoEncodeReq(
    UehUeCb       *ueCb
  )
{
  S16                   ret = RFAILED;
  Pst                   *pst = NULLP;
  NhuEncReqSdus         *nhuEncReqSdus = NULLP;
  NhuHovrPrepInfo       *hoverPrepInfo;

  NhuUE_CapRAT_ContLst                  *ue_RadioAccessCapInfo = NULLP;
  NhuAS_Config                          *asConfig   = NULLP;
  NhuRRM_Config                         *rrmConfig  = NULLP;
  NhuAS_Ctxt                            *asCtxt     = NULLP;
  NhuHovrPrepInfo_r8_IEsnonCriticalExtn *nonCriticalExtn = NULLP;
  NhuTimingInfo                         *timingInfo = NULLP;
  UehCellCb                             *cellCb = NULLP;

  logFunc("%s", "uehSndHoverPrepInfoEncodeReq");

  cellCb = uehGetCellCb();
  if(cellCb == NULLP)
  {
    /*TODO: update event received and source entity once handover implemented fully*/
    UEH_ERROR_NO_CB_LOCATED(CELL, NOT_APPLICABLE, NOT_APPLICABLE , NOT_APPLICABLE);
    RETVALUE(RFAILED);
  }
  timingInfo->sfn = cellCb->sfn;

  UEH_ALLOCEVNT_OR_RET(&nhuEncReqSdus, sizeof(NhuEncReqSdus));

  /* Fill RRC Container -> Handover Preparation Info */   /* TODO ------ change to E_UTRAN_HO_PREPINFO_ENCODE */
  uehFillNhuEvntHdr(&(nhuEncReqSdus->hdr), ueCb->cellId, ueCb->crnti, E_UTRAN_UE_RADACCCAPINFO_ENCODE);

  nhuEncReqSdus->sdu.msgCategory = NHU_MSG_HOPREPINFO;
  hoverPrepInfo = &(nhuEncReqSdus->sdu.u.hovrPrepInfo);
  UEH_FILL_TKN_UINT(hoverPrepInfo->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(hoverPrepInfo->criticalExtns.choice, CRITICALEXTNS_C23);
  UEH_FILL_TKN_UINT(hoverPrepInfo->criticalExtns.val.c1.choice, C23_HANDOVERPREPINFO_R8);
  UEH_FILL_TKN_UINT(hoverPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8.pres, PRSNT_NODEF);

  /* Field 1: Fill UE Radio Access Capability */
  ue_RadioAccessCapInfo = &(hoverPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8.ue_RadioAccessCapInfo);
  ret = uehFillUeRadioAccCapContPdu((Ptr)nhuEncReqSdus, ue_RadioAccessCapInfo, ueCb );
  if (ret != ROK)
  {
    UEH_ERROR_MSG_FILLING_FAILED("UE Capability RAT container PDU", UE, ueCb->crnti);
    UEH_FREEEVNT(nhuEncReqSdus);
    RETVALUE(RFAILED);
  }

  /* Field 2: Fill AS-Config */
  asConfig = &(hoverPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8.as_Config);
  UEH_FILL_TKN_UINT(asConfig->pres, PRSNT_NODEF);

  /* TODO Field 2.a NhuMeasConfig */
  logInfo("%s", "NhuMeasConfig Not filled ");
  UEH_FILL_TKN_UINT(asConfig->sourceMeasConfig.pres, NOTPRSNT); /* NOTPRSNT: not present as of now */

  /* Field 2.b SourceRadioResourceConfig  */
  /* Field 2.b.1 srb_ToAddModLst */
  UEH_FILL_TKN_UINT(asConfig->sourceRadioResourceConfig.pres, PRSNT_NODEF);
  ret = uehFillNhuSRBAddModLst((Ptr)nhuEncReqSdus, &asConfig->sourceRadioResourceConfig, UEH_PDCP_ID_SRB1);
  if (ret != ROK)
  {
    UEH_ERROR_MSG_FILLING_FAILED("SRB-AddModList (UEH_PDCP_ID_SRB1) Field of As-Config", UE, ueCb->crnti);
    UEH_FREEEVNT(nhuEncReqSdus);
    RETVALUE(RFAILED);
  }
  ret = uehFillNhuSRBAddModLst((Ptr)nhuEncReqSdus, &asConfig->sourceRadioResourceConfig, UEH_PDCP_ID_SRB2);
  if (ret != ROK)
  {
    UEH_ERROR_MSG_FILLING_FAILED("SRB-AddModList (UEH_PDCP_ID_SRB2) Field of As-Config" , UE, ueCb->crnti);
    UEH_FREEEVNT(nhuEncReqSdus);
    RETVALUE(RFAILED);
  }

  /* Field 2.b.2 drb_ToAddModLst */
  ret = uehFillNhuDRBAddModLst((Ptr)nhuEncReqSdus, &asConfig->sourceRadioResourceConfig, ueCb);
  if (ret != ROK)
  {
    UEH_ERROR_MSG_FILLING_FAILED("drb_ToAddModLst Field of As-Config", UE, ueCb->crnti);
    UEH_FREEEVNT(nhuEncReqSdus);
    RETVALUE(RFAILED);
  }

  /* Field 2.b.3 mac_MainConfig */
  uehFillNhuMACMainCfg((Ptr)nhuEncReqSdus, &asConfig->sourceRadioResourceConfig, ueCb);

  /* TODO Field 2.b.4 sps-Config */
  logInfo("%s", "SPS-Config not supported-Hence not filled ");
  UEH_FILL_TKN_UINT( asConfig->sourceRadioResourceConfig.sps_Config.pres, NOTPRSNT );

  /* Field 2.b.5 phy-config-dedicated */
  uehFillRadResPhyCfgDed((Ptr)nhuEncReqSdus, &asConfig->sourceRadioResourceConfig, ueCb);
  
  /* Field 2.c securityAlgorithmConfig */
  UEH_FILL_TKN_UINT(asConfig->sourceSecAlgorithmConfig.pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(asConfig->sourceSecAlgorithmConfig.cipheringAlgorithm, ueCb->secCfg.ciphAlgoSelected);
  UEH_FILL_TKN_UINT(asConfig->sourceSecAlgorithmConfig.integrityProtAlgorithm, ueCb->secCfg.intgAlgoSelected);

  /* TODO Field 2.d UE Identity */
  UEH_FILL_BIT_STR(asConfig->sourceUE_Identity, 2, ueCb->crnti, nhuEncReqSdus);

  /* Field 2.e Master Information Block*/
  ret = uehFillMibMsg((Ptr)nhuEncReqSdus, &(asConfig->sourceMasterInfoBlock), timingInfo);
  if (ret != ROK)
  {
    UEH_ERROR_MSG_FILLING_FAILED("MIB Field of As-Config" , UE, ueCb->crnti);
    UEH_FREEEVNT(nhuEncReqSdus);
    RETVALUE(RFAILED);
  }

  /* Field 2.f SIB 1 */
  ret = uehFillSib1Msg((Ptr)nhuEncReqSdus, &(asConfig->sourceSysInfoBlockTyp1) );
  if (ret != ROK)
  {
    UEH_ERROR_MSG_FILLING_FAILED("SIB1 Field of As-Config" , UE, ueCb->crnti);
    UEH_FREEEVNT(nhuEncReqSdus);
    RETVALUE(RFAILED);
  }

  /* Field 2.g SIB 2 */
  ret = uehFillSib2Msg((Ptr)nhuEncReqSdus, &(asConfig->sourceSysInfoBlockTyp2) );
  if (ret != ROK)
  {
    UEH_ERROR_MSG_FILLING_FAILED("SIB2 Field of As-Config" , UE, ueCb->crnti);
    UEH_FREEEVNT(nhuEncReqSdus);
    RETVALUE(RFAILED);
  }

  /* Field 2.h Antenna Info comman */
  logInfo("%s", "Antenna Info Comman not filled ");
  /* TODO UEH_FILL_TKN_UINT(asConfig->antennaInfoCmn, uehCb.uehCfgCb.cellCfg.numOfAntennas ); */

  /* Field 2.i Source DL carrier Freq */
  UEH_FILL_TKN_UINT(asConfig->sourceDl_CarrierFreq, uehCb.uehCfgCb.cellCfg.dlEarfcn );

  /* Field 3: Fill RRM-Config - Omitted ( As per design document ) */
  rrmConfig = &(hoverPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8.rrm_Config);
  UEH_FILL_TKN_UINT( rrmConfig->pres, NOTPRSNT );

  /* Field 4: Fill As-Context */
  asCtxt = &(hoverPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8.as_Ctxt);
  UEH_FILL_TKN_UINT( asCtxt->pres, NOTPRSNT );

  /* Field 4: Fill Non- Critical Extensions */
  nonCriticalExtn = &(hoverPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8.nonCriticalExtn);
  UEH_FILL_TKN_UINT( nonCriticalExtn->pres, NOTPRSNT );

  pst = &uehCb.nhuSap[0]->pst;
  pst->event = EVTNHUENCODEREQ;

  logInfo("HO PrepInfo Encode Request sent for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
  if(ROK != UehLiNhuEncodeReq(pst, uehCb.nhuSap[0]->spId, nhuEncReqSdus))
  {
    UEH_ERROR_SSI_EVENT_SEND_FAILED( pst, EVENTTYPE_OTHER, UE, ueCb->crnti);
    RETVALUE(RFAILED);
  }

  RETVALUE(ROK);
}

/**
 * @param   TODO
 * @section This function to be called when S1AP decides to start S1Ap based handover
 * Trigger : Not known yet
 * */
TBOOL uehTriggerToSendS1apHandoverRequired(
    UehUeCb        *ueCb
  )
{
  logFunc("%s", "uehTriggerToSendS1apHandoverRequired");

  /* Send Handover Prep Info Message to RRC for encoding */
  if(ROK != uehSndHoverPrepInfoEncodeReq(ueCb))
  {
    RETVALUE(RFAILED);
  }

  RETVALUE(ROK);

}

/**
  *
  *       Fun:   uehProcHandoverCmd
  *
  *       Desc:  This function will handle handover Command received from MME.
  *
  *       Ret:   None
  *
  *       Notes: None
  *
  *       File:  ueh_li_hdl.c
  *
  */
PUBLIC S16 uehProcHandoverCmd(
    UehUeCb     *ueCb,
    S1apPdu     *pdu
  )
{
  HandoverCmd    handoverCmd;
 // UehS1ConCb    *s1ConCb = ueCb->s1ConCb;

  logFunc("%s", "uehProcHandoverCmd");
  logInfo("Handover Command Received in Handover On-Going state for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  /* Decode the received Pdu */
  s1apUnWrapHandoverCommandMsgPdu( pdu, &handoverCmd );
  if (RFAILED == uehValidateEnbAndMmeUeS1apIds(handoverCmd.mmeUeS1apId,handoverCmd.enbUeS1apId,ueCb->crnti))
  {
    RETVALUE(RFAILED);
  }

  /* Decode Target to Source Transparent Container */
  /* Send message to RRC to Decode TargetToSourceTransparentContainter */

  /* TODO : Where should the information be stored - In s1ap context ? */
//  s1ConCb->mme_ue_s1ap_id = handoverCmd.mmeUeS1apId;
//  s1ConCb->enb_ue_s1ap_id = handoverCmd.enbUeS1apId;

  //uehStopTmr((PTR)ueCb, UEH_TMR_S1AP_TS1_RELOC_PREP);

 // uehSetState(ueCb, UE_STATE_CONNECTED);

  RETVALUE(ROK);
}

/**
 * @param
 * @param
 * @details This function is invoked when HandoverPreparationFailed from MME is received.
 **/
PUBLIC S16 uehProcHandoverPrepFailed(
    UehUeCb         *ueCb,
    S1apPdu         *pdu
  )
{
  HandoverPrepFailed    handoverPrepFailed;
  //UehS1ConCb           *s1ConCb = ueCb->s1ConCb;

  logFunc("%s", "uehProcHandoverPrepFailed");
  logInfo("Handover Preparation Failed Received in Handover On-Going state for UeId:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  /* Decode the received Pdu */
  s1apUnWrapHandoverPrepFailedMsgPdu( pdu, &handoverPrepFailed );
  if (RFAILED == uehValidateEnbAndMmeUeS1apIds(handoverPrepFailed.mmeUeS1apId,handoverPrepFailed.enbUeS1apId,ueCb->crnti))
  {
    RETVALUE(RFAILED);
  }

 // uehStopTmr((PTR)ueCb, UEH_TMR_S1AP_TS1_RELOC_PREP);

 // uehSetState(ueCb, UE_STATE_CONNECTED);

  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehProcUdhUlInactivityInd
 *
 *      Desc:  UDH UL Inactivity Indication handling
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcUdhUlInactivityInd(
    UehUeCb  *ueCb,
    void     *sdu
  )
{
  /* 1. Change rlStatus to UNKNOWN
   * 2. Get the count values for the DRBs of the UEs from PDCP.
   * */
  
  logFunc("%s", "uehProcUdhUlInactivityInd");

  

  if (ROK != uehSndUeCapEnqiry(ueCb))
  {
    UEH_ERROR_SSI_EVENT_SEND_FAILED( (&uehCb.nhuSap[0]->pst), EVENTTYPE_UECAPABILITYENQ, UE, ueCb->crnti);
    return RFAILED;
  }

  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehFillCounterCheckMsg
 *
 *      Desc:  Fill Counter Check Message
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehFillCounterCheckMsg(
    PTR                  *evnt,
    NhuCounterCheck      *counterCheck,
    NhuDRBCountInfoLst   *nhuDRBCountInfoLst
  )
{
  U8                      idx = 0;
  NhuDRB_CountMSB_InfoLst *drb_CountMSB_InfoLst = NULLP;

  UEH_FILL_TKN_UINT(counterCheck->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(counterCheck->rrc_TrnsxnId, UEH_VAL_ONE);
  UEH_FILL_TKN_UINT(counterCheck->criticalExtns.choice, CRITICALEXTNS_C7);
  UEH_FILL_TKN_UINT(counterCheck->criticalExtns.val.c1.choice, C7_COUNTERCHECK_R8);
  UEH_FILL_TKN_UINT(counterCheck->criticalExtns.val.c1.val.counterCheck_r8.pres, PRSNT_NODEF);

  drb_CountMSB_InfoLst = &(counterCheck->criticalExtns.val.c1.val.counterCheck_r8.drb_CountMSB_InfoLst);

  UEH_FILL_TKN_UINT(drb_CountMSB_InfoLst->noComp, nhuDRBCountInfoLst->numDrbs);
  UEH_GET_MEM(evnt, (drb_CountMSB_InfoLst->noComp.val) * (sizeof(NhuDRB_CountMSB_Info)), &(drb_CountMSB_InfoLst->member));

  for(idx = 0; idx < nhuDRBCountInfoLst->numDrbs; idx++)
  {
    UEH_FILL_TKN_UINT(drb_CountMSB_InfoLst->member[idx].pres, PRSNT_NODEF);
    UEH_FILL_TKN_UINT(drb_CountMSB_InfoLst->member[idx].drb_Identity, 3);
    UEH_FILL_TKN_UINT(drb_CountMSB_InfoLst->member[idx].countMSB_Ulnk, 100);
    UEH_FILL_TKN_UINT(drb_CountMSB_InfoLst->member[idx].countMSB_Dlnk, 200);


//    UEH_FILL_TKN_UINT(drb_CountMSB_InfoLst->member[idx].drb_Identity, nhuDRBCountInfoLst->member[idx].drbId);
//    UEH_FILL_TKN_UINT(drb_CountMSB_InfoLst->member[idx].countMSB_Ulnk, nhuDRBCountInfoLst->member[idx].ulCount);
//    UEH_FILL_TKN_UINT(drb_CountMSB_InfoLst->member[idx].countMSB_Dlnk, nhuDRBCountInfoLst->member[idx].dlCount);
  }
  RETVALUE(ROK);
}

/*
 *
 *      Fun:   uehProcRrcCountCfm
 *
 *      Desc:  Upon receiving Count Cfm from RRC, fill and send Counter Check Message to UE.
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcRrcCountCfm(
    UehUeCb              *ueCb,
    NhuDRBCountInfoLst   *nhuDRBCountInfoLst
  )
{

  logFunc("%s", "uehProcRrcCountCfm");
#ifdef COUNTER_CHECK

  /* Verify CountReq Message from RRC */
  if (nhuDRBCountInfoLst == NULLP)
  {
    UEH_ERROR_NULL_POINTER_IDENTIFIED();
   //  RETVALUE(RFAILED);
  }
  if ( nhuDRBCountInfoLst->status == FALSE )
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUCOUNTCFM, ENTNH, nhuDRBCountInfoLst->status, UE, ueCb->crnti);
  //  RETVALUE(RFAILED);
  }
  if( nhuDRBCountInfoLst->numDrbs == 0 )
  {
    UEH_ERROR_RECEIVED_FAILURE_EVENT( EVTNHUCOUNTCFM, ENTNH, nhuDRBCountInfoLst->numDrbs, UE, ueCb->crnti);
//    RETVALUE(RFAILED);
  }

  /* Allocate memory for event structure,
   * Fill HDR Info
   * Build Counter Check Message */
  UEH_ALLOCEVNT_OR_RET(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));

  uehFillNhuEvntHdr( &(nhuDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_COUNTERCHECK);

  nhuDatReqEvntSdu->sdu.ueMsg.isMsgDed = TRUE;
  nhuDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = UEH_PDCP_ID_SRB1;

  dlMsg = &(nhuDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
  UEH_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(dlMsg->message.choice, DL_DCCH_MSGTYP_C4);
  UEH_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C4_COUNTERCHECK);

  counterCheck = &(nhuDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg.message.val.c1.val.counterCheck);
  uehFillCounterCheckMsg((Ptr)nhuDatReqEvntSdu, counterCheck, nhuDRBCountInfoLst);

  /* Send the counter check msg to UE using NhuDatRsp Primitive*/
  if (RFAILED == uehSndMsgOut (ENTNH, EVENTTYPE_RRCCOUNTERCHECKREQ, ueCb, TRANSID_NONE, nhuDatReqEvntSdu))
  {
    RETVALUE(RFAILED);
  }
  logInfo("Counter Check Message sent for UE:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
#endif
  RETVALUE(ROK);
} /* uehProcRrcCountCfm */


/*
 *
 *      Fun:   uehProcRrcCounterChkRspHndlr
 *
 *      Desc:  Counter Check Response Handling received from UE
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ueh_li_hdl.c
 *
 */
PUBLIC S16 uehProcRrcCounterChkRsp(
    UehUeCb     *ueCb,
    void        *sdu
  )
{

  logInfo("Counter Check Response received for CRNTI:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);

  RETVALUE(ROK);
} /* uehProcRrcCounterChkRsp */


/**
 * @details UE_TMR_UE_RESPONSE, guards against the delayed responses from UE
 * When we send counter check message to UE, we wait for response from UE,
 * Stop: After receiving confirm message from this UE, we stop this timer.
 * Upon timer Expiry:
 * 1. Send UE context release request
 * 2. Invoke UE cancel
 * 3. Clear UE context
 **/
PUBLIC S16 uehProcUeRspTmrExpiryForRlf(
    UehUeCb     *ueCb
  )
{
  S1Cause        cause;
  UehCellCb      *cellCb = uehGetCellCb();
  
  logFunc("%s", "uehProcUeRspTmrExpiryForRlf");
 
   /* Updating the Rab Release Cause */
  cause.causeType = CauseType_RadioNw;
  cause.causeVal = SztCauseRadioNwradio_connection_with_ue_lostEnum;
  uehUeProcStart(ueCb,UEH_UE_CXT_REL_DUE_TO_RL_FAIL);

  uehSndS1UeCntxtRlsReq( ueCb, &cause );
  cellCb->uehRrmCellCounters.numRlFailures++;
  RETVALUE(ROK);
}
PUBLIC S16 uehProcInternalReconfTmrExpiry(
    UehUeCb    *ueCb
    )
{
  logInfo("uehProcInternalReconfTmrExpiry:Sending Initial Context setup failure UEID:%d eNbs1apId:%d mmeS1apId:%d", ueCb->crnti, ueCb->s1ConCb.enb_ue_s1ap_id, ueCb->s1ConCb.mme_ue_s1ap_id);
 
  uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_Prototcol, SztCauseProtunspecifiedEnum);
  RETVALUE(ROK);
}

PUBLIC S16 uehSndMsgToRrc ( 
   IN  EventType    uniqueEventType,
   IO  UehUeCb     *ueCb,
   void            *msg)
{
  
  S16          ret      = 0;
  ObjType      objType  = UE;
  SINT32          inst     = 0;
  U16          ueIdx    = 0;
  UehCellCb   *cellCb   = uehGetCellCb();

   switch (uniqueEventType)
  {
     /****Send Messages to RRC****/
     case  EVENTTYPE_RRCBNDREQ :
       objType=SAP; 
       inst=uehCb.nhuSap[0]->suId;
       uehStartTmr (UEH_TMR_NHU_SAP_BND,&(( UehLiSapCb*)uehCb.nhuSap[0])->timer,(PTR)uehCb.nhuSap[0],0);
       ret = UehLiNhuBndReq(&uehCb.nhuSap[0]->pst, uehCb.nhuSap[0]->suId, uehCb.nhuSap[0]->spId);
     break;

     case  EVENTTYPE_RRCCELLCFG :
       objType=CELL;
       inst=uehGetCellCb()->cellId;
       ret = UehLiNhuCellCfgReq(&uehCb.nhuSap[0]->pst, uehCb.nhuSap[0]->spId, (NhuCellCfgReqSdus *)msg);
     break;

     case  EVENTTYPE_RRCCONNSETUP :
        objType=UE; 
        inst=ueCb->crnti;
       uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].sentEvt = EVENTTYPE_RRCCONNSETUP;
       uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].expEvt = EVENTTYPE_RRCCONNSETUPCOMPL;
       uehStartTmr (UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
       ret = uehHdlNhuDatRsp ((NhuDatRspSdus *)msg);
       break;

     case  EVENTTYPE_RRCCONNREJ :
       objType=UE; 
       inst=ueCb->crnti;
       ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
       //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueFailureEvt = EVENTTYPE_RRCCONNREQ;
       if (ueCb->isUeCbAllocated)
       {
         uehStartTmr (UE_TMR_CRNTI_RETENTION,&((UehUeCb *)ueCb)->crntiRtnTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
       }
       ret = uehHdlNhuDatRsp ((NhuDatRspSdus *)msg);
       cellCb->uehRrmCellCounters.numRrcConRejected++;
     break;

     case EVENTTYPE_RRCREESTABREJ:
       objType=UE;
       if (ueCb)
       {
         inst=ueCb->crnti;
       }
       ret = uehHdlNhuDatRsp ((NhuDatRspSdus *)msg);
     break;

     case  EVENTTYPE_RRCUECAPDECODEREQ :
       objType=UE; 
       inst=ueCb->crnti;
       ret = UehLiNhuDecodeReq(&uehCb.nhuSap[0]->pst, uehCb.nhuSap[0]->spId, (NhuDecodeSduReq *)msg); 
     break;

     case  EVENTTYPE_RRCCONNRECONFIG : 

     objType=UE;
     inst=ueCb->crnti;
/* Started the timer in normal attach procedure.  In cae of reconf intended for pucch power reset timer is not started. */
/* Start UE Reponse timer */

     if (ueCb->ueProcedure != UEH_PUCCH_PWR_REC_ONGOING)
     {
       uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].sentEvt = EVENTTYPE_RRCCONNRECONFIG;
       uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].expEvt = EVENTTYPE_RRCCONNRECONFCOMPL;
       uehStartTmr (UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
     } 
     ret = uehSndUENhuDatReq((NhuDatReqSdus *)msg);
     if (ret != ROK)
     {
       logError("%s","RRC RECONFIGURATION has not been send");
     }
     break;

   case  EVENTTYPE_UECAPABILITYENQ : 
       
       objType=UE; 
       inst=ueCb->crnti;
       uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].sentEvt = EVENTTYPE_UECAPABILITYENQ;
       uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].expEvt = EVENTTYPE_UECAPABILITYINFO;
       uehStartTmr (UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
       ret = uehSndUENhuDatReq((NhuDatReqSdus *)msg);
     break;

   case  EVENTTYPE_SECMODECOMMAND :
       objType=UE; 
       inst=ueCb->crnti;
       /* uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].sentEvt = EVENTTYPE_SECMODECOMMAND;
       uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].expEvt = EVENTTYPE_SECMODECOMPL;
       uehStartTmr (UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti); */
       ret = uehSndUENhuDatReq((NhuDatReqSdus *)msg);
       break;

     case  EVENTTYPE_RRCCONNRELEASE :

       objType=UE; inst=ueCb->crnti;
       uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].sentEvt = EVENTTYPE_RRCCONNRELEASE;
       uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].expEvt = EVENTTYPE_OTHER;

       uehStartTmr (UE_TMR_RRCCONNRELL2ACK,&((UehUeCb *)ueCb)->ueRrcConRelL2Ack,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
       ret = uehSndUENhuDatReq((NhuDatReqSdus *)msg);
     break;

     case  EVENTTYPE_RRCCOUNTERCHECKREQ :
       objType=UE; 
       inst=ueCb->crnti;
       uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].sentEvt = EVENTTYPE_RRCCOUNTERCHECKREQ;
      /* uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].sentEvt = EVENTTYPE_RRCCOUNTERCHECKREQ;

       uehCb.uehTimerDescTable[UE_TMR_UE_RESPONSE].expEvt = EVENTTYPE_RRCCOUNTERCHECKRES;
       uehStartTmr (UE_TMR_UE_RESPONSE,&((UehUeCb *)ueCb)->ueRspTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti); */
       ret = uehSndUENhuDatReq((NhuDatReqSdus *)msg);
       break;

     case  EVENTTYPE_DLINFTRANSFER:
       objType=UE;
       inst=ueCb->crnti;
       ret = uehSndUENhuDatReq((NhuDatReqSdus *)msg);
     break;

     default :
       UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("EventType", uniqueEventType, "Messages to RRC");
       ret = RFAILED;
    break;
 }
    if(ret NOTEQ ROK)
      UEH_ERROR_SSI_EVENT_SEND_FAILED((&(uehCb.nhuSap[0]->pst)), uehCb.nhuSap[0]->pst.event, objType, inst);
    return ret;
}

PUBLIC S16 uehSndMsgToS1(
   IN  EventType     uniqueEventType,
   IO   UehUeCb           *ueCb,
   void              *msg)
{
   S16 ret = 0;
   ObjType objType=UE;
   SINT32 inst = 0;
   U16 ueIdx = 0;
   
   if (ueCb)
   {
     ueIdx = UEH_GET_UE_IDX(ueCb->crnti);
   }

   
   switch (uniqueEventType)
   {
      /****Send Messages to S1AP****/
     case  EVENTTYPE_S1BNDREQ:
       objType=SAP; 
       inst=uehCb.sztSap[0]->suId;
       uehStartTmr (UEH_TMR_SZT_SAP_BND,&(( UehLiSapCb*)uehCb.sztSap[0])->timer,(PTR)uehCb.sztSap[0],0);
       ret = UehLiSztBndReq(&uehCb.sztSap[0]->pst,uehCb.sztSap[0]->suId, uehCb.sztSap[0]->spId);
     break;

     case  EVENTTYPE_UECONTEXTRELREQ : 
       objType=UE; 
       inst=ueCb->crnti;
       uehStartTmr (UE_TMR_UE_CTX_REL,&(( UehUeCb*)ueCb)->ueCtxRelTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
       uehUeProcStart(ueCb,UEH_UE_REL_REQ_SENT);
       ret = UehLiSztDatReq(&uehCb.sztSap[0]->pst,uehCb.sztSap[0]->spId , (SztDatEvntReq *)msg);
     break;
     case  EVENTTYPE_OUTHOTRIGGERED :
       /* Start Timer TS1-RELOCprep*/
       /* Start a timer guard for MME response - Expected: Handover Command */
       /* ueS1apHandOverReqdRspDur set to UERESPONSETIMER_MS5000 during Init */
       objType=UE; 
       inst=ueCb->crnti;
       uehStartTmr (UEH_TMR_S1AP_TS1_RELOC_PREP,&((UehUeCb *)ueCb)->ueS1apHandOverReqdRsp,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
       ret = UehLiSztDatReq(&uehCb.sztSap[0]->pst,uehCb.sztSap[0]->spId , (SztDatEvntReq *)msg);
     break;

     case  EVENTTYPE_INITCONTEXTSETUPRESP :  
       /* Stop internal config timer */
       objType=UE;
       inst=ueCb->crnti;
       //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus =  uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueProcStatus | INITIAL_CTX_SETUP_SUCC;
       uehStopTmr (UE_TMR_INTERNAL_CONF,&((UehUeCb *)ueCb)->ueInternalConfTmr,(PTR)ueCb, ((UehUeCb *)ueCb)->crnti);
       ret = UehLiSztDatReq(&uehCb.sztSap[0]->pst,uehCb.sztSap[0]->spId , (SztDatEvntReq *)msg);
     break;

     case  EVENTTYPE_UECAPABILITYINFOIND :   
     case  EVENTTYPE_ULNASTRANSPORTMSG :
     case  EVENTTYPE_DLNASNONDELIVERYIND :
     case  EVENTTYPE_TRACEFAILUREIND :
     case  EVENTTYPE_DEACTIVATETRACEFAILUREIND :
     case  EVENTTYPE_RABRELCOMPL :
     case  EVENTTYPE_UECONTEXTMODFAIL :
     case  EVENTTYPE_UECONTEXTMODRESP :
     case  EVENTTYPE_LOCATIONREPORTFAILUREIND :
     case  EVENTTYPE_LOCATIONREPORTRES :
     case  EVENTTYPE_RABMODFAIL : 
     case  EVENTTYPE_RABSETUPREQ:
       objType=UE;
       inst=ueCb->crnti;
       ret = UehLiSztDatReq(&uehCb.sztSap[0]->pst,uehCb.sztSap[0]->spId , (SztDatEvntReq *)msg);
     break;

     case  EVENTTYPE_INITIALUEMSG :
        objType=UE;
        inst=ueCb->crnti;
     /* Start a timer guard for MME responses */
       uehStartTmr (UE_TMR_UE_SRB1,&((UehUeCb *)ueCb)->ueSrb1Tmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
       ret = UehLiSztConReq(&uehCb.sztSap[0]->pst, uehCb.sztSap[0]->spId, (SztConReq *)msg);
     break;

     case  EVENTTYPE_INITCONTEXTSETUPFAIL :    
       objType=UE;
       inst=ueCb->crnti;
       //uehCb.uehUeHisTbl.uehEvtInfo[ueIdx].ueFailureEvt = EVENTTYPE_INITCONTEXTSETUPFAIL;
       /* Start a timer to wait for the Ue context release command message from MME */ /*change by karthik*/
       uehCb.uehTimerDescTable[UEH_TMR_WAIT_MME_REL].sentEvt = EVENTTYPE_INITCONTEXTSETUPFAIL;
       uehCb.uehTimerDescTable[UEH_TMR_WAIT_MME_REL].expEvt = EVENTTYPE_UECONTEXTRELCMD;
       uehStartTmr (UEH_TMR_WAIT_MME_REL,&((UehUeCb *)ueCb)->mmeRelIndTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
       ret = UehLiSztRelReq(&uehCb.sztSap[0]->pst,uehCb.sztSap[0]->spId , (SztRelReq *)msg);
     break;

     case  EVENTTYPE_S1APERRIND :
       objType=UE;
       inst=ueCb->crnti;
       uehCb.uehTimerDescTable[UEH_TMR_WAIT_MME_REL].sentEvt = EVENTTYPE_S1APERRIND;
       uehCb.uehTimerDescTable[UEH_TMR_WAIT_MME_REL].expEvt = EVENTTYPE_UECONTEXTRELCMD;
       ret = UehLiSztRelReq(&uehCb.sztSap[0]->pst,uehCb.sztSap[0]->spId , (SztRelReq *)msg);
       uehStartTmr (UEH_TMR_WAIT_MME_REL,&((UehUeCb *)ueCb)->mmeRelIndTmr,(PTR)ueCb,((UehUeCb *)ueCb)->crnti);
       break;

     case  EVENTTYPE_UECONTEXTRELCOM :
       objType=UE;
       inst=ueCb->crnti;
       ret = UehLiSztRelRsp(&uehCb.sztSap[0]->pst, uehCb.sztSap[0]->spId,(SztRelRsp *)msg);
     break;
     
     case  EVENTTYPE_RESERVED1: /* Local Deletion of S1AP context */
       ret = UehLiSztRelReq(&uehCb.sztSap[0]->pst,uehCb.sztSap[0]->spId , (SztRelReq *)msg);
     break;

     default:
       UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("EventType", uniqueEventType, "Messages to s1AP");
       ret = RFAILED;
     break;
   }
   
    if(ret != ROK)
      UEH_ERROR_SSI_EVENT_SEND_FAILED((&(uehCb.sztSap[0]->pst)), uehCb.sztSap[0]->pst.event, objType, inst);

   return ret;
}

PUBLIC S16 uehSndMsgToSch (
   IN EventType uniqueEventType, 
   IO  void  *transId,
   void      *msg
   )
{
  S16 ret          =     0;
  ObjType  objType =     UE;
  SINT32   inst    =     0;
  switch (uniqueEventType)
  { 
    case EVENTTYPE_SCHBNDREQ :
      objType = SAP;
      inst = uehCb.rgrSap[0]->suId;
      uehStartTmr (UEH_TMR_RGR_SAP_BND,&(( UehLiSapCb*)uehCb.rgrSap[0])->timer,(PTR)uehCb.rgrSap[0],0);
      ret = UehLiRgrBndReq(&uehCb.rgrSap[0]->pst, uehCb.rgrSap[0]->suId, uehCb.rgrSap[0]->spId); 
    break;

    case EVENTTYPE_SCHCELLCONFIG:
       objType = CELL; 
       inst = uehGetCellCb()->cellId;
       ret = UehLiRgrCfgReq((Pst *)&uehCb.rgrSap[0]->pst,uehCb.rgrSap[0]->spId,*(RgrCfgTransId *) transId, (RgrCfgReqInfo *)msg);
       break;
    case EVENTTYPE_SCHLCHCONFIG :
    case EVENTTYPE_SCHLCGCONFIG :
    case EVENTTYPE_SCHUECONFIG :
    case EVENTTYPE_SCHUERECONFIG:
    case EVENTTYPE_SCHUEDELETE :
       objType = UE; 
       inst = ((RgrCfgReqInfo *)msg)->u.rstInfo.crnti;
       ret = UehLiRgrCfgReq((Pst *)&uehCb.rgrSap[0]->pst,uehCb.rgrSap[0]->spId,*(RgrCfgTransId *) transId, (RgrCfgReqInfo *)msg);
    break;
#ifdef RGR_SI_SCH
    case EVENTTYPE_SCHSICONFIG :
       objType = CELL; 
       inst = uehGetCellCb()->cellId; 
       ret = uehLiRgrSiCfgReq(&uehCb.rgrSap[0]->pst, uehCb.rgrSap[0]->spId, *(RgrCfgTransId *) transId,(RgrSiCfgReqInfo *)msg);
    break;
#endif
    default :
       UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("EventType", uniqueEventType, "Messages to Scheduler");
       ret = RFAILED;
    break;
  }

   if(ret NOTEQ ROK)
      UEH_ERROR_SSI_EVENT_SEND_FAILED((&(uehCb.rgrSap[0]->pst)), uehCb.rgrSap[0]->pst.event, objType, inst);

   return ret;
}  

PUBLIC S16 uehSndMsgToPhy (
   IN EventType uniqueEventType, 
   IO void   *transId,
   void      *msg)
{
  S16 ret          = 0;
  ObjType objType = UE;
  SINT32  inst     = 0;

  switch (uniqueEventType)
  { 
    case EVENTTYPE_PHYBNDREQ :
      objType = SAP; 
      inst = uehCb.ctfSap[0]->suId;
      uehStartTmr (UEH_TMR_CTF_SAP_BND,&(( UehLiSapCb*)uehCb.ctfSap[0])->timer,(PTR)uehCb.ctfSap[0],0);
      ret = UehLiCtfBndReq(&uehCb.ctfSap[0]->pst,  uehCb.ctfSap[0]->suId,  uehCb.ctfSap[0]->spId);
    break;

    case EVENTTYPE_PHYCELLCONFIG :
      objType = CELL; 
      inst = uehGetCellCb()->cellId; /* TODO: Extract from message */
      ret = UehLiCtfCfgReq(&uehCb.ctfSap[0]->pst, uehCb.ctfSap[0]->spId, *(CtfCfgTransId *)transId,  (CtfCfgReqInfo *)msg);
    break;

      case EVENTTYPE_PHYUECONFIG :
      case EVENTTYPE_PHYUEDELETE :
      objType = UE;
      inst =((CtfCfgReqInfo *)msg)->u.cfg.u.dedCfg.ueId;
      ret = UehLiCtfCfgReq(&uehCb.ctfSap[0]->pst, uehCb.ctfSap[0]->spId, *(CtfCfgTransId *)transId,  (CtfCfgReqInfo *)msg);
    break;
    
    default :
       UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("EventType", uniqueEventType, "Messages to PHY");
       ret = RFAILED;
    break;
  }

   if(ret NOTEQ ROK)
      UEH_ERROR_SSI_EVENT_SEND_FAILED((&(uehCb.ctfSap[0]->pst)), uehCb.ctfSap[0]->pst.event, objType, inst);

  return ret;
}
PUBLIC S16 uehSndMsgToUdh (
  IN EventType   uniqueEventType, 
   void       *msg
)
{
  S16         ret     = 0;
  ObjType    objType = UE;
  SINT32      inst    = 0;

  switch (uniqueEventType)
  {
     case EVENTTYPE_UDHBNDREQ :
        objType = SAP; 
        inst = uehCb.udhSap[0]->suId;
        uehStartTmr (UEH_TMR_UDH_SAP_BND,&(( UehLiSapCb*)uehCb.udhSap[0])->timer,(PTR)uehCb.udhSap[0],0);
        ret = UehLiUdhBndReq(&uehCb.udhSap[0]->pst, uehCb.udhSap[0]->suId, uehCb.udhSap[0]->spId);
     break;

     case EVENTTYPE_UDHADDRABREQ :
        objType = UE;
        inst = ((UehUdhAddRabReq *)msg)->pdcpId.ueId;
        ret = UehLiUdhAddRabReq(&uehCb.udhSap[0]->pst, 0,(UehUdhAddRabReq *)msg);
     break;
     
     case EVENTTYPE_UDHDELRABREQ :
        objType = UE;
        inst =( (UehUdhDelRabReq *)msg)->pdcpId.ueId;
        ret = UehLiUdhDelRabReq(&uehCb.udhSap[0]->pst, 0, (UehUdhDelRabReq *)msg); 
     break;

     case EVENTTYPE_UDHDELUEREQ :
        objType = UE; 
        inst = ((UehUdhDelUeReq *)msg)->ueId;
        ret = UehLiUdhDelUeReq(&uehCb.udhSap[0]->pst, 0, (UehUdhDelUeReq *)msg);
     break;

     default :
       UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("EventType", uniqueEventType, "Messages to UDH");
        ret = RFAILED;
  }

   if(ret NOTEQ ROK)
      UEH_ERROR_SSI_EVENT_SEND_FAILED((&(uehCb.udhSap[0]->pst)), uehCb.udhSap[0]->pst.event, objType, inst); 

   return ret;
}
 
PUBLIC S16 uehSndMsgOut (
   IN  Ent               dstEnt,
   IN  EventType         uniqueEventType,
   IO  UehUeCb           *ueCb,
   IO  void              *transId,
   FR  void              *msg)
{
   S16 ret = 0;

   switch (dstEnt)
   { 
     case ENTNH :
        ret = uehSndMsgToRrc (uniqueEventType,ueCb,msg);
     break;

     case ENTSZ :
        ret = uehSndMsgToS1 (uniqueEventType, ueCb, msg);
     break;

     case ENTRG :
        ret = uehSndMsgToSch (uniqueEventType, transId, msg);
     break;

     case ENTTF :
        ret = uehSndMsgToPhy (uniqueEventType, transId, msg);
     break;

     case ENTUDH :
        ret = uehSndMsgToUdh (uniqueEventType, msg);
     break;

     default :
       UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("Destination Entity", dstEnt, "ENTNH/ ENTSZ/ ENTRG/ ENTTF/ ENTUDH");
       ret = RFAILED;
     break;
   }
   if ((ROK EQ ret))
   {
     uehRecordUeEvnt( ueCb, uniqueEventType , msg,FALSE ); 
   }
   else if (ueCb) uehSetState(ueCb, UE_STATE_MSG_SEND_FAILED);
   return ret;
}

#ifdef PWAV_TS
struct timeval  gTv_a1;
struct timeval  gTv_a2;
PUBLIC S16 uehSndCtfSetTsReq()
{
  Pst             pst;
  SetTsReq        setTsReq;

  SM_SET_ZERO(&pst, sizeof(Pst));
  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTUEH;
  pst.dstEnt    = ENTTF;
  pst.dstProcId = SM_YS_PROC;
  pst.srcProcId = SM_UEH_PROC;

  gettimeofday(&gTv_a1, NULL);
  setTsReq.a1 = gTv_a1.tv_usec;

  RETVALUE(cmPkAndSndSetTsReq(&pst, &setTsReq));
}

PUBLIC S16 uehSndCtfSetTsCfm(SetTsRsp *setTsRsp)
{
  Pst           pst;
  SetTsCfm      setTsCfm;

  SM_SET_ZERO(&pst, sizeof(Pst));
  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTUEH;
  pst.dstEnt    = ENTTF;
  pst.dstProcId = SM_YS_PROC;
  pst.srcProcId = SM_UEH_PROC;
  
  gettimeofday(&gTv_a2, NULL);
  gRtt_a = ((gTv_a2.tv_sec * 1000000) + gTv_a2.tv_usec) - ((gTv_a1.tv_sec * 1000000) + gTv_a1.tv_usec);

  setTsCfm.a2 = gTv_a2.tv_usec;
  setTsCfm.b1 = setTsRsp->b1;
  setTsCfm.rtt_a = gRtt_a;

  logError("Roundtrip times: gTv_a1(sec.usec)=(%d.%d), gTv_a2(sec.usec)=(%d.%d), rtt_a(sec.usec)=(%d.%d)", \
           gTv_a1.tv_sec, gTv_a1.tv_usec, gTv_a2.tv_sec, gTv_a2.tv_usec, gRtt_a/1000000, gRtt_a%1000000);

  RETVALUE(cmPkAndSendSetTsCfm(&pst, &setTsCfm));
}

PUBLIC S16 UehLiCtfSetTsRsp(
    Pst           *pst,
    SetTsRsp      *setTsRsp
  )
{
  RETVALUE(uehSndCtfSetTsCfm(setTsRsp));
}

PUBLIC S16 uehValidateEnbAndMmeUeS1apIds(U32 mmeUeS1apId,U32 enbUeS1apId,U16 curCrnti)
{
  UehCellCb   *cellCb = uehGetCellCb();
  UehUeCb     *ueCb   = NULLP;
  U32         ueIdx = 0;
  U32         cause = -1;

  for (ueIdx = 0;ueIdx < cellCb->maxRntiPool;ueIdx++)
  {
    UEH_GET_UECB_FROM_UEIDX(ueCb, ueIdx);
    if ((NULLP != ueCb) && (TRUE == ueCb->isUeCbAllocated) && (ueCb->crnti != curCrnti))
    {
      if ((mmeUeS1apId == ueCb->s1ConCb.mme_ue_s1ap_id) && (enbUeS1apId == ueCb->s1ConCb.enb_ue_s1ap_id))
      {
        cause = SztCauseRadioNwunknown_pair_ue_s1ap_idEnum;
      }
      else if (mmeUeS1apId == ueCb->s1ConCb.mme_ue_s1ap_id)
      {
        cause = SztCauseRadioNwunknown_mme_ue_s1ap_idEnum;
      }
      else if (enbUeS1apId == ueCb->s1ConCb.enb_ue_s1ap_id)
      {
        cause = SztCauseRadioNwunknown_enb_ue_s1ap_idEnum;
      }
      if(cause != -1)
      {
        logError("TX|S1ErrInd|cause %d|enb_s1ap_id=%d|mme_s1ap_id=%d|cur-crnti=%d|existing-crnti=%d",
                 cause,
                 mmeUeS1apId,
                 enbUeS1apId,
                 curCrnti,
                 ueCb->crnti);

        uehSendS1apErrorInd(curCrnti,cause);
        RETVALUE(RFAILED);
      }
    }
  }
  RETVALUE(ROK);
}

void uehSendS1apErrorInd(U16 crnti,UINT32 errCause)
{
  UehUeCb    *ueCb = NULLP;
  ErrInd     errInd;
  SztRelReq  relReq;
  S1Cause    cause;

  cause.causeVal = errCause;
  cause.causeType = CauseType_RadioNw;
  UEH_GET_UECB_FROM_CRNTI(ueCb,crnti);

  clearBytes( &errInd , sizeof(ErrInd) );
  errInd.mmeUeS1apId = ueCb->s1ConCb.mme_ue_s1ap_id;
  errInd.enbUeS1apId = ueCb->s1ConCb.enb_ue_s1ap_id;

  s1apWrapS1apErrorIndPdu(&relReq.pdu,&errInd);
  relReq.relType = SZT_CXT_REL;
  relReq.connIdType = SZT_CONN_TYPE_SP;
  relReq.connId = ueCb->s1ConCb.spConnId;
  ueCb->s1ConCb.s1Cause = errCause;
  ueCb->s1ConCb.s1CauseType = CauseType_RadioNw;

  uehSndMsgOut (ENTSZ, EVENTTYPE_S1APERRIND, ueCb, TRANSID_NONE,&relReq);

}

/*************SEC_CHANGE_START*************/ 
PUBLIC S16 uehSndCtfDatReq (
 CtfKdfReqInfo *evt,
 U16 crnti
)
{
  Pst  pst;
  clearBytes( &pst , sizeof(Pst) );
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTNH;    // ENTNH is used in place of ENTUEH
  pst.dstEnt    = ENTTF;
  pst.dstProcId = SM_TF_PROC;

  pst.srcProcId = SM_NH_PROC;  //SM_UEH_PROC;
  pst.dstInst   = SM_INST_ZERO;
  pst.event   = EVTCTFKDFREQ;
  /* Issue the primitive */
  if(ROK != uehLiCtfDatReq(&uehCb.ctfSap[0]->pst, uehCb.ctfSap[0]->spId,crnti, evt))
  {
    UEH_ERROR_SSI_EVENT_SEND_FAILED( (&pst), EVTCTFKDFREQ, UE, crnti);   
    RETVALUE(RFAILED);
  }
  RETVALUE(ROK);
}


PUBLIC S16 uehFillCtfEvntHdr
(
CtfHdr      *ctfHdr,
U16         cellId,
U16         crnti
)
{
   ctfHdr->cellId = cellId;
   ctfHdr->ueId = crnti;

   RETVALUE(ROK);
} /* End of uehFillCtfEvntHdr */

/*
*
*       Fun:   uehKDF 
*
*       Desc:  Key Derv. Function 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_li_hdl.c 
*
*/
PUBLIC S16 uehKDF
(
  UehUeSecCfg  *secCfg,
  U8  *secKey,
  U32 SecKeyType,
  U16 crnti
)
{

  CtfKdfReqInfo  *kdfReqInfo = NULLP;

#ifdef SPACC_DBG
  U8 idx = 0;
  for(idx = 0; idx < CTF_SEC_KEY_LEN ; idx++)
  {
    logError("%x ",secKey[idx]);  
  }
#endif

  
  UEH_ALLOC(&kdfReqInfo, sizeof(CtfKdfReqInfo));

  if(NULLP == kdfReqInfo)
  {
    RETVALUE(RFAILED);
  }


  kdfReqInfo->keyDerType =SecKeyType;

  cmMemset(kdfReqInfo->u.kEnbInf.secKey,0,CTF_SEC_KEY_LEN);
  cmMemcpy(kdfReqInfo->u.kEnbInf.secKey,secKey,CTF_SEC_KEY_LEN);

  kdfReqInfo->u.kEnbInf.ciphAlgoType = secCfg->ciphAlgoSelected;
  kdfReqInfo->u.kEnbInf.intgAlgoType = secCfg->intgAlgoSelected;

   if (RFAILED == uehSndCtfDatReq(kdfReqInfo,crnti))
  {
    logError("%s","uehSndCtfDatReq Failed");
     UEH_FREEEVNT(kdfReqInfo);
    RETVALUE(RFAILED);
  }

  RETVALUE(ROK);
} /* uehKDF */
#endif

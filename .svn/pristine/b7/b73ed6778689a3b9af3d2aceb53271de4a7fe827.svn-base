/*
 * ueh_s1ap_cm.c
 *
 * This file contains S1AP common Procedures Used by different modules
 * @author: lmadala
 *          Shweta Polepally :30th Aug 2011 : Added Code under HO_SUPPORTED
 *
 */


#include "ccpu_cmn.h"

/* RRM related includes */
#include "lueh.h"
#include "lueh.x"
#include "ueh_eut_x.h"
#include "ueh.h"
#include "s1ap_cm.h"
#include "s1ap_cmn_proc.h"
#include "ueh_s1ap_cm.h"
#include "s1ap_mgr.h"
#include "bs_err.h"

UINT32 uehGetPeerIdForUeFromUeCb(
    UehUeCb          *ueCb
    )
{
  /* Compare the MCC & MNC and send it to that MME Only
   * Presently we get mmeIndex as 1 from BSM while S1AP config
   * so return 1 always */

  return UEHSM_LSZ_MME_PRC1;
}

UINT16 uehGetMaxS1apConnAllowed(void)
{
  return ( smCb.cfgCb.s1apCfgInfo.maxConnectedUes);//smCfgCb.s1apCfgInfo.maxConnectedUes);
}

static void uehGetPlmnIdByRrcPlmnIndex(
  UINT8    *plmnId,
  U8       selectedPlmnIndex
  )
{
  switch(selectedPlmnIndex)
  {
    case 1:
      /* Fill the PLMN ID received as part of uehConfig from BSM */
      s1apGetPlmnIdFromMccMnc( plmnId, uehCb.uehPlmnId.plmnMCC, uehCb.uehPlmnId.plmnMNC);
      break;
    default:
      UEH_ERROR_ELMNT_OUT_OF_RANGE_VERIFY_VALID_RANGE("PLMN-Identity", selectedPlmnIndex, "1");
      break;
  }
}

void uehFillS1apInitMsg(
    InitialUeMessage *initialUeMsg,
    UehUeCb          *ueCb,
    TBOOL            includeGummeiIe,
    TBOOL            plmnIdIeRcvd,
    UeGummei         *gummei
  )
{
  initialUeMsg->enbUeS1apId   = ueCb->s1ConCb.enb_ue_s1ap_id;

  s1apGetTaiFromCfg( &(initialUeMsg->tai) );
  s1apGetEUtranCgiFromCfg( &(initialUeMsg->eUtranCgi) );

  if( TRUE == ueCb->isSTmsi )
  {
    initialUeMsg->isSTimsi = TRUE;
    memcpy( &(initialUeMsg->sTimsi),  &(ueCb->s1Stmsi), sizeof(STimsi) );
  }

  initialUeMsg->rrcEstablishmentCause = ueCb->establishCause;

  if( TRUE EQ includeGummeiIe )
  {
    initialUeMsg->isGummei     = TRUE;
    initialUeMsg->gummei.mmeId = gummei->mmeId;
    memcpy(&(initialUeMsg->gummei.mmeGroupId),  &(gummei->mmeGrpId), SIZE_MMEGROUPID);
    if ( TRUE EQ plmnIdIeRcvd )
    {
      s1apGetPlmnIdFromMccMnc(initialUeMsg->gummei.plmnId, gummei->mcc, gummei->mnc);
    }
    else
    {
      uehGetPlmnIdByRrcPlmnIndex( initialUeMsg->gummei.plmnId, ueCb->selectedPlmnInfo );
    }
  }
}

void uehFillUlNasMsg(
    UlNasMessage     *ulNasMsg,
    UehS1ConCb       *s1ConCb,
    NhuDatIndSdus    *datIndSdu
    )
{
  /* Fill the UL NAS Message */
  clearBytes( ulNasMsg, sizeof(UlNasMessage) );

  ulNasMsg->mmeUeS1apId   = s1ConCb->mme_ue_s1ap_id;
  ulNasMsg->enbUeS1apId   = s1ConCb->enb_ue_s1ap_id;
  ulNasMsg->nasPdu.pduLen = getNasPduLenFromUlInfoTransfer( datIndSdu );
  memcpy( ulNasMsg->nasPdu.pdu,
          getNasPduFromUlInfoTransfer(datIndSdu),
          ulNasMsg->nasPdu.pduLen );

  s1apGetTaiFromCfg( &(ulNasMsg->tai) );
  s1apGetEUtranCgiFromCfg( &(ulNasMsg->eUtranCgi) );
}

void uehFillResetReqOamInt(
    S1ResetReq     *resetReqMsg,
    UehS1ConCb     *s1ConCb
    )
{
  /* Fill the UL NAS Message */
  clearBytes( resetReqMsg, sizeof(S1ResetReq) );

  resetReqMsg->resetType   = ResetType_UeS1Reset;
  resetReqMsg->u.ueAssocLogConnList.numOfS1UeAssocLogConnItems = 1;
  if( uehGetState( s1ConCb->ueCb ) >= UE_STATE_CONNECTED )
  {
    resetReqMsg->u.ueAssocLogConnList.s1ConnItem[0].isMmeUeS1apId = TRUE;
    resetReqMsg->u.ueAssocLogConnList.s1ConnItem[0].mmeUeS1apId = s1ConCb->mme_ue_s1ap_id;
  }
  resetReqMsg->u.ueAssocLogConnList.s1ConnItem[0].isEnbUeS1apId = TRUE;
  resetReqMsg->u.ueAssocLogConnList.s1ConnItem[0].enbUeS1apId = s1ConCb->enb_ue_s1ap_id;


  resetReqMsg->resetCause.causeType = CauseType_Misc;
  resetReqMsg->resetCause.causeVal = SztCauseMiscom_interventionEnum;
}

void uehFillUeCtxRelReq(
    UeContextRelReq     *ueXtxRelReqMsg,
    UehS1ConCb          *s1ConCb,
    S1Cause             *cause
    )
{
  clearBytes( ueXtxRelReqMsg, sizeof(UeContextRelReq) );

  ueXtxRelReqMsg->mmeUeS1apId   = s1ConCb->mme_ue_s1ap_id;
  ueXtxRelReqMsg->enbUeS1apId   = s1ConCb->enb_ue_s1ap_id;
  ueXtxRelReqMsg->cause         = *cause;
}

void uehFillUeCtxRelCmpl(
    UeContextRelCmp     *ueCtxRelCmplMsg,
    UehS1ConCb          *s1ConCb
    )
{
  /* Fill the UL NAS Message */
  clearBytes( ueCtxRelCmplMsg, sizeof(UeContextRelCmp) );

  ueCtxRelCmplMsg->mmeUeS1apId   = s1ConCb->mme_ue_s1ap_id;
  ueCtxRelCmplMsg->enbUeS1apId   = s1ConCb->enb_ue_s1ap_id;
  ueCtxRelCmplMsg->isCriticalDiagnostic = FALSE;
  /* TODO Add Critical diagnostics at a leter date */
}

void uehFillUeCtxModFail(
    UeContextModFail            *ueCtxModFailMsg,
    UehS1ConCb                  *s1ConCb,
    S1Cause                     *cause
    )
{
  clearBytes( ueCtxModFailMsg, sizeof(UeContextModFail) );

  ueCtxModFailMsg->mmeUeS1apId   = s1ConCb->mme_ue_s1ap_id;
  ueCtxModFailMsg->enbUeS1apId   = s1ConCb->enb_ue_s1ap_id;
  /* Fill Cause */
  ueCtxModFailMsg->cause         = *cause;
  ueCtxModFailMsg->isCriticalDiagnostic = FALSE;
  /* TODO Add Critical diagnostics at a later date */
}

void uehFillUeCtxModRsp(
    UeContextModRsp             *ueCtxModRsp,
    UehS1ConCb                  *s1ConCb
    )
{
  clearBytes( ueCtxModRsp, sizeof(UeContextModRsp) );

  ueCtxModRsp->mmeUeS1apId   = s1ConCb->mme_ue_s1ap_id;
  ueCtxModRsp->enbUeS1apId   = s1ConCb->enb_ue_s1ap_id;

  ueCtxModRsp->isCriticalDiagnostic = FALSE;
  /* TODO Add Critical diagnostics at a later date */
}

void uehFillInitCtxSetupFail(
    InitialContextSetupFail     *initCtxSetupFailMsg,
    UehS1ConCb                  *s1ConCb,
    S1Cause                     *cause
    )
{
  /* Fill the UL NAS Message */
  clearBytes( initCtxSetupFailMsg, sizeof(InitialContextSetupFail) );

  initCtxSetupFailMsg->mmeUeS1apId   = s1ConCb->mme_ue_s1ap_id;
  initCtxSetupFailMsg->enbUeS1apId   = s1ConCb->enb_ue_s1ap_id;
  /* Fill Cause */
  initCtxSetupFailMsg->cause         = *cause;
  initCtxSetupFailMsg->isCriticalDiagnostic = FALSE;
  /* TODO Add Critical diagnostics at a later date */
}

void uehFillInitCtxSetupRsp(
    InitialContextSetupRsp      *initCtxSetupRspMsg,
    UehUeCb                     *ueCb
    )
{
  UINT16 idx, succRabIdx, failRabIdx, cnt,len;
  UINT8  offSet;

  /* Fill the InitialContextSetup Response Message */
  clearBytes( initCtxSetupRspMsg, sizeof(InitialContextSetupRsp) );

  initCtxSetupRspMsg->mmeUeS1apId   = ueCb->s1ConCb.mme_ue_s1ap_id;
  initCtxSetupRspMsg->enbUeS1apId   = ueCb->s1ConCb.enb_ue_s1ap_id;
  /* TODO : need to correct the below code */
  for(idx=0, succRabIdx=0, failRabIdx=0; idx< ueCb->rbInfo.numOfRbCfgd; idx++ )
  {
    if( ueCb->rbInfo.drb[idx].isEgtpTnlEnbl && 
      (UEH_RB_RGRMSK_DONE == ueCb->rbInfo.drb[idx].rgrCfgMask))
    {
      initCtxSetupRspMsg->eRabSetupList.noComp++;
      initCtxSetupRspMsg->eRabSetupList.eRabSetupItem[succRabIdx].eRabId = ueCb->rbInfo.drb[idx].eRABId;
      initCtxSetupRspMsg->eRabSetupList.eRabSetupItem[succRabIdx].gtPTeId.gtPTeId[0] = ( (ueCb->rbInfo.drb[idx].locEgtpTunId) >> 24 );
      initCtxSetupRspMsg->eRabSetupList.eRabSetupItem[succRabIdx].gtPTeId.gtPTeId[1] = ( (ueCb->rbInfo.drb[idx].locEgtpTunId) >> 16 ) & 0xFF;
      initCtxSetupRspMsg->eRabSetupList.eRabSetupItem[succRabIdx].gtPTeId.gtPTeId[2] = ( (ueCb->rbInfo.drb[idx].locEgtpTunId) >> 8 ) & 0xFF;
      initCtxSetupRspMsg->eRabSetupList.eRabSetupItem[succRabIdx].gtPTeId.gtPTeId[3] = ( (ueCb->rbInfo.drb[idx].locEgtpTunId) & 0xFF);

      if(uehCb.datAppAddr.type == CM_TPTADDR_IPV4)
      {
        initCtxSetupRspMsg->eRabSetupList.eRabSetupItem[succRabIdx].transportLayerAddr.transportAddrType = TrasportAddrType_Ipv4;
        len = 4; /* 4bytes */

        for (cnt = 0; cnt < len; cnt++)
        {
           /* compute the offset to right shift the val in U32
            * shift by 24, 16 & 8 bytes */
           offSet = (len - (cnt + 1)) * 8;
           initCtxSetupRspMsg->eRabSetupList.eRabSetupItem[succRabIdx].transportLayerAddr.transportLevelAddr[cnt] =
              (U8)(uehCb.datAppAddr.u.ipv4TptAddr.address >> offSet);
        }

      }
      else
      {
        initCtxSetupRspMsg->eRabSetupList.eRabSetupItem[succRabIdx].transportLayerAddr.transportAddrType = TrasportAddrType_Ipv6;
        len = 16; /* First Four bytes of 20 bytes is for IPV4, so skip these 4bytes */
        for(cnt = 0; cnt < len; cnt++)
        {
          initCtxSetupRspMsg->eRabSetupList.eRabSetupItem[succRabIdx].transportLayerAddr.transportLevelAddr[cnt + 4] =
              uehCb.datAppAddr.u.ipv6TptAddr.ipv6NetAddr[cnt];
        }
      }

      succRabIdx++;
    }
    else
    {
      initCtxSetupRspMsg->isERabList = TRUE;
      initCtxSetupRspMsg->eRabList.noItems++;

      initCtxSetupRspMsg->eRabList.eRabListItem[failRabIdx].eRabId = ueCb->rbInfo.drb[idx].eRABId;
      initCtxSetupRspMsg->eRabList.eRabListItem[failRabIdx].cause.causeType = CauseType_RadioNw;
      initCtxSetupRspMsg->eRabList.eRabListItem[failRabIdx].cause.causeVal = SztCauseRadioNwradio_resources_not_availableEnum;
      failRabIdx++;
    }
  }

  /* TODO Add Critical diagnostics at a later date */
  initCtxSetupRspMsg->isCriticalDiagnostic = FALSE;
}

void uehFillNasNonDeliveryIndMsg(
    NasNonDeliveryInd   *nasNonDeliveryIndMsg,
    UehS1ConCb          *s1ConCb,
    U16                  nasPduLen,
    U8                  *nasPdu
    )
{
  /* Fill the UL NAS Message */
  clearBytes( nasNonDeliveryIndMsg, sizeof(UlNasMessage) );

  nasNonDeliveryIndMsg->mmeUeS1apId   = s1ConCb->mme_ue_s1ap_id;
  nasNonDeliveryIndMsg->enbUeS1apId   = s1ConCb->enb_ue_s1ap_id;

  nasNonDeliveryIndMsg->nasPdu.pduLen = nasPduLen;
  memcpy( nasNonDeliveryIndMsg->nasPdu.pdu, nasPdu, nasNonDeliveryIndMsg->nasPdu.pduLen );

  nasNonDeliveryIndMsg->cause.causeType = CauseType_RadioNw;
  nasNonDeliveryIndMsg->cause.causeVal  = SztCauseRadioNwfailure_in_radio_interface_procedureEnum;

}

void uehFillUeCapInfoIndMsg(
    UehUeCb             *ueCb,
    UeCapInfoInd        *ueCapInfoInd
  )
{
  ueCapInfoInd->mmeUeS1apId = ueCb->s1ConCb.mme_ue_s1ap_id;
  ueCapInfoInd->enbUeS1apId = ueCb->s1ConCb.enb_ue_s1ap_id;

  ueCapInfoInd->ueRadioCapInfoPdu.infoLen = ueCb->ueRadCap.ueRadAccCap.len;
  memcpy(ueCapInfoInd->ueRadioCapInfoPdu.capInfopdu, ueCb->ueRadCap.ueRadAccCap.UeRadioCapability, ueCb->ueRadCap.ueRadAccCap.len);

}

void uehFillUeLocReportFailInd(
  LocReqReportCntrl  *locReqReportCntrl,
  LocReqReportFail   *locReqReportFail,
  S1Cause            *cause
  )
{
  locReqReportFail->mmeUeS1apId = locReqReportCntrl->mmeUeS1apId;
  locReqReportFail->enbUeS1apId = locReqReportCntrl->enbUeS1apId;

  locReqReportFail->cause = *cause;
}

void uehFillUeLocReportSucc(
  LocReqReportCntrl  *locReqReportCntrl,
  LocReqReport       *locReqReport
  )
{
  locReqReport->enbUeS1apId = locReqReportCntrl->enbUeS1apId;
  locReqReport->mmeUeS1apId = locReqReportCntrl->mmeUeS1apId;

  s1apGetEUtranCgiFromCfg(&locReqReport->eUtranCgi);
  s1apGetTaiFromCfg(&locReqReport->tai);

  locReqReport->locReportCntrlReqType = locReqReportCntrl->locReportCntrlReqType;
}

void uehFillTraceFailInd(
  UehS1ConCb            *s1ConCb,
  EUtranTraceId         *eUtranTraceId,
  TraceFailInd          *traceFailInd,
  S1Cause               *cause
  )
{
  traceFailInd->mmeUeS1apId = s1ConCb->mme_ue_s1ap_id;
  traceFailInd->enbUeS1apId = s1ConCb->enb_ue_s1ap_id;
  traceFailInd->eUtranTraceId = *eUtranTraceId;
  traceFailInd->cause = *cause;
}

void uehFillERabSetupRsp(
  ERabSetupReq          *eRabSetupReq,
  ERabSetupRsp          *eRabSetupRsp,
  S1Cause               *cause
  )
{
  UINT16 i;

  eRabSetupRsp->mmeUeS1apId = eRabSetupReq->mmeUeS1apId;
  eRabSetupRsp->enbUeS1apId = eRabSetupReq->enbUeS1apId;

  eRabSetupRsp->isCriticalDiagnostic = FALSE;
  eRabSetupRsp->isERabSetupList = FALSE;

  eRabSetupRsp->isERabFailedSetupList = TRUE;

  eRabSetupRsp->eRabFailedSetupList.noItems = eRabSetupReq->eRabToSetupList.noComp;
  for( i=0; i< eRabSetupReq->eRabToSetupList.noComp; i++)
  {
    eRabSetupRsp->eRabFailedSetupList.eRabListItem[i].eRabId = eRabSetupReq->eRabToSetupList.eRabToSetupItem[i].eRabId;
    eRabSetupRsp->eRabFailedSetupList.eRabListItem[i].cause = *cause;
  }
}

TBOOL uehIsRabIdValid(
    UehUeCb             *ueCb,
    U32                  eRABId
    )
{
  UINT16 idx;

  for(idx = 0; idx < ueCb->rbInfo.numOfRbCfgd; idx++)
  {
    if( eRABId == ueCb->rbInfo.drb[idx].eRABId)
      return TRUE;
  }
  return FALSE;
}

void uehFillERabModifyRsp(
    ERabModifyReq          *eRabModifyReq,
    ERabModifyRsp          *eRabModifyRsp,
    S1Cause                *cause,
    UehUeCb                *ueCb
  )
{
  UINT16 i;

  eRabModifyRsp->mmeUeS1apId = eRabModifyReq->mmeUeS1apId;
  eRabModifyRsp->enbUeS1apId = eRabModifyReq->enbUeS1apId;

  eRabModifyRsp->isCriticalDiagnostic = FALSE;
  eRabModifyRsp->isERabModifyList = FALSE;

  eRabModifyRsp->isERabFailedModifyList = TRUE;
  eRabModifyRsp->eRabFailedModifyList.noItems = eRabModifyReq->eRabToModifyList.noComp;
  for( i=0; i< eRabModifyReq->eRabToModifyList.noComp; i++)
  {
    eRabModifyRsp->eRabFailedModifyList.eRabListItem[i].eRabId = eRabModifyReq->eRabToModifyList.eRabToModifyItem[i].eRabId;
    if( TRUE == uehIsRabIdValid( ueCb, eRabModifyRsp->eRabFailedModifyList.eRabListItem[i].eRabId) )
    {
      eRabModifyRsp->eRabFailedModifyList.eRabListItem[i].cause = *cause;
    }
    else
    {
      eRabModifyRsp->eRabFailedModifyList.eRabListItem[i].cause.causeType = CauseType_RadioNw;
      eRabModifyRsp->eRabFailedModifyList.eRabListItem[i].cause.causeVal  = SztCauseRadioNwunknown_E_RAB_IDEnum;
    }
  }
}

void uehFillERabReleaseRsp(
    ERabReleaseReq          *eRabReleaseReq,
    ERabReleaseRsp          *eRabReleaseRsp,
    S1Cause                 *cause,
    UehUeCb                 *ueCb
  )
{
  UINT16 i;

  eRabReleaseRsp->mmeUeS1apId = eRabReleaseReq->mmeUeS1apId;
  eRabReleaseRsp->enbUeS1apId = eRabReleaseReq->enbUeS1apId;

  eRabReleaseRsp->isCriticalDiagnostic = FALSE;
  eRabReleaseRsp->isERabReleaseList = FALSE;

  eRabReleaseRsp->isERabFailedReleaseList = TRUE;
  eRabReleaseRsp->eRabFailedReleaseList.noItems = eRabReleaseReq->eRabToReleaseList.noComp;
  for( i=0; i< eRabReleaseReq->eRabToReleaseList.noComp; i++)
  {
    eRabReleaseRsp->eRabFailedReleaseList.eRabListItem[i].eRabId = eRabReleaseReq->eRabToReleaseList.eRabToReleaseItem[i].eRabId;
    if( TRUE == uehIsRabIdValid( ueCb, eRabReleaseRsp->eRabFailedReleaseList.eRabListItem[i].eRabId) )
    {
      eRabReleaseRsp->eRabFailedReleaseList.eRabListItem[i].cause = *cause;
    }
    else
    {
      eRabReleaseRsp->eRabFailedReleaseList.eRabListItem[i].cause.causeType = CauseType_RadioNw;
      eRabReleaseRsp->eRabFailedReleaseList.eRabListItem[i].cause.causeVal  = SztCauseRadioNwunknown_E_RAB_IDEnum;
    }
  }
}

S1apMgrCb *uehGetS1CbFromTmsi(
    UehSTmsi     *sTmsi
    )
{
  UINT16 i, j;
  S1apMgrCb     *s1Cb =  getS1MgrCb( UEHSM_LSZ_MME_PRC1 );


  for( i=0; i< s1Cb->servedGummeIes.numOfGummeies; i++ )
  {
    for( j=0; j< s1Cb->servedGummeIes.servedGummeIe[i].servedMmes.numMme; j++ )
    {
      if( sTmsi->mmeCode == s1Cb->servedGummeIes.servedGummeIe[i].servedMmes.mmeId[j][0] )
      {
          return s1Cb;
      }
    }
  }
  return NULL;
}

S1apMgrCb *uehGetS1CbFromSelectedPlmnId(
    U8  selectedPlmnId
    )
{
  /* Currently we support only one PLMN,
   * We need to extend this function when we support multiple PLMN's or multiple MME's
   * Get PLMN MCC, MNC from PLMNID
   * Get s1Cb from MMC,MNC */
  S1apMgrCb     *s1Cb =  getS1MgrCb( UEHSM_LSZ_MME_PRC1 );


  return s1Cb;
}

PUBLIC S16 uehCheckMmeOverload(
    U32        establishCause,
    U16        ueId /* for logging */
    )
{
  S1apMgrCb     *s1Cb = getS1MgrCb( UEHSM_LSZ_MME_PRC1 );

  UECM_FUNC(ueId,uehCheckMmeOverload,UECM,CRM,"Check MME Overload");

#if 0
  /* This function is called at RRC Connection Request */
  if( TRUE == ueCb->isSTmsi )
  {
    s1Cb = uehGetS1CbFromTmsi( &(ueCb->sTmsi) );
    if( NULLP == s1Cb)
    {
      logInfo("The UE :%d is not registered with this MME ",ueCb->crnti); 
    }
  }
  if (NULLP EQ s1Cb)
  {
    s1Cb = uehGetS1CbFromSelectedPlmnId( ueCb->selectedPlmnInfo);
  }
#endif

  if( OverloadAction_PermitAllCalls == s1Cb->overloadState )
  {
    logInfo ("%s","MME State: All Calls are allowed");
    RETVALUE(ROK);
  }

  if( (OverloadAction_PermitEmergencyOnly EQ s1Cb->overloadState ) &&
      (RrcEstablishmentCause_Emergency != establishCause ) )
  {
    UEH_PROCEDURE_FAILED("Admit UE", UE, ueId, "MME Overload: Only emergency calls allowed");
    RETVALUE(RFAILED);
  }

  if( (OverloadAction_RejectAllRrcConnEstForSignalling EQ s1Cb->overloadState ) &&
      (RrcEstablishmentCause_MoSignalling EQ establishCause ) )
  {
    UEH_PROCEDURE_FAILED("Admit UE", UE, ueId, "MME Overload: MO Sig not allowed");
    RETVALUE(RFAILED);
  }

  if( (OverloadAction_RejectAllRrcNonEmergencyMoData EQ s1Cb->overloadState ) &&
      (RrcEstablishmentCause_MoData EQ establishCause ) )
  {
    UEH_PROCEDURE_FAILED("Admit UE", UE, ueId, "MME Overload: MO Data not allowed");
    RETVALUE(RFAILED);
  }

  RETVALUE(ROK);
}



static void uehFillSrcEnbToTargetEnbTransparentContainer(
  IO SrcEnbToTargetEnbTransparentCont *srcEnbToTgtEnb,
  IN UehUeHovrInfo                     *ueHovrInfo
  )
{
  /* Fill Mandatory Parameter rrcContainer */
  srcEnbToTgtEnb->rrcContainer.msgLen = ueHovrInfo->rrcCtnrHovrPrepInfo.msgLen;
  memcpy( srcEnbToTgtEnb->rrcContainer.handoverPrepInfoMsgPdu,
          ueHovrInfo->rrcCtnrHovrPrepInfo.handoverPrepInfoMsgPdu,
          ueHovrInfo->rrcCtnrHovrPrepInfo.msgLen);

  /* Fill Mandatory Parameter TargetCell ID */
  /* TODO Which Neighbor should be considered ? By default taken as index 0  */
  s1apGetPlmnIdFromMccMnc(&srcEnbToTgtEnb->targetCellId.plmnIdentity[0],
                          &uehCb.uehCfgCb.eutraNeighCellConfig[0].neighEutraPlmnMcc[0],
                          &uehCb.uehCfgCb.eutraNeighCellConfig[0].neighEutraPlmnMnc[0]);
  s1apGetCellIdFromEnbIdAncActCellId(&srcEnbToTgtEnb->targetCellId.cellIdentity[0],
                                     uehCb.uehCfgCb.eutraNeighCellConfig[0].neighEutraEnbId,
                                     uehCb.uehCfgCb.eutraNeighCellConfig[0].neighEutraCellId);

  /* Optional Parameters not filled */
  srcEnbToTgtEnb->isSubProfileIdOrFreqPriorityPresent = FALSE;
  srcEnbToTgtEnb->isErabsInfoListPresent = FALSE;

  /* Fill Mandatory Parameter UE HistoryInfo */
  /* TODO : Need to fill this Mandatory parameter
   *srcEnbToTgtEnb->ueHistoryInfo.lastVisitedCellList.numOfCells = */
}

void uehFillSrcToTargetTransparentContainer(
  IO SrcToTargetTransparentContainer *srcToTargetTransparentContainer,
  IN UehUeHovrInfo                   *ueHovrInfo
  )
{
  switch( ueHovrInfo->handoverType )
  {
    case HandoverType_IntraLte:
      {
        uehFillSrcEnbToTargetEnbTransparentContainer( &(srcToTargetTransparentContainer->srcEnbToTargetEnb),
                                                       ueHovrInfo );
      }
      break;
    case HandoverType_LteToUtran:
      BS_ERROR_S1AP_HANDOVER_TYPE_NOT_SUPPORTED("HandoverType_LteToUtran");
      break;
    case HandoverType_LteToGeran:
      BS_ERROR_S1AP_HANDOVER_TYPE_NOT_SUPPORTED("HandoverType_LteToGeran");
      break;
    case HandoverType_UtranToLte:
      BS_ERROR_S1AP_HANDOVER_TYPE_NOT_SUPPORTED("HandoverType_UtranToLte");
      break;
    case HandoverType_GeranToLte:
      BS_ERROR_S1AP_HANDOVER_TYPE_NOT_SUPPORTED("HandoverType_GeranToLte");
      break;
  }
}

/**
 * @param
 * @param
 * @details This functions fills Handover Required Message and sends to MME.
 */
void uehFillHandoverRequired(
  IO HandoverRequired  *handoverRequired,
  UehUeCb              *ueCb,
  S1Cause              *cause,
  S1apPdu              *pdu
  )
{
  handoverRequired->mmeUeS1apId  = ueCb->s1ConCb.mme_ue_s1ap_id;
  handoverRequired->enbUeS1apId  = ueCb->s1ConCb.enb_ue_s1ap_id;
  handoverRequired->handoverType =  ueCb->ueHovrInfo.handoverType;

  handoverRequired->cause = *cause;
  handoverRequired->targetId.targetIdType = ueCb->ueHovrInfo.targetIdType;

  switch( ueCb->ueHovrInfo.targetIdType )
  {
    case TargetType_TargetEnbId:
      {
        /* Fill Target ENB ID details based on TargetID Type */
        s1apFillEnbIdFromUint32( EnbType_MacroEnodeB,
                                 handoverRequired->targetId.val.targetEnbID.globalEnodeBId.enbId,
                                 uehCb.uehCfgCb.enodebId);
        handoverRequired->targetId.val.targetEnbID.globalEnodeBId.enodeBType = EnbType_MacroEnodeB;
        /* PBP: Taking MCC, MNC from proper place */
        s1apGetPlmnIdFromMccMnc(handoverRequired->targetId.val.targetEnbID.globalEnodeBId.plmnIdentity,
                                uehCb.uehPlmnId.plmnMCC, uehCb.uehPlmnId.plmnMNC);
        s1apGetTaiFromCfg( &(handoverRequired->targetId.val.targetEnbID.tai) );
      }
      break;
    case TargetType_TargetRncId:
      BS_ERROR_S1AP_TARGET_TYPE_NOT_SUPPORTED("TargetType_TargetRncId");
      break;
    case TargetType_Cgi:
      BS_ERROR_S1AP_TARGET_TYPE_NOT_SUPPORTED("TargetType_Cgi");
      break;
  }

  handoverRequired->isDirectFwdPathAvailabilityUsed = FALSE;
  handoverRequired->isSrvccHoIndicationUsed = FALSE;

  /* TODO Support to be added by CCPU
   * memcpy( handoverRequired->srcToTargetTransparentContainer, pdu->pdu.val.successfulOutcome.value.u.<ToBeAdded>,
          sizeof());*/

  handoverRequired->isSrcToTgtTprntContSecondary = FALSE;

  /*
   * MsClassmark2 & MsClassmark3 are mandatory if
   * if HandoverType = HandoverType_LteToGeran && isSrvccHoIndicationUsed = TRUE */
  if ( ( HandoverType_LteToGeran EQ ueCb->ueHovrInfo.handoverType )
       && ( TRUE EQ handoverRequired->isSrvccHoIndicationUsed ) )
  {
    /* This condition is not be hit as handover Type "HandoverType_LteToGeran" is not supported as of now */
    handoverRequired->isMsClassmark2Used = FALSE;
    handoverRequired->isMsClassmark3Used = FALSE;
  }
}

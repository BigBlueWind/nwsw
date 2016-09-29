/*
 * s1ap_unwrap.h
 *
 * This file contains S1AP wrap functions.
 * @author: lmadala
 * Shweta Polepally :30th Aug 2011 : Added Code under HO_SUPPORTED
 *
 *
 */

#ifndef S1AP_WRAP_H
#define S1AP_WRAP_H

#include "s1ap_cm.h"

TBOOL s1apWrapMsgFields(
    IO S1apPdu *pdu,
    IN UINT16   msgId
    );
TBOOL s1apWrapInitMsgId(
    IO S1apPdu *pdu,
    IN UINT16   msgId
    );
TBOOL s1apWrapSuccMsgId(
    IO S1apPdu *pdu,
    IN UINT16   msgId
    );
TBOOL s1apWrapUnsuccMsgId(
    IO S1apPdu *pdu,
    IN UINT16   msgId
    );
TBOOL s1apWrapIeCount(
    IO S1apPdu *pdu,
    IN UINT16   noIes
    );
TBOOL s1apWrapCause(
    IO SztCause  *causePdu,
    IN S1Cause   *cause
    );
void s1apWrapIeCriticalDiagnosticItem(
    IO SztCriticalityDiag_IE_Item   *ieCriticalDiagnosticPdu,
    IN IeCriticalDiagnostic         *ieCriticalDiagnostic
    );
TBOOL s1apWrapCriticalDiagnostic(
    IO S1apPdu              *pdu,
    IO SztCriticalityDiag   *sztCriticalityDiag,
    IN CriticalDiagnostic   *criticalDiagnostic
    );
TBOOL s1apWrapTac(
    IO SztTAC  *tacPdu,
    IN UINT8   *tac
    );
TBOOL s1apWrapMmeGrpId(
    IO SztMME_Group_ID  *mmeGrpIdPdu,
    IN UINT8            *mmeGrpId
    );
TBOOL s1apWrapMmeCode(
    IO SztMME_Code  *mmeCodePdu,
    IN UINT8        mmeCode
    );
TBOOL s1apWrapPlmnId(
    IO S1apPdu          *pdu,
    IO SztPLMNidentity  *plmnIdPdu,
    IN UINT8            *plmnId
    );
TBOOL s1apWrapCsgId(
    IO SztCSG_Id  *csgPdu,
    IN CsgId      *csgId
    );
TBOOL s1apWrapMTmsi(
    IO S1apPdu     *pdu,
    IO SztM_TMSI   *mTmsiPdu,
    IN MTimsi      *mTmsi
    );
TBOOL s1apWrapSTmsi(
    IO S1apPdu    *pdu,
    IO SztS_TMSI  *sTmsiPdu,
    IN STimsi     *sTmsi
    );
TBOOL s1apWrapGummei(
    IO S1apPdu    *pdu,
    IO SztGUMMEI  *gummeiPdu,
    IN Gummei     *gummei
    );
TBOOL s1apWrapEnodeBId(
    IO S1apPdu           *pdu,
    IO SztGlobal_ENB_ID  *enbIdPdu,
    IN GlobalEnodeBId    *enbId
    );
TBOOL s1apWrapEnodeName(
    IO S1apPdu     *pdu,
    IO SztENBname  *enbNamePdu,
    IN UINT8       *enbName
    );
TBOOL s1apWrapEutranTraceId(
    IO S1apPdu               *pdu,
    IO SztE_UTRAN_Trace_ID   *eutranTraceIdPdu,
    IO EUtranTraceId         *eutranTraceId
    );
TBOOL s1apWrapCsgList(
    IO S1apPdu       *pdu,
    IO SztCSG_IdLst  *csgListPdu,
    IN CsgIdList     *csgList
    );
TBOOL s1apWrapSupportedTacs(
    IO S1apPdu       *pdu,
    IO SztSuppTAs    *tacListPdu,
    IN SupportedTacs *tacList
    );
TBOOL s1apWrapDefaultPagingDrx(
    IO SztPagDRX  *drxPdu,
    IN UINT16     defPagingDrx
    );
TBOOL s1apWrapRstUeAssocLogConnList(
    IO S1apPdu                      *pdu,
    IO SztUE_assocLogS1_ConLstRes   *conListPdu,
    IN S1UeAssocLogConnList         *conList
    );
TBOOL s1apWrapRstAckUeAssocLogConnList(
    IO S1apPdu                       *pdu,
    IO SztUE_assocLogS1_ConLstResAck *rstAckConListPdu,
    IN S1UeAssocLogConnList          *conList
    );
TBOOL s1apWrapRrcEstablishmentCause(
    IO SztRRC_Establishment_Cause  *rrcEstablishmentCausePdu,
    IN UINT16                      rrcEstablishmentCause
    );
void s1apWrapUeAssocLogConnItem(
    IO SztUE_assocLogS1_ConItem   *conItemPdu,
    IN S1UeAssocLogConnItem       *conItem
    );
void s1apWrapEnodeBUeS1apId(
    IO SztENB_UE_S1AP_ID  *enbUeS1apIdPdu,
    IN EnbUeS1apId        enbId
    );
void s1apWrapTimeToWait(
    IO SztTimeToWait  *timeToWaitPdu,
    IN TimeToWait      timeToWait
    );
void s1apWrapLocReportCntrlReqType(
    IO SztRqstTyp                  *locReqtypePdu,
    IN LocReportCntrlReqType       *locReqtype
    );
void s1apWrapMmeUeS1apId(
    IO SztMME_UE_S1AP_ID  *mmeUeS1apIdPdu,
    IN MmeUeS1apId        mmeUeId
    );
TBOOL s1apWrapNasPdu(
    IO S1apPdu     *pdu,
    IO SztNAS_PDU  *s1NasPdu,
    IN NasPdu      *nasPdu
    );
TBOOL s1apWrapUeRadioCapInfoPdu(
    IO S1apPdu             *pdu,
    IO SztUERadioCapblty   *ueRadioCapPdu,
    IO UeRadioCapInfo      *ueRadioCap
    );
TBOOL s1apWrapTai(
    IO S1apPdu   *pdu,
    IO SztTAI    *taiPdu,
    IN Tai       *tai
    );
TBOOL s1apWrapEUtranCgi(
    IO S1apPdu        *pdu,
    IO SztEUTRAN_CGI  *eUtranCgiPdu,
    IN EUtranCgi      *eUtranCgi
    );
void s1apWrapERabListItem(
    IO SztE_RABItem   *sztE_RABItem,
    IN ERabListItem   *eRabListItem
    );
TBOOL s1apWrapGtpTeId(
    IO S1apPdu     *pdu,
    IO SztGTP_TEID *gTP_TEID,
    IO GtPTeId     *gtPTeId
  );
TBOOL s1apWrapTransportLayerAddr(
    IO S1apPdu               *pdu,
    IO SztTportLyrAddr       *transportLyrAddr,
    IN TransportLayerAddr    *transportLayerAddr
    );
TBOOL s1apWrapERabSetupItem(
    IO S1apPdu                     *pdu,
    IO SztE_RABSetupItemCtxtSURes   *sztE_RABSetupItem,
    IO ERabSetupItem               *eRabSetupItem
    );
TBOOL s1apWrapERabModifyItem(
    IO SztE_RABMdfyItemBrModRes    *sztE_RABModifyItem,
    IN ERabModifyItem              *eRabModifyItem
    );
TBOOL s1apWrapERabReleaseItem(
    IO SztE_RABRlsItemBrRelComp    *sztE_RABReleaseItem,
    IN ERabReleaseItem             *eRabReleaseItem
    );
TBOOL s1apWrapERabList(
    IO S1apPdu       *pdu,
    IO SztE_RABLst   *eztE_RABLst,
    IN ERabList      *eRabList
    );
TBOOL s1apWrapERabSetupCtxtList(
    IO S1apPdu                   *pdu,
    IO SztE_RABSetupLstCtxtSURes *sztE_RABSetupLstCtxtSURes,
    IO ERabSetupList             *eRabSetupList
    );
TBOOL s1apWrapERabSetupList(
    IO S1apPdu                   *pdu,
    IO SztE_RABSetupLstBrSURes   *sztE_RABSetupLstSURes,
    IO ERabSetupList             *eRabSetupList
    );
TBOOL s1apWrapERabModifyList(
    IO S1apPdu                   *pdu,
    IO SztE_RABMdfyLstBrModRes   *sztE_RABModifyLstCtxtSURes,
    IN ERabModifyList            *eRabModifyList
    );
TBOOL s1apWrapERabReleaseList(
    IO S1apPdu                   *pdu,
    IO SztE_RABRlsLstBrRelComp   *sztE_RABReleaseLstSURes,
    IN ERabReleaseList           *eRabReleaseList
    );
/************ S1AP Messages **************************/
TBOOL s1apWrapSetupReqPdu
  (
    IO S1apPdu      **pdu,
    IN S1SetupReq    *setupReq
  );
TBOOL s1apWrapResetReqPdu
  (
    IO S1apPdu       **pdu,
    IN S1ResetReq    *resetReq
  );
TBOOL s1apWrapInitialUeMsgPdu
  (
    IO S1apPdu            **pdu,
    IN InitialUeMessage   *initialUeMsg
  );
TBOOL s1apWrapUlNasMsgPdu
  (
    IO S1apPdu            **pdu,
    IN UlNasMessage       *ulNasMsg
  );
TBOOL s1apWrapInitialContextSetupFailPdu
  (
    IO S1apPdu                  **pdu,
    IN InitialContextSetupFail  *initialCtxSetupFail
  );
TBOOL s1apWrapS1apErrorIndPdu
  (
    IO S1apPdu     **pdu,
    IN  ErrInd     *errInd
  );

TBOOL s1apWrapInitialContextSetupRspPdu
  (
    IO S1apPdu                  **pdu,
    IO InitialContextSetupRsp  *initialCtxSetupRsp
  );
TBOOL s1apWrapUeContextRelReqPdu
  (
    IO S1apPdu          **pdu,
    IN UeContextRelReq  *ueContextRelReq
  );
TBOOL s1apWrapUeContextRelCmpPdu
  (
    IO S1apPdu          **pdu,
    IN UeContextRelCmp  *ueContextRelCmp
  );
TBOOL s1apWrapS1ResetAckPdu
  (
    IO S1apPdu          **pdu,
    IN S1ResetAck       *s1ResetAck
  );
TBOOL s1apWrapNasNonDeliveryIndPdu
  (
    IO S1apPdu            **pdu,
    IN NasNonDeliveryInd  *nasNonDeliveryMsg
  );
TBOOL s1apWrapUeCapInfoIndPdu
  (
    IO S1apPdu            **pdu,
    IO UeCapInfoInd        *ueCapInfoIndMsg
  );
TBOOL s1apWrapMmeCfgUpdateAckPdu
  (
    IO S1apPdu          **pdu,
    IN MmeCfgUpdateAck  *mmeCfgUpdateAck
  );
TBOOL s1apWrapMmeCfgUpdateFailPdu
  (
    IO S1apPdu          **pdu,
    IN MmeCfgUpdateFail *mmeCfgUpdateFail
  );
TBOOL s1apWrapLocReportFailIndPdu
  (
    IO S1apPdu            **pdu,
    IN LocReqReportFail   *locReportFailIndMsg
  );
TBOOL s1apWrapLocReportPdu
  (
    IO S1apPdu            **pdu,
    IN LocReqReport       *locReportMsg
  );
TBOOL s1apWrapTraceFailIndPdu
  (
    IO S1apPdu            **pdu,
    IO TraceFailInd       *traceFailInd
  );
TBOOL s1apWrapERabSetupRspPdu
  (
    IO S1apPdu                  **pdu,
    IO ERabSetupRsp             *eRabSetupRsp
  );
TBOOL s1apWrapERabModifyRspPdu
  (
    IO S1apPdu                  **pdu,
    IN ERabModifyRsp            *eRabModifyRsp
  );
TBOOL s1apWrapERabReleaseRspPdu
  (
    IO S1apPdu                  **pdu,
    IN ERabReleaseRsp           *eRabReleaseRsp
  );
TBOOL s1apWrapUecontextModFailPdu
  (
    IO S1apPdu                  **pdu,
    IN UeContextModFail         *ueContextModFail
  );
TBOOL s1apWrapUeContextModRspPdu
  (
    IO S1apPdu                  **pdu,
    IN UeContextModRsp          *ueContextModRsp
  );
TBOOL s1apWrapMmeWriteRplcWarnRsp
  (
    IO S1apPdu             **pdu,
    IN WriteReplcWarnRsp   *writeReplcWarnRsp
  );
TBOOL s1apWrapSrcToTargetTransparentContainer(
    IO S1apPdu                             *pdu,
    IO SrcToTargetTransparentContainer     *srcToTgetTprntCont,
    IO UehUeHovrInfo                       *ueHovrInfo
    );
/**
 *  S1AP Based handover functions defined here
 **/
TBOOL s1apWrapHandoverRequiredMsgPdu(
    IO S1apPdu                  **pdu,
    IN HandoverRequired         *handoverRequired
  );
#endif  /* S1AP_WRAP_H */

/**
 * @file    s1ap_unwrap.h
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @details
 * This file contains S1AP un-Wrap function prototypes
 * Unwrap functions are needed to De-couple the RRM applications from CCPU Stacks
 * These functions converts the data from CCPU structure to PWAV structures
 *
 * @author Lakshmi Narayana Madala
 *
 */

#ifndef S1AP_UNWRAP_H
#define S1AP_UNWRAP_H

#include "s1ap_cm.h"

void s1apUnwrapCriticalityDiagnostic(
    IN SztCriticalityDiag *critDiagPdu,
    IO CriticalDiagnostic *citDiag
    );
void s1apUnwrapUeRadioCapability(
    IN SztUERadioCapblty      *sztUERadioCapblty,
    IO UeRadioCapability      *ueRadioCapability
    );
void s1apUnwrapNasPdu(
    IN SztNAS_PDU  *s1NasPdu,
    IO NasPdu      *nasPdu
    );
void s1apUnwrapEUtranTraceId(
    IO EUtranTraceId         *eUtranTraceId,
    IN SztE_UTRAN_Trace_ID   *e_UTRAN_Trace_ID
    );
void s1apUnwrapPlmnId(
    IN SztPLMNidentity *plmnIdPdu,
    IO UINT8           *plmnId
    );
void s1apUnwrapServedPlmns(
    IN SztSrvdPLMNs  *servPlmnsPdu,
    IO ServedPlmns   *servPlmns
    );
void s1apUnwrapMmeCode(
    IN SztMME_Code  *mmeCodePdu,
    IO UINT8        *mmeCode
    );
void s1apUnwrapServedMmeCode(
    IN SztSrvdMMECs  *servMmeCodePdu,
    IO ServedMmes    *servMmeCode
    );
void s1apUnwrapMmeGrpId(
    IN SztMME_Group_ID  *mmeGrpIdPdu,
    IO UINT8            *mmeGrpId
    );
void s1apUnwrapServedMmeGrpId(
    IN SztSrvdGroupIDs  *servMmeGrpIdPdu,
    IO ServedMmeGrpIds  *servMmeGrpId
    );
void s1apUnwrapGummeIes(
    IN SztSrvdGUMMEIs      *servGummeIesPdu,
    IO ServedGummeies      *servGummeIes
    );
void s1apUnwrapCause(
    IN SztCause *causePdu,
    IO S1Cause  *cause
    );
void s1apUnwrapUeS1apId(
    IN SztUE_S1AP_IDs  *sztUE_S1AP_IDs,
    IO UeS1apId        *ueS1apId
    );
void s1apUnwrapTimetoWait(
    IN SztTimeToWait *timePdu,
    IO TimeToWait  *time
    );
void s1apUnwrapMmeName(
    IN SztMMEname *mmeNamePdu,
    IO UINT8      *mmeName
    );
void s1apUnwrapBitRate(
    IO BitRate      *bitRate,
    IN SztBitRate   *bitRatePdu
    );
void s1apUnwrapUeAggregateMaxBitRate(
    IN SztUEAggMaxBitrate   *sztUEAggMaxBitrate,
    IO UeAggMaxBitRate      *ueAggMaxBitRate
    );
void s1apUnwrapTransportLayerAddr(
    IO TransportLayerAddr    *transportLayerAddr,
    IN SztTportLyrAddr       *sztTportLyrAddr
    );
void s1apUnwrapAlgoList(
    IN TknBStr32     *suppAlgoPdu,
    IO UINT8         *suppAlgoBitMap
    );
void s1apUnwrapSecurityKey(
    IN SztSecurKey    *sztSecurKey,
    IO SecurityKey    *securityKey
    );
void s1apUnwrapUeSecurityCapability(
    IN SztUESecurCapabilities  *sztUESecurCapabilities,
    IO UeSecurityCapability    *ueSecurityCapability
    );
void s1apUnwrapTraceActivation(
    IN SztTraceActvn        *sztTraceActvn,
    IO TraceActivation      *traceActivation
    );
void s1apUnwrapGtpTeId(
    IO GtPTeId     *gtpTeid,
    IN SztGTP_TEID *gTP_TEID
    );
void s1apUnwrapGbrQosInfo(
    IN SztGBR_QosInform      *gbrQosInform,
    IO GbrQosInfo            *gbrQosInfo
    );
void s1apUnwrapAllocRetenPriority(
    IO AllocRetenPriority                  *allocRetenPriority,
    IN SztAllocnAndRetentionPriority       *allocationRetentionPriority
    );
void s1apUnwrapERabLevelQos(
    IO ERabLevelQosParams          *eRabLevelQosParams,
    IN SztE_RABLvlQoSParams        *e_RABlevelQoSParams
    );
void s1apUnwrapERabToSetupItem(
    IO ERabToSetupItem                *eRabToSetupItem,
    IN SztE_RABToBeSetupItemBrSUReq   *sztE_RABToBeSetupItemCtxtSUReq
    );
void s1apUnwrapInitCtxtERabToSetupItem(
    IO ERabToSetupItem                *eRabToSetupItem,
    IN SztE_RABToBeSetupItemCtxtSUReq *sztE_RABToBeSetupItemCtxtSUReq
    );
void s1apUnwrapERabToModifyItem(
    IO ERabToModifyItem                *eRabToModifyItem,
    IN SztE_RABToBeMdfdItemBrModReq    *sztE_RABToBeModifyItemSUReq
    );
void s1apUnwrapERabToReleaseItem(
    IO ERabReleaseItem               *eRabToReleaseItem,
    IN SztE_RABItem                    *sztE_RABToBeReleasedItem
    );
void s1apUnwrapERabToSetupList(
    IN SztE_RABToBeSetupLstBrSUReq     *rABToBeSetupLstSUReq,
    IO ERabToSetupList                 *eRabToSetupList
    );
void s1apUnwrapInitCtxtERabToSetupList(
    IN SztE_RABToBeSetupLstCtxtSUReq   *rABToBeSetupLstCtxtSUReq,
    IO ERabToSetupList                 *eRabToSetupList
    );
void s1apUnwrapERabToModifyList(
    IN SztE_RABToBeMdfdLstBrModReq      *rABToBeModifyLstSUReq,
    IO ERabToModifyList                 *eRabToModifyList
    );
void s1apUnwrapERabToReleaseList(
    IN SztE_RABLst                       *rABToBeReleasesLst,
    IO ERabReleaseList                 *eRabToReleaseList
    );
void s1apUnwrapUeAssocLogConnItem(
    IO S1UeAssocLogConnItem         *ueAssocLogConItem,
    IN SztUE_assocLogS1_ConItem     *sztueLogAssocItem
    );
void s1apUnwrapUeAssocLogConList(
    IN SztUE_assocLogS1_ConLstRes   *ueAssocLogConListPdu,
    IO S1UeAssocLogConnList         *ueAssocLogConList
    );
void extractMcc(
  IN SztPLMNidentity         *plmnIdentity,
  IO U8                      *pagingMcc
  );
void extractMnc(
  IN SztPLMNidentity         *plmnIdentity,
  IO U8                      *pagingMnc,
  IO TBOOL                   *fillerBitInMnc
  );
void extractTac(
  IN SztTAC      *tac,
  IO U16         *pagingTac
  );


TBOOL s1apUnwrapSetupResPdu
  (
    IO S1apPdu      *pdu,
    IO S1SetupRsp   *setupRsp
  );
TBOOL s1apUnwrapSetupFailPdu
  (
    IO S1apPdu       *pdu,
    IO S1SetupFail   *setupFail
  );
TBOOL s1apUnwrapResetAckPdu
  (
    IO S1apPdu       *pdu,
    IO S1ResetAck    *resetAck
  );
TBOOL s1apUnwrapDlNasPdu
  (
    IO S1apPdu       *pdu,
    IO DlNasMessage  *dlNasMsg
  );
TBOOL s1apUnwrapInitialContextSetupPdu(
    IO S1apPdu              *pdu,
    IO InitialContextSetup  *initialContextSetup
  );
TBOOL s1apUnwrapUeContextRelCmdPdu(
    IO S1apPdu          *pdu,
    IO UeContextRelCmd  *ueCtxRelMsg
  );
TBOOL s1apUnwrapS1ResetReqPdu(
    IO S1apPdu          *pdu,
    IO S1ResetReq       *resetReqMsg
  );
TBOOL  s1apUnwrapS1PagingReqPdu(
    IO S1apPdu          *s1apPdu,
    IO S1PagingInfo     *pagingInfo
  );
TBOOL s1apUnwrapMmeConfigUpdatePdu(
    IO S1apPdu             *pdu,
    IO MmeCfgUpdate        *mmeConfigUpdate
  );
TBOOL s1apUnwrapLocReportCntrlPdu(
  IO S1apPdu            *pdu,
  IO LocReqReportCntrl  *locReqReportCntrl
);
TBOOL s1apUnwrapTraceStartPdu(
    IO S1apPdu              *pdu,
    IO TraceStart           *traceStart
  );
TBOOL s1apUnwrapDeactivateTracePdu(
    IO S1apPdu              *pdu,
    IO DeactivateTrace      *deactivateTrace
  );
TBOOL s1apUnwrapERabSetupReqPdu(
    IO S1apPdu              *pdu,
    IO ERabSetupReq         *eRabSetupReq
  );
TBOOL s1apUnwrapOverloadStartPdu  (
    IO S1apPdu             *pdu,
    IO OverloadStart       *overloadStart
  );
TBOOL s1apUnwrapERabModifyReqPdu(
    IO S1apPdu              *pdu,
    IO ERabModifyReq        *eRabModifyReq
  );
TBOOL s1apUnwrapERabReleaseCmdPdu(
    IO S1apPdu              *pdu,
    IO ERabReleaseReq        *eRabReleaseReq
  );
TBOOL s1apUnwrapUeContextModReqPdu(
    IO S1apPdu                     *pdu,
    IO UeContextModReq             *ueContextModReq
  );

TBOOL s1apUnWrapHandoverCommandMsgPdu(
  IO S1apPdu                  *pdu,
  IO HandoverCmd              *handoverCmd
  );
TBOOL s1apUnWrapHandoverPrepFailedMsgPdu(
  IO S1apPdu                  *pdu,
  IO HandoverPrepFailed       *handoverPrepFailed
  );
TBOOL s1apUnwrapErrInd( 
  IO S1apPdu                *pdu, 
  IO ErrInd                 *errInd 
  );
TBOOL s1apUnwrapMmeWriteReplaceWarningReq(
  IO S1apPdu               *pdu,
  IO WriteReplcWarnReq     *writeReplcWarnReq
  );
#endif /* S1AP_UNWRAP_H */

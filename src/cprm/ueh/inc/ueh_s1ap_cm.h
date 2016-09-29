/**
 * @file    ueh_s1ap_cm.h
 * @brief This file contains S1AP common Procedures Used by different modules
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author :           Lakshmi Narayana Madala
 * Date   :           06-Oct-2010
 * Change History :
 *  Shweta Polepally :30th Aug 2011 : Added Code under HO_SUPPORTED
 */


#ifndef UEH_S1AP_CM_H
#define UEH_S1AP_CM_H

#include "ccpu_cmn.h"

/* RRM related includes */
#include "lueh.h"
#include "lueh.x"
#include "ueh_eut_x.h"
#include "ueh.h"
#include "s1ap_cm.h"
#include "bs_err.h"
#include "s1ap.h"
#include "imc_err.h"

#define getNasPduLenFromRrcConSetupRsp( datIndSdu )    (datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.rrcConSetupCompl.\
                                                        criticalExtns.val.c1.val.rrcConSetupCompl_r8.dedicatedInfoNAS.len );
#define getNasPduFromRrcConSetupRsp( datIndSdu )       (datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.rrcConSetupCompl.\
                                                        criticalExtns.val.c1.val.rrcConSetupCompl_r8.dedicatedInfoNAS.val )

#define getNasPduLenFromUlInfoTransfer( datIndSdu )    (datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.ulInfoTfr.\
                                                         criticalExtns.val.c1.val.ulInfoTfr_r8.dedicatedInfoTyp.val.dedicatedInfoNAS.len );
#define getNasPduFromUlInfoTransfer( datIndSdu )       (datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.ulInfoTfr.\
                                                          criticalExtns.val.c1.val.ulInfoTfr_r8.dedicatedInfoTyp.val.dedicatedInfoNAS.val )

#define fillPeerId( uDatEvnt, pId)             {  \
                                                      uDatEvnt.peerId.pres = PRSNT_NODEF; \
                                                      uDatEvnt.peerId.val = pId; \
                                                }
#define fillTransId( uDatEvnt, tId)             {  \
                                                      uDatEvnt.transId.pres = PRSNT_NODEF; \
                                                      uDatEvnt.transId.val = tId; \
                                                    }

UINT16 uehGetMaxS1apConnAllowed(void);
void uehFillS1apInitMsg(
    InitialUeMessage *initialUeMsg,
    UehUeCb          *ueCb,
    TBOOL            includeGummeiIe,
    TBOOL            plmnIdIeRcvd,
    UeGummei         *gummei
    );
UINT32 uehGetPeerIdForUeFromUeCb(
    UehUeCb          *ueCb
    );
void uehFillUlNasMsg(
    UlNasMessage     *ulNasMsg,
    UehS1ConCb       *s1ConCb,
    NhuDatIndSdus    *datIndSdu
    );
void uehFillUeCtxRelReq(
    UeContextRelReq     *ueXtxRelReqMsg,
    UehS1ConCb          *s1ConCb,
    S1Cause             *cause
    );
void uehFillResetReqOamInt(
    S1ResetReq     *resetReqMsg,
    UehS1ConCb     *s1ConCb
    );
void uehFillUeCtxRelCmpl(
    UeContextRelCmp     *ueCtxRelCmplMsg,
    UehS1ConCb          *s1ConCb
    );
void uehFillUeCtxModRsp(
    UeContextModRsp             *ueCtxModRsp,
    UehS1ConCb                  *s1ConCb
    );
void uehFillUeCtxModFail(
    UeContextModFail            *ueCtxModFailMsg,
    UehS1ConCb                  *s1ConCb,
    S1Cause                     *cause
    );
void uehFillInitCtxSetupFail(
    InitialContextSetupFail     *initCtxSetupFailMsg,
    UehS1ConCb                  *s1ConCb,
    S1Cause                     *cause
    );
void uehFillInitCtxSetupRsp(
    InitialContextSetupRsp      *initCtxSetupRspMsg,
    UehUeCb                     *ueCb
    );
void uehFillNasNonDeliveryIndMsg(
    NasNonDeliveryInd   *nasNonDeliveryIndMsg,
    UehS1ConCb          *s1ConCb,
    U16                  nasPduLen,
    U8                  *nasPdu
    );
void uehFillUeCapInfoIndMsg(
    UehUeCb             *ueCb,
    UeCapInfoInd        *ueCapInfoInd
  );
void uehFillUeLocReportFailInd(
  LocReqReportCntrl  *locReqReportCntrl,
  LocReqReportFail   *locReqReportFail,
  S1Cause            *cause
  );
void uehFillUeLocReportSucc(
  LocReqReportCntrl  *locReqReportCntrl,
  LocReqReport       *locReqReport
  );
void uehFillTraceFailInd(
  UehS1ConCb            *s1ConCb,
  EUtranTraceId         *eUtranTraceId,
  TraceFailInd          *traceFailInd,
  S1Cause               *cause
  );
void uehFillERabSetupRsp(
  ERabSetupReq          *eRabSetupReq,
  ERabSetupRsp          *eRabSetupRsp,
  S1Cause               *cause
  );
void uehFillERabModifyRsp(
    ERabModifyReq          *eRabModifyReq,
    ERabModifyRsp          *eRabModifyRsp,
    S1Cause                *cause,
    UehUeCb                *ueCb
  );
void uehFillERabReleaseRsp(
    ERabReleaseReq          *eRabReleaseReq,
    ERabReleaseRsp          *eRabReleaseRsp,
    S1Cause                 *cause,
    UehUeCb                 *ueCb
  );
S1apMgrCb *uehGetS1CbFromTmsi(
    UehSTmsi     *sTmsi
    );
S1apMgrCb *uehGetS1CbFromSelectedPlmnId(
    U8  selectedPlmnId
    );
PUBLIC S16 uehCheckMmeOverload(
    U32        establishCause,
    U16        ueId /* for logging */
    );
void uehFillSrcToTargetTransparentContainer(
  IO SrcToTargetTransparentContainer *srcToTargetTransparentContainer,
  IN UehUeHovrInfo                   *ueHovrInfo
  );
/* S1AP handover related function prototypes */
void uehFillHandoverRequired(
  IO HandoverRequired  *handoverRequired,
  UehUeCb              *ueCb,
  S1Cause              *cause,
  S1apPdu              *pdu
  );
TBOOL uehIsRabIdValid(
  UehUeCb             *ueCb,
  U32                  eRABId
  );

#endif /* UEH_S1AP_CM_H */

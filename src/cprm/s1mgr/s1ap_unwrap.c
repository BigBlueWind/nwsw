/**
 * @file    s1ap_unwrap.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @details
 * This file contains S1AP un-Wrap functions
 * Unwrap functions are needed to De-couple the RRM applications from CCPU Stacks
 * These functions converts the data from CCPU structure to PWAV structures
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
#include "s1ap_cm.h"
#include "s1ap_unwrap.h"
#include "log.h"

/**
 * @param   critDiagPdu        Critical diagnostics PDU from Peer
 * @param   citDiag            Critical diagnostics structure
 * @details
 * This functions extracts the Critical diagnostics from critDiagPdu and fills it
 * in the structure citDiag
 */
void s1apUnwrapCriticalityDiagnostic(
    IN SztCriticalityDiag *critDiagPdu,
    IO CriticalDiagnostic *citDiag
    )
{
  UINT16  indx=0;
  SztCriticalityDiag_IE_Item *critIeItem;

  if( PRSNT_NODEF == critDiagPdu->procedureCode.pres )
    citDiag->procedureCode = critDiagPdu->procedureCode.val;
  if( PRSNT_NODEF == critDiagPdu->triggeringMsg.pres )
    citDiag->triggeringMsg = critDiagPdu->triggeringMsg.val;
  if( PRSNT_NODEF == critDiagPdu->procedureCriticality.pres )
    citDiag->procedureCriticality = critDiagPdu->procedureCriticality.val;

  citDiag->noOfIeCriticalDiagnostics = critDiagPdu->iEsCriticalityDiag.noComp.val;
  for( indx=0; indx < citDiag->noOfIeCriticalDiagnostics ; indx++ )
  {
    critIeItem = &(critDiagPdu->iEsCriticalityDiag.member[indx]);
    citDiag->ieCriticalDiagnostic[indx].ieCriticality = critIeItem->iECriticality.val;
    citDiag->ieCriticalDiagnostic[indx].ieId = critIeItem->iE_ID.val;
    citDiag->ieCriticalDiagnostic[indx].typeOfError = critIeItem->typeOfErr.val;
  }
}

/**
 * @param   sztUERadioCapblty        UE Radio Capability PDU
 * @param   ueRadioCapability        UE Radio Capability
 * @details
 * This functions extracts the UE Radio Capability from sztUERadioCapblty and fills it
 * in the structure ueRadioCapability
 */
void s1apUnwrapUeRadioCapability(
    IN SztUERadioCapblty      *sztUERadioCapblty,
    IO UeRadioCapability      *ueRadioCapability
    )
{
  if( sztUERadioCapblty->len > MAX_SIZE_UE_RADIO_CAPABILITY )
  {
    logError("Len is more than :%dBytes", MAX_SIZE_UE_RADIO_CAPABILITY );
  }
  else
  {
    ueRadioCapability->len = sztUERadioCapblty->len;
    copyBytes( sztUERadioCapblty->val, sztUERadioCapblty->len, ueRadioCapability->UeRadioCapability );
  }
}

/**
 * @param   s1NasPdu      NAS PDU
 * @param   nasPdu        NAS message
 * @details
 * This functions extracts the NAS message from s1NasPdu and fills it
 * in the structure nasPdu
 */
void s1apUnwrapNasPdu(
    IN SztNAS_PDU  *s1NasPdu,
    IO NasPdu      *nasPdu
    )
{
  if( s1NasPdu->len > MAX_SIZE_NAS_PDU )
  {
    logError("Len is more than :%dBytes", MAX_SIZE_NAS_PDU );
  }
  else
  {
    nasPdu->pduLen = s1NasPdu->len;
    copyBytes( s1NasPdu->val, s1NasPdu->len, nasPdu->pdu );
  }
}

/**
 * @param   e_UTRAN_Trace_ID     E-UTRAN Trace ID PDU
 * @param   eUtranTraceId        E-UTRAN Trace ID
 * @details
 * This functions extracts the E-UTRAN Trace ID from e_UTRAN_Trace_ID PDU and fills it
 * in the structure eUtranTraceId
 */
void s1apUnwrapEUtranTraceId(
    IO EUtranTraceId         *eUtranTraceId,
    IN SztE_UTRAN_Trace_ID   *e_UTRAN_Trace_ID
    )
{
  copyBytes( e_UTRAN_Trace_ID->val, EUTRAN_TRACE_ID_LEN, eUtranTraceId->eUtranTraceId );
}

/**
 * @param   plmnIdPdu     PLMN ID PDU
 * @param   plmnId        PLMN TID
 * @details
 * This functions extracts the PLMN ID from plmnIdPdu PDU and fills it
 * in the structure plmnId
 */
void s1apUnwrapPlmnId(
    IN SztPLMNidentity *plmnIdPdu,
    IO UINT8           *plmnId
    )
{
  copyBytes( plmnIdPdu->val, SIZE_PLMNIDENTITY, plmnId );
}

/**
 * @param   servPlmnsPdu     Served PLMN ID PDU
 * @param   servPlmns        Served PLMN TID
 * @details
 * This functions extracts the Served PLMN ID from servPlmnsPdu PDU and fills it
 * in the structure servPlmns
 */
void s1apUnwrapServedPlmns(
    IN SztSrvdPLMNs  *servPlmnsPdu,
    IO ServedPlmns   *servPlmns
    )
{
  UINT16 indx=0;

  servPlmns->numPlmns = servPlmnsPdu->noComp.val;
  for( indx=0; indx < servPlmns->numPlmns ; indx++ )
  {
    s1apUnwrapPlmnId( &(servPlmnsPdu->member[indx]), (servPlmns->plmnId[indx])  );
  }
}

/**
 * @param   mmeCodePdu     MME Code PDU
 * @param   mmeCode        MME Code
 * @details
 * This functions extracts the MME Code from mmeCodePdu PDU and fills it
 * in the structure mmeCode
 */
void s1apUnwrapMmeCode(
    IN SztMME_Code  *mmeCodePdu,
    IO UINT8        *mmeCode
    )
{
  copyBytes( mmeCodePdu->val, SIZE_MMECODE, mmeCode );
}

/**
 * @param   servMmeCodePdu     Served MME Code PDU
 * @param   servMmeCode        Served MME Code
 * @details
 * This functions extracts the Served MME Code from servMmeCodePdu PDU and fills it
 * in the structure servMmeCode
 */
void s1apUnwrapServedMmeCode(
    IN SztSrvdMMECs  *servMmeCodePdu,
    IO ServedMmes    *servMmeCode
    )
{
  UINT16 indx=0;

  servMmeCode->numMme = servMmeCodePdu->noComp.val;
  for( indx=0; indx < servMmeCode->numMme ; indx++ )
  {
    s1apUnwrapMmeCode( &(servMmeCodePdu->member[indx]), (servMmeCode->mmeId[indx] ) );;
  }
}

/**
 * @param   mmeGrpIdPdu     MME Group ID PDU
 * @param   mmeGrpId        MME Group ID
 * @details
 * This functions extracts the MME Group ID from mmeGrpIdPdu PDU and fills it
 * in the structure mmeGrpId
 */
void s1apUnwrapMmeGrpId(
    IN SztMME_Group_ID  *mmeGrpIdPdu,
    IO UINT8            *mmeGrpId
    )
{
  copyBytes( mmeGrpIdPdu->val, SIZE_MMEGROUPID, mmeGrpId );
}

/**
 * @param   servMmeGrpIdPdu     Served MME Group ID PDU
 * @param   servMmeGrpId        Served MME Group ID
 * @details
 * This functions extracts the Served MME Group ID from servMmeGrpIdPdu PDU and fills it
 * in the structure servMmeGrpId
 */
void s1apUnwrapServedMmeGrpId(
    IN SztSrvdGroupIDs  *servMmeGrpIdPdu,
    IO ServedMmeGrpIds  *servMmeGrpId
    )
{
  UINT16 indx=0;

  servMmeGrpId->numMmeGrp = servMmeGrpIdPdu->noComp.val;
  for( indx=0; indx < servMmeGrpId->numMmeGrp ; indx++ )
  {
    s1apUnwrapMmeGrpId( &(servMmeGrpIdPdu->member[indx] ), (servMmeGrpId->mmeGroupId[indx] ) );
  }
}

/**
 * @param   servGummeIesPdu     GUMMEI IE's PDU
 * @param   servGummeIes        GUMMEI IE's
 * @details
 * This functions extracts the GUMMEI IE's from servGummeIesPdu PDU and fills it
 * in the structure servGummeIes
 */
void s1apUnwrapGummeIes(
    IN SztSrvdGUMMEIs      *servGummeIesPdu,
    IO ServedGummeies      *servGummeIes
    )
{
  UINT16  indx=0;
  SztSrvdGUMMEIsItem *mmeItem;

  servGummeIes->numOfGummeies = servGummeIesPdu->noComp.val;
  for( indx=0; indx< servGummeIes->numOfGummeies; indx++ )
  {
    mmeItem = &(servGummeIesPdu->member[indx]);
    s1apUnwrapServedPlmns( &(mmeItem->servedPLMNs),
                           &(servGummeIes->servedGummeIe[indx].servedPlmns) );
    s1apUnwrapServedMmeGrpId( &(mmeItem->servedGroupIDs),
                              &(servGummeIes->servedGummeIe[indx].servedMmeGrpIds) );
    s1apUnwrapServedMmeCode( &(mmeItem->servedMMECs),
                             &(servGummeIes->servedGummeIe[indx].servedMmes) );
  }

}

/**
 * @param   causePdu     Cause PDU
 * @param   cause        Cause
 * @details
 * This functions extracts the Cause from causePdu PDU and fills it
 * in the structure cause
 */
void s1apUnwrapCause(
    IN SztCause *causePdu,
    IO S1Cause  *cause
    )
{
  cause->causeType = causePdu->choice.val;
  switch(causePdu->choice.val)
  {
    case CAUSE_RADIONW:
      cause->causeVal = causePdu->val.radioNw.val;
      break;
    case CAUSE_TRANSPORT:
      cause->causeVal = causePdu->val.transport.val;
      break;
    case CAUSE_NAS:
      cause->causeVal = causePdu->val.nas.val;
      break;
    case CAUSE_PROTOCOL:
      cause->causeVal = causePdu->val.protocol.val;
      break;
    case CAUSE_MISC:
      cause->causeVal = causePdu->val.misc.val;
      break;
  }

}

/**
 * @param   sztUE_S1AP_IDs  UE-S1AP ID PDU
 * @param   ueS1apId        UE-S1AP ID
 * @details
 * This functions extracts the UE-S1AP ID from sztUE_S1AP_IDs PDU and fills it
 * in the structure ueS1apId
 */
void s1apUnwrapUeS1apId(
    IN SztUE_S1AP_IDs  *sztUE_S1AP_IDs,
    IO UeS1apId        *ueS1apId
    )
{
  ueS1apId->choice = sztUE_S1AP_IDs->choice.val;
  switch(ueS1apId->choice)
  {
    case UeS1apIdChoice_MmeUeS1apId:
      ueS1apId->u.mmeUeS1apId = sztUE_S1AP_IDs->val.mME_UE_S1AP_ID.val;
      break;
    case UeS1apIdChoice_UeIdPair:
      ueS1apId->u.ueS1apIdPair.enbUeS1apId= sztUE_S1AP_IDs->val.uE_S1AP_ID_pair.eNB_UE_S1AP_ID.val;
      ueS1apId->u.ueS1apIdPair.mmeUeS1apId= sztUE_S1AP_IDs->val.uE_S1AP_ID_pair.mME_UE_S1AP_ID.val;
      break;
  }

}

/**
 * @param   timePdu  Time to Wait PDU
 * @param   timeVal  Time to Wait
 * @details
 * This functions extracts the Time to Wait( before retry of S1 Setup) from timePdu PDU and fills it
 * in the structure timeVal
 */
void s1apUnwrapTimetoWait(
    IN SztTimeToWait *timePdu,
    IO TimeToWait    *timeVal
    )
{
  *timeVal = timePdu->val;
}

/**
 * @param   mmeNamePdu  MME Name PDU
 * @param   mmeName     MME Name
 * @details
 * This functions extracts the MME Name from mmeNamePdu PDU and fills it
 * in the structure mmeName
 */
void s1apUnwrapMmeName(
    IN SztMMEname *mmeNamePdu,
    IO UINT8      *mmeName
    )
{
  if( mmeNamePdu->len > MAX_SIZE_MMENAME )
  {
    logError("Len is more than :%dBytes", MAX_SIZE_MMENAME );
  }
  else
  {
    copyBytes( mmeNamePdu->val, mmeNamePdu->len, mmeName );
  }
}

/**
 * @param   bitRatePdu  Bit Rate PDU
 * @param   bitRate     Bit Rate
 * @details
 * This functions extracts the Bit Rate from bitRatePdu PDU and fills it
 * in the structure bitRate
 */
void s1apUnwrapBitRate(
    IO BitRate      *bitRate,
    IN SztBitRate   *bitRatePdu
    )
{
  if( bitRatePdu->len > BITRATE_LEN )
  {
    logError("BitRate is more than :%dBytes", BITRATE_LEN );
  }
  else
  {
    copyBytes(bitRatePdu->val, bitRatePdu->len, (bitRate->bitRate + (BITRATE_LEN - 1) - bitRatePdu->len));
  }
}

/**
 * @param   sztUEAggMaxBitrate  UE Maximum Aggregate Bit Rate PDU
 * @param   ueAggMaxBitRate     UE Maximum Aggregate Bit Rate
 * @details
 * This functions extracts the UE Maximum Aggregate Bit Rate from sztUEAggMaxBitrate PDU and fills it
 * in the structure ueAggMaxBitRate
 */
void s1apUnwrapUeAggregateMaxBitRate(
    IN SztUEAggMaxBitrate   *sztUEAggMaxBitrate,
    IO UeAggMaxBitRate      *ueAggMaxBitRate
    )
{
  s1apUnwrapBitRate( &(ueAggMaxBitRate->ueAggMaxBitRateDl), &(sztUEAggMaxBitrate->uEaggregateMaxBitRateDL) );
  s1apUnwrapBitRate( &(ueAggMaxBitRate->ueAggMaxBitRateUl), &(sztUEAggMaxBitrate->uEaggregateMaxBitRateUL) );
}

/**
 * @param   transportLayerAddr  Transport Layer Address PDU
 * @param   sztTportLyrAddr     Transport Layer Address
 * @details
 * This functions extracts the Transport Layer Address from transportLayerAddr PDU and fills it
 * in the structure sztTportLyrAddr
 */
void s1apUnwrapTransportLayerAddr(
    IO TransportLayerAddr    *transportLayerAddr,
    IN SztTportLyrAddr       *sztTportLyrAddr
    )
{
  UINT16 len=0;

  len = sztTportLyrAddr->len;
  switch( len)
  {
    case 32 :
      transportLayerAddr->transportAddrType = TrasportAddrType_Ipv4;
      break;
    case 128 :
      transportLayerAddr->transportAddrType = TrasportAddrType_Ipv6;
      break;
    default :
      transportLayerAddr->transportAddrType = TrasportAddrType_Both;
      break;
  }

  /* Convert length to Bytes */
  len = len / 8;

  copyBytes( sztTportLyrAddr->val, len, transportLayerAddr->transportLevelAddr );
}

/**
 * @param   suppAlgoPdu         Algorithm Supported PDU
 * @param   suppAlgoBitMap      Algorithm Supported
 * @details
 * This functions extracts the Security Algorithm Supported at UE from suppAlgoPdu PDU and fills it
 * in the structure suppAlgoBitMap
 */
void s1apUnwrapAlgoList(
    IN TknBStr32     *suppAlgoPdu,
    IO UINT8         *suppAlgoBitMap
    )
{
  copyBytes( suppAlgoPdu->val, UE_SECURITY_CAP_SUPP_ALGO_LEN, suppAlgoBitMap );
}

/**
 * @param   sztSecurKey      Security Key PDU
 * @param   securityKey      Security Key KeNB
 * @details
 * This functions extracts the Security Key KeNB for UE from sztSecurKey PDU and fills it
 * in the structure securityKey
 */
void s1apUnwrapSecurityKey(
    IN SztSecurKey    *sztSecurKey,
    IO SecurityKey    *securityKey
    )
{
  copyBytes( sztSecurKey->val, SECURITY_KEY_LEN, securityKey->securityKey );
}

/**
 * @param   sztUESecurCapabilities    UE Security Capabilities PDU
 * @param   ueSecurityCapability      UE Security Capabilities
 * @details
 * This functions extracts the Security Capabilities of UE from sztUESecurCapabilities PDU and fills it
 * in the structure ueSecurityCapability
 */
void s1apUnwrapUeSecurityCapability(
    IN SztUESecurCapabilities  *sztUESecurCapabilities,
    IO UeSecurityCapability    *ueSecurityCapability
    )
{
  s1apUnwrapAlgoList( &(sztUESecurCapabilities->encryptionAlgorithms), (ueSecurityCapability->encryptionAlgorithms) );
  s1apUnwrapAlgoList( &(sztUESecurCapabilities->integrityProtectionAlgorithms), (ueSecurityCapability->integrityAlgorithms) );
}

/**
 * @param   sztTraceActvn    Trace Activation PDU
 * @param   traceActivation  Trace Activation
 * @details
 * This functions extracts the Trace Activation of UE from sztTraceActvn PDU and fills it
 * in the structure traceActivation
 */
void s1apUnwrapTraceActivation(
    IN SztTraceActvn        *sztTraceActvn,
    IO TraceActivation      *traceActivation
    )
{
  s1apUnwrapEUtranTraceId( &(traceActivation->eUtranTraceId), &(sztTraceActvn->e_UTRAN_Trace_ID) );
  traceActivation->interfaceToTrace = sztTraceActvn->interfacesToTrace.val[0];  /* This is a 8 bit string */
  traceActivation->traceDepth       = sztTraceActvn->traceDepth.val;
  s1apUnwrapTransportLayerAddr( &(traceActivation->transportLayerAddr), &(sztTraceActvn->traceCollectionEntityIPAddr) );
}

/**
 * @param   gTP_TEID  GTP Tunnel ID PDU
 * @param   gtpTeid   GTPTEID
 * @details
 * This functions extracts the GTP Tunnel ID of SGW from gTP_TEID PDU and fills it
 * in the structure gtpTeid
 */
void s1apUnwrapGtpTeId(
    IO GtPTeId     *gtpTeid,
    IN SztGTP_TEID *gTP_TEID
    )
{
  copyBytes( gTP_TEID->val, GTP_TE_ID_LEN, gtpTeid->gtPTeId );
}

/**
 * @param   gbrQosInform Guaranteed Bit Rate PDU
 * @param   gbrQosInfo   Guaranteed Bit Rate.
 * @details
 * This functions extracts the Guaranteed Bit Rate for the UE from gbrQosInform PDU and fills it
 * in the structure gbrQosInfo
 */
void s1apUnwrapGbrQosInfo(
    IN SztGBR_QosInform      *gbrQosInform,
    IO GbrQosInfo            *gbrQosInfo
    )
{
  s1apUnwrapBitRate( &(gbrQosInfo->ueMaxBitRateDl), &(gbrQosInform->e_RAB_MaxBitrateDL) );
  s1apUnwrapBitRate( &(gbrQosInfo->ueMaxBitRateUl), &(gbrQosInform->e_RAB_MaxBitrateUL) );

  s1apUnwrapBitRate( &(gbrQosInfo->ueGurantedBitRateDl), &(gbrQosInform->e_RAB_GuaranteedBitrateDL) );
  s1apUnwrapBitRate( &(gbrQosInfo->ueGurantedBitRateDl), &(gbrQosInform->e_RAB_GuaranteedBitrateDL) );
}

/**
 * @param   allocationRetentionPriority
 * @param   allocRetenPriority
 * @details
 * This functions extracts the Allocation Retention Priority for the UE from allocationRetentionPriority PDU and fills it
 * in the structure allocRetenPriority
 */
void s1apUnwrapAllocRetenPriority(
    IO AllocRetenPriority                  *allocRetenPriority,
    IN SztAllocnAndRetentionPriority       *allocationRetentionPriority
    )
{
  allocRetenPriority->priorityLevel = allocationRetentionPriority->priorityLvl.val;
  allocRetenPriority->preemptionCability = allocationRetentionPriority->pre_emptionCapblty.val;
  allocRetenPriority->PreemptionVulnerability = allocationRetentionPriority->pre_emptionVulnerability.val;
}

/**
 * @param   eRabLevelQosParams  E-RAB QOS PDU from MME
 * @param   e_RABlevelQoSParams E-RAB QOS
 * @details
 * This functions extracts the E-RAB QOS for a given RAB in UE from eRabLevelQosParams PDU and fills it
 * in the structure e_RABlevelQoSParams
 */
void s1apUnwrapERabLevelQos(
    IO ERabLevelQosParams          *eRabLevelQosParams,
    IN SztE_RABLvlQoSParams        *e_RABlevelQoSParams
    )
{
  eRabLevelQosParams->qci = e_RABlevelQoSParams->qCI.val;
  s1apUnwrapAllocRetenPriority( &(eRabLevelQosParams->allocRetenPriority), &(e_RABlevelQoSParams->allocationRetentionPriority) );
  if( TRUE == e_RABlevelQoSParams->gbrQosInform.pres.val )
  {
    eRabLevelQosParams->isGbrQosInfo = TRUE;
    s1apUnwrapGbrQosInfo( &(e_RABlevelQoSParams->gbrQosInform), &(eRabLevelQosParams->gbrQosInfo) );
  }
}

/**
 * @param   sztE_RABToBeSetupItemCtxtSUReq      E-RAB Setup Item PDU
 * @param   eRabSubToFwdItem                     E-RAB Setup
 * @details
 * This functions extracts the E-RAB Setup Item for a given RAB in UE from sztE_RABToBeSetupItemCtxtSUReq PDU and fills it
 * in the structure eRabSubToFwdItem
 */
void s1apUnwrapERabToSetupItem(
    IO ERabToSetupItem                *eRabToSetupItem,
    IN SztE_RABToBeSetupItemBrSUReq   *sztE_RABToBeSetupItemCtxtSUReq
    )
{
  eRabToSetupItem->eRabId       = sztE_RABToBeSetupItemCtxtSUReq->e_RAB_ID.val;
  s1apUnwrapERabLevelQos( &(eRabToSetupItem->eRabLevelQosParams), &(sztE_RABToBeSetupItemCtxtSUReq->e_RABlevelQoSParams) );
  s1apUnwrapTransportLayerAddr( &(eRabToSetupItem->transportLayerAddr), &(sztE_RABToBeSetupItemCtxtSUReq->transportLyrAddr) );
  s1apUnwrapGtpTeId( &(eRabToSetupItem->gtPTeId), &(sztE_RABToBeSetupItemCtxtSUReq->gTP_TEID) );
  eRabToSetupItem->isNasPdu = TRUE;
  s1apUnwrapNasPdu( &(sztE_RABToBeSetupItemCtxtSUReq->nAS_PDU), &(eRabToSetupItem->nasPdu) );
}

/**
 * @param   sztE_RABToBeSetupItemCtxtSUReq      E-RAB Initial Context Setup Item PDU
 * @param   eRabSubToFwdItem                     E-RAB Setup
 * @details
 * This functions extracts the E-RAB Initial Context Setup Item for a given RAB in UE from sztE_RABToBeSetupItemCtxtSUReq PDU and fills it
 * in the structure eRabSubToFwdItem
 */
void s1apUnwrapInitCtxtERabToSetupItem(
    IO ERabToSetupItem                *eRabToSetupItem,
    IN SztE_RABToBeSetupItemCtxtSUReq *sztE_RABToBeSetupItemCtxtSUReq
    )
{
  eRabToSetupItem->eRabId       = sztE_RABToBeSetupItemCtxtSUReq->e_RAB_ID.val;
  s1apUnwrapERabLevelQos( &(eRabToSetupItem->eRabLevelQosParams), &(sztE_RABToBeSetupItemCtxtSUReq->e_RABlevelQoSParams) );
  s1apUnwrapTransportLayerAddr( &(eRabToSetupItem->transportLayerAddr), &(sztE_RABToBeSetupItemCtxtSUReq->transportLyrAddr) );
  s1apUnwrapGtpTeId( &(eRabToSetupItem->gtPTeId), &(sztE_RABToBeSetupItemCtxtSUReq->gTP_TEID) );
  if( TRUE == sztE_RABToBeSetupItemCtxtSUReq->nAS_PDU.pres )
  {
    eRabToSetupItem->isNasPdu = TRUE;
    s1apUnwrapNasPdu( &(sztE_RABToBeSetupItemCtxtSUReq->nAS_PDU), &(eRabToSetupItem->nasPdu) );
  }
}

/**
 * @param   sztE_RABToBeModifyItemSUReq      E-RAB Modify Item PDU
 * @param   eRabToModifyItem                 E-RAB Modify
 * @details
 * This functions extracts the E-RAB Modify Item for a given RAB in UE from sztE_RABToBeModifyItemSUReq PDU and fills it
 * in the structure eRabToModifyItem
 */
void s1apUnwrapERabToModifyItem(
    IO ERabToModifyItem                *eRabToModifyItem,
    IN SztE_RABToBeMdfdItemBrModReq    *sztE_RABToBeModifyItemSUReq
    )
{
  eRabToModifyItem->eRabId       = sztE_RABToBeModifyItemSUReq->e_RAB_ID.val;
  s1apUnwrapERabLevelQos( &(eRabToModifyItem->eRabLevelQosParams), &(sztE_RABToBeModifyItemSUReq->e_RABLvlQoSParams) );
  s1apUnwrapNasPdu( &(sztE_RABToBeModifyItemSUReq->nAS_PDU), &(eRabToModifyItem->nasPdu) );
}

/**
 * @param   sztE_RABToBeReleasedItem      E-RAB Release Item PDU
 * @param   eRabToReleaseItem             E-RAB Release
 * @details
 * This functions extracts the E-RAB Release Item for a given RAB in UE from sztE_RABToBeReleasedItem PDU and fills it
 * in the structure eRabToReleaseItem
 */
void s1apUnwrapERabToReleaseItem(
    IO ERabReleaseItem               *eRabToReleaseItem,
    IN SztE_RABItem                  *sztE_RABToBeReleasedItem
    )
{
  eRabToReleaseItem->eRabId       = sztE_RABToBeReleasedItem->e_RAB_ID.val;
  s1apUnwrapCause( &(sztE_RABToBeReleasedItem->cause), &(eRabToReleaseItem->cause) );
}

/**
 * @param   rABToBeSetupItemSUReqIEs    E-RAB Setup List PDU
 * @param   eRabToSetupList             E-RAB Setup List
 * @details
 * This functions extracts the E-RAB Setup List for a UE from rABToBeSetupItemSUReqIEs PDU and fills it
 * in the structure eRabToSetupList
 */
void s1apUnwrapERabToSetupList(
    IN SztE_RABToBeSetupLstBrSUReq     *rABToBeSetupLstSUReq,
    IO ERabToSetupList                 *eRabToSetupList
    )
{
  UINT16 indx;
  SztProtIE_SingleCont_E_RABToBeSetupItemBrSUReqIEs *rABToBeSetupItemSUReqIEs;

  eRabToSetupList->noComp = rABToBeSetupLstSUReq->noComp.val;
  for( indx=0; indx< eRabToSetupList->noComp; indx++ )
  {
    rABToBeSetupItemSUReqIEs = ( rABToBeSetupLstSUReq->member + indx);
    s1apUnwrapERabToSetupItem( &(eRabToSetupList->eRabToSetupItem[indx] ), &(rABToBeSetupItemSUReqIEs->value.u.sztE_RABToBeSetupItemBrSUReq) );
  }
}

/**
 * @param   rABToBeSetupLstCtxtSUReq    E-RAB Initial context Setup List PDU
 * @param   eRabToSetupList             E-RAB Setup List
 * @details
 * This functions extracts the E-RAB Initial context Setup List for a UE from rABToBeSetupLstCtxtSUReq PDU and fills it
 * in the structure eRabToSetupList
 */
void s1apUnwrapInitCtxtERabToSetupList(
    IN SztE_RABToBeSetupLstCtxtSUReq   *rABToBeSetupLstCtxtSUReq,
    IO ERabToSetupList                 *eRabToSetupList
    )
{
  UINT16 indx;
  SztProtIE_SingleCont_E_RABToBeSetupItemCtxtSUReqIEs *rABToBeSetupItemCtxtSUReqIEs;

  eRabToSetupList->noComp = rABToBeSetupLstCtxtSUReq->noComp.val;
  for( indx=0; indx< eRabToSetupList->noComp; indx++ )
  {
    rABToBeSetupItemCtxtSUReqIEs = ( rABToBeSetupLstCtxtSUReq->member + indx);
    s1apUnwrapInitCtxtERabToSetupItem( &(eRabToSetupList->eRabToSetupItem[indx] ), &(rABToBeSetupItemCtxtSUReqIEs->value.u.sztE_RABToBeSetupItemCtxtSUReq) );
  }
}

/**
 * @param   rABToBeModifyLstSUReq    E-RAB Modify List PDU
 * @param   eRabToModifyList         E-RAB Modify List
 * @details
 * This functions extracts the E-RAB Modify List for a UE from rABToBeModifyLstSUReq PDU and fills it
 * in the structure eRabToModifyList
 */
void s1apUnwrapERabToModifyList(
    IN SztE_RABToBeMdfdLstBrModReq      *rABToBeModifyLstSUReq,
    IO ERabToModifyList                 *eRabToModifyList
    )
{
  UINT16 indx;
  SztProtIE_SingleCont_E_RABToBeMdfdItemBrModReqIEs *rABToBeModifyItemSUReqIEs;

  eRabToModifyList->noComp = rABToBeModifyLstSUReq->noComp.val;
  for( indx=0; indx< eRabToModifyList->noComp; indx++ )
  {
    rABToBeModifyItemSUReqIEs = ( rABToBeModifyLstSUReq->member + indx);
    s1apUnwrapERabToModifyItem( &(eRabToModifyList->eRabToModifyItem[indx] ), &(rABToBeModifyItemSUReqIEs->value.u.sztE_RABToBeMdfdItemBrModReq) );
  }
}

/**
 * @param   rABToBeReleasesLst    E-RAB Release List PDU
 * @param   eRabToReleaseList     E-RAB Release List
 * @details
 * This functions extracts the E-RAB Release List for a UE from rABToBeReleasesLst PDU and fills it
 * in the structure eRabToReleaseList
 */
void s1apUnwrapERabToReleaseList(
    IN SztE_RABLst                     *rABToBeReleasesLst,
    IO ERabReleaseList                 *eRabToReleaseList
    )
{
  UINT16 indx;
  SztProtIE_SingleCont_E_RABItemIEs *rABToBeReleasedItemIEs;

  eRabToReleaseList->noComp = rABToBeReleasesLst->noComp.val;
  for( indx=0; indx< eRabToReleaseList->noComp; indx++ )
  {
    rABToBeReleasedItemIEs = ( rABToBeReleasesLst->member + indx);
    s1apUnwrapERabToReleaseItem( &(eRabToReleaseList->eRabToReleaseItem[indx] ), &(rABToBeReleasedItemIEs->value.u.sztE_RABItem) );
  }
}

/**
 * @param   sztueLogAssocItem  UE Associated Logical Connection Item PDU
 * @param   ueAssocLogConItem  UE Associated Logical Connection
 * @details
 * This functions extracts the UE Associated Logical Connection Item in Reset Request from
 * ueAssocLogConItem PDU and fills it in the structure ueAssocLogConItem
 */
void s1apUnwrapUeAssocLogConnItem(
    IO S1UeAssocLogConnItem         *ueAssocLogConItem,
    IN SztUE_assocLogS1_ConItem     *sztueLogAssocItem
    )
{
  if( TRUE == sztueLogAssocItem->eNB_UE_S1AP_ID.pres )
  {
    ueAssocLogConItem->isEnbUeS1apId = TRUE;
    ueAssocLogConItem->enbUeS1apId = sztueLogAssocItem->eNB_UE_S1AP_ID.val;
  }
  if( TRUE == sztueLogAssocItem->mME_UE_S1AP_ID.pres )
  {
    ueAssocLogConItem->isMmeUeS1apId = TRUE;
    ueAssocLogConItem->mmeUeS1apId = sztueLogAssocItem->mME_UE_S1AP_ID.val;
  }
}

/**
 * @param   ueAssocLogConListPdu  UE Associated Logical Connection List PDU
 * @param   ueAssocLogConList     UE Associated Logical Connection
 * @details
 * This functions extracts the UE Associated Logical Connection for a UE from ueAssocLogConListPdu PDU and fills it
 * in the structure ueAssocLogConList
 */
void s1apUnwrapUeAssocLogConList(
    IN SztUE_assocLogS1_ConLstRes   *ueAssocLogConListPdu,
    IO S1UeAssocLogConnList         *ueAssocLogConList
    )
{
  UINT16 indx;
  SztProtIE_SingleCont_UE_assocLogS1_ConItemRes *ueAssocLogConItem;

  ueAssocLogConList->numOfS1UeAssocLogConnItems = ueAssocLogConListPdu->noComp.val;
  for( indx=0; indx< ueAssocLogConList->numOfS1UeAssocLogConnItems; indx++ )
  {
    ueAssocLogConItem = ( ueAssocLogConListPdu->member + indx);
    s1apUnwrapUeAssocLogConnItem( &(ueAssocLogConList->s1ConnItem[indx] ), &(ueAssocLogConItem->value.u.sztUE_assocLogS1_ConItem) );
  }
}

/**
 * @param   plmnIdentity      PLMN ID PDU
 * @param   pagingMcc         MCC
 * @details
 * Extract the MCC from PLMN ID
 * MCC digits are BCD encoded, extract them to ASCII before passing to RRC
 *
 *  --------------------------
 *  |  MCC 1    |     MCC0   |
 *  --------------------------
 *  |  MNC 2    |     MCC2   |
 *  --------------------------
 *  |  MNC 1    |     MNC0   |
 *  --------------------------
 */
void extractMcc(
  IN SztPLMNidentity         *plmnIdentity,
  IO U8                      *pagingMcc
  )
{
  pagingMcc[0] = plmnIdentity->val[0] & 0x0F;
  pagingMcc[1] = plmnIdentity->val[0] >> 4;
  pagingMcc[2] = plmnIdentity->val[1] & 0x0F;
}

/**
 * @param   plmnIdentity      PLMN ID PDU
 * @param   pagingMnc         MCC
 * @param   fillerBitInMnc    Whether filler is present in MNC
 * @details
 * Extract the MNC from PLMN ID
 * MNC digits are BCD encoded, extract them to ASCII before passing to RRC
 * If MNC is only 2 digits then filler is present.
 *  --------------------------
 *  |  MCC 1    |     MCC0   |
 *  --------------------------
 *  |  MNC 2    |     MCC2   |
 *  --------------------------
 *  |  MNC 1    |     MNC0   |
 *  --------------------------
 */
void extractMnc(
  IN SztPLMNidentity         *plmnIdentity,
  IO U8                      *pagingMnc,
  IO TBOOL                   *fillerBitInMnc
  )
{
  *fillerBitInMnc   = FALSE;
  /* Check for Filler Bits */
  if((plmnIdentity->val[1] >> 4) EQ 0x0F )
  {
    pagingMnc[0] = plmnIdentity->val[2] & 0x0F;
    pagingMnc[1] = plmnIdentity->val[2] >> 4;
    *fillerBitInMnc   = TRUE;
  }
  else
  {
    pagingMnc[2] = plmnIdentity->val[1] >> 4;
    pagingMnc[0] = plmnIdentity->val[2] & 0x0F;
    pagingMnc[1] = plmnIdentity->val[2] >> 4 ;
  }
}

/**
 * @param   tac         TAC PDU
 * @param   pagingTac   tac
 * @details
 * Extract the TAC from PLMN ID
 * TAC digits are toggled when received from S1 Interface.
 */
void extractTac(
  IN SztTAC      *tac,
  IO U16         *pagingTac
  )
{
  *pagingTac |= tac->val[1];
  *pagingTac |= tac->val[0] << 8;
}


/************************************* MESSAGE UNWRAP STARTS HERE ****************************/
/**
 * @param   pdu                 S1 PDU received from MME
 * @param   setupRsp            Setup Response
 * @details
 * This functions extracts the Setup Response message from pdu and fills it
 * in the structure setupRsp
 */
TBOOL s1apUnwrapSetupResPdu
  (
    IO S1apPdu      *pdu,
    IO S1SetupRsp   *setupRsp
  )
{
  SztProtIE_Field_S1SetupRespIEs  *setupRspIe = NULL;
  SztS1SetupResp                  *rsp = NULL;
  UINT16                          noOfIes,memIdx;
  U32                             protId = 0;

  logInfo("%s", "unwrap Setup Response received");

  clearBytes( setupRsp, sizeof(S1SetupRsp) );
  rsp = &(pdu->pdu.val.successfulOutcome.value.u.sztS1SetupResp);
  noOfIes = rsp->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    setupRspIe = ( rsp->protocolIEs.member + memIdx);
    protId = setupRspIe->id.val;
    switch( protId)
    {
      case Sztid_MMEname:
        setupRsp->isMmeName = TRUE;
        s1apUnwrapMmeName( &(setupRspIe->value.u.sztMMEname), setupRsp->mmeName );
        break;
      case Sztid_RelativeMMECapacity:
        setupRsp->relativeMmeCapacity = setupRspIe->value.u.sztRelativeMMECapacity.val;
        break;
      case Sztid_CriticalityDiag:
        setupRsp->isCriticalDiagnostic = TRUE;
        s1apUnwrapCriticalityDiagnostic( &(setupRspIe->value.u.sztCriticalityDiag),&(setupRsp->criticalDiagnostic));
        break;
      case Sztid_SrvdGUMMEIs:
        s1apUnwrapGummeIes( &(setupRspIe->value.u.sztSrvdGUMMEIs), &(setupRsp->servedGummeIes));
        break;
    }
  }

  return TRUE;
}

/**
 * @param   pdu                 S1 PDU received from MME
 * @param   setupFail           Setup Fail from MME
 * @details
 * This functions extracts the Setup Fail message from pdu and fills it
 * in the structure setupFail
 */
TBOOL s1apUnwrapSetupFailPdu
  (
    IO S1apPdu       *pdu,
    IO S1SetupFail   *setupFail
  )
{
  SztProtIE_Field_S1SetupFailIEs  *setupFailIe = NULL;
  SztS1SetupFail                  *fail = NULL;
  UINT16                          noOfIes,memIdx;
  U32                             protId = 0;

  logInfo("%s", "unwrap Setup Failure received");

  clearBytes( setupFail, sizeof(S1SetupFail) );

  fail = &(pdu->pdu.val.unsuccessfulOutcome.value.u.sztS1SetupFail);
  noOfIes = fail->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    setupFailIe = ( fail->protocolIEs.member + memIdx);
    protId = setupFailIe->id.val;
    switch( protId)
    {
      case Sztid_Cause:
        s1apUnwrapCause( &(setupFailIe->value.u.sztCause), &(setupFail->failCause) );
        break;
      case Sztid_TimeToWait:
        setupFail->isTimeToWait = TRUE;
        s1apUnwrapTimetoWait( &(setupFailIe->value.u.sztTimeToWait), &(setupFail->timetoWait) );
        break;
      case Sztid_CriticalityDiag:
        setupFail->isCriticalDiagnostic = TRUE;
        s1apUnwrapCriticalityDiagnostic( &(setupFailIe->value.u.sztCriticalityDiag),&(setupFail->criticalDiagnostic));
        break;
    }
  }

  return TRUE;
}

/**
 * @param   pdu                 S1 PDU received from MME
 * @param   resetAck            Reset Ack
 * @details
 * This functions extracts the Reset Ack message from pdu and fills it
 * in the structure resetAck
 */
TBOOL s1apUnwrapResetAckPdu
  (
    IO S1apPdu       *pdu,
    IO S1ResetAck    *resetAck
  )
{
  SztProtIE_Field_ResetAckgIEs  *resetAckIe = NULL;
  SztResetAckg                 *rAck = NULL;
  UINT16                        noOfIes,memIdx;
  U32                           protId = 0;

  logInfo("%s", "unwrap Reset Ack received");

  clearBytes( resetAck, sizeof(S1ResetAck) );

  rAck = &(pdu->pdu.val.successfulOutcome.value.u.sztResetAckg);
  noOfIes = rAck->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    resetAckIe = ( rAck->protocolIEs.member + memIdx );
    protId = resetAckIe->id.val;
    switch( protId)
    {
      case Sztid_CriticalityDiag:
        resetAck->isCriticalDiagnostic = TRUE;
        s1apUnwrapCriticalityDiagnostic( &(resetAckIe->value.u.sztCriticalityDiag), &(resetAck->criticalDiagnostic) );
        break;
      /* TODO Add support for UE Logical connection fields */
    }
  }

  return TRUE;
}

/**
 * @param   pdu                 S1 PDU received from MME
 * @param   dlNasMsg            NAS Message from MME
 * @details
 * This functions extracts the NAS message from pdu and fills it
 * in the structure dlNasMsg
 */
TBOOL s1apUnwrapDlNasPdu
  (
    IO S1apPdu       *pdu,
    IO DlNasMessage  *dlNasMsg
  )
{
  SztProtIE_Field_DlnkNASTport_IEs  *dlNasMsgIe = NULL;
  SztDlnkNASTport                   *rsp = NULL;
  UINT16                             noOfIes,memIdx;
  U32                                protId = 0;

  logInfo("%s", "unwrap DL NAS Message received");

  clearBytes( dlNasMsg, sizeof(DlNasMessage) );
  rsp = &(pdu->pdu.val.initiatingMsg.value.u.sztDlnkNASTport);
  noOfIes = rsp->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    dlNasMsgIe = ( rsp->protocolIEs.member + memIdx);
    protId = dlNasMsgIe->id.val;
    switch( protId)
    {
      case Sztid_MME_UE_S1AP_ID:
        dlNasMsg->mmeUeS1apId = dlNasMsgIe->value.u.sztMME_UE_S1AP_ID.val;
        break;
      case Sztid_eNB_UE_S1AP_ID:
        dlNasMsg->enbUeS1apId = dlNasMsgIe->value.u.sztENB_UE_S1AP_ID.val;
        break;
      case Sztid_NAS_PDU:
        s1apUnwrapNasPdu( &(dlNasMsgIe->value.u.sztNAS_PDU),&(dlNasMsg->nasPdu) );
        break;
      case Sztid_HovrRestrnLst:
        logError("%s","Handover is not yet supported");
        break;
    }
  }

  return TRUE;
}

/**
 * @param   pdu                 S1 PDU received from MME
 * @param   initialContextSetup Initial Context Setup
 * @details
 * This functions extracts the Initial Context Setup request from pdu and fills it
 * in the structure initialContextSetup
 */
TBOOL s1apUnwrapInitialContextSetupPdu(
    IO S1apPdu              *pdu,
    IO InitialContextSetup  *initialContextSetup
  )
{
  SztProtIE_Field_InitCntxtSetupRqstIEs  *initContxtSetupIe = NULL;
  SztInitCntxtSetupRqst                  *rsp = NULL;
  UINT16                                 noOfIes,memIdx;
  U32                                    protId = 0;

  logInfo("%s", "unwrap Initial Context Setup Message received");

  clearBytes( initialContextSetup, sizeof(InitialContextSetup) );
  rsp = &(pdu->pdu.val.initiatingMsg.value.u.sztInitCntxtSetupRqst);
  noOfIes = rsp->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    initContxtSetupIe = ( rsp->protocolIEs.member + memIdx);
    protId = initContxtSetupIe->id.val;
    switch( protId)
    {
      case Sztid_MME_UE_S1AP_ID:
        initialContextSetup->mmeUeS1apId = initContxtSetupIe->value.u.sztMME_UE_S1AP_ID.val;
        break;
      case Sztid_eNB_UE_S1AP_ID:
        initialContextSetup->enbUeS1apId = initContxtSetupIe->value.u.sztENB_UE_S1AP_ID.val;
        break;
      case Sztid_uEaggregateMaxBitrate:
        s1apUnwrapUeAggregateMaxBitRate( &(initContxtSetupIe->value.u.sztUEAggMaxBitrate), &(initialContextSetup->ueAggMaxBitRate) );
        break;
      case Sztid_E_RABToBeSetupLstCtxtSUReq:
        s1apUnwrapInitCtxtERabToSetupList( &(initContxtSetupIe->value.u.sztE_RABToBeSetupLstCtxtSUReq), &(initialContextSetup->eRabToSetupList) );
        break;
      case Sztid_UESecurCapabilities:
        s1apUnwrapUeSecurityCapability( &(initContxtSetupIe->value.u.sztUESecurCapabilities), &(initialContextSetup->ueSecurityCapability) );
        break;
      case Sztid_SecurKey:
        s1apUnwrapSecurityKey( &(initContxtSetupIe->value.u.sztSecurKey), &(initialContextSetup->securityKey) );
        break;
      case Sztid_TraceActvn:
        initialContextSetup->isTraceActivation = TRUE;
        s1apUnwrapTraceActivation( &(initContxtSetupIe->value.u.sztTraceActvn), &(initialContextSetup->traceActivation) );
        break;
      case Sztid_UERadioCapblty:
        initialContextSetup->isUeRadioCapability = TRUE;
        s1apUnwrapUeRadioCapability( &(initContxtSetupIe->value.u.sztUERadioCapblty), &(initialContextSetup->ueRadioCapability) );
        break;
      case Sztid_SubscriberProfileIDforRFP:
        initialContextSetup->isSubscriberProfileIdForRat = TRUE;
        initialContextSetup->subscriberProfileIdForRat = initContxtSetupIe->value.u.sztSubscriberProfileIDforRFP.val;
        break;
      case Sztid_CSFallbackIndicator:
        initialContextSetup->isCsFallBackIndicator = TRUE;
        initialContextSetup->csFallBackIndicator = initContxtSetupIe->value.u.sztCSFallbackIndicator.val;
        break;
      case Sztid_SRVCCOperationPossible:
        initialContextSetup->isSrvccOperationPossible = TRUE;
        initialContextSetup->srvccOperationPossible = initContxtSetupIe->value.u.sztSRVCCOperationPossible.val;
        break;
      case Sztid_HovrRestrnLst:
        logError("%s","s1apUnwrapInitialContextSetupPdu: Handover is not yet supported");
        break;
    }
  }

  return TRUE;
}

/**
 * @param   pdu            S1 PDU received from MME
 * @param   ueCtxRelMsg    UE context Release request
 * @details
 * This functions extracts the UE context Release request from pdu and fills it
 * in the structure ueCtxRelMsg
 */
TBOOL s1apUnwrapUeContextRelCmdPdu(
    IO S1apPdu          *pdu,
    IO UeContextRelCmd  *ueCtxRelMsg
  )
{
  SztProtIE_Field_UECntxtRlsCmmd_IEs  *ueCtxRelCmdIe = NULL;
  SztUECntxtRlsCmmd                   *rsp = NULL;
  UINT16                             noOfIes,memIdx;
  U32                                protId = 0;

  logInfo("%s", "unwrap UE Context Release Message received ");

  clearBytes( ueCtxRelMsg, sizeof(UeContextRelCmd) );
  rsp = &(pdu->pdu.val.initiatingMsg.value.u.sztUECntxtRlsCmmd);
  noOfIes = rsp->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    ueCtxRelCmdIe = ( rsp->protocolIEs.member + memIdx);
    protId = ueCtxRelCmdIe->id.val;
    switch( protId)
    {
      case Sztid_UE_S1AP_IDs:
        s1apUnwrapUeS1apId( &(ueCtxRelCmdIe->value.u.sztUE_S1AP_IDs), &(ueCtxRelMsg->ueS1apId) );
        break;
      case Sztid_Cause:
        s1apUnwrapCause( &(ueCtxRelCmdIe->value.u.sztCause), &(ueCtxRelMsg->cause) );
        break;
    }
  }

  return TRUE;
}

/**
 * @param   pdu            S1 PDU received from MME
 * @param   resetReqMsg    Reset Request
 * @details
 * This functions extracts the Reset request from pdu and fills it
 * in the structure resetReqMsg
 */
TBOOL s1apUnwrapS1ResetReqPdu(
    IO S1apPdu          *pdu,
    IO S1ResetReq       *resetReqMsg
  )
{
  SztProtIE_Field_ResetIEs           *resetReqCmdIe = NULL;
  SztReset                           *req = NULL;
  UINT16                             noOfIes,memIdx;
  U32                                protId = 0;

  logInfo("%s", "unwrap Reset Request received");

  clearBytes( resetReqMsg, sizeof(S1ResetReq) );
  req = &(pdu->pdu.val.initiatingMsg.value.u.sztReset);
  noOfIes = req->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    resetReqCmdIe = ( req->protocolIEs.member + memIdx);
    protId = resetReqCmdIe->id.val;

    switch( protId)
    {
      case Sztid_ResetTyp:
      {
        if( RESETTYP_S1_INTF == resetReqCmdIe->value.u.sztResetTyp.choice.val )
        {
          resetReqMsg->resetType = ResetType_GlobalS1Reset;
          resetReqMsg->u.isGlobalReset = TRUE;
        }
        else
        {
          resetReqMsg->resetType = ResetType_UeS1Reset;
          s1apUnwrapUeAssocLogConList( &(resetReqCmdIe->value.u.sztResetTyp.val.partOfS1_Intf), &(resetReqMsg->u.ueAssocLogConnList) );
        }
        break;
      }
      case Sztid_Cause:
        s1apUnwrapCause( &(resetReqCmdIe->value.u.sztCause), &(resetReqMsg->resetCause) );
        break;
    }
  }

  return TRUE;
}

/**
 * @param   pdu            S1 PDU received from MME
 * @param   pagingInfo     Paging request from MME
 * @details
 * This functions extracts the Paging request from pdu and fills it
 * in the structure pagingInfo
 */
TBOOL s1apUnwrapS1PagingReqPdu(
    IO S1apPdu          *pdu,
    IO S1PagingInfo     *pagingInfo
  )
{
  SztProtIE_Field_PagIEs  *pagMember = NULLP;
  SztPag                  *sztPag = NULLP;
  U16                      noOfIes = 0;
  U32                      protId = 0;
  U16                      memberIdx = 0;
  TknBStr32               *ueIdxBitStr = NULLP;
  U8                       idx = 0 , idx1 = 0;
  int                       cnt = 0;
  U8                       count = 0;
  SztPLMNidentity         *plmnIdentity = NULLP;
  SztTAC                  *tac = NULLP;
  U8                       imsiLen = 0;

  logInfo("%s", "unwrap Paging Request Decoding started");

  clearBytes( pagingInfo, sizeof(S1PagingInfo) );
  /* Get the reference of the Paging Message */
  sztPag = &(pdu->pdu.val.initiatingMsg.value.u.sztPag);

  noOfIes = sztPag->protocolIEs.noComp.val;

  for( memberIdx=0; memberIdx< noOfIes; memberIdx++ )
  {
    pagMember= &(sztPag->protocolIEs.member[memberIdx]);
    protId = pagMember->id.val;

    switch(protId)
    {
      case Sztid_UEIdentityIdxValue:
      {
        ueIdxBitStr = &(pagMember->value.u.sztUEIdentityIdxValue);
        pagingInfo->ueId = ((ueIdxBitStr->val[0] << 2) | ((ueIdxBitStr->val[1] & 0xC0) >> 6));
        break;
      }
      case Sztid_UEPagID:
      {
        pagingInfo->choice = pagMember->value.u.sztUEPagID.choice.val;
        if(pagMember->value.u.sztUEPagID.choice.val == UEPAGID_IMSI)
        {
          idx1 = 0;

          /* IMSI length will vary from 6 to 15
           * If length is odd then filler bits will be added
           * TODO : Need to optimise
           * */
          imsiLen = pagMember->value.u.sztUEPagID.val.iMSI.len;
          pagingInfo->val.imsi.noOfImsiDigits = imsiLen * 2;
          for(idx = 0; idx < imsiLen -1 ; idx++)
          {
            pagingInfo->val.imsi.imsiVal[idx1]   = pagMember->value.u.sztUEPagID.val.iMSI.val[idx] & 0x0f;
            pagingInfo->val.imsi.imsiVal[idx1+1] = pagMember->value.u.sztUEPagID.val.iMSI.val[idx] >> 4;
            logInfo("pagingInfo->val.imsi.imsiVal[idx1] = %d, idx1 = %d ,idx = %d",pagingInfo->val.imsi.imsiVal[idx1],idx1,idx);
            logInfo("pagingInfo->val.imsi.imsiVal[idx1+1] = %d, idx1+1 = %d ,idx = %d",pagingInfo->val.imsi.imsiVal[idx1+1],idx1+1,idx);
            idx1 = idx1 + 2;
          } /* end of for loop */
          pagingInfo->val.imsi.imsiVal[idx1]   = pagMember->value.u.sztUEPagID.val.iMSI.val[idx] & 0x0f;
          pagingInfo->val.imsi.imsiVal[idx1+1] = pagMember->value.u.sztUEPagID.val.iMSI.val[idx] >> 4;
          logInfo("pagingInfo->val.imsi.imsiVal[idx1] = %d, idx1 = %d ,idx = %d",pagingInfo->val.imsi.imsiVal[idx1],idx1,idx);
          logInfo("pagingInfo->val.imsi.imsiVal[idx1+1] = %d, idx1+1 = %d ,idx = %d",pagingInfo->val.imsi.imsiVal[idx1+1],idx1+1,idx);

          if(0x0f == pagingInfo->val.imsi.imsiVal[idx1+1] )
          {
            pagingInfo->val.imsi.noOfImsiDigits -= 1;
            logInfo("pagingInfo->val.imsi.noOfImsiDigits=%d",pagingInfo->val.imsi.noOfImsiDigits);
          }
        }
        else if (pagMember->value.u.sztUEPagID.choice.val == UEPAGID_S_TMSI)
        {
          logInfo("%s","Paging message with UEPAGID_S_TMSI is received");
          pagingInfo->val.sTmsi.mmec = pagMember->value.u.sztUEPagID.val.s_TMSI.mMEC.val[0];
          for(cnt = 3; cnt >= 0; cnt--)
          {
            pagingInfo->val.sTmsi.sTMSIVal <<= 8;
            pagingInfo->val.sTmsi.sTMSIVal |= (U8)(pagMember->value.u.sztUEPagID.val.s_TMSI.m_TMSI.val[cnt]);
          } /* end of for loop */
        }
        else
        {
          logError("%s", "Valid UE Identity should be provided");
          return FALSE;
        } /* end of else statement */
        break;
      } /* end of Sztid_UEPagID */

      case Sztid_pagingDRX:
      {
        pagingInfo->ueSpecDrx.pagingDrxPres = TRUE;
        pagingInfo->ueSpecDrx.ueSpcPagDrx = pagMember->value.u.sztPagDRX.val;
        break;
      } /* end of Sztid_pagingDRX */
      case Sztid_CNDomain:
      {
        pagingInfo->cnDomain = pagMember->value.u.sztCNDomain.val;
        break;
      }
      case Sztid_TAILst:
      {
        pagingInfo->noOfTaiPresent = pagMember->value.u.sztTAILst.noComp.val;
        for(count = 0; count < pagingInfo->noOfTaiPresent; count++)
        {
          plmnIdentity = &pagMember->value.u.sztTAILst.member[count].value.u.sztTAIItem.tAI.pLMNidentity;
          tac  = &pagMember->value.u.sztTAILst.member[count].value.u.sztTAIItem.tAI.tAC;
          /* Extract MCC , MNC and TAC */
          extractMcc( plmnIdentity , &pagingInfo->taiList[count].plmnId.mcc[0]);
          extractMnc( plmnIdentity , &pagingInfo->taiList[count].plmnId.mnc[0], &pagingInfo->taiList[count].plmnId.fillerBitInMnc);
          extractTac( tac , &pagingInfo->taiList[count].tac );
        }
        break;
      }
      case Sztid_CSG_IdLst: /* If received with CSG ID list IE then no need to process Paging req */
      {
        logError("%s", "unwrap Sztid_CSG_IdLst IE  Received-- IE is Ignored");
        break;
      }
      default:/* Log Error */
      {
        logError("Unknown IE :%d Received-- Paging Message is Dropped", protId);
        return FALSE;
      }
    } /* end of switch statement */
  } /* end of for loop */
  return TRUE;
}

/**
 * @param   pdu                 S1 PDU received from MME
 * @param   mmeConfigUpdate     MME Config Update
 * @details
 * This functions extracts the MME Config Update Request from pdu and fills it
 * in the structure mmeConfigUpdate
 */
TBOOL s1apUnwrapMmeConfigUpdatePdu
  (
    IO S1apPdu             *pdu,
    IO MmeCfgUpdate        *mmeConfigUpdate
  )
{
  SztProtIE_Field_MMEConfigUpdIEs  *mmeCfgUpdIe = NULL;
  SztMMEConfigUpd                  *req = NULL;
  UINT16                          noOfIes,memIdx;
  U32                             protId = 0;

  logInfo("%s", "unwrap MME Config Update received");

  clearBytes( mmeConfigUpdate, sizeof(MmeCfgUpdate) );
  req = &(pdu->pdu.val.initiatingMsg.value.u.sztMMEConfigUpd);
  noOfIes = req->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    mmeCfgUpdIe = ( req->protocolIEs.member + memIdx);
    protId = mmeCfgUpdIe->id.val;
    switch( protId)
    {
      case Sztid_MMEname:
        mmeConfigUpdate->isMmeName = TRUE;
        s1apUnwrapMmeName( &(mmeCfgUpdIe->value.u.sztMMEname), mmeConfigUpdate->mmeName );
        break;
      case Sztid_RelativeMMECapacity:
        mmeConfigUpdate->isRelativeMmeCapacity = TRUE;
        mmeConfigUpdate->relativeMmeCapacity = mmeCfgUpdIe->value.u.sztRelativeMMECapacity.val;
        break;
      case Sztid_SrvdGUMMEIs:
        mmeConfigUpdate->isServedGummeIes = TRUE;
        s1apUnwrapGummeIes( &(mmeCfgUpdIe->value.u.sztSrvdGUMMEIs), &(mmeConfigUpdate->servedGummeIes));
        break;
    }
  }

  return TRUE;
}

/**
 * @param   pdu                 S1 PDU received from MME
 * @param   locReqReportCntrl   Location Report Request
 * @details
 * This functions extracts the Location Report Request from pdu and fills it in the structure locReqReportCntrl
 */
TBOOL s1apUnwrapLocReportCntrlPdu(
  IO S1apPdu            *pdu,
  IO LocReqReportCntrl  *locReqReportCntrl
  )
{
  SztProtIE_Field_LocRprtngCntrlIEs  *locRprtMsgIe = NULL;
  SztLocRprtngCntrl                  *req = NULL;
  UINT16                             noOfIes,memIdx;
  U32                                protId = 0;

  logInfo("%s", "unwrap Location Report received");

  clearBytes( locReqReportCntrl, sizeof(LocReqReportCntrl) );
  req = &(pdu->pdu.val.initiatingMsg.value.u.sztLocRprtngCntrl);
  noOfIes = req->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    locRprtMsgIe = ( req->protocolIEs.member + memIdx);
    protId = locRprtMsgIe->id.val;
    switch( protId)
    {
      case Sztid_MME_UE_S1AP_ID:
        locReqReportCntrl->mmeUeS1apId = locRprtMsgIe->value.u.sztMME_UE_S1AP_ID.val;
        break;
      case Sztid_eNB_UE_S1AP_ID:
        locReqReportCntrl->enbUeS1apId = locRprtMsgIe->value.u.sztENB_UE_S1AP_ID.val;
        break;
      case Sztid_RqstTyp:
        locReqReportCntrl->locReportCntrlReqType.locReportTypeEvent = locRprtMsgIe->value.u.sztRqstTyp.eventTyp.val;
        locReqReportCntrl->locReportCntrlReqType.locReportArea = locRprtMsgIe->value.u.sztRqstTyp.reportArea.val;
        break;
      default:
        return FALSE;
    }
  }
  return TRUE;
}

/**
 * @param   pdu            S1 PDU received from MME
 * @param   traceStart     Start Trace
 * @details
 * This functions extracts the Start Trace request from pdu and fills it in the structure traceStart
 */
TBOOL s1apUnwrapTraceStartPdu(
    IO S1apPdu              *pdu,
    IO TraceStart           *traceStart
  )
{
  SztProtIE_Field_TraceStartIEs  *traceStartIe = NULL;
  SztTraceStart                  *req = NULL;
  UINT16                         noOfIes,memIdx;
  U32                            protId = 0;

  logInfo("%s", "unwrap Trace Start Message received");

  clearBytes( traceStart, sizeof(TraceStart) );
  req = &(pdu->pdu.val.initiatingMsg.value.u.sztTraceStart);
  noOfIes = req->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    traceStartIe = ( req->protocolIEs.member + memIdx);
    protId = traceStartIe->id.val;
    switch( protId)
    {
      case Sztid_MME_UE_S1AP_ID:
        traceStart->mmeUeS1apId = traceStartIe->value.u.sztMME_UE_S1AP_ID.val;
        break;
      case Sztid_eNB_UE_S1AP_ID:
        traceStart->enbUeS1apId = traceStartIe->value.u.sztENB_UE_S1AP_ID.val;
        break;
      case Sztid_TraceActvn:
        s1apUnwrapTraceActivation( &(traceStartIe->value.u.sztTraceActvn), &(traceStart->traceActivation) );
        break;
    }
  }

  return TRUE;
}

/**
 * @param   pdu                 S1 PDU received from MME
 * @param   deactivateTrace     Deactivate Trace
 * @details
 * This functions extracts the Deactivate Trace request from pdu and fills it in the structure deactivateTrace
 */
TBOOL s1apUnwrapDeactivateTracePdu(
    IO S1apPdu              *pdu,
    IO DeactivateTrace      *deactivateTrace
  )
{
  SztProtIE_Field_DeactvTraceIEs *deactivateTraceIe = NULL;
  SztDeactvTrace                 *req = NULL;
  UINT16                         noOfIes,memIdx;
  U32                            protId = 0;

  logInfo("%s", "unwrap Deactivate Trace Message received");

  clearBytes( deactivateTrace, sizeof(DeactivateTrace) );
  req = &(pdu->pdu.val.initiatingMsg.value.u.sztDeactvTrace);
  noOfIes = req->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    deactivateTraceIe = ( req->protocolIEs.member + memIdx);
    protId = deactivateTraceIe->id.val;
    switch( protId)
    {
      case Sztid_MME_UE_S1AP_ID:
        deactivateTrace->mmeUeS1apId = deactivateTraceIe->value.u.sztMME_UE_S1AP_ID.val;
        break;
      case Sztid_eNB_UE_S1AP_ID:
        deactivateTrace->enbUeS1apId = deactivateTraceIe->value.u.sztENB_UE_S1AP_ID.val;
        break;
      case Sztid_E_UTRAN_Trace_ID:
        s1apUnwrapEUtranTraceId( &(deactivateTrace->eUtranTraceId), &( deactivateTraceIe->value.u.sztE_UTRAN_Trace_ID) );
        break;
    }
  }
  return TRUE;
}

/**
 * @param   pdu                 S1 PDU received from MME
 * @param   overloadStart       E-RAB Setup request
 * @details
 * This functions extracts the E-RAB Setup request from pdu and fills it in the structure eRabSetupReq
 */
TBOOL s1apUnwrapERabSetupReqPdu(
    IO S1apPdu              *pdu,
    IO ERabSetupReq         *eRabSetupReq
  )
{
  SztProtIE_Field_E_RABSetupRqstIEs  *eRabSetupReqIe = NULL;
  SztE_RABSetupRqst                  *req = NULL;
  UINT16                             noOfIes,memIdx;
  U32                                protId = 0;

  logInfo("%s", "unwrap ERab Setup Message received");

  clearBytes( eRabSetupReq, sizeof(ERabSetupReq) );
  req = &(pdu->pdu.val.initiatingMsg.value.u.sztE_RABSetupRqst);
  noOfIes = req->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    eRabSetupReqIe = ( req->protocolIEs.member + memIdx);
    protId = eRabSetupReqIe->id.val;
    switch( protId)
    {
      case Sztid_MME_UE_S1AP_ID:
        eRabSetupReq->mmeUeS1apId = eRabSetupReqIe->value.u.sztMME_UE_S1AP_ID.val;
        break;
      case Sztid_eNB_UE_S1AP_ID:
        eRabSetupReq->enbUeS1apId = eRabSetupReqIe->value.u.sztENB_UE_S1AP_ID.val;
        break;
      case Sztid_uEaggregateMaxBitrate:
        eRabSetupReq->isUeAggMaxBitRate = TRUE;
        s1apUnwrapUeAggregateMaxBitRate( &(eRabSetupReqIe->value.u.sztUEAggMaxBitrate), &(eRabSetupReq->ueAggMaxBitRate) );
        break;
      case Sztid_E_RABToBeSetupLstBrSUReq:
        s1apUnwrapERabToSetupList( &(eRabSetupReqIe->value.u.sztE_RABToBeSetupLstBrSUReq), &(eRabSetupReq->eRabToSetupList) );
        break;
    }
  }
  return TRUE;
}

/**
 * @param   pdu                 S1 PDU received from MME
 * @param   overloadStart       Overload start
 * @details
 * This functions extracts the Overload start from pdu and fills it in the structure overloadStart
 */
TBOOL s1apUnwrapOverloadStartPdu  (
    IO S1apPdu             *pdu,
    IO OverloadStart       *overloadStart
  )
{
  SztProtIE_Field_OverloadStartIEs *overloadStartIe = NULL;
  SztOverloadStart                 *req = NULL;
  UINT16                          noOfIes,memIdx;
  U32                             protId = 0;

  logInfo("%s", "unwrap Overload Start received");

  clearBytes( overloadStart, sizeof(OverloadStart) );
  req = &(pdu->pdu.val.initiatingMsg.value.u.sztOverloadStart);
  noOfIes = req->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    overloadStartIe = ( req->protocolIEs.member + memIdx);
    protId = overloadStartIe->id.val;
    switch( protId)
    {
      case Sztid_OverloadResp:
        overloadStart->overloadResponse.overloadAction = (OverloadAction)overloadStartIe->value.u.sztOverloadResp.val.overloadAction.val;
        break;
    }
  }

  return TRUE;
}

/**
 * @param   pdu                 S1 PDU received from MME
 * @param   eRabModifyReq       E-RAB Modify request
 * @details
 * This functions extracts the E-RAB Modify from pdu and fills it in the structure eRabModifyReq
 */
TBOOL s1apUnwrapERabModifyReqPdu(
    IO S1apPdu              *pdu,
    IO ERabModifyReq        *eRabModifyReq
  )
{
  SztProtIE_Field_E_RABMdfyRqstIEs  *eRabModifyReqIe = NULL;
  SztE_RABMdfyRqst                  *req = NULL;
  UINT16                             noOfIes,memIdx;
  U32                                protId = 0;

  logInfo("%s", "unwrap ERab Modify Message received");

  clearBytes( eRabModifyReq, sizeof(ERabModifyReq) );
  req = &(pdu->pdu.val.initiatingMsg.value.u.sztE_RABMdfyRqst);
  noOfIes = req->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    eRabModifyReqIe = ( req->protocolIEs.member + memIdx);
    protId = eRabModifyReqIe->id.val;
    switch( protId)
    {
      case Sztid_MME_UE_S1AP_ID:
        eRabModifyReq->mmeUeS1apId = eRabModifyReqIe->value.u.sztMME_UE_S1AP_ID.val;
        break;
      case Sztid_eNB_UE_S1AP_ID:
        eRabModifyReq->enbUeS1apId = eRabModifyReqIe->value.u.sztENB_UE_S1AP_ID.val;
        break;
      case Sztid_uEaggregateMaxBitrate:
        eRabModifyReq->isUeAggMaxBitRate = TRUE;
        s1apUnwrapUeAggregateMaxBitRate( &(eRabModifyReqIe->value.u.sztUEAggMaxBitrate), &(eRabModifyReq->ueAggMaxBitRate) );
        break;
      case Sztid_E_RABToBeMdfdLstBrModReq:
        s1apUnwrapERabToModifyList( &(eRabModifyReqIe->value.u.sztE_RABToBeMdfdLstBrModReq), &(eRabModifyReq->eRabToModifyList) );
        break;
    }
  }
  return TRUE;
}

/**
 * @param   pdu                 S1 PDU received from MME
 * @param   eRabReleaseReq      E-RAB Release request
 * @details
 * This functions extracts the E-RAB Release from pdu and fills it in the structure eRabReleaseReq
 */
TBOOL s1apUnwrapERabReleaseCmdPdu(
    IO S1apPdu               *pdu,
    IO ERabReleaseReq        *eRabReleaseReq
  )
{
  SztProtIE_Field_E_RABRlsCmmdIEs   *eRabModifyReqIe = NULL;
  SztE_RABRlsCmmd                   *req = NULL;
  UINT16                             noOfIes,memIdx;
  U32                                protId = 0;

  logInfo("%s", "unwrap ERab Release Message received");

  clearBytes( eRabReleaseReq, sizeof(ERabReleaseReq) );
  req = &(pdu->pdu.val.initiatingMsg.value.u.sztE_RABRlsCmmd);
  noOfIes = req->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    eRabModifyReqIe = ( req->protocolIEs.member + memIdx);
    protId = eRabModifyReqIe->id.val;
    switch( protId)
    {
      case Sztid_MME_UE_S1AP_ID:
        eRabReleaseReq->mmeUeS1apId = eRabModifyReqIe->value.u.sztMME_UE_S1AP_ID.val;
        break;
      case Sztid_eNB_UE_S1AP_ID:
        eRabReleaseReq->enbUeS1apId = eRabModifyReqIe->value.u.sztENB_UE_S1AP_ID.val;
        break;
      case Sztid_uEaggregateMaxBitrate:
        eRabReleaseReq->isUeAggMaxBitRate = TRUE;
        s1apUnwrapUeAggregateMaxBitRate( &(eRabModifyReqIe->value.u.sztUEAggMaxBitrate), &(eRabReleaseReq->ueAggMaxBitRate) );
        break;
      case Sztid_E_RABToBeRlsdLst:
        s1apUnwrapERabToReleaseList( &(eRabModifyReqIe->value.u.sztE_RABLst), &(eRabReleaseReq->eRabToReleaseList) );
        break;
      case Sztid_NAS_PDU:
        eRabReleaseReq->isNasPdu = TRUE;
        s1apUnwrapNasPdu( &(eRabModifyReqIe->value.u.sztNAS_PDU),&(eRabReleaseReq->nasPdu) );
        break;
    }
  }
  return TRUE;
}

/**
 * @param   pdu                 S1 PDU received from MME
 * @param   ueContextModReq     UE Context Modification request
 * @details
 * This functions extracts the UE Context Modification requeste from pdu and fills it in the structure ueContextModReq
 */
TBOOL s1apUnwrapUeContextModReqPdu(
    IO S1apPdu                     *pdu,
    IO UeContextModReq             *ueContextModReq
  )
{
  SztProtIE_Field_UECntxtModificationRqstIEs  *ueContextModReqIe = NULL;
  SztUECntxtModificationRqst                  *rsp = NULL;
  UINT16                                 noOfIes,memIdx;
  U32                                    protId = 0;

  logInfo("%s", "unwrap UE Context Modification Message received");

  clearBytes( ueContextModReq, sizeof(UeContextModReq) );
  rsp = &(pdu->pdu.val.initiatingMsg.value.u.sztUECntxtModificationRqst);
  noOfIes = rsp->protocolIEs.noComp.val;

  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    ueContextModReqIe = ( rsp->protocolIEs.member + memIdx);
    protId = ueContextModReqIe->id.val;
    switch( protId)
    {
      case Sztid_MME_UE_S1AP_ID:
        ueContextModReq->mmeUeS1apId = ueContextModReqIe->value.u.sztMME_UE_S1AP_ID.val;
        break;
      case Sztid_eNB_UE_S1AP_ID:
        ueContextModReq->enbUeS1apId = ueContextModReqIe->value.u.sztENB_UE_S1AP_ID.val;
        break;
      case Sztid_uEaggregateMaxBitrate:
        ueContextModReq->isUeAggMaxBitRate = TRUE;
        s1apUnwrapUeAggregateMaxBitRate( &(ueContextModReqIe->value.u.sztUEAggMaxBitrate), &(ueContextModReq->ueAggMaxBitRate) );
        break;
      case Sztid_SecurKey:
        ueContextModReq->isSecurityKey = TRUE;
        s1apUnwrapSecurityKey( &(ueContextModReqIe->value.u.sztSecurKey), &(ueContextModReq->securityKey) );
        break;
      case Sztid_SubscriberProfileIDforRFP:
        ueContextModReq->isSubscriberProfileIdForRat = TRUE;
        ueContextModReq->subscriberProfileIdForRat = ueContextModReqIe->value.u.sztSubscriberProfileIDforRFP.val;
        break;
      case Sztid_CSFallbackIndicator:
        ueContextModReq->isCsFallBackIndicator = TRUE;
        ueContextModReq->csFallBackIndicator = ueContextModReqIe->value.u.sztCSFallbackIndicator.val;
        break;
    }
  }
  return TRUE;
}


/**
 * @param   sztMsgIdentifier     Message Identifier received
 * @param   warnMsgId            Warning Message Identifier to be filled
 * @details
 * This functions extracts Message Identifier from sztMsgIdentifier PDU and fills it
 * in the structure warnMsgId
 */
static void s1apUnwrapMsgIdentifier(
    IO SztMsgIdentifier *sztMsgIdentifier,
    IO UINT8            *warnMsgId
    )
{
  copyBytes( sztMsgIdentifier->val, MAX_SIZE_WARNMSGID, warnMsgId );
}

/**
 * @param   sztSerialNumber      Serial Number received
 * @param   warnMsgId            Serial Number to be filled
 * @details
 * This functions extracts Serial Number from SztSerialNumber PDU and fills it
 * in the structure serialNum
 */
static void s1apUnwrapSerialNumber(
    IN SztSerialNumber *sztSerialNumber,
    IO UINT8            *serialNum
    )
{
  copyBytes( sztSerialNumber->val, MAX_SIZE_SERIALNUM, serialNum );
}

static void s1apUnwrapWarnAreaListTypeCellIdList(
    IN SztECGILst              *cellIDLst,
    IO CellIdListForWarning    *cellIdListForWarning
    )
{
  UINT16         memIdx;
  SztEUTRAN_CGI *eUtranCgi = NULLP;
  cellIdListForWarning->numOfCellIds = cellIDLst->noComp.val;

  for ( memIdx = 0; memIdx < cellIdListForWarning->numOfCellIds; memIdx++ )
  {
    eUtranCgi = (cellIDLst->member + memIdx);
    copyBytes(&eUtranCgi->cell_ID, MAX_SIZE_CELLID, &cellIdListForWarning->eUtranCgi[memIdx].cellIdentity);
    copyBytes(&eUtranCgi->pLMNidentity, SIZE_PLMNIDENTITY, &cellIdListForWarning->eUtranCgi[memIdx].plmnIdentity);
  }
}
/**
 * @param   sztWarningAreaLst     Warning Areal List
 * @param   warnAreaList          Warning Areal List to be filled
 * @details
 * This functions extracts Warning Areal List from SztWarningAreaLst PDU and fills it
 * in the structure warnAreaList
 */
static void s1apUnwrapWarningAreaLst(
    IN SztWarningAreaLst *sztWarningAreaLst,
    IO WarnAreaList      *warnAreaList
    )
{
  warnAreaList->warnAreaListType = sztWarningAreaLst->choice.val;
  switch(warnAreaList->warnAreaListType)
  {
    case WarnAreaListType_CellIdList:
      s1apUnwrapWarnAreaListTypeCellIdList( &(sztWarningAreaLst->val.cellIDLst), &warnAreaList->val.cellIdListForWarning );
      break;
    case WarnAreaListType_TaiList:
      break;
    case WarnAreaListType_EmergencyAreaIdList:
      break;
  }
}

/**
 * @param   sztMsgIdentifier     Message Identifier received
 * @param   warnMsgId            Warning Message Identifier to be filled
 * @details
 * This functions extracts Message Identifier from sztMsgIdentifier PDU and fills it
 * in the structure warnMsgId
 */
static TBOOL s1apUnwrapWarningMsgContents(
    IN SztWarningMsgContents *sztWarningMsgContents,
    IO WarnMsgContents       *warnMsgContents
    )
{
  if( sztWarningMsgContents->len > MAX_SIZE_WARN_MSG_CONTENTS )
  {
    logError("Len is more than :%d Bytes", MAX_SIZE_WARN_MSG_CONTENTS );
    return FALSE;
  }
  else
  {
    warnMsgContents->warnMsgLen = sztWarningMsgContents->len;
    copyBytes( sztWarningMsgContents->val, warnMsgContents->warnMsgLen, warnMsgContents->warnMsg );
    return TRUE;
  }
}

/**
 * @param   pdu                 S1 PDU received from MME
 * @param   mmeConfigUpdate     MME Config Update
 * @details
 * This functions extracts the MME Config Update Request from pdu and fills it
 * in the structure mmeConfigUpdate
 */
TBOOL s1apUnwrapMmeWriteReplaceWarningReq
  (
    IO S1apPdu               *pdu,
    IO WriteReplcWarnReq     *writeReplcWarnReq
  )
{
  SztProtIE_Field_WriteReplaceWarningRqstIEs  *writeReplcWarnIe = NULL;
  SztWriteReplaceWarningRqst                  *req = NULL;
  UINT16                                       noOfIes,memIdx;
  U32                                          protId = 0;

  logInfo("%s", "Unwrap MME Write-Replace Warning Request received");

  req = &(pdu->pdu.val.initiatingMsg.value.u.sztWriteReplaceWarningRqst);
  noOfIes = req->protocolIEs.noComp.val;

  for( memIdx=0; memIdx < noOfIes; memIdx++ )
  {
    writeReplcWarnIe = ( req->protocolIEs.member + memIdx);
    protId = writeReplcWarnIe->id.val;
    switch( protId)
    {
      case Sztid_MsgIdentifier:
        s1apUnwrapMsgIdentifier( &(writeReplcWarnIe->value.u.sztMsgIdentifier), writeReplcWarnReq->warnMsgId );
        break;
      case Sztid_SerialNumber:
        s1apUnwrapSerialNumber( &(writeReplcWarnIe->value.u.sztSerialNumber), writeReplcWarnReq->serialNum );
        break;
      case Sztid_WarningAreaLst:
        writeReplcWarnReq->isWarnAreaListPresent = TRUE;
        s1apUnwrapWarningAreaLst( &(writeReplcWarnIe->value.u.sztWarningAreaLst), &writeReplcWarnReq->warnAreaList );
        break;
      case Sztid_RepetitionPeriod:
        writeReplcWarnReq->repetitionPeriod = writeReplcWarnIe->value.u.sztRepetitionPeriod.val;
        break;
      case Sztid_NumberofBroadcastRqst:
        writeReplcWarnReq->numOfBroadcastRequested = writeReplcWarnIe->value.u.sztNumberofBroadcastRqst.val;
        break;
      case Sztid_WarningTyp:
        writeReplcWarnReq->isWarningTypePresent = TRUE;
        copyBytes( writeReplcWarnIe->value.u.sztWarningTyp.val, SIZE_WARNINGTYPE, writeReplcWarnReq->warningType );
        break;
      case Sztid_WarningSecurInfo:
        writeReplcWarnReq->isWarningSecurityInfoPresent = TRUE;
        copyBytes( writeReplcWarnIe->value.u.sztWarningSecurInfo.val, SIZE_WARNINGSECURITYINFO, writeReplcWarnReq->warningSecurityInfo );
        break;
      case Sztid_DataCodingScheme:
        writeReplcWarnReq->isDataCodingSchemePresent = TRUE;
        copyBytes( writeReplcWarnIe->value.u.sztDataCodingScheme.val, MAX_SIZE_DATACODINGSCHEME, writeReplcWarnReq->dataCodingScheme );
        break;
      case Sztid_WarningMsgContents:
        writeReplcWarnReq->isWarnMsgContentsPresent = TRUE;
        if( FALSE EQ s1apUnwrapWarningMsgContents( &(writeReplcWarnIe->value.u.sztWarningMsgContents), &writeReplcWarnReq->warnMsgContents ))
          return FALSE;
        break;
    }
  }
  return TRUE;
}


static void s1apUnwrapERabSubjectToFwdingItem(
  IO ERabsSubjectToFwdItem    *eRabSubjectToFwdItem,
  IN SztE_RABDataFwdingItem   *sztE_RABDataFwdingItem
  )
{
  eRabSubjectToFwdItem->eRabId       = sztE_RABDataFwdingItem->e_RAB_ID.val;
  s1apUnwrapTransportLayerAddr( &(eRabSubjectToFwdItem->dlTransportLayerAddr), &(sztE_RABDataFwdingItem->dL_transportLyrAddr) );
  s1apUnwrapGtpTeId( &(eRabSubjectToFwdItem->dlGtPTeId), &(sztE_RABDataFwdingItem->dL_gTP_TEID) );
  s1apUnwrapTransportLayerAddr( &(eRabSubjectToFwdItem->ulTransportLayerAddr), &(sztE_RABDataFwdingItem->uL_TportLyrAddr) );
  s1apUnwrapGtpTeId( &(eRabSubjectToFwdItem->ulGtPTeId), &(sztE_RABDataFwdingItem->uL_GTP_TEID) );
}

static void s1apUnwrapERabSubjectToFwdingList(
    IN SztE_RABSubjtoDataFwdingLst     *ccpuFwdingList,
    IO ERabsSubjectToFwdList           *pwavFwdingList
    )
{
  UINT16 indx;
  SztProtIE_SingleCont_E_RABDataFwdingItemIEs        *rabFwdingItemIes;

  pwavFwdingList->noComp = ccpuFwdingList->noComp.val;

  for( indx=0; indx< pwavFwdingList->noComp; indx++ )
  {
    rabFwdingItemIes = ( ccpuFwdingList->member + indx);
    s1apUnwrapERabSubjectToFwdingItem( &(pwavFwdingList->eRabSubToFwdItem[indx] ), &(rabFwdingItemIes->value.u.sztE_RABDataFwdingItem) );
  }
}

/**
 * @param   sztTget_ToSrc_TprntCont       Target To Source Transparent Container  PDU
 * @param   tgetToSrcTprntContainer       Target To Source Transparent Container
 * @details This functions extracts the Target To Source Transparent Container from
 *          sztTget_ToSrc_TprntCont and fills it in the structure tgetToSrcTprntContainer
 */
static TBOOL s1apUnwrapTgetToSrcTprntCont(
    IN SztTget_ToSrc_TprntCont      *sztTget_ToSrc_TprntCont,
    IO TgtToSrcTprntCont            *tgtToSrcTprntCont
    )
{
  if( sztTget_ToSrc_TprntCont->len > MAX_SIZE_TGT_TO_SRC_TPRNT_CONT )
  {
    logError("Length is more than :%d Bytes", MAX_SIZE_TGT_TO_SRC_TPRNT_CONT );
    return FALSE;
  }
  else
  {
    tgtToSrcTprntCont->msgLen = sztTget_ToSrc_TprntCont->len;
    copyBytes( sztTget_ToSrc_TprntCont->val, sztTget_ToSrc_TprntCont->len, tgtToSrcTprntCont->msgPdu );
  }
  return TRUE;
}

/**
 * @param   pdu          S1AP PDU
 * @param   handoverCmd  Handover Command
 * @details This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 *          values in handoverRequired message.
 */
TBOOL s1apUnWrapHandoverCommandMsgPdu(
  IO S1apPdu                  *pdu,
  IO HandoverCmd              *handoverCmd
  )
{
   SztProtIE_Field_HovrCmmdIEs   *handoverCmdIe = NULL;
   SztHovrCmmd                   *rsp = NULL;
   UINT16                        noOfIes,memIdx;
   U32                           protId = 0;

   logInfo("%s", "Unwrap received Handover Command Message");

   clearBytes( handoverCmd, sizeof(HandoverCmd) );

   rsp = &(pdu->pdu.val.successfulOutcome.value.u.sztHovrCmmd);
   noOfIes = rsp->protocolIEs.noComp.val;

   for( memIdx=0; memIdx< noOfIes; memIdx++ )
   {
     handoverCmdIe = ( rsp->protocolIEs.member + memIdx );
     protId = handoverCmdIe->id.val;
     switch( protId )
     {
       case Sztid_MME_UE_S1AP_ID:
         handoverCmd->mmeUeS1apId = handoverCmdIe->value.u.sztMME_UE_S1AP_ID.val;
         break;
       case Sztid_eNB_UE_S1AP_ID:
         handoverCmd->enbUeS1apId = handoverCmdIe->value.u.sztENB_UE_S1AP_ID.val;
         break;
       case Sztid_HovrTyp:
         handoverCmd->handoverType = handoverCmdIe->value.u.sztHovrTyp.val;
         break;
       case Sztid_NASSecurParamsfromE_UTRAN:
         /* TODO : Not Supported yet - If handoverType = LTEtoUTRAN or LTEtoGERAN */
         //s1apUnwrapNasSecurParamsFromEutran( &(handoverCmdIe->value.u.sztNASSecurParamsfromE_UTRAN),&(handoverCmd->nasSecurityParamsFromEUtran) );
         /* TODO: Log error */
         logInfo("%s", " Sztid_NASSecurParamsfromE_UTRAN Parameter not supported yet !!");
         break;
       case Sztid_E_RABSubjtoDataFwdingLst:
         handoverCmd->isERabsSubjectToFwdListUsed = TRUE;
         s1apUnwrapERabSubjectToFwdingList( &(handoverCmdIe->value.u.sztE_RABSubjtoDataFwdingLst), &(handoverCmd->eRabsSubjectToFwdList) );
         break;
       case Sztid_E_RABToBeRlsdLst:
         handoverCmd->isERabsToReleaseListUsed = TRUE;
         s1apUnwrapERabToReleaseList( &(handoverCmdIe->value.u.sztE_RABLst), &(handoverCmd->eRabsToReleaseList) );
         break;
       case Sztid_Tget_ToSrc_TprntCont:
           if ( FALSE EQ s1apUnwrapTgetToSrcTprntCont( &(handoverCmdIe->value.u.sztTget_ToSrc_TprntCont), &(handoverCmd->tgtToSrcTprntCont) ) )
           return FALSE;
         break;
       case Sztid_Tget_ToSrc_TprntCont_Secondary:
         handoverCmd->isTgtToSrcTprntContSecondary = TRUE;
         if ( FALSE EQ s1apUnwrapTgetToSrcTprntCont( &(handoverCmdIe->value.u.sztTget_ToSrc_TprntCont_Secondary), &(handoverCmd->tgtToSrcTprntContSecondary) ) )
           return FALSE;
         break;
       case Sztid_CriticalityDiag:
         handoverCmd->isCriticalDiagnosticUsed = TRUE;
         s1apUnwrapCriticalityDiagnostic( &(handoverCmdIe->value.u.sztCriticalityDiag),&(handoverCmd->criticalDiagnostic));
         break;
     }
   }
   return TRUE;
}


/**
 * @param   pdu                S1AP PDU
 * @param   handoverPrepFailed Handover PreparationFailed
 * @details This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 *          values in handoverPrepFailed message.
 */
TBOOL s1apUnWrapHandoverPrepFailedMsgPdu(
  IO S1apPdu                  *pdu,
  IO HandoverPrepFailed       *handoverPrepFailed
  )
{
  SztProtIE_Field_HovrPrepFailIEs   *handoverPrepFailIe = NULL;
  SztHovrPrepFail                   *rspFailed = NULL;
   UINT16                            noOfIes,memIdx;
   U32                               protId = 0;

   logInfo("%s", "Unwrap received Handover Preparation Failed Message");

   clearBytes( handoverPrepFailed, sizeof(HandoverPrepFailed) );

   rspFailed = &(pdu->pdu.val.unsuccessfulOutcome.value.u.sztHovrPrepFail);
   noOfIes = rspFailed->protocolIEs.noComp.val;

   for( memIdx=0; memIdx < noOfIes; memIdx++ )
   {
     handoverPrepFailIe = ( rspFailed->protocolIEs.member + memIdx);
     protId = handoverPrepFailIe->id.val;
     switch( protId )
     {
       case Sztid_MME_UE_S1AP_ID:
         handoverPrepFailed->mmeUeS1apId = handoverPrepFailIe->value.u.sztMME_UE_S1AP_ID.val;
         break;
       case Sztid_eNB_UE_S1AP_ID:
         handoverPrepFailed->enbUeS1apId = handoverPrepFailIe->value.u.sztENB_UE_S1AP_ID.val;
         break;
       case Sztid_Cause:
         s1apUnwrapCause( &(handoverPrepFailIe->value.u.sztCause), &(handoverPrepFailed->cause) );
         break;
       case Sztid_CriticalityDiag:
         handoverPrepFailed->isCriticalDiagnosticUsed = TRUE;
         s1apUnwrapCriticalityDiagnostic( &(handoverPrepFailIe->value.u.sztCriticalityDiag),&(handoverPrepFailed->criticalDiagnostic));
         break;
     }
   }
   return TRUE;
}

/**
 * @param   pdu                S1AP PDU
 * @param   s1ap Error indication  
 * @details This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 *          values in s1ap error indiaction message.
 */
TBOOL s1apUnwrapErrInd( 
    IO S1apPdu *pdu, 
    IO ErrInd *errInd 
 )
{
  SztProtIE_Field_ErrIndIEs  *errIndIe = NULLP;
  SztErrInd                  *rsp      = NULLP;
  UINT16                      noOfIes,memIdx;
  U32                         protId = 0;

  logInfo("%s","Unwarp Error Indication Received");

  clearBytes (errInd,sizeof(SztProtIE_Field_ErrIndIEs)); 
  rsp = &(pdu->pdu.val.initiatingMsg.value.u.sztErrInd);
  noOfIes = rsp->protocolIEs.noComp.val;
  
  for( memIdx=0; memIdx< noOfIes; memIdx++ )
  {
    errIndIe = ( rsp->protocolIEs.member + memIdx);
    protId = errIndIe->id.val;

    switch (protId)
    {
      case Sztid_MME_UE_S1AP_ID:
        errInd->mmeUeS1apId = errIndIe->value.u.sztMME_UE_S1AP_ID.val;
        break;
      case Sztid_eNB_UE_S1AP_ID:
        errInd->enbUeS1apId = errIndIe->value.u.sztENB_UE_S1AP_ID.val;
        break;
      case Sztid_Cause:
        s1apUnwrapCause( &(errIndIe->value.u.sztCause), &(errInd->cause) );
        break;
      case Sztid_CriticalityDiag:
        errInd->isCriticalDiagnostic = TRUE;
        s1apUnwrapCriticalityDiagnostic( &(errIndIe->value.u.sztCriticalityDiag),&(errInd->criticalDiagnostic));
        break;
    }
  }
  return TRUE;
}


/**
 * @file    s1ap_wrap.c
 * @details Copyright Powerwave Technologies, Inc., 2011
 * @details This file contains S1AP Wrapper structures and the fill functions
 *
 * @author Lakshmi Narayana Madala
 * @date   2011
 * @details
 * Change History
 *    Shweta Polepally :30th Aug 2011 : Added Code under HO_SUPPORTED
 */

#include "ccpu_cmn.h"

/* RRM related includes */
#include "lueh.h"
#include "lueh.x"
#include "ueh_eut_x.h"
#include "ueh.h"
#include "s1ap_cm.h"

#include "s1ap_wrap.h"
#include "log.h"
#include "imc_err.h"
#include "sz.h"
#define s1apWrapIeDefFields( ie, ieName, ieCrit ) { \
                                              uehSzFillTknU8( &(ie->pres),PRSNT_NODEF);  \
                                              uehSzFillTknU32( &(ie->id),(ieName) ); \
                                              uehSzFillTknU32( &(ie->criticality), (ieCrit) ); \
                                              }
/**
 * @param   pdu S1AP PDU
 * @param   msgId
 * @details
 * This functions fills the procedure code and the Criticality of the Procedure Code IE.
 */
TBOOL s1apWrapMsgFields(
    IO S1apPdu *pdu,
    IN UINT16   msgId
    )
{
  uehSzFillTknU8(&(pdu->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
  uehSzFillTknU32(&(pdu->pdu.val.initiatingMsg.procedureCode), msgId);
  uehSzFillTknU32(&(pdu->pdu.val.initiatingMsg.criticality), SztCriticalityrejectEnum);

  return TRUE;
}

/**
 * @param   pdu    S1AP PDU
 * @param   msgId
 * @details This functions fills the Initiating message code in S1 PDU.
 */
TBOOL s1apWrapInitMsgId(
    IO S1apPdu *pdu,
    IN UINT16   msgId
    )
{
  /*uehSzFillTkn functions will be referred from sz_utl.c file*/
  uehSzFillTknU8(&(pdu->pdu.choice), SZT_TRGR_INTI_MSG);
  s1apWrapMsgFields( pdu, msgId );

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   msgId
 * @details This functions fills the Successful message code in S1 PDU.
 */
TBOOL s1apWrapSuccMsgId(
    IO S1apPdu *pdu,
    IN UINT16   msgId
    )
{

  /*uehSzFillTkn functions will be referred from sz_utl.c file*/
  uehSzFillTknU8(&(pdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
  s1apWrapMsgFields( pdu, msgId );

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   msgId
 * @details
 * This functions fills the Unsuccessful message code in S1 PDU.
 */
TBOOL s1apWrapUnsuccMsgId(
    IO S1apPdu *pdu,
    IN UINT16   msgId
    )
{

  /*uehSzFillTkn functions will be referred from sz_utl.c file*/
  uehSzFillTknU8(&(pdu->pdu.choice), SZT_TRGR_UNSUCCESS_OUTCOME);
  s1apWrapMsgFields( pdu, msgId );

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   noIes Number of IE's in this PDU
 * @details
 * This functions fills the Number of IE's count in S1 PDU.
 * We have the count filed as the first element in all the messages,
 * to make it generic we typecast it to Setup, it could be any initiating message
 */
TBOOL s1apWrapIeCount(
    IO S1apPdu *pdu,
    IN UINT16   noIes
    )
{
  uehSzFillTknU8(&(pdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.\
           pres),PRSNT_NODEF);
  uehSzFillTknU16(&(pdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.\
           protocolIEs.noComp),noIes);

  return TRUE;
}

/**
 * @param   causePdu S1 Cause Pdu
 * @param   cause
 * @details
 * This functions fills the causePdu with the values in cause.
 */
TBOOL s1apWrapCause(
    IO SztCause  *causePdu,
    IN S1Cause   *cause
    )
{
  uehSzFillTknU8(&(causePdu->choice ), cause->causeType);
  uehSzFillTknU32((TknU32*)&(causePdu->val), cause->causeVal);
  return TRUE;
}

/**
 * @param   ieCriticalDiagnosticPdu
 * @param   ieCriticalDiagnostic
 * @details
 * This functions fills the ieCriticalDiagnosticPdu with the values in ieCriticalDiagnostic.
 */
void s1apWrapIeCriticalDiagnosticItem(
    IO SztCriticalityDiag_IE_Item   *ieCriticalDiagnosticPdu,
    IN IeCriticalDiagnostic         *ieCriticalDiagnostic
    )
{
  uehSzFillTknU8(&(ieCriticalDiagnosticPdu->pres), PRSNT_NODEF);
  uehSzFillTknU32( &(ieCriticalDiagnosticPdu->iECriticality), ieCriticalDiagnostic->ieCriticality );
  uehSzFillTknU32( &(ieCriticalDiagnosticPdu->iE_ID), ieCriticalDiagnostic->ieId );
  uehSzFillTknU32( &(ieCriticalDiagnosticPdu->typeOfErr), ieCriticalDiagnostic->typeOfError );

  /* Fill No Extensions */
  /* Fill No Extensions */
  ieCriticalDiagnosticPdu->iE_Extns.noComp.pres = NOTPRSNT;
  ieCriticalDiagnosticPdu->iE_Extns.noComp.val = 0;
}

/**
 * @param   pdu S1AP PDU
 * @param   sztCriticalityDiag
 * @param   criticalDiagnostic
 * @details
 * This functions allocates memory for sztCriticalityDiag in the S1AP PDU,
 * Fills the sztCriticalityDiag with the values in criticalDiagnostic.
 */
TBOOL s1apWrapCriticalDiagnostic(
    IO S1apPdu              *pdu,
    IO SztCriticalityDiag   *sztCriticalityDiag,
    IN CriticalDiagnostic   *criticalDiagnostic
    )
{
  UINT16 indx, numComp;
  SztCriticalityDiag_IE_Item *critDiagIe;
  uehSzFillTknU8(&(sztCriticalityDiag->pres), PRSNT_NODEF);

  if( TRUE == criticalDiagnostic->isProcedureCode )
  {
    uehSzFillTknU32( &(sztCriticalityDiag->procedureCode), criticalDiagnostic->procedureCode );
  }
  if( TRUE == criticalDiagnostic->isTriggeringMsg )
  {
    uehSzFillTknU32( &(sztCriticalityDiag->triggeringMsg ), criticalDiagnostic->triggeringMsg );
  }
  if( TRUE == criticalDiagnostic->isProcedureCriticality )
  {
    uehSzFillTknU32( &(sztCriticalityDiag->procedureCriticality), criticalDiagnostic->procedureCriticality );
  }

  numComp = criticalDiagnostic->noOfIeCriticalDiagnostics;

  /* Allocate memory for CSG items */
  if ((cmGetMem(pdu, numComp*sizeof(SztCriticalityDiag_IE_Item),
             (Ptr*)&(sztCriticalityDiag->iEsCriticalityDiag.member))) !=ROK)
  {
    logError("cmGetMem failed for CriticalDiagnostic Item of size:%d", (numComp*sizeof(SztCriticalityDiag_IE_Item)) );
    return FALSE;
  }
  uehSzFillTknU16(&(sztCriticalityDiag->iEsCriticalityDiag.noComp), numComp);

  for( indx=0; indx < numComp; indx++ )
  {
    critDiagIe = (sztCriticalityDiag->iEsCriticalityDiag.member + indx );
    s1apWrapIeCriticalDiagnosticItem( (critDiagIe), &(criticalDiagnostic->ieCriticalDiagnostic[indx]) );
  }

  /* Fill No Extensions */
  sztCriticalityDiag->iE_Extns.noComp.pres = NOTPRSNT;
  sztCriticalityDiag->iE_Extns.noComp.val = 0;

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   plmnIdPdu
 * @param   plmnId
 * @details
 * This functions allocates memory for plmnIdPdu in the S1AP PDU,
 * Fills the plmnIdPdu with the values in plmnId.
 */
TBOOL s1apWrapPlmnId(
    IO S1apPdu          *pdu,
    IO SztPLMNidentity  *plmnIdPdu,
    IN UINT8            *plmnId
    )
{
  if ((cmGetMem(pdu, SIZE_PLMNIDENTITY,
             (Ptr*)&(plmnIdPdu->val))) !=ROK)
  {
    logError("cmGetMem failed for PLMNID of size:%d", SIZE_PLMNIDENTITY );
    return FALSE;
  }
  plmnIdPdu->pres = PRSNT_NODEF;
  plmnIdPdu->len = SIZE_PLMNIDENTITY;
  memcpy( (plmnIdPdu->val),
          (plmnId), SIZE_PLMNIDENTITY );

  return TRUE;
}

/**
 * @param   tacPdu
 * @param   tac
 * @details
 * This functions fills the tacPdu with the values in tac.
 */
TBOOL s1apWrapTac(
    IO SztTAC  *tacPdu,
    IN UINT8   *tac
    )
{
  tacPdu->pres = TRUE;

  tacPdu->len = SIZE_TAC ;

  tacPdu->val[0] = *( tac + 0);
  tacPdu->val[1] = *( tac + 1);

  return TRUE;
}

/**
 * @param   mmeGrpIdPdu
 * @param   mmeGrpId
 * @details
 * This functions fills the mmeGrpIdPdu with the values in mmeGrpId.
 */
TBOOL s1apWrapMmeGrpId(
    IO SztMME_Group_ID  *mmeGrpIdPdu,
    IN UINT8            *mmeGrpId
    )
{
  mmeGrpIdPdu->pres = TRUE;
  mmeGrpIdPdu->len = SIZE_MMEGROUPID ;

  mmeGrpIdPdu->val[0] = *( mmeGrpId + 0);
  mmeGrpIdPdu->val[1] = *( mmeGrpId + 0);

  return TRUE;
}

/**
 * @param   mmeCodePdu
 * @param   mmeCode
 * @details
 * This functions fills the mmeCodePdu with the values in mmeCode.
 */
TBOOL s1apWrapMmeCode(
    IO SztMME_Code  *mmeCodePdu,
    IN UINT8        mmeCode
    )
{
  mmeCodePdu->pres = TRUE;
  mmeCodePdu->len = SIZE_MMECODE ;

  mmeCodePdu->val[0] = mmeCode;

  return TRUE;
}

/**
 * @param   csgPdu
 * @param   csgId
 * @details
 * This functions fills the csgPdu with the values in csgId.
 */
TBOOL s1apWrapCsgId(
    IO SztCSG_Id  *csgPdu,
    IN CsgId      *csgId
    )
{
  csgPdu->pres = TRUE;
  csgPdu->len = SIZE_CSGID ;

  memcpy( (csgPdu->val), csgId->csgId, MAX_SIZE_CSGID);

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   mTmsiPdu
 * @param   mTmsi
 * @details
 * This functions allocates memory for mTmsiPdu in the S1AP PDU,
 * Fills the mTmsiPdu with the values in mTmsi.
 */
TBOOL s1apWrapMTmsi(
    IO S1apPdu     *pdu,
    IO SztM_TMSI   *mTmsiPdu,
    IN MTimsi      *mTmsi
    )
{
  if ((cmGetMem(pdu, MAX_SIZE_M_TIMSI,
             (Ptr*)&(mTmsiPdu->val)) ) !=ROK)
  {
    logError("cmGetMem failed for M-TMSI of size:%d", MAX_SIZE_M_TIMSI );
    return FALSE;
  }

  /* ENB Name */
  mTmsiPdu->pres = PRSNT_NODEF;
  mTmsiPdu->len = MAX_SIZE_M_TIMSI;
  memcpy( (mTmsiPdu->val), mTmsi, MAX_SIZE_M_TIMSI ) ;

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   sTmsiPdu
 * @param   sTmsi
 * @details
 * This functions allocates memory for sTmsiPdu in the S1AP PDU,
 * Fills the sTmsiPdu with the values in sTmsi.
 */
TBOOL s1apWrapSTmsi(
    IO S1apPdu    *pdu,
    IO SztS_TMSI  *sTmsiPdu,
    IN STimsi     *sTmsi
    )
{
  uehSzFillTknU8(&(sTmsiPdu->pres), PRSNT_NODEF);

  /* Fill MME Code */
  s1apWrapMmeCode( &(sTmsiPdu->mMEC),sTmsi->mmeCode );

  /* Fill M-TMSI */
  s1apWrapMTmsi( pdu, &(sTmsiPdu->m_TMSI), &(sTmsi->mTimsi) );

  /* Fill No Extensions */
  sTmsiPdu->iE_Extns.noComp.pres = NOTPRSNT;
  sTmsiPdu->iE_Extns.noComp.val = 0;

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   gummeiPdu CCPU GUMMEI Id Pdu
 * @param   gummei PWAV GUMMEI Id
 * @details
 * This functions allocates memory for gummeiPdu in the S1AP PDU,
 * Fills the gummeiPdu with the values in gummei.
 */
TBOOL s1apWrapGummei(
    IO S1apPdu    *pdu,
    IO SztGUMMEI  *gummeiPdu,
    IN Gummei     *gummei
    )
{
  uehSzFillTknU8(&(gummeiPdu->pres), PRSNT_NODEF);

  /* Fill PLMN Identity */
  if( FALSE == s1apWrapPlmnId( pdu, &(gummeiPdu->pLMN_Identity), (gummei->plmnId) ) )
    return FALSE;

  /* Fill MME-GRP ID */
  s1apWrapMmeGrpId( &(gummeiPdu->mME_Group_ID), gummei->mmeGroupId );

  /* Fill MME Code */
  s1apWrapMmeCode( &(gummeiPdu->mME_Code), gummei->mmeId );

  /* Fill No Extensions */
  gummeiPdu->iE_Extns.noComp.pres = NOTPRSNT;
  gummeiPdu->iE_Extns.noComp.val = 0;

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   enbIdPdu CCPU enodeB Id
 * @param   enbId PWAV CCPU enodeB Id
 * @details
 * This functions allocates memory for enbIdPdu in the S1AP PDU,
 * Fills the enbIdPdu with the values in enbId.
 */
TBOOL s1apWrapEnodeBId(
    IO S1apPdu           *pdu,
    IO SztGlobal_ENB_ID  *enbIdPdu,
    IN GlobalEnodeBId    *enbId
    )
{
  uehSzFillTknU8(&(enbIdPdu->pres), PRSNT_NODEF);

  if( FALSE == s1apWrapPlmnId( pdu, &(enbIdPdu->pLMNidentity), (enbId->plmnIdentity) ) )
    return FALSE;

  /* ENB Id, We always Use MAcro EnodeBID  */
  if( EnbType_MacroEnodeB == enbId->enodeBType )
  {
    uehSzFillTknU8(&(enbIdPdu->eNB_ID.choice), ENB_ID_MACROENB_ID);
    enbIdPdu->eNB_ID.val.macroENB_ID.pres = PRSNT_NODEF;
    enbIdPdu->eNB_ID.val.macroENB_ID.len = SIZE_MACROENBID;
    memcpy( (enbIdPdu->eNB_ID.val.macroENB_ID.val),
        (enbId->enbId), MAX_SIZE_ENBID );
  }
  else
  {
    uehSzFillTknU8(&(enbIdPdu->eNB_ID.choice), ENB_ID_HOMEENB_ID);
    enbIdPdu->eNB_ID.val.homeENB_ID.pres = PRSNT_NODEF;
    enbIdPdu->eNB_ID.val.homeENB_ID.len = SIZE_HOMEENBID;
    memcpy( (enbIdPdu->eNB_ID.val.homeENB_ID.val),
        (enbId->enbId), MAX_SIZE_ENBID );

  }
  enbIdPdu->iE_Extns.noComp.pres = NOTPRSNT;
  enbIdPdu->iE_Extns.noComp.val = 0;

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   enbNamePdu CCPU eNodeB Name Pdu
 * @param   enbName PWAV eNodeB Name Pdu
 * @details
 * This functions allocates memory for enbNamePdu in the S1AP PDU,
 * Fills the enbNamePdu with the values in enbName.
 */
TBOOL s1apWrapEnodeName(
    IO S1apPdu     *pdu,
    IO SztENBname  *enbNamePdu,
    IN UINT8       *enbName
    )
{
  UINT16 len;

  /* warning: conversion from unsigned to signed conversion ignored */
  len = strlen((const S8 *)enbName);
  if ((cmGetMem(pdu, len,
             (Ptr*)&(enbNamePdu->val)) ) !=ROK)
  {
    logError("cmGetMem failed for EnbNAME of size:%d", len );
    return FALSE;
  }

  /* ENB Name */
  enbNamePdu->pres = PRSNT_NODEF;
  enbNamePdu->len = len;
  memcpy( (enbNamePdu->val), enbName, len ) ;

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   eutranTraceIdPdu CCPU E-UTRAN Trace ID Pdu
 * @param   eutranTraceId PWAV E-UTRAN Trace ID Pdu
 * @details
 * This functions allocates memory for eutranTraceIdPdu in the S1AP PDU,
 * Fills the eutranTraceIdPdu with the values in eutranTraceId.
 */
TBOOL s1apWrapEutranTraceId(
    IO S1apPdu               *pdu,
    IO SztE_UTRAN_Trace_ID   *eutranTraceIdPdu,
    IO EUtranTraceId         *eutranTraceId
    )
{
  UINT16 len;

  /* warning: conversion from unsigned to signed conversion ignored */
  len = EUTRAN_TRACE_ID_LEN;
  if ((cmGetMem(pdu, len,
             (Ptr*)&(eutranTraceIdPdu->val)) ) !=ROK)
  {
    logError("cmGetMem failed for E-UTRAN Trace ID of size:%d", len );
    return FALSE;
  }

  /* Copy the TraceId */
  eutranTraceIdPdu->pres = PRSNT_NODEF;
  eutranTraceIdPdu->len = len;
  memcpy( (eutranTraceIdPdu->val), eutranTraceId, len ) ;

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   csgListPdu CCPU Closed subscriber Group PDU
 * @param   csgList PWAV Closed subscriber Group
 * @details
 * This functions allocates memory for csgListPdu in the S1AP PDU,
 * Fills the csgListPdu with the values in csgList.
 */
TBOOL s1apWrapCsgList(
    IO S1apPdu       *pdu,
    IO SztCSG_IdLst  *csgListPdu,
    IN CsgIdList     *csgList
    )
{
  UINT16             numComp, indx;
  SztCSG_IdLst_Item *csgItem;

  numComp = csgList->numOfSupportedCsg;

  /* Allocate memory for CSG items */
  if ((cmGetMem(pdu, numComp*sizeof(SztCSG_IdLst_Item),
             (Ptr*)&(csgListPdu->member))) !=ROK)
  {
    logError("cmGetMem failed for CsgItem of size:%d", numComp*sizeof(SztCSG_IdLst_Item) );
    return FALSE;
  }
  uehSzFillTknU16(&(csgListPdu->noComp),numComp);

  /* Filling CSG item*/
  for( indx=0; indx< numComp; indx++ )
  {
    csgItem = &(csgListPdu->member[indx]);
    uehSzFillTknU8(&(csgItem->pres),PRSNT_NODEF);

    s1apWrapCsgId( &(csgItem->cSG_Id), &(csgList->csgId[indx]) );

    csgItem->iE_Extns.noComp.pres = NOTPRSNT;
    csgItem->iE_Extns.noComp.val = 0;
 } /* Supported CSGItem */

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   tacListPdu CCPU TAC List Pdu
 * @param   tacList PWAV TAC List Pdu
 * @details
 * This functions allocates memory for tacListPdu in the S1AP PDU,
 * Fills the tacListPdu with the values in tacList.
 */
TBOOL s1apWrapSupportedTacs(
    IO S1apPdu       *pdu,
    IO SztSuppTAs    *tacListPdu,
    IN SupportedTacs *tacList
    )
{
  UINT16  numTacComp, numPlmncomp, indx, plmnIndx;
  SztSuppTAs_Item *tACItem;

  numTacComp = tacList->numOfSupportedTac;
  /* Allocate memory for TA items */
  if ((cmGetMem(pdu, numTacComp*sizeof(SztSuppTAs_Item),
             (Ptr*)&(tacListPdu->member))) !=ROK)
  {
    logError("cmGetMem failed for SupportedTA's of size:%d", numTacComp*sizeof(SztSuppTAs_Item) );
    return FALSE;
  }

  uehSzFillTknU16(&(tacListPdu->noComp),numTacComp);

  /* Filling TA item 1 */
  for( indx=0; indx< numTacComp; indx++ )
  {
    tACItem = &(tacListPdu->member[indx]);
    uehSzFillTknU8(&(tACItem->pres),PRSNT_NODEF);

    /* TAC  */
    s1apWrapTac( &(tACItem->tAC), tacList->tacs[indx].tac );

    /* Allocate memory for broadcast PLMNs, Filling broadcastPLMNs */
    numPlmncomp = tacList->tacs[indx].servedBroadcastPlmns.numPlmns;
    if((cmGetMem(pdu, numPlmncomp * sizeof(SztPLMNidentity), \
             (Ptr*)&(tACItem->broadcastPLMNs.member))) !=ROK)
    {
      logError("cmGetMem failed for Broadcast PLMNID's of size:%d", numPlmncomp * sizeof(SztPLMNidentity) );
      RETVALUE(RFAILED);
   }
    uehSzFillTknU16(&(tACItem->broadcastPLMNs.noComp),numPlmncomp);
    for( plmnIndx=0; plmnIndx< numPlmncomp; plmnIndx++ )
    {
      if( FALSE == s1apWrapPlmnId( pdu, &(tACItem->broadcastPLMNs.member[plmnIndx]),
                                 (tacList->tacs[indx].servedBroadcastPlmns.plmnId[plmnIndx]) ) )
        return FALSE;
    }
 } /* Supported TAC */

  return TRUE;
}

/**
 * @param   drxPdu CCPU Default Paging DRX Pdu
 * @param   defPagingDrx PWAV Default Paging DRX Pdu
 * @details
 * This functions fills the drxPdu with the values in defPagingDrx.
 */
TBOOL s1apWrapDefaultPagingDrx(
    IO SztPagDRX  *drxPdu,
    IN UINT16     defPagingDrx
    )
{
  uehSzFillTknU32( drxPdu, defPagingDrx);

  return TRUE;
}

/**
 * @param   rrcEstablishmentCausePdu
 * @param   rrcEstablishmentCause
 * @details
 * This functions fills the rrcEstablishmentCausePdu with the values in rrcEstablishmentCause.
 */
TBOOL s1apWrapRrcEstablishmentCause(
    IO SztRRC_Establishment_Cause  *rrcEstablishmentCausePdu,
    IN UINT16                      rrcEstablishmentCause
    )
{
  uehSzFillTknU32( rrcEstablishmentCausePdu, rrcEstablishmentCause);

  return TRUE;
}

/**
 * @param   conItemPdu CCPU Connection Item of Reset
 * @param   conItem PWAV Connection Item of Reset
 * @details
 * This functions fills the conItemPdu with the values in conItem.
 */
void s1apWrapUeAssocLogConnItem(
    IO SztUE_assocLogS1_ConItem   *conItemPdu,
    IN S1UeAssocLogConnItem       *conItem
    )
{
  uehSzFillTknU8(&(conItemPdu->pres),PRSNT_NODEF);
  if( TRUE == conItem->isMmeUeS1apId)
    uehSzFillTknU32(&(conItemPdu->mME_UE_S1AP_ID), conItem->mmeUeS1apId );
  if( TRUE == conItem->isEnbUeS1apId)
    uehSzFillTknU32(&(conItemPdu->eNB_UE_S1AP_ID), conItem->enbUeS1apId );

  /* Fill No Extensions */
  conItemPdu->iE_Extns.noComp.pres = NOTPRSNT;
  conItemPdu->iE_Extns.noComp.val = 0;
}

/**
 * @param   pdu S1AP PDU
 * @param   rstConListPdu CCPU UE S1 Reset Connection List
 * @param   conList PWAV UE S1 Reset Connection List
 * @details
 * This functions allocates memory for rstConListPdu in the S1AP PDU,
 * Fills the rstConListPdu with the values in conList.
 */
TBOOL s1apWrapRstUeAssocLogConnList(
    IO S1apPdu                      *pdu,
    IO SztUE_assocLogS1_ConLstRes   *rstConListPdu,
    IN S1UeAssocLogConnList         *conList
    )
{
  UINT16 numComp, indx;
  SztProtIE_Field_UE_assocLogS1_ConItemRes *rstUeAssocLogConn;
  SztUE_assocLogS1_ConItem *conItem;

  numComp = conList->numOfS1UeAssocLogConnItems;
  /* Allocate memory for CSG items */
  if ((cmGetMem(pdu, numComp*sizeof(SztProtIE_Field_UE_assocLogS1_ConItemRes),
             (Ptr*)&(rstConListPdu->member))) !=ROK)
  {
    logError("cmGetMem failed for UE AssociatedLogicalConnectionItem's of size:%d", numComp*sizeof(SztProtIE_Field_UE_assocLogS1_ConItemRes) );
    return FALSE;
  }

  uehSzFillTknU16( &(rstConListPdu->noComp),numComp);

  for( indx=0; indx< numComp; indx++ )
  {
    rstUeAssocLogConn = &(rstConListPdu->member[indx]);
    s1apWrapIeDefFields( rstUeAssocLogConn, Sztid_UE_assocLogS1_ConItem, SztCriticalityrejectEnum );
    conItem = &(rstUeAssocLogConn->value.u.sztUE_assocLogS1_ConItem);
    s1apWrapUeAssocLogConnItem( conItem, &(conList->s1ConnItem[indx]));
  }
  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   rstAckConListPdu CCPU UE S1 Reset Ack Connection List
 * @param   conList PWAV UE S1 Reset Ack Connection List
 * @details
 * This functions allocates memory for rstAckConListPdu in the S1AP PDU,
 * Fills the rstAckConListPdu with the values in conList.
 */
TBOOL s1apWrapRstAckUeAssocLogConnList(
    IO S1apPdu                       *pdu,
    IO SztUE_assocLogS1_ConLstResAck *rstAckConListPdu,
    IN S1UeAssocLogConnList          *conList
    )
{
  UINT16 numComp, indx;
  SztProtIE_Field_UE_assocLogS1_ConItemResAck *ueAssocLogConnRstAck;
  SztUE_assocLogS1_ConItem *conItem;

  numComp = conList->numOfS1UeAssocLogConnItems;
  /* Allocate memory for CSG items */
  if ((cmGetMem(pdu, numComp*sizeof(SztProtIE_Field_UE_assocLogS1_ConItemResAck),
             (Ptr*)&(rstAckConListPdu->member))) !=ROK)
  {
    logError("cmGetMem failed for UE AssociatedLogicalConnectionItemResAck's of size:%d", numComp*sizeof(SztProtIE_Field_UE_assocLogS1_ConItemResAck) );
    return FALSE;
  }

  uehSzFillTknU16( &(rstAckConListPdu->noComp),numComp);

  for( indx=0; indx< numComp; indx++ )
  {
    ueAssocLogConnRstAck = &(rstAckConListPdu->member[indx]);
    s1apWrapIeDefFields( ueAssocLogConnRstAck, Sztid_UE_assocLogS1_ConItem, SztCriticalityrejectEnum );
    conItem = &(ueAssocLogConnRstAck->value.u.sztUE_assocLogS1_ConItem);
    s1apWrapUeAssocLogConnItem( conItem, &(conList->s1ConnItem[indx]));
  }
  return TRUE;
}

/**
 * @param   enbUeS1apIdPdu CCPU ENB-UES1AP Id
 * @param   enbUeId PWAV ENB-UES1AP Id
 * @details
 * This functions fills the enbUeS1apIdPdu with the values in enbUeId.
 */
void s1apWrapEnodeBUeS1apId(
    IO SztENB_UE_S1AP_ID  *enbUeS1apIdPdu,
    IN EnbUeS1apId        enbUeId
    )
{
  uehSzFillTknU32(enbUeS1apIdPdu, enbUeId);
}

/**
 * @param   timeToWaitPdu CCPU Time To Wait
 * @param   timeToWait PWAV Time To Wait
 * @details
 * This functions fills the timeToWaitPdu with the values in timeToWait.
 */
void s1apWrapTimeToWait(
    IO SztTimeToWait  *timeToWaitPdu,
    IN TimeToWait      timeToWait
    )
{
  uehSzFillTknU32(timeToWaitPdu, timeToWait);
}

/**
 * @param   locReqtypePdu CCPU Location Request Type
 * @param   locReqtype PWAV Location Request Type
 * @details
 * This functions fills the locReqtypePdu with the values in locReqtype.
 */
void s1apWrapLocReportCntrlReqType(
    IO SztRqstTyp                  *locReqtypePdu,
    IN LocReportCntrlReqType       *locReqtype
    )
{
  uehSzFillTknU8( &(locReqtypePdu->pres),PRSNT_NODEF);

  uehSzFillTknU32( &(locReqtypePdu->eventTyp), locReqtype->locReportTypeEvent);
  uehSzFillTknU32( &(locReqtypePdu->reportArea), locReqtype->locReportArea);

  /* Fill No Extensions */
  locReqtypePdu->iE_Extns.noComp.pres = NOTPRSNT;
  locReqtypePdu->iE_Extns.noComp.val = 0;
}

/**
 * @param   mmeUeS1apIdPdu CCPU MME-UES1AP ID
 * @param   mmeUeId PWAV MME-UES1AP ID
 * @details
 * This functions fills the mmeUeS1apIdPdu with the values in mmeUeId.
 */
void s1apWrapMmeUeS1apId(
    IO SztMME_UE_S1AP_ID  *mmeUeS1apIdPdu,
    IN MmeUeS1apId        mmeUeId
    )
{
  uehSzFillTknU32(mmeUeS1apIdPdu, mmeUeId);
}

/**
 * @param   pdu S1AP PDU
 * @param   s1NasPdu CCPU NAS Pdu
 * @param   nasPdu PWAV NAS Pdu
 * @details
 * This functions allocates memory for s1NasPdu in the S1AP PDU,
 * Fills the s1NasPdu with the values in nasPdu.
 */
TBOOL s1apWrapNasPdu(
    IO S1apPdu     *pdu,
    IO SztNAS_PDU  *s1NasPdu,
    IN NasPdu      *nasPdu
    )
{
  UINT16   len;

  len = nasPdu->pduLen;
  if ((cmGetMem(pdu, len,
             (Ptr*)&(s1NasPdu->val)) ) !=ROK)
  {
    logError("cmGetMem failed for NAS of size:%d", len );
    return FALSE;
  }

  /* NAS PDU */
  s1NasPdu->pres = PRSNT_NODEF;
  s1NasPdu->len = len;
  memcpy( (s1NasPdu->val), nasPdu->pdu, len ) ;

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   ueRadioCapPdu CCPU UE Radio Capability PDU
 * @param   ueRadioCapPdu PWAV UE Radio Capability PDU
 * @details
 * This functions allocates memory for ueRadioCapPdu in the S1AP PDU,
 * Fills the ueRadioCapPdu with the values in ueRadioCapPdu.
 */
TBOOL s1apWrapUeRadioCapInfoPdu(
    IO S1apPdu             *pdu,
    IO SztUERadioCapblty   *ueRadioCapPdu,
    IO UeRadioCapInfo      *ueRadioCap
    )
{
  UINT16   len;

  len = ueRadioCap->infoLen;
  if ((cmGetMem(pdu, len, (Ptr*)&(ueRadioCapPdu->val)) ) !=ROK)
  {
    logError("cmGetMem failed for UE RadioCapability PDU of size:%d", len );
    return FALSE;
  }

  /* UE Radio Capability PDU */
  ueRadioCapPdu->pres = PRSNT_NODEF;
  ueRadioCapPdu->len = len;

  UEH_MEM_COPY(ueRadioCapPdu->val, ueRadioCap->capInfopdu, len) ;

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   taiPdu CCPU Tracking Area Identity
 * @param   tai PWAV  Tracking Area Identity
 * @details
 * This functions allocates memory for taiPdu in the S1AP PDU,
 * Fills the taiPdu with the values in tai.
 */
TBOOL s1apWrapTai(
    IO S1apPdu   *pdu,
    IO SztTAI    *taiPdu,
    IN Tai       *tai
    )
{
  uehSzFillTknU8(&(taiPdu->pres), PRSNT_NODEF);

  /* Fill PLMN ID */
  if( FALSE == s1apWrapPlmnId( pdu, &(taiPdu->pLMNidentity), tai->plmnIdentity ) )
    return FALSE;

  /* Fill TAC */
  s1apWrapTac( &(taiPdu->tAC), tai->tac );

  taiPdu->iE_Extns.noComp.pres = NOTPRSNT;

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   eUtranCgiPdu CCPU E-UTRAN CGI Id
 * @param   eUtranCgi PWAV E-UTRAN CGI Id
 * @details
 * This functions allocates memory for eUtranCgiPdu in the S1AP PDU,
 * Fills the eUtranCgiPdu with the values in eUtranCgi.
 */
TBOOL s1apWrapEUtranCgi(
    IO S1apPdu        *pdu,
    IO SztEUTRAN_CGI  *eUtranCgiPdu,
    IN EUtranCgi      *eUtranCgi
    )
{

  uehSzFillTknU8(&(eUtranCgiPdu->pres), PRSNT_NODEF);

  /* Fill PLMN ID */
  if( FALSE == s1apWrapPlmnId( pdu, &(eUtranCgiPdu->pLMNidentity), eUtranCgi->plmnIdentity ) )
    return FALSE;

  /* Fill Cell ID */
  eUtranCgiPdu->cell_ID.pres = PRSNT_NODEF;
  eUtranCgiPdu->cell_ID.len = SIZE_CELLID;
  memcpy( (eUtranCgiPdu->cell_ID.val),
      (eUtranCgi->cellIdentity), MAX_SIZE_CELLID );

  eUtranCgiPdu->iE_Extns.noComp.pres = NOTPRSNT;
  return TRUE;
}
/**
 * @param   sztE_RABItem CCPU RAB Item
 * @param   eRabListItem PWAV RAB Item
 * @details
 * This functions fills the sztE_RABItem with the values in eRabListItem.
 */
void s1apWrapERabListItem(
    IO SztE_RABItem   *sztE_RABItem,
    IN ERabListItem   *eRabListItem
    )
{
  uehSzFillTknU8(&(sztE_RABItem->pres), PRSNT_NODEF);
  uehSzFillTknU32( &(sztE_RABItem->e_RAB_ID), eRabListItem->eRabId );
  s1apWrapCause( &(sztE_RABItem->cause), &(eRabListItem->cause) );

  /* Fill No Extensions */
  sztE_RABItem->iE_Extns.noComp.pres = NOTPRSNT;
  sztE_RABItem->iE_Extns.noComp.val = 0;
}

/**
 * @param   pdu S1AP PDU
 * @param   gTP_TEID CCPU GTP Tunnel ID
 * @param   gtPTeId PWAV GTP Tunnel ID
 * @details
 * This functions allocates memory for gTP_TEID in the S1AP PDU,
 * Fills the gTP_TEID with the values in gtPTeId.
 */
TBOOL s1apWrapGtpTeId(
    IO S1apPdu     *pdu,
    IO SztGTP_TEID *gTP_TEID,
    IO GtPTeId     *gtPTeId
  )
{
  if( uehSzFillTknStrOSXL( gTP_TEID, GTP_TE_ID_LEN, gtPTeId->gtPTeId, &pdu ) != ROK )
  {
    logError("cmGetMem failed for GTPTEID List Item of size:%d", GTP_TE_ID_LEN );
    return FALSE;
  }

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   transportLyrAddr CCPU Transport Layer Address
 * @param   transportLayerAddr PWAV Transport Layer Address
 * @details
 * This functions allocates memory for transportLyrAddr in the S1AP PDU,
 * Fills the transportLyrAddr with the values in transportLayerAddr.
 */
TBOOL s1apWrapTransportLayerAddr(
    IO S1apPdu               *pdu,
    IO SztTportLyrAddr       *transportLyrAddr,
    IN TransportLayerAddr    *transportLayerAddr
    )
{
  UINT16 len;

  switch( transportLayerAddr->transportAddrType )
  {
    case TrasportAddrType_Ipv4:
      len =SIZE_IPV4_ADDR;
      break;
    case TrasportAddrType_Ipv6:
      len =SIZE_IPV6_ADDR;
      break;
    default:
      len = SIZE_IPV4_AND_IPV6_ADDR;
      break;
  }
  if ((cmGetMem(pdu, len,
             (Ptr*)&(transportLyrAddr->val)) ) !=ROK)
  {
    logError("cmGetMem failed for Transport Layer Address size:%d", len );
    return FALSE;
  }

  /* Transport Layer Address */
  transportLyrAddr->pres = PRSNT_NODEF;
  transportLyrAddr->len = ( len * 8 ); /*We need to give the length in bits */
  memcpy( (transportLyrAddr->val), transportLayerAddr->transportLevelAddr, len ) ;

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   sztE_RABSetupItem CCPU E-RAB Setup Item
 * @param   eRabSetupItem PWAV E-RAB Setup Item
 * @details
 * This functions allocates memory for sztE_RABSetupItem in the S1AP PDU,
 * Fills the sztE_RABSetupItem with the values in eRabSetupItem.
 */
TBOOL s1apWrapERabSetupItem(
    IO S1apPdu                     *pdu,
    IO SztE_RABSetupItemCtxtSURes  *sztE_RABSetupItem,
    IO ERabSetupItem               *eRabSetupItem
    )
{
  uehSzFillTknU8(&(sztE_RABSetupItem->pres), PRSNT_NODEF);
  uehSzFillTknU32( &(sztE_RABSetupItem->e_RAB_ID), eRabSetupItem->eRabId );
  if ( FALSE == s1apWrapGtpTeId( pdu, &(sztE_RABSetupItem->gTP_TEID), &(eRabSetupItem->gtPTeId)  ) )
  {
    return FALSE;
  }

  if ( FALSE == s1apWrapTransportLayerAddr( pdu, &(sztE_RABSetupItem->transportLyrAddr), &(eRabSetupItem->transportLayerAddr)  ) )
  {
    return FALSE;
  }
  /* Fill No Extensions */
  sztE_RABSetupItem->iE_Extns.noComp.pres = NOTPRSNT;
  sztE_RABSetupItem->iE_Extns.noComp.val = 0;

  return TRUE;
}

/**
 * @param   sztE_RABModifyItem CCPU E-RAB  Modify Item
 * @param   eRabModifyItem PWAV E-RAB Modify Item
 * @details
 * This functions fills the sztE_RABModifyItem with the values in eRabModifyItem.
 */
TBOOL s1apWrapERabModifyItem(
    IO SztE_RABMdfyItemBrModRes    *sztE_RABModifyItem,
    IN ERabModifyItem              *eRabModifyItem
    )
{
  uehSzFillTknU8(&(sztE_RABModifyItem->pres), PRSNT_NODEF);
  uehSzFillTknU32( &(sztE_RABModifyItem->e_RAB_ID), eRabModifyItem->eRabId );

  /* Fill No Extensions */
  sztE_RABModifyItem->iE_Extns.noComp.pres = NOTPRSNT;
  sztE_RABModifyItem->iE_Extns.noComp.val = 0;

  return TRUE;
}

/**
 * @param   sztE_RABReleaseItem CCPU E-RAB  Release Item
 * @param   eRabReleaseItem PWAV E-RAB Release Item
 * @details
 * This functions fills the sztE_RABReleaseItem with the values in eRabReleaseItem.
 */
TBOOL s1apWrapERabReleaseItem(
    IO SztE_RABRlsItemBrRelComp    *sztE_RABReleaseItem,
    IN ERabReleaseItem             *eRabReleaseItem
    )
{
  uehSzFillTknU8(&(sztE_RABReleaseItem->pres), PRSNT_NODEF);
  uehSzFillTknU32( &(sztE_RABReleaseItem->e_RAB_ID), eRabReleaseItem->eRabId );

  /* Fill No Extensions */
  sztE_RABReleaseItem->iE_Extns.noComp.pres = NOTPRSNT;
  sztE_RABReleaseItem->iE_Extns.noComp.val = 0;

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   eztE_RABLst CCPU E-RAB List
 * @param   eRabList PWAV E-RAB List
 * @details
 * This functions allocates memory for eztE_RABLst in the S1AP PDU,
 * Fills the eztE_RABLst with the values in eRabList.
 */
TBOOL s1apWrapERabList(
    IO S1apPdu       *pdu,
    IO SztE_RABLst   *eztE_RABLst,
    IN ERabList      *eRabList
    )
{
  UINT16 indx, numComp;
  SztProtIE_SingleCont_E_RABItemIEs *eRabItemIe;

  numComp = eRabList->noItems;

  /* Allocate memory for CSG items */
  if ((cmGetMem(pdu, numComp*sizeof(SztProtIE_SingleCont_E_RABItemIEs),
             (Ptr*)&(eztE_RABLst->member))) !=ROK)
  {
    logError("cmGetMem failed for ERAB List Item:%d", numComp*sizeof(SztProtIE_SingleCont_E_RABItemIEs) );
    return FALSE;
  }
  uehSzFillTknU16( &(eztE_RABLst->noComp), numComp);

  for( indx=0; indx < numComp; indx++ )
  {
    eRabItemIe = (eztE_RABLst->member + indx);
    s1apWrapIeDefFields( eRabItemIe, Sztid_E_RABItem, SztCriticalityignoreEnum );
    s1apWrapERabListItem( &(eRabItemIe->value.u.sztE_RABItem), &(eRabList->eRabListItem[indx]) );
  }

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   sztE_RABSetupLstCtxtSURes CCPU E-RAB Init Context Setup List
 * @param   eRabSetupList PWAV E-RAB Init Context Setup List
 * @details
 * This functions allocates memory for sztE_RABSetupLstCtxtSURes in the S1AP PDU,
 * Fills the sztE_RABSetupLstCtxtSURes with the values in eRabSetupList.
 */
TBOOL s1apWrapERabSetupCtxtList(
    IO S1apPdu                   *pdu,
    IO SztE_RABSetupLstCtxtSURes *sztE_RABSetupLstCtxtSURes,
    IO ERabSetupList             *eRabSetupList
    )
{
  UINT16 indx, numComp;
  SztProtIE_Field_E_RABSetupItemCtxtSUResIEs *eRabItemIe;

  numComp = eRabSetupList->noComp;

  /* Allocate memory for CSG items */
  if ((cmGetMem(pdu, numComp*sizeof(SztProtIE_Field_E_RABSetupItemCtxtSUResIEs),
             (Ptr*)&(sztE_RABSetupLstCtxtSURes->member))) !=ROK)
  {
    logError("cmGetMem failed for ERAB Setup Item:%d", numComp*sizeof(SztProtIE_Field_E_RABSetupItemCtxtSUResIEs) );
    return FALSE;
  }
  uehSzFillTknU16( &(sztE_RABSetupLstCtxtSURes->noComp), numComp);

  for( indx=0; indx < numComp; indx++ )
  {
    eRabItemIe = (sztE_RABSetupLstCtxtSURes->member + indx);
    s1apWrapIeDefFields( eRabItemIe, Sztid_E_RABSetupItemCtxtSURes, SztCriticalityignoreEnum );
    s1apWrapERabSetupItem( pdu, &(eRabItemIe->value.u.sztE_RABSetupItemCtxtSURes), &(eRabSetupList->eRabSetupItem[indx]) );
  }

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   sztE_RABSetupLstSURes CCPU E-RAB Setup List
 * @param   eRabSetupList PWAV E-RAB Modify List
 * @details
 * This functions allocates memory for sztE_RABSetupLstSURes in the S1AP PDU,
 * Fills the sztE_RABSetupLstSURes with the values in eRabSetupList.
 */
TBOOL s1apWrapERabSetupList(
    IO S1apPdu                   *pdu,
    IO SztE_RABSetupLstBrSURes   *sztE_RABSetupLstSURes,
    IO ERabSetupList             *eRabSetupList
    )
{
  UINT16 indx, numComp;
  SztProtIE_Field_E_RABSetupItemBrSUResIEs *eRabItemIe;

  numComp = eRabSetupList->noComp;

  /* Allocate memory for CSG items */
  if ((cmGetMem(pdu, numComp*sizeof(SztProtIE_Field_E_RABSetupItemBrSUResIEs),
             (Ptr*)&(sztE_RABSetupLstSURes->member))) !=ROK)
  {
    logError("cmGetMem failed for ERABSetupResItem:%d", numComp*sizeof(SztProtIE_Field_E_RABSetupItemBrSUResIEs) );
    return FALSE;
  }
  uehSzFillTknU16( &(sztE_RABSetupLstSURes->noComp), numComp);

  for( indx=0; indx < numComp; indx++ )
  {
    eRabItemIe = (sztE_RABSetupLstSURes->member + indx);
    s1apWrapIeDefFields( eRabItemIe, Sztid_E_RABSetupItemBrSURes, SztCriticalityignoreEnum );
    s1apWrapERabSetupItem( pdu,(SztE_RABSetupItemCtxtSURes *)&(eRabItemIe->value.u.sztE_RABSetupItemBrSURes), &(eRabSetupList->eRabSetupItem[indx]) );
  }

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   sztE_RABModifyLstCtxtSURes CCPU E-RAB Modify List
 * @param   eRabModifyList PWAV E-RAB Modify List
 * @details
 * This functions allocates memory for sztE_RABModifyLstCtxtSURes in the S1AP PDU,
 * Fills the sztE_RABModifyLstCtxtSURes with the values in eRabModifyList.
 */
TBOOL s1apWrapERabModifyList(
    IO S1apPdu                   *pdu,
    IO SztE_RABMdfyLstBrModRes   *sztE_RABModifyLstCtxtSURes,
    IN ERabModifyList            *eRabModifyList
    )
{
  UINT16 indx, numComp;
  SztProtIE_Field_E_RABMdfyItemBrModResIEs *eRabItemIe;

  numComp = eRabModifyList->noComp;

  /* Allocate memory for CSG items */
  if ((cmGetMem(pdu, numComp*sizeof(SztProtIE_Field_E_RABMdfyItemBrModResIEs),
             (Ptr*)&(sztE_RABModifyLstCtxtSURes->member))) !=ROK)
  {
    logError("cmGetMem failed for ERAB Modify Item:%d", numComp*sizeof(SztProtIE_Field_E_RABMdfyItemBrModResIEs) );
    return FALSE;
  }
  uehSzFillTknU16( &(sztE_RABModifyLstCtxtSURes->noComp), numComp);

  for( indx=0; indx < numComp; indx++ )
  {
    eRabItemIe = (sztE_RABModifyLstCtxtSURes->member + indx);
    s1apWrapIeDefFields( eRabItemIe, Sztid_E_RABMdfyItemBrModRes, SztCriticalityignoreEnum );
    s1apWrapERabModifyItem( &(eRabItemIe->value.u.sztE_RABMdfyItemBrModRes), &(eRabModifyList->eRabModifyItem[indx]) );
  }
  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   sztE_RABReleaseLstSURes CCPU E-RAB Release List
 * @param   eRabReleaseList PWAV E-RAB Release List
 * @details
 * This functions allocates memory for sztE_RABReleaseLstSURes in the S1AP PDU,
 * Fills the sztE_RABReleaseLstSURes with the values in eRabReleaseList.
 */
TBOOL s1apWrapERabReleaseList(
    IO S1apPdu                   *pdu,
    IO SztE_RABRlsLstBrRelComp   *sztE_RABReleaseLstSURes,
    IN ERabReleaseList           *eRabReleaseList
    )
{
  UINT16 indx, numComp;
  SztProtIE_Field_E_RABRlsItemBrRelCompIEs *eRabItemIe;

  numComp = eRabReleaseList->noComp;

  /* Allocate memory for CSG items */
  if ((cmGetMem(pdu, numComp*sizeof(SztProtIE_Field_E_RABRlsItemBrRelCompIEs),
             (Ptr*)&(sztE_RABReleaseLstSURes->member))) !=ROK)
  {
    logError("cmGetMem failed for ERAB Release Item:%d", numComp*sizeof(SztProtIE_Field_E_RABRlsItemBrRelCompIEs) );
    return FALSE;
  }
  uehSzFillTknU16( &(sztE_RABReleaseLstSURes->noComp), numComp);

  for( indx=0; indx < numComp; indx++ )
  {
    eRabItemIe = (sztE_RABReleaseLstSURes->member + indx);
    s1apWrapIeDefFields( eRabItemIe, Sztid_E_RABRlsItemBrRelComp, SztCriticalityignoreEnum );
    s1apWrapERabReleaseItem( &(eRabItemIe->value.u.sztE_RABRlsItemBrRelComp), &(eRabReleaseList->eRabToReleaseItem[indx]) );
  }
  return TRUE;
}


/* ********************************* MESSAGE wrapping ***************************************** */
/**
 * @param   pdu S1AP PDU
 * @param   setupReq Setup Request
 * @details This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 *          values in setupReq.
 */
TBOOL s1apWrapSetupReqPdu
  (
    IO S1apPdu      **pdu,
    IN S1SetupReq    *setupReq
  )
{
   S1apPdu *szS1SetupReqPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_S1SetupRqstIEs *ie;

   logInfo("%s", "Wrap S1 Setup Request");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem,
         (Ptr *)&szS1SetupReqPdu);

   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for S1 Setup Request, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapInitMsgId( szS1SetupReqPdu, Sztid_S1Setup );

   /* Filling 3 mandatory IEs in Setup Request*/
   numComp = 3;
   if( TRUE == setupReq->isEnodeBNamePresent )
     numComp++;
   if( TRUE == setupReq->isCsgListPresent )
     numComp++;
   s1apWrapIeCount( szS1SetupReqPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(szS1SetupReqPdu, numComp * sizeof(SztProtIE_Field_S1SetupRqstIEs),\
               (Ptr*)&szS1SetupReqPdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in S1SetupRequest, Size:%d", numComp * sizeof(SztProtIE_Field_S1SetupRqstIEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling ENB Global Id */
   ie = &szS1SetupReqPdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.\
        protocolIEs.member[ieCount]; ieCount++;
   /* Global ENB Id */
   s1apWrapIeDefFields( ie, Sztid_Global_ENB_ID, SztCriticalityrejectEnum );
   if( FALSE == s1apWrapEnodeBId( szS1SetupReqPdu, &(ie->value.u.sztGlobal_ENB_ID), &(setupReq->globalEnodeBId) ) )
   {
     logError("%s", "Error in wrapping EndoeB ID");
     return FALSE;
   }

   /* IE2 - Filling ENB Name */
   if( TRUE == setupReq->isEnodeBNamePresent )
   {
      ie = &szS1SetupReqPdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.\
        protocolIEs.member[ieCount];ieCount++;
      s1apWrapIeDefFields( ie, Sztid_eNBname, SztCriticalityignoreEnum );
      if( FALSE == s1apWrapEnodeName( szS1SetupReqPdu, &(ie->value.u.sztENBname), setupReq->enbName ) )
      {
        logError("%s", "Error in wrapping EnodeB Name");
        return FALSE;
      }

   }

   /* IE3 - Filling Supported TAs */
   ie = &szS1SetupReqPdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_SuppTAs, SztCriticalityrejectEnum );
   if( FALSE == s1apWrapSupportedTacs( szS1SetupReqPdu, &(ie->value.u.sztSuppTAs) , &(setupReq->supportedTacs) ) )
   {
     logError("%s", "Error in wrapping TAC");
     return FALSE;
   }

   /* IE4 - Filling Supported CSGList */
   if( TRUE == setupReq->isCsgListPresent )
   {
     ie = &szS1SetupReqPdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.\
           protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CSG_IdLst, SztCriticalityrejectEnum );
     if( FALSE == s1apWrapCsgList( szS1SetupReqPdu, &(ie->value.u.sztCSG_IdLst), &(setupReq->csgIdList) ) )
     {
       logError("%s", "Error in wrapping CSG List");
       return FALSE;
     }
   }

  /* IE-5 Default Paging DRX */
  ie = &szS1SetupReqPdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.\
       protocolIEs.member[ieCount]; ieCount++;
  s1apWrapIeDefFields( ie, Sztid_DefaultPagDRX, SztCriticalityignoreEnum );
  s1apWrapDefaultPagingDrx( &(ie->value.u.sztPagDRX), setupReq->pagingDrx );

  *pdu = szS1SetupReqPdu;

   RETVALUE(ROK);
} /* s1apWrapSetupReqPdu */

/**
 * @param   pdu S1AP PDU
 * @param   resetReq Reset Request
 * @details This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 *          values in resetReq.
 */
TBOOL s1apWrapResetReqPdu
  (
    IO S1apPdu       **pdu,
    IN S1ResetReq    *resetReq
  )
{
  S1apPdu *szS1ResetReqPdu = NULLP;
  U16     numComp,ieCount=0;
  S16     ret;
  SztProtIE_Field_ResetIEs *ie;

  logInfo("%s", "Wrap S1 Reset Message");

  /* Initialize memory control point */
  ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem,
        (Ptr *)&szS1ResetReqPdu);
  if(ret != ROK)
  {
    logError("cmAllocEvnt failed for S1 Reset Request, Size:%d", sizeof(S1apPdu) );
    RETVALUE(RFAILED);
  }

  s1apWrapInitMsgId( szS1ResetReqPdu, Sztid_Reset );

  /* Filling 2 IEs, which are mandatory in Reset Request */
  numComp = 2;
  s1apWrapIeCount( szS1ResetReqPdu, numComp );
  /* Get memory for numComp IEs */
  if ((cmGetMem(szS1ResetReqPdu, numComp * sizeof(SztProtIE_Field_ResetIEs),\
              (Ptr*)&szS1ResetReqPdu->pdu.val.initiatingMsg.value.u.sztReset.\
              protocolIEs.member)) !=ROK)
  {
    logError("cmGetMem failed for IE's in S1ResetRequest, Size:%d", numComp * sizeof(SztProtIE_Field_ResetIEs) );
    RETVALUE(RFAILED);
  }

  /* IE1 - Filling Cause, If global reset we fill OAM intervention */
  ie = &szS1ResetReqPdu->pdu.val.initiatingMsg.value.u.sztReset.protocolIEs.member[ieCount]; ieCount++;
  s1apWrapIeDefFields( ie, Sztid_Cause, SztCriticalityignoreEnum );
  s1apWrapCause( &(ie->value.u.sztCause),&(resetReq->resetCause) );

  /* IE2 - Reset All */
  ie = &szS1ResetReqPdu->pdu.val.initiatingMsg.value.u.sztReset.protocolIEs.member[ieCount]; ieCount++;
  s1apWrapIeDefFields( ie, Sztid_ResetTyp, SztCriticalityrejectEnum );

  if( ResetType_GlobalS1Reset == resetReq->resetType )
  {
    uehSzFillTknU8(&(ie->value.u.sztResetTyp.choice ), RESETTYP_S1_INTF );
    uehSzFillTknU32(&(ie->value.u.sztResetTyp.val.s1_Intf), SztResetAllreset_allEnum );
  }
  else
  {
    uehSzFillTknU8(&(ie->value.u.sztResetTyp.choice ), RESETTYP_PARTOFS1_INTF );
    if( FALSE == s1apWrapRstUeAssocLogConnList( szS1ResetReqPdu, &(ie->value.u.sztResetTyp.val.partOfS1_Intf), &(resetReq->u.ueAssocLogConnList) ) )
    {
      logError("%s", "Error in wrapping UE Association connection List");
      return FALSE;
    }
  }

  *pdu = szS1ResetReqPdu;

  RETVALUE(ROK);

} /* s1apWrapResetReqPdu */

/**
 * @param   pdu S1AP PDU
 * @param   initialUeMsg Initial UE Message
 * @details This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 *          values in initialUeMsg.
 */
TBOOL s1apWrapInitialUeMsgPdu
  (
    IO S1apPdu            **pdu,
    IN InitialUeMessage   *initialUeMsg
  )
{
   S1apPdu *szInitialUeMsgPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_InitUEMsg_IEs *ie;

   logInfo("%s", "Wrap Initial UE Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem,
         (Ptr *)&szInitialUeMsgPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for Initial Ue Message, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapInitMsgId( szInitialUeMsgPdu, Sztid_initialUEMsg );

   /* Filling 5 mandatory IEs in Setup Request*/
   numComp = 5;
   if( TRUE == initialUeMsg->isSTimsi )
     numComp++;
   if( TRUE == initialUeMsg->isGummei )
     numComp++;
   if( TRUE == initialUeMsg->isCsgId )
     numComp++;

   s1apWrapIeCount( szInitialUeMsgPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(szInitialUeMsgPdu, numComp * sizeof(SztProtIE_Field_InitUEMsg_IEs),\
               (Ptr*)&szInitialUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in Initial Ue Message, Size:%d", numComp * sizeof(SztProtIE_Field_InitUEMsg_IEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling ENB UE S1AP Id */
   ie = &szInitialUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (initialUeMsg->enbUeS1apId) );

   /* IE2 - Filling NAS PDU */
   ie = &szInitialUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.\
        protocolIEs.member[ieCount];ieCount++;
   s1apWrapIeDefFields( ie, Sztid_NAS_PDU, SztCriticalityrejectEnum );
   if( FALSE == s1apWrapNasPdu( szInitialUeMsgPdu, &(ie->value.u.sztNAS_PDU), &(initialUeMsg->nasPdu) ) )
   {
     logError("%s", "Error in wrapping NAS PDU");
     return FALSE;
   }

   /* IE3 - Filling TAI */
   ie = &szInitialUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_TAI, SztCriticalityrejectEnum );
   if( FALSE == s1apWrapTai( szInitialUeMsgPdu, &(ie->value.u.sztTAI) , &(initialUeMsg->tai) ) )
   {
     logError("%s", "Error in wrapping TAI");
     return FALSE;
   }

   /* IE4 - E-UTRAN CGI */
   ie = &szInitialUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_EUTRAN_CGI, SztCriticalityignoreEnum );
   if( FALSE == s1apWrapEUtranCgi( szInitialUeMsgPdu, &(ie->value.u.sztEUTRAN_CGI) , &(initialUeMsg->eUtranCgi) ) )
   {
     logError("%s", "Error in wrapping E-UTRAN CGI");
     return FALSE;
   }

   /* IE-5 RRC Establishment Cause */
   ie = &szInitialUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_RRC_Establishment_Cause, SztCriticalityignoreEnum );
   s1apWrapRrcEstablishmentCause( &(ie->value.u.sztRRC_Establishment_Cause), initialUeMsg->rrcEstablishmentCause );

   /* Fill the optional Parameters */
   /* IE6 - Filling CSGID */
   if( TRUE == initialUeMsg->isCsgId )
   {
     ie = &szInitialUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.\
           protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CSG_Id, SztCriticalityrejectEnum );
     s1apWrapCsgId( &(ie->value.u.sztCSG_Id), &(initialUeMsg->csgId) );
   }

   /* IE7 - Filling S-TMSI */
   if( TRUE == initialUeMsg->isSTimsi )
   {
     ie = &szInitialUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.\
           protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_S_TMSI, SztCriticalityrejectEnum );
     s1apWrapSTmsi( szInitialUeMsgPdu, &(ie->value.u.sztS_TMSI), &(initialUeMsg->sTimsi) );
   }

   /* IE8 - Filling GUMMEI */
   if( TRUE == initialUeMsg->isGummei )
   {
     ie = &szInitialUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.\
           protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_GUMMEI_ID, SztCriticalityrejectEnum );
     s1apWrapGummei( szInitialUeMsgPdu, &(ie->value.u.sztGUMMEI), &(initialUeMsg->gummei) );
   }


  *pdu = szInitialUeMsgPdu;

   RETVALUE(ROK);
} /* s1apWrapInitialUeMsgPdu */

/**
 * @param   pdu S1AP PDU
 * @param   ulNasMsg Uplink NAS Message
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in ulNasMsg.
 */
TBOOL s1apWrapUlNasMsgPdu
  (
    IO S1apPdu            **pdu,
    IN UlNasMessage       *ulNasMsg
  )
{
   S1apPdu *ulNasMsgPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_UlnkNASTport_IEs *ie;

   logInfo("%s", "Wrap Uplink NAS Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&ulNasMsgPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for Uplink NAS Message, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapInitMsgId( ulNasMsgPdu, Sztid_uplinkNASTport );

   /* Filling 5 mandatory IEs in Setup Request*/
   numComp = 5;
   s1apWrapIeCount( ulNasMsgPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(ulNasMsgPdu, numComp * sizeof(SztProtIE_Field_UlnkNASTport_IEs),\
               (Ptr*)&ulNasMsgPdu->pdu.val.initiatingMsg.value.u.sztUlnkNASTport.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in Uplink NAS Message, Size:%d", numComp * sizeof(SztProtIE_Field_UlnkNASTport_IEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &ulNasMsgPdu->pdu.val.initiatingMsg.value.u.sztUlnkNASTport.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (ulNasMsg->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &ulNasMsgPdu->pdu.val.initiatingMsg.value.u.sztUlnkNASTport.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (ulNasMsg->enbUeS1apId) );

   /* IE3 - Filling NAS PDU */
   ie = &ulNasMsgPdu->pdu.val.initiatingMsg.value.u.sztUlnkNASTport.\
        protocolIEs.member[ieCount];ieCount++;
   s1apWrapIeDefFields( ie, Sztid_NAS_PDU, SztCriticalityrejectEnum );
   if( FALSE == s1apWrapNasPdu( ulNasMsgPdu, &(ie->value.u.sztNAS_PDU), &(ulNasMsg->nasPdu) ) )
   {
     logError("%s", "Error in wrapping NAS PDU");
     return FALSE;
   }

   /* IE4 - E-UTRAN CGI */
   ie = &ulNasMsgPdu->pdu.val.initiatingMsg.value.u.sztUlnkNASTport.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_EUTRAN_CGI, SztCriticalityignoreEnum );
   if( FALSE == s1apWrapEUtranCgi( ulNasMsgPdu, &(ie->value.u.sztEUTRAN_CGI) , &(ulNasMsg->eUtranCgi) ) )
   {
     logError("%s", "Error in wrapping E-UTRAN CGI");
     return FALSE;
   }

   /* IE5 - Filling TAI */
   ie = &ulNasMsgPdu->pdu.val.initiatingMsg.value.u.sztUlnkNASTport.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_TAI, SztCriticalityignoreEnum );
   if( FALSE == s1apWrapTai( ulNasMsgPdu, &(ie->value.u.sztTAI) , &(ulNasMsg->tai) ) )
   {
     logError("%s", "Error in wrapping TAI");
     return FALSE;
   }

   *pdu = ulNasMsgPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   initialCtxSetupFail Initial Context Setup Fail
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in initialCtxSetupFail.
 */
TBOOL s1apWrapInitialContextSetupFailPdu
  (
    IO S1apPdu                  **pdu,
    IN InitialContextSetupFail  *initialCtxSetupFail
  )
{
   S1apPdu *initCtxSetupFailPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_InitCntxtSetupFailIEs *ie;

   logInfo("%s", "Wrap Initial Context Setup Fail Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&initCtxSetupFailPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for InitialContextSetupFail Message, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapUnsuccMsgId( initCtxSetupFailPdu, Sztid_InitCntxtSetup );

   /* Filling 3 mandatory IEs in Setup Request*/
   numComp = 3;
   if( TRUE == initialCtxSetupFail->isCriticalDiagnostic )
     numComp++;

   s1apWrapIeCount( initCtxSetupFailPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(initCtxSetupFailPdu, numComp * sizeof(SztProtIE_Field_InitCntxtSetupFailIEs),\
               (Ptr*)&initCtxSetupFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztInitCntxtSetupFail.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in InitialContextSetupFail Message, Size:%d", numComp * sizeof(SztProtIE_Field_InitCntxtSetupFailIEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &initCtxSetupFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztInitCntxtSetupFail.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (initialCtxSetupFail->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &initCtxSetupFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztInitCntxtSetupFail.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (initialCtxSetupFail->enbUeS1apId) );

   /* IE3 - Filling Cause */
   ie = &initCtxSetupFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztInitCntxtSetupFail.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_Cause, SztCriticalityignoreEnum );
   s1apWrapCause( &(ie->value.u.sztCause),&(initialCtxSetupFail->cause) );

   /* IE4 - Critical Diagnostics */
   if( TRUE == initialCtxSetupFail->isCriticalDiagnostic )
   {
     ie = &initCtxSetupFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztInitCntxtSetupFail.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CriticalityDiag, SztCriticalityignoreEnum );
     if( FALSE == s1apWrapCriticalDiagnostic( initCtxSetupFailPdu, &(ie->value.u.sztCriticalityDiag),&(initialCtxSetupFail->criticalDiagnostic) ) )
     {
       logError("%s", "Error in wrapping Critical diagnostics");
       return FALSE;
     }

   }

   *pdu = initCtxSetupFailPdu;
   return TRUE;
}
/**
 * @param   pdu S1AP PDU
 * @param   s1apErrorIndiaction s1ap error indication
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in errInd.
 */
TBOOL s1apWrapS1apErrorIndPdu
  (
    IO S1apPdu     **pdu,
    IN  ErrInd     *errInd
  )
{
   S1apPdu *s1apErrorIndPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_ErrIndIEs *ie;

   logInfo("%s", "Wrap S1ap Error Indication Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&s1apErrorIndPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for S1ap Error Indication Message, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapInitMsgId( s1apErrorIndPdu, Sztid_ErrInd );

   /* Filling 3 mandatory IEs in Setup Request*/
   numComp = 3;
   if( TRUE == errInd->isCriticalDiagnostic )
     numComp++;

   s1apWrapIeCount( s1apErrorIndPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(s1apErrorIndPdu, numComp * sizeof(SztProtIE_Field_ErrIndIEs),\
               (Ptr*)&s1apErrorIndPdu->pdu.val.initiatingMsg.value.u.sztErrInd.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in InitialContextSetupFail Message, Size:%d", numComp * sizeof(SztProtIE_Field_ErrIndIEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &s1apErrorIndPdu->pdu.val.initiatingMsg.value.u.sztErrInd.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (errInd->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &s1apErrorIndPdu->pdu.val.initiatingMsg.value.u.sztErrInd.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (errInd->enbUeS1apId) );

   /* IE3 - Filling Cause */
   ie = &s1apErrorIndPdu->pdu.val.initiatingMsg.value.u.sztErrInd.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_Cause, SztCriticalityignoreEnum );
   s1apWrapCause( &(ie->value.u.sztCause),&(errInd->cause) );

   /* IE4 - Critical Diagnostics */
   if( TRUE == errInd->isCriticalDiagnostic )
   {
     ie = &s1apErrorIndPdu->pdu.val.initiatingMsg.value.u.sztErrInd.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CriticalityDiag, SztCriticalityignoreEnum );
     if( FALSE == s1apWrapCriticalDiagnostic( s1apErrorIndPdu, &(ie->value.u.sztCriticalityDiag),&(errInd->criticalDiagnostic) ) )
     {
       logError("%s", "Error in wrapping Critical diagnostics");
       return FALSE;
     }

   }

   *pdu = s1apErrorIndPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   initialCtxSetupRsp Initial Context Setup Response
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in initialCtxSetupRsp.
 */
TBOOL s1apWrapInitialContextSetupRspPdu
  (
    IO S1apPdu                  **pdu,
    IO InitialContextSetupRsp  *initialCtxSetupRsp
  )
{
   S1apPdu *initCtxSetupRspPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_InitCntxtSetupRespIEs *ie;

   logInfo("%s", "Wrap Initial Context Setup Response Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&initCtxSetupRspPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for InitialContextSetupResponse, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapSuccMsgId( initCtxSetupRspPdu, Sztid_InitCntxtSetup );

   /* Filling 3 mandatory IEs in Setup Request*/
   numComp = 3;
   if( TRUE == initialCtxSetupRsp->isERabList)
     numComp++;
   if( TRUE == initialCtxSetupRsp->isCriticalDiagnostic )
     numComp++;

   s1apWrapIeCount( initCtxSetupRspPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(initCtxSetupRspPdu, numComp * sizeof(SztProtIE_Field_InitCntxtSetupRespIEs),\
               (Ptr*)&initCtxSetupRspPdu->pdu.val.successfulOutcome.value.u.sztInitCntxtSetupResp.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in InitialContextSetupResponse, Size:%d", numComp * sizeof(SztProtIE_Field_InitCntxtSetupRespIEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &initCtxSetupRspPdu->pdu.val.successfulOutcome.value.u.sztInitCntxtSetupResp.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (initialCtxSetupRsp->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &initCtxSetupRspPdu->pdu.val.successfulOutcome.value.u.sztInitCntxtSetupResp.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (initialCtxSetupRsp->enbUeS1apId) );

   /* IE3 - SuccessRab List */
   ie = &initCtxSetupRspPdu->pdu.val.successfulOutcome.value.u.sztInitCntxtSetupResp.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_E_RABSetupLstCtxtSURes, SztCriticalityignoreEnum );
   s1apWrapERabSetupCtxtList( initCtxSetupRspPdu, &(ie->value.u.sztE_RABSetupLstCtxtSURes),&(initialCtxSetupRsp->eRabSetupList) );

   /* IE4 - FailedRab List */
   if( TRUE == initialCtxSetupRsp->isERabList )
   {
     ie = &initCtxSetupRspPdu->pdu.val.successfulOutcome.value.u.sztInitCntxtSetupResp.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_E_RABFailedToSetupLstCtxtSURes, SztCriticalityignoreEnum );
     s1apWrapERabList( initCtxSetupRspPdu, &(ie->value.u.sztE_RABLst),&(initialCtxSetupRsp->eRabList) );
   }

   /* IE5 - Critical Diagnostics */
   if( TRUE == initialCtxSetupRsp->isCriticalDiagnostic )
   {
     ie = &initCtxSetupRspPdu->pdu.val.successfulOutcome.value.u.sztInitCntxtSetupResp.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CriticalityDiag, SztCriticalityignoreEnum );
     if( FALSE == s1apWrapCriticalDiagnostic( initCtxSetupRspPdu, &(ie->value.u.sztCriticalityDiag),&(initialCtxSetupRsp->criticalDiagnostic) ) )
     {
       logError("%s", "Error in wrapping Critical diagnostics");
        return FALSE;
     }
   }

   *pdu = initCtxSetupRspPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   ueContextRelReq UE Context Release Request
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in ueContextRelReq.
 */
TBOOL s1apWrapUeContextRelReqPdu
  (
    IO S1apPdu          **pdu,
    IN UeContextRelReq  *ueContextRelReq
  )
{
   S1apPdu *ueCtxRelReqPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_UECntxtRlsRqst_IEs *ie;

   logInfo("%s", "Wrap UE context Release Request Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&ueCtxRelReqPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for UE ContextReleaseRequest, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapInitMsgId( ueCtxRelReqPdu, Sztid_UECntxtRlsRqst );

   /* Filling 3 mandatory IEs in Setup Request*/
   numComp = 3;

   s1apWrapIeCount( ueCtxRelReqPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(ueCtxRelReqPdu, numComp * sizeof(SztProtIE_Field_UECntxtRlsRqst_IEs),\
               (Ptr*)&ueCtxRelReqPdu->pdu.val.initiatingMsg.value.u.sztUECntxtRlsRqst.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in UE ContextReleaseRequest, Size:%d", numComp * sizeof(SztProtIE_Field_UECntxtRlsRqst_IEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &ueCtxRelReqPdu->pdu.val.initiatingMsg.value.u.sztUECntxtRlsRqst.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (ueContextRelReq->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &ueCtxRelReqPdu->pdu.val.initiatingMsg.value.u.sztUECntxtRlsRqst.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (ueContextRelReq->enbUeS1apId) );

   /* IE3 - Filling Cause */
   ie = &ueCtxRelReqPdu->pdu.val.initiatingMsg.value.u.sztUECntxtRlsRqst.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_Cause, SztCriticalityignoreEnum );
   s1apWrapCause( &(ie->value.u.sztCause),&(ueContextRelReq->cause) );

   *pdu = ueCtxRelReqPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   ueContextRelCmp UE Context Release complete
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in ueContextRelCmp.
 */
TBOOL s1apWrapUeContextRelCmpPdu
  (
    IO S1apPdu          **pdu,
    IN UeContextRelCmp  *ueContextRelCmp
  )
{
   S1apPdu *ueCtxRelCmpPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_UECntxtRlsComplete_IEs *ie;

   logInfo("%s", "Wrap UE context Release Complete Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&ueCtxRelCmpPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for UE ContextReleaseComplete, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapSuccMsgId( ueCtxRelCmpPdu, Sztid_UECntxtRls );

   /* Filling 3 mandatory IEs in Setup Request*/
   numComp = 2;
   if( TRUE == ueContextRelCmp->isCriticalDiagnostic )
     numComp++;

   s1apWrapIeCount( ueCtxRelCmpPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(ueCtxRelCmpPdu, numComp * sizeof(SztProtIE_Field_UECntxtRlsComplete_IEs),\
               (Ptr*)&ueCtxRelCmpPdu->pdu.val.successfulOutcome.value.u.sztUECntxtRlsComplete.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in UE ContextReleaseComplete, Size:%d", numComp * sizeof(SztProtIE_Field_UECntxtRlsComplete_IEs) );
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &ueCtxRelCmpPdu->pdu.val.successfulOutcome.value.u.sztUECntxtRlsComplete.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (ueContextRelCmp->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &ueCtxRelCmpPdu->pdu.val.successfulOutcome.value.u.sztUECntxtRlsComplete.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (ueContextRelCmp->enbUeS1apId) );

   /* IE3 - Filling Critical Diagnostics */
   if( TRUE == ueContextRelCmp->isCriticalDiagnostic )
   {
     ie = &ueCtxRelCmpPdu->pdu.val.successfulOutcome.value.u.sztUECntxtRlsComplete.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CriticalityDiag, SztCriticalityignoreEnum );
     if( FALSE == s1apWrapCriticalDiagnostic( ueCtxRelCmpPdu, &(ie->value.u.sztCriticalityDiag), &(ueContextRelCmp->criticalDiagnostic) ) )
     {
       logError("%s", "Error in wrapping Critical diagnostics");
       return FALSE;
     }
   }

   *pdu = ueCtxRelCmpPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   s1ResetAck Reset Ack
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in s1ResetAck.
 */
TBOOL s1apWrapS1ResetAckPdu
  (
    IO S1apPdu          **pdu,
    IN S1ResetAck       *s1ResetAck
  )
{
   S1apPdu *s1ResetAckPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_ResetAckgIEs *ie;

   logInfo("%s", "Wrap S1Reset Ack Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&s1ResetAckPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for S1ResetAck, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapSuccMsgId( s1ResetAckPdu, Sztid_Reset );

   /* Filling 3 mandatory IEs in Setup Request*/
   numComp = 0;
   if( TRUE == s1ResetAck->isCriticalDiagnostic )
     numComp++;
   if( TRUE == s1ResetAck->isUeAssociatedLogConnList )
     numComp++;

   s1apWrapIeCount( s1ResetAckPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(s1ResetAckPdu, numComp * sizeof(SztProtIE_Field_ResetAckgIEs),\
               (Ptr*)&s1ResetAckPdu->pdu.val.successfulOutcome.value.u.sztResetAckg.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in S1ResetAck, Size:%d", numComp * sizeof(SztProtIE_Field_ResetAckgIEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling Critical Diagnostics */
   if( TRUE == s1ResetAck->isUeAssociatedLogConnList )
   {
     ie = &s1ResetAckPdu->pdu.val.successfulOutcome.value.u.sztResetAckg.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_UE_assocLogS1_ConLstResAck, SztCriticalityignoreEnum );
     if( FALSE == s1apWrapRstAckUeAssocLogConnList( s1ResetAckPdu, &(ie->value.u.sztUE_assocLogS1_ConLstResAck), &(s1ResetAck->ueAssocLogConnList) ) )
     {
       logError("%s", "Error in wrapping UE Association connection List");
       return FALSE;
     }
   }

   /* IE2 - Filling Critical Diagnostics */
   if( TRUE == s1ResetAck->isCriticalDiagnostic )
   {
     ie = &s1ResetAckPdu->pdu.val.successfulOutcome.value.u.sztResetAckg.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CriticalityDiag, SztCriticalityignoreEnum );
     if( FALSE == s1apWrapCriticalDiagnostic( s1ResetAckPdu, &(ie->value.u.sztCriticalityDiag), &(s1ResetAck->criticalDiagnostic) ) )
     {
       logError("%s", "Error in wrapping Critical diagnostics");
       return FALSE;
     }
   }

   *pdu = s1ResetAckPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   nasNonDeliveryMsg NAS Non-Delivery indication.
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in nasNonDeliveryMsg. This function is invoked when there is a failure in sending the NAS message to UE.
 */
TBOOL s1apWrapNasNonDeliveryIndPdu
  (
    IO S1apPdu            **pdu,
    IN NasNonDeliveryInd  *nasNonDeliveryMsg
  )
{
   S1apPdu *nasNonDeliveryIndPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_NASNonDlvryInd_IEs *ie;

   logInfo("%s", "Wrap NAS Non Delivery Ind Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&nasNonDeliveryIndPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for NAS NonDeliveryInd, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapInitMsgId( nasNonDeliveryIndPdu, Sztid_NASNonDlvryInd );

   /* Filling 4 mandatory IEs in Setup Request*/
   numComp = 4;
   s1apWrapIeCount( nasNonDeliveryIndPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(nasNonDeliveryIndPdu, numComp * sizeof(SztProtIE_Field_NASNonDlvryInd_IEs),\
               (Ptr*)&nasNonDeliveryIndPdu->pdu.val.initiatingMsg.value.u.sztNASNonDlvryInd.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in NAS NonDeliveryInd, Size:%d", numComp * sizeof(SztProtIE_Field_NASNonDlvryInd_IEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &nasNonDeliveryIndPdu->pdu.val.initiatingMsg.value.u.sztNASNonDlvryInd.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (nasNonDeliveryMsg->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &nasNonDeliveryIndPdu->pdu.val.initiatingMsg.value.u.sztNASNonDlvryInd.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (nasNonDeliveryMsg->enbUeS1apId) );

   /* IE3 - Filling NAS PDU */
   ie = &nasNonDeliveryIndPdu->pdu.val.initiatingMsg.value.u.sztNASNonDlvryInd.\
        protocolIEs.member[ieCount];ieCount++;
   s1apWrapIeDefFields( ie, Sztid_NAS_PDU, SztCriticalityignoreEnum );
   if( FALSE == s1apWrapNasPdu( nasNonDeliveryIndPdu, &(ie->value.u.sztNAS_PDU), &(nasNonDeliveryMsg->nasPdu) ) )
   {
     logError("%s", "Error in wrapping NAS PDU");
     return FALSE;
   }

   /* IE4 - Cause */
   ie = &nasNonDeliveryIndPdu->pdu.val.initiatingMsg.value.u.sztNASNonDlvryInd.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_Cause, SztCriticalityignoreEnum );
   s1apWrapCause( &(ie->value.u.sztCause),&(nasNonDeliveryMsg->cause) );

   *pdu = nasNonDeliveryIndPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   ueCapInfoIndMsg UE Capability Info Indication
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in ueCapInfoIndMsg.
 */
TBOOL s1apWrapUeCapInfoIndPdu
  (
    IO S1apPdu            **pdu,
    IO UeCapInfoInd        *ueCapInfoIndMsg
  )
{
   S1apPdu *ueCapInfoIndPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_UECapbltyInfoIndIEs *ie;

   logInfo("%s", "Wrap UE Capability Info Ind Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&ueCapInfoIndPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for UE CapabilityInfoInd, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapInitMsgId( ueCapInfoIndPdu, Sztid_UECapbltyInfoInd );

   /* Filling 3 mandatory IEs in Setup Request*/
   numComp = 3;
   s1apWrapIeCount( ueCapInfoIndPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(ueCapInfoIndPdu, numComp * sizeof(SztProtIE_Field_UECapbltyInfoIndIEs),\
               (Ptr*)&ueCapInfoIndPdu->pdu.val.initiatingMsg.value.u.sztUECapbltyInfoInd.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in UE CapabilityInfoInd, Size:%d", numComp * sizeof(SztProtIE_Field_UECapbltyInfoIndIEs));
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &ueCapInfoIndPdu->pdu.val.initiatingMsg.value.u.sztUECapbltyInfoInd.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (ueCapInfoIndMsg->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &ueCapInfoIndPdu->pdu.val.initiatingMsg.value.u.sztUECapbltyInfoInd.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (ueCapInfoIndMsg->enbUeS1apId) );

   /* IE3 - Filling UE Radio Capability PDU */
   ie = &ueCapInfoIndPdu->pdu.val.initiatingMsg.value.u.sztUECapbltyInfoInd.\
        protocolIEs.member[ieCount];ieCount++;
   s1apWrapIeDefFields( ie, Sztid_UERadioCapblty, SztCriticalityignoreEnum );
   if( FALSE == s1apWrapUeRadioCapInfoPdu( ueCapInfoIndPdu, &(ie->value.u.sztUERadioCapblty), &(ueCapInfoIndMsg->ueRadioCapInfoPdu) ) )
   {
     logError("%s", "Error in wrapping UE Radio Capability PDU");
     return FALSE;
   }

   *pdu = ueCapInfoIndPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   mmeCfgUpdateAck MME Configuration Update Ack
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in mmeCfgUpdateAck.
 */
TBOOL s1apWrapMmeCfgUpdateAckPdu
  (
    IO S1apPdu          **pdu,
    IN MmeCfgUpdateAck  *mmeCfgUpdateAck
  )
{
   S1apPdu *mmeCfgUpdateAckPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_MMEConfigUpdAckgIEs *ie;

   logInfo("%s", "Wrap MmeCfgUpdateAck Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&mmeCfgUpdateAckPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for MmeCfgUpdateAck, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapSuccMsgId( mmeCfgUpdateAckPdu, Sztid_MMEConfigUpd );

   /* Filling 1 Optional IEs in MmeCfgUpdateAck */
   numComp = 0;
   if( TRUE == mmeCfgUpdateAck->isCriticalDiagnostic )
     numComp++;

   s1apWrapIeCount( mmeCfgUpdateAckPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(mmeCfgUpdateAckPdu, numComp * sizeof(SztProtIE_Field_MMEConfigUpdAckgIEs),\
               (Ptr*)&mmeCfgUpdateAckPdu->pdu.val.successfulOutcome.value.u.sztMMEConfigUpdAckg.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in MmeCfgUpdateAck, Size:%d", numComp * sizeof(SztProtIE_Field_MMEConfigUpdAckgIEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling Critical Diagnostics */
   if( TRUE == mmeCfgUpdateAck->isCriticalDiagnostic )
   {
     ie = &mmeCfgUpdateAckPdu->pdu.val.successfulOutcome.value.u.sztMMEConfigUpdAckg.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CriticalityDiag, SztCriticalityignoreEnum );
     if( FALSE == s1apWrapCriticalDiagnostic( mmeCfgUpdateAckPdu, &(ie->value.u.sztCriticalityDiag), &(mmeCfgUpdateAck->criticalDiagnostic) ) )
     {
       logError("%s", "Error in wrapping Critical Diagnostics");
       return FALSE;
     }
   }

   *pdu = mmeCfgUpdateAckPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   mmeCfgUpdateFail MME Configuration Update Fail
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in mmeCfgUpdateFail.
 */
TBOOL s1apWrapMmeCfgUpdateFailPdu
  (
    IO S1apPdu          **pdu,
    IN MmeCfgUpdateFail *mmeCfgUpdateFail
  )
{
   S1apPdu *mmeCfgUpdateFailPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_MMEConfigUpdFailIEs *ie;

   logInfo("%s", "Wrap MmeCfgUpdateFail Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&mmeCfgUpdateFailPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for MmeCfgUpdateFail, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapUnsuccMsgId( mmeCfgUpdateFailPdu, Sztid_MMEConfigUpd );

   /* Filling 1 Mandatory IEs in MmeCfgUpdateFail */
   numComp = 1;
   if( TRUE == mmeCfgUpdateFail->isTimeToWait )
     numComp++;
   if( TRUE == mmeCfgUpdateFail->isCriticalDiagnostic )
     numComp++;

   s1apWrapIeCount( mmeCfgUpdateFailPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(mmeCfgUpdateFailPdu, numComp * sizeof(SztProtIE_Field_MMEConfigUpdAckgIEs),\
               (Ptr*)&mmeCfgUpdateFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztMMEConfigUpdFail.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in MmeCfgUpdateFail, Size:%d", numComp * sizeof(SztProtIE_Field_MMEConfigUpdAckgIEs) );
     RETVALUE(RFAILED);
   }
   /* IE1 - Cause */
   ie = &mmeCfgUpdateFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztMMEConfigUpdFail.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_Cause, SztCriticalityignoreEnum );
   s1apWrapCause( &(ie->value.u.sztCause),&(mmeCfgUpdateFail->failCause) );

   /* IE2 - Filling TimetoWait */
   if( TRUE == mmeCfgUpdateFail->isTimeToWait )
   {
     ie = &mmeCfgUpdateFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztMMEConfigUpdFail.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_TimeToWait, SztCriticalityignoreEnum );
     s1apWrapTimeToWait( &(ie->value.u.sztTimeToWait), mmeCfgUpdateFail->timetoWait );
   }

   /* IE3 - Filling Critical Diagnostics */
   if( TRUE == mmeCfgUpdateFail->isCriticalDiagnostic )
   {
     ie = &mmeCfgUpdateFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztMMEConfigUpdFail.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CriticalityDiag, SztCriticalityignoreEnum );
     if( FALSE == s1apWrapCriticalDiagnostic( mmeCfgUpdateFailPdu, &(ie->value.u.sztCriticalityDiag), &(mmeCfgUpdateFail->criticalDiagnostic) ) )
     {
       logError("%s", "Error in wrapping Critical Diagnostics");
       return FALSE;
     }
   }

   *pdu = mmeCfgUpdateFailPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   locReportFailIndMsg Location Report Fail
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in locReportFailIndMsg.
 */
TBOOL s1apWrapLocReportFailIndPdu
  (
    IO S1apPdu            **pdu,
    IN LocReqReportFail   *locReportFailIndMsg
  )
{
   S1apPdu *locReportFailIndPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_LocRprtngFailIndIEs *ie;

   logInfo("%s", "Wrap Location Report Fail Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&locReportFailIndPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for LocationReportFail, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapInitMsgId( locReportFailIndPdu, Sztid_LocRprtngFailInd );

   /* Filling 3 mandatory IEs in Location Report Fail Request*/
   numComp = 3;
   s1apWrapIeCount( locReportFailIndPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(locReportFailIndPdu, numComp * sizeof(SztProtIE_Field_LocRprtngFailIndIEs),\
               (Ptr*)&locReportFailIndPdu->pdu.val.initiatingMsg.value.u.sztLocRprtngFailInd.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in LocationReportFail, Size:%d", numComp * sizeof(SztProtIE_Field_LocRprtngFailIndIEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &locReportFailIndPdu->pdu.val.initiatingMsg.value.u.sztLocRprtngFailInd.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (locReportFailIndMsg->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &locReportFailIndPdu->pdu.val.initiatingMsg.value.u.sztLocRprtngFailInd.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (locReportFailIndMsg->enbUeS1apId) );

   /* IE3 - Cause */
   ie = &locReportFailIndPdu->pdu.val.initiatingMsg.value.u.sztLocRprtngFailInd.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_Cause, SztCriticalityignoreEnum );
   s1apWrapCause( &(ie->value.u.sztCause),&(locReportFailIndMsg->cause) );

   *pdu = locReportFailIndPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   locReportMsg Location Report
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in locReportMsg.
 */
TBOOL s1apWrapLocReportPdu
  (
    IO S1apPdu            **pdu,
    IN LocReqReport       *locReportMsg
  )
{
   S1apPdu *locReportPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_LocReportIEs *ie;

   logInfo("%s", "Wrap Location Report Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&locReportPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for LocationReport, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapInitMsgId( locReportPdu, Sztid_LocReport );

   /* Filling 5 mandatory IEs in Location Report Request*/
   numComp = 5;
   s1apWrapIeCount( locReportPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(locReportPdu, numComp * sizeof(SztProtIE_Field_LocReportIEs),\
               (Ptr*)&locReportPdu->pdu.val.initiatingMsg.value.u.sztLocReport.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in LocationReport, Size:%d", numComp * sizeof(SztProtIE_Field_LocReportIEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &locReportPdu->pdu.val.initiatingMsg.value.u.sztLocReport.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (locReportMsg->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &locReportPdu->pdu.val.initiatingMsg.value.u.sztLocReport.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (locReportMsg->enbUeS1apId) );

   /* IE3 - E-UTRAN CGI */
   ie = &locReportPdu->pdu.val.initiatingMsg.value.u.sztLocReport.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_EUTRAN_CGI, SztCriticalityignoreEnum );
   if( FALSE == s1apWrapEUtranCgi( locReportPdu, &(ie->value.u.sztEUTRAN_CGI) , &(locReportMsg->eUtranCgi) ) )
   {
     logError("%s", "Error in wrapping E-UTRAN CGI");
     return FALSE;
   }

   /* IE4 - Filling TAI */
   ie = &locReportPdu->pdu.val.initiatingMsg.value.u.sztLocReport.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_TAI, SztCriticalityignoreEnum );
   if( FALSE == s1apWrapTai( locReportPdu, &(ie->value.u.sztTAI) , &(locReportMsg->tai) ) )
   {
     logError("%s", "Error in wrapping TAI");
     return FALSE;
   }

   /* IE5 - Filling Location Report Type */
   ie = &locReportPdu->pdu.val.initiatingMsg.value.u.sztLocReport.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_RqstTyp, SztCriticalityignoreEnum );
   s1apWrapLocReportCntrlReqType( &(ie->value.u.sztRqstTyp) , &(locReportMsg->locReportCntrlReqType) );

   *pdu = locReportPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   traceFailInd Trace Fail Indication
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in traceFailInd.
 */
TBOOL s1apWrapTraceFailIndPdu
  (
    IO S1apPdu            **pdu,
    IO TraceFailInd       *traceFailInd
  )
{
   S1apPdu *traceFailIndPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_TraceFailIndIEs *ie;

   logInfo("%s", "Wrap Trace Fail Ind Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&traceFailIndPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for TraceFailInd, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapInitMsgId( traceFailIndPdu, Sztid_TraceFailInd );

   /* Filling 4 mandatory IEs in Location Report Request*/
   numComp = 4;
   s1apWrapIeCount( traceFailIndPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(traceFailIndPdu, numComp * sizeof(SztProtIE_Field_TraceFailIndIEs),\
               (Ptr*)&traceFailIndPdu->pdu.val.initiatingMsg.value.u.sztTraceFailInd.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in TraceFailInd, Size:%d", numComp * sizeof(SztProtIE_Field_TraceFailIndIEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &traceFailIndPdu->pdu.val.initiatingMsg.value.u.sztTraceFailInd.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (traceFailInd->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &traceFailIndPdu->pdu.val.initiatingMsg.value.u.sztTraceFailInd.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (traceFailInd->enbUeS1apId) );

   /* IE3 - E-UTRAN Trace ID */
   ie = &traceFailIndPdu->pdu.val.initiatingMsg.value.u.sztTraceFailInd.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_E_UTRAN_Trace_ID, SztCriticalityignoreEnum );
   if( FALSE == s1apWrapEutranTraceId( traceFailIndPdu, &(ie->value.u.sztE_UTRAN_Trace_ID), &(traceFailInd->eUtranTraceId) ) )
   {
     logError("%s", "Error in wrappingEUtran Trace Id");
     return FALSE;
   }

   /* IE4 - Cause */
   ie = &traceFailIndPdu->pdu.val.initiatingMsg.value.u.sztTraceFailInd.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_Cause, SztCriticalityignoreEnum );
   s1apWrapCause( &(ie->value.u.sztCause),&(traceFailInd->cause) );

   *pdu = traceFailIndPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   eRabSetupRsp E-RAB Setup Response
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in eRabSetupRsp.
 */
TBOOL s1apWrapERabSetupRspPdu
  (
    IO S1apPdu                  **pdu,
    IO ERabSetupRsp             *eRabSetupRsp
  )
{
   S1apPdu              *eRabSetupRspPdu = NULLP;
   U16                  numComp, ieCount=0;
   S16                  ret;
   SztProtIE_Field_E_RABSetupRespIEs *ie;

   logInfo("%s", "Wrap ERab Setup Response Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&eRabSetupRspPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for ERabSetupResponse, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapSuccMsgId( eRabSetupRspPdu, Sztid_E_RABSetup );

   /* Filling 2 mandatory IEs in Setup Request*/
   numComp = 2;
   if( TRUE == eRabSetupRsp->isERabSetupList)
     numComp++;
   if( TRUE == eRabSetupRsp->isERabFailedSetupList)
     numComp++;
   if( TRUE == eRabSetupRsp->isCriticalDiagnostic )
     numComp++;

   s1apWrapIeCount( eRabSetupRspPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(eRabSetupRspPdu, numComp * sizeof(SztProtIE_Field_E_RABSetupRespIEs),\
               (Ptr*)&eRabSetupRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABSetupResp.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in ERabSetupResponse, Size:%d", numComp * sizeof(SztProtIE_Field_E_RABSetupRespIEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &eRabSetupRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABSetupResp.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (eRabSetupRsp->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &eRabSetupRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABSetupResp.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (eRabSetupRsp->enbUeS1apId) );

   /* IE3 - SuccessRab List */
   if( TRUE == eRabSetupRsp->isERabSetupList )
   {
     ie = &eRabSetupRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABSetupResp.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_E_RABSetupLstBrSURes, SztCriticalityignoreEnum );
     s1apWrapERabSetupList( eRabSetupRspPdu, &(ie->value.u.sztE_RABSetupLstBrSURes),&(eRabSetupRsp->eRabSetupList) );
   }

   /* IE4 - FailedRab List */
   if( TRUE == eRabSetupRsp->isERabFailedSetupList )
   {
     ie = &eRabSetupRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABSetupResp.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_E_RABFailedToSetupLstBrSURes, SztCriticalityignoreEnum );
     s1apWrapERabList( eRabSetupRspPdu, &(ie->value.u.sztE_RABLst),&(eRabSetupRsp->eRabFailedSetupList) );
   }

   /* IE5 - Critical Diagnostics */
   if( TRUE == eRabSetupRsp->isCriticalDiagnostic )
   {
     ie = &eRabSetupRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABSetupResp.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CriticalityDiag, SztCriticalityignoreEnum );
     if( FALSE == s1apWrapCriticalDiagnostic( eRabSetupRspPdu, &(ie->value.u.sztCriticalityDiag),&(eRabSetupRsp->criticalDiagnostic) ) )
     {
       logError("%s", "Error in wrapping Critical diagnostics");
       return FALSE;
     }
   }

   *pdu = eRabSetupRspPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   eRabModifyRsp E-RAB Modify Response
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in eRabModifyRsp.
 */
TBOOL s1apWrapERabModifyRspPdu
  (
    IO S1apPdu                  **pdu,
    IN ERabModifyRsp            *eRabModifyRsp
  )
{
   S1apPdu              *eRabModifyRspPdu = NULLP;
   U16                  numComp, ieCount=0;
   S16                  ret;
   SztProtIE_Field_E_RABMdfyRespIEs *ie;

   logInfo("%s", "Wrap ERab Modify Response Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&eRabModifyRspPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for ERabModifyResponse, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapSuccMsgId( eRabModifyRspPdu, Sztid_E_RABMdfy );

   /* Filling 2 mandatory IEs in Modify Response*/
   numComp = 2;
   if( TRUE == eRabModifyRsp->isERabFailedModifyList)
     numComp++;
   if( TRUE == eRabModifyRsp->isERabModifyList)
     numComp++;
   if( TRUE == eRabModifyRsp->isCriticalDiagnostic )
     numComp++;

   s1apWrapIeCount( eRabModifyRspPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(eRabModifyRspPdu, numComp * sizeof(SztProtIE_Field_E_RABMdfyRespIEs),\
               (Ptr*)&eRabModifyRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABMdfyResp.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in ERabModifyResponse, Size:%d", numComp * sizeof(SztProtIE_Field_E_RABMdfyRespIEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &eRabModifyRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABMdfyResp.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (eRabModifyRsp->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &eRabModifyRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABMdfyResp.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (eRabModifyRsp->enbUeS1apId) );

   /* IE3 - SuccessRab List */
   if( TRUE == eRabModifyRsp->isERabModifyList )
   {
     ie = &eRabModifyRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABMdfyResp.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_E_RABMdfyLstBrModRes, SztCriticalityignoreEnum );
     s1apWrapERabModifyList( eRabModifyRspPdu, &(ie->value.u.sztE_RABMdfyLstBrModRes),&(eRabModifyRsp->eRabModifyList) );
   }

   /* IE4 - FailedRab List */
   if( TRUE == eRabModifyRsp->isERabFailedModifyList )
   {
     ie = &eRabModifyRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABMdfyResp.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_E_RABFailedToMdfyLst, SztCriticalityignoreEnum );
     s1apWrapERabList( eRabModifyRspPdu, &(ie->value.u.sztE_RABLst),&(eRabModifyRsp->eRabFailedModifyList) );
   }

   /* IE5 - Critical Diagnostics */
   if( TRUE == eRabModifyRsp->isCriticalDiagnostic )
   {
     ie = &eRabModifyRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABMdfyResp.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CriticalityDiag, SztCriticalityignoreEnum );
     if( FALSE == s1apWrapCriticalDiagnostic( eRabModifyRspPdu, &(ie->value.u.sztCriticalityDiag),&(eRabModifyRsp->criticalDiagnostic) ) )
     {
       logError("%s", "Error in wrapping Critical diagnostics");
       return FALSE;
     }
   }

   *pdu = eRabModifyRspPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   eRabReleaseRsp E-RAB Release Response
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in eRabReleaseRsp.
 */
TBOOL s1apWrapERabReleaseRspPdu
  (
    IO S1apPdu                  **pdu,
    IN ERabReleaseRsp           *eRabReleaseRsp
  )
{
   S1apPdu              *eRabReleaseRspPdu = NULLP;
   U16                  numComp, ieCount=0;
   S16                  ret;
   SztProtIE_Field_E_RABRlsRespIEs *ie;

   logInfo("%s", "Wrap ERab Release Response Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&eRabReleaseRspPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for ERabReleaseResponse, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapSuccMsgId( eRabReleaseRspPdu, Sztid_E_RABRls );

   /* Filling 2 mandatory IEs in Modify Response*/
   numComp = 2;
   if( TRUE == eRabReleaseRsp->isERabFailedReleaseList)
     numComp++;
   if( TRUE == eRabReleaseRsp->isERabReleaseList)
     numComp++;
   if( TRUE == eRabReleaseRsp->isCriticalDiagnostic )
     numComp++;

   s1apWrapIeCount( eRabReleaseRspPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(eRabReleaseRspPdu, numComp * sizeof(SztProtIE_Field_E_RABRlsRespIEs),\
               (Ptr*)&eRabReleaseRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABRlsResp.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in ERabReleaseResponse, Size:%d", numComp * sizeof(SztProtIE_Field_E_RABRlsRespIEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &eRabReleaseRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABRlsResp.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (eRabReleaseRsp->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &eRabReleaseRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABRlsResp.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (eRabReleaseRsp->enbUeS1apId) );

   /* IE3 - SuccessRab List */
   if( TRUE == eRabReleaseRsp->isERabReleaseList )
   {
     ie = &eRabReleaseRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABRlsResp.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_E_RABRlsLstBrRelComp, SztCriticalityignoreEnum );
     s1apWrapERabReleaseList( eRabReleaseRspPdu, &(ie->value.u.sztE_RABRlsLstBrRelComp),&(eRabReleaseRsp->eRabReleaseList) );
   }

   /* IE4 - FailedRab List */
   if( TRUE == eRabReleaseRsp->isERabFailedReleaseList )
   {
     ie = &eRabReleaseRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABRlsResp.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_E_RABFailedToRlsLst, SztCriticalityignoreEnum );
     s1apWrapERabList( eRabReleaseRspPdu, &(ie->value.u.sztE_RABLst),&(eRabReleaseRsp->eRabFailedReleaseList) );
   }

   /* IE5 - Critical Diagnostics */
   if( TRUE == eRabReleaseRsp->isCriticalDiagnostic )
   {
     ie = &eRabReleaseRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABRlsResp.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CriticalityDiag, SztCriticalityignoreEnum );
     if( FALSE == s1apWrapCriticalDiagnostic( eRabReleaseRspPdu, &(ie->value.u.sztCriticalityDiag),&(eRabReleaseRsp->criticalDiagnostic) ) )
     {
       logError("%s", "Error in wrapping Critical diagnostics");
       return FALSE;
     }
   }

   *pdu = eRabReleaseRspPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   ueContextModFail UE Context Modification Fail
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in ueContextModFail.
 */
TBOOL s1apWrapUecontextModFailPdu
  (
    IO S1apPdu                  **pdu,
    IN UeContextModFail         *ueContextModFail
  )
{
   S1apPdu *ueContextModFailPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_UECntxtModificationFailIEs *ie;

   logInfo("%s", "Wrap UE Context Modification Fail Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&ueContextModFailPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for UE ContextModificationFail, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapUnsuccMsgId( ueContextModFailPdu, Sztid_UECntxtModification );

   /* Filling 3 mandatory IEs in UE Context Modification Request*/
   numComp = 3;
   if( TRUE == ueContextModFail->isCriticalDiagnostic )
     numComp++;

   s1apWrapIeCount( ueContextModFailPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(ueContextModFailPdu, numComp * sizeof(SztProtIE_Field_UECntxtModificationFailIEs),\
               (Ptr*)&ueContextModFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztUECntxtModificationFail.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in UE ContextModificationFail, Size:%d", numComp * sizeof(SztProtIE_Field_UECntxtModificationFailIEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &ueContextModFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztUECntxtModificationFail.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (ueContextModFail->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &ueContextModFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztUECntxtModificationFail.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (ueContextModFail->enbUeS1apId) );

   /* IE3 - Filling Cause */
   ie = &ueContextModFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztUECntxtModificationFail.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_Cause, SztCriticalityignoreEnum );
   s1apWrapCause( &(ie->value.u.sztCause),&(ueContextModFail->cause) );

   /* IE4 - Critical Diagnostics */
   if( TRUE == ueContextModFail->isCriticalDiagnostic )
   {
     ie = &ueContextModFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztUECntxtModificationFail.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CriticalityDiag, SztCriticalityignoreEnum );
     if( FALSE == s1apWrapCriticalDiagnostic( ueContextModFailPdu, &(ie->value.u.sztCriticalityDiag),&(ueContextModFail->criticalDiagnostic) ) )
     {
       logError("%s", "Error in wrapping Critical diagnostics");
       return FALSE;
     }

   }
   *pdu = ueContextModFailPdu;
   return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   ueContextModRsp UE Context Modification Response
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in ueContextModRsp.
 */
TBOOL s1apWrapUeContextModRspPdu
  (
    IO S1apPdu                  **pdu,
    IN UeContextModRsp          *ueContextModRsp
  )
{
   S1apPdu *ueContextModRspPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_UECntxtModificationRespIEs *ie;

   logInfo("%s", "Wrap UE Context Modification Response Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&ueContextModRspPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for UE ContextModificationResponse, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapSuccMsgId( ueContextModRspPdu, Sztid_UECntxtModification );

   /* Filling 2 mandatory IEs in Response*/
   numComp = 2;
   if( TRUE == ueContextModRsp->isCriticalDiagnostic )
     numComp++;

   s1apWrapIeCount( ueContextModRspPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(ueContextModRspPdu, numComp * sizeof(SztProtIE_Field_UECntxtModificationRespIEs),\
               (Ptr*)&ueContextModRspPdu->pdu.val.successfulOutcome.value.u.sztUECntxtModificationResp.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in UE ContextModificationResponse, Size:%d", numComp * sizeof(SztProtIE_Field_UECntxtModificationRespIEs) );
     RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME UE S1AP Id */
   ie = &ueContextModRspPdu->pdu.val.successfulOutcome.value.u.sztUECntxtModificationResp.\
       protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (ueContextModRsp->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id */
   ie = &ueContextModRspPdu->pdu.val.successfulOutcome.value.u.sztUECntxtModificationResp.\
        protocolIEs.member[ieCount]; ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityignoreEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (ueContextModRsp->enbUeS1apId) );

   /* IE3 - Critical Diagnostics */
   if( TRUE == ueContextModRsp->isCriticalDiagnostic )
   {
     ie = &ueContextModRspPdu->pdu.val.successfulOutcome.value.u.sztUECntxtModificationResp.\
         protocolIEs.member[ieCount]; ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CriticalityDiag, SztCriticalityignoreEnum );
     if( FALSE == s1apWrapCriticalDiagnostic( ueContextModRspPdu, &(ie->value.u.sztCriticalityDiag),&(ueContextModRsp->criticalDiagnostic) ) )
     {
       logError("%s", "Error in wrapping Critical diagnostics");
       return FALSE;
     }
   }

   *pdu = ueContextModRspPdu;
   return TRUE;
}

/**
 * @param   sztMsgIdPdu
 * @param   warnMsgId
 * @details
 * This functions fills the sztMsgIdPdu with the values in warnMsgId.
 */
static TBOOL s1apWrapMsgIdentifier(
    IO SztMsgIdentifier *sztMsgIdPdu,
    IN UINT8             *warnMsgId
    )
{
  sztMsgIdPdu->pres = TRUE;
  sztMsgIdPdu->len  = SIZE_WARNMSGID;

  memcpy( (sztMsgIdPdu->val), warnMsgId, MAX_SIZE_WARNMSGID);

  return TRUE;
}

/**
 * @param   sztSerialNumPdu
 * @param   serialNum
 * @details
 * This functions fills the sztSerialNumPdu with the values in serialNum.
 */
static TBOOL s1apWrapSerialNumber(
    IO SztSerialNumber   *sztSerialNumPdu,
    IN UINT8             *serialNum
    )
{
  sztSerialNumPdu->pres = TRUE;
  sztSerialNumPdu->len  = SIZE_SERIALNUM;

  memcpy( (sztSerialNumPdu->val), serialNum, MAX_SIZE_SERIALNUM);

  return TRUE;
}

/**
 * @param   pdu S1AP PDU
 * @param   writeReplcWarnRsp MME WriteReplace Warning Response message
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in writeReplcWarnRsp.
 */
TBOOL s1apWrapMmeWriteRplcWarnRsp
  (
    IO S1apPdu             **pdu,
    IN WriteReplcWarnRsp   *writeReplcWarnRsp
  )
{
   S1apPdu *writeReplcWarnRspPdu = NULLP;
   U16     numComp, ieCount=0;
   S16     ret;
   SztProtIE_Field_WriteReplaceWarningRespIEs *ie;

   logInfo("%s", "Wrap Write-Replace Warning Response Message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&writeReplcWarnRspPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for writeReplcWarnRsp, Size:%d", sizeof(S1apPdu) );
     RETVALUE(RFAILED);
   }

   /* Fill the message Id */
   s1apWrapSuccMsgId( writeReplcWarnRspPdu, Sztid_WriteReplaceWarning );

   /* Filling 2 Mandatory IEs in writeReplcWarnRsp */
   numComp = 2;
   if( TRUE == writeReplcWarnRsp->isBroadcastCmpltAreaListPresent )
     numComp++;
   if( TRUE == writeReplcWarnRsp->isCriticalDiagnosticPresent )
     numComp++;

   s1apWrapIeCount( writeReplcWarnRspPdu, numComp );
   /* Get memory for numComp IEs */
   if ((cmGetMem(writeReplcWarnRspPdu, numComp * sizeof(SztProtIE_Field_WriteReplaceWarningRespIEs),\
               (Ptr*)&writeReplcWarnRspPdu->pdu.val.successfulOutcome.value.u.sztWriteReplaceWarningResp.\
               protocolIEs.member)) !=ROK)
   {
     logError("cmGetMem failed for IE's in writeReplcWarnRsp, Size:%d", numComp * sizeof(SztProtIE_Field_WriteReplaceWarningRespIEs) );
     RETVALUE(RFAILED);
   }
   /* IE1 - MsgIdentifier */
   ie = &writeReplcWarnRspPdu->pdu.val.successfulOutcome.value.u.sztWriteReplaceWarningResp.\
       protocolIEs.member[ieCount];
   ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MsgIdentifier, SztCriticalityrejectEnum );
   s1apWrapMsgIdentifier( &(ie->value.u.sztMsgIdentifier), writeReplcWarnRsp->warnMsgId );

   /* IE2 - SerialNumber */
   ie = &writeReplcWarnRspPdu->pdu.val.successfulOutcome.value.u.sztWriteReplaceWarningResp.\
       protocolIEs.member[ieCount];
   ieCount++;
   s1apWrapIeDefFields( ie, Sztid_SerialNumber, SztCriticalityrejectEnum );
   s1apWrapSerialNumber( &(ie->value.u.sztSerialNumber), writeReplcWarnRsp->serialNum );

   /* IE3 - Filling Broadcast Cmplete Area List */
   if( TRUE == writeReplcWarnRsp->isBroadcastCmpltAreaListPresent )
   {
     logError("%s", "Error:This condition should NOT have hit, as WarningAreaLst will not be filled for fail response ");
     return FALSE;
   }

   /* IE4 - Filling Critical Diagnostics */
   if( TRUE == writeReplcWarnRsp->isCriticalDiagnosticPresent )
   {
     ie = &writeReplcWarnRspPdu->pdu.val.successfulOutcome.value.u.sztWriteReplaceWarningResp.\
         protocolIEs.member[ieCount];
     ieCount++;
     s1apWrapIeDefFields( ie, Sztid_CriticalityDiag, SztCriticalityignoreEnum );
     if( FALSE == s1apWrapCriticalDiagnostic( writeReplcWarnRspPdu, &(ie->value.u.sztCriticalityDiag), &(writeReplcWarnRsp->criticalDiagnostic) ) )
     {
       logError("%s", "Error in wrapping Critical Diagnostics");
       return FALSE;
     }
   }

   *pdu = writeReplcWarnRspPdu;
   return TRUE;
}


/****
 *
 *  Functions added for S1AP based Handover feature
 *
 * ****/
/**
 * @param   hovrTypPdu   CCPU  handover Type
 * @param   handoverType PWAV  handover Type
 * @details This functions fills the hovrTypPdu with the values in handoverType.
 */
static void s1apWrapHandoverType(
  IO SztHovrTyp        *hovrTypPdu,
  IN HandoverType       handoverType
  )
{
  uehSzFillTknU32( hovrTypPdu, handoverType );
}

/**
 * @param   targetIdPdu   CCPU  target Id
 * @param   targetId      PWAV  target Id
 * @details This functions fills the targetIdPdu with the values in targetId.
 */
static TBOOL s1apWrapTargetId(
  IO S1apPdu          *pdu,
  IO SztTgetID        *targetIdPdu,
  IN TargetId         *targetId
  )
{
  uehSzFillTknU8( &(targetIdPdu->choice ), targetId->targetIdType );
  switch ( targetId->targetIdType )
  {
    case TargetType_TargetEnbId:
        {
          uehSzFillTknU8( &(targetIdPdu->val.targeteNB_ID.pres), PRSNT_NODEF );
          if ( FALSE EQ s1apWrapEnodeBId( pdu, &(targetIdPdu->val.targeteNB_ID.global_ENB_ID), &(targetId->val.targetEnbID.globalEnodeBId) ) )
          {
            logError("%s", "Error in wrapping Global Enodeb ID ");
            return FALSE;
          }
          if( FALSE == s1apWrapTai( pdu, &(targetIdPdu->val.targeteNB_ID.selected_TAI) , &(targetId->val.targetEnbID.tai) ) )
          {
            logError("%s", "Error in wrapping TAI");
            return FALSE;
          }
          targetIdPdu->val.targeteNB_ID.iE_Extns.noComp.pres = NOTPRSNT;
        }
        break;
    case TargetType_TargetRncId:
        {
          logError("%s", "TargetType : TargetType_TargetRncId not Supported yet");
          return FALSE;
        }
        break;
    case TargetType_Cgi:
        {
          logError("%s", "TargetType : TargetType_Cgi not Supported yet");
          return FALSE;
        }
        break;
    default:
      logError("Invalid Target Type :%d ", targetId->targetIdType );
      return FALSE;
  }
  return TRUE;
}

static void s1apWrapDirectFwdPathAvailability(
  IO SztDirect_Fwding_Path_Avlblty        *directFwdPathAvailabilityPdu,
  IN DirectFwdPathAvailability             directFwdPathAvailability
  )
{
  uehSzFillTknU32( (TknU32*)directFwdPathAvailabilityPdu, directFwdPathAvailability );
}

static void s1apWrapSrvccHoIndication(
  IO SztSRVCCHOInd             *srvccHoIndPdu,
  IN SrvccHoIndication          srvccHoIndication
  )
{
  uehSzFillTknU32( (TknU32*)srvccHoIndPdu, srvccHoIndication );
}

static TBOOL s1apWrapMsClassmark2(
  IO S1apPdu             *pdu,
  IO SztMSClassmark2     *msClassmark2Pdu,
  IN UINT8               *msClassmark2
  )
{
  if ((cmGetMem(pdu, MAX_SIZE_MS_CLASSMARK_2,
             (Ptr*)&(msClassmark2Pdu->val)) ) != ROK )
  {
    logError("cmGetMem failed for msClassmark2 of size:%d", MAX_SIZE_MS_CLASSMARK_2 );
    return FALSE;
  }

  msClassmark2Pdu->pres = PRSNT_NODEF;
  msClassmark2Pdu->len  = MAX_SIZE_MS_CLASSMARK_2;
  memcpy( (msClassmark2Pdu->val), msClassmark2, MAX_SIZE_MS_CLASSMARK_2 ) ;

  return TRUE;
}

static TBOOL s1apWrapMsClassmark3(
  IO S1apPdu             *pdu,
  IO SztMSClassmark3     *msClassmark3Pdu,
  IN UINT8               *msClassmark3
  )
{
  if ((cmGetMem(pdu, MAX_SIZE_MS_CLASSMARK_3,
             (Ptr*)&(msClassmark3Pdu->val)) ) != ROK )
  {
    logError("cmGetMem failed for msClassmark3 of size:%d", MAX_SIZE_MS_CLASSMARK_3 );
    return FALSE;
  }

  msClassmark3Pdu->pres = PRSNT_NODEF;
  msClassmark3Pdu->len  = MAX_SIZE_MS_CLASSMARK_3;
  memcpy( (msClassmark3Pdu->val), msClassmark3, MAX_SIZE_MS_CLASSMARK_3 ) ;

  return TRUE;
}

static TBOOL s1apWrapSrcToTgtTprntContPdu(
    IO S1apPdu                             *pdu,
    IO SztSrc_ToTget_TprntCont             *sztSrc_ToTget_TprntContPdu,
    IN SrcToTgtTprntCont                   *srcToTgtTprntCont,
    IN HandoverType                        handoverType
    )
{
  sztSrc_ToTget_TprntContPdu->pres = PRSNT_NODEF;

  switch ( handoverType )
  {
    case HandoverType_IntraLte:
      {
        if ((cmGetMem(pdu, sizeof(SrcEnbToTargetEnbTransparentCont),
                   (Ptr*)&(sztSrc_ToTget_TprntContPdu->val)) ) != ROK )
        {
          logError("cmGetMem failed for msClassmark2 of size:%d", sizeof(SrcEnbToTargetEnbTransparentCont) );
          return FALSE;
        }
        sztSrc_ToTget_TprntContPdu->len = srcToTgtTprntCont->msgLen;
        memcpy( (sztSrc_ToTget_TprntContPdu->val), srcToTgtTprntCont->msgPdu, srcToTgtTprntCont->msgLen ) ;
      }
      break;
    default:
      BS_ERROR_S1AP_HANDOVER_TYPE_NOT_SUPPORTED("HandoverType_ToAdd");
      return FALSE;
  }
  return TRUE;
}

TBOOL s1apWrapSrcToTargetTransparentContainer(
    IO S1apPdu                             *pdu,
    IO SrcToTargetTransparentContainer     *srcToTgetTprntCont,
    IO UehUeHovrInfo                       *ueHovrInfo
    )
{
  S1apPdu                        *srcToTgetTprntContPdu = NULLP;
  S16                            ret;
  U16                            numComp;

  logInfo("%s", "Wrap Src To Target Transparent Container Message ");

  /* Initialise memory control point */
  ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&srcToTgetTprntContPdu);
  if(ret != ROK)
  {
    logError("cmAllocEvnt failed for Src To target Message, Size:%d", sizeof(S1apPdu) );
    RETVALUE(RFAILED);
  }

  if ( HandoverType_IntraLte EQ ueHovrInfo->handoverType )
  {
    SrcEnbToTargetEnbTransparentCont  *srcEnbToTgetEnb = &(srcToTgetTprntCont->srcEnbToTargetEnb);
    /* Fill the message Id */
    s1apWrapSuccMsgId( srcToTgetTprntContPdu, Sztid_Src_ToTget_TprntCont );

    /* Filling 3 mandatory IEs in Src To Target Transparent Container */
     numComp = 3;
     /* Filling 2 optional IEs in Src To Target Transparent Container */
     if( TRUE == srcEnbToTgetEnb->isSubProfileIdOrFreqPriorityPresent )
       numComp++;
     if( TRUE == srcEnbToTgetEnb->isErabsInfoListPresent )
       numComp++;

     s1apWrapIeCount( srcToTgetTprntContPdu, numComp );

     /* Get memory for numComp IEs */
/*   TODO: sztSrcToTget is not present in s1ap PDU by ccpu
     if ( (cmGetMem(srcToTgetTprntContPdu, numComp * sizeof(SztSrceNB_ToTgeteNB_TprntCont ),\
                     (Ptr*)&srcToTgetTprntContPdu->pdu.val.initiatingMsg.value.u.sztSrcToTget.\
                      protocolIEs.member)) != ROK )*/
    {
      logError("cmGetMem failed for IE in srcEnbToTgetEnbTprntCont, Size:%d", numComp * sizeof(SztSrceNB_ToTgeteNB_TprntCont) );
      RETVALUE(RFAILED);
    }
    /* TODO: After CCPU adds this field to s1apPdu */
    return TRUE;
  }
  else
  {
    BS_ERROR_S1AP_HANDOVER_TYPE_NOT_SUPPORTED("HandoverType_ToAdd");
    return FALSE;
  }

  return FALSE;

}

/**
 * @param   pdu          S1AP PDU
 * @param   initialUeMsg Initial UE Message
 * @details
 * This functions allocates memory for the S1AP PDU, Fills the IE's of the S1 PDU with the
 * values in handoverRequired message.
 */
TBOOL s1apWrapHandoverRequiredMsgPdu(
  IO S1apPdu                  **pdu,
  IN HandoverRequired         *handoverRequired
  )
{
   S1apPdu                     *handoverReqdPdu = NULLP;
   U16                          numComp, ieCount=0;
   S16                          ret;
   SztProtIE_Field_HovrReqdIEs *ie;

   logInfo("%s", "Wrap Handover Required Message");

   /* Initialise memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &uehCb.mem, (Ptr *)&handoverReqdPdu);
   if(ret != ROK)
   {
     logError("cmAllocEvnt failed for HandoverRequired Message, Size:%d", sizeof(S1apPdu) );
     RETVALUE(FALSE);
   }

   /* Fill the message Id */
   s1apWrapSuccMsgId( handoverReqdPdu, Sztid_HovrPrep );

   /* Filling 6 mandatory IEs in Handover Required Message */
   numComp = 6;
   /* Filling 5 optional IEs in Handover Required Message */
   if( TRUE == handoverRequired->isDirectFwdPathAvailabilityUsed)
     numComp++;
   if( TRUE == handoverRequired->isSrvccHoIndicationUsed )
     numComp++;
   if( TRUE == handoverRequired->isSrcToTgtTprntContSecondary )
     numComp++;
   if( TRUE == handoverRequired->isMsClassmark2Used )
     numComp++;
   if( TRUE == handoverRequired->isMsClassmark3Used )
     numComp++;

   s1apWrapIeCount( handoverReqdPdu, numComp );

   /* Get memory for numComp IEs */
   if ( (cmGetMem(handoverReqdPdu, numComp * sizeof(SztProtIE_Field_HovrReqdIEs ),\
                   (Ptr*)&handoverReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.\
                    protocolIEs.member)) != ROK )
   {
     logError("cmGetMem failed for IE's in handoverRequired, Size:%d", numComp * sizeof(SztProtIE_Field_HovrReqdIEs) );
     RETVALUE(FALSE);
   }

   /* IE1 - Filling MME UE S1AP Id - Mandatory IE */
   ie = &handoverReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.member[ieCount];
   ieCount++;
   s1apWrapIeDefFields( ie, Sztid_MME_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapMmeUeS1apId( &(ie->value.u.sztMME_UE_S1AP_ID), (handoverRequired->mmeUeS1apId) );

   /* IE2 - Filling ENB UE S1AP Id - Mandatory IE */
   ie = &handoverReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.member[ieCount];
   ieCount++;
   s1apWrapIeDefFields( ie, Sztid_eNB_UE_S1AP_ID, SztCriticalityrejectEnum );
   s1apWrapEnodeBUeS1apId( &(ie->value.u.sztENB_UE_S1AP_ID), (handoverRequired->enbUeS1apId) );

   /* IE3 - Fill Handover Type - Mandatory IE */
   ie = &handoverReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.member[ieCount];
   ieCount++;
   s1apWrapIeDefFields( ie, Sztid_HovrTyp, SztCriticalityrejectEnum );
   s1apWrapHandoverType( &(ie->value.u.sztHovrTyp), (handoverRequired->handoverType) );

   /* IE4 - Fill cause - Mandatory IE */
   ie = &handoverReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.member[ieCount];
   ieCount++;
   s1apWrapIeDefFields( ie, Sztid_Cause, SztCriticalityignoreEnum );
   s1apWrapCause( &(ie->value.u.sztCause),&(handoverRequired->cause) );

   /* IE5 - Fill TargetId - Mandatory IE */
   ie = &handoverReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.member[ieCount];
   ieCount++;
   s1apWrapIeDefFields( ie, Sztid_TgetID, SztCriticalityrejectEnum );
   if( FALSE EQ s1apWrapTargetId(  handoverReqdPdu, &(ie->value.u.sztTgetID), &(handoverRequired->targetId) ) )
     return FALSE;

   /* IE5 - Fill DirectFwdPathAvailability - Optional IE */
   if( TRUE EQ handoverRequired->isDirectFwdPathAvailabilityUsed )
   {
     ie = &handoverReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.member[ieCount];
     ieCount++;
     s1apWrapIeDefFields( ie, Sztid_Direct_Fwding_Path_Avlblty, SztCriticalityignoreEnum );
     s1apWrapDirectFwdPathAvailability( &(ie->value.u.sztDirect_Fwding_Path_Avlblty), handoverRequired->directFwdPathAvailability );
   }

   /* IE6 - Fill SrvccHoIndication - Optional IE */
   if( TRUE EQ handoverRequired->isSrvccHoIndicationUsed )
   {
     ie = &handoverReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.member[ieCount];
     ieCount++;
     s1apWrapIeDefFields( ie, Sztid_SRVCCHOInd, SztCriticalityrejectEnum );
     s1apWrapSrvccHoIndication( &(ie->value.u.sztSRVCCHOInd), handoverRequired->srvccHoIndication );
   }

   /* IE7 - Fill srcToTargetTransparentContainer - Mandatory IE */
   ie = &handoverReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.member[ieCount];
   ieCount++;
   s1apWrapIeDefFields( ie, Sztid_Src_ToTget_TprntCont, SztCriticalityrejectEnum );
   s1apWrapSrcToTgtTprntContPdu( handoverReqdPdu, &(ie->value.u.sztSrc_ToTget_TprntCont), \
                                            &(handoverRequired->srcToTgtTprntCont),
                                            handoverRequired->handoverType);

   /* IE8 - Fill SrcToTargetTransparentContainerSecondary - Optional IE */
   if( TRUE EQ handoverRequired->isSrcToTgtTprntContSecondary )
   {
     ie = &handoverReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.member[ieCount];
     ieCount++;
     s1apWrapIeDefFields( ie, Sztid_Src_ToTget_TprntCont_Secondary, SztCriticalityrejectEnum );
     /* Size of SrcToTgtTprntCnt and SrcToTgtTprntCntSecondary are same.
      * Hence calling same function as SrcToTgtTprntCnt. Verify if required *
      */
     s1apWrapSrcToTgtTprntContPdu( handoverReqdPdu, &(ie->value.u.sztSrc_ToTget_TprntCont), \
                                              &(handoverRequired->srcToTgtTprntContSecondary),
                                              handoverRequired->handoverType);
   }

   /* IE9 - Fill MsClassmark2 - Optional IE */
   if( TRUE EQ handoverRequired->isMsClassmark2Used )
   {
     ie = &handoverReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.member[ieCount];
     ieCount++;
     s1apWrapIeDefFields( ie, Sztid_MSClassmark2, SztCriticalityrejectEnum );
     s1apWrapMsClassmark2( handoverReqdPdu, &(ie->value.u.sztMSClassmark2),
                                              (UINT8*)&(handoverRequired->msClassmark2) );
   }

   /* IE10 - Fill MsClassmark3 - Optional IE */
   if( TRUE EQ handoverRequired->isMsClassmark3Used )
   {
     ie = &handoverReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.member[ieCount];
     ieCount++;
     s1apWrapIeDefFields( ie, Sztid_MSClassmark3, SztCriticalityignoreEnum );
     s1apWrapMsClassmark3( handoverReqdPdu, &(ie->value.u.sztMSClassmark3),
                                              (UINT8*)&(handoverRequired->msClassmark3) );
   }

   *pdu = handoverReqdPdu;
   return TRUE;
}

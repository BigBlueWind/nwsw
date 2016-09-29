/**
 * @file    ueh_relay.c
 * @brief This file contains relay module for UEH
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. @author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 16Sep2010 :
*     a) Created initial version from sample application
 */

#include "ccpu_cmn.h"

/* RRM related includes */

#include "ueh_eut.h"
#include "ueh.h" 
#include "lueh.h" 
#include "ueh_eut_x.h"
#include "lueh.x" 
#include "s1ap_cm.h"
#include "s1ap_unwrap.h"
#include "s1ap_wrap.h"
#include "ueh_cmn_sec.h"
#include "ueh_s1ap_cm.h"
#include "ueh_crm.h"
#include "log.h"
#include "sendToOam.h"
#include "pst_events.h"
#include "start_indication.h"

/* eNodeB Cb */
UehCb          uehCb;
UINT16         pagingCycleVal[4] = { 32, 64, 128, 256};
UINT8          nBEnumMappingVal[7] = { 4, 2, 1, 2, 4, 8, 16 };
/* po = poMappingVal[Ns/2][Is] ; Note: 0xFF is invalid filled value */
UINT8          poMappingValFromAsAndNs[3][4] = { {9,0XFF,0XFF,0XFF},{4,9,0XFF,0XFF},{0,4,5,9} };

inline PUBLIC S16 sendUehMsgToBsm (Event event, MsgLen len, U8 *data) { return sendEventToOam(ENTUEH, ENT_BSM, event, len, data); }
/*Change this to ENTCEM, when CEM registers with SSI*/
inline PUBLIC S16 sendCemMsgToBsm (Event event, MsgLen len, U8 *data) { return sendEventToOam(ENTUEH, ENT_BSM, event, len, data); }
inline PUBLIC S16 sendUehMsgToPrm (Event event, MsgLen len, U8 *data) { return sendEventToOam(ENTUEH, ENT_PRM, event, len, data); }
PUBLIC S16 sendUehMsgToPmh (Event event, MsgLen len, U8 *data) { return sendEventToOam(ENTUEH, ENT_PMH, event, len, data); }

void sendUehStartIndToPrm(void)
{
  U32 version = getSvnVersion(MODID_UEH);
  logInfo ("UEH svn version : %d", version);
  printf  ("UEH svn version : %u\n", (unsigned int)version);
  /*send start indication to PRM indicating successful startup*/
  if(ROK EQ sendUehMsgToPrm(EVT_START_IND, sizeof(U32), (U8 *)&version ))
  {
    logMsgOut("%s", "Sent EVT_START_IND from UEH to PRM" );
  }
}

void sendUehHelloAckToPrm(Buffer   *mBuf)
{
  MsgLen              cCnt;
  MsgPrmModHello      hello;
  MsgModPrmHelloAck   helloAck;
  clearBytes( &helloAck, sizeof(MsgModPrmHelloAck) );

  SCpyMsgFix( mBuf, 0, sizeof(MsgPrmModHello), (Data*)&hello , &cCnt );

  helloAck.helloAckSeqNum = hello.helloSeqNum;

  if(ROK EQ sendUehMsgToPrm(EVT_HELLO_ACK, sizeof(MsgModPrmHelloAck), (U8*)&helloAck))
  {
    logMsgOut("%s", "Sent EVT_HELLO_ACK from UEH to PRM" );
  }
}
void procRcvdUehExitMsg(void)
{
  logMsgIn("%s", "Received Exit Message" );
  exit(0);
}


/*
*       Fun:   uehSndInitCtxtSetupFail 
*
*       Desc:  Initial Context Setup Failure
*              The only way to force MME to send UE ContextClearCommand is to send UEContextClearRequest
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_relay.c 
*
*/
void uehSndInitCtxtSetupFail(
    UehS1ConCb   *s1ConCb,
    UINT16        causeType,
    UINT32        causeVal
  )
{
  UehUeCb                       *ueCb = NULLP;
  SztRelReq                     relReq;
  InitialContextSetupFail       initCtxSetupFailMsg;
  S1Cause                       cause;

  ueCb = s1ConCb->ueCb;

  cause.causeType = causeType;
  cause.causeVal  = causeVal;

  /* Change the state
   * Construct InitContextSetupFail S1AP Message,
   */
  uehFillInitCtxSetupFail( &initCtxSetupFailMsg, s1ConCb, &cause );
  s1apWrapInitialContextSetupFailPdu( &relReq.pdu, &initCtxSetupFailMsg );
  relReq.relType = SZT_CXT_REL;
  relReq.connIdType = SZT_CONN_TYPE_SP;
  relReq.connId = ueCb->s1ConCb.spConnId;
  ueCb->s1ConCb.s1Cause = causeVal;
  ueCb->s1ConCb.s1CauseType = causeType;

  ueCb->ueProcedure = UEH_INITIAL_CXT_SETUP_FAILURE;

  /* Send the message to S1ap Stack */
  uehSndMsgOut (ENTSZ, EVENTTYPE_INITCONTEXTSETUPFAIL, ueCb, TRANSID_NONE,&relReq);
}

void uehSndUeCtxtModFail(
    UehUeCb       *ueCb,
    UINT16        causeType,
    UINT32        causeVal
  )
{
  SztDatEvntReq          datEvt;
  UeContextModFail       ueCtxModFailMsg;
  S1Cause                cause;
  UehS1ConCb             *s1ConCb;
  cause.causeType = causeType;
  cause.causeVal  = causeVal;

  /* Change the state
   * Construct InitContextSetupFail S1AP Message,
   */
  s1ConCb = &ueCb->s1ConCb;
  uehFillUeCtxModFail( &ueCtxModFailMsg, s1ConCb, &cause );
  s1apWrapUecontextModFailPdu( &datEvt.pdu, &ueCtxModFailMsg );

  /* Send the message to S1ap Stack */
  datEvt.spConnId = s1ConCb->spConnId;
  uehSndMsgOut (ENTSZ, EVENTTYPE_UECONTEXTMODFAIL, ueCb, TRANSID_NONE,&datEvt);
}

void uehStoreInitialCntxtInfo (
    UehUeCb               *ueCb,
    InitialContextSetup   *initialContextSetup
    )
{
  U8                    rbIdx = 0;
  U8                    shiftBits = 0;
  U16                   idx = 0;
  U16                   ipAddrIdx = 0;
  U32                   addrMsk = 0;
  ERabToSetupItem       *eRabToSetupItem;
  UehUeRABInfo          *rbInfo;

  /* Store UE Aggregate Maximum Bit Rate */
  ueCb->aggMaxBr.dl = getUint32FromArray(initialContextSetup->ueAggMaxBitRate.ueAggMaxBitRateDl.bitRate);
  ueCb->aggMaxBr.ul = getUint32FromArray(initialContextSetup->ueAggMaxBitRate.ueAggMaxBitRateUl.bitRate);

  /* Store the RAB List details */
  rbInfo = &(ueCb->rbInfo);
  rbInfo->noNasPdus = 0;
  rbInfo->numOfDrbAdd = initialContextSetup->eRabToSetupList.noComp;

  for(idx = 0; idx < rbInfo->numOfDrbAdd; idx++)
  {
    eRabToSetupItem = &(initialContextSetup->eRabToSetupList.eRabToSetupItem[idx]);

    /* Update  local UE Cb
     * We assume that DRB start, increment numbers of DRBs */
    rbIdx = rbInfo->numOfRbCfgd;
    rbInfo->numOfRbCfgd++;

    rbInfo->drb[rbIdx].rbType = CM_LTE_DRB;
    rbInfo->drb[rbIdx].rbId = (rbIdx + 3); /* starting from 3 .. 10 */
    rbInfo->drb[rbIdx].eRABId = eRabToSetupItem->eRabId;
    rbInfo->drb[rbIdx].pdcpId = (rbIdx + 3);
    rbInfo->drb[rbIdx].rgrCfgMask = UEH_RB_RGRMSK_ZERO;
    rbInfo->drb[rbIdx].isEgtpTnlEnbl = FALSE;

    rbInfo->drbAddLstIndx[idx] = rbIdx;

    rbInfo->drb[rbIdx].qci = eRabToSetupItem->eRabLevelQosParams.qci;
    /* TODO Check the Conversion later */
    rbInfo->drb[rbIdx].mbr.dl = getUint32FromArray(eRabToSetupItem->eRabLevelQosParams.gbrQosInfo.ueMaxBitRateDl.bitRate);
    rbInfo->drb[rbIdx].mbr.ul = getUint32FromArray(eRabToSetupItem->eRabLevelQosParams.gbrQosInfo.ueMaxBitRateUl.bitRate);
    rbInfo->drb[rbIdx].gbr.dl = getUint32FromArray(eRabToSetupItem->eRabLevelQosParams.gbrQosInfo.ueGurantedBitRateDl.bitRate);
    rbInfo->drb[rbIdx].gbr.ul = getUint32FromArray(eRabToSetupItem->eRabLevelQosParams.gbrQosInfo.ueGurantedBitRateUl.bitRate);

    /* Store Remote GTP Tunnel ID */
    rbInfo->drb[rbIdx].remEgtpTunId = getUint32FromArray(eRabToSetupItem->gtPTeId.gtPTeId);

    /* retrieve Serv. GW Address later will be used for tunnel creation */
    switch( eRabToSetupItem->transportLayerAddr.transportAddrType )
    {
      case TrasportAddrType_Ipv4:
      {
        rbInfo->drb[rbIdx].sgwAddr.type = CM_TPTADDR_IPV4;
        rbInfo->drb[rbIdx].sgwAddr.u.ipv4TptAddr.port = UEH_DFLT_EGTP_PORT;

        /* copy 4 bytes into the U32 */
        shiftBits = SIZE_OF_IPV4;
        addrMsk = 0xFF000000;
        for(ipAddrIdx = 0; ipAddrIdx < 4; ipAddrIdx++)
        {
          shiftBits--;
          rbInfo->drb[rbIdx].sgwAddr.u.ipv4TptAddr.address |=
            ((U32)( eRabToSetupItem->transportLayerAddr.transportLevelAddr[ipAddrIdx] << (8 * shiftBits)) & addrMsk);
          addrMsk = addrMsk >> 8;
        }
        break;
      }

      case TrasportAddrType_Ipv6:
      {
        rbInfo->drb[rbIdx].sgwAddr.type = CM_TPTADDR_IPV6;
        rbInfo->drb[rbIdx].sgwAddr.u.ipv6TptAddr.port = UEH_DFLT_EGTP_PORT;
        for (ipAddrIdx = 0; ipAddrIdx < 16; ipAddrIdx++)
        {
          rbInfo->drb[rbIdx].sgwAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] =
            eRabToSetupItem->transportLayerAddr.transportLevelAddr[ipAddrIdx];
        }
        break;
      }

      default:
      {
        /* would be in case if len = 160 bits which means support for both ipv4 and
         * ipv6 */
        UEH_ERROR_ELMNT_OUT_OF_RANGE("Transport Addr Type", eRabToSetupItem->transportLayerAddr.transportAddrType, TrasportAddrType_Ipv4, TrasportAddrType_Ipv6);
        break;
      }
    }

    /* Store the NAS PDU list */
    if(TRUE EQ eRabToSetupItem->isNasPdu)
    {
      rbInfo->noNasPdus++;
      rbInfo->nasPdu[idx].pduLen = eRabToSetupItem->nasPdu.pduLen;
      cmMemcpy( (U8 *)rbInfo->nasPdu[idx].pdu, (U8 *)eRabToSetupItem->nasPdu.pdu, eRabToSetupItem->nasPdu.pduLen  );
    }

  }

  /* Store the Ue Radio Capability */
  if (TRUE EQ initialContextSetup->isUeRadioCapability)
  {
    ueCb->ueRadCap.isUeRadAccCap = TRUE;
    ueCb->ueRadCap.ueRadAccCap.len = initialContextSetup->ueRadioCapability.len;
    cmMemcpy(ueCb->ueRadCap.ueRadAccCap.UeRadioCapability, initialContextSetup->ueRadioCapability.UeRadioCapability, ueCb->ueRadCap.ueRadAccCap.len);
  }

}

/*
*
*       Fun:   uehProcInitCtxtSetup 
*
*       Desc:  Initial Context Setup 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_relay.c 
*
*/
PUBLIC S16 uehProcInitCtxtSetup(
    UehUeCb                *ueCb,
    InitialContextSetup *initialContextSetup
  )
{
  logFunc("%s","uehProcInitCtxtSetup");

  uehSetState(ueCb, UE_STATE_CONNECTED);
  /* TODO Trace Feature IE To be handled */
  if( TRUE EQ initialContextSetup->isTraceActivation)
  {
    uehHandleTraceStart(ueCb, &(initialContextSetup->traceActivation));
  }
  /* TODO Code to be added to handle Handover */

  /* CS1: Security Configuration */
  /* Fetch the security related values */
  #if 0
  if( ROK NOTEQ uehGetSecInfoFrmInitCtxSetupReq( ueCb, initialContextSetup ) )
  {
    UEH_PROCEDURE_FAILED("Security Algorithm Selection", UE, ueCb->crnti, "Integrity Alg not supported by UE");
    uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_RadioNw, SztCauseRadioNwencryption_and_or_integrity_protection_algorithms_not_supportedEnum);
    RETVALUE(RFAILED);
  }
  #endif

/* Store the ActiveUeStatus params */
  ueCb->activeIntProtAlgo = ueCb->secCfg.intgAlgoSelected;
  /* Create SRB2 */
  uehInitUeSrbInfo(&ueCb->rbInfo, UEH_PDCP_ID_SRB2);

  /* Save the User Info needed to frame the reconfig request */
  uehStoreInitialCntxtInfo (ueCb, initialContextSetup);

  if(TRUE EQ smCb.cfgCb.uehConfig.isUeSecEnble) //isUeSecEnble)
  {
    /* TODO Check the algorithms supported, based on this take a decision on
     * whether to continue with the call or drop the call
    if (ROK != uehSndRrcSecModeCmd(ueCb))
    {
      // TODO Decide on what to be done in case of error 
      UEH_ERROR_SSI_EVENT_SEND_FAILED( (&uehCb.nhuSap[0]->pst), EVENTTYPE_SECMODECOMMAND, UE, ueCb->crnti);
      RETVALUE(RFAILED);
    }*/
   /* Fetch the security related values */
    if( ROK != uehGetSecInfoFrmInitCtxSetupReq( ueCb, initialContextSetup ) )
    {
      UEH_PROCEDURE_FAILED("Security Algorithm Selection", UE, ueCb->crnti, "Integrity Alg not supported by UE");
      uehSndInitCtxtSetupFail(&ueCb->s1ConCb, CauseType_RadioNw, SztCauseRadioNwencryption_and_or_integrity_protection_algorithms_not_supportedEnum);
      RETVALUE(RFAILED);
    }
    uehSetState(ueCb, UE_STATE_SMC_ONGOING);
  }
  else
  {
    uehProcUeCapabilityInfo(ueCb);
  }

  RETVALUE(ROK);
} /* uehLiSztInitConSetHandler */

PUBLIC S16 uehProcInitCtxtSetupInS1Con(
    UehUeCb      *ueCb,
    S1apPdu      *pdu
  )
{
  InitialContextSetup   initialContextSetup;
  UehS1ConCb            *s1ConCb;
  S16                   ret;

  logInfo("%s","uehProcInitCtxtSetupInS1Con: Triggering Initial Context Setup Message");

  s1ConCb = &ueCb->s1ConCb;

  /* Decode the received PDU */
  s1apUnwrapInitialContextSetupPdu( pdu, &initialContextSetup );

  /* Save the MmeUeS1apId,
  * EnbUeS1apId, CCPU expects the place holder for this in InitialUeMessage, The value filled by RRM is
  * ignored and a local reference is filled by S1AP Stack. Save this after Connection confirmation
  * Stop the SRB1 timer
  */
  if(0 EQ s1ConCb->mme_ue_s1ap_id) s1ConCb->mme_ue_s1ap_id = initialContextSetup.mmeUeS1apId;
  if(0 EQ s1ConCb->enb_ue_s1ap_id) s1ConCb->enb_ue_s1ap_id = initialContextSetup.enbUeS1apId;
  if (RFAILED == uehValidateEnbAndMmeUeS1apIds(s1ConCb->mme_ue_s1ap_id,s1ConCb->enb_ue_s1ap_id,ueCb->crnti))
  {
    // TempFix: This error situation creates a context hang by not handling Context-Rel-Cmd in UE_STATE_S1AP_CONNECTING. So moving to UE_STATE_CONNECTED
    uehSetState(ueCb, UE_STATE_CONNECTED);
    RETVALUE(RFAILED);
  }
  ret = uehProcInitCtxtSetup( ueCb, &initialContextSetup );

  RETVALUE(ret);
}

PUBLIC S16 uehProcInitCtxtSetupInUeCntd(
    UehUeCb      *ueCb,
    S1apPdu      *pdu
  )
{
   InitialContextSetup initialContextSetup;

   logInfo("%s","uehProcInitCtxtSetupInUeCntd: Triggering Initial Context Setup Message");

   /* Decode the received PDU */
   s1apUnwrapInitialContextSetupPdu( pdu, &initialContextSetup );
   if (RFAILED == uehValidateEnbAndMmeUeS1apIds(initialContextSetup.mmeUeS1apId,initialContextSetup.enbUeS1apId,ueCb->crnti))
   {
     RETVALUE(RFAILED);
   }

   return (uehProcInitCtxtSetup(ueCb, &initialContextSetup));
}

#ifdef FT_ERAB_SETUP_REL
/*
*
*       Fun:   veLiSztERABSetHandler
*
*       Desc:  E-RAB Setup Request Handler
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_relay.c
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztERABSetHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztERABSetHandler(s1ConCb,pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{
   VeUeCb *ueCb = NULLP;

   logFunc("%s", "veLiSztERABSetHandler");

   logInfo ("%s", "veLiSztERABSetHandler: Triggering E-RAB Setup Request Message");


   /* Restore PDU, will be freed only after reconfig message is sent */
   ueCb = s1ConCb->ueCb;

   ueCb->pdu = (S1apPdu *) pdu;

    if(ueCb->pdu != NULLP)
   {
      /* Trigger RRC DL Connection Reconfiguration message*/
      veSndRrcRecfgReq(ueCb, Sztid_E_RABSetup);
   }
   else
   {
	   logInfo ("veLiSztERABSetHandler(): S1AP doesnot exist for crnti (%d) ", ueCb->crnti);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veLiSztERABSetHandler */


/*
*
*       Fun:   veLiSztERABModHandler
*
*       Desc:  E-RAB Modify Request Handler
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_relay.c
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztERABModHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztERABModHandler(s1ConCb,pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{

	logFunc("%s",veLiSztERABModHandler);

   logInfo ("%s", "veLiSztERABModHandler: Triggering E-RAB Modify Request Message ");

   /* Currently not supporting this procedure */

   RETVALUE(ROK);
} /* veLiSztERABModHandler */

/*
*
*       Fun:   veLiSztERABRelComHandler
*
*       Desc:  E-RAB Release Command Handler
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_relay.c
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztERABRelComHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztERABRelComHandler(s1ConCb,pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{
   VeUeCb *ueCb = NULLP;

   logFunc("%s", "veLiSztERABRelComHandler");

   logInfo ("%s", "veLiSztERABRelComHandler: Triggering E-RAB Release Command Message ");

   /* Restore PDU, will be freed only after reconfig message is sent */
   ueCb = s1ConCb->ueCb;

   ueCb->pdu = (S1apPdu *) pdu;

      if(ueCb->pdu != NULLP)
   {
      /* Trigger RRC DL Connection Reconfiguration message*/
      veSndRrcRecfgReq(ueCb, Sztid_E_RABRls);
   }
   else
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veLiSztERABRelComHandler */

#endif /* FT_ERAB_SETUP_REL */


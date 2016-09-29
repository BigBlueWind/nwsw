/********************************************************************16**

                         (c) COPYRIGHT 2009 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9450, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/**********************************************************************

     Name:     LTE-RRC User
  
     Type:     C Source file
  
     Desc:     Relay Module 

     Ret :     ROK - success
               RFAILED - failure

     File:     ve_relay.c

     Sid:      ve_relay.c@@/main/2 - Wed Dec  1 08:44:28 2010

     Prg:  

**********************************************************************/

#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_pasn.h"       /* Common Pasn includes */
#include "cm_tpt.h"       /* Common Pasn includes */

#include "nhu.h"           /* NHU interface defines */
#include "lnh.h"           /* Layer Manager interface defines */
#include "pju.h"           /* PDCP data interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "lrg.h"           /* MAC RRM control Interface */
#include "szt.h"           /* MAC RRM control Interface */
#include "szt_asn.h"           /* MAC RRM control Interface */


/* RRM related includes */
#include "ve_eut.h"
#include "egt.h"
#include "ctf.h"
#include "ve.h" 
#include "lve.h" 

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_lib.x"
#include "cm_pasn.x"       /* Common Pasn includes */
#include "cm_tpt.x"       /* Common Pasn includes */

#include "nhu.x"           /* NHU Interface includes */
#include "lnh.x"           /* LNH Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "lrg.x"           /* MAC RRM control Interface */
#include "nhu_asn.x"
#include "szt.x"
#include "czt.x"
#include "szt_asn.x"

/* RRM related includes */
#include "ve_eut.x"
#include "egt.x"
#include "ctf.x"
#include "lve.x" 
#include "ve.x" 
#ifndef MSPD
#include "ve_dflt.h"
#else
#include "ve_dflt_ms.x"
#endif

#ifdef __cplusplus
extern "C" {
#endif

EXTERN S16 szDbFillResetAck ARGS ((S1apPdu *pdu,
                                  S1apPdu **rstAckPdu, 
                                  Mem mem));

EXTERN S16 szDbFillResetFrmPdu ARGS ((S1apPdu *pdu,
                                      S1apPdu **rstPdu,
                                      Mem mem));

#ifdef __cplusplus
}
#endif /* __cplusplus */


/* eNodeB Cb */
PUBLIC VeCb      veCb;

#define VE_S1_RESET_TMR_VAL  60
#define VE_RESET_RTX_VAL     3

#ifndef LTE_ENB_PERF
/*
*
*       Fun:   veSztS1SetReq 
*
*       Desc:  S1 Setup Request 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_relay.c 
*
*/
#ifdef ANSI
PUBLIC S16 veSztS1SetReq
(
SpId spId,
U32  peerId
)
#else
PUBLIC S16 veSztS1SetReq(spId,peerId)
SpId spId;
U32  peerId;
#endif
{
   VeLiSapCb    *sap = NULLP;
   SztUDatEvnt  uDatEvnt;
   S16          ret = RFAILED;
   U16          retVal = 0;
   VeMmeCb      *mmeCb = NULLP;

   TRC2(veSztS1SetReq);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veSztS1SetReq:\
            Triggering S1 Setup Request \n"));
#endif

   VE_GET_NEXT_MME_CB(mmeCb);

   if(mmeCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,
                    "veSztS1SetReq : Not able to locate empty mmeCb\n"));
#endif /* DEBUGP */
      RETVALUE(RFAILED);
   }

   mmeCb->mmeId = (U16)peerId;

   sap = veCb.sztSap[0];

   uDatEvnt.transId.pres = PRSNT_NODEF;
   uDatEvnt.transId.val = 1;
   uDatEvnt.peerId.pres = PRSNT_NODEF;
   uDatEvnt.peerId.val = peerId;
  
   /* Build setup message */
   retVal = veUtlSztFillS1SetupReq(&(uDatEvnt.pdu));
  
   if(ROK != retVal)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSztS1SetReq : Failed to Fill veUtlSztFillS1SetupReq \n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Trigger UDat Req. */
   ret = VeLiSztUDatReq(&(sap->pst), spId, &uDatEvnt);

   RETVALUE(ret);
}

/*
*
*       Fun:   veLiSztDlNasHandler 
*
*       Desc:  Downlink NAS Handler
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_relay.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztDlNasHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztDlNasHandler(s1ConCb,pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{
   S16 ret = ROK;
   U16 retVal = 0;
   SztProtIE_Field_DlnkNASTport_IEs *ie = NULLP;
   SztDatEvntReq datEvt;

   TRC2(veLiSztDlNasHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veKDF: Key Derivation Function\n"));
#endif

   if(pdu->pdu.choice.val != SZT_TRGR_INTI_MSG)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztDlNasHandler: Invalid Pdu \n"));
#endif
      RETVALUE(RFAILED);
   }

   ie = &pdu->pdu.val.initiatingMsg.value.u.sztDlnkNASTport.\
        protocolIEs.member[2];

   if(ie->id.val != Sztid_NAS_PDU)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztDlNasHandler: NAS PDU doesnot exist \n"));
#endif
      RETVALUE(RFAILED);
   }
 
         /* ADD CRID:ccpu00118776 Stopping of Intial UE oriented connection Timer*/
         veUeConStopTmr ((PTR) s1ConCb->ueCb,VE_TMR_INITUE_CON);

   /* Fill RRC DL Information Transfer*/
   /* NAS NON Delivery Indication */
   ret = veSndRrcDlInfoTfr(s1ConCb->ueCb, &(ie->value.u.sztNAS_PDU));

   if(ret != ROK)
   {
      /* Trigger S1AP NAS NON DELIVERY INDICATION */
      VE_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));
      datEvt.spConnId = s1ConCb->spConnId;
      /* building S1AP message */
      retVal = veUtlSztFillNasNonDlvInd(s1ConCb, &pdu, &(ie->value.u.sztNAS_PDU));
      if(retVal != ROK)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztDlNasHandler : Failed to Fill veUtlSztFillNasNonDlvInd \n"));
#endif
         RETVALUE(RFAILED);
      } /* end of if part */
      /* Send the S1AP message to MME */
      VeLiSztDatReq(&veCb.sztSap[0]->pst,veCb.sztSap[0]->spId , &datEvt);
   }

   /* Free Pdu */
   cmFreeMem(pdu);

   RETVALUE(ROK);
} /* veLiSztDlNasHandler */
/*
*
*       Fun:   veLiSztOvldStartHandler
*
*       Desc:  MME Overload Start Handler
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_relay.c
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztOvldStartHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztOvldStartHandler(s1ConCb,pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{
   SztProtIE_Field_OverloadStartIEs *ie = NULLP;
   VeMmeCb *mmeCb = NULLP;

   TRC2(veLiSztOvldStartHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veKDF: Key Derivation Function\n"));
#endif
if(pdu->pdu.choice.val != SZT_TRGR_INTI_MSG)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztOvldStartHandler: Invalid Pdu \n"));
#endif
      RETVALUE(RFAILED);
   }

   ie = &pdu->pdu.val.initiatingMsg.value.u.sztOverloadStart.\
        protocolIEs.member[0];

   if(ie->id.val != Sztid_OverloadResp)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztOvldStartHandler: Overload Response doesnot exist \n"));
#endif
      RETVALUE(RFAILED);
   }
   VE_GET_MME_CB(s1ConCb->peerId, mmeCb);
   if(mmeCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztOvldStartHandler:\
               Failure in the allocation of configuration info\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

   mmeCb->mmeOvldAction = ie->value.u.sztOverloadResp.val.overloadAction.val;
   mmeCb->mmeOvldFlag = TRUE;

   RETVALUE(ROK);
} /* veLiSztOvldStartHandler */
/*
*
*       Fun:   veLiSztOvldStopHandler
*
*       Desc:  MME Overload Stop Handler
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_relay.c
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztOvldStopHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztOvldStopHandler(s1ConCb,pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{
   SztProtIE_Field_OverloadStopIEs *ie = NULLP;
   VeMmeCb *mmeCb = NULLP;

   TRC2(veLiSztOvldStopHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veKDF: Key Derivation Function\n"));
#endif

   if(pdu->pdu.choice.val != SZT_TRGR_INTI_MSG)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztOvldStopHandler: Invalid Pdu \n"));
#endif
      RETVALUE(RFAILED);
   }

   ie = &pdu->pdu.val.initiatingMsg.value.u.sztOverloadStop.\
        protocolIEs.member[0];

   VE_GET_MME_CB(s1ConCb->peerId, mmeCb);
   if(mmeCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztOvldStopHandler:\
               Failure in the allocation of configuration info\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */


   mmeCb->mmeOvldFlag = FALSE;

   RETVALUE(ROK);
} /* veLiSztOvldStopHandler */

/*
*
*       Fun:   veLiSztInitConSetHandler 
*
*       Desc:  Initial Context Setup 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_relay.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztInitConSetHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztInitConSetHandler(s1ConCb,pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{
   VeUeCb *ueCb = NULLP;
/* Ue Radio Capability handling */
#ifdef UE_RAD_CAP
   SztUERadioCapblty *ueRadCap;
   SztProtIE_Cont_InitCntxtSetupRqstIEs *protIes;
   U16  cNt;
#endif
   TRC2(veLiSztInitConSetHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztInitConSetHandler:\
            Triggering Initial Context Setup Message \n"));
#endif

   if(pdu->pdu.choice.val != SZT_TRGR_INTI_MSG)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztInitConSetHandler: Invalid PDU \n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Restore PDU, will be freed only after reconfig message is sent */
   ueCb = s1ConCb->ueCb;
/* RRC-ReEstab */
   ueCb->s1MsgType =  VE_ERAB_SETUP_REQ;
   ueCb->pdu = (S1apPdu *) pdu;
            /* ADD CRID:ccpu00118776 Stopping of Intial UE oriented connection Timer*/
            veUeConStopTmr ((PTR) ueCb,VE_TMR_INITUE_CON);
   /* Fetch the security related values */
   veFetchSecInfo(ueCb, pdu);
/* Ue Radio Capability handling */
#ifndef UE_RAD_CAP
#ifndef DISABLE_RRCSEC
   /* Build the message */
   veSndRrcSecModeCmd(ueCb, pdu);
#else
   /* Trigger RRC DL Connection Reconfiguration message*/
   veSndRrcRecfgReq(ueCb, Sztid_InitCntxtSetup);
#endif
/* Ue Radio Capability handling */
#else
   /* Build the message */
    protIes = &pdu->pdu.val.initiatingMsg.value.u.sztInitCntxtSetupRqst.protocolIEs; 
   for(cNt =0; cNt < protIes->noComp.val; cNt++)
   {
      if (protIes->member[cNt].pres.pres && 
                         (protIes->member[cNt].id.val == Sztid_UERadioCapblty))
      {
         ueRadCap = &protIes->member[cNt].value.u.sztUERadioCapblty;
         if(ueRadCap->pres)       
         {
            /* veSndRrcUeCapEnq(ueCb, pdu); */
            ueRadCap->len = (ueRadCap->len)/VE_BYTE_LEN; /*convert bits 
                                                     len to bytes len */
            veSndRrcUeCapDecReq(ueCb, ueRadCap, NHU_MSG_UERACAPINFO);
            RETVALUE(ROK);
         }
      }
   }
   /* if UeRa Capability is not Received , then Trigger UeCap Enq Proc */
   veSndRrcUeCapEnq(ueCb, pdu);
#endif
   RETVALUE(ROK);
} /* veLiSztInitConSetHandler */

/*
*
*       Fun:   veSndUeCntxModFail 
*
*       Desc:  Build and send the Context Modification
*              Failure
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_relay.c 
*
*/
#ifdef ANSI
PUBLIC S16 veSndUeCntxModFail
(
VeS1ConCb *s1ConCb
)
#else
PUBLIC S16 veSndUeCntxModFail(s1ConCb)
VeS1ConCb *s1ConCb;
#endif
{
   SztUDatEvnt      uDatReq;
   S16              ret;
   VeLiSapCb        *sap = NULLP;

   TRC2(veSndUeCntxModRsp);

   /* Build and send the UE context modification response to MME */
   ret = veUtlFillUeCntxModFail(&(uDatReq.pdu), s1ConCb->enb_ue_s1ap_id,
                                s1ConCb->mme_ue_s1ap_id,
                                SztCauseProtsemantic_errorEnum,
                                CAUSE_PROTOCOL);

   if(ret == ROK)
   {
      sap = veCb.sztSap[0];
      uDatReq.peerId.val = s1ConCb->mmeCb->mmeId;
      uDatReq.peerId.pres = PRSNT_NODEF;

      ret = VeLiSztUDatReq(&(sap->pst), sap->spId, &uDatReq);
   }

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,
                "veSndUeCntxModRsp: Failed to Send/Fill UE Cntx Mod Fail\n"));
#endif
      RETVALUE(RFAILED);
   }

   RETVALUE(ret);
}



/*
*
*       Fun:   veSndUeCntxModRsp 
*
*       Desc:  Build and send the Context Modification
*              Response
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_relay.c 
*
*/
#ifdef ANSI
PUBLIC S16 veSndUeCntxModRsp
(
VeS1ConCb *s1ConCb
)
#else
PUBLIC S16 veSndUeCntxModRsp(s1ConCb)
VeS1ConCb *s1ConCb;
#endif
{
   SztDatEvntReq    datReq;
   S16              ret;
   VeLiSapCb        *sap = NULLP;

   TRC2(veSndUeCntxModRsp);

   /* Build and send the UE context modification response to MME */
   ret = veUtlFillUeCntxModRsp(&(datReq.pdu), s1ConCb->enb_ue_s1ap_id,
                                s1ConCb->mme_ue_s1ap_id);

   if(ret == ROK)
   {
      sap = veCb.sztSap[0];
      datReq.spConnId = s1ConCb->spConnId;
      ret = VeLiSztDatReq(&(sap->pst), sap->spId, &datReq);
   }

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,
                "veSndUeCntxModRsp: Failed to Send/Fill UE Cntx Mod Rsp\n"));
#endif
      RETVALUE(RFAILED);
   }

   RETVALUE(ret);
}

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

   TRC2(veLiSztERABSetHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztERABSetHandler:\
            Triggering E-RAB Setup Request Message \n"));
#endif

   /* Restore PDU, will be freed only after reconfig message is sent */
   ueCb = s1ConCb->ueCb;
/* RRC-ReEstab */
   ueCb->s1MsgType =  VE_ERAB_SETUP_REQ;
   ueCb->pdu = (S1apPdu *) pdu;

    if(ueCb->pdu != NULLP)
   {
      /* Trigger RRC DL Connection Reconfiguration message*/
      veSndRrcRecfgReq(ueCb, Sztid_E_RABSetup);
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "\n veLiSztERABSetHandler(): S1AP doesnot exist for crnti (%d)\n", ueCb->crnti));
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
   VeUeCb *ueCb = NULLP; /* E-RAB Modify */ 

   TRC2(veLiSztERABModHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztERABModHandler:\
            Triggering E-RAB Modify Request Message \n"));
#endif
    /* E-RAB Modify */ 
   ueCb = s1ConCb->ueCb;

   ueCb->pdu = (S1apPdu *) pdu;
   if(ueCb->pdu != NULLP)
   {
      printf("\nE-RAB_MDFY_DBG[%s:%d]%s:\n",__FILE__, __LINE__, __FUNCTION__);
      /* Trigger RRC Connection Reconfiguration message*/
      if ( ROK != veSndRrcRecfgReq(ueCb, Sztid_E_RABMdfy))
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"%s: veSndRrcRecfgReq Failed\n",__FUNCTION__));
#endif
         printf("\nE-RAB_MDFY_DBG[%s:%d]%s: veSndRrcRecfgReq failed\n",__FILE__, __LINE__, __FUNCTION__);
         RETVALUE(RFAILED);
      }
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "\n veLiSztERABModHandler:(): S1AP doesnot exist for crnti (%d)\n", ueCb->crnti));
      RETVALUE(RFAILED);
   }
    /* E-RAB Modify End*/ 
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

   TRC2(veLiSztERABRelComHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztERABRelComHandler:\
            Triggering E-RAB Release Command Message \n"));
#endif

   /* Restore PDU, will be freed only after reconfig message is sent */
   ueCb = s1ConCb->ueCb;
/* RRC-ReEstab */
   ueCb->s1MsgType = VE_ERAB_RLS_COMM;
   ueCb->pdu = (S1apPdu *) pdu;

      if(ueCb->pdu != NULLP)
   {
      /* Trigger RRC DL Connection Reconfiguration message*/
      veSndRrcRecfgReq(ueCb, Sztid_E_RABRls);
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "\n veLiSztERABRelComHandler(): S1AP doesnot exist for crnti (%d)\n", ueCb->crnti));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veLiSztERABRelComHandler */

/*
*
*       Fun:   veSztNoOpr 
*
*       Desc:  No operation
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_relay.c 
*
*/
#ifdef ANSI
PUBLIC S16 veSztNoOpr
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veSztNoOpr(s1ConCb, pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{
   TRC2(veSztNoOpr); 

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veSztNoOpr: No operation \n"));
#endif
   
   /* Free PDU */
   cmFreeMem((Ptr)(pdu));
   RETVALUE(ROK);
} /* veSztNoOpr */

#ifdef LTE_HO_SUPPORT
/*
*
*       Fun:   veCztNoOpr 
*
*       Desc:  No operation
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_relay.c 
*
*/
#ifdef ANSI
PUBLIC S16 veCztNoOpr
(
VeX2ConCb *x2ConCb,
CztEvnt  *pdu
)
#else
PUBLIC S16 veCztNoOpr(x2ConCb, pdu)
VeX2ConCb *x2ConCb;
CztEvnt  *pdu;
#endif
{
   TRC2(veCztNoOpr); 

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veCztNoOpr: No operation \n"));
#endif
   
   /* Free PDU */
   cmFreeMem((Ptr)(pdu));
   RETVALUE(ROK);
} /* veSztNoOpr */
#endif
/*
*
*       Fun:   veLiSztUeRelCmdHandler 
*
*       Desc:  UE Release Command
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_relay.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztUeRelCmdHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztUeRelCmdHandler(s1ConCb, pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{
   SztRelRsp      relEvnt;
#ifdef LTE_HO_SUPPORT
   VeUeCb         *ueCb = NULLP;
   U8             ueIdx = 0;
#endif
   U16            retVal = 0;
   TRC2(veLiSztUeRelCmdHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztUeRelCmdHandler: \
            Triggering Ue Release Command Procedure \n"));
#endif

   if(pdu->pdu.choice.val != SZT_TRGR_INTI_MSG)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztUeRelCmdHandler: Invalid Pdu \n"));
#endif
      RETVALUE(RFAILED);
   }

#ifdef LTE_HO_SUPPORT
   ueCb = s1ConCb->ueCb;
   /* Stopping the S1 Overall Timer */
   veStopTmr((PTR)ueCb, VE_TMR_S1_OVRL_TMR);
   if(s1ConCb->ueCb->mobCtrlState == VE_HO_NORMAL)
   {
      /* Fill and Send RRC Connection Release*/
      veSndRrcConRel(ueCb);

      /* Send EGTP release command to EGTP-U*/
      veSndEgtpCom(VE_EGTP_TUN_DEL, s1ConCb);
   }
   else
   {
      veSndCtfUeRls(ueCb);
      /*RRC ReEst */
      veSndRgrUeRls(ueCb,NULLP,RGR_UE_CFG);
      /* request RRC to locally release the UE resources */
      VeLiNhuCnclUeReq(&(veCb.nhuSap[0]->pst), veCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);
 
      /* release x2 resources */
      if(ueCb->hoType == VE_HO_TYPE_X2)
      {
         veSndX2UeLocRlsReq(ueCb->x2ConCb);
      }

      ueIdx = VE_GET_UE_IDX(ueCb->crnti);
      if(ueIdx < veCb.cellCb[0]->maxUeSupp)
      {
        veCb.cellCb[0]->ueCbLst[ueIdx] = NULLP;
        veCb.cellCb[0]->numUeCfg--;
      }
      VE_FREE(ueCb,sizeof(VeUeCb));
   }
#else
   /* Fill and Send RRC Connection Release*/
   veSndRrcConRel(s1ConCb->ueCb);

   /* Send EGTP release command to EGTP-U*/
   veSndEgtpCom(VE_EGTP_TUN_DEL, s1ConCb);
#endif

   relEvnt.spConnId = s1ConCb->spConnId;

   /*Send S1AP Ue_Context_relase_compelete message*/
   retVal =  veUtlSztUeCntxtRelComp(s1ConCb,&(relEvnt.pdu));

   if(retVal != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF," veLiSztUeRelCmdHandler Failed to Fill veUtlSztUeCntxtRelComp \n"));
#endif
      RETVALUE(RFAILED);
   }
   printf("\n//////////////// Sending the UE Context Release Complete to MME /////////////\n");
   VeLiSztRelRsp(&veCb.sztSap[0]->pst, veCb.sztSap[0]->spId,&relEvnt);

   veCb.s1apConLst[s1ConCb->suConnId] = NULLP;
   VE_FREE(s1ConCb,sizeof(VeS1ConCb));
   /* Free PDU */
   cmFreeMem (pdu);

   RETVALUE(ROK);
} /* veLiSztUeRelCmdHandler */
/*
*
*       Fun:   veLiSztPagingHandler 
*
*       Desc:  UE Paging Handler
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_relay.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztPagingHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztPagingHandler(s1ConCb, pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{
   S16            ret = ROK;

   /* Corrected function name in trace macro */
   TRC2(veLiSztPagingHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztPagingHandler: \
            Triggering Paging Procedure \n"));
#endif

   if(pdu->pdu.choice.val != SZT_TRGR_INTI_MSG)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztPagingHandler: Invalid Pdu \n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Compute the various parameters including pf and po
    * and add the UE Paging Info into the Paging Pending
    * list */
    ret = veFillAndAddPagingInfo(pdu);
    if(ret != ROK)
    {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztPagingHandler: Failure in the filling of Paging Info\n"));
#endif
      RETVALUE(RFAILED);

    } /* end of if statement */
     
   /* Free PDU */
   cmFreeMem (pdu);

   RETVALUE(ROK);
} /* veLiSztPagingHandler */



/*
 * *
 * *       Fun:   VeSndRelReq
 * *
 * *       Desc:  Send Release Request
 * *
 * *       Ret:   ROK
 * *
 * *       Notes: None
 * *              
 * *       File:  ve_relay.c 
 * *
 * */
#ifdef ANSI
PUBLIC S16 VeSndRelReq
(
Pst            *pst,
U32            ueId
 )
#else
PUBLIC S16 VeSndRelReq(pst, ueId)
Pst            *pst;
U32            ueId;
#endif /* ANSI */
{

   VeS1ConCb *s1ConCb = NULLP;
   S1apPdu   *pdu = NULLP;
   VeUeCb    *ueCb = NULLP;
   VeCellCb *cellCb = NULLP;
   U16        ueIdx = 0;
   SztDatEvntReq  datEvt;
   
   TRC3(VeSndRelReq)

   cellCb = veCb.cellCb[0];

   if(cellCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeSndRelReq:\
                                   Locating CellCb failed.\n"));
#endif
      RETVALUE(RFAILED);
    } /* end of if statement */
   /* klock warning fix */
   ueIdx = (U16)VE_GET_UE_IDX(ueId);
   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
   } /* end of if statement */

   if(ueCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeSndRelReq:\
                                     Locating ueCb failed.\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

   s1ConCb = ueCb->s1ConCb;

   if(s1ConCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeSndRelReq:\
                               Locating S1ConCb failed.\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

   s1ConCb->cause.choice.val = CAUSE_RADIONW;
   s1ConCb->cause.val.radioNw.val = SztCauseRadioNwuser_inactivityEnum;

   /* Construct the S1AP UE Context Release towards MME */
   veUtlSztUeCntxtRel(s1ConCb, &pdu);

   VE_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));
   datEvt.spConnId = s1ConCb->spConnId;
   datEvt.pdu = pdu;

   /* Send the UE Context Release Request towards MME */
   VeLiSztDatReq(&veCb.sztSap[0]->pst,veCb.sztSap[0]->spId , &datEvt); 

   RETVALUE(ROK);
   
} /* end of VeSndRelReq */
#ifdef LTE_HO_SUPPORT
/*
 * 
 *        Fun:   veAdmnCntrlReq
 * 
 *        Desc:  Admission Control Request
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veAdmnCntrlReq
(
)
#else
PUBLIC S16 veAdmnCntrlReq()
#endif /* ANSI */
{
    
    /* From the E-RAB info present in UE Control block we can 
     * do the admission control. But currently we shall do admission
     * control based on the availability of the Dedicated CRNTI and Preambles
     * in the application */


  
   RETVALUE(ROK);

} /* end of veAdmnCntrlReq*/

#endif

/*
 * 
 *        Fun:   veHdlFullReset
 * 
 *        Desc:  Handle the Full reset request received
 * 
 *        Ret:   ROK
 * 
 *        Notes: 
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veHdlFullReset
(
U32     peerId
)
#else
PUBLIC S16 veHdlFullReset(peerId)
U32     peerId;
#endif /* ANSI */
{
   U32    ueIdx;
   U32    tmpMaxUeSupp;
   VeCellCb *cellCb = NULLP;
   VeUeCb *ueCb = NULLP;

   TRC3(veHdlFullReset);

   tmpMaxUeSupp = veCb.cellCb[0]->maxUeSupp;
   cellCb = veCb.cellCb[0];

   /* Go through all the connections and release all the connections */
   for(ueIdx = 0; ueIdx < tmpMaxUeSupp; ueIdx++)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
      if((ueCb != NULLP) && (ueCb->s1ConCb != NULLP) &&
         (ueCb->s1ConCb->mmeCb->mmeId == peerId))
      {
         /* Fill and Send RRC Connection Release*/
         veSndRrcConRel(ueCb);

         /* Send EGTP release command to EGTP-U*/
         veSndEgtpCom(VE_EGTP_TUN_DEL, ueCb->s1ConCb);
      }
   }

   RETVALUE(ROK);
} /* veHdlFullReset */

/*
 * 
 *        Fun:   veHdlPartReset
 * 
 *        Desc:  Handle the partial reset request received
 * 
 *        Ret:   ROK
 * 
 *        Notes: 
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veHdlPartReset
(
U32     peerId,
TknU8    *rstTypeIE
)
#else
PUBLIC S16 veHdlPartReset(peerId, rstTypeIE)
U32     peerId;
TknU8    *rstTypeIE;
#endif /* ANSI */
{
   U16    numCons;
   U16    idx;
   U32    ueRefNum;
   U32    ueIdx;
   U32    tmpMaxUeSupp;
   VeCellCb *cellCb = NULLP;
   VeUeCb *ueCb = NULLP;

   TRC3(veHdlPartReset);

   /* Get the number of connection present in the Reset message */
   numCons = ((SztResetTyp *)rstTypeIE)->val.partOfS1_Intf.noComp.val;

   tmpMaxUeSupp = veCb.cellCb[0]->maxUeSupp;
   cellCb = veCb.cellCb[0];

   /* Loop through all connection and release the same */
   for(idx = 0; idx < numCons; idx++)
   {
      if (((SztResetTyp *)rstTypeIE)->val.partOfS1_Intf.member[idx].\
            value.u.sztUE_assocLogS1_ConItem.eNB_UE_S1AP_ID.pres != NOTPRSNT)
      {
         /* Get the UE reference number and send the release request to RRC */
         ueRefNum = ((SztResetTyp *)rstTypeIE)->val.partOfS1_Intf.member[idx].\
                    value.u.sztUE_assocLogS1_ConItem.eNB_UE_S1AP_ID.val;

         /* Go through all the connections and match the spConnId */
         for(ueIdx = 0; ueIdx < tmpMaxUeSupp; ueIdx++)
         {
            ueCb = cellCb->ueCbLst[ueIdx];
            if((ueCb != NULLP) && (ueCb->s1ConCb != NULLP) &&
               (ueRefNum == ueCb->s1ConCb->enb_ue_s1ap_id) &&
               (ueCb->s1ConCb->mmeCb->mmeId == peerId))
            {
               /* Fill and Send RRC Connection Release*/
               veSndRrcConRel(ueCb);

               /* Send EGTP release command to EGTP-U*/
               veSndEgtpCom(VE_EGTP_TUN_DEL, ueCb->s1ConCb);
            }
         }
      }
      else if (((SztResetTyp *)rstTypeIE)->val.partOfS1_Intf.member[idx].\
            value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.pres != NOTPRSNT)
      {
         /* Get the UE reference number and send the release request to RRC */
         ueRefNum = ((SztResetTyp *)rstTypeIE)->val.partOfS1_Intf.member[idx].\
                    value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.val;

         /* Go through all the connections and match the spConnId */
         for(ueIdx = 0; ueIdx < tmpMaxUeSupp; ueIdx++)
         {
            ueCb = cellCb->ueCbLst[ueIdx];
            if((ueCb != NULLP) && (ueCb->s1ConCb != NULLP) &&
               (ueRefNum == ueCb->s1ConCb->mme_ue_s1ap_id) &&
               (ueCb->s1ConCb->mmeCb->mmeId == peerId))
            {
               /* Fill and Send RRC Connection Release*/
               veSndRrcConRel(ueCb);

               /* Send EGTP release command to EGTP-U*/
               veSndEgtpCom(VE_EGTP_TUN_DEL, ueCb->s1ConCb);
            }
         }
      }
   }

   RETVALUE(ROK);
} /* veHdlPartReset */


/*
 * 
 *        Fun:   veSztResetHdl
 * 
 *        Desc:  Handle the reset request received from
 *               MME
 * 
 *        Ret:   ROK
 * 
 *        Notes:
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veLiResetHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiResetHandler(s1ConCb, pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif /* ANSI */
{

   TknU8         *rstTypeIE = NULLP;        /* Reset type IE */
   U32            peerId;                   /* PeedId */
   S16            ret;                      /* Return value */
   SztUDatEvnt    uDatReq;                  /* Dat Request event struct */

   TRC3(veLiResetHandler);

   peerId = s1ConCb->peerId;

   /* Get the Reset Type IE from the S1AP PDU received */
   veSzGetIE(&(pdu->pdu), Sztid_ResetTyp, &rstTypeIE);

   /* Return the failure in case if we are not able to retrive the 
    * Reset type IE from S1AP PDU */
   if(rstTypeIE == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSztResetHdl:\
                               Locating Reset Type in S1-PDU failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Handle the reset based on the type of reset. Currently it is assumed
    * that all connections are releated to one peer */
   if (((SztResetTyp *)rstTypeIE)->choice.val == RESETTYP_S1_INTF)
   {
      veHdlFullReset(peerId);
   }
   else
   {
      veHdlPartReset(peerId, rstTypeIE);
   }

   VE_SET_ZERO(&uDatReq, sizeof(SztUDatEvnt));

   /* Fill and send the Reset ACK to peer. It is not required to wait till
    * the UE Context gets released before sending the Reset ACK */
   ret = szDbFillResetAck(pdu, &uDatReq.pdu, veCb.mem);

   if(ret == ROK)
   {
      uDatReq.peerId.val = peerId;
      uDatReq.peerId.pres = PRSNT_NODEF;

      /* Call the UDatReq funtion to send out the PDU */
      ret = VeLiSztUDatReq(&veCb.sztSap[0]->pst, veCb.sztSap[0]->spId,
                           &uDatReq);
      if(ret != ROK)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,
                  "veLiResetHandler(): Failed to Send Reset ACK to S1AP.\n"));
#endif
      }
   }

   RETVALUE(ret);
} /* veLiResetHandler */

/* UE Cntx Mod Start */
/*
 * 
 *        Fun:   veUpdSndUeCtxModInfo
 * 
 *        Desc:  Handles the UE Context Modification Request
 *               sent by the MME
 * 
 *        Ret:   ROK
 * 
 *        Notes: 
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veUpdSndUeCtxModInfo
(
VeUeCb   *ueCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veUpdSndUeCtxModInfo(ueCb, pdu)
VeUeCb   *ueCb;
S1apPdu  *pdu;
#endif /* ANSI */
{

   U8        needUpd;
   SztUECntxtModificationRqst *ueCntxtMod = NULLP;
   S16       idx;

   ueCntxtMod = &(pdu->pdu.val.initiatingMsg.value.u.\
                  sztUECntxtModificationRqst);

   veGetBitRateUeCntxtMod(ueCb, ueCntxtMod, &needUpd);

   if(needUpd == TRUE)
   {
      for(idx = 0; idx < ueCb->rbInfo.numOfRbCfgd && idx < VE_MAX_RABS_IN_UE;
          idx++)
      {
         if((ueCb->rbInfo.rab[idx].qci >= 5) && (ueCb->rbInfo.rab[idx].qci <= 9))
         {
            veSndAmbrRgrCfg(ueCb, &ueCb->rbInfo.rab[idx]);
         }
      }
   }

   RETVALUE(ROK);

} /* veUpdSndUeCtxModInfo */

/* UE Cntx Mod End */

/*
 * 
 *        Fun:   veLiSztUeCntxModHandler
 * 
 *        Desc:  Handles the UE Context Modification Request
 *               sent by the MME
 * 
 *        Ret:   ROK
 * 
 *        Notes: 
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veLiSztUeCntxModHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztUeCntxModHandler(s1ConCb, pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif /* ANSI */
{

   VeUeCb     *ueCb;
   TknU8      *csgFi = NULLP;
   TknU8      *secKey = NULLP;
   TknU8      *secCap = NULLP;

   TRC3(veLiSztUeCntxModHandler);

   ueCb = s1ConCb->ueCb;

   if(ueCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   veSzGetIE(&(pdu->pdu), Sztid_CSFallbackIndicator, &(csgFi));
   if(csgFi != NULLP)
   {
      veSzGetIE(&(pdu->pdu), Sztid_UESecurCapabilities, &(secCap));
      veSzGetIE(&(pdu->pdu), Sztid_SecurKey, &(secKey));

      if((secCap != NULLP) || (secKey != NULLP))
      {
         veSndUeCntxModFail(s1ConCb);
         RETVALUE(RFAILED);
      }
   }

   ueCb->pdu = pdu;

   /* Fetch the security related values */
   veFetchUeCntxModSecInfo(ueCb, pdu);

   veUpdSndUeCtxModInfo(ueCb, pdu);

   /* The response message will be sent after the RRC re-configuration 
    * confirmation is received */

   /* After updating the UE Context, send the response back to MME */
   veSndUeCntxModRsp(s1ConCb);

   RETVALUE(ROK);
} /* veLiSztUeCntxModHandler */

/*
 * 
 *        Fun:   veSztMmeCfgUpdHandler
 * 
 *        Desc:  Handles the MME Configuration Update Request
 *               sent by the MME
 * 
 *        Ret:   ROK
 * 
 *        Notes: 
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veGetPlmnVal
(
VePlmnId         *plmnId,
SztPLMNidentity  *ie
)
#else
PUBLIC S16 veGetPlmnVal(plmnId, ie)
VePlmnId         *plmnId;
SztPLMNidentity  *ie;
#endif /* ANSI */
{

   plmnId->mcc[0] = ie->val[0] & 0x0f;
   plmnId->mcc[1] = (ie->val[0] & 0xf0 >> 4);
   plmnId->mcc[2] = ie->val[1] & 0x0f;

   if((ie->val[1] & 0xf0) == 0xf0)
   {
      plmnId->numMncDigits = 2;
      plmnId->mnc[0] = ie->val[2] & 0xf0 >> 4;
      plmnId->mnc[1] = ie->val[2] & 0x0f;
   }
   else
   {
      plmnId->numMncDigits = 2;
      plmnId->mnc[0] = ie->val[1] & 0xf0 >> 4;
      plmnId->mnc[1] = ie->val[2] & 0x0f;
      plmnId->mnc[2] = ie->val[2] & 0xf0 >> 4;
   }

   RETVALUE(ROK);
}

/*
 * 
 *        Fun:   veAddGummei
 * 
 *        Desc:  Handles the MME Configuration Update Request
 *               sent by the MME
 * 
 *        Ret:   ROK
 * 
 *        Notes: 
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veAddGummei
(
U32      peerId,
SztProtIE_Field_S1SetupRespIEs  *ie
)
#else
PUBLIC S16 veAddGummei(peerId, ie)
U32      peerId;
SztProtIE_Field_S1SetupRespIEs  *ie;
#endif /* ANSI */
{
   SztSrvdGUMMEIs              *gummeis;
   SztSrvdGUMMEIsItem          *gummei;
   SztSrvdGroupIDs             *grpIds;
   SztSrvdMMECs                *mmeCs;
   SztSrvdPLMNs                *plmns;
   U16                         rcvdPlmns;
   U16                         rcvdGrpId;
   U16                         rcvdCodes;
   U16                         idx;
   VeMmeCb                     *mmeCb = NULLP;

   VE_GET_MME_CB(peerId, mmeCb);
   if(mmeCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veAddGummei:\
               Could not locate MmeCb\n"));
#endif
      RETVALUE(RFAILED);
   }

   gummeis = &ie->value.u.sztSrvdGUMMEIs;
   gummei = gummeis->member;
   grpIds = &gummei->servedGroupIDs;
   mmeCs = &gummei->servedMMECs;
   plmns = &gummei->servedPLMNs;
   rcvdPlmns = plmns->noComp.val; 

   if(rcvdPlmns > VE_MAX_PLMNS_PER_MME)
   {
      rcvdPlmns = VE_MAX_PLMNS_PER_MME;
   }

   for(idx = 0; idx < rcvdPlmns; idx++)
   {
      veGetPlmnVal(&(mmeCb->plmnIds[idx]), (plmns->member + idx));
   }

   mmeCb->numPlmnIds = (U8)rcvdPlmns;

   rcvdGrpId = grpIds->noComp.val;

   if(rcvdGrpId > VE_MAX_GRPS_PER_MME)
   {
      rcvdGrpId = VE_MAX_GRPS_PER_MME;
   }

   for(idx = 0; idx < rcvdGrpId; idx++)
   {
      mmeCb->groupIds[idx] = (((grpIds->member + idx)->val[0]) |
                              ((grpIds->member + idx)->val[1] << 8));
   }

   mmeCb->numGrpIds = rcvdGrpId;

   rcvdCodes = mmeCs->noComp.val;

   if(rcvdCodes > VE_MAX_CODES_PER_MME)
   {
      rcvdCodes = VE_MAX_CODES_PER_MME;
   }

   for(idx = 0; idx < rcvdCodes; idx++)
   {
      mmeCb->codes[idx] = (mmeCs->member + idx)->val[0];
   }

   mmeCb->numCodes = rcvdCodes;

   RETVALUE(ROK);
}

/*
 * 
 *        Fun:   veUpdateRelCap
 * 
 *        Desc:  Handles the MME Configuration Update Request
 *               sent by the MME
 * 
 *        Ret:   ROK
 * 
 *        Notes: 
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veUpdateRelCap
(
U32      peerId,
SztRelativeMMECapacity  *relCap
)
#else
PUBLIC S16 veUpdateRelCap(peerId, relCap)
U32      peerId;
SztRelativeMMECapacity  *relCap;
#endif /* ANSI */
{
   VeMmeCb    *mmeCb = NULLP;

   VE_GET_MME_CB(peerId, mmeCb);
   if(mmeCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUpdateRelCap:\
               Could not locate MmeCb\n"));
#endif
      RETVALUE(RFAILED);
   }

   mmeCb->relCapacity = (U8)relCap->val;

   RETVALUE(ROK);

}

/*
 * 
 *        Fun:   veSndMmeCfgUpdAck
 * 
 *        Desc:  Handles the MME Configuration Update Request
 *               sent by the MME
 * 
 *        Ret:   ROK
 * 
 *        Notes: 
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veSndMmeCfgUpdAck
(
U32      peerId
)
#else
PUBLIC S16 veSndMmeCfgUpdAck(peerId)
U32      peerId;
#endif /* ANSI */
{
   SztUDatEvnt    uDatReq;
   S16            ret;

   veSztFilMmeCfgUpdAck(&(uDatReq.pdu));

   uDatReq.transId.pres = PRSNT_NODEF;
   uDatReq.transId.val = 1;
   uDatReq.peerId.pres = PRSNT_NODEF;
   uDatReq.peerId.val = peerId;

   ret = VeLiSztUDatReq(&(veCb.sztSap[0]->pst), veCb.sztSap[0]->spId,
                        &(uDatReq));
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndMmeCfgUpdAck : Failed to Send Dat Req \n"));
#endif
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/*
 * 
 *        Fun:   veSetupResp
 * 
 *        Desc:  Handles the MME Configuration Update Request
 *               sent by the MME
 * 
 *        Ret:   ROK
 * 
 *        Notes: 
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veSetupResp
(
U32      peerId,
S1apPdu  *pdu
)
#else
PUBLIC S16 veSetupResp(peerId, pdu)
U32      peerId;
S1apPdu  *pdu;
#endif /* ANSI */
{
   SztSuccessfulOutcome    *succMsg;
   SztS1SetupResp          *setupRsp;
   SztProtIE_Field_S1SetupRespIEs *respIe;
   SztProtIE_Field_S1SetupRespIEs *tmpIe;
   U16               idx;

   succMsg  = &pdu->pdu.val.successfulOutcome;
   setupRsp = &succMsg->value.u.sztS1SetupResp;
   respIe = setupRsp->protocolIEs.member;

   for(idx = 0; idx < setupRsp->protocolIEs.noComp.val; idx++)
   {
      tmpIe = respIe + idx;
      switch(tmpIe->id.val)
      {
         case Sztid_RelativeMMECapacity:
            veUpdateRelCap(peerId, &tmpIe->value.u.sztRelativeMMECapacity);
            break;

         case Sztid_MMEname:
            /* We are not using this ie */
            break;

         case Sztid_SrvdGUMMEIs:
            veAddGummei(peerId, tmpIe);
            break;
      }
   }

   RETVALUE(ROK);
}

/*
 * 
 *        Fun:   veSztMmeCfgUpdHandler
 * 
 *        Desc:  Adds the GUMMEi information which are received in
 *               S1-Setup resp message
 * 
 *        Ret:   ROK
 * 
 *        Notes: 
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veUpdateGummei
(
U32      peerId,
SztProtIE_Field_MMEConfigUpdIEs *ie
)
#else
PUBLIC S16 veUpdateGummei(peerId, ie)
U32      peerId;
SztProtIE_Field_MMEConfigUpdIEs    *ie;
#endif /* ANSI */
{
   SztSrvdGUMMEIs              *gummeis;
   SztSrvdGUMMEIsItem          *gummei;
   SztSrvdGroupIDs             *grpIds;
   SztSrvdMMECs                *mmeCs;
   SztSrvdPLMNs                *plmns;
   U16                         rcvdPlmns = 0;
   U16                         rcvdGrpId;
   U16                         rcvdCodes;
   U16                         noPlmns;
   U16                         idx;
   VeMmeCb                     *mmeCb = NULLP;

   VE_GET_MME_CB(peerId, mmeCb);
   if(mmeCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUpdateGummei:\
               Could not locate MmeCb\n"));
#endif
      RETVALUE(RFAILED);
   }

   gummeis = &ie->value.u.sztSrvdGUMMEIs;
   gummei = gummeis->member;
   grpIds = &gummei->servedGroupIDs;
   mmeCs = &gummei->servedMMECs;
   plmns = &gummei->servedPLMNs;
   noPlmns = plmns->noComp.val;

   if(rcvdPlmns > VE_MAX_PLMNS_PER_MME)
   {
      rcvdPlmns = VE_MAX_PLMNS_PER_MME;
   }

   for(idx = 0; idx < rcvdPlmns; idx++)
   {
      veGetPlmnVal(&(mmeCb->plmnIds[idx]), (plmns->member + idx));
   }

   mmeCb->numPlmnIds = (U8)rcvdPlmns;

   rcvdGrpId = grpIds->noComp.val;

   if(rcvdGrpId > VE_MAX_GRPS_PER_MME)
   {
      rcvdGrpId = VE_MAX_GRPS_PER_MME;
   }

   for(idx = 0; idx < rcvdGrpId; idx++)
   {
      mmeCb->groupIds[idx] = (((grpIds->member + idx)->val[0]) |
                              ((grpIds->member + idx)->val[1] << 8));
   }

   mmeCb->numGrpIds = rcvdGrpId;

   rcvdCodes = mmeCs->noComp.val;

   if(rcvdCodes > VE_MAX_CODES_PER_MME)
   {
      rcvdCodes = VE_MAX_CODES_PER_MME;
   }

   for(idx = 0; idx < rcvdCodes; idx++)
   {
      mmeCb->codes[idx] = (mmeCs->member + idx)->val[0];
   }

   mmeCb->numCodes = rcvdCodes;

   RETVALUE(ROK);
}

/*
 * 
 *        Fun:   veSztMmeCfgUpdHandler
 * 
 *        Desc:  Handles the MME Configuration Update Request
 *               sent by the MME
 * 
 *        Ret:   ROK
 * 
 *        Notes: 
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veSztMmeCfgUpdHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veSztMmeCfgUpdHandler(s1ConCb, pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif /* ANSI */
{
   U32                 idx;
   SztInitiatingMsg    *initMsg = &pdu->pdu.val.initiatingMsg;
   SztMMEConfigUpd     *cfgUpd  = &initMsg->value.u.sztMMEConfigUpd;
   SztProtIE_Field_MMEConfigUpdIEs *ie;

   TRC3(veSztMmeCfgUpdHandler);

   for(idx = 0; idx < cfgUpd->protocolIEs.noComp.val; idx++)
   {
      ie = cfgUpd->protocolIEs.member + idx;

      switch(ie->id.val)
      {
         case Sztid_MMEname:
            /* We are not using this IE */
            break;

         case Sztid_GUMMEI_ID:
            veUpdateGummei(s1ConCb->peerId, ie);
            break;

         case Sztid_RelativeMMECapacity:
            veUpdateRelCap(s1ConCb->peerId, &ie->value.u.sztRelativeMMECapacity);
            break;
      }
   }

   veSndMmeCfgUpdAck(s1ConCb->peerId);

   RETVALUE(ROK);
} /* veLiSztUeCntxModHandler */

/*
 * 
 *        Fun:   veLiSztHdlEnbCfgUpdAck
 * 
 *        Desc:  Handles the eNodeB cfg Update ACK
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veLiSztHdlEnbCfgUpdAck
(
VeS1ConCb *s1ConCb,
S1apPdu   *pdu
)
#else
PUBLIC S16 veLiSztHdlEnbCfgUpdAck(s1ConCb, pdu)
VeS1ConCb *s1ConCb;
S1apPdu   *pdu;
#endif /* ANSI */
{
   VeMmeCb     *mmeCb = NULLP;
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf,
               "veLiSztHdlEnbCfgUpdAck: eNB Cfg Update Successful\n"));
#endif
   /* Stop the timer of eNB Cfg update */
   VE_GET_MME_CB(s1ConCb->peerId, mmeCb);

   if(mmeCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztHdlEnbCfgUpdAck:\
               Could not locate MmeCb\n"));
#endif
      RETVALUE(RFAILED);
   }

   veStopTmr((PTR)mmeCb, VE_ENB_CFG_UPD_TMR);

   RETVALUE(ROK);
}

/*
 * 
 *        Fun:   veLiSztHdlEnbCfgUpdFail
 * 
 *        Desc:  Handles the eNodeB update failure
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veLiSztHdlEnbCfgUpdFail
(
VeS1ConCb *s1ConCb,
S1apPdu   *pdu
)
#else
PUBLIC S16 veLiSztHdlEnbCfgUpdFail(s1ConCb, pdu)
VeS1ConCb *s1ConCb;
S1apPdu   *pdu;
#endif /* ANSI */
{
   VeMmeCb     *mmeCb = NULLP;
   TknU32      *timeToWait = NULLP;
   U16         delay = 0;

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf,
               "veLiSztHdlEnbCfgUpdAck: eNB Cfg Update Successful\n"));
#endif

   /* Stop the timer of eNB Cfg update */
   VE_GET_MME_CB(s1ConCb->peerId, mmeCb);

   if(mmeCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiSztUDatInd:\
               Could not locate MmeCb\n"));
#endif
      RETVALUE(RFAILED);
   }

   veStopTmr((PTR)mmeCb, VE_ENB_CFG_UPD_TMR);

   veSzGetIE(&(pdu->pdu), Sztid_TimeToWait, (TknU8**)&(timeToWait));

   /* Start the timer for the value which sent by the peer */
   if(timeToWait != NULLP)
   {
      mmeCb->cfgUpdRtxCnt = 0;
      VE_GET_TIME_TO_WAIT_VAL(timeToWait->val, delay);
      veStartTmr((PTR)mmeCb, VE_ENB_CFG_UPD_TMR, delay);
   }
   else
   {
      veSendLmAlarm(LCM_CATEGORY_PROTOCOL, LVE_EVENT_ENB_UPD_FAIL,
                    LCM_CAUSE_UNKNOWN);
   }

   RETVALUE(ROK);
}

/*
 * 
 *        Fun:   veSndEnbCfgUpd
 * 
 *        Desc:  Builds and sends the eNodeB cfg updates
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veSndEnbCfgUpd
(
 Void
)
#else
PUBLIC S16 veSndEnbCfgUpd()
#endif /* ANSI */
{
   SztUDatEvnt    uDatReq;
   U16            idx;
   VeMmeCb        *mmeCb;
   S16            ret;

   for(idx = 0; idx < VE_MAX_MMES; idx++)
   {
      mmeCb = veCb.mmeCont.mmes[idx];

      if((mmeCb != NULLP) && (mmeCb->state == VE_S1_SETUP_DONE))
      {
         ret = veUtlSztFillEnbCfgUpd(&(uDatReq.pdu));
         if(ret != ROK)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,
                         "veSndEnbCfgUpd: Failed to Fill eNbCfg Update\n"));
#endif
            RETVALUE(RFAILED);
         }

         uDatReq.peerId.pres = PRSNT_NODEF;
         uDatReq.peerId.val = mmeCb->mmeId; 

         ret = VeLiSztUDatReq(&(veCb.sztSap[0]->pst), veCb.sztSap[0]->spId,
                              &(uDatReq));
         if(ret != ROK)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,
                          "veSndEnbCfgUpd: Failed to Send eNbCfg Upd\n"));
#endif
            RETVALUE(RFAILED);
         }

         /* Start the timer for the default case for this message */
         cmInitTimers(&(mmeCb->enbUpdTmr), 1);
         veStartTmr((PTR)mmeCb, VE_ENB_CFG_UPD_TMR, VE_DFLT_CFG_UPD_TMR_VAL);
      }
   }

   RETVALUE(ROK);
}
 
/*
 * 
 *        Fun:   veHdlEnbCfgUpdTimOut
 * 
 *        Desc:  Resends the eNB cfg Update to peer
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veHdlEnbCfgUpdTimOut
(
VeMmeCb   *mmeCb
)
#else
PUBLIC S16 veHdlEnbCfgUpdTimOut(mmeCb)
VeMmeCb   *mmeCb;
#endif /* ANSI */
{
   SztUDatEvnt    uDatReq;
   S16            ret;

   if((mmeCb != NULLP) && (mmeCb->state == VE_S1_SETUP_DONE))
   {
      if(mmeCb->cfgUpdRtxCnt >= VE_MAX_ENB_CFG_UPD_RTX)
      {
         veSendLmAlarm(LCM_CATEGORY_PROTOCOL, LVE_EVENT_ENB_UPD_FAIL,
                   LCM_CAUSE_UNKNOWN);
         RETVALUE(ROK);
      }

      ret = veUtlSztFillEnbCfgUpd(&(uDatReq.pdu));
      if(ret != ROK)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,
                  "veSndEnbCfgUpd: Failed to Fill eNbCfg Update\n"));
#endif
         RETVALUE(RFAILED);
      }

      uDatReq.peerId.pres = PRSNT_NODEF;
      uDatReq.peerId.val = mmeCb->mmeId; 

      ret = VeLiSztUDatReq(&(veCb.sztSap[0]->pst), veCb.sztSap[0]->spId,
            &(uDatReq));
      if(ret != ROK)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,
                  "veSndEnbCfgUpd: Failed to Send eNbCfg Upd\n"));
#endif
         RETVALUE(RFAILED);
      }

      mmeCb->cfgUpdRtxCnt++;
      /* Start the timer for the default case for this message */
      veStartTmr((PTR)mmeCb, VE_ENB_CFG_UPD_TMR, VE_DFLT_CFG_UPD_TMR_VAL);
   }

   RETVALUE(ROK);
}

/*
 * 
 *        Fun:   veHdlEnbReset
 * 
 *        Desc:  Resends the eNB reset to peer
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veHdlEnbReset
(
U32       peerId,
U8        choice,
U32       cause,
U16       numCons,
U8        rstType,
VeS1ConId *conIds
)
#else
PUBLIC S16 veHdlEnbReset(peerId, choice, cause, numCons, rstType, conIds)
U32       peerId;
U8        choice;
U32       cause;
U16       numCons;
U8        rstType;
VeS1ConId *conIds;
#endif /* ANSI */
{
   SztUDatEvnt      uDatReq;
   S16              ret;
   VeMmeCb          *mmeCb = NULLP;
   U32              tmrVal;
   TknU8            *rstTypeIE;

   VE_GET_MME_CB(peerId, mmeCb);

   if(mmeCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veHdlEnbReset: Unable to find MME cb \n"));
#endif
      RETVALUE(RFAILED);
   }

   uDatReq.peerId.pres = PRSNT_NODEF;
   uDatReq.peerId.val  = peerId;

   /* Fill the Reset pdu and store it in the mmeCb which will be used for 
    * retransmission of the reset */
   ret = veUtlFillReset(&(mmeCb->rstPdu), rstType, numCons, choice, cause, conIds);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veHdlEnbReset: Failed to Build Reset\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Make a copy of the Reset Pdu and send it to peer */
   ret = szDbFillResetFrmPdu(mmeCb->rstPdu, &(uDatReq.pdu), veCb.mem);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veHdlEnbReset: Failed to Make copy of Reset\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Release the connection which are mentioned */
   if(rstType == RESETTYP_S1_INTF)
   {
      veHdlFullReset(peerId);
   }
   else
   {
      veSzGetIE(&(uDatReq.pdu->pdu), Sztid_ResetTyp, &rstTypeIE);
      if(rstTypeIE == NULLP)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,
                  "veHdlEnbReset: rstTypeIE is Null \n"));
#endif
         RETVALUE(RFAILED);
      }

      veHdlPartReset(peerId, rstTypeIE);
   }

   ret = VeLiSztUDatReq(&(veCb.sztSap[0]->pst), veCb.sztSap[0]->spId,
                        &(uDatReq));

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veHdlEnbReset: Failed to Send Reset\n"));
#endif
      RETVALUE(RFAILED);
   }

   tmrVal = VE_S1_RESET_TMR_VAL;
   veStartTmr((PTR)mmeCb, (S16)VE_S1_RESET_TMR, tmrVal);

   RETVALUE(ROK);
}
/*
 * 
 *        Fun:   veHdlEnbRsetTimOut
 * 
 *        Desc:  Resends the Reset to peer
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veHdlEnbRsetTimOut
(
VeMmeCb   *mmeCb
)
#else
PUBLIC S16 veHdlEnbRsetTimOut(mmeCb)
VeMmeCb   *mmeCb;
#endif /* ANSI */
{
   SztUDatEvnt      uDatReq;
   S16              ret;

   if(mmeCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veHdlEnbRsetTimOut: Unable to find MME cb \n"));
#endif
      RETVALUE(RFAILED);
   }

   if(mmeCb->s1ResetRtxCnt > VE_RESET_RTX_VAL)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veHdlEnbRsetTimOut: Max retry reached for reset \n"));
#endif
      RETVALUE(ROK);
   }

   uDatReq.peerId.pres = PRSNT_NODEF;
   uDatReq.peerId.val  = mmeCb->mmeId;

   /* Make a copy of the Reset Pdu and send it to peer */
   ret = szDbFillResetFrmPdu(mmeCb->rstPdu, &(uDatReq.pdu), veCb.mem);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veHdlEnbReset: Failed to Make copy of Reset\n"));
#endif
      RETVALUE(RFAILED);
   }

   ret = VeLiSztUDatReq(&(veCb.sztSap[0]->pst), veCb.sztSap[0]->spId,
                        &(uDatReq));

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veHdlEnbReset: Failed to Send Reset\n"));
#endif
      RETVALUE(RFAILED);
   }

   veStartTmr((PTR)mmeCb, VE_S1_RESET_TMR, VE_S1_RESET_TMR_VAL);

   mmeCb->s1ResetRtxCnt++;

   RETVALUE(ROK);
}

/*
 * 
 *        Fun:   veLiResetAckHandler
 * 
 *        Desc:  Handles the Reset ACK from MME
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veLiResetAckHandler
(
VeS1ConCb *s1ConCb,
S1apPdu   *pdu
)
#else
PUBLIC S16 veLiResetAckHandler(s1ConCb, pdu)
VeS1ConCb *s1ConCb;
S1apPdu   *pdu;
#endif /* ANSI */
{
   VeMmeCb     *mmeCb = NULLP;

   VE_GET_MME_CB(s1ConCb->peerId, mmeCb);

   if(mmeCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   veStopTmr((PTR)mmeCb, VE_S1_RESET_TMR);

   RETVALUE(ROK);
}

/*
 * 
 *        Fun:   veLiSztErrIndHdl
 * 
 *        Desc:  Handles the Error Indication from MME
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  ve_relay.c 
 * 
 */
#ifdef ANSI
PUBLIC S16 veLiSztErrIndHdl
(
VeS1ConCb *s1ConCb,
S1apPdu   *pdu
)
#else
PUBLIC S16 veLiSztErrIndHdl(s1ConCb, pdu)
VeS1ConCb *s1ConCb;
S1apPdu   *pdu;
#endif /* ANSI */
{
 
   TknU8           *ie = NULLP;
   SztCause        *cause = NULLP;

   TRC2(veLiSztErrIndHdl);

   veSzGetIE(&(pdu->pdu), Sztid_Cause, &ie);

   if(ie == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztErrIndHdl: Failed to retrive cause \n"));
#endif
      RETVALUE(RFAILED);
   }

   cause = (SztCause *)ie;

   veSendLmAlarm(LCM_CATEGORY_PROTOCOL, LVE_EVENT_ERR_IND_RCVD,
                 cause->val.radioNw.val);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztErrIndHdl:\
                   Received error indication choice (%d) and cause (%ld)\n",
                   cause->choice.val, cause->val.radioNw.val));
#endif /* DEBUGP */

   RETVALUE(ROK);
}
#endif

/********************************************************************30**

         End of file:     ve_relay.c@@/main/2 - Wed Dec  1 08:44:28 2010

*********************************************************************31*/


/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/

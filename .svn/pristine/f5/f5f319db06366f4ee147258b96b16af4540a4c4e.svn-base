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
  
     Desc:     Handover Module 

     Ret :     ROK - success
               RFAILED - failure

     File:     ve_ho.c

     Sid:      ve_x2_ho.c@@/main/eNB_1.3_With_LatestGpr3/11 - Mon Sep 19 16:35:17 2011

     Prg:  

**********************************************************************/
#ifdef LTE_HO_SUPPORT

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

#include "czt.h"           /* X2AP RRM control Interface */
#include "czt_asn.h"       /* X2AP RRM control Interface */

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
#include "szt_asn.x"

#include "czt.x"           /* X2AP RRM control Interface */
#include "czt_asn.x"       /* X2AP RRM control Interface */

/* RRM related includes */
#include "egt.x"
#include "ve_eut.x"
#include "ctf.x"
#include "lve.x" 
#include "ve.x" 
#ifndef MSPD
#include "ve_dflt.h"
#else
#include "ve_dflt_ms.x"
#endif

/* eNodeB Cb */


PRIVATE S16 veHoSndX2HORqst ARGS((VeX2ConCb *x2ConCb, void * ptr));
PRIVATE S16 veHoPrcX2HOPrepFailure ARGS((VeX2ConCb *x2ConCb, void * ptr));
PRIVATE S16 veHoPrcX2HOPrepTmrExpiry ARGS((VeX2ConCb *x2ConCb, void * ptr));
PRIVATE S16 veHoPrcX2HOAck ARGS((VeX2ConCb *x2ConCb, void * ptr));
PRIVATE S16 veHoPrcX2UeCtxtRls ARGS((VeX2ConCb *x2ConCb, void * ptr));
PRIVATE S16 veHoPrcX2OvrAllTmrExpiry ARGS((VeX2ConCb *x2ConCb, void * ptr));

PRIVATE S16 veHoPrcX2Cancel ARGS((VeX2ConCb *x2ConCb, void * ptr));
PRIVATE S16 veHoSndX2HOAck ARGS((VeX2ConCb *x2ConCb, void * ptr));
PRIVATE S16 veHoSndX2HOPrepFailure ARGS ((VeX2ConCb *x2ConCb, void * ptr));
PRIVATE S16 veX2HoPrcUeHoAttachTmrExpiry ARGS ((VeX2ConCb *x2ConCb, void * ptr));
PRIVATE S16 veHoX2NoOpr ARGS ((VeX2ConCb *x2ConCb, void * ptr));
PRIVATE S16 veX2HoProcHoCfm ARGS ((VeX2ConCb *x2ConCb, void * ptr));

/* X2 Handover State machine  for Source enb */
PUBLIC  VeX2SrcFSM veX2SrcFSM[VE_HO_MAX_STATES][VE_HO_SRC_MAX_EVENTS]=
{
   /*VE_HO_NORMAL */
   { 
      veHoSndX2HORqst,         /* VE_EVNT_HO_DECISION             */
      veHoX2NoOpr,             /* VE_EVNT_HO_PREP_FAILURE         */
      veHoX2NoOpr,             /* VE_EVNT_HO_PREP_TMR_EXPIRY      */
      veHoX2NoOpr,             /* VE_EVNT_HO_ACK_RCVD             */
      veHoX2NoOpr,             /* VE_EVNT_HO_UE_CNTXT_RLS         */
      veHoX2NoOpr              /* VE_EVNT_HO_OVERALL_TMR_EXPIRY   */
  },

  /* VE_HO_INITIATED */
   { 
      veHoX2NoOpr,             /* VE_EVNT_HO_DECISION             */ 
      veHoPrcX2HOPrepFailure,  /* VE_EVNT_HO_PREP_FAILURE         */
      veHoPrcX2HOPrepTmrExpiry,/* VE_EVNT_HO_PREP_TMR_EXPIRY      */
      veHoPrcX2HOAck,          /* VE_EVNT_HO_ACK_RCVD             */
      veHoX2NoOpr,             /* VE_EVNT_HO_UE_CNTXT_RLS         */
      veHoX2NoOpr              /* VE_EVNT_HO_OVERALL_TMR_EXPIRY   */
  },

   /* VE_HO_IN_PROGRESS */
   {
      veHoX2NoOpr,              /* VE_EVNT_HO_DECISION             */
      veHoX2NoOpr,              /* VE_EVNT_HO_PREP_FAILURE         */
      veHoX2NoOpr,              /* VE_EVNT_HO_PREP_TMR_EXPIRY      */
      veHoX2NoOpr,              /* VE_EVNT_HO_ACK_RCVD             */
      veHoPrcX2UeCtxtRls,       /* VE_EVNT_HO_UE_CNTXT_RLS         */
      veHoPrcX2OvrAllTmrExpiry  /* VE_EVNT_HO_OVERALL_TMR_EXPIRY   */
  },
   /* VE_HO_UE_RELEASE*/
   { 
      veHoX2NoOpr,          /* VE_EVNT_HO_DECISION             */
      veHoX2NoOpr,          /* VE_EVNT_HO_PREP_FAILURE         */
      veHoX2NoOpr,          /* VE_EVNT_HO_PREP_TMR_EXPIRY      */
      veHoX2NoOpr,          /* VE_EVNT_HO_ACK_RCVD             */
      veHoX2NoOpr,          /* VE_EVNT_HO_UE_CNTXT_RLS         */ 
      veHoX2NoOpr           /* VE_EVNT_HO_OVERALL_TMR_EXPIRY   */
  }
};

/* X2 Handover State machine for target enb*/
PUBLIC  VeX2TgtFSM veX2TgtFSM[VE_HO_MAX_STATES][VE_HO_TGT_MAX_EVENTS]=
{
   /*VE_HO_NORMAL */
   { 
      veHoX2NoOpr,     /* VE_EVNT_HO_PREP_SUCCESS         */
      veHoX2NoOpr,     /* VE_EVNT_HO_ADM_FAILURE          */
      veHoX2NoOpr,     /* VE_EVNT_HO_RECONFIG_CMPL_RCVD   */
      veHoX2NoOpr,     /* VE_EVNT_HO_PATH_SWTH_FAILED     */
      veHoX2NoOpr,     /* VE_EVNT_HO_UE_ATTACH_TMR_EXPIRY */
      veHoX2NoOpr      /* VE_EVNT_HO_CANCEL_RCVD          */
  },

  /* VE_HO_INITIATED */
   { 
      veHoSndX2HOAck,          /* VE_EVNT_HO_PREP_SUCCESS         */
      veHoSndX2HOPrepFailure,  /* VE_EVNT_HO_ADM_FAILURE          */
      veHoX2NoOpr,             /* VE_EVNT_HO_RECONFIG_CMPL_RCVD   */
      veHoX2NoOpr,             /* VE_EVNT_HO_PATH_SWTH_FAILED     */
      veHoX2NoOpr,             /* VE_EVNT_HO_UE_ATTACH_TMR_EXPIRY */
      veHoPrcX2Cancel          /* VE_EVNT_HO_CANCEL_RCVD          */
  },

   /* VE_HO_IN_PROGRESS */
   { 
      veHoX2NoOpr,                  /* VE_EVNT_HO_PREP_SUCCESS         */
      veHoX2NoOpr,                  /* VE_EVNT_HO_ADM_FAILURE          */
      veX2HoProcHoCfm,              /* VE_EVNT_HO_RECONFIG_CMPL_RCVD   */ 
      veHoX2NoOpr,                  /* VE_EVNT_HO_PATH_SWTH_FAILED     */
      veX2HoPrcUeHoAttachTmrExpiry, /* VE_EVNT_HO_UE_ATTACH_TMR_EXPIRY */ 
      veHoPrcX2Cancel               /* VE_EVNT_HO_CANCEL_RCVD          */
  },
  /* VE_HO_UE_RELEASE*/
   { 
      veHoX2NoOpr,     /* VE_EVNT_HO_PREP_SUCCESS         */
      veHoX2NoOpr,     /* VE_EVNT_HO_ADM_FAILURE          */
      veHoX2NoOpr,     /* VE_EVNT_HO_RECONFIG_CMPL_RCVD   */
      veHoX2NoOpr,     /* VE_EVNT_HO_PATH_SWTH_FAILED     */
      veHoX2NoOpr,     /* VE_EVNT_HO_UE_ATTACH_TMR_EXPIRY */
      veHoX2NoOpr      /* VE_EVNT_HO_CANCEL_RCVD          */
  }
};

/*
 *
 *      Fun:   veHoX2NoOpr
 *
 *      Desc: 
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_x2_ho.c
 *
 */
#ifdef ANSI
PRIVATE S16 veHoX2NoOpr
(
VeX2ConCb   *x2ConCb,
void        *ptr
)
#else
PRIVATE S16 veHoX2NoOpr(x2ConCb, ptr)
VeX2ConCb   *x2ConCb;
void        *ptr;
#endif
{

   TRC2(veHoX2NoOpr);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (VE_PRNT_BUF, \
            "veHoX2NoOpr: Invalid X2 msg for UE ID: %d.\n",\
            x2ConCb->ueCb->crnti));
#endif

   RETVALUE(ROK);
}/* veHoX2NoOpr */

/*
*
*       Fun:   veCztX2SetReq 
*
*       Desc:  X2 Setup Request 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_relay.c 
*
*/
#ifdef ANSI
PUBLIC S16 veCztX2SetupReq
(
U32 peerId
)
#else
PUBLIC S16 veCztX2SetupReq(peerId)
U32 peerId;
#endif
{
   CztEvnt *x2SetReq = NULLP;

   U16      retVal = 0;

   TRC2(veCztX2SetupReq);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veCztX2SetupReq:\
            Triggering X2 Setup Request \n"));
#endif

   /* Build setup message */
   retVal = veUtlCztFillX2SetupReq(&x2SetReq);
 
   if(ROK != retVal)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillX2SetupReq:\
                                                       Failed to Fill veUtlCztFillX2SetupReq \n"));
#endif
      RETVALUE(RFAILED);
   }
   x2SetReq->peerId = peerId;

   /* Trigger corresponding primitive . */
   RETVALUE(VeLiCztGpReq(&(veCb.cztSap[0]->pst), veCb.cztSap[0]->spId, x2SetReq));
} /* end of veCztX2SetReq */


/*
*
*       Fun:   veHoSndX2HORqst
*
*       Desc:  Send X2 Handover Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoSndX2HORqst
(
VeX2ConCb  *x2ConCb,
void       *nghInfo
)
#else
PRIVATE S16 veHoSndX2HORqst(x2ConCb, nghInfo)
VeX2ConCb  *x2ConCb;
void       *nghInfo;
#endif
{
   CztEvnt       *hoReqPdu = NULLP;
   VeUeCb        *ueCb = NULLP;
   VeNghCellCb   *nghCellCb = (VeNghCellCb *)nghInfo;

   /* Get the UE Context.*/
   ueCb =  x2ConCb->ueCb;
   /* To trace the Procedure.*/
   TRC2(veHoSndX2HORqst);

   /* Sanity Check */
   if(NULLP == nghCellCb)
   {
#ifdef DEBUGP
      VE_LYR_DBGP(DBGMASK_SM, (VE_PRNT_BUF, "veHoSndX2HORqst : Neighbor Info empty\n"));
#endif
      RETVALUE(RFAILED);
   }

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (VE_PRNT_BUF, "veHoSndX2HORqst : Triggering sending of X2 HO Req\n"));
#endif

   /* Fill the X2AP HO Req Pdu and send to T-eNB */
   if(RFAILED == veUtlCztFillHovrRqst (&hoReqPdu, nghCellCb, x2ConCb,ueCb->trnspContainer))
   {
      RETVALUE(RFAILED);
   }
   /* filling the peerId */
   hoReqPdu->peerId = x2ConCb->peerId;
   x2ConCb->hoNodeType = SRC_ENODEB;
   RETVALUE(VeLiCztBmpReq (&veCb.cztSap[0]->pst, veCb.cztSap[0]->spId, hoReqPdu));

} /* veHoSndX2HORqst */


/*
*
*       Fun:   veHoPrcX2HOPrepFailure
*
*       Desc:  Process X2 Handover Preparation Failure
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoPrcX2HOPrepFailure
(
VeX2ConCb *x2ConCb,
void      *ptr
)
#else
PRIVATE S16 veHoPrcX2HOPrepFailure(x2ConCb,ptr)
VeX2ConCb *x2ConCb;
void      *ptr;
#endif
{
   CztEvnt *hoPrepFlr = (CztEvnt *) ptr;

   TRC2(veHoPrcX2HOPrepFailure);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoPrcS1HOPrepFailure:\
                  Triggering X2 HO Prep failure\n"));
#endif

   if(NULLP == hoPrepFlr)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcS1HOPrepFailure:\
               Invalid Arguments\n"));
      RETVALUE(RFAILED);
   }
   /* update the candidate cell list for the ue */
   /* for current release, HO preparation with the next node is not tried */
   veClrCandLst(&x2ConCb->ueCb->candCellList);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoPrcS1HOPrepFailure:\
                  HO Prep failure at target\n"));
#endif

   x2ConCb->ueCb->mobCtrlState = VE_HO_NORMAL;

   veCb.x2apConLst[VE_GET_UE_X2_IDX(x2ConCb->oldEnbUeX2apId)] = NULLP;


   RETVALUE(ROK);
} /* veHoPrcX2HOPrepFailure */

/*
*
*       Fun:   veHoPrcX2HOPrepTmrExpiry
*
*       Desc:  Process X2 Handover Preparation Timer Expiry
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoPrcX2HOPrepTmrExpiry
(
VeX2ConCb *x2ConCb,
void      *ptr
)
#else
PRIVATE S16 veHoPrcX2HOPrepTmrExpiry(x2ConCb, ptr)
VeX2ConCb *x2ConCb;
void      *ptr;
#endif
{
   TRC2(veHoPrcX2HOPrepTmrExpiry);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoPrcX2HOPrepTmrExpiry:\
                  Triggering X2 HO Prep timer expiry\n"));
#endif

   /* Relese the references and resorces made for the concerned Ue 
      X2 associated signaling */
   /* for current release, HO preparation with the next node is not tried
      So all the candidate lista are deleted here */
   veClrCandLst(&x2ConCb->ueCb->candCellList);


   x2ConCb->ueCb->mobCtrlState = VE_HO_NORMAL;

   RETVALUE(ROK);
} /* veHoPrcX2HOPrepTmrExpiry */

/*
*
*       Fun:   veHoPrcX2HOAck
*
*       Desc:  Process X2 Handover Acknowledgement received from target enodeb
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoPrcX2HOAck
(
VeX2ConCb *x2ConCb,
void      *hoAckPdu
)
#else
PRIVATE S16 veHoPrcX2HOAck(x2ConCb, hoAckPdu)
VeX2ConCb *x2ConCb;
void      *hoAckPdu;
#endif
{
   VeUeCb   *ueCb = NULLP;
   NhuDecodeSduReq         *nhuDecodeSduReq = NULLP;
   Buffer                  *mBuf = NULLP;
   CztEvnt *cztInd = NULLP; 
   CztTgeteNBtoSrc_eNBTprntCont *cztTgeteNBtoSrc_eNBTprntCont;

   TRC2(veHoPrcX2HOAck);

   cztInd = (CztEvnt *)hoAckPdu;
   /* Fetch the ueCb from X2 Control block */
   ueCb = x2ConCb->ueCb;

   /* It is assumed that, only for the First HO Ack this function 
      will be called. */
   /* Updating the x2ConCb into ueCb as the UE enters into the HO EXECUTION state
      of X2 HO with this target eNB */
   ueCb->x2ConCb = x2ConCb;
   ueCb->hoType = VE_HO_TYPE_X2;
   
   if(ROK != veUpdCandLstSndCncl(ueCb))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcX2HOAck:\
               Udating the CandLst failed\n"));
     
   }
   /* Interpret the X2AP HO REQ ACK pdu and extract the RRC Reconfiguration ASN encoded buffer.
    * Send the RRC Connection Reconfiguration message to the UE */
   veUtlGetCztIE(&cztInd->pdu, Cztid_TgeteNBtoSrc_eNBTprntCont,(TknU8 **) &cztTgeteNBtoSrc_eNBTprntCont);

   if(NULLP == cztTgeteNBtoSrc_eNBTprntCont)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcX2HOAck:\
               cztTgeteNBtoSrc_eNBTprntCont is NULL.\n"));
      RETVALUE(RFAILED);
   }
   
   /* update the forward tunnel information */
   veFillErabFrmX2HoAck(ueCb, &cztInd->pdu);

   /* Request RRC to decode the Target_To_Src Trasparent Container*/

   VE_ALLOC(&nhuDecodeSduReq, sizeof(NhuDecodeSduReq));

   if(NULLP == nhuDecodeSduReq)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcX2HOAck:\
               Udating the CandLst failed\n"));

      RETVALUE(RFAILED);
   }

   nhuDecodeSduReq->hdr.cellId  =   ueCb->cellId;
   nhuDecodeSduReq->hdr.ueId    =   ueCb->crnti;
   nhuDecodeSduReq->hdr.transId =   VE_DEC_TRANSID_HO_CMD;
   nhuDecodeSduReq->sduType     =   NHU_MSG_HOCMD;
   VE_ALLOCMBUF(veCb.mem.region, veCb.mem.pool,&mBuf );
   /* convert the OSXL to Buffer */ 

   SAddPstMsgMult((Data *)cztTgeteNBtoSrc_eNBTprntCont->val,
         (MsgLen)cztTgeteNBtoSrc_eNBTprntCont->len, (Buffer *)mBuf);
   nhuDecodeSduReq->mbuf = mBuf;

    /* Decode the HO Command.This HO Command is used to send the RRC
     * Re-configuration. Further processing shall be done at the receipt
     * of NhuDecodeCfm*/
   VeLiNhuDecodeReq(&veCb.nhuSap[0]->pst, veCb.nhuSap[0]->suId, nhuDecodeSduReq); 

   RETVALUE(ROK);
} /* veHoPrcX2HOAck */

/*
*
*       Fun:   veHoPrcX2UeCtxtRls
*
*       Desc:  Process X2 UE Context Release
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoPrcX2UeCtxtRls
(
VeX2ConCb *x2ConCb,
void      *ptr
)
#else
PRIVATE S16 veHoPrcX2UeCtxtRls(x2ConCb,ptr)
VeX2ConCb *x2ConCb;
void      *ptr;
#endif
{
   VeUeCb  *ueCb = NULLP;
   U16     ueIdx = 0;
   CztEvnt *ueCnxtRls = (CztEvnt *)ptr;

   TRC2(veHoPrcX2UeCtxtRls); 

   if(NULLP == x2ConCb)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcX2UeCtxtRls:\
               x2ConCb is NULL.\n"));

      RETVALUE(RFAILED);
#endif
   }

   /* Get the UE Control Block */
   ueCb = x2ConCb->ueCb;

   if(NULLP == ueCb)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcX2UeCtxtRls:\
               ueCb is NULL.\n"));
#endif
      RETVALUE(RFAILED);
   }
   
   /* release UE resources */
   if(ROK != veSndCtfUeRls(ueCb))
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcX2UeCtxtRls:\
               Failed to veSndCtfUeRls .\n"));
#endif
       RETVALUE(RFAILED);
   }
   /* RRC ReEst */
   if(ROK != veSndRgrUeRls(ueCb,NULLP,RGR_UE_CFG))
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcX2UeCtxtRls:\
               Failed to veSndRgrUeRls\n"));
#endif
       RETVALUE(RFAILED);
   }

   VeLiNhuCnclUeReq(&(veCb.nhuSap[0]->pst), veCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);
   
   /* release s1 resources */
   veSndS1UeLocRlsReq(x2ConCb->ueCb->s1ConCb);
   veCb.s1apConLst[x2ConCb->ueCb->s1ConCb->suConnId] = NULLP;
   VE_FREE(x2ConCb->ueCb->s1ConCb,sizeof(VeS1ConCb));
   
   /* release x2 resources */
   veSndX2UeLocRlsReq(ueCb->x2ConCb);
   veCb.x2apConLst[x2ConCb->oldEnbUeX2apId] = NULLP;
   
   /* Free the candidate CellDb list */
   veClrCandLst(&ueCb->candCellList);

   ueIdx = VE_GET_UE_IDX(ueCb->crnti);
   if(ueIdx < veCb.cellCb[0]->maxUeSupp)
   {
      veCb.cellCb[0]->ueCbLst[ueIdx] = NULLP;
      veCb.cellCb[0]->numUeCfg--;
   }
   VE_FREE(ueCb,sizeof(VeUeCb));
   
   /* Free PDU */
   cmFreeMem (ueCnxtRls);

   RETVALUE(ROK);

} /* veHoPrcX2UeCtxtRls */

/*
*
*       Fun:   veHoPrcX2OvrAllTmrExpiry
*
*       Desc:  Process X2 Overall Timer Expiry indication
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoPrcX2OvrAllTmrExpiry
(
VeX2ConCb *x2ConCb,
void      *ptr
)
#else
PRIVATE S16 veHoPrcX2OvrAllTmrExpiry(x2ConCb, ptr)
VeX2ConCb *x2ConCb;
void      *ptr;
#endif
{
   VeUeCb *ueCb = NULLP;
   U8     ueIdx = 0;
   SztDatEvntReq  datEvt;
   VeS1ConCb *s1ConCb = NULLP;
   S1apPdu   *pdu = NULLP;

   TRC2(veHoPrcX2OvrAllTmrExpiry);
   
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoPrcX2OvrAllTmrExpiry:\
                  Triggering S1 Overall Timer Expiry\n"));
#endif
   /* Get the UE Control Block */
   ueCb = x2ConCb->ueCb;
      
   /* release UE resources */
   veSndCtfUeRls(ueCb);
   /* RRC ReEst */
   veSndRgrUeRls(ueCb,NULLP,RGR_UE_CFG);
   VeLiNhuCnclUeReq(&(veCb.nhuSap[0]->pst), veCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);
      
   s1ConCb = ueCb->s1ConCb;
   /* Send EGTP release command to EGTP-U*/
   veSndEgtpCom(VE_EGTP_TUN_DEL, s1ConCb);
  
   /* Build Ue Context Release Request */
   veUtlSztUeCntxtRel(s1ConCb, &pdu);

   VE_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));
   datEvt.spConnId = s1ConCb->spConnId;
   datEvt.pdu = pdu;
   /* Send Ue Context Release Request msg to MME */
   VeLiSztDatReq(&veCb.sztSap[0]->pst,veCb.sztSap[0]->spId , &datEvt);

   veCb.s1apConLst[s1ConCb->suConnId] = NULLP;
   VE_FREE(s1ConCb,sizeof(VeS1ConCb));

   veCb.s1apConLst[x2ConCb->oldEnbUeX2apId] = NULLP;

   /* Free the candidate CellDb list */
   veClrCandLst(&x2ConCb->ueCb->candCellList);

   ueIdx = VE_GET_UE_IDX(ueCb->crnti);
   if(ueIdx < veCb.cellCb[0]->maxUeSupp)
   {
      veCb.cellCb[0]->ueCbLst[ueIdx] = NULLP;
      veCb.cellCb[0]->numUeCfg--;
   }
   VE_FREE(ueCb,sizeof(VeUeCb));

   RETVALUE(ROK);

} /* veHoPrcX2OvrAllTmrExpiry */

/*
*
*       Fun:   veHoSndX2HOAck
*
*       Desc:  Send X2 Handover Ack
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoSndX2HOAck
(
VeX2ConCb *x2ConCb,
void      *trsptCntr
)
#else
PRIVATE S16 veHoSndX2HOAck(x2ConCb, trsptCntr)
VeX2ConCb *x2ConCb;
void      *trsptCntr;
#endif
{
   NhuEncCfmSdus *hoCmdTrsntCntr = (NhuEncCfmSdus*)trsptCntr;
   CztEvnt *hoAckPdu = NULLP;

   TRC2(veHoSndX2HOAck);

   if(RFAILED == veUtlCztFillHovrRqstAck(&hoAckPdu, x2ConCb, hoCmdTrsntCntr, x2ConCb->ueCb->rbInfo.numOfRbCfgd, x2ConCb->ueCb->rbInfo.rab))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoSndX2HOAck:\
               Filling of HO Ack Failed\n"));
      RETVALUE(RFAILED);
   }
   /* filling the peerId */
   hoAckPdu->peerId = x2ConCb->peerId;

   VeLiCztBmpRsp(&veCb.cztSap[0]->pst, veCb.cztSap[0]->spId, hoAckPdu);

   RETVALUE(ROK);

} /* veHoSndX2HOAck */

/*
*
*       Fun:   veHoSndX2HOPrepFailure
*
*       Desc:  Send X2 Handover Preparation Failure
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoSndX2HOPrepFailure
(
VeX2ConCb *x2ConCb,
void      *unused
)
#else
PRIVATE S16 veHoSndX2HOPrepFailure(x2ConCb, unused)
VeX2ConCb *x2ConCb;
void      *unused;
#endif
{
   CztEvnt *hoPrpFlrPdu = NULLP;

   TRC2(veHoSndX2HOPrepFailure);

   if(RFAILED == veUtlCztFillHovrPrepFail(&hoPrpFlrPdu, x2ConCb))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoSndX2HOPrepFailure:\
               Filling of HO Preparation Failure Failed\n"));
      RETVALUE(RFAILED);
   }
   /* filling the peerId */
   hoPrpFlrPdu->peerId = x2ConCb->peerId;

   VeLiCztBmpRsp(&veCb.cztSap[0]->pst, veCb.cztSap[0]->spId, hoPrpFlrPdu);

   RETVALUE (ROK);

} /* veHoSndX2HOPrepFailure */

/*
*
*       Fun:   veHoPrcX2Cancel
*
*       Desc:  Process handover cancel
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoPrcX2Cancel
(
VeX2ConCb *x2ConCb,
void      *ptr
)
#else
PRIVATE S16 veHoPrcX2Cancel(x2ConCb, ptr)
VeX2ConCb *x2ConCb;
void      *ptr;
#endif
{
   VeUeCb *ueCb = NULLP;
   U8     ueIdx = 0;

   TRC2(veHoPrcX2Cancel);

   /* Release UE Resources */
   /* Get the UE Control Block */
   ueCb = x2ConCb->ueCb;

   if(ueCb->lowLyrCfgSts == VE_LOW_LYR_CFG_COMPL || 
         ueCb->lowLyrCfgSts == VE_LOW_LYR_CFG_NOT_STRTD)
   {
      ueCb->lowLyrCfgSts = VE_LOW_LYR_CFG_REM;
      /* release UE resources */
      veSndCtfUeRls(ueCb);
      /* RRC ReEst */
      veSndRgrUeRls(ueCb,NULLP,RGR_UE_CFG);
      VeLiNhuCnclUeReq(&(veCb.nhuSap[0]->pst), veCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);

      /* release x2 resources */
      veSndX2UeLocRlsReq(ueCb->x2ConCb);
      veCb.x2apConLst[VE_GET_UE_X2_IDX(x2ConCb->oldEnbUeX2apId)] = NULLP;

      /* Send EGTP release command to EGTP-U*/
      veSndEgtpCom(VE_EGTP_TUN_DEL, x2ConCb->ueCb->s1ConCb);

      /* release s1 resources */
      veSndS1UeLocRlsReq(ueCb->s1ConCb);
      veCb.s1apConLst[ueCb->s1ConCb->suConnId] = NULLP;
      VE_FREE(ueCb->s1ConCb,sizeof(VeS1ConCb));

      /* Free the candidate CellDb list */
      veClrCandLst(&ueCb->candCellList);

      ueIdx = VE_GET_UE_IDX(ueCb->crnti);
      if(ueIdx < veCb.cellCb[0]->maxUeSupp)
      {
         veCb.cellCb[0]->ueCbLst[ueIdx] = NULLP;
         veCb.cellCb[0]->numUeCfg--;
      }
      VE_FREE(ueCb,sizeof(VeUeCb));
   }
   else if(ueCb->lowLyrCfgSts == VE_LOW_LYR_CFG_IN_PRGS)
   {
      ueCb->lowLyrCfgSts = VE_LOW_LYR_CFG_REM;
   }

   RETVALUE(ROK);
} /* veHoPrcX2Cancel */

/*
*
*       Fun:   veX2HoProcHoCfm
*
*       Desc:  Send S1AP Handover Notify message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veX2HoProcHoCfm
(
VeX2ConCb *x2ConCb,
void      *ptr
)
#else
PRIVATE S16 veX2HoProcHoCfm(x2ConCb, ptr)
VeX2ConCb *x2ConCb;
void      *ptr;
#endif
{

   TRC2(veX2HoProcHoCfm);
   if( ROK != veHoProcHoCfm(x2ConCb->ueCb))
   {
#ifdef DEBUGP
      VE_LYR_DBGP(DBGMASK_SM, (VE_PRNT_BUF, "\n Failed to fill Cancel Request\n"));
#endif
    } /* end of if statement */

   RETVALUE(ROK);
}


/*
*
*       Fun:   veX2HoPrcUeHoAttachTmrExpiry
*
*       Desc:  
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veX2HoPrcUeHoAttachTmrExpiry
(
VeX2ConCb *x2ConCb,
void      *ptr
)
#else
PRIVATE S16 veX2HoPrcUeHoAttachTmrExpiry(x2ConCb, ptr)
VeX2ConCb *x2ConCb;
void      *ptr;
#endif
{

   TRC2(veX2HoPrcUeHoAttachTmrExpiry);
   if( ROK != veHoPrcUeHoAttachTmrExpiry(x2ConCb->ueCb))
   {
#ifdef DEBUGP
      VE_LYR_DBGP(DBGMASK_SM, (VE_PRNT_BUF, "\n Failed to fill Cancel Request\n"));
#endif
    } /* end of if statement */
   RETVALUE(ROK);
}


/*
*
*       Fun:   veSndX2ResetRqst
*
*       Desc:  Send X2 Reset Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veSndX2ResetRqst
(
CztCause          *cause,
U32                peerId
)
#else
PUBLIC S16 veSndX2ResetRqst(cause, peerId)
CztCause          *cause;
U32                peerId;
#endif
{
   CztEvnt       *rstReqPdu = NULLP;

   /* To trace the Procedure */
   TRC2(veSndX2ResetRqst);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (VE_PRNT_BUF, "veSndX2ResetRqst : Sending of X2 Reset Req\n"));
#endif

   /* Fill the Reset Req Pdu and send to T-eNB */
   if(RFAILED == veUtlCztFillResetRqst(&rstReqPdu, cause))
   {
      RETVALUE(RFAILED);
   }
   /* filling the peerId */
   rstReqPdu->peerId = peerId;

   RETVALUE(VeLiCztGpReq (&veCb.cztSap[0]->pst, veCb.cztSap[0]->spId, rstReqPdu));

} /* veSndX2ResetRqst */

/*
*
*       Fun:   veSndX2ResetRsp
*
*       Desc:  Send X2 Reset Response 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_x2_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veSndX2ResetRsp
(
U32 peerId
)
#else
PUBLIC S16 veSndX2ResetRsp(peerId)
U32 peerId;
#endif
{
   CztEvnt       *rstRspPdu = NULLP;

   /* To trace the Procedure */
   TRC2(veSndX2ResetRsp);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (VE_PRNT_BUF, "veSndX2ResetRsp : Sending of X2 Reset Req\n"));
#endif

   /* Fill the Reset Req Pdu and send to T-eNB */
   if(ROK != veUtlCztFillResetRsp (&rstRspPdu))
   {
      RETVALUE(RFAILED);
   }
   /* filling the peerId */
   rstRspPdu->peerId = peerId;

   RETVALUE(VeLiCztGpRsp (&veCb.cztSap[0]->pst, veCb.cztSap[0]->spId, rstRspPdu));

} /* veSndX2ResetRsp */

/* X2_CFG_UPD_CHANGES */
/*
*
*       Fun:   veCztCfgUpdReq
*
*       Desc:  X2 Confguration Update Request
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_relay.c
*
*/
#ifdef ANSI
PUBLIC S16 veCztCfgUpdReq
(
U32 peerId,
VeCfgUpdInfo *veCfg
)
#else
PUBLIC S16 veCztCfgUpdReq(peerId, veCfg)
U32 peerId;
VeCfgUpdInfo *veCfg;
#endif
{
   CztEvnt *x2CfgUpdReq = NULLP;
   S16      retVal = 0;

   VeNghCellCb *neighCellCb = NULLP;
  
   TRC2(veCztCfgUpdReq);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veCztCfgUpdReq:\
            Triggering Configuration Update Request \n"));
#endif

   veHOFndInNghLstByPeerId(veCb.cellCb[0], peerId, &neighCellCb);
   if(NULLP == neighCellCb)
   {
      VE_DBG_ERROR((veCb.init.prntBuf,"veLiCztX2SetupRspHandler:\
                      invalid setup responde from non existing peer\n"));
      RETVALUE(RFAILED);
   }
  
/*   VE_ALLOC(&cfgUpdInfo , sizeof(VeCfgUpdInfo));*/
   neighCellCb->cfgUpdInfo.numSrvdCellAdded = veCfg->numSrvdCellAdded;
   neighCellCb->cfgUpdInfo.numSrvdCellDeleted = veCfg->numSrvdCellDeleted;
   neighCellCb->cfgUpdInfo.numSrvdCellModified = veCfg->numSrvdCellModified;
   neighCellCb->cfgUpdInfo.numGrpIdAdded = veCfg->numGrpIdAdded;
   neighCellCb-> cfgUpdInfo.numGrpIdDeleted = veCfg->numGrpIdDeleted;
   neighCellCb->cfgUpdInfo.srvdCellAdd[0] = veCfg->srvdCellAdd[0];
   neighCellCb->cfgUpdInfo.srvdCellMod[0].oldCellEcgi.plmnId = veCfg->srvdCellMod[0].oldCellEcgi.plmnId;
   neighCellCb->cfgUpdInfo.srvdCellMod[0].oldCellEcgi.eutranCellId = veCfg->srvdCellMod[0].oldCellEcgi.eutranCellId;
   neighCellCb->cfgUpdInfo.srvdCellMod[0].newCellId = veCfg->srvdCellMod[0].newCellId;
   neighCellCb->cfgUpdInfo.oldEcgiToDel[0].plmnId = veCfg->oldEcgiToDel[0].plmnId;
   neighCellCb->cfgUpdInfo.oldEcgiToDel[0].eutranCellId = veCfg->oldEcgiToDel[0].eutranCellId;
   neighCellCb->cfgUpdInfo.guIdAddLst[0].plmnId = veCfg->guIdAddLst[0].plmnId;
   neighCellCb->cfgUpdInfo.guIdAddLst[0].mmeGrpId = veCfg->guIdAddLst[0].mmeGrpId;
   neighCellCb->cfgUpdInfo.guIdDelLst[0].plmnId = veCfg->guIdDelLst[0].plmnId;
   neighCellCb->cfgUpdInfo.guIdDelLst[0].mmeGrpId = veCfg->guIdDelLst[0].mmeGrpId;

   /* Build setup message */
   retVal = veCztFillCfgUpdReq( veCfg , &x2CfgUpdReq);
   if(ROK != retVal)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillCfgUpdReq:\
                                Failed to Fill veUtlCztFillCfgUpdReq \n"));
#endif
      RETVALUE(RFAILED);
   }
   x2CfgUpdReq->peerId = peerId;
   /* Trigger corresponding primitive . */
   RETVALUE(VeLiCztGpReq(&(veCb.cztSap[0]->pst), veCb.cztSap[0]->spId, x2CfgUpdReq));
} /* end of veCztCfgUpdReq */




/*
*
*       Fun:   veCztErrInd
*
*       Desc:  X2 Error Indication
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_relay.c
*
*/
#ifdef ANSI
PUBLIC S16 veCztErrInd
(
U32                        peerId,
U32                        oldeNBUeX2apId,
U32                        neweNBUeX2apId,
Bool                       causePres,
U8                         causeType,
U8                         causeVal,
VeIeInfo                   *idLst
)
#else
PUBLIC S16 veCztErrInd(peerId, oldeNBUeX2apId, neweNBUeX2apId, causePres, causeType, causeVal, idLst)
U32                        peerId;
U32                        oldeNBUeX2apId;
U32                        neweNBUeX2apId;
Bool                       causePres;
U8                         causeType;
U8                         causeVal;
VeIeInfo                   *idLst;
#endif
{
   CztEvnt *x2ErrInd = NULLP;
   S16      retVal = 0; 

   TRC2(veCztErrIndReq);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veCztCfgUpdReq:\
            Triggering Error Indication Request \n"));
#endif

   /* Build setup message */
  
   retVal = veFillErrInd(&x2ErrInd, oldeNBUeX2apId,  neweNBUeX2apId, causePres, causeType,  causeVal, idLst);
   if(ROK != retVal)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillCfgUpdReq:\
                                Failed to Fill Error Indication \n"));
#endif 
      RETVALUE(RFAILED);
   }
   x2ErrInd->peerId = peerId;
   /* Trigger corresponding primitive . */
    RETVALUE(VeLiCztGpErrReq(&(veCb.cztSap[0]->pst), veCb.cztSap[0]->spId, x2ErrInd));
} /* end of veCztErrInd */

/* X2_CFG_UPD_CHANGES_END */
#endif /* End of LTE_HO_SUPPORT */
/********************************************************************30**

         End of file:     ve_x2_ho.c@@/main/eNB_1.3_With_LatestGpr3/11 - Mon Sep 19 16:35:17 2011

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

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

     File:     ve_s1_ho.c

     Sid:      ve_s1_ho.c@@/main/eNB_1.3_With_LatestGpr3/10 - Mon Sep  5 17:18:04 2011

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

#include "czt.h"           /* MAC RRM control Interface */
#include "czt_asn.h"           /* MAC RRM control Interface */

/* RRM related includes */
#include "ve_eut.h"
#include "egt.h"
#include "ctf.h"
#include "ve.h" 
#include "lve.h" 
#include "ve_sm_init.h"
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

#include "czt.x"
#include "czt_asn.x"

/* RRM related includes */
#include "ve_eut.x"
#include "egt.x"
#include "ctf.x"
#include "lve.x" 
#include "ve.x"
#include "ve_sm_init.x"

#ifndef MSPD
#include "ve_dflt.h"
#else
#include "ve_dflt_ms.x"
#endif


#include "sz.h"

EXTERN CmPAsnElmDef szTgeteNB_ToSrceNB_TprntCont;

/* eNodeB Cb */

/* S1AP source FSM related Functions */
PRIVATE S16 veHoSndS1HORqrd ARGS (( VeS1ConCb* s1ConCb, void * ptr ));
PRIVATE S16 veHoPrcS1HOPrepFailure ARGS (( VeS1ConCb *s1ConCb, void *ptr ));
PRIVATE S16 veHoPrcS1HOPrepTmrExpiry ARGS (( VeS1ConCb* s1ConCb, void * ptr ));
PRIVATE S16 veHoPrcS1HOAck ARGS (( VeS1ConCb* s1ConCb, void * ptr ));
PRIVATE S16 veHoPrcS1OvrAllTmrExpiry ARGS (( VeS1ConCb* s1ConCb, void * ptr ));

/* S1AP target FSM related Functions */
PRIVATE S16 veHoSndS1HOAck ARGS (( VeS1ConCb* s1ConCb, void * ptr ));
PRIVATE S16 veHoSndS1HOPrepFailure ARGS (( VeS1ConCb* s1ConCb, void * ptr ));
PRIVATE S16 veS1HoPrcUeHoAttachTmrExpiry ARGS (( VeS1ConCb* s1ConCb, void * ptr ));
PRIVATE S16 veS1HoProcHoCfm ARGS (( VeS1ConCb* s1ConCb, void * ptr ));
PRIVATE S16 veS1HoProcPthSwtchFlr ARGS (( VeS1ConCb* s1ConCb, void * ptr ));
/* Function to handle Non HO related messages */
PRIVATE S16 veS1hoNoOpr ARGS (( VeS1ConCb* s1ConCb, void * ptr ));

/* S1 Handover State machine  for Source enb */
PUBLIC VeS1SrcFSM veS1SrcFSM[VE_HO_MAX_STATES][VE_HO_SRC_MAX_EVENTS]=
{
   /*VE_HO_NORMAL */
   { 
      veHoSndS1HORqrd,  /* VE_EVNT_HO_DECISION             */
      veS1hoNoOpr,      /* VE_EVNT_HO_PREP_FAILURE         */
      veS1hoNoOpr,      /* VE_EVNT_HO_PREP_TMR_EXPIRY      */
      veS1hoNoOpr,      /* VE_EVNT_HO_ACK_RCVD             */
      veS1hoNoOpr,      /* VE_EVNT_HO_UE_CNTXT_RLS         */
      veS1hoNoOpr       /* VE_EVNT_HO_OVERALL_TMR_EXPIRY   */
  },

  /* VE_HO_INITIATED */
   {
      veS1hoNoOpr,                /* VE_EVNT_HO_DECISION             */
      veHoPrcS1HOPrepFailure,     /* VE_EVNT_HO_PREP_FAILURE         */
      veHoPrcS1HOPrepTmrExpiry,   /* VE_EVNT_HO_PREP_TMR_EXPIRY      */
      veHoPrcS1HOAck,             /* VE_EVNT_HO_ACK_RCVD             */
      veS1hoNoOpr,                /* VE_EVNT_HO_UE_CNTXT_RLS         */
      veS1hoNoOpr                 /* VE_EVNT_HO_OVERALL_TMR_EXPIRY   */
  },

   /* VE_HO_IN_PROGRESS */
   { 
      veS1hoNoOpr,                /* VE_EVNT_HO_DECISION             */
      veS1hoNoOpr,                /* VE_EVNT_HO_PREP_FAILURE         */
      veS1hoNoOpr,                /* VE_EVNT_HO_PREP_TMR_EXPIRY      */
      veS1hoNoOpr,                /* VE_EVNT_HO_ACK_RCVD             */
      veS1hoNoOpr,                /* VE_EVNT_HO_UE_CNTXT_RLS         */
      veHoPrcS1OvrAllTmrExpiry    /* VE_EVNT_HO_OVERALL_TMR_EXPIRY   */
  },
   /* VE_HO_UE_RELEASE*/
   {
      veS1hoNoOpr,      /* VE_EVNT_HO_DECISION             */
      veS1hoNoOpr,      /* VE_EVNT_HO_PREP_FAILURE         */
      veS1hoNoOpr,      /* VE_EVNT_HO_PREP_TMR_EXPIRY      */
      veS1hoNoOpr,      /* VE_EVNT_HO_ACK_RCVD             */
      veS1hoNoOpr,      /* VE_EVNT_HO_UE_CNTXT_RLS         */
      veS1hoNoOpr       /* VE_EVNT_HO_OVERALL_TMR_EXPIRY   */
  }
};

/* S1 Handover State machine  for target enb */
PUBLIC VeS1TgtFSM veS1TgtFSM[VE_HO_MAX_STATES][VE_HO_TGT_MAX_EVENTS]=
{
   /*VE_HO_NORMAL */
   { 
      veS1hoNoOpr, 
      veS1hoNoOpr,
      veS1hoNoOpr,
      veS1hoNoOpr,
      veS1hoNoOpr,
      veS1hoNoOpr
  },

  /* VE_HO_INITIATED */
   { 
      veHoSndS1HOAck, 
      veHoSndS1HOPrepFailure,
      veS1hoNoOpr,
      veS1hoNoOpr,
      veS1hoNoOpr,
      veS1hoNoOpr
  },

   /* VE_HO_IN_PROGRESS */
   { 
      veS1hoNoOpr, 
      veS1hoNoOpr,
      veS1HoProcHoCfm,
      veS1HoProcPthSwtchFlr,
      veS1HoPrcUeHoAttachTmrExpiry,
      veS1hoNoOpr
  },
  /* VE_HO_UE_RELEASE*/
   { 
      veS1hoNoOpr, 
      veS1hoNoOpr,
      veS1hoNoOpr,
      veS1hoNoOpr,
      veS1hoNoOpr,
      veS1hoNoOpr
  }
};


/*
*
*       Fun:   veHoSndS1HORqrd
*
*       Desc:  Send HO Required
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoSndS1HORqrd
(
VeS1ConCb    *s1ConCb,
void         *nghInfo
)
#else
PRIVATE S16 veHoSndS1HORqrd(s1ConCb, nghInfo)
VeS1ConCb    *s1ConCb;
void         *nghInfo;
#endif
{
   S1apPdu      *hoReqPdu = NULLP;
   SztDatEvntReq datEvt;
   S16      retVal;
   
   TRC2(veHoSndS1HORqrd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoSndS1HORqrd:\
            Triggering sending of S1 HO Req\n"));
#endif

   /* Fill the HO Req Pdu and send to MME */
   retVal = veUtlSztFillHovrReqd ( s1ConCb, &hoReqPdu, (VeNghCellCb*)nghInfo,s1ConCb->ueCb->trnspContainer);
   if(ROK != retVal)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqd : Failed "));
      RETVALUE(RFAILED);
#endif
   }
   datEvt.spConnId = s1ConCb->spConnId;
   datEvt.pdu    =  hoReqPdu;

   /* Trigger SZT Connectn. Req */
   retVal = VeLiSztDatReq(&veCb.sztSap[0]->pst, veCb.sztSap[0]->spId, &datEvt);

   if(ROK != retVal)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF," VeLiSztDatReq : Failed "));
      RETVALUE(RFAILED);
#endif
   }

   /* Start the S1 Reloc timer */
   veStartTmr((PTR)s1ConCb->ueCb, VE_TMR_S1_RELOC_TMR, veCb.s1PrepTimerVal);

   RETVALUE(ROK);
         
} /* veHoSndS1HORqrd */

/*
*
*       Fun:   veHoPrcS1HOPrepFailure
*
*       Desc:  Function to process HO Preparation Failure
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoPrcS1HOPrepFailure
(
VeS1ConCb *s1ConCb,
void      *ptr
)
#else
PRIVATE S16 veHoPrcS1HOPrepFailure(s1ConCb, ptr)
VeS1ConCb *s1ConCb;
void      *ptr;
#endif
{
   TRC2(veHoPrcS1HOPrepFailure);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoPrcS1HOPrepFailure:\
            Triggering S1 HO Prep failure\n"));
#endif

   /* Stop the Relocation Timer */
   veStopTmr((PTR)s1ConCb->ueCb, VE_TMR_S1_RELOC_TMR);

   /* update the candidate cell list for the ue */
   /* for current release, HO preparation with the next node is not tried */
   veClrCandLst(&(s1ConCb->ueCb->candCellList));
   
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoPrcS1HOPrepFailure:\
                  HO Prep failure at target\n"));
#endif
   
   s1ConCb->ueCb->mobCtrlState = VE_HO_NORMAL;


   RETVALUE(ROK);
} /* veHoPrcS1HOPrepFailure*/

/*
*
*       Fun:   veHoPrcS1HOPrepTmrExpiry
*
*       Desc:  Function to process S1 HO Prepartion Timer expiry 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoPrcS1HOPrepTmrExpiry
(
VeS1ConCb *s1ConCb,
void      *ptr
)
#else
PRIVATE S16 veHoPrcS1HOPrepTmrExpiry(s1ConCb,ptr)
VeS1ConCb *s1ConCb;
void      *ptr;
#endif
{
   TRC2(veHoPrcS1HOPrepTmrExpiry);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoPrcS1HOPrepTmrExpiry:\
            Triggering S1 HO Prep Timer Expiry\n"));
#endif

   /* update the candidate cell list for the ue */
   /* for current release, HO preparation with the next node is not tried */
   veClrCandLst(&(s1ConCb->ueCb->candCellList));
     
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoPrcS1HOPrepFailure:\
                  HO Prep failure at target\n"));
#endif
     
   s1ConCb->ueCb->mobCtrlState = VE_HO_NORMAL;

   /* Send S1AP Cancel message */
   if(RFAILED == veHoSndS1HOCncl((VeUeCb *)s1ConCb->ueCb))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcS1HOPrepTmrExpiry:\
                 sending of S1 HO Cancel failed for node\n"));
   } /* end of if statement */
   
   RETVALUE(ROK);

} /* veHoPrcS1HOPrepTmrExpiry */


/*
*
*       Fun:   veHoPrcS1HOAck
*
*       Desc:  processing of the HO Command Received from MME
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_s1_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoPrcS1HOAck
(
VeS1ConCb *s1ConCb,
void      *pdu
)
#else
PRIVATE S16 veHoPrcS1HOAck(s1ConCb,pdu)
VeS1ConCb *s1ConCb;
void      *pdu;
#endif
{
   SztTget_ToSrc_TprntCont *tgtToSrcTprntCont = NULLP;
   NhuDecodeSduReq         *nhuDecodeSduReq = NULLP;
   Buffer                  *mBuf = NULLP;
   U8                      ret = RFAILED;
   SztTgeteNB_ToSrceNB_TprntCont tgetToSrcCntr;
   Buffer *trnspContMBuf = NULLP;
   S1apEncDecIE *decTrpCnt = NULLP;
   SztReqStatus reqSts;

   U32 asnLen = 0;
   U32          numDecOct = 0;
   CmPAsnErr    asnErr;
   CmPAsnElmDef *szMsgIEDb_locTesting[][4] = { {&szTgeteNB_ToSrceNB_TprntCont} };

   TRC2(veHoPrcS1HOAck);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoPrcS1HOAck:\
            Triggering S1 HO Prep Timer Expiry\n"));
#endif

   /* Received the HO Command From MME. So stop the S1 Relocation Timer */
   veStopTmr((PTR)s1ConCb->ueCb, VE_TMR_S1_RELOC_TMR);

   /* Received the HO Command From MME. Start S1 Overall Timer */
   veStartTmr((PTR)s1ConCb->ueCb, VE_TMR_S1_OVRL_TMR, veCb.s1OvrAllTimerVal);

   if(ROK != veUpdCandLstSndCncl(s1ConCb->ueCb))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcS1HOAck:\
               Udating the CandLst failed\n"));
     
   }

   /* update the UE HO Type to S1 */
   s1ConCb->ueCb->hoType = VE_HO_TYPE_S1;

   /* Interpret the S1AP Handover Command pdu. From the PDU extract the RRC Reconfiguration ASN encoded buffer
    * Send th RRC Connection Reconfiguration message to the UE */

    /* Decoding the SrcToTget Tranparent Container */
   veSzGetIE((SztS1AP_PDU *)&((S1apPdu *)pdu)->pdu, Sztid_Tget_ToSrc_TprntCont,(TknU8 **) &tgtToSrcTprntCont);

   /* Alloc memory for mBuf */
   SGetMsg(veCb.mem.region, veCb.mem.pool, &trnspContMBuf);
   if(trnspContMBuf == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcS1HOAck: cmGetMem failed \
               for trnspContMBuf.\n"));
#endif
      RETVALUE(RFAILED);
   }

    /* convert the OSXL to Buffer */
    SAddPstMsgMult(tgtToSrcTprntCont->val, \
          tgtToSrcTprntCont->len, trnspContMBuf);

   /* Allocate the memory for S1 decTrpCnt */
   VE_ALLOC((Ptr *)&decTrpCnt, sizeof(S1apEncDecIE));
   if(NULLP == decTrpCnt)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcS1HOAck: cmGetMem failed \
               for S1 ENCDEC structure.\n"));
#endif
      RETVALUE(RFAILED);
   }
   cmMemset((U8 *)&reqSts, 0, sizeof(SztReqStatus));
   decTrpCnt->iePtr = &tgetToSrcCntr;


   /* Encode the IE */
   ret = cmPAsnDecMsg((TknU8 *)(decTrpCnt->iePtr), /* CM_PASN_USE_MBUF */ 1,
         (Void **)&(trnspContMBuf), &numDecOct, /* SZ_PASN_VER_ZERO */ 0,
         (CmPAsnElmDef **)/* szMsgIEDb[sztIeId] */ szMsgIEDb_locTesting[0],\
         /* CM_PASN_GEN_ERR */ 3, \
         &asnErr, asnLen, (CmMemListCp *)(&(decTrpCnt->memCp)),\
         /* dbgFlag */ TRUE);

   if(ret != ROK)
   {
      /* Report the error incase if encoding fails */
      if (asnErr.errCode == CM_PASN_ESC_FUNC_FLD)
      {
         printf("%s:Encoding failed, Abstract Syntax Error:Reason %d\n", \
                  __func__, asnErr.errCode);
      }
      else
      {
         printf("%s:Encoding failed, Transfer Syntax Error:Reason %d\n", \
                  __func__, asnErr.errCode);
      }
   }

    /* update the forward tunnel information */
    ret = veFillErabFrmS1HoAck(&s1ConCb->ueCb->rbInfo, \
          &((S1apPdu *)pdu)->pdu, s1ConCb->mmeCb);
    if(ret == RFAILED)
    {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcS1HOAck: \
                  Data Forwarding Decode Error.\n"));
#endif
         RETVALUE(RFAILED);
    }

    /* Provide the Target_To_Src Trasparent Container to RRC so that it is
     * decoded. Target_To_Src Transparent Container is OSXL type. */

    VE_ALLOC(&nhuDecodeSduReq, sizeof(NhuDecodeSduReq));

    if(NULLP == nhuDecodeSduReq)
    {
       VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcS1HOAck:\
               Udating the CandLst failed\n"));
      
       RETVALUE(RFAILED);
    }

    nhuDecodeSduReq->hdr.cellId  =   s1ConCb->ueCb->cellId;
    nhuDecodeSduReq->hdr.ueId    =   s1ConCb->ueCb->crnti;
    nhuDecodeSduReq->hdr.transId =   VE_DEC_TRANSID_HO_CMD;
    nhuDecodeSduReq->sduType     =   NHU_MSG_HOCMD;

    SGetMsg (veCb.mem.region, veCb.mem.pool, &mBuf);
    /* convert the OSXL to Buffer */
    SAddPstMsgMult(tgetToSrcCntr.rRC_Cont.val, \
          tgetToSrcCntr.rRC_Cont.len, mBuf);
    nhuDecodeSduReq->mbuf = mBuf;

    /* Decode the HO Command.This HO Command is used to send the RRC
     * Re-configuration. Further processing shall be done at the receipt
     * of NhuDecodeCfm*/
   VeLiNhuDecodeReq(&veCb.nhuSap[0]->pst, veCb.nhuSap[0]->suId, \
         nhuDecodeSduReq); 

   RETVALUE(ROK);

} /* veHoPrcS1HOAck */


/*
*
*       Fun:   veHoPrcS1OvrAllTmrExpiry
*
*       Desc:  Function to process S1AP Overall timer exipry
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoPrcS1OvrAllTmrExpiry
(
VeS1ConCb *s1ConCb,
void      *ptr
)
#else
PRIVATE S16 veHoPrcS1OvrAllTmrExpiry(s1ConCb,ptr)
VeS1ConCb *s1ConCb;
void      *ptr;
#endif
{
   U16 ueIdx;
   SztDatEvntReq datEvt;
   VeUeCb *ueCb = NULLP;
   S1apPdu *ueRlsPdu = NULLP;

   TRC2(veHoPrcS1OvrAllTmrExpiry);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoPrcS1OvrAllTmrExpiry:\
            Triggering S1 Overall Timer Expiry\n"));
#endif


   /* Get the UE Control Block */
   ueCb = s1ConCb->ueCb;
   
   /* release UE resources */
   veSndCtfUeRls(ueCb);
   /* RRC ReEst */
   veSndRgrUeRls(ueCb,NULLP,RGR_UE_CFG);
   VeLiNhuCnclUeReq(&(veCb.nhuSap[0]->pst), veCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);
   
   /* Send EGTP release command to EGTP-U*/
   veSndEgtpCom(VE_EGTP_TUN_DEL, s1ConCb);

   s1ConCb->cause.choice.val = CAUSE_RADIONW;
   s1ConCb->cause.val.radioNw.val = SztCauseRadioNwtS1relocoverall_expiryEnum;

   /* Construct the S1AP UE Context Release towards MME */
   veUtlSztUeCntxtRel(s1ConCb, &ueRlsPdu);

   VE_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));
   datEvt.spConnId = s1ConCb->spConnId;
   datEvt.pdu = ueRlsPdu;

   /* Send the UE Context Release Request towards MME */
   VeLiSztDatReq(&veCb.sztSap[0]->pst,veCb.sztSap[0]->spId , &datEvt);


   veCb.s1apConLst[s1ConCb->suConnId] = NULLP;
   VE_FREE(s1ConCb,sizeof(VeS1ConCb));
   
   ueIdx = VE_GET_UE_IDX(ueCb->crnti);
   if(ueIdx < veCb.cellCb[0]->maxUeSupp)
   {
      veCb.cellCb[0]->ueCbLst[ueIdx] = NULLP;
      veCb.cellCb[0]->numUeCfg--;
   }
   VE_FREE(ueCb,sizeof(VeUeCb));

   RETVALUE(ROK);

} /* veHoPrcS1OvrAllTmrExpiry */

/*
*
*       Fun:   veHoSndS1HOAck
*
*       Desc:  Process the S1AP Handover Acknowledgement 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoSndS1HOAck
(
VeS1ConCb *s1ConCb,
void      *trsptCntr
)
#else
PRIVATE S16 veHoSndS1HOAck(s1ConCb, trsptCntr)
VeS1ConCb *s1ConCb;
void      *trsptCntr;
#endif
{
   NhuEncCfmSdus *hoCmdTrsntCntr = (NhuEncCfmSdus*)trsptCntr;
   S1apPdu *hoAckPdu = NULLP;
   SztConRsp *conResp = NULLP;

   TRC2(veHoSndS1HOAck);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoSndS1HOAck:\
            Triggering processing of veHoSndS1HOAck Request \n"));
#endif

   if(RFAILED == veUtlSztFillHovrReqAck(s1ConCb, &hoAckPdu,s1ConCb->ueCb->rbInfo, hoCmdTrsntCntr))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoSndS1HOAck:\
               Filling of HO Ack Failed\n"));
      RETVALUE(RFAILED);
   }

   VE_ALLOC(&conResp, sizeof(SztConRsp));
   
   if(NULLP == conResp)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoSndS1HOAck:\
                   Failed to allocate Memory."));
      RETVALUE(RFAILED);
   }

   conResp->suConnId = s1ConCb->suConnId;
   conResp->spConnId = s1ConCb->spConnId;
   conResp->pdu      = hoAckPdu;

   /* Send pkt to S1AP */
   VeLiSztConRsp(&veCb.sztSap[0]->pst, veCb.sztSap[0]->spId, conResp);

   RETVALUE(ROK);
} /* veHoSndS1HOAck */

/*
*
*       Fun:   veHoSndS1HOPrepFailure
*
*       Desc:  Send S1AP HO Preparation Failure 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veHoSndS1HOPrepFailure
(
VeS1ConCb *s1ConCb,
void      *ptr
)
#else
PRIVATE S16 veHoSndS1HOPrepFailure(s1ConCb,ptr)
VeS1ConCb *s1ConCb;
void      *ptr;
#endif
{
   S1apPdu *hoPrepFlrPdu = NULLP;
   SztDatEvntReq *datEvt = NULLP;

   TRC2(veHoSndS1HOPrepFailure);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoSndS1HOPrepFailure:\
            Triggering sending of S1 HO Prep Failure Request \n"));
#endif

   if(ROK != veUtlSztFillHovrFail(s1ConCb, &hoPrepFlrPdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoSndS1HOPrepFailure:\
            Filling of HO Preparation Failure Failed\n"));
      RETVALUE(RFAILED);
   }
      
   VE_ALLOC(&datEvt, sizeof(SztDatEvntReq));
   
   if(NULLP == datEvt)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoSndS1HOPrepFailure:\
                Failed to allocate Memory.\n"));
      RETVALUE(RFAILED);
   }
      
   datEvt->spConnId = s1ConCb->spConnId;
   datEvt->pdu    =  hoPrepFlrPdu;

   VeLiSztDatReq(&veCb.sztSap[0]->pst, veCb.sztSap[0]->spId, datEvt);

   RETVALUE(ROK);

} /* veHoSndS1HOPrepFailure */

/*
*
*       Fun:   veS1HoProcHoCfm
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
PRIVATE S16 veS1HoProcHoCfm
(
VeS1ConCb *s1ConCb,
void      *ptr
)
#else
PRIVATE S16 veS1HoProcHoCfm(s1ConCb, ptr)
VeS1ConCb *s1ConCb;
void      *ptr;
#endif
{

   TRC2(veS1HoProcHoCfm);
   if( ROK != veHoProcHoCfm(s1ConCb->ueCb))
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\n Failed to fill Cancel Request\n"));
#endif
    } /* end of if statement */

   RETVALUE(ROK);
}

/*
*
*       Fun:   veS1HoProcPthSwtchFlr
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
PRIVATE S16 veS1HoProcPthSwtchFlr
(
VeS1ConCb *s1ConCb,
void      *ptr
)
#else
PRIVATE S16 veS1HoProcPthSwtchFlr(s1ConCb, ptr)
VeS1ConCb *s1ConCb;
void      *ptr;
#endif
{

   TRC2(veS1HoProcPthSwtchFlr);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veS1HoProcPthSwtchFlr:\
               Triggering S1AP HO Path Switch Failure Handler\n"));
#endif
   
   /* TODO : log the cause of failure */
   s1ConCb->ueCb->mobCtrlState = (VeHoState) VE_EVNT_HO_UE_CNTXT_RLS;
   
   /* invoke x2 UE release */
   veSndX2UeCtxtRls(s1ConCb->ueCb->x2ConCb, NULLP);

   RETVALUE(ROK);
}


/*
*
*       Fun:   veS1HoPrcUeHoAttachTmrExpiry
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
PRIVATE S16 veS1HoPrcUeHoAttachTmrExpiry
(
VeS1ConCb *s1ConCb,
void      *ptr
)
#else
PRIVATE S16 veS1HoPrcUeHoAttachTmrExpiry(s1ConCb, ptr)
VeS1ConCb *s1ConCb;
void      *ptr;
#endif
{

   TRC2(veS1HoPrcUeHoAttachTmrExpiry);

   if(NULLP == s1ConCb)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veS1HoPrcUeHoAttachTmrExpiry: invalid s1ConCb\n"));

      RETVALUE(RFAILED);
   }
   if( ROK != veHoPrcUeHoAttachTmrExpiry(s1ConCb->ueCb))
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\n Failed to fill Cancel Request\n"));
#endif
    } /* end of if statement */
   RETVALUE(ROK);
}

/*
 *
 *      Fun:   veS1hoNoOpr
 *
 *      Desc: 
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PRIVATE S16 veS1hoNoOpr
(
VeS1ConCb *s1ConCb,
void      *ptr
)
#else
PRIVATE S16 veS1hoNoOpr(s1ConCb, ptr)
VeS1ConCb *s1ConCb;
void      *ptr;
#endif
{
   TRC2(veS1hoNoOpr);

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, \
            "veS1hoNoOpr: Invalid S1 msg for UE ID: %d.\n", \
            s1ConCb->ueCb->crnti));
#endif

   RETVALUE(ROK);
   
}/* veS1hoNoOpr */

#endif /* End of LTE_HO_SUPPORT */
/********************************************************************30**

         End of file:     ve_s1_ho.c@@/main/eNB_1.3_With_LatestGpr3/10 - Mon Sep  5 17:18:04 2011

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

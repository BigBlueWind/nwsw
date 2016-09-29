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
  
     Desc:     Contains Handover Module's common and utility functions

     Ret :     ROK - success
               RFAILED - failure

     File:     ve_ho.c

     Sid:      ve_ho.c@@/main/eNB_1.3_With_LatestGpr3/20 - Mon Sep 19 16:35:11 2011

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
#include "nhu_asn.h"           /* NHU interface defines */
#include "lnh.h"           /* Layer Manager interface defines */
#include "pju.h"           /* PDCP data interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "lrg.h"           /* MAC RRM control Interface */
#include "szt.h"           /* MAC RRM control Interface */
#include "szt_asn.h"           /* MAC RRM control Interface */

#ifdef LTE_HO_SUPPORT
#include "czt.h"           /* X2AP Upper Interface*/
#include "czt_asn.h"       /* X2AP Upper Interface */
#endif

/* RRM related includes */
#include "ve_eut.h"
#include "egt.h"
#include "ctf.h"
#include "ve.h" 
#include "lve.h" 

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif


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
#include "nhu_asn.x"
#include "lnh.x"           /* LNH Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "lrg.x"           /* MAC RRM control Interface */
#include "szt.x"
#include "szt_asn.x"

#ifdef LTE_HO_SUPPORT
#include "czt.x"           /* X2AP Upper Interface*/
#include "czt_asn.x"       /* X2AP Upper Interface */
#endif

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
#include "ve_sm_init.h"
#include "ve_sm_init.x"

#include "sz.h"
#include "cm_pasn.h"
#include "cm_pasn.x"

EXTERN CmPAsnElmDef szTgeteNB_ToSrceNB_TprntCont;
EXTERN CmPAsnElmDef szSrceNB_ToTgeteNB_TprntCont;

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */
EXTERN VeS1SrcFSM veS1SrcFSM[VE_HO_MAX_STATES][VE_HO_SRC_MAX_EVENTS];
EXTERN VeS1TgtFSM veS1TgtFSM[VE_HO_MAX_STATES][VE_HO_TGT_MAX_EVENTS];
EXTERN VeX2SrcFSM veX2SrcFSM[VE_HO_MAX_STATES][VE_HO_SRC_MAX_EVENTS];
EXTERN VeX2TgtFSM veX2TgtFSM[VE_HO_MAX_STATES][VE_HO_TGT_MAX_EVENTS];
PRIVATE S16 veAbortX2HOProc(VeX2ConCb *x2ConCb);

/* X2_CFG_UPD_CHANGES */
PRIVATE S16 veCzFillCellAddFrmCfgUpdReq ARGS((
CztEvnt     *cfgUpdReq
));

PRIVATE S16 veCzFillCellModFrmCfgUpdReq ARGS((
CztEvnt     *cfgUpdReq
));

PRIVATE S16 veCzFillCellDelFrmCfgUpdReq ARGS((
CztEvnt     *cfgUpdReq
));

PRIVATE S16 veCzFillGuGrpIdAddFrmCfgUpdReq ARGS((
CztEvnt     *cfgUpdReq
));

PRIVATE S16 veCzFillGuGrpIdDelFrmCfgUpdReq ARGS((
CztEvnt     *cfgUpdReq
));
/* X2_CFG_UPD_CHANGES END*/

#ifdef __cplusplus
}
#endif /* __cplusplus */


/*
*
*       Fun:   veHoPrcUeHoAttachTmrExpiry
*
*       Desc:  Process UeHoAttach Timer Expiry
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veHoPrcUeHoAttachTmrExpiry
(
VeUeCb *ueCb
)
#else
PUBLIC S16 veHoPrcUeHoAttachTmrExpiry(ueCb)
VeUeCb *ueCb;
#endif
{
   U8 ueIdx = 0;

   TRC2(veHoPrcUeHoAttachTmrExpiry);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoPrcUeHoAttachTmrExpiry:\
            Triggering processing of veHoPrcUeHoAttachTmrExpiry\n"));
#endif

   /* Release UE Resources . Send UE Release on CTF
    * interface */
   if(NULLP == ueCb)
   {
     VE_DBG_ERROR((VE_PRNT_BUF,"veHoPrcUeHoAttachTmrExpiry:ueCb is NULL."));

      RETVALUE(RFAILED);
   }
   veSndCtfUeRls(ueCb);
   /* RRC ReEst */
   veSndRgrUeRls(ueCb,NULLP,RGR_UE_CFG);
   VeLiNhuCnclUeReq(&(veCb.nhuSap[0]->pst), veCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);
   
   /* Send EGTP release command to EGTP-U*/
   veSndEgtpCom(VE_EGTP_TUN_DEL, ueCb->s1ConCb);

   veCb.s1apConLst[ueCb->s1ConCb->suConnId] = NULLP;
   VE_FREE(ueCb->s1ConCb,sizeof(VeS1ConCb));

   /* if X2 HO is going on then free the X2Cb also */
   if (VE_HO_TYPE_X2 == ueCb->hoType)
   {
      veCb.x2apConLst[VE_GET_UE_X2_IDX(ueCb->x2ConCb->oldEnbUeX2apId)] = NULLP;
      VE_FREE(ueCb->x2ConCb, sizeof(VeX2ConCb));
   }
   
   ueIdx = VE_GET_UE_IDX(ueCb->crnti);
   if(ueIdx < veCb.cellCb[0]->maxUeSupp)
   {
      veCb.cellCb[0]->ueCbLst[ueIdx] = NULLP;
      veCb.cellCb[0]->numUeCfg--;
   }
   VE_FREE(ueCb,sizeof(VeUeCb));

   RETVALUE(ROK);
} /* veHoPrcUeHoAttachTmrExpiry */


/*
*
*       Fun:   veHoSndS1HOCncl
*
*       Desc:  Function to request S1AP to send Handover Cancel to MME
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veHoSndS1HOCncl
(
VeUeCb    *ueCb
)
#else
PUBLIC S16 veHoSndS1HOCncl(ueCb)
VeUeCb    *ueCb;
#endif
{
   S1apPdu      *hoCnclPdu = NULLP;
   SztDatEvntReq datEvt;
   
   TRC2(veHoSndS1HOCncl);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoSndS1HOCncl:\
            Triggering sending of S1 HO Cancel\n"));
#endif

   /* Fill the HO Req Pdu and send to MME */
   if( RFAILED == veUtlSztFillCnclReq(&hoCnclPdu, ueCb->s1ConCb))
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\n Failed to fill Cancel Request\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */
   
   datEvt.spConnId = ueCb->s1ConCb->spConnId;
   datEvt.pdu    =  hoCnclPdu;

   /* Trigger SZT Connectn. Req */

   if (ROK != VeLiSztDatReq(&veCb.sztSap[0]->pst, veCb.sztSap[0]->spId, &datEvt))
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\n Sending of S1 HO CANCEL Failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

} /* veHoSndS1HORqrd */

/*
 *
 *      Fun:   veLiSztHoCmdHandler
 *
 *      Desc:  S1 HO Command Handler
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *          
 *      File:  ve_ho.c 
 *
 */
#ifdef ANSI
PUBLIC S16 veLiSztHoCmdHandler
(
VeS1ConCb *s1ConCb,
S1apPdu   *pdu
)
#else
PUBLIC S16 veLiSztHoCmdHandler(s1ConCb,pdu)
VeS1ConCb *s1ConCb;
S1apPdu   *pdu;
#endif
{
   
   TRC2(veLiSztHoCmdHandler);
   
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztHoCmdHandler:\
           Triggering S1AP HO Command Handler\n"));
#endif
   /* This message is received from MME to inform the source eNB that resources
    * for the handover have been prepared at the target side.*/

   /* Invoke the Source Enode FSM. Ideally the mobCntrlState of the UE is
    * HO_INITITED and the function invoked would be veHoPrcS1HOAck*/ 
   RETVALUE(veS1SrcFSM[s1ConCb->ueCb->mobCtrlState][VE_EVNT_HO_ACK_RCVD](s1ConCb, (void *)pdu));

} /* veLiSztHoCmdHandler */

/*
*
*       Fun:   veLiSztHoPrepFlrHandler
*
*       Desc:  S1 HO Preparation Failure
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztHoPrepFlrHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztHoPrepFlrHandler(s1ConCb,pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{

   TRC2(veLiSztHoPrepFlrHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztHoPrepFlrHandler:\
            Received S1AP HO Prep Failure\n"));
#endif

   /* Invoke the Source Enode FSM. */ 
   RETVALUE(veS1SrcFSM[s1ConCb->ueCb->mobCtrlState][VE_EVNT_HO_PREP_FAILURE]\
         (s1ConCb, (void *)pdu));
} /* veLiSztHoPrepFlrHandler */

/*
*
*       Fun:   veHoSndPathSwtchReq
*
*       Desc:  Send Path Switch Request to MME
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veHoSndPathSwtchReq
(
VeUeCb    *ueCb
)
#else
PUBLIC S16 veHoSndPathSwtchReq(ueCb)
VeUeCb    *ueCb;
#endif
{
   S1apPdu      *pathSwthPdu = NULLP;
   SztDatEvntReq datEvt;

   TRC2(veHoSndPathSwtchReq);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoSndPathSwtchReq:\
            Triggering S1AP Path Switch Request\n"));
#endif

   /* Fill Path Switch structure by invoking the utility function*/
   if( (RFAILED == veUtlSztFillPathSwRqst(ueCb->s1ConCb, &pathSwthPdu)))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoSndPathSwtchReq:\
                       Failed to fill Path switch request\n"));
      RETVALUE(RFAILED);
   } /* end of if statement */

   datEvt.spConnId = ueCb->s1ConCb->suConnId;
   datEvt.pdu    =  pathSwthPdu;

   /* Trigger SZT Connectn. Req */
   VeLiSztDatReq(&veCb.sztSap[0]->pst, veCb.sztSap[0]->spId, &datEvt);

   RETVALUE (ROK);
} /* veHoSndPathSwtchReq */


/*
*
*       Fun:   veHoSndX2HOCncl
*
*       Desc:  Function to request X2AP to send the Handover Cancel to appropriate peer
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veHoSndX2HOCncl
(
VeX2ConCb *x2ConCb
)
#else
PUBLIC S16 veHoSndX2HOCncl(x2ConCb)
VeX2ConCb *x2ConCb;
#endif
{
   S16 ret = ROK;
   CztEvnt *hoCnclPdu = NULLP;
   TRC2(veHoSndX2HOCncl);
   if(NULLP == x2ConCb)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoSndX2HOCncl: Invalid Arguments\n"));
      RETVALUE(RFAILED);
   }

   if(ROK != veUtlCztFillHovrCancel(&hoCnclPdu, x2ConCb, FALSE))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoSndX2HOCncl: Filling of X2 HO CANCEL failed\n"));
      RETVALUE(RFAILED);
   }
   /* filling the peerId */
   hoCnclPdu->peerId = x2ConCb->peerId;
   
   if(ROK != VeLiCztBmpReq (&veCb.cztSap[0]->pst, veCb.cztSap[0]->spId, hoCnclPdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoSndX2HOCncl: Sending of X2 HO CANCEL Failed\n"));
      RETVALUE(RFAILED);
   }
   RETVALUE(ret);
}/* End of veHoSndX2HOCncl */


/*
*
*       Fun:   veHoSndX2SNTfr
*
*       Desc:  Function to request X2AP to send the SN Status Transfer message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veHoSndX2SNTfr
(
VeX2ConCb *x2ConCb,
PTR       pdcpSduInfo
)
#else
PUBLIC S16 veHoSndX2SNTfr(x2ConCb, pdcpSduInfo)
VeX2ConCb *x2ConCb;
PTR       pdcpSduInfo;
#endif
{
   CztEvnt *snStsPdu = NULLP;
   NhuPdcpSduStaCfm *nhuPdcpSduStaCfm = NULLP;

   TRC2(veHoSndX2SNTfr);

   if(NULLP == x2ConCb && NULLP == pdcpSduInfo)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoSndX2SNTfr: Invalid Arguments\n"));
      RETVALUE(RFAILED);
   }
   nhuPdcpSduStaCfm = (NhuPdcpSduStaCfm *)pdcpSduInfo;
   if(ROK != veUtlCztFillSnStatusTrfr(&snStsPdu, x2ConCb, nhuPdcpSduStaCfm->numRbId, \
         nhuPdcpSduStaCfm->pdcpSduStaInfo))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoSndX2SNTfr: Filling of X2 SN Status failed\n"));
      RETVALUE(RFAILED);
   }

   /* filling the peerId */
   snStsPdu->peerId = x2ConCb->peerId;
   VeLiCztBmpReq (&veCb.cztSap[0]->pst, veCb.cztSap[0]->spId, snStsPdu);

   RETVALUE(ROK);
}/* End of veHoSndX2SNTfr */


/*
*
*       Fun:   veHoSndS1SNTfr
*
*       Desc:  Function to request X2AP to send the SN Status Transfer message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veHoSndS1SNTfr
(
VeS1ConCb        *s1ConCb,
NhuPdcpSduStaCfm *nhuPdcpSduStaCfm
)
#else
PUBLIC S16 veHoSndS1SNTfr(s1ConCb, nhuPdcpSduStaCfm)
VeS1ConCb       *s1ConCb;
NhuPdcpSduStaCfm *nhuPdcpSduStaCfm;
#endif
{
   S1apPdu     *enbSnPdu = NULLP;
   SztDatEvntReq datEvt;

   TRC2(veHoSndS1SNTfr);

   if(ROK != veUtlSztFillENBStatusTfr(s1ConCb, &enbSnPdu, \
            nhuPdcpSduStaCfm->numRbId, nhuPdcpSduStaCfm))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoSndS1SNTfr: Filling of S1 ENB SN Status failed\n"));
      RETVALUE(RFAILED);
   }

   datEvt.spConnId = s1ConCb->spConnId;
   datEvt.pdu    =  enbSnPdu;

   VeLiSztDatReq(&veCb.sztSap[0]->pst, veCb.sztSap[0]->spId, &datEvt);
   RETVALUE(ROK);
}/* End of veHoSndS1SNTfr */


/*
*
*       Fun:   veLiSztHoRqstHandler
*
*       Desc:  S1 HO Request Handler
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztHoRqstHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztHoRqstHandler(s1ConCb,pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{
   VeCellCb *cellCb = NULLP;
   VeUeCb *ueCb = NULLP;
   U8 ueIdx = 0;
   U16 ret = 0;
   U16 numComp = 0;
   U8 suConId = 0;

   NhuCfgReqSdus *nhuCfgReqSdus = NULLP;
   Pst                       *pst = NULLP;
   SztE_RABToBeSetupLstHOReq *eRabToSetup = NULLP;
   SztMME_UE_S1AP_ID *mmeUeS1apId = NULLP;
   SztUEAggMaxBitrate *ueAggMaxBtRt = NULLP;

   TRC2(veLiSztHoRqstHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztHoRqstHandler:\
            Triggering S1AP HO Request\n"));
#endif

   /*
    * Start of Comment.
    * The purpose of this procedure is to reserve necessary resorces at the 
    * target eNB for the Handover of UE. Admission control is performed by 
    * target eNB depends upon received E-RAB QOS information and other factors.
    * If admission is sucess, the Target eNB configures the required resources 
    * according to the QOS information of requested bearers and reserves C-RNTI 
    * and RACH preamble. After all necessary resorces for admitted Bearer has 
    * been allocated the target eNB shall generate Handover Request Ack message 
    * towards MME otherwise, Shall send Failure message.
    * End of Comment.
    */

   VE_GET_S1AP_CON_ID(suConId);
   if(((ret = veAdmnCntrlReq()) != ROK) && (suConId >= VE_MAX_S1AP_CON))
   {
      /* Admission control failed. Build Handover preparation
       * failure message and send to MME */
#ifdef DEBUGP
      VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztHoRqstHandler:\
               Admission Control failed\n"));
#endif
      if( (ret = veS1TgtFSM[ueCb->mobCtrlState][VE_EVNT_HO_ADM_FAILURE](s1ConCb, (void *)NULLP)) != ROK)
      {
#ifdef DEBUGP
         VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztHoRqstHandler:\
                  Preparation of Handover Prep Failure message failed\n"));
#endif
      }
   }
   else
   {
      /* 
       * Start of Comment.
       * Admission is success. Target eNB is ready to allocate necessary
       * resorces to UE.
       * End of Comment.
       */
      /*Create UE control block*/
      VE_ALLOC(&ueCb, sizeof(VeUeCb));

      if(NULLP == ueCb)
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztHoRqstHandler:Failed to Allocate Memory. \n"));
 
         RETVALUE(ROK);
      }

      /* Reserve C-RNTI to the UE*/
      ret =  veGetDedCrnti(&ueCb->crnti);
      if(ROK != ret)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztHoRqstHandler:Failed to Generate C-RNTI \n"));
         VE_FREE(ueCb, sizeof(VeUeCb));
         RETVALUE(ROK);
#endif
      }

      /*Assign cell id which is serving cell of UE. */
      ueCb->cellId = veCb.cellCb[0]->cellId;

      /* Get the UE Index.*/
      cellCb = veCb.cellCb[0];
      ueIdx = VE_GET_UE_IDX(ueCb->crnti);
      if(ueIdx < cellCb->maxUeSupp)
      {
         /* place the ueCb in the ueLst in cellCb */
         cellCb->ueCbLst[ueIdx] = ueCb;
         cellCb->numUeCfg++;
         ueCb->s1ConCb = s1ConCb;

         /* update s1ConCb */
         s1ConCb->ueCb = ueCb;
         s1ConCb->suConnId = suConId;

         /* place the s1ConCb in s1apConLst */
         veCb.s1apConLst[s1ConCb->suConnId] = s1ConCb;
      }
      else
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztHoRqstHandler:\
                  UE supported maxxed out(%d)\n", ueIdx));
         /* Free UE control block */
         VE_FREE(ueCb, sizeof(VeUeCb));
         /* Free S1 control block */
         VE_FREE(s1ConCb, sizeof(VeS1ConCb));
         RETVALUE(RFAILED);
      }
      /* Fill the State Variable of UE.*/
      ueCb->rrcConnState = VE_RRC_CONNECTING;
      ueCb->mobCtrlState = VE_HO_INITIATED;

      /* Update SRB1 related information in the UeCb.*/
#ifdef VE_SRB2_SUPPORT
      ret = veInitUeSrbInfo(&ueCb->rbInfo,VE_PDCP_ID_SRB1);
#else
      ret = veInitUeSrbInfo(&ueCb->rbInfo);
#endif
      if(ROK != ret)
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztHoRqstHandler:\
                  Failed to update SRB Info\n"));
         /* Free UE control block */
         VE_FREE(ueCb, sizeof(VeUeCb));
         cellCb->ueCbLst[ueIdx] = NULLP;
         veCb.cellCb[0]->numUeCfg--;

         /* Free S1 control block */
         VE_FREE(s1ConCb, sizeof(VeS1ConCb));

         RETVALUE(RFAILED);
      }

      /* Update SRB2 related information in the UeCb.*/
#ifdef VE_SRB2_SUPPORT
      ret = veInitUeSrbInfo(&ueCb->rbInfo,VE_PDCP_ID_SRB2);
      if(ROK != ret)
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztHoRqstHandler:\
                  Failed to update SRB Info\n"));
         /* Free UE control block */
         VE_FREE(ueCb, sizeof(VeUeCb));
         cellCb->ueCbLst[ueIdx] = NULLP;
         /* Free S1 control block */
         VE_FREE(s1ConCb, sizeof(VeS1ConCb));
         veCb.s1apConLst[s1ConCb->suConnId] = NULLP;
         RETVALUE(RFAILED);
      }
#endif

      /* Update the UeCb which is present in veCb according to Handover Req.*/
      if(PRSNT_NODEF == pdu->pdu.val.initiatingMsg.pres.pres)
      {
         if(PRSNT_NODEF == pdu->pdu.val.initiatingMsg.value.u.sztHovrRqst.pres.pres)
         {
            numComp = pdu->pdu.val.initiatingMsg.value.u.sztHovrRqst.protocolIEs.noComp.val;
            veSzGetIE(&pdu->pdu, Sztid_MME_UE_S1AP_ID, (TknU8**)&mmeUeS1apId);
            /* Fill mme-ue-s1ap-id. */
            ueCb->s1ConCb->mme_ue_s1ap_id = mmeUeS1apId->val;

            /* Update UeCb with UE Aggregate Maximum Bit Rate */
            veSzGetIE(&pdu->pdu, Sztid_uEaggregateMaxBitrate, (TknU8**)&ueAggMaxBtRt);
            if(PRSNT_NODEF == ueAggMaxBtRt->pres.val)
            {
               VE_GET_U32_FRM_OSXL(ueCb->rbInfo.aggBr.dl, \
                     ueAggMaxBtRt->uEaggregateMaxBitRateDL);

               VE_GET_U32_FRM_OSXL(ueCb->rbInfo.aggBr.ul, \
                     ueAggMaxBtRt->uEaggregateMaxBitRateUL);
            }
            veSzGetIE(&pdu->pdu, Sztid_E_RABToBeSetupLstHOReq, (TknU8**)&eRabToSetup);
            ret = veFillErabFrmS1HoReq(&ueCb->rbInfo, eRabToSetup, s1ConCb->mmeCb);

            if(ROK != ret)
            {
               VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztHoRqstHandler:\
                        Failed to update SRB Info\n"));
               /* Free UE control block */
               VE_FREE(ueCb, sizeof(VeUeCb));
               cellCb->ueCbLst[ueIdx] = NULLP;
               /* Free S1 control block */
               veCb.s1apConLst[s1ConCb->suConnId] = NULLP;
               VE_FREE(s1ConCb, sizeof(VeS1ConCb));
               RETVALUE(RFAILED);
            }
         }
      }

      /* Allocate memory for event structure.*/
      VE_ALLOCEVNT(&nhuCfgReqSdus, sizeof(NhuCfgReqSdus));
      /* Fill Event Handler Information.*/
      /* selva IT-FIX*/
    /*  veFillNhuEvntHdr(&(nhuCfgReqSdus->hdr), ueCb->cellId, ueCb->crnti, VE_TRANSID_UE_CFG);*/
      veFillNhuEvntHdr(&(nhuCfgReqSdus->hdr), ueCb->cellId, 0, VE_TRANSID_UE_CFG);

      /*
       * Start of Comment.
       * Fill UE Configuration Information which is used to configure all
       * lower layer.
       * End of Comment.
       */
      ret = veFillNhuUeCfgInfo(nhuCfgReqSdus, &nhuCfgReqSdus->cfgInfo, ueCb, VE_TRANSID_UE_CFG);
      if(ROK != ret)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztHoRqstHandler:\
                  Failed to update SRB Info\n"));
#endif
         /* Free UE control block */
         VE_FREE(ueCb, sizeof(VeUeCb));
         cellCb->ueCbLst[ueIdx] = NULLP;
         /* Free S1 control block */
         VE_FREE(s1ConCb, sizeof(VeS1ConCb));
         veCb.s1apConLst[s1ConCb->suConnId] = NULLP;
         VE_FREE(nhuCfgReqSdus, sizeof(NhuCfgReqSdus));
         RETVALUE(RFAILED);
      }
      /* Send the UE configuration to RRC */
      pst = &veCb.nhuSap[0]->pst;
      pst->event   = EVTNHUCFGREQ;
       /* Set the appropriate state */
       ueCb->lowLyrCfgSts = VE_LOW_LYR_CFG_IN_PRGS;

      if (ROK != VeLiNhuCfgReq(pst, veCb.nhuSap[0]->spId, nhuCfgReqSdus))
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztHoRqstHandler:\
                  Failed to send Ue Cfg Info to RRC\n"));
#endif
         /* Free UE control block */
         VE_FREE(ueCb, sizeof(VeUeCb));
         cellCb->ueCbLst[ueIdx] = NULLP;
         /* Free S1 control block */
         VE_FREE(s1ConCb, sizeof(VeS1ConCb));
         veCb.s1apConLst[s1ConCb->suConnId] = NULLP;
         VE_FREE(nhuCfgReqSdus, sizeof(NhuCfgReqSdus));
         RETVALUE(RFAILED);
      }
      /* Update the appropriate HO Type in the UeCb */
      ueCb->hoType = VE_HO_TYPE_S1;
      /* The flow is continued from the Config Cfm receotion from the RRC */
   }
   RETVALUE(ROK);
} /* veLiSztHoRqstHandler*/


/*
*
*       Fun:   veLiSztHoPthSwthReqAckHandler
*
*       Desc:  S1 HO path Switch Request Ack
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztHoPthSwthReqAckHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztHoPthSwthReqAckHandler(s1ConCb,pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{

   TRC2(veLiSztHoPthSwthReqAckHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztHoPthSwthReqAckHandler:\
            Triggering S1AP HO Path Switch Request Ack\n"));
#endif

   /* update ueCb with UE Max Aggr Bit Rate */
   /* store sec context in ueCb and will be used for next HO (intra eNB) */
   /* updat UL tunnel id if specified */
   veUpdUlTunPthSwtAck(&s1ConCb->ueCb->rbInfo, &pdu->pdu);
   /* trigger rrc reconfig to release the bearers, if specified in release list */

   /* invoke x2 UE release */
   veSndX2UeCtxtRls(s1ConCb->ueCb->x2ConCb, NULLP);

   /* Setting the UE paramenters to Default. This is valid only in 
    * case of X2 Handover. For S1 Handover, these values are set upon
    * reception of RRC Reconfig Complete */
   s1ConCb->ueCb->hoType = VE_HO_TYPE_NONE;
   s1ConCb->ueCb->mobCtrlState = VE_HO_NORMAL;

   RETVALUE(ROK);
} /* veLiSztHoPthSwthReqAckHandler*/

/*
*
*       Fun:  veLiSztHoPthSwthReqFlrHandler 
*
*       Desc:  S1 HO path Switch Request Failure
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztHoPthSwthReqFlrHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztHoPthSwthReqFlrHandler(s1ConCb,pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{

   TRC2(veLiSztHoPthSwthReqFlrHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztHoPthSwthReqFlrHandler:\
            Triggering S1AP HO Path Switch Request Failure\n"));
#endif

   RETVALUE(veS1TgtFSM[s1ConCb->ueCb->mobCtrlState][VE_EVNT_HO_PATH_SWTH_FAILED]\
                       (s1ConCb, pdu));
} /* veLiSztHoPthSwthReqFlrHandler*/

/*
*
*       Fun:  veLiSztCnclAckHandler
*
*       Desc:  S1 HO Cancel Ack
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztCnclAckHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *pdu
)
#else
PUBLIC S16 veLiSztCnclAckHandler(s1ConCb,pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *pdu;
#endif
{

   TRC2(veLiSztCnclAckHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztCnclAckHandler:\
            S1 HO CANCEL Procedure completed Successfully\n"));
#endif
  /* Free the received PDU */
  cmFreeMem (pdu);
  RETVALUE(ROK);
} /* veLiSztCnclAckHandler */



/*
*
*       Fun:  veLiSztMmeStsTfrHandler
*
*       Desc:  S1 MME Status Transfer Handler
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiSztMmeStsTfrHandler
(
VeS1ConCb *s1ConCb,
S1apPdu  *s1pdu
)
#else
PUBLIC S16 veLiSztMmeStsTfrHandler(s1ConCb, s1pdu)
VeS1ConCb *s1ConCb;
S1apPdu  *s1pdu;
#endif
{
   U16           noComp = 0;
   U16           loopCnt = 0;
   VeSnStsInfo   snStsInfo;
   VeSnSts       *snSts = NULLP;
   NhuHoPdcpSduInfo *nhuHoPdcpSduInfo = NULLP;
   SztProtIE_Field_MMEStatusTfrIEs *mmeStsTfrInfo = NULLP;
   SztProtIE_SingleCont_Brs_SubjToStatusTfr_ItemIEs *snForeRAB = NULLP;
   U16 numbytes = 0;
   TRC2(veLiSztMmeStsTfrHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiSztMmeStsTfrHandler:\
            Processing S1AP MME Status Transfer\n"));
#endif

   /* the eNB status Trasnfer transparent container is received as a
    * part of MME Status Transfer message */
   
   /* Retrieve the SN status from the CZT Indication */
   if(TRUE == s1pdu->pdu.val.initiatingMsg.pres.pres)
   {
      if (TRUE == s1pdu->pdu.val.initiatingMsg.value.u.sztMMEDirectInformTfr.pres.pres)
      {
         cmMemset((U8 *)&snStsInfo, 0, sizeof(VeSnStsInfo));
         mmeStsTfrInfo = &s1pdu->pdu.val.initiatingMsg.value.u.sztMMEStatusTfr.protocolIEs.member[2];
         noComp = mmeStsTfrInfo->value.u.sztENB_StatusTfr_TprntCont.bearers_SubjToStatusTfrLst.noComp.val;
         snStsInfo.numOfRb = noComp;
         for (loopCnt = 0; loopCnt < noComp; loopCnt++)
         {
            snForeRAB = &mmeStsTfrInfo->value.u.sztENB_StatusTfr_TprntCont.bearers_SubjToStatusTfrLst.member[loopCnt];
            snSts = &snStsInfo.veSnSts[loopCnt];
            /* selva MSPD FIX */
            snSts->rbId = VE_GET_RB_ID_FRM_ERAB(snForeRAB->value.u.sztBrs_SubjToStatusTfr_Item.e_RAB_ID.val);
            nhuHoPdcpSduInfo = &snSts->nhuHoPdcpSduInfo;
            if (TRUE == snForeRAB->value.u.sztBrs_SubjToStatusTfr_Item.dL_COUNTvalue.pres.pres)
            {
               /* Initializing and updating the bitmap to indicate the presence of DL COUNT */
               nhuHoPdcpSduInfo->isHoPres = 0x00 | NHU_HO_DL;
               VE_CALC_COUNT(NHU_PDCP_SN_SIZE_7BITS, snForeRAB->value.u.sztBrs_SubjToStatusTfr_Item.dL_COUNTvalue.pDCP_SN.val, 
                  snForeRAB->value.u.sztBrs_SubjToStatusTfr_Item.dL_COUNTvalue.hFN.val, nhuHoPdcpSduInfo->dlCountVal);  
            }
            if (TRUE == snForeRAB->value.u.sztBrs_SubjToStatusTfr_Item.uL_COUNTvalue.pres.pres)
            {
               /* Initializing and updating the bitmap to indicate the presence of UL COUNT */
               nhuHoPdcpSduInfo->isHoPres = 0x00 | NHU_HO_UL;
               VE_CALC_COUNT(NHU_PDCP_SN_SIZE_7BITS, snForeRAB->value.u.sztBrs_SubjToStatusTfr_Item.uL_COUNTvalue.pDCP_SN.val,
                  snForeRAB->value.u.sztBrs_SubjToStatusTfr_Item.uL_COUNTvalue.hFN.val, nhuHoPdcpSduInfo->dlCountVal);  
            }
               /* selva MSPD FIX */
            /* UL Bit Map informations  */
            if (TRUE == snForeRAB->value.u.sztBrs_SubjToStatusTfr_Item.receiveStatusofULPDCPSDUs.pres)
            {
                nhuHoPdcpSduInfo->numBits = \
                snForeRAB->value.u.sztBrs_SubjToStatusTfr_Item.receiveStatusofULPDCPSDUs.len;
                numbytes = (nhuHoPdcpSduInfo->numBits > 8)?(nhuHoPdcpSduInfo->numBits/8):1;
                if(nhuHoPdcpSduInfo->numBits % 8)
                numbytes++;
                VE_ALLOC(&nhuHoPdcpSduInfo->ulBitMap, numbytes);
                cmMemset((U8*)nhuHoPdcpSduInfo->ulBitMap, 0, numbytes);
                cmMemcpy((U8*)nhuHoPdcpSduInfo->ulBitMap,\
                   (U8*)snForeRAB->value.u.sztBrs_SubjToStatusTfr_Item.receiveStatusofULPDCPSDUs.val,\
                    numbytes);
                 
            }
         }
      }/* End of if () */
      else
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztMmeStsTfrHandler:\
                  MME Status Transfer(sztMMEDirectInformTfr) IE is not present\
                  in the received Message \n"));
         RETVALUE(RFAILED);
      }

   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiSztMmeStsTfrHandler:\
               MME Status Transfer(initiatingMsg) is not present in the received Message \n"));
      RETVALUE(RFAILED);
   }
   /* Send PDCP SN Status to PDCP through RRC */
   vePdcpSnStsUpd(s1ConCb->ueCb, &snStsInfo);
   RETVALUE(ROK);
} /* veLiSztMmeStsTfrHandler */

/*
*
*       Fun:  veLiCztHoRqstHandler
*
*       Desc:  X2 HO Request Handler
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiCztHoRqstHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *cztInd
)
#else
PUBLIC S16 veLiCztHoRqstHandler(x2ConCb, cztInd)
VeX2ConCb *x2ConCb;
CztEvnt   *cztInd;
#endif
{
   VeCellCb *cellCb = NULLP;
   VeUeCb *ueCb = NULLP;
   U8 ueIdx = 0;
   U16  ret =0;
   U16  retVal =0;
   NhuCfgReqSdus *nhuCfgReqSdus = NULLP;
   CztProtIE_Field_HovrRqst_IEs *hoReq = NULLP;
   U16 numComp = 0;
   Pst *pst = NULLP;
   VeNghCellCb *nghCellCb = NULLP;
   VeS1ConCb *s1ConCb = NULLP;
   VePlmnId  tmpPlmnId;
   TknU8     *gummeId = NULLP;

   TRC2(veLiCztHoRqstHandler);
   printf ("\n-------------------HO REQ Has been RCVD-----------------\n");
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztHoRqstHandler:\
            Triggering X2AP HO Request\n"));
#endif
   /*
    * The purpose of this procedure is to reserve necessary resorces at the 
    * target eNB for the Handover of UE. Admission control is performed by 
    * target eNB depends upon received E-RAB QOS information and other factors.
    * If admission is sucess, the Target eNB configures the required resources 
    * according to the QOS information of requested bearers and reserves C-RNTI 
    * and RACH preamble. After all necessary resorces for admitted Bearer has 
    * been allocated the target eNB shall generate Handover Request Ack message 
    * towards target eNB otherwise, Shall send Failure message.
    */

   if( (ret = veAdmnCntrlReq()) != ROK)
   {
      /* Admission control failed. Build Handover preparation
       * failure message and send to MME */
#ifdef DEBUGP
      VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztHoRqstHandler:\
               Admission Control failed\n"));
#endif
      if( (ret = veX2TgtFSM[ueCb->mobCtrlState][VE_EVNT_HO_ADM_FAILURE](x2ConCb,(void *)NULLP)) != ROK)
      {
#ifdef DEBUGP
         VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztHoRqstHandler:\
                  Preparation of Handover Prep Failure message failed\n"));
#endif
      }
   }
   else
   {
      /* Admission control sucess. Prepare UE context references and resorces */
      /* Build Handover request Ack message and send to MME */

      /*Create UE control block*/
      VE_ALLOC(&ueCb, sizeof(VeUeCb));

      if(NULLP == ueCb)
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiCztHoRqstHandler:\
                  Failed to allocate Memory.\n"));
         RETVALUE(RFAILED);
      }

      /* assign cell id from pdu */

      ueCb->cellId = veCb.cellCb[0]->cellId;

      if (ROK != veGetDedCrnti(&ueCb->crnti))
      {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiCztHoRqstHandler:\
         Dedicated RNTI is not available : %d\n",__LINE__));
      RETVALUE(RFAILED);
#endif
      }
      printf("\n************Dedicated CRNTI value:%d********\n",ueCb->crnti);
      ueCb->rrcConnState = VE_RRC_CONNECTING;
      ueCb->mobCtrlState = VE_HO_INITIATED;

      cellCb = veCb.cellCb[0];
      ueIdx = VE_GET_UE_IDX(ueCb->crnti);

      if(ueIdx < cellCb->maxUeSupp)
      {
         cellCb->ueCbLst[ueIdx] = ueCb;
         cellCb->numUeCfg++;
         x2ConCb->ueCb = ueCb;
         /* SELVA IT-FIX */
         ueCb->x2ConCb = x2ConCb;
         veCb.x2apConLst[VE_GET_UE_X2_IDX(x2ConCb->oldEnbUeX2apId)] = x2ConCb;

         VE_ALLOC(&s1ConCb, sizeof(VeS1ConCb));

         if(NULLP == s1ConCb)
         {
            VE_DBG_ERROR((VE_PRNT_BUF,"veLiCztHoRqstHandler:\
                  Failed to allocate Memory.\n"));
            RETVALUE(RFAILED);
         }

         /* allocate s1ConCb */
         ueCb->s1ConCb = s1ConCb;

         /* Fill s1ConCb */
         s1ConCb->ueCb = ueCb;
         VE_GET_S1AP_CON_ID(s1ConCb->suConnId);
         s1ConCb->s1apConnState = VE_S1AP_CONNECTING;

         veUtlGetCztIE(&(cztInd->pdu), Cztid_GUMMEI_ID, &gummeId);
         if(gummeId == NULLP)
         {
            /* Select the default mmeCb at present */
            s1ConCb->mmeCb = veCb.mmeCont.mmes[0];
         }
         else
         {
            veUtlSelMmeCb(gummeId, &(s1ConCb->mmeCb));
            if(s1ConCb->mmeCb == NULLP)
            {
               s1ConCb->mmeCb = veCb.mmeCont.mmes[0];
            }
         }
         s1ConCb->peerId = s1ConCb->mmeCb->mmeId;

         /* place the s1ConCb in s1apConLst */
         veCb.s1apConLst[s1ConCb->suConnId] = s1ConCb;

      }
      else
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiCztHoRqstHandler:\
                  UE supported maxxed out(%d)\n", ueIdx));
         veX2TgtFSM[ueCb->mobCtrlState][VE_EVNT_HO_ADM_FAILURE](x2ConCb, (void *)NULLP);
         /* Free UE control block */
         VE_FREE(ueCb, sizeof(VeUeCb));
         cellCb->ueCbLst[ueIdx] = NULLP;
         cellCb->numUeCfg--;
         /* Free S1 control block */
         VE_FREE(s1ConCb, sizeof(VeS1ConCb));
         veCb.s1apConLst[s1ConCb->suConnId] = NULLP;
         /* Free X2 control block */
         veCb.x2apConLst[VE_GET_UE_X2_IDX(x2ConCb->oldEnbUeX2apId)] = NULLP;
         VE_FREE(x2ConCb, sizeof(VeX2ConCb));
         RETVALUE(ROK);
      }
      /* Update SRB1 related information in the UeCb.*/
#ifdef VE_SRB2_SUPPORT
      ret = veInitUeSrbInfo(&ueCb->rbInfo,VE_PDCP_ID_SRB1);
#else
      ret = veInitUeSrbInfo(&ueCb->rbInfo);
#endif
      if(ROK != ret)
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiCztHoRqstHandler:\
                  Failed to update SRB Info\n"));
         /* Free UE control block */
         VE_FREE(ueCb, sizeof(VeUeCb));
         cellCb->ueCbLst[ueIdx] = NULLP;
         /* Free S1 control block */
         VE_FREE(s1ConCb, sizeof(VeS1ConCb));
         veCb.s1apConLst[s1ConCb->suConnId] = NULLP;
         /* Free X2 control block */
         VE_FREE(x2ConCb, sizeof(VeX2ConCb));
         veCb.x2apConLst[VE_GET_UE_X2_IDX(x2ConCb->oldEnbUeX2apId)] = NULLP;
         RETVALUE(RFAILED);
      }

      /* Update SRB2 related information in the UeCb.*/
#ifdef VE_SRB2_SUPPORT
      ret = veInitUeSrbInfo(&ueCb->rbInfo,VE_PDCP_ID_SRB2);
      if(ROK != ret)
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiCztHoRqstHandler:\
                  Failed to update SRB Info\n"));
         /* Free UE control block */
         VE_FREE(ueCb, sizeof(VeUeCb));
         RETVALUE(RFAILED);
      }
#endif
      /* Update the UeCb which is present in veCb according to Handover Req.*/
      if(PRSNT_NODEF == cztInd->pdu.val.initiatingMsg.pres.pres)
      {
         if(PRSNT_NODEF == cztInd->pdu.val.initiatingMsg.value.u.cztHovrRqst.pres.pres)
         {
            /*numComp = cztInd->pdu.val.initiatingMsg.value.u.cztHovrRqst.protocolIEs.noComp.val;*/
            hoReq = &(cztInd->pdu.val.initiatingMsg.
                  value.u.cztHovrRqst.protocolIEs.member[numComp]);
            ueCb->x2ConCb->oldEnbUeX2apId = hoReq->value.u.cztUE_X2AP_ID.val;
            /* retrieve the UEContext IE wich is present as 4th element 
               skip the following elements cztCause,cztECGI,cztGUMMEI*/
            /* TODO this logic to retrieve the required member from the union has to be changed
               once the encoder fills the appropriate values in the parent structure's pres field */
            numComp += 4;   
            hoReq = &(cztInd->pdu.val.initiatingMsg.
                  value.u.cztHovrRqst.protocolIEs.member[numComp]);
               
            /* Update UeCb with UE Aggregate Maximum Bit Rate */
            if(PRSNT_NODEF == hoReq->value.u.cztUE_CntxtInform.pres.pres)
            {
               VE_GET_U32_FRM_OSXL(ueCb->rbInfo.aggBr.dl, \
                     hoReq->value.u.cztUE_CntxtInform.uEaggregateMaxBitRate.uEaggregateMaxBitRateDlnk);

               VE_GET_U32_FRM_OSXL(ueCb->rbInfo.aggBr.ul, \
                     hoReq->value.u.cztUE_CntxtInform.uEaggregateMaxBitRate.uEaggregateMaxBitRateUlnk);
            }
            /* fetch the neighbor cell from which the req was received */
            veHOFndInNghLstByPeerId(veCb.cellCb[0], cztInd->peerId, &nghCellCb);
            /* Retrieve and store the source MMEUES1AP Id into the UeCb */
            ueCb->src_mme_ue_s1ap_id = hoReq->value.u.cztUE_CntxtInform.mME_UE_S1AP_ID.val;
            ret = veFillErabFrmX2HoReq(&ueCb->rbInfo, &hoReq->value.u.cztUE_CntxtInform.e_RABs_ToBeSetup_Lst, \
                  nghCellCb, s1ConCb->mmeCb);

            if(ROK != ret)
            {
               VE_DBG_ERROR((VE_PRNT_BUF,"veLiCztHoRqstHandler:\
                  Failed to update SRB Info\n"));
               /* Free UE control block */
               VE_FREE(ueCb, sizeof(VeUeCb));
               RETVALUE(RFAILED);
            }
         }
      }
      /* Allocate memory for event structure.*/
      VE_ALLOCEVNT(&nhuCfgReqSdus, sizeof(NhuCfgReqSdus));
     
      if(NULLP == nhuCfgReqSdus)
      {
         printf("\n Failed to allocate the memmory.\n");
         RETVALUE(RFAILED);
      }
      /* Fill Event Handler Information.*/
      ret = veFillNhuEvntHdr(&(nhuCfgReqSdus->hdr), ueCb->cellId, 0,VE_TRANSID_UE_CFG);

      if(ROK != ret)
      {
         printf("\n Failed to Fill veFillNhuEvntHdr\n");
         RETVALUE(RFAILED);
      }

      /*
       * Fill UE Configuration Information which is used to configure all
       * lower layer.
       */
      ret = veFillNhuUeCfgInfo(nhuCfgReqSdus, &nhuCfgReqSdus->cfgInfo, ueCb, VE_TRANSID_UE_CFG);
      if(ROK != ret)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiCztHoRqstHandler:\
                  Failed to update SRB Info\n"));
         /* Free UE control block */
         VE_FREE(ueCb, sizeof(VeUeCb));
         VE_FREE(nhuCfgReqSdus, sizeof(NhuCfgReqSdus));
         RETVALUE(RFAILED);
#endif
      }
      /* Send the UE configuration to RRC */
      pst = &veCb.nhuSap[0]->pst;

      if(NULLP == pst)
      {
         printf("\n PST is NULL \n");
         RETVALUE(RFAILED);
      }
      pst->event   = EVTNHUCFGREQ;

      /* update the configuration status of lower layers */
      if(ueCb->lowLyrCfgSts == VE_LOW_LYR_CFG_NOT_STRTD && \
            ueCb->lowLyrCfgSts != VE_LOW_LYR_CFG_REM)
      {
         ueCb->lowLyrCfgSts = VE_LOW_LYR_CFG_IN_PRGS;
      }
      else
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiCztHoRqstHandler:\
                  Failed to send Ue Cfg Info to RRC\n"));
#endif
         VE_FREE(nhuCfgReqSdus, sizeof(NhuCfgReqSdus));
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

         /* remove ueCb from cellCb */
         ueIdx = VE_GET_UE_IDX(ueCb->crnti);
         if(ueIdx < veCb.cellCb[0]->maxUeSupp)
         {
            veCb.cellCb[0]->ueCbLst[ueIdx] = NULLP;
            veCb.cellCb[0]->numUeCfg--;
         }
         VE_FREE(ueCb,sizeof(VeUeCb));
         RETVALUE(RFAILED);
       }
       /* Set the appropriate state */
       ueCb->lowLyrCfgSts = VE_LOW_LYR_CFG_IN_PRGS;

      if (ROK != VeLiNhuCfgReq(pst, veCb.nhuSap[0]->spId, nhuCfgReqSdus))
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiCztHoRqstHandler:\
                  Failed to send Ue Cfg Info to RRC\n"));
#endif
         /* Free UE control block */
         VE_FREE(ueCb, sizeof(VeUeCb));
         VE_FREE(nhuCfgReqSdus, sizeof(NhuCfgReqSdus));
         RETVALUE(RFAILED);
      }

      /* Update the appropriate HO Type in the UeCb */
      ueCb->hoType = VE_HO_TYPE_X2;
      /* The flow is continued from the Config Cfm receotion from the RRC */
   }

   RETVALUE(retVal);
} /* veLiCztHoRqstHandler */

/*
*
*       Fun:  veLiCztHoAckHandler
*
*       Desc:  Process the X2 HO Ack Handler received from Target enodeb
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiCztHoAckHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *cztInd
)
#else
PUBLIC S16 veLiCztHoAckHandler(x2ConCb,cztInd)
VeX2ConCb *x2ConCb;
CztEvnt   *cztInd;
#endif
{

   TRC2(veLiCztHoAckHandler);
   printf("\n----HO ACK RCVD------\n");
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztHoAckHandler:\
               Triggering X2AP HO Ack\n"));
#endif

   RETVALUE(veX2SrcFSM[x2ConCb->ueCb->mobCtrlState][VE_EVNT_HO_ACK_RCVD]
          (x2ConCb, (void *)cztInd));

} /* veLiCztHoAckHandler */


/*
*
*       Fun:  veLiCztHoFlrHandler
*
*       Desc:  X2 HO Failure Handler
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiCztHoFlrHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *cztInd
)
#else
PUBLIC S16 veLiCztHoFlrHandler(x2ConCb,cztInd)
VeX2ConCb *x2ConCb;
CztEvnt   *cztInd;
#endif
{

   TRC2(veLiCztHoFlrHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztHoFlrHandler:\
            Triggering X2AP HO Failure \n"));
#endif

   RETVALUE(veX2SrcFSM[x2ConCb->ueCb->mobCtrlState][VE_EVNT_HO_PREP_FAILURE]
          (x2ConCb, (void *)cztInd));

} /* veLiCztHoFlrHandler */




/*
*
*       Fun:  veLiCztHoCnclHandler
*
*       Desc:  X2 HO Cancel Handler
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiCztHoCnclHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *cztInd
)
#else
PUBLIC S16 veLiCztHoCnclHandler(x2ConCb, cztInd)
VeX2ConCb *x2ConCb;
CztEvnt   *cztInd;
#endif
{

   TRC2(veLiCztHoCnclHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztHoCnclHandler:\
            X2AP HO Cancel Received\n"));
#endif
   RETVALUE(veX2TgtFSM[x2ConCb->ueCb->mobCtrlState][VE_EVNT_HO_CANCEL_RCVD]
          (x2ConCb, (void *)cztInd));

} /* veLiCztHoCnclHandler*/

/*
*
*       Fun:  veLiCztSnStsTrsfrHandler
*
*       Desc:  X2 HO SN Status Transfer
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiCztSnStsTrsfrHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *cztInd
)
#else
PUBLIC S16 veLiCztSnStsTrsfrHandler(x2ConCb, cztInd)
VeX2ConCb *x2ConCb;
CztEvnt   *cztInd;
#endif
{
   U16           noComp = 0;
   U16           loopCnt = 0;
   VeSnStsInfo   snStsInfo;
   VeSnSts       *snSts = NULLP;
   NhuHoPdcpSduInfo *nhuHoPdcpSduInfo = NULLP;
   CztE_RABs_SubjToStatusTfr_Lst *cztsnStatusLst = NULLP; 
   CztProtIE_Field_SNStatusTfr_IEs *snTfrIe = NULLP;
   CztProtIE_Single_Cont_E_RABs_SubjToStatusTfr_ItemIEs *snStatusForERAB = NULLP;
   U16 numbytes = 0;
   TRC2(veLiCztSnStsTrsfrHandler);
   cmMemset((U8 *)&snStsInfo, 0, (sizeof(VeSnStsInfo)));
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztSnStsTrsfrHandler:\
            Triggering X2AP SN Status Transfer\n"));
#endif
   printf("\n***********************SN STATUS TRX MSG RCVD************************\n"); 
   /* Retrieve the SN status from the CZT Indication */
   if (TRUE == cztInd->pdu.val.initiatingMsg.pres.pres)
   {
      if (TRUE == cztInd->pdu.val.initiatingMsg.value.u.cztSNStatusTfr.pres.pres)
      {
         snTfrIe = &cztInd->pdu.val.initiatingMsg.value.u.cztSNStatusTfr.protocolIEs.member[VE_VAL_TWO];  
         cztsnStatusLst = &snTfrIe->value.u.cztE_RABs_SubjToStatusTfr_Lst;
         noComp = cztsnStatusLst->noComp.val;
         snStsInfo.numOfRb = noComp;
         /* Assign the received SN status to local structure */
         for (loopCnt = 0; loopCnt < noComp; loopCnt++)
         {
           snStatusForERAB = &cztsnStatusLst->member[loopCnt];
           snSts = &snStsInfo.veSnSts[loopCnt];
           snSts->rbId = 
              VE_GET_RB_ID_FRM_ERAB(snStatusForERAB->value.u.cztE_RABs_SubjToStatusTfr_Item.e_RAB_ID.val);
           nhuHoPdcpSduInfo = &snSts->nhuHoPdcpSduInfo;
           if (TRUE == snStatusForERAB->value.u.cztE_RABs_SubjToStatusTfr_Item.uL_COUNTvalue.pres.pres)
           {
              /* Initializing and updating the bitmap to indicate the presence of DL COUNT */
              nhuHoPdcpSduInfo->isHoPres = 0x00 | NHU_HO_DL;
              VE_CALC_COUNT(NHU_PDCP_SN_SIZE_7BITS, snStatusForERAB->value.u.cztE_RABs_SubjToStatusTfr_Item.dL_COUNTvalue.pDCP_SN.val, 
                    snStatusForERAB->value.u.cztE_RABs_SubjToStatusTfr_Item.dL_COUNTvalue.hFN.val, nhuHoPdcpSduInfo->dlCountVal);
           }
           if (TRUE == snStatusForERAB->value.u.cztE_RABs_SubjToStatusTfr_Item.uL_COUNTvalue.pres.pres)
           {
              /* Initializing and updating the bitmap to indicate the presence of UL COUNT */
              nhuHoPdcpSduInfo->isHoPres = 0x00 | NHU_HO_UL;
              VE_CALC_COUNT(NHU_PDCP_SN_SIZE_7BITS, snStatusForERAB->value.u.cztE_RABs_SubjToStatusTfr_Item.uL_COUNTvalue.pDCP_SN.val, 
                    snStatusForERAB->value.u.cztE_RABs_SubjToStatusTfr_Item.uL_COUNTvalue.hFN.val, nhuHoPdcpSduInfo->ulCountVal);
           }
            /* selva MSPD FIX */
           /* UL Bit Map informations  */
           if (TRUE == snStatusForERAB->value.u.cztE_RABs_SubjToStatusTfr_Item.receiveStatusofULPDCPSDUs.pres)
           {
               nhuHoPdcpSduInfo->numBits = \
                  snStatusForERAB->value.u.cztE_RABs_SubjToStatusTfr_Item.receiveStatusofULPDCPSDUs.len;
               numbytes = (nhuHoPdcpSduInfo->numBits > 8)?(nhuHoPdcpSduInfo->numBits/8):1;
               VE_ALLOC(&nhuHoPdcpSduInfo->ulBitMap, numbytes);
               cmMemset((U8*)nhuHoPdcpSduInfo->ulBitMap, 0, numbytes);
               cmMemcpy((U8*)nhuHoPdcpSduInfo->ulBitMap,\
                  (U8*)snStatusForERAB->value.u.cztE_RABs_SubjToStatusTfr_Item.receiveStatusofULPDCPSDUs.val,\
                   numbytes);
                
           }
         }/* End of for() */
      }/* End of if (TRUE == cztInd->... */
      else
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veLiCztSnStsTrsfrHandler:\
                  SN Status Transfer(cztSNStatusTfr) IE \
                  is not present in the received Message \n"));
         RETVALUE(RFAILED);
      }
   }/* End of if (TRUE == cztInd->... */
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veLiCztSnStsTrsfrHandler:\
               SN Status Transfer(initiatingMsg) is not present in the received Message \n"));
      RETVALUE(RFAILED);
   }
   /* Send PDCP SN Status to PDCP through RRC */
   vePdcpSnStsUpd(x2ConCb->ueCb, &snStsInfo);
   RETVALUE(ROK);
} /* veLiCztSnStsTrsfrHandler*/


/*
*
*       Fun:  veLiCztUeCntxtRlsHandler
*
*       Desc:  X2 Ue context Release Handler
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiCztUeCntxtRlsHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *pdu
)
#else
PUBLIC S16 veLiCztUeCntxtRlsHandler(x2ConCb,pdu)
VeX2ConCb *x2ConCb;
CztEvnt   *pdu;
#endif
{

   TRC2(veLiCztUeCntxtRlsHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztUeCntxtRlsHandler:\
            Triggering X2AP Ue Cntxt Release \n"));
#endif

   RETVALUE(veX2SrcFSM[x2ConCb->ueCb->mobCtrlState][VE_EVNT_HO_UE_CNTXT_RLS]\
          (x2ConCb, (void *)pdu));
} /* veLiCztUeCntxtRlsHandler */


/*
*
*       Fun:  veLiCztX2SetupRqstHandler
*
*       Desc:  X2 HO Setup Request Handler
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiCztX2SetupRqstHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *pdu
)
#else
PUBLIC S16 veLiCztX2SetupRqstHandler(x2ConCb,pdu)
VeX2ConCb *x2ConCb;
CztEvnt   *pdu;
#endif
{
   VeNghCellCb *neighCellCb = NULLP;
   CztEvnt     *x2SetRsp = NULLP;

   U16         retVal = 0;
#ifdef VE_TEST_CODE
   PRIVATE  Bool flag = FALSE;
#endif   

   TRC2(veLiCztX2SetupRqstHandler);
   printf ("-------------X2SETUP REQ RCVD-------------\n");
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztX2SetupRqstHandler:\
            Triggering X2AP Setup Request\n"));
#endif

#ifdef VE_TEST_RTX_CODE
    /* TEST: Do nothing and return */
    if ( 1 )
     RETVALUE(ROK);
#endif

#ifdef VE_TEST_CODE
   if ( flag == FALSE )
   {
      retVal = veUtlCztFillX2SetupFail(&x2SetRsp, CAUSE_RADIONW, 0, TRUE, 5);
      if ( retVal != ROK )
      {
         printf("******veUtlCztFillX2SetupFail: FAILED****\n");
         RETVALUE(RFAILED);
      }
      flag = TRUE;
      x2SetRsp->peerId = pdu->peerId;
      RETVALUE(VeLiCztGpRsp(&(veCb.cztSap[0]->pst), veCb.cztSap[0]->spId, x2SetRsp));
   }
#endif


   veHOFndInNghLstByPeerId(veCb.cellCb[0], pdu->peerId, &neighCellCb);
   if(NULLP != neighCellCb)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((veCb.init.prntBuf,"veLiCztX2SetupRqstHandler:\
                    invalid setup request from already existing peer\n"));
#endif
      cmFreeMem((Ptr *)pdu);
      RETVALUE(RFAILED);
   }
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztX2SetupRqstHandler:\
            X2SETUP REQUEST HAS BEEN RECEIVED FROM THE PEER: %ld \n",
            pdu->peerId));
#endif
   /* create a new neighbor cell cb and fill the received information */
   VE_ALLOC(&neighCellCb, sizeof(VeNghCellCb));

   if(NULLP == neighCellCb)
   {
      VE_DBG_ERROR((veCb.init.prntBuf,"veLiCztX2SetupRqstHandler:\
                    Failed to allocate Memory.r\n"));
      RETVALUE(RFAILED);
   }

   veCzFillNghInfoFrmX2SetReq(pdu, neighCellCb);
   neighCellCb->peerId = pdu->peerId;
   neighCellCb->isX2ConnAvl = TRUE;

   /* add to nghCellLst of the corresponding cell */
   neighCellCb->lnk.node = (PTR)neighCellCb;
   cmLListAdd2Tail(&(veCb.cellCb[0]->nghCellLst), &neighCellCb->lnk);


   /* send X2 Setup Response */
   retVal = veUtlCztFillX2SetupRsp(&x2SetRsp);
   if(ROK != retVal)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((veCb.init.prntBuf,"veLiCztX2SetupRqstHandler : Failed to Fill veUtlCztFillX2SetupRsp"));
#endif
      RETVALUE(RFAILED);
   }
   x2SetRsp->peerId = neighCellCb->peerId;

   RETVALUE(VeLiCztGpRsp(&(veCb.cztSap[0]->pst), veCb.cztSap[0]->spId, x2SetRsp));   
} /* end of veLiCztX2SetupRqstHandler */

/*
*
*       Fun:  veLiCztX2SetupRspHandler
*
*       Desc:  X2 Setup Response Handler
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiCztX2SetupRspHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *pdu
)
#else
PUBLIC S16 veLiCztX2SetupRspHandler(x2ConCb,pdu)
VeX2ConCb *x2ConCb;
CztEvnt   *pdu;
#endif
{
   VeNghCellCb *neighCellCb = NULLP;
/* X2_CFG_UPD_CHANGES */
#ifdef VE_TEST_CODE
    VeMngmt      cntrl;
#endif
   TRC2(veLiCztX2SetupRspHandler);
   printf ("\n *******************X2SETUP Response Rcvd***********************\n");
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztX2SetupRspHandler:\
            Triggering X2AP Setup Rsp \n"));
#endif

   veHOFndInNghLstByPeerId(veCb.cellCb[0], pdu->peerId, &neighCellCb);
   if(NULLP == neighCellCb)
   {
      VE_DBG_ERROR((veCb.init.prntBuf,"veLiCztX2SetupRspHandler:\
                      invalid setup responde from non existing peer\n"));
      cmFreeMem((Ptr)pdu);
      RETVALUE(RFAILED);
   }
   /* stop the timer */
    veStopTmr((PTR)neighCellCb, VE_TMR_X2_SETUP_TMR); 
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztX2SetupRspHandler:\
            X2SETUP RESPONSE HAS BEEN SUCCESSFULLY RECEIVED FROM THE PEER: %ld \n",
            pdu->peerId));
#endif
   /* updating neighbor info with the received information */
   veCzFillNghInfoFrmX2SetRes(pdu, neighCellCb);
   neighCellCb->isX2ConnAvl = TRUE;

  
  /* X2_CFG_UPD_CHANGES */
  #ifdef VE_TEST_CODE
#ifdef VE_TEST_CFG_UPD
   printf ("\n ******************* TRIGGERING CONFIG UPD REQ ***********************\n");
   cntrl.hdr.elmId.elmnt = STVEX2CFGUPDREQ;
   cntrl.u.cntrl.u.cfgUpdInfo.numSrvdCellAdded = 1;
   cntrl.u.cntrl.u.cfgUpdInfo.numSrvdCellDeleted = 0; 
   cntrl.u.cntrl.u.cfgUpdInfo.numSrvdCellModified = 1;
   cntrl.u.cntrl.u.cfgUpdInfo.numGrpIdAdded = 1;
   cntrl.u.cntrl.u.cfgUpdInfo.numGrpIdDeleted = 0;
   cntrl.u.cntrl.u.cfgUpdInfo.srvdCellAdd[0] = 0;
   cntrl.u.cntrl.u.cfgUpdInfo.srvdCellMod[0].oldCellEcgi.plmnId = 0;
   cntrl.u.cntrl.u.cfgUpdInfo.srvdCellMod[0].oldCellEcgi.eutranCellId = 0;
   cntrl.u.cntrl.u.cfgUpdInfo.srvdCellMod[0].newCellId = 0;
   cntrl.u.cntrl.u.cfgUpdInfo.oldEcgiToDel[0].plmnId = 1;
   cntrl.u.cntrl.u.cfgUpdInfo.oldEcgiToDel[0].eutranCellId = 1;
   cntrl.u.cntrl.u.cfgUpdInfo.guIdAddLst[0].plmnId = 1;
   cntrl.u.cntrl.u.cfgUpdInfo.guIdAddLst[0].mmeGrpId = 1;
   cntrl.u.cntrl.u.cfgUpdInfo.guIdDelLst[0].plmnId = 1;
   cntrl.u.cntrl.u.cfgUpdInfo.guIdDelLst[0].mmeGrpId = 1;
#else
   cntrl.hdr.elmId.elmnt = STVEX2RESET; 
   cntrl.u.cntrl.u.x2Reset.peerId = pdu->peerId;
   cntrl.u.cntrl.u.x2Reset.causeType = CAUSE_MISC; 
   cntrl.u.cntrl.u.x2Reset.causeVal = CztCauseMiscunspecifiedEnum;
#endif /* VE_TEST_CFG_UPD */
  if(ROK != VeMiLveCntrlReq((Pst *)NULLP, &cntrl))
   {
      printf("veLiCztX2SetupRspHandler: VeMiLveCntrlReq failed\n");
   }
#endif /* VE_TEST_CODE */
#ifdef VE_TEST_CODE
#ifdef VE_TEST_ERR_IND
   printf ("\n ******************* TRIGGERING ERROR INDICATION ***********************\n");
   cntrl.hdr.elmId.elmnt = STVEX2GPERRIND;
   if(ROK != VeMiLveCntrlReq((Pst *)NULLP, &cntrl))
   {
      printf("veLiCztX2SetupRspHandler : VeMiLveCntrlReq Failed \n");
   }
#endif /* VE_TEST_ERR_IND */
#endif /* VE_TEST_CODE */

   cmFreeMem((Ptr)pdu);

   RETVALUE(ROK);
} /* veLiCztX2SetupRspHandler*/


/*
*
*       Fun: veLiCztX2SetupFlrHandler 
*
*       Desc:  X2 Setup Failure Handler
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiCztX2SetupFlrHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *pdu
)
#else
PUBLIC S16 veLiCztX2SetupFlrHandler(x2ConCb,pdu)
VeX2ConCb *x2ConCb;
CztEvnt   *pdu;
#endif
{
   VeNghCellCb *neighCellCb = NULLP;
   VeNghCellCb *neighCellCb2 = NULLP;
   CztCause *cause = NULLP;
   CztTimeToWait *timeToWait = NULLP;
   CmLList *lstNode = NULLP;
   U32 delay = 0;

   TRC2(veLiCztX2SetupFlrHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztX2SetupFlrHandler:\
            Triggering X2AP Setup Failure \n"));
#endif
   
   veHOFndInNghLstByPeerId(veCb.cellCb[0], pdu->peerId, &neighCellCb);
   if(NULLP == neighCellCb)
   {
      VE_DBG_ERROR((veCb.init.prntBuf,"veLiCztX2SetupRspHandler:\
                      invalid setup responde from non existing peer\n"));
      cmFreeMem((Ptr)pdu);
      RETVALUE(RFAILED);
   }

   /* log the cause of X2 Setup Failure */
   veUtlGetCztIE(&pdu->pdu, Cztid_Cause, (TknU8 **)&cause);
   VE_DBG_ERROR((veCb.init.prntBuf,"veLiCztX2SetupFlrHandler:\
            X2 Setup Response Failed. \n"));

   /* log the cause of X2 Setup Failure/TimeTowait */

   veUtlGetCztIE(&pdu->pdu, Cztid_TimeToWait, (TknU8 **)&timeToWait);
   if ( timeToWait != NULLP )
   {
      /* Stop if the timer is already running */
      veStopTmr((PTR) neighCellCb, VE_TMR_X2_SETUP_TMR); 

      /* assign retry = 0xFF to identify timeToWait */
      neighCellCb->maxSetupRetry = 0xFF;
      
      switch( timeToWait->val )
      {
         case 0:
            delay = (1 * SS_TICKS_SEC)/VE_TMR_RES; 
            break;
         case 1:
            delay = (2 * SS_TICKS_SEC)/VE_TMR_RES; 
            break;
         case 2:
            delay = (5 * SS_TICKS_SEC)/VE_TMR_RES; 
            break;
         case 3:
            delay = (10 * SS_TICKS_SEC)/VE_TMR_RES; 
            break;
         case 4:
            delay = (20 * SS_TICKS_SEC)/VE_TMR_RES; 
            break;
         case 5:
         /* fall through */
         default:
            delay = (60 * SS_TICKS_SEC)/VE_TMR_RES; 
            break;
      }

     /* start the timer with delay as 'timeToWait' */
     veStartTmr((PTR) neighCellCb, VE_TMR_X2_SETUP_TMR, delay);
   
     RETVALUE(ROK);
   }

   /* remove the neighbor cell info from the list */
   lstNode = veCb.cellCb[0]->nghCellLst.first;
   while(NULL != lstNode)
   {
      neighCellCb2 = (VeNghCellCb*)lstNode->node;
      if(neighCellCb2->peerId == neighCellCb->peerId)
      {
         /* remove the node from the candLst */
         cmLListDelFrm(&(veCb.cellCb[0]->nghCellLst), lstNode);
         /* Free the memory for that neighbor cell Cb */
         VE_FREE(lstNode->node, sizeof(VeNghCellCb));
         break;
      }
   }

   RETVALUE(ROK);
} /* veLiCztX2SetupFlrHandler */


/*
*
*       Fun: veHODecision
*
*       Desc:  HO Decision
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veHODecision
(
VeCellCb *cellCb,
NhuMeasResults *measResults,
Bool         *hoNeeded
)
#else
PUBLIC S16 veHODecision(cellCb, measResults, hoNeeded)
VeCellCb *cellCb;
NhuMeasResults *measResults;
Bool         *hoNeeded;
#endif
{

   TRC2(veHODecision);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHODecision:\
            Triggering HO Decision\n"));
#endif
   /* This is the place hold to map the measurement report with the corresponding event
      which triggered the same using the measurement identity  as a reference. Based on the event 
      HO decision should be taken. Currently measurement comes only in case of HO event we are directly
      going for HO */
     
   *hoNeeded = TRUE;


   RETVALUE(ROK);
} /*  veHODecision*/


/*
*
*       Fun:   veHOFillCandCells
*
*       Desc:  Fill Candidate Cells
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veHOFillCandCells
(
VeUeCb *ueCb,
NhuMeasResults *measResults
)
#else
PUBLIC S16 veHOFillCandCells(ueCb, measResults)
VeUeCb *ueCb;
NhuMeasResults *measResults;
#endif
{
   U8 loopCnt = 0;
   NhuMeasResultLstEUTRA *measResultLstEUTRA = NULLP;
   NhuMeasResultEUTRA *measResultEUTRA = NULLP;
   VeNghCellCb *neighCellCb = NULLP;
   U16   ret = 0;
   VeCellCb  *cellCb = NULLP;

   TRC2(veHOFillCandCells);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHOFillCandCells:\
            Triggering Fill Candidate cells\n"));
#endif

   cellCb = veCb.cellCb[0];
   /* Traverse through the list of E-UTRAN neighbouring cell measurements
   in datIndSdu, and based on the predefined macro(Num of Candidate cells)
   populate the candidate list of the UE with the information reported through
   the measurement and the cell info available at the eNB*/
   switch (measResults->measResultNeighCells.choice.val)
   {
      case 0:
      {
         measResultLstEUTRA = 
            &measResults->measResultNeighCells.val.measResultLstEUTRA;
            
         for(loopCnt = 0; loopCnt < measResultLstEUTRA->noComp.val; loopCnt++) 
         {
            /* Fetch the neighbor cell Cb using pci in Measurement Report
               from the neighborCell List  */
            measResultEUTRA = &measResultLstEUTRA->member[loopCnt];
            ret = veHOFndInNghLstByPci(cellCb, measResultEUTRA->physCellId.val, 
                                  &neighCellCb);
            if (ROK != ret)
            {
               break;
            }
            if(neighCellCb != NULLP)
            {
               veHOAddToCandLst(ueCb, neighCellCb); 
            }
            else
            {
               /* Print the debug info */
            }
         }/* End of for */
      }
      break;
      default:
      break;
   }/* End of switch()*/
   RETVALUE(ROK);
} /*  veHOFillCandCells */

/*
*
*       Fun:   veHOFndInNghLstByPci
*
*       Desc:  Finding the neighborCell from the list for the corrsponding pci
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veHOFndInNghLstByPci
(
VeCellCb *cellCb,
U32 physCellId,
VeNghCellCb **neighCellCb
)
#else
PUBLIC S16 veHOFndInNghLstByPci(cellCb, physCellId, neighCellCb)
VeCellCb *cellCb;
U32 physCellId;
VeNghCellCb **neighCellCb;
#endif
{
   S16 ret = RFAILED;
   CmLList *cmLstEnt = NULLP;
   VeNghCellCb *nghCellCb = NULLP;

   TRC2(veHOFndInNghLstByPci);
   
   cmLstEnt = cellCb->nghCellLst.first;
   while (NULLP != cmLstEnt)
   {
      nghCellCb = (VeNghCellCb *)cmLstEnt->node;
      if (physCellId == nghCellCb->veCellPrp.physCellId)
      {
         *neighCellCb = nghCellCb;
         ret = ROK;
         break;
      }
      /* Move to next entry */
      cmLstEnt = cmLstEnt->next;
   }/* End of while */ 

   RETVALUE(ret);
}/* End of veHOFndInNghLstPci() */

/*
*
*       Fun:   veHOFndInNghLstByPeerId
*
*       Desc:  Finding the neighborCell from the list for the corrsponding peerId
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veHOFndInNghLstByPeerId
(
VeCellCb *cellCb,
U32 peerId,
VeNghCellCb **neighCellCb
)
#else
PUBLIC S16 veHOFndInNghLstByPeerId(cellCb, peerId, neighCellCb)
VeCellCb *cellCb;
U32 peerId;
VeNghCellCb **neighCellCb;
#endif
{
   S16 ret = ROK;
   CmLList *cmLstEnt = NULLP;
   VeNghCellCb *nghCellCb = NULLP;

   TRC2(veHOFndInNghLstByPeerId);
   
   cmLstEnt = cellCb->nghCellLst.first;
   while (NULLP != cmLstEnt)
   {
      nghCellCb = (VeNghCellCb *)cmLstEnt->node;
     /* X2_CFG_UPD_CHANGES */
     if(nghCellCb != NULLP)
     {
        if (peerId == nghCellCb->peerId)
        {
          *neighCellCb = nghCellCb;
          break;
        }
     }
      /* Move to next entry */
      cmLstEnt = cmLstEnt->next;
   }/* End of while */ 

   RETVALUE(ret);
}/* End of veHOFndInNghLstByPeerId() */


/*
*
*       Fun:   veHOAddToCandLst
*
*       Desc:  Adding Candidate Cell into the list
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veHOAddToCandLst
(
VeUeCb *ueCb,
VeNghCellCb *neighCellCb
)
#else
PUBLIC S16 veHOAddToCandLst(ueCb, neighCellCb)
VeUeCb *ueCb;
VeNghCellCb *neighCellCb;
#endif
{
   S16 ret = ROK;
   VeCandCellDb *candCellDb = NULLP;
   TRC2(veHOAddToCandLst);
   
   /* Allocate memory to VeCandCellDb */
   VE_ALLOC(&candCellDb, sizeof(VeCandCellDb));

   if(NULLP == candCellDb)
   {
     VE_DBG_ERROR((VE_PRNT_BUF,"veHOAddToCandLst:\
                       Failed to allocate memory.\n"));
      RETVALUE(RFAILED); 
   }
   
   /* populate the members */
   candCellDb->nghCellCb = neighCellCb;
   candCellDb->x2ConCb = NULLP;
   
   /* Add the candidateCellDb in to the linked List */
   cmLListAdd2Tail(&(ueCb->candCellList), &(candCellDb->lnk));
   candCellDb->lnk.node = (PTR)candCellDb;
   RETVALUE(ret);
}/* End of veHOAddToCandLst() */


/*
*
*       Fun:   veUpdCandLstSndCncl
*
*       Desc:  Adding Candidate Cell into the list
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veUpdCandLstSndCncl
(
VeUeCb    *ueCb
)
#else
PUBLIC S16 veUpdCandLstSndCncl(ueCb)
VeUeCb    *ueCb;
#endif
{
   S16 ret = ROK;
   CmLList *lstNode = NULLP;
   VeCandCellDb *candCellDb = NULLP;

   TRC2(veUpdCandLstSndCncl);

   if(NULLP == ueCb)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veUpdCandLstSndCncl:ueCb is NULL."));

      RETVALUE(RFAILED);
   }

   /* For the current release, we are supporting sending of HO Request or HO Required.
      So deleting all the candidates except for the first one. */
   /* deleting the candcells node from the lsit except the first one */
   CM_LLIST_NEXT_NODE(&(ueCb->candCellList), lstNode);
   while(NULLP != lstNode)
   {
       /* remove the node from the candLst */
       cmLListDelFrm(&(ueCb->candCellList), lstNode);
       candCellDb = (VeCandCellDb*)lstNode->node;
       if(candCellDb->nghCellCb->isX2ConnAvl)
       {
          /* Free the memory for x2ConCb */
          VE_FREE(candCellDb->x2ConCb, sizeof(VeX2ConCb));
       }
       /* Free the memory for candidate DB */
       VE_FREE(candCellDb, sizeof(VeCandCellDb));
       /* Move to next node */
       CM_LLIST_NEXT_NODE(&(ueCb->candCellList), lstNode);
   }/* End Of while */


   RETVALUE(ret);
}/* End of veUpdCandLstSndCncl */

/*
*
*       Fun:   veClrCandLst
*
*       Desc:  Adding Candidate Cell into the list
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veClrCandLst
(
CmLListCp *cndLst
)
#else
PUBLIC S16 veClrCandLst(cndLst)
CmLListCp *cndLst;
#endif
{
   S16 ret = ROK;
   CmLList *lstNode = NULLP;
   VeCandCellDb *candCellDb = NULLP;

   TRC2(veClrCandLst);

   CM_LLIST_FIRST_NODE(cndLst, lstNode);
   while(NULLP != lstNode)
   {
       /* remove the node from the candLst */
       cmLListDelFrm(cndLst, lstNode);
       candCellDb = (VeCandCellDb*)lstNode->node;
       if(candCellDb->nghCellCb->isX2ConnAvl)
       {
          /* Free the memory for x2ConCb */
          VE_FREE(candCellDb->x2ConCb, sizeof(VeX2ConCb));
       }
       /* Free the memory for candidate DB */
       VE_FREE(candCellDb, sizeof(VeCandCellDb));
       /* Move to next node */
       CM_LLIST_NEXT_NODE(cndLst, lstNode);
   }/* End Of while */
   RETVALUE(ret);
}/* End of veUpdCandLstSndCncl */



/*
*
*       Fun:   veHoCfgTunls
*
*       Desc:  Configuring lower layers to create data forwarding tunnels
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veHoCfgTunls
(
VeUeCb    *ueCb,
U8        tunCreateType
)
#else
PUBLIC S16 veHoCfgTunls(ueCb, tunCreateType)
VeUeCb    *ueCb;
U8         tunCreateType;
#endif
{
   EuCrEGtpTunInf  tunelInf;
   VeUeRAB *rabInfo = NULLP;
   U8 tunIdx = 0;
   U8 rbIdx = 0;
   Pst pst;

   SztGTP_TEID *remTeIdStr = NULLP;
   U8 indx, shiftBits;
   U32 addrMask;


   TRC2(veHoCfgTunls);

   /* check for invalid arguments */
   if(NULLP == ueCb)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHoCfgFwdTunls:\
                       Invalid Arguments\n"));
      RETVALUE(RFAILED);
   }

   rabInfo = ueCb->rbInfo.rab;

   VE_SET_ZERO(&tunelInf, sizeof(EuCrEGtpTunInf));

   /* for number of tunnels that have been configured, add the tunnel information in the
      tunLst */
   for(rbIdx = 0; ((rbIdx < ueCb->rbInfo.numOfRbCfgd) && (tunIdx < VE_MAX_UE_TUNNELS)); rbIdx++)
   {
      if(VE_EGTP_TUN_CREATE == tunCreateType)
      {
         tunelInf.tunLst[tunIdx].rbDir = rabInfo[rbIdx].rbDir;
         tunelInf.tunLst[tunIdx].tunnType = VE_NORMAL_TUNNEL;
         tunelInf.tunLst[tunIdx].tunInf.locTeid = rabInfo[rbIdx].locEgtpTunId;

         tunelInf.tunLst[tunIdx].tunInf.remTeid = CM_INET_NTOH_U32(*((U32 *)(rabInfo[rbIdx].remEgtpTunId.val)));
         tunelInf.tunLst[tunIdx].tunInf.pdcpId.cellId = ueCb->cellId;
         tunelInf.tunLst[tunIdx].tunInf.pdcpId.ueId = ueCb->crnti;
         tunelInf.tunLst[tunIdx].tunInf.pdcpId.rbId = rabInfo[rbIdx].rbId;
         tunelInf.tunLst[tunIdx].tunInf.pdcpId.rbType = CM_LTE_DRB;

         /* filling dst and src ip address */
         veCpyCmTptAddr(&(tunelInf.tunLst[tunIdx].tunInf.dstIpAddr),&(rabInfo[rbIdx].sgwAddr));
         veCpyCmTptAddr(&(tunelInf.tunLst[tunIdx].tunInf.frmDstIpAddr),&(veCb.datAppAddr));

         tunIdx++;
      }
      if(rabInfo[rbIdx].isDataFwdAvl == TRUE)
      {
         switch(rabInfo[rbIdx].rbDir)
         {
            case VE_UL:
            {
               tunelInf.tunLst[tunIdx].rbDir = VE_UL;
               tunelInf.tunLst[tunIdx].rbMode = rabInfo[rbIdx].rbMode;
               tunelInf.tunLst[tunIdx].tunnType = VE_FORWARD_TUNNEL;
               tunelInf.tunLst[tunIdx].tunInf.locTeid = rabInfo[rbIdx].datFwdUlLocEgtpTunId;
               
          tunelInf.tunLst[tunIdx].tunInf.remTeid = CM_INET_NTOH_U32(*((U32 *)(rabInfo[rbIdx].remUlDataFwdTeId.val)));
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.cellId = ueCb->cellId;
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.ueId = ueCb->crnti;
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.rbId = rabInfo[rbIdx].rbId;
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.rbType = CM_LTE_DRB;
               veCpyCmTptAddr(&(tunelInf.tunLst[tunIdx].tunInf.dstIpAddr),&(rabInfo[rbIdx].datFwdPeerAddr));
               veCpyCmTptAddr(&(tunelInf.tunLst[tunIdx].tunInf.frmDstIpAddr),&(veCb.datAppAddr));
               tunIdx++;
               break;
            }

            case VE_DL:
            {
               tunelInf.tunLst[tunIdx].rbDir = VE_DL;
               tunelInf.tunLst[tunIdx].rbMode = rabInfo[rbIdx].rbMode;
               tunelInf.tunLst[tunIdx].tunnType = VE_FORWARD_TUNNEL;
               tunelInf.tunLst[tunIdx].tunInf.locTeid = rabInfo[rbIdx].datFwdDlLocEgtpTunId;
          
          tunelInf.tunLst[tunIdx].tunInf.remTeid = CM_INET_NTOH_U32(*((U32 *)(rabInfo[rbIdx].remDlDataFwdTeId.val)));
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.cellId = ueCb->cellId;
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.ueId = ueCb->crnti;
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.rbId = rabInfo[rbIdx].rbId;
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.rbType = CM_LTE_DRB;
               veCpyCmTptAddr(&(tunelInf.tunLst[tunIdx].tunInf.dstIpAddr),&(rabInfo[rbIdx].datFwdPeerAddr));
               veCpyCmTptAddr(&(tunelInf.tunLst[tunIdx].tunInf.frmDstIpAddr),&(veCb.datAppAddr));
               tunIdx++;
               break;
            }
            case VE_DL_UL:
            {
               tunelInf.tunLst[tunIdx].rbDir = VE_UL;
               tunelInf.tunLst[tunIdx].rbMode = rabInfo[rbIdx].rbMode;
               tunelInf.tunLst[tunIdx].tunnType = VE_FORWARD_TUNNEL;
               tunelInf.tunLst[tunIdx].tunInf.locTeid = rabInfo[rbIdx].datFwdUlLocEgtpTunId;
               tunelInf.tunLst[tunIdx].tunInf.remTeid = CM_INET_NTOH_U32(*((U32 *)(rabInfo[rbIdx].remUlDataFwdTeId.val)));
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.cellId = ueCb->cellId;
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.ueId = ueCb->crnti;
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.rbId = rabInfo[rbIdx].rbId;
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.rbType = CM_LTE_DRB;
               veCpyCmTptAddr(&(tunelInf.tunLst[tunIdx].tunInf.dstIpAddr),&(rabInfo[rbIdx].datFwdPeerAddr));
               veCpyCmTptAddr(&(tunelInf.tunLst[tunIdx].tunInf.frmDstIpAddr),&(veCb.datAppAddr));
               tunIdx++;

               tunelInf.tunLst[tunIdx].rbDir = VE_DL;
               tunelInf.tunLst[tunIdx].rbMode = rabInfo[rbIdx].rbMode;
               tunelInf.tunLst[tunIdx].tunnType = VE_FORWARD_TUNNEL;
               tunelInf.tunLst[tunIdx].tunInf.locTeid = rabInfo[rbIdx].datFwdDlLocEgtpTunId;

               tunelInf.tunLst[tunIdx].tunInf.remTeid = CM_INET_NTOH_U32(*((U32 *)(rabInfo[rbIdx].remDlDataFwdTeId.val)));
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.cellId = ueCb->cellId;
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.ueId = ueCb->crnti;
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.rbId = rabInfo[rbIdx].rbId;
               tunelInf.tunLst[tunIdx].tunInf.pdcpId.rbType = CM_LTE_DRB;
               veCpyCmTptAddr(&(tunelInf.tunLst[tunIdx].tunInf.dstIpAddr),&(rabInfo[rbIdx].datFwdPeerAddr));
               veCpyCmTptAddr(&(tunelInf.tunLst[tunIdx].tunInf.frmDstIpAddr),&(veCb.datAppAddr));
               tunIdx++;
               break;
            }
            default:
            {
               VE_DBG_ERROR((VE_PRNT_BUF,"veHoCfgTunls: Invalid RB direction\n"));
               RETVALUE(RFAILED);
               break;
            }
         }/* End of switch() */
      }
   }/* End of for() */

   tunelInf.numTunls = tunIdx;

   cmMemset((U8 *)&pst, 0, sizeof(Pst));
   pst.dstEnt = ENTVE;
   pst.srcEnt = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_VE_PROC;
   cmPkEuCreattLocTunSetReq(&pst, 0, &tunelInf);

   RETVALUE(ROK);
}/* End of veHoCfgTunls */


/*
*
*       Fun:   veGetPdcpSnSts 
*
*       Desc:  Request RRC to fetch the PDCP SN status
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veGetPdcpSnSts 
(
VeUeCb    *ueCb
)
#else
PUBLIC S16 veGetPdcpSnSts (ueCb)
VeUeCb    *ueCb;
#endif
{
   NhuHdr  nhuHdr;
   U8 loopCnt = 0;
   Bool sndSNSts = FALSE;
   TRC2(veGetPdcpSnSts);

   nhuHdr.cellId = ueCb->cellId;
   nhuHdr.ueId = ueCb->crnti;
   nhuHdr.transId = VE_TRANSID_GET_PDCP_SDUSTA;
   /* Block the SduStsReq to RRC in case there is no AM mode DRBs established for the UE */ 
   for (loopCnt = 0; loopCnt < ueCb->rbInfo.numOfRbCfgd; loopCnt++)
   {
      if (NHU_RLC_MODE_AM == ueCb->rbInfo.rab[loopCnt].rbMode)
      {
         sndSNSts = TRUE;
         break;
      }
   }
   if (sndSNSts)
   {
      RETVALUE(VeLiNhuPdcpSduStaReq(&veCb.nhuSap[0]->pst, veCb.nhuSap[0]->suId, nhuHdr));
   }
   else
   {
      RETVALUE(ROK);
   }

}/* End of veGetPdcpSnSts */

/*
*
*       Fun:   vePdcpSnStsUpd
*
*       Desc:  Request RRC to configure PDCP with the received SN information
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 vePdcpSnStsUpd
(
VeUeCb        *ueCb,
VeSnStsInfo   *snStsInfo
) 
#else
PUBLIC S16 vePdcpSnStsUpd(ueCb, snStsInfo)
VeUeCb        *ueCb;
VeSnStsInfo   *snStsInfo;
#endif
{
   Pst *pst = NULLP;
   NhuCfgReqSdus *nhuCfgReqSdus = NULLP;
   NhuUeCfgInfo *nhuUeCfgInfo = NULLP;
   NhuRbCfgList  *rbCfgList = NULLP;
   NhuRbCfgInfo *nhuRbCfgInfo = NULLP;
   NhuPdcpCfg *pdcpCfg = NULLP;
   U8 numOfRb = 0;
   U8 loopCnt = 0;
   TRC2(vePdcpSnStsUpd);
   
   /* Allocate memory for event structure.*/
   VE_ALLOCEVNT(&nhuCfgReqSdus, sizeof(NhuCfgReqSdus));

   /* Fill Event Handler Information.*/
   veFillNhuEvntHdr(&(nhuCfgReqSdus->hdr), ueCb->cellId, ueCb->crnti, VE_TRANSID_SN_CFG);

   /*
    * Fill SN status information in the ueCfg whic will be conveyed to PDCP by RRC .
    */
    nhuUeCfgInfo = &nhuCfgReqSdus->cfgInfo;
    nhuUeCfgInfo->ueCfgBitMsk = NHU_EXP_CFG_RB_CFG;
    nhuUeCfgInfo->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_MODIFY;
    rbCfgList = &(nhuUeCfgInfo->rbCfgAllList.rbToModCfgList);
    numOfRb = snStsInfo->numOfRb;
    rbCfgList->numOfRbCfgInfo = numOfRb;
    VE_GET_MEM(nhuCfgReqSdus, (sizeof(NhuRbCfgInfo) * (numOfRb)),\
                   &(rbCfgList->rbCfgInfoList));
    for (loopCnt = 0; loopCnt < numOfRb; loopCnt++)
    {
       nhuRbCfgInfo = &rbCfgList->rbCfgInfoList[loopCnt];
       /* Initialize and update the bitmask */
       nhuRbCfgInfo->rbCfgBitMsk = 0x00 | NHU_RB_CFG_PDCP_INFO;

       nhuRbCfgInfo->rbId.rbId = snStsInfo->veSnSts[loopCnt].rbId;
       /* As the data forwarding only happens for DRB and the SN transfer also comes
          only for the DRBs assign the bearer type to CM_LTE_DRB */
       nhuRbCfgInfo->rbId.rbTyp = CM_LTE_DRB;

       /* Logical channel Mapping */
       nhuRbCfgInfo->lcId.lcId = snStsInfo->veSnSts[loopCnt].rbId;
       /* As the data forwarding only happens for DRB and the SN transfer also comes
          only for the DRBs assign the Logical Channel Type to CM_LTE_LCH_DTCH */
       nhuRbCfgInfo->lcId.logChType = CM_LTE_LCH_DTCH;
  
       pdcpCfg = &nhuRbCfgInfo->pdcpCfg;
       /* Initialize and update the bitmask */
       pdcpCfg->pdcpCfgBitMsk = 0x00 | NHU_PDCP_SDU_STA_INFO;
       pdcpCfg->hoPdcpSduStaInfo = snStsInfo->veSnSts[loopCnt].nhuHoPdcpSduInfo; 
       /* initializing the members which are not used */
       pdcpCfg->intgReqd = FALSE;
       pdcpCfg->cfmReqd = FALSE;
    }/* End of for () */

    /* Send the UE configuration to RRC */
    pst = &veCb.nhuSap[0]->pst;
    pst->event = EVTNHUCFGREQ;

    RETVALUE(VeLiNhuCfgReq(pst, veCb.nhuSap[0]->spId, nhuCfgReqSdus));
}/* End of vePdcpSnStsUpd */

/*
*
*       Fun:   veSndX2UeCtxtRls
*
*       Desc:  Request X2AP to send the UE Context Release message to peer
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veSndX2UeCtxtRls
(
VeX2ConCb     *x2ConCb,
PTR           *notused
)
#else
PUBLIC S16 veSndX2UeCtxtRls(x2ConCb, notused)
VeX2ConCb     *x2ConCb;
PTR           *notused;
#endif
{
   CztEvnt *ueRlsPdu = NULLP;

   TRC2(veSndX2UeCtxtRls);

   if(NULLP == x2ConCb)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndX2UeCtxtRls: Invalid Arguments\n"));
      RETVALUE(RFAILED);
   }

   if(ROK != veUtlCztFillUeCxtRls(&ueRlsPdu, x2ConCb->oldEnbUeX2apId, x2ConCb->newEnbUeX2apId))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndX2UeCtxtRls: Filling of X2 UE Context Release failed\n"));
      RETVALUE(RFAILED);
   }
   /* filling the peerId */
   ueRlsPdu->peerId = x2ConCb->peerId;

   if(ROK != VeLiCztBmpReq (&veCb.cztSap[0]->pst, veCb.cztSap[0]->spId, ueRlsPdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndX2UeCtxtRls: Sending of X2 UE Context Release Failed\n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/* End of veSndX2UeCtxtRls */

/*
*
*       Fun:   veHoProcHoCfm
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
PUBLIC S16 veHoProcHoCfm
(
VeUeCb *ueCb
)
#else
PUBLIC S16 veHoProcHoCfm(ueCb)
VeUeCb *ueCb;
#endif
{
   Pst          pst;
   S1apPdu      *hoNtfyPdu = NULLP;
   S1apPdu      *pthSwtchPdu = NULLP;
   SztDatEvntReq datEvt;
   SztConReq pathSwtReq;
   U16 idx = 0;
   VeEuChngTnlStInfo stInfo;

   /* 1. Stop the UeHoAttach timer
      2. Send the buffered forwarded data at the application level into PDCP.
      3. In case of S1HO Prepare Handover Notify and send it to MME.
         - else in case of X2HO prepare PATH SWITCH REQUEST and send it to MME.
    */
   
   TRC2(veHoProcHoCfm);

   
   /* Stop the UeHoAttach timer */
   veStopTmr((PTR)ueCb, VE_TMR_UE_HO_ATTACH_TMR);

   /* Request Data APP to change the state of the tun so that, the buffered forwarded data 
    * at the Data APP will be sent to PDCP.*/
   cmMemset((U8 *)&pst, 0, sizeof(Pst));
   pst.dstEnt = ENTVE;
   pst.srcEnt = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_VE_PROC;

   stInfo.state = VE_EU_INITIAL;
   stInfo.cause = VE_EU_UE_FOUND_AT_CELL;
   for(idx = 0; idx < ueCb->rbInfo.numOfRbCfgd; idx++)
   {
      stInfo.teId = ueCb->rbInfo.rab[idx].locEgtpTunId;
      cmPkChngStReq(&pst, 0, &stInfo);
   }

   /*  Prepare Handover Notify/Path Switch Request and send it to MME. */
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veHoProcHoCfm:\
            Triggering sending of HO Notify/Path Switch Req\n"));
#endif


   if (VE_HO_TYPE_S1 == ueCb->hoType)
   {
      /* Fill Handove Notify structure */
      if(RFAILED == veUtlSztFillHovrNtfy (ueCb->s1ConCb, &hoNtfyPdu, veCb.cellCb[0]))
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"veHoProcHoCfm : Encoding of S1AP Handover Notify message Failed\n"));
#endif
         RETVALUE(RFAILED);
      }

      datEvt.spConnId = ueCb->s1ConCb->spConnId;
      datEvt.pdu    =  hoNtfyPdu;

      /* Trigger SZT data Req */
      printf("\n////// Sending HO Notify to MME ///////\n");
      VeLiSztDatReq(&veCb.sztSap[0]->pst, veCb.sztSap[0]->spId, &datEvt);
   }
   else if (VE_HO_TYPE_X2 == ueCb->hoType)
   {
      if(ROK != veUtlSztFillPathSwRqst(ueCb->s1ConCb, &pthSwtchPdu))
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"veHoProcHoCfm : Encoding of S1AP Path Switch message Failed\n"));
#endif
         RETVALUE(RFAILED);
      }
      VE_SET_ZERO(&pathSwtReq, sizeof(SztConReq));
      pathSwtReq.peerId.val = 1;
      pathSwtReq.peerId.pres = TRUE;
      pathSwtReq.u.suConnId = ueCb->s1ConCb->suConnId;
      pathSwtReq.pdu = pthSwtchPdu;
      /* Trigger SZT Connectn. Req */
      VeLiSztConReq(&veCb.sztSap[0]->pst, veCb.sztSap[0]->spId, &pathSwtReq);
   }
   RETVALUE (ROK);
} /* veHoProcHoCfm */



/*
*
*       Fun:   veLiCztResetRqstHandler, 
*
*       Desc:  Handle incoming Reset Request. 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiCztResetRqstHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *cztInd
)
#else
PUBLIC S16 veLiCztResetRqstHandler(x2ConCb, cztInd)
VeX2ConCb *x2ConCb;
CztEvnt   *cztInd;
#endif
{
   S16 ret = ROK;

   UNUSED(x2ConCb);

   TRC2(veLiCztResetRqstHandler);
   printf("****Received X2-Reset Request Message****\n");
   /* TEST: To Test rest x2-Setup retransmission */
#ifdef VE_TEST_RTX_CODE
    /* TEST: Do nothing and return */
    if ( 1 )
     RETVALUE(ROK);
#endif
   ret = veAbortX2OngoingProc(cztInd->peerId);
   if ( ret != ROK)
   {
#ifdef DEBUGP
      VE_LYR_DBGP(DBGMASK_SM, (VE_PRNT_BUF, "veLiCztResetRqstHandler: veSndX2ResetRsp failed!\n"));
#endif
     RETVALUE(ret);
   }

  /* Send Reset Response to peer */
   ret = veSndX2ResetRsp (cztInd->peerId);
   if ( ret != ROK )
   {
#ifdef DEBUGP
      VE_LYR_DBGP(DBGMASK_SM, (VE_PRNT_BUF, "veLiCztResetRqstHandler: veSndX2ResetRsp failed!\n"));
#endif
   }
   RETVALUE(ret);
} /* veLiCztResetRqstHandler */

/*
*
*       Fun:   veLiCztResetRspHandler, 
*
*       Desc:  Handle incoming Reset response. 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veLiCztResetRspHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *cztInd
)
#else
PUBLIC S16 veLiCztResetRspHandler(x2ConCb, cztInd)
VeX2ConCb *x2ConCb;
CztEvnt   *cztInd;
#endif
{
   VeNghCellCb *neighCellCb;
   TRC2(veLiCztResetRspHandler);

   UNUSED(x2ConCb);

   printf("****Received X2-Reset Response Message*****\n");

   veHOFndInNghLstByPeerId(veCb.cellCb[0], cztInd->peerId, &neighCellCb);
   if(NULLP != neighCellCb)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((veCb.init.prntBuf,"veLiCztResetRspHandler: \
               invalid Reset Resp\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Stop the reset TIMER */
   veStopTmr((PTR)neighCellCb, VE_TMR_X2_RESET_TMR);

   RETVALUE(ROK);
} /* veLiCztResetRspHandler */

/*
*
*       Fun:   veAbortX2OngoingProc   
*
*       Desc:  Abort any other ongoing procedures. 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PUBLIC S16 veAbortX2OngoingProc
(
U32 peerId
)
#else
PUBLIC S16 veAbortX2OngoingProc(peerId)
U32 peerId;
#endif
{
   U32      idx = 0;
   VeNghCellCb *neighCellCb = NULLP;
 
  /* Abort if any X2-setup procedure is going on */
    
   veHOFndInNghLstByPeerId(veCb.cellCb[0], peerId, &neighCellCb);
   if ( neighCellCb != NULLP )
   {
     veStopTmr((PTR)neighCellCb, VE_TMR_X2_SETUP_TMR); 
   }
    
   /* Abort if any handover procedure is going on */

   for (idx = 0; idx < VE_MAX_X2AP_CON; idx++)
   {
      if( (veCb.x2apConLst[idx] != NULL) && (veCb.x2apConLst[idx]->peerId == peerId))
      { 
         /* X2-handover is going on, clear all the handover info */ 
         if (ROK != veAbortX2HOProc(veCb.x2apConLst[idx]))
         {
            RETVALUE(RFAILED);
         } 
         VE_FREE(veCb.x2apConLst[idx], sizeof(VeX2ConCb));
         veCb.x2apConLst[idx] = NULLP;
      }
   }
   
   /*TODO: Abort if Any other ongoing procedure */

   RETVALUE(ROK);
} /* veAbortX2OngoingProc */ 
    
  
/*
*
*       Fun:   veAbortX2HOProc   
*
*       Desc:  Handle incoming Reset response. 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_ho.c 
*
*/
#ifdef ANSI
PRIVATE S16 veAbortX2HOProc
(
VeX2ConCb *x2ConCb
)
#else
PRIVATE S16 veAbortX2HOProc(x2ConCb)
VeX2ConCb *x2ConCb;
#endif
{
   S16      ret = ROK;
   VeUeCb  *ueCb = NULLP;
   U16      ueIdx = 0;

   TRC2(veAbortX2HOProc);

   /* Stop the timer */
   if ( (x2ConCb->hoNodeType == SRC_ENODEB) && 
        (x2ConCb->ueCb->mobCtrlState != VE_HO_NORMAL) )
   {
      /* clear candidate cell list for the ue */
      veClrCandLst(&x2ConCb->ueCb->candCellList);
      veStopTmr((PTR)x2ConCb->ueCb, VE_TMR_UE_HO_ATTACH_TMR); 
      x2ConCb->ueCb->mobCtrlState = VE_HO_NORMAL;
   }
   else if ( (x2ConCb->hoNodeType == TGT_ENODEB) && 
             (x2ConCb->ueCb->mobCtrlState != VE_HO_NORMAL) )
   {
      /* Release UE Resources */
      /* Get the UE Control Block */
      ueCb = x2ConCb->ueCb;

      if(ueCb->lowLyrCfgSts == VE_LOW_LYR_CFG_COMPL || 
            ueCb->lowLyrCfgSts == VE_LOW_LYR_CFG_NOT_STRTD)
      {
         ueCb->lowLyrCfgSts = VE_LOW_LYR_CFG_REM;
         /* release UE resources */
         veSndCtfUeRls(ueCb);
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

      veStopTmr((PTR)x2ConCb->ueCb, VE_TMR_UE_HO_ATTACH_TMR); 
      x2ConCb->ueCb->mobCtrlState = VE_HO_NORMAL;
   }
   RETVALUE(ret);
}

/* X2_CFG_UPD_CHANGES */

/*
*
*       Fun: veLiCztcfgUpdReqHandler
*
*       Desc:  Configuration Update Request Handler
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_ho.c
*
*/
#ifdef ANSI
PUBLIC S16 veLiCztCfgUpdReqHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *pdu
)
#else
PUBLIC S16 veLiCztCfgUpdReqHandler(x2ConCb,pdu)
VeX2ConCb *x2ConCb;
CztEvnt   *pdu;
#endif
{
   VeNghCellCb *neighCellCb = NULLP;
   CmLList     *lstNode = NULLP;
   CztEvnt     *cfgUpdRsp = NULLP;
   S16         retVal = 0, ret = 0;

   TRC2(veLiCztCfgUpdReqHandler);

   veHOFndInNghLstByPeerId(veCb.cellCb[0], pdu->peerId, &neighCellCb);
   if(NULLP == neighCellCb)
   {
#ifdef DEBUGP
     VE_DBG_ERROR((veCb.init.prntBuf,"veLiCztX2SetupRqstHandler:\
                    neighbour list does not exist. \n"));
#endif
      cmFreeMem((Ptr *)pdu);
      RETVALUE(RFAILED);
   }

   /* Case1 : Request contains the served cells to add IE */
   /* In this case the information received in the added served cells will be
      added to the information of neighbour cells. */

   /* The served cells to add IE contains 2 sub IEs:
      a. Served Cell Information
      b. Neighbour Information */
   retVal = veCzFillCellAddFrmCfgUpdReq(pdu);
   /* Case2 : Request contains the served cells to modify IE */
   /* In this case the information received in the modified served cells will
      be placed in that particular neighbour cell information. */
   if(retVal == ROK)
     retVal = veCzFillCellModFrmCfgUpdReq(pdu);
   /* Case3 : Request contains the served cells to delete IE */
   /* In this case the cell ids received in the request will be deleted from
      neighbour cell list. */
   if(retVal == ROK)
     retVal = veCzFillCellDelFrmCfgUpdReq(pdu);
   /* Case4 : Request contains the GU group id to be added IE */
   if(retVal == ROK)
     retVal = veCzFillGuGrpIdAddFrmCfgUpdReq(pdu);
   /* Case5 : Request contains the GU group id to be deleted IE */
   if (retVal == ROK)
   {
#ifdef VE_TEST_CODE
#ifdef VE_TEST_CFG_UPD_RETX
     RETVALUE(ROK);
#endif /* VE_TEST_CFG_UPD_RETX */
#ifdef VE_TEST_CFG_UPD_FAIL
      /* Adding for testing failure */
      ret = veFillConfigUpdateFail(&cfgUpdRsp, NULLP, CztCauseProtunspecifiedEnum, CAUSE_PROTOCOL, 4); 
     printf("\n**** Sending config update failure ****\n");
#else
     /* Build eNB Configuration Acknowledge */
     ret = veFillConfigUpdateAck(&cfgUpdRsp , NULLP);
#endif /* VE_TEST_CFG_UPD_FAIL */
#else
     /* Build eNB Configuration Acknowledge */
      ret = veFillConfigUpdateAck(&cfgUpdRsp , NULLP);
#endif /* VE_TEST_CODE */
   }
   else
   {
     ret = veFillConfigUpdateFail(&cfgUpdRsp, NULLP, CztCauseProtunspecifiedEnum, CAUSE_PROTOCOL, 0);
   }

   if(ROK != ret)
   {
#ifdef DEBUGP
     VE_DBG_ERROR((veCb.init.prntBuf,"veLiCztCfgUpdReqHandler : Failed to Fill\
                              configuration update response"));
#endif
     RETVALUE(RFAILED);
   }
   
   cfgUpdRsp->peerId = neighCellCb->peerId;
   RETVALUE(VeLiCztGpRsp(&(veCb.cztSap[0]->pst), veCb.cztSap[0]->spId, cfgUpdRsp));
} /* end of veLiCztCfgUpdReqHandler */



/*
*
*       Fun: veCzFillCellAddFrmCfgUpdReq 
*
*       Desc:  Configuration Update Request Handler for adding served cells
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_ho.c
*
*/

#ifdef ANSI
PRIVATE S16 veCzFillCellAddFrmCfgUpdReq
(
CztEvnt     *cfgUpdReq
)
#else
PRIVATE S16 veCzFillCellAddFrmCfgUpdReq (cfgUpdReq)
CztEvnt     *cfgUpdReq;
#endif
{
   CztSrvdCells              *srvdCellsAdd = NULLP;
   CztNeighbour_Inform       *nghMember = NULLP;
   CztSrvdCell_Inform        *cellInfo = NULLP;
   U16                       numOfCells;
   U8                        idx = 0;
   VeNghCellCb               *nghCellCb = NULLP;
   CmLList                   *lstNode = NULLP;

   U16                        retVal = 0;

   TRC2(veCzFillCellAddFrmCfgUpdReq);

   veHOFndInNghLstByPeerId(veCb.cellCb[0], cfgUpdReq->peerId, &nghCellCb);
   if(NULLP == nghCellCb)
   {
#ifdef DEBUGP
     VE_DBG_ERROR((veCb.init.prntBuf,"veCzFillCellAddFrmCfgUpdReq:\
                    neighbour list does not exist. \n"));
#endif
      cmFreeMem((Ptr *)cfgUpdReq);
      RETVALUE(RFAILED);
   }

   /* Fill Served Cell To Add IE Information */
   veUtlGetCztIE(&cfgUpdReq->pdu, Cztid_SrvdCellsToAdd, (TknU8 **)&srvdCellsAdd);

   if(NULLP != srvdCellsAdd)
   {
     /* Filling the Neighbor Information */
     numOfCells = nghCellCb->numNghInfo;
     nghCellCb->numNghInfo = srvdCellsAdd->member[0].neighbour_Info.noComp.val +
                            numOfCells;
     if (nghCellCb->numNghInfo != 0) /* If no neighbour list was present earlier */
     {
       VE_ALLOC(&(nghCellCb->nghInfo), ((nghCellCb->numNghInfo - numOfCells) *
                              sizeof(VeNghInfo)));

       if(NULLP == nghCellCb->nghInfo)
       {
#ifdef DEBUGP
         VE_DBG_ERROR((veCb.init.prntBuf,"veCzFillCellAddFrmCfgUpdReq: memory allocation \
                        failed. \n"));
#endif
         RETVALUE(RFAILED);
       }
     }

     for(idx = numOfCells; idx < nghCellCb->numNghInfo; idx++)
     {
       nghMember = &(srvdCellsAdd->member[0].neighbour_Info);
       nghCellCb->nghInfo[idx]->earfcn = nghMember->member[idx].eARFCN.val;
       nghCellCb->nghInfo[idx]->physCellId = nghMember->member[idx].pCI.val;
       cmMemcpy((U8 *)&nghCellCb->nghInfo[idx]->ecgi.eutranCellId,
                  nghMember->member[idx].eCGI.eUTRANcellIdentifier.val,
                  nghMember->member[idx].eCGI.eUTRANcellIdentifier.len);
       VE_GET_U32_FRM_OSXL(nghCellCb->nghInfo[idx]->ecgi.plmnId,
                  nghMember->member[idx].eCGI.pLMN_Identity);
     }

     /* Filling the Served Cell Information */
     cellInfo = &(srvdCellsAdd->member[0].servedCellInfo);
     nghCellCb->veCellPrp.physCellId = cellInfo->pCI.val;
     cmMemcpy((U8*)&nghCellCb->veCellPrp.tac, cellInfo->tAC.val,
                             cellInfo->tAC.len);
     cmMemcpy((U8*)&nghCellCb->veCellPrp.cellId,
                             cellInfo->cellId.eUTRANcellIdentifier.val,
                             cellInfo->cellId.eUTRANcellIdentifier.len);
     nghCellCb->veCellPrp.numBcastPlmn = cellInfo->broadcastPLMNs.noComp.val;
     for(idx = 0; idx < nghCellCb->veCellPrp.numBcastPlmn; idx++)
     {
       cmMemcpy((U8*)&nghCellCb->veCellPrp.plmnId[idx],
         cellInfo->broadcastPLMNs.member[idx].val,
         cellInfo->broadcastPLMNs.member[idx].len);
     }
     nghCellCb->veCellPrp.duplexMode = cellInfo->eUTRA_Mode_Info.choice.val;
     if(EUTRA_MODE_INFO_FDD == nghCellCb->veCellPrp.duplexMode)
     {
       nghCellCb->veCellPrp.u.fddCb.dlEarfcn = cellInfo->eUTRA_Mode_Info.val.
                             fDD.dL_EARFCN.val;
       nghCellCb->veCellPrp.u.fddCb.ulEarfcn = cellInfo->eUTRA_Mode_Info.val.
                             fDD.uL_EARFCN.val;
       nghCellCb->veCellPrp.u.fddCb.dlTxBndWdth =
         cellInfo->eUTRA_Mode_Info.val.fDD.dL_Transmission_Bandwidth.val;
       nghCellCb->veCellPrp.u.fddCb.ulTxBndWdth =
         cellInfo->eUTRA_Mode_Info.val.fDD.uL_Transmission_Bandwidth.val;
     }
     else
     {
       nghCellCb->veCellPrp.u.tddCb.earfcn = cellInfo->eUTRA_Mode_Info.val.tDD.
                             eARFCN.val;
       nghCellCb->veCellPrp.u.tddCb.txBndWdth =
                  cellInfo->eUTRA_Mode_Info.val.tDD.transmission_Bandwidth.val;
      nghCellCb->veCellPrp.u.tddCb.splSfInfo.dlCycPrefix =
       cellInfo->eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixDL.val;
       nghCellCb->veCellPrp.u.tddCb.splSfInfo.splSfPatrn =
                        cellInfo->eUTRA_Mode_Info.val.tDD.specialSubframe_Info.
                        specialSubframePatterns.val;
       nghCellCb->veCellPrp.u.tddCb.splSfInfo.ulCycPrefix =
       cellInfo->eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixUL.val;

       nghCellCb->veCellPrp.u.tddCb.sfAssgnmnt =
         cellInfo->eUTRA_Mode_Info.val.tDD.subframeAssignment.val;
     }


     nghCellCb->peerId = cfgUpdReq->peerId; 
     nghCellCb->isX2ConnAvl = TRUE;

     /* add to nghCellLst of the corresponding cell */
     nghCellCb->lnk.node = (PTR)nghCellCb;
     cmLListAdd2Tail(&(veCb.cellCb[0]->nghCellLst), &nghCellCb->lnk);
   } /* end of if */
   else
   {

#ifdef DEBUGP
      VE_DBG_ERROR((veCb.init.prntBuf,"veCzFillCellAddFrmCfgUpdReq:no served cells \
                        to add. \n"));
#endif     
   }
   RETVALUE(ROK);
} /* end of veCzFillCellAddFrmCfgUpdReq */




/*
*
*       Fun: veCzFillCellModFrmCfgUpdReq
*
*       Desc:  Configuration Update Request Handler for modifying served cells
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_ho.c
*
*/

#ifdef ANSI
PRIVATE S16 veCzFillCellModFrmCfgUpdReq
(
CztEvnt     *cfgUpdReq
)
#else
PRIVATE S16 veCzFillCellModFrmCfgUpdReq (cfgUpdReq)
CztEvnt     *cfgUpdReq;
#endif
{
   CztSrvdCellsToMdfy        *srvdCellsMod = NULLP;
   CztNeighbour_Inform       *nghMember = NULLP;
   CztSrvdCell_Inform        *cellInfo = NULLP;
   CztECGI                   *oldEcgi = NULLP;
   U8                        idx = 0;
   VeNghCellCb               *nghCellCb = NULLP;
   CmLList                   *lstNode = NULLP;

   U16                        retVal = 0;

   TRC2(veCzFillCellModFrmCfgUpdReq);

   /* Fill Served Cell To Add IE Information */
   veUtlGetCztIE(&cfgUpdReq->pdu,Cztid_SrvdCellsToMdfy,(TknU8 **)&srvdCellsMod);

   if(NULLP != srvdCellsMod)
   {
     veHOFndInNghLstByPeerId(veCb.cellCb[0], cfgUpdReq->peerId, &nghCellCb);
     if(NULLP == nghCellCb)
     {
#ifdef DEBUGP
      VE_DBG_ERROR((veCb.init.prntBuf,"veCzFillCellAddFrmCfgUpdReq:\
                    neighbour list does not exist. \n"));
#endif
       cmFreeMem((Ptr *)cfgUpdReq);
       RETVALUE(RFAILED);
     }

     /* Filling the Neighbor Information */
     for(idx = 0; idx < nghCellCb->numNghInfo; idx++)
     {
         nghMember = &(srvdCellsMod->member[0].neighbour_Info);
         nghCellCb->nghInfo[idx]->earfcn = nghMember->member[idx].eARFCN.val;
         nghCellCb->nghInfo[idx]->physCellId = nghMember->member[idx].pCI.val;
         cmMemcpy((U8 *)&nghCellCb->nghInfo[idx]->ecgi.eutranCellId,
                  nghMember->member[idx].eCGI.eUTRANcellIdentifier.val,
                  nghMember->member[idx].eCGI.eUTRANcellIdentifier.len);
         VE_GET_U32_FRM_OSXL(nghCellCb->nghInfo[idx]->ecgi.plmnId,
                  nghMember->member[idx].eCGI.pLMN_Identity);
     }

     /* Filling the Served Cell Information */
     cellInfo = &(srvdCellsMod->member[0].servedCellInfo);
     nghCellCb->veCellPrp.physCellId = cellInfo->pCI.val;
     cmMemcpy((U8*)&nghCellCb->veCellPrp.tac, cellInfo->tAC.val,
                             cellInfo->tAC.len);
     cmMemcpy((U8*)&nghCellCb->veCellPrp.cellId,
                             cellInfo->cellId.eUTRANcellIdentifier.val,
                             cellInfo->cellId.eUTRANcellIdentifier.len);
     nghCellCb->veCellPrp.numBcastPlmn = cellInfo->broadcastPLMNs.noComp.val;
     for(idx = 0; idx < nghCellCb->veCellPrp.numBcastPlmn; idx++)
     {
       cmMemcpy((U8*)&nghCellCb->veCellPrp.plmnId[idx],
         cellInfo->broadcastPLMNs.member[idx].val,
        cellInfo->broadcastPLMNs.member[idx].len);
     }

     nghCellCb->veCellPrp.duplexMode = cellInfo->eUTRA_Mode_Info.choice.val;
     if(EUTRA_MODE_INFO_FDD == nghCellCb->veCellPrp.duplexMode)
     {
       nghCellCb->veCellPrp.u.fddCb.dlEarfcn = cellInfo->eUTRA_Mode_Info.val.
                             fDD.dL_EARFCN.val;
       nghCellCb->veCellPrp.u.fddCb.ulEarfcn = cellInfo->eUTRA_Mode_Info.val.
                             fDD.uL_EARFCN.val;
       nghCellCb->veCellPrp.u.fddCb.dlTxBndWdth = cellInfo->eUTRA_Mode_Info.val.
                             fDD.dL_Transmission_Bandwidth.val;
       nghCellCb->veCellPrp.u.fddCb.ulTxBndWdth = cellInfo->eUTRA_Mode_Info.val.
                             fDD.uL_Transmission_Bandwidth.val;
     }
     else
     {
       nghCellCb->veCellPrp.u.tddCb.earfcn = cellInfo->eUTRA_Mode_Info.val.
                             tDD.eARFCN.val;
       nghCellCb->veCellPrp.u.tddCb.txBndWdth = cellInfo->eUTRA_Mode_Info.
                             val.tDD.transmission_Bandwidth.val;

       nghCellCb->veCellPrp.u.tddCb.splSfInfo.dlCycPrefix =
       cellInfo->eUTRA_Mode_Info.val.tDD.specialSubframe_Info.cyclicPrefixDL.val;
       nghCellCb->veCellPrp.u.tddCb.splSfInfo.splSfPatrn =
                             cellInfo->eUTRA_Mode_Info.val.tDD.
                             specialSubframe_Info.specialSubframePatterns.val;
       nghCellCb->veCellPrp.u.tddCb.splSfInfo.ulCycPrefix =
                             cellInfo->eUTRA_Mode_Info.val.tDD.
                             specialSubframe_Info.cyclicPrefixUL.val;

       nghCellCb->veCellPrp.u.tddCb.sfAssgnmnt =
                             cellInfo->eUTRA_Mode_Info.val.tDD.
                             subframeAssignment.val;
     }
   } /* end of if */
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((veCb.init.prntBuf,"veCzFillCellAddFrmCfgUpdReq:\
                   served cell to modify does not exist in the neighbour list. \n"));
#endif
   }
   RETVALUE(ROK);
} /* end of veCzFillCellModFrmCfgUpdReq */



/*
*
*       Fun: veCzFillCellDelFrmCfgUpdReq
*
*       Desc:  Configuration Update Request Handler for deleting served cells
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_ho.c
*
*/

#ifdef ANSI
PRIVATE S16 veCzFillCellDelFrmCfgUpdReq
(
CztEvnt     *cfgUpdReq
)
#else
PRIVATE S16 veCzFillCellDelFrmCfgUpdReq (cfgUpdReq)
CztEvnt     *cfgUpdReq;
#endif
{
   CztOld_ECGIs              *srvdCellsDel = NULLP;
   CztNeighbour_Inform       *nghMember = NULLP;
   CztSrvdCell_Inform        *cellInfo = NULLP;
   CztECGI                   *oldEcgi = NULLP;
   U8                        idx = 0;
   VeNghCellCb               *nghCellCb = NULLP, *neighCellCb2 = NULLP;
   CmLList                   *lstNode = NULLP;

   U16                        retVal = 0;

   TRC2(veCzFillCellDelFrmCfgUpdReq);

   /* Fill Served Cell To Add IE Information */
   veUtlGetCztIE(&cfgUpdReq->pdu,Cztid_SrvdCellsToDelete,(TknU8 **)&srvdCellsDel);

   if(NULLP != srvdCellsDel)
   {
     veHOFndInNghLstByPeerId(veCb.cellCb[0], cfgUpdReq->peerId, &nghCellCb);
     if(NULLP == nghCellCb)
     {
#ifdef DEBUGP
       VE_DBG_ERROR((veCb.init.prntBuf,"veCzFillCellAddFrmCfgUpdReq:\
                    neighbour list does not exist. \n"));
#endif
       cmFreeMem((Ptr *)cfgUpdReq);
       RETVALUE(RFAILED);
     }


     lstNode = veCb.cellCb[0]->nghCellLst.first;
     while(NULL != lstNode)
     {
        nghCellCb = (VeNghCellCb*)lstNode->node;
         /* remove the node from the candLst */
         cmLListDelFrm(&(veCb.cellCb[0]->nghCellLst), lstNode);
         /* Free the memory for that neighbor cell Cb */
         VE_FREE(lstNode->node, sizeof(VeNghCellCb));
         break;
     }
   }/* end of if */
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((veCb.init.prntBuf,"veCzFillCellDelFrmCfgUpdReq:\
                   served cell to delete does not exist in the neighbour list. \n"));
#endif

   }

   RETVALUE(ROK);
}/* end of veCzFillCellDelFrmCfgUpdReq */



/*
*
*       Fun: veCzFillGuGrpIdAddFrmCfgUpdReq 
*
*       Desc:  Configuration Update Request Handler for adding GU Group ID
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_ho.c
*
*/
#ifdef ANSI
PRIVATE S16 veCzFillGuGrpIdAddFrmCfgUpdReq
(
CztEvnt     *cfgUpdReq
)
#else
PRIVATE S16 veCzFillGuGrpIdAddFrmCfgUpdReq (cfgUpdReq)
CztEvnt     *cfgUpdReq;
#endif
{
   CztGUGroupIDLst           *guGrpIdAdd = NULLP;
   CztPLMN_Identity          *plmnId = NULLP;
   CztMME_Group_ID           *mmeGrpId = NULLP;
   U8                        idx = 0, numOfGrp = 0;

   VeNghCellCb               *nghCellCb = NULLP;
   CmLList                   *lstNode = NULLP;

   U16                        retVal = 0;

   TRC2(veCzFillGuGrpIdAddFrmCfgUpdReq);
   veHOFndInNghLstByPeerId(veCb.cellCb[0], cfgUpdReq->peerId, &nghCellCb);
   if(NULLP == nghCellCb)
   {
#ifdef DEBUGP
     VE_DBG_ERROR((veCb.init.prntBuf,"veCzFillGuIdAddFrmCfgUpdReq:\
                    neighbour list does not exist. \n"));
#endif
      cmFreeMem((Ptr *)cfgUpdReq);
      RETVALUE(RFAILED);
   }

   /* Fill GU Group ID To Add IE Information */
   veUtlGetCztIE(&cfgUpdReq->pdu, Cztid_GUGroupIDToAddLst, (TknU8 **)&guGrpIdAdd);

   if(NULLP != guGrpIdAdd)
   {
     /* Fill GU Group ID */
     numOfGrp = nghCellCb->numguGrp;
     nghCellCb->numguGrp = guGrpIdAdd->noComp.val + numOfGrp;

     if (nghCellCb->numguGrp != 0) /* If no guGrp list was present earlier */
     {
       VE_ALLOC(&(nghCellCb->guGrpId), ((nghCellCb->numguGrp - numOfGrp) *
                              sizeof(VeNghInfo)));

       if(NULLP == nghCellCb->guGrpId)
       {
#ifdef DEBUGP
         VE_DBG_ERROR((veCb.init.prntBuf,"veCzFillGuIDAddFrmCfgUpdReq: memory allocation \
                        failed. \n"));
#endif
         RETVALUE(RFAILED);
       }
     }

     for(idx = numOfGrp; idx < nghCellCb->numguGrp; idx++)
     {
       nghCellCb->guGrpId[idx].mmeGrpId = guGrpIdAdd->member[idx].mME_Group_ID.val;
       VE_GET_U32_FRM_OSXL(nghCellCb->guGrpId[idx].plmnId,
                 guGrpIdAdd->member[idx].pLMN_Identity);
     }
   } /*end of if */
  else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((veCb.init.prntBuf,"veCzFillGuIDAddFrmCfgUpdReq:\
                   no GU Group Id to add. \n"));
#endif

   }
   RETVALUE(ROK);
} /* end of veCzFillGuGrpIdAddFrmCfgUpdReq */




/*
*
*       Fun: veCzFillGuGrpIdDelFrmCfgUpdReq
*
*       Desc:  Configuration Update Request Handler for deleting GU Group ID
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_ho.c
*
*/
#ifdef ANSI
PRIVATE S16 veCzFillGuGrpIdDelFrmCfgUpdReq
(
CztEvnt     *cfgUpdReq
)
#else
PRIVATE S16 veCzFillGuGrpIdDelFrmCfgUpdReq (cfgUpdReq)
CztEvnt     *cfgUpdReq;
#endif
{
   CztGUGroupIDLst           *guGrpIdDel = NULLP;
   CztMME_Group_ID           *mmeGrpId = NULLP;
   U8                        idx = 0;
   VeNghCellCb               *nghCellCb = NULLP;
   CmLList                   *lstNode = NULLP;
   U32                        plmnId;
   U16                        retVal = 0;
   
   TRC2(veCzFillGuGrpIdDelFrmCfgUpdReq);
   veHOFndInNghLstByPeerId(veCb.cellCb[0], cfgUpdReq->peerId, &nghCellCb);
   if(NULLP == nghCellCb)
   {
#ifdef DEBUGP
     VE_DBG_ERROR((veCb.init.prntBuf,"veCzFillGuGrpIdDelFrmCfgUpdReq:\
                    neighbour list does not exist. \n"));
#endif
      cmFreeMem((Ptr *)cfgUpdReq);
      RETVALUE(RFAILED);
   }

   /* Fill GU Group ID To Del IE Information */
   veUtlGetCztIE(&cfgUpdReq->pdu, Cztid_GUGroupIDToDeleteLst, (TknU8 **)&guGrpIdDel);

   if(NULLP != guGrpIdDel)
   {
      for(idx=0; idx<nghCellCb->numguGrp; idx++)
      {
         VE_GET_U32_FRM_OSXL(plmnId,
                 guGrpIdDel->member[idx].pLMN_Identity);

         if((nghCellCb->guGrpId[idx].plmnId == plmnId) &&
             (nghCellCb->guGrpId[idx].mmeGrpId == guGrpIdDel->member[idx].mME_Group_ID.val ))
         {
           /* TODO : remove from the list*/
         }
      }

   }/* end of if */
  else 
   {
#ifdef DEBUGP
      VE_DBG_ERROR((veCb.init.prntBuf,"veCzFillGuIDDelFrmCfgUpdReq:\
                   no GU Group Id to delete. \n"));
#endif
   }

   RETVALUE(ROK);
} /* end of veCzFillGuGrpIdDelFrmCfgUpdReq */





/*
*
*       Fun: veLiCztcfgUpdAckHandler
*
*       Desc:  Configuration Update Ack Handler
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_ho.c
*
*/
#ifdef ANSI
PUBLIC S16 veLiCztCfgUpdAckHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *cfgAckPdu
)
#else
PUBLIC S16 veLiCztCfgUpdAckHandler(x2ConCb,cfgAckPdu)
VeX2ConCb *x2ConCb;
CztEvnt   *cfgAckPdu;
#endif
{
   VeNghCellCb *neighCellCb = NULLP;
   CztCriticalityDiag        *critDiag = NULLP;

   TRC2(veLiCztCfgUpdAckHandler);

   veHOFndInNghLstByPeerId(veCb.cellCb[0], cfgAckPdu->peerId, &neighCellCb);
   if(NULLP == neighCellCb)
   {
      VE_DBG_ERROR((veCb.init.prntBuf,"veLiCztX2SetupRspHandler:\
                      invalid setup responde from non existing peer\n"));
      cmFreeMem((Ptr)cfgAckPdu);
      RETVALUE(RFAILED);
   }

   /* stop the timer */
   veStopTmr((PTR)neighCellCb, VE_TMR_X2_CFGUPD_TMR);


#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztCfgUpdAckHandler:\
            Received eNodeB Configuration Update Ack from the peer. \n"));
#endif

   /* If the message contains criticality diagnostic IE the process it */
   veUtlGetCztIE( &(cfgAckPdu->pdu), Cztid_CriticalityDiag, (TknU8 **)&critDiag);

   if(critDiag)
   {
     /* veProcessCritDiagIE();*/ /*TODO*/
   }
   cmFreeMem((Ptr)cfgAckPdu);
   RETVALUE(ROK);
}/* end of veLiCztCfgUpdAckHandler */




/*
*
*       Fun: veLiCztcfgUpdFailHandler
*
*       Desc:  Configuration Update Ack Handler
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_ho.c
*
*/
#ifdef ANSI
PUBLIC S16 veLiCztCfgUpdFailHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *cfgFailPdu
)
#else
PUBLIC S16 veLiCztCfgUpdFailHandler(x2ConCb,cfgFailPdu)
VeX2ConCb *x2ConCb;
CztEvnt   *cfgFailPdu;
#endif
{
   CztCause                  *cause = NULLP;
   CztCriticalityDiag        *critDiag = NULLP;
   CztTimeToWait             *timeToWait = NULLP;
   U32                       delay = 0;
   VeNghCellCb               *neighCellCb = NULLP;
   TRC2(veLiCztCfgUpdFailHandler);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztCfgUpdFailHandler:\
            eNodeB Configuration Update Failure\n"));
#endif

   veHOFndInNghLstByPeerId(veCb.cellCb[0], cfgFailPdu->peerId, &neighCellCb);
   if(NULLP == neighCellCb)
   {
      VE_DBG_ERROR((veCb.init.prntBuf,"veLiCztX2SetupRspHandler:\
                      invalid setup responde from non existing peer\n"));
      cmFreeMem((Ptr)cfgFailPdu);
      RETVALUE(RFAILED);
   }

   /* log the cause of X2 Setup Failure */
   veUtlGetCztIE(&cfgFailPdu->pdu, Cztid_Cause, (TknU8 **)&cause);
   VE_DBG_ERROR((veCb.init.prntBuf,"veLiCztCfgUpdFailHandler:\
                 eNodeB Configuration Update Failed. \n"));

   /* Process Time To Wait IE */
   veUtlGetCztIE(&cfgFailPdu->pdu, Cztid_TimeToWait, (TknU8 **)&timeToWait);
   if ( timeToWait != NULLP )
   {
      /* Stop if the timer is already running */
      veStopTmr((PTR) neighCellCb, VE_TMR_X2_CFGUPD_TMR);

      switch( timeToWait->val )
      {
         case 0:
            delay = (1 * SS_TICKS_SEC)/ VE_TMR_RES;
            break;
         case 1:
            delay = (2 * SS_TICKS_SEC)/ VE_TMR_RES;
            break;
         case 2:
            delay = (5 * SS_TICKS_SEC)/ VE_TMR_RES;
            break;
         case 3:
            delay = (10 * SS_TICKS_SEC)/ VE_TMR_RES;
            break;
         case 4:
            delay = (20 * SS_TICKS_SEC)/ VE_TMR_RES;
            break;
         case 5:
            delay = (60 * SS_TICKS_SEC)/ VE_TMR_RES;
            break;
         default:
         VE_DBG_ERROR((veCb.init.prntBuf,"veLiCztCfgUpdFailHandler:\
                  Invalid Cztid_TimeToWait. \n")); 
         RETVALUE (RFAILED);
         break;
      }

      /* start the timer with new delay */
     neighCellCb->maxCfgupdRetry = 0xFF;
     veStartTmr((PTR) neighCellCb, VE_TMR_X2_CFGUPD_TMR, delay);

   }


   /* If the message contains criticality diagnostic IE the process it */
   veUtlGetCztIE( &(cfgFailPdu->pdu), Cztid_CriticalityDiag, (TknU8 **)&critDiag);

   if(critDiag)
   {
      /*veProcessCritDiagIE();*/ /*TODO*/
   }

  cmFreeMem((Ptr)cfgFailPdu);
   RETVALUE(ROK);
}/* end of veLiCztCfgUpdFailHandler */
                                                                                                                                                                                                                                                                                                                                                                                                                                           
/*
*
*       Fun: veLiCztErrIndHandler
*
*       Desc:  Error Indication Handler
*
*       Ret:   ROK
*  
*       Notes: None
*
*       File:  ve_ho.c
*  
*/
#ifdef ANSI 
PUBLIC S16 veLiCztErrIndHandler
(
VeX2ConCb *x2ConCb,
CztEvnt   *cfgErrIndPdu
)
#else
PUBLIC S16 veLiCztErrIndHandler(x2ConCb,cfgErrIndPdu)
VeX2ConCb *x2ConCb;
CztEvnt   *cfgErrIndPdu;
#endif
{

   TRC2(veLiCztErrIndHandler);
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veLiCztErrIndHandler:\
            Received Error Indication from the peer. \n"));
#endif

   cmFreeMem((Ptr)cfgErrIndPdu);
   RETVALUE(ROK);
}/* end of veLiCztErrIndHandler */

/* X2_CFG_UPD_CHANGES END*/



#endif /* End of LTE_HO_SUPPORT */
/********************************************************************30**

         End of file:     ve_ho.c@@/main/eNB_1.3_With_LatestGpr3/20 - Mon Sep 19 16:35:11 2011

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

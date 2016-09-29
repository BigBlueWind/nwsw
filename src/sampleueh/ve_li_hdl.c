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
  
     Desc:     Lwer Layer Interface Handlers

     Ret :     ROK - success
               RFAILED - failure

     File:     ve_li_hdl.c 

     Sid:      ve_li_hdl.c@@/main/2 - Wed Dec  1 08:44:26 2010

     Prg:      rbhat (RRC) & kpawar (s1ap) 

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
#include "cm_inet.h"
#include "cm_tpt.h"

#include "nhu_asn.h"
#include "nhu.h"           /* NHU interface defines */
#include "pju.h"           /* PDCP data interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"           /* PHY RRM control Interface */
#include "szt.h"           /* S1AP RRM control Interface */
#ifdef EU_DAT_APP
#include "egt.h"           /* S1AP RRM control Interface */
#endif 
#include "lrg.h"           /* MAC RRM control Interface */
#include "ve_eut.h"
#include "ve_sm_init.h"

/* RRM related includes */
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
#include "cm_inet.x"
#include "cm_tpt.x"

#include "nhu_asn.x"
#include "nhu.x"           /* NHU Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "ctf.x"           /* PHY RRM control Interface */
#include "szt.x"           /* S1AP RRM control Interface */
#ifdef LTE_HO_SUPPORT
#include "czt.x"
#include "czt_asn.x"
#endif
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "lrg.x"           /* MAC RRM control Interface */
#include"ve_eut.x"

/* RRM related includes */
#include "lve.x" 
#include "ve.x" 
#include "ve_sm_init.x" 
/* MSPD changes */
#ifndef MSPD
#include "ve_dflt.h"
#else
#include "ve_dflt_ms.x"
#endif

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif
#ifdef VE_PICO
#include "ve_debug.h"
#endif
EXTERN VeCb veCb;
EXTERN VeNhuLIMFSM veNhuLIMFSM[VE_NH_MAX_MSG_CLASS][VE_NH_MAX_MSG_TYPE];
#ifdef LTE_HO_SUPPORT
#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */
EXTERN VeS1TgtFSM veS1TgtFSM[VE_HO_MAX_STATES][VE_HO_TGT_MAX_EVENTS];
EXTERN VeX2TgtFSM veX2TgtFSM[VE_HO_MAX_STATES][VE_HO_TGT_MAX_EVENTS];
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
/* RRC-ReEstab */
PRIVATE S16 veSndReestReply ARGS ((NhuDatIndSdus *datIndSdu,VeCellCb *cellCb,VeUeCb *ueCb,Bool sndAccpt,U8 ueId));
PRIVATE S16 veSndCtfUeReCfg ARGS((VeUeCb        *ueCb,CmLteRnti     oldCrnti));
 

/* TIC_ID :ccpu00117545 ADD : Fixed warning for gcc compilation on CentOS */

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */
EXTERN S16 nhEDMEncReq ARGS((Void     *pdu,
                             U8       msgCategory,
                             Buffer   **mBuf,
                             U32      *length));
EXTERN S16 veSndRgrUeIdReCfg ARGS ((VeUeCb *ueCb,U8 oldCrnti));
#ifdef __cplusplus
}
#endif /* __cplusplus */


/* TIC_ID :ccpu00117545 ADD_END : Fixed warning for gcc compilation on CentOS */

/*
 *
 *      Fun:   veHdlNhuDatInd
 *
 *      Desc:  Dat Ind
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veHdlNhuDatInd
(
Pst     *pst,          /* post structure */
SuId     suId,
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 veHdlNhuDatInd(pst, suId, datIndSdu)
Pst     *pst;          /* post structure */
SuId     suId;
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
   NhuUL_CCCH_Msg  *ulCcchPdu = NULLP;  /* UL CCCH structure */
   NhuUL_DCCH_Msg  *ulDcchPdu = NULLP;  /* UL DCCH structure */
   U8   messageType = 0;
   U8   messageCat = 0;

   TRC2(veHdlNhuDatInd);
#ifdef YS_PICO
   SEND_DBG_MSG("Received Data Indication %s:%d\n", __FILE__, __LINE__);
#endif
   /* If message is received on CCCH */
   messageCat = datIndSdu->sdu.msgCategory;

   if (NHU_MSG_ULCCCH == messageCat)
   {
#ifdef YS_PICO
      SEND_DBG_MSG("Received ULCCCH Data Indication %s:%d\n", __FILE__, __LINE__);
#endif
      ulCcchPdu = &datIndSdu->sdu.m.ulCcchMsg.ccchMsg;
      if ((FALSE == ulCcchPdu->pres.pres) )
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veHdlNhuDatInd : Unknown Msg. Received.\n"));
         RETVALUE(RFAILED);
      } 

      messageType = ulCcchPdu->message.val.c1.choice.val;

      /*Call the state machine*/
      veNhuLIMFSM[messageCat][messageType](datIndSdu);

   }  /* end of if (NHU_MSG_ULCCCH == datIndSdu.sdu.msgCategory) */
   /* If message is received on DCCH */
   else if (NHU_MSG_ULDCCH == datIndSdu->sdu.msgCategory)
   { 
#ifdef YS_PICO
      SEND_DBG_MSG("Received ULDCCH Data Indication %s:%d\n", __FILE__, __LINE__);
#endif
      ulDcchPdu = &(datIndSdu->sdu.m.ulDcchMsg.dcchMsg);
      if ((FALSE == ulDcchPdu->pres.pres) )
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veHdlNhuDatInd : Message not Present.\n"));
         RETVALUE(RFAILED);
      }

      messageType = ulDcchPdu->message.val.c1.choice.val;
#ifdef YS_PICO
      SEND_DBG_MSG("Sending Message Type %d\n", messageType);
#endif
      if (messageType == 2) veLIMRrcReconfigCmpHndlr((NhuDatIndSdus*)datIndSdu);
      else
      /*Call the state machine*/
      veNhuLIMFSM[messageCat][messageType]((NhuDatIndSdus *)datIndSdu);
   }

   RETVALUE(ROK);
}/* veHdlNhuDatInd */

#if   !defined(SI_NEW) && defined(RGR_RRM_TICK)

/*
 *
 *      Fun:   veSndCmnNhuDatReq
 *
 *      Desc:  Common Dat Req
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndCmnNhuDatReq
(
U8    msgCategory,
U8    msgType,
CmLteTimingInfo tmnInf
)
#else
PUBLIC S16 veSndCmnNhuDatReq(msgCategory, msgType, tmnInf)
U8    msgCategory;
U8    msgType;
CmLteTimingInfo tmnInf;
#endif
{
   Pst           *pst = NULLP;
   S16           ret = RFAILED;
   U32           cellId = 0;

   NhuDatReqSdus *nhuDatReqEvntSdu = NULLP;

   TRC2(veSndCmnNhuDatReq);

   VE_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));

   pst = &veCb.nhuSap[0]->pst;
   pst->event   = EVTNHUDATREQ;

   /* Cell ID */
   cellId = veCb.cellCb[0]->cellId;/* warning fix */

   /* Fill HDR Info */
   veFillNhuEvntHdr(&(nhuDatReqEvntSdu->hdr), cellId, VE_VAL_ZERO, msgType);
   /* No ue specific info */
   nhuDatReqEvntSdu->sdu.isUeCfgPres = FALSE;
   /* call respective functions to build messages */
   switch(msgCategory)
   {
      case NHU_MSG_DLBCCHBCH:
         {
            /* Fill the message category */
            nhuDatReqEvntSdu->sdu.ueMsg.isMsgDed = FALSE;
            nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory = 
               NHU_MSG_DLBCCHBCH;
            nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.tbsSize = 
               veDfltBchTbsSz;

            switch(msgType)
            {
               case VE_SYS_MIB:
                  /* Master info block */
                  nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId = 
                     VE_BCCH_BCH_ID;
                  /* fill the message */
                  ret = veSibBldMIBMsg(nhuDatReqEvntSdu, tmnInf.sfn);
                  if (ret != ROK)
                  {
#ifdef DEBUGP
                     VE_DBG_ERROR((VE_PRNT_BUF,"veSndCmnNhuDatReq: veSibBldMIBMsg\n"));
#endif
                     VE_FREEEVNT(nhuDatReqEvntSdu);
                     RETVALUE(RFAILED);
                  }
                  break;

               default:
                  break;
            }
         }
         break;

      case NHU_MSG_DLBCCHDLSCH:
         {
            /* Fill the message category */
            nhuDatReqEvntSdu->sdu.ueMsg.isMsgDed = FALSE;
            nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory = 
               NHU_MSG_DLBCCHDLSCH;
            nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.tbsSize = 
               veDfltDlschTbsSz;

            switch(msgType)
            {
               case VE_SYS_SIB1:
                  /* Master info block */
                  nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId = 
                     VE_BCCH_SIB1_ID;
                  /* fill the message */
                  ret = veSibBldSIB1Msg(nhuDatReqEvntSdu, tmnInf.sfn);
                  if (ret != ROK)
                  {
#ifdef DEBUGP
                     VE_DBG_ERROR((VE_PRNT_BUF,"veSndCmnNhuDatReq: veSibBldSIB1Msg Failed \n"));
#endif
                     VE_FREEEVNT(nhuDatReqEvntSdu);
                     RETVALUE(RFAILED);
                  }
                  break;

               case VE_SYS_SIB2:
                  nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId =
                     VE_BCCH_SIB1_ID;
                  /* fill the message */
                  ret = veSibBldSIB2Msg(nhuDatReqEvntSdu, tmnInf.sfn,\
                        tmnInf.subframe);
                  if (ret != ROK)
                  {
#ifdef DEBUGP
                     VE_DBG_ERROR((VE_PRNT_BUF,"veSndCmnNhuDatReq: veSibBldSIB2Msg Failed \n"));
#endif
                     VE_FREEEVNT(nhuDatReqEvntSdu);
                     RETVALUE(RFAILED);
                  }
                  break;

               default:
                  break;
            }
         }
         break;

      default:
         /* Shouldnt be here */
         break;
   }

#ifndef SI_NEW
   /* Issue the primitive */
   if(ROK != VeLiNhuDatReq(pst, veCb.nhuSap[0]->spId, nhuDatReqEvntSdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndCmnNhuDatReq : VeLiNhuDatReq failed.\n"));      
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE(ROK);
} /* veSndCmnNhuDatReq */
#endif

#if (defined(SI_NEW) && defined(RGR_RRM_TICK))
/*
 *
 *      Fun:   veSibBldPdu
 *
 *      Desc:  build SI PDU 
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSibBldPdu
(
U8       msgCategory,
U8       msgType,
Buffer   **encBuf
)
#else
PUBLIC S16 veSibBldPdu(msgCategory, msgType, encBuf)
U8       msgCategory;
U8       msgType;
Buffer   **encBuf;
#endif
{
   S16           ret = RFAILED;
   U32           cellId = 0;
   Void          *pdu;
   NhuCmnMsg     *cmnMsg;
   U32           length;
   S16           idx;
   S16           padOctets;
   U32           msgOctets;


   NhuDatReqSdus *nhuDatReqEvntSdu = NULLP;

   TRC2(veSibBldPdu);

   VE_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));

   /* Cell ID */
   cellId = veCb.cellCb[0]->cellId;

   /* Fill HDR Info */
   veFillNhuEvntHdr(&(nhuDatReqEvntSdu->hdr), cellId, VE_VAL_ZERO, msgType);
   /* No ue specific info */
   nhuDatReqEvntSdu->sdu.isUeCfgPres = FALSE;
   /* call respective functions to build messages */
   switch(msgCategory)
   {
      case NHU_MSG_DLBCCHBCH:
         {
            /* Fill the message category */
            nhuDatReqEvntSdu->sdu.ueMsg.isMsgDed = FALSE;
            nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory = 
               NHU_MSG_DLBCCHBCH;
            nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.tbsSize = 
               veDfltBchTbsSz;

            switch(msgType)
            {
               case VE_SYS_MIB:
                  /* Master info block */
                  nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId = 
                     VE_BCCH_BCH_ID;
                  /* fill the message */
                  ret = veSibBldMIBMsg(nhuDatReqEvntSdu, 0);
                  if (ret != ROK)
                  {
#ifdef DEBUGP
                     VE_DBG_ERROR((VE_PRNT_BUF,"veSibBldPdu: veSibBldMIBMsg\n"));
#endif
                     VE_FREEEVNT(nhuDatReqEvntSdu);
                     RETVALUE(RFAILED);
                  }
                  break;

               default:
                  break;
            }
         }
         break;

      case NHU_MSG_DLBCCHDLSCH:
         {
            /* Fill the message category */
            nhuDatReqEvntSdu->sdu.ueMsg.isMsgDed = FALSE;
            nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory = 
               NHU_MSG_DLBCCHDLSCH;
            nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.tbsSize = 
               veDfltDlschTbsSz;

            switch(msgType)
            {
               case VE_SYS_SIB1:
                  /* Master info block */
                  nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId = 
                     VE_BCCH_SIB1_ID;
                  /* fill the message */
                  ret = veSibBldSIB1Msg(nhuDatReqEvntSdu, 0);
                  if (ret != ROK)
                  {
#ifdef DEBUGP
                     VE_DBG_ERROR((VE_PRNT_BUF,"veSibBldPdu: veSibBldSIB1Msg Failed \n"));
#endif
                     VE_FREEEVNT(nhuDatReqEvntSdu);
                     RETVALUE(RFAILED);
                  }
                  break;

               case VE_SYS_SIB2:
                  nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId =
                     VE_BCCH_SIB1_ID;
                  /* fill the message */
                  ret = veSibBldSIB2Msg(nhuDatReqEvntSdu, 0,\
                        0);
                  if (ret != ROK)
                  {
#ifdef DEBUGP
                     VE_DBG_ERROR((VE_PRNT_BUF,"veSibBldPdu: veSibBldSIB2Msg Failed \n"));
#endif
                     VE_FREEEVNT(nhuDatReqEvntSdu);
                     RETVALUE(RFAILED);
                  }
                  break;

               case VE_SYS_SIB3:
                  /* Master info block */
                  nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId =
                     VE_BCCH_SIB_OTHERS_ID;
                  /* fill the message */
                  ret = veSibBldSIB3Msg(nhuDatReqEvntSdu, 0,\
                        0);
                  if (ret != ROK)
                  {
#ifdef DEBUGP
                     VE_DBG_ERROR((VE_PRNT_BUF,"veSibBldPdu: veSibBldSIB3Msg Failed \n"));
#endif
                     VE_FREEEVNT(nhuDatReqEvntSdu);
                     RETVALUE(RFAILED);
                  }
                  break;

               case VE_SYS_SIB4:
                  /* Master info block */
                  nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId =
                     VE_BCCH_SIB_OTHERS_ID;
                  /* fill the message */
                  ret = veSibBldSIB4Msg(nhuDatReqEvntSdu, 0,\
                        0);
                  if (ret != ROK)
                  {
#ifdef DEBUGP
                     VE_DBG_ERROR((VE_PRNT_BUF,"veSibBldPdu: veSibBldSIB4Msg Failed \n"));
#endif
                     VE_FREEEVNT(nhuDatReqEvntSdu);
                     RETVALUE(RFAILED);
                  }
                  break;

               case VE_SYS_SIB5:
                  /* Master info block */
                  nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId =
                     VE_BCCH_SIB_OTHERS_ID;
                  /* fill the message */
                  ret = veSibBldSIB5Msg(nhuDatReqEvntSdu, 0,\
                        0);
                  if (ret != ROK)
                  {
#ifdef DEBUGP
                     VE_DBG_ERROR((VE_PRNT_BUF,"veSibBldPdu: veSibBldSIB5Msg Failed \n"));
#endif
                     VE_FREEEVNT(nhuDatReqEvntSdu);
                     RETVALUE(RFAILED);
                  }
                  break;

               default:
                  break;
            }
         }
         break;

      default:
         /* Shouldnt be here */
         break;
   }

   cmnMsg = &(nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg);
   /* Encode the System info */
   if (NHU_MSG_DLBCCHBCH == cmnMsg->cmnMsgCategory)
   {
      pdu = (Void *) &(cmnMsg->cmn.bcchBchMsg);
   }
   else
   {
      pdu = (Void *) &(cmnMsg->cmn.bcchDlSchMsg);
   }

   ret = nhEDMEncReq(pdu, cmnMsg->cmnMsgCategory, encBuf, &length);
   VE_FREEEVNT(nhuDatReqEvntSdu);
   if (ROK != ret)
   {
      /* Handle encode error here */
      VE_DBG_ERROR((VE_PRNT_BUF,"veSibBldPdu: encode PDU failed."));
      RETVALUE(ret);
   }

   /* Address padding requirement */
   msgOctets = (length/8) + ((length%8)==0? 0:1);
   padOctets = (S16) ((cmnMsg->cmnMsgInfo.tbsSize) - msgOctets);

   if (padOctets < 0)
   {
      /* Handle the length error */
      VE_DBG_ERROR((VE_PRNT_BUF,"veSibBldPdu: TBSz is smaller than encoded length."));
      VE_FREEMBUF(*encBuf);
      RETVALUE(RFAILED);
   }

   for (idx=0; idx < padOctets; idx++)
   {
      SAddPstMsg(0, *encBuf);
   }

   RETVALUE(ROK);
} /* veSibBldPdu */
#endif
/*
 *
 *      Fun:   veSndUENhuDatReq
 *
 *      Desc:  UE NHU Dat Req
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndUENhuDatReq
(
NhuDatReqSdus *evt
)
#else
PUBLIC S16 veSndUENhuDatReq(evt)
NhuDatReqSdus *evt;
#endif
{
   Pst           *pst = NULLP;

   TRC2(veSndUENhuDatReq);

   pst = &veCb.nhuSap[0]->pst;
   pst->event   = EVTNHUDATREQ;
   
   /* Issue the primitive */
   if(ROK != VeLiNhuDatReq(pst, veCb.nhuSap[0]->spId, evt))

   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndUENhuDatReq : VeLiNhuDatReq failed.\n"));      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndUENhuDatReq */
/* Ue Capability related functions */
#ifdef UE_RAD_CAP
/*
 *
 *      Fun:   veSndNhuDecodeReq
 *
 *      Desc:   NHU Decode Req
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndNhuDecodeReq
(
NhuDecodeSduReq *evt
)
#else
PUBLIC S16 veSndNhuDecodeReq(evt)
NhuDecodeSduReq *evt;
#endif
{
   Pst           *pst = NULLP;

   TRC2(veSndNhuDecodeReq);

   pst = &veCb.nhuSap[0]->pst;
   pst->event   = EVTNHUDCODEREQ;

   /* Issue the primitive */
   if(ROK != VeLiNhuDecodeReq(pst, veCb.nhuSap[0]->spId, evt))

   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndNhuDecodeReq : VeLiNhuDecodeReq failed.\n"));      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndNhuDecodeReq */
/*
 *
 *      Fun:   veSndNhuEncodeReq
 *
 *      Desc:   NHU Decode Req
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndNhuEncodeReq
(
NhuEncReqSdus *evt
)
#else
PUBLIC S16 veSndNhuEncodeReq(evt)
NhuEncReqSdus *evt;
#endif
{
   Pst           *pst = NULLP;

   TRC2(veSndNhuEncodeReq);

   pst = &veCb.nhuSap[0]->pst;
   pst->event   = EVTNHUENCODEREQ;

   /* Issue the primitive */
   if(ROK != VeLiNhuEncodeReq(pst, veCb.nhuSap[0]->spId, evt))

   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndNhuEncodeReq : VeLiNhuEncodeReq failed.\n"));      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndNhuEncodeReq */
#endif

/*
 *
 *      Fun:   veSndRrcConReqSetup
 *
 *      Desc:  RRC Con Req Setup
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veHdlNhuDatRsp
(
NhuDatRspSdus *nhuDatRspEvntSdu
)
#else
PUBLIC S16 veHdlNhuDatRsp(nhuDatRspEvntSdu)
NhuDatRspSdus *nhuDatRspEvntSdu;
#endif
{
   Pst           *pst = NULLP;

   TRC2(veHdlNhuDatRsp);

   pst = &veCb.nhuSap[0]->pst;
   pst->event   = EVTNHUDATREQ;

   /* Issue the primitive */
   if(ROK != VeLiNhuDatRsp(pst, veCb.nhuSap[0]->spId, nhuDatRspEvntSdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veHdlNhuDatRsp : VeLiNhuDatRsp failed.\n"));      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veHdlNhuDatRsp */

/* ==================================== */
/* RRC Downlink Message Handlers  */
/* ==================================== */

/*
 *
 *      Fun:   veSndRrcConReqSetup
 *
 *      Desc:  RRC Con Req Setup
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndRrcConReqSetup
(
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 veSndRrcConReqSetup( datIndSdu)
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
   S16 ret = RFAILED;
   U8   conResId[NHU_CONRES_ID_LEN];
   CmLList   *tmpUePagingInfo = NULLP;

   VeCellCb *cellCb = NULLP; 
   VeUeCb *ueCb = NULLP;
   NhuInitUE_Identity *ueID = NULLP;
   NhuDatRspSdus *nhuDatRspEvntSdu = NULLP;
   NhuUeCfgInfo  *ueCfg = NULLP;
   U16        ueIdx = 0;
   U16        cnt = 0;
   VeMmeCb   *mmeCb = NULLP;
   Bool mmeStatus = FALSE;

   TRC2(veSndRrcConReqSetup);

#ifdef VE_PICO
   /* Enabling Call Scenario Prints ccpu00117125*/
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "UE -> ENB : RRC Connection Request Message");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");

   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "ENB -> UE : RRC Connection Setup");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
#endif
   if(smCb.smState == VE_SM_STATE_AWAIT_S1_CON)
   {
      /* S1 State Down; Ignore Connection Req */
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConReqSetup:\
               S1 State Down; Ignore Connection Req \n"));
#endif
      RETVALUE(ROK);
   }

   /*Create UE control block*/
   VE_ALLOC(&ueCb, sizeof(VeUeCb));
   /* klock warning fixed */
   if(ueCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConReqSetup:\
               Failure in the allocation of ue control block\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */
   /* ADD CRID:ccpu00118776 Initilize Ue Timers*/
   cmInitTimers(&(ueCb->ueConTmr), VE_MAX_TMR_PER_UE);
/* MSPD changes */
#ifdef MSPD
   printf("RRC Connection Request received\n");
#endif
   ueCb->cellId = datIndSdu->hdr.cellId;
   ueCb->crnti = datIndSdu->sdu.m.ulCcchMsg.tmpCrnti;
   ueCb->rrcConnState = VE_RRC_CONNECTING;
/* RRC REEst */
   ueCb->reEstCause = INVALID_CAUSE;
   ueCb->s1MsgType = 0xFF;
   /* NAS NON Delivery Indication start */
   cmLListInit(&(ueCb->vePduLLst));
   ueCb->crntTransIdModifier = 0;
   /* NAS NON Delivery Indication end */

   /* Initialize srb related info */
#ifdef VE_SRB2_SUPPORT
   if(veInitUeSrbInfo(&ueCb->rbInfo, VE_PDCP_ID_SRB1) != ROK)
#else
   if(veInitUeSrbInfo(&ueCb->rbInfo) != ROK)
#endif
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConReqSetup: veInitUeSrbInfo\
               failed.\n"));
      RETVALUE(RFAILED);
   }


   cellCb = veCb.cellCb[0];
   ueIdx = VE_GET_UE_IDX(ueCb->crnti);

   if(ueIdx < cellCb->maxUeSupp)
   {
#ifdef LTE_HO_SUPPORT
      /* Initializing the Candidate Cell List and timers*/
      cmLListInit(&(ueCb->candCellList));
      cmInitTimers(&(ueCb->s1RelocTmr), 1);
      cmInitTimers(&(ueCb->s1OvrallTmr), 1);
      cmInitTimers(&(ueCb->ueHoAttachTmr), 1);
#endif
      cellCb->ueCbLst[ueIdx] = ueCb; 
      cellCb->numUeCfg++;
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConReqSetup:\
               UE supported maxxed out(%d)\n", ueIdx));
      /* Trigger RRC Connection Release Signaling */
      veSndRrcConRel(ueCb);
      /* Free control block */
      VE_FREE(ueCb, sizeof(VeUeCb));
      RETVALUE(ROK);
   }

   ueID = &(datIndSdu->sdu.m.ulCcchMsg.ccchMsg.message.val.c1.\
         val.rrcConRqst.criticalExtns.val.rrcConRqst_r8.ue_Identity);

   /* extract contention resolution id */
   if(ueID->choice.val == INITUE_IDENTITY_S_TMSI)
   {
      ueCb->sTMSI.pres = TRUE;
      ueCb->sTMSI.mmec = ueID->val.s_TMSI.mmec.val[0];
      ueCb->sTMSI.mTMSI = 0;
      for(cnt = 0; cnt < ueID->val.s_TMSI.m_TMSI.len/8; cnt++)
      {
         ueCb->sTMSI.mTMSI <<= 8;
         ueCb->sTMSI.mTMSI |= (U8)(ueID->val.s_TMSI.m_TMSI.val[cnt]);
      } /* end of for loop */
      
      VE_MEM_COPY(&conResId[1], &ueID->val.s_TMSI.mmec.val[0],\
            ueID->val.s_TMSI.mmec.len/8); /* 8 bits size*/
      VE_MEM_COPY(&conResId[2], &ueID->val.s_TMSI.m_TMSI.val[0],\
            ueID->val.s_TMSI.m_TMSI.len/8); /* 32 bits size*/
   }
   else
   {
      /* 40 bits size*/
      VE_MEM_COPY(&conResId[1], &ueID->val.randomValue.val[0],\
            ueID->val.randomValue.len/8);
   }

   /*Update the Establishment Cause*/
   ueCb->establishCause = (datIndSdu->sdu.m.ulCcchMsg.ccchMsg.message.val.c1.\
            val.rrcConRqst.criticalExtns.val.rrcConRqst_r8.establishmentCause.val);

   /*MME Overload Check */
   if (ueCb->sTMSI.pres == TRUE)
   {
     /* Function  to get MMECb*/
       veUtlSelMmeCbByMmec(ueCb->sTMSI.mmec, &mmeCb);
       if(mmeCb == NULLP)
       {
       #ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veLIMRrcConSetCmpHndlr:\
               Could not locate MmeCb\n"));
       #endif
       RETVALUE(RFAILED);
       }
      if(mmeCb->mmeOvldFlag == TRUE)
      {
        mmeStatus = (U8)veGetMmeAcceptStatus(ueCb->establishCause, mmeCb);
        if (mmeStatus == TRUE)
        {
          /*Trigger RRC Connection Reject Message */
          ret = veSndRrcConRej(ueCb);
          if (ret != ROK)
          {
            #ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConRej: veSndRrcConRej Failed \n"));
            #endif
            /* Free control block */
            VE_FREE(ueCb, sizeof(VeUeCb));
          }
          RETVALUE(ROK);
        }
     }
   }

   /* Allocate memory for event structure */
   VE_ALLOCEVNT(&nhuDatRspEvntSdu, sizeof(NhuDatRspSdus));

   /* Fill HDR Info */
   veFillNhuEvntHdr(&(nhuDatRspEvntSdu->hdr), ueCb->cellId, datIndSdu->hdr.ueId, C3_RRCCONSETUP);

   nhuDatRspEvntSdu->sdu.m.dlCcchMsg.rbId = VE_CCCH_DL_ID;
   nhuDatRspEvntSdu->sdu.m.dlCcchMsg.tmpCrnti = ueCb->crnti;

#ifndef CNE_UE_SIM_TEST
   veSndCtfUeCfg(ueCb);
#endif
   /* Build RRC Con Setup Message */
   ret = veFillRRC_ConSetup(nhuDatRspEvntSdu, ueCb);
   if (ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConReqSetup: veFillRRC_ConSetup Failed \n"));
#endif
      VE_FREEEVNT(nhuDatRspEvntSdu);
      RETVALUE(RFAILED);
   }

   /* Fill Config values */
   nhuDatRspEvntSdu->sdu.isUeCfgPres = TRUE;
   /*- Fill the UE configuration information --*/
   ueCfg = &(nhuDatRspEvntSdu->sdu.ueCfg);
   ret = veFillNhuUeCfgInfo(nhuDatRspEvntSdu, ueCfg, ueCb, C3_RRCCONSETUP);
   if (ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConReqSetup: veFillNhuUeCfgInfo Failed \n"));
#endif
      VE_FREEEVNT(nhuDatRspEvntSdu);
      RETVALUE(RFAILED);
   }

   /* UE Cfg - CTF */

   /* If Connection was initiated because of paging 
      delete the entry of the Paging Pending for this UE 
   */
   CM_LLIST_FIRST_NODE(&(cellCb->pendPagUesLst), tmpUePagingInfo);
 
   /* Supporting only STMSI Based Paging, delete the entry from the list
      using S-TMSI Key only */
   while( tmpUePagingInfo != NULLP)
   {

      if( (((VeUePagingInfo *)tmpUePagingInfo)->sTMSI.pres == TRUE) &&
          (((VeUePagingInfo *)tmpUePagingInfo)->sTMSI.mTMSI) == ueCb->sTMSI.mTMSI)
      {
         cmLListDelFrm(&(cellCb->pendPagUesLst),
                     (tmpUePagingInfo));
         VE_FREE(tmpUePagingInfo->node, sizeof(VeUePagingInfo));
      } /* end of if statement */
      CM_LLIST_NEXT_NODE(&(cellCb->pendPagUesLst), tmpUePagingInfo);
   } /* end of while loop */
  
      /* ADD CRID:ccpu00118776 Starting of  UE CON Procedure timer*/
#ifndef LTE_ENB_PERF
      veUeConStartTmr ((PTR)ueCb, VE_TMR_RRC_CON,smCfgCb.rrcConTmrVal);
#endif
   /* Send RRC Connection Setup through NhuDatRsp Primitive*/
   if (RFAILED == veHdlNhuDatRsp(nhuDatRspEvntSdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veSndRrcConReqSetup: veHdlNhuDatRsp failed.\n"));
      VE_FREEEVNT(datIndSdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndRrcConReqSetup */

/*
 *
 *      Fun:   veSndRrcRecfgReq
 *
 *      Desc:  RRC Reconfig Req
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndRrcRecfgReq
(
VeUeCb        *ueCb,
U8            s1Event
)
#else
PUBLIC S16 veSndRrcRecfgReq(ueCb, s1Event)
VeUeCb        *ueCb;
U8            s1Event;
#endif
{
   S16 ret = RFAILED;
   NhuDatReqSdus *nhuDatReqEvntSdu = NULLP;
   U8 idx;


   TRC2(veSndRrcRecfgReq);
#ifdef VE_PICO
   /* Enabling Call Scenario Prints ccpu00117125*/
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "ENB -> UE : RRC ReConfiguration Request");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
#endif   
   /* Allocate memory for event structure */
   VE_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));

#ifdef VE_SRB2_SUPPORT
   if(s1Event == Sztid_InitCntxtSetup)
   {
      /* Initialize srb2 related info */
      if(veInitUeSrbInfo(&ueCb->rbInfo, VE_PDCP_ID_SRB2) != ROK)
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConReqSetup: veInitUeSrbInfo\
                  failed.\n"));
         RETVALUE(RFAILED);
      }
   }
#endif
   /* Fill HDR Info */
   veFillNhuEvntHdr(&(nhuDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_RRCCONRECFGN);

   /* Build RRC Con ReConfig Message */
   ret = veFillNhuRrcConReCfgn(ueCb, nhuDatReqEvntSdu, s1Event);
   if (ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"vesndrrcrecfgreq: vefillnhurrcconrecfgn failed \n"));
#endif
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   /* Send RRC Connection Setup through NhuDatRsp Primitive*/
   if (RFAILED == veSndUENhuDatReq(nhuDatReqEvntSdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veSndRrcRecfgReq: veSndUENhuDatReq failed.\n"));
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }
#ifdef VE_SRB2_SUPPORT
   if (s1Event != Sztid_E_RABMdfy)
   {
      veSndRgrLchCfg(ueCb, &ueCb->rbInfo.srb[1], FALSE);
   }
#endif
   /* Trigger RGR LCH Cfg for DRBs */
   for (idx = 0; idx < ueCb->rbInfo.numOfRbCfgd && idx < VE_MAX_RABS_IN_UE; idx++)
   {
      /* ADD CRID:ccpu00117391 ERAB_RELEASE feature*/    
      if((ueCb->rbInfo.rab[idx].inUse == FALSE))
      {
         /*Sending the RGR LCH delete Request*/
         veSndRgrDelLchCfg(ueCb,&ueCb->rbInfo.rab[idx]);
         ueCb->rbInfo.numOfRbCfgd--;
      }
      else
      {
         /* TODO  E-RAB Modify  */
         if (s1Event != Sztid_E_RABMdfy)
         {
            if(ROK !=  veSndRgrLchCfg(ueCb, &ueCb->rbInfo.rab[idx], FALSE))
            {
               VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrLchCfg Recfg is failed.\n"));
               RETVALUE(RFAILED);
            }
         }
         else
         {
            if(ueCb->rbInfo.rab[idx].rabMdfyInfo.rabInfoModified)
            {
               if(ROK != veSndRgrLchCfg(ueCb, &ueCb->rbInfo.rab[idx], TRUE))
               {
                  VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrLchCfg Recfg is failed.\n"));
                  RETVALUE(RFAILED);
               }
            }
         }
      }
   }

   RETVALUE(ROK);
} /* veSndRrcRecfgReq */

/*
 *
 *      Fun:   veSndRrcDlInfoTfr
 *
 *      Desc:  RRC DL Info Tfr
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
/* NAS NON Delivery Indication start */
#ifdef ANSI
PUBLIC S16 veSndRrcDlInfoTfr
(
VeUeCb     *ueCb,
TknStrOSXL *nasPdu
)
#else
PUBLIC S16 veSndRrcDlInfoTfr(ueCb, nasPdu)
VeUeCb       *ueCb;
TknStrOSXL  *nasPdu;
#endif
{
   /* NAS NON Delivery Indication end */
   S16           ret = RFAILED;
   NhuDatReqSdus *nhuDatReqEvntSdu = NULLP;

   TRC2(veSndRrcDlInfoTfr);
#ifdef VE_PICO
   /* Call Scenario Prints ccpu00117125*/
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "ENB -> UE : RRC Downlink Direct Transfer");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");

#endif   

   /* Initial structs to zero */
   VE_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));

   /* Fill HDR Info */
   veFillNhuEvntHdr(&(nhuDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_DLINFOTFR);

   /* NAS NON Delivery Indication start */
   VE_ALLOC_TRANS_ID((nhuDatReqEvntSdu->hdr), ueCb->crnti, C4_DLINFOTFR, ueCb->crntTransIdModifier);
   ret  = veInsrtNasPduToLst(ueCb, nasPdu, nhuDatReqEvntSdu->hdr.transId);
   /* NAS NON Delivery Indication end */

   nhuDatReqEvntSdu->sdu.isUeCfgPres = FALSE;
   nhuDatReqEvntSdu->sdu.ueMsg.isMsgDed = TRUE;
/* RRC-ReEstab */
   if(ueCb->rrcConnState != VE_SRB2_DRB_SUSPENDED)
   {
      if(ueCb->rrcConnState != VE_SRB2_ESTABLISHED)
      {
         ret = veFillRRC_DlInfTr(nhuDatReqEvntSdu, nasPdu->len, nasPdu->val, VE_PDCP_ID_SRB1);
      }
      else
      {
         ret = veFillRRC_DlInfTr(nhuDatReqEvntSdu, nasPdu->len, nasPdu->val, VE_PDCP_ID_SRB2);
      }   

      /* Issue the primitive */
      if(ROK != veSndUENhuDatReq(nhuDatReqEvntSdu))
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcDlInfoTfr : VeLiNhuDatReq failed.\n"));
         VE_FREEEVNT(nhuDatReqEvntSdu);
         RETVALUE(RFAILED);
      }
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcDlInfoTfr : SRB2 and DRBs are suspended.\
               S1AP NAS NON DELIVERY INDICATION will be Triggered\n"));

      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

      RETVALUE(ROK);
} /* veSndRrcDlInfoTfr */

/*
 *
 *      Fun:   veSndRrcConRel
 *
 *      Desc:  RRC Con Rel
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndRrcConRel
(
VeUeCb        *ueCb
)
#else
PUBLIC S16 veSndRrcConRel(ueCb)
VeUeCb        *ueCb;
#endif
{
   S16 ret = RFAILED;
   NhuDatReqSdus *nhuDatReqEvntSdu = NULLP;

   TRC2(veSndRrcConRel);
#ifdef VE_PICO
   /* Enabling Call Scenario Prints ccpu00117125*/
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "ENB -> UE : RRC Connection Release");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
#endif   
   /* Allocate memory for event structure */
   VE_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
   /* CR ID- ccpu00116764-IOT changes */
   /* Update RRC state to Connetion Releasing */
   ueCb->rrcConnState = VE_RRC_RELEASING;
   /* Fill HDR Info */
   veFillNhuEvntHdr(&(nhuDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_RRCCONRLS);

   /* Build RRC Con Release Message */
   ret = veFillRRC_ConRls(nhuDatReqEvntSdu);
   if (ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConRel: veFillRRC_ConRls Failed \n"));
#endif
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   /* Fill Config values */
   nhuDatReqEvntSdu->sdu.isUeCfgPres = FALSE;

   /* Send RRC Connection Setup through NhuDatRsp Primitive*/
   if (RFAILED == veSndUENhuDatReq(nhuDatReqEvntSdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veSndRrcConRel: veSndUENhuDatReq failed.\n"));
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndRrcConRel */
/*
 *
 *      Fun:   veSndRrcConRej
 *
 *      Desc:  RRC Con Reject Message
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndRrcConRej
(
VeUeCb        *ueCb
)
#else
PUBLIC S16 veSndRrcConRej(ueCb)
VeUeCb        *ueCb;
#endif
{
   S16 ret = RFAILED;
   NhuDatRspSdus *nhuDatRspEvntSdu = NULLP;
   U32 waitTime = 10;

   TRC2(veSndRrcConRej);
#ifdef VE_PICO
   /* Enabling Call Scenario Prints ccpu00117125*/
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "ENB -> UE : RRC Connection Reject");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
#endif
   /* Allocate memory for event structure */
   VE_ALLOCEVNT(&nhuDatRspEvntSdu, sizeof(NhuDatRspSdus));
   /* CR ID- ccpu00116764-IOT changes */
   /* Update RRC state to Connetion Releasing */
   ueCb->rrcConnState = VE_RRC_RELEASING;
   /* Fill HDR Info */
   veFillNhuEvntHdr(&(nhuDatRspEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C3_RRCCONREJ);
   nhuDatRspEvntSdu->sdu.m.dlCcchMsg.rbId = VE_CCCH_DL_ID;
   nhuDatRspEvntSdu->sdu.m.dlCcchMsg.tmpCrnti = ueCb->crnti;
   /* Build RRC Con Reject Message */
   ret = veFillRRC_ConRej(nhuDatRspEvntSdu, waitTime);
   if (ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConRej: veFillRRC_ConRej Failed \n"));
#endif
      VE_FREEEVNT(nhuDatRspEvntSdu);
      RETVALUE(RFAILED);
   }

   /* Send RRC Connection Reject through NhuDatRsp Primitive*/
   if (RFAILED == veHdlNhuDatRsp(nhuDatRspEvntSdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veSndRrcConRej: veSndUENhuDatRsp failed.\n"));
      VE_FREEEVNT(nhuDatRspEvntSdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndRrcConRej */

 

/*
 *
 *      Fun:   veSndRrcSecModeCmd
 *
 *      Desc:  RRC Security Mode Command
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndRrcSecModeCmd
(
VeUeCb        *ueCb,
S1apPdu       *s1apPdu
)
#else
PUBLIC S16 veSndRrcSecModeCmd(ueCb, s1apPdu)
VeUeCb        *ueCb;
S1apPdu       *s1apPdu;
#endif
{
   S16 ret = RFAILED;
   NhuUeCfgInfo  *ueCfg = NULLP;

   NhuDatReqSdus *nhuDatReqEvntSdu = NULLP;

   TRC2(veSndRrcSecModeCmd);
#ifdef VE_PICO
   /* Enabling Call Scenario Prints ccpu00117125*/
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "ENB -> UE : Security Mode Command");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");

#endif   
   /* Allocate memory for event structure */
   VE_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));

   /* Fill HDR Info */
   veFillNhuEvntHdr(&(nhuDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_SECURITYMODECMD);

   /* Build RRC Con Setup Message */
   ret = veFillRRC_SecModeCmd(nhuDatReqEvntSdu, ueCb);
   if (ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcSecModeCmd: veFillRRC_SecModeCmd Failed \n"));
#endif
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   /* Fill Config values */
   nhuDatReqEvntSdu->sdu.isUeCfgPres = TRUE;
   /*- Fill the UE configuration information --*/
   ueCfg = &(nhuDatReqEvntSdu->sdu.ueCfg);
   if(RFAILED == veFillNhuUeCfgInfo(nhuDatReqEvntSdu, ueCfg, ueCb, C4_SECURITYMODECMD))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veSndRrcSecModeCmd: veSndUENhuDatReq failed.\n"));
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   /* Send RRC Connection Setup through NhuDatRsp Primitive*/
   if (RFAILED == veSndUENhuDatReq(nhuDatReqEvntSdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veSndRrcSecModeCmd: veSndUENhuDatReq failed.\n"));
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndRrcSecModeCmd */
/* Ue Capability related functions */
#ifdef UE_RAD_CAP
/*
 *
 *      Fun:   veSndRrcUeCapEnq
 *
 *      Desc:  RRC UE Capability Enquiry msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndRrcUeCapEnq
(
VeUeCb        *ueCb,
S1apPdu       *s1apPdu
)
#else
PUBLIC S16 veSndRrcUeCapEnq(ueCb, s1apPdu)
VeUeCb        *ueCb;
S1apPdu       *s1apPdu;
#endif
{
   S16 ret = RFAILED;

   NhuDatReqSdus *nhuDatReqEvntSdu = NULLP;

   TRC2(veSndRrcUeCapEnq);
#ifdef VE_PICO   
   /* Enabling Call Scenario Prints ccpu00117125*/
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "ENB -> UE : UE CAPABILITY ENQ Message");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
#endif
   /* Allocate memory for event structure */
   VE_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));

   /* Fill HDR Info */
   veFillNhuEvntHdr(&(nhuDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_UECAPENQUIRY);

   /* Build RRC Con Setup Message */
   ret = veFillRRC_UeCapEnq(nhuDatReqEvntSdu, ueCb);
   if (ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcUeCapEnq: veFillRRC_UeCapEnq Failed \n"));
#endif
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   /* Fill Config values */
   nhuDatReqEvntSdu->sdu.isUeCfgPres = FALSE;

   /* Send UE Capability Enq through NhuDatRsp Primitive*/
   if (RFAILED == veSndUENhuDatReq(nhuDatReqEvntSdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veSndRrcUeCapEnq: veSndUENhuDatReq failed.\n"));
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndRrcUeCapEnq */
/*
 *
 *      Fun:   veSndRrcUeCapDecReq
 *
 *      Desc:  RRC UE Capability Info Dec msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndRrcUeCapDecReq
(
VeUeCb        *ueCb,
TknStrOSXL    *str,
U16           sduType    /*Type of the IE or messaage that needs to be decoded */
)
#else
PUBLIC S16 veSndRrcUeCapDecReq(ueCb, str, sduType)
VeUeCb        *ueCb;
TknStrOSXL    *str;
U16           sduType; 
#endif
{
   S16 ret = RFAILED;
   NhuDecodeSduReq *nhuDecSduReq = NULLP;

   TRC2(veSndRrcUeCapDecReq);
     
   if(!(str->pres) && (str->len))
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                            " veSndRrcUeCapDecReq: Invalid Buffer - \n"));
   }
   /* Allocate memory for event structure */
   VE_ALLOCEVNT(&nhuDecSduReq, sizeof(NhuDecodeSduReq));

   /* Fill HDR Info */
   veFillNhuEvntHdr(&(nhuDecSduReq->hdr), ueCb->cellId, ueCb->crnti, C4_UECAPENQUIRY);
   nhuDecSduReq->sduType = sduType;

   /* Convert the U8 array  to Msg Buffer */
   ret = SGetMsg(0,0, &nhuDecSduReq->mbuf);
   if (ROK != ret)
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                            "Failed to allocate mBuf - %d\n", ret));
      RETVALUE(RFAILED);
   }
   /* Add the received flat buffer to to mBuf */
   ret = SAddPstMsgMult((Data *)str->val, (MsgLen)str->len, nhuDecSduReq->mbuf);
   if (ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcUeCapDecReq: SAddPstMsgMult Failed \n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Send Decode UE Capability Req Primitive*/
   if (RFAILED == veSndNhuDecodeReq(nhuDecSduReq))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veSndRrcUeCapDecReq: veSndNhuDecodeReq failed.\n"));
      VE_FREEEVNT(nhuDecSduReq);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndRrcUeCapDecReq */

/*
 *
 *      Fun:   veHdlNhuUeRaCapDecCfm
 *
 *      Desc:  RRC UE Capability Info Dec msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veHdlNhuUeRaCapDecCfm
(
VeUeCb        *ueCb,
NhuDecodeSduCfm *nhuDecSduCfm
)
#else
PUBLIC S16 veHdlNhuUeRaCapDecCfm(ueCb, nhuDecSduCfm)
VeUeCb        *ueCb;
NhuDecodeSduCfm *nhuDecSduCfm;
#endif
{
   S16 ret = RFAILED;
   NhuUERadioAccessCapInfo_r8_IEsue_RadioAccessCapInfo *ueRaCap;

   TRC2(veHdlNhuUeRaCapDecCfm);
     
   if((nhuDecSduCfm->status != NHU_DECODE_SUCCESS))
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                            " veHdlNhuUeRaCapDecCfm: Invalid Buffer - \n"));
      RETVALUE(RFAILED);
   }
   ueRaCap = &nhuDecSduCfm->ie.ueRadioCapInfo.criticalExtns.val.c1.val.\
              ueRadioAccessCapInfo_r8.ue_RadioAccessCapInfo;
 
   ret = veSndRrcUeCapDecReq(ueCb, ueRaCap, NHU_MSG_UECAPINFO);
   VE_FREE(ueRaCap->val, ueRaCap->len);

   RETVALUE(ret);
} /* veHdlNhuUeRaCapDecCfm */
/*
 *
 *      Fun:   veHdlNhuUeCapDecCfm
 *
 *      Desc:  RRC UE Capability Info Dec msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veHdlNhuUeCapDecCfm
(
VeUeCb        *ueCb,
NhuDecodeSduCfm *nhuDecSduCfm
)
#else
PUBLIC S16 veHdlNhuUeCapDecCfm(ueCb, nhuDecSduCfm)
VeUeCb        *ueCb;
NhuDecodeSduCfm *nhuDecSduCfm;
#endif
{
   S16 cNt;
   S16 ret = RFAILED;
   NhuUE_CapRAT_ContLst *ueContLst = NULLP;

   TRC2(veHdlNhuUeCapDecCfm);
     
   if((nhuDecSduCfm->status != NHU_DECODE_SUCCESS))
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                            " veHdlNhuUeCapDecCfm: Decode Failed - \n"));
      RETVALUE(RFAILED);
   }
  if (nhuDecSduCfm->ie.ueCapInfo.criticalExtns.val.c1.val.ueCapInfo_r8.pres.pres
                                       == PRSNT_NODEF)
   {
      ueContLst = &nhuDecSduCfm->ie.ueCapInfo.criticalExtns.val.c1.val.\
                               ueCapInfo_r8.ue_CapRAT_ContLst;
      for(cNt=0; cNt < ueContLst->noComp.val; cNt++)
      { 
         if((ueContLst->member[cNt].pres.pres == TRUE) &&
             (ueContLst->member[cNt].rat_Typ.val == NhuRAT_TypeutraEnum))
         {  
                /* Decode EUTRA UeCap Ie */
             ret = veSndRrcUeCapDecReq(ueCb, &ueContLst->member[cNt].ueCapRAT_Cont,
                                          NHU_DEC_SDUTYPE_EUTRA_UECAP_IE);
              
         }   
      }
      VE_FREE(ueContLst->member, 
         (sizeof(NhuUE_CapRAT_Cont))*(ueContLst->noComp.val));
   }
   RETVALUE(ret);
} /* veHdlNhuUeCapDecCfm */
/*
 *
 *      Fun:   veHdlNhuEutraUeCapIeDecCfm
 *
 *      Desc:  RRC UE Capability Info Dec msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veHdlNhuEutraUeCapIeDecCfm
(
VeUeCb        *ueCb,
NhuDecodeSduCfm *nhuDecSduCfm
)
#else
PUBLIC S16 veHdlNhuEutraUeCapIeDecCfm(ueCb, nhuDecSduCfm)
VeUeCb        *ueCb;
NhuDecodeSduCfm *nhuDecSduCfm;
#endif
{
   S16 ret = RFAILED;

   TRC2(veHdlNhuEutraUeCapIeDecCfm);

   if((nhuDecSduCfm->status != NHU_DECODE_SUCCESS))
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                            " veHdlNhuEutraUeCapIeDecCfm: Decode Failed - \n"));
      RETVALUE(RFAILED);
   }
   /* Fill the Ue Cap Str and keep in DB */
   cmMemcpy((U8*)&ueCb->ueCap.eUTRA_Cap, 
       (U8*)&nhuDecSduCfm->ie.nhuUE_EUTRA_Cap, sizeof(NhuUE_EUTRA_Cap));  
   ret = veSndRrcSecModeCmd(ueCb, ueCb->pdu);     

   RETVALUE(ret);
} /* veHdlNhuEutraUeCapIeDecCfm */
/*
 *
 *      Fun:   veHdlNhuUeCapEncCfm
 *
 *      Desc:  RRC UE Capability Info Dec msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veHdlNhuUeCapEncCfm
(
VeUeCb        *ueCb,
NhuEncCfmSdus *nhuEncCfmSdus
)
#else
PUBLIC S16 veHdlNhuUeCapEncCfm(ueCb, nhuEncCfmSdus)
VeUeCb        *ueCb;
NhuEncCfmSdus *nhuEncCfmSdus;
#endif
{
   
   NhuEncReqSdus *encSdu;
   S16 ret = RFAILED;
   MsgLen len = 0;
   MsgLen cnt = 0;
   TknStrOSXL  str;
   Data *octStr;
   TRC2(veHdlNhuUeCapEncCfm);
  
   if(nhuEncCfmSdus->sdu.status != NHU_ENCODE_SUCCESS)
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                  "veHdlNhuUeCapEncCfm: NHU Encode failed \n"));
      RETVALUE(RFAILED);
   }
   /* Find the length of the received IP packet */
   ret = SFndLenMsg(nhuEncCfmSdus->sdu.mBuf, &len);
   
   if((len == 0) || (ret != ROK))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                     "veHdlNhuUeCapEncCfm: In correct buffer len %d",len));
      RETVALUE(RFAILED);
   }   
       
   VE_ALLOC(&octStr, len);
   /* Convert the mBuf to U8 array */
   SCpyMsgFix(nhuEncCfmSdus->sdu.mBuf, 0, len, octStr, &cnt);
   VE_FREEMBUF(nhuEncCfmSdus->sdu.mBuf);

   VE_FILL_TKN_STR_UINT((&str),octStr,len)
   
   /* Allocate memory for event structure */
   VE_ALLOCEVNT(&encSdu, sizeof(NhuEncReqSdus));
   /* Fill HDR Info */
   veFillNhuEvntHdr(&(encSdu->hdr), ueCb->cellId, ueCb->crnti, C4_UECAPENQUIRY);
   encSdu->sdu.msgCategory = NHU_MSG_UERACAPINFO; 

   veFillRRC_UeRadAccessCapInfo(&str,encSdu, ueCb);

    /* Send Encode UE RA Capability Info Req Primitive*/
   if (RFAILED == veSndNhuEncodeReq(encSdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veHdlNhuUeCapEncCfm: veSndNhuEncodeReq failed.\n"));
      VE_FREEEVNT(encSdu);
      RETVALUE(RFAILED);
   }
   
   RETVALUE(ROK);

} /* veHdlNhuUeCapEncCfm */

/*
 *
 *      Fun:   veHdlNhuUeRaCapEncCfm
 *
 *      Desc:  RRC UE Capability Info Dec msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veHdlNhuUeRaCapEncCfm
(
VeUeCb        *ueCb,
NhuEncCfmSdus *nhuEncCfmSdus
)
#else
PUBLIC S16 veHdlNhuUeRaCapEncCfm(ueCb, nhuEncCfmSdus)
VeUeCb        *ueCb;
NhuEncCfmSdus *nhuEncCfmSdus;
#endif
{
   
   S16 ret = RFAILED;
   MsgLen len = 0;
   MsgLen cnt = 0;
   TknStrOSXL  str;
   VeS1ConCb *s1apCon = NULLP;
   S1apPdu      *s1Pdu = NULLP;
   Data *octStr;
   SztDatEvntReq datEvt;
   VeS1ConCb *s1apConCb = NULLP;
   s1apConCb = ueCb->s1ConCb;

   TRC2(veHdlNhuUeRaCapEncCfm);
  
   if(nhuEncCfmSdus->sdu.status != NHU_ENCODE_SUCCESS)
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                  "veHdlNhuUeRaCapEncCfm: NHU Encode failed \n"));
      RETVALUE(RFAILED);
   }
   /* Find the length of the received IP packet */
   ret = SFndLenMsg(nhuEncCfmSdus->sdu.mBuf, &len);
   
   if((len == 0) || (ret != ROK))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                     "veHdlNhuUeRaCapEncCfm: In correct buffer len %d",len));
      RETVALUE(RFAILED);
   }   
       
   VE_ALLOC(&octStr, len);
   /* Convert the mBuf to U8 array */
   SCpyMsgFix(nhuEncCfmSdus->sdu.mBuf, 0, len, octStr, &cnt);
   VE_FREEMBUF(nhuEncCfmSdus->sdu.mBuf);

   VE_FILL_TKN_STR_UINT((&str),octStr,len)
   s1apCon = ueCb->s1ConCb;
   veUtlSztFillUECapbltyInfoInd(s1apCon, &str, &s1Pdu); 
    

   VE_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));
   /* populate the event structure */
   datEvt.spConnId = s1apConCb->spConnId;
      datEvt.pdu = (S1apPdu *)s1Pdu;
      VeLiSztDatReq(&veCb.sztSap[0]->pst,veCb.sztSap[0]->spId , &datEvt);

       if(ueCb->ueCap.ueEutraOctStr.pres)
       {
         ret = veSndRrcUeCapDecReq(ueCb, &ueCb->ueCap.ueEutraOctStr,
                                   NHU_DEC_SDUTYPE_EUTRA_UECAP_IE);
       }
   
   RETVALUE(ROK);

} /* veHdlNhuUeRaCapEncCfm */
#endif

#ifdef LTE_HO_SUPPORT
/*
 *
 *      Fun:   veHdlNhuHoPrepEncCfm
 *
 *      Desc:  RRC UE Capability Info Dec msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veHdlNhuHoPrepEncCfm
(
VeUeCb        *ueCb,
NhuEncCfmSdus *nhuEncCfmSdus
)
#else
PUBLIC S16 veHdlNhuHoPrepEncCfm(ueCb, nhuEncCfmSdus)
VeUeCb        *ueCb;
NhuEncCfmSdus *nhuEncCfmSdus;
#endif
{
   
   S16 retVal = RFAILED;
   CmLList *cmLstEnt = NULLP;
   VeCandCellDb *candCellNode = NULLP;
   MsgLen len = 0;

   TRC2(veHdlNhuHoPrepEncCfm);
  
   if(nhuEncCfmSdus->sdu.status != NHU_ENCODE_SUCCESS)
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                  "veHdlNhuHoPrepEncCfm: NHU Encode failed \n"));
      RETVALUE(RFAILED);
   }
   /* Find the length of the received IP packet */
   retVal = SFndLenMsg(nhuEncCfmSdus->sdu.mBuf, &len);
   
   if((len == 0) || (retVal != ROK))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                     "veHdlNhuHoPrepEncCfm: In correct buffer len %d",len));
      RETVALUE(RFAILED);
   }   
   
   /* 
    * Start of Comment.
    * Travers through the candidate list in ueCb and intiate HO based 
    * on S1 or X2 appropriately.
    * Since X2 and S1 based Handover procedure carry same container,
    * we are storing encoded buffer into UeCb.
    * End of comment.
    */
   if(NULLP ==  ueCb->trnspContainer)
   {
      ueCb->trnspContainer = nhuEncCfmSdus;
   }
   
   cmLstEnt = ueCb->candCellList.first;
   if (NULLP != cmLstEnt)
   {
      candCellNode = (VeCandCellDb *)cmLstEnt->node;
      if(candCellNode->nghCellCb->isX2ConnAvl)
      {
         /* create a x2ConCb and update the candidate node */
         VE_ALLOC(&candCellNode->x2ConCb, sizeof(VeX2ConCb));
         if(candCellNode->x2ConCb == NULLP)
         {
            VE_DBG_ERROR((VE_PRNT_BUF,"veHdlNhuHoPrepEncCfm: alloc for x2ConCb failed.\n"));
         }
         else
         {
            candCellNode->x2ConCb->oldEnbUeX2apId = candCellNode->nghCellCb->x2apEnbUeIdCntr++;
            candCellNode->x2ConCb->ueCb = ueCb;
            /* assign the peerId */
            candCellNode->x2ConCb->peerId = candCellNode->nghCellCb->peerId;

            /* Add x2ConCb in veCb.x2ConLst */
            veCb.x2apConLst[VE_GET_UE_X2_IDX(candCellNode->x2ConCb->oldEnbUeX2apId)] = candCellNode->x2ConCb;

            veFillTknU8(&candCellNode->x2ConCb->cause.choice, CAUSE_RADIONW);
            veFillTknU32(&candCellNode->x2ConCb->cause.val.radioNw,\
                  CztCauseRadioNwhandover_desirable_for_radio_reasonsEnum);

            if(RFAILED == (retVal = veX2SrcFSM[ueCb->mobCtrlState][VE_EVNT_HO_DECISION](candCellNode->x2ConCb, (PTR *)candCellNode->nghCellCb)))
            {
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"veHdlNhuHoPrepEncCfm: X2 FSM Failure for EVNT - VE_EVNT_HO_DECISION\n"));
#endif
            }
         }
      }
      else
      {
         veFillTknU8(&ueCb->s1ConCb->cause.choice, CAUSE_RADIONW);
         veFillTknU32(&ueCb->s1ConCb->cause.val.radioNw, \
               SztCauseRadioNwhandover_desirable_for_radio_reasonEnum);

         if(RFAILED == (retVal = veS1SrcFSM[ueCb->mobCtrlState][VE_EVNT_HO_DECISION](ueCb->s1ConCb, (PTR *)candCellNode->nghCellCb)))
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"VeLiNhuEncodeCfm: S1 FSM Failure for EVNT - VE_EVNT_HO_DECISION\n"));
#endif
         }
      }
   }/* End of if... */
   ueCb->mobCtrlState = VE_HO_INITIATED;
   ueCb->hoType = VE_HO_TYPE_NONE;
   
   RETVALUE(ROK);

} /* veHdlNhuHoPrepEncCfm */

/*
 *
 *      Fun:   veHdlNhuHoCmdEncCfm
 *
 *      Desc:  RRC UE Capability Info Dec msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veHdlNhuHoCmdEncCfm
(
VeUeCb        *ueCb,
NhuEncCfmSdus *nhuEncCfmSdus
)
#else
PUBLIC S16 veHdlNhuHoCmdEncCfm(ueCb, nhuEncCfmSdus)
VeUeCb        *ueCb;
NhuEncCfmSdus *nhuEncCfmSdus;
#endif
{
   U8      ueIdx;
   S16 ret = RFAILED;
   MsgLen len = 0;

   TRC2(veHdlNhuHoCmdEncCfm);
  
   if(nhuEncCfmSdus->sdu.status != NHU_ENCODE_SUCCESS)
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                  "veHdlNhuHoCmdEncCfm: NHU Encode failed \n"));
      RETVALUE(RFAILED);
   }
   /* Find the length of the received IP packet */
   ret = SFndLenMsg(nhuEncCfmSdus->sdu.mBuf, &len);
   
   if((len == 0) || (ret != ROK))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                     "veHdlNhuHoCmdEncCfm: In correct buffer len %d",len));
      RETVALUE(RFAILED);
   }

   /* check if HO Cancel was received */
   if(ueCb->lowLyrCfgSts == VE_LOW_LYR_CFG_IN_PRGS)
   {

      if(ueCb->hoType == VE_HO_TYPE_X2)
         veX2TgtFSM[ueCb->mobCtrlState][VE_EVNT_HO_PREP_SUCCESS](ueCb->x2ConCb, (PTR *)nhuEncCfmSdus);
      else
         veS1TgtFSM[ueCb->mobCtrlState][VE_EVNT_HO_PREP_SUCCESS](ueCb->s1ConCb, (PTR *)nhuEncCfmSdus);

      /* Start UE HO Attach Timer */
      veStartTmr((PTR)ueCb, VE_TMR_UE_HO_ATTACH_TMR, veCb.ueHoAttachTimerVal);


      /* Requst EGTP for the creation of normal and forwarding tunnels */
      if(RFAILED == veHoCfgTunls(ueCb, VE_EGTP_TUN_CREATE))
      {
         VE_DBG_ERROR((VE_PRNT_BUF, "veHdlNhuHoCmdEncCfm:\
                  Configuration of eGTP tunnels Failed.\n"));
      }

      ueCb->mobCtrlState = VE_HO_IN_PROGRESS;
      ueCb->lowLyrCfgSts = VE_LOW_LYR_CFG_COMPL;
   }
   else if(ueCb->lowLyrCfgSts == VE_LOW_LYR_CFG_REM)
   {
      /* release UE resources */
      veSndCtfUeRls(ueCb);
      veSndRgrUeRls(ueCb, NULLP,RGR_UE_CFG);/* RRC ReEst */
      VeLiNhuCnclUeReq(&(veCb.nhuSap[0]->pst), veCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);

      /* release x2 resources */
      veSndX2UeLocRlsReq(ueCb->x2ConCb);
      veCb.x2apConLst[VE_GET_UE_X2_IDX(ueCb->x2ConCb->oldEnbUeX2apId)] = NULLP;

      /* Send EGTP release command to EGTP-U*/
      veSndEgtpCom(VE_EGTP_TUN_DEL, ueCb->s1ConCb);

      /* release s1 resources */
      veSndS1UeLocRlsReq(ueCb->s1ConCb);
      veCb.s1apConLst[ueCb->s1ConCb->suConnId] = NULLP;
      VE_FREE(ueCb->s1ConCb,sizeof(VeS1ConCb));

      /* remove ueCb from cellCb */
      ueIdx = VE_GET_UE_IDX(ueCb->crnti);
      if(ueIdx < veCb.cellCb[0]->maxUeSupp)
      {
         veCb.cellCb[ueIdx] = NULLP;
      }
      VE_FREE(ueCb,sizeof(VeUeCb));
   }

   RETVALUE(ROK);

} /* veHdlNhuHoCmdEncCfm */

/*
 *
 *      Fun:   veHdlNhuDlDcchEncCfm
 *
 *      Desc:  RRC UE Capability Info Dec msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veHdlNhuDlDcchEncCfm
(
VeUeCb        *ueCb,
NhuEncCfmSdus *nhuEncCfmSdus
)
#else
PUBLIC S16 veHdlNhuDlDcchEncCfm(ueCb, nhuEncCfmSdus)
VeUeCb        *ueCb;
NhuEncCfmSdus *nhuEncCfmSdus;
#endif
{
   
   S16 retVal = RFAILED;
   MsgLen len = 0;
   NhuEncReqSdus    *hoCmdEncReq= NULLP;

   TRC2(veHdlNhuDlDcchEncCfm);
  
   if(nhuEncCfmSdus->sdu.status != NHU_ENCODE_SUCCESS)
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                  "veHdlNhuDlDcchEncCfm: NHU Encode failed \n"));
      RETVALUE(RFAILED);
   }
   /* Find the length of the received IP packet */
   retVal = SFndLenMsg(nhuEncCfmSdus->sdu.mBuf, &len);
   
   if((len == 0) || (retVal != ROK))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
                     "veHdlNhuDlDcchEncCfm: In correct buffer len %d",len));
      RETVALUE(RFAILED);
   }
   /* take action based on transId */
   switch(nhuEncCfmSdus->hdr.transId)
   {
      case VE_ENC_TRANSID_HO_DL_DCCH_MSG:
         /* trigger NhuEncodeReq to encode Tgt to Src Transparent Container */
         VE_ALLOC(&hoCmdEncReq, sizeof(NhuEncReqSdus));
         hoCmdEncReq->hdr.cellId = ueCb->cellId;
         hoCmdEncReq->hdr.ueId = ueCb->crnti; 
         hoCmdEncReq->hdr.transId = VE_ENC_TRANSID_HO_CMD;

         /* fill the NhuHovrPrepInfo info inside hoPrepEncReq */
         if(RFAILED == (retVal = veUtlFillHoCmdTrnspCntr(ueCb, hoCmdEncReq, nhuEncCfmSdus)))
         {
            VE_DBG_ERROR((VE_PRNT_BUF,"\nveLiSztHoRqstHandler: \
                     Filling of HO Command Tranparent Container Failed.\n"));
            VE_FREE(hoCmdEncReq, sizeof(NhuEncReqSdus));
         }
         else
         {
            VeLiNhuEncodeReq(&veCb.nhuSap[0]->pst, veCb.nhuSap[0]->suId, hoCmdEncReq);                  
         }  
         break;

      default:
         /* Shouldnt be here */
         break;
   }

   RETVALUE(ROK);

} /* veHdlNhuDlDcchEncCfm */


/*
 *
 *      Fun:   veHdlNhuHoCmdDecCfm
 *
 *      Desc:  RRC UE Capability Info Dec msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veHdlNhuHoCmdDecCfm
(
VeUeCb        *ueCb,
NhuDecodeSduCfm *nhuDecodeSduCfm
)
#else
PUBLIC S16 veHdlNhuHoCmdDecCfm(ueCb, nhuDecodeSduCfm)
VeUeCb        *ueCb;
NhuDecodeSduCfm *nhuDecodeSduCfm;
#endif
{
   S16 ret = ROK;
   NhuHovrCmd    *hovrCmd = NULLP;
   NhuHovrCmd_r8_IEshandoverCmdMsg *hoCmdMsg = NULLP;  /* It is TknStrOSXL */
   Buffer                  *mBuf = NULLP;
   NhuDatReqSdus           *nhuDatReqSdu = NULLP;
   Pst                     *pst = NULLP;          

   TRC2(veHdlNhuHoCmdDecCfm);
     
   if((nhuDecodeSduCfm->status != NHU_DECODE_SUCCESS))
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                            " veHdlNhuHoCmdDecCfm: Decode Failed - \n"));
      RETVALUE(RFAILED);
   }
   hovrCmd = &nhuDecodeSduCfm->ie.hovrCmd;
   if(hovrCmd->pres.pres)
   {
      hoCmdMsg = &hovrCmd->criticalExtns.val.c1.val.handoverCmd_r8.handoverCmdMsg;

      /* convert the OSXL to Buffer */
      VE_ALLOCMBUF(veCb.mem.region, veCb.mem.pool,&mBuf ); 

      SAddPstMsgMult((Data *)hoCmdMsg->val,
         (MsgLen)hoCmdMsg->len, (Buffer *)mBuf);
      /* Send the RRC Connection Reconfiguration Msg
       * which is already in the encoded format to RRC so that it 
       * sends out to the UE.
       */
      VE_ALLOCEVNT(&nhuDatReqSdu, sizeof(NhuDatReqSdus));
      veFillNhuRrcConReCfgnWithMbuf( ueCb, nhuDatReqSdu, mBuf);
      pst = &veCb.nhuSap[0]->pst;
      pst->event   = EVTNHUDATREQ;

      if(ROK != VeLiNhuDatReq(pst, veCb.nhuSap[0]->spId, nhuDatReqSdu))
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veHdlNhuHoCmdDecCfm: VeLiNhuDatReq failed.\n"));
         RETVALUE(RFAILED);
      }
      else
      {
         printf("\n*****************Sending RRC ReCfg With MOB Info****************\n");
      }
      /* Requst EGTP for the creation of forwarding tunnels */
      if(RFAILED == veHoCfgTunls(ueCb, VE_EGTP_FWD_TUN_CREATE))
      {
         VE_DBG_ERROR((VE_PRNT_BUF, "veHdlNhuHoCmdDecCfm:\
                  Configuration of eGTP Forwarding Tunnels Failed.\n"));
         RETVALUE(RFAILED);
      }
      ueCb->mobCtrlState = VE_HO_IN_PROGRESS;
      ueCb->isLwrLyrRestNeeded = TRUE;       
   } /* end of if statement */
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
                            " veHdlNhuHoCmdDecCfm:  HoCmd not available- \n"));
      RETVALUE(RFAILED);
   }
   RETVALUE(ret);
} /* veHdlNhuHoCmdDecCfm */
#endif

/*
 *
 *      Fun:   veLIMRrcConReEst
 *
 *      Desc:  RRC Connection Reestablishment
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndRrcConReEst
(
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 veSndRrcConReEst(datIndSdu)
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
   VeCellCb  *cellCb = NULLP; 
   VeUeCb    *ueCb = NULLP;
   S1apPdu   *pdu = NULLP;
   U16       ueIdx = 0;
   U16       ret = 0;
   /* RRC-ReEstab */
   U8 rbCnt = 0;
   U8 idx = 0;
   VeUeRAB *rbInfo = NULLP;
   NhuUL_CCCH_Msg *NhuULccchMsg = NULL;
   U16       ueId = 0;
   U8     *ueIdBitStr = NULL;
   U8     tmpbyte = 0;
   Pst pst;
/* RRC-ReEstab */
   U8     failCause;
   Bool   rrcReEstFlag = TRUE;

   TRC2(veSndRrcConReEst);
#ifndef LTE_ENB_PERF 
#ifdef VE_PICO
   /* Enabling Call Scenario Prints ccpu00117125*/
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "ENB -> UE : RRC Connection ReEstablishment");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
#endif   
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Recived RRC Connection \
            ReEstablishment Request from UeId:%x\n", datIndSdu->hdr.ueId));
#endif

   cellCb = veCb.cellCb[0];
   NhuULccchMsg = &datIndSdu->sdu.m.ulCcchMsg.ccchMsg;
   ueIdBitStr = (NhuULccchMsg->message.val.c1.val.rrcConReestbRqst.criticalExtns.val.rrcConReestbRqst_r8.\
                     ue_Identity.c_RNTI.val);
   tmpbyte = ueIdBitStr[0];
   ueIdBitStr[0] = ueIdBitStr[1];
   ueIdBitStr[1] = tmpbyte;
   ueId = *(U16 *)ueIdBitStr;
   
   ueIdx = VE_GET_UE_IDX(ueId);
  /*****************************************************************************/ /* RRC Shishir Change*/
   
   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
                  /*sending rrc reestablishment reject incase of ueCb is NULLP*/
      if(ueCb == NULLP)
      {
         VE_DBG_ERROR((VE_PRNT_BUF,
                  "\nveSndRrcConReEst: ueCb is null"));
         ret = veSndReestReply(datIndSdu,cellCb,NULLP,FALSE,ueId);
         if (ret  != ROK)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConReEst: Sending Re-est replay Failed \n"));
#endif
            RETVALUE(RFAILED);
         }

      }

   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
       "\nveLIMRrcReconfigCmpHndlr(): Invalid ueIdx(%d)\n",ueIdx));
      RETVALUE(RFAILED);
   }

  /*****************************************************************************/
 

   /* RRC-ReEstab */
   failCause =(U8)datIndSdu->sdu.m.ulCcchMsg.ccchMsg.message.val.c1.val.rrcConReestbRqst.criticalExtns.val
                                                             .rrcConReestbRqst_r8.reestablishmentCause.val;
    ueCb->rrcConnState = VE_SRB2_DRB_SUSPENDED;
   /*If the reason for the Re-establishment is because of the Handover Failure
    * */
   /* RRC-ReEstab */
   if(failCause == NhuReestbCausehandoverFailEnum)
   {
      /* Received RRC Re-establishment request from UE because of handover
       * failure */

      /* Reset the state variables */
#ifdef LTE_HO_SUPPORT
      ueCb->mobCtrlState = VE_HO_NORMAL;
      ueCb->hoType = VE_HO_TYPE_NONE;
#endif
      /* Free x2ConCb */
      /* Stop Data Fwding */
      /* setting the value for pst */
      VE_SET_ZERO(&pst, sizeof(Pst));

      /*Currently hard coding the pst structure*/
      pst.dstEnt = ENTVE;
      pst.srcEnt = ENTVE;
      pst.dstProcId = SM_VE_PROC;
      pst.srcProcId = SM_VE_PROC;

      rbCnt = ueCb->rbInfo.numOfRbCfgd;
      rbInfo = ueCb->rbInfo.rab;
      for(idx = 0; idx < rbCnt; idx++)
      {
#ifdef LTE_HO_SUPPORT
         if(rbInfo[idx].isDataFwdAvl == TRUE)
         {
            switch(rbInfo[idx].rbDir)
            {
               case VE_DL:
               cmPkEuDelLocTunReq(&pst, 0, rbInfo[idx].datFwdDlLocEgtpTunId);
               /* free rem DL tunnel info */
               rbInfo[idx].remDlDataFwdTeId.pres = NOTPRSNT;
               VE_FREE(rbInfo[idx].remDlDataFwdTeId.val, rbInfo[idx].remDlDataFwdTeId.len);
               break;

               case VE_UL:
               cmPkEuDelLocTunReq(&pst, 0, rbInfo[idx].datFwdUlLocEgtpTunId);
               /* free rem UL tunnel info */
               rbInfo[idx].remUlDataFwdTeId.pres = NOTPRSNT;
               VE_FREE(rbInfo[idx].remUlDataFwdTeId.val, rbInfo[idx].remUlDataFwdTeId.len);
               break;

               case VE_DL_UL:
               default:
               cmPkEuDelLocTunReq(&pst, 0, rbInfo[idx].datFwdDlLocEgtpTunId);
               /* free rem DL tunnel info */
               rbInfo[idx].remDlDataFwdTeId.pres = NOTPRSNT;
               VE_FREE(rbInfo[idx].remDlDataFwdTeId.val, rbInfo[idx].remDlDataFwdTeId.len);

               cmPkEuDelLocTunReq(&pst, 0, rbInfo[idx].datFwdUlLocEgtpTunId);
               /* free rem UL tunnel info */
               rbInfo[idx].remUlDataFwdTeId.pres = NOTPRSNT;
               VE_FREE(rbInfo[idx].remUlDataFwdTeId.val, rbInfo[idx].remUlDataFwdTeId.len);
               break;
            }
         }
#endif
      }

/* RRC-ReEstab */
   } /* end of handover failure case */
   else if (failCause  == NhuReestbCausereconfigurationFailEnum)
   {
/*causeValue = re-configuration Failure */
     /* Currently assuming that reconfiguration message was rejected by the
     * UE; hence triggering initial context setup failure */
      switch(ueCb->s1MsgType)
      {
         case VE_INIT_CNTXT_SETUP_REQ:
             veUtlSztFillInitCntxtSetFail(ueCb->s1ConCb, &pdu,\
             SztCauseRadioNwfailure_in_radio_interface_procedureEnum);
              /* Send RRC REEST Reject to UE */
             rrcReEstFlag = FALSE;
         break;
         case  VE_ERAB_SETUP_REQ:
          ueCb->reEstCause = ERAB_REQ_FAIL_ANY_CAUSE;
               veUtlSztFillERABSetRsp(ueCb->s1ConCb,&pdu,FALSE);
         break;
         case  VE_ERAB_MODIFY_REQ:
            /* E-RAB Modify */
            veUtlSztFillERABMdfyRsp(ueCb->s1ConCb, &pdu, FALSE);

            /*Currently don't support this*/
            break;
         case  VE_ERAB_RLS_COMM:
               veUtlSztFillERABRelRsp(ueCb->s1ConCb, &pdu,FALSE);
               ueCb->reEstCause = ERAB_REL_RECON_FAIL;
          /* Send RRC REEST Reject to UE */
          rrcReEstFlag = FALSE;
         break;
         default:
#ifdef DEBUGP
                     VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConReEst Invalid s1ReqType %d\n",ueCb->s1MsgType));
#endif

      }
     /*Freeing pdu in ueCb block*/
     cmFreeMem((Ptr)(ueCb->pdu));

   }
   else
   {
    /* causeValue = otherFailure*/
      switch(ueCb->s1MsgType)
      {
         case VE_INIT_CNTXT_SETUP_REQ:

             if (ueCb->pdu->pdu.val.initiatingMsg.value.u.sztInitCntxtSetupRqst.\
                protocolIEs.member->value.u.sztCSFallbackIndicator.pres == TRUE &&
                 ueCb->pdu->pdu.val.initiatingMsg.value.u.sztInitCntxtSetupRqst.\
                 protocolIEs.member->value.u.sztCSFallbackIndicator.val == FALSE)
             {
                 veUtlSztFillInitCntxtSetFail(ueCb->s1ConCb, &pdu,\
                 SztCauseRadioNwfailure_in_radio_interface_procedureEnum);
                 /* Send RRC REEST Reject to UE */
       rrcReEstFlag = FALSE;
       /*Freeing pdu in ueCb block*/
       cmFreeMem((Ptr)(ueCb->pdu));
             }
             else
             {
                 ueCb->reEstCause = ICS_OTHER_FAIL_CSFALBK_SET;
             }

         break;
         case  VE_ERAB_SETUP_REQ:
               veUtlSztFillERABSetRsp(ueCb->s1ConCb,&pdu,FALSE);
          ueCb->reEstCause = ERAB_REQ_FAIL_ANY_CAUSE;
          /*Freeing pdu in ueCb block*/
          cmFreeMem((Ptr)(ueCb->pdu));
         break;
         case  VE_ERAB_RLS_COMM:
               ueCb->reEstCause = ERAB_REL_FAIL_ANY_CAUSE;
         break;
         default:
         #ifdef DEBUGP
                     VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConReEst Invalid s1ReqType %d\n",ueCb->s1MsgType));
         #endif
               ueCb->reEstCause = RLF_CAUSE;

      }

   } /* end of else part */
 
   /* Send Reply to Reestablishment request */
   ret = veSndReestReply(datIndSdu,cellCb,ueCb,rrcReEstFlag,ueId);
   if (ret  != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConReEst: Sending Re-est replay Failed \n"));
#endif
      RETVALUE(RFAILED);
   }
#endif/*LTE_ENB_PERF*/   
   
     RETVALUE(ROK);
   } /* veSndRrcConReEst */


 /* RRC-ReEstab */
 /*
 *
 *      Fun:   veSndReestReply
 *
 *      Desc:  Reest RRC Connection Reestablishment Replay
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PRIVATE S16 veSndReestReply
(
NhuDatIndSdus *datIndSdu, 
VeCellCb      *cellCb,
VeUeCb   *ueCb,
Bool sndAccpt,
U8    ueId
)
#else
PRIVATE S16 veSndReestReply(datIndSdu,cellCb,ueCb, sndAccpt,ueId)
NhuDatIndSdus *datIndSdu;
VeCellCb   *cellCb;
VeUeCb   *ueCb;
Bool sndAccpt;
U8    ueId;
#endif
{
    NhuDatRspSdus *nhuDatRspEvntSdu= NULLP;
    U8 ret;
    
    U8   ueNwIdx;
    U8   ueIdx;
    TRC2(veSndReestReply); 
       
   /*TODO:shishir:Filling Header of Reestablishment Response*/
   /* Allocate memory for event structure */
   VE_ALLOCEVNT(&nhuDatRspEvntSdu, sizeof(NhuDatRspSdus));
   /* Fill HDR Info */
   veFillNhuEvntHdr(&(nhuDatRspEvntSdu->hdr), cellCb->cellId, ueId, C3_RRCCONREESTB);

   nhuDatRspEvntSdu->sdu.m.dlCcchMsg.rbId = VE_CCCH_DL_ID;

   /*Fillling RRC Con Re-establishment Resp based on Accept or Reject*/   
   if(sndAccpt)
   {     
     /*Sending configuration  request to MAC scheduler for ue  re-cfg  and e-RAB re-cfg with new crnti*/

     /*shishir*/
/*     veSndRgrUeIdReCfg(ueCb,datIndSdu->sdu.m.ulCcchMsg.tmpCrnti); */
     /*Calculating the new idx of ueCb*/
     
     ueNwIdx = (U8)VE_GET_UE_IDX(datIndSdu->sdu.m.ulCcchMsg.tmpCrnti);
     ueIdx = (U8)VE_GET_UE_IDX(ueId);
     if(ueNwIdx < cellCb->maxUeSupp)
     {
         cellCb->ueCbLst[ueNwIdx] = ueCb;
         cellCb->ueCbLst[ueIdx] = NULLP;
     }
     else
     {
         VE_DBG_ERROR((VE_PRNT_BUF,
         "\nveLIMRrcReconfigCmpHndlr(): Invalid ueIdx(%d)\n",ueIdx));
         RETVALUE(RFAILED);
     }
      ueCb->crnti = datIndSdu->sdu.m.ulCcchMsg.tmpCrnti;
      nhuDatRspEvntSdu->sdu.m.dlCcchMsg.tmpCrnti = ueCb->crnti;
      /* Build RRC Con Re-establishment Message */
      ret = (U8)veFillRRC_ConReest(nhuDatRspEvntSdu, ueCb);
      if (ret != ROK)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veSndReestReply: veFillRRC_ConReest Failed \n"));
#endif
         VE_FREEEVNT(nhuDatRspEvntSdu);
         RETVALUE(RFAILED);
      } 
   /* Send RRC Connection Re-establishment Response  using NhuDatRsp Primitive*/
   if (RFAILED == veHdlNhuDatRsp(nhuDatRspEvntSdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veSndReestReply: veHdlNhuDatRsp failed.\n"));
      VE_FREEEVNT(datIndSdu);
      RETVALUE(RFAILED);
   } /* end of if statement */
     veSndRgrUeIdReCfg(ueCb,ueId);
     /* Configure CL with New RNTI */
     /* veSndCtfUeCfg(ueCb); */
     veSndCtfUeReCfg(ueCb,ueId);
   }
   else
   {
      nhuDatRspEvntSdu->sdu.m.dlCcchMsg.tmpCrnti = ueId;
      /*Build RRC Con Re-establihsment Reject Message*/
      ret = (U8)veFillRRC_ConReestRej(nhuDatRspEvntSdu, ueCb);
      if (ret != ROK)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veSndReestReply: veFillRRC_ConReestReject Failed \n"));
#endif
         VE_FREEEVNT(nhuDatRspEvntSdu);
         RETVALUE(RFAILED);
      }
   /* Send RRC Connection Re-establishment Response  using NhuDatRsp Primitive*/
   if (RFAILED == veHdlNhuDatRsp(nhuDatRspEvntSdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veSndReestReply: veHdlNhuDatRsp failed.\n"));
      VE_FREEEVNT(datIndSdu);
      RETVALUE(RFAILED);
   } /* end of if statement */
   }
  

   RETVALUE(ROK);
} /* veSndReestReply */
 
/* RRC-ReEstab  END*/

/* ==================================== */
/* RRC Uplink Message Handlers  */
/* ==================================== */
/*
 *
 *      Fun:   veLIMRrcConSetCmpHndlr
 *
 *      Desc:  RRC Connection setup Complete
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veLIMRrcConSetCmpHndlr
(
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 veLIMRrcConSetCmpHndlr(datIndSdu)
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
   VeCellCb *cellCb = NULLP; 
   VeUeCb *ueCb = NULLP;
#ifndef LTE_ENB_PERF
   VeS1ConCb *s1apConCb = NULLP;
   U32    suConId = 0;
#endif
   U16        ueIdx;

   U16        retVal = 0;

   SztConReq datEvt;
   U8              pres;
   U16             mmeId = 0;
   NhuRegisteredMME *regMME = NULLP;
   TRC2(veLIMRrcConSetCmpHndlr);
#ifdef VE_PICO
   /* Enabling Call Scenario Prints ccpu00117125*/
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "UE -> ENB : RRC Connection Setup Complete Message");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");

#endif   
   VE_SET_ZERO(&datEvt, sizeof(SztConReq));


   /* Get Cell Cb */
   cellCb = veCb.cellCb[0];
   ueIdx = VE_GET_UE_IDX(datIndSdu->hdr.ueId);
/* MSPD changes */
#ifdef MSPD
   printf("RRC ConnectionSetUp Complete received\n");
#endif
   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "\nveLIMRrcConSetCmpHndlr(): Invalid ueIdx(%d)\n",ueIdx));
      RETVALUE(RFAILED);
   }
   
   if(ueCb->rrcConnState == VE_RRC_CONNECTED)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"\n\n ENODEB APP: recived multiple RRC Conn Setup Complete Message!!!\n\n"));
      RETVALUE(RFAILED);
    } 
   ueCb->rrcConnState = VE_RRC_CONNECTED;
      /* ADD CRID:ccpu00118776 Stopping of  CON Procedure Timer*/
      veUeConStopTmr ((PTR)ueCb,VE_TMR_RRC_CON);
/* Check if GUMMEI is present in the message*/

   pres = datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.\
          rrcConSetupCompl.criticalExtns.val.c1.val.rrcConSetupCompl_r8.\
          registeredMME.pres.val;
   if(pres)
   {
      regMME = &datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.\
               rrcConSetupCompl.criticalExtns.val.c1.val.rrcConSetupCompl_r8.\
               registeredMME;
      ueCb->gummei.pres = TRUE;
      ueCb->gummei.plmnId.mcc[0] = (U8)regMME->plmn_Identity.
              mcc.member[0].val;
      ueCb->gummei.plmnId.mcc[1] = (U8)regMME->plmn_Identity.
              mcc.member[1].val;
      ueCb->gummei.plmnId.mcc[2] = (U8)regMME->plmn_Identity.
              mcc.member[2].val;
      ueCb->gummei.plmnId.mnc[0] = (U8)regMME->plmn_Identity.
              mnc.member[0].val;
      ueCb->gummei.plmnId.mnc[1] = (U8)regMME->plmn_Identity.
              mnc.member[1].val;
      ueCb->gummei.plmnId.numMncDigits
         = (U8)regMME->plmn_Identity.mnc.noComp.val;
      if(ueCb->gummei.plmnId.numMncDigits > 2)
      {
         ueCb->gummei.plmnId.mnc[2] = (U8)regMME->plmn_Identity.
            mnc.member[2].val;
      }
      VE_MEM_COPY(&ueCb->gummei.mmec, &regMME->mmec.val,\
            regMME->mmec.len/8);
      VE_MEM_COPY(&ueCb->gummei.mmeGrpId, &regMME->mmegi.val,\
            regMME->mmegi.len/8);

   }
   else
   {
      ueCb->selectedPLMNIdentity = (U32)&datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.\
                        rrcConSetupCompl.criticalExtns.val.c1.val.rrcConSetupCompl_r8.\
                        selectedPLMN_Identity;
   }
   retVal =  veMmeSelect(ueCb, &mmeId);
   if (retVal != ROK)
   {
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Connection \
            Complete failed. UeId:%x\n", datIndSdu->hdr.ueId));
#endif
   RETVALUE (retVal);
   }
     

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Connection \
            Complete Rcvd. UeId:%x\n", datIndSdu->hdr.ueId));
#endif
#ifndef LTE_ENB_PERF
   VE_ALLOC(&s1apConCb, sizeof(VeS1ConCb));
   /* klock warning fixed */
   if(s1apConCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLIMRrcConSetCmpHndlr:\
               Failure in the allocation of s1ap control block\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */


   VE_GET_S1AP_CON_ID(suConId);

   if (suConId >= VE_MAX_S1AP_CON)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLIMRrcConSetCmpHndlr:\
               Max S1AP Connection Setup \n"));
#endif
      /* MAX UE reached */
      RETVALUE(RFAILED); 
   }
   /* Initialize control blocks */
   veCb.s1apConLst[suConId] = s1apConCb;
   s1apConCb->suConnId = suConId;
   s1apConCb->s1apConnState = VE_S1AP_CONNECTING;

   s1apConCb->ueCb = ueCb;
   /* TODO: Need to change this logic as we need to validate the same */
   VE_GET_MME_CB(mmeId, s1apConCb->mmeCb);
   if(s1apConCb->mmeCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLIMRrcConSetCmpHndlr:\
               Could not locate MmeCb\n"));
#endif
      RETVALUE(RFAILED);
   }

   ueCb->s1ConCb = s1apConCb;

   s1apConCb->peerId = s1apConCb->mmeCb->peerId;
   /* Build Init Ue Msg */
   retVal = veUtlSztFillInitUeMsg(ueCb, datIndSdu,&datEvt.pdu);
   
   if(ROK != retVal)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLIMRrcConSetCmpHndlr : Failed to Fill veUtlSztFillInitUeMsg \n"));
#endif
      RETVALUE(RFAILED);
   }
   datEvt.peerId.val = mmeId;
   datEvt.peerId.pres = TRUE;
   datEvt.u.suConnId = suConId;
#ifndef LTE_ENB_PERF
      /* ADD CRID:ccpu00118776  Starting of RRC CON Procedure timer*/
      veUeConStartTmr ((PTR)ueCb, VE_TMR_INITUE_CON,VE_INITTMR_VAL);/*TBD for exact value*/
#endif
   /* Trigger SZT Connectn. Req */
   VeLiSztConReq(&veCb.sztSap[0]->pst, veCb.sztSap[0]->spId, &datEvt);
#endif
   RETVALUE(ROK);
} /* veLIMRrcConSetCmpHndlr */

/*
 *
 *      Fun:   veMmeSelect
 *
 *      Desc:  MME Selection based on MME Selection Algorithm
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veMmeSelect
(
VeUeCb        *ueCb,
U16           *mmeId
)
#else
PUBLIC S16 veMmeSelect(ueCb, mmeId)
VeUeCb        *ueCb;
U16           *mmeId;
#endif
{
  Bool mmeStatus =  FALSE;
  VeMmeCb   *mmeCb = NULLP;
  TRC2(veMmeSelect);
  
  if (ueCb->gummei.pres)
  {
    veUtlSelMmeCbByMmec(ueCb->gummei.mmec, &mmeCb);
    if (mmeCb == NULLP)
    { 
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veMmeSelect:\
               Could not locate MmeCb\n"));
#endif
       RETVALUE(RFAILED);
    }
    
  }
  else if (!(ueCb->gummei.pres) && (ueCb->sTMSI.pres))
  {
    veUtlSelMmeCbByMmec(ueCb->sTMSI.mmec, &mmeCb);
    if (mmeCb == NULLP)
    { 
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veMmeSelect:\
               Could not locate MmeCb\n"));
#endif
       RETVALUE(RFAILED);
    }
  }
  else
  {
    /*Apply MME Load balancing Algorithm */
    if (ueCb->selectedPLMNIdentity)
    {
       veGetMmeByRelCap(ueCb, &mmeCb);
       if (mmeCb == NULLP)
       { 
       #ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veMmeSelect:\
               Could not locate MmeCb\n"));
       #endif
         RETVALUE(RFAILED);
       }
       else
       {
         *mmeId = mmeCb->mmeId;
         RETVALUE (ROK);
       }
     }
  }
  if (mmeCb->mmeOvldFlag)
  {
    /*Accept the call based on the Establsihment Cause */
     mmeStatus = (U8) veGetMmeAcceptStatus(ueCb->establishCause, mmeCb);
     if (mmeStatus == TRUE)
     {
        /*Trigger RRC Connection Release Procedure*/
        veSndRrcConRel(ueCb);
        RETVALUE (RFAILED);
     }
  }
  *mmeId = mmeCb->mmeId;
  RETVALUE(ROK);
}
/*
 *
 *      Fun:   veGetMmeByRelCap
 *
 *      Desc:  MME Selection based on MME relative Capacity
 *
 *      Ret:   MME ID
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veGetMmeByRelCap
(
VeUeCb        *ueCb,
VeMmeCb       **mmeCb
)
#else
PUBLIC S16 veGetMmeByRelCap(ueCb, mmeCb)
VeUeCb        *ueCb;
VeMmeCb       **mmeCb;
#endif
{
   VeMmeCb *tmpMmeCb = NULLP;
   U8 idx = 0;
   U16 relCapValue = 0;
   TRC2(veGetMmeByRelCap);
   for (idx = 0; idx < veCb.mmeCont.numMmes; idx++)
   {
     tmpMmeCb = veCb.mmeCont.mmes[idx];
     if ((tmpMmeCb != NULLP) && (tmpMmeCb->relCapacity >= relCapValue))
     {
       relCapValue = tmpMmeCb->relCapacity;
       *mmeCb = tmpMmeCb;
     }
   }
   RETVALUE(ROK);
}
/*
 *
 *      Fun:   veGetMmeAcceptStatus
 *
 *      Desc:  Function compares RRC Connection Establishment cause with MME Overload A
ction
 *
 *      Ret:   Returns MME Accept Status
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veGetMmeAcceptStatus
(
 U32       establishmentCause,
 VeMmeCb   *mmeCb
)
#else
PUBLIC S16 veGetMmeAcceptStatus(establishmentCause, mmeCb)
U32        establishmentCause;
VeMmeCb   *mmeCb; 
#endif
{
  Bool mmeRejectStatus =  0;
  TRC2(veGetMmeAcceptStatus);

  switch(establishmentCause)
  {
    case NhuEstablishmentCauseemergencyEnum:
    {
#ifndef S1AP_REL8A0
      if (mmeCb->mmeOvldAction !=  SztOverloadActionpermit_emergency_sessions_and_mobile_terminated_services_onlyEnum)
        mmeRejectStatus = TRUE;
#else
      if(mmeCb->mmeOvldAction != SztOverloadActionpermit_emergency_sessions_onlyEnum)
          mmeRejectStatus = TRUE;
#endif /* S1AP_REL8A0 */

        break;
    }
    case NhuEstablishmentCausehighPriorityAccessEnum:
    {
       mmeRejectStatus = TRUE;
       break;
    }
    case NhuEstablishmentCausemt_AccessEnum:
    {
#ifndef S1AP_REL8A0
      if (mmeCb->mmeOvldAction !=  SztOverloadActionpermit_emergency_sessions_and_mobile_terminated_services_onlyEnum)
        mmeRejectStatus = TRUE;
#else
      if(mmeCb->mmeOvldAction != SztOverloadActionpermit_emergency_sessions_onlyEnum)
          mmeRejectStatus = TRUE;
#endif /* S1AP_REL8A0 */

        break;
    }
case NhuEstablishmentCausemo_SignallingEnum:
    {
      if (mmeCb->mmeOvldAction !=  SztOverloadActionreject_all_rrc_cr_signallingEnum)
        mmeRejectStatus = TRUE;
        break;
    }
    case NhuEstablishmentCausemo_DataEnum:
    {
#ifndef S1AP_REL8A0
      if (mmeCb->mmeOvldAction ==  SztOverloadActionpermit_emergency_sessions_and_mobile_terminated_services_onlyEnum)
      mmeRejectStatus = TRUE;
#else
      if(mmeCb->mmeOvldAction != SztOverloadActionpermit_emergency_sessions_onlyEnum)
          mmeRejectStatus = TRUE;
#endif /* S1AP_REL8A0 */
      break;
    }
  }
  RETVALUE(mmeRejectStatus);
}
/*
 *
 *      Fun:   veLIMRrcUlInfoTfrHndlr
 *
 *      Desc:  RRC UL Info transfer
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veLIMRrcUlInfoTfrHndlr
(
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 veLIMRrcUlInfoTfrHndlr(datIndSdu)
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{

   VeCellCb *cellCb = NULLP; 
   VeUeCb *ueCb = NULLP;
#ifndef LTE_ENB_PERF
   SztDatEvntReq datEvt;
   VeS1ConCb *s1apConCb = NULLP;
#endif
   U16        ueIdx;
  
   U16        retVal = 0;

   TRC2(veLIMRrcUlInfoTfrHndlr);
#ifdef YS_PICO
   /* Enabling Call Scenario Prints ccpu00117125*/
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "UE -> ENB : Uplink Direct transfer received ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");

   SEND_DBG_MSG("%s %d\n", __FILE__, __LINE__);
#endif

  
   cellCb = veCb.cellCb[0];
   ueIdx = VE_GET_UE_IDX(datIndSdu->hdr.ueId);
   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "\nnveLIMRrcUlInfoTfrHndlr(): Invalid ueIdx(%d)\n",ueIdx));
      RETVALUE(RFAILED);
   }
#ifdef YS_PICO
   SEND_DBG_MSG("%s %d\n", __FILE__, __LINE__);
#endif
#ifndef LTE_ENB_PERF
   /* Construct S1AP Message */
   s1apConCb = ueCb->s1ConCb; 
   if( s1apConCb == NULLP)
      RETVALUE(RFAILED);
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "UL Info Transfer Message\
            Rcvd. from UeId:%x\n", datIndSdu->hdr.ueId));
#endif

   VE_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));

   /* Construct S1AP Message */
   retVal =  veUtlSztFillUlNasTrans(s1apConCb, datIndSdu, &datEvt.pdu);
   
   if(ROK != retVal)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,
               "\nnveLIMRrcUlInfoTfrHndlr(): Failed to Fill ULNasTrans.\n"));
      RETVALUE(RFAILED);
#endif      
   }
   datEvt.spConnId = s1apConCb->spConnId;
#ifdef YS_PICO
   SEND_DBG_MSG("%s %d\n", __FILE__, __LINE__);
#endif
   retVal = VeLiSztDatReq(&veCb.sztSap[0]->pst,veCb.sztSap[0]->spId , &datEvt);

   if(ROK != retVal)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,
               "\nnveLIMRrcUlInfoTfrHndlr(): Failed to Send ULNasTrans.\n"));
      RETVALUE(RFAILED);
#endif    
   }

#ifdef YS_PICO
   SEND_DBG_MSG("%s %d\n", __FILE__, __LINE__);
#endif
#endif
   RETVALUE(ROK);
} /* veLIMRrcUlInfoTfrHndlr */

/*
 *
 *      Fun:   veLIMRrcSecModCmpHndlr
 *
 *      Desc:  Security Mode complete
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veLIMRrcSecModCmpHndlr
(
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 veLIMRrcSecModCmpHndlr(datIndSdu)
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
   VeCellCb *cellCb = NULLP; 
   VeUeCb   *ueCb = NULLP;
   U16      ueIdx;

   TRC2(veLIMRrcSecModCmpHndlr);
#ifdef VE_PICO
   /* Call Scenario Prints ccpu00117125*/
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "UE -> ENB : RRC Security Mode Complete ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
#endif   
   cellCb = veCb.cellCb[0];
   ueIdx = VE_GET_UE_IDX(datIndSdu->hdr.ueId);
   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "\nveLIMRrcSecModCmpHndlr(): Invalid ueIdx(%d)\n",ueIdx));
      RETVALUE(RFAILED);
   }

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Security Mode Complete Rcvd.\
            from UeId:%x\n", datIndSdu->hdr.ueId));
#endif

#ifdef LTE_HO_SUPPORT
   /* setting handover state to initial state */
   ueCb->mobCtrlState = VE_HO_NORMAL;
#endif

   if(ueCb->pdu != NULLP)
   {
      /* Trigger RRC DL Connection Reconfiguration message*/
      veSndRrcRecfgReq(ueCb, Sztid_InitCntxtSetup);
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "\nveLIMRrcSecModCmpHndlr(): S1AP doesnot exist for ueIdx (%d)\n",ueIdx));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /*veLIMRrcSecModCmpHndlr */
/*
 *
 *      Fun:   veLIMRrcSecModFailHndlr
 *
 *      Desc:  RRC Reconfiguration complete
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veLIMRrcSecModFailHndlr
(
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 veLIMRrcSecModFailHndlr(datIndSdu)
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
#ifndef LTE_ENB_PERF
   VeCellCb *cellCb = NULLP; 
   VeUeCb *ueCb = NULLP;
   SztRelRsp      relEvnt;
   U8   ueIdx =0;
   U16  retVal = 0;
#endif

   TRC2(veLIMRrcSecModFailHndlr);
#ifdef VE_PICO
   /* Enabling Call Scenario Prints ccpu00117125*/
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "UE -> ENB : RRC Security Mode Failure");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");

#endif   
   /* Leaving empty for now */
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Security Mode Failure Rcvd.\
            from UeId:%x\n", datIndSdu->hdr.ueId));
#endif

#ifndef LTE_ENB_PERF
   /* Get Cell Cb */
   cellCb = veCb.cellCb[0];
   ueIdx = (U8)VE_GET_UE_IDX(datIndSdu->hdr.ueId);/* klock warning fix */
   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "\nveLIMRrcConSetCmpHndlr(): Invalid ueIdx(%d)\n",ueIdx));
      RETVALUE(RFAILED);
   }

   /* Fill and Send RRC Connection Release*/
   veSndRrcConRel(ueCb);

   /*Send S1AP Ue_Context_relase_compelete message*/
   retVal =   veUtlSztUeCntxtRelComp(ueCb->s1ConCb,&(relEvnt.pdu));
   if(ROK != retVal)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"nveLIMRrcConSetCmpHndlr: Failed to Fill veUtlSztUeCntxtRelComp \n"));
#endif
      RETVALUE(RFAILED);
   }
   VeLiSztRelRsp(&veCb.sztSap[0]->pst, veCb.sztSap[0]->spId,&relEvnt);

   /*Delete ue,s1ap control blocks*/
   veCb.s1apConLst[ueCb->s1ConCb->suConnId] = NULLP;
   veCb.cellCb[ueCb->crnti] = NULLP;
   
   VE_FREE(ueCb->s1ConCb,sizeof(VeS1ConCb));
   VE_FREE(ueCb,sizeof(VeUeCb));
#endif
   
   RETVALUE(ROK);
} /*veLIMRrcSecModFailHndlr */
/* Ue Capability related functions */
#ifdef UE_RAD_CAP
/*
 *
 *      Fun:   veLIMRrcUeCapInfoHndlr
 *
 *      Desc:  Ue Capability Info
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veLIMRrcUeCapInfoHndlr
(
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 veLIMRrcUeCapInfoHndlr(datIndSdu)
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
   VeCellCb *cellCb = NULLP; 
   VeUeCb *ueCb = NULLP;
   U8   ueIdx =0,cNt;
   NhuUECapInfo *ueCapInfo;
   NhuUE_CapRAT_ContLst *ueContLst = NULLP;
   NhuEncReqSdus *encSdu;
   TRC2(veLIMRrcUeCapInfoHndlr);

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Ue Cap Info Rcvd.\
            from UeId:%x\n", datIndSdu->hdr.ueId));
#endif
#ifdef VE_PICO   
  /* Enabling Call Scenario Prints ccpu00117125*/ 
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "UE -> ENB : UE CAP INFO Message");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
#endif

   cellCb = veCb.cellCb[0];
   ueIdx = VE_GET_UE_IDX(datIndSdu->hdr.ueId);
   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
   }

   /* Allocate memory for event structure */
   VE_ALLOCEVNT(&encSdu, sizeof(NhuEncReqSdus));

   /* Fill HDR Info */
   veFillNhuEvntHdr(&(encSdu->hdr), ueCb->cellId, ueCb->crnti, C4_UECAPENQUIRY);
   encSdu->sdu.msgCategory = NHU_MSG_UECAPINFO; 

   ueCapInfo = &datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.ueCapInfo;
 
    ueContLst = &ueCapInfo->criticalExtns.val.c1.val.ueCapInfo_r8.ue_CapRAT_ContLst;
    for(cNt=0; cNt < ueContLst->noComp.val; cNt++)
    {
       if((ueContLst->member[cNt].pres.pres == TRUE) &&
           (ueContLst->member[cNt].rat_Typ.val == NhuRAT_TypeutraEnum))
       {  
           /* Store EUTRA UeCap Ie Octet */
          if(!ueCb->ueCap.ueEutraOctStr.pres)
          {
             VE_ALLOC(&ueCb->ueCap.ueEutraOctStr.val, 
                           ueContLst->member[cNt].ueCapRAT_Cont.len);
          }
          cmMemcpy(ueCb->ueCap.ueEutraOctStr.val,
                   ueContLst->member[cNt].ueCapRAT_Cont.val,
                   ueContLst->member[cNt].ueCapRAT_Cont.len);
          ueCb->ueCap.ueEutraOctStr.pres = TRUE;
          ueCb->ueCap.ueEutraOctStr.len = ueContLst->member[cNt].\
                                ueCapRAT_Cont.len;
       }
     } 

   cmMemcpy((U8*) &encSdu->sdu.u.ueCapInfo,(U8*)&datIndSdu->sdu.m.\
                 ulDcchMsg.dcchMsg.message.val.c1.val.ueCapInfo,
                    sizeof(NhuUECapInfo));

    /* Send Encode UE RA Capability Info Req Primitive*/
   if (RFAILED == veSndNhuEncodeReq(encSdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veLIMRrcUeCapInfoHndlr: veSndNhuEncodeReq failed.\n"));
      VE_FREEEVNT(encSdu);
      RETVALUE(RFAILED);
   }
   
   RETVALUE(ROK);
} /*veLIMRrcUeCapInfoHndlr */

#endif
#ifdef LTE_HO_SUPPORT
/*
 *
 *      Fun:   veLIMRrcMeasRprtHndlr
 *
 *      Desc:  Measurement Report
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veLIMRrcMeasRprtHndlr
(
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 veLIMRrcMeasRprtHndlr(datIndSdu)
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
   S16 retVal = RFAILED;
   U16 ueIdx = 0;
   VeCellCb *cellCb = NULLP; 
   VeUeCb *ueCb = NULLP;
   Bool hoNeeded = FALSE;
   NhuMeasurementReport *measReport = NULLP;
   NhuMeasurementReport_r8_IEs *measReportIe = NULLP;
   NhuMeasResults *measResults = NULLP;
   NhuEncReqSdus *hoPrepEncSdu = NULLP;
   static S16 measurementRprtCount = 0;

   TRC2(veLIMRrcMeasRprtHndlr);

/* To discard the duplicate measurement reports*/
   measurementRprtCount++;
   if (1 == measurementRprtCount)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veLIMRrcMeasRprtHndlr: \
               measurement report rcvd.\n"));
   }   
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veLIMRrcMeasRprtHndlr: \
         measurement report discarded.\n"));
      RETVALUE(ROK); 
   }
   cellCb = veCb.cellCb[0];
   ueIdx = VE_GET_UE_IDX(datIndSdu->hdr.ueId);

   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "\nveLIMRrcMeasRprtHndlr(): Invalid ueIdx(%d)\n",ueIdx));
      RETVALUE(RFAILED);
   }
   
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Measurement Report Rcvd\
            from UeId:%x\n", datIndSdu->hdr.ueId));
#endif
   measReport = 
       &datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.measurementReport;
   if (FALSE == measReport->pres.pres )
   {
       VE_DBG_ERROR((VE_PRNT_BUF,"\nveLIMRrcMeasRprtHndlr \
                       measurement report not Present.\n"));
       RETVALUE(RFAILED);
   }
   measReportIe = &measReport->criticalExtns.val.c1.val.measurementReport_r8;
   if (FALSE == measReportIe->pres.pres )
   {
       VE_DBG_ERROR((VE_PRNT_BUF,"\nveLIMRrcMeasRprtHndlr \
                      measurement results not Present.\n"));
       RETVALUE(RFAILED);
   }
   measResults = &measReportIe->measResults;
   retVal = veHODecision(cellCb, measResults, &hoNeeded);
   if(ROK != retVal)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"\n veHODecision : Failed  .\n"));
      RETVALUE(RFAILED);
#endif
   }

   if(hoNeeded)
   {
      retVal = veHOFillCandCells(ueCb, measResults);
      if(ROK != retVal)
      {
         RETVALUE(RFAILED);
      }
      VE_ALLOC(&hoPrepEncSdu, sizeof(NhuEncReqSdus));
      if(NULLP == hoPrepEncSdu)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"\n veHODecision : Failed To allocate Memmory .\n"));
#endif
         RETVALUE(RFAILED);
      }      
      hoPrepEncSdu->hdr.cellId = ueCb->cellId;
      hoPrepEncSdu->hdr.ueId = ueCb->crnti;
      hoPrepEncSdu->hdr.transId = VE_ENC_TRANSID_HO_REQ;

      if(RFAILED == veUtlFillHoPreparationMsg(ueCb, hoPrepEncSdu))
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"\nveLIMRrcMeasRprtHndlr\
               Filling of Transparent Container failed.\n"));
      }
      else
      {
         VeLiNhuEncodeReq(&veCb.nhuSap[0]->pst, veCb.nhuSap[0]->suId, hoPrepEncSdu);
      }
   }

   RETVALUE(ROK);
}/* veLIMRrcMeasRprtHndlr */
#endif
/*
 *
 *      Fun:   veLIMRrcReconfigCmpHndlr
 *
 *      Desc:  RRC Reconfiguration complete
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veLIMRrcReconfigCmpHndlr
(
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 veLIMRrcReconfigCmpHndlr(datIndSdu)
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
   VeCellCb *cellCb = NULLP; 
   VeUeCb *ueCb = NULLP;
   VeS1ConCb *s1apConCb = NULLP;
   U16        ueIdx;
   SztDatEvntReq datEvt;
#ifndef LTE_ENB_PERF
   U32        transId;
#endif
   U16        retVal = 0;
#ifndef LTE_ENB_PERF
   S1apPdu *pdu = NULLP;
#ifdef YS_PICO
   SEND_DBG_MSG(("%s %d\n", __FILE__, __LINE__));
#endif
#endif
   TRC2(veLIMRrcReconfigCmpHndlr);

#ifdef VE_PICO   

   /* Enabling Call Scenario Prints ccpu00117125*/
    ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "UE -> ENB : RRC Reconfiguration Complete ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
#endif   
   cellCb = veCb.cellCb[0];
   ueIdx = VE_GET_UE_IDX(datIndSdu->hdr.ueId);

   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "\nveLIMRrcReconfigCmpHndlr(): Invalid ueIdx(%d)\n",ueIdx));
      RETVALUE(RFAILED);
   }
   
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " RRC Recfg Complete Message Rcvd.\
            from UeId:%x\n", datIndSdu->hdr.ueId));
#endif
#ifdef YS_PICO
   SEND_DBG_MSG("%s %d\n", __FILE__, __LINE__);
#endif
   s1apConCb = ueCb->s1ConCb;
#ifndef LTE_ENB_PERF
   transId = datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.\
   rrcConRecfgnCompl.rrc_TrnsxnId.val;
  
   /* RRC-ReEstab :update RRC State */
   if ( ueCb->rrcConnState == VE_SRB2_DRB_SUSPENDED)
   {
      ueCb->rrcConnState = VE_SRB2_ESTABLISHED;
   }
 
   switch(transId)
   {
      case VE_INIT_CNTXT_SETUP_REQ: /* Init Context Setup Request */
         {
#ifdef LTE_HO_SUPPORT
            if(ueCb->hoType == VE_HO_TYPE_S1)
            {
               if (ROK != veS1TgtFSM[ueCb->mobCtrlState][VE_EVNT_HO_RECONFIG_CMPL_RCVD](ueCb->s1ConCb, (void *)ueCb))
               {
                  VE_DBG_ERROR((VE_PRNT_BUF,"processing of HO Confirm failed\n"));      
               }
               /* Setting the UE paramenters to Default. This is valid only in 
                * case of S1 Handover. For X2 Handover, these values are set upon
                * reception of Path Switch Request Acknowledge message. */
               ueCb->hoType = VE_HO_TYPE_NONE;
               ueCb->mobCtrlState = VE_HO_NORMAL;
            }
            else if (ueCb->hoType == VE_HO_TYPE_X2)
            {
               if (ROK != veX2TgtFSM[ueCb->mobCtrlState][VE_EVNT_HO_RECONFIG_CMPL_RCVD](ueCb->x2ConCb, (void *)ueCb))
               {
                  VE_DBG_ERROR((VE_PRNT_BUF,"processing of HO Confirm failed\n"));      
               }
            }
            else
            {
#endif            
               /* Freeing */
               if(ueCb->pdu)
               {
                  cmFreeMem((Ptr)(ueCb->pdu));
               }
               /*Send Command to DATA APP for local Egtp tunnel creation*/
               veSndEgtpCom(VE_EGTP_TUN_CREATE, s1apConCb);

               /* update RRC State */
               ueCb->rrcConnState = VE_SRB2_ESTABLISHED;

               /* build S1ap msg. */
               retVal =  veUtlSztFillInitCntxtSetRsp(s1apConCb, datIndSdu, &pdu);

               if(ROK != retVal)
               {
#ifdef DEBUGP
                  VE_DBG_ERROR((VE_PRNT_BUF,"veLIMRrcReconfigCmpHndlr : Failed to Fill veUtlSztFillInitCntxtSetRsp \n"));
#endif
                  RETVALUE(RFAILED);
               }
               /* Assigning pointer , will be used to build initial context response */
               /* reassing to the pdu with initial context response pdu */
               ueCb->pdu = pdu;
#ifdef LTE_HO_SUPPORT            
            }
#endif            
         }
         break;

      case VE_ERAB_SETUP_REQ: /* E-RAB Setup Request */
         {

            /*Send Command to DATA APP for local Egtp tunnel creation*/
            veSndEgtpComDed(VE_EGTP_TUN_CREATE, s1apConCb);

            /* build S1ap Response Msg. */
         /* RRC-ReEstab */
            retVal = veUtlSztFillERABSetRsp(s1apConCb, &pdu,TRUE);
            if(ROK != retVal)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veLIMRrcReconfigCmpHndlr: Failed to Fill veUtlSztFillERABSetRsp \n"));
#endif
               RETVALUE(RFAILED);
            }

            /* Freeing  */
            cmFreeMem((Ptr)(ueCb->pdu));

            /* Assigning pointer , will be used to build initial context response */
            /* reassing to the pdu with initial context response pdu */
            ueCb->pdu = pdu;

         }
         break;

      case VE_ERAB_MODIFY_REQ: /* E-RAB Modify Request */
         {
            /* E-RAB Modify */
            VE_CMPTL_RABMDFY_UE_INFO(ueCb);
            /* build S1ap Response Msg. */
            retVal = veUtlSztFillERABMdfyRsp(s1apConCb, &pdu, TRUE);
            if(ROK != retVal)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veLIMRrcReconfigCmpHndlr: Failed to Fill veUtlSztFillERABMdfyRsp\n"));
#endif
               RETVALUE(RFAILED);
            }
            /* Freeing  */
            cmFreeMem((Ptr)(ueCb->pdu));
            /* sending S1AP dat req with E-RAB Rsp PDU */
            VE_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));
            datEvt.spConnId = s1apConCb->spConnId;
            datEvt.pdu = (S1apPdu *)pdu;       

            retVal = VeLiSztDatReq(&veCb.sztSap[0]->pst, veCb.sztSap[0]->spId, &datEvt);

            if(ROK != retVal)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,
                        "\nnveLIMRrcUlInfoTfrHndlr(): Failed to Send ULNasTrans.\n"));
               RETVALUE(RFAILED);
#endif    
            } 
            /* E-RAB Modify End */ 
         }
         break;

      case VE_ERAB_RLS_COMM: /* E-RAB Release Command  */
         {
                     /* Send EGTP release command to EGTP-U*/
            veSndEgtpComDed(VE_EGTP_TUN_DEL, s1apConCb);
            /* veSndEgtpCom(VE_EGTP_TUN_DEL, s1apConCb); */

            /* build S1ap Response Msg. */
         /* RRC-ReEstab */
            retVal = veUtlSztFillERABRelRsp(s1apConCb, &pdu,TRUE);
            if(ROK != retVal)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veUtlCztFillX2SetupReq: cmGetMem failed \n"));
#endif     
               RETVALUE(RFAILED);
            }
           /* free S1AP ERAB Rel Command PDU */
            cmFreeMem((Ptr)(ueCb->pdu));

            /* Assigning pointer , will be used to build initial context response */
            /* reassing to the pdu with initial context response pdu */
            ueCb->pdu = pdu;

         }
         break;
      case VE_UE_CNTXT_MOD_REQ: /* UE Context Modification request */
      default:
         break;
   }
   /* update RRC State : Taken care in the switch case above */
   ueCb->reEstCause = INVALID_CAUSE;
#endif
   RETVALUE(ROK);
} /* veLIMRrcReconfigCmpHndlr */

/*
 *
 *      Fun:   veNoOpr
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
PUBLIC S16 veNoOpr
(
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 veNoOpr(datIndSdu)
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
   NhuUL_CCCH_Msg  *ulCcchPdu = NULLP;  /* UL CCCH structure */
   NhuUL_DCCH_Msg  *ulDcchPdu = NULLP;  /* UL DCCH structure */
   U16 msgCat = 0;
   U16 msgTyp = 0;

   TRC2(veNoOpr);
#ifdef YS_PICO
   SEND_DBG_MSG("No Operation Reached\n");
#endif
   msgCat = datIndSdu->sdu.msgCategory;

   if(msgCat == NHU_MSG_ULCCCH)
   {
      ulCcchPdu = &(datIndSdu->sdu.m.ulCcchMsg.ccchMsg);
      msgTyp = ulCcchPdu->message.val.c1.choice.val;

   }
   else
   {
      ulDcchPdu = &(datIndSdu->sdu.m.ulDcchMsg.dcchMsg);
      msgTyp = ulDcchPdu->message.val.c1.choice.val;

   }
   /* Nothing to be done */
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\n veNoOpr: Unknown msg. UE: %d.\
            Msg. Catg.: %d Msg. Type: %d\n", datIndSdu->hdr.ueId, msgCat, msgTyp));
#endif

   RETVALUE(ROK);
}/* veNoOpr */

/*
 *
 *      Fun:   veSndCtfUeRls
 *
 *      Desc:  Send Ctf Ue Release
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndCtfUeRls
(
VeUeCb        *ueCb
)
#else
PUBLIC S16 veSndCtfUeRls(ueCb)
VeUeCb        *ueCb;
#endif
{
   Pst           *pst = NULLP;
   CtfCfgReqInfo *cfgInfo;
   CtfCfgTransId transId;

   TRC2(veSndCtfUeRls);

   pst = &veCb.ctfSap[0]->pst;
   pst->event = EVTCTFCFGREQ;

   VE_SET_ZERO(&transId, sizeof(CtfCfgTransId));

   VE_ALLOC(&cfgInfo, sizeof(CtfCfgReqInfo));
   if(cfgInfo == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndCtfUeRls mem alloc failed.\n"));      
      RETVALUE(RFAILED);
   }
#ifdef CTF_VER3
    cfgInfo->vendorParams.paramBuffer = NULLP;
    cfgInfo->vendorParams.buffLen = 0;
#endif

   cfgInfo->cfgType = CTF_DELETE;
   cfgInfo->u.release.cfgElem = CTF_UE_CFG;
   cfgInfo->u.release.u.dedRel.cellId = ueCb->cellId;
   cfgInfo->u.release.u.dedRel.ueId = ueCb->crnti;

   if(ROK != VeLiCtfCfgReq(pst, veCb.ctfSap[0]->spId, transId, cfgInfo))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCtfCfgReq failed.\n"));      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndCtfUeRls */

/*
 *
 *      Fun:   veSndCtfUeCfg
 *
 *      Desc:  Send Ctf Ue config
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndCtfUeCfg
(
VeUeCb        *ueCb
)
#else
PUBLIC S16 veSndCtfUeCfg(ueCb)
VeUeCb        *ueCb;
#endif
{
   Pst *pst = NULLP;
   CtfCfgReqInfo *cfgInfo;
   CtfCfgTransId transId;
   TknU8        srCfgIdx;
   TknU8        cqiPmiCfgIdx;

   CtfDedCfgInfo  *ueCfgParam = NULLP;

   TRC2(veSndCtfUeCfg);

   pst = &veCb.ctfSap[0]->pst;
   pst->event = EVTCTFCFGREQ;

   /* Initialize */
   VE_SET_ZERO(&transId, sizeof(CtfCfgTransId));

   VE_ALLOC(&cfgInfo, sizeof(CtfCfgReqInfo));

   if(cfgInfo == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndCtfUeCfg mem alloc failed.\n"));      
      RETVALUE(RFAILED);
   }
#ifdef CTF_VER3
    cfgInfo->vendorParams.paramBuffer = NULLP;
    cfgInfo->vendorParams.buffLen = 0;
#endif


   cfgInfo->cfgType = CTF_CONFIG;
   cfgInfo->u.cfg.cfgElem = CTF_UE_CFG;

   ueCfgParam = &cfgInfo->u.cfg.u.dedCfg;

   if(ueCb == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndCtfUeCfg Invalid UeCb\n"));      
      RETVALUE(RFAILED);
   }
   else
   {
      ueCfgParam->cellId = ueCb->cellId;
      ueCfgParam->ueId = ueCb->crnti;
   }

   /* Ue category */
   ueCfgParam->ueCatCfg.pres = TRUE;
   ueCfgParam->ueCatCfg.ueCategory = veDfltUECategory;

   /* PDSCH Cfg */
   ueCfgParam->pdschCfg.pres = TRUE;
   ueCfgParam->pdschCfg.pA = veDfltPdschPA;

   /* PUCCH Cfg */
   ueCfgParam->pucchCfg.pres = TRUE;
   ueCfgParam->pucchCfg.dedPucchCfgType = CTF_IE_CFG_SETUP;
   ueCfgParam->pucchCfg.pucchSetup.repFact = veDfltRepFact;
   ueCfgParam->pucchCfg.pucchSetup.n1PUCCHRep = veDfltN1PUCCHRep;

   /* PUSCH Cfg */
   ueCfgParam->puschCfg.pres = TRUE;
   /* klock warning fixed */
   ueCfgParam->puschCfg.betaOffsetAckIdx = (U8)veDfltBetaOffACKIdx;
   ueCfgParam->puschCfg.betaOffsetRiIdx = (U8)veDfltBetaOffRIIdx;
   ueCfgParam->puschCfg.betaOffsetCqiIdx = (U8)veDfltBetaOffCQIIdx;

/* MS_FIX: tfu upgrade changes */
#ifndef TFU_UPGRADE
   /* CQI Reporting */
/* RNT: disabling CQI */
/* MSPD changes */
#ifdef MSPD
   ueCfgParam->cqiRptCfg.pres = FALSE;
#else
   ueCfgParam->cqiRptCfg.pres = TRUE;
#endif
   ueCfgParam->cqiRptCfg.reportingMode = CTF_CQI_RPTMODE_PRDIOC;

   /* APERIODIC is not supported for now */
   ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiPeriodicCfgType 
      = CTF_IE_CFG_SETUP;
   ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPUCCHRsrcIndx 
      = veDfltCqiPUCCHResIdx;
   ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPmiCfgIndx 
      = 38;
   ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.formatIndicator 
      = (U8)veDfltFormatInd;/* klock warning fixed */
   ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.subbandCqi.k 
      = (U8)veDfltSubBandK;/* klock warning fixed */
#ifndef LTEMAC_MIMO
   ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgPres
      = FALSE;
#else
   ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgPres
      = FALSE;
   ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgIndx
      = veDfltRiCfgIndx;
      /* This would make RI reporting freq half as that of WB CQI/PMI.*/
      /* With cqiPmiCfgIndx=38, CQIs would be reported at 40.1 80.1 ....(sfn.subframe)s
         and RiCfgIndx=163, RIs would be reported at 79.9, 159.9,.... */
#endif
   ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.simultaneousAckNack
      = veDfltSimultAckNackAndCQI;

   /* UL SRS Cfg */
   ueCfgParam->srsUlCfg.pres = NOTPRSNT;
   /* Dedicated SR Cfg */
   ueCfgParam->dedSRCfg.pres = TRUE;
   ueCfgParam->dedSRCfg.dedSRCfgType = CTF_IE_CFG_SETUP;
   ueCfgParam->dedSRCfg.dedSrSetup.srPUCCHRi = veDfltSrPUCCHResIdx;

   veGetSRCfgIdx(&srCfgIdx);
   ueCfgParam->dedSRCfg.dedSrSetup.srCfgIdx = srCfgIdx.val;

   ueCb->srCfgIdx = srCfgIdx.val;

#else
   veGetSRCfgIdx(&srCfgIdx);
   ueCb->srCfgIdx = srCfgIdx.val;
   veGetCqiPmiCfgIdx(&cqiPmiCfgIdx);
   ueCb->cqiPmiCfgIdx = cqiPmiCfgIdx.val;
#endif /* TFU_UPGRADE */

   /* Antenna Cfg */
   ueCfgParam->antInfo.pres = TRUE;
   ueCfgParam->antInfo.txMode = veDfltAntInfoTxMode;
   ueCfgParam->antInfo.ueTxAntSelection.cfgType = CTF_IE_CFG_SETUP;
   ueCfgParam->antInfo.ueTxAntSelection.txAntSelect = veDfltAntennaSel;

   if(ROK != VeLiCtfCfgReq(pst, veCb.ctfSap[0]->spId, transId, cfgInfo))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCtfCfgReq failed.\n"));      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndCtfUeCfg */

/* UE Cntx Mod Start */
/*
 *
 *      Fun:   veSndAmbrRgrCfg
 *
 *      Desc:  Send Rgr Logical group config when Ue Context modification
 *             received
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndAmbrRgrCfg
(
VeUeCb        *ueCb,
VeUeRAB       *rabInfo
)
#else
PUBLIC S16 veSndAmbrRgrCfg(ueCb, rabInfo)
VeUeCb        *ueCb;
VeUeRAB       *rabInfo;
#endif
{
   Pst *pst = NULLP;
   RgrCfgReqInfo *cfgInfo = NULLP;
   RgrCfgTransId transId;
   RgrUeRecfg *ueReCfg = NULLP;

   TRC2(veSndRgrLchCfg);

   pst = &veCb.rgrSap[0]->pst;
   pst->event = EVTRGRCFGREQ;

   /* Initialize */
   VE_ALLOC(&cfgInfo, sizeof(RgrCfgReqInfo));

   if(cfgInfo == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndAmbrRgrCfg mem alloc failed.\n"));
      RETVALUE(RFAILED);
   }


   VE_SET_ZERO(&transId, sizeof(RgrCfgTransId));

   cfgInfo->action = RGR_RECONFIG;
   cfgInfo->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueReCfg = &(cfgInfo->u.recfgInfo.u.ueRecfg);

   ueReCfg->ueRecfgTypes = RGR_UE_QOS_RECFG;
   ueReCfg->cellId   = ueCb->cellId;
   ueReCfg->oldCrnti = ueCb->crnti;
   ueReCfg->newCrnti = ueCb->crnti;

   ueReCfg->ueQosRecfg.ambrPres = TRUE;
   ueReCfg->ueQosRecfg.dlAmbr = ueCb->rbInfo.aggBr.dl;
   ueReCfg->ueQosRecfg.ueBr = ueCb->rbInfo.aggBr.ul;

   /* Fill appropriate TransId details */ 
   VE_SET_RGR_TRANS(transId.trans, RGR_UE_CFG, rabInfo->rbId, ueCb->crnti);

   /* Rgr Cfg Req primitive */
   if(ROK != VeLiRgrCfgReq(pst, veCb.rgrSap[0]->spId, transId, cfgInfo))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrLchCfg failed.\n"));      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndAmbrRgrCfg */
/* UE Cntx Mod end */

/*
 *
 *      Fun:   veSndRgrLcgCfg
 *
 *      Desc:  Send Rgr Logical group config
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndRgrLchCfg
(
VeUeCb        *ueCb,
VeUeRAB       *rabInfo,
Bool          lchRecfg
)
#else
PUBLIC S16 veSndRgrLchCfg(ueCb, rabInfo, lchRecfg)
VeUeCb        *ueCb;
VeUeRAB       *rabInfo;
Bool          lchRecfg;
#endif
{
   Pst *pst = NULLP;
   RgrCfgReqInfo *cfgInfo = NULLP;
   RgrCfgTransId transId;
   RgrLchCfg *dedLchCfg = NULLP;

   TRC2(veSndRgrLchCfg);

   pst = &veCb.rgrSap[0]->pst;
   pst->event = EVTRGRCFGREQ;

   /* Initialize */
   VE_ALLOC(&cfgInfo, sizeof(RgrCfgReqInfo));

   if(cfgInfo == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrLchCfg mem alloc failed.\n"));
      RETVALUE(RFAILED);
   }


   VE_SET_ZERO(&transId, sizeof(RgrCfgTransId));

    /* E-RAB Modify */ 
   if(lchRecfg != TRUE)
   {
      cfgInfo->action = RGR_CONFIG;
   }
   else
   {
      cfgInfo->action = RGR_RECONFIG;
   }
 /* E-RAB Modify End */ 
   cfgInfo->u.cfgInfo.cfgType = RGR_LCH_CFG;
   dedLchCfg = &(cfgInfo->u.cfgInfo.u.lchCfg);

   dedLchCfg->cellId = ueCb->cellId;
   dedLchCfg->crnti = ueCb->crnti;

   /* Logical channel ID  */
   dedLchCfg->lcId = rabInfo->rbId;
/* MSPD changes */
#ifdef MSPD
   printf ("\n Sending LC config for (%d) \n", dedLchCfg->lcId);
#endif

   if(rabInfo->rbType == CM_LTE_SRB)
   {
      dedLchCfg->lcType = CM_LTE_LCH_DCCH; 
      dedLchCfg->dlInfo.dlQos.qci = 0 ;/*mnawas:: Filling high qci for DCCH */ 
   }
   else
   {
      dedLchCfg->lcType = CM_LTE_LCH_DTCH;
      /* Downlink logical channel configuration info */
#ifdef LTE_ENB_PERF
      dedLchCfg->dlInfo.dlQos.qci = veDfltQci;
      dedLchCfg->dlInfo.dlQos.gbr = veDfltDlQosGbr;
      dedLchCfg->dlInfo.dlQos.mbr = veDfltDlQosMbr;
#else
       /* E-RAB Modify */ 
      if (lchRecfg != TRUE)
      {
         dedLchCfg->dlInfo.dlQos.qci = (U8) rabInfo->qci;
         dedLchCfg->dlInfo.dlQos.gbr = veDfltDlQosGbr;
         dedLchCfg->dlInfo.dlQos.mbr = veDfltDlQosMbr;
      }
      else
      {
         dedLchCfg->dlInfo.dlQos.qci = (U8)rabInfo->rabMdfyInfo.qci;
         dedLchCfg->dlInfo.dlQos.gbr = rabInfo->rabMdfyInfo.gbr.dl;
         dedLchCfg->dlInfo.dlQos.mbr = rabInfo->rabMdfyInfo.mbr.dl;
      }
       /* E-RAB Modify End */ 
#endif
   }

   /* Fill appropriate TransId details */ 
   VE_SET_RGR_TRANS(transId.trans, RGR_LCH_CFG, rabInfo->rbId, ueCb->crnti);

   /* Rgr Cfg Req primitive */
   if(ROK != VeLiRgrCfgReq(pst, veCb.rgrSap[0]->spId, transId, cfgInfo))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrLchCfg failed.\n"));      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndRgrLchCfg */
#ifndef ERAB_RELEASE
/*
 *
 *      Fun:   veSndRgrDelLchCfg
 *
 *      Desc:  Send Rgr Logical channel config
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndRgrDelLchCfg
(
VeUeCb        *ueCb,
VeUeRAB       *rabInfo
)
#else
PUBLIC S16 veSndRgrDelLchCfg(ueCb, rabInfo)
VeUeCb        *ueCb;
VeUeRAB       *rabInfo;
#endif
{
   Pst *pst = NULLP;
   RgrCfgReqInfo *cfgInfo = NULLP;
   RgrCfgTransId transId;
   RgrDel   *delInfo = NULLP;

   TRC2(veSndRgrDelLchCfg);

   pst = &veCb.rgrSap[0]->pst;
   pst->event = EVTRGRCFGREQ;

   /* Initialize */
   VE_ALLOC(&cfgInfo, sizeof(RgrCfgReqInfo));

   if(cfgInfo == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrLchDelCfg mem alloc failed.\n"));
      RETVALUE(RFAILED);
   }


   VE_SET_ZERO(&transId, sizeof(RgrCfgTransId));

   cfgInfo->action = RGR_DELETE;
   cfgInfo->u.cfgInfo.cfgType = RGR_LCH_CFG;
   delInfo = &(cfgInfo->u.delInfo);

   delInfo->u.lchDel.cellId = ueCb->cellId;
   delInfo->u.lchDel.crnti = ueCb->crnti;

   /* Logical channel ID  */
   delInfo->u.lchDel.lcId = rabInfo->rbId;

   /* Fill appropriate TransId details */ 
   VE_SET_RGR_TRANS(transId.trans, RGR_LCH_CFG, rabInfo->rbId, ueCb->crnti);

   /* Rgr Cfg Req primitive */
   if(ROK != VeLiRgrCfgReq(pst, veCb.rgrSap[0]->spId, transId, cfgInfo))
   {
      printf("\n VeLiRgrCfgReq is failed\n");
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrDelLchCfg failed.\n"));      
      RETVALUE(RFAILED);
   }
printf("\n VeLiRgrCfgReq is success\n");
   RETVALUE(ROK);
} /* veSndRgrDelLchCfg*/

#endif

/*
 *
 *      Fun:   veSndRgrLcgCfg
 *
 *      Desc:  Send Rgr Logical group config
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndRgrLcgCfg
(
VeUeCb        *ueCb,
U8            lcId,
U8            grpId
)
#else
PUBLIC S16 veSndRgrLcgCfg(ueCb, lcId, grpId)
VeUeCb        *ueCb;
U8            lcId;
U8            grpId;
#endif
{
   Pst *pst = NULLP;
   RgrCfgReqInfo *cfgInfo;
   RgrCfgTransId transId;
   RgrLcgCfg *lcgCfg = NULLP;

   TRC2(veSndRgrLcgCfg);

   pst = &veCb.rgrSap[0]->pst;
   pst->event = EVTRGRCFGREQ;

   VE_ALLOC(&cfgInfo, sizeof(RgrCfgReqInfo));

   if(cfgInfo == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrLchCfg mem alloc failed.\n"));
      RETVALUE(RFAILED);
   }

   VE_SET_ZERO(&transId, sizeof(RgrCfgTransId));

   cfgInfo->action = RGR_CONFIG;
   cfgInfo->u.cfgInfo.cfgType = RGR_LCG_CFG;
   lcgCfg = &(cfgInfo->u.cfgInfo.u.lcgCfg);

   lcgCfg->cellId = ueCb->cellId;
   lcgCfg->crnti = ueCb->crnti;

   /* Uplink logical channel configuration information */
   lcgCfg->ulInfo.lcgId = grpId;

   /* Fill appropriate TransId details */
   VE_SET_RGR_TRANS(transId.trans, RGR_LCG_CFG, lcId, ueCb->crnti);

   /* Rgr Cfg Req primitive */
   if(ROK != VeLiRgrCfgReq(pst, veCb.rgrSap[0]->spId, transId, cfgInfo))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrLcgCfg failed.\n"));      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndRgrLcgCfg */

/*
 *
 *      Fun:   veSndRgrUeRls
 *
 *      Desc:  Send Rgr Ue Release 
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndRgrUeRls
(
VeUeCb        *ueCb,
VeUeRAB       *rabInfo,/*RRC ReEst */
U8             delType/* RRC ReEst*/
)
#else
PUBLIC S16 veSndRgrUeRls(ueCb,rabInfo,delType)
VeUeCb        *ueCb;
VeUeRAB       *rabInfo;
U8             delType;
#endif
{
   RgrCfgReqInfo *cfgInfo = NULLP; /* Modified to pointer */
   RgrCfgTransId transId;
   Pst *pst = NULLP;

   TRC2(veSndRgrUeRls);

   pst = &veCb.rgrSap[0]->pst;
   pst->event = EVTRGRCFGREQ;

   /* Initialize */
   VE_SET_ZERO(&transId, sizeof(RgrCfgTransId));

   /* Allocating memory to cfgInfo */
   VE_ALLOC(&cfgInfo, sizeof(RgrCfgReqInfo));

   if(cfgInfo == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeRls: mem alloc failed.\n"));
      RETVALUE(RFAILED);
   }
   if(delType == RGR_UE_CFG)
   {
     cfgInfo->action = RGR_DELETE;
     cfgInfo->u.delInfo.delType = RGR_UE_CFG;
     cfgInfo->u.delInfo.u.ueDel.cellId = ueCb->cellId;
     cfgInfo->u.delInfo.u.ueDel.crnti = ueCb->crnti;
   }
   else
   {
     cfgInfo->u.delInfo.delType = RGR_LCH_CFG;
     cfgInfo->u.delInfo.u.lchDel.cellId = ueCb->cellId;
     cfgInfo->u.delInfo.u.lchDel.crnti = ueCb->crnti;
     cfgInfo->u.delInfo.u.lchDel.lcId = rabInfo->rbId;
   /* Fill appropriate TransId details */
     VE_SET_RGR_TRANS(transId.trans, RGR_LCH_CFG, rabInfo->rbId, ueCb->crnti);
   }

   if(ROK != VeLiRgrCfgReq(pst, veCb.rgrSap[0]->spId, transId, cfgInfo))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgReq failed.\n"));      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndRgrUeRls */
#ifdef LTE_HO_SUPPORT
/*
 *
 *      Fun:   veSndX2UeLocRlsReq
 *
 *      Desc:  Send UE Release to X2AP 
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndX2UeLocRlsReq
(
VeX2ConCb  *x2ConCb
)
#else
PUBLIC S16 veSndX2UeLocRlsReq(x2ConCb)
VeX2ConCb  *x2ConCb;
#endif
{
   CztRlsReq *relReq = NULLP;
   TRC2(veSndX2UeLocRlsReq);

   VE_ALLOC(&relReq, sizeof(CztRlsReq));

   if(NULLP == relReq)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndX2UeLocRlsReq: failed to allocate memory.\n"));
      RETVALUE(RFAILED);
   }

   relReq->peerId = x2ConCb->peerId;
   relReq->idType = CZT_IDTYPE_LOCAL;
   relReq->rlsType = CZT_RELTYPE_UE;
   relReq->u.oldUEX2APId = x2ConCb->oldEnbUeX2apId;

   VeLiCztRelReq(&(veCb.cztSap[0]->pst), veCb.cztSap[0]->suId, relReq);

   RETVALUE(ROK);
} /* End of veSndX2UeLocRlsReq */
#endif

/*
 *
 *      Fun:   veSndS1UeLocRlsReq
 *
 *      Desc:  Send UE Release to S1AP
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndS1UeLocRlsReq
(
VeS1ConCb  *s1ConCb
)
#else
PUBLIC S16 veSndS1UeLocRlsReq(s1ConCb)
VeS1ConCb  *s1ConCb;
#endif
{
   SztRelReq *relReq = NULLP;
   TRC2(veSndS1UeLocRlsReq);

   VE_ALLOC(&relReq, sizeof(SztRelReq));
   if(NULLP == relReq)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndS1UeLocRlsReq: failed to allocate memory.\n"));
      RETVALUE(RFAILED);
   }

   relReq->connId = s1ConCb->suConnId;
   relReq->relType = SZT_LOCAL_REL;
   relReq->connIdType = SZT_CONN_TYPE_SU;
#ifdef LTE_HO_SUPPORT
   VeLiSztRelReq(&veCb.sztSap[0]->pst, veCb.sztSap[0]->spId, relReq);
#endif

   RETVALUE(ROK);
} /* End of veSndS1UeLocRlsReq */

#ifndef LTE_ENB_PERF
/*
 *
 *      Fun:   veSndEgtpCom
 *
 *      Desc:  Send Egtp com
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndEgtpCom
(
U8 egtpCom,
VeS1ConCb *s1apConCb
)
#else
PUBLIC S16 veSndEgtpCom(egtpCom,s1apConCb)
U8 egtpCom;
VeS1ConCb *s1apConCb;
#endif
{
   EgtpTunInf tunelInf; 
   Pst pst;
   U8 idx;
   VeUeCb *ueCb = NULLP;

   SztGTP_TEID *remTeIdStr = NULLP;
   U8 indx, shiftBits;
   U32 addrMask;

   TRC2(veSndEgtpCom);

   if(NULLP == s1apConCb)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndEgtpCom: s1apConCb is NULL.\n"));
      RETVALUE(RFAILED);
   }

   ueCb = s1apConCb->ueCb;
   VE_SET_ZERO(&pst, sizeof(Pst));

   /*Currently hard coding the pst structure*/
   pst.dstEnt = ENTVE;
   pst.srcEnt = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_VE_PROC;


   /*Create EGTP tunnels as many eRabs configured*/

   for (idx = 0; idx < VE_MAX_RABS_IN_UE; idx++)
   {
      if((ueCb->rbInfo.rab[idx].inUse == VE_DRB_CONNECTING))       
      {

         if(egtpCom == VE_EGTP_TUN_CREATE)
         {
            VE_SET_ZERO(&tunelInf, sizeof(EgtpTunInf));
            tunelInf.locTeid = ueCb->rbInfo.rab[idx].locEgtpTunId;

            /* Retrieve the remote S-GW's EGTP-U  tunnel Id */
            remTeIdStr = &(ueCb->rbInfo.rab[idx].remEgtpTunId);
            shiftBits = (U8)(remTeIdStr->len);/* warning fix */
            addrMask = 0xFF000000;
            for(indx = 0;indx < remTeIdStr->len;indx++)
            {
               shiftBits--;
               tunelInf.remTeid |= ((U32)(remTeIdStr->val[indx] << (8*shiftBits)) & addrMask); 
               addrMask = addrMask>>8;
            }

            /* Tuning the local tunnel Id and PDCP Id for proper indexing of
               EGTP Tunnel List and PDCP Id List */
            tunelInf.locTeid = ueCb->rbInfo.rab[idx].locEgtpTunId;
            tunelInf.pdcpId.cellId = ueCb->cellId;
            tunelInf.pdcpId.ueId = (CmLteRnti)ueCb->crnti;

            tunelInf.pdcpId.rbId = ueCb->rbInfo.rab[idx].pdcpId;
            tunelInf.pdcpId.rbType = CM_LTE_DRB;
            /* memcpy the serv. GW */
            cmMemcpy((U8 *)&tunelInf.dstIpAddr, (U8 *)&ueCb->rbInfo.rab[idx].sgwAddr, sizeof(CmTptAddr));
            /* post message to ENTEU inorder to trigger tunnel creation */ 
            cmPkEuCreattLocTunReq(&pst, 0, &tunelInf);
         }
      }
      else if((ueCb->rbInfo.rab[idx].inUse == VE_DRB_DELETING))
      {
         if(egtpCom == VE_EGTP_TUN_DEL)
         {
                           /* number of tunnels delete request */
            ueCb->numOfTunnelDelReq++;
            cmPkEuDelLocTunReq(&pst, 0, ueCb->rbInfo.rab[idx].locEgtpTunId);
            /* free related info from UeCb */
            ueCb->rbInfo.rab[idx].remEgtpTunId.pres = NOTPRSNT;
            VE_FREE(ueCb->rbInfo.rab[idx].remEgtpTunId.val,\
                  ueCb->rbInfo.rab[idx].remEgtpTunId.len);
         }
      }
   }

   RETVALUE(ROK);
}
/*
 *
 *      Fun:   veSndEgtpComDed
 *
 *      Desc:  Send Egtp command for Dedicate Bearers
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndEgtpComDed
(
U8 egtpCom,
VeS1ConCb *s1apConCb
)
#else
PUBLIC S16 veSndEgtpComDed(egtpCom,s1apConCb)
U8 egtpCom;
VeS1ConCb *s1apConCb;
#endif
{
   EgtpTunInf tunelInf; 
   Pst pst;
   U8 idx;
   VeUeCb *ueCb = NULLP;

   SztGTP_TEID *remTeIdStr = NULLP;
   U8 indx, shiftBits;
   U32 addrMask;

   TRC2(veSndEgtpComDed)

      ueCb = s1apConCb->ueCb;
   VE_SET_ZERO(&pst, sizeof(Pst));

   /*Currently hard coding the pst structure*/
   pst.dstEnt = ENTVE;
   pst.srcEnt = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_VE_PROC;



   for (idx = 0; idx < VE_MAX_RABS_IN_UE; idx++)
   {
      if(ueCb->rbInfo.rab[idx].inUse == VE_DRB_CONNECTING)
      {

         if(egtpCom == VE_EGTP_TUN_CREATE)
         {
            VE_SET_ZERO(&tunelInf, sizeof(EgtpTunInf));
            tunelInf.locTeid = ueCb->rbInfo.rab[idx].locEgtpTunId;

            /* Retrieve the remote S-GW's EGTP-U  tunnel Id */
            remTeIdStr = &(ueCb->rbInfo.rab[idx].remEgtpTunId);
            shiftBits = (U8)(remTeIdStr->len);/* warning fix */
            addrMask = 0xFF000000;
            for(indx = 0;indx < remTeIdStr->len;indx++)
            {
               shiftBits--;
               tunelInf.remTeid |= ((U32)(remTeIdStr->val[indx] << (8*shiftBits)) & addrMask); 
               addrMask = addrMask>>8;
            }

            /* Tuning the local tunnel Id and PDCP Id for proper indexing of
          EGTP Tunnel List and PDCP Id List */
            tunelInf.locTeid = ueCb->rbInfo.rab[idx].locEgtpTunId;
            tunelInf.pdcpId.cellId = ueCb->cellId;
            tunelInf.pdcpId.ueId = (CmLteRnti)ueCb->crnti;

            tunelInf.pdcpId.rbId = ueCb->rbInfo.rab[idx].pdcpId;
            tunelInf.pdcpId.rbType = CM_LTE_DRB;
            /* memcpy the serv. GW */
            cmMemcpy((U8 *)&tunelInf.dstIpAddr, (U8 *)&ueCb->rbInfo.rab[idx].sgwAddr, sizeof(CmTptAddr));
            /* post message to ENTEU inorder to trigger tunnel creation */ 
            cmPkEuCreattLocTunReq(&pst, 0, &tunelInf);
         }
      }/* VE_DRB_CONNECTING */
      else if(ueCb->rbInfo.rab[idx].inUse == VE_DRB_DELETING)
      {
         if(egtpCom ==  VE_EGTP_TUN_DEL)
         {
               ueCb->numOfTunnelDelReq++;
            /* Trigger Deletion */
            cmPkEuDelLocTunReq(&pst, 0, ueCb->rbInfo.rab[idx].locEgtpTunId);
            /* free related info from UeCb */
            ueCb->rbInfo.rab[idx].remEgtpTunId.pres = NOTPRSNT;
            VE_FREE(ueCb->rbInfo.rab[idx].remEgtpTunId.val,\
                  ueCb->rbInfo.rab[idx].remEgtpTunId.len);
         }
      } /* VE_DRB_DELETING state */
    } /* for loop */
   RETVALUE(ROK);
}

#endif

/*
 *
 *      Fun:   veFillRgrUeTxModeCfg
 *
 *      Desc:  Populate RgrUeTxModeCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrUeTxModeCfg
(
RgrUeTxModeCfg *ueCfg
)
#else
PUBLIC S16 veFillRgrUeTxModeCfg(ueCfg)
RgrUeTxModeCfg *ueCfg;
#endif
{
   TRC2(veFillRgrUeTxModeCfg);

   if(NULLP == ueCfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrUeTxModeCfg: ueCfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   ueCfg->pres = TRUE;
   ueCfg->txModeEnum = (RgrTxMode)veDfltTxModeEnum;/* g++ compialtion fix  */
   
   /* Filling of ueCatEnum Removed. */

   RETVALUE(ROK);
} /* veFillRgrUeTxModeCfg */

/*
 *
 *      Fun:   veFillRgrUeDlCqiCfg
 *
 *      Desc:  Populate RgrUeDlCqiCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrUeDlCqiCfg
(
RgrUeDlCqiCfg *ueCfg,
VeUeCb        *ueCb
)
#else
PUBLIC S16 veFillRgrUeDlCqiCfg(ueCfg, ueCb)
RgrUeDlCqiCfg *ueCfg;
VeUeCb        *ueCb;
#endif
{
   TRC2(veFillRgrUeDlCqiCfg);

  if(NULLP == ueCfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrUeDlCqiCfg: ueCfg is NULL.\n"));
      RETVALUE(RFAILED);
   } 

   ueCfg->aprdCqiCfg.pres = FALSE;
   ueCfg->aprdCqiCfg.aprdModeEnum = (RgrAprdCqiMode)veDfltAPrdModeEnum;/* g++ compialtion fix  */
   
/* MS_FIX: tfu upgrade changes */
#ifndef TFU_UPGRADE
   ueCfg->prdCqiCfg.pres = FALSE;
   ueCfg->prdCqiCfg.prdModeEnum = (RgrPrdCqiMode)veDfltPrdModeEnum;
   ueCfg->prdCqiCfg.prdicityEnum = (RgrCqiPrdicity)veDfltPrdicityEnum;
   ueCfg->prdCqiCfg.subframeOffst = veDfltSubframeOffst;
   ueCfg->prdCqiCfg.cqiOffst = veDfltCqiOffst;
   ueCfg->prdCqiCfg.k = veDfltCqiCfgk;
   ueCfg->prdCqiCfg.cqiPmiCfgIdx = veDfltCqiPmiCfgIdx;
#else
   /* TODO: Need to fill actual values */
#ifdef VE_DL_CQI
   ueCfg->prdCqiCfg.type = 1; /* Setup */
#else
ueCfg->prdCqiCfg.type = 0; /* Release */
#endif
   ueCfg->prdCqiCfg.cqiSetup.cqiPResIdx = veDfltCqiPUCCHResIdx; /*!< cqi-PUCCH-ResourceIndex (0.. 1185) */
   ueCfg->prdCqiCfg.cqiSetup.cqiPCfgIdx = ueCb->cqiPmiCfgIdx;/*veDfltCqiPmiCfgIdx; *//*!< cqi-pmi-ConfigIndex (0..1023) */
   ueCfg->prdCqiCfg.cqiSetup.cqiRepType = 1; /*!< Wideband CQI = 1  Subband CQI =2 */
   ueCfg->prdCqiCfg.cqiSetup.k = veDfltCqiCfgk; /*!< Ref: 36.213 [23, 7.2.2] (1..4).
                                                  Valid only for Subband CQI */
   ueCfg->prdCqiCfg.cqiSetup.riEna = FALSE; /*!< Rand Indicator is Enabled TRUE(1) FALSE(0) */
   ueCfg->prdCqiCfg.cqiSetup.riCfgIdx = veDfltRiConfigIdx; /*!< ri-ConfigIndex    (0..1023)  */
   ueCfg->prdCqiCfg.cqiSetup.sANCQI = veDfltSimultAckNackAndCQI;  /*!< simultaneousAckNackAndCQI TRUE(1) FALSE(0) */
   ueCfg->prdCqiCfg.cqiSetup.prdModeEnum = (RgrPrdCqiMode)veDfltPrdModeEnum; /*!< Peiodic CQI reporting mode */
#endif

   RETVALUE(ROK);
} /* veFillRgrUeDlCqiCfg */

/*
 *
 *      Fun:   veFillRgrUeUlHqCfg
 *
 *      Desc:  Populate RgrUeUlHqCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrUeUlHqCfg
(
RgrUeUlHqCfg *ueCfg
)
#else
PUBLIC S16 veFillRgrUeUlHqCfg(ueCfg)
RgrUeUlHqCfg *ueCfg;
#endif
{
   TRC2(veFillRgrUeUlHqCfg);

   if(NULLP == ueCfg)
   { 
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrUeUlHqCfg: ueCfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   ueCfg->maxUlHqTx = veDfltMaxUlHqTx;
   ueCfg->deltaHqOffst = veDfltDeltaHqOffst;

   RETVALUE(ROK);
} /* veFillRgrUeUlHqCfg */


/*
 *
 *      Fun:   veFillRgrUeUlPwrCfg
 *
 *      Desc:  Populate RgrUeUlPwrCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrUeUlPwrCfg
(
RgrUeUlPwrCfg *ueCfg
)
#else
PUBLIC S16 veFillRgrUeUlPwrCfg(ueCfg)
RgrUeUlPwrCfg *ueCfg;
#endif
{
   TRC2(veFillRgrUeUlPwrCfg);

   if(NULLP == ueCfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrUeUlPwrCfg: ueCfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   ueCfg->uePuschPwr.pres = FALSE;
   ueCfg->uePucchPwr.pres = FALSE;
   ueCfg->isAccumulated = TRUE;
#ifdef VE_PICO
   ueCfg->isAccumulated = veDfltAccumulationEnabled;
#endif   
   ueCfg->isDeltaMCSEnabled = FALSE;
   /* warning fix */
   ueCfg->p0UePusch = (S8)veDfltP0UEPUSCH;
   ueCfg->p0UePucch = (S8)veDfltP0UEPUCCH;
   ueCfg->pSRSOffset = (U8)veDfltpSRSOffset;
   ueCfg->trgCqi = veDfltUeUlPwrTrgCqi;

   RETVALUE(ROK);
} /* veFillRgrUeUlPwrCfg */


/*
 *
 *      Fun:   veFillRgrUeQosCfg
 *
 *      Desc:  Populate RgrUeQosCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrUeQosCfg
(
RgrUeQosCfg *ueCfg
)
#else
PUBLIC S16 veFillRgrUeQosCfg(ueCfg)
RgrUeQosCfg *ueCfg;
#endif
{
   TRC2(veFillRgrUeQosCfg);

  if(NULLP == ueCfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrUeQosCfg: ueCfg is NULL.\n"));
      RETVALUE(RFAILED);
   } 

   ueCfg->ambrPres = FALSE;
   ueCfg->dlAmbr = VE_VAL_ZERO;
   ueCfg->ueBr = veDfltUeBr;

   RETVALUE(ROK);
} /* veFillRgrUeQosCfg */

/*
 *
 *      Fun:   veFillRgrUeTaTmrCfg
 *
 *      Desc:  Populate RgrUeTaTmrCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrUeTaTmrCfg
(
RgrUeTaTmrCfg *ueCfg
)
#else
PUBLIC S16 veFillRgrUeTaTmrCfg(ueCfg)
RgrUeTaTmrCfg *ueCfg;
#endif
{
   TRC2(veFillRgrUeTaTmrCfg);

   if(NULLP == ueCfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrUeTaTmrCfg: ueCfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   ueCfg->pres = veDfltTaTmrPres;
   ueCfg->taTmr = veDfltTaTmr; /* value does not matter */

   RETVALUE(ROK);
} /* veFillRgrUeTaTmrCfg */

/*
 *
 *      Fun:   veFillRgrUeDrxCfg
 *
 *      Desc:  Populate RgrUeDrxCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrUeDrxCfg
(
RgrUeDrxCfg *ueCfg
)
#else
PUBLIC S16 veFillRgrUeDrxCfg(ueCfg)
RgrUeDrxCfg *ueCfg;
#endif
{
   TRC2(veFillRgrUeDrxCfg);

   if(NULLP == ueCfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrUeDrxCfg: ueCfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   ueCfg->isDrxEnabled = FALSE;
   ueCfg->drxOnDurTmr = VE_VAL_ZERO;
   ueCfg->drxInactvTmr = VE_VAL_ZERO;
   ueCfg->drxRetxTmr = VE_VAL_ZERO;
   ueCfg->drxLongCycleOffst.longDrxCycle = VE_VAL_ZERO;
   ueCfg->drxLongCycleOffst.drxStartOffst = VE_VAL_ZERO;
   ueCfg->drxShortDrx.pres = VE_VAL_ZERO;
   ueCfg->drxShortDrx.shortDrxCycle = VE_VAL_ZERO;
   ueCfg->drxShortDrx.drxShortCycleTmr = VE_VAL_ZERO;

   RETVALUE(ROK);
} /* veFillRgrUeDrxCfg */


/*
 *
 *      Fun:   veFillRgrUeAckNackRepCfg
 *
 *      Desc:  Populate RgrUeAckNackRepCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrUeAckNackRepCfg
(
RgrUeAckNackRepCfg *ueCfg
)
#else
PUBLIC S16 veFillRgrUeAckNackRepCfg(ueCfg)
RgrUeAckNackRepCfg *ueCfg;
#endif
{
   TRC2(veFillRgrUeAckNackRepCfg);

   if(NULLP == ueCfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrUeAckNackRepCfg: ueCfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   ueCfg->isAckNackEnabled = FALSE;
   ueCfg->pucchAckNackRep = VE_VAL_ZERO;
   ueCfg->ackNackRepFactor = (RgrAckNackRepFactor)veDfltAckNackRepFactor;/* g++ compialtion fix */


   RETVALUE(ROK);
} /* veFillRgrUeAckNackRepCfg */

/*
 *
 *      Fun:   veFillRgrUeMeasGapCfg
 *
 *      Desc:  Populate RgrUeMeasGapCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrUeMeasGapCfg
(
RgrUeMeasGapCfg *ueCfg
)
#else
PUBLIC S16 veFillRgrUeMeasGapCfg(ueCfg)
RgrUeMeasGapCfg *ueCfg;
#endif
{
   TRC2(veFillRgrUeMeasGapCfg);

   if(NULLP == ueCfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrUeMeasGapCfg: ueCfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   ueCfg->isMesGapEnabled = FALSE;
   ueCfg->gapPrd = 40;
   ueCfg->gapOffst = 20;


   RETVALUE(ROK);
} /* veFillRgrUeMeasGapCfg */

/*
 *
 *      Fun:   veFillRgrUeCapCfg
 *
 *      Desc:  Populate RgrUeCapCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrUeCapCfg
(
RgrUeCapCfg *ueCfg
)
#else
PUBLIC S16 veFillRgrUeCapCfg(ueCfg)
RgrUeCapCfg *ueCfg;
#endif
{
   TRC2(veFillRgrUeCapCfg);

   if(NULLP == ueCfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrUeGapCfg: ueCfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

   ueCfg->pwrClass = VE_VAL_ZERO;
   ueCfg->intraSfFeqHop = VE_VAL_ZERO;
   ueCfg->resAloocType1 = VE_VAL_ZERO;
   ueCfg->simCqiAckNack = VE_VAL_ZERO;

   RETVALUE(ROK);
} /* veFillRgrUeCapCfg */

/*
 *
 *      Fun:   veFillRgrCodeBookRstCfg
 *
 *      Desc:  Populate RgrCodeBookRstCfg Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrCodeBookRstCfg
(
RgrCodeBookRstCfg *ueCfg
)
#else
PUBLIC S16 veFillRgrCodeBookRstCfg(ueCfg)
RgrCodeBookRstCfg *ueCfg;
#endif
{
   TRC2(veFillRgrCodeBookRstCfg);

   if(NULLP == ueCfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRgrCodeBookRstCfg: ueCfg is NULL.\n"));
      RETVALUE(RFAILED);
   }

/* MSPD changes */
#ifdef MSPD
   ueCfg->pres = veCodeBookRstnPres;
   ueCfg->pmiBitMap[0] = vePmiBitMap0;   
   ueCfg->pmiBitMap[1] = vePmiBitMap1;
#else
   ueCfg->pres = FALSE;
   ueCfg->pmiBitMap[0] = VE_VAL_ZERO;
   ueCfg->pmiBitMap[1] = VE_VAL_ZERO;
#endif

   RETVALUE(ROK);
} /* veFillRgrCodeBookRstCfg */


#ifdef LTE_TDD
/*
 *
 *      Fun:   veFillRgrTddAckNackMode
 *
 *      Desc:  Populate RgrTddAckNackMode Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRgrTddAckNackMode
(
RgrTddAckNackMode *mode
)
#else
PUBLIC S16 veFillRgrTddAckNackMode(mode)
RgrTddAckNackMode *mode;
#endif
{
   TRC2(veFillRgrTddAckNackMode);

   *mode = RGR_TDD_ACKNACK_MODE_BUNDL;

   RETVALUE(ROK);
} /* veFillRgrTddAckNackMode */
#endif /* LTE_TDD */

/*
 *
 *      Fun:   veSndRgrUeCfg
 *
 *      Desc:  Send Rgr UE Cfg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veSndRgrUeCfg
(
VeUeCb        *ueCb
)
#else
PUBLIC S16 veSndRgrUeCfg(ueCb)
VeUeCb        *ueCb;
#endif
{
   Pst *pst = NULLP;
   RgrCfgReqInfo *cfgInfo;
   RgrCfgTransId transId;
   RgrUeCfg *ueCfg = NULLP;
#ifdef LTE_HO_SUPPORT
   S16 retVal = ROK;
#endif

   TRC2(veSndRgrUeCfg);

   pst = &veCb.rgrSap[0]->pst;
   pst->event = EVTRGRCFGREQ;

   /* Initialize */
   VE_ALLOC(&cfgInfo, sizeof(RgrCfgReqInfo));
   if(cfgInfo == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeCfg mem alloc failed.\n"));
      RETVALUE(RFAILED);
   }

   VE_SET_ZERO(&transId, sizeof(RgrCfgTransId));

   /* Fill appropriate Trans details */
   VE_SET_RGR_TRANS(transId.trans, RGR_UE_CFG, VE_VAL_ZERO, ueCb->crnti);

   cfgInfo->action = RGR_CONFIG;
   cfgInfo->u.cfgInfo.cfgType = RGR_UE_CFG;
   ueCfg = &(cfgInfo->u.cfgInfo.u.ueCfg);

   ueCfg->cellId = ueCb->cellId;
   ueCfg->crnti = ueCb->crnti;

   /* UE Transmission mode */
   if(veFillRgrUeTxModeCfg(&ueCfg->txMode) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeCfg: veFillRgrUeTxModeCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* UE DL CQI config */
   if(veFillRgrUeDlCqiCfg(&ueCfg->ueDlCqiCfg, ueCb) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeCfg: veFillRgrUeDlCqiCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* UE related UL Harq Config */
   if(veFillRgrUeUlHqCfg(&ueCfg->ueUlHqCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeCfg: veFillRgrUeUlHqCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }


   /* UE UL power config */
   if(veFillRgrUeUlPwrCfg(&ueCfg->ueUlPwrCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeCfg: veFillRgrUeUlPwrCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* UE related Dl/UL QoS config: AMBR */
   if(veFillRgrUeQosCfg(&ueCfg->ueQosCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeCfg: veFillRgrUeQosCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* UE TA timer config */
   if(veFillRgrUeTaTmrCfg(&ueCfg->ueTaTmrCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeCfg: veFillRgrUeTaTmrCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* UE category */
   ueCfg->ueCatEnum = veDfltUECategory;

   /* UE specific DRX configuration */
#ifdef LTEMAC_DRX
   if(veFillRgrUeDrxCfg(&ueCfg->ueDrxCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeCfg: veFillRgrUeDrxCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }
#endif

   /* TtiBundling Enable/Disabled for UE */
   ueCfg->isTtiBundlEnabled = FALSE;
   
   /* Ack/Nack Configuration for UE */
   if(veFillRgrUeAckNackRepCfg(&ueCfg->ueAckNackCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeCfg: veFillRgrUeAckNackRepCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* measurement gap configuration for UE */
   if(veFillRgrUeMeasGapCfg(&ueCfg->ueMesGapCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeCfg: veFillRgrUeMeasGapCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* Ue Capabilty configuration */
   if(veFillRgrUeCapCfg(&ueCfg->ueCapCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeCfg: veFillRgrUeCapCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* Number of bits in codebook for Transmission modes */
   if(veFillRgrCodeBookRstCfg(&ueCfg->ueCodeBookRstCfg) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeCfg: veFillRgrCodeBookRstCfg\
               failed.\n"));
      RETVALUE(RFAILED);
   }

   /* If present then mapping exists at RGR user with crnti */
#ifdef LTE_HO_SUPPORT
   /* Check if the UE is being Handed Over or not */
   if (VE_HO_INITIATED ==  ueCb->mobCtrlState)
   {
      /* Allocate Dedicated preamble */
      retVal = veGetDedPrmbl(&ueCb->dedicatedPrmbl); 
      if (ROK != retVal)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeCfg:\
            Dedicated Preamble Allocation Failed : %d\n",__LINE__));
#endif
         RETVALUE(RFAILED);   
      }
      ueCfg->dedPreambleId.pres = PRSNT_NODEF;
      ueCfg->dedPreambleId.val = ueCb->dedicatedPrmbl;
   }
   else
   {
      ueCfg->dedPreambleId.pres = NOTPRSNT;
      ueCfg->dedPreambleId.val = NOTPRSNT;
   }
#else
   ueCfg->dedPreambleId.pres = NOTPRSNT;
   ueCfg->dedPreambleId.val = NOTPRSNT;
#endif /* End of LTE_HO_SUPPORT */
#ifdef LTE_TDD
   if(veFillRgrTddAckNackMode(&ueCfg->ackNackModeEnum) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeCfg: veFillRgrTddAckNackMode\
               failed.\n"));
      RETVALUE(RFAILED);
   }
#endif

/* MS_FIX: tfu upgrade changes */
#ifdef TFU_UPGRADE
   /* Configure SRS information */
   ueCfg->srsCfg.type = 1; /* Release = 0, Setup = 1 */
   ueCfg->srsCfg.srsSetup.srsCfgIdx = 0;
   ueCfg->srsCfg.srsSetup.srsBw = RGR_ULSRS_BW_0;
   ueCfg->srsCfg.srsSetup.srsHopBw = RGR_ULSRS_HOP_BW_0;
   ueCfg->srsCfg.srsSetup.cycShift = RGR_ULSRS_CYSHIFT_0;
   ueCfg->srsCfg.srsSetup.duration = 0;
   ueCfg->srsCfg.srsSetup.sANSrs = 0;
/* ueCfg->srsCfg.srsSetup.sANSrs Duplicate data member */
   ueCfg->srsCfg.srsSetup.txComb = 0;
   ueCfg->srsCfg.srsSetup.fDomPosi = 0;

   /* Configure SR information */
   ueCfg->srCfg.type = CTF_IE_CFG_SETUP;
   ueCfg->srCfg.srSetup.srResIdx = veDfltSrPUCCHResIdx;
   ueCfg->srCfg.srSetup.srCfgIdx = ueCb->srCfgIdx;
   ueCfg->srCfg.srSetup.dTMax = RGR_DSR_TXMAX_4;

   /* PDSCH related dedicated configuration */
   ueCfg->uePdschDedCfg.uepACfg.pA = RGRUE_DLPWRCNTRL_PA_DB_6;
   ueCfg->uePdschDedCfg.uepACfg.pAPrsnt = 0;
   ueCfg->puschDedCfg.bCQIIdx = veDfltBetaOffCQIIdx;
   ueCfg->puschDedCfg.pres = PRSNT_NODEF;
#endif /* TFU_UPGRADE */

   if(ROK != VeLiRgrCfgReq(pst, veCb.rgrSap[0]->spId, transId, cfgInfo))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgReq failed.\n"));      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* veSndRgrUeCfg */

/* NAS NON Delivery Indication start */


/*
*
*      Fun:   veInsrtNasPduToLst
*
*      Desc:  Insert the NAS PDU into Hash List present in CellCb 
*             and also into Linked list present in UeCb.
*
*      Ret:   ROK
*
*      Notes: None
*
*      File:  ve_li_hdl.c
*
*/
#ifdef ANSI
PUBLIC S16 veInsrtNasPduToLst
(
VeUeCb      *ueCb,
TknStrOSXL  *nasPdu,
U32         transId
)
#else
PUBLIC S16 veInsrtNasPduToLst(ueCb, nasPdu, transId)
VeUeCb      *ueCb;
TknStrOSXL  *nasPdu;
U32         transId;
#endif
{
   CmLListCp      *lCp = NULLP;
   VeNasInfo      *tempNasPduInfo = NULLP;
   VeCellCb       *cellCb = NULLP;
   S16            ret;

   TRC2(veRemoveNasPduFrmLst);

   /*Create NAS PDU info control block*/
   VE_ALLOC(&tempNasPduInfo,            sizeof(VeNasInfo));
   if(tempNasPduInfo == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veInsrtNasPduToLst:\
               Failure in the allocation of configuration info\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */


   VE_ALLOC(&(tempNasPduInfo->asn),     sizeof(TknStrOSXL));
   if(tempNasPduInfo->asn == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veInsrtNasPduToLst:\
               Failure in the allocation of configuration info\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */


   VE_ALLOC(&(tempNasPduInfo->asn->val), nasPdu->len);
   if(tempNasPduInfo->asn->val == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veInsrtNasPduToLst:\
               Failure in the allocation of configuration info\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */



   tempNasPduInfo->crnti       = ueCb->crnti;
   tempNasPduInfo->transId     = transId;

   cmMemcpy((U8*)tempNasPduInfo->asn->val, (U8*)nasPdu->val, nasPdu->len);
   tempNasPduInfo->asn->pres    = nasPdu->pres;
   tempNasPduInfo->asn->spare1  = nasPdu->spare1;
   tempNasPduInfo->asn->len     = nasPdu->len;
#ifdef ALIGN_64BIT
   tempNasPduInfo->asn->spare2  = nasPdu->spare2; 
#endif
   tempNasPduInfo->nasPdu.node = (PTR)tempNasPduInfo;


   lCp = &(ueCb->vePduLLst);
   cmLListAdd2Tail(lCp, &(tempNasPduInfo->nasPdu));  

   cellCb = veCb.cellCb[0];
   ret = cmHashListInsert(&(cellCb->vePduHashLst), (PTR)tempNasPduInfo, (U8 *)&transId, 
         sizeof (U32));
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veInsrtNasPduToLst:\
               Failure while insrting into HashList (cellCb.vePduHashLst)\n"));
#endif
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


/*
*
*      Fun:   veRemoveNasPduFrmLst
*
*      Desc:  Remove the NAS PDU from Hash List prsent in CellCb 
*             and also from Linked list present in UeCb.
*
*      Ret:   ROK
*
*      Notes: None
*
*      File:  ve_li_hdl.c
*
*/
#ifdef ANSI
PUBLIC S16 veRemoveNasPduFrmLst
(
VeUeCb *ueCb,
U32    transId
)
#else
PUBLIC S16 veRemoveNasPduFrmLst(ueCb, transId)
VeUeCb *ueCb;
U32    transId;
#endif
{
   CmLListCp     *l;                    /* linked list control point */
   CmLList       *n;                    /* linked list */
   VeNasInfo     *llNasPdu = NULLP;
   VeNasInfo     *hlNasPdu = NULLP;
   VeCellCb       *cellCb = NULLP;
   TRC2(veRemoveNasPduFrmLst);


   l = &(ueCb->vePduLLst);
   n = cmLListFirst(l);
   while ( n != NULLP )
   {
      llNasPdu = (VeNasInfo *) n->node;

      if (llNasPdu->transId ==  transId)
      {
         n = cmLListDelFrm( l, n );
         break;
      }

      n = cmLListNext(l);
   }

   cellCb = veCb.cellCb[0];

   cmHashListFind(&(cellCb->vePduHashLst), (U8 *)&transId, sizeof (U32), 0, (PTR *) &hlNasPdu);

   if (!(hlNasPdu == NULLP))
   {
      cmHashListDelete(&(cellCb->vePduHashLst), (PTR) hlNasPdu);
      VE_FREE(hlNasPdu, sizeof(VeNasInfo));    
   } 
   else if((hlNasPdu == NULLP) && (llNasPdu != NULLP))
   {
      VE_FREE(llNasPdu, sizeof(VeNasInfo));
   }

   RETVALUE(ROK);
} 


/*
*
*      Fun:   veDelAllNasPduFrmLst
*
*      Desc:  Remove All NAS PDU from Hash List prsent in CellCb 
*             and also from Linked list present in UeCb.
*
*      Ret:   ROK
*
*      Notes: None
*
*      File:  ve_li_hdl.c
*
*/
#ifdef ANSI
PUBLIC S16 veDelAllNasPduFrmLst
(
VeUeCb *ueCb
)
#else
PUBLIC S16 veDelAllNasPduFrmLst(ueCb)
VeUeCb *ueCb;
#endif
{
   CmLListCp     *l;                    /* linked list control point */
   CmLList       *n;                    /* linked list */
   VeNasInfo     *TempNasPdu;
   VeCellCb       *cellCb = NULLP;
   TRC2(veDelAllNasPduFrmLst);


   l = &(ueCb->vePduLLst); 
   cellCb = veCb.cellCb[0];
   n = cmLListFirst(l);
   while ( n != NULLP )
   {
      TempNasPdu = (VeNasInfo *) n->node;

      cmHashListFind(&(cellCb->vePduHashLst), (U8 *)&(TempNasPdu->transId), sizeof (U32), 0, (PTR *) &TempNasPdu);
      if (TempNasPdu == NULLP)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veDelAllNasPduFrmLst:\
                  Failure while deleting into HashList (cellCb.vePduHashLst)\n"));
#endif
         RETVALUE(RFAILED);
      }
      else
      {
         cmHashListDelete(&(cellCb->vePduHashLst), (PTR) TempNasPdu);
         n = cmLListDelFrm( l, n );
         VE_FREE(TempNasPdu, sizeof(VeNasInfo));
      }

      n = cmLListNext(l);
   }
   
   RETVALUE(ROK);
}

/* NAS NON Delivery Indication end*/   


/* ADD CRID:ccpu00118776  UeCancel Request*/   
/*
*
*       Fun:  veSendUeCnclReq 
*
*       Desc:  Sending the Ue Cancel Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c  
*
*/
#ifdef ANSI
PUBLIC S16 veSendUeCnclReq 
(
VeUeCb *ueCb
)
#else
PUBLIC S16 veSendUeCnclReq(ueCb)
VeUeCb *ueCb;
#endif /* ANSI */
{

   Pst pst;
   U8  ueIdx;   /* Added idx */

   TRC3(veSendUeCnclReq)

   VE_SET_ZERO(&pst, sizeof(Pst));
   /* ADD CRID:ccpu00118776 Stopping of the  Con Procedure timer*/
   veUeConStopTmr((PTR)ueCb, VE_TMR_RRC_CON);
   /*Local clearing of lower layer configuration*/
   /* 1. Sending Ctf Ue Rls request.
    * 2. Sending Rgr Ue Rls request.
    * 3. Sending Cancel Ue request to RRC.*/
   {
      veSndCtfUeRls(ueCb);
      veSndRgrUeRls(ueCb, NULLP, RGR_UE_CFG);
      VeLiNhuCnclUeReq(&(veCb.nhuSap[0]->pst), veCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);

      ueIdx = (U8)VE_GET_UE_IDX(ueCb->crnti);
      if(ueIdx < veCb.cellCb[0]->maxUeSupp)
      {
         veCb.cellCb[0]->ueCbLst[ueIdx] = NULLP;
      }
      VE_FREE(ueCb,sizeof(VeUeCb));

   }
   RETVALUE(ROK);

} /* end of veSendUeCnclReq*/

/* RRC-ReEstab */

/*
 *
 *      Fun:   veLIMRrcConReestabCmpHndlr
 *
 *      Desc:  RRC Connection Re-establishment complete
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File:  ve_li_hdl.c
 *
 */
#ifdef ANSI
PUBLIC S16 veLIMRrcConReestabCmpHndlr
(
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 veLIMRrcConReestabCmpHndlr(datIndSdu)
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
  
  
  VeCellCb  *cellCb = NULLP;
  VeUeCb *ueCb = NULLP;
  U8      ueIdx;
  TRC2(veLIMRrcConReestabCmpHndlr);

#ifdef VE_PICO

   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "UE -> ENB : RRC Connection Re-establishment Complete ");
   ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
#endif

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " RRC Connection Re-establishment Complete Message Rcvd.\
            from UeId:%x\n", datIndSdu->hdr.ueId));
#endif
 
   cellCb = veCb.cellCb[0];
   ueIdx = (U8)VE_GET_UE_IDX(datIndSdu->hdr.ueId);
   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];

   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
       "\nveLIMRrcReestabHndlr(): Invalid ueIdx(%d)\n",ueIdx));
      RETVALUE(RFAILED);

   }
  /*spandey*/
  /*Calling reconfiguation request after rrc-reestablishment*/
   
   switch(ueCb->reEstCause)
   {
       case ICS_OTHER_FAIL_CSFALBK_SET:
       {
           veSndRrcRecfgReq(ueCb, Sztid_InitCntxtSetup);
           break;
       }
       case ERAB_REQ_FAIL_ANY_CAUSE:
       {
           veSndRrcReCfgReqAftrReEst(ueCb);
           break;
       }
       case ERAB_REL_FAIL_ANY_CAUSE:
       {
           veSndRrcRecfgReq(ueCb, Sztid_E_RABRls);
           break;
       }
       case RLF_CAUSE:
       {
           veSndRrcReCfgReqAftrReEst(ueCb);
           break;
       }
    }

   RETVALUE(ROK);
} /* veLIMRrcConReestabCmpHndlr */

/* RRC ReEst */
#ifdef ANSI
PUBLIC S16 veSndRgrUeIdReCfg
(
VeUeCb *ueCb,
U8 oldCrnti
)
#else
PUBLIC S16 veSndRgrUeIdReCfg(ueCb,oldCrnti)
VeUeCb *ueCb;
U8     oldCrnti;
#endif
{
   Pst *pst = NULLP;
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   RgrCfgTransId transId;

   TRC2(veSndRgrUeIdReCfg);

   pst = &veCb.rgrSap[0]->pst;
   pst->event = EVTRGRCFGREQ;

   /* Initialize */
   VE_ALLOC(&cfgReq, sizeof(RgrCfgReqInfo));

   if(cfgReq == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeIdReCfg mem alloc failed.\n"));
      RETVALUE(RFAILED);
   }

   VE_SET_ZERO(&transId, sizeof(RgrCfgTransId));

   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
    cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = ueCb->cellId;
   ueRecfg->oldCrnti = oldCrnti;
   ueRecfg->newCrnti = ueCb->crnti;

   ueRecfg->ueUlHqRecfg.maxUlHqTx = veDfltMaxUlHqTx;
   #ifdef TFU_UPGRADE
   ueRecfg->puschDedCfg.bACKIdx = veDfltBetaOffACKIdx;
   #endif

   /* Fill appropriate Trans details */
   VE_SET_RGR_TRANS(transId.trans, RGR_UE_CFG, VE_VAL_ZERO, ueCb->crnti);
   /* Rgr Cfg Req primitive */
   if(ROK != VeLiRgrCfgReq(pst, veCb.rgrSap[0]->spId, transId, cfgReq))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRgrUeIdReCfg failed.\n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/*veSndRgrUeIdReCfg*/ 
/* ReEst Fix Start */ 
#ifdef ANSI
PRIVATE S16 veSndCtfUeReCfg
(
VeUeCb        *ueCb,
CmLteRnti     oldCrnti
)
#else
PRIVATE S16 veSndCtfUeReCfg(ueCb, oldCrnti)
VeUeCb        *ueCb;
CmLteRnti     oldCrnti;
#endif
{
   Pst *pst = NULLP;
   CtfCfgReqInfo *reCfgInfo;
   CtfCfgTransId transId;

   CtfDedRecfgInfo *ueCfgParam = NULLP;

   TRC2(veSndCtfUeCfg);

   pst = &veCb.ctfSap[0]->pst;
   pst->event = EVTCTFCFGREQ;

   /* Initialize */
   VE_SET_ZERO(&transId, sizeof(CtfCfgTransId));

   VE_ALLOC(&reCfgInfo, sizeof(CtfCfgReqInfo));

   if(reCfgInfo == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndCtfUeReCfg mem alloc failed.\n"));      
      RETVALUE(RFAILED);
   }


#ifdef CTF_VER3
   reCfgInfo->vendorParams.paramBuffer = NULLP;
   reCfgInfo->vendorParams.buffLen = 0;
#endif
   reCfgInfo->cfgType = CTF_RECONFIG;
   reCfgInfo->u.reCfg.cfgElem = CTF_UE_CFG;

   ueCfgParam = &reCfgInfo->u.reCfg.u.dedRecfg;

   if(ueCb == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndCtfUeReCfg Invalid UeCb\n"));      
      RETVALUE(RFAILED);
   }
   else
   {
      ueCfgParam->cellId = ueCb->cellId;
      ueCfgParam->ueId = oldCrnti;
#ifdef CTF_VER3
      ueCfgParam->newUeId = ueCb->crnti;
#endif
   }
   if(ROK != VeLiCtfCfgReq(pst, veCb.ctfSap[0]->spId, transId, reCfgInfo))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCtfCfgReq failed.\n"));      
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/********************************************************************30**

         End of file:     ve_li_hdl.c@@/main/2 - Wed Dec  1 08:44:26 2010

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

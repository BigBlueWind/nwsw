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

     Name:     LTE-S1AP User
  
     Type:     C Source file
  
     Desc:     Build configuration and message

     Ret :     ROK - success
               RFAILED - failure

     File:     ve_s1_bldmsg.c

     Sid:      ve_s1_bldmsg.c@@/main/eNB_1.3_With_LatestGpr3/14 - Fri Sep 16 17:30:01 2011

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
#include "cm_inet.h"
#include "cm_tpt.h"

#include "nhu_asn.h"
#include "nhu.h"           /* NHU interface defines */
#include "pju.h"           /* PJU interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"           /* PHY RRM control Interface */
#include "czt.h"           /* X2AP RRM control Interface */
#include "szt.h"           /* S1AP RRM control Interface */
#ifdef EU_DAT_APP
#include "egt.h"           /* S1AP RRM control Interface */
#endif 
#include "lrg.h"           /* MAC RRM control Interface */
#include "sz.h"


/* RRM related includes */
#include "ve.h" 
#include "lve.h"
#ifdef LTE_ENB_PERF
#include "ve_perf.h"
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
#include "cm_inet.x"
#include "cm_tpt.x"

#include "nhu_asn.x"
#include "nhu.x"           /* NHU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "szt.x"           /* S1AP RRM control Interface */
#include "ctf.x"           /* PHY RRM control Interface */
#ifndef MSPD
#include "ve_dflt.h"
#else
#include "ve_dflt_ms.x"
#endif
#include "czt.x"           /* X2AP RRM control Interface */
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "lrg.x"           /* MAC RRM control Interface */

/* RRM related includes */
#include "lve.x" 
#include "ve_eut.x" 
#include "ve.x" 

#include "szt_asn.x"

#ifdef LTE_HO_SUPPORT

#include "sz.h"
#include "cm_pasn.h"
#include "cm_pasn.x"

EXTERN CmPAsnElmDef szTgeteNB_ToSrceNB_TprntCont;
EXTERN CmPAsnElmDef szSrceNB_ToTgeteNB_TprntCont;
#endif
/* ======================================= */
/*S1AP S1 SETUP REQUEST MESSAGE BUILDING*/
/* ======================================= */
/*
*
*       Fun:   veUtlSztFillS1SetupReq
*
*       Desc:  Construct S1 Setup Message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c  
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillS1SetupReq
(
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillS1SetupReq(pdu)
S1apPdu      **pdu;
#endif
{
   S1apPdu *szS1SetupReqPdu = NULLP;
   U16     numComp;
   S16     ret;
   U16     len;
   U8      enbName[10];
   U8      pLMNId[10];
   VeCellCb *cellCb = NULLP;
   SztProtIE_Field_S1SetupRqstIEs *ie;
   SztSuppTAs_Item *tACItem;

   TRC2(veUtlSztFillS1SetupReq);
   cellCb = veCb.cellCb[0];
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlSztFillS1SetupReq:\
            Constructing S1 Setup Message\n"));
#endif

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem, 
         (Ptr *)&szS1SetupReqPdu);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillS1SetupReq:\
               cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   /*veFillTkn functions will be referred from sz_utl.c file*/
   veFillTknU8(&(szS1SetupReqPdu->pdu.choice), SZT_TRGR_INTI_MSG);
   veFillTknU8(&(szS1SetupReqPdu->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   veFillTknU32(&(szS1SetupReqPdu->pdu.val.initiatingMsg.procedureCode),
         Sztid_S1Setup);
   veFillTknU32(&(szS1SetupReqPdu->pdu.val.initiatingMsg.criticality),
         SztCriticalityrejectEnum);

   veFillTknU8(&(szS1SetupReqPdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.\
            pres),PRSNT_NODEF);

   /* Filling 3 mandatory IEs */
   numComp = 4;
   veFillTknU16(&(szS1SetupReqPdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.\
            protocolIEs.noComp),numComp);

   /* Get memory for numComp IEs */
   if ((cmGetMem(szS1SetupReqPdu, numComp * sizeof(SztProtIE_Field_S1SetupRqstIEs),\
               (Ptr*)&szS1SetupReqPdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.\
               protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillS1SetupReq:\
               cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(szS1SetupReqPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling ENB Global Id */
   ie = &szS1SetupReqPdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.\
        protocolIEs.member[0];

   /* Global ENB Id */
   veFillTknU8(&(ie->pres),PRSNT_NODEF);
   veFillTknU32(&(ie->id),Sztid_Global_ENB_ID);
   veFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   veFillTknU8(&(ie->value.u.sztGlobal_ENB_ID.pres), PRSNT_NODEF);
   switch (veCb.eNBType)
   {
      case ENB_ID_MACRO_ENB_ID:
         {
            /* MACRO ENB */
            len = 20;
            /* Fill eNB id */
            veFillTknU8(&(ie->value.u.sztGlobal_ENB_ID.eNB_ID.choice), ENB_ID_MACROENB_ID);
            ie->value.u.sztGlobal_ENB_ID.eNB_ID.val.macroENB_ID.pres = PRSNT_NODEF;
            veFillEnbId(&(ie->value.u.sztGlobal_ENB_ID.eNB_ID.val.macroENB_ID), len, cellCb->cellId);

       /* PLMN Identity */
       VE_GET_MEM(szS1SetupReqPdu, len, &(ie->value.u.sztGlobal_ENB_ID.\
                pLMNidentity.val));
       ie->value.u.sztGlobal_ENB_ID.pLMNidentity.pres = PRSNT_NODEF;
       ie->value.u.sztGlobal_ENB_ID.pLMNidentity.len = len = 3;

       /* CR ID- ccpu00116764-  IOT changes*/
       cmMemset(pLMNId,0,sizeof(pLMNId));
       /* MNC digits CRID:ccpu00118127*/
       if(cellCb->sysRelInfo.mnc[2].pres == PRSNT_NODEF )
       {   
          pLMNId[0] =(((cellCb->sysRelInfo.mcc[1].val)<<4)| (cellCb->sysRelInfo.mcc[0].val));
          pLMNId[1] =(((cellCb->sysRelInfo.mnc[0].val)<<4)| (cellCb->sysRelInfo.mcc[2].val));
          pLMNId[2] =(((cellCb->sysRelInfo.mnc[2].val)<<4)| (cellCb->sysRelInfo.mnc[1].val));
       }
       else
       {
          /*Addition of Filler digit as 0x0f plus two digits of MNC*/
          pLMNId[0] =(((cellCb->sysRelInfo.mcc[1].val)<<4)| (cellCb->sysRelInfo.mcc[0].val));
          pLMNId[1] =(((0x0f)<<4)| (cellCb->sysRelInfo.mcc[2].val));
          pLMNId[2] =(((cellCb->sysRelInfo.mnc[1].val)<<4)| (cellCb->sysRelInfo.mnc[0].val));

       }
       veFillTknStrOSXL(&(ie->value.u.sztGlobal_ENB_ID.pLMNidentity),len, pLMNId, &szS1SetupReqPdu);
       ie->value.u.sztGlobal_ENB_ID.iE_Extns.noComp.pres = NOTPRSNT;
         }
         break;
      case ENB_ID_HOME_ENB_ID:
         {
            /* Home ENB Id  */
            veFillTknU8(&(ie->value.u.sztGlobal_ENB_ID.eNB_ID.choice), ENB_ID_HOMEENB_ID);
       ie->value.u.sztGlobal_ENB_ID.eNB_ID.val.homeENB_ID.pres = PRSNT_NODEF;
       ie->value.u.sztGlobal_ENB_ID.eNB_ID.val.homeENB_ID.len = 28;
       veFillTknBStr32(&(ie->value.u.sztGlobal_ENB_ID.eNB_ID.val.homeENB_ID), 28,100);  
            ie->value.u.sztGlobal_ENB_ID.iE_Extns.noComp.pres = NOTPRSNT;

            /* GLOBAL_ENB Identity  from System Related Information*/
            cmMemset(pLMNId,0,sizeof(pLMNId));
            len = 3;
       if(cellCb->sysRelInfo.mnc[2].pres == PRSNT_NODEF )
       {   
          pLMNId[0] =(((cellCb->sysRelInfo.mcc[1].val)<<4)| (cellCb->sysRelInfo.mcc[0].val));
          pLMNId[1] =(((cellCb->sysRelInfo.mnc[0].val)<<4)| (cellCb->sysRelInfo.mcc[2].val));
          pLMNId[2] =(((cellCb->sysRelInfo.mnc[2].val)<<4)| (cellCb->sysRelInfo.mnc[1].val));
       }
       else
       {
          /*Addition of Filler digit as 0x0f plus two digits of MNC*/
          pLMNId[0] =(((cellCb->sysRelInfo.mcc[1].val)<<4)| (cellCb->sysRelInfo.mcc[0].val));
          pLMNId[1] =(((0x0f)<<4)| (cellCb->sysRelInfo.mcc[2].val));
          pLMNId[2] =(((cellCb->sysRelInfo.mnc[1].val)<<4)| (cellCb->sysRelInfo.mnc[0].val));
       }
       /* PLMN Identity */
       VE_GET_MEM(szS1SetupReqPdu, len, &(ie->value.u.sztGlobal_ENB_ID.\
                pLMNidentity.val));
       ie->value.u.sztGlobal_ENB_ID.pLMNidentity.pres = PRSNT_NODEF;
       ie->value.u.sztGlobal_ENB_ID.pLMNidentity.len = len;
       veFillTknStrOSXL(&(ie->value.u.sztGlobal_ENB_ID.pLMNidentity),len, pLMNId, &szS1SetupReqPdu);
       ie->value.u.sztGlobal_ENB_ID.iE_Extns.noComp.pres = NOTPRSNT;
    }
         break;
      default:
         {
            /* MACRO ENB */
            len = 20;
            /* Fill eNB id */
            veFillTknU8(&(ie->value.u.sztGlobal_ENB_ID.eNB_ID.choice), ENB_ID_MACROENB_ID);
            ie->value.u.sztGlobal_ENB_ID.eNB_ID.val.macroENB_ID.pres = PRSNT_NODEF;
            veFillEnbId(&(ie->value.u.sztGlobal_ENB_ID.eNB_ID.val.macroENB_ID), len, cellCb->cellId);
            ie->value.u.sztGlobal_ENB_ID.iE_Extns.noComp.pres = NOTPRSNT;
         }
         break;
   }

   /* IE2 - Filling ENB Name */
   ie = &szS1SetupReqPdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.\
        protocolIEs.member[1];

   veFillTknU8(&(ie->pres),PRSNT_NODEF);
   veFillTknU32(&(ie->id),Sztid_eNBname);
   veFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);

   VE_GET_MEM(szS1SetupReqPdu, len, &(ie->value.u.sztENBname.val));
   /* ENB Name */
   ie->value.u.sztENBname.pres = PRSNT_NODEF;
   ie->value.u.sztENBname.len = 3;

   /* CR ID- ccpu00116764-  IOT changes*/
   cmMemset(enbName,0,sizeof(enbName));
   strcpy((S8 *)enbName,"EUTRAN");
   veFillTknStrOSXL(&(ie->value.u.sztENBname),3,enbName,&szS1SetupReqPdu);

   /* IE3 - Filling Supported TAs */
   ie = &szS1SetupReqPdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.\
        protocolIEs.member[2];

   veFillTknU8(&(ie->pres),PRSNT_NODEF);
   veFillTknU32(&(ie->id),Sztid_SuppTAs);
   veFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);

   numComp = 1;
   veFillTknU16(&(ie->value.u.sztSuppTAs.noComp),numComp);

   /* IE3 - Allocate memory for TA items */
   if ((cmGetMem(szS1SetupReqPdu, numComp*sizeof(SztSuppTAs_Item), 
               (Ptr*)&(ie->value.u.sztSuppTAs.member))) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillS1SetupReq:\
               cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(szS1SetupReqPdu);
      RETVALUE(RFAILED);
   }

   /* IE3 - Filling TA item 1 */
   tACItem = &ie->value.u.sztSuppTAs.member[0];
   veFillTknU8(&(tACItem->pres),PRSNT_NODEF);

   /* IE3 - Filling tAC */
   len = 2;
   /* TAC  */
   tACItem->tAC.pres = PRSNT_NODEF;
   tACItem->tAC.len = (U8)len;

   /* CR ID- ccpu00116764-  IOT changes*/
   /*TAC values are updated from System Related Information*/
   tACItem->tAC.val[1] = (U8)(cellCb->sysRelInfo.trackAreaCode & 0xff);
   tACItem->tAC.val[0] = (U8)((cellCb->sysRelInfo.trackAreaCode & 0xff00)>>8);

   /* IE3 - Filling broadcastPLMNs */
   numComp = 1;
   /* Allocate memory for broadcast PLMNs */
   if((cmGetMem(szS1SetupReqPdu, numComp * sizeof(SztPLMNidentity), \
               (Ptr*)&(tACItem->broadcastPLMNs.member))) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillS1SetupReq:\
               cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(szS1SetupReqPdu);
      RETVALUE(RFAILED);
   }

   veFillTknU16(&(tACItem->broadcastPLMNs.noComp),numComp);

   len = 3;
   /* Broadcast PLMN */
   tACItem->broadcastPLMNs.member[0].pres = PRSNT_NODEF;
   tACItem->broadcastPLMNs.member[0].len = len;
   VE_GET_MEM(szS1SetupReqPdu, len, &(tACItem->broadcastPLMNs.member[0].val));

   /* CR ID- ccpu00116764-  IOT changes*/
   cmMemset(pLMNId,0,sizeof(pLMNId));
/*  MNC digits CRID:ccpu00118127*/
   if(cellCb->sysRelInfo.mnc[2].pres == PRSNT_NODEF )
   {   
      pLMNId[0] =(((cellCb->sysRelInfo.mcc[1].val)<<4)| (cellCb->sysRelInfo.mcc[0].val));
      pLMNId[1] =(((cellCb->sysRelInfo.mnc[0].val)<<4)| (cellCb->sysRelInfo.mcc[2].val));
      pLMNId[2] =(((cellCb->sysRelInfo.mnc[2].val)<<4)| (cellCb->sysRelInfo.mnc[1].val));
   }
   else
   {
      /*Addition of Filler digit as 0x0f plus two digits of MNC*/
      pLMNId[0] =(((cellCb->sysRelInfo.mcc[1].val)<<4)| (cellCb->sysRelInfo.mcc[0].val));
      pLMNId[1] =(((0x0f)<<4)| (cellCb->sysRelInfo.mcc[2].val));
      pLMNId[2] =(((cellCb->sysRelInfo.mnc[1].val)<<4)| (cellCb->sysRelInfo.mnc[0].val));

   }
   veFillTknStrOSXL(&(tACItem->broadcastPLMNs.member[0]), len, pLMNId, &szS1SetupReqPdu);
   /*Filling the PagDRX */
    ie = &szS1SetupReqPdu->pdu.val.initiatingMsg.value.u.sztS1SetupRqst.protocolIEs.member[3];
   
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_DefaultPagDRX);
   
   /* CR ID- ccpu00116764- IOT changes*/
   veFillTknU32(&(ie->value.u.sztPagDRX),SztPagDRXv32Enum);
   veFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);

   *pdu = szS1SetupReqPdu;

   RETVALUE(ROK);

} /* veUtlSztFillS1SetupReq */


/*
*
*       Fun:   veUtlSztFillInitUeMsg 
*
*       Desc:  S1AP INITIAL UE MESSAGE BUILDING 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c  
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillInitUeMsg
(
VeUeCb        *ueCb,
NhuDatIndSdus *datIndSdu,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillInitUeMsg(ueCb, datIndSdu, pdu)
VeUeCb        *ueCb;
NhuDatIndSdus *datIndSdu;
S1apPdu      **pdu;
#endif
{ 
   S1apPdu *szInitUeMsgPdu = NULLP;
   U16     numComp;
   U32     enbId;
   U8      pLMNId[10];
   U16     len;
   S16     ret;
   SztProtIE_Field_InitUEMsg_IEs *ie;
   NhuDedicatedInfoNAS *nasPdu=NULLP;
   VeCellCb *cellCb=NULLP;
   U16  crntIe = 0;
   U8 *cptr = NULLP;
   TknBStr32  *tptr;
   U32 tmp;
   U8 *p;

   TRC2(veUtlSztFillInitUeMsg);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlSztFillInitUeMsg: Constructing\
            Initial UE Message\n"));
#endif

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem, (Ptr *)&szInitUeMsgPdu);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillInitUeMsg:cmAllocEvnt Failed \n"));
#endif
      RETVALUE(RFAILED);
   }

   veFillTknU8(&(szInitUeMsgPdu->pdu.choice), SZT_TRGR_INTI_MSG);
   veFillTknU8(&(szInitUeMsgPdu->pdu.val.initiatingMsg.pres),
         PRSNT_NODEF);
   veFillTknU32(&(szInitUeMsgPdu->pdu.val.initiatingMsg.procedureCode),
         Sztid_initialUEMsg);
   veFillTknU32(&(szInitUeMsgPdu->pdu.val.initiatingMsg.criticality),
         SztCriticalityignoreEnum);

   /* Filling of 3 mandatory IEs */
   veFillTknU8(&(szInitUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.pres),\
         PRSNT_NODEF);

   numComp = 5;
  if(ueCb->sTMSI.pres == TRUE)
  {
    numComp++;
  }
   veFillTknU16(&(szInitUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.\
            protocolIEs.noComp),numComp);

   /* Allocate memory for 3 IEs */
   if ((cmGetMem(szInitUeMsgPdu, numComp*sizeof(SztProtIE_Field_InitUEMsg_IEs),\
               (Ptr*)&szInitUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.\
               protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillInitUeMsg: cmGetMem Failed \n"));
#endif
      VE_FREE_EVNT(szInitUeMsgPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling enb s1ap id */
   ie = &szInitUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.protocolIEs\
        .member[crntIe++];

   enbId = 1; 
   veFillTknU8(&(ie->pres),PRSNT_NODEF);     
   veFillTknU32(&(ie->id),Sztid_eNB_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID),enbId);

   /* IE2 - Filling nas pdu */
   ie = &szInitUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.protocolIEs\
        .member[crntIe++];

   veFillTknU8(&(ie->pres),PRSNT_NODEF);     
   veFillTknU32(&(ie->id),Sztid_NAS_PDU);
   veFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);

   len = datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.rrcConSetupCompl.\
         criticalExtns.val.c1.val.rrcConSetupCompl_r8.dedicatedInfoNAS.len;
   nasPdu = &datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.rrcConSetupCompl.\
            criticalExtns.val.c1.val.rrcConSetupCompl_r8.dedicatedInfoNAS;

   veFillTknStrOSXL(&(ie->value.u.sztNAS_PDU), len, nasPdu->val,&szInitUeMsgPdu);

   /* IE3 - Filling TAI */
   ie = &szInitUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.protocolIEs\
        .member[crntIe++];

   veFillTknU8(&(ie->pres),PRSNT_NODEF);     
   veFillTknU32(&(ie->id),Sztid_TAI);
   veFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   veFillTknU8(&(ie->value.u.sztTAI.pres),PRSNT_NODEF);     

   /* CR ID- ccpu00116764-  IOT changes*/
   cellCb = veCb.cellCb[0]; 
/* MNC digits CRID:ccpu00118127*/
  if(cellCb->sysRelInfo.mnc[2].pres == PRSNT_NODEF)
   {   
      pLMNId[0] =(((cellCb->sysRelInfo.mcc[1].val)<<4)| (cellCb->sysRelInfo.mcc[0].val));
      pLMNId[1] =(((cellCb->sysRelInfo.mnc[0].val)<<4)| (cellCb->sysRelInfo.mcc[2].val));
      pLMNId[2] =(((cellCb->sysRelInfo.mnc[2].val)<<4)| (cellCb->sysRelInfo.mnc[1].val));
   }
   else
   {
      /*Addition of Filler digit as 0x0f plus two digits of MNC*/
      pLMNId[0] =(((cellCb->sysRelInfo.mcc[1].val)<<4)| (cellCb->sysRelInfo.mcc[0].val));
      pLMNId[1] =(((0x0f)<<4)| (cellCb->sysRelInfo.mcc[2].val));
      pLMNId[2] =(((cellCb->sysRelInfo.mnc[1].val)<<4)| (cellCb->sysRelInfo.mnc[0].val));
   }
   len = 3;
   veFillTknStrOSXL(&(ie->value.u.sztTAI.pLMNidentity), len, pLMNId,\
         &szInitUeMsgPdu);     

   len = 2;
   /* CR ID- ccpu00116764-  IOT changes*/
   veFillTknU8((TknU8 *)&(ie->value.u.sztTAI.tAC.pres),PRSNT_NODEF);
   ie->value.u.sztTAI.tAC.len = (U8)len;
   ie->value.u.sztTAI.tAC.val[1] = (U8)(cellCb->sysRelInfo.trackAreaCode & 0xff);
   ie->value.u.sztTAI.tAC.val[0] = (U8)((cellCb->sysRelInfo.trackAreaCode & 0xff00)>>8);
   /*IE4 - Filling CGI */
   ie =
   &szInitUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.\
   protocolIEs.member[crntIe++];

   veFillTknU8(&(ie->pres),PRSNT_NODEF);
   veFillTknU32(&(ie->id),Sztid_EUTRAN_CGI);
   veFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.sztEUTRAN_CGI.pres), PRSNT_NODEF);
   
   len = 3;
   veFillTknStrOSXL(&(ie->value.u.sztEUTRAN_CGI.pLMNidentity), len, pLMNId,\
         &szInitUeMsgPdu);
   
   len = 28;
 /*Fix given by pradeep  Start*/
   /*veFillTknBStr32(&(ie->value.u.sztEUTRAN_CGI.cell_ID), len, cellCb->cellId);*/
   tptr = &(ie->value.u.sztEUTRAN_CGI.cell_ID);
   tptr->pres = TRUE;
   tptr->len = (U8) 28;
   cptr = (U8 *)&(tptr->val);         
   tmp = cellCb->cellId;
   tmp = tmp << 4;
   p = (U8 *)&tmp;
   cptr[0] = p[3];
   cptr[1] = p[2];
   cptr[2] = p[1];
   cptr[3] = p[0];
   /*Fix given by pradeep  End*/


   ie->value.u.sztEUTRAN_CGI.iE_Extns.noComp.pres = NOTPRSNT;


   /* Filling RRC Establishment Cause */
   ie =
   &szInitUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.protocolIEs.member[crntIe++];
 
   veFillTknU8(&(ie->pres),PRSNT_NODEF);
   veFillTknU32(&(ie->id),Sztid_RRC_Establishment_Cause);
   veFillTknU32(&(ie->criticality),SztCriticalityignoreEnum );
   veFillTknU32(&(ie->value.u.sztRRC_Establishment_Cause),2);
 
  if(ueCb->sTMSI.pres == TRUE)
  {

     /*IE - Filling STMSI */
     ie =
        &szInitUeMsgPdu->pdu.val.initiatingMsg.value.u.sztInitUEMsg.\
        protocolIEs.member[crntIe++];

     veFillTknU8(&(ie->pres),PRSNT_NODEF);
     veFillTknU32(&(ie->id),Sztid_S_TMSI);
     veFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);

     veFillTknU8(&(ie->value.u.sztS_TMSI.pres), PRSNT_NODEF);
     veFill2TknStr4(&(ie->value.u.sztS_TMSI.mMEC),1, (U8 *)&ueCb->sTMSI.mmec);
     veFillTknStrOSXL1(&(ie->value.u.sztS_TMSI.m_TMSI),4, ueCb->sTMSI.mTMSI, &szInitUeMsgPdu);

     ie->value.u.sztS_TMSI.iE_Extns.noComp.pres = NOTPRSNT;
  }

  *pdu = szInitUeMsgPdu;

   RETVALUE(ROK);
} /* veUtlSztFillInitUeMsg */


/*
*
*       Fun:   veUtlSztFillUlNasTrans 
*
*       Desc:  Fill Uplink NAS Transfer 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File: ve_s1_bldmsg.c 
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillUlNasTrans
(
VeS1ConCb *s1apCon,
NhuDatIndSdus *datIndSdu,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillUlNasTrans(s1apCon,datIndSdu,pdu)
VeS1ConCb *s1apCon;
NhuDatIndSdus *datIndSdu;
S1apPdu      **pdu;
#endif
{
   S1apPdu *szUplnkNasTptPdu = NULLP;
   U16     numComp;
   U8      *nasPdu;
   S16     ret;
   U16     len;
   SztProtIE_Field_UlnkNASTport_IEs *ie = NULLP;
   VeCellCb *cellCb = NULLP;
   U8      pLMNId[10];
   U8 *cptr = NULLP;
   TknBStr32  *tptr;
   U32 tmp;
   U8 *p;

   TRC2(veUtlSztFillUlNasTrans);

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem,\
         (Ptr *)&szUplnkNasTptPdu);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillUlNasTrans: cmAllocEvnt failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Access the cell Cb */
   cellCb = veCb.cellCb[0]; 

   veFillTknU8(&(szUplnkNasTptPdu->pdu.choice), SZT_TRGR_INTI_MSG);
   veFillTknU8(&(szUplnkNasTptPdu->pdu.val.initiatingMsg.pres),\
         PRSNT_NODEF);
   veFillTknU32(&(szUplnkNasTptPdu->pdu.val.initiatingMsg.procedureCode),\
         Sztid_uplinkNASTport);
   veFillTknU32(&(szUplnkNasTptPdu->pdu.val.initiatingMsg.criticality),\
         SztCriticalityignoreEnum);

   /* Filling of 3 mandatory IEs */
   veFillTknU8(&(szUplnkNasTptPdu->pdu.val.initiatingMsg.value.u.\
            sztUlnkNASTport.pres),PRSNT_NODEF);

   numComp = 5;
   veFillTknU16(&(szUplnkNasTptPdu->pdu.val.initiatingMsg.value.u.\
            sztUlnkNASTport.protocolIEs.noComp),numComp);

   /* Allocate memory for 5 IEs */
   if ((cmGetMem(szUplnkNasTptPdu, numComp * sizeof(SztProtIE_Field_UlnkNASTport_IEs),\
               (Ptr*)&szUplnkNasTptPdu->pdu.val.initiatingMsg.value.u.\
               sztUlnkNASTport.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillUlNasTrans: cmGetMem failed.\n"));
#endif
      VE_FREE_EVNT(szUplnkNasTptPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &szUplnkNasTptPdu->pdu.val.initiatingMsg.value.u.sztUlnkNASTport.\
        protocolIEs.member[0];
   
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);

   /* IE2 - Filling enb s1ap id */
   ie = &szUplnkNasTptPdu->pdu.val.initiatingMsg.value.u.sztUlnkNASTport.\
        protocolIEs.member[1];
  
   veFillTknU8(&(ie->pres),PRSNT_NODEF);     
   veFillTknU32(&(ie->id),Sztid_eNB_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID),s1apCon->enb_ue_s1ap_id);

   /* IE3 - Filling nas pdu */
   ie = &szUplnkNasTptPdu->pdu.val.initiatingMsg.value.u.sztUlnkNASTport.\
        protocolIEs.member[2];
  
   veFillTknU8(&(ie->pres),PRSNT_NODEF);     
   veFillTknU32(&(ie->id),Sztid_NAS_PDU);
   veFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   
   len = datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.ulInfoTfr.\
         criticalExtns.val.c1.val.ulInfoTfr_r8.dedicatedInfoTyp.val.dedicatedInfoNAS.len;

   nasPdu = datIndSdu->sdu.m.ulDcchMsg.dcchMsg.message.val.c1.val.ulInfoTfr.\
            criticalExtns.val.c1.val.ulInfoTfr_r8.dedicatedInfoTyp.val.dedicatedInfoNAS.val; 

   veFillTknStrOSXL(&(ie->value.u.sztNAS_PDU), len, nasPdu, &szUplnkNasTptPdu);

  
   /* IE4 - Filling  EUTRAN CGI */
   ie = &szUplnkNasTptPdu->pdu.val.initiatingMsg.value.u.sztUlnkNASTport.\
        protocolIEs.member[3];

   veFillTknU8(&(ie->pres),PRSNT_NODEF);
   veFillTknU32(&(ie->id),Sztid_EUTRAN_CGI);
   veFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.sztEUTRAN_CGI.pres), PRSNT_NODEF);


   /* CR ID- ccpu00116764- IOT changes*/
   cmMemset(pLMNId,0,sizeof(pLMNId));
/* MNC digits CRID:ccpu00118127*/
  if(cellCb->sysRelInfo.mnc[2].pres == PRSNT_NODEF)
   {   
      pLMNId[0] =(((cellCb->sysRelInfo.mcc[1].val)<<4)| (cellCb->sysRelInfo.mcc[0].val));
      pLMNId[1] =(((cellCb->sysRelInfo.mnc[0].val)<<4)| (cellCb->sysRelInfo.mcc[2].val));
      pLMNId[2] =(((cellCb->sysRelInfo.mnc[2].val)<<4)| (cellCb->sysRelInfo.mnc[1].val));
   }
   else
   {
      /*Addition of Filler digit as 0x0f plus two digits of MNC*/
      pLMNId[0] =(((cellCb->sysRelInfo.mcc[1].val)<<4)| (cellCb->sysRelInfo.mcc[0].val));
      pLMNId[1] =(((0x0f)<<4)| (cellCb->sysRelInfo.mcc[2].val));
      pLMNId[2] =(((cellCb->sysRelInfo.mnc[1].val)<<4)| (cellCb->sysRelInfo.mnc[0].val));

   }
 
   len = 3; /* 3 octects */
   veFillTknStrOSXL(&(ie->value.u.sztEUTRAN_CGI.pLMNidentity),len, pLMNId,\
         &szUplnkNasTptPdu);
   
   len = 28;
 /*Fix given by pradeep  Start*/
/* veFillTknBStr32(&(ie->value.u.sztEUTRAN_CGI.cell_ID), len, cellCb->cellId);*/
   tptr = &(ie->value.u.sztEUTRAN_CGI.cell_ID);
   tptr->pres = TRUE;
   tptr->len = (U8) 28;
   cptr = (U8 *)&(tptr->val);
   tmp = cellCb->cellId;
   tmp = tmp << 4;
   p = (U8 *)&tmp;
   cptr[0] = p[3];
   cptr[1] = p[2];
   cptr[2] = p[1];
   cptr[3] = p[0];

 /*Fix given by pradeep  End*/

   ie->value.u.sztEUTRAN_CGI.iE_Extns.noComp.pres = NOTPRSNT;
  
   /* IE4 - Filling  TAI*/
   ie = &szUplnkNasTptPdu->pdu.val.initiatingMsg.value.u.sztUlnkNASTport.\
        protocolIEs.member[4];

   veFillTknU8(&(ie->pres),PRSNT_NODEF);
   veFillTknU32(&(ie->id),Sztid_TAI);
   veFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   veFillTknU8(&(ie->value.u.sztTAI.pres),PRSNT_NODEF);

   /* CR ID- ccpu00116764-  IOT changes*/
   cmMemset(pLMNId,0,sizeof(pLMNId));
 /* MNC digits CRID:ccpu00118127*/
  if(cellCb->sysRelInfo.mnc[2].pres == PRSNT_NODEF)
   {   
      pLMNId[0] =(((cellCb->sysRelInfo.mcc[1].val)<<4)| (cellCb->sysRelInfo.mcc[0].val));
      pLMNId[1] =(((cellCb->sysRelInfo.mnc[0].val)<<4)| (cellCb->sysRelInfo.mcc[2].val));
      pLMNId[2] =(((cellCb->sysRelInfo.mnc[2].val)<<4)| (cellCb->sysRelInfo.mnc[1].val));
   }
   else
   {
      /*Addition of Filler digit as 0x0f plus two digits of MNC*/
      pLMNId[0] =(((cellCb->sysRelInfo.mcc[1].val)<<4)| (cellCb->sysRelInfo.mcc[0].val));
      pLMNId[1] =(((0x0f)<<4)| (cellCb->sysRelInfo.mcc[2].val));
      pLMNId[2] =(((cellCb->sysRelInfo.mnc[1].val)<<4)| (cellCb->sysRelInfo.mnc[0].val));

   }
   len = 3;
   veFillTknStrOSXL(&(ie->value.u.sztTAI.pLMNidentity), len, pLMNId,\
         &szUplnkNasTptPdu);

   len = 2;
   /* CR ID- ccpu00116764- IOT changes*/
   veFillTknU8((TknU8 *) &(ie->value.u.sztTAI.tAC.pres),PRSNT_NODEF);
   ie->value.u.sztTAI.tAC.len = (U8)len;
   ie->value.u.sztTAI.tAC.val[1] = (U8)(cellCb->sysRelInfo.trackAreaCode & 0xff);
   ie->value.u.sztTAI.tAC.val[0] = (U8)((cellCb->sysRelInfo.trackAreaCode & 0xff00)>>8);
   *pdu = szUplnkNasTptPdu;

   RETVALUE(ROK);
} /* veUtlSztFillUlNasTrans */

/*
*
*       Fun:   veUtlSztFillInitCntxtSetRsp 
*
*       Desc:  Fill Initial Context Setup Response
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File: ve_s1_bldmsg.c 
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillInitCntxtSetRsp
(
VeS1ConCb *s1apCon,
NhuDatIndSdus *datIndSdu,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillInitCntxtSetRsp(s1apCon,datIndSdu,pdu)
VeS1ConCb *s1apCon;
NhuDatIndSdus *datIndSdu;
S1apPdu      **pdu;
#endif
{

   S1apPdu *szInitCntxtSetupRspPdu = NULLP;
   S16  ret;
   U16  idx;
   U16  cnt;
   U8  offSet;
   U16  numComp;
   SztProtIE_Field_InitCntxtSetupRespIEs *ie = NULLP;
   SztProtIE_SingleCont_E_RABSetupItemCtxtSUResIEs *ie1 = NULLP;
   VeUeCb *ueCb=NULLP;
   U32 len;

   TRC2(veUtlSztFillInitCntxtSetRsp);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlSztFillInitCntxtSetRsp:\
            Constructing Initial Context Setup Message \n"));
#endif


   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem,
         (Ptr *)&szInitCntxtSetupRspPdu);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillInitCntxtSetRsp\
               cmAllocEvnt failed.\n"));
#endif
      RETVALUE(RFAILED);
   }


   veFillTknU8(&(szInitCntxtSetupRspPdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   veFillTknU8(&(szInitCntxtSetupRspPdu->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   veFillTknU32(&(szInitCntxtSetupRspPdu->pdu.val.successfulOutcome.procedureCode),
         Sztid_InitCntxtSetup);
   veFillTknU32(&(szInitCntxtSetupRspPdu->pdu.val.successfulOutcome.criticality),
         SztCriticalityrejectEnum);

   /* Filling 2 mandatory headers */
   veFillTknU8(&(szInitCntxtSetupRspPdu->pdu.val.successfulOutcome.value.u.\
            sztInitCntxtSetupResp.pres),PRSNT_NODEF);
   numComp = 3;
   veFillTknU16(&(szInitCntxtSetupRspPdu->pdu.val.successfulOutcome.value.u.\
            sztInitCntxtSetupResp.protocolIEs.noComp), numComp);

   if ((cmGetMem(szInitCntxtSetupRspPdu,\
               (numComp * sizeof(SztProtIE_Field_InitCntxtSetupRespIEs)),\
               (Ptr *)&szInitCntxtSetupRspPdu->pdu.val.successfulOutcome.value.u.\
               sztInitCntxtSetupResp.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillInitCntxtSetRsp\
               cmGetMem failed.\n"));
#endif
      VE_FREE_EVNT(szInitCntxtSetupRspPdu);
      RETVALUE(RFAILED);
   }

   /* Filling MME ID */
   ie = &szInitCntxtSetupRspPdu->pdu.val.successfulOutcome.value.u.\
        sztInitCntxtSetupResp.protocolIEs.member[0];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);   
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);   
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);  

   /* Filling ENB ID */
   ie = &szInitCntxtSetupRspPdu->pdu.val.successfulOutcome.value.u.\
        sztInitCntxtSetupResp.protocolIEs.member[1];    

   veFillTknU8(&(ie->pres), PRSNT_NODEF);  
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum); 
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);                        

   ueCb = s1apCon->ueCb;

   ie = &szInitCntxtSetupRspPdu->pdu.val.successfulOutcome.value.u.\
        sztInitCntxtSetupResp.protocolIEs.member[2];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_E_RABSetupLstCtxtSURes);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

   numComp = ueCb->rbInfo.numOfRbCfgd;

   veFillTknU16(&(ie->value.u.sztE_RABSetupLstCtxtSURes.noComp), numComp);

   if (cmGetMem(szInitCntxtSetupRspPdu, 
            (numComp * sizeof(SztProtIE_SingleCont_E_RABSetupItemCtxtSUResIEs)),
            (Ptr *)&(ie->value.u.sztE_RABSetupLstCtxtSURes.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillInitCntxtSetRsp\
               cmGetMem failed.\n"));
#endif
      VE_FREE_EVNT(szInitCntxtSetupRspPdu);
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < numComp; idx++)
   {
      ie1 = &(ie->value.u.sztE_RABSetupLstCtxtSURes.member[idx]);

      veFillTknU8(&(ie1->pres), PRSNT_NODEF);
      veFillTknU32(&(ie1->id), Sztid_E_RABSetupItemCtxtSURes);
      veFillTknU32(&(ie1->criticality), SztCriticalityignoreEnum);
      veFillTknU8(&(ie1->value.u.sztE_RABSetupItemCtxtSURes.pres), PRSNT_NODEF);
      veFillTknU32(&(ie1->value.u.sztE_RABSetupItemCtxtSURes.e_RAB_ID),\
            ueCb->rbInfo.rab[idx].eRABId);

      if(veCb.datAppAddr.type == CM_TPTADDR_IPV4)
      {
         /* Build bit string from a U32 */
         len = 4; /* 4bytes */
         ie1->value.u.sztE_RABSetupItemCtxtSURes.transportLyrAddr.pres = PRSNT_NODEF;
         ie1->value.u.sztE_RABSetupItemCtxtSURes.transportLyrAddr.len = (U16)(len * 8);
         VE_GET_MEM(szInitCntxtSetupRspPdu, len, 
               &ie1->value.u.sztE_RABSetupItemCtxtSURes.transportLyrAddr.val);

         for (cnt = 0; cnt < len; cnt++)
         {
            /* compute the offset to right shift the val in U32
             * shift by 24, 16 & 8 bytes */
            offSet = (U8)(len - (cnt + 1)) * 8;
            ie1->value.u.sztE_RABSetupItemCtxtSURes.transportLyrAddr.val[cnt] = 
               (U8)(veCb.datAppAddr.u.ipv4TptAddr.address >> offSet);  
         }

      }
      else
      {
         /*IPV6 addr*/
         len = 16; /* 16 bytes */
         ie1->value.u.sztE_RABSetupItemCtxtSURes.transportLyrAddr.pres = PRSNT_NODEF;
         ie1->value.u.sztE_RABSetupItemCtxtSURes.transportLyrAddr.len = (U16)(len * 8);
         VE_GET_MEM(szInitCntxtSetupRspPdu, len, 
               &ie1->value.u.sztE_RABSetupItemCtxtSURes.transportLyrAddr.val);

         for(cnt = 0; cnt < len; cnt++)
         {
            ie1->value.u.sztE_RABSetupItemCtxtSURes.transportLyrAddr.val[cnt + 4] = 
               veCb.datAppAddr.u.ipv6TptAddr.ipv6NetAddr[cnt];
         }

      }

      len = 4;
      ie1->value.u.sztE_RABSetupItemCtxtSURes.gTP_TEID.pres = PRSNT_NODEF;
      ie1->value.u.sztE_RABSetupItemCtxtSURes.gTP_TEID.len = (U16)len;
      VE_GET_MEM(szInitCntxtSetupRspPdu, len, 
            &ie1->value.u.sztE_RABSetupItemCtxtSURes.gTP_TEID.val);

      for (cnt = 0; cnt < len; cnt++)
      {
         /* compute the offset to right shift the val in U32
          * shift by 24, 16 & 8 bytes */
         offSet = (U8)(len - (cnt + 1)) * 8;
         ie1->value.u.sztE_RABSetupItemCtxtSURes.gTP_TEID.val[cnt] = 
            (U8)(ueCb->rbInfo.rab[idx].locEgtpTunId >> offSet);  
      }
      ie1->value.u.sztE_RABSetupItemCtxtSURes.iE_Extns.noComp.pres = NOTPRSNT;
   }

   *pdu = szInitCntxtSetupRspPdu; 

   RETVALUE(ROK);
} /* veUtlSztFillInitCntxtSetRsp */

/* Bulid S1 ue capability Info ind  */
#ifdef UE_RAD_CAP
/*
*
*       Fun:   veUtlSztFillUECapbltyInfoInd 
*
*       Desc:  Fill Initial Context Setup 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillUECapbltyInfoInd
(
VeS1ConCb *s1apCon,
TknStrOSXL  *octnStr,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillUECapbltyInfoInd(s1apCon,octnStr,pdu)
VeS1ConCb *s1apCon;
TknStrOSXL  *octnStr;
S1apPdu      **pdu;
#endif
{

   S1apPdu *szUeCapInfoIndPdu = NULLP;
   S16  ret;
   U16  numComp;
   SztProtIE_Field_UECapbltyInfoIndIEs  *ie = NULLP;

   TRC2(veUtlSztFillUECapbltyInfoInd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlSztFillUECapbltyInfoInd:\
            Constructing Initial Context Setup Message \n"));
#endif

   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &veCb.mem,
         (Ptr *)&szUeCapInfoIndPdu);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillUECapbltyInfoInd\
               cmAllocEvnt failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   veFillTknU8(&(szUeCapInfoIndPdu->pdu.choice), SZT_TRGR_INTI_MSG);
   veFillTknU8(&(szUeCapInfoIndPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   veFillTknU32(&(szUeCapInfoIndPdu->pdu.val.initiatingMsg.procedureCode),
         Sztid_UECapbltyInfoInd);
   veFillTknU32(&(szUeCapInfoIndPdu->pdu.val.initiatingMsg.criticality),
         SztCriticalityrejectEnum);

   /* Filling 3 mandatory headers */
   veFillTknU8(&(szUeCapInfoIndPdu->pdu.val.initiatingMsg.value.u.\
            sztUECapbltyInfoInd.pres),PRSNT_NODEF);
   numComp = 3;
   veFillTknU16(&(szUeCapInfoIndPdu->pdu.val.initiatingMsg.value.u.\
            sztUECapbltyInfoInd.protocolIEs.noComp), numComp);

   if ((cmGetMem(szUeCapInfoIndPdu, (numComp * sizeof(SztProtIE_Field_UECapbltyInfoIndIEs)),
               (Ptr *)&szUeCapInfoIndPdu->pdu.val.initiatingMsg.value.u.\
               sztUECapbltyInfoInd.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztUeCntxtRel: cmGetMem failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &szUeCapInfoIndPdu->pdu.val.initiatingMsg.value.u.\
        sztUECapbltyInfoInd.protocolIEs.member[0];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);   
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);   
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);  

   /* IE2 - Filling ENB ID */
   ie = &szUeCapInfoIndPdu->pdu.val.initiatingMsg.value.u.\
        sztUECapbltyInfoInd.protocolIEs.member[1];    

   veFillTknU8(&(ie->pres), PRSNT_NODEF);  
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum); 
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);                        

   /* IE3 - Filling Radio cap */
   ie = &szUeCapInfoIndPdu->pdu.val.initiatingMsg.value.u.\
        sztUECapbltyInfoInd.protocolIEs.member[2];    

   veFillTknU8(&(ie->pres), PRSNT_NODEF);  
   veFillTknU32(&(ie->id), Sztid_UERadioCapblty);   
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

   veFillTknStrOSXL(&(ie->value.u.sztUERadioCapblty), octnStr->len,
                               octnStr->val,&szUeCapInfoIndPdu);

   *pdu = szUeCapInfoIndPdu; 

   RETVALUE(ROK);
} /* veUtlSztFillUECapbltyInfoInd */
#endif

/*
*
*       Fun:   veUtlSztFillERABSetRsp 
*
*       Desc:  Fill E-RAB Setup Response
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillERABSetRsp
(
VeS1ConCb *s1apCon,
S1apPdu      **pdu,
Bool        isReCfgSucc   /*RRC-ReEstab : for successful erab setup set rsp; it to TRUE ie not to be sent */
)
#else
PUBLIC S16 veUtlSztFillERABSetRsp(s1apCon, pdu,isReCfgSucc)
VeS1ConCb *s1apCon;
S1apPdu      **pdu;
Bool        isReCfgSucc;   /* RRC-ReEstab :for successful erab setup set rsp; it to TRUE ie not to be sent */
#endif
{
   S1apPdu *szERABRspPdu = NULLP;
   S16  ret;
   U16  numComp;
   SztProtIE_Field_E_RABSetupRespIEs *ie = NULLP;
   
   TRC2(veUtlSztFillERABSetRsp);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlSztFillERABSetRsp:\
            Constructing E-RAB Setup Response Message \n"));
#endif

   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem,
         (Ptr *)&szERABRspPdu);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillERABSetRsp\
               cmAllocEvnt failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   veFillTknU8(&(szERABRspPdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   veFillTknU8(&(szERABRspPdu->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   veFillTknU32(&(szERABRspPdu->pdu.val.successfulOutcome.procedureCode),
         Sztid_E_RABSetup);
   veFillTknU32(&(szERABRspPdu->pdu.val.successfulOutcome.criticality),
         SztCriticalityrejectEnum);

   /* Filling 2 mandatory headers */
   veFillTknU8(&(szERABRspPdu->pdu.val.successfulOutcome.value.u.\
            sztE_RABSetupResp.pres),PRSNT_NODEF);

   numComp = 3;

   veFillTknU16(&(szERABRspPdu->pdu.val.successfulOutcome.value.u.\
            sztE_RABSetupResp.protocolIEs.noComp), numComp);

   if ((cmGetMem(szERABRspPdu,\
               (numComp * sizeof(SztProtIE_Field_E_RABSetupRespIEs)),\
               (Ptr *)&szERABRspPdu->pdu.val.successfulOutcome.value.u.\
               sztE_RABSetupResp.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillERABSetRsp\
               cmGetMem failed.\n"));
#endif
      VE_FREE_EVNT(szERABRspPdu);
      RETVALUE(RFAILED);
   }

   /* Filling MME ID */
   ie = &szERABRspPdu->pdu.val.successfulOutcome.value.u.\
        sztE_RABSetupResp.protocolIEs.member[0];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);   
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);   
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);  

   /* Filling ENB ID */
   ie = &szERABRspPdu->pdu.val.successfulOutcome.value.u.\
        sztE_RABSetupResp.protocolIEs.member[1];    

   veFillTknU8(&(ie->pres), PRSNT_NODEF);  
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum); 
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);   
/* RRC-ReEstab */
   if(isReCfgSucc)
   {
      /* Filling ERAB Setup List from E-RAB Setup Request Pdu */
      veUtlSztFillERABRspFromERABSetReqPdu(s1apCon, &szERABRspPdu);
   }
   else  /*TODO:shishir :Added for filling fail E-RAB Rsp IEs*/
   {
      veUtlSztFillERABFailRspFromERABSetReqPdu(s1apCon,&szERABRspPdu);
   }

 
   /* assign pdu */
   *pdu = szERABRspPdu; 
   /* RRC-ReEstab */
   s1apCon->ueCb->s1MsgType = 0;

   RETVALUE(ROK);
} /* veUtlSztFillERABSetRsp */

/* E-RAB Modify */
/*
*
*       Fun:   veUtlSztFillERABMdfyRsp
*
*       Desc:  Fill E-RAB Modify Response
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillERABMdfyRsp
(
VeS1ConCb   *s1apCon,
S1apPdu     **pdu,
Bool        rspType
)
#else
PUBLIC S16 veUtlSztFillERABMdfyRsp(s1apCon, pdu, rspType)
VeS1ConCb   *s1apCon;
S1apPdu     **pdu;
Bool        rspType;
#endif
{
   S16      ret;
   U16      numComp;
   U8       ieIdx = 0;
   U8       rabIdx = 0;
   U16      numOfComps = 0;
   S1apPdu                             *szERABRspPdu = NULLP;
   S1apPdu                             *s1apPdu = NULLP;
   SztE_RABMdfyResp                    *erabMod = NULLP;
   SztE_RABMdfyRqst                    *eRABMdfyRqst = NULLP;
   SztProtIE_Field_E_RABMdfyRespIEs    *ie = NULLP;
   SztProtIE_SingleCont_E_RABMdfyItemBrModResIEs *protoie = NULLP; 

   TRC2(veUtlSztFillERABSetRsp);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlSztFillERABMdfyRsp:\
            Constructing E-RAB Modify Response Message \n"));
#endif

   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem,
         (Ptr *)&szERABRspPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillERABMdfyRsp\
               cmAllocEvnt failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   veFillTknU8(&(szERABRspPdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   veFillTknU8(&(szERABRspPdu->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   veFillTknU32(&(szERABRspPdu->pdu.val.successfulOutcome.procedureCode),
         Sztid_E_RABMdfy);
   veFillTknU32(&(szERABRspPdu->pdu.val.successfulOutcome.criticality),
         SztCriticalityrejectEnum);

   /* Filling 2 mandatory headers */
   veFillTknU8(&(szERABRspPdu->pdu.val.successfulOutcome.value.u.\
            sztE_RABMdfyResp.pres),PRSNT_NODEF);

   printf("\nE-RAB_MDFY_DBG[%s:%d]%s: Filled header\n",__FILE__, __LINE__, __FUNCTION__);
   erabMod = &(szERABRspPdu->pdu.val.successfulOutcome.value.u.sztE_RABMdfyResp);
   numComp = 3;

  /* veFillTknU16(&(szERABRspPdu->pdu.val.successfulOutcome.value.u.\
            sztE_RABMdfyResp.protocolIEs.noComp), numComp);*/

   if ((cmGetMem(szERABRspPdu, (numComp * sizeof(SztProtIE_Field_E_RABMdfyRespIEs)),
               (Ptr *)&szERABRspPdu->pdu.val.successfulOutcome.value.u.
               sztE_RABMdfyResp.protocolIEs.member)) != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillERABMdfyRsp\
               cmGetMem failed.\n"));
#endif
      VE_FREE_EVNT(szERABRspPdu);
      RETVALUE(RFAILED);
   }

   /* Filling MME ID */
   ie = &erabMod->protocolIEs.member[ieIdx];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);   
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);   
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);  
   printf("\nE-RAB_MDFY_DBG[%s:%d]%s: filled MME ID\n",__FILE__, __LINE__, __FUNCTION__);

   ieIdx++;
   /* Filling ENB ID */
   ie = &erabMod->protocolIEs.member[ieIdx];    

   veFillTknU8(&(ie->pres), PRSNT_NODEF);  
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum); 
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);   
   printf("\nE-RAB_MDFY_DBG[%s:%d]%s: ENB ID\n",__FILE__, __LINE__, __FUNCTION__);

   /* Filling ERAB Modify List from E-RAB Setup Request Pdu */
   ieIdx++;
   /* ERAB Modify List*/
   ie = &erabMod->protocolIEs.member[ieIdx];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_E_RABMdfyLstBrModRes);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   ieIdx++;
   /* vsk */
   s1apPdu = (S1apPdu *)s1apCon->ueCb->pdu;
   eRABMdfyRqst = (SztE_RABMdfyRqst *) &(s1apPdu->pdu.val.initiatingMsg.
         value.u.sztE_RABMdfyRqst);
   numOfComps = eRABMdfyRqst->protocolIEs.noComp.val;      

   veFillTknU16(&(ie->value.u.sztE_RABMdfyLstBrModRes.noComp), numOfComps);

   if (cmGetMem(szERABRspPdu,
            (numOfComps * sizeof(SztProtIE_SingleCont_E_RABMdfyItemBrModResIEs)),
            (Ptr *)&(ie->value.u.sztE_RABMdfyLstBrModRes.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veUtlSztFillERABMdfyRsp cmGetMem failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   for(rabIdx = 0;rabIdx < numOfComps && rabIdx < VE_MAX_RABS_IN_UE; rabIdx++)
   {
      protoie = &ie->value.u.sztE_RABMdfyLstBrModRes.member[rabIdx];
      veFillTknU8(&(protoie->pres), PRSNT_NODEF);
      veFillTknU32(&(protoie->id), Sztid_E_RABMdfyItemBrModRes);
      veFillTknU32(&(protoie->criticality), SztCriticalityrejectEnum);
      /* vsk TODO fix this */
      veFillTknU32(&(protoie->value.u.sztE_RABMdfyItemBrModRes.e_RAB_ID),
         s1apCon->ueCb->rbInfo.rabLst[rabIdx] );
   }
   /* IE Extensions has been disabled*/
   protoie->value.u.sztE_RABMdfyItemBrModRes.iE_Extns.noComp.pres = NOTPRSNT;
   /*TODO Failed list*/

   /* Set the number of protocol IEs here                                 */
   veFillTknU16(&(erabMod->protocolIEs.noComp), ieIdx);

   /* assign pdu */
   *pdu = szERABRspPdu; 

   RETVALUE(ROK);
} /* veUtlSztFillERABMdfyRsp */ 
/* E-RAB Modify End */ 

/*
*
*       Fun:   veUtlSztFillERABRelRsp 
*
*       Desc:  Fill E-RAB Setup Response
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillERABRelRsp
(
VeS1ConCb *s1apCon,
S1apPdu      **pdu,
Bool        isReCfgSucc       /* RRC-ReEstab :for successfull response rspType is TRUE else FALSE*/
)
#else
PUBLIC S16 veUtlSztFillERABRelRsp(s1apCon, pdu,isReCfgSucc)
VeS1ConCb *s1apCon;
S1apPdu      **pdu;
Bool       isReCfgSucc;       /* RRC-ReEstab for successfull response rspType is TRUE else FALSE*/
#endif
{

   S1apPdu *szERABRspPdu = NULLP;
   S16  ret;
   U16  numComp;
   SztProtIE_Field_E_RABRlsRespIEs *ie = NULLP;

   TRC2(veUtlSztFillERABRelRsp);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlSztFillERABRelRsp:\
            Constructing E-RAB Release Response Message \n"));
#endif

   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem,
         (Ptr *)&szERABRspPdu);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillERABRelRsp\
               cmAllocEvnt failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   veFillTknU8(&(szERABRspPdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   veFillTknU8(&(szERABRspPdu->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   veFillTknU32(&(szERABRspPdu->pdu.val.successfulOutcome.procedureCode),
         Sztid_E_RABRls);
   veFillTknU32(&(szERABRspPdu->pdu.val.successfulOutcome.criticality),
         SztCriticalityrejectEnum);

   /* Filling 2 mandatory headers */
   veFillTknU8(&(szERABRspPdu->pdu.val.successfulOutcome.value.u.\
            sztE_RABRlsResp.pres),PRSNT_NODEF);
 /* RRC-ReEstab : changed from 2 to 3*/
    numComp =3;
 
  veFillTknU16(&(szERABRspPdu->pdu.val.successfulOutcome.value.u.\
            sztE_RABRlsResp.protocolIEs.noComp), numComp);

   if ((cmGetMem(szERABRspPdu,\
               (numComp * sizeof(SztProtIE_Field_E_RABRlsRespIEs)),\
               (Ptr *)&szERABRspPdu->pdu.val.successfulOutcome.value.u.\
               sztE_RABRlsResp.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillERABRelRsp\
               cmGetMem failed.\n"));
#endif
      VE_FREE_EVNT(szERABRspPdu);
      RETVALUE(RFAILED);
   }

   /* Filling MME ID */
   ie = &szERABRspPdu->pdu.val.successfulOutcome.value.u.\
        sztE_RABRlsResp.protocolIEs.member[0];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);   
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);   
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);  

   /* Filling ENB ID */
   ie = &szERABRspPdu->pdu.val.successfulOutcome.value.u.\
        sztE_RABRlsResp.protocolIEs.member[1];    

   veFillTknU8(&(ie->pres), PRSNT_NODEF);  
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum); 
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);                        
/* RRC-ReEstab */
  if(!isReCfgSucc)
  {
    veUtlSztFillERABRlsFailRspFromERABRlsCmmdPdu(s1apCon,&szERABRspPdu);
  }
  else
  {
   veUtlSztFillERABRspFromERABRelCmdPdu(s1apCon, &szERABRspPdu);
  }
   
   /* assign pdu */
   *pdu = szERABRspPdu; 
   /* RRC-ReEstab */
   s1apCon->ueCb->s1MsgType =0; 
   RETVALUE(ROK);
} /* veUtlSztFillERABRelRsp */



/*
*
*       Fun:   veUtlSztUeCntxtRel
*
*       Desc:  S1AP Ue Context Release Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztUeCntxtRel
(
VeS1ConCb *s1Con,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztUeCntxtRel(s1Con,pdu)
VeS1ConCb *s1Con;
S1apPdu      **pdu;
#endif
{
   S1apPdu *szUeCxtRlsPdu = NULLP;
   S16  ret;
   U16  numComp;
   SztProtIE_Field_UECntxtRlsRqst_IEs *ie = NULLP;

   TRC2(veUtlSztUeCntxtRelComp);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlSztUeCntxtRel:\
            Constructing UE Context Release Request\n"));
#endif

   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem, \
         (Ptr *)&szUeCxtRlsPdu);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztUeCntxtRelComp: cmAllocEvnt failed.\n"));
#endif
      RETVALUE(RFAILED);
   }


   veFillTknU8(&(szUeCxtRlsPdu->pdu.choice), SZT_TRGR_INTI_MSG);
   veFillTknU8(&(szUeCxtRlsPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   veFillTknU32(&(szUeCxtRlsPdu->pdu.val.initiatingMsg.procedureCode),
         Sztid_UECntxtRlsRqst);
   veFillTknU32(&(szUeCxtRlsPdu->pdu.val.initiatingMsg.criticality),
         SztCriticalityrejectEnum);

   /* Filling 3 mandatory headers */
   veFillTknU8(&(szUeCxtRlsPdu->pdu.val.initiatingMsg.value.u.\
            sztUECntxtRlsRqst.pres),PRSNT_NODEF);
   numComp = 3;
   veFillTknU16(&(szUeCxtRlsPdu->pdu.val.initiatingMsg.value.u.\
            sztUECntxtRlsRqst.protocolIEs.noComp), numComp);

   if ((cmGetMem(szUeCxtRlsPdu, (numComp * sizeof(SztProtIE_Field_UECntxtRlsRqst_IEs)),
               (Ptr *)&szUeCxtRlsPdu->pdu.val.initiatingMsg.value.u.\
               sztUECntxtRlsRqst.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztUeCntxtRel: cmGetMem failed.\n"));
#endif
      VE_FREE_EVNT(szUeCxtRlsPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &szUeCxtRlsPdu->pdu.val.initiatingMsg.value.u.\
        sztUECntxtRlsRqst.protocolIEs.member[0];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);   
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);   
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1Con->mme_ue_s1ap_id);  

   /* IE2 - Filling ENB ID */
   ie = &szUeCxtRlsPdu->pdu.val.initiatingMsg.value.u.\
        sztUECntxtRlsRqst.protocolIEs.member[1];    

   veFillTknU8(&(ie->pres), PRSNT_NODEF);  
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum); 
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1Con->enb_ue_s1ap_id);                        

   /* IE3 - Filling Cause */
   ie = &szUeCxtRlsPdu->pdu.val.initiatingMsg.value.u.\
        sztUECntxtRlsRqst.protocolIEs.member[2];    

   veFillTknU8(&(ie->pres), PRSNT_NODEF);  
   veFillTknU32(&(ie->id), Sztid_Cause);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum); 
   veFillTknU8(&(ie->value.u.sztCause.choice), s1Con->cause.choice.val);
   switch(s1Con->cause.choice.val)
   {
      case CAUSE_RADIONW:
      {
         veFillTknU32(&(ie->value.u.sztCause.val.radioNw), 
                                                      s1Con->cause.val.radioNw.val);
         break;
      }
      case CAUSE_TRANSPORT:
      {
         veFillTknU32(&(ie->value.u.sztCause.val.transport),
                                                    s1Con->cause.val.transport.val);
         break;
      }
      case CAUSE_NAS:
      {
         veFillTknU32(&(ie->value.u.sztCause.val.nas),s1Con->cause.val.nas.val);
         break;
      }
      case 3:/*TODO */
      {
         veFillTknU32(&(ie->value.u.sztCause.val.protocol),
                                                     s1Con->cause.val.protocol.val);
         break;
      }
      case 4: /*TODO CAUSE_MISC*/
      {
         veFillTknU32(&(ie->value.u.sztCause.val.misc),s1Con->cause.val.misc.val);
         break;
      }
      default:
      {
         veFillTknU32(&(ie->value.u.sztCause.val.radioNw), 
                                                      s1Con->cause.val.radioNw.val);
         break;  
      }
   }

   *pdu = szUeCxtRlsPdu; 

   RETVALUE(ROK);
} /* veUtlSztUeCntxtRel */


/*
*
*       Fun:   veUtlSztFillNasNonDlvInd 
*
*       Desc:  Fill NAS NON DELIVERY INDICATION
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillNasNonDlvInd
(
VeS1ConCb    *s1apCon,
S1apPdu      **s1ap_pdu,
TknStrOSXL   *nasPdu
)
#else
PUBLIC S16 veUtlSztFillNasNonDlvInd(s1apCon, s1ap_pdu, nasPdu)
VeS1ConCb    *s1apCon;
S1apPdu      **s1ap_pdu;
TknStrOSXL   *nasPdu;
#endif
{
   U16     numComp = 0;
   S16     ret = ROK;

   SztProtIE_Field_NASNonDlvryInd_IEs *ie = NULLP;
   S1apPdu *szNasNonDlvInd = NULLP;
   VeCellCb *cellCb = NULLP;

   TRC2(veUtlSztFillNasNonDlvInd);

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem,\
         (Ptr *)&szNasNonDlvInd);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillNasNonDlvInd: cmAllocEvnt failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Access the cell Cb */
   cellCb = veCb.cellCb[0]; 

   veFillTknU8(&(szNasNonDlvInd->pdu.choice), SZT_TRGR_INTI_MSG);
   veFillTknU8(&(szNasNonDlvInd->pdu.val.initiatingMsg.pres),\
         PRSNT_NODEF);
   veFillTknU32(&(szNasNonDlvInd->pdu.val.initiatingMsg.procedureCode),\
         Sztid_NASNonDlvryInd);
   veFillTknU32(&(szNasNonDlvInd->pdu.val.initiatingMsg.criticality),\
         SztCriticalityignoreEnum);

   /* Filling of 3 mandatory IEs */
   veFillTknU8(&(szNasNonDlvInd->pdu.val.initiatingMsg.value.u.\
            sztNASNonDlvryInd.pres), PRSNT_NODEF);

   numComp = 4;
   veFillTknU16(&(szNasNonDlvInd->pdu.val.initiatingMsg.value.u.\
            sztNASNonDlvryInd.protocolIEs.noComp), numComp);

   /* Allocate memory for 4 IEs */
   if ((cmGetMem(szNasNonDlvInd, numComp * sizeof(SztProtIE_Field_NASNonDlvryInd_IEs),\
               (Ptr*)&szNasNonDlvInd->pdu.val.initiatingMsg.value.u.\
               sztNASNonDlvryInd.protocolIEs.member)) != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillNasNonDlvInd: cmGetMem failed.\n"));
#endif
      VE_FREE_EVNT(szNasNonDlvInd);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &szNasNonDlvInd->pdu.val.initiatingMsg.value.u.sztNASNonDlvryInd.\
        protocolIEs.member[0];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);

   /* IE2 - Filling enb s1ap id */
   ie = &szNasNonDlvInd->pdu.val.initiatingMsg.value.u.sztNASNonDlvryInd.\
        protocolIEs.member[1];

   veFillTknU8(&(ie->pres),PRSNT_NODEF);     
   veFillTknU32(&(ie->id),Sztid_eNB_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID),s1apCon->enb_ue_s1ap_id);

   /* IE3 - Filling NAS PDU */
   ie = &szNasNonDlvInd->pdu.val.initiatingMsg.value.u.sztNASNonDlvryInd.\
        protocolIEs.member[2];

   veFillTknU8(&(ie->pres),PRSNT_NODEF);     
   veFillTknU32(&(ie->id),Sztid_NAS_PDU);
   veFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);

   veFillTknStrOSXL(&(ie->value.u.sztNAS_PDU), nasPdu->len, nasPdu->val, &szNasNonDlvInd);

   /* IE4 - Filling Cause */
   ie = &szNasNonDlvInd->pdu.val.initiatingMsg.value.u.sztNASNonDlvryInd.\
        protocolIEs.member[3];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_Cause);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.sztCause.choice), SztCauseRadioNwuser_inactivityEnum);
   veFillTknU32(&(ie->value.u.sztCause.val.radioNw),\
         SztCauseRadioNwunknown_targetIDEnum);

   *s1ap_pdu = szNasNonDlvInd;

   RETVALUE(ROK);
} /* veUtlSztFillNasNonDlvInd */

/*
*
*       Fun:   veUtlSztFillInitCntxtSetFail 
*
*       Desc:  Fill INITIAL CONTEXT SETUP FAILURE
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillInitCntxtSetFail
(
VeS1ConCb    *s1apCon,
S1apPdu      **s1apPdu,
U32          cause  
)
#else
PUBLIC S16 veUtlSztFillInitCntxtSetFail(s1apCon, s1apPdu, cause)
VeS1ConCb    *s1apCon;
S1apPdu      **s1apPdu;
U32          cause;
#endif
{
   U16     numComp = 0;
   S16     ret = ROK;
   
   SztProtIE_Field_InitCntxtSetupFailIEs *ie = NULLP;
   S1apPdu *szInitCntxtSetFail = NULLP;
   VeCellCb *cellCb = NULLP;

   TRC2(veUtlSztFillInitCntxtSetFail);

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem,\
         (Ptr *)&szInitCntxtSetFail);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillInitCntxtSetFail: cmAllocEvnt failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Access the cell Cb */
   cellCb = veCb.cellCb[0]; 

   veFillTknU8(&(szInitCntxtSetFail->pdu.choice), SZT_TRGR_UNSUCCESS_OUTCOME);
   veFillTknU8(&(szInitCntxtSetFail->pdu.val.unsuccessfulOutcome.pres),\
         PRSNT_NODEF);
   veFillTknU32(&(szInitCntxtSetFail->pdu.val.unsuccessfulOutcome.procedureCode),\
         Sztid_InitCntxtSetup);
   veFillTknU32(&(szInitCntxtSetFail->pdu.val.unsuccessfulOutcome.criticality),\
         SztCriticalityignoreEnum);

   /* Filling of 3 mandatory IEs */
   veFillTknU8(&(szInitCntxtSetFail->pdu.val.unsuccessfulOutcome.value.u.\
            sztInitCntxtSetupFail.pres), PRSNT_NODEF);

   numComp = 3;
   veFillTknU16(&(szInitCntxtSetFail->pdu.val.unsuccessfulOutcome.value.u.\
            sztInitCntxtSetupFail.protocolIEs.noComp), numComp);

   /* Allocate memory for 3 IEs */
   if ((cmGetMem(szInitCntxtSetFail, numComp * sizeof(SztProtIE_Field_InitCntxtSetupFailIEs),\
               (Ptr*)&szInitCntxtSetFail->pdu.val.unsuccessfulOutcome.value.u.\
               sztInitCntxtSetupFail.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillInitCntxtSetFail: cmGetMem failed.\n"));
#endif
      VE_FREE_EVNT(szInitCntxtSetFail);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &szInitCntxtSetFail->pdu.val.unsuccessfulOutcome.value.u.sztInitCntxtSetupFail.\
        protocolIEs.member[0];
   
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);

   /* IE2 - Filling enb s1ap id */
   ie = &szInitCntxtSetFail->pdu.val.unsuccessfulOutcome.value.u.sztInitCntxtSetupFail.\
        protocolIEs.member[1];
  
   veFillTknU8(&(ie->pres), PRSNT_NODEF);     
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID),s1apCon->enb_ue_s1ap_id);

   /* IE3 - Filling Cause */
   ie = &szInitCntxtSetFail->pdu.val.unsuccessfulOutcome.value.u.sztInitCntxtSetupFail.\
        protocolIEs.member[2];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_Cause);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.sztCause.choice), SztCauseRadioNwuser_inactivityEnum);
   veFillTknU32(&(ie->value.u.sztCause.val.radioNw),\
         cause);

   *s1apPdu = szInitCntxtSetFail;

   RETVALUE(ROK);
} /* veUtlSztFillInitCntxtSetFail */

/*
*
*       Fun:   veUtlSztUeCntxtRelComp 
*
*       Desc:  S1AP Ue Context Release Complete 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztUeCntxtRelComp
(
VeS1ConCb *s1Con,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztUeCntxtRelComp(s1Con,pdu)
VeS1ConCb *s1Con;
S1apPdu      **pdu;
#endif
{
   S1apPdu *szUeCxtRlsCmplPdu = NULLP;
   S16  ret;
   U16  numComp;
   SztProtIE_Field_UECntxtRlsComplete_IEs *ie = NULLP;

   TRC2(veUtlSztUeCntxtRelComp);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlSztUeCntxtRelComp:\
            Constructing UE Context Release \n"));
#endif

   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem, \
         (Ptr *)&szUeCxtRlsCmplPdu);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztUeCntxtRelComp: cmAllocEvnt failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   veFillTknU8(&(szUeCxtRlsCmplPdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   veFillTknU8(&(szUeCxtRlsCmplPdu->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   veFillTknU32(&(szUeCxtRlsCmplPdu->pdu.val.successfulOutcome.procedureCode),
         Sztid_UECntxtRls);
   veFillTknU32(&(szUeCxtRlsCmplPdu->pdu.val.successfulOutcome.criticality),
         SztCriticalityrejectEnum);

   /* Filling 2 mandatory headers */
   veFillTknU8(&(szUeCxtRlsCmplPdu->pdu.val.successfulOutcome.value.u.\
            sztUECntxtRlsComplete.pres),PRSNT_NODEF);
   numComp = 2;
   veFillTknU16(&(szUeCxtRlsCmplPdu->pdu.val.successfulOutcome.value.u.\
            sztUECntxtRlsComplete.protocolIEs.noComp), numComp);

   if ((cmGetMem(szUeCxtRlsCmplPdu, (numComp * sizeof(SztProtIE_Field_UECntxtRlsComplete_IEs)),
               (Ptr *)&szUeCxtRlsCmplPdu->pdu.val.successfulOutcome.value.u.\
               sztUECntxtRlsComplete.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztUeCntxtRelComp: cmGetMem failed.\n"));
#endif
      VE_FREE_EVNT(szUeCxtRlsCmplPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &szUeCxtRlsCmplPdu->pdu.val.successfulOutcome.value.u.\
        sztUECntxtRlsComplete.protocolIEs.member[0];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);   
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);   
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1Con->mme_ue_s1ap_id);  

   /* IE2 - Filling ENB ID */
   ie = &szUeCxtRlsCmplPdu->pdu.val.successfulOutcome.value.u.\
        sztUECntxtRlsComplete.protocolIEs.member[1];    

   veFillTknU8(&(ie->pres), PRSNT_NODEF);  
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum); 
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1Con->enb_ue_s1ap_id);                        

   *pdu = szUeCxtRlsCmplPdu; 

   RETVALUE(ROK);
} /* veUtlSztUeCntxtRelComp */

/*
*
*       Fun:   veUtlSztFillERABRspFromERABSetReqPdu
*
*       Desc:  Call handler to fill ERAB Response
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillERABRspFromERABSetReqPdu
(
VeS1ConCb *s1apCon,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillERABRspFromERABSetReqPdu(s1apCon, pdu)
VeS1ConCb *s1apCon;
S1apPdu      **pdu;
#endif
{
   S1apPdu *szERABRspPdu = NULLP;
   S16  ret = ROK;
   SztProtIE_Field_E_RABSetupRespIEs *ie = NULLP;
   SztProtIE_SingleCont_E_RABSetupItemBrSUResIEs *rspMember = NULLP;

   SztE_RABSetupRqst                          *eRABSetupReq = NULLP;
   SztProtIE_Field_E_RABSetupRqstIEs   *member = NULLP;
   SztProtIE_SingleCont_E_RABToBeSetupItemBrSUReqIEs *protIe = NULLP;
   SztE_RABToBeSetupItemBrSUReq *itemIe = NULLP;
   S1apPdu *szERABReqPdu = NULLP;


   U8   gTPId[4];
   U8   len = 0;
   U8   cnt = 0;
   U16 numOfComps = 0;
   U16 memberIdx = 0;
   U16 noERABIds = 0;
   U16 idx = 0;
   U16 rbIdx = 0;
   U32 protId = 0;
   U8 offSet;
   
   /* Added trace macro */
   TRC2(veUtlSztFillERABRspFromERABSetReqPdu)

   szERABRspPdu = *pdu;
   szERABReqPdu = s1apCon->ueCb->pdu;

   /* retrieve info from S1AP Pdu */
   eRABSetupReq = (SztE_RABSetupRqst *) &(szERABReqPdu->pdu.val.initiatingMsg.\
         value.u.sztE_RABSetupRqst);

   numOfComps = eRABSetupReq->protocolIEs.noComp.val;

   /* ERAB Setup List  */
   ie = &szERABRspPdu->pdu.val.successfulOutcome.value.u.\
        sztE_RABSetupResp.protocolIEs.member[2];    

   veFillTknU8(&(ie->pres), PRSNT_NODEF);  
   veFillTknU32(&(ie->id), Sztid_E_RABSetupLstBrSURes);   
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum); 
   veFillTknU16(&(ie->value.u.sztE_RABSetupLstBrSURes.noComp), 1); 

   if ((cmGetMem(szERABRspPdu,\
               (1 * sizeof(SztProtIE_SingleCont_E_RABSetupItemBrSUResIEs)),\
               (Ptr *)&ie->value.u.sztE_RABSetupLstBrSURes.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillERABRspFromERABSetReqPdu\
               cmGetMem failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Locate ERAB to be setup List IE  */
   for(memberIdx = 0; memberIdx < numOfComps; memberIdx++)
   {
      member = &(eRABSetupReq->protocolIEs.member[memberIdx]);
      protId = member->id.val;
      switch(protId)
      {
         case Sztid_E_RABToBeSetupLstBrSUReq:
            {
               noERABIds = member->value.u.sztE_RABToBeSetupLstBrSUReq.noComp.val;

               for(idx = 0; idx < noERABIds; idx++)
               {
                  rspMember = &(ie->value.u.sztE_RABSetupLstBrSURes.member[idx]);
                  protIe = &(member->value.u.sztE_RABToBeSetupLstBrSUReq.member[idx]);
                  itemIe = &(protIe->value.u.sztE_RABToBeSetupItemBrSUReq);

                  veFillTknU8(&(rspMember->pres), PRSNT_NODEF);  
                  veFillTknU32(&(rspMember->id), Sztid_E_RABSetupItemBrSURes);   
                  veFillTknU32(&(rspMember->criticality), SztCriticalityrejectEnum); 

                  veFillTknU8(&(rspMember->value.u.sztE_RABSetupItemBrSURes.pres), PRSNT_NODEF); 
                  /* E-RAB ID */
                  cmMemcpy((U8*)&(rspMember->value.u.sztE_RABSetupItemBrSURes.e_RAB_ID), \
                       (U8*)&(itemIe->e_RAB_ID), sizeof(SztE_RAB_ID));
                  /* Transport Layer Address Should be the eNodeB */
                  /* Build bit string from a U32 */
                  len = 4; /* 4bytes */
                  rspMember->value.u.sztE_RABSetupItemBrSURes.transportLyrAddr.pres
                                      = PRSNT_NODEF;
                  rspMember->value.u.sztE_RABSetupItemBrSURes.transportLyrAddr.len
                                          = (len * 8);

                  VE_GET_MEM(szERABRspPdu, len, 
                        &rspMember->value.u.sztE_RABSetupItemBrSURes.transportLyrAddr.val);

                  for (cnt = 0; cnt < len; cnt++)
                  {
                     /* compute the offset to right shift the val in U32
                      * shift by 24, 16 & 8 bytes */
                     offSet = (len - (cnt + 1)) * 8;
                     rspMember->value.u.sztE_RABSetupItemBrSURes.transportLyrAddr.val[cnt] = 
                        (U8)((veCb.datAppAddr.u.ipv4TptAddr.address >> offSet) & 0xff);  
                  }

                  /*cmMemcpy(&(rspMember->value.u.sztE_RABSetupItemBrSURes.transportLyrAddr), \
                        &(itemIe->transportLyrAddr), sizeof(SztTportLyrAddr));*/
                  /* Local  GTP ID */
                  for (rbIdx = 0; rbIdx < VE_MAX_RABS_IN_UE; rbIdx++)
                  {
                     if(itemIe->e_RAB_ID.val ==  s1apCon->ueCb->rbInfo.rab[rbIdx].eRABId)
                     {
                        len = 4;
                        gTPId[0] = (U8)(s1apCon->ueCb->rbInfo.rab[rbIdx].locEgtpTunId >> 24) & 0xff;
                        gTPId[1] = (U8)(s1apCon->ueCb->rbInfo.rab[rbIdx].locEgtpTunId >> 16) & 0xff;
                        gTPId[2] = (U8)(s1apCon->ueCb->rbInfo.rab[rbIdx].locEgtpTunId >> 8) & 0xff;
                        gTPId[3] = (U8)(s1apCon->ueCb->rbInfo.rab[rbIdx].locEgtpTunId & 0xff);
                        ret = veFillTknStrOSXL(&(rspMember->value.u.sztE_RABSetupItemBrSURes.gTP_TEID), 
                              len, gTPId, &szERABRspPdu);
                        break;
                     }
                  }
                  rspMember->value.u.sztE_RABSetupItemBrSURes.iE_Extns.noComp.pres = NOTPRSNT;

               }/* End of For Loop */
            }
            break;
         default:
            break;
      }
   }
   RETVALUE(ret);
}

/*
*
*       Fun:  veUtlSztFillERABRspFromERABRelCmdPdu  
*
*       Desc:  Call handler to fill ERAB Release Response
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillERABRspFromERABRelCmdPdu
(
VeS1ConCb *s1apCon,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillERABRspFromERABRelCmdPdu(s1apCon, pdu)
VeS1ConCb *s1apCon;
S1apPdu      **pdu;
#endif
{
   S1apPdu *szERABRspPdu = NULLP;
   S16  ret = ROK;
   SztProtIE_Field_E_RABRlsRespIEs *ie = NULLP;
  SztProtIE_SingleCont_E_RABRlsItemBrRelCompIEs  *rspMember = NULLP;

   SztE_RABRlsCmmd *eRABRlsCom = NULLP;
   SztProtIE_Field_E_RABRlsCmmdIEs *member;
   SztProtIE_SingleCont_E_RABItemIEs *protIe = NULLP;
   SztE_RABToBeSetupItemBrSUReq *itemIe = NULLP;
   S1apPdu *szERABRlsCmdPdu = NULLP;


   U16 numOfComps = 0;
   U16 memberIdx = 0;
   U16 noERABIds = 0;
   U16 idx = 0;
   U32 protId = 0;
   
   /* Added trace macro */
   TRC2(veUtlSztFillERABRspFromERABRelCmdPdu)

   szERABRspPdu = *pdu;
   szERABRlsCmdPdu = s1apCon->ueCb->pdu;

   /* retrieve info from S1AP Pdu */
   eRABRlsCom = (SztE_RABRlsCmmd *) &(szERABRlsCmdPdu->pdu.val.initiatingMsg.\
         value.u.sztE_RABRlsCmmd);

   numOfComps = eRABRlsCom->protocolIEs.noComp.val;

   /* ERAB Release Command List  */
   ie = &szERABRspPdu->pdu.val.successfulOutcome.value.u.\
        sztE_RABRlsResp.protocolIEs.member[2];    



   /* Locate ERAB to be Released List IE  */
   for(memberIdx = 0; memberIdx < numOfComps; memberIdx++)
   {
      member = &(eRABRlsCom->protocolIEs.member[memberIdx]);
      protId = member->id.val;
      switch(protId)
      {
         case Sztid_E_RABToBeRlsdLst:
            {
                noERABIds = member->value.u.sztE_RABLst.noComp.val;

                veFillTknU8(&(ie->pres), PRSNT_NODEF);  
                veFillTknU32(&(ie->id),Sztid_E_RABRlsLstBrRelComp);   
                veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum); 

                if ((cmGetMem(szERABRspPdu,\
                     (noERABIds * sizeof(SztProtIE_SingleCont_E_RABRlsItemBrRelCompIEs)),\
                     (Ptr *)&ie->value.u.sztE_RABRlsLstBrRelComp.member)) !=ROK)
                {
#ifdef DEBUGP
                        VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillERABRspFromERABSetReqPdu\
                                                cmGetMem failed.\n"));
#endif
                        RETVALUE(RFAILED);
                }

               veFillTknU16(&(ie->value.u.sztE_RABRlsLstBrRelComp.noComp), noERABIds);

               for(idx = 0; idx < noERABIds; idx++)
               {
                  rspMember = &(ie->value.u.sztE_RABRlsLstBrRelComp.member[idx]);

                  protIe = &(member->value.u.sztE_RABLst.member[idx]);
                  itemIe = (SztE_RABToBeSetupItemBrSUReq *)&(protIe->value.u.sztE_RABItem);

                  veFillTknU8(&(rspMember->pres), PRSNT_NODEF);  
                  veFillTknU32(&(rspMember->id), Sztid_E_RABRlsItemBrRelComp);   
                  veFillTknU32(&(rspMember->criticality), SztCriticalityrejectEnum); 

                  veFillTknU8(&(rspMember->value.u.sztE_RABRlsItemBrRelComp.pres), PRSNT_NODEF); 
                  /* E-RAB ID */
                  cmMemcpy((U8*)&(rspMember->value.u.sztE_RABRlsItemBrRelComp.e_RAB_ID), \
                       (U8*)&(itemIe->e_RAB_ID), sizeof(SztE_RAB_ID));

                  rspMember->value.u.sztE_RABRlsItemBrRelComp.iE_Extns.noComp.pres = NOTPRSNT;

               }/* End of For Loop */
            }
            break;
         default:
            break;
      }
   }
   RETVALUE(ret);
} 

#ifdef LTE_HO_SUPPORT
/* Handover Related messages */
/*
*
*       Fun:   veUtlSztFillHovrReqd
*
*       Desc:  S1AP Fill Handover Required
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File: ve_s1_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillHovrReqd
(
VeS1ConCb      *s1Con,
S1apPdu        **pdu,
VeNghCellCb    *nghCellCb,
NhuEncCfmSdus  *nhuEncCfmSdus
)
#else
PUBLIC S16 veUtlSztFillHovrReqd(s1Con, pdu, nghCellCb, nhuEncCfmSdus)
VeS1ConCb      *s1Con;
S1apPdu        **pdu;
VeNghCellCb    *nghCellCb;
NhuEncCfmSdus  *nhuEncCfmSdus;
#endif
{
   S1apPdu   *hovrReqdPdu = NULLP;
   S16        compCnt = 0;
   S16        numComp = 0;
   S16        ret = 0;
   SztProtIE_Field_HovrReqdIEs *ie = NULLP;
   SztTgeteNB_ID *ie1 = NULLP;
   SztSrceNB_ToTgeteNB_TprntCont srcToTgtCntr;
   U8      idx = 0;
   U8      rbIdx = 0;
   U8      rbCnt = 0;
   VeUeRAB *rbInfo = NULLP;
   SztE_RABInformLstItem *rbItem = NULLP;
   SztLastVisitedCell_Item *lastVstCelInfo = NULLP;
   Buffer *trnspContMBuf = NULLP;
   MsgLen cnt = 0;
   MsgLen len = 0;
   S1apEncDecIE *encTrpCnt = NULLP;
   SztReqStatus reqSts;

   U32 asnLen = 0;
   CmPAsnErr    asnErr;
   CmPAsnElmDef *szMsgIEDb_locTesting[][4] = { {&szSrceNB_ToTgeteNB_TprntCont} };

   TRC2(veUtlSztFillHovrReqd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlSztFillHovrReqd:\
            Constructing Handover Required \n"));
#endif

   /* Check s1Con pointer is NULL or not */
   if(s1Con == NULLP || pdu == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqd : Invald Arguments.\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem, 
                                                           (Ptr *)&hovrReqdPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqd: cmAllocEvnt failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&(hovrReqdPdu->pdu), 0, sizeof(SztS1AP_PDU));

   /* Fill the Interface Elementary Procedure for Handover Required Message. */
   veFillTknU8(&(hovrReqdPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   veFillTknU8(&(hovrReqdPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   veFillTknU32(&(hovrReqdPdu->pdu.val.initiatingMsg.procedureCode), 
                                                                Sztid_HovrPrep);
   veFillTknU32(&(hovrReqdPdu->pdu.val.initiatingMsg.criticality), 
                                                      SztCriticalityrejectEnum);

   /* Fill the Protocol IEs of Handover Required Message. */
   numComp = 6; /* Indicates No of Mandatory IEs.*/
   veFillTknU8(&(hovrReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.pres),
                                                                   PRSNT_NODEF);
   veFillTknU16(&(hovrReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.
                                                  protocolIEs.noComp), numComp);

   /* Allocate the memory for Protocol IEs.*/
   if ((cmGetMem(&hovrReqdPdu->memCp, (numComp * sizeof(SztProtIE_Field_HovrReqdIEs)),
               ((Ptr *)&hovrReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.\
                                                     protocolIEs.member))) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqd: cmGetMem failed.\n"));
#endif
      VE_FREE_EVNT(hovrReqdPdu);
      RETVALUE(RFAILED);
   }
   /* 
   * Start of comment
   * Fill MME UE S1AP ID IE which is generated by MME.
   * MME UE S1AP Id uniquly identify the UE over S1 interface within MME.
   * End of Comment.
   */
   ie = &hovrReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.
                                                              member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1Con->mme_ue_s1ap_id);
   /*
    * Start of Comment.
    * Fill eNB UE S1AP ID IE which is generated by eNB.
    * eNB UE S1AP ID uniquly identify the UE over S1 interface within eNB.
    * End of Comment.
    */
   ie = &hovrReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.
                                                              member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1Con->enb_ue_s1ap_id);
   /* 
    * Start of Comment.
    * Fill Handover type IE. In Current version,we supports only Intra LTE 
    * handover type.So, Value is hard coded here.
    * End of Comment.
    */
   ie = &hovrReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.
                                                              member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_HovrTyp);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztHovrTyp), SztHovrTypintralteEnum);
   /*
    * Start of Comment.
    * Fill Cause IE which indicates the reason why the UE Handover from source
    * eNB to Target eNB.
    * End of Comment.
    */
   ie = &hovrReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.
                                                              member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_Cause);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.sztCause.choice), s1Con->cause.choice.val);
   switch(s1Con->cause.choice.val)
   {
      case CAUSE_RADIONW:
      {
         veFillTknU32(&(ie->value.u.sztCause.val.radioNw), 
                                                      s1Con->cause.val.radioNw.val);
         break;
      }
      case CAUSE_TRANSPORT:
      {
         veFillTknU32(&(ie->value.u.sztCause.val.transport),
                                                    s1Con->cause.val.transport.val);
         break;
      }
      case CAUSE_NAS:
      {
         veFillTknU32(&(ie->value.u.sztCause.val.nas),s1Con->cause.val.nas.val);
         break;
      }
      case 3:/*TODO */
      {
         veFillTknU32(&(ie->value.u.sztCause.val.protocol),
                                                     s1Con->cause.val.protocol.val);
         break;
      }
      case 4: /*TODO CAUSE_MISC*/
      {
         veFillTknU32(&(ie->value.u.sztCause.val.misc),s1Con->cause.val.misc.val);
         break;
      }
      default:
      {
         veFillTknU32(&(ie->value.u.sztCause.val.radioNw), 
                                                      s1Con->cause.val.radioNw.val);
         break;  
      }
   }/*End of switch (s1Con->cause.choice)*/

   /* 
    * Start of Comment.
    * Fill Target EnbId IE. It is combination of Global eNB Id and Selected 
    * TAI.
    * End of comment.
    */
   ie = &hovrReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.
                                                              member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32((TknU32 *) &(ie->id), Sztid_TgetID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   /* Based on HANDOVER TYPE, Target-Id will change.*/
   switch(ie->value.u.sztHovrTyp.val)
   {
      case SztHovrTypintralteEnum:
      {
         veFillTknU8(&(ie->value.u.sztTgetID.choice), TGETID_TARGETENB_ID);
         break;
      }
      case SztHovrTypltetoutranEnum:
      {
         veFillTknU8(&(ie->value.u.sztTgetID.choice), TGETID_TARGETRNC_ID);
         break;
      }
      case SztHovrTypltetogeranEnum:
      {
         veFillTknU8(&(ie->value.u.sztTgetID.choice),TGETID_CGI);
         break;
      }
      case SztHovrTyputrantolteEnum:
      {
         veFillTknU8(&(ie->value.u.sztTgetID.choice), TGETID_TARGETRNC_ID);
         break;
      }
      case SztHovrTypgerantolteEnum:
      {
         veFillTknU8(&(ie->value.u.sztTgetID.choice),TGETID_CGI);
         break;
      }
      default:
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF," Invalid HandOver Type.\n"));
#endif
         VE_FREE_EVNT(hovrReqdPdu);
         RETVALUE(RFAILED);        
      }
   }/* End of Switch(ie->value.u.sztHovrTyp)*/

   switch(ie->value.u.sztTgetID.choice.val)
   {
      case TGETID_TARGETENB_ID:
      {
         ie1 = &ie->value.u.sztTgetID.val.targeteNB_ID;

         veFillTknU8(&(ie1->pres), PRSNT_NODEF);
         veFillTknU8(&(ie1->global_ENB_ID.pres), PRSNT_NODEF);
         len = 3;
         veFillTknStrOSXL(&(ie1->global_ENB_ID.pLMNidentity), len,
                                               (Data *)&nghCellCb->plmnId, &hovrReqdPdu);
         veFillTknU8(&(ie1->global_ENB_ID.eNB_ID.choice), nghCellCb->enbType);
         switch(ie1->global_ENB_ID.eNB_ID.choice.val)
         {
            case ENB_ID_MACROENB_ID:
            {
               len = 20;
               veFillTknBStr32(&(ie1->global_ENB_ID.eNB_ID.val.homeENB_ID), 
                                                                      len, 100);
               break;
            }
            case ENB_ID_HOMEENB_ID:
            {
               break;
            }
            default:
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF," Invalid eNB Type.\n"));
#endif
               VE_FREE_EVNT(hovrReqdPdu);
               RETVALUE(RFAILED);              
            }
         }/* End of switch(ie1->global_ENB_ID.eNB_ID.choice)*/

         ie1->global_ENB_ID.iE_Extns.noComp.pres = NOTPRSNT;
         /* Fill the Tracking Area Identity */
         veFillTknU8(&(ie1->selected_TAI.pres), PRSNT_NODEF);
         len = 3;
         veFillTknStrOSXL(&(ie1->selected_TAI.pLMNidentity), len, 
                                               (Data *)&nghCellCb->plmnId, &hovrReqdPdu);
         len = 2;
         veFillTknStr4(&(ie1->selected_TAI.tAC), (U8)len, nghCellCb->veCellPrp.
                                                                           tac);
         ie1->selected_TAI.iE_Extns.noComp.pres = NOTPRSNT;
         break;
      }
      case TGETID_TARGETRNC_ID:
      {
         /* filling of Target RNC info not supported for this release */
         ie1 = (SztTgeteNB_ID *)(&ie->value.u.sztTgetID.val.targetRNC_ID);
         break;
      }
      case TGETID_CGI:
      {
         /* filling of Target RNC info not supported for this release */
         ie1 = (SztTgeteNB_ID *)(&ie->value.u.sztTgetID.val.cGI);
         break;
      }
      default:
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF," Invalid Target eNB ID.\n"));
#endif
         VE_FREE_EVNT(hovrReqdPdu);
         RETVALUE(RFAILED);
         break;
      }
   }/* End of switch(ie->value.u.sztTgetID.choice) */

   /* Filling Source to Target Transparent Container IE.  */
   cmMemset((U8 *)&srcToTgtCntr, 0, sizeof(SztSrceNB_ToTgeteNB_TprntCont));
   veFillTknU8(&srcToTgtCntr.pres, PRSNT_NODEF);
   veFillTknU8((TknU8 *)&srcToTgtCntr.rRC_Cont.pres, PRSNT_NODEF);
   SFndLenMsg(nhuEncCfmSdus->sdu.mBuf, (MsgLen *)&srcToTgtCntr.rRC_Cont.len);
   /* Allocate the memory for transparent containier IEs.*/
   if ((cmGetMem(&hovrReqdPdu->memCp, srcToTgtCntr.rRC_Cont.len,
               ((Ptr *)&srcToTgtCntr.rRC_Cont.val))) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqd: cmGetMem failed \
               for srcToTgtCntr.\n"));
#endif
      VE_FREE_EVNT(hovrReqdPdu);
      RETVALUE(RFAILED);
   }
   /* Copy the RRC encoded transparent container */
   SCpyMsgFix(nhuEncCfmSdus->sdu.mBuf, 0, srcToTgtCntr.rRC_Cont.len,\
         (Data *)srcToTgtCntr.rRC_Cont.val, &cnt);

   /* fill ERAB information */
   rbCnt = s1Con->ueCb->rbInfo.numOfRbCfgd;
   rbInfo = (VeUeRAB *) &s1Con->ueCb->rbInfo.rab;
   veFillTknU16(&(srcToTgtCntr.e_RABInformLst.noComp), rbCnt);
   /* Allocate the memory for transparent containier IEs.*/
   if ((cmGetMem(&hovrReqdPdu->memCp, \
               (rbCnt * sizeof(SztProtIE_SingleCont_E_RABInformLstIEs)),
               ((Ptr *)&srcToTgtCntr.e_RABInformLst.member))) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqd: cmGetMem failed \
               for ERAB info in srcToTgtCntr.\n"));
#endif
      VE_FREE_EVNT(hovrReqdPdu);
      RETVALUE(RFAILED);
   }
   for(rbIdx = 0; rbIdx < rbCnt; rbIdx++)
   {
      veFillTknU8(&(srcToTgtCntr.e_RABInformLst.member[rbIdx].pres), \
            PRSNT_NODEF);
      veFillTknU32(&(srcToTgtCntr.e_RABInformLst.member[rbIdx].id), \
            Sztid_E_RABInformLstItem);
      veFillTknU32(&(srcToTgtCntr.e_RABInformLst.member[rbIdx].criticality), \
            SztCriticalityignoreEnum);
      rbItem = &(srcToTgtCntr.e_RABInformLst.member[rbIdx].value.u.sztE_RABInformLstItem);
      veFillTknU8(&(rbItem->pres), PRSNT_NODEF);
      veFillTknU32(&(rbItem->e_RAB_ID), rbInfo[rbIdx].eRABId);
      if(rbInfo->rbDir == VE_DL || rbInfo->rbDir == VE_DL_UL)
      {
         veFillTknU32(&(rbItem->dL_Fwding), SztDL_FwdingdL_Fwding_proposedEnum);
      }
      else
      {
         rbItem->dL_Fwding.pres = NOTPRSNT;
      }
      rbItem->iE_Extns.noComp.pres = NOTPRSNT;
   }

   /* fill targetCell_ID */
   veFillTknU8(&srcToTgtCntr.targetCell_ID.pres, PRSNT_NODEF);
   len = 3;
   veFillTknStrOSXL(&(srcToTgtCntr.targetCell_ID.pLMNidentity), len, 
         (Data *)&nghCellCb->plmnId, &hovrReqdPdu);

   veFillTknBStr32(&(srcToTgtCntr.targetCell_ID.cell_ID), 28, nghCellCb->veCellPrp.cellId);
   srcToTgtCntr.targetCell_ID.iE_Extns.noComp.pres = NOTPRSNT;

   /* fill subscriberProfileIDforRFP */
   srcToTgtCntr.subscriberProfileIDforRFP.pres = NOTPRSNT;

   /* fill UE History */
   veFillTknU16(&(srcToTgtCntr.uE_HistoryInform.noComp), 1);
   /* Allocate the memory for transparent containier IEs.*/
   if ((cmGetMem(&hovrReqdPdu->memCp, \
               (srcToTgtCntr.uE_HistoryInform.noComp.val * \
                sizeof(SztLastVisitedCell_Item)), \
               ((Ptr *)&srcToTgtCntr.uE_HistoryInform.member))) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqd: cmGetMem failed \
               for UE History Info in srcToTgtCntr.\n"));
#endif
      VE_FREE_EVNT(hovrReqdPdu);
      RETVALUE(RFAILED);
   }
   for(idx = 0; idx < srcToTgtCntr.uE_HistoryInform.noComp.val; idx++)
   {
      lastVstCelInfo = &(srcToTgtCntr.uE_HistoryInform.member[idx]);
      veFillTknU8(&(lastVstCelInfo->choice), \
            LASTVISITEDCELL_ITEM_E_UTRAN_CELL);

      switch(lastVstCelInfo->choice.val)
      {
         case LASTVISITEDCELL_ITEM_E_UTRAN_CELL:
            veFillTknU8(&(lastVstCelInfo->val.e_UTRAN_Cell.pres), PRSNT_NODEF);
            veFillTknU8(&(lastVstCelInfo->val.e_UTRAN_Cell.global_Cell_ID.pres), 
                  PRSNT_NODEF);
            /* Fill the PLMN ID of global cell id */
            len = 3;
            veFillTknStrOSXL(&(lastVstCelInfo->val.e_UTRAN_Cell.global_Cell_ID.pLMNidentity), len, 
                  (Data *)&nghCellCb->plmnId, &hovrReqdPdu);
            /* Fill the E-UTRAN Cell Identifier of global cell id*/
            veFillTknBStr32(&(lastVstCelInfo->val.e_UTRAN_Cell.global_Cell_ID.cell_ID), 28, nghCellCb->veCellPrp.cellId);
            lastVstCelInfo->val.e_UTRAN_Cell.global_Cell_ID.iE_Extns.noComp.pres = NOTPRSNT;

            veFillTknU8(&(lastVstCelInfo->val.e_UTRAN_Cell.cellTyp.pres), PRSNT_NODEF);
            veFillTknU32(&(lastVstCelInfo->val.e_UTRAN_Cell.cellTyp.cell_Size),
                  2);
            lastVstCelInfo->val.e_UTRAN_Cell.cellTyp.iE_Extns.noComp.pres = NOTPRSNT;
            veFillTknU32(&(lastVstCelInfo->val.e_UTRAN_Cell.time_UE_StayedInCell),
                  100);
            lastVstCelInfo->val.e_UTRAN_Cell.iE_Extns.noComp.pres = NOTPRSNT;
         break;
      }
   }

   srcToTgtCntr.iE_Extns.noComp.pres = NOTPRSNT;

   SGetMsg(veCb.mem.region, veCb.mem.pool, &trnspContMBuf);

   if(trnspContMBuf == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqd: cmGetMem failed \
               for trnspContMBuf.\n"));
#endif
      VE_FREE_EVNT(hovrReqdPdu);
      RETVALUE(RFAILED);
   }

   /* Allocate the memory for S1 encTrpCnt */
   if ((cmGetMem(&hovrReqdPdu->memCp, \
                sizeof(S1apEncDecIE), \
               ((Ptr *)&encTrpCnt))) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqd: cmGetMem failed \
               for S1 ENCDEC structure.\n"));
#endif
      VE_FREE_EVNT(hovrReqdPdu);
      RETVALUE(RFAILED);
   }
   cmMemset((U8 *)&reqSts, 0, sizeof(SztReqStatus));
   encTrpCnt->iePtr = &srcToTgtCntr;

   asnErr.errCode = 0;
   ret = cmPAsnEncMsg((TknU8 *)(encTrpCnt->iePtr), /* CM_PASN_USE_MBUF */ 1,
         (Void *)(trnspContMBuf), /* SZ_PASN_VER_ZERO */ 0,
         (CmPAsnElmDef **) /* tmpszMsgIEDb[0]*/ /* szMsgIEDb[sztIeId] */ \
         szMsgIEDb_locTesting[0], &asnErr,
         &asnLen, /* dbgFlag */ TRUE, FALSE );
   if(ret != ROK)
   {
      /* Report the error incase if encoding fails */
      if (asnErr.errCode == CM_PASN_ESC_FUNC_FLD)
      {
         printf(" %s : Encoding failed, Abstract Syntax Error:Reason %d\n",\
               __func__, asnErr.errCode);
      }
      else
      {
         printf(" %s : Encoding failed, Transfer Syntax Error:Reason %d\n",\
               __func__, asnErr.errCode);
      }

      RETVALUE(RFAILED);
   }

   /* fill the transparent container into the ASN structure */

   ie = &hovrReqdPdu->pdu.val.initiatingMsg.value.u.sztHovrReqd.protocolIEs.
                                                              member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_Src_ToTget_TprntCont);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);

   /* Find the length of the Encoded TrgtToSrcTC */
   ret = SFndLenMsg(trnspContMBuf, &len);
   if((len == 0) || (ret != ROK))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veUtlSztFillHovrReqd: Incorrect buffer len %d",len));
      RETVALUE(RFAILED);
   }
   ie->value.u.sztSrc_ToTget_TprntCont.pres = 1;
   ie->value.u.sztSrc_ToTget_TprntCont.len = len;
   VE_ALLOC(&(ie->value.u.sztSrc_ToTget_TprntCont.val), len);
   SCpyMsgFix(trnspContMBuf,0,len,
         (ie->value.u.sztSrc_ToTget_TprntCont.val), &cnt); 

   SPutMsg(trnspContMBuf);

   *pdu = hovrReqdPdu;

   RETVALUE(ROK);
} /* veUtlSztFillHovrReqd */

/*
*
*       Fun:   veUtlSztFillHovrReqAck
*
*       Desc:  S1AP Fill Handover Request Ack
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_s1_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillHovrReqAck
(
VeS1ConCb           *s1Con,
S1apPdu             **pdu,
VeUeRABInfo         rabInfo,
NhuEncCfmSdus       *nhuEncCfmSdus
)
#else
PUBLIC S16 veUtlSztFillHovrReqAck(s1Con, pdu, rabInfo, nhuEncCfmSdus)
VeS1ConCb          *s1Con;
S1apPdu            **pdu;
VeUeRABInfo        rabInfo;
NhuEncCfmSdus      *nhuEncCfmSdus;
#endif
{
   S1apPdu   *hovrRqstAckgPdu = NULLP;
   S16        numComp = 0;
   S16        compCnt = 0;
   S16        idx = 0;
   S16        ret = 0;
   SztProtIE_Field_HovrRqstAckgIEs *ie = NULLP;
   SztProtIE_SingleCont_E_RABAdmtdItemIEs *ie1 = NULLP;
   MsgLen cnt = 0;
   MsgLen len = 0;
   SztTgeteNB_ToSrceNB_TprntCont tgtToSrcCnt;
   Buffer *trnspContMBuf = NULLP;
   S1apEncDecIE *encTrpCnt = NULLP;
   SztReqStatus reqSts;

   U32 asnLen = 0;
   CmPAsnErr    asnErr;
   CmPAsnElmDef *szMsgIEDb_locTesting[][4] = { {&szTgeteNB_ToSrceNB_TprntCont} };


   TRC2(veUtlSztFillHovrReqAck);
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlSztFillHovrReqAck:\
            Constructing HO Req Ack Message\n"));
#endif
   if(s1Con == NULLP || pdu == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqAck: Invalid Arguments.\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem, 
         (Ptr *)&hovrRqstAckgPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqAck:\
               cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&(hovrRqstAckgPdu->pdu), 0, sizeof(SztS1AP_PDU));
   /* Fill Interface Elementary Procedures for Handover Request Ack Msg.*/
   veFillTknU8(&(hovrRqstAckgPdu->pdu.choice), S1AP_PDU_SUCCESSFULOUTCOME);
   veFillTknU8(&(hovrRqstAckgPdu->pdu.val.successfulOutcome.pres), 
         PRSNT_NODEF);
   veFillTknU32(&(hovrRqstAckgPdu->pdu.val.successfulOutcome.procedureCode),
         Sztid_HovrResourceAllocn);
   veFillTknU32(&(hovrRqstAckgPdu->pdu.val.successfulOutcome.criticality), 
         SztCriticalityrejectEnum);
   numComp = 4;/* Indicates no of Mandatory fields are present. */
   veFillTknU8(&(hovrRqstAckgPdu->pdu.val.successfulOutcome.value.u.
            sztHovrRqstAckg.pres), PRSNT_NODEF);
   veFillTknU16(&(hovrRqstAckgPdu->pdu.val.successfulOutcome.value.u.
            sztHovrRqstAckg.protocolIEs.noComp), numComp);

   if ((cmGetMem(&hovrRqstAckgPdu->memCp, 
               (numComp * sizeof(SztProtIE_Field_HovrRqstAckgIEs)),
               (Ptr *)&hovrRqstAckgPdu->pdu.val.successfulOutcome.value.u.
               sztHovrRqstAckg.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqAck: cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(hovrRqstAckgPdu);
      RETVALUE(RFAILED);
   }
   /* Fill MME UE S1AP ID IE which is received from MME.*/
   ie = &hovrRqstAckgPdu->pdu.val.successfulOutcome.value.u.sztHovrRqstAckg.
      protocolIEs.member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1Con->mme_ue_s1ap_id);

   /* Fill ENB-UE-S1AP-ID which is allocated by target eNB. */
   ie = &hovrRqstAckgPdu->pdu.val.successfulOutcome.value.u.sztHovrRqstAckg.
      protocolIEs.member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1Con->enb_ue_s1ap_id);
   /*
    * Start of Comment.
    * Target eNB supports atleast one bearer. So, Admitted list 
    * should not be empty.
    * End of Comment.
    */
   /* Fill E-RAB admitted list. */
   ie = &hovrRqstAckgPdu->pdu.val.successfulOutcome.value.u.sztHovrRqstAckg.
      protocolIEs.member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_E_RABAdmtdLst);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   veFillTknU16(&(ie->value.u.sztE_RABAdmtdLst.noComp),
         rabInfo.numOfRbCfgd);

   if ((cmGetMem(&hovrRqstAckgPdu->memCp, 
               ( rabInfo.numOfRbCfgd * sizeof(SztProtIE_SingleCont_E_RABAdmtdItemIEs)),
               (Ptr *)&ie->value.u.sztE_RABAdmtdLst.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqAck:cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(hovrRqstAckgPdu);
      RETVALUE(RFAILED);
   }
   for(idx = 0; idx < rabInfo.numOfRbCfgd; idx++)
   {
      ie1 = &ie->value.u.sztE_RABAdmtdLst.member[idx];

      veFillTknU8(&(ie1->pres), PRSNT_NODEF);
      veFillTknU32(&(ie1->id), Sztid_E_RABAdmtdItem);
      veFillTknU32(&(ie1->criticality), SztCriticalityignoreEnum);
      veFillTknU8(&(ie1->value.u.sztE_RABAdmtdItem.pres), PRSNT_NODEF);
      veFillTknU32(&(ie1->value.u.sztE_RABAdmtdItem.e_RAB_ID), rabInfo.rab[idx].eRABId);

      if (CM_TPTADDR_IPV4 == veCb.datAppAddr.type)
      {
         len = 32; /* Length of the Transport layer address.*/
         ie1->value.u.sztE_RABAdmtdItem.transportLyrAddr.len = len;

         veFillTknStrBSXL(&(ie1->value.u.sztE_RABAdmtdItem.transportLyrAddr), 
               len, (Data *)&veCb.datAppAddr.u.ipv4TptAddr.address, &hovrRqstAckgPdu);

      }
      else
      {
         len = 128; /* Length of the Transport layer address.*/
         ie1->value.u.sztE_RABAdmtdItem.transportLyrAddr.len = len;

         veFillTknStrBSXL(&(ie1->value.u.sztE_RABAdmtdItem.transportLyrAddr), 
               len, (Data *)&veCb.datAppAddr.u.ipv6TptAddr.ipv6NetAddr, &hovrRqstAckgPdu);

      }
      len = 4;/* Length of the GTP Tunnel ID. */
      veFillTknStrOSXL1(&(ie1->value.u.sztE_RABAdmtdItem.gTP_TEID), len, 
            rabInfo.rab[idx].locEgtpTunId, &hovrRqstAckgPdu);
      /* Filling the Forward Tunnel Information */
      if(rabInfo.rab[idx].isDataFwdAvl == TRUE)
      {
         /* fill DL Fwd Tunnel information */
         if((rabInfo.rab[idx].rbDir == VE_DL) || (rabInfo.rab[idx].rbDir == VE_DL_UL))
         {
            if (CM_TPTADDR_IPV4 == veCb.datAppAddr.type)
            {
               len = 32; /* Length of the Transport layer address.*/
               ie1->value.u.sztE_RABAdmtdItem.dL_transportLyrAddr.len = len;

              veFillTknStrBSXL(&(ie1->value.u.sztE_RABAdmtdItem.dL_transportLyrAddr), 
                     len, (Data *)&veCb.datAppAddr.u.ipv4TptAddr.address, &hovrRqstAckgPdu);

            }
            else
            {
               len = 128; /* Length of the Transport layer address.*/
               ie1->value.u.sztE_RABAdmtdItem.dL_transportLyrAddr.len = len;

               veFillTknStrBSXL(&(ie1->value.u.sztE_RABAdmtdItem.dL_transportLyrAddr), 
                     len, (Data *)&veCb.datAppAddr.u.ipv6TptAddr.ipv6NetAddr, &hovrRqstAckgPdu);

            }
            len = 4;/* Length of the GTP Tunnel ID. */
            veFillTknStrOSXL1(&(ie1->value.u.sztE_RABAdmtdItem.dL_gTP_TEID), len, 
                  rabInfo.rab[idx].datFwdDlLocEgtpTunId, &hovrRqstAckgPdu);
         }
         /* fill UL Fwd Tunnel information */
         if((rabInfo.rab[idx].rbDir == VE_UL) || (rabInfo.rab[idx].rbDir == VE_DL_UL))
         {
            if (CM_TPTADDR_IPV4 == veCb.datAppAddr.type)
            {
               len = 4; /* Length of the Transport layer address.*/
               ie1->value.u.sztE_RABAdmtdItem.uL_TportLyrAddr.len = len;

               veFillTknStrBSXL(&(ie1->value.u.sztE_RABAdmtdItem.uL_TportLyrAddr), 
                     len, (Data *)&veCb.datAppAddr.u.ipv4TptAddr.address, &hovrRqstAckgPdu);

            }
            else
            {
               len = 16; /* Length of the Transport layer address.*/
               ie1->value.u.sztE_RABAdmtdItem.uL_TportLyrAddr.len = len;

               veFillTknStrBSXL(&(ie1->value.u.sztE_RABAdmtdItem.uL_TportLyrAddr), 
                     len, (Data *)&veCb.datAppAddr.u.ipv6TptAddr.ipv6NetAddr, &hovrRqstAckgPdu);

            }
            len = 4;/* Length of the GTP Tunnel ID. */
            veFillTknStrOSXL1(&(ie1->value.u.sztE_RABAdmtdItem.uL_GTP_TEID), len, 
                  rabInfo.rab[idx].datFwdUlLocEgtpTunId, &hovrRqstAckgPdu);
         }
      }
      ie1->value.u.sztE_RABAdmtdItem.iE_Extns.noComp.pres = NOTPRSNT;
   }/* End of for(idx = 0; idx < rabInfo.numOfRbCfgd; idx++)*/

   /* Filling the Target eNB to Source eNB Transparent Container IE.  */
   veFillTknU8(&(tgtToSrcCnt.pres), PRSNT_NODEF);
   /* Filling Source to Target Transparent Container IE.  */
   veFillTknU8(&tgtToSrcCnt.pres, PRSNT_NODEF);
   veFillTknU8((TknU8 *)&tgtToSrcCnt.rRC_Cont.pres, PRSNT_NODEF);
   SFndLenMsg(nhuEncCfmSdus->sdu.mBuf, (MsgLen *)&tgtToSrcCnt.rRC_Cont.len);
   /* Allocate the memory for transparent containier IEs.*/
   if ((cmGetMem(&hovrRqstAckgPdu->memCp, tgtToSrcCnt.rRC_Cont.len,
               ((Ptr *)&tgtToSrcCnt.rRC_Cont.val))) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqd: cmGetMem failed \
               for tgtToSrcCnt.\n"));
#endif
      VE_FREE_EVNT(hovrRqstAckgPdu);
      RETVALUE(RFAILED);
   }
   /* Copy the RRC encoded transparent container */
   SCpyMsgFix(nhuEncCfmSdus->sdu.mBuf, 0, tgtToSrcCnt.rRC_Cont.len,\
         (Data *)tgtToSrcCnt.rRC_Cont.val, &cnt);

   tgtToSrcCnt.iE_Extns.noComp.pres = NOTPRSNT;

   SGetMsg(veCb.mem.region, veCb.mem.pool, &trnspContMBuf);

   if(trnspContMBuf == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqAck: cmGetMem failed \
               for trnspContMBuf.\n"));
#endif
      VE_FREE_EVNT(hovrRqstAckgPdu);
      RETVALUE(RFAILED);
   }

   /* Allocate the memory for S1 encdec */
   if ((cmGetMem(&hovrRqstAckgPdu->memCp, \
               sizeof(S1apEncDecIE), \
               ((Ptr *)&encTrpCnt))) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrReqAck: cmGetMem failed \
               for S1 ENCDEC structure.\n"));
#endif
      VE_FREE_EVNT(hovrRqstAckgPdu);
      RETVALUE(RFAILED);
   }
   cmMemset((U8 *)&reqSts, 0, sizeof(SztReqStatus));
   encTrpCnt->iePtr = &tgtToSrcCnt;

   asnErr.errCode = 0;
   ret = cmPAsnEncMsg((TknU8 *)(encTrpCnt->iePtr), /* CM_PASN_USE_MBUF */ 1,
         (Void *)(trnspContMBuf), /* SZ_PASN_VER_ZERO */ 0,
         (CmPAsnElmDef **) /* tmpszMsgIEDb[0]*/ /* szMsgIEDb[sztIeId] */ \
         szMsgIEDb_locTesting[0], &asnErr,
         &asnLen, /* dbgFlag */ TRUE, FALSE );
   if(ret != ROK)
   {
      /* Report the error incase if encoding fails */
      if (asnErr.errCode == CM_PASN_ESC_FUNC_FLD)
      {
         printf(" %s : Encoding failed, Abstract Syntax Error:Reason %d\n",\
               __func__, asnErr.errCode);
      }
      else
      {
         printf(" %s : Encoding failed, Transfer Syntax Error:Reason %d\n",\
               __func__, asnErr.errCode);
      }
   }

   /* Filling the Target to Source Transparent Container IE.  */
   ie = &hovrRqstAckgPdu->pdu.val.successfulOutcome.value.u.sztHovrRqstAckg.
      protocolIEs.member[compCnt];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_Tget_ToSrc_TprntCont);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   /* Find the length of the Encoded TrgtToSrcTC */
   ret = SFndLenMsg(trnspContMBuf, &len);
   /*ret = SFndLenMsg(nhuEncCfmSdus->sdu.mBuf, &len);*/
   if((len == 0) || (ret != ROK))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "veUtlSztFillHovrReqAck: Incorrect buffer len %d",len));
      RETVALUE(RFAILED);
   }
   ie->value.u.sztTget_ToSrc_TprntCont.pres = 1;
   ie->value.u.sztTget_ToSrc_TprntCont.len = len;
   VE_ALLOC(&(ie->value.u.sztTget_ToSrc_TprntCont.val), len);
   SCpyMsgFix(trnspContMBuf,0,len,
         (ie->value.u.sztTget_ToSrc_TprntCont.val), &cnt);
   *pdu = hovrRqstAckgPdu;
   RETVALUE(ROK);
} /* veUtlSztFillHovrReqAck */

/*
*
*       Fun:   veUtlSztFillHovrNtfy 
*
*       Desc:  Call handler to fill Handover Notify message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillHovrNtfy
(
VeS1ConCb    *s1Con,
S1apPdu      **pdu,
VeCellCb     *cellCb
)
#else
PUBLIC S16 veUtlSztFillHovrNtfy(s1Con, pdu, cellCb)
VeS1ConCb    *s1Con;
S1apPdu      **pdu;
VeCellCb     *cellCb;
#endif
{
   S1apPdu   *hovrNtfyPdu = NULLP;
   S16        numComp = 0;
   S16        compCnt = 0;
   S16        ret = 0;
   U16        len = 0;
   SztProtIE_Field_HovrNtfyIEs *ie = NULLP;

   TRC2(veUtlSztFillHovrNtfy);

   if(s1Con == NULLP || pdu == NULLP || cellCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrNtfy: Invalid Arguments.\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem, 
                                                           (Ptr *)&hovrNtfyPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrNtfy cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&(hovrNtfyPdu->pdu), 0, sizeof(SztS1AP_PDU));
   /* Fill Interface Elementary Procedure for Handover Notify Message.*/
   veFillTknU8(&(hovrNtfyPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   veFillTknU8(&(hovrNtfyPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   veFillTknU32(&(hovrNtfyPdu->pdu.val.initiatingMsg.procedureCode), 
                                                        Sztid_HovrNotification);
   veFillTknU32(&(hovrNtfyPdu->pdu.val.initiatingMsg.criticality), 
                                                      SztCriticalityignoreEnum);
   numComp = 4;/* Indicates No of Mandatory fields .*/
   veFillTknU8(&(hovrNtfyPdu->pdu.val.initiatingMsg.value.u.sztHovrNtfy.pres), 
                                                                   PRSNT_NODEF);
   veFillTknU16(&(hovrNtfyPdu->pdu.val.initiatingMsg.value.u.sztHovrNtfy.
                                                  protocolIEs.noComp), numComp);

   if ((cmGetMem(&hovrNtfyPdu->memCp, (numComp * sizeof(SztProtIE_Field_HovrNtfyIEs)), 
             (Ptr *)&hovrNtfyPdu->pdu.val.initiatingMsg.value.u.sztHovrNtfy.
                                                     protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrNtfy:cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(hovrNtfyPdu);
      RETVALUE(RFAILED);
   }
   /* 
    * Start of Comment.
    * Fill MME-UE-S1AP-ID which is allcated by MME to uniquly identify the UE 
    * within the MME.
    * End of Comment.
    */
   ie = &hovrNtfyPdu->pdu.val.initiatingMsg.value.u.sztHovrNtfy.
                                                  protocolIEs.member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1Con->mme_ue_s1ap_id);
   /*
    * Start of Comment.
    * Filling ENB-UE-S1AP-ID which is allocated by target eNB to uniquly 
    * identify the UE within eNB.
    * End of Comment.
    */
   ie = &hovrNtfyPdu->pdu.val.initiatingMsg.value.u.sztHovrNtfy.
                                                  protocolIEs.member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1Con->enb_ue_s1ap_id);
   /*
    * Start of Comment. 
    * Fill EUTRAN-CGI which is used to globally identify a cell. It consist of 
    * PLMN-ID and Cell identity.
    * End of Comment.
    */
   ie = &hovrNtfyPdu->pdu.val.initiatingMsg.value.u.sztHovrNtfy.
                                                  protocolIEs.member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_EUTRAN_CGI);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.sztEUTRAN_CGI.pres), PRSNT_NODEF);
   
   /* Fill the PLMN Identity of E-UTRAN-CGI. */
   len = 3; /* Indicates length of the PLMN-ID.*/
   veFillTknStrOSXL(&(ie->value.u.sztEUTRAN_CGI.pLMNidentity), len, 
                                                (Data *)cellCb->plmnId[0], &hovrNtfyPdu);
   /* Fill the Cell Identity of E_UTRAN-CGI.*/
   len = 28;/* Indicates length of the Cell ID in terms of bits.*/
   veFillTknBStr32(&(ie->value.u.sztEUTRAN_CGI.cell_ID), len, cellCb->cellId);
   ie->value.u.sztEUTRAN_CGI.iE_Extns.noComp.pres = NOTPRSNT;
   /* 
    * Start of Comment.
    * Fill the  TAI which is used to uniquely identify a tracking area. 
    * It consist of PLMN-ID and TAC(Tracking Area Code).
    * End of Comment.
    */
   ie = &hovrNtfyPdu->pdu.val.initiatingMsg.value.u.sztHovrNtfy.protocolIEs.
                                                              member[compCnt];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);     
   veFillTknU32(&(ie->id), Sztid_TAI);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.sztTAI.pres), PRSNT_NODEF);
   /* Fill the PLMN-ID of TAI*/ 
   len = 3;/* Indicates length of the PLMN.*/
   veFillTknStrOSXL(&(ie->value.u.sztTAI.pLMNidentity), len, (Data *)cellCb->plmnId[0],
                                                                  &hovrNtfyPdu);
   /* Fill the TAC of the TAI*/
   len = 2;/* Indicates length of the TAC.*/
   veFillTknStr4(&(ie->value.u.sztTAI.tAC), (U8)len, cellCb->tac);
   ie->value.u.sztTAI.iE_Extns.noComp.pres = NOTPRSNT;

   *pdu = hovrNtfyPdu;

   RETVALUE(ROK);
} /* veUtlSztFillHovrNtfy */


/*
*
*       Fun:   veUtlSztFillENBStatusTfr 
*
*       Desc:  Call handler to fill ENB Status Transfer message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillENBStatusTfr
(
VeS1ConCb         *s1Con,
S1apPdu           **pdu,
U8                noOfBearer,
NhuPdcpSduStaCfm  *nhuPdcpSduStaCfm
)
#else
PUBLIC S16 veUtlSztFillENBStatusTfr(s1Con, pdu, noOfBearer, nhuPdcpSduStaCfm)
VeS1ConCb         *s1Con;
S1apPdu           **pdu;
U8                noOfBearer;
NhuPdcpSduStaCfm  *nhuPdcpSduStaCfm;
#endif
{
   S1apPdu   *eNBStatusTfrPdu = NULLP;
   S16        numComp = 0;
   S16        compCnt = 0;
   S16        ret = 0;
   S16        idx = 0;
   U32        pdcpHfn = 0;
   U16        pdcpSn = 0;
   U8         rbCnt = 0;
   U8         idx1 = 0;
   VeUeRAB    *rbInfo = NULLP;
   U8 snStsRbCnt = 0;
   Bool sndSNSts = FALSE;

   SztProtIE_Field_ENBStatusTfrIEs *ie = NULLP;
   SztProtIE_SingleCont_Brs_SubjToStatusTfr_ItemIEs *ie1 = NULLP;

   TRC2(veUtlSztFillENBStatusTfr);

   if(s1Con == NULLP || pdu == NULLP || nhuPdcpSduStaCfm == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillENBStatusTfr: Invalid Arguments.\n"));
#endif
      RETVALUE(RFAILED);
   }
   
   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem, 
                                                       (Ptr *)&eNBStatusTfrPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillENBStatusTfr: cmAllocEvnt \
                                                                    failed\n"));
#endif
      RETVALUE(RFAILED);
   }
   cmMemset((U8 *)&(eNBStatusTfrPdu->pdu), 0, sizeof(SztS1AP_PDU));
   /* Fill the Interface Elementory Procedure for eNB Status Transfer Message.*/
   veFillTknU8(&(eNBStatusTfrPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   veFillTknU8(&(eNBStatusTfrPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   veFillTknU32(&(eNBStatusTfrPdu->pdu.val.initiatingMsg.procedureCode), 
                                                           Sztid_eNBStatusTfr);
   veFillTknU32(&(eNBStatusTfrPdu->pdu.val.initiatingMsg.criticality), 
                                                      SztCriticalityignoreEnum);

   numComp = 3;/* Indicates No of Mandatory Fields.*/
   veFillTknU8(&(eNBStatusTfrPdu->pdu.val.initiatingMsg.value.u.
                                            sztENBStatusTfr.pres), PRSNT_NODEF);
   veFillTknU16(&(eNBStatusTfrPdu->pdu.val.initiatingMsg.value.u.
                                  sztENBStatusTfr.protocolIEs.noComp), numComp);
   if ((cmGetMem(&eNBStatusTfrPdu->memCp, 
        (numComp * sizeof(SztProtIE_Field_ENBStatusTfrIEs)), 
              (Ptr *)&eNBStatusTfrPdu->pdu.val.initiatingMsg.value.u.
                                     sztENBStatusTfr.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillENBStatusTfr:\
                                                           cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(eNBStatusTfrPdu);
      RETVALUE(RFAILED);
   }
   /* 
    * Start of Comments.
    * Fill the MME_UE_S1AP_ID which is allocated by MME to uniquely 
    * identify the UE within the MME.
    * End of Comments.
    */
   ie = &eNBStatusTfrPdu->pdu.val.initiatingMsg.value.u.sztENBStatusTfr.
                                                  protocolIEs.member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1Con->mme_ue_s1ap_id);
   /* 
    * Start of Comment.
    * Fill ENB_UE_S1AP_ID which is allocated by Source eNB to uniquely 
    * identify the UE within the eNB.
    * End of Comment. */
   ie = &eNBStatusTfrPdu->pdu.val.initiatingMsg.value.u.sztENBStatusTfr.
                                                  protocolIEs.member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1Con->enb_ue_s1ap_id);
   /* 
    * Start of Comment.
    * Fill eNB Status Transfer Transparent Container which contains Bearer ID,
    * UL Count and DL Count value.
    * End of comment.
    */
   ie = &eNBStatusTfrPdu->pdu.val.initiatingMsg.value.u.sztENBStatusTfr.
                                                    protocolIEs.member[compCnt];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_eNB_StatusTfr_TprntCont);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU8(&(ie->value.u.sztENB_StatusTfr_TprntCont.pres), PRSNT_NODEF);
   
   rbCnt = s1Con->ueCb->rbInfo.numOfRbCfgd;
   rbInfo = s1Con->ueCb->rbInfo.rab;
   for(idx = 0; idx < noOfBearer; idx++)
   {
      for(idx1 = 0; idx1 < rbCnt; idx1++)
      {
         if(nhuPdcpSduStaCfm->pdcpSduStaInfo[idx].drbId == rbInfo[idx1].rbId)
         {
            if(NHU_RLC_MODE_AM == rbInfo[idx1].rbMode)
            {
               snStsRbCnt++;
               break;
            }
         }
         else
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillENBStatusTfr: SduStsCfm Reveived for invalid RBID: %d.\n", 
             nhuPdcpSduStaCfm->pdcpSduStaInfo[idx].drbId));
#endif
            RETVALUE(RFAILED);
         }
      }/* End of for() */
   }/* End of for() */
   veFillTknU16(&(ie->value.u.sztENB_StatusTfr_TprntCont.
                                    bearers_SubjToStatusTfrLst.noComp),snStsRbCnt);
   if ((cmGetMem(&eNBStatusTfrPdu->memCp, 
        (snStsRbCnt * sizeof(SztBrs_SubjToStatusTfrLst)),
                     (Ptr *)&ie->value.u.sztENB_StatusTfr_TprntCont.
                                      bearers_SubjToStatusTfrLst.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillENBStatusTfr : cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(eNBStatusTfrPdu);
      RETVALUE(RFAILED);
   }
   snStsRbCnt = 0;
   for(idx = 0; idx < noOfBearer; idx++)
   {
      for(idx1 = 0; idx1 < rbCnt; idx1++)
      {
         if((nhuPdcpSduStaCfm->pdcpSduStaInfo[idx].drbId == rbInfo[idx1].rbId) && 
            (NHU_RLC_MODE_AM == rbInfo[idx1].rbMode))
         {
            sndSNSts = TRUE;
            break;
         }
      }
      if (TRUE != sndSNSts)
      {
         continue;
      }
      else
      {
         /* reset the flag */
         sndSNSts =  FALSE;
      }

      ie1= &ie->value.u.sztENB_StatusTfr_TprntCont.bearers_SubjToStatusTfrLst.
                                                                    member[snStsRbCnt++];
      veFillTknU8(&(ie1->pres),PRSNT_NODEF);
      veFillTknU32(&(ie1->id), Sztid_Brs_SubjToStatusTfr_Item);
      veFillTknU32(&(ie1->criticality),SztCriticalityignoreEnum);
      veFillTknU8(&(ie1->value.u.sztBrs_SubjToStatusTfr_Item.pres),
                                                                   PRSNT_NODEF);
      veFillTknU32(&(ie1->value.u.sztBrs_SubjToStatusTfr_Item.e_RAB_ID), \
                  rbInfo[idx1].eRABId);
      /* Fill UL_COUNT_VALUE */
      veFillTknU8(&(ie1->value.u.sztBrs_SubjToStatusTfr_Item.uL_COUNTvalue.
                                                             pres),PRSNT_NODEF);
      pdcpSn = ((nhuPdcpSduStaCfm->pdcpSduStaInfo[idx].ulCountVal) & (0x0fff));
      veFillTknU32(&(ie1->value.u.sztBrs_SubjToStatusTfr_Item.uL_COUNTvalue.
          pDCP_SN), pdcpSn);
      /* Fill HFN */
      pdcpHfn = ((nhuPdcpSduStaCfm->pdcpSduStaInfo[idx].ulCountVal >> 12) & (0xfffff));
      veFillTknU32(&(ie1->value.u.sztBrs_SubjToStatusTfr_Item.uL_COUNTvalue.
          hFN), pdcpHfn);

      veFillTknU8(&(ie1->value.u.sztBrs_SubjToStatusTfr_Item.dL_COUNTvalue.
                                                             pres),PRSNT_NODEF);
      /* Fill DL_COUNT_VALUE */
      pdcpSn = ((nhuPdcpSduStaCfm->pdcpSduStaInfo[idx].dlCountVal) & (0x0fff));
      veFillTknU32(&(ie1->value.u.sztBrs_SubjToStatusTfr_Item.dL_COUNTvalue.
          pDCP_SN), pdcpSn);
      pdcpHfn = ((nhuPdcpSduStaCfm->pdcpSduStaInfo[idx].dlCountVal >> 12) & (0xfffff));
      veFillTknU32(&(ie1->value.u.sztBrs_SubjToStatusTfr_Item.dL_COUNTvalue.
          hFN), pdcpHfn);
      /* selva MSPD-FIX */
      /* Fill receive status of UL SDU's */
      if(0 != nhuPdcpSduStaCfm->pdcpSduStaInfo[idx].rcvStaOfUlPdcpSdus.numBits)
      {
         /* selva MSPD-FIX */
         veFillTknStrBSXL(&(ie1->value.u.sztBrs_SubjToStatusTfr_Item.receiveStatusofULPDCPSDUs),\
            nhuPdcpSduStaCfm->pdcpSduStaInfo[idx].rcvStaOfUlPdcpSdus.numBits,\
            nhuPdcpSduStaCfm->pdcpSduStaInfo[idx].rcvStaOfUlPdcpSdus.ulBitMap,\
            &eNBStatusTfrPdu);
      } 
   }/* End of for(idx = 0; idx < sztBrs_SubjToStatusTfrLst->noComp; idx++)*/
   ie->value.u.sztENB_StatusTfr_TprntCont.iE_Extns.noComp.pres = NOTPRSNT;

   *pdu = eNBStatusTfrPdu;

   RETVALUE(ROK);
} /* veUtlSztFillENBStatusTfr */

/*
*
*       Fun:   veUtlSztFillHovrFail 
*
*       Desc:  Call handler to fill Handover Failure message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillHovrFail
(
VeS1ConCb    *s1Con,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillHovrFail(s1Con, pdu)
VeS1ConCb    *s1Con;
S1apPdu      **pdu;
#endif
{
   S1apPdu   *hovrFailPdu = NULLP;
   S16        numComp = 0;
   S16        compCnt = 0;
   S16        ret = 0;
   SztProtIE_Field_HovrFailIEs *ie = NULLP;

   TRC2(veUtlSztFillHovrFail);

   if(s1Con == NULLP || pdu == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrFail : Invalid Arguments.\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem, 
                                                           (Ptr *)&hovrFailPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrFail:\
                                                    cmAllocEvnt is Failed \n"));
#endif
      RETVALUE(RFAILED);
   }
   cmMemset((U8 *)&(hovrFailPdu->pdu),0, sizeof(SztS1AP_PDU));
   /* Fill the Interface Elementary Procedure for Handover Failure Message.*/
   veFillTknU8(&(hovrFailPdu->pdu.choice), S1AP_PDU_UNSUCCESSFULOUTCOME);
   veFillTknU8(&(hovrFailPdu->pdu.val.unsuccessfulOutcome.pres), PRSNT_NODEF);
   veFillTknU32(&(hovrFailPdu->pdu.val.unsuccessfulOutcome.procedureCode), 
                                                      Sztid_HovrResourceAllocn);
   veFillTknU32(&(hovrFailPdu->pdu.val.unsuccessfulOutcome.criticality), 
                                                      SztCriticalityrejectEnum);
   numComp = 2;/* Indicates no of mandatory fields*/
   veFillTknU8(&(hovrFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztHovrFail.
                                                            pres), PRSNT_NODEF);
   veFillTknU16(&(hovrFailPdu->pdu.val.unsuccessfulOutcome.value.u.
                                      sztHovrFail.protocolIEs.noComp), numComp);

   if ((cmGetMem(&hovrFailPdu->memCp, (numComp * sizeof(SztProtIE_Field_HovrFailIEs)), 
             (Ptr *)&hovrFailPdu->pdu.val.unsuccessfulOutcome.value.u.
                                         sztHovrFail.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillHovrFail:cmGetMem Failed \n"));
#endif
      VE_FREE_EVNT(hovrFailPdu);
      RETVALUE(RFAILED);
   }

   ie = &hovrFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztHovrFail.
                                                  protocolIEs.member[compCnt++];
   /*
    * Start of Comment.
    * Fill MME_UE_S1AP_ID which is allocated by MME to uniquely identify the 
    * UE within the MME.
    * End of Comment.
    */
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1Con->mme_ue_s1ap_id);

   ie = &hovrFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztHovrFail.
                                                    protocolIEs.member[compCnt];
   /* Fill Cause value.It indicates reason why HO is failed.*/
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_Cause);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.sztCause.choice), s1Con->cause.choice.val);
   switch (s1Con->cause.choice.val)
   {
      case CAUSE_RADIONW:
         veFillTknU32(&(ie->value.u.sztCause.val.radioNw), s1Con->cause.val.radioNw.val);
         break;
      case CAUSE_TRANSPORT:
         veFillTknU32(&(ie->value.u.sztCause.val.transport), s1Con->cause.val.transport.val);
         break;
      case CAUSE_NAS:
         veFillTknU32(&(ie->value.u.sztCause.val.nas), s1Con->cause.val.nas.val);
         break;
      case 3:/* TODO CAUSE_PROTOCOL*/
         veFillTknU32(&(ie->value.u.sztCause.val.protocol), s1Con->cause.val.protocol.val);
         break;
      case 4:/*TODO CAUSE_MISC*/
         veFillTknU32(&(ie->value.u.sztCause.val.misc), s1Con->cause.val.misc.val);
         break;
      default:
         veFillTknU8(&(ie->value.u.sztCause.choice), CAUSE_RADIONW);
         veFillTknU32(&(ie->value.u.sztCause.val.radioNw), 
                                                SztCauseRadioNwunspecifiedEnum);
         break;
   }/* End of switch(cause.choice*/

   *pdu = hovrFailPdu;

   RETVALUE(ROK);
} /* veUtlSztFillHovrFail */


/*
*
*       Fun:   veUtlSztFillPathSwRqst 
*
*       Desc:  Call handler to fill Path Switch Request message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  szac_sztutl.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillPathSwRqst
(
VeS1ConCb    *s1Con,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillPathSwRqst(s1Con, pdu)
VeS1ConCb    *s1Con;
S1apPdu      **pdu;
#endif
{
   S1apPdu   *pathSwRqstPdu = NULLP;
   S16        numComp = 0;
   S16        compCnt = 0;
   S16        idx = 0;
   S16        ret = 0;
   U16        len = 0;
   SztProtIE_Field_PathSwRqstIEs *ie = NULLP;
   SztProtIE_SingleCont_E_RABToBeSwedDLItemIEs *ie1 = NULLP;
   VeCellCb  *cellCb = NULLP;

   TRC2(veUtlSztFillPathSwRqst);

   if(s1Con == NULLP || pdu == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillPathSwRqst : Invalid Arguments.\n"));
#endif
      RETVALUE(RFAILED);
   }

   cellCb = veCb.cellCb[0];

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem, 
                                                         (Ptr *)&pathSwRqstPdu);
   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillPathSwRqst\
                                                    cmAllocEvnt is Failed \n"));
#endif
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&(pathSwRqstPdu->pdu), 0, sizeof(SztS1AP_PDU));
   /* Fill the Interface Elementory Procedures */
   veFillTknU8(&(pathSwRqstPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   veFillTknU8(&(pathSwRqstPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   veFillTknU32(&(pathSwRqstPdu->pdu.val.initiatingMsg.procedureCode), 
                                                              Sztid_PathSwRqst);
   veFillTknU32(&(pathSwRqstPdu->pdu.val.initiatingMsg.criticality), 
                                                      SztCriticalityrejectEnum);

   numComp = 6;/* Indicates no of Manadatory fields are present.*/
   veFillTknU8(&(pathSwRqstPdu->pdu.val.initiatingMsg.value.u.
                                              sztPathSwRqst.pres), PRSNT_NODEF);
   veFillTknU16(&(pathSwRqstPdu->pdu.val.initiatingMsg.value.u.
                                    sztPathSwRqst.protocolIEs.noComp), numComp);

   if ((cmGetMem(&pathSwRqstPdu->memCp,(numComp * sizeof(SztProtIE_Field_PathSwRqstIEs)), 
                  (Ptr *)&pathSwRqstPdu->pdu.val.initiatingMsg.value.u.
                                       sztPathSwRqst.protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
       VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillPathSwRqst:cmGetMem Failed \n"));
#endif
       VE_FREE_EVNT(pathSwRqstPdu);
       RETVALUE(RFAILED); 
   }
   ie = &pathSwRqstPdu->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.
                                                              member[compCnt++];
   /* Fill the eNB UE S1AP ID */
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1Con->enb_ue_s1ap_id);

   ie = &pathSwRqstPdu->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.
                                                              member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_E_RABToBeSwedDLLst); 
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   /* Fill the no of bearers are present in the List.*/
   numComp = s1Con->ueCb->rbInfo.numOfRbCfgd;
   veFillTknU16(&(ie->value.u.sztE_RABToBeSwedDLLst.noComp), numComp);

   if ((cmGetMem(&pathSwRqstPdu->memCp, 
           (numComp * sizeof(SztProtIE_SingleCont_E_RABToBeSwedDLItemIEs)), 
                       (Ptr *)&ie->value.u.sztE_RABToBeSwedDLLst.member)) !=ROK)
   {
#ifdef DEBUGP
       VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillPathSwRqst:cmGetMem Failed \n"));
#endif
       VE_FREE_EVNT(pathSwRqstPdu);
       RETVALUE(RFAILED);
   }
   /* Fill all the members of E_RABToBeSwedList.*/
   for(idx = 0; idx < numComp; idx++)
   {
      ie1 = &ie->value.u.sztE_RABToBeSwedDLLst.member[idx];
      veFillTknU8(&(ie1->pres), PRSNT_NODEF);
      veFillTknU32(&(ie1->id), Sztid_E_RABToBeSwedDLItem); 
      veFillTknU32(&(ie1->criticality), SztCriticalityrejectEnum);
      /* Fill the E-RAB_ID */
      veFillTknU8(&(ie1->value.u.sztE_RABToBeSwedDLItem.pres), PRSNT_NODEF);
      veFillTknU32(&(ie1->value.u.sztE_RABToBeSwedDLItem.e_RAB_ID), 
                                           s1Con->ueCb->rbInfo.rab[idx].eRABId);
      if (CM_IPV4ADDR_TYPE == s1Con->ueCb->rbInfo.rab[idx].sgwAddr.type)
      {
         len = 32;
         veFillTknStrBSXL(&(ie1->value.u.sztE_RABToBeSwedDLItem.transportLyrAddr),
                        len,(Data *)&veCb.datAppAddr.u.ipv4TptAddr.address,
                          &pathSwRqstPdu); 
      }
      else
      {
         len = 128;
         veFillTknStrBSXL(&(ie1->value.u.sztE_RABToBeSwedDLItem.transportLyrAddr),
                        len,(Data *)&veCb.datAppAddr.u.ipv4TptAddr.address,
                         &pathSwRqstPdu); 
      }
      len = 4;
      veFillTknStrOSXL(&(ie1->value.u.sztE_RABToBeSwedDLItem.gTP_TEID), len, 
         (Data *)&s1Con->ueCb->rbInfo.rab[idx].locEgtpTunId, &pathSwRqstPdu);
      ie1->value.u.sztE_RABToBeSwedDLItem.iE_Extns.noComp.pres = NOTPRSNT;
   }  

   ie = &pathSwRqstPdu->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.
                                                              member[compCnt++];
   /* Fill the MME-UE-S1AP-ID */
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_SrcMME_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1Con->ueCb->src_mme_ue_s1ap_id);

   ie = &pathSwRqstPdu->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.
                                                              member[compCnt++];
   /* Fill the EUTRAN CGI*/
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_EUTRAN_CGI);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.sztEUTRAN_CGI.pres), PRSNT_NODEF);
   len = 3;
   veFillTknStrOSXL(&(ie->value.u.sztEUTRAN_CGI.pLMNidentity), len, 
                                             (Data *)cellCb->plmnId[0], &pathSwRqstPdu);
   len = 28;
   veFillTknBStr32(&(ie->value.u.sztEUTRAN_CGI.cell_ID), len, cellCb->cellId);
   ie->value.u.sztEUTRAN_CGI.iE_Extns.noComp.pres = NOTPRSNT;
   /* Fill the TAI which contains PLMN and TAC*/
   ie = &pathSwRqstPdu->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.
                                                              member[compCnt++];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);     
   veFillTknU32(&(ie->id), Sztid_TAI);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.sztTAI.pres), PRSNT_NODEF);     
   len = 3;
   veFillTknStrOSXL(&(ie->value.u.sztTAI.pLMNidentity), len, 
                                             (Data *)cellCb->plmnId[0], &pathSwRqstPdu);
   len = 2;
   veFillTknStr4(&(ie->value.u.sztTAI.tAC), (U8)len, cellCb->tac);
   ie->value.u.sztTAI.iE_Extns.noComp.pres = NOTPRSNT;
   /* Fill UE Security capabilities.*/
   ie = &pathSwRqstPdu->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.
                                                              member[compCnt];
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_UESecurCapabilities);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.sztUESecurCapabilities.pres), PRSNT_NODEF);
   len = 16;
   veFillTknBStr32(&(ie->value.u.sztUESecurCapabilities.encryptionAlgorithms), 
                                            len, *(s1Con->ueCb->secCfg.cpCiphKey));
   len = 16;
   veFillTknBStr32(&(ie->value.u.sztUESecurCapabilities.
                integrityProtectionAlgorithms),len,*(s1Con->ueCb->secCfg.intgKey));
   ie->value.u.sztUESecurCapabilities.iE_Extns.noComp.pres = NOTPRSNT;

   *pdu = pathSwRqstPdu;
   RETVALUE(ROK);
}

/*
*
*       Fun:   veUtlSztFillCnclReq
*
*       Desc:  Call handler to fill Handover Cancel message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_s1_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillCnclReq
(
S1apPdu    **pdu,
VeS1ConCb  *s1Con
)
#else
PUBLIC S16 veUtlSztFillCnclReq(pdu, s1Con)
S1apPdu    **pdu;
VeS1ConCb  *s1Con;
#endif
{
   S1apPdu   *hovrCancelPdu = NULLP;
   S16        numComp;
   S16        ret;
   U8         choice;
   U32        cause;
   U8         compCnt = 0;
   SztProtIE_Field_HovrCancelIEs *ie;

   TRC2(veUtlSztFillCnclReq);

   if(s1Con == NULLP || pdu == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillCnclReq : Invalid Arguments.\n"));
#endif
      RETVALUE(RFAILED);
   }


   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem, (Ptr *)&hovrCancelPdu);
   if(ret != ROK)
   {
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&(hovrCancelPdu->pdu), 0, sizeof(SztS1AP_PDU));

   veFillTknU8(&(hovrCancelPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   veFillTknU8(&(hovrCancelPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   veFillTknU32(&(hovrCancelPdu->pdu.val.initiatingMsg.procedureCode), Sztid_HovrCancel);
   veFillTknU32(&(hovrCancelPdu->pdu.val.initiatingMsg.criticality), SztCriticalityrejectEnum);

   {
      numComp = 3;
      veFillTknU8(&(hovrCancelPdu->pdu.val.initiatingMsg.value.u.sztHovrCancel.pres), PRSNT_NODEF);
      veFillTknU16(&(hovrCancelPdu->pdu.val.initiatingMsg.value.u.sztHovrCancel.protocolIEs.noComp), numComp);

      if ((cmGetMem(&hovrCancelPdu->memCp, (numComp * sizeof(SztProtIE_Field_HovrCancelIEs)), 
             (Ptr *)&hovrCancelPdu->pdu.val.initiatingMsg.value.u.sztHovrCancel.protocolIEs.member)) !=ROK)
      {
         RETVALUE(RFAILED);
      }

      {

         ie = &hovrCancelPdu->pdu.val.initiatingMsg.value.u.sztHovrCancel.protocolIEs.member[compCnt++];

         veFillTknU8(&(ie->pres), PRSNT_NODEF);
         veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
         veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
         veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID),  s1Con->mme_ue_s1ap_id);

         ie = &hovrCancelPdu->pdu.val.initiatingMsg.value.u.sztHovrCancel.protocolIEs.member[compCnt++];

         veFillTknU8(&(ie->pres), PRSNT_NODEF);
         veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
         veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
         veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID),  s1Con->enb_ue_s1ap_id);

         ie = &hovrCancelPdu->pdu.val.initiatingMsg.value.u.sztHovrCancel.protocolIEs.member[compCnt];

         veFillTknU8(&(ie->pres), PRSNT_NODEF);
         veFillTknU32(&(ie->id), Sztid_Cause);
         veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
         choice = CAUSE_RADIONW;
         cause  = SztCauseRadioNwunspecifiedEnum;
         
         switch (choice)
         {
            case CAUSE_RADIONW:
               veFillTknU8(&(ie->value.u.sztCause.choice), choice);
               veFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
               break;
            case CAUSE_TRANSPORT:
               veFillTknU8(&(ie->value.u.sztCause.choice), choice);
               veFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
               break;
            case CAUSE_NAS:
               veFillTknU8(&(ie->value.u.sztCause.choice), choice);
               veFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
               break;
            case S1AP_CAUSE_PROTOCOL:
               veFillTknU8(&(ie->value.u.sztCause.choice), choice);
               veFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
               break;
            case S1AP_CAUSE_MISC:
               veFillTknU8(&(ie->value.u.sztCause.choice), choice);
               veFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
               break;
           default:
               veFillTknU8(&(ie->value.u.sztCause.choice), CAUSE_RADIONW);
               veFillTknU32(&(ie->value.u.sztCause.val.radioNw), SztCauseRadioNwunspecifiedEnum);
               break;
         }
      }
   }

   *pdu = hovrCancelPdu;

   RETVALUE(ROK);
} /* end of veUtlSztFillCnclReq */


/*
*
*       Fun:   veFillErabFrmS1HoReq
*
*       Desc:  Call handler to fill RAB info in UECB from S1 HO Request message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_s1_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillErabFrmS1HoReq
(
VeUeRABInfo *rbInfo,
SztE_RABToBeSetupLstHOReq *erabLst,
VeMmeCb      *mmeCb
)
#else
PUBLIC S16 veFillErabFrmS1HoReq(rbInfo, erabLst, mmeCb)
VeUeRABInfo *rbInfo;
SztE_RABToBeSetupLstHOReq *erabLst;
VeMmeCb      *mmeCb;
#endif
{
   U8   idx = 0;
   U8   idx1 = 0;
   U8   shiftBits = 0;
   U16  ipAddrIdx = 0;
   U32  addrMsk = 0;

   SztE_RABToBeSetupItemHOReq *item = NULLP;

   TRC2(veFillErabFrmS1HoReq);

   if(rbInfo == NULLP || erabLst == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillErabFrmS1HoReq : Invalid Arguments.\n"));
#endif
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < erabLst->noComp.val; idx++)
   {
      item = &erabLst->member[idx].value.u.sztE_RABToBeSetupItemHOReq;
      if (PRSNT_NODEF == item->pres.pres)
      {
         for(idx1 = 0; idx1 < idx; idx1++)
         {
            if(rbInfo->rab[idx1].eRABId == item->e_RAB_ID.val)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veFillErabFrmS1HoReq : \
                        Multiple ERAB Id [id = %ld].\n",item->e_RAB_ID.val));
#endif
               RETVALUE(RFAILED);
            }
         } /* end of for(idx1...( */

         rbInfo->rab[idx].eRABId = item->e_RAB_ID.val;

         rbInfo->rab[idx].rbType = CM_LTE_DRB;
         rbInfo->rab[idx].rbId = VE_GET_RB_ID_FRM_ERAB(rbInfo->rab[idx].eRABId);
         rbInfo->rab[idx].pdcpId = rbInfo->rab[idx].rbId;
         rbInfo->rab[idx].qci = item->e_RABlevelQosParams.qCI.val;

         VE_GET_U32_FRM_OSXL(rbInfo->rab[idx].mbr.dl, \
               item->e_RABlevelQosParams.gbrQosInform.e_RAB_MaxBitrateDL);

         VE_GET_U32_FRM_OSXL(rbInfo->rab[idx].mbr.ul, \
               item->e_RABlevelQosParams.gbrQosInform.e_RAB_MaxBitrateUL);

         VE_GET_U32_FRM_OSXL(rbInfo->rab[idx].gbr.dl, \
               item->e_RABlevelQosParams.gbrQosInform.e_RAB_GuaranteedBitrateDL);

         VE_GET_U32_FRM_OSXL(rbInfo->rab[idx].gbr.ul, \
               item->e_RABlevelQosParams.gbrQosInform.e_RAB_GuaranteedBitrateUL);

         /* TODO : Determine the rbDir and rbMode */
#ifndef VE_UM_MODE
         rbInfo->rab[idx].rbDir = VE_DL_UL;
         rbInfo->rab[idx].rbMode = NHU_RLC_MODE_AM;
#else
#ifndef  VE_UM_MODE_BI_DIR
         rbInfo->rab[idx].rbDir = VE_DL;
#else
         rbInfo->rab[idx].rbDir = VE_DL_UL;
#endif
         rbInfo->rab[idx].rbMode = NHU_RLC_MODE_UM;
#endif

         VE_ALLOC(&(rbInfo->rab[idx].remEgtpTunId.val), (item->gTP_TEID.len));

         VE_SWAP_BYTES_U32((*(U32 *)item->gTP_TEID.val));
         /* Copy Byte by Byte */
         cmMemcpy(rbInfo->rab[idx].remEgtpTunId.val, item->gTP_TEID.val, \
               item->gTP_TEID.len);

         rbInfo->rab[idx].remEgtpTunId.len = item->gTP_TEID.len;


         /* retrieve Serv. GW Address later will be used for tunnel creation */
         switch(item->transportLyrAddr.len)
         {
            case 32:
               rbInfo->rab[idx].sgwAddr.type = CM_TPTADDR_IPV4;
               rbInfo->rab[idx].sgwAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

               VE_SWAP_BYTES_U32((*(U32 *)item->transportLyrAddr.val));
               rbInfo->rab[idx].sgwAddr.u.ipv4TptAddr.address = 
                  *(U32 *)item->transportLyrAddr.val;

               break;

            case 128:
               rbInfo->rab[idx].sgwAddr.type = CM_TPTADDR_IPV6;
               rbInfo->rab[idx].sgwAddr.u.ipv6TptAddr.port = VE_DFLT_EGTP_PORT; 
               for (ipAddrIdx = 0; ipAddrIdx < 16; ipAddrIdx++)
               {
                  rbInfo->rab[idx].sgwAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = 
                     item->transportLyrAddr.val[ipAddrIdx];
               }
               break;

            default:
               /* would be in case if len = 160 bits which means support for both ipv4 and
                * ipv6 */
               break;
         }

         /* Store local Tunnel Id */
         VE_GET_LOC_TUNNEL_ID((mmeCb)->locEgtpTunCntr);
         rbInfo->rab[idx].locEgtpTunId = (mmeCb)->locEgtpTunCntr;

         /* TODO : determine from the incoming transparent container.
          * currently setting data forwarding flag to TRUE */
         rbInfo->rab[idx].isDataFwdAvl = TRUE;
         if(rbInfo->rab[idx].isDataFwdAvl == TRUE)
         {
            switch(rbInfo->rab[idx].rbDir)
            {
               case VE_DL:
                  /* Store local DL Fwd Tunnel Id */
                  VE_GET_LOC_TUNNEL_ID((mmeCb)->locEgtpTunCntr);
                  rbInfo->rab[idx].datFwdDlLocEgtpTunId = (mmeCb)->locEgtpTunCntr;
                  /* setting the remote tunnel information */
                  /* NOTE */
                  /* Since there will not be any forwarding of data,
                   * from Tget and SGW, there will not be any remote tunnel 
                   * info for dat fwding, present in the HO REQ message.
                   * And EGTP needs both loc and rem tunnel info to 
                   * create a tunnel.
                   * So we are filling a dummy remote tunnel value 
                   * (normal tunnel id),
                   * just for the sake of creating the tunnel. */
                  VE_ALLOC(&(rbInfo->rab[idx].remDlDataFwdTeId.val), \
                        (rbInfo->rab[idx].remEgtpTunId.len));

                  /* Copy Byte by Byte */
                  cmMemcpy(rbInfo->rab[idx].remDlDataFwdTeId.val, \
                        rbInfo->rab[idx].remEgtpTunId.val, \
                        rbInfo->rab[idx].remEgtpTunId.len);

                  rbInfo->rab[idx].remDlDataFwdTeId.len = rbInfo->rab[idx].remEgtpTunId.len;

                  switch(item->transportLyrAddr.len)
                  {
                     case 32:
                        rbInfo->rab[idx].datFwdPeerAddr.type = CM_TPTADDR_IPV4;
                        rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

                        memcpy(&(rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.address), \
                              &(rbInfo->rab[idx].sgwAddr.u.ipv4TptAddr.address),\
                              sizeof(rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.address));
                        break;

                     case 128:
                        rbInfo->rab[idx].datFwdPeerAddr.type = CM_TPTADDR_IPV6;
                        rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

                        for (ipAddrIdx = 0; ipAddrIdx < 16; ipAddrIdx++)
                        {
                           rbInfo->rab[idx].datFwdPeerAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = \
                                   rbInfo->rab[idx].sgwAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx];
                        }
                        break;

                     default:
                        /* would be in case if len = 160 bits which means support for both ipv4 and
                         * ipv6 */
                        break;
                  }

                  break;

               case VE_UL:
                  /* Store local UL Fwd Tunnel Id */
                  VE_GET_LOC_TUNNEL_ID((mmeCb)->locEgtpTunCntr);
                  rbInfo->rab[idx].datFwdUlLocEgtpTunId = (mmeCb)->locEgtpTunCntr;
                  /* setting the remote tunnel information */
                  /* NOTE */
                  /* Since there will not be any forwarding of data,
                   * from Tget and SGW, there will not be any remote tunnel 
                   * info for dat fwding, present in the HO REQ message.
                   * And EGTP needs both loc and rem tunnel info to 
                   * create a tunnel.
                   * So we are filling a dummy remote tunnel value 
                   * (normal tunnel id),
                   * just for the sake of creating the tunnel. */
                  VE_ALLOC(&(rbInfo->rab[idx].remUlDataFwdTeId.val), \
                        (rbInfo->rab[idx].remEgtpTunId.len));

                  /* Copy Byte by Byte */
                  cmMemcpy(rbInfo->rab[idx].remUlDataFwdTeId.val, \
                        rbInfo->rab[idx].remEgtpTunId.val, \
                        rbInfo->rab[idx].remEgtpTunId.len);

                  rbInfo->rab[idx].remUlDataFwdTeId.len = rbInfo->rab[idx].remEgtpTunId.len;

                  switch(item->transportLyrAddr.len)
                  {
                     case 32:
                        rbInfo->rab[idx].datFwdPeerAddr.type = CM_TPTADDR_IPV4;
                        rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

                        memcpy((void *)&(rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.address), \
                              (void *)sizeof(rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.address), \
                              rbInfo->rab[idx].sgwAddr.u.ipv4TptAddr.address);
                        break;

                     case 128:
                        rbInfo->rab[idx].datFwdPeerAddr.type = CM_TPTADDR_IPV6;
                        rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

                        for (ipAddrIdx = 0; ipAddrIdx < 16; ipAddrIdx++)
                        {
                           rbInfo->rab[idx].datFwdPeerAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = \
                                   rbInfo->rab[idx].sgwAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx];
                        }
                        break;

                     default:
                        /* would be in case if len = 160 bits which means support for both ipv4 and
                         * ipv6 */
                        break;
                  }

                  break;

               case VE_DL_UL:
               default:
                  /* Store local DL Fwd Tunnel Id */
                  VE_GET_LOC_TUNNEL_ID((mmeCb)->locEgtpTunCntr);
                  rbInfo->rab[idx].datFwdDlLocEgtpTunId = (mmeCb)->locEgtpTunCntr;

                  /* Store local UL Fwd Tunnel Id */
                  VE_GET_LOC_TUNNEL_ID((mmeCb)->locEgtpTunCntr);
                  rbInfo->rab[idx].datFwdUlLocEgtpTunId = (mmeCb)->locEgtpTunCntr;

                  /* setting the remote tunnel information */
                  /* NOTE */
                  /* Since there will not be any forwarding of data,
                   * from Tget and SGW, there will not be any remote tunnel 
                   * info for dat fwding, present in the HO REQ message.
                   * And EGTP needs both loc and rem tunnel info to 
                   * create a tunnel.
                   * So we are filling a dummy remote tunnel value 
                   * (normal tunnel id),
                   * just for the sake of creating the tunnel. */
                  VE_ALLOC(&(rbInfo->rab[idx].remDlDataFwdTeId.val), \
                        (rbInfo->rab[idx].remEgtpTunId.len));

                  /* Copy Byte by Byte */
                  cmMemcpy(rbInfo->rab[idx].remDlDataFwdTeId.val, \
                        rbInfo->rab[idx].remEgtpTunId.val, \
                        rbInfo->rab[idx].remEgtpTunId.len);

                  rbInfo->rab[idx].remDlDataFwdTeId.len = rbInfo->rab[idx].remEgtpTunId.len;

                  VE_ALLOC(&(rbInfo->rab[idx].remUlDataFwdTeId.val), \
                        (rbInfo->rab[idx].remEgtpTunId.len));

                  /* Copy Byte by Byte */
                  cmMemcpy(rbInfo->rab[idx].remUlDataFwdTeId.val, \
                        rbInfo->rab[idx].remEgtpTunId.val, \
                        rbInfo->rab[idx].remEgtpTunId.len);

                  rbInfo->rab[idx].remUlDataFwdTeId.len = rbInfo->rab[idx].remEgtpTunId.len;

                  switch(item->transportLyrAddr.len)
                  {
                     case 32:
                        rbInfo->rab[idx].datFwdPeerAddr.type = CM_TPTADDR_IPV4;
                        rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

                        memcpy(&(rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.address), \
                              &(rbInfo->rab[idx].sgwAddr.u.ipv4TptAddr.address),\
                              sizeof(rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.address));
                        break;

                     case 128:
                        rbInfo->rab[idx].datFwdPeerAddr.type = CM_TPTADDR_IPV6;
                        rbInfo->rab[idx].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

                        for (ipAddrIdx = 0; ipAddrIdx < 16; ipAddrIdx++)
                        {
                           rbInfo->rab[idx].datFwdPeerAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = \
                                                                                              rbInfo->rab[idx].sgwAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx];
                        }
                        break;

                     default:
                        /* would be in case if len = 160 bits which means support for both ipv4 and
                         * ipv6 */
                        break;
                  }

                  break;
            }
         }
      }
   }
   rbInfo->numOfRbCfgd = erabLst->noComp.val;
   
   RETVALUE(ROK);
} /* end of veFillErabFrmS1HoReq */

/*
*
*       Fun:   veFillErabFrmS1HoAck
*
*       Desc:  Call handler to fill RAB info in UECB from S1 HO Request Acknowledgement message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_s1_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillErabFrmS1HoAck
(
VeUeRABInfo *rbInfo,
SztS1AP_PDU *s1HoAckPdu,
VeMmeCb     *mmeCb
)
#else
PUBLIC S16 veFillErabFrmS1HoAck(rbInfo, s1HoAckPdu, mmeCb)
VeUeRABInfo *rbInfo;
SztS1AP_PDU *s1HoAckPdu;
VeMmeCb     *mmeCb;
#endif
{
   U8 erabId = 0;
   U8 rbId = 0;
   SztE_RABSubjtoDataFwdingLst *eRABLst = NULLP;
   SztE_RABDataFwdingItem *eRABItem = NULLP;
   U8   shiftBits = 0;
   U16  ipAddrIdx = 0;
   U32  addrMsk = 0;
   Bool errFlag = FALSE;

   TRC2(veFillErabFrmS1HoAck);

   /* fetch the ERAB Admitted list from the ACK PDU */
   veSzGetIE(s1HoAckPdu, Sztid_E_RABSubjtoDataFwdingLst, (TknU8 **) &eRABLst);

   if(eRABLst == NULLP)
   {
      for (rbId = 0; rbId < rbInfo->numOfRbCfgd; rbId++)
      {
         if(rbInfo->rab[rbId].isDataFwdAvl == TRUE)
         {
            errFlag = TRUE;
         }
      }
      if(errFlag == TRUE)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veFillErabFrmS1HoAck : \
                  Data Forwarding information missing.\n"));
#endif
      }
      RETVALUE(RFAILED);
   }

   /* fill the FWD tunnel information from the ERAB list */
   for(erabId = 0; erabId < eRABLst->noComp.val; erabId++)
   {
      eRABItem = &eRABLst[erabId].member->value.u.sztE_RABDataFwdingItem;
      for (rbId = 0; rbId < rbInfo->numOfRbCfgd; rbId++)
      {
         if(rbInfo->rab[rbId].eRABId == eRABItem->e_RAB_ID.val)
         {
            /* check if FWD tunel is already configured for this ERAB */
            if(rbInfo->rab[rbId].remDlDataFwdTeId.val != NULLP && 
                  rbInfo->rab[rbId].remUlDataFwdTeId.val != NULLP)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veFillErabFrmS1HoAck : \
                        Multiple entry of same ERAB ID[id = %ld].\n",eRABItem->e_RAB_ID.val));
#endif
               RETVALUE(RFAILED);
            }

            /* Copy DL Tunnel info */
            if(eRABItem->dL_gTP_TEID.pres == PRSNT_NODEF)
            {
               VE_ALLOC(&(rbInfo->rab[rbId].remDlDataFwdTeId.val), \
                     (eRABItem->dL_gTP_TEID.len));

               rbInfo->rab[rbId].remDlDataFwdTeId.len = eRABItem->dL_gTP_TEID.len;
               VE_SWAP_BYTES_U32((*(U32 *)eRABItem->dL_gTP_TEID.val));               

               /* Copy Byte by Byte */
               cmMemcpy(rbInfo->rab[rbId].remDlDataFwdTeId.val, \
                     eRABItem->dL_gTP_TEID.val, eRABItem->dL_gTP_TEID.len);

               /* retrieve Serv. GW Address later will be used for tunnel creation */
               switch(eRABItem->dL_transportLyrAddr.len)
               {
                  case 32:
                     rbInfo->rab[rbId].datFwdPeerAddr.type = CM_TPTADDR_IPV4;
                     rbInfo->rab[rbId].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 


               VE_SWAP_BYTES_U32((*(U32 *)eRABItem->dL_transportLyrAddr.val)); 
               rbInfo->rab[rbId].datFwdPeerAddr.u.ipv4TptAddr.address = *(U32 *)eRABItem->dL_transportLyrAddr.val;              
                     break;

                  case 128:
                     rbInfo->rab[rbId].datFwdPeerAddr.type = CM_TPTADDR_IPV6;
                     rbInfo->rab[rbId].datFwdPeerAddr.u.ipv6TptAddr.port = VE_DFLT_EGTP_PORT; 
                     for (ipAddrIdx = 0; ipAddrIdx < 16; ipAddrIdx++)
                     {
                        rbInfo->rab[rbId].datFwdPeerAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = 
                           eRABItem->dL_transportLyrAddr.val[ipAddrIdx];
                     }
                     break;

                  default:
                     /* would be in case if len = 160 bits which means support for both ipv4 and
                      * ipv6 */
                     break;
               }
               /* Store local DL Fwd Tunnel Id */
               VE_GET_LOC_TUNNEL_ID((mmeCb)->locEgtpTunCntr);
               rbInfo->rab[rbId].datFwdDlLocEgtpTunId = (mmeCb)->locEgtpTunCntr;
            }
            /* Copy UL Tunnel info */
            if(eRABItem->uL_GTP_TEID.pres == PRSNT_NODEF)
            {
               VE_ALLOC(&(rbInfo->rab[rbId].remUlDataFwdTeId.val), \
                     (eRABItem->uL_GTP_TEID.len));

               rbInfo->rab[rbId].remUlDataFwdTeId.len = eRABItem->uL_GTP_TEID.len;
               VE_SWAP_BYTES_U32((*(U32 *)eRABItem->uL_GTP_TEID.val));               

               /* Copy Byte by Byte */
               cmMemcpy(rbInfo->rab[rbId].remUlDataFwdTeId.val, \
                     eRABItem->uL_GTP_TEID.val, eRABItem->uL_GTP_TEID.len);

               /* if DL Fwding was not proposed by S-eNB and UL Fwding was proposed by T-eNB, 
                  then also set isDataFwdAvl flag to TRUE */
               rbInfo->rab[rbId].isDataFwdAvl = TRUE;

               /* retrieve Serv. GW Address later will be used for tunnel creation */
               switch(eRABItem->uL_TportLyrAddr.len)
               {
                  case 32:
                     rbInfo->rab[rbId].datFwdPeerAddr.type = CM_TPTADDR_IPV4;
                     rbInfo->rab[rbId].datFwdPeerAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 


               VE_SWAP_BYTES_U32((*(U32 *)eRABItem->uL_TportLyrAddr.val));               
               rbInfo->rab[rbId].datFwdPeerAddr.u.ipv4TptAddr.address = *(U32 *)eRABItem->uL_TportLyrAddr.val;
                     break;

                  case 128:
                     rbInfo->rab[rbId].datFwdPeerAddr.type = CM_TPTADDR_IPV6;
                     rbInfo->rab[rbId].datFwdPeerAddr.u.ipv6TptAddr.port = VE_DFLT_EGTP_PORT; 
                     for (ipAddrIdx = 0; ipAddrIdx < 16; ipAddrIdx++)
                     {
                        rbInfo->rab[rbId].datFwdPeerAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = 
                           eRABItem->uL_TportLyrAddr.val[ipAddrIdx];
                     }
                     break;

                  default:
                     /* would be in case if len = 160 bits which means support for both ipv4 and
                      * ipv6 */
                     break;
               }
               /* Store local UL Fwd Tunnel Id */
               VE_GET_LOC_TUNNEL_ID((mmeCb)->locEgtpTunCntr);
               rbInfo->rab[rbId].datFwdUlLocEgtpTunId = (mmeCb)->locEgtpTunCntr;
            }
         }
      }
   }

   RETVALUE(ROK);
} /* end of veFillErabFrmS1HoAck */


/*
*
*       Fun:   veUpdUlTunPthSwtAck
*
*       Desc:  Call handler to Update UL Tunnel Information UECB from 
*              S1 Path Switch Request Acknowledgement message.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_s1_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veUpdUlTunPthSwtAck
(
VeUeRABInfo *rbInfo,
SztS1AP_PDU *s1PthSwtAckPdu
)
#else
PUBLIC S16 veUpdUlTunPthSwtAck(rbInfo, s1PthSwtAckPdu)
VeUeRABInfo *rbInfo;
SztS1AP_PDU *s1PthSwtAckPdu;
#endif
{
   U8 erabId = 0;
   U8 rbId = 0;
   SztE_RABToBeSwedULLst *eRABLst = NULLP;
   SztE_RABToBeSwedULItem *eRABItem = NULLP;
   U8   shiftBits = 0;
   U16  ipAddrIdx = 0;
   U32  addrMsk = 0;

   TRC2(veUpdUlTunPthSwtAck);

   /* fetch the ERAB Admitted list from the Path Switch Ack PDU */
   if(ROK != veSzGetIE(s1PthSwtAckPdu, Sztid_E_RABToBeSwedULLst, (TknU8 **) &eRABLst))
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUpdUlTunPthSwtAck : \
                        Failed to veSzGetIE.\n"));
#endif
      RETVALUE(RFAILED);
   }

   if (eRABLst == NULLP)
   {
      /* NO change in UL tunnel information */
      RETVALUE(ROK);
   }
   /* fill the FWD tunnel information from the ERAB list */
   for(erabId = 0; erabId < eRABLst->noComp.val; erabId++)
   {
      eRABItem = &eRABLst[erabId].member->value.u.sztE_RABToBeSwedULItem;
      for (rbId = 0; rbId < rbInfo->numOfRbCfgd; rbId++)
      {
         if(rbInfo->rab[rbId].eRABId == eRABItem->e_RAB_ID.val)
         {
            /* Copy UL Tunnel info */
            if(eRABItem->gTP_TEID.pres == PRSNT_NODEF)
            {
               VE_ALLOC(&(rbInfo->rab[rbId].remEgtpTunId.val), \
                     (eRABItem->gTP_TEID.len));

               /* Copy Byte by Byte */
               cmMemcpy(rbInfo->rab[rbId].remEgtpTunId.val, \
                     eRABItem->gTP_TEID.val, eRABItem->gTP_TEID.len);

               rbInfo->rab[rbId].remEgtpTunId.val =  (U8 *) &eRABItem->gTP_TEID.len;

               /* retrieve Serv. GW Address later will be used for tunnel creation */
               switch(eRABItem->transportLyrAddr.len)
               {
                  case 32:
                     rbInfo->rab[rbId].sgwAddr.type = CM_TPTADDR_IPV4;
                     rbInfo->rab[rbId].sgwAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

                     /* copy 4bytes into the U32 */
                     shiftBits = eRABItem->transportLyrAddr.len / 8;
                     addrMsk = 0xFF000000;
                     for(ipAddrIdx = 0; ipAddrIdx < 4; ipAddrIdx++)
                     {
                        shiftBits--;
                        rbInfo->rab[rbId].sgwAddr.u.ipv4TptAddr.address |=
                           ((U32)(eRABItem->transportLyrAddr.val[ipAddrIdx] << (8 * shiftBits)) & addrMsk);
                        addrMsk = addrMsk >> 8;
                     }
                     break;

                  case 128:
                     rbInfo->rab[rbId].sgwAddr.type = CM_TPTADDR_IPV6;
                     rbInfo->rab[rbId].sgwAddr.u.ipv6TptAddr.port = VE_DFLT_EGTP_PORT; 
                     for (ipAddrIdx = 0; ipAddrIdx < 16; ipAddrIdx++)
                     {
                        rbInfo->rab[rbId].sgwAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = 
                           eRABItem->transportLyrAddr.val[ipAddrIdx];
                     }
                     break;

                  default:
                     /* would be in case if len = 160 bits which means support for both ipv4 and
                      * ipv6 */
                     break;
               }
            }
         }
      }
   }

   RETVALUE(ROK);
} /* end of veUpdUlTunPthSwtAck */

#endif /* End of LTE_HO_SUPPORT */



/*
*
*       Fun:   veSztFilMmeCfgUpdAck 
*
*       Desc:  Fill MME Configuration Update ACK
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  
*
*/
#ifdef ANSI
PUBLIC S16 veSztFilMmeCfgUpdAck
(
S1apPdu      **pdu
)
#else
PUBLIC S16 veSztFilMmeCfgUpdAck(pdu)
S1apPdu      **pdu;
#endif
{

   S16        ret;
   S1apPdu   *mmeCfgUpdAck;

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem,
         (Ptr *)&mmeCfgUpdAck);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSztFilMmeCfgUpdAck\
               cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   veFillTknU8(&(mmeCfgUpdAck->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   veFillTknU8(&(mmeCfgUpdAck->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   veFillTknU32(&(mmeCfgUpdAck->pdu.val.successfulOutcome.procedureCode),
                  Sztid_MMEConfigUpd);
   veFillTknU32(&(mmeCfgUpdAck->pdu.val.successfulOutcome.criticality),
                  SztCriticalityrejectEnum);
   veFillTknU8(&(mmeCfgUpdAck->pdu.val.successfulOutcome.value.u.sztMMEConfigUpdAckg.pres),
              PRSNT_NODEF);
   veFillTknU16(&(mmeCfgUpdAck->pdu.val.successfulOutcome.value.u.sztMMEConfigUpdAckg.protocolIEs.noComp),
                0);

   *pdu = mmeCfgUpdAck;

   RETVALUE(ROK);
}

/*
*
*       Fun:   veUtlSztFillEnbCfgUpd
*
*       Desc:  Construct S1 Setup Message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c  
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillEnbCfgUpd
(
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillEnbCfgUpd(pdu)
S1apPdu      **pdu;
#endif
{
   S1apPdu *szEnbCfgUpd = NULLP;
   U16     numComp;
   S16     ret;
   U16     len;
   U8      pLMNId[10];
   VeCellCb *cellCb = NULLP;
   SztProtIE_Field_ENBConfigUpdIEs *ie;
   SztSuppTAs_Item *tACItem;

   TRC2(veUtlSztFillEnbCfgUpd);
   cellCb = veCb.cellCb[0];
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veUtlSztFillS1SetupReq:\
            Constructing S1 Setup Message\n"));
#endif

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem, 
         (Ptr *)&szEnbCfgUpd);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillS1SetupReq:\
               cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   /*veFillTkn functions will be referred from sz_utl.c file*/
   veFillTknU8(&(szEnbCfgUpd->pdu.choice), SZT_TRGR_INTI_MSG);
   veFillTknU8(&(szEnbCfgUpd->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   veFillTknU32(&(szEnbCfgUpd->pdu.val.initiatingMsg.procedureCode),
         Sztid_ENBConfigUpd);
   veFillTknU32(&(szEnbCfgUpd->pdu.val.initiatingMsg.criticality),
         SztCriticalityrejectEnum);

   veFillTknU8(&(szEnbCfgUpd->pdu.val.initiatingMsg.value.u.sztENBConfigUpd.\
            pres), PRSNT_NODEF);

   /* Filling 2 IEs */
   numComp = 2;
   veFillTknU16(&(szEnbCfgUpd->pdu.val.initiatingMsg.value.u.sztENBConfigUpd.\
            protocolIEs.noComp), numComp);

   /* Get memory for numComp IEs */
   if ((cmGetMem(szEnbCfgUpd, numComp * sizeof(SztProtIE_Field_ENBConfigUpdIEs),\
               (Ptr*)&szEnbCfgUpd->pdu.val.initiatingMsg.value.u.sztENBConfigUpd.\
               protocolIEs.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillEnbCfgUpd:\
               cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(szEnbCfgUpd);
      RETVALUE(RFAILED);
   }

   /* IE3 - Filling Supported TAs */
   ie = &szEnbCfgUpd->pdu.val.initiatingMsg.value.u.sztENBConfigUpd.\
        protocolIEs.member[0];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_SuppTAs);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);

   numComp = 1;
   veFillTknU16(&(ie->value.u.sztSuppTAs.noComp), numComp);

   /* IE3 - Allocate memory for TA items */
   if ((cmGetMem(szEnbCfgUpd, numComp*sizeof(SztSuppTAs_Item), 
               (Ptr*)&(ie->value.u.sztSuppTAs.member))) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillEnbCfgUpd:\
               cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(szEnbCfgUpd);
      RETVALUE(RFAILED);
   }

   /* IE3 - Filling TA item 1 */
   tACItem = &ie->value.u.sztSuppTAs.member[0];
   veFillTknU8(&(tACItem->pres), PRSNT_NODEF);

   /* IE3 - Filling tAC */
   len = 2;
   /* TAC  */
   tACItem->tAC.pres = PRSNT_NODEF;
   tACItem->tAC.len = (U8)len;

   /*TAC values are updated from System Related Information*/
   tACItem->tAC.val[1] = (U8)(cellCb->sysRelInfo.trackAreaCode & 0xff);
   tACItem->tAC.val[0] = (U8)((cellCb->sysRelInfo.trackAreaCode & 0xff00)>>8);

   /* IE3 - Filling broadcastPLMNs */
   numComp = 1;
   /* Allocate memory for broadcast PLMNs */
   if((cmGetMem(szEnbCfgUpd, numComp * sizeof(SztPLMNidentity), \
               (Ptr*)&(tACItem->broadcastPLMNs.member))) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillEnbCfgUpd:\
               cmGetMem failed\n"));
#endif
      VE_FREE_EVNT(szEnbCfgUpd);
      RETVALUE(RFAILED);
   }

   veFillTknU16(&(tACItem->broadcastPLMNs.noComp), numComp);

   len = 3;
   /* Broadcast PLMN */
   tACItem->broadcastPLMNs.member[0].pres = PRSNT_NODEF;
   tACItem->broadcastPLMNs.member[0].len = len;
   VE_GET_MEM(szEnbCfgUpd, len, &(tACItem->broadcastPLMNs.member[0].val));

   cmMemset(pLMNId,0,sizeof(pLMNId));

   if(cellCb->sysRelInfo.mnc[2].pres == PRSNT_NODEF )
   {
      pLMNId[0] =(((cellCb->sysRelInfo.mcc[1].val)<<4)| (cellCb->sysRelInfo.mcc[0].val));
      pLMNId[1] =(((cellCb->sysRelInfo.mnc[0].val)<<4)| (cellCb->sysRelInfo.mcc[2].val));
      pLMNId[2] =(((cellCb->sysRelInfo.mnc[2].val)<<4)| (cellCb->sysRelInfo.mnc[1].val));
   }
   else
   {
      /*Addition of Filler digit as 0x0f plus two digits of MNC*/
      pLMNId[0] =(((cellCb->sysRelInfo.mcc[1].val)<<4)| (cellCb->sysRelInfo.mcc[0].val));
      pLMNId[1] =(((0x0f)<<4)| (cellCb->sysRelInfo.mcc[2].val));
      pLMNId[2] =(((cellCb->sysRelInfo.mnc[1].val)<<4)| (cellCb->sysRelInfo.mnc[0].val));
   }
   veFillTknStrOSXL(&(tACItem->broadcastPLMNs.member[0]), len, pLMNId, &szEnbCfgUpd);

   /*Filling the PagDRX */
    ie = &szEnbCfgUpd->pdu.val.initiatingMsg.value.u.sztENBConfigUpd.protocolIEs.member[1];
   
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_DefaultPagDRX);
   
   veFillTknU32(&(ie->value.u.sztPagDRX), SztPagDRXv32Enum);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);

   *pdu = szEnbCfgUpd;

   RETVALUE(ROK);

} /* veUtlSztFillEnbCfgUpd */

/*
*
*       Fun:   veUtlSztFillReset
*
*       Desc:  Construct Reset Message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c  
*
*/
#ifdef ANSI
PUBLIC S16 veUtlFillReset
(
S1apPdu      **pdu,
U8           rstType,
U16          numCons,
U8           choice,
U32          cause,
VeS1ConId   *conIds
)
#else
PUBLIC S16 veUtlFillReset(pdu, rstType, numCons, choice, cause, conIds)
S1apPdu      **pdu;
U8           rstType;
U16          numCons;
U8           choice;
U32          cause;
VeS1ConId   *conIds;
#endif
{
   S1apPdu     *reset;
   S16         ret;
   U16         noComp;
   U16         idx;
   SztProtIE_Field_ResetIEs *ie;
   SztProtIE_SingleCont_UE_assocLogS1_ConItemRes *ie1;

   TRC2(veUtlSztFillReset);

   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &veCb.mem, 
                     (Ptr *)&reset);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillReset: cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Fill the PDU with the procedure informarion */
   veFillTknU8(&(reset->pdu.choice), SZT_TRGR_INTI_MSG);
   veFillTknU8(&(reset->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   veFillTknU32(&(reset->pdu.val.initiatingMsg.procedureCode), Sztid_Reset);
   veFillTknU32(&(reset->pdu.val.initiatingMsg.criticality),
                 SztCriticalityrejectEnum);

   veFillTknU8(&(reset->pdu.val.initiatingMsg.value.u.sztReset.pres),
                 PRSNT_NODEF);

   noComp = 2;

   veFillTknU16(&(reset->pdu.val.initiatingMsg.value.u.sztReset.protocolIEs.\
                  noComp), noComp);

   if ((cmGetMem(reset, (noComp * sizeof(SztProtIE_Field_ResetIEs)),
                 (Ptr *)&reset->pdu.val.initiatingMsg.value.u.sztReset.\
                 protocolIEs.member)) != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillReset: cmGetMem failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   ie = &(reset->pdu.val.initiatingMsg.value.u.sztReset.protocolIEs.\
          member[0]);

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_Cause);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

   switch (choice)
   {
      case CAUSE_RADIONW:
         veFillTknU8(&(ie->value.u.sztCause.choice), choice);
         veFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
         break;
      case CAUSE_TRANSPORT:
         veFillTknU8(&(ie->value.u.sztCause.choice), choice);
         veFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
         break;
      case CAUSE_NAS:
         veFillTknU8(&(ie->value.u.sztCause.choice), choice);
         veFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
         break;
      case CAUSE_MISC:
         veFillTknU8(&(ie->value.u.sztCause.choice), choice);
         veFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
         break;
   }

   ie = &(reset->pdu.val.initiatingMsg.value.u.sztReset.protocolIEs.\
         member[1]);

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_ResetTyp);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);

   if(rstType == RESETTYP_S1_INTF)
   {
      veFillTknU8(&(ie->value.u.sztResetTyp.choice), RESETTYP_S1_INTF);
      veFillTknU32(&(ie->value.u.sztResetTyp.val.s1_Intf),
                     SztResetAllreset_allEnum);
   }
   else
   {
      /* Partial reset, fill the S1AP eNB ids */
      noComp = numCons;
      veFillTknU8(&(ie->value.u.sztResetTyp.choice), RESETTYP_PARTOFS1_INTF);
      veFillTknU16(&(ie->value.u.sztResetTyp.val.partOfS1_Intf.noComp), 
                    noComp);

      if (cmGetMem(reset,
             (noComp * sizeof(SztProtIE_SingleCont_UE_assocLogS1_ConItemRes)),
             (Ptr *)&(ie->value.u.sztResetTyp.val.partOfS1_Intf.member)) !=ROK)
      {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillReset: cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
      }

      for(idx = 0; idx < noComp; idx++)
      {
         ie1 = &(ie->value.u.sztResetTyp.val.partOfS1_Intf.member[idx]);

         veFillTknU8(&(ie1->pres), PRSNT_NODEF);
         veFillTknU32(&(ie1->id), SZT_ELM_ID_UE_ASSOC_LOG_S1_CON_ITM);
         veFillTknU32(&(ie1->criticality), SztCriticalityignoreEnum);
         veFillTknU8(&(ie1->value.u.sztUE_assocLogS1_ConItem.pres), 
                     PRSNT_NODEF);
         if(conIds[idx].type == VE_S1AP_ENB_ID)
         {
            veFillTknU32(&(ie1->value.u.sztUE_assocLogS1_ConItem.eNB_UE_S1AP_ID),
                  conIds[idx].conId);
            ie1->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.pres = NOTPRSNT;
         }
         else if(conIds[idx].type == VE_S1AP_MME_ID)
         {
            veFillTknU32(&(ie1->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID),
                  conIds[idx].conId);
            ie1->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.pres = NOTPRSNT;
         }
         else
         {
            ie1->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.pres = NOTPRSNT;
            ie1->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.pres = NOTPRSNT;
         }
         ie1->value.u.sztUE_assocLogS1_ConItem.iE_Extns.noComp.pres = FALSE;
      }
   }

   *pdu = reset;

   RETVALUE(ROK);
}

/*
*
*       Fun:   veUtlSztFillErrInd
*
*       Desc:  Construct Error Indicaiton Message
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c  
*
*/
#ifdef ANSI
PUBLIC S16 veUtlFillErrInd
(
S1apPdu      **pdu,
U32          cause,
U8           choice,
U32          *eNbUeId,
U32          *mmeUeId
)
#else
PUBLIC S16 veUtlFillErrInd(pdu, cause, choice, eNbUeId, mmeUeId)
S1apPdu      **pdu;
U32          cause;
U8          choice;
U32          *eNbUeId;
U32          *mmeUeId;
#endif
{

   U16                         noComp = 1;
   S1apPdu                    *errPdu;
   SztProtIE_Field_ErrIndIEs  *ie;
   U16                         idx = 0;
   S16                         ret;

   if(eNbUeId != NULLP)
   {
      noComp++;
   }
   if(mmeUeId != NULLP)
   {
      noComp++;
   }

   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &veCb.mem, 
                     (Ptr *)&errPdu);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillErrInd: cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Fill the PDU with the procedure informarion */
   veFillTknU8(&(errPdu->pdu.choice), SZT_TRGR_INTI_MSG);
   veFillTknU8(&(errPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   veFillTknU32(&(errPdu->pdu.val.initiatingMsg.procedureCode), Sztid_ErrInd);
   veFillTknU32(&(errPdu->pdu.val.initiatingMsg.criticality),
                 SztCriticalityrejectEnum);

   veFillTknU8(&(errPdu->pdu.val.initiatingMsg.value.u.sztErrInd.pres),
                 PRSNT_NODEF);

   veFillTknU16(&(errPdu->pdu.val.initiatingMsg.value.u.sztErrInd.protocolIEs.\
                  noComp), noComp);

   if ((cmGetMem(errPdu, (noComp * sizeof(SztProtIE_Field_ErrIndIEs)),
                 (Ptr *)errPdu->pdu.val.initiatingMsg.value.u.sztErrInd.\
                 protocolIEs.member)) != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillErrInd: cmGetMem failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   ie = &(errPdu->pdu.val.initiatingMsg.value.u.sztErrInd.protocolIEs.\
          member[idx]);

   if(mmeUeId != NULLP)
   {
      veFillTknU8(&(ie->pres), PRSNT_NODEF);
      veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
      veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
      veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), *mmeUeId);
      idx++;
   }

   ie = &(errPdu->pdu.val.initiatingMsg.value.u.sztErrInd.protocolIEs.\
          member[idx]);

   if(eNbUeId != NULLP)
   {
      veFillTknU8(&(ie->pres), PRSNT_NODEF);
      veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
      veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
      veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), *eNbUeId);
      idx++;
   }

   ie = &(errPdu->pdu.val.initiatingMsg.value.u.sztErrInd.protocolIEs.\
          member[idx]);

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_Cause);
   veFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   veFillTknU8(&(ie->value.u.sztCause.choice), choice);

   switch(choice)
   {
      case CAUSE_RADIONW:
         veFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
         break;

      case CAUSE_TRANSPORT:
         veFillTknU32(&(ie->value.u.sztCause.val.transport), cause);
         break;

      case CAUSE_NAS:
         veFillTknU32(&(ie->value.u.sztCause.val.nas), cause);
         break;

      case CAUSE_MISC:
         veFillTknU32(&(ie->value.u.sztCause.val.misc), cause);
         break;

      default:
         veFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
         break;
   }

   *pdu = errPdu;

   RETVALUE(ROK);
}

/*
*
*       Fun:   veUtlBuildUeCntxModRsp
*
*       Desc:  Construct UE Context Modification Response
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c  
*
*/
#ifdef ANSI
PUBLIC S16 veUtlFillUeCntxModRsp
(
S1apPdu      **pdu,
U32          eNbId,
U32          mmeId
)
#else
PUBLIC S16 veUtlFillUeCntxModRsp(pdu, eNbId, mmeId)
S1apPdu      **pdu;
U32          eNbId;
U32          mmeId;
#endif
{
   S1apPdu      *ueCntxModRsp = NULLP;
   S16           ret;
   U16           numComp;
   SztProtIE_Field_UECntxtModificationRespIEs   *ie;

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem, 
         (Ptr *)&ueCntxModRsp);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlBuildUeCntxModRsp:\
               cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Fill the procedure code */
   veFillTknU8(&(ueCntxModRsp->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   veFillTknU8(&(ueCntxModRsp->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   veFillTknU32(&(ueCntxModRsp->pdu.val.successfulOutcome.procedureCode),
         Sztid_UECntxtModification);
   veFillTknU32(&(ueCntxModRsp->pdu.val.successfulOutcome.criticality),
         SztCriticalityrejectEnum);

   veFillTknU8(&(ueCntxModRsp->pdu.val.successfulOutcome.value.u.\
                sztUECntxtModificationResp.pres), PRSNT_NODEF);

   /* Filling 2 IEs mmeId and eNb Id */
   numComp = 2;
   veFillTknU16(&(ueCntxModRsp->pdu.val.successfulOutcome.value.u.\
                 sztUECntxtModificationResp.protocolIEs.noComp), numComp);
/* Prasanna given patch patch_ue_cntx_mod.txt */
    if ((cmGetMem(ueCntxModRsp, (numComp * sizeof(SztProtIE_Field_UECntxtModificationRespIEs)),
                  (Ptr*)&(ueCntxModRsp->pdu.val.successfulOutcome.value.u.sztUECntxtModificationResp.\
                  protocolIEs.member))) != ROK)
    {          
 #ifdef DEBUGP
       VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillErrInd: cmGetMem failed\n"));
 #endif
       RETVALUE(RFAILED);
    }           


   ie = &(ueCntxModRsp->pdu.val.successfulOutcome.value.u.\
         sztUECntxtModificationResp.protocolIEs.member[0]);

   veFillTknU8(&(ie->pres), PRSNT_NODEF);     
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), mmeId);

   ie = &(ueCntxModRsp->pdu.val.successfulOutcome.value.u.\
           sztUECntxtModificationResp.protocolIEs.member[1]);

   veFillTknU8(&(ie->pres), PRSNT_NODEF);     
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), eNbId);

   *pdu = ueCntxModRsp;

   RETVALUE(ROK);
}

/*
*
*       Fun:   veUtlBuildUeCntxModFail
*
*       Desc:  Construct UE Context Modification Failure
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c  
*
*/
#ifdef ANSI
PUBLIC S16 veUtlFillUeCntxModFail
(
S1apPdu      **pdu,
U32          eNbId,
U32          mmeId,
U32          cause,
U8           choice
)
#else
PUBLIC S16 veUtlFillUeCntxModFail(pdu, eNbId, mmeId, cause, choice)
S1apPdu      **pdu;
U32          eNbId;
U32          mmeId;
U32          cause;
U8           choice;
#endif
{
   S1apPdu      *ueCntxModFail = NULLP;
   S16           ret;
   U16           numComp;
   SztProtIE_Field_UECntxtModificationFailIEs   *ie;

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VE_SZ_MEM_SDU_SIZE, &veCb.mem, 
         (Ptr *)&ueCntxModFail);

   if(ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veUtlBuildUeCntxModRsp:\
               cmAllocEvnt failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Fill the procedure code */
   veFillTknU8(&(ueCntxModFail->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   veFillTknU8(&(ueCntxModFail->pdu.val.unsuccessfulOutcome.pres),
                 PRSNT_NODEF);
   veFillTknU32(&(ueCntxModFail->pdu.val.unsuccessfulOutcome.procedureCode),
         Sztid_UECntxtModification);
   veFillTknU32(&(ueCntxModFail->pdu.val.unsuccessfulOutcome.criticality),
         SztCriticalityrejectEnum);

   veFillTknU8(&(ueCntxModFail->pdu.val.unsuccessfulOutcome.value.u.\
                sztUECntxtModificationFail.pres), PRSNT_NODEF);

   /* Filling 2 IEs mmeId and eNb Id */
   numComp = 3;
  
    /* Prasanna given patch patch_ue_cntx_mod.txt */
    if ((cmGetMem(ueCntxModFail, (numComp * sizeof(SztProtIE_Field_UECntxtModificationFailIEs)),
                  (Ptr*)&(ueCntxModFail->pdu.val.unsuccessfulOutcome.value.u.sztUECntxtModificationFail.\
                  protocolIEs.member))) != ROK)
    {
 #ifdef DEBUGP
       VE_DBG_ERROR((VE_PRNT_BUF,"veUtlSztFillErrInd: cmGetMem failed\n"));
 #endif
       RETVALUE(RFAILED);
    }     

   veFillTknU16(&(ueCntxModFail->pdu.val.unsuccessfulOutcome.value.u.\
                 sztUECntxtModificationFail.protocolIEs.noComp), numComp);

   ie = &(ueCntxModFail->pdu.val.unsuccessfulOutcome.value.u.\
         sztUECntxtModificationFail.protocolIEs.member[0]);

   veFillTknU8(&(ie->pres), PRSNT_NODEF);     
   veFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), mmeId);

   ie = &(ueCntxModFail->pdu.val.unsuccessfulOutcome.value.u.\
           sztUECntxtModificationFail.protocolIEs.member[1]);

   veFillTknU8(&(ie->pres), PRSNT_NODEF);     
   veFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), eNbId);

   ie = &(ueCntxModFail->pdu.val.unsuccessfulOutcome.value.u.\
           sztUECntxtModificationFail.protocolIEs.member[2]);

   veFillTknU8(&(ie->pres), PRSNT_NODEF);     
   veFillTknU32(&(ie->id), Sztid_Cause);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU8(&(ie->value.u.sztCause.choice), choice);

   switch(choice)
   {
      case CAUSE_RADIONW:
         veFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
         break;
      case CAUSE_PROTOCOL:
         veFillTknU32(&(ie->value.u.sztCause.val.protocol), cause);
         break;
      case CAUSE_MISC:
         veFillTknU32(&(ie->value.u.sztCause.val.misc), cause);
         break;
   }

   *pdu = ueCntxModFail;

   RETVALUE(ROK);
}

/********************************************************************30**

         End of file:     ve_s1_bldmsg.c@@/main/eNB_1.3_With_LatestGpr3/14 - Fri Sep 16 17:30:01 2011

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

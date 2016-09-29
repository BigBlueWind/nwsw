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

/********************************************************************20**

     Name:     LTE RRC

     Type:     C source file

     Desc:     This file contains call back functions for all lower
               interfaces.

     File:     ve_li.c

     Sid:      ve_li.c@@/main/2 - Wed Dec  1 08:44:26 2010

     Prg:      

*********************************************************************21*/
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
#include "ve_eut.h"
#ifdef EU_DAT_APP
#include "egt.h"           /* S1AP RRM control Interface */
#endif 
#include "lrg.h"           /* MAC RRM control Interface */


/* RRM related includes */
#include "sz.h"
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
#include "cm_inet.x"
#include "cm_tpt.x"

#include "nhu_asn.x"
#include "czt.x"
#include "nhu.x"           /* NHU Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "ctf.x"           /* PHY RRM control Interface */
#include "szt.x"           /* S1AP RRM control Interface */
#include "ve_eut.x"
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "ve_eut.x"
#include "lrg.x"           /* MAC RRM control Interface */

/* RRM related includes */
#include "lve.x" 
#include "ve.x" 
/* MSPD changes */
#ifndef MSPD
#include "ve_dflt.h"
/* TIC_ID ccpu00120082 ADD : For disabling SIBs */
#include "ve_dflt_pal.x"
#else
#include "ve_dflt_ms.x"
#endif
#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif



#ifdef LTE_HO_SUPPORT
#include "szt_asn.x"
#include "czt_asn.x"
#endif

VeEuCb veEuCb;

/* RRC State Machine for Uplink RRC Messages */
/* based on Message Category and Message Type*/
PUBLIC  VeNhuLIMFSM veNhuLIMFSM[VE_NH_MAX_MSG_CLASS][VE_NH_MAX_MSG_TYPE]=
{
/*DUMMY*/
{     veNoOpr, 
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr
},
{ /*DL-DCCH message */
      veNoOpr, 
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr
},
{ /* UL-DCCH message */
      veNoOpr,  

#ifdef LTE_HO_SUPPORT
      veLIMRrcMeasRprtHndlr,
#else
      veNoOpr, 
#endif
      veLIMRrcReconfigCmpHndlr, /*RRC Reconfiguration Request Handler*/
      veLIMRrcConReestabCmpHndlr, /* RRC-ReEstab :RRC Connection Re-establishment Handler*/
      veLIMRrcConSetCmpHndlr,  /*RRC Connection Setup Complete Handler */
      veLIMRrcSecModCmpHndlr,   /*RRC Security Mode Complete handler*/
      veLIMRrcSecModFailHndlr,
/* Ue Capability info handler */
#ifdef UE_RAD_CAP
      veLIMRrcUeCapInfoHndlr,
#else
      veNoOpr,
#endif
      veNoOpr,
      veLIMRrcUlInfoTfrHndlr,
      veNoOpr
  },
{ /*DL-CCCH message */
      veNoOpr, 
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr
},
{ /* UL-CCCH message */
      veSndRrcConReEst, 
      veSndRrcConReqSetup, /* RRC Connection Request handler */
      veNoOpr, 
      veNoOpr, 
      veNoOpr, 
      veNoOpr, 
      veNoOpr, 
      veNoOpr, 
      veNoOpr, 
      veNoOpr, 
      veNoOpr 
  }
};
#ifndef LTE_ENB_PERF
PUBLIC  VeSztLIMFSM veSztLIMFSM[SZ_MAX_MSG_ID]=
{
   veLiSztERABSetHandler,     /* E-RAB setup request */
   veSztNoOpr,     /* E-RAB setup response */
   veLiSztERABModHandler,     /* E-RAB modify request */
   veSztNoOpr,     /* E-RAB modify response */
   veLiSztERABRelComHandler,     /* E-RAB release command */
   veSztNoOpr,     /* E-RAB release response */
   veSztNoOpr,     /* E-RAB release Indication */
   veLiSztInitConSetHandler,     /* Initial Context setup request */
   veSztNoOpr,     /* Initial Context setup response */
   veSztNoOpr,     /* Initial Context setup failure */
   veSztNoOpr,     /* UE Context release request */
   veLiSztUeRelCmdHandler,     /* UE Context release command */
   veSztNoOpr,     /* UE Context release complete */
   veLiSztUeCntxModHandler,     /* UE Context modification request */
   veSztNoOpr,     /* UE Context modification response */
   veSztNoOpr,     /* UE Context modification failure */
   veSztNoOpr,     /* Initial UE message */
   veLiSztDlNasHandler,     /* Downlink NAS transport */
   veSztNoOpr,     /* Uplink NAS transport */
   veSztNoOpr,     /* NAS non delivery indication */
   veLiSztErrIndHdl,     /* Error indication */
   veSztNoOpr,     /* No Data */
   veLiResetHandler,  /* Reset */
   veLiResetAckHandler,     /* Reset Acknowledge */
   veSztNoOpr,   /* Setup Request */
   veSztNoOpr,     /* Setup Response */
   veSztNoOpr, /* Setup Fail */
   veLiSztPagingHandler,   /* Paging */
   veSztNoOpr,     /* Handover Required */
#ifdef LTE_HO_SUPPORT
   veLiSztHoCmdHandler,     /* Handover Command */
   veLiSztHoPrepFlrHandler, /* Handover Preperation Failure */
   veLiSztHoRqstHandler,    /* Handover Request */
#else
   veSztNoOpr,     /* Handover Command */
   veSztNoOpr,     /* Handover Preperation Failure */
   veSztNoOpr,     /* Handover Request */
#endif
   veSztNoOpr,     /* Handover Request Acknowledge */
   veSztNoOpr,     /* Handover Request Failure */
   veSztNoOpr,     /* Handover Notify */
   veSztNoOpr,     /* Path Switch Request */
#ifdef LTE_HO_SUPPORT
   veLiSztHoPthSwthReqAckHandler,  /* Path Switch Request Acknowledge */
   veLiSztHoPthSwthReqFlrHandler,  /* Path Switch Request Failure */
#else
   veSztNoOpr,                     /* Path Switch Request Acknowledge */
   veSztNoOpr,                     /* Path Switch Request Failure */
#endif
   veSztNoOpr,     /* Handover Cancel */
#ifdef LTE_HO_SUPPORT
   veLiSztCnclAckHandler,  /* Handover Cancel Acknowledge */
#else
   veSztNoOpr,  /* Handover Cancel Acknowledge */
#endif
   veSztNoOpr,     /* ENB Status Transfer */
#ifdef LTE_HO_SUPPORT
   veLiSztMmeStsTfrHandler, /* MME Status Transfer */
#else
   veSztNoOpr,     /* MME Status Transfer */
#endif
   veSztNoOpr,     /* Deactivate Trace */
   veSztNoOpr,     /* Trace Start */
   veSztNoOpr,     /* Trace Failure Indication */
   veSztNoOpr,     /* Location Reporting Control */
   veSztNoOpr,     /* Location Report Failure Indication */
   veSztNoOpr,     /* Location Report */
   veSztNoOpr,   /* ENB Configuration Update */
   veLiSztHdlEnbCfgUpdAck,   /* ENB Configuration Update Acknowledge */
   veLiSztHdlEnbCfgUpdFail,   /* ENB Configuration Update Failure */
   veSztMmeCfgUpdHandler,   /* MME Configuration Update */
   veSztNoOpr,   /* MME Configuration Update Acknowledge */
   veSztNoOpr,   /* MME Configuration Update Failure */
   veSztNoOpr,     /* Uplink CDMA 2000 Tunneling */
   veSztNoOpr,     /* Downlink CDMA 2000 Tunneling */
   veSztNoOpr,     /* UE Capability Info Indication */
   veLiSztOvldStartHandler, /* MME Overload Start */
   veLiSztOvldStopHandler, /* MME Overload Stop */
   veSztNoOpr,   /* Write Replace Warning Request */
   veSztNoOpr,   /* Write Replace Warning Response */
   veSztNoOpr,   /* ENB Information Direct Transfer */
   veSztNoOpr    /* MME Information Direct Transfer */
};
#ifdef LTE_HO_SUPPORT
PUBLIC  VeCztLIMFSM veCztLIMFSM[VE_CZ_MAX_MSG_ID]=
{
   veLiCztHoRqstHandler,         /* Handover request */
   veLiCztHoAckHandler,          /* Handover ACK*/
   veLiCztHoFlrHandler,          /* Handover Failure */
   veLiCztHoCnclHandler,         /* Handover Cancel */
   veLiCztSnStsTrsfrHandler,     /* SN Status Transfer */
   veLiCztUeCntxtRlsHandler,     /* UE Context release */
   veCztNoOpr,                   /* Load Indication */
   veLiCztErrIndHandler,                   /* Error Indication */
   veLiCztX2SetupRqstHandler,    /* X2 Setup request */
   veLiCztX2SetupRspHandler,     /* X2 Setup response */
   veLiCztX2SetupFlrHandler,     /* X2 Setup Failure */
   veLiCztResetRqstHandler,      /* X2AP Reset */
   veLiCztResetRspHandler,       /* X2AP Reset Rsp */
   /* X2_CFG_UPD_CHANGES */
   veLiCztCfgUpdReqHandler,      /* X2AP Config Update Request */
   veLiCztCfgUpdAckHandler,      /* X2AP config Update Ack */
   veLiCztCfgUpdFailHandler,     /* X2AP config Update Fail */
   veCztNoOpr,                   /* X2AP Resource Status Req*/
   veCztNoOpr,                   /* X2AP Resource Status Response */
   veCztNoOpr,                   /* X2AP Resource Status Failure */
   veCztNoOpr,                   /* X2AP Resource Status Update */
   veCztNoOpr,                   /* X2AP Private Message */
};
#endif /* End of LTE_HO_SUPPORT */

#endif


/*
*
*       Fun:   VeLiNhuBndCfm - NHU Bind Cfm
*
*       Desc:  This function is used by to present Bind Cfm to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 VeLiNhuBndCfm
(
Pst     *pst,
SuId    suId,
U8      status
)
#else
PUBLIC S16 VeLiNhuBndCfm(pst,suId,status)
Pst     *pst;  
SuId    suId;
U8      status;
#endif
{
   TRC2(VeLiNhuBndCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiNhuBndCfm: \
            NHU Bind Confirm primitive \n"));
#endif

   /* Send alarm */
   if (CM_BND_OK == status)
   {
      veStopTmr((PTR)veCb.nhuSap[0],VE_TMR_NHU_SAP_BND);

#ifdef DEBUGP
      VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiNhuBndCfm: \
               Successful NHU Bind Confirm\n"));
#endif
      /* trigger  alarm toward stack manager */
      veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, LVE_CAUSE_NHU_SAP_BOUND);
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiNhuBndCfm: Binding Unsuccessful\n"));
#endif
      /* trigger  alarm toward stack manager */
      veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
   }

   RETVALUE(ROK);
} /* end of VeLiNhuBndCfm */



/*
*
*       Fun:   VeLiNhuDatInd - NHU Data Indication
*
*       Desc:  This function is used by to present Data Indication to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiNhuDatInd
(
Pst     *pst,          
SuId     suId,
NhuDatIndSdus *nhuDatIndSdus           
)
#else
PUBLIC S16 VeLiNhuDatInd (pst,suId,nhuDatIndSdus)
Pst     *pst;          /* post structure */
SuId     suId;
NhuDatIndSdus *nhuDatIndSdus;          /* configuration */
#endif
{
   TRC2(VeLiNhuDatInd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiNhuDatInd: NHU Data Ind \n"));
#endif

#ifdef VE_PERF_MEAS
   vePerfHdlNhuDatInd(pst, suId, nhuDatIndSdus);
#else
   /* Call the Dat Ind handler */
   veHdlNhuDatInd(pst, suId, nhuDatIndSdus);
#endif

   /* Free event structure */ 
   VE_FREEEVNT(nhuDatIndSdus);

   RETVALUE(ROK);
} /* end of VeLiNhuDatInd */

/*
*
*       Fun:   VeLiNhuDatCfm - NHU Data Confirm
*
*       Desc:  This function is used by to present Data Confirmation to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiNhuDatCfm
(
Pst     *pst,          
SuId    suId,
NhuDatCfmSdus *nhDatCfmSdus           
)
#else
PUBLIC S16 VeLiNhuDatCfm (pst,suId,nhDatCfmSdus)
Pst     *pst;          
SuId     suId;
NhuDatCfmSdus *nhDatCfmSdus;
#endif
{
   U8             ueIdx;
   VeUeCb         *ueCb = NULLP;
   /* NAS NON Delivery Indication start */
   SztDatEvntReq  datEvt;
   VeNasInfo      *tempNasPdu;
   VeCellCb       *cellCb = NULLP;
   S1apPdu        *pdu = NULLP;
   /* NAS NON Delivery Indication end */
   Pst            psttemp;

   TRC2(VeLiNhuDatCfm)

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiNhuDatCfm: NHU Data Confirm \n"));
#endif

   /* Get the Ue Cb*/
   VE_GET_UECB(ueCb, nhDatCfmSdus->hdr.ueId);

   /* The transaction should be cheked against RRC Connection release*/
   /* A better approach would be to consider SRB and DRB separately */
   /* More thought is required */
   if (ueCb == NULLP)
   {
      VE_FREE(nhDatCfmSdus, sizeof(NhuDatCfmSdus));
      RETVALUE(ROK);
   }
   /* CR ID- ccpu00116764-IOT Changes*/
      if ((NHU_ERRCAUSE_NONE == nhDatCfmSdus->datCfmStatus)
                    && (VE_RRC_RELEASING == ueCb->rrcConnState))
   {
      veSndCtfUeRls(ueCb);
      /* RRC ReEst */
      veSndRgrUeRls(ueCb,NULLP,RGR_UE_CFG);
      VeLiNhuCnclUeReq(&(veCb.nhuSap[0]->pst), veCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);
           /* NAS NON Delivery Indication */
      /* need to add changed for delete all the nodes */
      veDelAllNasPduFrmLst(ueCb);
      /* Get ue Index */
      ueIdx = (U8)VE_GET_UE_IDX(ueCb->crnti);/* klock warning fixed */
      if(ueIdx < veCb.cellCb[0]->maxUeSupp)
      {
   /* Changes are incorporated*/
         veCb.cellCb[0]->ueCbLst[ueIdx] = NULLP;
         veCb.cellCb[0]->numUeCfg--;
      }
      VE_FREE(ueCb,sizeof(VeUeCb));

   }
/* CRID:ccpu00118776  Handling RLC MAX Retx failure*/      
      /*Cleaning up of resources when RLC MAX Retx has reached*/
      /*This scenario is only for SRBs*/
#ifdef VE_SRB2_SUPPORT      
      else if(ueCb->rbInfo.srb[1].rbType  == VE_PDCP_ID_SRB2)
#else
      else if(ueCb->rbInfo.srb[0].rbType == VE_PDCP_ID_SRB1)
#endif
      {
         if ((NHU_ERRCAUSE_MAX_RLC_TRANS_REACH == nhDatCfmSdus->datCfmStatus))
         {
            /* Get ue Index */
            ueIdx = (U8)VE_GET_UE_IDX(ueCb->crnti);
            if((VE_RRC_RELEASING != ueCb->rrcConnState))
            { 
               /*Currently hard coding the pst structure*/
               psttemp.dstEnt = ENTVE;
               psttemp.srcEnt = ENTVE;
               psttemp.dstProcId = SM_VE_PROC;
               psttemp.srcProcId = SM_VE_PROC;

               /* The UeId is packed and event is triggered towards
                *     * the Relay system thread to send the S1 Release 
                *         * Request */
               if(ueIdx < veCb.cellCb[0]->maxUeSupp)
               {
#ifndef LTE_ENB_PERF
                  cmPkEuRelReq(&psttemp, 0, (U32)ueCb->crnti);
#endif
               }
            }
            /*Release towards UE side */
            /* UE release Req towards CTF and RGR Interface*/
            veSndCtfUeRls(ueCb);
            veSndRgrUeRls(ueCb, NULLP, RGR_UE_CFG);
            VeLiNhuCnclUeReq(&(veCb.nhuSap[0]->pst), veCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);

            if(ueIdx < veCb.cellCb[0]->maxUeSupp)
            {
               veCb.cellCb[0]->ueCbLst[ueIdx] = NULLP;
            }
            VE_FREE(ueCb,sizeof(VeUeCb));
      }
      /*NAS NON Delivery Indication start */
      else if (NHU_ERRCAUSE_NONE != nhDatCfmSdus->datCfmStatus)
      {
         /* NAS message has been Undelivered due some internal error 
          *        * and report the NAS NON DELIVERY INDICATION(NNDI)  to MME for recovery and 
          *               * remove NAS PDU from UECB and CELLCB after successful NNDI*/
         /* Trigger S1AP NAS NON DELIVERY INDICATION */
         VE_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));
         datEvt.spConnId = ueCb->s1ConCb->spConnId;

         cellCb = veCb.cellCb[0];
         /* Retrive the NAS PDU from  HASH list, using transId */
         cmHashListFind(&(cellCb->vePduHashLst), (U8 *)&(nhDatCfmSdus->hdr.transId), sizeof (U32), 0, (PTR *) &tempNasPdu);
         /* building S1AP message */
         veUtlSztFillNasNonDlvInd(ueCb->s1ConCb, &pdu, (tempNasPdu->asn));
         /* Send the S1AP message to MME */
         VeLiSztDatReq(&veCb.sztSap[0]->pst,veCb.sztSap[0]->spId , &datEvt);

         /* Deleting the NAS PDU entries from Hash and linked list */
         veRemoveNasPduFrmLst(ueCb, nhDatCfmSdus->hdr.transId);
      }
      else
      {
         /* Successfully  NAS message has been delivered and it is safe to remove
          *        * NAS PDU from UECB and CELLCB */

         /* Deleting the NAS PDU entries from Hash and linked list */
         veRemoveNasPduFrmLst(ueCb, nhDatCfmSdus->hdr.transId);
      }
      /* NAS NON Delivery Indication end */

   }   
      /* Free the SDU */
      VE_FREE(nhDatCfmSdus, sizeof(NhuDatCfmSdus));
      RETVALUE(ROK);
} /* end of VeLiNhuDatCfm */

/*
*
*       Fun:   VeLiNhuStaInd - NHU status Indication
*
*       Desc:  This function is used by to present status indication to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiNhuStaInd
(
Pst     *pst,          
SuId    suId,
NhuStaIndSdus *nhStaIndSdus           
)
#else
PUBLIC S16 VeLiNhuStaInd (pst,suId, nhStaIndSdus)
Pst     *pst;          
SuId     suId;
NhuStaIndSdus *nhStaIndSdus;
#endif
{
   TRC2(VeLiNhuStaInd);
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiNhuStaInd:\
            NHU Status Ind \n"));
#endif
   /* Free the SDU */
   VE_FREE(nhStaIndSdus, sizeof(NhuStaIndSdus));
   RETVALUE(ROK);
} /* end of VeLiNhuStaInd */

/*
*
*       Fun:   VeLiNhuErrInd - Error Indication
*
*       Desc:  This function is used by to present configuration 
*              confirm information to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 VeLiNhuErrInd
(
Pst     *pst,          
SuId     suId,
NhuErrIndSdus *nhErrIndSdus           
)
#else
PUBLIC S16 VeLiNhuErrInd(pst,suId,nhErrIndSdus)
Pst     *pst;          
SuId     suId;
NhuErrIndSdus *nhErrIndSdus;          
#endif
{
   TRC2(VeLiNhuErrInd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiNhuErrInd: \
            NHU Error Ind \n"));

#ifdef ALIGN_64BIT
   VE_DBG_ERROR((VE_PRNT_BUF, "VeLiNhuErrInd:\
            \nHeader(Cell:Ue:TransId): %d:%d:%d\n",
            nhErrIndSdus->hdr.cellId,
            nhErrIndSdus->hdr.ueId,
            nhErrIndSdus->hdr.transId
            ));
#else
   VE_DBG_ERROR((VE_PRNT_BUF, "VeLiNhuErrInd:\
            \nHeader(Cell:Ue:TransId): %d:%d:%ld\n",
            nhErrIndSdus->hdr.cellId,
            nhErrIndSdus->hdr.ueId,
            nhErrIndSdus->hdr.transId
            ));
#endif
   VE_DBG_ERROR((VE_PRNT_BUF, "VeLiNhuErrInd:\
            \nErrorInfo(Type:Cause): %d:%d\n",
            nhErrIndSdus->sdu.errType,
            nhErrIndSdus->sdu.errCause
            ));
#endif
   /* Free the SDU */
   VE_FREE(nhErrIndSdus, sizeof(NhuErrIndSdus));

   RETVALUE(ROK);
} /* end of VeLiNhuErrInd */

/*
*
*       Fun:   VeLiNhuCfgCfm - Lower layer config confirm
*
*       Desc:  This function is used by to present configuration confirm
*              to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 VeLiNhuCfgCfm
(
Pst     *pst,
SuId     suId,
NhuCfgCfmSdus *nhuCfgCfmSdus
)
#else
PUBLIC S16 VeLiNhuCfgCfm(pst,suId,nhuCfgCfmSdus)
Pst     *pst;
SuId     suId;
NhuCfgCfmSdus *nhuCfgCfmSdus;
#endif
{

   S16     retVal = 0;

   VeUeCb *ueCb = NULLP;
#ifdef LTE_HO_SUPPORT
   NhuEncReqSdus *hoCmdEncReq = NULLP;
   U8      ueIdx;
   U8 idx = 0;
#endif

   TRC2(VeLiNhuCfgCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiNhuCfgCfm:\
            NHU Config. Cfm. \n"));
#endif

   /* Get the Ue Cb*/
   VE_GET_UECB(ueCb, nhuCfgCfmSdus->hdr.ueId);
   /* klock warning fixed */
   if(ueCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiNhuCfgCfm:\
               Failure in the allocation of ue control block\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */


   switch(nhuCfgCfmSdus->hdr.transId)
   {
      case C3_RRCCONSETUP:

         retVal = veSndRgrUeCfg(ueCb);

         if(ROK != retVal)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"\n VeLiNhuCfgCfm: \
                        Failed to Send veSndRgrUeCfg\n"));
#endif
            RETVALUE(RFAILED);
         }

         break;

      case C4_RRCCONRECFGN:
         /* triggering of rgr lch cfg for SRB2 and DRBs
         as already triggered in veSndRrcRecfgReq*/
         break;
#ifdef LTE_HO_SUPPORT
      case VE_TRANSID_UE_CFG:
         if(ueCb->lowLyrCfgSts == VE_LOW_LYR_CFG_IN_PRGS)
         {
            retVal = veSndRgrUeCfg(ueCb);
            
            if(ROK != retVal)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"\n VeLiNhuCfgCfm: \
                        Failed to Send veSndRgrUeCfg\n"));
#endif
               RETVALUE(RFAILED);
            }

            /* SRB 1 */
            retVal = veSndRgrLchCfg(ueCb, &ueCb->rbInfo.srb[0], FALSE);

            if(ROK != retVal)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"\n VeLiNhuCfgCfm: \
                        Failed to Send veSndRgrLchCfg\n"));
#endif
               RETVALUE(RFAILED);
            }

#ifdef VE_SRB2_SUPPORT
            /* SRB 2 */
            retVal = veSndRgrLchCfg(ueCb, &ueCb->rbInfo.srb[1], FALSE);

            if(ROK != retVal)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"\n VeLiNhuCfgCfm: \
                        Failed to Send veSndRgrLchCfg\n"));
#endif
               RETVALUE(RFAILED);
            }
#endif

            /* Trigger RGR LCH Cfg for DRBs */
            for (idx = 0; idx < ueCb->rbInfo.numOfRbCfgd; idx++)
            {
               retVal = veSndRgrLchCfg(ueCb, &ueCb->rbInfo.rab[idx], FALSE);

               if(ROK != retVal)
               {
#ifdef DEBUGP
                  VE_DBG_ERROR((VE_PRNT_BUF,"\n VeLiNhuCfgCfm: \
                        Failed to Send veSndRgrLchCfg\n"));
#endif
                  RETVALUE(RFAILED);
               }
            }
            /* MSPD_FIX for HO*/
            /* Send the UE CFG to CL */
            veSndCtfUeCfg(ueCb);

            /* fill HoCommand RRC Structure */

            /* trigger NhuEncodeReq to encode Tgt to Src Transparent Container */
            VE_ALLOC(&hoCmdEncReq, sizeof(NhuEncReqSdus));

            if(NULLP == hoCmdEncReq)
            {
#ifdef DEBUGP
                  VE_DBG_ERROR((VE_PRNT_BUF,"\n VeLiNhuCfgCfm: \
                        Failed to allocate memory for hoCmdEncReq.\n"));
#endif
                  RETVALUE(RFAILED);
            }

            hoCmdEncReq->hdr.cellId = ueCb->cellId;
            hoCmdEncReq->hdr.ueId = ueCb->crnti;
            /* To encode of RRC Connection Reconfiguration.*/
            hoCmdEncReq->hdr.transId = VE_ENC_TRANSID_HO_DL_DCCH_MSG;

            /* fill the NhuHovrPrepInfo info inside hoPrepEncReq */
            if(ROK != veUtlFillHoCmdTrnspCntrWthMobility(ueCb, hoCmdEncReq, \
                     ueCb->crnti,veCb.cellCb[0]->physCellId))
            {
               VE_DBG_ERROR((VE_PRNT_BUF,"\nveLiSztHoRqstHandler: \
                        Filling of HO Command Tranparent Container Failed.\n"));
               VE_FREE(hoCmdEncReq, sizeof(NhuEncReqSdus));
            }
            else
            {
               if(ROK != VeLiNhuEncodeReq(&veCb.nhuSap[0]->pst, veCb.nhuSap[0]->suId, hoCmdEncReq))
               {
#ifdef DEBUGP
                  VE_DBG_ERROR((VE_PRNT_BUF,"\n VeLiNhuCfgCfm: \
                        Failed to VeLiNhuEncodeReq.\n"));
#endif
                  RETVALUE(RFAILED);
               }
            }
         }
         else if(ueCb->lowLyrCfgSts == VE_LOW_LYR_CFG_REM)
         {
            /* release UE resources */
            retVal = veSndCtfUeRls(ueCb);

            if(ROK != retVal)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"\n VeLiNhuCfgCfm: \
                        Failed to Send veSndCtfUeRls\n"));
#endif
               RETVALUE(RFAILED);
            }
            /* RRC ReEst */ 
            retVal = veSndRgrUeRls(ueCb,NULLP,RGR_UE_CFG);

            if(ROK != retVal)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"\n VeLiNhuCfgCfm: \
                        Failed to Send veSndRgrUeRls\n"));
#endif
               RETVALUE(RFAILED);
            }

            retVal = VeLiNhuCnclUeReq(&(veCb.nhuSap[0]->pst), veCb.nhuSap[0]->spId, ueCb->cellId, ueCb->crnti);

            if(ROK != retVal)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"\n VeLiNhuCfgCfm: \
                        Failed to Send VeLiNhuCnclUeReq\n"));
#endif
               RETVALUE(RFAILED);
            }

            /* release x2 resources */
            retVal = veSndX2UeLocRlsReq(ueCb->x2ConCb);
            
            if(ROK != retVal)
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"\n VeLiNhuCfgCfm: \
                        Failed to Send veSndX2UelocRlsReq\n"));
#endif
               RETVALUE(RFAILED);
            }
            veCb.x2apConLst[VE_GET_UE_X2_IDX(ueCb->x2ConCb->oldEnbUeX2apId)] = NULLP;

            /* Send EGTP release command to EGTP-U*/
            if(ROK != veSndEgtpCom(VE_EGTP_TUN_DEL, ueCb->x2ConCb->ueCb->s1ConCb))
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"\n VeLiNhuCfgCfm: \
                        Failed to veSndEgtpCom.\n"));
#endif
               RETVALUE(RFAILED);
            }

            /* release s1 resources */
            if(ROK != veSndS1UeLocRlsReq(ueCb->s1ConCb))
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"\n VeLiNhuCfgCfm: \
                        Failed to veSndS1UelocRlsReq.\n"));
#endif
               RETVALUE(RFAILED);
            }
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
         }
         break;
#endif
      default:
         break;
   }

   /* Free the SDU */
   VE_FREE(nhuCfgCfmSdus, sizeof(NhuCfgCfmSdus));

   RETVALUE(ROK);
} /* end of VeLiNhuCfgCfm */

/*
*
*       Fun:   VeLiNhuCellCfgCfm - Cell Configuration Confirm
*
*       Desc:  This function is used by to present Cell Config Status Confirm
*              information to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 VeLiNhuCellCfgCfm
(
Pst     *pst,          
SuId     suId,
NhuCellCfgCfmSdus *nhuCellCfgCfmSdus           
)
#else
PUBLIC S16 VeLiNhuCellCfgCfm(pst,suId,nhuCellCfgCfmSdus)
Pst     *pst;         
SuId     suId;
NhuCellCfgCfmSdus *nhuCellCfgCfmSdus;          
#endif
{
   TRC2(VeLiNhuCellCfgCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiNhuCellCfgCfm:\
            NHU Cell Cfg Cfm \n"));
#endif

   if(nhuCellCfgCfmSdus->sdu.isCfgSuccess == FALSE)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF, "cell configuration failed: %d\n",
               nhuCellCfgCfmSdus->hdr.cellId));
#endif
      /* Free the SDU */
      VE_FREE(nhuCellCfgCfmSdus, sizeof(NhuCellCfgCfmSdus));
      /* Send alarm */
      veSendLmAlarm(LCM_CATEGORY_PROTOCOL, LVE_EVENT_CELL_CFG_FAILED, LCM_CAUSE_UNKNOWN);

      RETVALUE(RFAILED);
   }

   /* Free the SDU */
   VE_FREE(nhuCellCfgCfmSdus, sizeof(NhuCellCfgCfmSdus));
   /* Trigger RGR Cell Cfg */
   if(ROK != veHdlRgrCellCfgReq(veCb.cellCb[0]->cellId))
   {
#ifdef DEBUGP
#ifdef ALIGN_64BIT
      VE_DBG_ERROR((VE_PRNT_BUF, "VeLiNhuCellCfgCfm: veHdlRgrCellCfgReq failed %d\n", veCb.cellCb[0]->cellId));
#else
      VE_DBG_ERROR((VE_PRNT_BUF, "VeLiNhuCellCfgCfm: veHdlRgrCellCfgReq failed %ld\n", veCb.cellCb[0]->cellId));
#endif
#endif
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of VeLiNhuCellCfgCfm */

/*
*
*       Fun:   VeLiNhuDecodeCfm - Decode Confirm
*
*       Desc:  This function is used to inform decode confirmation to RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 VeLiNhuDecodeCfm
(
Pst     *pst,
SuId     suId,
NhuDecodeSduCfm *nhuDecodeSduCfm
)
#else
PUBLIC S16 VeLiNhuDecodeCfm(pst,suId,nhuDecodeSduCfm)
Pst     *pst;
SuId     suId;
NhuDecodeSduCfm *nhuDecodeSduCfm;
#endif
{
   /* Ue Capability changes */
   S16            ret   = ROK;
#if (defined (LTE_HO_SUPPORT) || defined(UE_RAD_CAP))
   VeUeCb         *ueCb = NULLP;
#endif
   TRC2(VeLiNhuDecodeCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiNhuDecodeCfm:\
            NHU Decode Cfm Received\n"));
#endif
#if (defined (LTE_HO_SUPPORT) || defined(UE_RAD_CAP))
   /* Ue Capability changes */
   VE_GET_UECB(ueCb,nhuDecodeSduCfm->hdr.ueId);
   if(NULLP == ueCb)
   {
      VE_DBG_ERROR((VE_PRNT_BUF, "VeLiNhuDecodeCfm: invalid UE CB [crnti = %d] ",
         nhuDecodeSduCfm->hdr.ueId));
      RETVALUE(RFAILED);
   }
#endif
   
   switch(nhuDecodeSduCfm->sduType)
   {
#ifdef UE_RAD_CAP
     case NHU_DEC_SDUTYPE_EUTRA_UECAP_IE:
        ret = veHdlNhuEutraUeCapIeDecCfm(ueCb,nhuDecodeSduCfm); 
        break;  
     case NHU_MSG_UERACAPINFO:
        ret =  veHdlNhuUeRaCapDecCfm(ueCb,nhuDecodeSduCfm);
        break;
     case NHU_MSG_UECAPINFO:
        ret = veHdlNhuUeCapDecCfm(ueCb,nhuDecodeSduCfm); 
        break;
#endif
#ifdef LTE_HO_SUPPORT
     case NHU_MSG_HOCMD:
        ret = veHdlNhuHoCmdDecCfm(ueCb,nhuDecodeSduCfm); 
        break;
#endif /* End of LTE_HO_SUPPORT */
      default:
         VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiNhuDecodeCfm:\
               Unknown Cfm \n"));
         ret = RFAILED;
         break;
   }
   /* Free the SDU */
   VE_FREE(nhuDecodeSduCfm, sizeof(NhuDecodeSduCfm));
   /* Ue Capability changes */
   RETVALUE(ret);
} /* end of VeLiNhuDecodeCfm */

/*
*
*       Fun:   VeLiNhuCountCfm - Count Confirm
*
*       Desc:  This function is used to inform COUNT values of DRBs to RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 VeLiNhuCountCfm
(
Pst     *pst,
SuId     suId,
NhuDRBCountInfoLst *nhuDRBCountInfoLst
)
#else
PUBLIC S16 VeLiNhuCountCfm(pst,suId,nhuDRBCountInfoLst)
Pst     *pst;
SuId     suId;
NhuDRBCountInfoLst *nhuDRBCountInfoLst;
#endif
{
   TRC2(VeLiNhuCountCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiNhuCountCfm:\
            NHU Count Confirm \n"));
#endif

   /* Free the SDU */
   VE_FREE(nhuDRBCountInfoLst, sizeof(NhuDRBCountInfoLst));
   RETVALUE(ROK);
} /* end of VeLiNhuCountCfm */

/*
*
*       Fun:   VeLiNhuEncodeCfm - Encode Confirm
*
*       Desc:  This function is used to inform Encode confirmation to RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 VeLiNhuEncodeCfm
(
Pst     *pst,
SuId     suId,
NhuEncCfmSdus *nhuEncCfmSdus
)
#else
PUBLIC S16 VeLiNhuEncodeCfm(pst,suId,nhuEncCfmSdus)
Pst     *pst;
SuId     suId;
NhuEncCfmSdus *nhuEncCfmSdus;
#endif
{
   /* Fix for CID- 1657-01-01 DefectId- 115433 */
   /* declaration is moved here to remove the warning */ 
   S16             retVal = ROK;
   /* Ue Capability changes */
#if (defined (LTE_HO_SUPPORT) || defined(UE_RAD_CAP))
   VeUeCb         *ueCb;
#endif

   TRC2(VeLiNhuEncodeCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiNhuEncodeCfm:\
            NHU Encode Confirm \n"));
#endif
#if (defined (LTE_HO_SUPPORT) || defined(UE_RAD_CAP))
   /* Ue Capability changes */
    VE_GET_UECB(ueCb,nhuEncCfmSdus->hdr.ueId);
    if(NULLP == ueCb)
    {
       VE_DBG_ERROR((VE_PRNT_BUF, "VeLiNhuEncodeCfm: invalid UE CB [crnti = %d] ",
          nhuEncCfmSdus->hdr.ueId));
       RETVALUE(RFAILED);
    }
#endif
   switch(nhuEncCfmSdus->sdu.msgCategory) 
   {
#ifdef UE_RAD_CAP
      case NHU_MSG_UECAPINFO:
         retVal = veHdlNhuUeCapEncCfm(ueCb, nhuEncCfmSdus); 
         break; 
      case NHU_MSG_UERACAPINFO:
         retVal = veHdlNhuUeRaCapEncCfm(ueCb, nhuEncCfmSdus);
         break; 
#endif      
#ifdef LTE_HO_SUPPORT
      case NHU_MSG_HOPREPINFO:
         retVal = veHdlNhuHoPrepEncCfm(ueCb, nhuEncCfmSdus);
         break; 
      case NHU_MSG_HOCMD:
         retVal = veHdlNhuHoCmdEncCfm(ueCb, nhuEncCfmSdus);
         break; 
      case NHU_MSG_DLDCCH:
         retVal = veHdlNhuDlDcchEncCfm(ueCb, nhuEncCfmSdus);
         break; 
#endif

      default:
       VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiNhuEncodeCfm:\
               Unknown Cfm \n"));
         retVal = RFAILED;
      break; 
   }
   /* Free the SDU */
   VE_FREE(nhuEncCfmSdus, sizeof(NhuEncCfmSdus));
   /* Ue Capability changes */
   RETVALUE(retVal);
} /* end of VeLiNhuEncodeCfm */

/*
*
*       Fun:   VeLiNhuPdcpSduStaCfm - PDCP SDU Status Confirm
*
*       Desc:  This function is used to inform PDCP SDU Status confirmation to 
*              RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 VeLiNhuPdcpSduStaCfm
(
Pst     *pst,
SuId     suId,
NhuPdcpSduStaCfm *nhuPdcpSduStaCfm
)
#else
PUBLIC S16 VeLiNhuPdcpSduStaCfm(pst,suId,nhuPdcpSduStaCfm)
Pst     *pst;
SuId     suId;
NhuPdcpSduStaCfm *nhuPdcpSduStaCfm;
#endif
{
   VeCellCb          *cellCb = NULLP;
   U8                ueIdx = 0;
   VeUeCb            *ueCb = NULLP;
  
   TRC2(VeLiNhuPdcpSduStaCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiNhuPdcpSduStaCfm:\
            NHU PDCP Status Confirm \n"));
#endif
    if(nhuPdcpSduStaCfm->status == ROK)
    {
       /* From the NhuHdr get the UeId and get the UeCb */
       cellCb = veCb.cellCb[0];
       ueIdx = (U8) VE_GET_UE_IDX(nhuPdcpSduStaCfm->hdr.ueId);
       ueCb = cellCb->ueCbLst[ueIdx];
       if(NULLP == ueCb)
       {
          VE_DBG_ERROR((VE_PRNT_BUF, "VeLiNhuPdcpSduStaCfm: invalid UE CB [crnti = %d] ",nhuPdcpSduStaCfm->hdr.ueId));
          RETVALUE(RFAILED);
       }
       else
       {

#ifdef LTE_HO_SUPPORT
          if(nhuPdcpSduStaCfm->hdr.transId == VE_TRANSID_GET_PDCP_SDUSTA)
          {
             if(ueCb->hoType == VE_HO_TYPE_X2)
             {
                if(RFAILED == veHoSndX2SNTfr(ueCb->x2ConCb,(PTR)nhuPdcpSduStaCfm))
                {
                   VE_DBG_ERROR((VE_PRNT_BUF,"VeLiNhuPdcpSduStaCfm:\
                            sending of X2SN Transfer failed\n"));
                }
             }
             else if(ueCb->hoType == VE_HO_TYPE_S1)
             {
                if(RFAILED == veHoSndS1SNTfr(ueCb->s1ConCb, nhuPdcpSduStaCfm))
                {
                   VE_DBG_ERROR((VE_PRNT_BUF,"VeLiNhuPdcpSduStaCfm:\
                            sending of S1SN Transfer failed\n"));
                }
             }
             else
             {
                VE_DBG_ERROR((VE_PRNT_BUF,"VeLiNhuPdcpSduStaCfm:\
                         sending of S1SN Transfer failed\n"));
             }
          }
#endif
       }
    }
    else
    {
       VE_DBG_ERROR((VE_PRNT_BUF, "VeLiNhuPdcpSduStaCfm: Status received is RFAILED"));
    }

   /* Free event structure */ 
   VE_FREEEVNT(nhuPdcpSduStaCfm);
   RETVALUE(ROK);
} /* end of VeLiNhuPdcpSduStaCfm */




/**************************************************************
RGR functions
***************************************************************/


/*
*
*       Fun:   VeLiRgrBndCfm - RGR Bind Cfm
*
*       Desc:  This function is used by to present Bind Cfm to RGR.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 VeLiRgrBndCfm
(
Pst     *pst,
SuId    suId,
U8      status
)
#else
PUBLIC S16 VeLiRgrBndCfm(pst,suId,status)
Pst     *pst;  
SuId    suId;
U8      status;
#endif
{
   TRC2(VeLiRgrBndCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiRgrBndCfm: RGR Bind Cfm \n"));
#endif

   /*-- Update Event --*/
   pst->event = EVTRGRBNDCFM;

   /* Send alarm */
   if (CM_BND_OK == status)
   {
#ifdef DEBUGP
      VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiRgrBndCfm:\
               RGR Bind Successful \n"));
#endif
      veStopTmr((PTR)veCb.rgrSap[0],VE_TMR_RGR_SAP_BND);
      veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, LVE_CAUSE_RGR_SAP_BOUND);
   }
   else
   {
#ifdef DEBUGP
      VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiRgrBndCfm:\
                Bind Unsuccessful \n"));
#endif
      veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
   }

   RETVALUE(ROK);
} /* end of VeLiRgrBndCfm */



/*
*
*       Fun:   VeLiRgrCfgCfm - Cell Configuration Confirm
*
*       Desc:  This function is used by to present Cell Config Status Confirm
*              information to RGR.
*
*       Ret:   None
*
*       nOTES: nONe
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 VeLiRgrCfgCfm
(
Pst            *pst,          
SuId           suId,
RgrCfgTransId  transId,
U8             status
)
#else
PUBLIC S16 VeLiRgrCfgCfm(pst, suId, transId, status)
Pst            *pst;          
SuId           suId;
RgrCfgTransId  transId;
U8             status;
#endif
{
   U8 idx = 0;
   U8  lchId = 0;
   U16 cfgType = 0;
   U16 crnti = 0;

  /* TIC_ID ccpu00120182 ADD : For disabling SIBs */
#if defined(SI_NEW) && defined(RGR_RRM_TICK)
  U8 numSiConfigured = 0; /* starting from SIB2 */
#endif

   VeUeCb *ueCb = NULLP;

   TRC2(VeLiRgrCfgCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiRgrCfgCfm: RGR Cfg Cfm \n"));
#endif

   /* fetch values from the transId */
   VE_GET_VALS_RGR_TRANS(transId.trans, cfgType, lchId, crnti);

   if(RGR_CFG_CFM_NOK == status)
   {
#ifdef DEBUGP
#ifdef ALIGN_64BIT
      VE_DBG_ERROR((VE_PRNT_BUF, "VeLiRgrCfgCfm status NOK \
               for Cfg Type:%d Crnti:%d Cell:%d\n", cfgType, \
               crnti, veCb.cellCb[0]->cellId));
#else
      VE_DBG_ERROR((VE_PRNT_BUF, "VeLiRgrCfgCfm status NOK \
               for Cfg Type:%d Crnti:%d Cell:%ld\n", cfgType, \
               crnti, veCb.cellCb[0]->cellId));
#endif
#endif
      RETVALUE(RFAILED);
   }

   if(cfgType == RGR_CELL_CFG)
   {
#ifdef SI_NEW
      Pst             *sipst = NULLP;
      RgrSiCfgReqInfo *siCfgReq;
      RgrCfgTransId   sitransId;
#endif
      /* Send the Ctf Cell Configuration */
      if(ROK != veSndCtfCellCfgReq(veCb.cellCb[0]->cellId))
      {
#ifdef ALIGN_64BIT
         VE_DBG_ERROR((VE_PRNT_BUF, "VeLiRgrCfgCfm : veSndCtfCellCfgReq failed %d\n", veCb.cellCb[0]->cellId));
#else
         VE_DBG_ERROR((VE_PRNT_BUF, "VeLiRgrCfgCfm : veSndCtfCellCfgReq failed %ld\n", veCb.cellCb[0]->cellId));
#endif
         RETVALUE(RFAILED);
      }

#if defined(SI_NEW) && defined(RGR_RRM_TICK)
      sipst = &veCb.rgrSap[0]->pst;
      sipst->event = EVTRGRSICFGREQ;

      /* Configure MIB */
      /* Allocate RGR Cfg struct */
      VE_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
      if(siCfgReq == NULLP)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgCfm:\
                  Failure in the allocation of configuration info\n"));
#endif
         RETVALUE(RFAILED);
      } /* end of if statement */

      /* set Cfg type as 5(base)+offset. where offset is based on
       * type of SI being configured. base is 5 since 1 to 4 values
       * are already in use by RGR CFG for cell, ue, lch, lcg. */
      VE_SET_RGR_TRANS(sitransId.trans, (5+RGR_SI_CFG_TYPE_MIB), VE_VAL_ZERO, VE_VAL_ZERO);
      siCfgReq->cellId = (U16)veCb.cellCb[0]->cellId;
      siCfgReq->cfgType = RGR_SI_CFG_TYPE_MIB;
      /* Build MIB pdu */
      veSibBldPdu(NHU_MSG_DLBCCHBCH, VE_SYS_MIB, &siCfgReq->pdu);
      if(ROK != VeLiRgrSiCfgReq(sipst, veCb.rgrSap[0]->spId, sitransId, siCfgReq))
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgCfm: VeLiRgrSiCfgReq primitive\
                  for MIB failed.\n"));      
         RETVALUE(RFAILED);
      }
   
      /* Configure SIB1 */
      /* Allocate RGR Cfg struct */
      VE_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
      if(siCfgReq == NULLP)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgCfm:\
                  Failure in the allocation of configuration info\n"));
#endif
         RETVALUE(RFAILED);
      } /* end of if statement */

      /* set Cfg type as 5(base)+offset. where offset is based on
       * type of SI being configured. base is 5 since 1 to 4 values
       * are already in use by RGR CFG for cell, ue, lch, lcg. */
      VE_SET_RGR_TRANS(sitransId.trans, (5+RGR_SI_CFG_TYPE_SIB1), VE_VAL_ZERO, VE_VAL_ZERO);
      siCfgReq->cellId = (U16)veCb.cellCb[0]->cellId;
      siCfgReq->cfgType = RGR_SI_CFG_TYPE_SIB1;
      /* Build SIB1 pdu */
      veSibBldPdu(NHU_MSG_DLBCCHDLSCH, VE_SYS_SIB1, &siCfgReq->pdu);
      if(ROK != VeLiRgrSiCfgReq(sipst, veCb.rgrSap[0]->spId, sitransId, siCfgReq))
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgCfm: VeLiRgrSiCfgReq primitive\
                  for SIB1 failed.\n"));      
         RETVALUE(RFAILED);
      }

      /* Configure SIB2 */
      /* TIC_ID ccpu00120182 ADD : For disabling SIBs */
      if(numSiConfigured < veDfltNumSi)
      {
      /* TIC_ID ccpu00120182 ADD_END : For disabling SIBs */
         /* Allocate RGR Cfg struct */
         VE_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
         if(siCfgReq == NULLP)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgCfm:\
                     Failure in the allocation of configuration info\n"));
#endif
            RETVALUE(RFAILED);
         } /* end of if statement */

         /* set Cfg type as 5(base)+offset. where offset is based on
          * type of SI being configured. base is 5 since 1 to 4 values
          * are already in use by RGR CFG for cell, ue, lch, lcg. */
         VE_SET_RGR_TRANS(sitransId.trans, (5+RGR_SI_CFG_TYPE_SI), VE_VAL_ZERO, VE_VAL_ZERO);
         siCfgReq->cellId = (U16)veCb.cellCb[0]->cellId;
         siCfgReq->cfgType = RGR_SI_CFG_TYPE_SI;
         siCfgReq->siId = 1;
         /* Build SIB2 pdu */
         veSibBldPdu(NHU_MSG_DLBCCHDLSCH, VE_SYS_SIB2, &siCfgReq->pdu);
         if(ROK != VeLiRgrSiCfgReq(sipst, veCb.rgrSap[0]->spId, sitransId, siCfgReq))
         {
            VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgCfm: VeLiRgrSiCfgReq primitive\
                     for SIB2 failed.\n"));      
            RETVALUE(RFAILED);
         }
      /* TIC_ID ccpu00120182 ADD : For disabling SIBs */
         numSiConfigured++;
      }   
      /* TIC_ID ccpu00120182 ADD_END : For disabling SIBs */

      /* Configure SIB3 */
      /* TIC_ID ccpu00120182 ADD : For disabling SIBs */
      if(numSiConfigured < veDfltNumSi)
      {
      /* TIC_ID ccpu00120182 ADD_END : For disabling SIBs */
         /* Allocate RGR Cfg struct */
         VE_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
         if(siCfgReq == NULLP)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgCfm:\
                     Failure in the allocation of configuration info\n"));
#endif
            RETVALUE(RFAILED);
         } /* end of if statement */

         /* set Cfg type as 5(base)+offset. where offset is based on
          * type of SI being configured. base is 5 since 1 to 4 values
          * are already in use by RGR CFG for cell, ue, lch, lcg. */
         VE_SET_RGR_TRANS(sitransId.trans, (5+RGR_SI_CFG_TYPE_SI), VE_VAL_ZERO, VE_VAL_ZERO);
         siCfgReq->cellId = (U16)veCb.cellCb[0]->cellId;
         siCfgReq->cfgType = RGR_SI_CFG_TYPE_SI;
         siCfgReq->siId = 2;
         /* Build SIB3 pdu */
         veSibBldPdu(NHU_MSG_DLBCCHDLSCH, VE_SYS_SIB3, &siCfgReq->pdu);
         if(ROK != VeLiRgrSiCfgReq(sipst, veCb.rgrSap[0]->spId, sitransId, siCfgReq))
         {
            VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgCfm: VeLiRgrSiCfgReq primitive\
                     for SIB3 failed.\n"));      
            RETVALUE(RFAILED);
         }
      /* TIC_ID ccpu00120182 ADD : For disabling SIBs */
         numSiConfigured++;
      }   
      /* TIC_ID ccpu00120182 ADD_END : For disabling SIBs */

      /* Configure SIB4 */
      /* TIC_ID ccpu00120182 ADD : For disabling SIBs */
      if(numSiConfigured < veDfltNumSi)
      {
      /* TIC_ID ccpu00120182 ADD_END : For disabling SIBs */
         /* Allocate RGR Cfg struct */
         VE_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
         if(siCfgReq == NULLP)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgCfm:\
                     Failure in the allocation of configuration info\n"));
#endif
            RETVALUE(RFAILED);
         } /* end of if statement */

         /* set Cfg type as 5(base)+offset. where offset is based on
          * type of SI being configured. base is 5 since 1 to 4 values
          * are already in use by RGR CFG for cell, ue, lch, lcg. */
         VE_SET_RGR_TRANS(sitransId.trans, (5+RGR_SI_CFG_TYPE_SI), VE_VAL_ZERO, VE_VAL_ZERO);
         siCfgReq->cellId = (U16)veCb.cellCb[0]->cellId;
         siCfgReq->cfgType = RGR_SI_CFG_TYPE_SI;
         siCfgReq->siId = 3;
         /* Build SIB4 pdu */
         veSibBldPdu(NHU_MSG_DLBCCHDLSCH, VE_SYS_SIB4, &siCfgReq->pdu);
         if(ROK != VeLiRgrSiCfgReq(sipst, veCb.rgrSap[0]->spId, sitransId, siCfgReq))
         {
            VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgCfm: VeLiRgrSiCfgReq primitive\
                     for SIB4 failed.\n"));      
            RETVALUE(RFAILED);
         }
      /* TIC_ID ccpu00120182 ADD : For disabling SIBs */
         numSiConfigured++;
      }   
      /* TIC_ID ccpu00120182 ADD_END : For disabling SIBs */

      /* Configure SIB5 */
      /* TIC_ID ccpu00120182 ADD : For disabling SIBs */
      if(numSiConfigured < veDfltNumSi)
      {
      /* TIC_ID ccpu00120182 ADD_END : For disabling SIBs */
         /* Allocate RGR Cfg struct */
         VE_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
         if(siCfgReq == NULLP)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgCfm:\
                     Failure in the allocation of configuration info\n"));
#endif
            RETVALUE(RFAILED);
         } /* end of if statement */

         /* set Cfg type as 5(base)+offset. where offset is based on
          * type of SI being configured. base is 5 since 1 to 4 values
          * are already in use by RGR CFG for cell, ue, lch, lcg. */
         VE_SET_RGR_TRANS(sitransId.trans, (5+RGR_SI_CFG_TYPE_SI), VE_VAL_ZERO, VE_VAL_ZERO);
         siCfgReq->cellId = (U16)veCb.cellCb[0]->cellId;
         siCfgReq->cfgType = RGR_SI_CFG_TYPE_SI;
         siCfgReq->siId = 4;
         /* Build SIB3 pdu */
         veSibBldPdu(NHU_MSG_DLBCCHDLSCH, VE_SYS_SIB5, &siCfgReq->pdu);
         if(ROK != VeLiRgrSiCfgReq(sipst, veCb.rgrSap[0]->spId, sitransId, siCfgReq))
         {
            VE_DBG_ERROR((VE_PRNT_BUF,"VeLiRgrCfgCfm: VeLiRgrSiCfgReq primitive\
                     for SIB5 failed.\n"));      
            RETVALUE(RFAILED);
         }
      /* TIC_ID ccpu00120182 ADD : For disabling SIBs */
         numSiConfigured++;
      }   
      /* TIC_ID ccpu00120182 ADD_END : For disabling SIBs */
#endif
      RETVALUE(ROK);
   } /* end of if statement */

#ifdef SI_NEW
   if ((cfgType == (5+RGR_SI_CFG_TYPE_MIB)) ||
      (cfgType == (5+RGR_SI_CFG_TYPE_SIB1)) ||
      (cfgType == (5+RGR_SI_CFG_TYPE_SI)))
   {
      RETVALUE(ROK);
   }
#endif
   /* Checking if LCH Cfg */
   VE_GET_UECB(ueCb, crnti);

   if(ueCb != NULLP)
   {
      switch(cfgType)
      {
         case RGR_UE_CFG:
           /* Trigger RGR LCH Cfg for SRB */
           for (idx = 0; idx < ueCb->rbInfo.numOfSrbCfgd; idx++)
           {
               veSndRgrLchCfg(ueCb, &ueCb->rbInfo.srb[idx],FALSE);
           }
           break;
         case RGR_LCH_CFG:
            if((lchId == VE_PDCP_ID_SRB1) || (lchId == VE_PDCP_ID_SRB2))
            {
               ueCb->rbInfo.rgrCfgMask |= VE_RGR_SRB_CFG_LCH;
                  /* Send RGR LCG Cfg Group 0*/
                  veSndRgrLcgCfg(ueCb, lchId, veDfltSrbLogchcfgGrp);
            }
            else
            {
               ueCb->rbInfo.rgrCfgMask |= VE_RGR_DRB_CFG_LCH;
                  /* Send RGR LCG Cfg for Group 1 */
                  veSndRgrLcgCfg(ueCb, lchId, veDfltDrbGrp);
            }
            break;
         case RGR_LCG_CFG:
            if((lchId == VE_PDCP_ID_SRB1) || (lchId == VE_PDCP_ID_SRB2))
            {
               ueCb->rbInfo.rgrCfgMask |= VE_RGR_SRB_CFG_LCG;
            }
            else
            {
               ueCb->rbInfo.rgrCfgMask |= VE_RGR_DRB_CFG_LCG;
            }
            break;
         default:
            break;
      }
   }
   else
   {
#ifdef DEBUGP
#ifdef ALIGN_64BIT
      VE_DBG_ERROR((VE_PRNT_BUF, "VeLiRgrCfgCfm UE Cb not located \
               for Cfg Type:%d Crnti:%d Cell:%d\n", cfgType, crnti, veCb.cellCb[0]->cellId));
#else
      VE_DBG_ERROR((VE_PRNT_BUF, "VeLiRgrCfgCfm UE Cb not located \
               for Cfg Type:%d Crnti:%d Cell:%ld\n", cfgType, crnti, veCb.cellCb[0]->cellId));
#endif 
#endif 
   }

   RETVALUE(ROK);
} /* end of VeLiRgrCfgCfm */

#ifdef RGR_RRM_TICK
/*
*
*       Fun:   NxLiRgrTtiInd
*
*       Desc:  Tti Indication 
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 NxLiRgrTtiInd
(
Pst            *pst,          
SuId           suId,
RgrTtiIndInfo  *ttiInd
)
#else
PUBLIC S16 NxLiRgrTtiInd(pst, suId, ttiInd)
Pst            *pst;          
SuId           suId;
RgrTtiIndInfo  *ttiInd;
#endif
{
   TRC2(NxLiRgrTtiInd);
   
   RETVALUE(VeLiRgrTtiInd(pst, suId, ttiInd));
} /* end of NxLiRgrTtiInd */

/*
*
*       Fun:   VeLiRgrTtiInd
*
*       Desc:  Tti Indication 
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiRgrTtiInd
(
Pst            *pst,          
SuId           suId,
RgrTtiIndInfo  *ttiInd
)
#else
PUBLIC S16 VeLiRgrTtiInd(pst, suId, ttiInd)
Pst            *pst;          
SuId           suId;
RgrTtiIndInfo  *ttiInd;
#endif
{

   TRC2(VeLiRgrTtiInd);

#ifdef DEBUGP   
    VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiRgrTtiInd: RGR TTI Indication\n")); 
#endif
 
#ifndef SI_NEW
   /* TTI indication handler */
   veSibHdlTmrInd(ttiInd);
#endif
   /* TTI indication handler */
   vePagHdlTmrInd(ttiInd);
   VE_FREE(ttiInd, sizeof (RgrTtiIndInfo));

   RETVALUE(ROK);
} /* end of VeLiRgrTtiInd */
#endif 


/**************************************************************
 CTF Interface functions
***************************************************************/
/*
*
*       Fun:   VeLiCtfBndCfm
*
*       Desc:  Bind Confirm 
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiCtfBndCfm
(
Pst       *pst,         
SuId      suId,        
U8        status      
)
#else
PUBLIC S16 VeLiCtfBndCfm(pst, suId, status)
Pst       *pst;      
SuId      suId;     
U8        status;  
#endif
{
   TRC2(VeLiCtfBndCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiCtfBndCfm: CTF Bind Cfm \n"));
#endif

   /* Send alarm */
   if (CM_BND_OK == status)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCtfBndCfm: CTF Bind Successful\n"));
#endif
      veStopTmr((PTR)veCb.ctfSap[0],VE_TMR_CTF_SAP_BND);
      veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, LVE_CAUSE_CTF_SAP_BOUND);
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCtfBndCfm: CTF Bind Unsuccessful\n"));
#endif
      veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
   }

   RETVALUE(ROK);
} /* end of VeLiCtfBndCfm */

/*
*
*       Fun:   VeLiCtfCfgCfm
*
*       Desc:  Config Cfm
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiCtfCfgCfm
(
Pst *pst,             
SuId suId,           
CtfCfgTransId transId,
U8 status            
)
#else
PUBLIC S16 VeLiCtfCfgCfm(pst, suId, transId, status)
Pst *pst;           
SuId suId;         
CtfCfgTransId transId; 
U8 status;            
#endif
{

   TRC3(VeLiCtfCfgCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiCtfCfgCfm:\
            CTF Cfg Cfm \n"));
#endif

   if(status != CTF_CFG_CFM_OK)
   {
#ifdef DEBUGP
#ifdef ALIGN_64BIT
      VE_DBG_ERROR((VE_PRNT_BUF, "VeLiCtfCfgCfm Status failed: %d\n",\
               veCb.cellCb[0]->cellId));
#else
      VE_DBG_ERROR((VE_PRNT_BUF, "VeLiCtfCfgCfm Status failed: %ld\n",\
               veCb.cellCb[0]->cellId));
#endif
#endif
      RETVALUE(RFAILED);
   }

   /* Send alarm */
   veSendLmAlarm(LCM_CATEGORY_PROTOCOL, LVE_EVENT_CELL_CFG_SUCCESS, LCM_CAUSE_UNKNOWN);

   RETVALUE(ROK);
}  /* end of VeLiCtfCfgCfm */

/*
*
*       Fun:   VeLiCtfUeIdChgCfm
*
*       Desc:  Ue Id Change Cfm
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiCtfUeIdChgCfm
(
Pst       *pst,            
SuId       suId,
CtfCfgTransId transId,
CtfUeInfo *ueInfo, 
U8 status
)  
#else
PUBLIC S16 VeLiCtfUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst        *pst;             
SuId        suId;
CtfCfgTransId         transId;
CtfUeInfo  *ueInfo;
U8 status;
#endif
{
   TRC3(VeLiCtfUeIdChgCfm)

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiCtfUeIdChgCfm: CTF UeId Change Cfm \n"));
#endif

   RETVALUE(ROK);
}  /* end of VeLiCtfUeIdChgCfm */

#ifndef LTE_ENB_PERF
/**************************************************************
 SZT Interface functions
***************************************************************/
/*
*
*       Fun:   VeLiSztUDatInd
*
*       Desc:  SZT UDat Ind
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiSztUDatInd
(
Pst     *pst,
SuId     suId,
SztUDatEvnt *uDatEvnt
)
#else
PUBLIC S16 VeLiSztUDatInd (pst,suId,uDatEvnt)
Pst     *pst;          /* post structure */
SuId     suId;
SztUDatEvnt *uDatEvnt;
#endif
{
   U8 evnt;
   VeS1ConCb s1ConCb;
   TknU32   *s1TimeToWait = NULLP;
   U16      delay = 0;
   VeMmeCb  *mmeCb = NULLP;

   TRC2(VeLiSztUDatInd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiSztUDatInd:\
            SZT UData Ind \n"));
#endif

   veSztUtilsGetMsgIdx (&evnt, uDatEvnt->pdu);

   VE_GET_MME_CB(uDatEvnt->peerId.val, mmeCb);
   if(mmeCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiSztUDatInd:\
               Could not locate MmeCb\n"));
#endif
      RETVALUE(RFAILED);
   }

   /*Successful outcome*/
   if((evnt == SZT_MSG_ID_SETUP_RSP) || (evnt == SZT_MSG_ID_SETUP_FAIL))
   {
      if((uDatEvnt->pdu->pdu.val.successfulOutcome.procedureCode.val == Sztid_S1Setup)
          && (uDatEvnt->pdu->pdu.choice.val == SZT_TRGR_SUCCESS_OUTCOME))
      {
         mmeCb->s1Setup = VE_S1_SETUP_DONE;
         mmeCb->state = VE_S1_SETUP_DONE;
         /* Send alarm */
         veSendLmAlarm(LCM_CATEGORY_PROTOCOL, LVE_EVENT_S1CON_SET_SUCCESS, LCM_CAUSE_UNKNOWN);

         veSetupResp(uDatEvnt->peerId.val, uDatEvnt->pdu);
  
#ifdef VE_TEST_CODE
         /* Start a timer to test this feature. This will be started for sending the 
          * eNB initiated reset and configuration update */
         cmInitTimers(&(mmeCb->tstS1Rest), 1);
         cmInitTimers(&(mmeCb->tstenbUpd), 1);

         veStartTmr((PTR)mmeCb, VE_TMR_TEST_ENB_UPD, 1200);
         veStartTmr((PTR)mmeCb, VE_TMR_TEST_ENB_RST, 3000);
#endif /* VE_TEST_CODE */
      }
      else
      {
         /* Get the time to wait IE from the pdu and if the IE is present,
          * start timer to retry the S1-Setup */
         veSzGetIE(&(uDatEvnt->pdu->pdu), Sztid_TimeToWait, (TknU8**)&s1TimeToWait);
         if(s1TimeToWait != NULLP)
         {
            cmInitTimers(&(mmeCb->s1SetupTmr), 1);

            /* Convert the time to wait value into corret value which
             * needs to be used in timer function */
            VE_GET_TIME_TO_WAIT_VAL(s1TimeToWait->val, delay);

            veStopTmr((PTR)mmeCb, VE_TMR_S1SETUP_TMR);
            veStartTmr((PTR)mmeCb, VE_TMR_S1SETUP_TMR, delay);
         }
         /* If the time to wait IE is not present, pass the alarm to LM */
         else
         {
            veSendLmAlarm(LCM_CATEGORY_PROTOCOL, LVE_EVENT_S1CON_SET_FAIL,
                          LCM_CAUSE_UNKNOWN);
         }
      }
   }
   else
   {
      s1ConCb.peerId = uDatEvnt->peerId.val;
      veSztLIMFSM[evnt](&s1ConCb, uDatEvnt->pdu);
   }

   cmFreeMem ((Ptr)(uDatEvnt->pdu));
   RETVALUE(ROK);
} /* VeLiSztUDatInd */ 

/*
*
*       Fun:   VeLiSztConCfm
*
*       Desc:  SZT Connection Confirm 
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiSztConCfm
(
Pst     *pst,
SuId     suId,
SztConCfm      *conCfm
)
#else
PUBLIC S16 VeLiSztConCfm (pst,suId,conCfm)
Pst     *pst;          /* post structure */
SuId     suId;
SztConCfm      *conCfm;
#endif
{
   U8 evnt;
   VeS1ConCb *s1ConCb = NULLP; 
   TknU8     *mmeIdTkn = NULLP; 
   TknU8     *enbIdTkn = NULLP; 

   TRC2(VeLiSztConCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiSztConCfm:\
            SZT Conn. Cfm\n"));
#endif

   /* Locate S1ConCb */ 
   s1ConCb = veCb.s1apConLst[conCfm->suConnId];
   if(s1ConCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiSztConCfm:\
               Could not locate S1AP Conn Cb \n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Fetch info from pdu */
   veSztUtilsGetMsgIdx (&evnt, conCfm->pdu);
   veSzGetIE(&conCfm->pdu->pdu, Sztid_MME_UE_S1AP_ID, &mmeIdTkn);
   veSzGetIE(&conCfm->pdu->pdu, Sztid_eNB_UE_S1AP_ID, &enbIdTkn);

   if(enbIdTkn == NULLP || mmeIdTkn == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiSztConCfm:\
               veSzGetIE failed\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Update S1AP Con Cb */
   s1ConCb->mme_ue_s1ap_id = ((TknU32 *)mmeIdTkn)->val;
   s1ConCb->enb_ue_s1ap_id = ((TknU32 *)enbIdTkn)->val;
   s1ConCb->spConnId = conCfm->spConnId;
   s1ConCb->s1apConnState = VE_S1AP_CONNECTED;

   /* State machine */
   veSztLIMFSM[evnt](s1ConCb,conCfm->pdu);

   RETVALUE(ROK);
}

/*
*
*       Fun:   VeLiSztBndCfm
*
*       Desc:  SZT Bind Confirm
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiSztBndCfm
(
Pst            *pst,
SuId           suId,
U8             status
)
#else
PUBLIC S16 VeLiSztBndCfm(pst, suId, status)
Pst            *pst;
SuId           suId;
U8             status;
#endif
{

   TRC2(VeLiSztBndCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiSztBndCfm:\
            SZT Bind Confirm \n"));
#endif

   /* Send alarm */
   if (CM_BND_OK == status)
   {
#ifdef DEBUGP
      VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiSztBndCfm:\
               SZT Bind Successful\n"));
#endif
      veStopTmr((PTR)veCb.sztSap[0],VE_TMR_SZT_SAP_BND);
      veSendLmAlarm(LCM_CATEGORY_INTERFACE,\
            LCM_EVENT_BND_OK, LVE_CAUSE_SZT_SAP_BOUND);
   }
   else
   {
      veSendLmAlarm(LCM_CATEGORY_INTERFACE,\
            LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
   }
   RETVALUE(ROK);
} /* End of VeLiSztBndCfm */

/*
*
*       Fun:   VeLiSztConInd
*
*       Desc:  SZT Con Ind
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiSztConInd
(
Pst            *pst,
SuId           suId,
SztConInd      *conInd
)
#else
PUBLIC S16 VeLiSztConInd(pst, suId, conInd)
Pst            *pst;
SuId           suId;
SztConInd      *conInd;
#endif /* ANSI */
{
#ifdef LTE_HO_SUPPORT
   U8 evnt;
   VeS1ConCb *s1apConCb = NULLP;
#endif/* End of LTE_HO_SUPPORT */
   
   TRC2(VeLiSztConInd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiSztConInd: SZT Conn. Ind \n"));
#endif
   
#ifdef LTE_HO_SUPPORT
   veSztUtilsGetMsgIdx(&evnt, conInd->pdu);
   if(SZT_MSG_ID_HO_REQ == evnt)
   {
      /* Generate the suConnId, allocate VeS1ConCb and fill the values*/
      /* Add the conCb in veCb global control block */
      VE_ALLOC(&s1apConCb, sizeof(VeS1ConCb));

      if(NULLP == s1apConCb)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiSztConInd:\
               Failed to allocate Memory.\n"));
#endif
      RETVALUE(RFAILED); 
      }

      /* Initialize control blocks */
      s1apConCb->spConnId = conInd->u.spConnId;
      s1apConCb->s1apConnState = VE_S1AP_CONNECTING;
      VE_GET_MME_CB(conInd->peerId.val, s1apConCb->mmeCb);
      if(s1apConCb->mmeCb == NULLP)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiSztConInd:\
                  Could not locate MmeCb\n"));
#endif
         RETVALUE(RFAILED);
      }

      /* Setting enb_ue_s1ap_id to 1. 
       * TODO : Logic will be added as part of multi ue support */
      s1apConCb->enb_ue_s1ap_id = 1;

      veSztLIMFSM[evnt](s1apConCb, conInd->pdu);
   }
#endif/* End of LTE_HO_SUPPORT */

   RETVALUE(ROK);
} /* End of VeLiSztConInd */

/*
*
*       Fun:   VeLiSztRelCfm
*
*       Desc:  SZT Release Confirm
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiSztRelCfm
(
Pst            *pst,
SuId           suId,
SztRelCfm      *relCfm
)
#else
PUBLIC S16 VeLiSztRelCfm(pst, suId, relCfm)
Pst            *pst;
SuId           suId;
SztRelCfm      *relCfm;
#endif /* ANSI */
{
   TRC2(VeLiSztRelCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiSztRelCfm: SZT Release Cfm \n"));
#endif

   RETVALUE(ROK);
} /* End of VeLiSztRelCfm */


/*
*
*       Fun:   VeLiSztDatInd
*
*       Desc:  SZT Data Ind.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiSztDatInd
(
Pst            *pst,
SuId           suId,
SztDatEvntInd  *datEvnt
)
#else
PUBLIC S16 VeLiSztDatInd(pst, suId, datEvnt)
Pst            *pst;
SuId           suId;
SztDatEvntInd  *datEvnt;
#endif /* ANSI */
{
   VeS1ConCb *s1ConCb = NULLP;
   U8    evnt;

   TRC2(VeLiSztDatInd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiSztDatInd: SZT Data Ind\n"));
#endif

   veSztUtilsGetMsgIdx(&evnt, datEvnt->pdu);

   s1ConCb = veCb.s1apConLst[datEvnt->suConnId]; 
   if (s1ConCb != NULLP)
   {
      /* fetch evnt and call appropriate state machine function */
      veSztLIMFSM[evnt](s1ConCb, datEvnt->pdu);
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiSztDatInd:\
               Could not locate S1AP Conn. Cb\n"));
#endif
      cmFreeMem ((Ptr)(datEvnt->pdu));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* End of VeLiSztDatInd */

/*
*
*       Fun:   VeLiSztRelInd
*
*       Desc:  SZT Release Ind.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiSztRelInd
(
Pst            *pst,
SuId           suId,
SztRelInd      *relInd
)
#else
PUBLIC S16 VeLiSztRelInd(pst, suId, relInd)
Pst            *pst;
SuId           suId;
SztRelInd      *relInd;
#endif /* ANSI */
{
   VeS1ConCb *s1ConCb = NULLP;
   U8    evnt;

   TRC2(VeLiSztRelInd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiSztRelInd:\
            SZT Release Indication\n"));
#endif


   s1ConCb = veCb.s1apConLst[relInd->suConnId.val];

   if(s1ConCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiSztRelInd :\
               Could not locate S1AP Conn. Cb\n"));
#endif
      RETVALUE(RFAILED);
   }


   veSztUtilsGetMsgIdx (&evnt, relInd->pdu);
   veSztLIMFSM[evnt](s1ConCb,relInd->pdu);

   RETVALUE(ROK);
} /* End of VeLiSztRelInd */

/*
*
*       Fun:   VeLiSztStaInd
*
*       Desc:  Call handler for Status Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_sz_ptli.c
*       
*/
#ifdef ANSI
PUBLIC S16 VeLiSztStaInd
(
Pst            *post,
SuId           suId,
SztStaInd      *sztSta
)
#else
PUBLIC S16 VeLiSztStaInd(post, suId, sztSta)
Pst            *post;
SuId           suId;
SztStaInd      *sztSta;
#endif /* ANSI */
{
   VeCellCb *cellCb = NULLP; 
   VeUeCb   *ueCb = NULLP;
   U8       ueIdx = 0;
   U8       maxNumUEs = 0;

   TRC2(VeLiSztStaInd);

   /* Verify if Assoc is down; in that case RRC Connection Release
    * is sent to all UEs and hence leading to release of resources 
    * */
   if(sztSta->type == SZT_STA_TYPE_ASSOC)
   {
      if(sztSta->status == SZT_STATUS_ASSOC_DOWN)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiSztStaInd :\
                  Assoc. Down Reason (SZT) %x\n", sztSta->reason));
#endif

         /* Get Cell Cb */
         cellCb = veCb.cellCb[0];
         maxNumUEs = cellCb->numUeCfg;

         for(ueIdx = 0; ueIdx < maxNumUEs; ueIdx++)
         {
            ueCb = cellCb->ueCbLst[ueIdx];
            /* Trigger RRC Connection Release Signaling */
            if(ueCb != NULLP)
            {
               veSndRrcConRel(ueCb);
            }
         }
      }
   }

   RETVALUE(ROK);
} /* End of VeLiSztStaInd */

#ifdef LTE_HO_SUPPORT
/*
*
*       Fun:   VeLiCztBndCfm
*       
*       Desc:  Call handler for Bind Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_li.c
*       
*/
#ifdef ANSI
PUBLIC S16 VeLiCztBndCfm
(
Pst            *pst,
SuId           suId,
U8             status
)
#else
PUBLIC S16 VeLiCztBndCfm(pst, suId, status)
Pst            *pst;
SuId           suId;
U8             status;
#endif
{
   TRC2(VeLiCztBndCfm);
 
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiCztBndCfm:\
            CZT Bind Confirm\n"));
#endif

   /* Send alarm */
   if (CM_BND_OK == status)
   {
#ifdef DEBUGP
      VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiCztBndCfm:\
               SZT Bind Successful\n"));
#endif
      veStopTmr((PTR)veCb.cztSap[0],VE_TMR_CZT_SAP_BND);
      veSendLmAlarm(LCM_CATEGORY_INTERFACE,\
            LCM_EVENT_BND_OK, LVE_CAUSE_CZT_SAP_BOUND);
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCztBndCfm: Binding Unsuccessful\n"));
      veSendLmAlarm(LCM_CATEGORY_INTERFACE,\
            LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
   }
   RETVALUE(ROK);
} /* End of VeLiCztBndCfm */


/*
*
*       Fun:   VeLiCztGpInd
*
*       Desc:  Call handler for Release Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_li.c
*       
*/
#ifdef ANSI
PUBLIC S16 VeLiCztGpInd
(
Pst            *pst,
SuId           suId,
CztEvnt      *cztInd
)
#else
PUBLIC S16 VeLiCztGpInd(pst, suId, cztInd)
Pst            *pst;
SuId           suId;
CztEvnt      *cztInd;
#endif /* ANSI */
{
   S16         msgIdx = CZT_MSG_UNKNOWN;
   TRC2(VeLiCztGpInd);
   
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiCztGpInd:\
            CZT General Procedure Indication\n"));
#endif

   msgIdx = veCztUtilsGetMsgIdx (&cztInd->pdu);
   if(CZT_MSG_UNKNOWN == msgIdx)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCztGpInd:\
                     Invalid X2AP message ID\n"));
#endif
      cmFreeMem((Ptr *)cztInd);
      RETVALUE(RFAILED);
   }
   
   veCztLIMFSM[msgIdx](NULLP, cztInd);

   RETVALUE(ROK);
} /* End of VeLiCztGpInd */


/*
*
*       Fun:   VeLiCztGpCfm
*
*       Desc:  Call handler for Release Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_li.c
*       
*/
#ifdef ANSI
PUBLIC S16 VeLiCztGpCfm
(
Pst          *pst,
SuId         suId,
CztEvnt      *cztCfm
)
#else
PUBLIC S16 VeLiCztGpCfm(pst, suId, cztCfm)
Pst          *pst;
SuId         suId;
CztEvnt      *cztCfm;
#endif /* ANSI */
{
   S16         msgIdx = CZT_MSG_UNKNOWN;

   TRC2(VeLiCztGpCfm);
   
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiCztGpCfm:\
            CZT General Procedure Confirm\n"));
#endif

   msgIdx = veCztUtilsGetMsgIdx (&cztCfm->pdu);
   if(CZT_MSG_UNKNOWN == msgIdx)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCztGpCfm:\
                     Invalid X2AP message ID\n"));
#endif
      cmFreeMem((Ptr *)cztCfm);
      RETVALUE(RFAILED);
   }
   
   veCztLIMFSM[msgIdx](NULLP, cztCfm);

   RETVALUE(ROK);
} /* End of VeLiCztGpCfm */


/*
*
*       Fun:   VeLiCztBmpInd
*
*       Desc:  Call handler for Release Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_li.c
*       
*/
#ifdef ANSI
PUBLIC S16 VeLiCztBmpInd
(
Pst            *pst,
SuId           suId,
CztEvnt        *cztInd
)
#else
PUBLIC S16 VeLiCztBmpInd(pst, suId, cztInd)
Pst            *pst;
SuId           suId;
CztEvnt        *cztInd;
#endif /* ANSI */
{
   VeX2ConCb      *x2ConCb = NULLP;
   S16            msgIdx = CZT_MSG_UNKNOWN;
   TknU8          *ie = NULLP;
   U32            ueX2apId = 0;

   printf("\n--------------HO REQ received in BMP IND-----\n");
   TRC2(VeLiCztBmpInd);
   
   if(cztInd == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCztBmpInd:\
               invalid cztInd parameter\n"));
      RETVALUE(RFAILED);
   }
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiCztBmpInd:\
            CZT BMP Indication\n"));
#endif

   msgIdx = veCztUtilsGetMsgIdx (&cztInd->pdu);
   if(CZT_MSG_UNKNOWN == msgIdx)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCztBmpInd:\
               Invalid X2AP message ID\n"));
#endif
      cmFreeMem ((Ptr *)&(cztInd->pdu));
      RETVALUE(RFAILED);
   }

   ie = NULLP;

   veUtlGetCztIE(&cztInd->pdu, Cztid_Old_eNB_UE_X2AP_ID, (TknU8 **)&ie);
   if(ie == NULLP)
   {
      RETVALUE(RFAILED);
   }

   ueX2apId = (ie->val);
   
   x2ConCb = veCb.x2apConLst[VE_GET_UE_X2_IDX(ueX2apId)];
   if(NULLP == x2ConCb && CZT_MSG_HO_REQ == msgIdx)
   {
      VE_ALLOC(&x2ConCb, sizeof(VeX2ConCb));
      if(NULLP == x2ConCb)
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCztBmpInd:\
                  Alloc for new X2ConCb Failed\n"));
         cmFreeMem ((Ptr)&(cztInd->pdu));
         RETVALUE(RFAILED);
      }
      x2ConCb->peerId = cztInd->peerId;
      x2ConCb->hoNodeType = TGT_ENODEB;
   }
   else if(NULLP == x2ConCb)     
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCztBmpInd:\
               Invalid peerId\n"));
      RETVALUE(RFAILED);
   }

   veCztLIMFSM[msgIdx](x2ConCb, cztInd);
   
   RETVALUE(ROK);
} /* End of VeLiCztBmpInd */


/*
*
*       Fun:   VeLiCztBmpCfm
*
*       Desc:  Call handler for Connection Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_li.c
*       
*/
#ifdef ANSI
PUBLIC S16 VeLiCztBmpCfm
(
Pst          *pst,
SuId         suId,
CztEvnt      *cztCfm
)
#else
PUBLIC S16 VeLiCztBmpCfm(pst, suId, cztCfm)
Pst          *pst;
SuId         suId;
CztEvnt      *cztCfm;
#endif /* ANSI */
{
   VeX2ConCb  *x2ConCb = NULLP;
   S16        msgIdx = CZT_MSG_UNKNOWN;
   TknU8      *ie = NULLP;
   U32        ueX2apId = 0;

   TRC2(VeLiCztBmpCfm);
   
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiCztGpCfm:\
            CZT BMP Indication\n"));
#endif

   if(cztCfm == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCztBmpCfm:\
                     invalid cztInd parameter\n"));
      RETVALUE(RFAILED);
   }
 
   msgIdx = veCztUtilsGetMsgIdx (&cztCfm->pdu);
   if(CZT_MSG_UNKNOWN == msgIdx)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCztBmpCfm:\
                     Invalid X2AP message ID\n"));
#endif
      cmFreeMem ((Ptr *)&(cztCfm->pdu));
      RETVALUE(RFAILED);
   }

   ie = NULLP;
   veUtlGetCztIE(&cztCfm->pdu, Cztid_Old_eNB_UE_X2AP_ID, (TknU8 **)&ie);
   if(ie == NULLP)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCztBmpCfm:\
                     Error in decoding Cztid_Old_eNB_UE_X2AP_ID\n"));
      cmFreeMem ((Ptr *)&(cztCfm->pdu));
      RETVALUE(RFAILED);
   }

   ueX2apId = (ie->val);
   x2ConCb = veCb.x2apConLst[VE_GET_UE_X2_IDX(ueX2apId)];
   if(NULLP == x2ConCb)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCztBmpCfm:\
                     Invalid ueX2apId\n"));
      cmFreeMem ((Ptr *)&(cztCfm->pdu));
      RETVALUE(RFAILED);
   }
   veCztLIMFSM[msgIdx](x2ConCb, cztCfm);

   RETVALUE(ROK);
} /* End of VeLiCztBmpCfm */

/*
*
*       Fun:   VeLiCztGpErrInd
*
*       Desc:  Call handler for Connection based data indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_li.c
*       
*/
#ifdef ANSI
PUBLIC S16 VeLiCztGpErrInd
(
Pst      *pst,
SuId     suId,
CztEvnt  *cztEvnt
)
#else
PUBLIC S16 VeLiCztGpErrInd (pst, suId, cztEvnt)
Pst      *pst;
SuId     suId;
CztEvnt  *cztEvnt;
#endif /* ANSI */
{
   U16 msgIdx;
   TRC2(VeLiCztGpErrInd);
   
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiCztGpErrInd:\
            CZT Gp Error Indication\n"));
#endif

/* X2_CFG_UPD_CHANGES */
   msgIdx = veCztUtilsGetMsgIdx (&cztEvnt->pdu);
  printf("msgIdx ==  %d\n", msgIdx);

   veCztLIMFSM[msgIdx](NULLP, cztEvnt);

   RETVALUE(ROK);
} /* End of VeLiCztGpErrInd */


/*
*
*       Fun:   VeLiCztLclErrorInd
*
*       Desc:  Call handler for Error Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_li.c
*       
*/
#ifdef ANSI
PUBLIC S16 VeLiCztLclErrorInd
(
Pst            *post,
SuId           suId,
CztLclErrInd   *cztLclErrInd
)
#else
PUBLIC S16 VeLiCztLclErrorInd(post, suId, cztLclErrInd)
Pst            *post;
SuId           suId;
CztLclErrInd   *cztLclErrInd;
#endif /* ANSI */
{
   TRC2(VeLiCztLclErrorInd);
  
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiCztLclErrorInd:\
            CZT Gp Local Error Indication\n"));
#endif

   VE_FREE(cztLclErrInd,sizeof(CztLclErrInd));

   /* Received Local Error Indication from X2AP layer */

   RETVALUE(ROK);
} /* End of VeLiCztLclErrorInd */

/*
*
*       Fun:   VeLiCztRelCfm
*
*       Desc:  Call handler for Connection Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_li.c
*       
*/
#ifdef ANSI
PUBLIC S16 VeLiCztRelCfm
(
Pst            *pst,
SuId           suId,
CztRlsCfm      *rlsCfm
)
#else
PUBLIC S16 VeLiCztRelCfm(pst, suId, rlsCfm)
Pst            *pst;
SuId           suId;
CztRlsCfm      *rlsCfm;
#endif /* ANSI */
{
   TRC2(VeLiCztRelCfm);

   /* Immediately after sending the RlsReq to X2AP the UE Context in VE is released/deleted.
      So we do not have a context to cross verify this RlsCfm.
    */
   /* Logging the reception of CFM */
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"VeLiCztRelCfm: Received for id:%d status:%d, reason:%d\n",\
      rlsCfm->oldUEX2APId, rlsCfm->status, rlsCfm->reason));
#endif


   /* Free the memory */
   SPutSBuf(pst->region, pst->pool, (Data *)rlsCfm, sizeof(CztRlsCfm));

   RETVALUE(ROK);
} /* End of VeLiCztRelCfm */


/*
*
*       Fun:   VeLiCztStaInd
*
*       Desc:  Call handler for Status Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_li.c
*       
*/
#ifdef ANSI
PUBLIC S16 VeLiCztStaInd
(
Pst            *post,
SuId           suId,
CztStaInd      *cztSta
)
#else
PUBLIC S16 VeLiCztStaInd(post, suId, cztSta)
Pst            *post;
SuId           suId;
CztStaInd      *cztSta;
#endif /* ANSI */
{
   VeUeCb    *ueCb = NULLP;
   VeCellCb  *cellCb = NULLP;
   VeX2ConCb *x2ConCb = NULLP;
   S16       ret;
   U16       event = 0;

   TRC2(VeLiCztStaInd);
 
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"VeLiCztStaInd: Received status:%d, reason:%d\n"\
      ,cztSta->status, cztSta->reason));
#endif

    /* To Get UeCb */
    cellCb = veCb.cellCb[0];

    ueCb = cellCb->ueCbLst[0];

   if(NULLP == ueCb)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"VeLiCztStaInd:ueCb is NULL. "));
#endif
      RETVALUE(RFAILED);
   }

   switch(cztSta->status)
   {
      case CZT_STATUS_UE_TRELOCPREP_EXPIRY:
      {
         x2ConCb = veCb.x2apConLst[VE_GET_UE_X2_IDX(cztSta->oldUEX2APId)];
         event = VE_EVNT_HO_PREP_TMR_EXPIRY;
         if( RFAILED == (ret = veX2SrcFSM[ueCb->mobCtrlState][event](x2ConCb, NULLP)))
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"VeLiCztStaInd: X2 FSM Failure for EVNT - VE_EVNT_HO_PREP_TMR_EXPIRY\n"));
#endif
         } /* end of if statement */

         break;
      }
      case CZT_STATUS_UE_TX2RELOCOVERALL_EXPIRY:
      {
         x2ConCb = veCb.x2apConLst[VE_GET_UE_X2_IDX(cztSta->oldUEX2APId)];
         event = VE_EVNT_HO_OVERALL_TMR_EXPIRY;
         if( RFAILED == (ret = veX2SrcFSM[ueCb->mobCtrlState][event](x2ConCb, NULLP)))
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"VeLiCztStaInd: X2 FSM Failure for EVNT - VE_EVNT_HO_PREP_TMR_EXPIRY\n"));
#endif
         } /* end of if statement */
         break;
      } 
      case CZT_STATUS_ASSOC_DOWN:
      {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"VeLiCztStaInd: SCTP Association failed\
               - CZT_STATUS_ASSOC_DOWN\n"));
#endif
         break;
      }
      default:
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"VeLiCztStaInd: Invalid Status.\n"));
#endif
         RETVALUE(RFAILED);
         break;
      }
   }/* End of switch() */
  
   RETVALUE(ROK);
} /* End of VeLiCztStaInd */



/*
*
*       Fun:   VeLiCztDynamPeerAssocInd
*
*       Desc:  Call handler for Peer Assoc Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_li.c
*       
*/
#ifdef ANSI
PUBLIC S16 VeLiCztDynamPeerAssocInd
(
Pst              *pst,
SuId             suId,
CztPeerAssocInd  *peerAssocInd
)
#else
PUBLIC S16 VeLiCztDynamPeerAssocInd(pst, suId, peerAssocInd)
Pst              *pst;
SuId             suId;
CztPeerAssocInd  *peerAssocInd;
#endif /* ANSI */
{
   CztPeerAssocRsp *peerAssocRsp = NULLP;
   Pst              rspPst;
   SpId             spId;

   TRC2(VeLiCztDynamPeerAssocInd);

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"VeLiCztDynamPeerAssocInd : Received Dynamic Peer Association Indication.\n"));
#endif

   if(peerAssocInd != NULLP)
   {
      /* Allocate memory for Peer Association Response */
      VE_ALLOC(&peerAssocRsp, sizeof(CztPeerAssocRsp));

      if(NULLP == peerAssocRsp)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCztPeerAssocInd: AssocInd Received NULLP\n"));
#endif
         RETVALUE(RFAILED);
      }

      /* Fill the peer Association Response structure */
      peerAssocRsp->peerId = 1; /* currently hardcoded */
      cmMemcpy( (U8 *)&peerAssocRsp->dstAddrLst, (U8 *)&peerAssocInd->dstAddrLst, sizeof(CztNetAddrLst));
      peerAssocRsp->dstPort = peerAssocInd->dstPort;
      peerAssocRsp->globalStreamId = 0;
      peerAssocRsp->locOutStrms = 5;
      /* Dynamic peer is accepted irrespective of the IP address as of now */
      peerAssocRsp->status.status  = CZT_PRIM_OK;

      cmMemset((U8 *)&rspPst, 0, sizeof(Pst) );
      rspPst.event = EVTCZTPEERASSOCRSP;
      rspPst.srcProcId = SFndProcId();
      rspPst.dstProcId = SFndProcId();
      rspPst.srcEnt = ENTVE;
      rspPst.dstEnt = ENTCZ;
      rspPst.selector = VESM_VECZSEL;
      spId = veCb.cztSap[0]->spId;
      /* Build and send the peer Assoc Rsp admitting the peer */
      VeLiCztDynamPeerAssocRsp(&rspPst, spId, peerAssocRsp);
      /* Free the peerAssociation Indication */
      VE_FREE(peerAssocInd,sizeof(CztPeerAssocInd));
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiCztPeerAssocInd: AssocInd Received NULLP\n"));
#endif
   } /* end of else part */
   
   RETVALUE(ROK);
} /* End of VeLiCztPeerAssocInd */


/*
*
*       Fun:   VeLiCztAudCfm
*
*       Desc:  Call handler for Audit confirm
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  ve_li.c
*       
*/
#ifdef ANSI
PUBLIC S16 VeLiCztAudCfm
(
Pst              *pst,
SuId             suId,
CztAudEvnt  *audEvnt
)
#else
PUBLIC S16 VeLiCztAudCfm(pst, suId, audEvnt)
Pst              *pst;
SuId             suId;
CztAudEvnt  *audEvnt;
#endif /* ANSI */
{
   TRC2(VeLiCztAudCfm);
   
   RETVALUE(ROK);
} /* End of VeLiCztAudCfm */


/**************************************************************
   RS  functions (Wrapper function)
***************************************************************/

/*
*
*       Fun:   RsLiCztGpInd
*
*       Desc:  This function is used to receive the Non UE Associated messages from X2.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 RsLiCztGpInd
(
Pst     *pst,
SuId    suId,
CztEvnt *cztInd
)
#else
PUBLIC S16 RsLiCztGpInd(pst,suId,cztInd)
Pst     *pst;  
SuId    suId;
CztEvnt *cztInd;
#endif
{
   TRC2(RsLiCztGpInd);
   RETVALUE(VeLiCztGpInd(pst, suId, cztInd));
} /* end of RsLiCztGpInd */

/*
*
*       Fun:   RsLiCztGpCfm
*
*       Desc:  This function is used to receive comfirmation of
*              the Non UE Associated messages from X2.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 RsLiCztGpCfm
(
Pst     *pst,
SuId    suId,
CztEvnt *cztCfm
)
#else
PUBLIC S16 RsLiCztGpCfm(pst,suId,cztCfm)
Pst     *pst;  
SuId    suId;
CztEvnt *cztCfm;
#endif
{
   TRC2(RsLiCztGpCfm);
   RETVALUE(VeLiCztGpCfm(pst, suId, cztCfm));
} /* end of RsLiCztGpCfm */

/*
*
*       Fun:   RsLiCztBmpInd
*
*       Desc:  This function is used to receive
*              the UE Associated messages from X2.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 RsLiCztBmpInd
(
Pst     *pst,
SuId    suId,
CztEvnt *cztInd
)
#else
PUBLIC S16 RsLiCztBmpInd(pst,suId,cztInd)
Pst     *pst;  
SuId    suId;
CztEvnt *cztInd;
#endif
{
   TRC2(RsLiCztBmpInd);
   RETVALUE(VeLiCztBmpInd(pst, suId, cztInd));
} /* end of RsLiCztBmpInd */

/*
*
*       Fun:   RsLiCztBmpCfm
*
*       Desc:  This function is used to receive the confirmation of 
*              the UE Associated messages from X2.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 RsLiCztBmpCfm
(
Pst     *pst,
SuId    suId,
CztEvnt *cztCfm
)
#else
PUBLIC S16 RsLiCztBmpCfm(pst,suId,cztCfm)
Pst     *pst;  
SuId    suId;
CztEvnt *cztCfm;
#endif
{
   TRC2(RsLiCztBmpCfm);
   RETVALUE(VeLiCztBmpCfm(pst, suId, cztCfm));
} /* end of RsLiCztBmpCfm */


/*
*
*       Fun:   RsLiCztGpErrInd
*
*       Desc:  This function is used to receive error indication from X2.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 RsLiCztGpErrInd
(
Pst     *pst,
SuId    suId,
CztEvnt *cztEvnt
)
#else
PUBLIC S16 RsLiCztGpErrInd(pst,suId,cztEvnt)
Pst     *pst;  
SuId    suId;
CztEvnt *cztEvnt;
#endif
{
   TRC2(RsLiCztGpErrInd);
   RETVALUE(VeLiCztGpErrInd(pst, suId, cztEvnt));
} /* end of RsLiCztGpErrInd */

/*
*
*       Fun:   RsLiCztDynamPeerAssocInd
*
*       Desc:  This function is used to receive dynamic peer 
*              association information from X2.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 RsLiCztDynamPeerAssocInd
(
Pst     *pst,
SuId    suId,
CztPeerAssocInd *peerAssocInd
)
#else
PUBLIC S16 RsLiCztDynamPeerAssocInd(pst,suId,peerAssocInd)
Pst     *pst;  
SuId    suId;
CztPeerAssocInd *peerAssocInd;
#endif
{
   TRC2(RsLiCztDynamPeerAssocInd);
   RETVALUE(VeLiCztDynamPeerAssocInd(pst, suId, peerAssocInd));
} /* end of RsLiCztDynamPeerAssocInd */

/*
 *
 *       Fun:   RsLiCztStatusInd
 *
 *       Desc:  This function resolves the status indication primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  ve_cz_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 RsLiCztStatusInd
(
Pst   *pst,
SpId  spId,
CztStaInd * cztSta
)
#else
PUBLIC S16 RsLiCztStatusInd(pst, spId, cztSta)
Pst   *pst;
SpId  spId;
CztStaInd * cztSta;
#endif
{
   TRC3(RsLiCztStaInd);

   RETVALUE(VeLiCztStaInd(pst, spId, cztSta));

} /* RsLiCztStaInd */

/*
*
*       Fun:   RsLiCztRelCfm
*
*       Desc:  This function is used to receive UE Release confirmation from X2.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 RsLiCztRelCfm
(
Pst     *pst,
SuId    suId,
CztRlsCfm *rlsCfm
)
#else
PUBLIC S16 RsLiCztRelCfm(pst,suId,rlsCfm)
Pst     *pst;  
SuId    suId;
CztRlsCfm *rlsCfm;
#endif
{
   TRC2(RsLiCztRelCfm);
   RETVALUE(VeLiCztRelCfm(pst, suId, rlsCfm));
} /* end of RsLiCztRelCfm */

/*
*
*       Fun:   RsLiCztAudCfm
*
*       Desc:  This function is used to receive Audit confirmation from X2.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 RsLiCztAudCfm
(
Pst     *pst,
SuId    suId,
CztAudEvnt *audEvnt
)
#else
PUBLIC S16 RsLiCztAudCfm(pst,suId,audEvnt)
Pst     *pst;  
SuId    suId;
CztAudEvnt *audEvnt;
#endif
{
   TRC2(RsLiCztAudCfm);
   RETVALUE(VeLiCztAudCfm(pst, suId, audEvnt));
} /* end of RsLiCztAudCfm */



#endif/* End of LTE_HO_SUPPORT */

#endif


/**************************************************************
   NX  functions (Wrapper function)
***************************************************************/
/*
*
*       Fun:   NxLiNhuBndCfm - NHU Bind Cfm
*
*       Desc:  This function is used by to present Bind Cfm to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 NxLiNhuBndCfm
(
Pst     *pst,
SuId    suId,
U8      status
)
#else
PUBLIC S16 NxLiNhuBndCfm(pst,suId,status)
Pst     *pst;  
SuId    suId;
U8      status;
#endif
{
   TRC2(NxLiNhuBndCfm);
   RETVALUE(VeLiNhuBndCfm(pst, suId, status));
} /* end of NxLiNhuBndCfm */

/*
*
*       Fun:   NxLiNhuDatInd - NHU Data Indication
*
*       Desc:  This function is used by to present Data Indication to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 NxLiNhuDatInd
(
Pst     *pst,          
SuId     suId,
NhuDatIndSdus *nhuDatIndSdus           
)
#else
PUBLIC S16 NxLiNhuDatInd (pst,suId,nhuDatIndSdus)
Pst     *pst;          /* post structure */
SuId     suId;
NhuDatIndSdus *nhuDatIndSdus;          /* configuration */
#endif
{
   TRC2(NxLiNhuDatInd);
   RETVALUE(VeLiNhuDatInd (pst, suId, nhuDatIndSdus));
} /* end of NxLiNhuDatInd */

/*
*
*       Fun:   NxLiNhuDatCfm - NHU Data Confirm
*
*       Desc:  This function is used by to present Data Confirmation to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 NxLiNhuDatCfm
(
Pst     *pst,          
SuId    suId,
NhuDatCfmSdus *nhDatCfmSdus           
)
#else
PUBLIC S16 NxLiNhuDatCfm (pst,suId,nhDatCfmSdus)
Pst     *pst;          
SuId     suId;
NhuDatCfmSdus *nhDatCfmSdus;
#endif
{
   TRC2(NxLiNhuDatCfm);
   RETVALUE(VeLiNhuDatCfm (pst, suId, nhDatCfmSdus));
} /* end of NxLiNhuDatCfm */

/*
*
*       Fun:   NxLiNhuStaInd - NHU status Indication
*
*       Desc:  This function is used by to present status indication to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 NxLiNhuStaInd
(
Pst     *pst,          
SuId    suId,
NhuStaIndSdus *nhStaIndSdus           
)
#else
PUBLIC S16 NxLiNhuStaInd (pst, suId, nhStaIndSdus)
Pst     *pst;          
SuId     suId;
NhuStaIndSdus *nhStaIndSdus;
#endif
{
   TRC2(NxLiNhuStaInd);
   RETVALUE(VeLiNhuStaInd (pst, suId, nhStaIndSdus));
} /* end of NxLiNhuStaInd */
/*
*
*       Fun:   NxLiNhuErrInd - Error Indication
*
*       Desc:  This function is used by to present configuration 
*              confirm information to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 NxLiNhuErrInd
(
Pst     *pst,          
SuId     suId,
NhuErrIndSdus *nhErrIndSdus           
)
#else
PUBLIC S16 NxLiNhuErrInd(pst,suId,nhErrIndSdus)
Pst     *pst;          
SuId     suId;
NhuErrIndSdus *nhErrIndSdus;          
#endif
{
   TRC2(NxLiNhuErrInd);
   RETVALUE(VeLiNhuErrInd(pst, suId, nhErrIndSdus));
} /* end of NxLiNhuErrInd */

/*
*
*       Fun:   NxLiNhuCfgCfm - Lower layer config confirm
*
*       Desc:  This function is used by to present configuration confirm
*              to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 NxLiNhuCfgCfm
(
Pst     *pst,
SuId     suId,
NhuCfgCfmSdus *nhuCfgCfmSdus
)
#else
PUBLIC S16 NxLiNhuCfgCfm(pst,suId,nhuCfgCfmSdus)
Pst     *pst;
SuId     suId;
NhuCfgCfmSdus *nhuCfgCfmSdus;
#endif
{
   TRC2(NxLiNhuCfgCfm);
   RETVALUE(VeLiNhuCfgCfm(pst, suId, nhuCfgCfmSdus));
} /* end of NxLiNhuCfgCfm */

/*
*
*       Fun:   NxLiNhuCellCfgCfm - Cell Configuration Confirm
*
*       Desc:  This function is used by to present Cell Config Status Confirm
*              information to NHU.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 NxLiNhuCellCfgCfm
(
Pst     *pst,          
SuId     suId,
NhuCellCfgCfmSdus *nhuCellCfgCfmSdus           
)
#else
PUBLIC S16 NxLiNhuCellCfgCfm(pst,suId,nhuCellCfgCfmSdus)
Pst     *pst;         
SuId     suId;
NhuCellCfgCfmSdus *nhuCellCfgCfmSdus;          
#endif
{
   TRC2(NxLiNhuCellCfgCfm);
   RETVALUE(VeLiNhuCellCfgCfm(pst, suId, nhuCellCfgCfmSdus));
} /* end of NxLiNhuCellCfgCfm */

/*
*
*       Fun:   NxLiNhuDecodeCfm - Decode Confirm
*
*       Desc:  This function is used to inform decode confirmation to RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 NxLiNhuDecodeCfm
(
Pst     *pst,
SuId     suId,
NhuDecodeSduCfm *nhuDecodeSduCfm
)
#else
PUBLIC S16 NxLiNhuDecodeCfm(pst,suId,nhuDecodeSduCfm)
Pst     *pst;
SuId     suId;
NhuDecodeSduCfm *nhuDecodeSduCfm;
#endif
{
   TRC2(NxLiNhuDecodeCfm);
   RETVALUE(VeLiNhuDecodeCfm(pst, suId, nhuDecodeSduCfm));
} /* end of NxLiNhuDecodeCfm */

/*
*
*       Fun:   NxLiNhuCountCfm - Count Confirm
*
*       Desc:  This function is used to inform COUNT values of DRBs to RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 NxLiNhuCountCfm
(
Pst     *pst,
SuId     suId,
NhuDRBCountInfoLst *nhuDRBCountInfoLst
)
#else
PUBLIC S16 NxLiNhuCountCfm(pst,suId,nhuDRBCountInfoLst)
Pst     *pst;
SuId     suId;
NhuDRBCountInfoLst *nhuDRBCountInfoLst;
#endif
{
   TRC2(NxLiNhuCountCfm);
   RETVALUE(VeLiNhuCountCfm(pst, suId, nhuDRBCountInfoLst));
} /* end of NxLiNhuCountCfm */

/*
*
*       Fun:   NxLiNhuEncodeCfm - Encode Confirm
*
*       Desc:  This function is used to inform Encode confirmation to RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 NxLiNhuEncodeCfm
(
Pst     *pst,
SuId     suId,
NhuEncCfmSdus *nhuEncCfmSdus
)
#else
PUBLIC S16 NxLiNhuEncodeCfm(pst,suId,nhuEncCfmSdus)
Pst     *pst;
SuId     suId;
NhuEncCfmSdus *nhuEncCfmSdus;
#endif
{
   TRC2(NxLiNhuEncodeCfm);
   RETVALUE(VeLiNhuEncodeCfm(pst,suId,nhuEncCfmSdus));
} /* end of NxLiNhuEncodeCfm */

/*
*
*       Fun:   NxLiNhuPdcpSduStaCfm - PDCP SDU Status Confirm
*
*       Desc:  This function is used to inform PDCP SDU Status confirmation to 
*              RRC user.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 NxLiNhuPdcpSduStaCfm
(
Pst     *pst,
SuId     suId,
NhuPdcpSduStaCfm *nhuPdcpSduStaCfm
)
#else
PUBLIC S16 NxLiNhuPdcpSduStaCfm(pst,suId,nhuPdcpSduStaCfm)
Pst     *pst;
SuId     suId;
NhuPdcpSduStaCfm *nhuPdcpSduStaCfm;
#endif
{
   TRC2(NxLiNhuPdcpSduStaCfm);
   RETVALUE(VeLiNhuPdcpSduStaCfm(pst,suId,nhuPdcpSduStaCfm));
} /* end of NxLiNhuPdcpSduStaCfm */



/*
*
*       Fun:   NxLiRgrBndCfm - RGR Bind Cfm
*
*       Desc:  This function is used by to present Bind Cfm to RGR.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 NxLiRgrBndCfm
(
Pst     *pst,
SuId    suId,
U8      status
)
#else
PUBLIC S16 NxLiRgrBndCfm(pst,suId,status)
Pst     *pst;  
SuId    suId;
U8      status;
#endif
{

   TRC2(NxLiRgrBndCfm);
   RETVALUE(VeLiRgrBndCfm(pst,suId,status));
} /* end of NxLiRgrBndCfm */


/*
*
*       Fun:   NxLiRgrCfgCfm - Cell Configuration Confirm
*
*       Desc:  This function is used by to present Cell Config Status Confirm
*              information to RGR.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 NxLiRgrCfgCfm
(
Pst            *pst,          
SuId           suId,
RgrCfgTransId  transId,
U8             status
)
#else
PUBLIC S16 NxLiRgrCfgCfm(pst, suId, transId, status)
Pst            *pst;          
SuId           suId;
RgrCfgTransId  transId;
U8             status;
#endif
{
   TRC2(NxLiRgrCfgCfm);
   RETVALUE(VeLiRgrCfgCfm(pst, suId, transId, status));
} /* end of NxLiRgrCfgCfm */

/*
*
*       Fun:   TfLiCtfBndCfm
*
*       Desc:  CTF Bind Cfm
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 TfLiCtfBndCfm
(
Pst       *pst,         
SuId      suId,        
U8        status      
)
#else
PUBLIC S16 TfLiCtfBndCfm(pst, suId, status)
Pst       *pst;      
SuId      suId;     
U8        status;  
#endif
{
   TRC2(TfLiCtfBndCfm);
   RETVALUE(VeLiCtfBndCfm(pst, suId, status));
} /* end of TfLiCtfBndCfm */

/*
*
*       Fun:   TfLiCtfCfgCfm
*
*       Desc:  CTF Cfg Cfm
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 TfLiCtfCfgCfm
(
Pst *pst,             
SuId suId,           
CtfCfgTransId transId,
U8 status            
)
#else
PUBLIC S16 TfLiCtfCfgCfm(pst, suId, transId, status)
Pst *pst;           
SuId suId;         
CtfCfgTransId transId; 
U8 status;            
#endif
{
  
   TRC3(TfLiCtfCfgCfm);
   RETVALUE(VeLiCtfCfgCfm(pst, suId, transId, status));
}   /* end of TfLiCtfCfgCfm */

/*
*
*       Fun:   TfLiCtfUeIdChgCfm
*
*       Desc:  CTF UeId Change Cfm
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 TfLiCtfUeIdChgCfm
(
Pst       *pst,            
SuId       suId,
CtfCfgTransId transId,
CtfUeInfo *ueInfo, 
U8 status
)  
#else
PUBLIC S16 TfLiCtfUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst        *pst;             
SuId        suId;
CtfCfgTransId         transId;
CtfUeInfo  *ueInfo;
U8 status;
#endif
{
   TRC3(TfLiCtfUeIdChgCfm);
   RETVALUE(VeLiCtfUeIdChgCfm(pst, suId, transId, ueInfo, status));
}  /* end of TfLiCtfUeIdChgCfm */

/*************************************************************/
/* EGT Interface */
/*************************************************************/
/*
*
*       Fun:   VeLiEgtBndCfm - EGT Bind Cfm
*
*       Desc:  This function is used by to present Bind Cfm to EGT.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/

#ifdef ANSI
PUBLIC S16 VeLiEgtBndCfm
(
Pst     *pst,
SuId    suId,
U8      status
)
#else
PUBLIC S16 VeLiEgtBndCfm(pst,suId,status)
Pst     *pst;  
SuId    suId;
U8      status;
#endif
{
   TRC2(VeLiEgtBndCfm);
  
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiEgtBndCfm: EGT Bind Cfm\n"));
#endif

   /* Send alarm */
   if (CM_BND_OK == status)
   {
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiEgtBndCfm: Successful Bind Cfm\n"));
#endif

      veStopTmr((PTR)veEuCb.lsapLst[suId],VE_TMR_EGT_SAP_BND);
      veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, LVE_CAUSE_EGT_SAP_BOUND);
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiEgtBndCfm: Unsuccessful bind confirm \n"));
#endif
      veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
   }

   RETVALUE(ROK);
} /* end of VeLiEgtBndCfm */

#ifdef EU_DAT_APP
#ifndef LTE_ENB_PERF
/*
*
*       Fun:   VeLiEgtEguLclTnlMgmtCfm
*
*       Desc:  Call handler for eGTP-U Tunnel Mgmt Confirm.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiEgtEguLclTnlMgmtCfm
(
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
)
#else
PUBLIC S16 VeLiEgtEguLclTnlMgmtCfm(pst, suId, eguMsg)
Pst         *pst;
SuId        suId;
EgtUEvnt    *eguMsg;
#endif /* ANSI */
{
   Pst tmpPst;
   U32 lclTeid = 0;
   VeEgtpTunCb *egtpCb = NULLP;
   VePdcpCb *pdcpCb = NULLP;

   TRC2(EuLiEgtEguLclTnlMgmtCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiEgtEguLclTnlMgmtCfm:\
            EGTP Tunnel Mgmt. Cfm. \n"));
#endif

   lclTeid = eguMsg->u.tnlMgmt.teId;
   if (lclTeid < VE_MAX_TUNNEL_ID)
   {
      egtpCb = veEuCb.egtpTunLst[lclTeid];
      if (egtpCb == NULLP)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiEgtEguLclTnlMgmtCfm: \
                  Could not locate tunnel id\n"));
#endif
         /* Freeing EgtUEvnt structure */
         SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));
         RETVALUE(RFAILED);
      }
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiEgtEguLclTnlMgmtCfm: Invalid tunnel id\n"));
#endif
      /* Freeing EgtUEvnt structure */
      SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));
      RETVALUE(RFAILED);
   }

   pdcpCb = egtpCb->pdcpCb;

   if(eguMsg->u.tnlMgmt.cfmStatus.status == LCM_PRIM_OK)
   {
      switch(eguMsg->u.tnlMgmt.action)
      {
         case EGT_TMGMT_REQTYPE_ADD:
         {
            /* Update State */
            egtpCb->state = VE_EU_CONNECTED;

            cmMemset((U8 *)&tmpPst,0,sizeof(Pst));
            /* Populating pst structure*/
            tmpPst.dstEnt = ENTVE;
            tmpPst.srcEnt = ENTVE;
            tmpPst.dstProcId = SM_VE_PROC;
            tmpPst.srcProcId = SM_VE_PROC;
            /* Post the Cfm status */
            cmPkEuCreattLocTunCfm(&tmpPst, 0, &pdcpCb->pdcpId);
            break;
         }
         case EGT_TMGMT_REQTYPE_DEL:
         {
       cmMemset((U8 *)&tmpPst,0,sizeof(Pst));
            tmpPst.dstEnt = ENTVE;
            tmpPst.srcEnt = ENTVE;
            tmpPst.dstProcId = SM_VE_PROC;
            tmpPst.srcProcId = SM_VE_PROC;
            /* Post the Cfm status */
            cmPkEuDelLocTunCfm(&tmpPst, 0, &pdcpCb->pdcpId);
            break;
         }
         default:
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"VeLiEgtEguLclTnlMgmtCfm: \
                     Tnl Mgmt Unhandled Action :%x\n", eguMsg->u.tnlMgmt.action));
#endif
            break;
         } 
      }
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiEgtEguLclTnlMgmtCfm: \
               Tnl Mgmt Failure Status Action :%x Reason: %x\n",\
               eguMsg->u.tnlMgmt.action, eguMsg->u.tnlMgmt.cfmStatus.reason));
#endif
      SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));
      RETVALUE(RFAILED);
   }
   /* Free the EgtUEvnt structure */
   SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));
   RETVALUE(ROK);
} /* VeLiEgtEguLclTnlMgmtCfm */ 

/*
*
*       Fun:   VeLiEgtEguDatInd
*
*       Desc:  Call handler for eGTP-U Data Indication.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiEgtEguDatInd
(
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
)
#else
PUBLIC S16 VeLiEgtEguDatInd(pst, suId, eguMsg)
Pst         *pst;
SuId        suId;
EgtUEvnt    *eguMsg;
#endif /* ANSI */
{
   TRC2(VeLiEgtEguDatInd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiEgtEguDatInd:\
            Egt Data Ind\n"));
#endif

   /* callng dat ind handler */ 
   VeLiEgtDatInd(pst, suId, eguMsg);

   /* Freeing EgtUEvnt structure */
   cmFreeMem(eguMsg->u.egMsg); 
   SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));

   RETVALUE(ROK);
} /* End of VeLiEgtEguDatInd */

/*
*
*       Fun:   VeLiEgtEguStaCfm
*
*       Desc:  Call handler for eGTP-U Status Confirm.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiEgtEguStaCfm
(
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
)
#else
PUBLIC S16 VeLiEgtEguStaCfm(pst, suId, eguMsg)
Pst         *pst;
SuId        suId;
EgtUEvnt    *eguMsg;
#endif /* ANSI */
{
   TRC2(VeLiEgtEguStaCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiEgtEguStaCfm: \
            EGT Status Confirm\n"));
#endif

   /* Freeing EgtUEvnt structure */
   SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));

   RETVALUE(ROK);
}

/*
*
*       Fun:   VeLiEgtEguUStaInd
*
*       Desc:  Call handler for eGTP-U Unsolicited Status Indication.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiEgtEguUStaInd
(
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
)
#else
PUBLIC S16 VeLiEgtEguUStaInd(pst, suId, eguMsg)
Pst         *pst;
SuId        suId;
EgtUEvnt    *eguMsg;
#endif /* ANSI */
{
   TRC2(VeLiEgtEguUStaInd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiEgtEguUStaInd:\
            EGT Unsolicited Status Indication \n"));
#endif

   /* Freeing EgtUEvnt structure */
   SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));

   RETVALUE(ROK);
}

/*
*
*       Fun:   VeLiEgtEguErrInd
*
*       Desc:  Call handler for eGTP-U Error Indication.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiEgtEguErrInd
(
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
)
#else
PUBLIC S16 VeLiEgtEguErrInd(pst, suId, eguMsg)
Pst         *pst;
SuId        suId;
EgtUEvnt    *eguMsg;
#endif /* ANSI */
{
   TRC2(VeLiEgtEguErrInd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiEgtEguErrInd:\
            EGT Error Ind. \n"));
#endif

   /* Freeing EgtUEvnt structure */
   SPutSBuf(pst->region, pst->pool, (Data* )eguMsg, sizeof(EgtUEvnt));

   RETVALUE(ROK);
}
#endif
#endif 

/*
*
*       Fun:   EuLiEgtBndCfm - EGT Bind Cfm
*
*       Desc:  This function is used by to present Bind Cfm to EGT.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtBndCfm
(
Pst     *pst,
SuId    suId,
U8      status
)
#else
PUBLIC S16 EuLiEgtBndCfm(pst,suId,status)
Pst     *pst;  
SuId    suId;
U8      status;
#endif
{

   TRC2(EuLiEgtBndCfm);
   RETVALUE(VeLiEgtBndCfm(pst,suId,status));
} /* end of EuLiEgtBndCfm */

#ifdef EU_DAT_APP
#ifndef LTE_ENB_PERF
/*
*
*       Fun:   EuLiEgtEguDatInd
*
*       Desc:  eGTP Data Ind.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtEguDatInd
(
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
)
#else
PUBLIC S16 EuLiEgtEguDatInd(pst, suId, eguMsg)
Pst         *pst;
SuId        suId;
EgtUEvnt    *eguMsg;
#endif /* ANSI */
{
   TRC2(EuLiEgtEguDatInd);
   RETVALUE(VeLiEgtEguDatInd(pst, suId, eguMsg));
} /* End of EuLiEgtEguDatInd */


/*
*
*       Fun:   EuLiEgtEguLclTnlMgmtCfm
*
*       Desc:  Call handler for eGTP-U Tunnel Mgmt Confirm.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtEguLclTnlMgmtCfm
(
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
)
#else
PUBLIC S16 EuLiEgtEguLclTnlMgmtCfm(pst, suId, eguMsg)
Pst         *pst;
SuId        suId;
EgtUEvnt    *eguMsg;
#endif /* ANSI */
{
   TRC2(EuLiEgtEguLclTnlMgmtCfm);
   RETVALUE(VeLiEgtEguLclTnlMgmtCfm(pst, suId, eguMsg));
} /* EuLiEgtEguLclTnlMgmtCfm */


/*
*
*       Fun:   VeHdlCreatEgtpTunelCfm
*
*       Desc:  Call handler for eGTP-U Tunnel Mgmt Confirm.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeHdlCreatEgtpTunelCfm
(
Pst         *pst,
SuId        suId,
CmLtePdcpId *pdcpId
)
#else
PUBLIC S16 VeHdlCreatEgtpTunelCfm(pst, suId, pdcpId)
Pst         *pst;
SuId        suId;
CmLtePdcpId *pdcpId;
#endif /* ANSI */
{
   VeCellCb *cellCb = NULLP; 
   VeUeCb *ueCb = NULLP;
   VeS1ConCb *s1apConCb = NULLP;
   U16        ueIdx = 0;
   SztDatEvntReq datEvt;
   U8  idx = 0;

   TRC2(VeHdlCreatEgtpTunelCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeHdlCreatEgtpTunelCfm:\
            EGTP Tunnel Cfm Handler \n"));
#endif


   /* locate approp. structures */
   cellCb = veCb.cellCb[0];

   if(cellCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeHdlCreatEgtpTunelCfm:\
               Locating CellCb failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   ueIdx = VE_GET_UE_IDX(pdcpId->ueId);
   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeHdlCreatEgtpTunelCfm:\
               Invalid ueIdx(%d)\n", ueIdx));
#endif
      RETVALUE(RFAILED);
   }

   if(ueCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeHdlCreatEgtpTunelCfm:\
               Locating ueCb failed.\n"));
#endif
      RETVALUE(RFAILED);
   }


   s1apConCb = ueCb->s1ConCb;

   if(s1apConCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeHdlCreatEgtpTunelCfm:\
               Locating S1ConCb failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Set zero to datreq event struct. */
   VE_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));
   /* populate the event structure */
   datEvt.spConnId = s1apConCb->spConnId;
   /* Decrement number of tunnels created */
   ueCb->numOfTunnelCreReq --;

#ifdef LTE_HO_SUPPORT
   /* check whether it received all tunnel create confirm */
   if(ueCb->numOfTunnelCreReq == 0)
   {
      if(ueCb->mobCtrlState == VE_HO_NORMAL)
      {
         /* assign the pointer to previously built Init. context resp. msg. */
         datEvt.pdu = (S1apPdu *)ueCb->pdu;

         VeLiSztDatReq(&veCb.sztSap[0]->pst,veCb.sztSap[0]->spId , &datEvt);

         /* Freeing  */
         cmFreeMem((Ptr)(ueCb->pdu));
         /* Assigning NULLP */
         ueCb->pdu = NULLP;
      }    
      else if (ueCb->mobCtrlState == VE_HO_IN_PROGRESS && ueCb->isLwrLyrRestNeeded == TRUE)    
      {
         /* Get the PDCP status from the PDCP layer . Enb Status Transfer
          * shall be sent once the PDCP status cfm is received.*/
         veGetPdcpSnSts(ueCb);    
         ueCb->isLwrLyrRestNeeded = FALSE;
      }
   }
#else
   /* check whether it received all tunnel create confirm */
   if(ueCb->numOfTunnelCreReq == 0)
   {
      for (idx = 0; idx < VE_MAX_RABS_IN_UE; idx++)
      {
             if((ueCb->rbInfo.rab[idx].inUse == VE_DRB_CONNECTING))       
         {
          ueCb->rbInfo.rab[idx].inUse = VE_DRB_CONNECTED;       
         }
      }

      /* assign the pointer to previously built Init. context resp. msg. */
      datEvt.pdu = (S1apPdu *)ueCb->pdu;

      VeLiSztDatReq(&veCb.sztSap[0]->pst,veCb.sztSap[0]->spId , &datEvt);

      /* Freeing  */
      cmFreeMem((Ptr)(ueCb->pdu));
      /* Assigning NULLP */
      ueCb->pdu = NULLP;
   }
#endif
   RETVALUE(ROK);
} /* VeHdlCreatEgtpTunelCfm */

/*
*
*       Fun:   VeHdlDelEgtpTunelCfm
*
*       Desc:  Call handler for eGTP-U Tunnel Mgmt Confirm.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeHdlDelEgtpTunelCfm
(
Pst         *pst,
SuId        suId,
CmLtePdcpId *pdcpId
)
#else
PUBLIC S16 VeHdlDelEgtpTunelCfm(pst, suId, pdcpId)
Pst         *pst;
SuId        suId;
CmLtePdcpId *pdcpId;
#endif /* ANSI */
{
   VeCellCb *cellCb = NULLP; 
   VeUeCb *ueCb = NULLP;
   VeS1ConCb *s1apConCb = NULLP;
   U16        ueIdx = 0;
   SztDatEvntReq datEvt;
   U8         idx;

   TRC2(VeHdlDelEgtpTunelCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeHdlDelEgtpTunelCfm:\
            EGTP Tunnel Cfm Handler \n"));
#endif


   /* locate approp. structures */
   cellCb = veCb.cellCb[0];

   if(cellCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeHdlDelEgtpTunelCfm:\
               Locating CellCb failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   ueIdx = VE_GET_UE_IDX(pdcpId->ueId);
   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeHdlDelEgtpTunelCfm:\
               Invalid ueIdx(%d)\n", ueIdx));
#endif
      RETVALUE(RFAILED);
   }

   if(ueCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeHdlDelEgtpTunelCfm:\
               Locating ueCb failed.\n"));
#endif
      RETVALUE(RFAILED);
   }
   ueCb->numOfTunnelDelReq--;

   s1apConCb = ueCb->s1ConCb;

   if(s1apConCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeHdlDelEgtpTunelCfm:\
               Locating S1ConCb failed.\n"));
#endif
      RETVALUE(RFAILED);
   }
   if(ueCb->numOfTunnelDelReq == 0)
   {
    for (idx = 0; idx < VE_MAX_RABS_IN_UE; idx++)
           {
                  if((ueCb->rbInfo.rab[idx].inUse == VE_DRB_DELETING))
                   {
                         ueCb->rbInfo.rab[idx].inUse = VE_DRB_NOT_IN_USE;
                   }
           }


   VE_SET_ZERO(&datEvt, sizeof(SztDatEvntReq));
   /* populate the event structure */
   datEvt.spConnId = s1apConCb->spConnId;

   /* assign the pointer to previously built Init. context resp. msg. */
   datEvt.pdu = (S1apPdu *)ueCb->pdu;

   VeLiSztDatReq(&veCb.sztSap[0]->pst,veCb.sztSap[0]->spId , &datEvt);
   /* Freeing  */
   cmFreeMem((Ptr)(ueCb->pdu));
      /* Assigning NULLP */
   ueCb->pdu = NULLP;
   }

   RETVALUE(ROK);
} /* VeHdlDelEgtpTunelCfm */

/*
*
*       Fun:   EuLiEgtEguStaCfm
*
*       Desc:  Call handler for eGTP-U Status Confirm.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtEguStaCfm
(
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
)
#else
PUBLIC S16 EuLiEgtEguStaCfm(pst, suId, eguMsg)
Pst         *pst;
SuId        suId;
EgtUEvnt    *eguMsg;
#endif /* ANSI */
{
   TRC2(EuLiEgtEguStaCfm);
   RETVALUE(VeLiEgtEguStaCfm(pst, suId, eguMsg));
}

/*
*
*       Fun:   EuLiEgtEguUStaInd
*
*       Desc:  Call handler for eGTP-U Unsolicited Status Indication.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtEguUStaInd
(
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
)
#else
PUBLIC S16 EuLiEgtEguUStaInd(pst, suId, eguMsg)
Pst         *pst;
SuId        suId;
EgtUEvnt    *eguMsg;
#endif /* ANSI */
{
   TRC2(EuLiEgtEguUStaInd);
   RETVALUE(VeLiEgtEguUStaInd(pst, suId, eguMsg));
}

/*
*
*       Fun:   EuLiEgtEguErrInd
*
*       Desc:  Call handler for eGTP-U Error Indication.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtEguErrInd
(
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
)
#else
PUBLIC S16 EuLiEgtEguErrInd(pst, suId, eguMsg)
Pst         *pst;
SuId        suId;
EgtUEvnt    *eguMsg;
#endif /* ANSI */
{
   TRC2(EuLiEgtEguErrInd);
   RETVALUE(VeLiEgtEguErrInd(pst, suId, eguMsg));
}
#endif
/*************************************************************/
/* PJU Interface */
/*************************************************************/
/*
*
*       Fun:   VeLiPjuBndCfm
*
*       Desc:  PJU Bind Cfm
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiPjuBndCfm
(
Pst *pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 VeLiPjuBndCfm(pst,suId,status)
Pst *pst;
SuId suId;
U8 status;
#endif
{
   TRC2(VeLiPjuBndCfm);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiPjuBndCfm: PJU Bind Cfm. \n"));
#endif

   /* Send alarm */
   if (CM_BND_OK == status)
   {
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiPjuBndCfm:Bind Successful \n"));
#endif
      veStopTmr((PTR)veEuCb.lsapLst[suId],VE_TMR_PJU_SAP_BND);
      veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, LVE_CAUSE_PJU_SAP_BOUND);
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiPjuBndCfm:Bind Unsuccessful\n"));
#endif
      veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
   }

   RETVALUE(ROK);
} /* VeLiPjuBndCfm */

/*
*
*       Fun:   VeLiPjuDatCfm
*
*       Desc:  PJU Data Cfm
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiPjuDatCfm
(
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
PjuDatCfmInfo  *datCfm
)
#else
PUBLIC S16 VeLiPjuDatCfm(pst, suId, pdcpId, datCfm)
Pst            *pst;
SuId           suId;
CmLtePdcpId    *pdcpId;
PjuDatCfmInfo  *datCfm;
#endif
{
   TRC2(VeLiPjuDatCfm);
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiPjuDatCfm: PJU Data Cfm\n"));
#endif
   /* Free pdcpId and datCfm */
   SPutSBuf(pst->region, pst->pool, (Data *) pdcpId, sizeof(CmLtePdcpId));
   SPutSBuf(pst->region, pst->pool, (Data *) datCfm, sizeof(PjuDatCfmInfo)); 
   RETVALUE(ROK);
} /* VeLiPjuDatCfm */

/*
*
*       Fun:   VeLiPjuStaInd
*
*       Desc:  PJU Status Ind
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiPjuStaInd
(
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
PjuStaIndInfo  *staInd,
Buffer         *mBuf
)
#else
PUBLIC S16 VeLiPjuStaInd(pst, suId, pdcpId, staInd, mBuf)
Pst            *pst;
SuId           suId;
CmLtePdcpId    *pdcpId;
PjuStaIndInfo  *staInd;
Buffer         *mBuf;
#endif
{
   TRC2(VeLiPjuStaInd);
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiPjuStaInd: PJU Status Ind \n"));
#endif
   /* Free PDCP Id, stdInd and mBuf */
   SPutSBuf(pst->region, pst->pool, (Data *) pdcpId, sizeof(CmLtePdcpId));
   SPutSBuf(pst->region, pst->pool, (Data *) staInd, sizeof(PjuStaIndInfo)); 
   if (mBuf != NULL)
   {
      SPutMsg(mBuf);
      mBuf = NULL;
   }
   
   RETVALUE(ROK);
} /* VeLiPjuStaInd */

/*
*
*       Fun:   PxLiPjuBndCfm
*
*       Desc:  PJU Bind Cfm
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 PxLiPjuBndCfm
(
Pst *pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PxLiPjuBndCfm(pst,suId,status)
Pst *pst;
SuId suId;
U8 status;
#endif
{
   TRC2(PxLiPjuBndCfm);
   RETVALUE(VeLiPjuBndCfm(pst,suId,status));
} /* PxLiPjuBndCfm */

#ifndef  VE_PERF_MEAS
/*
*
*       Fun:   PxLiPjuDatInd
*
*       Desc:  PJU Data Ind
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 PxLiPjuDatInd
(
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
Buffer         *mBuf
)
#else
PUBLIC S16 PxLiPjuDatInd(pst, suId, pdcpId, mBuf)
Pst            *pst;
SuId           suId;
CmLtePdcpId    *pdcpId;
Buffer         *mBuf;
#endif
{
   TRC2(PxLiPjuDatInd);
   RETVALUE(VeLiPjuDatInd(pst, suId, pdcpId, mBuf));
} /* PxLiPjuDatInd */

/*
*
*       Fun:   PxLiPjuDatCfm
*
*       Desc:  PJU Data Cfm
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 PxLiPjuDatCfm
(
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
PjuDatCfmInfo  *datCfm
)
#else
PUBLIC S16 PxLiPjuDatCfm(pst, suId, pdcpId, datCfm)
Pst            *pst;
SuId           suId;
CmLtePdcpId    *pdcpId;
PjuDatCfmInfo  *datCfm;
#endif
{
   TRC2(PxLiPjuDatCfm);
   RETVALUE(VeLiPjuDatCfm(pst, suId, pdcpId, datCfm));
} /* PxLiPjuDatCfm */

/*
*
*       Fun:   PxLiPjuStaInd
*
*       Desc:  PJU Status Ind.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 PxLiPjuStaInd
(
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
PjuStaIndInfo  *staInd,
Buffer         *mBuf
)
#else
PUBLIC S16 PxLiPjuStaInd(pst, suId, pdcpId, staInd, mBuf)
Pst            *pst;
SuId           suId;
CmLtePdcpId    *pdcpId;
PjuStaIndInfo  *staInd;
Buffer         *mBuf;
#endif
{
   TRC2(PxLiPjuStaInd);
   RETVALUE(VeLiPjuStaInd(pst, suId, pdcpId, staInd, mBuf));
} /* PxLiPjuStaInd */

/*
*
*       Fun:   PxLiPjuDatFwdInd
*
*       Desc:  PJU Data Forward Ind.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 PxLiPjuDatFwdInd
(
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
PjuDatFwdIndInfo   *datFwdInd
)
#else
PUBLIC S16 PxLiPjuDatFwdInd(pst, suId, pdcpId, datFwdInd)
Pst            *pst;
SuId           suId;
CmLtePdcpId    *pdcpId;
PjuDatFwdIndInfo   *datFwdInd;
#endif
{
   TRC2(PxLiPjuDatFwdInd);
#ifdef LTE_HO_SUPPORT
   RETVALUE(VeLiPjuDatFwdInd(pst, suId, pdcpId, datFwdInd));
#endif

   RETVALUE(ROK);
} /* PxLiPjuDatFwdInd */

#endif
#endif /* EU_DAT_APP */
/********************************************************************30**

         End of file:     ve_li.c@@/main/2 - Wed Dec  1 08:44:26 2010

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

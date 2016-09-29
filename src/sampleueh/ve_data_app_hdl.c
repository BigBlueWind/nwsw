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

     Desc:     Data App 

     File:     ve_data_app_hdl.c

     Sid:      ve_data_app_hdl.c@@/main/2 - Wed Dec  1 08:44:22 2010

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
#include "cm_pasn.h"       /* Common Pasn includes */
#include "cm_tpt.h"       /* Common Pasn includes */
#include <unistd.h>

#include "nhu.h"           /* NHU interface defines */
#include "lnh.h"           /* Layer Manager interface defines */
#include "pju.h"           /* PDCP data interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "lrg.h"           /* MAC RRM control Interface */

#include "egt.h"
#include "lkw.h"
#include "ve_sm_init.h"    /* Stack Manager interface */
#include "ve_eut.h"

/* RRM related includes */
#include "szt_asn.h"
#include "szt.h"
#include "ctf.h"
#include "ve.h" 
#include "lve.h" 
#include "lkw.h"
#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif


#ifdef LTE_HO_SUPPORT
#include "kw.h"
#endif
/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lib.x"
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_pasn.x"       /* Common Pasn includes */
#include "cm_tpt.x"       /* Common Pasn includes */

#include "nhu.x"           /* NHU Interface includes */
#include "lnh.x"           /* LNH Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "lrg.x"           /* MAC RRM control Interface */
#include "nhu_asn.x"
#ifdef LTE_HO_SUPPORT
#include "czt.x"
#include "czt_asn.x"
#endif
#include "egt.x"
#include "ve_eut.x"

#include <stdlib.h>

/* RRM related includes */
#include "lve.x"
#include "szt.x"
#include "szt_asn.x"
#include "ctf.x"
#include "ve.x" 
/* MSPD changes */
#ifndef MSPD
#include "ve_dflt.h"
#else
#include "ve_dflt_ms.x"
#endif
#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif



#ifdef EU_DAT_APP
#ifndef LTE_ENB_PERF

/*
*
*       Fun:   veDatFillLocEgtpTunnel 
*
*       Desc:  Populate Tunnel Info 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_data_app_hdl.c  
*
*/
#ifdef ANSI
PUBLIC S16 veDatFillLocEgtpTunnel
(
VeEgtpTunCb    *egtpTun,
EgtUEvnt       **eguEvtMsg,
U8 action
)
#else
PUBLIC S16 veDatFillLocEgtpTunnel(egtpTun, eguEvtMsg, action)
VeEgtpTunCb    *egtpTun;
EgtUEvnt       **eguEvtMsg;
U8 action;
#endif /* ANSI */
{
   S16            ret = ROK; 
   Bool           reOrderPres = FALSE;   /* Reordering present in that tunnel */
   U8             intfType = 0;          /* Interface type */
   U16            expSeqNmb = 0;         /* Expected sequence number */
   U16            maxTPduSize = 0;       /* MAX T-PDU size */

   TRC2(veDatFillLocEgtpTunnel);

   if(NULLP == egtpTun)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veDatFillLocEgtpTunnel: SGetSBuf failed.\n"));
#endif
      RETVALUE(RFAILED);
   }
   /*****************************************************************
    * allocate memory for the e-gtpu event message of size EgtUEvnt *
    *****************************************************************/
   if ((ret = SGetSBuf(veEuCb.mem.region, veEuCb.mem.pool, \
               (Data **)eguEvtMsg, sizeof(EgtUEvnt))) != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veDatFillLocEgtpTunnel: SGetSBuf failed.\n"));
#endif
      RETVALUE(ret);
   }

   cmMemset((U8*)*eguEvtMsg, 0, sizeof(EgtUEvnt));

   /* Currently default values are filled */
   reOrderPres = FALSE;
   intfType    = VE_EGU_DFLT_INTFTYPE;
   expSeqNmb   = VE_EGU_DFLT_EXPSEQNMB;
   maxTPduSize = VE_EGU_DFLT_MAXTPDUSZ;

   (*eguEvtMsg)->u.tnlMgmt.action = action;

   (*eguEvtMsg)->u.tnlMgmt.intfType = intfType;
   if(action == EGT_TMGMT_REQTYPE_ADD)
   {
      (*eguEvtMsg)->u.tnlMgmt.reOrderPres = reOrderPres;
      (*eguEvtMsg)->u.tnlMgmt.expSeqNmb = expSeqNmb;
      (*eguEvtMsg)->u.tnlMgmt.maxTPduSize = maxTPduSize;
   }

   (*eguEvtMsg)->u.tnlMgmt.teId = egtpTun->locTeId;
   (*eguEvtMsg)->u.tnlMgmt.reordAlgo = EG_REORDER_TYPE_1;
   veCpyCmTptAddr(&((*eguEvtMsg)->u.tnlMgmt.srcAddr),&veEuCb.srcAddr);
   veCpyCmTptAddr(&((*eguEvtMsg)->u.tnlMgmt.dstIpAddr),&(egtpTun->dstIpAddr));
   veCpyCmTptAddr(&((*eguEvtMsg)->u.tnlMgmt.frmDstIpAddr),&(egtpTun->frmDstIpAddr));

   RETVALUE(ROK);
} /* veDatFillLocEgtpTunnel */
/*
*
*       Fun:   VeDatCreatEgtpTunel 
*
*       Desc:  Trigger Tunnel Creation 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_data_app_hdl.c 
*
*/
#ifdef ANSI
PUBLIC S16 VeDatCreatEgtpTunel
(
Pst            *pst,
SuId           suId,
EgtpTunInf     *tunelInf
)
#else
PUBLIC S16 VeDatCreatEgtpTunel(pst, suId, tunelInf)
Pst            *pst;
SuId           suId;
EgtpTunInf     *tunelInf;
#endif /* ANSI */
{
   EgtUEvnt       *eguEvtMsg = NULLP;
   VeEgtpTunCb    *egtpCb = NULLP;
   VePdcpCb       *pdcpCb = NULLP;
   VeEuUeCb       *euUeCb = NULLP;
   U16              idx = 0;
   VeUeCb          *ueCb = NULLP; /* Added ueCb */

   TRC2(VeDatCreatEgtpTunel);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeDatCreatEgtpTunel:\
            Triggering ETGP Tunnel Creation \n"));
#endif

   /* Creation of PDCP control block and EGTP control block*
    * Keeping the Mapping from PDCP to EGTP cb and vice versa*/

   VE_ALLOC(&egtpCb, sizeof(VeEgtpTunCb));
   /* klock warning fixed */
   if(egtpCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeDatCreatEgtpTunel:\
               Failure in the allocation of egtp ontrol block\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */


   if ((tunelInf->locTeid < VE_MAX_TUNNEL_ID) &&
         (veEuCb.egtpTunLst[tunelInf->locTeid] == NULLP))
   {
      veEuCb.egtpTunLst[tunelInf->locTeid] = egtpCb;
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeDatCreatEgtpTunel: Invalid Loc Tunnel Id\n"));
#endif
      VE_FREE(egtpCb, sizeof (VeEgtpTunCb));
      RETVALUE(RFAILED);
   }

   VE_ALLOC(&pdcpCb, sizeof(VePdcpCb));
   /* klock warning fixed */
   if(pdcpCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeDatCreatEgtpTunel:\
               Failure in the allocation of pdcp control block\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */


   idx = (U8)VE_GET_UE_IDX(tunelInf->pdcpId.ueId);/* klock warning fix */
   idx = (idx * VE_MAX_RABS_IN_UE) + tunelInf->pdcpId.rbId; 
   if ((idx < veEuCb.pdcpIdLstMaxSize) &&
         (veEuCb.pdcpIdLst[idx] == NULLP))
   {
      veEuCb.pdcpIdLst[idx] = pdcpCb;
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeDatCreatEgtpTunel: Invalid Pdcp Idx \n"));
#endif
      VE_FREE(egtpCb, sizeof (VeEgtpTunCb));
      VE_FREE(pdcpCb, sizeof(VePdcpCb));
      veEuCb.pdcpIdLst[idx] = NULLP; 
      RETVALUE(RFAILED);
   }

   idx = (U8)VE_GET_UE_IDX(tunelInf->pdcpId.ueId);/* klock warning fix */
   if ( veEuCb.ueCbLst[idx] == NULLP)
   {
      VE_ALLOC(&euUeCb, sizeof(VeEuUeCb));
      /* klock warning fixed */
      if(euUeCb == NULLP)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"VeDatCreatEgtpTunel:\
                  Failure in the allocation of Ue control block\n"));
#endif
         RETVALUE(RFAILED);
      } /* end of if statement */

      veEuCb.ueCbLst[idx] = euUeCb;
   /* fill the VeEuUeCb */
   /* Start the Timer */
      euUeCb->ueId = tunelInf->pdcpId.ueId;
      euUeCb->datRcvd = FALSE;
      euUeCb->expiryCnt = 0;
      cmInitTimers(&(euUeCb->inactivityTmr), 1);
      /* Fix for CID- 1657-01-01 DefectId- 115433 */
      /* type casting added to remove warning*/
      veInactvStartTmr((PTR)euUeCb, VE_TMR_INACTIVITY, smCfgCb.inactvTmrVal);
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"VeDatCreatEgtpTunel: Ue Already present  \n"));
#endif
   }
   /* Get ueCb */
   if(veCb.cellCb[0] == NULLP || veCb.cellCb[0]->ueCbLst[idx] == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeDatCreatEgtpTunel:\
               Locating CellCb/ueCb failed.\n"));
#endif
      RETVALUE(RFAILED);
   }
   ueCb = veCb.cellCb[0]->ueCbLst[idx];
   /* Increment number of tunnels created */
   ueCb->numOfTunnelCreReq ++;

   
   cmMemcpy((U8 *)&pdcpCb->pdcpId, (U8 *)&tunelInf->pdcpId, sizeof(CmLtePdcpId));
   /* Tunnel Info */
   pdcpCb->locTeId = tunelInf->locTeid;
   pdcpCb->egtpTunCb = egtpCb;
   pdcpCb->liSap = veEuCb.lsapLst[VE_PJ_SUID];

   egtpCb->pdcpCb = pdcpCb;
   egtpCb->locTeId = tunelInf->locTeid;
   egtpCb->remTeid = tunelInf->remTeid;

   veCpyCmTptAddr(&(egtpCb->dstIpAddr),&(tunelInf->dstIpAddr));
   veCpyCmTptAddr(&(egtpCb->frmDstIpAddr),&(egtpCb->frmDstIpAddr));

   cmMemcpy((U8 *)&egtpCb->pdcpId, (U8 *)&pdcpCb->pdcpId, sizeof(CmLtePdcpId));
   egtpCb->state = VE_EU_INITIAL;
   egtpCb->liSap = veEuCb.lsapLst[suId];

#ifdef LTE_HO_SUPPORT
   cmLListInit(&egtpCb->dataBuffLst);
   egtpCb->datFwdIndFlag = VE_EU_NO_FWD;
#endif

   veDatFillLocEgtpTunnel(egtpCb, &eguEvtMsg, EGT_TMGMT_REQTYPE_ADD);

   eguEvtMsg->event = EVTEGTULCLTNLMGMTREQ;
   /* Trigger primitive */
   VeLiEgtEguLclTnlMgmtReq(&egtpCb->liSap->pst, egtpCb->liSap->spId, eguEvtMsg);

   RETVALUE(ROK);
} /* End of VeDatCreatEgtpTunel */
#ifdef LTE_HO_SUPPORT
/*
*
*       Fun:   veFreeBufLst
*
*       Desc:  Delete EGTP Tunnel
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_data_app_hdl.c 
*
*/
#ifdef ANSI
PUBLIC S16 veFreeBufLst
(
VeEgtpTunCb *egtpTunCb
)
#else
PUBLIC S16 veFreeBufLst(egtpTunCb)
VeEgtpTunCb *egtpTunCb;
#endif /* ANSI */
{
   CmLList *bufNode = NULLP;
   EuEgtpBuffInfo *egtpBufInfo = NULLP;

   TRC2(veFreeBufLst);

   /* Start Forwarding of buffered packets */
   bufNode = egtpTunCb->dataBuffLst.first;

   while(NULLP != bufNode)
   {
      egtpBufInfo = (EuEgtpBuffInfo *)bufNode->node;

      SPutSBuf(veEuCb.mem.region, veEuCb.mem.pool,
           (Data *)&egtpBufInfo, sizeof(EuEgtpBuffInfo));

      bufNode = bufNode->next;
   }
 
   RETVALUE(ROK);

} /* end of veFreeBufLst */
#endif
/*
*
*       Fun:   VeDatDelEgtpTunel 
*
*       Desc:  Delete EGTP Tunnel
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_data_app_hdl.c 
*
*/
#ifdef ANSI
PUBLIC S16 VeDatDelEgtpTunel
(
Pst     *pst,
SuId    suId,
U32     locTeid
)
#else
PUBLIC S16 VeDatDelEgtpTunel(pst, suId, locTeid)
Pst     *pst;
SuId    suId;
U32     locTeid;
#endif /* ANSI */
{
   VeEgtpTunCb    *egtpCb = NULLP;
   VePdcpCb       *pdcpCb = NULLP;
   EgtUEvnt       *eguEvtMsg = NULLP;
   U16 idx = 0;

   TRC2(VeDatDelEgtpTunel);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeDatDelEgtpTunel:\
            Triggering EGTP Tunnel Deletion \n"));
#endif

   if (locTeid < VE_MAX_TUNNEL_ID)
   {
      egtpCb = veEuCb.egtpTunLst[locTeid];
      if (egtpCb != NULLP)
      {
         veDatFillLocEgtpTunnel(egtpCb, &eguEvtMsg, EGT_TMGMT_REQTYPE_DEL);

         eguEvtMsg->event = EVTEGTULCLTNLMGMTREQ;


         VeLiEgtEguLclTnlMgmtReq(&egtpCb->liSap->pst,egtpCb->liSap->spId,eguEvtMsg);

         idx = (U8)VE_GET_UE_IDX(egtpCb->pdcpId.ueId);/* klock warning fix */
         idx = (idx * VE_MAX_RABS_IN_UE) + egtpCb->pdcpId.rbId; 
         if (idx  < veEuCb.pdcpIdLstMaxSize)
         {
            pdcpCb = veEuCb.pdcpIdLst[idx];
            if (pdcpCb != NULLP)
            {
#ifdef LTE_HO_SUPPORT
               if(egtpCb == pdcpCb->egtpTunCb)
               {
                  if(NULLP != pdcpCb->datFwdDlEgtpTunCb)
                  {
                     if(ROK != veDatFillLocEgtpTunnel(pdcpCb->datFwdDlEgtpTunCb, &eguEvtMsg, EGT_TMGMT_REQTYPE_DEL))
                     {
#ifdef DEBUGP
                        VE_DBG_ERROR((VE_PRNT_BUF,"VeDatDelEgtpTunel:Failed to veDatFillLocEgtpTunnel.\n"));
#endif
                        RETVALUE(RFAILED);
                     }

                     eguEvtMsg->event = EVTEGTULCLTNLMGMTREQ;

                     VeLiEgtEguLclTnlMgmtReq(&egtpCb->liSap->pst,egtpCb->liSap->spId,eguEvtMsg);

                     veEuCb.egtpTunLst[pdcpCb->datFwdDlEgtpTunCb->locTeId] = NULLP;
                     VE_FREE(pdcpCb->datFwdDlEgtpTunCb,sizeof(VeEgtpTunCb));
                  }
                  if(NULLP != pdcpCb->datFwdUlEgtpTunCb)
                  {
                     if(ROK != veDatFillLocEgtpTunnel(pdcpCb->datFwdUlEgtpTunCb, &eguEvtMsg, EGT_TMGMT_REQTYPE_DEL))
                     {
#ifdef DEBUGP
                        VE_DBG_ERROR((VE_PRNT_BUF,"VeDatDelEgtpTunel:Failed to veDatFillLocEgtpTunnel.\n"));
#endif
                        RETVALUE(RFAILED);
                     }

                     eguEvtMsg->event = EVTEGTULCLTNLMGMTREQ;

                     VeLiEgtEguLclTnlMgmtReq(&egtpCb->liSap->pst,egtpCb->liSap->spId,eguEvtMsg);

                     veEuCb.egtpTunLst[pdcpCb->datFwdUlEgtpTunCb->locTeId] = NULLP;
                     
                     VE_FREE(pdcpCb->datFwdUlEgtpTunCb,sizeof(VeEgtpTunCb));
                  }
                  VE_FREE(pdcpCb,sizeof(VePdcpCb));
                  veEuCb.pdcpIdLst[idx] = NULLP;
               }
               else
               {
                  veFreeBufLst(egtpCb);
                  VE_FREE(egtpCb,sizeof(VeEgtpTunCb));
                  veEuCb.egtpTunLst[locTeid] = NULLP;
               }
#else
               VE_FREE(pdcpCb,sizeof(VePdcpCb));
               veEuCb.pdcpIdLst[idx] = NULLP;
#endif
            }
         }
      }
      else
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"VeDatDelEgtpTunel: Invalid Tunnel\n"));
#endif
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
} /* End of VeDatDelEgtpTunel */

/*
*
*       Fun:   veFillEgtpDatMsg 
*
*       Desc:  Populate Dat Req Msg
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_data_app_hdl.c  
*
*/
#ifdef ANSI
PUBLIC S16 veFillEgtpDatMsg
(
VeEgtpTunCb    *egtpCb,
EgtUEvnt       **eguEvtMsg,
U8             msgType
)
#else
PUBLIC S16 veFillEgtpDatMsg(egtpCb, eguEvtMsg, msgType)
VeEgtpTunCb    *egtpCb;
EgtUEvnt       **eguEvtMsg;
U8             msgType;
#endif /* ANSI */
{
   EgUMsgHdr *eguHdr = NULLP;
   S16       ret = ROK;

   TRC2(veFillEgtpDatMsg);

   if(NULLP == egtpCb)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillEgtpDatMsg: egtpCb is NULL.\n"));
#endif
      RETVALUE(RFAILED);
   }

   if ((ret = SGetSBuf(veEuCb.mem.region, veEuCb.mem.pool,\
               (Data **)eguEvtMsg, sizeof(EgtUEvnt))) != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillEgtpDatMsg: SGetSBuf  failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   cmMemset((U8*)*eguEvtMsg, 0, sizeof(EgtUEvnt));

   if (cmAllocEvnt(sizeof(EgUMsg), VE_EG_UTIL_MEM_SIZE,\
            &veEuCb.mem, (Ptr* )&((*eguEvtMsg)->u.egMsg)) != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillEgtpDatMsg: cmAllocEvnt failed.\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* Src & Dst Address */
   veCpyCmTptAddr(&((*eguEvtMsg)->u.egMsg->srcAddr),&veEuCb.srcAddr);
   veCpyCmTptAddr(&((*eguEvtMsg)->u.egMsg->remAddr),&(egtpCb->dstIpAddr));

   eguHdr = &(*eguEvtMsg)->u.egMsg->msgHdr;
   /* Populating Header Info */
   eguHdr->extHdr.udpPort.pres = TRUE;
   eguHdr->extHdr.pdcpNmb.pres = TRUE;

   eguHdr->msgType = msgType;

   eguHdr->nPduNmb.pres = FALSE;
   eguHdr->seqNumber.pres = FALSE;
   eguHdr->extHdr.udpPort.pres = FALSE;
   eguHdr->extHdr.pdcpNmb.pres = FALSE;

   eguHdr->teId = egtpCb->remTeid;

   RETVALUE(ROK);
} /* veFillEgtpDatMsg */ 

#ifdef LTE_HO_SUPPORT
/*
*
*       Fun:   veFillPjuDatFwdReq
*
*       Desc:  Populate PDCP Dat Fwd Req
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_data_app_hdl.c  
*
*/
#ifdef ANSI
PUBLIC S16 veFillPjuDatFwdReq
(
PjuDatFwdReqInfo **pjuDatFwdReq,
EgtUEvnt       *eguMsg,
U8               dir
)
#else
PUBLIC S16 veFillPjuDatFwdReq(pjuDatFwdReq, eguMsg, dir)
PjuDatFwdReqInfo **pjuDatFwdReq;
EgtUEvnt         *eguMsg;
U8               dir;
#endif /* ANSI */
{
   U8   cnt = 1;
   U16   idx = 0;
   EgUMsg *eguMsgTmp = NULLP;
   PjuDatFwdReqInfo *datFwdReq = NULLP;

   TRC2(veFillPjuDatFwdReq);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillPjuDatFwdReq:\
                Filling PJU Data Forward Request\n"));
#endif

   VE_ALLOC(&datFwdReq, sizeof(PjuDatFwdReqInfo)); 
   if (NULLP == datFwdReq)
   {
      /* Log error */
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillPjuDatFwdReq: Alloc Failed for PjuDatFwdReqInfo\n"));
#endif
      RETVALUE(RFAILED);
   }

   datFwdReq->dir = dir;

   if (NULLP == eguMsg)
   {
      /* Log error */
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillPjuDatFwdReq: Alloc Failed for PjuDatFwdReqInfo\n"));
#endif
      RETVALUE(RFAILED);
   }

   while (eguMsg->u.egMsg->next)
      cnt++;

   datFwdReq->numSdus = cnt;

   VE_ALLOC(&datFwdReq->datFwdInfo, cnt * sizeof(PjuDatFwdInfo));
   if (NULLP == datFwdReq->datFwdInfo)
   {
      /* Log error */
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillPjuDatFwdReq: Alloc Failed for pjuDatFwdInfo\n"));
#endif
      RETVALUE(RFAILED);
   }

   eguMsgTmp = eguMsg->u.egMsg;

   if(NULLP == eguMsgTmp)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillPjuDatFwdReq: eguMsgTmp is NULL.\n"));
#endif
      RETVALUE(RFAILED);
   }
   for(idx = 0; (idx < cnt) && (eguMsgTmp != NULLP); idx++, eguMsgTmp = eguMsgTmp->next)
   {
      datFwdReq->datFwdInfo[idx].sdu = eguMsgTmp->u.mBuf;
      datFwdReq->datFwdInfo[idx].sn = eguMsgTmp->msgHdr.seqNumber.val;
      datFwdReq->datFwdInfo[idx].sduId = eguMsgTmp->msgHdr.nPduNmb.val;
   }

   *pjuDatFwdReq = datFwdReq;

   RETVALUE(ROK);
} /* end of veFillPjuDatFwdReq */
#endif

/*
*
*       Fun:   VeLiPjuDatInd 
*
*       Desc:  PJU Data Indication
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_data_app_hdl.c 
*
*/
#ifdef ANSI
PUBLIC S16 VeLiPjuDatInd
(
Pst           *pst,
SuId           suId,
CmLtePdcpId   *pdcpId,
Buffer *mBuf
)
#else
PUBLIC S16 VeLiPjuDatInd(pst, suId, pdcpId, mBuf)
Pst           *pst;
SuId           suId;
CmLtePdcpId   *pdcpId;
Buffer *mBuf;
#endif /* ANSI */
{
   VePdcpCb    *pdcpCb = NULLP;
   VeCellCb    *cellCb  = NULLP;
   EgtUEvnt    *eguEvtMsg = NULLP;
   EgUMsg      *egMsg = NULLP;
   U16          idx = 0;
   VeEuUeCb    *euUeCb = NULLP;
   VeEgtpTunCb *egtpCb = NULLP;

   TRC2(VeLiPjuDatInd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiPjuDatInd:\
            Data Indication recieved at PJU Interface \n"));
#endif

   cellCb = veCb.cellCb[0];
   idx = VE_GET_UE_IDX(pdcpId->ueId);

   /* Get VeEuUeCb from VeEuCb using pdcpId->ueID */
   euUeCb = veEuCb.ueCbLst[idx];
   if(euUeCb != NULLP)
   {
      euUeCb->datRcvd = TRUE;
      euUeCb->expiryCnt = 0;
   }
   else
   {
      printf("\n SHOULD NOT COME HERE \n"); 
   }

   idx = (idx * VE_MAX_RABS_IN_UE) + pdcpId->rbId; 

   /* Freeing */
   SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
   /* Drop the data if pdcpCb is NULL or pdcp Id is invalid */
   if (idx >= veEuCb.pdcpIdLstMaxSize || (!(pdcpCb = veEuCb.pdcpIdLst[idx])))
   {
      SPutMsg(mBuf);
      RETVALUE(ROK);
   }
   egtpCb = pdcpCb->egtpTunCb;

   /* Fill EGTP Data Message */
   veFillEgtpDatMsg(egtpCb,&eguEvtMsg, EGT_GTPU_MSG_GPDU);

   egMsg = eguEvtMsg->u.egMsg;
   egMsg->u.mBuf = mBuf;
   /* Trigger EGTP Data Req */
   VeLiEgtEguDatReq(&(egtpCb->liSap->pst), egtpCb->liSap->spId, eguEvtMsg);
   
   RETVALUE(ROK);
} /* VeLiPjuDatInd */

/*
*
*       Fun:   VeLiEgtDatInd 
*
*       Desc:  EGT Data Indication
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_data_app_hdl.c  
*
*/
#ifdef ANSI
PUBLIC S16 VeLiEgtDatInd
(
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
)
#else
PUBLIC S16 VeLiEgtDatInd(pst, suId, eguMsg)
Pst         *pst;
SuId        suId;
EgtUEvnt    *eguMsg;
#endif /* ANSI */
{
   VeEgtpTunCb  *egtpCb = NULLP;
   VePdcpCb     *pdcpCb = NULLP;
   CmLtePdcpId  *pdcpId = NULLP;
   S16          ret = ROK;

   VeEuUeCb  *euUeCb;
   U16        idx = 0;
#ifdef LTE_HO_SUPPORT
   EuEgtpBuffInfo *egtpBufInfo = NULLP;
   PjuDatFwdReqInfo *datFwdReq = NULLP;
   U8 dir;
   EgtUEvnt  *eguEvtMsg = NULLP;
   Buffer  *mBufTmp = NULLP;
   VeEuChngTnlStInfo stInfo;
   Bool pktDiscard = FALSE;

   U8 delTunInd = VE_EU_DEL_TUNNELS_NONE; /* to indicate if fwding tunnels 
                                             needs to be deleted. 
                                             Eg., after reception of 
                                             END MARKER pkt */
   Bool fwdPktFlag = FALSE; /* to indicate if the received fwd packet is a 
                               fresh IP packet or a un-acked pkt */
   U8 msgType = EGT_GTPU_MSG_GPDU;
#endif

   U32 locTeid;


   TRC2(VeLiEgtDatInd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiEgtDatInd:\
            EGT Data Indication Primitive \n"));
#endif

   locTeid = eguMsg->u.egMsg->msgHdr.teId;

   if(eguMsg->u.egMsg->msgHdr.msgType == EGT_GTPU_MSG_SUPP_EXT_HDR_NTF)
   {
      printf("\nrecvd HDR NOTIFY\n");
      RETVALUE(ROK);
   }
   else if(eguMsg->u.egMsg->msgHdr.msgType == EGT_GTPU_MSG_ERROR_IND)
   {
      printf("\nrecvd ERROR IND\n");
      RETVALUE(ROK);
   }

   if (locTeid < VE_MAX_TUNNEL_ID)
   {
      egtpCb = veEuCb.egtpTunLst[locTeid];
      if (egtpCb != NULLP)
      {
         pdcpCb = egtpCb->pdcpCb;
      }
      else
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiEgtDatInd: Invalid Tunnel Id\n"));
#endif
         SPutMsg(eguMsg->u.egMsg->u.mBuf);
         RETVALUE(RFAILED);
      }
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiEgtDatInd: Invalid Tunnel Id\n"));
#endif
      SPutMsg(eguMsg->u.egMsg->u.mBuf);

      RETVALUE(RFAILED);
   }

#ifdef LTE_HO_SUPPORT
   /* check the state of the normal tunnel and not the forward tunnels */
   /* because, at target, the fwd pkts are recived via forward tunnels
      and then tunnel state is VE_EU_INITIAL */
   if(pdcpCb->egtpTunCb->state == VE_EU_START_DATBUF)
   {
      /* check if pkt storage limit is execeeded */
      if(VE_EU_PKT_LIMIT == (egtpCb->dataBuffLst.count + 1))
      {
         pktDiscard = TRUE;
      }
      /* if sequence number field is not present, then buffer the packet, else send to PDCP */
      if(eguMsg->u.egMsg->msgHdr.msgType == EGT_GTPU_MSG_GPDU)
      {
         if(locTeid != pdcpCb->egtpTunCb->locTeId)
         {
            if(((NULLP != pdcpCb->datFwdUlEgtpTunCb) && \
                     (locTeid == pdcpCb->datFwdUlEgtpTunCb->locTeId)) ||
                  ((NULLP != pdcpCb->datFwdDlEgtpTunCb) && \
                   (locTeid == pdcpCb->datFwdDlEgtpTunCb->locTeId)))
            {
               if(TRUE == eguMsg->u.egMsg->msgHdr.seqNumber.pres)
               {
                  fwdPktFlag = TRUE;
               }
            }
         }
         msgType = EGT_GTPU_MSG_GPDU;
         /* store the pkt in tht buff list */
         if(pktDiscard == FALSE && fwdPktFlag == FALSE)
         {
            /* add to dataBuff */
            ret = SGetSBuf(veEuCb.mem.region, veEuCb.mem.pool,
                  (Data **)&egtpBufInfo, sizeof(EuEgtpBuffInfo)); 
            if (ret != ROK)
            {
               /* Log error */
               VE_DBG_ERROR((VE_PRNT_BUF,"VeLiEgtDatInd: SGetSBuf Failed\n"));
               SPutMsg(eguMsg->u.egMsg->u.mBuf);
               RETVALUE(ret);
            }
            egtpBufInfo->data = eguMsg->u.egMsg->u.mBuf;
            cmLListAdd2Tail(&(egtpCb->dataBuffLst), &(egtpBufInfo->lnk));
            egtpBufInfo->lnk.node = (PTR)egtpBufInfo;
         }
         else if(fwdPktFlag == TRUE)
         {
            /* find the direction of the bearer */
            if(NULLP != pdcpCb->datFwdDlEgtpTunCb)
               dir = PJ_DIR_DL;
            else
               dir = PJ_DIR_UL;

            /* Fill PDCP Data Fwd Message */
            if(ROK != veFillPjuDatFwdReq(&datFwdReq, eguMsg, dir))
            {
               /* Log error */
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"VeLiEgtDatInd: Filling of datFwdReq Failed\n"));
#endif
               SPutMsg(eguMsg->u.egMsg->u.mBuf);
               RETVALUE(RFAILED);
            }
            VeLiPjuDatFwdReq(&pdcpCb->liSap->pst, pdcpCb->liSap->spId, pdcpId, datFwdReq);
         }
      }
      else if(eguMsg->u.egMsg->msgHdr.msgType == EGT_GTPU_MSG_END_MARKER)
      {
         if(egtpCb == pdcpCb->egtpTunCb)
         {
            msgType = EGT_GTPU_MSG_END_MARKER;
            delTunInd = VE_EU_DEL_ALL_TUNNELS;
         }
         else if(egtpCb == pdcpCb->datFwdDlEgtpTunCb)
         {
            /* change state of the tunnel */
            stInfo.teId = pdcpCb->egtpTunCb->locTeId;
            stInfo.state = VE_EU_INITIAL;
            stInfo.cause = VE_EU_END_MARKER_RECVD;
            veChngTnlState (pst, suId, &stInfo);
            delTunInd = VE_EU_DEL_FWD_TUNNELS;
         }
      }

      if(pdcpCb->egtpTunCb->datFwdIndFlag == VE_EU_FWD_NEW_PKTS)
      {
         /* Fill EGTP Data Message */
         ret = veFillEgtpDatMsg(pdcpCb->datFwdDlEgtpTunCb, &eguEvtMsg, msgType);

         if(eguMsg->u.egMsg->msgHdr.msgType == EGT_GTPU_MSG_GPDU)
         {
            if(pktDiscard == FALSE)
            {
               /* Create a copy of the data buffer and forward the packet */
               SCpyMsgMsg(egtpBufInfo->data, veEuCb.mem.region, veEuCb.mem.pool, &mBufTmp);
               eguEvtMsg->u.egMsg->u.mBuf = mBufTmp;
            }
            else
            {
               eguEvtMsg->u.egMsg->u.mBuf = eguMsg->u.egMsg->u.mBuf;
            }
         }

         /* Trigger EGTP Data Req */
         ret = VeLiEgtEguDatReq(&(pdcpCb->datFwdDlEgtpTunCb->liSap->pst), \
               pdcpCb->datFwdUlEgtpTunCb->liSap->spId, eguEvtMsg);
      }

      /* addressing the pktDiscard flag */
      if(pktDiscard == TRUE)
      {
         SPutMsg(eguMsg->u.egMsg->u.mBuf);
         SPutSBuf(pst->region, pst->pool, (Data *)eguMsg, sizeof(EgtUEvnt));
      }

      /* addressing the delFwdTun flag */
      if(delTunInd == VE_EU_DEL_ALL_TUNNELS)
      {
         /* delete Normal Tunnel */
         /* VeDatDelEgtpTunel - takes care of deleting the fwding tunnels
            while deleting the normal tunnel */
         VeDatDelEgtpTunel(pst, suId, egtpCb->locTeId);
      }
      else if (delTunInd == VE_EU_DEL_FWD_TUNNELS)
      {
         /* delete DL Forwarding tunnel */
         VeDatDelEgtpTunel(pst, suId, pdcpCb->datFwdDlEgtpTunCb->locTeId);
         /* delete UL Forwarding tunnel */
         if(pdcpCb->datFwdUlEgtpTunCb)
         {
            VeDatDelEgtpTunel(pst, suId, pdcpCb->datFwdUlEgtpTunCb->locTeId);
         }
      }

      RETVALUE(ret);
   }
#endif
   
   ret = SGetSBuf(veEuCb.mem.region, veEuCb.mem.pool,
         (Data **)&pdcpId, sizeof(CmLtePdcpId)); 
   if (ret != ROK)
   {
      /* Log error */
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiEgtDatInd: SGetSBuf Failed\n"));
#endif
      SPutMsg(eguMsg->u.egMsg->u.mBuf);
      RETVALUE(RFAILED);
   }
  /* FOR PAGING FEAUTRE */
   idx = VE_GET_UE_IDX(pdcpCb->pdcpId.ueId);
   euUeCb = veEuCb.ueCbLst[idx];
   if(euUeCb != NULLP)
   {
      euUeCb->datRcvd = TRUE;
      euUeCb->expiryCnt = 0;
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiEgtDatInd: Unable to get euUeCb\n"));
#endif
   } 

   cmMemcpy ((U8 *)pdcpId, (U8 *)&(pdcpCb->pdcpId), sizeof(CmLtePdcpId));
   /* Triggering Data Req towards PJU */
   VeLiPjuDatReq(&pdcpCb->liSap->pst, pdcpCb->liSap->spId, pdcpId, 
         pdcpCb->sduIdCntr++, 
         eguMsg->u.egMsg->u.mBuf);

   RETVALUE(ROK);
} /* End of VeLiEgtDatInd */

/*
*
*       Fun:   VeLiEgtDatInd 
*
*       Desc:  EGT Data Indication
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_data_app_hdl.c  
*
*/
#ifdef ANSI
PUBLIC S16 veInactivityDetected
(
VeEuUeCb    *euUeCb
)
#else
PUBLIC S16 veInactivityDetected(euUeCb)
VeEuUeCb    *euUeCb;
#endif /* ANSI */
{
   Pst pst;
   U8  idx;   /* Added idx */

   TRC3(veInactivityDetected)

   VE_SET_ZERO(&pst, sizeof(Pst));

   veInactvStopTmr((PTR)euUeCb, VE_TMR_INACTIVITY);

   /*Currently hard coding the pst structure*/
   pst.dstEnt = ENTVE;
   pst.srcEnt = ENTVE;
   pst.dstProcId = SM_VE_PROC;
   pst.srcProcId = SM_VE_PROC;

   /* The UeId is packed and event is triggered towards
    * the Relay system thread to send the S1 Release 
    * Request */
   cmPkEuRelReq(&pst, 0, euUeCb->ueId);
   idx = (U8)VE_GET_UE_IDX(euUeCb->ueId);
   /* Get ue index */
   idx = (U8)VE_GET_UE_IDX(euUeCb->ueId);/* klock warning fix */
   VE_FREE(euUeCb , sizeof(VeEuUeCb));
   /* Set NULLP in ueCbLst */
   veEuCb.ueCbLst[idx] = NULLP;
   RETVALUE(ROK);

} /* end of veInactivityDetected */

#ifdef LTE_HO_SUPPORT
/*
*
*       Fun:   veChngTnlState 
*
*       Desc:  To change the state of the tunnel and take appropriate steps.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_data_app_hdl.c  
*
*/
#ifdef ANSI
PUBLIC S16 veChngTnlState
(
Pst *pst,
SuId suId,
VeEuChngTnlStInfo *stInfo
)
#else
PUBLIC S16 veChngTnlState(pst, suId, stInfo)
Pst *pst;
SuId suId;
VeEuChngTnlStInfo *stInfo;
#endif /* ANSI */
{
   VeEgtpTunCb  *egtpCb = NULLP;
   VePdcpCb       *pdcpCb = NULLP;
   S16            ret = ROK;
   EuEgtpBuffInfo *egtpBufInfo = NULLP;
   CmLList *bufNode = NULLP;
   CmLtePdcpId *pdcpId = NULLP;

   TRC3(veChngTnlState)

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veChngTnlState:\
                Changing the state of the tunnel \n"));
#endif

   if(NULLP == stInfo)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veChngTnlState: Invalid Argument\n"));
      RETVALUE(RFAILED);
   }
   if (stInfo->teId < VE_MAX_TUNNEL_ID)
   {
      egtpCb = veEuCb.egtpTunLst[stInfo->teId];
      if (egtpCb != NULLP)
      {
         pdcpCb = egtpCb->pdcpCb;
      }
      else
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veChngTnlState: Invalid Tunnel Id\n"));
#endif
         RETVALUE(RFAILED);
      }
   }
   else
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veChngTnlState: Invalid Tunnel Id\n"));
#endif
      RETVALUE(RFAILED);
   }

   switch(stInfo->state)
   {
      case VE_EU_INITIAL:
         /* tx packets from fwd tunels */
         /* state is not changed when there are fwd tunnels. 
            this is to wait and handle the reception of END MARKER packets */
         /* send packets from dl forward tunnel cb */
         if (pdcpCb->datFwdDlEgtpTunCb)
         {
            CM_LLIST_FIRST_NODE(&pdcpCb->datFwdDlEgtpTunCb->dataBuffLst, bufNode);

            while(NULLP != bufNode && pdcpCb->datFwdDlEgtpTunCb->dataBuffLst.count != 0)
            {
               egtpBufInfo = (EuEgtpBuffInfo *)bufNode->node;

               /* remove the node from the candLst */
               cmLListDelFrm(&pdcpCb->datFwdDlEgtpTunCb->dataBuffLst, bufNode);

               ret = SGetSBuf(veEuCb.mem.region, veEuCb.mem.pool,
                     (Data **)&pdcpId, sizeof(CmLtePdcpId)); 
               if (ret != ROK)
               {
                  /* Log error */
#ifdef DEBUGP
                  VE_DBG_ERROR((VE_PRNT_BUF,"VeChngTnlState: Alloc for PDCP ID Failed.\n"));
#endif
                  RETVALUE(RFAILED);
               }
               cmMemcpy ((U8 *)pdcpId, (U8 *)&(pdcpCb->pdcpId), sizeof(CmLtePdcpId));
               /* Triggering Data Req towards PJU */
               VeLiPjuDatReq(&pdcpCb->liSap->pst, pdcpCb->liSap->spId, pdcpId, 
                     pdcpCb->sduIdCntr++, 
                     (Buffer *)egtpBufInfo->data);

               SPutSBuf(veEuCb.mem.region, veEuCb.mem.pool, (Data *)egtpBufInfo, sizeof(EuEgtpBuffInfo));
               CM_LLIST_NEXT_NODE(&pdcpCb->datFwdDlEgtpTunCb->dataBuffLst, bufNode);
            }
         }

         /* send packets from normal tunel cb */
         if ((pdcpCb->egtpTunCb) && (stInfo->cause == VE_EU_END_MARKER_RECVD))
         {
            CM_LLIST_FIRST_NODE(&pdcpCb->egtpTunCb->dataBuffLst, bufNode);

            /* update the tunnel state */
            pdcpCb->egtpTunCb->state = stInfo->state;

            while(NULLP != bufNode && pdcpCb->egtpTunCb->dataBuffLst.count != 0)
            {
               egtpBufInfo = (EuEgtpBuffInfo *)bufNode->node;

               /* remove the node from the candLst */
               cmLListDelFrm(&pdcpCb->egtpTunCb->dataBuffLst, bufNode);

               ret = SGetSBuf(veEuCb.mem.region, veEuCb.mem.pool,
                     (Data **)&pdcpId, sizeof(CmLtePdcpId)); 
               if (ret != ROK)
               {
                  /* Log error */
#ifdef DEBUGP
                  VE_DBG_ERROR((VE_PRNT_BUF,"VeChngTnlState: Alloc for PDCP ID Failed.\n"));
#endif
                  RETVALUE(RFAILED);
               }
               cmMemcpy ((U8 *)pdcpId, (U8 *)&(pdcpCb->pdcpId), sizeof(CmLtePdcpId));
               /* Triggering Data Req towards PJU */
               VeLiPjuDatReq(&pdcpCb->liSap->pst, pdcpCb->liSap->spId, pdcpId, 
                     pdcpCb->sduIdCntr++, 
                     (Buffer *)egtpBufInfo->data);

               SPutSBuf(veEuCb.mem.region, veEuCb.mem.pool, (Data *)egtpBufInfo, sizeof(EuEgtpBuffInfo));
               CM_LLIST_NEXT_NODE(&pdcpCb->egtpTunCb->dataBuffLst, bufNode);
            }
         }
         break;
      case VE_EU_START_DATBUF:
         pdcpCb->egtpTunCb->state = stInfo->state;
         break;
      default:
         VE_DBG_ERROR((VE_PRNT_BUF,"veChngTnlState: Invalid State\n"));
         break;
   }
   RETVALUE(ROK);
} /* end of veChngTnlState */



/*
*
*       Fun:   VeDatCreateEgtpTunelSet
*
*       Desc:  Trigger Tunnel Creation for Set of tunnels. 
*              Tunnels can be forwarding tunnels or normal tunnels.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_data_app_hdl.c 
*
*/
#ifdef ANSI
PUBLIC S16 VeDatCreateEgtpTunelSet
(
Pst             *pst,
SuId            suId,
EuCrEGtpTunInf  *tunelInf
)
#else
PUBLIC S16 VeDatCreateEgtpTunelSet(pst, suId, tunelInf)
Pst             *pst;
SuId            suId;
EuCrEGtpTunInf  *tunelInf;
#endif /* ANSI */
{
   EgtUEvnt       *eguEvtMsg = NULLP;
   VeEgtpTunCb    *egtpCb    = NULLP;
   VePdcpCb       *pdcpCb    = NULLP;
   VeEuUeCb       *euUeCb    = NULLP;
   U8             idx        = 0;
   U8             idx1       = 0;
   EuGenEGtpTunInf *genEgtpInf = NULLP;
   U8             retVal     = RFAILED;
   VeUeCb          *ueCb = NULLP;
   Bool           istrgteNB = FALSE;

   TRC2(VeDatCreateEgtpTunelSet);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeDatCreatDatFwdEgtpTunel:\
            Triggering ETGP Tunnel Creation \n"));
#endif

   for (idx = 0;idx < tunelInf->numTunls; idx++)
   {
      genEgtpInf = &(tunelInf->tunLst[idx]);

      if (genEgtpInf->tunnType == VE_FORWARD_TUNNEL)
      {
         /* createa a EGTP control block */
         VE_ALLOC(&egtpCb, sizeof(VeEgtpTunCb));

         if ((genEgtpInf->tunInf.locTeid < VE_MAX_TUNNEL_ID) &&
               (veEuCb.egtpTunLst[genEgtpInf->tunInf.locTeid] == NULLP))
         {
            veEuCb.egtpTunLst[genEgtpInf->tunInf.locTeid] = egtpCb;
         }
         else
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"VeDatCreateEgtpTunelSet: \
                     Invalid Loc Tunnel Id\n"));
#endif
            VE_FREE(egtpCb, sizeof (VeEgtpTunCb));
            RETVALUE(RFAILED);
         }

         idx1 = VE_GET_UE_IDX(genEgtpInf->tunInf.pdcpId.ueId);
         idx1 = (idx1 * VE_MAX_RABS_IN_UE) + genEgtpInf->tunInf.pdcpId.rbId; 
         if ((idx1 < veEuCb.pdcpIdLstMaxSize) &&
               (veEuCb.pdcpIdLst[idx1] != NULLP))
         {
            pdcpCb = veEuCb.pdcpIdLst[idx1];
         }
         else
         {
            VE_DBG_ERROR((VE_PRNT_BUF,"VeDatCreateEgtpTunelSet: \
                     Invalid Pdcp Idx \n"));
            VE_FREE(egtpCb, sizeof (VeEgtpTunCb));
            RETVALUE(RFAILED);
         }
      }
      else
      {
         /* Only at the target eNB side this function will get called to create the
            Normal tunnels hence setting the flag*/
         istrgteNB = TRUE;
         retVal = VeDatCreatEgtpTunel(pst, suId, &genEgtpInf->tunInf);
         if(retVal == RFAILED)
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"VeDatCreateEgtpTunelSet: \
                     Creation of Tunnel [id = %ld] failed.\n",
                     genEgtpInf->tunInf.locTeid));
#endif
         }
         continue;
      }


      idx1 = VE_GET_UE_IDX(genEgtpInf->tunInf.pdcpId.ueId);
      if ( veEuCb.ueCbLst[idx1] != NULLP)
      {
         euUeCb = veEuCb.ueCbLst[idx1];
      }
      else
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM,(VE_PRNT_BUF,"VeDatCreateEgtpTunelSet: \
                  UE not present  \n"));
#endif
         VE_FREE(egtpCb, sizeof (VeEgtpTunCb));

         RETVALUE(RFAILED);
      }
      /* stop the inactivity timer */
      veInactvStopTmr((PTR)euUeCb, VE_TMR_INACTIVITY);

      if(veCb.cellCb[0] == NULLP || veCb.cellCb[0]->ueCbLst[idx1] == NULLP)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"VeDatCreateEgtpTunelSet:\
                  Locating CellCb/ueCb failed.\n"));
#endif
         RETVALUE(RFAILED);
      }
      ueCb = veCb.cellCb[0]->ueCbLst[idx1];
      /* Increment number of tunnels created */
      ueCb->numOfTunnelCreReq ++;


      cmMemcpy((U8 *)&egtpCb->pdcpId, (U8 *)&pdcpCb->pdcpId, sizeof(CmLtePdcpId));
      egtpCb->locTeId = genEgtpInf->tunInf.locTeid;
      egtpCb->remTeid = genEgtpInf->tunInf.remTeid;
      egtpCb->pdcpCb = pdcpCb;

      veCpyCmTptAddr(&(egtpCb->dstIpAddr),&(genEgtpInf->tunInf.dstIpAddr));
      veCpyCmTptAddr(&(egtpCb->frmDstIpAddr),&(genEgtpInf->tunInf.frmDstIpAddr));
      egtpCb->state = VE_EU_INITIAL;
      egtpCb->liSap = veEuCb.lsapLst[suId];
      cmLListInit(&egtpCb->dataBuffLst);

      /* Tunnel Info */
      if(genEgtpInf->rbDir == VE_UL)
      {
         pdcpCb->datFwdUlEgtpTunCb = egtpCb;
      }
      else
      {
         pdcpCb->datFwdDlEgtpTunCb = egtpCb;
      }

      /* Setting the normal data tunnel state to VE_EU_START_DATFWD */
      /* during this state all the packets sent or received via this state are 
         buffered */
      pdcpCb->egtpTunCb->state = VE_EU_START_DATBUF;
      if (TRUE != istrgteNB)
      {
         if (genEgtpInf->rbMode == NHU_RLC_MODE_AM)
         {  
            pdcpCb->egtpTunCb->datFwdIndFlag = VE_EU_FWD_OLD_PKTS;
         }
         else
         {
            pdcpCb->egtpTunCb->datFwdIndFlag = VE_EU_FWD_NEW_PKTS;
         }
      }
      else
         pdcpCb->egtpTunCb->datFwdIndFlag = VE_EU_NO_FWD; 
      veDatFillLocEgtpTunnel(egtpCb, &eguEvtMsg, EGT_TMGMT_REQTYPE_ADD);
      eguEvtMsg->event = EVTEGTULCLTNLMGMTREQ;
      /* Trigger primitive */
      VeLiEgtEguLclTnlMgmtReq(&egtpCb->liSap->pst, egtpCb->liSap->spId, eguEvtMsg);
   }
   RETVALUE(ROK);
} /* End of VeDatCreateEgtpTunelSet */


/*
*
*       Fun:   veFillEgtpDatFwdMsg 
*
*       Desc:  Populate Dat Req Msg for data forward indication
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_data_app_hdl.c  
*
*/
#ifdef ANSI
PUBLIC S16 veFillEgtpDatFwdMsg
(
EgtUEvnt       **eguEvtMsg,
VeEgtpTunCb    *egtpCb,
PjuDatFwdInfo  *datFwdInfo
)
#else
PUBLIC S16 veFillEgtpDatFwdMsg(eguEvtMsg, egtpCb, datFwdInfo)
EgtUEvnt       **eguEvtMsg;
VeEgtpTunCb    *egtpCb;
PjuDatFwdInfo  *datFwdInfo;
#endif /* ANSI */
{
   EgUMsgHdr *eguHdr = NULLP;
   S16       ret = ROK;

   TRC2(veFillEgtpDatFwdMsg);

  if(NULLP == egtpCb)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillEgtpDatMsg: egtpCb is NULL.\n"));

      RETVALUE(RFAILED);
   } 

   if ((ret = SGetSBuf(veEuCb.mem.region, veEuCb.mem.pool,\
               (Data **)eguEvtMsg, sizeof(EgtUEvnt))) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillEgtpDatMsg: SGetSBuf  failed for EgtUEvnt.\n"));
      RETVALUE(RFAILED);
   }

   cmMemset((U8*)*eguEvtMsg, 0, sizeof(EgtUEvnt));

   if (cmAllocEvnt(sizeof(EgUMsg), VE_EG_UTIL_MEM_SIZE,\
            &veEuCb.mem, (Ptr* )&((*eguEvtMsg)->u.egMsg)) != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillEgtpDatMsg: cmAllocEvnt failed for egMsg.\n"));
      RETVALUE(RFAILED);
   }

   /* Src & Dst Address */
   veCpyCmTptAddr(&((*eguEvtMsg)->u.egMsg->srcAddr),&veEuCb.srcAddr);
   veCpyCmTptAddr(&((*eguEvtMsg)->u.egMsg->remAddr),&(egtpCb->dstIpAddr));

   eguHdr = &(*eguEvtMsg)->u.egMsg->msgHdr;
   /* Populating Header Info */
   eguHdr->extHdr.udpPort.pres = TRUE;
   eguHdr->extHdr.pdcpNmb.pres = TRUE;

   eguHdr->msgType = EGT_GTPU_MSG_GPDU;

   if(!(datFwdInfo->sn))
   {
      eguHdr->seqNumber.pres = TRUE;
      eguHdr->seqNumber.val = datFwdInfo->sn;
   }
   else
   {
      eguHdr->seqNumber.pres = FALSE;
   }
   eguHdr->nPduNmb.pres = FALSE;
   eguHdr->extHdr.udpPort.pres = FALSE;
   eguHdr->extHdr.pdcpNmb.pres = FALSE;

   eguHdr->teId = egtpCb->remTeid;

   RETVALUE(ROK);
} /* veFillEgtpDatFwdMsg */ 


/*
*
*       Fun:   VeLiPjuDatFwdInd
*
*       Desc:  PJU Data Fwd Ind
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_li.c
*
*/
#ifdef ANSI
PUBLIC S16 VeLiPjuDatFwdInd
(
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
PjuDatFwdIndInfo   *datFwdInd
)
#else
PUBLIC S16 VeLiPjuDatFwdInd(pst, suId, pdcpId, datFwdInd)
Pst            *pst;
SuId           suId;
CmLtePdcpId    *pdcpId;
PjuDatFwdIndInfo   *datFwdInd;
#endif
{
   VePdcpCb     *pdcpCb = NULLP;
   VeCellCb     *cellCb  = NULLP;
   EgtUEvnt     *eguEvtMsg = NULLP;
   U16         idx = 0;
   VeEuUeCb     *euUeCb = NULLP;
   VeEgtpTunCb *egtpCb = NULLP;
   EuEgtpBuffInfo *egtpBufInfo = NULLP;
   U8 ret = RFAILED;
   Buffer *mBufTmp = NULLP;
   CmLList *bufNode = NULLP;
   
   TRC2(VeLiPjuDatFwdInd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "VeLiPjuDatFwdInd: PJU Data Fwd Ind\n"));
#endif


   cellCb = veCb.cellCb[0];
   idx = VE_GET_UE_IDX(pdcpId->ueId);
   
   /* Get VeEuUeCb from VeEuCb using pdcpId->ueID */
   euUeCb = veEuCb.ueCbLst[idx];
   if(euUeCb != NULLP)
   {
     euUeCb->datRcvd = TRUE;
     euUeCb->expiryCnt = 0;
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"VeLiPjuDatInd: Invalid UE index for ueId = %d\n",pdcpId->ueId));
   }
   
   idx = (idx * VE_MAX_RABS_IN_UE) + pdcpId->rbId; 
   if (idx >= veEuCb.pdcpIdLstMaxSize || (!(pdcpCb = veEuCb.pdcpIdLst[idx])))
   {
      SPutSBuf(pst->region, pst->pool, (Data *)pdcpId,\
                        sizeof(CmLtePdcpId));
      if(datFwdInd->numSdus)
      {
         SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd->datFwdInfo,\
               (sizeof(PjuDatFwdInfo)* datFwdInd->numSdus));
      }
      SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd,\
                        sizeof(PjuDatFwdIndInfo));
     RETVALUE(ROK);
   }

   if(datFwdInd->dir == PJ_DIR_DL)
   {
      if(pdcpCb->datFwdDlEgtpTunCb)
      {
         egtpCb = pdcpCb->datFwdDlEgtpTunCb;
      }
      else
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiPjuDatInd: \
                  Forward tunnels not configured for DL direction\n"));
         SPutSBuf(pst->region, pst->pool, (Data *)&pdcpId,\
               sizeof(CmLtePdcpId));
         /* in case when datFwdInd->numSdus is 0, i.e, no packets at PDCP, */
         if(datFwdInd->numSdus)
         {
            SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd->datFwdInfo,\
                  (sizeof(PjuDatFwdInfo)* datFwdInd->numSdus));
         }
         SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd,\
               sizeof(PjuDatFwdIndInfo));
         RETVALUE(RFAILED);
      }
   }
   else if(datFwdInd->dir == PJ_DIR_UL)
   {
      if(pdcpCb->datFwdUlEgtpTunCb)
      {
         egtpCb = pdcpCb->datFwdUlEgtpTunCb;
      }
      else
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiPjuDatInd: \
                  Forward tunnels not configured for UL direction\n"));
         SPutSBuf(pst->region, pst->pool, (Data *)pdcpId,\
               sizeof(CmLtePdcpId));
         if(datFwdInd->numSdus)
         {
            SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd->datFwdInfo,\
                  (sizeof(PjuDatFwdInfo)* datFwdInd->numSdus));
         }
         SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd,\
               sizeof(PjuDatFwdIndInfo));
         RETVALUE(RFAILED);
      }
   }

   for(idx = 0; idx < datFwdInd->numSdus; idx++)
   {
      eguEvtMsg = NULLP;
      /* Fill EGTP Data Message */
      if(ROK == (ret = veFillEgtpDatFwdMsg(&eguEvtMsg, egtpCb, &datFwdInd->datFwdInfo[idx])))
      {
         /* Create a copy of the data buffer */
         SCpyMsgMsg(datFwdInd->datFwdInfo[idx].sdu, veEuCb.mem.region, veEuCb.mem.pool, &mBufTmp);
        
         /* Buffer the Packet before sending to Peer */
         VE_ALLOC(&egtpBufInfo, sizeof(EuEgtpBuffInfo));
         if (NULLP == egtpBufInfo)
         {
            /* Log error */
            VE_DBG_ERROR((VE_PRNT_BUF,"VeLiPjuDatInd: \
                     Alloc Failed for EuEgtpBuffInfo\n"));
            SPutSBuf(pst->region, pst->pool, (Data *)pdcpId,\
                  sizeof(CmLtePdcpId));
            if(datFwdInd->numSdus)
            {
               SPutSBuf(pst->region, pst->pool, \
                     (Data *)datFwdInd->datFwdInfo,\
                     (sizeof(PjuDatFwdInfo)* datFwdInd->numSdus));
            }
            SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd,\
                  sizeof(PjuDatFwdIndInfo));
            RETVALUE(RFAILED);
         }
        
         egtpBufInfo->data = datFwdInd->datFwdInfo->sdu;
         cmLListAdd2Tail(&(egtpCb->dataBuffLst), &(egtpBufInfo->lnk));
         egtpBufInfo->lnk.node = (PTR)egtpBufInfo;

         eguEvtMsg->u.egMsg->u.mBuf = mBufTmp;

         /* Trigger EGTP Data Req */
         ret = VeLiEgtEguDatReq(&(egtpCb->liSap->pst), egtpCb->liSap->spId, eguEvtMsg);

         if(ret != ROK)
         {
            VE_DBG_ERROR((VE_PRNT_BUF,"VeLiPjuDatInd: \
                  Failed for VeLiEgtEguDatReq\n"));

            RETVALUE(RFAILED);
         }
       
      }
      else
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiPjuDatInd: \
                  Filling of EgtpDatMsg failed for pkt number [%d] sn [%d]\n", \
                  idx, datFwdInd->datFwdInfo[idx].sn));
      }
   }
   SPutSBuf(pst->region, pst->pool, (Data *)pdcpId,\
         sizeof(CmLtePdcpId));
   /* in case when datFwdInd->numSdus is 0, i.e, no packets at PDCP, */
   if(datFwdInd->numSdus)
   {
      SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd->datFwdInfo,\
            (sizeof(PjuDatFwdInfo)* datFwdInd->numSdus));
   }
   SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd,\
         sizeof(PjuDatFwdIndInfo));

   /* Start Forwarding of buffered packets */
   bufNode = pdcpCb->egtpTunCb->dataBuffLst.first;

   while(NULLP != bufNode)
   {
      mBufTmp = NULLP;
      eguEvtMsg = NULLP;
      egtpBufInfo = (EuEgtpBuffInfo *)bufNode->node;

      /* Fill EGTP Data Message */
      if(ROK != veFillEgtpDatMsg(egtpCb,&eguEvtMsg,EGT_GTPU_MSG_GPDU))
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiPjuDatInd: \
                  Failed for veFillEgtpDatMsg\n"));

         RETVALUE(RFAILED);
      }
     
      /* Create a copy of the data buffer and forward the packet */
      SCpyMsgMsg(egtpBufInfo->data, veEuCb.mem.region, veEuCb.mem.pool, &mBufTmp);

      eguEvtMsg->u.egMsg->u.mBuf = mBufTmp;

      /* Trigger EGTP Data Req */
      if(ROK != VeLiEgtEguDatReq(&(egtpCb->liSap->pst), egtpCb->liSap->spId, eguEvtMsg))
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"VeLiPjuDatInd: \
                  Failed for VeLiEgtEguDatReq\n"));

         RETVALUE(RFAILED);
      }

      bufNode = bufNode->next;
   }
   pdcpCb->egtpTunCb->datFwdIndFlag = VE_EU_FWD_NEW_PKTS;
   RETVALUE(ret);
} /* VeLiPjuDatFwdInd */


#endif

#endif /* End of LTE_ENB_PERF */
#endif
/********************************************************************30**

         End of file:     ve_data_app_hdl.c@@/main/2 - Wed Dec  1 08:44:22 2010

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
$SID$     ----       mnawas        1. eNodeb 3.1 Release
*********************************************************************91*/

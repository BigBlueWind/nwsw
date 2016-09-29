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
  
     Desc:     RRC user Management Interface handlers

     Ret :     ROK - success
               RFAILED - failure

     File:     ve_mi.c

     Sid:      ve_mi.c@@/main/2 - Wed Dec  1 08:44:26 2010

     Prg:      an 

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
#ifdef LTE_HO_SUPPORT
#include "czt_asn.h"
#endif
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
#include"ve_eut.h"

#ifdef VE_PERF_MEAS 
#include "ve_perf.h"

#ifdef VE_LNX_PCAP_PERF
#include <pcap.h>
#include "ve_rout.h"
#endif
#endif

/* RRM related includes */
#include "ve.h" 
#include "lve.h"
#include <signal.h>

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
#include "czt.x"
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "lrg.x"           /* MAC RRM control Interface */
#include"ve_eut.x"

/* RRM related includes */
#include "lve.x" 
#include "ve.x" 


#ifdef VE_LNX_PCAP_PERF
#include "ve_rout.x"
#endif


#include <signal.h>

#ifdef VE_PERF_MEAS 
EXTERN S16 vePerfActvTmr ARGS((Void));
#endif

EXTERN S16 veGenerateCfm ARGS((Pst *pst, Header *hdr, U8 prim, VeMngmt *cfm));

/*
 *
 *       Fun:     veSendLmAlarm
 *
 *       Desc:    Function trigger alarm
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  None
 *
 *       File:    ve_mi.c
 *
 */
#ifdef ANSI
PUBLIC Void veSendLmAlarm
(
U16 category,           
U16 event,             
U16 cause            
)
#else
PUBLIC Void veSendLmAlarm(category, event, cause)
U16 category;       
U16 event;         
U16 cause;        
#endif
{
   VeMngmt usta;        /* Unsolicited Status Indication structure */

   TRC2(veSendLmAlarm);

#ifdef VE_RELAY
   if (veCb.init.usta == FALSE)
   {
      RETVOID;
   }
#endif

#ifdef EU_DAT_APP
   if (veEuCb.init.usta == FALSE)
   {
      RETVOID;
   }
#endif

   /* Initialize the management structure */
   (Void)cmMemset((U8 *) &usta, '\0', (PTR)sizeof(VeMngmt));

   usta.hdr.elmId.elmnt = STVEGEN;
#ifdef VE_RELAY
   usta.hdr.entId.ent   = veCb.init.ent;
   usta.hdr.entId.inst  = veCb.init.inst;
#endif

   usta.u.usta.alarm.category = category;
   usta.u.usta.alarm.event    = event;
   usta.u.usta.alarm.cause    = cause;

#ifdef VE_RELAY
   /* Send an unsolicited status indication */
   VeMiLveStaInd(&veCb.init.lmPst, &usta);
#endif

   RETVOID;

} /* veSendLmAlarm */

#ifdef VE_RELAY
/*
 *
 *       Fun:     veActvInit
 *
 *       Desc:    
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  The task initializes its global variables.
 *
 *       File:    ve_mi.c
 *
 */
#ifdef ANSI
PUBLIC S16 veActvInit
(
Ent    entity,    /* Entity   */
Inst   inst,      /* Instance */
Region region,    /* Region   */
Reason reason     /* reason   */
)
#else
PUBLIC S16 veActvInit(entity, inst, region, reason)
Ent    entity;    /* Entity   */
Inst   inst;      /* Instance */
Region region;    /* Region   */
Reason reason;    /* reason   */
#endif
{
   TRC2(veActvInit);

   veCb.init.procId  = SFndProcId();
   veCb.init.ent     = entity;
   veCb.init.inst    = inst;
   veCb.init.region  = region;
   veCb.init.reason  = reason;
   veCb.init.cfgDone = FALSE;
   veCb.init.pool    = 1;
   veCb.init.acnt    = FALSE;
   veCb.init.trc     = FALSE; 
   veCb.init.usta    = TRUE;

#ifdef DEBUGP
   veCb.init.dbgMask = 0;
#endif /* end of DEBUGP */

   veCb.mem.region = DFLT_REGION;
   veCb.mem.pool = DFLT_POOL;

#if (defined(VE_PERF_MEAS) && defined(VE_LNX_PCAP_PERF))
      /* Route Init */ 
      veAppRoutInit();
#endif

#ifdef EU_DAT_APP
   veEuCb.init.procId  = SFndProcId();
   veEuCb.init.ent     = entity;
   veEuCb.init.inst    = inst;
   veEuCb.init.region  = region;
   veEuCb.init.reason  = reason;
   veEuCb.init.cfgDone = FALSE;
   veEuCb.init.pool    = 1;
   veEuCb.init.acnt    = FALSE;
   veEuCb.init.trc     = FALSE; 
   veEuCb.init.usta    = TRUE;

   veEuCb.mem.region = DFLT_REGION;
   veEuCb.mem.pool = DFLT_POOL;
#endif
/* Initializing log file informations */
#if (VE_SM_LOG_TO_FILE && DEBUGP)
   veCb.dbgFp = NULLP;
   veCb.nmbDbgLines = 0;
   veCb.cfgDbgLines = 1000;
   cmMemset( (U8* )veCb.filePath, 0, LVE_MAX_FILE_PATH);
#endif   

#ifdef WIRESHARK_LOG
   veInitWireShark();
#endif

   RETVALUE(ROK);
} /* veActvInit */
#endif

/*
 *
 *       Fun:     veGenerateCfm 
 *
 *       Desc:    To generate confirms
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  None.
 *
 *       File:    ve_mi.c
 *
 */
#ifdef ANSI
PUBLIC S16  veGenerateCfm
(
Pst                     *pst,
Header                  *hdr,
U8                      prim,
VeMngmt                 *cfm
)
#else
PUBLIC S16 veGenerateCfm (pst, hdr, prim, cfm)
Pst                     *pst;
Header                  *hdr;
U8                      prim;
VeMngmt                 *cfm;
#endif
{
   Pst                  cPst;

   TRC2(veGenerateCfm);

   /* Fill the pst structure for LM i/f */
#ifdef VE_RELAY
   cPst.srcEnt    = veCb.init.ent;
   cPst.srcInst   = veCb.init.inst;
#endif

   cPst.srcProcId = SFndProcId();

   /*
    * If genconfig is done, fill the destn identity
    * from the pst configured, else use the incoming
    * pst structure for destination identity.
    */
#ifdef VE_RELAY
   if (TRUE == veCb.init.cfgDone)
   {  
      cPst.dstEnt    = veCb.init.lmPst.dstEnt;
      cPst.dstInst   = veCb.init.lmPst.dstInst;
      cPst.dstProcId = veCb.init.lmPst.dstProcId;
   }
   else
   {  
      cPst.dstEnt    = pst->srcEnt;
      cPst.dstInst   = pst->srcInst;
      cPst.dstProcId = pst->srcProcId;
   }
#endif

#ifdef EU_DAT_APP
   if (TRUE == veEuCb.init.cfgDone)
   {  
      cPst.dstEnt    = veEuCb.init.lmPst.dstEnt;
      cPst.dstInst   = veEuCb.init.lmPst.dstInst;
      cPst.dstProcId = veEuCb.init.lmPst.dstProcId;
   }
   else
   {  
      cPst.dstEnt    = pst->srcEnt;
      cPst.dstInst   = pst->srcInst;
      cPst.dstProcId = pst->srcProcId;
   }
#endif

   cPst.selector  = hdr->response.selector;
   cPst.prior     = hdr->response.prior;
   cPst.route     = hdr->response.route;
   cPst.region    = hdr->response.mem.region;
   cPst.pool      = hdr->response.mem.pool;

   /* Fill the Header in cfm structure */
   cmMemset ((U8 *)&cfm->hdr, '\0', sizeof(Header));
   cfm->hdr.elmId.elmnt = hdr->elmId.elmnt;
#ifdef VE_RELAY
   cfm->hdr.entId.ent   = veCb.init.ent;
   cfm->hdr.entId.inst  = veCb.init.inst;
#endif

   /* Fill the transcation for which the cfm is being sent */
   cfm->hdr.transId = hdr->transId;

   switch (prim)
   {  
      case TCFG:
         cmMemset ((U8 *)&cfm->u.cfg, 0, sizeof(VeCfg));
         VeMiLveCfgCfm (&cPst, cfm);
         break;   

      case TCNTRL:
         VeMiLveCntrlCfm (&cPst, cfm);
         break;
   }  

   RETVALUE(ROK);
} /* veGenerateCfm */

#ifdef VE_RELAY

/********************************************************************IB**
 Layer Management Incoming Primitives
*********************************************************************IB*/

/*
 *
 *       Fun:     veUbndLSap
 *
 *       Desc:    This function is invoked to unbind & disable a SAP 
 *                A disconnect is sent and the SAP state is changed
 *                to VE_SAP_CFG without sending a unbind request.
 *
 *       Ret:     ROK or LCM_REASON_INVALID_STATE
 *
 *       Notes :  This function provides run time support for the
 *                stack reinitialization procedure.
 *
 *       File:    ve_mi.c
 *
 */
#ifdef ANSI
PRIVATE S16 veUbndLSap
(
VeLiSapCb *sapCb   /* Lower SAP control block */
)
#else
PRIVATE S16 veUbndLSap(sapCb)
VeLiSapCb *sapCb;   /* Lower SAP control block */
#endif
{
   S16       retVal;

   TRC2(veUbndLSap);

   switch(sapCb->sapState)
   {
      case LVE_SAP_BINDING:
         sapCb->sapState = LVE_SAP_UNBOUND;
         retVal = ROK;
         break;

      case LVE_SAP_BOUND:
         sapCb->sapState = LVE_SAP_UNBOUND;
         retVal = ROK;
         break;

      default:
         retVal = LCM_REASON_INVALID_STATE;
         break;
   }

   RETVALUE(retVal);
} /* veUbndLSap */

/*
 *
 *       Fun:     veBndLSap
 *
 *       Desc:    This function binds the lower sap.
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  This function is a state transition action handler
 *                for lower SAP ABND_ENA control request. 
 *
 *       File:    ve_mi.c
 *
 */
#ifdef ANSI
PRIVATE S16 veBndLSap
(
VeLiSapCb               *sapCb,
CmStatus                *status,
Elmnt                   elmnt
)
#else
PRIVATE S16 veBndLSap(sapCb, status, elmnt)
VeLiSapCb               *sapCb;
CmStatus                *status;
Elmnt                   elmnt;
#endif
{
   TRC2(veBndLSap);

   if (LVE_SAP_UNBOUND == sapCb->sapState)
   {
      status->status = LCM_PRIM_OK_NDONE;
      sapCb->sapState = LVE_SAP_BINDING;
      sapCb->bndRetryCnt = 0;

      switch(elmnt)
      {
         case STVENHUSAP:
            veStartTmr ((PTR)sapCb, VE_TMR_NHU_SAP_BND, sapCb->bndTmr.val);
            VeLiNhuBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
         case STVERGRSAP:
            veStartTmr ((PTR)sapCb, VE_TMR_RGR_SAP_BND, sapCb->bndTmr.val);
            VeLiRgrBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
         case STVECTFSAP:
            veStartTmr ((PTR)sapCb, VE_TMR_CTF_SAP_BND, sapCb->bndTmr.val);
            VeLiCtfBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
#ifndef LTE_ENB_PERF
         case STVESZTSAP:
            veStartTmr ((PTR)sapCb, VE_TMR_SZT_SAP_BND, sapCb->bndTmr.val);
            VeLiSztBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
#ifdef LTE_HO_SUPPORT
         case STVECZTSAP:
            veStartTmr ((PTR)sapCb, VE_TMR_CZT_SAP_BND, sapCb->bndTmr.val);
            VeLiCztBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
#endif
#endif
#ifdef EU_DAT_APP
#ifndef LTE_ENB_PERF
         case STVEEGTSAP:
            veStartTmr ((PTR)sapCb, VE_TMR_EGT_SAP_BND, sapCb->bndTmr.val);
            VeLiEgtBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
#endif
         case STVEPJUSAP:
            veStartTmr ((PTR)sapCb, VE_TMR_PJU_SAP_BND, sapCb->bndTmr.val);
            VeLiPjuBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
#endif 

         default:
            break;
      }
   }
   else
   {
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_INVALID_STATE;
   }

   RETVALUE (ROK);
} /* veBndLSap */

/*
 *
 *       Fun:     veLSapCntrl
 *
 *       Desc:    This function processes control request for the
 *                Lower SAP.
 *
 *       Ret:     ROK - Ok/RFAILED - Failure 
 *
 *       Notes :  This function implements the State transition
 *                for Lower SAP control requests. The function
 *                is the entry point for all control request
 *                events defined.
 *
 *       File:    ve_mi.c
 *
 */
#ifdef ANSI
PRIVATE S16 veLSapCntrl
(
VeCntrl                 *sapCntrl,
CmStatus                *status,
Elmnt                   elmnt
)
#else
PRIVATE S16 veLSapCntrl(sapCntrl, status, elmnt)
VeCntrl                 *sapCntrl;
CmStatus                *status;
Elmnt                   elmnt;
#endif
{
   VeLiSapCb            *lSapCb = NULLP;
   SuId                 lSapId;
   S16                  retVal;

   retVal = ROK;

   TRC2(veLSapCntrl);

   /* Validate the parameters */
   lSapId = sapCntrl->u.sapId;

   switch (elmnt) {
      case STVERGRSAP:
         lSapCb = veCb.rgrSap[0];
         break;
      case STVENHUSAP:
         lSapCb = veCb.nhuSap[0];
         break;
      case STVECTFSAP:
         lSapCb = veCb.ctfSap[0];
         break;
      case STVESZTSAP:
         lSapCb = veCb.sztSap[0];
         break;
#ifdef LTE_HO_SUPPORT
      case STVECZTSAP:
         lSapCb = veCb.cztSap[0];
         break;
#endif
#ifdef EU_DAT_APP
      case STVEEGTSAP:
      case STVEPJUSAP:
         lSapCb = veEuCb.lsapLst[lSapId];
         break;
#endif
      default:
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_PAR_VAL;
         RETVALUE(RFAILED);
   }

   /* Initialize the status members to success */
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   switch (sapCntrl->action)
   {
      case ABND_ENA:
         /* Bind Lower Sap  */
         veBndLSap(lSapCb, status, elmnt);

         /* Set return value to ROK */
         retVal = ROK;
         break;

      case AUBND_DIS:
         /* Disconnect PVC and DONT send Unbind Request */
         retVal = veUbndLSap(lSapCb);
         if (ROK != retVal)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCntrl: Invalid Receiver\
                     State in call to nuUbndLSap\n"));
#endif
            status->status = LCM_PRIM_NOK;
            status->reason = retVal;
         }
         break;

      default:
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCntrl: Invalid Action Value \n"));
#endif
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_ACTION;
         retVal = RFAILED;
         break;
   }

   RETVALUE(retVal);
} /* veLSapCntrl */

#endif

/*
 *
 *       Fun:     veShutdown
 *
 *       Desc:    
 *
 *       Ret:     None. 
 *
 *       Notes :  This function is invoked on a control request
 *                to shutdown the layer. All the data structures
 *                allocated during configuration are freed.
 *
 *       File:    ve_mi.c
 *
 */
#ifdef ANSI
PRIVATE Void veShutdown
(
Void
)
#else
PRIVATE Void veShutdown()
#endif
{
   TRC2(veShutdown);
#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veShutdown: eNodeB Layer Shutdown\n"));
#endif

#ifdef VE_RELAY
   /* Reintialize the layer - the reason parameter is not used */
   veActvInit(veCb.init.ent, veCb.init.inst, veCb.init.region, 
             veCb.init.reason);
#endif

   RETVOID;
} /* veShutdown */

/*
 *
 *       Fun:     veGenCfg
 *
 *       Desc:    This function is invoked to do the general 
 *                configuration for RRC User Layer.
 *
 *       Ret:     ROK     - ok
 *                RFAILED - Failure
 *                ROUTRES - Memory Allocation Failure due to resource
 *                          crunch.
 *
 *       Notes :  The general configuration configures the lmPst
 *                to use in all calls to the Stack Manager. This
 *                function reserves the static memory requirements
 *                for the layer and configures the static data strs. 
 *
 *       File:    ve_mi.c
 *
 */
#ifdef ANSI
PRIVATE S16 veGenCfg 
(
VeGenCfg                *gen,
CmStatus                *status
)
#else
PRIVATE S16 veGenCfg (gen, status)
VeGenCfg                *gen;
CmStatus                *status;
#endif
{
#ifdef VE_RELAY
   VeCellCb   *cellCb = NULLP;
   U8         idx = 0;
   U8         prmblId = 0;
   U8         maxPrmbls = 0;
   U16        rnti = 0;
   U16        maxRnti = 0;
   U16 pdcpIdLstMaxSize = 0;     
   VeRntinode  *rntiNode = NULLP;
   VePrmblnode *prmblNode = NULLP;
#ifdef LTE_HO_SUPPORT
   S16 loopCnt = 0;
#endif
#endif
   VeMmeCb      *tmpMmeCb;

   TRC2(veGenCfg);

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

#ifdef VE_RELAY

   if (TRUE != veCb.init.cfgDone)
   {
      cmMemcpy((U8 *)&(veCb.init.lmPst), (U8 *)&(gen->lmPst),
            (PTR)sizeof(Pst));

      veCb.init.lmPst.srcProcId = gen->lmPst.srcProcId;
      veCb.init.lmPst.srcEnt    = gen->lmPst.srcEnt;
      veCb.init.lmPst.srcInst   = gen->lmPst.srcInst;
      veCb.init.lmPst.event     = EVTNONE;
      veCb.init.cfgDone = TRUE;
   }

   /* allocate Cell Cb */
   VE_ALLOC(&veCb.cellCb, sizeof(VeCellCb *));
   /* klock warning fixed */
   if(veCb.cellCb == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veGenCfg:\
               Failure in the allocation sap \n"));
#endif   
      RETVALUE(RFAILED);
   } /* end of if statement */

   VE_ALLOC(&veCb.cellCb[0], sizeof(VeCellCb));
   if(NULLP == veCb.cellCb[0])
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,":veLGenCfg Failed to allocate Memory.\n"));
#endif
      RETVALUE(RFAILED);
   }

   /* configure cell related info */
   cellCb = veCb.cellCb[0];
   cellCb->state = VE_INIT_STATE;
   cellCb->cellId = gen->cellCfg.cellId;
   cellCb->modType = gen->cellCfg.modType;
   cellCb->duplexMode = gen->cellCfg.duplexMode;
   cellCb->maxUeSupp = (U8)(gen->cellCfg.maxMacRntis + gen->cellCfg.maxRrmRntis);
   cellCb->numUeCfg = VE_VAL_ZERO;
   /* plmn identity */
   for (idx = 0; idx < 3; idx++)
   {
      /* mcc */
      if(gen->cellCfg.mcc[idx].pres == PRSNT_NODEF)
      {
         cellCb->sysRelInfo.mcc[idx].pres = PRSNT_NODEF;
         cellCb->sysRelInfo.mcc[idx].val = gen->cellCfg.mcc[idx].val;
      }
      /* mnc */
      if(gen->cellCfg.mnc[idx].pres == PRSNT_NODEF)
      {
         /* changes are incorporated*/
         cellCb->sysRelInfo.mnc_nocomp++;
         cellCb->sysRelInfo.mnc[idx].pres = PRSNT_NODEF;
         cellCb->sysRelInfo.mnc[idx].val = gen->cellCfg.mnc[idx].val;
      }
   }

   cellCb->sysRelInfo.trackAreaCode =  gen->cellCfg.trackAreaCode;
   cellCb->sysRelInfo.freqBandInd = gen->cellCfg.freqBandInd;
   /* Filling the eNB Type */
   veCb.eNBType = gen->eNBType;
#ifdef LTE_HO_SUPPORT
   /* Filling the Neighbor Cell Informations */
   veCb.numNghInfo = gen->noOfNghInfo;
   veCb.nghInfo = (VeNghInfoLst **) gen->nghInfo;
   /* Filling the MME group Informations */
   veCb.numguGrp = (U8) gen->noOfguGrp;
   veCb.guGrpId = (VeGuGrpIdLst**) gen->guGrpId;

   /* Filling the S1 Handover Timers */
   veCb.s1PrepTimerVal = gen->s1PrepTimerVal;
   veCb.s1OvrAllTimerVal = gen->s1OvrAllTimerVal;

   /* Filling UE HO Attach Timer */
   veCb.ueHoAttachTimerVal = gen->ueHoAttachTimerVal;
    
   /* Filling PCI and TAC values */
   cellCb->physCellId = gen->cellCfg.physCellId;
   cellCb->tac = gen->cellCfg.trackAreaCode;
   /* FDD and TDD properties configuration */
   if(cellCb->duplexMode == VE_MODE_FDD)
   {
      cellCb->u.fddInfo.ulEarfcn = gen->cellCfg.fddCfg.ulEarfcn;
      cellCb->u.fddInfo.dlEarfcn = gen->cellCfg.fddCfg.dlEarfcn;
      cellCb->u.fddInfo.ulTxBndWdth = (VeTxBndWdth) gen->cellCfg.fddCfg.ulTxBndWdth;
      cellCb->u.fddInfo.dlTxBndWdth = (VeTxBndWdth) gen->cellCfg.fddCfg.dlTxBndWdth;
   }
   else
   { 
      cellCb->u.tddInfo.earfcn = gen->cellCfg.tddCfg.earfcn;
      cellCb->u.tddInfo.txBndWdth = (VeTxBndWdth) gen->cellCfg.tddCfg.txBndWdth;
      cellCb->u.tddInfo.sfAssgnmnt = (VeSubFrameAssignment) gen->cellCfg.tddCfg.sfAssgnmnt;
      cellCb->u.tddInfo.splSfInfo.splSfPatrn = (VeSplSubFrmPattern) gen->cellCfg.tddCfg.splSfPatrn;
      cellCb->u.tddInfo.splSfInfo.dlCycPrefix = (VeCyclicPrefix) gen->cellCfg.tddCfg.dlCycPrefix;
      cellCb->u.tddInfo.splSfInfo.ulCycPrefix = (VeCyclicPrefix) gen->cellCfg.tddCfg.ulCycPrefix;
   }
   cellCb->numBrdCstPlmn = gen->cellCfg.noOfBrdCstPlmn;
   for (loopCnt = 0; loopCnt < cellCb->numBrdCstPlmn; loopCnt++)
   {
      strcpy((S8*) cellCb->plmnId[loopCnt], (const S8*) gen->cellCfg.plmnId[loopCnt]);
   }

   /* serving cell RSRP Threshold configuration */
   cellCb->rsrpTrshldRange = gen->cellCfg.rsrpTrshldRange; 
#endif /* End of LTE_HO_SUPPORT */
   /* RNTI configuration */
   rnti = (gen->cellCfg.macRntiStart + gen->cellCfg.maxMacRntis);
   cellCb->rntiDb.rntiStart = rnti;
   maxRnti = gen->cellCfg.maxRrmRntis;
   cellCb->rntiDb.maxRrmRntis  = maxRnti;
   
   /* Initializing RNTI Management Lists */
   cmLListInit(&(cellCb->rntiDb.freeRntiLst));
   cmLListInit(&(cellCb->rntiDb.inUseRntiLst));

   /* Filling only free RNTI list as the inuse RNTI list will
      get filled as and when it's get assigned */
   for (idx = 0; idx < maxRnti; idx++)
   {
      VE_ALLOC(&rntiNode, sizeof(VeRntinode));
      if(NULLP == rntiNode)
      {
#ifdef VE_DBG_ERROR
         VE_DBG_ERROR((VE_PRNT_BUF,"veGenCfg : %d :\
                  Alloc for RNTI Node failed \n", __LINE__));
#endif
         RETVALUE(RFAILED);
      }
      rntiNode->rnti = (U8) rnti++;
      rntiNode->lnk.node = (PTR)rntiNode;
      cmLListAdd2Tail(&cellCb->rntiDb.freeRntiLst,&rntiNode->lnk);
   }/* End of for() */
   
   /* Preamble configuration */
   prmblId = (U8)gen->cellCfg.rrmPrmblStart;
   cellCb->prmblDb.prmblStart = prmblId;
   maxPrmbls = (U8)gen->cellCfg.maxRrmPrmbls; 
   cellCb->prmblDb.maxPrmbls = maxPrmbls;
   
   /* Initializing preamble lists */
   cmLListInit(&cellCb->prmblDb.freePrmblLst);
   cmLListInit(&cellCb->prmblDb.inUsePrmblLst);

   /* Filling only free preamble list as the inuse preamble list will
      get filled as and when it's get assigned */
   for (idx = 0; idx < maxPrmbls; idx++)
   {
      VE_ALLOC(&prmblNode, sizeof(VePrmblnode));
      if (NULLP == prmblNode)
      {
#ifdef VE_DBG_ERROR
         VE_DBG_ERROR((VE_PRNT_BUF,"veGenCfg : %d :\
                     Alloc for PreambleIdNode has failed \n", __LINE__));
#endif
         RETVALUE(RFAILED);
      }
      prmblNode->prmblId = prmblId++;
      prmblNode->lnk.node = (PTR)prmblNode;
      cmLListAdd2Tail(&cellCb->prmblDb.freePrmblLst,&prmblNode->lnk);      
   }/* End of for() */
   /* MAC rnti And Preamble configurations */
   cellCb->veDfltMacRntisize = gen->cellCfg.maxMacRntis;
   cellCb->veDfltStartRnti = gen->cellCfg.macRntiStart;
   cellCb->veDfltNumRaPreamble = gen->cellCfg.numOfMacPrm;

   veCpyCmTptAddr(&veCb.datAppAddr,&gen->dataAppAddr);

   /* PHY configuration parameters */
   cellCb->opMode  = gen->cellCfg.opMode;
   cellCb->counter = gen->cellCfg.counter;
   cellCb->period  = gen->cellCfg.period;

   veCb.init.cfgDone = TRUE;
#endif

#ifdef EU_DAT_APP

   if (TRUE != veEuCb.init.cfgDone)
   {
      cmMemcpy((U8 *)&(veEuCb.init.lmPst), (U8 *)&(gen->lmPst),
            (PTR)sizeof(Pst));

      veEuCb.init.lmPst.srcProcId = gen->lmPst.srcProcId;
      veEuCb.init.lmPst.srcEnt    = gen->lmPst.srcEnt;
      veEuCb.init.lmPst.srcInst   = gen->lmPst.srcInst;
      veEuCb.init.lmPst.event     = EVTNONE;
      veEuCb.init.cfgDone = TRUE;
#ifdef VE_PERF_MEAS
#ifndef VE_LNX_PCAP_PERF
      /* Register Timer */
      cmInitTimers(veEuCb.loadTimers, VE_PERF_MAX_TMR);
      veEuCb.sduCntr = 0;
#endif /* VE_LNX_PCAP_PERF */
#endif /* VE_PERF_MEAS */
   }

   VE_ALLOC(&veEuCb.lsapLst, (VE_MAX_SAPS * sizeof(VeLiSapCb *)));
   /* Initialize EGTP-U Tunnel set and PDCP Id set as well */
#ifndef VE_PERF_MEAS
#ifndef VE_LNX_PCAP_PERF
   VE_ALLOC(&veEuCb.egtpTunLst, (VE_MAX_TUNNEL_ID * sizeof(VeEgtpTunCb *)));
#endif /* VE_LNX_PCAP_PERF */
#endif /* VE_PERF_MEAS */
   pdcpIdLstMaxSize = (cellCb->maxUeSupp *  VE_MAX_RABS_IN_UE);   
   veEuCb.pdcpIdLstMaxSize = pdcpIdLstMaxSize; 
   VE_ALLOC(&veEuCb.pdcpIdLst,  ((pdcpIdLstMaxSize + 1)  * sizeof(VePdcpCb *)));
   VE_ALLOC(&veEuCb.ueCbLst,  ( VE_MAX_UE * sizeof(VeEuUeCb *)));
   VE_ALLOC(&cellCb->ueCbLst, ( cellCb->maxUeSupp * sizeof(VeUeCb *)));

   /* Initializing pendPagUesLst */
   cmLListInit(&(cellCb->pendPagUesLst));
#ifdef LTE_HO_SUPPORT
   /* Initializing neighbour Cell List */
   cmLListInit(&(cellCb->nghCellLst));
   VE_ALLOC(&veCb.x2apConLst, (VE_MAX_X2AP_CON * sizeof( VeX2ConCb *)));
#endif
   /* NAS NON Delivery Indication */
   cmHashListInit(&cellCb->vePduHashLst, NUM_PDU_BINS,
         VE_GET_OFFSET(VeNasInfo, hlEnt), FALSE,
         CM_HASH_KEYTYPE_U32MOD, VE_MEM_REGION, VE_MEM_POOL);
   VE_ALLOC(&veCb.s1apConLst, (VE_MAX_S1AP_CON * sizeof( VeS1ConCb *)));


   veCpyCmTptAddr(&veEuCb.srcAddr,&gen->dataAppAddr);
   veEuCb.init.cfgDone = TRUE;
#endif
    
/* #ifndef LTE_ENB_PAL */
   if (veRegInitTmr() != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veGenCfg: veRegInitTmr Failed\n"));
#endif
      RETVALUE (RFAILED);
   }

#ifndef LTE_ENB_PERF
   if (veRegInitDatTmr() != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veGenCfg: veRegInitDatTmr Failed\n"));
#endif
      RETVALUE (RFAILED);
   }
#endif /* end of LTE_ENB_PERF */

/* #endif   */
   for(idx = 0; idx < VE_MAX_MMES; idx++)
   {
      VE_ALLOC(&tmpMmeCb, sizeof(VeMmeCb));
      veCb.mmeCont.mmes[idx] = tmpMmeCb;
   }
   veCb.mmeCont.numMmes = 0;

   RETVALUE(ROK);
} /* veGenCfg */

#ifdef VE_RELAY
/*
 *
 *       Fun:     veLSapCfg
 * 
 *       Desc:    This function is used to configure the Lower SAP.
 * 
 *       Ret:     ROK - ok / RFAILED - Failure 
 *                / ROUTRES - Memory out of resources.
 *
 *       Notes :  None.
 *
 *       File:    ve_mi.c
 * 
 */
#ifdef ANSI
PRIVATE S16 veLSapCfg
(     
VeMngmt      *cfg,
CmStatus     *status
)
#else
PRIVATE S16 veLSapCfg(cfg, status)
VeMngmt      *cfg;
CmStatus     *status;
#endif
{
   VeLiSapCb *sap = NULLP;
   VeLSapCfg *lSap = &cfg->u.cfg.u.lSap;
   U16 idx;

   TRC2(veLSapCfg);

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   if (NULLP == lSap)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCfg: Invalid Sap Cfg \n"));
#endif
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_INVALID_PAR_VAL;
      RETVALUE(RFAILED);
   }

   switch(cfg->hdr.elmId.elmnt)
   {
      case STVENHUSAP:
         /* Allocate memory */
         VE_ALLOC(&veCb.nhuSap, sizeof(VeLiSapCb *));
         /* klock warning fixed */
         if(veCb.nhuSap == NULLP)
         {       
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCfg:\
                     Failure in the allocation sap \n"));
#endif
            RETVALUE(RFAILED);
         } /* end of if statement */

         VE_ALLOC(&veCb.nhuSap[0], sizeof(VeLiSapCb));
         if(NULLP == veCb.nhuSap[0])
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCfg: Failed to allocate Memory.\n"));
#endif
            RETVALUE(RFAILED);
         }
         sap = veCb.nhuSap[0];
         break;
      case STVERGRSAP:
         /* Allocate memory */
         VE_ALLOC(&veCb.rgrSap, sizeof(VeLiSapCb *));
         /* klock warning fixed */
         if(veCb.rgrSap == NULLP)
         {       
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCfg:\
                     Failure in the allocation sap \n"));
#endif   
            RETVALUE(RFAILED);
         } /* end of if statement */

         VE_ALLOC(&veCb.rgrSap[0], sizeof(VeLiSapCb));
         if(NULLP == veCb.rgrSap[0])
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCfg: Failed to allocate Memory.\n"));
#endif
            RETVALUE(RFAILED);
         }
         sap = veCb.rgrSap[0];
         break;
      case STVECTFSAP:
         /* Allocate memory */
         VE_ALLOC(&veCb.ctfSap, sizeof(VeLiSapCb *));
         /* klock warning fixed */
         if(veCb.ctfSap == NULLP)
         {       
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCfg:\
                     Failure in the allocation sap \n"));
#endif   
            RETVALUE(RFAILED);
         } /* end of if statement */
         VE_ALLOC(&veCb.ctfSap[0], sizeof(VeLiSapCb));
         if(NULLP == veCb.ctfSap[0])
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCfg: Failed to allocate Memory.\n"));
#endif
            RETVALUE(RFAILED);
         }
         sap = veCb.ctfSap[0];
         break;
      case STVESZTSAP:
         /* Allocate memory */
         VE_ALLOC(&veCb.sztSap, sizeof(VeLiSapCb *));
         /* klock warning fixed */
         if(veCb.sztSap == NULLP)
         {       
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCfg:\
                     Failure in the allocation sap \n"));
#endif   
            RETVALUE(RFAILED);
         } /* end of if statement */
         VE_ALLOC(&veCb.sztSap[0], sizeof(VeLiSapCb));
         if(NULLP == veCb.sztSap[0])
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCfg: Failed to allocate Memory.\n"));
#endif
            RETVALUE(RFAILED);
         }
         sap = veCb.sztSap[0];

         for(idx = 0; idx < VE_MAX_MMES; idx++)
         {
            if(veCb.mmeCont.mmes[idx])
               veCb.mmeCont.mmes[idx]->s1apSap = veCb.sztSap[0];
         }

         break;

#ifdef LTE_HO_SUPPORT
      case STVECZTSAP:
         /* Allocate memory */
         VE_ALLOC(&veCb.cztSap, sizeof(VeLiSapCb *));
         if(NULLP == veCb.cztSap)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCfg: Failed to allocate Memory.\n"));
#endif
            RETVALUE(RFAILED);
         }
         VE_ALLOC(&veCb.cztSap[0], sizeof(VeLiSapCb));
         if(NULLP == veCb.cztSap[0])
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCfg: Failed to allocate Memory.\n"));
#endif
            RETVALUE(RFAILED);
         }
         sap = veCb.cztSap[0];
         break;
#endif
#ifdef EU_DAT_APP
      case STVEEGTSAP:
      case STVEPJUSAP:
         /*         sap = veEuCb.lsapLst[lSap->suId]; */
         VE_ALLOC(&veEuCb.lsapLst[lSap->suId], sizeof(VeLiSapCb));
         if(NULLP == veEuCb.lsapLst[lSap->suId])
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCfg: Failed to allocate Memory.\n"));
#endif
            RETVALUE(RFAILED);
         }
         sap = veEuCb.lsapLst[lSap->suId];
         break;
#endif 
      default:
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veLSapCfg: Invalid Sap Element\n"));
#endif
         RETVALUE(RFAILED);
         break;
   }

   sap->suId = lSap->suId;
   sap->spId = lSap->spId;
   sap->pst.srcProcId = SFndProcId();
   sap->pst.srcEnt = lSap->srcEnt;
   sap->pst.srcInst = lSap->srcInst;
   sap->pst.dstProcId = lSap->dstProcId;
   sap->pst.dstEnt = lSap->dstEnt;
   sap->pst.dstInst = lSap->dstInst;
   sap->pst.prior = lSap->dstPrior;
   sap->pst.route = lSap->dstRoute;
   sap->pst.selector = lSap->dstSel;
   /* With multicore support layer shall use the assigned region
      from SSI */
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
   sap->mem.region = veCb.init.region;
#else
   sap->mem.region = lSap->mem.region;
#endif
   sap->mem.pool = lSap->mem.pool;
   sap->maxBndRetry = lSap->maxBndRetry;
   sap->bndTmr.val = lSap->bndTmr.val;
   sap->bndTmr.enb = lSap->bndTmr.enb;
   sap->sapState = LVE_SAP_UNBOUND;
   cmInitTimers(&sap->timer , 1);

   RETVALUE(ROK);
} /* veLSapCfg */
#endif


/*
 *
 *       Fun:     VeMiLveCfgReq
 *
 *       Desc:    This primitive is called by LM to configure the 
 *                Framing Protocol User 
 *
 *       Ret:     ROK/RFAILED
 *
 *       Notes :  All configuration requests are handled from this
 *                function. 
 *
 *       File:    ve_mi.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeMiLveCfgReq
(
Pst                     *pst,
VeMngmt                 *cfg
)
#else
PUBLIC S16 VeMiLveCfgReq(pst, cfg)
Pst                     *pst;
VeMngmt                 *cfg;
#endif
{
   VeMngmt              cfmCfg;
   S16                  retVal;

   TRC3(VeMiLveCfgReq);


#ifdef VE_RELAY
   VE_DBG_ERROR((VE_PRNT_BUF,
            "VeMiLveCfgReq(pst, elmnt(%d))\n", cfg->hdr.elmId.elmnt));
   if (NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "VeMiLveCfgReq: NULL Management structure"));
      RETVALUE(RFAILED);
   }

   /* Check if general configuration is already done */
   if ( (!veCb.init.cfgDone) && (STVEGEN != cfg->hdr.elmId.elmnt) )
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
               "VeMiLveCfgReq: General configuration not done"));

      /* Send Config Confirm to LM with NOK status */
      cfmCfg.cfm.status = LCM_PRIM_NOK;
      cfmCfg.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      veGenerateCfm(pst, &cfg->hdr, TCFG, &cfmCfg);

      RETVALUE(RFAILED);
   }
#endif

#ifdef EU_DAT_APP
   VE_DBG_ERROR((VE_PRNT_BUF,
            "VeMiLveCfgReq(pst, elmnt(%d))\n", cfg->hdr.elmId.elmnt));
   if (NULLP == cfg)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "VeMiLveCfgReq: NULL Management structure"));
      RETVALUE(RFAILED);
   }

   /* Check if general configuration is already done */
   if ( (!veEuCb.init.cfgDone) && (STVEGEN != cfg->hdr.elmId.elmnt) )
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "VeMiLveCfgReq: General configuration not done"));

      /* Send Config Confirm to LM with NOK status */
      cfmCfg.cfm.status = LCM_PRIM_NOK;
      cfmCfg.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      veGenerateCfm(pst, &cfg->hdr, TCFG, &cfmCfg);

      RETVALUE(RFAILED);
   }
#endif

   retVal = ROK;
   switch (cfg->hdr.elmId.elmnt)
   {
      case STVEGEN:
         retVal = veGenCfg(&cfg->u.cfg.u.genCfg, &cfmCfg.cfm);
         break;
#ifdef LTE_HO_SUPPORT
      case STVENGH:
         retVal = veNghCfg(&cfg->u.cfg.u.NghCellCfgLst, &cfmCfg.cfm, pst);
         break;
#endif
#ifdef VE_RELAY
      case STVECTFSAP:
      case STVENHUSAP:
      case STVERGRSAP:
      case STVESZTSAP:
#ifdef LTE_HO_SUPPORT
      case STVECZTSAP:
#endif
         retVal = veLSapCfg(cfg, &cfmCfg.cfm);
         break;
#endif
#ifdef EU_DAT_APP
      case STVEEGTSAP:
      case STVEPJUSAP:
         retVal = veLSapCfg(cfg, &cfmCfg.cfm);
         break;
#endif

      default:
#ifdef VE_RELAY
         VE_DBG_ERROR((VE_PRNT_BUF, 
                  "VeMiLveCfgReq: Invalid Element"));
#endif

         /* Send Config Confirm to LM with NOK status */
         cfmCfg.cfm.status = LCM_PRIM_NOK;
         cfmCfg.cfm.reason = LCM_REASON_INVALID_ELMNT;
         retVal = RFAILED;
         break;
   }

   veGenerateCfm(pst, &cfg->hdr, TCFG, &cfmCfg);

   RETVALUE(retVal);
} /* VeMiLveCfgReq */


/*
 *
 *       Fun:     VeMiLveCntrlReq
 *
 *       Desc:    This primitive is called by LM to issue control 
 *                requests to the RRC User layer
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 *
 *       File:    ve_mi.c
 *
 */
#ifdef ANSI
PUBLIC S16 VeMiLveCntrlReq
(
Pst                     *pst,
VeMngmt                 *cntrl
)
#else
PUBLIC S16 VeMiLveCntrlReq(pst, cntrl)
Pst                     *pst;
VeMngmt                 *cntrl;
#endif
{
   VeMngmt              cntrlCfm;
   S16                  retVal = ROK;
#ifdef LTE_HO_SUPPORT
   CmLList *crtNode = NULLP;
   VeNghCellCb *nghCellCb = NULLP;
   CztCause cause;
#endif
   U32       peerId = 0;
   U16       idx;

   TRC3(VeMiLveCntrlReq);

   cmMemset((U8 *) &cntrlCfm, (U8) 0, (S16) sizeof(VeMngmt));
#ifdef VE_RELAY
   VE_DBG_ERROR((VE_PRNT_BUF,
            "VeMiLveCntrlReq(pst, elmnt(%d))\n", cntrl->hdr.elmId.elmnt));

   /* validate input parameters */
   if (NULLP == cntrl)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "VeMiLveCntrlReq: NULL Management structure"));

      RETVALUE(RFAILED);
   }

   /* Is genconfig done  */
   if (TRUE != veCb.init.cfgDone) 
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
               "VeMiLveCntrlReq: General configuration not done"));
      cntrlCfm.cfm.status = LCM_PRIM_NOK;
      cntrlCfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cntrlCfm.u.cntrl.action = cntrl->u.cntrl.action;
      cntrlCfm.u.cntrl.subAction = cntrl->u.cntrl.subAction;

      veGenerateCfm (pst, &cntrl->hdr, TCNTRL, &cntrlCfm);
      RETVALUE(RFAILED);
   }

   cmMemcpy((U8 *)&(veCb.lmHdr), (U8 *)(&cntrl->hdr), sizeof(Header));

#endif

#ifdef EU_DAT_APP
   VE_DBG_ERROR((VE_PRNT_BUF, 
            "VeMiLveCntrlReq(pst, elmnt(%d))\n", cntrl->hdr.elmId.elmnt));

   /* validate input parameters */
   if (NULLP == cntrl)
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
               "VeMiLveCntrlReq: NULL Management structure"));

      RETVALUE(RFAILED);
   }

   /* Is genconfig done  */
   if (TRUE != veEuCb.init.cfgDone) 
   {
      VE_DBG_ERROR((VE_PRNT_BUF, 
               "VeMiLveCntrlReq: General configuration not done"));
      cntrlCfm.cfm.status = LCM_PRIM_NOK;
      cntrlCfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cntrlCfm.u.cntrl.action = cntrl->u.cntrl.action;
      cntrlCfm.u.cntrl.subAction = cntrl->u.cntrl.subAction;

      veGenerateCfm (pst, &cntrl->hdr, TCNTRL, &cntrlCfm);
      RETVALUE(RFAILED);
   }

   cmMemcpy((U8 *)&(veEuCb.lmHdr), (U8 *)(&cntrl->hdr), sizeof(Header));
#endif

   cntrlCfm.cfm.status = LCM_PRIM_OK;
   cntrlCfm.cfm.reason = LCM_REASON_NOT_APPL;

   /* Identify the element for control */
   switch (cntrl->hdr.elmId.elmnt)
   { 
      case STVEGEN: 
         switch (cntrl->u.cntrl.action)
         {
            case ASHUTDOWN:
               veShutdown();
               break;
            case AENA:
               switch(cntrl->u.cntrl.subAction)
               {
                  case SAUSTA:  /* Sub Action Unsolicited Status Alarm */
#ifdef VE_RELAY
                     veCb.init.usta = TRUE;
#endif
#ifdef EU_DAT_APP
                     veEuCb.init.usta = TRUE;
#endif 
                     break;

/* Updating log file informations */
#ifdef DEBUGP
                  case SADBG:   /* Sub Action DEBUG */
                     veCb.init.dbgMask = cntrl->u.cntrl.u.dbgMask;
#if (VE_SM_LOG_TO_FILE && DEBUGP)
                     cmMemcpy((U8*)veCb.filePath,(U8*)cntrl->u.cntrl.u.dbg.filePath,
                           LVE_MAX_FILE_PATH);
                     veCb.cfgDbgLines = cntrl->u.cntrl.u.dbg.nmbDbgLines;
#endif /* end of (VE_SM_LOG_TO_FILE && DEBUGP) */
                     break;

                  case ADISIMM:        /* Action DISABLE */
                     veCb.init.dbgMask &= ~(cntrl->u.cntrl.u.dbgMask);
#if (VE_SM_LOG_TO_FILE && DEBUGP)
                     veCb.cfgDbgLines = 0;
                     cmMemset((U8*)veCb.filePath, 0,LVE_MAX_FILE_PATH);
                     if(veCb.dbgFp)
                     {
                        fclose(veCb.dbgFp);
                        veCb.dbgFp = NULLP;
                     }
#endif /* (VE_SM_LOG_TO_FILE && DEBUGP) */
                     break;
#endif /* DEBUGP */

                  default:
                     break;
               }
               break;
         }
         break;

#ifdef VE_RELAY
      case STVERGRSAP:
      case STVECTFSAP:
      case STVENHUSAP: 
      case STVESZTSAP:
#ifdef LTE_HO_SUPPORT
      case STVECZTSAP:
#endif 
         retVal = veLSapCntrl(&cntrl->u.cntrl, &cntrlCfm.cfm,
               cntrl->hdr.elmId.elmnt);
         break;
#endif
#ifdef EU_DAT_APP
      case STVEEGTSAP:
      case STVEPJUSAP:
         retVal = veLSapCntrl(&(cntrl->u.cntrl), &(cntrlCfm.cfm),cntrl->hdr.elmId.elmnt);
         break;
#endif
#ifdef VE_RELAY
      case STVECELLCFG:
         /* DONT DELETE - CODE REQD FOR END TO END TESTING 
          * COMMENTED FOR UNIT TESTING */

#if !(defined(VE_PERF_MEAS) || defined(VE_WITHOUT_CNE))
         /* if(veCb.mmeCb->s1Setup == VE_S1_SETUP_DONE) */
   
       for(idx = 0; idx < VE_MAX_MMES; idx++)
       {
         if(veCb.mmeCont.mmes[idx]->s1Setup == VE_S1_SETUP_DONE)
         {
            retVal = veSndNhuCellCfgReq(veCb.cellCb[0]->cellId);
            break;
         }
         else
         {
            cntrlCfm.cfm.status = LCM_PRIM_NOK;
            cntrlCfm.cfm.reason = LCM_REASON_NOT_APPL;

            VE_DBG_ERROR((VE_PRNT_BUF, "VeMiLveCntrlReq: S1 Setup Not Done Yet!"));
         }
       }
#else
         retVal = veSndNhuCellCfgReq(veCb.cellCb[0]->cellId);
#endif
         break;
#ifndef LTE_ENB_PERF
      case STVES1CONSETUP:
         /* Triggering S1 Setup Request  */
         for(idx = 0; idx < cntrl->u.cntrl.u.s1Setup.numPeers; idx++)
         {
            peerId = cntrl->u.cntrl.u.s1Setup.peerId[idx];
            retVal = veSztS1SetReq(0, peerId);
         }
         break;

#ifdef LTE_HO_SUPPORT
      case STVEX2CONSETUP:
         /* Triggering X2 Setup Request by traversing through the list  */
         crtNode = veCb.cellCb[0]->nghCellLst.first;
         while(NULLP != crtNode)
         {
            nghCellCb = (VeNghCellCb *)crtNode->node;
            if((VE_VAL_ZERO != nghCellCb->peerId) && (TRUE == nghCellCb->trgrX2))
            {
               retVal = veCztX2SetupReq(nghCellCb->peerId);
               if ( retVal == ROK)
               {
                  veStartTmr ((PTR)nghCellCb, VE_TMR_X2_SETUP_TMR, VE_X2_SETUP_TMR_VAL);
               }
            }

            crtNode = crtNode->next;
         }
         break;
#ifdef VE_TEST_CODE
#ifdef VE_TEST_ERR_IND
         case STVEX2GPERRIND: /* X2_CFG_UPD_CHANGES */
         /* Trigger X2 ErrorIndication */
         crtNode = veCb.cellCb[0]->nghCellLst.first;
         nghCellCb = (VeNghCellCb *)crtNode->node;
         if(VE_VAL_ZERO != nghCellCb->peerId)
         {
           retVal = veCztErrInd(nghCellCb->peerId,0,0, FALSE, 0, 0, NULLP);

           /*retVal = veCztErrInd(nghCellCb->peerId,(U32)1,(U32)2, TRUE, CAUSE_RADIONW,  CztCauseRadioNwhandover_desirable_for_radio_reasonsEnum, NULLP);*/
         }
         if(retVal == ROK)
         {
           printf("\n******* Triggering X2AP Error Indication **************\n");
         }
         RETVALUE(ROK);
         break;
#endif /*VE_TEST_ERR_IND */
#endif /*VE_TEST_CODE */
         case STVEX2CFGUPDREQ:  /* X2_CFG_UPD_CHANGES*/
         /* Trigger X2 Configuration Update Request. */
         crtNode = veCb.cellCb[0]->nghCellLst.first;
         while(NULLP != crtNode) 
         {
           nghCellCb = (VeNghCellCb *)crtNode->node;
           if(VE_VAL_ZERO != nghCellCb->peerId)
           {
#ifdef VE_TEST_CODE
#ifdef VE_TEST_CFG_UPD
             retVal = veCztCfgUpdReq(nghCellCb->peerId, &(cntrl->u.cntrl.u.cfgUpdInfo));
#endif
#else
             retVal = veCztCfgUpdReq(nghCellCb->peerId, NULLP); /* TODO */
#endif
             if ( retVal == ROK)
             {
                 printf ("\n *******************CALLING CFG UPD REQ Bld - Starting tmr ***********************\n"); 
                 veStartTmr ((PTR)nghCellCb, VE_TMR_X2_CFGUPD_TMR, VE_X2_CFGUPD_TMR_VAL); 

             }
           }
           crtNode = crtNode->next; 
           
         }
#ifdef VE_TEST_CODE
#ifdef VE_TEST_CFG_UPD
       RETVALUE(ROK);
#endif /* VE_TEST_CFG_UPD */   
#endif /*VE_TEST_CODE*/
       break;         
      case STVEX2RESET:
         /* Triggering X2 Reset Request */
         retVal = RFAILED;
         crtNode = veCb.cellCb[0]->nghCellLst.first;
         while(NULLP != crtNode)
         {
            nghCellCb = (VeNghCellCb *)crtNode->node;
            if((nghCellCb->peerId == cntrl->u.cntrl.u.x2Reset.peerId))
            {
               /* Clear all the on going procedure, before sending Reset Request */
               retVal = veAbortX2OngoingProc(nghCellCb->peerId);
               if ( retVal != ROK )
               {
                 break;
               }
               cause.choice.pres = PRSNT_NODEF;
               cause.choice.val = cntrl->u.cntrl.u.x2Reset.causeType; 
               cause.val.misc.pres =  PRSNT_NODEF;
               cause.val.misc.val =  cntrl->u.cntrl.u.x2Reset.causeVal;
               /* Fill the timerCb */
               nghCellCb->x2ResetCb.causeType = cntrl->u.cntrl.u.x2Reset.causeType;
               nghCellCb->x2ResetCb.causeVal = cntrl->u.cntrl.u.x2Reset.causeVal;

               retVal = veSndX2ResetRqst(&cause, nghCellCb->peerId);
               if ( retVal != ROK )
               {
                 break;
               }
               veStartTmr ((PTR)nghCellCb, VE_TMR_X2_RESET_TMR, VE_X2_RESET_TMR_VAL);
#ifdef VE_TEST_CODE
               RETVALUE(ROK);
#endif
               break;
            }
            crtNode = crtNode->next;
         }
         break;
#endif /* End of LTE_HO_SUPPORT */
      case STVESNDS1RESET:
         veResetCntrl(&cntrl->u.cntrl.u.s1Reset);
         break;

#endif
#endif
      default:
#ifdef VE_RELAY
         VE_DBG_ERROR((VE_PRNT_BUF, 
                  "VeMiLveCntrlReq: Invalid Element"));
#endif
#ifdef EU_DAT_APP
         VE_DBG_ERROR((VE_PRNT_BUF, 
                  "VeMiLveCntrlReq: Invalid Element"));
#endif
         retVal = RFAILED;
         break;
   }

   if(retVal == RFAILED)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"VeMiLveCntrlReq: Action Failed \n"));
#endif
      cntrlCfm.cfm.status = LCM_PRIM_NOK;
      cntrlCfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
   }
   cntrlCfm.u.cntrl.action = cntrl->u.cntrl.action;
   cntrlCfm.u.cntrl.subAction = (U8)cntrl->u.cntrl.subAction;

   veGenerateCfm (pst, &cntrl->hdr, TCNTRL, &cntrlCfm);

   RETVALUE(retVal);
} /* VeMiLveCntrlReq */


/*
 *
 *       Fun:     veResetCntrl
 *
 *       Desc:    Build and send the reset request message
 *
 *       Ret:     ROK/RFAILED
 *
 *       File:    ve_mi.c
 *
 */
#ifdef ANSI
PUBLIC S16 veResetCntrl
(
VeS1Reset    *veReset
)
#else
PUBLIC S16 veResetCntrl(veReset)
VeS1Reset    *veReset;
#endif
{
   VeS1ConId    *conId = NULLP;
   U16           idx;

   if(veReset->nmbUes > 0)
   {
      VE_ALLOC(&conId, veReset->nmbUes);

      if(conId == NULLP)
      {
         RETVALUE(RFAILED);
      }

      for(idx = 0; idx < veReset->nmbUes; idx++)
      {
         conId[idx].conId = veReset->conIds[idx];
         conId[idx].type = VE_S1AP_ENB_ID;
      }
   }

   veHdlEnbReset(veReset->peerId, CAUSE_RADIONW, veReset->cause,
                 veReset->nmbUes, veReset->type, conId);

   RETVALUE(ROK);
}

#ifdef LTE_HO_SUPPORT
/*
 *
 *       Fun:     veNghCfg
 *
 *       Desc:    This primitive is called by LM to configure the 
 *                Neighbor information in APPL. 
 *
 *       Ret:     ROK/RFAILED
 *
 *       File:    ve_mi.c
 *
 */
#ifdef ANSI
PUBLIC S16 veNghCfg
(
VeNghCellCfgLst         *nghCfgLst,
CmStatus                *status,
Pst                     *pst
)
#else
PUBLIC S16 veNghCfg(nghCfgLst, status, pst)
VeNghCellCfgLst         *nghCfgLst;
CmStatus                *status;
Pst                     *pst;
#endif
{
   VeNghCellCb          *nghCfg = NULLP;
   VeCellCb             *cellCb = NULLP;
   U16                  idx = 0;

   TRC3(veNghCfg);
   
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;


#ifdef VE_RELAY
   if (TRUE != veCb.init.cfgDone)
   {
#ifdef VE_DBG_ERROR
      VE_DBG_ERROR((VE_PRNT_BUF,"veNghCfg : %d :\
               GenCfg not done\n", __LINE__));
#endif
      RETVALUE(RFAILED);
   }

   cellCb = veCb.cellCb[0];
   for (idx = 0; idx < nghCfgLst->noOfCfg; idx++)
   {
      VE_ALLOC(&nghCfg, sizeof(VeNghCellCb));
      if(NULLP == nghCfg)
      {
#ifdef VE_DBG_ERROR
         VE_DBG_ERROR((VE_PRNT_BUF,"veNghCfg : %d :\
                  Alloc for nghCfg failed\n", __LINE__));
#endif
         RETVALUE(RFAILED);
      }
      nghCfg->enbType          = nghCfgLst->NghCellCfg[idx]->enbType;
      nghCfg->veCellPrp.physCellId    = nghCfgLst->NghCellCfg[idx]->physCellId;
      nghCfg->veCellPrp.tac    = nghCfgLst->NghCellCfg[idx]->tac;
      nghCfg->veCellPrp.cellId = nghCfgLst->NghCellCfg[idx]->cellId;
      nghCfg->plmnId            = nghCfgLst->NghCellCfg[idx]->plmnId;
      nghCfg->peerId            = nghCfgLst->NghCellCfg[idx]->peerId;
      nghCfg->trgrX2            = nghCfgLst->NghCellCfg[idx]->trgrX2;
      nghCfg->tEnbAddr.type = CM_TPTADDR_IPV4;
      /* port number is assigned to zero as it doesn't play any role when 
         the communication is happening through CCPU  E-GTP */
      nghCfg->tEnbAddr.u.ipv4TptAddr.port = VE_VAL_ZERO;
      nghCfg->tEnbAddr.u.ipv4TptAddr.address = nghCfgLst->NghCellCfg[idx]->enbAddr;

      nghCfg->lnk.node = (PTR)nghCfg;

      /* Initilize the X2-Setup Rtx Timer */
      cmInitTimers(&(nghCfg->x2SetUpTmr), 1);
      nghCfg->maxSetupRetry = 0; 

      cmInitTimers(&(nghCfg->x2ResetTmr), 1);
      nghCfg->x2ResetCb.maxResetRetry = 0;
     
      /* X2_CFG_UPD_CHANGES */
      /* Initilize the X2-Setup Rtx Timer */
      cmInitTimers(&(nghCfg->cfgUpdTmr), 1);
      nghCfg->maxCfgupdRetry = 0;

      cmLListAdd2Tail(&cellCb->nghCellLst, &nghCfg->lnk);
   }/* End of for */
   /* Freeing the configuration pointers */
   if (0 != nghCfgLst->noOfCfg)
   {
      SPutSBuf(pst->region, pst->pool, (Ptr)(nghCfgLst->NghCellCfg),
         sizeof(VeNghCellCfg) * nghCfgLst->noOfCfg);
   }

#endif
   RETVALUE(ROK);
} /* veNghCfg */


#endif /* End of LTE_HO_SUPPORT */


/********************************************************************30**
  
         End of file:     ve_mi.c@@/main/2 - Wed Dec  1 08:44:26 2010
  
*********************************************************************31*/

/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/

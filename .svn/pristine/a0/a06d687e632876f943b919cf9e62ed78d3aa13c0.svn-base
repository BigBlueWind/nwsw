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

        Name:    LTE - PDCP - Layer Manager file

        Type:    C source file

        Desc:    It contains the following common  functions for processing
                 the all the external interfaces.
                    -- PjMiLpjCfgReq
                    -- PjMiLpjCfgCfm 
                    -- PjMiLpjCntrlReq
                    -- PjMiLpjCntrlCfm
                    -- PjMiLpjStaReq
                    -- PjMiLpjStaCfm
                    -- PjMiLpjStaInd
                    -- PjMiLpjStsReq
                    -- PjMiLpjStsCfm
                    -- PjMiLpjTrcInd 

        File:     gp_lmm.c

        Sid:      gp_pj_lmm.c@@/main/1 - Fri Nov 13 14:13:47 2009

        Prg:     cp 

*********************************************************************21*/
/** @file kw_pj_lmm.c
@brief PDCP Layer Manager Module
**/


/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lkw.h"           /* LKW defines */
#include "lpj.h"           /* LPJ defines */
#include "ckw.h"           /* CKW defines */
#include "cpj.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* PDCP environment options */
#include "kw.h"            /* PDCP defines */

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lkw.x"           /* LKW */
#include "lpj.x"           /* LPJ */
#include "ckw.x"           /* CKW */
#include "cpj.x"           /* CPJ */
#include "pju.x"           /* PJU */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#ifdef PWAV_HC
#include "basic_types.h"
#include "pst_events.h"
#include "cmn_larm_uarm.h"
#include "version.x"
#include "log.h"
#endif

/*********************************************************************
 *             Forward Declaration of LKW Porting Functions
 ********************************************************************/
EXTERN S16 kwActvTmr ARGS ((Void));

PRIVATE S16 pjLmmSendCfm ARGS ((Pst *pst,PjMngmt *cfm,U8 type,Header *hdr));
PRIVATE S16 pjLmmGenCfg ARGS ((PjGenCfg *cfg));
PRIVATE S16 pjLmmPjuSapCfg ARGS ((PjSapCfg *cfg));
PRIVATE S16 pjLmmCpjSapCfg ARGS ((PjSapCfg   *cfg));
PRIVATE S16 pjLmmGenCntrl ARGS ((PjMngmt *cntrl));
PRIVATE S16 pjLmmFormLmCfm ARGS ((Pst *pst, Header *hdr, Reason reason));
PRIVATE S16 pjLmmGetPjuSapSta ARGS ((PjPjuSapSta *sta));
PRIVATE S16 pjLmmGetCpjSapSta ARGS ((PjCpjSapSta *sta));
PRIVATE S16 pjLmmGetGenSts ARGS ((PjGenSts *sts,Action action));
PRIVATE S16 pjLmmGetSapSts ARGS ((PjMngmt *sts,Elmnt elmnt,Action action));


/*********************************************************************
 *             Primitives for LPJ interface 
 ********************************************************************/
/**
 * @brief
      Stores the general PDCP configuration. It calculates the
      memory requirement and reserves it. It also initializes the
      control blocks, the hash lists and registers the timers. In
      case of error it rolls back and reports error in the confirm.
      If the general configuration is already done it is treated as
      reconfiguration, but right now reconfiguration is not supported.
 
 *  @param[in] cfm         PDCP LM general config structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL 
 *      -# Failure : LCM_REASON_MEM_NOAVAIL
 *                   LCM_REASON_REGTMR_FAIL
 *                   LCM_REASON_INVALID_PAR_VAL
 *                   LCM_REASON_HASHING_FAILED
 */
#ifdef ANSI
PRIVATE S16 pjLmmGenCfg
(
PjGenCfg    *cfg
)
#else
PRIVATE S16 pjLmmGenCfg(cfg)
PjGenCfg    *cfg;
#endif
{
   /* kw006.201, ccpu00120574, fixed the warning */

   TRC2(pjLmmGenCfg)

   /* kw006.201, ccpu00120574, fixed the warning */

   if(kwCb.init.cfgDone & PJ_LMM_GENCFG_DONE)
   {
      /* reconfiguration is supported only for obdDiscTmrInt */
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
      kwCb.pjGenCfg.obdTmrInt = cfg->obdTmrInt;
      kwCb.pjGenCfg.obdWtTmrInt = cfg->obdWtTmrInt;
#endif /* PJ_SEC_ASYNC || PJ_CMP_ASYNC */
      RETVALUE(LCM_REASON_NOT_APPL);
   }

   KW_MEM_SET(&(kwCb.pjGenCfg), 0, sizeof(PjGenCfg));
   KW_MEM_CPY(&(kwCb.pjGenCfg), cfg, sizeof(PjGenCfg));

   /* Initialize the PJU SAP control blocks */
   KW_MEM_SET(&(kwCb.pjuSap[PJ_SRB_SAP]), 0, sizeof(KwPjuSapCb));
   KW_MEM_SET(&(kwCb.pjuSap[PJ_DRB_SAP]), 0, sizeof(KwPjuSapCb));

   /* Initialize CPJ SAP control block */
   KW_MEM_SET(&(kwCb.cpjSap), 0, sizeof(KwCpjSapCb));

   KW_MEM_CPY(&(kwCb.pjInit.lmPst), &cfg->lmPst,
                   sizeof(Pst));

   kwCb.pjInit.lmPst.srcProcId = kwCb.init.procId;
   kwCb.pjInit.lmPst.srcEnt = kwCb.pjInit.ent;
   kwCb.pjInit.lmPst.srcInst = kwCb.init.inst;
   kwCb.pjInit.lmPst.event = EVTNONE;

   /* kw002.201 For multi core and multi region, no need to reinitiailize the region again .
      This would be done with pjActvInit from SSI */
#if !defined(SS_MULTICORE_SUPPORT) && !defined(SS_M_PROTO_REGION)
   kwCb.pjInit.region = cfg->lmPst.region;
#endif
   kwCb.pjInit.pool   = cfg->lmPst.pool;

   kwCb.init.cfgDone |= PJ_LMM_GENCFG_DONE;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmGenCfg */

/**
 *
 * @brief
 *     Sends confirmation message to LM Called by Request function
 *  @param[in] pst         Post structure
 *  @param[in] cfm         PDCP LM structure 
 *  @param[in] type        type of message
 *  @param[in] hdr         message header
 *  @return  S16
 *      -# ROK
 */
#ifdef ANSI
PRIVATE S16 pjLmmSendCfm
(
Pst *pst,                    /* Post structure */
PjMngmt *cfm,                /* PDCP LM structure */
U8 type,                     /* type of message */
Header *hdr                  /* message header */
)
#else
PRIVATE S16 pjLmmSendCfm(pst, cfm, type, hdr)
Pst *pst;                    /* Post structure */
PjMngmt *cfm;                /* PDCP LM structure */
U8 type;                     /* type of message */
Header *hdr;                 /* message header */
#endif
{
   Pst rPst;                 /* Reply post structure */

   TRC2(pjLmmSendCfm);

   KW_MEM_SET(&rPst, 0, sizeof(Pst));   
 
   /* reply post structure for confirmation */
   rPst.srcEnt = kwCb.pjInit.ent;
   /*rPst.srcInst = kwCb.init.inst;*/ /*Rahul:Bug fixed*/
   rPst.srcInst = kwCb.pjInit.inst;
   rPst.srcProcId = SFndProcId();
   rPst.dstEnt = pst->srcEnt;
   rPst.dstInst = pst->srcInst;
   rPst.dstProcId = pst->srcProcId;
   rPst.selector = hdr->response.selector;
   rPst.prior = hdr->response.prior;
   rPst.route = hdr->response.route;
   rPst.region = hdr->response.mem.region;
   rPst.pool= hdr->response.mem.pool;

   /* Fill the reply header */
   KW_MEM_SET (&cfm->hdr, '\0', sizeof(cfm->hdr));
   cfm->hdr.elmId.elmnt = hdr->elmId.elmnt;
   cfm->hdr.entId.ent = kwCb.pjInit.ent;
   /*cfm->hdr.entId.inst = kwCb.init.inst;*/
   cfm->hdr.entId.inst = kwCb.pjInit.inst; /*Rahul:Bug fixed*/
   cfm->hdr.transId = hdr->transId;

   switch(type)
   {
      case TCFG:
         PjMiLpjCfgCfm(&rPst,cfm);
         break;
      case TCNTRL:
         SGetDateTime(&(cfm->t.cntrl.dt));
         PjMiLpjCntrlCfm(&rPst,cfm);
         break;
      case TSTS:
         SGetDateTime(&(cfm->t.sts.dt));
         PjMiLpjStsCfm(&rPst,0,cfm);
         break;
      case TSSTA:
         SGetDateTime(&(cfm->t.ssta.dt));
         PjMiLpjStaCfm(&rPst,cfm);
         break;
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         KWLOGERROR(ERRCLS_INT_PAR, EKW078, (ErrVal) type,
               "Invalid message type");
          
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         break;
   }

   RETVALUE(ROK);
} /* pjLmmSendCfm */


/**
@brief
This function processes configuration requests received from the layer manager.
The configuration requests can be for general configuration, or configuration
of PJU, and CPJ SAPs. The type of the configuration request can be determined
from the elmId field in the header of the layer manager message.This function
is called by the layer manager to configure PDCP. 

This function implements the following logic: \n

- switch (cfg->hdr.elmId.elmnt)
   - case STGEN 
      - update the genCfg field in RlCb; 
      - allocate the maximum static memory required by the PDCP product; 
      - register the timer service by calling SReqTmr; 
      - set CfgDone for not configuring again; 
   - case STPJUSAP 
      - if genCfg is not done, send negative Cfm to the layer manager; 
      - if all the parameters are valid, then allocate the RlSap control block;
      - update the SAP control block with the information provided in the 
      - configuration request; 
      - send configuration confirm with the status, success; 
   - case STCPJSAP 
      - if genCfg is not done, send negative Cfm to the layer manager; 
      - if all the parametes are valid, then allocate the CrSap control block; 
      - update the SAP control block with the information provided in the
         configuration request; 
      - send configuration confirm with the status, success; 
- end switch;
 
 *  @param[in] pst      post structure 
 *  @param[in] cfg      LM management structure 
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/

#ifdef ANSI
PUBLIC S16 PjMiLpjCfgReq 
(
Pst *pst, 
PjMngmt *cfg
)
#else
PUBLIC S16 PjMiLpjCfgReq (pst, cfg)
Pst  *pst;
PjMngmt *cfg;
#endif
{
   S16   ret;
   Elmnt elmnt;          /* header element */
   PjMngmt rCfm;         /* Reply cfm */
   Reason reason;        /* failure reason */

   TRC3(PjMiLpjCfgReq);

   logInfo( "PjMiLpjCfgReq(post, elmId(%d))\n", cfg->hdr.elmId.elmnt);

   ret = ROK;

   /* check configuration is done or not */
   if ((!(kwCb.init.cfgDone & PJ_LMM_GENCFG_DONE)) &&
         (cfg->hdr.elmId.elmnt != STGEN))
   {
      /*
       * if general config is not over then use pst structure
       * in primitive to communicate to stack manager
       */

      logInfo("%s", "Gen cfg is not done\n");

      rCfm.cfm.status = LCM_PRIM_NOK;
      rCfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      pjLmmSendCfm(pst, &rCfm, TCFG, &cfg->hdr);     
      RETVALUE(RFAILED);
   }

   elmnt = cfg->hdr.elmId.elmnt;

   switch(elmnt)
   {
      case STGEN:
      {
         reason = pjLmmGenCfg(&cfg->t.cfg.s.gen);
         break;
      }
      case STPJUSAP:
      {
         reason = pjLmmPjuSapCfg(&cfg->t.cfg.s.sap);
         break;
      }
      case STCPJSAP:
      {
         reason = pjLmmCpjSapCfg(&cfg->t.cfg.s.sap);
         break;
      }
      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }   
   }

   if (reason == LCM_REASON_NOT_APPL)
   {
      rCfm.cfm.status = LCM_PRIM_OK;
      rCfm.cfm.reason = LCM_REASON_NOT_APPL;

      ret = ROK;
   }
   else
   {
      rCfm.cfm.status = LCM_PRIM_NOK;
      rCfm.cfm.reason = reason;

      ret = RFAILED;
   }

   logInfo("PjMiLpjCfgReq - cfm: status(%d) reason(%d)\n",
            rCfm.cfm.status, rCfm.cfm.reason);

   ret = pjLmmSendCfm(pst, &rCfm, TCFG, &cfg->hdr);

   RETVALUE(ret);
} /* PjMiLpjCfgReq */

/**
@brief 
The layer manager initiates and uses the management-control procedure to
control PDCP elements.The PDCP control request primitive (PjMiLpjCntrlReq)
can be called more than once and at any time after the management-configuration
procedure.The control request primitive is confirmed by a PjMiLpjCntrlCfm
primitive.

This function implements the following logic: \n

- if(cfgDone = FALSE) 
   - send negative confirmation to the layer manager; 
   - return; 
- else 
- switch(cntrl->hdr.elmId.elmnt) 
   -  case STGEN 
        - switch (action) 
        - case AENA 
             - switch(subAction) 
             - case SAUSTA 
                  - enable the unsolicited status flag; 
                  - send the control Cfm with success; 
             - case SADBG 
                  - set the debug mask; 
                  - send the control Cfm with success; 
             - case SATRC 
                  - enable the trace flag; 
                  - send the control Cfm with success; 
        - case ADISIMM 
             - switch(subAction) 
             - case SAUSTA 
                  - disable the unsolicited status flag; 
                  - send the control Cfm with success; 
             - case SATRC 
                  - disable the trace flag; 
                  - send the control Cfm with success; 
        - case ASHUTDOWN 
   - case STMKSAP 
        - switch (action) 
        - case ABND 
        - case AUBND 
   - default  
- end else  
 *  @param[in] pst      post structure
 *  @param[in] cntrl    LM management structure
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 PjMiLpjCntrlReq
(
Pst *pst, 
PjMngmt *cntrl
)
#else
PUBLIC S16 PjMiLpjCntrlReq(pst, cntrl)
Pst     *pst;
PjMngmt *cntrl;
#endif
{
   Elmnt elmnt;          /* header element */
   PjMngmt rCfm;         /* Reply cfm */
   Reason reason;        /* failure reason */

   S16 ret;

   TRC3(PjMiLpjCntrlReq)

   logInfo("PjMiLpjCntrlReq(post, elmId(%d))\n", cntrl->hdr.elmId.elmnt);

   ret = ROK;

   if (!(kwCb.init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      rCfm.cfm.status = LCM_PRIM_NOK;
      rCfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;

      pjLmmSendCfm(pst, &rCfm, TCNTRL, &cntrl->hdr);
      RETVALUE(RFAILED);
   }

   /* In normal cases, LCM_REASON_NOT_APPL is returned in cfm.
    * In all error cases appropriate reason is returned
    * by the functions below
    */

   elmnt = cntrl->hdr.elmId.elmnt;

   reason = LCM_REASON_NOT_APPL;


   switch (elmnt)
   {
      case STGEN:
      {
         /* general control */
         reason = pjLmmGenCntrl(cntrl);
         break;
      }

      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }
   } /* end switch */

   logInfo("PjMiLpjCntrlReq - cfm: reason(%d)\n", reason);

   ret = pjLmmFormLmCfm(pst, &cntrl->hdr, reason);
   RETVALUE(ret);
} /* PjMiLpjCntrlReq */
/**
@brief 
This function processes solicited status requests received from the layer
manager. The layer manager can request status information regarding the
system ID, PJUSAP, or CPJSAP.

 *  @param[in] pst      post structure
 *  @param[in] cfm    LM management structure
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 PjMiLpjStaReq
(
Pst *pst, 
PjMngmt *sta
)
#else
PUBLIC S16 PjMiLpjStaReq(pst, sta)
Pst     *pst;
PjMngmt *sta;
#endif
{
   S16      ret;
   Elmnt    elmnt;            /* Header element */
   PjMngmt  rSta;             /* Status */
   Reason   reason;           /* Failure reason */



   logInfo("PjMiLpjStaReq(post, elmId(%d))\n", sta->hdr.elmId.elmnt);

   KW_MEM_SET(&rSta, 0, sizeof(PjMngmt));

   reason = LCM_REASON_NOT_APPL;

   elmnt = sta->hdr.elmId.elmnt;

   switch (elmnt)
   {
      case STSID:
      {
         reason = LCM_REASON_NOT_APPL;
         kwGetSId(&rSta.t.ssta.s.sysId);
         break;
      }
      case STGEN:
      {
         break;
      }
      case STPJUSAP:
      {
         KW_MEM_CPY (&rSta.t.ssta.s.pjuSap, &sta->t.ssta.s.pjuSap,
               sizeof (PjPjuSapSta));
         reason = pjLmmGetPjuSapSta (&rSta.t.ssta.s.pjuSap);
         break;
      }
      case STCPJSAP:
      {
         KW_MEM_CPY (&rSta.t.ssta.s.cpjSap, &sta->t.ssta.s.cpjSap,
               sizeof (PjCpjSapSta));
         reason = pjLmmGetCpjSapSta (&rSta.t.ssta.s.cpjSap);
         break;
      }
      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }
   } /* end of switch */

   if (reason == LCM_REASON_NOT_APPL)
   {
      rSta.cfm.status = LCM_PRIM_OK;
      rSta.cfm.reason = LCM_REASON_NOT_APPL;

      ret = ROK;
   }
   else
   {
      rSta.cfm.status = LCM_PRIM_NOK;
      rSta.cfm.reason = reason;

      ret = RFAILED;
   }

   logInfo("PjMiLpjStaReq - cfm: status(%d) reason(%d)\n",
            rSta.cfm.status, rSta.cfm.reason);

   ret = pjLmmSendCfm(pst, &rSta, TSSTA, &sta->hdr);

   RETVALUE(ret);
} /* PjMiLpjStaReq */
#if 0
/**
@brief 
This function processes statistics requests received from the layer manager.
After collecting the statistics, this function calls the statistics confirm
function to send the statistics to the layer manager.

- switch(sts->hdr.elmId.elmnt) 
  - case STGEN 
     - get the general statistics from the KW control block; 
     - if (action = RESET) 
        - reset the general statistic field in the RlCb; 
        - call PjMiLpjStsCfm to send statistics back to layer manager; 
  - case STCPJSAP 
  - case STPJUSAP 
      - get the SAP specific statistics from KW control block; 
      - if (action = RESET) 
      - reset the general statistic field in the RlCb; 
      - call PjMiLpjStsCfm to send statistics to the layer manager; 
-end switch;

 *  @param[in] pst      post structure
 *  @param[in] action   action 
 *  @param[in] cfg      LM management structure
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 PjMiLpjStsReq 
(
Pst *pst, 
Action action, 
PjMngmt *sts
)
#else
PUBLIC S16 PjMiLpjStsReq (pst, action, sts)
Pst     *pst;
Action  action;
PjMngmt *sts;
#endif
{
   S16      ret;
   PjMngmt  rSts;             /* Statistics */
   Reason   reason;           /* Reason for failure */
   Elmnt    elmnt;            /* Element Id */

   TRC3(PjMiLpjStsReq);

   PJDBGP((KW_PJ_DBGMASK_LMM | KW_DBGMASK_BRIEF), (_kwPBuf,
            "PjMiLpjStsReq(post, elmId(%d),action(%d))\n",
            action, sts->hdr.elmId.elmnt));

   KW_MEM_SET(&rSts, 0, sizeof(PjMngmt));
   elmnt = sts->hdr.elmId.elmnt;

   switch (elmnt)
   {
      case STGEN:
      {
         reason = pjLmmGetGenSts(&rSts.t.sts.s.gen, action);
         break;
      }
      case STPJUSAP:
      {
         reason = pjLmmGetSapSts(sts, elmnt, action);
         KW_MEM_CPY(&rSts.t.sts.s.pjuSap, &sts->t.sts.s.pjuSap, 
                    sizeof(PjPjuSts));
         break;
      }
      case STCPJSAP: /* No CPJ SAP statistics available */
      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }
   } /* end switch */

   if (reason == LCM_REASON_NOT_APPL)
   {
      rSts.cfm.status = LCM_PRIM_OK;
      rSts.cfm.reason = LCM_REASON_NOT_APPL;

      ret = ROK;
   }
   else
   {
      rSts.cfm.status = LCM_PRIM_NOK;
      rSts.cfm.reason = reason;

      ret = RFAILED;
   }

   PJDBGP((KW_PJ_DBGMASK_LMM | KW_DBGMASK_DETAIL), (_kwPBuf,
            "PjMiLpjStsReq - cfm: status(%d) reason(%d)\n",
            rSts.cfm.status, rSts.cfm.reason));

   ret = pjLmmSendCfm(pst, &rSts, TSTS, &sts->hdr);

   RETVALUE(ret);
} /* PjMiLpjStsReq */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
/**
@brief 
This function processes L2 Measuremnet requests received from the layer manager.
The L2 Measurement is start for a time period and after the timer expiry, the 
measurement confirm is sent.

-  If more than one measurement is received, 
   -  verify that they are a valid pair of measurements
-  Allocate and initialise PjL2MeasEvtCb.
-  Validate if the measurement is already in progress.
   -  Loop through the existing measEvtCb with matching measType
      -  Check if measurement is running for any qci present in the measReq
      -  If present, move the qCi to list of invalid qCIs.
-  Set PjL2MeasEvtCb.measOn to TRUE for QCIs to be measured.
-  For the number of measurements requested.
   -  For all Ues with RBs matching qCI.
      -  Add RbCb to the RB linked list.
      -  Set pjRbCb.measOn to measurement type.
      -  If Meas type is DL_DELAY
         -  Update COUNT to startCount.
-  Start l2Timer

 *  @param[in] pst      post structure
 *  @param[in] action   action 
 *  @param[in] cfg      LM management structure
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 PjMiLpjL2MeasReq 
(
Pst            *pst, 
PjL2MeasReqEvt *measReqEvt 
)
#else
PUBLIC S16 PjMiLpjL2MeasReq (pst, measReqEvt)
Pst            *pst; 
PjL2MeasReqEvt *measReqEvt;
#endif
{
   S16            ret = ROK;
   PjL2MeasEvtCb  *measEvtCb = NULLP;
   PjL2MeasCb     *measCb    = NULLP;
   U8             cntr;
   U8             qciIdx;
   KwUeCb         *prevUeCb;
   KwUeCb         *ueCb;
   RbCb           **rbCbLst;
   PjRbCb         *pjRbCb;
   PjL2MeasCfmEvt measCfmEvt;
   Bool           matchFound[LPJ_MAX_QCI];

   TRC3(PjMiLpjL2MeasReq);
   
   /* Initialize measCfmEvt */
   cmMemset((U8 *)&measCfmEvt, 0, sizeof(PjL2MeasCfmEvt));

   /* validate the received measReqEvt */
   ret = pjUtlValidateL2Meas(measReqEvt, &measCfmEvt);
   if(ret != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      PJLOGERROR(ERRCLS_ADD_RES, EKWXXX, (ErrVal) RFAILED,
            "PjMiLpjL2MeasReq: pjUtlValidateL2Meas failed.");
#endif /* ERRCLASS & ERRCLS_ADD_RES */
      pjUtlSndL2MeasNCfm(measReqEvt, &measCfmEvt);
      RETVALUE(RFAILED);
   }

   /* Allocate and intialise the Event Control Block */
   PJ_ALLOC(measEvtCb, sizeof(PjL2MeasEvtCb));
   if(measEvtCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      PJLOGERROR(ERRCLS_ADD_RES, EKWXXX, (ErrVal) RFAILED,
            "PjMiLpjL2MeasReq: PJ_ALLOC Failed for measCfmEvt.");
#endif /* ERRCLASS & ERRCLS_ADD_RES */
      measCfmEvt.numCfm = 1;
      measCfmEvt.transId = measReqEvt->transId;
      measCfmEvt.measType = measReqEvt->measReq.measType;
      measCfmEvt.status.status = LCM_PRIM_NOK;
      measCfmEvt.status.reason = LPJ_CAUSE_MEM_FAILED;
      pjUtlSndL2MeasNCfm(measReqEvt, &measCfmEvt);
      RETVALUE(RFAILED);
   }

   measEvtCb->transId      = measReqEvt->transId;
   measEvtCb->l2TmrCfg.enb = TRUE;
   measEvtCb->l2TmrCfg.val = measReqEvt->measPeriod;
   cmInitTimers(&measEvtCb->l2Tmr, PJ_L2_MAX_TIMERS);

   measCb = &measEvtCb->measCb;
   cmMemset((U8 *)matchFound, 0, sizeof(matchFound));
   measCb->measType  = measReqEvt->measReq.measType;
   for(cntr = 0; cntr < measReqEvt->measReq.numQci; cntr++)
   {
      measCb->qci[measCb->numQci++]  = measReqEvt->measReq.qci[cntr];
      kwCb.pjL2Cb.measOn[measReqEvt->measReq.qci[cntr]] |= 
                           measReqEvt->measReq.measType;
   }

   ueCb = NULLP;
   prevUeCb = NULLP;

   /* update the RBCBs of the measurement */
   while (cmHashListGetNext(&(kwCb.ueLstCp), (PTR) prevUeCb, (PTR *)&ueCb) == ROK)
   {
      rbCbLst  =  ueCb->drbCb;
      for(cntr = 0; cntr< KW_MAX_DRB_PER_UE; cntr++)
      {
         if(rbCbLst[cntr] == NULLP) 
         {
            continue;
         }
         pjRbCb = &rbCbLst[cntr]->pjRbCb;
         
         if(((kwCb.pjL2Cb.measOn[pjRbCb->qci] & measCb->measType) == measCb->measType) 
             &&((pjRbCb->rbL2Cb.measOn & measCb->measType) == LPJ_L2MEAS_NONE))
         {
             if(measCb->measType &  LPJ_L2MEAS_DL_DELAY)
             {
                PJ_INIT_RB_L2CB(PJ_L2MEAS_DL_DELAY, measCb, pjRbCb);
                pjRbCb->rbL2Cb.startCount = pjRbCb->dlCb.count;
             }
             if(measCb->measType & LPJ_L2MEAS_DL_DISC)
             {
                PJ_INIT_RB_L2CB(PJ_L2MEAS_DL_DISC, measCb, pjRbCb);
             }
             if(measCb->measType & LPJ_L2MEAS_UL_LOSS)
             {
                PJ_INIT_RB_L2CB(PJ_L2MEAS_UL_LOSS, measCb, pjRbCb);
             }
            pjRbCb->rbL2Cb.measOn |= measCb->measType;
            matchFound[pjRbCb->qci] = TRUE;
         }
         else
         {
            /* Check if qci is configured for rbCb */
            for(qciIdx = 0; qciIdx < measCb->numQci; qciIdx++)
            {
               if(pjRbCb->qci == measCb->qci[qciIdx])
               {
                  matchFound[pjRbCb->qci] = TRUE;
                  break;
               }
            }
         }
      }
      prevUeCb = ueCb;
   }
   for(cntr = 0; cntr < measCb->numQci; cntr++)
   {
      if(matchFound[measCb->qci[cntr]] == FALSE)
      {
         measCfmEvt.measType = measReqEvt->measReq.measType;
         measCfmEvt.measCfm[measCfmEvt.numCfm].qci  = measCb->qci[cntr];
         kwCb.pjL2Cb.measOn[measCb->qci[cntr]] = 0;
         measCfmEvt.numCfm++;
      }
   }
   if(measCfmEvt.numCfm > 0)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      PJLOGERROR(ERRCLS_ADD_RES, EKWXXX, (ErrVal) RFAILED,
               "PjMiLpjL2MeasReq: qci not configured.");
#endif /* ERRCLASS & ERRCLS_ADD_RES */
      measCfmEvt.transId = measReqEvt->transId;
      measCfmEvt.status.status = LCM_PRIM_NOK;
      measCfmEvt.status.reason = LPJ_CAUSE_INVALID_QCI;
      PJ_FREE(measEvtCb, sizeof(PjL2MeasEvtCb));
      pjUtlSndL2MeasNCfm(measReqEvt, &measCfmEvt);
      RETVALUE(RFAILED);
   }

   for(cntr = 0; cntr < PJ_MAX_L2MEAS_EVT; cntr++)
   {
      if(kwCb.pjL2Cb.pjMeasEvtCb[cntr] == NULLP)
      {
         measEvtCb->cbIdx = cntr;
         kwCb.pjL2Cb.pjMeasEvtCb[cntr]  = measEvtCb;
         kwCb.pjL2Cb.pjNumMeas++;
         break;
      }
   }

   pjStartTmr((PTR)measEvtCb, PJ_EVT_L2_TMR); 

   RETVALUE(ret);
} /* PjMiLpjL2MeasReq */

#endif /* LTE_L2_MEAS */
#endif
/**
 * @brief
      This function configures the PDCP data sap
 *
 *  @param[in] cfg         PDCP LM Sap structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SAP
 *                   LCM_REASON_INVALID_SAP 
 *                   LCM_REASON_RECONFIG_FAIL 
 */
#ifdef ANSI
PRIVATE S16 pjLmmPjuSapCfg
(
PjSapCfg   *cfg               /* SAP control block */
)
#else
PRIVATE S16 pjLmmPjuSapCfg(cfg)
PjSapCfg   *cfg;              /* SAP control block */
#endif
{
   KwPjuSapCb  *pjuSap;
   
   TRC2(pjLmmPjuSapCfg)

   /* Validate config parameters */
   if (cfg->sapId >= PJ_MAX_PJUSAPS )
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   pjuSap = &(kwCb.pjuSap[cfg->sapId]);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
   if(pjuSap->state  != KW_SAP_NOT_CFG)
   {
      KWLOGERROR(ERRCLS_INT_PAR, EKW079, (ErrVal) pjuSap->state,
               "pjLmmPjuSapCfg: Invalid pjuSap State");
      RETVALUE(LCM_REASON_RECONFIG_FAIL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   
   /* Fill the parameters */
   pjuSap->pst.selector = cfg->selector;
   pjuSap->pst.route = cfg->route;
   pjuSap->pst.prior =  cfg->priority;
   /* kw002.201 With multicore support layer shall use the assigned region
      and pool from SSI */
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
   pjuSap->pst.region = kwCb.pjInit.region; 
   pjuSap->pst.pool = kwCb.pjInit.pool;
#else
   pjuSap->pst.region = cfg->mem.region; 
   pjuSap->pst.pool = cfg->mem.pool;
#endif
   pjuSap->pst.dstProcId = cfg->procId;
   pjuSap->pst.dstEnt = cfg->ent;
   pjuSap->pst.dstInst = cfg->inst;
   pjuSap->pst.srcProcId = kwCb.init.procId;
   pjuSap->pst.srcEnt = kwCb.pjInit.ent;
   pjuSap->pst.srcInst = kwCb.init.inst;
   pjuSap->pst.event = EVTNONE;
   pjuSap->spId = cfg->sapId;

   /* make sap unbound initially */
   pjuSap->state = KW_SAP_CFG;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmPjuSapCfg */

/**
 * @brief
      This function configures the PDCP control sap
 *
 *  @param[in] cfg         PDCP LM Sap structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SAP
 *                   LCM_REASON_INVALID_SAP 
 *                   LCM_REASON_RECONFIG_FAIL 
 */
#ifdef ANSI
PRIVATE S16 pjLmmCpjSapCfg
(
PjSapCfg   *cfg               /* SAP control block */
)
#else
PRIVATE S16 pjLmmCpjSapCfg(cfg)
PjSapCfg   *cfg;              /* SAP control block */
#endif
{
   KwCpjSapCb  *cpjSap;
   
   TRC2(pjLmmCpjSapCfg)

   /* Validate config parameters */
   if (cfg->sapId >= PJ_MAX_CPJSAPS )
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   cpjSap = &(kwCb.cpjSap);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
   if(cpjSap->state  != KW_SAP_NOT_CFG)
   {
      KWLOGERROR(ERRCLS_INT_PAR, EKW080, (ErrVal) cpjSap->state,
               "pjLmmCpjSapCfg: Invalid cpjSap State");
      RETVALUE(LCM_REASON_RECONFIG_FAIL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   
   /* Fill the parameters */
   cpjSap->pst.selector = cfg->selector;
   cpjSap->pst.route = cfg->route;
   cpjSap->pst.prior =  cfg->priority;
   /* kw002.201 With multicore support layer shall use the assigned region
      and pool from SSI */
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
   cpjSap->pst.region = kwCb.pjInit.region; 
   cpjSap->pst.pool = kwCb.pjInit.pool;
#else
   cpjSap->pst.region = cfg->mem.region; 
   cpjSap->pst.pool = cfg->mem.pool;
#endif
   cpjSap->pst.dstProcId = cfg->procId;
   cpjSap->pst.dstEnt = cfg->ent;
   cpjSap->pst.dstInst = cfg->inst;
   cpjSap->pst.srcProcId = kwCb.init.procId;
   cpjSap->pst.srcEnt = kwCb.pjInit.ent;
   cpjSap->pst.srcInst = kwCb.init.inst;
   cpjSap->pst.event = EVTNONE;
   cpjSap->spId = cfg->sapId;

   /* make sap unbound initially */
   cpjSap->state = KW_SAP_CFG;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmCpjSapCfg */

/**
 * @brief
     Validates the PDCP general control parameters 
 *
 *  @param[in] cntrl PDCP LM structure 
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SUBACTION
 *                   LCM_REASON_INVALID_ACTION
 */
#ifdef ANSI
PRIVATE S16 pjLmmValidateGenCntrl
(
PjMngmt *cntrl                 /* PDCP LM structure */
)
#else
PRIVATE S16 pjLmmValidateGenCntrl(cntrl)
PjMngmt *cntrl;                /* PDCP LM structure */
#endif
{
   S16 reason;                 /* reason for failure */
   U8 action;                  /* action parameter */
   U8 sAction;                 /* subaction field */


   reason = LCM_REASON_NOT_APPL;

   TRC2(pjLmmValidateGenCntrl);

   action = cntrl->t.cntrl.action;
   sAction = cntrl->t.cntrl.subAction;

   switch (action)
   {
      case AENA:
      case ADISIMM:
         if ((sAction != SAUSTA)
            && (sAction != SADBG)
            && (sAction != SATRC))
         {
            reason = LCM_REASON_INVALID_SUBACTION;
         }
         break;
      case ASHUTDOWN:
         break;
      default:
         reason = LCM_REASON_INVALID_ACTION;
         break;
   }
   
   RETVALUE(reason);
} /* pjLmmValidateGenCntrl */

#if 0
/**
 * @brief
      Delete all SAPs and Control Blocks
 *
 * @b Description
 *       Shutdown of PDCP happens at modules level, tm, um and am modules will
 *       be shutdown using utility functin and dbm shutdown will clean up the
 *       SAPs, control blocks and lists. 
 *
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_HASHING_FAILED 
 */
#ifdef ANSI
PRIVATE S16 pjLmmShutdown
(
Void
)
#else
PRIVATE S16 pjLmmShutdown()
#endif
{
   TRC2(pjLmmShutdown)

   /* Call PDCP DBM Cleanup function.
    * RLC LM shutdown takes care of cleanup of Gloabal and RLC resources */
   pjUtlShutdown();

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of pjLmmShutdown */
#endif
/**
 * @brief
     Function processes the general control request 
 *
 *  @param[in] cntrl PDCP LM structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SUBACTION
 *                   LCM_REASON_INVALID_ACTION
 */
#ifdef ANSI
PRIVATE S16 pjLmmGenCntrl
(
PjMngmt *cntrl                 /* PDCP LM structure */
)
#else
PRIVATE S16 pjLmmGenCntrl(cntrl)
PjMngmt *cntrl;                /* PDCP LM structure */
#endif
{
   U8 action;            /* action parameter */
   U8 sAction;           /* subaction field */
   PjTrcCntrl *trcCntrl; /* trace */

#ifdef DEBUGP
   PjDbgCntrl *dbgCntrl; /* debug */
#endif /* DEBUGP */

   S16   reason;         /* reason for failure */
   
   TRC2(pjLmmGenCntrl);

   reason = LCM_REASON_NOT_APPL;

   /* Validate control parameters */
   reason = pjLmmValidateGenCntrl (cntrl);

   action = cntrl->t.cntrl.action;
   sAction = cntrl->t.cntrl.subAction;

   if (reason != LCM_REASON_NOT_APPL)
   {
      RETVALUE(reason);
   }

   switch(action)
   {
      case AENA:
      {
         switch(sAction)
         {
            case SAUSTA:
            {
               KW_SET_USTA_FLAG(kwCb, TRUE);
               break;
            }
            case SADBG:
            {
#ifdef DEBUGP
               dbgCntrl = &cntrl->t.cntrl.s.dbgCntrl;
               KW_GET_DBG_MASK(kwCb) |= dbgCntrl->dbgMask;
#endif /* DEBUGP */
               break;
            }
            case SATRC:
            {
               trcCntrl = &cntrl->t.cntrl.s.trcCntrl;
               kwCb.init.trc = TRUE;
               kwCb.trcLen = trcCntrl->trcLen;
               (kwCb.trcMask) |= trcCntrl->trcMask;
                break;
            }
         }
         break;
      }
      case ADISIMM:
      {
         switch(sAction)
         {
            case SAUSTA:
            {
               kwCb.init.usta = FALSE;
            }
            break;
            case SADBG:
            {
#ifdef DEBUGP
               dbgCntrl = &cntrl->t.cntrl.s.dbgCntrl;
               KW_GET_DBG_MASK(kwCb) &= ~(dbgCntrl->dbgMask);
#endif /* DEBUGP */
            }
            break;
            case SATRC:
            {
               trcCntrl = &cntrl->t.cntrl.s.trcCntrl;
               kwCb.init.trc = FALSE;
               kwCb.trcMask &= ~(trcCntrl->trcMask);
            }
            break;
         }
         break;
      }
#if 0
      case ASHUTDOWN:
      {
         reason = pjLmmShutdown();
         break;
      }
#endif      
   }
   RETVALUE(reason);
} /* pjLmmGenCntrl */

/**
 * @brief
     forms Lm Cfm based on the return values 
 *
 *  @param[in] cntrl PDCP LM structure
 *  @return  S16
 *      -# Success : ROK 
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 pjLmmFormLmCfm
(
Pst      *pst,
Header   *hdr,
Reason   reason
)
#else
PRIVATE S16 pjLmmFormLmCfm(pst, hdr, reason)
Pst      *pst;
Header   *hdr;
Reason   reason;
#endif
{
   PjMngmt  rCfm;
   S16      ret;
   TRC3(pjLmmFormLmCfm)

   ret = ROK;

   if (reason == LCM_REASON_NOT_APPL)
   {
      rCfm.cfm.status = LCM_PRIM_OK;
      rCfm.cfm.reason = LCM_REASON_NOT_APPL;

      ret = ROK;
   }
   else
   {
      rCfm.cfm.status = LCM_PRIM_NOK;
      rCfm.cfm.reason = reason;

      ret = RFAILED;
   }

   pjLmmSendCfm(pst, &rCfm, TCNTRL, hdr);

   RETVALUE(ret);
} /* pjLmmFormLmCfm */

/**
 * @brief
     Function gather the general PJU SAP status 
 *
 *  @param[in] sta LM PJU Sap Status structure 
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 pjLmmGetPjuSapSta 
(
PjPjuSapSta *sta                /* PJU SAP status */
)
#else
PRIVATE S16 pjLmmGetPjuSapSta(sta)
PjPjuSapSta *sta;               /* PJU SAP status */
#endif
{
   TRC2(pjLmmGetPjuSapSta);

   /* Validate config parameters */
   if (sta->spId >= PJ_MAX_PJUSAPS )
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   /* Get Sap control block */
   sta->state = kwCb.pjuSap[sta->spId].state;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmGetPjuSapSta */

/**
 * @brief
     Function gather the general CPJ SAP status
 *
 *  @param[in] sta LM CPJ Sap Status structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 pjLmmGetCpjSapSta
(
PjCpjSapSta *sta              /* RLU SAP status */
)
#else
PRIVATE S16 pjLmmGetCpjSapSta(sta)
PjCpjSapSta *sta;             /* RLU SAP status */
#endif
{
   TRC2(pjLmmGetCpjSapSta);

   /* Validate config parameters */
   if (sta->spId >= PJ_MAX_CPJSAPS )
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   sta->state = kwCb.cpjSap.state;

   RETVALUE(LCM_REASON_NOT_APPL);

} /* pjLmmGetCpjSapSta */

/**
 * @brief
     Gather the general statistics 
 *
 *  @param[in] sts       LM general statistics structure
 *  @param[in] action    action
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 pjLmmGetGenSts
(
PjGenSts *sts,              /* general statistics structure */
Action action               /* Action */
)
#else
PRIVATE S16 pjLmmGetGenSts(sts, action)
PjGenSts *sts;              /* general statistics structure */
Action action;              /* Action */
#endif
{
   PjGenSts *genSts;        /* General statistics */

   TRC2(pjLmmGetGenSts);

   /* Validate protocol parameters */
   if ((action != LPJ_ZEROSTS) && (action != LPJ_NOZEROSTS))
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   KW_MEM_CPY (sts, &(kwCb.pjGenSts), sizeof (PjGenSts));
   
   if(action == LKW_ZEROSTS)
   {
      genSts = &(kwCb.pjGenSts);
      KW_MEM_SET (genSts, 0, sizeof (PjGenSts));
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmGetGenSts */

/**
 * @brief
     Gather the SAP statistics
 *
 *  @param[in] sts     LM general statistics structure
 *  @param[in] elmnt   element 
 *  @param[in] action  action 
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 pjLmmGetSapSts
(
PjMngmt *sts,               /* PDCP layer management */
Elmnt elmnt,                /* element */
Action action               /* Action */
)
#else
PRIVATE S16 pjLmmGetSapSts(sts, elmnt, action)
PjMngmt *sts;               /* PDCP layer management */
Elmnt elmnt;                /* element */
Action action;              /* Action */
#endif
{
   KwPjuSapCb   *pjuSapCb;      /* PDCP User SAP control block */
   PjPjuSts     *pjuSap;        /* place holder for SAP statistics */

   TRC2(pjLmmGetSapSts);

   /* Validate protocol parameters */
   if (action != LPJ_ZEROSTS && action != LPJ_NOZEROSTS)
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   pjuSap = &sts->t.sts.s.pjuSap;

   /* Get Sap control block */
   /* kw005.201, modified suId to spId */
   pjuSapCb = &kwCb.pjuSap[pjuSap->spId];

   pjuSap->rxSdus = pjuSapCb->sts.rxSdus;
   pjuSap->txSdus = pjuSapCb->sts.txSdus;

   if (action == LPJ_ZEROSTS)
   {
      KW_MEM_SET (&pjuSapCb->sts, 0, sizeof (PjPjuSts));
   }

   SGetDateTime(&sts->t.sts.dt);

   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmGetSapSts */


/**
 * @brief
    This function sends Unsolicited Status Indication to the Layer Management
    Entity. 
 *
 *  @param[in] category  Category 
 *  @param[in] event     event
 *  @param[in] cause     cause
 *  @param[in] cause     ueId 
 *  @return  Void
 */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC Void pjLmmSendAlarm
(
U16      category,  /* Category */
U16      event,     /* Alarm event */
U16      cause,     /* Alarm cause */
SuId     suId,      /* suId */
U32      ueId,      /* ueId */
U8       qci        /* qci */
)
#else
PUBLIC Void pjLmmSendAlarm(category, event, cause, suId, ueId, qci)
U16      category;  /* Category */
U16      event;     /* Alarm event */
U16      cause;     /* Alarm cause */
SuId     suId;      /* suId */
U32      ueId;      /* ueId */
U8       qci;       /* qci */
#endif
#else
#ifdef ANSI
PUBLIC Void pjLmmSendAlarm
(
U16      category,  /* Category */
U16      event,     /* Alarm event */
U16      cause,     /* Alarm cause */
SuId     suId,      /* suId */
U32      ueId       /* ueId */
)
#else
PUBLIC Void pjLmmSendAlarm(category, event, cause, suId, ueId)
U16      category;  /* Category */
U16      event;     /* Alarm event */
U16      cause;     /* Alarm cause */
SuId     suId;      /* suId */
U32      ueId;      /* ueId */
#endif
#endif
{

   PjMngmt usta;    /* Rlc Management Structure */

   TRC2(pjLmmSendAlarm);

   if(kwCb.init.usta == FALSE)
   {
      RETVOID;
   }

   /* initialize the management structure */
   KW_MEM_SET(&usta, 0, sizeof(PjMngmt));

   usta.hdr.elmId.elmnt = STGEN;
   usta.hdr.entId.ent = kwCb.init.ent;
   usta.hdr.entId.inst = kwCb.init.inst;

   /* fill in the event and category */
   usta.t.usta.alarm.category = category;
   usta.t.usta.alarm.event = event;
   usta.t.usta.alarm.cause = cause;

   /* set the suId and ueId */
   usta.t.usta.ueId = ueId;
   usta.t.usta.suId = suId;
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   if(event == LPJ_EVT_MEAS_HALT)
   {
      usta.t.usta.qci = qci;
   }
#endif

   /* update the date and time */
   (Void) SGetDateTime(&usta.t.usta.alarm.dt);

   PjMiLpjStaInd(&(kwCb.pjInit.lmPst), &usta);

   RETVOID;

} /* pjLmmSendAlarm */

/**
 * @brief
    This function sends trace indication to LM 
 *
 *  @param[in] event    event
 *  @param[in] mBuf     meessage buffer
 *  @return S16
 *      -# Success : ROK 
 *      -# Failure : RFAILED 
 */
#ifdef ANSI
PUBLIC S16 pjLmmSendTrc
(
Event event,                 /* event */
Buffer *mBuf                 /* message buffer */
)
#else
PUBLIC S16 pjLmmSendTrc(event, mBuf)
Event event;                 /* event */
Buffer *mBuf;                /* message buffer */
#endif
{
   /* patch kw004.201 Corrected pjLmmSendTrc function definition */
   PjMngmt trc;              /* PDCP management control block */
   Buffer   *dstMbuf;   
   MsgLen   bufLen;
   Data     *tempBuf;
   MsgLen   tempCnt;
   Pst      pst;

   TRC2(pjLmmSendTrc);

   KW_MEM_SET(&trc, 0, sizeof(PjMngmt));

   cmMemset((U8 *)&trc, 0, sizeof(PjMngmt));

   pst = kwCb.pjInit.lmPst;

   trc.t.trc.event = event;
   SGetDateTime(&trc.t.trc.dt);
   trc.cfm.status = LCM_PRIM_OK;
   trc.cfm.reason = LCM_REASON_NOT_APPL;
    
   if(mBuf != NULLP)
   {
     /* Check if the whole buffer is to be sent in Trace indication */
     if(kwCb.trcLen == LPJ_FULL_TRACE)
     {
        if (SCpyMsgMsg(mBuf, pst.region, pst.pool, &dstMbuf)
            != ROK)
        {
           /* rg005.201 removed SPutSBuf on error */
           PJDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF),(_kwPBuf,
                 "\npjLmmSendTrc(): SCpyMsgMsg Failed."));
           RETVALUE(RFAILED);
        }
        /* Send Trace Indication to Layer manager */
        PjMiLpjTrcInd(&pst, &trc, dstMbuf);
     }
     /* check if only a specified number of bytes are to be sent */
     else if(kwCb.trcLen > 0)
     {
        /* Get the length of the recvd message buffer */
        if (SFndLenMsg(mBuf, &bufLen) != ROK)
        {
           PJDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF),(_kwPBuf, 
                    "\npjLmmSendTrc(): SFndLenMsg Failed."));
           RETVALUE(RFAILED);
        }
        /* Check if the recvd buffer size is less than request trace len */
        if(bufLen < kwCb.trcLen)
        {
           /* Copy the whole of the recvd buffer in trace indication */
      
           if (SCpyMsgMsg(mBuf, pst.region, pst.pool, &dstMbuf)
              != ROK)
           {
              PJDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF),(_kwPBuf,
                      "\npjLmmSendTrc(): SCpyMsgMsg Failed."));
              RETVALUE(RFAILED);
           }
         
           /* Send Trace Indication to Layer manager */
           PjMiLpjTrcInd(&pst, &trc, dstMbuf);
        }
        /* if the recvd buffer size is greater than request trace len */
        if(bufLen >= kwCb.trcLen)
        {
           /* Get a temporary buffer to store the msg */
           PJ_ALLOC(tempBuf, kwCb.trcLen);

#if (ERRCLASS & ERRCLS_INT_PAR)
           if(tempBuf == NULLP)
           {
              (Void) SPutSMem(KW_GET_MEM_REGION(kwCb), KW_GET_MEM_POOL(kwCb));
      
               KWLOGERROR(ERRCLS_INT_PAR, EKWXXX, (ErrVal)0 ,
                   "pjLmmSendTrc: SGetSBuf Failed for tempBuf...!");
               RETVALUE(LCM_REASON_MEM_NOAVAIL);
           }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         
           /* Copy trcLen nos of bytes from the recvd message */
           if (SCpyMsgFix(mBuf,0,kwCb.trcLen,tempBuf,&tempCnt) != ROK)   
           {
              PJDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF), (_kwPBuf,
                      "\npjLmmSendTrc(): SCpyMsgFix Failed."));
              RETVALUE(RFAILED);
           }

           if (SGetMsg(pst.region, pst.pool, &dstMbuf) != ROK)
           {
              PJDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF), (_kwPBuf,
                      "\npjLmmSendTrc(): dstMbuf Allocation Failed."));
              RETVALUE(RFAILED);
           }
           /* Copy the tempBuf data to dst mBuf */
           if (SCpyFixMsg(tempBuf,dstMbuf,0,kwCb.trcLen,&tempCnt) != ROK)
           {
              PJDBGP((KW_DBGMASK_LMM | KW_DBGMASK_BRIEF), (_kwPBuf,
                     "\npjLmmSendTrc(): SCpyFixMsg Failed."));
              RETVALUE(RFAILED);
           }
  
           /* Free the memory allocated for tempBuf */
           PJ_FREE(tempBuf, kwCb.trcLen);
           /* Send Trace Indication to Layer manager */
           PjMiLpjTrcInd(&pst, &trc, dstMbuf);
        }
     }
   }
   else
   {
      PjMiLpjTrcInd(&pst, &trc, mBuf);
   }

   RETVALUE(ROK);

} /* end of pjLmmSendTrc */

#ifdef PWAV_HC
PUBLIC S16 sendPjMsgToFrm(
    Ent     dstEnt,
    Event   event,
    U16     len,
    U32     *data
    )
{
  Pst   pst;
  Buffer *mBuf = NULLP;

  if (SGetMsg(DFLT_REGION, DFLT_POOL, &mBuf) != ROK)
  {
    logError("%s", "Error in Getting Message buffer");
  }
  cmMemset((U8*)&pst, '\0', sizeof(Pst));

  pst.selector  = 0;
  pst.srcEnt    = ENTPJ;
  pst.srcProcId = 1; /* RADIO_PROC_ID */
  pst.dstEnt    = dstEnt;
  pst.dstProcId = 4; /* OAM_PROC_ID */
  pst.event     = event;

  /* Add data to mBuf here.
   */
  if( (len > 0) && (NULL != data) )
  {
    SAddPstMsgMult((Data *)data, len, mBuf);
  }

  if(SPstTsk(&pst, mBuf) !=ROK)
  {
    return RFAILED;
  }
  else
  {
    return ROK;
  }
}


PUBLIC S16 sendPjMsgToPrm(
    Event   event,
    U16     len,
    U32     *data
    )
{
  /* Add data to mBuf here.
   * There is no data in these functions.
   * So mBuf is allocated but empty.
   */
  if( RFAILED == sendPjMsgToFrm( ENT_PRM, event, len, data ))
  {
    return RFAILED;
  }
  else
  {
    return ROK;
  }
}

void sendPjStartIndToPrm(void)
{
  
  U32 version = VERSION_MODID_PDCP;
  /*send start indication to PRM indicating successful startup*/
  if( sendPjMsgToPrm(EVT_START_IND, sizeof(U32), &version ) == ROK)
  {
    logError("%s", "Sent EVT_START_IND from PJ to PRM \n");
  }
}

void sendPjHelloAckToPrm(Buffer   *mBuf)
{
  MsgLen              cCnt;
  MsgPrmModHello      hello;
  MsgModPrmHelloAck   helloAck;

  if (SCpyMsgFix( mBuf, 0, sizeof(MsgPrmModHello), (Data*)&hello , &cCnt ) != ROK)   
  {
    logError("%s", "sendPjHelloAckToPrm(): SCpyMsgFix Failed\n" );
    RETVOID;
  }
  helloAck.helloAckSeqNum = hello.helloSeqNum;

  if(sendPjMsgToPrm(EVT_HELLO_ACK, sizeof(MsgModPrmHelloAck), (U32*)&helloAck) == ROK)
  {
    logInfo("Sent EVT_HELLO_ACK from PJ to PRM for sequence number:%d \n", helloAck.helloAckSeqNum );
  }
  else
  {
    logInfo("Sent EVT_HELLO_ACK from PJ to PRM failed for sequence number:%d \n", helloAck.helloAckSeqNum );
  }
}

void procRcvdPjExitMsg(void)
{
  logInfo("%s", "Exiting the PJ \n" );
  exit(0);
}
#endif
/********************************************************************30**

         End of file:     gp_pj_lmm.c@@/main/1 - Fri Nov 13 14:13:47 2009

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
/main/1      ---       gk   1. LTERLC Release 2.1
/main/2      kw002.201 vp   1. Patch for optimization,fixes for multi-region
                               and retransmission.
/main/3      kw004.201 ap   1. Corrected pjLmmSendTrc function definition
/main/4      kw005.201 ap   1. Modified suId to spId in PjPjuSts.
                            2. Added support for L2 Measurement.
/main/5      kw006.201 ap   1. ccpu00120574, fixed the warning.
*********************************************************************91*/

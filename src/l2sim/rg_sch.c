/********************************************************************16**

                         (c) COPYRIGHT 2011 by 
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

                    Email: support@ccpu.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_sch.c
  
     Sid:      gk_sch.c@@/main/3 - Sat Jul 30 02:21:34 2011
  
     Prg:      SY
  
**********************************************************************/

/** @file rg_sch.c
@brief This file implements the schedulers main access to MAC layer code.
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "rg_sch_err.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"       /* common tokens */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */

#ifdef PWAV
  #include "log.h"
#endif

#ifdef LTE_L2_MEAS
#undef LTE_L2_MEAS
#undef LTE_L2_MEAS_STUB
#endif

PUBLIC RgSchCb rgSchCb[RGSCH_MAX_INST];
/* local defines */

/************** LRG Interface ****************/
/**
 * @brief Layer Manager Configuration request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgSchCfgReq
 *     
 *     This function handles the configuration
 *     request received at scheduler instance from the Layer Manager.
 *     -# Based on the cfg->hdr.elmId.elmnt value it invokes one of the
 *        functions rgHdlGenCfg() or rgHdlSapCfg().
 *     -# Invokes RgMiLrgSchCfgCfm() to send back the confirmation to the LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfg, the configuration parameter's structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgSchCfgReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfg     /* config structure  */
)
#else
PUBLIC S16 RgMiLrgSchCfgReq(pst, cfg)
Pst      *pst;    /* post structure  */
RgMngmt  *cfg;    /* config structure  */
#endif    
{
   U16       ret = LCM_PRIM_OK;
   U16       reason = LCM_REASON_NOT_APPL;
   RgMngmt   cfm;
   Pst       cfmPst;
#ifdef DEBUGP
   Inst      inst = (pst->dstInst  - RGSCH_INST_START);
#endif

   TRC3(RgMiLrgSchCfgReq)
   
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "\nRgMiLrgSchCfgReq(): pst->srcEnt=%d, pst->srcInst=%d", 
   pst->srcEnt, pst->srcInst));

   logInfo("Received LRG config request from pst->srcEnt=%d", pst->srcEnt);

   if(pst->dstInst < RGSCH_INST_START)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "\nRgMiLrgSchCfgReq(): Invalid inst ID"));
      RETVALUE(ROK);
   }

   /* Fill the post structure for sending the confirmation */
   rgSCHLmmFillCfmPst(pst, &cfmPst, cfg);

   /* rg003.201: Removed allocation for RgMngmt */
   /* Initialize the cfg cfm structure 
   if (SGetSBuf(cfmPst.region, cfmPst.pool, (Data **)&cfm, sizeof(RgMngmt))
      != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "\nRgMiLrgSchCfgReq(): Memory Unavailable for Confirmation"));
      RETVALUE(ROK);
   } */
   cmMemset((U8 *)&cfm, 0, sizeof(RgMngmt));

   /* rg009.lpr-ccpu00116453: Adding transId */
#ifdef LMINT3
   cfm.hdr.transId =
      cfg->hdr.transId;
#endif

   cfm.hdr.elmId.elmnt = cfg->hdr.elmId.elmnt;
   switch(cfg->hdr.elmId.elmnt)
   {
      case STSCHINST:
         reason = rgSCHLmmInstCfg(&cfg->t.cfg,pst->dstInst );
         RGSCHDBGINFO(inst,(rgSchPBuf(inst), "\nRgMiLrgSchCfgReq(): Scheduler instance \
                   configuration is done with reason=%d.",  reason));
         break;
      default:
         ret = LCM_PRIM_NOK;
         reason = LCM_REASON_INVALID_ELMNT;
         RGSCHDBGERR(inst,(rgSchPBuf(inst), "\nRgMiLrgSchCfgReq(): Invalid Elmnt=%d", cfg->hdr.elmId.elmnt));
         break;
   }

   if (reason != LCM_REASON_NOT_APPL)
   {
      ret = LCM_PRIM_NOK;
   }

   /* rg003.201: Changed from pointer to Structure */ 
   cfm.cfm.status = ret;
   cfm.cfm.reason = reason;

   RgMiLrgSchCfgCfm(&cfmPst, &cfm);
  /* rg004.201: Memory Fix */
/*   SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(RgMngmt)); */
   
   RETVALUE(ROK);
}/*-- RgMiLrgSchCfgReq --*/


/**
 * @brief Layer Manager Control request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgSchCntrlReq
 *     
 *     This function handles the control
 *     request received from the Layer Manager.
 *      -# Based on cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action
 *      and cntrl->t.cntrl.subAction, it performs the appropriate control action
 *      of SAP (enable/disable) and layer shutdown.
 *      -# Invokes the RgMiLrgSchCntrlCfm to send back the confirmation to LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cntrl, the control parameter's structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgSchCntrlReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *cntrl   /* control structure  */
)
#else
PUBLIC S16 RgMiLrgSchCntrlReq(pst, cntrl)
Pst      *pst;    /* post structure  */
RgMngmt  *cntrl;  /* control structure  */
#endif    
{
   S16       ret = ROK;            /* return value */
   Pst       cfmPst;
   RgMngmt   cfm;
   
   Inst      inst = (pst->dstInst - RGSCH_INST_START); /* Scheduler instance Id */
   TRC3(RgMiLrgSchCntrlReq)
   
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "\nRgMiLrgSchCntrlReq():cntrlReq for elmnt= %d,action=%d,subaction=%d",
   cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action, cntrl->t.cntrl.subAction));
#ifdef PWAV
   logInfo("\nRgMiLrgSchCntrlReq():cntrlReq for elmnt= %d,action=%d,subaction=%d",
   cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action, cntrl->t.cntrl.subAction);
#endif

   /* Fill the post structure for sending the confirmation */
   rgSCHLmmFillCfmPst(pst, &cfmPst, cntrl);

   /* rg003.201: Changed from pointer to Structure so removed allocation*/ 
   /* Initialize the cfg cfm structure 
   if (SGetSBuf(cfmPst.region, cfmPst.pool, (Data **)&cfm, sizeof(RgMngmt))
      != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "\nRgMiLrgSchCntrlReq(): Memory Unavailable for Confirmation"));
      SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(RgMngmt));
      RETVALUE(ROK);
   } */
   cmMemset((U8 *)&cfm, 0, sizeof(RgMngmt));

   /* rg009.lpr-ccpu00116453: Adding transId */
#ifdef LMINT3
   cfm.hdr.transId =
      cntrl->hdr.transId;
#endif

   cfm.hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
   cfm.t.cntrl.action = cntrl->t.cntrl.action;
   cfm.t.cntrl.subAction = cntrl->t.cntrl.subAction;

   /* Check if General Config Done*/
   if(rgSchCb[inst].rgSchInit.cfgDone != TRUE)
   {
      cfm.cfm.status = LCM_PRIM_NOK;
      cfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cfm.hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
      RgMiLrgSchCntrlCfm(&cfmPst, &cfm);

      RGSCHDBGERR(inst,(rgSchPBuf(inst), "\nRgMiLrgSchCntrlReq(): Gen Cfg not done."));
      /*rg004.201: Memory Fix */
/*      SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(RgMngmt)); */
      RETVALUE(ROK);
   }
   /* General Config done, process the Control request */   
   switch(cntrl->hdr.elmId.elmnt)
   {
      case STGEN:
         rgSCHLmmGenCntrl(cntrl, &cfm, &cfmPst);
         break;
      case STTFUSAP:
      case STRGRSAP:
         rgSCHLmmSapCntrl(cntrl, &cfm, &cfmPst);
		 
         break;
      default: 
         cfm.cfm.status = LCM_PRIM_NOK;
         cfm.cfm.reason = LCM_REASON_INVALID_PAR_VAL;
         RgMiLrgSchCntrlCfm(&cfmPst, &cfm);
         RGSCHDBGERR(inst,(rgSchPBuf(inst), "\nRgMiLrgSchCntrlReq(): invalid elmnt=%d",
         cntrl->hdr.elmId.elmnt));
         break;
   }
   //13-07
   
/*   SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(RgMngmt)); */
   RETVALUE(ret);
}/*-- RgMiLrgSchCntrlReq --*/
#if 0
#ifdef LTE_L2_MEAS
/**
 * @brief Layer Manager L2 Measurement request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgSchL2MeasReq
 *     
 *     This function handles the control
 *     measurement request received from the Layer Manager.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  LrgSchMeasReqInfo *measInfo, measurement request info
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgSchL2MeasReq
(
Pst               *pst,     /* post structure  */
LrgSchMeasReqInfo *measInfo /* Meas Req Info */
)
#else
PUBLIC S16 RgMiLrgSchL2MeasReq(pst, measInfo)
   Pst               *pst;     /* post structure  */
   LrgSchMeasReqInfo *measInfo; /* Meas Req Info */
#endif    
{
   Pst                 cfmPst;
   RgSchCellCb         *cell;
   RgSchErrInfo        err;
   S16                 ret = ROK; 
   RgSchCb             *instCb =  &rgSchCb[(pst->dstInst - 1)];
#if (ERRCLASS & ERRCLS_ADD_RES) 
   CmLList             *lnk;
#endif
   U32                 idx;
   RgSchL2MeasCb       *measCb = NULLP;
#ifdef DEBUGP
   Inst      inst = (pst->dstInst - RGSCH_INST_START); /* Scheduler instance Id */
#endif

   err.errType  = 0;
   err.errCause = 0;

   TRC3(RgMiLrgSchL2MeasReq)
   
   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "\nRgMiLrgSchL2MeasReq():MeasReq"));

   /* Find the cellCb using cellId in measInfo. Iterate through all cells
    * in rgrsapCb in RgschCb */
   cell = NULLP;
   for (idx = 0; idx < instCb->numSaps; idx++)
   {
      if ( instCb->rgrSap[idx].cell->cellId == measInfo->cellId)
      {
         /* got the cell break the loop */
         cell = instCb->rgrSap[idx].cell;
         break;
      }
   }
   /* If no cellCb return Err with Invalid Cell Id */
   if (cell == NULLP)
   {
      rgSchL2mFillCfmPst(pst, &cfmPst, measInfo);
      RGSCHFILLERR(err, RGSCHERR_L2M_MEASREQ, RGSCHERR_SCH_INVALID_CELLID);
      rgSchL2mSndCfm(&cfmPst, NULLP, measInfo, TRUE);
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "\nRgMiLrgSchL2MeasReq():"
                  "Meas req Failed.Invalid Cell Id errType(%d) errCause(%d)\n",\
                  err.errType, err.errCause));
      RETVALUE(RFAILED);
   }
   /* Validate for Meas Types */
   if ( (ret = rgSCHUtlValidateMeasReq(cell, measInfo, &err)) != ROK)
   {
      rgSchL2mFillCfmPst(pst, &cfmPst, measInfo);
      RGSCHFILLERR(err, RGSCHERR_L2M_MEASREQ, RGSCHERR_SCH_INVALID_MEASTYPE);
      rgSchL2mSndCfm(&cfmPst, NULLP, measInfo, TRUE);
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "\nRgMiLrgSchL2MeasReq():"
               "Meas req Failed.Invalid Measurement Type"
               "errCasue(%d) errType(%d)\n", err.errType, err.errCause));
      RETVALUE(RFAILED);
   }
   
   /* Iterate through all meas requests in l2mList in cellCb */
#if (ERRCLASS & ERRCLS_ADD_RES) 
   lnk = cell->l2mList.first;
   while(lnk != NULLP)
   {
      /* Get the MeasCb : RgSchL2MeasCb */
      measCb = (RgSchL2MeasCb *)lnk->node;
      lnk = lnk->next;
      if (measCb->measReq.hdr.transId == measInfo->hdr.transId)
      {
         rgSchL2mFillCfmPst(pst, &cfmPst, measInfo);
         RGSCHFILLERR(err, RGSCHERR_L2M_MEASREQ, RGSCHERR_SCH_DUP_TRANSID);
         rgSchL2mSndCfm(&cfmPst, measCb, measInfo, TRUE);
         RGSCHDBGERR(inst,(rgSchPBuf(inst), "\nRgMiLrgSchL2MeasReq():"
                     "Meas req Failed.Dublicate TransId"
                     "errType(%d) errCause(%d)\n", err.errType, err.errCause));
         RETVALUE(RFAILED);
      }
   }
#endif
   /* Call L2M Function to store Meas req */
   ret = rgSchL2mMeasReq(cell, measInfo, err);
   if (ret != ROK)
   {
      rgSchL2mFillCfmPst(pst, &cfmPst, measInfo);
      RGSCHFILLERR(err, RGSCHERR_L2M_MEASREQ, RGSCHERR_SCH_L2MEAS_FAILED);
      rgSchL2mSndCfm(&cfmPst, measCb, measInfo, TRUE);
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "\nRgMiLrgSchL2MeasReq():"
               "Meas req Failed.errType(%d) errCause(%d)\n",\
               err.errType, err.errCause));
      RETVALUE(RFAILED);
   }
   RETVALUE(ret);
} /* -- RRgMiLrgSchL2MeasReq-- */
#endif /* LTE_L2_MEAS */

#endif


/************* RGR  Interface ****************/
/**
 * @brief API for bind request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrBndReq
 *     
 *     This API is invoked by RRM towards MAC to bind RGR SAP. 
 *     These API validates the Pst, spId, suId and sends the bind confirm to
 *     RRM.
 *
 *           
 *  @param[in]  Pst   *pst
 *  @param[in]  SuId  suId
 *  @param[in]  SpId  spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRgrBndReq
(
Pst   *pst, 
SuId  suId,
SpId  spId
)
#else
PUBLIC S16 RgUiRgrBndReq(pst, suId, spId)
Pst   *pst; 
SuId  suId;
SpId  spId;
#endif
{
   S16       ret = ROK;
   Pst       tmpPst;   /* Temporary Post Structure */
   Inst      instId = pst->dstInst-RGSCH_INST_START;
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   TRC3(RgUiRgrBndReq)
   
   RGSCHDBGPRM(instId,(rgSchPBuf(instId),"RgUiRgrBndReq: suId = %d; spId = %d\n",
                 suId, spId));
   
   tmpPst.prior       = pst->prior;
   tmpPst.route       = pst->route;
   tmpPst.selector    = pst->selector;
   tmpPst.region      = rgSchCb[instId].rgSchInit.region;
   tmpPst.pool        = rgSchCb[instId].rgSchInit.pool;
   tmpPst.srcProcId   = rgSchCb[instId].rgSchInit.procId;
   tmpPst.srcEnt      = rgSchCb[instId].rgSchInit.ent;
   tmpPst.srcInst     = rgSchCb[instId].rgSchInit.inst + RGSCH_INST_START;
   tmpPst.event       = EVTNONE;
   tmpPst.dstProcId   = pst->srcProcId;
   tmpPst.dstEnt      = pst->srcEnt;
   tmpPst.dstInst     = pst->srcInst;

   if(spId < rgSchCb[instId].numSaps)
   {
      /* Check the state of the SAP */
      switch (rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
         /* This case might not be needed if SAP not configured then it will go
          * to else of above if condition */
         case LRG_UNBND: /* SAP is not bound */
            RGSCHDBGINFO(instId,(rgSchPBuf(instId),"SAP Not yet bound\n"));
            rgSchCb[instId].rgrSap[spId].sapSta.sapState = LRG_BND;
            rgSchCb[instId].rgrSap[spId].sapCfg.suId = suId;
            /* Send Bind Confirm with status as SUCCESS */
            ret = rgSCHUtlRgrBndCfm(instId, suId, CM_BND_OK);
             /*Indicate to Layer manager  */
            rgSCHUtlFillDgnParams(instId, &dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgSCHLmmStaInd(instId, LCM_CATEGORY_INTERFACE,
                       LRG_EVENT_RGRSAP_ENB, LCM_CAUSE_UNKNOWN, &dgn);
            break;
         case LRG_BND: /* SAP is already bound*/
            RGSCHDBGINFO(instId,(rgSchPBuf(instId),"SAP Is already Bound\n"));
            ret = rgSCHUtlRgrBndCfm(instId, suId, CM_BND_OK);
            break;
         default: /* Should Never Enter here */
#if (ERRCLASS & ERRCLS_ADD_RES) 
            RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG001, 
                 (ErrVal)rgSchCb[instId].rgrSap[spId].sapSta.sapState,
                  "Invalid SAP State:RgUiRrgBndReq failed\n");
#endif
            ret = rgSCHUtlRgrBndCfm(instId, suId, CM_BND_NOK);
            break;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
/* ccpu00117035 - MOD - Changed ErrVal argument from accessing sap state 
   to spId to avoid seg fault due to invalid sapID */
      RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG002,
            (ErrVal)spId, "Invalid SAP Id:RgUiRrgBndReq failed\n");
#endif
      ret = RgUiRgrBndCfm(&tmpPst, suId, CM_BND_NOK);
   }
   RETVALUE(ret);
}  /* RgUiRgrBndReq */

/**
 * @brief API for unbind request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrUbndReq
 *     
 *     This API is invoked by RRM towards MAC to unbind RGR SAP. 
 *     These API validates the Pst, spId, suId and transfers the unbind request 
 *     specific information to corresponding ownership module (GOM) API.
 *
 *           
 *  @param[in]  Pst    *pst
 *  @param[in]  SuId   suId
 *  @param[in]  Reason reason
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRgrUbndReq
(
Pst    *pst,
SpId   spId,
Reason reason
)
#else
PUBLIC S16 RgUiRgrUbndReq(pst, spId, reason)
Pst    *pst; 
SpId   spId;
Reason reason;
#endif
{
   Inst instId = pst->dstInst-RGSCH_INST_START;
   TRC3(RgUiRgrUbndReq)
   
   RGSCHDBGPRM(instId,(rgSchPBuf(instId),"RgUiRgrUbndReq: spId = %d; reason = %d \n", spId, reason));

   /* SAP Id validation */
   if (spId < rgSchCb[instId].numSaps)
   {
      switch(rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound*/
            /* setting SAP state to UN BOUND */
            RGSCHDBGINFO(instId,(rgSchPBuf(instId),"SAP Is already Bound\n"));
            rgSchCb[instId].rgrSap[spId].sapSta.sapState = LRG_UNBND;
            break;
         default:
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG003,
                   (ErrVal)rgSchCb[instId].rgrSap[spId].sapSta.sapState,
                  "Invalid SAP State: RgUiRgrUbndReq failed\n");
#endif
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGSCHLOGERROR(instId,ERRCLS_INT_PAR, ERG004, 
            (ErrVal)rgSchCb[instId].rgrSap[spId].sapSta.sapState,
            "Invalid SAP Id:RgUiRgrUbndReq failed\n");
#endif
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* RgUiRgrUbndReq */
#if 0
/**
 * @brief API for configuration request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrCfgReq
 *     
 *     This API is invoked by RRM towards MAC to configure MAC. 
 *     These API validates the Pst, spId, suId and transfers the config request 
 *     specific information to corresponding ownership module (GOM) API.
 *
 *           
 *  @param[in]  Pst           *pst
 *  @param[in]  SpId          spId
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRgrCfgReq
(
Pst           *pst, 
SpId          spId,
RgrCfgTransId transId,
RgrCfgReqInfo *cfgReqInfo
)
#else
PUBLIC S16 RgUiRgrCfgReq(pst, spId, transId, cfgReqInfo)
Pst           *pst; 
SpId          spId;
RgrCfgTransId transId;
RgrCfgReqInfo *cfgReqInfo;
#endif
{

   S16       ret       = ROK;
   U8        cfmStatus = 0x00ff;
   U8        prntTrans[RGR_CFG_TRANSID_SIZE+1];
   Inst      instId = pst->dstInst-RGSCH_INST_START;

   TRC3(RgUiRgrCfgReq);

   cmMemcpy((U8 *)prntTrans, (U8 *)transId.trans, RGR_CFG_TRANSID_SIZE);
   prntTrans[RGR_CFG_TRANSID_SIZE] = '\0';

   RGSCHDBGPRM(instId,(rgSchPBuf(instId),"RgUiRgrCfgReq(): spId = %d; transId = %s;",
                 spId, prntTrans));

   if (cfgReqInfo == NULLP)
   {
      RGSCHDBGERR(instId,(rgSchPBuf(instId),"Input Message Buffer is NULL\n"));
      rgSCHUtlRgrCfgCfm(instId, spId, transId, cfmStatus); 
      RETVALUE(RFAILED);
   }

   if (spId < rgSchCb[instId].numSaps)
   {
      switch(rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound */
            RGSCHDBGINFO(instId,(rgSchPBuf(instId),"SAP Is already Bound\n"));
            break;
         default: /* Should never reach here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG005, 
                  (ErrVal)rgSchCb[instId].rgrSap[spId].sapSta.sapState,
                  "Invalid SAP State: RgUiRgrCfgReq failed\n");
#endif
            SPutSBuf(pst->region, pst->pool, (Data *)cfgReqInfo,
                                                (Size)sizeof(*cfgReqInfo));
            rgSCHUtlRgrCfgCfm(instId, spId, transId, cfmStatus); 
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG006,
            (ErrVal)rgSchCb[instId].rgrSap[spId].sapCfg.spId,
            "Invalid SAP Id:RgUiRgrCfgReq failed\n");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)cfgReqInfo,
                                            (Size)sizeof(*cfgReqInfo));
      rgSCHUtlRgrCfgCfm(instId, spId, transId, cfmStatus); 
      RETVALUE(RFAILED);
   }

   /* Handle configuration */
   ret = rgSCHGomHndlCfg(pst->region, pst->pool,
            &rgSchCb[(pst->dstInst - 1)], spId, transId, 
                        cfgReqInfo);
   if (ret != ROK)
   {
      RGSCHDBGERR(instId,(rgSchPBuf(instId),"RgUiRgrCfgReq:Configuration Request Handling Failed \n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* RgUiRgrCfgReq */
#endif
/* rg004.201-ADD-Added for SI Enhancement*/
#ifdef RGR_SI_SCH
/**
 * @brief API for SI configuration request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrSiCfgReq
 *     
 *     This API is invoked by RRM towards MAC to configure SI at MAC. 
 *     These API validates the Pst, spId, suId and transfers the 
 *     config request specific information to corresponding ownership 
 *     module (GOM) API.
 *
 *           
 *  @param[in]  Pst           *pst
 *  @param[in]  SpId          spId
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrSiCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRgrSiCfgReq
(
Pst           *pst, 
SpId          spId,
RgrCfgTransId transId,
RgrSiCfgReqInfo *cfgReqInfo
)
#else
PUBLIC S16 RgUiRgrSiCfgReq(pst, spId, transId, cfgReqInfo)
Pst           *pst; 
SpId          spId;
RgrCfgTransId transId;
RgrSiCfgReqInfo *cfgReqInfo;
#endif
{
   S16       ret       = ROK;
   U8        cfmStatus = RGR_CFG_CFM_NOK;
   U8        prntTrans[RGR_CFG_TRANSID_SIZE+1];
   Inst      instId = pst->dstInst-RGSCH_INST_START;

   TRC2(RgUiRgrSiCfgReq);

   cmMemcpy((U8 *)prntTrans, (U8 *)transId.trans, RGR_CFG_TRANSID_SIZE);
   prntTrans[RGR_CFG_TRANSID_SIZE] = '\0';

   RGSCHDBGPRM(instId,(rgSchPBuf(instId),"RgUiRgrSiCfgReq(): "
                "spId = %d; transId = %s;", spId, prntTrans));

   if (cfgReqInfo == NULLP)
   {
      RGSCHDBGERR(instId,(rgSchPBuf(instId),"Input Message Buffer "
                   "is NULL\n"));
      rgSCHUtlRgrSiCfgCfm(instId, spId, transId, cfmStatus); 
      RETVALUE(RFAILED);
   }

   if (spId < rgSchCb[instId].numSaps)
   {
      if(LRG_BND != rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG007, 
                  (ErrVal)rgSchCb[instId].rgrSap[spId].sapSta.sapState,
                  "Invalid SAP State: RgUiRgrSiCfgReq failed\n");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfgReqInfo,
                                          (Size)sizeof(*cfgReqInfo));
         rgSCHUtlRgrSiCfgCfm(instId, spId, transId, cfmStatus); 
         RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG008,
            (ErrVal)rgSchCb[instId].rgrSap[spId].sapCfg.spId,
            "Invalid SAP Id:RgUiRgrSiCfgReq failed\n");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)cfgReqInfo,
                                   (Size)sizeof(*cfgReqInfo));
      rgSCHUtlRgrSiCfgCfm(instId, spId, transId, cfmStatus); 
      RETVALUE(RFAILED);
   }

   /* Handle configuration */
   ret = rgSCHGomHndlSiCfg(pst->region, pst->pool,
            &rgSchCb[(pst->dstInst - 1)], spId, transId, 
                        cfgReqInfo);
   if (ret != ROK)
   {
      RGSCHDBGERR(instId,(rgSchPBuf(instId),"RgUiRgrSiCfgReq:"
                   "Configuration Request Handling Failed \n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* RgUiRgrSiCfgReq */
#endif /*RGR_SI_SCH */
#if 0
/************** MAC SCH Interface **************/ 
/**
 * @brief Function for updating dedicated channel BO at scheduler from MAC.
 *
 * @details
 *
 *     Function : rgMacSchDedBoUpdtReq
 *     
 *     This function shall be invoked whenever MAC gets staRsp from RLC
 *     Scheduler shall update BO in its dedicated logical channel control block.
 *           
 *  @param[in] Pst*           pst
 *  @param[in] S16            cellSapId
 *  @param[in] RgInfDedBoRpt  *boRpt
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 RgMacSchDedBoUpdtReq
(
Pst*           pst,
RgInfDedBoRpt  *boRpt
)
#else
PUBLIC S16 RgMacSchDedBoUpdtReq(pst, boRpt)
Pst*           pst;
RgInfDedBoRpt  *boRpt;
#endif
{
   RgSchCellCb   *cell;
   RgSchUeCb     *ue;

   Inst          inst = (pst->dstInst - RGSCH_INST_START);
   S16           cellSapId = boRpt->cellSapId;

   TRC3(RgMacSchDedBoUpdtReq)

   RGSCHDBGPRM(inst,(rgSchPBuf(inst), "rgMacSchDedBoUpdtReq():"
             " boRpt->cellId = %u; boRpt->rnti = %u,"
             " boRpt->lcId = %u\n",boRpt->cellId, boRpt->rnti, boRpt->lcId));

   /* No need to chk for cell being NULL as MAC wouldn't have found instance if
    * it doesnt exist */
   cell = rgSchCb[inst].rgrSap[cellSapId].cell;

   /* rg005.201 Removing validation for optimization */
#ifndef NO_ERRCLS
   if (cell->cellId != boRpt->cellId)
   {
      /* Handle Cell fetch failure */
      RGSCHLOGERROR(inst, ERRCLS_INT_PAR,ERG009,(ErrVal)boRpt->cellId,
                            "rgMacSchDedBoUpdtReq(): Invalid cell Id");
      RETVALUE(RFAILED);
   }
#endif

   /* Update Bo in the given logical channel of the UE */
   if ((ue = rgSCHDbmGetUeCb(cell, boRpt->rnti)) != NULLP)
   {
      RgSchDlLcCb   *dlLc;
      if ((dlLc = rgSCHDbmGetDlDedLcCb(ue, boRpt->lcId)) != NULLP)
      {
#ifdef LTE_L2_MEAS
        if((dlLc->bo == 0) &&(boRpt->bo != 0))
        {
           /* UE is active */
           dlLc->qciCb->dlUeCount++;
        }
        else if((boRpt->bo == 0) && (dlLc->qciCb->dlUeCount))
        {
          /* UE is inactive */
          dlLc->qciCb->dlUeCount--;
        }
#endif
         dlLc->bo = boRpt->bo;
         /* Update the scheduler */
         rgSCHUtlDlDedBoUpd(cell, ue, dlLc);
         RETVALUE(ROK);
      }
   }
   RGSCHDBGERR(inst,(rgSchPBuf(inst), 
               "\nrgMacSchDedBoUpdtReq(): Invalid UE(%d) or Lc Id(%d)",
                boRpt->rnti, boRpt->lcId));
   RETVALUE(RFAILED);

} /* end of rgMacSchDedBoUpdtReq */

#if 0
/**
 * @brief Function for updating common channel BO at scheduler from MAC.
 *
 * @details
 *
 *     Function : RgMacSchCmnBoUpdtReq
 *     
 *     This function shall be invoked whenever MAC gets staRsp from RLC
 *     Scheduler shall update BO in its common logical channel control block.
 *           
 *  @param[in] Pst*            pst
 *  @param[in] S16             cellSapId
 *  @param[in] RgInfCmnBoRpt  *boRpt
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 RgMacSchCmnBoUpdtReq
(
Pst*           pst,
RgInfCmnBoRpt  *boRpt
)
#else
PUBLIC S16 RgMacSchCmnBoUpdtReq(pst, boRpt)
Pst*           pst;
RgInfCmnBoRpt  *boRpt;
#endif
{
   RgSchCellCb   *cell;
   Inst          inst = (pst->dstInst - RGSCH_INST_START);
   S16           cellSapId = boRpt->cellSapId;

   TRC3(RgMacSchCmnBoUpdtReq)

   /* No need to chk for cell being NULL as MAC would not have found instance if
    * it doesnt exist */
   cell = rgSchCb[inst].rgrSap[cellSapId].cell;
   if (cell->cellId != boRpt->cellId)
   {
      RGSCHLOGERROR(inst,ERRCLS_INT_PAR,ERG010,(ErrVal)boRpt->cellId,
                           "RgMacSchCmnBoUpdtReq(): Invalid cell Id");
      RETVALUE(RFAILED);
   }

   /* handle status response on CCCH */
   if(boRpt->lcId == cell->dlCcchId)
   {
      RGSCHDBGINFO(inst,(rgSchPBuf(inst), "RgMacSchCmnBoUpdtReq(): \
                 BO update for CCCH\n"));
      rgSCHUtlHndlCcchBoUpdt(cell, boRpt); 
   }
   else
   {
      rgSCHUtlHndlBcchPcchBoUpdt(cell, boRpt); 
   }
   
   RETVALUE(ROK);
} /* end of RgMacSchCmnBoUpdtReq */
#endif
/**
 * @brief This API is used to send data indication to Scheduler instance from MAC.
 *
 * @details
 *
 *     Function : rgMacSchSfRecpInd
 *     
 *     This function shall be invoked whenever MAC gets datInd on TFU
 *     Scheduler shall act on the CEs and data that are received as part of
 *     datInd.
 *           
 *  @param[in] Pst*            pst
 *  @param[in] S16             cellSapId
 *  @param[in] CmLteRnti       rnti
 *  @param[in] DatIndInfo      *datInd
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 RgMacSchSfRecpInd
(
Pst*             pst,
RgInfSfDatInd    *subfrmInfo
)
#else
PUBLIC S16 RgMacSchSfRecpInd(pst, subfrmInfo)
Pst*             pst;
RgInfSfDatInd    *subfrmInfo;
#endif
{
   S16               ret = RFAILED;
   RgSchErrInfo      err;
   RgSchUeCb         *ue;
   RgInfUeDatInd     *datInd;
   RgSchCellCb       *cell;
   Inst              inst = (pst->dstInst - RGSCH_INST_START);
   CmLteRnti         rnti;
   CmLListCp         *lnkLst;
   CmLList           *tmp;
   S16               cellSapId = subfrmInfo->cellSapId;
   
   TRC3(RgMacSchSfRecpInd)

   /* No need to chk for cell being NULL as MAC wouldn't have found instance if
    * it doesnt exist */
   cell = rgSchCb[inst].rgrSap[cellSapId].cell;

   /* lnkLst assignment */
   lnkLst = &(subfrmInfo->ueLst);

   CM_LLIST_FIRST_NODE(lnkLst, tmp);

   while((NULLP != tmp) && ((RgInfUeDatInd *)tmp->node != NULLP))
   {
      ue = NULLP;
      datInd   = (RgInfUeDatInd *)tmp->node;
      rnti     = datInd->rnti;

      /* We shall try and find
       * out the RaCb based on the following - 
       * 1. If the incoming PDU contained a CCCH SDU i.e. this is message 3.
       * 2. If the incoming PDU contained a CRNTI control element, i.e. we should
       * have a ueCb also for this 
       */
      
      /* It could be that a non-msg3 pdu contains a CRNTI Control element. We
       * should check for CRNTI CE and if it exists the UECb must exist, also an
       * if the CRNTI in the CE and the one with which the message came in are
       * different we shall look for an raCb as well. 
       */
      if (datInd->ceInfo.bitMask & RGSCH_CRNTI_CE_PRSNT)
      {
         if (datInd->ceInfo.bitMask & RGSCH_CCCH_SDU_PRSNT)
         {
            err.errType = RGSCHERR_TOM_DATIND;
            RGSCHDBGERR (inst,(rgSchPBuf(inst),"\nrgSCHTomDatInd() Received MSG3 with CRNTI and also CCCH (%d)\n", 
                     datInd->ceInfo.ces.cRnti));
            RETVALUE(RFAILED);
         }
         ue = rgSCHDbmGetUeCb (cell, datInd->ceInfo.ces.cRnti);
         if (ue == NULLP)
         {
         /*rg008.201 - Added support for SPS*/
#ifdef LTEMAC_SPS 
         if((ue = rgSCHDbmGetSpsUeCb (cell, datInd->ceInfo.ces.cRnti)) == NULLP)
#endif
 {
            err.errType = RGSCHERR_TOM_DATIND;
            RGSCHDBGERR (inst, (rgSchPBuf(inst),"\nrgSCHTomDatInd() Received MSG3 with CRNTI, unable to find ueCb for rnti(%d)\n", 
                     datInd->ceInfo.ces.cRnti));
            RETVALUE(RFAILED);
 }
         }

         if ((ret = rgSCHUtlProcMsg3 (subfrmInfo, cell, ue, 
               rnti, datInd, &err)) != ROK)
         { 
            /* rg009.201 Added Insure Fixes Of mem leak. Freed subfrmInfo */
            cmFreeMem(subfrmInfo);
            err.errType = RGSCHERR_TOM_DATIND;
            RGSCHDBGERR (inst, (rgSchPBuf(inst),"\nrgSCHTomDatInd() Processing for MSG3 failed for rnti(%d)\n", 
                     datInd->rnti));
            RETVALUE(RFAILED);
         }
         /* rg001.201 removed extra comments */
         /* fix */
         CM_LLIST_NEXT_NODE(lnkLst, tmp);
         continue;
      } /* end of CRNTI based message */
      else if (datInd->ceInfo.bitMask & RGSCH_CCCH_SDU_PRSNT)
      {
         if ((ret = rgSCHUtlProcMsg3 (subfrmInfo, cell, ue, 
               rnti, datInd, &err)) != ROK)
         { 
            /* rg009.201 Added Insure Fixes Of mem leak. Freed subfrmInfo */
            cmFreeMem(subfrmInfo);
            err.errType = RGSCHERR_TOM_DATIND;
            RGSCHDBGERR (inst,(rgSchPBuf(inst),"\nrgSCHTomDatInd() Processing for MSG3 failed for rnti(%d)\n", 
                     datInd->rnti));
            RETVALUE(RFAILED);
         }
         /* fix */
          CM_LLIST_NEXT_NODE(lnkLst, tmp);
          continue;
      } /* end of Msg3 processing */

      if (ue == NULLP)
      {
         ue = rgSCHDbmGetUeCb (cell, datInd->rnti);
         if (ue == NULLP)
         {
#ifdef LTEMAC_SPS 
         if((ue = rgSCHDbmGetSpsUeCb (cell, datInd->rnti)) == NULLP)
#endif
            {
               /* rg009.201 Added Insure Fixes Of mem leak. Freed subfrmInfo */
               cmFreeMem(subfrmInfo);
               err.errType = RGSCHERR_TOM_DATIND;
               RGSCHDBGERR (inst,(rgSchPBuf(inst),"rgSCHTomDatInd() Unable to get the UE CB for RNTI (%d) \n", 
               datInd->rnti));
               RETVALUE(RFAILED);
            }
         }
      }
      /* Just copy the timing information from the dat indication into the one
       * stored in the UE CB, will be later utilized to handle Timing advance 
       */

      if ((ret = rgSCHUtlUpdSch (subfrmInfo, cell, ue, datInd, &err)) != ROK)
      {
         /* rg009.201 Added Insure Fixes Of mem leak. Freed subfrmInfo */
         cmFreeMem(subfrmInfo);
         err.errType = RGSCHERR_TOM_DATIND;
         RGSCHDBGERR (inst, (rgSchPBuf(inst),"rgSCHTomDatInd() Unable to handle Data"
                   " Indication\n"));
         RETVALUE(RFAILED);
      }

      CM_LLIST_NEXT_NODE(lnkLst, tmp);
   }
   /* rg001.201: chk if Sch needs to dealloc datInd after unpk */
   cmFreeMem(subfrmInfo);
   RETVALUE(ret);
}  /* rgMacSchSfRecpInd */
#endif
#ifdef LTEMAC_SPS
/**
 * @brief Function to handle relInd from MAC for a UE
 *
 * @details
 *
 *     Function: RgMacSchSpsRelInd
 *
 *     Handler for processing relInd for UL SPS of a UE
 *
 *     Invoked by: 
 *         MAC
 *
 *     Processing Steps:
 *           
 *  @param[in] Pst               *pst
 *  @param[in] RgInfSpsRelInfo   *relInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgMacSchSpsRelInd
(
Pst                *pst, 
RgInfSpsRelInfo    *relInfo
)
#else
PUBLIC S16 RgMacSchSpsRelInd(pst, relInfo)
Pst                *pst;
RgInfSpsRelInfo    *relInfo;
#endif
{
   RgSchUeCb       *ue;
   RgSchCellCb     *cell;
   Inst            inst = (pst->dstInst - RGSCH_INST_START);

   TRC2(RgMacSchSpsRelInd);

   /* No need to chk for cell being NULL as MAC wouldn't have found instance if
    * it doesnt exist */
   cell = rgSchCb[inst].rgrSap[relInfo->cellSapId].cell;

   if ((ue = rgSCHDbmGetUeCb(cell, relInfo->cRnti)) == NULLP)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst),
           "RgMacSchSpsRelInd:No Ue exists with ueId=%d\n",
                                           relInfo->cRnti));
      RETVALUE(RFAILED);
   }

   if ((rgSCHUtlSpsRelInd(cell, ue, relInfo->isExplRel)) != ROK)
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst),
           "RgMacSchSpsRelInd:RelInd processing for Ue(%d) failed\n",
                                           relInfo->cRnti));
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of RgMacSchSpsRelInd */
#endif /* LTEMAC_SPS */

#ifdef LTE_L2_MEAS

/**
 * @brief Function to handle L2MeasCfm from MAC
 *
 * @details
 *
 *     Function: RgMacSchL2MeasCfm
 *
 *     Handler for processing L2 measurement confirm 
 *
 *     Invoked by: 
 *         MAC
 *
 *     Processing Steps:
 *           
 *  @param[in] Pst               *pst
 *  @param[in] RgInfL2MeasCfm    *measCfm
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgMacSchL2MeasCfm
(
Pst                *pst, 
RgInfL2MeasCfm     *measCfm
)
#else
PUBLIC S16 RgMacSchL2MeasCfm(pst, measCfm)
Pst                *pst;
RgInfL2MeasCfm     *measCfm;
#endif
{
   RgSchCellCb       *cell = NULLP;
   Inst              inst = (pst->dstInst - RGSCH_INST_START);
   CmLList           *lnk;
   RgSchL2MeasCb     *measCb;
   RgSchCb           *instCb =  &rgSchCb[inst];
   U32               idx;
   LrgSchMeasCfmInfo schMeasCfm;
   U8                qciVal;

   TRC2(RgMacSchL2MeasCfm);

   /* Find the cellCb using cellId in measInfo. Iterate through all cells
    * in rgrsapCb in RgschCb */
   for (idx = 0; idx < instCb->numSaps; idx++)
   {
      if ( instCb->rgrSap[idx].cell->cellId == measCfm->cellId)
      {
         /* got the cell break the loop */
         cell = instCb->rgrSap[idx].cell;
         break;
      }
   }
   /* If no cellCb return Err with Invalid Cell Id */
   if (cell == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst), "\nRgMacSchL2MeasCfm():"
                  "Meas Cfm Failed.Invalid Cell Id\n"));
      RETVALUE(RFAILED);
   }
   
   measCb = NULLP;

   /* Iterate through all meas requests in l2mList in cellCb */
   lnk = cell->l2mList.first;
   while(lnk != NULLP)
   {
      /* Get the MeasCb : RgSchL2MeasCb */
      measCb = (RgSchL2MeasCb *)lnk->node;
      lnk = lnk->next;
      if (measCb->measReq.hdr.transId == measCfm->transId)
      {
         break;
      }
   }
   if ( measCb == NULLP )
   {
      RETVALUE( RFAILED );
   }


   if(measCfm->cfm.status != LCM_PRIM_OK)
   {
      for (idx = 0; idx < measCb->measReq.avgPrbQciUl.numQci; idx++)
      {
        qciVal = measCb->measReq.avgPrbQciUl.qci[idx];
        cell->qciArray[qciVal].qci = 0;
      }
      /* Delete this measCb, send the negative confirmation to
       * stack manager */
      cmLListDelFrm(&cell->l2mList, &measCb->measLnk);
      /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&measCb, sizeof(RgSchL2MeasCb));
      cmMemset((U8 *)&schMeasCfm, 0, sizeof(LrgSchMeasCfmInfo));
      schMeasCfm.measType     = measCfm->measType;
      schMeasCfm.cfm          = measCfm->cfm;
      schMeasCfm.hdr.transId  = measCfm->transId;
      schMeasCfm.cellId       = measCfm->cellId;
      RgMiLrgSchL2MeasCfm(&(instCb->rgSchInit.lmPst), &schMeasCfm);
      RETVALUE(ROK);
   }
#if 0   
   for(idx = 0; idx < measCfm->u.prbCfm.numQci; idx++)
   {
      /*measCb->avgPrbQciUl.prbPercQci[idx] = measCfm->u.prbCfm.prbUsage[idx]; */
      measCb->avgPrbQciUl.prbPercQci[idx].prbPercQci = measCfm->u.prbCfm.prbUsage[idx].prbUsage;
      measCb->avgPrbQciUl.prbPercQci[idx].qciValue = measCfm->u.prbCfm.prbUsage[idx].qciValue;
   }
#endif   
   measCb->avgPrbQciUl.numQci = measCfm->u.prbCfm.numQci;
   measCb->cfmRcvd = TRUE;
   RETVALUE(ROK);
} /* end of RgMacSchL2MeasCfm */
#endif

/************** TFU Interface *************/

/**
 * @brief Bind confirm API for TFU SAP on scheduler instance. 
 *
 * @details
 *
 *     Function : RgLiTfuSchBndCfm
 *      
 *      This API is invoked by PHY to confirm TFU SAP bind. 
 *     
 *           
 *  @param[in]  Pst   *pst 
 *  @param[in]  SuId  suId 
 *  @param[in]  U8    status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuSchBndCfm 
(
Pst     *pst,
SuId    suId, 
U8      status
)
#else
PUBLIC S16 RgLiTfuSchBndCfm(pst, suId, status)
Pst     *pst; 
SuId    suId; 
U8      status;
#endif
{
   S16 ret;
   RgSchLowSapCb  *tfuSap;
   Inst  instId = pst->dstInst - RGSCH_INST_START;

   TRC3(RgLiTfuSchBndCfm);

   RGSCHDBGPRM(instId,(rgSchPBuf(instId),"RgLiTfuSchBndCfm SuId(%d), Status(%d)\n", 
         suId, status));

   if(suId >= rgSchCb[instId].numSaps)
   {
      RGSCHDBGERR(instId, (rgSchPBuf(instId),"Incorrect SuId"));
      RETVALUE(RFAILED);
   }
   /* Lets validate suId first */
   tfuSap = &(rgSchCb[instId].tfuSap[suId]);

   if (suId != tfuSap->sapCfg.suId)
   {
      RGSCHDBGERR(instId, (rgSchPBuf(instId),"Incorrect SuId. Configured (%d)\
            Recieved (%d)\n", tfuSap->sapCfg.suId, suId));
      RETVALUE(RFAILED);
   }
   ret = rgSCHLmmBndCfm (pst, suId, status);
   RETVALUE(ret);
}  /* RgLiTfuSchBndCfm */
#if 0
/**
 * @brief Random Access Request indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuRaReqInd
 *      
 *      This API is invoked by PHY to send Random Access Request to Scheduler.
 *      This API contains information for Random Access Request including 
 *      raRnti, list of associated RAPIDs and related information.
 *           
 *  @param[in]  Pst              *pst 
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuRaReqIndInfo  *raReqInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuRaReqInd
(
Pst              *pst, 
SuId             suId, 
TfuRaReqIndInfo  *raReqInd
)
#else
PUBLIC S16 RgLiTfuRaReqInd(pst, suId, raReqInd)
Pst              *pst; 
SuId             suId; 
TfuRaReqIndInfo  *raReqInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuRaReqInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuRaReqInd() SAP Validation failed SuId(%d).\n", suId));
      /* Free up the memory for the request structure */
      SPutSBuf (pst->region, pst->pool, (Data *)raReqInd,
            sizeof(TfuRaReqIndInfo));
      RETVALUE(ret);
   }

   if(raReqInd == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"Invalid input pointer for raReqInd:RgLiTfuRaReqInd Failed"));
      RETVALUE(RFAILED);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuRaReqInd()No cell exists\n"));
      RETVALUE(RFAILED);
   }

   ret = rgSCHTomRaReqInd(rgSchCb[inst].tfuSap[suId].cell, raReqInd);
   /* Free up the memory for the request structure */
   cmFreeMem((Ptr)raReqInd);
   /*SPutSBuf (pst->region, pst->pool, (Data *)raReqInd,
         sizeof(TfuRaReqIndInfo)); */
   RETVALUE(ret);
}  /* RgLiTfuRaReqInd */

/**
 * @brief Uplink CQI indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuUlCqiInd
 *      
 *      This API is invoked by PHY to send Uplink CQI to Scheduler.
 *      This API contains Uplink CQI information reported per UE.
 *           
 *  @param[in]  Pst               *pst 
 *  @param[in]  SuId              suId 
 *  @param[in]  TfuUlCqiIndInfo   *ulCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuUlCqiInd
(
Pst              *pst, 
SuId             suId, 
TfuUlCqiIndInfo  *ulCqiInd
)
#else
PUBLIC S16 RgLiTfuUlCqiInd(pst, suId, ulCqiInd)
Pst              *pst; 
SuId             suId; 
TfuUlCqiIndInfo  *ulCqiInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuUlCqiInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuUlCqiInd() SAP Validation failed.\n"));
      /* Free up the memory for the request structure */
      cmFreeMem((Ptr)ulCqiInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuUlCqiInd()No cell exists\n"));
      RETVALUE(RFAILED);
   }
   ret = rgSCHTomUlCqiInd (rgSchCb[inst].tfuSap[suId].cell, ulCqiInd);
   /* Free up the memory for the request structure */
   cmFreeMem((Ptr)ulCqiInd);
   RETVALUE(ret);
}  /* RgLiTfuUlCqiInd */

/**
 * @brief PUCCH power adjustment indication.
 *
 * @details
 *
 *     Function : RgLiTfuPucchDeltaPwrInd
 *
 *      This API is invoked by PHY to send PUCCH power adjustment
 *      indication.
 *
 *  @param[in]  Pst                     *pst 
 *  @param[in]  SuId                    suId 
 *  @param[in]  TfuPucchDeltaPwrIndInfo *pucchDeltaPwr
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuPucchDeltaPwrInd
(
Pst                     *pst,
SuId                    suId,
TfuPucchDeltaPwrIndInfo *pucchDeltaPwr
)
#else
PUBLIC S16 RgLiTfuPucchDeltaPwrInd(pst, suId, pucchDeltaPwr)
Pst                     *pst;
SuId                    suId;
TfuPucchDeltaPwrIndInfo *pucchDeltaPwr;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuPucchDeltaPwrInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuPucchDeltaPwrInd() SAP Validation failed.\n"));
      /* Free up the memory for the request structure */
      cmFreeMem((Ptr)pucchDeltaPwr);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuPucchDeltaPwrInd()No cell exists\n"));
      RETVALUE(RFAILED);
   }
   ret = rgSCHTomPucchDeltaPwrInd (rgSchCb[inst].tfuSap[suId].cell, pucchDeltaPwr);
   /* Free up the memory for the request structure */
   cmFreeMem((Ptr)pucchDeltaPwr);
   RETVALUE(ret);
}  /* RgLiTfuPucchDeltaPwrInd */


/**
 * @brief HARQ ACK indication from PHY for Downlink transmissions.
 *
 * @details
 *
 *     Function : RgLiTfuHqInd
 *      
 *      This API is invoked by PHY to send HARQ ACK information to Scheduler
 *      on recieving HARQ ACK/NACK from UEs.
 *      This API contains HARQ ACK information recieved by PHY for downlink
 *      transmissions.
 *           
 *  @param[in]  Pst                *pst
 *  @param[in]  SuId               suId 
 *  @param[in]  TfuHqIndInfo       *harqAckInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuHqInd
(
Pst                *pst, 
SuId               suId, 
TfuHqIndInfo       *harqAckInd
)
#else
PUBLIC S16 RgLiTfuHqInd(pst, suId, harqAckInd)
Pst                *pst; 
SuId               suId; 
TfuHqIndInfo       *harqAckInd;
#endif
{
   S16   ret;
   Inst  inst = (pst->dstInst - RGSCH_INST_START);

   TRC3(RgLiTfuHqInd);

   /* rg005.201 Removing validation for optimization */
#ifndef NO_ERRCLS
   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuHarqAckInd() SAP Validation failed.\n"));
      cmFreeMem((Ptr)harqAckInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuHarqAckInd()No cell exists\n"));
      RETVALUE(RFAILED);
   }
#endif

   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHTomHarqAckInd (rgSchCb[inst].tfuSap[suId].cell, harqAckInd);
   /* Free up the memory for the request structure */
   cmFreeMem((Ptr)harqAckInd);
   RETVALUE(ret);
}  /* RgLiTfuHqInd */


/**
 * @brief Scheduling request(SR) indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuSrInd
 *      
 *      This API is invoked by PHY to send Scheduling request information to
 *      Scheduler on recieving SR from a list of UEs.
 *      This API contains scheduling request information recieved by PHY for UEs.
 *           
 *  @param[in]  Pst           *pst
 *  @param[in]  SuId          suId 
 *  @param[in]  TfuSrIndInfo  *srInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuSrInd
(
Pst                *pst, 
SuId               suId, 
TfuSrIndInfo       *srInd
)
#else 
PUBLIC S16 RgLiTfuSrInd(pst, suId, srInd)
Pst                *pst; 
SuId               suId; 
TfuSrIndInfo       *srInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuSrInd);

   /* rg005.201 Removing validation for optimization */
#ifndef NO_ERRCLS
   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuSrInd() SAP Validation failed.\n"));
      cmFreeMem((Ptr)srInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuSrInd()No cell exists\n"));
      RETVALUE(RFAILED);
   }
#endif
   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHTomSrInd (rgSchCb[inst].tfuSap[suId].cell, srInd);
   /* Free up the memory for the request structure */
   cmFreeMem((Ptr)srInd);
   RETVALUE(ret);
}  /* RgLiTfuSrInd */


/**
 * @brief Downlink CQI indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuDlCqiInd
 *      
 *      This API is invoked by PHY to send Downlink CQI indication to Scheduler
 *      on recieving downlink CQI from UE.
 *      This API contains downlink CQI information recieved by PHY for an UE.
 *           
 *  @param[in]  Pst              *pst
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuDlCqiIndInfo  *dlCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuDlCqiInd
(
Pst                *pst, 
SuId               suId, 
TfuDlCqiIndInfo    *dlCqiInd
)
#else
PUBLIC S16 RgLiTfuDlCqiInd(pst, suId, dlCqiInd)
Pst                *pst; 
SuId               suId; 
TfuDlCqiIndInfo    *dlCqiInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuDlCqiInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuDlCqiInd() SAP Validation failed.\n"));
      /* Free up the memory for the request structure */
      cmFreeMem((Ptr)dlCqiInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuDlCqiInd()No cell exists\n"));
      RETVALUE(RFAILED);
   }
   ret = rgSCHTomDlCqiInd (rgSchCb[inst].tfuSap[suId].cell, dlCqiInd);
   /* Free up the memory for the request structure */
   cmFreeMem((Ptr)dlCqiInd);
   RETVALUE(ret);
}  /* RgLiTfuDlCqiInd */
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE

/**
 * @brief Raw CQI indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuRawCqiInd
 *      
 *      This API is invoked by PHY to send Raw CQI indication to Scheduler
 *      on receiving Raw CQI from UE.
 *      This API contains Raw CQI information recieved by PHY for an UE.
 *           
 *  @param[in]  Pst              *pst
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuRawCqiIndInfo  *rawCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuRawCqiInd
(
Pst                *pst, 
SuId               suId, 
TfuRawCqiIndInfo    *rawCqiInd
)
#else
PUBLIC S16 RgLiTfuRawCqiInd(pst, suId, rawCqiInd)
Pst                *pst; 
SuId               suId; 
TfuRawCqiIndInfo    *rawCqiInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuRawCqiInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuRawCqiInd() SAP Validation failed.\n"));
      /* Free up the memory for the request structure */
      cmFreeMem((Ptr)rawCqiInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuRawCqiInd() No cell exists\n"));
      RETVALUE(RFAILED);
   }
   ret = rgSCHTomRawCqiInd (rgSchCb[inst].tfuSap[suId].cell, rawCqiInd);
   /* Free up the memory for the request structure */
   cmFreeMem((Ptr)rawCqiInd);
   RETVALUE(ret);
}  /* RgLiTfuRawCqiInd */

/**
 * @brief SRS indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuSrsInd
 *      
 *      This API is invoked by PHY to send UL SRS indication to Scheduler
 *      on receiving a SRS from UE.
 *      This API contains SRS information recieved by PHY for an UE.
 *           
 *  @param[in]  Pst              *pst
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuSrsIndInfo  *srsInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuSrsInd
(
Pst                *pst, 
SuId               suId, 
TfuSrsIndInfo    *srsInd
)
#else
PUBLIC S16 RgLiTfuSrsInd(pst, suId, srsInd)
Pst                *pst; 
SuId               suId; 
TfuSrsIndInfo    *srsInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuSrsInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuSrsInd() SAP Validation failed.\n"));
      /* Free up the memory for the request structure */
      cmFreeMem((Ptr)srsInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuSrsInd() No cell exists\n"));
      RETVALUE(RFAILED);
   }
   ret = rgSCHTomSrsInd (rgSchCb[inst].tfuSap[suId].cell, srsInd);
   /* Free up the memory for the request structure */
   cmFreeMem((Ptr)srsInd);
   RETVALUE(ret);
}  /* RgLiTfuSrsInd */

#endif 
/* rg007.201 - Changes for MIMO feature addition */
#ifdef LTEMAC_MIMO
/**
 * @brief DOA indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuDoaInd
 *      
 *      This API is invoked by PHY to send Direction Of Arrival to Scheduler
 *      on calculating DOA at PHYSICAL LAYER for an UE.
 *      This API contains DOA information calculated by PHY for an UE.
 *           
 *  @param[in]  Pst              *pst
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuDoaIndInfo    *doaInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuDoaInd 
(
Pst                *pst, 
SuId               suId, 
TfuDoaIndInfo      *doaInd
)
#else
PUBLIC S16 RgLiTfuDoaInd(pst, suId, doaInd)
Pst                *pst; 
SuId               suId; 
TfuDoaIndInfo      *doaInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC2(RgLiTfuDoaInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuDoaInd() SAP Validation failed.\n"));
      /* Free up the memory for the request structure */
      cmFreeMem((Ptr)doaInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuDoaInd()No cell exists\n"));
      RETVALUE(RFAILED);
   }
   ret = rgSCHTomDoaInd (rgSchCb[inst].tfuSap[suId].cell, doaInd);
   /* Free up the memory for the request structure */
   cmFreeMem((Ptr)doaInd);
   RETVALUE(ret);
}  /* RgLiTfuDlCqiInd */
#endif

/**
 * @brief CRC indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuCrcInd
 *      
 *      This API is invoked by PHY to give CRC indication to scheduler.
 *           
 *  @param[in]  Pst               *pst
 *  @param[in]  SuId              suId 
 *  @param[in]  TfuCrcIndInfo *crcInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuCrcInd
(
Pst                *pst, 
SuId               suId, 
TfuCrcIndInfo  *crcInd
)
#else
PUBLIC S16 RgLiTfuCrcInd (pst, suId, crcInd)
Pst                *pst; 
SuId               suId; 
TfuCrcIndInfo  *crcInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;
#ifdef MSPD
   volatile int t = GetTIMETICK();
#endif

   TRC3(RgLiTfuCrcInd);

   /* rg005.201 Removing validation for optimization */
#ifndef NO_ERRCLS
   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuCrcInd() SAP Validation failed.\n"));
      cmFreeMem((Ptr)crcInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuCrcInd()No cell exists\n"));
      RETVALUE(RFAILED);
   }
#endif

   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHTomCrcInd(rgSchCb[inst].tfuSap[suId].cell, crcInd);
   /* Free up the memory for the request structure */
   cmFreeMem((Ptr)crcInd);
#ifdef MSPD_MLOG
   MLogTask(PID_UL_SCHEDULING, RESOURCE_LARM, t, GetTIMETICK());
#endif
   RETVALUE(ret);
}  /* RgLiTfuCrcInd */

/**
 * @brief Timing Advance indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuTimingAdvInd
 *      
 *      This API is invoked by PHY to indicate timing advance to Scheduler for
 *       an UE.
 *           
 *  @param[in]  Pst                  *pst
 *  @param[in]  SuId                 suId 
 *  @param[in]  TfuTimingAdvIndInfo  *timingAdvInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuTimingAdvInd
(
Pst                  *pst, 
SuId                 suId, 
TfuTimingAdvIndInfo  *timingAdvInd
)
#else
PUBLIC S16 RgLiTfuTimingAdvInd(pst, suId, timingAdvInd)
Pst                  *pst; 
SuId                 suId; 
TfuTimingAdvIndInfo  *timingAdvInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuTimingAdvInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuTimingAdvInd() SAP Validation failed.\n"));
      /* Free up the memory for the request structure */
      cmFreeMem((Ptr)timingAdvInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuTimingAdvInd()No cell exists\n"));
      RETVALUE(RFAILED);
   }
   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHTomTimingAdvInd(rgSchCb[inst].tfuSap[suId].cell, timingAdvInd);
   /* Free up the memory for the request structure */
   cmFreeMem((Ptr)timingAdvInd);
   RETVALUE(ret);
}  /* RgLiTfuTimingAdvInd */


/**
 * @brief Transmission time interval indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuSchTtiInd 
 *      
 *      This API is invoked by PHY to indicate TTI indication to Scheduler for
 *      a cell.
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  SuId           suId 
 *  @param[in]  TfuTtiIndInfo  *ttiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuSchTtiInd 
(
Pst                 *pst, 
SuId                suId, 
TfuTtiIndInfo       *ttiInd
)
#else
PUBLIC S16 RgLiTfuSchTtiInd(pst, suId, ttiInd)
Pst                 *pst; 
SuId                suId; 
TfuTtiIndInfo       *ttiInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;
   volatile int t;

   TRC3(RgLiTfuSchTtiInd);

   
   /* rg005.201 Removing the validation with every TTI - for optimization */
#ifndef NO_ERRCLS 
   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuSchTtiInd() SAP Validation failed.\n"));
      SPutSBuf (pst->region, pst->pool, (Data *)ttiInd,
            sizeof(TfuTtiIndInfo));
      RETVALUE(ret);
   }
#endif
   /*rg009.201:[ccpu00116677]: Moved check for cell outside ERRCLS*/

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RGSCHDBGERR(inst,(rgSchPBuf(inst),"RgLiTfuTtiInd()No cell exists\n"));
      RETVALUE(RFAILED);
   }
   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHTomTtiInd(rgSchCb[inst].tfuSap[suId].cell, suId, ttiInd);
   /* Free up the memory for the request structure */
   SPutSBuf (pst->region, pst->pool, (Data *)ttiInd,
         sizeof(TfuTtiIndInfo));

   RETVALUE(ret);
}  /* RgLiTfuSchTtiInd */
#endif

/**********************************************************************
 
         End of file:     gk_sch.c@@/main/3 - Sat Jul 30 02:21:34 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       apany 1. Initial Release.
/main/2      ---       apany 1. LTE MAC 2.1 release
             rg001.201 sm    1. Removed extra comments. 
             rg003.201 ns    1. Trace added for functions.
             rg004.201 ns    1. Memory Fix.
             rg004.201 rnt   1. Modified for SI Enhancement
             rg005.201 sm    1. Changes for optimization
             rg007.201   ap  1. Added support for MIMO
             rg008.201 rsharon 1. Added support for SPS.
             rg009.201 dv 1. Added changes of TFU_UPGRADE.
                          2. [ccpu00116677]: Moved Cell check 
                             outside ERRCLS.
             rg009.201 gj 3. [ccpu00116453] Added transId filling
                              in confirm messages
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
*********************************************************************91*/

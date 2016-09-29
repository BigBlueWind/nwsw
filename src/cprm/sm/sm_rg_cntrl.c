/**
 * @file  sm_rg_cntrl.c
 * @brief This file contains configuration functions for MAC
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */


/* header include files (.h) */
#include "ccpu_cmn.h"

/* RRM Related Files */
#include "ueh.h"
#include "log.h"

/*
 *
 *       Fun:    uehSmRgCfg - configure RG
 *
 *       Desc:
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ueh_sm_rg_cntrl.c
 *
 */


PUBLIC Void uehSmRgCfg(
  Void
  )
{
   logFunc("%s","uehSmRgCfg");

   smBuildRgGenCfg();
   smBuildRgUsapCfg(STRGUSAP);
   smBuildRgUsapCfg(STCRGSAP);
   smBuildRgSchInsCfg();
   /* Build the TFU sap configuration of LTE MAC */
   smBuildRgTfuLsapCfg();

   RETVOID;
} /* end of uehSmRgCfg */

/*
*
*       Fun:   smBuildRgGenCfg
*
*       Desc:  Invoked to build MAC GEN CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_rg_cntrl.c
*
*/

PUBLIC S16 smBuildRgGenCfg(
    void
  )
{
   RgMngmt       rgMngmt;
   RgGenCfg      *genCfg=NULLP;
   Pst           pst;

   logFunc("%s","smBuildRgGenCfg");

   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(&rgMngmt, sizeof(RgMngmt));

   genCfg   = &(rgMngmt.t.cfg.s.genCfg);

   /*----------- Fill General Configuration Parameters ---------*/
   genCfg->mem.region  = smCb.init.region;
   genCfg->mem.pool    = RG_POOL;
   genCfg->tmrRes     = 10;
   
   genCfg->lmPst.dstProcId = SM_SM_PROC;
   genCfg->lmPst.srcProcId = SM_RG_PROC;
   genCfg->lmPst.dstEnt    = ENTSM;
   genCfg->lmPst.dstInst   = 0;
   genCfg->lmPst.srcEnt    = ENTRG;
   genCfg->lmPst.srcInst   = 0;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = smCb.init.region;
   genCfg->lmPst.pool      = RG_POOL;
   genCfg->lmPst.selector  = SM_SELECTOR_LC;
   

   /* Fill Header */
   rgMngmt.hdr.msgType             = TCFG;
   rgMngmt.hdr.msgLen              = 0;
   rgMngmt.hdr.entId.ent           = ENTRG;
   rgMngmt.hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt.hdr.elmId.elmnt         = STGEN;
   rgMngmt.hdr.seqNmb              = 0;
   rgMngmt.hdr.version             = 0;
   rgMngmt.hdr.transId             = 0;
  
   rgMngmt.hdr.response.prior      = PRIOR0;
   rgMngmt.hdr.response.route      = RTESPEC;
   rgMngmt.hdr.response.mem.region = smCb.init.region;
   rgMngmt.hdr.response.mem.pool   = RG_POOL;
   rgMngmt.hdr.response.selector   = SM_SELECTOR_LC;
   
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s","MAC General Config request sent");

   /* Send the request to the RRC */
   SmMiLrgCfgReq(&pst, &rgMngmt);

   RETVALUE(ROK);
}

/*
*
*       Fun:   smBuildCrgUsapCfg
*
*       Desc:  Invoked to build USAP CFG.(CRG)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_rg_cntrl.c
*
*/

PUBLIC S16 smBuildCrgUsapCfg(
    void
  )
{
   RgMngmt     rgMngmt;
   RgUpSapCfg  *uSap = NULLP;
   Pst         pst;

   logFunc("%s","smBuildCrgUsapCfg");

   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(&rgMngmt, sizeof(RgMngmt));

   uSap   = &(rgMngmt.t.cfg.s.crgSap);
   
   uSap->mem.region = smCb.init.region;
   uSap->mem.pool   = RG_POOL;
   uSap->suId       = 0;
   uSap->spId       = 0;
   uSap->procId     = SM_NH_PROC;
   uSap->ent        = ENTNH;
   uSap->inst       = SM_INST_ZERO;
   uSap->prior      = PRIOR0;
   uSap->route      = RTESPEC;
   uSap->selector   = SM_SELECTOR_LC;

   /* Fill Header */
   rgMngmt.hdr.msgType             = TCFG;
   rgMngmt.hdr.entId.ent           = ENTRG;
   rgMngmt.hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt.hdr.elmId.elmnt         = STCRGSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s","MAC Upper Control SAP(STCRGSAP) Config request sent");

   /* Send the request to the RRC */
   SmMiLrgCfgReq(&pst, &rgMngmt);
   
   RETVALUE(ROK);
}

/*
*
*       Fun:   smBuildRguUsapCfg
*
*       Desc:  Invoked to build USAP CFG.(RGU)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_rg_cntrl.c
*
*/

PUBLIC S16 smBuildRguUsapCfg(
    Void
  )
{
   RgMngmt     rgMngmt;
   RgUpSapCfg  *uSap = NULLP;
   Pst         pst;

   logFunc("%s","smBuildRguUsapCfg");

   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(&rgMngmt, sizeof(RgMngmt));

   uSap   = &(rgMngmt.t.cfg.s.rguSap);
   
   uSap->mem.region = smCb.init.region;
   uSap->mem.pool = RG_POOL;
   uSap->suId       = 0;
   uSap->spId       = 0;
   uSap->procId     = SM_KW_PROC;
   uSap->ent        = ENTKW;
   uSap->inst       = SM_INST_ZERO;
   uSap->prior      = PRIOR0;
   uSap->route      = RTESPEC;
   uSap->selector   = SM_SELECTOR_TC;


   /* Fill Header */
   rgMngmt.hdr.msgType             = TCFG;
   rgMngmt.hdr.entId.ent           = ENTRG;
   rgMngmt.hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt.hdr.elmId.elmnt         = STRGUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s","MAC Upper Data SAP(STRGUSAP) Config request sent");

   /* Send the request to the RRC */
   SmMiLrgCfgReq(&pst, &rgMngmt);
   
   RETVALUE(ROK);
} /* end of smBuildRguUsapCfg */


/*
*
*       Fun:   smBuildRgUsapCfg
*
*       Desc:  Invoked to build USAP CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_rg_cntrl.c
*
*/

PUBLIC S16 smBuildRgUsapCfg(
  U8  elemId
  )
{
   logFunc("%s","smBuildRgUsapCfg");

   switch (elemId)
   {
      case STRGUSAP:
         smBuildRguUsapCfg();
         break;

      case STCRGSAP:
         smBuildCrgUsapCfg();
         break;

      default:
         logError("MAC has only two upper SAP's, invalid SAP id:%d", elemId );
         break;
   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   smBuildRgSchInsCfg
*
*       Desc:  Invoked to build Scheduler Instance configuration.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_rg_cntrl.c
*
*/

PUBLIC S16 smBuildRgSchInsCfg(
    Void
  )
{
   RgMngmt       rgMngmt;
   RgSchInstCfg  *cfg = NULLP;
   Pst           pst;
   /* Instance Id 1 is used for scheduler */
   Inst          instId = SCH_INST_ID;
   U8            numSaps = 1;

   logFunc("%s","smBuildRgSchInsCfg");

   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(&rgMngmt, sizeof(RgMngmt));

   cfg = &(rgMngmt.t.cfg.s.schInstCfg);
 
   /* Filling of Instance Id */
   cfg->instId = instId;
   /* Filling of Gen config */
   cfg->genCfg.mem.region = smCb.init.region;
   cfg->genCfg.mem.pool = RG_POOL;
   cfg->genCfg.tmrRes = 10;

   cfg->genCfg.lmPst.dstProcId = SM_SM_PROC;
   cfg->genCfg.lmPst.srcProcId = SM_RG_PROC;
   cfg->genCfg.lmPst.dstEnt    = ENTSM;
   cfg->genCfg.lmPst.dstInst   = 0;
   cfg->genCfg.lmPst.srcEnt    = ENTRG;
   cfg->genCfg.lmPst.srcInst   = SCH_INST_ID;
   cfg->genCfg.lmPst.prior     = PRIOR0;
   cfg->genCfg.lmPst.route     = RTESPEC;
   cfg->genCfg.lmPst.region    = smCb.init.region;
   cfg->genCfg.lmPst.pool      = RG_POOL;
   cfg->genCfg.lmPst.selector  = SM_SELECTOR_LC;

   /* Filling of Number of RGR and TFU saps. Both are same and is 1 */
   cfg->numSaps = numSaps;

   /* Filling the RGR sap information */
   cfg->rgrSap[0].selector  = SM_SELECTOR_LC;
   cfg->rgrSap[0].mem.region= smCb.init.region;
   cfg->rgrSap[0].mem.pool  = RG_POOL;
   cfg->rgrSap[0].prior     = PRIOR0;
   cfg->rgrSap[0].procId    = SM_NX_PROC;
   cfg->rgrSap[0].ent       = ENTNX;
   cfg->rgrSap[0].inst      = SM_INST_ZERO;
   cfg->rgrSap[0].route     = RTESPEC;
   cfg->rgrSap[0].spId      = 0;
   cfg->rgrSap[0].suId      = 0;

   /* Filling the TFU sap information of the scheduler */
  
#ifdef CL_MAC_LWLC
   cfg->tfuSap[0].selector  = SM_SELECTOR_TC;
#else
   cfg->tfuSap[0].selector  = SM_SELECTOR_LWLC;
#endif
   cfg->tfuSap[0].mem.region= smCb.init.region;
   cfg->tfuSap[0].mem.pool  = RG_POOL;
   cfg->tfuSap[0].prior     = PRIOR0;
   cfg->tfuSap[0].procId    = SM_TF_PROC;
   cfg->tfuSap[0].ent       = ENTTF;
   cfg->tfuSap[0].inst      = SM_INST_ZERO;
   cfg->tfuSap[0].route     = RTESPEC;
   cfg->tfuSap[0].spId      = 1;
   cfg->tfuSap[0].suId      = 0;
   cfg->tfuSap[0].bndTmr.enb= TRUE;
   cfg->tfuSap[0].bndTmr.val= 10;
 
   /* Fill Header */
   rgMngmt.hdr.msgType             = TCFG;
   rgMngmt.hdr.entId.ent           = ENTRG;
   rgMngmt.hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt.hdr.elmId.elmnt         = STSCHINST;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.srcInst   = 0;
   pst.dstInst   = SCH_INST_ID;

   logInfo("%s","MAC, Schedular General Config request sent");

   SmMiLrgSchCfgReq(&pst, &rgMngmt);

   RETVALUE(ROK);
} /* end of smBuildRgSchInsCfg */


/*
*
*       Fun:   smBuildRgTfuLsapCfg
*
*       Desc:  Invoked to build LSAP RGU CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_rg_cntrl.c
*
*/

PUBLIC S16 smBuildRgTfuLsapCfg(
    Void
  )
{
   RgMngmt       rgMngmt;
   RgLowSapCfg   *lSap=NULLP;
   Pst           pst;

   logFunc("%s","smBuildRgTfuLsapCfg");

   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(&rgMngmt, sizeof(RgMngmt));

   lSap   = &(rgMngmt.t.cfg.s.tfuSap);
   
   lSap->mem.region  = smCb.init.region;
   lSap->mem.pool    = RG_POOL;
   
   lSap->procId      = SM_TF_PROC;
   lSap->ent         = ENTTF;
   lSap->inst        = SM_INST_ZERO;
   lSap->spId        = RG_YS_SPID;
   lSap->suId        = RG_YS_SUID;
   lSap->bndTmr.enb  = TRUE;
   lSap->bndTmr.val  = 10;
   lSap->prior       = PRIOR0;
   lSap->route       = RTESPEC;
#ifndef CL_MAC_LWLC   
   lSap->selector    = SM_SELECTOR_TC;
#else
   lSap->selector    = SM_SELECTOR_LWLC;
#endif
   
   /* Fill Header */
   rgMngmt.hdr.msgType             = TCFG;
   rgMngmt.hdr.entId.ent           = ENTRG;
   rgMngmt.hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt.hdr.elmId.elmnt         =  STTFUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s", "MAC: TFU lower SAP(STTFUSAP) Config request sent");

   /* Send the request to the MAC*/
   SmMiLrgCfgReq(&pst, &rgMngmt);
   
   RETVALUE(ROK);
}

/*
*
*       Fun:   smBindRgToTfuSap
*
*       Desc:  Invoked to bind MAC to TFU Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_rg_cntrl.c
*
*/

PUBLIC S16 smBindRgToTfuSap(
    Void
  )
{

   RgMngmt  rgMngmt;
   RgCntrl  *cntrl=NULLP;
   Pst      pst;
  
   logFunc("%s","smBindRgToTfuSap");
 
   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(&rgMngmt, sizeof(RgMngmt));

   cntrl = &(rgMngmt.t.cntrl);

   cntrl->action             =  ABND;
   cntrl->subAction          =  SM_ZERO_VAL;
   cntrl->s.rgSapCntrl.suId  =  SM_ZERO_VAL;
   cntrl->s.rgSapCntrl.spId  =  SM_ZERO_VAL;


   /* Fill Header */
   rgMngmt.hdr.msgType             = TCNTRL;
   rgMngmt.hdr.entId.ent           = ENTRG;
   rgMngmt.hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt.hdr.elmId.elmnt         = STTFUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "MAC: TFU lower SAP(STTFUSAP) Bind request sent");

   /* Send the request to the RRC */
   SmMiLrgCntrlReq(&pst, &rgMngmt);

   RETVALUE(ROK);
} /* end of smBindRgToTfuSap */

/*
*
*       Fun:   smBindRgSchToTfuSap
*
*       Desc:  Invoked to bind Scheduler to TFU Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_rg_cntrl.c
*
*/

PUBLIC S16 smBindRgSchToTfuSap
(
Void
)
{

   RgMngmt  rgMngmt;
   RgCntrl  *cntrl=NULLP;
   Pst      pst;

   logFunc("%s","smBindRgSchToTfuSap");
   
   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(&rgMngmt, sizeof(RgMngmt));

   cntrl = &(rgMngmt.t.cntrl);

   cntrl->action             =  ABND;
   cntrl->subAction          =  SM_ZERO_VAL;
   cntrl->s.rgSapCntrl.suId  =  SM_ZERO_VAL;
   cntrl->s.rgSapCntrl.spId  =  1;


   /* Fill Header */
   rgMngmt.hdr.msgType             = TCNTRL;
   rgMngmt.hdr.entId.ent           = ENTRG;
   rgMngmt.hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt.hdr.elmId.elmnt         = STTFUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.srcInst   = 0;
   pst.dstInst   = SCH_INST_ID;
   
   logInfo("%s", "MAC-SCH: TFU lower SAP(STTFUSAP) Bind request sent");

   /* Send the request to the RRC */
   SmMiLrgSchCntrlReq(&pst, &rgMngmt);

   RETVALUE(ROK);
} /* end of smBindRgSchToTfuSap */


/*
*
*       Fun:   smBuildRgGenCntrl
*
*       Desc:  Invoked to Enable Alarms at RRC
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_rg_cntrl.c
*
*/

PUBLIC S16 smBuildRgGenCntrl(
    Void
  )
{
   RgCntrl  *cntrl = NULLP;
   RgMngmt  rgMngmt;
   Pst      pst;
  
   logFunc("%s","smBuildRgGenCntrl");
 
   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(&rgMngmt, sizeof(RgMngmt));

   cntrl = &(rgMngmt.t.cntrl);

   cntrl->action        =  AENA;
   cntrl->subAction     =  SAUSTA;

   /* Fill Header */
   rgMngmt.hdr.msgType             = TCNTRL;
   rgMngmt.hdr.entId.ent           = ENTRG;
   rgMngmt.hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt.hdr.elmId.elmnt         = STGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;
   
  logInfo("%s","MAC Gen Cntrl Req sent ");

   /* Send the request to the RRC */
   SmMiLrgCntrlReq(&pst, &rgMngmt);

   RETVALUE(ROK);
} /* end of smBuildRgGenCntrl */

/*
*
*       Fun:   smBuildRgDbgCntrl
*
*       Desc:  Invoked to Enable Debug at MAC
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_rg_cntrl.c
*
*/

PUBLIC S16 smBuildRgDbgCntrl(
    LogConfig *logConfig
  )
{
   RgCntrl  *cntrl = NULLP;
   RgMngmt  rgMngmt;
   Pst      pst;
   U16      rgdbgMask =0;

   logFunc("%s","smBuildRgDbgCntrl");
   
   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(&rgMngmt, sizeof(RgMngmt));

   if( (logConfig->logMask1 & LOG_MASK_MSGIN_02 ) || (logConfig->logMask1 & LOG_MASK_MSGOUT_02 ) )
   {
      rgdbgMask = rgdbgMask | MAC_DBG_MOD;
   }
   if(logConfig->logMask1 & LOG_MASK_ERROR )
   {
      rgdbgMask = rgdbgMask | MAC_DBG_ERROR;
   }
   if(logConfig->logMask1 & LOG_MASK_DEBUG )
   {
      rgdbgMask = rgdbgMask | MAC_DBG_INFO;
   }
   logInfo(":%s: DBG-Mask: %02x",__func__, rgdbgMask);

   cntrl = &(rgMngmt.t.cntrl);


   cntrl->action        =  ADISIMM;
   cntrl->subAction     =  SADBG;
   cntrl->s.rgDbgCntrl.dbgMask = 0xffffffff;

   /* Fill Header */
   rgMngmt.hdr.msgType             = TCNTRL;
   rgMngmt.hdr.entId.ent           = ENTRG;
   rgMngmt.hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt.hdr.elmId.elmnt         = STGEN;
   rgMngmt.hdr.transId             = SM_MAC_DEBUG_TRANS_ID;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "Debug control request sent to MAC(RG)");

   /* Sending CNTRL-req for resetting the log level at MAC */
   SmMiLrgCntrlReq(&pst, &rgMngmt);

   cntrl->action        =  AENA;
   cntrl->s.rgDbgCntrl.dbgMask = rgdbgMask;
   /* Sending CNTRL-req for setting the actual log level at MAC */
   SmMiLrgCntrlReq(&pst, &rgMngmt);

   RETVALUE(ROK);
} /* end of smBuildRgGenCntrl */

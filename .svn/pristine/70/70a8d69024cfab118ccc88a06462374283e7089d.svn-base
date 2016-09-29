/**
 * @file  sm_kw_cntrl.c
 * @brief This file contains RLC configuration functions
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
 *       Fun:    uehSmKwCfg - configure KW
 *
 *       Desc:
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ueh_sm_kw_cntrl.c
 *
 */


PUBLIC Void uehSmKwCfg(
  Void
  )
{
   logFunc("%s","uehSmKwCfg");

   smBuildKwGenCfg();
   smBuildKwUsapCfg(STCKWSAP, ENTNH);
   smBuildKwUsapCfg(STKWUSAP, ENTNH);
   smBuildKwRguLsapCfg();

   RETVOID;
} /* end of uehSmKwCfg */



/*
*
*       Fun:   smBuildKwGenCfg
*
*       Desc:  Invoked to build RLC GEN CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_kw_cntrl.c
*
*/

PUBLIC S16 smBuildKwGenCfg(
    void
  )
{
   KwMngmt   kwMngmt;
   KwGenCfg  *genCfg = NULLP;
   Pst       pst;

   logFunc("%s","smBuildKwGenCfg");

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   genCfg   = &(kwMngmt.t.cfg.s.gen);

   /*----------- Fill General Configuration Parameters ---------*/
   genCfg->maxUe       = smCb.cfgCb.maxUeSupp;
   genCfg->maxKwuSaps  = 2;
   genCfg->timeRes     = 10;
   
   /*----------- Fill lmPst Parameters ---------*/
   genCfg->lmPst.dstProcId = SM_SM_PROC;
   genCfg->lmPst.srcProcId = SM_KW_PROC;
   genCfg->lmPst.dstEnt    = ENTSM;
   genCfg->lmPst.dstInst   = 0;
   genCfg->lmPst.srcEnt    = ENTKW;
   genCfg->lmPst.srcInst   = 0;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = smCb.init.region;
   genCfg->lmPst.pool      = KW_POOL;
   genCfg->lmPst.selector  = SM_SELECTOR_LC;
   
   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.msgLen              = 0;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STGEN;
   kwMngmt.hdr.seqNmb              = 0;
   kwMngmt.hdr.version             = 0;
   kwMngmt.hdr.transId             = 0;
   kwMngmt.hdr.response.prior      = PRIOR0;
   kwMngmt.hdr.response.route      = RTESPEC;
   kwMngmt.hdr.response.mem.region = smCb.init.region;
   kwMngmt.hdr.response.mem.pool   = smCb.init.pool;
   kwMngmt.hdr.response.selector   = SM_SELECTOR_LC;
   
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s","RLC General Config request sent");

   /* Send the request to the RLC */
   SmMiLkwCfgReq(&pst, &kwMngmt);

   RETVALUE(ROK);
} /* smBuildKwGenCfg */


/*
*
*       Fun:   smBuildCkwUsapCfg
*
*       Desc:  Invoked to build USAP CFG.(CKW)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_kw_cntrl.c
*
*/

PUBLIC S16 smBuildCkwUsapCfg(
    void
  )
{
   KwMngmt    kwMngmt;
   KwSapCfg   *uSap = NULLP;
   Pst       pst;

   logFunc("%s","smBuildCkwUsapCfg");

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   uSap   = &(kwMngmt.t.cfg.s.sap);
  
   uSap->selector =  SM_SELECTOR_LC;
   uSap->mem.region = smCb.init.region;
   uSap->mem.pool = KW_POOL;
   uSap->mem.spare = 0;
   uSap->procId = SM_NH_PROC;
   uSap->ent = ENTNH;
   uSap->inst = SM_INST_ZERO;
   uSap->sapId = 0;
   uSap->bndTmrIntvl = 2000;
   uSap->priority = PRIOR0;
   uSap->route = RTESPEC;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STCKWSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s","RLC Upper Control SAP(STCKWSAP) Config request sent");

   /* Send the request to the RLC */
   SmMiLkwCfgReq(&pst, &kwMngmt);
   
   RETVALUE(ROK);
} /* smBuildCkwUsapCfg */

/*
*
*       Fun:   smBuildKwuUsapCfg
*
*       Desc:  Invoked to build USAP CFG.(KWU -- NH/PJ)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_kw_cntrl.c
*
*/

PUBLIC S16 smBuildKwuUsapCfg(
    U8  ent
  )
{
   KwMngmt    kwMngmt;
   KwSapCfg   *uSap = NULLP;
   Pst        pst;

   logFunc("%s","smBuildKwuUsapCfg");

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   uSap   = &(kwMngmt.t.cfg.s.sap);

   uSap->selector   = SM_SELECTOR_LC;
   uSap->mem.region = smCb.init.region;
   uSap->mem.pool = KW_POOL;
   uSap->mem.spare = 0;
   
   if (ent == ENTNH)
   {
      uSap->procId      = SM_NH_PROC;
      uSap->ent         = ENTNH;
      uSap->sapId       = 0;
   }
   else
   {
      uSap->procId      = SM_PJ_PROC;
      uSap->ent         = ENTKW;
      uSap->sapId       = 1;
   }
   
   uSap->inst = SM_INST_ZERO;
   uSap->bndTmrIntvl = 1000;
   uSap->priority = PRIOR0;
   uSap->route = RTESPEC;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STKWUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s","RLC Upper Data SAP(STKWUSAP) Config request sent");

   /* Send the request to the RLC */
   SmMiLkwCfgReq(&pst, &kwMngmt);
   
   RETVALUE(ROK);
} /* end of smBuildKwuUsapCfg */

/*
*
*       Fun:   smBuildKwUsapCfg
*
*       Desc:  Invoked to build USAP CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_kw_cntrl.c
*
*/

PUBLIC S16 smBuildKwUsapCfg(
    U8  elemId,
    U8  ent
  )
{
   S16 ret = ROK;

   logFunc("%s","smBuildKwUsapCfg");

   switch (elemId)
   {
      case STCKWSAP:
      {
         smBuildCkwUsapCfg();
         break;
      }
      case STKWUSAP:
      {
         /* entity can be NH or PJ */
         smBuildKwuUsapCfg(ent);
         break;
      }
      default:
      {
         logError("RLC has only two upper SAP's, invalid SAP id:%d", elemId );
         ret = RFAILED;
         break;
      }
   }

   RETVALUE(ret);
}

/*
*
*       Fun:   smBuildKwRguLsapCfg
*
*       Desc:  Invoked to build LSAP RGU CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_kw_cntrl.c
*
*/

PUBLIC S16 smBuildKwRguLsapCfg(
  Void
  )
{
   KwMngmt    kwMngmt;
   KwSapCfg   *lSap = NULLP;
   Pst        pst;

   logFunc("%s","smBuildKwRguLsapCfg");

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(kwMngmt.t.cfg.s.sap);
  
   lSap->mem.region  = smCb.init.region;
   lSap->mem.pool    = KW_POOL;
   lSap->mem.spare   = 0;
   lSap->procId      = SM_RG_PROC;
   lSap->ent         = ENTRG;
   lSap->inst        = SM_INST_ZERO;
   lSap->sapId       = 0;
   lSap->bndTmrIntvl = 10;
   lSap->priority    = PRIOR0;
   lSap->route       = RTESPEC;
   lSap->selector    = SM_SELECTOR_TC;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STRGUSAP; 

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s", "RLC: MAC SAP(STRGUSAP) Config request sent");

   /* Send the request to the RRC */
   SmMiLkwCfgReq(&pst, &kwMngmt);
   
   RETVALUE(ROK);
}

/*
*
*       Fun:   smBindKwToRguSap
*
*       Desc:  Invoked to bind RLC to RGU Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_kw_cntrl.c
*
*/

PUBLIC S16 smBindKwToRguSap(
    Void
  )
{
   KwCntrl  *cntrl = NULLP;
   KwMngmt  kwMngmt;
   Pst      pst;
  
   logFunc("%s","smBindKwToRguSap");
 
   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(kwMngmt.t.cntrl);

   cntrl->action            =  ABND;
   cntrl->subAction         =  SM_ZERO_VAL;
   cntrl->s.sapCntrl.suId   =  SM_ZERO_VAL;
   cntrl->s.sapCntrl.spId   =  SM_ZERO_VAL;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCNTRL;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STRGUSAP; /* ambiguous defines in lkw.h and lrg.h so direct hardcoded*/

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "RLC: MAC SAP(STRGUSAP) Bind request sent");

   /* Send the request to the RRC */
   SmMiLkwCntrlReq(&pst, &kwMngmt);

   RETVALUE(ROK);
} /* smBindKwToRguSap */

/*
*
*       Fun:   smBuildKwDbgCntrl
*
*       Desc:  Invoked to enable debug prints
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_kw_cntrl.c
*
*/

PUBLIC S16 smBuildKwDbgCntrl(
    LogConfig *logConfig
  )
{
   KwCntrl  *cntrl = NULLP;
   KwMngmt  kwMngmt;
   Pst      pst;
   U32      kwdbgMask=0;
 
   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(logConfig->logMask1 & LOG_MASK_DEBUG )
   {
     kwdbgMask = kwdbgMask | KW_DBG_INFO;
   }
   if(logConfig->logMask1 & LOG_MASK_ERROR )
   {
     kwdbgMask = kwdbgMask | KW_DBG_ERROR;
   }
   logInfo(":%s: DBG-Mask: %02x",__func__, kwdbgMask);

   cntrl = &(kwMngmt.t.cntrl);

   cntrl->action            =  ADISIMM;
   cntrl->subAction         =  SADBG;
   cntrl->s.dbgCntrl.dbgMask = 0xffffffff;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCNTRL;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STGEN; 
   kwMngmt.hdr.transId             = SM_RLC_DEBUG_TRANS_ID;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "Debug control request sent to RLC(KW)");

   SmMiLkwCntrlReq(&pst, &kwMngmt);

   cntrl->action            =  AENA;
   cntrl->s.dbgCntrl.dbgMask = kwdbgMask;
   SmMiLkwCntrlReq(&pst, &kwMngmt);

   RETVALUE(ROK);

} /* smBuildKwDbgCntrl */

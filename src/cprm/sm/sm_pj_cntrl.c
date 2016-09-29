/**
 * @file  sm_pj_cntrl.c
 * @brief This file contains configuration functions for PDCP
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
 *       Fun:    uehSmPjCfg - configure PJ
 *
 *       Desc:
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ueh_sm_pj_cntrl.c
 *
 */
PUBLIC Void uehSmPjCfg(
  Void
  )
{
   logFunc("%s","uehSmPjCfg");

   smBuildPjGenCfg();
   smBuildPjCpjSapCfg();
   smBuildPjPjuSapCfg(UDH_PJ_SPID);
   smBuildPjPjuSapCfg(NH_PJ_SPID);

   RETVOID;
} /* end of uehSmPjCfg */

/*
*
*       Fun:   smBuildPjGenCfg
*
*       Desc:  Invoked to build PDCP GEN CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_pj_cntrl.c
*
*/
PUBLIC S16 smBuildPjGenCfg(
    void
  )
{
   PjMngmt       pjMngmt;
   PjGenCfg      *genCfg=NULLP;
   Pst           pst;

   logFunc("%s","smBuildPjGenCfg");

   SM_SET_ZERO(&pjMngmt, sizeof(PjMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   genCfg   = &(pjMngmt.t.cfg.s.gen);
   genCfg->maxPjuSaps = 2;
   genCfg->nmbUlBins  = 10;
   genCfg->nmbDlBins  = 10;
   genCfg->lmPst.dstProcId = SM_SM_PROC;
   genCfg->lmPst.srcProcId = SM_PJ_PROC;
   genCfg->lmPst.dstEnt    = ENTSM;
   genCfg->lmPst.dstInst   = 0;
   genCfg->lmPst.srcEnt    = ENTPJ;
   genCfg->lmPst.srcInst   = 0;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = smCb.init.region;
   genCfg->lmPst.pool      = PJ_POOL;
   genCfg->lmPst.selector  = SM_SELECTOR_LC;

#ifdef PJ_ASYNC_HOOK
   genCfg->obdPst.cmpPst.dstProcId = SM_SM_PROC;
   genCfg->obdPst.cmpPst.srcProcId = SM_PJ_PROC;
   genCfg->obdPst.cmpPst.dstEnt    = ENTSM;
   genCfg->obdPst.cmpPst.dstInst   = 0;
   genCfg->obdPst.cmpPst.srcEnt    = ENTPJ;
   genCfg->obdPst.cmpPst.srcInst   = 0;
   genCfg->obdPst.cmpPst.prior     = PRIOR0;
   genCfg->obdPst.cmpPst.route     = RTESPEC;
   genCfg->obdPst.cmpPst.region    = smCb.init.region;
   genCfg->obdPst.cmpPst.pool      = PJ_POOL;
   genCfg->obdPst.cmpPst.selector  = SM_SELECTOR_LC;

   genCfg->obdPst.secPst.dstProcId = SM_SM_PROC;
   genCfg->obdPst.secPst.srcProcId = SM_PJ_PROC;
   genCfg->obdPst.secPst.dstEnt    = ENTSM;
   genCfg->obdPst.secPst.dstInst   = 0;
   genCfg->obdPst.secPst.srcEnt    = ENTPJ;
   genCfg->obdPst.secPst.srcInst   = 0;
   genCfg->obdPst.secPst.prior     = PRIOR0;
   genCfg->obdPst.secPst.route     = RTESPEC;
   genCfg->obdPst.secPst.region    = smCb.init.region;
   genCfg->obdPst.secPst.pool      = PJ_POOL;
   genCfg->obdPst.secPst.selector  = SM_SELECTOR_LC;

   genCfg->obdTmrInt        = 50;
   genCfg->obdWtTmrInt      = 50;
#endif  /* PJ_ASYNC_HOOK */
   genCfg->nmbUlBins        = 100;
   genCfg->nmbDlBins        = 100;

   /* Fill Header */
   pjMngmt.hdr.msgType             = TCFG;
   pjMngmt.hdr.msgLen              = 0;
   pjMngmt.hdr.entId.ent           = ENTPJ;
   pjMngmt.hdr.entId.inst          = SM_INST_ZERO;
   pjMngmt.hdr.elmId.elmnt         = STGEN;
   pjMngmt.hdr.seqNmb              = 0;
   pjMngmt.hdr.version             = 0;
   pjMngmt.hdr.transId             = 0;

   pjMngmt.hdr.response.prior      = PRIOR0;
   pjMngmt.hdr.response.route      = RTESPEC;
   pjMngmt.hdr.response.mem.region = smCb.init.region;
   pjMngmt.hdr.response.mem.pool   = PJ_POOL;
   pjMngmt.hdr.response.selector   = SM_SELECTOR_LC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTPJ;
   pst.dstProcId = SM_PJ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;

   logInfo("%s","PDCP General Config request sent");

   /* Send the request to the RRC */
   SmMiLpjCfgReq(&pst, &pjMngmt);

   RETVALUE(ROK);
}


/*
*
*       Fun:   smBuildPjCpjSapCfg
*
*       Desc:  Invoked to build PDCP CPJ SAP Config (CPJ).
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_pj_cntrl.c
*
*/

PUBLIC S16 smBuildPjCpjSapCfg(
    void
  )
{
   PjMngmt       pjMngmt;
   PjSapCfg     *usap=NULLP;
   Pst           pst;

   logFunc("%s","smBuildPjCpjSapCfg");

   SM_SET_ZERO(&pjMngmt, sizeof(PjMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   usap   = &(pjMngmt.t.cfg.s.sap);

   usap->sapId        = 0;
   usap->procId      = SM_KW_PROC;
   usap->priority    = PRIOR0;
   usap->route       = RTESPEC;
   usap->selector    = SM_SELECTOR_LC;
   usap->ent         = ENTPJ;
   usap->inst        = SM_INST_ZERO;
   usap->mem.region  = smCb.init.region;
   usap->mem.pool    = PJ_POOL;
   usap->bndTmrIntvl = TRUE;


   /* Fill Header */
   pjMngmt.hdr.msgType             = TCFG;
   pjMngmt.hdr.msgLen              = 0;
   pjMngmt.hdr.entId.ent           = ENTPJ;
   pjMngmt.hdr.entId.inst          = SM_INST_ZERO;
   pjMngmt.hdr.elmId.elmnt         = STCPJSAP;
   pjMngmt.hdr.seqNmb              = 0;
   pjMngmt.hdr.version             = 0;
   pjMngmt.hdr.transId             = 0;
  
   pjMngmt.hdr.response.prior      = PRIOR0;
   pjMngmt.hdr.response.route      = RTESPEC;
   pjMngmt.hdr.response.mem.region = smCb.init.region;
   pjMngmt.hdr.response.mem.pool   = PJ_POOL;
   pjMngmt.hdr.response.selector   = SM_SELECTOR_LC;
   
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTPJ;
   pst.dstProcId = SM_PJ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;

   logInfo("%s","PDCP control SAP Config request sent");

   /* Send the request to the RRC */
   SmMiLpjCfgReq(&pst, &pjMngmt);

   RETVALUE(ROK);
} /* end of smBuildPjCpjSapCfg */

/*
*
*       Fun:   smBuildPjPjuSapCfg
*
*       Desc:  Invoked to build PDCP PJU SAP Config (PJU).
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_pj_cntrl.c
*
*/
PUBLIC S16 smBuildPjPjuSapCfg(
  SpId spId
  )
{
   PjMngmt       pjMngmt;
   PjSapCfg     *usap=NULLP;
   Pst           pst;

   logFunc("%s","smBuildPjPjuSapCfg");

   SM_SET_ZERO(&pjMngmt, sizeof(PjMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   usap   = &(pjMngmt.t.cfg.s.sap);

   usap->sapId        = spId;
   usap->procId      = SM_KW_PROC;
   usap->priority    = PRIOR0;
   usap->route       = RTESPEC;
   usap->selector    = SM_SELECTOR_LC;
   if(spId == UDH_PJ_SPID)
   {
      usap->ent = ENTUDH;
   }
   else if(spId == NH_PJ_SPID)
   {
      usap->ent = ENTNH;
   }
   else
   {
     logError("PDCP has only two user SAP's, Invalid spId:%d", spId);
   }
   usap->inst        = SM_INST_ZERO;
   usap->mem.region  = smCb.init.region;
   usap->mem.pool    = PJ_POOL;
   usap->bndTmrIntvl = TRUE;

   /* Fill Header */
   pjMngmt.hdr.msgType             = TCFG;
   pjMngmt.hdr.msgLen              = 0;
   pjMngmt.hdr.entId.ent           = ENTPJ;
   pjMngmt.hdr.entId.inst          = SM_INST_ZERO;
   pjMngmt.hdr.elmId.elmnt         = STPJUSAP;
   pjMngmt.hdr.seqNmb              = 0;
   pjMngmt.hdr.version             = 0;
   pjMngmt.hdr.transId             = 0;
  
   pjMngmt.hdr.response.prior      = PRIOR0;
   pjMngmt.hdr.response.route      = RTESPEC;
   pjMngmt.hdr.response.mem.region = smCb.init.region;
   pjMngmt.hdr.response.mem.pool   = PJ_POOL;
   pjMngmt.hdr.response.selector   = SM_SELECTOR_LC;
   
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTPJ;
   pst.dstProcId = SM_PJ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;

   logInfo("PDCP Data SAP Config request sent for spId:%d", spId);

   /* Send the request to the RRC */
   SmMiLpjCfgReq(&pst, &pjMngmt);

   RETVALUE(ROK);
} /* end of smBuildPjPjuSapCfg */

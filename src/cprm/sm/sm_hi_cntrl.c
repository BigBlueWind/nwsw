/**
 * @file  sm_hi_cntrl.c
 * @brief This file contains configuration functions for TUCL
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */

#include "ccpu_cmn.h"

/* RRM Related Files */
#include "ueh.h"
#include "log.h"

/*
 *
 *       Fun:    uehSmHiCfg - configure HI
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ueh_sm_hi_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void uehSmHiCfg
(  
Void
)
#else
PUBLIC Void uehSmHiCfg()
#endif /* ANSI */ 
{  
   logFunc("%s","uehSmHiCfg");

   smBuildHiGenCfg();
   smBuildHiTSapCfg(SB_HI_SPID);
   smBuildHiTSapCfg(EG_HI_SPID);

   RETVOID;
} /* end of uehSmHiCfg */

/*
 *
 *       Fun:    smBuildHiGenCfg - fill in default genCfg for HI
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  ueh_sm_hi_cntrl.c
 *
 */

PUBLIC Void smBuildHiGenCfg(
  Void
  )
{

   HiMngmt     hiMgt;
   HiGenCfg    *cfg = NULLP;
   Pst         pst;

   logFunc("%s","smBuildHiGenCfg");

   SM_SET_ZERO(&hiMgt, sizeof(HiMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &hiMgt.t.cfg.s.hiGen;

   cfg->numSaps = UEHSM_HI_NUMBSAPS;
   cfg->numCons = UEHSM_HI_NMBCONS;
   cfg->numFdsPerSet = UEHSM_HI_FDS;
   cfg->numFdBins = UEHSM_HI_FDBINS;

   cfg->selTimeout = UEHSM_HI_SELTIMEOUT;
   cfg->numRawMsgsToRead = 80;

#if (defined(HI_LKSCTP) && defined(HIAC_LKSCTP))
   cfg->numAssocBins   =  5;
   cfg->maxInitReTx    =  5; 
   cfg->maxAssocReTx   =  8;
   cfg->maxPathReTx    =  4; 
#endif

   cfg->permTsk = UEHSM_HI_PERM_TSK_FLAG;
   cfg->schdTmrVal = UEHSM_HI_SCHDTMR_VAL;
   cfg->poolStrtThr = UEHSM_HI_STRT_THRESH;
   cfg->poolDropThr = UEHSM_HI_DRP_THRESH;
   cfg->poolStopThr = UEHSM_HI_STP_THRESH;
   cfg->timeRes = UEHSM_HI_TIME_RES;
   cfg->initPool = HI_POOL;
#ifdef HI_TLS
   cfg->initOpenSSL = TRUE;
   cfg->numContexts = 2;
#ifdef HI_ACC_TLS_PRNG_FILE
   cfg->prng.option = LHI_OSSL_PRNG_FILE;
   cmMemcpy((U8 *)cfg.t.cfg.s.hiGen.prng.u.file, (U8 *)"/dev/urandom", 13);
#else
   cfg->prng.option =  LHI_OSSL_PRNG_SEED;
   for (i = 0;  i < LHI_OSSL_PRNG_SEED_LEN;  i++)
   {
      SRandom(&rnd);
      cfg->prng.u.seed[i] = (U8)rnd;
   }
#endif /* HI_ACC_TLS_PRNG_FILE */
#endif

   cfg->lmPst.srcProcId = SM_HI_PROC;
   cfg->lmPst.dstProcId = SM_SM_PROC;
   cfg->lmPst.srcEnt = (Ent)ENTHI;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;
   cfg->lmPst.prior = (Prior)UEHSM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = DFLT_REGION;
   cfg->lmPst.pool = HI_POOL;
   cfg->lmPst.selector = (Selector)UEHSM_HISMSEL;

 /* Fill Header */
   hiMgt.hdr.msgType             = TCFG;
   hiMgt.hdr.msgLen              = 0;
   hiMgt.hdr.entId.ent           = ENTHI;
   hiMgt.hdr.entId.inst          = SM_INST_ZERO;
   hiMgt.hdr.elmId.elmnt         = STGEN;
   hiMgt.hdr.elmId.elmntInst1    = 0;
   hiMgt.hdr.elmId.elmntInst2    = 0;
   hiMgt.hdr.elmId.elmntInst3    = 0;
   hiMgt.hdr.seqNmb              = 0;
   hiMgt.hdr.version             = 0;
   hiMgt.hdr.transId             = 0;

   hiMgt.hdr.response.prior      = PRIOR0;
   hiMgt.hdr.response.route      = RTESPEC;
   hiMgt.hdr.response.mem.region = DFLT_REGION;
   hiMgt.hdr.response.mem.pool   = HI_POOL;
   hiMgt.hdr.response.selector   = SM_SELECTOR_LC;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.srcProcId = SM_SM_PROC;
   pst.dstEnt    = ENTHI;
   pst.dstProcId = SM_HI_PROC;

   logInfo("%s", "TUCL General Config Request sent");

   (Void)SmMiLhiCfgReq(&pst, &hiMgt);
  
   RETVOID;
}


/*
 *
 *       Fun:    smBuildHiTSapCfg - fill in default HI SAP
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  ueh_sm_hi_cntrl.c
 *
 */

PUBLIC Void smBuildHiTSapCfg(
  SpId        spId
  )
{
   HiMngmt     hiMgt;
   HiSapCfg    *cfg;
   Pst         pst;

   logFunc("%s","smBuildHiTSapCfg");

   SM_SET_ZERO(&hiMgt, sizeof(HiMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &hiMgt.t.cfg.s.hiSap;

   cfg->spId = spId;
  
#if 0
   if(spId == EG_HI_SPID)
   {
     cfg->uiSel = 13;
   }
   else 
#endif
   {
      cfg->uiSel = 0;
   }
   cfg->flcEnb = FALSE;
   cfg->txqCongStrtLim = UEHSM_HI_CONG_STRT;
   cfg->txqCongDropLim  = UEHSM_HI_CONG_DRP;
   cfg->txqCongStopLim = UEHSM_HI_CONG_STP;
   cfg->numBins = UEHSM_HI_NMB_HLBINS;
     
   cfg->uiMemId.region =  DFLT_REGION;
   cfg->uiMemId.pool   =  HI_POOL;
   cfg->uiPrior        = PRIOR0;
   cfg->uiRoute        = RTESPEC;

    /* profile_1 : srvcType value = 3 (GTP header format) */
   cfg->hdrInf[0].hdrLen = 20;
   cfg->hdrInf[0].offLen = 2;
   cfg->hdrInf[0].lenLen = 2;
   cfg->hdrInf[0].flag   = 0;

   /* profile_2 : srvcType value = 4 (LDP header format) */
   cfg->hdrInf[1].hdrLen = 10;
   cfg->hdrInf[1].offLen = 2;
   cfg->hdrInf[1].lenLen = 2;
   cfg->hdrInf[1].flag   = 0;

#if (defined(HI_LKSCTP) && defined(HIAC_LKSCTP))
   cfg->type = HI_SCT_SAP;        
#endif

    /* Fill Header */
   hiMgt.hdr.msgType             = TCFG;
   hiMgt.hdr.msgLen              = 0;
   hiMgt.hdr.entId.ent           = ENTHI;
   hiMgt.hdr.entId.inst          = SM_INST_ZERO;
   hiMgt.hdr.elmId.elmnt         = STTSAP;
 
      /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTHI;
   pst.srcProcId = SM_SM_PROC;
   pst.dstProcId = SM_HI_PROC;

   logInfo("TUCL upper SAP Config Request sent for SAP Id:%d", spId);

   (Void)SmMiLhiCfgReq(&pst, &hiMgt);

   RETVOID;
} /* end of smBuildHiTSapCfg() */

/*
 *
 *       Fun:    smBuildHiDbgCntrl
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_eg_cntrl.c
 *
 */

PUBLIC Void smBuildHiDbgCntrl(
  LogConfig *logConfig
  )
{
   HiMngmt              hiMgt;
   Pst                  pst;
   U32                  hidbgMask = 0;

   logFunc("%s","smBuildHiDbgCntrl");

   SM_SET_ZERO(&hiMgt, sizeof(HiMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   hiMgt.hdr.msgType             = TCNTRL;
   hiMgt.hdr.entId.ent           = ENTHI;
   hiMgt.hdr.entId.inst          = SM_INST_ZERO;
   hiMgt.hdr.elmId.elmnt         = STGEN;
   hiMgt.hdr.transId             = SM_TUCL_DEBUG_TRANS_ID;

   if (logConfig->logMask1 & LOG_MASK_DEBUG)
     hidbgMask = hidbgMask | HI_DBG_INFO;
   logInfo(":%s: DBG-Mask: %02x",__func__,hidbgMask);
   hiMgt.t.cntrl.action          =  ADISIMM;
   hiMgt.t.cntrl.subAction       =  SADBG;
   hiMgt.t.cntrl.ctlType.hiDbg.dbgMask = 0xffffffff;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTHI;
   pst.dstProcId = SM_HI_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s", "Debug control request sent to TUCL(HI)");

   /* Sending CTRL-req for resetting the loglevel at TUCL */
   (Void)SmMiLhiCntrlReq(&pst, &hiMgt);

   hiMgt.t.cntrl.action          =  AENA;
   hiMgt.t.cntrl.ctlType.hiDbg.dbgMask = hidbgMask;
   /* Sending CTRL-req for setting the actual loglevel at TUCL */
   (Void)SmMiLhiCntrlReq(&pst, &hiMgt);

   RETVOID;
} /* end of smBuildHiDbgCntrl*/

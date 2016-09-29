/**
 * @file  sm_sb_cntrl.c
 * @brief This file contains configuration functions for SCTP
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

PRIVATE U8      uehSbCfg = 0;

/*
*
*       Fun:   Initialize SCTP task.
*
*       Desc:  Invoked to create SCTP TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_sb_cntrl.c
*
*/
PUBLIC S16 smSbInit
(
SSTskId    sysTskId
)
{
   logFunc("%s","smSbInit");

   /* Register SCTP TAPA Task */
   if (SRegTTsk((Ent)ENTSB, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 sbActvInit, (ActvTsk)sbActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach SCTP TAPA Task */
   if (SAttachTTsk((Ent)ENTSB, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }
  


  logInfo ("SCTP Tapa Task successfully registered\
		       and attached to %d",sysTskId);

   RETVALUE(ROK);
} /* end of smSbInit */

/*
 *
 *       Fun:    uehSmSbCfg - configure SB 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  ueh_sm_sb_cntrl.c
 *
 */


PUBLIC Void uehSmSbCfg
(
Void
)
{
#ifdef SS_MULTIPLE_PROCS
   U16   cnt;
#endif
   logFunc("%s","uehSmSbCfg");

   uehSbCfg = 0;
   smBuildSbGenCfg();
   smBuildSbTSapCfg(SB_HI_SPID, SB_HI_SUID);
   smBuildSbSctSapCfg(SZ_SB_SPID);

   RETVOID;
}


/*
 *
 *       Fun:    smBuildSbGenCfg - fill in default genCfg for SB
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
         File:   ueh_sm_sb_cntrl.c
 *
 */

PUBLIC Void smBuildSbGenCfg(
  Void
  )
{
   SbMgmt      sbMgt;
   SbGenCfg    *cfg = NULLP;
   Pst         pst;
   SctpConfig  *sctpConfig;
   //SctpConfig *sctpCfgTable;

   logFunc("%s","smSbInit");

   SM_SET_ZERO(&sbMgt, sizeof(SbMgmt));
   SM_SET_ZERO(&pst, sizeof(Pst));


   cfg = &sbMgt.t.cfg.s.genCfg;
   /*cfg->serviceType   = HI_SRVC_RAW_SCTP_PRIOR; */
       cfg->serviceType   = HI_SRVC_UDP;

#ifdef LSB5
   cfg->srcUdpPort = 9898;
   cfg->dstUdpPort = 9898;
#endif /*LSB5*/

   cfg->serviceType   = HI_SRVC_RAW_SCTP_PRIOR;
   sctpConfig         = &(smCb.cfgCb.sctpConfig);
   {
     cfg->maxNmbSctSaps          = sctpConfig->maxNmbSctSaps;
     cfg->maxNmbTSaps            = sctpConfig->maxNmbTSaps;
     cfg->maxNmbAssoc            = sctpConfig->maxNmbAssoc;
     cfg->maxNmbEndp             = sctpConfig->maxNmbEndp;
     cfg->maxNmbDstAddr          = sctpConfig->maxNmbDstAddr;
     cfg->maxNmbSrcAddr          = sctpConfig->maxNmbSrcAddr;
     cfg->maxNmbTxChunks         = sctpConfig->maxNmbTxChunks;
     cfg->maxNmbRxChunks         = sctpConfig->maxNmbRxChunks;
     cfg->maxNmbInStrms          = sctpConfig->maxNmbInStrms;
     cfg->maxNmbOutStrms         = sctpConfig->maxNmbOutStrms;
     cfg->initARwnd              = sctpConfig->initARwnd;
     cfg->mtuInitial             = sctpConfig->mtuInitial;
     cfg->mtuMinInitial          = sctpConfig->mtuMinInitial;
     cfg->mtuMaxInitial          = sctpConfig->mtuMaxInitial;
     cfg->performMtu             = sctpConfig->performMtu;
     cfg->timeRes                = sctpConfig->timeRes;
     cfg->reConfig.maxInitReTx   = sctpConfig->reConfig_MaxInitReTx;
     cfg->reConfig.maxAssocReTx  = sctpConfig->reConfig_MaxAssocReTx;
     cfg->reConfig.maxPathReTx   = sctpConfig->reConfig_MaxPathReTx;
     cfg->reConfig.altAcceptFlg  = sctpConfig->reConfig_AltAcceptFlg;
     cfg->reConfig.keyTm         = sctpConfig->reConfig_KeyTm;
     cfg->reConfig.alpha         = sctpConfig->reConfig_Alpha;
     cfg->reConfig.beta          = sctpConfig->reConfig_Beta;
   }

   cfg->smPst.srcProcId          = SM_SM_PROC;
   cfg->smPst.dstProcId          = SM_SB_PROC;
   cfg->smPst.srcEnt             = (Ent)ENTSB;
   cfg->smPst.dstEnt             = (Ent)ENTSM;
   cfg->smPst.srcInst            = (Inst)0;
   cfg->smPst.dstInst            = (Inst)0;

   cfg->smPst.prior              = (Prior)UEHSM_MSGPRIOR;
   cfg->smPst.route              = (Route)RTESPEC;
   cfg->smPst.event              = (Event)EVTNONE;
   cfg->smPst.region             = DFLT_REGION;
   cfg->smPst.pool               =  SB_POOL;
   cfg->smPst.selector           = (Selector)UEHSM_SBSMSEL;

    /* Fill Header */
   sbMgt.hdr.msgType             = TCFG;
   sbMgt.hdr.msgLen              = 0;
   sbMgt.hdr.entId.ent           = ENTSB;
   sbMgt.hdr.entId.inst          = SM_INST_ZERO;
   sbMgt.hdr.elmId.elmnt         = STSBGEN;
   sbMgt.hdr.elmId.elmntInst1    = 0;
   sbMgt.hdr.elmId.elmntInst2    = 0;
   sbMgt.hdr.elmId.elmntInst3    = 0;
   sbMgt.hdr.seqNmb              = 0;
   sbMgt.hdr.version             = 0;
   sbMgt.hdr.transId             = 0;

   sbMgt.hdr.response.prior      = PRIOR0;
   sbMgt.hdr.response.route      = RTESPEC;
   sbMgt.hdr.response.mem.region = DFLT_REGION;
   sbMgt.hdr.response.mem.pool   = SB_POOL;
   sbMgt.hdr.response.selector   = SM_SELECTOR_LC;

    /* Fill Pst */
   pst.selector                  = SM_SELECTOR_LC;
   pst.srcEnt                    = ENTSM;
   pst.dstEnt                    = ENTSB;
   pst.dstProcId                 = SM_SB_PROC;
   pst.srcProcId                 = SM_SM_PROC;

   logInfo("%s", "SCTP General Config Request sent");

   (Void)SmMiLsbCfgReq(&pst, &sbMgt);

   RETVOID;
} /* end of smBuildSbGenCfg() */

/*
 *
 *       Fun:    smBuildSbTSapCfg - fill in default SCTP TSAP
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_sb_cntrl.c
 *
 */


PUBLIC Void smBuildSbTSapCfg(
  SpId        spId,
  SuId        suId
  )
{
   SbMgmt      sbMgt;
   SbTSapCfg   *cfg = NULLP;
   Pst          pst;
   SctpConfig *sctpConfig;

   logFunc("%s","smBuildSbTSapCfg");

   SM_SET_ZERO(&sbMgt, sizeof(SbMgmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cfg = &(sbMgt.t.cfg.s.tSapCfg);

   cfg->swtch                     = LSB_SW_RFC_REL0;
   cfg->sel                       = UEHSM_SBHISEL;      /* Loosely coupled upper layer */
   cfg->ent                       = ENTHI;              /* Loosely coupled upper layer */
   cfg->inst                      = 0;                  /* Loosely coupled upper layer */
   cfg->procId                    = SM_HI_PROC;       /* Loosely coupled upper layer */
   cfg->memId.region              = DFLT_REGION;
   cfg->memId.pool                = SB_POOL;
   cfg->prior                     = PRIOR0;
   cfg->route                     = RTESPEC;
   cfg->suId                      = suId;
   cfg->srcNAddrLst.nmb           = 1;
   cfg->srcNAddrLst.nAddr[0].type = CM_NETADDR_IPV4;
   /*BS-1290 Changes*/
   cfg->srcNAddrLst.nAddr[0].u.ipv4NetAddr = smCb.cfgCb.enbIpAddr;
   cfg->reConfig.spId         = spId;

   sctpConfig = &(smCb.cfgCb.sctpConfig);
   cfg->reConfig.tIntTmr       = sctpConfig->reConfig_TIntTmr;
   cfg->reConfig.maxBndRetry   = sctpConfig->reConfig_MaxBndRetry;
   
   /* Fill Header */
   sbMgt.hdr.msgType             = TCFG;
   sbMgt.hdr.msgLen              = 0;
   sbMgt.hdr.entId.ent           = ENTSB;
   sbMgt.hdr.entId.inst          = SM_INST_ZERO;
   sbMgt.hdr.elmId.elmnt         = STSBTSAP;

    /* Fill Pst */
   pst.selector                  = SM_SELECTOR_LC;
   pst.srcEnt                    = ENTSM;
   pst.dstEnt                    = ENTSB;
   pst.dstProcId                 = SM_SB_PROC;
   pst.srcProcId                 = SM_SM_PROC;

   logInfo("SCTP: TUCL SAP Config Request sent for spId:%d suid:%d", spId, suId);

   (Void)SmMiLsbCfgReq(&pst, &sbMgt);

   RETVOID;
} /* end of smBuildSbTSapCfg */

/*
 *
 *       Fun:    smBuildSbSctSapCfg - fill in default SCTP SAP
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_sb_cntrl.c
 *
 */


PUBLIC Void smBuildSbSctSapCfg(
  SpId          spId
  )
{
   SbMgmt               sbMgt;
   SbSctSapCfg          *cfg = NULLP;
   Pst                  pst;
   SctpConfig           *sctpConfig;

   logFunc("%s","smBuildSbSctSapCfg");

   SM_SET_ZERO(&sbMgt, sizeof(SbMgmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(sbMgt.t.cfg.s.sctSapCfg);
   cfg->swtch                  = LSB_SW_RFC_REL0;
   cfg->sel                    = UEHSM_SZSBSEL;     /* Loosely coupled upper layer */
   cfg->memId.region           =  DFLT_REGION;
   cfg->memId.pool             =  SB_POOL;
   cfg->prior                  = PRIOR0;
   cfg->route                  = RTESPEC;
   cfg->spId                   = spId;

   sctpConfig = &(smCb.cfgCb.sctpConfig);
   cfg->reConfig.maxAckDelayTm   = sctpConfig->reConfig_MaxAckDelayTm;
   cfg->reConfig.maxAckDelayDg   = sctpConfig->reConfig_MaxAckDelayDg;
   cfg->reConfig.rtoInitial      = sctpConfig->reConfig_RtoInitial;
   cfg->reConfig.rtoMin          = sctpConfig->reConfig_RtoMin;
   cfg->reConfig.rtoMax          = sctpConfig->reConfig_RtoMax;
   cfg->reConfig.freezeTm        = sctpConfig->reConfig_FreezeTm;
#ifdef LSB4
   cfg->reConfig.bundleTm        = sctpConfig->reConfig_BundleTime;
#endif
   cfg->reConfig.cookieLife      = sctpConfig->reConfig_CookieLife;
   cfg->reConfig.intervalTm      = sctpConfig->reConfig_HbeatIntervalTm;
   cfg->reConfig.maxBurst        = sctpConfig->reConfig_MaxBurst;
   cfg->reConfig.maxHbBurst      = sctpConfig->reConfig_MaxHbBurst;
   cfg->reConfig.t5SdownGrdTm    = sctpConfig->reConfig_T5SdownGrdTm;
   cfg->reConfig.handleInitFlg   = sctpConfig->reConfig_HandleInitFlg;
   cfg->reConfig.negAbrtFlg      = sctpConfig->reConfig_NegAbrtFlg;
   cfg->reConfig.hBeatEnable     = sctpConfig->reConfig_HBeatEnable;
   cfg->reConfig.flcUpThr        = sctpConfig->reConfig_FlcUpThr;    /* number of packets */
   cfg->reConfig.flcLowThr       = sctpConfig->reConfig_FlcLowThr;
   
   /* Fill Header */
   sbMgt.hdr.msgType             = TCFG;
   sbMgt.hdr.msgLen              = 0;
   sbMgt.hdr.entId.ent           = ENTSB;
   sbMgt.hdr.entId.inst          = SM_INST_ZERO;
   sbMgt.hdr.elmId.elmnt         = STSBSCTSAP;

  /* Fill Pst */
   pst.selector                  = SM_SELECTOR_LC;
   pst.srcEnt                    = ENTSM;
   pst.dstEnt                    = ENTSB;
   pst.dstProcId                 = SM_SB_PROC;
   pst.srcProcId                 = SM_SM_PROC;
 
   logInfo("SCTP upper SAP:SZ_SB_SPID Config Request sent for SAP spId:%d", spId);

   (Void)SmMiLsbCfgReq(&pst, &sbMgt);

   RETVOID;
} /* end of smBuildSbSctSapCfg */

/*
 *
 *       Fun:    smBindSbToLSap
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_sb_cntrl.c
 *
 */


PUBLIC Void smBindSbToLSap(
    SpId        spId
  )
{
   SbMgmt               sbMgt;
   Pst                  pst;

   logFunc("%s","smBindSbToLSap");

   SM_SET_ZERO(&sbMgt, sizeof(SbMgmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   sbMgt.hdr.msgType             = TCNTRL;
   sbMgt.hdr.entId.ent           = ENTSB;
   sbMgt.hdr.entId.inst          = SM_INST_ZERO;
   sbMgt.hdr.elmId.elmnt         = STSBTSAP;

   sbMgt.t.cntrl.action          = ABND_ENA;
   sbMgt.t.cntrl.subAction       = SAELMNT;
   sbMgt.t.cntrl.sapId           = spId;

    /* Fill Pst */
   pst.selector                  = SM_SELECTOR_LC;
   pst.srcEnt                    = ENTSM;
   pst.dstEnt                    = ENTSB;
   pst.dstProcId                 = SM_SB_PROC;
   pst.srcProcId                 = SM_SM_PROC;

   logInfo("SCTP Lower SAP:STSBTSAP Bind Request sent to TUCL for spId:%d", spId);

   (Void)SmMiLsbCntrlReq(&pst, &sbMgt);

   RETVOID;
} /* end of smBindSbToLSap */

/*
 *
 *       Fun:    smBuildSbDbgCntrl
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_sb_cntrl.c
 *
 */

PUBLIC Void smBuildSbDbgCntrl(
    LogConfig *logConfig
  )
{
   SbMgmt               sbMgt;
   Pst                  pst;
   U32                  sbdbgMask = 0;

   logFunc("%s","smBuildSbDbgCntrl");

   SM_SET_ZERO(&sbMgt, sizeof(SbMgmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   sbMgt.hdr.msgType             = TCNTRL;
   sbMgt.hdr.entId.ent           = ENTSB;
   sbMgt.hdr.entId.inst          = SM_INST_ZERO;
   sbMgt.hdr.elmId.elmnt         = STSBGEN;
   sbMgt.hdr.transId             = SM_SCTP_DEBUG_TRANS_ID;

   if (logConfig->logMask1 & LOG_MASK_DEBUG)
     sbdbgMask = sbdbgMask | SB_DBG_INFO;

   sbMgt.t.cntrl.action          = ADISIMM;
   sbMgt.t.cntrl.subAction       = SADBG;
   sbMgt.t.cntrl.dbgMask         = 0xffffffff;

    /* Fill Pst */
   pst.selector                  = SM_SELECTOR_LC;
   pst.srcEnt                    = ENTSM;
   pst.dstEnt                    = ENTSB;
   pst.dstProcId                 = SM_SB_PROC;
   pst.srcProcId                 = SM_SM_PROC;

   logInfo("-> SB CTRL %s sbdbgMask=%02x", __func__, sbdbgMask);

   /* Sendig msg to SCTP module for resetting the log level */
   (Void)SmMiLsbCntrlReq(&pst, &sbMgt);

   sbMgt.t.cntrl.action          = AENA;
   sbMgt.t.cntrl.dbgMask         = sbdbgMask;
   /* Sendig msg to SCTP module for setting the actual log level */
   (Void)SmMiLsbCntrlReq(&pst, &sbMgt);

   RETVOID;
} /* end of smBuildSbDbgCntrl*/

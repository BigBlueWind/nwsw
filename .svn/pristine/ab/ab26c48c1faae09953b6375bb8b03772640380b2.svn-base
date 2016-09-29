/**
 * @file   sm_cntrl.c
 * @brief This file is contains the stack manager control functions 
 *    for User Equipment Handler (UEH) module
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */

#include "ccpu_cmn.h"

/* RRM Related Files */
#include "lueh.h"  
#include "ueh.h"
#include "lueh.x"           /* APP types */
#include "log.h"
#include "sm.h"
/*
*
*       Fun:   Initialize UEH task.
*
*       Desc:  Invoked to create UEH TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_cntrl.c
*
*/

PUBLIC S16 smUehInit
(
SSTskId    sysTskId
)
{

   logFunc("%s","smUehInit");

   /* Register UEH Task */
   if (SRegTTsk((Ent)ENTUEH, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 uehActvInit, (ActvTsk)uehActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Register RRC User TAPA Task */
   if (SRegTTsk((Ent)ENTNX, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 NULLP, (ActvTsk)uehActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

    /* Register EGTPU User TAPA Task */
   if (SRegTTsk((Ent)ENTEU, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 NULLP , (ActvTsk)uehActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

    /* Register PDCP Data user TAPA Task */
   if (SRegTTsk((Ent)ENTPX, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 NULLP , (ActvTsk)uehActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach RRC APP TAPA Task */
   if (SAttachTTsk((Ent)ENTUEH, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach RRC APP TAPA Task */
   if (SAttachTTsk((Ent)ENTNX, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }
   
   /* Attach RRC APP TAPA Task */
   if (SAttachTTsk((Ent)ENTEU, (Inst)0, sysTskId)!= ROK)
   {
      RETVALUE(RFAILED);
   }
    
   /* Attach RRC APP TAPA Task */
   if (SAttachTTsk((Ent)ENTPX, (Inst)0, sysTskId)!= ROK)
   {
      RETVALUE(RFAILED);
   }
 
  logInfo ("RRC App Tapa Task successfully registered and attached to %d",sysTskId);

   RETVALUE(ROK);
} /* end of smUehInit */


/*
 *
 *       Fun:    uehSmUehCfg - configure UEH 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ueh_cntrl.c
 *
 */


PUBLIC Void uehSmUehCfg(
  Void
  )
{
   logFunc("%s","uehSmUehCfg");

   smBuildUehGenCfg();
   smBuildUehCtfLSapCfg(UEH_YS_SUID, UEH_YS_SPID);
   smBuildUehNhuLSapCfg(UEH_NH_SUID, UEH_NH_SPID);
   smBuildUehUdhLSapCfg(UEH_UDH_SUID, UEH_UDH_SPID);
   smBuildUehSztLSapCfg(UEH_SZ_SUID, UEH_SZ_SPID);
   smBuildUehRgrLSapCfg(UEH_RG_SUID, UEH_RG_SPID);

   RETVOID;
} /* end of uehSmUehCfg */



/*
*
*       Fun:   smBuildUehGenCfg
*
*       Desc:  fill in genCfg for UEH
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_ueh_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildUehGenCfg(
  Void
  )
#else
PUBLIC S16 smBuildUehGenCfg(Void)
#endif
{
   UehMngmt  uehMgt;
   UehGenCfg    *cfg = NULLP;
   Pst       pst;

   logFunc("%s","smBuildUehGenCfg");

   SM_SET_ZERO(&uehMgt, sizeof(UehMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(uehMgt.u.cfg.u.genCfg);

   /*----------- Fill General Configuration Parameters ---------*/

   /* CELL Configuration  */
   cfg->cellCfg.cellId       =  smCb.cfgCb.cellId;
   cfg->cellCfg.modType      =  smCb.cfgCb.modType;
   cfg->cellCfg.duplexMode   =  smCb.cfgCb.uehConfig.dupMode;//smCfgCb.duplexMode;
   cfg->cellCfg.maxUeSupp    =  smCb.cfgCb.maxUeSupp;
   cfg->cellCfg.mcc[0].pres  =  smCb.cfgCb.mcc[0].pres;
   cfg->cellCfg.mcc[0].val   =  smCb.cfgCb.mcc[0].val;
   cfg->cellCfg.mcc[1].pres  =  smCb.cfgCb.mcc[1].pres;
   cfg->cellCfg.mcc[1].val   =  smCb.cfgCb.mcc[1].val;
   cfg->cellCfg.mcc[2].pres  =  smCb.cfgCb.mcc[2].pres;
   cfg->cellCfg.mcc[2].val   =  smCb.cfgCb.mcc[2].val;
   cfg->cellCfg.mnc[0].pres  =  smCb.cfgCb.mnc[0].pres;
   cfg->cellCfg.mnc[0].val   =  smCb.cfgCb.mcc[0].val;
   cfg->cellCfg.mnc[1].pres  =  smCb.cfgCb.mnc[1].pres;
   cfg->cellCfg.mnc[1].val   =  smCb.cfgCb.mcc[1].val;
   cfg->cellCfg.mnc[2].pres  =  smCb.cfgCb.mcc[2].pres;
   cfg->cellCfg.mnc[2].val   =  smCb.cfgCb.mcc[2].val;
   cfg->cellCfg.trackAreaCode=  smCb.cfgCb.trackAreaCode;
   cfg->cellCfg.freqBandInd  =  smCb.cfgCb.freqBandInd;

   cfg->dataAppAddr.type                     = CM_TPTADDR_IPV4;
   cfg->dataAppAddr.u.ipv4TptAddr.port       = UEH_DFLT_EGTP_PORT;
   cfg->dataAppAddr.u.ipv4TptAddr.address    = smCb.cfgCb.enbIpAddr;//smCfgCb.enbIpAddr;

   cfg->lmPst.srcProcId = SM_UEH_PROC;
   cfg->lmPst.dstProcId = SM_SM_PROC;
   cfg->lmPst.srcEnt = (Ent)ENTUEH;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;
   cfg->lmPst.prior = (Prior)UEHSM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = smCb.init.region;
   cfg->lmPst.pool   =  UEH_POOL;
   cfg->lmPst.selector = (Selector)UEHSM_UEHSMSEL;

   /* Fill Header */
   uehMgt.hdr.msgType             = TCFG;
   uehMgt.hdr.msgLen              = 0;
   uehMgt.hdr.entId.ent           = ENTUEH;
   uehMgt.hdr.entId.inst          = SM_INST_ZERO;
   uehMgt.hdr.elmId.elmnt         = STUEHGEN;
   uehMgt.hdr.elmId.elmntInst1    = 0;
   uehMgt.hdr.elmId.elmntInst2    = 0;
   uehMgt.hdr.elmId.elmntInst3    = 0;
   uehMgt.hdr.seqNmb              = 0;
   uehMgt.hdr.version             = 0;
   uehMgt.hdr.transId             = 0;

   uehMgt.hdr.response.prior      = PRIOR0;
   uehMgt.hdr.response.route      = RTESPEC;
   uehMgt.hdr.response.mem.region = smCb.init.region;
   uehMgt.hdr.response.mem.pool   = UEH_POOL;
   uehMgt.hdr.response.selector   = SM_SELECTOR_LC;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_SM_PROC;

  logInfo("%s","UEH General Config request sent");

   /* Send the request to the LM */
   (Void) SmMiLuehCfgReq(&pst, &uehMgt);
 
   RETVALUE(ROK);
}/* smBuildUehGenCfg */

/*
*
*       Fun:   smBuildUehCtfLSapCfg
*
*       Desc:  Call handler for CTF LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_ueh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildUehCtfLSapCfg(
  SuId        suId,             /* RRC App Sap ID */
  SpId        spId              /* Service Provider Sap ID */
  )
#else
PUBLIC S16 smBuildUehCtfLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   UehMngmt    uehMgt;
   UehLSapCfg  *cfg = NULLP;
   Pst        pst;
 
   logFunc("%s","smBuildUehCtfLSapCfg");

   SM_SET_ZERO(&uehMgt, sizeof(UehMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(uehMgt.u.cfg.u.lSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_YS_PROC;
   cfg->dstEnt       = ENTYS;
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = UEHSM_UEHYSSEL;
   cfg->srcEnt       = ENTUEH;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = UEH_POOL;

   cfg->maxBndRetry = UEHSM_LUEH_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = UEHSM_LUEH_VAL_10;

   /* Fill Header */
   uehMgt.hdr.msgType             = TCFG;
   uehMgt.hdr.msgLen              = 0;
   uehMgt.hdr.entId.ent           = ENTUEH;
   uehMgt.hdr.entId.inst          = SM_INST_ZERO;
   uehMgt.hdr.elmId.elmnt         = STUEHCTFSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("UEH: CTF SAP Config Request sent for spId:%d suid:%d", spId, suId);

   /* Send the request to the LM */
   (Void) SmMiLuehCfgReq(&pst, &uehMgt);
 
   RETVALUE(ROK);
}/* smBuildUehCtfLSapCfg */

/*
*
*       Fun:   smBuildUehNhuLSapCfg
*
*       Desc:  Call handler for NHU LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_ueh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildUehNhuLSapCfg(
  SuId        suId,             /* RRC App Sap ID */
  SpId        spId              /* Service Provider Sap ID */
  )
#else
PUBLIC S16 smBuildUehNhuLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   UehMngmt    uehMgt;
   UehLSapCfg  *cfg = NULLP;
   Pst        pst;
 
   logFunc("%s","smBuildUehNhuLSapCfg");

   SM_SET_ZERO(&uehMgt, sizeof(UehMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(uehMgt.u.cfg.u.lSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_NH_PROC;
   cfg->dstEnt       = ENTNH;
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = UEHSM_UEHNHSEL;
   cfg->srcEnt       = ENTNX;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = UEH_POOL;

   cfg->maxBndRetry = UEHSM_LUEH_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = UEHSM_LUEH_VAL_10;

   /* Fill Header */
   uehMgt.hdr.msgType             = TCFG;
   uehMgt.hdr.msgLen              = 0;
   uehMgt.hdr.entId.ent           = ENTUEH;
   uehMgt.hdr.entId.inst          = SM_INST_ZERO;
   uehMgt.hdr.elmId.elmnt         = STUEHNHUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("UEH: RRC SAP Config Request sent for spId:%d suid:%d", spId, suId);

   /* Send the request to the LM */
   (Void) SmMiLuehCfgReq(&pst, &uehMgt);
 
   RETVALUE(ROK);
}/* smBuildUehNhuLSapCfg */

/*
*
*       Fun:   smBuildUehRgrLSapCfg
*
*       Desc:  Call handler for RGR LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_ueh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildUehRgrLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildUehRgrLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   UehMngmt    uehMgt;
   UehLSapCfg  *cfg = NULLP;
   Pst        pst;
 
   logFunc("%s","smBuildUehRgrLSapCfg");

   SM_SET_ZERO(&uehMgt, sizeof(UehMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(uehMgt.u.cfg.u.lSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_RG_PROC;
   cfg->dstEnt       = ENTRG;
   cfg->dstInst      = (Inst)SCH_INST_ID;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = UEHSM_UEHRGSEL;
   cfg->srcEnt       = ENTNX;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = UEH_POOL;

   cfg->maxBndRetry = UEHSM_LUEH_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = UEHSM_LUEH_VAL_10;

   /* Fill Header */
   uehMgt.hdr.msgType             = TCFG;
   uehMgt.hdr.msgLen              = 0;
   uehMgt.hdr.entId.ent           = ENTUEH;
   uehMgt.hdr.entId.inst          = SM_INST_ZERO;
   uehMgt.hdr.elmId.elmnt         = STUEHRGRSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("UEH: MAC SAP Config Request sent for spId:%d suid:%d", spId, suId);

   /* Send the request to the LM */
   (Void) SmMiLuehCfgReq(&pst, &uehMgt);
 
   RETVALUE(ROK);
}/* smBuildUehRgrLSapCfg */


/*
*
*       Fun:   smBuildUehSztLSapCfg
*
*       Desc:  Call handler for SZT LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_ueh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildUehSztLSapCfg(
  SuId        suId,             /* RRC App Sap ID */
  SpId        spId              /* Service Provider Sap ID */
  )
#else
PUBLIC S16 smBuildUehSztLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   UehMngmt    uehMgt;
   UehLSapCfg  *cfg = NULLP;
   Pst        pst;
 
   logFunc("%s","smBuildUehSztLSapCfg");

   SM_SET_ZERO(&uehMgt, sizeof(UehMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(uehMgt.u.cfg.u.lSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->dstProcId    = SM_SZ_PROC;
   cfg->dstEnt       = ENTSZ;
   cfg->dstInst      = (Inst)0;
   cfg->dstPrior     = PRIOR0;
   cfg->dstRoute     = RTESPEC;
   cfg->dstSel       = UEHSM_UEHSZSEL;
   cfg->srcEnt       = ENTUEH;
   cfg->srcInst      = (Inst)0;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = UEH_POOL;

   cfg->maxBndRetry = UEHSM_LUEH_MAX_BND_RETRY;
   cfg->bndTmr.enb = TRUE;
   cfg->bndTmr.val = UEHSM_LUEH_VAL_10;

   /* Fill Header */
   uehMgt.hdr.msgType             = TCFG;
   uehMgt.hdr.msgLen              = 0;
   uehMgt.hdr.entId.ent           = ENTUEH;
   uehMgt.hdr.entId.inst          = SM_INST_ZERO;
   uehMgt.hdr.elmId.elmnt         = STUEHSZTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("UEH: S1AP SAP Config Request sent for spId:%d suid:%d", spId, suId);

   /* Send the request to the LM */
   (Void) SmMiLuehCfgReq(&pst, &uehMgt);
 
   RETVALUE(ROK);
}/* smBuildUehSztLSapCfg */

/*
*
*       Fun:   smBuildUehUdhLSapCfg
*
*       Desc:  Call handler for UDH LSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_ueh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildUehUdhLSapCfg
(
SuId        suId,             /* RRC App Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildUehUdhLSapCfg( suId, spId)
SuId        suId;             /* RRC App Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   UehMngmt    uehMgt;
   UehLSapCfg  *sapCfg = NULLP;
   Pst        pst;
 
   logFunc("%s","smBuildUehUdhLSapCfg");

   SM_SET_ZERO(&uehMgt, sizeof(UehMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   sapCfg   = &(uehMgt.u.cfg.u.lSap);

   sapCfg->suId   = suId;
   sapCfg->spId   = spId;

   sapCfg->dstProcId    = SM_UDH_PROC;
   sapCfg->dstEnt       = ENTUDH;
   sapCfg->dstInst      = (Inst)0;
   sapCfg->dstPrior     = PRIOR0;
   sapCfg->dstRoute     = RTESPEC;
   sapCfg->dstSel       = UEHSM_UEHUDHSEL;
   sapCfg->srcEnt       = ENTUEH;
   sapCfg->srcInst      = (Inst)0;
   sapCfg->mem.region   = smCb.init.region;
   sapCfg->mem.pool     = UDH_POOL;

   sapCfg->maxBndRetry = UEHSM_LUEH_MAX_BND_RETRY;
   sapCfg->bndTmr.enb = TRUE;
   sapCfg->bndTmr.val = UEHSM_LUEH_VAL_10;

   /* Fill Header */
   uehMgt.hdr.msgType             = TCFG;
   uehMgt.hdr.msgLen              = 0;
   uehMgt.hdr.entId.ent           = ENTUEH;
   uehMgt.hdr.entId.inst          = SM_INST_ZERO;
   uehMgt.hdr.elmId.elmnt         = STUEHUDHSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_SM_PROC;

  logInfo("%s", "UEH UDH LSap(STUEHUDHSAP) Config sent");

   /* Send the request to the LM */
   (Void) SmMiLuehCfgReq(&pst, &uehMgt);
 
   RETVALUE(ROK);
}/* smBuildUehUdhLSapCfg */

/*
 *      FUN:   smBindUehToSztSap
 *
 *      Desc:  Bind UEH with SCT LSAP. 
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  ueh_sm_ueh_cntrl.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindUehToSztSap
(
Void
)
#else
PUBLIC S16 smBindUehToSztSap(Void)
#endif /* ANSI */
{
   UehMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   logFunc("%s","smBindUehToSztSap");

   SM_SET_ZERO(&cntrl, sizeof(UehMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sapId       = UEH_SZ_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTUEH;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STUEHSZTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s", "UEH Lower SAP:STUEHSZTSAP Bind Request sent to S1AP");

    /* Send the control request to the APP */
   (Void)SmMiLuehCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* smBindUehToSztSap */

/*
 *      FUN:   smBindUehToNhuSap
 *
 *      Desc:  Bind UEH with NHU LSAP. 
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  ueh_sm_ueh_cntrl.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindUehToNhuSap(
    Void
  )
#else
PUBLIC S16 smBindUehToNhuSap(Void)
#endif /* ANSI */
{
   UehMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   logFunc("%s","smBindUehToNhuSap");

   SM_SET_ZERO(&cntrl, sizeof(UehMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sapId       = UEH_NH_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTUEH;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STUEHNHUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s", "UEH Lower SAP:STUEHNHUSAP Bind Request sent to RRC");

   /* Send the control request to the UEH*/
   (Void)SmMiLuehCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* smBindUehToNhuSap */

/*
 *      FUN:   smBindUehToCtfSap
 *
 *      Desc:  Bind UEH with CTF LSAP. 
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  ueh_sm_ueh_cntrl.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindUehToCtfSap(
    Void
  )
#else
PUBLIC S16 smBindUehToCtfSap(Void)
#endif /* ANSI */
{
   UehMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   logFunc("%s","smBindUehToCtfSap");

   SM_SET_ZERO(&cntrl, sizeof(UehMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sapId       = UEH_YS_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTUEH;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STUEHCTFSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s", "UEH: CTF lower SAP(STUEHCTFSAP) Bind request sent");

   /* Send the control request to the UEH*/
   (Void)SmMiLuehCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* smBindUehToCtfSap */


/*
 *      FUN:   smBindUehToRgrSap
 *
 *      Desc:  Bind UEH with RGR LSAP. 
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  ueh_sm_ueh_cntrl.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindUehToRgrSap(
    Void
  )
#else
PUBLIC S16 smBindUehToRgrSap(Void)
#endif /* ANSI */
{
   UehMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   logFunc("%s","smBindUehToRgrSap");

   SM_SET_ZERO(&cntrl, sizeof(UehMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sapId       = UEH_RG_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTUEH;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STUEHRGRSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s", "UEH Lower SAP:STUEHRGRSAP Bind Request sent to MAC");

   /* Send the control request to the UEH*/
   (Void)SmMiLuehCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* smBindUehToRgrSap */



/*
*
*       Fun:   smBuildUehGenCntrl
*
*       Desc:  Invoked to Enable Alarms at APP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_ueh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildUehAlarmCntrl
(
)
#else
PUBLIC S16 smBuildUehAlarmCntrl(Void)
#endif
{
   UehMngmt cntrl;
   Pst      pst;
   
   logFunc("%s","smBindUehGenCntrl");

   SM_SET_ZERO(&cntrl, sizeof(UehMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl.u.cntrl.action        =  AENA;
   cntrl.u.cntrl.u.sapId       =  0;
   cntrl.u.cntrl.subAction     =  SAUSTA;

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTUEH;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STUEHGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "Alarm control request to UEH sent");

   /* Send the request to the APP */
   SmMiLuehCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildUehGenCntrl */

/*
*
*       Fun:   smBuildUehCellCntrl
*
*       Desc:  Invoked to intimate the APP that Binding is DONE
*              and initiate cell control
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_ueh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildUehCellCntrl(
    void
  )
#else
PUBLIC S16 smBuildUehCellCntrl(Void)
#endif
{
   UehMngmt cntrl;
   Pst      pst;
   
   logFunc("%s", __FUNCTION__);

   SM_SET_ZERO(&cntrl, sizeof(UehMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTUEH;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STUEHCELLCFG;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "Cell Control request to UEH sent");

   /* Send the request to the APP */
   SmMiLuehCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildUehCellCntrl */

/*
*
*       Fun:   smBuildUehS1InitCntrl
*
*       Desc:  Invoked to intimate the APP that Binding is DONE.
*              APP should initiate the S1AP association
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_ueh_cntrl.c
*
*/
#if 0
#ifdef ANSI
PUBLIC S16 smBuildUehS1InitCntrl
(
)
#else
PUBLIC S16 smBuildUehS1InitCntrl(Void)
#endif
{
   UehMngmt cntrl;
   Pst      pst;
   
   logFunc("%s","smBuildUehS1InitCntrl");

#if (defined(UEH_PERF_MEAS) || defined(UEH_WITHOUT_CNE))
   smBuildUehCellCntrl();
   smCb.smState = UEH_SM_STATE_AWAIT_CELL_UP;
   RETVALUE(ROK);
#endif


   SM_SET_ZERO(&cntrl, sizeof(UehMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTUEH;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STUEHS1CONSETUP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
  logInfo ("%s", "RRM Cntrl Req to ueh for S1 Init");
   /* Send the request to the APP */
   SmMiLuehCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildUehInitCntrl */
#endif
/*
*
*       Fun:   smBuildUehDbgCntrl
*
*       Desc:  Invoked to enable debug prints
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_ueh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildUehDbgCntrl(
    LogConfig *logConfig
  )
#else
PUBLIC S16 smBuildUehDbgCntrl(Void)
#endif
{
   logFunc("%s","smBuildUehDbgCntrl");

   //Set the latest log level to cp.exe log level
   getLogContext()->logConfig = *logConfig;
   logInfo("%s", "CP Loglevel is set to new values");
   RETVALUE(ROK);
} /* end of smBuildUehInitCntrl */

/*
 *      FUN:   smBindUehToUdhSap
 *
 *      Desc:  Bind UEH with UDH LSAP. 
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  ueh_sm_ueh_cntrl.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindUehToUdhSap
(
Void
)
#else
PUBLIC S16 smBindUehToUdhSap(Void)
#endif /* ANSI */
{
   UehMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   logFunc("%s","smBindUehToUdhSap");

   SM_SET_ZERO(&cntrl, sizeof(UehMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.u.cntrl.action       = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sapId      = UEH_UDH_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTUEH;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STUEHUDHSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTUEH;
   pst.dstProcId = SM_UEH_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s", "UEH BIND control request sent to UDH");

    /* Send the control request to the UEH*/
   (Void)SmMiLuehCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* smBindUehToUdhSap */

/**
 * @file  sm_ys_cntrl.c
 * @brief This file contains configuration functions for CL
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
 *       Fun:    uehSmYsCfg - configure YS
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ueh_sm_ys_cntrl.c
 *
 */

PUBLIC Void uehSmYsCfg(
  Void
  )
{
   logFunc("%s","uehSmYsCfg");

   smBuildYsGenCfg();
   smBuildYsTfuSapCfg(RG_YS_SUID, RG_YS_SPID);
   smBuildYsSchTfuSapCfg(RG_YS_SUID, RG_SCH_YS_SPID);
   smBuildYsCtfSapCfg(UEH_YS_SUID, UEH_YS_SPID);

   RETVOID;
} /* end of uehSmYsCfg */



/*
*
*       Fun:   smBuildYsGenCfg
*
*       Desc:  fill in genCfg for YS
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_ys_cntrl.c 
*
*/

PUBLIC S16 smBuildYsGenCfg(
    Void
  )
{
   YsMngmt     *ysMgt = NULLP;
   YsGenCfg    *cfg = NULLP;
   Pst         pst;

   logFunc("%s","smBuildYsGenCfg");

   SGetSBuf(0, 0, (Data **)&ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(ysMgt->t.cfg.s.genCfg);

   /*----------- Fill General Configuration Parameters ---------*/
   cfg->maxTfuSaps  = (U16)UEHSM_LYS_MAX_TFU_SAPS;
   cfg->nmbUe       = (U16)UEHSM_LYS_NMB_UES;

   /*----------- Fill lmPst ---------*/
   cfg->lmPst.srcProcId = SM_YS_PROC;
   cfg->lmPst.dstProcId = SM_SM_PROC;
   cfg->lmPst.srcEnt = (Ent)ENTTF;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;
   cfg->lmPst.prior = (Prior)UEHSM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = smCb.init.region;
   cfg->lmPst.pool =  smCb.init.pool;
   cfg->lmPst.selector = (Selector)UEHSM_YSSMSEL;

   /* Fill Header */
   ysMgt->hdr.msgType             = TCFG;
   ysMgt->hdr.msgLen              = 0;
   ysMgt->hdr.entId.ent           = ENTTF;
   ysMgt->hdr.entId.inst          = SM_INST_ZERO;
   ysMgt->hdr.elmId.elmnt         = STGEN;
   ysMgt->hdr.elmId.elmntInst1    = 0;
   ysMgt->hdr.elmId.elmntInst2    = 0;
   ysMgt->hdr.elmId.elmntInst3    = 0;
   ysMgt->hdr.seqNmb              = 0;
   ysMgt->hdr.version             = 0;
   ysMgt->hdr.transId             = 0;

   ysMgt->hdr.response.prior      = PRIOR0;
   ysMgt->hdr.response.route      = RTESPEC;
   ysMgt->hdr.response.mem.region = smCb.init.region;
   ysMgt->hdr.response.mem.pool   = smCb.init.pool;
   ysMgt->hdr.response.selector   = SM_SELECTOR_LC;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTTF;
   pst.dstProcId = SM_YS_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s","YS General Config request sent");
 
   /* Send the request to the LM */
   (Void) SmMiLysCfgReq(&pst, ysMgt);
 
   RETVALUE(ROK);
}/* smBuildYsGenCfg */

/*
*
*       Fun:   smBuildYsTfuSapCfg
*
*       Desc:  Call handler for TFU SAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_ys_cntrl.c
*
*/
PUBLIC S16 smBuildYsTfuSapCfg(
    SuId        suId,             /* CL Sap ID */
    SpId        spId              /* Service Provider Sap ID */
  )
{
   YsMngmt      *ysMgt = NULLP;
   YsTfuSapCfg  *cfg = NULLP;
   Pst          pst;
 
   logFunc("%s","smBuildYsTfuSapCfg");

   SGetSBuf(0, 0, (Data **)&ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(ysMgt->t.cfg.s.tfuSap);

   cfg->selector     = UEHSM_RGYSSEL;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = YS_POOL;
   cfg->prior        = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->procId       = SM_RG_PROC;
   cfg->ent          = ENTRG;
   cfg->inst         = (Inst)0;
   cfg->route        = RTESPEC;
    cfg->type         = (spId == RG_YS_SPID) ? LYS_TFU_USR_SAP : LYS_TFU_SCH_SAP;

   cfg->suId         = suId;
   cfg->spId         = spId;
   cfg->type         = LYS_TFU_USR_SAP;
   cfg->cellId       = smCb.cfgCb.cellId;//smCfgCb.cellId;
   
   /* Fill Header */
   ysMgt->hdr.msgType             = TCFG;
   ysMgt->hdr.msgLen              = 0;
   ysMgt->hdr.entId.ent           = ENTTF;
   ysMgt->hdr.entId.inst          = SM_INST_ZERO;
   ysMgt->hdr.elmId.elmnt         = STTFUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTTF;
   pst.dstProcId = SM_YS_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("CL: MAC SAP Config Request sent for spId:%d suid:%d", spId, suId);
 
   /* Send the request to the LM */
   (Void) SmMiLysCfgReq(&pst, ysMgt);
 
   RETVALUE(ROK);
}/* smBuildYsTfuSapCfg */


/*
*
*       Fun:   smBuildYsSchTfuSapCfg
*
*       Desc:  Call handler for SCH TFU SAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_ys_cntrl.c
*
*/
PUBLIC S16 smBuildYsSchTfuSapCfg(
    SuId        suId,             /* CL Sap ID */
    SpId        spId              /* Service Provider Sap ID */
  )
{
   YsMngmt      *ysMgt = NULLP;
   YsTfuSapCfg  *cfg = NULLP;
   Pst          pst;
 
   logFunc("%s","smBuildYsSchTfuSapCfg");

   SGetSBuf(0, 0, (Data **)&ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(ysMgt->t.cfg.s.tfuSap);

   //cfg->selector     = UEHSM_RGYSSEL;
   cfg->selector     = 1;
   cfg->mem.region   = DFLT_REGION;
   cfg->mem.pool     = YS_POOL;
   cfg->prior        = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->procId       = SM_RG_PROC;
   cfg->ent          = ENTRG;
   cfg->inst         = (Inst)1;
   cfg->route        = RTESPEC;

   cfg->suId         = suId;
   cfg->spId         = spId;
   cfg->type         = LYS_TFU_SCH_SAP;
   cfg->cellId       = smCb.cfgCb.cellId;//smCfgCb.cellId;
   
   /* Fill Header */
   ysMgt->hdr.msgType             = TCFG;
   ysMgt->hdr.msgLen              = 0;
   ysMgt->hdr.entId.ent           = ENTTF;
   ysMgt->hdr.entId.inst          = SM_INST_ZERO;
   ysMgt->hdr.elmId.elmnt         = STTFUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTTF;
   pst.dstProcId = SM_YS_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("CL: MAC-SCH SAP Config Request sent for spId:%d suid:%d", spId, suId);
 
   /* Send the request to the LM */
   (Void) SmMiLysCfgReq(&pst, ysMgt);
 
   RETVALUE(ROK);
}/* smBuildYsTfuSapCfg */



/*
*
*       Fun:   smBuildYsCtfSapCfg
*
*       Desc:  Call handler for CTF SAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_sm_ys_cntrl.c
*
*/
PUBLIC S16 smBuildYsCtfSapCfg(
    SuId        suId,             /* CL Sap ID */
    SpId        spId              /* Service Provider Sap ID */
  )
{
   YsMngmt      *ysMgt = NULLP;
   YsCtfSapCfg  *cfg = NULLP;
   Pst          pst;
 
   logFunc("%s","smBuildYsCtfSapCfg");

   SGetSBuf(0, 0, (Data **)&ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(ysMgt->t.cfg.s.ctfSap);

   cfg->selector     = UEHSM_UEHYSSEL;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = YS_POOL;
   cfg->prior        = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->procId       = SM_UEH_PROC;

   cfg->inst         = (Inst)0;
   cfg->route        = RTESPEC;

   cfg->suId         = suId;
   cfg->spId         = spId;
   
   /* Fill Header */
   ysMgt->hdr.msgType             = TCFG;
   ysMgt->hdr.msgLen              = 0;
   ysMgt->hdr.entId.ent           = ENTTF;
   ysMgt->hdr.entId.inst          = SM_INST_ZERO;
   ysMgt->hdr.elmId.elmnt         = STCTFSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTTF;
   pst.dstProcId = SM_YS_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("CL: CTF SAP Config Request sent for spId:%d suid:%d", spId, suId);
 
   /* Send the request to the LM */
   (Void) SmMiLysCfgReq(&pst, ysMgt);
 
   RETVALUE(ROK);
}/* smBuildYsCtfSapCfg */


/*
*
*       Fun:   smBuildYsGenCntrl
*
*       Desc:  Invoked to Enable Alarms at CL
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_ys_cntrl.c
*
*/
PUBLIC S16 smBuildYsGenCntrl()
{
   YsMngmt  *cntrl = NULLP;
   Pst      pst;
   
   logFunc("%s","smBuildYsGenCntrl");

   SGetSBuf(0, 0, (Data **)&cntrl, sizeof(YsMngmt));
   SM_SET_ZERO(cntrl, sizeof(YsMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl->t.cntrl.action        =  AENA;
   cntrl->t.cntrl.s.ysSapCntrl.suId =  0;
   cntrl->t.cntrl.subAction     =  SAUSTA;

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTTF;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTTF;
   pst.dstProcId = SM_YS_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo ("%s", "Sending Gen Cntrl Req to ys\n");
 
   /* Send the request to the CL */
   SmMiLysCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildYsGenCntrl */

/*
*
*       Fun:   smBuildYsDbgCntrl
*
*       Desc:  Invoked to Enable Debugs at CL
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_ys_cntrl.c
*
*/
/* Patch ve001.101 */
PUBLIC S16 smBuildYsDbgCntrl(
    LogConfig *logConfig
  )
{
  YsMngmt  *cntrl = NULLP;
  YsMngmt  *cntrlSet = NULLP;
  Pst      pst;
  U32                  ysdbgMask=0;

  SGetSBuf(0, 0, (Data **)&cntrl, sizeof(YsMngmt));
  SM_SET_ZERO(cntrl, sizeof(YsMngmt));
  SGetSBuf(0, 0, (Data **)&cntrlSet, sizeof(YsMngmt));
  SM_SET_ZERO(cntrlSet, sizeof(YsMngmt));
  SM_SET_ZERO(&pst, sizeof(Pst));

  //Logging for YS is different than others, if INFO is set, all logs get enabled
  if((logConfig->logMask1 & LOG_MASK_MSGIN_02 ) || (logConfig->logMask1 & LOG_MASK_MSGOUT_02 ) )
    {
       ysdbgMask = ysdbgMask | YS_DBG_MOD;
    }
  if(logConfig->logMask1 & LOG_MASK_ERROR )
    {
       ysdbgMask = ysdbgMask  | YS_DBG_ERROR;
    }
  if(logConfig->logMask1 & LOG_MASK_DEBUG )
    {
       ysdbgMask = ysdbgMask | YS_DBG_INFO;
    }
  logInfo(":%s: DBG-Mask: %02x",__func__, ysdbgMask);

  cntrl->t.cntrl.action        =  ADISIMM;
  cntrl->t.cntrl.subAction     =  SADBG;
  cntrl->t.cntrl.s.ysDbgCntrl.dbgMask = 0xffffffff;

  /* Fill Header */
  cntrl->hdr.msgType             = TCNTRL;
  cntrl->hdr.entId.ent           = ENTTF;
  cntrl->hdr.entId.inst          = SM_INST_ZERO;
  cntrl->hdr.elmId.elmnt         = STGEN;
  cntrl->hdr.transId             = SM_CL_DEBUG_TRANS_ID;

  *cntrlSet = *cntrl;

  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTTF;
  pst.dstProcId = SM_YS_PROC;
  pst.srcProcId = SM_SM_PROC;

  logInfo("%s", "Debug control request sent to CL(YS)");

  /* Send the CTRL-request to reset the log level for CL module */
  SmMiLysCntrlReq(&pst, cntrl);

  /* Send the CTRL-request to set the log level for CL module */
  cntrlSet->t.cntrl.action        =  AENA;
  cntrlSet->t.cntrl.s.ysDbgCntrl.dbgMask = ysdbgMask;
  SmMiLysCntrlReq(&pst, cntrlSet);

  RETVALUE(ROK);
}

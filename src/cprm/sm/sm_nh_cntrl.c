/**
 * @file   sm_nh_cntrl.c
 * @brief This file contains stack manager code for RRC Control
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */
/**********************************************************************

     Name:     LTE eNodeB Stack Manager 
  
     Type:     C Source file
  
     Desc:     RRC Stack Manager Initialization Functions.

     File:     ueh_sm_nh_cntrl.c

**********************************************************************/

/* header include files (.h) */
#include "ccpu_cmn.h"

/* RRM Related Files */
#include "ueh.h"
#include "log.h"

/*
*
*       Fun:   Initialize RRC task.
*
*       Desc:  Invoked to create RRC TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smNhInit
(
SSTskId    sysTskId
)
{
   logFunc("%s","smNhInit");

   /* Register RRC TAPA Task */
   if (SRegTTsk((Ent)ENTNH, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
            nhActvInit, (ActvTsk)nhActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Attach RRC TAPA Task */
   if (SAttachTTsk((Ent)ENTNH, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */  
   logInfo("RRC Tapa Task successfully registered and attached to %d\n",sysTskId);
   RETVALUE(ROK);
} /* end of smNhInit */

/*
 *
 *       Fun:    uehSmNhCfg - configure NH
 *
 *       Desc:
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ueh_sm_nh_cntrl.c
 *
 */


PUBLIC Void uehSmNhCfg(
  Void
  )
{
   logFunc("%s","uehSmNhCfg");

   smBuildNhGenCfg();
   smBuildNhProtCfg();
   smBuildNhUsapCfg();
   smBuildNhLsapCfg(STNHKWUSAP);
   smBuildNhLsapCfg(STNHCKWSAP);
   smBuildNhLsapCfg(STNHCRGSAP);
   smBuildNhLsapCfg(STNHCPJSAP);
   smBuildNhLsapCfg(STNHPJUSAP);

   RETVOID;
} /* end of uehSmNhCfg */



/*
*
*       Fun:   smBuildNhGenCfg
*
*       Desc:  Invoked to build RRC GEN CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBuildNhGenCfg(void)
{
   NhMngmt   nhMngmt;
   NhGenCfg  *genCfg = NULLP;
   Pst       pst;

   logFunc("%s","smBuildNhGenCfg");

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   genCfg   = &(nhMngmt.u.cfg.u.gen);

   /*----------- Fill General Configuration Parameters ---------*/
   genCfg->maxCell      =  1;
   genCfg->msecTmrRes   = SM_DFLT_MSEC_TMR_CFG;   /* Milli Sec Timer Resolution */
   genCfg->cfgTmr.enb = FALSE;
   genCfg->cfgTmr.val  = 60;
   genCfg->nmbKwuSaps = 1;
   genCfg->nmbCkwSaps = 1;
   genCfg->nmbCrgSaps = 1;
   genCfg->nmbCpjSaps = 1;
   genCfg->nmbPjuSaps = 1;
   genCfg->minTransLstLen = 2;
   genCfg->minMuiLstLen = 2;

#ifdef DEBUGP  /* By Deafult Enabled*/
   genCfg->dbgMsk =  0x0;
#endif
   
   genCfg->lmPst.dstProcId = SM_SM_PROC;
   genCfg->lmPst.srcProcId = SM_NH_PROC;
   genCfg->lmPst.dstEnt    = ENTSM;
   genCfg->lmPst.dstInst   = 0;
   genCfg->lmPst.srcEnt    = ENTNH;
   genCfg->lmPst.srcInst   = 0;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = smCb.init.region;
   genCfg->lmPst.pool      = UEH_NH_POOL;
   genCfg->lmPst.selector  = SM_SELECTOR_LC;
   

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.msgLen              = 0;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHGEN;
   nhMngmt.hdr.elmId.elmntInst1    = 0;
   nhMngmt.hdr.elmId.elmntInst2    = 0;
   nhMngmt.hdr.elmId.elmntInst3    = 0;
   nhMngmt.hdr.seqNmb              = 0;
   nhMngmt.hdr.version             = 0;
   nhMngmt.hdr.transId             = 0;
  
   nhMngmt.hdr.response.prior      = PRIOR0;
   nhMngmt.hdr.response.route      = RTESPEC;
   nhMngmt.hdr.response.mem.region = smCb.init.region;
   nhMngmt.hdr.response.mem.pool   = UEH_NH_POOL;
   nhMngmt.hdr.response.selector   = SM_SELECTOR_LC;
   
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s","RRC General Config request sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBuildNhGenCfg */

/*
*
*       Fun:   smBuildNhProtCfg
*
*       Desc:  Invoked to build RRC Protocol CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBuildNhProtCfg(
  Void
  )
{
   NhMngmt    nhMngmt;
   NhProtCfg  *protCfg = NULLP;
   Pst       pst;

   logFunc("%s","smBuildNhProtCfg");

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   protCfg   = &(nhMngmt.u.cfg.u.prot);

   /* Fill Prot Config*/
   protCfg->maxConn = 1000;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHPROT;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s","RRC Protocol Config request sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);
   
   RETVALUE(ROK);
} /* smBuildNhProtCfg */

/*
*
*       Fun:   smBuildNhUsapCfg
*
*       Desc:  Invoked to build USAP CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBuildNhUsapCfg(
  Void
  )
{
   NhMngmt    nhMngmt;
   NhUSapCfg  *uSap = NULLP;
   Pst       pst;

   logFunc("%s","smBuildNhUsapCfg");

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   uSap   = &(nhMngmt.u.cfg.u.uSap);

   uSap->selector     = SM_SELECTOR_TC; /* Between RRC-NX Tight coupled */
   uSap->mem.region   = smCb.init.region;
   uSap->mem.pool     = UEH_NH_POOL;
   uSap->prior        = PRIOR0;
   uSap->procId       = SM_NX_PROC;
   uSap->ent          = ENTNX;
   uSap->inst         = SM_INST_ZERO;
   uSap->route        = RTESPEC;
   uSap->spId         = SM_ZERO_VAL;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;

   logInfo("%s","RRC upper SAP Config request sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);
   
   RETVALUE(ROK);
} /* smBuildNhUsapCfg */


/*
*
*       Fun:   smBuildNhKwuLsapCfg
*
*       Desc:  Invoked to build LSAP KWU CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBuildNhKwuLsapCfg(
  Void
  )
{
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   logFunc("%s","smBuildNhKwuLsapCfg");

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and RLC Loosely Coupled */
   lSap->mem.region   = smCb.init.region;
   lSap->mem.pool     = UEH_NH_POOL;
   lSap->prior        = PRIOR0;
   lSap->procId       = SM_KW_PROC;
   lSap->ent          = ENTKW;
   lSap->inst         = SM_INST_ZERO;
   lSap->route        = RTESPEC;
   lSap->spId         = 0;
   lSap->suId         = SM_ZERO_VAL;
   lSap->bndRetryCnt  = 0;
   lSap->bndTmr.enb   = TRUE;
   lSap->bndTmr.val   = 1000;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHKWUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "RRC: RLC Data SAP(STNHKWUSAP) Config Request sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);
   
   RETVALUE(ROK);
} /* smBuildNhKwuLsapCfg */

/*
*
*       Fun:   smBuildNhCkwLsapCfg
*
*       Desc:  Invoked to build LSAP CKW CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBuildNhCkwLsapCfg(
  Void
  )
{
   
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   logFunc("%s","smBuildNhCkwLsapCfg");

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and RLC Loosely Coupled */
   lSap->mem.region   = smCb.init.region;
   lSap->mem.pool     = UEH_NH_POOL;
   lSap->prior        = PRIOR0;
   lSap->procId       = SM_KW_PROC;
   lSap->ent          = ENTKW;
   lSap->inst         = SM_INST_ZERO;
   lSap->route        = RTESPEC;
   lSap->spId         = 0;
   lSap->suId         = SM_ZERO_VAL;
   lSap->bndRetryCnt  = 0;
   lSap->bndTmr.enb   = TRUE;
   lSap->bndTmr.val   = 1000;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHCKWSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "RRC: RLC Control SAP(STNHCKWSAP) Config Request sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);
   
   RETVALUE(ROK);
} /* smBuildNhCkwLsapCfg */

/*
*
*       Fun:   smBuildNhCrgLsapCfg
*
*       Desc:  Invoked to build LSAP CRG CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBuildNhCrgLsapCfg(
  Void
  )
{
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   logFunc("%s","smBuildNhCrgLsapCfg");

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and MAC Loosely Coupled */
   lSap->mem.region   = smCb.init.region;
   lSap->mem.pool     = UEH_NH_POOL;
   lSap->prior        = PRIOR0;
   lSap->procId       = SM_RG_PROC;
   lSap->ent          = ENTRG;
   lSap->inst         = SM_INST_ZERO;
   lSap->route        = RTESPEC;
   lSap->spId         = 0;
   lSap->suId         = 0;
   lSap->bndRetryCnt  = 0;
   lSap->bndTmr.enb   = TRUE;
   lSap->bndTmr.val   = 1000;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHCRGSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "RRC: MAC SAP(STNHCRGSAP) Config Request sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);
   
   RETVALUE(ROK);
} /* end of smBuildNhCrgLsapCfg */

/*
*
*       Fun:   smBuildNhCpjLsapCfg
*
*       Desc:  Invoked to build LSAP CPJ CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBuildNhCpjLsapCfg(
  Void
  )
{
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   logFunc("%s","smBuildNhCpjLsapCfg");

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and PDCP Loosely Coupled */
   lSap->mem.region   = smCb.init.region;
   lSap->mem.pool     = UEH_NH_POOL;
   lSap->prior        = PRIOR0;
   lSap->procId       = SM_PJ_PROC;
   lSap->ent          = ENTPJ;
   lSap->inst         = SM_INST_ZERO;
   lSap->route        = RTESPEC;
   lSap->spId         = 0;
   lSap->suId         = SM_ZERO_VAL;
   lSap->bndRetryCnt  = 0;
   lSap->bndTmr.enb   = TRUE;
   lSap->bndTmr.val   = 100;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHCPJSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "RRC: PDCP control SAP(STNHCPJSAP) Config Request sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);
   
   RETVALUE(ROK);
} /* smBuildNhCpjLsapCfg */

/*
*
*       Fun:   smBuildNhPjuLsapCfg
*
*       Desc:  Invoked to build LSAP PJU CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBuildNhPjuLsapCfg(
  Void
  )
{
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   logFunc("%s","smBuildNhPjuLsapCfg");

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and RLC Loosely Coupled */
   lSap->mem.region   = smCb.init.region;
   lSap->mem.pool     = UEH_NH_POOL;
   lSap->prior        = PRIOR0;
   lSap->procId       = SM_PJ_PROC;
   lSap->ent          = ENTPJ;
   lSap->inst         = SM_INST_ZERO;
   lSap->route        = RTESPEC;
   lSap->spId         = 0;   /* Vamsee */
   lSap->suId         = SM_ZERO_VAL;
   lSap->bndRetryCnt  = 0;
   lSap->bndTmr.enb   = TRUE;
   lSap->bndTmr.val   = 100;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHPJUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "RRC: PDCP Data SAP(STNHPJUSAP) Config Request sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);
   
   RETVALUE(ROK);
} /* smBuildNhPjuLsapCfg */

/*
*
*       Fun:   smBuildNhLsapCfg
*
*       Desc:  Invoked to build LSAP CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBuildNhLsapCfg(
  U8  elemId
  )
{
   S16 ret = ROK;

   logFunc("%s","smBuildNhLsapCfg");

   switch (elemId)
   {
      case STNHKWUSAP:
      {
         smBuildNhKwuLsapCfg();
         break;
      }
      case STNHCKWSAP:
      {
         smBuildNhCkwLsapCfg();
         break;
      }
      case STNHCRGSAP:
      {
         smBuildNhCrgLsapCfg();
         break;
      }
      case STNHCPJSAP:
      {
         smBuildNhCpjLsapCfg();
         break;
      }
      case STNHPJUSAP:
      {
         smBuildNhPjuLsapCfg();
         break;
      }
      default:
      {
        logError("Invalid SAP element for configuration in RRC:%d",elemId );
        ret = RFAILED;
        break;
      }
   }
   
   RETVALUE(ret);
}


/*
*
*       Fun:   smBindNhToKwuSap
*
*       Desc:  Invoked to bind RRC to KWU Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBindNhToKwuSap(
    Void
  )
{

   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
   
   logFunc("%s","smBindNhToKwuSap");

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  ABND_ENA;
   cntrl->u.sap.sapId   =  0;
   cntrl->subAction     =  SM_ZERO_VAL;


   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHKWUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "RRC: RLC User lower SAP(STNHKWUSAP) Bind request sent");

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);

} /* smBindNhToKwuSap */


/*
*
*       Fun:   smBindNhToCkwSap
*
*       Desc:  Invoked to bind RRC to CKW Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBindNhToCkwSap(
    Void
  )
{
   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;

   logFunc("%s","smBindNhToCkwSap");
   
   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  ABND_ENA;
   cntrl->u.sap.sapId   =  0;
   cntrl->subAction     =  SM_ZERO_VAL;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHCKWSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "RRC: RLC lower SAP(STNHCKWSAP) Bind request sent");

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBindNhToCkwSap */


/*
*
*       Fun:   smBindNhToCrgSap
*
*       Desc:  Invoked to bind RRC to CRG Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBindNhToCrgSap(
    Void
  )
{
   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;

   logFunc("%s","smBindNhToCrgSap");
   
   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  ABND_ENA;
   cntrl->u.sap.sapId   =  0;
   cntrl->subAction     =  SM_ZERO_VAL;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHCRGSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "RRC: MAC lower SAP(STNHCRGSAP) Bind request sent");

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBindNhToCrgSap */


/*
*
*       Fun:   smBindNhToCpjSap
*
*       Desc:  Invoked to bind RRC to CPJ Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBindNhToCpjSap(
    Void
  )
{
   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
   
   logFunc("%s","smBindNhToCpjSap");
   
   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  ABND_ENA;
   cntrl->u.sap.sapId   =  0;
   cntrl->subAction     =  SM_ZERO_VAL;


   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHCPJSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "RRC: RLC lower SAP(STNHCPJSAP) Bind request sent");

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBindNhToCpjSap */


/*
*
*       Fun:   smBindNhToPjuSap
*
*       Desc:  Invoked to bind RRC to PJU Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBindNhToPjuSap(
    Void
  )
{
   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
   
   logFunc("%s","smBindNhToPjuSap");

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  ABND_ENA;
   cntrl->u.sap.sapId   =  0;
   cntrl->subAction     =  SM_ZERO_VAL;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHPJUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "RRC: PDCP lower SAP(STNHPJUSAP) Bind request sent");

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBindNhToPjuSap */

/*
*
*       Fun:   smBindNhToLsap
*
*       Desc:  Invoked to bind RRC to LSAP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBindNhToLsap
(
U8  elemId
)
{
   S16   ret = ROK;

   logFunc("%s","smBindNhToLsap");

   switch (elemId)
   {
      case STNHKWUSAP:
         smBindNhToKwuSap();
         break;

      case STNHCKWSAP:
         smBindNhToCkwSap();
         break;

      case STNHCRGSAP:
         smBindNhToCrgSap();
         break;

      case STNHCPJSAP:
         smBindNhToCpjSap();
         break;

      case STNHPJUSAP:
         smBindNhToPjuSap();
         break;

      default:
         ret = RFAILED;
         logError ("%s", "smBindNhToLsap: Invalid element");
         break;
   } /* end of switch statement */

   RETVALUE(ret);
} /* end of smBindNhToLsap */

/*
*
*       Fun:   smBuildNhGenCntrl
*
*       Desc:  Invoked to Enable Alarms at RRC
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBuildNhAlarmCntrl(
    Void
  )
{
   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
  
   logFunc("%s","smBuildNhAlarmCntrl");
 
   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  AENA;
   cntrl->u.sap.sapId   =  0;
   cntrl->subAction     =  SAUSTA;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHGEN;
   nhMngmt.hdr.transId             = SM_RRC_ALARM_TRANS_ID;
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "Alarm control request sent to RRC(NH)");

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
}

/*
*
*       Fun:   smBuildNhDbgCntrl
*
*       Desc:  Invoked to Enable debugs at RRC
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_sm_nh_cntrl.c
*
*/

PUBLIC S16 smBuildNhDbgCntrl(
    LogConfig *logConfig
  )
{
   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
   U32                  nhdbgMask=0;
   
   logFunc("%s","smBuildNhDbgCntrl");
 
   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(logConfig->logMask1 & LOG_MASK_FUNCTION)
   {
      nhdbgMask = nhdbgMask | LNH_DBGMASK_DBG;
   }
   if(logConfig->logMask1 & LOG_MASK_ERROR )
   {
      nhdbgMask = nhdbgMask | LNH_DBGMASK_ERROR | LNH_DBGMASK_FATAL;
   }
   if(logConfig->logMask1 & LOG_MASK_DEBUG )
   {
      nhdbgMask = nhdbgMask | LNH_DBGMASK_INFO;
   }
   logInfo(":%s: DBG-Mask: %02x",__func__, nhdbgMask);
   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  ADISIMM;
   cntrl->subAction     =  SADBG;
   cntrl->u.dbg.dbgMsk  =  0xffffffff; /* TODO: Why is the debug mask not used ? */

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHGEN;
   nhMngmt.hdr.transId             = SM_RRC_DEBUG_TRANS_ID;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
   logInfo("%s", "Debug control request sent to RRC(NH)");

   /* Send the CTRL-request for resetting the loglevel to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   cntrl->action        =  AENA;
   cntrl->u.dbg.dbgMsk  =  nhdbgMask;
   /* Send the CTRL-request for setting the loglevel to the RRC with nhdbgMask*/
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBuildNhDbgCntrl */

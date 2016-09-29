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

/**********************************************************************

     Name:     LTE eNodeB Stack Manager 
  
     Type:     C Source file
  
     Desc:     Stack Manager Initialization Functions.

     File:     ve_sm_nh_cntrl.c

     Sid:      ve_sm_nh_cntrl.c@@/main/2 - Wed Dec  1 08:44:34 2010

     Prg:      an 

**********************************************************************/

/* header include files (.h) */
  
#include "envopt.h"           /* environment options */
#include "envdep.h"           /* environment dependent */
#include "envind.h"           /* environment independent */
  
#include "gen.h"              /* general layer */
#include "ssi.h"              /* system services */
#include "cm5.h"           /* common timers */
#include "cm_hash.h"       /* common hash */
#include "cm_inet.h"       /* common sockets */
#include "cm_tpt.h"        /* common transport addressing */
#include "cm_llist.h"
#include "ve_sm_init.h"
#include "ve.h"

  
/* header/extern include files (.x) */
  
#include "gen.x"              /* general layer */
#include "ssi.x"              /* system services */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash */
#include "cm_inet.x"       /* common sockets */
#include "cm_tpt.x"        /* common transport addressing */
#include "cm_llist.x"
#include "cm_lib.x"
#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif



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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smNhInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smNhInit(sysTskId)
SSTskId    sysTskId;
#endif
{
   TRC2(smNhInit);

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
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Tapa Task successfully \
            registered and attached to %d\n",sysTskId));
#endif

   RETVALUE(ROK);
} /* end of smNhInit */

/*
 *
 *       Fun:    veSmNhCfg - configure NH
 *
 *       Desc:
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ve_sm_nh_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void veSmNhCfg
(
Void
)
#else
PUBLIC Void veSmNhCfg(Void)
#endif /* ANSI */
{
   TRC2(veSmNhCfg)

   smBuildNhGenCfg();
   smBuildNhProtCfg();
   smBuildNhUsapCfg();
   smBuildNhLsapCfg(STNHKWUSAP);
   smBuildNhLsapCfg(STNHCKWSAP);
   smBuildNhLsapCfg(STNHCRGSAP);
   smBuildNhLsapCfg(STNHCPJSAP);
   smBuildNhLsapCfg(STNHPJUSAP);

   RETVOID;
} /* end of veSmNhCfg */



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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhGenCfg
(
)
#else
PUBLIC S16 smBuildNhGenCfg(Void)
#endif
{
   
   NhMngmt   nhMngmt;
   NhGenCfg  *genCfg = NULLP;
   Pst       pst;

   TRC2(smBuildNhGenCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   genCfg   = &(nhMngmt.u.cfg.u.gen);

   /*----------- Fill General Configuration Parameters ---------*/
   genCfg->maxCell      =  1;
   genCfg->msecTmrRes   = SM_DFLT_MSEC_TMR_CFG;   /* Milli Sec Timer Resolution */
   genCfg->cfgTmr.enb = FALSE;
#ifndef CNE_UE_SIM_TEST
   genCfg->cfgTmr.val  = 60;
#else 
   genCfg->cfgTmr.val  = 30;
#endif 
   genCfg->nmbKwuSaps = 1;
   genCfg->nmbCkwSaps = 1;
   genCfg->nmbCrgSaps = 1;
   genCfg->nmbCpjSaps = 1;
   genCfg->nmbPjuSaps = 1;
   genCfg->minTransLstLen = 2;
   genCfg->minMuiLstLen = 2;

#ifdef DEBUGP
   /* By Deafult Enabled*/
#ifndef CNE_UE_SIM_TEST
   genCfg->dbgMsk =  0x0;
#else 
   genCfg->dbgMsk =  0xFFFFFFFF;
#endif
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
   genCfg->lmPst.pool      = NH_POOL;
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
   nhMngmt.hdr.response.mem.pool   = NH_POOL;
   nhMngmt.hdr.response.selector   = SM_SELECTOR_LC;
   
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Gen Cfg Req sent\n"));
#endif

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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhProtCfg
(
Void
)
#else
PUBLIC S16 smBuildNhProtCfg(Void)
#endif
{

   NhMngmt    nhMngmt;
   NhProtCfg  *protCfg = NULLP;
   Pst       pst;

   TRC2(smBuildNhProtCfg)

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

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Prot Cfg Req sent\n"));
#endif

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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhUsapCfg
(
Void
)
#else
PUBLIC S16 smBuildNhUsapCfg(Void)
#endif
{

   NhMngmt    nhMngmt;
   NhUSapCfg  *uSap = NULLP;
   Pst       pst;

   TRC2(smBuildNhUsapCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   uSap   = &(nhMngmt.u.cfg.u.uSap);

   uSap->selector     = SM_SELECTOR_TC; /* Between RRC-NX Tight coupled */
   uSap->mem.region   = smCb.init.region;
   uSap->mem.pool     = NH_POOL;
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

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Nhu USap Cfg Req sent\n"));
#endif

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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhKwuLsapCfg
(
Void
)
#else
PUBLIC S16 smBuildNhKwuLsapCfg(Void)
#endif
{
   
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   TRC2(smBuildNhKwuLsapCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and RLC Loosely Coupled */
   lSap->mem.region   = smCb.init.region;
   lSap->mem.pool     = NH_POOL;
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
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Kwu LSap Cfg Req sent\n"));
#endif

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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhCkwLsapCfg
(
Void
)
#else
PUBLIC S16 smBuildNhCkwLsapCfg(Void)
#endif
{
   
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   TRC2(smBuildNhCkwLsapCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and RLC Loosely Coupled */
   lSap->mem.region   = smCb.init.region;
   lSap->mem.pool     = NH_POOL;
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
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Ckw LSap Cfg Req sent\n"));
#endif

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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhCrgLsapCfg
(
Void
)
#else
PUBLIC S16 smBuildNhCrgLsapCfg(Void)
#endif
{
   
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   TRC2(smBuildNhCrgLsapCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and MAC Loosely Coupled */
   lSap->mem.region   = smCb.init.region;
   lSap->mem.pool     = NH_POOL;
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
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Crg LSap Cfg Req sent\n"));
#endif

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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhCpjLsapCfg
(
Void
)
#else
PUBLIC S16 smBuildNhCpjLsapCfg(Void)
#endif
{
   
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   TRC2(smBuildNhCpjLsapCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and PDCP Loosely Coupled */
   lSap->mem.region   = smCb.init.region;
   lSap->mem.pool     = NH_POOL;
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
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Cpj LSap Cfg Req sent\n"));
#endif

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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhPjuLsapCfg
(
Void
)
#else
PUBLIC S16 smBuildNhPjuLsapCfg(Void)
#endif
{
   
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   TRC2(smBuildNhPjuLsapCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and RLC Loosely Coupled */
   lSap->mem.region   = smCb.init.region;
   lSap->mem.pool     = NH_POOL;
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
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Pju LSap Cfg Req sent \n"));
#endif

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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhLsapCfg
(
U8  elemId
)
#else
PUBLIC S16 smBuildNhLsapCfg (elemId)
U8  elemId;
#endif
{
   S16 ret = ROK;

   TRC2(smBuildNhLsapCfg)

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
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "smBindNhToLsap: Invalid element\n"));
#endif
         ret = RFAILED;
         break;
      }
   
   } /* end of switch */
   
   RETVALUE(ret);
} /* end of smBuildNhLsapCfg */


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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindNhToKwuSap
(
Void
)
#else
PUBLIC S16 smBindNhToKwuSap(Void)
#endif
{

   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
   
   TRC2(smBindNhToKwuSap)

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
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Cntrl Req to nh for Kwu Bind\n"));
#endif

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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindNhToCkwSap
(
Void
)
#else
PUBLIC S16 smBindNhToCkwSap(Void)
#endif
{

   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;

   TRC2(smBindNhToCkwSap)
   
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
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Cntrl Req to nh for Ckw Bind\n"));
#endif

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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindNhToCrgSap
(
Void
)
#else
PUBLIC S16 smBindNhToCrgSap(Void)
#endif
{

   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;

   TRC2(smBindNhToCrgSap)
   
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
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Cntrl Req to nh for Crg Bind\n"));
#endif

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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindNhToCpjSap
(
Void
)
#else
PUBLIC S16 smBindNhToCpjSap(Void)
#endif
{

   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
   
   TRC2(smBindNhToCpjSap)
   
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
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Cntrl Req to nh for Cpj Bind\n"));
#endif

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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindNhToPjuSap
(
Void
)
#else
PUBLIC S16 smBindNhToPjuSap(Void)
#endif
{
   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
   
   TRC2(smBindNhToPjuSap)

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
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Cntrl Req to nh for Pju Bind\n"));
#endif

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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindNhToLsap
(
U8  elemId
)
#else
PUBLIC S16 smBindNhToLsap (elemId)
U8  elemId;
#endif
{
   S16   ret = ROK;

   TRC2(smBindNhToLsap)

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
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "smBindNhToLsap: Invalid element"));
#endif
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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhGenCntrl
(
Void
)
#else
PUBLIC S16 smBuildNhGenCntrl(Void)
#endif
{
   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
  
   TRC2(smBuildNhGenCntrl)
 
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

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Gen Cntrl Req to nh\n"));
#endif

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBuildNhGenCntrl */

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
*       File:  ve_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhDbgCntrl
(
Void
)
#else
PUBLIC S16 smBuildNhDbgCntrl(Void)
#endif
{
   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
   
   TRC2(smBuildNhDbgCntrl)
 
   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  AENA;
   cntrl->subAction     =  SADBG;
#ifdef DEBUGP
   cntrl->u.dbg.dbgMsk  =  0xffffffff;
#endif

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Dbg Cntrl Req to nh\n"));
#endif

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBuildNhDbgCntrl */

/********************************************************************30**
  
         End of file:     ve_sm_nh_cntrl.c@@/main/2 - Wed Dec  1 08:44:34 2010
  
*********************************************************************31*/

/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/

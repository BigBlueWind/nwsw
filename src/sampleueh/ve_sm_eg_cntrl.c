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
/********************************************************************20**

     Name:    eGTP - acc test file

     Type:    C Include file

     Desc:    This file contains the acc source code

     File:    ve_sm_eg_cntrl.c

     Sid:      ve_sm_eg_cntrl.c@@/main/2 - Wed Dec  1 08:44:30 2010

     Prg:     an
*********************************************************************21*/

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_inet.h"       /* common INET defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport defines */

#include "leg.h"           /* layer management defines for EGCP */
#include "egt.h"            /* defines and macros for EG */

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#include "hi.h"
#endif

#include "ve_sm_init.h"
#include "ve.h"

/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_inet.x"       /* common INET */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_tpt.x"        /* common transport types */

#include "egt.x"           /* EGT Interface Structures */
#include "leg.x"           /* layer management typedefs for EGCP */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#include "hi.x"
#endif

#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif

/*
*
*       Fun:   Initialize E-GTP task.
*
*       Desc:  Invoked to create E-GTP Tapa task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_eg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smEgInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smEgInit(sysTskId)
SSTskId    sysTskId;
#endif
{
   TRC2(smEgInit)

   /* Register e-gtp TAPA Task */
   if (SRegTTsk((Ent)ENTEG, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 egActvInit, (ActvTsk)egActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach e-gtp TAPA Task */
   if (SAttachTTsk((Ent)ENTEG, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "EGTP Tapa Task successfully \
registered and attached to %d\n",sysTskId));
#endif

   RETVALUE(ROK);
} /* end of smEgInit */


/*
 *
 *       Fun:    veSmEgCfg - configure EG
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ve_sm_eg_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void veSmEgCfg
(
Void
)
#else
PUBLIC Void veSmEgCfg()
#endif /* ANSI */
{
   TRC2(veSmEgCfg)

   smBuildEgGenCfg();
   smBuildEgLSapCfg(EG_HI_SUID, EG_HI_SPID);
   smBuildEgUSapCfg(VE_EG_SPID);
   smBuildEgIpAddrCfg(VE_EG_SPID);
   smBuildTServerCfg();

   RETVOID;
} /* end of veSmEgCfg */



/*
*
*       Fun:   smBuildEgGenCfg
*
*       Desc:  fill in genCfg for EG
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_eg_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildEgGenCfg
(
Void
)
#else
PUBLIC S16 smBuildEgGenCfg()
#endif
{
   EgMngmt  egMgt;
   EgGenCfg    *cfg = NULLP;
   Pst       pst;

   TRC2(smBuildEgGenCfg)

   SM_SET_ZERO(&egMgt, sizeof(EgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(egMgt.u.cfg.s.gen);

   /*----------- Fill General Configuration Parameters ---------*/

   cfg->nmbUSaps     = 5;
   cfg->maxNmbTSaps  = 5;
   cfg->nmbSrvs      = 15;
   cfg->timerRes     = 1000;
   cfg->resThreshUpper= 9;
   cfg->resThreshLower= 1;
   cfg->lmPst.dstProcId  = SM_SM_PROC;
   cfg->lmPst.srcProcId  = SM_EG_PROC;
   cfg->lmPst.dstEnt     = ENTSM;
   cfg->lmPst.srcEnt     = ENTEG;
   cfg->lmPst.dstInst    = 0;
   cfg->lmPst.srcInst    = 0;
   cfg->lmPst.prior      = 0;
   cfg->lmPst.route      = 0;
   cfg->lmPst.event      = 0;
   cfg->lmPst.region     = 0;
   cfg->lmPst.pool       = 0;
   cfg->lmPst.selector   = SM_SELECTOR_LC;

#ifdef EGTP_U
   cfg->eguT3Timer.enb           = FALSE;
   cfg->eguT3Timer.val           = 15;
   cfg->nmbRetries               = 3;
   cfg->echoTmr.enb              = FALSE;
   cfg->echoTmr.val              = 60;
   cfg->reOrderTmr.enb           = TRUE;
   cfg->reOrderTmr.val           = 20;
   cfg->pvtExtId                 = 10;
   cfg->pduWindSize              = 20;
   cfg->maxNumOfIp               =  1;
   cfg->nodeArch                 =  1;
#ifdef EGTP_U_MULTI_THREADED
   cfg->nmbWorkerThread          =   3;
   cfg->loadDistTmr.enb           = TRUE;
   cfg->loadDistTmr.val           =  1;
#endif /* EGTP_U_MULTI_THREADED */
#endif /* EGTP_U */

   /* Fill Header */
   egMgt.hdr.msgType             = TCFG;
   egMgt.hdr.msgLen              = 0;
   egMgt.hdr.entId.ent           = ENTEG;
   egMgt.hdr.entId.inst          = SM_INST_ZERO;
   egMgt.hdr.elmId.elmnt         = STEGGEN;
   egMgt.hdr.elmId.elmntInst1    = 0;
   egMgt.hdr.elmId.elmntInst2    = 0;
   egMgt.hdr.elmId.elmntInst3    = 0;
   egMgt.hdr.seqNmb              = 0;
   egMgt.hdr.version             = 0;
   egMgt.hdr.transId             = 0;

   egMgt.hdr.response.prior      = PRIOR0;
   egMgt.hdr.response.route      = RTESPEC;
   egMgt.hdr.response.mem.region = smCb.init.region;
   egMgt.hdr.response.mem.pool   = EG_POOL;
   egMgt.hdr.response.selector   = SM_SELECTOR_LC;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTEG;
   pst.srcProcId = SM_SM_PROC;
   pst.dstProcId = SM_EG_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "EGTP Gen Cfg Req sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&pst, &egMgt);
 
   RETVALUE(ROK);
}/* smBuildEgGenCfg */


/*
*
*       Fun:   smBuildEgUSapCfg
*
*       Desc:  Call handler for SSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_eg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildEgUSapCfg
(
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildEgUSapCfg(spId)
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   EgMngmt  egMgt;
   EgUSapCfg    *cfg = NULLP;
   Pst      pst;

   TRC2(smBuildEgUSapCfg)
 
   SM_SET_ZERO(&egMgt, sizeof(EgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(egMgt.u.cfg.s.uSap);

   cfg->sapId      = 0;

#ifdef EGTP_C
   cfg->echo       = FALSE;
   cfg->intfType   = EGT_GTP_INT_S11;
#endif
   cfg->selector   = SM_SELECTOR_LWLC;
   cfg->mem.region =  DFLT_REGION;
   cfg->mem.pool   =  EG_POOL;
   cfg->priority   =  PRIOR0;
   cfg->route      =  RTESPEC;
   cfg->dstProcId  =  SM_VE_PROC;
   cfg->dstEnt     = ENTVE;
   cfg->dstInst    = 0;
   cfg->flag       = LEG_EGTP_U;

   /* Fill Header */
   egMgt.hdr.msgType             = TCFG;
   egMgt.hdr.msgLen              = 0;
   egMgt.hdr.entId.ent           = ENTEG;
   egMgt.hdr.entId.inst          = SM_INST_ZERO;
   egMgt.hdr.elmId.elmnt         = STEGUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTEG;
   pst.srcProcId = SM_SM_PROC;
   pst.dstProcId = SM_EG_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Egtp USap Cfg Req sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&pst, &egMgt);
 
   RETVALUE(ROK);
}/* smBuildEgUSapCfg */

/*
*
*       Fun:   smBuildEgLSapCfg
*
*       Desc:  Call handler for TSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_eg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildEgLSapCfg
(
SuId        suId,             /* E-GTP Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildEgLSapCfg(suId, spId)
SuId        suId;             /* E-GTP Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   EgMngmt  egMgt;
   EgTSapCfg  *cfg = NULLP;
   EgTSapReCfg *reCfg = NULLP;
   Pst       pst;

   TRC2(smBuildEgLSapCfg)
  
   SM_SET_ZERO(&egMgt, sizeof(EgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(egMgt.u.cfg.s.tSap);
   reCfg = &(egMgt.u.cfg.r.tSapReCfg);

   cfg->tSapId  = suId;
   cfg->spId    = spId;
   cfg->tptType = LEG_TSAP_TYPE_TUCL;

   cfg->selector     = VESM_EGHISEL;
   cfg->mem.region   = DFLT_REGION;
   cfg->mem.pool     = EG_POOL;
   cfg->priority     = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->dstProcId    = SM_HI_PROC;
   cfg->dstEnt       = ENTHI;
   cfg->dstInst      = (Inst)0;

   cfg->tInitTmr.enb = TRUE;
   cfg->tInitTmr.val = 10;

   cfg->tptParam.type = CM_TPTPARAM_SOCK;
   cfg->tptParam.u.sockParam.listenQSize = 5;
   cfg->tptParam.u.sockParam.numOpts     = 0;

   reCfg->tPar.type = CM_TPTPARAM_SOCK;
   reCfg->tPar.u.sockParam.listenQSize = 5;
   reCfg->tPar.u.sockParam.numOpts     = 0;
   reCfg->maxBndRetry  = 3;
   reCfg->bndTmCfg.enb = TRUE;
   reCfg->bndTmCfg.val = 300;

   /* Fill Header */
   egMgt.hdr.msgType             = TCFG;
   egMgt.hdr.msgLen              = 0;
   egMgt.hdr.entId.ent           = ENTEG;
   egMgt.hdr.entId.inst          = SM_INST_ZERO;
   egMgt.hdr.elmId.elmnt         = STEGTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTEG;
   pst.srcProcId = SM_SM_PROC;
   pst.dstProcId = SM_EG_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Egtp LSap Cfg Req sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&pst, &egMgt);
 
   RETVALUE(ROK);
}/* smBuildEgLSapCfg */


/*
*
*       Fun:   smBuildEgIpAddrCfg
*
*       Desc:  Call handler for IP Address Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_eg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildEgIpAddrCfg
(
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildEgIpAddrCfg(spId)
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   EgMngmt  egMgt;
   EgIpAddrCfg *cfg = NULLP;
   Pst       pst;

   TRC2(smBuildEgIpAddrCfg)

   SM_SET_ZERO(&egMgt, sizeof(EgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   /* Fill Header */
   egMgt.hdr.msgType             = TCFG;
   egMgt.hdr.msgLen              = 0;
   egMgt.hdr.entId.ent           = ENTEG;
   egMgt.hdr.entId.inst          = SM_INST_ZERO;
   egMgt.hdr.elmId.elmnt         = STEGIPADDR;

   cfg = &(egMgt.u.cfg.s.ipAddr);

   cfg->nmbIpAddr = 1;

   VESM_ALLOC(&(cfg->ipInfo), sizeof(EgIpInfo));
   /* klock warning fixed */
   if((cfg->ipInfo) == NULLP)
   {
#ifdef DEBUGP
      VE_LOGERROR(ERRCLS_DEBUG,0,ERRZERO,
            "Failure in the allocation of cfg\n");
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

   cfg->ipInfo[0].srcIpAddr.type                  = CM_TPTADDR_IPV4;
   cfg->ipInfo[0].srcIpAddr.u.ipv4TptAddr.port    = 0;
   cfg->ipInfo[0].srcIpAddr.u.ipv4TptAddr.address = smCfgCb.enbIpAddr;
   cfg->ipInfo[0].spId                            = spId;
   cfg->ipInfo[0].nodeType                        = TRUE;
   cfg->ipInfo[0].cfmStatus                       = FALSE;
   cfg->ipInfo[0].echoReqPvtExtVal.length         = 1;
   cfg->ipInfo[0].echoReqPvtExtVal.buff[0]        = 0x01;
   cfg->ipInfo[0].echoRspPvtExtVal.length         = 1;
   cfg->ipInfo[0].echoRspPvtExtVal.buff[0]        = 0x07;
   cfg->ipInfo[0].errIndPvtExtVal.length          = 1;
   cfg->ipInfo[0].errIndPvtExtVal.buff[0]         = 0x05;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTEG;
   pst.srcProcId = SM_SM_PROC;
   pst.dstProcId = SM_EG_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Egtp IpAddr Cfg sent\n"));
#endif

   /* Send the request to the LM */
   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&pst, &egMgt);

   RETVALUE(ROK);

}/* smBuildEgIpAddrCfg */

                                                                
/*
*
*       Fun:   smBuildTServerCfg
*
*       Desc:  Call handler for Transport server Cfg Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  egac_leghdl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildTServerCfg
(
Void
)
#else
PUBLIC S16 smBuildTServerCfg()
#endif
{
   EgMngmt  egMgt;
   Pst      pst;
   EgTptSrvCfg  *cfg = NULLP;

   TRC2(smBuildTServerCfg)

   SM_SET_ZERO(&egMgt, sizeof(EgMngmt));   
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(egMgt.u.cfg.s.tptSrv);

   cfg->tptSrvId                         = 1;
   cfg->tSapId                           = 0;
   cfg->uSapId                           = 0;
   cfg->tptAddr.type                     = CM_TPTADDR_IPV4;
   cfg->tptAddr.u.ipv4TptAddr.port       = VE_DFLT_EGTP_PORT; 
   /* Shwetha - giving the self address configured for this eNodeB for local endpoint */
   cfg->tptAddr.u.ipv4TptAddr.address = smCfgCb.enbIpAddr;  /* 0xAC1A001C */
   /* cmInetAddr ((S8 *)"127.0.0.1", &(cfg->tptAddr.u.ipv4TptAddr.address)); */
   /* cfg->tptAddr.u.ipv4TptAddr.address = CM_INET_NTOH_U32(cfg->tptAddr.u.ipv4TptAddr.address); */
   cfg->tptParam.u.sockParam.numOpts     = 0; /* 0 */
   cfg->tptParam.u.sockParam.listenQSize = 5; /* 5 */
   cfg->opnSrvRetryCnt                   = 3;
   cfg->opnSrvTmr.enb                    = TRUE;
   cfg->opnSrvTmr.val                    = 10;
#ifdef EGTP_C
   cfg->isDfltSrv                        = FALSE;
#endif
   cfg->hostName.pres                    = FALSE;
   cfg->hostName.len                     = (U16) cmStrlen((CONSTANT U8 *) "host.domain.com");
   cfg->gtp_C_Or_U                       = LEG_EGTP_U;

  /* Fill Header */
   egMgt.hdr.msgType             = TCFG;
   egMgt.hdr.msgLen              = 0;
   egMgt.hdr.entId.ent           = ENTEG;
   egMgt.hdr.entId.inst          = SM_INST_ZERO;
   egMgt.hdr.elmId.elmnt         = STEGSRVR;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTEG;
   pst.srcProcId = SM_SM_PROC;
   pst.dstProcId = SM_EG_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Egtp TServ Cfg sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&pst, &egMgt);

   RETVALUE(ROK);
} /* end of smBuildTServerCfg */ 

/*
 *
 *       Fun:    smBindEgToHitSap
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *       File:   ve_sm_eg_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void smBindEgToHitSap
(
SpId        spId
)
#else
PUBLIC Void smBindEgToHitSap(spId)
SpId        spId;
#endif /* ANSI */
{
   EgMngmt              egMgt;
   Pst                  pst;

   TRC2(smBindEgToHitSap)

   SM_SET_ZERO(&egMgt, sizeof(EgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   egMgt.hdr.msgType             = TCNTRL;
   egMgt.hdr.entId.ent           = ENTEG;
   egMgt.hdr.entId.inst          = SM_INST_ZERO;
   egMgt.hdr.elmId.elmnt         = STEGTSAP;

   egMgt.u.cntrl.action  = ABND_ENA;
   egMgt.u.cntrl.subAction  = SAELMNT;
   egMgt.u.cntrl.s.sap.sapId    = spId;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTEG;
   pst.dstProcId = SM_EG_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "EGTP Bnd Cntrl to eg for Hit sent\n"));
#endif

   (Void)SmMiLegCntrlReq(&pst, &egMgt);

   RETVOID;
} /* end of smBindEgToHitSap */

/*
 *
 *       Fun:    smBuildEgDbgCntrl
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *       File:   ve_sm_eg_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void smBuildEgDbgCntrl
(
Void
)
#else
PUBLIC Void smBuildEgDbgCntrl()
#endif /* ANSI */
{
   EgMngmt              egMgt;
   Pst                  pst;

   TRC2(smBuildEgDbgCntrl)

   SM_SET_ZERO(&egMgt, sizeof(EgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   egMgt.hdr.msgType             = TCNTRL;
   egMgt.hdr.entId.ent           = ENTEG;
   egMgt.hdr.entId.inst          = SM_INST_ZERO;
   egMgt.hdr.elmId.elmnt         = STEGGEN;

   egMgt.u.cntrl.action          =  AENA;
   egMgt.u.cntrl.subAction       =  SADBG;
#ifdef DEBUGP
   egMgt.u.cntrl.s.dbgCntrl.genDbgMask = 0xffffffff;
#endif

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTEG;
   pst.dstProcId = SM_EG_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "EGTP Dbg Cntrl to eg \n"));
#endif

   (Void)SmMiLegCntrlReq(&pst, &egMgt);

   RETVOID;
} /* end of smBuildEgDbgCntrl*/

/********************************************************************30**
  
         End of file:     ve_sm_eg_cntrl.c@@/main/2 - Wed Dec  1 08:44:30 2010
  
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

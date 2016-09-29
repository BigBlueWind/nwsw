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

     File:     ve_sm_cz_cntrl.c

     Sid:      ve_sm_cz_cntrl.c@@/main/eNB_1.3_With_LatestGpr3/7 - Tue Jul 19 13:38:42 2011

     Prg:      

**********************************************************************/
#ifdef LTE_HO_SUPPORT

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

#include "sct.h"           /* SB (SCTP) layer */
#include "lcz.h"           /* layer management defines for X2AP */

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#include "hi.h"
#endif

#ifdef    SB
#include "lsb.h"
#endif    /* SB */

#include "czt.h"            /* defines and macros for X2AP */
#include "ve_sm_init.h"
#include "ve.h"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif

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

#include "sct.x"           /* SCTP Interface Structures */
#include "czt.x"           /* SCTP Interface Structures */
#include "lcz.x"           /* X2AP types */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#include "hi.x"
#endif

#ifdef    SB
#include "lsb.x"
#endif    /* SB */

#include "ve_sm_init.x"



/*
*
*       Fun:   Initialize X2AP task.
*
*       Desc:  Invoked to create X2AP Tapa task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smCzInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smCzInit(sysTskId)
SSTskId    sysTskId;
#endif
{
   TRC2(smCzInit)
 
   /* Register X2AP TAPA Task */
   if (SRegTTsk((Ent)ENTCZ, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 czActvInit, (ActvTsk)czActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach X2AP TAPA Task */
   if (SAttachTTsk((Ent)ENTCZ, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "X2AP Tapa Task successfully \
registered and attached to %d\n",sysTskId));
#endif

   RETVALUE(ROK);
} /* end of smCzInit */

/*
 *
 *       Fun:    veSmCzCfg - configure CZ 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ve_sm_cz_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void veSmCzCfg
(
Void
)
#else
PUBLIC Void veSmCzCfg()
#endif /* ANSI */
{
   TRC2(veSmCzCfg)

   smBuildCzGenCfg();
   smBuildCzProtCfg();
   smBuildCzLSapCfg(CZ_SB_SUID,CZ_SB_SPID);
   smBuildCzUSapCfg(VE_CZ_SPID);
   smBuildCzPeerCfg();

   RETVOID;
} /* end of veSmCzCfg */


/*
*
*       Fun:   smBuildCzGenCfg
*
*       Desc:  fill in genCfg for CZ
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_cz_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzGenCfg
(
Void
)
#else
PUBLIC S16 smBuildCzGenCfg()
#endif
{
   CzMngmt  czMgt;
   CzGenCfg    *cfg = NULLP;
   Pst       pst;

   TRC2(smBuildCzGenCfg)

   SM_SET_ZERO(&czMgt, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(czMgt.t.cfg.u.gen);

   /*----------- Fill General Configuration Parameters ---------*/
   cfg->nmbCztSaps = (S16)VESM_LCZ_MAX_CZTSAP;
   cfg->nmbSctSaps = (S16)VESM_LCZ_MAX_SCTSAP;
   cfg->memUpperThr = DFLT_CZ_RES_THR_UP;
   cfg->memLowerThr = DFLT_CZ_RES_THR_LOW;
   cfg->maxNmbPeer = (U32) smCfgCb.maxX2Peers;
   cfg->timeRes = (Ticks)VESM_LCZ_TIMERES;

   cfg->lmPst.srcProcId = SM_CZ_PROC;
   cfg->lmPst.dstProcId = SM_SM_PROC;
   cfg->lmPst.srcEnt = (Ent)ENTCZ;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;
   cfg->lmPst.prior = (Prior)VESM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = DFLT_REGION;
   cfg->lmPst.pool =  CZ_POOL;
   cfg->lmPst.selector = (Selector)VESM_CZSMSEL;

   /* Fill Header */
   czMgt.hdr.msgType             = TCFG;
   czMgt.hdr.msgLen              = 0;
   czMgt.hdr.entId.ent           = ENTCZ;
   czMgt.hdr.entId.inst          = SM_INST_ZERO;
   czMgt.hdr.elmId.elmnt         = STCZGEN;
   czMgt.hdr.elmId.elmntInst1    = 0;
   czMgt.hdr.elmId.elmntInst2    = 0;
   czMgt.hdr.elmId.elmntInst3    = 0;
   czMgt.hdr.seqNmb              = 0;
   czMgt.hdr.version             = 0;
   czMgt.hdr.transId             = 0;

   czMgt.hdr.response.prior      = PRIOR0;
   czMgt.hdr.response.route      = RTESPEC;
   czMgt.hdr.response.mem.region = DFLT_REGION;
   czMgt.hdr.response.mem.pool   = CZ_POOL;
   czMgt.hdr.response.selector   = SM_SELECTOR_LC;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "X2AP Gen Cfg sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLczCfgReq(&pst, &czMgt);
 
   RETVALUE(ROK);
}/* smBuildCzGenCfg */

/*
*
*       Fun:   smBuildCzLSapCfg
*
*       Desc:  Call handler for TSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzLSapCfg
(
SuId        suId,             /* X2AP Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildCzLSapCfg(suId, spId)
SuId        suId;             /* X2AP Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   CzMngmt  czMgt;
   CzSctSapCfg  *cfg = NULLP;
   Pst       pst;
 
   TRC2(smBuildCzLSapCfg)

   SM_SET_ZERO(&czMgt, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(czMgt.t.cfg.u.sctSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->selector     = VESM_CZSBSEL;
   cfg->mem.region   = DFLT_REGION;
   cfg->mem.pool     = CZ_POOL;
   cfg->priority     = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->dstProcId    = SM_SB_PROC;
   cfg->dstEntId     = ENTSB;
   cfg->dstInstId    = (Inst)0;

   cfg->tBndTmr.enb = TRUE;
   cfg->tBndTmr.val = VESM_LCZ_VAL_10;
   cfg->maxBndRetry = VESM_LCZ_VAL_3;

   /* Fill Header */
   czMgt.hdr.msgType             = TCFG;
   czMgt.hdr.msgLen              = 0;
   czMgt.hdr.entId.ent           = ENTCZ;
   czMgt.hdr.entId.inst          = SM_INST_ZERO;
   czMgt.hdr.elmId.elmnt         = STCZSCTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "X2AP LSap Cfg sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLczCfgReq(&pst, &czMgt);
   RETVALUE(ROK);
}/* smBuildCzLSapCfg */


/*
*
*       Fun:   smBuildCzUSapCfg
*
*       Desc:  Call handler for SSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzUSapCfg
(
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildCzUSapCfg(spId)
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   CzMngmt  czMgt;
   CzCztSapCfg    *cfg = NULLP;
   Pst      pst;
 
   TRC2(smBuildCzUSapCfg)

   SM_SET_ZERO(&czMgt, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(czMgt.t.cfg.u.cztSap);

   cfg->spId   = spId;
   cfg->selector     = VESM_CZVESEL;
   cfg->mem.region   = DFLT_REGION;
   cfg->mem.pool     = CZ_POOL;
   cfg->priority     = PRIOR0;
   cfg->route        = RTESPEC;

   /* Fill Header */
   czMgt.hdr.msgType             = TCFG;
   czMgt.hdr.msgLen              = 0;
   czMgt.hdr.entId.ent           = ENTCZ;
   czMgt.hdr.entId.inst          = SM_INST_ZERO;
   czMgt.hdr.elmId.elmnt         = STCZCZTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "X2AP USap Cfg sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLczCfgReq(&pst, &czMgt);
   RETVALUE(ROK);
}/* smBuildCzUSapCfg */


/*
*
*       Fun:   smBuildCzProtCfg
*
*       Desc:  Call handler for Protocol Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzProtCfg
(
Void
)
#else
PUBLIC S16 smBuildCzProtCfg()
#endif
{
   CzMngmt  czMgt;
   CzProtCfg *protCfg = NULLP;
   Pst      pst;
 
   TRC2(smBuildCzProtCfg)

   SM_SET_ZERO(&czMgt, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   protCfg = &(czMgt.t.cfg.u.prot);

   protCfg->srcAddr.ipAddr.nmb  =   1;
   protCfg->srcAddr.ipAddr.nAddr[0].type  =   CM_IPV4ADDR_TYPE;
   protCfg->tRELOCprep = smCfgCb.x2PrepTimerVal;
   protCfg->tX2RELOCOverall = smCfgCb.x2OvrAllTimerVal;
   protCfg->timeToWait = smCfgCb.x2TimeToWaitTimerVal;
   protCfg->dynamPeerFlag = 1;
   protCfg->defDPeerUSapId = 0;
#ifdef CZ_ENB   
   protCfg->srcAddr.ipAddr.nAddr[0].u.ipv4NetAddr   =  smCfgCb.enbIpAddr;
   protCfg->eNbId     =   VESM_LCZ_ENB_PRC0;
   protCfg->srcAddr.sctPort =   VESM_LCZ_ENB_SCTPORT;
#endif /* CZ_ENB */   

   /* Fill Header */
   czMgt.hdr.msgType             = TCFG;
   czMgt.hdr.msgLen              = 0;
   czMgt.hdr.entId.ent           = ENTCZ;
   czMgt.hdr.entId.inst          = SM_INST_ZERO;
   czMgt.hdr.elmId.elmnt         = STCZPROT;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
 
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "X2AP Protocol Cfg sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLczCfgReq(&pst, &czMgt);
   RETVALUE(ROK);
}/* smBuildCzProtCfg */

/*
*
*       Fun:   smBuildCzPeerCfg
*
*       Desc:  Call handler for Peer Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzPeerCfg
(
Void
)
#else
PUBLIC S16 smBuildCzPeerCfg()
#endif
{
   CzMngmt  czMgt;
   CzPeerLstCfg *cfg = NULLP;
   U16      cnt;
   Pst      pst;
 
   TRC2(smBuildCzPeerCfg)

   SM_SET_ZERO(&czMgt, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(czMgt.t.cfg.u.peerLst);
   
   /* Due a bug in X2AP , noOfNghCfg must always be maxX2Peers -1 */
   cfg->nmbPeers = (U32)(smCfgCb.noOfNghCfg);
   if(VE_VAL_ZERO == cfg->nmbPeers)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "There are no X2AP Peer Cfg present now.\n"));
#endif
      RETVALUE(ROK);
   }

   VESM_ALLOC(&cfg->peerCfg, sizeof(CzPeerCfg)* cfg->nmbPeers);

   if(NULLP == cfg->peerCfg)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Failed to allocate memory.\n"));
#endif
      RETVALUE(RFAILED);
   }

   for (cnt=0; cnt < cfg->nmbPeers; cnt++)
   {
#ifdef CZ_ENB   
      cfg->peerCfg[cnt].peerId   =  smCfgCb.veNghCellCfg[cnt]->peerId; 
      cfg->peerCfg[cnt].tSapId   =  0; 
      cfg->peerCfg[cnt].uSapId   =  0;  
      cfg->peerCfg[cnt].eAValue  =  UNKNOWN_VALUE;
      /*cfg->peerCfg[cnt].iPAValue =  0;*/
      cfg->peerCfg[cnt].iPAValue =  UNKNOWN_VALUE;
      cfg->peerCfg[cnt].assocCfg.locOutStrms =  VESM_LCZ_OUT_STRMS;

      cfg->peerCfg[cnt].assocCfg.priDstAddr.type    =  CM_IPV4ADDR_TYPE;
      cfg->peerCfg[cnt].assocCfg.priDstAddr.u.ipv4NetAddr   =  smCfgCb.veNghCellCfg[cnt]->enbAddr;
      cfg->peerCfg[cnt].assocCfg.dstAddrLst.nmb     =  1;
      cfg->peerCfg[cnt].assocCfg.dstAddrLst.nAddr[0].type   =  CM_IPV4ADDR_TYPE;
      cfg->peerCfg[cnt].assocCfg.dstAddrLst.nAddr[0].u.ipv4NetAddr   =  smCfgCb.veNghCellCfg[cnt]->enbAddr;


      cfg->peerCfg[cnt].assocCfg.dstPort            =  VESM_LCZ_ENB_SCTPORT;/*Peer eNB's port number.*/

#endif /* CZ_ENB */
  }

   /* Fill Header */
   czMgt.hdr.msgType             = TCFG;
   czMgt.hdr.msgLen              = 0;
   czMgt.hdr.entId.ent           = ENTCZ;
   czMgt.hdr.entId.inst          = SM_INST_ZERO;
   czMgt.hdr.elmId.elmnt         = STCZPEER;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
 
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "X2AP Peer Cfg sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLczCfgReq(&pst, &czMgt);
   RETVALUE(ROK);
}/* smBuildCzPeerCfg */



/*
 *      FUN:   smBindCzToLSap
 *
 *      Desc:  Brings X2AP to the state before configuration
 *
 *      Ret:   void
 *
 *      Notes: None
 *
 *      File:  ve_sm_cz_cntrl.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindCzToLSap
(
void
)
#else
PUBLIC S16 smBindCzToLSap()
#endif /* ANSI */
{
   CzMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   TRC2(smBindCzToLSap)
   
   SM_SET_ZERO(&cntrl, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.t.cntrl.action        = ABND_ENA;
   cntrl.t.cntrl.subAction     = SAELMNT;
   cntrl.t.cntrl.u.sap.id      = VE_CZ_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTCZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STCZSCTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "X2AP Bind Req to cz LSap sent\n"));
#endif

    /* Send the control request to the X2AP */
   (Void)SmMiLczCntrlReq(&pst, &cntrl);
   RETVALUE(ret);
   RETVALUE(0);
} /* end of smBindCzToLSap */

/*
*
*       Fun:   smBuildCzGenCntrl
*
*       Desc:  Invoked to enable Alarm at X2AP.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzGenCntrl
(
)
#else
PUBLIC S16 smBuildCzGenCntrl(Void)
#endif
{
   CzMngmt cntrl;
   Pst      pst;
   
   TRC2(smBuildCzGenCntrl)
   
   SM_SET_ZERO(&cntrl, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl.t.cntrl.action        =  AENA;
   cntrl.t.cntrl.u.sap.id      =  0;
   cntrl.t.cntrl.subAction     =  SAUSTA;

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTCZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STCZGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "X2AP Gen Cntrl Req to cz sent\n"));
#endif

   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, &cntrl);
   RETVALUE(ROK);
} /* end of smBuildCzGenCntrl */

/*
*
*       Fun:   smBuildCzDbgCntrl
*
*       Desc:  Invoked to Enable Debugs at X2AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzDbgCntrl
(
)
#else
PUBLIC S16 smBuildCzDbgCntrl(Void)
#endif
{
   CzMngmt cntrl;
   Pst      pst;

   TRC2(smBuildCzDbgCntrl)
   
   SM_SET_ZERO(&cntrl, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl.t.cntrl.action        =  AENA;
   cntrl.t.cntrl.subAction     =  SADBG;
#ifdef DEBUGP
   cntrl.t.cntrl.u.dbg.dbgMask =  0xffffffff;
#endif

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTCZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STCZGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "X2AP Dbg Cntrl Req to cz sent\n"));
#endif

   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, &cntrl);
   RETVALUE(ROK);
} /* end of smBuildCzDbgCntrl */


/*
*
*       Fun:   smBuildCzShutDownCntrl
*
*       Desc:  Invoked to shut down the X2AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzShutDownCntrl
(
)
#else
PUBLIC S16 smBuildCzShutDownCntrl(Void)
#endif
{
   CzMngmt cntrl;
   Pst      pst;

   TRC2(smBuildCzShutDownCntrl)

   SM_SET_ZERO(&cntrl, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl.t.cntrl.action        =  ASHUTDOWN;
   cntrl.t.cntrl.subAction     =  NULLD;

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTCZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STCZGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "X2AP ShutDown Cntrl Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, &cntrl);
   RETVALUE(ROK);
} /* end of smBuildCzShutDownCntrl */    

/*
*
*       Fun:   smUbindCzFrmUSap
*
*       Desc:  Invoked to disable CZT SAP at X2AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smUbindCzFrmUSap
(
)
#else
PUBLIC S16 smUbindCzFrmUSap(Void)
#endif
{
   CzMngmt cntrl;
   Pst      pst;

   TRC2(smUbindCzFrmUSap)

   SM_SET_ZERO(&cntrl, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   cntrl.t.cntrl.action        =  AUBND_DIS;
   cntrl.t.cntrl.subAction     =  NULLD;

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTCZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STCZCZTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Upper SAP Disable+Unbind Cntrl Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, &cntrl);
   RETVALUE(ROK);
} /* end of smUbindCzFrmUSap */

/*
*
*       Fun:   smDelCzUSap
*
*       Desc:  Invoked to delete a CZT SAP at X2AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smDelCzUSap
(
)
#else
PUBLIC S16 smDelCzUSap(Void)
#endif
{
   CzMngmt cntrl;
   Pst      pst;

   TRC2(smDelCzUSap)

   SM_SET_ZERO(&cntrl, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   cntrl.t.cntrl.action        =  ADEL;
   cntrl.t.cntrl.subAction     =  NULLD;

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTCZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STCZCZTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Upper SAP Delete Cntrl Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, &cntrl);
   RETVALUE(ROK);
} /* end of smDelCzUSap */

/*
*
*       Fun:   smBuildCzLSapTrcCntrl
*
*       Desc:  Invoked to Enable\Disable Trace for a SCT SAP at X2AP.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzLSapTrcCntrl
(
U16 action
)
#else
PUBLIC S16 smBuildCzLSapTrcCntrl(action)
U16 action;
#endif
{
   CzMngmt cntrl;
   Pst      pst;

   TRC2(smBuildCzLSapTrcCntrl)

   SM_SET_ZERO(&cntrl, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   cntrl.t.cntrl.action        =  action;
   cntrl.t.cntrl.subAction     =  SATRC;
   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTCZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STCZSCTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Lower SAP Trace Cntrl Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, &cntrl);
   RETVALUE(ROK);
} /* end of smBuildCzLSapTrcCntrl */


/*
*
*       Fun:   smUbindCzFrmLSap
*
*       Desc:  Invoked to  CZT SAP at X2AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smUbindCzFrmLSap
(
)
#else
PUBLIC S16 smUbindCzFrmLSap(Void)
#endif
{
   CzMngmt cntrl;
   Pst      pst;

   TRC2(smUbindCzFrmLSap)

   SM_SET_ZERO(&cntrl, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   cntrl.t.cntrl.action        =  AUBND_DIS;
   cntrl.t.cntrl.subAction     =  SAELMNT;

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTCZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STCZSCTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Lower SAP Disable+Unbind Cntrl Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, &cntrl);
   RETVALUE(ROK);
} /* end of smUbindCzFrmLSap */

/*
*
*       Fun:   smDelCzLSap
*
*       Desc:  Invoked to delete a SCT SAP at X2AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smDelCzLSap
(
)
#else
PUBLIC S16 smDelCzLSap(Void)
#endif
{
   CzMngmt cntrl;
   Pst      pst;

   TRC2(smDelCzLSap)

   SM_SET_ZERO(&cntrl, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   cntrl.t.cntrl.action        =  ADEL;
   cntrl.t.cntrl.subAction     =  SAELMNT;
   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTCZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STCZSCTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Upper SAP Delete Cntrl Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, &cntrl);
   RETVALUE(ROK);
} /* end of smDelCzLSap */

/*
*
*       Fun:   smBuildCzPeerTrcCntrl
*
*       Desc:  Invoked to Enable\Disable Trace at a particular Peer.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzPeerTrcCntrl
(
)
#else
PUBLIC S16 smBuildCzPeerTrcCntrl(Void)
#endif
{
   CzMngmt cntrl;
   Pst      pst;

   TRC2(smBuildCzPeerTrcCntrl)

   SM_SET_ZERO(&cntrl, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   cntrl.t.cntrl.action        =  AENA;
   cntrl.t.cntrl.subAction     =  SATRC;
   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTCZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STCZPEER;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "PeerTrc Cntrl Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, &cntrl);
   RETVALUE(ROK);
} /* end of smBuildCzPeerTrcCntrl */


/*
*
*       Fun:   smDelCzPeer
*
*       Desc:  Invoked to delete a particular Peer.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smDelCzPeer
(
)
#else

PUBLIC S16 smDelCzPeer(Void)
#endif
{
   CzMngmt cntrl;
   Pst      pst;

   TRC2(smDelCzPeer)

   SM_SET_ZERO(&cntrl, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   cntrl.t.cntrl.action        =  ADEL;
   cntrl.t.cntrl.subAction     =  SAELMNT;
   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTCZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STCZPEER;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Peer Delete Cntrl Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, &cntrl);
   RETVALUE(ROK);
} /* end of smDelCzPeer */


/*
*
*       Fun:   smBuildCzEndPtCntrl
*
*       Desc:  Invoked to Enable\Disable SCTP End Point.
*
*       Ret:   ROK
*

*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzEndPtCntrl
(
)
#else

PUBLIC S16 smBuildCzEndPtCntrl(Void)
#endif
{
   CzMngmt cntrl;
   Pst      pst;

   TRC2(smBuildCzEndPtCntrl)

   SM_SET_ZERO(&cntrl, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   cntrl.t.cntrl.action        =  AENA;
   cntrl.t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTCZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STTPTSRV;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "End Point Cntrl Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, &cntrl);
   RETVALUE(ROK);
} /* end of smBuildCzEndPtCntrl */

/*
*
*       Fun:   smBuildCzAssocCntrl
*
*       Desc:  Invoked to change the primary destination IP Address.
*
*       Ret:   ROK
*

*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzAssocCntrl
(
)
#else

PUBLIC S16 smBuildCzAssocCntrl(Void)
#endif
{
   CzMngmt cntrl;
   Pst      pst;

   TRC2(smBuildCzAssocCntrl)

   SM_SET_ZERO(&cntrl, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   cntrl.t.cntrl.action        =  AMODIFY;
   cntrl.t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTCZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STASSOC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Associaion Cntrl Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, &cntrl);
   RETVALUE(ROK);
} /* end of smBuildCzAssocCntrl */

/*
*
*       Fun:   smBuildCzGenStsReq
*
*       Desc:  Invoked to send the General statistic request to X2AP.
*
*       Ret:   ROK
*

*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzGenStsReq
(
)
#else

PUBLIC S16 smBuildCzGenStsReq(Void)
#endif
{
   CzMngmt sts;
   Pst      pst;
   Action action;

   TRC2(smBuildCzGenStsReq)

   SM_SET_ZERO(&sts, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   action = NOZEROSTS;
   sts.t.cntrl.action        =  NULLD;
   sts.t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   sts.hdr.msgType             = TSTS;
   sts.hdr.entId.ent           = ENTCZ;
   sts.hdr.entId.inst          = SM_INST_ZERO;
   sts.hdr.elmId.elmnt         = STCZGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "General Statistics Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczStsReq(&pst, action, &sts);
   RETVALUE(ROK);
} /* end of smBuildCzGenStsReq */


/*
*
*       Fun:   smBuildCzPeerStsReq
*
*       Desc:  Invoked to send the Peer statistic request to X2AP.
*
*       Ret:   ROK
*

*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzPeerStsReq
(
)
#else

PUBLIC S16 smBuildCzPeerStsReq(Void)
#endif
{
   CzMngmt sts;
   Pst      pst;
   Action action;

   TRC2(smBuildCzPeerStsReq)

   SM_SET_ZERO(&sts, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   action = NOZEROSTS;
   sts.t.cntrl.action        =  NULLD;
   sts.t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   sts.hdr.msgType             = TSTS;
   sts.hdr.entId.ent           = ENTCZ;
   sts.hdr.entId.inst          = SM_INST_ZERO;
   sts.hdr.elmId.elmnt         = STCZPEER;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "General Statistics Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczStsReq(&pst, action, &sts);
   RETVALUE(ROK);
} /* end of smBuildCzStsReq */


/*
*
*       Fun:   smBuildCzGenStaReq
*
*       Desc:  Invoked to send the General status request to X2AP.
*
*       Ret:   ROK
*

*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzGenStaReq
(
)
#else

PUBLIC S16 smBuildCzGenStaReq(Void)
#endif
{
   CzMngmt sta;
   Pst      pst;

   TRC2(smBuildCzGenStaReq)

   SM_SET_ZERO(&sta, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   sta.t.cntrl.action        =  NULLD;
   sta.t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   sta.hdr.msgType             = TSSTA;
   sta.hdr.entId.ent           = ENTCZ;
   sta.hdr.entId.inst          = SM_INST_ZERO;
   sta.hdr.elmId.elmnt         = STCZGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "General Status Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczStaReq(&pst, &sta);
   RETVALUE(ROK);
} /* end of smBuildCzGenStaReq */

/*
*
*       Fun:   smBuildCzSidStaReq
*
*       Desc:  Invoked to send the Sysytem Id request to X2AP.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzSidStaReq
(
)
#else

PUBLIC S16 smBuildCzSidStaReq(Void)
#endif
{
   CzMngmt sta;
   Pst      pst;

   TRC2(smBuildCzSidStaReq)

   SM_SET_ZERO(&sta, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   sta.t.cntrl.action        =  NULLD;
   sta.t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   sta.hdr.msgType             = TSSTA;
   sta.hdr.entId.ent           = ENTCZ;
   sta.hdr.entId.inst          = SM_INST_ZERO;
   sta.hdr.elmId.elmnt         = STCZSID;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "System Id Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczStaReq(&pst, &sta);
   RETVALUE(ROK);
} /* end of smBuildCzSidStaReq */

/*
*
*       Fun:   smBuildCzUSapStaReq
*
*       Desc:  Invoked to send Upper Sap statistic request to X2AP.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzUSapStaReq
(
)
#else

PUBLIC S16 smBuildCzUSapStaReq(Void)
#endif
{
   CzMngmt sta;
   Pst      pst;

   TRC2(smBuildCzUSapStaReq)

   SM_SET_ZERO(&sta, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   sta.t.cntrl.action        =  NULLD;
   sta.t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   sta.hdr.msgType             = TSSTA;
   sta.hdr.entId.ent           = ENTCZ;
   sta.hdr.entId.inst          = SM_INST_ZERO;
   sta.hdr.elmId.elmnt         = STCZCZTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Upper SAP Statistics Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczStaReq(&pst, &sta);
   RETVALUE(ROK);
} /* end of smBuildCzUSapStaReq */

/*
*
*       Fun:   smBuildCzLSapStaReq
*
*       Desc:  Invoked to send Lower Sap statistic request to X2AP.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzLSapStaReq
(
)
#else

PUBLIC S16 smBuildCzLSapStaReq(Void)
#endif
{
   CzMngmt sta;
   Pst      pst;

   TRC2(smBuildCzLSapStaReq)

   SM_SET_ZERO(&sta, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   sta.t.cntrl.action        =  NULLD;
   sta.t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   sta.hdr.msgType             = TSSTA;
   sta.hdr.entId.ent           = ENTCZ;
   sta.hdr.entId.inst          = SM_INST_ZERO;
   sta.hdr.elmId.elmnt         = STCZSCTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Lower SAP Statistics Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczStaReq(&pst, &sta);
   RETVALUE(ROK);
} /* end of smBuildCzLSapStaReq */

/*
*
*       Fun:   smBuildCzPeerStaReq
*
*       Desc:  Invoked to send Peer statistic request to X2AP.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzPeerStaReq
(
)
#else

PUBLIC S16 smBuildCzPeerStaReq(Void)
#endif
{
   CzMngmt sta;
   Pst      pst;

   TRC2(smBuildCzPeerStaReq)

   SM_SET_ZERO(&sta, sizeof(CzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   sta.t.cntrl.action        =  NULLD;
   sta.t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   sta.hdr.msgType             = TSSTA;
   sta.hdr.entId.ent           = ENTCZ;
   sta.hdr.entId.inst          = SM_INST_ZERO;
   sta.hdr.elmId.elmnt         = STCZSCTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Lower SAP Statistics Req to cz sent\n"));
#endif
   /* Send the request to the X2AP */
   SmMiLczStaReq(&pst, &sta);
   RETVALUE(ROK);
} /* end of smBuildCzPeerStaReq */

#endif

/********************************************************************30**
  
         End of file:     ve_sm_cz_cntrl.c@@/main/eNB_1.3_With_LatestGpr3/7 - Tue Jul 19 13:38:42 2011
  
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

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

     File:     ve_sm_sz_cntrl.c

     Sid:      ve_sm_sz_cntrl.c@@/main/2 - Wed Dec  1 08:44:38 2010

     Prg:      an 

**********************************************************************/

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
#include "lsz.h"           /* layer management defines for S1AP */

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#include "hi.h"
#endif

#ifdef    SB
#include "lsb.h"
#endif    /* SB */

#include "szt.h"            /* defines and macros for S1AP */
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

#include "sct.x"           /* SCTP Interface Structures */
#include "szt.x"           /* SCTP Interface Structures */
#include "lsz.x"           /* S1AP types */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#include "hi.x"
#endif

#ifdef    SB
#include "lsb.x"
#endif    /* SB */

#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif


/*
*
*       Fun:   Initialize S1AP task.
*
*       Desc:  Invoked to create S1AP Tapa task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_sz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smSzInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smSzInit(sysTskId)
SSTskId    sysTskId;
#endif
{
   TRC2(smSzInit)
 
   /* Register S1AP TAPA Task */
   if (SRegTTsk((Ent)ENTSZ, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 szActvInit, (ActvTsk)szActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach S1AP TAPA Task */
   if (SAttachTTsk((Ent)ENTSZ, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP Tapa Task successfully \
registered and attached to %d\n",sysTskId));
#endif

   RETVALUE(ROK);
} /* end of smSzInit */

/*
 *
 *       Fun:    veSmSzCfg - configure SZ 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ve_sm_sz_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void veSmSzCfg
(
Void
)
#else
PUBLIC Void veSmSzCfg()
#endif /* ANSI */
{
   TRC2(veSmSzCfg)

   smBuildSzGenCfg();
   smBuildSzProtCfg();
#ifdef VE_SB_SCTP
   smBuildSzLSapCfg(SZ_SB_SPID, SZ_SB_SUID);
#else
   smBuildSzLSapCfg(SZ_HI_SPID, SZ_HI_SUID);
#endif
   smBuildSzUSapCfg(VE_SZ_SPID);
   smBuildSzPeerCfg();

   RETVOID;
} /* end of veSmSzCfg */


/*
*
*       Fun:   smBuildSzGenCfg
*
*       Desc:  fill in genCfg for SZ
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_sz_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildSzGenCfg
(
Void
)
#else
PUBLIC S16 smBuildSzGenCfg()
#endif
{
   SzMngmt  szMgt;
   SzGenCfg    *cfg = NULLP;
   Pst       pst;

   TRC2(smBuildSzGenCfg)

   SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(szMgt.u.cfg.u.gen);

   /*----------- Fill General Configuration Parameters ---------*/
   cfg->nmbSztSaps = (S16)VESM_LSZ_MAX_SZTSAP;
   cfg->nmbSctSaps = (S16)VESM_LSZ_MAX_SCTSAP;
   cfg->nmbConn = (U32)(VESM_LSZ_MAX_PEER * VESM_MAX_UE);
   cfg->nmbPeer = (U32) VESM_LSZ_MAX_PEER;
   cfg->timeRes = (S16)VESM_LSZ_TIMERES; /* klock warning fixed */

   cfg->lmPst.srcProcId = SM_SZ_PROC;
   cfg->lmPst.dstProcId = SM_SM_PROC;
   cfg->lmPst.srcEnt = (Ent)ENTSZ;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;
   cfg->lmPst.prior = (Prior)VESM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = DFLT_REGION;
   cfg->lmPst.pool =  SZ_POOL;
   cfg->lmPst.selector = (Selector)VESM_SZSMSEL;

   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZGEN;
   szMgt.hdr.elmId.elmntInst1    = 0;
   szMgt.hdr.elmId.elmntInst2    = 0;
   szMgt.hdr.elmId.elmntInst3    = 0;
   szMgt.hdr.seqNmb              = 0;
   szMgt.hdr.version             = 0;
   szMgt.hdr.transId             = 0;

   szMgt.hdr.response.prior      = PRIOR0;
   szMgt.hdr.response.route      = RTESPEC;
   szMgt.hdr.response.mem.region = DFLT_REGION;
   szMgt.hdr.response.mem.pool   = SZ_POOL;
   szMgt.hdr.response.selector   = SM_SELECTOR_LC;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP Gen Cfg sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);
 
   RETVALUE(ROK);
}/* smBuildSzGenCfg */

/*
*
*       Fun:   smBuildSzLSapCfg
*
*       Desc:  Call handler for TSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_sz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildSzLSapCfg
(
SuId        suId,             /* S1AP Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildSzLSapCfg(suId, spId)
SuId        suId;             /* S1AP Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   SzMngmt  szMgt;
   SzSctSapCfg  *cfg = NULLP;
   Pst       pst;
 
   TRC2(smBuildSzLSapCfg)

   SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(szMgt.u.cfg.u.sctSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->selector     = VESM_SZSBSEL;
   cfg->mem.region   = DFLT_REGION;
   cfg->mem.pool     = SZ_POOL;
   cfg->priority     = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->dstProcId    = SM_SB_PROC;
#ifdef VE_SB_SCTP
   cfg->dstEntId     = ENTSB;
#else
   cfg->dstEntId     = ENTHI;
#endif
   cfg->dstInstId    = (Inst)0;

   cfg->tBndTmr.enb = TRUE;
   cfg->tBndTmr.val = VESM_LSZ_VAL_10;
   cfg->maxBndRetry = VESM_LSZ_VAL_3;

#ifdef LSZV1
   /* Fill source address info */
   cfg->srcAddr.sctPort = (U16) smCfgCb.eNBsctpPortNum; 
   cfg->srcAddr.ipAddr.nmb = 1;
   cfg->srcAddr.ipAddr.nAddr[0].type = CM_IPV4ADDR_TYPE;
   cfg->srcAddr.ipAddr.nAddr[0].u.ipv4NetAddr = smCfgCb.enbIpAddr;
#endif /* LSZV1 */

   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZSCTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP LSap Cfg sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);
 
   RETVALUE(ROK);
}/* smBuildSzLSapCfg */


/*
*
*       Fun:   smBuildSzUSapCfg
*
*       Desc:  Call handler for SSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_sz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildSzUSapCfg
(
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildSzUSapCfg(spId)
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   SzMngmt  szMgt;
   SzSztSapCfg    *cfg = NULLP;
   Pst      pst;
 
   TRC2(smBuildSzUSapCfg)

   SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(szMgt.u.cfg.u.sztSap);

   cfg->spId   = spId;
   cfg->selector     = VESM_SZVESEL;
   cfg->mem.region   = DFLT_REGION;
   cfg->mem.pool     = SZ_POOL;
   cfg->priority     = PRIOR0;
   cfg->route        = RTESPEC;

   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZSZTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP USap Cfg sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);
 
   RETVALUE(ROK);
}/* smBuildSzUSapCfg */


/*
*
*       Fun:   smBuildSzProtCfg
*
*       Desc:  Call handler for Protocol Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_sz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildSzProtCfg
(
Void
)
#else
PUBLIC S16 smBuildSzProtCfg()
#endif
{
   SzMngmt  szMgt;
   SzProtCfg *protCfg = NULLP;
   Pst      pst;
 
   TRC2(smBuildSzProtCfg)

   SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   protCfg = &(szMgt.u.cfg.u.prot);

#ifndef LSZV1   
   /* Compilation error fix */
   protCfg->srcAddr.ipAddr.nmb  =   1;
   protCfg->srcAddr.ipAddr.nAddr[0].type  =   CM_IPV4ADDR_TYPE;
   protCfg->srcAddr.ipAddr.nAddr[0].u.ipv4NetAddr   =  smCfgCb.enbIpAddr;
   protCfg->srcAddr.sctPort =  smCfgCb.eNBsctpPortNum; 
   
#endif /* LSZV1 */
#ifdef SZ_ENB   
   protCfg->nodeType =   LSZ_NODE_ENB;
   protCfg->u.eNbId     =   VESM_LSZ_ENB_PRC0;
#endif /* SZ_ENB */   

   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZPROT;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;
 
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP Protocol Cfg sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);
 
   RETVALUE(ROK);
}/* smBuildSzProtCfg */

/*
*
*       Fun:   smBuildSzPeerCfg
*
*       Desc:  Call handler for Peer Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_sm_sz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildSzPeerCfg
(
Void
)
#else
PUBLIC S16 smBuildSzPeerCfg()
#endif
{
   SzMngmt  szMgt;
   SzPeerCfg *cfg = NULLP;
   U16      cnt;
   Pst      pst;
 
   TRC2(smBuildSzPeerCfg)

   SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(szMgt.u.cfg.u.peer);

   cfg->nmbPeers = (U32)smCfgCb.numMmes;

   VESM_ALLOC(&cfg->peerInfo, sizeof(SzPeerInfo)* cfg->nmbPeers);
   /* klock warning fixed */
   if(cfg->peerInfo == NULLP)
   {
#ifdef DEBUGP 
       VE_LOGERROR(ERRCLS_DEBUG,0,ERRZERO,
                 "Failure in the allocation of cfg\n");
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */


   for (cnt=0; cnt < cfg->nmbPeers; cnt++)
   {
#ifdef SZ_ENB   
      cfg->peerInfo[cnt].type      =  LSZ_NODE_MME;   
      cfg->peerInfo[cnt].peerId.pres   =  TRUE; 
      cfg->peerInfo[cnt].peerId.val    =  smCfgCb.mmeIdx[cnt]; 
      cfg->peerInfo[cnt].uSapId     =  VE_SZ_SPID;  
      cfg->peerInfo[cnt].sapId     =  VE_SZ_SUID;  
      cfg->peerInfo[cnt].assocCfg.locOutStrms        =  VESM_LSZ_OUT_STRMS;

      cfg->peerInfo[cnt].assocCfg.priDstAddr.type    =  CM_IPV4ADDR_TYPE;
      cfg->peerInfo[cnt].assocCfg.priDstAddr.u.ipv4NetAddr   =  smCfgCb.mmeIpAddr[cnt];
      cfg->peerInfo[cnt].assocCfg.dstAddrLst.nmb     =  1;
      cfg->peerInfo[cnt].assocCfg.dstAddrLst.nAddr[0].type   =  CM_IPV4ADDR_TYPE;
      cfg->peerInfo[cnt].assocCfg.dstAddrLst.nAddr[0].u.ipv4NetAddr   =  smCfgCb.mmeIpAddr[cnt];
      cfg->peerInfo[cnt].assocCfg.dstPort            =  (U16)smCfgCb.mmesctpPortNum;
      cfg->peerInfo[cnt].assocCfg.nmbRetry           =  VESM_LSZ_VAL_3;
      cfg->peerInfo[cnt].assocCfg.tmrAssocRecov.enb  =  TRUE;
      cfg->peerInfo[cnt].assocCfg.tmrAssocRecov.val  =  VESM_LSZ_VAL_50;

#endif /* SZ_ENB */
  }

   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZPEER;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;
 
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP Peer Cfg sent\n"));
#endif

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);
 
   RETVALUE(ROK);
}/* smBuildSzPeerCfg */



/*
 *      FUN:   smBindSzToLSap
 *
 *      Desc:  Brings S1AP to the state before configuration
 *
 *      Ret:   void
 *
 *      Notes: None
 *
 *      File:  ve_sm_sz_cntrl.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindSzToLSap
(
void
)
#else
PUBLIC S16 smBindSzToLSap()
#endif /* ANSI */
{
   SzMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;
   
   TRC2(smBindSzToLSap)
   
   SM_SET_ZERO(&cntrl, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sap.id       = VE_SZ_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTSZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STSZSCTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP Bind Req to sz LSap sent\n"));
#endif

    /* Send the control request to the S1AP */
   (Void)SmMiLszCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* end of smBindSzToLSap */

/*
*
*       Fun:   smBuildSzGenCntrl
*
*       Desc:  Invoked to Enable Alarms at S1AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_sz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildSzGenCntrl
(
)
#else
PUBLIC S16 smBuildSzGenCntrl(Void)
#endif
{
   SzMngmt cntrl;
   Pst      pst;
   
   TRC2(smBuildSzGenCntrl)
   
   SM_SET_ZERO(&cntrl, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl.u.cntrl.action        =  AENA;
   cntrl.u.cntrl.u.sap.id      =  0;
   cntrl.u.cntrl.subAction     =  SAUSTA;

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTSZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STSZGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP Gen Cntrl Req to sz sent\n"));
#endif

   /* Send the request to the S1AP */
   SmMiLszCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildSzGenCntrl */

/*
*
*       Fun:   smBuildSzDbgCntrl
*
*       Desc:  Invoked to Enable Debugs at S1AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_sz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildSzDbgCntrl
(
)
#else
PUBLIC S16 smBuildSzDbgCntrl(Void)
#endif
{
   SzMngmt cntrl;
   Pst      pst;

   TRC2(smBuildSzDbgCntrl)
   
   SM_SET_ZERO(&cntrl, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl.u.cntrl.action        =  AENA;
   cntrl.u.cntrl.subAction     =  SADBG;
#ifdef DEBUGP
   cntrl.u.cntrl.u.dbg.dbgMask =  0xffffffff;
#endif

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTSZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STSZGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP Dbg Cntrl Req to sz sent\n"));
#endif

   /* Send the request to the S1AP */
   SmMiLszCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildSzGenCntrl */

/********************************************************************30**
  
         End of file:     ve_sm_sz_cntrl.c@@/main/2 - Wed Dec  1 08:44:38 2010
  
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

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

     File:     ve_sm_hi_cntrl.c

     Sid:      ve_sm_hi_cntrl.c@@/main/2 - Wed Dec  1 08:44:31 2010

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

#include "lhi.h"
#include "hit.h"           /* HI layer */

#ifdef    SB
#include "lsb.h"
#endif    /* SB */

#include "ve_sm_init.h"
#include "ve.h"
#include "hit.h"            /* defines and macros for S1AP */

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
#include "hit.x"           /* SCTP Interface Structures */

#include "lhi.x"
#include "hit.x"           /* HI layer */

#ifdef    SB
#include "lsb.x"
#endif    /* SB */

#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif
/*
*
*       Fun:   Initialize TUCL task.
*
*       Desc:  Invoked to create TUCL Tapa task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_hi_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smHiInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smHiInit(sysTskId)
SSTskId    sysTskId;
#endif
{

   TRC2(smHiInit)

   /* Register TUCL TAPA Task */
   if (SRegTTsk((Ent)ENTHI, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 hiActvInit, (ActvTsk)hiActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach TUCL TAPA Task */
   if (SAttachTTsk((Ent)ENTHI, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "TUCL Tapa Task successfully \
registered and attached to %d\n",sysTskId));
#endif

   RETVALUE(ROK);
} /* end of smHiInit */

/*
 *
 *       Fun:    veSmHiCfg - configure HI
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ve_sm_hi_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void veSmHiCfg
(  
Void
)
#else
PUBLIC Void veSmHiCfg()
#endif /* ANSI */ 
{  
   TRC2(veSmHiCfg)

   smBuildHiGenCfg();
#ifdef VE_SB_SCTP
   smBuildHiTSapCfg(SB_HI_SPID);
#else
   smBuildHiTSapCfg(SZ_HI_SPID);
#endif
   smBuildHiTSapCfg(EG_HI_SPID);

   RETVOID;
} /* end of veSmHiCfg */

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
 *      File:  ve_sm_hi_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void smBuildHiGenCfg
(
Void
)
#else
PUBLIC Void smBuildHiGenCfg()
#endif /* ANSI */
{

   HiMngmt     hiMgt;
   HiGenCfg    *cfg = NULLP;
   Pst         pst;

   TRC2(smBuildHiGenCfg)

   SM_SET_ZERO(&hiMgt, sizeof(HiMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &hiMgt.t.cfg.s.hiGen;

   cfg->numSaps = VESM_HI_NUMBSAPS;
   cfg->numCons = VESM_HI_NMBCONS;
   cfg->numFdsPerSet = VESM_HI_FDS;
   cfg->numFdBins = VESM_HI_FDBINS;

   cfg->selTimeout = VESM_HI_SELTIMEOUT;
   cfg->numRawMsgsToRead = 80;

#if (defined(HI_LKSCTP))
   cfg->numAssocBins   =  5;
   cfg->maxInitReTx    =  5; 
   cfg->maxAssocReTx   =  8;
   cfg->maxPathReTx    =  4; 
#endif

   cfg->permTsk = VESM_HI_PERM_TSK_FLAG;
   cfg->schdTmrVal = VESM_HI_SCHDTMR_VAL;
   cfg->poolStrtThr = VESM_HI_STRT_THRESH;
   cfg->poolDropThr = VESM_HI_DRP_THRESH;
   cfg->poolStopThr = VESM_HI_STP_THRESH;
   cfg->timeRes = VESM_HI_TIME_RES;
   
   /* Madhur Adding pool configuration */
   cfg->initPool = 1;

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
   cfg->lmPst.prior = (Prior)VESM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = DFLT_REGION;
   cfg->lmPst.pool = HI_POOL;
   cfg->lmPst.selector = (Selector)VESM_HISMSEL;

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

#ifdef DEBUGP 
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "TUCL Gen Cfg Req sent\n"));
#endif
   (Void)SmMiLhiCfgReq(&pst, &hiMgt);
  
   RETVOID;
} /* end of smBuildHiGenCfg() */


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
 *      File:  ve_sm_hi_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void smBuildHiTSapCfg
(
SpId        spId
)
#else
PUBLIC Void smBuildHiTSapCfg(spId)
SpId        spId;
#endif /* ANSI */
{
   HiMngmt     hiMgt;
   HiSapCfg    *cfg;
   Pst         pst;

   TRC2(smBuildHiTSapCfg)

   SM_SET_ZERO(&hiMgt, sizeof(HiMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &hiMgt.t.cfg.s.hiSap;

   cfg->spId = spId;
   cfg->uiSel = 0;
   cfg->flcEnb = FALSE;
   cfg->txqCongStrtLim = VESM_HI_CONG_STRT;
   cfg->txqCongDropLim  = VESM_HI_CONG_DRP;
   cfg->txqCongStopLim = VESM_HI_CONG_STP;
   cfg->numBins = VESM_HI_NMB_HLBINS;
     
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

#if (defined(HI_LKSCTP))
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

#ifdef DEBUGP 
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "TUCL TSap Cfg Req sent\n"));
#endif
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
 *       File:   ve_sm_eg_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void smBuildHiDbgCntrl
(
Void
)
#else
PUBLIC Void smBuildHiDbgCntrl()
#endif /* ANSI */
{
   HiMngmt              hiMgt;
   Pst                  pst;

   TRC2(smBuildHiDbgCntrl)

   SM_SET_ZERO(&hiMgt, sizeof(HiMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   hiMgt.hdr.msgType             = TCNTRL;
   hiMgt.hdr.entId.ent           = ENTHI;
   hiMgt.hdr.entId.inst          = SM_INST_ZERO;
   hiMgt.hdr.elmId.elmnt         = STGEN;

   hiMgt.t.cntrl.action          =  AENA;
   hiMgt.t.cntrl.subAction       =  SADBG;
#ifdef DEBUGP
   hiMgt.t.cntrl.ctlType.hiDbg.dbgMask = 0xffffffff;
#endif

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTHI;
   pst.dstProcId = SM_HI_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP 
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "TUCL TSap Cntrl Req sent\n"));
#endif

   (Void)SmMiLhiCntrlReq(&pst, &hiMgt);

   RETVOID;
} /* end of smBuildHiDbgCntrl*/

/********************************************************************30**
  
         End of file:     ve_sm_hi_cntrl.c@@/main/2 - Wed Dec  1 08:44:31 2010
  
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

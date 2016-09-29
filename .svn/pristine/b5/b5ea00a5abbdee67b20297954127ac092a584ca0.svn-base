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

     File:     ve_sm_sb_cntrl.c

     Sid:      ve_sm_sb_cntrl.c@@/main/2 - Wed Dec  1 08:44:37 2010

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
#include "cm_dns.h"        /* common dns defines */

#include "sct.h"           /* SB (SCTP) layer */
#include "lsb.h"           /* layer management defines for S1AP */

#include "lhi.h"
#include "hit.h"           /* HI layer */

#include "lsb.h"
#include "ve_sm_init.h"
#include "ve.h"
#include <unistd.h>       /* CR 7528 root check */
#include <stdlib.h>
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
#include "cm_dns.x"        /* common dns types */

#include "sct.x"           /* SCTP Interface Structures */
#include "lsb.x"           /* S1AP types */

#include "lhi.x"
#include "hit.x"           /* HI layer */

#include "lsb.x"
#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif

PRIVATE U8      veSbCfg = 0;

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
*       File:  ve_sm_sb_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smSbInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smSbInit(sysTskId)
SSTskId    sysTskId;
#endif
{
   TRC2(smSbInit)

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

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SCTP Tapa Task successfully \
registered and attached to %d\n",sysTskId));
#endif

   RETVALUE(ROK);
} /* end of smSbInit */

/*
 *
 *       Fun:    veSmSbCfg - configure SB 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  ve_sm_sb_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void veSmSbCfg
(
Void
)
#else
PUBLIC Void veSmSbCfg()
#endif /* ANSI */
{
#ifdef SS_MULTIPLE_PROCS
   U16   cnt;
#endif
   TRC2(veSmSbCfg)

   veSbCfg = 0;
   smBuildSbGenCfg();
   smBuildSbTSapCfg(SB_HI_SPID, SB_HI_SUID);
   smBuildSbSctSapCfg(SZ_SB_SPID);
#ifdef LTE_HO_SUPPORT
   smBuildSbSctSapCfg(CZ_SB_SPID);
#endif

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
         File:   ve_sm_sb_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void smBuildSbGenCfg
(
Void
)
#else
PUBLIC Void smBuildSbGenCfg()
#endif /* ANSI */
{
   SbMgmt      sbMgt;
   SbGenCfg    *cfg = NULLP;
   Pst         pst;

   /* Corrected function name in trace macro */
   TRC2(smBuildSbGenCfg)

   SM_SET_ZERO(&sbMgt, sizeof(SbMgmt));
   SM_SET_ZERO(&pst, sizeof(Pst));


   cfg = &sbMgt.t.cfg.s.genCfg;
   /* Service Type made configurable */
   if(smCfgCb.sctpUdpServiceType == 0)
   {
      /*  CR ID- ccpu00116764-IOT changes*/
      /*S1-MME connection is IP/SCTP*/
      cfg->serviceType   = HI_SRVC_RAW_SCTP_PRIOR;
       /* CR 7528 root check */
      if (getuid())
      {
         VE_LOGERROR(ERRCLS_DEBUG,0,ERRZERO,
               "You must be root to open SCTP Association!\n");
         exit(0);
      }

   }
   else
   {
      cfg->serviceType   = HI_SRVC_UDP;       
   }

#ifdef LSB5
   cfg->srcUdpPort = 9898;
   cfg->dstUdpPort = 9898;
#endif /*LSB5*/
   
   cfg->maxNmbSctSaps     = 10;
   cfg->maxNmbTSaps       = 5;
   cfg->maxNmbAssoc       = 10;
   cfg->maxNmbEndp        = 10;
   cfg->maxNmbDstAddr     = (2 * cfg->maxNmbAssoc);
   cfg->maxNmbSrcAddr     = 10;
   cfg->maxNmbTxChunks    = 95000;
   cfg->maxNmbRxChunks    = 95000;
   cfg->maxNmbInStrms     = 10;
   cfg->maxNmbOutStrms    = 10;
   cfg->initARwnd         = 16000;
   cfg->mtuInitial        = 1500;
   cfg->mtuMinInitial     = 1500;
   cfg->mtuMaxInitial     = 1500;
   cfg->performMtu        = FALSE;
   cfg->timeRes           = 1;
   cfg->reConfig.maxInitReTx   = 8;
   cfg->reConfig.maxAssocReTx  = 20;
#ifndef CNE_UE_SIM_TEST
   cfg->reConfig.maxPathReTx   = 4;
#else
   cfg->reConfig.maxPathReTx   = 10;
#endif
   cfg->reConfig.altAcceptFlg  = FALSE;
   cfg->reConfig.keyTm         = 600;
   cfg->reConfig.alpha         = 12;
   cfg->reConfig.beta          = 25;

   cfg->smPst.srcProcId = SM_SM_PROC;
   cfg->smPst.dstProcId = SM_SB_PROC;
   cfg->smPst.srcEnt = (Ent)ENTSB;
   cfg->smPst.dstEnt = (Ent)ENTSM;
   cfg->smPst.srcInst = (Inst)0;
   cfg->smPst.dstInst = (Inst)0;

   cfg->smPst.prior = (Prior)VESM_MSGPRIOR;
   cfg->smPst.route = (Route)RTESPEC;
   cfg->smPst.event = (Event)EVTNONE;
   cfg->smPst.region = DFLT_REGION;
   cfg->smPst.pool =  SB_POOL;
   cfg->smPst.selector = (Selector)VESM_SBSMSEL;

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
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSB;
   pst.dstProcId = SM_SB_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SCTP Gen Cfg sent\n"));
#endif

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
 *       File:   ve_sm_sb_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void smBuildSbTSapCfg
(
SpId        spId,
SuId        suId
)
#else
PUBLIC Void smBuildSbTSapCfg(spId, suId)
SpId        spId;
SuId        suId;
#endif /* ANSI */
{
   SbMgmt      sbMgt;
   SbTSapCfg   *cfg = NULLP;
   Pst          pst;

   TRC2(smBuildSbTSapCfg)

   SM_SET_ZERO(&sbMgt, sizeof(SbMgmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   cfg = &(sbMgt.t.cfg.s.tSapCfg);

   cfg->swtch        = LSB_SW_RFC_REL0;
   cfg->sel          = VESM_SBHISEL;      /* Loosely coupled upper layer */
   cfg->ent          = ENTHI;              /* Loosely coupled upper layer */
   cfg->inst         = 0;                  /* Loosely coupled upper layer */
   cfg->procId       = SM_HI_PROC;       /* Loosely coupled upper layer */
   cfg->memId.region = DFLT_REGION;
   cfg->memId.pool   = SB_POOL;
   cfg->prior = PRIOR0;
   cfg->route = RTESPEC;
   cfg->suId  = suId;
   cfg->srcNAddrLst.nmb = 1;
   cfg->srcNAddrLst.nAddr[0].type = CM_NETADDR_IPV4;
   cfg->srcNAddrLst.nAddr[0].u.ipv4NetAddr = smCfgCb.sctpIpAddr;
   cfg->reConfig.spId         = spId;
   cfg->reConfig.tIntTmr      = 200;
   cfg->reConfig.maxBndRetry  = 2;

   /* Fill Header */
   sbMgt.hdr.msgType             = TCFG;
   sbMgt.hdr.msgLen              = 0;
   sbMgt.hdr.entId.ent           = ENTSB;
   sbMgt.hdr.entId.inst          = SM_INST_ZERO;
   sbMgt.hdr.elmId.elmnt         = STSBTSAP;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSB;
   pst.dstProcId = SM_SB_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SCTP TCfg sent \n"));
#endif

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
 *       File:   ve_sm_sb_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void smBuildSbSctSapCfg
(
SpId          spId
)
#else
PUBLIC Void smBuildSbSctSapCfg(spId)
SpId          spId;
#endif /* ANSI */
{
   SbMgmt               sbMgt;
   SbSctSapCfg   *cfg = NULLP;
   Pst           pst;

   TRC2(smBuildSbSctSapCfg)

   SM_SET_ZERO(&sbMgt, sizeof(SbMgmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(sbMgt.t.cfg.s.sctSapCfg);
   cfg->swtch        = LSB_SW_RFC_REL0;
   cfg->sel          = VESM_SZSBSEL;     /* Loosely coupled upper layer */
   cfg->memId.region =  DFLT_REGION;
   cfg->memId.pool   =  SB_POOL;
   cfg->prior = PRIOR0;
   cfg->route = RTESPEC;
   cfg->spId  = spId;
   cfg->reConfig.maxAckDelayTm = 2;  /* increased from 2;
                                         Equivalent of 200 ms, TBD*/
   cfg->reConfig.maxAckDelayDg = 5;
   cfg->reConfig.rtoInitial = 30;
   cfg->reConfig.rtoMin     = 10;
   cfg->reConfig.rtoMax     = 50;
   cfg->reConfig.freezeTm   = 15;
#ifdef LSB4
   cfg->reConfig.bundleTm = 1;
#endif /* LSB4 */
   cfg->reConfig.cookieLife = 50000;   /* increased from 500 */
   cfg->reConfig.intervalTm = 500;
#ifdef LSB8
   cfg->reConfig.maxBurst = 4;
   cfg->reConfig.maxHbBurst = 1;
   cfg->reConfig.t5SdownGrdTm = 5 * cfg->reConfig.rtoMax;
#endif /* LSB8 */
   cfg->reConfig.handleInitFlg = TRUE; 
   cfg->reConfig.negAbrtFlg = TRUE;
#ifndef CNE_UE_SIM_TEST
   cfg->reConfig.hBeatEnable= TRUE;
#else
   cfg->reConfig.hBeatEnable= FALSE;
#endif
   cfg->reConfig.flcUpThr   = 80000;    /* number of packets */
   cfg->reConfig.flcLowThr  = 30000;
 
   /* Fill Header */
   sbMgt.hdr.msgType             = TCFG;
   sbMgt.hdr.msgLen              = 0;
   sbMgt.hdr.entId.ent           = ENTSB;
   sbMgt.hdr.entId.inst          = SM_INST_ZERO;
   sbMgt.hdr.elmId.elmnt         = STSBSCTSAP;

  /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSB;
   pst.dstProcId = SM_SB_PROC;
   pst.srcProcId = SM_SM_PROC;
 
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SCTP Sct Sap Cfg sent\n"));
#endif

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
 *       File:   ve_sm_sb_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void smBindSbToLSap
(
SpId        spId
)
#else
PUBLIC Void smBindSbToLSap(spId)
SpId        spId;
#endif /* ANSI */
{
   SbMgmt               sbMgt;
   Pst                  pst;

   TRC2(smBindSbToLSap)

   SM_SET_ZERO(&sbMgt, sizeof(SbMgmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   sbMgt.hdr.msgType             = TCNTRL;
   sbMgt.hdr.entId.ent           = ENTSB;
   sbMgt.hdr.entId.inst          = SM_INST_ZERO;
   sbMgt.hdr.elmId.elmnt         = STSBTSAP;

   sbMgt.t.cntrl.action  = ABND_ENA;
   sbMgt.t.cntrl.subAction  = SAELMNT;
   sbMgt.t.cntrl.sapId    = spId;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSB;
   pst.dstProcId = SM_SB_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Sending Bnd Cntrl Req to sb Lsap\n"));
#endif

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
 *       File:   ve_sm_sb_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void smBuildSbDbgCntrl
(
Void
)
#else
PUBLIC Void smBuildSbDbgCntrl()
#endif /* ANSI */
{
   SbMgmt               sbMgt;
   Pst                  pst;

   TRC2(smBuildSbDbgCntrl)

   SM_SET_ZERO(&sbMgt, sizeof(SbMgmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   sbMgt.hdr.msgType             = TCNTRL;
   sbMgt.hdr.entId.ent           = ENTSB;
   sbMgt.hdr.entId.inst          = SM_INST_ZERO;
   sbMgt.hdr.elmId.elmnt         = STSBGEN;

   sbMgt.t.cntrl.action          = AENA;
   sbMgt.t.cntrl.subAction       = SADBG;
#ifdef DEBUGP
   sbMgt.t.cntrl.dbgMask         = 0xffffffff;
#endif /* end of DEBUGP */

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSB;
   pst.dstProcId = SM_SB_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Sending Dbg Cntrl Req to sb \n"));
#endif

   (Void)SmMiLsbCntrlReq(&pst, &sbMgt);

   RETVOID;
} /* end of smBuildSbDbgCntrl*/

/********************************************************************30**
  
         End of file:     ve_sm_sb_cntrl.c@@/main/2 - Wed Dec  1 08:44:37 2010
  
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

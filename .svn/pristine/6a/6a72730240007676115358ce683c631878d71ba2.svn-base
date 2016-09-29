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

     File:     ve_sm_ys_cntrl.c

     Sid:      ve_sm_ys_cntrl.c@@/main/2 - Wed Dec  1 08:44:40 2010

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
#include "cm_lte.h"

#include "sct.h"           /* SB (SCTP) layer */

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#include "hi.h"
#endif

#ifdef    SB
#include "lsb.h"
#endif    /* SB */

#include "lys.h"           /* layer management defines for CL */
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
#include "cm_lte.x"

#include "sct.x"           /* SCTP Interface Structures */
#include "lys.x"           /* CL types */

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
 *       Fun:    veSmYsCfg - configure YS
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ve_sm_ys_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void veSmYsCfg
(
Void
)
#else
PUBLIC Void veSmYsCfg()
#endif /* ANSI */
{
   TRC2(veSmYsCfg)

   smBuildYsGenCfg();
   smBuildYsTfuSapCfg(RG_YS_SUID, RG_YS_SPID);
   smBuildYsSchTfuSapCfg(RG_YS_SUID, RG_SCH_YS_SPID);
   smBuildYsCtfSapCfg(VE_YS_SUID, VE_YS_SPID);

   RETVOID;
} /* end of veSmYsCfg */



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
*       File:  ve_sm_ys_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildYsGenCfg
(
Void
)
#else
PUBLIC S16 smBuildYsGenCfg()
#endif
{
   YsMngmt     *ysMgt = NULLP;
   YsGenCfg    *cfg = NULLP;
   Pst         pst;

   TRC2(smBuildYsGenCfg)

   SGetSBuf(0, 0, (Data **)&ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(ysMgt->t.cfg.s.genCfg);

   /*----------- Fill General Configuration Parameters ---------*/
   cfg->maxTfuSaps  = (U16)VESM_LYS_MAX_TFU_SAPS;
   cfg->nmbUe       = (U16)VESM_LYS_NMB_UES;

   /*----------- Fill lmPst ---------*/
   cfg->lmPst.srcProcId = SM_YS_PROC;
   cfg->lmPst.dstProcId = SM_SM_PROC;
   cfg->lmPst.srcEnt = (Ent)ENTTF;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;
   cfg->lmPst.prior = (Prior)VESM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = smCb.init.region;
   cfg->lmPst.pool =  smCb.init.pool;
   cfg->lmPst.selector = (Selector)VESM_YSSMSEL;

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

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Sending Gen Cfg Req to ys\n"));
#endif
 
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
*       File:  ve_sm_ys_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildYsTfuSapCfg
(
SuId        suId,             /* CL Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildYsTfuSapCfg(suId, spId)
SuId        suId;             /* CL Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   YsMngmt      *ysMgt = NULLP;
   YsTfuSapCfg  *cfg = NULLP;
   Pst          pst;
 
   TRC2(smBuildYsTfuSapCfg)

   SGetSBuf(0, 0, (Data **)&ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(ysMgt->t.cfg.s.tfuSap);

   cfg->selector     = VESM_RGYSSEL;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = YS_POOL;
   cfg->prior        = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->procId       = SM_RG_PROC;
   cfg->ent          = ENTRG;
   cfg->inst         = (Inst)0;
   cfg->route        = RTESPEC;

   cfg->type         = (spId == RG_YS_SPID) ? LYS_TFU_USR_SAP :  
                                               LYS_TFU_SCH_SAP;
   cfg->suId         = suId;
   cfg->spId         = spId;
   cfg->type         = LYS_TFU_USR_SAP;
   cfg->cellId       = VESM_CELLID;
   
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

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Sending Tfu Sap Req to ys\n"));
#endif
 
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
*       File:  ve_sm_ys_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildYsSchTfuSapCfg
(
SuId        suId,             /* CL Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildYsSchTfuSapCfg(suId, spId)
SuId        suId;             /* CL Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   YsMngmt      *ysMgt = NULLP;
   YsTfuSapCfg  *cfg = NULLP;
   Pst          pst;
 
   TRC2(smBuildYsSchTfuSapCfg)

   SGetSBuf(0, 0, (Data **)&ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(ysMgt->t.cfg.s.tfuSap);

   cfg->selector     = VESM_RGYSSEL;
   cfg->selector     = 1;           //L2Split
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
   cfg->cellId       = VESM_CELLID;
   
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

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Sending Tfu Sap Req to ys\n"));
#endif
 
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
*       File:  ve_sm_ys_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildYsCtfSapCfg
(
SuId        suId,             /* CL Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildYsCtfSapCfg(suId, spId)
SuId        suId;             /* CL Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   YsMngmt      *ysMgt = NULLP;
   YsCtfSapCfg  *cfg = NULLP;
   Pst          pst;
 
   TRC2(smBuildYsCtfSapCfg)

   SGetSBuf(0, 0, (Data **)&ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(ysMgt, sizeof(YsMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(ysMgt->t.cfg.s.ctfSap);

   cfg->selector     = VESM_VEYSSEL;
   cfg->mem.region   = smCb.init.region;
   cfg->mem.pool     = YS_POOL;
   cfg->prior        = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->procId       = SM_VE_PROC;

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

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Sending Ctf Sap Req to ys\n"));
#endif
 
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
*       File:  ve_sm_ys_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildYsGenCntrl
(
)
#else
PUBLIC S16 smBuildYsGenCntrl(Void)
#endif
{
   YsMngmt  *cntrl = NULLP;
   Pst      pst;
   
   TRC2(smBuildYsGenCntrl)

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
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Sending Gen Cntrl Req to ys\n"));
#endif
 
   /* Send the request to the CL */
   SmMiLysCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildYsGenCntrl */
/* MSPD changes */
#if defined(VE_PICO) || defined(MSPD)
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
*       File:  ve_sm_ys_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildYsDbgCntrl
(
)
#else
PUBLIC S16 smBuildYsDbgCntrl(Void)
#endif
{
   YsMngmt  *cntrl = NULLP;
   Pst      pst;
   
   TRC2(smBuildYsDbgCntrl)

   SGetSBuf(0, 0, (Data **)&cntrl, sizeof(YsMngmt));
   SM_SET_ZERO(cntrl, sizeof(YsMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl->t.cntrl.action        =  AENA;
   cntrl->t.cntrl.subAction     =  SADBG;

/* MSPD changes */
#ifdef MSPD
   cntrl->t.cntrl.s.ysDbgCntrl.dbgMask = 0x1;
#else
   cntrl->t.cntrl.s.ysDbgCntrl.dbgMask = 0xffffffff;
#endif

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
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Sending Debug Cntrl Req to ys\n"));
#endif
 
   /* Send the request to the CL */
   SmMiLysCntrlReq(&pst, cntrl);

   RETVALUE(ROK);
} /* end of smBuildYsDbgCntrl */
#endif /* VE_PICO || MSPD */


/********************************************************************30**
  
         End of file:     ve_sm_ys_cntrl.c@@/main/2 - Wed Dec  1 08:44:40 2010
  
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

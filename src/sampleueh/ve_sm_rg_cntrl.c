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

     Name:     LTE Uu Stack Manager 
  
     Type:     C Source file
  
     Desc:     Stack Manager Initialization Functions.

     File:     ve_sm_rg_cntrl.c

     Sid:      ve_sm_rg_cntrl.c@@/main/2 - Wed Dec  1 08:44:36 2010

     Prg:      rer 

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

#define RG_POOL 0
  
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
 *       Fun:    veSmRgCfg - configure RG
 *
 *       Desc:
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ve_sm_rg_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void veSmRgCfg
(
Void
)
#else
PUBLIC Void veSmRgCfg(Void)
#endif /* ANSI */
{
   TRC2(veSmRgCfg)

   smBuildRgGenCfg();
   smBuildRgUsapCfg(STRGUSAP);
   smBuildRgUsapCfg(STCRGSAP);
   smBuildRgSchInsCfg();
   /* Build the TFU sap configuration of LTE MAC */
   smBuildRgTfuLsapCfg();

   RETVOID;
} /* end of veSmRgCfg */

/*
*
*       Fun:   smBuildRgGenCfg
*
*       Desc:  Invoked to build MAC GEN CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_rg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildRgGenCfg
(
)
#else
PUBLIC S16 smBuildRgGenCfg(Void)
#endif
{
   RgMngmt       *rgMngmt= NULLP;
   RgGenCfg      *genCfg=NULLP;
   Pst           pst;

   TRC2(smBuildRgGenCfg)

   SGetSBuf(0, 0, (Data **)&rgMngmt, sizeof(RgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(rgMngmt, sizeof(RgMngmt));

   genCfg   = &(rgMngmt->t.cfg.s.genCfg);

   /*----------- Fill General Configuration Parameters ---------*/
   
   genCfg->mem.region  = smCb.init.region;
   genCfg->mem.pool    = RG_POOL;
   genCfg->tmrRes     = 10;
   
   genCfg->lmPst.dstProcId = SM_SM_PROC;
   genCfg->lmPst.srcProcId = SM_RG_PROC;
   genCfg->lmPst.dstEnt    = ENTSM;
   genCfg->lmPst.dstInst   = 0;
   genCfg->lmPst.srcEnt    = ENTRG;
   genCfg->lmPst.srcInst   = 0;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = smCb.init.region;
   genCfg->lmPst.pool      = RG_POOL;
   genCfg->lmPst.selector  = SM_SELECTOR_LC;
   

   /* Fill Header */
   rgMngmt->hdr.msgType             = TCFG;
   rgMngmt->hdr.msgLen              = 0;
   rgMngmt->hdr.entId.ent           = ENTRG;
   rgMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt->hdr.elmId.elmnt         = STGEN;
   rgMngmt->hdr.seqNmb              = 0;
   rgMngmt->hdr.version             = 0;
   rgMngmt->hdr.transId             = 0;
  
   rgMngmt->hdr.response.prior      = PRIOR0;
   rgMngmt->hdr.response.route      = RTESPEC;
   rgMngmt->hdr.response.mem.region = smCb.init.region;
   rgMngmt->hdr.response.mem.pool   = RG_POOL;
   rgMngmt->hdr.response.selector   = SM_SELECTOR_LC;
   
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "MAC Gen Cfg Req sent\n"));
#endif

   /* Send the request to the RRC */
   SmMiLrgCfgReq(&pst, rgMngmt);

   RETVALUE(ROK);
} /* end of smBuildRgGenCfg */

/*
*
*       Fun:   smBuildCrgUsapCfg
*
*       Desc:  Invoked to build USAP CFG.(CRG)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_rg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCrgUsapCfg
(
)
#else
PUBLIC S16 smBuildCrgUsapCfg(Void)
#endif
{

   RgMngmt     *rgMngmt = NULLP;
   RgUpSapCfg  *uSap = NULLP;
   Pst         pst;

   TRC2(smBuildCrgUsapCfg)

   SGetSBuf(0, 0, (Data **)&rgMngmt, sizeof(RgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(rgMngmt, sizeof(RgMngmt));

   uSap   = &(rgMngmt->t.cfg.s.crgSap);
   
   uSap->mem.region = smCb.init.region;
   uSap->mem.pool   = RG_POOL;
   uSap->suId       = 0;
   uSap->spId       = 0;
   uSap->procId     = SM_NH_PROC;
   uSap->ent        = ENTNH;
   uSap->inst       = SM_INST_ZERO;
   uSap->prior      = PRIOR0;
   uSap->route      = RTESPEC;
   uSap->selector   = SM_SELECTOR_LC;

   /* Fill Header */
   rgMngmt->hdr.msgType             = TCFG;
   rgMngmt->hdr.entId.ent           = ENTRG;
   rgMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt->hdr.elmId.elmnt         = STCRGSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "MAC Crg USap Cfg Req sent\n"));
#endif

   /* Send the request to the RRC */
   SmMiLrgCfgReq(&pst, rgMngmt);
   
   RETVALUE(ROK);
}

/*
*
*       Fun:   smBuildRguUsapCfg
*
*       Desc:  Invoked to build USAP CFG.(RGU)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_rg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildRguUsapCfg
(
Void
)
#else
PUBLIC S16 smBuildRguUsapCfg(Void)
#endif
{

   RgMngmt     *rgMngmt= NULLP;
   RgUpSapCfg  *uSap = NULLP;
   Pst         pst;

   TRC2(smBuildRguUsapCfg)

   SGetSBuf(0, 0, (Data **)&rgMngmt, sizeof(RgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(rgMngmt, sizeof(RgMngmt));

   uSap   = &(rgMngmt->t.cfg.s.rguSap);
   
   uSap->mem.region = smCb.init.region;
   uSap->mem.pool = RG_POOL;
   uSap->suId       = 0;
   uSap->spId       = 0;
   uSap->procId     = SM_KW_PROC;
   uSap->ent        = ENTKW;
   uSap->inst       = SM_INST_ZERO;
   uSap->prior      = PRIOR0;
   uSap->route      = RTESPEC;
   uSap->selector   = SM_SELECTOR_TC;


   /* Fill Header */
   rgMngmt->hdr.msgType             = TCFG;
   rgMngmt->hdr.entId.ent           = ENTRG;
   rgMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt->hdr.elmId.elmnt         = STRGUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "MAC Rgu USap Cfg Req sent\n"));
#endif

   /* Send the request to the RRC */
   SmMiLrgCfgReq(&pst, rgMngmt);
   
   RETVALUE(ROK);
} /* end of smBuildRguUsapCfg */


/*
*
*       Fun:   smBuildRgUsapCfg
*
*       Desc:  Invoked to build USAP CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_rg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildRgUsapCfg
(
U8  elemId
)
#else
PUBLIC S16 smBuildRgUsapCfg (elemId)
U8  elemId;
#endif
{
   TRC2(smBuildRgUsapCfg)

   switch (elemId)
   {
      case STRGUSAP:
         smBuildRguUsapCfg();
         break;

      case STCRGSAP:
         smBuildCrgUsapCfg();
         break;

      default:
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "smBuildRgUsapCfg : Invalid element Id"));
#endif
         break;
   
   } /* end of switch statement */

   RETVALUE(ROK);
} /* end of smBuildRgUsapCfg */

/*
*
*       Fun:   smBuildRgSchInsCfg
*
*       Desc:  Invoked to build Scheduler Instance configuration.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_rg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildRgSchInsCfg
(
Void
)
#else
PUBLIC S16 smBuildRgSchInsCfg(Void)
#endif
{
   RgMngmt       *rgMngmt = NULLP;
   RgSchInstCfg  *cfg = NULLP;
   Pst           pst;
   /* Instance Id 1 is used for scheduler */
   Inst          instId = SCH_INST_ID;
   U8            numSaps = 1;

   TRC2(smBuildRgSchInsCfg)

   SGetSBuf(0, 0, (Data **)&rgMngmt, sizeof(RgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(rgMngmt, sizeof(RgMngmt));

   cfg = &(rgMngmt->t.cfg.s.schInstCfg);
 
   /* Filling of Instance Id */
   cfg->instId = instId;
   /* Filling of Gen config */
   cfg->genCfg.mem.region = smCb.init.region;
   cfg->genCfg.mem.pool = RG_POOL;
   cfg->genCfg.tmrRes = 10;

   cfg->genCfg.lmPst.dstProcId = SM_SM_PROC;
   cfg->genCfg.lmPst.srcProcId = SM_RG_PROC;
   cfg->genCfg.lmPst.dstEnt    = ENTSM;
   cfg->genCfg.lmPst.dstInst   = 0;
   cfg->genCfg.lmPst.srcEnt    = ENTRG;
   cfg->genCfg.lmPst.srcInst   = SCH_INST_ID;
   cfg->genCfg.lmPst.prior     = PRIOR0;
   cfg->genCfg.lmPst.route     = RTESPEC;
   cfg->genCfg.lmPst.region    = smCb.init.region;
   cfg->genCfg.lmPst.pool      = RG_POOL;
   cfg->genCfg.lmPst.selector  = SM_SELECTOR_LC;

   /* Filling of Number of RGR and TFU saps. Both are same and is 1 */
   cfg->numSaps = numSaps;

   /* Filling the RGR sap information */
   cfg->rgrSap[0].selector  = SM_SELECTOR_LC;
   cfg->rgrSap[0].mem.region= smCb.init.region;
   cfg->rgrSap[0].mem.pool  = RG_POOL;
   cfg->rgrSap[0].prior     = PRIOR0;
   cfg->rgrSap[0].procId    = SM_NX_PROC;
   cfg->rgrSap[0].ent       = ENTNX;
   cfg->rgrSap[0].inst      = SM_INST_ZERO;
   cfg->rgrSap[0].route     = RTESPEC;
   cfg->rgrSap[0].spId      = 0;
   cfg->rgrSap[0].suId      = 0;

   /* Filling the TFU sap information of the scheduler */
#ifdef  CL_MAC_LWLC
   cfg->tfuSap[0].selector  = SM_SELECTOR_TC;
#else
   cfg->tfuSap[0].selector  = SM_SELECTOR_LWLC;
#endif
   cfg->tfuSap[0].mem.region= smCb.init.region;
   cfg->tfuSap[0].mem.pool  = RG_POOL;
   cfg->tfuSap[0].prior     = PRIOR0;
   cfg->tfuSap[0].procId    = SM_TF_PROC;
   cfg->tfuSap[0].ent       = ENTTF;
   cfg->tfuSap[0].inst      = SM_INST_ZERO;
   cfg->tfuSap[0].route     = RTESPEC;
   cfg->tfuSap[0].spId      = 1;
   cfg->tfuSap[0].suId      = 0;
   cfg->tfuSap[0].bndTmr.enb= TRUE;
   cfg->tfuSap[0].bndTmr.val= 10;
 
   /* Fill Header */
   rgMngmt->hdr.msgType             = TCFG;
   rgMngmt->hdr.entId.ent           = ENTRG;
   rgMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt->hdr.elmId.elmnt         = STSCHINST;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.srcInst   = 0;
   pst.dstInst   = SCH_INST_ID;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "MAC Sch Cfg sent\n"));
#endif

   SmMiLrgSchCfgReq(&pst, rgMngmt); 

   RETVALUE(ROK);
} /* end of smBuildRgSchInsCfg */


/*
*
*       Fun:   smBuildRgTfuLsapCfg
*
*       Desc:  Invoked to build LSAP RGU CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_rg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildRgTfuLsapCfg
(
Void
)
#else
PUBLIC S16 smBuildRgTfuLsapCfg(Void)
#endif
{
   
   RgMngmt       *rgMngmt = NULLP;
   RgLowSapCfg   *lSap=NULLP;
   Pst           pst;

   TRC2(smBuildRgTfuLsapCfg)

   SGetSBuf(0, 0, (Data **)&rgMngmt, sizeof(RgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(rgMngmt, sizeof(RgMngmt));

   lSap   = &(rgMngmt->t.cfg.s.tfuSap);
   
   lSap->mem.region  = smCb.init.region;
   lSap->mem.pool    = RG_POOL;
   
   lSap->procId      = SM_TF_PROC;
   lSap->ent         = ENTTF;
   lSap->inst        = SM_INST_ZERO;
   lSap->spId        = 0;
   lSap->suId        = 0;
   lSap->bndTmr.enb  = TRUE;
   lSap->bndTmr.val  = 10;
   lSap->prior       = PRIOR0;
   lSap->route       = RTESPEC;
#ifndef CL_MAC_LWLC   
   lSap->selector    = SM_SELECTOR_TC;
#else
   lSap->selector    = SM_SELECTOR_LWLC;
#endif
   /* Fill Header */
   rgMngmt->hdr.msgType             = TCFG;
   rgMngmt->hdr.entId.ent           = ENTRG;
   rgMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt->hdr.elmId.elmnt         =  STTFUSAP; 

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "MAC Tfu LSap Cfg sent\n"));
#endif

   /* Send the request to the MAC*/
   SmMiLrgCfgReq(&pst, rgMngmt);
   
   RETVALUE(ROK);
}

/*
*
*       Fun:   smBindRgToTfuSap
*
*       Desc:  Invoked to bind MAC to TFU Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_rg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindRgToTfuSap
(
Void
)
#else
PUBLIC S16 smBindRgToTfuSap(Void)
#endif
{

   RgMngmt  *rgMngmt = NULLP;
   RgCntrl  *cntrl=NULLP;
   Pst      pst;
  
   TRC2(smBindRgToTfuSap)
 
   SGetSBuf(0, 0, (Data **)&rgMngmt, sizeof(RgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(rgMngmt, sizeof(RgMngmt));

   cntrl = &(rgMngmt->t.cntrl);

   cntrl->action             =  ABND;
   cntrl->subAction          =  SM_ZERO_VAL;
   cntrl->s.rgSapCntrl.suId  =  SM_ZERO_VAL;
   cntrl->s.rgSapCntrl.spId  =  SM_ZERO_VAL;


   /* Fill Header */
   rgMngmt->hdr.msgType             = TCNTRL;
   rgMngmt->hdr.entId.ent           = ENTRG;
   rgMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt->hdr.elmId.elmnt         = STTFUSAP; 

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "MAC Lrg Cntrl Req sent \n"));
#endif

   /* Send the request to the RRC */
   SmMiLrgCntrlReq(&pst, rgMngmt);

   RETVALUE(ROK);
} /* end of smBindRgToTfuSap */

/*
*
*       Fun:   smBindRgSchToTfuSap
*
*       Desc:  Invoked to bind Scheduler to TFU Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_rg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindRgSchToTfuSap
(
Void
)
#else
PUBLIC S16 smBindRgSchToTfuSap(Void)
#endif
{

   RgMngmt  *rgMngmt = NULLP;
   RgCntrl  *cntrl=NULLP;
   Pst      pst;

   TRC2(smBindRgSchToTfuSap)
   
   SGetSBuf(0, 0, (Data **)&rgMngmt, sizeof(RgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   SM_SET_ZERO(rgMngmt, sizeof(RgMngmt));

   cntrl = &(rgMngmt->t.cntrl);

   cntrl->action             =  ABND;
   cntrl->subAction          =  SM_ZERO_VAL;
   cntrl->s.rgSapCntrl.suId  =  SM_ZERO_VAL;
   cntrl->s.rgSapCntrl.spId  =  1;


   /* Fill Header */
   rgMngmt->hdr.msgType             = TCNTRL;
   rgMngmt->hdr.entId.ent           = ENTRG;
   rgMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt->hdr.elmId.elmnt         = STTFUSAP; 

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.srcInst   = 0;
   pst.dstInst   = SCH_INST_ID;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SCH Lrg Cntrl Req to Bind Tfu \n"));
#endif

   /* Send the request to the RRC */
   SmMiLrgSchCntrlReq(&pst, rgMngmt);

   RETVALUE(ROK);
} /* end of smBindRgSchToTfuSap */


/*
*
*       Fun:   smBuildRgGenCntrl
*
*       Desc:  Invoked to Enable Alarms at RRC
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_rg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildRgGenCntrl
(
Void
)
#else
PUBLIC S16 smBuildRgGenCntrl(Void)
#endif
{
   RgCntrl  *cntrl = NULLP;
   RgMngmt  *rgMngmt = NULLP;
   Pst      pst;
  
   TRC2(smBuildRgGenCntrl)
 
   SGetSBuf(0, 0, (Data **)&rgMngmt, sizeof(RgMngmt));
   SM_SET_ZERO(rgMngmt, sizeof(RgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(rgMngmt->t.cntrl);

   cntrl->action        =  AENA;
   cntrl->subAction     =  SAUSTA;

   /* Fill Header */
   rgMngmt->hdr.msgType             = TCNTRL;
   rgMngmt->hdr.entId.ent           = ENTRG;
   rgMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt->hdr.elmId.elmnt         = STGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "MAC Gen Cntrl Req sent\n"));
#endif

   /* Send the request to the RRC */
   SmMiLrgCntrlReq(&pst, rgMngmt);

   RETVALUE(ROK);
} /* end of smBuildRgGenCntrl */

/*
*
*       Fun:   smBuildRgDbgCntrl
*
*       Desc:  Invoked to Enable Debug at MAC
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_rg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildRgDbgCntrl
(
Void
)
#else
PUBLIC S16 smBuildRgDbgCntrl(Void)
#endif
{
   RgCntrl  *cntrl = NULLP;
   RgMngmt  *rgMngmt = NULLP;
   Pst      pst;

   TRC2(smBuildRgDbgCntrl)
   
   SGetSBuf(0, 0, (Data **)&rgMngmt, sizeof(RgMngmt));
   SM_SET_ZERO(rgMngmt, sizeof(RgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(rgMngmt->t.cntrl);

   cntrl->action        =  AENA;
   cntrl->subAction     =  SADBG;
/* MSPD changes */
#ifdef MSPD
   cntrl->s.rgDbgCntrl.dbgMask = 0;
#else
   cntrl->s.rgDbgCntrl.dbgMask = 0xffffffff;
#endif

   /* Fill Header */
   rgMngmt->hdr.msgType             = TCNTRL;
   rgMngmt->hdr.entId.ent           = ENTRG;
   rgMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt->hdr.elmId.elmnt         = STGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "MAC Dbg Cntrl Req\n"));
#endif

   /* Send the request to the RRC */
   SmMiLrgCntrlReq(&pst, rgMngmt);

   RETVALUE(ROK);
} /* end of smBuildRgGenCntrl */
/*CRID: ccpu00119280 Enabling Debug for MAC Scheduler*/
/*
*
*       Fun:   smBuildRgSchDbgCntrl
*
*       Desc:  Invoked to Enable Debug at MAC Scheduler
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_rg_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildRgSchDbgCntrl
(
Void
)
#else
PUBLIC S16 smBuildRgSchDbgCntrl(Void)
#endif
{
   RgCntrl  *cntrl = NULLP;
   RgMngmt  *rgMngmt = NULLP;
   Pst      pst;

   TRC2(smBuildRgSchDbgCntrl)

   SGetSBuf(0, 0, (Data **)&rgMngmt, sizeof(RgMngmt));
   SM_SET_ZERO(rgMngmt, sizeof(RgMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(rgMngmt->t.cntrl);

   cntrl->action        =  AENA;
   cntrl->subAction     =  SADBG;
   cntrl->s.rgDbgCntrl.dbgMask = 0xffffffff;

   /* Fill Header */
   rgMngmt->hdr.msgType             = TCNTRL;
   rgMngmt->hdr.entId.ent           = ENTRG;
   rgMngmt->hdr.entId.inst          = SM_INST_ZERO;
   rgMngmt->hdr.elmId.elmnt         = STGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTRG;
   pst.dstProcId = SM_RG_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.srcInst   = 0;
   pst.dstInst   = SCH_INST_ID;
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "MAC SCHEUDLER Dbg Cntrl Req\n"));
#endif

   /* Send the request to the RRC */
   SmMiLrgSchCntrlReq(&pst, rgMngmt);

   RETVALUE(ROK);
} /* end of  smBuildRgSchDbgCntrl*/

/********************************************************************30**
  
         End of file:     ve_sm_rg_cntrl.c@@/main/2 - Wed Dec  1 08:44:36 2010
  
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

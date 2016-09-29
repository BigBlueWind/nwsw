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

     File:     ve_sm_kw_cntrl.c

     Sid:      ve_sm_kw_cntrl.c@@/main/2 - Wed Dec  1 08:44:34 2010

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
 *       Fun:    veSmKwCfg - configure KW
 *
 *       Desc:
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ve_sm_kw_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void veSmKwCfg
(
Void
)
#else
PUBLIC Void veSmKwCfg(Void)
#endif /* ANSI */
{
   TRC2(veSmKwCfg)

   smBuildKwGenCfg();
   smBuildKwUsapCfg(STCKWSAP, ENTNH);
   smBuildKwUsapCfg(STKWUSAP, ENTNH);
   smBuildKwRguLsapCfg();

   RETVOID;
} /* end of veSmKwCfg */



/*
*
*       Fun:   smBuildKwGenCfg
*
*       Desc:  Invoked to build RLC GEN CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildKwGenCfg
(
)
#else
PUBLIC S16 smBuildKwGenCfg(Void)
#endif
{
   KwMngmt   kwMngmt;
   KwGenCfg  *genCfg = NULLP;
   Pst       pst;

   TRC2(smBuildKwGenCfg)

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   genCfg   = &(kwMngmt.t.cfg.s.gen);

   /*----------- Fill General Configuration Parameters ---------*/
   genCfg->maxUe       = 1000;
   genCfg->maxKwuSaps  = 2;
   /*MSPD changes */
   genCfg->timeRes     = 1; /* Changed the resolution from 10 to 1 */
   
   /*----------- Fill lmPst Parameters ---------*/
   genCfg->lmPst.dstProcId = SM_SM_PROC;
   genCfg->lmPst.srcProcId = SM_KW_PROC;
   genCfg->lmPst.dstEnt    = ENTSM;
   genCfg->lmPst.dstInst   = 0;
   genCfg->lmPst.srcEnt    = ENTKW;
   genCfg->lmPst.srcInst   = 0;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = smCb.init.region;
   genCfg->lmPst.pool      = KW_POOL;
   genCfg->lmPst.selector  = SM_SELECTOR_LC;
   
   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.msgLen              = 0;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STGEN;
   kwMngmt.hdr.seqNmb              = 0;
   kwMngmt.hdr.version             = 0;
   kwMngmt.hdr.transId             = 0;
   kwMngmt.hdr.response.prior      = PRIOR0;
   kwMngmt.hdr.response.route      = RTESPEC;
   kwMngmt.hdr.response.mem.region = smCb.init.region;
   kwMngmt.hdr.response.mem.pool   = smCb.init.pool;
   kwMngmt.hdr.response.selector   = SM_SELECTOR_LC;
   
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RLC Gen Cfg Req sent\n"));
#endif

   /* Send the request to the RLC */
   SmMiLkwCfgReq(&pst, &kwMngmt);

   RETVALUE(ROK);
} /* smBuildKwGenCfg */


/*
*
*       Fun:   smBuildCkwUsapCfg
*
*       Desc:  Invoked to build USAP CFG.(CKW)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCkwUsapCfg
(
)
#else
PUBLIC S16 smBuildCkwUsapCfg(Void)
#endif
{
   KwMngmt    kwMngmt;
   KwSapCfg   *uSap = NULLP;
   Pst       pst;

   TRC2(smBuildCkwUsapCfg)

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   uSap   = &(kwMngmt.t.cfg.s.sap);
  
   uSap->selector =  SM_SELECTOR_LC;
   uSap->mem.region = smCb.init.region;
   uSap->mem.pool = KW_POOL;
   uSap->mem.spare = 0;
   uSap->procId = SM_NH_PROC;
   uSap->ent = ENTNH;
   uSap->inst = SM_INST_ZERO;
   uSap->sapId = 0;
   uSap->bndTmrIntvl = 1000;
   uSap->priority = PRIOR0;
   uSap->route = RTESPEC;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STCKWSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RLC Ckw Upper Sap Cfg Req sent\n"));
#endif

   /* Send the request to the RLC */
   SmMiLkwCfgReq(&pst, &kwMngmt);
   
   RETVALUE(ROK);
} /* smBuildCkwUsapCfg */

/*
*
*       Fun:   smBuildKwuUsapCfg
*
*       Desc:  Invoked to build USAP CFG.(KWU -- NH/PJ)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildKwuUsapCfg
(
U8  ent
)
#else
PUBLIC S16 smBuildKwuUsapCfg (ent)
U8  ent;
#endif
{
   KwMngmt    kwMngmt;
   KwSapCfg   *uSap = NULLP;
   Pst        pst;

   TRC2(smBuildKwuUsapCfg)

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   uSap   = &(kwMngmt.t.cfg.s.sap);

   uSap->selector   = SM_SELECTOR_LC;
   uSap->mem.region = smCb.init.region;
   uSap->mem.pool = KW_POOL;
   uSap->mem.spare = 0;
   
   if (ent == ENTNH)
   {
      uSap->procId = SM_NH_PROC;
      uSap->ent = ENTNH;
      uSap->sapId = 0;
   }
   else
   {
      uSap->procId = SM_PJ_PROC;
      uSap->ent = ENTKW;
      uSap->sapId = 1;
   }
   
   uSap->inst = SM_INST_ZERO;
   uSap->bndTmrIntvl = 1000;
   uSap->priority = PRIOR0;
   uSap->route = RTESPEC;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STKWUSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RLC Kwu Upper Sap Cfg Req sent\n"));
#endif

   /* Send the request to the RLC */
   SmMiLkwCfgReq(&pst, &kwMngmt);
   
   RETVALUE(ROK);
} /* end of smBuildKwuUsapCfg */

/*
*
*       Fun:   smBuildKwUsapCfg
*
*       Desc:  Invoked to build USAP CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildKwUsapCfg
(
U8  elemId,
U8  ent
)
#else
PUBLIC S16 smBuildKwUsapCfg (elemId, ent)
U8  elemId;
U8  ent;
#endif
{
   S16 ret = ROK;

   TRC2(smBuildKwUsapCfg)

   switch (elemId)
   {
      case STCKWSAP:
      {
         smBuildCkwUsapCfg();
         break;
      }
      case STKWUSAP:
      {
         /* entity can be NH or PJ */
         smBuildKwuUsapCfg(ent);
         break;
      }
      default:
      {
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "smBuildKwUsapCfg:\
                          Invalid elementId "));
#endif
         ret = RFAILED;
         break;
      }
   } /* end of switch statement */

   RETVALUE(ret);
} /* end of smBuildKwUsapCfg */

/*
*
*       Fun:   smBuildKwRguLsapCfg
*
*       Desc:  Invoked to build LSAP RGU CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildKwRguLsapCfg
(
Void
)
#else
PUBLIC S16 smBuildKwRguLsapCfg(Void)
#endif
{
   
   KwMngmt    kwMngmt;
   KwSapCfg   *lSap = NULLP;
   Pst        pst;

   TRC2(smBuildKwRguLsapCfg)

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(kwMngmt.t.cfg.s.sap);
  
   lSap->mem.region  = smCb.init.region;
   lSap->mem.pool    = KW_POOL;
   lSap->mem.spare   = 0;
   lSap->procId      = SM_RG_PROC;
   lSap->ent         = ENTRG;
   lSap->inst        = SM_INST_ZERO;
   lSap->sapId       = 0;
   lSap->bndTmrIntvl = 10;
   lSap->priority    = PRIOR0;
   lSap->route       = RTESPEC;
   lSap->selector    = SM_SELECTOR_TC;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STRGUSAP; 

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RLC Rgu Lower Sap Cfg Req sent\n"));
#endif
   /* Send the request to the RRC */
   SmMiLkwCfgReq(&pst, &kwMngmt);
   
   RETVALUE(ROK);
} /* end of smBuildKwRguLsapCfg */

/*
*
*       Fun:   smBindKwToRguSap
*
*       Desc:  Invoked to bind RLC to RGU Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindKwToRguSap
(
Void
)
#else
PUBLIC S16 smBindKwToRguSap(Void)
#endif
{
   KwCntrl  *cntrl = NULLP;
   KwMngmt  kwMngmt;
   Pst      pst;
  
   TRC2(smBindKwToRguSap)
 
   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(kwMngmt.t.cntrl);

   cntrl->action            =  ABND;
   cntrl->subAction         =  SM_ZERO_VAL;
   cntrl->s.sapCntrl.suId   =  SM_ZERO_VAL;
   cntrl->s.sapCntrl.spId   =  SM_ZERO_VAL;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCNTRL;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = 186; /* ambiguous defines in lkw.h and lrg.h so direct hardcoded*/

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RLC Cntrl Req to rg \
for Rgu sap\n"));
#endif
   /* Send the request to the RRC */
   SmMiLkwCntrlReq(&pst, &kwMngmt);

   RETVALUE(ROK);
} /* smBindKwToRguSap */

/*
*
*       Fun:   smBuildKwDbgCntrl
*
*       Desc:  Invoked to enable debug prints
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildKwDbgCntrl
(
Void
)
#else
PUBLIC S16 smBuildKwDbgCntrl(Void)
#endif
{
   KwCntrl  *cntrl = NULLP;
   KwMngmt  kwMngmt;
   Pst      pst;
  
   TRC2(smBuildKwDbgCntrl)
 
   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(kwMngmt.t.cntrl);

   cntrl->action            =  AENA;
   cntrl->subAction         =  SADBG;
   /*MSPD changes */
#ifdef MSPD
   cntrl->s.dbgCntrl.dbgMask = 0;
#else
   cntrl->s.dbgCntrl.dbgMask = 0xffffffff;
#endif

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCNTRL;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STGEN; 

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;
   
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RLC Debug Cntrl Req sent to rg\n"));
#endif
   /* Send the request to the RRC */
   SmMiLkwCntrlReq(&pst, &kwMngmt);

   RETVALUE(ROK);

} /* smBuildKwDbgCntrl */

/********************************************************************30**
  
         End of file:     ve_sm_kw_cntrl.c@@/main/2 - Wed Dec  1 08:44:34 2010
  
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

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

     File:     ve_sm_pj_cntrl.c

     Sid:      ve_sm_pj_cntrl.c@@/main/2 - Wed Dec  1 08:44:35 2010

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
#include "lkw.h"
#include "lpj.h"
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
#include "lkw.x"
#include "lpj.x"
#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif

/*
 *
 *       Fun:    veSmPjCfg - configure PJ
 *
 *       Desc:
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    ve_sm_pj_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void veSmPjCfg
(
Void
)
#else
PUBLIC Void veSmPjCfg(Void)
#endif /* ANSI */
{
   TRC2(veSmPjCfg)

   smBuildPjGenCfg();
   smBuildPjCpjSapCfg();
   smBuildPjPjuSapCfg(VE_PJ_SPID);
   smBuildPjPjuSapCfg(NH_PJ_SPID);

   RETVOID;
} /* end of veSmPjCfg */

/*
*
*       Fun:   smBuildPjGenCfg
*
*       Desc:  Invoked to build PDCP GEN CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_pj_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildPjGenCfg
(
)
#else
PUBLIC S16 smBuildPjGenCfg(Void)
#endif
{
   PjMngmt       pjMngmt;
   PjGenCfg      *genCfg=NULLP;
   Pst           pst;

   TRC2(smBuildPjGenCfg);

   SM_SET_ZERO(&pjMngmt, sizeof(PjMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   genCfg   = &(pjMngmt.t.cfg.s.gen);
   genCfg->maxPjuSaps = 2;
   genCfg->nmbUlBins  = 10;
   genCfg->nmbDlBins  = 10;
   genCfg->lmPst.dstProcId = SM_SM_PROC;
   genCfg->lmPst.srcProcId = SM_PJ_PROC;
   genCfg->lmPst.dstEnt    = ENTSM;
   genCfg->lmPst.dstInst   = 0;
   genCfg->lmPst.srcEnt    = ENTPJ;
   genCfg->lmPst.srcInst   = 0;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = smCb.init.region;
   genCfg->lmPst.pool      = PJ_POOL;
   genCfg->lmPst.selector  = SM_SELECTOR_LC;

#ifdef PJ_ASYNC_HOOK
   genCfg->obdPst.cmpPst.dstProcId = SM_SM_PROC;
   genCfg->obdPst.cmpPst.srcProcId = SM_PJ_PROC;
   genCfg->obdPst.cmpPst.dstEnt    = ENTSM;
   genCfg->obdPst.cmpPst.dstInst   = 0;
   genCfg->obdPst.cmpPst.srcEnt    = ENTPJ;
   genCfg->obdPst.cmpPst.srcInst   = 0;
   genCfg->obdPst.cmpPst.prior     = PRIOR0;
   genCfg->obdPst.cmpPst.route     = RTESPEC;
   genCfg->obdPst.cmpPst.region    = smCb.init.region;
   genCfg->obdPst.cmpPst.pool      = PJ_POOL;
   genCfg->obdPst.cmpPst.selector  = SM_SELECTOR_LC;

   genCfg->obdPst.secPst.dstProcId = SM_SM_PROC;
   genCfg->obdPst.secPst.srcProcId = SM_PJ_PROC;
   genCfg->obdPst.secPst.dstEnt    = ENTSM;
   genCfg->obdPst.secPst.dstInst   = 0;
   genCfg->obdPst.secPst.srcEnt    = ENTPJ;
   genCfg->obdPst.secPst.srcInst   = 0;
   genCfg->obdPst.secPst.prior     = PRIOR0;
   genCfg->obdPst.secPst.route     = RTESPEC;
   genCfg->obdPst.secPst.region    = smCb.init.region;
   genCfg->obdPst.secPst.pool      = PJ_POOL;
   genCfg->obdPst.secPst.selector  = SM_SELECTOR_LC;

   genCfg->obdTmrInt        = 50;
   genCfg->obdWtTmrInt      = 50;
#endif  /* PJ_ASYNC_HOOK */
   genCfg->nmbUlBins        = 100;
   genCfg->nmbDlBins        = 100;

   /* Fill Header */
   pjMngmt.hdr.msgType             = TCFG;
   pjMngmt.hdr.msgLen              = 0;
   pjMngmt.hdr.entId.ent           = ENTPJ;
   pjMngmt.hdr.entId.inst          = SM_INST_ZERO;
   pjMngmt.hdr.elmId.elmnt         = STGEN;
   pjMngmt.hdr.seqNmb              = 0;
   pjMngmt.hdr.version             = 0;
   pjMngmt.hdr.transId             = 0;

   pjMngmt.hdr.response.prior      = PRIOR0;
   pjMngmt.hdr.response.route      = RTESPEC;
   pjMngmt.hdr.response.mem.region = smCb.init.region;
   pjMngmt.hdr.response.mem.pool   = PJ_POOL;
   pjMngmt.hdr.response.selector   = SM_SELECTOR_LC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTPJ;
   pst.dstProcId = SM_PJ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "PDCP Gen Cfg Req sent\n"));
#endif

   /* Send the request to the RRC */
   SmMiLpjCfgReq(&pst, &pjMngmt);

   RETVALUE(ROK);
}


/*
*
*       Fun:   smBuildPjCpjSapCfg
*
*       Desc:  Invoked to build PDCP CPJ SAP Config (CPJ).
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_pj_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildPjCpjSapCfg
(
)
#else
PUBLIC S16 smBuildPjCpjSapCfg(Void)
#endif
{
   
   PjMngmt       pjMngmt;
   PjSapCfg     *usap=NULLP;
   Pst           pst;

   TRC2(smBuildPjCpjSapCfg)

   SM_SET_ZERO(&pjMngmt, sizeof(PjMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   usap   = &(pjMngmt.t.cfg.s.sap);

   usap->sapId        = 0;
   usap->procId      = SM_KW_PROC;
   usap->priority    = PRIOR0;
   usap->route       = RTESPEC;
   usap->selector    = SM_SELECTOR_LC;
   usap->ent         = ENTPJ;
   usap->inst        = SM_INST_ZERO;
   usap->mem.region  = smCb.init.region;
   usap->mem.pool    = PJ_POOL;
   usap->bndTmrIntvl = TRUE;


   /* Fill Header */
   pjMngmt.hdr.msgType             = TCFG;
   pjMngmt.hdr.msgLen              = 0;
   pjMngmt.hdr.entId.ent           = ENTPJ;
   pjMngmt.hdr.entId.inst          = SM_INST_ZERO;
   pjMngmt.hdr.elmId.elmnt         = STCPJSAP;
   pjMngmt.hdr.seqNmb              = 0;
   pjMngmt.hdr.version             = 0;
   pjMngmt.hdr.transId             = 0;
  
   pjMngmt.hdr.response.prior      = PRIOR0;
   pjMngmt.hdr.response.route      = RTESPEC;
   pjMngmt.hdr.response.mem.region = smCb.init.region;
   pjMngmt.hdr.response.mem.pool   = PJ_POOL;
   pjMngmt.hdr.response.selector   = SM_SELECTOR_LC;
   
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTPJ;
   pst.dstProcId = SM_PJ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "PDCP cpj Sap Cfg Req sent\n"));
#endif

   /* Send the request to the RRC */
   SmMiLpjCfgReq(&pst, &pjMngmt);

   RETVALUE(ROK);
} /* end of smBuildPjCpjSapCfg */

/*
*
*       Fun:   smBuildPjPjuSapCfg
*
*       Desc:  Invoked to build PDCP PJU SAP Config (PJU).
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_sm_pj_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildPjPjuSapCfg
(
SpId spId
)
#else
PUBLIC S16 smBuildPjPjuSapCfg (spId)
SpId spId;
#endif
{
   
   PjMngmt       pjMngmt;
   PjSapCfg     *usap=NULLP;
   Pst           pst;

   TRC2(smBuildPjPjuSapCfg)

   SM_SET_ZERO(&pjMngmt, sizeof(PjMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   usap   = &(pjMngmt.t.cfg.s.sap);

   usap->sapId        = spId;
   usap->procId      = SM_KW_PROC;
   usap->priority    = PRIOR0;
   usap->route       = RTESPEC;
   usap->selector    = SM_SELECTOR_LC;
   if(spId == VE_PJ_SPID)
   {
      usap->ent         = ENTVE;
   }
   else if(spId == NH_PJ_SPID)
   {
      usap->ent         = ENTNH;
   }
   else
   {
#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "smBuildPjPjuSapCfg: Invalid spId\n"));
#endif
   }
   usap->inst        = SM_INST_ZERO;
   usap->mem.region  = smCb.init.region;
   usap->mem.pool    = PJ_POOL;
   usap->bndTmrIntvl = TRUE;

   /* Fill Header */
   pjMngmt.hdr.msgType             = TCFG;
   pjMngmt.hdr.msgLen              = 0;
   pjMngmt.hdr.entId.ent           = ENTPJ;
   pjMngmt.hdr.entId.inst          = SM_INST_ZERO;
   pjMngmt.hdr.elmId.elmnt         = STPJUSAP;
   pjMngmt.hdr.seqNmb              = 0;
   pjMngmt.hdr.version             = 0;
   pjMngmt.hdr.transId             = 0;
  
   pjMngmt.hdr.response.prior      = PRIOR0;
   pjMngmt.hdr.response.route      = RTESPEC;
   pjMngmt.hdr.response.mem.region = smCb.init.region;
   pjMngmt.hdr.response.mem.pool   = PJ_POOL;
   pjMngmt.hdr.response.selector   = SM_SELECTOR_LC;
   
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTPJ;
   pst.dstProcId = SM_PJ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;

#ifdef DEBUGP
  VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "PDCP pju Sap Cfg Req sent\n"));
#endif

   /* Send the request to the RRC */
   SmMiLpjCfgReq(&pst, &pjMngmt);

   RETVALUE(ROK);
} /* end of smBuildPjPjuSapCfg */

/********************************************************************30**
  
         End of file:     ve_sm_pj_cntrl.c@@/main/2 - Wed Dec  1 08:44:35 2010
  
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

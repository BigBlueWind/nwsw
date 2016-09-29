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
  
     Name:     ve_sm_eg_ptmi.c - Layer Management Interface for S1AP
  
     Type:     C source file
  
     Desc:     C code for SmMiLegXxxReq functions
  
      File:    ve_sm_eg_ptmi.c
  
     Sid:      ve_sm_eg_ptmi.c@@/main/2 - Wed Dec  1 08:44:31 2010
  
     Prg:      an
  
*********************************************************************21*/
/*

  ve_sm_eg_ptmi.c - Layer Management Interface for S1AP - SmMiLegXxxReq functions

Following functions are provided in this file:
         prUnanMiLprCfgReq    - Unpack configuration request
*/
/*
 *     this software may be combined with the following TRILLIUM
 *     software:
 *
 *     part no.                      description
 *     --------    ----------------------------------------------
 *     1000XXX                      S1AP v 1.1
 */

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

#include "sct.h"           /* SB (SCTP) layer */
#include "leg.h"           /* layer management defines for EGCP */
#include "egt.h"            /* defines and macros for EG */
#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#include "hi.h"
#endif

#ifdef    SB
#include "lsb.h"
#include "sb_mtu.h"
#include "sb.h"
#endif    /* SB */
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
#include "egt.x"           /* EGT Interface Structures */
#include "leg.x"           /* layer management typedefs for EGCP */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#include "hi.x"
#endif

#ifdef    SB
#include "lsb.x"
#include "sct.x"           /* SB (SCTP) layer */
#include "sb_mtu.x"
#include "sb.x"
#endif    /* SB */

#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif

/*local defines */
#define SM_MAX_EG_SEL 4

PRIVATE U8 veEgCfg = 0;
/*local typedefs */

/*forward references */

PUBLIC S16 PtMiLegCfgReq      ARGS((Pst *post, EgMngmt *cfg));
PUBLIC S16 PtMiLegStsReq      ARGS((Pst *post, Action action, EgMngmt *sts));
PUBLIC S16 PtMiLegStaReq      ARGS((Pst *post, EgMngmt *ssta));
PUBLIC S16 PtMiLegCntrlReq    ARGS((Pst *post, EgMngmt *cntrl));

/* public variable declarations */

/* public variable definitions */

/* private variable definitions */

/* public routines */

/* Following matrices define the mapping between the primitives called by the
 * layer manager (as SmMiLegXXXReq) and the corresponding primitives of the 
 * S1AP layer (layer management service provider) (as EgMiLegXXXReq).
 * Each primitive is mapped to one of SM_MAX_EG_SEL functions in the array.
 * The mapping is based on post->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCSMEGMILEG  loose         cmPkMiLegXXXReq
 * 1                 MG          tight         EgMiLegXXXReq
 * 2+                            tight         PtMiLegXXXReq
 */

PRIVATE LegCfgReq smMiLegCfgReqMt[] =
{
#ifdef LCSMEGMILEG
   cmPkLegCfgReq,
#else
   PtMiLegCfgReq,
#endif

#ifdef EG
   EgMiLegCfgReq,
#else
   PtMiLegCfgReq,
#endif

};


PRIVATE LegStsReq smMiLegStsReqMt[] =
{
#ifdef LCSMEGMILEG
   cmPkLegStsReq,
#else
   PtMiLegStsReq,
#endif

#ifdef EG
   EgMiLegStsReq,
#else
   PtMiLegStsReq,
#endif

};


PRIVATE LegStaReq smMiLegStaReqMt[] =
{
#ifdef LCSMEGMILEG
   cmPkLegStaReq,
#else
   PtMiLegStaReq,
#endif

#ifdef EG
   EgMiLegStaReq,
#else
   PtMiLegStaReq,
#endif

};


PRIVATE LegCntrlReq smMiLegCntrlReqMt[] =
{
#ifdef LCSMEGMILEG
   cmPkLegCntrlReq,
#else
   PtMiLegCntrlReq,
#endif

#ifdef EG
   EgMiLegCntrlReq,
#else
   PtMiLegCntrlReq,
#endif

};


/*
 *
 *       Fun:    SmMiLegCfgReq - Configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_eg_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegCfgReq
(
Pst     *post,       /* post structure */
EgMngmt *cfg        /* management structure */
)
#else
PUBLIC S16 SmMiLegCfgReq(post, cfg)
Pst     *post;       /* post structure */
EgMngmt *cfg;       /* management structure */
#endif
{

   TRC3(SmMiLegCfgReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_EG_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLegCfgReqMt[post->selector]))(post, cfg));
} /* end of SmMiLegCfgReq() */

/*
 *
 *       Fun:    SmMiLegStsReq - Statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_eg_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegStsReq
(
Pst     *post,       /* post structure */
Action action,
EgMngmt *sts        /* management structure */
)
#else
PUBLIC S16 SmMiLegStsReq(post, action, sts)
Pst     *post;       /* post structure */
Action action;
EgMngmt *sts;       /* management structure */
#endif
{
   TRC3(SmMiLegStsReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_EG_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLegStsReqMt[post->selector]))(post, action, sts));
} /* end of SmMiLegStsReq() */

/*
 *
 *       Fun:    SmMiLegStaReq - Status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_eg_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegStaReq
(
Pst     *post,       /* post structure */
EgMngmt *ssta        /* management structure */
)
#else
PUBLIC S16 SmMiLegStaReq(post, ssta)
Pst     *post;       /* post structure */
EgMngmt *ssta;       /* management structure */
#endif
{
   TRC3(SmMiLegStaReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_EG_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLegStaReqMt[post->selector]))(post, ssta));
} /* end of SmMiLegStaReq() */

/*
 *
 *       Fun:    SmMiLegCntrlReq - Control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   eg_sm_eg_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegCntrlReq
(
Pst     *post,       /* post structure */
EgMngmt *cntrl        /* management structure */
)
#else
PUBLIC S16 SmMiLegCntrlReq(post, cntrl)
Pst     *post;       /* post structure */
EgMngmt *cntrl;       /* management structure */
#endif
{
   TRC3(SmMiLegCntrlReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_EG_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLegCntrlReqMt[post->selector]))(post, cntrl));
} /* end of SmMiLegCntrlReq() */

/*
 *
 *       Fun:    PtMiLegCfgReq - portable configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   eg_sm_eg_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLegCfgReq
(
Pst     *post,       /* post structure */
EgMngmt *cfg        /* management structure */
)
#else
PUBLIC S16 PtMiLegCfgReq(post, cfg)
Pst     *post;       /* post structure */
EgMngmt *cfg;       /* management structure */
#endif
{
   TRC3(PtMiLegCfgReq)

   UNUSED(post);
   UNUSED(cfg);
   RETVALUE(RFAILED);
} /* end of PtMiLegCfgReq() */

/*
 *
 *       Fun:    PtMiLegStsReq - portable statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   eg_sm_eg_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLegStsReq
(
Pst     *post,       /* post structure */
Action action,
EgMngmt *sts        /* management structure */
)
#else
PUBLIC S16 PtMiLegStsReq(post, action, sts)
Pst     *post;       /* post structure */
Action action;
EgMngmt *sts;       /* management structure */
#endif
{
   TRC3(PtMiLegStsReq)

   UNUSED(post);
   UNUSED(action);
   UNUSED(sts);
   RETVALUE(RFAILED);
} /* end of PtMiLegStsReq() */

/*
 *
 *       Fun:    PtMiLegStaReq - portable status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_eg_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLegStaReq
(
Pst     *post,       /* post structure */
EgMngmt *ssta        /* management structure */
)
#else
PUBLIC S16 PtMiLegStaReq(post, ssta)
Pst     *post;       /* post structure */
EgMngmt *ssta;       /* management structure */
#endif
{
   TRC3(PtMiLegStaReq)

   UNUSED(post);
   UNUSED(ssta);
   RETVALUE(RFAILED);
} /* end of PtMiLegStaReq() */

/*
 *
 *       Fun:    PtMiLegCntrlReq - portable control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_eg_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLegCntrlReq
(
Pst     *post,       /* post structure */
EgMngmt *cntrl        /* management structure */
)
#else
PUBLIC S16 PtMiLegCntrlReq(post, cntrl)
Pst     *post;       /* post structure */
EgMngmt *cntrl;       /* management structure */
#endif
{
   TRC3(PtMiLegCntrlReq)

   UNUSED(post);
   UNUSED(cntrl);
   RETVALUE(RFAILED);
} /* end of PtMiLegCntrlReq() */

/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_eg_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLegCfgCfm
(
Pst     *pst,          /* post structure */
EgMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLegCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
EgMngmt *cfm;          /* configuration */
#endif
{
   Pst      egPst;

   TRC2(SmMiLegCfgCfm);

   SM_SET_ZERO(&egPst,  sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
       switch (cfm->hdr.elmId.elmnt)
       {
          case STEGGEN:
               veEgCfg |= VE_EG_GEN_CFG;
               break;      
          case STEGUSAP:
               veEgCfg |= VE_EG_EGT_SAP_CFG;
               break;
          case STEGTSAP:
               veEgCfg |= VE_EG_TSAP_CFG;
               break;
          case STEGSRVR:
               veEgCfg |= VE_EG_TSRV_CFG;
               break;
          case STEGIPADDR:
               veEgCfg |= VE_EG_IPADDR_CFG;
               break;
          default:
#ifdef DEBUGP
       VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "EGTP Cfg Cfm received \
for invalid element %d\n",cfm->hdr.elmId.elmnt));
#endif
               break;
       } /* end of switch statement */

#ifdef DEBUGP
       VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "EGTP Cfg Cfm received\
for the element %d\n",cfm->hdr.elmId.elmnt));
#endif

       if (veEgCfg == VE_SM_EG_CONFIGURED)
       {
          smCb.smState = VE_SM_STATE_EGTP_CFG_DONE;

#ifdef LTE_HO_SUPPORT
          VE_FILL_PST(egPst, SM_EG_PROC, ENTSM, ENTVE, EVTVEX2APCFG, VE_POOL, SM_SELECTOR_LC);
#else
          VE_FILL_PST(egPst, SM_EG_PROC, ENTSM, ENTVE, EVTVEAPPCFG, VE_POOL, SM_SELECTOR_LC);
#endif
          if(veSmSelfPstMsg(&egPst) != ROK)
          {
#ifdef DEBUGP
             VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "EGTP CONFIGURATION \
SUCCESSFULLY PERFORMED. Failure at Self Pst.\n"));
#endif
          }
          else
          {
#ifdef DEBUGP
             VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "EGTP CONFIGURATION \
SUCCESSFULLY PERFORMED.\n"));
#endif
          } /* end of else part */

       }  /* end of veEgCfg == VE_SM_EG_CONFIGURED */
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */
   else
   {
#ifdef DEBUGP
             VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "eGTP Configuration \
confirm received with NOK.\n"));
#endif
   }

   RETVALUE(ROK);
} /* end of SmMiLegCfgCfm */

/*
 *
 *       Fun:    SmMiLegCntrlCfm - control confirm
 *
 *       Desc:   prints the control confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_eg_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegCntrlCfm
(
Pst         *pst,
EgMngmt     *cntrl
)
#else
PUBLIC S16 SmMiLegCntrlCfm(pst, cntrl)
Pst         *pst;
EgMngmt     *cntrl;
#endif /* ANSI */
{
   Pst  egPst;

   TRC2(SmMiLegCntrlCfm)

   SM_SET_ZERO(&egPst, sizeof(Pst));

   if (cntrl->cfm.status == LCM_PRIM_OK && cntrl->hdr.elmId.elmnt == STEGTSAP)
   {

#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF EGTP LSAP WITH TUCL \
SUCCESSFULLY PERFORMED.\n"));
#endif
      smBindVeToEgtSap();
   } /* end of if statement cntrl->cfm.status == LCM_PRIM_OK */
   if(cntrl->cfm.status == LCM_PRIM_OK)
   {
      if((smCfgCb.egDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_EG_DBG_MASK))
      {
         /* Update DbgMsk */
         smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | VE_SM_EG_DBG_MASK);
         /* Trigger Debug Cntrl */
         veSmHdlEnbDebugs(EVTVEENBDEBUGS);
      }
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SmMiLegCntrlCfm failed "));
#endif

   }

   
   RETVALUE(ROK);
} /* end of SmMiLegCntrlCfm() */

/*
 *
 *       Fun:    SmMiLegStsCfm - Statistics confirm
 *
 *       Desc:   prints the statistics confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_eg_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegStsCfm
(
Pst         *pst,
EgMngmt     *sts
)
#else
PUBLIC S16 SmMiLegStsCfm(pst, sts)
Pst         *pst;
EgMngmt     *sts;
#endif /* ANSI */
{
   TRC2(SmMiLegStsCfm)

   RETVALUE(ROK);
} /* end of SmMiLegStsCfm() */

/*
 *
 *       Fun:    SmMiLegStaCfm - status confirm
 *
 *       Desc:   prints the status confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_eg_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegStaCfm
(
Pst         *pst,
EgMngmt     *sta
)
#else
PUBLIC S16 SmMiLegStaCfm(pst, sta)
Pst         *pst;
EgMngmt     *sta;
#endif /* ANSI */
{
   TRC2(SmMiLegStaCfm)

   RETVALUE(ROK);
} /* end of SmMiLegStaCfm() */

/*
 *
 *       Fun:    SmMiLegStaInd - status indication
 *
 *       Desc:   prints the status indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_eg_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegStaInd
(
Pst         *pst,
EgMngmt     *ind
)
#else
PUBLIC S16 SmMiLegStaInd(pst, ind)
Pst         *pst;
EgMngmt     *ind;
#endif /* ANSI */
{
   TRC2(SmMiLegStaInd)

   RETVALUE(ROK);
} /* end of SmMiLegStaInd() */

/*
 *
 *       Fun:    SmMiLegTrcInd - Trace indication
 *
 *       Desc:   prints the trace indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_eg_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegTrcInd
(
Pst         *pst,
EgMngmt     *ind,
Buffer      *mBuf
)
#else
PUBLIC S16 SmMiLegTrcInd(pst, ind, mBuf)
Pst         *pst;
EgMngmt     *ind;
Buffer      *mBuf;
#endif /* ANSI */
{
   TRC2(SmMiLegTrcInd)

   RETVALUE(ROK);
} /* end of SmMiLegTrcInd() */




/********************************************************************30**
  
         End of file:     ve_sm_eg_ptmi.c@@/main/2 - Wed Dec  1 08:44:31 2010
  
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

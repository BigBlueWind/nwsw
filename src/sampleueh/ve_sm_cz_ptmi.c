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
  
     Name:     ve_sm_cz_ptmi.c - Layer Management Interface for X2AP
  
     Type:     C source file
  
     Desc:     C code for SmMiLczXxxReq functions
  
      File:    ve_sm_cz_ptmi.c
  
     Sid:      ve_sm_cz_ptmi.c@@/main/eNB_1.3_With_LatestGpr3/2 - Thu Jun 23 17:04:47 2011
  
     Prg:      
  
*********************************************************************21*/
/*

  ve_sm_cz_ptmi.c - Layer Management Interface for X2AP - SmMiLczXxxReq functions

*/
/*
 *     this software may be combined with the following TRILLIUM
 *     software:
 *
 *     part no.                      description
 *     --------    ----------------------------------------------
 *    
 */

#ifdef LTE_HO_SUPPORT

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
#include "lcz.h"           /* layer management defines for CZCP */
#include "czt.h"            /* defines and macros for CZ */
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
#include "czt.x"           /* CZT Interface Structures */
#include "lcz.x"           /* layer management typedefs for CZCP */

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


/*local defines */
#define SM_MAX_CZ_SEL 4

PRIVATE U8 veCzCfg = 0;
/*local typedefs */

/*forward references */
PUBLIC S16 PtMiLczCfgReq      ARGS((Pst *post, CzMngmt *cfg));
PUBLIC S16 PtMiLczStsReq      ARGS((Pst *post, Action action, CzMngmt *sts));
PUBLIC S16 PtMiLczStaReq      ARGS((Pst *post, CzMngmt *ssta));
PUBLIC S16 PtMiLczCntrlReq    ARGS((Pst *post, CzMngmt *cntrl));

/* public variable declarations */

/* public variable definitions */

/* private variable definitions */

/* public routines */

/* Following matrices define the mapping between the primitives called by the
 * layer manager (as SmMiLczXXXReq) and the corresponding primitives of the 
 * X2AP layer (layer management service provider) (as CzMiLczXXXReq).
 * Each primitive is mapped to one of SM_MAX_CZ_SEL functions in the array.
 * The mapping is based on post->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCSMCZMILCZ  loose         cmPkMiLczXXXReq
 * 1                 MG          tight         CzMiLczXXXReq
 * 2+                            tight         PtMiLczXXXReq
 */

PRIVATE LczCfgReq smMiLczCfgReqMt[] =
{
#ifdef LCSMCZMILCZ
   cmPkLczCfgReq,
#else
   PtMiLczCfgReq,
#endif

#ifdef CZ
   CzMiLczCfgReq,
#else
   PtMiLczCfgReq,
#endif

};


PRIVATE LczStsReq smMiLczStsReqMt[] =
{
#ifdef LCSMCZMILCZ
   cmPkLczStsReq,
#else
   PtMiLczStsReq,
#endif

#ifdef CZ
   CzMiLczStsReq,
#else
   PtMiLczStsReq,
#endif

};


PRIVATE LczStaReq smMiLczStaReqMt[] =
{
#ifdef LCSMCZMILCZ
   cmPkLczStaReq,
#else
   PtMiLczStaReq,
#endif

#ifdef CZ
   CzMiLczStaReq,
#else
   PtMiLczStaReq,
#endif

};


PRIVATE LczCntrlReq smMiLczCntrlReqMt[] =
{
#ifdef LCSMCZMILCZ
   cmPkLczCntrlReq,
#else
   PtMiLczCntrlReq,
#endif

#ifdef CZ
   CzMiLczCntrlReq,
#else
   PtMiLczCntrlReq,
#endif

};


/*
 *
 *       Fun:    SmMiLczCfgReq - Configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczCfgReq
(
Pst     *post,       /* post structure */
CzMngmt *cfg        /* management structure */
)
#else
PUBLIC S16 SmMiLczCfgReq(post, cfg)
Pst     *post;       /* post structure */
CzMngmt *cfg;       /* management structure */
#endif
{

   TRC3(SmMiLczCfgReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_CZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLczCfgReqMt[post->selector]))(post, cfg));
} /* end of SmMiLczCfgReq() */

/*
 *
 *       Fun:    SmMiLczStsReq - Statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczStsReq
(
Pst     *post,       /* post structure */
Action action,
CzMngmt *sts        /* management structure */
)
#else
PUBLIC S16 SmMiLczStsReq(post, action, sts)
Pst     *post;       /* post structure */
Action action;
CzMngmt *sts;       /* management structure */
#endif
{
   TRC3(SmMiLczStsReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_CZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLczStsReqMt[post->selector]))(post, action, sts));
} /* end of SmMiLczStsReq() */

/*
 *
 *       Fun:    SmMiLczStaReq - Status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczStaReq
(
Pst     *post,       /* post structure */
CzMngmt *ssta        /* management structure */
)
#else
PUBLIC S16 SmMiLczStaReq(post, ssta)
Pst     *post;       /* post structure */
CzMngmt *ssta;       /* management structure */
#endif
{
   TRC3(SmMiLczStaReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_CZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLczStaReqMt[post->selector]))(post, ssta));
} /* end of SmMiLczStaReq() */

/*
 *
 *       Fun:    SmMiLczCntrlReq - Control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczCntrlReq
(
Pst     *post,       /* post structure */
CzMngmt *cntrl        /* management structure */
)
#else
PUBLIC S16 SmMiLczCntrlReq(post, cntrl)
Pst     *post;       /* post structure */
CzMngmt *cntrl;       /* management structure */
#endif
{
   TRC3(SmMiLczCntrlReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_CZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLczCntrlReqMt[post->selector]))(post, cntrl));
} /* end of SmMiLczCntrlReq() */

/*
 *
 *       Fun:    PtMiLczCfgReq - portable configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLczCfgReq
(
Pst     *post,       /* post structure */
CzMngmt *cfg        /* management structure */
)
#else
PUBLIC S16 PtMiLczCfgReq(post, cfg)
Pst     *post;       /* post structure */
CzMngmt *cfg;       /* management structure */
#endif
{
   TRC3(PtMiLczCfgReq)

   UNUSED(post);
   UNUSED(cfg);
   RETVALUE(RFAILED);
} /* end of PtMiLczCfgReq() */

/*
 *
 *       Fun:    PtMiLczStsReq - portable statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLczStsReq
(
Pst     *post,       /* post structure */
Action action,
CzMngmt *sts        /* management structure */
)
#else
PUBLIC S16 PtMiLczStsReq(post, action, sts)
Pst     *post;       /* post structure */
Action action;
CzMngmt *sts;       /* management structure */
#endif
{
   TRC3(PtMiLczStsReq)

   UNUSED(post);
   UNUSED(action);
   UNUSED(sts);
   RETVALUE(RFAILED);
} /* end of PtMiLczStsReq() */

/*
 *
 *       Fun:    PtMiLczStaReq - portable status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLczStaReq
(
Pst     *post,       /* post structure */
CzMngmt *ssta        /* management structure */
)
#else
PUBLIC S16 PtMiLczStaReq(post, ssta)
Pst     *post;       /* post structure */
CzMngmt *ssta;       /* management structure */
#endif
{
   TRC3(PtMiLczStaReq)

   UNUSED(post);
   UNUSED(ssta);
   RETVALUE(RFAILED);
} /* end of PtMiLczStaReq() */

/*
 *
 *       Fun:    PtMiLczCntrlReq - portable control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLczCntrlReq
(
Pst     *post,       /* post structure */
CzMngmt *cntrl        /* management structure */
)
#else
PUBLIC S16 PtMiLczCntrlReq(post, cntrl)
Pst     *post;       /* post structure */
CzMngmt *cntrl;       /* management structure */
#endif
{
   TRC3(PtMiLczCntrlReq)

   UNUSED(post);
   UNUSED(cntrl);
   RETVALUE(RFAILED);
} /* end of PtMiLczCntrlReq() */


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
*       File:  ve_sm_cz_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLczCfgCfm
(
Pst     *pst,          /* post structure */
CzMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLczCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
CzMngmt *cfm;          /* configuration */
#endif
{
   Pst      czPst;

   TRC2(SmMiLczCfgCfm);

   SM_SET_ZERO(&czPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
       switch (cfm->hdr.elmId.elmnt)
       {
          case STCZGEN:
               veCzCfg |= VE_CZ_GEN_CFG;
               break;      
          case STCZPROT:
               veCzCfg |= VE_CZ_PROT_CFG;
               break;
          case STCZCZTSAP:
               veCzCfg |= VE_CZ_CZT_SAP_CFG;
               break;
          case STCZSCTSAP:
               veCzCfg |= VE_CZ_SCT_SAP_CFG;
               break;
          case STCZPEER:
               veCzCfg |= VE_CZ_PEER_CFG;
               break;
          default:
               break;
       } /* end of switch statement */
#ifdef DEBUGP
       VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "X2AP Cfg Cfm received\
for the element %d\n",cfm->hdr.elmId.elmnt));
#endif

       /* Since for the first eNB that is getting deployed, there may not be
          any Peers. Hence there is no Peer config present. So to avoid 
          failure, we are checking for another condition of if CZ Configured
          without peers. */
       if (((veCzCfg == VE_SM_CZ_CONFGRD_WO_PEER) && (VE_VAL_ZERO == smCfgCb.noOfNghCfg)) 
          || veCzCfg == VE_SM_CZ_CONFIGURED)
       {
          smCb.smState = VE_SM_STATE_X2AP_CFG_DONE;
          VE_FILL_PST(czPst, SM_CZ_PROC, ENTSM, ENTVE, EVTVEAPPCFG, VE_POOL, SM_SELECTOR_LC);

          if(veSmSelfPstMsg(&czPst) != ROK)
          {
#ifdef DEBUGP
             VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "X2AP CONFIGURATION \
SUCCESSFULLY PERFORMED. Failure at Self Pst.\n"));
#endif
          }
          else
          {
#ifdef DEBUGP
             VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "X2AP CONFIGURATION \
SUCCESSFULLY PERFORMED.\n"));
#endif
          } /* end of else part */

       }  /* end of veCzCfg == VE_SM_CZ_CONFIGURED */
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */
   else
   {
#ifdef DEBUGP
             VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Configuration confirm\
not OK.\n"));
#endif
   }
   RETVALUE(ROK);
} /* end of SmMiLczCfgCfm */

/*
 *
 *       Fun:    SmMiLczCntrlCfm - control confirm
 *
 *       Desc:   prints the control confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczCntrlCfm
(
Pst         *pst,
CzMngmt     *cntrl
)
#else
PUBLIC S16 SmMiLczCntrlCfm(pst, cntrl)
Pst         *pst;
CzMngmt     *cntrl;
#endif /* ANSI */
{
   Pst  czPst;

   TRC2(SmMiLczCntrlCfm);

   SM_SET_ZERO(&czPst, sizeof(Pst));

   if (cntrl->cfm.status == LCM_PRIM_OK)
   {
      if(cntrl->hdr.elmId.elmnt == STCZGEN)
      {
         if((smCfgCb.czDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_CZ_DBG_MASK))
         {
            /* Update DbgMsk */
            smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | VE_SM_CZ_DBG_MASK);
            /* Trigger Debug Cntrl */
            veSmHdlEnbDebugs(EVTVEENBDEBUGS);
         }
      }

   } /* end of if statement */
   /* Initiate control request for binding App to X2AP */
   if(cntrl->hdr.elmId.elmnt == STCZSCTSAP)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Bind of X2AP LSap with SCTP \
               is in progress \n"));
#endif
   } /* end of if statement */

   RETVALUE(ROK);
} /* end of SmMiLczCntrlCfm() */

/*
 *
 *       Fun:    SmMiLczStaInd - status indication
 *
 *       Desc:   Received the Status Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczStaInd
(
Pst         *pst,
CzMngmt      *staInd
)
#else
PUBLIC S16 SmMiLczStaInd(pst, staInd)
Pst         *pst;
CzMngmt      *staInd;
#endif /* ANSI */
{
   Pst  czPst;
   CzUsta  *usta= NULLP;
   CzUstaDgn *dgn;

   TRC2(SmMiLczStaInd)

   SM_SET_ZERO(&czPst, sizeof(Pst));
   usta = &(staInd->t.usta);
   dgn = &(usta->dgn);

   if (usta->alarm.event == LCZ_EVENT_LI_ASSOC_CFM  
      && usta->alarm.cause == LCZ_CAUSE_ASSOC_UP) 
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "X2AP ASSOCIATION\
               SUCCESSFULLY ESTABLISHED. \n"));
#endif

   } /* end of if statement */
   else if (usta->alarm.category == LCM_CATEGORY_INTERFACE)
   {
      if (usta->alarm.event == LCM_EVENT_BND_OK)
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF X2AP lsap id[%d] IS SUCCESSFUL\n", dgn->u.sapId));
#endif
        smBindVeToCztSap(); 
      }
      else if (usta->alarm.event == LCM_EVENT_BND_FAIL) 
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF X2AP lsap id[%d] FAILED\n", dgn->u.sapId));
#endif
      }
      else if (usta->alarm.event == LCZ_EVENT_LI_TERM_IND)
         {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Term Indication Recvd. lsap id[%d] FAILED\n", dgn->u.sapId));
#endif
         }
   } /* end of LCM_CATEGORY_INTERFACE */

   else if (usta->alarm.category == LCM_CATEGORY_PROTOCOL)
   {
           if (usta->alarm.event == LCZ_EVENT_LI_TERM_IND)
           {
#ifdef DEBUGP
                   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Term Indication Recvd. lsap id[%d] FAILED\n", dgn->u.sapId));
#endif
           }

   }
   RETVALUE(ROK);
} /* end of SmMiLczStaInd() */

/*
 *
 *       Fun:    SmMiLczTrcInd - trace indication
 *
 *       Desc:   Received the Trace Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczTrcInd
(
Pst         *pst,
CzMngmt      *trcInd,
Buffer      *mBuf
)
#else
PUBLIC S16 SmMiLczTrcInd(pst, trcInd, mBuf)
Pst         *pst;
CzMngmt     *trcInd;
Buffer      *mBuf;
#endif /* ANSI */
{
   TRC2(SmMiLczTrcInd);

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Received X2AP TrcInd \n"));
#endif

   RETVALUE(ROK);
} /* end of SmMiLczStaInd() */

/*
 *
 *       Fun:    SmMiLczStaCfm - status confirm
 *
 *       Desc:   Received the Status Confirm
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczStaCfm
(
Pst         *pst,
CzMngmt      *staCfm
)
#else
PUBLIC S16 SmMiLczStaCfm(pst, staCfm)
Pst         *pst;
CzMngmt      *staCfm;
#endif /* ANSI */
{
   TRC2(SmMiLczStaCfm)

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Received X2AP StaCfm\n"));
#endif

   RETVALUE(ROK);
} /* end of SmMiLczStaCfm() */

/*
 *
 *       Fun:    SmMiLczStsCfm - statistics confirm
 *
 *       Desc:   Received the Statistics Confirm
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczStsCfm
(
Pst         *pst,
CzMngmt      *stsCfm
)
#else
PUBLIC S16 SmMiLczStsCfm(pst, stsCfm)
Pst         *pst;
CzMngmt      *stsCfm;
#endif /* ANSI */
{
   TRC2(SmMiLczStsCfm);

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Received X2AP StsCfm\n"));
#endif

   RETVALUE(ROK);
} /* end of SmMiLczStsCfm() */

#endif


/********************************************************************30**
  
         End of file:     ve_sm_cz_ptmi.c@@/main/eNB_1.3_With_LatestGpr3/2 - Thu Jun 23 17:04:47 2011
  
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

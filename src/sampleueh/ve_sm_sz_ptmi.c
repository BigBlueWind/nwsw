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
  
     Name:     ve_sm_sz_ptmi.c - Layer Management Interface for S1AP
  
     Type:     C source file
  
     Desc:     C code for SmMiLszXxxReq functions
  
      File:    ve_sm_sz_ptmi.c
  
     Sid:      ve_sm_sz_ptmi.c@@/main/2 - Wed Dec  1 08:44:39 2010
  
     Prg:      an
  
*********************************************************************21*/
/*

  ve_sm_sz_ptmi.c - Layer Management Interface for S1AP - SmMiLszXxxReq functions

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
#include "lsz.h"           /* layer management defines for SZCP */
#include "szt.h"            /* defines and macros for SZ */
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
#include "szt.x"           /* SZT Interface Structures */
#include "lsz.x"           /* layer management typedefs for SZCP */

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
#define SM_MAX_SZ_SEL 4

PRIVATE U8 veSzCfg = 0;
/*local typedefs */

/*forward references */
PUBLIC S16 PtMiLszCfgReq      ARGS((Pst *post, SzMngmt *cfg));
PUBLIC S16 PtMiLszStsReq      ARGS((Pst *post, Action action, SzMngmt *sts));
PUBLIC S16 PtMiLszStaReq      ARGS((Pst *post, SzMngmt *ssta));
PUBLIC S16 PtMiLszCntrlReq    ARGS((Pst *post, SzMngmt *cntrl));

/* public variable declarations */

/* public variable definitions */

/* private variable definitions */

/* public routines */

/* Following matrices define the mapping between the primitives called by the
 * layer manager (as SmMiLszXXXReq) and the corresponding primitives of the 
 * S1AP layer (layer management service provider) (as SzMiLszXXXReq).
 * Each primitive is mapped to one of SM_MAX_SZ_SEL functions in the array.
 * The mapping is based on post->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCSMSZMILSZ  loose         cmPkMiLszXXXReq
 * 1                 MG          tight         SzMiLszXXXReq
 * 2+                            tight         PtMiLszXXXReq
 */

PRIVATE LszCfgReq smMiLszCfgReqMt[] =
{
#ifdef LCSMSZMILSZ
   cmPkLszCfgReq,
#else
   PtMiLszCfgReq,
#endif

#ifdef SZ
   SzMiLszCfgReq,
#else
   PtMiLszCfgReq,
#endif

};


PRIVATE LszStsReq smMiLszStsReqMt[] =
{
#ifdef LCSMSZMILSZ
   cmPkLszStsReq,
#else
   PtMiLszStsReq,
#endif

#ifdef SZ
   SzMiLszStsReq,
#else
   PtMiLszStsReq,
#endif

};


PRIVATE LszStaReq smMiLszStaReqMt[] =
{
#ifdef LCSMSZMILSZ
   cmPkLszStaReq,
#else
   PtMiLszStaReq,
#endif

#ifdef SZ
   SzMiLszStaReq,
#else
   PtMiLszStaReq,
#endif

};


PRIVATE LszCntrlReq smMiLszCntrlReqMt[] =
{
#ifdef LCSMSZMILSZ
   cmPkLszCntrlReq,
#else
   PtMiLszCntrlReq,
#endif

#ifdef SZ
   SzMiLszCntrlReq,
#else
   PtMiLszCntrlReq,
#endif

};


/*
 *
 *       Fun:    SmMiLszCfgReq - Configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszCfgReq
(
Pst     *post,       /* post structure */
SzMngmt *cfg        /* management structure */
)
#else
PUBLIC S16 SmMiLszCfgReq(post, cfg)
Pst     *post;       /* post structure */
SzMngmt *cfg;       /* management structure */
#endif
{

   TRC3(SmMiLszCfgReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLszCfgReqMt[post->selector]))(post, cfg));
} /* end of SmMiLszCfgReq() */

/*
 *
 *       Fun:    SmMiLszStsReq - Statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszStsReq
(
Pst     *post,       /* post structure */
Action action,
SzMngmt *sts        /* management structure */
)
#else
PUBLIC S16 SmMiLszStsReq(post, action, sts)
Pst     *post;       /* post structure */
Action action;
SzMngmt *sts;       /* management structure */
#endif
{
   TRC3(SmMiLszStsReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLszStsReqMt[post->selector]))(post, action, sts));
} /* end of SmMiLszStsReq() */

/*
 *
 *       Fun:    SmMiLszStaReq - Status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszStaReq
(
Pst     *post,       /* post structure */
SzMngmt *ssta        /* management structure */
)
#else
PUBLIC S16 SmMiLszStaReq(post, ssta)
Pst     *post;       /* post structure */
SzMngmt *ssta;       /* management structure */
#endif
{
   TRC3(SmMiLszStaReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLszStaReqMt[post->selector]))(post, ssta));
} /* end of SmMiLszStaReq() */

/*
 *
 *       Fun:    SmMiLszCntrlReq - Control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszCntrlReq
(
Pst     *post,       /* post structure */
SzMngmt *cntrl        /* management structure */
)
#else
PUBLIC S16 SmMiLszCntrlReq(post, cntrl)
Pst     *post;       /* post structure */
SzMngmt *cntrl;       /* management structure */
#endif
{
   TRC3(SmMiLszCntrlReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLszCntrlReqMt[post->selector]))(post, cntrl));
} /* end of SmMiLszCntrlReq() */

/*
 *
 *       Fun:    PtMiLszCfgReq - portable configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLszCfgReq
(
Pst     *post,       /* post structure */
SzMngmt *cfg        /* management structure */
)
#else
PUBLIC S16 PtMiLszCfgReq(post, cfg)
Pst     *post;       /* post structure */
SzMngmt *cfg;       /* management structure */
#endif
{
   TRC3(PtMiLszCfgReq)

   UNUSED(post);
   UNUSED(cfg);
   RETVALUE(RFAILED);
} /* end of PtMiLszCfgReq() */

/*
 *
 *       Fun:    PtMiLszStsReq - portable statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLszStsReq
(
Pst     *post,       /* post structure */
Action action,
SzMngmt *sts        /* management structure */
)
#else
PUBLIC S16 PtMiLszStsReq(post, action, sts)
Pst     *post;       /* post structure */
Action action;
SzMngmt *sts;       /* management structure */
#endif
{
   TRC3(PtMiLszStsReq)

   UNUSED(post);
   UNUSED(action);
   UNUSED(sts);
   RETVALUE(RFAILED);
} /* end of PtMiLszStsReq() */

/*
 *
 *       Fun:    PtMiLszStaReq - portable status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLszStaReq
(
Pst     *post,       /* post structure */
SzMngmt *ssta        /* management structure */
)
#else
PUBLIC S16 PtMiLszStaReq(post, ssta)
Pst     *post;       /* post structure */
SzMngmt *ssta;       /* management structure */
#endif
{
   TRC3(PtMiLszStaReq)

   UNUSED(post);
   UNUSED(ssta);
   RETVALUE(RFAILED);
} /* end of PtMiLszStaReq() */

/*
 *
 *       Fun:    PtMiLszCntrlReq - portable control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ve_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLszCntrlReq
(
Pst     *post,       /* post structure */
SzMngmt *cntrl        /* management structure */
)
#else
PUBLIC S16 PtMiLszCntrlReq(post, cntrl)
Pst     *post;       /* post structure */
SzMngmt *cntrl;       /* management structure */
#endif
{
   TRC3(PtMiLszCntrlReq)

   UNUSED(post);
   UNUSED(cntrl);
   RETVALUE(RFAILED);
} /* end of PtMiLszCntrlReq() */

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
*       File:  ve_sm_sz_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLszCfgCfm
(
Pst     *pst,          /* post structure */
SzMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLszCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
SzMngmt *cfm;          /* configuration */
#endif
{
   Pst      szPst;

   TRC2(SmMiLszCfgCfm);

   SM_SET_ZERO(&szPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
       switch (cfm->hdr.elmId.elmnt)
       {
          case STSZGEN:
               veSzCfg |= VE_SZ_GEN_CFG;
               break;      
          case STSZPROT:
               veSzCfg |= VE_SZ_PROT_CFG;
               break;
          case STSZSZTSAP:
               veSzCfg |= VE_SZ_SZT_SAP_CFG;
               break;
          case STSZSCTSAP:
               veSzCfg |= VE_SZ_SCT_SAP_CFG;
               break;
          case STSZPEER:
               veSzCfg |= VE_SZ_PEER_CFG;
               break;
          default:
               break;
       } /* end of switch statement */
#ifdef DEBUGP
       VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP Cfg Cfm received\
for the element %d\n",cfm->hdr.elmId.elmnt));
#endif

       if (veSzCfg == VE_SM_SZ_CONFIGURED)
       {
          smCb.smState = VE_SM_STATE_S1AP_CFG_DONE;
          VE_FILL_PST(szPst, SM_SZ_PROC, ENTSM, ENTVE, EVTVEEGTPCFG, VE_POOL, SM_SELECTOR_LC);

          if(veSmSelfPstMsg(&szPst) != ROK)
          {
#ifdef DEBUGP
             VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP CONFIGURATION \
SUCCESSFULLY PERFORMED. Failure at Self Pst.\n"));
#endif
          }
          else
          {
#ifdef DEBUGP
             VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP CONFIGURATION \
SUCCESSFULLY PERFORMED.\n"));
#endif
          } /* end of else part */

       }  /* end of veSzCfg == VE_SM_SZ_CONFIGURED */
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */
   else
   {
#ifdef DEBUGP
             VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Configuration confirm\
not OK.\n"));
#endif
   }

   RETVALUE(ROK);
} /* end of SmMiLszCfgCfm */

/*
 *
 *       Fun:    SmMiLszCntrlCfm - control confirm
 *
 *       Desc:   prints the control confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszCntrlCfm
(
Pst         *pst,
SzMngmt     *cntrl
)
#else
PUBLIC S16 SmMiLszCntrlCfm(pst, cntrl)
Pst         *pst;
SzMngmt     *cntrl;
#endif /* ANSI */
{
   Pst  szPst;

   TRC2(SmMiLszCntrlCfm);

   SM_SET_ZERO(&szPst, sizeof(Pst));

   if (cntrl->cfm.status == LCM_PRIM_OK)
   {
      if(cntrl->hdr.elmId.elmnt == STSZGEN)
      {
         if((smCfgCb.szDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_SZ_DBG_MASK))
         {
            /* Update DbgMsk */
            smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | VE_SM_SZ_DBG_MASK);
            /* Trigger Debug Cntrl */
            veSmHdlEnbDebugs(EVTVEENBDEBUGS);
         }
      }

   } /* end of if statement */
   /* Initiate control request for binding App to S1AP */
   if(cntrl->hdr.elmId.elmnt == STSZSCTSAP)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Bind of S1AP LSap with SCTP \
               is in progress \n"));
#endif
   } /* end of if statement */

   RETVALUE(ROK);
} /* end of SmMiLszCntrlCfm() */

/*
 *
 *       Fun:    SmMiLszStaInd - status indication
 *
 *       Desc:   Received the Status Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszStaInd
(
Pst         *pst,
SzMngmt      *staInd
)
#else
PUBLIC S16 SmMiLszStaInd(pst, staInd)
Pst         *pst;
SzMngmt      *staInd;
#endif /* ANSI */
{
   Pst  szPst;
   SzUsta  *usta= NULLP;
   SzUstaDgn *dgn;

   TRC2(SmMiLszStaInd)

   SM_SET_ZERO(&szPst, sizeof(Pst));
   usta = &(staInd->u.usta);
   dgn = &(usta->dgn);

   if (usta->alarm.event == LSZ_EVENT_LI_ASSOC_CFM  
      && usta->alarm.cause == LSZ_CAUSE_ASSOC_UP) 
   {
      smCb.smState = VE_SM_STATE_AWAIT_CELL_UP;
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP ASSOCIATION\
               SUCCESSFULLY ESTABLISHED. \n"));
#endif

   } /* end of if statement */
   else if (usta->alarm.category == LCM_CATEGORY_INTERFACE)
   {
      if (usta->alarm.event == LCM_EVENT_BND_OK)
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF S1AP lsap id[%d] IS SUCCESSFUL\n", dgn->u.sapId));
#endif
         smBindVeToSztSap(); 
      }
      else if (usta->alarm.event == LCM_EVENT_BND_FAIL) 
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF S1AP lsap id[%d] FAILED\n", dgn->u.sapId));
#endif
      }
      else if (usta->alarm.event == LSZ_EVENT_LI_TERM_IND)
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Term Indication Recvd. lsap id[%d] FAILED\n", dgn->u.sapId));
#endif
         /* Updating Stack Manager's state */
         smCb.smState = VE_SM_STATE_AWAIT_S1_CON;
         }
   } /* end of LCM_CATEGORY_INTERFACE */

 

   RETVALUE(ROK);
} /* end of SmMiLszStaInd() */

/*
 *
 *       Fun:    SmMiLszTrcInd - trace indication
 *
 *       Desc:   Received the Trace Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszTrcInd
(
Pst         *pst,
SzMngmt      *trcInd,
Buffer      *mBuf
)
#else
PUBLIC S16 SmMiLszTrcInd(pst, trcInd, mBuf)
Pst         *pst;
SzMngmt     *trcInd;
Buffer      *mBuf;
#endif /* ANSI */
{
   TRC2(SmMiLszTrcInd);

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Received S1AP TrcInd \n"));
#endif

   RETVALUE(ROK);
} /* end of SmMiLszStaInd() */

/*
 *
 *       Fun:    SmMiLszStaCfm - status confirm
 *
 *       Desc:   Received the Status Confirm
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszStaCfm
(
Pst         *pst,
SzMngmt      *staCfm
)
#else
PUBLIC S16 SmMiLszStaCfm(pst, staCfm)
Pst         *pst;
SzMngmt      *staCfm;
#endif /* ANSI */
{
   TRC2(SmMiLszStaCfm)

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Received S1AP StaCfm\n"));
#endif

   RETVALUE(ROK);
} /* end of SmMiLszStaCfm() */

/*
 *
 *       Fun:    SmMiLszStsCfm - statistics confirm
 *
 *       Desc:   Received the Statistics Confirm
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszStsCfm
(
Pst         *pst,
SzMngmt      *stsCfm
)
#else
PUBLIC S16 SmMiLszStsCfm(pst, stsCfm)
Pst         *pst;
SzMngmt      *stsCfm;
#endif /* ANSI */
{
   TRC2(SmMiLszStsCfm);

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Received S1AP StsCfm\n"));
#endif

   RETVALUE(ROK);
} /* end of SmMiLszStsCfm() */




/********************************************************************30**
  
         End of file:     ve_sm_sz_ptmi.c@@/main/2 - Wed Dec  1 08:44:39 2010
  
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

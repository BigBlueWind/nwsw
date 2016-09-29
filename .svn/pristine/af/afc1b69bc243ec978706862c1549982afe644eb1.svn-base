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

     Name:      SCTP layer portable management interface

     Type:     C source file

     Desc:     C source code for the SCTP layer layer management
               service user primitives used in loosely coupled
               anstems.

     File:     ve_sm_sb_ptmi.c

     Sid:      ve_sm_sb_ptmi.c@@/main/2 - Wed Dec  1 08:44:38 2010

     Prg:      an

*********************************************************************21*/

/*

  smsbptmi.c -

Following functions are provided in this file:
*/

/*
 *     this software may be combined with the following TRILLIUM
 *     software:
 *
 *     part no.                      description
 *     --------    ----------------------------------------------
 *     1000163                      SCTP layer
 */

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general */
#include "ssi.h"           /* anstem services */
#include "cm_hash.h"       /* common hashing */
#include "cm_llist.h"      /* common linked list */
#include "cm5.h"           /* common timer */
#include "cm_inet.h"       /* common network address */
#include "cm_tpt.h"        /* common transport defines */
#include "cm_dns.h"        /* common dns defines */
#include "lsb.h"           /* layer management, SCTP  */
#include "sct.h"           /* SCT interface */
#include "hit.h"           /* HIT interface */

#include "ve_sm_init.h"
#include "ve.h"

/* header/extern include files (.x) */

#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_hash.x"       /* common hashing */
#include "cm_llist.x"      /* common linked list */
#include "cm_lib.x"        /* common library */
#include "cm5.x"           /* common timer */
#include "cm_inet.x"       /* common network address */
#include "cm_tpt.x"        /* common transport address */
#include "cm_dns.x"        /* common dns address */
#include "sct.x"           /* SCT interface */
#include "lsb.x"           /* layer management SCTP */
#include "hit.x"           /* HIT interface */

#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif

/* local defines */

#define MAXSBMI                2

#ifndef LCSMSBMILSB
#define PTSMSBMILSB
#else
#ifndef SB
#define PTSMSBMILSB
#else
#endif
#endif

PRIVATE U8    veSbCfg = 0;
/* local typedefs */

/* local externs */

/* forward references */
#ifdef PTSMSBMILSB
PRIVATE S16 PtMiLsbCfgReq   ARGS((Pst *pst, SbMgmt *cfg ));
PRIVATE S16 PtMiLsbStaReq   ARGS((Pst *pst, SbMgmt *sta ));
PRIVATE S16 PtMiLsbStsReq   ARGS((Pst *pst, Action action, SbMgmt *sts ));
PRIVATE S16 PtMiLsbCntrlReq ARGS((Pst *pst, SbMgmt *cntrl ));
#endif

/* functions in other modules */

/* public variable declarations */

/* private variable declarations */


/*
The following matrices define the mapping between the primitives
called by the layer management interface of SCTP layer and the corresponding
primitives in the driver.

The parameter MAXSBMI defines the maximum number of layer manager entities
on top of the driver. There is an array of functions per primitive insbked by
the driver. Every array is MAXSBMI long (i.e. there are as many functions as the
number of service users).

The dispatching is performed by the configurable variable: selector.
The selector is configured during general configuration.

The selectors are:

   0 - loosely coupled (#define LCSMSBMILSB)
   1 - Lsb (#define SB)

*/

/* Configuration request primitive */

PRIVATE LsbCfgReq SmMiLsbCfgReqMt[MAXSBMI] =
{
#ifdef LCSMSBMILSB
   cmPkLsbCfgReq,        /* 0 - loosely coupled - bc */
#else
   PtMiLsbCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbMiLsbCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLsbCfgReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Statistics request primitive */

PRIVATE LsbStsReq SmMiLsbStsReqMt[MAXSBMI] =
{
#ifdef LCSMSBMILSB
   cmPkLsbStsReq,        /* 0 - loosely coupled - bc */
#else
   PtMiLsbStsReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbMiLsbStsReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLsbStsReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Status request primitive */

PRIVATE LsbStaReq SmMiLsbStaReqMt[MAXSBMI] =
{
#ifdef LCSMSBMILSB
   cmPkLsbStaReq,        /* 0 - loosely coupled - bc */
#else
   PtMiLsbStaReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbMiLsbStaReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLsbStaReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Control request primitive */

PRIVATE LsbCntrlReq SmMiLsbCntrlReqMt[MAXSBMI] =
{
#ifdef LCSMSBMILSB
   cmPkLsbCntrlReq,        /* 0 - loosely coupled - bc */
#else
   PtMiLsbCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbMiLsbCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLsbCntrlReq,          /* 1 - tightly coupled, portable */
#endif
};

/*
*     layer management interface functions
*/

/*
*
*       Fun:   Configuration request
*
*       Desc:  This function is used to configure SCTP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_sb_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLsbCfgReq
(
Pst *pst,                 /* post structure */
SbMgmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLsbCfgReq(pst, cfg)
Pst *pst;                 /* post structure */
SbMgmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLsbCfgReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLsbCfgReqMt[pst->selector])(pst, cfg);
   RETVALUE(ROK);
} /* end of SmMiLsbCfgReq */

/*
*
*       Fun:   Status request
*
*       Desc:  This function is used to send a status request to SCTP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_sb_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLsbStaReq
(
Pst *pst,                 /* post structure */
SbMgmt *sta              /* status */
)
#else
PUBLIC S16 SmMiLsbStaReq(pst, sta)
Pst *pst;                 /* post structure */
SbMgmt *sta;             /* status */
#endif
{
   TRC3(SmMiLsbStaReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLsbStaReqMt[pst->selector])(pst, sta);
   RETVALUE(ROK);
} /* end of SmMiLsbStaReq */

/*
*
*       Fun:   Statistics request
*
*       Desc:  This function is used to request statistics from SCTP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_sb_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLsbStsReq
(
Pst *pst,                 /* post structure */
Action action,
SbMgmt *sts              /* statistics */
)
#else
PUBLIC S16 SmMiLsbStsReq(pst, action, sts)
Pst *pst;                 /* post structure */
Action action;
SbMgmt *sts;             /* statistics */
#endif
{
   TRC3(SmMiLsbStsReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLsbStsReqMt[pst->selector])(pst, action, sts);
   RETVALUE(ROK);
} /* end of SmMiLsbStsReq */

/*
*
*       Fun:   Control request
*
*       Desc:  This function is used to send control request to SCTP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_sb_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLsbCntrlReq
(
Pst *pst,                 /* post structure */
SbMgmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLsbCntrlReq(pst, cntrl)
Pst *pst;                 /* post structure */
SbMgmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLsbCntrlReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLsbCntrlReqMt[pst->selector])(pst, cntrl);
   RETVALUE(ROK);
} /* end of SmMiLsbCntrlReq */


#ifdef PTSMSBMILSB
/*
*
*       Fun:   Portable configure Request SCTP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_sb_ptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLsbCfgReq
(
Pst *pst,                   /* post structure */
SbMgmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLsbCfgReq(pst, cfg)
Pst *pst;                   /* post structure */
SbMgmt *cfg;               /* configure */
#endif
{
   TRC3(PtMiLsbCfgReq);
   UNUSED(pst);
   UNUSED(cfg);
   RETVALUE(ROK);
} /* end of PtMiLsbCfgReq */


/*
*
*       Fun:   Portable status Request SCTP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_sb_ptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLsbStaReq
(
Pst *pst,                   /* post structure */
SbMgmt *sta                /* status */
)
#else
PRIVATE S16 PtMiLsbStaReq(pst, sta)
Pst *pst;                   /* post structure */
SbMgmt *sta;               /* status */
#endif
{
   TRC3(PtMiLsbStaReq);
   UNUSED(pst);
   UNUSED(sta);
   RETVALUE(ROK);
} /* end of PtMiLsbStaReq */


/*
*
*       Fun:   Portable statistics Request SCTP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_sb_ptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLsbStsReq
(
Pst *pst,                   /* post structure */
Action action,
SbMgmt *sts                /* statistics */
)
#else
PRIVATE S16 PtMiLsbStsReq(pst, action, sts)
Pst *pst;                   /* post structure */
Action action;
SbMgmt *sts;               /* statistics */
#endif
{
   TRC3(PtMiLsbStsReq);
   UNUSED(pst);
   UNUSED(action);
   UNUSED(sts);
   RETVALUE(ROK);
} /* end of PtMiLsbStsReq */


/*
*
*       Fun:   Portable control Request SCTP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_sb_ptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLsbCntrlReq
(
Pst *pst,                   /* post structure */
SbMgmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLsbCntrlReq(pst, cntrl)
Pst *pst;                   /* post structure */
SbMgmt *cntrl;             /* control */
#endif
{
   TRC3(PtMiLsbCntrlReq);
   UNUSED(pst);
   UNUSED(cntrl);
   RETVALUE(ROK);
} /* end of PtMiLsbCntrlReq */

#endif

/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smsbptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLsbCfgCfm
(
Pst     *pst,          /* post structure */
SbMgmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLsbCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
SbMgmt *cfm;          /* configuration */
#endif
{
   Pst  sbPst;

#ifdef LTE_HO_SUPPORT
   PRIVATE U8    veSbSctSapCnt = 0;
#endif

   TRC2(SmMiLsbCfgCfm)

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
     switch(cfm->hdr.elmId.elmnt)
     {
        case STSBGEN:
           veSbCfg |=  VE_SB_GEN_CFG;
           break;
        case STSBTSAP:
           veSbCfg |=  VE_SB_TSAP_CFG;
           break;
        case STSBSCTSAP:
           veSbCfg |=  VE_SB_SCT_SAP_CFG;

#ifdef LTE_HO_SUPPORT
           veSbSctSapCnt++;
#endif
           break;
        default:
           break; 
     } /* end of switch */ 
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SCTP Cfg Cfm received\
for the element %d\n",cfm->hdr.elmId.elmnt));
#endif

#ifdef LTE_HO_SUPPORT
      if ((veSbCfg == VE_SM_SB_CONFIGURED) && (veSbSctSapCnt == 2))
#else
      if (veSbCfg == VE_SM_SB_CONFIGURED)
#endif
      {
         /* Initiate S1AP configuration after SCTP configuration. 
            So send EVTVES1APCFG */
         smCb.smState = VE_SM_STATE_SCTP_CFG_DONE;
         VE_FILL_PST(sbPst, SM_SB_PROC, ENTSM, ENTVE, EVTVES1APCFG, VE_POOL, SM_SELECTOR_LC);
         if(veSmSelfPstMsg(&sbPst) != ROK)
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SCTP CONFIGURATION \
SUCCESSFULLY PERFORMED. Failure at Self Pst.\n"));
#endif
         }
         else
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SCTP CONFIGURATION \
SUCCESSFULLY PERFORMED.\n"));
#endif
         } /* end of else part */

      } /* end of if statement  veSbCfg == VE_SM_SB_CONFIGURED */

   } /* end of if statement  cfm->cfm.status == LCM_PRIM_OK */

   RETVALUE(ROK);
} /* end of SmMiLsbCfgCfm */

/*
 *
 *       Fun:    SmMiLsbCntrlCfm - control confirm
 *
 *       Desc:   prints the control confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_sb_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLsbCntrlCfm
(
Pst         *pst,
SbMgmt     *cntrl
)
#else
PUBLIC S16 SmMiLsbCntrlCfm(pst, cntrl)
Pst         *pst;
SbMgmt     *cntrl;
#endif /* ANSI */
{
   Pst  sbPst;

   TRC2(SmMiLsbCntrlCfm)

   SM_SET_ZERO(&sbPst, sizeof(Pst));

   if (cntrl->hdr.elmId.elmnt == STSBTSAP) 
   {
      if (cntrl->cfm.status == LCM_PRIM_OK)
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF SCTP TSAP WITH TUCL \
                  SUCCESSFUL\n"));
#endif
         /* initiate the bind control request for S1AP and SCTP Saps */
         smBindSzToLSap();
      }
      else if (cntrl->cfm.status == LCM_PRIM_OK_NDONE)
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Bind of SCTP TSap with TUCL \
                  is in progress\n"));
#endif
      } /* end of if statement */
      else 
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Bind of SCTP TSap with TUCL \
                  failed\n"));
#endif
      }
   }
   if(cntrl->cfm.status == LCM_PRIM_OK)
   {
      if((smCfgCb.sbDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_SB_DBG_MASK))
      {
         /* Update DbgMsk */
         smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | VE_SM_SB_DBG_MASK);
         /* Trigger Debug Cntrl */
         veSmHdlEnbDebugs(EVTVEENBDEBUGS);
      }
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SmMiLsbCntrlCfm failed!"));
#endif

   }

   RETVALUE(ROK);
} /* end of SmMiLsbCntrlCfm() */

/*
 *
 *       Fun:    SmMiLsbStsCfm - statistics confirm
 *
 *       Desc:   prints the statistics confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_sb_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLsbStsCfm
(
Pst         *pst,
SbMgmt     *sts
)
#else
PUBLIC S16 SmMiLsbStsCfm(pst, sts)
Pst         *pst;
SbMgmt     *sts;
#endif /* ANSI */
{
   TRC2(SmMiLsbStsCfm)

   RETVALUE(ROK);
} /* end of SmMiLsbStsCfm() */

/*
 *
 *       Fun:    SmMiLsbStaCfm - status confirm
 *
 *       Desc:   prints the status confirm 
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_sb_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLsbStaCfm
(
Pst         *pst,
SbMgmt     *sta
)
#else
PUBLIC S16 SmMiLsbStaCfm(pst, sta)
Pst         *pst;
SbMgmt     *sta;
#endif /* ANSI */
{
   TRC2(SmMiLsbStaCfm)

   RETVALUE(ROK);
} /* end of SmMiLsbStaCfm() */

/*
 *
 *       Fun:    SmMiLsbStaInd - status indication
 *
 *       Desc:   prints the status indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_sb_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLsbStaInd
(
Pst         *pst,
SbMgmt     *ind
)
#else
PUBLIC S16 SmMiLsbStaInd(pst, ind)
Pst         *pst;
SbMgmt     *ind;
#endif /* ANSI */
{
   TRC2(SmMiLsbStaInd)

   RETVALUE(ROK);
} /* end of SmMiLsbStaInd() */

/*
 *
 *       Fun:    SmMiLsbTrcInd - trace indication
 *
 *       Desc:   prints the trace indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_sb_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLsbTrcInd
(
Pst         *pst,
SbMgmt     *trc
)
#else
PUBLIC S16 SmMiLsbTrcInd(pst, trc)
Pst         *pst;
SbMgmt     *trc;
#endif /* ANSI */
{
   TRC2(SmMiLsbTrcInd)

   RETVALUE(ROK);
} /* end of SmMiLsbTrcInd() */

/********************************************************************30**
  
         End of file:     ve_sm_sb_ptmi.c@@/main/2 - Wed Dec  1 08:44:38 2010
  
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

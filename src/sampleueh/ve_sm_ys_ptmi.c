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
  
     Name:     Stack Manager - portable LTE Convergence Layer
  
     Type:     C source file
  
     Desc:     Code for LTE Convergence Layer  layer management
               primitives. 
              
     File:     ve_sm_ys_ptmi.c
  
     Sid:      ve_sm_ys_ptmi.c@@/main/2 - Wed Dec  1 08:44:41 2010

     Prg:      an
  
*********************************************************************21*/

/*
  
Layer management provides the necessary functions to control and
monitor the condition of each protocol layer.

The following functions are provided in this file:

     SmMiLysCfgReq      Configure Request
     SmMiLysStaReq      Status Request
     SmMiLysStsReq      Statistics Request
     SmMiLysCntrlReq    Control Request
   
It is assumed that the following functions are provided in the
stack management body files:

     SmMiLysStaInd      Status Indication
     SmMiLysStaCfm      Status Confirm
     SmMiLysStsCfm      Statistics Confirm
     SmMiLysTrcInd      Trace Indication
   
*/   


/* header include files (.h) */
  
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "cm5.h"           /* common timer */
#include "ssi.h"           /* system services */
#include "cm_inet.h"       /* common sockets */
#include "cm_tpt.h"
#include "cm_lte.h"
#include "lys.h"           /* YS layer management */
#include "cm_err.h"        /* common error */
#include "ve_sm_init.h"
#include "ve.h"

/* header/extern include files (.x) */
  
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* common timer */
#include "cm_inet.x"       /* common sockets */
#include "cm_tpt.x"        /* common ss7 */
#include "cm_lib.x"
#include "cm_lte.x"
#include "lys.x"           /* ys layer management */
#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif

#define  MAXYSMI  2

PRIVATE U8 veYsCfg = 0;
PRIVATE U8 veNumTfuSaps = 0;

#ifndef  LCSMYSMILYS
#define  PTSMYSMILYS
#else
#ifndef   YS
#define  PTSMYSMILYS
#endif
#endif

#ifdef PTSMYSMILYS
PRIVATE S16 PtMiLysCfgReq    ARGS((Pst *pst, YsMngmt *cfg));
PRIVATE S16 PtMiLysCntrlReq  ARGS((Pst *pst, YsMngmt *cntrl));
PRIVATE S16 PtMiLysStaReq    ARGS((Pst *pst, YsMngmt *sta));
#endif


/*
the following matrices define the mapping between the primitives
called by the layer management interface of TCP UDP Convergence Layer
and the corresponding primitives in TUCL
 
The parameter MAXYSMI defines the maximum number of layer manager
entities on top of TUCL . There is an array of functions per primitive
invoked by TCP UDP Conbvergence Layer. Every array is MAXYSMI long
(i.e. there are as many functions as the number of service users).

The dispatcysng is performed by the configurable variable: selector.
The selector is configured during general configuration.

The selectors are:

   0 - loosely coupled (#define LCSMYSMILYS) 2 - Lys (#define YS)

*/


/* Configuration request primitive */
 
PRIVATE LysCfgReq SmMiLysCfgReqMt[MAXYSMI] =
{
#ifdef LCSMYSMILYS
   cmPkLysCfgReq,          /* 0 - loosely coupled  */
#else
   PtMiLysCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef YS
   YsMiLysCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLysCfgReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Control request primitive */
 
PRIVATE LysCntrlReq SmMiLysCntrlReqMt[MAXYSMI] =
{
#ifdef LCSMYSMILYS
   cmPkLysCntrlReq,          /* 0 - loosely coupled  */
#else
   PtMiLysCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef YS
   YsMiLysCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLysCntrlReq,          /* 1 - tightly coupled, portable */
#endif
};




/* Status request primitive */
 
PRIVATE LysStaReq SmMiLysStaReqMt[MAXYSMI] =
{
#ifdef LCSMYSMILYS
   cmPkLysStaReq,          /* 0 - loosely coupled  */
#else
   PtMiLysStaReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef YS
   YsMiLysStaReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLysStaReq,          /* 1 - tightly coupled, portable */
#endif
};


/*
*     layer management interface functions 
*/
 
/*
*
*       Fun:   Configuration request
*
*       Desc:  Tyss function is used to configure  TUCL
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_ys_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLysCfgReq
(
Pst *spst,                /* post structure */
YsMngmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLysCfgReq(spst, cfg)
Pst *spst;                /* post structure */   
YsMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLysCfgReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLysCfgReqMt[spst->selector])(spst, cfg); 
   RETVALUE(ROK);
} /* end of SmMiLysCfgReq */



/*
*
*       Fun:   Control request
*
*       Desc:  This function is used to send control request to 
*              TUCL
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_ys_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLysCntrlReq
(
Pst *spst,                 /* post structure */
YsMngmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLysCntrlReq(spst, cntrl)
Pst *spst;                 /* post structure */   
YsMngmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLysCntrlReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLysCntrlReqMt[spst->selector])(spst, cntrl); 
   RETVALUE(ROK);
} /* end of SmMiLysCntrlReq */


/*
*
*       Fun:   Status request
*
*       Desc:  Tyss function is used to send a status request to 
*              TUCL
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_ys_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLysStaReq
(
Pst *spst,                /* post structure */
YsMngmt *sta              /* status */
)
#else
PUBLIC S16 SmMiLysStaReq(spst, sta)
Pst *spst;                /* post structure */   
YsMngmt *sta;             /* status */
#endif
{
   TRC3(SmMiLysStaReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLysStaReqMt[spst->selector])(spst, sta); 
   RETVALUE(ROK);
} /* end of SmMiLysStaReq */

#ifdef PTSMYSMILYS

/*
 *             Portable Functions
 */

/*
*
*       Fun:   Portable configure Request- TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_ys_ptmi.c
*
*/
  
#ifdef ANSI
PRIVATE S16 PtMiLysCfgReq
(
Pst *spst,                  /* post structure */
YsMngmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLysCfgReq(spst, cfg)
Pst *spst;                  /* post structure */
YsMngmt *cfg;               /* configure */
#endif
{
  TRC3(PtMiLysCfgReq) 

  UNUSED(spst);
  UNUSED(cfg);

  RETVALUE(ROK);
} /* end of PtMiLysCfgReq */


/*
*
*       Fun:   Portable control Request TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_ys_ptmi.c
*
*/
#ifdef ANSI
PRIVATE S16 PtMiLysCntrlReq
(
Pst *spst,                  /* post structure */
YsMngmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLysCntrlReq(spst, cntrl)
Pst *spst;                  /* post structure */
YsMngmt *cntrl;             /* control */
#endif
{
  TRC3(PtMiLysCntrlReq)

  UNUSED(spst);
  UNUSED(cntrl);

 RETVALUE(ROK);
} /* end of PtMiLysCntrlReq */


/*
*
*       Fun:   Portable status Request TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_ys_ptmi.c
*
*/
  
#ifdef ANSI
PRIVATE S16 PtMiLysStaReq
(
Pst *spst,                  /* post structure */
YsMngmt *sta                /* status */
)
#else
PRIVATE S16 PtMiLysStaReq(spst, sta)
Pst *spst;                  /* post structure */
YsMngmt *sta;               /* status */
#endif
{
  TRC3(PtMiLysStaReq);

  UNUSED(spst);
  UNUSED(sta);

 RETVALUE(ROK);
} /* end of PtMiLysStaReq */

#endif /* PTSMYSMILHI */

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
*       File:  ve_sm_ys_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLysCfgCfm
(
Pst     *pst,          /* post structure */
YsMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLysCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
YsMngmt *cfm;          /* configuration */
#endif
{
   Pst      ysPst;

   TRC2(SmMiLysCfgCfm);

   SM_SET_ZERO(&ysPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            veYsCfg |= VE_YS_GEN_CFG;
            break;      
         case STTFUSAP:
            veNumTfuSaps++;
            if (veNumTfuSaps == 2)
            {
               veNumTfuSaps = 0;
               veYsCfg |= VE_YS_TFU_SAP_CFG;
            }
            break;
         case STCTFSAP:
            veYsCfg |= VE_YS_CTF_SAP_CFG;
            break;
         default:
            break;
      } /* end of switch statement */
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "CL Cfg Cfm received\
               for the element %d\n",cfm->hdr.elmId.elmnt));
#endif

      if (veYsCfg == VE_SM_YS_CONFIGURED)
      {
         smCb.smState = VE_SM_STATE_CL_CFG_DONE;
         VE_FILL_PST(ysPst, SM_VE_PROC, ENTSM, ENTVE, EVTVEENBDEBUGS, VE_POOL, SM_SELECTOR_LC);

         if(veSmSelfPstMsg(&ysPst) != ROK)
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "CL CONFIGURATION\
                     SUCCESSFULLY PERFORMED. Failure at Self Pst.\n"));
#endif
         }
         else
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "CL CONFIGURATION\
                     SUCCESSFULLY PERFORMED. Self Pst done to initiate next cfg.\n"));
#endif
         } /* end of else part */

      }  /* end of veYsCfg == VE_SM_YS_CONFIGURED */
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "CL CONFIGURATION CFM NOK\n"));
#endif
   }

   RETVALUE(ROK);
} /* end of SmMiLysCfgCfm */

/*
*
*       Fun:   Control Confirm
*
*       Desc:  This function is used by to present control confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_ys_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLysCntrlCfm
(
Pst     *pst,          /* post structure */
YsMngmt *cntrl         /* control */
)
#else
PUBLIC S16 SmMiLysCntrlCfm(pst, cntrl)
Pst     *pst;          /* post structure */
YsMngmt *cntrl;        /* control */
#endif
{
   TRC2(SmMiLysCntrlCfm);

#ifdef VE_PICO
   if (cntrl->cfm.status == LCM_PRIM_OK)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "CL Cntrl Cfm received with \
               status OK"));
#endif
      if ((smCfgCb.ysDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_YS_DBG_MASK))
      {
         /* Update DbgMsk */
         smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | VE_SM_YS_DBG_MASK);
         /* Trigger Debug Cntrl */
         veSmHdlEnbDebugs(EVTVEENBDEBUGS);
      }
   } /* end of if statement */
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "CL Cntrl Cfm received with \
               status NOK"));
#endif
   } /* end of if statement */
#endif /* VE_PICO */ 
   
   RETVALUE(ROK);
} /* end of SmMiLysCntrlCfm */

/********************************************************************30**
  
         End of file:     ve_sm_ys_ptmi.c@@/main/2 - Wed Dec  1 08:44:41 2010
  
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

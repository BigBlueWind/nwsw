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
  
     Name:     Stack Manager - portable TUCL
  
     Type:     C source file
  
     Desc:     Code for TCP UDP Convergence Layer  layer management
               primitives. 
              
     File:     ve_sm_hi_ptmi.c
  
     Sid:      ve_sm_hi_ptmi.c@@/main/2 - Wed Dec  1 08:44:32 2010

     Prg:      an
  
*********************************************************************21*/

/*
  
Layer management provides the necessary functions to control and
monitor the condition of each protocol layer.

The following functions are provided in this file:

     SmMiLhiCfgReq      Configure Request
     SmMiLhiStaReq      Status Request
     SmMiLhiStsReq      Statistics Request
     SmMiLhiCntrlReq    Control Request
   
It is assumed that the following functions are provided in the
stack management body files:

     SmMiLhiStaInd      Status Indication
     SmMiLhiStaCfm      Status Confirm
     SmMiLhiStsCfm      Statistics Confirm
     SmMiLhiTrcInd      Trace Indication
   
*/   


/* header include files (.h) */
  
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "cm5.h"           /* common timer */
#include "ssi.h"           /* system services */
#include "cm_inet.h"       /* common sockets */
#include "cm_tpt.h"        /* system services */
#include "lhi.h"           /* HI layer management */
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
#include "lhi.x"           /* hi layer management */
#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif


#define  MAXHIMI  2

#ifndef  LCSMHIMILHI
#define  PTSMHIMILHI
#else
#ifndef   HI
#define  PTSMHIMILHI
#endif
#endif

PRIVATE U8 veHiCfg = 0;
PRIVATE U8 veHiTSapCnt = 0;

#ifdef PTSMHIMILHI
PRIVATE S16 PtMiLhiCfgReq    ARGS((Pst *pst, HiMngmt *cfg));
PRIVATE S16 PtMiLhiStsReq    ARGS((Pst *pst, Action action, HiMngmt *sts));
PRIVATE S16 PtMiLhiCntrlReq  ARGS((Pst *pst, HiMngmt *cntrl));
PRIVATE S16 PtMiLhiStaReq    ARGS((Pst *pst, HiMngmt *sta));
#endif


/*
the following matrices define the mapping between the primitives
called by the layer management interface of TCP UDP Convergence Layer
and the corresponding primitives in TUCL
 
The parameter MAXHIMI defines the maximum number of layer manager
entities on top of TUCL . There is an array of functions per primitive
invoked by TCP UDP Conbvergence Layer. Every array is MAXHIMI long
(i.e. there are as many functions as the number of service users).

The dispatching is performed by the configurable variable: selector.
The selector is configured during general configuration.

The selectors are:

   0 - loosely coupled (#define LCSMHIMILHI) 2 - Lhi (#define HI)

*/


/* Configuration request primitive */
 
PRIVATE LhiCfgReq SmMiLhiCfgReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiCfgReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiCfgReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Control request primitive */
 
PRIVATE LhiCntrlReq SmMiLhiCntrlReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiCntrlReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiCntrlReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Statistics request primitive */
 
PRIVATE LhiStsReq SmMiLhiStsReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiStsReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiStsReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiStsReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiStsReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Status request primitive */
 
PRIVATE LhiStaReq SmMiLhiStaReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiStaReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiStaReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiStaReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiStaReq,          /* 1 - tightly coupled, portable */
#endif
};


/*
*     layer management interface functions 
*/
 
/*
*
*       Fun:   Configuration request
*
*       Desc:  This function is used to configure  TUCL
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_hi_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiCfgReq
(
Pst *spst,                /* post structure */
HiMngmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLhiCfgReq(spst, cfg)
Pst *spst;                /* post structure */   
HiMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLhiCfgReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiCfgReqMt[spst->selector])(spst, cfg); 
   RETVALUE(ROK);
} /* end of SmMiLhiCfgReq */



/*
*
*       Fun:   Statistics request
*
*       Desc:  This function is used to request statistics from 
*              TUCL
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_hi_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiStsReq
(
Pst *spst,                /* post structure */
Action action,
HiMngmt *sts              /* statistics */
)
#else
PUBLIC S16 SmMiLhiStsReq(spst, action, sts)
Pst *spst;                /* post structure */   
Action action;
HiMngmt *sts;             /* statistics */
#endif
{
   TRC3(SmMiLhiStsReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiStsReqMt[spst->selector])(spst, action, sts); 
   RETVALUE(ROK);
} /* end of SmMiLhiStsReq */


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
*       File:     ve_sm_hi_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiCntrlReq
(
Pst *spst,                 /* post structure */
HiMngmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLhiCntrlReq(spst, cntrl)
Pst *spst;                 /* post structure */   
HiMngmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLhiCntrlReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiCntrlReqMt[spst->selector])(spst, cntrl); 
   RETVALUE(ROK);
} /* end of SmMiLhiCntrlReq */


/*
*
*       Fun:   Status request
*
*       Desc:  This function is used to send a status request to 
*              TUCL
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_hi_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiStaReq
(
Pst *spst,                /* post structure */
HiMngmt *sta              /* status */
)
#else
PUBLIC S16 SmMiLhiStaReq(spst, sta)
Pst *spst;                /* post structure */   
HiMngmt *sta;             /* status */
#endif
{
   TRC3(SmMiLhiStaReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiStaReqMt[spst->selector])(spst, sta); 
   RETVALUE(ROK);
} /* end of SmMiLhiStaReq */

#ifdef PTSMHIMILHI

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
*       File:     ve_sm_hi_ptmi.c
*
*/
  
#ifdef ANSI
PRIVATE S16 PtMiLhiCfgReq
(
Pst *spst,                  /* post structure */
HiMngmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLhiCfgReq(spst, cfg)
Pst *spst;                  /* post structure */
HiMngmt *cfg;               /* configure */
#endif
{
  TRC3(PtMiLhiCfgReq) 

  UNUSED(spst);
  UNUSED(cfg);

  RETVALUE(ROK);
} /* end of PtMiLhiCfgReq */



/*
*
*       Fun:   Portable statistics Request TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_hi_ptmi.c
*
*/
#ifdef ANSI
PRIVATE S16 PtMiLhiStsReq
(
Pst *spst,                  /* post structure */
Action action,
HiMngmt *sts                /* statistics */
)
#else
PRIVATE S16 PtMiLhiStsReq(spst, action, sts)
Pst *spst;                  /* post structure */
Action action;
HiMngmt *sts;               /* statistics */
#endif
{
  TRC3(PtMiLhiStsReq) 

  UNUSED(spst);
  UNUSED(action);
  UNUSED(sts);

  RETVALUE(ROK);
} /* end of PtMiLhiStsReq */


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
*       File:     ve_sm_hi_ptmi.c
*
*/
#ifdef ANSI
PRIVATE S16 PtMiLhiCntrlReq
(
Pst *spst,                  /* post structure */
HiMngmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLhiCntrlReq(spst, cntrl)
Pst *spst;                  /* post structure */
HiMngmt *cntrl;             /* control */
#endif
{
  TRC3(PtMiLhiCntrlReq)

  UNUSED(spst);
  UNUSED(cntrl);

  RETVALUE(ROK);
} /* end of PtMiLhiCntrlReq */


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
*       File:     ve_sm_hi_ptmi.c
*
*/
  
#ifdef ANSI
PRIVATE S16 PtMiLhiStaReq
(
Pst *spst,                  /* post structure */
HiMngmt *sta                /* status */
)
#else
PRIVATE S16 PtMiLhiStaReq(spst, sta)
Pst *spst;                  /* post structure */
HiMngmt *sta;               /* status */
#endif
{
  TRC3(PtMiLhiStaReq);

  UNUSED(spst);
  UNUSED(sta);

  RETVALUE(ROK);
} /* end of PtMiLhiStaReq */

#endif /* PTSMHIMILHI */

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
*       File:  smhiptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiCfgCfm
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLhiCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* configuration */
#endif
{
   Pst  hiPst;

   TRC2(SmMiLhiCfgCfm)
  
  /*-- Update Event --*/
   UNUSED(pst);

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch(cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            veHiCfg |= VE_HI_GEN_CFG;
            break;
         case STTSAP:
            veHiCfg |= VE_HI_TSAP_CFG;
            veHiTSapCnt++;
            break;
         default:
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "TUCL Cfg Cfm received\
with invalid element %d\n",cfm->hdr.elmId.elmnt));
#endif
            break;
      } /* end of switch statement */
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "TUCL Cfg Cfm received\
for the element %d\n",cfm->hdr.elmId.elmnt));
#endif

      /* Wait for two Tsap confirms one for SCTP provider Sap
         and the other egtp provider Sap */
      if (veHiCfg == VE_SM_HI_CONFIGURED && veHiTSapCnt == 2)
      {
         /* Initiate SCTP configuration after TUCL configuration. 
            So send EVTVESCTPCFG */
         smCb.smState = VE_SM_STATE_TUCL_CFG_DONE;
#ifdef VE_SB_SCTP
         VE_FILL_PST(hiPst, SM_HI_PROC, ENTSM, ENTVE, EVTVESCTPCFG, VE_POOL, SM_SELECTOR_LC);
#else
         VE_FILL_PST(hiPst, SM_HI_PROC, ENTSM, ENTVE, EVTVES1APCFG, VE_POOL, SM_SELECTOR_LC);
#endif
         if(veSmSelfPstMsg(&hiPst) != ROK)
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "TUCL CONFIGURATION\
SUCCESSFULLY PERFORMED. Failure at Self Pst.\n"));
#endif
         }
         else
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "TUCL CONFIGURATION \
SUCCESSFULLY PERFORMED. \n"));
#endif
         } /* end of else part */

      } /* end of if statement  veHiCfg == VE_SM_HI_CONFIGURED */

   } /* end of if statement  cfm->cfm.status == LCM_PRIM_OK */

   RETVALUE(ROK);
} /* end of SmMiLhiCfgCfm */


/*
*
*       Fun:   Control Confirm
*
*       Desc:  This function is used by to present control confirm
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiCntrlCfm
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLhiCntrlCfm(pst, cfm)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* configuration */
#endif
{
   TRC2(SmMiLhiCntrlCfm);
   if(cfm->cfm.status == LCM_PRIM_OK)
   {
      if((smCfgCb.hiDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_HI_DBG_MASK))
      {
         /* Update DbgMsk */
         smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | VE_SM_HI_DBG_MASK);
         /* Trigger Debug Cntrl */
         veSmHdlEnbDebugs(EVTVEENBDEBUGS);
      }
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SmMiLhiCntrlCfm failed!"));
#endif

   }

   RETVALUE(ROK);
} /* end of SmMiLhiCntrlCfm */

/*
*
*       Fun:   Statistics Confirm
*
*       Desc:  This function is used by to present statistics confirm
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiStsCfm
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLhiStsCfm(pst, cfm)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* configuration */
#endif
{
    TRC2(SmMiLhiStsCfm)

    RETVALUE(ROK);
} /* end of SmMiLhiStsCfm */

/*
*
*       Fun:   Status Confirm
*
*       Desc:  This function is used by to present status confirm
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiStaCfm
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLhiStaCfm(pst, cfm)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* configuration */
#endif
{
    TRC2(SmMiLhiStaCfm)

    RETVALUE(ROK);
} /* end of SmMiLhiStaCfm */

/*
*
*       Fun:   Status Indication
*
*       Desc:  This function is used by to present status indication
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiStaInd
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLhiStaInd(pst, cfm)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* configuration */
#endif
{
    TRC2(SmMiLhiStaInd)

    RETVALUE(ROK);
} /* end of SmMiLhiStaInd*/

/*
*
*       Fun:   Trace Indication
*
*       Desc:  This function is used by to present trace indication
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiTrcInd
(
Pst     *pst,          /* post structure */
HiMngmt *cfm,           /* configuration */
Buffer  *mBuf
)
#else
PUBLIC S16 SmMiLhiTrcInd(pst, cfm, mBuf)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* configuration */
Buffer  *mBuf;
#endif
{
    TRC2(SmMiLhiStaInd)

    RETVALUE(ROK);
} /* end of SmMiLhiTrcInd*/

/********************************************************************30**
  
         End of file:     ve_sm_hi_ptmi.c@@/main/2 - Wed Dec  1 08:44:32 2010
  
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

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

     Name:     LTE-Uu Integrated Stack 
  
     Type:     C Source file
  
     Desc:     RRC Layer Manager Interface

     File:     ve_sm_kw_ptmi.c

     Sid:      ve_sm_kw_ptmi.c@@/main/2 - Wed Dec  1 08:44:34 2010

     Prg:      an

**********************************************************************/

#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */
#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_lte.h"       /* Common UMTS library             */
#include "cm_inet.h"
#include "cm_tpt.h"
#include "ve_sm_init.h"
#include "ve.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_lte.x"       /* Common UMTS library             */
#include "cm_inet.x"
#include "cm_tpt.x"
#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif


/* g++ compilation fix */
#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */

PRIVATE U8 veKwCfg = 0;

#define MAXNHMI  2         /* The size of the LC/TC matrix */

/* local defines */
#ifndef LCKWMILKW
#define PTSMLKW
#endif

#ifndef KW
#define PTSMLKW
#endif

#ifndef SM 
#define PTSMLKW
#endif

/* local typedefs */

/* local externs */

#ifdef PTSMLKW
/* forward references */
PRIVATE S16 PtMiLkwCfgReq   ARGS((Pst *pst, KwMngmt *cfg ));
PRIVATE S16 PtMiLkwCntrlReq   ARGS((Pst *pst, KwMngmt *cfg ));
#endif



PRIVATE LkwCfgReq smMiLkwCfgReqMt[] =
{
#ifdef LCKWMILKW
   cmPkLkwCfgReq,          /* 0 - loosely coupled - fc */
#else
   PtMiLkwCfgReq,          /* 0 - loosely coupled portable */
#endif /* LCKWMILKW */
#ifdef KW
   KwMiLkwCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLkwCfgReq,          /* 1 - tightly coupled portable */
#endif /* KW */
};


/* control request primitive */
PRIVATE LkwCntrlReq smMiLkwCntrlReqMt[] =
{
#ifdef LCKWMILKW 
   cmPkLkwCntrlReq,          /* 0 - loosely coupled - fc */
#else
   PtMiLkwCntrlReq,          /* 0 - tightly coupled, portable */
#endif /* LCKWMILKW */
#ifdef KW 
   KwMiLkwCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLkwCntrlReq,          /* 1 - tightly coupled, portable */
#endif /* RL */
};



/**************************************************************************
 *
 *      Fun:   Configuration request
 *
 *      Desc:  This function is used to configure RRC
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  ve_sm_kw_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 SmMiLkwCfgReq
(
Pst *pst,                 /* post structure */
KwMngmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLkwCfgReq(pst, cfg)
Pst *pst;                 /* post structure */
KwMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLkwCfgReq)
   /* jump to specific primitive depending on configured selector */
   (*smMiLkwCfgReqMt[pst->selector])(pst, cfg);
   RETVALUE(ROK);

} /* end SmMiLkwCfgReq */

/**************************************************************************
 *
 *      Fun:   Control request
 *
 *      Desc:  This function is used to send the control request RLC
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  ve_sm_kw_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 SmMiLkwCntrlReq
(
Pst *pst,                 /* post structure */
KwMngmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLkwCntrlReq(pst, cntrl)
Pst *pst;                 /* post structure */
KwMngmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLkwCntrlReq)
   /* jump to specific primitive depending on configured selector */
   (*smMiLkwCntrlReqMt[pst->selector])(pst, cntrl);
   RETVALUE(ROK);
} /* end of SmMiLkwCntrlReq */


#ifdef PTSMLKW
/**************************************************************************
 *
 *      Fun:   Portable Configuration request
 *
 *      Desc:  This function is used to send the configuration request RLC
 *             (Portable)
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  ve_sm_kw_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtMiLkwCfgReq
(
Pst *pst,                   /* post structure */
KwMngmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLkwCfgReq(pst, cfg)
Pst *pst;                   /* post structure */
KwMngmt *cfg;               /* configure */
#endif
{
  TRC3(PtMiLkwCfgReq);
  UNUSED(cfg);
  UNUSED(pst);
  /* Removed TRC2 macro */
  RETVALUE(ROK);
} /* end of PtMiLkwCfgReq */

/**************************************************************************
 *
 *      Fun:   Portable Control request
 *
 *      Desc:  This function is used to send the control request RLC
 *             (Portable)
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  ve_sm_kw_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtMiLkwCntrlReq
(
Pst *pst,                   /* post structure */
KwMngmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLkwCntrlReq(pst, cntrl)
Pst *pst;                   /* post structure */
KwMngmt *cntrl;             /* control */
#endif
{
  TRC3(PtMiLkwCntrlReq);

  UNUSED(cntrl);
  UNUSED(pst);
  /* Removed TRC2 macro */
  RETVALUE(ROK);
} /* end of PtMiLkwCntrlReq */


#endif /* PTSMLKW */


/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Managemer.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_kw_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLkwCfgCfm
(
Pst     *pst,          /* post structure */
KwMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLkwCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
KwMngmt *cfm;          /* configuration */
#endif
{
   Pst      kwPst;

   TRC2(SmMiLkwCfgCfm);

   SM_SET_ZERO(&kwPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            veKwCfg |= VE_KW_GEN_CFG;
            break;      
         case STCKWSAP:
            veKwCfg |= VE_KW_CKW_SAP_CFG;
            break;
         case STKWUSAP:
            veKwCfg |= VE_KW_KWU_SAP_CFG;
            break;
         case STRGUSAP: 
            veKwCfg |= VE_KW_RGU_SAP_CFG;
            break;
         default:
            break;
      } /* end of switch statement */
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RLC Cfg Cfm received\
               for the element %d\n",cfm->hdr.elmId.elmnt));
#endif

      if (veKwCfg == VE_SM_KW_CONFIGURED)
      {
         smCb.smState = VE_SM_STATE_RLC_CFG_DONE;
         VE_FILL_PST(kwPst, SM_KW_PROC, ENTSM, ENTVE, EVTVEMACCFG, VE_POOL, SM_SELECTOR_LC);

         if(veSmSelfPstMsg(&kwPst) != ROK)
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RLC CONFIGURATION \
                     SUCCESSFULLY PERFORMED. Failure at Self Pst.\n"));
#endif
         }
         else
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RLC CONFIGURATION \
                     SUCCESSFULLY PERFORMED.\n"));
#endif
         } /* end of else part */

      }  /* end of veKwCfg == VE_SM_KW_CONFIGURED */
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SmMiLkwCfgCfm: \
               CONFIGURATION CONFIRM NOK \n"));
#endif
   }

   RETVALUE(ROK);
} /* end of SmMiLkwCfgCfm */

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
*       File:  ve_sm_kw_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLkwCntrlCfm
(
Pst     *pst,          /* post structure */
KwMngmt *cntrl         /* control */
)
#else
PUBLIC S16 SmMiLkwCntrlCfm(pst, cntrl)
Pst     *pst;          /* post structure */
KwMngmt *cntrl;        /* control */
#endif
{
   Pst  kwPst;

   TRC2(SmMiLkwCntrlCfm);

   SM_SET_ZERO(&kwPst, sizeof(Pst));

   if (cntrl->cfm.status == LCM_PRIM_OK && cntrl->hdr.elmId.elmnt == STRGUSAP)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RLC TO \
MAC (RGU) SAP SUCCESSFUL\n"));
#endif
      /* Initiate control request for binding RRC to RLC */
      smBindNhToCkwSap(); 

   } /* end of if statement */
   if(cntrl->cfm.status == LCM_PRIM_OK)
   {
      if((smCfgCb.kwDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_KW_DBG_MASK))
      {
         /* Update DbgMsk */
         smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | VE_SM_KW_DBG_MASK);
         /* Trigger Debug Cntrl */
         veSmHdlEnbDebugs(EVTVEENBDEBUGS);
      }
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\n SmMiLkwCntrlCfm failed"));
#endif

   }

   RETVALUE(ROK);
} /* end of SmMiLkwCntrlCfm */


/*
*
*       Fun:   Sts Confirm
*
*       Desc:  This function is used by to present Sts confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_kw_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLkwStsCfm 
(
Pst     *pst,          /* post structure */
Action  action,        /* Action */
KwMngmt *sts           /* statistics */
)
#else
PUBLIC S16 SmMiLkwStsCfm(pst, action, sts)
Pst     *pst;          /* post structure */
Action  action;        /* Action */
KwMngmt *sts;          /*  statistics */
#endif
{
   TRC2(SmMiLkwStsCfm)

   UNUSED(pst);
   UNUSED(action);
   UNUSED(sts);

   RETVALUE(ROK);
} /* end of SmMiLkwStsCfm */


/*
*
*       Fun:   Status Confirm
*
*       Desc:  This function is used by to present Status confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_kw_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLkwStaCfm
(
Pst     *pst,          /* post structure */
KwMngmt *sta           /* Status */
)
#else
PUBLIC S16 SmMiLkwStaCfm(pst,sta)
Pst     *pst;          /* post structure */
KwMngmt *sta;          /* Status */
#endif  
{
   TRC2(SmMiLkwStaCfm)

   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* end of SmMiLkwStaCfm */


/*
*
*       Fun:   Status Ind
*
*       Desc:  This function is used by to present Status Ind
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_kw_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLkwStaInd
(
Pst     *pst,          /* post structure */
KwMngmt *usta           /* configuration */
)
#else
PUBLIC S16 SmMiLkwStaInd(pst, usta)
Pst     *pst;          /* post structure */
KwMngmt *usta;          /* configuration */
#endif
{
   TRC2(SmMiLkwStaInd)

   UNUSED(pst);
   UNUSED(usta);

   RETVALUE(ROK);
} /* end of SmMiLkwStaInd */


/*
*
*       Fun:   Trc Ind
*
*       Desc:  This function is used by to present Trc Ind
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_kw_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLkwTrcInd
(
Pst      *pst,          /* post structure */
KwMngmt  *trc,          /* trace indication */
Buffer   *mBuf          /* message buffer */
)
#else
PUBLIC S16 SmMiLkwTrcInd(pst,trc, mBuf)
Pst      *pst;          /* post structure */
KwMngmt  *trc;          /* trace indication */
Buffer   *mBuf;         /* message buffer */
#endif
{
   TRC2(SmMiLkwTrcInd)

   UNUSED(pst);
   UNUSED(trc);
   UNUSED(mBuf);

   RETVALUE(ROK);
} /* end of SmMiLkwTrcInd */


#ifdef __cplusplus
}
#endif /* cplusplus */

/********************************************************************30**
  
         End of file:     ve_sm_kw_ptmi.c@@/main/2 - Wed Dec  1 08:44:34 2010
  
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
